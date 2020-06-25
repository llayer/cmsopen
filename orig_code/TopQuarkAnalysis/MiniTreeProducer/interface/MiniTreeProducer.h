1	#ifndef __MiniTreeProducer_H__
2	#define __MiniTreeProducer_H__
3	#include "DataFormats/BeamSpot/interface/BeamSpot.h"
4	
5	// -*- C++ -*-
6	//
7	// Package:    MiniTreeProducer
8	// Class:      MiniTreeProducer
9	// 
10	/**\class MiniTreeProducer MiniTreeProducer.cc TMP/MiniTreeProducer/src/MiniTreeProducer.cc
11	
12	 Description: <one line class summary>
13	
14	 Implementation:
15	     <Notes on implementation>
16	*/
17	//
18	// Original Author:  Jeremy ANDREA
19	//         Created:  Sun Nov  1 21:30:20 CET 2009
20	// $Id: MiniTreeProducer.h,v 1.14 2011/12/09 14:10:57 jandrea Exp $
21	//
22	//
23	
24	//----------------- system include files
25	#include <memory>
26	#include <iostream>
27	#include <list>
28	#include <string>
29	#include <sstream>
30	#include <map>
31	#include <vector>
32	#include <algorithm>
33	
34	//----------------- cmssw includes
35	
36	#include "FWCore/Framework/interface/Frameworkfwd.h"
37	
38	#include "FWCore/Framework/interface/Event.h"
39	#include <FWCore/Framework/interface/Run.h>
40	
41	#include "FWCore/Framework/interface/EDProducer.h"
42	
43	#include "FWCore/Framework/interface/MakerMacros.h"
44	#include "FWCore/ParameterSet/interface/ParameterSet.h"
45	
46	#include "DataFormats/Common/interface/TriggerResults.h"
47	#include "FWCore/Framework/interface/TriggerNamesService.h"
48	#include "FWCore/ServiceRegistry/interface/Service.h" 
49	
50	#include "DataFormats/TrackReco/interface/Track.h"
51	#include "DataFormats/VertexReco/interface/Vertex.h"
52	#include "JetMETCorrections/JetVertexAssociation/interface/JetVertexMain.h"
53	
54	#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
55	#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
56	#include "TrackingTools/Records/interface/TransientTrackRecord.h"
57	#include "TrackingTools/IPTools/interface/IPTools.h"
58	
59	
60	#include "RecoLocalCalo/EcalRecAlgos/interface/EcalSeverityLevelAlgo.h"
61	
62	
63	
64	//--------------------PAT includes
65	#include "DataFormats/PatCandidates/interface/Particle.h"
66	#include "DataFormats/PatCandidates/interface/Muon.h"
67	#include "DataFormats/PatCandidates/interface/Tau.h"
68	#include "DataFormats/PatCandidates/interface/Electron.h"
69	#include "DataFormats/PatCandidates/interface/Photon.h"
70	#include "DataFormats/PatCandidates/interface/MET.h"
71	#include "DataFormats/PatCandidates/interface/Jet.h"
72	#include "DataFormats/PatCandidates/interface/TriggerEvent.h"
73	
74	#include "PhysicsTools/SelectorUtils/interface/JetIDSelectionFunctor.h"
75	#include "PhysicsTools/SelectorUtils/interface/PFJetIDSelectionFunctor.h"
76	#include "DataFormats/HLTReco/interface/TriggerEvent.h"
77	#include "DataFormats/HepMCCandidate/interface/PdfInfo.h"
78	
79	
80	//--------------------ROOT includes
81	#include "TFile.h"
82	#include "TTree.h"
83	#include "TChain.h"
84	#include "TLorentzVector.h"
85	#include "TH1.h"
86	#include "TRandom.h"
87	
88	//--------------------CLASS includes
89	#include "MiniTreeFormat/MTFormat/interface/MTEvent.h"
90	#include "MiniTreeFormat/NTFormat/interface/NTSampleInfo.h"
91	
92	
93	using namespace std;
94	using namespace edm;
95	using namespace reco;
96	
97	//MiniTree classes
98	typedef TopTree::MTEvent        TopTreeEvent;
99	typedef TopTree::MTJet          TopTreeJet;
100	typedef TopTree::MTMET          TopTreeMET;
101	typedef TopTree::MTJetMet       TopTreeJetMet;
102	typedef TopTree::MTElectron     TopTreeElectron;
103	typedef TopTree::MTPhoton       TopTreePhoton;
104	typedef TopTree::MTMuon         TopTreeMuon;
105	typedef TopTree::MTTau          TopTreeTau;
106	typedef TopTree::MTTrack        TopTreeTrack;
107	typedef TopTree::MTVertex       TopTreePVx;
108	typedef TopTree::MTVertex       TopTreeBS;
109	
110	typedef TopTree::NTSampleInfo   TopTreeSampleInfo;
111	
112	
113	
114	// LHAPDF stuff
115	extern "C" {
116	  void initpdfset_ (char *, int len);
117	  void initpdfsetm_(int &, char *);
118	  void initpdf_(int &);
119	  void evolvepdf_(double &, double &, double *);
120	  void numberpdf_(int &);
121	}
122	  
123	
124	class MiniTreeProducer : public edm::EDProducer {
125	 public:
126	  explicit MiniTreeProducer(const edm::ParameterSet&);
127	  ~MiniTreeProducer();
128	  
129	  
130	 private:
131	  virtual void  beginJob() ;
132	  virtual void  produce(edm::Event&, const edm::EventSetup&);
133	  virtual void  endJob() ;
134	  bool          MyJetCleaner(const vector<pat::Electron>*, pat::Jet);
135	  void          EventInfo();  
136	  int           getMuonOrigin(edm::Handle<reco::GenParticleCollection> , const pat::Muon * , reco::GenParticle&, reco::GenParticle&, reco::GenParticle&);
137	  int           getElectronOrigin(edm::Handle<reco::GenParticleCollection> particles, const pat::Electron * thePatElec,reco::GenParticle&, reco::GenParticle&, reco::GenParticle&);
138	  int           getTauDecay(edm::Handle<reco::GenParticleCollection> particles, const pat::Tau * thePatTau);
139	  void          setGenParticleFromCandidate(reco::GenParticle&, const reco::Candidate *);
140	  
141	  // ----------member data ---------------------------
142	 
143	  int           verbose;
144	 
145	  InputTag      TriggerMenu;
146	  bool          TriggerInfo;
147	  bool          saveAllTriggers;
148	  std::vector<std::string> triggerList;
149	  std::vector<std::string> prescaleList;
150	  std::string   hltJetsMatcher;
151	  std::string   hltTausMatcher;
152	  std::string   hltJetsMatcher45;
153	  std::string   hltTausMatcher45;
154	
155	  InputTag      electronProducer;
156	  bool          doElectrons;
157	  double        electron_cut_pt;
158	  double        electron_cut_eta;
159	  
160	  InputTag      photonProducer;
161	  bool          doPhotons;
162	  double        photon_cut_pt;
163	  double        photon_cut_eta;
164	  
165	  InputTag      muonProducer;
166	  bool          doMuons;
167	  double        muon_cut_pt;
168	  double        muon_cut_eta;
169	  
170	  std::vector<std::string> tauProducer;
171	  bool          doTaus;
172	  double        tau_cut_pt;
173	  double        tau_cut_eta;
174	  
175	  InputTag      trackProducer; 
176	  bool          doTracks;
177	  double        track_cut_pt;
178	  double        track_cut_eta;
179	
180	  InputTag      beamSpotProducer; 
181	  InputTag      vertexProducer;
182	  bool          saveAllVertex;
183	  InputTag      rho_PUUE_dens;
184	  InputTag      neutralRho_PUUE_dens;
185	  typedef std::vector<edm::ParameterSet> VParameters;
186	  VParameters jetmetProducer;
187	  double        jet_cut_pt;
188	  double        jet_cut_eta;
189	  
190	  JetVertexMain* vertexAlgo;
191	  string        cftbjetstring;
192	 
193	  bool          doGenParticleCollection;
194	 
195	  
196	  //Pointers to TopTree objects
197	  TopTreeSampleInfo* info;
198	 
199	  JetIDSelectionFunctor   *jetSel_;
200	  PFJetIDSelectionFunctor *pfJetSel_;
201	  bool          isAOD; 
202	  bool          isData; 
203	  
204	  bool          readPDFWeights; 
205	  
206	  
207	  
208	  double xfx(const double &x, const double &Q, int fl) {
209	    double f[13], mx = x, mQ = Q;
210	    evolvepdf_(mx, mQ, f);
211	    return f[fl+6];
212	  }
213	
214	  
215	  
216	  
217	  
218	  
219	  
220	  
221	  
222	  
223	  
224	
225	};
226	
227	#endif

