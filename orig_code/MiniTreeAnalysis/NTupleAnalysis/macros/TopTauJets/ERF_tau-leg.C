#include <TDirectory.h>
#include <TH1.h>
#include <TH2.h>
#include <TProfile.h>
#include <TFile.h>
#include <TMath.h>
#include <iostream>
//#include <ostringstream>
#include <TCanvas.h> 
#include <TLegend.h>
#include <stdlib.h>
#include <stdio.h>
#include <THStack.h>
#include "TROOT.h"
#include "TStyle.h"
#include <TGraph.h>
#include <TF1.h>


void ERF_tau_leg()
{

  gROOT->Reset();
  gROOT->Clear(); 
  gStyle->SetPadGridX(kTRUE);
  gStyle->SetPadGridY(kTRUE);
  //gStyle->SetNdivisions(5);
  gStyle->SetCanvasBorderMode(0); 
  gStyle->SetPadBorderMode(0);
  gStyle->SetOptTitle(1);
  gStyle->SetStatFont(42);
  gStyle->SetCanvasColor(10);
  gStyle->SetPadColor(30);
  gStyle->SetTitleFont(62,"xy");
  gStyle->SetLabelFont(62,"xy");
  gStyle->SetTitleFontSize(0.05);
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(1);
  gStyle->SetFitFormat("5.4g");
  gStyle->SetFuncColor(1);
  gStyle->SetFuncStyle(1);
  gStyle->SetFuncWidth(1);

  bool fit = true;
  
  TFile* f0;
  f0 = new TFile("TriggerEffHisto_match45_newTauID.root");
 
 
  TCanvas *d = new TCanvas("d", "d",700,700,700,700);
  
  TH1F *ali = (TH1F*)f0->Get("eff_tau");
  

  TF1 * erf = new TF1("erf","[0] * (1+TMath::Erf((x-[1])/(sqrt(2)*[2])))", 40,90);
  erf->SetParameters(1,15,1);
  
  //if (fit) ali->Fit(erf,"IM","",50,90);
 
  /*ali->SetMarkerStyle(20);
  ali->SetMarkerColor(kBlue); 
  ali->SetMarkerSize(0.8);
  ali->SetLineColor(kBlue);
  ali->SetTitle("");
  ali->GetXaxis()->SetTitle("pf-tau p_{T} [GeV/c]");
  ali->GetYaxis()->SetTitle("Tau-leg efficiency");
  ali->SetMaximum(1.);
  ali->Draw("");*/
  
 
  TH1F *ali2 = (TH1F*)f0->Get("eff_tau_2");

  if (fit) ali2->Fit(erf,"IM","",43,90);
  
  ali2->SetMarkerStyle(20);
  ali2->SetMarkerColor(kRed);
  ali2->SetMarkerSize(0.8);
  ali2->SetLineColor(kRed);
  ali2->SetTitle("");
  ali2->GetXaxis()->SetTitle("pf-tau p_{T} [GeV/c]");
  //ali->GetYaxis()->SetTitle("Jet-leg efficiency");
  ali2->GetYaxis()->SetTitle("Tau-leg efficiency");
  ali2->SetMaximum(1.);
  ali2->Draw("");
  
  text1 = new TLatex(0.15,0.94,"CMS Preliminary, #sqrt{s}=7 TeV - HLT_QuadJet45_IsoPFTau45");
  text1->SetNDC();
  //text1->SetTextAlign(13);
  //text1->SetX(0.18);
  //text1->SetY(0.92);
  //text1->SetLineWidth(2);
  text1->SetTextFont(42);
  text1->SetTextSize(0.03);
  //text1->SetTextSizePixels(24);// dflt=28
  text1->Draw();
 

 /* TLegend *l = new TLegend(0.55,0.5,0.99,0.7);
  l->SetTextSize(0.022);
  l->SetTextFont(42);
 
  l->SetFillColor(0);
  l->SetBorderSize(0.);
  //l->AddEntry((TObject*)0, "CMS preliminary 2011 - #sqrt{s}=7 TeV", "");
  //l->AddEntry((TObject*)0, " ", "");
  l->AddEntry((TObject*)0, "HLT_QuadJet40_IsoPFTau40 ", "");
  l->AddEntry((TObject*)0, " ", "");
  //l->AddEntry(ali, "pf-tau matched to pf-jet", "p");
  //l->AddEntry(ali2, "pf-tau not matched to pf-jet", "p");
  l->Draw();*/
 

  d->Print("tau_leg_quadjet45_isopftau45_fit.pdf");
  d->Print("tau_leg_quadjet45_isopftau45_fit.root");
  

  ////////////////////////////////////////////////

 
}
