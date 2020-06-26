#ifndef GeneralMMEstimation_h
#define GeneralMMEstimation_h

// system include files
#include <memory>
#include <vector>
#include <iostream>
#include <string>
#include "TH1F.h"
//#include "THStack.h"
#include "TF1.h"

#include "TRandom.h"
#include "TMatrixF.h"
#include "TCanvas.h"

#include "TLegend.h"

#include "TGraphErrors.h"


#include "../../Tools/interface/Dataset.h"
#include "../../Selection/interface/Selection.h"
#include "../../Selection/interface/SelectionTable.h"
#include "../../Selection/interface/DiLeptonSelection.h"
#include "../../../../MiniTreeFormat/NTFormat/interface/NTEvent.h"


using namespace std;
//using namespace TopTree;


struct Isolations{
  float iso1[4]; 
  float iso2[4];
};

struct NSelected{
  float NSel[4]; 
};

struct MMEstimated{
 float NofMMEstimatedTT[4];
 float NofMMEstimatedTF[4];
 float NofMMEstimatedFT[4];
 float NofMMEstimatedFF[4];
 float MMEstimatedTTErr[4];
 float MMEstimatedTFErr[4];
 float MMEstimatedFTErr[4];
 float MMEstimatedFFErr[4];
};

struct MMEstimatedPlots{
      TH1F * MMEstimated_TT[4];
      TH1F * MMEstimated_TF[4];
      TH1F * MMEstimated_FT[4];
      TH1F * MMEstimated_FF[4];
};

struct MMExpectedPlots{
      string Name[4];
      TH1F* MMExpected[4];
};

struct MMEpsilons{
  float EpsilonESignal;
  float EpsilonESignalErr;
  float EpsilonEFake;
  float EpsilonEFakeErr;
  float EpsilonMuSignal;
  float EpsilonMuSignalErr;
  float EpsilonMuFake;
  float EpsilonMuFakeErr;
};


struct Distribution{
   TH1F * NMMEstimatedTTDistribution[4];
   TH1F * NMMEstimatedTFDistribution[4];
   TH1F * NMMEstimatedFTDistribution[4];
   TH1F * NMMEstimatedFFDistribution[4];
};

class GeneralMMEstimation {


   public:
      GeneralMMEstimation(vector<Dataset>, float, float, float, unsigned int, float, float, string);
      ~GeneralMMEstimation();

      void CountNSel(const DiLeptonSelection&, Dataset, SelectionTable, unsigned int, float, string, int);
      void FillMMExpected(const DiLeptonSelection&, Dataset, SelectionTable, unsigned int, float, string, int, const NTEvent*);
      void RunTheMatrixMethod(vector<struct MMEpsilons>, unsigned int, bool, bool, bool);
      void ReadMMFile(string);
      void ReadMMFileForPullTest(string, float, float, float, float);
      vector<struct MMEstimated> GetMMEstimated();
      void PrintMMEstimated();
      struct MMEstimatedPlots GetMMEstimatedPlots();
      vector<struct MMExpectedPlots> GetMMExpectedPlots();
      void WriteMMFile(string);
      void WriteMMFileFast(string);
      void WriteMMFileFastForPullTest(string, string, float, float, float, float);

    private:

      struct Isolations GetIsolations();
      void IncrementNSelBin(unsigned int, unsigned int, float);
      void FillMMExpectedPlot(string, unsigned int, float, float);
      void IncludeStatisticalError();
      void IncludeSystematicError();
      void SolveTheSystem(bool);
      void FillDistributions(unsigned int);
      void CalculateRms(unsigned int, unsigned int);
      void SetMMEstimated(unsigned int, unsigned int);

      vector<struct Distribution> theDistributions;
      unsigned int theNBins; 
      float theMinBin;
      float theMaxBin;

      string IsoNames[4];
      string theChannel;
      struct Isolations theIsolations;

      TRandom ranEpsEFake, ranEpsESignal, ranEpsMuFake, ranEpsMuSignal, ranN1, ranN2, ranN3, ranN4;
      float locNSelected[4];
      float EpsilonEFake;
      float EpsilonEFakeErr;
      float EpsilonESignal;
      float EpsilonESignalErr;
      float EpsilonMuFake;
      float EpsilonMuFakeErr;
      float EpsilonMuSignal;
      float EpsilonMuSignalErr;

      vector<struct NSelected> theNSelected;

      float NMMEstimatedTT[4];
      float NMMEstimatedTF[4];
      float NMMEstimatedFT[4];
      float NMMEstimatedFF[4];

      TH1F* histoTT[4];
      TH1F* histoTF[4];
      TH1F* histoFT[4];
      TH1F* histoFF[4];

      TH1F* histoN[4];


      vector<struct MMEstimated> theMMEstimatedValues;
      struct MMEstimatedPlots theMMEstimatedPlots;
      vector<struct MMExpectedPlots> theMMExpectedPlots;
      //      struct MMStacks theMMStacks;

};

#endif
