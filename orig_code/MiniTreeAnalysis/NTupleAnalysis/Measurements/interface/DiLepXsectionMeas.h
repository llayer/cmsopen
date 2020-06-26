#ifndef DiLepXsectionMeas_h
#define DiLepXsectionMeas_h

#include <fstream>
#include <time.h>
#include "TMinuit.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TGraph.h"
#include "TLegend.h"
#include "TMath.h"
#include "TRandom.h"

#include "AllDatasetUncertainties.h"
#include "../../Tools/interface/AnalysisEnvironmentLoader.h"
#include "ChannelDescription.h"

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

//Structure which represents a process
// DD means Data Driven
/*
struct DDEstimation{
        bool isDDEstimated; // DD estimated[true] or from MC[false]
        bool isASyst; // Do we need to consider it has an uncertainty in the likelihood ?
        float NofEvts;
        // Errors on the NofEvts 
        float ErrNeg;   
        float ErrPos;
};
*/

void fcn(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag);/** Function which is used by TMinuit. It contains the computation of the likelihood */

class DiLepXsectionMeas{

  public:
        DiLepXsectionMeas(string channel, string SignalDatasetName, float BR);
        DiLepXsectionMeas(const DiLepXsectionMeas& meas);
        ~DiLepXsectionMeas();
        
        //--- Initialisation methods ---// 
        
        //--- Datasets ---//    
        void LoadDatasets(const vector<Dataset>& datasets);
        vector<Dataset> GetDatasets() const {return datasets;};
        
        //--- Uncertainties ---//       
        void AddUncertainty(string name, float* param);
        void AddUncertainty(string name, float paramMean, float paramMinus, float paramPlus);
        void AddUncertainty(string name, Syst SysValues);
        void AddUncertainty(string name, SystEffect SysValues);
        void LoadUncertainties(ifstream ifile); //extension ... 
        void Fill(int idataset, int iUncertainty, int icond = 0., float weight = 1.);
        void Fill(int idataset, string sysName, int icond = 0., float weight = 1.);
        
        //--- Selection Efficiencies ---//
        void PassTheSelection(int idataset);
        void PassTheSelection(int idataset, float weight);
        void SetSelEff(int idataset, float value);
        void SetSelEff(string DatasetName, float value);
        float GetSelEff(int idataset) const;
        void Compute(); /** compute the efficiencies */
        void PrintSelEff();

        //-- Getter on eff/param/uncertainties ---//
        vector<AllDatasetUncertainties> GetUncertainties()const {return uncertainties;};
        vector<vector<float*> > GetParameters();
        vector<vector<float*> > GetEfficiencies(); 
        float* GetParameters(int idataset, int iUncertainty);
        float* GetEfficiencies(int idataset, int iUncertainty); 
        void LoadEfficiencies(int iUncertainty, float effmean, float effm1s, float effp1s);
        vector<string> GetListOfDatasets() const;
        vector<string> GetListOfUncertainties() const;

        //--- Luminosity ---//
        void SetLumi(float value, float absErr);
        double GetLumi()const;
        double GetLumiError()const;
        bool IsLumiASyst() const {return isLumiASyst;};
        void SetIsLumiASyst(bool isASyst) { isLumiASyst = isASyst;};
        
        //--- Nof observed events ---//
        void SetNobs(double nobs);
        double GetNobs() const;

        //--- Getter on channel,signal,BR ---//
        string GetName() const {return channel;};
        string GetSignalDatasetName() const;
        double GetBRChannel() const;

        //--- NofEvts ---//
        float NumberOfExpectedEvents(bool write = false);
        float NumberOfExpectedEvents(int idataset, bool verbose = false) const;
        float NumberOfExpectedEvents (int idataset, bool computeSlope, double par, float lumi, bool verbose = false) const;
        float NumberOfExpectedEvents(double* par, bool verbose = false, bool write = false);/** Compute the nof evts according to the par **/
        float SystCorrectionOnNofEvents(int idataset, int iUncertainty, double par) const; /** correction on expected nof events for one systematic **/
        float SystCorrectionOnNofEvents(int idataset, double* par, bool verbose) const; /** correction on expected nof events for all systematics [product] **/
        int NumberOfExpectedEventsRandom(TRandom& rand, bool randoSyst, bool verbose); /** Compute the nof evts: randomisation due to systematics [randoSyst] and then statistic - It's important to pass the address of TRandom in order to not reinitialize the seed **/


        bool CheckDataset(int idataset) const; //return true if idataset is valid
        bool CheckUncertainty(int iUncertainty) const; // return true if iUncertainty is valid

        //--- Indice for the parameters of the likelihood ---//
        int Offset() const { return offset;};
        int NofBkgXsectionUncertainties() const {return paramXsection;};
        int GetNofParam()const {return NofParam;};
        int NofUncertainties()const{return uncertainties.size();};
        int IndiceForXsection(int idataset) const;
        int IndiceForUncertainty(int iUncertainty) const;

        //--- MC Cross sections ---//   
        vector<float> GetMCXsection() const;
        float GetMCXsection(int idataset)const;
        AllDatasetUncertainties GetMCXsectionUncertainty() const {return Xsection;};
        
        //-- Data Driven estimation ---//
        void BkgDataDrivenEstimation(string Process, float NofEvts, float ErrNeg, float ErrPos, bool isASyst = true );/** "Process" should correspond to a datasetname. NofEvts estimated with a error of -ErrNeg/+ErrPos */
        vector<DDEstimation> GetDDEstimation() const {return DDEstim;};

        
        //--- Main methods ---//
        void ComputeLikelihood(vector<bool> fixParam, vector<double> ParamValues, bool WriteSysName = false, bool doCorrelationMatrixHisto = false, bool verbose = false);
        double XsectionFCN(Double_t *par); /*par are the parameters:  Xsection+Lumi+Uncertainties */
        void XExtraction(double Nobs, bool doMCPseudoExp = false, int NofPseudoExp = 5000, bool SmearSystInPseudoExp = false, int NofPoints = 400, double MinXforLogLPlot = 100, double MaxXforLogLPlot = 300, int verbosity = 0, bool SkipOneByOne = false); /** Nobs = nof observed events */ 
        //--- Plotting functions ---//
        void Plot (TString S, int NofPoints, double Xmin, double Xmax, double minimumL, bool verbose = false);
        void Plot_PLR (int NofPoints, double Xmin, double Xmax, double *x, double *y, TGraph*& graph, int color = 1, string name = string(""));

        //--- Results ---//
        double GetXsectionEstim() const{ return XsectionEstim;};
        double GetXsectionEstimWithAllSyst() const{ return XsectionEstimWithAllSyst;};
        double GetXsectionEstimStat() const{ return XsectionEstimStat;};
        double GetXsectionExpected() const {return ExpectedXsection;};
        float IsolateSystError(float statError, float combinedError) const;

        //--- Extraction of min and errors from PLR ---//
        void PLRExamination(int NofPoints, double* X_PLR, double* Y_PLR, int indice);//indice: negative: stat, positive: indice of the systematics used, indice = NofParam -1 : all syst
        void PLRValuesWithPol2Fit(int NofPoints, double* X_PLR, double* Y_PLR, float& ErrNeg, float& ErrPos, float& Min);
        void PLRValuesWithLinearAppromixation(int NofPoints, double* X_PLR, double* Y_PLR, float& ErrNeg, float& ErrPos, float& Min);
        
        //--- Methods to produce tables ---//
        void NumberOfEventsTable(ofstream& fout);
        void MinimizationTable(ofstream& fout);
        void EfficienciesTable(ofstream& fout);
        void UncertaintiesTable(ofstream& fout, bool SkipOneByOne = false);

        void TimingReport();


        void Write(TFile* fout);

  private:

        string channel;
        string SignalDatasetName;
        double BRChannel;

        double Lumi;
        double LumiError;
        double Nobs;    /** nof observed events */
        
        //--- Estimation ---//
        double MinFCN;
        double XsectionEstim; /** estimation with all free parameters **/
        double XsectionEstimWithAllSyst; /** estimation with PLR, all systematics considered **/
        double XsectionEstimStat; /** estimation with PLR, no systematics included **/ 
        double ExpectedXsection;
        
        //--- Nof evts from the several process ---//
        vector<float> NofEvtsContribMC;  /** nof evts from MC - no systematic **/
        vector<float> NofEvtsContribFitted; /** nof evts estimated with the PLR - all systematics considered **/
        
        // the size of the vector is the #datasets
        vector<float> InitNofEvents;
        vector<float> SelEvents;
        vector<float> SelEff;
        vector<Dataset> datasets;

        
        //--- Parameters ---//
        int offset;
        int paramXsection;
        int NofParam; // offset+ paramXsection + uncertainties.size()

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

        //--- Plots and display ---//
        int MaxNofPoints; 
        double* X_PLR;
        double* Y_PLR;
        double Ymin_plot;
        TCanvas* c;
        TLegend* leg;
        TH2F* histo;
        TH2F* CorrelationMatrixHisto;
        TGraph* StatGraph;
        bool graphsDeclared;
        TGraph** graphs;
        //vector<TGraph*> graphs;
        TH1F *Xseccentralvalue_wosys; 
        TH1F *Erreurmean_wosys;
        TH1F *Erreurneg_wosys; 
        TH1F *Erreurpos_wosys;
        TH1F *PullErreurmean_wosys;
        TH1F *PullErreurneg_wosys; 
        TH1F *PullErreurpos_wosys;
        TH1F *PullErreur2cases_wosys;
        TH1F *Xseccentralvalue_wsys;
        TH1F *Erreurmean_wsys;
        TH1F *Erreurneg_wsys; 
        TH1F *Erreurpos_wsys; 
        TH1F *PullErreurmean_wsys;
        TH1F *PullErreurneg_wsys; 
        TH1F *PullErreurpos_wsys;
        TH1F *PullErreur2cases_wsys;
        bool ParameterHistoDeclared; 
        TH1F** ParametersHisto;       
 
        //--- Systematics and uncertainties ---//       
        vector<DDEstimation> DDEstim;
        // the size of the vector is the #uncertainty
        vector<AllDatasetUncertainties> uncertainties;
        AllDatasetUncertainties Xsection;
        vector<string> SysName;
        vector<pair<double,double> >XsectionEstimSysError; 
        pair<double,double> XsectionEstimStatError; 
        pair<double,double> XsectionEstimAllError; 
        bool isLumiASyst;

        clock_t time_1;
        clock_t time_2;
        clock_t time_3;
        clock_t time_4;

};

#endif
