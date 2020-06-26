#include "../interface/ChannelDescription.h"

        
ChannelDescription::ChannelDescription(){
        BR = 1;
}
        
ChannelDescription::ChannelDescription(string channelName, string signalDatasetName, float br ){
        ChannelName = channelName;
        SignalDatasetName = signalDatasetName;
        BR = br;
}
        
ChannelDescription::ChannelDescription(const ChannelDescription& cd){
        ChannelName = cd.ChannelName;
        SignalDatasetName = cd.SignalDatasetName;
        BR = cd.BR;
        isBRASyst_ = cd.isBRASyst_;
        Efficiencies = cd.Efficiencies; 
        EffUncertainties = cd.EffUncertainties; 
        BkgEstimations = cd.BkgEstimations;
        distribs = cd.distribs;         
}
        
ChannelDescription::~ChannelDescription(){
}


void ChannelDescription::SetIsBRASyst(bool flag){
        isBRASyst_ = flag;
}

bool ChannelDescription::isBRASyst() const{
        return isBRASyst_;
}

void ChannelDescription::SetChannelName(const string& name){
        ChannelName = name;
}
        
string ChannelDescription::GetChannelName() const {return ChannelName;}
        
void ChannelDescription::SetSignalDatasetName(const string& datasetName){
        SignalDatasetName = datasetName;
}
        
string ChannelDescription::GetSignalDatasetName() const {return SignalDatasetName;}
        
void ChannelDescription::SetBR(const float& br){
        BR = br;
}
        
float ChannelDescription::GetBR()const{return BR;}


void ChannelDescription::AddHisto(const string& name, const string& channel, const int& dim, const vector<bool>& datasetIsDD){
        DistributionDescription dd(name,channel,dim, datasetIsDD);
        distribs.push_back(dd);
}

void ChannelDescription::LoadDistributions(TFile* fin, const vector<Dataset>& datasets, const vector<Parameter>& uncert, const vector<Parameter>& GraphUncert, const vector<Parameter>& estim, const vector<Parameter>& GraphEstim){
        for(unsigned int i=0;i<distribs.size();i++){
                distribs[i].LoadHistos(fin, datasets, uncert, GraphUncert, estim, GraphEstim);
        }

}

vector<DistributionDescription> ChannelDescription::GetHistos(){
        return distribs;
}

vector<DistributionDescription>& ChannelDescription::GetHistosAddress(){
        return distribs;
}

void ChannelDescription::AddBkgEstimation(const DDEstimation& dd){
        BkgEstimations.push_back(dd);
}
