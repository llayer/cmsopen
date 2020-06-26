#include "../interface/AnalysisEnvironmentLoader.h"


void Print(const ExtendedSyst& sys){
        cout<<" * "<<sys.name<<" mean: "<<sys.mean<<" m1s: "<<sys.m1sigma<<" p1s: "<<sys.p1sigma<<" discretized: ";
        if(sys.discretized) cout<<"True ";
        else cout<<"False ";
        cout<<" values:(";
        for(unsigned int i=0;i<sys.values.size();i++){
                cout<<sys.values[i];
                if(i<sys.values.size()-1) cout<<",)";
        }
        cout<<")"<<endl;
}

void Print(const Observable& obs){
        cout<<" * Obs: "<<obs.name<<" dim: "<<obs.dim;
        if((int)obs.NofBins.size()!=obs.dim || (int) obs.axisName.size()!=obs.dim || (int)obs.axisRange.size()!=obs.dim){
                cout<<" ERROR of size !!"<<endl;
        }
        cout<<" NofBins: "; 
        for(unsigned int i=0;i<obs.axisName.size();i++){
                cout<<obs.axisName[i];
                if(i<(obs.axisName.size()-1))cout<<","; 
        }
        cout<<" -  axisName: ";
        for(unsigned int i=0;i<obs.axisName.size();i++){
                cout<<obs.axisName[i];
                if(i<(obs.axisName.size()-1))cout<<","; 
        }
        cout<<" -  axisRange: ";
        for(unsigned int i=0;i<obs.axisRange.size();i++){
                cout<<obs.axisRange[i].first<<","<<obs.axisRange[i].second;
                if(i<(obs.axisName.size()-1))cout<<"|"; 
        }
        cout<<endl;
}

AnalysisEnvironmentLoader::AnalysisEnvironmentLoader (){
  doc = 0;
  hdl = 0;
  node = 0;
  elem = 0;
}

AnalysisEnvironmentLoader::~AnalysisEnvironmentLoader (){
  node = 0;
  delete node;
  elem = 0;
  delete elem;
  // in this order
  delete hdl;
  delete doc;
}

void AnalysisEnvironmentLoader::Reset(){
  node = 0;
  elem = 0;
}

AnalysisEnvironmentLoader::AnalysisEnvironmentLoader (string filename){
        LoadXMLFile(filename);
}

bool AnalysisEnvironmentLoader::LoadXMLFile(string filename)
{
  doc = new TiXmlDocument(filename.c_str());
  if (!doc->LoadFile ())
    {
      cerr << "Error while loading the xml file in AnalysisEnvionment::Load(" << filename << ") " << endl;
      cerr << " error #" << doc->ErrorId () << " : " << doc->ErrorDesc () << endl;
      return false;
    }
  hdl = new TiXmlHandle (&(*doc));
  return true;
}


TiXmlElement* AnalysisEnvironmentLoader::NodeLoader(string nodeName){
  Reset();
  bool nodeFound = false;
  node = hdl->Node ();
  for( node = node->FirstChild(); node; node = node->NextSibling() )
    {
      if (node->Value () == string (nodeName))
        {
          nodeFound = true;
          break;
        }
    }

  if (nodeFound)
    elem = node->FirstChildElement ();
  else
    {
      cerr << "The node " <<nodeName<<" doesn't exist" << endl;
    }
  if (!elem)
    {
      cerr << "No elements in The node "<<nodeName<< endl;
    }
   return elem;

}

void AnalysisEnvironmentLoader::LoadGeneralInfo(int& DataType, float& Luminosity, int& verbosity ){
  Reset();
  elem = NodeLoader(string("Analysis"));
  if(!elem) return;
  while (elem)
    {
      string type = elem->Attribute ("type");
      if (type == string ("General"))
        {
        elem->QueryIntAttribute ("Verbosity", &verbosity);
        elem->QueryIntAttribute ("DataType", &DataType);
        elem->QueryFloatAttribute ("Luminosity", &Luminosity);
        }
        elem = elem->NextSiblingElement ();     // iteration
    }
}

void AnalysisEnvironmentLoader::LoadGeneralInfo(int& DataType, float& Luminosity, float& LumiError, int& verbosity ){
  Reset();
  elem = NodeLoader(string("Analysis"));
  if(!elem) return;
  while (elem)
    {
      string type = elem->Attribute ("type");
      if (type == string ("General"))
        {
        elem->QueryIntAttribute ("Verbosity", &verbosity);
        elem->QueryIntAttribute ("DataType", &DataType);
        elem->QueryFloatAttribute ("Luminosity", &Luminosity);
        elem->QueryFloatAttribute ("LumiError", &LumiError);
        }
        elem = elem->NextSiblingElement ();     // iteration
    }
}
void AnalysisEnvironmentLoader::LoadGeneralInfo(int& DataType, float& Luminosity, float& LumiError, int& verbosity, int& btagAlgo_, float& btagDiscriCut_ ){
  Reset();
  elem = NodeLoader(string("Analysis"));
  if(!elem) return;
  while (elem)
    {
      string type = elem->Attribute ("type");
      if (type == string ("General"))
        {
        elem->QueryIntAttribute ("Verbosity", &verbosity);
        elem->QueryIntAttribute ("DataType", &DataType);
        elem->QueryIntAttribute ("btagAlgo_", &btagAlgo_);
        elem->QueryFloatAttribute ("Luminosity", &Luminosity);
        elem->QueryFloatAttribute ("LumiError", &LumiError);
        elem->QueryFloatAttribute ("btagDiscriCut_", &btagDiscriCut_);

        }
        elem = elem->NextSiblingElement ();     // iteration
    }
}

void AnalysisEnvironmentLoader::LoadGeneralInfo(int& DataType, float& Luminosity, float& LumiError,  string& PUWeightFile, int& verbosity ){
  Reset();
  
  elem = NodeLoader(string("Analysis"));
  if(!elem) return;
  while (elem)
    {
      string type = elem->Attribute ("type");
      
      if (type == string ("General"))
        {
          elem->QueryIntAttribute ("Verbosity", &verbosity);
          
          elem->QueryIntAttribute ("DataType", &DataType);
         
          elem->QueryFloatAttribute ("Luminosity", &Luminosity);
          
          elem->QueryFloatAttribute ("LumiError", &LumiError);
          
          PUWeightFile = elem->Attribute("PUWeightFile");
          
        }
      
      elem = elem->NextSiblingElement ();       // iteration
     
    }
 
}


void AnalysisEnvironmentLoader::LoadObservables(vector<Observable>& vobs){
  Reset();
  elem = NodeLoader(string("Observables"));
  if(!elem) return;
  while (elem)
    {
      string type = elem->Attribute ("obs");
      Observable obs;
        obs.name = type;
        elem->QueryIntAttribute ("dim", &obs.dim);
        string sNofBins;
        vector<int> NofBins;
        sNofBins = elem->Attribute ("NofBins");
        StringSeparator(sNofBins,string(","),NofBins);
        obs.NofBins = NofBins;
        string saxisName;
        vector<string> axisName;
        saxisName = elem->Attribute ("axisName");
        StringSeparator(saxisName,string(","),axisName);
        obs.axisName = axisName;
        string saxisRange;
        vector<string> vsaxisRange;
        vector<pair<float,float> > axisRange;
        saxisRange = elem->Attribute("axisRange");
        StringSeparator(saxisRange,string("|"),vsaxisRange);
        for(unsigned int i=0;i<vsaxisRange.size();i++){
                vector<float> range;
                StringSeparator(vsaxisRange[i],string(","),range);
                if(range.size()!=2) cerr<<"AnalysisEnvironmentLoader::LoadObservables:: range should be min,max "<<endl;
                else axisRange.push_back(pair<float,float>(range[0],range[1]));
        }
        obs.axisRange = axisRange;
        if((int)NofBins.size()!=obs.dim || (int) axisName.size()!=obs.dim || (int)axisRange.size()!=obs.dim)
                cerr<<"AnalysisEnvironmentLoader::LoadObservables::You should verify your config file: problem of dimension !!"<<endl;
        vobs.push_back(obs);
        elem = elem->NextSiblingElement ();     // iteration
    }
    

} 

void AnalysisEnvironmentLoader::LoadSystematics(string name, float& mean, float& m1sigma, float& p1sigma){
  Reset();
  elem = NodeLoader(string("Systematics"));
  if(!elem) return;
  while (elem)
    {
      string type = elem->Attribute ("type");
      if (type == name)
        {
        elem->QueryFloatAttribute ("mean", &mean);
        elem->QueryFloatAttribute ("m1sigma", &m1sigma);
        elem->QueryFloatAttribute ("p1sigma", &p1sigma);
        }
        elem = elem->NextSiblingElement ();     // iteration
    }
}

void AnalysisEnvironmentLoader::LoadSystematics(string name, Syst& SysValues){
  Reset();
  elem = NodeLoader(string("Systematics"));
  if(!elem) return;
  while (elem)
    {
      string type = elem->Attribute ("type");
      if (type == name)
        {
        int doIt = 0;
        elem->QueryIntAttribute ("doIt", &doIt);
        if(doIt==1) SysValues.doIt = true;
        else SysValues.doIt = false;
        elem->QueryFloatAttribute ("mean", &SysValues.mean);
        elem->QueryFloatAttribute ("m1sigma", &SysValues.m1sigma);
        elem->QueryFloatAttribute ("p1sigma", &SysValues.p1sigma);
        }
        elem = elem->NextSiblingElement ();     // iteration
    }
}

void AnalysisEnvironmentLoader::LoadSystematics(string name, ExtendedSyst& SysValues){
  Reset();
  elem = NodeLoader(string("Systematics"));
  if(!elem) return;
  while (elem)
    {
      string type = elem->Attribute ("type");
      if (type == name)
        {
        SysValues.name = type;
        int doIt = 0;
        elem->QueryIntAttribute ("doIt", &doIt);
        if(doIt==1) SysValues.doIt = true;
        else SysValues.doIt = false;
        //
        int discretized = 0;
        elem->QueryIntAttribute ("discretized", &discretized);
        if(discretized==1) SysValues.discretized = true;
        else SysValues.discretized = false;
        //
        elem->QueryFloatAttribute ("mean", &SysValues.mean);
        elem->QueryFloatAttribute ("m1sigma", &SysValues.m1sigma);
        elem->QueryFloatAttribute ("p1sigma", &SysValues.p1sigma);
        string svalues = elem->Attribute("values");
        vector<float> values;
        StringSeparator(svalues,string(","),values);
        SysValues.values = values;
        }
        elem = elem->NextSiblingElement ();     // iteration
    }
}

void AnalysisEnvironmentLoader::LoadSystematicsEffect(string name, string channel, SystEffect& SysValues){
  Reset();
  elem = NodeLoader(string("SystematicsEffect"));
  if(!elem) return;
  while (elem)
    {
      string ch = elem->Attribute ("channel");
      string type = elem->Attribute ("type");
      if (type == name && channel == ch)
        {
        int doIt = 0;
        elem->QueryIntAttribute ("doIt", &doIt);
        if(doIt==1) SysValues.doIt = true;
        else SysValues.doIt = false;
        elem->QueryFloatAttribute ("mean", &SysValues.mean);
        elem->QueryFloatAttribute ("m1s", &SysValues.m1sigma);
        elem->QueryFloatAttribute ("p1s", &SysValues.p1sigma);
        elem->QueryFloatAttribute ("effmean", &SysValues.effmean);
        elem->QueryFloatAttribute ("effm1s", &SysValues.effm1s);
        elem->QueryFloatAttribute ("effp1s", &SysValues.effp1s);
        }
        elem = elem->NextSiblingElement ();     // iteration
    }
}

void AnalysisEnvironmentLoader::LoadXExtraction(string channel, XExtractionChannel& xec){
  Reset();
  elem = NodeLoader(string("XExtraction"));
  if(!elem) return;
  while (elem)
    {
        string ch =  elem->Attribute ("channel");
        if(ch == channel){
                xec.channel = ch;
                int doIt = 0;
                 elem->QueryIntAttribute ("doIt", &doIt);
                if(doIt==1) xec.doIt = true;
                else xec.doIt = false;
                elem->QueryFloatAttribute ("BR", &xec.BR);
                elem->QueryFloatAttribute ("NEvtsData", &xec.NEvtsData);
                elem->QueryFloatAttribute ("NEvtsDY", &xec.NEvtsDY);
                elem->QueryFloatAttribute ("NEvtsDYErrPos", &xec.NEvtsDYErrPos);
                elem->QueryFloatAttribute ("NEvtsDYErrNeg", &xec.NEvtsDYErrNeg);
                elem->QueryFloatAttribute ("NEvtsWjets", &xec.NEvtsWjets);
                elem->QueryFloatAttribute ("NEvtsWjetsErrPos", &xec.NEvtsWjetsErrPos);
                elem->QueryFloatAttribute ("NEvtsWjetsErrNeg", &xec.NEvtsWjetsErrNeg);
                elem->QueryFloatAttribute ("NEvtsQCD", &xec.NEvtsQCD);
                elem->QueryFloatAttribute ("NEvtsQCDErrPos", &xec.NEvtsQCDErrPos);
                elem->QueryFloatAttribute ("NEvtsQCDErrNeg", &xec.NEvtsQCDErrNeg);
        }
        elem = elem->NextSiblingElement ();     // iteration
    }
}

void AnalysisEnvironmentLoader::LoadBgkInformation(PLRMeasurement& plr){
  Reset();
  elem = NodeLoader(string("EvtNumbers"));
  if(!elem) return;
  while (elem)
    {
        cout<<"THERE"<<endl;
        if( elem->Value() == string("Channel")){
                string channel = elem->Attribute("channel");
                TiXmlElement *ChannelElem = elem->FirstChildElement();
                while(ChannelElem){
                        float number = 0;
                        float errm = 0;
                        float errp = 0;
                        bool isASyst = 0;
                        int isASyst_i = 0;
                        bool isDD = 0;
                        int isDD_i = 0;
                        int doIt = 0;
                        string name = ChannelElem->Attribute("name");
                        ChannelElem->QueryIntAttribute ("doIt", &doIt);
                        ChannelElem->QueryFloatAttribute("number", &number);    
                        ChannelElem->QueryFloatAttribute("errm", &errm);        
                        ChannelElem->QueryFloatAttribute("errp", &errp);
                        ChannelElem->QueryIntAttribute("isASyst", &isASyst_i);
                        ChannelElem->QueryIntAttribute("isDD", &isDD_i);
                        if(isASyst_i != 0) isASyst = true;
                        if(isDD_i != 0) isDD = true;
                
                        for(unsigned int i=0;i<plr.GetChannelsAddress().size();i++){    
                                if( plr.GetChannelsAddress()[i].GetChannelName() == channel){
                                        DDEstimation dd;
                                        dd.name = name;
                                        dd.isDDEstimated = isDD;
                                        dd.isASyst = isASyst;
                                        dd.NofEvts = number;
                                        dd.ErrNeg = errm;
                                        dd.ErrPos = errp;
                                        plr.GetChannelsAddress()[i].AddBkgEstimation(dd);
                                        cout<<"Ca ajoute ! "<<isDD<<endl;
                                }
                        }
        
                        ChannelElem = ChannelElem->NextSiblingElement ();       // iteration 
                }
        }
        elem = elem->NextSiblingElement ();     // iteration 
    }
        
}

void AnalysisEnvironmentLoader::LoadSystematicsEffect(PLRMeasurement& plr){
  Reset();
  elem = NodeLoader(string("SystematicsEffect"));
  if(!elem) return;
  while (elem)
    {
        if( elem->Value() == string("Syst")){
                int doIt = 0;
                elem->QueryIntAttribute("doIt", &doIt);
                string name = elem->Attribute("name");
                if(doIt>0){
                        vector<Systematic> ChannelsSyst;
                        TiXmlElement *SystElem = elem->FirstChildElement();
                        while(SystElem){
                                Systematic mysyst;
                                mysyst.name = name;
                                string channel = SystElem->Attribute("channel");
                                mysyst.channel = channel;
                                float effm1s = 0;
                                float effp1s = 0;
                                SystElem->QueryFloatAttribute ("effm1s", &effm1s);
                                SystElem->QueryFloatAttribute ("effp1s", &effp1s);
                                mysyst.effm1s = effm1s;
                                mysyst.effp1s = effp1s;
                                ChannelsSyst.push_back(mysyst);
                                SystElem = SystElem->NextSiblingElement ();     // iteration 
                        }
                        delete SystElem;
                        plr.AddSignalSystematic(ChannelsSyst);
                }
        }
        elem = elem->NextSiblingElement ();     // iteration 
    }
        
}

void AnalysisEnvironmentLoader::LoadPLRInformation(PLRMeasurement& plr, int& doPseudoExp, int& NofPseudoExp){
  Reset();
  elem = NodeLoader(string("PLRMeasurement"));
  if(!elem) return;
  string SignalDSName;
  bool XsectionIsAParam = false;
  while (elem)
    {
        if( elem->Value() == string("Channels")){
                TiXmlElement *ChannelElem = elem->FirstChildElement();
                while(ChannelElem){
                        int doIt = 0;
                        ChannelElem->QueryIntAttribute ("doIt", &doIt);

                        float BR = 1.; 
                        string name = ChannelElem->Attribute("name");   
                        string SignalDatasetName = ChannelElem->Attribute("SignalDatasetName");
                        if(doIt>0) plr.AddChannel(name, SignalDatasetName, BR);
                        //to be changed .. do it for one at least
                        SignalDSName = SignalDatasetName;
                        ChannelElem = ChannelElem->NextSiblingElement ();       // iteration 
                }
                delete ChannelElem;
        }
        if( elem->Value() == string("Parameters")){
                TiXmlElement *ParamElem = elem->FirstChildElement();
                while(ParamElem){
                        int doIt = 0;
                        ParamElem->QueryIntAttribute ("doIt", &doIt);

                        float ExpectedValue = 0;
                        float xmin = 0;
                        float xmax = 0;
                        float xErrmin = 0;
                        float xErrmax = 0;
                        int nbins = 0;
                        float step = 0;
                        int npoints = 0;
                        string name = ParamElem->Attribute("name");
                        ParamElem->QueryFloatAttribute("Expectation",&ExpectedValue);
                        ParamElem->QueryFloatAttribute("xmin",&xmin);
                        ParamElem->QueryFloatAttribute("xmax",&xmax);
                        ParamElem->QueryFloatAttribute("xErrmin",&xErrmin);
                        ParamElem->QueryFloatAttribute("xErrmax",&xErrmax);
                        ParamElem->QueryIntAttribute("nbins",&nbins);
                        ParamElem->QueryFloatAttribute("step",&step);
                        ParamElem->QueryIntAttribute("npoints",&npoints);
                        string option = ParamElem->Attribute("option");
                        bool WithGraph = false;
                        if(option == string("graph")) WithGraph = true;
                                if(doIt>0) plr.AddFreeParameter(name, ExpectedValue, npoints, nbins, xmin, xmax, xErrmin, xErrmax, step, WithGraph);
                        if(doIt>0 && name == string("SignalXsection") ) XsectionIsAParam = true;
                        ParamElem = ParamElem->NextSiblingElement ();   // iteration 
                }
                delete ParamElem;
        }
        if( elem->Value() == string("NuisanceParameters")){
                TiXmlElement *NParamElem = elem->FirstChildElement();
                while(NParamElem){
                        int doIt = 0;
                        NParamElem->QueryIntAttribute ("doIt", &doIt);

                        string name = NParamElem->Attribute("name");
                        string graph = NParamElem->Attribute("option");
                        bool WithGraph = false;
                        if(graph==string("graph")) WithGraph = true;
                        float ExpectedValue = -999;
                        float ErrNeg = -999;
                        float ErrPos = -999;
                        NParamElem->QueryFloatAttribute("Expectation",&ExpectedValue);
                        NParamElem->QueryFloatAttribute("ErrNeg",&ErrNeg);
                        NParamElem->QueryFloatAttribute("ErrPos",&ErrPos);
                        if(doIt>0){ 
                                //if(name == string("SignalXsection")) plr.AddNuisanceParameter(name, false, WithGraph, ExpectedValue, ErrNeg, ErrPos);
                                //else plr.AddNuisanceParameter(name, false, WithGraph);
                                if(ExpectedValue>-999 && ErrNeg>-999 && ErrPos>-999) plr.AddNuisanceParameter(name, false, WithGraph, ExpectedValue, ErrNeg, ErrPos);
                                else plr.AddNuisanceParameter(name, false, WithGraph);
                        }
                        NParamElem = NParamElem->NextSiblingElement (); // iteration 
                }
                delete NParamElem;
                cout<<"end"<<endl;
        }
        if( elem->Value() == string("Histos")){
                TiXmlElement *HistoElem = elem->FirstChildElement();
                while(HistoElem){
                        int doIt = 0;
                        HistoElem->QueryIntAttribute ("doIt", &doIt);
                        if(doIt!=1) continue;
                        
                        int dim = 0;
                        string name = HistoElem->Attribute("name");
                        HistoElem->QueryIntAttribute ("dim", & dim);
                        plr.AddHisto(name, dim);
                        HistoElem = HistoElem->NextSiblingElement ();   // iteration 
                }
                delete HistoElem;
        }
        if( elem->Value() == string("General")){
                int isLumiASyst = 0;
                doPseudoExp = 0;
                NofPseudoExp = 0;
                elem->QueryIntAttribute("isLumiASyst",&isLumiASyst);
                elem->QueryIntAttribute("doPseudoExp",&doPseudoExp);
                elem->QueryIntAttribute("NofPseudoExp",&NofPseudoExp);
                string method = string("linear");
                method = elem->Attribute("InterpolMethod");
                string filename = elem->Attribute("rootFileName");
                plr.SetIsLumiASyst((bool) isLumiASyst);
                plr.SetHistoRootFileName(filename);
                plr.SetInterpolMethod(method);
                
        }
        elem = elem->NextSiblingElement ();     // iteration 
    }
    plr.SetSignalInfo(SignalDSName,XsectionIsAParam);   
        
}


void AnalysisEnvironmentLoader::LoadSamples(vector<Dataset>& datasets){
  Reset();
  elem = NodeLoader(string("Datasets"));
  if(!elem) return;
  while (elem)
    {
        string name = elem->Attribute("name");
        string filename = elem->Attribute("filenames");
        cout<<"Load for dataset "<<name<<" the files "<<filename<<endl;
        int isData = 0;
        int add = 0;
        int ls = 1;
        int lw = 1;
        int color = 1;
        float normf = 1.;
        float xsection = 1.;
        float xsErrorMinus = 0.;
        float xsErrorPlus = 0.;
        elem->QueryIntAttribute ("ls", &ls);
        elem->QueryIntAttribute ("lw", &lw);
        elem->QueryIntAttribute ("color", &color);
        elem->QueryFloatAttribute ("normf", &normf);
        elem->QueryFloatAttribute ("xsection", &xsection);
        elem->QueryIntAttribute ("add", &add);
        elem->QueryIntAttribute ("isData", &isData);
        elem->QueryFloatAttribute ("xsErrorMinus", &xsErrorMinus);
        elem->QueryFloatAttribute ("xsErrorPlus", &xsErrorPlus);
        vector<string> filenames;
        bool star = SearchStar(filename,filenames);
        if(!star){
                filenames.clear();
                StringSeparator(filename,string(","),filenames);
        }

        //create a Dataset and push it back in the vector
        if(filenames.size()==0 || filenames[0] == string("")){
                Dataset d;
                d.SetName(name);
                d.SetCrossSection(xsection);
                d.SetIsData(isData);
                if(add) datasets.push_back(d);
        }
        else{
                cout<<"THere "<<name<<" "<<isData<<endl;
                Dataset d(name, (bool) isData, true, color, ls, lw, normf, xsection, filenames);
                cout<<d.isData()<<endl;
                d.SetIsData(isData);
                cout<<d.isData()<<endl;
                d.SetCrossSectionError(xsErrorMinus,xsErrorPlus);
                //Norm with Eff and NofEvts
                float PreselEff = -1.;
                int NofPSEvts = -1;
                elem->QueryFloatAttribute ("PreselEff", &PreselEff);
                elem->QueryIntAttribute ("NofPSEvts", &NofPSEvts);
                if(PreselEff>0 && NofPSEvts>0){
                 (NofPSEvts<= d.eventTree()->GetEntries())? d.SetPreselEffAndNumberOfPreselEvents(PreselEff,NofPSEvts):d.SetPreselEffAndNumberOfPreselEvents(PreselEff,d.eventTree()->GetEntries()) ;
                }
                //Norm with nof of evts before preselection
                int NofEvts = -1;
                elem->QueryIntAttribute ("NofEvts", &NofEvts);
                if(NofEvts!=-1)
                        (NofEvts>0 && NofEvts<= d.eventTree()->GetEntries())? d.SetOriginalNumberOfEvents(NofEvts):d.SetOriginalNumberOfEvents(d.eventTree()->GetEntries());
                //Norm with eq lumi
                float EqLumi = -1;
                elem->QueryFloatAttribute ("EqLumi", &EqLumi);
                if(EqLumi>0)
                 d.SetEquivalentLuminosity(EqLumi);
                if(add) datasets.push_back(d);
        }

        elem = elem->NextSiblingElement ();     // iteration
    }
}






void AnalysisEnvironmentLoader::LoadSamples(vector<Dataset>& datasets, string datasetname){
  Reset();
  elem = NodeLoader(string("Datasets"));
  if(!elem) return;
  while (elem)
    {
        string name = elem->Attribute("name");
        string filename = elem->Attribute("filenames");
        cout<<"Load for dataset "<<name<<" the files "<<filename<<endl;
        int isData = 0;
        int add = 0;
        int ls = 1;
        int lw = 1;
        int color = 1;
        float normf = 1.;
        float xsection = 1.;
        float xsErrorMinus = 0.;
        float xsErrorPlus = 0.;
        elem->QueryIntAttribute ("ls", &ls);
        elem->QueryIntAttribute ("lw", &lw);
        elem->QueryIntAttribute ("color", &color);
        elem->QueryFloatAttribute ("normf", &normf);
        elem->QueryFloatAttribute ("xsection", &xsection);
        elem->QueryIntAttribute ("add", &add);
        elem->QueryIntAttribute ("isData", &isData);
        elem->QueryFloatAttribute ("xsErrorMinus", &xsErrorMinus);
        elem->QueryFloatAttribute ("xsErrorPlus", &xsErrorPlus);
        vector<string> filenames;
        bool star = SearchStar(filename,filenames);
        if(!star){
                filenames.clear();
                StringSeparator(filename,string(","),filenames);
        }

        //create a Dataset and push it back in the vector
        if(filenames.size()==0 || filenames[0] == string("")){
                Dataset d;
                d.SetName(name);
                d.SetCrossSection(xsection);
                d.SetIsData(isData);
                if(add && datasetname == name) datasets.push_back(d);
        }
        else{
             if(add && datasetname == name) {
                cout<<"THere "<<name<<" "<<isData<<endl;
                Dataset d(name, (bool) isData, true, color, ls, lw, normf, xsection, filenames);
                cout<<d.isData()<<endl;
                d.SetIsData(isData);
                cout<<d.isData()<<endl;
                d.SetCrossSectionError(xsErrorMinus,xsErrorPlus);
                //Norm with Eff and NofEvts
                float PreselEff = -1.;
                int NofPSEvts = -1;
                elem->QueryFloatAttribute ("PreselEff", &PreselEff);
                elem->QueryIntAttribute ("NofPSEvts", &NofPSEvts);
                if(PreselEff>0 && NofPSEvts>0){
                 (NofPSEvts<= d.eventTree()->GetEntries())? d.SetPreselEffAndNumberOfPreselEvents(PreselEff,NofPSEvts):d.SetPreselEffAndNumberOfPreselEvents(PreselEff,d.eventTree()->GetEntries()) ;
                }
                //Norm with nof of evts before preselection
                int NofEvts = -1;
                elem->QueryIntAttribute ("NofEvts", &NofEvts);
                if(NofEvts!=-1)
                        (NofEvts>0 && NofEvts<= d.eventTree()->GetEntries())? d.SetOriginalNumberOfEvents(NofEvts):d.SetOriginalNumberOfEvents(d.eventTree()->GetEntries());
                //Norm with eq lumi
                float EqLumi = -1;
                elem->QueryFloatAttribute ("EqLumi", &EqLumi);
                if(EqLumi>0)
                 d.SetEquivalentLuminosity(EqLumi);
                datasets.push_back(d);
             }
        }

        elem = elem->NextSiblingElement ();     // iteration
    }
}













void AnalysisEnvironmentLoader::LoadSelection (Selection& sel)
{

  Reset();
  elem = NodeLoader(string("Selection"));
  if(!elem) return;
  while (elem)
    {
      string type = elem->Attribute ("type");
      if (type == string ("Vertex"))
        {
          float VertexNdofThr=0;
          float VertexZThr=0;
          float VertexRhoThr=0;
          elem->QueryFloatAttribute ("VertexNdofThr", &VertexNdofThr);
          elem->QueryFloatAttribute ("VertexZThr", &VertexZThr);
          elem->QueryFloatAttribute ("VertexRhoThr", &VertexRhoThr);
          sel.SetVertexRequirements(VertexNdofThr, VertexZThr, VertexRhoThr);
        }
      if (type == string ("Electron"))
        {
          float PtThreshold=0;
          float EtaThreshold=0;
          float RelIso=0;
          float D0Cut=0;
          float VertexMatchThr=0;
          float ElectronETSCThr=0;
          float DRemuThr=0;
          elem->QueryFloatAttribute ("PtThreshold", &PtThreshold);
          elem->QueryFloatAttribute ("EtaThreshold", &EtaThreshold);
          elem->QueryFloatAttribute ("RelIso", &RelIso);
          elem->QueryFloatAttribute ("D0Cut", &D0Cut);
          elem->QueryFloatAttribute ("VertexMatchThr", &VertexMatchThr);
          elem->QueryFloatAttribute ("ElectronETSCThr", &ElectronETSCThr);
          elem->QueryFloatAttribute ("DRemuThr", &DRemuThr);
          sel.SetElectronRequirements(PtThreshold,EtaThreshold,RelIso,D0Cut,VertexMatchThr,ElectronETSCThr,DRemuThr);
        }
      if (type == string ("Muon"))
        {
          float PtThreshold=0;
          float EtaThreshold=0;
          float RelIso=0;
          float D0Cut=0;
          float VertexMatchThr=0;
          float NofValidHits=0;
          float NofValidTkHits=0;
          float NormChi2=0;
          elem->QueryFloatAttribute ("PtThreshold", &PtThreshold);
          elem->QueryFloatAttribute ("EtaThreshold", &EtaThreshold);
          elem->QueryFloatAttribute ("RelIso", &RelIso);
          elem->QueryFloatAttribute ("D0Cut", &D0Cut);
          elem->QueryFloatAttribute ("VertexMatchThr", &VertexMatchThr);
          elem->QueryFloatAttribute ("NofValidHits",   &NofValidHits);
          elem->QueryFloatAttribute ("NofValidTkHits", &NofValidTkHits);
          elem->QueryFloatAttribute ("NormChi2", &NormChi2);
          sel.SetMuonRequirements(PtThreshold,EtaThreshold,RelIso,D0Cut,VertexMatchThr,NofValidHits,NofValidTkHits,NormChi2);
        }
      if (type == string ("Tau"))
        {
          string Algo;
          float PtThreshold=0;
          float EtaThreshold=0;
          float VertexMatchThr=0;
          float TauLeadTrkPtCut=0;
          elem->QueryFloatAttribute ("PtThreshold", &PtThreshold);
          elem->QueryFloatAttribute ("EtaThreshold", &EtaThreshold);
          elem->QueryFloatAttribute ("VertexMatchThr", &VertexMatchThr);
          elem->QueryFloatAttribute ("TauLeadTrkPtCut", &TauLeadTrkPtCut);
          Algo = elem->Attribute("Algo");
          sel.SetDefaultTauCollection(Algo);
          sel.SetTauRequirements(PtThreshold,EtaThreshold,VertexMatchThr,TauLeadTrkPtCut);
        }
      if (type == string ("Jet"))
        {
          string Algo;
          float PtThreshold=0;
          float EtaThreshold=0;
          elem->QueryFloatAttribute ("PtThreshold", &PtThreshold);
          elem->QueryFloatAttribute ("EtaThreshold", &EtaThreshold);
          sel.SetJetRequirements(PtThreshold, EtaThreshold);
          Algo = elem->Attribute("Algo");
          sel.SetDefaultJetCollection(Algo);
        }
      elem = elem->NextSiblingElement ();       // iteration
    }
}


void AnalysisEnvironmentLoader::LoadDiLeptonSelection (DiLeptonSelection& sel)
{

  LoadSelection(dynamic_cast<Selection&>(sel));

  Reset();

  elem = NodeLoader(string("DiLeptonSelection"));
  if(!elem) return;
      float MinMassCut = 0;
      float METEMu = 0;
      float METLL = 0;
      float ZMassWindowMin = 0;
      float ZMassWindowMax = 0;
      int btagAlgo = -1;
      float btagDiscriCut = -999.;
      int NofBtagJets = 0;
      elem->QueryFloatAttribute ("MinMassCut", &MinMassCut);
      elem->QueryFloatAttribute ("METCutsEMU", &METEMu);
      elem->QueryFloatAttribute ("METCutsLL",  &METLL);
      elem->QueryFloatAttribute ("ZMassWindowMin", &ZMassWindowMin);
      elem->QueryFloatAttribute ("ZMassWindowMax", &ZMassWindowMax);
      elem->QueryIntAttribute ("btagAlgo", &btagAlgo);
      elem->QueryFloatAttribute ("btagDiscriCut", &btagDiscriCut);
      elem->QueryIntAttribute ("NofBtagJets", &NofBtagJets);
      sel.SetParameters(MinMassCut, pair<float,float> (METEMu,METLL), pair<float,float> (ZMassWindowMin,ZMassWindowMax), btagAlgo, btagDiscriCut, NofBtagJets);
}


void AnalysisEnvironmentLoader::LoadSemiLeptonicTauSelection (SemiLeptonicTauSelection& sel)
{

  LoadSelection(dynamic_cast<Selection&>(sel));
  Reset();
  elem = NodeLoader(string("SemiLeptonicTauSelection"));
  if(!elem) return;
     
      float METCut = 0;
     
      int btagAlgo = -1;
      float btagDiscriCut = -999.;
      int NofBtagJets = 0;
      elem->QueryFloatAttribute ("METCut", &METCut);
      elem->QueryIntAttribute ("btagAlgo", &btagAlgo);
      elem->QueryFloatAttribute ("btagDiscriCut", &btagDiscriCut);
      elem->QueryIntAttribute ("NofBtagJets", &NofBtagJets);
      sel.SetParameters(btagAlgo, btagDiscriCut, NofBtagJets, METCut);
}

void AnalysisEnvironmentLoader::LoadWeight (DiLeptonSelection& sel)
{
  LoadSelection(dynamic_cast<Selection&>(sel));
  Reset();
  elem = NodeLoader(string("Weight"));
  string type = elem->Attribute ("type");
  if(!elem) return;
      int flag =0;
      int methodb =0;
      int systb =0;
      if (type == string ("Btag")) {
          elem->QueryIntAttribute ("flag", &flag);
          elem->QueryIntAttribute ("methodb", &methodb);
          elem->QueryIntAttribute ("systb", &systb);
      }
      std::cout << " Flag for  Btag Weight : " << flag << std::endl;
      int btagAlgo = sel.GetbtagAlgo();
      float btagDiscriCut = sel.GetbtagDiscriCut();
      int btagNjet = sel.GetNofBtagJetsCut();
      sel.InitSFBWeight(flag, methodb, systb, btagAlgo, btagDiscriCut, btagNjet);
      sel.InitJESUnc();
}

void AnalysisEnvironmentLoader::LoadWeight (SemiLeptonicTauSelection& sel)
{
  LoadSelection(dynamic_cast<Selection&>(sel));
  Reset();
  elem = NodeLoader(string("Weight"));
  string type = elem->Attribute ("type");
  if(!elem) return;
      int flag =0;
      int methodb =0;
      int systb =0;
      if (type == string ("Btag")) {
          elem->QueryIntAttribute ("flag", &flag);
          elem->QueryIntAttribute ("methodb", &methodb);
          elem->QueryIntAttribute ("systb", &systb);
      }
      std::cout << " Flag for  Btag Weight : " << flag << std::endl;
      int btagAlgo = sel.GetbtagAlgo();
      std::cout << " LoadWeight btagAlgo : " << btagAlgo << std::endl;
      float btagDiscriCut = sel.GetbtagDiscriCut();
      int btagNjet = sel.GetNofBtagJetsCut();
      sel.InitSFBWeight(flag, methodb, systb, btagAlgo, btagDiscriCut, btagNjet);
      sel.InitJESUnc();

}
void AnalysisEnvironmentLoader::StringSeparator(string s, string sep, vector<int>& out)
{
  string::size_type pos, pos_ex = 0;
  while ( (pos = s.find(sep, pos_ex)) != string::npos)
  {
        string a = s.substr(pos_ex, pos-pos_ex);
        out.push_back(atoi(a.c_str()));
        pos_ex = pos + 1;
  }
  string a = s.substr(pos_ex);
  out.push_back(atoi(a.c_str()));
}

void AnalysisEnvironmentLoader::StringSeparator(string s, string sep, vector<float>& out)
{
  string::size_type pos, pos_ex = 0;
  while ( (pos = s.find(sep, pos_ex)) != string::npos)
  {
        string a = s.substr(pos_ex, pos-pos_ex);
        out.push_back(atof(a.c_str()));
        pos_ex = pos + 1;
  }
  string a = s.substr(pos_ex);
  out.push_back(atof(a.c_str()));
}

void AnalysisEnvironmentLoader::StringSeparator(string s, string sep, vector<string>& out)
{
  string::size_type pos, pos_ex = 0;
  while ( (pos = s.find(sep, pos_ex)) != string::npos)
  {
        out.push_back(s.substr(pos_ex, pos-pos_ex));
        pos_ex = pos + 1;
  }
  out.push_back(s.substr(pos_ex));
}

bool AnalysisEnvironmentLoader::SearchStar(string s, vector<string>& out)
{
  int pos = s.find('*');
  if(pos<(int)s.size() && pos>=0){
        string command;
        command = "ls -ltrh " + s + "| awk {'print $9'} > tmp.AnaEnvLoader";
        system(command.c_str());
        ifstream ifile("tmp.AnaEnvLoader");
        while(!ifile.eof()){
                string sread;
                ifile>>sread;
                out.push_back(sread);
        }
        out.erase(out.end()-1,out.end());
        system("rm tmp.AnaEnvLoader");
        return true;
  }
  return false;
}

void AnalysisEnvironmentLoader::LoadAnalysisInfo(string typeName, string varName, int& value){
  Reset();
  elem = NodeLoader(string("Analysis"));
  if(!elem) return;
  while (elem)
    {
      string type = elem->Attribute ("type");
      if (type == typeName)
        elem->QueryIntAttribute (varName.c_str(), &value);
      elem = elem->NextSiblingElement ();       // iteration
    }
}

void AnalysisEnvironmentLoader::LoadAnalysisInfo(string typeName, string varName, float& value){
  Reset();
  elem = NodeLoader(string("Analysis"));
  if(!elem) return;
  while (elem)
    {
      string type = elem->Attribute ("type");
      if (type == typeName)
        elem->QueryFloatAttribute (varName.c_str(), &value);
      elem = elem->NextSiblingElement ();       // iteration
    }
}

void AnalysisEnvironmentLoader::LoadAnalysisInfo(string typeName, string varName, bool& value){
  Reset();
  elem = NodeLoader(string("Analysis"));
  if(!elem) return;
  int val = 0;
  while (elem)
    {
      string type = elem->Attribute ("type");
      if (type == typeName)
        elem->QueryIntAttribute (varName.c_str(), &val);
      elem = elem->NextSiblingElement ();       // iteration
    }
  value = val;
}
void AnalysisEnvironmentLoader::LoadAnalysisInfo(string typeName, string varName, string& value){
  Reset();
  elem = NodeLoader(string("Analysis"));
  if(!elem) return;
  while (elem)
    {
      string type = elem->Attribute ("type");
      if (type == typeName)
        value = elem->Attribute(varName.c_str());
      elem = elem->NextSiblingElement ();       // iteration
    }
}

