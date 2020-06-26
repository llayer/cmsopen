#ifndef TtSemiLepKinFitter_h
#define TtSemiLepKinFitter_h

#include <vector>
#include <map>

#include "TLorentzVector.h"

//AC#include "DataFormats/PatCandidates/interface/Lepton.h"

//AC#include "AnalysisDataFormats/TopObjects/interface/TtSemiEvtSolution.h"

//AC
#include "../../../../MiniTreeFormat/NTFormat/interface/NTLepton.h"
#include "../../../../MiniTreeFormat/NTFormat/interface/NTJet.h"
#include "../../../../MiniTreeFormat/NTFormat/interface/NTMET.h"

#include "../interface/TopKinFitter.h"

class TAbsFitParticle;
class TFitConstraintM;

using namespace TopTree;

/*
  \class   TtSemiLepKinFitter TtSemiLepKinFitter.h "TopQuarkAnalysis/TopKinFitter/interface/TtSemiLepKinFitter.h"
  
  \brief   one line description to be added here...

  text to be added here...
  
**/

class TtSemiLepKinFitter : public TopKinFitter {
  
 public:
  
  /// supported constraints
  enum Constraint { kWHadMass = 1, kWLepMass, kTopHadMass, kTopLepMass, kNeutrinoMass, kEqualTopMasses };
  //AC from AnalysisDataFormats/TopObjects/interface/TtSemiLepEvtPartons.h
  enum { LightQ, LightQBar, HadB, LepB, Lepton };
 
 public:
  /// default constructor
  explicit TtSemiLepKinFitter();
  /// constructor initialized with built-in types and class enum's custom parameters
  explicit TtSemiLepKinFitter(Param jetParam, Param lepParam, Param metParam, int maxNrIter, double maxDeltaS, double maxF,
                              std::vector<Constraint> constraints, double mW=80.4, double mTop=173.);
  /// default destructor
  ~TtSemiLepKinFitter();

  /// kinematic fit interface
  int fit(const std::vector<NTJet>& jets, const NTLepton& lepton, std::string lepType, const NTMET& neutrino, const double jetResolutionSmearFactor);
  // return hadronic b quark candidate
  const TLorentzVector fittedHadB() const { return (fitter_->getStatus()==0 ? *fittedHadB_: TLorentzVector()); };
  // return hadronic light quark candidate
  const TLorentzVector fittedHadP() const { return (fitter_->getStatus()==0 ? *fittedHadP_ : TLorentzVector()); };
  // return hadronic light quark candidate
  const TLorentzVector fittedHadQ() const { return (fitter_->getStatus()==0 ? *fittedHadQ_ : TLorentzVector()); };
  // return leptonic b quark candidate
  const TLorentzVector fittedLepB() const { return (fitter_->getStatus()==0 ? *fittedLepB_ : TLorentzVector()); };
  // return lepton candidate
  const TLorentzVector fittedLepton() const { return (fitter_->getStatus()==0 ? *fittedLepton_ : TLorentzVector()); };
  // return neutrino candidate
  const TLorentzVector fittedNeutrino() const { return (fitter_->getStatus()==0 ? *fittedNeutrino_ : TLorentzVector()); };
  /// add kin fit information to the old event solution (in for legacy reasons)
  //AC TtSemiEvtSolution addKinFitInfo(TtSemiEvtSolution* asol, const double jetResolutionSmearFactor=1.);
  
 private:
  /// print fitter setup  
  void printSetup() const;
  /// setup fitter  
  void setupFitter();
  /// initialize jet inputs
  void setupJets();
  /// initialize lepton inputs
  void setupLeptons();
  /// initialize constraints
  void setupConstraints();
  
 private:
  // input particles
  TAbsFitParticle* hadB_;
  TAbsFitParticle* hadP_;
  TAbsFitParticle* hadQ_;
  TAbsFitParticle* lepB_;
  TAbsFitParticle* lepton_;
  TAbsFitParticle* neutrino_;
  // supported constraints
  std::map<Constraint, TFitConstraintM*> massConstr_;
  
  // output particles
  //AC
  TLorentzVector* fittedHadB_;
  TLorentzVector* fittedHadP_;
  TLorentzVector* fittedHadQ_;
  TLorentzVector* fittedLepB_;
  TLorentzVector* fittedLepton_;
  TLorentzVector* fittedNeutrino_;
  
  /// jet parametrization
  Param  jetParam_;
  /// lepton parametrization
  Param lepParam_;
  /// met parametrization
  Param metParam_;
  /// vector of constraints to be used
  std::vector<Constraint> constrList_;  
};

#endif
