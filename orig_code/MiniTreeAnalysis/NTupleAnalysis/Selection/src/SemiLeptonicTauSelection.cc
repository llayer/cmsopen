#include "../interface/SemiLeptonicTauSelection.h"
#include "../../Tools/interface/EventShapes.h"
#include "TRandom.h"

SemiLeptonicTauSelection::SemiLeptonicTauSelection()
{
      btagAlgo_ = -1;
      btagDiscriCut_ = -999.;
      NofBtagJets_ = 0;
      //Fill the table of cuts
      cuts_.push_back("All"); 
      cuts_.push_back("All ttbar MC sel"); 
      cuts_.push_back("Trigger"); 
      cuts_.push_back("$>=4$ jets ");
      cuts_.push_back("tau");
      cuts_.push_back("e mu veto");
      cuts_.push_back("1 btag");
      cuts_.push_back("met");
      //cuts_.push_back("2 btag");
      
      //Fill Channels
      channels_.push_back(string("all"));      
       
      //TFile* f1 = new  TFile("/opt/sbg/data/data1/cms/cferro/Analysis_allversions/official8/CMSSW_4_2_5/src/MiniTreeAnalysis/NTupleAnalysis/macros/GOOD_Turnon/TriggerEffHisto_JetLeg40_simmetricBinSingleMuon_QuadJet40.root");
      //Paper file TFile* f1 = new  TFile("/opt/sbg/data/data1/cms/cferro/TOP_APPROVAL/CMSSW_4_2_8_patch7/src/MiniTreeAnalysis/NTupleAnalysis/macros/TopTauJets/TriggerEffHisto_data_match40_JETLEG.root");
      //Same as paper file stored here for backup
      TFile* f1 = new  TFile("rootFilesTurnOn/TriggerEffHisto_data_match40_JETLEG.root");
      f1->cd();
      Jet40_Leg = (TH1F*)f1->Get("jet4_eff"); 
      
      //TFile* f2 = new  TFile("/opt/sbg/data/data1/cms/cferro/Analysis_allversions/official8/CMSSW_4_2_5/src/MiniTreeAnalysis/NTupleAnalysis/macros/GOOD_Turnon/TriggerEffHisto_JetLeg45_simmetricBinSingleMuon_Full.root");
      //Paper file TFile* f2 = new  TFile("/opt/sbg/data/data1/cms/cferro/TOP_APPROVAL/CMSSW_4_2_8_patch7/src/MiniTreeAnalysis/NTupleAnalysis/macros/TopTauJets/TriggerEffHisto_data_match45_JETLEG.root");
      //Same as paper file stored here for backup
      TFile* f2 = new  TFile("rootFilesTurnOn/TriggerEffHisto_data_match45_JETLEG.root");
      f2->cd();
      Jet45_Leg = (TH1F*)f2->Get("jet4_eff");
      
      //PAS TFile* f3 = new TFile("/opt/sbg/data/data1/cms/lebihan/clean_january_2012_2/CMSSW_4_2_8_patch7/src/MiniTreeAnalysis/NTupleAnalysis/macros/TopTauJets/rootFiles/TriggerEffHisto_match40_newTauID_AC.root");
      //Paper file TFile* f3 = new TFile("/opt/sbg/data/data1/cms/lebihan/clean_january_2012_2/CMSSW_4_2_8_patch7/src/MiniTreeAnalysis/NTupleAnalysis/macros/TopTauJets/TriggerEffHisto_match40_newTauID.root");
      //Same as paper file stored here for backup 
      TFile* f3 = new TFile("rootFilesTurnOn/TriggerEffHisto_match40_newTauID.root");
      f3->cd();
      Tau40_Leg= (TH1F*)f3->Get("eff_tau");
      
      //PAS TFile* f4 = new TFile("/opt/sbg/data/data1/cms/lebihan/clean_january_2012_2/CMSSW_4_2_8_patch7/src/MiniTreeAnalysis/NTupleAnalysis/macros/TopTauJets/rootFiles/TriggerEffHisto_match45_newTauID_AC.root");
      //Paper file TFile* f4 = new TFile("/opt/sbg/data/data1/cms/lebihan/clean_january_2012_2/CMSSW_4_2_8_patch7/src/MiniTreeAnalysis/NTupleAnalysis/macros/TopTauJets/TriggerEffHisto_match45_newTauID.root");
      //Same as paper file stored here for backup 
      TFile* f4 = new TFile("rootFilesTurnOn/TriggerEffHisto_match45_newTauID.root");
      
      f4->cd();
      Tau45_Leg= (TH1F*)f4->Get("eff_tau");
      
      reader = new TMVA::Reader( "" ); 
      reader->AddVariable( "aplanarity", &aplanarity);
      reader->AddVariable( "chargeEta" , &chargeEta);
      reader->AddVariable( "Met",        &Met);
      reader->AddVariable( "MtauJet",    &MTauJet); 
      reader->AddVariable( "Ht" ,        &Ht); 
      reader->AddVariable( "kinFitChi2", &kinFitChi2);
      reader->AddVariable( "DeltaPhiTauMet", &DeltaPhiTauMet);
      //reader->AddVariable( "D", &D);
      
     
      //reader->BookMVA("MLP","/opt/sbg/data/data1/cms/lebihan/official8/CMSSW_4_2_5/src/MiniTreeAnalysis/NTupleAnalysis/macros/weights/TMVAClassification_MLP_09_01_2012_2011B_L1Seed_looseTau.weights.xml");
      //reader->BookMVA("MLP","/opt/sbg/data/data1/cms/lebihan/official8/CMSSW_4_2_5/src/MiniTreeAnalysis/NTupleAnalysis/macros/weights/TMVAClassification_MLP_09_01_2012_2011B_L1Seed_medTau.weights.xml");

      //reader->BookMVA("MLP","/opt/sbg/data/data1/cms/lebihan/official8/CMSSW_4_2_5/src/MiniTreeAnalysis/NTupleAnalysis/macros/weights/TMVAClassification_MLP_09_01_2012_2011B_L1Seed_looseTau_met25.weights.xml");
      //reader->BookMVA("MLP","/opt/sbg/data/data1/cms/lebihan/official8/CMSSW_4_2_5/src/MiniTreeAnalysis/NTupleAnalysis/macros/weights/TMVAClassification_MLP_medtau_met15_loosebtag.weights.xml");
      //reader->BookMVA("MLP","/opt/sbg/data/data1/cms/lebihan/official8/CMSSW_4_2_5/src/MiniTreeAnalysis/NTupleAnalysis/macros/weights/TMVAClassification_MLP_medtau_met15_medbtag.weights.xml");
       
      //reader->BookMVA("MLP","/opt/sbg/data/data1/cms/lebihan/clean_january_2012_2/CMSSW_4_2_8_patch7/src/MiniTreeAnalysis/NTupleAnalysis/macros/TopTauJets/weights/TMVAClassification_MLP_medtau_met20_medbtag_438_p7.weights.xml");

      //reader->BookMVA("MLP","/opt/sbg/data/data1/cms/lebihan/clean_january_2012_2/CMSSW_4_2_8_patch7/src/MiniTreeAnalysis/NTupleAnalysis/macros/TopTauJets/weights/TMVAClassification_MLP_medtau_met20_medbtag_438_p7_oldTauID.weights.xml");
      //reader->BookMVA("MLP","/opt/sbg/data/data1/cms/lebihan/clean_january_2012_2/CMSSW_4_2_8_patch7/src/MiniTreeAnalysis/NTupleAnalysis/macros/TopTauJets/weights/TMVAClassification_MLP_medtau_met20_medbtag_438_p7_BTV003_newtauID_jetleg.weights.xml");
      //reader->BookMVA("MLP","/opt/sbg/data/data1/cms/cferro/TMVA_official/TMVA-4.0.1/macros/weights/TMVAClassification_MLP_newTurnOn.weights.xml");
     
      reader->BookMVA("MLP","/opt/sbg/data/data1/cms/lebihan/clean_january_2012_2/CMSSW_4_2_8_patch7/src/MiniTreeAnalysis/NTupleAnalysis/macros/TopTauJets/weights/TMVAClassification_MLP_newmedtau_met20_medbtag_438_p7_BTV003_newPU_newTauLeg_JP_AC.xml");
         
}



SemiLeptonicTauSelection::SemiLeptonicTauSelection(const SemiLeptonicTauSelection & s){
      cuts_          = s.cuts_;
      btagAlgo_      = s.btagAlgo_;
      btagDiscriCut_ = s.btagDiscriCut_;
      NofBtagJets_   = s.NofBtagJets_;
      METCut_        = s.METCut_;
}
     
SemiLeptonicTauSelection::~SemiLeptonicTauSelection(){}


void SemiLeptonicTauSelection::SetParameters(int btagAlgo, float btagDiscriCut, int NofBtagJets, float METCut){
        btagAlgo_      = btagAlgo;
        btagDiscriCut_ = btagDiscriCut;
        NofBtagJets_   = NofBtagJets;
        METCut_        = METCut; 
}

      
int SemiLeptonicTauSelection::doFullSelection(Dataset* dataset, string channelName){
      
        bjetsAna.clear();
        jetsAna.clear();
        tausAna.clear();
 
        bool filterTTbarMC = false;
        if(dataset->Name() == "TTbarEE"       && ( tmeme == 2  || tmeme == 10101 || tmeme == 20200 )) filterTTbarMC = true;
        if(dataset->Name() == "TTbarMM"       && ( tmeme == 20 || tmeme == 11010 || tmeme == 22000 )) filterTTbarMC = true;
        if(dataset->Name() == "TTbarEM"       && ( tmeme == 11 || tmeme == 11001 || tmeme == 10110 || tmeme == 21100 )) filterTTbarMC = true;
        if(dataset->Name() == "TTbarETauH"    && ( tmeme==10010 || tmeme==21000 )) filterTTbarMC = true;
        if(dataset->Name() == "TTbarMTauH"    && ( tmeme==10001 || tmeme==20100 )) filterTTbarMC = true;
        if(dataset->Name() == "TTbarLQQ"      && ( tmeme==1 || tmeme==10 || tmeme==10100 || tmeme==11000 )) filterTTbarMC = true;
        if(dataset->Name() == "TTbarTauHQQ"   && ( tmeme==10000)) filterTTbarMC = true;
        if(dataset->Name() == "TTbarTauHTauH" && ( tmeme==20000)) filterTTbarMC = true;
        if(dataset->Name() == "TTbarQQQQ"     && ( tmeme==0)) filterTTbarMC = true;
        if(dataset->Name() == "TTbarLL"       && 
          (tmeme == 2  || tmeme == 10101 || tmeme == 20200 || 
           tmeme == 20 || tmeme == 11010 || tmeme == 22000 ||
           tmeme == 11 || tmeme == 11001 || tmeme == 10110 || tmeme == 21100)) filterTTbarMC = true;
        
                
       //vector<NTMuon> muon_cand      = GetSelectedMuonsNoIso();
       //vector<NTElectron> elec_cand  = GetSelectedElectronsNoIso();
       
       vector<NTMuon> muon_cand      = GetSelectedMuonsLooseIDLooseIso();
       vector<NTElectron> elec_cand  = GetSelectedElectronsLooseIDLooseIso();
       
       vector<NTTau> tau_cand;
       if (dataset->Name().find("DATA")==std::string::npos)
       tau_cand  = GetSelectedTaus(muon_cand, elec_cand, false ,1.0,1,true);
       else
       tau_cand  = GetSelectedTaus(muon_cand, elec_cand, false,1.0,1,true);
       tausAna = tau_cand;
       vector<NTJet> jet_cand = GetSelectedJets(muon_cand, elec_cand, tau_cand,false,1.,false,1);
       jetsAna = jet_cand;
        
       /*
       vector<TLorentzVector> genATaus = GetGenATaus();
       std::cout <<"genATaus.size() " << genATaus.size() << std::endl;
       for(unsigned int j=0;j<genATaus.size();j++){
        std::cout <<"genTaus " << genATaus[j].Eta() <<std::endl;
       }*/
       
        int FinalStep = 0;
        
        
        //Step 1        
        if( (filterTTbarMC && dataset->Name().find("TTbar")!=std::string::npos) || dataset->Name().find("TTbar")==std::string::npos){
         FinalStep++;
           
        //Step 2 Trigger
        // 173692 last 2011A run
        // 175832 first 2011B run
        // QuadJet45 : prescaled 178421 178479 178703 178786 178803 178840 178920  178970 179411 179434 179497 179547 179889 
        // QuadJet45 not in, run >= 179959     
        
        if (  (  dataset->Name().find("DATA")!=std::string::npos && 
                ((passTriggerQuadJet40IsoPFTauSelection() && (runNumber<165970  || (runNumber>166782 && runNumber<171050) ) ) ||
                (passTriggerQuadJet45IsoPFTauSelection() && ((runNumber>=165970 && runNumber<=166782) || (runNumber>=171050 && runNumber<178421)))   ))
             ||
             dataset->Name().find("DATA")==std::string::npos  
            )
          { 
          FinalStep++;
        vector<TLorentzVector> jetObjTrig = GetJetTrigObj();
        
        //Step 3 Minimal jet multiplicity 
        int njets_45 = 0; 
        for (unsigned int j=0;j<jet_cand.size();j++)
            { 
               if (jet_cand[j].p4.Pt()>45) 
               {
               /*float deltaRMinJetTrig =99.; 
               float deltaRJetTrig =99.;
               for(unsigned int k=0;k<jetObjTrig.size();k++)
               {
               deltaRJetTrig = jet_cand[j].p4.DeltaR(jetObjTrig[k]);
               if (deltaRJetTrig<deltaRMinJetTrig) deltaRMinJetTrig = deltaRJetTrig; }
               
               if(deltaRMinJetTrig<0.4)*/
               njets_45++;}
             }
          
        if( jet_cand.size()>=4 && njets_45>=3){ 
          FinalStep++;
        
        //Step 4 One tau 
        if( tau_cand.size()==1){
        
        vector<TLorentzVector> tauObjTrig = GetTauTrigObj();
        
        //std::cout <<"sizes " <<tauObjTrig.size()<< " "<< tau_cand.size()<< std::endl;
        
        /*if( tau_cand.size()==1 && tauObjTrig.size()>=1 ){
        
          float deltaRMinTauTrig = 99.;
          float deltaRTauTrig = 99.;
          for(unsigned int j=0;j<tauObjTrig.size();j++)
          {
           deltaRTauTrig = tau_cand[0].p4.DeltaR(tauObjTrig[j]);
           if(deltaRTauTrig<deltaRMinTauTrig) deltaRMinTauTrig = deltaRTauTrig; }
        
        if(deltaRMinTauTrig<0.4){*/
          
         //std::cout <<"FinalStep " <<FinalStep <<  std::endl;
         FinalStep++;
         
        //Step 5 
        if( muon_cand.size()==0 && elec_cand.size()==0 ){
           FinalStep++;
           
        //Step 6 
        vector<NTJet> btagjets;
        vector<float> btagDiscri;
        for(unsigned int j=0;j<jet_cand.size();j++){
                switch(btagAlgo_){
                        case 0 :
                                if(jet_cand[j].TCDiscri>=btagDiscriCut_){
                                         btagjets.push_back(jet_cand[j]);
                                         btagDiscri.push_back(jet_cand[j].TCDiscri);
                                }
                                break;  
                        case 1 :
                                if(jet_cand[j].SVDiscri>=btagDiscriCut_){
                                        btagjets.push_back(jet_cand[j]);        
                                        btagDiscri.push_back(jet_cand[j].SVDiscri);
                                }
                                break;  
                        case 2 :
                                if(jet_cand[j].SMDiscri>=btagDiscriCut_){
                                        btagjets.push_back(jet_cand[j]);        
                                        btagDiscri.push_back(jet_cand[j].SMDiscri);
                                }
                                break;  
                        case 4 :
                                //std::cout << "JP discri " << jet_cand[j].GetDiscri("jetProbabilityBJetTags") << std::endl;
                                if(jet_cand[j].GetDiscri("jetProbabilityBJetTags")>=btagDiscriCut_){
                                        btagjets.push_back(jet_cand[j]);        
                                        btagDiscri.push_back(jet_cand[j].GetDiscri("jetProbabilityBJetTags"));
                                }
                                break;  
                                
                        default:
                                cerr << "btagAlgo doesn't exist !"<<endl;
                                break;  
                }
        }
        bjetsAna = btagjets;
        
          
        //if((dataset->Name().find("DATA")!=std::string::npos && (int)btagjets.size()==0) || dataset->Name().find("DATA")==std::string::npos) {
        if((dataset->Name().find("DATA")!=std::string::npos && (int)btagjets.size()>=1) || 
           (dataset->Name().find("DATA")==std::string::npos)) {
        
        //      if((int) btagjets.size()>=1){
                FinalStep++;
                
        //Step 7
        if( GetMET().p4.Et()>=20){
          FinalStep++;
        
          } 
         }
        //}
       }
      }
     }
    }
   }
   
   return FinalStep;

}

std::vector<float> SemiLeptonicTauSelection::GetNNOutputAndKinFit()
{   
    std::vector<float> output;

    float NNOutput = -99.;
    kinFitChi2 = -99.;
    kinFitTopMass = -99.;
    float kinFitProba = -99.;
   
    /* 
    vector<NTMuon> muon_cand      = GetSelectedMuonsNoIso();
    vector<NTElectron> elec_cand  = GetSelectedElectronsNoIso(); 
    vector<NTTau> tau_cand        = GetSelectedTaus(muon_cand, elec_cand,false,1,0,true);
    vector<NTJet> jet_cand        = GetSelectedJets(muon_cand, elec_cand, tau_cand);*/
    NTMET met = GetMET();
    
    
    if (tausAna.size()==1 && jetsAna.size() >= 4)
    { 
      //------------------------------------ kinFitter stuff
        
      std::vector<NTMET> mets;
      mets.push_back(met);
    
      vector<NTLepton> lep;
      for (unsigned int i =0; i<tausAna.size(); i++)
        lep.push_back(dynamic_cast<NTLepton&> (tausAna.at(i)));
    
      std::string lepType = "tau";
      kinFitter.produce(jetsAna, lep,"tau", mets);
      
      kinFitChi2 = kinFitter.GetFitChi2();
      kinFitProba = kinFitter.GetFitProb();
     
      std::vector<int> jetCombi = kinFitter.GetFitJetCombi();
       
      TLorentzVector HadB = kinFitter.GetFitHadB();
      TLorentzVector HadP = kinFitter.GetFitHadP();
      TLorentzVector HadQ = kinFitter.GetFitHadQ();   
      TLorentzVector LepB = kinFitter.GetFitLepB();   
      TLorentzVector LepL = kinFitter.GetFitLepL();   
      TLorentzVector LepN = kinFitter.GetFitLepN();   
      
      //TLorentzVector HadSide = HadB+HadP+HadQ;
      //std::cout << "M top had "<< HadSide.M() <<std::endl; 
      //kinFitTopMass = HadSide.M();
      
      TLorentzVector HadSide = jetsAna[jetCombi[0]].p4 + jetsAna[jetCombi[1]].p4 +jetsAna[jetCombi[2]].p4;
      kinFitTopMass = HadSide.M();
      
      TLorentzVector LepSide = LepB+LepL+LepN;
      //std::cout << "M top Lep "<< LepSide.M() <<std::endl; 
      TLorentzVector WLepSide = LepL+LepN;
      //std::cout << "M W lep "<< WLepSide.M() <<std::endl; 
      TLorentzVector WHadSide = HadP+HadQ;
      //std::cout << "M W  Had"<< WHadSide.M() <<std::endl; 
      
      
      //------------------------------------  NN stuff
      
      EventShapes evShape(jetsAna, tausAna);
      EventShapes evShapeM3(jetsAna);
    
      aplanarity  = evShape.aplanarity();
      circularity = evShape.circularity();
      sphericity  = evShape.sphericity();
      chargeEta   = tausAna[0].Charge*fabs(tausAna[0].p4.Eta()); 
      Met         = met.p4.Et();
      Mt          = sqrt (pow(tausAna[0].p4.Et() + met.p4.Et(),2) - pow(tausAna[0].p4.Px() + met.p4.Px(),2) - pow(tausAna[0].p4.Py() + met.p4.Py(),2)); 
      Ht          = evShape.HT();
      chi2        = evShape.chi2(jetsAna);
      C           = evShape.C();
      D           = evShape.D();
      MTauJet     = evShape.sqrt_s();   
      Ht3         = evShapeM3.HT3();
      M3          = evShapeM3.M3chi2(jetsAna);
      M2          = evShapeM3.M2chi2(jetsAna);
      
      DeltaPhiTauMet = fabs(tausAna[0].p4.DeltaPhi(met.p4));
      
      NNOutput = reader->EvaluateMVA("MLP"); 
      
      }
    
    output.push_back(NNOutput);
    output.push_back(kinFitChi2);
    output.push_back(kinFitTopMass);
    output.push_back(kinFitProba);
   
    return output;
    
}


int SemiLeptonicTauSelection::FillTable(SelectionTable& selTable, Dataset* dataset, int idataset, float weight,int triggerMenu){
        int sel = doFullSelection(dataset, selTable.Channel());
        for(unsigned int i=0;i<cuts_.size()+1;i++)
                if(sel >=(int)i ) {
                
                                    if (dataset->Name().find("DATA")==std::string::npos && i>5){
                                    selTable.Fill(idataset,i,weight*getWeightAtLeast1bjet()*getWeightTau()*getWeightTauLeg(triggerMenu)*getWeightJetLegs(triggerMenu)); 
                                    //selTable.Fill(idataset,i,weight*getWeightTau()*getWeightTauLeg(triggerMenu)*getWeightJetLegs(triggerMenu)); 
                                    
                                    //std::cout <<"i>5 " << sel << " "<<i<<" " << getWeightTau() << " " << getWeightAtLeast1bjet()<< std::endl;
                                    }
                                    else if (dataset->Name().find("DATA")==std::string::npos && i>3 && i<=5){
                                    selTable.Fill(idataset,i,weight*getWeightTau()*getWeightTauLeg(triggerMenu)*getWeightJetLegs(triggerMenu)); 
                                    //std::cout <<"i>3 " << sel << " "<<i<<" "  << getWeightTau() << std::endl;
                                    }
                                    else {
                                    selTable.Fill(idataset,i, weight);
                                    //std::cout <<"any i " << std::endl;
                                    }
                                  }
        return sel;
}


float SemiLeptonicTauSelection::getWeightAtLeast1bjet() {  

  //vector<NTMuon> muon_cand      = GetSelectedMuonsNoIso();
  //vector<NTElectron> elec_cand  = GetSelectedElectronsNoIso();  
  //vector<NTTau> tau_cand = GetSelectedTaus(muon_cand, elec_cand, false,1,0,true);
  //vector<NTJet> jet_cand = GetSelectedJets(muon_cand, elec_cand, tau_cand);
   
  /*
  vector < float >weightb;
  weightb.push_back (1.);
  weightb.push_back (0.);
  weightb.push_back (0.);
  weightb.push_back (0.);
  weightb.push_back (0.);*/
       
  vector < float >weight_temp = GetSFBweight().GetWeigth4BSel (GetMethodb(), GetSystb(), jetsAna);
  //weightb[0] = weight_temp[0]; //weight of the event
  //weightb[1] = weight_temp[1];  //proba 0 jet
  //weightb[2] = weight_temp[2];  //proba 1 jet
  //weightb[3] = weight_temp[3];  //proba 2 jets
  //weightb[4] = weight_temp[4];  //proba at least 3 jets
 
  return 1.- weight_temp[1];
  //return weight_temp[0];// For direct tagging with SF !
             
}            

float SemiLeptonicTauSelection::getWeightAtLeast1bjetTMVA() {  

  //vector<NTMuon> muon_cand    = GetSelectedMuonsNoIso();
  //vector<NTElectron> elec_cand  = GetSelectedElectronsNoIso();   
  
  vector<NTMuon> muon_cand      = GetSelectedMuonsLooseIDLooseIso();
  vector<NTElectron> elec_cand  = GetSelectedElectronsLooseIDLooseIso();
  
  vector<NTTau> tau_cand = GetSelectedTaus(muon_cand, elec_cand, false,1,1,true);
  vector<NTJet> jet_cand = GetSelectedJets(muon_cand, elec_cand, tau_cand);   
       
  vector < float >weight_temp = GetSFBweight().GetWeigth4BSel (GetMethodb(), GetSystb(), jet_cand);
  
  return 1.- weight_temp[1];
             
}            

float SemiLeptonicTauSelection::getWeightTau() {
  
  float weight = 1.;
  //vector<NTTau> tau_cand = GetSelectedTaus(muon_cand, elec_cand, false,1,0,true);
  
  if (tausAna.size()==1)
  {
   if (tausAna[0].LeptonOrigin==1) weight = 0.96; // +/- 0.07
   else if (tausAna[0].LeptonOrigin==0) weight = 1.0; // +/- 0.04
  }
  return weight;

} 
             

bool SemiLeptonicTauSelection::passTriggerMuonSelection(){
  
   bool pass = false;    
   for(unsigned int i=0; i< triggerList.size(); i++)
   {  
     if(( triggerList[i].first == "HLT_Mu15_v1" && triggerList[i].second == true  )) { pass = true;}
     if(( triggerList[i].first == "HLT_Mu15_v2" && triggerList[i].second == true  )) { pass = true;}
     if(( triggerList[i].first == "HLT_Mu15_v3" && triggerList[i].second == true  )) { pass = true;}
     if(( triggerList[i].first == "HLT_Mu15_v4" && triggerList[i].second == true  )) { pass = true;}
     if(( triggerList[i].first == "HLT_Mu15_v5" && triggerList[i].second == true  )) { pass = true;}
     if(( triggerList[i].first == "HLT_Mu20_v1" && triggerList[i].second == true  )) { pass = true;}
     if(( triggerList[i].first == "HLT_Mu20_v2" && triggerList[i].second == true  )) { pass = true;}
     if(( triggerList[i].first == "HLT_Mu20_v3" && triggerList[i].second == true  )) { pass = true;}
     if(( triggerList[i].first == "HLT_Mu20_v4" && triggerList[i].second == true  )) { pass = true;}
     if(( triggerList[i].first == "HLT_Mu20_v5" && triggerList[i].second == true  )) { pass = true;}
     if(( triggerList[i].first == "HLT_Mu24_v1" && triggerList[i].second == true  )) { pass = true;}
     if(( triggerList[i].first == "HLT_Mu24_v2" && triggerList[i].second == true  )) { pass = true;}
     if(( triggerList[i].first == "HLT_Mu24_v3" && triggerList[i].second == true  )) { pass = true;}
     if(( triggerList[i].first == "HLT_Mu24_v4" && triggerList[i].second == true  )) { pass = true;}
     if(( triggerList[i].first == "HLT_Mu24_v5" && triggerList[i].second == true  )) { pass = true;}
     if(( triggerList[i].first == "HLT_Mu30_v1" && triggerList[i].second == true  )) { pass = true;}
     if(( triggerList[i].first == "HLT_Mu30_v2" && triggerList[i].second == true  )) { pass = true;}
     if(( triggerList[i].first == "HLT_Mu30_v3" && triggerList[i].second == true  )) { pass = true;}
     if(( triggerList[i].first == "HLT_Mu30_v4" && triggerList[i].second == true  )) { pass = true;}
     if(( triggerList[i].first == "HLT_Mu30_v5" && triggerList[i].second == true  )) { pass = true;}   
   } 
 
  return pass;
  
}

bool SemiLeptonicTauSelection::passTriggerQuadJet40Selection(){
  
  
   bool pass = false; 
   int j =-1;
        
   for(unsigned int i=0; i< triggerList.size(); i++)
   {
     
     if (triggerList[i].first.find("HLT_QuadJet40_v") != string::npos && triggerList[i].second == true)
     { 
      pass = true; j=i; break;}
        
   } 
 
   return pass;
  
}

bool SemiLeptonicTauSelection::passTriggerQuadJet40IsoPFTauSelection(){
  
  
   bool pass = false;
   int j=-1; 
   
   for(unsigned int i=0; i< triggerList.size(); i++)
   {     
     if(triggerList[i].first.find("HLT_QuadJet40_IsoPFTau40_v") != string::npos && triggerList[i].second == true) 
     { pass = true; j=i; break;}
    } 
  
  return pass;
  
}

bool SemiLeptonicTauSelection::passTriggerQuadJet45IsoPFTauSelection(){
  
  
   bool pass = false;
   int j=-1; 
   
   for(unsigned int i=0; i< triggerList.size(); i++)
   {     
     if(triggerList[i].first.find("HLT_QuadJet45_IsoPFTau45_v") != string::npos && triggerList[i].second == true) 
     { pass = true; j=i; break;}
    } 
  
  //if (pass==true) std::cout <<"QuadJet IsoPFTau  name " <<triggerList[j].first<< " " << pass << std::endl; 
  return pass;
  
}


float SemiLeptonicTauSelection::passTriggerQuadJet40PrescaledSelection(){
  
  
   float prescale = 0;
   bool  passQuadJet40_v1 = false;
   bool  passQuadJet40_v2 = false;
   bool  passQuadJet40_v3 = false;
   bool  passQuadJet40_v4 = false;
   bool  passQuadJet40_v5 = false;
   bool  passQuadJet40_v6 = false;
   bool  passQuadJet40_v7 = false;
  
   for(unsigned int i=0; i< triggerList.size(); i++)
   {  
     if (triggerList[i].first == "HLT_QuadJet40_v1"  && triggerList[i].second == true) passQuadJet40_v1 =true;
     if (triggerList[i].first == "HLT_QuadJet40_v2"  && triggerList[i].second == true) passQuadJet40_v2 =true;
     if (triggerList[i].first == "HLT_QuadJet40_v3"  && triggerList[i].second == true) passQuadJet40_v3 =true;
     if (triggerList[i].first == "HLT_QuadJet40_v4"  && triggerList[i].second == true) passQuadJet40_v4 =true;
     if (triggerList[i].first == "HLT_QuadJet40_v5"  && triggerList[i].second == true) passQuadJet40_v5 =true;
     if (triggerList[i].first == "HLT_QuadJet40_v6"  && triggerList[i].second == true) passQuadJet40_v6 =true;     
     if (triggerList[i].first == "HLT_QuadJet40_v7"  && triggerList[i].second == true) passQuadJet40_v7 =true;     
   }
   
   
   if (passQuadJet40_v1==true)
   { 
     for(unsigned int i=0; i< triggerPrescaleList.size(); i++)
     {if( triggerPrescaleList[i].first == "HLT_QuadJet40_v1")  prescale = triggerPrescaleList[i].second ;}
    }
   if (passQuadJet40_v2==true)
   { 
     for(unsigned int i=0; i< triggerPrescaleList.size(); i++)
     {if( triggerPrescaleList[i].first == "HLT_QuadJet40_v2")  prescale = triggerPrescaleList[i].second ;}
    }
   if (passQuadJet40_v3==true)
   { 
     for(unsigned int i=0; i< triggerPrescaleList.size(); i++)
     {if( triggerPrescaleList[i].first == "HLT_QuadJet40_v3")  prescale = triggerPrescaleList[i].second ;}
    }
   if (passQuadJet40_v4==true)
   { 
     for(unsigned int i=0; i< triggerPrescaleList.size(); i++)
     {if( triggerPrescaleList[i].first == "HLT_QuadJet40_v4")  prescale = triggerPrescaleList[i].second ;}
    }
   if (passQuadJet40_v5==true)
   { 
     for(unsigned int i=0; i< triggerPrescaleList.size(); i++)
     {if( triggerPrescaleList[i].first == "HLT_QuadJet40_v5")  prescale = triggerPrescaleList[i].second ;}
    }
   if (passQuadJet40_v6==true)
   { 
     for(unsigned int i=0; i< triggerPrescaleList.size(); i++)
     {if( triggerPrescaleList[i].first == "HLT_QuadJet40_v6")  prescale = triggerPrescaleList[i].second ;}
    }
   if (passQuadJet40_v7==true)
   { 
     for(unsigned int i=0; i< triggerPrescaleList.size(); i++)
     {if( triggerPrescaleList[i].first == "HLT_QuadJet40_v7")  prescale = triggerPrescaleList[i].second ;}
    }
  
  
  
  return prescale;
   
}

 

float SemiLeptonicTauSelection::QuadJet40Prescale(){
  
 
   float prescale = 0;
   bool  QuadJet40_v1 = false;
   bool  QuadJet40_v2 = false;
   bool  QuadJet40_v3 = false;
   bool  QuadJet40_v4 = false;
   bool  QuadJet40_v5 = false;
   bool  QuadJet40_v6 = false;
   bool  QuadJet40_v7 = false;
   
   for(unsigned int i=0; i< triggerList.size(); i++)
   {  
     if (triggerList[i].first == "HLT_QuadJet40_v1" ) QuadJet40_v1 =true;
     if (triggerList[i].first == "HLT_QuadJet40_v2" ) QuadJet40_v2 =true;
     if (triggerList[i].first == "HLT_QuadJet40_v3" ) QuadJet40_v3 =true;
     if (triggerList[i].first == "HLT_QuadJet40_v4" ) QuadJet40_v4 =true;
     if (triggerList[i].first == "HLT_QuadJet40_v5" ) QuadJet40_v5 =true;
     if (triggerList[i].first == "HLT_QuadJet40_v6" ) QuadJet40_v6 =true; 
     if (triggerList[i].first == "HLT_QuadJet40_v7" ) QuadJet40_v7 =true;     
     }
   
   
   if (QuadJet40_v1==true)
   { 
     for(unsigned int i=0; i< triggerPrescaleList.size(); i++)
     {if( triggerPrescaleList[i].first == "HLT_QuadJet40_v1")  prescale = triggerPrescaleList[i].second ;}
    }
   if (QuadJet40_v2==true)
   { 
     for(unsigned int i=0; i< triggerPrescaleList.size(); i++)
     {if( triggerPrescaleList[i].first == "HLT_QuadJet40_v2")  prescale = triggerPrescaleList[i].second ;}
    }
   if (QuadJet40_v3==true)
   { 
     for(unsigned int i=0; i< triggerPrescaleList.size(); i++)
     {if( triggerPrescaleList[i].first == "HLT_QuadJet40_v3")  prescale = triggerPrescaleList[i].second ;}
    }
   if (QuadJet40_v4==true)
   { 
     for(unsigned int i=0; i< triggerPrescaleList.size(); i++)
     {if( triggerPrescaleList[i].first == "HLT_QuadJet40_v4")  prescale = triggerPrescaleList[i].second ;}
    }
   if (QuadJet40_v5==true)
   { 
     for(unsigned int i=0; i< triggerPrescaleList.size(); i++)
     {if( triggerPrescaleList[i].first == "HLT_QuadJet40_v5")  prescale = triggerPrescaleList[i].second ;}
    }
   if (QuadJet40_v6==true)
   { 
     for(unsigned int i=0; i< triggerPrescaleList.size(); i++)
     {if( triggerPrescaleList[i].first == "HLT_QuadJet40_v6")  prescale = triggerPrescaleList[i].second ;}
    }
   if (QuadJet40_v7==true)
   { 
     for(unsigned int i=0; i< triggerPrescaleList.size(); i++)
     {if( triggerPrescaleList[i].first == "HLT_QuadJet40_v7")  prescale = triggerPrescaleList[i].second ;}
    }
    
  return prescale;
   
}



float SemiLeptonicTauSelection::GetQuadJet40IsoPFTau40Weight()
{
 
 vector<NTTau> tau_cand_trig   = GetSelectedTaus(false, 1., 0, false);
 vector<NTJet> jet_cand_trig   = GetSelectedJets();
    
 float weight = 1.;
 if (jet_cand_trig.size()>=4 && tau_cand_trig.size()>=1)
 {
   for (int i =0; i<4; i++) weight *= GetQuadJet40IsoPFTau40JetLegTurnOn(jet_cand_trig[i].p4.Pt());
   weight *= 0.75;
 }
 return weight;
}

float SemiLeptonicTauSelection::GetQuadJet40IsoPFTau40JetLegTurnOn(float pT)
{
 float weight = 0.;
 weight = CrystalBallInt(pT,3.50000e+01,4.50000e-01,4.17472e-02,6.50000e+00,9.50000e-01);
 
   /*
   1 alpha 4.17472e-02   1.93212e+00   0.00000e+00  -1.21538e+00
   2  mean         3.50000e+01   1.26221e+01   0.00000e+00   0.00000e+00
   3  n            6.50000e+00   1.04390e+01   0.00000e+00  -7.49696e-01
   4  norm         9.50000e-01   3.25900e-02   0.00000e+00   2.52680e-01
   5  sigma        4.50000e-01*/
  
 return weight;
}


Double_t SemiLeptonicTauSelection::CrystalBallInt(Double_t pT, Double_t mean, Double_t sigma, Double_t alpha, Double_t n, Double_t norm) 
{ 
   
   const double sqrtPiOver2 = 1.2533141373;
   const double sqrt2 = 1.4142135624;

   Double_t sig = fabs((Double_t) sigma);
   
   Double_t t = (pT - mean)/sig ;
   
   if (alpha < 0)
     t = -t;

   Double_t absAlpha = fabs(alpha / sig);
   Double_t a = TMath::Power(n/absAlpha,n)*exp(-0.5*absAlpha*absAlpha);
   Double_t b = absAlpha - n/absAlpha;
   
   if (a>=std::numeric_limits<double>::max()) return -1. ;
   //cout<<"P1 "<<absAlpha<<" "<<a<<" "<<b<<endl ;

   Double_t aireGauche = (1 + ApproxErf( absAlpha / sqrt2 )) * sqrtPiOver2 ;
   Double_t aireDroite = ( a * 1/TMath::Power(absAlpha - b,n-1)) / (n - 1);
   Double_t aire = aireGauche + aireDroite;

   //cout<<"P2 "<<aireGauche<<" "<<aireDroite<<" "<<aire<<endl ;

   if ( t <= absAlpha ){
     return norm * (1 + ApproxErf( t / sqrt2 )) * sqrtPiOver2 / aire ;
   }
   else{
     return norm * (aireGauche +  a * (1/TMath::Power(t-b,n-1) - 1/TMath::Power(absAlpha - b,n-1)) / (1 - n)) / aire ;
   }
   
  
} 


Double_t SemiLeptonicTauSelection::ApproxErf(Double_t arg) 
{ 
  static const double erflim = 5.0;
  if( arg > erflim )
    return 1.0;
  if( arg < -erflim )
    return -1.0;
      
  //return RooMath::erf(arg);
  return TMath::Erf(arg);
}


float SemiLeptonicTauSelection::GetQuadJet40IsoPFTau40JetLegTurnOnFermi(float pT)
{
 float weight_fermi = 0.;
 weight_fermi = Fermi(pT, 4.63988e-01, 3.89091e+01, 8.74685e+00);
 
 /*
  FCN=463.341 FROM IMProve     STATUS=RESET           9 CALLS         184 TOTAL
                     EDM=1.53542e-11    STRATEGY= 1  ERROR MATRIX UNCERTAINTY  50.0 per cent
  EXT PARAMETER                APPROXIMATE        STEP         FIRST  
  NO.   NAME      VALUE            ERROR          SIZE      DERIVATIVE
  1  p0           4.63988e-01   7.63457e-04   7.63457e-06   3.06426e+03
  2  p1           3.89091e+01   7.64658e-02   7.64658e-04   1.96639e+01
  3  p2           8.74685e+00   7.62449e-02   7.62449e-04  -3.99334e+01*/
   
 return weight_fermi;
}

Double_t SemiLeptonicTauSelection::GetQuadJet40IsoPFTau40JetLegTurnOnFermiError(float pT)
{
  Double_t a  = 4.63988e-01;
  Double_t Da = 7.63457e-04;
  Double_t b  = 3.89091e+01;
  Double_t Db = 7.64658e-02;
  Double_t c  = 8.74685e+00;
  Double_t Dc = 7.62449e-02;
 
  Double_t error = fabs(1.+TMath::Erf((pT-b)/sqrt(c)))*Da + fabs(-2*a*exp(-pow((pT-b),2)/c)/sqrt(c*TMath::Pi()))*Db + fabs(a*(b-pT)*exp(-pow(pT-b,2)/c)/(c*sqrt(c*TMath::Pi())))*Dc;
 /*
  FCN=463.341 FROM IMProve     STATUS=RESET           9 CALLS         184 TOTAL
                     EDM=1.53542e-11    STRATEGY= 1  ERROR MATRIX UNCERTAINTY  50.0 per cent
  EXT PARAMETER                APPROXIMATE        STEP         FIRST  
  NO.   NAME      VALUE            ERROR          SIZE      DERIVATIVE
  1  p0           4.63988e-01   7.63457e-04   7.63457e-06   3.06426e+03
  2  p1           3.89091e+01   7.64658e-02   7.64658e-04   1.96639e+01
  3  p2           8.74685e+00   7.62449e-02   7.62449e-04  -3.99334e+01*/
   
 return error;
}

float SemiLeptonicTauSelection::Fermi(Double_t pT, Double_t a_0, Double_t a_1, Double_t a_2)
{  
 return  a_0 * (1+TMath::Erf((pT-a_1)/(sqrt(2)*a_2)));
} 


float SemiLeptonicTauSelection::GetQuadJet40IsoPFTau40WeightFermi()
{ 
 vector<NTJet> jet_cand_trig   = GetSelectedJets();
 vector<NTTau> tau_cand_trig   = GetSelectedTaus(false, 1., 0, false);
    
 float weight = 1.;
 if (jet_cand_trig.size()>=4 && tau_cand_trig.size()>=1)
 {
   for (int i =0; i<4; i++) weight *= GetQuadJet40IsoPFTau40JetLegTurnOnFermi(jet_cand_trig[i].p4.Pt());
   weight *= 0.75;
  }
 return weight;
}



float SemiLeptonicTauSelection::QuadJet40IsoPFTau40Prescale()
{                                
 
   float prescale = 0;
   bool  QuadJet40_IsoPFTau40_v1 = false;
   bool  QuadJet40_IsoPFTau40_v2 = false;
   bool  QuadJet40_IsoPFTau40_v3 = false;
   bool  QuadJet40_IsoPFTau40_v4 = false;
   bool  QuadJet40_IsoPFTau40_v5 = false;
   bool  QuadJet40_IsoPFTau40_v6 = false;
   bool  QuadJet40_IsoPFTau40_v7 = false;
   bool  QuadJet40_IsoPFTau40_v8 = false;
   bool  QuadJet40_IsoPFTau40_v9 = false;
   bool  QuadJet40_IsoPFTau40_v10 = false;
   bool  QuadJet40_IsoPFTau40_v11 = false;
   bool  QuadJet40_IsoPFTau40_v12 = false;
   
   for(unsigned int i=0; i< triggerList.size(); i++)
   {  
     if (triggerList[i].first == "HLT_QuadJet40_IsoPFTau40_v1" ) QuadJet40_IsoPFTau40_v1 =true;
     if (triggerList[i].first == "HLT_QuadJet40_IsoPFTau40_v2" ) QuadJet40_IsoPFTau40_v2 =true;
     if (triggerList[i].first == "HLT_QuadJet40_IsoPFTau40_v3" ) QuadJet40_IsoPFTau40_v3 =true;
     if (triggerList[i].first == "HLT_QuadJet40_IsoPFTau40_v4" ) QuadJet40_IsoPFTau40_v4 =true;
     if (triggerList[i].first == "HLT_QuadJet40_IsoPFTau40_v5" ) QuadJet40_IsoPFTau40_v5 =true;
     if (triggerList[i].first == "HLT_QuadJet40_IsoPFTau40_v6" ) QuadJet40_IsoPFTau40_v6 =true; 
     if (triggerList[i].first == "HLT_QuadJet40_IsoPFTau40_v7" ) QuadJet40_IsoPFTau40_v7 =true;     
     if (triggerList[i].first == "HLT_QuadJet40_IsoPFTau40_v8" ) QuadJet40_IsoPFTau40_v8 =true;     
     if (triggerList[i].first == "HLT_QuadJet40_IsoPFTau40_v9" ) QuadJet40_IsoPFTau40_v9 =true;     
     if (triggerList[i].first == "HLT_QuadJet40_IsoPFTau40_v10" ) QuadJet40_IsoPFTau40_v10 =true;     
     if (triggerList[i].first == "HLT_QuadJet40_IsoPFTau40_v11" ) QuadJet40_IsoPFTau40_v11 =true;     
     if (triggerList[i].first == "HLT_QuadJet40_IsoPFTau40_v12" ) QuadJet40_IsoPFTau40_v12 =true;     
 
  }
   
   
   if (QuadJet40_IsoPFTau40_v1==true)
   { 
     for(unsigned int i=0; i< triggerPrescaleList.size(); i++)
     {if( triggerPrescaleList[i].first == "HLT_QuadJet40_IsoPFTau40_v1")  prescale = triggerPrescaleList[i].second ;}
    }
   if (QuadJet40_IsoPFTau40_v2==true)
   { 
     for(unsigned int i=0; i< triggerPrescaleList.size(); i++)
     {if( triggerPrescaleList[i].first == "HLT_QuadJet40_IsoPFTau40_v2")  prescale = triggerPrescaleList[i].second ;}
    }
   if (QuadJet40_IsoPFTau40_v3==true)
   { 
     for(unsigned int i=0; i< triggerPrescaleList.size(); i++)
     {if( triggerPrescaleList[i].first == "HLT_QuadJet40_IsoPFTau40_v3")  prescale = triggerPrescaleList[i].second ;}
    }
   if (QuadJet40_IsoPFTau40_v4==true)
   { 
     for(unsigned int i=0; i< triggerPrescaleList.size(); i++)
     {if( triggerPrescaleList[i].first == "HLT_QuadJet40_IsoPFTau40_v4")  prescale = triggerPrescaleList[i].second ;}
    }
   if (QuadJet40_IsoPFTau40_v5==true)
   { 
     for(unsigned int i=0; i< triggerPrescaleList.size(); i++)
     {if( triggerPrescaleList[i].first == "HLT_QuadJet40_IsoPFTau40_v5")  prescale = triggerPrescaleList[i].second ;}
    }
   if (QuadJet40_IsoPFTau40_v6==true)
   { 
     for(unsigned int i=0; i< triggerPrescaleList.size(); i++)
     {if( triggerPrescaleList[i].first == "HLT_QuadJet40_IsoPFTau40_v6")  prescale = triggerPrescaleList[i].second ;}
    }
   if (QuadJet40_IsoPFTau40_v7==true)
   { 
     for(unsigned int i=0; i< triggerPrescaleList.size(); i++)
     {if( triggerPrescaleList[i].first == "HLT_QuadJet40_IsoPFTau40_v7")  prescale = triggerPrescaleList[i].second ;}
    }
   if (QuadJet40_IsoPFTau40_v8==true)
   { 
     for(unsigned int i=0; i< triggerPrescaleList.size(); i++)
     {if( triggerPrescaleList[i].first == "HLT_QuadJet40_IsoPFTau40_v8")  prescale = triggerPrescaleList[i].second ;}
    }
   if (QuadJet40_IsoPFTau40_v9==true)
   { 
     for(unsigned int i=0; i< triggerPrescaleList.size(); i++)
     {if( triggerPrescaleList[i].first == "HLT_QuadJet40_IsoPFTau40_v9")  prescale = triggerPrescaleList[i].second ;}
    }
  if (QuadJet40_IsoPFTau40_v10==true)
   { 
     for(unsigned int i=0; i< triggerPrescaleList.size(); i++)
     {if( triggerPrescaleList[i].first == "HLT_QuadJet40_IsoPFTau40_v10")  prescale = triggerPrescaleList[i].second ;}
    }
  if (QuadJet40_IsoPFTau40_v11==true)
   { 
     for(unsigned int i=0; i< triggerPrescaleList.size(); i++)
     {if( triggerPrescaleList[i].first == "HLT_QuadJet40_IsoPFTau40_v11")  prescale = triggerPrescaleList[i].second ;}
    }
  if (QuadJet40_IsoPFTau40_v12==true)
   { 
     for(unsigned int i=0; i< triggerPrescaleList.size(); i++)
     {if( triggerPrescaleList[i].first == "HLT_QuadJet40_IsoPFTau40_v12")  prescale = triggerPrescaleList[i].second ;}
    }
  
   
  //if (prescale !=1) std::cout << "QuadJet40_IsoPFTau40 is prescaled, run " <<runNumber << " " << prescale << std::endl;
  
  return prescale;
   
}

float SemiLeptonicTauSelection::QuadJet45IsoPFTau45Prescale()
{                                
 
   float prescale = 0;
   bool  QuadJet45_IsoPFTau45_v1 = false;
   bool  QuadJet45_IsoPFTau45_v2 = false;
   bool  QuadJet45_IsoPFTau45_v3 = false;
   bool  QuadJet45_IsoPFTau45_v4 = false;
   bool  QuadJet45_IsoPFTau45_v5 = false;
   bool  QuadJet45_IsoPFTau45_v6 = false;
   bool  QuadJet45_IsoPFTau45_v7 = false;
   bool  QuadJet45_IsoPFTau45_v8 = false;
   bool  QuadJet45_IsoPFTau45_v9 = false;
   bool  QuadJet45_IsoPFTau45_v10 = false;
   bool  QuadJet45_IsoPFTau45_v11 = false;
   bool  QuadJet45_IsoPFTau45_v12 = false;
   
   for(unsigned int i=0; i< triggerList.size(); i++)
   {  
     if (triggerList[i].first == "HLT_QuadJet45_IsoPFTau45_v1" ) QuadJet45_IsoPFTau45_v1 =true;
     if (triggerList[i].first == "HLT_QuadJet45_IsoPFTau45_v2" ) QuadJet45_IsoPFTau45_v2 =true;
     if (triggerList[i].first == "HLT_QuadJet45_IsoPFTau45_v3" ) QuadJet45_IsoPFTau45_v3 =true;
     if (triggerList[i].first == "HLT_QuadJet45_IsoPFTau45_v4" ) QuadJet45_IsoPFTau45_v4 =true;
     if (triggerList[i].first == "HLT_QuadJet45_IsoPFTau45_v5" ) QuadJet45_IsoPFTau45_v5 =true;
     if (triggerList[i].first == "HLT_QuadJet45_IsoPFTau45_v6" ) QuadJet45_IsoPFTau45_v6 =true; 
     if (triggerList[i].first == "HLT_QuadJet45_IsoPFTau45_v7" ) QuadJet45_IsoPFTau45_v7 =true;     
     if (triggerList[i].first == "HLT_QuadJet45_IsoPFTau45_v8" ) QuadJet45_IsoPFTau45_v8 =true;     
     if (triggerList[i].first == "HLT_QuadJet45_IsoPFTau45_v9" ) QuadJet45_IsoPFTau45_v9 =true;     
     if (triggerList[i].first == "HLT_QuadJet45_IsoPFTau45_v10" ) QuadJet45_IsoPFTau45_v10 =true;     
     if (triggerList[i].first == "HLT_QuadJet45_IsoPFTau45_v11" ) QuadJet45_IsoPFTau45_v11 =true;     
     if (triggerList[i].first == "HLT_QuadJet45_IsoPFTau45_v12" ) QuadJet45_IsoPFTau45_v12 =true;     
 
  }
   
   
   if (QuadJet45_IsoPFTau45_v1==true)
   { 
     for(unsigned int i=0; i< triggerPrescaleList.size(); i++)
     {if( triggerPrescaleList[i].first == "HLT_QuadJet45_IsoPFTau45_v1")  prescale = triggerPrescaleList[i].second ;}
    }
   if (QuadJet45_IsoPFTau45_v2==true)
   { 
     for(unsigned int i=0; i< triggerPrescaleList.size(); i++)
     {if( triggerPrescaleList[i].first == "HLT_QuadJet45_IsoPFTau45_v2")  prescale = triggerPrescaleList[i].second ;}
    }
   if (QuadJet45_IsoPFTau45_v3==true)
   { 
     for(unsigned int i=0; i< triggerPrescaleList.size(); i++)
     {if( triggerPrescaleList[i].first == "HLT_QuadJet45_IsoPFTau45_v3")  prescale = triggerPrescaleList[i].second ;}
    }
   if (QuadJet45_IsoPFTau45_v4==true)
   { 
     for(unsigned int i=0; i< triggerPrescaleList.size(); i++)
     {if( triggerPrescaleList[i].first == "HLT_QuadJet45_IsoPFTau45_v4")  prescale = triggerPrescaleList[i].second ;}
    }
   if (QuadJet45_IsoPFTau45_v5==true)
   { 
     for(unsigned int i=0; i< triggerPrescaleList.size(); i++)
     {if( triggerPrescaleList[i].first == "HLT_QuadJet45_IsoPFTau45_v5")  prescale = triggerPrescaleList[i].second ;}
    }
   if (QuadJet45_IsoPFTau45_v6==true)
   { 
     for(unsigned int i=0; i< triggerPrescaleList.size(); i++)
     {if( triggerPrescaleList[i].first == "HLT_QuadJet45_IsoPFTau45_v6")  prescale = triggerPrescaleList[i].second ;}
    }
   if (QuadJet45_IsoPFTau45_v7==true)
   { 
     for(unsigned int i=0; i< triggerPrescaleList.size(); i++)
     {if( triggerPrescaleList[i].first == "HLT_QuadJet45_IsoPFTau45_v7")  prescale = triggerPrescaleList[i].second ;}
    }
   if (QuadJet45_IsoPFTau45_v8==true)
   { 
     for(unsigned int i=0; i< triggerPrescaleList.size(); i++)
     {if( triggerPrescaleList[i].first == "HLT_QuadJet45_IsoPFTau45_v8")  prescale = triggerPrescaleList[i].second ;}
    }
   if (QuadJet45_IsoPFTau45_v9==true)
   { 
     for(unsigned int i=0; i< triggerPrescaleList.size(); i++)
     {if( triggerPrescaleList[i].first == "HLT_QuadJet45_IsoPFTau45_v9")  prescale = triggerPrescaleList[i].second ;}
    }
  if (QuadJet45_IsoPFTau45_v10==true)
   { 
     for(unsigned int i=0; i< triggerPrescaleList.size(); i++)
     {if( triggerPrescaleList[i].first == "HLT_QuadJet45_IsoPFTau45_v10")  prescale = triggerPrescaleList[i].second ;}
    }
  if (QuadJet45_IsoPFTau45_v11==true)
   { 
     for(unsigned int i=0; i< triggerPrescaleList.size(); i++)
     {if( triggerPrescaleList[i].first == "HLT_QuadJet45_IsoPFTau45_v11")  prescale = triggerPrescaleList[i].second ;}
    }
  if (QuadJet45_IsoPFTau45_v12==true)
   { 
     for(unsigned int i=0; i< triggerPrescaleList.size(); i++)
     {if( triggerPrescaleList[i].first == "HLT_QuadJet45_IsoPFTau45_v12")  prescale = triggerPrescaleList[i].second ;}
    }
  
   
  //if (prescale !=1) std::cout << "QuadJet45_IsoPFTau45 is prescaled " <<runNumber << " " << prescale << std::endl;
  
  return prescale;
   
}

float SemiLeptonicTauSelection::hltFilter(std::string str)
{                                
   float status = -999;
     
   for (unsigned int i=0; i< triggerPrescaleList.size(); i++){
   if ( triggerPrescaleList[i].first == str)  status = triggerPrescaleList[i].second ;}
   return status;  
}

float SemiLeptonicTauSelection::Trigger_QuadJet40_IsoPFTau40(){


  //vector<NTMuon> muon_cand      = GetSelectedMuonsNoIso();
  //vector<NTElectron> elec_cand  = GetSelectedElectronsNoIso();
  vector<NTTau> tau_cand        = GetSelectedTaus(45,2.5,false,1.0,0,false);
  vector<NTJet> jet_cand        = GetSelectedJets(tau_cand);

  float weight = 1;    
 
    //std::cout << "====" <<std::endl;
  if(jet_cand.size()>=3 && tau_cand.size()>=1){
      
    for (int i =0; i<3; i++){
      //if(jet_cand[i].p4.Pt()>=40){
      weight *= GetQuadJet40IsoPFTau40_HISTO_JET(jet_cand[i].p4.Pt());
      //std::cout <<"jet " << jet_cand[i].p4.Eta()<<" " << jet_cand[i].p4.Phi()<<" " << jet_cand[i].p4.Pt()<< std::endl;
      //}
    }
    //if(tau_cand[0].p4.Pt()>=45){
      weight *=GetQuadJet40IsoPFTau40_HISTO_TAU(tau_cand[0].p4.Pt());
      //std::cout <<"tau " << tau_cand[0].p4.Eta()<<" " << tau_cand[0].p4.Phi()<<" " << tau_cand[0].p4.Pt()<<std::endl;
    //}
    
  }
  return weight;
  
}

float SemiLeptonicTauSelection::Trigger_QuadJet40_IsoPFTau40_ERROR(){


  
  vector<NTMuon> muon_cand      = GetMuons();
  vector<NTElectron> elec_cand  = GetElectrons();
  // vector<NTTau> tau_cand        = GetSelectedTaus(20,2.5,false,1.,0.,false);
  //vector<NTJet> jet_cand        = GetSelectedJets(20,2.5,false,1.,false,0.);
  vector<NTTau> tau_cand        = GetSelectedTaus(45,2.5,false,1.,0,true);
  vector<NTJet> jet_cand        = GetSelectedJets(tau_cand,false,1.,false,0);
 
  float weight = 1;    
 
    
  if(jet_cand.size()>=3 && tau_cand.size()>=1){
      
    for (int i =0; i<3; i++){
      if(jet_cand[i].p4.Pt()>=45){
      weight *= GetQuadJet40IsoPFTau40_HISTO_JET_ERROR(jet_cand[i].p4.Pt());
      }
    }
    //if(tau_cand[0].p4.Pt()>=45){
      weight *=GetQuadJet40IsoPFTau40_HISTO_TAU(tau_cand[0].p4.Pt());
      
    //}
    
  }
  return weight;
  
}


float SemiLeptonicTauSelection::getWeightTauLeg(int triggerMenu)
{
  float weight =1;
  if (triggerMenu==40) weight = GetQuadJet40IsoPFTau40_HISTO_TAU(tausAna[0].p4.Pt());
  else if (triggerMenu==45) weight = GetQuadJet45IsoPFTau45_HISTO_TAU(tausAna[0].p4.Pt());
  return weight;
}

float SemiLeptonicTauSelection::getWeightJetLegs(int triggerMenu)
{
  float weight =1;
  if (triggerMenu==40)
  {  
    for (int i =0; i<3; i++) weight *= GetQuadJet40IsoPFTau40_HISTO_JET(jetsAna[i].p4.Pt());  
    } 
  else if (triggerMenu==45)
  { 
    for (int i =0; i<3; i++) weight *= GetQuadJet45IsoPFTau45_HISTO_JET(jetsAna[i].p4.Pt());  
    }
  
  return weight;
}

float SemiLeptonicTauSelection::getWeightTauLegTMVA(int triggerMenu)
{
  float weight =1;
  
  vector<NTMuon> muon_cand      = GetSelectedMuonsLooseIDLooseIso();
  vector<NTElectron> elec_cand  = GetSelectedElectronsLooseIDLooseIso();
  
  vector<NTTau> tau_cand = GetSelectedTaus(muon_cand, elec_cand, false,1,1,true);
  //vector<NTJet> jet_cand = GetSelectedJets(muon_cand, elec_cand, tau_cand);   
  
  if(tau_cand.size()==1)
  {
  if (triggerMenu==40) weight = GetQuadJet40IsoPFTau40_HISTO_TAU(tau_cand[0].p4.Pt());
  else if (triggerMenu==45) weight = GetQuadJet45IsoPFTau45_HISTO_TAU(tau_cand[0].p4.Pt());
  }
  return weight;
}

float SemiLeptonicTauSelection::getWeightJetLegsTMVA(int triggerMenu)
{
  float weight =1;
  
  vector<NTMuon> muon_cand      = GetSelectedMuonsLooseIDLooseIso();
  vector<NTElectron> elec_cand  = GetSelectedElectronsLooseIDLooseIso();
  
  vector<NTTau> tau_cand = GetSelectedTaus(muon_cand, elec_cand, false,1,1,true);
  vector<NTJet> jet_cand = GetSelectedJets(muon_cand, elec_cand, tau_cand);   
  
  if(jet_cand.size()>=3)
  {
  if (triggerMenu==40)
  {  
    for (int i =0; i<3; i++) weight *= GetQuadJet40IsoPFTau40_HISTO_JET(jet_cand[i].p4.Pt());  
    } 
  else if (triggerMenu==45)
  { 
    for (int i =0; i<3; i++) weight *= GetQuadJet45IsoPFTau45_HISTO_JET(jet_cand[i].p4.Pt());  
    }
  }
  
  return weight;
}

float SemiLeptonicTauSelection::Trigger_QuadJet45_IsoPFTau45(){


  //vector<NTMuon> muon_cand      = GetSelectedMuonsNoIso();
  //vector<NTElectron> elec_cand  = GetSelectedElectronsNoIso();
  vector<NTTau> tau_cand        = GetSelectedTaus(45,2.5,false,1.0,0,false);
  //vector<NTJet> jet_cand        = GetSelectedJets(20,2.5,false,1.,false,0.);
  vector<NTJet> jet_cand        = GetSelectedJets(tau_cand);
  
  float weight = 1;    
 
    
  if(jet_cand.size()>=3 && tau_cand.size()>=1){
      
    for (int i =0; i<3; i++){
      //if(jet_cand[i].p4.Pt()>=40){
        weight *= GetQuadJet45IsoPFTau45_HISTO_JET(jet_cand[i].p4.Pt());
      //}
    }
    //if(tau_cand[0].p4.Pt()>=45){
      weight *=GetQuadJet45IsoPFTau45_HISTO_TAU(tau_cand[0].p4.Pt());
    //}
    
  }
  return weight;
  
}

float SemiLeptonicTauSelection::Trigger_QuadJet45_IsoPFTau45_ERROR(){

  vector<NTMuon> muon_cand      = GetMuons();
  vector<NTElectron> elec_cand  = GetElectrons();
  vector<NTTau> tau_cand        = GetSelectedTaus(45,2.5,false,1.,1.,true);
  //vector<NTJet> jet_cand        = GetSelectedJets(20,2.5,false,1.,false,0.);
  //vector<NTJet> jet_cand        = GetSelectedJets(muon_cand, elec_cand, tau_cand,false,1.,false,0.);
   vector<NTJet> jet_cand        = GetSelectedJets(tau_cand,false,1.,false,0.);

   
  float weight = 1;    
 
    
  if(jet_cand.size()>=3 && tau_cand.size()>=1){
      
    for (int i =0; i<3; i++){
      if(jet_cand[i].p4.Pt()>=45){
        weight *= GetQuadJet45IsoPFTau45_HISTO_JET_ERROR(jet_cand[i].p4.Pt());
      }
    }
    //if(tau_cand[0].p4.Pt()>=45){
      weight *=GetQuadJet45IsoPFTau45_HISTO_TAU(tau_cand[0].p4.Pt());
    //}
    
  }
  return weight;
  
}

/*
float SemiLeptonicTauSelection::Trigger_QuadJet40_IsoPFTau40(){


 // vector<NTTau> tau_cand        = GetSelectedTaus(45,2.5,false,1.0,1,false);
  //vector<NTJet> jet_cand        = GetSelectedJets(tau_cand);

  float weight = 1;    
// std::cout <<"-----" << std::endl;
 
     if(jetsAna.size()>=3 && tausAna.size()>=1){
      
    for (int i =0; i<3; i++){
      //std::cout << "jetsAna[i].p4.Pt() " << jetsAna[i].p4.Pt() <<std::endl;
      
      weight *= GetQuadJet40IsoPFTau40_HISTO_JET(jetsAna[i].p4.Pt());
      
    }
    
      weight *=GetQuadJet40IsoPFTau40_HISTO_TAU(tausAna[0].p4.Pt());
   
    
  }
  return weight;
  
}

float SemiLeptonicTauSelection::Trigger_QuadJet40_IsoPFTau40_ERROR(){


  
//  vector<NTTau> tau_cand        = GetSelectedTaus(45,2.5,false,1.,1,true);
//  vector<NTJet> jet_cand        = GetSelectedJets(tau_cand,false,1.,false,0);
 
  float weight = 1;    
 
    
  if(jetsAna.size()>=3 && tausAna.size()>=1){
      
    for (int i =0; i<3; i++){
      if(jetsAna[i].p4.Pt()>=45){
      weight *= GetQuadJet40IsoPFTau40_HISTO_JET_ERROR(jetsAna[i].p4.Pt());
      }
    }
    
    weight *=GetQuadJet40IsoPFTau40_HISTO_TAU(tausAna[0].p4.Pt());
      
    
  }
  return weight;
  
}

float SemiLeptonicTauSelection::Trigger_QuadJet45_IsoPFTau45(){


  //vector<NTMuon> muon_cand      = GetSelectedMuonsNoIso();
  //vector<NTElectron> elec_cand  = GetSelectedElectronsNoIso();
  vector<NTTau> tau_cand        = GetSelectedTaus(45,2.5,false,1.0,1,false);
  //vector<NTJet> jet_cand        = GetSelectedJets(20,2.5,false,1.,false,0.);
  vector<NTJet> jet_cand        = GetSelectedJets(tau_cand);
  
  float weight = 1;    
 
    
  if(jetsAna.size()>=3 && tausAna.size()>=1){
      
    for (int i =0; i<3; i++){
      //if(jet_cand[i].p4.Pt()>=40){
        weight *= GetQuadJet45IsoPFTau45_HISTO_JET(jetsAna[i].p4.Pt());
      //}
    }
    //if(tau_cand[0].p4.Pt()>=45){
      weight *=GetQuadJet45IsoPFTau45_HISTO_TAU(tausAna[0].p4.Pt());
    //}
    
  }
  return weight;
  
}

float SemiLeptonicTauSelection::Trigger_QuadJet45_IsoPFTau45_ERROR(){

  vector<NTMuon> muon_cand      = GetMuons();
  vector<NTElectron> elec_cand  = GetElectrons();
  vector<NTTau> tau_cand        = GetSelectedTaus(45,2.5,false,1.,1.,true);
  //vector<NTJet> jet_cand        = GetSelectedJets(20,2.5,false,1.,false,0.);
  //vector<NTJet> jet_cand        = GetSelectedJets(muon_cand, elec_cand, tau_cand,false,1.,false,0.);
   vector<NTJet> jet_cand        = GetSelectedJets(tau_cand,false,1.,false,0.);

   
  float weight = 1;    
 
    
  if(jetsAna.size()>=3 && tausAna.size()>=1){
      
    for (int i =0; i<3; i++){
      if(jetsAna[i].p4.Pt()>=45){
        weight *= GetQuadJet45IsoPFTau45_HISTO_JET_ERROR(jetsAna[i].p4.Pt());
      }
    }
    //if(tau_cand[0].p4.Pt()>=45){
      weight *=GetQuadJet45IsoPFTau45_HISTO_TAU(tausAna[0].p4.Pt());
    //}
    
  }
  return weight;
  
}
*/




float SemiLeptonicTauSelection::GetQuadJet40IsoPFTau40_HISTO_JET(float jet_pT){

  float weight = 1;
  
  if(jet_pT>=20 && jet_pT<30) weight =Jet40_Leg->GetBinContent(2); 
  if(jet_pT>=30 && jet_pT<40) weight =Jet40_Leg->GetBinContent(3); 
  if(jet_pT>=40 && jet_pT<50) weight =Jet40_Leg->GetBinContent(4); 
  if(jet_pT>=50 && jet_pT<60) weight =Jet40_Leg->GetBinContent(5); 
  if(jet_pT>=60 && jet_pT<70) weight =Jet40_Leg->GetBinContent(6); 
  if(jet_pT>=70 && jet_pT<80) weight =Jet40_Leg->GetBinContent(7); 
  if(jet_pT>=80 && jet_pT<90) weight =Jet40_Leg->GetBinContent(8); 
  if(jet_pT>=90 && jet_pT<100) weight =Jet40_Leg->GetBinContent(9); 
  if(jet_pT>=100 && jet_pT<110) weight =Jet40_Leg->GetBinContent(10); 
  if(jet_pT>=110 && jet_pT<120) weight =Jet40_Leg->GetBinContent(11); 
  if(jet_pT>=120 && jet_pT<130) weight =Jet40_Leg->GetBinContent(12); 
  if(jet_pT>=130 && jet_pT<140) weight =Jet40_Leg->GetBinContent(13); 
  if(jet_pT>=140 && jet_pT<150) weight =Jet40_Leg->GetBinContent(14); 
  if(jet_pT>=150 && jet_pT<160) weight =Jet40_Leg->GetBinContent(15); 
  if(jet_pT>=160 && jet_pT<170) weight =Jet40_Leg->GetBinContent(16); 
  if(jet_pT>=170 && jet_pT<180) weight =Jet40_Leg->GetBinContent(17); 
  if(jet_pT>=180 ) weight =Jet40_Leg->GetBinContent(18); 
  
  return weight;

}

float SemiLeptonicTauSelection::GetQuadJet45IsoPFTau45_HISTO_JET(float jet_Pt ){

  float weight = 1;
  
  if(jet_Pt>=20 && jet_Pt<30) weight =Jet45_Leg->GetBinContent(2); 
  if(jet_Pt>=30 && jet_Pt<40) weight =Jet45_Leg->GetBinContent(3); 
  if(jet_Pt>=40 && jet_Pt<50) weight =Jet45_Leg->GetBinContent(4); 
  if(jet_Pt>=50 && jet_Pt<60) weight =Jet45_Leg->GetBinContent(5); 
  if(jet_Pt>=60 && jet_Pt<70) weight =Jet45_Leg->GetBinContent(6); 
  if(jet_Pt>=70 && jet_Pt<80) weight =Jet45_Leg->GetBinContent(7); 
  if(jet_Pt>=80 && jet_Pt<90) weight =Jet45_Leg->GetBinContent(8); 
  if(jet_Pt>=90 && jet_Pt<100) weight =Jet45_Leg->GetBinContent(9); 
  if(jet_Pt>=100 && jet_Pt<110) weight =Jet45_Leg->GetBinContent(10); 
  if(jet_Pt>=110 && jet_Pt<120) weight =Jet45_Leg->GetBinContent(11); 
  if(jet_Pt>=120 && jet_Pt<130) weight =Jet45_Leg->GetBinContent(12); 
  if(jet_Pt>=130 && jet_Pt<140) weight =Jet45_Leg->GetBinContent(13); 
  if(jet_Pt>=140 && jet_Pt<150) weight =Jet45_Leg->GetBinContent(14); 
  if(jet_Pt>=150 && jet_Pt<160) weight =Jet45_Leg->GetBinContent(15); 
  if(jet_Pt>=160 && jet_Pt<170) weight =Jet45_Leg->GetBinContent(16); 
  if(jet_Pt>=170 && jet_Pt<180) weight =Jet45_Leg->GetBinContent(17); 
  if(jet_Pt>=180 ) weight =Jet45_Leg->GetBinContent(18); 

  return weight;

}

float SemiLeptonicTauSelection::GetQuadJet40IsoPFTau40_HISTO_JET_ERROR(float jet_pT){

  float weight = 1;
  /*
  TFile* f1 = new  TFile("/opt/sbg/data/data1/cms/cferro/official8/CMSSW_4_2_5/src/MiniTreeAnalysis/NTupleAnalysis/macros/GOOD_Turnon/TriggerEffHisto_JetLeg40_simmetricBinSingleMuon_QuadJet40.root");
  TDirectory *dir1 = gDirectory;

  TH1F* Jet40_Leg= new TH1F("Jet40_Leg","Jet40_Leg",19,10,190);
  dir1->GetObject("jet4_eff",Jet40_Leg ); */
  if(jet_pT>=20 && jet_pT<30) weight =(Jet40_Leg->GetBinContent(2)+ (Jet40_Leg->GetBinError(2))/2); 
  if(jet_pT>=30 && jet_pT<40) weight =(Jet40_Leg->GetBinContent(3)+ (Jet40_Leg->GetBinError(3))/2); 
  if(jet_pT>=40 && jet_pT<50) weight =(Jet40_Leg->GetBinContent(4) + (Jet40_Leg->GetBinError(4))/2); 
  if(jet_pT>=50 && jet_pT<60) weight =(Jet40_Leg->GetBinContent(5) + (Jet40_Leg->GetBinError(5))/2); 
  if(jet_pT>=60 && jet_pT<70) weight =(Jet40_Leg->GetBinContent(6) + (Jet40_Leg->GetBinError(6))/2); 
  if(jet_pT>=70 && jet_pT<80) weight =(Jet40_Leg->GetBinContent(7) + (Jet40_Leg->GetBinError(7))/2); 
  if(jet_pT>=80 && jet_pT<90) weight =(Jet40_Leg->GetBinContent(8) + (Jet40_Leg->GetBinError(8))/2); 
  if(jet_pT>=90 && jet_pT<100) weight =(Jet40_Leg->GetBinContent(9) + (Jet40_Leg->GetBinError(9))/2); 
  if(jet_pT>=100 && jet_pT<110) weight =(Jet40_Leg->GetBinContent(10) + (Jet40_Leg->GetBinError(10))/2); 
  if(jet_pT>=110 && jet_pT<120) weight =(Jet40_Leg->GetBinContent(11) + (Jet40_Leg->GetBinError(11))/2); 
  if(jet_pT>=120 && jet_pT<130) weight =(Jet40_Leg->GetBinContent(12) + (Jet40_Leg->GetBinError(12))/2); 
  if(jet_pT>=130 && jet_pT<140) weight =(Jet40_Leg->GetBinContent(13) + (Jet40_Leg->GetBinError(13))/2); 
  if(jet_pT>=140 && jet_pT<150) weight =(Jet40_Leg->GetBinContent(14) + (Jet40_Leg->GetBinError(14))/2); 
  if(jet_pT>=150 && jet_pT<160) weight =(Jet40_Leg->GetBinContent(15) + (Jet40_Leg->GetBinError(15))/2); 
  if(jet_pT>=160 && jet_pT<170) weight =(Jet40_Leg->GetBinContent(16) + (Jet40_Leg->GetBinError(16))/2); 
  if(jet_pT>=170 && jet_pT<180) weight =(Jet40_Leg->GetBinContent(17) + (Jet40_Leg->GetBinError(17))/2); 
  if(jet_pT>=180 ) weight =(Jet40_Leg->GetBinContent(18) + (Jet40_Leg->GetBinError(18))/2); 
  
  return weight;

}

float SemiLeptonicTauSelection::GetQuadJet45IsoPFTau45_HISTO_JET_ERROR(float jet_Pt ){

  float weight = 1;
  /*
  TFile* f2 = new  TFile("/opt/sbg/data/data1/cms/cferro/official8/CMSSW_4_2_5/src/MiniTreeAnalysis/NTupleAnalysis/macros/GOOD_Turnon/TriggerEffHisto_JetLeg45_simmetricBinSingleMuon_Full.root");
  TDirectory *dir2 = gDirectory;

  TH1F* Jet45_Leg= new TH1F("Jet45_Leg","Jet45_Leg",19,10,190);
  dir2->GetObject("jet4_eff",Jet45_Leg ); 
 */
  if(jet_Pt>=20 && jet_Pt<30) weight =(Jet45_Leg->GetBinContent(2) + (Jet45_Leg->GetBinError(2))/2); 
  if(jet_Pt>=30 && jet_Pt<40) weight =(Jet45_Leg->GetBinContent(3) + (Jet45_Leg->GetBinError(3))/2); 
  if(jet_Pt>=40 && jet_Pt<50) weight =(Jet45_Leg->GetBinContent(4) + (Jet45_Leg->GetBinError(4))/2); 
  if(jet_Pt>=50 && jet_Pt<60) weight =(Jet45_Leg->GetBinContent(5) + (Jet45_Leg->GetBinError(4))/2); 
  if(jet_Pt>=60 && jet_Pt<70) weight =(Jet45_Leg->GetBinContent(6) + (Jet45_Leg->GetBinError(4))/2); 
  if(jet_Pt>=70 && jet_Pt<80) weight =(Jet45_Leg->GetBinContent(7) + (Jet45_Leg->GetBinError(4))/2); 
  if(jet_Pt>=80 && jet_Pt<90) weight =(Jet45_Leg->GetBinContent(8) + (Jet45_Leg->GetBinError(4))/2); 
  if(jet_Pt>=90 && jet_Pt<100) weight =(Jet45_Leg->GetBinContent(9) + (Jet45_Leg->GetBinError(4))/2); 
  if(jet_Pt>=100 && jet_Pt<110) weight =(Jet45_Leg->GetBinContent(10) + (Jet45_Leg->GetBinError(4))/2); 
  if(jet_Pt>=110 && jet_Pt<120) weight =(Jet45_Leg->GetBinContent(11) + (Jet45_Leg->GetBinError(4))/2); 
  if(jet_Pt>=120 && jet_Pt<130) weight =(Jet45_Leg->GetBinContent(12) + (Jet45_Leg->GetBinError(4))/2 ); 
  if(jet_Pt>=130 && jet_Pt<140) weight =(Jet45_Leg->GetBinContent(13) + (Jet45_Leg->GetBinError(4))/2); 
  if(jet_Pt>=140 && jet_Pt<150) weight =(Jet45_Leg->GetBinContent(14) + (Jet45_Leg->GetBinError(4))/2); 
  if(jet_Pt>=150 && jet_Pt<160) weight =(Jet45_Leg->GetBinContent(15) + (Jet45_Leg->GetBinError(4))/2); 
  if(jet_Pt>=160 && jet_Pt<170) weight =(Jet45_Leg->GetBinContent(16) + (Jet45_Leg->GetBinError(4))/2); 
  if(jet_Pt>=170 && jet_Pt<180) weight =(Jet45_Leg->GetBinContent(17) + (Jet45_Leg->GetBinError(4))/2); 
  if(jet_Pt>=180 ) weight =(Jet45_Leg->GetBinContent(18) + (Jet45_Leg->GetBinError(4))/2); 

  return weight;

}
/*
float SemiLeptonicTauSelection::GetQuadJet40IsoPFTau40_HISTO_TAU(float tau_pT){

  float weight = 1;
  
  if(tau_pT>=20 && tau_pT<25) weight =Tau40_Leg->GetBinContent(5); 
  if(tau_pT>=25 && tau_pT<30) weight =Tau40_Leg->GetBinContent(6); 
  if(tau_pT>=30 && tau_pT<35) weight =Tau40_Leg->GetBinContent(7); 
  if(tau_pT>=35 && tau_pT<40) weight =Tau40_Leg->GetBinContent(8); 
  if(tau_pT>=40 && tau_pT<45) weight =Tau40_Leg->GetBinContent(9); 
  if(tau_pT>=45 && tau_pT<50) weight =Tau40_Leg->GetBinContent(10); 
  if(tau_pT>=50 && tau_pT<60) weight =Tau40_Leg->GetBinContent(11); 
  if(tau_pT>=60 && tau_pT<70) weight =Tau40_Leg->GetBinContent(12); 
  if(tau_pT>=70 && tau_pT<80) weight =Tau40_Leg->GetBinContent(13); 
  if(tau_pT>=80 && tau_pT<90) weight =Tau40_Leg->GetBinContent(14); 
  if(tau_pT>=90 ) weight =Tau40_Leg->GetBinContent(15); 
  
  return weight;
}

float SemiLeptonicTauSelection::GetQuadJet40IsoPFTau40_HISTO_TAU_ERROR(float tau_pT){

  float weight = 1;
 
 
  if(tau_pT>=40 && tau_pT<45) weight =(Tau40_Leg->GetBinContent(9) +(Tau40_Leg->GetBinError(9))/2 ); 
  if(tau_pT>=45 && tau_pT<50) weight =(Tau40_Leg->GetBinContent(10) +(Tau40_Leg->GetBinError(10))/2 ); 
  if(tau_pT>=50 && tau_pT<60) weight =(Tau40_Leg->GetBinContent(11) +(Tau40_Leg->GetBinError(11))/2 ); 
  if(tau_pT>=60 && tau_pT<70) weight =(Tau40_Leg->GetBinContent(12) +(Tau40_Leg->GetBinError(12))/2 ); 
  if(tau_pT>=70 && tau_pT<80) weight =(Tau40_Leg->GetBinContent(13) +(Tau40_Leg->GetBinError(13))/2 ); 
  if(tau_pT>=80 && tau_pT<90) weight =(Tau40_Leg->GetBinContent(14) +(Tau40_Leg->GetBinError(14))/2 ); 
  if(tau_pT>=90 ) weight =(Tau40_Leg->GetBinContent(15) +(Tau40_Leg->GetBinError(15))/2 ); 
  
  return weight;
}

float SemiLeptonicTauSelection::GetQuadJet45IsoPFTau45_HISTO_TAU(float tau_pT){

  float weight = 1;
  
  if( tau_pT>=20 && tau_pT<25) weight =Tau45_Leg->GetBinContent(5);
  if( tau_pT>=25 && tau_pT<30) weight =Tau45_Leg->GetBinContent(6);
  if( tau_pT>=30 && tau_pT<35) weight =Tau45_Leg->GetBinContent(7); 
  if( tau_pT>=35 && tau_pT<40) weight =Tau45_Leg->GetBinContent(8); 
  if( tau_pT>=40 && tau_pT<45) weight =Tau45_Leg->GetBinContent(9); 
  if( tau_pT>=45 &&  tau_pT<50) weight =Tau45_Leg->GetBinContent(10); 
  if( tau_pT>=50 &&  tau_pT<60) weight =Tau45_Leg->GetBinContent(11); 
  if( tau_pT>=60 &&  tau_pT<70) weight =Tau45_Leg->GetBinContent(12); 
  if( tau_pT>=70 &&  tau_pT<80) weight =Tau45_Leg->GetBinContent(13); 
  if( tau_pT>=80 &&  tau_pT<90) weight =Tau45_Leg->GetBinContent(14); 
  if( tau_pT>=90 ) weight =Tau45_Leg->GetBinContent(15); 
 
 
  return weight;
  
}

float SemiLeptonicTauSelection::GetQuadJet45IsoPFTau45_HISTO_TAU_ERROR(float tau_pT){

  float weight = 1;
  
 
  if(tau_pT>=40 && tau_pT<45) weight =(Tau45_Leg->GetBinContent(9) +(Tau45_Leg->GetBinError(9))/2 ); 
  if(tau_pT>=45 && tau_pT<50) weight =(Tau45_Leg->GetBinContent(10) +(Tau45_Leg->GetBinError(10))/2 ); 
  if(tau_pT>=50 && tau_pT<60) weight =(Tau45_Leg->GetBinContent(11) +(Tau45_Leg->GetBinError(11))/2 ); 
  if(tau_pT>=60 && tau_pT<70) weight =(Tau45_Leg->GetBinContent(12) +(Tau45_Leg->GetBinError(12))/2 ); 
  if(tau_pT>=70 && tau_pT<80) weight =(Tau45_Leg->GetBinContent(13) +(Tau45_Leg->GetBinError(13))/2 ); 
  if(tau_pT>=80 && tau_pT<90) weight =(Tau45_Leg->GetBinContent(14) +(Tau45_Leg->GetBinError(14))/2 ); 
  if(tau_pT>=90 ) weight =(Tau45_Leg->GetBinContent(15) +(Tau45_Leg->GetBinError(15))/2 ); 
  
  return weight;
}
*/


float SemiLeptonicTauSelection::GetQuadJet40IsoPFTau40_HISTO_TAU(float tau_pT){

  float weight = 1;
  
  if(tau_pT>=20 && tau_pT<25) weight =Tau40_Leg->GetBinContent(5); 
  if(tau_pT>=25 && tau_pT<30) weight =Tau40_Leg->GetBinContent(6); 
  if(tau_pT>=30 && tau_pT<32.5) weight =Tau40_Leg->GetBinContent(7); 
  if(tau_pT>=32.5 && tau_pT<35) weight =Tau40_Leg->GetBinContent(8); 
  if(tau_pT>=35 && tau_pT<37.5) weight =Tau40_Leg->GetBinContent(9); 
  if(tau_pT>=37.5 && tau_pT<40) weight =Tau40_Leg->GetBinContent(10); 
  if(tau_pT>=40 && tau_pT<42.5) weight =Tau40_Leg->GetBinContent(11); 
  if(tau_pT>=42.5 && tau_pT<45) weight =Tau40_Leg->GetBinContent(12); 
  if(tau_pT>=45 && tau_pT<47.5) weight =Tau40_Leg->GetBinContent(13); 
  if(tau_pT>=47.5 && tau_pT<50) weight =Tau40_Leg->GetBinContent(14); 
  if(tau_pT>=50 && tau_pT<60) weight =Tau40_Leg->GetBinContent(15); 
  if(tau_pT>=60 && tau_pT<70) weight =Tau40_Leg->GetBinContent(16); 
  if(tau_pT>=70 && tau_pT<80) weight =Tau40_Leg->GetBinContent(17); 
  if(tau_pT>=80 && tau_pT<90) weight =Tau40_Leg->GetBinContent(18); 
  if(tau_pT>=90 ) weight =Tau40_Leg->GetBinContent(18); 
  
  return weight;
}

float SemiLeptonicTauSelection::GetQuadJet40IsoPFTau40_HISTO_TAU_ERROR(float tau_pT){

  float weight = 1;
 
 
  if(tau_pT>=40 && tau_pT<42.5) weight =(Tau40_Leg->GetBinContent(11) +(Tau40_Leg->GetBinError(11))/2 ); 
  if(tau_pT>=42.5 && tau_pT<45) weight =(Tau40_Leg->GetBinContent(12) +(Tau40_Leg->GetBinError(12))/2 ); 
  if(tau_pT>=45 && tau_pT<47.5) weight =(Tau40_Leg->GetBinContent(13) +(Tau40_Leg->GetBinError(13))/2 ); 
  if(tau_pT>=47.5 && tau_pT<50) weight =(Tau40_Leg->GetBinContent(14) +(Tau40_Leg->GetBinError(14))/2 ); 
  if(tau_pT>=50 && tau_pT<60) weight =(Tau40_Leg->GetBinContent(15) +(Tau40_Leg->GetBinError(15))/2 ); 
  if(tau_pT>=60 && tau_pT<70) weight =(Tau40_Leg->GetBinContent(16) +(Tau40_Leg->GetBinError(16))/2 ); 
  if(tau_pT>=70 && tau_pT<80) weight =(Tau40_Leg->GetBinContent(17) +(Tau40_Leg->GetBinError(17))/2 ); 
  if(tau_pT>=80 && tau_pT<90) weight =(Tau40_Leg->GetBinContent(18) +(Tau40_Leg->GetBinError(18))/2 ); 
  if(tau_pT>=90 ) weight =(Tau40_Leg->GetBinContent(18) +(Tau40_Leg->GetBinError(18))/2 ); 
  
  return weight;
}

float SemiLeptonicTauSelection::GetQuadJet45IsoPFTau45_HISTO_TAU(float tau_pT){

  float weight = 1;
  
  if(tau_pT>=20 && tau_pT<25) weight =Tau40_Leg->GetBinContent(5); 
  if(tau_pT>=25 && tau_pT<30) weight =Tau40_Leg->GetBinContent(6); 
  if(tau_pT>=30 && tau_pT<32.5) weight =Tau40_Leg->GetBinContent(7); 
  if(tau_pT>=32.5 && tau_pT<35) weight =Tau40_Leg->GetBinContent(8); 
  if(tau_pT>=35 && tau_pT<37.5) weight =Tau40_Leg->GetBinContent(9); 
  if(tau_pT>=37.5 && tau_pT<40) weight =Tau40_Leg->GetBinContent(10); 
  if(tau_pT>=40 && tau_pT<42.5) weight =Tau40_Leg->GetBinContent(11); 
  if(tau_pT>=42.5 && tau_pT<45) weight =Tau40_Leg->GetBinContent(12); 
  if(tau_pT>=45 && tau_pT<47.5) weight =Tau40_Leg->GetBinContent(13); 
  if(tau_pT>=47.5 && tau_pT<50) weight =Tau40_Leg->GetBinContent(14); 
  if(tau_pT>=50 && tau_pT<60) weight =Tau40_Leg->GetBinContent(15); 
  if(tau_pT>=60 && tau_pT<70) weight =Tau40_Leg->GetBinContent(16); 
  if(tau_pT>=70 && tau_pT<80) weight =Tau40_Leg->GetBinContent(17); 
  if(tau_pT>=80 && tau_pT<90) weight =Tau40_Leg->GetBinContent(18); 
  if(tau_pT>=90 ) weight =Tau40_Leg->GetBinContent(18); 
 
  return weight;
  
}

float SemiLeptonicTauSelection::GetQuadJet45IsoPFTau45_HISTO_TAU_ERROR(float tau_pT){

  float weight = 1;
  
  if(tau_pT>=40 && tau_pT<42.5) weight =(Tau40_Leg->GetBinContent(11) +(Tau40_Leg->GetBinError(11))/2 ); 
  if(tau_pT>=42.5 && tau_pT<45) weight =(Tau40_Leg->GetBinContent(12) +(Tau40_Leg->GetBinError(12))/2 ); 
  if(tau_pT>=45 && tau_pT<47.5) weight =(Tau40_Leg->GetBinContent(13) +(Tau40_Leg->GetBinError(13))/2 ); 
  if(tau_pT>=47.5 && tau_pT<50) weight =(Tau40_Leg->GetBinContent(14) +(Tau40_Leg->GetBinError(14))/2 ); 
  if(tau_pT>=50 && tau_pT<60) weight =(Tau40_Leg->GetBinContent(15) +(Tau40_Leg->GetBinError(15))/2 ); 
  if(tau_pT>=60 && tau_pT<70) weight =(Tau40_Leg->GetBinContent(16) +(Tau40_Leg->GetBinError(16))/2 ); 
  if(tau_pT>=70 && tau_pT<80) weight =(Tau40_Leg->GetBinContent(17) +(Tau40_Leg->GetBinError(17))/2 ); 
  if(tau_pT>=80 && tau_pT<90) weight =(Tau40_Leg->GetBinContent(18) +(Tau40_Leg->GetBinError(18))/2 ); 
  if(tau_pT>=90 ) weight =(Tau40_Leg->GetBinContent(18) +(Tau40_Leg->GetBinError(18))/2 ); 
 
  return weight;
}

int SemiLeptonicTauSelection::GetbtagAlgo () const
{
  return btagAlgo_;
}

float SemiLeptonicTauSelection::GetbtagDiscriCut () const
{
  return btagDiscriCut_;
}

int SemiLeptonicTauSelection::GetNofBtagJetsCut () const
{
  return NofBtagJets_;
}
