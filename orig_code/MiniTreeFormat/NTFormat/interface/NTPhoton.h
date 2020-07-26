#ifndef __TOP_TREE_NTPhoton_H__
#define __TOP_TREE_NTPhoton_H__


#include <iostream>
#include "TLorentzVector.h"
#include "TVector3.h"

using namespace std;


/**
NTPhoton: mother class for all Photons: NTElectron, NTMuon, NTTau. \\
Should contain the minimal common information as p4, charge, isolation, d0, chi2, PhotonOrigin
*/ 

namespace TopTree
{
  class NTPhoton
  {
  public:

    //methods
    NTPhoton ();
    ~NTPhoton ();
    void Reset ();
    
    void PrintInfo(std::ostream & os = std::cout);
    void PrintP4(TLorentzVector a, std::ostream & os = std::cout){
        os << " p4 = ("<< a.Px() << "," << a.Py() << "," << a.Pz() << "," << a.E() << ") & pT = " << a.Pt() << " ";
    }
    void PrintP3(TVector3 a, std::ostream & os = std::cout){
        os << " p3 = ("<< a.X() << "," << a.Y() << "," << a.Z() << ") ";
    }

    //data members
    TLorentzVector p4;
    
    // variables for Photon isolation:
    double TrkIso;              // summed track pt in a cone of deltaR<0.4
    double ECaloIso;            // ecal iso deposit from RecHits with electron footprint removed (cone = 0.4)
    double HCaloIso;            // hcal iso deposit from calo Towers (cone = 0.4)

    bool isEB ;
    float e1x5 ;
    float e2x5 ;
    float e3x3 ;
    float e5x5 ;
    
  };
}
#endif
