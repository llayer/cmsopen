#ifndef __TOP_TREE_NTPU_H__
#define __TOP_TREE_NTPU_H__


#include <iostream>


/** NTPU: class pileup for NTuple. \\
 */

using namespace std;

namespace TopTree
{
  class NTPU
  {
  public:

    //methods
    NTPU ();
    ~NTPU ();
    void Reset ();
    void Dump(std::ostream & os = std::cout);
    NTPU GetNTPU(){ return *this;};
    NTPU* GetNTPUPointer(){ return this;};
    
    //data members
    int    bunchCrossing;   // to which bunch crossing does this interaction belong?  
    float  zpositions;      // the true primary vertex position along the z axis for each added interaction
    float  sumpT_lowpT;     // the sum of the transverse momentum of the tracks originating from each interaction, where track pT > low_cut
    float  sumpT_highpT;    // the sum of the transverse momentum of the tracks originating from each interaction, where track pT > high_cut
    int    ntrks_lowpT;     // the number of tracks originating from each interaction, where track pT > low_cut
    int    ntrks_highpT;    // the number of tracks originating from each interaction, where track pT > high_cut
    // Currently, the low_cut is set to 0.1 GeV and the high_cut is set to 0.5 GeV [date : 19/05/2011]

  };
}
#endif
