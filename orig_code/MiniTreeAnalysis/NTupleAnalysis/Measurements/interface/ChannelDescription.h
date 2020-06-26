#ifndef ChannelDescription_h
#define ChannelDescription_h
#include <string>
#include <vector>
#include "DistributionDescription.h"
//#include "DiLepXsectionMeas.h"

using namespace std;


class ChannelDescription{
        
  public:       
        ChannelDescription();
        ChannelDescription(string ChannelName, string SignalDatasetName, float BR );
        ChannelDescription(const ChannelDescription& cd);
        ~ChannelDescription();


        //--- accessors to generic information ---//
        void SetChannelName(const string& name);
        string GetChannelName() const;
        void SetSignalDatasetName(const string& datasetName);
        string GetSignalDatasetName() const;
        void SetBR(const float& br);
        float GetBR()const;
        void SetIsBRASyst(bool flag);
        bool isBRASyst() const;

        //--- Histos ---//
        vector<DistributionDescription> GetHistos();
        vector<DistributionDescription>& GetHistosAddress();
        
        vector<DDEstimation>& GetBkgEstimation() {return BkgEstimations;};

        //--- Load functions: to be implemented ---//
        void AddBkgEstimation(const DDEstimation& dd);
        void AddHisto(const string& name, const string& channel, const int& dim, const vector<bool>& datasetIsDD);
        void LoadDistributions(TFile* fin, const vector<Dataset>& datasets, const vector<Parameter>& uncert, const vector<Parameter>& GraphUncert, const vector<Parameter>& estim, const vector<Parameter>& GraphEstim);
        void LoadBkgEstimations();


  private:

        string ChannelName;
        string SignalDatasetName;
        float BR;
        bool isBRASyst_;

        //--- This is only used if there is no distributions ---//      
        vector<double> Efficiencies; //** One per dataset - size = #datasets **//
        vector<AllDatasetUncertainties> EffUncertainties; //** Uncertainties on efficiencies due to nuisance parameters **//

        vector<DDEstimation> BkgEstimations;

        vector<DistributionDescription> distribs;               


};

#endif
