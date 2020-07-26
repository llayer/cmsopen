#include "../interface/NTTau.h"
using namespace TopTree;


NTTau::NTTau ():
NTLepton (),
leadTrackPt (-999.),
numSigConeTracks (-999.),
numIsoConeTracks (-999.),
decay(-1),
isolationPFChargedHadrCandsPtSum (-999.),
isolationPFGammaCandsEtSum (-999.),
maximumHCALPFClusterEt (-999.), 
emFraction (-999.), 
hcalTotOverPLead (-999.), 
hcalMaxOverPLead (-999.), 
hcal3x3OverPLead (-999.), 
ecalStripSumEOverPLead (-999.),
decayMode(-99)
/*discrByVLooseIsolation(-999.),
discrByLooseIsolation(-999.),
discrByMediumIsolation(-999.),
discrByTightIsolation(-999.),
discrByTrackIsolation(-999.),
discrByEcalIsolation(-999.),
discrByLeadingTrackFinding(-999.),
discrByLeadingTrackPtCut(-999.),
discrByTaNC(-999.),
discrByTaNCOnePercent(-999.),
discrByTaNCHalfPercent(-999.),
discrByTaNCQuarterPercent(-999.),
discrByTaNCTenthPercent(-999.),
discrAgainstElectronLoose(-999.),
discrAgainstElectronMedium(-999.),
discrAgainstElectronTight(-999.),
discrAgainstMuonLoose(-999.),
discrAgainstMuonTight(-999.)*/

{
}

NTTau::~NTTau ()
{
}

int NTTau::GetDiscriminator(std::string discriminatorName)
{
int discriminatorValue = -1;
for (unsigned int j = 0; j < tauIDs.size (); j++) 
    {//std::cout << "tau discr " << tauIDs[j].first << std::endl;
     if (tauIDs[j].first==discriminatorName) discriminatorValue = tauIDs[j].second;
     }
return discriminatorValue;
}

void
NTTau::Reset ()
{
  NTLepton::Reset ();
  leadTrackPt = -999.;
  numSigConeTracks = -999.;
  numIsoConeTracks = -999.;
  isolationPFChargedHadrCandsPtSum = -999.;
  isolationPFGammaCandsEtSum = -999.;
  maximumHCALPFClusterEt = -999.;
  emFraction = -999.;
  hcalTotOverPLead = -999.;
  hcalMaxOverPLead = -999.;
  hcal3x3OverPLead = -999.;
  ecalStripSumEOverPLead = -999.; 
  decay = -1;
  /*discrByLooseIsolation = -999.;
  discrByMediumIsolation = -999.;
  discrByTightIsolation = -999.;
  discrByVLooseIsolation = -999.;
  discrByTrackIsolation = -999.;
  discrByEcalIsolation = -999.;
  discrByLeadingTrackFinding = -999.;
  discrByLeadingTrackPtCut = -999.;
  discrByTaNC = -999.;
  discrByTaNCOnePercent = -999.;
  discrByTaNCHalfPercent = -999.;
  discrByTaNCQuarterPercent = -999.;
  discrByTaNCTenthPercent = -999.;
  discrAgainstElectronLoose = -999.;
  discrAgainstElectronMedium = -999.;
  discrAgainstElectronTight = -999.;
  discrAgainstMuonLoose = -999.;
  discrAgainstMuonTight = -999.;*/
  decayMode = -99;
  tauIDs.clear();

  
}

void NTTau::Dump(std::ostream & os){
        os << "Object: Tau \n";
        PrintInfo();
        os << " leadTrackPt = " << leadTrackPt << " ";
        os << " numSigConeTracks = " << numSigConeTracks << " ";
        os << " numIsoConeTracks = " << numIsoConeTracks << " ";
        os << " maximumHCALPFClusterEt = " << maximumHCALPFClusterEt << " ";
        os << " emFraction = " << emFraction << " ";
        os << " hcalTotOverPLead = " << hcalTotOverPLead << " ";
        os << " hcalMaxOverPLead = " << hcalMaxOverPLead << " ";
        os << " hcal3x3OverPLead = " << hcal3x3OverPLead << " ";
        os << " ecalStripSumEOverPLead = " << ecalStripSumEOverPLead << " ";
        os << endl;
}
