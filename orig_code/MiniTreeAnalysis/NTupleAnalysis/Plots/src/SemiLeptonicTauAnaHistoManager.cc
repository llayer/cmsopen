#include "../interface/SemiLeptonicTauAnaHistoManager.h"



SemiLeptonicTauAnaHistoManager::SemiLeptonicTauAnaHistoManager(){
}

SemiLeptonicTauAnaHistoManager::~SemiLeptonicTauAnaHistoManager(){
}

void SemiLeptonicTauAnaHistoManager::LoadDatasets(vector<Dataset> datasets){
        //elecHistos.LoadDatasets(datasets);
        //muHistos.LoadDatasets(datasets);
        jetHistos.LoadDatasets(datasets);
        metHistos.LoadDatasets(datasets);
        tauHistos.LoadDatasets(datasets);
        evShapesHistos.LoadDatasets(datasets);
        //recoHistos.LoadDatasets(datasets);
        //mcHistos.LoadDatasets(datasets);
        bjetHistos.LoadDatasets(datasets);
}    
        

void SemiLeptonicTauAnaHistoManager::LoadSelectionSteps(vector<string> selectionSteps){
        //elecHistos.LoadSelectionSteps(selectionSteps);
        //muHistos.LoadSelectionSteps(selectionSteps);
        jetHistos.LoadSelectionSteps(selectionSteps);
        metHistos.LoadSelectionSteps(selectionSteps);
        tauHistos.LoadSelectionSteps(selectionSteps);
        evShapesHistos.LoadSelectionSteps(selectionSteps);
        //recoHistos.LoadSelectionSteps(selectionSteps);
        //mcHistos.LoadSelectionSteps(selectionSteps);
        bjetHistos.LoadSelectionSteps(selectionSteps);
}
        
 
void SemiLeptonicTauAnaHistoManager::LoadChannels(vector<string> channels){
        //elecHistos.LoadChannels(channels);
        //muHistos.LoadChannels(channels);
        jetHistos.LoadChannels(channels);
        metHistos.LoadChannels(channels);
        tauHistos.LoadChannels(channels);
        evShapesHistos.LoadChannels(channels);
        //recoHistos.LoadChannels(channels);
        //mcHistos.LoadChannels(channels);
        bjetHistos.LoadChannels(channels);
}

void SemiLeptonicTauAnaHistoManager::Clear(){
        //elecHistos.Clear();
        //muHistos.Clear();
        jetHistos.Clear();
        metHistos.Clear();
        tauHistos.Clear();
        evShapesHistos.Clear();
        //recoHistos.Clear();
        //mcHistos.Clear();
        bjetHistos.Clear();
}


void SemiLeptonicTauAnaHistoManager::CreateHistos(){
        //elecHistos.CreateHistos();
        //muHistos.CreateHistos();
        jetHistos.CreateHistos();
        metHistos.CreateHistos();
        tauHistos.CreateHistos();
        evShapesHistos.CreateHistos();
        //recoHistos.CreateHistos();
        //mcHistos.CreateHistos();
        bjetHistos.CreateHistos();
}

void SemiLeptonicTauAnaHistoManager::Fill(const SemiLeptonicTauSelection& sel, const int& maxSelStep, const int& iChannel, const int&
iDataset, const float& weight, const vector<float>& NNOutput){
        
        //vector<NTMuon> muon_cand    = sel.GetSelectedMuonsNoIso();
        //vector<NTElectron> elec_cand  = sel.GetSelectedElectronsNoIso();
        
        vector<NTMuon> muon_cand      = sel.GetSelectedMuonsLooseIDLooseIso();
        vector<NTElectron> elec_cand  = sel.GetSelectedElectronsLooseIDLooseIso();
        
        //very dangerous !!!!!
        //vector<NTTau> tau_cand        = sel.GetSelectedTaus(muon_cand,elec_cand,false,1,0,true);
        
        vector<NTTau> tau_cand        = sel.GetTausForAna();
        vector<NTJet> jet_cand        = sel.GetJetsForAna();
        
        vector<NTJet> jet_cand_2      = sel.GetSelectedJets(muon_cand, elec_cand);
        vector<NTVertex> vtx_cand     = sel.GetSelectedVertex();
        
        vector<TLorentzVector> jetObjTrig = sel.GetJetTrigObj();
        vector<TLorentzVector> tauObjTrig = sel.GetTauTrigObj();
        
        //elecHistos.Fill(sel.GetSelectedElectrons(), maxSelStep, iChannel, iDataset, weight);
        //muHistos.Fill(sel.GetSelectedMuons(), maxSelStep, iChannel, iDataset, weight);
        
        jetHistos.Fill(jet_cand, maxSelStep, iChannel, iDataset, weight);
        metHistos.Fill(sel.GetMET(), maxSelStep, iChannel, iDataset, weight);
        tauHistos.Fill(sel.GetMET(), tau_cand, vtx_cand, tauObjTrig, jetObjTrig, maxSelStep, iChannel, iDataset, weight, NNOutput.at(0));
        evShapesHistos.Fill(jet_cand_2, jet_cand, maxSelStep, iChannel, iDataset, weight, NNOutput);
        //recoHistos.Fill(event,candMuon, candElec, maxSelStep, iChannel, iDataset, weight);
        //mcHistos.Fill(event, maxSelStep, iChannel, iDataset, weight);
        bjetHistos.Fill(sel.GetBJetsForAna().size(), maxSelStep, iChannel, iDataset, weight, weight);

}
        
/*
void SemiLeptonicTauAnaHistoManager::FillwBweight(const SemiLeptonicTauSelection& sel, NTEvent* event, const vector<NTMuon>& candMuon, const vector<NTElectron>& candElec, const int& maxSelStep, const int& iChannel, const int& iDataset, const float& weight, const float& weightb){
       
        int flagb=sel.GetFlagb();
        int methodb=sel.GetMethodb();
        if (flagb==0) {
//              std::cout << " in SemiLeptonicTauAnaHistoManager::FillwBweight flag==0 " << std::endl;
              Fill( sel, event, candMuon, candElec, maxSelStep, iChannel, iDataset, weight);
        } 
        else if (flagb==1) {


//          std::cout << " in SemiLeptonicTauAnaHistoManager::FillwBweight flag==1  maxSelStep " <<  maxSelStep << std::endl;
          elecHistos.Fill(sel.GetSelectedElectrons(), maxSelStep-1, iChannel, iDataset, weight);
//             std::cout << " in SemiLeptonicTauAnaHistoManager:: aftr elec  " << std::endl;
          muHistos.Fill(sel.GetSelectedMuons(), maxSelStep-1, iChannel, iDataset, weight);
//             std::cout << " in SemiLeptonicTauAnaHistoManager:: aftr mu  " << std::endl;
          jetHistos.Fill(sel.GetSelectedJets(), maxSelStep-1, iChannel, iDataset, weight);
//             std::cout << " in SemiLeptonicTauAnaHistoManager:: aftr jet  " << std::endl;
          metHistos.Fill(sel.GetMET(), maxSelStep-1, iChannel, iDataset, weight);
//             std::cout << " in SemiLeptonicTauAnaHistoManager:: aftr met  " << std::endl;         
          tauHistos.Fill(sel.GetSelectedTaus(), maxSelStep-1, iChannel, iDataset, weight);
//             std::cout << " in SemiLeptonicTauAnaHistoManager:: aftr tau  " << std::endl;
//          recoHistos.Fill(event,candMuon, candElec, maxSelStep-1, iChannel, iDataset, weight);
//             std::cout << " in SemiLeptonicTauAnaHistoManager:: aftr reco  " << std::endl;
//          mcHistos.Fill(event, maxSelStep-1, iChannel, iDataset, weight);
//             std::cout << " in SemiLeptonicTauAnaHistoManager:: aftr mc  " << std::endl;

          // laststep = btag!
          if (methodb==0) {
            elecHistos.FillSelStep(sel.GetSelectedElectrons(), maxSelStep , iChannel, iDataset, weightb);
            muHistos.FillSelStep(sel.GetSelectedMuons(), maxSelStep, iChannel, iDataset, weightb);
            jetHistos.FillSelStep(sel.GetSelectedJets(), maxSelStep, iChannel, iDataset, weightb);
            metHistos.FillSelStep(sel.GetMET(), maxSelStep, iChannel, iDataset, weightb);
            tauHistos.FillSelStep(sel.GetSelectedTaus(), maxSelStep, iChannel, iDataset, weightb);
            //recoHistos.FillSelStep(event,candMuon, candElec, maxSelStep, iChannel, iDataset, weightb);
            //mcHistos.FillSelStep(event, maxSelStep, iChannel, iDataset, weightb);
//             std::cout << " in SemiLeptonicTauAnaHistoManager:: aftr last step  " << std::endl;
          }
          else {
            std::cout << " You use methodb=" << methodb << " there is no histogram filled in that case " << std::endl;
          }
        }
//       std::cout << " in SemiLeptonicTauAnaHistoManager::FillwBweight --> DONE " << std::endl;
}
*/

void SemiLeptonicTauAnaHistoManager::Compute(){
        //elecHistos.MergeChannels();
        //muHistos.MergeChannels();
        jetHistos.MergeChannels();
        metHistos.MergeChannels();
        tauHistos.MergeChannels();
        evShapesHistos.MergeChannels();
        //recoHistos.MergeChannels();
        //mcHistos.MergeChannels();
        bjetHistos.MergeChannels();
        
        //MCStack
        //elecHistos.DoMCStack();
        //muHistos.DoMCStack();
        jetHistos.DoMCStack();
        metHistos.DoMCStack();
        tauHistos.DoMCStack();
        evShapesHistos.DoMCStack();
        //recoHistos.DoMCStack();
        //mcHistos.DoMCStack();
        bjetHistos.DoMCStack();
        
        //MCDatasets
        //elecHistos.MergeMCDatasets();
        //muHistos.MergeMCDatasets();
        jetHistos.MergeMCDatasets();
        metHistos.MergeMCDatasets();
        tauHistos.MergeMCDatasets();
        evShapesHistos.MergeMCDatasets();
        //recoHistos.MergeMCDatasets();
        //mcHistos.MergeMCDatasets();
        bjetHistos.MergeMCDatasets();

        //Cut by cut
        //elecHistos.PlotsCutByCut();
        //muHistos.PlotsCutByCut();
        jetHistos.PlotsCutByCut();
        metHistos.PlotsCutByCut();
        tauHistos.PlotsCutByCut();
        evShapesHistos.PlotsCutByCut();
        //recoHistos.PlotsCutByCut();
        //mcHistos.PlotsCutByCut();
        bjetHistos.PlotsCutByCut();
}


void SemiLeptonicTauAnaHistoManager::Write(TFile* file){
        TDirectory * dir = 0;
        dir = file->mkdir("SemiLeptonicTauAnaPlots");
        dir = dir->mkdir("Electrons");
        elecHistos.Write(dir);
        dir = file->GetDirectory("SemiLeptonicTauAnaPlots");
        dir = dir->mkdir("Muons");
        muHistos.Write(dir);
        dir = file->GetDirectory("SemiLeptonicTauAnaPlots");
        dir = dir->mkdir("Jets");
        jetHistos.Write(dir);
        dir = file->GetDirectory("SemiLeptonicTauAnaPlots");
        dir = dir->mkdir("MET");
        metHistos.Write(dir);
        dir = file->GetDirectory("SemiLeptonicTauAnaPlots");
        dir = dir->mkdir("Taus");
        tauHistos.Write(dir);
        dir = file->GetDirectory("SemiLeptonicTauAnaPlots");
        dir = dir->mkdir("EventShapes");
        evShapesHistos.Write(dir);
//      dir = file->GetDirectory("SemiLeptonicTauAnaPlots");
//      dir = dir->mkdir("RECO");
//      recoHistos.Write(dir);
//      dir = file->GetDirectory("SemiLeptonicTauAnaPlots");
//      dir = dir->mkdir("MC");
//      mcHistos.Write(dir);
        dir = file->GetDirectory("SemiLeptonicTauAnaPlots");
        dir = dir->mkdir("Bjets");
        bjetHistos.Write(dir);
        dir = 0;
        delete dir;
}



/*
        ElectronHistoManager elecHistos;
*/
        /*
        MuonHistoManager muHistos;
        JetHistoManager jetHistos;
        METHistoManager metHistos;
        RecoHistoManager recoHistos;
        MCHistoManager mcHistos;
        */

