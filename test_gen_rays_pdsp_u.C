#include "TVector3.h"

#include <fstream>
#include <iomanip>
#include <sstream>
#include <iostream>

using namespace std;

double project_polar(const TVector3& t, const TVector3&n, const TVector3&z) {
    auto tp = t-t.Dot(n)*n;
    return TMath::ACos(tp.Dot(z)/tp.Mag()/z.Mag());
}

void test_gen_rays_pdsp_u(double polar_u_degree = 80) {

    double polar_u = polar_u_degree * TMath::Pi()/180.;

    double u_from_vert = 37.5 * TMath::Pi()/180.;
    double v_from_vert = -37.5 * TMath::Pi()/180.;
    double meter = 1.0;

    double track_length = 3*meter;

    TVector3 x(1, 0, 0);
    TVector3 zw(0, 0, 1);
    TVector3 zu(0, -sin(u_from_vert), cos(u_from_vert));
    TVector3 zv(0, -sin(v_from_vert), cos(v_from_vert));

    auto nw = zw.Cross(x);
    auto nu = zw.Cross(x);
    auto nv = zv.Cross(x);


    int Nreplica = 1;



    TVector3 track_start(0*meter, 3*meter, 3.5*meter);
    TVector3 track = cos(polar_u) * zu + sin(polar_u) * x;
    track = track_length*track;
    TVector3 track_end = track_start + track;
    // track.Print();

    auto polar_w = project_polar(track, nw, zw);
    auto polar_v = project_polar(track, nv, zv);

    cout
    << "  polar_u: " << polar_u * 180/TMath::Pi()
    << ", polar_v: " << polar_v * 180/TMath::Pi()
    << ", polar_w: " << polar_w * 180/TMath::Pi()
    << endl;


    stringstream out_name;
    out_name
    << "u_"
    << std::fixed << std::setprecision(0) << polar_u_degree
    << ".json";
    ofstream fout(out_name.str());
    
    fout << "local wc = import 'wirecell.jsonnet';\n";
    fout << "{\n  rays: [\n";

    for(int i=0;i<Nreplica;++i) {
        fout << "    {\n";
        fout << "      head: wc.point("<< track_start.X() <<", " << track_start.Y() << ", " << track_start.Z() <<", wc.m),\n";
        fout << "      tail: wc.point("<< track_end.X() <<", " << track_end.Y() << ", " << track_end.Z() <<", wc.m),\n";
        fout << "    },\n";
    }

    fout << "  ]\n}\n";
    fout.close();
}
