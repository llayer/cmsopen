#ifndef FitFuctions_h
#define EventShapes_h

// system include files
#include <memory>
#include <iostream>
#include <string>
#include "TH1F.h"
#include "THStack.h"
#include "TF1.h"
#include <math.h>
#include "TMath.h"
#include "TMatrixDSym.h"
#include "TVectorD.h"
#include "Math/Vector3D.h"
#include <vector>
#include "../../../../MiniTreeFormat/NTFormat/interface/NTLepton.h"
#include "../../../../MiniTreeFormat/NTFormat/interface/NTJet.h"
#include "../../../../MiniTreeFormat/NTFormat/interface/NTTau.h"

using namespace std;
using namespace TopTree;
using namespace ROOT::Math;

class EventShapes {

/**
   Stolen from CMSSW...
*/

 typedef ROOT::Math::DisplacementVector3D< ROOT::Math::Polar3D< double > > RThetaPhiVector;

 public:
 
  /** TopIPHC constructors...*/
  /// constructor for NTJet  
  EventShapes(const vector<NTJet>& inputJets);  
  
  /// constructor for NTJet, NTLepton  
  EventShapes(const vector<NTJet>& inputJets, const vector<NTLepton>& inputLeptons);  
 
  /// constructor for NTJet, NTTau 
  EventShapes(const vector<NTJet>& inputJets, const vector<NTTau>& inputTaus);  
 
  /// add whatever you need...


  /** Original constructors from CMSSW...
      To be removed ?                     
      HT,H, sqrt_s not filled using these constructors. */
      
  /// constructor from XYZ coordinates  
  EventShapes(const vector<XYZVector>& inputVectors);  

  /// constructor from rho eta phi coordinates
  explicit EventShapes(const vector<RhoEtaPhiVector>& inputVectors);  

  /// constructor from r theta phi coordinates
  explicit EventShapes(const vector<RThetaPhiVector>& inputVectors);  

  /// default destructor
  ~EventShapes(){};



  /// the return value is 1 for spherical events and 0 for events linear in r-phi. This function 
  /// needs the number of steps to determine how fine the granularity of the algorithm in phi 
  /// should be
  double isotropy(const unsigned int& numberOfSteps = 1000) const;
 
  
  /// the return value is 1 for spherical and 0 linear events in r-phi. This function needs the 
  /// number of steps to determine how fine the granularity of the algorithm in phi should be
  double circularity(const unsigned int& numberOfSteps = 1000) const;
  
 
  /// 1.5*(q1+q2) where 0<=q1<=q2<=q3 are the eigenvalues of the momemtum tensor 
  /// sum{p_j[a]*p_j[b]}/sum{p_j**2} normalized to 1. Return values are 1 for spherical, 3/4 for 
  /// plane and 0 for linear events
  double sphericity(double = 2.)  const;
  
 
  /// 1.5*q1 where 0<=q1<=q2<=q3 are the eigenvalues of the momemtum tensor 
  /// sum{p_j[a]*p_j[b]}/sum{p_j**2} normalized to 1. Return values are 0.5 for spherical and 0 
  /// for plane and linear events
  double aplanarity(double = 2.)  const;
 
  
  /// 3.*(q1*q2+q1*q3+q2*q3) where 0<=q1<=q2<=q3 are the eigenvalues of the momemtum tensor 
  /// sum{p_j[a]*p_j[b]}/sum{p_j**2} normalized to 1. Return value is between 0 and 1 
  /// and measures the 3-jet structure of the event (C vanishes for a "perfect" 2-jet event)
  double C(double = 2.) const;
 
  
  /// 27.*(q1*q2*q3) where 0<=q1<=q2<=q3 are the eigenvalues of the momemtum tensor 
  /// sum{p_j[a]*p_j[b]}/sum{p_j**2} normalized to 1. Return value is between 0 and 1 
  /// and measures the 4-jet structure of the event (D vanishes for a planar event)
  double D(double = 2.) const;
  
  double  chi2(const vector<NTJet>& inputJets)  const;
  double M2chi2(const vector<NTJet>& inputJets) const;
  double M3chi2(const vector<NTJet>& inputJets) const;
  double M3(const vector<NTJet>& inputJets) const;

  ///
  inline double sqrt_s() const {return sqrt_s_;}
  
  ///
  inline double H() const {return H_;}
  
  ///
  inline double HT() const {return HT_;}
  
  ///
  inline double HT3() const {return HT3_;}
 
 
 private:
  /// helper function to fill the 3 dimensional momentum tensor from the inputVectors where 
  /// needed
  TMatrixDSym compMomentumTensor(double = 2.) const;
  TVectorD compEigenValues(double = 2.) const;
 
  /// cashing of input vectors
  vector<XYZVector> inputVectors_;
  
  double sqrt_s_;
  double H_;
  double HT_;
  double HT3_;
};

#endif
