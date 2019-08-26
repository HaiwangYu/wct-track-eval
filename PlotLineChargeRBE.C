void PlotLineChargeRBE()
{
    float Ubias[14] = {0.011493, 0.006419, 0.009148, 0.005875, 0.006515, -0.000326, -0.011290, -0.040314, -0.063513, -0.106115, -0.162116, -0.228458, -0.37195, 0};
    float Ures[14] = {0.089963, 0.090907, 0.092719, 0.090419, 0.093008, 0.097193, 0.104472, 0.112373, 0.123970, 0.146242, 0.197366, 0.248028, 0.298863, 0};
    float Ueff[14] = {0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000883, 0.016757, 0.076077, 0.328093, 0};

    float Vbias[14] = {0.005687, 0.007252, 0.001749, 0.006452, 0.005404, 0.003354, -0.016645, -0.042496, -0.061916, -0.117819, -0.200783, -0.280389, -0.435324, 0};
    float Vres[14] = {0.098811, 0.100698, 0.096829, 0.104930, 0.100702, 0.111259, 0.117112, 0.123683, 0.139192, 0.174084, 0.242132, 0.291936, 0.327737, 0};
    float Veff[14] = {0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.004844, 0.083900, 0.245638, 0.578173, 0};

    float Wbias[14] = {0.023169, 0.023409, 0.024855, 0.027193, 0.025628, 0.027117, 0.027252, 0.027958, 0.024657, 0.022300, 0.017823, 0.0110151, 0.0103148, 0};
    float Wres[14] = {0.031339, 0.031223, 0.031633, 0.031602, 0.030399, 0.029554, 0.035335, 0.028650, 0.048993, 0.048470, 0.063173, 0.0908533, 0.0734137, 0};
    float Weff[14] = {0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0, 0, 0};


    string axis[14] = {"0(0)", "1(1.2)", "3(3.7)", "5(6.1)", "10(12)", "20(24)", "30(35)", "45(51)", "60(65)", "75(78)", "80(82)", "82(83.5)", "84(85.1)", "89(89.2)"};


    TH1F* hu = new TH1F("hu","", 14, 0, 14);
    hu->SetBarWidth(0.2);
    hu->SetFillColor(kGreen);
    hu->SetStats(0);
    
    TH1F* hv = new TH1F("hv","", 14, 0, 14);
    hv->SetBarWidth(0.2);
    hv->SetFillColor(kBlue);
    hv->SetBarOffset(0.2);
    hv->SetStats(0);

    TH1F* hw = new TH1F("hw","", 14, 0, 14);
    hw->SetBarWidth(0.2);
    hw->SetFillColor(kRed);
    hw->SetBarOffset(0.4);
    hw->SetStats(0);
    
    for(int i=1; i<=14; i++)
    {
        hu->SetBinContent(i, 100*Ubias[i-1]);
        hv->SetBinContent(i, 100*Vbias[i-1]);
        hv->GetXaxis()->SetBinLabel(i, axis[i-1].c_str());
        hw->SetBinContent(i, 100*Wbias[i-1]);
    }



    TH1F* hur = new TH1F("hur","", 14, 0, 14);
    hur->SetBarWidth(0.2);
    hur->SetFillColor(kGreen);
    hur->SetStats(0);

    TH1F* hvr = new TH1F("hvr","", 14, 0, 14);
    hvr->SetBarWidth(0.2);
    hvr->SetFillColor(kBlue);
    hvr->SetBarOffset(0.2);
    hvr->SetStats(0);

    TH1F* hwr = new TH1F("hwr","", 14, 0, 14);
    hwr->SetBarWidth(0.2);
    hwr->SetFillColor(kRed);
    hwr->SetBarOffset(0.4);
    hwr->SetStats(0);
    
    for(int i=1; i<=14; i++)
    {
        hur->SetBinContent(i, 100*Ures[i-1]);
        hvr->SetBinContent(i, 100*Vres[i-1]);
        hvr->GetXaxis()->SetBinLabel(i, axis[i-1].c_str());
        hwr->SetBinContent(i, 100*Wres[i-1]);
    }


    TH1F* hue = new TH1F("hue","", 14, 0, 14);
    hue->SetBarWidth(0.2);
    hue->SetFillColor(kGreen);
    hue->SetStats(0);

    TH1F* hve = new TH1F("hve","", 14, 0, 14);
    hve->SetBarWidth(0.2);
    hve->SetFillColor(kBlue);
    hve->SetBarOffset(0.2);
    hve->SetStats(0);

    TH1F* hwe = new TH1F("hwe","", 14, 0, 14);
    hwe->SetBarWidth(0.2);
    hwe->SetFillColor(kRed);
    hwe->SetBarOffset(0.4);
    hwe->SetStats(0);
    
    for(int i=1; i<=14; i++)
    {
        hue->SetBinContent(i, -100*Ueff[i-1]);
        hve->SetBinContent(i, -100*Veff[i-1]);
        hve->GetXaxis()->SetBinLabel(i, axis[i-1].c_str());
        hwe->SetBinContent(i, -100*Weff[i-1]);
    }



    TCanvas* c = new TCanvas();
    gPad->SetBottomMargin(0.35);
    c->Divide(1, 3, 0, 0.1);
    c->cd(1);
    gPad->SetLeftMargin(0.1);
    gPad->SetRightMargin(0.1);
    hv->GetYaxis()->SetRangeUser(-52, 5);
    hv->Draw("b");
    hv->GetYaxis()->SetTitle("Bias [\%]");
    hv->GetXaxis()->SetTitle("Angle to Z axis [degree]");
    hv->GetXaxis()->SetLabelSize(0.06);
    hv->GetXaxis()->SetTitleSize(0.07);
    hv->GetYaxis()->SetNdivisions(405);
    hv->GetYaxis()->SetLabelSize(0.1);
    hv->GetYaxis()->SetTitleSize(0.12);
    hv->GetYaxis()->SetTitleOffset(0.3);
    hu->Draw("b same");
    hw->Draw("b same");
    c->cd(2);
    gPad->SetLeftMargin(0.1);
    gPad->SetRightMargin(0.1);
    hvr->Draw("b");
    hvr->GetYaxis()->SetRangeUser(1, 48);
    hvr->GetYaxis()->SetTitle("Resolution [\%]");
    hvr->GetXaxis()->SetTitle("Angle to Z axis [degree]");
    hvr->GetXaxis()->SetLabelSize(0.06);
    hvr->GetXaxis()->SetTitleSize(0.07);
    hvr->GetYaxis()->SetNdivisions(405);
    hvr->GetYaxis()->SetLabelSize(0.1);
    hvr->GetYaxis()->SetTitleSize(0.12);
    hvr->GetYaxis()->SetTitleOffset(0.3);
    hur->Draw("b same");
    hwr->Draw("b same");
    c->cd(3);
    gPad->SetLeftMargin(0.1);
    gPad->SetRightMargin(0.1);
    gPad->SetBottomMargin(0.25);
    hve->Draw("b");
    hve->GetYaxis()->SetRangeUser(-68, 5);
    hve->GetYaxis()->SetTitle("Inefficiency [\%]");
    hve->GetXaxis()->SetTitle("#theta_{xz} (#theta_{x'z'}) [degree]");
    hve->GetXaxis()->SetLabelSize(0.1);
    hve->GetXaxis()->SetTitleSize(0.1);
    hve->GetYaxis()->SetNdivisions(505);
    hve->GetYaxis()->SetLabelSize(0.07);
    hve->GetYaxis()->SetTitleSize(0.08);
    hve->GetYaxis()->SetTitleOffset(0.45);
    hue->Draw("b same");
    hwe->Draw("b same");


    

}
