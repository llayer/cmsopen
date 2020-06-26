#ifndef SFBweight_h
#define SFBweight_h

#include "../../../../MiniTreeFormat/NTFormat/interface/NTEvent.h"

// system include files
#include <memory>
#include <assert.h>
#include <typeinfo>
#include <vector>
#include <map>
#include <sstream>
#include <fstream>
#include "TROOT.h"
#include "TCanvas.h"
#include "TH2D.h"
#include "TGraph.h"
#include "TFile.h"



using namespace std;
using namespace TopTree;

class SFBweight {

   public:
        SFBweight();
        SFBweight(const SFBweight& sfw);
        SFBweight(int, float, int );
        ~SFBweight();

        void SFBinit(int, float, int );
        void LoadInfo();
        void LoadInfo2();
        void InitAlgoAndWP(int algo, float wp);
        float GetWeight(int, int, float, float) const;
        float GetWeight_BTV003(int info,  int quarkorigin, float pt, float eta, int sys) const;
        float GetWeight_JP_BTV003(int info,  int quarkorigin, float pt, float eta, int sys) const;
        
        vector<float> GetWeigth4BSel(int,  int, const std::vector<NTJet> &  selJets) const;
        vector<float> GetWeigth4BSel(int method_b,  int syst_b, const std::vector<NTJet> &  selJets, float sf_val_for_b, float sf_val_for_l) const;
        const TH2D* GetHistoSFB() const;
        const TH2D* GetHistoEffMCb() const;
        const TH2D* GetHistoEffMCc() const;
        const TH2D* GetHistoEffMCl() const;

        int Test() const {return map_effmcc_.size();};

   private:
        std::string method_origin1_;
        std::string method_origin2_;



        TH2D* histo_sfvalb_;
        TH2D* histo_sferrb_;
        TH2D* histo_sfvall_;
        TH2D* histo_sferrl_;

        TH2D* histo_effvalb_;
        TH2D* histo_efferrb_;
        TH2D* histo_effvall_;
        TH2D* histo_efferrl_;

        //access to the current histo for the algo/wp initialized

        TH2D* histo_effmcb_;
        TH2D* histo_effmcc_;
        TH2D* histo_effmcl_;

        TH2D* histo_errmcb_;
        TH2D* histo_errmcc_;
        TH2D* histo_errmcl_;
                
        //map containing all the algo/wp

        map<string,TH2D*> map_effmcb_;
        map<string,TH2D*> map_effmcc_;
        map<string,TH2D*> map_effmcl_;

        map<string,TH2D*> map_errmcb_;
        map<string,TH2D*> map_errmcc_;
        map<string,TH2D*> map_errmcl_;
        

        int btag_algo_;
        float btag_discri_;
        int n_bjets_;

};

#endif


