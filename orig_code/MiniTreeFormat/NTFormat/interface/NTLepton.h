#ifndef __TOP_TREE_NTLEPTON_H__
#define __TOP_TREE_NTLEPTON_H__


#include <iostream>
#include "TLorentzVector.h"
#include "TVector3.h"

using namespace std;


/**
NTLepton: mother class for all leptons: NTElectron, NTMuon, NTTau. \\
Should contain the minimal common information as p4, charge, isolation, d0, chi2, leptonOrigin
*/ 

namespace TopTree
{
  class NTLepton
  {
  public:

    //methods
    NTLepton ();
    ~NTLepton ();
    void Reset ();
    double RelIso03();
    double RelIso03PF(); 
    double RelIso03RhoCorrected( double rho);
    double RelIso03PFRhoCorrected( double rho);  
    double RelIso03NeutralRhoCorrected(   double rho);
    double RelIso03PFNeutralRhoCorrected( double rho);  
    void PrintInfo(std::ostream & os = std::cout);
    void PrintP4(TLorentzVector a, std::ostream & os = std::cout){
        os << " p4 = ("<< a.Px() << "," << a.Py() << "," << a.Pz() << "," << a.E() << ") & pT = " << a.Pt() << " ";
    }
    void PrintP3(TVector3 a, std::ostream & os = std::cout){
        os << " p3 = ("<< a.X() << "," << a.Y() << "," << a.Z() << ") ";
    }

    //data members
    TLorentzVector p4; 
    TLorentzVector p4HLT;
    TVector3 vertex;
    double Charge;

    // variables for lepton isolation:
    double TrkIso03;            //summed track pt in a cone of deltaR<0.3
    double ECaloIso03;          // ecal iso deposit from RecHits with electron footprint removed (cone = 0.3)
    double HCaloIso03;          // hcal iso deposit from calo Towers (cone = 0.3)
    double CaloIso03;           // sum of ECaloIso and HCaloIso
    
    //PF isolation 
    double PATNeutralHadronIso ;
    double PATChargedHadronIso ;
    double PATPhotonIso ;
    double PATTrackIso ;
    
    
    // related to the default track  
    float D0;                   //Transverse IP
    float Chi2;                 //Normalized chi2 

    // MC information 
    int LeptonOrigin;
    
    //turn-on weight
    double turnOnWeight;
  };
}
#endif
