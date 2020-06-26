#ifndef __TOP_TREE_MTTau_H__
#define __TOP_TREE_MTTau_H__

#include "MiniTreeFormat/NTFormat/interface/NTTau.h"
#include "MTLepton.h"

/** MTTau: class tau for MiniTree - inherit from NTTau \\
Add all relevant information that should be saved in MiniTree but not in the NTuple \\
*/

namespace TopTree
{
  class MTTau:public NTTau
  {
  public:

    //methods
    MTTau ();
    ~MTTau ();
    void Reset ();
    void DumpExtended(std::ostream & os = std::cout);

    //data members
    MTLepton addInfo;

  };
}
#endif
