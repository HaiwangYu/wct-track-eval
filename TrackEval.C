/***
//  for protoDUNE SP evaluation
//  inefficiency (gaps), resolution, bias
//  Two input files for a certain angle, xz plane track. One is "ROI", one is SP results.
//  tan(theta_xz)/cosPhi = tan(theta_x'z') 
//  xz: collection plane (default) coordinate, x'z' induction plane coordinate
//  Phi: induction wire orientation relative to collection wire
***/

void TrackEval(const char* file0, const char* file1, const char* plane, double polar /*theta_xz*/)
{
    const double chargepermm = 5000; //# of electrons
    const double ind_base = 2349; //ADC
    const double col_base = 901;  //ADC
    const int toffset = 0; // ticks offset to align SP result with raw waveforms
    const double wire_pitch_col = 4.79; //mm for collection plane in wire geometry file for WCT simulation
    const double wire_pitch_ind = 4.71; //mm for induction plane 

    const double cos35d7 = TMath::Cos(35.7/180*TMath::Pi());
    const double sin35d7 = TMath::Sin(35.7/180*TMath::Pi());
    const TVector3 upitch(0, -sin35d7,  cos35d7);
    const TVector3 uwire (0,  cos35d7,  sin35d7);
    const TVector3 vpitch(0,  sin35d7,  cos35d7);
    const TVector3 vwire (0,  cos35d7, -sin35d7);
    const TVector3 wpitch(0, 0, 1);
    const TVector3 wwire (0, 1, 0);

    double rad = polar*TMath::Pi()/180.;
    const double track_length = 3600; //!!should be user-defined
    //TVector3 track(0, sin(rad), cos(rad)); // y-z plane
    TVector3 track(sin(rad), 0, cos(rad)); // z-x plane
    float uwcharge, vwcharge, wwcharge;
    if(polar != 90){
     uwcharge = TMath::Abs(chargepermm*wire_pitch_ind/(track*upitch));
     vwcharge = TMath::Abs(chargepermm*wire_pitch_ind/(track*vpitch));
     wwcharge = TMath::Abs(chargepermm*wire_pitch_col/(track*wpitch));
    }
    if(polar == 90){
     uwcharge = chargepermm*track_length;    
     vwcharge = chargepermm*track_length;    
     wwcharge = chargepermm*track_length;    
    }
    cout<<"Track polar: "<<polar<<" degree"<<endl;
    cout<<"Track charge: "<<chargepermm<<" /mm"<<endl;
    cout<<"U wire charge: "<<uwcharge<<endl;
    cout<<"V wire charge: "<<vwcharge<<endl;
    cout<<"W wire charge: "<<wwcharge<<endl;
    double charge = 0;
    double base = 0;
    if(strcmp("w",plane)==0) { charge = wwcharge; base = col_base; } 
    if(strcmp("u",plane)==0) { charge = uwcharge; base = ind_base; }
    if(strcmp("v",plane)==0) { charge = vwcharge; base = ind_base; } 
    
    TH1F* hres = new TH1F("hres","", 4000, -2, 2);
   
    TFile *froi = new TFile(file0, "READ");
    TFile *fsp = new TFile(file1, "READ");
    TString ss;
    TString sss;

    int count=0; /*how many ROIs*/
    int ineff_count=0; /*tiny charge within ROIs*/

    /* loop each APA*/
    for(int i=0; i<6; i++)
    {
        cout<<"APA: "<<i<<endl;
        ss.Form("h%s_orig%d", plane, i);
        sss.Form("h%s_gauss%d", plane, i);
        TH2F* hroi = (TH2F*)froi->Get(ss.Data());
        TH2F* hsp = (TH2F*)fsp->Get(sss.Data());
        
        std::vector<int> channel;
        std::vector<int> beg;
        std::vector<int> end;
       
        /* loop each channel, i.e. Y projection*/
        int nChannel = hroi->GetNbinsX();
        cout<<"nChannel: " << nChannel<<endl;
        for(int j=1; j<=nChannel; j++)
        {
            TH1D* h = hroi->ProjectionY("h", j, j, "");
            if (h->GetMaximum()<base+50 /*50 ADC*/) continue;
            channel.push_back(j);
            int maxbin = h->GetMaximumBin();
            int nticks = h->GetNbinsX();
            for (int k=0; k<maxbin; k++) // maxbin-k
            {
                if(h->GetBinContent(maxbin-k)<base+2 /* ADC as threshold*/)
                {
                    beg.push_back((maxbin-k+1-10)>1?(maxbin-k+1-10):1);
                    break;
                }
                if(k==maxbin-1) beg.push_back(1);
            }
            for (int m=0; m<=nticks-maxbin; m++) // maxbin+m
            {
                if(h->GetBinContent(maxbin+m)<base+2 /* ADC as threshold*/)
                {
                    end.push_back((maxbin+m-1+10)<nticks?(maxbin+m-1+10):nticks);
                    break;
                }
                if(m==nticks-maxbin) end.push_back(nticks);
            }
        }

        /*apply truth ROI to SP results*/
        for(auto ind=0; ind<channel.size(); ind++)
        {
            count ++;
            cout<<"ROI:"<<endl;
            //cout<<"channel: "<<channel[ind]<<" beg: "<<beg[ind]<<" end: "<<end[ind]<<endl;
            double content = hsp->Integral(channel[ind], channel[ind], beg[ind]-toffset, end[ind]-toffset);
            if(content<700 /*# of electrons, arbitray threshold*/) {     
                ineff_count++; 
                cout<<"channel: "<<channel[ind]<<" beg: "<<beg[ind]<<" end: "<<end[ind]<<endl;
                continue;}
            hres->Fill(content/charge-1);
        }
    }

     
    TCanvas *c = new TCanvas();
    hres->Draw();
    
    /* output results to txt files for U, V, W planes*/
    ofstream outputfile;
    TString filename;
    filename.Form("%splane.txt",plane);
    outputfile.open(filename.Data(), ios::out | ios::app);
     
    if(strcmp("w",plane)==0)
    {
    cout<<"True: "<<charge<<endl;
    cout<<"Ineff: "<<ineff_count*100./count<<"%"<<endl;
    /*quantile method to calculate peak+34.1%*/
    // two sided quantile
    double medium;
    double left, right;
    double xq = 0.5 - 0.341;
    hres->GetQuantiles(1, &left, &xq);
    xq = 0.5;
    hres->GetQuantiles(1, &medium, &xq);
    xq = 0.5 + 0.341;
    hres->GetQuantiles(1, &right, &xq);
    double res = TMath::Sqrt((TMath::Power(medium-left, 2) + TMath::Power(right-medium, 2))*0.5);
    cout<<"Bias: "<<medium*100<<"%"<<endl;
    cout<<"Res: "<<res*100<<"%"<<endl;
    outputfile << medium << " " << res << " " << ineff_count*1.0/count << endl;
    }
    else{
    hres->Rebin(10);//to allow a reasonable fit
    double peakpos = hres->GetBinCenter(hres->GetMaximumBin());  
    TF1 *f1 = new TF1("f1","gaus(0)", peakpos-2*hres->GetRMS(), peakpos+3*hres->GetRMS());
    hres->Fit("f1", "RIM");
    cout<<"True: "<<charge<<endl;
    cout<<"Bias: "<<f1->GetParameter(1)*100<<"%"<<endl;
    cout<<"Res: "<<f1->GetParameter(2)*100<<"%"<<endl;
    cout<<"Ineff: "<<ineff_count*100./count<<"%"<<endl;

    peakpos = f1->GetParameter(1);
    /*quantile method to calculate peak+34.1%*/
    // one side quantile, the other side suffers low charge tail 
    double peak_quantile = hres->Integral(1, hres->FindBin(peakpos))/hres->Integral();
    cout << "Peak quantile: " << peak_quantile << endl;
    double xq = peak_quantile + (1-peak_quantile)*0.341*2;
    cout << "Res quantile: " << xq << endl;
    double endvalue;
    hres->GetQuantiles(1, &endvalue, &xq);
    cout << endvalue << " " <<  peakpos << endl;
    cout<<"Bias: "<<peakpos*100<<"%"<<endl;
    cout << "Res (quantile method): " << (endvalue - peakpos)*100 <<"%"<< endl;
    outputfile << peakpos << " " << endvalue - peakpos << " " << ineff_count*1.0/count << endl;
    }
    outputfile.close();
}
