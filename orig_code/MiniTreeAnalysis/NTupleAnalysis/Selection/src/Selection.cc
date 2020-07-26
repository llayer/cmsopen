#include "../interface/Selection.h"

//for b-tagging
void InitVectorOfWeight(vector<float>& weightb){
            weightb.push_back (1.);
            weightb.push_back (0.);
            weightb.push_back (0.);
            weightb.push_back (0.);
            weightb.push_back (0.);
}

void LoadBWeight(Selection& sel, vector<float>& weightb, const vector<NTJet>& theselJets){
                vector < float >weight_temp = sel.GetSFBweight ().GetWeigth4BSel (sel.GetMethodb (), sel.GetSystb (), theselJets);
                weightb[0] = weight_temp[0];    //weight of the event
                weightb[1] = weight_temp[1];    //proba 0 jet
                weightb[2] = weight_temp[2];    //proba 1 jet
                weightb[3] = weight_temp[3];    //proba 2 jets
                weightb[4] = weight_temp[4];    //proba at least 3 jets
}

                
Selection::Selection(){
      JetType = "";
      jets.clear();
      jetmets.clear();
      electrons.clear();
      muons.clear();
      TauType = "";
      taus.clear();
      tausVec.clear();
      vertex.clear();
      genTaus.clear();
      genATaus.clear();
      tauObjTrig.clear();
      jetObjTrig.clear();
      triggerList.clear();
      triggerPrescaleList.clear();
      eventNumber = 0;
      runNumber = 0;
      tmeme = -1;
      eleHLTMatch = false;
      JetPtThreshold_     = 0.;
      JetEtaThreshold_    = 999.;
      MuonPtThreshold_    = 0.;
      MuonEtaThreshold_   = 999.;
      MuonRelIso_         = 999.;
      MuonNofValidHits_   = 0;
      MuonNofValidTrHits_ = 10;
      MuonD0Cut_          = 0.;
      MuonNormChi2_       = 0;
      MuonVertexMatchThr_ = 999.;
      ElectronPtThreshold_  = 0.;
      ElectronEtaThreshold_ = 999.;
      ElectronRelIso_       = 999.;
      ElectronD0Cut_        = 0.;
      ElectronVertexMatchThr_ = 999.;
      ElectronETSCThr_        = 0;
      DRemuThr_         = 999.;
      TauPtThreshold_   = 0.;
      TauEtaThreshold_  = 999.;
      TauLeadTrkPtCut_  = 0;
      TauVertexMatchThr_= 999.;
      VertexNdofThr_    = 0;
      VertexZThr_       = 999.;
      VertexRhoThr_     = 999.;
      METThreshold_     = 0;
      npu=0;
      histo_jesunc_ = 0;
      scaleFactEl   = 0;
      scaleFactMu   = 0;
}

void Selection::LoadEvent(const NTEvent* event){
      jetmets      = event->jetMetVec;
      electrons    = event->electrons;
      muons        = event->muons;
      tausVec      = event->tausVec;
      vertex       = event->VertexVec;
      triggerList  = event->triggers;
      triggerPrescaleList  = event->prescales;
      eleHLTMatch  = event->match_HLT_Ele10_LW_L1R_recoEl;
      eventNumber  = event->eventNb;
      runNumber    = event->runNb;
      tmeme        = event->TMEME;
      genTaus      = event->Generatedtaus;
      genATaus     = event->GeneratedAtaus;
      tauObjTrig   = event->tauObjTrig; 
      jetObjTrig   = event->jetObjTrig;
      bool JetAlgoFound = SetDefaultJetCollection(JetType); 
      if(!JetAlgoFound) cerr<<"The jets collection was not found !"<<endl;
      bool TauAlgoFound = SetDefaultTauCollection(TauType); 
      if(!TauAlgoFound) cerr<<"The tau collection was not found !"<<endl;
      npu  = event->num_pileup_bc0; 
      wAndDecays = event->wAndDecays;
      zAndDecays = event->zAndDecays;
      
}

void Selection::Reset(){
      met = NTMET();
      jets.clear();
      jetmets.clear();
      electrons.clear();
      muons.clear();
      tausVec.clear();
      vertex.clear();
      triggerList.clear();
      triggerPrescaleList.clear();
      genTaus.clear();
      genATaus.clear();
      tauObjTrig.clear();
      jetObjTrig.clear();
}

Selection::Selection(const Selection & s){
      jets      = s.jets;
      met       = s.met;
      jetmets   = s.jetmets;
      electrons = s.electrons;
      muons     = s.muons;
      tausVec   = s.tausVec;
      vertex    = s.vertex; 
      genTaus   = s.genTaus;
      genATaus  = s.genATaus;
      tauObjTrig = s.tauObjTrig;
      jetObjTrig = s.jetObjTrig;
      JetType   = s.JetType;
      TauType   = s.TauType;
      triggerList  = s.triggerList;
      triggerPrescaleList  = s.triggerPrescaleList;
      eleHLTMatch  = s.eleHLTMatch;
      eventNumber    = s.eventNumber;
      runNumber    = s.runNumber;
      tmeme        = s.tmeme;
      JetPtThreshold_         = s.JetPtThreshold_;
      JetEtaThreshold_        = s.JetEtaThreshold_;
      MuonPtThreshold_        = s.MuonPtThreshold_;
      MuonEtaThreshold_       = s.MuonEtaThreshold_;
      MuonRelIso_             = s.MuonRelIso_;
      MuonNofValidTrHits_     = s.MuonNofValidTrHits_;
      MuonNofValidHits_       = s.MuonNofValidHits_;
      MuonD0Cut_              = s.MuonD0Cut_;
      MuonNormChi2_           = s.MuonNormChi2_;
      MuonVertexMatchThr_     = s.MuonVertexMatchThr_;
      ElectronPtThreshold_    = s.ElectronPtThreshold_;
      ElectronEtaThreshold_   = s.ElectronEtaThreshold_;
      ElectronRelIso_         = s.ElectronRelIso_;
      ElectronD0Cut_          = s.ElectronD0Cut_;
      ElectronVertexMatchThr_ = s.ElectronVertexMatchThr_;
      ElectronETSCThr_        = s.ElectronETSCThr_;
      DRemuThr_               = s.DRemuThr_;
      TauEtaThreshold_        = s.TauEtaThreshold_;
      TauLeadTrkPtCut_        = s.TauLeadTrkPtCut_;
      TauVertexMatchThr_      = s.TauVertexMatchThr_;
      VertexNdofThr_          = s.VertexNdofThr_;
      VertexZThr_             = s.VertexZThr_;
      VertexRhoThr_           = s.VertexRhoThr_;
      METThreshold_           = s.METThreshold_;
      npu    = s.npu;


      // specific to btag weight
     flag_btagweight_ = s.flag_btagweight_;
     systb_ = s.systb_;
     methodb_ = s.methodb_;
     sfb_ = s.sfb_;

     //copy of histos
      if (s.histo_jesunc_) histo_jesunc_      = (TH2F*) s.histo_jesunc_->Clone();
      if (s.scaleFactEl) scaleFactEl          = (TH2F*) s.scaleFactEl->Clone();
      if (s.scaleFactMu) scaleFactMu          = (TH2F*) s.scaleFactMu->Clone();

}

     
Selection::~Selection(){
        //cout<<"Call the destructor of selection"<<endl;
        delete histo_jesunc_;
        delete scaleFactEl;
        delete scaleFactMu;
}


void Selection::AddCollection(std::vector<NTElectron> elec){
        electrons = electrons;
}

void Selection::AddCollection(std::vector<NTMuon> mu){
        muons = mu;
}

void Selection::AddCollection(std::vector<NTCollection<NTTau> > ta){
        tausVec = ta;
}

void Selection::AddCollection(std::vector<NTJetMet> jm){
        jetmets = jm;
}

void Selection::AddCollection(std::vector<NTJet> je){
        jets = je;
}

void Selection::AddCollection(std::vector<NTVertex> ve){
        vertex = ve;
}

void Selection::AddCollection(std::vector<std::pair<string, bool> >  tl){
        triggerList = tl;
}

void Selection::AddCollection(std::vector<std::pair<string, int> >  tpl){
        triggerPrescaleList = tpl;
}



void Selection::AddMET(NTMET me){
        met = me;
}
      
bool Selection::SetDefaultJetCollection(string jetCollName) {
        JetType = jetCollName;
        for(unsigned int i=0;i<jetmets.size();i++){
                if(jetmets[i].algo==JetType){
                        jets = jetmets[i].objects;
                        met = jetmets[i].met;
                        return true;
                }
        } 
        return false;
}

bool Selection::SetDefaultTauCollection(string tauCollName) {
       TauType = tauCollName;
       for(unsigned int i=0;i<tausVec.size();i++){
               if(tausVec[i].algo==TauType){
                       taus = tausVec[i].objects;
                       return true;
               }
       } 
       return false;
}
 
void Selection::SetElectronRequirements(float PtThr, float EtaThr, float RelIsoThr, float D0Thr, float VertexMatchThr, float ElectronETSCThr, float DRemuThr){
      ElectronPtThreshold_    = PtThr;
      ElectronEtaThreshold_   = EtaThr;
      ElectronRelIso_         = RelIsoThr;
      ElectronD0Cut_          = D0Thr;
      ElectronVertexMatchThr_ = VertexMatchThr;
      ElectronETSCThr_        = ElectronETSCThr; 
      DRemuThr_               = DRemuThr;
}

void Selection::SetMuonRequirements(float PtThr, float EtaThr, float RelIsoThr, float D0Thr, float VertexMatchThr, float NValidHitsThr, float NValidTkHitsThr, float Chi2Thr){
      MuonPtThreshold_    = PtThr;
      MuonEtaThreshold_   = EtaThr;
      MuonRelIso_         = RelIsoThr;
      MuonNofValidTrHits_ = NValidTkHitsThr;
      MuonNofValidHits_   = NValidHitsThr;
      MuonD0Cut_          = D0Thr;
      MuonNormChi2_       = Chi2Thr;
      MuonVertexMatchThr_ = VertexMatchThr; 
}

void Selection::SetTauRequirements(float PtThr, float EtaThr, float VertexMatchThr, float LeadTrkPtThr){
      TauPtThreshold_    = PtThr;
      TauEtaThreshold_   = EtaThr;
      TauLeadTrkPtCut_   = LeadTrkPtThr;
      TauVertexMatchThr_ = VertexMatchThr; 
}

void Selection::SetVertexRequirements(float VertexNdofThr, float VertexZThr, float VertexRhoThr){
        VertexNdofThr_ = VertexNdofThr;
        VertexNdofThr_ = VertexNdofThr;
        VertexRhoThr_  = VertexRhoThr;
}

void Selection::SetJetRequirements(float PtThr, float EtaThr){
        JetPtThreshold_  = PtThr;
        JetEtaThreshold_ = EtaThr;
}

void Selection::SetMETRequirements(float PtThr){
}
    
//************************************************
//Vertex selection
//************************************************  

std::vector<NTVertex> Selection::GetSelectedVertex() const{
        vector<NTVertex> selectedVertex;
        for(unsigned int i=0;i<vertex.size();i++){
                if(vertex[i].isFake)                    continue;
                if(vertex[i].ndof<= VertexNdofThr_)     continue;
                if(fabs(vertex[i].p3.Z())> VertexZThr_) continue;
                if(vertex[i].rho>= VertexRhoThr_)       continue;
                selectedVertex.push_back(vertex[i]);
        }
        return selectedVertex;
} 
    
//************************************************
//Jets selection
//************************************************  

std::vector<NTJet> Selection::GetScaledJets(float scale) const{
    std::vector<NTJet> newJets = jets;
    for (unsigned int i=0; i<newJets.size(); i++){
       float newscale = 0;
       float eta  = newJets[i].p4.Eta();
       float pt  = newJets[i].p4.Pt();
       if (pt>=300. ) pt = 300;
       if ( scale<1.0 ) {
          newscale = (1.- histo_jesunc_->GetBinContent(histo_jesunc_->FindBin(eta,pt)));
       } else {
          newscale = (1.+ histo_jesunc_->GetBinContent(histo_jesunc_->FindBin(eta,pt)));
       }
//       std::cout<<"eta,pt, scale, newscale "<<eta<<" "<<pt<<" "<<scale<<" "<<newscale<<std::endl;
      double Px = 0;
      double Py = 0;
      double Pz = 0;
      double E = 0;
      Px   = newscale*newJets[i].p4.Px();
      Py   = newscale*newJets[i].p4.Py();
      Pz   = newscale*newJets[i].p4.Pz();
      E    =  newscale*newJets[i].p4.E();
      newJets[i].p4.SetPxPyPzE(Px, Py, Pz, E);
    }
    
    return newJets;
}

std::vector<NTJet>  Selection::GetSmearedJets(vector<NTJet> injets, float jetResFactor) const{
    std::vector<NTJet> newJets = injets;
    for (unsigned int i=0; i<newJets.size(); i++){
      double Px = 0;
      double Py = 0;
      double Pz = 0;
      double E = 0;
      double gen_pt = newJets[i].p4Gen.Pt(); //to be changed
      double jet_pt = newJets[i].p4.Pt();
      double deltapt = (jet_pt - gen_pt) * jetResFactor;
      double ptscale = ((jet_pt + deltapt) / jet_pt);
      if(ptscale <0 ) ptscale = 0;
      Px   = ptscale*newJets[i].p4.Px();
      Py   = ptscale*newJets[i].p4.Py();
      Pz   = ptscale*newJets[i].p4.Pz();
      E   = ptscale*newJets[i].p4.E();
      newJets[i].p4.SetPxPyPzE(Px, Py, Pz, E);
    }
    return newJets;
}

 
NTMET Selection::GetScaledMET(float scale) const{
    double missetX = 0.;
    double missetY = 0.;
    for (unsigned int i=0; i<jets.size(); i++){
      missetX += (1-scale) * jets[i].p4.Px();
      missetY += (1-scale) * jets[i].p4.Py();
    }
    NTMET newMET = met;
    newMET.p4.SetPxPyPzE(met.p4.Px()+missetX, met.p4.Py()+missetY, 0 , sqrt(pow(met.p4.Px()+missetX,2.) + pow(met.p4.Py()+missetY,2.) + met.p4.M()));
    return newMET;
}

NTMET  Selection::GetSmearedMET(vector<NTJet> injets, float jetResFactor) const{
    double missetX = 0.;
    double missetY = 0.;
    for (unsigned int i=0; i<injets.size(); i++){
      double gen_pt = injets[i].p4Gen.Pt(); //to be changed: should be the pt of genjet !
      double jet_pt = injets[i].p4.Pt();
      double deltapt = (jet_pt - gen_pt) * jetResFactor;
      double ptscale = ((jet_pt + deltapt) / jet_pt);
      if(ptscale <0 ) ptscale = 0;
      missetX   = (ptscale-1)*injets[i].p4.Px();
      missetY   = (ptscale-1)*injets[i].p4.Py();
    }
    NTMET newMET = met;
    newMET.p4.SetPxPyPzE(met.p4.Px()+missetX, met.p4.Py()+missetY, 0 , sqrt(pow(met.p4.Px()+missetX,2.) + pow(met.p4.Py()+missetY,2.) + met.p4.M()));
    return newMET;
}

NTMET Selection::GetUnclusScaledMET(bool applyUnclusScale, float scale) const{
    double missetX = met.p4.Px();
    double missetY = met.p4.Py();
    for (unsigned int i=0; i<jets.size(); i++){
      missetX -= jets[i].p4.Px();
      missetY -= jets[i].p4.Py();
    }
    for (unsigned int i=0; i<electrons.size(); i++){
      missetX -= electrons[i].p4.Px();
      missetY -= electrons[i].p4.Py();
    }
    for (unsigned int i=0; i<muons.size(); i++){
      missetX -= muons[i].p4.Px();
      missetY -= muons[i].p4.Py();
    }
    for (unsigned int i=0; i<taus.size(); i++){
      missetX -= taus[i].p4.Px();
      missetY -= taus[i].p4.Py();
    }
    NTMET newMET = met;
    newMET.p4.SetPxPyPzE(met.p4.Px()+(scale-1)*missetX, met.p4.Py()+(scale-1)*missetY, 0 , sqrt(pow(met.p4.Px()+(scale-1)*missetX,2.) + pow(met.p4.Py()+(scale-1)*missetY,2.) + met.p4.M()));
    return newMET;
}



NTMET Selection::GetMET(bool applyJES, float scale, bool applyJER, float ResFactor) const{
        //WARNING: force to not apply the JES - it's due to JES/JEC correction scenario in PAT
        applyJES = false;
        if(applyJES){
                if(applyJER){
                        return GetSmearedMET(GetScaledJets(scale),ResFactor);
                }
                return GetScaledMET(scale); 
        }
        if(applyJER){
                return GetSmearedMET(GetJets(), ResFactor);
        }
        else return met;
}







//*******************************************************
//tools for Type1 MET calculated ourselves, on the flight.
//*******************************************************

NTMET Selection::GetScaledType1MET(NTMET &themet, float scale) const{
    double missetX = 0.;
    double missetY = 0.;
    for (unsigned int i=0; i<jets.size(); i++){
      missetX += (1-scale) * jets[i].p4.Px();
      missetY += (1-scale) * jets[i].p4.Py();
    }
    NTMET newMET = themet;
    newMET.p4.SetPxPyPzE(themet.p4.Px()+missetX, themet.p4.Py()+missetY, 0 , sqrt(pow(themet.p4.Px()+missetX,2.) + pow(themet.p4.Py()+missetY,2.) + themet.p4.M()));
    return newMET;
}

NTMET  Selection::GetSmearedType1MET(NTMET &themet, vector<NTJet> injets, float jetResFactor) const{
    double missetX = 0.;
    double missetY = 0.;
    for (unsigned int i=0; i<injets.size(); i++){
      double gen_pt = injets[i].p4Gen.Pt(); //to be changed: should be the pt of genjet !
      double jet_pt = injets[i].p4.Pt();
      double deltapt = (jet_pt - gen_pt) * jetResFactor;
      double ptscale = ((jet_pt + deltapt) / jet_pt);
      if(ptscale <0 ) ptscale = 0;
      missetX   = (ptscale-1)*injets[i].p4.Px();
      missetY   = (ptscale-1)*injets[i].p4.Py();
    }
    NTMET newMET = met;
    newMET.p4.SetPxPyPzE(themet.p4.Px()+missetX, themet.p4.Py()+missetY, 0 , sqrt(pow(themet.p4.Px()+missetX,2.) + pow(themet.p4.Py()+missetY,2.) + themet.p4.M()));
    return newMET;
}





NTMET Selection::GetType1MET( vector<NTJet> injets, bool applyJES, float scale, bool applyJER, float ResFactor) const{
        //WARNING: force to not apply the JES - it's due to JES/JEC correction scenario in PAT
        
        double missetX = 0.;
        double missetY = 0.;
 
        for (unsigned int i=0; i<injets.size(); i++){
          if(injets[i].p4.Pt() < 10 ) continue;
          missetX   += - injets[i].corrJES*injets[i].p4.Px() + injets[i].p4.Px();
          missetY   += - injets[i].corrJES*injets[i].p4.Py() + injets[i].p4.Py();
        }

        
        
        NTMET themet = met;
        themet.p4.SetPxPyPzE(themet.p4.Px()+missetX, themet.p4.Py()+missetY, 0 , sqrt(pow(themet.p4.Px()+missetX,2.) + pow(themet.p4.Py()+missetY,2.) + themet.p4.M()));
  
        
        if(applyJES){
                if(applyJER){
                        return GetSmearedType1MET(themet, GetScaledJets(scale),ResFactor );
                }
                return GetScaledType1MET(themet, scale); 
        }
        if(applyJER){
                return GetSmearedType1MET(themet, GetJets(), ResFactor);
        }
        else return themet;
}



NTMET Selection::GetScaledType1METWithJER(vector<NTJet> injets, bool applyJES, float scale, bool applyJER, float ResFactor) const{
    
    double missetX = 0.;
    double missetY = 0.;
 
    NTMET themet = GetType1MET(injets, applyJES, scale, applyJER, ResFactor);
    
    double MCkfac[4];
    MCkfac[0] = 1.06177;
    MCkfac[1] = 1.08352;
    MCkfac[2] = 1.02911;
    MCkfac[3] = 1.15288;
    
    for (unsigned int i=0; i<injets.size(); i++){
          if(injets[i].p4.Pt() < 10 ) continue;
          double  DeltaPt = injets[i].p4.Pt() - injets[i].p4Gen.Pt();
          double kfac = 0;
          if(fabs(injets[i].p4.Eta()) < 1.1                                   ) kfac = MCkfac[0];
          if(fabs(injets[i].p4.Eta()) > 1.1 && fabs(injets[i].p4.Eta()) < 1.7 ) kfac = MCkfac[1];
          if(fabs(injets[i].p4.Eta()) > 1.7 && fabs(injets[i].p4.Eta()) < 2.3 ) kfac = MCkfac[2];
          if(fabs(injets[i].p4.Eta()) >2.3                                    ) kfac = MCkfac[3];
          
          double pt_smear = injets[i].p4.Pt() + (kfac-1)*DeltaPt;
           
          double pt_ratio = pt_smear/injets[i].p4.Pt();
          
          missetX   += -pt_ratio*injets[i].p4.Px() + injets[i].p4.Px();
          missetY   += -pt_ratio*injets[i].p4.Py() + injets[i].p4.Py();
          //missetX   += - injets[i].corrJES*injets[i].p4.Px() + injets[i].p4.Px();
          //missetY   += - injets[i].corrJES*injets[i].p4.Py() + injets[i].p4.Py();
    }
    
    
    NTMET newMET = themet;  
    newMET.p4.SetPxPyPzE(themet.p4.Px()+missetX, themet.p4.Py()+missetY, 0 , sqrt(pow(themet.p4.Px()+missetX,2.) + pow(themet.p4.Py()+missetY,2.) + themet.p4.M()));
    return newMET;
}

NTMET Selection::GetGenMET(std::vector<WDecaysMC> &wAndDecays){
    NTMET theGenMET ;  
    TLorentzVector p4GenMET;
    for(unsigned int i = 0; i < wAndDecays.size(); i++){
       p4GenMET += (wAndDecays)[i].p4_Neu_gen;
    }

    theGenMET.p4.SetPxPyPzE(-p4GenMET.Px(), -p4GenMET.Py(), 0, sqrt(pow(p4GenMET.Px(),2.) + pow(p4GenMET.Py(),2.) + p4GenMET.M()));

    return theGenMET;

}


//*******************************************************
//*******************************************************



std::vector<NTJet> Selection::GetSelectedJets(std::vector<NTMuon> muon_cand, std::vector<NTElectron> elec_cand, bool applyJES, float scale, bool applyJER, float ResFactor) const{
  std::vector<NTJet> selectedJets;
  std::vector<NTJet> scaledJets;
  if(applyJES) scaledJets = GetScaledJets(scale);
  else scaledJets = jets;
  if(applyJER) scaledJets = GetSmearedJets(scaledJets, ResFactor);
  for(unsigned int i=0;i<scaledJets.size();i++){
    if(fabs(scaledJets[i].p4.Eta())> JetEtaThreshold_ ||  scaledJets[i].p4.Pt()<JetPtThreshold_) continue;
    
    double deltaRmu = 10000;
    double deltaRel = 10000;
    
    for(unsigned int imu=0; imu< muon_cand.size(); imu++){
      double deltaR = scaledJets[i].p4.DeltaR(muon_cand[imu].p4);
      if(deltaR < deltaRmu) deltaRmu = deltaR;
    }
    
    for(unsigned int iel=0; iel< elec_cand.size(); iel++){
      double deltaR = scaledJets[i].p4.DeltaR(elec_cand[iel].p4);
      if(deltaR < deltaRel) deltaRel = deltaR;
    }
    
    if( deltaRmu > 0.4  && deltaRel > 0.4) selectedJets.push_back(scaledJets[i]);
    //if( deltaRmu > 0.4  ) selectedJets.push_back(scaledJets[i]);
        
  }
  std::sort(selectedJets.begin(),selectedJets.end(),HighestPt());
  return selectedJets;
}


std::vector<NTJet> Selection::GetSelectedJets(std::vector<NTMuon> muon_cand, std::vector<NTElectron> elec_cand, std::vector<NTTau> tau_cand,bool applyJES, float scale, bool applyJER, float ResFactor) const{
  std::vector<NTJet> selectedJets;
  std::vector<NTJet> scaledJets;
  if(applyJES) scaledJets = GetScaledJets(scale);
  else scaledJets = jets;
  if(applyJER) scaledJets = GetSmearedJets(scaledJets, ResFactor);
  for(unsigned int i=0;i<scaledJets.size();i++){
    if(fabs(scaledJets[i].p4.Eta())> JetEtaThreshold_ ||  scaledJets[i].p4.Pt()<JetPtThreshold_) continue;
    
    double deltaRmu  = 10000;
    double deltaRel  = 10000;
    double deltaRtau = 10000;
    
    for(unsigned int imu=0; imu< muon_cand.size(); imu++){
      double deltaR = scaledJets[i].p4.DeltaR(muon_cand[imu].p4);
      if(deltaR < deltaRmu) deltaRmu = deltaR;
    }
    
    for(unsigned int iel=0; iel< elec_cand.size(); iel++){
      double deltaR = scaledJets[i].p4.DeltaR(elec_cand[iel].p4);
      if(deltaR < deltaRel) deltaRel = deltaR;
    }
    
    for(unsigned int itau=0; itau< tau_cand.size(); itau++){
      double deltaR = scaledJets[i].p4.DeltaR(tau_cand[itau].p4);
      if(deltaR < deltaRtau) deltaRtau = deltaR;
    }
    
    if( deltaRmu > 0.4  && deltaRel > 0.4 && deltaRtau > 0.5) selectedJets.push_back(scaledJets[i]);
    //if( deltaRmu > 0.4  ) selectedJets.push_back(scaledJets[i]);
        
  }
  std::sort(selectedJets.begin(),selectedJets.end(),HighestPt());
  return selectedJets;
}


std::vector<NTJet> Selection::GetSelectedJets(std::vector<NTTau> tau_cand, bool applyJES, float scale, bool applyJER, float ResFactor) const{
  std::vector<NTJet> selectedJets;
  std::vector<NTJet> scaledJets;
  if(applyJES) scaledJets = GetScaledJets(scale);
  else scaledJets = jets;
  if(applyJER) scaledJets = GetSmearedJets(scaledJets, ResFactor);
 
  for(unsigned int i=0;i<scaledJets.size();i++){
    if(fabs(scaledJets[i].p4.Eta())> JetEtaThreshold_ ||  scaledJets[i].p4.Pt()<JetPtThreshold_) continue;
   
    double deltaRtau = 10000;
    
    for(unsigned int itau=0; itau< tau_cand.size(); itau++){
      double deltaR = scaledJets[i].p4.DeltaR(tau_cand[itau].p4);
      if(deltaR < deltaRtau) deltaRtau = deltaR; 
      
    }
    
    if(deltaRtau > 0.5) selectedJets.push_back(scaledJets[i]);
    //if( deltaRmu > 0.4  ) selectedJets.push_back(scaledJets[i]);
        
  }
  std::sort(selectedJets.begin(),selectedJets.end(),HighestPt());
  return selectedJets;
}

std::vector<NTJet> Selection::GetSelectedJets(bool applyJES, float scale, bool applyJER, float ResFactor) const{
  std::vector<NTJet> selectedJets;
  std::vector<NTJet> scaledJets;
  if(applyJES) scaledJets = GetScaledJets(scale);
  else scaledJets = jets;
  if(applyJER) scaledJets = GetSmearedJets(scaledJets, ResFactor);
  for(unsigned int i=0;i<scaledJets.size();i++){
    if(fabs(scaledJets[i].p4.Eta())> JetEtaThreshold_ ||  scaledJets[i].p4.Pt()<JetPtThreshold_) continue;   
    selectedJets.push_back(scaledJets[i]);
        
  }
  std::sort(selectedJets.begin(),selectedJets.end(),HighestPt());
  return selectedJets;
}

std::vector<NTJet> Selection::GetSelectedJets(float PtThr, float EtaThr, bool applyJES, float scale, bool applyJER, float ResFactor) const{
  std::vector<NTJet> selectedJets;
  std::vector<NTJet> scaledJets;
  if(applyJES) scaledJets = GetScaledJets(scale);
  else scaledJets = jets;
  if(applyJER) scaledJets = GetSmearedJets(scaledJets, ResFactor);
  for(unsigned int i=0;i<scaledJets.size();i++){
        if(fabs(scaledJets[i].p4.Eta())<EtaThr && scaledJets[i].p4.Pt()>PtThr)
                selectedJets.push_back(scaledJets[i]);
  }
  std::sort(selectedJets.begin(),selectedJets.end(),HighestPt());
  return selectedJets;
}


//************************************************
//Bjets selection
//************************************************

std::vector<NTJet> Selection::GetSelectedBJets(const std::vector<NTJet>& SelectedJets, const int& algo, const float & discricut ) const{
   std::vector<NTJet> btagjets;
   for(unsigned int j=0;j<SelectedJets.size();j++){
         switch(algo){
              case 0 :
                      if(SelectedJets[j].TCDiscri>=discricut){
                            btagjets.push_back(SelectedJets[j]);
                      }
                      break;
              case 1 :
                      if(SelectedJets[j].SVDiscri>=discricut){
                            btagjets.push_back(SelectedJets[j]);
                      }
                      break;
              case 2 :
                      if(SelectedJets[j].SMDiscri>=discricut){
                            btagjets.push_back(SelectedJets[j]);
                      }
                      break;
              default:
                      cerr << "btagAlgo doesn't exist !"<<endl;
                      break;
          }
    }
    return btagjets;
}

//************************************************
//Electron selection
//************************************************

std::vector<NTElectron> Selection::GetScaledElectrons(float scale) const{
    std::vector<NTElectron> newElectrons = electrons;
     
    for (unsigned int i=0; i<newElectrons.size(); i++){
      
      if(scale > 1){
        if(newElectrons[i].isEB == 1) scale = 1.005;
        else scale = 1.0275;
      }
      //cout << "scale " << scale << endl;
      //cout << "newElectrons[i].p4.Pt() " << newElectrons[i].p4.Pt() << endl;
      double Px = 0;
      double Py = 0;
      double Pz = 0;
      double E = 0;
      Px   = scale*newElectrons[i].p4.Px();
      Py   = scale*newElectrons[i].p4.Py();
      Pz   = scale*newElectrons[i].p4.Pz();
      E   = scale*newElectrons[i].p4.E();
      newElectrons[i].p4.SetPxPyPzE(Px, Py, Pz, E);
      //cout << "newElectrons[i].p4.Pt() rescaled" << newElectrons[i].p4.Pt() << endl;
    }
    return newElectrons;
}


std::vector<NTElectron> Selection::GetSmearedElectrons(float resol) const{
    std::vector<NTElectron> newElectrons = electrons;
    
    //cout << "############ in smeared " << endl;
    for (unsigned int i=0; i<newElectrons.size(); i++){
      
      if(resol > 0){
        if(newElectrons[i].isEB == 1) resol = 0.25;
        else resol = 0.34;
      }
      
      //cout << "resol " << resol << endl;
      //cout << "********************newElectrons[i].p4.Pt() " << newElectrons[i].p4.Pt() << endl;
      
      double deltaR = -1;
      double gen_pt = -1; //to be changed
      for(unsigned int igenEl=0; igenEl<wAndDecays.size(); igenEl++){
        if(abs(wAndDecays[igenEl].mcLepId) != 11  ) continue;
        //cout << "wAndDecays[igenEl] "<< igenEl << "  pt "  << wAndDecays[igenEl].p4_Lep_gen.Pt() << endl;
        deltaR = wAndDecays[igenEl].p4_Lep_gen.DeltaR(newElectrons[i].p4);
        if(deltaR<0.1) gen_pt = wAndDecays[igenEl].p4_Lep_gen.Pt();
      
      }
      
      for(unsigned int igenEl=0; igenEl<zAndDecays.size(); igenEl++){
        if(zAndDecays[igenEl].Lep1_pdgID == 11 || zAndDecays[igenEl].Lep1_pdgID == -11){
          deltaR = zAndDecays[igenEl].p4_Lep1_gen.DeltaR(newElectrons[i].p4);
          if(deltaR<0.1) gen_pt = zAndDecays[igenEl].p4_Lep1_gen.Pt();
        }
        if(zAndDecays[igenEl].Lep2_pdgID == 11 || zAndDecays[igenEl].Lep2_pdgID == -11){
          deltaR = zAndDecays[igenEl].p4_Lep2_gen.DeltaR(newElectrons[i].p4);
          if(deltaR<0.1) gen_pt = zAndDecays[igenEl].p4_Lep2_gen.Pt();
        }
      
      }
      
      if( gen_pt > 0){
        double Px = 0;
        double Py = 0;
        double Pz = 0;
        double E = 0;
        double ele_pt = newElectrons[i].p4.Pt();
        double deltapt = (ele_pt - gen_pt) * resol;
        double ptscale = ((ele_pt + deltapt) / ele_pt);
        if(ptscale <0 ) ptscale = 0;
        Px   = ptscale*newElectrons[i].p4.Px();
        Py   = ptscale*newElectrons[i].p4.Py();
        Pz   = ptscale*newElectrons[i].p4.Pz();
        E   = ptscale*newElectrons[i].p4.E();
        newElectrons[i].p4.SetPxPyPzE(Px, Py, Pz, E);
      }
      //cout << "newElectrons[i].p4.Pt() smeared " << newElectrons[i].p4.Pt() << endl;
    }
    //cout << "############ end smeared " << endl;
    return newElectrons;
}





std::vector<NTElectron> Selection::GetSelectedElectronsNoIso(float PtThr, float EtaThr, bool applyLES, float scale,
        bool applyLER, float resol) const{
  std::vector<NTElectron> selectedElectrons;
  vector<NTElectron> localElectrons;
  
  
  if(applyLES ){ localElectrons = GetScaledElectrons(scale); }
  else{ if(!applyLER) localElectrons = electrons;}
  
  if(applyLER ) { localElectrons = GetSmearedElectrons(resol); }
  else{ if(!applyLES) localElectrons = electrons;}
  
  for(unsigned int i=0;i<localElectrons.size();i++){
    NTElectron electron = localElectrons[i];// to avoid const problem while calling NTLepton methods
    
    
    //cout << "electron.pT in sel " << electron.p4.Pt() << endl;
    //bool hadId(electron.elecIdWP90_r & 0x1);
    //bool isNotConv(electron.elecIdWP90_r & 0x4);    
    bool hadId(electron.elecIdWP90_c & 0x1);
    bool isNotConv(electron.elecIdWP90_c & 0x4);    
   
    //useless 
    if(!electron.isGsfElectron)                     continue; 
    if(!hadId)                                      continue; //hadId
    //old line: replaced by the following one
    //if(!isNotConv)                                  continue; //isNotConv
    if(electron.nlost >=2 || (fabs(electron.deltaCotTheta)<0.02 && fabs(electron.deltaDistance)<0.02)) continue;
    //if(electron.nlost <=2 || fabs(electron.deltaCotTheta)<=0.02 || fabs(electron.deltaDistance)<=0.02) continue;
    //old line:
    //if(electron.elspike>0.95)             continue;
    if(electron.isEcalDriven!=1)                    continue;
    if(fabs(electron.D0)       >=ElectronD0Cut_)    continue; 
    if(fabs(electron.p4.Eta()) >=EtaThr)            continue;
    
    
    if(electron.p4.Pt()        <=PtThr)             continue;
    if(electron.ET_SC          <=ElectronETSCThr_)  continue;
    
    
    bool SharedCone = false; //DR computed between localElectrons candidates and all Global or Tracker Muon
    for(unsigned int j=0;j<muons.size();j++){
        //if((muons[j].MuonType-(muons[j].MuonType/10)*10) ==0) continue; //isTrackerMuon
        if(muons[j].MuonType>=100 || (muons[j].MuonType-(muons[j].MuonType/10)*10) ==0){
                //if(electron.p4.DeltaR(muons[j].p4)<DRemuThr_){
                if(electron.p4.DeltaR(muons[j].p4)<0.1){
                        SharedCone = true;
                        break;
                }
        }
    }
    if(SharedCone)                      continue;
    if(GetSelectedVertex().size() == 0) continue;
    if ( fabs( electron.vertex.Z() - GetSelectedVertex()[0].p3.Z() ) > ElectronVertexMatchThr_ ) continue;
    selectedElectrons.push_back(localElectrons[i]);
  }
  std::sort(selectedElectrons.begin(),selectedElectrons.end(),HighestPt());
  return selectedElectrons;
}



std::vector<NTElectron> Selection::GetSelectedElectrons(float PtThr, float EtaThr, float ElectronRelIso, bool applyLES, float scale, 
       bool applyLER , float resol) const{
  std::vector<NTElectron> selectedElectrons;
  std::vector<NTElectron> electrons = GetSelectedElectronsNoIso(PtThr,EtaThr,applyLES,scale, applyLER, resol);
  for(unsigned int i=0;i<electrons.size();i++){
    NTElectron electron = electrons[i];// to avoid const problem while calling NTLepton methods
//    if((double) electron.CombinedRelIso03() > ElectronRelIso) continue;
    if((double) electron.RelIso03PF() > ElectronRelIso) continue;
    selectedElectrons.push_back(electrons[i]);
  }
  std::sort(selectedElectrons.begin(),selectedElectrons.end(),HighestPt());
  return selectedElectrons;
}


std::vector<NTElectron> Selection::GetSelectedElectronsLooseIDLooseIso(float PtThr, float EtaThr, bool applyLES, float scale) const{
  std::vector<NTElectron> selectedElectrons;
  vector<NTElectron> localElectrons;
  if(applyLES) localElectrons = GetScaledElectrons(scale); 
  else localElectrons = electrons;
  for(unsigned int i=0;i<localElectrons.size();i++){
  
    NTElectron electron = localElectrons[i];// to avoid const problem while calling NTLepton methods
   
    bool hadId(electron.elecIdWP90_c & 0x1);
    if(!hadId) continue;
    
    if((double) electron.RelIso03() > ElectronRelIso_) continue;
  
    if(GetSelectedVertex().size() == 0) continue;
    if ( fabs( electron.vertex.Z() - GetSelectedVertex()[0].p3.Z() ) > ElectronVertexMatchThr_ ) continue;
    selectedElectrons.push_back(localElectrons[i]);
  }
  std::sort(selectedElectrons.begin(),selectedElectrons.end(),HighestPt());
  return selectedElectrons;
}
   



std::vector<NTElectron> Selection::GetSelectedElectronsIsoNonID(bool applyLES, float scale,
   bool applyLER , float resol ) const{
  
  std::vector<NTElectron> selectedElectrons;
  vector<NTElectron> localElectrons;
  
  
  if(applyLES) localElectrons = GetScaledElectrons(scale); 
  else localElectrons = electrons;
  
  if(applyLER) localElectrons = GetSmearedElectrons(resol); 
  else localElectrons = electrons;
 
  
  for(unsigned int i=0;i<localElectrons.size();i++){
    NTElectron electron = electrons[i];// to avoid const problem while calling NTLepton methods
    if( electron.p4.Pt() < ElectronPtThreshold_)               continue;
    if( fabs(electron.p4.Eta() ) > ElectronEtaThreshold_)      continue;
//    if((double) electron.CombinedRelIso03() > ElectronRelIso_) continue;
    if((double) electron.RelIso03PF() > ElectronRelIso_) continue;
    selectedElectrons.push_back(localElectrons[i]);
  }
  std::sort(selectedElectrons.begin(),selectedElectrons.end(),HighestPt());
  return selectedElectrons;
}


std::vector<NTElectron> Selection::GetSelectedElectronsNonIsoNonID(bool applyLES, float scale, bool applyLER , float resol ) const{
  
  std::vector<NTElectron> selectedElectrons;
  vector<NTElectron> localElectrons;
  
  if(applyLES) localElectrons = GetScaledElectrons(scale); 
  else localElectrons = electrons;
  
  if(applyLER) localElectrons = GetSmearedElectrons(resol); 
  else localElectrons = electrons;
 
  for(unsigned int i=0;i<localElectrons.size();i++){
    NTElectron electron = electrons[i];// to avoid const problem while calling NTLepton methods
    if( electron.p4.Pt() < ElectronPtThreshold_)               continue;
    if( fabs(electron.p4.Eta() ) > ElectronEtaThreshold_)      continue;   
    if(!electron.isGsfElectron)                     continue; 
 
//    if((double) electron.CombinedRelIso03() > ElectronRelIso_) continue;
    selectedElectrons.push_back(localElectrons[i]);
  }
  std::sort(selectedElectrons.begin(),selectedElectrons.end(),HighestPt());
  return selectedElectrons;
}



std::vector<NTElectron> Selection::GetSelectedElectronsNoIso(bool applyLES, float scale, bool applyLER , float resol ) const{
        return GetSelectedElectronsNoIso(ElectronPtThreshold_, ElectronEtaThreshold_, applyLES, scale);
}

std::vector<NTElectron> Selection::GetSelectedElectrons(bool applyLES, float scale, bool applyLER, float resol ) const{
       
        return GetSelectedElectrons(ElectronPtThreshold_, ElectronEtaThreshold_, ElectronRelIso_, applyLES, scale, applyLER, resol);
}


std::vector<NTElectron> Selection::GetSelectedElectronsLooseIDLooseIso(bool applyLES, float scale) const{
        return GetSelectedElectronsLooseIDLooseIso(ElectronPtThreshold_, ElectronEtaThreshold_, applyLES, scale);
}

//************************************************
//Muon selection
//************************************************

std::vector<NTMuon> Selection::GetScaledMuons(float scale) const{
    std::vector<NTMuon> newMuons = muons;
    for (unsigned int i=0; i<newMuons.size(); i++){
      double Px = 0;
      double Py = 0;
      double Pz = 0;
      double E = 0;
      Px   = scale*newMuons[i].p4.Px();
      Py   = scale*newMuons[i].p4.Py();
      Pz   = scale*newMuons[i].p4.Pz();
      E   = scale*newMuons[i].p4.E();
      newMuons[i].p4.SetPxPyPzE(Px, Py, Pz, E);
    }
    return newMuons;
}
  

std::vector<NTMuon> Selection::GetSelectedMuonsNoIso(float PtThr, float EtaThr, bool applyLES, float scale) const{
  std::vector<NTMuon> selectedMuons;
  vector<NTMuon> localMuons;
  if(applyLES) localMuons = GetScaledMuons(scale); 
  else localMuons = muons;
  for(unsigned int i=0;i<localMuons.size();i++){
    NTMuon muon = localMuons[i];// to avoid const problem while calling NTLepton methods!
    if(muon.MuonType<100)                         continue; // isGlobalMuon
    if((muon.MuonType-(muon.MuonType/10)*10) ==0) continue; //isTrackerMuon
    if(muon.Chi2           >=MuonNormChi2_)         continue;
    if(muon.NTrValidHits   <=MuonNofValidTrHits_)   continue;
    if(muon.NValidHits     <=MuonNofValidHits_  )   continue;
    if(fabs(muon.D0Inner)  >=MuonD0Cut_)            continue;
    if(fabs(muon.p4.Eta()) >=EtaThr)                continue;
    if(muon.p4.Pt()<PtThr)                          continue;
    if(GetSelectedVertex().size() == 0)             continue;
    if ( fabs( muon.vertex.Z() - GetSelectedVertex()[0].p3.Z() ) > MuonVertexMatchThr_ ) continue;
    selectedMuons.push_back(localMuons[i]);
  }
  std::sort(selectedMuons.begin(),selectedMuons.end(),HighestPt());
  return selectedMuons;
}

std::vector<NTMuon> Selection::GetSelectedMuons(float PtThr, float EtaThr,float MuonRelIso, bool applyLES, float scale) const{
  std::vector<NTMuon> selectedMuons;
  std::vector<NTMuon> muons = GetSelectedMuonsNoIso(PtThr,EtaThr,applyLES,scale);
  for(unsigned int i=0;i<muons.size();i++){
    NTMuon muon = muons[i];// to avoid const problem while calling NTLepton methods!
//    if((double) muon.RelIso03() > MuonRelIso) continue;
    if((double) muon.RelIso03PF() > MuonRelIso) continue;
    selectedMuons.push_back(muons[i]);
  }
  std::sort(selectedMuons.begin(),selectedMuons.end(),HighestPt());
  return selectedMuons;
}


std::vector<NTMuon> Selection::GetSelectedMuonIsoNonID(bool applyLES, float scale) const{

  std::vector<NTMuon> selectedMuons;
  vector<NTMuon> localMuons;
  if(applyLES) localMuons = GetScaledMuons(scale); 
  else localMuons = muons;
  for(unsigned int i=0;i<localMuons.size();i++){
    NTMuon muon = localMuons[i];// to avoid const problem while calling NTLepton methods!
    if( muon.p4.Pt()            < MuonPtThreshold_)  continue;
    if( fabs(muon.p4.Eta())     > MuonEtaThreshold_) continue;
//    if((double) muon.RelIso03() > MuonRelIso_)       continue;
    if((double) muon.RelIso03PF() > MuonRelIso_)       continue;
    selectedMuons.push_back(localMuons[i]);
  }
  std::sort(selectedMuons.begin(),selectedMuons.end(),HighestPt());
  return selectedMuons;
  
}

std::vector<NTMuon> Selection::GetSelectedMuonsLooseIDLooseIso(float PtThr, float EtaThr, bool applyLES, float scale) const{
  std::vector<NTMuon> selectedMuons;
  vector<NTMuon> localMuons;
  if(applyLES) localMuons = GetScaledMuons(scale); 
  else localMuons = muons;
  for(unsigned int i=0;i<localMuons.size();i++){
    NTMuon muon = localMuons[i];// to avoid const problem while calling NTLepton methods!
    if(muon.MuonType<100)                         continue; // isGlobalMuon
    //if((muon.MuonType-(muon.MuonType/10)*10) ==0) continue; //isTrackerMuon
    //if(muon.Chi2           >=MuonNormChi2_)         continue;
    //if(muon.NTrValidHits   <=MuonNofValidTrHits_)   continue;
    //if(muon.NValidHits     <=MuonNofValidHits_  )   continue;
    //if(fabs(muon.D0Inner)  >=MuonD0Cut_)            continue;
    if(fabs(muon.p4.Eta()) >=EtaThr)                continue;
    if(muon.p4.Pt()<PtThr)                          continue;
    if((double) muon.RelIso03() > MuonRelIso_)      continue;
    if(GetSelectedVertex().size() == 0)             continue;
    if ( fabs( muon.vertex.Z() - GetSelectedVertex()[0].p3.Z() ) > MuonVertexMatchThr_ ) continue;
    selectedMuons.push_back(localMuons[i]);
  }
  std::sort(selectedMuons.begin(),selectedMuons.end(),HighestPt());
  return selectedMuons;
}


std::vector<NTMuon> Selection::GetSelectedMuonsNoIso(bool applyLES, float scale) const{
        return GetSelectedMuonsNoIso(MuonPtThreshold_, MuonEtaThreshold_, applyLES, scale);
}

std::vector<NTMuon> Selection::GetSelectedMuons(bool applyLES, float scale) const{
        return GetSelectedMuons(MuonPtThreshold_, MuonEtaThreshold_, MuonRelIso_, applyLES, scale);
}


std::vector<NTMuon> Selection::GetSelectedMuonsLooseIDLooseIso(bool applyLES, float scale) const{
        return GetSelectedMuonsLooseIDLooseIso(MuonPtThreshold_, MuonEtaThreshold_, applyLES, scale);
}


//************************************************
//Muon selection for muon+jets selection
//************************************************





std::vector<NTMuon> Selection::GetSelectedMuonsNoIsoForMuJets(float PtThr, float EtaThr, bool applyLES, float scale) const{
  std::vector<NTMuon> selectedMuons;
  vector<NTMuon> localMuons;
  if(applyLES) localMuons = GetScaledMuons(scale); 
  else localMuons = muons;
  for(unsigned int i=0;i<localMuons.size();i++){
    NTMuon muon = localMuons[i];// to avoid const problem while calling NTLepton methods!
    if(muon.MuonType<100)                           continue; // isGlobalMuon
    if((muon.MuonType-(muon.MuonType/10)*10) ==0)   continue; //isTrackerMuon
    if(muon.Chi2           >=MuonNormChi2_)         continue;
    if(muon.NTrValidHits   <=MuonNofValidTrHits_)   continue;
    if(muon.NValidHits     <=MuonNofValidHits_  )   continue;
    if(fabs(muon.D0Inner)  >=MuonD0Cut_)            continue;
    if(fabs(muon.p4.Eta()) >=EtaThr)                continue;
    if(muon.p4.Pt()<PtThr)                          continue;
    if(GetSelectedVertex().size() == 0)             continue;
    if ( fabs( muon.vertex.Z() - GetSelectedVertex()[0].p3.Z() ) > MuonVertexMatchThr_ ) continue;
    selectedMuons.push_back(localMuons[i]);
  }
  std::sort(selectedMuons.begin(),selectedMuons.end(),HighestPt());
  return selectedMuons;
}

std::vector<NTMuon> Selection::GetSelectedMuonsForMuJets(float PtThr, float EtaThr,float MuonRelIso, bool applyLES, float scale) const{
  std::vector<NTMuon> selectedMuons;
  std::vector<NTMuon> muons = GetSelectedMuonsNoIso(PtThr,EtaThr,applyLES,scale);
  for(unsigned int i=0;i<muons.size();i++){
    NTMuon muon = muons[i];// to avoid const problem while calling NTLepton methods!
//    if((double) muon.RelIso03() > MuonRelIso) continue;
    if((double) muon.RelIso03PF() > MuonRelIso) continue;
    selectedMuons.push_back(muons[i]);
  }
  std::sort(selectedMuons.begin(),selectedMuons.end(),HighestPt());
  return selectedMuons;
}



std::vector<NTMuon> Selection::GetSelectedMuonsForMuJets(bool applyLES, float scale) const{
        return GetSelectedMuonsForMuJets(MuonPtThreshold_, MuonEtaThreshold_, MuonRelIso_, applyLES, scale);
}




std::vector<NTMuon>    Selection::GetSelectedLooseMuonsForMuJets(bool applyLES, float scale) const{
 std::vector<NTMuon> selectedLooseMuons;
  vector<NTMuon> localMuons;
  if(applyLES) localMuons = GetScaledMuons(scale); 
  else localMuons = muons;
  for(unsigned int i=0; i < localMuons.size();i++){
    NTMuon muon = localMuons[i];
//    if(fabs(muon.p4.Eta()) < 2.5 && muon.p4.Pt() > 10 && (double) muon.RelIso03() > 0.2)     
    if(fabs(muon.p4.Eta()) < 2.5 && muon.p4.Pt() > 10 && (double) muon.RelIso03PF() > 0.2)     
    selectedLooseMuons.push_back(localMuons[i]);
  }
 
 return selectedLooseMuons;

}


std::vector<NTElectron>    Selection::GetSelectedLooseElectronsForMuJets(bool applyLES, float scale) const{

std::vector<NTElectron> selectedLooseElectrons;
  vector<NTElectron> localElectrons;
  if(applyLES) localElectrons = GetScaledElectrons(scale); 
  else localElectrons = electrons;
  for(unsigned int i=0;i<localElectrons.size();i++){
    NTElectron elec = localElectrons[i];
//    if(fabs(elec.p4.Eta()) < 2.5 && elec.p4.Pt() > 15 && (double) elec.RelIso03() > 0.2)     
    if(fabs(elec.p4.Eta()) < 2.5 && elec.p4.Pt() > 15 && (double) elec.RelIso03PF() > 0.2)     
    selectedLooseElectrons.push_back(localElectrons[i]);
  }
  return selectedLooseElectrons;
}



//************************************************
//Tau selection
//************************************************

std::vector<NTTau> Selection::GetScaledTaus(float scale) const{
    std::vector<NTTau> newTaus = taus;
    for (unsigned int i=0; i<newTaus.size(); i++){
      double Px = 0;
      double Py = 0;
      double Pz = 0;
      double E = 0;
      Px   = scale*newTaus[i].p4.Px();
      Py   = scale*newTaus[i].p4.Py();
      Pz   = scale*newTaus[i].p4.Pz();
      E   = scale*newTaus[i].p4.E();
      newTaus[i].p4.SetPxPyPzE(Px, Py, Pz, E);
    }
    return newTaus;
}

std::vector<NTTau> Selection::GetSelectedTaus(float PtThr, float EtaThr, bool applyLES, float scale, int isoLevel, bool antiLep) const{
 std::vector<NTTau> selectedTaus;
 std::vector<NTTau> localTaus;
   
 if(applyLES) localTaus = GetScaledTaus(scale);
 else localTaus = taus;
  
 for(unsigned int i=0;i<localTaus.size();i++){
   NTTau tau = localTaus[i];
   
   if ( tau.leadTrackPt    <= TauLeadTrkPtCut_ ) continue;
   //if ( tau.GetDiscriminator("ByDecayModeFinding")  == 1) continue;
   //if ( isoLevel ==0 &&  tau.GetDiscriminator("byLooseIsolation")  != 1)   continue;
   //if ( isoLevel ==1 &&  tau.GetDiscriminator("byMediumIsolation") != 1)   continue;
   //if ( isoLevel ==2 &&  tau.GetDiscriminator("byTightIsolation")  != 1)   continue;
   //if ( tau.GetDiscriminator("ByDecayModeFinding")  == 1) continue;
   
   if ( isoLevel ==0 &&  tau.GetDiscriminator("byLooseCombinedIsolationDeltaBetaCorr")  != 1)   continue;
   if ( isoLevel ==1 &&  tau.GetDiscriminator("byMediumCombinedIsolationDeltaBetaCorr") != 1)   continue;
   if ( isoLevel ==2 &&  tau.GetDiscriminator("byTightCombinedIsolationDeltaBetaCorr")  != 1)   continue;
 
   //if ( tau.discrByTaNC  != 1                ) continue;
   if(antiLep)
   { //std::cout <<tau.GetDiscriminator("againstElectronTight")<<" " << tau.GetDiscriminator("againstMuonTight")<<  std::endl;
    if ( tau.GetDiscriminator("againstElectronTight") != 1) continue;
    if ( tau.GetDiscriminator("againstMuonTight") != 1)     continue;
    }
   //if(tau.decayMode<10) continue;
    
   if(fabs(tau.p4.Eta())>=EtaThr)                continue;
   if(fabs(tau.p4.Eta())<1.566 && fabs(tau.p4.Eta())>1.4442)continue;
   if(tau.p4.Pt()<PtThr)                         continue;
   if(GetSelectedVertex().size() == 0)           continue;
   if( fabs( tau.vertex.Z() - GetSelectedVertex()[0].p3.Z() ) > TauVertexMatchThr_ ) continue;
   if( fabs( tau.D0)  >= 0.04 )                 continue;
   selectedTaus.push_back(localTaus[i]);
 }
 std::sort(selectedTaus.begin(),selectedTaus.end(),HighestPt());
 return selectedTaus;
}


std::vector<NTTau> Selection::GetTaus(std::vector<NTMuon> muon_cand, std::vector<NTElectron> elec_cand) const{
  
  std::vector<NTTau> cleanedTaus;
   
  for(unsigned int i=0;i<taus.size();i++){
    //NTTau tau = taus[i];
    double deltaRmu = 10000;
    double deltaRel = 10000;
     
    for(unsigned int imu=0; imu< muon_cand.size(); imu++){
      double deltaR = taus[i].p4.DeltaR(muon_cand[imu].p4);
      if(deltaR < deltaRmu) deltaRmu = deltaR;
    }
    
    for(unsigned int iel=0; iel< elec_cand.size(); iel++){
      double deltaR = taus[i].p4.DeltaR(elec_cand[iel].p4);
      if(deltaR < deltaRel) deltaRel = deltaR;
    }
    
    if( deltaRmu > 0.4  && deltaRel > 0.4) cleanedTaus.push_back(taus[i]); 
  }
  std::sort(cleanedTaus.begin(),cleanedTaus.end(),HighestPt());
  return cleanedTaus;
}

std::vector<NTTau> Selection::GetSelectedTaus(bool applyLES, float scale, int isoLevel, bool antiLep) const{
        return GetSelectedTaus(TauPtThreshold_, TauEtaThreshold_, applyLES, scale, isoLevel, antiLep);
}
std::vector<NTTau> Selection::GetSelectedTaus(std::vector<NTMuon> muon_cand, std::vector<NTElectron> elec_cand, float PtThr, float EtaThr, bool applyLES, float scale, int isoLevel, bool antiLep) const{
  
  std::vector<NTTau> selectedTaus = GetSelectedTaus(PtThr,EtaThr,applyLES,scale,isoLevel,antiLep);
  std::vector<NTTau> cleanTaus;
  
  for(unsigned int i=0; i< selectedTaus.size(); i++){
  
     double deltaRmu = 10000;
     double deltaRel = 10000;
     
     for(unsigned int imu=0; imu< muon_cand.size(); imu++){
       double deltaR = selectedTaus[i].p4.DeltaR(muon_cand[imu].p4);
       if(deltaR < deltaRmu) deltaRmu = deltaR;
     }
     
     for(unsigned int iel=0; iel< elec_cand.size(); iel++){
       double deltaR = selectedTaus[i].p4.DeltaR(elec_cand[iel].p4);
       if(deltaR < deltaRel) deltaRel = deltaR;
     }
     
     if( deltaRmu > 0.4  && deltaRel > 0.4) cleanTaus.push_back(selectedTaus[i]);
  }
  return cleanTaus;
}

std::vector<NTTau> Selection::GetSelectedTaus(std::vector<NTMuon> muon_cand, std::vector<NTElectron> elec_cand, bool applyLES, float scale, int isoLevel, bool antiLep) const{
        return GetSelectedTaus(muon_cand, elec_cand, TauPtThreshold_, TauEtaThreshold_, applyLES, scale, isoLevel, antiLep);
}

bool Selection::isAnEventSelected(unsigned int nElectrons, unsigned int nMuons, unsigned int nTaus, unsigned int nJets){
        std::vector<NTMuon> muon_cand     = GetSelectedMuons();
        std::vector<NTElectron> elec_cand = GetSelectedElectrons(); 
        
        if(elec_cand.size() < nElectrons) return false;
        if(muon_cand.size() < nMuons)         return false;
        if(GetSelectedTaus().size() < nTaus)           return false;
        if(GetSelectedJets(muon_cand, elec_cand).size() < nJets)           return false;
        return true;
}


//************************************************
//Weight for B-tag 
//************************************************

void Selection::InitSFBWeight (int flagb, int methodb, int systb, int btagAlgo_, float btagDiscriCut_, int NofBtagJets_)
{
  flag_btagweight_ = flagb;
  methodb_ = methodb;
  systb_ = systb;
  //is it usefull if methodb=2??
  sfb_.SFBinit (btagAlgo_, btagDiscriCut_, NofBtagJets_);
  if (flagb>0) {
    sfb_.LoadInfo();
    sfb_.LoadInfo2();
    sfb_.InitAlgoAndWP(btagAlgo_, btagDiscriCut_);
  }
}

void Selection::ReInitSFBWeight (int flagb, int methodb, int systb, int btagAlgo_, float btagDiscriCut_, int NofBtagJets_)
{
  //cout<<"Enter in Selection::ReInitSFBWeight"<<endl;
  flag_btagweight_ = flagb;
  methodb_ = methodb;
  systb_ = systb;
  //is it usefull if methodb=2 ??
  sfb_.SFBinit (btagAlgo_, btagDiscriCut_, NofBtagJets_);
  if (flagb>0) {
    //sfb_.LoadInfo();
    //sfb_.LoadInfo2();
    sfb_.InitAlgoAndWP(btagAlgo_, btagDiscriCut_);
  }
}

void Selection::ResetParameters4Bweight (int flagb, int methodb, int systb)
{
  flag_btagweight_ = flagb;
  methodb_ = methodb;
  systb_ = systb;

  if (sfb_.GetHistoSFB()==0) {
    sfb_.LoadInfo();
    sfb_.LoadInfo2();
  }
}

int Selection::GetFlagb () const
{
     return flag_btagweight_;
}

int Selection::GetMethodb () const
{
     return methodb_;
}
int Selection::GetSystb () const
{
     return systb_;
}
const SFBweight& Selection::GetSFBweight() const
{
     return sfb_;
}

vector<double> Selection::generate_flat10_weights(TH1D* data_npu_estimated){

    // distri verte
    // see SimGeneral/MixingModule/python/mix_E7TeV_FlatDist10_2011EarlyData_inTimeOnly_cfi.py; copy and paste from there: 
    const double npu_probs[25] = {0.0698146584, 0.0698146584, 0.0698146584,0.0698146584,0.0698146584,0.0698146584,0.0698146584,0.0698146584,0.0698146584,0.0698146584,0.0698146584,
           0.0630151648,0.0526654164,0.0402754482,0.0292988928,0.0194384503,0.0122016783,0.007207042,0.004003637,0.0020278322,
           0.0010739954,0.0004595759,0.0002229748,0.0001028162,4.58337152809607E-05 };


    // see SimGeneral/MixingModule/python/mix_E7TeV_Summer_2011_50ns_PoissonOOT.py for Summer11
    // const double npu_probs[25] = {0.0400676665,0.040358009,0.0807116334,0.0924154156,0.0924154156,0.0924154156,0.0924154156,0.0924154156,0.0870356742,0.0767913175,0.0636400516,0.0494955563,0.036223831,0.0249767088,0.0162633216,0.0099919945,0.0058339324,0.0032326433,0.0017151846,0.0008505404,0.0004108859,0.0001905137,0.0000842383,0.000034939,0.0000142801};

/*
    // distri bleue
    // info taken on ttbar mc (spring11 madgraph - no skim) : /opt/sbg/data/data1/cms/ccollard/CMSSW/fichier_root2011/pu_in_mc.C  
    const double npu_probs[25] = {0.0795275, 0.0761536, 0.069365, 0.0730743, 0.0699924, 0.075654, 0.076423, 0.0684544, 0.0732846, 0.0748051, 0.0669443, 0.0582711, 0.0484596, 0.0348441, 0.0204753, 0.0157868, 0.00964813, 0.00537668, 0.00266835, 0.000791556, 0, 0, 0, 0, 0};
*/

    vector<double> result(25);
    double s = 0.0;
    for(int inpu=0; inpu<25; ++inpu){
        double npu_estimated = data_npu_estimated->GetBinContent(data_npu_estimated->GetXaxis()->FindBin(inpu));                              
        if (npu_probs[inpu]>0) {result[inpu] = npu_estimated / npu_probs[inpu]; }
        else {
          result[inpu] = 0.;
        }
        s += npu_estimated;
    }
    // normalize weights such that the total sum of weights over thw whole sample is 1.0, i.e., sum_i  result[i] * npu_probs[i] should be 1.0 (!)
    for(int inpu=0; inpu<25; ++inpu){
        result[inpu] /= s;
    }
    return result;
}


void Selection::GeneratePUWeight(string filename){
        TFile file(filename.c_str(),"READ");
        TH1D* histo = 0;
        histo = (TH1D*) file.Get("pileup");
        // get TH1D* histo from the root file created by estimatePileupD above
        PUWeights = generate_flat10_weights(histo);
}

float Selection::GetPUWeight(){
        // get the number of true pileup events from MC in n_pu
        if(npu<(int)PUWeights.size()){
            return PUWeights[npu];
        }
        else{ //should not happen as we have a weight for all simulated n_pu multiplicities!
           cerr<<"n_pu too big"<<endl;
           return 1.;
        }
}

void Selection::LoadElScaleFactors(){
  
  //TFile *f_Data_El = new TFile("/opt/sbg/data/data1/cms/jandrea/TopIPHC_2011_09_16_testProff/CMSSW_4_2_5/src/MiniTreeAnalysis/NTupleAnalysis/macros/electronSF.root");
  TFile *f_Data_El = new TFile("/opt/sbg/data/data1/cms/ccollard/CMSSW/CMSSW_4_2_5/src/MiniTreeAnalysis/NTupleAnalysis/macros/data/electronSF.root");
  f_Data_El->cd();
  scaleFactEl = (TH2F*)gROOT->FindObject("thehistSF")->Clone("");
  scaleFactEl->SetDirectory(0);
  f_Data_El->Close();
  delete f_Data_El;
  



} 
void Selection::LoadMuScaleFactors(){


  //TFile *f_Data_Mu = new TFile("/opt/sbg/data/data1/cms/jandrea/TopIPHC_2011_09_16_testProff/CMSSW_4_2_5/src/MiniTreeAnalysis/NTupleAnalysis/macros/muonSF.root"); 
  TFile *f_Data_Mu = new TFile("/opt/sbg/data/data1/cms/ccollard/CMSSW/CMSSW_4_2_5/src/MiniTreeAnalysis/NTupleAnalysis/macros/data/muonSF.root");
  f_Data_Mu->cd();
  scaleFactMu = (TH2F*)gROOT->FindObject("thehistSF")->Clone("");
  scaleFactMu->SetDirectory(0);
  f_Data_Mu->Close();
  delete f_Data_Mu;


} 

void Selection::InitJESUnc ()
{

    //TFile* f1 = new TFile("/opt/sbg/data/data1/cms/gele/PAT/423/V_prod/CMSSW_4_2_3/src/JESUncertaintiesPlotProduction/JESUncertaintiesPlotProduction/JESUncMC.root");
    TFile* f1 = new TFile("../../macros/data/JESUncMC.root");
    
    f1->cd();
    histo_jesunc_    = (TH2F*) f1->Get("JESUncHisto")->Clone("JESUncHisto") ;
    std::cout<<"Reading the JES Uncertainty file "<<histo_jesunc_->GetEntries()<<std::endl;
    histo_jesunc_->SetDirectory(0); // 
    f1->Close();
    delete f1;


}














