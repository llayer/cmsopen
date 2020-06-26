#ifndef PLRPlotsFurnisher_h
#define PLRPlotsFurnisher_h 

#include <iomanip>
#include <iostream>
#include <limits.h>
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


#include "PLRHistoCollection.h"
#include "../../Plots/interface/HistoManager.h"
#include "../../Selection/interface/DiLeptonSelectionSystematics.h"


class PLRPlotsFurnisher{

        public:
                PLRPlotsFurnisher();
                ~PLRPlotsFurnisher();


                void LoadSetup(AnalysisEnvironmentLoader& anaEL,vector<Dataset> datasets, vector<string> CutName, vector<string> Channels);

                void PrintSystematics();
                void PrintObservables();

                void CreateHistos();
                
                //Do it once per event
                void LoadEvent (const DiLeptonSelection& sel, const string& ChannelName, const bool& IsSignal,
                const int& itypemc, const int& tmeme,  double weight, Dataset& dataset,
                const double& sf_trig_mu,const double& sf_trig_emu, const double& sf_trig_ee,
                const double& sf_e,const double& sf_mu, 
                const double& sf_dy_ee,const double& sf_dy_mm, const double& sf_dy_em, const int& nrecoel,
                const string& CandType0, const double& LeptonSF);
                //void LoadSelection (const DiLeptonSelection& sel);
                //void FillHistos(bool FlagB, bool IsSignal, float weight, string DatasetName);
                //

                vector<ExtendedSyst> GetListOfSystematics() {return systList;};
                void WriteHistos();

        private:
                void LoadInfoForHistoManager(vector<Dataset> datasets, vector<string> CutName, vector<string> Channels);
                void LoadSystematics(AnalysisEnvironmentLoader& anaEL);
                void LoadObservables(AnalysisEnvironmentLoader& anaEL);
                int GetiDataset(string DatasetName);

                vector<string> recognizedSystList;
                vector<ExtendedSyst> systList;

                vector<string> recognizedObs;
                vector<Observable> obs;

                vector<Dataset> datasets;
                vector<string> channels;

                //Book keeping of my histos
                HistoManager MyhistoManager;
                PLRHistoCollection histoCollection;//histo with systematics
                PLRHistoCollection RefHistoCollection;//reference histogram

};

#endif

