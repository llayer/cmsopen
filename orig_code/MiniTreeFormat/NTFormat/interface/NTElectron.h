#ifndef __TOP_TREE_NTELECTRON_H__
#define __TOP_TREE_NTELECTRON_H__


#include "NTLepton.h"

/** NTElectron: class electron for NTuple. \\
//default track = gsfTrack \\
//D0 = Transverse IP of gsfTrack  wrt BS (or 0 if !BS)\\
*/

using namespace std;

namespace TopTree
{
  class NTElectron: public NTLepton
  {
  public:

    //methods
    NTElectron ();
    ~NTElectron ();
    void Reset ();
    bool isNotConv(int elecId) const { return (elecId & 0x4);};
    bool hadId(int elecId) const { return (elecId & 0x1);};
    void Dump(std::ostream & os = std::cout);
    NTElectron GetNTElectron(){ return *this;};
    NTElectron* GetNTElectronPointer(){ return this;};
    
    float CombinedRelIso03();/** consider Barrel & EndCap cases */
    //bool isEE() const;
    //bool isEB() const;

    //data members
    bool isGsfElectron; /** correspond to elec->ecalDrivenSeed () in PAT */
    float ET_SC; /** ET of the SuperCluster */
    int nLost;
    
    int elecIdLoose;
    int elecIdTight;
    int elecIdRobustLoose;
    int elecIdRobustTight;
    int elecIdWP90_c;           /** Working Point 90, combined iso (simple cut based ID) */ 
    int elecIdWP90_r;           /** Working Point 90, relative iso (simple cut based ID) */
    //have been removed: useless
    /*
    int elecIdWP95_c;
    int elecIdWP95_r; 
    int elecIdWP85_c;  
    int elecIdWP85_r; 
    int elecIdWP70_c;
    int elecIdWP70_r; 
    */    

    //new e-Id
    int cicVeryLoose;
    int cicLoose;
    int cicMedium;
    int cicTight;
    int cicSuperTight;
    int cicHyperTight1;
    int cicHyperTight2;
    int cicHyperTight3;
    int cicVeryLooseMC;
    int cicLooseMC;
    int cicMediumMC;
    int cicTightMC;
    int cicSuperTightMC;
    int cicHyperTight1MC;
    int cicHyperTight2MC;
    int cicHyperTight3MC;
    

    int isEB; // isEE * 1 + isEB * 10

    // variables for lepton ID:
    bool isEcalDriven;
    float elspike;
    double eSuperClusterOverP;
    //double deltaEtaSuperClusterTrackAtVtx;
    //double deltaPhiSuperClusterTrackAtVtx;
    //double hcalOverEcal;
    //double sigmaIetaIeta;
    //double e2x5Max;
    //double e5x5;
    double fbrem;
    
    //for electron conversion rejection
    int nlost;
    float deltaCotTheta;
    float deltaDistance;
 

  };
}
#endif

