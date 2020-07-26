#include "../interface/EventShapes.h"

#include "TMath.h"


/// constructor for NTJets
EventShapes::EventShapes(const vector<NTJet>& inputJets) 
{ 
  HT_ = 0.; HT3_ = 0.;
  H_ = 0.;
  sqrt_s_ = 0.;
  TLorentzVector p4_sqrt_s;
  
  inputVectors_.reserve( inputJets.size() ); 
  for(unsigned int i=0;i<inputJets.size();i++){
    inputVectors_.push_back(XYZVector(inputJets[i].p4.Px(),inputJets[i].p4.Py(),inputJets[i].p4.Pz()));
    H_  += inputJets[i].p4.E();
    HT_ += inputJets[i].p4.Et();
    if (i>1) HT3_ += inputJets[i].p4.Et();
    p4_sqrt_s += inputJets[i].p4;
  }
  sqrt_s_ = p4_sqrt_s.M();
}


/// constructor for NTJets
EventShapes::EventShapes(const vector<NTJet>& inputJets, const vector<NTLepton>& inputLeptons) 
{ 
  HT_ = 0.; HT3_ = 0.;
  H_ = 0.;
  sqrt_s_ = 0.;
  TLorentzVector p4_sqrt_s;
  
  inputVectors_.reserve( inputJets.size()+ inputLeptons.size()); 
  for(unsigned int i=0;i<inputJets.size();i++){
    inputVectors_.push_back(XYZVector(inputJets[i].p4.Px(),inputJets[i].p4.Py(),inputJets[i].p4.Pz()));  
    H_  += inputJets[i].p4.E();
    HT_ += inputJets[i].p4.Et(); 
    p4_sqrt_s += inputJets[i].p4;
  }
  for(unsigned int i=0;i<inputLeptons.size();i++){
    inputVectors_.push_back(XYZVector(inputLeptons[i].p4.Px(),inputLeptons[i].p4.Py(),inputLeptons[i].p4.Pz()));  
    H_  += inputJets[i].p4.E();
    HT_ += inputJets[i].p4.Et();
    p4_sqrt_s += inputJets[i].p4;
  }
  sqrt_s_ = p4_sqrt_s.M();
}

/// constructor for NTJets and NTTaus
EventShapes::EventShapes(const vector<NTJet>& inputJets, const vector<NTTau>& inputTaus) 
{ 
  HT_ = 0.; HT3_ = 0.;
  H_ = 0.;
  sqrt_s_ = 0.;
  TLorentzVector p4_sqrt_s;
  
  inputVectors_.reserve( inputJets.size()+ inputTaus.size()); 
  for(unsigned int i=0;i<inputJets.size();i++){
    inputVectors_.push_back(XYZVector(inputJets[i].p4.Px(),inputJets[i].p4.Py(),inputJets[i].p4.Pz()));  
    H_  += inputJets[i].p4.E();
    HT_ += inputJets[i].p4.Et();
    p4_sqrt_s += inputJets[i].p4;
  }
  for(unsigned int i=0;i<inputTaus.size();i++){
    inputVectors_.push_back(XYZVector(inputTaus[i].p4.Px(),inputTaus[i].p4.Py(),inputTaus[i].p4.Pz()));  
    H_  += inputTaus[i].p4.E();
    HT_ += inputTaus[i].p4.Et();
    p4_sqrt_s += inputTaus[i].p4;
  }
  sqrt_s_ = p4_sqrt_s.M();
}

/// constructor for NTJet 
EventShapes::EventShapes(const vector<XYZVector>& inputVectors) 
  : inputVectors_(inputVectors)
{
  HT_=-999.;H_=-999.;sqrt_s_=-999.;HT3_=-999.;
}

/// constructor from rH_o eta pH_i coordinates
EventShapes::EventShapes(const vector<RhoEtaPhiVector>& inputVectors)
{ 
  HT_=-999.;H_=-999.;sqrt_s_=-999.;HT3_=-999.;
  inputVectors_.reserve( inputVectors.size() );
  for ( vector<RhoEtaPhiVector>::const_iterator vec = inputVectors.begin(); vec != inputVectors.end(); ++vec ){
    inputVectors_.push_back(XYZVector(vec->x(), vec->y(), vec->z()));
  }
}

/// constructor from r tH_eta pH_i coordinates
EventShapes::EventShapes(const vector<RThetaPhiVector>& inputVectors)
{ 
  HT_=-999.;H_=-999.;sqrt_s_=-999.;HT3_=-999.;
  inputVectors_.reserve( inputVectors.size() );
  for(vector<RThetaPhiVector>::const_iterator vec = inputVectors.begin(); vec != inputVectors.end(); ++vec ){
    inputVectors_.push_back(XYZVector(vec->x(), vec->y(), vec->z()));
  }
}
  
/// tH_e return value is 1 for spH_erical events and 0 for events linear in r-pH_i. TH_is function 
/// needs tH_e number of steps to determine H_ow fine tH_e granularity of tH_e algoritH_m in pH_i 
/// sH_ould be
double 
EventShapes::isotropy(const unsigned int& numberOfSteps) const
{
  const double deltaPhi=2*TMath::Pi()/numberOfSteps;
  double pH_i = 0, eIn =-1., eOut=-1.;
  for(unsigned int i=0; i<numberOfSteps; ++i){
    pH_i+=deltaPhi;
    double sum=0;
    for(unsigned int j=0; j<inputVectors_.size(); ++j){
      // sum over inner product of unit vectors and momenta
      sum+=TMath::Abs(TMath::Cos(pH_i)*inputVectors_[j].x()+TMath::Sin(pH_i)*inputVectors_[j].y());
    }
    if( eOut<0. || sum<eOut ) eOut=sum;
    if( eIn <0. || sum>eIn  ) eIn =sum;
  }
  return (eIn-eOut)/eIn;
}


/// tH_e return value is 1 for spH_erical and 0 linear events in r-pH_i. TH_is function needs tH_e
/// number of steps to determine H_ow fine tH_e granularity of tH_e algoritH_m in pH_i sH_ould be
double 
EventShapes::circularity(const unsigned int& numberOfSteps) const
{
  const double deltaPH_i=2*TMath::Pi()/numberOfSteps;
  double circularity=-1, pH_i=0, area = 0;
  for(unsigned int i=0;i<inputVectors_.size();i++) {
    area+=TMath::Sqrt(inputVectors_[i].x()*inputVectors_[i].x()+inputVectors_[i].y()*inputVectors_[i].y());
  }
  for(unsigned int i=0; i<numberOfSteps; ++i){
    pH_i+=deltaPH_i;
    double sum=0, tmp=0.;
    for(unsigned int j=0; j<inputVectors_.size(); ++j){
      sum+=TMath::Abs(TMath::Cos(pH_i)*inputVectors_[j].x()+TMath::Sin(pH_i)*inputVectors_[j].y());
    }
    tmp=TMath::Pi()/2*sum/area;
    if( circularity<0 || tmp<circularity ){
      circularity=tmp;
    }
  }
  return circularity;
}

/// H_elper function to fill tH_e 3 dimensional momentum tensor from tH_e inputVecotrs wH_ere needed
TMatrixDSym 
EventShapes::compMomentumTensor(double r) const
{
  TMatrixDSym momentumTensor(3);
  momentumTensor.Zero();

  if ( inputVectors_.size() < 2 ){
    return momentumTensor;
  }

  // fill momentumTensor from inputVectors
  double norm = 1.;
  for ( int i = 0; i < (int)inputVectors_.size(); ++i ){
    double p2 = inputVectors_[i].Dot(inputVectors_[i]);
    double pR = ( r == 2. ) ? p2 : TMath::Power(p2, 0.5*r);
    norm += pR;
    double pRminus2 = ( r == 2. ) ? 1. : TMath::Power(p2, 0.5*r - 1.);
    momentumTensor(0,0) += pRminus2*inputVectors_[i].x()*inputVectors_[i].x();
    momentumTensor(0,1) += pRminus2*inputVectors_[i].x()*inputVectors_[i].y();
    momentumTensor(0,2) += pRminus2*inputVectors_[i].x()*inputVectors_[i].z();
    momentumTensor(1,0) += pRminus2*inputVectors_[i].y()*inputVectors_[i].x();
    momentumTensor(1,1) += pRminus2*inputVectors_[i].y()*inputVectors_[i].y();
    momentumTensor(1,2) += pRminus2*inputVectors_[i].y()*inputVectors_[i].z();
    momentumTensor(2,0) += pRminus2*inputVectors_[i].z()*inputVectors_[i].x();
    momentumTensor(2,1) += pRminus2*inputVectors_[i].z()*inputVectors_[i].y();
    momentumTensor(2,2) += pRminus2*inputVectors_[i].z()*inputVectors_[i].z();
  }
  //cout << "momentumTensor:" << endl;
  //cout << momentumTensor(0,0) << " " << momentumTensor(0,1) << " " << momentumTensor(0,2) 
  //          << momentumTensor(1,0) << " " << momentumTensor(1,1) << " " << momentumTensor(1,2) 
  //          << momentumTensor(2,0) << " " << momentumTensor(2,1) << " " << momentumTensor(2,2) << endl;

  // return momentumTensor normalized to determinant 1
  return (1./norm)*momentumTensor;
}


/// H_elper function to fill tH_e 3 dimensional vector of eigen-values;
/// tH_e largest (smallest) eigen-value is stored at index position 0 (2)
TVectorD
EventShapes::compEigenValues(double r) const
{
  TVectorD eigenValues(3);
  TMatrixDSym myTensor = compMomentumTensor(r);
  if( myTensor.IsSymmetric() ){
    if( myTensor.NonZeros() != 0 ) myTensor.EigenVectors(eigenValues);
  }

  // CV: TMatrixDSym::EigenVectors returns eigen-values and eigen-vectors
  //     ordered by descending eigen-values, so no need to do any sorting H_ere...
  //cout << "eigenValues(0) = " << eigenValues(0) << ","
  //          << " eigenValues(1) = " << eigenValues(1) << ","
  //          << " eigenValues(2) = " << eigenValues(2) << endl;

  return eigenValues;
}


/// 1.5*(q1+q2) wH_ere 0<=q1<=q2<=q3 are tH_e eigenvalues of tH_e momentum tensor sum{p_j[a]*p_j[b]}/sum{p_j**2} 
/// normalized to 1. Return values are 1 for spH_erical, 3/4 for plane and 0 for linear events
double 
EventShapes::sphericity(double r) const
{
  TVectorD eigenValues = compEigenValues(r);
  return 1.5*(eigenValues(1) + eigenValues(2));
}

/// 1.5*q1 wH_ere 0<=q1<=q2<=q3 are tH_e eigenvalues of tH_e momentum tensor sum{p_j[a]*p_j[b]}/sum{p_j**2} 
/// normalized to 1. Return values are 0.5 for spH_erical and 0 for plane and linear events
double 
EventShapes::aplanarity(double r) const
{
  TVectorD eigenValues = compEigenValues(r);
  return 1.5*eigenValues(2);
}


/// 3.*(q1*q2+q1*q3+q2*q3) wH_ere 0<=q1<=q2<=q3 are tH_e eigenvalues of tH_e momentum tensor sum{p_j[a]*p_j[b]}/sum{p_j**2} 
/// normalized to 1. Return value is between 0 and 1 
/// and measures tH_e 3-jet structure of tH_e event (C vanisH_es for a "perfect" 2-jet event)
double 
EventShapes::C(double r) const
{
  TVectorD eigenValues = compEigenValues(r);
  return 3.*(eigenValues(0)*eigenValues(1) + eigenValues(0)*eigenValues(2) + eigenValues(1)*eigenValues(2));
}


/// 27.*(q1*q2*q3) wH_ere 0<=q1<=q2<=q3 are tH_e eigenvalues of tH_e momemtum tensor sum{p_j[a]*p_j[b]}/sum{p_j**2} 
/// normalized to 1. Return value is between 0 and 1 
/// and measures tH_e 4-jet structure of tH_e event (D vanisH_es for a planar event)
double 
EventShapes::D(double r) const
{
  TVectorD eigenValues = compEigenValues(r);
  return 27.*eigenValues(0)*eigenValues(1)*eigenValues(2);
}


double
EventShapes::chi2(const vector<NTJet>& inputJets) const
{

  double Mass3 = 5000;
  double Mass2 = 5000;
  double sigmaW = 10;
  double sigmaT = 45;
  double MT = 170;
  double MW = 80;
  double Chi2W = -50000;
  double Chi2T = -50000;

  double chi2_ = 5000;
  double chi2_min = 10000;

  for(unsigned int i=0; i<inputJets.size(); ++i){
    for(unsigned int j=i+1; j<inputJets.size(); ++j){

      TLorentzVector p4 = inputJets[i].p4 + inputJets[j].p4;
      Mass2 = p4.M();
      Chi2W = pow((Mass2 - MW),2)/pow(sigmaW,2);
      
      for(unsigned int s=j+1; s<inputJets.size(); ++s){
        
        TLorentzVector p4 = inputJets[i].p4 + inputJets[j].p4 + inputJets[s].p4;
        Mass3 = p4.M();
        Chi2T = pow((Mass3 - MT),2)/pow(sigmaT,2);

        chi2_ =  Chi2T ;
        if(chi2_<chi2_min) chi2_min = chi2_;
        
      }
    }
  }  
        

  return chi2_min;
}


double 
EventShapes::M3(const vector<NTJet>& inputJets) const{
 double M3 = -99;
 double minpT = 0.;
 for(unsigned int i=0; i<inputJets.size(); ++i){
    for(unsigned int j=i+1; j<inputJets.size(); ++j){
       for(unsigned int s=j+1; s<inputJets.size(); ++s){
       TLorentzVector p4 = inputJets[s].p4 +inputJets[i].p4 + inputJets[j].p4;
       float pT = p4.Pt();
       if (pT>minpT) {minpT = pT; M3 = p4.M();}
       }}}
       
       return M3;
}

double
EventShapes::M3chi2(const vector<NTJet>& inputJets) const
{

  double Mass3 = 5000;
  double Mass2 = 5000;
  double sigmaW = 10;
  double sigmaT = 45;
  double MT = 172.5;
  double MW = 80;
  double Chi2W = -50000;
  double Chi2T = -50000;

  double chi2_ = 5000;
  double chi2_min = 10000;
  
  double M3_min = 10000;
  double M2_min = 10000;
  

  for(unsigned int i=0; i<inputJets.size(); ++i){
    for(unsigned int j=i+1; j<inputJets.size(); ++j){

      TLorentzVector p4 = inputJets[i].p4 + inputJets[j].p4;
      Mass2 = p4.M();
      Chi2W = pow((Mass2 - MW),2)/pow(sigmaW,2);
      
      for(unsigned int s=j+1; s<inputJets.size(); ++s){
        
        TLorentzVector p4 = inputJets[i].p4 + inputJets[j].p4 + inputJets[s].p4;
        Mass3 = p4.M();
        Chi2T = pow((Mass3 - MT),2)/pow(sigmaT,2);

        chi2_ =  Chi2T ;
        if(chi2_<chi2_min) {chi2_min = chi2_; M3_min = Mass3; M2_min = Mass2;}
        
      }
    }
  }  
        

  return M3_min;
}

double
EventShapes::M2chi2(const vector<NTJet>& inputJets) const
{

  double Mass3 = 5000;
  double Mass2 = 5000;
  double sigmaW = 10;
  double sigmaT = 45;
  double MT = 170;
  double MW = 80;
  double Chi2W = -50000;
  double Chi2T = -50000;

  double chi2_ = 5000;
  double chi2_min = 10000;
  
  double M3_min = 10000;
  double M2_min = 10000;
  

  for(unsigned int i=0; i<inputJets.size(); ++i){
    for(unsigned int j=i+1; j<inputJets.size(); ++j){

      TLorentzVector p4 = inputJets[i].p4 + inputJets[j].p4;
      Mass2 = p4.M();
      Chi2W = pow((Mass2 - MW),2)/pow(sigmaW,2);
      
      for(unsigned int s=j+1; s<inputJets.size(); ++s){
        
        TLorentzVector p4 = inputJets[i].p4 + inputJets[j].p4 + inputJets[s].p4;
        Mass3 = p4.M();
        Chi2T = pow((Mass3 - MT),2)/pow(sigmaT,2);

        chi2_ =  Chi2T ;
        if(chi2_<chi2_min) {chi2_min = chi2_; M3_min = Mass3; M2_min = Mass2;}
        
      }
    }
  }  
        

  return M2_min;
}
