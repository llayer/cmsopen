#ifndef CombinedDiLepXsectionMeas_h
#define CombinedDiLepXsectionMeas_h

#include <fstream>
#include "TMinuit.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TGraph.h"
#include "TLegend.h"
#include "TMath.h"
#include "TRandom.h"

#include "AllDatasetUncertainties.h"
#include "DiLepXsectionMeas.h"
#include "../../Tools/interface/AnalysisEnvironmentLoader.h"

/**
        This class is dedicated to the computation of the Xsection with all the systematics uncertainties associated.
        List of actions:
        - Constructor(SignalDatasetName, BR)
        - LoadDatasets()
        - AddUncertainty() : do this for all the uncertainties you want to consider
        - Fill(d,iu,...) : do this in the loop over the events for a dataset d and for each uncertainty (iu) in 3 conditions (mean, +/- 1 sigma)
        - PassTheSelection(d) : if the event pass the selection (will be used to compute a selection efficiency for a given MC dataset)
        - PassTheSelection(d,w) : if the event pass the selection (will be used to compute a selection efficiency for a given MC dataset)
        - SetNobs : at the end of the loop over events. Can come from MC or data
        - XExtraction() : can be done on MC-pseudo-exp or on data
        - Write()

*/

//--- Structure which represent an uncertainty: can be 0 or 100% correlated between the channels
struct CombinedUncertainty{
        string name;
        bool correlated; //0 or 100% correlated among the channels
};


void CombinedFcn(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag);

class CombinedDiLepXsectionMeas{

  public:
        CombinedDiLepXsectionMeas();
        ~CombinedDiLepXsectionMeas();

        //--- Initialisation Methods ---//
        void AddChannel(const DiLepXsectionMeas& m, bool verbose = false);
        void LoadDatasets(const vector<Dataset>& datasets);
        bool CheckDatasets(vector<Dataset> DS1, vector<Dataset> DS2) const;
        bool CheckLumi(float Lumi1, float LumiErr1, float Lumi2, float LumiErr2) const;
        void AddSystematic(string name, bool correlated = true);
        bool CheckSystematicList(const DiLepXsectionMeas& m) const;

        //--- Getter on datasets/paramets/uncertainties ---// 
        vector<CombinedUncertainty> GetUncertainties(){return uncertainties;};
        vector<vector<float*> > GetParameters();
        vector<Dataset> GetDatasets() const {return datasets;};
        vector<string> GetListOfDatasets() const;
        vector<string> GetListOfUncertainties() const;

        //--- Luminosity ---//
        void SetLumi(float value, float absErr);
        double GetLumi()const;
        double GetLumiError()const;
        bool IsLumiASyst() const {return isLumiASyst;};
        void SetIsLumiASyst(bool isASyst) { isLumiASyst = isASyst;};

        //--- MC cross-section ---//    
        vector<float> GetMCXsection() const;
        float GetMCXsection(int idataset)const;
        AllDatasetUncertainties GetMCXsectionUncertainty() const {return Xsection;};
        
        int GetNofParam()const {return NofParam;};
        double GetXsectionEstim() const{ return XsectionEstim;};

        //--- Main methods ---//
        void ComputeLikelihood(vector<bool> fixParam, vector<double> ParamValues, bool WriteSysName = false,  bool doCorrelationMatrixHisto = false, bool verbose = false);
        double XsectionFCN(Double_t *par); /*par are the parameters:  Xsection+Lumi+Uncertainties */
        void XExtraction(int NofPoints = 400, double MinXforLogLPlot = 100, double MaxXforLogLPlot = 300, int verbosity = 0, bool SkipOneByOne = false); /** Nobs = nof observed events */      

        //--- Plotting methods ---//
        void Plot (TString S, int NofPoints, double Xmin, double Xmax, double minimumL, bool verbose = false);
        void Plot_PLR (TString S, int NofPoints, double Xmin, double Xmax, double *x, double *y, TGraph*& graph, int color = 1, string name = string(""));
        
        float IsolateSystError(float statError, float combinedError) const;

        //--- Tables ---//
        void MinimizationTable(ofstream& fout);
        void UncertaintiesTable(ofstream& fout, bool SkipOneByOne = false);

        //--- Getter on channels ---//
        vector<const DiLepXsectionMeas*> GetChannels()  {return channels;};
        const DiLepXsectionMeas* GetChannel(int i) const; //to be implemented
        const DiLepXsectionMeas* GetChannel(string name) const; // to be implemented

        void Write(TFile* fout);

  private:
        vector<Dataset> datasets;
        vector<const DiLepXsectionMeas*> channels;
        vector<CombinedUncertainty> uncertainties;
        AllDatasetUncertainties Xsection;

        double Lumi;
        double LumiError;
        bool isLumiASyst;
        //double Nobs;  /** nof observed events */

        //--- Estimation ---//
        int NofParam;
        double MinFCN;
        double XsectionEstim;

        int SizeOfParam;
        vector<string> ParamNames;
        double* Param;
        double* ErrParam;
        double* ErrNegParam;
        double* ErrPosParam;
        double* Param_Min;
        double* ErrParam_Min;
        double* ErrNegParam_Min;
        double* ErrPosParam_Min;
        
        //--- Plots ---//
        int MaxNofPoints;
        double* X_PLR;
        double* Y_PLR;
        double Ymin_plot;
        TCanvas* c;
        TLegend* leg;
        TGraph** graphs;
        TH2F* histo;
        TH2F* CorrelationMatrixHisto;
        TGraph* StatGraph;
        bool graphsDeclared;
        //other histos
        TH1F *Xseccentralvalue_wosys; 
        TH1F *Erreurparabolic_wosys; 
        TH1F *Erreurmean_wosys;
        TH1F *Erreurneg_wosys; 
        TH1F *Erreurpos_wosys;
        TH1F *Xseccentralvalue_wsys;
        TH1F *Erreurparabolic_wsys; 
        TH1F *Erreurmean_wsys;
        TH1F *Erreurneg_wsys; 
        TH1F *Erreurpos_wsys;
 
        //--- Uncertainties ---//
        vector<string> SysName;
        vector<pair<double,double> >XsectionEstimSysError; 
        pair<double,double> XsectionEstimStatError; 
        pair<double,double> XsectionEstimAllError; 

};

#endif
