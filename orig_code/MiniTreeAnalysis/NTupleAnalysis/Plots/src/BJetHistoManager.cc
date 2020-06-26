
#include "../interface/BJetHistoManager.h"




BJetHistoManager::BJetHistoManager(){
}

BJetHistoManager::~BJetHistoManager(){
}


void BJetHistoManager::CreateHistos(){
        AddHisto(string("Multiplicity"),string("#(Bjets)"),string("Nof Bjets"),4,0,4);  
        AddHisto(string("NoweightInfo"),string("#(Bjets)"),string("Nof Bjets"),4,0,4);  
        AddHisto(string("TheWeight"),string("weight_b"),string("weight_b"),100.,-4.,4.);        
}

void BJetHistoManager::Fill(const size_t& nbtagjet, const int& maxSelStep, const int& iChannel, const int& iDataset, const float& weight,const float& weightb){
        if(!Check(iChannel, iDataset)) return;
        for(unsigned int i=0;i<SelectionSteps.size();i++){
                if(maxSelStep>=(int)i) FillSelStep(nbtagjet, i , iChannel, iDataset, weightb);  
                if(maxSelStep>=(int)i) FillTemporary(nbtagjet, i , iChannel, iDataset, weight); 
        }       
}

void BJetHistoManager::FillSelStep(const size_t& nbtagjet, const int& iSelStep, const int& iChannel, const int& iDataset, const float& weight){
        if(!Check(iChannel, iSelStep, iDataset, 0) ) return;
        size_t valtoplot=nbtagjet;
        if (valtoplot>3) valtoplot=3;
        Histos[0][iChannel][iSelStep][iDataset].Fill(valtoplot,weight);
        Histos[2][iChannel][iSelStep][iDataset].Fill(weight);
}

void BJetHistoManager::FillTemporary(const size_t& nbtagjet, const int& iSelStep, const int& iChannel, const int& iDataset, const float& weight){
        if(!Check(iChannel, iSelStep, iDataset, 0) ) return;
        size_t valtoplot=nbtagjet;
        if (valtoplot>3) valtoplot=3;
        Histos[1][iChannel][iSelStep][iDataset].Fill(valtoplot,weight);
}


