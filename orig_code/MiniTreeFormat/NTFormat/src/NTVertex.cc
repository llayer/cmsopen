#include "../interface/NTVertex.h"
using namespace TopTree;


NTVertex::NTVertex ():
p3(0.0,0.0,0.0), 
chi2 (-999), 
isFake (-1), 
isValid (-1), 
ndof (0), 
rho (-999), 
normalizedChi2 (-999),
NumberOfTracks(0)
{
}

NTVertex::~NTVertex ()
{
}

void
NTVertex::Reset ()
{
  p3.SetXYZ(0.0,0.0,0.0);
  chi2 = -999;
  isFake = -1;
  isValid = -1;
  ndof = 0;
  rho = -999;
  normalizedChi2 = -999.;
  NumberOfTracks = 0;
}

void NTVertex::Dump(std::ostream & os){
        os << "Object: Vertex \n";
        os << " vertex: ";
        PrintP3(p3, os); 
        os << " chi2 = " << chi2 << " ";
        os << " isFake = " << isFake << " ";
        os << " isValid = " << isValid << " ";
        os << " ndof = " << ndof << " ";
        os << " rho = " << rho << " ";
        os << " normalizedChi2 = " << normalizedChi2 << " ";
        os << " NumberOfTracks = " << NumberOfTracks << " ";
        os << endl;
}
