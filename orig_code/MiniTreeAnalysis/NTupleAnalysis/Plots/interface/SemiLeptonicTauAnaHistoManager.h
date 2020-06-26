#ifndef SemiLeptonicTauAnaHistoManager_h
#define SemiLeptonicTauAnaHistoManager_h

#include "../../../../MiniTreeFormat/NTFormat/interface/NTEvent.h"

#include "../../Selection/interface/SemiLeptonicTauSelection.h"
#include "../../Tools/interface/Dataset.h"
#include "ElectronHistoManager.h"
#include "MuonHistoManager.h"
#include "JetHistoManager.h"
#include "METHistoManager.h"
#include "TauHistoManager.h"
#include "EventShapesHistoManager.h"
//#include "DiLepRecoHistoManager.h"
//#include "DiLepMCHistoManager.h"
#include "BJetHistoManager.h"

#include <TH1F.h>

using namespace TopTree;

class SemiLeptonicTauAnaHistoManager{

  public:
        SemiLeptonicTauAnaHistoManager();
        ~SemiLeptonicTauAnaHistoManager();

        void LoadDatasets(vector<Dataset> datasets);    
        void LoadSelectionSteps(vector<string> selectionSteps);
        void LoadChannels(vector<string> channels);

        void Clear();   
        void CreateHistos();    

        void Fill(const SemiLeptonicTauSelection& sel, const int& maxSelStep, const int& iChannel, const int& iDataset, const float& weight, const vector<float>& NNOutput);
        //void FillwBweight(const SemiLeptonicTauSelection& sel, NTEvent* event, const vector<NTMuon>& candMuon, const vector<NTElectron>& candElec, const int& maxSelStep, const int& iChannel, const int& iDataset, const float& weight, const float& weightb);
        
        void Compute();
        void Write(TFile* file);



  private:

        ElectronHistoManager elecHistos;
        MuonHistoManager muHistos;
        JetHistoManager jetHistos;
        METHistoManager metHistos;
        TauHistoManager tauHistos;
        EventShapesHistoManager evShapesHistos;
        //DiLepRecoHistoManager recoHistos;
        //DiLepMCHistoManager mcHistos;
        BJetHistoManager bjetHistos;
};

#endif
