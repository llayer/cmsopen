#ifndef __TOP_TREE_MTELECTRON_H__
#define __TOP_TREE_MTELECTRON_H__


#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "MiniTreeFormat/NTFormat/interface/NTElectron.h"
#include "MTLepton.h"

/** MTElectron: class electron for MiniTree - inherit from NTElectron \\
//default track = gsfTrack \\
//D0 = Transverse IP of gsfTrack  wrt BS (or 0 if !BS) \\
Add all relevant information that should be saved in MiniTree but not in the NTuple \\
*/

using namespace std;

namespace TopTree
{
  class MTElectron:public NTElectron
  {
  public:

    //methods
    MTElectron ();
    ~MTElectron ();
    void Reset ();
    void DumpExtended(std::ostream & os = std::cout);

    //data members

    MTLepton addInfo;

    double caloEnergy;
    double deltaEtaSeedClusterTrackAtCalo;
    double deltaPhiSeedClusterTrackAtCalo;
    double rawSCEnergy;
    double hadronicOverEm;
    int classification;

    // variables for lepton isolation:
    double TrkPt;
    double TrkZ0;
    double TrkIso04;            /**summed track pt in a cone of deltaR<0.4 */
    double HadEnergy;
    double EmEnergy;
    double CaloIso04;           /** sum of ECaloIso and HCaloIso (cone = 0.4) */
    double ECaloIso04;          /** ecal iso deposit from RecHits with electron footprint removed (cone = 0.4) */
    double HCaloIso04;          /** hcal iso deposit from calo Towers (cone = 0.4) */

    // variables for identification
    //actually all located in NTElectron

    //track 
    reco::GsfTrack TrackEl;     /** associated GsfTrack for electron */

    // IP variables   
    double IP3D;
    double IP3DErr;

  };
}
#endif
