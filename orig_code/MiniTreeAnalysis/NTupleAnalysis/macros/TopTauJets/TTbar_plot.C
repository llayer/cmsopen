{
   TCanvas *c1 = new TCanvas("c1", "c1",10,32,782,552);
   c1->cd();
 
  gROOT->Reset();
  // gROOT->SetStyle("Plain");
  // gStyle->SetOPtStat(kFALSE);

  gROOT->SetStyle("Plain");
  gStyle->SetPalette(6);
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  gStyle->SetCanvasColor(kWhite);
  


  TFile *_file_directTag ;
  _file_directTag = TFile::Open("SemiLeptonicTauXsectionMeas_WZT_3-7fb_438p7_met20_newTauLeg_JP_plotfix_directTagging.root");
  _file_directTag->cd();

  TH1F*  hist_0 = (TH1F*)_file_directTag->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarTauHQQ/q*Eta");
  hist_0->Sumw2();
  hist_0->Rebin(2);


  hist_0->SetMarkerStyle(20);
  hist_0->SetMarkerColor(kRed);
  hist_0->SetMarkerSize(1.);
  //hist_0->SetMaximum(0.25);
 
  TFile *_filebtag0 ;
  _filebtag0 = TFile::Open("SemiLeptonicTauXsectionMeas_WZT_3-7fb_438p7_met20_newTauLeg_JP_plotfix.root");
  _filebtag0->cd();

  TH1F*  hist_0_probabilistic = (TH1F*)_filebtag0->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarTauHQQ/q*Eta");
  hist_0_probabilistic->Sumw2();
  
  hist_0_probabilistic->SetMarkerStyle(20);
  hist_0_probabilistic->SetMarkerColor(kBlue);
  hist_0_probabilistic->SetMarkerSize(1.);
  hist_0_probabilistic->Rebin(2);

  //hist_0_probabilistic->SetMaximum(0.25);

  hist_0->Scale(1/hist_0->Integral());
  hist_0_probabilistic->Scale(1/hist_0_probabilistic->Integral());
  
  hist_0->Draw();
  hist_0_probabilistic->Draw("same");
  
  l = new TLegend(0.4,0.6,0.7,0.7);
  l->SetTextSize(0.03);
  l->SetFillColor(0);
  l->SetBorderSize(0);
  //  l->SetHeader("HLT_QuadJet45_IsoPFTau45: Run2011A ");
  
  l->AddEntry(hist_0_probabilistic,"btag>=1 (probabilistic approach)","LPF");
  l->AddEntry(hist_0,"btag>=1 (direct tagging)","LPF");
  
  l->Draw("");
  c1->SaveAs("ttbar_q_Eta.png");
  //c1->SaveAs("ttbar_q*Eta.jpeg");
}

