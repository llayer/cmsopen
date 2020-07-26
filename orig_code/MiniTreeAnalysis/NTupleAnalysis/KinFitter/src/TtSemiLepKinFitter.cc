#include "../interface/TFitConstraintM.h"
#include "../interface/TAbsFitParticle.h"
#include "../interface/TFitParticleEMomDev.h"
#include "../interface/TFitParticleEtEtaPhi.h"
#include "../interface/TFitParticleEtThetaPhi.h"
#include "../interface/TFitParticleEScaledMomDev.h"

//AC #include "AnalysisDataFormats/TopObjects/interface/TtSemiLepEvtPartons.h"
#include "../interface/TtSemiLepKinFitter.h"
#include "../interface/CovarianceMatrix.h"

# include<sstream>

//#include "FWCore/MessageLogger/interface/MessageLogger.h"

/// default configuration is: Parametrization kEMom, Max iterations = 200, deltaS<= 5e-5, maxF<= 1e-4, no constraints
TtSemiLepKinFitter::TtSemiLepKinFitter():
  TopKinFitter(),
  hadB_(0), hadP_(0), hadQ_(0), lepB_(0), lepton_(0), neutrino_(0),
  jetParam_(kEMom), lepParam_(kEMom), metParam_(kEMom)
{
  setupFitter();
}

TtSemiLepKinFitter::TtSemiLepKinFitter(Param jetParam, Param lepParam, Param metParam,
                                       int maxNrIter, double maxDeltaS, double maxF,
                                       std::vector<Constraint> constraints, double mW, double mTop):
  TopKinFitter(maxNrIter, maxDeltaS, maxF, mW, mTop),
  hadB_(0), hadP_(0), hadQ_(0), lepB_(0), lepton_(0), neutrino_(0),
  jetParam_(jetParam), lepParam_(lepParam), metParam_(metParam), constrList_(constraints)
{
  setupFitter();
}

TtSemiLepKinFitter::~TtSemiLepKinFitter() 
{
  delete hadB_; 
  delete hadP_; 
  delete hadQ_;
  delete lepB_; 
  delete lepton_; 
  delete neutrino_;
  for(std::map<Constraint, TFitConstraintM*>::iterator it = massConstr_.begin(); it != massConstr_.end(); ++it)
    delete it->second;
}

void TtSemiLepKinFitter::printSetup() const
{
  std::stringstream constr;
  for(unsigned int i=0; i<constrList_.size(); ++i){
    switch(constrList_[i]){
    case kWHadMass       : constr << "    * hadronic W-mass (" << mW_   << " GeV) \n"; break;
    case kWLepMass       : constr << "    * leptonic W-mass (" << mW_   << " GeV) \n"; break;
    case kTopHadMass     : constr << "    * hadronic t-mass (" << mTop_ << " GeV) \n"; break;
    case kTopLepMass     : constr << "    * leptonic t-mass (" << mTop_ << " GeV) \n"; break;
    case kNeutrinoMass   : constr << "    * neutrino   mass (0 GeV) \n"; break;
    case kEqualTopMasses : constr << "    * equal    t-masses \n"; break;
    }
  }
  std::cout << "TtSemiLepKinFitter"  
    << "\n"
    << "+++++++++++ TtSemiLepKinFitter Setup ++++++++++++ \n"
    << "  Parametrization:                                \n" 
    << "   * jet : " << param(jetParam_) << "\n"
    << "   * lep : " << param(lepParam_) << "\n"
    << "   * met : " << param(metParam_) << "\n"
    << "  Constraints:                                    \n"
    <<    constr.str()
    << "  Max(No iterations): " << maxNrIter_ << "\n"
    << "  Max(deltaS)       : " << maxDeltaS_ << "\n"
    << "  Max(F)            : " << maxF_      << "\n"
    << "+++++++++++++++++++++++++++++++++++++++++++++++++ \n";
}

void TtSemiLepKinFitter::setupJets()
{
  TMatrixD empty3x3(3,3); 
  TMatrixD empty4x4(4,4);
  switch(jetParam_){ // setup jets according to parameterization
  case kEMom :
    hadB_= new TFitParticleEMomDev   ("Jet1", "Jet1", 0, &empty4x4);
    hadP_= new TFitParticleEMomDev   ("Jet2", "Jet2", 0, &empty4x4);
    hadQ_= new TFitParticleEMomDev   ("Jet3", "Jet3", 0, &empty4x4);
    lepB_= new TFitParticleEMomDev   ("Jet4", "Jet4", 0, &empty4x4);
    break;
  case kEtEtaPhi :
    hadB_= new TFitParticleEtEtaPhi  ("Jet1", "Jet1", 0, &empty3x3);
    hadP_= new TFitParticleEtEtaPhi  ("Jet2", "Jet2", 0, &empty3x3);
    hadQ_= new TFitParticleEtEtaPhi  ("Jet3", "Jet3", 0, &empty3x3);
    lepB_= new TFitParticleEtEtaPhi  ("Jet4", "Jet4", 0, &empty3x3);
    break;
  case kEtThetaPhi :
    hadB_= new TFitParticleEtThetaPhi("Jet1", "Jet1", 0, &empty3x3);
    hadP_= new TFitParticleEtThetaPhi("Jet2", "Jet2", 0, &empty3x3);
    hadQ_= new TFitParticleEtThetaPhi("Jet3", "Jet3", 0, &empty3x3);
    lepB_= new TFitParticleEtThetaPhi("Jet4", "Jet4", 0, &empty3x3);
    break;
  }
}

void TtSemiLepKinFitter::setupLeptons()
{
  TMatrixD empty3x3(3,3); 
  switch(lepParam_){ // setup lepton according to parameterization
  case kEMom       : lepton_  = new TFitParticleEScaledMomDev("Lepton",   "Lepton",   0, &empty3x3); break;
  case kEtEtaPhi   : lepton_  = new TFitParticleEtEtaPhi     ("Lepton",   "Lepton",   0, &empty3x3); break;
  case kEtThetaPhi : lepton_  = new TFitParticleEtThetaPhi   ("Lepton",   "Lepton",   0, &empty3x3); break;
  }
  switch(metParam_){ // setup neutrino according to parameterization
  case kEMom       : neutrino_= new TFitParticleEScaledMomDev("Neutrino", "Neutrino", 0, &empty3x3); break;
  case kEtEtaPhi   : neutrino_= new TFitParticleEtEtaPhi     ("Neutrino", "Neutrino", 0, &empty3x3); break;
  case kEtThetaPhi : neutrino_= new TFitParticleEtThetaPhi   ("Neutrino", "Neutrino", 0, &empty3x3); break;
  }
}

void TtSemiLepKinFitter::setupConstraints() 
{
  massConstr_[kWHadMass      ] = new TFitConstraintM("WMassHad",      "WMassHad",      0, 0, mW_  );
  massConstr_[kWLepMass      ] = new TFitConstraintM("WMassLep",      "WMassLep",      0, 0, mW_  );
  massConstr_[kTopHadMass    ] = new TFitConstraintM("TopMassHad",    "TopMassHad",    0, 0, mTop_);
  massConstr_[kTopLepMass    ] = new TFitConstraintM("TopMassLep",    "TopMassLep",    0, 0, mTop_);
  massConstr_[kNeutrinoMass  ] = new TFitConstraintM("NeutrinoMass",  "NeutrinoMass",  0, 0,    0.);
  massConstr_[kEqualTopMasses] = new TFitConstraintM("EqualTopMasses","EqualTopMasses",0, 0,    0.);

  massConstr_[kWHadMass      ]->addParticles1(hadP_,   hadQ_    );
  massConstr_[kWLepMass      ]->addParticles1(lepton_, neutrino_);
  massConstr_[kTopHadMass    ]->addParticles1(hadP_, hadQ_, hadB_);
  massConstr_[kTopLepMass    ]->addParticles1(lepton_, neutrino_, lepB_);
  massConstr_[kNeutrinoMass  ]->addParticle1 (neutrino_);
  massConstr_[kEqualTopMasses]->addParticles1(hadP_, hadQ_, hadB_);
  massConstr_[kEqualTopMasses]->addParticles2(lepton_, neutrino_, lepB_);
}

void TtSemiLepKinFitter::setupFitter() 
{
  printSetup();

  setupJets();
  setupLeptons();
  setupConstraints();

  // add measured particles
  fitter_->addMeasParticle(hadB_);
  fitter_->addMeasParticle(hadP_);
  fitter_->addMeasParticle(hadQ_);
  fitter_->addMeasParticle(lepB_);
  fitter_->addMeasParticle(lepton_);
  fitter_->addMeasParticle(neutrino_);
  
  //std::cout << " constrList_.size() " << constrList_.size() <<std::endl;
  
  // add constraints
  for(unsigned int i=0; i<constrList_.size(); i++){
    fitter_->addConstraint(massConstr_[constrList_[i]]);
  }
}

//AC template <class LeptonType>
//int TtSemiLepKinFitter::fit(const std::vector<pat::Jet>& jets, const pat::Lepton<LeptonType>& lepton, const pat::MET& neutrino, const double jetEnergyResolutionSmearFactor = 1.)
int TtSemiLepKinFitter::fit(const std::vector<NTJet>& jets, const NTLepton& lepton, std::string lepType, const NTMET& neutrino, const double jetEnergyResolutionSmearFactor = 1.)
{
  if( jets.size()<4 ) {
    std::cout <<"Cannot run the TtSemiLepKinFitter with less than 4 jets"<<std::endl;
    //??throw std::exception;
  }

  // get jets in right order
  NTJet hadP = jets[TtSemiLepKinFitter::LightQ ];
  NTJet hadQ = jets[TtSemiLepKinFitter::LightQBar];
  NTJet hadB = jets[TtSemiLepKinFitter::HadB   ];
  NTJet lepB = jets[TtSemiLepKinFitter::LepB   ];
 
  // initialize particles
  TLorentzVector p4HadP( hadP.p4.Px(), hadP.p4.Py(), hadP.p4.Pz(), hadP.p4.E() );
  TLorentzVector p4HadQ( hadQ.p4.Px(), hadQ.p4.Py(), hadQ.p4.Pz(), hadQ.p4.E() );
  TLorentzVector p4HadB( hadB.p4.Px(), hadB.p4.Py(), hadB.p4.Pz(), hadB.p4.E() );
  TLorentzVector p4LepB( lepB.p4.Px(), lepB.p4.Py(), lepB.p4.Pz(), lepB.p4.E() );
  TLorentzVector p4Lepton  ( lepton.p4.Px(), lepton.p4.Py(), lepton.p4.Pz(), lepton.p4.E() );
  TLorentzVector p4Neutrino( neutrino.p4.Px(), neutrino.p4.Py(), 0, neutrino.p4.Et() );


  // initialize covariance matrices
  CovarianceMatrix covM;
  TMatrixD m1 = covM.setupMatrix(p4HadP, "jet", jetParam_);
  TMatrixD m2 = covM.setupMatrix(p4HadQ, "jet", jetParam_);
  TMatrixD m3 = covM.setupMatrix(p4HadB, "jet", jetParam_, "bjets");
  TMatrixD m4 = covM.setupMatrix(p4LepB, "jet", jetParam_, "bjets");
  TMatrixD m5 = covM.setupMatrix(p4Lepton, lepType,  lepParam_);
  TMatrixD m6 = covM.setupMatrix(p4Neutrino, "met",  metParam_);

  // as covM contains resolution^2
  // the correction of jet energy resolutions
  // is just *jetEnergyResolutionSmearFactor^2
  m1(0,0)*=jetEnergyResolutionSmearFactor * jetEnergyResolutionSmearFactor; 
  m2(0,0)*=jetEnergyResolutionSmearFactor * jetEnergyResolutionSmearFactor; 
  m3(0,0)*=jetEnergyResolutionSmearFactor * jetEnergyResolutionSmearFactor; 
  m4(0,0)*=jetEnergyResolutionSmearFactor * jetEnergyResolutionSmearFactor; 

  // set the kinematics of the objects to be fitted
  hadP_->setIni4Vec( &p4HadP );
  hadQ_->setIni4Vec( &p4HadQ );
  hadB_->setIni4Vec( &p4HadB );
  lepB_->setIni4Vec( &p4LepB );
  lepton_->setIni4Vec( &p4Lepton );
  neutrino_->setIni4Vec( &p4Neutrino );

  hadP_->setCovMatrix( &m1 );
  hadQ_->setCovMatrix( &m2 );
  hadB_->setCovMatrix( &m3 );
  lepB_->setCovMatrix( &m4 );
  lepton_->setCovMatrix( &m5 );
  neutrino_->setCovMatrix( &m6 );

  // now do the fit
  fitter_->fit();

  // read back the resulting particles if the fit converged
  if(fitter_->getStatus()==0){
    // read back jet kinematics
    /*
    fittedHadP_= pat::Particle(reco::LeafCandidate(0, math::XYZTLorentzVector(hadP_->getCurr4Vec()->X(),
                               hadP_->getCurr4Vec()->Y(), hadP_->getCurr4Vec()->Z(), hadP_->getCurr4Vec()->E()), math::XYZPoint()));
    fittedHadQ_= pat::Particle(reco::LeafCandidate(0, math::XYZTLorentzVector(hadQ_->getCurr4Vec()->X(),
                               hadQ_->getCurr4Vec()->Y(), hadQ_->getCurr4Vec()->Z(), hadQ_->getCurr4Vec()->E()), math::XYZPoint()));
    fittedHadB_= pat::Particle(reco::LeafCandidate(0, math::XYZTLorentzVector(hadB_->getCurr4Vec()->X(),
                               hadB_->getCurr4Vec()->Y(), hadB_->getCurr4Vec()->Z(), hadB_->getCurr4Vec()->E()), math::XYZPoint()));
    fittedLepB_= pat::Particle(reco::LeafCandidate(0, math::XYZTLorentzVector(lepB_->getCurr4Vec()->X(),
                               lepB_->getCurr4Vec()->Y(), lepB_->getCurr4Vec()->Z(), lepB_->getCurr4Vec()->E()), math::XYZPoint()));

    // read back lepton kinematics
    fittedLepton_= pat::Particle(reco::LeafCandidate(lepton.charge(), math::XYZTLorentzVector(lepton_->getCurr4Vec()->X(),
                                 lepton_->getCurr4Vec()->Y(), lepton_->getCurr4Vec()->Z(), lepton_->getCurr4Vec()->E()), math::XYZPoint()));

    // read back the MET kinematics
    fittedNeutrino_= pat::Particle(reco::LeafCandidate(0, math::XYZTLorentzVector(neutrino_->getCurr4Vec()->X(),
                                   neutrino_->getCurr4Vec()->Y(), neutrino_->getCurr4Vec()->Z(), neutrino_->getCurr4Vec()->E()), math::XYZPoint()));
   */
    fittedHadP_= new TLorentzVector(hadP_->getCurr4Vec()->X(),hadP_->getCurr4Vec()->Y(), hadP_->getCurr4Vec()->Z(), hadP_->getCurr4Vec()->E());
    fittedHadQ_= new TLorentzVector(hadQ_->getCurr4Vec()->X(),hadQ_->getCurr4Vec()->Y(), hadQ_->getCurr4Vec()->Z(), hadQ_->getCurr4Vec()->E());         
    fittedHadB_= new TLorentzVector(hadB_->getCurr4Vec()->X(),hadB_->getCurr4Vec()->Y(), hadB_->getCurr4Vec()->Z(), hadB_->getCurr4Vec()->E());
    fittedLepB_= new TLorentzVector(lepB_->getCurr4Vec()->X(),lepB_->getCurr4Vec()->Y(), lepB_->getCurr4Vec()->Z(), lepB_->getCurr4Vec()->E());

    // read back lepton kinematics
    fittedLepton_= new TLorentzVector(lepton_->getCurr4Vec()->X(),lepton_->getCurr4Vec()->Y(), lepton_->getCurr4Vec()->Z(), lepton_->getCurr4Vec()->E());

    // read back the MET kinematics
    fittedNeutrino_= new TLorentzVector(neutrino_->getCurr4Vec()->X(),neutrino_->getCurr4Vec()->Y(), neutrino_->getCurr4Vec()->Z(), neutrino_->getCurr4Vec()->E());
  
  }
  return fitter_->getStatus();
}

/*TtSemiEvtSolution TtSemiLepKinFitter::addKinFitInfo(TtSemiEvtSolution* asol, const double jetEnergyResolutionSmearFactor) 
{

  TtSemiEvtSolution fitsol(*asol);

  std::vector<pat::Jet> jets;
  jets.resize(4);
  jets[TtSemiLepEvtPartons::LightQ   ] = fitsol.getCalHadp();
  jets[TtSemiLepEvtPartons::LightQBar] = fitsol.getCalHadq();
  jets[TtSemiLepEvtPartons::HadB     ] = fitsol.getCalHadb();
  jets[TtSemiLepEvtPartons::LepB     ] = fitsol.getCalLepb();

  // perform the fit, either using the electron or the muon
  if(fitsol.getDecay() == "electron") fit( jets, fitsol.getCalLepe(), fitsol.getCalLepn(), jetEnergyResolutionSmearFactor);
  if(fitsol.getDecay() == "muon"    ) fit( jets, fitsol.getCalLepm(), fitsol.getCalLepn(), jetEnergyResolutionSmearFactor);
  
  // add fitted information to the solution
  if (fitter_->getStatus() == 0) {
    // fill the fitted particles
    fitsol.setFitHadb( fittedHadB() );
    fitsol.setFitHadp( fittedHadP() );
    fitsol.setFitHadq( fittedHadQ() );
    fitsol.setFitLepb( fittedLepB() );
    fitsol.setFitLepl( fittedLepton() );
    fitsol.setFitLepn( fittedNeutrino() );
    // store the fit's chi2 probability
    fitsol.setProbChi2( fitProb() );
  }
  return fitsol;
}*/

