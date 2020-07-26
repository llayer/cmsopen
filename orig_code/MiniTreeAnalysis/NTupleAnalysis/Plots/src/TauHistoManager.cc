#include "../interface/TauHistoManager.h"




TauHistoManager::TauHistoManager(){
}

TauHistoManager::~TauHistoManager(){
}


void TauHistoManager::CreateHistos(){
        AddHisto(string("taumutiplicity"),string("#(taus)"),string("Nof taus"),10,0,10);        
        AddHisto(string("Pt"),string("p_{T}(#tau)"),string("p_{T}(#tau) (GeV)"),125,0,250);     
        AddHisto(string("Eta"),string("Eta(#tau)"),string("#eta(#tau)"),60,-3,3);       
        AddHisto(string("Phi"),string("Phi(#tau)"),string("#phi(#tau)"),16,-3.2,3.2);   
        AddHisto(string("d0"),string("d0(#tau)"),string("d0(#tau)"),25,0,0.1);  
        AddHisto(string("dz"),string("dz(#tau,vtx)"),string("dz(#tau,vtx)"),50,-1,1);   
        AddHisto(string("charge"),string("charge(#tau)"),string("charge(#tau)"),5,-2,2);
        AddHisto(string("leadTrackPt"),string("leadTrackPt(#tau)"),string("leadTrackPt(#tau) (GeV/c)"),40,0,200);
        AddHisto(string("byLooseIsolation"),string("byLooseIsolation(#tau)"),string("byLooseIsolation(#tau)"),3,-1,2);
        AddHisto(string("byMediumIsolation"),string("byMediumIsolation(#tau)"),string("byMediumIsolation(#tau)"),3,-1,2);
        AddHisto(string("byTightIsolation"),string("byTightIsolation(#tau)"),string("byTightIsolation(#tau)"),3,-1,2);  
        AddHisto(string("numSigConeTracks"),string("numSigConeTracks(#tau)"),string("numSigConeTracks(#tau)"),20,0,20);
        AddHisto(string("numIsoConeTracks"),string("numIsoConeTracks(#tau)"),string("numIsoConeTracks(#tau)"),20,0,20);
        AddHisto(string("isolationPFChargedHadrCandsPtSum"),string("isolationPFChargedHadrCandsPtSum(#tau)"),string("isolationPFChargedHadrCandsPtSum(#tau)"),10,0,1.);
        AddHisto(string("isolationPFGammaCandsEtSum"),string("isolationPFGammaCandsEtSum(#tau)"),string("isolationPFGammaCandsEtSum(#tau)"),10,0,1.);
        AddHisto(string("emFraction"),string("emFraction(#tau)"),string("emFraction(#tau)"),10,0,1.);
        AddHisto(string("againstElectronLoose"),string("againstElectronLoose(#tau)"),string("againstElectronLoose(#tau)"),3,-1,2.);
        AddHisto(string("againstMuonLoose"),string("againstMuonLoose(#tau)"),string("againstMuonLoose(#tau)"),3,-1,2.);
        AddHisto(string("q*Eta"),string("q*|Eta(#tau)|"),string("q*|#eta(#tau)|"),20,-3,3);
        AddHisto(string("MT"),string("M_{T}(#tau,MET)"),string("M_{T}(#tau,MET) (GeV)"),30,0,300);
        AddHisto(string("decayMode"),string("decayMode(#tau)"),string("decayMode(#tau)"),40,-20,20);
        AddHisto(string("decayModeNN"),string("decayMode(#tau)"),string("decayMode(#tau) NN>0.5"),40,-20,20);
        AddHisto(string("qEtaNN"),string("q*|Eta(#tau)| NN>0.5"),string("q*Eta(#tau) NN>0.5"),20,-3,3);
        AddHisto(string("PtNN"),string("Pt(#tau) NN>0.5"),string("Pt(#tau) NN>0.5"),100,0,200);
        AddHisto(string("MTNN"),string("M_{T}(#tau,MET) NN>0.5"),string("M_{T}(#tau,MET) NN>0.5"),20,0,200);
        AddHisto(string("DeltaPhiTauMet"),string("#Delta #phi (#tau,MET)"),string("#Delta #phi (#tau,MET)"),17,0,3.1416);
        AddHisto(string("DeltaPhiTauMetNN"),string("#Delta #phi (#tau,MET) NN>0.5"),string("#Delta #phi (#tau,MET) NN>0.5"),17,0,3.1416);
        AddHisto(string("leadTrackPtNN"),string("leadTrackPt(#tau) NN>0.5"),string("leadTrackPt(#tau) NN>0.5"),40,0,200);
        AddHisto(string("emFractionNN"),string("emFraction(#tau) NN>0.5"),string("emFraction(#tau) NN>0.5"),10,0,1.);
        AddHisto(string("DR_wHLTtau"),string("DR_wHLTtau"),string("DR_wHLTtau"),110,-1.,10.);
        AddHisto(string("NPV"),string("NPV"),string("N. of primary vertices"),25,0,25.);
        AddHisto(string("DR_tauObj_jetObj"),string("DR_tauObj_jetObj"),string("#Delta R (HLT-#tau,HLT-jet)"),150,0,3.);
        
}

void TauHistoManager::Fill(NTMET met, const vector<NTTau>& taus, const vector<NTVertex>& vertices, const
vector<TLorentzVector>& tauObjTrig, const vector<TLorentzVector>& jetObjTrig, const int& maxSelStep, const int& iChannel, const int& iDataset, const float& weight, const float& NNOutput){
        if(!Check(iChannel, iDataset)) return;
        for(unsigned int i=0;i<SelectionSteps.size();i++){
                if(maxSelStep>=(int)i) FillSelStep(met, taus, vertices, tauObjTrig, jetObjTrig, i, iChannel, iDataset, weight, NNOutput);       
        }       
}

void TauHistoManager::FillSelStep(NTMET met, const vector<NTTau>& taus, const vector<NTVertex>& vertices, const
vector<TLorentzVector>& tauObjTrig, const vector<TLorentzVector>& jetObjTrig,const int& iSelStep, const int& iChannel, const int& iDataset, const float& weight, const float& NNOutput){
        if(!Check(iChannel, iSelStep, iDataset, 0) ) return;
        Histos[0][iChannel][iSelStep][iDataset].Fill(taus.size(),weight);
        float MTW = -9999;
        for(int i=0;i<(int) taus.size();i++){
                NTTau taui = taus[i];
                float deltaPhi = fabs(taui.p4.DeltaPhi(met.p4));
                MTW = sqrt(2*taui.p4.Pt()*met.p4.Et()*(1-cos(deltaPhi)));
                
                //very important:
                //respect the order of the function CreateHistos to fill the histograms
                Histos[1][iChannel][iSelStep][iDataset].Fill(taus[i].p4.Pt(),weight);
                Histos[2][iChannel][iSelStep][iDataset].Fill(taus[i].p4.Eta(),weight);
                Histos[3][iChannel][iSelStep][iDataset].Fill(taus[i].p4.Phi(),weight);
                Histos[4][iChannel][iSelStep][iDataset].Fill(taus[i].D0,weight);
                Histos[5][iChannel][iSelStep][iDataset].Fill(taus[i].vertex.Z()-vertices[0].p3.Z(),weight);
                Histos[6][iChannel][iSelStep][iDataset].Fill(taus[i].Charge,weight);
                Histos[7][iChannel][iSelStep][iDataset].Fill(taus[i].leadTrackPt,weight);
                Histos[8][iChannel][iSelStep][iDataset].Fill(taui.GetDiscriminator("byLooseIsolation"),weight);
                Histos[9][iChannel][iSelStep][iDataset].Fill(taui.GetDiscriminator("byMediumIsolation"),weight);
                Histos[10][iChannel][iSelStep][iDataset].Fill(taui.GetDiscriminator("byTightIsolation"),weight);                
                Histos[11][iChannel][iSelStep][iDataset].Fill(taus[i].numSigConeTracks,weight);
                Histos[12][iChannel][iSelStep][iDataset].Fill(taus[i].numIsoConeTracks,weight);
                Histos[13][iChannel][iSelStep][iDataset].Fill(taus[i].isolationPFChargedHadrCandsPtSum,weight);
                Histos[14][iChannel][iSelStep][iDataset].Fill(taus[i].isolationPFGammaCandsEtSum,weight);
                Histos[15][iChannel][iSelStep][iDataset].Fill(taus[i].emFraction,weight);
                Histos[16][iChannel][iSelStep][iDataset].Fill(taui.GetDiscriminator("againstElectronLoose"),weight);
                Histos[17][iChannel][iSelStep][iDataset].Fill(taui.GetDiscriminator("againstMuonLoose"),weight);
                Histos[18][iChannel][iSelStep][iDataset].Fill((taus[i].p4.Eta())*fabs(taus[i].Charge),weight);
                Histos[19][iChannel][iSelStep][iDataset].Fill(MTW,weight);
                Histos[20][iChannel][iSelStep][iDataset].Fill(taus[i].decayMode,weight);
                if (taus[i].p4HLT.Px()!=0) Histos[29][iChannel][iSelStep][iDataset].Fill(taus[i].p4.DeltaR(taus[i].p4HLT),weight);
                else Histos[29][iChannel][iSelStep][iDataset].Fill(-1.,weight);
                
                if(NNOutput>0.5)
                {
                Histos[21][iChannel][iSelStep][iDataset].Fill(taus[i].decayMode,weight);
                Histos[22][iChannel][iSelStep][iDataset].Fill((taus[i].p4.Eta())*(taus[i].Charge),weight);
                Histos[23][iChannel][iSelStep][iDataset].Fill(taus[i].p4.Pt(),weight);
                Histos[24][iChannel][iSelStep][iDataset].Fill(MTW,weight);
                Histos[26][iChannel][iSelStep][iDataset].Fill(deltaPhi,weight);
                Histos[27][iChannel][iSelStep][iDataset].Fill(taus[i].leadTrackPt,weight);
                Histos[28][iChannel][iSelStep][iDataset].Fill(taus[i].emFraction,weight);
                }
                Histos[25][iChannel][iSelStep][iDataset].Fill(deltaPhi,weight);
                
                Histos[30][iChannel][iSelStep][iDataset].Fill(vertices.size(),weight);
                
                }
              
               float deltaR=3;
               float deltaRmin=3;
   
               for (unsigned int i=0; i<tauObjTrig.size(); i++)
               {
                for (unsigned int i=0; i<jetObjTrig.size(); i++)
                {
                 deltaR = tauObjTrig[0].DeltaR(jetObjTrig[i]);
                 if(deltaR<deltaRmin) deltaRmin = deltaR;
                 }
                }
                Histos[31][iChannel][iSelStep][iDataset].Fill(deltaRmin,weight);        
                
                
}


