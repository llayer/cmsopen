#ifndef DiLepRecoHistoManager_h
#define DiLepRecoHistoManager_h

#include "../../../../MiniTreeFormat/NTFormat/interface/NTEvent.h"

#include "../../Selection/interface/DiLeptonSelection.h"
#include "HistoManager.h"


using namespace TopTree;

class DiLepRecoHistoManager: public HistoManager{

  public:
        DiLepRecoHistoManager();
        ~DiLepRecoHistoManager();

        //Initialisation methods

        void CreateHistos(); /** Create a bunch of standard histos */

        //Fill methods
        void Fill(NTEvent* event, const vector<NTMuon>& candMuon, const vector<NTElectron>& candElec, const int& maxSelStep, const int& iChannel, const int& iDataset, const float& weight);
        void FillSelStep(NTEvent* event, const vector<NTMuon>& candMuon, const vector<NTElectron>& candElec, const int& iSelStep, const int& iChannel, const int& iDataset, const float& weight);


};

#endif
