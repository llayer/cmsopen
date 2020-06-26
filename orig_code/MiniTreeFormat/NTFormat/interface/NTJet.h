#ifndef __TOP_TREE_NTJET_H__
#define __TOP_TREE_NTJET_H__

#include <iostream>
#include "TLorentzVector.h"

/** NTJet: class jet for NTuple. \\
Contains info on identification, b-tagging, partonFlavour, SV.
*/

using namespace std;

namespace TopTree
{
  class NTJet
  {
  public:
    //methods
    NTJet ();
    ~NTJet ();
    void Reset ();
    void Dump(std::ostream & os = std::cout);
    void PrintP4(TLorentzVector a, std::ostream & os = std::cout){
        os << " p4 = ("<< a.Px() << "," << a.Py() << "," << a.Pz() << "," << a.E() << ") & pT = " << a.Pt() << " ";
    }
    NTJet GetNTJet(){ return *this;};
    NTJet* GetNTJetPointer(){ return this;};

    //data members 
    TLorentzVector p4;          /** 4 vector of jet.*/
    TLorentzVector p4Gen;       /** 4 vector of gen jet.*/
    TLorentzVector p4Parton;    /** 4 vector of gen jet.*/
    TLorentzVector p4HLT;       /** 4 vector of matched HLT jet.*/

    // for Jet IDentification: 
    double emEnergyFraction;    /** jet electromagnetic energy fraction  // ( not defined for pfjet)  */
    double hadEnergyFraction;   /** jet hadronic energy fraction */
    double CHEF;                /** Charged Hadronic Energy Fraction */  
    double CEEF;                /** Charged Electromagnetic Energy Fraction  (for Pf jet ID) */
    double NHEF;                /** Neutral Hadronic Energy Fraction  */
    double NEEF;                /** Neutral Electromagnetic Energy Fraction */  
    double n90Hits;             /** Number of recHits (i.e. cells) needed to make up 90% of the jet's energy 
     (!= n90 == Number of towers (i.e. cells) needed to make up 90% of the jet's energy(not for pfjet) */
    double fHPD;                /** fraction of jet energy from the highest energy Hybrid Photo Diode (not for pfjet) */
    
    
    bool passJetID;
    //MC info
    int partonFlavour;
    
    // b-tagging
    float GetDiscri(string algo);
    vector<std::pair< std::string, float > > DiscriPair; /** dump of PAT content for all the taggers */
    double TCDiscri;  /** b-tagging discriminator for TrackCounting algorithm */
    double SVDiscri;  /** b-tagging discriminator for SecondaryVertex algorithm */
    double SMDiscri;  /** b-tagging discriminator for SM algorithm */
   
    int nTracks;     /** number of tracks associated to the jet */
    int sumPtTracks; /** the scalar sum of Pt of the nTracks tracks */ 
    //info on Secondaray Vertex
    int nTracksSVX;             /** number of tracks from SV tagger (== IP tagger) */
    int nSVX;                   /** number of Secondary Vertices */
    double chi2SVX;             /** chi2 of the best secondary vertex */
    double ndofSVX;             /** ndof of the best secondary vertex */
    double flightDistanceSVX;   /** distance (3D)  between primary and best secondary vertex */
    double flightDistanceSigSVX;        /** distance significance (3D) between primary and secondary vertex */
    double flightDistanceErrSVX;        /** distance error (3D) between primary and secondary vertex */
    double MassSVX;             /** inv. mass of the best secondary vertex */
    //double resCorr; 
    
    //turn-on weight
    double turnOnWeight;
    double corrJES;
    
  };
}
#endif
