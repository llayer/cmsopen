#include "../interface/TriggerCommissioning.h"

                
TriggerCommissioning::TriggerCommissioning(){}


TriggerCommissioning::~TriggerCommissioning(){}


void TriggerCommissioning::CreateHistos(){
  
  //
  jet1_ref = new TH1F("jet1_ref","jet1_ref", 19,10,200);
  jet2_ref = new TH1F("jet2_ref","jet2_ref", 19,10,200);
  jet3_ref = new TH1F("jet3_ref","jet3_ref", 19,10,200);
  jet4_ref = new TH1F("jet4_ref","jet4_ref", 19,10,200);
 
  jet1_trig = new TH1F("jet1_trig","jet1_trig", 19,10,200);
  jet2_trig = new TH1F("jet2_trig","jet2_trig", 19,10,200);
  jet3_trig = new TH1F("jet3_trig","jet3_trig", 19,10,200);
  jet4_trig = new TH1F("jet4_trig","jet4_trig", 19,10,200);
  
  jet1_eff = new TH1F("jet1_eff","jet-leg efficiency vs pT", 19,10,200);
  jet2_eff = new TH1F("jet2_eff","jet-leg efficiency vs pT", 19,10,200);
  jet3_eff = new TH1F("jet3_eff","jet-leg efficiency vs pT", 19,10,200);
  jet4_eff = new TH1F("jet4_eff","jet-leg efficiency vs pT", 19,10,200);
  
  jet1_eff->Sumw2();
  jet2_eff->Sumw2();
  jet3_eff->Sumw2();
  jet4_eff->Sumw2();
  
  //
  //Double_t x2bins[16] = {0,5,10,15,20,25,30,35,40,45,50,60,70,80,90,100};
  Double_t x2bins[19] = {0,5,10,15,20,25,30,32.5,35,37.5,40,42.5,45,47.5,50,60,70,80,90};
  
  reference_tau= new TH1F("reference_tau","reference_tau", 18, x2bins);
  triggered_tau= new TH1F("triggered_tau","triggered_tau", 18, x2bins);
  reference_tau_eta= new TH1F("reference_tau_eta","reference_tau_eta", 5,0,2.5);
  triggered_tau_eta= new TH1F("triggered_tau_eta","triggered_tau_eta", 5,0,2.5);
    
  eff_tau= new TH1F("eff_tau","eff_tau", 18, x2bins);
  eff_tau_eta= new TH1F("eff_tau_eta","eff_tau_eta", 5,0,2.5);
  
  reference_tau->Sumw2();
  triggered_tau->Sumw2();
  reference_tau_eta->Sumw2();
  triggered_tau_eta->Sumw2();
  eff_tau->Sumw2();
  eff_tau_eta->Sumw2();
  
  //
  HLTtau_pt       = new TH1F("HLTtau_pt","HLTtau_pt", 50,0,100);
  HLTtau_eta      = new TH1F("HLTtau_eta","HLTtau_eta", 100,0,2.5);
  HLTtau_pt_diff  = new TH1F("HLTtau_pt_diff","HLTtau_pt_diff", 100,-100,100);
  HLTtau_eta_diff = new TH1F("HLTtau_eta_diff","HLTtau_eta_diff", 100,-0.2,0.2);
  HLTtau_DR_diff  = new TH1F("HLTtau_DR_diff","HLTtau_DR_diff", 100,0,0.1);
 
 
  HLTTau_HLTJet_DR = new TH1F("HLTTau_HLTJet_DR","HLTTau_HLTJet_DR",100,0,5);
  reference_tau_2= new TH1F("reference_tau_2","reference_tau_2", 18, x2bins);
  triggered_tau_2= new TH1F("triggered_tau_2","triggered_tau_2", 18, x2bins);
  eff_tau_2= new TH1F("eff_tau_2","eff_tau_2", 18, x2bins);
  
  HLTTau_HLTJet_DR->Sumw2();
  reference_tau_2->Sumw2();
  triggered_tau_2->Sumw2();
  eff_tau_2->Sumw2();    
 }


void TriggerCommissioning::SaveHistos(TString inputRootFile){

  TFile* f = new TFile(inputRootFile,"RECREATE");
  f->cd();
  
  //       
  jet1_ref->Write();
  jet2_ref->Write();
  jet3_ref->Write();
  jet4_ref->Write();
  
  jet1_trig->Write();
  jet2_trig->Write();
  jet3_trig->Write();
  jet4_trig->Write();
  
  jet1_eff->Divide(jet1_trig,jet1_ref,1,1,"B");
  jet2_eff->Divide(jet2_trig,jet2_ref,1,1,"B");
  jet3_eff->Divide(jet3_trig,jet3_ref,1,1,"B");
  jet4_eff->Divide(jet4_trig,jet4_ref,1,1,"B"); 
  
  jet1_eff->Write();
  jet2_eff->Write();
  jet3_eff->Write();
  jet4_eff->Write();
  
  //
  reference_tau->Write();
  triggered_tau->Write();
  eff_tau->Write();
  
  reference_tau_eta->Write();
  triggered_tau_eta->Write();
  eff_tau_eta->Write();
 
  HLTtau_pt->Write();
  HLTtau_eta->Write();
  HLTtau_pt_diff->Write();
  HLTtau_eta_diff->Write();
  HLTtau_DR_diff->Write();
  
  HLTTau_HLTJet_DR->Write();
  reference_tau_2->Write();
  triggered_tau_2->Write();
  eff_tau_2->Write();
 
}


void TriggerCommissioning::FillHistos(std::vector<NTJet> thejets, std::vector<NTTau> theTaus,  
bool passQuadJetIsoPFTau, float filterStatus, float filterStatus2, 
vector<TLorentzVector> tauObjTrig, vector<TLorentzVector> jetObjTrig){
   
  // find the jets
     
  int njets_eta25 = 0;
  int ijet0_eta25 =-1;
  int ijet1_eta25 =-1;
  int ijet2_eta25 =-1;
  int ijet3_eta25 =-1;
    
  
  for (unsigned int j=0; j<thejets.size(); j++)
  {
    if(fabs(thejets[j].p4.Eta())<2.5 && thejets[j].p4.Pt()>10.)
    {
     njets_eta25++; 
    
     if(njets_eta25 == 1 && thejets[j].p4.Pt()>70 && thejets[j].p4HLT.Px() !=0) ijet0_eta25 = j;
     if(njets_eta25 == 2 && thejets[j].p4.Pt()>70 && thejets[j].p4HLT.Px() !=0) ijet1_eta25 = j;
     if(njets_eta25 == 3 && thejets[j].p4.Pt()>70 && thejets[j].p4HLT.Px() !=0) ijet2_eta25 = j;
     if(njets_eta25 == 4 && thejets[j].p4.Pt()>10) ijet3_eta25 = j;}
   }
   
    
   if(njets_eta25 >= 4 && ijet0_eta25!=-1 && ijet1_eta25!=-1 && ijet2_eta25!=-1 && ijet3_eta25!=-1)
   { 
    if (njets_eta25>=1) jet1_ref->Fill(thejets[ijet0_eta25].p4.Pt()); 
    if (njets_eta25>=2) jet2_ref->Fill(thejets[ijet1_eta25].p4.Pt()); 
    if (njets_eta25>=3) jet3_ref->Fill(thejets[ijet2_eta25].p4.Pt()); 
    if (njets_eta25>=4) jet4_ref->Fill(thejets[ijet3_eta25].p4.Pt()); 
        
    if (njets_eta25>=1 && thejets[ijet0_eta25].p4HLT.Px() !=0)  jet1_trig->Fill(thejets[ijet0_eta25].p4.Pt());
    if (njets_eta25>=2 && thejets[ijet1_eta25].p4HLT.Px() !=0)  jet2_trig->Fill(thejets[ijet1_eta25].p4.Pt());
    if (njets_eta25>=3 && thejets[ijet2_eta25].p4HLT.Px() !=0)  jet3_trig->Fill(thejets[ijet2_eta25].p4.Pt());
    if (njets_eta25>=4 && thejets[ijet3_eta25].p4HLT.Px() !=0)  jet4_trig->Fill(thejets[ijet3_eta25].p4.Pt());
   }
    
    
   
  ///////////Tau Leg/////////////////////////////////////////////////////////////////

   int njet = 0;
   float deltaRMin = 99;
   double deltaR = 99;
   
   //std::cout << "=====================" <<std::endl;
  
   for (unsigned int s=0; s<thejets.size(); s++)
   {
     if (fabs(thejets[s].p4.Eta())<2.4 && thejets[s].p4.Pt()>10 && thejets[s].p4HLT.Px()!=0) 
     { 
       //if (thejets[s].p4.DeltaR(thejets[s].p4HLT)>0.4) std::cout <<"got it : DeltaR jet" << thejets[s].p4.DeltaR(thejets[s].p4HLT) <<std::endl; }
       //else njetMatched++;
       //std::cout << "theJet " <<thejets[s].p4.Eta()<<" "<< thejets[s].p4.Phi()<<" "<< thejets[s].p4.Pt()<< std::endl;
       //std::cout << "trigJet " <<thejets[s].p4HLT.Eta()<<" "<< thejets[s].p4HLT.Phi()<<" "<< thejets[s].p4HLT.Pt()<< std::endl;
       
       if (thejets[s].p4.DeltaR(thejets[s].p4HLT)<0.4)
       {
        njet++;
        //std::cout << "theJet DeltaR " <<thejets[s].p4.DeltaR(thejets[s].p4HLT) << std::endl;
        if (theTaus.size()==1) 
        {
         deltaR = thejets[s].p4.DeltaR(theTaus[0].p4);
         if(deltaR < deltaRMin) deltaRMin = deltaR;
         }
       }     
     }
    }
    //std::cout <<"filter  " << filterStatus << " " << njet << std::endl;
        
    
   if (njet>=4 && theTaus.size()==1 && deltaRMin<0.4 && filterStatus==1)
   {  
        reference_tau->Fill(theTaus[0].p4.Pt());
        if (theTaus[0].p4.Pt()>=50) reference_tau_eta->Fill(theTaus[0].p4.Eta());

        //if( theTaus[0].p4HLT.Px()!=0 && passQuadJetIsoPFTau == 1)
        //std::cout <<"filter2 " << filterStatus2 << std::endl;
        
        float deltaRMinTrig = 99.;
        float deltaRTrig =99.;
        float idx = -1;
        
        for (unsigned int i=0; i<tauObjTrig.size(); i++)
        { deltaRTrig = theTaus[0].p4.DeltaR(tauObjTrig[i]);
          if (deltaRTrig<deltaRMinTrig)
          { 
           deltaRMinTrig = deltaRTrig; 
           idx = i;
           }
         }
          
        if(filterStatus2==1 && deltaRMinTrig<0.4)
        {
          //std::cout <<"theTau  "<<theTaus[0].p4.Eta()<<" "<<theTaus[0].p4.Phi()<<" "<<theTaus[0].p4.Pt()<<std::endl;
          //std::cout <<"trigTau "<<tauObjTrig[idx].Eta()<<" "<<tauObjTrig[idx].Phi()<<" "<<tauObjTrig[idx].Pt()<<std::endl;
          //std::cout <<"Delta R " <<deltaRMinTrig<< std::endl;
        
          if (fabs(tauObjTrig[idx].Eta())<2.3)
          {
           HLTtau_pt->Fill(tauObjTrig[idx].Pt());
           HLTtau_eta->Fill(tauObjTrig[idx].Eta());
           HLTtau_pt_diff->Fill(theTaus[0].p4.Pt()-tauObjTrig[idx].Pt());
           HLTtau_eta_diff->Fill(theTaus[0].p4.Eta()-tauObjTrig[idx].Eta());
           HLTtau_DR_diff->Fill(theTaus[0].p4.Eta()-tauObjTrig[idx].Eta());
     
           triggered_tau->Fill(theTaus[0].p4.Pt());
           if (theTaus[0].p4.Pt()>=50) triggered_tau_eta->Fill(deltaRMinTrig);
           }
         }
      }

   eff_tau->Divide(triggered_tau,reference_tau,1,1,"B");
   eff_tau_eta->Divide(triggered_tau_eta,reference_tau_eta,1,1,"B");
   
   
   // number of objects in trigger
   //std::cout <<"======= "<< passQuadJetIsoPFTau<< std::endl;
   if(passQuadJetIsoPFTau)
   {
   float deltaR=3;
   float deltaRmin=3;
   if (filterStatus==1 && filterStatus2==1) 
   {
    for (unsigned int i=0; i<tauObjTrig.size(); i++)
    {
     //std::cout <<"tau "<< tauObjTrig[i].Eta()<<" " << tauObjTrig[i].Phi()<< std::endl;
     for (unsigned int i=0; i<jetObjTrig.size(); i++)
     {
      //std::cout <<"jet " << jetObjTrig[i].Eta()<<" " << jetObjTrig[i].Phi()<< std::endl;
      deltaR = tauObjTrig[0].DeltaR(jetObjTrig[i]);
      if(deltaR<deltaRmin)
      deltaRmin = deltaR;
      }
   
      }
      HLTTau_HLTJet_DR->Fill(deltaRmin);
     }//else cout << "problem"<<endl;
    }
    
    // tau-leg 2
    
    if (theTaus.size()==1 && filterStatus==1 && jetObjTrig.size()==4)
    {   reference_tau_2->Fill(theTaus[0].p4.Pt());
    
        float deltaRMinTrigTau = 99.;
        float deltaRTrigTau =99.;
        float idxTau = -1;
        
        for (unsigned int i=0; i<tauObjTrig.size(); i++)
        { deltaRTrigTau = theTaus[0].p4.DeltaR(tauObjTrig[i]);
          if (deltaRTrigTau<deltaRMinTrigTau)
          { 
           deltaRMinTrigTau = deltaRTrigTau; 
           idxTau = i;
           }
         }
        
        float deltaRMinTrigJet = 99.;
        float deltaRTrigJet =99.;
        float idxJet = -1;
        //std::cout <<" jetObjTrig.size() " <<jetObjTrig.size()<< std::endl;
        
        for (unsigned int j=0; j<jetObjTrig.size(); j++)
        { deltaRTrigJet = theTaus[0].p4.DeltaR(jetObjTrig[j]);
          if (deltaRTrigJet<deltaRMinTrigJet)
          { 
           deltaRMinTrigJet = deltaRTrigJet; 
           idxJet = j;
           }
         }
        
        //if(filterStatus2==1 && deltaRMinTrigTau<0.4)
          
        if(filterStatus2==1 && deltaRMinTrigTau<0.4 && deltaRMinTrigJet<0.4)
        { 
         triggered_tau_2->Fill(theTaus[0].p4.Pt());}
     }   
    eff_tau_2->Divide(triggered_tau_2,reference_tau_2,1,1,"B");
 
   
}

void TriggerCommissioning::Plots(){

}


