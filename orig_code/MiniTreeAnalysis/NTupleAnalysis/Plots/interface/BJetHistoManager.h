#ifndef BJetHistoManager_h
#define BJetHistoManager_h

#include "../../../../MiniTreeFormat/NTFormat/interface/NTJet.h"

#include "HistoManager.h"


using namespace TopTree;

class BJetHistoManager: public HistoManager{

  public:
        BJetHistoManager();
        ~BJetHistoManager();

        //Initialisation methods

        void CreateHistos(); /** Create a bunch of standard histos */

        //Fill methods
        void Fill(const size_t& nbtagjet, const int& maxSelStep, const int& iChannel, const int& iDataset, const float& weight,const float& weightb);
        void FillSelStep(const size_t& nbtagjet, const int& iSelStep, const int& iChannel, const int& iDataset, const float& weight);
        void FillTemporary(const size_t& nbtagjet, const int& iSelStep, const int& iChannel, const int& iDataset, const float& weight);  /** fill temporary histogram to be able to reweight correctly the first bin in case of SF method */


};

#endif
