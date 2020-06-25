1
2
3
4	#include "TopQuarkAnalysis/MiniTreeProducer/interface/NeutralPFCandidates.h"
5
6
7
8
9	NeutralPFCandidates::NeutralPFCandidates(const edm::ParameterSet& iConfig)
10	{
11	   //register your products
12
13	   produces<reco::PFCandidateCollection>();
14
15
16
17	   inputPFcandidates_     = iConfig.getParameter < edm::InputTag >         ("inputPFcandidates");
18
19	   //now do what ever other initialization is needed
20
21	}
22
23
24	NeutralPFCandidates::~NeutralPFCandidates()
25	{
26
27	   // do anything here that needs to be done at desctruction time
28	   // (e.g. close files, deallocate resources etc.)
29
30	}
31
32
33	//
34	// member functions
35	//
36
37	// ------------ method called to produce the data  ------------
38	void
39	NeutralPFCandidates::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
40	{
41	   using namespace edm;
42
43	   reco::PFCandidateCollection neutralPfCandidates;
44
45
46	   Handle<reco::PFCandidateCollection> pIn;
47	   iEvent.getByLabel(inputPFcandidates_,pIn);
48
49
50
51	   for (reco::PFCandidateCollection::const_iterator itpf = pIn->begin(), itpfend = pIn->end(); itpf!= itpfend; ++itpf) {
52
53	     if( (*itpf).charge() == 0 ) neutralPfCandidates.push_back(*itpf);
54
55	   }
56
57	   std::auto_ptr<reco::PFCandidateCollection> pOut(new reco::PFCandidateCollection(neutralPfCandidates));
58	   iEvent.put(pOut);
59
60	/* This is an event example
61	   //Read 'ExampleData' from the Event
62	   Handle<ExampleData> pIn;
63	   iEvent.getByLabel("example",pIn);
64
65	   //Use the ExampleData to create an ExampleData2 which
66	   // is put into the Event
67	   std::auto_ptr<ExampleData2> pOut(new ExampleData2(*pIn));
68	   iEvent.put(pOut);
69	*/
70
71
72
73	}
74
75	// ------------ method called once each job just before starting event loop  ------------
76	void
77	NeutralPFCandidates::beginJob()
78	{
79	}
80
81	// ------------ method called once each job just after ending the event loop  ------------
82	void
83	NeutralPFCandidates::endJob() {
84	}
85
86	// ------------ method called when starting to processes a run  ------------
87	void
88	NeutralPFCandidates::beginRun(edm::Run&, edm::EventSetup const&)
89	{
90	}
91
92	// ------------ method called when ending the processing of a run  ------------
93	void
94	NeutralPFCandidates::endRun(edm::Run&, edm::EventSetup const&)
95	{
96	}
97
98	// ------------ method called when starting to processes a luminosity block  ------------
99	void
100	NeutralPFCandidates::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
101	{
102	}
103
104	// ------------ method called when ending the processing of a luminosity block  ------------
105	void
106	NeutralPFCandidates::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
107	{
108	}
109
110	// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
111	void
112	NeutralPFCandidates::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
113	  //The following says we do not know what parameters are allowed so do no validation
114	  // Please change this to state exactly what you do use, even if it is no parameters
115	  edm::ParameterSetDescription desc;
116	  desc.setUnknown();
117	  descriptions.addDefault(desc);
118	}
119
120	//define this as a plug-in
121	DEFINE_FWK_MODULE(NeutralPFCandidates);
