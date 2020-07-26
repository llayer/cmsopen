#ifndef MuonHistoManager_h
#define MuonHistoManager_h

#include "../../../../MiniTreeFormat/NTFormat/interface/NTMuon.h"

#include "HistoManager.h"


using namespace TopTree;

class MuonHistoManager: public HistoManager{

  public:
        MuonHistoManager();
        ~MuonHistoManager();

        //Initialisation methods

        void CreateHistos(); /** Create a bunch of standard histos */

        //Fill methods
        void Fill(const vector<NTMuon>& muons, const int& maxSelStep, const int& iChannel, const int& iDataset, const float& weight);
        void FillSelStep(const vector<NTMuon>& muons, const int& iSelStep, const int& iChannel, const int& iDataset, const float& weight);


};

#endif
