#include "../interface/TMVA_Tree.h"

                
TMVA_Tree::TMVA_Tree(){}


TMVA_Tree::~TMVA_Tree(){}


void TMVA_Tree::CreateHistos(TString inputRootFile){


  file = new TFile(inputRootFile.Data(),"RECREATE");
  T = new TTree("Ttree","Ttree");
    
  T->Branch("weight",     &weight_,     "weight/F");
  T->Branch("weight_BTAG",     &weight_BTAG_,     "weight_BTAG/F");
  T->Branch("aplanarity", &aplanarity_, "aplanarity/F");
  T->Branch("circularity",&circularity_,"circularity/F");
  T->Branch("sphericity", &sphericity_, "sphericity/F");
  T->Branch("Mt",         &Mt_,         "Mt/F");
  T->Branch("Ht",         &Ht_,         "Ht/F");
  T->Branch("Ht3",        &Ht3_,        "Ht3/F");
  T->Branch("chargeEta",  &chargeEta_,  "chargeEta/F");
  T->Branch("Met",        &Met_,        "Met/F"); 
  T->Branch("MtauJet",    &MTauJet_,    "MTauJet/F"); 
  T->Branch("M3",         &M3_,         "M3/F"); 
  T->Branch("M2",         &M2_,         "M2/F"); 
  T->Branch("chi2",       &chi2_,       "chi2/F"); 
  T->Branch("C",          &C_,          "C/F"); 
  T->Branch("D",          &D_,          "D/F"); 
  T->Branch("DeltaPhiTauMet", &DeltaPhiTauMet_, "DeltaPhiTauMet/F"); 
  T->Branch("NNOutput",       &NNOutput_,        "NNOutput/F"); 
  T->Branch("TauPt",          &TauPt_,           "TauPt/F");
  T->Branch("TauEta",         &TauEta_,          "TauEta/F");
  T->Branch("jetMultiplicity",&jetMultiplicity_, "jetMultiplicity/I");
  T->Branch("jetPt",          &jetPt_,           "jetPt/F");
  T->Branch("jetEta",         &jetEta_,          "jetEta/F");
  T->Branch("kinFitChi2",     &kinFitChi2_,      "kinFitChi2/F");
  T->Branch("kinFitTopMass",  &kinFitTopMass_,   "kinFitTopMass/F");
 
  reader = new TMVA::Reader( "" ); 
  reader->AddVariable( "aplanarity", &aplanarity_);
  reader->AddVariable( "chargeEta" , &chargeEta_ );
  reader->AddVariable( "Met",        &Met_);
  reader->AddVariable( "MtauJet",    &MTauJet_ ); 
  reader->AddVariable( "Ht" ,        &Ht_ ); 
  reader->AddVariable( "kinFitChi2", &kinFitChi2_ );
  reader->AddVariable( "DeltaPhiTauMet", &DeltaPhiTauMet_ );
  reader->AddVariable( "D", &D_);

  reader->BookMVA("MLP","/opt/sbg/data/data1/cms/lebihan/official8/CMSSW_4_2_5/src/MiniTreeAnalysis/NTupleAnalysis/macros/weights/TMVAClassification_MLP_KinFit_MET15_cris.weights.xml");
}


void TMVA_Tree::SaveHistos(TString inputRootFile){
 
  T->Print();
  T->Write();
  file->Close();
}  

////////:to run on MC
void TMVA_Tree::FillHistos(std::vector<NTJet> theJets, vector<TLorentzVector>jetObjTrig, NTMET Met, std::vector<NTMuon> selMuons, std::vector<NTElectron> selElectrons,
                           std::vector<NTTau> theTausLoose, std::vector<NTTau> theTausMedium, vector<TLorentzVector> tauObjTrig, bool PassTriggerQuadJet4045IsoPFTauSelection,  const float& weight, float& weight_BTAG){


        vector<NTJet> btagjets;
        vector<float> btagDiscri;
        int btagAlgo_ = 0 ;
        float btagDiscriCut_ = 3.3;
        
        int njets_45 = 0;
        
        for(unsigned int j=0;j<theJets.size();j++){
                
          if (theJets[j].p4.Pt()>45){

                
        float deltaRMinJetTrig =99.; 
            float deltaRJetTrig =99.;
            for(unsigned int k=0;k<jetObjTrig.size();k++)
              {
                deltaRJetTrig = theJets[j].p4.DeltaR(jetObjTrig[k]);
                if (deltaRJetTrig<deltaRMinJetTrig) deltaRMinJetTrig = deltaRJetTrig; }
            
            if(deltaRMinJetTrig<0.4) 
                  
                  njets_45++;
                 
          } 
                
                switch(btagAlgo_){
                        case 0 :
                                if(theJets[j].TCDiscri>=btagDiscriCut_){
                                         btagjets.push_back(theJets[j]);
                                         btagDiscri.push_back(theJets[j].TCDiscri);
                                }
                                break;  
                        case 1 :
                                if(theJets[j].SVDiscri>=btagDiscriCut_){
                                        btagjets.push_back(theJets[j]); 
                                        btagDiscri.push_back(theJets[j].SVDiscri);
                                }
                                break;  
                        case 2 :
                                if(theJets[j].SMDiscri>=btagDiscriCut_){
                                        btagjets.push_back(theJets[j]); 
                                        btagDiscri.push_back(theJets[j].SMDiscri);
                                }
                                break;  
                        case 4 :
                                //std::cout << "JP discri " << jet_cand[j].GetDiscri("jetProbabilityBJetTags") << std::endl;
                                if(theJets[j].GetDiscri("jetProbabilityBJetTags")>=btagDiscriCut_){
                                        btagjets.push_back(theJets[j]); 
                                        btagDiscri.push_back(theJets[j].GetDiscri("jetProbabilityBJetTags"));
                                }
                                break;          
                                
                        default:
                                cerr << "btagAlgo doesn't exist !"<<endl;
                                break;  
                }
                
        }
        
        

        //// da scommentare quando si gira sui dati!!!!!!!!
  if( theTausMedium.size()==1 && tauObjTrig.size()>=1 ){
    
    float deltaRMinTauTrig = 99.;
    float deltaRTauTrig = 99.;
    for(unsigned int j=0;j<tauObjTrig.size();j++)
      {
        deltaRTauTrig = theTausMedium[0].p4.DeltaR(tauObjTrig[j]);
        if(deltaRTauTrig<deltaRMinTauTrig) deltaRMinTauTrig = deltaRTauTrig; }
    
    if(deltaRMinTauTrig<0.4){
            
            
            if ( theJets.size() >= 4 && njets_45>=3 &&  
                 theTausMedium.size() == 1 && Met.p4.Et()>=20 &&  selMuons.size()==0 && selElectrons.size()==0 && btagjets.size()==0)
              {
                if(PassTriggerQuadJet4045IsoPFTauSelection==true){
                
                weight_ = weight;
                weight_BTAG_ = weight_BTAG;
                EventShapes evShape(theJets, theTausMedium);
                EventShapes evShapeM3(theJets);
                
                aplanarity_  = evShape.aplanarity();
                circularity_ = evShape.circularity();
                sphericity_  = evShape.sphericity();
                Ht_          = evShape.HT();
                Ht3_         = evShapeM3.HT3();
                MTauJet_     = evShape.sqrt_s();
                chi2_        = evShapeM3.chi2(theJets);
                Mt_          = sqrt (pow(theTausMedium[0].p4.Et() + Met.p4.Et(),2) - pow(theTausMedium[0].p4.Px() + Met.p4.Px(),2) - pow(theTausMedium[0].p4.Py() + Met.p4.Py(),2));
                chargeEta_   =(theTausMedium[0].Charge)*fabs((theTausMedium[0].p4.Eta()));
                Met_         = Met.p4.Et();      
                M3_          = evShape.M3chi2(theJets);
                M2_          = evShape.M2chi2(theJets);
                C_           = evShape.C();
                D_           = evShape.D();
                DeltaPhiTauMet_ = fabs(theTausMedium[0].p4.DeltaPhi(Met.p4));
                
                NNOutput_= reader->EvaluateMVA("MLP");
                TauPt_ = theTausMedium[0].p4.Et();
                TauEta_= theTausMedium[0].p4.Eta();
                jetMultiplicity_ = theJets.size();
                jetPt_ = theJets[0].p4.Pt();
                jetEta_= theJets[0].p4.Eta();
                
                //------------------------------------ kinFitter stuff
                
                std::vector<NTMET> mets;
                mets.push_back(Met);
                
                vector<NTLepton> lep;
                lep.push_back(dynamic_cast<NTLepton&> (theTausMedium.at(0)));
                
                std::string lepType = "tau";
                kinFitter.produce(theJets, lep,"tau", mets);
                
                kinFitChi2_ = kinFitter.GetFitChi2();
                
                TLorentzVector HadB = kinFitter.GetFitHadB();
                TLorentzVector HadP = kinFitter.GetFitHadP();
                TLorentzVector HadQ = kinFitter.GetFitHadQ();   
                TLorentzVector LepB = kinFitter.GetFitLepB();   
                TLorentzVector LepL = kinFitter.GetFitLepL();   
                TLorentzVector LepN = kinFitter.GetFitLepN();   
                
                TLorentzVector HadSide = HadB+HadP+HadQ;
                //std::cout << "M top had "<< HadSide.M() <<std::endl; 
                kinFitTopMass_ = HadSide.M();
                TLorentzVector LepSide = LepB+LepL+LepN;
                //std::cout << "M top Lep "<< LepSide.M() <<std::endl; 
                TLorentzVector WLepSide = LepL+LepN;
                //std::cout << "M W lep "<< WLepSide.M() <<std::endl; 
                TLorentzVector WHadSide = HadP+HadQ;
                //std::cout << "M W  Had"<< WHadSide.M() <<std::endl; 
                
                
                T->Fill();
                        }
                    }
          }
        }
}
void TMVA_Tree::Plots(){
  
  gStyle->SetPadRightMargin(0.13);
  gStyle->SetPadLeftMargin(0.13);
  gStyle->SetPadBottomMargin(0.13);
  gStyle->SetOptTitle(1);
  gStyle->SetOptStat(0);
  
  gStyle->SetOptDate(0);
  gStyle->SetStatColor(0);
  gStyle->SetTitleColor(1);
  gStyle->SetOptStat(0);
  gStyle->SetPalette(1);
  
  
  TCanvas *c1 = new TCanvas("c1", "c1",10,32,782,552);
  
  c1->SetFillColor(10);
  c1->SetFillStyle(4000);
  c1->SetBorderSize(2);
  
  //c1.Divide(2,2);
  c1->SetFillColor(0);
  c1->SetBorderMode(0);
  c1->SetFrameFillColor(10);
  
}

