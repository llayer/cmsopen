#ifndef DiLepAnaHistoManager_h
#define DiLepAnaHistoManager_h

#include "../../../../MiniTreeFormat/NTFormat/interface/NTEvent.h"

#include "../../Selection/interface/DiLeptonSelection.h"
#include "../../Tools/interface/Dataset.h"
#include "ElectronHistoManager.h"
#include "MuonHistoManager.h"
#include "JetHistoManager.h"
#include "BJetHistoManager.h"
#include "METHistoManager.h"
#include "DiLepRecoHistoManager.h"
#include "DiLepMCHistoManager.h"

#include <TH1F.h>

using namespace TopTree;

class DiLepAnaHistoManager{

  public:
        DiLepAnaHistoManager();
        ~DiLepAnaHistoManager();

        void LoadDatasets(vector<Dataset> datasets);    
        void LoadSelectionSteps(vector<string> selectionSteps);
        void LoadChannels(vector<string> channels);

        void Clear();   
        void CreateHistos();    

        void Fill(const DiLeptonSelection& sel, NTEvent* event, const vector<NTMuon>& candMuon, const vector<NTElectron>& candElec, const int& maxSelStep, const int& iChannel, const int& iDataset, const float& weight);
        void FillwBweight(const DiLeptonSelection& sel, NTEvent* event, const vector<NTMuon>& candMuon, const vector<NTElectron>& candElec, const int& maxSelStep, const int& iChannel, const int& iDataset, const float& weight, const vector<float>& weightb);
        
        void Compute();
        void Write(TFile* file);



  private:

        ElectronHistoManager elecHistos;
        MuonHistoManager muHistos;
        JetHistoManager jetHistos;
        BJetHistoManager bjetHistos;
        METHistoManager metHistos;
        DiLepRecoHistoManager recoHistos;
        DiLepMCHistoManager mcHistos;
};

#endif
