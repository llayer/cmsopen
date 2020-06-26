#ifndef __TOP_TREE_MTMET_H__
#define __TOP_TREE_MTMET_H__


#include "TLorentzVector.h"

#include "MiniTreeFormat/NTFormat/interface/NTMET.h"

/** MTMET: class MET for MiniTree - inherit from NTMET \\
Add all relevant information that should be saved in MiniTree but not in the NTuple \\
*/

namespace TopTree
{
  class MTMET : public NTMET
  {
  public:

    //methods
    MTMET ();
    ~MTMET ();
    void Reset ();
    void doCorrection (float SumMuMetCorrX, float SumMuMetCorrY);
    void DumpExtended(std::ostream & os = std::cout);

    //data members
    TLorentzVector p4MuonCorr;  /** missing Et vector -->> 4D vector since no 2D object available in [root/5.14.00f-CMS3q] ... */

    double dmEx;                /** missing Et x correction */
    double dmEy;                /** missing Et y correction */
    double misEt;
    double uncmisEt;
    double sumEt;
  };
}
#endif

