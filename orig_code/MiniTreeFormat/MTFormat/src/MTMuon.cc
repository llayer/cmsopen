#include "../interface/MTMuon.h"

using namespace TopTree;

MTMuon::MTMuon ():
  IP3D(-999),
  IP3DErr(-999.)
  //D0Inner(-999.)
{
}

MTMuon::~MTMuon ()
{
}

void
MTMuon::Reset ()
{
  NTMuon::Reset ();
  addInfo.Reset();
  IP3D = -999;
  IP3DErr = -999. ;
  //D0Inner = -999.;
}

void MTMuon::DumpExtended(std::ostream & os){
        Dump(os);
        addInfo.PrintAdditionalInfo(os);
        os << " IP3D = " << IP3D << " ";
        os << " IP3DErr = " << IP3DErr << " ";
        //os << " D0Inner = " << D0Inner << " ";
}
