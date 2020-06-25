1	  ===================================
2	    How it works
3	  ===================================
4	   
5	   The class to be called in the analysis is TtSemiLepKinFitProducer,
6	   which calls TtSemiLepKinFitter, which inherits itself from TopKinFitter,
7	   which finally calls the FITTER TKinFitter. 
8	   
9	   Everything has been taken from cvs, PAT dependencies and other useless stuff
10	   have been removed, but the original code was modified as less as possible.
11	 
12	   Input of TtSemiLepKinFitProducer are objets of the local framework, output
13	   are TLorentzVectors... (to be improved ?).
14	   
15	   Particle resolutions are defined in Muon.h, Electron.h, Tau.h (preliminary), Jet.h, Met.h.
16	   
17	   Constraints to be used in the fit (on W-masses, top masses...) are :
18	   # 1: Whad-mass, 2: Wlep-mass, 3: thad-mass,
19	   # 4: tlep-mass, 5: nu-mass, 6: equal t-masses
20	   # 7: sum-pt conservation
21	   
22	   More infos here :
23	   https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideKinFitter
24	   
25	  ===================================
26	    How to use, example
27	  ===================================
28	  
29	  std::vector<int> constraints;
30	  constraints.push_back(1); //constraint on W(lep) mass
31	  constraints.push_back(2); //constraint on W(had) mass
32	  constraints.push_back(6); //equality of top masses
33	  
34	  TtSemiLepKinFitProducer kinFitter(constraints);
35	  // constraints are hard-coded in the default constructor !
36	  // use the default constructor with caution.
37	 
38	  std::vector<NTMET> mets;
39	  mets.push_back(met);
40	
41	  vector<NTLepton> lep;
42	  for (unsigned int i =0; i<tau_cand.size(); i++)
43	    lep.push_back(dynamic_cast<NTLepton&> (tau_cand.at(i)));
44	
45	  std::string lepType = "tau";
46	  kinFitter.produce(jet_cand, lep,"tau", mets);
47	  
48	  kinFitChi2 = kinFitter.GetFitChi2();
49	   
50	  TLorentzVector HadB = kinFitter.GetFitHadB();
51	  TLorentzVector HadP = kinFitter.GetFitHadP();
52	  TLorentzVector HadQ = kinFitter.GetFitHadQ();   
53	  TLorentzVector LepB = kinFitter.GetFitLepB();   
54	  TLorentzVector LepL = kinFitter.GetFitLepL();   
55	  TLorentzVector LepN = kinFitter.GetFitLepN();   
56	  
57	  TLorentzVector HadSide = HadB+HadP+HadQ;
58	  //std::cout << "M top had "<< HadSide.M() <<std::endl; 
59	  topMass = HadSide.M();
60	  TLorentzVector LepSide = LepB+LepL+LepN;
61	  //std::cout << "M top Lep "<< LepSide.M() <<std::endl; 
62	  TLorentzVector WLepSide = LepL+LepN;
63	  //std::cout << "M W lep "<< WLepSide.M() <<std::endl; 
64	  TLorentzVector WHadSide = HadP+HadQ;
65	  //std::cout << "M W  Had"<< WHadSide.M() <<std::endl; 
