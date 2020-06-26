#include "../interface/PLRPlots.h"

PLRPlots::PLRPlots(string Name, float ExpVal, int nbins, float xmin, float xmax, float Errmin, float Errmax, int NofUnc){
        name = Name;
        ExpectedValue = ExpVal;
        
        //allocation of the histo
        ParametersMeanValueStatHisto = new TH1F("ParametersMeanValueStatHisto","",nbins,xmin,xmax);       
        ParametersNegErrStatHisto = new TH1F("ParametersNegErrStatHisto","",nbins,Errmin,Errmax);       
        ParametersPosErrStatHisto = new TH1F("ParametersPosErrStatHisto","",nbins,Errmin,Errmax);       
        ParametersAvErrStatHisto = new TH1F("ParametersAvErrStatHisto","",nbins,Errmin,Errmax);       
        ParametersPullNegErrStatHisto = new TH1F("ParametersPullNegErrStatHisto","",nbins,-4,4);       
        ParametersPullPosErrStatHisto = new TH1F("ParametersPullPosErrStatHisto","",nbins,-4,4);       
        ParametersPullAvErrStatHisto = new TH1F("ParametersPullAvErrStatHisto","",nbins,-4,4);       
        ParametersMeanValueSystHisto = new TH1F("ParametersMeanValueSystHisto","",nbins,xmin,xmax);       
        ParametersNegErrSystHisto = new TH1F("ParametersNegErrSystHisto","",nbins,Errmin,Errmax);       
        ParametersPosErrSystHisto = new TH1F("ParametersPosErrSystHisto","",nbins,Errmin,Errmax);       
        ParametersAvErrSystHisto = new TH1F("ParametersAvErrSystHisto","",nbins,Errmin,Errmax);       
        ParametersPullNegErrSystHisto = new TH1F("ParametersPullNegErrSystHisto","",nbins,-4,4);       
        ParametersPullPosErrSystHisto = new TH1F("ParametersPullPosErrSystHisto","",nbins,-4,4);       
        ParametersPullAvErrSystHisto = new TH1F("ParametersPullAvErrSystHisto","",nbins,-4,4);       
        ParameterStatMeanMinusSystMeanHisto = new TH1F("ParameterStatMeanMinusSystMeanHisto","",nbins, -2,2);
        
        NofUncert = NofUnc;
        if(NofUncert>0){
                UncertInputHisto = new TH1F*[NofUncert];
                UncertEstimHisto = new TH1F*[NofUncert];
                UncertDiffHisto = new TH1F*[NofUncert];
                for(int i=0;i<NofUncert;i++){
                        char name[100]; 
                        sprintf(name,"Uncert_%d_input",i);
                        UncertInputHisto[i] = new TH1F(name,name,nbins,-4,4);
                        sprintf(name,"Uncert_%d_estim",i);
                        UncertEstimHisto[i] = new TH1F(name,name,nbins,-4,4);
                        sprintf(name,"Uncert_%d_diff",i);
                        UncertDiffHisto[i] = new TH1F(name,name,nbins,-4,4);
                }
        }       

        c = 0;
        leg = 0;
        histo = 0;
        CorrelationMatrixHisto = 0;
        graphsDeclared = false;

        StatGraph1D = 0;
        AllSystGraph1D = 0;
        graphs1D = 0;
        StatGraph2D = 0;
        AllSystGraph2D = 0;
        graphs2D = 0;
}

PLRPlots::~PLRPlots(){
        //to be written !!
}

void PLRPlots::Write(TFile* fout){
        fout->cd();
        string dirname = string("PseudoExp_")+name;
        fout->mkdir(dirname.c_str());
        fout->cd(dirname.c_str());
        ParametersMeanValueStatHisto->Write();       
        ParametersNegErrStatHisto->Write();       
        ParametersPosErrStatHisto->Write();       
        ParametersAvErrStatHisto->Write();       
        ParametersPullNegErrStatHisto->Write();       
        ParametersPullPosErrStatHisto->Write();       
        ParametersPullAvErrStatHisto->Write();       
        ParametersMeanValueSystHisto->Write();       
        ParametersNegErrSystHisto->Write();       
        ParametersPosErrSystHisto->Write();       
        ParametersAvErrSystHisto->Write();       
        ParametersPullNegErrSystHisto->Write();       
        ParametersPullPosErrSystHisto->Write();       
        ParametersPullAvErrSystHisto->Write();      
        ParameterStatMeanMinusSystMeanHisto->Write();
        for(int i=0;i<NofUncert;i++){
                UncertInputHisto[i]->Write(); 
                UncertEstimHisto[i]->Write();
                UncertDiffHisto[i]->Write();
        } 
        
}

/*      
void PLRPlots:SetAllSystPLRPoints1D(vector<double> Xvalues, vector<double> Yvalues){
        if(Xvalues.size() != Yvalues.size()){
                cerr<<"PLRPlots:SetAllSystPLRPoints1D: There is an inconsistency in the size of the collections .."<<endl;
        }
        int size = Xvalues.size();
        double* Xval = new double[size];
        double* Yval = new double[size];
        for(int i=0;i<size;i++){
                Xval[i] = Xvalues[i];
                Yval[i] = Yvalues[i];
        }
        AllSystGraph1D = new TGraph(size,Xval, Yval);
        
} 
*/ 
        
void PLRPlots::Fill(float Estim, float NegErr, float PosErr, bool stat){        
        if(stat){
        ParametersMeanValueStatHisto->Fill(Estim);       
        ParametersNegErrStatHisto->Fill(NegErr);       
        ParametersPosErrStatHisto->Fill(PosErr);       
        ParametersAvErrStatHisto->Fill((NegErr+PosErr)/2.);       
        ParametersPullNegErrStatHisto->Fill((Estim-ExpectedValue)/NegErr);       
        ParametersPullPosErrStatHisto->Fill((Estim-ExpectedValue)/PosErr);       
        ParametersPullAvErrStatHisto->Fill(2.*(Estim-ExpectedValue)/(NegErr+PosErr));  
        }
        else{     
        ParametersMeanValueSystHisto->Fill(Estim);       
        ParametersNegErrSystHisto->Fill(NegErr);       
        ParametersPosErrSystHisto->Fill(PosErr);       
        ParametersAvErrSystHisto->Fill((NegErr+PosErr)/2.);       
        ParametersPullNegErrSystHisto->Fill((Estim-ExpectedValue)/NegErr);       
        ParametersPullPosErrSystHisto->Fill((Estim-ExpectedValue)/PosErr);       
        ParametersPullAvErrSystHisto->Fill(2.*(Estim-ExpectedValue)/(NegErr+PosErr));    
        }   
}

void PLRPlots::Fill(float StatEstim, float SystEstim){
        ParameterStatMeanMinusSystMeanHisto->Fill(StatEstim-SystEstim);
}

/*      
void PLRPlots::FillUncertInput(int iuncert, float value){
        if(iuncert>=0 && iuncert<NofUncert)
                UncertInputHisto[iuncert]->Fill(value);
}
*/

void PLRPlots::FillUncert(int iuncert, float InputValue, float EstimValue){
        if(iuncert>=0 && iuncert<NofUncert){
                UncertEstimHisto[iuncert]->Fill(EstimValue);
                UncertInputHisto[iuncert]->Fill(InputValue);
                UncertDiffHisto[iuncert]->Fill(EstimValue-InputValue);
        }
}

