{
#ifdef __CINT__
#include "RooGlobalFunc.h"
#endif
#include "TFile.h"
#include "TH1.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TStyle.h"
#include <TROOT.h>
#include <TMath.h>
#include <TText.h>
#include <TPaveLabel.h>
#include "TROOT.h"
#include "TSystem.h"
#include "TFrame.h"
#include "iostream"
#include "TTree.h"
#include "TLegend.h"
#include "TPaveText.h"
#include "TGraphErrors.h"
#include <vector>
#include <string>
#include <fstream>

using namespace RooFit;

  gROOT->SetStyle("Plain");
  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  //gStyle->SetOptFit(0111);
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
  gStyle->SetTitleXOffset(1.2);
  gStyle->SetTitleYOffset(0.01);
  gStyle->SetLabelOffset(0.005, "XYZ");
  //  gStyle->SetLabelOffset(0.1, "XYZ");
  gStyle->SetTitleSize(0.07, "XYZ");
  gStyle->SetTitleFont(22,"X");
  gStyle->SetTitleFont(22,"Y");
  gStyle->SetPadBottomMargin(0.13);
  gStyle->SetPadLeftMargin(0.15);
  gStyle->SetPadRightMargin(0.15);
  gStyle->SetHistLineWidth(2);


  //const int nbins = 20;
  //Double_t bins[nbins] = {2.0, 4.0, 6.0, 8.0, 10., 12., 14., 16., 18., 20., 25., 30., 40., 50., 60., 70., 80, 90, 100, 150.}; 
  double thres = 8 ; // GeV
   
  int pause ;
  gROOT->ForceStyle();
  gSystem->Load("libRooFit.so");
  
  double fit_cuts_min = 20-1.5;//thres-1.5 ;
  double fit_cuts_max = 200 ;
  
  TString name_image="efficiencyTnP" ;  
 
  int colors[5] = {kRed,kBlack,kBlue,kRed,kBlack};
  int styles[5] = {kFullCircle, kFullTriangleUp, kFullTriangleDown, kFullSquare, kOpenStar};
   
  TFile *_file0 ;
  //_file0 = TFile::Open("TriggerEffHisto_Data_OK3.root");
  //_file0 = TFile::Open("TriggerEffHisto_Singlemu_v4_pfjets.root");
  //_file0 = TFile::Open("TriggerEffHisto_Data_singleMu_735pb-1_jetleg_pf.root");
  _file0 = TFile::Open("TriggerEffHisto_JetLeg45_15binSingleMuon_Full.root");
  _file0->cd();
  
  
  //TTree* mytree = (TTree*) gDirectory->Get("jet4_eff") ;
  TH1F*  jet4_eff = (TH1F*)_file0->Get("jet4_eff");
  
#ifdef __CINT__
  gROOT->ProcessLineSync(".x FuncCB.cxx+") ;
#endif
  
  
  // Variables from tree
  RooRealVar xaxis("pt","P_{T} [GeV]",20,200) ;
  //RooRealVar xaxis("et","E_{T} [GeV]",0,150) ;
  RooCategory cut("match","trigger efficieny") ; // discrete variables
  cut.defineType("accept",1) ;
  cut.defineType("reject",0) ;
  RooCategory cutL1("L1match","L1 trigger efficieny") ; // discrete variables
  cutL1.defineType("accept",1) ;
  cutL1.defineType("reject",0) ;
  cutL1.defineType("notset",-1) ;
  RooRealVar mass("mass","mass",20,200) ;// consider only this mass range when importing data
  //RooRealVar mass("mass","mass",0,280) ;// consider only this mass range when importing data
  RooRealVar eta("eta","eta",-3., 3) ;
  RooDataHist * dataSet ;
  dataSet = new RooDataHist("data","data",xaxis, jet4_eff);
  
  // PARAMETRES ROOFIT CRYSTAL BALL
 /*
  RooRealVar norm("norm","N",0.95,0.92,0.98);
  RooRealVar alpha("alpha","#alpha",0.25,0.00001,8);
  RooRealVar n("n","n",2,1.1,35);
  RooRealVar mean("mean","mean",40.5,10,70);
  RooRealVar sigma("sigma","#sigma",0.23,0.01,10);
  //mean.setVal(42);*/
  
  
  RooRealVar norm("norm","N",0.95,0.9,0.98);
  RooRealVar alpha("alpha","#alpha",0.25,0.00001,8);
  RooRealVar n("n","n",6.5,1.1,35);
  RooRealVar mean("mean","mean",35,20,50);
  RooRealVar sigma("sigma","#sigma",0.45,0.01,10);
  
  //FONCTION CRYSTALL BALL
  FuncCB cb("cb","Crystal Ball Integree",xaxis,mean,sigma,alpha,n,norm) ;
     
  //////////////////////////////////////:: Construct efficiency p.d.f eff(cut|x)/////////////////////////
  RooEfficiency eff("eff","efficiency",cb, cut,"accept");
     
  //RooBinning binning;
  //binning = RooBinning(nbins - 1, bins, "binning");
    
  //RooPlot* frame = xaxis.frame(Bins(18000),Title("Fitted efficiency")) ;
  RooPlot* frame =  xaxis.frame();
  int color=colors[0];
  int style=styles[0];
  
  //dataSet->plotOn(frame,Binning(binning),Efficiency(cut),MarkerColor(1),LineColor(1), MarkerStyle(style) );
  dataSet->plotOn(frame);
  
  ///////////////////////////////////////////// FITTING /////////////////////////////
  xaxis.setRange("interesting",fit_cuts_min,fit_cuts_max);
  cout<<endl<<endl<<endl<<endl<<endl ;
  cout<<"======================================================================="<<endl ;
  //RooFitResult * fit = eff.fitTo(*dataSet); //asymmetric errors
  RooFitResult * fit = eff.fitTo(*dataSet,ConditionalObservables(xaxis),Range("interesting"),Minos(kFALSE), Save(kTRUE)); //symmetric errors
  
  //draw
  cb.plotOn(frame,LineColor(1),LineWidth(2));
  
 
  ////////////////////////////  DRAWING PLOTS AND LEGENDS /////////////////////////////////
  // _____________________________________________________________________________________

  TCanvas* ca = new TCanvas("ca","Trigger Efficiency") ;

  ca->SetGridx();
  ca->SetGridy();
  ca->cd();
  
  //gPad->SetLogx();
  //gPad->SetObjectStat(1);

  //frame->GetYaxis()->SetRangeUser(0,1.05);
  //frame->GetXaxis()->SetRangeUser(0.5,150.);
  //frame->GetYaxis()->SetTitle("Efficiency");
  frame->Draw() ;
   
 /* TPaveText *paveText = new TPaveText(0.162752,0.573357,0.380872,0.673007,"brNDC");
  paveText->SetLineColor(1);
  paveText->SetTextColor(1);
  paveText->SetTextFont(42);
  paveText->SetTextSize(0.0227273);
  paveText->SetFillColor(kWhite);
  paveText->SetShadowColor(kWhite);
  //paveText->AddText("HLT_El15 Trigger");
  //paveText->AddText("Electrons from Z");
  paveText->Draw();*/


  ca->Print(name_image+".pdf");
  ca->Print(name_image+".root");

  return fit ;

}

