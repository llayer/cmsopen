#ifndef TtSemiLepKinFitProducer_h
#define TtSemiLepKinFitProducer_h

#include <list>

#include "combination.h"
/*AC 
#include "AnalysisDataFormats/TopObjects/interface/TtSemiLepEvtPartons.h"
relevant enum declared in TtSemiLepKinFitter.h*/

#include "TtSemiLepKinFitter.h"

//ACtemplate <typename LeptonCollection>
class TtSemiLepKinFitProducer {
  
 public:
  
  TtSemiLepKinFitProducer();
  TtSemiLepKinFitProducer(std::vector<unsigned> constr);
  ~TtSemiLepKinFitProducer();
  void SetupFitParameters(bool useOnlyMatch, std::string bTagAlgo, double minBTagValueBJet, double maxBTagValueNonBJet, bool useBTag,              
  int maxNJets, int maxNComb, unsigned int  maxNrIter, double maxDeltaS, double maxF,             
  unsigned int jetParam, unsigned int lepParam, unsigned int  metParam,              
  std::vector<unsigned> constraints, double mW, double mTop, double
  jetEnergyResolutionSmearFactor);
  
  inline const int GetNConvertedFits() const {return FitResultList.size();} ;  
  
  double GetFitChi2(int i = 0);
  
  //inline double GetFitChi2(int i = 0) {std::list<KinFitResult>::iterator it = FitResultList.begin(); std::advance(it, i);return it->Chi2;} ;   
  inline double GetFitProb(int i = 0) {std::list<KinFitResult>::iterator it = FitResultList.begin(); std::advance(it, i);return it->Prob;} ;  
  inline TLorentzVector GetFitHadB(int i = 0) {std::list<KinFitResult>::iterator it = FitResultList.begin(); std::advance(it, i);return it->HadB;} ;  
  inline TLorentzVector GetFitHadP(int i = 0) {std::list<KinFitResult>::iterator it = FitResultList.begin(); std::advance(it, i);return it->HadP;} ;  
  inline TLorentzVector GetFitHadQ(int i = 0) {std::list<KinFitResult>::iterator it = FitResultList.begin(); std::advance(it, i);return it->HadQ;} ;  
  inline TLorentzVector GetFitLepB(int i = 0) {std::list<KinFitResult>::iterator it = FitResultList.begin(); std::advance(it, i);return it->LepB;} ;  
  inline TLorentzVector GetFitLepL(int i = 0) {std::list<KinFitResult>::iterator it = FitResultList.begin(); std::advance(it, i);return it->LepL;} ;  
  inline TLorentzVector GetFitLepN(int i = 0) {std::list<KinFitResult>::iterator it = FitResultList.begin(); std::advance(it, i);return it->LepN;} ;  
  inline std::vector<int> GetFitJetCombi(int i = 0) {std::list<KinFitResult>::iterator it = FitResultList.begin(); std::advance(it, i);return it->JetCombi;}; ;  
 
  void produce(const vector<NTJet>& jets, const vector<NTLepton>& leps, std::string lepType, const  vector<NTMET>& mets);

 private:
  // produce
 
  // convert unsigned to Param
  TtSemiLepKinFitter::Param param(unsigned);
  // convert unsigned to Param
  TtSemiLepKinFitter::Constraint constraint(unsigned);
  // convert unsigned to Param
  std::vector<TtSemiLepKinFitter::Constraint> constraints(std::vector<unsigned>&);
  // helper function for b-tagging
  bool doBTagging(bool& useBTag_, const vector<NTJet>& jets, std::vector<int>& combi,
                  std::string& bTagAlgo_, double& minBTagValueBJets_, double& maxBTagValueNonBJets_);

  /*
  edm::InputTag jets_;
  edm::InputTag leps_;
  edm::InputTag mets_;*/
  
  //edm::InputTag match_;//?
  /// switch to use only a combination given by another hypothesis
  bool useOnlyMatch_;
  /// input tag for b-tagging algorithm
  std::string bTagAlgo_;
  /// min value of bTag for a b-jet
  double minBTagValueBJet_;
  /// max value of bTag for a non-b-jet
  double maxBTagValueNonBJet_;
  /// switch to tell whether to use b-tagging or not
  bool useBTag_;
  /// maximal number of jets (-1 possible to indicate 'all')
  int maxNJets_;
  /// maximal number of combinations to be written to the event
  int maxNComb_;

  /// maximal number of iterations to be performed for the fit
  unsigned int maxNrIter_;
  /// maximal chi2 equivalent
  double maxDeltaS_;
  /// maximal deviation for contstraints
  double maxF_;
  unsigned int jetParam_;
  unsigned int lepParam_;
  unsigned int metParam_;
  /// constrains
  std::vector<unsigned> constraints_;
  double mW_;
  double mTop_;
  /// smear factor for jet resolutions
  double jetEnergyResolutionSmearFactor_;

  TtSemiLepKinFitter* fitter;
    
  struct KinFitResult {
    int Status;
    double Chi2;
    double Prob;
    TLorentzVector HadB;
    TLorentzVector HadP;
    TLorentzVector HadQ;
    TLorentzVector LepB;
    TLorentzVector LepL;
    TLorentzVector LepN;
    std::vector<int> JetCombi;
    bool operator< (const KinFitResult& rhs) { return Chi2 < rhs.Chi2; };
  };  
   
   std::list<KinFitResult> FitResultList;

};

#endif

