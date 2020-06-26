#ifndef AnalysisEnvironmentLoader_h
#define AnalysisEnvironmentLoader_h

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "TObject.h"
#include "../../tinyxml/tinyxml.h"
//#include "../../Content/interface/AnalysisEnvironment.h"
#include "../../Selection/interface/Selection.h"
#include "../../Selection/interface/DiLeptonSelection.h"
#include "../../Selection/interface/SemiLeptonicTauSelection.h"
#include "../../Measurements/interface/PLRMeasurement.h"
#include "Dataset.h"

#include <string>

using namespace std;

struct Syst{
        bool doIt;
        float mean;
        float m1sigma;
        float p1sigma;
};

struct ExtendedSyst{
        bool doIt;
        string name;
        float mean;
        bool discretized; //false: +/- true: scan of values;
        //if !discretized
        float m1sigma;
        float p1sigma;
        //else
        vector<float> values;
};

struct SystEffect{
        bool doIt;
        float mean;
        float m1sigma;
        float p1sigma;
        float effmean;
        float effm1s;
        float effp1s;
};

struct XExtractionChannel{
        bool doIt;
        string channel;
        float BR;
        float NEvtsData;
        float NEvtsDY;
        float NEvtsDYErrPos;
        float NEvtsDYErrNeg;
        float NEvtsWjets;
        float NEvtsWjetsErrPos;
        float NEvtsWjetsErrNeg;
        float NEvtsQCD;
        float NEvtsQCDErrPos;
        float NEvtsQCDErrNeg;
};

void Print(const ExtendedSyst& syst);

struct Observable{
        string  name;
        int dim; //dim of the histo
        //size of vector = dim of histo
        vector<int> NofBins;
        vector<string> axisName;
        vector<pair<float,float> > axisRange;
};

void Print(const Observable& obs);

class AnalysisEnvironmentLoader
{
public:

  AnalysisEnvironmentLoader ();
  AnalysisEnvironmentLoader (string filename);
  ~AnalysisEnvironmentLoader ();

  void Reset();/** Pointers reset to NULL */
  bool LoadXMLFile(string filename);/** Load the xml file and return true if everythin went fine */
  TiXmlElement* NodeLoader(string nodeName);/** Look for a node called nodeName and return the first element in the node */

  void StringSeparator(string s, string sep, std::vector<float>& out);
  void StringSeparator(string s, string sep, std::vector<int>& out);
  void StringSeparator(string s, string sep, std::vector<string>& out);
  bool SearchStar(string s, vector<string>& out);

  void LoadSamples(vector<Dataset>& datasets);/** Read the xml file and fill a vector of datasets */
  void LoadSamples(vector<Dataset>& datasets, string name);/** Read the xml file and fill a vector of datasets */
  void LoadSelection(Selection& sel); /** Read the xml file and update the selection parameters */
  void LoadDiLeptonSelection(DiLeptonSelection& sel); /** Read the xml file and update the DiLeptonSelection parameters */
  void LoadSemiLeptonicTauSelection(SemiLeptonicTauSelection& sel); /** Read the xml file and update the SemiLeptonicTauSelection parameters */
  void LoadGeneralInfo(int& DataType, float& Luminosity, int& verbosity );/** Read general information from the xml file - DataType = 0 for MC, 1 for Data, 2 for MC&Data */
  void LoadGeneralInfo(int& DataType, float& Luminosity, float& LumiError, int& verbosity );/** Read general information from the xml file - DataType = 0 for MC, 1 for Data, 2 for MC&Data */
//<<<<<<< AnalysisEnvironmentLoader.h
  void LoadGeneralInfo(int& DataType, float& Luminosity, float& LumiError, int& verbosity, int& btagAlgo_, float& btagDiscriCut_ );
 void LoadGeneralInfo(int& DataType, float& Luminosity, float& LumiError,string& PUWeightFile, int& verbosity );


 

  void LoadSystematics(string name, float& mean, float& m1sigma, float& p1sigma); /** Read the xml file and update the 3 parameters for a given systematic: mean & +/- 1 sigma */
  void LoadSystematics(string name, Syst&  SysValues); /** Read the xml file and update the 3 parameters for a given systematic: mean & +/- 1 sigma */
  void LoadSystematics(string name, ExtendedSyst&  SysValues); /** Read the xml file and update the 3 parameters for a given systematic: mean & +/- 1 sigma */
  void LoadSystematicsEffect(string name, string channel, SystEffect& SysValues); /** Read the xml file and update the 4 parameters for a given systematic: +/- 1 sigma  eff +/- 1 sigma */
  void LoadObservables(vector<Observable>& obs);
  void LoadWeight(DiLeptonSelection& sel); /** Read the xml file and initialize the SFBweight class for b-tag */
  void LoadWeight(SemiLeptonicTauSelection& sel); /** Read the xml file and initialize the SFBweight class for b-tag */
  void LoadXExtraction(string channel, XExtractionChannel& xec);
  void LoadBgkInformation(PLRMeasurement& plr);
  void LoadPLRInformation(PLRMeasurement& plr, int& doPseudoExp, int& NofPseudoExp);
  void LoadSystematicsEffect(PLRMeasurement& plr);
 
  /**
   *   Read an analysis specific-specific parameter in the Analysis block
   *   \param typeName The name of the type to look for
   *   \param varName  The name of the parameter to look for
   *   \param value    The variable into which the parameter will be written to
   */
  void LoadAnalysisInfo(string typeName, string varName, bool& value);

  /**
   *   Read an analysis specific-specific parameter in the Analysis block
   *   \param typeName The name of the type to look for
   *   \param varName  The name of the parameter to look for
   *   \param value    The variable into which the parameter will be written to
   */
  void LoadAnalysisInfo(string typeName, string varName, int& value);

  /**
   *   Read an analysis specific-specific parameter in the Analysis block
   *   \param typeName The name of the type to look for
   *   \param varName  The name of the parameter to look for
   *   \param value    The variable into which the parameter will be written to
   */
  void LoadAnalysisInfo(string typeName, string varName, float& value);

  /**
   *   Read an analysis specific-specific parameter in the Analysis block
   *   \param typeName The name of the type to look for
   *   \param varName  The name of the parameter to look for
   *   \param value    The variable into which the parameter will be written to
   */
  void LoadAnalysisInfo(string typeName, string varName, string& value);



private:
  TiXmlDocument* doc;
  TiXmlHandle* hdl;
  TiXmlNode *node; 
  TiXmlElement *elem;
  
};

#endif
