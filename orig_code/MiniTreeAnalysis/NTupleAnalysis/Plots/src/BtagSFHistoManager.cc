#include "../interface/BtagSFHistoManager.h"




BtagSFHistoManager::BtagSFHistoManager(){
}

BtagSFHistoManager::~BtagSFHistoManager(){
}


void BtagSFHistoManager::CreateHistos( const int& nb, float* valsfb,  const int& nl, float* valsfl){
//        nb=         number of SF_b values
//        valsfb[nb]= list of SF_b values
//        nl=         number of SF_l values
//        valsfl[nl]= list of SF_l values
        AddHisto2D_BC(string("NoSel"),string("SF map for NoSel"),string("SF_b"),nb,valsfb,string("SF_l"),nl,valsfl);    
        AddHisto2D_BC(string("SelB0"),string("SF map for SelB=0"),string("SF_b"),nb,valsfb,string("SF_l"),nl,valsfl);   
        AddHisto2D_BC(string("SelB1"),string("SF map for SelB=1"),string("SF_b"),nb,valsfb,string("SF_l"),nl,valsfl);   
        AddHisto2D_BC(string("SelB2"),string("SF map for SelB=2"),string("SF_b"),nb,valsfb,string("SF_l"),nl,valsfl);   
        AddHisto2D_BC(string("SelBge3"),string("SF map for SelB>=3"),string("SF_b"),nb,valsfb,string("SF_l"),nl,valsfl);        
        AddHisto2D_BC(string("SelBge2"),string("SF map for SelB>=2"),string("SF_b"),nb,valsfb,string("SF_l"),nl,valsfl);        

        AddHisto2D_BC(string("EffSelB0"),string("Efficiency for SelB=0"),string("SF_b"),nb,valsfb,string("SF_l"),nl,valsfl);    
        AddHisto2D_BC(string("EffSelB1"),string("Efficiency for SelB=1"),string("SF_b"),nb,valsfb,string("SF_l"),nl,valsfl);    
        AddHisto2D_BC(string("EffSelB2"),string("Efficiency for SelB=2"),string("SF_b"),nb,valsfb,string("SF_l"),nl,valsfl);    
        AddHisto2D_BC(string("EffSelBge3"),string("Efficiency for SelB>=3"),string("SF_b"),nb,valsfb,string("SF_l"),nl,valsfl); 
        AddHisto2D_BC(string("EffSelBge2"),string("Efficiency for SelB>=2"),string("SF_b"),nb,valsfb,string("SF_l"),nl,valsfl); 
}

void BtagSFHistoManager::Fill(const int& sfbval, const int& sflval, const int& iChannel, const int& iNjetSel, const int& iDataset, const vector<float>& weight){

//        cout << " sfbval " << sfbval << " sflval " << sflval << " iChannel " << iChannel << " iNjetSel " << iNjetSel << " iDataset " << iDataset
//             << " w " << weight[0] << " sel0 " <<  weight[1] << " sel1 " <<  weight[2] << " sel2 " << weight[3] << " sel3 " << weight[4] << endl;

        if(!Check(iChannel, iDataset)) return;
        if(!Check2D(iChannel, iNjetSel, iDataset, 0) ) return;
        // this works if SF weighting is activated in xml!
        FillSelStep( Histos2D[0][iChannel][iNjetSel][iDataset], sfbval, sflval, iNjetSel , iChannel, iDataset, weight[0]);      // all events ==> need to change weightb[0] in macros!
        FillSelStep( Histos2D[1][iChannel][iNjetSel][iDataset], sfbval, sflval, iNjetSel , iChannel, iDataset, weight[1]);      // 0 bjets
        FillSelStep( Histos2D[2][iChannel][iNjetSel][iDataset], sfbval, sflval, iNjetSel , iChannel, iDataset, weight[2]);      // 1 bjets
        FillSelStep( Histos2D[3][iChannel][iNjetSel][iDataset], sfbval, sflval, iNjetSel , iChannel, iDataset, weight[3]);      // 2 bjets
        FillSelStep( Histos2D[4][iChannel][iNjetSel][iDataset], sfbval, sflval, iNjetSel , iChannel, iDataset, weight[4]);      // >=3 bjets
        FillSelStep( Histos2D[5][iChannel][iNjetSel][iDataset], sfbval, sflval, iNjetSel , iChannel, iDataset, weight[3]+weight[4]);// >=2 bjets
        if (iNjetSel>=2) {
        }
}

void BtagSFHistoManager::FillSelStep(TH2D& hh, const int& sfbval, const int& sflval, const int& iNjetSel, const int& iChannel, const int& iDataset, float weight){
        float info_1=hh.GetBinContent(sfbval+1,sflval+1);
        float error_1=hh.GetBinError(sfbval+1,sflval+1);
        float new_info = info_1 + weight;
        float new_error = sqrt(error_1*error_1+weight*weight);
        hh.SetBinContent(sfbval+1,sflval+1,new_info);
        hh.SetBinError(sfbval+1,sflval+1,new_error);

//        float info_2=hh.GetBinContent(sfbval+1,sflval+1);
//        cout << " histo " << hh.GetName() << " info1 " << info_1 << " info2 " << new_info << " test " << info_2 << endl;
}


void BtagSFHistoManager::Compute(){
      // compute the efficiency of the selection

     // channels loop
     for(unsigned int i=0;i<Channels.size();i++){
           //selection steps loop
           for(unsigned int j=0;j<SelectionSteps.size();j++){
                 //datasets loop
                 for(unsigned int k=0;k<Datasets.size();k++){
                                EffCompute(Histos2D[6][i][j][k],Histos2D[1][i][j][k], Histos2D[0][i][j][k]);  // Sel0
                                EffCompute(Histos2D[7][i][j][k],Histos2D[2][i][j][k], Histos2D[0][i][j][k]);  // Sel1
                                EffCompute(Histos2D[8][i][j][k],Histos2D[3][i][j][k], Histos2D[0][i][j][k]);  // Sel2
                                EffCompute(Histos2D[9][i][j][k],Histos2D[4][i][j][k], Histos2D[0][i][j][k]);  // Sel>=3
                                EffCompute(Histos2D[10][i][j][k],Histos2D[5][i][j][k], Histos2D[0][i][j][k]);  // Sel>=2
                 }
            }
      }

};

void BtagSFHistoManager::EffCompute(TH2D& hheff, TH2D& hhnum, TH2D& hhdenom){


   for (int i=0; i<hhnum.GetNbinsX(); i++) {
     for (int j=0; j<hhnum.GetNbinsY(); j++) {
        float num=hhnum.GetBinContent(i+1,j+1);
        float denom=hhdenom.GetBinContent(i+1,j+1);
        float err_denom=hhdenom.GetBinError(i+1,j+1);
        float eff=0;
        if (denom>0) eff=num/denom;
        hheff.SetBinContent(i+1,j+1,eff);
        // erreur : binomiale 
        float neq =0;
        if (err_denom>0) neq= denom*denom/(err_denom*err_denom);
        float err_eff= 0;
        if (neq>0) err_eff=sqrt(eff*(1-eff)/neq);
        hheff.SetBinError(i+1,j+1,err_eff);
     }
   }


}

void BtagSFHistoManager::PlotTGraph(TDirectory* dir){

       //Temporary variable;
       TDirectory* dir1 = dir;
       vector<TCanvas*> cantowrite;
       gROOT->SetStyle("Plain");

       dir->cd();

       const int maxi=20;
       const int maxj=5; // 5 Btag selections considered 
       int ii=-1;
       int jj=-1;
       const int maxbin=20;
       float sfb[maxbin];
       int indexb=0;
       float sfl[maxbin];
       int indexl=0;
       float eff[maxi][maxj][maxbin][maxbin];
       float efferr[maxi][maxj][maxbin][maxbin];
       float tot_check[maxi][maxbin][maxbin];
       for (unsigned int i1=0; i1<maxi; i1++) {
         for (int k1=0; k1<maxbin; k1++) {
            for (int k2=0; k2<maxbin; k2++) {
               tot_check[i1][k1][k2]=0;
            }
         }
       }

       TH2F *h_support = new TH2F("h_support","Btag Selection vs SF_b",10,0.5,1.5,10,0.,1.2);
       h_support->SetStats(0);


       dir->cd();
       for(unsigned int iChannel = 0;iChannel<Channels.size();iChannel++){
         dir1 = dir->mkdir(Channels[iChannel].c_str());
         dir1->cd();
         for(unsigned int iDataset =0;iDataset<Datasets.size();iDataset++){
          // one canvas per dataset and per channel 
          TCanvas * c1 = new TCanvas();
          string name = "c";
          name = Datasets[iDataset].Name().c_str();
          c1->SetTitle(name.c_str());
          pair<int,int> div = CanvasDivide(SelectionSteps.size());
          if(div.second!=0) c1->Divide(div.first, div.second);
          else c1->Divide(div.first);
          TLegend*  qw = 0;
          for (unsigned int i1=0; i1<SelectionSteps.size(); i1++) {
             for (int j1=0; j1<maxj; j1++) {
                TH2D* h1 = (TH2D*) Histos2D[j1+6][iChannel][i1][iDataset].Clone();
               //==============================================================
               // decode SF_b and SF_l info
                 if (i1==0 && j1==0 && iChannel==0 && iDataset==0) {
                  cout << " ======SFb===== " << endl;
                  for (int i=0; i<h1->GetNbinsX(); i++) {
                     sfb[indexb]= atof(h1->GetXaxis()->GetBinLabel(i+1));
                     cout << indexb << " " <<  sfb[indexb]  << endl;
                     if (sfb[indexb]==1.) ii=indexb;
                     indexb++;
                  }
                  cout << " ======SFl===== " << endl;
                  for (int i=0; i<h1->GetNbinsY(); i++) {
                     sfl[indexl]= atof(h1->GetYaxis()->GetBinLabel(i+1));
                     cout << indexl << " " <<  sfl[indexl]  << endl;
                     if (sfl[indexl]==1.) jj=indexl;
                     indexl++;
                  }
                  cout << " ============== " << endl;

                }
                //==============================================================
                // keep eff info
                for (int i=0; i<h1->GetNbinsX(); i++) {
                   for (int j=0; j<h1->GetNbinsY(); j++) {
                      eff[i1][j1][i][j]=h1->GetBinContent(i+1,j+1);
                      efferr[i1][j1][i][j]=h1->GetBinError(i+1,j+1);
                      // check du tot des efficacites par Sel de Jets, par SFb et par SFl
                      // on ne prend pas j1==4 car Nbtag>=2 (deja inclus dans Nbtag=2 et >=3)
                      if (j1<4) tot_check[i1][i][j]+=eff[i1][j1][i][j];
                   }       
                 }
               //==============================================================
             } //j1 - end loop



             // plot TGraph in canvas
             c1->cd(i1+1);
             char newtit[60];
             sprintf(newtit,"%s for %s",SelectionSteps[i1].c_str(),Datasets[iDataset].Name().c_str());
             TH2F* hh_sup = (TH2F*) h_support->Clone();
             hh_sup->SetTitle(newtit);
             hh_sup->Draw();



             qw = new TLegend(0.70,0.70,0.88,0.88);
             qw->SetHeader("Btag selection");



             for (int j1=0; j1<maxj; j1++) {
               for (int k2=0; k2<indexl; k2++) {
               // loop SF_l
                  float aa[maxbin];
                  float bb[maxbin];
                  float esfb[maxbin];
                  for (int k1=0; k1<indexb; k1++) {
                      // loop SF_b
                      aa[k1]=eff[i1][j1][k1][k2]; 
                      if (k2==jj) bb[k1]=efferr[i1][j1][k1][k2]; 
                      else bb[k1]=0;
                      esfb[k1]=0.;
                  } // SF_b - end loop

                  //TGraph* gaa = new TGraph(indexb,sfb,aa);
                  // plot TGraph with errors only for sf_l==1, otherwise bb=0!
                  TGraphErrors* gaa = new TGraphErrors(indexb,sfb,aa, esfb,bb);
                  int ikcol=0;
                  if (j1==0) ikcol=13; // 0 bjets
                  else if (j1==1) ikcol=2; // 1 bjets
                  else if (j1==2) ikcol=4; // 2 bjets
                  else if (j1==3) ikcol=8; // >=3 bjets
                  else if (j1==4) ikcol=1; // >=2 bjets
                  gaa->SetMarkerColor(ikcol);
                  if (k2==jj) gaa->SetMarkerStyle(20);
                  gaa->SetLineColor(ikcol);
                  if (k2!=jj) gaa->SetLineStyle(2);
                  gaa->Draw("pl");
                  char selname[20];
                  if      (j1==0) sprintf(selname,"P(Nbtag=0)");
                  else if (j1==1) sprintf(selname,"P(Nbtag=1)");
                  else if (j1==2) sprintf(selname,"P(Nbtag=2)");
                  else if (j1==3) sprintf(selname,"P(Nbtag>=3)");
                  else if (j1==4) sprintf(selname,"P(Nbtag>=2)");
                  if (k2==jj) qw->AddEntry(gaa,    selname,"p");


               } // SF_l - end loop
             } // j1 - end loop



             qw->SetFillColor(0);
             qw->Draw();



          } // i1 - end loop

          // check du tot des eff
          for (unsigned int i1=0; i1<SelectionSteps.size(); i1++) {
             for (int k1=0; k1<indexb; k1++) {
               for (int k2=0; k2<indexl; k2++) {
                    if(tot_check[i1][k1][k2]<0.999) cout << " Probleme : Eff[" << i1 << "]["<< k1 << "][" << k2 << "] < 0.999 " << tot_check[i1][k1][k2] << endl;
                    if(tot_check[i1][k1][k2]>1.001) cout << " Probleme : Eff[" << i1 << "]["<< k1 << "][" << k2 << "] < 1.001 " << tot_check[i1][k1][k2] << endl;
               }
             }
          }

          c1->Write();
          c1=0;
          delete c1;
          qw=0;
          delete qw;
         } // iDataset - end loop
       } // iChannel - end loop



    dir1=0;
    delete dir1;

}
