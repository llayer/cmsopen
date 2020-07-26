#include "../interface/FakeRate_LeptEff.h"

                
FakeRate_LeptEff::FakeRate_LeptEff(){

     
}


FakeRate_LeptEff::~FakeRate_LeptEff(){}





void FakeRate_LeptEff::CreateHistos(string TypeSel, string datasetname){

  TH2F *h_pt, *h_eta, *h_phi, *h_njet;
  TH3F *h_pt_eta, *h_pt_njet, *h_eta_njet;
  
  
  if(TypeSel == "muons"){
     
     //*******************
     //for loose selection
     //*******************
     h_pt     = new TH2F( (datasetname+TypeSel+"_loose_pt").c_str(),  "", 48, 0, 120, 12, 10, 70); 
     hlist_Mu_Loose.push_back(h_pt);
     h_eta    = new TH2F( (datasetname+TypeSel+"_loose_eta").c_str(), "", 48, 0, 120, 10, -2.5, 2.5); 
     hlist_Mu_Loose.push_back(h_eta);
     h_phi    = new TH2F( (datasetname+TypeSel+"_loose_phi").c_str(), "", 48, 0, 120, 10, -3.14, 3.14); 
     hlist_Mu_Loose.push_back(h_phi);
     h_njet   = new TH2F( (datasetname+TypeSel+"_loose_njet").c_str(),"", 48, 0, 120, 7, -0.5, 6.5); 
     hlist_Mu_Loose.push_back(h_njet);
     
     
     h_pt_eta   = new TH3F ( (datasetname+TypeSel+"_loose_pt_eta").c_str(),  "", 48, 0, 120,12, 10, 70, 10, -2.5, 2.5); 
     hlist_Mu_Loose_2D.push_back(h_pt_eta);
     h_pt_njet  = new TH3F ( (datasetname+TypeSel+"_loose_pt_njet").c_str(), "", 48, 0, 120,12, 10, 70, 7, -0.5, 6.5); 
     hlist_Mu_Loose_2D.push_back(h_pt_njet);
     h_eta_njet = new TH3F ( (datasetname+TypeSel+"_loose_eta_njet").c_str(),"", 48, 0, 120, 10, -2.5, 2.5, 7, -0.5, 6.5); 
     hlist_Mu_Loose_2D.push_back(h_eta_njet);
     
     //*******************
     //for tight selection
     //*******************
     h_pt     = new TH2F( (datasetname+TypeSel+"_tight_pt").c_str(),  "", 48, 0, 120, 12, 10, 70); 
     hlist_Mu_Tight.push_back(h_pt);
     h_eta    = new TH2F( (datasetname+TypeSel+"_tight_eta").c_str(), "", 48, 0, 120, 10, -2.5, 2.5); 
     hlist_Mu_Tight.push_back(h_eta);
     h_phi    = new TH2F( (datasetname+TypeSel+"_tight_phi").c_str(), "", 48, 0, 120, 10, -3.14, 3.14); 
     hlist_Mu_Tight.push_back(h_phi);
     h_njet   = new TH2F( (datasetname+TypeSel+"_tight_njet").c_str(),"", 48, 0, 120, 7, -0.5, 6.5); 
     hlist_Mu_Tight.push_back(h_njet);
     
     
     h_pt_eta   = new TH3F ( (datasetname+TypeSel+"_tight_pt_eta").c_str(),  "", 48, 0, 120, 12,   10,  70, 10, -2.5, 2.5); 
     hlist_Mu_Tight_2D.push_back(h_pt_eta);
     h_pt_njet  = new TH3F ( (datasetname+TypeSel+"_tight_pt_njet").c_str(), "", 48, 0, 120, 12,   10,  70,  7, -0.5, 6.5); 
     hlist_Mu_Tight_2D.push_back(h_pt_njet);
     h_eta_njet = new TH3F ( (datasetname+TypeSel+"_tight_eta_njet").c_str(),"", 48, 0, 120, 10, -2.5, 2.5,  7, -0.5, 6.5); 
     hlist_Mu_Tight_2D.push_back(h_eta_njet);
    
     
     if(datasetname == "Data"){
     
     
       h_pt     = new TH2F( (datasetname+TypeSel+"_Fake_pt").c_str(),  "", 48, 0, 120, 12, 10, 70); 
       hlist_Mu_Fake_Data.push_back(h_pt);
       h_eta    = new TH2F( (datasetname+TypeSel+"_Fake_eta").c_str(), "", 48, 0, 120, 10, -2.5, 2.5); 
       hlist_Mu_Fake_Data.push_back(h_eta);
       h_phi    = new TH2F( (datasetname+TypeSel+"_Fake_phi").c_str(), "", 48, 0, 120, 10, -3.14, 3.14); 
       hlist_Mu_Fake_Data.push_back(h_phi);
       h_njet   = new TH2F( (datasetname+TypeSel+"_Fake_njet").c_str(),"", 48, 0, 120, 7, -0.5, 6.5); 
       hlist_Mu_Fake_Data.push_back(h_njet);
     
     
       h_pt_eta   = new TH3F ( (datasetname+TypeSel+"_Fake_pt_eta").c_str(),  "", 48, 0, 120,12, 10, 70, 10, -2.5, 2.5); 
       hlist_Mu_Fake_Data_2D.push_back(h_pt_eta);
       h_pt_njet  = new TH3F ( (datasetname+TypeSel+"_Fake_pt_njet").c_str(), "", 48, 0, 120,12, 10, 70, 7, -0.5, 6.5); 
       hlist_Mu_Fake_Data_2D.push_back(h_pt_njet);
       h_eta_njet = new TH3F ( (datasetname+TypeSel+"_Fake_eta_njet").c_str(),"", 48, 0, 120, 10, -2.5, 2.5, 7, -0.5, 6.5); 
       hlist_Mu_Fake_Data_2D.push_back(h_eta_njet);
     
       h_pt     = new TH2F( (datasetname+TypeSel+"_LeptEff_pt").c_str(),  "", 48, 0, 120, 12, 10, 70); 
       hlist_Mu_LeptEff_Data.push_back(h_pt);
       h_eta    = new TH2F( (datasetname+TypeSel+"_LeptEff_eta").c_str(), "", 48, 0, 120, 10, -2.5, 2.5); 
       hlist_Mu_LeptEff_Data.push_back(h_eta);
       h_phi    = new TH2F( (datasetname+TypeSel+"_LeptEff_phi").c_str(), "", 48, 0, 120, 10, -3.14, 3.14); 
       hlist_Mu_LeptEff_Data.push_back(h_phi);
       h_njet   = new TH2F( (datasetname+TypeSel+"_LeptEff_njet").c_str(),"", 48, 0, 120, 7, -0.5, 6.5); 
       hlist_Mu_LeptEff_Data.push_back(h_njet);
     
     
       h_pt_eta   = new TH3F ( (datasetname+TypeSel+"_LeptEff_pt_eta").c_str(),  "", 48, 0, 120,12, 10, 70, 10, -2.5, 2.5); 
       hlist_Mu_LeptEff_Data_2D.push_back(h_pt_eta);
       h_pt_njet  = new TH3F ( (datasetname+TypeSel+"_LeptEff_pt_njet").c_str(), "", 48, 0, 120,12, 10, 70, 7, -0.5, 6.5); 
       hlist_Mu_LeptEff_Data_2D.push_back(h_pt_njet);
       h_eta_njet = new TH3F ( (datasetname+TypeSel+"_LeptEff_eta_njet").c_str(),"", 48, 0, 120, 10, -2.5, 2.5, 7, -0.5, 6.5); 
       hlist_Mu_LeptEff_Data_2D.push_back(h_eta_njet);
     
     
     
     }
     
     
     
    
  }
  
  if(TypeSel == "electron"){
    std::cout << " not available for electrons " << std::endl;
  }

}


void FakeRate_LeptEff::ReadHistos(TString inputRootFile){
   // to read histograms already produced



}



void FakeRate_LeptEff::FillHistos(NTMET theMet, std::vector<NTJet> thejets, string TypeSel, string datasetname){
  //cout << "TypeSel " << TypeSel << endl;
  //cout << " muons.size() " <<  muons.size() << endl;
   if(TypeSel == "muons" && muons.size() == 1){
    
    int njet = thejets.size();
     
    double deltaphiMuMET =  theMet.p4.Phi() - muons[0].p4.Phi();
    if(deltaphiMuMET < -1*M_PI ) deltaphiMuMET += 2*M_PI;
    if(deltaphiMuMET > M_PI    ) deltaphiMuMET -= 2*M_PI;
    double mT = pow(2*(muons[0].p4.Pt())*(theMet.p4.Pt())*(1-cos(deltaphiMuMET))  , 0.5);
    
    
    double relIso = muons[0].RelIso03();
    for(unsigned int i=0; i< hlist_Mu_Loose.size(); i++){
     if(relIso < 0.5){
       if((datasetname+TypeSel+"_loose_pt")   == hlist_Mu_Loose[i]->GetName() )  hlist_Mu_Loose[i]->Fill(mT, muons[0].p4.Pt());
       if((datasetname+TypeSel+"_loose_eta")  == hlist_Mu_Loose[i]->GetName() )  hlist_Mu_Loose[i]->Fill(mT, muons[0].p4.Eta());
       if((datasetname+TypeSel+"_loose_phi")  == hlist_Mu_Loose[i]->GetName() )  hlist_Mu_Loose[i]->Fill(mT, muons[0].p4.Phi());
       if((datasetname+TypeSel+"_loose_njet") == hlist_Mu_Loose[i]->GetName() )  hlist_Mu_Loose[i]->Fill(mT, njet);
     
     
       if( i<3 && (datasetname+TypeSel+"_loose_pt_eta")  == hlist_Mu_Loose_2D[i]->GetName() )  hlist_Mu_Loose_2D[i]->Fill(mT, muons[0].p4.Pt(),  muons[0].p4.Eta() );
       if( i<3 && (datasetname+TypeSel+"_loose_pt_njet") == hlist_Mu_Loose_2D[i]->GetName() )  hlist_Mu_Loose_2D[i]->Fill(mT, muons[0].p4.Pt(),  njet);
       if( i<3 && (datasetname+TypeSel+"_loose_eta_njet")== hlist_Mu_Loose_2D[i]->GetName() )  hlist_Mu_Loose_2D[i]->Fill(mT, muons[0].p4.Eta(), njet);
     }
     //*******************
     //for tight selection
     //*******************
     if(relIso < 0.15){
       if((datasetname+TypeSel+"_tight_pt")   == hlist_Mu_Tight[i]->GetName() ) hlist_Mu_Tight[i]->Fill(mT, muons[0].p4.Pt());
       if((datasetname+TypeSel+"_tight_eta")  == hlist_Mu_Tight[i]->GetName() ) hlist_Mu_Tight[i]->Fill(mT, muons[0].p4.Eta());
       if((datasetname+TypeSel+"_tight_phi")  == hlist_Mu_Tight[i]->GetName() ) hlist_Mu_Tight[i]->Fill(mT, muons[0].p4.Phi());
       if((datasetname+TypeSel+"_tight_njet") == hlist_Mu_Tight[i]->GetName() ) hlist_Mu_Tight[i]->Fill(mT, njet);
     
     
       if( i<3 && (datasetname+TypeSel+"_tight_pt_eta")   == hlist_Mu_Tight_2D[i]->GetName() ) hlist_Mu_Tight_2D[i]->Fill(mT, muons[0].p4.Pt(),  muons[0].p4.Eta());
       if( i<3 && (datasetname+TypeSel+"_tight_pt_njet")  == hlist_Mu_Tight_2D[i]->GetName() ) hlist_Mu_Tight_2D[i]->Fill(mT, muons[0].p4.Pt(),  njet);
       if( i<3 && (datasetname+TypeSel+"_tight_eta_njet") == hlist_Mu_Tight_2D[i]->GetName() ) hlist_Mu_Tight_2D[i]->Fill(mT, muons[0].p4.Eta(), njet);
     }
    }
    
    
    
  }
  
  
  
  

}



void FakeRate_LeptEff::CalculateFakeRateLeptonEff(){
 
 
 
  
  
  gStyle->SetPadRightMargin(0.13);
  gStyle->SetPadLeftMargin(0.13);
  gStyle->SetPadBottomMargin(0.13);
  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  
  gStyle->SetOptDate(0);
  gStyle->SetStatColor(0);
  gStyle->SetTitleColor(1);
  //gStyle->SetTitleW(0.4);
  //gStyle->SetTitleH(0.07);
  //gStyle->SetOptStat(stati);
  gStyle->SetOptStat(0);
  // gStyle->SetFrameFillColor(18);
  //gStyle->SetPalette(51,0);
  gStyle->SetPalette(1);
  
  
  TCanvas *c1 = new TCanvas("c1", "c1",10,32,782,552);
   
  c1->SetFillColor(10);
  c1->SetFillStyle(4000);
  c1->SetBorderSize(2);
  
  //c1.Divide(2,2);
  c1->SetFillColor(0);
  c1->SetBorderMode(0);
  c1->SetFrameFillColor(10);

  
  //RooWorkspace *w_loose = new RooWorkspace("w",kTRUE) ;  

  
  
  
  
  
  


}


void FakeRate_LeptEff::ShowFit(float pt, float eta, float phi, int njet, int param){





}


//******************************
//get values and errors for muon
//******************************

std::pair<float, float> FakeRate_LeptEff::GetFakeRateMuonPt(    float pt  ){

   std::pair<float, float> val_err(0, 0);
   
   
   
   
   return val_err;
}



std::pair<float, float> FakeRate_LeptEff::GetFakeRateMuonEta(   float eta ){


   std::pair<float, float> val_err(0, 0);

   return val_err;


}



std::pair<float, float> FakeRate_LeptEff::GetFakeRateMuonPhi(   float phi ){


   std::pair<float, float> val_err(0, 0);

   return val_err;


}



std::pair<float, float> FakeRate_LeptEff::GetFakeRateMuonNJets( int njet  ){


   std::pair<float, float> val_err(0, 0);

   return val_err;


}




std::pair<float, float> FakeRate_LeptEff::GetFakeRateMuonPtEta(   float pt,  float eta   ){

   std::pair<float, float> val_err(0, 0);

   return val_err;



}



std::pair<float, float> FakeRate_LeptEff::GetFakeRateMuonPtNJet(  float pt,  int   njets ){


   std::pair<float, float> val_err(0, 0);

   return val_err;


}



std::pair<float, float> FakeRate_LeptEff::GetFakeRateMuonEtaNJet( float eta, int   njets ){


   std::pair<float, float> val_err(0, 0);

   return val_err;


}







std::pair<float, float> FakeRate_LeptEff::GetLeptEffMuonPt(      float pt  ){

   std::pair<float, float> val_err(0, 0);

   return val_err;



}



std::pair<float, float> FakeRate_LeptEff::GetLeptEffMuonEta(   float eta ){

   std::pair<float, float> val_err(0, 0);

   return val_err;



}



std::pair<float, float> FakeRate_LeptEff::GetLeptEffMuonPhi(   float phi ){


   std::pair<float, float> val_err(0, 0);

   return val_err;


}



std::pair<float, float> FakeRate_LeptEff::GetLeptEffMuonNJets( int njet  ){


   std::pair<float, float> val_err(0, 0);

   return val_err;


}




std::pair<float, float> FakeRate_LeptEff::GetLeptEffMuonPtEta(   float pt,  float eta   ){


   std::pair<float, float> val_err(0, 0);

   return val_err;


}



std::pair<float, float> FakeRate_LeptEff::GetLeptEffMuonPtNJet(  float pt,  int   njets ){


   std::pair<float, float> val_err(0, 0);

   return val_err;


}



std::pair<float, float> FakeRate_LeptEff::GetLeptEffMuonEtaNJet( float eta, int   njets ){


   std::pair<float, float> val_err(0, 0);

   return val_err;


}


// get values for electrons


std::pair<float, float> FakeRate_LeptEff::GetFakeRateElectronPt(    float pt  ){

   std::pair<float, float> val_err(0, 0);
   
   
   
   return val_err;
}



std::pair<float, float> FakeRate_LeptEff::GetFakeRateElectronEta(   float eta ){


   std::pair<float, float> val_err(0, 0);

   return val_err;


}



std::pair<float, float> FakeRate_LeptEff::GetFakeRateElectronPhi(   float phi ){


   std::pair<float, float> val_err(0, 0);

   return val_err;


}



std::pair<float, float> FakeRate_LeptEff::GetFakeRateElectronNJets( int njet  ){


   std::pair<float, float> val_err(0, 0);

   return val_err;


}




std::pair<float, float> FakeRate_LeptEff::GetFakeRateElectronPtEta(   float pt,  float eta   ){

   std::pair<float, float> val_err(0, 0);

   return val_err;



}



std::pair<float, float> FakeRate_LeptEff::GetFakeRateElectronPtNJet(  float pt,  int   njets ){


   std::pair<float, float> val_err(0, 0);

   return val_err;


}



std::pair<float, float> FakeRate_LeptEff::GetFakeRateElectronEtaNJet( float eta, int   njets ){


   std::pair<float, float> val_err(0, 0);

   return val_err;


}







std::pair<float, float> FakeRate_LeptEff::GetLeptEffElectronPt(  float pt  ){

   std::pair<float, float> val_err(0, 0);

   return val_err;



}



std::pair<float, float> FakeRate_LeptEff::GetLeptEffElectronEta(   float eta ){

   std::pair<float, float> val_err(0, 0);

   return val_err;



}



std::pair<float, float> FakeRate_LeptEff::GetLeptEffElectronPhi(   float phi ){


   std::pair<float, float> val_err(0, 0);

   return val_err;


}



std::pair<float, float> FakeRate_LeptEff::GetLeptEffElectronNJets( int njet  ){


   std::pair<float, float> val_err(0, 0);

   return val_err;


}




std::pair<float, float> FakeRate_LeptEff::GetLeptEffElectronPtEta(   float pt,  float eta   ){


   std::pair<float, float> val_err(0, 0);

   return val_err;


}



std::pair<float, float> FakeRate_LeptEff::GetLeptEffElectronPtNJet(  float pt,  int   njets ){


   std::pair<float, float> val_err(0, 0);

   return val_err;


}



std::pair<float, float> FakeRate_LeptEff::GetLeptEffElectronEtaNJet( float eta, int   njets ){


   std::pair<float, float> val_err(0, 0);

   return val_err;


}
void FakeRate_LeptEff::Savehistos(TString outputFileName){
    cout << " write histo " << endl;
    TFile * myFile = new TFile(outputFileName.Data(), "recreate");
    myFile->cd();
    for(unsigned int i=0; i< hlist_Mu_Loose.size(); i++){
       hlist_Mu_Loose[i]   ->Write();
       if(i < 3) hlist_Mu_Loose_2D[i]->Write();
       hlist_Mu_Tight[i]   ->Write();
       if(i < 3) hlist_Mu_Tight_2D[i]->Write();
    }
    for(unsigned int i=0; i< hlist_Mu_Fake_Data.size(); i++){
       hlist_Mu_Fake_Data[i]      ->Write();
       if(i < 3) hlist_Mu_Fake_Data_2D[i]   ->Write();
       hlist_Mu_LeptEff_Data[i]   ->Write();
       if(i < 3) hlist_Mu_LeptEff_Data_2D[i]->Write();
    }
    myFile->Close();
    myFile = 0;
    delete myFile ;
}

