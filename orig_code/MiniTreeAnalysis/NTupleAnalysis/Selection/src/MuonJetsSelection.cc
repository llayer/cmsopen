#include "../interface/MuonJetsSelection.h"

                
MuonJetsSelection::MuonJetsSelection(){
      MinMassCut_ = 0.;
      ZMassWindow_ = pair<float,float>(9999.,0.);
      METCuts_     = pair<float,float>(9999.,0.);
      btagAlgo_ = -1;
      btagDiscriCut_ = -999.;
      NofBtagJets_ = 0;
      //Fill the table of cuts
      cuts_.push_back("All"); 
      cuts_.push_back("All dilept"); 
      cuts_.push_back("Trigger"); 
      cuts_.push_back("DiLepton pair");
      cuts_.push_back("Z mass veto");
      cuts_.push_back("NJets cut");
      cuts_.push_back("MET cuts");
      cuts_.push_back("NbtagJets cut");
      //Fill Channels
      channels_.push_back(string("ee"));
      channels_.push_back(string("emu"));
      channels_.push_back(string("mumu"));
}


int MuonJetsSelection::GetChannel(string& CandPairType){
        for(unsigned int i=0;i<channels_.size();i++){
                if(channels_[i] == CandPairType) return (int)i;
        }
        return -999;
}

MuonJetsSelection::MuonJetsSelection(const MuonJetsSelection & s){
      MinMassCut_    = s.MinMassCut_;
      METCuts_       = s.METCuts_;
      ZMassWindow_   = s.ZMassWindow_;
      cuts_          = s.cuts_;
      btagAlgo_      = s.btagAlgo_;
      btagDiscriCut_ = s.btagDiscriCut_;
      NofBtagJets_   = s.NofBtagJets_;
}
     
MuonJetsSelection::~MuonJetsSelection(){}


void MuonJetsSelection::SetParameters(float MinValue, pair<float,float> METCuts, pair<float,float> ZMassWindow, int btagAlgo, float btagDiscriCut, int NofBtagJets){
        MinMassCut_  = MinValue;
        METCuts_     = METCuts;
        ZMassWindow_ = ZMassWindow;
        btagAlgo_      = btagAlgo;
        btagDiscriCut_ = btagDiscriCut;
        NofBtagJets_   = NofBtagJets;
}


      
int MuonJetsSelection::doFullSelection(Dataset* dataset, string channelName){

        //cout << " is in doFullSelection "  << endl;
        bool applyChannel = false;
        if(channelName != string("") && channelName != string("*") && channelName!=string("allChannels")) applyChannel = true;
        
        bool filterTTbarMC = false;
        if(dataset->Name() == "TTbarMuJet" && applyChannel && ( tmeme == 20  || tmeme == 11000 )  ) filterTTbarMC = true;
        if(dataset->Name() == "TTbarBckMu" && applyChannel && !( tmeme == 20  || tmeme == 11000 ) ) filterTTbarMC = true;
        
        
        int FinalStep = 0;
        
        std::vector<NTMuon>     muon_cand, muon_loose_cand;
        std::vector<NTElectron> elec_loose_cand, elec_cand; 
        //0 no selection
        FinalStep++;
        
        if( 1 == 1  ) {
          //1 trigger selection
            //cout << " passTriggerSelection(dataset->Name()) " << passTriggerSelection(dataset->Name())  << endl;
          if(passTriggerSelection(dataset->Name()) ){
             FinalStep++;
             
             
            //2 good vertex selection
             if(GetSelectedVertex().size() > 0){
             FinalStep++;
             muon_cand        = GetSelectedMuonsForMuJets();
             muon_loose_cand  = GetSelectedLooseMuonsForMuJets();
             elec_loose_cand  = GetSelectedLooseElectronsForMuJets();
             
             //3 exactly one isolated muon
             if(muon_cand.size() == 1){
               FinalStep++;
               //4 loose muon veto 
               if(muon_loose_cand.size() == 1){
                 FinalStep++;
                 //5 loose electron veto
                 if(elec_loose_cand.size() == 0){
                   FinalStep++;
                   //6 >= 1, 2, 3 jets
                   vector<NTJet> SelectedJets = GetSelectedJets(muon_cand, elec_cand);
                   if(SelectedJets.size() >=1) FinalStep++;
                   if(SelectedJets.size() >=2) FinalStep++;
                   if(SelectedJets.size() >=3) FinalStep++;
                   if(SelectedJets.size() >=4) FinalStep++;
                    
                  }
                 }
               }
               
             }
             
            
          }
        
        }
        
        
        // not implemented yet
        return FinalStep;
}

int MuonJetsSelection::FillTable(SelectionTable& selTable, Dataset* dataset, int idataset, float weight){
        int sel = doFullSelection(dataset, selTable.Channel());
        for(unsigned int i=0;i<cuts_.size()+1;i++)
                if(sel >=(int)i ) selTable.Fill(idataset,i, weight);
        return sel;
}


bool MuonJetsSelection::passTriggerSelection(string datasetName){
  
  
  bool passMu            = false;
  
  
   //cout << " test trigger list size  " << triggerList.size() << endl;
  for(unsigned int i=0; i< triggerList.size(); i++){
    if(  (triggerList[i].first == "HLT_Mu9"          && triggerList[i].second == true  ))  passMu = true;
  } 
   
  
  
  return passMu;
  
  
}








