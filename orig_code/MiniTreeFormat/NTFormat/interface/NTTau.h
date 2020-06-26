#ifndef __TOP_TREE_NTTau_H__
#define __TOP_TREE_NTTau_H__

#include "NTLepton.h"

/** NTTau: class tau for NTuple. \\
*/

namespace TopTree
{
  class NTTau:public NTLepton
  {
  public:

    //methods
    NTTau ();
    ~NTTau ();
    void Reset ();
    void Dump(std::ostream & os = std::cout);
    NTTau GetNTTau(){ return *this;};
    NTTau* GetNTTauPointer(){ return this;};
    int GetDiscriminator (std::string discriminatorName);

    //data members

    double leadTrackPt;
    double numSigConeTracks;
    double numIsoConeTracks;
    int decay;

    //pf tau specific
    float isolationPFChargedHadrCandsPtSum;
    float isolationPFGammaCandsEtSum;
    float maximumHCALPFClusterEt;
    float emFraction;
    float hcalTotOverPLead;
    float hcalMaxOverPLead;
    float hcal3x3OverPLead;
    float ecalStripSumEOverPLead;
    int decayMode;
    
    /*
    double discrByLooseIsolation;
    double discrByMediumIsolation;
    double discrByTightIsolation;
    double discrByVLooseIsolation;
    double discrByTrackIsolation;
    double discrByEcalIsolation;
    double discrByLeadingTrackFinding;
    double discrByLeadingTrackPtCut;
    double discrByTaNC;
    double discrByTaNCOnePercent;
    double discrByTaNCHalfPercent;
    double discrByTaNCQuarterPercent;
    double discrByTaNCTenthPercent;
    double discrAgainstElectronLoose;
    double discrAgainstMuonLoose;
    double discrAgainstElectronTight;
    double discrAgainstMuonTight;
    double discrAgainstElectronMedium;
    double discrHPSLooseIsolation;
    double discrHPSMediumIsolation;
    double discrHPSTightIsolation;*/
    std::vector< std::pair<std::string,float> > tauIDs;
    
  };
}
#endif
