#include "../interface/AllDatasetUncertainties.h"




AllDatasetUncertainties::AllDatasetUncertainties(){
        NofDim = 1;
}

AllDatasetUncertainties::AllDatasetUncertainties(const string& Name, const int& nofDim){
        name = Name;
        NofDim = nofDim;
}
        
AllDatasetUncertainties::~AllDatasetUncertainties(){}
        
void AllDatasetUncertainties::LoadDatasets(const vector<Dataset>& datasets){
        for(unsigned int i=0;i<datasets.size();i++){
                Uncertainty unc;
                unc.SetName(name);      
                unc.SetDataset(datasets[i].Name());
                uncertainties.push_back(unc);
                bool load = false;
                effLoaded.push_back(load);
        }
}

void AllDatasetUncertainties::SetName(const string& Name){
        name = Name;
}
        
void AllDatasetUncertainties::SetParameters(const int& idataset, const float& paramMean, const float& paramMinus, const float& paramPlus){
        if(idataset>=0 && idataset<(int)uncertainties.size()){
                uncertainties[idataset].LoadParameters(paramMean, paramMinus, paramPlus);
        }
        else cerr<<"AllDatasetUncertainties::SetParameters  idataset parameter is wrong "<<endl;
}

void AllDatasetUncertainties::LoadEfficiencies(const int& idataset, const float& eff_nom, const float& eff_m1sigma, const float& eff_p1sigma){
        if(idataset>=0 && idataset<(int)uncertainties.size()){
                uncertainties[idataset].LoadEfficiencies(eff_nom, eff_m1sigma, eff_p1sigma);
                effLoaded[idataset] = true;
        }
        else cerr<<"AllDatasetUncertainties::SetParameters  idataset parameter is wrong "<<endl;
}

void AllDatasetUncertainties::SetParameters(const float& paramMean, const float& paramMinus, const float& paramPlus){
        for(unsigned int i=0;i<uncertainties.size();i++){
                uncertainties[i].LoadParameters(paramMean, paramMinus, paramPlus);
        }       
}       

void AllDatasetUncertainties::Fill(const int& idataset, const int& icond, const float& weight){
        if(idataset>=0 && idataset<(int)uncertainties.size()) uncertainties[idataset].Fill(icond, weight);
}
        
void AllDatasetUncertainties::Compute(){
        for(unsigned int i=0;i<uncertainties.size();i++){
                if(!effLoaded[i]) uncertainties[i].Compute();
        }       
}
        
vector<float*> AllDatasetUncertainties::GetParameters() const{
        vector<float*> param;   
        for(unsigned int i=0;i<uncertainties.size();i++){
                param.push_back(uncertainties[i].GetParameters());
        }       
        return param;
}
        
vector<float*> AllDatasetUncertainties::GetEfficiencies() const{
        vector<float*> eff;     
        for(unsigned int i=0;i<uncertainties.size();i++){
                eff.push_back(uncertainties[i].GetEfficiencies());
        }       
        return eff;
} 
        
float* AllDatasetUncertainties::GetParameters(int idataset) const{
        if(idataset>=0 && idataset<(int)uncertainties.size()) return uncertainties[idataset].GetParameters();
        return 0;
}

float* AllDatasetUncertainties::GetEfficiencies(int idataset) const{
        if(idataset>=0 && idataset<(int)uncertainties.size()) return uncertainties[idataset].GetEfficiencies();
        return 0;
}
 
string AllDatasetUncertainties::GetName() const {return name;}
        
vector<string> AllDatasetUncertainties::GetListOfDatasets() const{
        vector<string> names;
        for(unsigned int i=0;i<uncertainties.size();i++){
                names.push_back(uncertainties[i].GetDataset());
        }
        return names;
}


//--- Accessors to histo ---//
        
bool AllDatasetUncertainties::LoadEffiencies(TFile* fin, string dirname, string histoName){
        if(fin==0){
                cerr<<"AllDatasetUncertainties::LoadEffiencies:: pointer on file nul"<<endl;
                return false;
        }
        TDirectory* dir = 0;
        TDirectory* dir2 = 0;
        if(dirname == string("")){
                dir = fin;
        } 
        else{
                dir = fin->GetDirectory(dirname.c_str());
                if(dir == 0 ){
                        cerr<<"AllDatasetUncertainties::LoadEffiencies:: the subdirector "<<dirname<<" was not found in the file "<<fin->GetName()<<endl;
                        delete dir;
                        delete dir2;
                        return false;
                }
        }
        for(unsigned int i=0;i<uncertainties.size();i++){
                dir2 = dir->GetDirectory(uncertainties[i].GetDataset().c_str());
                if(dir2 == 0){
                        cerr<<"AllDatasetUncertainties::LoadEffiencies:: the subdirector "<<dirname<<"/"<<uncertainties[i].GetDataset()<<" was not found in the file "<<fin->GetName()<<endl;
                        delete dir;
                        delete dir2;
                        return false;
                }
                if(histoName !=string("")){
                        if(dir2->Get(histoName.c_str())==0){
                                cerr<<"AllDatasetUncertainties::LoadEffiencies:: the histo "<<histoName<<" was not found in the file "<<fin->GetName()<<" in the subdirectory "<<uncertainties[i].GetDataset()<<endl;
                                delete dir;
                                delete dir2;
                                return false;
                        }
                        if(NofDim==1) uncertainties[i].LoadEfficiencies((TH1F*) dir2->Get(histoName.c_str())->Clone());
                        if(NofDim==2) uncertainties[i].LoadEfficiencies((TH1F*) dir2->Get(histoName.c_str())->Clone());
                        if(NofDim==3) uncertainties[i].LoadEfficiencies((TH1F*) dir2->Get(histoName.c_str())->Clone());
                }
                else{
                        for(int i=0;i<dir2->GetListOfKeys()->GetSize();i++){
                                if(NofDim==1 && dir2->GetListOfKeys()[i].IsA()->InheritsFrom("TH1")){
                                        uncertainties[i].LoadEfficiencies((TH1F*) dir2->Get(dir2->GetListOfKeys()[i].GetName())->Clone());
                                        break;
                                }
                                if(NofDim==2 && dir2->GetListOfKeys()[i].IsA()->InheritsFrom("TH1")){
                                        uncertainties[i].LoadEfficiencies((TH2F*) dir2->Get(dir2->GetListOfKeys()[i].GetName())->Clone());
                                        break;
                                }
                                if(NofDim==3 && dir2->GetListOfKeys()[i].IsA()->InheritsFrom("TH1")){
                                        uncertainties[i].LoadEfficiencies((TH3F*) dir2->Get(dir2->GetListOfKeys()[i].GetName())->Clone());
                                        break;
                                }
                        }
                }
        }
        delete dir;
        delete dir2;
        return true;
}

void AllDatasetUncertainties::LoadEfficiencies(const int& idataset, TH1F* histo){
        if(idataset>=0 && idataset<(int)uncertainties.size()) uncertainties[idataset].LoadEfficiencies(histo);
}

        
void AllDatasetUncertainties::LoadEfficiencies(const int& idataset, TH2F* histo){
        if(idataset>=0 && idataset<(int)uncertainties.size()) uncertainties[idataset].LoadEfficiencies(histo);
}
        
void AllDatasetUncertainties::LoadEfficiencies(const int& idataset, TH3F* histo){
        if(idataset>=0 && idataset<(int)uncertainties.size()) uncertainties[idataset].LoadEfficiencies(histo);
}
        
vector<TH1F*> AllDatasetUncertainties::GetEfficiencies1D() const{
        vector<TH1F*> histos;
        for(unsigned int i=0;i<uncertainties.size();i++){
                histos.push_back(uncertainties[i].GetEfficiencies1D());
        }
        return histos;
}
        
vector<TH2F*> AllDatasetUncertainties::GetEfficiencies2D() const{
        vector<TH2F*> histos;
        for(unsigned int i=0;i<uncertainties.size();i++){
                histos.push_back(uncertainties[i].GetEfficiencies2D());
        }
        return histos;
}
        
vector<TH3F*> AllDatasetUncertainties::GetEfficiencies3D() const{
        vector<TH3F*> histos;
        for(unsigned int i=0;i<uncertainties.size();i++){
                histos.push_back(uncertainties[i].GetEfficiencies3D());
        }
        return histos;
}
        
TH1F* AllDatasetUncertainties::GetEfficiencies1D(const int& idataset) const{
        if(idataset>=0 && idataset<(int)uncertainties.size()) return uncertainties[idataset].GetEfficiencies1D();
        return 0;
}
        
TH2F* AllDatasetUncertainties::GetEfficiencies2D(const int& idataset) const{
        if(idataset>=0 && idataset<(int)uncertainties.size()) return uncertainties[idataset].GetEfficiencies2D();
        return 0;
}
        
TH3F* AllDatasetUncertainties::GetEfficiencies3D(const int& idataset) const{
        if(idataset>=0 && idataset<(int)uncertainties.size()) return uncertainties[idataset].GetEfficiencies3D();
        return 0;
}
