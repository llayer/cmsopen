#ifndef Uncertainty_h
#define Uncertainty_h

#include <iostream>

#include <TH1F.h>
#include <TH2F.h>
#include <TH3F.h>

/**
        This class is dedicated to represent an uncertainty [or nuisance parameter] (with a name) for a given process (dataset).\n
        The parameters (param) are the mean and the +/- 1 sigma associated to the observable/effect related to the uncertainty (ex: JES: 0.9-1.0-1.1). \n
        These values have no incluence on the calculation but are just store to be used in future display \n
        The efficiencies (eff) are in fact ratio of efficiencies and not efficiencies directly. \n
        They are the ratio of the number of events at a certain selection level between +/- 1 sigma and the nominal case  \n
        There is different Load* functions.
        The efficiencies can be computed using Fill() and Compute() or can be loaded.
        The notion has been extented from a single description of a measurement to distribution at 1,2,3 dimensions.
        NofDim = 0: single value
                 1,2,3: 1,2,3 D histo

        NB: the efficiencies are in fact ratio between a #events at +/- 1 sigma and the #events in the nominal case. It can be higher than 1.
        eff[0] should be == 1.

                 
*/

using namespace std;


class Uncertainty{
  
  public:

        Uncertainty();
        Uncertainty(const string& name, const string& dataset, const int& NofDim = 0);
        Uncertainty(const Uncertainty& u);
        ~Uncertainty();

        void InitEfficiencies();/** reset efficiencies to 0 */
        void LoadParameters(const float& mean, const float& m1sigma, const float& p1sigma);
        void LoadParameters(float* param);
        void Fill(const int& icond = 0, const float& weight = 1.); /** used once per event (with a weight) if you fill the cut you're looking at  - icond = 0 [nominal] or 1 [-1sigma] or 2 [+1sigma]*/
        void Compute(); /** Will compute the efficiencies according to what you filled */
        void LoadEfficiencies(const float& eff_nom, const float& eff_m1sigma, const float& eff_p1sigma);
        void LoadEfficiencies(const float& eff_nom, const float& eff_pm1sigma);/** for symetric uncertainty*/
        void LoadEfficiencies(float* eff);
        float* GetParameters() const;
        float* GetEfficiencies() const;
        void Print(std::ostream & os = std::cout);

        void SetName(const string& name);
        string GetName()const;
        void SetDataset(const string& dataset);
        string GetDataset()const;
        void SetNofDimensions(const int& NofDim);
        int GetNofDimensions() const;

        //--- Accessors to histo ---//
        void LoadEfficiencies(TH1F* histo);
        void LoadEfficiencies(TH2F* histo);
        void LoadEfficiencies(TH3F* histo);
        TH1F* GetEfficiencies1D() const;
        TH2F* GetEfficiencies2D() const;
        TH3F* GetEfficiencies3D() const;


  private:
        string name;
        string dataset;
        int NofDim; //0: a single value, 1/2/3: a 1/2/3 histo

        float* param;// mean & +/- 1 sigma
        float* eff;// [0]: nominal - [1]: -1 sigma - [2]: +1 sigma
        
        //Extension of the concept to a distribution at 1,2,3 dimensions
        TH1F* hEfficiencies1D;
        TH2F* hEfficiencies2D;
        TH3F* hEfficiencies3D;

};

#endif
