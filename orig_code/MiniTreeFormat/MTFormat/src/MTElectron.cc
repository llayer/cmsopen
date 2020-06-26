#include "../interface/MTElectron.h"

using namespace TopTree;

MTElectron::MTElectron ():
    caloEnergy(-999.),
    deltaEtaSeedClusterTrackAtCalo(-999.),
    deltaPhiSeedClusterTrackAtCalo(-999.),
    rawSCEnergy(-999.),
    hadronicOverEm(-999.),
    classification(-999),
    TrkPt(-999.),
    TrkZ0(-999.),
    TrkIso04(-999.),            
    HadEnergy(-999.),
    EmEnergy(-999.),
    CaloIso04(-999.),           
    ECaloIso04(-999.),          
    HCaloIso04(-999.),          
    IP3D(-999.),
    IP3DErr(-999)
{
}

MTElectron::~MTElectron ()
{
}

void
MTElectron::Reset ()
{
  NTElectron::Reset ();
  addInfo.Reset();
    caloEnergy = -999.;
    deltaEtaSeedClusterTrackAtCalo = -999.;
    deltaPhiSeedClusterTrackAtCalo = -999.;
    rawSCEnergy = -999.;
    hadronicOverEm = -999.;
    classification = -999;
    TrkPt = -999.;
    TrkZ0 = -999.;
    TrkIso04 = -999.;           
    HadEnergy = -999.;
    EmEnergy = -999.;
    CaloIso04 = -999.;          
    ECaloIso04 = -999.;         
    HCaloIso04 = -999.;         
    IP3D = -999.;
    IP3DErr = -999;
  
}

void MTElectron::DumpExtended(std::ostream & os){
        Dump(os);
        addInfo.PrintAdditionalInfo(os);
    os << "caloEnergy = " << caloEnergy << " ";
    os << "deltaEtaSeedClusterTrackAtCalo = " << deltaEtaSeedClusterTrackAtCalo << " ";
    os << "deltaPhiSeedClusterTrackAtCalo = " << deltaPhiSeedClusterTrackAtCalo << " ";
    os << "rawSCEnergy = " << rawSCEnergy << " ";
    os << "hadronicOverEm = " << hadronicOverEm << " ";
    os << "classification = " << classification << " ";
    os << "TrkPt = " << TrkPt << " ";
    os << "TrkZ0 = " << TrkZ0 << " ";
    os << "TrkIso04 = " << TrkIso04 << " ";
    os << "HadEnergy = " << HadEnergy << " ";
    os << "EmEnergy = " << EmEnergy << " ";
    os << "CaloIso04 = " << CaloIso04 << " ";
    os << "ECaloIso04 = " << ECaloIso04 << " ";
    os << "HCaloIso04 = " << HCaloIso04 << " ";
    os << "IP3D = " << IP3D << " ";
    os << "IP3DErr = " << IP3DErr << " ";
}

