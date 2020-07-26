#include "../interface/EventShapesHistoManager.h"




EventShapesHistoManager::EventShapesHistoManager(){
}

EventShapesHistoManager::~EventShapesHistoManager(){
}


void EventShapesHistoManager::CreateHistos(){
        AddHisto(string("Aplanarity"),string("Aplanarity"),string("Aplanarity"),25,0,0.5);
        AddHisto(string("Sphericity"),string("Sphericity"),string("Sphericity"),25,0,1);
        AddHisto(string("Circularity"),string("Circularity"),string("Circularity"),50,0,1);
        AddHisto(string("Isotropy"),string("Isotropy"),string("Isotropy"),50,0,1);
        AddHisto(string("C"),string("C"),string("C"),50,0,1);
        AddHisto(string("D"),string("D"),string("D"),50,0,1);
        AddHisto(string("Ht"),string("HT"),string("H_{T} (GeV)"),60,0,1800);
        AddHisto(string("H"),string("H"),string("H"),200,0,1000);
        AddHisto(string("sqrts"),string("M(#tau,jets)"),string("M(#tau,jets) (GeV)"),110,0,2200);
        AddHisto(string("NNOutput"),string("NNOutput"),string("NNOutput"),30,-1.5,1.5);
        AddHisto(string("Chi2"),string("Chi2"),string("Chi2"),200,0,1000);
        AddHisto(string("Chi2zoom"),string("Chi2zoom"),string("Chi2"),40,0,20);
        AddHisto(string("Chi2NN"),string("Chi2 NN>0.5"),string("Chi2 NN>0.5"),200,0,1000);
        AddHisto(string("HtNN"),string("H_{T} NN>0.5"),string("H_{T} NN>0.5"),50,0,1500);
        AddHisto(string("AplanarityNN"),string("Aplanarity NN>0.5"),string("Aplanarity NN>0.5"),50,0,1.);
        AddHisto(string("SphericityNN"),string("Sphericity NN>0.5"),string("Sphericity NN>0.5"),25,0,1);
        AddHisto(string("CircularityNN"),string("Circularity NN>0.5"),string("Circularity NN>0.5"),50,0,1);
        AddHisto(string("M3Chi2"),string("M3Chi2"),string("M3 (GeV)"),140,0,700);
        AddHisto(string("M2Chi2"),string("M2Chi2"),string("M2 (GeV)"),200,0,1000);
        AddHisto(string("M3Chi2NN"),string("M3Chi2 NN>0.5"),string("M3Chi2 NN>0.5"),200,0,1000);
        AddHisto(string("M2Chi2NN"),string("M2Chi2 NN>0.5"),string("M2Chi2 NN>0.5"),200,0,1000);
        AddHisto(string("CNN"),string("C NN>0.5"),string("C NN>0.5"),50,0,1);
        AddHisto(string("DNN"),string("D NN>0.5"),string("D NN>0.5"),50,0,1);
        AddHisto(string("sqrtsNN"),string("M(#tau,jets) NN>0.5"),string("M(#tau,jets) NN>0.5"),100,0,2000);
        AddHisto(string("kinFitChi2"),string("kinFitChi2"),string("kinFit #chi^{2}"),100,0,100);
        AddHisto(string("kinFitTopMass"),string("kinFit top mass"),string("reconstructed top mass (GeV)"),200,0,600);
        AddHisto(string("kinFitChi2NN"),string("kinFitChi2 NN>0.5"),string("kinFit #chi_2 NN>0.5"),200,-100,100);
        AddHisto(string("kinFitTopMassNN"),string("kinFit top mass NN>0.5"),string("kinFit top mass NN>0.5"),150,-100,500);
        AddHisto(string("kinFitTopMassChi2"),string("kinFit top mass chi2<1"),string("kinFit top mass #chi_2<1"),150,-100,500);
        AddHisto(string("NNOutput_bin15"),string("NNOutput_bin15"),string("NNOutput"),15,-1.5,1.5);
        AddHisto(string("NNOutput_bin20"),string("NNOutput_bin20"),string("NNOutput"),20,-1.5,1.5);
        AddHisto(string("NNOutput_bin45"),string("NNOutput_bin45"),string("NNOutput"),45,-1.5,1.5);
        AddHisto(string("NNOutput_bin60"),string("NNOutput_bin60"),string("NNOutput"),60,-1.5,1.5);
        AddHisto(string("NNOutput_bin25"),string("NNOutput_bin25"),string("NNOutput"),25,-1.5,1.5);
        AddHisto(string("NNOutput_bin35"),string("NNOutput_bin35"),string("NNOutput"),35,-1.5,1.5);
        AddHisto(string("NNOutput_bin40"),string("NNOutput_bin40"),string("NNOutput"),40,-1.5,1.5);
        AddHisto(string("NNOutput_bin50"),string("NNOutput_bin50"),string("NNOutput"),50,-1.5,1.5);
        AddHisto(string("NNOutput_bin55"),string("NNOutput_bin55"),string("NNOutput"),55,-1.5,1.5);
        AddHisto(string("kinFitProba"),string("kinFitProba"),string("kinFit probability"),50,0,1);
        AddHisto(string("M3"),string("M3"),string("M3 (GeV)"),100,0,700);
        AddHisto(string("NNOutput_1"),string("NNOutput_1"),string("NNOutput_1"),30,-1.5,1.5);
        AddHisto(string("NNOutput_2"),string("NNOutput_2"),string("NNOutput_2"),30,-1.5,1.5);
        AddHisto(string("NNOutput_3"),string("NNOutput_3"),string("NNOutput_3"),30,-1.5,1.5);
        AddHisto(string("NNOutput_4"),string("NNOutput_4"),string("NNOutput_4"),30,-1.5,1.5);
        AddHisto(string("NNOutput_5"),string("NNOutput_5"),string("NNOutput_5"),30,-1.5,1.5);
        AddHisto(string("NNOutput_6"),string("NNOutput_6"),string("NNOutput_6"),30,-1.5,1.5);
        AddHisto(string("NNOutput_7"),string("NNOutput_7"),string("NNOutput_7"),30,-1.5,1.5);
        AddHisto(string("NNOutput_8"),string("NNOutput_8"),string("NNOutput_8"),30,-1.5,1.5);
        AddHisto(string("NNOutput_9"),string("NNOutput_9"),string("NNOutput_9"),30,-1.5,1.5);
        AddHisto(string("NNOutput_10"),string("NNOutput_10"),string("NNOutput_10"),30,-1.5,1.5);
        
        AddHisto(string("M3_NN5_60"),string("M3_NN5_60"),string("M3 (GeV), NN>0.5"),60,0,750);
        AddHisto(string("M3_NN5_75"),string("M3_NN5_75"),string("M3 (GeV), NN>0.5"),75,0,750);
        AddHisto(string("M3_NN5_100"),string("M3_NN5_100"),string("M3 (GeV), NN>0.5"),100,0,750);
        
        AddHisto(string("M3_NN3_60"),string("M3_NN3_60"),string("M3 (GeV), NN>0.3"),60,0,750);
        AddHisto(string("M3_NN3_75"),string("M3_NN3_75"),string("M3 (GeV), NN>0.3"),75,0,750);
        AddHisto(string("M3_NN3_100"),string("M3_NN3_100"),string("M3 (GeV), NN>0.3"),100,0,750);
        
        AddHisto(string("M3_NN4_60"),string("M3_NN4_60"),string("M3 (GeV), NN>0.4"),60,0,750);
        AddHisto(string("M3_NN4_75"),string("M3_NN4_75"),string("M3 (GeV), NN>0.4"),75,0,750);
        AddHisto(string("M3_NN4_100"),string("M3_NN4_100"),string("M3 (GeV), NN>0.4"),100,0,750);
        
        AddHisto(string("M3_NN6_60"),string("M3_NN6_60"),string("M3 (GeV), NN>0.6"),60,0,750);
        AddHisto(string("M3_NN6_75"),string("M3_NN6_75"),string("M3 (GeV), NN>0.6"),75,0,750);
        AddHisto(string("M3_NN6_100"),string("M3_NN6_100"),string("M3 (GeV), NN>0.6"),100,0,750);
        
        AddHisto(string("M3_NN7_60"),string("M3_NN7_60"),string("M3 (GeV), NN>0.7"),60,0,750);
        AddHisto(string("M3_NN7_75"),string("M3_NN7_75"),string("M3 (GeV), NN>0.7"),75,0,750);
        AddHisto(string("M3_NN7_100"),string("M3_NN7_100"),string("M3 (GeV), NN>0.7"),100,0,750);
        
        AddHisto(string("M3_NN8_60"),string("M3_NN8_60"),string("M3 (GeV), NN>0.8"),60,0,750);
        AddHisto(string("M3_NN8_75"),string("M3_NN8_75"),string("M3 (GeV), NN>0.8"),75,0,750);
        AddHisto(string("M3_NN8_100"),string("M3_NN8_100"),string("M3 (GeV), NN>0.8"),100,0,750);
        
        AddHisto(string("M3_NN5_35_350"),string("M3_NN5_35_350"),string("M3 (GeV), NN>0.5"),35,0,350);
        AddHisto(string("M3_NN5_30_50_350"),string("M3_NN5_30_50_350"),string("M3 (GeV), NN>0.5"),30,50,350);
        AddHisto(string("sqrtsNN"),string("M(#tau,jets) NN>0.5"),string("M(#tau,jets) NN>0.5"),140,0,2800);
        
        sumOfWeights[0]=0.;sumOfWeights[1]=0.;sumOfWeights[2]=0.;sumOfWeights[3]=0.;sumOfWeights[4]=0.;
        sumOfWeights[5]=0.;sumOfWeights[6]=0.;sumOfWeights[7]=0.;sumOfWeights[8]=0.;sumOfWeights[9]=0.;
        
}

void EventShapesHistoManager::Fill(const vector<NTJet>& candJet, const vector<NTJet>& candJetTauClean, const int& maxSelStep, const int&
iChannel, const int& iDataset, const float& weight, const vector<float>& NNOutput){
        if(!Check(iChannel, iDataset)) return;
        for(unsigned int i=0;i<SelectionSteps.size();i++){
                if(maxSelStep>=(int)i) FillSelStep(candJet, candJetTauClean, i, iChannel, iDataset, weight, NNOutput);  
        }       
}

void EventShapesHistoManager::FillSelStep(const vector<NTJet>& candJet, const vector<NTJet>& candJetTauClean, const int& iSelStep, const int&
iChannel, const int& iDataset, const float& weight, const vector<float>& NNOutput){
        if(!Check(iChannel, iSelStep, iDataset, 0) ) return;
        EventShapes evShape(candJet);
        Histos[0][iChannel][iSelStep][iDataset].Fill(evShape.aplanarity(),weight);
        Histos[1][iChannel][iSelStep][iDataset].Fill(evShape.sphericity(),weight);
        Histos[2][iChannel][iSelStep][iDataset].Fill(evShape.circularity(),weight);
        Histos[3][iChannel][iSelStep][iDataset].Fill(evShape.isotropy(),weight);
        Histos[4][iChannel][iSelStep][iDataset].Fill(evShape.C(),weight);
        Histos[5][iChannel][iSelStep][iDataset].Fill(evShape.D(),weight);
        Histos[6][iChannel][iSelStep][iDataset].Fill(evShape.HT(),weight);
        Histos[7][iChannel][iSelStep][iDataset].Fill(evShape.H(),weight);
        Histos[8][iChannel][iSelStep][iDataset].Fill(evShape.sqrt_s(),weight);
        Histos[70][iChannel][iSelStep][iDataset].Fill(evShape.sqrt_s(),weight);
        Histos[9][iChannel][iSelStep][iDataset].Fill(NNOutput.at(0),weight);
        Histos[10][iChannel][iSelStep][iDataset].Fill(evShape.chi2(candJetTauClean),weight);
        Histos[11][iChannel][iSelStep][iDataset].Fill(evShape.chi2(candJetTauClean),weight);
        
        if (NNOutput.at(0)>0.5) 
        { 
        Histos[12][iChannel][iSelStep][iDataset].Fill(evShape.chi2(candJetTauClean),weight);
        Histos[13][iChannel][iSelStep][iDataset].Fill(evShape.HT(),weight);
        Histos[14][iChannel][iSelStep][iDataset].Fill(evShape.aplanarity(),weight);
        Histos[15][iChannel][iSelStep][iDataset].Fill(evShape.sphericity(),weight);
        Histos[16][iChannel][iSelStep][iDataset].Fill(evShape.circularity(),weight);
        Histos[19][iChannel][iSelStep][iDataset].Fill(evShape.M3chi2(candJetTauClean),weight);
        Histos[20][iChannel][iSelStep][iDataset].Fill(evShape.M2chi2(candJetTauClean),weight);
        Histos[21][iChannel][iSelStep][iDataset].Fill(evShape.C(),weight);
        Histos[22][iChannel][iSelStep][iDataset].Fill(evShape.D(),weight);
        Histos[23][iChannel][iSelStep][iDataset].Fill(evShape.sqrt_s(),weight);
        Histos[26][iChannel][iSelStep][iDataset].Fill(NNOutput.at(1),weight);
        Histos[27][iChannel][iSelStep][iDataset].Fill(NNOutput.at(2),weight);
        
        Histos[50][iChannel][iSelStep][iDataset].Fill(evShape.M3(candJetTauClean),weight);
        Histos[51][iChannel][iSelStep][iDataset].Fill(evShape.M3(candJetTauClean),weight);
        Histos[52][iChannel][iSelStep][iDataset].Fill(evShape.M3(candJetTauClean),weight);
        
        Histos[68][iChannel][iSelStep][iDataset].Fill(evShape.M3(candJetTauClean),weight);
        Histos[69][iChannel][iSelStep][iDataset].Fill(evShape.M3(candJetTauClean),weight);
        }
        
        if (NNOutput.at(0)>0.3) 
        { 
        Histos[53][iChannel][iSelStep][iDataset].Fill(evShape.M3(candJetTauClean),weight);
        Histos[54][iChannel][iSelStep][iDataset].Fill(evShape.M3(candJetTauClean),weight);
        Histos[55][iChannel][iSelStep][iDataset].Fill(evShape.M3(candJetTauClean),weight);}
        
        if (NNOutput.at(0)>0.4) 
        { 
        Histos[56][iChannel][iSelStep][iDataset].Fill(evShape.M3(candJetTauClean),weight);
        Histos[57][iChannel][iSelStep][iDataset].Fill(evShape.M3(candJetTauClean),weight);
        Histos[58][iChannel][iSelStep][iDataset].Fill(evShape.M3(candJetTauClean),weight);}
        
        if (NNOutput.at(0)>0.6) 
        { 
        Histos[59][iChannel][iSelStep][iDataset].Fill(evShape.M3(candJetTauClean),weight);
        Histos[60][iChannel][iSelStep][iDataset].Fill(evShape.M3(candJetTauClean),weight);
        Histos[61][iChannel][iSelStep][iDataset].Fill(evShape.M3(candJetTauClean),weight);}
        
        if (NNOutput.at(0)>0.7) 
        { 
        Histos[62][iChannel][iSelStep][iDataset].Fill(evShape.M3(candJetTauClean),weight);
        Histos[63][iChannel][iSelStep][iDataset].Fill(evShape.M3(candJetTauClean),weight);
        Histos[64][iChannel][iSelStep][iDataset].Fill(evShape.M3(candJetTauClean),weight);}
        
        if (NNOutput.at(0)>0.8) 
        { 
        Histos[65][iChannel][iSelStep][iDataset].Fill(evShape.M3(candJetTauClean),weight);
        Histos[66][iChannel][iSelStep][iDataset].Fill(evShape.M3(candJetTauClean),weight);
        Histos[67][iChannel][iSelStep][iDataset].Fill(evShape.M3(candJetTauClean),weight);}
        
        
        Histos[17][iChannel][iSelStep][iDataset].Fill(evShape.M3chi2(candJetTauClean),weight);
        Histos[18][iChannel][iSelStep][iDataset].Fill(evShape.M2chi2(candJetTauClean),weight);
        Histos[24][iChannel][iSelStep][iDataset].Fill(NNOutput.at(1),weight);
        Histos[25][iChannel][iSelStep][iDataset].Fill(NNOutput.at(2),weight);
        if(NNOutput.at(1)<1) Histos[28][iChannel][iSelStep][iDataset].Fill(NNOutput.at(2),weight);
        
        Histos[29][iChannel][iSelStep][iDataset].Fill(NNOutput.at(0),weight);
        Histos[30][iChannel][iSelStep][iDataset].Fill(NNOutput.at(0),weight);
        Histos[31][iChannel][iSelStep][iDataset].Fill(NNOutput.at(0),weight);
        Histos[32][iChannel][iSelStep][iDataset].Fill(NNOutput.at(0),weight);
        Histos[33][iChannel][iSelStep][iDataset].Fill(NNOutput.at(0),weight);
        Histos[34][iChannel][iSelStep][iDataset].Fill(NNOutput.at(0),weight);
        Histos[35][iChannel][iSelStep][iDataset].Fill(NNOutput.at(0),weight);
        Histos[36][iChannel][iSelStep][iDataset].Fill(NNOutput.at(0),weight);
        Histos[37][iChannel][iSelStep][iDataset].Fill(NNOutput.at(0),weight);
        Histos[38][iChannel][iSelStep][iDataset].Fill(NNOutput.at(3),weight);
        
        Histos[39][iChannel][iSelStep][iDataset].Fill(evShape.M3(candJetTauClean),weight);
        
        float randNum1 = rand1.Uniform(1);
      
        if(sumOfWeights[0] < 223 && randNum1 <0.1){
        Histos[40][iChannel][iSelStep][iDataset].Fill(NNOutput.at(0),weight);
        sumOfWeights[0] += weight;
        }
        
        if(sumOfWeights[1] < 223 && randNum1 >0.1 && randNum1 <0.2){
        Histos[41][iChannel][iSelStep][iDataset].Fill(NNOutput.at(0),weight);
        sumOfWeights[1] += weight;
        }
        
        if(sumOfWeights[2] < 223 && randNum1 >0.2 && randNum1 <0.3){
        Histos[42][iChannel][iSelStep][iDataset].Fill(NNOutput.at(0),weight);
        sumOfWeights[2] += weight;
        }
        
        if(sumOfWeights[3] < 223 && randNum1 >0.3 && randNum1 <0.4){
        Histos[43][iChannel][iSelStep][iDataset].Fill(NNOutput.at(0),weight);
        sumOfWeights[3] += weight;
        }
        
        if(sumOfWeights[4] < 223 && randNum1 >0.4 && randNum1 <0.5){
        Histos[44][iChannel][iSelStep][iDataset].Fill(NNOutput.at(0),weight);
        sumOfWeights[4] += weight;
        }
        
        if(sumOfWeights[5] < 223 && randNum1 >0.5 && randNum1 <0.6){
        Histos[45][iChannel][iSelStep][iDataset].Fill(NNOutput.at(0),weight);
        sumOfWeights[5] += weight;
        }
        
        if(sumOfWeights[6] < 223 && randNum1 >0.6 && randNum1 <0.7){
        Histos[46][iChannel][iSelStep][iDataset].Fill(NNOutput.at(0),weight);
        sumOfWeights[6] += weight;
        }
        
        if(sumOfWeights[7] < 223 && randNum1 >0.7 && randNum1 <0.8){
        Histos[47][iChannel][iSelStep][iDataset].Fill(NNOutput.at(0),weight);
        sumOfWeights[7] += weight;
        }
        
        if(sumOfWeights[8] < 223 && randNum1 >0.8 && randNum1 <0.9){
        Histos[48][iChannel][iSelStep][iDataset].Fill(NNOutput.at(0),weight);
        sumOfWeights[8] += weight;
        }
        
        if(sumOfWeights[9] < 223 && randNum1 >0.9 && randNum1 <1.0){
        Histos[49][iChannel][iSelStep][iDataset].Fill(NNOutput.at(0),weight);
        sumOfWeights[9] += weight;
        }
        
}
