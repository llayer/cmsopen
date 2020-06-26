#ifndef __TOP_TREE_MTVERTEX_H__
#define __TOP_TREE_MTVERTEX_H__


#include <map>
#include <vector>
#include <string>
#include "TVector3.h"

#include "MiniTreeFormat/NTFormat/interface/NTVertex.h"

/** MTVertex: class vertex for MiniTree - inherit from NTVertex \\
Add all relevant information that should be saved in MiniTree but not in the NTuple \\
*/

namespace TopTree
{
  class MTVertex: public NTVertex
  {
  public:
    //methods
    MTVertex ();
    ~MTVertex ();
    void Reset ();
    void DumpExtended(std::ostream & os = std::cout);

    //data members
    TVector3 p3Err;             //  error position

  };
}
#endif

