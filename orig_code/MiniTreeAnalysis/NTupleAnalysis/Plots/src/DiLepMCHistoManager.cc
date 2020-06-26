
#include "../interface/DiLepMCHistoManager.h"




DiLepMCHistoManager::DiLepMCHistoManager(){
}

DiLepMCHistoManager::~DiLepMCHistoManager(){
}


void DiLepMCHistoManager::CreateHistos(){
        AddHisto(string("TMEME"),string("TMEME"),string("TMEME"),22222,0,22222);        
}

void DiLepMCHistoManager::Fill(NTEvent* event, const int& maxSelStep, const int& iChannel, const int& iDataset, const float& weight){
        if(!Check(iChannel, iDataset)) return;
        for(unsigned int i=0;i<SelectionSteps.size();i++){
                if(maxSelStep>=(int)i) FillSelStep(event, i , iChannel, iDataset, weight);      
        }       
}

void DiLepMCHistoManager::FillSelStep(NTEvent* event, const int& iSelStep, const int& iChannel, const int& iDataset, const float& weight){
        if(!Check(iChannel, iSelStep, iDataset, 0) ) return;
        Histos[0][iChannel][iSelStep][iDataset].Fill(event->TMEME,weight);
}


