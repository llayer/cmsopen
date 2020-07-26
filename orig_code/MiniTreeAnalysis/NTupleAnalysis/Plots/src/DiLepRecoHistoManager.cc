#include "../interface/DiLepRecoHistoManager.h"




DiLepRecoHistoManager::DiLepRecoHistoManager(){
}

DiLepRecoHistoManager::~DiLepRecoHistoManager(){
}


void DiLepRecoHistoManager::CreateHistos(){
        AddHisto(string("DiLepMass"),string("DiLepMass"),string("Di-lepton mass [GeV]"),100,0,800);     
        AddHisto(string("DiLepMassOppSign"),string("DiLepMass-opposite sign"),string("Di-lepton mass [GeV]"),100,0,800);        
        AddHisto(string("DiLepMassSameSign"),string("DiLepMass-same sign"),string("Di-lepton mass [GeV]"),100,0,800);   
        AddHisto(string("MTLeptons"),string("MT(leptons)"),string("M_{T}(di-leptons)"),100,0,800);      
        AddHisto(string("DeltaPhiLeptons"),string("DeltaPhi(leptons)"),string("#DeltaPhi(di-leptons)"),60,-3,3);        
}

void DiLepRecoHistoManager::Fill(NTEvent* event, const vector<NTMuon>& candMuon, const vector<NTElectron>& candElec, const int& maxSelStep, const int& iChannel, const int& iDataset, const float& weight){
        if(!Check(iChannel, iDataset)) return;
        for(unsigned int i=0;i<SelectionSteps.size();i++){
                if(maxSelStep>=(int)i) FillSelStep(event, candMuon, candElec, i , iChannel, iDataset, weight);  
        }       
}

void DiLepRecoHistoManager::FillSelStep(NTEvent* event, const vector<NTMuon>& candMuon, const vector<NTElectron>& candElec, const int& iSelStep, const int& iChannel, const int& iDataset, const float& weight){
        if(!Check(iChannel, iSelStep, iDataset, 0) ) return;
        DiLeptonSelection sel;
        float mass = sel.DiLeptonMass(candMuon,candElec);
        float mt = sel.DiLeptonMass(candMuon,candElec);
        float DPhi = -999;
        bool isSameSign = false;
        if(candMuon.size()==2) {
                DPhi = candMuon[0].p4.Phi()-candMuon[1].p4.Phi();       
                if(candMuon[0].Charge == candMuon[1].Charge) isSameSign = true;
        }
        if(candElec.size()==2){
                DPhi = candElec[0].p4.Phi()-candElec[1].p4.Phi();       
                if(candElec[0].Charge == candElec[1].Charge) isSameSign = true;
        }
        if(candMuon.size()==1 && candElec.size()==1){
                DPhi = candMuon[0].p4.Phi()-candElec[0].p4.Phi();
                if(candMuon[0].Charge == candElec[0].Charge) isSameSign = true;
        }

        Histos[0][iChannel][iSelStep][iDataset].Fill(mass,weight);
        if(!isSameSign) Histos[1][iChannel][iSelStep][iDataset].Fill(mass,weight);
        if(isSameSign) Histos[2][iChannel][iSelStep][iDataset].Fill(mass,weight);
        Histos[3][iChannel][iSelStep][iDataset].Fill(mt,weight);
        Histos[4][iChannel][iSelStep][iDataset].Fill(DPhi,weight);
}



