
#include "../interface/DiLeptonSelectionSystematics.h"

DiLeptonSelectionSystematics::DiLeptonSelectionSystematics(const DiLeptonSelection& sel):DiLeptonSelection(sel){
        recognizedSyst.push_back(string("JES"));
        recognizedSyst.push_back(string("JER"));
        recognizedSyst.push_back(string("MES"));
        recognizedSyst.push_back(string("EES"));
        recognizedSyst.push_back(string("METS"));

        doJES = false;
        doJER = false;
        doMES = false;
        doEES = false;
        doMETS = false;
        dosfb = false;
        dosfl = false;

        JES = 1.;
        JER = 1.;
        MES = 1.;
        EES = 1.;
        METS = 1.;
        sfb = 1.;
        sfl = -1.;

        NofBtaggedJets = 0;
}

DiLeptonSelectionSystematics::~DiLeptonSelectionSystematics(){
}


void DiLeptonSelectionSystematics::Clear(){
        doJES = false;
        doJER = false;
        doMES = false;
        doEES = false;
        doMETS = false;
        dosfb = false;
        dosfl = false;

        JES = 1.;
        JER = 1.;
        MES = 1.;
        EES = 1.;
        METS = 1.;
        sfb = 1.;
        sfl = -1.;

        NofBtaggedJets = 0;

        SelElectrons.clear();
        SelMuons.clear();
        CandElectrons.clear();
        CandMuons.clear();
        CandType = string("");
        SelJets.clear();
        btagWeight.clear();
}


void DiLeptonSelectionSystematics::LoadSystematics(string systName, float value){
        if(systName == string("JES")){
                doJES = true;
                JES   = value;  
        }
        if(systName == string("JER")){
                doJER = true;
                JER   = value;  
        }
        if(systName == string("MES")){
                doMES = true;
                MES   = value;  
        }
        if(systName == string("EES")){
                doEES = true;
                EES   = value;  
        }
        if(systName == string("METS")){
                doMETS = true;
                METS   = value; 
        }
        if(systName == string("sfb")){
                dosfb = true;
                sfb   = value;
        }       
        if(systName == string("sfl")){
                dosfl = true;
                sfl   = value;
        }       
}


int DiLeptonSelectionSystematics::GetJetMultiplicity() const{
        if(SelJets.size()>=4) return 4;
        return (int) SelJets.size(); 
}
        

vector<float> DiLeptonSelectionSystematics::RecomputeBtagWeight(int algo, float btagDiscriCut){
      int flag = GetFlagb();    
      int methodb = GetMethodb();
      int systb = GetSystb(); 
      int btagNjet = GetNofBtagJetsCut();
      ReInitSFBWeight(flag, methodb, systb, algo, btagDiscriCut, btagNjet);

      if (GetFlagb() == 1) {
           btagWeight = GetSFBweight ().GetWeigth4BSel (GetMethodb (), GetSystb (), SelJets, sfb, sfl);
      }
      return btagWeight;
}

bool DiLeptonSelectionSystematics::ApplySelection(Dataset& dataset, string ChannelName){
        //Clear();
        //don't do Clear !!!
        SelElectrons = GetSelectedElectrons(doEES, EES);        
        SelMuons = GetSelectedMuons(doMES, MES);
        GetLeptonPair(SelMuons, SelElectrons, CandMuons, CandElectrons, CandType);
        TLorentzVector pair;
        for(unsigned int i=0;i<CandMuons.size();i++) pair+=CandMuons[i].p4;     
        for(unsigned int i=0;i<CandElectrons.size();i++) pair+=CandElectrons[i].p4;     
        //After pairing
        SelJets = GetSelectedJets(CandMuons,CandElectrons,doJES, JES, doJER, JER );     

        //b-tagging     
        int AlgoBtag = GetbtagAlgo ();
        NofBtaggedJets = 0;
        for (unsigned int j = 0; j < SelJets.size (); j++) {
             /*
             if (AlgoBtag == 0 && SelJets[j].TCDiscri >= GetbtagDiscriCut ())
                NofBtaggedJets++;
             if (AlgoBtag == 1 && SelJets[j].SVDiscri >= GetbtagDiscriCut ())
                NofBtaggedJets++;
             if (AlgoBtag == 2 && SelJets[j].SMDiscri >= GetbtagDiscriCut ())
                NofBtaggedJets++;
             */
                //cout<<"AlgoBtag=="<<AlgoBtag<<" "<<GetbtagDiscriCut()<<endl;
             if ( AlgoBtag==0 &&  SelJets[j].GetDiscri(string("trackCountingHighEffBJetTags"))>=GetbtagDiscriCut ()) NofBtaggedJets++;
             if ( AlgoBtag==1 &&  SelJets[j].GetDiscri(string("simpleSecondaryVertexHighEffBJetTags"))>=GetbtagDiscriCut ()) NofBtaggedJets++;
             if ( AlgoBtag==2 &&  SelJets[j].GetDiscri(string("trackCountingHighPurBJetTags"))>=GetbtagDiscriCut ()) NofBtaggedJets++;
             if ( AlgoBtag==3 &&  SelJets[j].GetDiscri(string("simpleSecondaryVertexHighPurBJetTags"))>=GetbtagDiscriCut ()) NofBtaggedJets++;
             if ( AlgoBtag==4 &&  SelJets[j].GetDiscri(string("jetProbabilityBJetTags"))>=GetbtagDiscriCut ()) NofBtaggedJets++;
             if ( AlgoBtag==5 &&  SelJets[j].GetDiscri(string("jetBProbabilityBJetTags"))>=GetbtagDiscriCut ()) NofBtaggedJets++;
             if ( AlgoBtag==6 &&  SelJets[j].GetDiscri(string("combinedSecondaryVertexBJetTags"))>=GetbtagDiscriCut ()) NofBtaggedJets++;
        }
        //with weight
        //float sfb = 1.;
        //float sfl = 1.;
        //cout<<GetMethodb ()<<" "<<GetSystb ()<<" "<<sfb<<" "<<sfl<<endl;
// modifdg
      if (GetFlagb() == 1) {
        if (!dataset.isData ()) {       //MC
//      std::cout<<"je passe 99 "<<GetFlagb() <<" "<<!dataset.isData ()<<std::endl;             
           btagWeight = GetSFBweight ().GetWeigth4BSel (GetMethodb (), GetSystb (), SelJets, sfb, sfl);
        //btagWeight = GetSFBweight ().GetWeigth4BSel (GetMethodb (), GetSystb (), GetSelectedJets());//, sfb, sfl);
        }
      } 
// endmodifdg

        //Selection itself
        bool selected = false;
        //trigger
        if(passTriggerSelection (&dataset, ChannelName)){
                //pair
                if ( ((CandType=="mumu" && ChannelName=="mumu")   || 
                (CandType=="ee"   && ChannelName=="ee")     ||
                (CandType=="emu"  && ChannelName=="emu")     )
                && GetSelectedVertex().size()>0 
                 && pair.M()> GetMinValueMassCut() ) {
                        //Z-veto
                        if ( ( (ChannelName=="mumu" || ChannelName=="ee") &&
                         DiLeptonMassCut(GetMinValueMassCut(),GetZmassWindowCut(),CandMuons,CandElectrons,ChannelName) ) ||
                        (ChannelName=="emu" && pair.M()> GetMinValueMassCut()))  {
                                //Jets
                                if(SelJets.size()>=2){
                                        //MET
                                        if ( ((ChannelName=="mumu" || ChannelName=="ee") &&  GetMET().p4.Et()>GetMETCut().second) || (ChannelName=="emu"  &&  GetMET().p4.Et()>GetMETCut().first)    )  {
                                                selected = true;
                                        }
                                }
                        }
                }
        }

        return selected;
}



