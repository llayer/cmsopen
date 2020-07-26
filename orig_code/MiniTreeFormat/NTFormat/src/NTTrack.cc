#include "../interface/NTTrack.h"
using namespace TopTree;


NTTrack::NTTrack ():
    charge(0),
    Chi2(-999.),
    d0(-999.),
    d0Error(-999.),
    dxy(-999.),
    dxy_BS(-999.),
    dxyError(-999.),
    dz(-999.),
    dzError(-999.)
{
}

NTTrack::~NTTrack ()
{
}

void
NTTrack::Reset ()
{
    p4.SetPxPyPzE(0.0,0.0,0.0,0.0);
    charge = 0;
    Chi2 = -999.;
    d0 = -999.;
    d0Error = -999.;
    dxy = -999.;
    dxy_BS = -999.;
    dxyError = -999.;
    dz = -999.;
    dzError = -999.;
}

void NTTrack::Dump(std::ostream & os){
        os << "Object: Track \n";
        PrintP4(p4, os);
        os << " charge = " <<  charge << " ";
        os << " Chi2 = " << Chi2 << " ";
        os << " d0 = " << d0 << " ";
        os << " d0Error = " << d0Error << " ";
        os << " dxy = " << dxy_BS << " ";
        os << " dxyError = " << dxyError << " ";
        os << " dz = " << dz << " ";
        os << " dzError = " << dzError << " ";
        os << endl;
}

