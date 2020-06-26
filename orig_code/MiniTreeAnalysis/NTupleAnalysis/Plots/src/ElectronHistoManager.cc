#include "../interface/ElectronHistoManager.h"




ElectronHistoManager::ElectronHistoManager(){
}

ElectronHistoManager::~ElectronHistoManager(){
}


void ElectronHistoManager::CreateHistos(){
        AddHisto(string("Multiplicity"),string("#(electrons)"),string("Nof electrons"),10,0,10);        
        AddHisto(string("Pt"),string("Pt(electron)"),string("p_{T}(electron)"),50,0,100);       
        AddHisto(string("Eta"),string("Eta(electron)"),string("#eta(electron)"),60,-3,3);       
        AddHisto(string("Phi"),string("Phi(electron)"),string("#phi(electron)"),64,-3.2,3.2);   
        AddHisto(string("d0"),string("d0(electron)"),string("d0(electron)"),50,0,2);    
        AddHisto(string("charge"),string("charge(electron)"),string("charge(electron)"),5,-2,2);        
        AddHisto(string("trackIso"),string("trackIso(electron)"),string("trackIso(electron)"),50,0,10); 
        AddHisto(string("caloIso"),string("caloIso(electron)"),string("caloIso(electron)"),50,0,10);    
        AddHisto(string("hcalIso"),string("hcalIso(electron)"),string("hcalIso(electron)"),50,0,10);    
        AddHisto(string("relIso"),string("relIso(electron)"),string("relIso(electron)"),50,0,10);       
}

void ElectronHistoManager::Fill(const vector<NTElectron>& electrons, const int& maxSelStep, const int& iChannel, const int& iDataset, const float& weight){
        if(!Check(iChannel, iDataset)) return;
        for(unsigned int i=0;i<SelectionSteps.size();i++){
                if(maxSelStep>=(int)i) FillSelStep(electrons, i , iChannel, iDataset, weight);  
        }       
}

void ElectronHistoManager::FillSelStep(const vector<NTElectron>& electrons, const int& iSelStep, const int& iChannel, const int& iDataset, const float& weight){
        if(!Check(iChannel, iSelStep, iDataset, 0) ) return;
        Histos[0][iChannel][iSelStep][iDataset].Fill(electrons.size(),weight);
        for(int i=0;i<(int) electrons.size();i++){
                //very important:
                //respect the order of the function CreateHistos to fill the histograms
                Histos[1][iChannel][iSelStep][iDataset].Fill(electrons[i].p4.Pt(),weight);
                Histos[2][iChannel][iSelStep][iDataset].Fill(electrons[i].p4.Eta(),weight);
                Histos[3][iChannel][iSelStep][iDataset].Fill(electrons[i].p4.Phi(),weight);
                Histos[4][iChannel][iSelStep][iDataset].Fill(electrons[i].D0,weight);
                Histos[5][iChannel][iSelStep][iDataset].Fill(electrons[i].Charge,weight);
                Histos[6][iChannel][iSelStep][iDataset].Fill(electrons[i].TrkIso03,weight);
                Histos[7][iChannel][iSelStep][iDataset].Fill(electrons[i].ECaloIso03,weight);
                Histos[8][iChannel][iSelStep][iDataset].Fill(electrons[i].HCaloIso03,weight);
                //Histos[9][iChannel][iSelStep][iDataset].Fill(electrons[i].CombinedRelIso03(),weight);
                NTElectron el = electrons[i];
                float relIso = el.CombinedRelIso03();
                Histos[9][iChannel][iSelStep][iDataset].Fill(relIso,weight);
        }
}


