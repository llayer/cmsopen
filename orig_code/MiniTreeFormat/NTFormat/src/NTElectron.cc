#include "../interface/NTElectron.h"
using namespace TopTree;

    double eSuperClusterOverP;

NTElectron::NTElectron ():
    NTLepton(), 
    isGsfElectron(false),
    ET_SC(-999.),
    nLost(-999),
    elecIdLoose(-999),
    elecIdTight(-999),
    elecIdRobustLoose(-999),
    elecIdRobustTight(-999),
    elecIdWP90_c(-999),         
    elecIdWP90_r(-999),         
    cicVeryLoose(-999),
    cicLoose(-999),
    cicMedium(-999),
    cicTight(-999),
    cicSuperTight(-999),
    cicHyperTight1(-999),
    cicHyperTight2(-999),
    cicHyperTight3(-999),
    cicVeryLooseMC(-999),
    cicLooseMC(-999),
    cicMediumMC(-999),
    cicTightMC(-999),
    cicSuperTightMC(-999),
    cicHyperTight1MC(-999),
    cicHyperTight2MC(-999),
    cicHyperTight3MC(-999),
    isEB(0),
    isEcalDriven(false),
    elspike(-999.),
    //eSuperClusterOverP(-999.),
    //deltaEtaSuperClusterTrackAtVtx(-999.),
    //deltaPhiSuperClusterTrackAtVtx(-999.),
    //hcalOverEcal(-999.),
    //sigmaIetaIeta(-999.),
    //e2x5Max(-999.),
    //e5x5(-999.),      
    nlost(-999),
    deltaCotTheta(-999.),
    deltaDistance(-999.)
{
}

NTElectron::~NTElectron ()
{
}

void NTElectron::Reset ()
{
    NTLepton::Reset ();
    isGsfElectron = false;
    ET_SC = -999.;
    nLost = -999;
    isEB = 0;
    isEcalDriven = false;
    elspike = -999.;
    //eSuperClusterOverP = -999.;
    //deltaEtaSuperClusterTrackAtVtx = -999.;
    //deltaPhiSuperClusterTrackAtVtx = -999.;
    //hcalOverEcal = -999.;
    //sigmaIetaIeta = -999.;
    //e2x5Max = -999.;
    //e5x5 = -999.;     
    elecIdLoose = -999;
    elecIdTight = -999;
    elecIdRobustLoose = -999;
    elecIdRobustTight = -999;
    elecIdWP90_c = -999;                
    elecIdWP90_r = -999;                
    cicVeryLoose = -999;
    cicLoose = -999;
    cicMedium = -999;
    cicTight = -999;
    cicSuperTight = -999;
    cicHyperTight1 = -999;
    cicHyperTight2 = -999;
    cicHyperTight3 = -999;
    cicVeryLooseMC = -999;
    cicLooseMC = -999;
    cicMediumMC = -999;
    cicTightMC = -999;
    cicSuperTightMC = -999;
    cicHyperTight1MC = -999;
    cicHyperTight2MC = -999;
    cicHyperTight3MC = -999;
    nlost = -999;
    deltaCotTheta = -999.;
    deltaDistance = -999.;
}

void NTElectron::Dump(std::ostream & os){
        os << "Object: Electron \n";
        PrintInfo (os);
        os << " isGsfElectron = "<<isGsfElectron<<" ";
        os << " ET_SC = "<<ET_SC<<" ";
        os << " nLost = "<< nLost <<" ";
        os << " elecId = "<< elecIdWP90_r <<" ";
        os << " isEB = "<<  isEB <<" ";
        os << " isEcalDriven = "<< isEcalDriven<<" ";
        os << " elspike = "<<elspike<<" ";
        //os << " eSuperClusterOverP = "<< eSuperClusterOverP <<" ";
        //os << " deltaEtaSuperClusterTrackAtVtx = "<< deltaEtaSuperClusterTrackAtVtx <<" ";
        //os << " deltaPhiSuperClusterTrackAtVtx = "<<  deltaPhiSuperClusterTrackAtVtx <<" ";
        //os << " hcalOverEcal = "<< hcalOverEcal <<" ";
        //os << " sigmaIetaIeta = "<< sigmaIetaIeta <<" ";
        //os << " e2x5Max = "<<  e2x5Max <<" ";
        //os << " e5x5 = "<<  e5x5 <<" ";
        os << " nlost = " << nlost <<" ";
        os << " deltaCotTheta = " << deltaCotTheta<<" ";
        os << " deltaDistance = " << deltaDistance<<" ";
        os << endl;
}
 
  
float NTElectron::CombinedRelIso03(){
      double combinedIso = -999;
      if      ( isEB==1 )  combinedIso = (TrkIso03 + TMath::Max(0.,ECaloIso03-1.) + HCaloIso03)/TMath::Max(20.,p4.Et());
      else              combinedIso = (TrkIso03 + ECaloIso03 +  HCaloIso03)/TMath::Max(20.,p4.Et());
      return combinedIso;
}
  
