#ifndef DYEstimation_h
#define DYEstimation_h

//#include ".h"

// system include files
#include <memory>
#include <vector>
#include <iostream>
#include <string>
#include "TH1F.h"
#include "THStack.h"
#include "TF1.h"
#include "TFile.h"
#include "../../Tools/interface/FitFunctions.h"


using namespace std;
//using namespace TopTree;

/**
        Steps of the selection: (integer returned by doFullSelection() or FillTable(...))
        - Step 1        Dilepton pair choice
        - Step 2        Z mass veto 
        - Step 3        Minimal jet multiplicity 
        - Step 4        MET cuts
        - Step 5        btagging cuts

*/

class DYEstimation {


   public:
      DYEstimation();
      ~DYEstimation();

      void WriteHisto();
      void FillNbofEventsInSRandCR(bool, bool, float, string,float);
      std::vector<TH1F*> CreateDYHisto(string, string, int, float, float);
      void FillDYHistos(string, std::vector<TH1F*>, string, int, float, float);
      void MakeDYHistos(string, std::vector<TH1F*>, TH1F*);
      void MakeDataHistos(string, std::vector<TH1F*>, TH1F*);
      void ComputeDYEstimatedEvents_MC();
      float GetDYEvents_MC(); 
      float GetDYEstimatedEvents_MC(); 
      void MakeDYFits();
      vector<double> MakeDYFits_emu(TH1F*);
      void MakeDYFits_SR(TH1F*,int,float,float,vector<double>,double,double,string,float);
      void MakeDYFits_SR_asym(TH1F*,int,float,float,vector<double>,double,double,string,float);
      void MakeDYFits_CR(TH1F*,int,float,float,double,double,string);
      void MakeDYFits_CR_asym_plusCE(TH1F*,int,float,float,double,double,string);
      void MakeDYFits_CR_WithBkgSubtraction(TH1F*,int,float,float,vector<double>,double,double,string,float);
      void MakeDYFits_CR_WithBkgSubtraction_asym(TH1F*,int,float,float,vector<double>,double,double,string,float);
      void SetNFittedDYMCinMz_CR(double);
      float GetNDYMCinMz_CR();
      float GetNFittedDYMCinMz_CR();
      void SetNFittedDYMCinMz_SR(double);
      void SetNFittedDATAinMz_SR(double);
      void SetNFittedDATAinMz_CR(double);
      void SetNFittedDYMC_CR(double);
      void SetDYVerbose(bool);
      float GetNDYMCinMz_SR();
      float GetNFittedDYMCinMz_SR();
      float GetNDYMC_CR();
      float GetNFittedDYMC_CR();
      float GetNDYMC_SR();
      float GetNDATAinMz_SR();
      float GetNDATAinMz_CR();
      float GetNDATA_CR();
      float GetNDATA_BkgSubtracted_CR();
      float GetNDATA_SR();
      float GetNDYEstimatedMC();
      void ComputeDYEstimatedEvents_DATA(string,string);
      float GetNDYEstimatedDATA();
      float GetNDYEstimatedStatErrorDATA();
      float GetNDYEstimatedSysErrorDATA();
      float GetNDYEstimatedDATACounting();
      float GetNDYEstimatedStatErrorDATACounting();
      float GetNDYEstimatedSysErrorDATACounting();
      float GetNFittedDATA_CR();
      float GetNFittedDATAinMz_SR();
      float GetNFittedDATAinMz_CR();
      float GetContinuumInZdata();
      
      
      
    private:
      float f1_;
      float NDYMCinMz_SR;        // # of Zjets events with Mz_inf<Mll<Mz_sup in SR
      float NDYMCinMz_CR;        // # of Zjets events with Mz_inf<Mll<Mz_sup in CR
      float NDYMC_CR;            // # of Zjets events with Mll<Mz_inf(76GeV) || Mll>Mz_sup(106GeV) in CR
      float NDYMC_SR;            // # of Zjets events with Mll<Mz_inf(76GeV) || Mll>Mz_sup(106GeV) in SR
      float NFittedDYMCinMz_SR;        // # of fitted Zjets events with Mz_inf<Mll<Mz_sup in SR
      float NFittedDYMCinMz_CR;        // # of fitted Zjets events with Mz_inf<Mll<Mz_sup in CR
      float NFittedDYMC_CR;            // # of fitted Zjets events with Mll<Mz_inf(76GeV) || Mll>Mz_sup(106GeV) in CR
      float NunnormDYMCinMz_SR;        // Unnormalized # of Zjets events with Mz_inf<Mll<Mz_sup in SR
      float NunnormDYMCinMz_CR;        // Unnormalized # of Zjets events with Mz_inf<Mll<Mz_sup in CR
      float NunnormDYMC_CR;            // Unnormalized # of Zjets events with Mll<Mz_inf(76GeV) || Mll>Mz_sup(106GeV) in CR
      float NMCinMz_SR;        // # of events with Mz_inf<Mll<Mz_sup in SR
      float NMCinMz_CR;        // # of events with Mz_inf<Mll<Mz_sup in CR
      float NMC_CR;            // # of events with Mll<Mz_inf(76GeV) || Mll>Mz_sup(106GeV) in CR
      float NDYEstimatedMC;      // # of estimated MC Zjets events with Mll<Mz_inf(76GeV) || Mll>Mz_sup(106GeV)  
      float NDYMCinMzWithScaledPlusContinuum_SR;        // # of Zjets + fraction of continuum  events with Mz_inf<Mll<Mz_sup in SR 
      float NDYMCinMzWithScaledMinusContinuum_SR;        // # of Zjets - fraction of continuum  events with Mz_inf<Mll<Mz_sup in SR 

      float NDATAinMz_SR;        // # of DATA events with Mz_inf<Mll<Mz_sup in SR
      float NDATAinMz_CR;        // # of DATA events with Mz_inf<Mll<Mz_sup in CR
      float NDATA_CR;            // # of DATA events with Mll<Mz_inf || Mll>Mz_sup in CR
      float NDATA_SR;            // # of DATA events with Mll<Mz_inf || Mll>Mz_sup in SR
      float NFittedDATA_CR;      // # of fitted DATA events with Mll<Mz_inf || Mll>Mz_sup in CR
      float NFittedDATAinMz_SR;  // # of fitted DATA events with Mz_inf<Mll<Mz_sup in SR
      float NFittedDATAinMz_CR;  // # of fitted DATA events with Mz_inf<Mll<Mz_sup in CR
      float NDYEstimatedDATA;    // # of estimated DY events with Mll<Mz_inf || Mll>Mz_sup in SR
      float NDYEstimatedDATACounting;    // # of estimated DY events with Mz_inf<Mll<Mz_sup in SR with counting method
      float NDYEstimatedStatErrorDATA;    // Stat. error of the # of estimated DY events with Mll<Mz_inf || Mll>Mz_sup in SR
      float NDYEstimatedStatErrorDATACounting;    // Stat. error of the # of estimated DY events with Mll<Mz_inf || Mll>Mz_sup in SR with counting method
      float NDYEstimatedSysErrorDATA;     // Sys. error of the # of estimated DY events with Mll<Mz_inf || Mll>Mz_sup in SR
      float NDYEstimatedSysErrorDATACounting;     // Sys. error of the # of estimated DY events with Mll<Mz_inf || Mll>Mz_sup in      SR with counting method
      float NFittedDATAinMzWithScaledPlusContinuum_SR;  // # of fitted DATA + fraction of continuum  events with Mll<Mz_inf || Mll>Mz_sup in SR
      float NFittedDATAinMzWithScaledMinusContinuum_SR;  // # of fitted DATA - fraction of continuum  events with Mll<Mz_inf || Mll>Mz_sup in SR
      
      float ContinuumInZdata;
      
      bool DYVerbose;
      
      
      std::vector<TH1F*> hlistSR; 
      std::vector<TH1F*> hlistCR; 
      std::vector<TH1F*> hlistemu; 
      std::vector<TH1F*> hlistContCR; 
           
      // emu (for bkg)
      THStack *hstack1 ;
      
      // ll (for SR)
      THStack *hstackSR ;
      TH1F *allSR;

      // ll (for CR)
      THStack *hstackCR ;
      TH1F *allCR;

      // Continuum in CR (for bkg)
      THStack *hstackContCR ;
      TH1F *allContCR;

};


#endif
