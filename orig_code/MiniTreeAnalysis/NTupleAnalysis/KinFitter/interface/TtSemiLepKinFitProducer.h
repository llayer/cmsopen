1	#ifndef TtSemiLepKinFitProducer_h
2	#define TtSemiLepKinFitProducer_h
3	
4	#include <list>
5	
6	#include "combination.h"
7	/*AC 
8	#include "AnalysisDataFormats/TopObjects/interface/TtSemiLepEvtPartons.h"
9	relevant enum declared in TtSemiLepKinFitter.h*/
10	
11	#include "TtSemiLepKinFitter.h"
12	
13	//ACtemplate <typename LeptonCollection>
14	class TtSemiLepKinFitProducer {
15	  
16	 public:
17	  
18	  TtSemiLepKinFitProducer();
19	  TtSemiLepKinFitProducer(std::vector<unsigned> constr);
20	  ~TtSemiLepKinFitProducer();
21	  void SetupFitParameters(bool useOnlyMatch, std::string bTagAlgo, double minBTagValueBJet, double maxBTagValueNonBJet, bool useBTag,              
22	  int maxNJets, int maxNComb, unsigned int  maxNrIter, double maxDeltaS, double maxF,             
23	  unsigned int jetParam, unsigned int lepParam, unsigned int  metParam,              
24	  std::vector<unsigned> constraints, double mW, double mTop, double
25	  jetEnergyResolutionSmearFactor);
26	  
27	  inline const int GetNConvertedFits() const {return FitResultList.size();} ;  
28	  
29	  double GetFitChi2(int i = 0);
30	  
31	  //inline double GetFitChi2(int i = 0) {std::list<KinFitResult>::iterator it = FitResultList.begin(); std::advance(it, i);return it->Chi2;} ;   
32	  inline double GetFitProb(int i = 0) {std::list<KinFitResult>::iterator it = FitResultList.begin(); std::advance(it, i);return it->Prob;} ;  
33	  inline TLorentzVector GetFitHadB(int i = 0) {std::list<KinFitResult>::iterator it = FitResultList.begin(); std::advance(it, i);return it->HadB;} ;  
34	  inline TLorentzVector GetFitHadP(int i = 0) {std::list<KinFitResult>::iterator it = FitResultList.begin(); std::advance(it, i);return it->HadP;} ;  
35	  inline TLorentzVector GetFitHadQ(int i = 0) {std::list<KinFitResult>::iterator it = FitResultList.begin(); std::advance(it, i);return it->HadQ;} ;  
36	  inline TLorentzVector GetFitLepB(int i = 0) {std::list<KinFitResult>::iterator it = FitResultList.begin(); std::advance(it, i);return it->LepB;} ;  
37	  inline TLorentzVector GetFitLepL(int i = 0) {std::list<KinFitResult>::iterator it = FitResultList.begin(); std::advance(it, i);return it->LepL;} ;  
38	  inline TLorentzVector GetFitLepN(int i = 0) {std::list<KinFitResult>::iterator it = FitResultList.begin(); std::advance(it, i);return it->LepN;} ;  
39	  inline std::vector<int> GetFitJetCombi(int i = 0) {std::list<KinFitResult>::iterator it = FitResultList.begin(); std::advance(it, i);return it->JetCombi;}; ;  
40	 
41	  void produce(const vector<NTJet>& jets, const vector<NTLepton>& leps, std::string lepType, const  vector<NTMET>& mets);
42	
43	 private:
44	  // produce
45	 
46	  // convert unsigned to Param
47	  TtSemiLepKinFitter::Param param(unsigned);
48	  // convert unsigned to Param
49	  TtSemiLepKinFitter::Constraint constraint(unsigned);
50	  // convert unsigned to Param
51	  std::vector<TtSemiLepKinFitter::Constraint> constraints(std::vector<unsigned>&);
52	  // helper function for b-tagging
53	  bool doBTagging(bool& useBTag_, const vector<NTJet>& jets, std::vector<int>& combi,
54	                  std::string& bTagAlgo_, double& minBTagValueBJets_, double& maxBTagValueNonBJets_);
55	
56	  /*
57	  edm::InputTag jets_;
58	  edm::InputTag leps_;
59	  edm::InputTag mets_;*/
60	  
61	  //edm::InputTag match_;//?
62	  /// switch to use only a combination given by another hypothesis
63	  bool useOnlyMatch_;
64	  /// input tag for b-tagging algorithm
65	  std::string bTagAlgo_;
66	  /// min value of bTag for a b-jet
67	  double minBTagValueBJet_;
68	  /// max value of bTag for a non-b-jet
69	  double maxBTagValueNonBJet_;
70	  /// switch to tell whether to use b-tagging or not
71	  bool useBTag_;
72	  /// maximal number of jets (-1 possible to indicate 'all')
73	  int maxNJets_;
74	  /// maximal number of combinations to be written to the event
75	  int maxNComb_;
76	
77	  /// maximal number of iterations to be performed for the fit
78	  unsigned int maxNrIter_;
79	  /// maximal chi2 equivalent
80	  double maxDeltaS_;
81	  /// maximal deviation for contstraints
82	  double maxF_;
83	  unsigned int jetParam_;
84	  unsigned int lepParam_;
85	  unsigned int metParam_;
86	  /// constrains
87	  std::vector<unsigned> constraints_;
88	  double mW_;
89	  double mTop_;
90	  /// smear factor for jet resolutions
91	  double jetEnergyResolutionSmearFactor_;
92	
93	  TtSemiLepKinFitter* fitter;
94	    
95	  struct KinFitResult {
96	    int Status;
97	    double Chi2;
98	    double Prob;
99	    TLorentzVector HadB;
100	    TLorentzVector HadP;
101	    TLorentzVector HadQ;
102	    TLorentzVector LepB;
103	    TLorentzVector LepL;
104	    TLorentzVector LepN;
105	    std::vector<int> JetCombi;
106	    bool operator< (const KinFitResult& rhs) { return Chi2 < rhs.Chi2; };
107	  };  
108	   
109	   std::list<KinFitResult> FitResultList;
110	
111	};
112	
113	#endif

