#ifndef SelectionTable_h
#define SelectionTable_h
#include "Selection.h"
#include "../../Tools/interface/Dataset.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include "TFile.h"

using namespace std;

class SelectionTable {

  
 public:
  
  SelectionTable();
  SelectionTable(vector<string> listOfCuts, vector<Dataset> listOfDatasets, string channel=string(""));
  SelectionTable(const SelectionTable& psel);
  ~SelectionTable();

  void SetLuminosity(float Lumi) {Lumi_ = Lumi;};

  float Luminosity() const {return Lumi_;};
  string Channel()const { return channelName_;};
  vector<string> ListOfCuts() const {return listOfCuts_;};
  vector<Dataset> ListOfDatasets() const {return listOfDatasets_;};

  float BinomialeError(float p, float n) const {return (sqrt((p*(1-p)/n)*p));};
  float ErrorCalculator(float number, float factor) const {return (sqrt(number)*factor);};
  float FactorCalculator(float nofevents, float xsection) const { return(Lumi_*xsection/nofevents);};
  void TableCalculator();
  void Scale(float Lumi);


  void Clear(){;};
  void DefineFirstDataset(string DatasetName);/** call after fill and before *Calculator methods */
  void MergeDatasets(vector<string> DatasetNames_ToMerge, string MergedName); /** merge several datasets in one called "MergedName" - This function has to be called before *Calculator methods */
  void Fill(unsigned int DatasetNumber, vector<float> PassTheCuts);
  void Fill(unsigned int DatasetNumber, unsigned int CutNumber, float value);
  void WriteTable(ofstream& fout, vector<vector<float> > listTable_, unsigned int istart, unsigned int istop);
  void WriteTable(ofstream& fout, vector<vector<float> > listTable_, vector<vector<float> > listTableError_, unsigned int istart, unsigned int istop);
  void Write(string filename, bool WithError = true, int maxColumns = -1);
  void Write(ofstream& fout, bool WithError = true, int maxColumns = -1);

 private:

  float Lumi_; // in 1/pb
  string channelName_;
  vector<string> listOfCuts_;
  vector<Dataset> listOfDatasets_;
  //first dimension: cuts
  //second dimension: datasets
  vector<vector<float> >  nofEventsRaw_;        //from Fill method
  vector<vector<float> >  nofEventsRawError_;   //from Fill method
  vector<vector<float> >  nofEvents_;   //rescaled
  vector<vector<float> >  nofEventsError_;
  vector<vector<float> >  cutEfficiency_;
  vector<vector<float> >  cutEfficiencyError_;
  vector<vector<float> >  totalCutEfficiency_;
  vector<vector<float> >  totalCutEfficiencyError_;

    
};

#endif
