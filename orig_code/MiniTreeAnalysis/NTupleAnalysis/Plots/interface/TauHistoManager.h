#ifndef TauHistoManager_h
#define TauHistoManager_h

#include "../../../../MiniTreeFormat/NTFormat/interface/NTTau.h"
#include "../../../../MiniTreeFormat/NTFormat/interface/NTVertex.h"
#include "../../../../MiniTreeFormat/NTFormat/interface/NTEvent.h"

#include "HistoManager.h"


using namespace TopTree;

class TauHistoManager: public HistoManager{

  public:
        TauHistoManager();
        ~TauHistoManager();

        //Initialisation methods

        void CreateHistos(); /** Create a bunch of standard histos */

        //Fill methods
        void Fill(NTMET met, const vector<NTTau>& taus, const vector<NTVertex>& vertices, const vector<TLorentzVector>& tauObjTrig,const vector<TLorentzVector>& jetObjTrig, const int& maxSelStep, const int& iChannel, const int& iDataset, const float& weight, const float& NNOutput);
        void FillSelStep(NTMET met, const vector<NTTau>& taus, const vector<NTVertex>& vertices, const vector<TLorentzVector>& tauObjTrig, const vector<TLorentzVector>& jetObjTrig,const int& iSelStep, const int& iChannel, const int& iDataset, const float& weight, const float& NNOutput);


};

#endif
