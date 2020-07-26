{

// Setup a style
  using namespace RooFit;

  gROOT->SetStyle("Plain");
  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(1111);
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
  gStyle->SetTitleSize(0.07, "XYZ");
  gStyle->SetTitleFont(22,"X");
  gStyle->SetTitleFont(22,"Y");
  gStyle->SetPadBottomMargin(0.13);
  gStyle->SetPadLeftMargin(0.15);
  gStyle->SetPadRightMargin(0.15);
  gStyle->SetHistLineWidth(2);

// Grab PDF's from histos
  
   THStack hs("hs","test stacked histograms");
  
   TFile *_file_WZT ;
  _file_WZT = TFile::Open("~/SemiLeptonicTauXsectionMeas_WZT_3-9fb_438p7_met20_newTauLeg_JP_plotfix2_M3_multibinning_taulegnewbinning.root");
  _file_WZT->cd();
  
  TH1F*  hist0 = (TH1F*)_file_WZT->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarTauHQQ/NNOutput");
  hist0->Sumw2();
  TH1F*  hist1 = (TH1F*)_file_WZT->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarQQQQ/NNOutput");
  hist1->Sumw2();
  TH1F*  hist2 = (TH1F*)_file_WZT->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarLQQ/NNOutput");
  hist2->Sumw2();
  TH1F*  hist3 = (TH1F*)_file_WZT->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/WJetsToLNu/NNOutput");
  hist3->Sumw2();
  TH1F*  hist4 = (TH1F*)_file_WZT->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DYJetsToLL/NNOutput");
  hist4->Sumw2();
  TH1F*  hist6 = (TH1F*)_file_WZT->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarETauH/NNOutput");
  hist6->Sumw2();
  TH1F*  hist7 = (TH1F*)_file_WZT->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarMTauH/NNOutput");
  hist7->Sumw2();
  TH1F*  hist8 = (TH1F*)_file_WZT->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarTauHTauH/NNOutput");
  hist8->Sumw2();
  
   
  hist1->Add(hist2);  
  hist1->Add(hist3);
  hist1->Add(hist4);
  hist1->Add(hist6);
  hist1->Add(hist7);
  hist1->Add(hist8);
  
  TFile *_file_singleTop ;
  _file_singleTop = TFile::Open("~/SemiLeptonicTauXsectionMeas_singletop_3-9fb_438p7_met20_newTauLeg_JP_plotfix2_M3_multibinning_taulegnewbinning.root");
  _file_singleTop->cd();
  TH1F*  hist9 = (TH1F*)_file_singleTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletoptW/NNOutput");
  hist9->Sumw2();
  TH1F*  hist10 = (TH1F*)_file_singleTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletopBartW/NNOutput");
  hist10->Sumw2();
  TH1F*  hist11 = (TH1F*)_file_singleTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/Singletopt/NNOutput");
  hist11->Sumw2();
  TH1F*  hist12 = (TH1F*)_file_singleTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletopBart/NNOutput");
  hist12->Sumw2();
  
  hist1->Add(hist9);
  hist1->Add(hist10);
  hist1->Add(hist11);
  hist1->Add(hist12);
 
  hist1->Sumw2();
  hist0->SetFillColor(2);hist0->SetLineColor(2);
  hist1->SetFillColor(3);hist1->SetLineColor(3);
 
 
  TFile *_fileMULTIJET ;
  _fileMULTIJET = TFile::Open("~/SemiLeptonicTauXsectionMeas_multijet_3-7fb_438p7_met20_newTauLeg_JP_plotfix2.root");
  _fileMULTIJET->cd();
  TH1F*  hist5 = (TH1F*)_fileMULTIJET->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DATA/NNOutput");
  hist5->SetFillColor(7);hist5->SetLineColor(7);
 
  RooRealVar x("x","x",-1.5,1.5); 
  RooDataHist * rooHist0 = new RooDataHist("TTbarTauHQQ","TTbarTauHQQ",x, hist0);
  RooDataHist * rooHist1 = new RooDataHist("BKG","BKG",x, hist1);
  RooDataHist * rooHist5 = new RooDataHist("MultiJet","MultiJet",x, hist5);
 
  RooHistPdf histpdf0("histpdf0","histpdf0",x,*rooHist0) ;
  RooHistPdf histpdf5("histpdf5","histpdf5",x,*rooHist5) ;

  // Construct composite pdf
  
  RooRealVar c0("c0","c0",0.5,0.,1.) ;
  //RooAddPdf pdf("pdf","pdf",RooArgList(histpdf0,histpdf1,histpdf5),RooArgList(c0,c1,c5),false) ;
  RooAddPdf pdf("pdf","pdf",RooArgSet(histpdf0,histpdf5),c0) ;
 
  // Get Data histo
  
  TFile *_fileData ;
  _fileData = TFile::Open("~/SemiLeptonicTauXsectionMeas_data_3-7fb_438p7_met20_newTauLeg_JP_plotfix2.root");
  _fileData->cd();
  TH1F*  histData = (TH1F*)_fileData->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DATA/NNOutput");
  histData->Sumw2(); 
  histData->SetMarkerStyle(20);
  RooDataHist* data = new RooDataHist("data","data",x, histData);
 
  TH1F*  histDataFit = new TH1F("histDataFit","histDataFit",histData->GetNbinsX(),histData->GetXaxis()->GetXmin(),histData->GetXaxis()->GetXmax());
  histDataFit->Sumw2(); 
  histDataFit->Add(histData,hist1,1,-1);
  RooDataHist* dataFit = new RooDataHist("data","data",x, histDataFit);
 
  
  // Fit the stuff
  
  RooFitResult* fitResult = pdf.fitTo(*dataFit, Save()) ;
  std::cout << c0.getVal() << std::endl;
  std::cout << c0.Print()  << std::endl;
  

  
  // Plot pdf's
  
  RooPlot* mframe = x.frame(-1.5,1.5) ; 
  dataFit->plotOn(mframe,Name("data")) ;  
  pdf->plotOn(mframe,LineColor(kBlack),LineStyle(kDashed));
  pdf->plotOn(mframe,Components("histpdf0"),VisualizeError(*fitResult),FillColor(kMagenta)) ; 
  //pdf->plotOn(mframe,Components("histpdf1"),VisualizeError(*fitResult),FillColor(kGreen)) ; 
  pdf->plotOn(mframe,Components("histpdf5"),VisualizeError(*fitResult),FillColor(kBlue)) ; 
  //dataFit->plotOn(mframe) ;  
  pdf->plotOn(mframe,Name("pdf")) ;
  mframe->Draw() ; 
  
  
  // Calculate the various contributions
  
  cout << " the TTbar tau(h) QQ " << c0.getVal()*histDataFit->Integral() << " ± " << c0.getError()*histDataFit->Integral() << endl;
  cout << " the MultiJet        " << (1.-c0.getVal())*histDataFit->Integral() << " ± " << c0.getError()*histDataFit->Integral() << endl;
  cout << " the MC stuff "        << hist1->Integral()    << endl; 
  cout << " the Data "            << histData->Integral()    << endl; 
  cout << " the MC TTbar tau(h) QQ " << hist0->Integral() << endl; 
 
  cout << "scale factor TTbar tau(h) QQ " << c0.getVal()*histDataFit->Integral()/hist0->Integral() << " ± " << c0.getError()*histDataFit->Integral()/hist0->Integral()<< endl; 
  cout << "scale factor MultiJet "        << (1-c0.getVal())*histDataFit->Integral()/hist5->Integral()    << " ± " << c0.getError()*histDataFit->Integral()/hist5->Integral()<< endl; 
  cout <<" " <<endl;
  cout << "chisquare " <<mframe->chiSquare("pdf","data",1)<<endl;
  cout << "pull " <<(c0.getVal()*histDataFit->Integral()-hist0->Integral())/hist0->Integral() << std::endl;
 
  // Plot the stuff
  
  hist5->Scale((1-c0.getVal())*histDataFit->Integral()/hist5->Integral());
  hist0->Scale(c0.getVal()*histDataFit->Integral()/hist0->Integral());
 
  histData->Draw("e");
  hs.Add(hist5);
  hs.Add(hist1);
  hs.Add(hist0);
  hs.Draw("histosame");
  histData->Draw("esame");
  
  fitResult->Print("") ;
  
 std::cout <<"W " << hist3->Integral()<< " " << sqrt(hist3->Integral())<< std::endl;
 std::cout <<"Z " << hist4->Integral()<< " " << sqrt(hist4->Integral())<< std::endl;
 
 std::cout <<"tt " << hist1->Integral()-hist3->Integral()-hist4->Integral()-hist9->Integral()-hist10->Integral()-hist11->Integral()-hist12->Integral()<< " " << std::endl;
 std::cout <<"s t " << hist9->Integral()+hist10->Integral()+hist11->Integral()+hist12->Integral()<< " " << sqrt(pow(hist9->Integral(),2)+pow(hist10->Integral(),2)+pow(hist11->Integral(),2)+pow(hist12->Integral(),2))<< std::endl;
 
}
/*
  // Construct binned likelihood
  RooNLLVar nll("nll","nll",pdf,*dataFit) ;
  // Start Minuit session on NLL
  RooMinuit m(nll) ;
  m.migrad() ;
  m.hesse() ;
  RooFitResult* r1 = m.save() ; 
  
  // Construct Chi2
  RooChi2Var chi2("chi2","chi2",pdf,*dataFit) ;
  // Start Minuit session on Chi2
  RooMinuit m2(chi2) ;
  m2.migrad() ;
  m2.hesse() ;
  RooFitResult* r2 = m2.save() ;

  // Print results 
  cout << "result of binned likelihood fit" << endl ;
  r1->Print("v") ;
  cout << "result of chi2 fit" << endl ;
  r2->Print("v") ;*/
