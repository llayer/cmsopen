#ifndef __TOP_TREE_MTJET_H__
#define __TOP_TREE_MTJET_H__

#include <map>
#include <vector>
#include <string>
#include "TVector3.h"


#include <DataFormats/Common/interface/RefVector.h>
#include  "DataFormats/JetReco/interface/JetTracksAssociation.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "MiniTreeFormat/NTFormat/interface/NTJet.h"
#include "MTTrack.h"


/** MTJet: class jet for MiniTree - inherit from NTJet \\
Contains collections of tracks and the vertex.
Add all relevant information that should be saved in MiniTree but not in the NTuple \\
*/

namespace TopTree
{
  class MTJet : public NTJet
  {
  public:
    //methods
    MTJet ();
    ~MTJet ();
    void Reset ();
    MTTrack *NewTrack ();
    void setTracks (const reco::TrackRefVector &);
    void DumpExtended(std::ostream & os = std::cout);

    //data members 

    double scale;               // Jet correction factor (from L1+L2+L3 corrections to raw)
    int isLOOSE;                // JetId criteria (LOOSE PURE09 for CaloJet, LOOSE FIRSTDATA for PfJet)  
    int isTIGHT;                // JetId criteria (TIGHT PURE09 for CaloJet, TIGHT FIRSTDATA for PfJet)  

    reco::Candidate::LorentzVector uncorrJet;//add the possibility to recorrect at NTuple level (while producing NTUple from Minitree)
    float jetArea;

    TVector3 vertex;
    // Tracks associated to the jet
    std::vector < reco::Track > TracksOfJet;
    //Tracks associated to the jet from the IP taggerr
    std::vector < MTTrack > trackassociatedVec;


  };
}
#endif
