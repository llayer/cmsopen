#include "../interface/HistoManager.h"

HistoManager::HistoManager(){
}

HistoManager::~HistoManager(){
}


void HistoManager::LoadDatasets(vector<Dataset> datasets){
        for(unsigned int i=0;i<datasets.size();i++){
                Datasets.push_back(datasets[i]);
        }
}    


void HistoManager::LoadSelectionSteps(vector<string> selectionSteps){
        SelectionSteps = selectionSteps;
        for(unsigned int i=0;i<SelectionSteps.size();i++){
                SelStepColor.push_back(2+i);
        }
}

void HistoManager::LoadSelectionSteps(vector<string> selectionSteps, vector<int> selectionStepColor){
        SelectionSteps = selectionSteps;
        if(selectionSteps.size()==selectionStepColor.size())
                SelStepColor = selectionStepColor;
        else
                for(unsigned int i=0;i<SelectionSteps.size();i++){
                        SelStepColor.push_back(2+i);
                }
                
}

void HistoManager::LoadChannels(vector<string> channels){
        Channels = channels;
}


void HistoManager::Clear(){
        SumMCDatasetsHistos.clear(); 
        SumDataDatasetsHistos.clear(); 
        MCStack.clear();            
        MCDataCompCanvas.clear();           
        MCDataCompCanvasDebug.clear();      
        CutEffectCanvas.clear();            
        CutEffectCanvasDebug.clear();       
        SumMCDatasetsHistos2D.clear(); 
        SumDataDatasetsHistos2D.clear(); 
}


void HistoManager::AddHisto(string name, string title, string xaxis, const int& nbins, const float& min, const float& max){
        TH1F h (name.c_str(),title.c_str(),nbins,min,max);
        h.GetXaxis()->SetTitle(xaxis.c_str());
        h.Sumw2();
        //channels loop
        vector<vector<vector<TH1F> > > v3;
        for(unsigned int i=0;i<Channels.size();i++){
                //selection steps loop
                vector<vector<TH1F> > v2;
                for(unsigned int j=0;j<SelectionSteps.size();j++){
                        //datasets loop
                        vector<TH1F> v1;
                        for(unsigned int k=0;k<Datasets.size();k++){
                                //h.SetLineColor(Datasets[k]->Color());
                                v1.push_back(h);
                        }
                        v2.push_back(v1);
                }
                v3.push_back(v2);
        }
        Histos.push_back(v3);
        //h = 0;
        //delete h;
}

void HistoManager::AddHisto2D(string name, string title, string xaxis, const int& nxbins, const float& xmin, const float& xmax, string yaxis, const int& nybins, const float& ymin, const float& ymax) {
        
        TH2D h (name.c_str(),title.c_str(),nxbins,xmin,xmax,nybins,ymin,ymax);
        h.GetXaxis()->SetTitle(xaxis.c_str());
        h.GetYaxis()->SetTitle(yaxis.c_str());
        h.Sumw2();
        //channels loop
        vector<vector<vector<TH2D> > > v3;
        for(unsigned int i=0;i<Channels.size();i++){
                //selection steps loop
                vector<vector<TH2D> > v2;
                for(unsigned int j=0;j<SelectionSteps.size();j++){
                        //datasets loop
                        vector<TH2D> v1;
                        for(unsigned int k=0;k<Datasets.size();k++){
                                //h.SetLineColor(Datasets[k]->Color());
                                v1.push_back(h);
                        }
                        v2.push_back(v1);
                }
                v3.push_back(v2);
        }
        Histos2D.push_back(v3);
}

void HistoManager::AddHisto2D_BC(string name, string title, string xaxis, const int& nxbins, float* rangex, string yaxis, const int& nybins, float * rangey) {
   // special to keep bin center information

        TH2D h (name.c_str(),title.c_str(),nxbins,0, nxbins,nybins,0,nybins);
        TAxis* a1=h.GetXaxis();
        a1->SetTitle(xaxis.c_str());
        TAxis* b1=h.GetYaxis();
        b1->SetTitle(yaxis.c_str());
        for (int i1=0; i1<nxbins; i1++) {
          char blablibloblu[10];
          sprintf(blablibloblu,"%f",rangex[i1]);
          a1->SetBinLabel(i1+1,blablibloblu);
        }
        for (int i2=0; i2<nybins; i2++) {
          char blabliblobleu[10];
          sprintf(blabliblobleu,"%f",rangey[i2]);
          b1->SetBinLabel(i2+1,blabliblobleu);
        }
        h.Sumw2();
        //channels loop
        vector<vector<vector<TH2D> > > v3;
        for(unsigned int i=0;i<Channels.size();i++){
                //selection steps loop
                vector<vector<TH2D> > v2;
                for(unsigned int j=0;j<SelectionSteps.size();j++){
                        //datasets loop
                        vector<TH2D> v1;
                        for(unsigned int k=0;k<Datasets.size();k++){
                                //h.SetLineColor(Datasets[k]->Color());
                                v1.push_back(h);
                        }
                        v2.push_back(v1);
                }
                v3.push_back(v2);
        }
        Histos2D.push_back(v3);
}


void HistoManager::Fill(const int& iChannel, const int& iSelStep, const int& iDataset, const int& varNumber, const float& value, const float& weight){
        Histos[varNumber][iChannel][iSelStep][iDataset].Fill(value, weight);
}   

void HistoManager::Fill2D(const int& iChannel, const int& iSelStep, const int& iDataset, const int& varNumber, const float& value, const float& weight){
        Histos2D[varNumber][iChannel][iSelStep][iDataset].Fill(value, weight);
}   


bool HistoManager::Check(const int& iChannel, const int& iDataset){
        if(iChannel>= (int)Channels.size()){
                //cerr<<"HistoManager::iChannel incorrect"<<endl;
                return false;
        }
        if(iDataset>= (int)Datasets.size()){
                cerr<<"HistoManager::iDataset incorrect"<<endl;
                return false;
        }
        return true;
}       


bool HistoManager::Check(const int& iChannel, const int& iSelStep, const int& iDataset, const int& varNumber){
        if(varNumber<0 || varNumber>=(int)Histos.size()){
                cerr<<"HistoManger::varNumber incorrect"<<endl;
                return false;
        }
        if(iChannel<0 || iChannel>=(int) Channels.size() || iChannel>=(int)Histos[varNumber].size()){
                //cerr<<"HistoManger::iChannel incorrect"<<endl;
                return false;
        }
        if(iSelStep<0 || iSelStep>=(int) SelectionSteps.size() || iSelStep>=(int)Histos[varNumber][iChannel].size()){
                cerr<<"HistoManger::iSelStep incorrect"<<endl;
                return false;
        }
        if(iDataset<0 || iDataset>=(int)  Datasets.size() || iDataset>=(int)Histos[varNumber][iChannel][iSelStep].size()){
                cerr<<"HistoManger::iDataset incorrect"<<endl;
                return false;
        }
        return true;
}

bool HistoManager::Check2D(const int& iChannel, const int& iSelStep, const int& iDataset, const int& varNumber){
        if(varNumber<0 || varNumber>=(int)Histos2D.size()){
                cerr<<"HistoManger::varNumber incorrect"<<endl;
                return false;
        }
        if(iChannel<0 || iChannel>=(int) Channels.size() || iChannel>=(int)Histos2D[varNumber].size()){
                //cerr<<"HistoManger::iChannel incorrect"<<endl;
                return false;
        }
        if(iSelStep<0 || iSelStep>=(int) SelectionSteps.size() || iSelStep>=(int)Histos2D[varNumber][iChannel].size()){
                cerr<<"HistoManger::iSelStep incorrect"<<endl;
                return false;
        }
        if(iDataset<0 || iDataset>=(int)  Datasets.size() || iDataset>=(int)Histos2D[varNumber][iChannel][iSelStep].size()){
                cerr<<"HistoManger::iDataset incorrect"<<endl;
                return false;
        }
        return true;
}
bool HistoManager::Check(vector<vector<TCanvas*> >& can, int ix, int iy){
        if ((int)can.size()<= ix) {
                return false;
        }
        if ((int)can[ix].size()<= iy) {
                return false;
        }
        return true;
}       
bool HistoManager::Check(vector<vector<vector<TCanvas*> > >& can, int ix, int iy, int iz){
        if ((int)can.size()<= ix) {
                return false;
        }
        if ((int)can[ix].size()<= iy) {
                return false;
        }
        if ((int)can[ix][iy].size()<= iz) {
                return false;
        }
        return true;
}       
bool HistoManager::Check(vector<vector<vector<vector<TCanvas*> > > >& can, int ix, int iy, int iz, int izz){
        if ((int)can.size()<= ix) {
                return false;
        }
        if ((int)can[ix].size()<= iy) {
                return false;
        }       
        if ((int)can[ix][iy].size()<= iz) {
                return false;
        }
        if ((int)can[ix][iy][iz].size()<= izz) {
                return false;
        }
        return true;
}  

        //vector<vector<TH1F*> > SumChannelsHistos;   /** dimensions: SelectionSteps.size()xDatasets.size() */
        //vector<vector<vector<TH1F> > > SumMCDatasetsHistos; /** dimensions: Channels.size()xSelectionSteps.size()xNofHistos */
        //vector<vector<vector<THStack> > > MCStack;        /** dimensions: Channels.size()xSelectionSteps.size()xNofHistos */
        //vector<vector<vector<TCanvas > > > MCDataCompCanvas;      /** dimensions: Channels.size()xSelectionSteps.size()xNofHistos */
//      vector<vector<TCanvas> > MCDataCompCanvasDebug;             /** dimensions: Channels.size()xNofHistos - SelectionSteps.size() plots in a canvas*/
//      vector<vector<vector<TCanvas> > > CutEffectCanvas;          /** dimensions: Channels.size()xDatasets.size()xNofHistos */
//      vector<vector<TCanvas> > CutEffectCanvasDebug;      /** dimensions: Channels.size()xNofHistos - Datasets.size() plots in a canvas*/

void HistoManager::MergeChannels(){
        if(Channels.size()<2) return;
        for(unsigned int varNumber = 0 ;varNumber<Histos.size();varNumber++){
                vector<vector<TH1F> > v2;
                for(unsigned int iSelStep = 0 ;iSelStep<SelectionSteps.size();iSelStep++){
                        vector<TH1F> v1;
                        for(unsigned int iDataset = 0;iDataset<Datasets.size();iDataset++){
                                if(Channels.size()>0){
                                        TH1F h = Histos[varNumber][0][iSelStep][iDataset];
                                        for(unsigned int iChannel = 1;iChannel<Channels.size();iChannel++){
                                                h.Add(&Histos[varNumber][iChannel][iSelStep][iDataset]);
                                        }
                                        v1.push_back(h);
                                        //h = 0;
                                        //delete h;
                                }
                        }
                        v2.push_back(v1);
                }
                Histos[varNumber].push_back(v2);
        }       
        Channels.push_back("merged");
}

void HistoManager::MergeChannels2D(){
        if(Channels.size()<2) return;
        for(unsigned int varNumber = 0 ;varNumber<Histos2D.size();varNumber++){
                vector<vector<TH2D> > v2;
                for(unsigned int iSelStep = 0 ;iSelStep<SelectionSteps.size();iSelStep++){
                        vector<TH2D> v1;
                        for(unsigned int iDataset = 0;iDataset<Datasets.size();iDataset++){
                                if(Channels.size()>0){
                                        TH2D h = Histos2D[varNumber][0][iSelStep][iDataset];
                                        for(unsigned int iChannel = 1;iChannel<Channels.size();iChannel++){
                                                h.Add(&Histos2D[varNumber][iChannel][iSelStep][iDataset]);
                                        }
                                        v1.push_back(h);
                                        //h = 0;
                                        //delete h;
                                }
                        }
                        v2.push_back(v1);
                }
                Histos2D[varNumber].push_back(v2);
        }       
        Channels.push_back("merged");
}

void HistoManager::MergeMCDatasets(){
        for(unsigned int varNumber = 0 ;varNumber<Histos.size();varNumber++){
                vector<vector<TH1F> >v2MC;
                vector<vector<TH1F> >v2Data;
                for(unsigned int iSelStep = 0 ;iSelStep<SelectionSteps.size();iSelStep++){
                        vector<TH1F> v1MC;
                        vector<TH1F> v1Data;
                        for(unsigned int iChannel = 0;iChannel<Channels.size();iChannel++){
                                TH1F hData;
                                TH1F hMC;
                                bool firstMCD = true;
                                bool firstDataD = true;
                                for(unsigned int iDataset = 0;iDataset<Datasets.size();iDataset++){
                                        if(Datasets[iDataset].isData()){
                                                if(firstDataD){ 
                                                        hData = Histos[varNumber][iChannel][iSelStep][iDataset];
                                                        hData.SetName(Histos[varNumber][iChannel][iSelStep][iDataset].GetName()+TString("_Data"));
                                                        firstMCD = false;
                                                }
                                                else hData.Add(&Histos[varNumber][iChannel][iSelStep][iDataset]);
                                        }
                                        else{
                                                if(firstMCD){ 
                                                        hMC = Histos[varNumber][iChannel][iSelStep][iDataset];
                                                        hMC.SetName(Histos[varNumber][iChannel][iSelStep][iDataset].GetName()+TString("_MC"));
                                                        firstMCD = false;
                                                }
                                                else hMC.Add(&Histos[varNumber][iChannel][iSelStep][iDataset]);
                                        }
                                }
                                v1MC.push_back(hMC);
                                v1Data.push_back(hData);
                                //h = 0;
                                //delete h;
                        }
                        v2MC.push_back(v1MC);
                        v2Data.push_back(v1Data);
                }
                SumMCDatasetsHistos.push_back(v2MC);
                SumDataDatasetsHistos.push_back(v2Data);
        }       

}

void HistoManager::MergeMCDatasets2D(){
        for(unsigned int varNumber = 0 ;varNumber<Histos2D.size();varNumber++){
                vector<vector<TH2D> >v2MC;
                vector<vector<TH2D> >v2Data;
                for(unsigned int iSelStep = 0 ;iSelStep<SelectionSteps.size();iSelStep++){
                        vector<TH2D> v1MC;
                        vector<TH2D> v1Data;
                        for(unsigned int iChannel = 0;iChannel<Channels.size();iChannel++){
                                TH2D hData;
                                TH2D hMC;
                                bool firstMCD = true;
                                bool firstDataD = true;
                                for(unsigned int iDataset = 0;iDataset<Datasets.size();iDataset++){
                                        if(Datasets[iDataset].isData()){
                                                if(firstDataD){ 
                                                        hData = Histos2D[varNumber][iChannel][iSelStep][iDataset];
                                                        hData.SetName(Histos2D[varNumber][iChannel][iSelStep][iDataset].GetName()+TString("_Data"));
                                                        firstMCD = false;
                                                }
                                                else hData.Add(&Histos2D[varNumber][iChannel][iSelStep][iDataset]);
                                        }
                                        else{
                                                if(firstMCD){ 
                                                        hMC = Histos2D[varNumber][iChannel][iSelStep][iDataset];
                                                        hMC.SetName(Histos2D[varNumber][iChannel][iSelStep][iDataset].GetName()+TString("_MC"));
                                                        firstMCD = false;
                                                }
                                                else hMC.Add(&Histos2D[varNumber][iChannel][iSelStep][iDataset]);
                                        }
                                }
                                v1MC.push_back(hMC);
                                v1Data.push_back(hData);
                                //h = 0;
                                //delete h;
                        }
                        v2MC.push_back(v1MC);
                        v2Data.push_back(v1Data);
                }
                SumMCDatasetsHistos2D.push_back(v2MC);
                SumDataDatasetsHistos2D.push_back(v2Data);
        }       

}


void HistoManager::DoMCStack(){
        for(unsigned int varNumber = 0 ;varNumber<Histos.size();varNumber++){
                vector<vector<THStack*> >v2;
                for(unsigned int iSelStep = 0 ;iSelStep<SelectionSteps.size();iSelStep++){
                        vector<THStack*> v1;
                        for(unsigned int iChannel = 0;iChannel<Channels.size();iChannel++){
                                THStack* hS = new THStack();
                                bool first = true;
                                for(unsigned int iDataset = 0;iDataset<Datasets.size();iDataset++){
                                        if(!Datasets[iDataset].isData()){
                                                TH1F* h = (TH1F*) Histos[varNumber][iChannel][iSelStep][iDataset].Clone();
                                                h->SetFillColor(Datasets[iDataset].Color());
                                                if(first){ 
                                                        hS->Add(h);
                                                        hS->SetName(h->GetName()+TString("_hs"));
                                                        first = false;
                                                }
                                                else hS->Add(h);
                                                h = 0;
                                                delete h;
                                        }
                                }
                                v1.push_back(hS);
                                hS = 0;
                                delete hS;
                        }
                        v2.push_back(v1);
                }
                MCStack.push_back(v2);
        }       


}


void HistoManager::MCDataComparison(){

}


pair<int,int> HistoManager::CanvasDivide(int NofPad){
        if(NofPad <= 2 ) return pair<int,int>(NofPad,0);
        int max = 8;
        for(int i=2;i<max+1;i++){
                if(NofPad<=(i*i)) return pair<int,int>(i,i);
                if(NofPad<=(i*(i+1))) return pair<int,int>(i+1,i);
        }
        cerr<<"The number of pad is greater than "<<max*max<<endl;
        return pair<int,int>(max,max);
}

void HistoManager::PlotsCutByCut(){
        for(unsigned int varNumber = 0 ;varNumber<Histos.size();varNumber++){
                vector<vector<TCanvas*> >v2;
                vector<TCanvas*> vdeb;
                for(unsigned int iChannel = 0 ;iChannel<Channels.size();iChannel++){
                        vector<TCanvas*> v1;
                        string name = "c";
                        if(SelectionSteps.size()>0 && Datasets.size()>0) name = Histos[varNumber][iChannel][0][0].GetTitle();
                        TCanvas* cdeb = new TCanvas();
                        cdeb->SetTitle(name.c_str());
                        pair<int,int> div = CanvasDivide(Datasets.size());
                        if(div.second!=0) cdeb->Divide(div.first, div.second);
                        else cdeb->Divide(div.first);
                        for(unsigned int iDataset = 0;iDataset<Datasets.size();iDataset++){
                                TCanvas* c = new TCanvas();
                                c->SetTitle(name.c_str());
                                TLegend* l = new TLegend(0.7,0.7,0.9,0.9);
                                for(unsigned int iSelStep = 0;iSelStep<SelectionSteps.size();iSelStep++){
                                        TH1F* tmp = (TH1F*) Histos[varNumber][iChannel][iSelStep][iDataset].Clone();
                                        tmp->SetLineColor(SelStepColor[iSelStep]);
                                        c->cd();
                                        if(iSelStep==0) tmp->Draw();
                                        else tmp->Draw("same");
                                        cdeb->cd(iDataset+1);
                                        if(iSelStep==0) tmp->Draw();
                                        else tmp->Draw("same");
                                        l->AddEntry(tmp,SelectionSteps[iSelStep].c_str(),"l");
                                        //if decommented: information not stored ...
                                        //tmp = 0 ;
                                        //delete tmp;
                                }
                                c->cd();
                                l->Draw();
                                cdeb->cd(iDataset+1);
                                l->Draw();
                                v1.push_back(c);
                                c = 0;
                                delete c;
                                l = 0;
                                delete l;
                        }
                        v2.push_back(v1);
                        vdeb.push_back(cdeb);
                        cdeb = 0;
                        delete cdeb;
                }
                CutEffectCanvas.push_back(v2);
                CutEffectCanvasDebug.push_back(vdeb);
        }

} 

void HistoManager::PlotsCutByCut2D(){
        // per Variable
        for(unsigned int varNumber = 0 ;varNumber<Histos2D.size();varNumber++){
                vector<vector<vector<TCanvas*> > >v3;
                vector<vector<TCanvas*> > vdeb;
                // per Channel
                for(unsigned int iChannel = 0 ;iChannel<Channels.size();iChannel++){
                   vector<vector<TCanvas*> >v2;
                   vector<TCanvas*> vdeb1;
                   // per Selection
                   for(unsigned int iSelStep = 0;iSelStep<SelectionSteps.size();iSelStep++){
                        vector<TCanvas*> v1;
                        string name = "c";
                        if(SelectionSteps.size()>0 && Datasets.size()>0) name = Histos2D[varNumber][iChannel][iSelStep][0].GetTitle();
                        TCanvas* cdeb = new TCanvas();
                        cdeb->SetTitle(name.c_str());
                        pair<int,int> div = CanvasDivide(Datasets.size());
                        if(div.second!=0) cdeb->Divide(div.first, div.second);
                        else cdeb->Divide(div.first);
                        // per DataSet
                        for(unsigned int iDataset = 0;iDataset<Datasets.size();iDataset++){
                                TCanvas* c = new TCanvas();
                                c->SetTitle(name.c_str());
                                TH2D* tmp = (TH2D*) Histos2D[varNumber][iChannel][iSelStep][iDataset].Clone();
                                c->cd();
                                tmp->Draw("colz");
                                cdeb->cd(iDataset+1);
                                tmp->Draw("colz");
                                //if decommented: information not stored ...
                                //tmp = 0 ;
                                //delete tmp;
                                v1.push_back(c);
                                c = 0;
                                delete c;
                        }
                        v2.push_back(v1);
                        vdeb1.push_back(cdeb);
                        cdeb = 0;
                        delete cdeb;
                   }
                   v3.push_back(v2);
                   vdeb.push_back(vdeb1);
                }
                CutEffectCanvas2D.push_back(v3);
                CutEffectCanvasDebug2D.push_back(vdeb);
        }

} 


void HistoManager::ProduceDebugCanvas(){

} 


void HistoManager::Write(TDirectory* dir){
        //Temporary variable;
        TDirectory* dir1 = dir;
        TDirectory* dir2 = dir;
        TDirectory* dir3 = dir;
        for(unsigned int varNumber =0; varNumber < Histos.size();varNumber++){
                dir->cd();
                for(unsigned int iChannel = 0;iChannel<Channels.size();iChannel++){
                        if(varNumber == 0 ) dir1 = dir->mkdir(Channels[iChannel].c_str());
                        else dir1 = dir->GetDirectory(Channels[iChannel].c_str());
                        //dir1->pwd();
                        //And cut by cut histos 
                        bool check1=Check(CutEffectCanvasDebug,0,0);
                        bool check2=Check(CutEffectCanvas,0,0,0);
                        if (check1 && check2) {
                         if(varNumber == 0) dir2 = dir1->mkdir("CutByCut");
                         else dir2 = dir1->GetDirectory("CutByCut");
                         dir2->cd();
                         if (check1) CutEffectCanvasDebug[varNumber][iChannel]->Write();
                         if (check2) {
                          for(unsigned int iDataset =0;iDataset<Datasets.size();iDataset++){
                                if(varNumber == 0 ) dir3 = dir2->mkdir(Datasets[iDataset].Name().c_str());
                                else dir3 = dir2->GetDirectory(Datasets[iDataset].Name().c_str());
                                dir3->cd();
                                CutEffectCanvas[varNumber][iChannel][iDataset]->Write();
                          }
                         } 
                        }
                        for(unsigned int iSelStep=0;iSelStep<SelectionSteps.size();iSelStep++){
                                if(varNumber == 0) dir2 = dir1->mkdir(SelectionSteps[iSelStep].c_str());
                                else dir2 = dir1->GetDirectory(SelectionSteps[iSelStep].c_str());
                                //dir2->pwd();
                                for(unsigned int iDataset =0;iDataset<Datasets.size();iDataset++){
                                        if(varNumber == 0 ) dir3 = dir2->mkdir(Datasets[iDataset].Name().c_str());
                                        else dir3 = dir2->GetDirectory(Datasets[iDataset].Name().c_str());
                                        dir3->cd();
                                        Check(iChannel,iSelStep,iDataset,varNumber);
                                        Histos[varNumber][iChannel][iSelStep][iDataset].Write();
                                }
                                dir2->cd();
                                //Stack & SumHisto
                                if(varNumber == 0 ) dir3 = dir2->mkdir("SumOverDatasets");
                                else dir3 = dir2->GetDirectory("SumOverDatasets");
                                dir3->cd();
                                MCStack[varNumber][iSelStep][iChannel];
                                if(MCStack.size()>0 && MCStack[varNumber][iSelStep][iChannel]) MCStack[varNumber][iSelStep][iChannel]->Write();
                                if(SumMCDatasetsHistos.size()>0) SumMCDatasetsHistos[varNumber][iSelStep][iChannel].Write();
                                SumDataDatasetsHistos[varNumber][iSelStep][iChannel].Write();

                        }
                }
        }
        dir1 = 0;
        dir2 = 0;
        dir3 = 0;
        delete dir1;
        delete dir2;
        delete dir3;
}

void HistoManager::Write2D(TDirectory* dir){
        //Temporary variable;
        TDirectory* dir1 = dir;
        TDirectory* dir2 = dir;
        TDirectory* dir3 = dir;
        TDirectory* dir4 = dir;
        for(unsigned int varNumber =0; varNumber < Histos2D.size();varNumber++){
                dir->cd();
                for(unsigned int iChannel = 0;iChannel<Channels.size();iChannel++){
                        if(varNumber == 0 ) dir1 = dir->mkdir(Channels[iChannel].c_str());
                        else dir1 = dir->GetDirectory(Channels[iChannel].c_str());
                        //dir1->pwd();
                        //And cut by cut histos 
                        bool check1=Check(CutEffectCanvasDebug2D,0,0,0);
                        bool check2=Check(CutEffectCanvas2D,0,0,0,0);
                        if (check1 && check2) {
                         if(varNumber == 0) dir2 = dir1->mkdir("CutByCut");
                         else dir2 = dir1->GetDirectory("CutByCut");
                         dir2->cd();
                         for(unsigned int iSelStep = 0;iSelStep<SelectionSteps.size();iSelStep++){
                           if(varNumber == 0 ) dir3 = dir2->mkdir(SelectionSteps[iSelStep].c_str());
                           else dir3 = dir2->GetDirectory(SelectionSteps[iSelStep].c_str());
                           dir3->cd();
                           if (check1) CutEffectCanvasDebug2D[varNumber][iChannel][iSelStep]->Write();
                           if (check2) {
                            for(unsigned int iDataset =0;iDataset<Datasets.size();iDataset++){
                                if(varNumber == 0 ) dir4 = dir3->mkdir(Datasets[iDataset].Name().c_str());
                                else dir4 = dir3->GetDirectory(Datasets[iDataset].Name().c_str());
                                dir4->cd();
                                CutEffectCanvas2D[varNumber][iChannel][iSelStep][iDataset]->Write();
                            }
                           }   
                         }
                        }
                        for(unsigned int iSelStep=0;iSelStep<SelectionSteps.size();iSelStep++){
                                if(varNumber == 0) dir2 = dir1->mkdir(SelectionSteps[iSelStep].c_str());
                                else dir2 = dir1->GetDirectory(SelectionSteps[iSelStep].c_str());
                                //dir2->pwd();
                                for(unsigned int iDataset =0;iDataset<Datasets.size();iDataset++){
                                        if(varNumber == 0 ) dir3 = dir2->mkdir(Datasets[iDataset].Name().c_str());
                                        else dir3 = dir2->GetDirectory(Datasets[iDataset].Name().c_str());
                                        dir3->cd();
                                        Check2D(iChannel,iSelStep,iDataset,varNumber);
                                        Histos2D[varNumber][iChannel][iSelStep][iDataset].Write();
                                }
                                dir2->cd();
                                //Stack & SumHisto
                                if(varNumber == 0 ) dir3 = dir2->mkdir("SumOverDatasets");
                                else dir3 = dir2->GetDirectory("SumOverDatasets");
                                dir3->cd();
//                              MCStack[varNumber][iSelStep][iChannel];
//                              if(MCStack.size()>0 && MCStack[varNumber][iSelStep][iChannel]) MCStack[varNumber][iSelStep][iChannel]->Write();
                                if(SumMCDatasetsHistos2D.size()>0) SumMCDatasetsHistos2D[varNumber][iSelStep][iChannel].Write();
                                SumDataDatasetsHistos2D[varNumber][iSelStep][iChannel].Write();

                        }
                }
        }
        dir1 = 0;
        dir2 = 0;
        dir3 = 0;
        dir4 = 0;
        delete dir1;
        delete dir2;
        delete dir3;
        delete dir4;
}


//modifdg
void HistoManager::SimpleWrite(){
        for(unsigned int varNumber =0; varNumber < Histos.size();varNumber++){
           std::cout<<"Histos.size() "<<Histos.size()<<" "<<varNumber <<std::endl;
                for(unsigned int iChannel = 0;iChannel<Channels.size();iChannel++){
        std::cout<<"Channels.size() "<<Channels.size() <<" "<<Channels[iChannel].c_str()<<std::endl;
                        for(unsigned int iDataset =0;iDataset<Datasets.size();iDataset++){
                            for(unsigned int iSelStep=0;iSelStep<SelectionSteps.size();iSelStep++){
                                        Histos[varNumber][iChannel][iSelStep][iDataset].Write();
                            }           
                        }
                }
        }
}

void HistoManager::SimpleWrite2D(){
        for(unsigned int varNumber =0; varNumber < Histos2D.size();varNumber++){
           std::cout<<"Histos2D.size() "<<Histos2D.size()<<" "<<varNumber <<std::endl;
                for(unsigned int iChannel = 0;iChannel<Channels.size();iChannel++){
        std::cout<<"Channels.size() "<<Channels.size() <<" "<<Channels[iChannel].c_str()<<std::endl;
                        for(unsigned int iDataset =0;iDataset<Datasets.size();iDataset++){
                            for(unsigned int iSelStep=0;iSelStep<SelectionSteps.size();iSelStep++){
                                        Histos2D[varNumber][iChannel][iSelStep][iDataset].Write();
                            }           
                        }
                }
        }
}

void HistoManager::CreateHisto(vector<TH1F>& hlist, string  v, string datasetname, string XTitle,  string YTitle,int bin, float min, float max){

    if ( datasetname=="TTbar" ) {
       TH1F hsig ((v+"_"+datasetname+"Sig").c_str(),v.c_str(),bin,min,max);
       TH1F hbkg ((v+"_"+datasetname+"Bkg").c_str(),v.c_str(),bin,min,max);
       hsig.GetXaxis()->SetTitle(XTitle.c_str()); hsig.GetYaxis()->SetTitle(YTitle.c_str());
       hbkg.GetXaxis()->SetTitle(XTitle.c_str()); hbkg.GetYaxis()->SetTitle(YTitle.c_str());
//       hsig.Sumw2(); hbkg.Sumw2();
       hlist.push_back(hsig);hlist.push_back(hbkg);
    } else {
       TH1F h ((v+"_"+datasetname).c_str(),v.c_str(),bin,min,max);
       h.GetXaxis()->SetTitle(XTitle.c_str()); h.GetYaxis()->SetTitle(YTitle.c_str());
       if ( datasetname=="Data" || datasetname=="DATA" || datasetname=="DataMu" || datasetname=="DataEG" ||
       datasetname=="DataMuEG" ||  datasetname=="DataEGMu") h.Sumw2();
//       h.Sumw2();
       hlist.push_back(h);
    } 
     
}

void HistoManager::CreateHisto2D(vector<TH2D>& hlist, string  v, string datasetname, string XTitle,int bin, float min, float max, string YTitle, int ybin, float ymin, float ymax){
        CreateHisto2D(hlist,  v, datasetname, XTitle,  YTitle,bin, min, max, ybin, ymin, ymax);
}
void HistoManager::CreateHisto2D(vector<TH2D>& hlist, string  v, string datasetname, string XTitle,  string YTitle,int bin, float min, float max, int ybin, float ymin, float ymax){

    if ( datasetname=="TTbar" ) {
       TH2D hsig ((v+"_"+datasetname+"Sig").c_str(),v.c_str(),bin,min,max,ybin,ymin,ymax);
       TH2D hbkg ((v+"_"+datasetname+"Bkg").c_str(),v.c_str(),bin,min,max,ybin,ymin,ymax);
       hsig.GetXaxis()->SetTitle(XTitle.c_str()); hsig.GetYaxis()->SetTitle(YTitle.c_str());
       hbkg.GetXaxis()->SetTitle(XTitle.c_str()); hbkg.GetYaxis()->SetTitle(YTitle.c_str());
       hsig.Sumw2(); hbkg.Sumw2();
       hlist.push_back(hsig);hlist.push_back(hbkg);
    } else {
       TH2D h ((v+"_"+datasetname).c_str(),v.c_str(),bin,min,max,ybin,ymin,ymax);
       h.GetXaxis()->SetTitle(XTitle.c_str()); h.GetYaxis()->SetTitle(YTitle.c_str());
       h.Sumw2();
       hlist.push_back(h);
    } 
     
}


void HistoManager::CreateHisto3D(vector<TH3D>& hlist, string  v, string datasetname, string XTitle,int bin, float min, float max, string YTitle, int ybin, float ymin, float ymax, string ZTitle, int zbin, float zmin, float zmax){
        CreateHisto3D(hlist,  v, datasetname, XTitle,  YTitle, ZTitle ,bin, min, max, ybin, ymin, ymax, zbin, zmin, zmax);
}
void HistoManager::CreateHisto3D(vector<TH3D>& hlist, string  v, string datasetname, string XTitle,  string YTitle,  string ZTitle, int bin, float min, float max, int ybin, float ymin, float ymax, int zbin, float zmin, float zmax){

    if ( datasetname=="TTbar" ) {
       TH3D hsig ((v+"_"+datasetname+"Sig").c_str(),v.c_str(),bin,min,max,ybin,ymin,ymax,zbin,zmin,zmax);
       TH3D hbkg ((v+"_"+datasetname+"Bkg").c_str(),v.c_str(),bin,min,max,ybin,ymin,ymax,zbin,zmin,zmax);
       hsig.GetXaxis()->SetTitle(XTitle.c_str()); hsig.GetYaxis()->SetTitle(YTitle.c_str()); hsig.GetZaxis()->SetTitle(ZTitle.c_str());
       hbkg.GetXaxis()->SetTitle(XTitle.c_str()); hbkg.GetYaxis()->SetTitle(YTitle.c_str()); hbkg.GetZaxis()->SetTitle(ZTitle.c_str());
       hsig.Sumw2(); hbkg.Sumw2();
       hlist.push_back(hsig);hlist.push_back(hbkg);
    } else {
       TH3D h ((v+"_"+datasetname).c_str(),v.c_str(),bin,min,max,ybin,ymin,ymax,zbin,zmin,zmax);
       h.GetXaxis()->SetTitle(XTitle.c_str()); h.GetYaxis()->SetTitle(YTitle.c_str());h.GetYaxis()->SetTitle(ZTitle.c_str());
       h.Sumw2();
       hlist.push_back(h);
    } 
     
}









void HistoManager::CreateHisto(vector<TH1F>& hlist, string  v, string datasetname, string XTitle,  string YTitle, int bin, float * range){

    if ( datasetname=="TTbar" ) {
       TH1F hsig ((v+"_"+datasetname+"Sig").c_str(),v.c_str(),bin, range);
       TH1F hbkg ((v+"_"+datasetname+"Bkg").c_str(),v.c_str(),bin, range);
       hsig.GetXaxis()->SetTitle(XTitle.c_str()); hsig.GetYaxis()->SetTitle(YTitle.c_str());
       hbkg.GetXaxis()->SetTitle(XTitle.c_str()); hbkg.GetYaxis()->SetTitle(YTitle.c_str());
       hsig.Sumw2(); hbkg.Sumw2();
       hlist.push_back(hsig);hlist.push_back(hbkg);
    } else {
       TH1F h ((v+"_"+datasetname).c_str(),v.c_str(),bin, range);
       h.GetXaxis()->SetTitle(XTitle.c_str()); h.GetYaxis()->SetTitle(YTitle.c_str());
//       if ( datasetname=="Data" || datasetname=="DATA" || datasetname=="DataMu" || datasetname=="DataEG" ) h.Sumw2();
       h.Sumw2();
       hlist.push_back(h);
    } 
     
}

void HistoManager::CreateHisto2D(vector<TH2D>& hlist, string  v, string datasetname, string XTitle,  string YTitle, int bin, float * range, int ybin, float * yrange){

    if ( datasetname=="TTbar" ) {
       TH2D hsig ((v+"_"+datasetname+"Sig").c_str(),v.c_str(),bin, range,ybin,yrange);
       TH2D hbkg ((v+"_"+datasetname+"Bkg").c_str(),v.c_str(),bin, range,ybin,yrange);
       hsig.GetXaxis()->SetTitle(XTitle.c_str()); hsig.GetYaxis()->SetTitle(YTitle.c_str());
       hbkg.GetXaxis()->SetTitle(XTitle.c_str()); hbkg.GetYaxis()->SetTitle(YTitle.c_str());
       hsig.Sumw2(); hbkg.Sumw2();
       hlist.push_back(hsig);hlist.push_back(hbkg);
    } else {
       TH2D h ((v+"_"+datasetname).c_str(),v.c_str(),bin, range,ybin,yrange);
       h.GetXaxis()->SetTitle(XTitle.c_str()); h.GetYaxis()->SetTitle(YTitle.c_str());
       h.Sumw2();
       hlist.push_back(h);
    } 
     
}

void HistoManager::FillHisto(vector<TH1F>& hlist, const char* v, float vvalue, string datasetname,  bool issignal, float w){
        FillHisto(hlist,string(v),vvalue,datasetname, issignal,w);
}

void HistoManager::FillHisto(vector<TH1F>& hlist, string v, float vvalue, string datasetname,  bool issignal, float w){

  for (unsigned int i=0; i < hlist.size(); i++){
  
     if ( datasetname=="TTbar" ) {
        if ( issignal ) {
            if ( hlist[i].GetName() == (v+"_"+datasetname+"Sig") )  hlist[i].Fill(vvalue,w);
        }else{
            if ( hlist[i].GetName() == (v+"_"+datasetname+"Bkg") )  hlist[i].Fill(vvalue,w); 
        }
     }else {
           if ( hlist[i].GetName() == (v+"_"+datasetname) )  hlist[i].Fill(vvalue,w);
     
     }
  }   

}  

void HistoManager::DeleteHisto(vector<TH1F*> hlist){

  for (unsigned int i=0; i < hlist.size(); i++){
  
     delete hlist[i];
  }  
} 



void HistoManager::DeleteHisto2D(vector<TH2D*> hlist){

  for (unsigned int i=0; i < hlist.size(); i++){
  
     delete hlist[i];
  }  
} 


void HistoManager::FillHisto2D(vector<TH2D>& hlist, const char* v, float vvalue1, float vvalue2,  string datasetname,  bool issignal, float w){
        FillHisto2D(hlist,string(v),vvalue1,vvalue2,datasetname,issignal,w);
}

void HistoManager::FillHisto2D(vector<TH2D>& hlist, string v, float vvalue1, float vvalue2, string datasetname,  bool issignal, float w){

  for (unsigned int i=0; i < hlist.size(); i++){
  
     if ( datasetname=="TTbar" ) {
        if ( issignal ) {
            if ( hlist[i].GetName() == (v+"_"+datasetname+"Sig") )  hlist[i].Fill(vvalue1, vvalue2, w);
        }else{
            if ( hlist[i].GetName() == (v+"_"+datasetname+"Bkg") )  hlist[i].Fill(vvalue1, vvalue2, w); 
        }
     }else {
           if ( hlist[i].GetName() == (v+"_"+datasetname) )  hlist[i].Fill(vvalue1, vvalue2, w);
     
     }
  }   

}  



void HistoManager::FillHisto3D(vector<TH3D>& hlist, const char* v, float vvalue1, float vvalue2, float vvalue3,  string datasetname,  bool issignal, float w){
        FillHisto3D(hlist,string(v),vvalue1,vvalue2,vvalue3,datasetname,issignal,w);
}

void HistoManager::FillHisto3D(vector<TH3D>& hlist, string v, float vvalue1, float vvalue2, float vvalue3, string datasetname,  bool issignal, float w){

  for (unsigned int i=0; i < hlist.size(); i++){
  
     if ( datasetname=="TTbar" ) {
        if ( issignal ) {
            if ( hlist[i].GetName() == (v+"_"+datasetname+"Sig") )  hlist[i].Fill(vvalue1, vvalue2, vvalue3, w);
        }else{
            if ( hlist[i].GetName() == (v+"_"+datasetname+"Bkg") )  hlist[i].Fill(vvalue1, vvalue2, vvalue3, w); 
        }
     }else {
           if ( hlist[i].GetName() == (v+"_"+datasetname) )  hlist[i].Fill(vvalue1, vvalue2, vvalue3, w);
     
     }
  }   

}  
void HistoManager::WriteMyHisto(vector<TH1F>& hlist, string t){

  if ( t=="all" ) {
  std::cout<<"je fill "<<hlist.size()<<std::endl;
     for (unsigned int i=0; i < hlist.size(); i++){
        hlist[i].Write();
     }
  }

}

void HistoManager::WriteMyHisto2D(vector<TH2D>& hlist, string t){

  if ( t=="all" ) {
  std::cout<<"je fill "<<hlist.size()<<std::endl;
     for (unsigned int i=0; i < hlist.size(); i++){
        hlist[i].Write();
     }
  }

}

void HistoManager::WriteMyHisto3D(vector<TH3D>& hlist, string t){

  if ( t=="all" ) {
  std::cout<<"je fill "<<hlist.size()<<std::endl;
     for (unsigned int i=0; i < hlist.size(); i++){
        hlist[i].Write();
     }
  }

}

void HistoManager::MergeHistos(vector<TH1F>& hlist, string h1, string h2){

// // h1-> h1+h2
     int i1 = -1 ;
     int j2 = -1 ;
     
     
     for (unsigned int i=0; i < hlist.size(); i++){
        string d1 = (string)hlist[i].GetName();
        string::size_type loc1 = d1.find(h1,0);
        if ( loc1 != string::npos ) i1 = i;
        for (unsigned int j=0; j < hlist.size(); j++){
            string d2 = (string)hlist[j].GetName();
            string::size_type loc2 = d2.find(h2,0);
            if ( loc2 != string::npos ) j2 = j;
        }
     }
     if ( i1!=-1 && j2 !=-1 ) {
        hlist[i1].Add(&hlist[i1],&hlist[j2],1.,1.);
     }

}
void HistoManager::MergeHistos2D(vector<TH2D>& hlist, string h1, string h2){

// // h1-> h1+h2
     int i1 = -1 ;
     int j2 = -1 ;
     
     
     for (unsigned int i=0; i < hlist.size(); i++){
        string d1 = (string)hlist[i].GetName();
        string::size_type loc1 = d1.find(h1,0);
        if ( loc1 != string::npos ) i1 = i;
        for (unsigned int j=0; j < hlist.size(); j++){
            string d2 = (string)hlist[j].GetName();
            string::size_type loc2 = d2.find(h2,0);
            if ( loc2 != string::npos ) j2 = j;
        }
     }
     if ( i1!=-1 && j2 !=-1 ) {
        hlist[i1].Add(&hlist[i1],&hlist[j2],1.,1.);
     }

}

  
void HistoManager::StackHisto(THStack* hs, vector<TH1F>& hlist, string v, vector<string>ordre){

  hs->SetName(("HStack_"+v).c_str());
  for (unsigned int i=0; i < ordre.size(); i++)
    {
//       std::cout<<"+ordre[i] "<<ordre[i]<<std::endl;
      for (unsigned int j=0; j < hlist.size(); j++)
       {
//       std::cout<<"hlist[j].GetName() "<< hlist[j].GetName()<<std::endl;
          if ( hlist[j].GetName()==(v+"_"+ordre[i]) ) {  
             if  ( ordre[i] == "TTbarSig" ) hlist[j].SetFillColor(kRed+1);
             if  ( ordre[i] == "TTbarBkg" ) hlist[j].SetFillColor(kRed-7);
             if  ( ordre[i] == "Zjets" )    hlist[j].SetFillColor(4);
             if  ( ordre[i] == "Wjets" )    hlist[j].SetFillColor(kGreen-3);
             if  ( ordre[i] == "SingleToptW" )    hlist[j].SetFillColor(kMagenta);
             if  ( ordre[i] == "VV" )       hlist[j].SetFillColor(5);
             hs->Add(&hlist[j]); 
          }
       }  
    }

}  
//endmodifdg

/*
        vector<vector<vector<pair<TH1F> > > Histos;
        vector<string> Channels;
        vector<string> SelectionSteps;
        vector<Dataset*> Datasets;

        vector<vector<TH1F*> > SumMCDatasetsHistos; 
        vector<vector<THStack*> > MCStack;          
        vector<vector<TCanvas* > > MCDataCompCanvas;        
        vector<<TCanvas*>  MCDataCompCanvasDebug;           
        vector<vector<TCanvas* > > CutEffectCanvas;         
        vector<<TCanvas*> CutEffectCanvasDebug;             

*/

void HistoManager::SetCutFlowAxisTitle(vector<TH1F>& hlist, string v, string datasetname){

  for (unsigned int i=0; i < hlist.size(); i++){
  
     hlist[i].GetXaxis()->SetLabelSize(0.07);
     hlist[i].GetXaxis()->SetBinLabel(1, "no sel.");
     hlist[i].GetXaxis()->SetBinLabel(2, "trig.");
     hlist[i].GetXaxis()->SetBinLabel(3, "dilept");
     hlist[i].GetXaxis()->SetBinLabel(4, "inv M.");
     hlist[i].GetXaxis()->SetBinLabel(5, "Jet");
     hlist[i].GetXaxis()->SetBinLabel(6, "met");
     hlist[i].GetXaxis()->SetBinLabel(7, "btag1");
     hlist[i].GetXaxis()->SetBinLabel(8, "btag2");
    
     
     
  }   

}  
