#include "../interface/NTLepton.h"
using namespace TopTree;



NTLepton::NTLepton ():
  p4(0.0, 0.0, 0.0, 0.0),
  p4HLT(0.0, 0.0, 0.0, 0.0),
  vertex(0.0,0.0,0.0),
  Charge(-999.),
  ECaloIso03(-999.),
  HCaloIso03(-999.),
  CaloIso03(-999.),
  PATNeutralHadronIso(-999),
  PATChargedHadronIso(-999),
  PATPhotonIso(-999), 
  PATTrackIso(-999),
  D0(-999.),
  Chi2(-999.),
  LeptonOrigin(-999),
  turnOnWeight(-999.)
{
}


NTLepton::~NTLepton ()
{
}

void
NTLepton::Reset ()
{
  p4.SetPxPyPzE (0.0, 0.0, 0.0, 0.0);
  p4HLT.SetPxPyPzE (0.0, 0.0, 0.0, 0.0);
  vertex.SetXYZ(0.0,0.0,0.0);
  Charge = -999.;
  ECaloIso03 = -999.;
  HCaloIso03 = -999.;
  CaloIso03 = -999.;
  D0 = -999.;
  Chi2 = -999.;
  LeptonOrigin = -999;
  PATNeutralHadronIso = -999 ;
  PATChargedHadronIso = -999;
  PATPhotonIso = -999;
  PATTrackIso  = -999; 
  turnOnWeight = -999.;
}

void NTLepton::PrintInfo(std::ostream & os){
        PrintP4(p4, os); 
        os << "\n";
        os << "vertex: ";
        PrintP3(vertex, os); 
        os << "\n";
        os << " Charge = "<< Charge <<" ";
        os << " CaloIso03 =  "<< CaloIso03 <<" ";
        os << " ECaloIso03 =  "<< ECaloIso03 <<" ";
        os << " HCaloIso03 =  "<< HCaloIso03 <<" ";
        os << " D0 =  "<< D0 <<" ";
        os << " Chi2 =  "<< Chi2 <<" ";
        os << " LeptonOrigin =  "<< LeptonOrigin <<" ";
        os << " turnOnWeight =  "<< turnOnWeight <<" ";
        os << endl;
}

double NTLepton::RelIso03(){
        if(p4.Pt()>0)  return( ((TrkIso03+ECaloIso03+HCaloIso03)/p4.Pt()));
        return (-999.);
}

double NTLepton::RelIso03PF(){
        //if(p4.Pt()>0)  return( ((PATNeutralHadronIso+PATChargedHadronIso+PATTrackIso)/p4.Pt()));
        //modification (from Denis): avoid to duplicate TrackIso (included in ChargedHadron) and take into account photons
        if(p4.Pt()>0)  return( ((PATNeutralHadronIso+PATChargedHadronIso+PATPhotonIso)/p4.Pt()));
        return (-999.);
}

double NTLepton::RelIso03RhoCorrected(double rho){
         double pi = 3.141592654;
        if(p4.Pt()>0)  return( ((TrkIso03+ECaloIso03+HCaloIso03 - rho*pi*pow(0.3,2))/p4.Pt()));
        return (-999.);
}

double NTLepton::RelIso03PFRhoCorrected(double rho){
         double pi = 3.141592654;
        if(p4.Pt()>0)  return( ((PATNeutralHadronIso+PATChargedHadronIso+PATTrackIso - rho*pi*pow(0.3,2))/p4.Pt()));
        return (-999.);
}

double NTLepton::RelIso03NeutralRhoCorrected(double neutralRho){
         double pi = 3.141592654;
        if(p4.Pt()>0)  return( ((TrkIso03+ECaloIso03+HCaloIso03 - neutralRho*pi*pow(0.3,2))/p4.Pt()));
        return (-999.);
}

double NTLepton::RelIso03PFNeutralRhoCorrected(double neutralRho){
         double pi = 3.141592654;
        if(p4.Pt()>0)  return( ((PATNeutralHadronIso+PATChargedHadronIso+PATTrackIso - neutralRho*pi*pow(0.3,2))/p4.Pt()));
        return (-999.);
}
