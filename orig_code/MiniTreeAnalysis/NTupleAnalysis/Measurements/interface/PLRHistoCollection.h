#ifndef PLRHistosCollection_h
#define PLRHistosCollection_h 

#include <iomanip>
#include <iostream>
#include <limits.h>
#include <vector>
#include <string>
#include "../../../../MiniTreeFormat/NTFormat/interface/NTEvent.h"

//NTupleAnalysis classes
#include "../../Selection/interface/DiLeptonSelection.h"
#include "../../Selection/interface/SelectionTable.h"
#include "../../Tools/interface/Dataset.h"
#include "../../Tools/interface/AnalysisEnvironmentLoader.h"
#include "../../Plots/interface/DiLepAnaHistoManager.h"
//#include "../../BckgdEstimation/interface/DYEstimation.h"
//#include "../../BckgdEstimation/interface/MMEstimation.h"


#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>


using namespace std;



class PLRHistoCollection {

        public: 

                PLRHistoCollection();
                ~PLRHistoCollection();

                //AddObservable(def)
                //SetChannel(const vector<string> channels);

                void LoadSetup(vector<string> channels, vector<ExtendedSyst> systematics, vector<Observable> observables);
        
                void CreateHistos();
                vector<TH1F>& GetHisto1D(int iChannel, int iObservables, int iSystematic, int iSystValue);//iSystValue: 0: minus 1: plus if discretized (-1s,+1s) or could be scanned ...
                vector<TH2D>& GetHisto2D(int iChannel, int iObservables, int iSystematic, int iSystValue);
                void FillHistos(bool FlagB, bool IsSignal, float weight);       


        private:
                vector<string> channels;
                vector<ExtendedSyst> systematics;
                vector<Observable> observables;  //name and dimension

                vector<vector<vector<vector<vector<TH1F> > > > > histos1D;
                vector<vector<vector<vector<vector<TH2D> > > > > histos2D;

                vector<TH1F> null1;
                vector<TH2D> null2;
};


#endif
