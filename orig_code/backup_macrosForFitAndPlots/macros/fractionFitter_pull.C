#include <iostream>
#include <TROOT.h>
#include <TPaveText.h>

void fractionFitter_pull()
{ 
  ofstream ofile;
  ofile.open ("pull_results_Haryo_fractionFitter.txt",ios::app); 
 
// *****************************************************************************
  
  TFile *f0 = new TFile("~/SemiLeptonicTauXsectionMeas_data_3-7fb_438p7_met20_newTauLeg_JP_plotfix2.root");
  TFile *f1 = new TFile("~/SemiLeptonicTauXsectionMeas_multijet_3-7fb_438p7_met20_newTauLeg_JP_plotfix2.root");
  TFile *f2 = new TFile("~/SemiLeptonicTauXsectionMeas_WZT_3-7fb_438p7_met20_newTauLeg_JP_plotfix2.root");
  TFile *f3 = new TFile("~/SemiLeptonicTauXsectionMeas_singletop_3-7fb_438p7_met20_newTauLeg_JP_plotfix2.root");
  TFile *fttbarPE = new TFile("~/scratch0/SemiLeptonicTauXsectionMeas_PE_ttbar_168.root");//TTbartobeModified
  TFile *fmultijetPE = new TFile("~/SemiLeptonicTauXsectionMeas_PE_multijet_49.root");//MultijettobeModified
 
// *****************************************************************************

 Int_t stati=0;
 Bool_t  fit=0;
 Bool_t logy=1;

// *****************************************************************************

 TCanvas *c1 = new TCanvas("c1", "plots",200,10,700,730);
 c1->SetFillColor(10);
 c1->SetFillStyle(4000);
 c1->SetBorderSize(2);

// *****************************************************************************

pad1 = new TPad("pad1","This is pad1",0.01,0.01,0.99,0.99,21);

pad1->SetFillColor(0);
pad1->SetBorderMode(0);
pad1->SetFrameFillColor(10);
pad1->Draw();
//if (logy) pad1->SetLogy();

//$$ gStyle->SetOptDate(1);
gStyle->SetOptDate(0);
gStyle->SetStatColor(0);
gStyle->SetTitleColor(29);
gStyle->SetTitleW(0.5);
gStyle->SetTitleH(0.05);
gStyle->SetOptStat(stati);

// *****************************************************************************

       //Multijet
       f1->cd();
       TH1F*  g1 = (TH1F*)f1->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DATA/NNOutput");
       g1->Sumw2();
       
       fmultijetPE->cd();
       TH1F*  h0 = (TH1F*)fmultijetPE->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DATA/NNOutput");
       h0->Sumw2();
       
       //TTbar signal
       f2->cd();
       TH1F*  g2 = (TH1F*)f2->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarTauHQQ/NNOutput");
       g2->Sumw2();
     
       fttbarPE->cd();
       TH1F*  h1 = (TH1F*)fttbarPE->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarTauHQQ/NNOutput");
       h1->Sumw2();
       
       //Pseudo-data
       h0->Add(h1,1);
       
      /* //
       f2->cd();
       TH1F*  g3 = (TH1F*)f2->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarQQQQ/NNOutput");
       g3->Sumw2();
     
       TH1F*  hist2 = (TH1F*)f2->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarLQQ/NNOutput");
       hist2->Sumw2();
       TH1F*  hist3 = (TH1F*)f2->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/WJetsToLNu/NNOutput");
       hist3->Sumw2();
       TH1F*  hist4 = (TH1F*)f2->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DYJetsToLL/NNOutput");
       hist4->Sumw2();
       TH1F*  hist6 = (TH1F*)f2->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarETauH/NNOutput");
       hist6->Sumw2();
       TH1F*  hist7 = (TH1F*)f2->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarMTauH/NNOutput");
       hist7->Sumw2();
       TH1F*  hist8 = (TH1F*)f2->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarTauHTauH/NNOutput");
       hist8->Sumw2();
    
       f3->cd();
       TH1F*  hist9 = (TH1F*)f3->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletoptW/NNOutput");
       hist9->Sumw2();
       TH1F*  hist10 = (TH1F*)f3->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletopBartW/NNOutput");
       hist10->Sumw2();
       TH1F*  hist11 = (TH1F*)f3->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/Singletopt/NNOutput");
       hist11->Sumw2();
       TH1F*  hist12 = (TH1F*)f3->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletopBart/NNOutput");
       hist12->Sumw2();
       
       g3->Add(hist2);
       g3->Add(hist3);
       g3->Add(hist4);
       g3->Add(hist6);
       g3->Add(hist7);
       g3->Add(hist8);
       g3->Add(hist9);
       g3->Add(hist10);
       g3->Add(hist11);
       g3->Add(hist12);*/
   
       TObjArray *mc = new TObjArray(2); // MC histograms are put in this array
       mc->Add(g1);
       mc->Add(g2);
       h0->Draw();
       
       TFractionFitter* fitter = new TFractionFitter(h0, mc); // initialise
       fitter->Constrain(1,0.0,1.0);     // constrain fraction 1 to be between 0 and 1
       fitter->Constrain(2,0.0,1.0);
       //fitter->Constrain(3,0.0,1.0);
       fitter->SetRangeX(-1.5,1.5);          // use only the first 15 bins in the fit

       Int_t status = fitter->Fit();     // perform the fit
       cout << "###############" << endl;
       cout << " fit status: " << status << endl;
       cout << "###############" << endl;
       
       
       Double_t _c1,_c2,_c3;
       Double_t c1_err,c2_err,c3_err;
       
       //TH1F* result = (TH1F*) fitter->GetPlot();
       fitter->GetResult(0, _c1, c1_err);
       fitter->GetResult(1, _c2, c2_err);
       Chi2 = fitter->GetChisquare();
       Ndf = fitter->GetNDF();
 
 
       std::cout << "Chi2 " << Chi2 << std::endl;
       std::cout << "Ndf " << Ndf << std::endl;
       std::cout << "Chi2/Ndf " << Chi2/Ndf << std::endl;
       
       std::cout << "TTbar signal fraction " << _c2 <<" "<< c2_err << std::endl;
       std::cout << "Multijet signal fraction " << _c1 <<" "<< c1_err << std::endl;
       
       std::cout << "TTbar signal " << _c2*h0->GetSumOfWeights() <<" "<< c2_err*h0->GetSumOfWeights() << std::endl;
       std::cout << "Multijet signal " << _c1*h0->GetSumOfWeights() <<" "<< c1_err*h0->GetSumOfWeights() << std::endl;
       
       //float SF2 = _c2*h0->GetSumOfWeights()/g2->GetSumOfWeights();
       //float SF1 = _c1*h0->GetSumOfWeights()/g1->GetSumOfWeights();
      
       //std::cout << h1->GetSumOfWeights() << std::endl;
       std::cout <<"pull " << (_c2*h0->GetSumOfWeights() - h1->GetSumOfWeights())/(c2_err*h0->GetSumOfWeights()) <<std::endl;
       ofile<<"h1->Fill("<< (_c2*h0->GetSumOfWeights() - h1->GetSumOfWeights())/(c2_err*h0->GetSumOfWeights()) <<");" <<endl;
  
     
}

