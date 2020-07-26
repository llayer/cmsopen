  ===================================
    How it works
  ===================================
   
   The class to be called in the analysis is TtSemiLepKinFitProducer,
   which calls TtSemiLepKinFitter, which inherits itself from TopKinFitter,
   which finally calls the FITTER TKinFitter. 
   
   Everything has been taken from cvs, PAT dependencies and other useless stuff
   have been removed, but the original code was modified as less as possible.
 
   Input of TtSemiLepKinFitProducer are objets of the local framework, output
   are TLorentzVectors... (to be improved ?).
   
   Particle resolutions are defined in Muon.h, Electron.h, Tau.h (preliminary), Jet.h, Met.h.
   
   Constraints to be used in the fit (on W-masses, top masses...) are :
   # 1: Whad-mass, 2: Wlep-mass, 3: thad-mass,
   # 4: tlep-mass, 5: nu-mass, 6: equal t-masses
   # 7: sum-pt conservation
   
   More infos here :
   https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideKinFitter
   
  ===================================
    How to use, example
  ===================================
  
  std::vector<int> constraints;
  constraints.push_back(1); //constraint on W(lep) mass
  constraints.push_back(2); //constraint on W(had) mass
  constraints.push_back(6); //equality of top masses
  
  TtSemiLepKinFitProducer kinFitter(constraints);
  // constraints are hard-coded in the default constructor !
  // use the default constructor with caution.
 
  std::vector<NTMET> mets;
  mets.push_back(met);

  vector<NTLepton> lep;
  for (unsigned int i =0; i<tau_cand.size(); i++)
    lep.push_back(dynamic_cast<NTLepton&> (tau_cand.at(i)));

  std::string lepType = "tau";
  kinFitter.produce(jet_cand, lep,"tau", mets);
  
  kinFitChi2 = kinFitter.GetFitChi2();
   
  TLorentzVector HadB = kinFitter.GetFitHadB();
  TLorentzVector HadP = kinFitter.GetFitHadP();
  TLorentzVector HadQ = kinFitter.GetFitHadQ();   
  TLorentzVector LepB = kinFitter.GetFitLepB();   
  TLorentzVector LepL = kinFitter.GetFitLepL();   
  TLorentzVector LepN = kinFitter.GetFitLepN();   
  
  TLorentzVector HadSide = HadB+HadP+HadQ;
  //std::cout << "M top had "<< HadSide.M() <<std::endl; 
  topMass = HadSide.M();
  TLorentzVector LepSide = LepB+LepL+LepN;
  //std::cout << "M top Lep "<< LepSide.M() <<std::endl; 
  TLorentzVector WLepSide = LepL+LepN;
  //std::cout << "M W lep "<< WLepSide.M() <<std::endl; 
  TLorentzVector WHadSide = HadP+HadQ;
  //std::cout << "M W  Had"<< WHadSide.M() <<std::endl; 
