#include "../interface/TagAndProbe.h"

                
TagAndProbe::TagAndProbe(){

     
}


TagAndProbe::~TagAndProbe(){}





void TagAndProbe::CreateHistos(string TypeSel, string datasetname){
  
  
  TH1F *h_pt, *h_pt_supEta15,*h_pt_infEta15,*h_eta, *h_phi, *h_njet, *h_invM,*h_invM_LS ;
  
  //double ptRange[] = {20, 22, 24, 26, 28, 30, 32, 34, 36, 40, 100};
  double ptRange[] = {20, 22, 24, 26, 28, 30, 32, 34, 36, 40, 45, 50, 65, 80, 100};
  if(TypeSel == "muons"){
    
    
    //*******************
    //for loose ID selection
    h_pt   = new TH1F( (datasetname+TypeSel+"_looseID_pt").c_str(),  "", 14, ptRange); 
    hlist_Mu_LooseID.push_back(h_pt);
    h_pt_supEta15   = new TH1F( (datasetname+TypeSel+"_looseID_pt_supEta15").c_str(),  "", 14, ptRange); 
    hlist_Mu_LooseID.push_back(h_pt_supEta15);
    h_pt_infEta15   = new TH1F( (datasetname+TypeSel+"_looseID_pt_infEta15").c_str(),  "", 14, ptRange); 
    hlist_Mu_LooseID.push_back(h_pt_infEta15);
    h_eta  = new TH1F( (datasetname+TypeSel+"_looseID_eta").c_str(), "", 40, -2.5 , 2.5); 
    hlist_Mu_LooseID.push_back(h_eta);
    h_phi  = new TH1F( (datasetname+TypeSel+"_looseID_phi").c_str(), "", 40, -3.14, 3.14); 
    hlist_Mu_LooseID.push_back(h_phi);
    h_njet = new TH1F( (datasetname+TypeSel+"_looseID_njet").c_str(),"", 7,  -0.5 , 6.5); 
    hlist_Mu_LooseID.push_back(h_njet);
    h_invM = new TH1F( (datasetname+TypeSel+"_looseID_invM").c_str(),"", 100,  0 , 200); 
    hlist_Mu_LooseID.push_back(h_invM);
    h_invM_LS = new TH1F( (datasetname+TypeSel+"_looseID_invM_LS").c_str(),"", 100,  0 , 200); 
    hlist_Mu_LooseID.push_back(h_invM_LS);
    
    //*******************
    //for loose Iso selection
    
    
    h_pt   = new TH1F( (datasetname+TypeSel+"_looseIso_pt").c_str(),  "", 14, ptRange); 
    hlist_Mu_LooseIso.push_back(h_pt);       
    h_pt_supEta15   = new TH1F( (datasetname+TypeSel+"_looseIso_pt_supEta15").c_str(),  "", 14, ptRange); 
    hlist_Mu_LooseIso.push_back(h_pt_supEta15);   
    h_pt_infEta15   = new TH1F( (datasetname+TypeSel+"_looseIso_pt_infEta15").c_str(),  "", 14, ptRange); 
    hlist_Mu_LooseIso.push_back(h_pt_infEta15);
    h_eta  = new TH1F( (datasetname+TypeSel+"_looseIso_eta").c_str(), "", 40, -2.5 , 2.5); 
    hlist_Mu_LooseIso.push_back(h_eta);
    h_phi  = new TH1F( (datasetname+TypeSel+"_looseIso_phi").c_str(), "", 40, -3.14, 3.14); 
    hlist_Mu_LooseIso.push_back(h_phi);
    h_njet = new TH1F( (datasetname+TypeSel+"_looseIso_njet").c_str(),"", 7,  -0.5 , 6.5); 
    hlist_Mu_LooseIso.push_back(h_njet);
    h_invM = new TH1F( (datasetname+TypeSel+"_looseIso_invM").c_str(),"", 100,  0 , 200); 
    hlist_Mu_LooseIso.push_back(h_invM);
    h_invM_LS = new TH1F( (datasetname+TypeSel+"_looseIso_invM_LS").c_str(),"", 100,  0 , 200); 
    hlist_Mu_LooseIso.push_back(h_invM_LS);
    
    
    
    
    //*******************
    //for tight selection
    h_pt   = new TH1F( (datasetname+TypeSel+"_Tight_pt").c_str(),  "", 14, ptRange); 
    hlist_Mu_Tight.push_back(h_pt);
    h_pt_supEta15   = new TH1F( (datasetname+TypeSel+"_Tight_pt_supEta15").c_str(),  "", 14, ptRange); 
    hlist_Mu_Tight.push_back(h_pt_supEta15);
    h_pt_infEta15   = new TH1F( (datasetname+TypeSel+"_Tight_pt_infEta15").c_str(),  "", 14, ptRange); 
    hlist_Mu_Tight.push_back(h_pt_infEta15);
    h_eta  = new TH1F( (datasetname+TypeSel+"_Tight_eta").c_str(), "", 40, -2.5 , 2.5); 
    hlist_Mu_Tight.push_back(h_eta);
    h_phi  = new TH1F( (datasetname+TypeSel+"_Tight_phi").c_str(), "", 40, -3.14, 3.14); 
    hlist_Mu_Tight.push_back(h_phi);
    h_njet = new TH1F( (datasetname+TypeSel+"_Tight_njet").c_str(),"", 7,  -0.5 , 6.5); 
    hlist_Mu_Tight.push_back(h_njet);
    h_invM = new TH1F( (datasetname+TypeSel+"_Tight_invM").c_str(),"", 100,  0 , 200); 
    hlist_Mu_Tight.push_back(h_invM);
    h_invM_LS = new TH1F( (datasetname+TypeSel+"_Tight_invM_LS").c_str(),"", 100,  0 , 200); 
    hlist_Mu_Tight.push_back(h_invM_LS);
    
    
    
  }
  
  if(TypeSel == "electrons"){
    
    //*******************
    //for loose ID selection
    
    h_pt   = new TH1F( (datasetname+TypeSel+"_looseID_pt").c_str(),  "", 14, ptRange); 
    hlist_El_LooseID.push_back(h_pt);
    h_pt_supEta15   = new TH1F( (datasetname+TypeSel+"_looseID_pt_supEta15").c_str(),  "", 14, ptRange); 
    hlist_El_LooseID.push_back(h_pt_supEta15); 
    h_pt_infEta15   = new TH1F( (datasetname+TypeSel+"_looseID_pt_infEta15").c_str(),  "", 14, ptRange); 
    hlist_El_LooseID.push_back(h_pt_infEta15);
    h_eta  = new TH1F( (datasetname+TypeSel+"_looseID_eta").c_str(), "", 40, -2.5 , 2.5); 
    hlist_El_LooseID.push_back(h_eta);
    h_phi  = new TH1F( (datasetname+TypeSel+"_looseID_phi").c_str(), "", 40, -3.14, 3.14); 
    hlist_El_LooseID.push_back(h_phi);
    h_njet = new TH1F( (datasetname+TypeSel+"_looseID_njet").c_str(),"", 7,  -0.5 , 6.5); 
    hlist_El_LooseID.push_back(h_njet);
    h_invM = new TH1F( (datasetname+TypeSel+"_looseID_invM").c_str(),"", 100,  0 , 200); 
    hlist_El_LooseID.push_back(h_invM);
    h_invM_LS = new TH1F( (datasetname+TypeSel+"_looseID_invM_LS").c_str(),"", 100,  0 , 200); 
    hlist_El_LooseID.push_back(h_invM_LS);
    
    
    
    
    //*******************
    //for loose Iso selection
    
    h_pt   = new TH1F( (datasetname+TypeSel+"_looseIso_pt").c_str(),  "", 14, ptRange); 
    hlist_El_LooseIso.push_back(h_pt);
    h_pt_supEta15   = new TH1F( (datasetname+TypeSel+"_looseIso_pt_supEta15").c_str(),  "", 14, ptRange); 
    hlist_El_LooseIso.push_back(h_pt_supEta15);
    h_pt_infEta15   = new TH1F( (datasetname+TypeSel+"_looseIso_pt_infEta15").c_str(),  "", 14, ptRange); 
    hlist_El_LooseIso.push_back(h_pt_infEta15);
    h_eta  = new TH1F( (datasetname+TypeSel+"_looseIso_eta").c_str(), "", 40, -2.5 , 2.5); 
    hlist_El_LooseIso.push_back(h_eta);
    h_phi  = new TH1F( (datasetname+TypeSel+"_looseIso_phi").c_str(), "", 40, -3.14, 3.14); 
    hlist_El_LooseIso.push_back(h_phi);
    h_njet = new TH1F( (datasetname+TypeSel+"_looseIso_njet").c_str(),"", 7,  -0.5 , 6.5); 
    hlist_El_LooseIso.push_back(h_njet);
    h_invM = new TH1F( (datasetname+TypeSel+"_looseIso_invM").c_str(),"", 100,  0 , 200); 
    hlist_El_LooseIso.push_back(h_invM);
    h_invM_LS = new TH1F( (datasetname+TypeSel+"_looseIso_invM_LS").c_str(),"", 100,  0 , 200); 
    hlist_El_LooseIso.push_back(h_invM_LS);
    
    
    
    
    //*******************
    //for tight selection
    h_pt   = new TH1F( (datasetname+TypeSel+"_Tight_pt").c_str(),  "", 14, ptRange); 
    hlist_El_Tight.push_back(h_pt);
    h_pt_supEta15   = new TH1F( (datasetname+TypeSel+"_Tight_pt_supEta15").c_str(),  "", 14, ptRange); 
    hlist_El_Tight.push_back(h_pt_supEta15);
    h_pt_infEta15   = new TH1F( (datasetname+TypeSel+"_Tight_pt_infEta15").c_str(),  "", 14, ptRange); 
    hlist_El_Tight.push_back(h_pt_infEta15);
    h_eta  = new TH1F( (datasetname+TypeSel+"_Tight_eta").c_str(), "", 40, -2.5 , 2.5); 
    hlist_El_Tight.push_back(h_eta);
    h_phi  = new TH1F( (datasetname+TypeSel+"_Tight_phi").c_str(), "", 40, -3.14, 3.14); 
    hlist_El_Tight.push_back(h_phi);
    h_njet = new TH1F( (datasetname+TypeSel+"_Tight_njet").c_str(),"", 7,  -0.5 , 6.5); 
    hlist_El_Tight.push_back(h_njet);
    h_invM = new TH1F( (datasetname+TypeSel+"_Tight_invM").c_str(),"", 100,  0 , 200); 
    hlist_El_Tight.push_back(h_invM);
    h_invM_LS = new TH1F( (datasetname+TypeSel+"_Tight_invM_LS").c_str(),"", 100,  0 , 200); 
    hlist_El_Tight.push_back(h_invM_LS);
    
    
    
  }
  
}


void TagAndProbe::ReadHistos(TString inputRootFile){
  // to read histograms already produced
  
  
  
}



void TagAndProbe::FillHistos(std::vector<NTJet> thejets, string TypeSel, string datasetname, double weight){
  
  int njet = thejets.size();
  
  //*******************************************
  //**********For muons ***********************
  //*******************************************
  
  int infMassCut = 76;
  int supMassCut = 106;
  
  
  //  int infMassCut = 81;
  //  int supMassCut = 111;
  
  //  int infMassCut = 71;
  //  int supMassCut = 101;
  
  
  if(TypeSel == "muons" ){
    
    
    // ask for a tag muon
    if(tightMuons.size() > 0){
      
      
      //*******************
      //for loose ID selection
      for( unsigned int  itmuon= 0; itmuon < tightMuons.size(); itmuon++){
        
        
        TLorentzVector dilepton_looseID, lepton_tight, lepton_loose;
        lepton_tight.SetPtEtaPhiM(tightMuons[itmuon].p4.Pt(), tightMuons[itmuon].p4.Eta(), tightMuons[itmuon].p4.Phi(), 0.105658367);
        for(unsigned int  imuon= 0; imuon < looseIDMuons.size(); imuon++){
          
          if( fabs(tightMuons[itmuon].p4.Pt() - looseIDMuons[imuon].p4.Pt()) > 0.0001 ){
            lepton_loose.SetPtEtaPhiM(looseIDMuons[imuon].p4.Pt(), looseIDMuons[imuon].p4.Eta(), looseIDMuons[imuon].p4.Phi(), 0.105658367);
            dilepton_looseID = lepton_loose + lepton_tight;
            
            double invM = dilepton_looseID.M();
            for(unsigned int i=0; i< hlist_Mu_LooseID.size(); i++){
              
              if(tightMuons[itmuon].Charge != looseIDMuons[imuon].Charge && (invM >infMassCut && invM < supMassCut)){
                if( datasetname+TypeSel+"_looseID_pt"      == hlist_Mu_LooseID[i]->GetName()) hlist_Mu_LooseID[i]->Fill(looseIDMuons[imuon].p4.Pt(), weight);  
                if( datasetname+TypeSel+"_looseID_pt_infEta15"      == hlist_Mu_LooseID[i]->GetName() && fabs(looseIDMuons[imuon].p4.Eta()) < 1.5) hlist_Mu_LooseID[i]->Fill(looseIDMuons[imuon].p4.Pt(), weight);  
                if( datasetname+TypeSel+"_looseID_pt_supEta15"      == hlist_Mu_LooseID[i]->GetName() && fabs(looseIDMuons[imuon].p4.Eta()) > 1.5) hlist_Mu_LooseID[i]->Fill(looseIDMuons[imuon].p4.Pt(), weight); 
                if( datasetname+TypeSel+"_looseID_eta"     == hlist_Mu_LooseID[i]->GetName()) hlist_Mu_LooseID[i]->Fill(looseIDMuons[imuon].p4.Eta(), weight);   
                if( datasetname+TypeSel+"_looseID_phi"     == hlist_Mu_LooseID[i]->GetName()) hlist_Mu_LooseID[i]->Fill(looseIDMuons[imuon].p4.Phi(), weight);   
                if( datasetname+TypeSel+"_looseID_njet"    == hlist_Mu_LooseID[i]->GetName()) hlist_Mu_LooseID[i]->Fill(njet, weight); 
              }
              if(tightMuons[itmuon].Charge != looseIDMuons[imuon].Charge) { 
                if( datasetname+TypeSel+"_looseID_invM"    == hlist_Mu_LooseID[i]->GetName()) hlist_Mu_LooseID[i]->Fill(invM, weight);
              }  
              else{ if( datasetname+TypeSel+"_looseID_invM_LS" == hlist_Mu_LooseID[i]->GetName()) hlist_Mu_LooseID[i]->Fill(invM, weight); }  
            }
          } 
        }
        
        
        //*******************
        //for loose Iso selection
        TLorentzVector dilepton_looseIso;
        for(unsigned int  imuon= 0; imuon < looseIsoMuons.size(); imuon++){
          
          if( fabs(tightMuons[itmuon].p4.Pt() - looseIsoMuons[imuon].p4.Pt()) > 0.0001 ){
            lepton_loose.SetPtEtaPhiM(looseIsoMuons[imuon].p4.Pt(), looseIsoMuons[imuon].p4.Eta(), looseIsoMuons[imuon].p4.Phi(), 0.105658367);
            dilepton_looseIso = lepton_loose + lepton_tight;
            
            double invM = dilepton_looseIso.M();
            for(unsigned int i=0; i< hlist_Mu_LooseIso.size(); i++){
              
              if(tightMuons[itmuon].Charge != looseIsoMuons[imuon].Charge && (invM >infMassCut && invM < supMassCut)){
                if( datasetname+TypeSel+"_looseIso_pt"      == hlist_Mu_LooseIso[i]->GetName()) hlist_Mu_LooseIso[i]->Fill(looseIsoMuons[imuon].p4.Pt(), weight); 
                if( datasetname+TypeSel+"_looseIso_pt_infEta15"      == hlist_Mu_LooseIso[i]->GetName()&& fabs(looseIsoMuons[imuon].p4.Eta()) < 1.5) hlist_Mu_LooseIso[i]->Fill(looseIsoMuons[imuon].p4.Pt(), weight); 
                if( datasetname+TypeSel+"_looseIso_pt_supEta15"      == hlist_Mu_LooseIso[i]->GetName()&& fabs(looseIsoMuons[imuon].p4.Eta()) > 1.5) hlist_Mu_LooseIso[i]->Fill(looseIsoMuons[imuon].p4.Pt(), weight); 
                if( datasetname+TypeSel+"_looseIso_eta"     == hlist_Mu_LooseIso[i]->GetName()) hlist_Mu_LooseIso[i]->Fill(looseIsoMuons[imuon].p4.Eta(), weight);   
                if( datasetname+TypeSel+"_looseIso_phi"     == hlist_Mu_LooseIso[i]->GetName()) hlist_Mu_LooseIso[i]->Fill(looseIsoMuons[imuon].p4.Phi(), weight);   
                if( datasetname+TypeSel+"_looseIso_njet"    == hlist_Mu_LooseIso[i]->GetName()) hlist_Mu_LooseIso[i]->Fill(njet, weight); 
              }
              if(tightMuons[itmuon].Charge != looseIsoMuons[imuon].Charge) { 
                if( datasetname+TypeSel+"_looseIso_invM"    == hlist_Mu_LooseIso[i]->GetName()) hlist_Mu_LooseIso[i]->Fill(invM, weight);
              }  
              else{ if( datasetname+TypeSel+"_looseIso_invM_LS" == hlist_Mu_LooseIso[i]->GetName()) hlist_Mu_LooseIso[i]->Fill(invM, weight); }  
            }
          } 
        } 
        
        
        
        //*******************
        //for tight selection 
        TLorentzVector dilepton_Tight, lepton_tight1, lepton_tight2;
        if(tightMuons.size() > 1 && (itmuon == 0 || itmuon == 1) ){
          
          lepton_tight1.SetPtEtaPhiM(tightMuons[0].p4.Pt(), tightMuons[0].p4.Eta(), tightMuons[0].p4.Phi(), 0.105658367);
          lepton_tight2.SetPtEtaPhiM(tightMuons[1].p4.Pt(), tightMuons[1].p4.Eta(), tightMuons[1].p4.Phi(), 0.105658367);
          dilepton_Tight = lepton_tight1 + lepton_tight2;
          double invM = dilepton_Tight.M(); 
          for(unsigned int i=0; i< hlist_Mu_Tight.size(); i++){
            if(tightMuons[0].Charge != tightMuons[1].Charge && (invM >infMassCut && invM < supMassCut)){
              if( datasetname+TypeSel+"_Tight_pt"          == hlist_Mu_Tight[i]->GetName()) hlist_Mu_Tight[i]->Fill(tightMuons[itmuon].p4.Pt(), weight);  
              if( datasetname+TypeSel+"_Tight_pt_infEta15" == hlist_Mu_Tight[i]->GetName()&& fabs(tightMuons[itmuon].p4.Eta()) < 1.5) hlist_Mu_Tight[i]->Fill(tightMuons[itmuon].p4.Pt(), weight);  
              if( datasetname+TypeSel+"_Tight_pt_supEta15" == hlist_Mu_Tight[i]->GetName()&& fabs(tightMuons[itmuon].p4.Eta()) > 1.5) hlist_Mu_Tight[i]->Fill(tightMuons[itmuon].p4.Pt(), weight); 
              if( datasetname+TypeSel+"_Tight_eta"         == hlist_Mu_Tight[i]->GetName()) hlist_Mu_Tight[i]->Fill(tightMuons[itmuon].p4.Eta(), weight);   
              if( datasetname+TypeSel+"_Tight_phi"         == hlist_Mu_Tight[i]->GetName()) hlist_Mu_Tight[i]->Fill(tightMuons[itmuon].p4.Phi(), weight);   
              if( datasetname+TypeSel+"_Tight_njet"        == hlist_Mu_Tight[i]->GetName()) hlist_Mu_Tight[i]->Fill(njet, weight);
            }
            if(tightMuons[0].Charge != tightMuons[1].Charge) { 
              if( datasetname+TypeSel+"_Tight_invM"    == hlist_Mu_Tight[i]->GetName()) hlist_Mu_Tight[i]->Fill(invM, weight);
            }  
            else{ if( datasetname+TypeSel+"_Tight_invM_LS" == hlist_Mu_Tight[i]->GetName()) hlist_Mu_Tight[i]->Fill(invM, weight); } 
          }
        }
      }
    }
  }
  
  
  
  //*******************************************
  //**********For electrons *******************
  //*******************************************
  
  if(TypeSel == "electrons" ){
    
    // ask for a tag Electron
    if(tightElectrons.size() > 0){
      
      
      //*******************
      //for loose ID selection
      for(unsigned int  itElectron= 0; itElectron < tightElectrons.size(); itElectron++){
        TLorentzVector dilepton_looseID, lepton_tight, lepton_loose;
        lepton_tight.SetPtEtaPhiM(tightElectrons[itElectron].p4.Pt(), tightElectrons[itElectron].p4.Eta(), tightElectrons[itElectron].p4.Phi(), 0);
        for(unsigned int  iElectron= 0; iElectron < looseIDElectrons.size(); iElectron++){
          
          if( fabs(tightElectrons[itElectron].p4.Pt() - looseIDElectrons[iElectron].p4.Pt()) > 0.0001 ){
            lepton_loose.SetPtEtaPhiM(looseIDElectrons[iElectron].p4.Pt(), looseIDElectrons[iElectron].p4.Eta(), looseIDElectrons[iElectron].p4.Phi(), 0);
            dilepton_looseID = lepton_loose + lepton_tight;
            
            double invM = dilepton_looseID.M();
            for(unsigned int i=0; i< hlist_El_LooseID.size(); i++){
              
              if(tightElectrons[itElectron].Charge != looseIDElectrons[iElectron].Charge && (invM >infMassCut && invM < supMassCut)){
                if( datasetname+TypeSel+"_looseID_pt"      == hlist_El_LooseID[i]->GetName()) hlist_El_LooseID[i]->Fill(looseIDElectrons[iElectron].p4.Pt(), weight);  
                if( datasetname+TypeSel+"_looseID_pt_infEta15"      == hlist_El_LooseID[i]->GetName()&& fabs(looseIDElectrons[iElectron].p4.Eta()) < 1.5) hlist_El_LooseID[i]->Fill(looseIDElectrons[iElectron].p4.Pt(), weight);  
                if( datasetname+TypeSel+"_looseID_pt_supEta15"      == hlist_El_LooseID[i]->GetName()&& fabs(looseIDElectrons[iElectron].p4.Eta()) > 1.5) hlist_El_LooseID[i]->Fill(looseIDElectrons[iElectron].p4.Pt(), weight); 
                if( datasetname+TypeSel+"_looseID_eta"     == hlist_El_LooseID[i]->GetName()) hlist_El_LooseID[i]->Fill(looseIDElectrons[iElectron].p4.Eta(), weight);   
                if( datasetname+TypeSel+"_looseID_phi"     == hlist_El_LooseID[i]->GetName()) hlist_El_LooseID[i]->Fill(looseIDElectrons[iElectron].p4.Phi(), weight);   
                if( datasetname+TypeSel+"_looseID_njet"    == hlist_El_LooseID[i]->GetName()) hlist_El_LooseID[i]->Fill(njet, weight); 
              }
              if(tightElectrons[itElectron].Charge != looseIDElectrons[iElectron].Charge) { 
                if( datasetname+TypeSel+"_looseID_invM"    == hlist_El_LooseID[i]->GetName()) hlist_El_LooseID[i]->Fill(invM, weight);
              }  
              else{ if( datasetname+TypeSel+"_looseID_invM_LS" == hlist_El_LooseID[i]->GetName()) hlist_El_LooseID[i]->Fill(invM, weight); }  
            }
          } 
        }
        
        
        //*******************
        //for loose Iso selection
        TLorentzVector dilepton_looseIso;
        for(unsigned int  iElectron= 0; iElectron < looseIsoElectrons.size(); iElectron++){
          
          if( fabs(tightElectrons[itElectron].p4.Pt() - looseIsoElectrons[iElectron].p4.Pt()) > 0.0001 ){
            lepton_loose.SetPtEtaPhiM(looseIsoElectrons[iElectron].p4.Pt(), looseIsoElectrons[iElectron].p4.Eta(), looseIsoElectrons[iElectron].p4.Phi(), 0);
            dilepton_looseIso = lepton_loose + lepton_tight;
            
            double invM = dilepton_looseIso.M();
            for(unsigned int i=0; i< hlist_El_LooseIso.size(); i++){
              
              if(tightElectrons[itElectron].Charge != looseIsoElectrons[iElectron].Charge && (invM >infMassCut && invM < supMassCut)){
                if( datasetname+TypeSel+"_looseIso_pt"      == hlist_El_LooseIso[i]->GetName()) hlist_El_LooseIso[i]->Fill(looseIsoElectrons[iElectron].p4.Pt(), weight);  
                if( datasetname+TypeSel+"_looseIso_pt_infEta15"      == hlist_El_LooseIso[i]->GetName()&& fabs(looseIsoElectrons[iElectron].p4.Eta()) < 1.5) hlist_El_LooseIso[i]->Fill(looseIsoElectrons[iElectron].p4.Pt(), weight);  
                if( datasetname+TypeSel+"_looseIso_pt_supEta15"      == hlist_El_LooseIso[i]->GetName()&& fabs(looseIsoElectrons[iElectron].p4.Eta()) > 1.5) hlist_El_LooseIso[i]->Fill(looseIsoElectrons[iElectron].p4.Pt(), weight); 
                if( datasetname+TypeSel+"_looseIso_eta"     == hlist_El_LooseIso[i]->GetName()) hlist_El_LooseIso[i]->Fill(looseIsoElectrons[iElectron].p4.Eta(), weight);   
                if( datasetname+TypeSel+"_looseIso_phi"     == hlist_El_LooseIso[i]->GetName()) hlist_El_LooseIso[i]->Fill(looseIsoElectrons[iElectron].p4.Phi(), weight);   
                if( datasetname+TypeSel+"_looseIso_njet"    == hlist_El_LooseIso[i]->GetName()) hlist_El_LooseIso[i]->Fill(njet, weight); 
              }
              if(tightElectrons[itElectron].Charge != looseIsoElectrons[iElectron].Charge) { 
                if( datasetname+TypeSel+"_looseIso_invM"    == hlist_El_LooseIso[i]->GetName()) hlist_El_LooseIso[i]->Fill(invM, weight);
              }  
              else{ if( datasetname+TypeSel+"_looseIso_invM_LS" == hlist_El_LooseIso[i]->GetName()) hlist_El_LooseIso[i]->Fill(invM, weight); }  
            }
          } 
        } 
        
        
        
        //*******************
        //for tight selection 
        TLorentzVector dilepton_Tight, lepton_tight1, lepton_tight2;
        if(tightElectrons.size() > 1 && (itElectron == 0 || itElectron == 1 ) ){
          
          lepton_tight1.SetPtEtaPhiM(tightElectrons[0].p4.Pt(), tightElectrons[0].p4.Eta(), tightElectrons[0].p4.Phi(), 0);
          lepton_tight2.SetPtEtaPhiM(tightElectrons[1].p4.Pt(), tightElectrons[1].p4.Eta(), tightElectrons[1].p4.Phi(), 0);
          dilepton_Tight = lepton_tight1 + lepton_tight2;
          double invM = dilepton_Tight.M(); 
          for(unsigned int i=0; i< hlist_El_Tight.size(); i++){
            if(tightElectrons[0].Charge != tightElectrons[1].Charge && (invM >infMassCut && invM < supMassCut)){
              if( datasetname+TypeSel+"_Tight_pt"      == hlist_El_Tight[i]->GetName()) hlist_El_Tight[i]->Fill(tightElectrons[itElectron].p4.Pt(), weight);  
              if( datasetname+TypeSel+"_Tight_pt_infEta15"      == hlist_El_Tight[i]->GetName()&& fabs(tightElectrons[itElectron].p4.Eta()) < 1.5) hlist_El_Tight[i]->Fill(tightElectrons[itElectron].p4.Pt(), weight);  
              if( datasetname+TypeSel+"_Tight_pt_supEta15"      == hlist_El_Tight[i]->GetName()&& fabs(tightElectrons[itElectron].p4.Eta()) > 1.5) hlist_El_Tight[i]->Fill(tightElectrons[itElectron].p4.Pt(), weight); 
              if( datasetname+TypeSel+"_Tight_eta"     == hlist_El_Tight[i]->GetName()) hlist_El_Tight[i]->Fill(tightElectrons[itElectron].p4.Eta(), weight);   
              if( datasetname+TypeSel+"_Tight_phi"     == hlist_El_Tight[i]->GetName()) hlist_El_Tight[i]->Fill(tightElectrons[itElectron].p4.Phi(), weight);   
              if( datasetname+TypeSel+"_Tight_njet"    == hlist_El_Tight[i]->GetName()) hlist_El_Tight[i]->Fill(njet, weight);
            }
            if(tightElectrons[0].Charge != tightElectrons[1].Charge) { 
              if( datasetname+TypeSel+"_Tight_invM"    == hlist_El_Tight[i]->GetName()) hlist_El_Tight[i]->Fill(invM, weight);
            }  
            else{ if( datasetname+TypeSel+"_Tight_invM_LS" == hlist_El_Tight[i]->GetName()) hlist_El_Tight[i]->Fill(invM, weight); } 
          }
        }
      }
    }
  }
  
}






void TagAndProbe::Savehistos(TString outputFileName){
  cout << " write histo " << endl;
  TFile * myFile = new TFile(outputFileName.Data(), "recreate");
  myFile->cd();
  for(unsigned int i=0; i< hlist_Mu_LooseID.size(); i++){
    
    hlist_Mu_LooseID[i]->Write(); 
    hlist_Mu_LooseIso[i]->Write(); 
    hlist_Mu_Tight[i]->Write(); 
    
    
  }
  
  for(unsigned int i=0; i< hlist_El_Tight.size(); i++){
    
    hlist_El_LooseID[i]->Write(); 
    hlist_El_LooseIso[i]->Write(); 
    hlist_El_Tight[i]->Write();  
  }
  
  
  myFile->Close();
  myFile = 0;
  delete myFile ;
}

