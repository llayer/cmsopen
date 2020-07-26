#ifndef __TOP_TREE_NTMUON_H__
#define __TOP_TREE_NTMUON_H__


#include <iostream>
#include "NTLepton.h"

/** NTMuon: class muon for NTuple. \\
//default track = globalTrack (tracker+mu) \\
//D0   =   Transverse IP of globalTrack (tracker+mu) wrt BS (or 0 if !BS) \\
//Chi2 =   Normalized chi2 of globalTrack wrt BS (or 0 if !BS) \\
*/

namespace TopTree
{
  class NTMuon : public NTLepton
  {
  public:
    //methods
    NTMuon ();
    ~NTMuon ();
    void Reset ();
    void Dump(std::ostream & os = std::cout);
    NTMuon GetNTMuon(){ return *this;};
    NTMuon* GetNTMuonPointer(){ return this;};

    //data members
    int MuonType; /** IsGlobalMuon * 100 + IsStandAloneMuon * 10 + IsTrackerMuon * 1; */
    int GlobalMuonPromptTight;
    int NTrValidHits; /** nof valid hits associated to the tracker track */
    int NValidHits; /** nof valid hits associated to the global track */
    float D0Inner;              //Transverse IP from tracks
    int NOfMatches;

  };
}
#endif

