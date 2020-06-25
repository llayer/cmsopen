1	#ifndef TtSemiLepKinFitter_h
2	#define TtSemiLepKinFitter_h
3	
4	#include <vector>
5	#include <map>
6	
7	#include "TLorentzVector.h"
8	
9	//AC#include "DataFormats/PatCandidates/interface/Lepton.h"
10	
11	//AC#include "AnalysisDataFormats/TopObjects/interface/TtSemiEvtSolution.h"
12	
13	//AC
14	#include "../../../../MiniTreeFormat/NTFormat/interface/NTLepton.h"
15	#include "../../../../MiniTreeFormat/NTFormat/interface/NTJet.h"
16	#include "../../../../MiniTreeFormat/NTFormat/interface/NTMET.h"
17	
18	#include "../interface/TopKinFitter.h"
19	
20	class TAbsFitParticle;
21	class TFitConstraintM;
22	
23	using namespace TopTree;
24	
25	/*
26	  \class   TtSemiLepKinFitter TtSemiLepKinFitter.h "TopQuarkAnalysis/TopKinFitter/interface/TtSemiLepKinFitter.h"
27	  
28	  \brief   one line description to be added here...
29	
30	  text to be added here...
31	  
32	**/
33	
34	class TtSemiLepKinFitter : public TopKinFitter {
35	  
36	 public:
37	  
38	  /// supported constraints
39	  enum Constraint { kWHadMass = 1, kWLepMass, kTopHadMass, kTopLepMass, kNeutrinoMass, kEqualTopMasses };
40	  //AC from AnalysisDataFormats/TopObjects/interface/TtSemiLepEvtPartons.h
41	  enum { LightQ, LightQBar, HadB, LepB, Lepton };
42	 
43	 public:
44	  /// default constructor
45	  explicit TtSemiLepKinFitter();
46	  /// constructor initialized with built-in types and class enum's custom parameters
47	  explicit TtSemiLepKinFitter(Param jetParam, Param lepParam, Param metParam, int maxNrIter, double maxDeltaS, double maxF,
48	                              std::vector<Constraint> constraints, double mW=80.4, double mTop=173.);
49	  /// default destructor
50	  ~TtSemiLepKinFitter();
51	
52	  /// kinematic fit interface
53	  int fit(const std::vector<NTJet>& jets, const NTLepton& lepton, std::string lepType, const NTMET& neutrino, const double jetResolutionSmearFactor);
54	  // return hadronic b quark candidate
55	  const TLorentzVector fittedHadB() const { return (fitter_->getStatus()==0 ? *fittedHadB_: TLorentzVector()); };
56	  // return hadronic light quark candidate
57	  const TLorentzVector fittedHadP() const { return (fitter_->getStatus()==0 ? *fittedHadP_ : TLorentzVector()); };
58	  // return hadronic light quark candidate
59	  const TLorentzVector fittedHadQ() const { return (fitter_->getStatus()==0 ? *fittedHadQ_ : TLorentzVector()); };
60	  // return leptonic b quark candidate
61	  const TLorentzVector fittedLepB() const { return (fitter_->getStatus()==0 ? *fittedLepB_ : TLorentzVector()); };
62	  // return lepton candidate
63	  const TLorentzVector fittedLepton() const { return (fitter_->getStatus()==0 ? *fittedLepton_ : TLorentzVector()); };
64	  // return neutrino candidate
65	  const TLorentzVector fittedNeutrino() const { return (fitter_->getStatus()==0 ? *fittedNeutrino_ : TLorentzVector()); };
66	  /// add kin fit information to the old event solution (in for legacy reasons)
67	  //AC TtSemiEvtSolution addKinFitInfo(TtSemiEvtSolution* asol, const double jetResolutionSmearFactor=1.);
68	  
69	 private:
70	  /// print fitter setup  
71	  void printSetup() const;
72	  /// setup fitter  
73	  void setupFitter();
74	  /// initialize jet inputs
75	  void setupJets();
76	  /// initialize lepton inputs
77	  void setupLeptons();
78	  /// initialize constraints
79	  void setupConstraints();
80	  
81	 private:
82	  // input particles
83	  TAbsFitParticle* hadB_;
84	  TAbsFitParticle* hadP_;
85	  TAbsFitParticle* hadQ_;
86	  TAbsFitParticle* lepB_;
87	  TAbsFitParticle* lepton_;
88	  TAbsFitParticle* neutrino_;
89	  // supported constraints
90	  std::map<Constraint, TFitConstraintM*> massConstr_;
91	  
92	  // output particles
93	  //AC
94	  TLorentzVector* fittedHadB_;
95	  TLorentzVector* fittedHadP_;
96	  TLorentzVector* fittedHadQ_;
97	  TLorentzVector* fittedLepB_;
98	  TLorentzVector* fittedLepton_;
99	  TLorentzVector* fittedNeutrino_;
100	  
101	  /// jet parametrization
102	  Param  jetParam_;
103	  /// lepton parametrization
104	  Param lepParam_;
105	  /// met parametrization
106	  Param metParam_;
107	  /// vector of constraints to be used
108	  std::vector<Constraint> constrList_;  
109	};
110	
111	#endif
