#include "../interface/PLRPlotsFurnisher.h"



PLRPlotsFurnisher::PLRPlotsFurnisher ()
{
  recognizedSystList.push_back (string ("JES"));
  recognizedSystList.push_back (string ("JER"));
  recognizedSystList.push_back (string ("METS"));
  recognizedSystList.push_back (string ("EES"));
  recognizedSystList.push_back (string ("MES"));
  recognizedSystList.push_back (string ("sfb"));
  recognizedSystList.push_back (string ("sfl"));
  //
  recognizedObs.push_back (string ("Count"));
  recognizedObs.push_back (string ("Njets"));
  recognizedObs.push_back (string ("NjetsNBjets"));
  //add recognized variables for all the taggers and w.p.
  //int algoname[number_of_cases] =     {0  ,0   ,0   ,1   ,1   , 2   ,2   ,2   ,3  ,   4    ,4    ,4    ,5   ,5   ,5   ,6    ,6     ,6    }; 
  //float discri_val[number_of_cases] = {1.7, 3.3,10.2,1.74,3.05,1.19 ,1.93,3.41,2.00,  0.275,0.545,0.790,1.33,2.55,3.74,0.244,0.679 ,0.898};   
  recognizedObs.push_back (string ("NjetsNBjets_0_1.7"));
  recognizedObs.push_back (string ("NjetsNBjets_0_1.70"));
  recognizedObs.push_back (string ("NjetsNBjets_0_1.700"));
  recognizedObs.push_back (string ("NjetsNBjets_0_3.30"));
  recognizedObs.push_back (string ("NjetsNBjets_0_10.2"));
  recognizedObs.push_back (string ("NjetsNBjets_1_1.74"));
  recognizedObs.push_back (string ("NjetsNBjets_1_3.05"));
  recognizedObs.push_back (string ("NjetsNBjets_2_1.19"));
  recognizedObs.push_back (string ("NjetsNBjets_2_1.93"));
  recognizedObs.push_back (string ("NjetsNBjets_2_3.41"));
  recognizedObs.push_back (string ("NjetsNBjets_3_2.00"));
  recognizedObs.push_back (string ("NjetsNBjets_4_0.275"));
  recognizedObs.push_back (string ("NjetsNBjets_4_0.545"));
  recognizedObs.push_back (string ("NjetsNBjets_4_0.790"));
  recognizedObs.push_back (string ("NjetsNBjets_5_1.33"));
  recognizedObs.push_back (string ("NjetsNBjets_5_2.55"));
  recognizedObs.push_back (string ("NjetsNBjets_5_3.74"));
  recognizedObs.push_back (string ("NjetsNBjets_6_0.244"));
  recognizedObs.push_back (string ("NjetsNBjets_6_0.679"));
  recognizedObs.push_back (string ("NjetsNBjets_6_0.898"));
}


PLRPlotsFurnisher::~PLRPlotsFurnisher ()
{
}

void PLRPlotsFurnisher::LoadInfoForHistoManager (vector < Dataset > Datasets, vector < string > CutName, vector < string > Channels)
{
  MyhistoManager.LoadDatasets (Datasets);
  MyhistoManager.LoadSelectionSteps (CutName);
  MyhistoManager.LoadChannels (Channels);
  datasets = Datasets;
  channels = Channels;
}


void PLRPlotsFurnisher::LoadSystematics (AnalysisEnvironmentLoader & anaEl)
{
  systList.clear ();
  ExtendedSyst syst_;
  for (unsigned int i = 0; i < recognizedSystList.size (); i++) {
    anaEl.LoadSystematics (recognizedSystList[i], syst_);
    if (syst_.doIt) {
      systList.push_back (syst_);
    }
  }
}

void PLRPlotsFurnisher::PrintSystematics ()
{
  for (unsigned int i = 0; i < systList.size (); i++) {
    Print (systList[i]);
  }
}


void PLRPlotsFurnisher::LoadSetup (AnalysisEnvironmentLoader & anaEl, vector < Dataset > Datasets, vector < string > CutName, vector < string > Channels)
{
  cout << "PLRPlotsFurnisher::LoadSetup: Loading info for histo ..." << endl;
  LoadInfoForHistoManager (Datasets, CutName, Channels);
  cout << "PLRPlotsFurnisher::LoadSetup: Loading systematics from xml file ..." << endl;
  LoadSystematics (anaEl);
  PrintSystematics ();
  cout << "PLRPlotsFurnisher::LoadSetup: Loading observables from xml file ..." << endl;
  LoadObservables (anaEl);
  PrintObservables ();
  cout << "PLRPlotsFurnisher::LoadSetup: Loading the histograms  ..." << endl;
  histoCollection.LoadSetup (channels, systList, obs);
  RefHistoCollection.LoadSetup (channels, systList, obs);
}

void PLRPlotsFurnisher::LoadObservables (AnalysisEnvironmentLoader & anaEL)
{
  vector < Observable > obs_temp;
  anaEL.LoadObservables (obs_temp);
  for (unsigned int i = 0; i < obs_temp.size (); i++) {
    for (unsigned int j = 0; j < recognizedObs.size (); j++) {
      if (obs_temp[i].name == recognizedObs[j]) {
        obs.push_back (obs_temp[i]);
        break;
      }
    }
  }
}

void PLRPlotsFurnisher::PrintObservables ()
{
  for (unsigned int i = 0; i < obs.size (); i++) {
    Print (obs[i]);
  }
}


void PLRPlotsFurnisher::CreateHistos ()
{
  histoCollection.CreateHistos ();
  RefHistoCollection.CreateHistos ();
  for (unsigned int d = 0; d < datasets.size (); d++) {
    for (unsigned int c = 0; c < channels.size (); c++) {
      for (unsigned int o = 0; o < obs.size (); o++) {
        //Reference plot: no systematic
        string hname = channels[c] + "_" + obs[o].name;
        string Yaxis = string ("entries");
        if (obs[o].dim == 1) {
          MyhistoManager.CreateHisto (RefHistoCollection.GetHisto1D (c, o, 0, 0), hname, datasets[d].Name (), obs[o].axisName[0], Yaxis, obs[o].NofBins[0], obs[o].axisRange[0].first,
                                      obs[o].axisRange[0].second);
        }
        if (obs[o].dim == 2) {
          MyhistoManager.CreateHisto2D (RefHistoCollection.GetHisto2D (c, o, 0, 0), hname, datasets[d].Name (), obs[o].axisName[0], obs[o].NofBins[0], obs[o].axisRange[0].first,
                                        obs[o].axisRange[0].second, obs[o].axisName[1], obs[o].NofBins[1], obs[o].axisRange[1].first, obs[o].axisRange[1].second);
        }
        /////////////////////////////////
        for (unsigned int s = 0; s < systList.size (); s++) {
          string hname = channels[c] + "_" + obs[o].name + "_" + systList[s].name;
          if (systList[s].discretized) {
            string hnamePlus = hname + "-plus";
            string hnameMinus = hname + "-minus";
            string Yaxis = string ("entries");
            if (obs[o].dim == 1) {
              // 0 correspond to minus and 1 to plus
              MyhistoManager.CreateHisto (histoCollection.GetHisto1D (c, o, s, 0), hnameMinus, datasets[d].Name (), obs[o].axisName[0], Yaxis, obs[o].NofBins[0], obs[o].axisRange[0].first,
                                          obs[o].axisRange[0].second);
              MyhistoManager.CreateHisto (histoCollection.GetHisto1D (c, o, s, 1), hnamePlus, datasets[d].Name (), obs[o].axisName[0], Yaxis, obs[o].NofBins[0], obs[o].axisRange[0].first,
                                          obs[o].axisRange[0].second);
            }
            if (obs[o].dim == 2) {
              MyhistoManager.CreateHisto2D (histoCollection.GetHisto2D (c, o, s, 0), hnameMinus, datasets[d].Name (), obs[o].axisName[0], obs[o].NofBins[0], obs[o].axisRange[0].first,
                                            obs[o].axisRange[0].second, obs[o].axisName[1], obs[o].NofBins[1], obs[o].axisRange[1].first, obs[o].axisRange[1].second);
              MyhistoManager.CreateHisto2D (histoCollection.GetHisto2D (c, o, s, 1), hnamePlus, datasets[d].Name (), obs[o].axisName[0], obs[o].NofBins[0], obs[o].axisRange[0].first,
                                            obs[o].axisRange[0].second, obs[o].axisName[1], obs[o].NofBins[1], obs[o].axisRange[1].first, obs[o].axisRange[1].second);
            }
          }
          else {
            if (obs[o].dim == 2 && (systList[s].name == string ("sfb") || systList[s].name == string ("sfl"))) {
              //take care ... works only for b-tagging
              //cout<<"Create plots for syst "<<systList[s].name<<endl;
              string hnameV = hname + "_0b";
              MyhistoManager.CreateHisto2D (histoCollection.GetHisto2D (c, o, s, 0), hnameV, datasets[d].Name (), obs[o].name, obs[o].axisName[0], systList[s].values.size (),
                                            systList[s].values[0] - (systList[s].values[1] - systList[s].values[0]) / 2.,
                                            systList[s].values[systList[s].values.size () - 1] + (systList[s].values[1] - systList[s].values[0]) / 2., obs[o].NofBins[0], obs[o].axisRange[0].first,
                                            obs[o].axisRange[0].second);
              hnameV = hname + "_1b";
              MyhistoManager.CreateHisto2D (histoCollection.GetHisto2D (c, o, s, 0), hnameV, datasets[d].Name (), obs[o].name, obs[o].axisName[0], systList[s].values.size (),
                                            systList[s].values[0] - (systList[s].values[1] - systList[s].values[0]) / 2.,
                                            systList[s].values[systList[s].values.size () - 1] + (systList[s].values[1] - systList[s].values[0]) / 2., obs[o].NofBins[0], obs[o].axisRange[0].first,
                                            obs[o].axisRange[0].second);
              hnameV = hname + "_2b";
              MyhistoManager.CreateHisto2D (histoCollection.GetHisto2D (c, o, s, 0), hnameV, datasets[d].Name (), obs[o].name, obs[o].axisName[0], systList[s].values.size (),
                                            systList[s].values[0] - (systList[s].values[1] - systList[s].values[0]) / 2.,
                                            systList[s].values[systList[s].values.size () - 1] + (systList[s].values[1] - systList[s].values[0]) / 2., obs[o].NofBins[0], obs[o].axisRange[0].first,
                                            obs[o].axisRange[0].second);
              hnameV = hname + "_3b";
              MyhistoManager.CreateHisto2D (histoCollection.GetHisto2D (c, o, s, 0), hnameV, datasets[d].Name (), obs[o].name, obs[o].axisName[0], systList[s].values.size (),
                                            systList[s].values[0] - (systList[s].values[1] - systList[s].values[0]) / 2.,
                                            systList[s].values[systList[s].values.size () - 1] + (systList[s].values[1] - systList[s].values[0]) / 2., obs[o].NofBins[0], obs[o].axisRange[0].first,
                                            obs[o].axisRange[0].second);
            }
            else
              cerr << "PLRPlotsFurnisher::CreateHistos:: Dim !2 is not implemented there !!" << endl;
          }
        }
      }
    }
  }

}


void PLRPlotsFurnisher::WriteHistos ()
{
  for (unsigned int c = 0; c < channels.size (); c++) {
    for (unsigned int o = 0; o < obs.size (); o++) {
      //Reference histo
      if (obs[o].dim == 1) {
        MyhistoManager.WriteMyHisto (RefHistoCollection.GetHisto1D (c, o, 0, 0), string ("all"));
      }
      if (obs[o].dim == 2) {
        MyhistoManager.WriteMyHisto2D (RefHistoCollection.GetHisto2D (c, o, 0, 0), string ("all"));
      }
      /////////////////////////////////
      for (unsigned int s = 0; s < systList.size (); s++) {
        //cout<<"Write histo for channel "<<channels[c]<<" obs: "<<obs[o].name<<" syst: "<<systList[s].name<<endl;
        if (systList[s].discretized) {
          if (obs[o].dim == 1) {
            MyhistoManager.WriteMyHisto (histoCollection.GetHisto1D (c, o, s, 0), string ("all"));
            MyhistoManager.WriteMyHisto (histoCollection.GetHisto1D (c, o, s, 1), string ("all"));
          }
          if (obs[o].dim == 2) {
            MyhistoManager.WriteMyHisto2D (histoCollection.GetHisto2D (c, o, s, 0), string ("all"));
            MyhistoManager.WriteMyHisto2D (histoCollection.GetHisto2D (c, o, s, 1), string ("all"));
          }
        }
        else {
          if (obs[o].dim == 2 && (systList[s].name == string ("sfb") || systList[s].name == string ("sfl"))) {
            MyhistoManager.WriteMyHisto2D (histoCollection.GetHisto2D (c, o, s, 0), string ("all"));
          }
        }
      }
    }
  }
}


void PLRPlotsFurnisher::LoadEvent (const DiLeptonSelection & sel, const string & ChannelName, const bool & IsSignal,
                                   const int &itypemc, const int &tmeme, double weight, Dataset & dataset, const double &sf_trig_mu,
                                   const double &sf_trig_emu, const double &sf_trig_ee, const double &sf_e, const double &sf_mu,
                                   const double &sf_dy_ee, const double &sf_dy_mm, const double &sf_dy_em, const int &nrecoel, const string & CandType0, const double &LeptonSF)
{
  string DatasetName = dataset.Name ();
  DiLeptonSelectionSystematics systSel (sel);

// modifdg : pour prendre en compte les SF (trigger eff, lepton eff et DY ) correctement
// NB le SF DY n'est applique QU"APRES la coupure en MET, ici il est aussi appliquer arbitrairement AVANT car en principe on ne
// determine pas la section efficace AVANT l'application de la coupure en MET (donc meme si c'est appliquer, ce n'est pas grave)

//   if ( ChannelName=="ee" ) std::cout<<"code de schtroumpf before "<<weight<<std::endl;

  if (ChannelName == "mumu" && !dataset.isData ())
    weight *= sf_trig_mu;
  if (ChannelName == "ee" && !dataset.isData ())
    weight *= sf_trig_ee;
  if (ChannelName == "emu" && !dataset.isData ())
    weight *= sf_trig_emu;

  if (ChannelName == "ee" && !dataset.isData ()) {
    if (dataset.Name () == "TTbar") {
      // ee
      if (tmeme == 2 || tmeme == 10101 || tmeme == 20200)
        weight = weight * sf_e * sf_e;
      // emu
      if (tmeme == 11 || tmeme == 21100 || tmeme == 11001 || tmeme == 10110)
        weight = weight * sf_e;
      // ejet
      if (tmeme == 1 || tmeme == 10100)
        weight = weight * sf_e;
    }
    if (dataset.Name () == "Zjets" || dataset.Name () == "DYToEE_M-20" || dataset.Name () == "DYToTauTau_M-20" || dataset.Name () == "DYToEE_M-10To20" || dataset.Name () == "DYToTauTau_M-10To20")
      weight = weight * sf_e * sf_e;
    if (dataset.Name () == "WZ" || dataset.Name () == "WW" || dataset.Name () == "ZZ" || dataset.Name () == "VV")
      weight = weight * sf_e * sf_e;
    if (dataset.Name () == "WjetsE" || dataset.Name () == "WjetsTau" || dataset.Name () == "Wjets")
      weight = weight * sf_e;
    if (dataset.Name () == "SingleToptW" || dataset.Name () == "TtW" || dataset.Name () == "TbartW")
      weight = weight * sf_e * sf_e;
  }

  if (ChannelName == "emu" && !dataset.isData ()) {
    if (dataset.Name () == "TTbar") {
      // ee
      if (tmeme == 2 || tmeme == 10101 || tmeme == 20200) {
        weight = weight * sf_e;
        if (nrecoel > 1)
          weight = weight * sf_e;
      }
      // emu
      if (tmeme == 11 || tmeme == 21100 || tmeme == 11001 || tmeme == 10110)
        weight = weight * sf_e;
      // ejet
      if (tmeme == 1 || tmeme == 10100)
        weight = weight * sf_e;
    }
    if (
         // dataset.Name()=="Zjets"          --> hyp: real mu, fake e
         dataset.Name () == "DYToEE_M-20" || dataset.Name () == "DYToEE_M-10To20"
         // DYTauTau -->> hyp : fake e
      ) {
      weight = weight * sf_e;
      if (nrecoel > 1)
        weight = weight * sf_e;
    }
    if (dataset.Name () == "WZ" || dataset.Name () == "WW" || dataset.Name () == "ZZ" || dataset.Name () == "VV") {
      weight = weight * sf_e;
      if (nrecoel > 1)
        weight = weight * sf_e;
    }
//       if ( dataset.Name()=="WjetsE"  || dataset.Name()=="WjetsTau" ) --> hyp : real mu, fake e
    if (dataset.Name () == "SingleToptW" || dataset.Name () == "TtW" || dataset.Name () == "TbartW")
      weight = weight * sf_e;
  }

//   std::cout<<"CandType0 "<<CandType0<<" "<<LeptonSF<<std::endl;
  if (CandType0 == "mumu" && !dataset.isData () && (dataset.Name () != "Wjets" && dataset.Name () != "QCD" && !(dataset.Name () == "TTbar" && !IsSignal))) {
//    || (dataset.Name()  =="TTbar" &&  IsSignal)  )   ) {
    weight *= LeptonSF;
  }
  if (CandType0 == "emu" && !dataset.isData ()
      && (dataset.Name () != "Wjets" && dataset.Name () != "QCD" && !(dataset.Name () == "TTbar" && !IsSignal))) {
//    || (dataset.Name()  =="TTbar" &&  IsSignal)         ) ){
    weight *= LeptonSF;
  }
  if (CandType0 == "ee" && !dataset.isData ()
      && (dataset.Name () != "Wjets" && dataset.Name () != "QCD" && !(dataset.Name () == "TTbar" && !IsSignal))) {
//    || (dataset.Name()  =="TTbar" &&  IsSignal)         ) ) {
    weight *= LeptonSF;
  }
// NB SF_mu pas code dans MyCutFlow.C (car ==1): il n'apparait pas ici non plus 

  if (ChannelName == "mumu") {
    if (itypemc == 2 || itypemc == 6 || itypemc == 7) {
      weight *= sf_dy_mm;
    }
  }
  else if (ChannelName == "emu") {
    if (itypemc == 2 || itypemc == 6 || itypemc == 7) {
      weight *= sf_dy_em;
    }
  }
  else if (ChannelName == "ee") {
    if (itypemc == 2 || itypemc == 6 || itypemc == 7) {
      weight *= sf_dy_ee;
    }
  }
//endmodifdg


  //Produce reference plots
  for (unsigned int c = 0; c < channels.size (); c++) {
    if (ChannelName != channels[c])
      continue;
    bool selected = systSel.ApplySelection (dataset, channels[c]);
    if (selected) {
      for (unsigned int o = 0; o < obs.size (); o++) {
        string hname = channels[c] + "_" + obs[o].name;
        if (obs[o].name == string ("Count")) {
          MyhistoManager.FillHisto (RefHistoCollection.GetHisto1D (c, o, 0, 0), hname, 0., DatasetName, IsSignal, weight);
        }
        if (obs[o].name == string ("Njets")) {
          MyhistoManager.FillHisto (RefHistoCollection.GetHisto1D (c, o, 0, 0), hname, systSel.GetJetMultiplicity (), DatasetName, IsSignal, weight);
        }
        if (obs[o].name.find (string ("NjetsNBjets")) < obs[o].name.size ()) {  //include all the observable starting with NjetsNBjets
          //Recompute the b-tagging depending on the algo & working point
          AnalysisEnvironmentLoader anEL;
          vector < string > out_string;
          anEL.StringSeparator (obs[o].name, string ("_"), out_string);
          if (out_string.size () == 3) {        //otherwise it's just the default variable NjetsNBjets
            int algo = atoi (out_string[1].c_str ());
            float wp = atof (out_string[2].c_str ());
            systSel.RecomputeBtagWeight (algo, wp);     //then the systSel.GetBtagWeight() output will be changed !
            systSel.SetBtagAlgo(algo);
            systSel.SetBtagDiscriCut(wp);
            //cout<<"Before apply selection"<<endl;
            systSel.ApplySelection(dataset,channels[c]);
            //cout<<"After apply selection"<<endl;
          }
          else if(obs[o].name!=string ("NjetsNBjets")) cerr<<"Warning:: problem to find the algo and wp in the variable name: "<<obs[o].name<<endl;
          if (!dataset.isData ()) {
            //0 b-jet
            MyhistoManager.FillHisto2D (RefHistoCollection.GetHisto2D (c, o, 0, 0), hname, systSel.GetJetMultiplicity (), 0., DatasetName, IsSignal, systSel.GetBtagWeight ()[1] * weight);
            //1 b-jet
            MyhistoManager.FillHisto2D (RefHistoCollection.GetHisto2D (c, o, 0, 0), hname, systSel.GetJetMultiplicity (), 1., DatasetName, IsSignal, systSel.GetBtagWeight ()[2] * weight);
            //2 b-jet
            MyhistoManager.FillHisto2D (RefHistoCollection.GetHisto2D (c, o, 0, 0), hname, systSel.GetJetMultiplicity (), 2., DatasetName, IsSignal, systSel.GetBtagWeight ()[3] * weight);
            //3 b-jet
            MyhistoManager.FillHisto2D (RefHistoCollection.GetHisto2D (c, o, 0, 0), hname, systSel.GetJetMultiplicity (), 3., DatasetName, IsSignal, systSel.GetBtagWeight ()[4] * weight);
          }
          else {
            float nbtaggedJets = systSel.GetNofBtaggedJets ();
            if(nbtaggedJets>3) nbtaggedJets = 3; //bin 3 is inclusive
            MyhistoManager.FillHisto2D (RefHistoCollection.GetHisto2D (c, o, 0, 0), hname, systSel.GetJetMultiplicity (), nbtaggedJets, DatasetName, IsSignal, weight);
          }
        }
      }
    }
//      if ( ChannelName=="ee" ) std::cout<<"code de schroumf after "<<weight<<std::endl;       
  }

  //test: removed to avoid crash
  if (dataset.isData ())
    return;                     //don't compute any systematic

  for (unsigned int s = 0; s < systList.size (); s++) {
    if (systList[s].discretized) {
      //cout<<"discretized"<<endl;
      for (unsigned int c = 0; c < channels.size (); c++) {
        if (ChannelName != channels[c])
          continue;
        //Case of -1 sigma
        systSel.Clear ();
        systSel.LoadSystematics (systList[s].name, systList[s].m1sigma);
        bool selected = systSel.ApplySelection (dataset, channels[c]);
        if (selected) {
          for (unsigned int o = 0; o < obs.size (); o++) {
            string hname = channels[c] + "_" + obs[o].name + "_" + systList[s].name;
            string hnameMinus = hname + "-minus";
            if (obs[o].name == string ("Count")) {
              MyhistoManager.FillHisto (histoCollection.GetHisto1D (c, o, s, 0), hnameMinus, 0., DatasetName, IsSignal, weight);
            }
            if (obs[o].name == string ("Njets")) {
              MyhistoManager.FillHisto (histoCollection.GetHisto1D (c, o, s, 0), hnameMinus, systSel.GetJetMultiplicity (), DatasetName, IsSignal, weight);
            }
            if (obs[o].name.find (string ("NjetsNBjets")) < obs[o].name.size ()) {      //include all the observable starting with NjetsNBjets
              //Recompute the b-tagging depending on the algo & working point
              AnalysisEnvironmentLoader anEL;
              vector < string > out_string;
              anEL.StringSeparator (obs[o].name, string ("_"), out_string);
              if (out_string.size () == 3) {    //otherwise it's just the default variable NjetsNBjets
                int algo = atoi (out_string[1].c_str ());
                float wp = atof (out_string[2].c_str ());
                systSel.RecomputeBtagWeight (algo, wp); //then the systSel.GetBtagWeight() output will be changed !
              }
              else if(obs[o].name!=string ("NjetsNBjets")) cerr<<"Warning:: problem to find the algo and wp in the variable name: "<<obs[o].name<<endl;
              //0 b-jet
              MyhistoManager.FillHisto2D (histoCollection.GetHisto2D (c, o, s, 0), hnameMinus, systSel.GetJetMultiplicity (), 0., DatasetName, IsSignal, systSel.GetBtagWeight ()[1] * weight);
              //1 b-jet
              MyhistoManager.FillHisto2D (histoCollection.GetHisto2D (c, o, s, 0), hnameMinus, systSel.GetJetMultiplicity (), 1., DatasetName, IsSignal, systSel.GetBtagWeight ()[2] * weight);
              //2 b-jet
              MyhistoManager.FillHisto2D (histoCollection.GetHisto2D (c, o, s, 0), hnameMinus, systSel.GetJetMultiplicity (), 2., DatasetName, IsSignal, systSel.GetBtagWeight ()[3] * weight);
              //3 b-jet
              MyhistoManager.FillHisto2D (histoCollection.GetHisto2D (c, o, s, 0), hnameMinus, systSel.GetJetMultiplicity (), 3., DatasetName, IsSignal, systSel.GetBtagWeight ()[4] * weight);
            }
          }
        }
        systSel.Clear ();
        //Clear has to be done before Loading syst'
        //Case of +1 sigma
        systSel.LoadSystematics (systList[s].name, systList[s].p1sigma);
        //if(!selected && systSel.ApplySelection(dataset,channels[c])) cout<<"WARNING >>>> "<<endl;
        selected = systSel.ApplySelection (dataset, channels[c]);
        if (selected) {
          for (unsigned int o = 0; o < obs.size (); o++) {
            string hname = channels[c] + "_" + obs[o].name + "_" + systList[s].name;
            string hnamePlus = hname + "-plus";
            if (obs[o].name == string ("Count")) {
              MyhistoManager.FillHisto (histoCollection.GetHisto1D (c, o, s, 1), hnamePlus, 0., DatasetName, IsSignal, weight);
            }
            if (obs[o].name == string ("Njets")) {
              MyhistoManager.FillHisto (histoCollection.GetHisto1D (c, o, s, 1), hnamePlus, systSel.GetJetMultiplicity (), DatasetName, IsSignal, weight);
            }
            if (obs[o].name.find (string ("NjetsNBjets")) < obs[o].name.size ()) {      //include all the observable starting with NjetsNBjets
              //Recompute the b-tagging depending on the algo & working point
              AnalysisEnvironmentLoader anEL;
              vector < string > out_string;
              anEL.StringSeparator (obs[o].name, string ("_"), out_string);
              if (out_string.size () == 3) {    //otherwise it's just the default variable NjetsNBjets
                int algo = atoi (out_string[1].c_str ());
                float wp = atof (out_string[2].c_str ());
                systSel.RecomputeBtagWeight (algo, wp); //then the systSel.GetBtagWeight() output will be changed !
                //cout<<"algo = "<<algo<<" wp = "<<wp<<" weight[1]: "<<systSel.GetBtagWeight ()[1]<<endl;
              }
              else if(obs[o].name!=string ("NjetsNBjets")) cerr<<"Warning:: problem to find the algo and wp in the variable name: "<<obs[o].name<<endl;
              //cout<<" weight[1]: "<<systSel.GetBtagWeight ()[1]<<endl;
              //0 b-jet
              MyhistoManager.FillHisto2D (histoCollection.GetHisto2D (c, o, s, 1), hnamePlus, systSel.GetJetMultiplicity (), 0., DatasetName, IsSignal, systSel.GetBtagWeight ()[1] * weight);
              //1 b-jet
              MyhistoManager.FillHisto2D (histoCollection.GetHisto2D (c, o, s, 1), hnamePlus, systSel.GetJetMultiplicity (), 1., DatasetName, IsSignal, systSel.GetBtagWeight ()[2] * weight);
              //2 b-jet
              MyhistoManager.FillHisto2D (histoCollection.GetHisto2D (c, o, s, 1), hnamePlus, systSel.GetJetMultiplicity (), 2., DatasetName, IsSignal, systSel.GetBtagWeight ()[3] * weight);
              //3 b-jet
              MyhistoManager.FillHisto2D (histoCollection.GetHisto2D (c, o, s, 1), hnamePlus, systSel.GetJetMultiplicity (), 3., DatasetName, IsSignal, systSel.GetBtagWeight ()[4] * weight);
            }
          }
        }
      }
    }
    else {                      //not discretized
      for (unsigned int c = 0; c < channels.size (); c++) {
        if (ChannelName != channels[c])
          continue;
        for (unsigned int x = 0; x < systList[s].values.size (); x++) {
          systSel.Clear ();
          systSel.LoadSystematics (systList[s].name, systList[s].values[x]);
          bool selected = systSel.ApplySelection (dataset, channels[c]);
          if (selected) {
            for (unsigned int o = 0; o < obs.size (); o++) {
              string hname = channels[c] + "_" + obs[o].name + "_" + systList[s].name;
              //if (obs[o].name == string ("Count") || obs[o].name == string ("Njets")) {
              if (obs[o].name == string ("Count") || obs[o].name.find (string ("NjetsNBjets")) < obs[o].name.size ()) {
                float yvalue = -1.;
                if (obs[o].name == string ("Count"))
                  yvalue = 0.;
                //if (obs[o].name == string ("Njets"))
                if (obs[o].name.find (string ("NjetsNBjets")) < obs[o].name.size ()){
                  yvalue = systSel.GetJetMultiplicity ();
                  //NEW ......................................
                  //Recompute the b-tagging depending on the algo & working point
                 AnalysisEnvironmentLoader anEL;
                 vector < string > out_string;
                 anEL.StringSeparator (obs[o].name, string ("_"), out_string);
                 if (out_string.size () == 3) { //otherwise it's just the default variable NjetsNBjets
                   int algo = atoi (out_string[1].c_str ());
                   float wp = atof (out_string[2].c_str ());
                   systSel.RecomputeBtagWeight (algo, wp);      //then the systSel.GetBtagWeight() output will be changed !
                   //cout<<"algo = "<<algo<<" wp = "<<wp<<" weight[1]: "<<systSel.GetBtagWeight ()[1]<<endl;
                 }
                 else if(obs[o].name!=string ("NjetsNBjets")) cerr<<"Warning:: problem to find the algo and wp in the variable name: "<<obs[o].name<<endl;
                  //END NEW ......................................
                }
                //cout<<"FILL:: "<<hname<<" "<<obs[o].name<<endl;
                string hnameLocal = hname + "_0b";
                MyhistoManager.FillHisto2D (histoCollection.GetHisto2D (c, o, s, 0), hnameLocal, systList[s].values[x], yvalue, DatasetName, IsSignal, systSel.GetBtagWeight ()[1] * weight);
                hnameLocal = hname + "_1b";
                MyhistoManager.FillHisto2D (histoCollection.GetHisto2D (c, o, s, 0), hnameLocal, systList[s].values[x], yvalue, DatasetName, IsSignal, systSel.GetBtagWeight ()[2] * weight);
                hnameLocal = hname + "_2b";
                MyhistoManager.FillHisto2D (histoCollection.GetHisto2D (c, o, s, 0), hnameLocal, systList[s].values[x], yvalue, DatasetName, IsSignal, systSel.GetBtagWeight ()[3] * weight);
                hnameLocal = hname + "_3b";
                MyhistoManager.FillHisto2D (histoCollection.GetHisto2D (c, o, s, 0), hnameLocal, systList[s].values[x], yvalue, DatasetName, IsSignal, systSel.GetBtagWeight ()[4] * weight);
              }
            }
          }
        }
      }
    }                           //fin else
  }

}

int PLRPlotsFurnisher::GetiDataset (string DatasetName)
{
  for (unsigned int i = 0; i < datasets.size (); i++) {
    if (datasets[i].Name () == DatasetName)
      return (int) i;
  }
  return -1;
}

