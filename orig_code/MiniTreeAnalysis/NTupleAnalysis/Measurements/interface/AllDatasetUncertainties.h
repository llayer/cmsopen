#ifndef AllDatasetUncertainties_h
#define AllDatasetUncertainties_h

#include <vector>
#include "Uncertainty.h"
#include "../../Tools/interface/Dataset.h"

using namespace std;


/**
        This class is dedicated to represent an uncertainty (with a name) for a list of processes (datasets).\n
        It is based on the class Uncertainty, cf the description of this class.
        List of actions:
        - Constructor(name)
        - LoadDatasets()
        - SetParameters() [optional]
        - [ Fill() & Compute() ] or LoadEfficiencies()
*/

class AllDatasetUncertainties{
  public:
        AllDatasetUncertainties();
        AllDatasetUncertainties(const string& name, const int& NofDim = 0);
        ~AllDatasetUncertainties();

        void LoadDatasets(const vector<Dataset>& datasets);
        void Fill(const int& idataset, const int& icond = 0, const float& weight = 1.); /** used once per event (with a weight) per dataset if you fill the cut you're looking at  - icond = 0 [nominal] or 1 [-1sigma] or 2 [+1sigma]*/
        void Compute(); /** Will Compute the parameters accoding to what you filled */
        void SetParameters(const float& paramMean, const float& paramMinus, const float& paramPlus); /** to be used when the parameters are the same for all the datasets */
        void SetParameters(const int& idataset, const float& paramMean, const float& paramMinus, const float& paramPlus); /** to be used to specify different paramters for a the datasets */
        void LoadEfficiencies(const int& idataset, const float& eff_nom, const float& eff_m1sigma, const float& eff_p1sigma);
        vector<float*> GetParameters() const;
        vector<float*> GetEfficiencies() const; 
        float* GetParameters(int idataset) const;
        float* GetEfficiencies(int idataset) const;


        //--- Accessors to histo ---//
        bool LoadEffiencies(TFile* fin, string dirnamem = string(""), string histoName = string(""));//** function which directly look inside a TFile, under a directory and search inside subdirectories having the name of a dataset [loop over datasets] a histo of dim = NofDim and then load them **// 
        void LoadEfficiencies(const int& idataset, TH1F* histo);
        void LoadEfficiencies(const int& idataset, TH2F* histo);
        void LoadEfficiencies(const int& idataset, TH3F* histo);
        vector<TH1F*> GetEfficiencies1D() const;
        vector<TH2F*> GetEfficiencies2D() const;
        vector<TH3F*> GetEfficiencies3D() const;
        TH1F* GetEfficiencies1D(const int& idataset) const;
        TH2F* GetEfficiencies2D(const int& idataset) const;
        TH3F* GetEfficiencies3D(const int& idataset) const;

        vector<string> GetListOfDatasets() const;
        string GetName() const;
        void SetName(const string& name);
        void SetNofDimensions(const int& NofDim);
        int GetNofDimensions() const;

  private:
        string name;
        int NofDim;
        vector<bool> effLoaded; // bool = true if the efficiencies are loaded from the method LoadEfficiencies
        vector<Uncertainty> uncertainties;

};

#endif
