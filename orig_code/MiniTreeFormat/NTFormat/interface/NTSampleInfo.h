#ifndef __TOP_TREE_NTSAMPLEINFO__
#define __TOP_TREE_NTSAMPLEINFO__

#include <string>
#include <iostream>

namespace TopTree
{
  class NTSampleInfo
  {
  public:
    //methods
    NTSampleInfo ();
    ~NTSampleInfo ();
    void Print (std::ostream & os = std::cout) const;

    //data members
    double luminosity;
    double cross_section;
    int TotalEvents;
      std::string Name;
  };
}
#endif

