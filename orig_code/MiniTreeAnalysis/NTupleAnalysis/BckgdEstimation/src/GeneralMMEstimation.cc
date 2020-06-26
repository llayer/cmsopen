#include "../interface/GeneralMMEstimation.h"
                
GeneralMMEstimation::GeneralMMEstimation(vector<Dataset> datasets, float isoLoose, float isoTight1, float isoTight2, unsigned int n_bins, float min, float max, string Channel){

      theNBins = n_bins;
      theMinBin = min;
      theMaxBin = max;

      theIsolations.iso1[0] = isoLoose;
      theIsolations.iso2[0] = isoLoose;

      theIsolations.iso1[1] = isoLoose;
      theIsolations.iso2[1] = isoTight2;

      theIsolations.iso1[2] = isoTight1;
      theIsolations.iso2[2] = isoLoose;

      theIsolations.iso1[3] = isoTight1;
      theIsolations.iso2[3] = isoTight2;

      theChannel = Channel;
      //Reinitialized to a value, during RunTheMatrixMethod() call, for each bin and each isolation
      for(unsigned int iso_index=0; iso_index<4; iso_index++){
       locNSelected[iso_index] = 0;
      }

      //Reinitialized to a value, during RunTheMatrixMethod() call, for each bin
      EpsilonEFake = 0;
      EpsilonEFakeErr = 0;
      EpsilonESignal = 0;
      EpsilonESignalErr = 0;

      EpsilonMuFake = 0;
      EpsilonMuFakeErr = 0;
      EpsilonMuSignal = 0;
      EpsilonMuSignalErr = 0;

      //Reinitialized to a value, for each bin, at each IncrementNSetBin() call
      //Then passed bin by bin to RunTheMatrixMethod() call
      theNSelected.reserve(100000);
      for(unsigned int bin_index=0; bin_index<theNBins; bin_index++){
       struct NSelected theNSelElement;
       for(unsigned int iso_index=0; iso_index<4; iso_index++){
         theNSelElement.NSel[iso_index] = 0.;
       }
       theNSelected.push_back(theNSelElement);
      }

      IsoNames[0] = "LL";
      IsoNames[1] = "LT";
      IsoNames[2] = "TL";
      IsoNames[3] = "TT";

      //Reinitialized to a value, during RunTheMatrixMethod() call, for each bin, each isolation and for each iteration a new value is filled
      theDistributions.reserve(100000);
      for(unsigned int bin_index=0; bin_index<theNBins; bin_index++){
       std::stringstream ss;
       ss << bin_index;
       struct Distribution theDistributionElement;
       for(unsigned int iso_index=0; iso_index<4; iso_index++){
        theDistributionElement.NMMEstimatedTTDistribution[iso_index] = new TH1F(("NMMEstimated"+IsoNames[iso_index]+theChannel+"TTDistribution_"+(ss.str())).c_str(), ("NMMEstimated"+IsoNames[iso_index]+theChannel+"TTDistribution_"+(ss.str())).c_str(),  2000, -1000, 1000);
        theDistributionElement.NMMEstimatedTFDistribution[iso_index] = new TH1F(("NMMEstimated"+IsoNames[iso_index]+theChannel+"TFDistribution_"+(ss.str())).c_str(), ("NMMEstimated"+IsoNames[iso_index]+theChannel+"TFDistribution_"+(ss.str())).c_str(),  2000, -1000, 1000);
        theDistributionElement.NMMEstimatedFTDistribution[iso_index] = new TH1F(("NMMEstimated"+IsoNames[iso_index]+theChannel+"FTDistribution_"+(ss.str())).c_str(), ("NMMEstimated"+IsoNames[iso_index]+theChannel+"FTDistribution_"+(ss.str())).c_str(),  2000, -1000, 1000);
        theDistributionElement.NMMEstimatedFFDistribution[iso_index] = new TH1F(("NMMEstimated"+IsoNames[iso_index]+theChannel+"FFDistribution_"+(ss.str())).c_str(), ("NMMEstimated"+IsoNames[iso_index]+theChannel+"FFDistribution_"+(ss.str())).c_str(),  2000, -1000, 1000);
       }
       theDistributions.push_back(theDistributionElement);
      }


      //Initialized only here, once! Then filled elsewhere
      theMMEstimatedValues.reserve(100000);
      for(unsigned int bin_index=0; bin_index<theNBins; bin_index++){
       struct MMEstimated theMMEstimatedValuesElement;        
       for(unsigned int iso_index=0; iso_index<4; iso_index++){
        theMMEstimatedValuesElement.NofMMEstimatedTT[iso_index]= 0.;
        theMMEstimatedValuesElement.MMEstimatedTTErr[iso_index]= 0.;
        theMMEstimatedValuesElement.NofMMEstimatedTF[iso_index]= 0.;
        theMMEstimatedValuesElement.MMEstimatedTFErr[iso_index]= 0.;
        theMMEstimatedValuesElement.NofMMEstimatedFT[iso_index]= 0.;
        theMMEstimatedValuesElement.MMEstimatedFTErr[iso_index]= 0.;
        theMMEstimatedValuesElement.NofMMEstimatedFF[iso_index]= 0.;
        theMMEstimatedValuesElement.MMEstimatedFFErr[iso_index]= 0.;
       }
       theMMEstimatedValues.push_back(theMMEstimatedValuesElement);
      }

      //Initialized only here, once! Then filled elsewhere
      for(unsigned int iso_index=0; iso_index<4; iso_index++){
       theMMEstimatedPlots.MMEstimated_TT[iso_index] = new TH1F(("MMEstimated_"+IsoNames[iso_index]+theChannel+"_TT").c_str(), ("MMEstimated_"+IsoNames[iso_index]+theChannel+"_TT").c_str(), theNBins, theMinBin, theMaxBin);
       theMMEstimatedPlots.MMEstimated_TF[iso_index] = new TH1F(("MMEstimated_"+IsoNames[iso_index]+theChannel+"_TF").c_str(), ("MMEstimated_"+IsoNames[iso_index]+theChannel+"_TF").c_str(), theNBins, theMinBin, theMaxBin);
       theMMEstimatedPlots.MMEstimated_FT[iso_index] = new TH1F(("MMEstimated_"+IsoNames[iso_index]+theChannel+"_FT").c_str(), ("MMEstimated_"+IsoNames[iso_index]+theChannel+"_FT").c_str(), theNBins, theMinBin, theMaxBin);
       theMMEstimatedPlots.MMEstimated_FF[iso_index] = new TH1F(("MMEstimated_"+IsoNames[iso_index]+theChannel+"_FF").c_str(), ("MMEstimated_"+IsoNames[iso_index]+theChannel+"_FF").c_str(), theNBins, theMinBin, theMaxBin);
      }


      //Initialized only here, once! Then filled elsewhere
      theMMExpectedPlots.reserve(100000);
      {
       struct MMExpectedPlots tmpMMExpectedPlot;
       for(unsigned int iso_index=0; iso_index<4; iso_index++){
         tmpMMExpectedPlot.Name[iso_index] = "TTbarSignal";
         TH1F *h = new TH1F(("MMExpected_"+IsoNames[iso_index]+theChannel+"_TTbarSignal").c_str(),("MMExpected_"+IsoNames[iso_index]+theChannel+"_TTbarSignal").c_str(), theNBins, theMinBin, theMaxBin); 
         h->Sumw2();
         tmpMMExpectedPlot.MMExpected[iso_index] = h;
         h = 0;
         delete h;        
       }
       theMMExpectedPlots.push_back(tmpMMExpectedPlot);
      }


      {
       struct MMExpectedPlots tmpMMExpectedPlot;
       for(unsigned int iso_index=0; iso_index<4; iso_index++){
         tmpMMExpectedPlot.Name[iso_index] = "TTbarSemileptonic";
         TH1F *h = new TH1F(("MMExpected_"+IsoNames[iso_index]+theChannel+"_TTbarSemileptonic").c_str(),("MMExpected_"+IsoNames[iso_index]+theChannel+"_TTbarSemileptonic").c_str(), theNBins, theMinBin, theMaxBin); 
         h->Sumw2();
         tmpMMExpectedPlot.MMExpected[iso_index] = h;
         h = 0;
         delete h;        
       }
       theMMExpectedPlots.push_back(tmpMMExpectedPlot);
      }

      for(unsigned int dataset_index=0; dataset_index < datasets.size(); dataset_index++){
       struct MMExpectedPlots tmpMMExpectedPlot;
       for(unsigned int iso_index=0; iso_index<4; iso_index++){
         tmpMMExpectedPlot.Name[iso_index] = datasets[dataset_index].Name();
         TH1F *h = new TH1F(("MMExpected_"+IsoNames[iso_index]+theChannel+"_"+datasets[dataset_index].Name()).c_str(),("MMExpected_"+IsoNames[iso_index]+theChannel+"_"+datasets[dataset_index].Name()).c_str(), theNBins, theMinBin, theMaxBin); 
         h->Sumw2();
         tmpMMExpectedPlot.MMExpected[iso_index] = h;
         h = 0;
         delete h;        
       }
       theMMExpectedPlots.push_back(tmpMMExpectedPlot);
      }

      for(unsigned int iso_index=0; iso_index<4; iso_index++){
       histoN[iso_index] = new TH1F(("histoN"+IsoNames[iso_index]+theChannel).c_str(), ("histoN"+IsoNames[iso_index]+theChannel).c_str(), theNBins, theMinBin, theMaxBin);
       histoTT[iso_index] = new TH1F(("histoTT"+IsoNames[iso_index]+theChannel).c_str(), ("histoTT"+IsoNames[iso_index]+theChannel).c_str(), theNBins, theMinBin, theMaxBin);
       histoTF[iso_index] = new TH1F(("histoTF"+IsoNames[iso_index]+theChannel).c_str(), ("histoTF"+IsoNames[iso_index]+theChannel).c_str(), theNBins, theMinBin, theMaxBin);
       histoFT[iso_index] = new TH1F(("histoFT"+IsoNames[iso_index]+theChannel).c_str(), ("histoFT"+IsoNames[iso_index]+theChannel).c_str(), theNBins, theMinBin, theMaxBin);
       histoFF[iso_index] = new TH1F(("histoFF"+IsoNames[iso_index]+theChannel).c_str(), ("histoFF"+IsoNames[iso_index]+theChannel).c_str(), theNBins, theMinBin, theMaxBin);
      }


}

GeneralMMEstimation::~GeneralMMEstimation(){

      for(unsigned int bin_index=0; bin_index<theNBins; bin_index++){
        for(unsigned int iso_index=0; iso_index<4; iso_index++){
         delete theDistributions[bin_index].NMMEstimatedTTDistribution[iso_index];
         delete theDistributions[bin_index].NMMEstimatedTFDistribution[iso_index];
         delete theDistributions[bin_index].NMMEstimatedFTDistribution[iso_index];
         delete theDistributions[bin_index].NMMEstimatedFFDistribution[iso_index];
       }
      }


      for(unsigned int iso_index=0; iso_index<4; iso_index++){
       delete theMMEstimatedPlots.MMEstimated_TT[iso_index];
       delete theMMEstimatedPlots.MMEstimated_TF[iso_index];
       delete theMMEstimatedPlots.MMEstimated_FT[iso_index];
       delete theMMEstimatedPlots.MMEstimated_FF[iso_index];
      }

      for(unsigned int iso_index=0; iso_index<4; iso_index++){
       for(unsigned int i=0;i< theMMExpectedPlots.size(); i++){
         delete theMMExpectedPlots[i].MMExpected[iso_index];
       }
      }

      for(unsigned int iso_index=0; iso_index<4; iso_index++){
       delete histoN[iso_index];
       delete histoTT[iso_index];
       delete histoTF[iso_index];
       delete histoFT[iso_index];
       delete histoFF[iso_index];
      }


}


struct Isolations GeneralMMEstimation::GetIsolations(){
  return theIsolations;
}


void GeneralMMEstimation::IncrementNSelBin(unsigned int iso_index, unsigned int bin_index, float weight){
         theNSelected[bin_index].NSel[iso_index] += weight;
}

void GeneralMMEstimation::CountNSel(const DiLeptonSelection & sel, Dataset dataset, SelectionTable selTable, unsigned int d, float weight, string CandTypeRequested, int selStepCut){

    int selStepMM[4]= {-1, -1, -1, -1};
    string CandTypeForMM[4]= {"", "", "", ""};
    unsigned int njets[4] = {-1, -1, -1, -1};


    for (unsigned int iso_index = 0; iso_index<4; iso_index++) {
        vector<NTJet> SelectedJetsForMM; bool LepPairForMM[4]; vector<NTElectron> candElecForMM; vector<NTMuon> candMuonForMM;
        DiLeptonSelection sel_aux(sel);
        selStepMM[iso_index] = sel_aux.FillTableForMM(GetIsolations().iso1[iso_index],GetIsolations().iso2[iso_index], selTable, &(dataset), d, weight);         
        // Be careful if inverted or not (at the moment pairing is applied before isolation)
        LepPairForMM[iso_index] = sel_aux.GetLeptonPairForMM(GetIsolations().iso1[iso_index],GetIsolations().iso2[iso_index], candMuonForMM, candElecForMM, CandTypeForMM[iso_index]);
        SelectedJetsForMM = sel_aux.GetSelectedJets(candMuonForMM, candElecForMM);
        njets[iso_index] = SelectedJetsForMM.size(); //if(iso_index == 3){njets[3]=SelectedJetsForMM.size(); njets[2]=SelectedJetsForMM.size(); njets[1]=SelectedJetsForMM.size(); njets[0]=SelectedJetsForMM.size();}
    }
    //    for (unsigned int iso_index = 0; iso_index<3; iso_index++) {
    //        CandTypeForMM[iso_index] = CandTypeForMM[3];
    //    }

     for(unsigned int bin_index = 0; bin_index < theNBins; bin_index++){
        //Here, njets[] is the variable vs. whih the plots of Matrix Method will be done. It could be any other variable!     
       if((CandTypeForMM[3] == CandTypeRequested) && (selStepMM[3] >= selStepCut)  && ((njets[3] >= (((theMaxBin-theMinBin)/theNBins)*bin_index)) && (njets[3] < (((theMaxBin-theMinBin)/theNBins)*(bin_index+1))))){
         IncrementNSelBin(3, bin_index, weight);
       }
       if((CandTypeForMM[2] == CandTypeRequested) && (selStepMM[2] >= selStepCut)  && ((njets[2] >= (((theMaxBin-theMinBin)/theNBins)*bin_index)) && (njets[2] < (((theMaxBin-theMinBin)/theNBins)*(bin_index+1))))){
         IncrementNSelBin(2, bin_index, weight);
       }
       if((CandTypeForMM[1] == CandTypeRequested) && (selStepMM[1] >= selStepCut) && ((njets[1] >= (((theMaxBin-theMinBin)/theNBins)*bin_index)) && (njets[1] < (((theMaxBin-theMinBin)/theNBins)*(bin_index+1))))){
         IncrementNSelBin(1, bin_index, weight);
       }
       if((CandTypeForMM[0] == CandTypeRequested) && (selStepMM[0] >= selStepCut) && ((njets[0] >= (((theMaxBin-theMinBin)/theNBins)*bin_index)) && (njets[0] < (((theMaxBin-theMinBin)/theNBins)*(bin_index+1))))){
         IncrementNSelBin(0, bin_index, weight);
       }




        if(    ((CandTypeForMM[3] == CandTypeRequested) && (selStepMM[3] >= selStepCut) && ((njets[3] >= (((theMaxBin-theMinBin)/theNBins)*bin_index)) && (njets[3] < (((theMaxBin-theMinBin)/theNBins)*(bin_index+1)))))  && 
               (
                !((CandTypeForMM[2] == CandTypeRequested) && (selStepMM[2] >= selStepCut) && ((njets[2] >= (((theMaxBin-theMinBin)/theNBins)*bin_index)) && (njets[2] < (((theMaxBin-theMinBin)/theNBins)*(bin_index+1)))))                 ||
                !((CandTypeForMM[1] == CandTypeRequested) && (selStepMM[1] >= selStepCut) && ((njets[1] >= (((theMaxBin-theMinBin)/theNBins)*bin_index)) && (njets[1] < (((theMaxBin-theMinBin)/theNBins)*(bin_index+1)))))                 ||
                !((CandTypeForMM[0] == CandTypeRequested) && (selStepMM[0] >= selStepCut)  && ((njets[0] >= (((theMaxBin-theMinBin)/theNBins)*bin_index)) && (njets[0] < (((theMaxBin-theMinBin)/theNBins)*(bin_index+1)))))
               )
          ){ std::cout << "WARNING in CountNSel: Passing Selection TT, but not passing selection TL or LT or LL!" << std::endl; }



        if(    ((CandTypeForMM[2] == CandTypeRequested) && (selStepMM[2] >= selStepCut) && ((njets[2] >= (((theMaxBin-theMinBin)/theNBins)*bin_index)) && (njets[2] < (((theMaxBin-theMinBin)/theNBins)*(bin_index+1)))))                 &&
               !((CandTypeForMM[0] == CandTypeRequested) && (selStepMM[0] >= selStepCut)  && ((njets[0] >= (((theMaxBin-theMinBin)/theNBins)*bin_index)) && (njets[0] < (((theMaxBin-theMinBin)/theNBins)*(bin_index+1)))))
          ){ std::cout << "WARNING in CountNSel: Passing Selection TL, but not passing selection LL!" << std::endl;  }



        if(    ((CandTypeForMM[1] == CandTypeRequested) && (selStepMM[1] >= selStepCut) && ((njets[1] >= (((theMaxBin-theMinBin)/theNBins)*bin_index)) && (njets[1] < (((theMaxBin-theMinBin)/theNBins)*(bin_index+1)))))                 &&
               !((CandTypeForMM[0] == CandTypeRequested) && (selStepMM[0] >= selStepCut)  && ((njets[0] >= (((theMaxBin-theMinBin)/theNBins)*bin_index)) && (njets[0] < (((theMaxBin-theMinBin)/theNBins)*(bin_index+1)))))
          ){ std::cout << "WARNING in CountNSel: Passing Selection LT, but not passing selection LL!" << std::endl;  }





     }


}



void GeneralMMEstimation::FillMMExpected(const DiLeptonSelection & sel, Dataset dataset, SelectionTable selTable, unsigned int d, float weight, string CandTypeRequested, int selStepCut, const NTEvent* event){


       int selStepMM[4] = {-1, -1, -1, -1};
       string CandTypeForMM[4]= {"", "", "", ""};
       unsigned int njets[4] = {-1, -1, -1, -1};


       for (unsigned int iso_index = 0; iso_index<4; iso_index++) {
        vector<NTJet> SelectedJetsForMM; bool LepPairForMM[4]; vector<NTElectron> candElecForMM; vector<NTMuon> candMuonForMM;
        DiLeptonSelection sel_aux(sel);
        selStepMM[iso_index] = sel_aux.FillTableForMM(GetIsolations().iso1[iso_index],GetIsolations().iso2[iso_index], selTable, &(dataset), d, weight);         
        // Be careful if inverted or not
        LepPairForMM[iso_index] = sel_aux.GetLeptonPairForMM(GetIsolations().iso1[iso_index],GetIsolations().iso2[iso_index], candMuonForMM, candElecForMM, CandTypeForMM[iso_index]);
        SelectedJetsForMM = sel_aux.GetSelectedJets(candMuonForMM, candElecForMM);
        njets[iso_index] = SelectedJetsForMM.size(); //if(iso_index == 3){njets[3]=SelectedJetsForMM.size(); njets[2]=SelectedJetsForMM.size(); njets[1]=SelectedJetsForMM.size(); njets[0]=SelectedJetsForMM.size();}
       }
       //       for (unsigned int iso_index = 0; iso_index<3; iso_index++) {
       //           CandTypeForMM[iso_index] = CandTypeForMM[3];
       //       }

       //Here, njets[] is the variable vs. whih the plots of Matrix Method will be done. It could be any other variable!     


     if((CandTypeForMM[3] == CandTypeRequested) && (selStepMM[3] >= selStepCut)){

       if((event->TMEME == 11 || event->TMEME == 11001 || event->TMEME == 10110 || event->TMEME == 21100) && dataset.Name() == "TTbar" // emu
       ){
         FillMMExpectedPlot("TTbarSignal",3,njets[3],weight);
       }else if((event->TMEME == 1 || event->TMEME == 10 || // l(=e/mu)jets
                event->TMEME == 2 || event->TMEME == 10101 || event->TMEME == 20200 || // ee
                event->TMEME == 20 || event->TMEME == 11010 || event->TMEME == 22000 || // mumu                
                event->TMEME == 10010 || event->TMEME == 10001 || // l(=e/mu)tau(->had)
                event->TMEME == 10100 || event->TMEME == 11000) && dataset.Name() == "TTbar" // tau(->e/mu)jets
       ){
         FillMMExpectedPlot("TTbarSemileptonic",3,njets[3],weight);
       }else{
         FillMMExpectedPlot(dataset.Name(),3,njets[3],weight);
       }

     }

     if((CandTypeForMM[2] == CandTypeRequested) && (selStepMM[2] >= selStepCut)){

       if((event->TMEME == 11 || event->TMEME == 11001 || event->TMEME == 10110 || event->TMEME == 21100) && dataset.Name() == "TTbar" // emu
       ){
         FillMMExpectedPlot("TTbarSignal",2,njets[2],weight);
       }else if((event->TMEME == 1 || event->TMEME == 10 || // l(=e/mu)jets
                event->TMEME == 2 || event->TMEME == 10101 || event->TMEME == 20200 || // ee
                event->TMEME == 20 || event->TMEME == 11010 || event->TMEME == 22000 || // mumu
                event->TMEME == 10010 || event->TMEME == 10001 || // l(=e/mu)tau(->had)
                event->TMEME == 10100 || event->TMEME == 11000) && dataset.Name() == "TTbar" // tau(->e/mu)jets
       ){
         FillMMExpectedPlot("TTbarSemileptonic",2,njets[2],weight);
       }else{
         FillMMExpectedPlot(dataset.Name(),2,njets[2],weight);
       }

     }

    if((CandTypeForMM[1] == CandTypeRequested) && (selStepMM[1] >= selStepCut)){

        if((event->TMEME == 11 || event->TMEME == 11001 || event->TMEME == 10110 || event->TMEME == 21100)  && dataset.Name() == "TTbar" // emu
        ){
          FillMMExpectedPlot("TTbarSignal",1,njets[1],weight);
        }else if((event->TMEME == 1 || event->TMEME == 10 || // l(=e/mu)jets
                 event->TMEME == 2 || event->TMEME == 10101 || event->TMEME == 20200 || // ee
                 event->TMEME == 20 || event->TMEME == 11010 || event->TMEME == 22000 || // mumu
                 event->TMEME == 10010 || event->TMEME == 10001 || // l(=e/mu)tau(->had)
                 event->TMEME == 10100 || event->TMEME == 11000) && dataset.Name() == "TTbar" // tau(->e/mu)jets
        ){
          FillMMExpectedPlot("TTbarSemileptonic",1,njets[1],weight);
        }else{
          FillMMExpectedPlot(dataset.Name(),1,njets[1],weight);
        }
    }
        
    if((CandTypeForMM[0] == CandTypeRequested) && (selStepMM[0] >= selStepCut)){

         if((event->TMEME == 11 || event->TMEME == 11001 || event->TMEME == 10110 || event->TMEME == 21100) && dataset.Name() == "TTbar" // emu
         ){
           FillMMExpectedPlot("TTbarSignal",0,njets[0],weight);
         }else if((event->TMEME == 1 || event->TMEME == 10 || // l(=e/mu)jets
                  event->TMEME == 2 || event->TMEME == 10101 || event->TMEME == 20200 || // ee
                  event->TMEME == 20 || event->TMEME == 11010 || event->TMEME == 22000 || // mumu
                  event->TMEME == 10010 || event->TMEME == 10001 || // l(=e/mu)tau(->had)
                  event->TMEME == 10100 || event->TMEME == 11000) && dataset.Name() == "TTbar" // tau(->e/mu)jets
         ){
           FillMMExpectedPlot("TTbarSemileptonic",0,njets[0],weight);
         }else{
           FillMMExpectedPlot(dataset.Name(),0,njets[0],weight);
         }
    }      

      







        if(    ((CandTypeForMM[3] == CandTypeRequested) && (selStepMM[3] >= selStepCut))  && 
               (
                !((CandTypeForMM[2] == CandTypeRequested) && (selStepMM[2] >= selStepCut)) ||
                !((CandTypeForMM[1] == CandTypeRequested) && (selStepMM[1] >= selStepCut)) ||
                !((CandTypeForMM[0] == CandTypeRequested) && (selStepMM[0] >= selStepCut))
               )
          ){ std::cout << "WARNING in FillMMExpected: Passing Selection TT, but not passing selection TL or LT or LL!" << std::endl;  }



        if(    ((CandTypeForMM[2] == CandTypeRequested) && (selStepMM[2] >= selStepCut))  &&
               !((CandTypeForMM[0] == CandTypeRequested) && (selStepMM[0] >= selStepCut))
          ){ std::cout << "WARNING in FillMMExpected: Passing Selection TL, but not passing selection LL!" << std::endl;  }



        if(    ((CandTypeForMM[1] == CandTypeRequested) && (selStepMM[1] >= selStepCut))  &&
               !((CandTypeForMM[0] == CandTypeRequested) && (selStepMM[0] >= selStepCut))
          ){ std::cout << "WARNING in FillMMExpected: Passing Selection LT, but not passing selection LL!" << std::endl;  }








}





void GeneralMMEstimation::IncludeSystematicError(){
 float EpsilonEFake0 = EpsilonEFake;
 float EpsilonESignal0  = EpsilonESignal;
 EpsilonEFake = ranEpsEFake.Gaus(EpsilonEFake0, EpsilonEFakeErr);
 EpsilonESignal  = ranEpsESignal.Gaus(EpsilonESignal0,  EpsilonESignalErr);
 float EpsilonMuFake0 = EpsilonMuFake;
 float EpsilonMuSignal0  = EpsilonMuSignal;
 EpsilonMuFake = ranEpsMuFake.Gaus(EpsilonMuFake0, EpsilonMuFakeErr);
 EpsilonMuSignal  = ranEpsMuSignal.Gaus(EpsilonMuSignal0,  EpsilonMuSignalErr);
}


void GeneralMMEstimation::IncludeStatisticalError(){
 float N4 = locNSelected[3];
 float N3;
 float N2;
 if(locNSelected[2] <= locNSelected[1]){
  N3 = locNSelected[2] - N4;
  N2 = locNSelected[1] - N3 - N4;
 }else{
  N3 = locNSelected[1] - N4;
  N2 = locNSelected[2] - N3 - N4;
 }
 float N1 = locNSelected[0] - N2 - N3 -N4;
 N1 = ranN1.Poisson(N1);
 N2 = ranN2.Poisson(N2);
 N3 = ranN3.Poisson(N3);
 N4 = ranN4.Poisson(N4);
 locNSelected[3] = N4;
 if(locNSelected[2] <= locNSelected[1]){
  locNSelected[2] = N3+N4;
  locNSelected[1] = N2+N3+N4;
 }else{
  locNSelected[1] = N3+N4;
  locNSelected[2] = N2+N3+N4;
 }
 locNSelected[0] = N1+N2+N3+N4;
}


void GeneralMMEstimation::SolveTheSystem(bool doCorrections){

      for(unsigned int iso_index=0; iso_index<4; iso_index++){
        NMMEstimatedTT[iso_index] = 0;
        NMMEstimatedTF[iso_index] = 0;
        NMMEstimatedFT[iso_index] = 0;
        NMMEstimatedFF[iso_index] = 0;
      }


      TMatrixF theEffmatrix(4,4); // A
      TMatrixF solutions(4,1);    // x
      TMatrixF mesured(4,1);      // b

      float eff[16] ;  // A

      float Eff_llttTT   = EpsilonESignal * EpsilonMuSignal;
      float Eff_llttFT   = EpsilonEFake * EpsilonMuSignal;
      float Eff_llttTF   = EpsilonESignal * EpsilonMuFake;
      float Eff_llttFF   = EpsilonEFake * EpsilonMuFake;

      float Eff_lltlTT   = EpsilonESignal;
      float Eff_lltlFT   = EpsilonEFake;
      float Eff_lltlTF   = EpsilonESignal;
      float Eff_lltlFF   = EpsilonEFake;

      float Eff_llltTT   = EpsilonMuSignal;
      float Eff_llltFT   = EpsilonMuSignal;
      float Eff_llltTF   = EpsilonMuFake;
      float Eff_llltFF   = EpsilonMuFake;

      eff[0] = 1.;
      eff[1] = 1.;
      eff[2] = 1.;
      eff[3] = 1.;

      eff[4] = Eff_llltFF;
      eff[5] = Eff_llltTF;
      eff[6] = Eff_llltFT;
      eff[7] = Eff_llltTT;

      eff[8] = Eff_lltlFF;
      eff[9] = Eff_lltlTF;
      eff[10] = Eff_lltlFT;
      eff[11] = Eff_lltlTT;

      eff[12] = Eff_llttFF;
      eff[13] = Eff_llttTF;
      eff[14] = Eff_llttFT;
      eff[15] = Eff_llttTT;
      
      theEffmatrix.SetMatrixArray(eff);
      theEffmatrix.Invert();

      float ArrayMes[4] ; // b
      ArrayMes[0] = locNSelected[0];
      ArrayMes[1] = locNSelected[1];
      ArrayMes[2] = locNSelected[2];
      ArrayMes[3] = locNSelected[3];

      mesured.SetMatrixArray(ArrayMes);
      solutions = theEffmatrix*mesured; 

      NMMEstimatedFF[3] = solutions(0,0)*Eff_llttFF;
      NMMEstimatedTF[3] = solutions(1,0)*Eff_llttTF;
      NMMEstimatedFT[3] = solutions(2,0)*Eff_llttFT;
      NMMEstimatedTT[3] = solutions(3,0)*Eff_llttTT;

      NMMEstimatedFF[2] = solutions(0,0)*Eff_lltlFF;
      NMMEstimatedTF[2] = solutions(1,0)*Eff_lltlTF;
      NMMEstimatedFT[2] = solutions(2,0)*Eff_lltlFT;
      NMMEstimatedTT[2] = solutions(3,0)*Eff_lltlTT;

      NMMEstimatedFF[1] = solutions(0,0)*Eff_llltFF;
      NMMEstimatedTF[1] = solutions(1,0)*Eff_llltTF;
      NMMEstimatedFT[1] = solutions(2,0)*Eff_llltFT;
      NMMEstimatedTT[1] = solutions(3,0)*Eff_llltTT;

      NMMEstimatedFF[0] = solutions(0,0);
      NMMEstimatedTF[0] = solutions(1,0);
      NMMEstimatedFT[0] = solutions(2,0);
      NMMEstimatedTT[0] = solutions(3,0);

      if(doCorrections){

        float EpsilonESignal_bar = 1 - EpsilonESignal;
        float EpsilonMuSignal_bar = 1 - EpsilonMuSignal;

        NMMEstimatedTT[3] = NMMEstimatedTT[3]
                          + NMMEstimatedTT[0] * EpsilonESignal * EpsilonMuSignal_bar * EpsilonMuFake
                          + NMMEstimatedTT[0] * EpsilonMuSignal * EpsilonESignal_bar * EpsilonEFake
                          + NMMEstimatedTT[0] * EpsilonESignal_bar * EpsilonMuSignal_bar * EpsilonEFake * EpsilonMuFake;

        NMMEstimatedFT[3] = NMMEstimatedFT[3]
                          - NMMEstimatedTT[0] * EpsilonESignal * EpsilonMuSignal_bar * EpsilonMuFake
                          + NMMEstimatedFT[0] * EpsilonMuFake * EpsilonESignal_bar * EpsilonEFake;

        NMMEstimatedTF[3] = NMMEstimatedTF[3]
                          - NMMEstimatedTT[0] * EpsilonMuSignal * EpsilonESignal_bar * EpsilonEFake
                          + NMMEstimatedTF[0] * EpsilonEFake * EpsilonMuSignal_bar * EpsilonMuFake;

        NMMEstimatedFF[3] = NMMEstimatedFF[3]
                          - NMMEstimatedTT[0] * EpsilonESignal_bar * EpsilonMuSignal_bar * EpsilonEFake * EpsilonMuFake
                          - NMMEstimatedFT[0] * EpsilonMuFake * EpsilonESignal_bar * EpsilonEFake
                          - NMMEstimatedTF[0] * EpsilonEFake * EpsilonMuSignal_bar * EpsilonMuFake;
      }


}

void GeneralMMEstimation::FillDistributions(unsigned int bin_index){

    for(unsigned int iso_index=0; iso_index<4; iso_index++){
          theDistributions[bin_index].NMMEstimatedTTDistribution[iso_index]->Fill(NMMEstimatedTT[iso_index]);
          theDistributions[bin_index].NMMEstimatedTFDistribution[iso_index]->Fill(NMMEstimatedTF[iso_index]);
          theDistributions[bin_index].NMMEstimatedFTDistribution[iso_index]->Fill(NMMEstimatedFT[iso_index]);
          theDistributions[bin_index].NMMEstimatedFFDistribution[iso_index]->Fill(NMMEstimatedFF[iso_index]);
          theMMEstimatedValues[bin_index].NofMMEstimatedTT[iso_index] += NMMEstimatedTT[iso_index];
          theMMEstimatedValues[bin_index].NofMMEstimatedTF[iso_index] += NMMEstimatedTF[iso_index];
          theMMEstimatedValues[bin_index].NofMMEstimatedFT[iso_index] += NMMEstimatedFT[iso_index];
          theMMEstimatedValues[bin_index].NofMMEstimatedFF[iso_index] += NMMEstimatedFF[iso_index];
    }
}


void GeneralMMEstimation::CalculateRms(unsigned int bin_index, unsigned int NbIterations){

    for(unsigned int iso_index=0; iso_index<4; iso_index++){
          theMMEstimatedValues[bin_index].MMEstimatedTTErr[iso_index] += ((theMMEstimatedValues[bin_index].NofMMEstimatedTT[iso_index]/NbIterations)-NMMEstimatedTT[iso_index])*((theMMEstimatedValues[bin_index].NofMMEstimatedTT[iso_index]/NbIterations)-NMMEstimatedTT[iso_index]);
          theMMEstimatedValues[bin_index].MMEstimatedTFErr[iso_index] += ((theMMEstimatedValues[bin_index].NofMMEstimatedTF[iso_index]/NbIterations)-NMMEstimatedTF[iso_index])*((theMMEstimatedValues[bin_index].NofMMEstimatedTF[iso_index]/NbIterations)-NMMEstimatedTF[iso_index]);
          theMMEstimatedValues[bin_index].MMEstimatedFTErr[iso_index] += ((theMMEstimatedValues[bin_index].NofMMEstimatedFT[iso_index]/NbIterations)-NMMEstimatedFT[iso_index])*((theMMEstimatedValues[bin_index].NofMMEstimatedFT[iso_index]/NbIterations)-NMMEstimatedFT[iso_index]);
          theMMEstimatedValues[bin_index].MMEstimatedFFErr[iso_index] += ((theMMEstimatedValues[bin_index].NofMMEstimatedFF[iso_index]/NbIterations)-NMMEstimatedFF[iso_index])*((theMMEstimatedValues[bin_index].NofMMEstimatedFF[iso_index]/NbIterations)-NMMEstimatedFF[iso_index]);
    }
}



void GeneralMMEstimation::SetMMEstimated(unsigned int bin_index, unsigned int NbIterations){

     for(unsigned int iso_index=0; iso_index<4; iso_index++){
      theMMEstimatedValues[bin_index].NofMMEstimatedTT[iso_index] = (theMMEstimatedValues[bin_index].NofMMEstimatedTT[iso_index])/NbIterations;
      theMMEstimatedValues[bin_index].MMEstimatedTTErr[iso_index] = sqrt((theMMEstimatedValues[bin_index].MMEstimatedTTErr[iso_index])/(NbIterations-1));
      theMMEstimatedValues[bin_index].NofMMEstimatedTF[iso_index] = (theMMEstimatedValues[bin_index].NofMMEstimatedTF[iso_index])/NbIterations;
      theMMEstimatedValues[bin_index].MMEstimatedTFErr[iso_index] = sqrt((theMMEstimatedValues[bin_index].MMEstimatedTFErr[iso_index])/(NbIterations-1));
      theMMEstimatedValues[bin_index].NofMMEstimatedFT[iso_index] = (theMMEstimatedValues[bin_index].NofMMEstimatedFT[iso_index])/NbIterations;
      theMMEstimatedValues[bin_index].MMEstimatedFTErr[iso_index] = sqrt((theMMEstimatedValues[bin_index].MMEstimatedFTErr[iso_index])/(NbIterations-1));
      theMMEstimatedValues[bin_index].NofMMEstimatedFF[iso_index] = (theMMEstimatedValues[bin_index].NofMMEstimatedFF[iso_index])/NbIterations;
      theMMEstimatedValues[bin_index].MMEstimatedFFErr[iso_index] = sqrt((theMMEstimatedValues[bin_index].MMEstimatedFFErr[iso_index])/(NbIterations-1));
     }

}


void GeneralMMEstimation::ReadMMFile(string fileName){

  TFile* file_MM   = new TFile((fileName).c_str());
  file_MM->cd();
  
  TH1F* tmpMMNSelected[4];
  for(unsigned int iso_index=0; iso_index<4; iso_index++){
   tmpMMNSelected[iso_index]  = (TH1F*)gDirectory->Get(("MMNSelected_"+IsoNames[iso_index]+theChannel).c_str());
  }
  for(unsigned int iso_index=0; iso_index<4; iso_index++){
   for(unsigned int bin_index = 0; bin_index < theNBins; bin_index++){
    theNSelected[bin_index].NSel[iso_index] = tmpMMNSelected[iso_index]->GetBinContent(bin_index+1) ;
   }
  }
  for(unsigned int iso_index=0; iso_index<4; iso_index++){
   tmpMMNSelected[iso_index] = 0;
   delete tmpMMNSelected[iso_index];
  }

  file_MM->Close();
  delete file_MM;

}




void GeneralMMEstimation::ReadMMFileForPullTest(string fileName, float epsilon_s_e, float epsilon_f_e, float epsilon_s_mu, float epsilon_f_mu){

  TFile* file_MM   = new TFile((fileName).c_str());
  file_MM->cd();

  TH1F* histoTmpTT[4];
  TH1F* histoTmpTF[4];
  TH1F* histoTmpFT[4];
  TH1F* histoTmpFF[4];
  
  for(unsigned int iso_index=0; iso_index<4; iso_index++){
   histoTmpTT[iso_index] = (TH1F*)gDirectory->Get(("MMExpected_"+IsoNames[iso_index]+theChannel+"_TTbarSignal").c_str());
   histoTmpTF[iso_index] = (TH1F*)gDirectory->Get(("MMExpected_"+IsoNames[iso_index]+theChannel+"_TTbarSemileptonic").c_str());
   histoTmpFT[iso_index] = (TH1F*)gDirectory->Get(("MMExpected_"+IsoNames[iso_index]+theChannel+"_TTbarSemileptonic").c_str());
   histoTmpFF[iso_index] = (TH1F*)gDirectory->Get(("MMExpected_"+IsoNames[iso_index]+theChannel+"_TTbar").c_str());
   histoTmpTF[iso_index]->Add(histoTmpTF[iso_index], histoTmpTF[iso_index], 0, 0.5);
   histoTmpFT[iso_index]->Add(histoTmpFT[iso_index], histoTmpFT[iso_index], 0, 0.5);
  }  

  for(unsigned int iso_index=0; iso_index<4; iso_index++){
   for(unsigned int bin_index = 0; bin_index < theNBins; bin_index++){
     histoTT[iso_index]->SetBinContent(bin_index+1, histoTmpTT[iso_index]->GetBinContent(bin_index+1));
     histoTF[iso_index]->SetBinContent(bin_index+1, histoTmpTF[iso_index]->GetBinContent(bin_index+1));
     histoFT[iso_index]->SetBinContent(bin_index+1, histoTmpFT[iso_index]->GetBinContent(bin_index+1));
     histoFF[iso_index]->SetBinContent(bin_index+1, histoTmpFF[iso_index]->GetBinContent(bin_index+1));
   }
  }  

  //LT
  histoTT[1]->Add(histoTT[0], histoTT[0], 0, epsilon_s_mu);
  histoTF[1]->Add(histoTF[0], histoTF[0], 0, epsilon_f_mu);
  histoFT[1]->Add(histoFT[0], histoFT[0], 0, epsilon_s_mu);
  histoFF[1]->Add(histoFF[0], histoFF[0], 0, epsilon_f_mu);

  //TL
  histoTT[2]->Add(histoTT[0], histoTT[0], 0, epsilon_s_e);
  histoTF[2]->Add(histoTF[0], histoTF[0], 0, epsilon_s_e);
  histoFT[2]->Add(histoFT[0], histoFT[0], 0, epsilon_f_e);
  histoFF[2]->Add(histoFF[0], histoFF[0], 0, epsilon_f_e);

  //TT
  histoTT[3]->Add(histoTT[0], histoTT[0], 0, epsilon_s_e * epsilon_s_mu);
  histoTF[3]->Add(histoTF[0], histoTF[0], 0, epsilon_s_e * epsilon_f_mu);
  histoFT[3]->Add(histoFT[0], histoFT[0], 0, epsilon_f_e * epsilon_s_mu);
  histoFF[3]->Add(histoFF[0], histoFF[0], 0, epsilon_f_e * epsilon_f_mu);


  histoN[0]->Add(histoTT[0], histoTF[0], 1, 1);
  histoN[0]->Add(histoN[0], histoFT[0], 1, 1);
  histoN[0]->Add(histoN[0], histoFF[0], 1, 1);

  histoN[1]->Add(histoTT[1], histoTF[1], 1, 1);
  histoN[1]->Add(histoN[1], histoFT[1], 1, 1);
  histoN[1]->Add(histoN[1], histoFF[1], 1, 1);

  histoN[2]->Add(histoTT[2], histoTF[2], 1, 1);
  histoN[2]->Add(histoN[2], histoFT[2], 1, 1);
  histoN[2]->Add(histoN[2], histoFF[2], 1, 1);

  histoN[3]->Add(histoTT[3], histoTF[3], 1, 1);
  histoN[3]->Add(histoN[3], histoFT[3], 1, 1);
  histoN[3]->Add(histoN[3], histoFF[3], 1, 1);

  for(unsigned int iso_index=0; iso_index<4; iso_index++){
   for(unsigned int bin_index = 0; bin_index < theNBins; bin_index++){
    theNSelected[bin_index].NSel[iso_index] = histoN[iso_index]->GetBinContent(bin_index+1) ;
   }
  }

 
  for(unsigned int iso_index=0; iso_index<4; iso_index++){
   histoTmpTT[iso_index] = 0;
   delete histoTmpTT[iso_index];
   histoTmpTF[iso_index] = 0;
   delete histoTmpTF[iso_index];
   histoTmpFT[iso_index] = 0;
   delete histoTmpFT[iso_index];
   histoTmpFF[iso_index] = 0;
   delete histoTmpFF[iso_index];
  }

  file_MM->Close();
  delete file_MM;

}










void GeneralMMEstimation::RunTheMatrixMethod(vector<struct MMEpsilons> valMMEpsilons, unsigned int NbIterations, bool doStatistical, bool doSystematic, bool doCorrections){


  for(unsigned int bin_index = 0; bin_index < theNBins; bin_index++){

   for(unsigned int iso_index=0; iso_index<4; iso_index++){
    for(unsigned int i=0; i< (unsigned int) theDistributions[bin_index].NMMEstimatedTTDistribution[iso_index]->GetXaxis()->GetNbins(); i++){
     theDistributions[bin_index].NMMEstimatedTTDistribution[iso_index]->SetBinContent(i+1, 0.);
    }
    for(unsigned int i=0; i< (unsigned int) theDistributions[bin_index].NMMEstimatedTFDistribution[iso_index]->GetXaxis()->GetNbins(); i++){
     theDistributions[bin_index].NMMEstimatedTFDistribution[iso_index]->SetBinContent(i+1, 0.);
    }
    for(unsigned int i=0; i< (unsigned int) theDistributions[bin_index].NMMEstimatedFTDistribution[iso_index]->GetXaxis()->GetNbins(); i++){
     theDistributions[bin_index].NMMEstimatedFTDistribution[iso_index]->SetBinContent(i+1, 0.);
    }
    for(unsigned int i=0; i< (unsigned int) theDistributions[bin_index].NMMEstimatedFFDistribution[iso_index]->GetXaxis()->GetNbins(); i++){
     theDistributions[bin_index].NMMEstimatedFFDistribution[iso_index]->SetBinContent(i+1, 0.);
    }
   }



   for (unsigned int i=0 ; i<NbIterations ; i++ ) {   // pseudoexperiments
      for(unsigned int iso_index=0; iso_index<4; iso_index++){
       locNSelected[iso_index] = theNSelected[bin_index].NSel[iso_index];
      }
      EpsilonEFake = valMMEpsilons[bin_index].EpsilonEFake;
      EpsilonEFakeErr = valMMEpsilons[bin_index].EpsilonEFakeErr;
      EpsilonESignal = valMMEpsilons[bin_index].EpsilonESignal;
      EpsilonESignalErr = valMMEpsilons[bin_index].EpsilonESignalErr;

      EpsilonMuFake = valMMEpsilons[bin_index].EpsilonMuFake;
      EpsilonMuFakeErr = valMMEpsilons[bin_index].EpsilonMuFakeErr;
      EpsilonMuSignal = valMMEpsilons[bin_index].EpsilonMuSignal;
      EpsilonMuSignalErr = valMMEpsilons[bin_index].EpsilonMuSignalErr;

      if(doStatistical) IncludeStatisticalError();
      if(doSystematic) IncludeSystematicError();
      SolveTheSystem(doCorrections);
      FillDistributions(bin_index);
   }


   for (unsigned int i=0 ; i<NbIterations ; i++ ) {   // pseudoexperiments
      for(unsigned int iso_index=0; iso_index<4; iso_index++){
       locNSelected[iso_index] = theNSelected[bin_index].NSel[iso_index];
      }
      EpsilonEFake = valMMEpsilons[bin_index].EpsilonEFake;
      EpsilonEFakeErr = valMMEpsilons[bin_index].EpsilonEFakeErr;
      EpsilonESignal = valMMEpsilons[bin_index].EpsilonESignal;
      EpsilonESignalErr = valMMEpsilons[bin_index].EpsilonESignalErr;

      EpsilonMuFake = valMMEpsilons[bin_index].EpsilonMuFake;
      EpsilonMuFakeErr = valMMEpsilons[bin_index].EpsilonMuFakeErr;
      EpsilonMuSignal = valMMEpsilons[bin_index].EpsilonMuSignal;
      EpsilonMuSignalErr = valMMEpsilons[bin_index].EpsilonMuSignalErr;

      if(doStatistical) IncludeStatisticalError();
      if(doSystematic) IncludeSystematicError();
      SolveTheSystem(doCorrections);
      CalculateRms(bin_index, NbIterations); 
   }

 
   SetMMEstimated(bin_index, NbIterations);

   for(unsigned int iso_index=0; iso_index<4; iso_index++){
    theMMEstimatedPlots.MMEstimated_TT[iso_index]->SetBinContent(bin_index+1, theMMEstimatedValues[bin_index].NofMMEstimatedTT[iso_index] );
    theMMEstimatedPlots.MMEstimated_TT[iso_index]->SetBinError(bin_index+1, theMMEstimatedValues[bin_index].MMEstimatedTTErr[iso_index] );
    theMMEstimatedPlots.MMEstimated_TF[iso_index]->SetBinContent(bin_index+1, theMMEstimatedValues[bin_index].NofMMEstimatedTF[iso_index] );
    theMMEstimatedPlots.MMEstimated_TF[iso_index]->SetBinError(bin_index+1, theMMEstimatedValues[bin_index].MMEstimatedTFErr[iso_index] );
    theMMEstimatedPlots.MMEstimated_FT[iso_index]->SetBinContent(bin_index+1, theMMEstimatedValues[bin_index].NofMMEstimatedFT[iso_index] );
    theMMEstimatedPlots.MMEstimated_FT[iso_index]->SetBinError(bin_index+1, theMMEstimatedValues[bin_index].MMEstimatedFTErr[iso_index] );
    theMMEstimatedPlots.MMEstimated_FF[iso_index]->SetBinContent(bin_index+1, theMMEstimatedValues[bin_index].NofMMEstimatedFF[iso_index] );
    theMMEstimatedPlots.MMEstimated_FF[iso_index]->SetBinError(bin_index+1, theMMEstimatedValues[bin_index].MMEstimatedFFErr[iso_index] );
   }

  }



}


vector<struct MMEstimated> GeneralMMEstimation::GetMMEstimated(){
return theMMEstimatedValues;
}


void GeneralMMEstimation::PrintMMEstimated(){

   std::cout << "\\begin{table}" << std::endl;
   std::cout << "\\begin{center}" << std::endl;
   std::cout << "\\begin{tabular}{|";
   for(unsigned int bin_index=2; bin_index<theNBins; bin_index++){
    std::cout << " c |";
   }
   std::cout << "}" << std::endl;
   std::cout << "\\hline" << std::endl;


   for(unsigned int bin_index=2; bin_index<theNBins; bin_index++){
    std::cout << " & Njets = " << bin_index;
   }
   std::cout << "\\\\ \\hline" << std::endl;

   std::cout << "N of TT-like";
   for(unsigned int bin_index=2; bin_index<theNBins; bin_index++){
    std::cout << " & " << theMMEstimatedValues[bin_index].NofMMEstimatedTT[3] << "$\\pm$" << theMMEstimatedValues[bin_index].MMEstimatedTTErr[3];
   }
   std::cout << "\\\\ \\hline" << std::endl;

   std::cout << "N of TF-like";
   for(unsigned int bin_index=2; bin_index<theNBins; bin_index++){
    std::cout << " & " << theMMEstimatedValues[bin_index].NofMMEstimatedTF[3] << "$\\pm$" << theMMEstimatedValues[bin_index].MMEstimatedTFErr[3];
   }
   std::cout << "\\\\ \\hline" << std::endl;

   std::cout << "N of FT-like";
   for(unsigned int bin_index=2; bin_index<theNBins; bin_index++){
    std::cout << " & " << theMMEstimatedValues[bin_index].NofMMEstimatedFT[3] << "$\\pm$" << theMMEstimatedValues[bin_index].MMEstimatedFTErr[3];
   }
   std::cout << "\\\\ \\hline" << std::endl;

   std::cout << "N of FF-like";
   for(unsigned int bin_index=2; bin_index<theNBins; bin_index++){
    std::cout << " & " << theMMEstimatedValues[bin_index].NofMMEstimatedFF[3] << "$\\pm$" << theMMEstimatedValues[bin_index].MMEstimatedFFErr[3];
   }
   std::cout << "\\\\ \\hline" << std::endl;

   std::cout << "\\end{tabular}" << std::endl;
   std::cout << "\\caption{FIX ME} \\label{FIX ME}" << std::endl;
   std::cout << "\\end{center}" << std::endl;
   std::cout << "\\end{table}" << std::endl;

   float NofMMEstimatedTTTOTAL = 0;
   float MMEstimatedTTErrTOTAL = 0;
   float NofMMEstimatedTFTOTAL = 0;
   float MMEstimatedTFErrTOTAL = 0;
   float NofMMEstimatedFTTOTAL = 0;
   float MMEstimatedFTErrTOTAL = 0;
   float NofMMEstimatedFFTOTAL = 0;
   float MMEstimatedFFErrTOTAL = 0;

   for(unsigned int bin_index=0; bin_index<theNBins; bin_index++){
    NofMMEstimatedTTTOTAL += theMMEstimatedValues[bin_index].NofMMEstimatedTT[3];
    MMEstimatedTTErrTOTAL += theMMEstimatedValues[bin_index].MMEstimatedTTErr[3]*theMMEstimatedValues[bin_index].MMEstimatedTTErr[3];
    NofMMEstimatedTFTOTAL += theMMEstimatedValues[bin_index].NofMMEstimatedTF[3];
    MMEstimatedTFErrTOTAL += theMMEstimatedValues[bin_index].MMEstimatedTFErr[3]*theMMEstimatedValues[bin_index].MMEstimatedTFErr[3];
    NofMMEstimatedFTTOTAL += theMMEstimatedValues[bin_index].NofMMEstimatedFT[3];
    MMEstimatedFTErrTOTAL += theMMEstimatedValues[bin_index].MMEstimatedFTErr[3]*theMMEstimatedValues[bin_index].MMEstimatedFTErr[3];
    NofMMEstimatedFFTOTAL += theMMEstimatedValues[bin_index].NofMMEstimatedFF[3];
    MMEstimatedFFErrTOTAL += theMMEstimatedValues[bin_index].MMEstimatedFFErr[3]*theMMEstimatedValues[bin_index].MMEstimatedFFErr[3];
   }

   std::cout << "NofMMEstimatedTTTOTAL: " << NofMMEstimatedTTTOTAL << std::endl;
   std::cout << "MMEstimatedTTErrTOTAL: " << sqrt(MMEstimatedTTErrTOTAL) << std::endl;
   std::cout << "NofMMEstimatedTFTOTAL: " << NofMMEstimatedTFTOTAL << std::endl;
   std::cout << "MMEstimatedTFErrTOTAL: " << sqrt(MMEstimatedTFErrTOTAL) << std::endl;
   std::cout << "NofMMEstimatedFTTOTAL: " << NofMMEstimatedFTTOTAL << std::endl;
   std::cout << "MMEstimatedFTErrTOTAL: " << sqrt(MMEstimatedFTErrTOTAL) << std::endl;
   std::cout << "NofMMEstimatedFFTOTAL: " << NofMMEstimatedFFTOTAL << std::endl;
   std::cout << "MMEstimatedFFErrTOTAL: " << sqrt(MMEstimatedFFErrTOTAL) << std::endl;

}


struct MMEstimatedPlots GeneralMMEstimation::GetMMEstimatedPlots(){
 return theMMEstimatedPlots;
}


void GeneralMMEstimation::FillMMExpectedPlot(string datasetname, unsigned int iso_index, float val, float weight){

       for(unsigned int i=0;i< theMMExpectedPlots.size(); i++){
         if(datasetname == theMMExpectedPlots[i].Name[iso_index]) (theMMExpectedPlots[i].MMExpected[iso_index])->Fill(val, weight);
       }

}

vector <struct MMExpectedPlots> GeneralMMEstimation::GetMMExpectedPlots(){
  return theMMExpectedPlots;
}




void GeneralMMEstimation::WriteMMFile(string fileName){

    TFile* file_MM = new TFile((fileName).c_str(),"RECREATE");
    file_MM->cd();

    for(unsigned int bin_index=0; bin_index<theNBins; bin_index++){
      for(unsigned int iso_index=0; iso_index<4; iso_index++){
        theDistributions[bin_index].NMMEstimatedTTDistribution[iso_index]->Write();
        theDistributions[bin_index].NMMEstimatedTFDistribution[iso_index]->Write();
        theDistributions[bin_index].NMMEstimatedFTDistribution[iso_index]->Write();
        theDistributions[bin_index].NMMEstimatedFFDistribution[iso_index]->Write();
      }
    }



    for(unsigned int iso_index=0; iso_index<4; iso_index++){
       for(unsigned int i=0;i< theMMExpectedPlots.size(); i++){
         (theMMExpectedPlots[i].MMExpected[iso_index])->Write();
       }
    }

    for(unsigned int iso_index=0; iso_index<4; iso_index++){
      theMMEstimatedPlots.MMEstimated_TT[iso_index]->Write();
      theMMEstimatedPlots.MMEstimated_TF[iso_index]->Write();
      theMMEstimatedPlots.MMEstimated_FT[iso_index]->Write();
      theMMEstimatedPlots.MMEstimated_FF[iso_index]->Write();
    }

    // Writing histos contaning all information concerning NSelected
    TH1F* MMNSelected[4];

    for(unsigned int iso_index=0; iso_index<4; iso_index++){
      MMNSelected[iso_index] = new TH1F(("MMNSelected_"+IsoNames[iso_index]+theChannel).c_str(), ("MMNSelected_"+IsoNames[iso_index]+theChannel).c_str(),  theNBins, theMinBin, theMaxBin);
    }
    for(unsigned int iso_index=0; iso_index<4; iso_index++){
     for(unsigned int bin_index = 0; bin_index < theNBins; bin_index++){
       MMNSelected[iso_index]->SetBinContent(bin_index+1, theNSelected[bin_index].NSel[iso_index]);
     }
     MMNSelected[iso_index]->Write();
    }
    for(unsigned int iso_index=0; iso_index<4; iso_index++){
     MMNSelected[iso_index] = 0;
     delete MMNSelected[iso_index];
    }

    file_MM->Write();
    file_MM->Close();
    delete file_MM;


}





void GeneralMMEstimation::WriteMMFileFast(string fileName){

    TFile* file_MM = new TFile((fileName).c_str(),"RECREATE");
    file_MM->cd();

    for(unsigned int bin_index=0; bin_index<theNBins; bin_index++){
      for(unsigned int iso_index=0; iso_index<4; iso_index++){
        theDistributions[bin_index].NMMEstimatedTTDistribution[iso_index]->Write();
        theDistributions[bin_index].NMMEstimatedTFDistribution[iso_index]->Write();
        theDistributions[bin_index].NMMEstimatedFTDistribution[iso_index]->Write();
        theDistributions[bin_index].NMMEstimatedFFDistribution[iso_index]->Write();
      }
    }

    for(unsigned int iso_index=0; iso_index<4; iso_index++){
      theMMEstimatedPlots.MMEstimated_TT[iso_index]->Write();
      theMMEstimatedPlots.MMEstimated_TF[iso_index]->Write();
      theMMEstimatedPlots.MMEstimated_FT[iso_index]->Write();
      theMMEstimatedPlots.MMEstimated_FF[iso_index]->Write();
    }

    // Writing histos contaning all information concerning NSelected
    TH1F* MMNSelected[4];
    for(unsigned int iso_index=0; iso_index<4; iso_index++){
      MMNSelected[iso_index] = new TH1F(("MMNSelected_"+IsoNames[iso_index]+theChannel).c_str(), ("MMNSelected_"+IsoNames[iso_index]+theChannel).c_str(),  theNBins, theMinBin, theMaxBin);
    }
    for(unsigned int iso_index=0; iso_index<4; iso_index++){
     for(unsigned int bin_index = 0; bin_index < theNBins; bin_index++){
       MMNSelected[iso_index]->SetBinContent(bin_index+1, theNSelected[bin_index].NSel[iso_index]);
     }
     MMNSelected[iso_index]->Write();
    }
    for(unsigned int iso_index=0; iso_index<4; iso_index++){
     MMNSelected[iso_index] = 0;
     delete MMNSelected[iso_index];
    }

    file_MM->Write();
    file_MM->Close();
    delete file_MM;


}






void GeneralMMEstimation::WriteMMFileFastForPullTest(string fileINPUT, string fileOUTPUT, float epsilon_s_e, float epsilon_f_e, float epsilon_s_mu, float epsilon_f_mu ){

 
    TFile* file_MM_INPUT   = new TFile((fileINPUT).c_str());
    file_MM_INPUT->cd();
     
    TH1F* tmpTT[4];
    TH1F* tmpTF[4];
    TH1F* tmpFT[4];
    TH1F* tmpFF[4];

    for(unsigned int iso_index=0; iso_index<4; iso_index++){
     tmpTT[iso_index] = (TH1F*)gDirectory->Get(("MMExpected_"+IsoNames[iso_index]+theChannel+"_TTbarSignal").c_str());
     tmpTF[iso_index] = (TH1F*)gDirectory->Get(("MMExpected_"+IsoNames[iso_index]+theChannel+"_TTbarSemileptonic").c_str());
     tmpFT[iso_index] = (TH1F*)gDirectory->Get(("MMExpected_"+IsoNames[iso_index]+theChannel+"_TTbarSemileptonic").c_str());
     tmpFF[iso_index] = (TH1F*)gDirectory->Get(("MMExpected_"+IsoNames[iso_index]+theChannel+"_TTbar").c_str());
     tmpTF[iso_index]->Add(tmpTF[iso_index], tmpTF[iso_index], 0, 0.5);
     tmpFT[iso_index]->Add(tmpFT[iso_index], tmpFT[iso_index], 0, 0.5);
    }
  
    //LT
    tmpTT[1]->Add(tmpTT[0], tmpTT[0], 0, epsilon_s_mu);
    tmpTF[1]->Add(tmpTF[0], tmpTF[0], 0, epsilon_f_mu);
    tmpFT[1]->Add(tmpFT[0], tmpFT[0], 0, epsilon_s_mu);
    tmpFF[1]->Add(tmpFF[0], tmpFF[0], 0, epsilon_f_mu);

    //TL
    tmpTT[2]->Add(tmpTT[0], tmpTT[0], 0, epsilon_s_e);
    tmpTF[2]->Add(tmpTF[0], tmpTF[0], 0, epsilon_s_e);
    tmpFT[2]->Add(tmpFT[0], tmpFT[0], 0, epsilon_f_e);
    tmpFF[2]->Add(tmpFF[0], tmpFF[0], 0, epsilon_f_e);

    //TT
    tmpTT[3]->Add(tmpTT[0], tmpTT[0], 0, epsilon_s_e * epsilon_s_mu);
    tmpTF[3]->Add(tmpTF[0], tmpTF[0], 0, epsilon_s_e * epsilon_f_mu);
    tmpFT[3]->Add(tmpFT[0], tmpFT[0], 0, epsilon_f_e * epsilon_s_mu);
    tmpFF[3]->Add(tmpFF[0], tmpFF[0], 0, epsilon_f_e * epsilon_f_mu);

    TFile* file_MM_OUTPUT = new TFile((fileOUTPUT).c_str(),"RECREATE");
    file_MM_OUTPUT->cd();
   
    for(unsigned int iso_index=0; iso_index<4; iso_index++){
     tmpTT[iso_index]->Write();
     tmpTF[iso_index]->Write(("MMExpected_"+IsoNames[iso_index]+theChannel+"_TTbarSemileptonic_TF").c_str());
     tmpFT[iso_index]->Write(("MMExpected_"+IsoNames[iso_index]+theChannel+"_TTbarSemileptonic_FT").c_str());
     tmpFF[iso_index]->Write();
     tmpTT[iso_index] = 0;
     tmpTF[iso_index] = 0;
     tmpFT[iso_index] = 0;
     tmpFF[iso_index] = 0;
     delete tmpTT[iso_index];
     delete tmpTF[iso_index];
     delete tmpFT[iso_index];
     delete tmpFF[iso_index];
    }
    
    file_MM_INPUT->Close();
    delete file_MM_INPUT;


    for(unsigned int bin_index=0; bin_index<theNBins; bin_index++){
      for(unsigned int iso_index=0; iso_index<4; iso_index++){
        theDistributions[bin_index].NMMEstimatedTTDistribution[iso_index]->Write();
        theDistributions[bin_index].NMMEstimatedTFDistribution[iso_index]->Write();
        theDistributions[bin_index].NMMEstimatedFTDistribution[iso_index]->Write();
        theDistributions[bin_index].NMMEstimatedFFDistribution[iso_index]->Write();
      }
    }

    for(unsigned int iso_index=0; iso_index<4; iso_index++){
      theMMEstimatedPlots.MMEstimated_TT[iso_index]->Write();
      theMMEstimatedPlots.MMEstimated_TF[iso_index]->Write();
      theMMEstimatedPlots.MMEstimated_FT[iso_index]->Write();
      theMMEstimatedPlots.MMEstimated_FF[iso_index]->Write();
    }

    // Writing histos contaning all information concerning NSelected
    TH1F* MMNSelected[4];
    for(unsigned int iso_index=0; iso_index<4; iso_index++){
      MMNSelected[iso_index] = new TH1F(("MMNSelected_"+IsoNames[iso_index]+theChannel).c_str(), ("MMNSelected_"+IsoNames[iso_index]+theChannel).c_str(),  theNBins, theMinBin, theMaxBin);
    }
    for(unsigned int iso_index=0; iso_index<4; iso_index++){
     for(unsigned int bin_index = 0; bin_index < theNBins; bin_index++){
       MMNSelected[iso_index]->SetBinContent(bin_index+1, theNSelected[bin_index].NSel[iso_index]);
     }
     MMNSelected[iso_index]->Write();
    }
    for(unsigned int iso_index=0; iso_index<4; iso_index++){
     MMNSelected[iso_index] = 0;
     delete MMNSelected[iso_index];
    }


    file_MM_OUTPUT->Write();
    file_MM_OUTPUT->Close();
    delete file_MM_OUTPUT;


}
