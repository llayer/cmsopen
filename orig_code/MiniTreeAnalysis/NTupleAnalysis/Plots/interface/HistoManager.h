#ifndef HistoManager_h
#define HistoManager_h

// system include files
#include <memory>
#include <vector>

#include <TH1F.h>
#include <TH2D.h>
#include <TH3D.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TCanvas.h>
#include <THStack.h>
#include <TLegend.h>
#include <TFile.h>

#include "../../Tools/interface/Dataset.h"

using namespace std;

class HistoManager{
  public:
        HistoManager();
        ~HistoManager();

        //Initialisation methods
        void LoadDatasets(vector<Dataset> datasets);    
        void LoadSelectionSteps(vector<string> selectionSteps);
        void LoadSelectionSteps(vector<string> selectionSteps, vector<int> selStepColor);
        void LoadChannels(vector<string> channels);

        void AddHisto(string name, string title, string xaxis, const int& nbins, const float& min, const float& max);
        void CreateHisto(vector<TH1F>&, string, string, string, string, int, float, float );
        void CreateHisto(vector<TH1F>&, string, string, string, string, int, float *);
        void FillHisto(vector<TH1F>&, string, float, string, bool, float);   
        void DeleteHisto(vector<TH1F*>);   
        void FillHisto(vector<TH1F>&, const char*, float, string, bool, float);   
        void WriteMyHisto(vector<TH1F>&, string);   
        void StackHisto(THStack*,vector<TH1F>&, string, vector<string>);   
        void MergeHistos(vector<TH1F>& , string , string );
        
        void SetCutFlowAxisTitle(vector<TH1F>& , string , string );

//modif 2D : duplicate 1D functions to cope with 2D histograms --> get a "2D" in the name ;)
        void AddHisto2D(string name, string title, string xaxis, const int& nxbins, const float& xmin, const float& xmax, string yaxis, const int& nybins, const float& ymin, const float& ymax);
        void AddHisto2D_BC(string name, string title, string xaxis, const int& nxbins, float* rangex, string yaxis, const int& nybins, float* rangey); // allow to add 2D histo with center of bin information, used for example by BtagSFHistoManager
        void CreateHisto2D(vector<TH2D>&, string, string, string, int, float, float, string, int , float, float );
        void CreateHisto2D(vector<TH2D>&, string, string, string, string, int, float, float, int , float, float );
        void CreateHisto2D(vector<TH2D>&, string, string, string, string, int, float *, int, float * );
        void FillHisto2D(vector<TH2D>&, string, float, float, string, bool, float);   
        void FillHisto2D(vector<TH2D>&, const char*, float, float, string, bool, float);   
        void DeleteHisto2D(vector<TH2D*>);    
        void WriteMyHisto2D(vector<TH2D>&, string);   
        void MergeHistos2D(vector<TH2D>& , string , string );
//end modif 2D
        
        
        void CreateHisto3D(vector<TH3D>&, string, string, string, int, float, float, string, int , float, float, string, int , float, float );
        void CreateHisto3D(vector<TH3D>&, string, string, string, string, string, int, float, float, int , float, float, int , float, float );
        void FillHisto3D(vector<TH3D>&, string, float, float, float, string, bool, float);   
        void FillHisto3D(vector<TH3D>&, const char*, float, float, float, string, bool, float);   
        
        
        
        void WriteMyHisto3D(vector<TH3D>&, string);   

        
        
        
        pair<int,int> CanvasDivide(int NofPad);
        bool Check(const int& iChannel, const int& iDataset);/** check if these parameters doesn't exceeed the size of the associated vectors */
        bool Check(const int& iChannel, const int& iSelStep, const int& iDataset, const int& varNumber);/** Call this method to see if the size of Histos is correct or not */
        bool Check(vector<vector<TCanvas*> >& can, int ix, int iy);
        bool Check(vector<vector<vector<TCanvas*> > >& can, int ix, int iy, int iz);
        bool Check(vector<vector<vector<vector<TCanvas*> > > >& can, int ix, int iy, int iz, int izz);
        void Clear();

        //Fill method
        void Fill(const int& iChannel, const int& iSelStep, const int& iDataset, const int& varNumber, const float& value, const float& weight);   
//modif 2D
        void Fill2D(const int& iChannel, const int& iSelStep, const int& iDataset, const int& varNumber, const float& value, const float& weight);   
        bool Check2D(const int& iChannel, const int& iSelStep, const int& iDataset, const int& varNumber);/** Call this method to see if the size of Histos is correct or not */
//end modif 2D

        //Methods called once the main histos are filled
        void MergeChannels();/** Create new histos which are the sum over all channels registered */
        void MergeMCDatasets();/** Produce plots which are the sum over all MC datasets */
        void DoMCStack();/** Create THStack which are based on all histos over all MC datasets */
        void MCDataComparison();/** Superimpose THStack of MC and data in a TCanvas */
        void PlotsCutByCut(); /** See the evolution of the distribution cut after cut */
        void ProduceDebugCanvas(); /** Produce TCanvas with n plots each time */
        void Write(TDirectory* dir);/** Write all plots & canvas of the class in an rootfile */
        void SimpleWrite();

//modif 2D
        void MergeChannels2D();/** Create new histos which are the sum over all channels registered */
        void MergeMCDatasets2D();/** Produce plots which are the sum over all MC datasets */
        void PlotsCutByCut2D(); /** See the evolution of the distribution cut after cut */
        void Write2D(TDirectory* dir);/** Write all plots & canvas of the class in an rootfile */
        void SimpleWrite2D();
//end modif 2D

        vector<vector<vector<vector<TH1F> > > > Histos;/** dimensions: NofHistosxChannels.size()xSelectionSteps.size()xDatasets.size() */
        vector<string> Channels;
        vector<string> SelectionSteps;
        vector<int> SelStepColor;
        vector<Dataset> Datasets;

        vector<vector<vector<TH1F> > > SumMCDatasetsHistos; /** dimensions: NofHistosxSelectionSteps.size()xChannels.size() */
        vector<vector<vector<TH1F> > > SumDataDatasetsHistos; /** dimensions: NofHistosxSelectionSteps.size()xChannels.size() */
        vector<vector<vector<THStack*> > > MCStack;         /** dimensions: NofHistosxSelectionSteps.size()xChannels.size() */
        vector<vector<vector<TCanvas* > > > MCDataCompCanvas;       /** dimensions: NofHistosxChannels.size()xSelectionSteps.size() */
        vector<vector<TCanvas*> > MCDataCompCanvasDebug;            /** dimensions: NofHistosxChannels.size() - SelectionSteps.size() plots in a canvas*/
        vector<vector<vector<TCanvas*> > > CutEffectCanvas;         /** dimensions: NofHistosxChannels.size()xDatasets.size() */
        vector<vector<TCanvas*> > CutEffectCanvasDebug;             /** dimensions: NofHistosxChannels.size() - Datasets.size() plots in a canvas*/

//modif 2D
        vector<vector<vector<vector<TH2D> > > > Histos2D;/** dimensions: NofHistosxChannels.size()xSelectionSteps.size()xDatasets.size() */
        vector<vector<vector<TH2D> > > SumMCDatasetsHistos2D; /** dimensions: NofHistosxSelectionSteps.size()xChannels.size() */
        vector<vector<vector<TH2D> > > SumDataDatasetsHistos2D; /** dimensions: NofHistosxSelectionSteps.size()xChannels.size() */
        vector<vector<vector<vector<TCanvas*> > > > CutEffectCanvas2D;      /** dimensions: NofHistosxChannels.size()xNSel.size()xDatasets.size() */
        vector<vector<vector<TCanvas*> > > CutEffectCanvasDebug2D;          /** dimensions: NofHistosxChannels.size()xNSel.size() - Datasets.size() plots in a canvas*/
//end modif 2D

//modifdg
//      vector<TH1F> Hs;
//endmodifdg

};

#endif
