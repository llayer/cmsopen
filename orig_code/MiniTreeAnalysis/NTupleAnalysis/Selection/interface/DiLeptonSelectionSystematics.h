#ifndef DiLeptonSelectionSystematics_h
#define DiLeptonSelectionSystematics_h

#include "DiLeptonSelection.h"


class DiLeptonSelectionSystematics: public DiLeptonSelection{
        

        public:

        DiLeptonSelectionSystematics(const DiLeptonSelection& sel);
        ~DiLeptonSelectionSystematics();

        void LoadSystematics(string systName, float value);
        vector<string> RecognizedSystematics() {return recognizedSyst;};

        void Clear();
        bool ApplySelection(Dataset& dataset, string ChannelName); //returns true if selected
        
        vector<NTJet> GetSelJets() const {return SelJets;};
        vector<NTElectron> GetSelElectrons() const {return SelElectrons;};
        vector<NTMuon> GetSelMuons() const {return SelMuons;};
        vector<NTElectron> GetCandElectrons() const {return CandElectrons;};
        vector<NTMuon> GetCandMuons() const {return CandMuons;};
        int GetJetMultiplicity() const; //bin 4 is inclusive
        int GetNofBtaggedJets() const {return NofBtaggedJets;};
        vector<float>  GetBtagWeight() const {return btagWeight;};
        vector<float> RecomputeBtagWeight(int algo, float btagDiscriCut);

        private:

        vector<string> recognizedSyst;

        bool doJES;
        bool doJER;
        bool doMES;
        bool doEES;
        bool doMETS;
        bool dosfb;
        bool dosfl;

        float JES;
        float JER;
        float MES;
        float EES;
        float METS;
        float sfb;
        float sfl;

        vector<NTElectron> SelElectrons;
        vector<NTMuon> SelMuons;
        vector<NTElectron> CandElectrons;
        vector<NTMuon> CandMuons;
        string CandType;
        vector<NTJet> SelJets;
        int NofBtaggedJets;
        vector<float> btagWeight;
};

#endif
