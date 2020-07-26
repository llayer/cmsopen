#include "../interface/TauIdEff.h"

                
TauIdEff::TauIdEff(){}


TauIdEff::~TauIdEff(){}


void TauIdEff::CreateHistos(){
 
  tauId_reference       = new TH1F ("tauId_reference      ","",50,0,100); 
  tauId_leadTrack       = new TH1F ("tauId_leadTrack      ","",50,0,100); 
  tauId_discr           = new TH1F ("tauId_discr          ","",50,0,100);
  tauId_againstElectron = new TH1F ("tauId_againstElectron","",50,0,100);
  tauId_againstMuon     = new TH1F ("tauId_againstMuon    ","",50,0,100);
  tauId_all             = new TH1F ("tauId_all            ","",50,0,100);
  tauId_eff             = new TH1F ("tauId_eff            ","",50,0,100);
  
  tauId_gen             = new TH1F ("tauId_gen            ","",50,0,100);
  tauId_gen_matched     = new TH1F ("tauId_gen_matched    ","",50,0,100);
  tauId_gen_eta         = new TH1F ("tauId_gen_eta","",25,-2.5,2.5);
  tauId_gen_matched_eta = new TH1F ("tauId_gen_matched_eta","",25,-2.5,2.5);
}


void TauIdEff::SaveHistos(TString inputRootFile){

  TFile* f = new TFile(inputRootFile,"RECREATE");
  f->cd();
  
  
  tauId_eff->Divide(tauId_all,tauId_reference);
  
  tauId_reference      ->Write();    
  tauId_leadTrack      ->Write();   
  tauId_discr          ->Write();   
  tauId_againstElectron->Write();
  tauId_againstMuon    ->Write();
  tauId_all            ->Write();
  tauId_eff            ->Write();
  
  
  tauId_gen            ->Write();
  tauId_gen_matched    ->Write();
  tauId_gen_eta        ->Write();
  tauId_gen_matched_eta->Write();
}


void TauIdEff::FillHistos(std::vector<NTTau> theTaus, std::vector<TLorentzVector> genTaus, std::vector<TLorentzVector> agenTaus){

  float DeltaRmin = 999.;
  float DeltaR    = 999.;
  
  //std::cout <<"----- " << theTaus.size()<< " " <<genTaus.size() <<" " <<agenTaus.size()<< std::endl;
   
  for (unsigned int j=0; j<theTaus.size(); j++)
  {
    //std::cout <<"reco " << theTaus[j].p4.Phi()<<" "<<theTaus[j].p4.Eta() <<std::endl;
    
    for (unsigned int k=0; k<genTaus.size(); k++)
    { 
       DeltaR = theTaus[j].p4.DeltaR(genTaus[k]);
       if (DeltaR < DeltaRmin) DeltaRmin = DeltaR;   
     }
    
    for (unsigned int l=0; l<agenTaus.size(); l++)
    { 
       DeltaR = theTaus[j].p4.DeltaR(agenTaus[l]);
       if (DeltaR < DeltaRmin) DeltaRmin = DeltaR;   
     }
     
     if (DeltaRmin<0.4)
     {
       tauId_reference->Fill(theTaus[j].p4.Pt());
       
       if (theTaus[j].leadTrackPt>5.)                              tauId_leadTrack->Fill(theTaus[j].p4.Pt());
       if (theTaus[j].GetDiscriminator("byTightIsolation")    ==1) tauId_discr->Fill(theTaus[j].p4.Pt());
       if (theTaus[j].GetDiscriminator("againstElectronLoose")==1) tauId_againstElectron->Fill(theTaus[j].p4.Pt());
       if (theTaus[j].GetDiscriminator("againstMuonLoose")    ==1) tauId_againstMuon->Fill(theTaus[j].p4.Pt());
       if (theTaus[j].leadTrackPt>5.&& theTaus[j].GetDiscriminator("byTightIsolation") ==1
                                    && theTaus[j].GetDiscriminator("againstElectronLoose")==1
                                    && theTaus[j].GetDiscriminator("againstMuonLoose")    ==1) tauId_all->Fill(theTaus[j].p4.Pt());
      }
    }
    
    
   
    // Matching generated taus to reco taus
    
    
    float DeltaRmin2 = 999.;
    float DeltaR2    = 999.;
  
    
    //for (unsigned int l=0; l<agenTaus.size(); l++)
    //{ 
    if(agenTaus.size()>0)
    {
     if(fabs(agenTaus[0].Eta())<2.5)
     {
     tauId_gen->Fill(agenTaus[0].Pt());
     tauId_gen_eta->Fill(agenTaus[0].Eta());
     for (unsigned int j=0; j<theTaus.size(); j++)
     {
      DeltaR2 = theTaus[j].p4.DeltaR(agenTaus[0]);
      if (DeltaR2 < DeltaRmin2) DeltaRmin2 = DeltaR2;   
     }
     
     if (DeltaRmin2<0.4) {tauId_gen_matched->Fill(agenTaus[0].Pt());tauId_gen_matched_eta->Fill(agenTaus[0].Eta());}
     }
     }
     //}
    
    DeltaRmin2 = 999.;
    DeltaR2    = 999.;
  
    //for (unsigned int l=0; l<genTaus.size(); l++)
    //{ 
     
    if(genTaus.size()>0)
    {
  
    if(fabs(genTaus[0].Eta())<2.5)
     {
    
     tauId_gen->Fill(genTaus[0].Pt());
     tauId_gen_eta->Fill(genTaus[0].Eta());
     for (unsigned int j=0; j<theTaus.size(); j++)
     {
      DeltaR2 = theTaus[j].p4.DeltaR(genTaus[0]);
      if (DeltaR2 < DeltaRmin2) DeltaRmin2 = DeltaR2;   
     }
     
     if (DeltaRmin2<0.4) {tauId_gen_matched->Fill(genTaus[0].Pt());tauId_gen_matched_eta->Fill(genTaus[0].Eta());}
     }
    }
   
}

void TauIdEff::Plots(){
 
  gStyle->SetPadRightMargin(0.13);
  gStyle->SetPadLeftMargin(0.13);
  gStyle->SetPadBottomMargin(0.13);
  gStyle->SetOptTitle(1);
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

}

