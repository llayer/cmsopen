1	// -*- C++ -*-
2	//
3	// Package:    NeutralPFCandidates
4	// Class:      NeutralPFCandidates
5	// 
6	/**\class NeutralPFCandidates NeutralPFCandidates.cc TopQuarkAnalysis/NeutralPFCandidates/src/NeutralPFCandidates.cc
7	
8	 Description: [one line class summary]
9	
10	 Implementation:
11	     [Notes on implementation]
12	*/
13	//
14	// Original Author:  Andrea Jeremy,B25/117,6262,
15	//         Created:  Mon May  2 10:05:12 CEST 2011
16	// $Id: NeutralPFCandidates.h,v 1.2 2011/05/03 11:58:24 alebihan Exp $
17	//
18	//
19	
20	
21	// system include files
22	#include <memory>
23	
24	// user include files
25	#include "FWCore/Framework/interface/Frameworkfwd.h"
26	#include "FWCore/Framework/interface/EDProducer.h"
27	
28	#include "FWCore/Framework/interface/Event.h"
29	#include "FWCore/Framework/interface/MakerMacros.h"
30	
31	#include "FWCore/ParameterSet/interface/ParameterSet.h"
32	
33	
34	#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
35	#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
36	
37	
38	
39	//
40	// class declaration
41	//
42	
43	class NeutralPFCandidates : public edm::EDProducer {
44	   public:
45	      explicit NeutralPFCandidates(const edm::ParameterSet&);
46	      ~NeutralPFCandidates();
47	
48	      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
49	
50	   private:
51	      virtual void beginJob() ;
52	      virtual void produce(edm::Event&, const edm::EventSetup&);
53	      virtual void endJob() ;
54	      
55	      virtual void beginRun(edm::Run&, edm::EventSetup const&);
56	      virtual void endRun(edm::Run&, edm::EventSetup const&);
57	      virtual void beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
58	      virtual void endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
59	
60	      // ----------member data ---------------------------
61	      
62	      edm::InputTag      inputPFcandidates_;
63	      
64	      
65	      
66	};
