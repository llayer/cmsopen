#include "../interface/MTLepton.h"

using namespace TopTree;


MTLepton::MTLepton ():
    DB (-999.)
{
}

MTLepton::~MTLepton ()
{
}

void
MTLepton::Reset ()
{
  p4Gen.SetPxPyPzE (0.0, 0.0, 0.0, 0.0);
  DB = -999.;
}

void MTLepton::PrintAdditionalInfo(std::ostream & os){
  os << " p4Gen ";
  PrintP4(p4Gen, os);
  os << "pgdID-mother = "<< GenMother.pdgId() << " ";
  os << "pgdID-Gmother = "<< GenGrandMother.pdgId() << " ";
  os << "pgdID-GGmother = "<< GenGGrandMother.pdgId() << " ";
  os << endl;
}
