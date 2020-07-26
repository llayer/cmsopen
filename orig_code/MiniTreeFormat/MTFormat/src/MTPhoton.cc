#include "../interface/MTPhoton.h"

using namespace TopTree;


MTPhoton::MTPhoton ()//:
    //DB (-999.)
{
}

MTPhoton::~MTPhoton ()
{
}

void
MTPhoton::Reset ()
{
  //p4Gen.SetPxPyPzE (0.0, 0.0, 0.0, 0.0);
  //DB = -999.;
}

void MTPhoton::PrintAdditionalInfo(std::ostream & os){
  //os << " p4Gen ";
  //PrintP4(p4Gen, os);
  //os << "pgdID-mother = "<< GenMother.pdgId() << " ";
  //os << "pgdID-Gmother = "<< GenGrandMother.pdgId() << " ";
  //os << "pgdID-GGmother = "<< GenGGrandMother.pdgId() << " ";
  //os << endl;
}
