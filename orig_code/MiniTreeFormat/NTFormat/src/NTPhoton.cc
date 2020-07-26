
#include "../interface/NTPhoton.h"
using namespace TopTree;



NTPhoton::NTPhoton ():
  p4(0.0, 0.0, 0.0, 0.0), 
  TrkIso(-999.), 
  ECaloIso(-999.), 
  HCaloIso(-999.),
  isEB(0),
  e1x5(-999.),
  e2x5(-999.),
  e3x3(-999.),
  e5x5(-999.)
{
}


NTPhoton::~NTPhoton ()
{
}

void
NTPhoton::Reset ()
{
  p4.SetPxPyPzE (0.0, 0.0, 0.0, 0.0);
  ECaloIso = -999.;
  HCaloIso = -999.;
  TrkIso = -999.;
  isEB = 0;
  e1x5 = -999.;
  e2x5 = -999.;
  e3x3 = -999.;
  e5x5 = -999.;
}

void NTPhoton::PrintInfo(std::ostream & os){
        PrintP4(p4, os); 
        os << "\n";
        os << "ECaloIso " << ECaloIso << "  " ;
        os << "HCaloIso " << HCaloIso << "  " ;
        os << "TrkIso "   << TrkIso << "  " ;
        os << "isEB "     << isEB << "  " ;
        os << "e1x5 "     << e1x5 << "  " ;
        os << "e2x5 "     << e2x5 << "  " ;
        os << "e3x3 "     << e3x3 << "  " ;
        os << "e5x5 "     << e5x5 << "  " ;
        os << "\n";
        os << endl;
}

