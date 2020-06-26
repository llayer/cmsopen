
#include "../interface/MTMET.h"


using namespace TopTree;

MTMET::MTMET ():
  dmEx(0.),
  dmEy(0.),
  misEt(0.),
  uncmisEt(0.),
  sumEt(0.)
{
}

MTMET::~MTMET ()
{
}
void
MTMET::doCorrection (float SumMuMetCorrX, float SumMuMetCorrY)
{
  correction = sqrt (pow ((p4.Px () + SumMuMetCorrX), 2) + pow ((p4.Py () + SumMuMetCorrY), 2)) - misEt;
// to get the corrected misEt do : misEt + correction             
}

void
MTMET::Reset ()
{
  NTMET::Reset();
  p4MuonCorr.SetPxPyPzE (0.0, 0.0, 0.0, 0.0);
  dmEx = 0.;
  dmEy = 0.;
  misEt = 0.;
  uncmisEt = 0.;
  sumEt = 0.;
}

void MTMET::DumpExtended(std::ostream & os){
        Dump(os);
        os << " p4MuonCorr ";
        PrintP4(p4MuonCorr, os);
        os << " dmEx = " << dmEx << " ";
        os << " dmEy = " << dmEy << " ";
        os << " misEt = " << misEt << " ";
        os << " uncmisEt = " << uncmisEt << " ";
        os << " sumEt = " << sumEt << " ";
        os << endl;
}
