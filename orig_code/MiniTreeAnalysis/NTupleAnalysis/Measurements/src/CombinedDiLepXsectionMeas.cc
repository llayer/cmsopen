#include "../interface/CombinedDiLepXsectionMeas.h"


//important: it's needed to have the FCN working
CombinedDiLepXsectionMeas *myCombinedMeas = 0;


CombinedDiLepXsectionMeas::CombinedDiLepXsectionMeas ()
{
  XsectionEstim = 0.;
  NofParam = 0;
  Lumi = 1.;
  isLumiASyst = false;
  MinFCN = 0.;
  SizeOfParam = 1000;
  Param = new double[SizeOfParam];
  ErrParam = new double[SizeOfParam];
  ErrNegParam = new double[SizeOfParam];
  ErrPosParam = new double[SizeOfParam];
  Param_Min = new double[SizeOfParam];
  ErrParam_Min = new double[SizeOfParam];
  ErrNegParam_Min = new double[SizeOfParam];
  ErrPosParam_Min = new double[SizeOfParam];
  for (int i = 0; i < SizeOfParam; i++) {
    Param[i] = 0;
    ErrParam[i] = 0;
    ErrPosParam[i] = 0;
    ErrNegParam[i] = 0;
    Param_Min[i] = 0;
    ErrParam_Min[i] = 0;
    ErrPosParam_Min[i] = 0;
    ErrNegParam_Min[i] = 0;
  }
  MaxNofPoints = 1000;
  X_PLR = new double[MaxNofPoints];
  X_PLR = new double[MaxNofPoints];
  for (int i = 0; i < MaxNofPoints; i++) {
    X_PLR[i] = 0;
    Y_PLR[i] = 0;
  }
  //--- Plots ---//
  string cname = "DiLepXsection_Combined";
  c = new TCanvas (cname.c_str ());
  c->SetFillColor (10);
  c->SetFillStyle (4000);
  leg = new TLegend (0.6, 0.7, 0.8, 0.9);
  histo = 0;
  CorrelationMatrixHisto = 0;
  StatGraph = 0;
  graphs = 0;
  graphsDeclared = false;
  //other histos
  Xseccentralvalue_wosys = new TH1F ("Xseccentralvalue_wosys", "", 50, 0., 300.);
  Erreurparabolic_wosys = new TH1F ("Erreurparabolic_wosys", "", 50, 0., 100.);
  Erreurmean_wosys = new TH1F ("Erreurmean_wosys", "", 50, 0., 100.);
  Erreurneg_wosys = new TH1F ("Erreurneg_wosys", "", 50, 0., 100.);
  Erreurpos_wosys = new TH1F ("Erreurpos_wosys", "", 50, 0., 100.);
  Xseccentralvalue_wsys = new TH1F ("Xseccentralvalue_wsys", "", 50, 0., 300.);
  Erreurparabolic_wsys = new TH1F ("Erreurparabolic_wsys", "", 50, 0., 100.);
  Erreurmean_wsys = new TH1F ("Erreurmean_wsys", "", 50, 0., 100.);
  Erreurneg_wsys = new TH1F ("Erreurneg_wsys", "", 50, 0., 100.);
  Erreurpos_wsys = new TH1F ("Erreurpos_wsys", "", 50, 0., 100.);

  //important: it's needed to have the FCN working
  myCombinedMeas = this;
}

CombinedDiLepXsectionMeas::~CombinedDiLepXsectionMeas ()
{
  cout<<"delete"<<endl;
  delete[]Param;
  delete[]ErrParam;
  delete[]ErrNegParam;
  delete[]ErrPosParam;
  delete[]Param_Min;
  delete[]ErrParam_Min;
  delete[]ErrNegParam_Min;
  delete[]ErrPosParam_Min;
  cout<<"delete"<<endl;
  //delete[]X_PLR;
  //delete[]Y_PLR;
  cout<<"delete"<<endl;
  if (graphsDeclared) {
    if (graphs != 0) {
      for (int i = 0; i < NofParam + 2; i++) {
        delete graphs[i];
      }
      delete[]graphs;
    }
  }
  cout<<"delete"<<endl;
  delete Xseccentralvalue_wosys;
  delete Erreurparabolic_wosys;
  delete Erreurmean_wosys;
  delete Erreurneg_wosys;
  delete Erreurpos_wosys;
  delete Xseccentralvalue_wsys;
  delete Erreurparabolic_wsys;
  delete Erreurmean_wsys;
  delete Erreurneg_wsys;
  delete Erreurpos_wsys;
  cout<<"delete-a"<<endl;
  //
  delete c;
  delete leg;
  delete histo;
  delete CorrelationMatrixHisto;
  delete StatGraph;
  cout<<"delete-b"<<endl;
}


void CombinedDiLepXsectionMeas::AddChannel (const DiLepXsectionMeas & m, bool verbose)
{
  if (verbose)
    cout << "Add channel " << m.GetName () << endl;
  bool isOk = false;
  if (channels.size () == 0) {
    SetLumi (m.GetLumi (), m.GetLumiError ());
    LoadDatasets (m.GetDatasets ());
    isOk = true;
  }
  else {
    if (CheckDatasets (datasets, m.GetDatasets ()))
      isOk = true;
    else
      isOk = false;
    if (CheckLumi (GetLumi (), GetLumiError (), m.GetLumi (), m.GetLumiError ()))
      isOk = true;
    else
      isOk = false;
    if (CheckSystematicList (m))
      isOk = true;
    else
      isOk = false;
    if (verbose) {
      cout << "Datasets " << CheckDatasets (datasets, m.GetDatasets ()) << endl;
      cout << "Lumi " << CheckLumi (GetLumi (), GetLumiError (), m.GetLumi (), m.GetLumiError ()) << endl;
      cout << "Systematics " << CheckSystematicList (m) << endl;
    }
  }
  if (isOk)
    channels.push_back (&m);

}

bool CombinedDiLepXsectionMeas::CheckDatasets (vector < Dataset > DS1, vector < Dataset > DS2) const
{
  if (DS1.size () != DS2.size ()) {
    cout << "CombinedDiLepXsectionMeas::CheckDatasets: the size of datasets collection is different !" << endl;
    return false;
  }
  for (unsigned int i = 0; i < DS1.size (); i++) {
    if (DS1[i].Name () != DS2[i].Name ()) {
      cout << "CombinedDiLepXsectionMeas::CheckDatasets: the name of datasets differ !" << endl;
      return false;
    }
  }
  return true;
}

bool CombinedDiLepXsectionMeas::CheckLumi (float Lumi1, float LumiErr1, float Lumi2, float LumiErr2) const
{
  float diff = 0.000001;        //test of equality with float
  bool isOk = false;
  if (Lumi1 > (Lumi2 - diff) && Lumi1 < (Lumi2 + diff))
    isOk = true;
  else
    isOk = false;
  if (LumiErr1 > (LumiErr2 - diff) && LumiErr1 < (LumiErr2 + diff))
    isOk = true;
  else
    isOk = false;
  return isOk;
}

void CombinedDiLepXsectionMeas::AddSystematic (string name, bool correlated)
{
  CombinedUncertainty u;
  u.name = name;
  u.correlated = correlated;
  uncertainties.push_back (u);
}

bool CombinedDiLepXsectionMeas::CheckSystematicList (const DiLepXsectionMeas & m) const
{
  if (uncertainties.size () == 0) {
    cout << "CombinedDiLepXsectionMeas::CheckSystematicList: you should first load the systematics !" << endl;
    return false;
  }
  if (uncertainties.size () != m.GetUncertainties ().size ()) {
    cout << "CombinedDiLepXsectionMeas::CheckSystematicList: the number of systematics differ" << endl;
    return false;
  }
  for (unsigned int i = 0; i < uncertainties.size (); i++) {
    m.GetLumi ();
    m.GetUncertainties ();      //.GetName();
    if (uncertainties[i].name != m.GetUncertainties ()[i].GetName ()) {
      cout << "CombinedDiLepXsectionMeas::CheckSystematicList: the names of systematics differ" << endl;
      return false;
    }
  }
  return true;
}

void CombinedDiLepXsectionMeas::SetLumi (float value, float absErr)
{
  Lumi = value;
  LumiError = absErr;
}

double CombinedDiLepXsectionMeas::GetLumi () const
{
  return Lumi;
}

double CombinedDiLepXsectionMeas::GetLumiError () const
{
  return LumiError;
}

/*
void CombinedDiLepXsectionMeas::AddUncertainty(string name, float paramMean, float paramMinus, float paramPlus){
        AllDatasetUncertainties uncert(name);
        uncert.LoadDatasets(datasets);
        uncert.SetParameters(paramMean, paramMinus, paramPlus);
        uncertainties.push_back(uncert);
}

void CombinedDiLepXsectionMeas::AddUncertainty(string name, Syst SysValues){
        AllDatasetUncertainties uncert(name);
        uncert.LoadDatasets(datasets);
        uncert.SetParameters(SysValues.mean, SysValues.m1sigma, SysValues.p1sigma);
        uncertainties.push_back(uncert);
}

void CombinedDiLepXsectionMeas::AddUncertainty(string name, SystEffect SysValues){
        AllDatasetUncertainties uncert(name);
        uncert.LoadDatasets(datasets);
        uncert.SetParameters(SysValues.mean, SysValues.m1sigma, SysValues.p1sigma);
        uncertainties.push_back(uncert);
        LoadEfficiencies(uncertainties.size()-1, SysValues.effmean, SysValues.effm1s, SysValues.effp1s);
}

void CombinedDiLepXsectionMeas::SetNobs(double nobs) {Nobs = nobs;}

double CombinedDiLepXsectionMeas::GetNobs() const{return Nobs;}
        
string CombinedDiLepXsectionMeas::GetSignalDatasetName() const { return SignalDatasetName;}
*/

//double CombinedDiLepXsectionMeas::GetBRChannel() const{ return BRChannel;}

/*
void CombinedDiLepXsectionMeas::LoadEfficiencies(int iUncertainty, float effmean, float effm1s, float effp1s){
        if(iUncertainty>=0 && iUncertainty<(int)uncertainties.size()){
                //put the same systematic for all samples (1) except for ttbar...
                for(unsigned int i=0;i<datasets.size();i++){
                        if(datasets[i].Name().find("TTbar")<datasets[i].Name().size())
                                uncertainties[iUncertainty].LoadEfficiencies(i,effmean, effm1s, effp1s);
                        else    
                                uncertainties[iUncertainty].LoadEfficiencies(i,1., 1., 1.);
                }
        }       
}
*/

void CombinedDiLepXsectionMeas::LoadDatasets (const vector < Dataset > &Datasets)
{
  datasets = Datasets;
  if (Lumi <= 0) {
    cout << "CombinedDiLepXsectionMeas::The luminosity has to be provided before loading the datasets !" << endl;
    return;
  }
  /*
     for(unsigned int i=0;i<datasets.size();i++){
     InitNofEvents.push_back(datasets[i].NofEvtsToRunOver()*datasets[i].NormFactor()*Lumi);
     SelEff.push_back(1.);
     SelEvents.push_back(0);
     }
     //Add the uncertainty on the Xsection as first one
     Xsection.SetName(string("Xsection"));
     Xsection.LoadDatasets(datasets);
     DDEstim.clear();
     for(unsigned int d=0;d<datasets.size();d++){
     Xsection.SetParameters(d, datasets[d].Xsection(), (1-datasets[d].GetCrossSectionError().first)*datasets[d].Xsection(), (1+datasets[d].GetCrossSectionError().second)*datasets[d].Xsection());
     Xsection.LoadEfficiencies(d, datasets[d].Xsection(), (1-datasets[d].GetCrossSectionError().first)*datasets[d].Xsection(), (1+datasets[d].GetCrossSectionError().second)*datasets[d].Xsection());
     DDEstimation a;
     a.isDDEstimated = false;
     a.NofEvts = 0;
     a.ErrNeg = 0;
     a.ErrPos = 0;
     DDEstim.push_back(a);        
     }
   */

}

/*
void CombinedDiLepXsectionMeas::Fill(int idataset, int iUncertainty, int icond, float weight){
        if(iUncertainty>=0 && iUncertainty<(int)uncertainties.size()){
                if(idataset>=0 && idataset<(int)datasets.size()){
                        uncertainties[iUncertainty].Fill(idataset, icond, weight);                      
                }
                else cout<<"DiLepXSectionMeas::Fill the iDataset parameter is wrong"<<endl;
        }
        else cout<<"DiLepXSectionMeas::Fill the iUncertainty parameter is wrong"<<endl;
}

void CombinedDiLepXsectionMeas::Fill(int idataset, string sysName, int icond, float weight){
        int iUncertainty = -1;
        for(unsigned int i=0;i<uncertainties.size();i++){
                if(uncertainties[i].GetName() == sysName){ 
                        iUncertainty = i;
                        break;
                }
        }
        if(iUncertainty>=0 && iUncertainty<(int)uncertainties.size()){
                if(idataset>=0 && idataset<(int)datasets.size()){
                        uncertainties[iUncertainty].Fill(idataset, icond, weight);                      
                }
                else cout<<"DiLepXSectionMeas::Fill the iDataset parameter is wrong"<<endl;
        }
        else cout<<"DiLepXSectionMeas::Fill the iUncertainty parameter is wrong"<<endl;
}

*/
//void CombinedDiLepXsectionMeas::ComputeLikelihood (bool IncludeSYS, bool FixParameter, int NumberOfP, int *ListOfP, double *ValuesOfP)
void CombinedDiLepXsectionMeas::ComputeLikelihood (vector < bool > fixParam, vector < double >ParamValues, bool WriteSysName,  bool doCorrelationMatrixHisto,  bool verbose)
{


  TMinuit minuit (uncertainties.size () + 2);
  // Set fonction pointer
  minuit.SetFCN (CombinedFcn);

  double arglist[uncertainties.size () + 2];
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

  int iter = 1;
  //First parameter is the Xsection 
  minuit.mnparm (0, "Xsection", ParamValues[0], 0.1, 0., 700., ierflg); //should not be hard-coded
  //Second paramter is the Luminosity
  if (isLumiASyst) {
    minuit.mnparm (1, "Lumi", ParamValues[1], 0.01, -5, 5, ierflg);
    if (WriteSysName)
      SysName.push_back (string ("Lumi"));
    iter = 2;
  }
  //Add the MC cross-section as parameters in the TMinuit
  for (unsigned int d = 0; d < datasets.size (); d++) {
    if (!datasets[d].isData () && datasets[d].Name ().find ("TT") > datasets[d].Name ().size ()) {
      string name ("Xsection-");
      name += datasets[d].Name ();
      //replace by loop over channels
      if (GetChannels ()[0]->GetDDEstimation ()[d].isASyst) {
        if (WriteSysName)
          SysName.push_back (name);
        minuit.mnparm (iter, name.c_str (), ParamValues[iter], 0.01, -5, 5, ierflg);
        iter++;
      }
    }
  }
  for (unsigned int i = 0; i < uncertainties.size (); i++) {
    // interval +/- N sigma
    //int NSigma = 20;
    //float min = NSigma*(uncertainties[i].GetParameters(0)[1]-uncertainties[i].GetParameters(0)[0]);
    //float max = NSigma*(uncertainties[i].GetParameters(0)[2]-uncertainties[i].GetParameters(0)[0]);
    //minuit.mnparm (i + iter, uncertainties[i].GetName ().c_str (), ParamValues[i+iter], 0.001, min, max, ierflg);
    minuit.mnparm (i + iter, uncertainties[i].name, ParamValues[i + iter], 0.001, -5, 5, ierflg);
    if (WriteSysName)
      SysName.push_back (uncertainties[i].name);
  }
  iter += uncertainties.size ();
  //Add the MC cross-section as parameters in the TMinuit: for other channels when it's uncorrelated
  if (GetChannels ().size () > 1) {
    for (unsigned int c = 1; c < GetChannels ().size (); c++)
      for (unsigned int d = 0; d < datasets.size (); d++) {
        if (!datasets[d].isData () && datasets[d].Name ().find ("TT") > datasets[d].Name ().size ()) {
          string name ("Xsection-");
          name += datasets[d].Name ();
          name += GetChannels ()[c]->GetName ();
          //replace by loop over channels
          if (GetChannels ()[0]->GetDDEstimation ()[d].isASyst && GetChannels ()[c]->GetDDEstimation ()[d].isDDEstimated) {
            if (WriteSysName)
              SysName.push_back (name);
            minuit.mnparm (iter, name.c_str (), ParamValues[iter], 0.01, -5, 5, ierflg);
            iter++;
          }
        }
      }
  }

  for (unsigned int i = 0; i < fixParam.size (); i++) {
    if (fixParam[i])
      minuit.FixParameter (i);
  }
  cout << "THERE" << endl;

  // Set Print Level
  // -1 no output
  // 1 standard output   
  minuit.SetPrintLevel (-1);

  // Set error Definition: 0.5 for negative log likelihood
  minuit.SetErrorDef (0.5);

  // 1 standard
  // 2 try to improve minimum (slower)
  arglist[0] = 2;               //1000
  cout << "THERE" << endl;
  cout << "Max iterations: " << minuit.GetMaxIterations () << endl;
  minuit.SetMaxIterations (10);
  minuit.mnexcm ("SET STR", arglist, 1, ierflg);
  cout << "Max iterations: " << minuit.GetMaxIterations () << endl;
  cout << "THERE" << endl;

  arglist[0] = 2;               //1000
  cout << "THERE" << endl;
  cout << "Max iterations: " << minuit.GetMaxIterations () << endl;
  minuit.SetMaxIterations (10);
  cout << "Max iterations: " << minuit.GetMaxIterations () << endl;
  minuit.mnexcm ("MIGRAD", arglist, 1, ierflg);
  cout << "THERE" << endl;
  cout << "Max iterations: " << minuit.GetMaxIterations () << endl;
  minuit.SetMaxIterations (10);
  cout << "Max iterations: " << minuit.GetMaxIterations () << endl;
  minuit.mnexcm ("MINOS", arglist, 1, ierflg);
  cout << "THERE" << endl;
  cout << "Max iterations: " << minuit.GetMaxIterations () << endl;
  minuit.SetMaxIterations (10);
  cout << "Max iterations: " << minuit.GetMaxIterations () << endl;
  minuit.mnexcm ("STOP", arglist, 0, ierflg);
  cout << "THERE" << endl;

  // Values of the fitting parameters  
  double ln0, edm, errdef;
  int nvpar, nparx, icstat;
  minuit.mnstat (ln0, edm, errdef, nvpar, nparx, icstat);
  MinFCN = ln0;                 // to be defined
  cout << "THERE-HERE" << endl;

  // Values of errors (parabolic and MINOS)  
  for (int j = 0; j < NofParam; ++j) {
    double ErrParabolic, gcc;
    ErrNegParam[j] = 0.;        // to be defined
    ErrPosParam[j] = 0.;        // to be defined
    minuit.mnerrs (j, ErrPosParam[j], ErrNegParam[j], ErrParabolic, gcc);
  }
   cout<<"blabla"<<endl;
  for (int j = 0; j < NofParam; ++j) {
    minuit.GetParameter (j, Param[j], ErrParam[j]);
  }
   cout<<"blabla"<<endl;
  if (verbose) {
    cout << "################# after minimization ####################" << endl;
    for (int j = 0; j < NofParam; ++j) {
      std::cout << "Param[" << j << "] = " << Param[j] << " +/-" << ErrParam[j] << " (" << ErrNegParam[j] << "+" << ErrPosParam[j] << ")" << std::endl;
    }
  }
   cout<<"blabla"<<endl;
  if(doCorrelationMatrixHisto){
        CorrelationMatrixHisto = new TH2F("CorrelationMatrixHisto","Correlation Matrix",NofParam,0,NofParam,NofParam,0,NofParam);
        for(int i=0;i<NofParam;i++){
                CorrelationMatrixHisto->GetXaxis()->SetBinLabel(i+1,ParamNames[i].c_str());
                CorrelationMatrixHisto->GetYaxis()->SetBinLabel(i+1,ParamNames[i].c_str());
        }
        double matrix[NofParam][NofParam];
        minuit.mnemat(&matrix[0][0],NofParam);  
        for(int i=0;i<NofParam;i++){
                for(int j=0;j<NofParam;j++){
                        CorrelationMatrixHisto->SetBinContent(i+1,j+1,matrix[i][j]);
                }
        }
  }

}

void CombinedDiLepXsectionMeas::Plot (TString S, int NofPoints, double Xmin, double Xmax, double minimumL, bool verbose)
{
  c->cd ();
  double P[100];
  for (int j = 0; j < NofParam; j++) {
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

  histo = new TH2F ("histo", "", 500, 0.9 * Xmin, 1.1 * Xmax, 100, -0.01, Ymin_plot + 5.);
  histo->GetYaxis ()->SetTitleOffset (1.2);
  histo->GetXaxis ()->SetTitle ("Xsec");
  histo->GetYaxis ()->SetTitle ("LogL");

  if (S != "Same")
    histo->Draw ();

  StatGraph = new TGraph (NofPoints, x, y);
  StatGraph->SetName("Graph_Stat");
  StatGraph->SetMarkerStyle (29);
  StatGraph->SetMarkerColor (1);
  StatGraph->SetMarkerSize (1.);
  StatGraph->SetTitle ("");
  if (S == "AP")
    StatGraph->Draw ("*");
  if (S == "Same")
    StatGraph->Draw ("Psame");
  leg->AddEntry (StatGraph, "Stat", "P");

  //Search the errors
  bool first = true;
  float ErrMin = 0.;
  float ErrMax = 0.;
  //int NPSearch = 100;
  for (int i = 0; i < NofPoints; i++) {
    if (y[i] < 0.5 && first) {
      //use a linear approximation between 2 NofPoints
      ///* 
      float A = (y[i] - y[i - 1]) / (x[i] - x[i - 1]);
      float Y = 0.5 - y[i - 1];
      ErrMin = Y / A + x[i - 1];
      //*/
      first = false;
      //fit the graph:
      /*
         TF1* f1 = new TF1("func1","pol3",y[i-1],XsectionEstim);
         StatGraph->Fit(f1);
         c->cd();
         f1->Draw("same");
         for(int x=0;x<NPSearch;x++){
         float value = x*(y[i]-y[i-1])+y[i-1];
         float out = f1->Eval(value);
         if(out<0.5){ 
         //ErrMin = out;
         break;
         }
         }
       */
    }
    if (y[i] > 0.5 && !first) {
      ///*
      float A = (y[i] - y[i - 1]) / (x[i] - x[i - 1]);
      float Y = 0.5 - y[i - 1];
      ErrMax = Y / A + x[i - 1];
      //*/
      //fit the graph:
      /*
         TF1* f2 = new TF1("func2","pol3",XsectionEstim,y[i]);
         f2->SetLineColor(2);
         f2->SetLineWidth(2);
         StatGraph->Fit(f2);
         for(int x=0;x<NPSearch;x++){
         float value = x*(y[i]-y[i-1])+y[i-1];
         float out = f2->Eval(value);
         if(out>0.5){ 
         //ErrMax = out;
         break;
         }
         }
       */
      break;
    }
  }
  if (verbose) {
    cout << "--------------------------------------" << endl;
    cout << "Stat Errors are: -" << ErrMin << "/+" << ErrMax << endl;
    cout << "--------------------------------------" << endl;
  }
  XsectionEstimStatError = pair < double, double >((ErrMin - Param[0]), (ErrMax - Param[0]));




}

void CombinedDiLepXsectionMeas::Plot_PLR (TString S, int NofPoints, double Xmin, double Xmax, double *x, double *y, TGraph * &graph, int color, string name)
{
  c->cd ();

  TH2F *histo = new TH2F ("histo", "", 500, 0.9 * Xmin, 1.1 * Xmax, 100, -0.01, Ymin_plot + 5.);
  histo->GetYaxis ()->SetTitleOffset (1.2);
  histo->GetXaxis ()->SetTitle ("Xsec");
  histo->GetYaxis ()->SetTitle ("LogL");

  if (S != "Same")
    histo->Draw ();

  graph = new TGraph (NofPoints, x, y);
  graph->SetMarkerStyle (20);
  graph->SetMarkerSize (1.);
  graph->SetMarkerColor (color);
  leg->AddEntry (graph, name.c_str (), "P");
  graph->SetTitle ("");
  if (S == "AP")
    graph->Draw ("*");
  if (S == "Same")
    graph->Draw ("Psame");
}

double CombinedDiLepXsectionMeas::XsectionFCN (Double_t * par)
{
  //int npar = uncertainties.size () + 2; 
  int npar = NofParam;
  int iflag = 1;                // compute
  double returnValue = 0.;
  double gin[npar];             //derivative
  for (int i = 0; i < npar; i++)
    gin[i] = 0;
  CombinedFcn (npar, gin, returnValue, par, iflag);
  return (returnValue);
}

void CombinedFcn (Int_t & npar, Double_t * gin, Double_t & f, Double_t * par, Int_t iflag)
{
  bool debug = false;           // put to true carrefully ... this function is called a lot of time. This boolean cannot be give as a parameter

  double LnLikelihood = 0;
  float localSignXsection = par[0];
  float localLumi = myCombinedMeas->GetLumi ();
  if (myCombinedMeas->IsLumiASyst ())
    localLumi = myCombinedMeas->GetLumi () + par[1] * myCombinedMeas->GetLumiError ();

  if (debug) {
    cout << "###############################" << endl;
    cout << "    In CombinedFcn              " << endl;
    cout << "###############################" << endl;
    cout << "LocalSignXsection = " << localSignXsection << " " << par[0] << endl;
    cout << "LocalLumi = " << localLumi << endl;
    //cout<<"BR = "<<BRChannel<<endl;
  }
  int offset = myCombinedMeas->GetNofParam () - myCombinedMeas->GetUncertainties ().size ();

  /////////////////////////////////
  // Loop over the channels
  ////////////////////////////////

  float *NexpTot = new float[myCombinedMeas->GetChannels ().size ()];
  for (unsigned int w = 0; w < myCombinedMeas->GetChannels ().size (); w++)
    NexpTot[w] = 0;
  int iter_uncorr = 0;
  int iter_uncorr2 = 0;

  cout << "LOOP over channels " << myCombinedMeas->GetChannels ().size () << endl;
  for (unsigned c = 0; c < myCombinedMeas->GetChannels ().size (); c++) {
    cout << "Channel " << c << endl;

    //slope 1srD: myCombinedMeas->GetUncertainties() - 2ndD: datasets 
    //slope: it's computed per nof sigma (and not in term of DeltaParam)
    float **slope = new float *[myCombinedMeas->GetUncertainties ().size () + offset];
    for (unsigned int i = 0; i < myCombinedMeas->GetUncertainties ().size () + offset; i++) {
      slope[i] = new float[1];
      slope[i][0] = 0.;
    }
    int iter = 0;
    for (unsigned int d = 0; d < myCombinedMeas->GetDatasets ().size (); d++) {
      if (!myCombinedMeas->GetDatasets ()[d].isData () && myCombinedMeas->GetDatasets ()[d].Name ().find ("TT") > myCombinedMeas->GetDatasets ()[d].Name ().size ()) {
        int case_ = 0;
        if (c == 0)
          case_ = iter;
        else {
          case_ = iter + iter_uncorr;
          iter_uncorr++;
        }
        if (myCombinedMeas->GetChannels ()[c]->GetDDEstimation ()[d].isASyst) {
          if (par[case_] > 0)
            slope[case_][0] = myCombinedMeas->GetChannels ()[c]->GetMCXsectionUncertainty ().GetParameters (d)[0];
          else
            slope[case_][0] = myCombinedMeas->GetChannels ()[c]->GetMCXsectionUncertainty ().GetParameters (d)[2];
          iter++;
        }
      }
    }
    for (unsigned int i = offset; i < myCombinedMeas->GetUncertainties ().size () + offset; i++) {
      slope[i] = new float[myCombinedMeas->GetDatasets ().size ()];
    }

    for (unsigned int i = 0; i < myCombinedMeas->GetChannels ()[c]->GetUncertainties ().size (); i++) {
      if (debug)
        cout << "# uncertainty: " << myCombinedMeas->GetUncertainties ()[i].name << endl;
      for (unsigned int j = 0; j < myCombinedMeas->GetDatasets ().size (); j++) {
        if (debug)
          cout << "  - Dataset: " << myCombinedMeas->GetDatasets ()[j].Name () << endl;
        //treat the asymetric case - slope: treat it as a linear behaviour
        int case_ = i + offset;
        if (!myCombinedMeas->GetUncertainties ()[i].correlated && c != 0) {
          case_ = i + offset + iter_uncorr;
          iter_uncorr++;
        }
        if (par[case_] > 0) {
          slope[case_][j] = (myCombinedMeas->GetChannels ()[c]->GetUncertainties ()[i].GetEfficiencies (j)[2] - myCombinedMeas->GetChannels ()[c]->GetUncertainties ()[i].GetEfficiencies (j)[0]);
        }
        else {
          slope[case_][j] = (myCombinedMeas->GetChannels ()[c]->GetUncertainties ()[i].GetEfficiencies (j)[0] - myCombinedMeas->GetChannels ()[c]->GetUncertainties ()[i].GetEfficiencies (j)[1]);
        }
        if (debug) {
          cout << "slope + = " << (myCombinedMeas->GetChannels ()[c]->GetUncertainties ()[i].GetEfficiencies (j)[2] -
                                   myCombinedMeas->GetChannels ()[c]->GetUncertainties ()[i].GetEfficiencies (j)[0]) << endl;
          cout << "slope - = " << (myCombinedMeas->GetChannels ()[c]->GetUncertainties ()[i].GetEfficiencies (j)[0] -
                                   myCombinedMeas->GetChannels ()[c]->GetUncertainties ()[i].GetEfficiencies (j)[1]) << endl;
        }
      }
    }


    vector < float >Nexp;
    //For each process, compute the nof of expected/estimated event
    if (debug) {
      cout << "#####################" << endl;
      cout << "Compute the #events" << endl;
      cout << "#####################" << endl;
    }
    int ndatasets = 0;
    if (myCombinedMeas->GetUncertainties ().size () > 0)
      ndatasets = myCombinedMeas->GetDatasets ().size ();
    iter = 0;
    for (int i = 0; i < ndatasets; i++) {
      float a = 1.;
      Nexp.push_back (a);
      if (debug)
        cout << "# Dataset: " << myCombinedMeas->GetDatasets ()[i].Name () << endl;

      if (myCombinedMeas->GetChannels ()[c]->GetDDEstimation ()[i].isDDEstimated) {
        int case_ = 0;
        if (c == 0)
          case_ = iter;
        else {
          case_ = iter + iter_uncorr2;
          iter_uncorr2++;
        }
        if (myCombinedMeas->GetChannels ()[c]->GetDDEstimation ()[i].isASyst) {
          float slope = 0;
          if (par[case_] > 0)
            slope = myCombinedMeas->GetChannels ()[c]->GetDDEstimation ()[i].ErrPos;
          if (par[case_] < 0)
            slope = myCombinedMeas->GetChannels ()[c]->GetDDEstimation ()[i].ErrNeg;
          Nexp.back () = myCombinedMeas->GetChannels ()[c]->GetDDEstimation ()[i].NofEvts + slope * par[iter];
          iter++;
          continue;
        }
        else {
          Nexp.back () = myCombinedMeas->GetChannels ()[c]->GetDDEstimation ()[i].NofEvts;
          continue;
        }
      }
      //only run on samples estimated from MC
      // for j=0, it should be Xsection*Lumi, then for j>0, it should be efficiencies 
      // TT sign = TT x BR_sign
      // TT bkg = TT x (1-BR_sign)

      //Compute the first term for each MC dataset which is the cross-sectionxbrxlumixeff
      //if(!myCombinedMeas->GetDatasets()[i].isData() && myCombinedMeas->GetDatasets()[i].Name().find("TT")>myCombinedMeas->GetDatasets()[i].Name().size()){
      if (!myCombinedMeas->GetDatasets ()[i].isData ()) {
        if (myCombinedMeas->GetDatasets ()[i].Name () == myCombinedMeas->GetChannels ()[c]->GetSignalDatasetName ()) {
          Nexp.back () =
            localSignXsection * myCombinedMeas->GetChannels ()[c]->GetBRChannel () * localLumi * myCombinedMeas->GetChannels ()[c]->GetSelEff (i) *
            myCombinedMeas->GetChannels ()[c]->GetDatasets ()[i].PreSelEfficiency ();
        }
        else {
          string dname = myCombinedMeas->GetDatasets ()[i].Name ();
          if (dname.find ("TT") >= 0 && dname.find ("TT") < dname.size ()) {
            Nexp.back () = 0;   // this means that there is no ttbar contamination taken from the MC.
            // if you want it comment the line and uncomment the following part.
            /*
               if(dname == string("TTbarEE") || dname == string("TTbarMM")){
               Nexp.back () = localSignXsection * BRChannel * localLumi * myCombinedMeas->GetSelEff(i) * myCombinedMeas->GetDatasets()[i].PreSelEfficiency();
               if(debug) cout<<"Ninit = (X)"<<localSignXsection<<" * (BR)"<<BRChannel<<" * (L)"<<localLumi<<" * (Eff)"<<myCombinedMeas->GetSelEff(i)<<" * (PreselEff)"<<myCombinedMeas->GetDatasets()[i].PreSelEfficiency()<<" = "<<Nexp.back()<<endl;
               }
               if(dname == string("TTbarEM")){
               Nexp.back () = localSignXsection * 2 * BRChannel * localLumi * myCombinedMeas->GetSelEff(i) * myCombinedMeas->GetDatasets()[i].PreSelEfficiency();
               if(debug) cout<<"Ninit = (X)"<<localSignXsection<<" * (BR)"<<2*BRChannel<<" * (L)"<<localLumi<<" * (Eff)"<<myCombinedMeas->GetSelEff(i)<<" * (PreselEff)"<<myCombinedMeas->GetDatasets()[i].PreSelEfficiency()<<" = "<<Nexp.back()<<endl;
               }
               if(dname == string("TTbarOther")){
               Nexp.back () = localSignXsection * (1 - 4* BRChannel) * localLumi * myCombinedMeas->GetSelEff(i) * myCombinedMeas->GetDatasets()[i].PreSelEfficiency();
               if(debug) cout<<"Ninit = (X)"<<localSignXsection<<" * (BR)"<<(1-4*BRChannel)<<" * (L)"<<localLumi<<" * (Eff)"<<myCombinedMeas->GetSelEff(i)<<" * (PreselEff)"<<myCombinedMeas->GetDatasets()[i].PreSelEfficiency()<<" = "<<Nexp.back()<<endl;
               }
             */
          }
          else {
            Nexp.back () =
              (slope[iter][0] * par[iter] + myCombinedMeas->GetChannels ()[c]->GetMCXsectionUncertainty ().GetParameters (i)[0]) * localLumi * myCombinedMeas->GetChannels ()[c]->GetSelEff (i) *
              myCombinedMeas->GetChannels ()[c]->GetDatasets ()[i].PreSelEfficiency ();
            if (debug)
              cout << "Ninit = (X)" << (slope[iter][0] * par[iter] +
                                        myCombinedMeas->GetChannels ()[c]->GetMCXsectionUncertainty ().
                                        GetParameters (i)[0]) << " ( = " << slope[iter][0] << " * " << par[iter] << " + " << myCombinedMeas->GetChannels ()[c]->GetMCXsectionUncertainty ().
                GetParameters (i)[0] << " ) " << " * (L)" << localLumi << " * (Eff)" << myCombinedMeas->GetChannels ()[c]->GetSelEff (i) << " * (PreselEff)" << myCombinedMeas->GetChannels ()[c]->
                GetDatasets ()[i].PreSelEfficiency () << " = " << Nexp.back () << endl;
          }
        }
        iter++;
      }

      //Compute the uncertainties terms which are not the cross-section
      for (unsigned int j = 0; j < myCombinedMeas->GetChannels ()[c]->GetUncertainties ().size (); j++) {
        if (debug)
          cout << " For uncertainty: " << myCombinedMeas->GetChannels ()[c]->GetUncertainties ()[j].GetName () << " Factor = ";
        if (myCombinedMeas->GetDatasets ()[i].Name () == myCombinedMeas->GetChannels ()[c]->GetSignalDatasetName ()) {
          Nexp.back () *= (slope[j + offset][i] * par[j + offset] + myCombinedMeas->GetChannels ()[c]->GetUncertainties ()[j].GetParameters (i)[0]);    // linear parametrisation which can be asymetric (slope depend on signe of par[]) 
          if (debug)
            cout << (slope[j + offset][i] * par[j + offset] + myCombinedMeas->GetChannels ()[c]->GetUncertainties ()[j].GetParameters (i)[0]) << endl;
        }
        else {
          string dname = myCombinedMeas->GetDatasets ()[i].Name ();
          if (dname.find ("TT") >= 0 && dname.find ("TT") < dname.size ()) {
            Nexp.back () *= (slope[j + offset][i] * par[j + offset] + myCombinedMeas->GetChannels ()[c]->GetUncertainties ()[j].GetParameters (i)[0]);  // linear parametrisation which can be asymetric (slope depend on signe of par[]) 
            if (debug)
              cout << (slope[j + offset][i] * par[j + offset] + myCombinedMeas->GetChannels ()[c]->GetUncertainties ()[j].GetParameters (i)[0]) << endl;
          }
          else {
            Nexp.back () *= (slope[j + offset][i] * par[j + offset] + myCombinedMeas->GetChannels ()[c]->GetUncertainties ()[j].GetParameters (i)[0]);  // linear parametrisation which can be asymetric (slope depend on signe of par[])
            if (debug)
              cout << (slope[j + offset][i] * par[j + offset] + myCombinedMeas->GetChannels ()[c]->GetUncertainties ()[j].GetParameters (i)[0]) << endl;
          }
        }
        if (debug)
          cout << slope[j + offset][i] << " * " << par[j + offset] << " =  " << slope[j + offset][i] * par[j +
                                                                                                           offset] << " + " << myCombinedMeas->GetChannels ()[c]->GetUncertainties ()[j].
            GetParameters (i)[0] << endl;
      }
      if (debug)
        cout << "Nend = " << Nexp.back () << endl;
    }
    cout << "END OF A LOOP" << endl;
    for (unsigned int i = 0; i < Nexp.size (); i++)
      NexpTot[c] += Nexp[i];
    //cout<<i<<" "<<Nexp[i]<<endl;
    //cout<<NexpTot.size()<<endl;
    cout << "before delete" << endl;
    for (int z = 0; z < (int) (myCombinedMeas->GetUncertainties ().size () + offset); z++)
      delete[]slope[z];
    delete[]slope;
    cout << "after delete" << endl;
  }                             //End of the loop over the channels
  cout << "there" << endl;


  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //  Computation of the LnLikelihood
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  //Compute the total nof events expected
  float NexpCombinedTot = 0;
  for (unsigned int w = 0; w < myCombinedMeas->GetChannels ().size (); w++)
    NexpCombinedTot += NexpTot[w];

  cout << "NexpCombinedTot " << NexpCombinedTot << endl;
  if (NexpCombinedTot > 0) {
    if (debug) {
      cout << "--------------------------" << endl;
      cout << "    Computation of LnLike " << endl;
      cout << "--------------------------" << endl;
    }
    //Change the formula // WARNING
    //loop over the channels
    if (debug)
      cout << "LogLikelihood = ";
    for (unsigned c = 0; c < myCombinedMeas->GetChannels ().size (); c++) {
      //poisson
      LnLikelihood = myCombinedMeas->GetChannels ()[c]->GetNobs () * TMath::Log (NexpTot[c]) - NexpTot[c];
      if (debug)
        cout << "[Channel=" << myCombinedMeas->GetChannels ()[c]->GetName () << "] -( (Nobs) " << myCombinedMeas->GetChannels ()[c]->
          GetNobs () << " * (log(Nexp)) " << TMath::Log (NexpTot[c]) << " - (Nexp) " << NexpTot[c];
    }
    //Only if Lumi is a nuisance parameter
    LnLikelihood -= 0.5 * (par[1] - 0.) * (par[1] - 0.) / (1. * 1.);
    if (debug)
      cout << "- 0.5 * " << par[1] << "^2 (for Lumi)";
    //here: add terms about Xsection uncertainty or the #evts from DD estimation
    //MISSING 
    //for (unsigned int i = 0; i < myCombinedMeas->GetUncertainties().size (); i++) {
    //for (int i = 2; i < myCombinedMeas->GetNofParam()+1; i++) {
    for (int i = 2; i < myCombinedMeas->GetNofParam (); i++) {
      LnLikelihood -= 0.5 * (par[i] - 0.) * (par[i] - 0.) / (1. * 1.);
      if (debug)
        cout << "- 0.5 * " << par[i] << "^2 ";  //("<<myCombinedMeas->GetUncertainties()[i].GetName()<<")";
      //LnLikelihood -= 0.5 * (par[i + offset] - 0.) * (par[i + offset] - 0.) / (1. * 1.);
      //if(debug) cout<<"- 0.5 * "<<par[i+offset]<<"^2 ";//("<<myCombinedMeas->GetUncertainties()[i].GetName()<<")";
      //if(debug) cout<<"- 0.5 * "<<par[i+offset]<<"^2 ("<<myCombinedMeas->GetUncertainties()[i].GetName()<<")";
    }
    if (debug)
      cout << endl;

    /*
       LnLikelihood = myCombinedMeas->GetNobs () * TMath::Log (NexpTot) - NexpTot;
       if (debug)
       cout << "LnLike = -( (Nobs) " << myCombinedMeas->GetNobs () << " * (log(Nexp)) " << TMath::Log (NexpTot) << " - (Nexp) " << NexpTot;
       LnLikelihood -= 0.5 * (par[1] - 0.) * (par[1] - 0.) / (1. * 1.);
       if (debug)
       cout << "- 0.5 * " << par[1] << "^2 (for Lumi)";
       //here: add terms about Xsection uncertainty or the #evts from DD estimation
       //MISSING 
       //for (unsigned int i = 0; i < myCombinedMeas->GetUncertainties().size (); i++) {
       //for (int i = 2; i < myCombinedMeas->GetNofParam()+1; i++) {
       for (int i = 2; i < myCombinedMeas->GetNofParam (); i++) {
       LnLikelihood -= 0.5 * (par[i] - 0.) * (par[i] - 0.) / (1. * 1.);
       if (debug)
       cout << "- 0.5 * " << par[i] << "^2 ";   //("<<myCombinedMeas->GetUncertainties()[i].GetName()<<")";
       //LnLikelihood -= 0.5 * (par[i + offset] - 0.) * (par[i + offset] - 0.) / (1. * 1.);
       //if(debug) cout<<"- 0.5 * "<<par[i+offset]<<"^2 ";//("<<myCombinedMeas->GetUncertainties()[i].GetName()<<")";
       //if(debug) cout<<"- 0.5 * "<<par[i+offset]<<"^2 ("<<myCombinedMeas->GetUncertainties()[i].GetName()<<")";
       }
       if (debug)
       cout << endl;
     */
  }
  else
    cout << "Nof combined expected events is negative" << endl;
  LnLikelihood *= -1.;
  f = LnLikelihood;
  if (debug) {
    //cout << " Diff evts ( data - expt) = " << myCombinedMeas->GetNobs () - NexpTot << " = " << myCombinedMeas->GetNobs () << " - " << -NexpTot << endl;
    cout << " LnLikelihood = " << f << endl;
    cout << "--------------------------" << endl;
  }
  delete[]NexpTot;
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

/*
void CombinedDiLepXsectionMeas::PassTheSelection(int idataset){
        if(idataset>=0 && idataset<(int)datasets.size()){
                SelEvents[idataset]++;
        }
        else cout<<"DiLepXSectionMeas::PassTheSelection the iDataset parameter is wrong"<<endl;
}


void CombinedDiLepXsectionMeas::PassTheSelection(int idataset, float weight){
        if(idataset>=0 && idataset<(int)datasets.size()){
                SelEvents[idataset]+= weight; 
        }
        else cout<<"DiLepXSectionMeas::PassTheSelection the iDataset parameter is wrong"<<endl;
}

void CombinedDiLepXsectionMeas::PrintSelEff(){
        cout<<" --------------------------------"<<endl;
        cout<<"   Selection efficiencies        "<<endl;
        cout<<" --------------------------------"<<endl;
        for(unsigned int i=0;i<datasets.size();i++){
                cout<<datasets[i].Name()<<" Eff = "<<GetSelEff(i)<<endl;
        }
        cout<<" --------------------------------"<<endl;
}

void CombinedDiLepXsectionMeas::SetSelEff(int idataset, float value){
        if(idataset>=0 && idataset<(int)datasets.size()){
                SelEff[idataset] = value;
        }
        else cout<<"DiLepXSectionMeas::SetSelEff the iDataset parameter is wrong"<<endl;
}

void CombinedDiLepXsectionMeas::SetSelEff(string DatasetName, float value){
        int idataset = -1;
        for(unsigned int i=0;i<datasets.size();i++){
                if(datasets[i].Name() == DatasetName){
                        idataset = i;
                        break;
                }
        }
        if(idataset>=0 && idataset<(int)datasets.size()){
                SelEff[idataset] = value;
        }
        else cout<<"DiLepXSectionMeas::SetSelEff the iDataset parameter is wrong"<<endl;
}

float CombinedDiLepXsectionMeas::GetSelEff(int idataset) const{
        if(idataset>=0 && idataset<(int)datasets.size()){
                return SelEff[idataset];
        }
        else cout<<"DiLepXSectionMeas::GetSelEff the iDataset parameter is wrong"<<endl;
        return 1.;
}
*/

vector < float >CombinedDiLepXsectionMeas::GetMCXsection () const
{
  vector < float >out;
  for (unsigned int i = 0; i < datasets.size (); i++) {
    out.push_back (GetMCXsection (i));
  }
  return out;
}

float CombinedDiLepXsectionMeas::GetMCXsection (int idataset) const
{
  if (idataset >= 0 && idataset < (int) datasets.size ()) {
    return Xsection.GetParameters (idataset)[0];
  }
  cout << "CombinedDiLepXsectionMeas::GetMCXsection:  idataset is a wrong paramter" << endl;
  return -999.;
}

/*
void CombinedDiLepXsectionMeas::Compute(){
        for(unsigned int i=0;i<uncertainties.size();i++){
                cout<<"UncertaintY: "<<uncertainties[i].GetName()<<endl;
                uncertainties[i].Compute();
        }
        for(unsigned int i=0;i<SelEff.size();i++){
                SelEff[i] = (float)SelEvents[i]/InitNofEvents[i];               
        }
}
*/
/*
void CombinedDiLepXsectionMeas::EfficienciesTable(ofstream& fout){
        fout<<"\\begin{table}"<<endl;
        fout<<"\\centering"<<endl;
        fout<<"\\begin{tabular}{|c|";
        for(unsigned int d=0;d<datasets.size();d++) fout<<"c";
        fout<<"|}"<<endl;
        fout<<"\\hline"<<endl;
        fout<<"Uncertainties & ";
        for(unsigned int d=0;d<datasets.size();d++){
                fout<<datasets[d].Name();
                if(d<datasets.size()-1) fout<<" & ";
                else fout<<"\\\\"<<endl;
        }
        for(unsigned int i=0;i<uncertainties.size();i++){
                fout<<uncertainties[i].GetName()<< " & "; 
                for(unsigned int d=0;d<datasets.size();d++){
                        fout<<uncertainties[i].GetEfficiencies(d)[1]<<" / "<<uncertainties[i].GetEfficiencies(d)[2]<<endl;      
                        if(d<datasets.size()-1) fout<<" & ";
                        else fout<<"\\\\"<<endl;
                }
        }
        fout<<"\\hline"<<endl;
        fout<<"\\end{tabular}"<<endl;
        fout<<"\\caption{ Ratio table for channel "<< channel<<" }"<<endl;
        fout<<"\\label{tab:RatioTable"<<channel<<"}"<<endl;
        fout<<"\\end{table}"<<endl;
}
*/

float CombinedDiLepXsectionMeas::IsolateSystError (float statError, float combinedError) const
{
  return (sqrt (combinedError * combinedError - statError * statError));
}

void CombinedDiLepXsectionMeas::UncertaintiesTable (ofstream & fout, bool SkipOneByOne)
{
  fout << "\\begin{table}" << endl;
  fout << "\\centering" << endl;
  fout << "\\begin{tabular}{|c|c|c|}";
  fout << "\\hline" << endl;
  fout << "Stat &" << XsectionEstimStatError.first << " (" << XsectionEstimStatError.first / XsectionEstim *
    100. << "\\%) & " << XsectionEstimStatError.second << " (" << XsectionEstimStatError.second / XsectionEstim * 100. << "\\%)\\\\" << endl;
  if (!SkipOneByOne) {
    for (unsigned int i = 0; i < SysName.size (); i++) {
      fout << SysName[i] << " & ";
      fout << XsectionEstimSysError[i].first << " (" << XsectionEstimSysError[i].first / XsectionEstim *
        100. << "\\%) & " << XsectionEstimSysError[i].second << " (" << XsectionEstimSysError[i].second / XsectionEstim * 100. << "\\%)\\\\" << endl;
      //fout<<IsolateSystError(XsectionEstimStatError.first,XsectionEstimSysError[i].first)<<" ("<<IsolateSystError(XsectionEstimStatError.first,XsectionEstimSysError[i].first)/XsectionEstim*100.<<"\\%) & "<<IsolateSystError(XsectionEstimStatError.second,XsectionEstimSysError[i].second)<<" ("<<IsolateSystError(XsectionEstimStatError.second,XsectionEstimSysError[i].second)/XsectionEstim*100.<<"\\%)\\\\"<<endl;
    }
  }
  fout << "All &" << XsectionEstimAllError.first << " (" << XsectionEstimAllError.first / XsectionEstim * 100. << "\\%) & " << XsectionEstimAllError.second << " (" << XsectionEstimAllError.second /
    XsectionEstim * 100. << "\\%)\\\\" << endl;
  fout << "All &" << IsolateSystError (XsectionEstimStatError.first, XsectionEstimAllError.first) << " (" << IsolateSystError (XsectionEstimStatError.first,
                                                                                                                               XsectionEstimAllError.first) / XsectionEstim *
    100. << "\\%) & " << IsolateSystError (XsectionEstimStatError.second, XsectionEstimAllError.second) << " (" << IsolateSystError (XsectionEstimStatError.second,
                                                                                                                                     XsectionEstimAllError.second) / XsectionEstim *
    100. << "\\%)\\\\" << endl;
  fout << "\\hline" << endl;
  fout << "\\end{tabular}" << endl;
  fout << "\\caption{ Uncertainties table for combined channels }" << endl;
  fout << "\\label{tab:UncerTableCombinedChannels}" << endl;
  fout << "\\end{table}" << endl;
}

void CombinedDiLepXsectionMeas::MinimizationTable (ofstream & fout)
{
  fout << "\\begin{table}" << endl;
  fout << "\\centering" << endl;
  fout << "\\begin{tabular}{|c|c|c|c|c|}";
  fout << "\\hline" << endl;
  fout << "Parameter & Value & Error & NegErr & PosErr \\\\" << endl;
  fout << "\\hline" << endl;
  for (unsigned int i = 0; i < ParamNames.size (); i++) {
    fout << ParamNames[i] << " & " << Param_Min[i] << " & " << ErrParam_Min[i] << " & " << ErrNegParam_Min[i] << " & " << ErrPosParam_Min[i] << "\\\\" << endl;
  }
  fout << "\\hline" << endl;
  fout << "\\end{tabular}" << endl;
  fout << "\\caption{ Minimization table for combined channels }" << endl;
  fout << "\\label{tab:MinTableCombinedChannels}" << endl;
  fout << "\\end{table}" << endl;
}

void CombinedDiLepXsectionMeas::XExtraction (int NofPoints, double MinXforLogLPlot, double MaxXforLogLPlot, int verbosity, bool SkipOneByOne)
{

  //the following line is extremely important.
  //this allow a correct behaviour of the code event is many objects CombinedDiLepXsectionMeas are instanciated.
  myCombinedMeas = this;

  bool verbose = false;
  bool verboseMax = false;
  if (verbosity > 0)
    verbose = true;
  if (verbosity > 1)
    verboseMax = true;
  if (verbose) {
    cout << "####################################" << endl;
    cout << "   Cross-Section computation        " << endl;
    cout << "####################################" << endl;
  }

  //Reinitialization needed if the method is called many times !
  ParamNames.clear ();

  //Replace by something else ... //WARNING
  //SetNobs(NObs);
  MinFCN = 0.;
  double MinlogL0 = 0.;

  cout << "Initate hisot" << endl;

  // Give main input parameters
  bool PlotLogCurve = true;     // to plot loglikelihood curves 

  ///////////////////
  // Initialisation
  ///////////////////
  //vectors for parameters
  vector < bool > fixParam;
  vector < double >ParamValues;
  //do not change the following order !!
  ParamValues.push_back (170.); //ttbar-xsection: init-value
  ParamNames.push_back (string ("Xsection-ttbar"));
  //ParamValues.push_back(Lumi);//lumi
  if (isLumiASyst) {
    ParamValues.push_back (0.); //lumi
    ParamNames.push_back (string ("Luminosity"));
  }

  //Way to store the paramters:
  // 0: ttbar cross-section
  // 1: Lumi is it's a systematic
  // then: all cross-section of bkg level (DD) for a given channel
  // then: all systematics for a given channel
  // then: if some of the previous parameters are not common, store then bkg, then other syst channel 2 then the others ...
  // ie:   xsection, lumi, [bkg1, bkg2,...], [JES,btag,...], ([bkg_2,bkg_4,...], [syst_uncorr])_channel_2, (idem)_channel_3 ... 

  NofParam = 2;
  if (isLumiASyst)
    NofParam = 2;
  else
    NofParam = 1;
  cout << "NofDatasets: " << datasets.size () << endl;
  cout << "Channels: " << GetChannels ().size () << endl;
  cout << "DDEstim: " << GetChannels ()[0]->GetDDEstimation ().size () << endl;

  for (unsigned int d = 0; d < datasets.size (); d++) {
    cout << "Datasets " << d << endl;
    if (!datasets[d].isData () && datasets[d].Name ().find ("TT") > datasets[d].Name ().size ()) {
      cout << "there" << endl;
      cout << GetChannels ().size () << endl;
      cout << GetChannels ()[0]->GetDDEstimation ().size () << endl;
      cout << GetChannels ()[0]->GetDDEstimation ()[d].isASyst << endl;
      if (GetChannels ().size () > 0)
        cout << "ah" << endl;
      if (GetChannels ()[0]->GetDDEstimation ()[d].isASyst)
        cout << "ah" << endl;
      if (GetChannels ().size () > 0 && GetChannels ()[0]->GetDDEstimation ()[d].isASyst)
        cout << "ohoh" << endl;
      if (GetChannels ().size () > 0 && GetChannels ()[0]->GetDDEstimation ()[d].isASyst)
        cout << "ohoh" << endl;
      if (GetChannels ().size () > 0 && GetChannels ()[0]->GetDDEstimation ()[d].isASyst) {
        cout << "Iki" << endl;
        cout << "here" << endl;
        ParamValues.push_back (datasets[d].Xsection ());
        cout << "here" << endl;
        ParamNames.push_back (string ("Xsection") + datasets[d].Name ());
        cout << "here" << endl;
        NofParam++;
      }
      cout << "there after if" << endl;
    }
  }
  cout << "NofParam " << NofParam << endl;
  for (unsigned int i = 0; i < uncertainties.size (); i++) {
    ParamValues.push_back (0);  //delta-parameter center around 0
    ParamNames.push_back (uncertainties[i].name);
  }
  NofParam += uncertainties.size ();

  cout << "NofParam " << NofParam << endl;

  //loop on over channels and check if syst are correlated ...
  if (GetChannels ().size () > 1) {
    for (unsigned int c = 1; c < GetChannels ().size (); c++) {
      for (unsigned int d = 0; d < datasets.size (); d++) {
        if (!datasets[d].isData () && datasets[d].Name ().find ("TT") > datasets[d].Name ().size ()) {
          if (GetChannels ()[c]->GetDDEstimation ()[d].isASyst && GetChannels ()[c]->GetDDEstimation ()[d].isDDEstimated) {
            ParamValues.push_back (datasets[d].Xsection ());
            ParamNames.push_back (string ("Xsection") + datasets[d].Name () + GetChannels ()[c]->GetName ());
            NofParam++;
          }
        }
      }
    }
  }
  cout << "NofParam " << NofParam << endl;

  for (int i = 0; i < NofParam; i++) {
    if (i == 0)
      fixParam.push_back (false);       // sigma_tt is free
    else
      fixParam.push_back (true);
  }
  cout << "finish" << endl;
  if ((int) fixParam.size () != NofParam || (int) ParamValues.size () != NofParam)
    cout << "XExtraction: nof parameters is incorrect" << endl;
  ///////////////////
  cout << "NofParam " << NofParam << endl;

  ////fix the luminosity
  //fixParam[1] = true;

  graphs = new TGraph *[NofParam + 2];

  double MinlogL = MinFCN;
  /*
     if (!doMCPseudoExp)
     NofPseudoExp = 1;
     double MinlogL = MinFCN;
     for (int k = 0; k < NofPseudoExp; ++k) {
     //Randomisation of the nof observed event if (MC-pseudo)
     if (doMCPseudoExp) {
     TRandom rand;
     double localN = rand.Poisson (Nobs);
     //Change \\WARNING
     //SetNobs(localN);
     }
   */
  if (verbose) {
    std::cout << "======= First step: Minimization of LogL, Sys OFF, one (=sigma_tt) parameter =======" << std::endl;
    //std::cout << "Number of events : " << Nobs << std::endl;
  }
  ComputeLikelihood (fixParam, ParamValues, true, false, verboseMax);
  cout << "End of computelikelihood" << endl;
  MinlogL0 = MinFCN;
  if (verbose) {
    for (int j = 0; j < NofParam; ++j) {
      std::cout << "Param[" << j << "] = " << Param[j] << " +/-" << ErrParam[j] << " (" << ErrNegParam[j] << "+" << ErrPosParam[j] << ")" << std::endl;
    }
    std::cout << "MinlogL " << MinlogL << std::endl;
  }
  //Plotting of the values coming from the minimisation
  Xseccentralvalue_wosys->Fill (Param[0]);
  Erreurparabolic_wosys->Fill (ErrParam[0]);
  Erreurneg_wosys->Fill (TMath::Abs (ErrNegParam[0]));
  Erreurpos_wosys->Fill (ErrPosParam[0]);
  Erreurmean_wosys->Fill ((ErrPosParam[0] + TMath::Abs (ErrNegParam[0])) / 2.);
  if (verbose)
    std::cout << "====================================================================================" << std::endl;
  if (PlotLogCurve) {
    Plot ("AP", NofPoints, MinXforLogLPlot, MaxXforLogLPlot, MinlogL0);
  }
  //return;

  if (verbose) {
    std::cout << "======= Second step: Minimization of LogL, Sys ON, all free parameters =======" << std::endl;
  }
  for (int i = 0; i < NofParam; i++) {
    fixParam[i] = false;
  }
  //fix the luminosity
  //fixParam[1] = true;
  ComputeLikelihood (fixParam, ParamValues, false, true, verboseMax); // true: doCorrelationMatrix
  cout<<"End of Lik2"<<endl;
  //return;
  MinlogL0 = MinFCN;
  for (int j = 0; j < NofParam; ++j) {
    if (verbose)
      std::cout << "Param[" << j << "] = " << Param[j] << " +/-" << ErrParam[j] << " (" << ErrNegParam[j] << "+" << ErrPosParam[j] << ")" << std::endl;
    //important: update the values according to the previous minimization where all paramters are free
    ParamValues[j] = Param[j];
  }
  if (verbose)
    std::cout << "MinlogL " << MinlogL << std::endl;
  XsectionEstim = Param[0];     //fill the estimation
  Xseccentralvalue_wsys->Fill (Param[0]);
  Erreurparabolic_wsys->Fill (ErrParam[0]);
  Erreurneg_wsys->Fill (TMath::Abs (ErrNegParam[0]));
  Erreurpos_wsys->Fill (ErrPosParam[0]);
  Erreurmean_wsys->Fill ((ErrPosParam[0] + TMath::Abs (ErrNegParam[0])) / 2.);
  if (verbose)
    std::cout << "====================================================================================" << std::endl;
  //}
  /////////////// Save the Parameters of the optimization
  for (int i = 0; i < NofParam; i++) {
    Param_Min[i] = Param[i];
    ErrParam_Min[i] = ErrParam[i];
    ErrNegParam_Min[i] = ErrNegParam[i];
    ErrPosParam_Min[i] = ErrPosParam[i];
  }

  if (!PlotLogCurve) {
    Xseccentralvalue_wosys->Draw ();
  }


  if (PlotLogCurve) {
    for (int j = 0; j < NofPoints; ++j) {
      X_PLR[j] = 0.;
      Y_PLR[j] = 0.;
    }

    //save the ParamValues which minimize the likelihood when all paramters are free
    vector < double >ParamValuesMin;
    for (unsigned int i = 0; i < ParamValues.size (); i++)
      ParamValuesMin.push_back (ParamValues[i]);

    ////////////////////////////////////////////////
    ///   Scan of the Xsection: Profile Likelihood
    ////////////////////////////////////////////////                            
    //Compute the PLR for each uncertainty + stat only (u==0) + all uncertainties combined (u==NofParam+1)
        cout<<"just there"<<endl;
    //return;
    for (int u = 0; u < NofParam; u++) {
      cout << "begin" << endl;
      if (SkipOneByOne == true && u < (NofParam - 1))
        continue;
      //for(int u=0;u<NofParam+1;u++){
      for (int i = 1; i < NofPoints + 1; i++) {
        //Compute the Xsection for this iteration 
        double Sig = MinXforLogLPlot + (MaxXforLogLPlot - MinXforLogLPlot) * i / NofPoints;
        if (verbose)
          std::cout << "======= Third step: Minimization of LogL, sigma fixed parameter =======" << std::endl;
        if (u < NofParam - 1) {
          for (int p = 0; p < NofParam; p++)
            fixParam[p] = true;
          fixParam[u + 1] = false;      //for u==0; statistic uncertainty only
        }
        else
          for (int p = 0; p < NofParam; p++)
            fixParam[p] = false;        //all uncertainties considered
        //Simga is fixed
        fixParam[0] = true;
        ParamValues[0] = Sig;
        ComputeLikelihood (fixParam, ParamValues, false, false, verboseMax);
        //ComputeLikelihood (fixParam, ParamValuesMin);
        //MinlogL0 = MinFCN;
        cout << "MinLogL: " << MinlogL << endl;
        if (verbose) {
          for (int j = 0; j < NofParam; j++) {
            std::cout << "Param[" << j << "] = " << Param[j] << " +/-" << ErrParam[j] << " (" << ErrNegParam[j] << "+" << ErrPosParam[j] << ")" << std::endl;
          }
          std::cout << "MinlogL " << MinlogL << std::endl;
          std::cout << "=======================================================================" << std::endl;
          std::cout << "======= Fourth step: Computation of Profile Likelihood Ratio curve =======" << std::endl;
        }
        double P[NofParam];
        for (int j = 0; j < NofParam; j++) {
          P[j] = Param[j];
        }
        X_PLR[i] = Sig;
        Y_PLR[i] = XsectionFCN (P) - MinlogL0;
        cout << "LogLik " << Y_PLR[i] << " = " << XsectionFCN (P) << " - " << MinlogL0 << endl;
        std::cout << "=============================================================================" << std::endl;
      }
      string sysname;
      if (u >= 0 && u < (NofParam - 1))
        sysname = SysName[u];
      if (u == NofParam - 1)
        sysname = string ("all");
      Plot_PLR ("Same", NofPoints, MinXforLogLPlot, MaxXforLogLPlot, X_PLR, Y_PLR, graphs[u], 2 + u, sysname);
      bool first = true;
      float ErrMin = 0.;
      float ErrMax = 0.;
      //int NPSearch = 100;
      for (int i = 1; i < NofPoints; i++) {
        if (Y_PLR[i] < 0.5 && first) {
          //linear approach, need enought point to be exact
          ///*
          float a = (Y_PLR[i] - Y_PLR[i - 1]) / (X_PLR[i] - X_PLR[i - 1]);
          float b = Y_PLR[i - 1] - a * X_PLR[i - 1];
          ErrMin = (0.5 - b) / a;
          //*/
          first = false;
          //fit the graph:
          /*
             TF1* f1 = new TF1("func1","pol3",X_PLR[i-1],XsectionEstim);
             graphs[u]->Fit(f1);
             c->cd();
             f1->Draw("same");
             for(int x=0;x<NPSearch;x++){
             float value = x*(X_PLR[i]-X_PLR[i-1])+X_PLR[i-1];
             float out = f1->Eval(value);
             if(out<0.5){ 
             //ErrMin = out;
             break;
             }
             }
           */
        }
        if (Y_PLR[i] > 0.5 && !first) {
          //linear approach, need enought point to be exact
          ///*
          float a = (Y_PLR[i] - Y_PLR[i - 1]) / (X_PLR[i] - X_PLR[i - 1]);
          float b = Y_PLR[i - 1] - a * X_PLR[i - 1];
          ErrMax = (0.5 - b) / a;
          //*/
          //fit the graph:
          /*
             TF1* f2 = new TF1("func2","pol3",XsectionEstim,X_PLR[i+1]);
             f2->SetLineColor(2);
             f2->SetLineWidth(2);
             graphs[u]->Fit(f2);
             c->cd();
             f2->Draw("same");
             //graphs[u]->Draw("same");
             c->Update();
             c->Modified();
             for(int x=0;x<NPSearch;x++){
             float value = x*(X_PLR[i]-X_PLR[i-1])+X_PLR[i-1];
             float out = f2->Eval(value);
             if(out>0.5){ 
             //ErrMax = out;
             break;
             }
             }
           */
          break;
        }
      }
      if (u < NofParam - 1) {
        double err1 = sqrt ((ErrMin - XsectionEstim) * (ErrMin - XsectionEstim) - (XsectionEstimStatError.first) * (XsectionEstimStatError.first));
        double err2 = sqrt ((ErrMax - XsectionEstim) * (ErrMax - XsectionEstim) - (XsectionEstimStatError.second) * (XsectionEstimStatError.second));
        XsectionEstimSysError.push_back (pair < double, double >(err1, err2));
      }
      else
        XsectionEstimAllError = pair < double, double >((ErrMin - XsectionEstim), (ErrMax - XsectionEstim));
    }
  }                             //PlotLogCurve
  cout<<"finish"<<endl;
}



void CombinedDiLepXsectionMeas::Write (TFile * fout)
{
  fout->cd ();
  if (!fout->cd ("Xsection"))
    fout->mkdir ("Xsection");
  TDirectory* dir1 = 0;
  TDirectory* dir2 = 0;
  dir1 = fout->GetDirectory("Xsection");
  dir2 = dir1->GetDirectory ("Combination");
  if(dir2 == 0 ) dir2 = dir1->mkdir("Combination"); 
  dir2->cd ();

  c->cd ();
  leg->Draw ();
  c->Write ();
  /*
     if(graphsDeclared){
     cout<<"there"<<endl;
     for(int i=0;i<NofParam;i++){ 
     //graphs[i]->Write();
     cout<<graphs[i+2]->GetName()<<endl;
     }
     }
   */
  if(StatGraph) StatGraph->Write();
  if(CorrelationMatrixHisto) CorrelationMatrixHisto->Write();
  Xseccentralvalue_wosys->Write ();
  Erreurparabolic_wosys->Write ();
  Erreurmean_wosys->Write ();
  Erreurneg_wosys->Write ();
  Erreurpos_wosys->Write ();
  Xseccentralvalue_wsys->Write ();
  Erreurparabolic_wsys->Write ();
  Erreurmean_wsys->Write ();
  Erreurneg_wsys->Write ();
  Erreurpos_wsys->Write ();
}
