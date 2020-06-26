#include "../interface/PLRMeasurement.h"


//important: it's needed to have the FCN working
PLRMeasurement *myPLRMeas = 0;


PLRMeasurement::PLRMeasurement ()
{

  Lumi = 1.;
  LumiError = 0;
  isLumiASyst = false;
  
  InterpolMethod = string("linear");

  MinFCN = 0.;

  SizeOfParam = 1000;
  Param = new double[SizeOfParam];
  ErrParam = new double[SizeOfParam];
  ErrNegParam = new double[SizeOfParam];
  ErrPosParam = new double[SizeOfParam];
  ErrParaParam = new double[SizeOfParam];
  Param_Min = new double[SizeOfParam];
  ErrParam_Min = new double[SizeOfParam];
  ErrNegParam_Min = new double[SizeOfParam];
  ErrPosParam_Min = new double[SizeOfParam];
  ErrParaParam_Min = new double[SizeOfParam];
  for (int i = 0; i < SizeOfParam; i++) {
    Param[i] = 0;
    ErrParam[i] = 0;
    ErrPosParam[i] = 0;
    ErrNegParam[i] = 0;
    ErrParaParam[i] = 0;
    Param_Min[i] = 0;
    ErrParam_Min[i] = 0;
    ErrPosParam_Min[i] = 0;
    ErrNegParam_Min[i] = 0;
    ErrParaParam_Min[i] = 0;
  }
  
  StatGraph1D = 0;
  SystGraph1D = 0; 
  StatGraph2D = 0;
  SystGraph2D = 0;  
  StatFit = 0;
  SystFit = 0;
 
  time_1 = 0;
  time_2 = 0;
  time_3 = 0;
  time_4 = 0;

  doMCPSExp = false;

  doAnim = false;
  InitAnimateGraph();
  gStyle = new TStyle();
  gStyle->SetOptStat(0);
  gStyle->SetTitleFillColor(kWhite);
  gStyle->SetTitleFontSize(0.08);

  //important: it's needed to have the FCN working
  myPLRMeas = this;
}



PLRMeasurement::PLRMeasurement (const PLRMeasurement & meas)
{

  datasets = meas.datasets;
  uncertainties = meas.uncertainties;
  Lumi = meas.Lumi;
  LumiError = meas.LumiError;
  isLumiASyst = meas.isLumiASyst;

  MinFCN = meas.MinFCN;
  SizeOfParam = meas.SizeOfParam;

  Param = new double[SizeOfParam];
  ErrParam = new double[SizeOfParam];
  ErrNegParam = new double[SizeOfParam];
  ErrPosParam = new double[SizeOfParam];
  ErrParaParam = new double[SizeOfParam];
  Param_Min = new double[SizeOfParam];
  ErrParam_Min = new double[SizeOfParam];
  ErrNegParam_Min = new double[SizeOfParam];
  ErrPosParam_Min = new double[SizeOfParam];
  ErrParaParam_Min = new double[SizeOfParam];
  for (int i = 0; i < SizeOfParam; i++) {
    Param[i] = meas.Param[i];
    ErrParam[i] = meas.ErrParam[i];
    ErrNegParam[i] = meas.ErrNegParam[i];
    ErrPosParam[i] = meas.ErrPosParam[i];
    ErrParaParam[i] = meas.ErrParaParam[i];
    Param_Min[i] = meas.Param_Min[i];
    ErrParam_Min[i] = meas.ErrParam_Min[i];
    ErrNegParam_Min[i] = meas.ErrNegParam_Min[i];
    ErrPosParam_Min[i] = meas.ErrPosParam_Min[i];
    ErrParaParam_Min[i] = meas.ErrParaParam_Min[i];
  }

  doMCPSExp = meas.doMCPSExp;

  if(meas.StatGraph1D!=0) StatGraph1D = (TGraph*)meas.StatGraph1D->Clone();
  if(meas.SystGraph1D!=0) SystGraph1D = (TGraph*)meas.SystGraph1D->Clone();
  if(meas.StatGraph2D!=0) StatGraph2D = (TGraph2D*)meas.StatGraph2D->Clone();
  if(meas.SystGraph2D!=0) SystGraph2D = (TGraph2D*)meas.SystGraph2D->Clone();
  if(meas.StatFit!=0) StatFit = (TF2*) meas.StatFit->Clone();
  if(meas.SystFit!=0) SystFit = (TF2*) meas.SystFit->Clone();
        
  //copy of histos is missing
  ///////////////////////////////////////////


 // copy of estimators is missing
  
  time_1 = meas.time_1;
  time_2 = meas.time_2;
  time_3 = meas.time_3;
  time_4 = meas.time_4;

//important: it's needed to have the FCN working
  myPLRMeas = this;
}

PLRMeasurement::~PLRMeasurement ()
{
  delete[]Param;
  delete[]ErrParam;
  delete[]ErrNegParam;
  delete[]ErrPosParam;
  delete[]ErrParaParam;
  delete[]Param_Min;
  delete[]ErrParam_Min;
  delete[]ErrNegParam_Min;
  delete[]ErrPosParam_Min;
  delete[]ErrParaParam_Min;
  //delete[]X_PLR;
  //delete[]Y_PLR;
}

void PLRMeasurement::SetLumi (double value, double absErr)
{
  Lumi = value;
  LumiError = absErr;
  Parameter par;
  ResetParameter(par);
  par.name = string("Lumi");
  par.ExpectedValue = value;
  par.PEValue = value;
  par.value = value;
  par.isZeroCentered = false;
  par.uncertainty = pair<double,double>(absErr,absErr);
  par.range = pair<double,double>(value-absErr,value+absErr);
  if(isLumiASyst){
         par.fixed = false;
         par.isANuisanceParameter = true;
  }
  else par.fixed = true;
  parameters.push_back(par);
}

double PLRMeasurement::GetLumi () const
{
  return Lumi;
}

double PLRMeasurement::GetLumiError () const
{
  return LumiError;
}

void PLRMeasurement::SetIsLumiASyst(bool isASyst){
  isLumiASyst = isASyst;
  for(unsigned int i=0;i<parameters.size();i++){
        if(parameters[i].name == string("Lumi")){
                if(isASyst){
                        parameters[i].fixed = false;
                        parameters[i].isANuisanceParameter = true;
                }
                else parameters[i].fixed = true;
        }
  } 
}

void PLRMeasurement::SetSignalInfo(const string& signalDSName, const bool& isASyst){
        SignalDatasetName = signalDSName;
        isSignalXSASyst = isASyst;
}
        

vector<int> PLRMeasurement::GetSignalDatasetIterator() const{
        vector<int> idatasets;
        for(unsigned int i=0;i<datasets.size();i++){
                if(datasets[i].Name().find(SignalDatasetName)<=datasets[i].Name().size()) idatasets.push_back((int)i);
        }
        return idatasets;
}

vector<int> PLRMeasurement::GetSignalDatasetIteratorForHisto() const{
        vector<int> idatasets;
        int it=0;
        for(unsigned int i=0;i<datasets.size();i++){
                if(datasets[i].Name().find(SignalDatasetName)<=datasets[i].Name().size()) idatasets.push_back(it);
                if(!datasets[i].isData())it++;
        }
        return idatasets;
}


void PLRMeasurement::LoadDatasets (const vector < Dataset > &Datasets)
{
  datasets = Datasets;
  if (Lumi <= 0) {
    cerr << "PLRMeasurement::The luminosity has to be provided before loading the datasets !" << endl;
    return;
  }
  //Add the uncertainty on the Xsection as first one
  Xsection.SetName (string ("Xsection"));
  Xsection.LoadDatasets (datasets);
  for (unsigned int d = 0; d < datasets.size (); d++) {
    Xsection.SetParameters (d, datasets[d].Xsection (), (1 - datasets[d].GetCrossSectionError ().first) * datasets[d].Xsection (), (1 + datasets[d].GetCrossSectionError ().second) * datasets[d].Xsection ());
    Xsection.LoadEfficiencies (d, datasets[d].Xsection (), (1 - datasets[d].GetCrossSectionError ().first) * datasets[d].Xsection (), (1 + datasets[d].GetCrossSectionError ().second) * datasets[d].Xsection ());
  }

}

//--- Initialisation methods ---// 
        
void PLRMeasurement::AddChannel(const string& name, const string& signalDatasetName, const double& BR){
        ChannelDescription cd(name, signalDatasetName, BR);
        //add to the list
        channels.push_back(cd); 
}
        
void PLRMeasurement::AddFreeParameter(const string& name, const double& ExpectedValue, const int& npoints, const int& nbins, const double& xmin, const double& xmax, const double& xErrmin, const double& xErrmax,const double& step, const bool& WithGraph){
        Parameter par;
        ResetParameter(par);
        par.isAnEstimator = true;
        par.isANuisanceParameter = false;
        par.WithGraph = WithGraph;
        par.name = name;
        par.ExpectedValue = ExpectedValue;
        par.PEValue = ExpectedValue;
        par.value = ExpectedValue;
        par.NofPoints = npoints;
        par.nbins = nbins;
        par.range = pair<double,double>(xmin,xmax);
        par.ErrRange = pair<double,double>(xErrmin,xErrmax);
        par.step = step;
        //add to the list
        parameters.push_back(par);
}
        
void PLRMeasurement::AddNuisanceParameter(const string& name, bool isGlobal, bool WithGraph){
        //take care:: isGlobal means it affect globally the signal
        Parameter par;
        ResetParameter(par);
        par.isANuisanceParameter = true;
        par.isAnEstimator = false;
        if(isGlobal){
                par.isZeroCentered = false;
                par.ExpectedValue = 1.;
                par.value = 1.;
                par.PEValue = 1.;
        }
        else{
                par.isZeroCentered = true;
                par.ExpectedValue = 0.;
                par.value = 0.;
                par.PEValue = 0.;
        }
        par.isGlobal = isGlobal;
        par.WithGraph = WithGraph;
        par.uncertainty = pair<double,double>(1.,1.);
        par.name = name;
        //add to the list
        parameters.push_back(par);
}
        
void PLRMeasurement::AddNuisanceParameter(const string& name, bool isGlobal, bool WithGraph, const float& Estimation, const float& ErrNeg, const float& ErrPos){
        //take care:: isGlobal means it affect globally the signal
        Parameter par;
        ResetParameter(par);
        par.isANuisanceParameter = true;
        par.isAnEstimator = false;
        par.ExpectedValue = Estimation;
        if(isGlobal){
                par.isZeroCentered = false;
                par.value = 1.;
                par.PEValue = 1.;
        }
        else{
                if(Estimation!=0) par.isZeroCentered = false;
                else par.isZeroCentered = true;
                par.value = 0.;
                par.PEValue = 0.;
        }
        par.isGlobal = isGlobal;
        par.WithGraph = WithGraph;
        par.uncertainty = pair<double,double>(ErrNeg,ErrPos);
        par.range = pair<double,double>(Estimation-3*ErrNeg,Estimation+3*ErrPos);
        par.name = name;
        cout<<par.name<<" "<<par.ExpectedValue<<" "<<par.isZeroCentered<<endl;
        //add to the list
        parameters.push_back(par);
}
        
void PLRMeasurement::AddHisto(const string& name, const int& dim){
        for(unsigned int i=0; i<channels.size();i++){
                channels[i].AddHisto(name, channels[i].GetChannelName(), dim, GetIsADDDatasetList() );
        }
}

void PLRMeasurement::LoadHistos(){
        TFile* fin = new TFile(HistoRootFileName.c_str(),"READ");
        for(unsigned int i=0; i<channels.size();i++){
                channels[i].LoadDistributions(fin, datasets, GetNuisanceParametersNoGraph(), GetNuisanceParametersGraph(), GetEstimatorsNoGraph(), GetEstimatorsGraph());
                for(unsigned int j=0;j<channels[i].GetHistosAddress().size();j++)
                        channels[i].GetHistosAddress()[j].SetInterpolMethod(InterpolMethod);
        }
        fin->Close();
        delete fin;
}

void PLRMeasurement::AddSignalSystematic(const vector<Systematic>& s){
        SignalSyst.push_back(s);
        string nameref;
        bool isOK = true;
        for(unsigned int i=0;i<s.size();i++){
                if(i==0) nameref = s[i].name;
                else{
                        if(nameref!=s[i].name){
                         cerr<<"PLRMeasurement::AddSignalSystematic:: There is an inconsistency in the name of the systematic there [ "<<nameref<< "]. Look into your xml file !!"<<endl;
                         isOK = false;
                        }
                }
        }
        if(isOK && s.size()>0) AddNuisanceParameter(nameref, true);// putting true is important, it means that it is a signal global systematic
}


//Interpolation

void PLRMeasurement::SetInterpolMethod(string name) {
        InterpolMethod = name;
        for(unsigned int i=0;i<channels.size();i++){
                for(unsigned int j=0;j<channels[i].GetHistosAddress().size();j++)
                        channels[i].GetHistosAddress()[j].SetInterpolMethod(name);
        }
}

double PLRMeasurement::Interpol(double x, double* par){
        if(InterpolMethod == string("linear")) return LinearInterpol(x, par);
        if(InterpolMethod == string("expo")) return ExpoInterpol(x, par);
        if(InterpolMethod == string("cubic")) return CubicInterpol(x, par);
        cerr<<"PLRMeasurement::Interpol:: no prefered method - use linear method"<<endl; 
        return LinearInterpol(x, par);
}

double PLRMeasurement::Interpol(double x, double mean, double effm1s, double effp1s){
        double par[3];
        par[0] = mean;
        par[1] = effm1s;
        par[2] = effp1s;
        return Interpol(x,par);
}

////////////////////////////////////////////////////


void PLRMeasurement::ComputeLikelihood (vector < bool > fixParam, vector < double >ParamValues, bool WriteSysName, bool doCorrelationMatrixHisto, bool verbose)
{

  //check size of vectors
  if(fixParam.size()!= parameters.size() || ParamValues.size()!=parameters.size()){
        cerr<<"PLRMeasurement::ComputeLikelihood:: size of the input vectors is incorrect ("<<parameters.size()<<" vs "<<fixParam.size()<<" and "<<ParamValues.size()<<endl;
  }

  TMinuit minuit (GetParametersAddress().size());
  // Set fonction pointer
  minuit.SetFCN (PLRFcn);

  double arglist[GetParametersAddress().size()];
  int ierflg = 0;

  if (verbose) {
    cout << "----------------------------" << endl;
    cout << " ComputeLikelihood    " << endl;
    cout << "----------------------------" << endl;
    cout << "List of parameters (Fix/Not)-value" << endl;
    for (unsigned int i = 0; i < fixParam.size (); i++) {
      cout << "Param[" << i << "] = " << ParamValues[i] << " fixed " << fixParam[i] << endl;
    }
  }

  for(unsigned i=0;i<parameters.size();i++){
        if (verbose) cout<<"name = "<<parameters[i].name<<" step = "<<parameters[i].step <<" value = "<< ParamValues[i] <<" min: "<< parameters[i].range.first <<" max: "<< parameters[i].range.second<<" "<<parameters[i].isZeroCentered<<endl;
        minuit.mnparm (i, parameters[i].name, ParamValues[i], parameters[i].step, parameters[i].range.first, parameters[i].range.second, ierflg);
  }

  for (unsigned int i = 0; i < fixParam.size (); i++) {
    if (fixParam[i])
      minuit.FixParameter (i);
  }

  // Set Print Level
  // -1 no output
  // 1 standard output   
  minuit.SetPrintLevel (-1);
  minuit.mnexcm("SET NOW", arglist ,1,ierflg);

  // Set error Definition: 0.5 for negative log likelihood
  minuit.SetErrorDef (0.5);

  // 1 standard
  // 2 try to improve minimum (slower)
  arglist[0] = 1;
  clock_t t0 = 0;
  clock_t t = 0;
  t0 = clock();
  minuit.mnexcm ("SET STR", arglist, 1, ierflg);
  t = clock();
  time_1 += t-t0;
  t0 = clock();


  //arglist[0] = 10000; //max iterations
  arglist[0] = 10000; //max iterations
  minuit.mnexcm ("MIGRAD", arglist, 1, ierflg);
  //minuit.mnexcm ("MINOS", arglist, 1, ierflg);
  t = clock();
  time_2 += t-t0;
  t0 = clock();
  minuit.mnexcm ("STOP", arglist, 0, ierflg);
  t = clock();
  time_3 += t-t0;

  // Values of the fitting parameters  
  double ln0, edm, errdef;
  int nvpar, nparx, icstat;
  minuit.mnstat (ln0, edm, errdef, nvpar, nparx, icstat);
  MinFCN = ln0;                 // to be defined

  // Values of errors (parabolic and MINOS)  
  for (unsigned int j = 0; j < GetParametersAddress().size(); ++j) {
    double gcc;
    ErrNegParam[j] = 0.;        // to be defined
    ErrPosParam[j] = 0.;        // to be defined
    ErrParaParam[j] = 0.;       // to be defined
    minuit.mnerrs (j, ErrPosParam[j], ErrNegParam[j], ErrParaParam[j], gcc);
  }
  for (unsigned int j = 0; j < GetParametersAddress().size(); ++j) {
    minuit.GetParameter (j, Param[j], ErrParam[j]);
  }
  if (verbose) {
    cout << "################# after minimization ####################" << endl;
    for (unsigned int j = 0; j < GetParametersAddress().size(); ++j) {
      std::cout << "Param[" << j << "] = " << Param[j] << " +/-" << ErrParam[j] << " [parabolic] "<<ErrParaParam[j] <<" (" << ErrNegParam[j] << "," << ErrPosParam[j] << ")" << std::endl;
    }
  }

  if (doCorrelationMatrixHisto) {
    cout<<"Correlation Matrix !"<<endl;
    int size = GetParametersAddress().size();
    CorrelationMatrixHisto = new TH2F ("CorrelationMatrixHisto", "Correlation Matrix", size, 0, size, size, 0, size);
    for (unsigned int i = 0; i < GetParametersAddress().size(); i++) {
      CorrelationMatrixHisto->GetXaxis ()->SetBinLabel (i + 1, GetParametersAddress()[i].name.c_str ());
      CorrelationMatrixHisto->GetYaxis ()->SetBinLabel (i + 1, GetParametersAddress()[i].name.c_str ());
    }
    double matrix[size][size];
    minuit.mnemat (&matrix[0][0], size);
    for (int i = 0; i < size; i++) {
      for (int j = 0; j < size; j++) {
        CorrelationMatrixHisto->SetBinContent (i + 1, j + 1, matrix[i][j]);
      }
    }
  }

}

/*
void PLRMeasurement::Plot (TString S, int NofPoints, double Xmin, double Xmax, double minimumL, bool verbose)
{
  c->cd ();
  double P[100];
  for (unsigned int j = 0; j < GetParametersAddress().size(); j++) {
    P[j] = Param[j];
  }

  double x[NofPoints], y[NofPoints];
  for (int i = 0; i < NofPoints; i++) {
    P[0] = Xmin + (Xmax - Xmin) * i / NofPoints;
    x[i] = P[0];
    y[i] = XsectionFCN (P) - minimumL;  // - car difference de 2 log (<=> rapport des likelihoods)
  }
  Ymin_plot = 9999999.;
  for (int i = 0; i < NofPoints; ++i) {
    if (y[i] < Ymin_plot) {
      Ymin_plot = y[i];
    }
  }

  //Call a graphic function to have stat plots

  //PLRExamination (NofPoints, X_PLR, Y_PLR, -1);       //-1 means it's purely statistic
  PLRExamination (NofPoints, x, y, -1); //-1 means it's purely statistic

  if (verbose) {
    cout << "--------------------------------------" << endl;
    cout << "PLR just with: " << XsectionEstimStat << " -" << XsectionEstimStatError.first << "/+" << XsectionEstimStatError.second << endl;
    cout << "--------------------------------------" << endl;
  }




}
*/

/*
void PLRMeasurement::Plot_PLR (int NofPoints, double Xmin, double Xmax, double *x, double *y, TGraph * &graph, int color, string name)
{
  c->cd ();

  graph = new TGraph (NofPoints, x, y);
  graph->SetMarkerStyle (20);
  graph->SetMarkerSize (1.);
  graph->SetMarkerColor (color);
  leg->AddEntry (graph, name.c_str (), "P");
  graph->SetTitle ("");
     //if (S == "AP")
     //graph->Draw ("*");
     //if (S == "Same")
   
  graph->Draw ("Psame");
}
*/

double PLRMeasurement::EvaluateFCN (Double_t * par)
{
  //int npar = uncertainties.size () + 2; 
  int npar = GetParametersAddress().size();
  int iflag = 1;                // compute
  double returnValue = 0.;
  double gin[npar];             //derivative
  for (int i = 0; i < npar; i++)
    gin[i] = 0;
  PLRFcn (npar, gin, returnValue, par, iflag);
  return (returnValue);
}


void LoadParameterInFCN(double& value, const string& name, Double_t*& par){
  Parameter param;
  int ipar = -1;
  param = myPLRMeas->GetParameter(name, ipar);
  if(ipar<0) cerr<<"LoadParameterInFCN:: problem while loading "<<name<<endl;
  else{
        if(param.isAnEstimator) value = par[ipar];
        else{
                if(param.isANuisanceParameter){
                        /*
                        if(param.name == string("SignalXsection")){
                                if(par[ipar]<0) value = par[ipar];
                                if(par[ipar]>0) value = par[ipar];
                        }
                        else{
                                if(par[ipar]<0) value = param.ExpectedValue + par[ipar]*param.uncertainty.first;
                                if(par[ipar]>0) value = param.ExpectedValue + par[ipar]*param.uncertainty.second;
                        }
                        */
                        value = par[ipar];
                }
                else{
                        value = param.ExpectedValue;
                }
        }
  }
        //cout<<"VAL = "<<value<<endl;
}

void PLRFcn (Int_t & npar, Double_t * gin, Double_t & f, Double_t * par, Int_t iflag)
{
  bool debug = false;           // put to true carrefully ... this function is called a lot of time. This boolean cannot be give as a parameter

  //double BRChannel = myPLRMeas->GetBRChannel ();
  double LnLikelihood = 0;

  //--- Signal Xsection ---//
  double localSignXsection = 0;
  double SignalFactor = 1.;
  LoadParameterInFCN(localSignXsection, string("SignalXsection"), par);
  SignalFactor = localSignXsection/myPLRMeas->GetSignalXsection();

  //--- Luminosity ---// 
  double localLumi = 0;
  double LumiFactor = 1.;
  LoadParameterInFCN(localLumi, string("Lumi"), par);
  LumiFactor = localLumi/myPLRMeas->GetLumi();
  //cout<<"LumiFactor = "<<LumiFactor<<" = "<<localLumi<<" "<<myPLRMeas->GetLumi()<<endl;

  if (debug) {
    cout << "###############################" << endl;
    cout << "    In PLRFcn                   " << endl;
    cout << "###############################" << endl;
    cout << "LocalSignXsection = " << localSignXsection << endl;
    cout << "LocalLumi = " << localLumi << endl;
  }

    if (debug) {
      cout << "--------------------------" << endl;
      cout << "    Computation of LnLike " << endl;
      cout << "--------------------------" << endl;
    }
  
    // You have to know if it is a combination, if you use all histos ...

    //loop over the channel
    double Nobs_tot = 0;
    double Nexp_tot = 0;
    if(debug) cout<<"Nof channels = "<<myPLRMeas->GetChannelsAddress().size()<<endl;
    //cout<<"localXsection: "<<localSignXsection<<endl;
    for(unsigned int c=0; c<myPLRMeas->GetChannelsAddress().size();c++){
        myPLRMeas->GetChannelsAddress()[c];
        //if no histos

        //loop over histos
        if(debug) cout<<"  Channel "<<myPLRMeas->GetChannelsAddress()[c].GetChannelName()<<" Nof Histos = "<<myPLRMeas->GetChannelsAddress()[c].GetHistosAddress().size()<<endl;
        for(unsigned int h=0;h<myPLRMeas->GetChannelsAddress()[c].GetHistosAddress().size();h++){
                        //--------------------------------------//
                        //vector of uncertainties values
                        vector<double> uncert; // uncertainty is modelized with 3 points and an interpolation
                        vector<double> GraphUncert;  //uncertainty is modelized with a graphic
                        vector<double> estim; // vector of the value for the estimator
                        vector<double> GraphEstim; // the estimator are modelized with a graphic
                        vector<pair<string,double> > SignalGlobalSyst; //systematic global (affect the signal by a flat factor
                        if(debug) cout<<"Nof parameters: "<<myPLRMeas->GetParametersAddress().size()<<endl;
                        for(unsigned int u=0;u<myPLRMeas->GetParametersAddress().size();u++){
                                //cout<<"Param name: "<<myPLRMeas->GetParametersAddress()[u].name<<endl;
                                if(myPLRMeas->GetParametersAddress()[u].name!=string("Lumi") && myPLRMeas->GetParametersAddress()[u].name!=string("SignalXsection") && !myPLRMeas->GetParametersAddress()[u].isNormalization && !myPLRMeas->GetParametersAddress()[u].isGlobal){
                                        if(myPLRMeas->GetParametersAddress()[u].isANuisanceParameter){
                                                if(!myPLRMeas->GetParametersAddress()[u].WithGraph){
                                                        if(!myPLRMeas->GetParametersAddress()[u].fixed) uncert.push_back(par[u]);
                                                        else uncert.push_back(myPLRMeas->GetParametersAddress()[u].ExpectedValue);//0.0000 
                                                }
                                                else{
                                                        if(!myPLRMeas->GetParametersAddress()[u].fixed) GraphUncert.push_back(par[u]);
                                                        else GraphUncert.push_back(myPLRMeas->GetParametersAddress()[u].ExpectedValue);//0.0000 
                                                }
                                        }
                                        if(myPLRMeas->GetParametersAddress()[u].isAnEstimator){
                                                if(!myPLRMeas->GetParametersAddress()[u].WithGraph){
                                                        if(!myPLRMeas->GetParametersAddress()[u].fixed) estim.push_back(par[u]);
                                                        else estim.push_back(myPLRMeas->GetParametersAddress()[u].ExpectedValue);//0.0000 
                                                }
                                                else{
                                                        if(!myPLRMeas->GetParametersAddress()[u].fixed) GraphEstim.push_back(par[u]);
                                                        else GraphEstim.push_back(myPLRMeas->GetParametersAddress()[u].ExpectedValue); //0.0000
                                                }
                                        }
                                }
                                if(myPLRMeas->GetParametersAddress()[u].isGlobal){
                                 SignalGlobalSyst.push_back(pair<string, double>(myPLRMeas->GetParametersAddress()[u].name,par[u]));
                                }
                        }
                        if(debug){
                                cout<<"Global uncertainties: "<<endl;
                                for(unsigned int x=0;x<SignalGlobalSyst.size();x++){
                                        cout<<SignalGlobalSyst[x].first<<" "<<SignalGlobalSyst[x].second<<endl;
                                }
                                cout<<"uncertainties modelized with a graph: "<<endl;
                                for(unsigned int x=0;x<GraphUncert.size();x++){
                                        cout<<GraphUncert[x]<<endl;
                                }
                                cout<<"uncertainties modeled without a graph: "<<endl;
                                for(unsigned int x=0;x<uncert.size();x++){
                                        cout<<uncert[x]<<endl;
                                }
                                cout<<"estimators modelized with a graph: "<<endl;
                                for(unsigned int x=0;x<GraphEstim.size();x++){
                                        cout<<GraphEstim[x]<<endl;
                                }
                                cout<<"estimators modelized without a graph: "<<endl;
                                for(unsigned int x=0;x<estim.size();x++){
                                        cout<<estim[x]<<endl;
                                }
                        }
                        //---------------------------------------------------------------
                        //  Systematics of the normalization (level of backgrounds)
                        //---------------------------------------------------------------
                        vector<double> norm;
                        for(unsigned int id=0;id<myPLRMeas->GetDatasets().size();id++){
                                int iu = -1;
                                //if(!myPLRMeas->GetDatasets()[id].isData()){
                                if(!myPLRMeas->GetDatasets()[id].isData() && myPLRMeas->GetDatasets()[id].Name()!=myPLRMeas->GetSignalDatasetName()){
                                        for(unsigned int u=0;u<myPLRMeas->GetParametersAddress().size();u++){
                                                if(myPLRMeas->GetParametersAddress()[u].name.find(myPLRMeas->GetDatasets()[id].Name())<myPLRMeas->GetParametersAddress()[u].name.size()){
                                                        if(myPLRMeas->GetParametersAddress()[u].isCommon){
                                                                if(debug) cout<<myPLRMeas->GetParametersAddress()[u].name<<" in channel "<<myPLRMeas->GetChannelsAddress()[c].GetChannelName()<<" is a common parameter"<< endl;
                                                                iu = u;
                                                                break;
                                                        }
                                                        else{
                                                                if(myPLRMeas->GetParametersAddress()[u].name.find(myPLRMeas->GetChannelsAddress()[c].GetChannelName())<myPLRMeas->GetParametersAddress()[u].name.size()){
                                                                        if(debug) cout<<myPLRMeas->GetParametersAddress()[u].name<<" in channel "<<myPLRMeas->GetChannelsAddress()[c].GetChannelName()<<" is NOT a common parameter"<< endl;
                                                                        iu = u;
                                                                        break;
                                                                }
                                                        }
                                                }
                                        }
                                        if(iu>0) norm.push_back(par[iu]);
                                        else norm.push_back(-1);
                                }
                        }
                        //---------------------------------------------------------------
                        //  Compute the signal global systematic effect
                        //---------------------------------------------------------------
                        double SigGlobFactor = 1.;
                        for(unsigned int i=0;i<SignalGlobalSyst.size();i++){
                                for(unsigned int j=0;j<myPLRMeas->GetSignalSystematics().size();j++){
                                        for(unsigned int k=0;k<myPLRMeas->GetSignalSystematics()[j].size();k++){
                                                if(myPLRMeas->GetSignalSystematics()[j][k].channel == myPLRMeas->GetChannelsAddress()[c].GetChannelName() &&
                                                  SignalGlobalSyst[i].first == myPLRMeas->GetSignalSystematics()[j][k].name ){
                                                        //cout<<"eff "<<myPLRMeas->GetSignalSystematics()[j][k].effp1s<<endl;
                                                        //cout<<"PAR "<<SignalGlobalSyst[i].second<<endl;
                                                        //find the corresponding parameter
                                                        //if(myPLRMeas->GetParameter(SignalGlobalSyst[i].first).isZeroCentered) mean = 0.;
                                                        //This is false, take directly the value of the parameter !!
                                                        //float mean = 1.;
                                                        //float impact = myPLRMeas->Interpol(SignalGlobalSyst[i].second, mean, myPLRMeas->GetSignalSystematics()[j][k].effm1s, myPLRMeas->GetSignalSystematics()[j][k].effp1s);
                                                        SigGlobFactor*=SignalGlobalSyst[i].second;
                                                        //if(debug) cout<<"In channel "<<myPLRMeas->GetChannelsAddress()[c].GetChannelName()<<", syst: "<<SignalGlobalSyst[i].first<<" par = "<<SignalGlobalSyst[i].second<<" impact = "<<impact<<endl; 
                                                        if(debug) cout<<"In channel "<<myPLRMeas->GetChannelsAddress()[c].GetChannelName()<<", syst: "<<SignalGlobalSyst[i].first<<" par = "<<SignalGlobalSyst[i].second<<endl; 
                                                } 
                                        }
                                }
                        }
                        //include the SigGlobFactor in SignalFactor
                        SignalFactor*=SigGlobFactor;
                        ///////////////////////////////////////////////////////
                        ////
                //loop over bins
                int nofdim = myPLRMeas->GetChannelsAddress()[c].GetHistosAddress()[h].GetNofDimensions();
                if(nofdim == 1 ){
                        TH1F* hData = 0;
                        if(!myPLRMeas->GetDoPseudoExp()) hData = myPLRMeas->GetChannelsAddress()[c].GetHistosAddress()[h].Get1DNobs();
                        else{
                                hData = myPLRMeas->GetChannelsAddress()[c].GetHistosAddress()[h].Get1DPseudoData();
                        }               
                        //to be changed !!!
                        //cout<<"factor- "<<uncert[0]<<endl;    
                        TH1F* hExp = myPLRMeas->GetChannelsAddress()[c].GetHistosAddress()[h].Get1DNexp(uncert, GraphUncert, norm, LumiFactor, SignalFactor,myPLRMeas->GetSignalDatasetIteratorForHisto());
                        for(int b=1; b<hData->GetNbinsX()+1;b++){
                                double nobs = hData->GetBinContent(b);
                                double nexp = hExp->GetBinContent(b);
                                //if(nexp>0) LnLikelihood+= nobs*TMath::Log(nexp)-nexp;
                                if(nexp>0) LnLikelihood+= TMath::Log(TMath::Poisson(nobs,nexp));
                                else{ 
                                        if(nexp<0) cerr<<"Number of events expected lower than zero"<<endl;
                                        else if(debug) cerr<<"Number of events expected is null"<<endl;
                                }
                                Nobs_tot+=nobs;
                                Nexp_tot+=nexp;
                        }
                        hData = 0;
                        hExp = 0;
                        delete hData;
                        delete hExp;
                }
                //cout<<"there "<<nofdim<<" debug "<<debug<<" "<<endl;
                if(nofdim == 2 ){
                        TH2F* hData = 0;
                        if(!myPLRMeas->GetDoPseudoExp()) hData = myPLRMeas->GetChannelsAddress()[c].GetHistosAddress()[h].Get2DNobs();
                        else{
                        hData = myPLRMeas->GetChannelsAddress()[c].GetHistosAddress()[h].Get2DPseudoData();
                        }                       
                        TH2F* hExp = myPLRMeas->GetChannelsAddress()[c].GetHistosAddress()[h].Get2DNexp(uncert, GraphUncert, estim, GraphEstim, norm, LumiFactor, SignalFactor,myPLRMeas->GetSignalDatasetIteratorForHisto());
                        
                        //cout<<hData->GetNbinsX()<<" "<<hData->GetNbinsX()<<endl;      
                        for(int xb=1; xb<hData->GetNbinsX()+1;xb++){
                                for(int yb=1; yb<hData->GetNbinsX()+1;yb++){
                                        double nobs = hData->GetBinContent(xb,yb);
                                        double nexp = hExp->GetBinContent(xb,yb);
                                        if(debug) cout<<"bin ("<<xb<<","<<yb<<") - nobs: "<<nobs<<" nexp: "<<nexp<<endl;
                                        //cout<<"ERIC "<<nexp<<" "<<hExp->GetEntries()<<endl;
                                        //if(nexp>0) LnLikelihood+= nobs*TMath::Log(nexp)-nexp;
                                        if(nexp>0) LnLikelihood+= TMath::Log(TMath::Poisson(nobs,nexp));
                                        else{ 
                                                if(nexp<0) cerr<<"Number of events expected lower than zero"<<endl;
                                                else if(debug) cerr<<"Number of events expected is null"<<endl;
                                        }
                                        Nobs_tot+=nobs;
                                        Nexp_tot+=nexp;
                                }
                        }
                        hData = 0;
                        hExp = 0;
                        delete hData;
                        delete hExp;
                }       
        }
    }
    if(debug)  cout<<"Comparison: Nobs = "<<Nobs_tot<<" & Nexp = "<<Nexp_tot<<endl;

    for (unsigned int i = 0; i< myPLRMeas->GetParametersAddress().size(); i++) {
      if(myPLRMeas->GetParametersAddress()[i].isANuisanceParameter && !myPLRMeas->GetParametersAddress()[i].fixed){
                //if(myPLRMeas->GetParametersAddress()[i].isZeroCentered) LnLikelihood -= 0.5 * (par[i] - 0.) * (par[i] - 0.) / (1. * 1.);
                //else LnLikelihood -= 0.5 * pow((par[i] - myPLRMeas->GetParametersAddress()[i].ExpectedValue),2.) / (pow((myPLRMeas->GetParametersAddress()[i].uncertainty.first+myPLRMeas->GetParametersAddress()[i].uncertainty.second)/2.,2.));
                if(!myPLRMeas->GetParametersAddress()[i].isGlobal){ LnLikelihood -= 0.5 * pow((par[i] - myPLRMeas->GetParametersAddress()[i].ExpectedValue),2.) / (pow((myPLRMeas->GetParametersAddress()[i].uncertainty.first+myPLRMeas->GetParametersAddress()[i].uncertainty.second)/2.,2.));
                        if(debug) cout<<"II "<<myPLRMeas->GetParametersAddress()[i].isZeroCentered<<" "<<par[i]<<" "<<myPLRMeas->GetParametersAddress()[i].ExpectedValue<<" "<<myPLRMeas->GetParametersAddress()[i].uncertainty.first<<endl;
                }
                else{
                        //Add the Signal Systematics
                         //take care if they are common or not !!! 
                        for(unsigned int j=0;j<myPLRMeas->GetSignalSystematics().size();j++){
                                for(unsigned int k=0;k<myPLRMeas->GetSignalSystematics()[j].size();k++){
                                        if(myPLRMeas->GetSignalSystematics()[j][k].name == myPLRMeas->GetParametersAddress()[i].name){
                                                if(k==0){
                                                LnLikelihood -= 0.5 * pow((par[i] - 1.0),2.) / (pow((myPLRMeas->GetSignalSystematics()[j][k].effm1s+myPLRMeas->GetSignalSystematics()[j][k].effp1s)/2.,2.));
                                                if(debug) cout<<"II "<<myPLRMeas->GetParametersAddress()[i].isZeroCentered<<" "<<par[i]<<" "<<myPLRMeas->GetParametersAddress()[i].ExpectedValue<<" "<<myPLRMeas->GetSignalSystematics()[j][k].effm1s<<endl;
                                                }
                                        }
                                }
        
                         }
                }
        }
    }
    if (debug) cout << endl;

  LnLikelihood *= -1.;
  f = LnLikelihood;
  if (debug) {
    cout << " LnLikelihood = " << f << endl;
    cout << "##################################" << endl;

  }

 //update animation
 //if(myPLRMeas->GetAnimateGraph().doIt && myPLRMeas->GetAnimateGraph().iteration<100){ 
 if(myPLRMeas->GetDoAnimateGraph() && myPLRMeas->GetAnimateGraph().doIt){ 
        myPLRMeas->UpdateAnimateGraph(par, LnLikelihood);
 }
}

double PLRMeasurement::GetSignalXsection() const{
        for(unsigned int i=0;i<datasets.size();i++){
                if(datasets[i].Name()==SignalDatasetName) return datasets[i].Xsection();
        }
        return 1.;
}

vector < double >PLRMeasurement::GetMCXsection () const
{
  vector < double >out;
  for (unsigned int i = 0; i < datasets.size (); i++) {
    out.push_back (GetMCXsection (i));
  }
  return out;
}

double PLRMeasurement::GetMCXsection (int idataset) const
{
  if (idataset >= 0 && idataset < (int) datasets.size ()) {
    return Xsection.GetParameters (idataset)[0];
  }
  cerr << "PLRMeasurement::GetMCXsection:  idataset is a wrong paramter" << endl;
  return -999.;
}



void PLRMeasurement::EfficienciesTable (ofstream & fout)
{
  fout << "\\begin{table}" << endl;
  fout << "\\centering" << endl;
  fout << "\\begin{tabular}{|c|";
  for (unsigned int d = 0; d < datasets.size (); d++)
    fout << "c";
  fout << "|}" << endl;
  fout << "\\hline" << endl;
  fout << "Uncertainties & ";
  for (unsigned int d = 0; d < datasets.size (); d++) {
    fout << datasets[d].Name ();
    if (d < datasets.size () - 1)
      fout << " & ";
    else
      fout << "\\\\" << endl;
  }
  for (unsigned int i = 0; i < uncertainties.size (); i++) {
    fout << uncertainties[i].GetName () << " & ";
    for (unsigned int d = 0; d < datasets.size (); d++) {
      fout << uncertainties[i].GetEfficiencies (d)[1] << " / " << uncertainties[i].GetEfficiencies (d)[2] << endl;
      if (d < datasets.size () - 1)
        fout << " & ";
      else
        fout << "\\\\" << endl;
    }
  }
  fout << "\\hline" << endl;
  fout << "\\end{tabular}" << endl;
  //to be changed
  //fout << "\\caption{ Ratio table for channel " << channel << " }" << endl;
  //fout << "\\label{tab:RatioTable" << channel << "}" << endl;
  fout << "\\end{table}" << endl;
}

/*
double PLRMeasurement::IsolateSystError (double statError, double combinedError) const
{
  return (sqrt (combinedError * combinedError - statError * statError));
}
*/

double PLRMeasurement::Pourcentage(const double& num, const double& denom)const {
        if(denom>0) return (num/denom*100);
        return (-999.);
}

void PLRMeasurement::UncertaintiesTable (ofstream & fout, bool SkipOneByOne)
{
  //to be rewritten
  fout << "\\begin{table}" << endl;
  fout << "\\centering" << endl;
  fout << "\\begin{tabular}{|c|c|c|}";
  fout << "Parameters & Stat only & With syst \\";
  fout << "\\hline" << endl;
  for(unsigned int i=0;i<parameters.size();i++){
        if(parameters[i].isAnEstimator){
        fout<<parameters[i].name<<" & ";
        fout<<parameters[i].estimation_stat<<" $pm_{"<<parameters[i].StatError.first<<"}^{"<<parameters[i].StatError.second<<"}$ ";
        fout<<"($pm_{"<<Pourcentage(parameters[i].StatError.first,parameters[i].estimation_stat)<<"\\%}^{"<<Pourcentage(parameters[i].StatError.second,parameters[i].estimation_stat)<<"\\%}$) &";
        fout<<parameters[i].estimation_syst<<" ($pm_{"<<parameters[i].SystError.first<<"}^{"<<parameters[i].SystError.second<<"}$) ";
        fout<<"($pm_{"<<Pourcentage(parameters[i].SystError.first,parameters[i].estimation_syst)<<"\\%}^{"<<Pourcentage(parameters[i].SystError.second,parameters[i].estimation_syst)<<"\\%}$) \\\\";

/*
   if (!SkipOneByOne) {
    for (unsigned int i = 0; i < SysName.size (); i++) {
      fout << SysName[i] << " & ";
      fout << XsectionEstimSysError[i].first << " (" << XsectionEstimSysError[i].first / XsectionEstim * 100. << "\\%) & " << XsectionEstimSysError[i].second << " (" << XsectionEstimSysError[i].second / XsectionEstim * 100. << "\\%)\\\\" << endl;
      //fout<<IsolateSystError(XsectionEstimStatError.first,XsectionEstimSysError[i].first)<<" ("<<IsolateSystError(XsectionEstimStatError.first,XsectionEstimSysError[i].first)/XsectionEstim*100.<<"\\%) & "<<IsolateSystError(XsectionEstimStatError.second,XsectionEstimSysError[i].second)<<" ("<<IsolateSystError(XsectionEstimStatError.second,XsectionEstimSysError[i].second)/XsectionEstim*100.<<"\\%)\\\\"<<endl;
    }
    }
  }
  */
   fout << "\\hline" << endl;
   }
  }
  fout << "\\end{tabular}" << endl;
  // to be changed
  //fout << "\\caption{ Uncertainties table for channel " << channel << "}" << endl;
  //fout << "\\label{tab:UncerTable" << channel << "}" << endl;
  fout << "\\end{table}" << endl;
}

void PLRMeasurement::MinimizationTable (ofstream & fout)
{
  fout << "\\begin{table}" << endl;
  fout << "\\centering" << endl;
  //fout << "\\begin{tabular}{|c|c|c|c|c|}";
  fout << "\\begin{tabular}{|c||c|c||c|c|}";
  fout << "\\hline" << endl;
  //fout << "Parameter & Value & Error & NegErr & PosErr \\\\" << endl;
  fout << "Parameter & Value (Lik) & Error (Lik) & Value (PLR) & Error (PLR) \\\\" << endl;
  fout << "\\hline" << endl;
  for (unsigned int i = 0; i < parameters.size (); i++) {
        if(parameters[i].isANuisanceParameter){
    fout << parameters[i].name << " & " << parameters[i].estimation_minuit << " & " << parameters[i].Error_minuit ;
    fout <<" & " << parameters[i].estimation_syst << " & " << parameters[i].SystError.first<< "\\\\" << endl;
        }
        if(parameters[i].isAnEstimator){
    fout << parameters[i].name << " & " << parameters[i].estimation_minuit << " & " << parameters[i].Error_minuit <<";["<<  parameters[i].uncertainty_lik.first << "," << parameters[i].uncertainty_lik.second << "]";
    fout <<" & " << parameters[i].estimation_syst << " & " << parameters[i].SystError.first<< "\\\\" << endl;
        }
  }
  fout << "\\hline" << endl;
  fout << "\\end{tabular}" << endl;
  //to be changed
  //fout << "\\caption{ Minimization table for channel " << channel << " }" << endl;
  //fout << "\\label{tab:MinTable" << channel << "}" << endl;
  fout << "\\end{table}" << endl;
}

void PLRMeasurement::NumberOfEventsTable (ofstream & fout)
{
  fout << "\\begin{table}" << endl;
  fout << "\\centering" << endl;
  fout << "\\begin{tabular}{|c|c|c|c|}";
  fout << "\\hline" << endl;
  fout << "Channel & Contribution & MC Expectation & PLR Estimation  \\\\" << endl;
  fout << "\\hline" << endl;
  fout << "\\hline" << endl;


  for(unsigned int i=0;i<channels.size();i++){
        string name = channels[i].GetChannelName();
        double MCExpectationSum = 0;
        double PLREstimationSum = 0;
        if(channels[i].GetHistos().size()>0  && channels[i].GetHistos()[0].GetNofDimensions()==1){ 
        if(channels[i].GetHistos()[0].Get1DProcessExp().size()==channels[i].GetHistos()[0].Get1DProcessFinal().size()){
                for(unsigned int k=0;k<channels[i].GetHistos()[0].Get1DProcessExp().size();k++){
                        double MCExpectation = 0;
                        double PLREstimation = 0;
                        MCExpectation = channels[i].GetHistos()[0].Get1DProcessExp()[k]->Integral();
                        MCExpectationSum+=MCExpectation;
                        PLREstimation = channels[i].GetHistos()[0].Get1DProcessFinal()[k]->Integral();
                        cout<<"INTEG: "<<channels[i].GetHistos()[0].Get1DProcessFinal()[k]->Integral()<<endl;
                        PLREstimationSum+=PLREstimation;
                        string process;
                        unsigned int it=0;
                        for(unsigned int j=0;j<datasets.size();j++){
                                //if(!datasets[j].isData() && GetDatasets()[j].Name()!=SignalDatasetName){
                                if(!datasets[j].isData()){
                                        if(it==k){
                                                process = datasets[j].Name();
                                                break;
                                        }
                                        it++;
                                }
                        }
                        fout<<name<<" & "<< process<<" & "<<MCExpectation<<" & "<<PLREstimation<<"\\\\"<<endl;
                }
        }
        //Sum of MC
        fout << "\\hline" << endl;
        fout<<name<<" & "<< "Sum" <<" & "<<MCExpectationSum<<" & "<<PLREstimationSum<<"\\\\"<<endl;
        //Data
        fout << "\\hline" << endl;
        fout<<name<<" & "<< "Data"<<" & "<< channels[i].GetHistos()[0].Get1DNobs()->Integral()<<" & "<<channels[i].GetHistos()[0].Get1DNobs()->Integral()<<"\\\\"<<endl;
        fout << "\\hline" << endl;
        fout << "\\hline" << endl;
  
  }
  }
  fout << "\\end{tabular}" << endl;
  fout << "\\caption{ NofEvts in PLR  }" << endl;
  fout << "\\label{tab:NofEvtsPLRTable}" << endl;
  fout << "\\end{table}" << endl;

}



bool PLRMeasurement::CheckUncertainty (int iUncertainty) const
{
  if (iUncertainty >= 0 && iUncertainty < (int) uncertainties.size ())
    return true;
  return false;
}

void PLRMeasurement::RunLikelihood (bool doMCPseudoExp, int NofPseudoExp, bool SmearSystInPseudoExp, int verbosity, bool SkipOneByOne)
{

  //the following line is extremely important.
  //this allow a correct behaviour of the code event is many objects PLRMeasurement are instanciated.
  myPLRMeas = this;

  cout<<"blabla"<<endl;
  if(doAnim) InitAnimateGraph(750,string("anim.gif"));

  bool verbose = false;
  bool verboseMax = false;
  if (verbosity > 0){
    verbose = true;
    verboseMax = true;
  }
  //verboseMax = true;          //comment
  if (verbose) {
    cout << "####################################" << endl;
    cout << "   Run the Likelihood               " << endl;
    cout << "####################################" << endl;
  }

  MinFCN = 0.;
  double MinlogL0 = 0.;





  // Give main input parameters
  bool PlotLogCurve = true;     // to plot loglikelihood curves
  bool doCorrelationMatrixHisto = true;
  cout<<"doCorrelationMatrixHisto = "<<doCorrelationMatrixHisto<<endl;
  //TRandom rand;
  doMCPSExp = doMCPseudoExp;
  if (doMCPseudoExp){
    PlotLogCurve = false;
    doCorrelationMatrixHisto = false;
    SkipOneByOne = true;
    for(unsigned int i=0;i<GetEstimators().size();i++){
        cout<<"TOTOTO "<<GetEstimators()[i].range.first<<" "<<GetEstimators()[i].range.second<<" "<<GetEstimators()[i].ErrRange.first<<" "<<GetEstimators()[i].ErrRange.second<<endl;
        plots.push_back(PLRPlots(GetEstimators()[i].name, GetEstimators()[i].ExpectedValue, GetEstimators()[i].nbins, GetEstimators()[i].range.first, GetEstimators()[i].range.second, GetEstimators()[i].ErrRange.first, GetEstimators()[i].ErrRange.second, GetNuisanceParameters().size())); 
        //plots.push_back(PLRPlots(GetEstimators()[i].name, GetEstimators()[i].ExpectedValue, (int)(GetEstimators()[i].range.second-GetEstimators()[i].range.first), GetEstimators()[i].range.first, GetEstimators()[i].range.second, GetEstimators()[i].ExpectedValue*0.1, GetEstimators()[i].ExpectedValue*0.5)); 
        //plots.push_back(PLRPlots(GetEstimators()[i].name, GetEstimators()[i].ExpectedValue, 100, GetEstimators()[i].range.first, GetEstimators()[i].range.second, 0., 0.2)); 
    }
  }

  //Write the table of expected events from MC
  //NumberOfExpectedEvents (true);

  ///////////////////
  // Initialisation
  ///////////////////
  vector < bool > fixParam;
  vector < double >ParamValues;
  for(unsigned i=0;i<parameters.size();i++){
        //Fill the vector ParamValues
        if(parameters[i].isANuisanceParameter){
                if(parameters[i].isZeroCentered) ParamValues.push_back(0.);//normally redondant( ie: ZeroCentered <=> ExpectedValue == 0 )
                else ParamValues.push_back(parameters[i].ExpectedValue);
        }
        else ParamValues.push_back(parameters[i].ExpectedValue);
        //Fill the vector fixParam
        //don't forget to update .fixed
        if(parameters[i].isAnEstimator){
                fixParam.push_back(false); //all estimators are free in first stage
                parameters[i].fixed = false;
        }
        else {
                fixParam.push_back(true);
                parameters[i].fixed = true;
        }
  }
  ///////////////////

  //    Calculation of the Normalization of the initial histos
   normInit.clear();

    for(unsigned int c=0; c<myPLRMeas->GetChannelsAddress().size();c++){
        //loop over histos
        vector<float> normInitChannel;
        for(unsigned int h=0;h<myPLRMeas->GetChannelsAddress()[c].GetHistosAddress().size();h++){
                        //---------------------------------------------------------------
                        //  Systematics of the normalization (level of backgrounds)
                        //---------------------------------------------------------------
                        for(unsigned int id=0;id<myPLRMeas->GetDatasets().size();id++){
                                int iu = -1;
                                if(!myPLRMeas->GetDatasets()[id].isData() && myPLRMeas->GetDatasets()[id].Name()==myPLRMeas->GetSignalDatasetName()){
                                        for(unsigned int u=0;u<myPLRMeas->GetParametersAddress().size();u++){
                                                if(myPLRMeas->GetParametersAddress()[u].name == string("SignalXsection")){
                                                        //if(myPLRMeas->GetChannelsAddress()[c].GetHistosAddress()[h].Get1DProcessExp().size()!=0) normInitChannel.push_back(myPLRMeas->GetParametersAddress()[u].ExpectedValue*myPLRMeas->GetChannelsAddress()[c].GetHistosAddress()[h].Get1DProcessExp()[id]->Integral());
                                                        //if(myPLRMeas->GetChannelsAddress()[c].GetHistosAddress()[h].Get2DProcessExp().size()!=0) normInitChannel.push_back(myPLRMeas->GetParametersAddress()[u].ExpectedValue*myPLRMeas->GetChannelsAddress()[c].GetHistosAddress()[h].Get2DProcessExp()[id]->Integral());
                                                        normInitChannel.push_back(-1.);
                                                        break;
                                                }
                                        }
                                }
                                if(!myPLRMeas->GetDatasets()[id].isData() && myPLRMeas->GetDatasets()[id].Name()!=myPLRMeas->GetSignalDatasetName()){
                                        for(unsigned int u=0;u<myPLRMeas->GetParametersAddress().size();u++){
                                                if(myPLRMeas->GetParametersAddress()[u].name.find(myPLRMeas->GetDatasets()[id].Name()+string("-")+myPLRMeas->GetChannelsAddress()[c].GetChannelName())<myPLRMeas->GetParametersAddress()[u].name.size()){
                                                        if(myPLRMeas->GetParametersAddress()[u].isCommon){
                                                                iu = u;
                                                                break;
                                                        }
                                                        else{
                                                                if(myPLRMeas->GetParametersAddress()[u].name.find(myPLRMeas->GetChannelsAddress()[c].GetChannelName())<myPLRMeas->GetParametersAddress()[u].name.size()){
                                                                        iu = u;
                                                                        break;
                                                                }
                                                        }
                                                }
                                        }
                                        if(iu>0) normInitChannel.push_back(myPLRMeas->GetParametersAddress()[iu].ExpectedValue);
                                        else normInitChannel.push_back(1.);
                                }
                        
                        }
        }
        normInit.push_back(normInitChannel);
   }
    
  //////////////////

  if (!doMCPseudoExp)
    NofPseudoExp = 1;

  cout<<"doCorrelationMatrixHisto = "<<doCorrelationMatrixHisto<<endl;
  double MinlogL = MinFCN;
  for (int k = 0; k < NofPseudoExp; ++k) {
    //Randomisation of the nof observed event if (MC-pseudo)
    //randomisation of the systematics ...
    int count = 0;
    for(unsigned int i=0;i<parameters.size();i++){
        if(parameters[i].isANuisanceParameter){
                /* 
                if(parameters[i].isZeroCentered){
                         double v = rand.Gaus(0.,1.);
                        cout<<"v = "<<v<<endl;
                         if(doMCPseudoExp && NofPseudoExp==1) parameters[i].PEValue = 0.;
                         else parameters[i].PEValue = v;
                         //plots[0].FillUncertInput(count,parameters[i].PEValue);
                        //count++;
                }
                else{
                */
                        double v = rand.Gaus(parameters[i].ExpectedValue, (parameters[i].uncertainty.first+parameters[i].uncertainty.second)/2.);
                        cout<<parameters[i].ExpectedValue<<" "<<(parameters[i].uncertainty.first+parameters[i].uncertainty.second)/2.<<endl;
                        cout<<"v2 = "<<v<<endl;
                        if(doMCPseudoExp && NofPseudoExp==1) parameters[i].PEValue = parameters[i].ExpectedValue;
                        //else parameters[i].PEValue = v;
                        else parameters[i].PEValue = parameters[i].ExpectedValue;
                        //plots[0].FillUncertInput(count,parameters[i].PEValue);
                        //count++;
                //}
        }
        else{
                parameters[i].PEValue = parameters[i].ExpectedValue;
        }
        cout<<"Values of parameters in the pseudo-experiments "<<k<<endl;
        cout<<" - "<<parameters[i].name << " : "<<parameters[i].PEValue <<endl;
    } 
    if (doMCPseudoExp) {
        ParamValues.clear();
        fixParam.clear();
        for(unsigned i=0;i<parameters.size();i++){
                //Fill the vector ParamValues
                ParamValues.push_back(parameters[i].PEValue);
                //Fill the vector fixParam
                //don't forget to update .fixed
                if(parameters[i].isAnEstimator){
                        fixParam.push_back(false); //all estimators are free in first stage
                        parameters[i].fixed = false;
                }
                else {
                                fixParam.push_back(true);
                                parameters[i].fixed = true;
                }
        }
        /////////////////////////////////////////////////////
        //---- Compute the distributions for Pseudo-Data ---//
        /////////////////////////////////////////////////////
        for(unsigned int c=0; c<GetChannelsAddress().size();c++){
                for(unsigned int h=0;h<GetChannelsAddress()[c].GetHistosAddress().size();h++){
                //loop over bins
                  int nofdim = GetChannelsAddress()[c].GetHistosAddress()[h].GetNofDimensions();
                cout<<"nofdim = "<<nofdim<<endl;
                        double SignalXsectionPE = 0;
                        double LumiPE = 0;
                        vector<double> uncert_PE;
                        vector<double> GraphUncert_PE;
                        vector<double> estim_PE;
                        vector<double> GraphEstim_PE;
                        vector<pair<string,double> > SignalGlobalSyst_PE;
                        for(unsigned int u=0;u<GetParametersAddress().size();u++){
                                if(GetParametersAddress()[u].name!=string("Lumi") && GetParametersAddress()[u].name!=string("SignalXsection") && !GetParametersAddress()[u].isNormalization && !GetParametersAddress()[u].isGlobal){
                                                 if(GetParametersAddress()[u].isANuisanceParameter){
                                                         if(!GetParametersAddress()[u].WithGraph) uncert_PE.push_back(GetParametersAddress()[u].PEValue);//smearing as been done before
                                                         else GraphUncert_PE.push_back(GetParametersAddress()[u].PEValue);//smearing as been done before
                                                 }
                                                 else{
                                                         if(!GetParametersAddress()[u].WithGraph) estim_PE.push_back(GetParametersAddress()[u].PEValue);//smearing as been done before
                                                         else GraphEstim_PE.push_back(GetParametersAddress()[u].PEValue);//smearing as been done before
                                                 }
                                }
                                if(GetParametersAddress()[u].name==string("SignalXsection")) SignalXsectionPE = GetParametersAddress()[u].PEValue;
                                if(GetParametersAddress()[u].name==string("Lumi")) LumiPE = GetParametersAddress()[u].PEValue;
                                if(GetParametersAddress()[u].isGlobal) SignalGlobalSyst_PE.push_back(pair<string, double>(GetParametersAddress()[u].name,GetParametersAddress()[u].PEValue));
                        }
                        vector<double> normPE;
                        for(unsigned int id=0;id<GetDatasets().size();id++){
                                int iu = -1;
                                //if(!GetDatasets()[id].isData()){
                                if(!GetDatasets()[id].isData() && GetDatasets()[id].Name()!=SignalDatasetName){
                                        for(unsigned int u=0;u<GetParametersAddress().size();u++){
                                          if(myPLRMeas->GetParametersAddress()[u].name.find(myPLRMeas->GetDatasets()[id].Name())<myPLRMeas->GetParametersAddress()[u].name.size()){
                                                if(myPLRMeas->GetParametersAddress()[u].isCommon){
                                                        iu = u;
                                                        break;
                                                }
                                                else{
                                                        if(myPLRMeas->GetParametersAddress()[u].name.find(myPLRMeas->GetChannelsAddress()[c].GetChannelName())<myPLRMeas->GetParametersAddress()[u].name.size()){
                                                                iu = u;
                                                                break;
                                                        }
                                                }
                                          }
                                        }
                                        if(iu>0) normPE.push_back(GetParametersAddress()[iu].PEValue);
                                        else normPE.push_back(-1);
                                }
                        }
                        double LumiFactorPE = LumiPE/GetLumi();
                        //LumiFactorPE = 10.;
                        /// Compute the signal global systematic effect
                        double SigGlobFactorPE = 1.;
                        for(unsigned int i=0;i<SignalGlobalSyst_PE.size();i++){
                                for(unsigned int j=0;j<GetSignalSystematics().size();j++){
                                        for(unsigned int k=0;k<GetSignalSystematics()[j].size();k++){
                                                if(GetSignalSystematics()[j][k].channel == GetChannelsAddress()[c].GetChannelName() &&
                                                  SignalGlobalSyst_PE[i].first == GetSignalSystematics()[j][k].name ){
                                                        SigGlobFactorPE*=Interpol(SignalGlobalSyst_PE[i].second, 1, GetSignalSystematics()[j][k].effm1s, GetSignalSystematics()[j][k].effp1s);
                                                        /*
                                                        if(SignalGlobalSyst_PE[i].second>0) SigGlobFactorPE*=(1+SignalGlobalSyst_PE[i].second*(GetSignalSystematics()[j][k].effp1s-1));
                                                        else SigGlobFactorPE*=(1+SignalGlobalSyst_PE[i].second*(1-GetSignalSystematics()[j][k].effm1s));
                                                        */
                                                        cout<<"SigGlob "<<SigGlobFactorPE<<endl;        
                                                } 
                                        }
                                }
                        }
                        //include the SigGlobFactor in SignalFactor
                        double SignalFactorPE=(SignalXsectionPE/GetSignalXsection())*SigGlobFactorPE;
                        if(nofdim == 1 ){
                        //to be changed ...
                        GetChannelsAddress()[c].GetHistosAddress()[h].Compute1DPseudoData(GetRandom(), uncert_PE, GraphUncert_PE, normPE, LumiFactorPE, SignalFactorPE,GetSignalDatasetIteratorForHisto());
                        }
                        if(nofdim == 2){
                                GetChannelsAddress()[c].GetHistosAddress()[h].Compute2DPseudoData(GetRandom(), uncert_PE, GraphUncert_PE, estim_PE, GraphEstim_PE, normPE, LumiFactorPE, SignalFactorPE,GetSignalDatasetIteratorForHisto());
                        }
                  }     
                }               
                /////////////////////////////////////////////////////
    }

    if (verbose) {
      std::cout << "======= First step: Minimization of LogL, Sys OFF, parameters are free =======" << std::endl;
    }
    ComputeLikelihood (fixParam, ParamValues, true, false, verboseMax);
    //to be changed: as to be done somewhere else
    //assign values for the estimators
    /*
     PLREstim.clear();
    for(unsigned int p=0;p<parameters.size();p++){
        if(parameters[p].isAnEstimator) PLREstim.push_back(Param[p].ExpectedValue);
    }
    */
    MinlogL0 = MinFCN;
    if (verbose) {
      for (unsigned int j = 0; j < parameters.size(); ++j) {
        std::cout << "Param[" << j << "] = " << Param[j] << " +/-" << ErrParam[j] << " [parabolic] "<< ErrParaParam[j]<<" (" << ErrNegParam[j] << "," << ErrPosParam[j] << ")" << std::endl;
      }
      std::cout << "MinlogL " << MinlogL << std::endl;
    }
    //Update the estimations
    for(unsigned int p=0;p<parameters.size();p++){
        if(parameters[p].isAnEstimator){
        parameters[p].estimation_minuit = Param[p];
        parameters[p].Error_minuit = ErrParam[p];
        parameters[p].uncertainty_lik = pair<double,double> (ErrNegParam[p],ErrPosParam[p]);
        }
    }
    //to be changed. Plots should be done elsewhere ...
    //Plotting of the values coming from the minimisation
    /*if (PlotLogCurve) {
      Plot ("AP", NofPoints, MinXforLogLPlot, MaxXforLogLPlot, MinlogL0);
    }
    */
     //PLRExamination (NofPoints, X_PLR, Y_PLR, -1);    //-1 means it's purely statistic
    //Xseccentralvalue_wosys->Fill (Param[0]);
        /*
    Xseccentralvalue_wosys->Fill (XsectionEstimStat);
    Erreurneg_wosys->Fill (XsectionEstimStatError.first);
    Erreurpos_wosys->Fill (XsectionEstimStatError.second);
    //cout<<"Stat error: "<<XsectionEstimStatError.first<<" "<<XsectionEstimStatError.second<<endl;
    Erreurmean_wosys->Fill ((XsectionEstimStatError.first + XsectionEstimStatError.second) / 2.);
    PullErreurneg_wosys->Fill ((XsectionEstimStat - ExpectedXsection) / XsectionEstimStatError.first);
    PullErreurpos_wosys->Fill ((XsectionEstimStat - ExpectedXsection) / XsectionEstimStatError.second);
    PullErreurmean_wosys->Fill ((XsectionEstimStat - ExpectedXsection) / ((XsectionEstimStatError.first + XsectionEstimStatError.second) / 2.));
    if(XsectionEstimStat<ExpectedXsection) PullErreur2cases_wosys->Fill((XsectionEstimStat - ExpectedXsection) / XsectionEstimStatError.first);
    else PullErreur2cases_wosys->Fill((XsectionEstimStat - ExpectedXsection) / XsectionEstimStatError.second);
        */
    if (verbose){
      std::cout << "====================================================================================" << std::endl;
      std::cout << "======= Second step: Minimization of LogL, Sys ON, all free parameters =======" << std::endl;
    }
    for (unsigned int i = 0; i < fixParam.size(); i++) {
        //don't forget to update .fixed
        if(parameters[i].isAnEstimator || parameters[i].isANuisanceParameter){ //if it is not a "constant"
                fixParam[i] = false;
                parameters[i].fixed = false;
        }
        else{ 
                fixParam[i] = true;
                parameters[i].fixed = true;
        }
    }
    //Start Animate
    StartAnimateGraph();
    cout<<"doCorrelationMatrixHisto = "<<doCorrelationMatrixHisto<<endl;
    ComputeLikelihood (fixParam, ParamValues, false, doCorrelationMatrixHisto, verboseMax);
    MinlogL0 = MinFCN;
    //--- update the value of the free parameters ---//
    for(unsigned int ifp=0;ifp<GetParametersAddress().size();ifp++){
        if(GetParametersAddress()[ifp].isAnEstimator){
                SetParameterValue(ifp,Param[ifp]);
        }
    }
    for (unsigned int j = 0; j < ParamValues.size(); ++j) {
      if (verbose)
        std::cout << "Param[" << j << "] = " << Param[j] << " +/-" << ErrParam[j] << " [parabolic] "<< ErrParaParam[j]<<" (" << ErrNegParam[j] << "," << ErrPosParam[j] << ")" << std::endl;
      //important: update the values according to the previous minimization where all paramters are free
      ParamValues[j] = Param[j];
    }
    if (verbose)
      std::cout << "MinlogL " << MinlogL << std::endl;
    
    //Update the estimations
    for(unsigned int p=0;p<parameters.size();p++){
        if(!parameters[p].isAnEstimator){
                parameters[p].estimation_minuit = Param[p];
                parameters[p].Error_minuit = ErrParam[p];
                parameters[p].uncertainty_lik = pair<double,double> (ErrNegParam[p],ErrPosParam[p]);
        }
    }
    if (verbose)
      std::cout << "====================================================================================" << std::endl;

    //Finish animate
    if(doAnim) WriteAnimateGraph();

    //save the ParamValues which minimize the likelihood when all paramters are free
    vector < double >ParamValuesMin;
    for (unsigned int i = 0; i < ParamValues.size (); i++)
      ParamValuesMin.push_back (ParamValues[i]);
  

   //--- recompute PLRScan from free parameters [according to Minimum & Errors obtained with the likelihood]---//
  plrScan.clear();
  int SizeOfPLRScan=1;
  for(unsigned int i=0;i<GetEstimators().size();i++){
          SizeOfPLRScan*=GetEstimators()[i].NofPoints;
  }
  
  vector<double> tmp_vec;
  for(unsigned int i=0;i<GetEstimators().size();i++) tmp_vec.push_back(0.0);
  for(int i=0;i<SizeOfPLRScan;i++){
        PLRScan plrs;
        plrs.LogLik = 0.;
        plrs.FreeParamValue = tmp_vec;
        plrScan.push_back(plrs); 
  } 

  for(unsigned int i=0;i<GetEstimators().size();i++){
        int size = 1;
        if(i<GetEstimators().size()-1)
                for(unsigned int s=1;s<GetEstimators().size();s++)
                        size*=GetEstimators()[s].NofPoints;
        int compl_size = SizeOfPLRScan/(size*GetEstimators()[i].NofPoints);
        int it=0;
        float ntimes = 5.;
        //just for debug
        double xmin = GetEstimators()[i].estimation_minuit-ntimes*GetEstimators()[i].Error_minuit;
        //double xmin = 0.01;
        double step = (double)((2*ntimes*GetEstimators()[i].Error_minuit)/GetEstimators()[i].NofPoints);
        if(xmin<0){
                xmin = 0;
                if(ntimes*GetEstimators()[i].Error_minuit>300){
                        step = (double)(300./GetEstimators()[i].NofPoints);
                }
                else{
                        step = (double)((ntimes*GetEstimators()[i].Error_minuit)/GetEstimators()[i].NofPoints);
                }
        }
        //explicitely change for btagging
        xmin = 0.85;
        step =(1.05-0.85)/GetEstimators()[i].NofPoints;
        //
        //double step = 0.05;
        for(int cj=0;cj<compl_size;cj++){
                for(int k=0;k<GetEstimators()[i].NofPoints;k++){
                  for(int j=0;j<size;j++){
                         plrScan[it].FreeParamValue[i] = xmin+k*step;
                         it++;
                  }
                }
        }
  }
  //-------------------------------//
   
    if (verbose){
      std::cout << "====================================================================================" << std::endl;
      std::cout << "======= Second' step: Statistical PLR (everything is fixed)  =======" << std::endl;
    }
    for(unsigned int ie = 0; ie<plrScan.size(); ie++){
        for(unsigned int ind=0;ind<GetEstimatorsIndice().size();ind++){
                ParamValues[GetEstimatorsIndice()[ind]] = plrScan[ie].FreeParamValue[ind];
                //the other values are equal to the minimum obtained in step two
        }
        double* p = new double[ParamValues.size()];
        for(unsigned int i=0;i<ParamValues.size();i++) p[i] = ParamValues[i];
        plrScan[ie].LogLik = EvaluateFCN(p)-MinlogL0;
        delete p; 
    }
  // Now search the minimium of the PLR
   if(GetEstimators().size()==1){
        int size = plrScan.size();
        double* Xval = new double[size];
        double* Yval = new double[size];
        for(int i=0;i<size;i++){
                if(plrScan[i].FreeParamValue.size()!=1) cerr<<"PLREstimation-1D: problem in the size of FreeParamValue"<<endl;
                else{
                        Xval[i] = plrScan[i].FreeParamValue[0];
                        Yval[i] = plrScan[i].LogLik-MinlogL;//diff to the min observed in Likelihood
                }
        }
        double Estimation = GetEstimatorsValue()[0];
        double ErrN = 0;
        double ErrP = 0;
        double Min = 0;
        StatGraph1D = new TGraph(size,Xval,Yval);
        //StatGraph1D->SetDirectory(0);
        StatGraph1D->SetName("StatGraph1D");
        //PLRValuesWithPol2Fit(size, Xval, Yval, Estimation, ErrN, ErrP, Min);
        //PLRValuesWithPol3Fit(size, Xval, Yval, Estimation, ErrN, ErrP, Min);
        PLRValuesWithPol3Fit(StatGraph1D, size, Xval, Estimation, ErrN, ErrP, Min);
        cout<<"Result of the PLR-1D minimization: "<<endl;
        cout<<"Estimation: "<<Estimation<<" - "<<ErrN<<" + "<< ErrP<< " ( Min: "<<Min<<" )"<<endl; 
        //Write the results
        for(unsigned int i=0;i<parameters.size();i++){
                if(parameters[i].isAnEstimator){
                         parameters[i].StatError = pair<double,double>(ErrN,ErrP);
                         parameters[i].estimation_stat = Min;
                }
        } 
 
        if(doMCPseudoExp){
                plots[0].Fill(Min, ErrN,ErrP, true); 
        }
   }
   else{
        if(GetEstimators().size()==2){
                int size = plrScan.size();
                double* Xval = new double[size];
                double* Yval = new double[size];
                double* Zval = new double[size];
                for(int i=0;i<size;i++){
                        if(plrScan[i].FreeParamValue.size()!=2) cerr<<"PLREstimation-1D: problem in the size of FreeParamValue"<<endl;
                        else{
                                Xval[i] = plrScan[i].FreeParamValue[0];
                                Yval[i] = plrScan[i].FreeParamValue[1];
                                Zval[i] = plrScan[i].LogLik-MinlogL0;//diff to the min observed in Likelihood
                }
                }
                StatGraph2D = new TGraph2D(size,Xval,Yval,Zval);
                StatGraph2D->SetName("StatGraph2D");
                //The following line is REQUIRED !!
                StatGraph2D->GetHistogram();
                //--- Search for the minimum ---//
                //Doing a fit (TF2) 
                //adapt the scale ..
                StatFit= new TF2("StatFit","[0]*x*x*x+[1]*y*y*y+[2]*x*x*y+[3]*x*y*y+[4]*x*x+[5]*y*y+[6]*x+[7]*y+[8]",0.9,1.3,100,240);
                double xmin = 0;
                double ymin = 0;
                StatGraph2D->Fit(StatFit,"R");
                StatFit->GetMinimumXY(xmin,ymin);
                //cout<<xmin<<" "<<ymin<<endl;
                double fmin = 0;
                fmin = StatFit->Eval(xmin,ymin);
                //change there ...
                /*
                TList* list = StatGraph2D->GetContourList(0.5+fmin);
                TGraph* contour = (TGraph*) list->At(0);
                int n = contour->GetN();
                Double_t* xcont = contour->GetX();
                Double_t* ycont = contour->GetY();
                double xminErr = TMath::MinElement(n,xcont);
                double xmaxErr = TMath::MaxElement(n,xcont);
                double yminErr = TMath::MinElement(n,ycont);
                double ymaxErr = TMath::MaxElement(n,ycont);
                //Change estimation
                cout<<"Result of the PLR-2D minimization: "<<endl;
                cout<<"Estimation: "<<xmin<<" - "<<xminErr<<" + "<< xmaxErr<< endl;
                cout<<"Estimation: "<<ymin<<" - "<<yminErr<<" + "<< ymaxErr<< endl;
                //----------------------------------            

                delete list;
                delete contour;
                
                //Write the results
                int it=0;
                for(unsigned int i=0;i<parameters.size();i++){
                        if(parameters[i].isAnEstimator){
                                 if(it==0){
                                         parameters[i].StatError = pair<double,double>(xminErr,xmaxErr);
                                         parameters[i].estimation_stat = xmin;
                                 }
                                 if(it==1){
                                         parameters[i].StatError = pair<double,double>(yminErr,ymaxErr);
                                         parameters[i].estimation_stat = ymin;
                                 }
                                if(it==0 || it==1) it++;
                        }
                } 
                // to be changed !
                if(doMCPseudoExp){
                        plots[0].Fill(xmin, xminErr,xmaxErr, true);     
                        plots[1].Fill(ymin, yminErr,ymaxErr, true);     
                        //plots[0].Fill(StatEstim,Min); 
                }
                */
        }
        else{
                cerr<<"We cannot treat the PLR for more than two free parameters for the moment !!"<<endl;
        }
    }
    if (verbose)
      std::cout << "====================================================================================" << std::endl;

    ////////////////////////////////////////////////
    ///   Scan of the Xsection: Profile Likelihood
    ////////////////////////////////////////////////                            
    //Compute the PLR for each uncertainty + stat only (u [0,SizeOfLoop-2]) + all uncertainties combined (u==SizeOfLoop-1)
    int SizeOfLoop = GetNuisanceParameters().size() + 2;   
 
    for (int u = 0; u < SizeOfLoop; u++) {
      if ( (SkipOneByOne == true) && u < (SizeOfLoop - 1))
        continue;
      
      //Loop over the scan of all free estimators
      for(unsigned int ie = 0; ie<plrScan.size(); ie++){
 
        if (verbose)
          std::cout << "======= Third step: Minimization of LogL, sigma fixed parameter =======" << std::endl;
        if (u < SizeOfLoop - 1) {
          for (int p = 0; p < SizeOfLoop; p++){
                //don't forget to update .fixed
                fixParam[p] = true;
                parameters[p].fixed = true;
           }
          fixParam[u + 1] = false;      //for u==0; statistic uncertainty only
          parameters[u+1].fixed = false;
        }
        else
          for (int p = 0; p < SizeOfLoop; p++){
            //don't forget to update .fixed
            fixParam[p] = false;        //all uncertainties considered
            parameters[p].fixed = false;
          }
        //--- The free parameters are fixed ---//
        for(unsigned int ind=0;ind<GetEstimatorsIndice().size();ind++){
                fixParam[GetEstimatorsIndice()[ind]] = true;
                ParamValues[GetEstimatorsIndice()[ind]] = GetEstimatorsValue()[ind];
        }
        //---------------------------//
        ComputeLikelihood (fixParam, ParamValues, false, false, verboseMax);
        //MinlogL0 = MinFCN;
        //cout << "MinLogL0: " << MinlogL0 << endl;
        if (verbose) {
          for (int j = 0; j < SizeOfLoop; j++) {
            std::cout << "Param[" << j << "] = " << Param[j] << " +/-" << ErrParam[j] << " [parabolic] "<< ErrParaParam[j]<<" (" << ErrNegParam[j] << "," << ErrPosParam[j] << ")" << std::endl;
          }
          std::cout << "MinlogL " << MinlogL << std::endl;
          std::cout << "=======================================================================" << std::endl;
          std::cout << "======= Fourth step: Computation of Profile Likelihood Ratio curve =======" << std::endl;
        }
        double P[SizeOfLoop];
        for (int j = 0; j < SizeOfLoop; j++) {
          P[j] = Param[j];
        }
        if(verbose){
        std::cout << "=============================================================================" << std::endl;
        }


      //--- With all systematics considered ---//
      if (u == SizeOfLoop - 1) {
        //--- REDO the PLR at minimum with all systematics ---//
        for (int p = 0; p < SizeOfLoop; p++){
          //don't forget to update .fixed
          fixParam[p] = false;  //all uncertainties considered
          parameters[p].fixed = false;
        }
        //--- The free parameters are fixed ---//
        for(unsigned int ind=0;ind<GetEstimatorsIndice().size();ind++){
                fixParam[GetEstimatorsIndice()[ind]] = true;
                ParamValues[GetEstimatorsIndice()[ind]] = plrScan[ie].FreeParamValue[ind];
        }
        //---------------------------//
        //to be changed
        ComputeLikelihood (fixParam, ParamValues, false, false, verboseMax);
        plrScan[ie].LogLik = MinFCN; 
        double *par = new double[ParamValues.size ()];
        for (unsigned int p = 0; p < ParamValues.size (); p++)
          par[p] = ParamValues[p];
  /////////////// Save the Parameters of the optimization
  for (int i = 0; i < SizeOfLoop; i++) {
    if(i==0){
        }
        else{
    ErrNegParam_Min[i] = ErrNegParam[i];
    ErrPosParam_Min[i] = ErrPosParam[i];
    ErrParaParam_Min[i] = ErrParaParam[i];
        }
    Param_Min[i] = Param[i];
    ErrParam_Min[i] = ErrParam[i];
    ErrParaParam_Min[i] = ErrParaParam[i];
  }
      }
    }
   if(verbose){
          std::cout << "=======================================================================" << std::endl;
          std::cout << "======= Fifth step: Computation of Profile Likelihood Ratio curve =======" << std::endl;
          std::cout << "=======================================================================" << std::endl;
   }
   if(GetEstimators().size()==1){
        int size = plrScan.size();
        double* Xval = new double[size];
        double* Yval = new double[size];
        for(int i=0;i<size;i++){
                if(plrScan[i].FreeParamValue.size()!=1) cerr<<"PLREstimation-1D: problem in the size of FreeParamValue"<<endl;
                else{
                        Xval[i] = plrScan[i].FreeParamValue[0];
                        Yval[i] = plrScan[i].LogLik-MinlogL0;//diff to the min observed in Likelihood
                        //Yval[i] = - plrScan[i].LogLik/MinlogL0;//diff to the min observed in Likelihood
                }
        }
        double Estimation = GetEstimatorsValue()[0];
        double ErrN = 0;
        double ErrP = 0;
        double Min = 0;
        SystGraph1D = new TGraph(size,Xval,Yval);
        //SystGraph1D->SetDirectory(0);
        SystGraph1D->SetName("SystGraph1D");
        //PLRValuesWithPol2Fit(size, Xval, Yval, Estimation, ErrN, ErrP, Min );
        //PLRValuesWithPol3Fit(size, Xval, Yval, Estimation, ErrN, ErrP, Min);
        PLRValuesWithPol3Fit(SystGraph1D, size, Xval, Estimation, ErrN, ErrP, Min);
        cout<<"Result of the PLR-1D minimization: "<<endl;
        cout<<"Estimation: "<<Estimation<<" - "<<ErrN<<" + "<< ErrP<< " ( Min: "<<Min<<" )"<<endl; 
        //Write the results
        double StatEstim = 0;
        for(unsigned int i=0;i<parameters.size();i++){
                if(parameters[i].isAnEstimator){
                         parameters[i].SystError = pair<double,double>(ErrN,ErrP);
                         parameters[i].estimation_syst = Min;
                         StatEstim = parameters[i].estimation_stat;
                }
        } 
 
        if(doMCPseudoExp){
                plots[0].Fill(Min, ErrN,ErrP, false);   
                plots[0].Fill(StatEstim,Min);   
        }
   }
   else{
        if(GetEstimators().size()==2){
                int size = plrScan.size();
                double* Xval = new double[size];
                double* Yval = new double[size];
                double* Zval = new double[size];
                for(int i=0;i<size;i++){
                        if(plrScan[i].FreeParamValue.size()!=2) cerr<<"PLREstimation-1D: problem in the size of FreeParamValue"<<endl;
                        else{
                                Xval[i] = plrScan[i].FreeParamValue[0];
                                Yval[i] = plrScan[i].FreeParamValue[1];
                                Zval[i] = plrScan[i].LogLik-MinlogL0;//diff to the min observed in Likelihood
                }
                }
                SystGraph2D = new TGraph2D(size,Xval,Yval,Zval);
                SystGraph2D->SetName("SystGraph2D");
                //The following line is REQUIRED !!
                SystGraph2D->GetHistogram();
                //--- Search for the minimum ---//
                //Doing a fit (TF2) 
                //adapt the scale ..
                SystFit = new TF2("SystFit","[0]*x*x*x+[1]*y*y*y+[2]*x*x*y+[3]*x*y*y+[4]*x*x+[5]*y*y+[6]*x+[7]*y+[8]",0.9,1.3,100,240);
                double xmin = 0;
                double ymin = 0;
                SystGraph2D->Fit(SystFit,"R");
                SystFit->GetMinimumXY(xmin,ymin);
                //cout<<xmin<<" "<<ymin<<endl;
                double fmin = 0;
                fmin = SystFit->Eval(xmin,ymin);
                //change there ...
                /*
                TList* list = SystGraph2D->GetContourList(0.5+fmin);
                TGraph* contour = (TGraph*) list->At(0);
                int n = contour->GetN();
                Double_t* xcont = contour->GetX();
                Double_t* ycont = contour->GetY();
                double xminErr = TMath::MinElement(n,xcont);
                double xmaxErr = TMath::MaxElement(n,xcont);
                double yminErr = TMath::MinElement(n,ycont);
                double ymaxErr = TMath::MaxElement(n,ycont);
                //Change estimation
                cout<<"Result of the PLR-2D minimization: "<<endl;
                cout<<"Estimation: "<<xmin<<" - "<<xminErr<<" + "<< xmaxErr<< endl;
                cout<<"Estimation: "<<ymin<<" - "<<yminErr<<" + "<< ymaxErr<< endl;
                //----------------------------------            
                delete list;
                delete contour;
                //Write the results
                int it=0;
                for(unsigned int i=0;i<parameters.size();i++){
                        if(parameters[i].isAnEstimator){
                                 if(it==0){
                                         parameters[i].SystError = pair<double,double>(xminErr,xmaxErr);
                                         parameters[i].estimation_syst = xmin;
                                 }
                                 if(it==1){
                                         parameters[i].SystError = pair<double,double>(yminErr,ymaxErr);
                                         parameters[i].estimation_syst = ymin;
                                         //StatEstim = parameters[i].estimation_stat;
                                 }
                                if(it==0 || it==1) it++;
                        }
                } 
                // to be changed !
                if(doMCPseudoExp){
                        plots[0].Fill(xmin, xminErr,xmaxErr, false);    
                        plots[1].Fill(ymin, yminErr,ymaxErr, false);    
                        //plots[0].Fill(StatEstim,Min); 
                }
                */
                
        }
        else{
                cerr<<"We cannot treat the PLR for more than one free parameter for the moment !!"<<endl;
        }
   }
  }
  //}//end of loop over Pseudo-Experiments


        /////////////////////////////////////////////////////
        //---- Redo the minimization of Likelihood @ Minimum ---//
        /////////////////////////////////////////////////////

        for(unsigned int i=0;i<fixParam.size();i++){    
          fixParam[i] = false;  //all uncertainties considered
          parameters[i].fixed = false;
        }
        //--- The free parameters are fixed ---//
        for(unsigned int ind=0;ind<GetEstimatorsIndice().size();ind++){
                fixParam[GetEstimatorsIndice()[ind]] = true;
                ParamValues[GetEstimatorsIndice()[ind]] = parameters[GetEstimatorsIndice()[ind]].estimation_syst;
        }
        //---------------------------//
        ComputeLikelihood (fixParam, ParamValues, false, false, verboseMax);
        //--- Save the results ---//
        count = 0;
        for(unsigned int i=0;i<parameters.size();i++){
                if(parameters[i].isANuisanceParameter){ 
                        parameters[i].estimation_syst = Param[i];
                        parameters[i].SystError = pair<double,double>(ErrParam[i],ErrParam[i]);
                        if(doMCPseudoExp){
                                plots[0].FillUncert(count, parameters[i].PEValue, Param[i]);    
                                count++;
                        }
                }
        }
  }//end of loop over Pseudo-Experiments
        
        /////////////////////////////////////////////////////
        //---- Compute the distributions after minimization ---//
        /////////////////////////////////////////////////////
        for(unsigned int c=0; c<GetChannelsAddress().size();c++){
                for(unsigned int h=0;h<GetChannelsAddress()[c].GetHistosAddress().size();h++){
                //loop over bins
                  int nofdim = GetChannelsAddress()[c].GetHistosAddress()[h].GetNofDimensions();
                  if(nofdim == 1 ){
                        double SignalXsection = 0;
                        double Lumi = 0;
                        vector<double> uncert;
                        vector<double> GraphUncert;
                        vector<double> estim;
                        vector<double> GraphEstim;
                        vector<pair<string,double> > SignalGlobalSyst;
                        for(unsigned int u=0;u<GetParametersAddress().size();u++){
                                if(GetParametersAddress()[u].name!=string("Lumi") && GetParametersAddress()[u].name!=string("SignalXsection") && !GetParametersAddress()[u].isNormalization && !GetParametersAddress()[u].isGlobal){
                                                if(GetParametersAddress()[u].isANuisanceParameter){
                                                 if(!GetParametersAddress()[u].WithGraph) uncert.push_back(GetParametersAddress()[u].estimation_syst);//smearing as been done before
                                                 else GraphUncert.push_back(GetParametersAddress()[u].estimation_syst);//smearing as been done before
                                                }
                                                if(GetParametersAddress()[u].isAnEstimator){
                                                 if(!GetParametersAddress()[u].WithGraph) estim.push_back(GetParametersAddress()[u].estimation_syst);//smearing as been done before
                                                 else GraphEstim.push_back(GetParametersAddress()[u].estimation_syst);//smearing as been done before
                                                }
                                }
                                if(GetParametersAddress()[u].name==string("SignalXsection")) SignalXsection = GetParametersAddress()[u].estimation_syst;
                                if(GetParametersAddress()[u].name==string("Lumi")) Lumi = GetParametersAddress()[u].ExpectedValue;
                                if(GetParametersAddress()[u].isGlobal) SignalGlobalSyst.push_back(pair<string, double>(GetParametersAddress()[u].name,GetParametersAddress()[u].estimation_syst));
                        }
                        vector<double> norm;
                        for(unsigned int id=0;id<GetDatasets().size();id++){
                                int iu = -1;
                                //modif here
                                if(!GetDatasets()[id].isData() && GetDatasets()[id].Name()!=SignalDatasetName){
                                        for(unsigned int u=0;u<GetParametersAddress().size();u++){
                                          if(myPLRMeas->GetParametersAddress()[u].name.find(GetDatasets()[id].Name())<myPLRMeas->GetParametersAddress()[u].name.size()){
                                                if(myPLRMeas->GetParametersAddress()[u].isCommon){
                                                        iu = u;
                                                        break;
                                                }
                                                else{
                                                        cout<<"names "<<myPLRMeas->GetParametersAddress()[u].name<<" "<<myPLRMeas->GetChannelsAddress()[c].GetChannelName()<<endl;
                                                        if(myPLRMeas->GetParametersAddress()[u].name.find(myPLRMeas->GetChannelsAddress()[c].GetChannelName())<myPLRMeas->GetParametersAddress()[u].name.size()){
                                                                iu = u;
                                                                break;
                                                        }
                                                }
                                          }
                                        }
                                        if(iu>0) norm.push_back(GetParametersAddress()[iu].estimation_syst);
                                        else {  
                                                norm.push_back(-1);
                                                cout<<"iu: "<<iu<<" Dname: "<<GetDatasets()[id].Name()<<endl;
                                        }
                                }
                        }
                        double LumiFactor = Lumi/GetLumi();
                        /// Compute the signal global systematic effect
                        double SigGlobFactor = 1.;
                        for(unsigned int i=0;i<SignalGlobalSyst.size();i++){
                                for(unsigned int j=0;j<GetSignalSystematics().size();j++){
                                        for(unsigned int k=0;k<GetSignalSystematics()[j].size();k++){
                                                if(GetSignalSystematics()[j][k].channel == GetChannelsAddress()[c].GetChannelName() &&
                                                  SignalGlobalSyst[i].first == GetSignalSystematics()[j][k].name ){
                                                        SigGlobFactor*=Interpol(SignalGlobalSyst[i].second, 1, myPLRMeas->GetSignalSystematics()[j][k].effm1s, myPLRMeas->GetSignalSystematics()[j][k].effp1s);
                                                        /*
                                                        if(SignalGlobalSyst[i].second>0) SigGlobFactor*=(1+SignalGlobalSyst[i].second*(GetSignalSystematics()[j][k].effp1s-1));
                                                        else SigGlobFactor*=(1+SignalGlobalSyst[i].second*(1-GetSignalSystematics()[j][k].effm1s));     
                                                        */
                                                } 
                                        }
                                }
                        }
                        //include the SigGlobFactor in SignalFactor
                        double SignalFactor=(SignalXsection/GetSignalXsection())*SigGlobFactor;
                        for(unsigned int i=0;i<uncert.size();i++) cout<<"uncert "<<uncert[i]<<endl;
                        for(unsigned int i=0;i<norm.size();i++) cout<<"norm "<<norm[i]<<endl;
                        //to be changed ...
                        if(GetChannelsAddress()[c].GetHistosAddress()[h].GetNofDimensions()==1) GetChannelsAddress()[c].GetHistosAddress()[h].Compute1DFinalDistrib(uncert, GraphUncert, norm, LumiFactor, SignalFactor,GetSignalDatasetIteratorForHisto());
                        if(GetChannelsAddress()[c].GetHistosAddress()[h].GetNofDimensions()==2) GetChannelsAddress()[c].GetHistosAddress()[h].Compute2DFinalDistrib(uncert, GraphUncert, estim, GraphEstim, norm, LumiFactor, SignalFactor,GetSignalDatasetIteratorForHisto());
                        }
                  }     
                }               
                /////////////////////////////////////////////////////



}

void PLRMeasurement::PLRValuesWithLinearAppromixation (int NofPoints, double *X_PLR, double *Y_PLR, const double& Estimation, double &ErrNeg, double &ErrPos, double &Min)
{
  bool first = true;
  bool first_min = true;
  ErrNeg = 0.;
  ErrPos = 0.;
  Min = 0.;
  //Determination du min manquante
  for (int i = 1; i < NofPoints; i++) {
    if (Y_PLR[i] < 0.5 && first) {
      //linear approach, need enought point to be exact
      double a = (Y_PLR[i] - Y_PLR[i - 1]) / (X_PLR[i] - X_PLR[i - 1]);
      double b = Y_PLR[i - 1] - a * X_PLR[i - 1];
      ErrNeg = (0.5 - b) / a;
      first = false;
    }
    if (Y_PLR[i] > 0.5 && !first) {
      //linear approach, need enought point to be exact
      double a = (Y_PLR[i] - Y_PLR[i - 1]) / (X_PLR[i] - X_PLR[i - 1]);
      double b = Y_PLR[i - 1] - a * X_PLR[i - 1];
      ErrPos = (0.5 - b) / a;
      break;
    }
    if (X_PLR[i] > Estimation && first_min) {
      double a = (Y_PLR[i - 1] - Y_PLR[i - 2]) / (X_PLR[i - 1] - X_PLR[i - 2]);
      double b = Y_PLR[i - 2] - a * X_PLR[i - 2];
      Min = (0 - b) / a;
      first_min = false;
    }
  }
  ErrNeg = Min - ErrNeg;
  ErrPos = ErrPos - Min;
  if (fabs (Min - Estimation) > 0.05 * Estimation)
    cerr << "PLRMeasurement::PLRValuesWithLinearAppromixation:: There is a problem in the search of the minimum !!" << endl;
}

//void PLRMeasurement::PLRValuesWithPol3Fit (int NofPoints, double *X_PLR, double *Y_PLR, const double& Estimation, double &ErrNeg, double &ErrPos, double &Min, bool verbose, bool print)
void PLRMeasurement::PLRValuesWithPol3Fit (TGraph* g, int NofPoints, double *X_PLR,  const double& Estimation, double &ErrNeg, double &ErrPos, double &Min, bool verbose, bool print)
{
  double YMini = 0.;
  //TGraph g(NofPoints, X_PLR, Y_PLR);
  if(print){
         TCanvas c;
         c.cd();
         g->Draw("AC");
         c.SaveAs("toto.png");
  }

         TF1 fmin("funcmin","pol3",X_PLR[0],X_PLR[NofPoints-1]);
         if(verbose) g->Fit(&fmin,"QF","",X_PLR[0],X_PLR[NofPoints-1]);
         else g->Fit(&fmin,"QF","",X_PLR[0],X_PLR[NofPoints-1]);
         //pol2
         //Pol3: ax3+bx2+cx+d=0
         double A = fmin.GetParameter(3);
         double B = fmin.GetParameter(2);
         double C = fmin.GetParameter(1);
         double D = fmin.GetParameter(0);
         double P = C/A-B*B/(3*A*A);
         double Q = (2*pow(B,3))/(27*pow(A,3))+D/A-(B*C)/(3*pow(A,3));
         if(verbose){
                cout<<"Poly3 with A: "<<A<<" B: "<<B<<" C: "<<C<<" D: "<<D<<endl;
                cout<<"P: "<<P<<" Q: "<<Q<<endl;
         }      

         //--- Search of the minimum (derivative=0) ---//
         double A2 = 3*A;
         double B2 = 2*B;
         double C2 = C;
         double Delta = B2*B2-4*A2*C2;
         if(Delta>0){
                double X1=0;
                double X2=0;
                X1 = (-B2-sqrt(Delta))/(2*A2);
                X2 = (-B2+sqrt(Delta))/(2*A2);
                if(verbose) cout<<"ROOTS: "<<X1<<" "<<X2<<endl;
                bool found = false;
                if(X_PLR[0]<X1 && X_PLR[NofPoints-1]>X1){
                         Min = X1;
                         found = true;
                }
                if(X_PLR[0]<X2 && X_PLR[NofPoints-1]>X2){
                        Min = X2;
                         found = true;
                }
                if(found){ 
                        YMini = fmin.Eval(Min);
                        if(verbose) cout<<"Min: "<<Min<<" Ymin: "<<YMini<<endl;
                }
                else cerr<<"PLRMeasurement::PLRValuesWithPol3Fit:: Minimum is not found !"<<endl;
         }
         else{
                cerr<<"PLRMeasurement::PLRValuesWithPol3Fit:: Delta is negative ..."<<endl;
         }

         //---- Determination of the uncertainties ---//
         // recompute D and then Q
         D = D - 0.5 - YMini; 
         Q = (2*pow(B,3))/(27*pow(A,3))+D/A-(B*C)/(3*pow(A,3));
         if(verbose) cout<<"Search of the uncertainties with D: "<<D<<" P: "<<P<<" Q: "<<Q<<endl;
         double coeff[4];
         coeff[0] = D;
         coeff[1] = C;
         coeff[2] = B;
         coeff[3] = A;
         double X1 = 0;
         double X2 = 0;
         double X3 = 0;
         TMath::RootsCubic(coeff,X1,X2,X3);
         if(verbose) cout<<"RootsCubic: "<<X1<<" "<<X2<<" "<<X3<<endl;
         //ErrNeg
         if(X1<Min && X1>X_PLR[0]) ErrNeg = Min-X1;
         if(X2<Min && X2>X_PLR[0]) ErrNeg = Min-X2;
         if(X3<Min && X3>X_PLR[0]) ErrNeg = Min-X3;
         //ErrPos
         if(X1>Min && X1<X_PLR[NofPoints-1]) ErrPos = X1-Min;
         if(X2>Min && X2<X_PLR[NofPoints-1]) ErrPos = X2-Min;
         if(X3>Min && X3<X_PLR[NofPoints-1]) ErrPos = X3-Min;
         if(verbose) cout<<"Min"<<Min<<" ErrNeg: "<<ErrNeg<<" ErrPos: "<<ErrPos<<endl;

        
  
  //---------------------//

  //--- Determination of the error @ Ymini + 0.5



}

void PLRMeasurement::PLRValuesWithPol2Fit (int NofPoints, double *X_PLR, double *Y_PLR, const double& Estimation, double &ErrNeg, double &ErrPos, double &Min, bool verbose, bool print)
{
  bool first = true;
  double ErrMin = 0.;
  double ErrMax = 0.;
  double YMini = 0.;
  int NeighbourgPoints = 2;
  TGraph g(NofPoints, X_PLR, Y_PLR);
  if(print){
         TCanvas c;
         c.cd();
         g.Draw("AC");
         c.SaveAs("toto.png");
  }

  //--- First search for the minimim ---//
  // Search the minimim from the measured point
  double min = 9999.;
  int imin = -1;
  for (int i = 1; i < NofPoints; i++) {
        if(Y_PLR[i]<min){
                min = Y_PLR[i];
                imin = i;
        }
  }
  //--- Fit around this point
         int Ximin = 0;
         int Ximax = 0;
         if(imin-NeighbourgPoints<0) Ximin = 0;
         else Ximin = imin-NeighbourgPoints;
         if(imin+NeighbourgPoints>NofPoints) Ximax = NofPoints-NeighbourgPoints;
         else Ximax = imin+NeighbourgPoints;
         TF1 fmin("funcmin","pol2",X_PLR[Ximin],X_PLR[Ximax]);
         if(verbose) g.Fit(&fmin,"","",X_PLR[Ximin],X_PLR[Ximax]);
         else g.Fit(&fmin,"Q","",X_PLR[Ximin],X_PLR[Ximax]);
         //pol2
         double A = fmin.GetParameter(2);
         double B = fmin.GetParameter(1);
         //double C = fmin.GetParameter(0);//ax2+bx+c=0
          Min = (-B)/(2*A); // solution unique normaly
          YMini = fmin.Eval(Min);
          if(verbose){
                cout<<"Determination of the minimum "<<endl;
                cout<<" Point:  "<<imin<<" LogLik =  "<<min<<endl;
                cout<<" Fitted: "<<Min<<" LogLik = "<<YMini<<endl; 
          }
  
  //---------------------//

  //--- Determination of the error @ Ymini + 0.5
  for (int i = 1; i < NofPoints; i++) {
    if (Y_PLR[i] < (0.5+YMini) && first) {
      first = false;
      //fit the graph:
         int imin = 0;
         if(i-NeighbourgPoints>=0) imin = i-NeighbourgPoints;
         int imax = i+NeighbourgPoints;
         TF1 f1("func1","pol2",X_PLR[imin],X_PLR[imax]);
         if(verbose) g.Fit(&f1,"","",X_PLR[imin],X_PLR[imax]);
         else g.Fit(&f1,"Q","",X_PLR[imin],X_PLR[imax]);
         double value = 0;
         //pol2
         double A = f1.GetParameter(2);
         double B = f1.GetParameter(1);
         double C = f1.GetParameter(0)-0.5-YMini;//ax2+bx+c=Ymini+0.5
         //if(verbose) cout<<"NofPoints: "<<f1.GetNumberFitPoints()<<endl;
         double Delta = B*B-4*A*C;
         if(Delta<0.){
                 cerr<<"PLRMeasurement::PLRValuesWithPol2Fit::No real solution ! Delta = "<<Delta<<endl;
                 value = -B/(2*A);
         }
         else value = (-B-sqrt(Delta))/(2*A); // solution min
         ErrMin = value;
    }
    if (Y_PLR[i] > (0.5+YMini) && !first) {
      //fit the graph:
         int imin = i-NeighbourgPoints;
         int imax = 0;
         if(i+NeighbourgPoints>NofPoints) imax = NofPoints-NeighbourgPoints;
         else imax = i+NeighbourgPoints;
         TF1 f2("func2","pol2",X_PLR[imin],X_PLR[imax]);
         if(verbose) g.Fit(&f2,"","",X_PLR[imin],X_PLR[imax]);
         else g.Fit(&f2,"Q","",X_PLR[imin],X_PLR[imax]);
         double value = 0;
         //pol2
         double A = f2.GetParameter(2);
         double B = f2.GetParameter(1);
         double C = f2.GetParameter(0)-0.5-YMini;//ax2+bx+c=YMini+0.5
         double Delta = B*B-4*A*C;
         if(Delta<0.){
                 cerr<<"PLRMeasurement::PLRValuesWithPol2Fit::No real solution ! Delta = "<<Delta<<endl;
                 value = -B/(2*A);
         }
         else value = (-B+sqrt(Delta))/(2*A); // solution max
         ErrMax = value;
      break;
    }
  }
  ErrNeg = Min - ErrMin;
  ErrPos = ErrMax - Min;
  if(verbose) cout<<"Summary: Min: "<<Min<<" ErrNeg: "<<ErrNeg<<" ErrPos: "<<ErrPos<<endl;
  //if (fabs (Min - Estimation) > 0.01 * Estimation)
  //  cerr << "PLRMeasurement::PLRValuesWithPol2Fit:: There is a problem in the search of the minimum !!" << endl;
}

void PLRMeasurement::PLRExamination (int NofPoints, double *X_PLR, double *Y_PLR, int indice)
{
  //to be rewritten for many dimensions
  /*
  double ErrNeg = 0.;
  double ErrPos = 0.;
  double Min = 0.;
  //PLRValuesWithLinearAppromixation (NofPoints, X_PLR, Y_PLR, ErrNeg, ErrPos, Min);
  PLRValuesWithPol2Fit (NofPoints, X_PLR, Y_PLR, ErrNeg, ErrPos, Min);

  if (indice < 0) {             //means it's PLR just with statistic - no systematics
    XsectionEstimStatError = pair < double, double >(ErrNeg, ErrPos);
    XsectionEstimStat = Min;

  }
  else {
    if (indice < NofParam - 1) {
      double err1 = sqrt ((ErrNeg * ErrNeg) - (XsectionEstimStatError.first * XsectionEstimStatError.first));
      double err2 = sqrt ((ErrPos * ErrPos) - (XsectionEstimStatError.second * XsectionEstimStatError.second));
      XsectionEstimSysError.push_back (pair < double, double >(err1, err2));
    }
    else
      XsectionEstimAllError = pair < double, double >(ErrNeg, ErrPos);
    XsectionEstimWithAllSyst = Min;
  }
  */
}



void PLRMeasurement::Write (TFile * fout)
{
  //change the structure of the output

 // fout->cd ();
 // if (!fout->cd ("Xsection"))
  //  fout->mkdir ("Xsection");
  //TDirectory *dir1 = 0;
  //TDirectory *dir2 = 0;
  //dir1 = fout->GetDirectory ("Xsection");
  //dir2 = dir1->GetDirectory (channel.c_str ());
  //if (dir2 == 0)
   // dir2 = dir1->mkdir (channel.c_str ());
  //dir2->cd ();

  //leg->Draw ();
  //c->Write ();
  /*
     if(graphsDeclared){
     cout<<"there"<<endl;
     for(int i=0;i<NofParam;i++){ 
     //graphs[i]->Write();
     cout<<graphs[i+2]->GetName()<<endl;
     }
     }
   */
  //if (StatGraph)
  //  StatGraph->Write ();
  //if (CorrelationMatrixHisto)
  //  CorrelationMatrixHisto->Write ();
        /*
  Xseccentralvalue_wosys->Write ();
  Erreurmean_wosys->Write ();
  Erreurneg_wosys->Write ();
  Erreurpos_wosys->Write ();
  PullErreurmean_wosys->Write ();
  PullErreurneg_wosys->Write ();
  PullErreurpos_wosys->Write ();
  PullErreur2cases_wosys->Write ();
  Xseccentralvalue_wsys->Write ();
  Erreurmean_wsys->Write ();
  Erreurneg_wsys->Write ();
  Erreurpos_wsys->Write ();
  PullErreurmean_wsys->Write ();
  PullErreurneg_wsys->Write ();
  PullErreurpos_wsys->Write ();
  PullErreur2cases_wsys->Write ();
  if (ParameterHistoDeclared) {
    for (int i = 0; i < NofParam; i++) {
      ParametersHisto[i]->Write ();
    }
  }
        */

 fout->cd();
 if(StatGraph1D!=0) StatGraph1D->Write();
 if(SystGraph1D!=0) SystGraph1D->Write();
 if(StatGraph2D!=0) StatGraph2D->Write();
 if(SystGraph2D!=0) SystGraph2D->Write();
 if(StatFit!=0) StatFit->Write();
 if(SystFit!=0) SystFit->Write();
 for(unsigned int i=0;i<plots.size();i++)
        plots[i].Write(fout);
 //Correlation Matrix
 if (CorrelationMatrixHisto)
   CorrelationMatrixHisto->Write ();

  //Add
  TCanvas* cBefore = new TCanvas("cBeforeFit");
  TH2F* hsum  = 0;
  TH2F* hsumFinal  = 0;
  TH2F* hdata = 0;
  //cBefore->Divide(channels.size());
  //int nbkg = 0;
  //if(channels.size()>0 && channels[0].GetHistosAddress().size()>0) nbkg = channels[0].GetHistosAddress()[0].Get2DProcessExp().size();
  vector<TH2F*> mc_process;
  for(unsigned int c = 0;c<channels.size();c++){
        for(unsigned int h=0;h<channels[c].GetHistosAddress().size();h++){
                //do the normalisation of the histograms !!
                //myPLRMeas->GetChannelsAddress()[c].GetHistosAddress()[h].NormalizeDistrib(normInit[c]);
                cout<<"toot "<<h<<endl;
                if(c==0) hdata = myPLRMeas->GetChannelsAddress()[c].GetHistosAddress()[h].Get2DNobs();
                else hdata->Add(myPLRMeas->GetChannelsAddress()[c].GetHistosAddress()[h].Get2DNobs());
                cout<<"toot2"<<endl;
                vector<TH2F*> mc = channels[c].GetHistosAddress()[h].Get2DProcessExp();
                vector<TH2F*> mcFinal = channels[c].GetHistosAddress()[h].Get2DProcessFinal();
                cout<<"toot"<<endl;
                for(unsigned int i=0;i<mc.size();i++){
                        cout<<"Integ before: "<<mc[i]->Integral()<<endl;
                        cout<<"norm: "<<normInit[c][i]<<endl;
                        if(mc[i]->Integral()>0){
                                 if(normInit[c][i]<0) mc[i]->Scale(1.9155);
                                 else mc[i]->Scale(normInit[c][i]/mc[i]->Integral());
                                
                        }
                        cout<<"Integ after: "<<mc[i]->Integral()<<endl;
                }
                if(mc.size()>0 && c==0){
                         hsum = mc[0];
                         mc_process.push_back((TH2F*) mc[0]->Clone(""));
                }
                if(mcFinal.size()>0 && c==0) hsumFinal = mcFinal[0];
                cout<<"toot"<<endl;
                if(mc.size()>1 && c==0){ 
                        for(unsigned int i=1;i<mc.size();i++){
                                        hsum->Add(mc[i]);
                                        mc_process.push_back((TH2F*) (mc[i]->Clone("")));
                        }
                }
                if(mc.size()>1 && c!=0){ 
                        for(unsigned int i=0;i<mc.size();i++){
                                        hsum->Add(mc[i]);
                                        mc_process[i]->Add(mc[i]);
                        }
                }
                if(mcFinal.size()>1 && c==0) for(unsigned int i=1;i<mcFinal.size();i++)         hsumFinal->Add(mcFinal[i]);
                if(mcFinal.size()>1 && c!=0) for(unsigned int i=0;i<mcFinal.size();i++)         hsumFinal->Add(mcFinal[i]);
        }
  }
                THStack* stack = new THStack("stack","");
                for(unsigned int i=0;i<mc_process.size();i++){ 
                        TH1F* tmp = (TH1F*) mc_process[i]->ProjectionY()->Clone("");
                        tmp->SetFillStyle(3003);
                        tmp->SetFillColor(2+i);
                        tmp->SetLineColor(2+i);
                        stack->Add(tmp);
                }
                cout<<"toot"<<endl;
                cBefore->cd(1);
                hdata->SetLineColor(kRed);
                hdata->ProjectionY()->Draw();
                hsum->SetLineColor(kBlack);
                hsum->ProjectionY()->Draw("same");
                hsumFinal->SetLineColor(kBlue);
                //hsumFinal->ProjectionY()->Draw("same");
                //cout<<"hstack integ: "<<stack->Integral()<<endl;
                stack->Draw("same");
                cout<<"toot"<<endl;
  fout->cd();
  cBefore->Write();
  //
  

}

void PLRMeasurement::ResetParameter(Parameter& par){
        par.name = string("");
        par.value = -999.;
        par.PEValue = -999.;
        par.isANuisanceParameter = false;
        par.isAnEstimator = false;
        par.isZeroCentered = false;
        par.isNormalization = false;
        par.isCommon = true;
        par.isGlobal = false;
        par.WithGraph = false;
        par.ExpectedValue = -999.;
        par.estimation_stat = -999.;
        par.estimation_syst = -999.;
        par.estimation_minuit = -999.;
        par.StatError = pair<double,double>(-999.,-999.);
        par.SystError = pair<double,double>(-999.,-999.);
        par.CombinedError = pair<double,double>(-999.,-999.);
        par.uncertainty = pair<double,double>(-999.,-999.);
        par.Error_minuit = -999.;
        //par.step = 0.000001;
        //par.step = 0.000000000001;
        par.step = 0.00001;
        //par.step = 0.0001;
        //par.step = 0.00005;
        par.NofPoints = 0;
        par.nbins = 0;
        par.range = pair<double,double>(-5.,+5.);// in terms of sigma
        par.ErrRange = pair<double,double>(-5.,+5.);// in terms of sigma
        par.fixed = true;
        
}

Parameter PLRMeasurement::GetParameter(const string& name){
        int indice;
        return GetParameter(name, indice);
}

Parameter PLRMeasurement::GetParameter(const string& name, int& ipar){
        ipar = -1;
        for(unsigned int i=0;i<parameters.size();i++){
                if(parameters[i].name == name) {
                        ipar = i;
                        return parameters[i];
                }
        }
        Parameter par;
        ResetParameter(par);
        return par;
}
        
Parameter PLRMeasurement::GetParameter(const int& i){
        if(i< (int)parameters.size()) return parameters[i];
        Parameter par;
        ResetParameter(par);
        return par;
}
        
vector<Parameter> PLRMeasurement::GetParameters() const{
        return parameters;
}

vector<Parameter>& PLRMeasurement::GetParametersAddress(){
        return parameters;
}
        
vector<Parameter> PLRMeasurement::GetNuisanceParameters() const{
        vector<Parameter> vec;
        for(unsigned int i=0;i<parameters.size();i++){
                if(parameters[i].isANuisanceParameter == true ) vec.push_back(parameters[i]);
        }
        return vec;
}
        
vector<Parameter> PLRMeasurement::GetNuisanceParametersNoGraph() const{
        vector<Parameter> vec;
        for(unsigned int i=0;i<parameters.size();i++){
                if(parameters[i].name !=string("Lumi") && parameters[i].name!=string("SignalXsection"))
                        if(parameters[i].isANuisanceParameter == true && parameters[i].WithGraph == false) vec.push_back(parameters[i]);
        }
        return vec;
}
        
vector<Parameter> PLRMeasurement::GetNuisanceParametersGraph() const{
        vector<Parameter> vec;
        for(unsigned int i=0;i<parameters.size();i++){
                if(parameters[i].name !=string("Lumi") && parameters[i].name!=string("SignalXsection"))
                        if(parameters[i].isANuisanceParameter == true && parameters[i].WithGraph == true) vec.push_back(parameters[i]);
        }
        return vec;
}
        
vector<Parameter> PLRMeasurement::GetEstimatorsNoGraph() const{
        vector<Parameter> vec;
        for(unsigned int i=0;i<parameters.size();i++){
                if(parameters[i].name !=string("Lumi") && parameters[i].name!=string("SignalXsection"))
                        if(parameters[i].isAnEstimator == true && parameters[i].WithGraph == false) vec.push_back(parameters[i]);
        }
        return vec;
}
        
vector<Parameter> PLRMeasurement::GetEstimatorsGraph() const{
        vector<Parameter> vec;
        for(unsigned int i=0;i<parameters.size();i++){
                if(parameters[i].name !=string("Lumi") && parameters[i].name!=string("SignalXsection"))
                        if(parameters[i].isAnEstimator == true && parameters[i].WithGraph == true) vec.push_back(parameters[i]);
        }
        return vec;
}

vector<Parameter> PLRMeasurement::GetEstimators() const{
        vector<Parameter> vec;
        for(unsigned int i=0;i<parameters.size();i++){
                if(parameters[i].isAnEstimator == true ) vec.push_back(parameters[i]);
        }
        return vec;
}


vector<int> PLRMeasurement::GetEstimatorsIndice() const{
        vector<int> vec;
        for(unsigned int i=0;i<parameters.size();i++){
                //cout<<"parameters-name "<<parameters[i].name<<" "<<parameters[i].isAnEstimator<<endl;
                if(parameters[i].isAnEstimator == true ) vec.push_back(i);
        }
        return vec;
}

vector<double> PLRMeasurement::GetEstimatorsValue() const{
        vector<double> vec;
        vector<Parameter> estim = GetEstimators();
        for(unsigned int i=0;i<estim.size();i++){
                vec.push_back(estim[i].value);
        }
        return vec;
}
        
void PLRMeasurement::SetParameterValue(const int& indice, const double& value){
        if(indice>=(int)parameters.size()){
                cerr<<"PLRMeasurement::SetParameterValue:: problem in the indice !"<<endl;
                return;
        } 
        parameters[indice].value = value;
}
        
vector<pair<double,double> > PLRMeasurement::GetEstimatorsSystError() const{
        vector<pair<double,double> > vec;
        vector<Parameter> estim = GetEstimators();
        for(unsigned int i=0;i<estim.size();i++){
                vec.push_back(estim[i].SystError);
        }
        return vec;
}
        
vector<pair<double,double> > PLRMeasurement::GetEstimatorsStatError() const{
        vector<pair<double,double> > vec;
        vector<Parameter> estim = GetEstimators();
        for(unsigned int i=0;i<estim.size();i++){
                vec.push_back(estim[i].StatError);
        }
        return vec;
}
        
vector<pair<double,double> > PLRMeasurement::GetEstimatorsCombinedError() const{
        vector<pair<double,double> > vec;
        vector<Parameter> estim = GetEstimators();
        for(unsigned int i=0;i<estim.size();i++){
                vec.push_back(estim[i].CombinedError);
        }
        return vec;
}
        
        
vector<double> PLRMeasurement::GetEstimatorsExpectation() const{
        vector<double> vec;
        vector<Parameter> estim = GetEstimators();
        for(unsigned int i=0;i<estim.size();i++){
                vec.push_back(estim[i].ExpectedValue);
        }
        return vec;
}
        
        
void PLRMeasurement::TimingReport(){
        cout<<"==================================="<<endl;
        cout<<" Timing report (PLRMeasurement) "<<endl;
        cout<<"==================================="<<endl;
        cout<<" Action 1 : "<<time_1/CLOCKS_PER_SEC<<" sec "<<endl;     
        cout<<" Action 2 : "<<time_2/CLOCKS_PER_SEC<<" sec "<<endl;     
        cout<<" Action 3 : "<<time_3/CLOCKS_PER_SEC<<" sec "<<endl;     
        cout<<" Action 4 : "<<time_4/CLOCKS_PER_SEC<<" sec "<<endl;
        cout<<" Total    : "<<(time_1+time_2+time_3+time_4)/CLOCKS_PER_SEC<<" sec "<<endl;      
        cout<<"==================================="<<endl;
}

vector<bool> PLRMeasurement::GetIsADDDatasetList()const {
        vector<bool> out;
        for(unsigned int i=0;i<datasets.size();i++){
                if(datasets[i].isData()) continue;
                bool isDD = false;
                for(unsigned int j=0;j<parameters.size();j++){
                        if(parameters[j].name.find(datasets[i].Name())<parameters[j].name.size()){
                                if(parameters[j].isCommon){
                                        isDD = true;
                                        break;
                                }
                        }
                }
                out.push_back(isDD);
        }
        return out;
}
        
void PLRMeasurement::AddBkgDeterminationParameter(){
        cout<<"size: "<<GetChannelsAddress().size()<<endl;
        for(unsigned int i=0;i<GetChannelsAddress().size();i++){
                cout<<i<<endl;
                GetChannelsAddress()[i];
                cout<<i<<endl;
                GetChannelsAddress()[i].GetBkgEstimation();
                cout<<i<<endl;
                cout<<GetChannelsAddress()[i].GetBkgEstimation().size()<<endl;
                for(unsigned int j=0; j<GetChannelsAddress()[i].GetBkgEstimation().size();j++){
                        cout<<i<<" "<<j<<endl;
                        if(GetChannelsAddress()[i].GetBkgEstimation()[j].isASyst){
                                cout<<"isDD "<<GetChannelsAddress()[i].GetBkgEstimation()[j].isDDEstimated<<" name: "<<GetChannelsAddress()[i].GetBkgEstimation()[j].name<<endl;
                                if(GetChannelsAddress()[i].GetBkgEstimation()[j].isDDEstimated){
                                        Parameter par;
                                        ResetParameter(par);
                                        par.name = string("Xsection-")+GetChannelsAddress()[i].GetBkgEstimation()[j].name+string("-")+GetChannelsAddress()[i].GetChannelName();// ??
                                        par.value = GetChannelsAddress()[i].GetBkgEstimation()[j].NofEvts;
                                        cout<<"parvalue "<<par.value<<endl;
                                        par.ExpectedValue = GetChannelsAddress()[i].GetBkgEstimation()[j].NofEvts;
                                        par.PEValue = GetChannelsAddress()[i].GetBkgEstimation()[j].NofEvts;
                                        par.value = GetChannelsAddress()[i].GetBkgEstimation()[j].NofEvts;
                                        par.isANuisanceParameter = true;
                                        par.isNormalization = true;
                                        par.isCommon = false;
                                        par.uncertainty = pair<double,double>(GetChannelsAddress()[i].GetBkgEstimation()[j].ErrNeg,GetChannelsAddress()[i].GetBkgEstimation()[j].ErrPos);
                                        //par.step = 1.*GetChannelsAddress()[i].GetBkgEstimation()[j].ErrNeg;;
                                        par.step = 0.1;
                                        double rmin = par.value-3*GetChannelsAddress()[i].GetBkgEstimation()[j].ErrNeg;
                                        if(rmin<0) rmin = 0.;
                                        par.range = pair<double,double>(rmin,par.value+3*GetChannelsAddress()[i].GetBkgEstimation()[j].ErrPos);
                                        parameters.push_back(par);
                                }
                                else{
                                        //here: problem ...
                                        //if(i==0){
                                                Parameter par;
                                                ResetParameter(par);
                                                par.isCommon = true;
                                                //par.name = string("Xsection-")+GetChannelsAddress()[i].GetBkgEstimation()[j].name;// ??
                                                par.name = string("Xsection-")+GetChannelsAddress()[i].GetBkgEstimation()[j].name+string("-")+GetChannelsAddress()[i].GetChannelName();// ??
                                                par.value = GetChannelsAddress()[i].GetBkgEstimation()[j].NofEvts;
                                                par.ExpectedValue = GetChannelsAddress()[i].GetBkgEstimation()[j].NofEvts;
                                                par.isANuisanceParameter = true;
                                                par.isNormalization = true;
                                                par.uncertainty = pair<double,double>(GetChannelsAddress()[i].GetBkgEstimation()[j].ErrNeg,GetChannelsAddress()[i].GetBkgEstimation()[j].ErrPos);
                                                par.step = 0.001*GetChannelsAddress()[i].GetBkgEstimation()[j].ErrNeg;
                                                par.range = pair<double,double>(par.value-5*GetChannelsAddress()[i].GetBkgEstimation()[j].ErrNeg,par.value+5*GetChannelsAddress()[i].GetBkgEstimation()[j].ErrPos);
                                                parameters.push_back(par);
                                        //}
                                }
                        }
                }
        
        }
}

//--- Animate graph ---//

void PLRMeasurement::InitAnimateGraph(){
        anim.canvas = 0;
        anim.LogLikEvolution = 0;
        anim.legend = 0;
        anim.doIt = false;
}

void PLRMeasurement::InitAnimateGraph(int MaxNofIter, string outfilename){
        anim.outfilename = outfilename;
        anim.MaxNofIter = MaxNofIter;
        //decleration of histo
        //anim.canvas = new TCanvas("canvas","",1200,1600);
        anim.canvas = new TCanvas("canvas","",600,800);
        anim.canvas->SetFillColor(kWhite);
        for(unsigned int i=0;i<parameters.size();i++){
                if(parameters[i].isAnEstimator || parameters[i].isANuisanceParameter){
                        string name = string("param_")+parameters[i].name;
                        anim.ParamEvolution.push_back( new TH1F(name.c_str(),"",MaxNofIter,1,MaxNofIter+1));
                        anim.ParamEvolution.back()->SetTitle(parameters[i].name.c_str());
                }
        }
        anim.LogLikEvolution = new TH1F("LogLikEvolution","",MaxNofIter,1,MaxNofIter+1);
        anim.LogLikEvolution->SetTitle("-LogLikelihood");
        anim.legend = new TLegend();
        //put histos there ... : addEntry               
        
        //Treatment of canvas
        anim.canvas->Divide(2,1);
        anim.canvas->cd(1)->Divide(1,channels.size());
        int size = 0;
        for(unsigned int i=0;i<parameters.size();i++){
                if(parameters[i].isAnEstimator || parameters[i].isANuisanceParameter) size++;
        }
        anim.canvas->cd(2)->Divide(1,size+1);
        anim.iteration = 1;
}
        
void PLRMeasurement::StartAnimateGraph(){
        anim.doIt = true;
        anim.iteration = 1;
}

void PLRMeasurement::UpdateAnimateGraph(double* param, const double& LogLik){
        /*
        anim.canvas->Divide(2,1);
        anim.canvas->cd(1)->Divide(1,channels.size());
        anim.canvas->cd(2)->Divide(1,parameters.size()+1);
        */
        vector<THStack*> ChannelStack;  
        for(unsigned int i=0;i<channels.size();i++){
                if(channels[i].GetHistosAddress().size()!=1)
                        cerr<<"PLRMeasurement::UpdateAnimateGraph:: too much graphics "<<endl;
                else{
                        anim.canvas->cd(1)->cd(i+1) ;
                        
                        ChannelStack.push_back(new THStack());
                        for(unsigned int j=0;j<channels[i].GetHistosAddress()[0].Get1DProcessExp().size();j++){
                                channels[i].GetHistosAddress()[0].Get1DProcessExp()[j]->SetFillStyle(1001);
                                channels[i].GetHistosAddress()[0].Get1DProcessExp()[j]->SetFillColor(j+3);
                                channels[i].GetHistosAddress()[0].Get1DProcessExp()[j]->SetLineColor(j+3);
                                ChannelStack.back()->Add(channels[i].GetHistosAddress()[0].Get1DProcessExp()[j]);
                                if(anim.iteration == 1)
                                        for(int k=0;k<channels[i].GetHistosAddress()[0].Get1DProcessExp()[j]->GetNbinsX();k++) channels[i].GetHistosAddress()[0].Get1DProcessExp()[j]->SetBinError(k+1,0);
                        }
                        if(anim.iteration ==1 ){
                                 channels[i].GetHistosAddress()[0].Get1DNobs()->GetYaxis()->SetRangeUser(0.,channels[i].GetHistosAddress()[0].Get1DNobs()->GetMaximum()*1.2);
                        }
                        string title = channels[i].GetChannelName();
                        channels[i].GetHistosAddress()[0].Get1DNobs()->SetTitle(title.c_str());
                        channels[i].GetHistosAddress()[0].Get1DNobs()->Draw("");
                        ChannelStack.back()->Draw("Hsame");
                        channels[i].GetHistosAddress()[0].Get1DNobs()->Draw("same");
                }
        }
        anim.canvas->cd(2)->cd(1)->SetFrameFillColor(kGreen);
        anim.canvas->cd(2)->cd(1);
        //Update histo
        anim.LogLikEvolution->SetBinContent(anim.iteration, LogLik);
        //in order to avoid zero ... and scaling pbs ...
        if(anim.iteration==1){
                for(int ib=0;ib<anim.LogLikEvolution->GetNbinsX()+2;ib++) anim.LogLikEvolution->SetBinContent(ib,LogLik);
                //anim.LogLikEvolution->GetYaxis()->SetRangeUser(LogLik*0.99,LogLik*1.01);
        }
        //if(LogLik<anim.LogLikEvolution->GetMinimum()*0.8 || LogLik>anim.LogLikEvolution->GetMinimum()*1.2) anim.LogLikEvolution->GetYaxis()->SetRangeUser(anim.LogLikEvolution->GetMinimum()*0.8,anim.LogLikEvolution->GetMaximum()*1.2);
        cout<<"LogLik: "<<LogLik<<endl;
        anim.LogLikEvolution->Draw();
        for(unsigned int i=0;i<anim.ParamEvolution.size();i++){
                if(parameters[i].isAnEstimator) anim.canvas->cd(2)->cd(i+2)->SetFrameFillColor(kYellow);
                anim.canvas->cd(2)->cd(i+2);
                //Update histo
                anim.ParamEvolution[i]->SetBinContent(anim.iteration, param[i]);
                if(anim.iteration==1){
                        for(int ib=0;ib<anim.ParamEvolution[i]->GetNbinsX()+2;ib++) anim.ParamEvolution[i]->SetBinContent(ib,param[i]);
                        //anim.ParamEvolution[i]->GetYaxis()->SetRangeUser(param[i]*0.99,param[i]*1.01);
                }
                //if(param[i]<anim.ParamEvolution[i]->GetMinimum()*0.8 || param[i]>anim.ParamEvolution[i]->GetMaximum()*1.2) anim.ParamEvolution[i]->GetYaxis()->SetRangeUser(anim.ParamEvolution[i]->GetMinimum()*0.8,anim.ParamEvolution[i]->GetMaximum()*1.2);
                anim.ParamEvolution[i]->Draw();
        }
        //style
        gStyle->SetOptStat(0);
        gStyle->SetTitleFillColor(kWhite);
        gStyle->SetTitleFontSize(0.1);//0.08
        gStyle->cd();
        anim.canvas->Update();
        anim.canvas->Print(((string)(anim.outfilename+string("+"))).c_str());
        anim.iteration++;
        
        // delete THStack
        for(unsigned int i=0;i<ChannelStack.size();i++) delete ChannelStack[i];
        ChannelStack.clear();
}

void PLRMeasurement::WriteAnimateGraph(){
        anim.doIt = false;
        anim.canvas->Print(((string)(anim.outfilename+string("++"))).c_str());
        
}


