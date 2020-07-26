void QCD_PLOT()
{
  gROOT->Reset();
 //  gROOT->SetStyle("Plain");
//   gStyle->SetOPtStat(kFALSE);

  gROOT->SetStyle("Plain");
  gStyle->SetPalette(6);
  gStyle->SetOptStat(0);



  TFile *_file_directTag ;
  _file_directTag = TFile::Open("SemiLeptonicTauXsectionMeas_QCD_directTagging.root");
  _file_directTag->cd();

  TH1F*  hist_0 = (TH1F*)_file_directTag->Get("SemiLeptonicTauAnaPlots/Jets/all/met/QCD1/Pt");
  hist_0->Sumw2();
  TH1F*  hist_1 = (TH1F*)_file_directTag->Get("SemiLeptonicTauAnaPlots/Jets/all/met/QCD2/Pt");
  hist_1->Sumw2();
  TH1F*  hist_2 = (TH1F*)_file_directTag->Get("SemiLeptonicTauAnaPlots/Jets/all/met/QCD3/Pt");
  hist_2->Sumw2();
  TH1F*  hist_3 = (TH1F*)_file_directTag->Get("SemiLeptonicTauAnaPlots/Jets/all/met/QCD4/Pt");
  hist_3->Sumw2();
  TH1F*  hist_4 = (TH1F*)_file_directTag->Get("SemiLeptonicTauAnaPlots/Jets/all/met/QCD5/Pt");
  hist_4->Sumw2();
  TH1F*  hist_5 = (TH1F*)_file_directTag->Get("SemiLeptonicTauAnaPlots/Jets/all/met/QCD6/Pt");
  hist_5->Sumw2();
  TH1F*  hist_6 = (TH1F*)_file_directTag->Get("SemiLeptonicTauAnaPlots/Jets/all/met/QCD7/Pt");
  hist_6->Sumw2();
  //TH1F*  hist_7 = (TH1F*)_file_directTag->Get("SemiLeptonicTauAnaPlots/Jets/all/met/QCD8/Pt");
  //hist_7->Sumw2();
 
  hist_0->Add(hist_1);
  hist_0->Add(hist_2);
  hist_0->Add(hist_3);
  hist_0->Add(hist_4);
  hist_0->Add(hist_5);
  hist_0->Add(hist_6);
  //hist_0->Add(hist_7);
 

  hist_0->SetMarkerStyle(20);
  hist_0->SetMarkerColor(kGreen);
  hist_0->SetMarkerSize(1.5);

 
  TFile *_filebtag0 ;
  _filebtag0 = TFile::Open("SemiLeptonicTauXsectionMeas_QCD_method1.root");
  _filebtag0->cd();

  TH1F*  hist_btag0_0 = (TH1F*)_filebtag0->Get("SemiLeptonicTauAnaPlots/Jets/all/1 btag/QCD1/Pt");
  hist_btag0_0->Sumw2();
  TH1F*  hist_btag0_1 = (TH1F*)_filebtag0->Get("SemiLeptonicTauAnaPlots/Jets/all/1 btag/QCD2/Pt");
  hist_btag0_1->Sumw2();
  TH1F*  hist_btag0_2 = (TH1F*)_filebtag0->Get("SemiLeptonicTauAnaPlots/Jets/all/1 btag/QCD3/Pt");
  hist_btag0_2->Sumw2();
  TH1F*  hist_btag0_3 = (TH1F*)_filebtag0->Get("SemiLeptonicTauAnaPlots/Jets/all/1 btag/QCD4/Pt");
  hist_btag0_3->Sumw2();
  TH1F*  hist_btag0_4 = (TH1F*)_filebtag0->Get("SemiLeptonicTauAnaPlots/Jets/all/1 btag/QCD5/Pt");
  hist_btag0_4->Sumw2();
  TH1F*  hist_btag0_5 = (TH1F*)_filebtag0->Get("SemiLeptonicTauAnaPlots/Jets/all/1 btag/QCD6/Pt");
  hist_btag0_5->Sumw2();
  TH1F*  hist_btag0_6 = (TH1F*)_filebtag0->Get("SemiLeptonicTauAnaPlots/Jets/all/1 btag/QCD7/Pt");
  hist_btag0_6->Sumw2();
  //TH1F*  hist_btag0_7 = (TH1F*)_filebtag0->Get("SemiLeptonicTauAnaPlots/Jets/all/1 btag/QCD8/Pt");
  //hist_btag0_7->Sumw2();
 
  hist_btag0_0->Add(hist_btag0_1);
  hist_btag0_0->Add(hist_btag0_2);
  hist_btag0_0->Add(hist_btag0_3);
  hist_btag0_0->Add(hist_btag0_4);
  hist_btag0_0->Add(hist_btag0_5);
  hist_btag0_0->Add(hist_btag0_6);
  //hist_btag0_0->Add(hist_btag0_7);
 

  hist_btag0_0->SetMarkerStyle(20);
  hist_btag0_0->SetMarkerColor(kBlack);
  hist_btag0_0->SetMarkerSize(1.5);



  TFile *_filenoTag ;
  _filenoTag = TFile::Open("SemiLeptonicTauXsectionMeas_QCD_method2.root");
  _filenoTag->cd();

  TH1F*  hist_noTag_0 = (TH1F*)_filenoTag->Get("SemiLeptonicTauAnaPlots/Jets/all/1 btag/QCD1/Pt");
  hist_noTag_0->Sumw2();
  TH1F*  hist_noTag_1 = (TH1F*)_filenoTag->Get("SemiLeptonicTauAnaPlots/Jets/all/1 btag/QCD2/Pt");
  hist_noTag_1->Sumw2();
  TH1F*  hist_noTag_2 = (TH1F*)_filenoTag->Get("SemiLeptonicTauAnaPlots/Jets/all/1 btag/QCD3/Pt");
  hist_noTag_2->Sumw2();
  TH1F*  hist_noTag_3 = (TH1F*)_filenoTag->Get("SemiLeptonicTauAnaPlots/Jets/all/1 btag/QCD4/Pt");
  hist_noTag_3->Sumw2();
  TH1F*  hist_noTag_4 = (TH1F*)_filenoTag->Get("SemiLeptonicTauAnaPlots/Jets/all/1 btag/QCD5/Pt");
  hist_noTag_4->Sumw2();
  TH1F*  hist_noTag_5 = (TH1F*)_filenoTag->Get("SemiLeptonicTauAnaPlots/Jets/all/1 btag/QCD6/Pt");
  hist_noTag_5->Sumw2();
  TH1F*  hist_noTag_6 = (TH1F*)_filenoTag->Get("SemiLeptonicTauAnaPlots/Jets/all/1 btag/QCD7/Pt");
  hist_noTag_6->Sumw2();
  //TH1F*  hist_noTag_7 = (TH1F*)_filenoTag->Get("SemiLeptonicTauAnaPlots/Jets/all/1 btag/QCD8/Pt");
  //hist_noTag_7->Sumw2();
 
  hist_noTag_0->Add(hist_noTag_1);
  hist_noTag_0->Add(hist_noTag_2);
  hist_noTag_0->Add(hist_noTag_3);
  hist_noTag_0->Add(hist_noTag_4);
  hist_noTag_0->Add(hist_noTag_5);
  hist_noTag_0->Add(hist_noTag_6);
  //hist_noTag_0->Add(hist_noTag_7);
 

  hist_noTag_0->SetMarkerStyle(20);
  hist_noTag_0->SetMarkerColor(kRed);
  hist_noTag_0->SetMarkerSize(1.5);


  hist_0->Scale(1/hist_0->Integral());
  hist_noTag_0->Scale(1/hist_noTag_0->Integral());
  hist_btag0_0->Scale(1/hist_btag0_0->Integral());

  hist_0->Draw();
  hist_noTag_0->Draw("same");
  hist_btag0_0->Draw("same");
  

  l = new TLegend(0.4,0.6,0.7,0.7);
  l->SetTextSize(0.026);
  l->SetFillColor(0);
  l->SetBorderSize(0);
  //  l->SetHeader("HLT_QuadJet45_IsoPFTau45: Run2011A ");
  
  l->AddEntry(hist_btag0_0,"btag==0 + b-mistag reweight","LPF");
  l->AddEntry(hist_noTag_0,"btag>=1 (probabilistic approach)","LPF");
  l->AddEntry(hist_0,"btag>=1 (direct tagging)","LPF");
  
   l->Draw("");

}
