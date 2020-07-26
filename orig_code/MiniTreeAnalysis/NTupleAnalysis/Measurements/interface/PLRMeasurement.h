#ifndef PLRMeasurement_h
#define PLRMeasurement_h

#include <fstream>
#include "TMinuit.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TF2.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TGraph.h"
#include "TGraph2D.h"
#include "TLegend.h"
#include "THStack.h"
#include "TMath.h"
#include "TRandom.h"
#include "TStyle.h"

#include "AllDatasetUncertainties.h"
#include "../../Measurements/interface/ChannelDescription.h"
#include "DistributionDescription.h"
#include "PLRPlots.h"
#include "Interpolation.h"
//#include "../../Tools/interface/AnalysisEnvironmentLoader.h"



/**
        This class is dedicated to the computation of a generic PLR measurement.
        It describe the number of events in data ("Nobs") in a signal region, for a giving process ("SignalDatasetName"), for a given channel ("channel") having a branching ratio BRChannel.
        The likelihood take into account the poisson distribution of the data considering the expected number of events.
        This last can be expressed in terms of contribution of several processes ("datasets") which are determined from data with Data-Driven estimation or from Monte Carlo.
        In the case of MC expectation, the number of events is expressed as following:
        N = cross-section x Luminosity x Prod of efficiencies 
        The cross-section are stored in the "datasets" objects. The efficiencies can be loaded in several ways.
        A dedicated struct is used to store information about the data-driven estimation "DDEstimation".       

        The computation of the systematics come from differents sides, it can be:
        - the luminosity uncertainty
        - the cross-section uncertainties (bkg and signal if it's not the main parameter)
        - the data-driven estimation of the backgrounds
        - additionnal nuisance parameters which can be loaded. The effects on the number of events has to be expressed for each process (dataset)       

        The idea of the PLR is to have at least one parameter that will be free and which occurs in the description of the number of events.
        As this class is a generic class, the number of events cannot be expressed explicitely here, that's why there is a virtual methods: "NumberOfExpectedEvents"

        The minimisation will used the class TMinuit from ROOT.

        List of actions:
        - Constructor(SignalDatasetName, BR)
        - LoadDatasets()
        - AddUncertainty() : do this for all the uncertainties you want to consider
        - Fill(d,iu,...) : do this in the loop over the events for a dataset d and for each uncertainty (iu) in 3 conditions (mean, +/- 1 sigma)
        - PassTheSelection(d) : if the event pass the selection (will be used to compute a selection efficiency for a given MC dataset)
        - PassTheSelection(d,w) : if the event pass the selection (will be used to compute a selection efficiency for a given MC dataset)
        - SetNobs : at the end of the loop over events. Can come from MC or data
        - RunLikelihood() : can be done on MC-pseudo-exp or on data
        - Write()

        Format for the root-file:
                All histos are in the same directory and their as to be as following:
                $Channel_$HistoName_$COND_$DatasetName  where
                $Channel = is the name of the channel
                $Histoname = is the reference name of the histos. ex: Count = histo with one bin, Njets = jets multiplicity
                $COND can be "nominal" if no systematic is applied
                             $SystName-$Value  where $SystName is the name of the systematic concerned and $value can be "minus", "plus", or the value of the nuisance parameter (ex: SFb-0.8)
                $DatasetName = is the name of the process (dataset)


*/

struct PLRScan{
        vector<double> FreeParamValue;
        double LogLik;
};

struct AnimateGraph{
        bool doIt;
        string outfilename;
        int MaxNofIter;
        int iteration;
        TCanvas* canvas;
        vector<THStack> ChannelStack;
        vector<TH1F*> ParamEvolution;
        TH1F* LogLikEvolution;  
        TLegend* legend;
};

void LoadParameterInFCN(double& value, const string& name, Double_t*& par);

void PLRFcn(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag);/** Function which is used by TMinuit. It contains the computation of the likelihood */


class PLRMeasurement{

  public:
        PLRMeasurement();
        PLRMeasurement(const PLRMeasurement& meas);
        ~PLRMeasurement();
        
        //--- Initialisation methods ---// 
        void AddChannel(const string& name, const string& signalDatasetName, const double& BR);
        void AddFreeParameter(const string& name, const double& ExpectedValue, const int& npoints, const int& nbins,const double& xmin, const double& xmax,  const double& xErrmin, const double& xErrmax, const double& step, const bool& WithGraph);
        void AddNuisanceParameter(const string& name, bool isGlobal = false, bool WithGraph = false);
        void AddNuisanceParameter(const string& name, bool isGlobal, bool WithGraph, const float& Estimation, const float& ErrNeg, const float& ErrPos);
        void AddBkgDeterminationParameter();
        void AddHisto(const string& name, const int& dim);//to be implemented   
        void LoadHistos();
        void AddSignalSystematic(const vector<Systematic>& s);

        //--- Systematic ---//
        vector<vector<Systematic> > GetSignalSystematics() const {return SignalSyst;}
        
        //--- Datasets ---//    
        void LoadDatasets(const vector<Dataset>& datasets);
        vector<Dataset>& GetDatasets(){return datasets;};
        vector<string>& GetListOfDatasets();
        vector<bool> GetIsADDDatasetList()const; //used in DistributionDescription to reweight to Lumi or not

        //--- Uncertainties useless ? ---//
        void LoadUncertainties();

        //--- Luminosity ---//
        void SetLumi(double value, double absErr);
        double GetLumi()const;
        double GetLumiError()const;
        bool IsLumiASyst() const {return isLumiASyst;};
        void SetIsLumiASyst(bool isASyst);

        void SetHistoRootFileName(const string& name){HistoRootFileName = name;};
        string GetHistoRootFileName()const  {return HistoRootFileName;};

        //--- Signal ---//
        void SetSignalInfo(const string& SignalDatasetName, const bool& isASyst = false);
        double GetSignalXsection() const;
        vector<int> GetSignalDatasetIterator() const;
        vector<int> GetSignalDatasetIteratorForHisto() const;//doesn't count the dataset which are data

        //--- Access to channels ---//
        vector<ChannelDescription> GetChannels() {return channels;};
        vector<ChannelDescription>& GetChannelsAddress() {return channels;};

        //--- Consistency test ---//
        bool CheckDataset(int idataset) const; //return true if idataset is valid
        bool CheckUncertainty(int iUncertainty) const; // return true if iUncertainty is valid

        //--- Indice for the parameters of the likelihood ---//
        //change the following functions
        //int NofBkgXsectionUncertainties() const {return paramXsection;};

        //--- MC Cross sections ---//   
        vector<double> GetMCXsection() const;
        double GetMCXsection(int idataset)const;
        AllDatasetUncertainties GetMCXsectionUncertainty() const {return Xsection;};
        
        
        //--- Main methods ---//
        void ComputeLikelihood(vector<bool> fixParam, vector<double> ParamValues, bool WriteSysName = false, bool doCorrelationMatrixHisto = false, bool verbose = false);
        double EvaluateFCN(Double_t *par); /*par are the parameters:  Xsection+Lumi+Uncertainties */
        void RunLikelihood(bool doMCPseudoExp = false, int NofPseudoExp = 5000, bool SmearSystInPseudoExp = false, int verbosity = 0, bool SkipOneByOne = false);
        //void RunLikelihood(double Nobs, bool doMCPseudoExp = false, int NofPseudoExp = 5000, bool SmearSystInPseudoExp = false, int NofPoints = 400, double MinXforLogLPlot = 100, double MaxXforLogLPlot = 300, int verbosity = 0, bool SkipOneByOne = false); /** Nobs = nof observed events */     
        //--- Plotting functions ---//
        void Plot (TString S, int NofPoints, double Xmin, double Xmax, double minimumL, bool verbose = false);
        void Plot_PLR (int NofPoints, double Xmin, double Xmax, double *x, double *y, TGraph*& graph, int color = 1, string name = string(""));

        //--- Parameters ---//
        void BuildParametersList();//once the parameters are loaded !
        void ResetParameter(Parameter& par);
        Parameter GetParameter(const string& name, int& indice );
        Parameter GetParameter(const string& name);
        Parameter GetParameter(const int& i);
        vector<Parameter> GetParameters() const;
        vector<Parameter>& GetParametersAddress();
        vector<Parameter> GetEstimators() const;
        //Lumi and Xsection are not included in the two next methods
        vector<Parameter> GetEstimatorsNoGraph() const;
        vector<Parameter> GetEstimatorsGraph() const;
        vector<int> GetEstimatorsIndice() const;
        vector<double> GetEstimatorsValue() const;
        void SetParameterValue(const int& indice, const double& value);
        vector<Parameter> GetNuisanceParameters() const;
        //Lumi and Xsection are not included in the two next methods
        vector<Parameter> GetNuisanceParametersNoGraph() const;
        vector<Parameter> GetNuisanceParametersGraph() const;
        //vector<string> GetUncertainties();
        vector<pair<double,double> > GetEstimatorsSystError() const;
        vector<pair<double,double> > GetEstimatorsStatError() const;
        vector<pair<double,double> > GetEstimatorsCombinedError() const;
        vector<double> GetEstimatorsExpectation() const;

        //double IsolateSystError(double statError, double combinedError) const;

        //--- Extraction of min and errors from PLR ---//
        void PLRExamination(int NofPoints, double* X_PLR, double* Y_PLR, int indice);//indice: negative: stat, positive: indice of the systematics used, indice = NofParam -1 : all syst
        void PLRValuesWithPol2Fit(int NofPoints, double* X_PLR, double* Y_PLR, const double& Estimation, double& ErrNeg, double& ErrPos, double& Min, bool verbose = false, bool fit = false);
        //void PLRValuesWithPol3Fit(int NofPoints, double* X_PLR, double* Y_PLR, const double& Estimation, double& ErrNeg, double& ErrPos, double& Min, bool verbose = false, bool fit = false);
        void PLRValuesWithPol3Fit(TGraph* graph, int NofPoints, double* X_PLR, const double& Estimation, double& ErrNeg, double& ErrPos, double& Min, bool verbose = false, bool fit = false);
        void PLRValuesWithLinearAppromixation(int NofPoints, double* X_PLR, double* Y_PLR, const double& Estimation, double& ErrNeg, double& ErrPos, double& Min);
        
        //--- Methods to produce tables ---//
        void NumberOfEventsTable(ofstream& fout);
        void MinimizationTable(ofstream& fout);
        void EfficienciesTable(ofstream& fout);
        void UncertaintiesTable(ofstream& fout, bool SkipOneByOne = false);

        void TimingReport();
        bool GetDoPseudoExp() const {return doMCPSExp;}
        TRandom& GetRandom() {return rand;};


        //Interpolation
        string GetInterpolMethod() const {return InterpolMethod;};
        void SetInterpolMethod(string name);
        double Interpol(Double_t x, Double_t *par);
        double Interpol(double x, double mean, double effm1s, double effp1s);

        string GetSignalDatasetName() const {return SignalDatasetName;};

        //--- Animate graph ---//
        void DoAnimateGraph(){doAnim = true;};
        bool GetDoAnimateGraph()const {return doAnim;};
        AnimateGraph& GetAnimateGraph() {return anim;};
        void InitAnimateGraph();// used in the constructor
        void InitAnimateGraph(int MaxNofIter, string outfilename);
        void StartAnimateGraph();// used in the constructor
        void UpdateAnimateGraph(double* par, const double& LogLik);
        void WriteAnimateGraph();

        void Write(TFile* fout);

  private:

        double Pourcentage(const double& num, const double& denom)const;


        vector<ChannelDescription> channels;    

        double Lumi;
        double LumiError;
        bool isLumiASyst;

        string HistoRootFileName;
        
        string SignalDatasetName;
        bool isSignalXSASyst;
        
        //--- Estimation ---//
        double MinFCN;
        int NofFreeParam;
        vector<Parameter> parameters;
        //errors associated
        //vector<vector<pair<double,double> > > EstimatorsSysError; //** size: #estimators x #nuisances parameters //
        
        //--- Datasets ---//    
        vector<Dataset> datasets;

        //--- Parameters ---//
        int SizeOfParam;
        double* Param;
        double* ErrParam;
        double* ErrNegParam;
        double* ErrPosParam;
        double* ErrParaParam;
        double* Param_Min;
        double* ErrParam_Min;
        double* ErrNegParam_Min;
        double* ErrPosParam_Min;
        double* ErrParaParam_Min;

        string InterpolMethod;

        //--- Plots and display ---//
        vector<PLREstimation> PLREstim; // size = nof free param
        vector<PLRPlots> plots;
        TH2F* CorrelationMatrixHisto;
        
        //1D
        TGraph* StatGraph1D;
        TGraph* SystGraph1D;
        TGraph2D* StatGraph2D;
        TGraph2D* SystGraph2D;
        TF2* StatFit;
        TF2* SystFit;

        
        vector<PLRScan> plrScan;

        vector<vector<float> >normInit;//used to normalize the histograms before doing the draw / write
 
        //--- Systematics and uncertainties ---//
        vector<vector<Systematic> > SignalSyst;//size: #syst x #channels 
        
        //Useless ...
        // the size of the vector is #uncertainties. This vectors corresponds to the nuisance parameters
        vector<AllDatasetUncertainties>  uncertainties;
        //change
        AllDatasetUncertainties Xsection;/** It's the uncertainties due to the Xsection of the different processes **/

        //Pseudo-exp
        TRandom rand;
        bool doMCPSExp;

        //
        TStyle* gStyle;
        bool doAnim;
        AnimateGraph anim;

        clock_t time_1;
        clock_t time_2;
        clock_t time_3;
        clock_t time_4;

};

#endif
