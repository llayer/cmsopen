#include "../interface/Uncertainty.h"

        
Uncertainty::Uncertainty(){
        param = new float[3];
        param[0] = 1.;
        param[1] = 1.;
        param[2] = 1.;
        eff = new float[3];
        InitEfficiencies();
        name = string("");
        dataset = string("");
        NofDim = 0;
        hEfficiencies1D = 0;
        hEfficiencies2D = 0;
        hEfficiencies3D = 0;
}

Uncertainty::Uncertainty(const string& Name, const string& Dataset, const int& nofDim){
        name = Name;
        dataset = Dataset;
        NofDim = nofDim;
}

Uncertainty::~Uncertainty(){
        delete[] eff;
        delete[] param;
        //delete hEfficiencies1D;
        //delete hEfficiencies2D;
        //delete hEfficiencies3D;
}

Uncertainty::Uncertainty(const Uncertainty& u){
        param = new float[3];
        param[0] = u.param[0];
        param[1] = u.param[1];
        param[2] = u.param[2];
        eff = new float[3];
        eff[0] = u.eff[0];
        eff[1] = u.eff[1];
        eff[2] = u.eff[2];
        name = u.name;
        dataset = u.dataset;
        NofDim = u.NofDim;
        /*
        if(u.hEfficiencies1D) hEfficiencies1D = (TH1F*) u.hEfficiencies1D->Clone();
        if(u.hEfficiencies2D) hEfficiencies2D = (TH2F*) u.hEfficiencies2D->Clone();
        if(u.hEfficiencies3D) hEfficiencies3D = (TH3F*) u.hEfficiencies3D->Clone();
        */
}

void Uncertainty::InitEfficiencies(){
        eff[0] = 1.;
        eff[1] = 1.;
        eff[2] = 1.;
}

void Uncertainty::LoadParameters(const float& mean, const float& m1sigma, const float& p1sigma){
        param[0] = mean;
        param[1] = m1sigma;
        param[2] = p1sigma;
}
        
void Uncertainty::LoadParameters(float* Param){
        param[0] = Param[0];
        param[1] = Param[1];
        param[2] = Param[2];
}
        
void Uncertainty::Fill(const int& icond, const float& weight){
        if(icond>=0 && icond<3)
                eff[icond]+=weight;
}       

void Uncertainty::Compute(){
        if(eff[0]<=0) return;
        eff[1] = eff[1]/eff[0];
        eff[2] = eff[2]/eff[0];
        eff[0] = 1.;
}
        
void Uncertainty::LoadEfficiencies(const float& eff_nom, const float& eff_m1sigma, const float& eff_p1sigma){
        eff[0] = eff_nom;
        eff[1] = eff_m1sigma;
        eff[2] = eff_p1sigma;
}
        
void Uncertainty::LoadEfficiencies(const float& eff_nom, const float& eff_pm1sigma){
        eff[0] = eff_nom;
        eff[1] = eff_pm1sigma;
        eff[2] = eff_pm1sigma;
}
        
void Uncertainty::LoadEfficiencies(float* Eff){ for(int i=0;i<2;i++) eff[i] = Eff[i];}
        
float* Uncertainty::GetParameters() const {return param;}
        
float* Uncertainty::GetEfficiencies() const {return eff;}
        
void Uncertainty::Print(std::ostream & os){
        os<<" --- Uncertainty "<<name<<" for dataset "<<dataset<<endl;
        os<<"  Parameters: "<<param[0]<<" & "<<param[1]<<" & "<<param[2]<<endl;
        os<<"  Efficiencies: nominal: "<<eff[0]<<" -1sigma: "<<eff[1]<<" +1sigma: "<<eff[2]<<endl;
}

        
void Uncertainty::SetName(const string& Name) { name = Name;}
        
string Uncertainty::GetName() const {return name;}
        
void Uncertainty::SetDataset(const string& datasetName) {dataset = datasetName;}

string Uncertainty::GetDataset() const {return dataset;};
        
void Uncertainty::SetNofDimensions(const int& nofDim){
        NofDim = nofDim;
}
        
int Uncertainty::GetNofDimensions() const{
        return NofDim;
}

//--- Accessors to histo ---//
        
void Uncertainty::LoadEfficiencies(TH1F* histo){
        hEfficiencies1D = (TH1F*) histo->Clone();
}
        
void Uncertainty::LoadEfficiencies(TH2F* histo){
        hEfficiencies2D = (TH2F*) histo->Clone();
}
        
void Uncertainty::LoadEfficiencies(TH3F* histo){
        hEfficiencies3D = (TH3F*) histo->Clone();
}
        
TH1F* Uncertainty::GetEfficiencies1D() const{
        return hEfficiencies1D;
}
        
TH2F* Uncertainty::GetEfficiencies2D() const{
        return hEfficiencies2D;
}
        
TH3F* Uncertainty::GetEfficiencies3D() const{
        return hEfficiencies3D;
}



