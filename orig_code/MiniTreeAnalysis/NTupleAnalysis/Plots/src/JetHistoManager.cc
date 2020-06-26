#include "../interface/JetHistoManager.h"




JetHistoManager::JetHistoManager(){
}

JetHistoManager::~JetHistoManager(){
}


void JetHistoManager::CreateHistos(){
        AddHisto(string("Multiplicity"),string("#(jets)"),string("Nof jets"),9,3.5,12.5);       
        AddHisto(string("Pt"),string("Pt(jet)"),string("p_{T}(jet) (GeV)"),200,0,400);  
        AddHisto(string("Eta"),string("Eta(jet)"),string("#eta(jet)"),60,-3,3); 
        AddHisto(string("Phi"),string("Phi(jet)"),string("#phi(jet)"),64,-3.2,3.2);     
        AddHisto(string("TCDisc"),string("TCDisc(jet)"),string("TCDisc(jet)"),10,-50,50);       
        AddHisto(string("SVDisc"),string("SVDisc(jet)"),string("SVDisc(jet)"),10,-50,50);       
        AddHisto(string("SMDisc"),string("SMDisc(jet)"),string("SMDisc(jet)"),10,-50,50);
        AddHisto(string("Pt_1"),string("Pt(jet)"),string("p_{T}(jet)"),50,0,100);
        AddHisto(string("Pt_2"),string("Pt(jet)"),string("p_{T}(jet)"),50,0,100);
        AddHisto(string("Pt_3"),string("Pt(jet)"),string("p_{T}(jet)"),50,0,100);
        AddHisto(string("Pt_4"),string("Pt(jet)"),string("p_{T}(jet)"),50,0,100);
        AddHisto(string("frac_btag_1"),string("frac_btag_1"),string("frac. of b-tagged jets"),2,0,2);
        AddHisto(string("frac_btag_2"),string("frac_btag_2"),string("frac. of b-tagged jets"),2,0,2);
        AddHisto(string("frac_btag_3"),string("frac_btag_3"),string("frac. of b-tagged jets"),2,0,2);
        AddHisto(string("frac_btag_4"),string("frac_btag_4"),string("frac. of b-tagged jets"),2,0,2);
        AddHisto(string("flavour_1"),string("flavour_1"),string("flavour 1st.jet"),25,0,25);
        AddHisto(string("flavour_2"),string("flavour_2"),string("flavour 2nd.jet"),25,0,25);
        AddHisto(string("flavour_3"),string("flavour_3"),string("flavour 3rd.jet"),25,0,25);
        AddHisto(string("flavour_4"),string("flavour_4"),string("flavour 4th.jet"),25,0,25);

                
}

void JetHistoManager::Fill(const vector<NTJet>& jets, const int& maxSelStep, const int& iChannel, const int& iDataset, const float& weight){
        if(!Check(iChannel, iDataset)) return;
        for(unsigned int i=0;i<SelectionSteps.size();i++){
                if(maxSelStep>=(int)i) FillSelStep(jets, i , iChannel, iDataset, weight);       
        }       
}

void JetHistoManager::FillSelStep(const vector<NTJet>& jets, const int& iSelStep, const int& iChannel, const int& iDataset, const float& weight){
        if(!Check(iChannel, iSelStep, iDataset, 0) ) return;
        Histos[0][iChannel][iSelStep][iDataset].Fill(jets.size(),weight);
        for(int i=0;i<(int) jets.size();i++){
                //very important:
                //respect the order of the function CreateHistos to fill the histograms
                Histos[1][iChannel][iSelStep][iDataset].Fill(jets[i].p4.Pt(),weight);
                Histos[2][iChannel][iSelStep][iDataset].Fill(jets[i].p4.Eta(),weight);
                Histos[3][iChannel][iSelStep][iDataset].Fill(jets[i].p4.Phi(),weight);
                Histos[4][iChannel][iSelStep][iDataset].Fill(jets[i].TCDiscri,weight);
                Histos[5][iChannel][iSelStep][iDataset].Fill(jets[i].SVDiscri,weight);
                Histos[6][iChannel][iSelStep][iDataset].Fill(jets[i].SMDiscri,weight);
        }
        
        if (jets.size()>=4)
        {
          Histos[7][iChannel][iSelStep][iDataset].Fill(jets[0].p4.Pt(),weight);
          Histos[8][iChannel][iSelStep][iDataset].Fill(jets[1].p4.Pt(),weight);
          Histos[9][iChannel][iSelStep][iDataset].Fill(jets[2].p4.Pt(),weight);
          Histos[10][iChannel][iSelStep][iDataset].Fill(jets[3].p4.Pt(),weight);
          
          if (jets[0].TCDiscri>=3.3)
          Histos[11][iChannel][iSelStep][iDataset].Fill(1,weight);
          else
          Histos[11][iChannel][iSelStep][iDataset].Fill(0.,weight);
          
          if (jets[1].TCDiscri>=3.3)
          Histos[12][iChannel][iSelStep][iDataset].Fill(1,weight);
          else
          Histos[12][iChannel][iSelStep][iDataset].Fill(0.,weight);
          
          if (jets[2].TCDiscri>=3.3)
          Histos[13][iChannel][iSelStep][iDataset].Fill(1,weight);
          else
          Histos[13][iChannel][iSelStep][iDataset].Fill(0.,weight);
          
          if (jets[3].TCDiscri>=3.3)
          Histos[14][iChannel][iSelStep][iDataset].Fill(1,weight);
          else
          Histos[14][iChannel][iSelStep][iDataset].Fill(0.,weight);
          
          
          Histos[15][iChannel][iSelStep][iDataset].Fill(fabs(jets[0].partonFlavour),weight);
          Histos[16][iChannel][iSelStep][iDataset].Fill(fabs(jets[1].partonFlavour),weight);
          Histos[17][iChannel][iSelStep][iDataset].Fill(fabs(jets[2].partonFlavour),weight);
          Histos[18][iChannel][iSelStep][iDataset].Fill(fabs(jets[3].partonFlavour),weight);
        
        }
        
        
}


