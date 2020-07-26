#ifndef EventShapesHistoManager_h
#define EventShapesHistoManager_h

#include "../../../../MiniTreeFormat/NTFormat/interface/NTEvent.h"

#include "../../Tools/interface/EventShapes.h"
#include "HistoManager.h"

#include <TRandom.h>

using namespace TopTree;

class EventShapesHistoManager: public HistoManager{

  public:
        EventShapesHistoManager();
        ~EventShapesHistoManager();

        //Initialisation methods

        void CreateHistos(); /** Create a bunch of standard histos */

        //Fill methods
        void Fill(const vector<NTJet>& candJet, const vector<NTJet>& candJetTauClean, const int& maxSelStep, const int& iChannel, const int& iDataset, const float& weight, const vector<float>& NNOutput);
        void FillSelStep(const vector<NTJet>& candJet, const vector<NTJet>& candJetTauClean, const int& iSelStep, const int& iChannel, const int& iDataset, const float& weight, const vector<float>& NNOutput);
  
  private:
  
  float sumOfWeights[10];
  TRandom rand1; 
 

};

#endif
