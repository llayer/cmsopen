#ifndef DistributionDescription_h
#define DistributionDescription_h
#include <vector>
#include <string>
#include "TMath.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TGraphErrors.h"
#include "TF1.h"
#include "TRandom.h"
#include "Structures.h"
#include "AllDatasetUncertainties.h"
#include "../../Tools/interface/Dataset.h"
#include "Interpolation.h"

using namespace std;

/**
        This can work for 1,2 or 3 dimensions distributions. It uses TH1/2/3F to store the events and efficiencies. 

**/


class DistributionDescription{
        
  public:
        DistributionDescription(const string& name, const string& channel, const int& dim, const vector<bool>& DatasetIsDD, string InterpolMethod = string("linear"));
        DistributionDescription(const DistributionDescription& dd);
        ~DistributionDescription();

        //--- accessors ---//
        void SetNofDimensions(const int& dim);
        int GetNofDimensions()const;
        void SetName(const string& name);
        string GetName()const;
        void SetChannel(const string& channel);
        string GetChannel()const;

        //--- Initialition of the histos ---//

        //--- Loading ---//
        void LoadHistos(TFile* fin, const vector<Dataset>& datasets, const vector<Parameter>& uncert, const vector<Parameter>& GraphUncert, const vector<Parameter>& estim, const vector<Parameter>& GraphEstim);
        void LoadHistos(TFile* fin, const vector<Dataset>& datasets);
        void LoadHistosUncertainties(TFile* fin, const vector<Parameter>& uncertainties, const vector<Dataset>& datasets, bool ParAreUncertainties = true);// if ParAreUncertainties = false: it is estimators and not uncertainties
        void LoadGraphUncertainties(TFile* fin, const vector<Parameter>& GraphUncerties, const vector<Dataset>& datasets, bool ParAreUncertainties = true);// if ParAreUncertainties = false: it is estimators and not uncertainties
        //--- should be useless ---//
        void LoadUncertainties(TFile* fin, const string& dirname, const vector<AllDatasetUncertainties>& uncert);
        void LoadEfficiencies(const int& idataset, float eff);
        
        bool ClosureTestOnSumEfficiencies();    
        bool HistoConsistency();

        //--- Accessors to the histos ---//
        // missing
        TH1F* Get1DNobs() const;
        TH2F* Get2DNobs() const;
        TH3F* Get3DNobs() const;

        //--- 1D ---//  
        TH1F* Get1DPseudoData() const;
        void Compute1DFinalDistrib(const vector<double>& parUnc, const vector<double>& GraphUnc, const vector<double>& bkgNorm, const double& LumiFactor, const double& SignalFactor, const vector<int>& iSignalDatasets);
        TH1F* Compute1DPseudoData(TRandom& rand, const vector<double>& parUnc, const vector<double>& GraphUnc, const vector<double>& bkgNorm, const double& LumiFactor, const double& SignalFactor, const vector<int>& iSignalDatasets) const;
        TH1F* Get1DNexp(const vector<double>& parUnc, const vector<double>& GraphUnc, const vector<double>& bkgNorm, const double& LumiFactor, const double& SignalFactor, const vector<int>& iSignalDatasets) const;
        vector<TH1F*>& Get1DProcessExp(){ return NofExpectedEvtsInit_1D;};
        vector<TH1F*>& Get1DProcessFinal(){ return NofExpectedEvtsFinal_1D;};
        void NormalizeDistrib(const vector<float>& norm);
        //--- 2D ---//
        TH2F* Get2DPseudoData() const;
        void Compute2DFinalDistrib(const vector<double>& parUnc, const vector<double>& GraphUnc, const vector<double>& parEstim, const vector<double>& GraphEstim, const vector<double>& bkgNorm, const double& LumiFactor, const double& SignalFactor, const vector<int>& iSignalDatasets);
        TH2F* Compute2DPseudoData(TRandom& rand, const vector<double>& parUnc, const vector<double>& GraphUnc, const vector<double>& parEstim, const vector<double>& GraphEstim, const vector<double>& bkgNorm, const double& LumiFactor, const double& SignalFactor, const vector<int>& iSignalDatasets) const;
        TH2F* Get2DNexp(const vector<double>& parUnc, const vector<double>& GraphUnc, const vector<double>& parEstim, const vector<double>& GraphEstim, const vector<double>& bkgNorm, const double& LumiFactor, const double& SignalFactor, const vector<int>& iSignalDatasets) const;
        vector<TH2F*>& Get2DProcessExp(){ return NofExpectedEvtsInit_2D;};
        vector<TH2F*>& Get2DProcessFinal(){ return NofExpectedEvtsFinal_2D;};
        //--- 3D --- not yet implemented//
        TH3F* Get3DNexp(const vector<double>& parUnc, const double& LumiFactor) const;

        //Interpolation
        string GetInterpolMethod() const {return InterpolMethod;};
        void SetInterpolMethod(string name) {InterpolMethod = name;};
        double Interpol(Double_t x, Double_t *par) const;
        double Interpol(double x, double mean, double effm1s, double effp1s) const;

        //Computing factor from the fit function associated to the graph 
        float GetFactorFromFitFunctionOfTheGraph(const TGraphErrors* graph, float xValue, string MethodName, float YThreshold = 5.) const;
        void CheckGraph();
        
  private:
        string Name;
        string Channel;
        string InterpolMethod;

        vector<bool> DatasetIsDD;
        
        int NofDimensions;
        //string* DimName; //** Name to each dimension -- useless ?? **//
        bool initialized;

        //--- Nof observed events in data ---//
        TH1F*  Nobs_1D;
        TH2F*  Nobs_2D;
        TH3F*  Nobs_3D;

        //---- Pseudo-data --- (P.Exp.) //
        TH1F*  NobsPE_1D;
        TH2F*  NobsPE_2D;
        TH3F*  NobsPE_3D;
        
        //--- Histos per dataset (MC) ---//
        // -- Ref -- //
        vector<TH1F*> Histo_1D;         
        vector<TH2F*> Histo_2D;         
        vector<TH3F*> Histo_3D; 
        // -- Uncertainties -- //
        vector<vector<TH1F*> > Histo_1D_UncertMinus; // size = #uncert x # datasets             
        vector<vector<TH1F*> > Histo_1D_UncertPlus;             
        vector<vector<TH2F*> > Histo_2D_UncertMinus; // size = #uncert x # datasets             
        vector<vector<TH2F*> > Histo_2D_UncertPlus;             
        vector<vector<TH3F*> > Histo_3D_UncertMinus; // size = #uncert x # datasets             
        vector<vector<TH3F*> > Histo_3D_UncertPlus;             
        // -- Estimators -- //
        vector<vector<TH1F*> > Histo_1D_EstimMinus; // size = #uncert x # datasets              
        vector<vector<TH1F*> > Histo_1D_EstimPlus;              
        vector<vector<TH2F*> > Histo_2D_EstimMinus; // size = #uncert x # datasets              
        vector<vector<TH2F*> > Histo_2D_EstimPlus;              
        vector<vector<TH3F*> > Histo_3D_EstimMinus; // size = #uncert x # datasets              
        vector<vector<TH3F*> > Histo_3D_EstimPlus;              

        //--- With Graph ---//
        vector<vector<vector<TGraphErrors*> > > Graph_Uncert; // size = #uncert x # datasets #bins of the histos (sequentially x then y(if 2/3D) then z(if 3D))
        vector<vector<vector<TGraphErrors*> > > Graph_Estim; // size = #estimators x # datasets #bins of the histos (sequentially x then y(if 2/3D) then z(if 3D))

        //--- In principle it should be useless ---//
        vector<float> Efficiencies; //from SR to the integral of the histos - size = #datasets 
        //--- Uncertainties on the efficiencies due to the nuisance parameters ---//    
        vector<AllDatasetUncertainties> uncertainties;//size = #nuisance parameters
        //--- 
                
        
        //--- NumberOfExpectedEvents per dataset ---//
        //Init = with all default value - useless = Histo_1D
        vector<TH1F*> NofExpectedEvtsInit_1D; 
        vector<TH2F*> NofExpectedEvtsInit_2D; 
        vector<TH3F*> NofExpectedEvtsInit_3D;
        //Sum over all the datasets
        TH1F* NofExpectedEvtsSumInit_1D; 
        TH2F* NofExpectedEvtsSumInit_2D; 
        TH3F* NofExpectedEvtsSumInit_3D;
        //Final = after PLR
        vector<TH1F*> NofExpectedEvtsFinal_1D; 
        vector<TH2F*> NofExpectedEvtsFinal_2D; 
        vector<TH3F*> NofExpectedEvtsFinal_3D;
        //Sum over all the datasets: current plot
        TH1F* NofExpectedEvtsSumCurrent_1D; 
        TH2F* NofExpectedEvtsSumCurrent_2D; 
        TH3F* NofExpectedEvtsSumCurrent_3D;
        //Sum over all the datasets: final plot
        TH1F* NofExpectedEvtsSumFinal_1D; 
        TH2F* NofExpectedEvtsSumFinal_2D; 
        TH3F* NofExpectedEvtsSumFinal_3D;

};

#endif
