#ifndef BtagSFHistoManager_h
#define BtagSFHistoManager_h

#include "../../../../MiniTreeFormat/NTFormat/interface/NTJet.h"

#include "HistoManager.h"


using namespace TopTree;

class BtagSFHistoManager: public HistoManager{

  public:
        BtagSFHistoManager();
        ~BtagSFHistoManager();

        //Initialisation methods
        void CreateHistos( const int& nb, float* valsfb,  const int& nl, float* valsfl);

        //Fill methods
        void Fill(const int& sfbval, const int& sflval,const int& iChannel, const int& iNjetSel, const int& iDataset, const vector<float>& weight);
        void FillSelStep(TH2D& hh, const int& sfbval, const int& sflval, const int& iNjetSel, const int& iChannel, const int& iDataset, float weight);

        //compute efficiency of the selection
        void Compute();
        void EffCompute(TH2D& hheff, TH2D& hhnum, TH2D& hhdenom);

        //plot TGraph with efficiency
        void PlotTGraph(TDirectory* dir);



};

#endif
