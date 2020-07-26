// Setup a style
  using namespace RooFit;
  #include "TStyle.h"

// tdrGrid: Turns the grid lines on (true) or off (false)
void tdrGrid(bool gridOn) {
  tdrStyle->SetPadGridX(gridOn);
  tdrStyle->SetPadGridY(gridOn);
}

// fixOverlay: Redraws the axis
void fixOverlay() {
  gPad->RedrawAxis();
}

void setTDRStyle() {
  TStyle *tdrStyle = new TStyle("tdrStyle","Style for P-TDR");

// For the canvas:
  tdrStyle->SetCanvasBorderMode(0);
  tdrStyle->SetCanvasColor(kWhite);
  tdrStyle->SetCanvasDefH(600); //Height of canvas
  tdrStyle->SetCanvasDefW(600); //Width of canvas
  tdrStyle->SetCanvasDefX(0);   //POsition on screen
  tdrStyle->SetCanvasDefY(0);

// For the Pad:
  tdrStyle->SetPadBorderMode(0);
  // tdrStyle->SetPadBorderSize(Width_t size = 1);
  tdrStyle->SetPadColor(kWhite);
  tdrStyle->SetPadGridX(false);
  tdrStyle->SetPadGridY(false);
  tdrStyle->SetGridColor(0);
  tdrStyle->SetGridStyle(3);
  tdrStyle->SetGridWidth(1);

// For the frame:
  tdrStyle->SetFrameBorderMode(0);
  tdrStyle->SetFrameBorderSize(1);
  tdrStyle->SetFrameFillColor(0);
  tdrStyle->SetFrameFillStyle(0);
  tdrStyle->SetFrameLineColor(1);
  tdrStyle->SetFrameLineStyle(1);
  tdrStyle->SetFrameLineWidth(1);

// For the histo:
  // tdrStyle->SetHistFillColor(1);
  // tdrStyle->SetHistFillStyle(0);
  tdrStyle->SetHistLineColor(1);
  tdrStyle->SetHistLineStyle(0);
  tdrStyle->SetHistLineWidth(1);
  // tdrStyle->SetLegoInnerR(Float_t rad = 0.5);
  // tdrStyle->SetNumberContours(Int_t number = 20);

/*  tdrStyle->SetEndErrorSize(2);
//  tdrStyle->SetErrorMarker(20);
  tdrStyle->SetErrorX(0.);
  
  tdrStyle->SetMarkerStyle(20);*/

//For the fit/function:
  tdrStyle->SetOptFit(1);
  tdrStyle->SetFitFormat("5.4g");
  tdrStyle->SetFuncColor(2);
  tdrStyle->SetFuncStyle(1);
  tdrStyle->SetFuncWidth(1);

//For the date:
  tdrStyle->SetOptDate(0);
  // tdrStyle->SetDateX(Float_t x = 0.01);
  // tdrStyle->SetDateY(Float_t y = 0.01);

// For the statistics box:
  tdrStyle->SetOptFile(0);
  tdrStyle->SetOptStat(0); // To display the mean and RMS:   SetOptStat("mr");
  tdrStyle->SetStatColor(kWhite);
  tdrStyle->SetStatFont(42);
  tdrStyle->SetStatFontSize(0.025);
  tdrStyle->SetStatTextColor(1);
  tdrStyle->SetStatFormat("6.4g");
  tdrStyle->SetStatBorderSize(1);
  tdrStyle->SetStatH(0.1);
  tdrStyle->SetStatW(0.15);
  // tdrStyle->SetStatStyle(Style_t style = 1001);
  // tdrStyle->SetStatX(Float_t x = 0);
  // tdrStyle->SetStatY(Float_t y = 0);

// Margins:
  tdrStyle->SetPadTopMargin(0.05);
  tdrStyle->SetPadBottomMargin(0.13);
  tdrStyle->SetPadLeftMargin(0.16);
  tdrStyle->SetPadRightMargin(0.02);

// For the Global title:

  tdrStyle->SetOptTitle(0);
  tdrStyle->SetTitleFont(42);
  tdrStyle->SetTitleColor(1);
  tdrStyle->SetTitleTextColor(1);
  tdrStyle->SetTitleFillColor(10);
  tdrStyle->SetTitleFontSize(0.05);
  // tdrStyle->SetTitleH(0); // Set the height of the title box
  // tdrStyle->SetTitleW(0); // Set the width of the title box
  // tdrStyle->SetTitleX(0); // Set the position of the title box
  // tdrStyle->SetTitleY(0.985); // Set the position of the title box
  // tdrStyle->SetTitleStyle(Style_t style = 1001);
  // tdrStyle->SetTitleBorderSize(2);

// For the axis titles:

  tdrStyle->SetTitleColor(1, "XYZ");
  tdrStyle->SetTitleFont(42, "XYZ");
  tdrStyle->SetTitleSize(0.06, "XYZ");
  // tdrStyle->SetTitleXSize(Float_t size = 0.02); // Another way to set the size?
  // tdrStyle->SetTitleYSize(Float_t size = 0.02);
  tdrStyle->SetTitleXOffset(0.9);
  tdrStyle->SetTitleYOffset(1.25);
  // tdrStyle->SetTitleOffset(1.1, "Y"); // Another way to set the Offset

// For the axis labels:

  tdrStyle->SetLabelColor(1, "XYZ");
  tdrStyle->SetLabelFont(42, "XYZ");
  tdrStyle->SetLabelOffset(0.007, "XYZ");
  tdrStyle->SetLabelSize(0.05, "XYZ");

// For the axis:

  tdrStyle->SetAxisColor(1, "XYZ");
  tdrStyle->SetStripDecimals(kTRUE);
  tdrStyle->SetTickLength(0.03, "XYZ");
  tdrStyle->SetNdivisions(510, "XYZ");
  tdrStyle->SetPadTickX(1);  // To get tick marks on the opposite side of the frame
  tdrStyle->SetPadTickY(1);

// Change for log plots:
 tdrStyle->SetOptLogx(0);
  tdrStyle->SetOptLogy(0);
 tdrStyle->SetOptLogz(0);

// Postscript options:
  tdrStyle->SetPaperSize(20.,20.);
  // tdrStyle->SetLineScalePS(Float_t scale = 3);
  // tdrStyle->SetLineStyleString(Int_t i, const char* text);
  // tdrStyle->SetHeaderPS(const char* header);
  // tdrStyle->SetTitlePS(const char* pstitle);

  // tdrStyle->SetBarOffset(Float_t baroff = 0.5);
  // tdrStyle->SetBarWidth(Float_t barwidth = 0.5);
  // tdrStyle->SetPaintTextFormat(const char* format = "g");
  // tdrStyle->SetPalette(Int_t ncolors = 0, Int_t* colors = 0);
  // tdrStyle->SetTimeOffset(Double_t toffset);
  // tdrStyle->SetHistMinimumZero(kTRUE);

  tdrStyle->cd();

}

void StackPlot_2_2011B_FR() {
   
   setTDRStyle();

// Which histogram ?  

  int i = 22;//1,2,6,12,13,14,16,17//15
             //9 8 18 19 20
  
  TH1F*  hist0;
  TH1F*  hist1;
  TH1F*  hist2;
  TH1F*  hist3;
  TH1F*  hist4;
  TH1F*  hist5;
  TH1F*  hist6;
  TH1F*  hist7;
  TH1F*  hist8;
  TH1F*  hist9;
  TH1F*  hist10;
  TH1F*  hist11;
  TH1F*  histDATA;
  TH1F*  histERROR;
  TH1F*  histTOT;
  
  TH1F *hist0 = (TH1F*)gDirectory->Get("hist0");
  TH1F *hist1 = (TH1F*)gDirectory->Get("hist1");
  TH1F *hist2 = (TH1F*)gDirectory->Get("hist2");
  TH1F *hist3 = (TH1F*)gDirectory->Get("hist3");
  TH1F *hist4 = (TH1F*)gDirectory->Get("hist4");
  TH1F *hist6 = (TH1F*)gDirectory->Get("hist6");
  TH1F *hist7 = (TH1F*)gDirectory->Get("hist7");
  TH1F *hist8 = (TH1F*)gDirectory->Get("hist8");
  TH1F *hist9 = (TH1F*)gDirectory->Get("hist9");
  TH1F *hist10 = (TH1F*)gDirectory->Get("hist10");
  TH1F *histERROR = (TH1F*)gDirectory->Get("histERROR");
  TH1F *histTOT = (TH1F*)gDirectory->Get("histTOT");
  //histERROR->Sumw2();
  
  
// Grab PDF's from histos
  
  THStack hs("hs","test stacked histograms");
   
  TFile *_file_TTbar ;
  // _file_TTbar = TFile::Open("~/SemiLeptonicTauXsectionMeas_WZT_3-5fb_medtau_met15_medbtag_NPV");
  _file_TTbar = TFile::Open("~/Final_rootfiles/SemiLeptonicTauXsectionMeas_WZT_Fermi.root");

  _file_TTbar->cd();
  
  if(i==22)
  { 
   hist0 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarTauHQQ/M3_NN5_30_50_350");
   hist1 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarQQQQ/M3_NN5_30_50_350");
   hist2 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarLQQ/M3_NN5_30_50_350");
   hist3 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/WJetsToLNu/M3_NN5_30_50_350");
   hist4 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DYJetsToLL/M3_NN5_30_50_350");
   hist6 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarETauH/M3_NN5_30_50_350");
   hist7 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarMTauH/M3_NN5_30_50_350");
   hist8 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarTauHTauH/M3_NN5_30_50_350");
   hist0->Rebin(1);
   hist1->Rebin(1);
   hist2->Rebin(1);
   hist3->Rebin(1);
   hist4->Rebin(1);   
   hist6->Rebin(1);   
   hist7->Rebin(1);   
   hist8->Rebin(1);  
   }
  if(i==0)
  { 
   hist0 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarTauHQQ/NNOutput");
   hist1 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarQQQQ/NNOutput");
   hist2 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarLQQ/NNOutput");
   hist3 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/WJetsToLNu/NNOutput");
   hist4 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DYJetsToLL/NNOutput");
   hist6 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarETauH/NNOutput");
   hist7 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarMTauH/NNOutput");
   hist8 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarTauHTauH/NNOutput"); 
   }
  if(i==1)
  { 
   hist0 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarTauHQQ/Ht");
   hist1 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarQQQQ/Ht");
   hist2 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarLQQ/Ht");
   hist3 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/WJetsToLNu/Ht");
   hist4 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DYJetsToLL/Ht");
   hist6 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarETauH/Ht");
   hist7 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarMTauH/Ht");
   hist8 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarTauHTauH/Ht"); 
   hist0->Rebin(1);
   hist1->Rebin(1);
   hist2->Rebin(1);
   hist3->Rebin(1);
   hist4->Rebin(1);   
   hist6->Rebin(1);   
   hist7->Rebin(1);   
   hist8->Rebin(1);   
   }
  if(i==2)
  { 
   hist0 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarTauHQQ/Aplanarity");
   hist1 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarQQQQ/Aplanarity");
   hist2 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarLQQ/Aplanarity");
   hist3 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/WJetsToLNu/Aplanarity");
   hist4 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DYJetsToLL/Aplanarity");
   hist6 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarETauH/Aplanarity");
   hist7 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarMTauH/Aplanarity");
   hist8 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarTauHTauH/Aplanarity"); 
   }
  if(i==3)
  { 
   hist0 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarTauHQQ/Sphericity");
   hist1 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarQQQQ/Sphericity");
   hist2 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarLQQ/Sphericity");
   hist3 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/WJetsToLNu/Sphericity");
   hist4 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DYJetsToLL/Sphericity");
   hist6 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarETauH/Sphericity");
   hist7 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarMTauH/Sphericity");
   hist8 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarTauHTauH/Sphericity"); 
   }
  if(i==4)
  { 
   hist0 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarTauHQQ/C");
   hist1 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarQQQQ/C");
   hist2 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarLQQ/C");
   hist3 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/WJetsToLNu/C");
   hist4 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DYJetsToLL/C");
   hist6 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarETauH/C");
   hist7 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarMTauH/C");
   hist8 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarTauHTauH/C"); 
   }
  if(i==5)
  { 
   hist0 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarTauHQQ/D");
   hist1 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarQQQQ/D");
   hist2 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarLQQ/D");
   hist3 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/WJetsToLNu/D");
   hist4 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DYJetsToLL/D");
   hist6 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarETauH/D");
   hist7 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarMTauH/D");
   hist8 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarTauHTauH/D"); 
   }
  if(i==6)
  { 
   hist0 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/MET/all/met/TTbarTauHQQ/Et");
   hist1 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/MET/all/met/TTbarQQQQ/Et");
   hist2 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/MET/all/met/TTbarLQQ/Et");
   hist3 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/MET/all/met/TTbarQQQQ/Et");
   hist4 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/MET/all/met/TTbarLQQ/Et");
   hist6 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/MET/all/met/TTbarETauH/Et");
   hist7 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/MET/all/met/TTbarMTauH/Et");
   hist8 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/MET/all/met/TTbarTauHTauH/Et"); 
   }
  if(i==7)
  { 
   hist0 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarTauHQQ/decayMode");
   hist1 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarQQQQ/decayMode");
   hist2 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarLQQ/decayMode");
   hist3 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/WJetsToLNu/decayMode");
   hist4 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/DYJetsToLL/decayMode");
   hist6 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarETauH/decayMode");
   hist7 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarMTauH/decayMode");
   hist8 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarTauHTauH/decayMode"); 
   }
  if(i==8)
  { 
   hist0 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarTauHQQ/Pt");
   hist1 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarQQQQ/Pt");
   hist2 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarLQQ/Pt");
   hist3 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/WJetsToLNu/Pt");
   hist4 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/DYJetsToLL/Pt");
   hist6 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarETauH/Pt");
   hist7 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarMTauH/Pt");
   hist8 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarTauHTauH/Pt"); 
   hist0->Rebin(1);
   hist1->Rebin(1);
   hist2->Rebin(1);
   hist3->Rebin(1);
   hist4->Rebin(1); hist6->Rebin(1); hist7->Rebin(1); hist8->Rebin(1); 
  }
  if(i==9)
  { 
   hist0 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarTauHQQ/q*Eta");
   hist1 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarQQQQ/q*Eta");
   hist2 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarLQQ/q*Eta");
   hist3 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/WJetsToLNu/q*Eta");
   hist4 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/DYJetsToLL/q*Eta");
   hist6 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarETauH/q*Eta");
   hist7 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarMTauH/q*Eta");
   hist8 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarTauHTauH/q*Eta"); 
   hist0->Rebin(1);
   hist1->Rebin(1);
   hist2->Rebin(1);
   hist3->Rebin(1);
   hist4->Rebin(1); 
  }
  if(i==10)
  { 
   hist0 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarTauHQQ/leadTrackPt");
   hist1 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarQQQQ/leadTrackPt");
   hist2 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarLQQ/leadTrackPt");
   hist3 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/WJetsToLNu/leadTrackPt");
   hist4 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/DYJetsToLL/leadTrackPt");
   hist6 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarETauH/leadTrackPt");
   hist7 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarMTauH/leadTrackPt");
   hist8 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarTauHTauH/leadTrackPt"); 
  }
  if(i==11)
  { 
   hist0 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarTauHQQ/emFraction");
   hist1 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarQQQQ/emFraction");
   hist2 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarLQQ/emFraction");
   hist3 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/WJetsToLNu/emFraction");
   hist4 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/DYJetsToLL/emFraction");
   hist6 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarETauH/emFraction");
   hist7 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarMTauH/emFraction");
   hist8 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarTauHTauH/emFraction"); 
  }
  if(i==12)
  { 
   hist0 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Jets/all/met/TTbarTauHQQ/Multiplicity");
   hist1 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Jets/all/met/TTbarQQQQ/Multiplicity");
   hist2 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Jets/all/met/TTbarLQQ/Multiplicity");
   hist3 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Jets/all/met/WJetsToLNu/Multiplicity");
   hist4 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Jets/all/met/DYJetsToLL/Multiplicity");
   hist6 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Jets/all/met/TTbarETauH/Multiplicity");
   hist7 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Jets/all/met/TTbarMTauH/Multiplicity");
   hist8 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Jets/all/met/TTbarTauHTauH/Multiplicity"); 
  }
 if(i==13)
  { 
   hist0 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarTauHQQ/MT");
   hist1 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarQQQQ/MT");
   hist2 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarLQQ/MT");
   hist3 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/WJetsToLNu/MT");
   hist4 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/DYJetsToLL/MT");
   hist6 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarETauH/MT");
   hist7 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarMTauH/MT");
   hist8 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarTauHTauH/MT"); 
  }
 if(i==14)
  { 
   hist0 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarTauHQQ/DeltaPhiTauMet");
   hist1 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarQQQQ/DeltaPhiTauMet");
   hist2 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarLQQ/DeltaPhiTauMet");
   hist3 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/WJetsToLNu/DeltaPhiTauMet");
   hist4 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/DYJetsToLL/DeltaPhiTauMet");
   hist6 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarETauH/DeltaPhiTauMet");
   hist7 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarMTauH/DeltaPhiTauMet");
   hist8 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarTauHTauH/DeltaPhiTauMet"); 
  }
  if(i==15)
  { 
   hist0 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Jets/all/met/TTbarTauHQQ/Pt");
   hist1 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Jets/all/met/TTbarQQQQ/Pt");
   hist2 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Jets/all/met/TTbarLQQ/Pt");
   hist3 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Jets/all/met/WJetsToLNu/Pt");
   hist4 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Jets/all/met/DYJetsToLL/Pt");
   hist6 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Jets/all/met/TTbarETauH/Pt");
   hist7 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Jets/all/met/TTbarMTauH/Pt");
   hist8 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Jets/all/met/TTbarTauHTauH/Pt"); 
  }
  if(i==16)
  { 
   hist0 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarTauHQQ/sqrts");
   hist1 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarQQQQ/sqrts");
   hist2 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarLQQ/sqrts");
   hist3 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/WJetsToLNu/sqrts");
   hist4 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DYJetsToLL/sqrts");
   hist6 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarETauH/sqrts");
   hist7 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarMTauH/sqrts");
   hist8 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarTauHTauH/sqrts"); 
   hist0->Rebin(4);
   hist1->Rebin(4);
   hist2->Rebin(4);
   hist3->Rebin(4);
   hist4->Rebin(4);hist6->Rebin(4); hist7->Rebin(4);hist8->Rebin(4);}
  if(i==17)
  { 
   hist0 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarTauHQQ/kinFitChi2");
   hist1 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarQQQQ/kinFitChi2");
   hist2 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarLQQ/kinFitChi2");
   hist3 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/WJetsToLNu/kinFitChi2");
   hist4 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DYJetsToLL/kinFitChi2");
   hist6 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarETauH/kinFitChi2");
   hist7 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarMTauH/kinFitChi2");
   hist8 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarTauHTauH/kinFitChi2"); 
  }
  if(i==18)
  { 
   hist0 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Jets/all/met/TTbarTauHQQ/Eta");
   hist1 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Jets/all/met/TTbarQQQQ/Eta");
   hist2 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Jets/all/met/TTbarLQQ/Eta");
   hist3 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Jets/all/met/WJetsToLNu/Eta");
   hist4 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Jets/all/met/DYJetsToLL/Eta");
   hist6 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Jets/all/met/TTbarETauH/Eta");
   hist7 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Jets/all/met/TTbarMTauH/Eta");
   hist8 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Jets/all/met/TTbarTauHTauH/Eta"); 
  }
  if(i==19)
  { 
   hist0 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarTauHQQ/Eta");
   hist1 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarQQQQ/Eta");
   hist2 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarLQQ/Eta");
   hist3 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/WJetsToLNu/Eta");
   hist4 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/DYJetsToLL/Eta");
   hist6 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarETauH/Eta");
   hist7 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarMTauH/Eta");
   hist8 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarTauHTauH/Eta");  
  }
  if(i==20)
  { 
   hist0 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarTauHQQ/NPV");
   hist1 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarQQQQ/NPV");
   hist2 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarLQQ/NPV");
   hist3 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/WJetsToLNu/NPV");
   hist4 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/DYJetsToLL/NPV");
   hist6 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarETauH/NPV");
   hist7 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarMTauH/NPV");
   hist8 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarTauHTauH/NPV");  
  }

   TFile *_file_sTop ;
   //  _file_sTop = TFile::Open("~/SemiLeptonicTauXsectionMeas_singletop_3-5fb_medtau_met15_medbtag_NPV.root");
   _file_sTop = TFile::Open("~/Final_rootfiles/SemiLeptonicTauXsectionMeas_singletop_Fermi.root");
   _file_sTop->cd();
  
  if(i==22)
  { 
   hist9 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletoptW/M3_NN5_30_50_350");
   hist10 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletopBartW/M3_NN5_30_50_350");
   hist9->Rebin(1);
   hist10->Rebin(1);
   
   }
  if(i==0)
  { 
   hist9 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletoptW/NNOutput");
   hist10 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletopBartW/NNOutput");
   //hist11 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletopT/NNOutput");
   }
  if(i==1)
  { 
   hist9 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletoptW/Ht");
   hist10 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletopBartW/Ht");
   //hist11 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletopT/Ht");
   hist9->Rebin(1);
   hist10->Rebin(1);
   //hist11->Rebin(1);
   }
  if(i==2)
  { 
   hist9 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletoptW/Aplanarity");
   hist10 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletopBartW/Aplanarity");
  // hist11 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletopT/Aplanarity");
  }
  if(i==3)
  { 
   hist9  = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletoptW/Sphericity");
   hist10 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletopBartW/Sphericity");
   //hist11 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletopT/Sphericity");
  }
  if(i==4)
  { 
   hist9  = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletoptW/C");
   hist10 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletopBartW/C");
  // hist11 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletopT/C");
   }
  if(i==5)
  { 
   hist9  = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletoptW/D");
   hist10 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletopBartW/D");
   //hist11 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletopT/D");
   }
  if(i==6)
  { 
   hist9  = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/MET/all/met/SingletoptW/Et");
   hist10 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/MET/all/met/SingletopBartW/Et");
  // hist11 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/MET/all/met/SingletopT/Et");
  }
  if(i==7)
  { 
   hist9  = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/SingletoptW/decayMode");
   hist10 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/SingletopBartW/decayMode");
  // hist11 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/SingletopT/decayMode");
  }
  if(i==8)
  { 
   hist9  = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/SingletoptW/Pt");
   hist10 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/SingletopBartW/Pt");
  // hist11 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/SingletopT/Pt");
   hist9->Rebin(1);
   hist10->Rebin(1);
  // hist11->Rebin(1);
  }
  if(i==9)
  { 
   hist9  = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/SingletoptW/q*Eta");
   hist10 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/SingletopBartW/q*Eta");
   //hist11 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/SingletopT/q*Eta");
   hist9->Rebin(1);
   hist10->Rebin(1); 
   //hist11->Rebin(1);
  }
  if(i==10)
  { 
   hist9  = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/SingletoptW/leadTrackPt");
   hist10 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/SingletopBartW/leadTrackPt");
   //hist11 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/SingletopT/leadTrackPt");
  }
  if(i==11)
  { 
   hist9  = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/SingletoptW/emFraction");
   hist10 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/SingletopBartW/emFraction");
  // hist11 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/SingletopT/emFraction");
  }
  if(i==12)
  { 
   hist9 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Jets/all/met/SingletoptW/Multiplicity");
   hist10 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Jets/all/met/SingletopBartW/Multiplicity");
   //hist11 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Jets/all/met/SingletopT/Multiplicity");
 }
 if(i==13)
  { 
   hist9 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/SingletoptW/MT");
   hist10 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/SingletopBartW/MT");
  // hist11 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/SingletopT/MT");
   }
 if(i==14)
  { 
   hist9 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/SingletoptW/DeltaPhiTauMet");
   hist10 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/SingletopBartW/DeltaPhiTauMet");
  // hist11 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/SingletopT/DeltaPhiTauMet");
  }
  if(i==15)
  { 
   hist9 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Jets/all/met/SingletoptW/Pt");
   hist10 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Jets/all/met/SingletopBartW/Pt");
   //hist11 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Jets/all/met/SingletopT/Pt");
  }
  if(i==16)
  { 
   hist9 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletoptW/sqrts");
   hist10 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletopBartW/sqrts");
   //hist11 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletopT/sqrts");
   hist9->Rebin(4);
   hist10->Rebin(4);
   //hist11->Rebin(4);
   }
  if(i==17)
  { 
   hist9  = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletoptW/kinFitChi2");
   hist10 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletopBartW/kinFitChi2");
  // hist11 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletopT/kinFitChi2");
  }
  if(i==18)
  { 
   hist9 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Jets/all/met/SingletoptW/Eta");
   hist10 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Jets/all/met/SingletopBartW/Eta");
  // hist11 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Jets/all/met/SingletopT/Eta");
  }
  if(i==19)
  { 
   hist9  = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/SingletoptW/Eta");
   hist10 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/SingletopBartW/Eta");
   //hist11 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/SingletopT/Eta");
  }
  if(i==20)
  { 
   hist9  = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/SingletoptW/NPV");
   hist10 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/SingletopBartW/NPV");
   //hist11 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/SingletopT/NPV");
  }
  
  // Get MultiJet histo
  
  TFile *_fileMULTIJET ;
  // _fileMULTIJET = TFile::Open("~/SemiLeptonicTauXsectionMeas_multijet_3-5fb_medtau_met15_medbtag_NPV.root");
  _fileMULTIJET = TFile::Open("~/Final_rootfiles/SemiLeptonicTauXsectionMeas_multijet_check.root");
  _fileMULTIJET->cd();
    
  if(i==22){
   hist5 = (TH1F*)_fileMULTIJET->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DATA/M3_NN5_30_50_350");
   hist5->Rebin(1);}
  if(i==0)
   hist5 = (TH1F*)_fileMULTIJET->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DATA/NNOutput");
  if(i==1){
   hist5 = (TH1F*)_fileMULTIJET->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DATA/Ht");
   hist5->Rebin(1);}
  if(i==2)
   hist5 = (TH1F*)_fileMULTIJET->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DATA/Aplanarity");
  if(i==3)
   hist5 = (TH1F*)_fileMULTIJET->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DATA/Sphericity");
  if(i==4)
   hist5 = (TH1F*)_fileMULTIJET->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DATA/C");
  if(i==5)
   hist5 = (TH1F*)_fileMULTIJET->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DATA/D");
  if(i==6)
   hist5 = (TH1F*)_fileMULTIJET->Get("SemiLeptonicTauAnaPlots/MET/all/met/DATA/Et");
  if(i==7)
   hist5 = (TH1F*)_fileMULTIJET->Get("SemiLeptonicTauAnaPlots/Taus/all/met/DATA/decayMode");
  if(i==8){
   hist5 = (TH1F*)_fileMULTIJET->Get("SemiLeptonicTauAnaPlots/Taus/all/met/DATA/Pt");
   hist5->Rebin(1);}
  if(i==9){
   hist5 = (TH1F*)_fileMULTIJET->Get("SemiLeptonicTauAnaPlots/Taus/all/met/DATA/q*Eta");
   hist5->Rebin(1);}
  if(i==10)
   hist5 = (TH1F*)_fileMULTIJET->Get("SemiLeptonicTauAnaPlots/Taus/all/met/DATA/leadTrackPt");
  if(i==11)
   hist5 = (TH1F*)_fileMULTIJET->Get("SemiLeptonicTauAnaPlots/Taus/all/met/DATA/emFraction");
  if(i==12)
   hist5 = (TH1F*)_fileMULTIJET->Get("SemiLeptonicTauAnaPlots/Jets/all/met/DATA/Multiplicity");
  if(i==13)
   hist5 = (TH1F*)_fileMULTIJET->Get("SemiLeptonicTauAnaPlots/Taus/all/met/DATA/MT");
  if(i==14)
   hist5 = (TH1F*)_fileMULTIJET->Get("SemiLeptonicTauAnaPlots/Taus/all/met/DATA/DeltaPhiTauMet");
  if(i==15)
    hist5 = (TH1F*)_fileMULTIJET->Get("SemiLeptonicTauAnaPlots/Jets/all/met/DATA/Pt");
  if(i==16){
    hist5 = (TH1F*)_fileMULTIJET->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DATA/sqrts");
    hist5->Rebin(4);}
  if(i==17)
   hist5 = (TH1F*)_fileMULTIJET->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DATA/kinFitChi2"); 
  if(i==18){
   hist5 = (TH1F*)_fileMULTIJET->Get("SemiLeptonicTauAnaPlots/Jets/all/met/DATA/Eta");
  }  
  if(i==19){
   hist5 = (TH1F*)_fileMULTIJET->Get("SemiLeptonicTauAnaPlots/Taus/all/met/DATA/Eta");
  }
  if(i==20){
   hist5 = (TH1F*)_fileMULTIJET->Get("SemiLeptonicTauAnaPlots/Taus/all/met/DATA/NPV");
  }
  // Get Data histo
  
  TFile *_fileDATA ;
  // _fileDATA = TFile::Open("~/SemiLeptonicTauXsectionMeas_data_3-5fb_medtau_met15_medbtag_NPV.root");
  _fileDATA = TFile::Open("~/Final_rootfiles/SemiLeptonicTauXsectionMeas_searchsample.root");
 
  //_fileDATA = TFile::Open("~/SemiLeptonicTauXsectionMeas_Data_1-991fb_relaxed4thjet_looseTau_rebinNN_3pfjet40_NPV.root");
  _fileDATA->cd();
  
  if(i==22){
   histDATA = (TH1F*)_fileDATA->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DATA/M3_NN5_30_50_350");
   histDATA->Rebin(1);}
  if(i==0)
   histDATA = (TH1F*)_fileDATA->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DATA/NNOutput");
  if(i==1){
   histDATA = (TH1F*)_fileDATA->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DATA/Ht");
   histDATA->Rebin(1);}
  if(i==2)
   histDATA = (TH1F*)_fileDATA->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DATA/Aplanarity");
  if(i==3)
   histDATA = (TH1F*)_fileDATA->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DATA/Sphericity");
  if(i==4)
   histDATA = (TH1F*)_fileDATA->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DATA/C");
  if(i==5)
   histDATA = (TH1F*)_fileDATA->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DATA/D");
  if(i==6)
   histDATA = (TH1F*)_fileDATA->Get("SemiLeptonicTauAnaPlots/MET/all/met/DATA/Et");
  if(i==7)
   histDATA = (TH1F*)_fileDATA->Get("SemiLeptonicTauAnaPlots/Taus/all/met/DATA/decayMode");
  if(i==8){
   histDATA = (TH1F*)_fileDATA->Get("SemiLeptonicTauAnaPlots/Taus/all/met/DATA/Pt"); 
   histDATA->Rebin(1);}
  if(i==9){
   histDATA = (TH1F*)_fileDATA->Get("SemiLeptonicTauAnaPlots/Taus/all/met/DATA/q*Eta"); 
   histDATA->Rebin(1);}
  if(i==10)
   histDATA = (TH1F*)_fileDATA->Get("SemiLeptonicTauAnaPlots/Taus/all/met/DATA/leadTrackPt");
  if(i==11)
   histDATA = (TH1F*)_fileDATA->Get("SemiLeptonicTauAnaPlots/Taus/all/met/DATA/emFraction");
  if(i==12)
   histDATA = (TH1F*)_fileDATA->Get("SemiLeptonicTauAnaPlots/Jets/all/met/DATA/Multiplicity");
  if(i==13)
   histDATA = (TH1F*)_fileDATA->Get("SemiLeptonicTauAnaPlots/Taus/all/met/DATA/MT");
  if(i==14)
   histDATA = (TH1F*)_fileDATA->Get("SemiLeptonicTauAnaPlots/Taus/all/met/DATA/DeltaPhiTauMet");
  if(i==15)
    histDATA = (TH1F*)_fileDATA->Get("SemiLeptonicTauAnaPlots/Jets/all/met/DATA/Pt");
  if(i==16){
    histDATA = (TH1F*)_fileDATA->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DATA/sqrts");
    histDATA->Rebin(4);}
  if(i==17)
   histDATA = (TH1F*)_fileDATA->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DATA/kinFitChi2");
  if(i==18)
    histDATA = (TH1F*)_fileDATA->Get("SemiLeptonicTauAnaPlots/Jets/all/met/DATA/Eta");
  if(i==19){
   histDATA = (TH1F*)_fileDATA->Get("SemiLeptonicTauAnaPlots/Taus/all/met/DATA/Eta"); 
   }
  if(i==20){
   histDATA = (TH1F*)_fileDATA->Get("SemiLeptonicTauAnaPlots/Taus/all/met/DATA/NPV"); 
   }
  if(i==22){
    histDATA = (TH1F*)_fileDATA->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DATA/M3_NN5_30_50_350");
 }
   
  
  hist0->SetFillColor(2);hist0->SetLineColor(2);
  hist1->SetFillColor(4);hist1->SetLineColor(4);
  hist2->SetFillColor(4);hist2->SetLineColor(4);
  hist3->SetFillColor(5);hist3->SetLineColor(5);
  hist4->SetFillColor(5);hist4->SetLineColor(5);
  hist5->SetFillColor(7);hist5->SetLineColor(7);
  hist6->SetFillColor(4);hist6->SetLineColor(4); 
  hist7->SetFillColor(4);hist7->SetLineColor(4);
  hist8->SetFillColor(4);hist8->SetLineColor(4);
  hist9->SetFillColor(6);hist9->SetLineColor(6);
  hist10->SetFillColor(6);hist10->SetLineColor(6);
 
  histDATA->SetMarkerStyle(20);
 
  // Scaling from the fit
 
  //loose tau  met 15
  //hist5->Scale(3.14697);
  //hist0->Scale(1.11073);
  
  //loose tau no b reweight met 15
  //hist5->Scale(0.252091);
  //hist0->Scale(1.30616);
  
  //med tau met 15
  //hist5->Scale(3.07762);
  //hist0->Scale(1.04014);
  
  //loose tau met25
  //hist5->Scale(3.22745); 
  //hist0->Scale(1.11232);
  
  //med tau med btag for multijet met 15
  hist5->Scale(4.65478); 
  hist0->Scale(0.919743);
  
  //med tau med btag for multijet met 15 no b reweight
  //hist5->Scale(0.2469); 
  //hist0->Scale(1.157);
 
  // For legend and plotting purposes
  
  TH1F*  hcolor4;
  TH1F*  hcolor5;
  TH1F*  hcolor6;
  TH1F*  hmc;
  
  hcolor4 = (TH1F*) hist1->Clone();
  hcolor4->Add(hist2);
  hcolor4->Add(hist6);
  hcolor4->Add(hist7);
  hcolor4->Add(hist8);
  
  hcolor5 = (TH1F*) hist3->Clone();
  hcolor5->Add(hist4);
  
  hcolor6 = (TH1F*) hist9->Clone();
  hcolor6->Add(hist10);
  //hcolor6->Add(hist11);
  
  hmc = (TH1F*) hist0->Clone(); hmc->Sumw2();
  hmc->Add(hcolor4);
  hmc->Add(hcolor5);
  hmc->Add(hcolor6);
  //hmc->Add(hist0);
  hmc->Add(hist5);
 
  
   TH1F* hmcHatchedArea = (TH1F*) hmc->Clone();
   hmcHatchedArea->SetLineColor(1);
   hmcHatchedArea->SetFillStyle(3004);
   hmcHatchedArea->SetFillColor(1);
 
  for(int j=0; j<hmcHatchedArea->GetNbinsX(); j++)
  {
    hmcHatchedArea->SetBinError(j+1,sqrt(pow(hmc->GetBinError(j+1),2)+pow(hmc->GetBinContent(j+1)*0.13,2)));}
    
 
  //TCanvas *c1 = new TCanvas("c1", "c1",10,32,782,552);
  TCanvas *c1 = new TCanvas("c1", "c1",10,54,782,552);
  c1->cd();
   
  //TPad *canvas_1 = new TPad("canvas_1", "canvas_1",0,0.5,1.0,0.98);
  TPad *canvas_1 = new TPad("canvas_1", "canvas_1",0,0.25,1.0,0.98);
  canvas_1->Range(-8.307211,-9.283388,357.5235,53.01629);
  canvas_1->SetLeftMargin(0.159383);
  canvas_1->SetRightMargin(0.02056555);
  canvas_1->SetTopMargin(0.06148698);
  canvas_1->SetBottomMargin(0.1490118);
  
  
  int logy = 0;
  canvas_1->SetLogy(logy);
  canvas_1->Draw();
  canvas_1->cd(); 
  
  histDATA->GetYaxis()->SetTitleFont(42);
  histDATA->GetXaxis()->SetLabelSize(0);
  histDATA->GetYaxis()->SetLabelFont(42);
  histDATA->GetYaxis()->SetTitleOffset( 0.8 );
  histDATA->GetYaxis()->SetTitleSize( 0.05);
  histDATA->GetYaxis()->SetLabelSize(0.05); 
 // histDATA->SetMinimum(0);
 // histDATA->SetMaximum(histDATA->GetMaximum()+15);
  histDATA->SetMaximum(50);
  //histDATA->GetXaxis()->SetNdivisions(0);
 histDATA->Draw("e"); 
  //hist0->GetXaxis()->SetNdivisions(0);
  //histDATA->SetMinimum(1);
  
  if(i==0)histDATA->GetYaxis()->SetTitle("Events / 0.1");  
  else histDATA->GetYaxis()->SetTitle("Events / 10 GeV");
  histDATA->GetYaxis()->SetNdivisions(5,1,0,kFALSE);
  hs.Add(hist5); 
  hs.Add(hist1);
  hs.Add(hist2);
  hs.Add(hist6); 
  hs.Add(hist7);
  hs.Add(hist8);
  hs.Add(hist3);
  hs.Add(hist4);
  hs.Add(hist9);
  hs.Add(hist10);
  hs.Add(hist0);
 
  hs.Draw("histosame");hmcHatchedArea->Draw("e2same");
 //histDATA->Draw("esameaxis");
 histDATA->Draw("esame");
 //histDATA->Draw("sameaxis");
  
  TLegend* qw = 0;
  qw = new TLegend(0.6580977,0.507163,0.9498715,0.9306703,NULL,"brNDC");
  //qw = new TLegend(0.75,0.70,0.98,0.98);
  
  qw->AddEntry(histDATA, "Data" ,             "p");
  qw->AddEntry(hmcHatchedArea,  "stat.+syst. uncertainty" ,"f");
  qw->AddEntry(hist0,    "t#bar{t} #tau_{h}+jets" ,"f");
  qw->AddEntry(hcolor6,  "single top" ,"f");
  qw->AddEntry(hcolor5,  "W/Z + jets" ,"f");
  qw->AddEntry(hcolor4,  "t#bar{t} background" ,"f");
  qw->AddEntry(hist5,    "multijet "          ,"f");
  
  qw->SetFillColor(0);
  qw->SetTextFont(42);
  qw->SetLineWidth(0);
  qw->SetBorderSize(0);
  
  qw->Draw();
    
  text1 = new TLatex(0.15,0.9,"CMS #sqrt{s}=7 TeV, 3.9 fb^{-1}");
  text1->SetNDC();
  text1->SetTextAlign(12);
  text1->SetX(0.18);
  //text1->SetY(0.92);
  //text1->SetLineWidth(2);
  text1->SetTextFont(42);
  text1->SetTextSize(0.05);
  //text1->SetTextSizePixels(24);// dflt=28
  text1->Draw();
  canvas_1->RedrawAxis() ;
  
  //
 
  c1->cd();

  
  TPad *canvas_2 = new TPad("canvas_2", "canvas_2",0,0.,1.0,0.34);
 // canvas_2->Range(-8.307211,-1.381443,357.5235,2.291959);
  canvas_2->Draw();
  canvas_2->cd();
  gPad->SetBottomMargin(0.375);
  gPad->SetGridy();
  gPad->SetGridx();

  histo_ratio = (TH1F*) histDATA->Clone();
  
  //histDATA->GetXaxis()->SetNdivisions(0);
 //histDATA->GetXaxis()->SetRange(100,300);
 //histDATA->GetXaxis()->SetTicks(0);
 
  histo_ratio->SetName("histo_ratio");
  histo_ratio->SetTitle("");
  histo_ratio->Divide(hmc);

  histo_ratio->SetMarkerStyle(20);
  histo_ratio->SetMarkerSize(0.7);
  //histo_ratio->GetXaxis()->SetRange(4,28);
  histo_ratio->GetYaxis()->SetTitle("Data/MC");
  histo_ratio->GetYaxis()->SetTitleFont(42);
  histo_ratio->GetYaxis()->SetLabelFont(42);
  histo_ratio->GetXaxis()->SetTitleFont(42);
  histo_ratio->GetXaxis()->SetLabelFont(42);
  histo_ratio->GetXaxis()->SetLabelSize(0.1);

  histo_ratio->GetYaxis()->SetTitleOffset( 0.4 );
  histo_ratio->GetYaxis()->SetTitleSize( 0.1 );
  histo_ratio->GetYaxis()->SetLabelSize(0.1);
  histo_ratio->GetYaxis()->SetNdivisions( 505 );
  histo_ratio->GetXaxis()->SetTitleSize( 0.15 );
  histo_ratio->GetXaxis()->SetTitle( " M_{3} (GeV), D_{NN}>0.5" );

  histo_ratio->SetMinimum(0.);
  histo_ratio->SetMaximum(2.5);
  histo_ratio->Draw("E1X0");

  c1->cd();
   
  
 // Printing
  
  TString name1 = "c1_topmeeting_medtau_met15_medbtag_log";
  TString name2 = Form("%d", i);
  TString name3 = ".pdf";
  TString name4 = ".png";
  TString name5 = Form("%d", logy);
  TString name6 = "_";
  TString name_pdf = name1+name5+name6+name2+name3;
  TString name_png = name1+name5+name6+name2+name4;
  
  c1->SaveAs(name_pdf.Data());
  //c1->SaveAs(name_png.Data());

}

