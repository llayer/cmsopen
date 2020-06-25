1	#include "../interface/TFitConstraintM.h"
2	#include "../interface/TAbsFitParticle.h"
3	#include "../interface/TFitParticleEMomDev.h"
4	#include "../interface/TFitParticleEtEtaPhi.h"
5	#include "../interface/TFitParticleEtThetaPhi.h"
6	#include "../interface/TFitParticleEScaledMomDev.h"
7	
8	//AC #include "AnalysisDataFormats/TopObjects/interface/TtSemiLepEvtPartons.h"
9	#include "../interface/TtSemiLepKinFitter.h"
10	#include "../interface/CovarianceMatrix.h"
11	
12	# include<sstream>
13	
14	//#include "FWCore/MessageLogger/interface/MessageLogger.h"
15	
16	/// default configuration is: Parametrization kEMom, Max iterations = 200, deltaS<= 5e-5, maxF<= 1e-4, no constraints
17	TtSemiLepKinFitter::TtSemiLepKinFitter():
18	  TopKinFitter(),
19	  hadB_(0), hadP_(0), hadQ_(0), lepB_(0), lepton_(0), neutrino_(0),
20	  jetParam_(kEMom), lepParam_(kEMom), metParam_(kEMom)
21	{
22	  setupFitter();
23	}
24	
25	TtSemiLepKinFitter::TtSemiLepKinFitter(Param jetParam, Param lepParam, Param metParam,
26	                                       int maxNrIter, double maxDeltaS, double maxF,
27	                                       std::vector<Constraint> constraints, double mW, double mTop):
28	  TopKinFitter(maxNrIter, maxDeltaS, maxF, mW, mTop),
29	  hadB_(0), hadP_(0), hadQ_(0), lepB_(0), lepton_(0), neutrino_(0),
30	  jetParam_(jetParam), lepParam_(lepParam), metParam_(metParam), constrList_(constraints)
31	{
32	  setupFitter();
33	}
34	
35	TtSemiLepKinFitter::~TtSemiLepKinFitter() 
36	{
37	  delete hadB_; 
38	  delete hadP_; 
39	  delete hadQ_;
40	  delete lepB_; 
41	  delete lepton_; 
42	  delete neutrino_;
43	  for(std::map<Constraint, TFitConstraintM*>::iterator it = massConstr_.begin(); it != massConstr_.end(); ++it)
44	    delete it->second;
45	}
46	
47	void TtSemiLepKinFitter::printSetup() const
48	{
49	  std::stringstream constr;
50	  for(unsigned int i=0; i<constrList_.size(); ++i){
51	    switch(constrList_[i]){
52	    case kWHadMass       : constr << "    * hadronic W-mass (" << mW_   << " GeV) \n"; break;
53	    case kWLepMass       : constr << "    * leptonic W-mass (" << mW_   << " GeV) \n"; break;
54	    case kTopHadMass     : constr << "    * hadronic t-mass (" << mTop_ << " GeV) \n"; break;
55	    case kTopLepMass     : constr << "    * leptonic t-mass (" << mTop_ << " GeV) \n"; break;
56	    case kNeutrinoMass   : constr << "    * neutrino   mass (0 GeV) \n"; break;
57	    case kEqualTopMasses : constr << "    * equal    t-masses \n"; break;
58	    }
59	  }
60	  std::cout << "TtSemiLepKinFitter"  
61	    << "\n"
62	    << "+++++++++++ TtSemiLepKinFitter Setup ++++++++++++ \n"
63	    << "  Parametrization:                                \n" 
64	    << "   * jet : " << param(jetParam_) << "\n"
65	    << "   * lep : " << param(lepParam_) << "\n"
66	    << "   * met : " << param(metParam_) << "\n"
67	    << "  Constraints:                                    \n"
68	    <<    constr.str()
69	    << "  Max(No iterations): " << maxNrIter_ << "\n"
70	    << "  Max(deltaS)       : " << maxDeltaS_ << "\n"
71	    << "  Max(F)            : " << maxF_      << "\n"
72	    << "+++++++++++++++++++++++++++++++++++++++++++++++++ \n";
73	}
74	
75	void TtSemiLepKinFitter::setupJets()
76	{
77	  TMatrixD empty3x3(3,3); 
78	  TMatrixD empty4x4(4,4);
79	  switch(jetParam_){ // setup jets according to parameterization
80	  case kEMom :
81	    hadB_= new TFitParticleEMomDev   ("Jet1", "Jet1", 0, &empty4x4);
82	    hadP_= new TFitParticleEMomDev   ("Jet2", "Jet2", 0, &empty4x4);
83	    hadQ_= new TFitParticleEMomDev   ("Jet3", "Jet3", 0, &empty4x4);
84	    lepB_= new TFitParticleEMomDev   ("Jet4", "Jet4", 0, &empty4x4);
85	    break;
86	  case kEtEtaPhi :
87	    hadB_= new TFitParticleEtEtaPhi  ("Jet1", "Jet1", 0, &empty3x3);
88	    hadP_= new TFitParticleEtEtaPhi  ("Jet2", "Jet2", 0, &empty3x3);
89	    hadQ_= new TFitParticleEtEtaPhi  ("Jet3", "Jet3", 0, &empty3x3);
90	    lepB_= new TFitParticleEtEtaPhi  ("Jet4", "Jet4", 0, &empty3x3);
91	    break;
92	  case kEtThetaPhi :
93	    hadB_= new TFitParticleEtThetaPhi("Jet1", "Jet1", 0, &empty3x3);
94	    hadP_= new TFitParticleEtThetaPhi("Jet2", "Jet2", 0, &empty3x3);
95	    hadQ_= new TFitParticleEtThetaPhi("Jet3", "Jet3", 0, &empty3x3);
96	    lepB_= new TFitParticleEtThetaPhi("Jet4", "Jet4", 0, &empty3x3);
97	    break;
98	  }
99	}
100	
101	void TtSemiLepKinFitter::setupLeptons()
102	{
103	  TMatrixD empty3x3(3,3); 
104	  switch(lepParam_){ // setup lepton according to parameterization
105	  case kEMom       : lepton_  = new TFitParticleEScaledMomDev("Lepton",   "Lepton",   0, &empty3x3); break;
106	  case kEtEtaPhi   : lepton_  = new TFitParticleEtEtaPhi     ("Lepton",   "Lepton",   0, &empty3x3); break;
107	  case kEtThetaPhi : lepton_  = new TFitParticleEtThetaPhi   ("Lepton",   "Lepton",   0, &empty3x3); break;
108	  }
109	  switch(metParam_){ // setup neutrino according to parameterization
110	  case kEMom       : neutrino_= new TFitParticleEScaledMomDev("Neutrino", "Neutrino", 0, &empty3x3); break;
111	  case kEtEtaPhi   : neutrino_= new TFitParticleEtEtaPhi     ("Neutrino", "Neutrino", 0, &empty3x3); break;
112	  case kEtThetaPhi : neutrino_= new TFitParticleEtThetaPhi   ("Neutrino", "Neutrino", 0, &empty3x3); break;
113	  }
114	}
115	
116	void TtSemiLepKinFitter::setupConstraints() 
117	{
118	  massConstr_[kWHadMass      ] = new TFitConstraintM("WMassHad",      "WMassHad",      0, 0, mW_  );
119	  massConstr_[kWLepMass      ] = new TFitConstraintM("WMassLep",      "WMassLep",      0, 0, mW_  );
120	  massConstr_[kTopHadMass    ] = new TFitConstraintM("TopMassHad",    "TopMassHad",    0, 0, mTop_);
121	  massConstr_[kTopLepMass    ] = new TFitConstraintM("TopMassLep",    "TopMassLep",    0, 0, mTop_);
122	  massConstr_[kNeutrinoMass  ] = new TFitConstraintM("NeutrinoMass",  "NeutrinoMass",  0, 0,    0.);
123	  massConstr_[kEqualTopMasses] = new TFitConstraintM("EqualTopMasses","EqualTopMasses",0, 0,    0.);
124	
125	  massConstr_[kWHadMass      ]->addParticles1(hadP_,   hadQ_    );
126	  massConstr_[kWLepMass      ]->addParticles1(lepton_, neutrino_);
127	  massConstr_[kTopHadMass    ]->addParticles1(hadP_, hadQ_, hadB_);
128	  massConstr_[kTopLepMass    ]->addParticles1(lepton_, neutrino_, lepB_);
129	  massConstr_[kNeutrinoMass  ]->addParticle1 (neutrino_);
130	  massConstr_[kEqualTopMasses]->addParticles1(hadP_, hadQ_, hadB_);
131	  massConstr_[kEqualTopMasses]->addParticles2(lepton_, neutrino_, lepB_);
132	}
133	
134	void TtSemiLepKinFitter::setupFitter() 
135	{
136	  printSetup();
137	
138	  setupJets();
139	  setupLeptons();
140	  setupConstraints();
141	
142	  // add measured particles
143	  fitter_->addMeasParticle(hadB_);
144	  fitter_->addMeasParticle(hadP_);
145	  fitter_->addMeasParticle(hadQ_);
146	  fitter_->addMeasParticle(lepB_);
147	  fitter_->addMeasParticle(lepton_);
148	  fitter_->addMeasParticle(neutrino_);
149	  
150	  //std::cout << " constrList_.size() " << constrList_.size() <<std::endl;
151	  
152	  // add constraints
153	  for(unsigned int i=0; i<constrList_.size(); i++){
154	    fitter_->addConstraint(massConstr_[constrList_[i]]);
155	  }
156	}
157	
158	//AC template <class LeptonType>
159	//int TtSemiLepKinFitter::fit(const std::vector<pat::Jet>& jets, const pat::Lepton<LeptonType>& lepton, const pat::MET& neutrino, const double jetEnergyResolutionSmearFactor = 1.)
160	int TtSemiLepKinFitter::fit(const std::vector<NTJet>& jets, const NTLepton& lepton, std::string lepType, const NTMET& neutrino, const double jetEnergyResolutionSmearFactor = 1.)
161	{
162	  if( jets.size()<4 ) {
163	    std::cout <<"Cannot run the TtSemiLepKinFitter with less than 4 jets"<<std::endl;
164	    //??throw std::exception;
165	  }
166	
167	  // get jets in right order
168	  NTJet hadP = jets[TtSemiLepKinFitter::LightQ ];
169	  NTJet hadQ = jets[TtSemiLepKinFitter::LightQBar];
170	  NTJet hadB = jets[TtSemiLepKinFitter::HadB   ];
171	  NTJet lepB = jets[TtSemiLepKinFitter::LepB   ];
172	 
173	  // initialize particles
174	  TLorentzVector p4HadP( hadP.p4.Px(), hadP.p4.Py(), hadP.p4.Pz(), hadP.p4.E() );
175	  TLorentzVector p4HadQ( hadQ.p4.Px(), hadQ.p4.Py(), hadQ.p4.Pz(), hadQ.p4.E() );
176	  TLorentzVector p4HadB( hadB.p4.Px(), hadB.p4.Py(), hadB.p4.Pz(), hadB.p4.E() );
177	  TLorentzVector p4LepB( lepB.p4.Px(), lepB.p4.Py(), lepB.p4.Pz(), lepB.p4.E() );
178	  TLorentzVector p4Lepton  ( lepton.p4.Px(), lepton.p4.Py(), lepton.p4.Pz(), lepton.p4.E() );
179	  TLorentzVector p4Neutrino( neutrino.p4.Px(), neutrino.p4.Py(), 0, neutrino.p4.Et() );
180	
181	
182	  // initialize covariance matrices
183	  CovarianceMatrix covM;
184	  TMatrixD m1 = covM.setupMatrix(p4HadP, "jet", jetParam_);
185	  TMatrixD m2 = covM.setupMatrix(p4HadQ, "jet", jetParam_);
186	  TMatrixD m3 = covM.setupMatrix(p4HadB, "jet", jetParam_, "bjets");
187	  TMatrixD m4 = covM.setupMatrix(p4LepB, "jet", jetParam_, "bjets");
188	  TMatrixD m5 = covM.setupMatrix(p4Lepton, lepType,  lepParam_);
189	  TMatrixD m6 = covM.setupMatrix(p4Neutrino, "met",  metParam_);
190	
191	  // as covM contains resolution^2
192	  // the correction of jet energy resolutions
193	  // is just *jetEnergyResolutionSmearFactor^2
194	  m1(0,0)*=jetEnergyResolutionSmearFactor * jetEnergyResolutionSmearFactor; 
195	  m2(0,0)*=jetEnergyResolutionSmearFactor * jetEnergyResolutionSmearFactor; 
196	  m3(0,0)*=jetEnergyResolutionSmearFactor * jetEnergyResolutionSmearFactor; 
197	  m4(0,0)*=jetEnergyResolutionSmearFactor * jetEnergyResolutionSmearFactor; 
198	
199	  // set the kinematics of the objects to be fitted
200	  hadP_->setIni4Vec( &p4HadP );
201	  hadQ_->setIni4Vec( &p4HadQ );
202	  hadB_->setIni4Vec( &p4HadB );
203	  lepB_->setIni4Vec( &p4LepB );
204	  lepton_->setIni4Vec( &p4Lepton );
205	  neutrino_->setIni4Vec( &p4Neutrino );
206	
207	  hadP_->setCovMatrix( &m1 );
208	  hadQ_->setCovMatrix( &m2 );
209	  hadB_->setCovMatrix( &m3 );
210	  lepB_->setCovMatrix( &m4 );
211	  lepton_->setCovMatrix( &m5 );
212	  neutrino_->setCovMatrix( &m6 );
213	
214	  // now do the fit
215	  fitter_->fit();
216	
217	  // read back the resulting particles if the fit converged
218	  if(fitter_->getStatus()==0){
219	    // read back jet kinematics
220	    /*
221	    fittedHadP_= pat::Particle(reco::LeafCandidate(0, math::XYZTLorentzVector(hadP_->getCurr4Vec()->X(),
222	                               hadP_->getCurr4Vec()->Y(), hadP_->getCurr4Vec()->Z(), hadP_->getCurr4Vec()->E()), math::XYZPoint()));
223	    fittedHadQ_= pat::Particle(reco::LeafCandidate(0, math::XYZTLorentzVector(hadQ_->getCurr4Vec()->X(),
224	                               hadQ_->getCurr4Vec()->Y(), hadQ_->getCurr4Vec()->Z(), hadQ_->getCurr4Vec()->E()), math::XYZPoint()));
225	    fittedHadB_= pat::Particle(reco::LeafCandidate(0, math::XYZTLorentzVector(hadB_->getCurr4Vec()->X(),
226	                               hadB_->getCurr4Vec()->Y(), hadB_->getCurr4Vec()->Z(), hadB_->getCurr4Vec()->E()), math::XYZPoint()));
227	    fittedLepB_= pat::Particle(reco::LeafCandidate(0, math::XYZTLorentzVector(lepB_->getCurr4Vec()->X(),
228	                               lepB_->getCurr4Vec()->Y(), lepB_->getCurr4Vec()->Z(), lepB_->getCurr4Vec()->E()), math::XYZPoint()));
229	
230	    // read back lepton kinematics
231	    fittedLepton_= pat::Particle(reco::LeafCandidate(lepton.charge(), math::XYZTLorentzVector(lepton_->getCurr4Vec()->X(),
232	                                 lepton_->getCurr4Vec()->Y(), lepton_->getCurr4Vec()->Z(), lepton_->getCurr4Vec()->E()), math::XYZPoint()));
233	
234	    // read back the MET kinematics
235	    fittedNeutrino_= pat::Particle(reco::LeafCandidate(0, math::XYZTLorentzVector(neutrino_->getCurr4Vec()->X(),
236	                                   neutrino_->getCurr4Vec()->Y(), neutrino_->getCurr4Vec()->Z(), neutrino_->getCurr4Vec()->E()), math::XYZPoint()));
237	   */
238	    fittedHadP_= new TLorentzVector(hadP_->getCurr4Vec()->X(),hadP_->getCurr4Vec()->Y(), hadP_->getCurr4Vec()->Z(), hadP_->getCurr4Vec()->E());
239	    fittedHadQ_= new TLorentzVector(hadQ_->getCurr4Vec()->X(),hadQ_->getCurr4Vec()->Y(), hadQ_->getCurr4Vec()->Z(), hadQ_->getCurr4Vec()->E());         
240	    fittedHadB_= new TLorentzVector(hadB_->getCurr4Vec()->X(),hadB_->getCurr4Vec()->Y(), hadB_->getCurr4Vec()->Z(), hadB_->getCurr4Vec()->E());
241	    fittedLepB_= new TLorentzVector(lepB_->getCurr4Vec()->X(),lepB_->getCurr4Vec()->Y(), lepB_->getCurr4Vec()->Z(), lepB_->getCurr4Vec()->E());
242	
243	    // read back lepton kinematics
244	    fittedLepton_= new TLorentzVector(lepton_->getCurr4Vec()->X(),lepton_->getCurr4Vec()->Y(), lepton_->getCurr4Vec()->Z(), lepton_->getCurr4Vec()->E());
245	
246	    // read back the MET kinematics
247	    fittedNeutrino_= new TLorentzVector(neutrino_->getCurr4Vec()->X(),neutrino_->getCurr4Vec()->Y(), neutrino_->getCurr4Vec()->Z(), neutrino_->getCurr4Vec()->E());
248	  
249	  }
250	  return fitter_->getStatus();
251	}
252	
253	/*TtSemiEvtSolution TtSemiLepKinFitter::addKinFitInfo(TtSemiEvtSolution* asol, const double jetEnergyResolutionSmearFactor) 
254	{
255	
256	  TtSemiEvtSolution fitsol(*asol);
257	
258	  std::vector<pat::Jet> jets;
259	  jets.resize(4);
260	  jets[TtSemiLepEvtPartons::LightQ   ] = fitsol.getCalHadp();
261	  jets[TtSemiLepEvtPartons::LightQBar] = fitsol.getCalHadq();
262	  jets[TtSemiLepEvtPartons::HadB     ] = fitsol.getCalHadb();
263	  jets[TtSemiLepEvtPartons::LepB     ] = fitsol.getCalLepb();
264	
265	  // perform the fit, either using the electron or the muon
266	  if(fitsol.getDecay() == "electron") fit( jets, fitsol.getCalLepe(), fitsol.getCalLepn(), jetEnergyResolutionSmearFactor);
267	  if(fitsol.getDecay() == "muon"    ) fit( jets, fitsol.getCalLepm(), fitsol.getCalLepn(), jetEnergyResolutionSmearFactor);
268	  
269	  // add fitted information to the solution
270	  if (fitter_->getStatus() == 0) {
271	    // fill the fitted particles
272	    fitsol.setFitHadb( fittedHadB() );
273	    fitsol.setFitHadp( fittedHadP() );
274	    fitsol.setFitHadq( fittedHadQ() );
275	    fitsol.setFitLepb( fittedLepB() );
276	    fitsol.setFitLepl( fittedLepton() );
277	    fitsol.setFitLepn( fittedNeutrino() );
278	    // store the fit's chi2 probability
279	    fitsol.setProbChi2( fitProb() );
280	  }
281	  return fitsol;
282	}*/

