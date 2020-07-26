#ifndef PUWeighting_h
#define PUWeighting_h

//#include ".h"

// system include files
#include <memory>
#include <vector>
#include <iostream>
#include <string>
#include "TH1F.h"
// #include "TH2F.h"
// #include "TH3F.h"
// #include "THStack.h"
// #include "TF1.h"
// #include "TCanvas.h"
// #include "TStyle.h"
// #include "../../../../MiniTreeFormat/NTFormat/interface/NTEvent.h"
//#include "RooGlobalFunc.h"


 
using namespace std;
//using namespace RooFit;
// using namespace TopTree;


class PUWeighting {


   public:
      PUWeighting();
      PUWeighting( TH1D * thehistData,  TH1F * thehistMC, const bool useoot);
      ~PUWeighting();

      
     void setPUHisto(TH1D * thehistData, TH1F * thehistMC);
     void setPUHisto(TH1D * thehistData);
     void setPUHisto(const string filename);
     void setUseOutOfTimePU(bool useoot);
     bool getUseOutOfTimePU();
     void weightOOT_init();
     double weight(int npu);
     double weight_Spring11(int npu);
     double weight(int npu, int npuoot);
     double weight_Summer11ITP(int npu);
     vector<double> generate_flat10_weights(const TH1D* data_npu_estimated);
     vector<double> reweight2011_inputOnly(const TH1D* data_npu_estimated);
     
    private:
     
        bool useOOT;
        TH1F * puHisto_MC;
        TH1D * puHisto_Data;
        TH1F * weights_;
        double WeightOOTPU_[25][25];
        vector<double> weightvectorSummer11ITP, weightvector_flat10, Correct_Weights2011;



};

#endif

