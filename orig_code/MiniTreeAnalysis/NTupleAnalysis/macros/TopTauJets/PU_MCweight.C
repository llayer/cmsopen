{
  TFile* file = new TFile("/opt/sbg/data/data1/cms/cferro/TTBar_outoftime-0807/7968d69237c592f572456e64d6c16e5d/NTuple.root");
  TTree* tree = (TTree*) file->Get("MyModule/Event");
  
  TFile *file1 = new TFile("Crosssection.root","RECREATE");
  file1->cd();
  
  
  TH1D * pileup = new TH1D("pileup","pileup",51,-0.5,50.5);
  // pileup->Sumw2();
  tree->Draw("NTEvent.num_pileup_bc0>>pileup");
  
  pileup->Write();
  
  file1->Close();
  
}

