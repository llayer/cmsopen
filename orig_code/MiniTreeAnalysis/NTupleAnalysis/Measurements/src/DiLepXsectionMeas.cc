#include "../interface/DiLepXsectionMeas.h"


//important: it's needed to have the FCN working
DiLepXsectionMeas *myMeas = 0;


DiLepXsectionMeas::DiLepXsectionMeas (string Channel, string signalDatasetName, float br)
{
  channel = Channel;
  SignalDatasetName = signalDatasetName;
  BRChannel = br;

  Lumi = 1.;
  LumiError = 0;
  isLumiASyst = false;

  Nobs = 0;
  XsectionEstim = 0.;
  XsectionEstimStat = 0.;
  XsectionEstimWithAllSyst = 0.;
  ExpectedXsection = 0.;
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
  Y_PLR = new double[MaxNofPoints];
  for (int i = 0; i < MaxNofPoints; i++) {
    X_PLR[i] = 0;
    Y_PLR[i] = 0;
  }

  //--- histos and display ---//
  string cname = "DiLepXsection_" + Channel;
  c = new TCanvas (cname.c_str ());
  c->SetFillColor (10);
  c->SetFillStyle (4000);
  leg = new TLegend (0.6, 0.7, 0.8, 0.9);
  histo = 0;
  CorrelationMatrixHisto = 0;
  StatGraph = 0;
  graphsDeclared = false;
  graphs = 0;
  Xseccentralvalue_wosys = new TH1F ("Xseccentralvalue_wosys", "", 50, 0., 300.);
  Erreurmean_wosys = new TH1F ("Erreurmean_wosys", "", 50, 0., 100.);
  Erreurneg_wosys = new TH1F ("Erreurneg_wosys", "", 50, 0., 100.);
  Erreurpos_wosys = new TH1F ("Erreurpos_wosys", "", 50, 0., 100.);
  PullErreurmean_wosys = new TH1F ("PullErreurmean_wosys", "", 50, -5., 5.);
  PullErreurneg_wosys = new TH1F ("PullErreurneg_wosys", "", 50, -5., 5.);
  PullErreurpos_wosys = new TH1F ("PullErreurpos_wosys", "", 50, -5., 5.);
  PullErreur2cases_wosys = new TH1F ("PullErreur2cases_wosys", "", 50, -5., 5.);
  Xseccentralvalue_wsys = new TH1F ("Xseccentralvalue_wsys", "", 50, 0., 300.);
  Erreurmean_wsys = new TH1F ("Erreurmean_wsys", "", 50, 0., 100.);
  Erreurneg_wsys = new TH1F ("Erreurneg_wsys", "", 50, 0., 100.);
  Erreurpos_wsys = new TH1F ("Erreurpos_wsys", "", 50, 0., 100.);
  PullErreurmean_wsys = new TH1F ("PullErreurmean_wsys", "", 50, -5., 5.);
  PullErreurneg_wsys = new TH1F ("PullErreurneg_wsys", "", 50, -5., 5.);
  PullErreurpos_wsys = new TH1F ("PullErreurpos_wsys", "", 50, -5., 5.);
  PullErreur2cases_wsys = new TH1F ("PullErreur2cases_wsys", "", 50, -5., 5.);
  Ymin_plot = 0;

  offset = 0;
  paramXsection = 0;
  NofParam = 0;

  ParameterHistoDeclared = false;
  ParametersHisto = 0;
  
  time_1 = 0;
  time_2 = 0;
  time_3 = 0;
  time_4 = 0;

  //important: it's needed to have the FCN working
  myMeas = this;
}



DiLepXsectionMeas::DiLepXsectionMeas (const DiLepXsectionMeas & meas)
{

  bool graphsDeclared;

  channel = meas.channel;
  SignalDatasetName = meas.SignalDatasetName;
  BRChannel = meas.BRChannel;

  InitNofEvents = meas.InitNofEvents;
  SelEvents = meas.SelEvents;
  SelEff = meas.SelEff;
  datasets = meas.datasets;
  DDEstim = meas.DDEstim;
  uncertainties = meas.uncertainties;
  Xsection = meas.Xsection;
  Lumi = meas.Lumi;
  LumiError = meas.LumiError;
  isLumiASyst = meas.isLumiASyst;

  Nobs = meas.Nobs;
  MinFCN = meas.MinFCN;
  XsectionEstim = meas.XsectionEstim;
  XsectionEstimStat = meas.XsectionEstimStat;
  XsectionEstimWithAllSyst = meas.XsectionEstimWithAllSyst;
  NofParam = meas.NofParam;
  offset = meas.offset;
  paramXsection = meas.paramXsection;
  SizeOfParam = meas.SizeOfParam;
  ParamNames = meas.ParamNames;

  Param = new double[SizeOfParam];
  ErrParam = new double[SizeOfParam];
  ErrNegParam = new double[SizeOfParam];
  ErrPosParam = new double[SizeOfParam];
  Param_Min = new double[SizeOfParam];
  ErrParam_Min = new double[SizeOfParam];
  ErrNegParam_Min = new double[SizeOfParam];
  ErrPosParam_Min = new double[SizeOfParam];
  for (int i = 0; i < SizeOfParam; i++) {
    Param[i] = meas.Param[i];
    ErrParam[i] = meas.ErrParam[i];
    ErrNegParam[i] = meas.ErrNegParam[i];
    ErrPosParam[i] = meas.ErrPosParam[i];
    Param_Min[i] = meas.Param_Min[i];
    ErrParam_Min[i] = meas.ErrParam_Min[i];
    ErrNegParam_Min[i] = meas.ErrNegParam_Min[i];
    ErrPosParam_Min[i] = meas.ErrPosParam_Min[i];
  }
  MaxNofPoints = meas.MaxNofPoints;
  X_PLR = new double[MaxNofPoints];
  Y_PLR = new double[MaxNofPoints];
  for (int i = 0; i < MaxNofPoints; i++) {
    X_PLR[i] = meas.X_PLR[i];
    Y_PLR[i] = meas.Y_PLR[i];
  }
  Ymin_plot = meas.Ymin_plot;
  if (meas.c)
    c = (TCanvas *) meas.c->Clone ();
  else
    c = 0;
  if (meas.leg)
    leg = (TLegend *) meas.leg->Clone ();
  else
    leg = 0;
  if (meas.histo)
    histo = (TH2F *) meas.histo->Clone ();
  else
    histo = 0;
  if (meas.CorrelationMatrixHisto)
    CorrelationMatrixHisto = (TH2F *) meas.CorrelationMatrixHisto->Clone ();
  else
    CorrelationMatrixHisto = 0;
  if (meas.StatGraph)
    StatGraph = (TGraph *) meas.StatGraph->Clone ();
  else
    StatGraph = 0;
  graphsDeclared = meas.graphsDeclared;
  //graphs = meas.graphs
  if (graphsDeclared) {
    graphs = new TGraph *[NofParam + 2];
    //for(int i=0;i<NofParam+2;i++) if(meas.graphs[i]!=0) graphs[i] = (TGraph*) (meas.graphs[i]->Clone());
  }
  Xseccentralvalue_wosys = (TH1F *) meas.Xseccentralvalue_wosys->Clone ();
  Erreurmean_wosys = (TH1F *) meas.Erreurmean_wosys->Clone ();
  Erreurneg_wosys = (TH1F *) meas.Erreurneg_wosys->Clone ();
  Erreurpos_wosys = (TH1F *) meas.Erreurpos_wosys->Clone ();
  PullErreurmean_wosys = (TH1F *) meas.PullErreurmean_wosys->Clone ();
  PullErreurneg_wosys = (TH1F *) meas.PullErreurneg_wosys->Clone ();
  PullErreurpos_wosys = (TH1F *) meas.PullErreurpos_wosys->Clone ();
  PullErreur2cases_wosys = (TH1F*) meas.PullErreur2cases_wosys->Clone();
  Xseccentralvalue_wsys = (TH1F *) meas.Xseccentralvalue_wsys->Clone ();
  Erreurmean_wsys = (TH1F *) meas.Erreurmean_wsys->Clone ();
  Erreurneg_wsys = (TH1F *) meas.Erreurneg_wsys->Clone ();
  Erreurpos_wsys = (TH1F *) meas.Erreurpos_wsys->Clone ();
  PullErreurmean_wsys = (TH1F *) meas.PullErreurmean_wsys->Clone ();
  PullErreurneg_wsys = (TH1F *) meas.PullErreurneg_wsys->Clone ();
  PullErreurpos_wsys = (TH1F *) meas.PullErreurpos_wsys->Clone ();
  PullErreur2cases_wsys = (TH1F*) meas.PullErreur2cases_wsys->Clone();
  ///////////////////////////////////////////

  //ParameterHistoDeclared = false; 
  //ParametersHisto = 0;       

  SysName = meas.SysName;
  XsectionEstimSysError = meas.XsectionEstimSysError;
  XsectionEstimStatError = meas.XsectionEstimStatError;
  XsectionEstimAllError = meas.XsectionEstimAllError;

  NofEvtsContribMC = meas.NofEvtsContribMC;
  NofEvtsContribFitted = meas.NofEvtsContribFitted;
  
  time_1 = meas.time_1;
  time_2 = meas.time_2;
  time_3 = meas.time_3;
  time_4 = meas.time_4;

//important: it's needed to have the FCN working
  myMeas = this;
}

DiLepXsectionMeas::~DiLepXsectionMeas ()
{
  cout << "There-DiLep" << endl;
  delete[]Param;
  delete[]ErrParam;
  delete[]ErrNegParam;
  delete[]ErrPosParam;
  delete[]Param_Min;
  delete[]ErrParam_Min;
  delete[]ErrNegParam_Min;
  delete[]ErrPosParam_Min;
  delete[]X_PLR;
  delete[]Y_PLR;
  delete Xseccentralvalue_wosys;
  delete Erreurmean_wosys;
  delete Erreurneg_wosys;
  delete Erreurpos_wosys;
  delete PullErreurmean_wosys;
  delete PullErreurneg_wosys;
  delete PullErreurpos_wosys;
  delete PullErreur2cases_wosys;
  delete Xseccentralvalue_wsys;
  delete Erreurmean_wsys;
  //delete Erreurneg_wsys;
  //delete Erreurpos_wsys;
  delete PullErreurmean_wsys;
  //delete PullErreurneg_wsys;
  //delete PullErreurpos_wsys;
  //delete PullErreur2cases_wosys;
  //
  delete c;
  delete leg;
  delete histo;
  delete CorrelationMatrixHisto;
  delete StatGraph;
  if (graphsDeclared) {
    ///*
    if (graphs != 0) {
      for (int i = 0; i < NofParam + 2; i++) {
        //delete graphs[i];
      }
      delete[]graphs;
    }
    //*/
  }
  if (ParameterHistoDeclared) {
    for (int i = 0; i < NofParam; i++) 
      delete ParametersHisto[i];
    delete ParametersHisto;
  }
  //delete myMeas;
}

void DiLepXsectionMeas::SetLumi (float value, float absErr)
{
  Lumi = value;
  LumiError = absErr;
}

double DiLepXsectionMeas::GetLumi () const
{
  return Lumi;
}

double DiLepXsectionMeas::GetLumiError () const
{
  return LumiError;
}


void DiLepXsectionMeas::AddUncertainty (string name, float paramMean, float paramMinus, float paramPlus)
{
  AllDatasetUncertainties uncert (name);
  uncert.LoadDatasets (datasets);
  uncert.SetParameters (paramMean, paramMinus, paramPlus);
  uncertainties.push_back (uncert);
}

void DiLepXsectionMeas::AddUncertainty (string name, Syst SysValues)
{
  AllDatasetUncertainties uncert (name);
  uncert.LoadDatasets (datasets);
  uncert.SetParameters (SysValues.mean, SysValues.m1sigma, SysValues.p1sigma);
  uncertainties.push_back (uncert);
}

void DiLepXsectionMeas::AddUncertainty (string name, SystEffect SysValues)
{
  AllDatasetUncertainties uncert (name);
  uncert.LoadDatasets (datasets);
  uncert.SetParameters (SysValues.mean, SysValues.m1sigma, SysValues.p1sigma);
  uncertainties.push_back (uncert);
  LoadEfficiencies (uncertainties.size () - 1, SysValues.effmean, SysValues.effm1s, SysValues.effp1s);
}

void DiLepXsectionMeas::SetNobs (double nobs)
{
  Nobs = nobs;
}

double DiLepXsectionMeas::GetNobs () const
{
  return Nobs;
}

string DiLepXsectionMeas::GetSignalDatasetName () const
{
  return SignalDatasetName;
}

double DiLepXsectionMeas::GetBRChannel () const
{
  return BRChannel;
}

void DiLepXsectionMeas::LoadEfficiencies (int iUncertainty, float effmean, float effm1s, float effp1s)
{
  if (iUncertainty >= 0 && iUncertainty < (int) uncertainties.size ()) {
    //put the same systematic for all samples (1) except for ttbar...
    for (unsigned int i = 0; i < datasets.size (); i++) {
      if (datasets[i].Name ().find ("TTbar") < datasets[i].Name ().size ())
        uncertainties[iUncertainty].LoadEfficiencies (i, effmean, effm1s, effp1s);
      else
        uncertainties[iUncertainty].LoadEfficiencies (i, 1., 1., 1.);
    }
  }
}

void DiLepXsectionMeas::LoadDatasets (const vector < Dataset > &Datasets)
{
  datasets = Datasets;
  if (Lumi <= 0) {
    cerr << "DiLepXsectionMeas::The luminosity has to be provided before loading the datasets !" << endl;
    return;
  }
  for (unsigned int i = 0; i < datasets.size (); i++) {
    InitNofEvents.push_back (datasets[i].NofEvtsToRunOver () * datasets[i].NormFactor () * Lumi);
    SelEff.push_back (1.);
    SelEvents.push_back (0);
    if (datasets[i].Name () == SignalDatasetName) {
      ExpectedXsection = datasets[i].Xsection ();
    }
  }
  //Add the uncertainty on the Xsection as first one
  Xsection.SetName (string ("Xsection"));
  Xsection.LoadDatasets (datasets);
  DDEstim.clear ();
  for (unsigned int d = 0; d < datasets.size (); d++) {
    Xsection.SetParameters (d, datasets[d].Xsection (), (1 - datasets[d].GetCrossSectionError ().first) * datasets[d].Xsection (), (1 + datasets[d].GetCrossSectionError ().second) * datasets[d].Xsection ());
    Xsection.LoadEfficiencies (d, datasets[d].Xsection (), (1 - datasets[d].GetCrossSectionError ().first) * datasets[d].Xsection (), (1 + datasets[d].GetCrossSectionError ().second) * datasets[d].Xsection ());
    DDEstimation a;
    a.isDDEstimated = false;
    a.NofEvts = 0;
    a.ErrNeg = 0;
    a.ErrPos = 0;
    DDEstim.push_back (a);
  }

}


void DiLepXsectionMeas::Fill (int idataset, int iUncertainty, int icond, float weight)
{
  if (iUncertainty >= 0 && iUncertainty < (int) uncertainties.size ()) {
    if (idataset >= 0 && idataset < (int) datasets.size ()) {
      uncertainties[iUncertainty].Fill (idataset, icond, weight);
    }
    else
      cerr << "DiLepXSectionMeas::Fill the iDataset parameter is wrong" << endl;
  }
  else
    cerr << "DiLepXSectionMeas::Fill the iUncertainty parameter is wrong" << endl;
}

void DiLepXsectionMeas::Fill (int idataset, string sysName, int icond, float weight)
{
  int iUncertainty = -1;
  for (unsigned int i = 0; i < uncertainties.size (); i++) {
    if (uncertainties[i].GetName () == sysName) {
      iUncertainty = i;
      break;
    }
  }
  if (iUncertainty >= 0 && iUncertainty < (int) uncertainties.size ()) {
    if (idataset >= 0 && idataset < (int) datasets.size ()) {
      uncertainties[iUncertainty].Fill (idataset, icond, weight);
    }
    else
      cerr << "DiLepXSectionMeas::Fill the iDataset parameter is wrong" << endl;
  }
  else
    cerr << "DiLepXSectionMeas::Fill the iUncertainty parameter is wrong" << endl;
}


//void DiLepXsectionMeas::ComputeLikelihood (bool IncludeSYS, bool FixParameter, int NumberOfP, int *ListOfP, double *ValuesOfP)
void DiLepXsectionMeas::ComputeLikelihood (vector < bool > fixParam, vector < double >ParamValues, bool WriteSysName, bool doCorrelationMatrixHisto, bool verbose)
{


  TMinuit minuit (NofParam);
  // Set fonction pointer
  minuit.SetFCN (fcn);

  double arglist[NofParam];
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
  //int offset = 0;
  //First parameter is the Xsection 
  minuit.mnparm (0, "Xsection", ParamValues[0], 0.1, 0., 700., ierflg); //should not be hard-coded
  //offset++;
  //Second paramter is the Luminosity
  if (isLumiASyst) {
    minuit.mnparm (1, "Lumi", ParamValues[1], 0.01, -5, 5, ierflg);
    if (WriteSysName)
      SysName.push_back (string ("Lumi"));
    iter++;
    //offset++;
  }
  //Add the MC cross-section as parameters in the TMinuit
  //paramXsection = 0;
  for (unsigned int d = 0; d < datasets.size (); d++) {
    if (!datasets[d].isData () && datasets[d].Name ().find ("TT") > datasets[d].Name ().size ()) {
      string name ("Xsection-");
      name += datasets[d].Name ();
      if (DDEstim[d].isASyst) {
        if (WriteSysName)
          SysName.push_back (name);
        minuit.mnparm (iter, name.c_str (), ParamValues[iter], 0.01, -5, 5, ierflg);
        iter++;
        //paramXsection++;
      }
    }
  }
  for (unsigned int i = 0; i < uncertainties.size (); i++) {
    // interval +/- N sigma
    //int NSigma = 20;
    //float min = NSigma*(uncertainties[i].GetParameters(0)[1]-uncertainties[i].GetParameters(0)[0]);
    //float max = NSigma*(uncertainties[i].GetParameters(0)[2]-uncertainties[i].GetParameters(0)[0]);
    //minuit.mnparm (i + iter, uncertainties[i].GetName ().c_str (), ParamValues[i+iter], 0.001, min, max, ierflg);
    minuit.mnparm (i + iter, uncertainties[i].GetName ().c_str (), ParamValues[i + iter], 0.001, -5, 5, ierflg);
    if (WriteSysName)
      SysName.push_back (uncertainties[i].GetName ());
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

  arglist[0] = 1000; //max iterations
  minuit.mnexcm ("MIGRAD", arglist, 1, ierflg);
  t = clock();
  time_2 += t-t0;
  t0 = clock();
  //minuit.mnexcm ("MINOS", arglist, 1, ierflg);
  t = clock();
  time_3 += t-t0;
  t0 = clock();
  minuit.mnexcm ("STOP", arglist, 0, ierflg);
  t = clock();
  time_4 += t-t0;

  // Values of the fitting parameters  
  double ln0, edm, errdef;
  int nvpar, nparx, icstat;
  minuit.mnstat (ln0, edm, errdef, nvpar, nparx, icstat);
  MinFCN = ln0;                 // to be defined

  // Values of errors (parabolic and MINOS)  
  for (int j = 0; j < NofParam; ++j) {
    double ErrParabolic, gcc;
    ErrNegParam[j] = 0.;        // to be defined
    ErrPosParam[j] = 0.;        // to be defined
    minuit.mnerrs (j, ErrPosParam[j], ErrNegParam[j], ErrParabolic, gcc);
  }
  for (int j = 0; j < NofParam; ++j) {
    minuit.GetParameter (j, Param[j], ErrParam[j]);
  }
  if (verbose) {
    cout << "################# after minimization ####################" << endl;
    for (int j = 0; j < NofParam; ++j) {
      std::cout << "Param[" << j << "] = " << Param[j] << " +/-" << ErrParam[j] << " (" << ErrNegParam[j] << "+" << ErrPosParam[j] << ")" << std::endl;
    }
  }

  if (doCorrelationMatrixHisto) {
    CorrelationMatrixHisto = new TH2F ("CorrelationMatrixHisto", "Correlation Matrix", NofParam, 0, NofParam, NofParam, 0, NofParam);
    for (int i = 0; i < NofParam; i++) {
      CorrelationMatrixHisto->GetXaxis ()->SetBinLabel (i + 1, ParamNames[i].c_str ());
      CorrelationMatrixHisto->GetYaxis ()->SetBinLabel (i + 1, ParamNames[i].c_str ());
    }
    double matrix[NofParam][NofParam];
    minuit.mnemat (&matrix[0][0], NofParam);
    for (int i = 0; i < NofParam; i++) {
      for (int j = 0; j < NofParam; j++) {
        CorrelationMatrixHisto->SetBinContent (i + 1, j + 1, matrix[i][j]);
      }
    }
  }

}

void DiLepXsectionMeas::Plot (TString S, int NofPoints, double Xmin, double Xmax, double minimumL, bool verbose)
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
  StatGraph->SetName ("Graph_Stat");
  StatGraph->SetMarkerStyle (29);
  StatGraph->SetMarkerColor (1);
  StatGraph->SetMarkerSize (1.);
  StatGraph->SetTitle ("");
  if (S == "AP")
    StatGraph->Draw ("*");
  if (S == "Same")
    StatGraph->Draw ("Psame");
  leg->AddEntry (StatGraph, "Stat", "P");

  //PLRExamination (NofPoints, X_PLR, Y_PLR, -1);       //-1 means it's purely statistic
  PLRExamination (NofPoints, x, y, -1); //-1 means it's purely statistic

  if (verbose) {
    cout << "--------------------------------------" << endl;
    cout << "PLR just with: " << XsectionEstimStat << " -" << XsectionEstimStatError.first << "/+" << XsectionEstimStatError.second << endl;
    cout << "--------------------------------------" << endl;
  }




}

void DiLepXsectionMeas::Plot_PLR (int NofPoints, double Xmin, double Xmax, double *x, double *y, TGraph * &graph, int color, string name)
{
  c->cd ();

  graph = new TGraph (NofPoints, x, y);
  graph->SetMarkerStyle (20);
  graph->SetMarkerSize (1.);
  graph->SetMarkerColor (color);
  leg->AddEntry (graph, name.c_str (), "P");
  graph->SetTitle ("");
  /*
     if (S == "AP")
     graph->Draw ("*");
     if (S == "Same")
   */
  graph->Draw ("Psame");
}

double DiLepXsectionMeas::XsectionFCN (Double_t * par)
{
  //int npar = uncertainties.size () + 2; 
  int npar = NofParam;
  int iflag = 1;                // compute
  double returnValue = 0.;
  double gin[npar];             //derivative
  for (int i = 0; i < npar; i++)
    gin[i] = 0;
  fcn (npar, gin, returnValue, par, iflag);
  return (returnValue);
}

void fcn (Int_t & npar, Double_t * gin, Double_t & f, Double_t * par, Int_t iflag)
{
  bool debug = false;           // put to true carrefully ... this function is called a lot of time. This boolean cannot be give as a parameter

  float BRChannel = myMeas->GetBRChannel ();
  double LnLikelihood = 0;
  float localSignXsection = par[0];
  float localLumi = myMeas->GetLumi ();
  if (myMeas->IsLumiASyst ())
    localLumi = myMeas->GetLumi () + par[1] * myMeas->GetLumiError ();

  float NexpTot = myMeas->NumberOfExpectedEvents (par, debug);

  if (debug) {
    cout << "###############################" << endl;
    cout << "    In fcn              " << endl;
    cout << "###############################" << endl;
    cout << "LocalSignXsection = " << localSignXsection << " " << par[0] << endl;
    cout << "LocalLumi = " << localLumi << endl;
    cout << "BR = " << BRChannel << endl;
    cout << "NofExpected evts = " << NexpTot << endl;
    cout << "Nof evts in data = " << myMeas->GetNobs () << endl;
  }
  if (NexpTot > 0) {
    if (debug) {
      cout << "--------------------------" << endl;
      cout << "    Computation of LnLike " << endl;
      cout << "--------------------------" << endl;
    }
    LnLikelihood = myMeas->GetNobs () * TMath::Log (NexpTot) - NexpTot;
    if (debug)
      cout << "LnLike = -( (Nobs) " << myMeas->GetNobs () << " * (log(Nexp)) " << TMath::Log (NexpTot) << " - (Nexp) " << NexpTot;
    for (int i = (myMeas->Offset ()); i < myMeas->GetNofParam (); i++) {
      LnLikelihood -= 0.5 * (par[i] - 0.) * (par[i] - 0.) / (1. * 1.);
      if (debug)
        cout << "- 0.5 * " << par[i] << "^2 ";
    }
    if (debug)
      cout << endl;
  }
  LnLikelihood *= -1.;
  f = LnLikelihood;
  if (debug) {
    cout << " LnLikelihood = " << f << endl;
    cout << "##################################" << endl;

  }
}

void DiLepXsectionMeas::PassTheSelection (int idataset)
{
  if (idataset >= 0 && idataset < (int) datasets.size ()) {
    SelEvents[idataset]++;
  }
  else
    cerr << "DiLepXSectionMeas::PassTheSelection the iDataset parameter is wrong" << endl;
}


void DiLepXsectionMeas::PassTheSelection (int idataset, float weight)
{
  if (idataset >= 0 && idataset < (int) datasets.size ()) {
    SelEvents[idataset] += weight;
  }
  else
    cerr << "DiLepXSectionMeas::PassTheSelection the iDataset parameter is wrong" << endl;
}

void DiLepXsectionMeas::PrintSelEff ()
{
  cout << " --------------------------------" << endl;
  cout << "   Selection efficiencies    " << endl;
  cout << " --------------------------------" << endl;
  for (unsigned int i = 0; i < datasets.size (); i++) {
    cout << datasets[i].Name () << " Eff = " << GetSelEff (i) << endl;
  }
  cout << " --------------------------------" << endl;
}

void DiLepXsectionMeas::SetSelEff (int idataset, float value)
{
  if (idataset >= 0 && idataset < (int) datasets.size ()) {
    SelEff[idataset] = value;
  }
  else
    cerr << "DiLepXSectionMeas::SetSelEff the iDataset parameter is wrong" << endl;
}

void DiLepXsectionMeas::SetSelEff (string DatasetName, float value)
{
  int idataset = -1;
  for (unsigned int i = 0; i < datasets.size (); i++) {
    if (datasets[i].Name () == DatasetName) {
      idataset = i;
      break;
    }
  }
  if (idataset >= 0 && idataset < (int) datasets.size ()) {
    SelEff[idataset] = value;
  }
  else
    cerr << "DiLepXSectionMeas::SetSelEff the iDataset parameter is wrong" << endl;
}

float DiLepXsectionMeas::GetSelEff (int idataset) const
{
  if (idataset >= 0 && idataset < (int) datasets.size ()) {
    return SelEff[idataset];
  }
  else
    cerr << "DiLepXSectionMeas::GetSelEff the iDataset parameter is wrong" << endl;
  return 1.;
}

vector < float >DiLepXsectionMeas::GetMCXsection () const
{
  vector < float >out;
  for (unsigned int i = 0; i < datasets.size (); i++) {
    out.push_back (GetMCXsection (i));
  }
  return out;
}

float DiLepXsectionMeas::GetMCXsection (int idataset) const
{
  if (idataset >= 0 && idataset < (int) datasets.size ()) {
    return Xsection.GetParameters (idataset)[0];
  }
  cerr << "DiLepXsectionMeas::GetMCXsection:  idataset is a wrong paramter" << endl;
  return -999.;
}

void DiLepXsectionMeas::Compute ()
{
  for (unsigned int i = 0; i < uncertainties.size (); i++) {
    cout << "UncertaintY: " << uncertainties[i].GetName () << endl;
    uncertainties[i].Compute ();
  }
  for (unsigned int i = 0; i < SelEff.size (); i++) {
    SelEff[i] = (float) SelEvents[i] / InitNofEvents[i];
  }
}


void DiLepXsectionMeas::EfficienciesTable (ofstream & fout)
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
  fout << "\\caption{ Ratio table for channel " << channel << " }" << endl;
  fout << "\\label{tab:RatioTable" << channel << "}" << endl;
  fout << "\\end{table}" << endl;
}


float DiLepXsectionMeas::IsolateSystError (float statError, float combinedError) const
{
  return (sqrt (combinedError * combinedError - statError * statError));
}

void DiLepXsectionMeas::UncertaintiesTable (ofstream & fout, bool SkipOneByOne)
{
  fout << "\\begin{table}" << endl;
  fout << "\\centering" << endl;
  fout << "\\begin{tabular}{|c|c|c|}";
  fout << "\\hline" << endl;
  fout << "Stat &" << XsectionEstimStatError.first << " (" << XsectionEstimStatError.first / XsectionEstim * 100. << "\\%) & " << XsectionEstimStatError.second << " (" << XsectionEstimStatError.second / XsectionEstim * 100. << "\\%)\\\\" << endl;
  if (!SkipOneByOne) {
    for (unsigned int i = 0; i < SysName.size (); i++) {
      fout << SysName[i] << " & ";
      fout << XsectionEstimSysError[i].first << " (" << XsectionEstimSysError[i].first / XsectionEstim * 100. << "\\%) & " << XsectionEstimSysError[i].second << " (" << XsectionEstimSysError[i].second / XsectionEstim * 100. << "\\%)\\\\" << endl;
      //fout<<IsolateSystError(XsectionEstimStatError.first,XsectionEstimSysError[i].first)<<" ("<<IsolateSystError(XsectionEstimStatError.first,XsectionEstimSysError[i].first)/XsectionEstim*100.<<"\\%) & "<<IsolateSystError(XsectionEstimStatError.second,XsectionEstimSysError[i].second)<<" ("<<IsolateSystError(XsectionEstimStatError.second,XsectionEstimSysError[i].second)/XsectionEstim*100.<<"\\%)\\\\"<<endl;
    }
  }
  fout << "All &" << XsectionEstimAllError.first << " (" << XsectionEstimAllError.first / XsectionEstim * 100. << "\\%) & " << XsectionEstimAllError.second << " (" << XsectionEstimAllError.second / XsectionEstim * 100. << "\\%)\\\\" << endl;
  fout << "All &" << IsolateSystError (XsectionEstimStatError.first, XsectionEstimAllError.first) << " (" << IsolateSystError (XsectionEstimStatError.first,
                                                                                                                               XsectionEstimAllError.first) / XsectionEstim *
    100. << "\\%) & " << IsolateSystError (XsectionEstimStatError.second, XsectionEstimAllError.second) << " (" << IsolateSystError (XsectionEstimStatError.second, XsectionEstimAllError.second) / XsectionEstim * 100. << "\\%)\\\\" << endl;
  fout << "\\hline" << endl;
  fout << "\\end{tabular}" << endl;
  fout << "\\caption{ Uncertainties table for channel " << channel << "}" << endl;
  fout << "\\label{tab:UncerTable" << channel << "}" << endl;
  fout << "\\end{table}" << endl;
}

void DiLepXsectionMeas::MinimizationTable (ofstream & fout)
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
  fout << "\\caption{ Minimization table for channel " << channel << " }" << endl;
  fout << "\\label{tab:MinTable" << channel << "}" << endl;
  fout << "\\end{table}" << endl;
}

void DiLepXsectionMeas::NumberOfEventsTable (ofstream & fout)
{
  fout << "\\begin{table}" << endl;
  fout << "\\centering" << endl;
  fout << "\\begin{tabular}{|c|c|c|}";
  fout << "\\hline" << endl;
  fout << "Contribution & MC Expectation & PLR Estimation  \\\\" << endl;
  fout << "\\hline" << endl;
  if (NofEvtsContribMC.size () == datasets.size () && NofEvtsContribFitted.size () == datasets.size ()) {
    float sumMC = 0;
    float sumFit = 0;
    for (unsigned int i = 0; i < datasets.size (); i++) {
      fout << datasets[i].Name () << " & " << NofEvtsContribMC[i] << " & " << NofEvtsContribFitted[i] << " \\\\ " << endl;
      sumMC += NofEvtsContribMC[i];
      sumFit += NofEvtsContribFitted[i];
    }
    fout << "\\hline" << endl;
    fout << "Sum & " << sumMC << " & " << sumFit << " \\\\ " << endl;
    fout << "\\hline" << endl;
    fout << "Data & " << GetNobs () << " & " << GetNobs () << " \\\\ " << endl;
    fout << "\\hline" << endl;
  }
  fout << "\\end{tabular}" << endl;
  fout << "\\caption{ NofEvts in PLR for channel " << channel << " }" << endl;
  fout << "\\label{tab:NofEvtsPLRTable" << channel << "}" << endl;
  fout << "\\end{table}" << endl;

}

float DiLepXsectionMeas::NumberOfExpectedEvents (bool write)
{
  bool verbose = false;
  float NofEvts = 0;
  if (verbose)
    cout << "Number of events:" << endl;
  if (write)
    NofEvtsContribMC.clear ();
  for (unsigned int d = 0; d < datasets.size (); d++) {
    float n = NumberOfExpectedEvents ((int) d, verbose);
    NofEvts += n;
    if (write)
      NofEvtsContribMC.push_back (n);
  }
  if (verbose)
    cout << "Total = " << NofEvts << endl;
  return NofEvts;
}

float DiLepXsectionMeas::NumberOfExpectedEvents (int idataset, bool verbose) const
{
  float par = 0;
  if (datasets[idataset].Name ().find ("TT") >= 0 && datasets[idataset].Name ().find ("TT") < datasets[idataset].Name ().size ()) {
    par = ExpectedXsection;
  }
  return NumberOfExpectedEvents (idataset, false, par, Lumi, verbose);
}

float DiLepXsectionMeas::NumberOfExpectedEvents (int idataset, bool computeSlope, double par, float lumi, bool verbose) const
{
  if (!CheckDataset (idataset)) {
    cerr << "DiLepXsectionMeas::SystCorrectionOnNofEvents:: idataset is invalid" << endl;
    return -999.;
  }
  float NofEvts = 0;
  //Data Driven estimation ... or
  if (GetDDEstimation ()[idataset].isDDEstimated) {
    if (GetDDEstimation ()[idataset].isASyst) {
      float slope = 0;
      if (computeSlope) {
        if (par > 0)
          slope = GetDDEstimation ()[idataset].ErrPos;
        else
          slope = GetDDEstimation ()[idataset].ErrNeg;
      }
      NofEvts = GetDDEstimation ()[idataset].NofEvts + slope * par;
    }
    else {
      NofEvts = GetDDEstimation ()[idataset].NofEvts;
    }
  }
  //MC expectation
  else {
    //The signal
    if (datasets[idataset].Name () == GetSignalDatasetName ()) {
      // For the signal, par is the cross-section
      NofEvts = par * BRChannel * lumi * datasets[idataset].PreSelEfficiency () * GetSelEff (idataset);
    }
    else {
      if (datasets[idataset].Name ().find ("TT") >= 0 && datasets[idataset].Name ().find ("TT") < datasets[idataset].Name ().size ())
        NofEvts = 0;
      // The non-ttbar background
      else {
        float slope = 0;
        if (computeSlope) {
          if (par > 0)
            slope = GetMCXsectionUncertainty ().GetParameters (idataset)[0];
          else
            slope = GetMCXsectionUncertainty ().GetParameters (idataset)[2];
        }
        NofEvts = (datasets[idataset].Xsection () + slope * par) * lumi * datasets[idataset].PreSelEfficiency () * GetSelEff (idataset);
      }
    }
  }
  if (verbose)
    cout << datasets[idataset].Name () << ": " << NofEvts << endl;
  return NofEvts;
}

int DiLepXsectionMeas::NumberOfExpectedEventsRandom (TRandom & rand, bool randoSyst, bool verbose)
{
  double *par = new double[NofParam];
  for (int i = 0; i < NofParam; i++) {
    if (randoSyst)
      par[i] = rand.Gaus (0., 1.);
    else
      par[i] = 0.;
  }
  //Impose the MC cross-section for signal
  par[0] = ExpectedXsection;
  //The randomisation due to systematics will be done now ...
  float n = NumberOfExpectedEvents (par, verbose);
  //The randomisation with a poisson distribution will be done now ...
  int NofEvts = rand.Poisson (n);
  return NofEvts;
}

float DiLepXsectionMeas::NumberOfExpectedEvents (double *par, bool verbose, bool write)
{
  bool computeSlope = true;
  //compute lumi
  float lumi = Lumi;
  if (isLumiASyst)
    lumi = Lumi + par[1] * LumiError;
  float NofEvts = 0;
  if (verbose)
    cout << "Number of events:" << endl;
  if (write)
    NofEvtsContribFitted.clear ();
  for (unsigned int i = 0; i < datasets.size (); i++) {
    float param = 0;
    int indice = IndiceForXsection (i);
    if (indice >= 0)
      param = par[indice];
    float n = NumberOfExpectedEvents (i, computeSlope, param, lumi, verbose);
    float kfactor = SystCorrectionOnNofEvents (i, par, verbose);
    float nprime = n * kfactor;
    if (write)
      NofEvtsContribFitted.push_back (nprime);
    NofEvts += nprime;
    if (verbose)
      cout << datasets[i].Name () << ": " << nprime << " = " << n << " * " << kfactor << endl;
  }
  if (verbose)
    cout << "Total = " << NofEvts << endl;
  return NofEvts;
}

float DiLepXsectionMeas::SystCorrectionOnNofEvents (int idataset, double *par, bool verbose) const
{
  double Kfactor = 1;
  if (verbose)
    cout << "Kfactor = 1";
  //K-factor from systematics (JES,etc ...)
  for (unsigned int i = 0; i < uncertainties.size (); i++) {
    double param = 0;
    int indice = IndiceForUncertainty (i);
    if (indice >= 0)
      param = par[indice];
    double k = SystCorrectionOnNofEvents (idataset, i, param);
    if (verbose)
      cout << "*" << k;
    Kfactor *= k;
  }
  if (verbose)
    cout << " = " << Kfactor << endl;
  return Kfactor;
}

float DiLepXsectionMeas::SystCorrectionOnNofEvents (int idataset, int iUncertainty, double par) const
{
  if (!CheckDataset (idataset)) {
    cerr << "DiLepXsectionMeas::SystCorrectionOnNofEvents:: idataset is invalid" << endl;
    return -999.;
  }
  if (!CheckUncertainty (iUncertainty)) {
    cerr << "DiLepXsectionMeas::SystCorrectionOnNofEvents:: iUncertainty is invalid" << endl;
    return -999.;
  }
  float slope = 0;
  if (par > 0)
    slope = (GetUncertainties ()[iUncertainty].GetEfficiencies (idataset)[2] - GetUncertainties ()[iUncertainty].GetEfficiencies (idataset)[0]);
  else
    slope = (GetUncertainties ()[iUncertainty].GetEfficiencies (idataset)[0] - GetUncertainties ()[iUncertainty].GetEfficiencies (idataset)[1]);
  return (slope * par + GetUncertainties ()[iUncertainty].GetParameters (idataset)[0]);
}

bool DiLepXsectionMeas::CheckDataset (int idataset) const
{
  if (idataset >= 0 && idataset < (int) datasets.size ())
    return true;
  return false;
}

bool DiLepXsectionMeas::CheckUncertainty (int iUncertainty) const
{
  if (iUncertainty >= 0 && iUncertainty < (int) uncertainties.size ())
    return true;
  return false;
}

int DiLepXsectionMeas::IndiceForXsection (int idataset) const
{
  if (!CheckDataset (idataset)) {
    cerr << "DiLepXsectionMeas::IndiceForXsection:: idataset is invalid" << endl;
    return -999;
  }
  int indice = offset - 1;      //indice starts at 0, that's why ther is a "-1"
  //Add the MC cross-section as parameters in the TMinuit
  for (int d = 0; d <= idataset; d++) {
    if (!datasets[d].isData ()) {
      if (datasets[d].Name ().find ("TT") > datasets[d].Name ().size ()) {
        if (DDEstim[d].isASyst) {
          indice++;
        }
      }
      else {
        indice = 0;
      }
    }
  }
  return indice;
}

int DiLepXsectionMeas::IndiceForUncertainty (int iUncertainty) const
{
  if (!CheckUncertainty (iUncertainty)) {
    cerr << "DiLepXsectionMeas::IndiceForUncertainty:: iUncertainty is invalid" << endl;
    return -999;
  }
  return (offset + paramXsection + iUncertainty);
}

void DiLepXsectionMeas::XExtraction (double NObs, bool doMCPseudoExp, int NofPseudoExp, bool SmearSystInPseudoExp, int NofPoints, double MinXforLogLPlot, double MaxXforLogLPlot, int verbosity, bool SkipOneByOne)
{

  //the following line is extremely important.
  //this allow a correct behaviour of the code event is many objects DiLepXsectionMeas are instanciated.
  myMeas = this;

  bool verbose = false;
  bool verboseMax = false;
  if (verbosity > 0){
    verbose = true;
    verboseMax = true;
  }
  //verboseMax = true;          //comment
  if (verbose) {
    cout << "####################################" << endl;
    cout << "   Cross-Section computation        " << endl;
    cout << "####################################" << endl;
  }

  //Reinitialization needed if the method is called many times !
  ParamNames.clear ();

  SetNobs (NObs);
  MinFCN = 0.;
  double MinlogL0 = 0.;


  // Give main input parameters
  bool PlotLogCurve = true;     // to plot loglikelihood curves
  bool doCorrelationMatrixHisto = true;
  if (doMCPseudoExp)
    PlotLogCurve = false;
  if (doMCPseudoExp)
    doCorrelationMatrixHisto = false;
  if (doMCPseudoExp)
    SkipOneByOne = true;

  //Write the table of expected events from MC
  NumberOfExpectedEvents (true);

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
  if (isLumiASyst)
    offset = 2;
  else
    offset = 1;

  paramXsection = 0;
  for (unsigned int d = 0; d < datasets.size (); d++) {
    if (!datasets[d].isData () && datasets[d].Name ().find ("TT") > datasets[d].Name ().size ()) {
      //change by loop other Channels \\ WARNING
      if (DDEstim[d].isASyst) {
        ParamValues.push_back (datasets[d].Xsection ());
        ParamNames.push_back (string ("Xsection") + datasets[d].Name ());
        paramXsection++;
      }
    }
  }
  for (unsigned int i = 0; i < uncertainties.size (); i++) {
    ParamValues.push_back (0);  //delta-parameter center around 0
    ParamNames.push_back (uncertainties[i].GetName ());
  }
  NofParam = offset + paramXsection + uncertainties.size ();    // NofParam is now correct


  for (int i = 0; i < NofParam; i++) {
    if (i == 0)
      fixParam.push_back (false);       // sigma_tt is free
    else
      fixParam.push_back (true);
  }
  if ((int) fixParam.size () != NofParam || (int) ParamValues.size () != NofParam)
    cerr << "XExtraction: nof parameters is incorrect" << endl;
  ///////////////////

  ////fix the luminosity
  //fixParam[1] = true;

  graphs = new TGraph *[NofParam + 2];
  //for(int i=0;i<NofParam+2;i++) graphs.push_back(TGraph* a);
  graphsDeclared = true;

  if (doMCPseudoExp && !ParameterHistoDeclared) {
    ParameterHistoDeclared = true;
    ParametersHisto = new TH1F *[NofParam];
    for (int h = 0; h < NofParam; h++) {
      string name = string ("histo_") + ParamNames[h];
      if (h == 0)
        ParametersHisto[h] = new TH1F (name.c_str (), "", 50, 0, 300);
      else
        ParametersHisto[h] = new TH1F (name.c_str (), "", 50, -0.5, 0.5);
    }
  }

  if (!doMCPseudoExp)
    NofPseudoExp = 1;
  double MinlogL = MinFCN;
  TRandom rand;
  for (int k = 0; k < NofPseudoExp; ++k) {
    //Randomisation of the nof observed event if (MC-pseudo)
    if (doMCPseudoExp) {
      double localN = NumberOfExpectedEventsRandom (rand, SmearSystInPseudoExp, true);
      SetNobs (localN);
    }

    if (verbose) {
      std::cout << "======= First step: Minimization of LogL, Sys OFF, one (=sigma_tt) parameter =======" << std::endl;
      std::cout << "Number of events : " << Nobs << std::endl;
    }
    ComputeLikelihood (fixParam, ParamValues, true, false, verboseMax);
    XsectionEstim = Param[0];
    MinlogL0 = MinFCN;
    if (verbose) {
      for (int j = 0; j < NofParam; ++j) {
        std::cout << "Param[" << j << "] = " << Param[j] << " +/-" << ErrParam[j] << " (" << ErrNegParam[j] << "+" << ErrPosParam[j] << ")" << std::endl;
      }
      std::cout << "MinlogL " << MinlogL << std::endl;
    }
    //Plotting of the values coming from the minimisation
    if (PlotLogCurve) {
      Plot ("AP", NofPoints, MinXforLogLPlot, MaxXforLogLPlot, MinlogL0);
    }
     //PLRExamination (NofPoints, X_PLR, Y_PLR, -1);    //-1 means it's purely statistic
    //Xseccentralvalue_wosys->Fill (Param[0]);
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
    if (verbose)
      std::cout << "====================================================================================" << std::endl;

    if (verbose) {
      std::cout << "======= Second step: Minimization of LogL, Sys ON, all free parameters =======" << std::endl;
    }
    for (int i = 0; i < NofParam; i++) {
      fixParam[i] = false;
    }
    //fix the luminosity
    //fixParam[1] = true;
    ComputeLikelihood (fixParam, ParamValues, false, doCorrelationMatrixHisto, verboseMax);
    MinlogL0 = MinFCN;
    for (int j = 0; j < NofParam; ++j) {
      if (verbose)
        std::cout << "Param[" << j << "] = " << Param[j] << " +/-" << ErrParam[j] << " (" << ErrNegParam[j] << "+" << ErrPosParam[j] << ")" << std::endl;
      //important: update the values according to the previous minimization where all paramters are free
      ParamValues[j] = Param[j];
    }
    if (verbose)
      std::cout << "MinlogL " << MinlogL << std::endl;
    XsectionEstim = Param[0];   //fill the estimation
    if (verbose)
      std::cout << "====================================================================================" << std::endl;
  ////end of loop over Pseudo-Experiments
    //cout<<"CONTINUE"<<endl;

  /*
     if (!PlotLogCurve) {
     Xseccentralvalue_wosys->Draw ();
     }
   */


  if (PlotLogCurve) {
    for (int j = 0; j < NofPoints; ++j) {
      X_PLR[j] = 0.;
      Y_PLR[j] = 0.;
    }
  }

    //save the ParamValues which minimize the likelihood when all paramters are free
    vector < double >ParamValuesMin;
    for (unsigned int i = 0; i < ParamValues.size (); i++)
      ParamValuesMin.push_back (ParamValues[i]);

    ////////////////////////////////////////////////
    ///   Scan of the Xsection: Profile Likelihood
    ////////////////////////////////////////////////                            
    //Compute the PLR for each uncertainty + stat only (u [0,NofParam-2]) + all uncertainties combined (u==NofParam-1)
    for (int u = 0; u < NofParam; u++) {
      //cout << "begin" << endl;
      if ( (SkipOneByOne == true) && u < (NofParam - 1))
        continue;
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
        //cout << "MinLogL0: " << MinlogL0 << endl;
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
        if(verbose){
        cout << "LogLik " << Y_PLR[i] << " = " << XsectionFCN (P) << " - " << MinlogL0 << endl;
        std::cout << "=============================================================================" << std::endl;
        }
      }

      string sysname;
      if (u >= 0 && u < (NofParam - 1))
        sysname = SysName[u];
      if (u == NofParam - 1)
        sysname = string ("all");

      if (PlotLogCurve)
        Plot_PLR (NofPoints, MinXforLogLPlot, MaxXforLogLPlot, X_PLR, Y_PLR, graphs[u], 2 + u, sysname);
      PLRExamination (NofPoints, X_PLR, Y_PLR, u);

      //--- With all systematics considered ---//
      if (u == NofParam - 1) {
        //--- REDO the PLR at minimum with all systematics ---//
        //Compute the number of evts fitted with the PLR, all systematics at minimum
        for (int p = 0; p < NofParam; p++)
          fixParam[p] = false;  //all uncertainties considered
        //Simga is fixed
        fixParam[0] = true;
        ParamValues[0] = XsectionEstimWithAllSyst;
        ComputeLikelihood (fixParam, ParamValues, false, false, verboseMax);
        double *par = new double[ParamValues.size ()];
        for (unsigned int p = 0; p < ParamValues.size (); p++)
          par[p] = ParamValues[p];
        NumberOfExpectedEvents (par, verbose, true);
        if (doMCPseudoExp) {
          for (int h = 0; h < NofParam; h++) {
            ParametersHisto[h]->Fill (ParamValues[h]);
          }
        }
  /////////////// Save the Parameters of the optimization
  for (int i = 0; i < NofParam; i++) {
    if(i==0){
        ErrNegParam_Min[0] = -XsectionEstimAllError.first; 
        ErrPosParam_Min[0] = XsectionEstimAllError.second; 
        }
        else{
    ErrNegParam_Min[i] = ErrNegParam[i];
    ErrPosParam_Min[i] = ErrPosParam[i];
        }
    Param_Min[i] = Param[i];
    ErrParam_Min[i] = ErrParam[i];
  }
    Xseccentralvalue_wsys->Fill (XsectionEstimWithAllSyst);
    Erreurneg_wsys->Fill (XsectionEstimAllError.first);
    Erreurpos_wsys->Fill (XsectionEstimAllError.second);
    Erreurmean_wsys->Fill ((XsectionEstimAllError.first + XsectionEstimAllError.second) / 2.);
    PullErreurneg_wsys->Fill ((XsectionEstimWithAllSyst - ExpectedXsection) / XsectionEstimAllError.first);
    PullErreurpos_wsys->Fill ((XsectionEstimWithAllSyst - ExpectedXsection) / XsectionEstimAllError.second);
    PullErreurmean_wsys->Fill ((XsectionEstimWithAllSyst - ExpectedXsection) / ((XsectionEstimAllError.first + XsectionEstimAllError.second) / 2.));
    if(XsectionEstimWithAllSyst<ExpectedXsection) PullErreur2cases_wsys->Fill((XsectionEstimWithAllSyst - ExpectedXsection) / XsectionEstimAllError.first);
    else PullErreur2cases_wsys->Fill((XsectionEstimWithAllSyst - ExpectedXsection) / XsectionEstimAllError.second);

      }
    }
  //}                           //PlotLogCurve
  }//end of loop over Pseudo-Experiments
}

void DiLepXsectionMeas::PLRValuesWithLinearAppromixation (int NofPoints, double *X_PLR, double *Y_PLR, float &ErrNeg, float &ErrPos, float &Min)
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
      float a = (Y_PLR[i] - Y_PLR[i - 1]) / (X_PLR[i] - X_PLR[i - 1]);
      float b = Y_PLR[i - 1] - a * X_PLR[i - 1];
      ErrNeg = (0.5 - b) / a;
      first = false;
    }
    if (Y_PLR[i] > 0.5 && !first) {
      //linear approach, need enought point to be exact
      float a = (Y_PLR[i] - Y_PLR[i - 1]) / (X_PLR[i] - X_PLR[i - 1]);
      float b = Y_PLR[i - 1] - a * X_PLR[i - 1];
      ErrPos = (0.5 - b) / a;
      break;
    }
    if (X_PLR[i] > XsectionEstim && first_min) {
      float a = (Y_PLR[i - 1] - Y_PLR[i - 2]) / (X_PLR[i - 1] - X_PLR[i - 2]);
      float b = Y_PLR[i - 2] - a * X_PLR[i - 2];
      Min = (0 - b) / a;
      first_min = false;
    }
  }
  ErrNeg = Min - ErrNeg;
  ErrPos = ErrPos - Min;
  if (fabs (Min - XsectionEstim) > 0.01 * XsectionEstim)
    cerr << "DiLepXsectionMeas::PLRValuesWithLinearAppromixation:: There is a problem in the search of the minimum !!" << endl;
}

void DiLepXsectionMeas::PLRValuesWithPol2Fit (int NofPoints, double *X_PLR, double *Y_PLR, float &ErrNeg, float &ErrPos, float &Min)
{
  bool first = true;
  bool first_min = true;
  float ErrMin = 0.;
  float ErrMax = 0.;
  float Mini = 0.;
  //int NPSearch = 100;
  float precision = 0.0001;
  int MaxIt = 100;
  int NeighbourgPoints = 2;
  TGraph g(NofPoints, X_PLR, Y_PLR);
  for (int i = 1; i < NofPoints; i++) {
        cout<<X_PLR[i]<<" "<<Y_PLR[i]<<endl;
  }
  for (int i = 1; i < NofPoints; i++) {
    if (Y_PLR[i] < 0.5 && first) {
      first = false;
      //fit the graph:
         int imin = 0;
         if(i-NeighbourgPoints>=0) imin = i-NeighbourgPoints;
         int imax = i+NeighbourgPoints;
         cout<<"imin: "<<imin<<" "<<X_PLR[imin]<<" imax: "<<imax<<" "<<X_PLR[imax]<<endl;
         TF1 f1("func1","pol2",X_PLR[imin],X_PLR[imax]);
         g.Fit(&f1,"","",X_PLR[imin],X_PLR[imax]);
         float diff = 1.;
         float a = X_PLR[imin];
         float b = X_PLR[imax];
         float value = 0;
         int it = 0;
         //pol2
         float A = f1.GetParameter(2);
         float B = f1.GetParameter(1);
         float C = f1.GetParameter(0)-0.5;//ax2+bx+c=0.5
         cout<<"ErrMin"<<endl;
         cout<<A<<"*x2+"<<B<<"*x+"<<C<<endl;
         cout<<"NofPoints: "<<f1.GetNumberFitPoints()<<endl;
         float Delta = B*B-4*A*C;
          cout<<"Delta: "<<Delta<<endl;
         if(Delta<0) cerr<<"DiLepXsectionMeas::PLRValuesWithPol2Fit::No real solution !"<<endl;
         else value = (-B-sqrt(Delta))/(2*A); // solution min
         ErrMin = value;
         cout<<"Pos: "<<value<<endl;
         /*
         while(diff>precision && it<MaxIt ){
                float x = (a+b)/2.;
                cout<<"diff: "<<diff<<" it: "<<it<<" x: "<<x<<endl;     
                value = f1.Eval(x);
                diff = fabs(value-0.5);
                if(value<0.5) b = x; //pente decroissante
                else a = x;
                ErrMin = x;
                it++;
         }
        */
    }
    if (Y_PLR[i] > 0.5 && !first) {
      //fit the graph:
         int imin = i-NeighbourgPoints;
         int imax = 0;
         if(i+NeighbourgPoints>NofPoints) imax = NofPoints-NeighbourgPoints;
         else imax = i+NeighbourgPoints;
         TF1 f2("func2","pol2",X_PLR[imin],X_PLR[imax]);
         g.Fit(&f2,"","",X_PLR[imin],X_PLR[imax]);
         float diff = 1.;
         float a = X_PLR[imin];
         float b = X_PLR[imax];
         float value = 0;
         int it = 0;
         //pol2
         float A = f2.GetParameter(2);
         float B = f2.GetParameter(1);
         float C = f2.GetParameter(0)-0.5;//ax2+bx+c=0.5
         cout<<"ErrMax"<<endl;
         cout<<A<<"*x2+"<<B<<"*x+"<<C<<endl;
         cout<<"NofPoints: "<<f2.GetNumberFitPoints()<<endl;
         float Delta = B*B-4*A*C;
          cout<<"Delta: "<<Delta<<endl;
         if(Delta<0) cerr<<"DiLepXsectionMeas::PLRValuesWithPol2Fit::No real solution !"<<endl;
         else value = (-B+sqrt(Delta))/(2*A); // solution max
         ErrMax = value;
         cout<<"Pos: "<<value<<endl;
         /*
         while(diff>precision && it<MaxIt ){
                float x = (a+b)/2.;
                value = f2.Eval(x);
                diff = fabs(value-0.5);
                if(value>0.5) b = x; //pente croissante
                else a = x;
                ErrMax = x;
                it++;
         }
        */
      break;
    }
    if (X_PLR[i] > XsectionEstim && first_min) {
         first_min = false;
      //fit the graph:
         int imin = 0;
         int imax = 0;
         if(i-NeighbourgPoints<0) imin = 0;
         else imin = i-NeighbourgPoints;
         if(i+NeighbourgPoints>NofPoints) imax = NofPoints-NeighbourgPoints;
         else imax = i+NeighbourgPoints;
         imax = i+2;
         TF1 fmin("funcmin","pol2",X_PLR[imin],X_PLR[imax]);
         g.Fit(&fmin,"","",X_PLR[imin],X_PLR[imax]);
         float diff = 1.;
         float a = X_PLR[imin];
         float b = X_PLR[imax];
         float value = 0;
         int it = 0;
         //pol2
         float A = fmin.GetParameter(2);
         float B = fmin.GetParameter(1);
         float C = fmin.GetParameter(0);//ax2+bx+c=0
         //cout<<"Minimum"<<endl;
         //cout<<A<<"*x2+"<<B<<"*x+"<<C<<endl;
         //cout<<"NofPoints: "<<fmin.GetNumberFitPoints()<<endl;
         float Delta = B*B-4*A*C;
          //cout<<"Delta: "<<Delta<<endl;
         if(fabs(Delta)>0.001) cerr<<"DiLepXsectionMeas::PLRValuesWithPol2Fit::No real solution !"<<endl;
         else{  
                 value = (-B)/(2*A); // solution unique normaly
          }
         Mini = value;
         //cout<<"Pos: "<<value<<endl;
         /*
         while(diff>precision && it<MaxIt ){
                float x = (a+b)/2.;
                float f_x = fmin.Eval(x);
                float f_a = fmin.Eval(a);
                float f_b = fmin.Eval(b);
                //if(it==0) x = a;//initialisation
                cout<<"Mini-diff: "<<diff<<" it: "<<it<<" x: "<<x<<endl;        
                value = fmin.Eval(x);
                if((f_a+f_x)>(f_b+f_x))//symetric pol2
                        a = x;
                else
                        b = x;
                diff = fabs(value-0.);
                Mini = x;
                it++;
         }
        */
    }
  }
  ErrNeg = Mini - ErrMin;
  ErrPos = ErrMax - Mini;
  Min = Mini;
  cout<<"Min: "<<Min<<" ErrNeg: "<<ErrNeg<<" ErrPos: "<<ErrPos<<endl;
  if (fabs (Min - XsectionEstim) > 0.01 * XsectionEstim)
    cerr << "DiLepXsectionMeas::PLRValuesWithPol2Fit:: There is a problem in the search of the minimum !!" << endl;
}

void DiLepXsectionMeas::PLRExamination (int NofPoints, double *X_PLR, double *Y_PLR, int indice)
{
  float ErrNeg = 0.;
  float ErrPos = 0.;
  float Min = 0.;
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
}




void DiLepXsectionMeas::BkgDataDrivenEstimation (string Process, float NofEvts, float ErrNeg, float ErrPos, bool isASyst)
{
  int id = -1;
  for (unsigned int d = 0; d < datasets.size (); d++) {
    if (datasets[d].Name () == Process) {
      id = d;
      break;
    }
  }
  if (id >= 0) {
    DDEstim[id].isDDEstimated = true;
    DDEstim[id].isASyst = isASyst;
    DDEstim[id].NofEvts = NofEvts;
    DDEstim[id].ErrNeg = ErrNeg;
    DDEstim[id].ErrPos = ErrPos;
  }
  if (id < 0)
    return;
  //missing code here !!!

}

void DiLepXsectionMeas::Write (TFile * fout)
{
  fout->cd ();
  if (!fout->cd ("Xsection"))
    fout->mkdir ("Xsection");
  TDirectory *dir1 = 0;
  TDirectory *dir2 = 0;
  dir1 = fout->GetDirectory ("Xsection");
  dir2 = dir1->GetDirectory (channel.c_str ());
  if (dir2 == 0)
    dir2 = dir1->mkdir (channel.c_str ());
  dir2->cd ();

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
  if (StatGraph)
    StatGraph->Write ();
  if (CorrelationMatrixHisto)
    CorrelationMatrixHisto->Write ();
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
}
        
void DiLepXsectionMeas::TimingReport(){
        cout<<"==================================="<<endl;
        cout<<" Timing report (DiLepXsectionMeas) "<<endl;
        cout<<"==================================="<<endl;
        cout<<" Action 1 : "<<time_1/CLOCKS_PER_SEC<<" sec "<<endl;     
        cout<<" Action 2 : "<<time_2/CLOCKS_PER_SEC<<" sec "<<endl;     
        cout<<" Action 3 : "<<time_3/CLOCKS_PER_SEC<<" sec "<<endl;     
        cout<<" Action 4 : "<<time_4/CLOCKS_PER_SEC<<" sec "<<endl;
        cout<<" Total    : "<<(time_1+time_2+time_3+time_4)/CLOCKS_PER_SEC<<" sec "<<endl;      
        cout<<"==================================="<<endl;
}
