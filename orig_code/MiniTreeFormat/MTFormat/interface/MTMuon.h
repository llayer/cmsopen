#ifndef __TOP_TREE_MTMUON_H__
#define __TOP_TREE_MTMUON_H__


#include "DataFormats/TrackReco/interface/Track.h"
#include "MiniTreeFormat/NTFormat/interface/NTMuon.h"
#include "MTLepton.h"

/** MTMuon: class muon for MiniTree - inherit from NTMuon \\
//default track = globalTrack (tracker+mu) \\
//D0   =   Transverse IP of globalTrack (tracker+mu) wrt BS (or 0 if !BS) \\
//Chi2 =   Normalized chi2 of globalTrack wrt BS (or 0 if !BS) \\
Add all relevant information that should be saved in MiniTree but not in the NTuple \\
*/

namespace TopTree
{
  class MTMuon : public NTMuon
  {
  public:
    //methods
    MTMuon ();
    ~MTMuon ();
    void Reset ();
    void DumpExtended(std::ostream & os = std::cout);

    //data members

    MTLepton addInfo;

    //tracks 
    reco::Track TrackMu;        // associated track (tracker only) for muon
    reco::Track StaMu;          // associated standalone track
    reco::Track ComMu;          // associated combined track

    //IP variables
    double IP3D;
    double IP3DErr;
    //double D0Inner;           // Transverse IP of InnerTrack (tracker) wrt BS (or 0 if !BS)

  };
}
#endif

