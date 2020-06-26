#ifndef __TOP_TREE_MTPU_H__
#define __TOP_TREE_MTPU_H__

#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "MiniTreeFormat/NTFormat/interface/NTPU.h"


/** MTPU: class pileup for MiniTree - inherit from NTPU \\
Add all relevant information that should be saved in MiniTree but not in the NTuple \\
*/

using namespace std;

namespace TopTree
{
  class MTPU:public NTPU
  {
  public:

    //methods
    MTPU ();
    ~MTPU ();
    void Reset ();
    void DumpExtended(std::ostream & os = std::cout);

  };
}
#endif
