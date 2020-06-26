#include "../interface/DiLepAnaHistoManager.h"



DiLepAnaHistoManager::DiLepAnaHistoManager(){
}

DiLepAnaHistoManager::~DiLepAnaHistoManager(){
}

void DiLepAnaHistoManager::LoadDatasets(vector<Dataset> datasets){
        elecHistos.LoadDatasets(datasets);
        muHistos.LoadDatasets(datasets);
        jetHistos.LoadDatasets(datasets);
        metHistos.LoadDatasets(datasets);
        recoHistos.LoadDatasets(datasets);
        mcHistos.LoadDatasets(datasets);
        bjetHistos.LoadDatasets(datasets);
}    
        

void DiLepAnaHistoManager::LoadSelectionSteps(vector<string> selectionSteps){
        elecHistos.LoadSelectionSteps(selectionSteps);
        muHistos.LoadSelectionSteps(selectionSteps);
        jetHistos.LoadSelectionSteps(selectionSteps);
        metHistos.LoadSelectionSteps(selectionSteps);
        recoHistos.LoadSelectionSteps(selectionSteps);
        mcHistos.LoadSelectionSteps(selectionSteps);
        bjetHistos.LoadSelectionSteps(selectionSteps);
}
        
 
void DiLepAnaHistoManager::LoadChannels(vector<string> channels){
        elecHistos.LoadChannels(channels);
        muHistos.LoadChannels(channels);
        jetHistos.LoadChannels(channels);
        metHistos.LoadChannels(channels);
        recoHistos.LoadChannels(channels);
        mcHistos.LoadChannels(channels);
        bjetHistos.LoadChannels(channels);
}

void DiLepAnaHistoManager::Clear(){
        elecHistos.Clear();
        muHistos.Clear();
        jetHistos.Clear();
        metHistos.Clear();
        recoHistos.Clear();
        mcHistos.Clear();
        bjetHistos.Clear();
}


void DiLepAnaHistoManager::CreateHistos(){
        elecHistos.CreateHistos();
        muHistos.CreateHistos();
        jetHistos.CreateHistos();
        metHistos.CreateHistos();
        recoHistos.CreateHistos();
        mcHistos.CreateHistos();
        bjetHistos.CreateHistos();
}

void DiLepAnaHistoManager::Fill(const DiLeptonSelection& sel, NTEvent* event, const vector<NTMuon>& candMuon, const vector<NTElectron>& candElec, const int& maxSelStep, const int& iChannel, const int& iDataset, const float& weight){
        elecHistos.Fill(sel.GetSelectedElectrons(), maxSelStep, iChannel, iDataset, weight);
        muHistos.Fill(sel.GetSelectedMuons(), maxSelStep, iChannel, iDataset, weight);
        jetHistos.Fill(sel.GetJetsForAna(), maxSelStep, iChannel, iDataset, weight);
        metHistos.Fill(sel.GetMET(), maxSelStep, iChannel, iDataset, weight);
        recoHistos.Fill(event,candMuon, candElec, maxSelStep, iChannel, iDataset, weight);
        mcHistos.Fill(event, maxSelStep, iChannel, iDataset, weight);
        bjetHistos.Fill(sel.GetBJetsForAna().size(), maxSelStep, iChannel, iDataset, weight, weight);
}       

void DiLepAnaHistoManager::FillwBweight(const DiLeptonSelection& sel, NTEvent* event, const vector<NTMuon>& candMuon, const vector<NTElectron>& candElec, const int& maxSelStep, const int& iChannel, const int& iDataset, const float& weight, const vector<float>& weightb){
       
        int flagb=sel.GetFlagb();
        int methodb=sel.GetMethodb();
        float nbjetcut=sel.GetNofBtagJetsCut();
        if (flagb==0) {
              Fill( sel, event, candMuon, candElec, maxSelStep, iChannel, iDataset, weight);
        } 
        else if (flagb==1) {

           int irealmax=sel.GetCutList().size();
           int imaxplot=maxSelStep;
           if (maxSelStep==irealmax-1) {
                 // case of the btag cut
                 imaxplot=maxSelStep-1;  // we fill histo with weight up to imaxplot - see outside "if"
           }
           elecHistos.Fill(sel.GetSelectedElectrons(), imaxplot, iChannel, iDataset, weight);
           muHistos.Fill(sel.GetSelectedMuons()      , imaxplot, iChannel, iDataset, weight);
           jetHistos.Fill(sel.GetJetsForAna()      , imaxplot, iChannel, iDataset, weight);
           metHistos.Fill(sel.GetMET()               , imaxplot, iChannel, iDataset, weight);
           recoHistos.Fill(event,candMuon, candElec  , imaxplot, iChannel, iDataset, weight);
           mcHistos.Fill(event                       , imaxplot, iChannel, iDataset, weight);


           // Now : we apply the btag weight for the last cut

           if (methodb==0 && maxSelStep==irealmax-1) {
                   // with method_b : SF after Discri cut
                   elecHistos.FillSelStep(sel.GetSelectedElectrons(), maxSelStep, iChannel, iDataset, weightb[0]);
                   muHistos.FillSelStep(sel.GetSelectedMuons()      , maxSelStep, iChannel, iDataset, weightb[0]);
                   jetHistos.FillSelStep(sel.GetJetsForAna()      , maxSelStep, iChannel, iDataset, weightb[0]);
                   metHistos.FillSelStep(sel.GetMET()               , maxSelStep, iChannel, iDataset, weightb[0]);
                   recoHistos.FillSelStep(event,candMuon, candElec  , maxSelStep, iChannel, iDataset, weightb[0]);
                   mcHistos.FillSelStep(event                       , maxSelStep, iChannel, iDataset, weightb[0]);
          }


          if (methodb>=1 && maxSelStep>=irealmax-2) {
                   // with method_b : Efficiency without Discri cut
                   // so apply the weight to events passing the MET cut level
                   int iplothere= irealmax-1;
                   elecHistos.FillSelStep(sel.GetSelectedElectrons(), iplothere, iChannel, iDataset, weightb[0]);
                   muHistos.FillSelStep(sel.GetSelectedMuons()      , iplothere, iChannel, iDataset, weightb[0]);
                   jetHistos.FillSelStep(sel.GetJetsForAna()      , iplothere, iChannel, iDataset, weightb[0]);
                   metHistos.FillSelStep(sel.GetMET()               , iplothere, iChannel, iDataset, weightb[0]);
                   recoHistos.FillSelStep(event,candMuon, candElec  , iplothere, iChannel, iDataset, weightb[0]);
                   mcHistos.FillSelStep(event                       , iplothere, iChannel, iDataset, weightb[0]);
          }


          // Now : we fill the histo with the number of btag jets

          if (methodb==0) {
                 // at each level, compute with the btag weight
                 bjetHistos.Fill(sel.GetBJetsForAna().size(), imaxplot, iChannel, iDataset, weight, weightb[0]);
                 if (maxSelStep==irealmax-1) {
                   bjetHistos.FillSelStep(sel.GetBJetsForAna().size(), maxSelStep, iChannel, iDataset, weightb[0]);
                   bjetHistos.FillTemporary(sel.GetBJetsForAna().size(), maxSelStep, iChannel, iDataset, weightb[0]);
                 }
          }
          else  if (methodb>=1) {
                 // at each level, use the probability of the number of jets
                 bjetHistos.Fill(0, imaxplot, iChannel, iDataset, weight, weight*weightb[1]);
                 bjetHistos.Fill(1, imaxplot, iChannel, iDataset, weight, weight*weightb[2]);
                 bjetHistos.Fill(2, imaxplot, iChannel, iDataset, weight, weight*weightb[3]);
                 bjetHistos.Fill(3, imaxplot, iChannel, iDataset, weight, weight*weightb[4]);
                 if (maxSelStep>=irealmax-2) {
                   int iplothere= irealmax-1; // Case of Btag Level Cut : Suppress the bin0!
                   // bjetHistos.Fill(0., iplothere, iChannel, iDataset, 0.);
                   if (nbjetcut==1) {
                       bjetHistos.FillSelStep(1, iplothere, iChannel, iDataset, weight*weightb[2]);
                       bjetHistos.FillSelStep(2, iplothere, iChannel, iDataset, weight*weightb[3]);
                       bjetHistos.FillSelStep(3, iplothere, iChannel, iDataset, weight*weightb[4]);
                       // FillTemporary but histo[1] not used later in this case
                       bjetHistos.FillTemporary(1, iplothere, iChannel, iDataset, weight);
                       bjetHistos.FillTemporary(2, iplothere, iChannel, iDataset, weight);
                       bjetHistos.FillTemporary(3, iplothere, iChannel, iDataset, weight);
                   }
                   else if (nbjetcut==2) {
                       bjetHistos.FillSelStep(2, iplothere, iChannel, iDataset, weight*weightb[3]);
                       bjetHistos.FillSelStep(3, iplothere, iChannel, iDataset, weight*weightb[4]);
                       // FillTemporary but histo[1] not used later in this case
                       bjetHistos.FillTemporary(2, iplothere, iChannel, iDataset, weight);
                       bjetHistos.FillTemporary(3, iplothere, iChannel, iDataset, weight);
                   }
                   
                 }
          }

      }
}


void DiLepAnaHistoManager::Compute(){
        elecHistos.MergeChannels();
        muHistos.MergeChannels();
        jetHistos.MergeChannels();
        metHistos.MergeChannels();
        recoHistos.MergeChannels();
        mcHistos.MergeChannels();
        bjetHistos.MergeChannels();

        //MCStack
        elecHistos.DoMCStack();
        muHistos.DoMCStack();
        jetHistos.DoMCStack();
        metHistos.DoMCStack();
        recoHistos.DoMCStack();
        mcHistos.DoMCStack();
        bjetHistos.DoMCStack();

        //MCDatasets
        elecHistos.MergeMCDatasets();
        muHistos.MergeMCDatasets();
        jetHistos.MergeMCDatasets();
        metHistos.MergeMCDatasets();
        recoHistos.MergeMCDatasets();
        mcHistos.MergeMCDatasets();
        bjetHistos.MergeMCDatasets();

        //Cut by cut
        elecHistos.PlotsCutByCut();
        muHistos.PlotsCutByCut();
        jetHistos.PlotsCutByCut();
        metHistos.PlotsCutByCut();
        recoHistos.PlotsCutByCut();
        mcHistos.PlotsCutByCut();
        bjetHistos.PlotsCutByCut();

}


void DiLepAnaHistoManager::Write(TFile* file){
        TDirectory * dir = 0;
        dir = file->mkdir("DiLepAnaPlots");
        dir = dir->mkdir("Electrons");
        elecHistos.Write(dir);
        dir = file->GetDirectory("DiLepAnaPlots");
        dir = dir->mkdir("Muons");
        muHistos.Write(dir);
        dir = file->GetDirectory("DiLepAnaPlots");
        dir = dir->mkdir("Jets");
        jetHistos.Write(dir);
        dir = file->GetDirectory("DiLepAnaPlots");
        dir = dir->mkdir("MET");
        metHistos.Write(dir);
        dir = file->GetDirectory("DiLepAnaPlots");
        dir = dir->mkdir("RECO");
        recoHistos.Write(dir);
        dir = file->GetDirectory("DiLepAnaPlots");
        dir = dir->mkdir("MC");
        mcHistos.Write(dir);
        dir = file->GetDirectory("DiLepAnaPlots");
        dir = dir->mkdir("BJets");
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
