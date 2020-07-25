#include <iomanip>
#include <iostream>
#include "../../../../MiniTreeFormat/NTFormat/interface/NTEvent.h"

//NTupleAnalysis classes
#include "../../Selection/interface/SelectionTable.h"
#include "../../Tools/interface/Dataset.h"
#include "../../Tools/interface/AnalysisEnvironmentLoader.h"
#include "../../Plots/interface/SemiLeptonicTauAnaHistoManager.h"
#include "../../Selection/interface/SemiLeptonicTauSelection.h"
#include "../../Tools/interface/PUWeighting.h"
#include "../../Tools/interface/LumiReweightingStandAlone.h"

#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TRandom.h>

#include "TMVA/Reader.h"

using namespace TopTree;
using namespace std;

int main ()
{
 
  clock_t start = clock();
  clock_t tl_tableDATA = 0;
  clock_t tl_tableMC = 0;
  clock_t time;
 
  cout<<"#########################"<<endl;
  cout<<"Beginning of the program"<<endl;
  cout<<"#########################"<<endl;
  
  //////////////////////
  //Global variables
  //////////////////////
  vector < Dataset > datasets;
  SemiLeptonicTauSelection sel; 
  float Luminosity = 0.;
  float LumiError = 0.;
  // 0: MC - 1: Data - 2 Data & MC
  int DataType = 0; 
  int verbosity = -1;
  string PUWeightFileName;

  //////////////////////
 
  //////////////////////
  // Initialisation
  //////////////////////
  string xmlFileName = string ("../../config/SemiLeptonicTauXsectionMeasurement.xml");
  AnalysisEnvironmentLoader anaEL (xmlFileName);
  anaEL.LoadSamples (datasets); // now the list of datasets written in the xml file is known
  anaEL.LoadSemiLeptonicTauSelection (sel); // now the parameters for the selection are given to the selection
  anaEL.LoadGeneralInfo(DataType, Luminosity, verbosity );
  anaEL.LoadWeight (sel); // now the parameters for SFBweight are initialized (for b-tag!)
  //int flagOriginal=sel.GetFlagb();
  //int methodOriginal=sel.GetMethodb();
  //int systOriginal= sel.GetSystb();
  //std::cout << " For btag : flag " << flagOriginal << ", method " << methodOriginal << ", syst " << systOriginal << std::endl;
  TopTree::NTEvent * event = 0;
  
  //////////////////////
  // PDF uncertainties
  //  
  bool PDFsys= true;
  float Npdf[145]={0.};
  
  //////////////////////
  //Haryo's pull
  //
  /*
  std::vector<float> W;// weights of selected events in the final state
  std::vector<float> C;//sum of weights of selected events in the final state
  std::vector<float> Cidx;//indices of selected events in the final state
  std::vector<float> CNN;//NN output of selected events in the final state
  float S = 0.;//sum of weights of events selected in the final state

  TFile *_file_PE_matrix ;
  _file_PE_matrix = TFile::Open("MatrixIdxSorted_multijet.root");
  _file_PE_matrix->cd();
  TMatrixD*  matrixIdx = (TMatrixD*)_file_PE_matrix->Get("matrixIdx");
  TMatrixD*  matrixNN  = (TMatrixD*)_file_PE_matrix->Get("matrixNN");

  int subSampleIdx = 299;//tobeModified
*/
  //////////////////////
  // Reweighting
  //////////////////////
  
  bool IReweight = true;
  if (DataType == 1) IReweight = false;
  
  // 3D reweighting
  bool  IReweight_puUp	 = false;
  bool  IReweight_puDown = false;
 
  string datafile = "";
  string mcfile   = "rootFiles/PU3DMC.root";
  
  if(!IReweight_puUp && !IReweight_puDown) datafile = "rootFiles/MyDataPileupHistogram_observed_73500.root";
  if( IReweight_puDown  		 ) datafile = "rootFiles/MyDataPileupHistogram_observed_67620.root";
  if( IReweight_puUp			 ) datafile = "rootFiles/MyDataPileupHistogram_observed_79380.root";

  reweight::LumiReWeighting *LumiWeights = new reweight::LumiReWeighting(mcfile, datafile, "histoMCPU", "pileup" );
            
  if(!IReweight_puUp && !IReweight_puDown)  LumiWeights->weight3D_init( 1. );
  if( IReweight_puDown  		 )  LumiWeights->weight3D_init( 1. );
  if( IReweight_puUp			 )  LumiWeights->weight3D_init( 1. );
  
  
  ///////////////////////
  
  sel.GetCutList();
  
  //Selection table
  SelectionTable selTable(sel.GetCutList(),datasets,string("tauhjetjet"));
  SelectionTable selDATATable(sel.GetCutList (),datasets,string ("tauhjetjet"));
  

  //Book keeping of standard histos
  SemiLeptonicTauAnaHistoManager histoManager;
  histoManager.LoadDatasets(datasets);    
  histoManager.LoadSelectionSteps(sel.GetCutList());
  histoManager.LoadChannels(sel.GetChannelList());
  histoManager.CreateHistos();	
  //////////////////////
  
  cout<<"The verbosity mode is "<<verbosity <<endl;
  cout<<"The luminosity is equal to "<< Luminosity<<endl;
  cout<<"The DataType is ";
  switch(DataType){
  	case 0: 
		cout<<"MC"<<endl; 
		break;
	case 1:
		cout<<"Data"<<endl; 
		break;
	case 2:
		cout<<"Data & MC"<<endl; 
		break;
	default:
		cout<<" unknown"<<endl; 
		break;
  }
  //////////////////////

  //////////////////////
  //LOOP OVER THE DATASETS
  //////////////////////
  if(verbosity>0) {
  	cout<<"#########################"<<endl;
  	cout<<" Loop over the datasets  "<<endl;
	cout<<"#########################"<<endl;
  }
  
  for (unsigned int d = 0; d < datasets.size (); d++) {
 
    datasets[d].eventTree ()->SetBranchAddress ("NTEvent",&event);

    unsigned int nEvents = (int) (datasets[d].eventTree ()->GetEntries ());
    cout << "NEvents = " << nEvents << endl;
    cout <<" NEvents to run over, skimmed events = "<<datasets[d].NofEvtsToRunOver()<<" "<<datasets[d].getNSkimmedEvent()<<endl;
     
    //Haryo's pull 
    //float sumWeights =0.;
   
    /////////////////////
    //LOOP OVER THE EVENTS
    /////////////////////
    
    TRandom random;
    for (unsigned int ievt = 0; ievt < datasets[d].NofEvtsToRunOver(); ievt++) {
    
      float weight = 1.;
      if(datasets[d].isData() == false) //weight = datasets[d].NormFactor()*Luminosity; //if Data , weight = 1
      weight = Luminosity*datasets[d].Xsection()/datasets[d].getNSkimmedEvent();
      
      //cout<<"**weight "<<weight<<" " << Luminosity << " " << datasets[d].Xsection()<<" " << datasets[d].getNSkimmedEvent()<< endl;
     
      datasets[d].eventTree ()->GetEntry (ievt);
   
      if (verbosity > 3){
	cout << "event " << ievt <<" - event number=" << event->eventNb <<" - run number=" << event->runNb << endl;
      }
      if (ievt % 10000 == 0)
	      cout << "number of events " << ievt << endl;


     sel.LoadEvent(event);
      
     int selLastStep = 0;
     
     
     /////////////////////
     //triggerMenu
     /////////////////////
     
     int triggerMenu = 0;
     if(datasets[d].isData() == false)
     {
       float trigRange = random.Uniform(1);
       if(trigRange<0.218)//0.208 new lumi	
       triggerMenu = 40;
       else triggerMenu = 45;}
     
     
     /////////////////////
     //PU reweighting
     /////////////////////
     
     if(datasets[d].isData() == false && IReweight == true) weight *= LumiWeights->weight3D(event->num_pileup_bcm1 ,event->num_pileup_bc0,event->num_pileup_bcp1); 
     
     /////////////////////
     //PDF reweighting 
     /////////////////////
 
     if(datasets[d].isData() == false && PDFsys == true) 
     {
       std::vector < float > pdfWeights = event->pdfWeights ;
       //std::cout <<"pdfWeights.size() " << pdfWeights.size() <<" "<< pdfWeights[0] <<" "<< pdfWeights[45]<< std::endl;
       for (unsigned int k = 0; k<pdfWeights.size(); k++) Npdf[k] += pdfWeights[k];      
       //std::cout <<"pdfWeights[11]/pdfWeights[0] " <<pdfWeights[11]/pdfWeights[0] <<  std::endl; 
       weight *= pdfWeights[11]/pdfWeights[0];
      }
     
     
     /////////////////////
     //Fill tables
     /////////////////////
     
     if(datasets[d].isData()){
	 time = clock(); 
	 selLastStep = sel.FillTable (selDATATable, &(datasets[d]), d, weight, triggerMenu); 
	 tl_tableDATA+=clock()-time;}
     else{  
         time = clock(); 
	
         selLastStep = sel.FillTable(selTable, &(datasets[d]), d, weight, triggerMenu); 
	 tl_tableMC+=clock()-time;}
     
     
      /////////////////////
      //Haryo's method
      /////////////////////
      /*
      bool randomSel = false;
      int oMatched = -1;
      if(selLastStep==7) 
      {
        //S+=weight*sel.getWeightAtLeast1bjet()*sel.getWeightTauLeg(triggerMenu)*sel.getWeightJetLegs(triggerMenu);//TTbar
        S+=weight*sel.getWeightAtLeast1bjet();//Multijet
	C.push_back(S);
	Cidx.push_back(ievt);
	CNN.push_back(sel.GetNNOutputAndKinFit()[0]);
	//W.push_back(weight*sel.getWeightAtLeast1bjet()*sel.getWeightTauLeg(triggerMenu)*sel.getWeightJetLegs(triggerMenu));//TTbar
        W.push_back(weight*sel.getWeightAtLeast1bjet());//Multijet    
       }
     
      // uncomment to create PE
      
      for (int o=0; o<2399; o++) 
        {
	 //std::cout <<(*matrixIdx)(subSampleIdx,o) <<" " << ievt << std::endl;
         if ((*matrixIdx)(subSampleIdx,o)==ievt) {randomSel = true; oMatched=o;}
       }
       
      if (randomSel )
      {
       std::vector<float> NNKinFit;
       NNKinFit.push_back((*matrixNN)(subSampleIdx,oMatched));
       NNKinFit.push_back(0.);
       NNKinFit.push_back(0.); 
       NNKinFit.push_back(0.);
       histoManager.Fill(sel, 7, 0, d, 1.,NNKinFit );
       }*/
       
       
      
      //Fill histograms
      if(!datasets[d].isData() && selLastStep>5)   
      histoManager.Fill(sel, selLastStep, 0, d, weight*sel.getWeightAtLeast1bjet()*sel.getWeightTauLeg(triggerMenu)*sel.getWeightJetLegs(triggerMenu), sel.GetNNOutputAndKinFit());
      //histoManager.Fill(sel, selLastStep, 0, d, weight*sel.getWeightAtLeast1bjet(), sel.GetNNOutputAndKinFit());
      //histoManager.Fill(sel, selLastStep, 0, d, weight*sel.getWeightTau()*sel.getWeightTauLeg(triggerMenu)*sel.getWeightJetLegs(triggerMenu), sel.GetNNOutputAndKinFit());   
      else if(!datasets[d].isData() && selLastStep>3 && selLastStep<=5)   
      histoManager.Fill(sel, selLastStep, 0, d, weight*sel.getWeightTau()*sel.getWeightTauLeg(triggerMenu)*sel.getWeightJetLegs(triggerMenu), sel.GetNNOutputAndKinFit());
      else
      histoManager.Fill(sel, selLastStep, 0, d, weight, sel.GetNNOutputAndKinFit());
     
    
    } // end of loop over evts
  }   // end of loop over the datasets 
  
  cout<<"#########################"<<endl;
  cout<<" Loop over the datasets  "<<endl;
  cout<<"#########################"<<endl;
 
  histoManager.Compute();
 
  ////////////////////////////
  //  Computation after loops
  ////////////////////////////
  
  ////////////////////////////
  //Haryo's method
  ////////////////////////////
  //To be done only once
  // First time measure S and C and Cidx
  // use Suite_Haryo.C and Indices_Haryo.C
  // Then match events with event indices stored in matrixIdx 
  /*std::cout <<"S " <<S <<std::endl; 
  std::cout <<"C.size() " <<C.size() <<std::endl;
  std::cout <<"C[4527]={";
  for (int r =0; r < C.size(); r++) std::cout <<C[r] <<",";
  std::cout <<"}"<<std::endl;
  std::cout <<"Cidx[4527]={";
  for (int r =0; r < Cidx.size(); r++) std::cout <<Cidx[r] <<",";
  std::cout <<"}"<<std::endl;
  std::cout <<"CNN[4527]={";
  for (int r =0; r < CNN.size(); r++) std::cout <<CNN[r] <<",";
  std::cout <<"}"<<std::endl;
  std::cout <<"W[4527]={";
  for (int r =0; r < W.size(); r++) std::cout <<W[r] <<",";
  std::cout <<"}"<<std::endl;*/
   
  ////////////////////////////
  //  PDF sys
  ////////////////////////////
  std::cout << "PDFsys "<< PDFsys <<std::endl;
  if (PDFsys==true)
  {
    cout<<"#########################"<<endl;
    cout<<" PDFsys  "<<endl;
    cout<<"#########################"<<endl;
    
    float wa[22] = {0.};
    float wb[22] = {0.};
    
    float waMax = 0.;
    float wbMax = 0.;
    
    int kaMax=0;
    int kbMax=0;
    
    std::cout <<"--------------------- " << std::endl; 
    std::cout <<"N0 " << Npdf[0]<< std::endl;
    
    std::cout <<"--------------------- " << std::endl;    
    for (int k = 0; k<22; k++) 
    { //std::cout <<"Npdf " << Npdf[2*k+1]<< std::endl; 
      wa[k] = Npdf[2*k+1]/Npdf[0]-1.;
      std::cout <<wa[k]<< std::endl;
      if (fabs(wa[k])>waMax) {waMax = fabs(wa[k]);kaMax=2*k+1;}      
      }
    
    std::cout <<"--------------------- " << std::endl;    
    for (int k = 0; k<22; k++) 
    { //std::cout <<"Npdf " << Npdf[2*k+2]<< std::endl; 
      wb[k] = Npdf[2*k+2]/Npdf[0]-1.;
      std::cout <<wb[k]<< std::endl;
      if (fabs(wb[k])>wbMax) {wbMax = fabs(wb[k]);kbMax=2*k+2;} 
      }
    std::cout <<"--------------------- " << std::endl;   
    std::cout << "k Max : "<<kaMax << " " << kbMax << std::endl;
    
    float wPlus = 0;
    float wMinus = 0.;
    
    for (int k = 0; k<22; k++) {wMinus += wa[k]*wa[k]; wPlus += wb[k]*wb[k];}
   
    wMinus = sqrt(wMinus);
    wPlus = sqrt(wPlus);
    
    std::cout << wMinus <<std::endl;
    std::cout << wPlus <<std::endl;
    std::cout << (Npdf[0]-Npdf[45])/Npdf[0]<<std::endl;
    }
  

  if(verbosity>0) {
  	cout<<"#########################"<<endl;
  	cout<<" Fill the latex tables   "<<endl;
	cout<<"#########################"<<endl;
  } 

  ofstream ofile("SemiLeptonicTauCrossSectionTable.tex");
  ofile.precision(5);
  ofile<<"\\documentclass[8pt]{article}"<<endl;
  ofile<<"\\begin{document}"<<endl;
  //Calculations
  if(DataType==1) selDATATable.TableCalculator();
  else selTable.TableCalculator();
  //Write
  if(DataType==1) selDATATable.Write(ofile,true,3);
  else selTable.Write(ofile,true,3);
  
  ofile<<"\\end{document}"<<endl;
  system("pdflatex SemiLeptonicTauCrossSectionTable.tex");


  if(verbosity>0) {
        cout<<"#########################"<<endl;
  	cout<<" Write output root file "<<endl;
	cout<<"#########################"<<endl;
  }
  
  TString name1 = "SemiLeptonicTauXsectionMeas_PDFTest";
  //TString name2 = Form("%d", subSampleIdx);
  TString name3 = ".root";
  TString name_root = name1+name3;
  
  TFile* fout  = new TFile(name_root.Data(),"RECREATE");
  histoManager.Write(fout);
  //fout->Write();
  fout->Close();
  
  //Clear histos before deleting the TFile
  histoManager.Clear();  

  delete fout;

  if(verbosity>0) {
  	cout<<"#########################"<<endl;
  	cout<<"    End of the program   "<<endl;
	cout<<"#########################"<<endl;
  }

  return (0);
}
