#include "../interface/TtSemiLepKinFitProducer.h"

//template<typename LeptonCollection>
TtSemiLepKinFitProducer::TtSemiLepKinFitProducer():
  useOnlyMatch_(false),
  bTagAlgo_("trackCountingHighEffBJetTags"),    
  minBTagValueBJet_(1.0),      
  maxBTagValueNonBJet_(3.),   
  useBTag_(false),              
  maxNJets_(4),              
  maxNComb_(1),  //maximum number of jet combinations finally written into the event            
  maxNrIter_(500),             
  maxDeltaS_(5e-05),             
  maxF_(0.0001),             
  jetParam_(1),  // select parametrisation : 0: EMom, 1: EtEtaPhi, 2: EtThetaPhi      
  lepParam_(1),              
  metParam_(1),  
  mW_(80.4),               
  mTop_(173.),             
  jetEnergyResolutionSmearFactor_(1.0)
{
  constraints_.clear();
  constraints_.push_back(1);
  constraints_.push_back(3);
  //constraints_.push_back(3);
  constraints_.push_back(7);
  
  std::cout <<" " <<std::endl;
  std::cout <<"Caution, you are using a hard-coded list of constraints: W(lep) mass, W(had) mass, equal top masses " << std::endl;
  std::cout <<"Use the TtSemiLepKinFitProducer(std::vector<unsigned> constr) constructor to change the constraints !"<< std::endl;
  std::cout <<" " <<std::endl;
  
  fitter = new TtSemiLepKinFitter(param(jetParam_), param(lepParam_), param(metParam_), maxNrIter_, maxDeltaS_, maxF_,
                                  constraints(constraints_), mW_, mTop_);
                                  
  
  FitResultList.clear();
  
}

//template<typename LeptonCollection>
TtSemiLepKinFitProducer::TtSemiLepKinFitProducer(std::vector<unsigned> constr):
  useOnlyMatch_(false),
  bTagAlgo_("trackCountingHighEffBJetTags"),    
  minBTagValueBJet_(1.0),      
  maxBTagValueNonBJet_(3.),   
  useBTag_(false),              
  maxNJets_(4),              
  maxNComb_(1),  //maximum number of jet combinations finally written into the event            
  maxNrIter_(500),             
  maxDeltaS_(5e-05),             
  maxF_(0.0001),             
  jetParam_(1),  // select parametrisation : 0: EMom, 1: EtEtaPhi, 2: EtThetaPhi      
  lepParam_(1),              
  metParam_(1),  
  mW_(80.4),               
  mTop_(173.),             
  jetEnergyResolutionSmearFactor_(1.0)
 
/* AC
  match_                   (cfg.getParameter<edm::InputTag>("match")), //??
  useOnlyMatch_            (cfg.getParameter<bool>         ("useOnlyMatch"        )),
  bTagAlgo_                (cfg.getParameter<std::string>  ("bTagAlgo"            )),
  minBTagValueBJet_        (cfg.getParameter<double>       ("minBDiscBJets"       )),
  maxBTagValueNonBJet_     (cfg.getParameter<double>       ("maxBDiscLightJets"   )),
  useBTag_                 (cfg.getParameter<bool>         ("useBTagging"         )),
  maxNJets_                (cfg.getParameter<int>          ("maxNJets"            )),
  maxNComb_                (cfg.getParameter<int>          ("maxNComb"            )),
  maxNrIter_               (cfg.getParameter<unsigned>     ("maxNrIter"           )),
  maxDeltaS_               (cfg.getParameter<double>       ("maxDeltaS"           )),
  maxF_                    (cfg.getParameter<double>       ("maxF"                )),
  jetParam_                (cfg.getParameter<unsigned>     ("jetParametrisation"  )),
  lepParam_                (cfg.getParameter<unsigned>     ("lepParametrisation"  )),
  metParam_                (cfg.getParameter<unsigned>     ("metParametrisation"  )),
  constraints_             (cfg.getParameter<std::vector<unsigned> >("constraints")),
  mW_                      (cfg.getParameter<double>       ("mW"                  )),
  mTop_                    (cfg.getParameter<double>       ("mTop"                )),
  jetEnergyResolutionSmearFactor_(cfg.getParameter<double> ("jetEnergyResolutionSmearFactor"))
  */

{
  
  
  constraints_ = constr;
  
  fitter = new TtSemiLepKinFitter(param(jetParam_), param(lepParam_), param(metParam_), maxNrIter_, maxDeltaS_, maxF_,
                                  constraints(constraints_), mW_, mTop_);
                                  

  FitResultList.clear();

  /* AC
  produces< std::vector<TLorentzVector> >("PartonsHadP");
  produces< std::vector<TLorentzVector> >("PartonsHadQ");
  produces< std::vector<TLorentzVector> >("PartonsHadB");
  produces< std::vector<TLorentzVector> >("PartonsLepB");
  produces< std::vector<TLorentzVector> >("Leptons");
  produces< std::vector<TLorentzVector> >("Neutrinos");

  produces< std::vector<std::vector<int> > >();
  produces< std::vector<double> >("Chi2");
  produces< std::vector<double> >("Prob");
  produces< std::vector<int> >("Status");*/
  
}


void TtSemiLepKinFitProducer::SetupFitParameters(bool useOnlyMatch,
  std::string bTagAlgo, double minBTagValueBJet, double maxBTagValueNonBJet, bool useBTag,              
  int maxNJets, int maxNComb, unsigned int  maxNrIter, double maxDeltaS, double maxF,             
  unsigned int jetParam, unsigned int lepParam, unsigned int  metParam,              
  std::vector<unsigned> constraints, double mW, double mTop, double jetEnergyResolutionSmearFactor)
{
  useOnlyMatch_        =  useOnlyMatch;
  bTagAlgo_            =  bTagAlgo;   
  minBTagValueBJet_    =  minBTagValueBJet;     
  maxBTagValueNonBJet_ =  maxBTagValueNonBJet;  
  useBTag_             =  useBTag;       
  maxNJets_            =  maxNJets;             
  maxNComb_            =  maxNComb;             
  maxNrIter_           =  maxNrIter;            
  maxDeltaS_           =  maxDeltaS;            
  maxF_                =  maxF;            
  jetParam_            =  jetParam;            
  lepParam_            =  lepParam;             
  metParam_            =  metParam;             
  constraints_         =  constraints;                   
  mW_                  =  mW;             
  mTop_                =  mTop;           
 jetEnergyResolutionSmearFactor_  = jetEnergyResolutionSmearFactor;

}

//template<typename LeptonCollection>
TtSemiLepKinFitProducer::~TtSemiLepKinFitProducer()
{
  delete fitter;
}

//template<typename LeptonCollection>
bool TtSemiLepKinFitProducer::doBTagging(bool& useBTag_, const vector<NTJet>& jets, std::vector<int>& combi,
                                                           std::string& bTagAlgo_, double& minBTagValueBJet_, double& maxBTagValueNonBJet_){
  
  if ( useBTag_ && bTagAlgo_ != "trackCountingHighEffBJetTags") std::cout <<"btagAlgo "<< bTagAlgo_ << " not yet supported " <<std::endl;
  
  if( !useBTag_ ) {
    return true;
  }
  if( useBTag_ && bTagAlgo_ == "trackCountingHighEffBJetTags" && 
      jets[combi[TtSemiLepKinFitter::HadB     ]].TCDiscri >= minBTagValueBJet_ &&
      jets[combi[TtSemiLepKinFitter::LepB     ]].TCDiscri >= minBTagValueBJet_ &&
      jets[combi[TtSemiLepKinFitter::LightQ   ]].TCDiscri <  maxBTagValueNonBJet_ &&
      jets[combi[TtSemiLepKinFitter::LightQBar]].TCDiscri <  maxBTagValueNonBJet_ ) {
    return true;
  }
  else{
    return false;
  }
}

//template<typename LeptonCollection>
void TtSemiLepKinFitProducer::produce(const vector<NTJet>& jets, const vector<NTLepton>& leps, std::string
lepType, const vector<NTMET>& mets)
{
  /*
  std::auto_ptr< std::vector<TLorentzVector> > pPartonsHadP( new std::vector<TLorentzVector> );
  std::auto_ptr< std::vector<TLorentzVector> > pPartonsHadQ( new std::vector<TLorentzVector> );
  std::auto_ptr< std::vector<TLorentzVector> > pPartonsHadB( new std::vector<TLorentzVector> );
  std::auto_ptr< std::vector<TLorentzVector> > pPartonsLepB( new std::vector<TLorentzVector> );
  std::auto_ptr< std::vector<TLorentzVector> > pLeptons    ( new std::vector<TLorentzVector> );
  std::auto_ptr< std::vector<TLorentzVector> > pNeutrinos  ( new std::vector<TLorentzVector> );

  std::auto_ptr< std::vector<std::vector<int> > > pCombi ( new std::vector<std::vector<int> > );
  std::auto_ptr< std::vector<double>            > pChi2  ( new std::vector<double> );
  std::auto_ptr< std::vector<double>            > pProb  ( new std::vector<double> );
  std::auto_ptr< std::vector<int>               > pStatus( new std::vector<int> );

  
  edm::Handle<std::vector<pat::Jet> > jets;
  evt.getByLabel(jets_, jets);

  edm::Handle<std::vector<pat::MET> > mets;
  evt.getByLabel(mets_, mets);

  edm::Handle<LeptonCollection> leps;
  evt.getByLabel(leps_, leps);*/

  unsigned int nPartons = 4;
  
  bool invalidMatch = false;
  if(useOnlyMatch_) { 
    std::cout <<"Original code has not been translated, useOnlyMatch_ not supported !" << std::endl;
    }
  /* 
  std::vector<int> match;
  bool invalidMatch = false;
  if(useOnlyMatch_) {
    edm::Handle<std::vector<std::vector<int> > > matchHandle;
    evt.getByLabel(match_, matchHandle);
    match = *(matchHandle->begin());
    // check if match is valid
    if(match.size()!=nPartons) invalidMatch=true;
    else {
      for(unsigned int idx=0; idx<match.size(); ++idx) {
        if(match[idx]<0 || match[idx]>=(int)jets->size()) {
          invalidMatch=true;
          break;
        }
      }
    }
  }*/

  // -----------------------------------------------------
  // skip events with no appropriate lepton candidate in
  // or empty MET or less jets than partons or invalid match
  // -----------------------------------------------------

  if( leps.empty() || mets.empty() || jets.size()<nPartons || invalidMatch ) {
    std::cout <<"skipping events with no appropriate lepton candidate in or empty MET or less jets than partons or invalid match " <<  std::endl;
      
    /*
    // the kinFit getters return empty objects here
    pPartonsHadP->push_back( fitter->fittedHadP()     );
    pPartonsHadQ->push_back( fitter->fittedHadQ()     );
    pPartonsHadB->push_back( fitter->fittedHadB()     );
    pPartonsLepB->push_back( fitter->fittedLepB()     );
    pLeptons    ->push_back( fitter->fittedLepton()   );
    pNeutrinos  ->push_back( fitter->fittedNeutrino() );
    // indices referring to the jet combination
    std::vector<int> invalidCombi;
    for(unsigned int i = 0; i < nPartons; ++i) 
      invalidCombi.push_back( -1 );
    pCombi->push_back( invalidCombi );
    // chi2
    pChi2->push_back( -1. );
    // chi2 probability
    pProb->push_back( -1. );
    // status of the fitter
    pStatus->push_back( -1 );
    // feed out all products
    evt.put(pCombi);
    evt.put(pPartonsHadP, "PartonsHadP");
    evt.put(pPartonsHadQ, "PartonsHadQ");
    evt.put(pPartonsHadB, "PartonsHadB");
    evt.put(pPartonsLepB, "PartonsLepB");
    evt.put(pLeptons    , "Leptons"    );
    evt.put(pNeutrinos  , "Neutrinos"  );
    evt.put(pChi2       , "Chi2"       );
    evt.put(pProb       , "Prob"       );
    evt.put(pStatus     , "Status"     );*/
    return;
  }

  // -----------------------------------------------------
  // analyze different jet combinations using the KinFitter
  // (or only a given jet combination if useOnlyMatch=true)
  // -----------------------------------------------------
    
  std::vector<int> jetIndices;
  if(!useOnlyMatch_) {
    for(unsigned int i=0; i<jets.size(); ++i){
      if(maxNJets_ >= (int) nPartons && maxNJets_ == (int) i) break;
      jetIndices.push_back(i);
    }
  }
  
  std::vector<int> combi;
  for(unsigned int i=0; i<nPartons; ++i) {
    //if(useOnlyMatch_) combi.push_back( match[i] );
    //else 
    combi.push_back(i);
  }
  
  FitResultList.clear();

  do{

    for(int cnt = 0; cnt < TMath::Factorial( combi.size() ); ++cnt){
      // take into account indistinguishability of the two jets from the hadr. W decay,
      // reduces combinatorics by a factor of 2
      if( (combi[TtSemiLepKinFitter::LightQ] < combi[TtSemiLepKinFitter::LightQBar]
         || useOnlyMatch_ ) && doBTagging(useBTag_, jets, combi, bTagAlgo_, minBTagValueBJet_, maxBTagValueNonBJet_) ){
        
        std::vector<NTJet> jetCombi;
        jetCombi.resize(nPartons);
        jetCombi[TtSemiLepKinFitter::LightQ   ] = jets[combi[TtSemiLepKinFitter::LightQ   ]];
        jetCombi[TtSemiLepKinFitter::LightQBar] = jets[combi[TtSemiLepKinFitter::LightQBar]];
        jetCombi[TtSemiLepKinFitter::HadB     ] = jets[combi[TtSemiLepKinFitter::HadB     ]];
        jetCombi[TtSemiLepKinFitter::LepB     ] = jets[combi[TtSemiLepKinFitter::LepB     ]];

        // do the kinematic fit
        int status = fitter->fit(jetCombi, leps[0], lepType, mets[0], jetEnergyResolutionSmearFactor_);

        if( status == 0 ) { // only take into account converged fits
        
        
          KinFitResult result;
          result.Status = status;
          result.Chi2 = fitter->fitS();
          result.Prob = fitter->fitProb();
          result.HadB = fitter->fittedHadB();
          result.HadP = fitter->fittedHadP();
          result.HadQ = fitter->fittedHadQ();
          result.LepB = fitter->fittedLepB();
          result.LepL = fitter->fittedLepton();
          result.LepN = fitter->fittedNeutrino();
          result.JetCombi = combi;

          FitResultList.push_back(result);
        }

      }
      if(useOnlyMatch_) break; // don't go through combinatorics if useOnlyMatch was chosen
      next_permutation( combi.begin(), combi.end() );
    }
    if(useOnlyMatch_) break; // don't go through combinatorics if useOnlyMatch was chosen
  }
  while(stdcomb::next_combination( jetIndices.begin(), jetIndices.end(), combi.begin(), combi.end() ));
 
  // sort results w.r.t. chi2 values
  FitResultList.sort();
  
  /* AC : no need to feed out result 
  // -----------------------------------------------------
  // feed out result
  // starting with the JetComb having the smallest chi2
  // -----------------------------------------------------

  if( FitResultList.size() < 1 ) { // in case no fit results were stored in the list (all fits aborted)
    pPartonsHadP->push_back( fitter->fittedHadP()     );
    pPartonsHadQ->push_back( fitter->fittedHadQ()     );
    pPartonsHadB->push_back( fitter->fittedHadB()     );
    pPartonsLepB->push_back( fitter->fittedLepB()     );
    pLeptons    ->push_back( fitter->fittedLepton()   );
    pNeutrinos  ->push_back( fitter->fittedNeutrino() );
    // indices referring to the jet combination
    std::vector<int> invalidCombi;
    for(unsigned int i = 0; i < nPartons; ++i) 
      invalidCombi.push_back( -1 );
    pCombi->push_back( invalidCombi );
    // chi2
    pChi2->push_back( -1. );
    // chi2 probability
    pProb->push_back( -1. );
    // status of the fitter
    pStatus->push_back( -1 );
  }
  else {
    unsigned int iComb = 0;
    for(typename std::list<KinFitResult>::const_iterator result = FitResultList.begin(); result != FitResultList.end(); ++result) {
      if(maxNComb_ >= 1 && iComb == (unsigned int) maxNComb_) break;
      iComb++;
      // partons
      pPartonsHadP->push_back( result->HadP );
      pPartonsHadQ->push_back( result->HadQ );
      pPartonsHadB->push_back( result->HadB );
      pPartonsLepB->push_back( result->LepB );
      // lepton
      pLeptons->push_back( result->LepL );
      // neutrino
      pNeutrinos->push_back( result->LepN );
      // indices referring to the jet combination
      pCombi->push_back( result->JetCombi );
      // chi2
      pChi2->push_back( result->Chi2 );
      // chi2 probability
      pProb->push_back( result->Prob );
      // status of the fitter
      pStatus->push_back( result->Status );
    }
  }
  evt.put(pCombi);
  evt.put(pPartonsHadP, "PartonsHadP");
  evt.put(pPartonsHadQ, "PartonsHadQ");
  evt.put(pPartonsHadB, "PartonsHadB");
  evt.put(pPartonsLepB, "PartonsLepB");
  evt.put(pLeptons    , "Leptons"    );
  evt.put(pNeutrinos  , "Neutrinos"  );
  evt.put(pChi2       , "Chi2"       );
  evt.put(pProb       , "Prob"       );
  evt.put(pStatus     , "Status"     );*/
}
 
//template<typename LeptonCollection>
TtSemiLepKinFitter::Param TtSemiLepKinFitProducer::param(unsigned val) 
{
  TtSemiLepKinFitter::Param result;
  switch(val){
  case TtSemiLepKinFitter::kEMom       : result=TtSemiLepKinFitter::kEMom;       break;
  case TtSemiLepKinFitter::kEtEtaPhi   : result=TtSemiLepKinFitter::kEtEtaPhi;   break;
  case TtSemiLepKinFitter::kEtThetaPhi : result=TtSemiLepKinFitter::kEtThetaPhi; break;
  default: 
    //AC throw cms::Exception("WrongConfig") 
      std::cout << "Chosen jet parametrization is not supported: " << val << std::endl;
    break;
  }
  return result;
} 

//template<typename LeptonCollection>
TtSemiLepKinFitter::Constraint TtSemiLepKinFitProducer::constraint(unsigned val) 
{
  TtSemiLepKinFitter::Constraint result;
  switch(val){
  case TtSemiLepKinFitter::kWHadMass       : result=TtSemiLepKinFitter::kWHadMass;       break;
  case TtSemiLepKinFitter::kWLepMass       : result=TtSemiLepKinFitter::kWLepMass;       break;
  case TtSemiLepKinFitter::kTopHadMass     : result=TtSemiLepKinFitter::kTopHadMass;     break;
  case TtSemiLepKinFitter::kTopLepMass     : result=TtSemiLepKinFitter::kTopLepMass;     break;
  case TtSemiLepKinFitter::kNeutrinoMass   : result=TtSemiLepKinFitter::kNeutrinoMass;   break;
  case TtSemiLepKinFitter::kEqualTopMasses : result=TtSemiLepKinFitter::kEqualTopMasses; break;
  default: 
    //AC throw cms::Exception("WrongConfig") 
      std::cout << "Chosen fit constraint is not supported: " << val << std::endl;
    break;
  }
  return result;
} 

//template<typename LeptonCollection>
std::vector<TtSemiLepKinFitter::Constraint> TtSemiLepKinFitProducer::constraints(std::vector<unsigned>& val)
{
  std::vector<TtSemiLepKinFitter::Constraint> result;
  for(unsigned i=0; i<val.size(); ++i){
    result.push_back(constraint(val[i]));
  }
  return result; 
}

double TtSemiLepKinFitProducer::GetFitChi2(int i)
{
  if (FitResultList.size()>=i+1) 
  {  std::list<KinFitResult>::iterator it = FitResultList.begin();
     std::advance(it, i);
     return it->Chi2;}
     
  else 
  { std::cout <<"Number of converged fits is "<<  FitResultList.size() << std::endl;
    return -999;}

}
