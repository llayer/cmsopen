#ifndef PLRPlots_h
#define PLRPlots_h

#include <string>
#include <vector>
#include <TH1F.h>
#include <TH2F.h>
#include <TGraph.h>
#include <TGraph2D.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TLegend.h>

using namespace std;

class PLRPlots{

 public:

        PLRPlots(string name, float ExpectedValue, int nbins, float xmin, float xmax, float ErrMin, float Errmax, int NofUncert = 0);
        ~PLRPlots();

        void SetName(string Name) {name = Name;};
        string GetName() const {return name;};
        void Write(TFile* fout);        

        void Fill(float Estim, float NegErr, float PosErr, bool stat = true);//stat or stat+syst
        void Fill(float StatEstim, float SystEstim);
        //void FillUncertInput(int iuncert, float value);
        //void FillUncertEstim(int iuncert, float value);
        void FillUncert(int iuncert, float inputValue, float estimValue);
        
        //void SetStatPLRPoints1D(vector<double> Xvalues, vector<double> Yvalues);  
        //void SetAllSystPLRPoints1D(vector<double> Xvalues, vector<double> Yvalues);  

 private:
        string name;
        float ExpectedValue;
        int NofUncert;
        
        TCanvas* c;
        TLegend* leg;
        TH2F* histo;
        TH2F* CorrelationMatrixHisto;
        bool graphsDeclared;

        TGraph* StatGraph1D;
        TGraph* AllSystGraph1D;
        TGraph** graphs1D;
        TGraph2D* StatGraph2D;
        TGraph2D* AllSystGraph2D;
        TGraph2D** graphs2D;
        //dim>2 = stocker info ds array

        bool ParameterHistoDeclared; 
        TH1F* ParametersMeanValueStatHisto;       
        TH1F* ParametersNegErrStatHisto;       
        TH1F* ParametersPosErrStatHisto;       
        TH1F* ParametersAvErrStatHisto;       
        TH1F* ParametersPullNegErrStatHisto;       
        TH1F* ParametersPullPosErrStatHisto;       
        TH1F* ParametersPullAvErrStatHisto;       
        TH1F* ParametersMeanValueSystHisto;       
        TH1F* ParametersNegErrSystHisto;       
        TH1F* ParametersPosErrSystHisto;       
        TH1F* ParametersAvErrSystHisto;       
        TH1F* ParametersPullNegErrSystHisto;       
        TH1F* ParametersPullPosErrSystHisto;       
        TH1F* ParametersPullAvErrSystHisto;       
        TH1F* ParameterStatMeanMinusSystMeanHisto;

        TH1F** UncertInputHisto;
        TH1F** UncertEstimHisto;
        TH1F** UncertDiffHisto;

};

#endif
