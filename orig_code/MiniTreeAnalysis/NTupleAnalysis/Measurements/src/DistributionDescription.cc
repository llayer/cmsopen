#include "../interface/DistributionDescription.h"

        
DistributionDescription::DistributionDescription(const string& name, const string& channel, const int& dim, const vector<bool>& datasetIsDD, string interpMeth){
        Name = name;
        Channel = channel;
        NofDimensions = dim;
        DatasetIsDD = datasetIsDD;
        InterpolMethod = interpMeth;
        //DimName = 0;
        initialized = false;
        Nobs_1D = 0;
        Nobs_2D = 0;
        Nobs_3D = 0;
        NobsPE_1D = 0;
        NobsPE_2D = 0;
        NobsPE_3D = 0;
        NofExpectedEvtsSumInit_1D = 0; 
        NofExpectedEvtsSumInit_2D = 0; 
        NofExpectedEvtsSumInit_3D = 0;
        NofExpectedEvtsSumFinal_1D = 0; 
        NofExpectedEvtsSumFinal_2D = 0; 
        NofExpectedEvtsSumFinal_3D = 0;
}
        
DistributionDescription::DistributionDescription(const DistributionDescription& dd){
        Name = dd.Name;
        Channel = dd.Channel;
        NofDimensions = dd.NofDimensions;
        DatasetIsDD = dd.DatasetIsDD;
        //
        //DimName = new string[NofDimensions];
        //if(NofDimensions>=0)
        //      for(int i=0;i<NofDimensions;i++) DimName[i] = dd.DimName[i];
        //
        initialized = dd.initialized;
        if(dd.Nobs_1D) Nobs_1D = (TH1F*) (dd.Nobs_1D->Clone());
        else Nobs_1D = 0;
        if(dd.Nobs_2D) Nobs_2D = (TH2F*) (dd.Nobs_2D->Clone());
        else Nobs_2D = 0;
        if(dd.Nobs_3D) Nobs_3D = (TH3F*) (dd.Nobs_3D->Clone());
        else Nobs_3D = 0;
        if(dd.NobsPE_1D) NobsPE_1D = (TH1F*) (dd.NobsPE_1D->Clone());
        else NobsPE_1D = 0;
        if(dd.NobsPE_2D) NobsPE_2D = (TH2F*) (dd.NobsPE_2D->Clone());
        else NobsPE_2D = 0;
        if(dd.NobsPE_3D) NobsPE_3D = (TH3F*) (dd.NobsPE_3D->Clone());
        else NobsPE_3D = 0;
        Histo_1D = dd.Histo_1D;         
        Histo_2D = dd.Histo_2D;         
        Histo_3D = dd.Histo_3D; 
        Histo_1D_UncertMinus = dd.Histo_1D_UncertMinus;
        Histo_1D_UncertPlus = dd.Histo_1D_UncertPlus;           
        Histo_2D_UncertMinus = dd.Histo_2D_UncertMinus;         
        Histo_2D_UncertPlus = dd.Histo_2D_UncertPlus;           
        Histo_3D_UncertMinus = dd.Histo_3D_UncertMinus;                 
        Histo_3D_UncertPlus = dd.Histo_3D_UncertPlus;   
        Graph_Uncert = dd.Graph_Uncert; 
        Efficiencies = dd.Efficiencies; 
        uncertainties = dd.uncertainties;
        NofExpectedEvtsInit_1D = dd.NofExpectedEvtsInit_1D; 
        NofExpectedEvtsInit_2D = dd.NofExpectedEvtsInit_2D; 
        NofExpectedEvtsInit_3D = dd.NofExpectedEvtsInit_3D;
        if(dd.NofExpectedEvtsSumInit_1D) NofExpectedEvtsSumInit_1D = (TH1F*) (dd.NofExpectedEvtsSumInit_1D->Clone()); 
        else NofExpectedEvtsSumInit_1D = 0;
        if(dd.NofExpectedEvtsSumInit_2D) NofExpectedEvtsSumInit_2D = (TH2F*) (dd.NofExpectedEvtsSumInit_2D->Clone()); 
        else NofExpectedEvtsSumInit_2D = 0;
        if(dd.NofExpectedEvtsSumInit_3D) NofExpectedEvtsSumInit_3D = (TH3F*) (dd.NofExpectedEvtsSumInit_3D->Clone());
        else NofExpectedEvtsSumInit_3D = 0;
        NofExpectedEvtsFinal_1D = dd.NofExpectedEvtsFinal_1D; 
        NofExpectedEvtsFinal_2D = dd.NofExpectedEvtsFinal_2D; 
        NofExpectedEvtsFinal_3D = dd.NofExpectedEvtsFinal_3D;
        if(dd.NofExpectedEvtsSumFinal_1D) NofExpectedEvtsSumFinal_1D = (TH1F*) (dd.NofExpectedEvtsSumFinal_1D->Clone()); 
        else NofExpectedEvtsSumFinal_1D = 0;
        if(dd.NofExpectedEvtsSumFinal_2D) NofExpectedEvtsSumFinal_2D = (TH2F*) (dd.NofExpectedEvtsSumFinal_2D->Clone()); 
        else NofExpectedEvtsSumFinal_2D = 0;
        if(dd.NofExpectedEvtsSumFinal_3D) NofExpectedEvtsSumFinal_3D = (TH3F*) (dd.NofExpectedEvtsSumFinal_3D->Clone());
        else NofExpectedEvtsSumFinal_3D = 0;
        InterpolMethod = dd.InterpolMethod;
}
        
DistributionDescription::~DistributionDescription(){
        //cout<<"Call of the destructor"<<endl;
        //cout<<"address: "<<Nobs_1D<<endl;
        //delete[] DimName;     
        delete Nobs_1D;
        delete Nobs_2D;
        delete Nobs_3D;
        delete NobsPE_1D;
        delete NobsPE_2D;
        delete NobsPE_3D;
        delete NofExpectedEvtsSumInit_1D; 
        delete NofExpectedEvtsSumInit_2D; 
        delete NofExpectedEvtsSumInit_3D;
        delete NofExpectedEvtsSumFinal_1D; 
        delete NofExpectedEvtsSumFinal_2D; 
        delete NofExpectedEvtsSumFinal_3D;
}
        
void DistributionDescription:: SetNofDimensions(const int& dim){
        NofDimensions = dim;
}
        
void DistributionDescription::SetName(const string& name){
        Name = name;
}
        
string DistributionDescription::GetName()const{
        return Name;
}
        
void DistributionDescription::SetChannel(const string& channel){
        Channel = channel;
}       

string DistributionDescription::GetChannel()const{
        return Channel;
}
        
int DistributionDescription::GetNofDimensions()const {return NofDimensions;}
        
TH1F* DistributionDescription::Get1DNobs() const{
        return Nobs_1D;
}
        
TH2F* DistributionDescription::Get2DNobs() const {return Nobs_2D;}
        
TH3F* DistributionDescription::Get3DNobs() const {return Nobs_3D;}
        
        
void DistributionDescription:: LoadHistos(TFile* fin, const vector<Dataset>& datasets, const vector<Parameter>& uncert, const vector<Parameter>& GraphUncert, const vector<Parameter>& estim, const vector<Parameter>& GraphEstim){
        LoadHistos(fin, datasets);
        if(uncert.size()>0)
                LoadHistosUncertainties(fin, uncert, datasets, true);
        if(GraphUncert.size()>0)
                LoadGraphUncertainties(fin, GraphUncert, datasets, true);
        if(estim.size()>0)
                LoadHistosUncertainties(fin, estim, datasets, false);
        if(GraphEstim.size()>0)
                LoadGraphUncertainties(fin, GraphEstim, datasets, false);
        if(NofDimensions==1 && DatasetIsDD.size()!=Histo_1D.size()) cerr<<"DistributionDescription:: LoadHistos::  Problem in the size of the collection DatasetIsDD: "<<DatasetIsDD.size()<<" Histo_1D: "<<Histo_1D.size()<<endl;
        if(NofDimensions==2 && DatasetIsDD.size()!=Histo_2D.size()) cerr<<"DistributionDescription:: LoadHistos::  Problem in the size of the collection DatasetIsDD: "<<DatasetIsDD.size()<<" Histo_2D: "<<Histo_2D.size()<<endl;
        if(NofDimensions==3 && DatasetIsDD.size()!=Histo_3D.size()) cerr<<"DistributionDescription:: LoadHistos::  Problem in the size of the collection DatasetIsDD: "<<DatasetIsDD.size()<<" Histo_3D: "<<Histo_3D.size()<<endl;
}

void DistributionDescription:: LoadHistos(TFile* fin, const vector<Dataset>& datasets){
        for(unsigned int d=0;d<datasets.size();d++){
                //string histoname = Channel+"_"+Name+"_nominal_"+datasets[d].Name();
                string histoname = Channel+"_"+Name+"_"+datasets[d].Name();
                cout<<"Loading histo "<<histoname<<endl;
                if(fin->Get(histoname.c_str())!=0){
                        bool loaded = false;    
                        if(NofDimensions==1 && fin->Get(histoname.c_str())->InheritsFrom("TH1")){
                                if(datasets[d].isData()){
                                         Nobs_1D = (TH1F*) fin->Get(histoname.c_str())->Clone("");
                                         Nobs_1D->SetDirectory(0);
                                         NobsPE_1D = (TH1F*) fin->Get(histoname.c_str())->Clone("");
                                         NobsPE_1D->SetDirectory(0);
                                }
                                else{ 
                                        Histo_1D.push_back((TH1F*) fin->Get(histoname.c_str())->Clone(""));
                                        Histo_1D.back()->SetDirectory(0);
                                        NofExpectedEvtsInit_1D.push_back((TH1F*) fin->Get(histoname.c_str())->Clone(""));
                                        NofExpectedEvtsInit_1D.back()->SetDirectory(0);
                                        NofExpectedEvtsFinal_1D.push_back((TH1F*) fin->Get(histoname.c_str())->Clone(""));
                                        NofExpectedEvtsFinal_1D.back()->SetDirectory(0);
                                }
                                loaded = true;
                        } 
                        if(NofDimensions==2 && fin->Get(histoname.c_str())->InheritsFrom("TH2")){
                                if(datasets[d].isData()){
                                         Nobs_2D = (TH2F*) fin->Get(histoname.c_str())->Clone("");
                                         Nobs_2D->SetDirectory(0);
                                         NobsPE_2D = (TH2F*) fin->Get(histoname.c_str())->Clone("");
                                         NobsPE_2D->SetDirectory(0);
                                }
                                else{
                                        cout<<"pb"<<endl; 
                                        Histo_2D.push_back((TH2F*) fin->Get(histoname.c_str())->Clone(""));
                                        Histo_2D.back()->SetDirectory(0);
                                        NofExpectedEvtsInit_2D.push_back((TH2F*) fin->Get(histoname.c_str())->Clone(""));
                                        NofExpectedEvtsInit_2D.back()->SetDirectory(0);
                                        NofExpectedEvtsFinal_2D.push_back((TH2F*) fin->Get(histoname.c_str())->Clone(""));
                                        NofExpectedEvtsFinal_2D.back()->SetDirectory(0);
                                }
                                loaded = true;
                        } 
                        if(NofDimensions==3 && fin->Get(histoname.c_str())->InheritsFrom("TH3")){
                                if(datasets[d].isData()){
                                         Nobs_3D = (TH3F*) fin->Get(histoname.c_str())->Clone("");
                                         Nobs_3D->SetDirectory(0);
                                         NobsPE_3D = (TH3F*) fin->Get(histoname.c_str())->Clone("");
                                         NobsPE_2D->SetDirectory(0);
                                }
                                else{ 
                                        Histo_3D.push_back((TH3F*) fin->Get(histoname.c_str())->Clone(""));
                                        Histo_3D.back()->SetDirectory(0);
                                        NofExpectedEvtsInit_3D.push_back((TH3F*) fin->Get(histoname.c_str())->Clone(""));
                                        NofExpectedEvtsInit_3D.back()->SetDirectory(0);
                                        NofExpectedEvtsFinal_3D.push_back((TH3F*) fin->Get(histoname.c_str())->Clone(""));
                                        NofExpectedEvtsFinal_3D.back()->SetDirectory(0);
                                }
                                loaded = true;
                        } 
                        if(!loaded) cerr<<"DistributionDescription:: histo "<<histoname<<" - dimension is incorrect "<<endl;
                }
                else cerr<<"DistributionDescription:: histo "<<histoname<<" not found in the file "<<fin->GetName()<<endl;
        }
        //Compute the sum
        //1D
        if(Histo_1D.size()>0){
                NofExpectedEvtsSumInit_1D = (TH1F*) Histo_1D[0]->Clone();
                NofExpectedEvtsSumCurrent_1D = (TH1F*) Histo_1D[0]->Clone();
                NofExpectedEvtsSumFinal_1D = (TH1F*) Histo_1D[0]->Clone();
                NofExpectedEvtsSumInit_1D->SetDirectory(0);
                NofExpectedEvtsSumCurrent_1D->SetDirectory(0);
                NofExpectedEvtsSumFinal_1D->SetDirectory(0);
                for(int ibin=0;ibin<Histo_1D[0]->GetNbinsX()+1;ibin++){
                        float sum = 0;
                        for(unsigned int i=0;i<Histo_1D.size();i++){
                                sum+=Histo_1D[i]->GetBinContent(ibin);
                        }
                        NofExpectedEvtsSumInit_1D->SetBinContent(ibin,sum);
                        NofExpectedEvtsSumCurrent_1D->SetBinContent(ibin,sum);
                        NofExpectedEvtsSumFinal_1D->SetBinContent(ibin,sum);
                }
        }
        //2D
        if(Histo_2D.size()>0){
                NofExpectedEvtsSumInit_2D = (TH2F*) Histo_2D[0]->Clone();
                NofExpectedEvtsSumCurrent_2D = (TH2F*) Histo_2D[0]->Clone();
                NofExpectedEvtsSumFinal_2D = (TH2F*) Histo_2D[0]->Clone();
                NofExpectedEvtsSumInit_2D->SetDirectory(0);
                NofExpectedEvtsSumCurrent_2D->SetDirectory(0);
                NofExpectedEvtsSumFinal_2D->SetDirectory(0);
                for(int ibin=0;ibin<Histo_2D[0]->GetNbinsX()+1;ibin++){
                        float sum = 0;
                        for(unsigned int i=0;i<Histo_2D.size();i++){
                                sum+=Histo_2D[i]->GetBinContent(ibin);
                        }
                        NofExpectedEvtsSumInit_2D->SetBinContent(ibin,sum);
                        NofExpectedEvtsSumCurrent_2D->SetBinContent(ibin,sum);
                        NofExpectedEvtsSumFinal_2D->SetBinContent(ibin,sum);
                }
        }
        //3D
        if(Histo_3D.size()>0){
                NofExpectedEvtsSumInit_3D = (TH3F*) Histo_3D[0]->Clone();
                NofExpectedEvtsSumCurrent_3D = (TH3F*) Histo_3D[0]->Clone();
                NofExpectedEvtsSumFinal_3D = (TH3F*) Histo_3D[0]->Clone();
                NofExpectedEvtsSumInit_3D->SetDirectory(0);
                NofExpectedEvtsSumCurrent_3D->SetDirectory(0);
                NofExpectedEvtsSumFinal_3D->SetDirectory(0);
                for(int ibin=0;ibin<Histo_3D[0]->GetNbinsX()+1;ibin++){
                        float sum = 0;
                        for(unsigned int i=0;i<Histo_3D.size();i++){
                                sum+=Histo_3D[i]->GetBinContent(ibin);
                        }
                        NofExpectedEvtsSumInit_3D->SetBinContent(ibin,sum);
                        NofExpectedEvtsSumCurrent_3D->SetBinContent(ibin,sum);
                        NofExpectedEvtsSumFinal_3D->SetBinContent(ibin,sum);
                }
        }
}

void DistributionDescription:: LoadGraphUncertainties(TFile* fin, const vector<Parameter>& uncert, const vector<Dataset>& datasets, bool ParAreUncertainties){
        bool verbose = true;
        //Check that Nobs exist && dimension
        bool OK = false;
        if(Nobs_1D && NofDimensions==1 ) OK = true;     
        if(Nobs_2D && NofDimensions==2 ) OK = true;     
        if(Nobs_3D && NofDimensions==3 ) OK = true;
        if(!OK){
                cerr<<"DistributionDescription:: LoadGraphUncertainties:: The data histo file needs to be loaded first"<<endl;  
                return;
        }
        for(unsigned int i=0;i<uncert.size();i++){
          if(verbose) cout<<"Loading histo for uncertainty: "<<uncert[i].name<<endl;
                if(NofDimensions==2){
                        vector<vector<TGraphErrors*> > vGraphDatasets;
                        for(unsigned int d=0;d<datasets.size();d++){
                                if(verbose) cout<<" - Loading histo dataset: "<<datasets[d].Name()<<endl;
                                if(datasets[d].isData()) continue;
                                vector<TGraphErrors*> vGraph;
                                for(int x=1;x<Nobs_2D->GetNbinsX()+1;x++){
                                        for(int y=1;y<Nobs_2D->GetNbinsY()+1;y++){
                                                //take care::it's integer !!
                                                int xvalue = Nobs_2D->GetXaxis()->GetBinCenter(x);
                                                int yvalue = Nobs_2D->GetYaxis()->GetBinCenter(y);
                                                char graphName[200];
                                                //sprintf(graphName,"%s_%d_%d_%s",Channel.c_str(),xvalue,yvalue,datasets[d].Name().c_str());
                                                //sprintf(graphName,"%s_%s_%d_%d_%s",Channel.c_str(),uncert[i].name.c_str(),yvalue,xvalue,datasets[d].Name().c_str());
                                                //sprintf(graphName,"%s_%s_%d_%d_%s",Channel.c_str(),uncert[i].name.c_str(),xvalue,yvalue,datasets[d].Name().c_str());
                                                sprintf(graphName,"%s_%s_%s_%d_%d_%s",Channel.c_str(),Name.c_str(),uncert[i].name.c_str(),xvalue,yvalue,datasets[d].Name().c_str());
                                                cout<<"name: "<<graphName<<endl;
                                                //attention aux problems d'indice ...
                                                if(fin->Get(graphName))
                                                        vGraph.push_back((TGraphErrors*) fin->Get(graphName)->Clone(""));
                                                else{   
                                                        cerr<<"DistributionDescription:: LoadGraphUncertainties:: Graph "<<graphName<<" not found !"<<endl;
                                                        vGraph.push_back(0);
                                                }
                                        }
                                }
                                if(vGraph.size()>0) vGraphDatasets.push_back(vGraph);
                        }
                        if(vGraphDatasets.size()>0){
                         if(ParAreUncertainties) Graph_Uncert.push_back(vGraphDatasets);
                         else Graph_Estim.push_back(vGraphDatasets);
                        }
                }
        }
}
                
        
void DistributionDescription:: LoadHistosUncertainties(TFile* fin, const vector<Parameter>& uncert, const vector<Dataset>& datasets, bool ParAreUncertainties){
        bool verbose = true;
        for(unsigned int i=0;i<uncert.size();i++){
          if(verbose) cout<<"Loading histo for uncertainty: "<<uncert[i].name<<endl;
          vector<TH1F*> vhisto1DMinus;
          vector<TH1F*> vhisto1DPlus;
          vector<TH2F*> vhisto2DMinus;
          vector<TH2F*> vhisto2DPlus;
          vector<TH3F*> vhisto3DMinus;
          vector<TH3F*> vhisto3DPlus;
          int it_id = 0;
          for(unsigned int d=0;d<datasets.size();d++){
                if(verbose) cout<<" - Loading histo dataset: "<<datasets[d].Name()<<endl;
                if(datasets[d].isData()) continue;
                string histonameMinus = Channel+"_"+Name+"_"+uncert[i].name+"-minus"+"_"+datasets[d].Name();
                string histonamePlus = Channel+"_"+Name+"_"+uncert[i].name+"-plus"+"_"+datasets[d].Name();
                //to be changed: add the possibility to have more value that just minus/plus

                //Plot Minus
                if(fin->Get(histonameMinus.c_str())!=0){
                        bool loaded = false;    
                        if(NofDimensions==1 && fin->Get(histonameMinus.c_str())->InheritsFrom("TH1")){
                                vhisto1DMinus.push_back((TH1F*) fin->Get(histonameMinus.c_str())->Clone(""));
                                vhisto1DMinus.back()->SetDirectory(0);
                                loaded = true;
                        } 
                        if(NofDimensions==2 && fin->Get(histonameMinus.c_str())->InheritsFrom("TH2")){
                                vhisto2DMinus.push_back((TH2F*) fin->Get(histonameMinus.c_str())->Clone(""));
                                vhisto2DMinus.back()->SetDirectory(0);
                                loaded = true;
                        } 
                        if(NofDimensions==3 && fin->Get(histonameMinus.c_str())->InheritsFrom("TH3")){
                                vhisto3DMinus.push_back((TH3F*) fin->Get(histonameMinus.c_str())->Clone(""));
                                vhisto3DMinus.back()->SetDirectory(0);
                                loaded = true;
                        }        
                        if(!loaded) cerr<<"DistributionDescription:: histo "<<histonameMinus<<" - dimension is incorrect "<<endl;
                }
                else{
                        cerr<<"DistributionDescription:: histo "<<histonameMinus<<" not found in the file "<<fin->GetName()<<endl;
                        cerr<<" It will be replace by the nominal histo"<<endl;
                        if(NofDimensions==1){
                                vhisto1DMinus.push_back((TH1F*) NofExpectedEvtsInit_1D[it_id]->Clone(""));
                                vhisto1DMinus.back()->SetDirectory(0);
                        }
                        if(NofDimensions==2){ 
                                vhisto2DMinus.push_back((TH2F*) NofExpectedEvtsInit_2D[it_id]->Clone(""));
                                vhisto2DMinus.back()->SetDirectory(0);
                        }
                        if(NofDimensions==3){
                                vhisto3DMinus.push_back((TH3F*) NofExpectedEvtsInit_3D[it_id]->Clone(""));
                                vhisto3DMinus.back()->SetDirectory(0);
                        }
                }

                //Plot Plus
                if(fin->Get(histonamePlus.c_str())!=0){
                        bool loaded = false;    
                        if(NofDimensions==1 && fin->Get(histonamePlus.c_str())->InheritsFrom("TH1")){
                                vhisto1DPlus.push_back((TH1F*) fin->Get(histonamePlus.c_str())->Clone(""));
                                vhisto1DPlus.back()->SetDirectory(0);
                                loaded = true;
                        } 
                        if(NofDimensions==2 && fin->Get(histonamePlus.c_str())->InheritsFrom("TH2")){
                                vhisto2DPlus.push_back((TH2F*) fin->Get(histonamePlus.c_str())->Clone(""));
                                vhisto2DPlus.back()->SetDirectory(0);
                                loaded = true;
                        } 
                        if(NofDimensions==3 && fin->Get(histonamePlus.c_str())->InheritsFrom("TH3")){
                                vhisto3DPlus.push_back((TH3F*) fin->Get(histonamePlus.c_str())->Clone(""));
                                vhisto3DPlus.back()->SetDirectory(0);
                                loaded = true;
                        }        
                        if(!loaded) cerr<<"DistributionDescription:: histo "<<histonamePlus<<" - dimension is incorrect "<<endl;
                }
                else{
                        cerr<<"DistributionDescription:: histo "<<histonamePlus<<" not found in the file "<<fin->GetName()<<endl;
                        cerr<<" It will be replace by the nominal histo"<<endl;
                        if(NofDimensions==1){
                                vhisto1DPlus.push_back((TH1F*) NofExpectedEvtsInit_1D[it_id]->Clone(""));
                                vhisto1DPlus.back()->SetDirectory(0);
                        }
                        if(NofDimensions==2){ 
                                vhisto2DPlus.push_back((TH2F*) NofExpectedEvtsInit_2D[it_id]->Clone(""));
                                vhisto2DPlus.back()->SetDirectory(0);
                        }
                        if(NofDimensions==3){
                                vhisto3DPlus.push_back((TH3F*) NofExpectedEvtsInit_3D[it_id]->Clone(""));
                                vhisto3DPlus.back()->SetDirectory(0);
                        }
                }
                it_id++;
          }
          if(ParAreUncertainties){
                 if(vhisto1DMinus.size()>0) Histo_1D_UncertMinus.push_back(vhisto1DMinus);
                 if(vhisto1DPlus.size()>0) Histo_1D_UncertPlus.push_back(vhisto1DPlus);
                 if(vhisto2DMinus.size()>0) Histo_2D_UncertMinus.push_back(vhisto2DMinus);
                 if(vhisto2DPlus.size()>0) Histo_2D_UncertPlus.push_back(vhisto2DPlus);
                 if(vhisto3DMinus.size()>0) Histo_3D_UncertMinus.push_back(vhisto3DMinus);
                 if(vhisto3DPlus.size()>0) Histo_3D_UncertPlus.push_back(vhisto3DPlus);
         }
         else{
                 if(vhisto1DMinus.size()>0) Histo_1D_EstimMinus.push_back(vhisto1DMinus);
                 if(vhisto1DPlus.size()>0) Histo_1D_EstimPlus.push_back(vhisto1DPlus);
                 if(vhisto2DMinus.size()>0) Histo_2D_EstimMinus.push_back(vhisto2DMinus);
                 if(vhisto2DPlus.size()>0) Histo_2D_EstimPlus.push_back(vhisto2DPlus);
                 if(vhisto3DMinus.size()>0) Histo_3D_EstimMinus.push_back(vhisto3DMinus);
                 if(vhisto3DPlus.size()>0) Histo_3D_EstimPlus.push_back(vhisto3DPlus);
         }
        }
        
}
        
void DistributionDescription:: LoadUncertainties(TFile* fin, const string& dirname, const vector<AllDatasetUncertainties>& uncert){
}
        
void DistributionDescription:: LoadEfficiencies(const int& idataset, float eff){
}
        
bool DistributionDescription::ClosureTestOnSumEfficiencies(){
        return false;
}
        
bool DistributionDescription::HistoConsistency(){
        return false;
}

TH1F* DistributionDescription::Get1DPseudoData() const{
        return NobsPE_1D;
}

TH2F* DistributionDescription::Get2DPseudoData() const{
        return NobsPE_2D;
}

float DistributionDescription::GetFactorFromFitFunctionOfTheGraph(const TGraphErrors* graph, float xValue, string MethodName, float YThreshold) const{
        float NewFactor = 1.;
        bool error = false;
        //cout<<"XValue: "<<xValue<<endl;
        if(graph->GetListOfFunctions()->GetSize()>=1){
                for(int i=0;i<graph->GetListOfFunctions()->GetSize();i++){
                        error = false;
                        //NewFactor=((TF1*) graph->GetListOfFunctions()->At(i-1))->Eval(xValue);
                        NewFactor=((TF1*) graph->GetListOfFunctions()->At(i))->Eval(xValue);
                        char message[200];
                        sprintf(message,"(x=%f,%f) - GraphName: %s",xValue,NewFactor,graph->GetName());
                        if(NewFactor==0){
                                 cout<<"DistributionDescription::"<<MethodName<<":: factor from the fit function should not be null - "<<message<<endl;
                                 NewFactor = 0;
                                 error = true;
                        } 
                        if(NewFactor<0){
                                 cout<<"DistributionDescription::"<<MethodName<<":: factor from the fit function should not be negative - "<<message<<endl;
                                 NewFactor = 0;
                                 error = true;
                        } 
                        if(NewFactor>YThreshold){ 
                                cout<<"DistributionDescription::"<<MethodName<<":: factor from the fit function is high - "<<message<<endl;
                                error = true;
                        }
                        if(TMath::IsNaN(NewFactor)){
                                cout<<"DistributionDescription::"<<MethodName<<":: factor frome the fit function is NAN  - "<<message<<endl;
                                error = true;
                        }
                        if(!error) return NewFactor;
                }
        }
        cout<<"DistributionDescription::"<<MethodName<<":: there are not fit functions associated to the graph "<<graph->GetName()<<endl;
        return 1.; //It's the safer return ...
}

void DistributionDescription::CheckGraph(){
        for(unsigned int i=0;i<Graph_Uncert.size();i++){
                
        }
}


void DistributionDescription::Compute1DFinalDistrib(const vector<double>& parUnc, const vector<double>& GraphUnc, const vector<double>& bkgNorm, const double& LumiFactor, const double& SignalFactor, const vector<int>& iSignalDatasets){
        if(parUnc.size()!=Histo_1D_UncertMinus.size() || parUnc.size()!=Histo_1D_UncertPlus.size()){
                cerr<<"DistributionDescription::Compute1DFinalDistrib:: inconsistency in the size of the collection "<<endl;
                cerr<<"\t parUnc.size = "<<parUnc.size()<<" Histo_1D_UncertMinus.size = "<<Histo_1D_UncertMinus.size()<<" Histo_1D_UncertPlus.size = "<<Histo_1D_UncertPlus.size()<<endl;
        }
        if(GraphUnc.size()!=Graph_Uncert.size()){
                cerr<<"DistributionDescription::Compute1DFinalDistrib:: inconsistency in the size of the collection "<<endl;
                cerr<<"\t GraphUnc.size = "<<GraphUnc.size()<<" Graph_Uncert.size = "<<Graph_Uncert.size()<<endl;
        }
        if((bkgNorm.size()+iSignalDatasets.size())!=(NofExpectedEvtsInit_1D.size())){
                cerr<<"DistributionDescription::Compute1DFinalDistrib:: inconsistency in the size of the collection "<<endl;
                cerr<<"\t bkgNorm.size = "<<bkgNorm.size()<<" iSignalDatasets.size= "<<iSignalDatasets.size()<<" NofExpectedEvtsInit_1D.size = "<<NofExpectedEvtsInit_1D.size()<<endl;
        }
        for(int ibin=0;ibin<NofExpectedEvtsSumCurrent_1D->GetNbinsX()+1;ibin++){
                int it=0;
                for(unsigned int d=0;d<NofExpectedEvtsInit_1D.size();d++){
                        if(NofExpectedEvtsInit_1D[d]->GetBinContent(ibin)==0) continue;
                        float factor = 1.;
                        for(unsigned id=0;id<iSignalDatasets.size();id++){
                                if((int)d == iSignalDatasets[id]) factor = SignalFactor;
                                else{
                                        if(bkgNorm[it]>0) factor = bkgNorm[it]/NofExpectedEvtsInit_1D[d]->Integral();
                                        it++;
                                }
                        }
                        float UncertaintyCorr = 1.;//0
                        for(unsigned int i=0;i<parUnc.size();i++){
                                UncertaintyCorr*=Interpol(parUnc[i], 1., Histo_1D_UncertMinus[i][d]->GetBinContent(ibin)/NofExpectedEvtsInit_1D[d]->GetBinContent(ibin), Histo_1D_UncertPlus[i][d]->GetBinContent(ibin)/NofExpectedEvtsInit_1D[d]->GetBinContent(ibin));
                                /*
                                if(parUnc[i]<0)
                                        UncertaintyCorr*= 1+(((NofExpectedEvtsInit_1D[d]->GetBinContent(ibin)-Histo_1D_UncertMinus[i][d]->GetBinContent(ibin))*parUnc[i])/NofExpectedEvtsInit_1D[d]->GetBinContent(ibin));
                                else
                                        UncertaintyCorr*= 1+(((Histo_1D_UncertPlus[i][d]->GetBinContent(ibin)-NofExpectedEvtsInit_1D[d]->GetBinContent(ibin))*parUnc[i])/NofExpectedEvtsInit_1D[d]->GetBinContent(ibin));
                                */      
                        }
                        int graphIndice = 0;
                        for(unsigned int i=0;i<GraphUnc.size();i++){
                                if(Graph_Uncert[i][d][graphIndice]){
                                        //cout<<Graph_Uncert[i][d][graphIndice]->GetFunction("pol1")<<endl;
                                        //cout<<Graph_Uncert[i][d][graphIndice]->GetFunction("pol2")<<endl;
                                        //if(Graph_Uncert[i][d][graphIndice]->GetFunction("pol1")) UncertaintyCorr+=(Graph_Uncert[i][d][graphIndice]->GetFunction("pol1")->Eval(GraphUnc[i])-1)*NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin);
                                        //if(Graph_Uncert[i][d][graphIndice]->GetFunction("pol2")) UncertaintyCorr+=(Graph_Uncert[i][d][graphIndice]->GetFunction("pol2")->Eval(GraphUnc[i])-1)*NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin);
                                        //if(Graph_Uncert[i][d][graphIndice]->GetFunction("pol1")) UncertaintyCorr*=(Graph_Uncert[i][d][graphIndice]->GetFunction("pol1")->Eval(GraphUnc[i]));
                                        //if(Graph_Uncert[i][d][graphIndice]->GetFunction("pol2")) UncertaintyCorr*=(Graph_Uncert[i][d][graphIndice]->GetFunction("pol2")->Eval(GraphUnc[i]));
                                        float NewFactor = GetFactorFromFitFunctionOfTheGraph(Graph_Uncert[i][d][graphIndice], GraphUnc[i], string("Compute1DFinalDistrib"));
                                        UncertaintyCorr*=NewFactor;
                                        /*
                                        if(Graph_Uncert[i][d][graphIndice]->GetListOfFunctions()->GetSize()>=1){
                                                float NewFactor=((TF1*) Graph_Uncert[i][d][graphIndice]->GetListOfFunctions()->At(0))->Eval(GraphUnc[i]);
                                                if(NewFactor<=0) cout<<"DistributionDescription::Compute2DFinalDistrib:: factor for the fit function should not be negative ; factor = "<<NewFactor<<endl; 
                                                if(NewFactor>2.) cout<<"DistributionDescription::Compute2DFinalDistrib:: factor for the fit function is high; factor = "<<NewFactor<<endl;
                                                if(TMath::IsNaN(NewFactor)) cout<<"DistributionDescription::Compute2DFinalDistrib:: the factor computed for input = "<<GraphUnc[i]<<" with the graph "<<Graph_Uncert[i][d][graphIndice]->GetName()<<" is NaN !!"<<endl;
                                                cout<<"NewFactor: "<<NewFactor<<" ("<<GraphUnc[i]<<")"<<endl; 
                                                UncertaintyCorr*=NewFactor;
                                        }
                                        else cerr<<"DistributionDescription::Compute2DFinalDistrib:: no fit function found in the graphic "<<Graph_Uncert[i][d][graphIndice]->GetName()<<endl;
                                        */
                                }
                        }
                        //cout<<"factor = "<<UncertaintyCorr<<endl;
                        //cout<<"int: "<<NofExpectedEvtsInit_1D[d]->Integral()<<endl;
                        //update also NofExpectedEvtsFinal_1D
                        if(!DatasetIsDD[d]) NofExpectedEvtsFinal_1D[d]->SetBinContent(ibin, LumiFactor*factor*UncertaintyCorr*NofExpectedEvtsInit_1D[d]->GetBinContent(ibin));
                        else NofExpectedEvtsFinal_1D[d]->SetBinContent(ibin, factor*UncertaintyCorr*NofExpectedEvtsInit_1D[d]->GetBinContent(ibin));
                        //do not rescale to luminosity if the dataset is Data Driven
                        //cout<<"int2: "<<NofExpectedEvtsFinal_1D[d]->Integral()<<endl;
                }
        }
}

TH1F* DistributionDescription::Compute1DPseudoData(TRandom& rand, const vector<double>& parUnc, const vector<double>& GraphUnc, const vector<double>& bkgNorm, const double& LumiFactor, const double& SignalFactor, const vector<int>& iSignalDatasets) const{
        if(parUnc.size()!=Histo_1D_UncertMinus.size() || parUnc.size()!=Histo_1D_UncertPlus.size()){
                cerr<<"DistributionDescription::Compute1DPseudoData:: inconsistency in the size of the collection "<<endl;
                cerr<<"\t parUnc.size = "<<parUnc.size()<<" Histo_1D_UncertMinus.size = "<<Histo_1D_UncertMinus.size()<<" Histo_1D_UncertPlus.size = "<<Histo_1D_UncertPlus.size()<<endl;
        }
        if(GraphUnc.size()!=Graph_Uncert.size()){
                cerr<<"DistributionDescription::Compute1DPseudoData:: inconsistency in the size of the collection "<<endl;
                cerr<<"\t GraphUnc.size = "<<GraphUnc.size()<<" Graph_Uncert.size = "<<Graph_Uncert.size()<<endl;
        }
        if((bkgNorm.size()+iSignalDatasets.size())!=(NofExpectedEvtsInit_1D.size())){
                cerr<<"DistributionDescription::Compute1DPseudoData:: inconsistency in the size of the collection "<<endl;
                cerr<<"\t bkgNorm.size = "<<bkgNorm.size()<<" iSignalDatasets.size= "<<iSignalDatasets.size()<<" NofExpectedEvtsInit_1D.size = "<<NofExpectedEvtsInit_1D.size()<<endl;
        }
        for(int ibin=0;ibin<NofExpectedEvtsSumCurrent_1D->GetNbinsX()+1;ibin++){
                float value = 0;
                int it=0;
                for(unsigned int d=0;d<NofExpectedEvtsInit_1D.size();d++){
                        if(NofExpectedEvtsInit_1D[d]->GetBinContent(ibin)==0) continue;
                        float factor = 1.;
                        for(unsigned id=0;id<iSignalDatasets.size();id++){
                                if((int)d == iSignalDatasets[id]) factor = SignalFactor;
                                else{
                                        if(bkgNorm[it]>0) factor = bkgNorm[it]/NofExpectedEvtsInit_1D[d]->Integral();
                                        it++;
                                }
                        }
                        float UncertaintyCorr = 1.;//0
                        for(unsigned int i=0;i<parUnc.size();i++){
                                Interpol(0.,1.,-1.,1.);
                                Interpol(parUnc[i], NofExpectedEvtsInit_1D[d]->GetBinContent(ibin), Histo_1D_UncertMinus[i][d]->GetBinContent(ibin), Histo_1D_UncertPlus[i][d]->GetBinContent(ibin));
                                UncertaintyCorr*=Interpol(parUnc[i], 1., Histo_1D_UncertMinus[i][d]->GetBinContent(ibin)/NofExpectedEvtsInit_1D[d]->GetBinContent(ibin), Histo_1D_UncertPlus[i][d]->GetBinContent(ibin)/NofExpectedEvtsInit_1D[d]->GetBinContent(ibin));
                                /*
                                if(parUnc[i]<0)
                                        UncertaintyCorr*= 1+(((NofExpectedEvtsInit_1D[d]->GetBinContent(ibin)-Histo_1D_UncertMinus[i][d]->GetBinContent(ibin))*parUnc[i])/NofExpectedEvtsInit_1D[d]->GetBinContent(ibin));
                                else
                                        UncertaintyCorr*= 1+(((Histo_1D_UncertPlus[i][d]->GetBinContent(ibin)-NofExpectedEvtsInit_1D[d]->GetBinContent(ibin))*parUnc[i])/NofExpectedEvtsInit_1D[d]->GetBinContent(ibin));
                                */      
                                cout<<"parUnc "<<parUnc[i]<<endl;
                                cout<<"Fact: "<<UncertaintyCorr<<endl;
                        }
                        //update also NofExpectedEvtsFinal_1D
                        if(!DatasetIsDD[d]){
                                 value+=LumiFactor*factor*UncertaintyCorr*NofExpectedEvtsInit_1D[d]->GetBinContent(ibin);
                                 NofExpectedEvtsFinal_1D[d]->SetBinContent(ibin, LumiFactor*factor*UncertaintyCorr*NofExpectedEvtsInit_1D[d]->GetBinContent(ibin));
                        //do not rescale to luminosity if the dataset is Data Driven
                        }
                        else{
                                 value+=factor*UncertaintyCorr*NofExpectedEvtsInit_1D[d]->GetBinContent(ibin);
                                 NofExpectedEvtsFinal_1D[d]->SetBinContent(ibin, factor*UncertaintyCorr*NofExpectedEvtsInit_1D[d]->GetBinContent(ibin));
                        }
                }
                //randomization with a Poisson distribution - returns a integer as expected for data
                value = (float) rand.Poisson(value);
                NobsPE_1D->SetBinContent(ibin,value);
        }
        return NobsPE_1D;
}
TH1F* DistributionDescription::Get1DNexp(const vector<double>& parUnc, const vector<double>& GraphUnc, const vector<double>& bkgNorm, const double& LumiFactor, const double& SignalFactor, const vector<int>& iSignalDatasets) const{
        if(parUnc.size()!=Histo_1D_UncertMinus.size() || parUnc.size()!=Histo_1D_UncertPlus.size()){
                cerr<<"DistributionDescription::Get1DNexp:: inconsistency in the size of the collection "<<endl;
                cerr<<"\t parUnc.size = "<<parUnc.size()<<" Histo_1D_UncertMinus.size = "<<Histo_1D_UncertMinus.size()<<" Histo_1D_UncertPlus.size = "<<Histo_1D_UncertPlus.size()<<endl;
        }
        if(GraphUnc.size()!=Graph_Uncert.size()){
                cerr<<"DistributionDescription::Get1DNexp:: inconsistency in the size of the collection "<<endl;
                cerr<<"\t GraphUnc.size = "<<GraphUnc.size()<<" Graph_Uncert.size = "<<Graph_Uncert.size()<<endl;
        }
        if((bkgNorm.size()+iSignalDatasets.size())!=(NofExpectedEvtsInit_1D.size())){
                cerr<<"DistributionDescription::Get1DNexp:: inconsistency in the size of the collection "<<endl;
                cerr<<"\t bkgNorm.size = "<<bkgNorm.size()<<" iSignalDatasets.size= "<<iSignalDatasets.size()<<" NofExpectedEvtsInit_1D.size = "<<NofExpectedEvtsInit_1D.size()<<endl;
        }
        for(int ibin=0;ibin<NofExpectedEvtsSumCurrent_1D->GetNbinsX()+1;ibin++){
                float value = 0;
                int it=0;
                for(unsigned int d=0;d<NofExpectedEvtsInit_1D.size();d++){
                        if(NofExpectedEvtsInit_1D[d]->GetBinContent(ibin)==0) continue;
                        float factor = 1.;
                        //cout<<"iSignalDatasets "<<iSignalDatasets.size()<<" "<<iSignalDatasets[0]<<endl;
                        for(unsigned id=0;id<iSignalDatasets.size();id++){
                                if((int)d == iSignalDatasets[id]) factor = SignalFactor;
                                else{
                                        if(bkgNorm[it]>0) factor = bkgNorm[it]/NofExpectedEvtsInit_1D[d]->Integral();
                                        //cout<<"factor = "<<bkgNorm[it]<<"/"<<NofExpectedEvtsInit_1D[d]->Integral()<<endl;
                                        it++;
                                }
                        }
                        float UncertaintyCorr = 1.;//0
                        for(unsigned int i=0;i<parUnc.size();i++){
                                UncertaintyCorr*=Interpol(parUnc[i], 1., Histo_1D_UncertMinus[i][d]->GetBinContent(ibin)/NofExpectedEvtsInit_1D[d]->GetBinContent(ibin), Histo_1D_UncertPlus[i][d]->GetBinContent(ibin)/NofExpectedEvtsInit_1D[d]->GetBinContent(ibin));
                                /*
                                if(parUnc[i]<0)
                                        UncertaintyCorr+= (NofExpectedEvtsInit_1D[d]->GetBinContent(ibin)-Histo_1D_UncertMinus[i][d]->GetBinContent(ibin))*parUnc[i];
                                else
                                        UncertaintyCorr+= (NofExpectedEvtsInit_1D[d]->GetBinContent(ibin)-Histo_1D_UncertPlus[i][d]->GetBinContent(ibin))*parUnc[i];
                                */
                                /*
                                if(parUnc[i]<0)
                                        UncertaintyCorr*= 1+(((NofExpectedEvtsInit_1D[d]->GetBinContent(ibin)-Histo_1D_UncertMinus[i][d]->GetBinContent(ibin))*parUnc[i])/NofExpectedEvtsInit_1D[d]->GetBinContent(ibin));
                                else
                                        UncertaintyCorr*= 1+(((Histo_1D_UncertPlus[i][d]->GetBinContent(ibin)-NofExpectedEvtsInit_1D[d]->GetBinContent(ibin))*parUnc[i])/NofExpectedEvtsInit_1D[d]->GetBinContent(ibin));
                                */              
                        //UncertaintyCorr*= 1+(((-Histo_1D_UncertPlus[i][d]->GetBinContent(ibin)+NofExpectedEvtsInit_1D[d]->GetBinContent(ibin))*parUnc[i])/NofExpectedEvtsInit_1D[d]->GetBinContent(ibin));
                        //cout<<Histo_1D_UncertMinus[i][d]->GetBinContent(ibin)<<" "<<NofExpectedEvtsInit_1D[d]->GetBinContent(ibin)<<" "<<Histo_1D_UncertPlus[i][d]->GetBinContent(ibin)<<endl;
                        //cout<<"factor = "<<UncertaintyCorr<<" "<<parUnc[i]<<endl;
                                        
                        }
                        int graphIndice = 0;
                        for(unsigned int i=0;i<GraphUnc.size();i++){
                                if(Graph_Uncert[i][d][graphIndice]){
                                        //if(Graph_Uncert[i][d][graphIndice]->GetFunction("pol1")) UncertaintyCorr+=(Graph_Uncert[i][d][graphIndice]->GetFunction("pol1")->Eval(GraphUnc[i])-1)*NofExpectedEvtsInit_1D[d]->GetBinContent(ibin);
                                        //if(Graph_Uncert[i][d][graphIndice]->GetFunction("pol2")) UncertaintyCorr+=(Graph_Uncert[i][d][graphIndice]->GetFunction("pol2")->Eval(GraphUnc[i])-1)*NofExpectedEvtsInit_1D[d]->GetBinContent(ibin);
                                        //if(Graph_Uncert[i][d][graphIndice]->GetFunction("pol1")) UncertaintyCorr*=(Graph_Uncert[i][d][graphIndice]->GetFunction("pol1")->Eval(GraphUnc[i]));
                                        //if(Graph_Uncert[i][d][graphIndice]->GetFunction("pol2")) UncertaintyCorr*=(Graph_Uncert[i][d][graphIndice]->GetFunction("pol2")->Eval(GraphUnc[i]));
                                        /*
                                        if(Graph_Uncert[i][d][graphIndice]->GetListOfFunctions()->GetSize()>=1){
                                                float NewFactor=((TF1*) Graph_Uncert[i][d][graphIndice]->GetListOfFunctions()->At(0))->Eval(GraphUnc[i]);
                                                if(NewFactor<=0) cout<<"DistributionDescription::Get1DNexp:: factor for the fit function should not be negative ; factor = "<<NewFactor<<endl; 
                                                if(NewFactor>2.) cout<<"DistributionDescription::Get1DNexp:: factor for the fit function is high; factor = "<<NewFactor<<endl;
                                                cout<<"NewFactor: "<<NewFactor<<" ("<<GraphUnc[i]<<")"<<endl; 
                                                UncertaintyCorr*=NewFactor;
                                        }
                                        else cerr<<"DistributionDescription::Get1DNexp:: no fit function found in the graphic "<<Graph_Uncert[i][d][graphIndice]->GetName()<<endl;
                                        */
                                        float NewFactor = GetFactorFromFitFunctionOfTheGraph(Graph_Uncert[i][d][graphIndice], GraphUnc[i], string("Get1DNexp"));
                                        UncertaintyCorr*=NewFactor;
                                }
                        }
                        /*
                        for(unsigned int i=0;i<GraphEstim.size();i++){
                                if(Graph_Estim[i][d][graphIndice]){
                                        if(Graph_Estim[i][d][graphIndice]->GetFunction("pol1")) UncertaintyCorr+=(Graph_Estim[i][d][graphIndice]->GetFunction("pol1")->Eval(GraphEstim[i])-1)*NofExpectedEvtsInit_1D[d]->GetBinContent(ibin);
                                        if(Graph_Estim[i][d][graphIndice]->GetFunction("pol2")) UncertaintyCorr+=(Graph_Estim[i][d][graphIndice]->GetFunction("pol2")->Eval(GraphEstim[i])-1)*NofExpectedEvtsInit_1D[d]->GetBinContent(ibin);
                                }
                        }
                        */
                        //update also NofExpectedEvtsFinal_1D
                        //NofExpectedEvtsFinal_1D[d]->SetBinContent(ibin, factor*(UncertaintyCorr+NofExpectedEvtsInit_1D[d]->GetBinContent(ibin)));
                        //value+=factor*(UncertaintyCorr+NofExpectedEvtsInit_1D[d]->GetBinContent(ibin));
                        if(!DatasetIsDD[d]){
                                NofExpectedEvtsFinal_1D[d]->SetBinContent(ibin, LumiFactor*factor*UncertaintyCorr*NofExpectedEvtsInit_1D[d]->GetBinContent(ibin));
                                value+=LumiFactor*factor*UncertaintyCorr*NofExpectedEvtsInit_1D[d]->GetBinContent(ibin);
                                //cout<<"VALUE: "<<LumiFactor*factor*UncertaintyCorr*NofExpectedEvtsInit_1D[d]->GetBinContent(ibin)<<" = "<<LumiFactor<<"*"<<factor<<"*"<<UncertaintyCorr<<"*"<<NofExpectedEvtsInit_1D[d]->GetBinContent(ibin)<<endl;
                        }
                        //do not rescale to luminosity if the dataset is Data Driven
                        else{
                                NofExpectedEvtsFinal_1D[d]->SetBinContent(ibin, factor*UncertaintyCorr*NofExpectedEvtsInit_1D[d]->GetBinContent(ibin));
                                value+=factor*UncertaintyCorr*NofExpectedEvtsInit_1D[d]->GetBinContent(ibin);
                                //cout<<"VALUE: "<<factor*UncertaintyCorr*NofExpectedEvtsInit_1D[d]->GetBinContent(ibin)<<" = "<<factor<<"*"<<UncertaintyCorr<<"*"<<NofExpectedEvtsInit_1D[d]->GetBinContent(ibin)<<endl;
                        }
                }
                NofExpectedEvtsSumCurrent_1D->SetBinContent(ibin,value);
        }
        return NofExpectedEvtsSumCurrent_1D;
}

void DistributionDescription::Compute2DFinalDistrib(const vector<double>& parUnc, const vector<double>& GraphUnc, const vector<double>& parEstim, const vector<double>& GraphEstim, const vector<double>& bkgNorm, const double& LumiFactor, const double& SignalFactor, const vector<int>& iSignalDatasets){
        if(parUnc.size()!=Histo_2D_UncertMinus.size() || parUnc.size()!=Histo_2D_UncertPlus.size()){
                cerr<<"DistributionDescription::Compute2DFinalDistrib:: inconsistency in the size of the collection "<<endl;
                cerr<<"\t parUnc.size = "<<parUnc.size()<<" Histo_2D_UncertMinus.size = "<<Histo_2D_UncertMinus.size()<<" Histo_2D_UncertPlus.size = "<<Histo_2D_UncertPlus.size()<<endl;
        }
        if(GraphUnc.size()!=Graph_Uncert.size()){
                cerr<<"DistributionDescription::Compute2DFinalDistrib:: inconsistency in the size of the collection "<<endl;
                cerr<<"\t GraphUnc.size = "<<GraphUnc.size()<<" Graph_Uncert.size = "<<Graph_Uncert.size()<<endl;
        }
        if((bkgNorm.size()+iSignalDatasets.size())!=(NofExpectedEvtsInit_2D.size())){
                cerr<<"DistributionDescription::Compute2DFinalDistrib:: inconsistency in the size of the collection "<<endl;
                cerr<<"\t bkgNorm.size = "<<bkgNorm.size()<<" iSignalDatasets.size= "<<iSignalDatasets.size()<<" NofExpectedEvtsInit_2D.size = "<<NofExpectedEvtsInit_2D.size()<<endl;
        }
        for(int ibin=1;ibin<NofExpectedEvtsSumCurrent_2D->GetNbinsX()+1;ibin++){
          for(int jbin=1;jbin<NofExpectedEvtsSumCurrent_2D->GetNbinsY()+1;jbin++){
                int it=0;
                for(unsigned int d=0;d<NofExpectedEvtsInit_2D.size();d++){
                        if(NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin)==0) continue;
                        float factor = 1.;
                        //int it=0;
                        for(unsigned id=0;id<iSignalDatasets.size();id++){
                                if((int)d == iSignalDatasets[id]) factor = SignalFactor;
                                else{
                                        if(bkgNorm[it]>0) factor = bkgNorm[it]/NofExpectedEvtsInit_2D[d]->Integral();
                                        it++;
                                }
                        }
                        float UncertaintyCorr = 1.;//0
                        for(unsigned int i=0;i<parUnc.size();i++){
                                UncertaintyCorr*=Interpol(parUnc[i], 1., Histo_2D_UncertMinus[i][d]->GetBinContent(ibin,jbin)/NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin), Histo_2D_UncertPlus[i][d]->GetBinContent(ibin,jbin)/NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin));
                                /*
                                if(parUnc[i]<0)
                                        UncertaintyCorr+= (NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin)-Histo_2D_UncertMinus[i][d]->GetBinContent(ibin,jbin))*parUnc[i];
                                else
                                        UncertaintyCorr+= (NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin)-Histo_2D_UncertPlus[i][d]->GetBinContent(ibin,jbin))*parUnc[i];
                                */
                                /*
                                if(parUnc[i]<0)
                                        UncertaintyCorr*= 1+(((NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin)-Histo_2D_UncertMinus[i][d]->GetBinContent(ibin,jbin))*parUnc[i])/NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin));
                                else
                                        UncertaintyCorr*= 1+(((Histo_2D_UncertPlus[i][d]->GetBinContent(ibin,jbin)-NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin))*parUnc[i])/NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin));
                                */      
                        }
                        for(unsigned int i=0;i<parEstim.size();i++){
                                UncertaintyCorr*=Interpol(parEstim[i], 1., Histo_2D_EstimMinus[i][d]->GetBinContent(ibin,jbin)/NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin), Histo_2D_EstimPlus[i][d]->GetBinContent(ibin,jbin)/NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin));
                                /*
                                if(parEstim[i]<0)
                                        UncertaintyCorr*= 1+(((NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin)-Histo_2D_EstimMinus[i][d]->GetBinContent(ibin,jbin))*parEstim[i])/NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin));
                                else
                                        UncertaintyCorr*= 1+(((Histo_2D_EstimPlus[i][d]->GetBinContent(ibin,jbin)-NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin))*parEstim[i])/NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin));
                                */      
                        }
                        int graphIndice = 0;
                        graphIndice = (ibin-1)*Nobs_2D->GetNbinsY()+(jbin-1);
                        for(unsigned int i=0;i<GraphUnc.size();i++){
                                if(Graph_Uncert[i][d][graphIndice]){
                                        //cout<<Graph_Uncert[i][d][graphIndice]->GetFunction("pol1")<<endl;
                                        //cout<<Graph_Uncert[i][d][graphIndice]->GetFunction("pol2")<<endl;
                                        //if(Graph_Uncert[i][d][graphIndice]->GetFunction("pol1")) UncertaintyCorr+=(Graph_Uncert[i][d][graphIndice]->GetFunction("pol1")->Eval(GraphUnc[i])-1)*NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin);
                                        //if(Graph_Uncert[i][d][graphIndice]->GetFunction("pol2")) UncertaintyCorr+=(Graph_Uncert[i][d][graphIndice]->GetFunction("pol2")->Eval(GraphUnc[i])-1)*NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin);
                                        //if(Graph_Uncert[i][d][graphIndice]->GetFunction("pol1")) UncertaintyCorr*=(Graph_Uncert[i][d][graphIndice]->GetFunction("pol1")->Eval(GraphUnc[i]));
                                        //if(Graph_Uncert[i][d][graphIndice]->GetFunction("pol2")) UncertaintyCorr*=(Graph_Uncert[i][d][graphIndice]->GetFunction("pol2")->Eval(GraphUnc[i]));
                                        float NewFactor = GetFactorFromFitFunctionOfTheGraph(Graph_Uncert[i][d][graphIndice], GraphUnc[i], string("Compute2DFinalDistrib"));
                                        UncertaintyCorr*=NewFactor;
                                        /*
                                        if(Graph_Uncert[i][d][graphIndice]->GetListOfFunctions()->GetSize()>=1){
                                                float NewFactor=((TF1*) Graph_Uncert[i][d][graphIndice]->GetListOfFunctions()->At(0))->Eval(GraphUnc[i]);
                                                if(NewFactor<=0) cout<<"DistributionDescription::Compute2DFinalDistrib:: factor for the fit function should not be negative ; factor = "<<NewFactor<<endl; 
                                                if(NewFactor>2.) cout<<"DistributionDescription::Compute2DFinalDistrib:: factor for the fit function is high; factor = "<<NewFactor<<endl;
                                                if(TMath::IsNaN(NewFactor)) cout<<"DistributionDescription::Compute2DFinalDistrib:: the factor computed for input = "<<GraphUnc[i]<<" with the graph "<<Graph_Uncert[i][d][graphIndice]->GetName()<<" is NaN !!"<<endl;
                                                cout<<"NewFactor: "<<NewFactor<<" ("<<GraphUnc[i]<<")"<<endl; 
                                                UncertaintyCorr*=NewFactor;
                                        }
                                        else cerr<<"DistributionDescription::Compute2DFinalDistrib:: no fit function found in the graphic "<<Graph_Uncert[i][d][graphIndice]->GetName()<<endl;
                                        */
                                }
                        }
                        for(unsigned int i=0;i<GraphEstim.size();i++){
                                if(Graph_Estim[i][d][graphIndice]){
                                        //cout<<Graph_Uncert[i][d][graphIndice]->GetFunction("pol1")<<endl;
                                        //cout<<Graph_Uncert[i][d][graphIndice]->GetFunction("pol2")<<endl;
                                        //if(Graph_Estim[i][d][graphIndice]->GetFunction("pol1")) UncertaintyCorr+=(Graph_Estim[i][d][graphIndice]->GetFunction("pol1")->Eval(GraphEstim[i])-1)*NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin);
                                        //if(Graph_Estim[i][d][graphIndice]->GetFunction("pol2")) UncertaintyCorr+=(Graph_Estim[i][d][graphIndice]->GetFunction("pol2")->Eval(GraphEstim[i])-1)*NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin);
                                        //if(Graph_Estim[i][d][graphIndice]->GetFunction("pol1")) UncertaintyCorr*=(Graph_Estim[i][d][graphIndice]->GetFunction("pol1")->Eval(GraphEstim[i]));
                                        //if(Graph_Estim[i][d][graphIndice]->GetFunction("pol2")) UncertaintyCorr*=(Graph_Estim[i][d][graphIndice]->GetFunction("pol2")->Eval(GraphEstim[i]));
                                        float NewFactor = GetFactorFromFitFunctionOfTheGraph(Graph_Estim[i][d][graphIndice], GraphUnc[i], string("Compute2DFinalDistrib"));
                                        UncertaintyCorr*=NewFactor;
                                        /*
                                        if(Graph_Estim[i][d][graphIndice]->GetListOfFunctions()->GetSize()>=1){
                                                UncertaintyCorr*=((TF1*) Graph_Estim[i][d][graphIndice]->GetListOfFunctions()->At(0))->Eval(GraphEstim[i]);
                                        }
                                        */
                                }
                        }
                        //update also NofExpectedEvtsFinal_2D
                        if(!DatasetIsDD[d]){
                                NofExpectedEvtsFinal_2D[d]->SetBinContent(ibin,jbin, LumiFactor*factor*UncertaintyCorr*NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin));
                        }
                        //do not rescale to luminosity if the dataset is Data Driven
                        else{
                                NofExpectedEvtsFinal_2D[d]->SetBinContent(ibin,jbin, factor*UncertaintyCorr*NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin));
                        }
                }
          }
        }
}

TH2F* DistributionDescription::Compute2DPseudoData(TRandom& rand, const vector<double>& parUnc, const vector<double>& GraphUnc, const vector<double>& parEstim, const vector<double>& GraphEstim, const vector<double>& bkgNorm, const double& LumiFactor, const double& SignalFactor, const vector<int>& iSignalDatasets) const{
        if(parUnc.size()!=Histo_2D_UncertMinus.size() || parUnc.size()!=Histo_2D_UncertPlus.size()){
                cerr<<"DistributionDescription::Compute2DPseudoData:: inconsistency in the size of the collection "<<endl;
                cerr<<"\t parUnc.size = "<<parUnc.size()<<" Histo_2D_UncertMinus.size = "<<Histo_2D_UncertMinus.size()<<" Histo_2D_UncertPlus.size = "<<Histo_2D_UncertPlus.size()<<endl;
        }
        if(GraphUnc.size()!=Graph_Uncert.size()){
                cerr<<"DistributionDescription::Compute2DPseudoData:: inconsistency in the size of the collection "<<endl;
                cerr<<"\t GraphUnc.size = "<<GraphUnc.size()<<" Graph_Uncert.size = "<<Graph_Uncert.size()<<endl;
        }
        if((bkgNorm.size()+iSignalDatasets.size())!=(NofExpectedEvtsInit_2D.size())){
                cerr<<"DistributionDescription::Compute2DPseudoData:: inconsistency in the size of the collection "<<endl;
                cerr<<"\t bkgNorm.size = "<<bkgNorm.size()<<" iSignalDatasets.size= "<<iSignalDatasets.size()<<" NofExpectedEvtsInit_2D.size = "<<NofExpectedEvtsInit_2D.size()<<endl;
        }
        for(int ibin=1;ibin<NofExpectedEvtsSumCurrent_2D->GetNbinsX()+1;ibin++){
          for(int jbin=1;jbin<NofExpectedEvtsSumCurrent_2D->GetNbinsY()+1;jbin++){
                float value = 0;
                int it=0;
                for(unsigned int d=0;d<NofExpectedEvtsInit_2D.size();d++){
                        if(NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin)==0) continue;
                        float factor = 1.;
                        //int it=0;
                        for(unsigned id=0;id<iSignalDatasets.size();id++){
                                if((int)d == iSignalDatasets[id]) factor = SignalFactor;
                                else{
                                        if(bkgNorm[it]>0) factor = bkgNorm[it]/NofExpectedEvtsInit_2D[d]->Integral();
                                        it++;
                                }
                        }
                        float UncertaintyCorr = 1.;//0
                        for(unsigned int i=0;i<parUnc.size();i++){
                                UncertaintyCorr*=Interpol(parUnc[i], 1., Histo_2D_UncertMinus[i][d]->GetBinContent(ibin,jbin)/NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin), Histo_2D_UncertPlus[i][d]->GetBinContent(ibin,jbin)/NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin));
                                /*
                                if(parUnc[i]<0)
                                        UncertaintyCorr+= (NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin)-Histo_2D_UncertMinus[i][d]->GetBinContent(ibin,jbin))*parUnc[i];
                                else
                                        UncertaintyCorr+= (NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin)-Histo_2D_UncertPlus[i][d]->GetBinContent(ibin,jbin))*parUnc[i];
                                */
                                /*
                                if(parUnc[i]<0)
                                        UncertaintyCorr*= 1+(((NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin)-Histo_2D_UncertMinus[i][d]->GetBinContent(ibin,jbin))*parUnc[i])/NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin));
                                else
                                        UncertaintyCorr*= 1+(((Histo_2D_UncertPlus[i][d]->GetBinContent(ibin,jbin)-NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin))*parUnc[i])/NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin));
                                */      
                        }
                        for(unsigned int i=0;i<parEstim.size();i++){
                                UncertaintyCorr*=Interpol(parEstim[i], 1., Histo_2D_EstimMinus[i][d]->GetBinContent(ibin,jbin)/NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin), Histo_2D_EstimPlus[i][d]->GetBinContent(ibin,jbin)/NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin));
                                /*
                                if(parUnc[i]<0)
                                        UncertaintyCorr+= (NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin)-Histo_2D_EstimMinus[i][d]->GetBinContent(ibin,jbin))*parEstim[i];
                                else
                                        UncertaintyCorr+= (NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin)-Histo_2D_EstimPlus[i][d]->GetBinContent(ibin,jbin))*parEstim[i];
                                */
                                /*
                                if(parEstim[i]<0)
                                        UncertaintyCorr*= 1+(((NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin)-Histo_2D_EstimMinus[i][d]->GetBinContent(ibin,jbin))*parEstim[i])/NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin));
                                else
                                        UncertaintyCorr*= 1+(((Histo_2D_EstimPlus[i][d]->GetBinContent(ibin,jbin)-NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin))*parEstim[i])/NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin));
                                */      
                        }
                        int graphIndice = 0;
                        graphIndice = (ibin-1)*Nobs_2D->GetNbinsY()+(jbin-1);
                        for(unsigned int i=0;i<GraphUnc.size();i++){
                                if(Graph_Uncert[i][d][graphIndice]){
                                        //cout<<Graph_Uncert[i][d][graphIndice]->GetFunction("pol1")<<endl;
                                        //cout<<Graph_Uncert[i][d][graphIndice]->GetFunction("pol2")<<endl;
                                        //if(Graph_Uncert[i][d][graphIndice]->GetFunction("pol1")) UncertaintyCorr+=(Graph_Uncert[i][d][graphIndice]->GetFunction("pol1")->Eval(GraphUnc[i])-1)*NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin);
                                        //if(Graph_Uncert[i][d][graphIndice]->GetFunction("pol2")) UncertaintyCorr+=(Graph_Uncert[i][d][graphIndice]->GetFunction("pol2")->Eval(GraphUnc[i])-1)*NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin);
                                        //if(Graph_Uncert[i][d][graphIndice]->GetFunction("pol1")) UncertaintyCorr*=(Graph_Uncert[i][d][graphIndice]->GetFunction("pol1")->Eval(GraphUnc[i]));
                                        //if(Graph_Uncert[i][d][graphIndice]->GetFunction("pol2")) UncertaintyCorr*=(Graph_Uncert[i][d][graphIndice]->GetFunction("pol2")->Eval(GraphUnc[i]));
                                        float NewFactor = GetFactorFromFitFunctionOfTheGraph(Graph_Uncert[i][d][graphIndice], GraphUnc[i], string("Compute2DPseudoData"));
                                        UncertaintyCorr*=NewFactor;
                                        /*
                                        if(Graph_Uncert[i][d][graphIndice]->GetListOfFunctions()->GetSize()>=1){
                                                UncertaintyCorr*=((TF1*) Graph_Uncert[i][d][graphIndice]->GetListOfFunctions()->At(0))->Eval(GraphUnc[i]);
                                        }
                                        */
                                }
                        }
                        for(unsigned int i=0;i<GraphEstim.size();i++){
                                if(Graph_Estim[i][d][graphIndice]){
                                        //cout<<Graph_Uncert[i][d][graphIndice]->GetFunction("pol1")<<endl;
                                        //cout<<Graph_Uncert[i][d][graphIndice]->GetFunction("pol2")<<endl;
                                        //if(Graph_Estim[i][d][graphIndice]->GetFunction("pol1")) UncertaintyCorr+=(Graph_Estim[i][d][graphIndice]->GetFunction("pol1")->Eval(GraphEstim[i])-1)*NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin);
                                        //if(Graph_Estim[i][d][graphIndice]->GetFunction("pol2")) UncertaintyCorr+=(Graph_Estim[i][d][graphIndice]->GetFunction("pol2")->Eval(GraphEstim[i])-1)*NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin);
                                        //if(Graph_Estim[i][d][graphIndice]->GetFunction("pol1")) UncertaintyCorr*=(Graph_Estim[i][d][graphIndice]->GetFunction("pol1")->Eval(GraphEstim[i]));
                                        //if(Graph_Estim[i][d][graphIndice]->GetFunction("pol2")) UncertaintyCorr*=(Graph_Estim[i][d][graphIndice]->GetFunction("pol2")->Eval(GraphEstim[i]));
                                        float NewFactor = GetFactorFromFitFunctionOfTheGraph(Graph_Estim[i][d][graphIndice], GraphUnc[i], string("Compute2DPseudoData"));
                                        UncertaintyCorr*=NewFactor;
                                        /*
                                        if(Graph_Estim[i][d][graphIndice]->GetListOfFunctions()->GetSize()>=1){
                                                UncertaintyCorr*=((TF1*) Graph_Estim[i][d][graphIndice]->GetListOfFunctions()->At(0))->Eval(GraphEstim[i]);
                                        }
                                        */
                                }
                        }
                        //update also NofExpectedEvtsFinal_2D
                        if(!DatasetIsDD[d]){
                                NofExpectedEvtsFinal_2D[d]->SetBinContent(ibin,jbin, LumiFactor*factor*UncertaintyCorr*NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin));
                                value+=LumiFactor*factor*UncertaintyCorr*NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin);
                        }
                        //do not rescale to luminosity if the dataset is Data Driven
                        else{
                                NofExpectedEvtsFinal_2D[d]->SetBinContent(ibin,jbin, factor*UncertaintyCorr*NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin));
                                value+=factor*UncertaintyCorr*NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin);
                        }
                }
                //randomization with a Poisson distribution - returns a integer as expected for data
                value = (float) rand.Poisson(value);
                NobsPE_2D->SetBinContent(ibin,jbin,value);
          }
        }
        return NobsPE_2D;
}
TH2F* DistributionDescription::Get2DNexp(const vector<double>& parUnc, const vector<double>& GraphUnc, const vector<double>& parEstim, const vector<double>& GraphEstim, const vector<double>& bkgNorm, const double& LumiFactor, const double& SignalFactor, const vector<int>& iSignalDatasets) const{
        if(parUnc.size()!=Histo_2D_UncertMinus.size() || parUnc.size()!=Histo_2D_UncertPlus.size()){
                cerr<<"DistributionDescription::Get2DNexp:: inconsistency in the size of the collection "<<endl;
                cerr<<"\t parUnc.size = "<<parUnc.size()<<" Histo_2D_UncertMinus.size = "<<Histo_2D_UncertMinus.size()<<" Histo_2D_UncertPlus.size = "<<Histo_2D_UncertPlus.size()<<endl;
        }
        if(GraphUnc.size()!=Graph_Uncert.size()){
                cerr<<"DistributionDescription::Get2DNexp:: inconsistency in the size of the collection "<<endl;
                cerr<<"\t GraphUnc.size = "<<GraphUnc.size()<<" Graph_Uncert.size = "<<Graph_Uncert.size()<<endl;
        }
        if((bkgNorm.size()+iSignalDatasets.size())!=(NofExpectedEvtsInit_2D.size())){
                cerr<<"DistributionDescription::Get2DNexp:: inconsistency in the size of the collection "<<endl;
                cerr<<"\t bkgNorm.size = "<<bkgNorm.size()<<" iSignalDatasets.size= "<<iSignalDatasets.size()<<" NofExpectedEvtsInit_2D.size = "<<NofExpectedEvtsInit_2D.size()<<endl;
        }
        //it was ibin=0 & jbin=0 before ... false
        for(int ibin=1;ibin<NofExpectedEvtsSumCurrent_2D->GetNbinsX()+1;ibin++){
          for(int jbin=1;jbin<NofExpectedEvtsSumCurrent_2D->GetNbinsY()+1;jbin++){
                float value = 0;
                int it=0;
                for(unsigned int d=0;d<NofExpectedEvtsInit_2D.size();d++){
                        if(NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin)==0) continue;
                        float factor = 1.;
                        //int it=0;
                        //cout<<"ERIC "<<SignalFactor<<" "<<NofExpectedEvtsInit_2D[d]->Integral()<<" "<<bkgNorm[it]<<endl;
                        for(unsigned id=0;id<iSignalDatasets.size();id++){
                                if((int)d == iSignalDatasets[id]) factor = SignalFactor;
                                else{
                                        if(bkgNorm[it]>0 && NofExpectedEvtsInit_2D[d]->Integral()>0) factor = bkgNorm[it]/NofExpectedEvtsInit_2D[d]->Integral();
                                        it++;
                                }
                        }
                        float UncertaintyCorr = 1.;//0
                        for(unsigned int i=0;i<parUnc.size();i++){
                                //float tmp = Interpol(parUnc[i], 1., Histo_2D_UncertMinus[i][d]->GetBinContent(ibin,jbin)/Histo_2D_UncertPlus[i][d]->GetBinContent(ibin,jbin)/NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin), Histo_2D_UncertPlus[i][d]->GetBinContent(ibin,jbin)/NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin));
                                float tmp = Interpol(parUnc[i], 1., Histo_2D_UncertMinus[i][d]->GetBinContent(ibin,jbin)/NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin), Histo_2D_UncertPlus[i][d]->GetBinContent(ibin,jbin)/NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin));
                                //cout<<Histo_2D_UncertMinus[i][d]->GetBinContent(ibin,jbin)<<" "<<Histo_2D_UncertPlus[i][d]->GetBinContent(ibin,jbin)<<" "<<Histo_2D_UncertPlus[i][d]->GetBinContent(ibin,jbin)<<endl;
                                //cout<<"tmp: x: "<<parUnc[i]<<" effect: "<<tmp<<endl;
                                //UncertaintyCorr*=Interpol(parUnc[i], 1., Histo_2D_UncertMinus[i][d]->GetBinContent(ibin,jbin)/Histo_2D_UncertPlus[i][d]->GetBinContent(ibin,jbin)/NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin), Histo_2D_UncertPlus[i][d]->GetBinContent(ibin,jbin)/NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin));
                                UncertaintyCorr*=Interpol(parUnc[i], 1., Histo_2D_UncertMinus[i][d]->GetBinContent(ibin,jbin)/NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin), Histo_2D_UncertPlus[i][d]->GetBinContent(ibin,jbin)/NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin));
                                /*
                                if(parUnc[i]<0)
                                        UncertaintyCorr+= (NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin)-Histo_2D_UncertMinus[i][d]->GetBinContent(ibin,jbin))*parUnc[i];
                                else
                                        UncertaintyCorr+= (NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin)-Histo_2D_UncertPlus[i][d]->GetBinContent(ibin,jbin))*parUnc[i];
                                */
                                /*
                                if(parUnc[i]<0)
                                        UncertaintyCorr*= 1+(((NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin)-Histo_2D_UncertMinus[i][d]->GetBinContent(ibin,jbin))*parUnc[i])/NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin));
                                else
                                        UncertaintyCorr*= 1+(((Histo_2D_UncertPlus[i][d]->GetBinContent(ibin,jbin)-NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin))*parUnc[i])/NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin));
                                */      
                        }
                        for(unsigned int i=0;i<parEstim.size();i++){
                                UncertaintyCorr*=Interpol(parEstim[i], 1., Histo_2D_EstimMinus[i][d]->GetBinContent(ibin,jbin)/NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin), Histo_2D_EstimPlus[i][d]->GetBinContent(ibin,jbin)/NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin));
                                /*
                                if(parUnc[i]<0)
                                        UncertaintyCorr+= (NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin)-Histo_2D_EstimMinus[i][d]->GetBinContent(ibin,jbin))*parEstim[i];
                                else
                                        UncertaintyCorr+= (NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin)-Histo_2D_EstimPlus[i][d]->GetBinContent(ibin,jbin))*parEstim[i];
                                */
                                /*
                                if(parEstim[i]<0)
                                        UncertaintyCorr*= 1+(((NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin)-Histo_2D_EstimMinus[i][d]->GetBinContent(ibin,jbin))*parEstim[i])/NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin));
                                else
                                        UncertaintyCorr*= 1+(((Histo_2D_EstimPlus[i][d]->GetBinContent(ibin,jbin)-NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin))*parEstim[i])/NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin));
                                */      
                        }
                        int graphIndice = 0;
                        //cout<<"XYSIZE: "<<Nobs_2D->GetNbinsX()<<" "<<Nobs_2D->GetNbinsY()<<endl;
                        graphIndice = (ibin-1)*Nobs_2D->GetNbinsY()+(jbin-1);
                        //cout<<"graphIndice "<<graphIndice<<" "<<ibin<<" "<<jbin<<endl; 
                        for(unsigned int i=0;i<GraphUnc.size();i++){
                                if(Graph_Uncert[i][d][graphIndice]){
                                        //cout<<Graph_Uncert[i][d][graphIndice]->GetFunction("pol1")<<endl;
                                        //cout<<Graph_Uncert[i][d][graphIndice]->GetFunction("pol2")<<endl;
                                        //if(Graph_Uncert[i][d][graphIndice]->GetFunction("pol1")) UncertaintyCorr+=(Graph_Uncert[i][d][graphIndice]->GetFunction("pol1")->Eval(GraphUnc[i])-1)*NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin);
                                        //if(Graph_Uncert[i][d][graphIndice]->GetFunction("pol2")) UncertaintyCorr+=(Graph_Uncert[i][d][graphIndice]->GetFunction("pol2")->Eval(GraphUnc[i])-1)*NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin);
                                        //if(Graph_Uncert[i][d][graphIndice]->GetFunction("pol1")) UncertaintyCorr*=(Graph_Uncert[i][d][graphIndice]->GetFunction("pol1")->Eval(GraphUnc[i]));
                                        //if(Graph_Uncert[i][d][graphIndice]->GetFunction("pol2")) UncertaintyCorr*=(Graph_Uncert[i][d][graphIndice]->GetFunction("pol2")->Eval(GraphUnc[i]));
                                        //cout<<"ATTENTION: "<<GraphUnc[i]<<endl;
                                        float NewFactor = GetFactorFromFitFunctionOfTheGraph(Graph_Uncert[i][d][graphIndice], GraphUnc[i], string("Get2DNexp"));
                                        UncertaintyCorr*=NewFactor;
                                }
                        }
                        //cout<<"#graphEstim: "<<GraphEstim.size()<<endl;
                        for(unsigned int i=0;i<GraphEstim.size();i++){
                                if(Graph_Estim[i][d][graphIndice]){
                                        //cout<<Graph_Estim[i][d][graphIndice]<<" pol1 "<<Graph_Estim[i][d][graphIndice]->GetFunction("pol1")<<" "<<Graph_Estim[i][d][graphIndice]->GetFunction("pol2")<<endl;
                                        //if(Graph_Estim[i][d][graphIndice]->GetFunction("pol1")) UncertaintyCorr+=(Graph_Estim[i][d][graphIndice]->GetFunction("pol1")->Eval(GraphEstim[i])-1)*NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin);
                                        //if(Graph_Estim[i][d][graphIndice]->GetFunction("pol2"))       UncertaintyCorr+=(Graph_Estim[i][d][graphIndice]->GetFunction("pol2")->Eval(GraphEstim[i])-1)*NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin);
                                        //if(Graph_Estim[i][d][graphIndice]->GetFunction("pol1")) UncertaintyCorr*=(Graph_Estim[i][d][graphIndice]->GetFunction("pol1")->Eval(GraphEstim[i]));
                                        //if(Graph_Estim[i][d][graphIndice]->GetFunction("pol2")) UncertaintyCorr*=(Graph_Estim[i][d][graphIndice]->GetFunction("pol2")->Eval(GraphEstim[i]));
                                        //if(Graph_Estim[i][d][graphIndice]->GetListOfFunctions()->GetSize()>=1){
                                //              UncertaintyCorr*=((TF1*) Graph_Estim[i][d][graphIndice]->GetListOfFunctions()->At(0))->Eval(GraphEstim[i]);
                                        //}
                                        //cout<<"ATTENTION2: "<<GraphEstim[i]<<endl;
                                        float NewFactor = GetFactorFromFitFunctionOfTheGraph(Graph_Estim[i][d][graphIndice], GraphEstim[i], string("Get2DNexp"));
                                        //cout<<"NewFactor due to GraphEstim_"<<i<<" : "<<NewFactor<<" for param = "<<GraphEstim[i]<<endl;
                                        UncertaintyCorr*=NewFactor;
                                }
                        }
                        //update also NofExpectedEvtsFinal_2D
                        if(!DatasetIsDD[d]){
                                NofExpectedEvtsFinal_2D[d]->SetBinContent(ibin,jbin, LumiFactor*factor*UncertaintyCorr*NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin));
                                if(isnan(UncertaintyCorr)) UncertaintyCorr = 1.;
                                value+=LumiFactor*factor*UncertaintyCorr*NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin);
                                //cout<<"VALUE: "<<value<<": "<<factor<<"*"<<UncertaintyCorr<<"*"<<NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin)<<"*"<<LumiFactor<<endl;
                        }
                        //do not rescale to luminosity if the dataset is Data Driven
                        else{
                                NofExpectedEvtsFinal_2D[d]->SetBinContent(ibin,jbin, factor*UncertaintyCorr*NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin));
                                if(isnan(UncertaintyCorr)) UncertaintyCorr = 1.;
                                value+=factor*UncertaintyCorr*NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin);
                                //cout<<"VALUe: "<<value<<": "<<factor<<"*"<<UncertaintyCorr<<"*"<<NofExpectedEvtsInit_2D[d]->GetBinContent(ibin,jbin)<<"*"<<LumiFactor<<endl;
                        }
                }
                NofExpectedEvtsSumCurrent_2D->SetBinContent(ibin,jbin,value);
          }
        }
        //for debugging
        /*
        cout<<"ChecK - bgkNorm "<<endl;
        for(unsigned int i=0;i<bkgNorm.size();i++) cout<<"bkgNorm["<<i<<"] = "<<bkgNorm[i]<<endl;
        //once everything is modified{
        for(unsigned int d=0;d<NofExpectedEvtsInit_2D.size();d++){
                cout<<"NofExpectedEvtsInit_2D::"<<NofExpectedEvtsInit_2D[d]->GetName()<<" Integ::"<<NofExpectedEvtsInit_2D[d]->Integral()<<endl;
                cout<<"NofExpectedEvtsFinal_2D::"<<NofExpectedEvtsFinal_2D[d]->GetName()<<" Integ::"<<NofExpectedEvtsFinal_2D[d]->Integral()<<endl;
        }
        */
        return NofExpectedEvtsSumCurrent_2D;
}

double DistributionDescription::Interpol(double x, double* par) const{
        if(InterpolMethod == string("linear")) return LinearInterpol(x, par);
        if(InterpolMethod == string("expo")) return ExpoInterpol(x, par);
        if(InterpolMethod == string("cubic")) return CubicInterpol(x, par);
        cerr<<"PLRMeasurement::Interpol:: no prefered method - use linear method"<<endl; 
        return LinearInterpol(x, par);
}

double DistributionDescription::Interpol(double x, double mean, double effm1s, double effp1s) const{
        double par[3];
        par[0] = mean;
        par[1] = effm1s;
        par[2] = effp1s;
        return Interpol(x,par);
}
        
void DistributionDescription::NormalizeDistrib(const vector<float>& norm){
        if(NofExpectedEvtsInit_1D.size()!=0){
                if(NofExpectedEvtsInit_1D.size()==norm.size()){
                        for(unsigned int i=0;i<NofExpectedEvtsInit_1D.size();i++){
                                if(NofExpectedEvtsInit_1D[i]->Integral()>0) NofExpectedEvtsInit_1D[i]->Scale(norm[i]/NofExpectedEvtsInit_1D[i]->Integral());
                        }
                }       
                else cerr<<"DistributionDescription::NormalizeDistrib::Error while normalizing the histograms, norm.size=="<<norm.size()<<" NofExpectedEvtsInit_1D.size=="<<NofExpectedEvtsInit_1D.size()<<endl;
        }
        if(NofExpectedEvtsInit_2D.size()!=0){
                        for(unsigned int i=0;i<NofExpectedEvtsInit_2D.size();i++){ cout<<NofExpectedEvtsInit_2D[i]->GetName()<<endl;}
                if(NofExpectedEvtsInit_2D.size()==norm.size()){
                        for(unsigned int i=0;i<NofExpectedEvtsInit_2D.size();i++){
                                if(NofExpectedEvtsInit_2D[i]->Integral()>0) NofExpectedEvtsInit_2D[i]->Scale(norm[i]/NofExpectedEvtsInit_2D[i]->Integral());
                                cout<<"NORMALIZEDISTRIB: "<<norm[i]<<endl;
                        }
                }       
                else cerr<<"DistributionDescription::NormalizeDistrib::Error while normalizing the histograms, norm.size=="<<norm.size()<<" NofExpectedEvtsInit_1D.size=="<<NofExpectedEvtsInit_2D.size()<<endl;
        }
}

