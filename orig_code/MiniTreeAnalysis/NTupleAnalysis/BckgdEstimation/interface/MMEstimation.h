#ifndef MMEstimation_h
#define MMEstimation_h

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
  float iso1[3]; 
  float iso2[3];
};

struct NSelected{
  float NSel[3]; 
};

struct MMEstimated{
 float NofMMEstimatedQCD[3];
 float NofMMEstimatedWJets[3];
 float NofMMEstimatedSignal[3];
 float MMEstimatedQCDErr[3];
 float MMEstimatedWJetsErr[3];
 float MMEstimatedSignalErr[3];
};

struct MMEstimatedPlots{
      TH1F * MMEstimated_Signal[3];
      TH1F * MMEstimated_QCD[3];
      TH1F * MMEstimated_WJets[3];
};

struct MMExpectedPlots{
      string Name[3];
      TH1F* MMExpected[3];
};

/*
struct MMStacks{
  THStack* stackSignal[3];
  THStack* stackWJets[3];
  THStack* stackQCD[3];
};
*/

struct MMEpsilons{
  float EpsilonSignal;
  float EpsilonSignalErr;
  float EpsilonFake;
  float EpsilonFakeErr;
};


struct Distribution{
   TH1F * NMMEstimatedQCDDistribution[3];
   TH1F * NMMEstimatedWJetsDistribution[3];
   TH1F * NMMEstimatedSignalDistribution[3];
};

class MMEstimation {


   public:
      MMEstimation(vector<Dataset>, float, float, unsigned int, float, float, string);
      ~MMEstimation();

      void CountNSel(const DiLeptonSelection&, Dataset, SelectionTable, unsigned int, float, string, int);
      void FillMMExpected(const DiLeptonSelection&, Dataset, SelectionTable, unsigned int, float, string, int, const NTEvent*);
      void RunTheMatrixMethod(vector<struct MMEpsilons>, unsigned int, bool, bool, bool);
      void RunTheMatrixMethodForEpsilonsTest(vector<struct MMEpsilons>, unsigned int, bool, bool, float);
      void ReadMMFile(string);
      void ReadMMFileForPullTest(string, float, float);
      vector<struct MMEstimated> GetMMEstimated();
      void PrintMMEstimated();
      struct MMEstimatedPlots GetMMEstimatedPlots();
      vector<struct MMExpectedPlots> GetMMExpectedPlots();
      void WriteMMFile(string);
      void WriteMMFileFast(string);
      void WriteMMFileFastForPullTest(string, string, float, float);

    private:

      struct Isolations GetIsolations();
      void IncrementNSelBin(unsigned int, unsigned int, float);
      void FillMMExpectedPlot(string, unsigned int, float, float);
      void IncludeStatisticalError();
      void IncludeStatisticalErrorForEpsilonsTest(float);
      void IncludeSystematicError();
      void SolveTheSystem(bool);
      void FillDistributions(unsigned int);
      void CalculateRms(unsigned int, unsigned int);
      void SetMMEstimated(unsigned int, unsigned int);

      vector<struct Distribution> theDistributions;
      unsigned int theNBins; 
      float theMinBin;
      float theMaxBin;

      string IsoNames[3];
      string theChannel;
      struct Isolations theIsolations;

      TRandom ranEpsFake, ranEpsSignal, ranN1, ranN2, ranN3;
      float locNSelected[3];

      float locNSelectedSignal[3];
      float locNSelectedW[3];
      float locNSelectedQCD[3];


      float EpsilonFake;
      float EpsilonFakeErr;
      float EpsilonSignal;
      float EpsilonSignalErr;
      vector<struct NSelected> theNSelected;
      float NMMEstimatedSignal[3];
      float NMMEstimatedWJets[3];
      float NMMEstimatedQCD[3];


      TH1F* histoSignal[3];
      TH1F* histoW[3];
      TH1F* histoQCD[3];

      TH1F* histoN[3];


      vector<struct MMEstimated> theMMEstimatedValues;
      struct MMEstimatedPlots theMMEstimatedPlots;
      vector<struct MMExpectedPlots> theMMExpectedPlots;
      //      struct MMStacks theMMStacks;

};

#endif
