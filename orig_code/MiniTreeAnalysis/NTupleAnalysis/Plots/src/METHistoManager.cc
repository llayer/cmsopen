#include "../interface/METHistoManager.h"




METHistoManager::METHistoManager(){
}

METHistoManager::~METHistoManager(){
}


void METHistoManager::CreateHistos(){
        AddHisto(string("Et"),string("Et(met)"),string("E_{T}(met) (GeV)"),100,0,400);  
        AddHisto(string("Px"),string("Px(met)"),string("Px(met)"),50,0,100);    
        AddHisto(string("Py"),string("Py(met)"),string("Py(met)"),50,0,100);    
        AddHisto(string("Phi"),string("Phi(met)"),string("#phi(met)"),64,-3.2,3.2);     
}

void METHistoManager::Fill(NTMET met, const int& maxSelStep, const int& iChannel, const int& iDataset, const float& weight){
        if(!Check(iChannel, iDataset)) return;
        for(unsigned int i=0;i<SelectionSteps.size();i++){
                if(maxSelStep>=(int)i) FillSelStep(met, i , iChannel, iDataset, weight);        
        }       
}

void METHistoManager::FillSelStep(NTMET met, const int& iSelStep, const int& iChannel, const int& iDataset, const float& weight){
        if(!Check(iChannel, iSelStep, iDataset, 0) ) return;
        //respect the order of the function CreateHistos to fill the histograms
        Histos[0][iChannel][iSelStep][iDataset].Fill(met.p4.Et(),weight);
        Histos[1][iChannel][iSelStep][iDataset].Fill(met.p4.Px(),weight);
        Histos[2][iChannel][iSelStep][iDataset].Fill(met.p4.Py(),weight);
        Histos[3][iChannel][iSelStep][iDataset].Fill(met.p4.Phi(),weight);
}


