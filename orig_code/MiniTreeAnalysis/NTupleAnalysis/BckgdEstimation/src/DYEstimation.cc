include "../interface/DYEstimation.h"

                
DYEstimation::DYEstimation(){
     NDYMC_CR          = 0;
     NDYMCinMz_SR      = 0;   
     NDYMCinMz_CR      = 0;   
     NFittedDYMC_CR          = 0;
     NFittedDYMCinMz_SR      = 0;   
     NFittedDYMCinMz_CR      = 0;   
     NunnormDYMC_CR          = 0;
     NunnormDYMCinMz_SR      = 0;   
     NunnormDYMCinMz_CR      = 0;   
     NDYEstimatedMC    = 0;  
     NDYMC_SR          = 0;          
     NMC_CR = 0.; 
     NMCinMz_CR = 0.;
     NMCinMz_SR = 0.;

     NDATAinMz_SR      = 0;   
     NDATAinMz_CR      = 0;   
     NDATA_CR          = 0;          
     NDATA_SR          = 0;
     NDYEstimatedDATA  = 0;     
     NDYEstimatedDATACounting  = 0;     
     NDYEstimatedStatErrorDATA = 0.;
     NDYEstimatedStatErrorDATACounting = 0.;
     NFittedDATA_CR          = 0;            
     NDYMCinMzWithScaledPlusContinuum_SR = 0.;
     NDYMCinMzWithScaledMinusContinuum_SR = 0.;
     
     ContinuumInZdata  = 0.; 
     DYVerbose  = true;   
}

DYEstimation::~DYEstimation(){}

void DYEstimation::FillNbofEventsInSRandCR (bool isInCR, bool isInSR,float DilM, string datasetname,float w){
 
    bool isZjetslike = false;
    isZjetslike = ( datasetname=="Zjets" || datasetname=="DYToMuMu_M-20" || datasetname=="DYToEE_M-20" || datasetname=="DYToTauTau_M-20" );
    if ( isInCR && isZjetslike && (DilM<76 || DilM>106) ) { NDYMC_CR += w; NunnormDYMC_CR +=1;}
    if ( isInCR && isZjetslike && (DilM>76 && DilM<106) ) { NDYMCinMz_CR += w; NunnormDYMCinMz_CR += 1;}
    if ( isInSR && isZjetslike && (DilM<76 || DilM>106) )  NDYMC_SR += w;
    if ( isInSR && isZjetslike && (DilM>76 && DilM<106) )  { NDYMCinMz_SR += w; NunnormDYMCinMz_SR += 1; }
    bool isMC = false;
    isMC = (datasetname!="DATA" && datasetname!="Data" && 
           datasetname!="DataMu" && datasetname!="DATAMu" &&
           datasetname!="DataEG" && datasetname!="DATAEG" &&
           datasetname!="DataMuEG" && datasetname!="DATAMuEG" );
    if ( isInCR && isMC && (DilM<76 || DilM>106) )  NMC_CR += w; 
    if ( isInCR && isMC && (DilM>76 && DilM<106) )  NMCinMz_CR += w;
    if ( isInSR && isMC && (DilM>76 && DilM<106) )  NMCinMz_SR += w;
    bool isd = false;
    isd = (datasetname=="DATA" || datasetname=="Data" || 
           datasetname=="DataMu" || datasetname=="DATAMu" ||
           datasetname=="DataEG" || datasetname=="DATAEG");
    if ( isInCR && isd && (DilM<76 || DilM>106) )  NDATA_CR++; 
    if ( isInCR && isd && (DilM>76 && DilM<106) )  NDATAinMz_CR++;
    if ( isInSR && isd && (DilM<76 || DilM>106) )  NDATA_SR++;
    if ( isInSR && isd && (DilM>76 && DilM<106) )  NDATAinMz_SR++;
 
}

std::vector<TH1F*> DYEstimation::CreateDYHisto(string TypeSel, string datasetname, int bin, float min, float max){
 
    TH1F *h;
    if ( datasetname=="TTbar" ) {
       h = new TH1F((datasetname+"sig"+TypeSel).c_str(),"",bin,min,max); 
//       hlist.push_back(h);
       if (TypeSel=="SR" )   hlistSR.push_back(h);
       if (TypeSel=="CR" )   hlistCR.push_back(h);
       if (TypeSel=="emu" )  hlistemu.push_back(h);
       if (TypeSel=="ContCR" )  hlistContCR.push_back(h);
       h = new TH1F((datasetname+"bkg"+TypeSel).c_str(),"",bin,min,max); 
//       hlist.push_back(h);
       if (TypeSel=="SR" )   hlistSR.push_back(h);
       if (TypeSel=="CR" )   hlistCR.push_back(h);
       if (TypeSel=="emu" )  hlistemu.push_back(h);
       if (TypeSel=="ContCR" )  hlistContCR.push_back(h);
    } else {
       h = new TH1F((datasetname+TypeSel).c_str(),"",bin,min,max); 
//       hlist.push_back(h);
       if (TypeSel=="SR" )   hlistSR.push_back(h);
       if (TypeSel=="CR" )   hlistCR.push_back(h);
       if (TypeSel=="emu" )  hlistemu.push_back(h);
       bool isZjetslike = false;
       isZjetslike = ( datasetname=="Zjets" || datasetname=="DYToMuMu_M-20" || datasetname=="DYToEE_M-20" || datasetname=="DYToTauTau_M-20" );
       if (TypeSel=="ContCR" && !isZjetslike )  hlistContCR.push_back(h);
    } 
    
    if ( TypeSel=="emu" ) {
       hstack1 = new THStack("hstack1","hstack1");
//       all1    = new TH1F("all1","",bin,min,max); 
    }   
    if ( TypeSel=="SR" ) {
       hstackSR = new THStack("hstackSR","hstackSR");
//       allSR    = new TH1F("allSR","",bin,min,max); 
    }   
    if ( TypeSel=="CR" ) {
       hstackCR = new THStack("hstackCR","hstackCR");
//       allCR    = new TH1F("allCR","",bin,min,max); 
    }   
    if ( TypeSel=="ContCR" ) {
       hstackContCR = new THStack("hstackContCR","hstackContCR");
    }   
    
//    return hlist;
    if (TypeSel=="SR") return hlistSR;
    if (TypeSel=="CR") return hlistCR;
    if (TypeSel=="emu") return hlistemu;
    if (TypeSel=="ContCR") return hlistContCR;


}


void DYEstimation::FillDYHistos(string TypeSel, std::vector<TH1F*>hlist, string datasetname, int tmeme, float DilM, float weight){
 
   //std::cout<<"tmeme,DilM "<<tmeme<<" "<<DilM<<std::endl;

  for (unsigned int i=0; i < hlist.size(); i++)
//     cout<<"hlist[i]->GetName() "<<hlist[i]->GetName()<<endl;
    {
       if ( datasetname=="TTbar" ) {
          if (TypeSel == "emu" ) { 
             if ( ((tmeme==11)||(tmeme==21100)||(tmeme==11001)||(tmeme==10110)) && string(hlist[i]->GetName())==("TTbarsig"+TypeSel)   ) {
                 hlist[i]->Fill(DilM, weight);
             }
             if ( !((tmeme==11)||(tmeme==21100)||(tmeme==11001)||(tmeme==10110)) && string(hlist[i]->GetName())==("TTbarbkg"+TypeSel)   ) {
                 hlist[i]->Fill(DilM, weight);
             } 
          }// TypeSel == "emu"   
          if ( TypeSel == "SR" || TypeSel == "CR" || TypeSel == "ContCR") { 
            // for mumu
             if ( ((tmeme==20)||(tmeme==11010)||(tmeme==22000)) && string(hlist[i]->GetName())==("TTbarsig"+TypeSel)   ) {
                 hlist[i]->Fill(DilM, weight);
             }
            if ( !((tmeme==20)||(tmeme==11010)||(tmeme==22000)) && string(hlist[i]->GetName())==("TTbarbkg"+TypeSel)   ) {
                 hlist[i]->Fill(DilM, weight);
             } 
            // for ee
             if ( ((tmeme==2)||(tmeme==10101)||(tmeme==20200)) && string(hlist[i]->GetName())==("TTbarsig"+TypeSel)   ) {
                 hlist[i]->Fill(DilM, weight);
             }
            if ( !((tmeme==2)||(tmeme==10101)||(tmeme==20200)) && string(hlist[i]->GetName())==("TTbarbkg"+TypeSel)   ) {
                 hlist[i]->Fill(DilM, weight);
             } 
          }// TypeSel == "SR"  || TypeSel == "CR" || TypeSel == "ContCR"
       } else {
          if ( datasetname=="DATA" || datasetname=="Data" || datasetname=="DataMu" || datasetname=="DataEG") {
             if ( datasetname+TypeSel == hlist[i]->GetName()  )  hlist[i]->Fill(DilM, 1 );
          }else{
             if ( datasetname+TypeSel == hlist[i]->GetName()  )  hlist[i]->Fill(DilM, weight);
          }
       }  
    }
 
}

void DYEstimation::MakeDYHistos(string TypeSel, std::vector<TH1F*>hlist, TH1F* all1){

// modifdg
// merging of Wlnu->Wjets
   int IWCR = -1;
   int IWSR = -1;
   int IWemu = -1;
   int IWContCR = -1;
   for (unsigned int j=0; j < hlist.size(); j++) {
      if ( hlist[j]->GetName()==string("WjetsCR") ) IWCR = j;
      if ( hlist[j]->GetName()==string("WjetsSR") ) IWSR = j;
      if ( hlist[j]->GetName()==string("Wjetsemu") ) IWemu = j;
      if ( hlist[j]->GetName()==string("WjetsContCR") ) IWContCR = j;
   }
   for (unsigned int j=0; j < hlist.size(); j++) {
          if ( hlist[j]->GetName()==string("WjetsMuCR") || hlist[j]->GetName()==string("WjetsECR") ||
          hlist[j]->GetName()==string("WjetsTauCR") ) {  
             if ( IWCR>=0 ) hlist[IWCR]->Add(hlist[j]);  
          }         
          if ( hlist[j]->GetName()==string("WjetsMuSR") || hlist[j]->GetName()==string("WjetsESR") ||
          hlist[j]->GetName()==string("WjetsTauSR")) {  
             if ( IWSR>=0 ) hlist[IWSR]->Add(hlist[j]);  
          }
          if ( hlist[j]->GetName()==string("WjetsMuemu") || hlist[j]->GetName()==string("WjetsEemu") ||
          hlist[j]->GetName()==string("WjetsTauemu")) {  
             if ( IWemu>=0 ) hlist[IWemu]->Add(hlist[j]);  
          }          
          if ( hlist[j]->GetName()==string("WjetsMuContCR") || hlist[j]->GetName()==string("WjetsEContCR") ||
          hlist[j]->GetName()==string("WjetsTauContCR") ) {  
             if ( IWContCR>=0 ) hlist[IWContCR]->Add(hlist[j]);  
          }         
   } 
// merging of WW,WZ,ZZ -> VV
   int IVVSR = -1;
   int IVVCR = -1;
   int IVVemu = -1;
   int IVVContCR = -1;
   for (unsigned int j=0; j < hlist.size(); j++) {
      if ( hlist[j]->GetName()==string("VVCR") ) IVVCR = j;
      if ( hlist[j]->GetName()==string("VVSR") ) IVVSR = j;
      if ( hlist[j]->GetName()==string("VVemu") ) IVVemu = j;
      if ( hlist[j]->GetName()==string("VVContCR") ) IVVContCR = j;
   }
   for (unsigned int j=0; j < hlist.size(); j++) {
          if ( hlist[j]->GetName()==string("WWCR") || hlist[j]->GetName()==string("WZCR") ||
          hlist[j]->GetName()==string("ZZCR") ) {  
             if ( IVVCR>=0 ) hlist[IVVCR]->Add(hlist[j]);  
          }         
          if ( hlist[j]->GetName()==string("WWSR") || hlist[j]->GetName()==string("WZSR") ||
          hlist[j]->GetName()==string("ZZSR") ) {  
             if ( IVVSR>=0 ) hlist[IVVSR]->Add(hlist[j]);  
          }         
          if ( hlist[j]->GetName()==string("WWemu") || hlist[j]->GetName()==string("WZemu") ||
          hlist[j]->GetName()==string("ZZemu") ) {  
             if ( IVVemu>=0 ) hlist[IVVemu]->Add(hlist[j]);  
          }         
          if ( hlist[j]->GetName()==string("WWContCR") || hlist[j]->GetName()==string("WZContCR") ||
          hlist[j]->GetName()==string("ZZContCR") ) {  
             if ( IVVContCR>=0 ) hlist[IVVContCR]->Add(hlist[j]);  
          }         
   } 
   
// merging of T (singletoptW), Tbar (singleantitopW)  -> SingleToptW 
   int ISingleToptWSR = -1;
   int ISingleToptWCR = -1;
   int ISingleToptWemu = -1;
   int ISingleToptWContCR = -1;
   for (unsigned int j=0; j < hlist.size(); j++) {
      if ( hlist[j]->GetName()==string("SingleToptWCR") ) ISingleToptWCR = j;
      if ( hlist[j]->GetName()==string("SingleToptWSR") ) ISingleToptWSR = j;
      if ( hlist[j]->GetName()==string("SingleToptWemu") ) ISingleToptWemu = j;
      if ( hlist[j]->GetName()==string("SingleToptWContCR") ) ISingleToptWContCR = j;
   }
   for (unsigned int j=0; j < hlist.size(); j++) {
          if ( hlist[j]->GetName()==string("TtWCR") || hlist[j]->GetName()==string("TbartWCR") ) {  
             if ( ISingleToptWCR>=0 ) hlist[ISingleToptWCR]->Add(hlist[j]);  
          }         
          if ( hlist[j]->GetName()==string("TtWSR") || hlist[j]->GetName()==string("TbartWSR") ) {  
             if ( ISingleToptWSR>=0 ) hlist[ISingleToptWSR]->Add(hlist[j]);  
          }         
          if ( hlist[j]->GetName()==string("TtWemu") || hlist[j]->GetName()==string("TbartWemu") ) {  
             if ( IVVemu>=0 ) hlist[IVVemu]->Add(hlist[j]);  
          }         
          if ( hlist[j]->GetName()==string("TtWContCR") || hlist[j]->GetName()==string("TbartWContCR")  ) {  
             if ( ISingleToptWContCR>=0 ) hlist[ISingleToptWContCR]->Add(hlist[j]);  
          }         
   } 
// merging of low DY events -> DY
   int IZjSR = -1;
   int IZjCR = -1;
   int IZjemu = -1;
   for (unsigned int j=0; j < hlist.size(); j++) {
      if ( hlist[j]->GetName()==string("ZjetsCR") ) IZjCR = j;
      if ( hlist[j]->GetName()==string("ZjetsSR") ) IZjSR = j;
      if ( hlist[j]->GetName()==string("Zjetsemu") ) IZjemu = j;
   }
   for (unsigned int j=0; j < hlist.size(); j++) {
          if ( hlist[j]->GetName()==string("DYToMuMu_M-20CR") || hlist[j]->GetName()==string("DYToEE_M-20CR") ||
          hlist[j]->GetName()==string("DYToTauTau_M-20CR") ) {  
             if ( IZjCR>=0 ) hlist[IZjCR]->Add(hlist[j]);  
          }         
          if ( hlist[j]->GetName()==string("DYToMuMu_M-20SR") || hlist[j]->GetName()==string("DYToEE_M-20SR") ||
          hlist[j]->GetName()==string("DYToTauTau_M-20SR") ) {  
             if ( IZjSR>=0 ) hlist[IZjSR]->Add(hlist[j]);  
          }         
          if ( hlist[j]->GetName()==string("DYToMuMu_M-20emu") || hlist[j]->GetName()==string("DYToEE_M-20emu") ||
          hlist[j]->GetName()==string("DYToTauTau_M-20emu") ) {  
             if ( IZjemu>=0 ) hlist[IZjemu]->Add(hlist[j]);  
          }         
   } 
   
// endmodifdg

  std::vector<string> ordre; // to be verified
//   ordre.push_back("Zjets");
//   ordre.push_back("VV");
//   ordre.push_back("TTbarsig");
//   ordre.push_back("SingleToptW");
//   ordre.push_back("Wjets");
//   ordre.push_back("TTbarbkg");
  ordre.push_back("TTbarbkg");
  ordre.push_back("Wjets");
  ordre.push_back("SingleToptW");
  ordre.push_back("TTbarsig");
  ordre.push_back("VV");
  ordre.push_back("Zjets");
    
 
  for (unsigned int i=0; i < ordre.size(); i++)
    {
      for (unsigned int j=0; j < hlist.size(); j++)
       {
             if ( hlist[j]->GetName()==(ordre[i]+TypeSel) ) {  
                hstack1->Add(hlist[j]);        
                all1->Add(hlist[j]); 
             }  
       }  
    }
 
//   return all1;
}

void DYEstimation::MakeDataHistos(string TypeSel, std::vector<TH1F*>hlist, TH1F* all1){

      for (unsigned int j=0; j < hlist.size(); j++)
       {
          if ( ( hlist[j]->GetName()==("Data"+TypeSel)) || ( hlist[j]->GetName()==("DATA"+TypeSel)) ||
               ( hlist[j]->GetName()==("DataMu"+TypeSel)) || ( hlist[j]->GetName()==("DataEG"+TypeSel)) 
          ) {  all1->Add(hlist[j]); }
       }  

}

void DYEstimation::ComputeDYEstimatedEvents_MC(){

  NDYEstimatedMC = NFittedDYMC_CR * NFittedDYMCinMz_SR / NFittedDYMCinMz_CR;
  if ( DYVerbose ) { 
     std::cout<<"passage dsComputeDYEstimatedEvents_MC "<<NDYEstimatedMC<<" "<<NFittedDYMC_CR<<" "<<NFittedDYMCinMz_SR<<" "<<NFittedDYMCinMz_CR<<std::endl; 
     std::cout<<"passage dsComputeDYEstimatedEvents_MC NMC_CR"<<NMC_CR <<std::endl; 
  } 
  float stat_uncert = sqrt(( ((NunnormDYMC_CR+NunnormDYMCinMz_CR)/NunnormDYMCinMz_CR)*((NunnormDYMC_CR+NunnormDYMCinMz_CR)/NunnormDYMCinMz_CR) )
                                 *(1./(NunnormDYMC_CR))
                                 + 1./(NunnormDYMCinMz_SR) );
  if ( DYVerbose ) { 
     std::cout<<"statistical error on the number of ESTIMATED DY "<<NDYEstimatedMC*stat_uncert<<std::endl; 
  }
  
}

float DYEstimation::GetNDYEstimatedMC(){
   return  NDYEstimatedMC;
 
} 


float DYEstimation::GetDYEvents_MC(){
   return  NDYMC_SR;
 
} 


float DYEstimation::GetDYEstimatedEvents_MC(){
   return  NDYEstimatedMC;
 
} 

void DYEstimation::MakeDYFits(){
//   MakeDYFits_emu();
}

vector<double> DYEstimation::MakeDYFits_emu(TH1F* all){
  
// Fit of the emu continuum   
//     double * param;
     vector<double> pa;
     TF1* myf = new TF1("CF",FitFunctions::Continuum, 10., 350., 6); 
     myf->SetParameters(1.7256, -4.19201, .0000207, .849781, 125., 0.005);
     myf->SetLineWidth(3);
     all->Fit("CF", "M","same");
     double I = myf->Integral(76.,106.);
//     std::cout<<"Integrale = "<<I <<std::endl;
//      myf->GetParameters(param);
     for (unsigned int j=0; j<6; ++j){
//       param[j] = myf->GetParameter(j);
       pa.push_back(myf->GetParameter(j));
         if ( DYVerbose ) { 
            std::cout<<"parametre["<<j<<"]"<< myf->GetParameter(j)<<std::endl;
         }   
     }
     return pa;
}


void DYEstimation::MakeDYFits_SR(TH1F* all, int nb, float minx ,float maxx,  vector<double> param, double minval, double maxval,string datatype, float FracBkgd){
  
//    int nb = 200;
//    float minx = 0.;
//    float maxx = 350;
// conversion factor for the integral computation
    float convfactor = (maxx-minx)/nb;
    
     vector<double> pa;
// Fit of the DY in the Signal Region 
     if ( minval!=maxval ) {
        int minbin = all->FindBin(minval) - 1 ;
        int maxbin = all->FindBin(maxval) + 1 ;
        all->GetXaxis()->SetRange(minbin,maxbin); // en numero de bins (66,91)
     }   
     TF1* myf = new TF1("BWplusCF",FitFunctions::BreitWignerPlusFixedContinuum, 10., 300., 11); 
     if ( minval!=maxval ) {
//        myf->SetParameters(param[0], param[1], param[2] ,param[3] ,param[4] , param[5], 1., 90., 100., 1., 0.1);
        myf->SetParameters(param[0], param[1], param[2] ,param[3] ,param[4] , param[5], 1., 90., 1000., 1., 0.1);
     } else {  
//        myf->SetParameters(param[0], param[1], param[2] ,param[3] ,param[4] , param[5], 10., 90., 100., 10., 0.1);
        myf->SetParameters(param[0], param[1], param[2] ,param[3] ,param[4] , param[5], 10., 90., 1000., 10., 0.1);
     }
     for (int j=0; j<6; ++j){
       myf->SetParLimits(j,param[j],param[j]);
     }  


     myf->SetLineWidth(3);
     all->Fit("BWplusCF", "M","same");
     double I = myf->Integral(76.,106.);
     I = I/convfactor; 
     if ( DYVerbose ) { 
        std::cout<<"Integrale totale= "<<I <<std::endl;
        std::cout<<"Integrale [0;350]= "<< myf->Integral(0.,350.)/convfactor<<std::endl;
     }  

     for (unsigned int j=0; j<11; ++j){
       pa.push_back(myf->GetParameter(j));
       if ( DYVerbose ) { 
          std::cout<<"MakeDYFits_SR parametre["<<j<<"]"<< myf->GetParameter(j)<<std::endl;
       }          
     }

     myf->SetParameter(10,0.);
     double IBW = myf->Integral(76.,106.);
     IBW = IBW/convfactor; 
     if ( DYVerbose ) { 
        std::cout<<"IBW "<<IBW<<std::endl;
        std::cout<<"IntegraleIBW [0;350]= "<< myf->Integral(0.,350.)/convfactor<<std::endl;
     }  

     myf->SetParameter(10,pa[10]);
     myf->SetParameter(9,0.);
     
     for (unsigned int j=0; j<11; ++j){
       if ( DYVerbose ) { 
          std::cout<<"MakeDYFits_SR parametre["<<j<<"] pour calcule contimuum "<< myf->GetParameter(j)<<std::endl;
       }          
     }
     
     double IContinuum = myf->Integral(76.,106.);
     IContinuum = IContinuum/convfactor; 
     if ( DYVerbose ) { 
        std::cout<<"Icontinuum "<< IContinuum<<std::endl;
        std::cout<<"Icontinuum[0;350] "<< myf->Integral(0.,350.)/convfactor<<std::endl;
     }  

     if ( datatype=="MC" ) { 
        SetNFittedDYMCinMz_SR(IBW);
        NDYMCinMzWithScaledPlusContinuum_SR = IBW + sqrt(IContinuum);
        NDYMCinMzWithScaledMinusContinuum_SR = IBW - sqrt(IContinuum);
     }  
     if ( datatype=="Data" || datatype=="DATA" || datatype=="DataMu" || datatype=="DataEG") { 
        ContinuumInZdata = IContinuum;
        SetNFittedDATAinMz_SR(IBW);
        NFittedDATAinMzWithScaledPlusContinuum_SR = IBW + sqrt(IContinuum);
        NFittedDATAinMzWithScaledMinusContinuum_SR = IBW - sqrt(IContinuum);
     }
    
}

void DYEstimation::MakeDYFits_SR_asym(TH1F* all, int nb, float minx ,float maxx,  vector<double> param, double minval, double maxval,string datatype, float FracBkgd){
  
//    int nb = 200;
//    float minx = 0.;
//    float maxx = 350;
// conversion factor for the integral computation
    float convfactor = (maxx-minx)/nb;
    
     vector<double> pa;
// Fit of the DY in the Signal Region 
     if ( minval!=maxval ) {
        int minbin = all->FindBin(minval) - 1 ;
        int maxbin = all->FindBin(maxval) + 1 ;
        all->GetXaxis()->SetRange(minbin,maxbin); // en numero de bins (66,91)
     }   
     TF1* myf = new TF1("AsymBWplusCF",FitFunctions::AsymBreitWignerPlusFixedContinuum, 10., 300., 13); 
     if ( minval!=maxval ) {
//        myf->SetParameters(param[0], param[1], param[2] ,param[3] ,param[4] , param[5], 1., 90., 100., 1., 0.1);
        myf->SetParameters(param[0], param[1], param[2] ,param[3] ,param[4] , param[5], 1., 90., 1000., 1., 0.1);
        myf->SetParameter(11,1.);
        myf->SetParameter(12,1.);
     } else {  
//        myf->SetParameters(param[0], param[1], param[2] ,param[3] ,param[4] , param[5], 10., 90., 100., 10., 0.1);
        myf->SetParameters(param[0], param[1], param[2] ,param[3] ,param[4] , param[5], 10., 90., 1000., 10., 0.1);
        myf->SetParameter(11,1.);
        myf->SetParameter(12,1.);
     }
     for (int j=0; j<6; ++j){
       myf->SetParLimits(j,param[j],param[j]);
     }  


     myf->SetLineWidth(3);
     all->Fit("AsymBWplusCF", "M","same");
     double I = myf->Integral(76.,106.);
     I = I/convfactor; 
     if ( DYVerbose ) { 
        std::cout<<"Integrale totale asym= "<<I <<std::endl;
        std::cout<<"Integrale [0;350] asym= "<< myf->Integral(0.,350.)/convfactor<<std::endl;
     }  

     for (unsigned int j=0; j<13; ++j){
       pa.push_back(myf->GetParameter(j));
       if ( DYVerbose ) { 
          std::cout<<"MakeDYFits_SR asym parametre["<<j<<"]"<< myf->GetParameter(j)<<std::endl;
       }          
     }

     myf->SetParameter(10,0.);
     double IBW = myf->Integral(76.,106.);
     IBW = IBW/convfactor; 
     if ( DYVerbose ) { 
        std::cout<<"IBW asym "<<IBW<<std::endl;
        std::cout<<"IntegraleIBW asym [0;350]= "<< myf->Integral(0.,350.)/convfactor<<std::endl;
     }  

     myf->SetParameter(10,pa[10]);
     myf->SetParameter(9,0.);
     
     for (unsigned int j=0; j<13; ++j){
       if ( DYVerbose ) { 
          std::cout<<"MakeDYFits_SR asym parametre["<<j<<"] pour calcul contimuum "<< myf->GetParameter(j)<<std::endl;
       }  
     }
     
     double IContinuum = myf->Integral(76.,106.);
     IContinuum = IContinuum/convfactor; 
     if ( DYVerbose ) { 
        std::cout<<"Icontinuum asym "<< IContinuum<<std::endl;
        std::cout<<"Icontinuum asym[0;350] "<< myf->Integral(0.,350.)/convfactor<<std::endl;
     }  

     if ( datatype=="MC" ) { 
        SetNFittedDYMCinMz_SR(IBW);
        NDYMCinMzWithScaledPlusContinuum_SR = IBW + sqrt(IContinuum);
        NDYMCinMzWithScaledMinusContinuum_SR = IBW - sqrt(IContinuum);
     }  
     if ( datatype=="Data" || datatype=="DATA" || datatype=="DataMu" || datatype=="DataEG") { 
        ContinuumInZdata = IContinuum;
        SetNFittedDATAinMz_SR(IBW);
        NFittedDATAinMzWithScaledPlusContinuum_SR = IBW + sqrt(IContinuum);
        NFittedDATAinMzWithScaledMinusContinuum_SR = IBW - sqrt(IContinuum);
     }
    
}

void DYEstimation::MakeDYFits_CR(TH1F* all,int nb, float minx, float maxx, double minval, double maxval,string datatype){
  
//    int nb = 200;
//    float minx = 0;
//    float maxx = 350;
// conversion factor for the integral computation
    float convfactor = (maxx-minx)/nb;
    
// Fit of the DY in the Control Region 
     if ( minval!=maxval ) {
        // for zoom in the peak mass
        int minbin = all->FindBin(minval) - 1 ;
        int maxbin = all->FindBin(maxval) + 1 ;
        all->GetXaxis()->SetRange(minbin,maxbin); // en numero de bins (66,91)
     }   
//     TF1* myf = new TF1("BWF",FitFunctions::BreitWigner, 10., 350., 3); 
     TF1* myf = new TF1("BWF",FitFunctions::BreitWigner, 70., 150., 3); 
     myf->SetNpx(1000);
//     myf->SetParameters(3., 90., 100.);  //  ok   pour mumu
     myf->SetParameters(5., 88., 100.);  // semble meilleur pour ee (shape deformee)
     myf->SetLineWidth(3);
     all->Fit("BWF", "RM","same");
     double I = myf->Integral(76.,106.);
     I = I/convfactor; 
     double Itot = myf->Integral(20.,350.);
     Itot = Itot/convfactor; 
     
     if ( DYVerbose ) { 
        std::cout<<"Integrale BreitWigner= "<<I <<std::endl;
        std::cout<<"Integrale BreitWigner[20;350]= "<<Itot <<std::endl;
     }  

//          if ( NormalizedToZpeakPosition ) {
//            IntMz_LowMet = myf->Eval(myf->GetParameter(1));
//      } else {
//            IntMz_LowMet = I;
//      }   

     if ( datatype=="MC" )  {SetNFittedDYMCinMz_CR(I) ;SetNFittedDYMC_CR(Itot-I) ;}
     if ( datatype=="Data" || datatype=="DATA" || datatype=="DataMu" || datatype=="DataEG") {SetNFittedDATAinMz_CR(I); NFittedDATA_CR = Itot-I;}
   
}

void DYEstimation::MakeDYFits_CR_asym_plusCE(TH1F* all,int nb, float minx, float maxx, double minval, double maxval,string datatype){
  
//    int nb = 200;
//    float minx = 0;
//    float maxx = 350;
// conversion factor for the integral computation
    float convfactor = (maxx-minx)/nb;
    
// Fit of the DY in the Control Region 
     if ( minval!=maxval ) {
        // for zoom in the peak mass
        int minbin = all->FindBin(minval) - 1 ;
        int maxbin = all->FindBin(maxval) + 1 ;
        all->GetXaxis()->SetRange(minbin,maxbin); // en numero de bins (66,91)
     }   
     TF1* myf = new TF1("BWFPlusCE",FitFunctions::AsymBreitWignerPlusCutEffect, 10., 300., 10); 
//       myf->SetParameter(0,2.6);
//       myf->SetParameter(1,90.);
//       myf->SetParameter(2,30000.);
//       myf->SetParameter(3,3.);
//       myf->SetParameter(4,0.);
//           myf->SetParLimits(4,0.,0.);
//       myf->SetParameter(5,1.);
//       myf->SetParameter(6,1.);
// //          myf->SetParLimits(5,1.,1.);
// //          myf->SetParLimits(6,1.,1.);
//       myf->SetParameter(7,10.);
//       myf->SetParameter(8,40.);
//       myf->SetParameter(9,25.);
      myf->SetParameter(0,2.6);
      myf->SetParameter(1,90.);
      myf->SetParameter(2,30000.);
      myf->SetParameter(3,3.);
      myf->SetParameter(4,0.);
          myf->SetParLimits(4,0.,0.);
      myf->SetParameter(5,1.);
      myf->SetParameter(6,1.);
//          myf->SetParLimits(5,1.,1.);
//          myf->SetParLimits(6,1.,1.);
      myf->SetParameter(7,50.);
      myf->SetParameter(8,40.);
      myf->SetParameter(9,25.);


     myf->SetLineWidth(3);
     all->Fit("BWFPlusCE", "RM","same");
     double I = myf->Integral(76.,106.);
     I = I/convfactor; 
     double Itot = myf->Integral(0.,350.);
     Itot = Itot/convfactor; 
     
     if ( DYVerbose ) { 
        std::cout<<"Integrale BreitWigner asym plus CE = "<<I <<std::endl;
        std::cout<<"Integrale BreitWigner[0;350] asym plus CE = "<<Itot <<std::endl;
     }  


     if ( datatype=="MC" )  {
        SetNFittedDYMCinMz_CR(I) ;
        SetNFittedDYMC_CR(Itot-I) ;
     }
     if ( datatype=="Data" || datatype=="DATA" || datatype=="DataMu" || datatype=="DataEG") {
        SetNFittedDATAinMz_CR(I); 
        NFittedDATA_CR = Itot-I;
     }
   
}

void DYEstimation::MakeDYFits_CR_WithBkgSubtraction(TH1F* all, int nb, float minx ,float maxx,  vector<double> param, double minval, double maxval,string datatype, float FracBkgd){
  
//    int nb = 200;
//    float minx = 0.;
//    float maxx = 350;
// conversion factor for the integral computation
    float convfactor = (maxx-minx)/nb;
    
     vector<double> pa;
// Fit of the DY in the Control Region 
     if ( minval!=maxval ) {
        int minbin = all->FindBin(minval) - 1 ;
        int maxbin = all->FindBin(maxval) + 1 ;
        all->GetXaxis()->SetRange(minbin,maxbin); // en numero de bins (66,91)
     }   
     TF1* myf = new TF1("BWplusCF",FitFunctions::BreitWignerPlusFixedContinuum, 10., 300., 11); 
     if ( minval!=maxval ) {
        myf->SetParameters(param[0], param[1], param[2] ,param[3] ,param[4] , param[5], 1., 90., 100., 1., 0.1);
     } else {  
        myf->SetParameters(param[0], param[1], param[2] ,param[3] ,param[4] , param[5], 10., 90., 100., 10., 0.1);
     }
     for (int j=0; j<6; ++j){
       myf->SetParLimits(j,param[j],param[j]);
     }  


     myf->SetLineWidth(3);
     all->Fit("BWplusCF", "M","same");
     double I = myf->Integral(76.,106.);
     double Itot = myf->Integral(0.,350.);
     int numbin66 = all->FindBin(66.);
     double N66 = all->Integral(0.,numbin66);
     I = I/convfactor; 
     Itot = Itot/convfactor; 
     if ( DYVerbose ) { 
        std::cout<<"Integrale totale CR = "<<I <<std::endl;
        std::cout<<"Integrale [0;350] CR = "<< myf->Integral(0.,350.)/convfactor<<std::endl;
        std::cout<<"N66 CR= "<<N66 <<std::endl;
     }  

     for (unsigned int j=0; j<11; ++j){
       pa.push_back(myf->GetParameter(j));
       if ( DYVerbose ) { 
          std::cout<<"MakeDYFits_CR parametre["<<j<<"]"<< myf->GetParameter(j)<<std::endl;
       }          
     }

     myf->SetParameter(10,0.);
     double IBW = myf->Integral(76.,106.);
     double IBW66 = myf->Integral(66.,350.);
     double ItotIBW = myf->Integral(0.,350.);
     IBW = IBW/convfactor; 
     ItotIBW = ItotIBW/convfactor; 
     IBW66 = IBW66/convfactor; 
     if ( DYVerbose ) { 
        std::cout<<"IBW CR "<<IBW<<std::endl;
        std::cout<<"IntegraleIBW [0;350] CR= "<< ItotIBW <<std::endl;
        std::cout<<"IntegraleIBW [66;350] CR = "<< IBW66 <<std::endl;
     }  

     myf->SetParameter(10,pa[10]);
     myf->SetParameter(9,0.);
     
     for (unsigned int j=0; j<11; ++j){
       if ( DYVerbose ) { 
          std::cout<<"MakeDYFits_CR parametre["<<j<<"] pour calcule contimuum "<< myf->GetParameter(j)<<std::endl;
        }  
     }
     
     double IContinuum = myf->Integral(76.,106.);
     double IContinuum66 = myf->Integral(0.,66.);
     IContinuum = IContinuum/convfactor; 
     IContinuum66 = IContinuum66/convfactor; 
     if ( DYVerbose ) { 
        std::cout<<"Icontinuum CR "<< IContinuum<<std::endl;
        std::cout<<"Icontinuum[0;350] CR "<< myf->Integral(0.,350.)/convfactor<<std::endl;
        std::cout<<"Icontinuum[0;66] CR  "<< IContinuum66<<std::endl;
     }  


     if ( datatype=="MC" ) { 
        SetNFittedDYMCinMz_CR(IBW) ;
//        SetNFittedDYMC_CR(ItotIBW-IBW) ;
        SetNFittedDYMC_CR(IBW66-IBW+ N66-IContinuum66) ;
     }  
     if ( datatype=="Data" || datatype=="DATA" || datatype=="DataMu" || datatype=="DataEG") { 
         SetNFittedDATAinMz_CR(IBW);
//       NFittedDATA_CR = ItotIBW-IBW;
         NFittedDATA_CR = IBW66-IBW+ N66-IContinuum66;
         if ( DYVerbose ) { 
            std::cout<<"NFittedDATA_CR = "<<NFittedDATA_CR<<std::endl;
         }   
     }
    
}

// void DYEstimation::MakeDYFits_CR_WithBkgSubtraction(TH1F* all, int nb, float minx ,float maxx,  vector<double> param, double minval, double maxval,string datatype, float FracBkgd){
//   
// //    int nb = 200;
// //    float minx = 0.;
// //    float maxx = 350;
// // conversion factor for the integral computation
//     float convfactor = (maxx-minx)/nb;
//     
//      vector<double> pa;
// // Fit of the DY in the Control Region 
//      if ( minval!=maxval ) {
//         int minbin = all->FindBin(minval) - 1 ;
//         int maxbin = all->FindBin(maxval) + 1 ;
//         all->GetXaxis()->SetRange(minbin,maxbin); // en numero de bins (66,91)
//      }   
//      TF1* myf = new TF1("BWplusCFplusCE",FitFunctions::BreitWignerPlusFixedContinuumPlusCutEffect, 10., 300., 14); 
//      if ( minval!=maxval ) {
//         myf->SetParameters(param[0], param[1], param[2] ,param[3] ,param[4] , param[5], 5., 90., 100., 1., 0.1);
//         myf->SetParameter(11,40.);
//         myf->SetParameter(12,15.);
//         myf->SetParameter(13,300.);
//      } else {  
//         myf->SetParameters(param[0], param[1], param[2] ,param[3] ,param[4] , param[5], 5., 90., 1000., 60., 0.1);
//         myf->SetParameter(11,40.);
//         myf->SetParameter(12,15.);
//         myf->SetParameter(13,300.);
//      }
//      for (int j=0; j<6; ++j){
//        myf->SetParLimits(j,param[j],param[j]);
//      }       
//      myf->SetParLimits(10,0.,99999999.);
// 
// 
//      myf->SetLineWidth(3);
//      all->Fit("BWplusCFplusCE", "M","same");
//      double I = myf->Integral(76.,106.);
//      double Itot = myf->Integral(0.,350.);
//      I = I/convfactor; 
//      Itot = Itot/convfactor; 
//      std::cout<<"Integrale totale CR= "<<I <<std::endl;
//      std::cout<<"Integrale [0;350] CR= "<< myf->Integral(0.,350.)/convfactor<<std::endl;
// 
//      for (unsigned int j=0; j<14; ++j){
//        pa.push_back(myf->GetParameter(j));
//        std::cout<<"MakeDYFits_CR parametre["<<j<<"]"<< myf->GetParameter(j)<<std::endl;
//      }
// 
//      myf->SetParameter(10,0.);
//      double IBW = myf->Integral(76.,106.);
//      double ItotIBW = myf->Integral(0.,350.);
//      IBW = IBW/convfactor; 
//      ItotIBW = ItotIBW/convfactor; 
//      std::cout<<"IBW CR "<<IBW<<std::endl;
//      std::cout<<"IntegraleIBW [0;350] CR= "<< ItotIBW <<std::endl;
// 
//      myf->SetParameter(10,pa[10]);
//      myf->SetParameter(9,0.);
//      myf->SetParameter(13,0.);
//      
//      for (unsigned int j=0; j<14; ++j){
//        std::cout<<"MakeDYFits_CR parametre["<<j<<"] pour calcule contimuum "<< myf->GetParameter(j)<<std::endl;
//      }
//      
//      double IContinuum = myf->Integral(76.,106.);
//      IContinuum = IContinuum/convfactor; 
//      std::cout<<"Icontinuum CR "<< IContinuum<<std::endl;
//      std::cout<<"Icontinuum[0;350] CR "<< myf->Integral(0.,350.)/convfactor<<std::endl;
// 
// 
//      if ( datatype=="MC" ) { 
//         SetNFittedDYMCinMz_CR(IBW) ;
//         SetNFittedDYMC_CR(ItotIBW-IBW) ;
//      }       
//      if ( datatype=="Data" || datatype=="DATA" || datatype=="DataMu" || datatype=="DataEG") { 
//          SetNFittedDATAinMz_CR(IBW);
//       NFittedDATA_CR = ItotIBW-IBW;
//       std::cout<<"NFittedDATA_CR = "<<NFittedDATA_CR<<std::endl;
//      }
//     
// }

void DYEstimation::MakeDYFits_CR_WithBkgSubtraction_asym(TH1F* all, int nb, float minx ,float maxx,  vector<double> param, double minval, double maxval,string datatype, float FracBkgd){
  
//    int nb = 200;
//    float minx = 0.;
//    float maxx = 350;
// conversion factor for the integral computation
    float convfactor = (maxx-minx)/nb;
    
     vector<double> pa;
// Fit of the DY in the Control Region 
     if ( minval!=maxval ) {
        int minbin = all->FindBin(minval) - 1 ;
        int maxbin = all->FindBin(maxval) + 1 ;
        all->GetXaxis()->SetRange(minbin,maxbin); // en numero de bins (66,91)
     }   
     TF1* myf = new TF1("AsymBWplusCF",FitFunctions::AsymBreitWignerPlusFixedContinuum, 10., 300., 13); 
     if ( minval!=maxval ) {
        myf->SetParameters(param[0], param[1], param[2] ,param[3] ,param[4] , param[5], 1., 90., 1000., 1., 0.1);
        myf->SetParameter(11,0.1);
        myf->SetParameter(12,0.1);
     } else {  
        myf->SetParameters(param[0], param[1], param[2] ,param[3] ,param[4] , param[5], 1., 90., 1000., 1., 0.1);
        myf->SetParameter(11,0.1);
        myf->SetParameter(12,0.1);
     }
     for (int j=0; j<6; ++j){
       myf->SetParLimits(j,param[j],param[j]);
     }  


     myf->SetLineWidth(3);
     all->Fit("AsymBWplusCF", "M","same");
     double I = myf->Integral(76.,106.);
     double Itot = myf->Integral(0.,350.);
     int numbin66 = all->FindBin(66.);
     double N66 = all->Integral(0.,numbin66);
     I = I/convfactor; 
     Itot = Itot/convfactor; 
     if ( DYVerbose ) { 
        std::cout<<"Integrale totale CR asym= "<<I <<std::endl;
        std::cout<<"Integrale [0;350] CR asym= "<< myf->Integral(0.,350.)/convfactor<<std::endl;
        std::cout<<"N66 CR= "<<N66 <<std::endl;
     }  

     for (unsigned int j=0; j<13; ++j){
       pa.push_back(myf->GetParameter(j));
       if ( DYVerbose ) { 
          std::cout<<"MakeDYFits_CR asym  parametre["<<j<<"]"<< myf->GetParameter(j)<<std::endl;
       }          
     }

     myf->SetParameter(10,0.);
     double IBW = myf->Integral(76.,106.);
     double IBW66 = myf->Integral(66.,350.);
     double ItotIBW = myf->Integral(0.,350.);
     IBW = IBW/convfactor; 
     ItotIBW = ItotIBW/convfactor; 
     IBW66 = IBW66/convfactor; 
     if ( DYVerbose ) { 
        std::cout<<"IBW CR asym "<<IBW<<std::endl;
        std::cout<<"IntegraleIBW [0;350] CR asym= "<< ItotIBW <<std::endl;
        std::cout<<"IntegraleIBW [66;350] CR asym= "<< IBW66 <<std::endl;
     }  

     myf->SetParameter(10,pa[10]);
     myf->SetParameter(9,0.);
     
     for (unsigned int j=0; j<13; ++j){
        if ( DYVerbose ) { 
          std::cout<<"MakeDYFits_CR asym  parametre["<<j<<"] pour calcule contimuum "<< myf->GetParameter(j)<<std::endl;
        }  
     }
     
     double IContinuum = myf->Integral(76.,106.);
     double IContinuum66 = myf->Integral(0.,66.);
     IContinuum = IContinuum/convfactor; 
     IContinuum66 = IContinuum66/convfactor; 
     if ( DYVerbose ) { 
        std::cout<<"Icontinuum CR asym  "<< IContinuum<<std::endl;
        std::cout<<"Icontinuum[0;350] CR asym "<< myf->Integral(0.,350.)/convfactor<<std::endl;
        std::cout<<"Icontinuum[0;66] CR asym "<< IContinuum66<<std::endl;
     }  


     if ( datatype=="MC" ) { 
        SetNFittedDYMCinMz_CR(IBW) ;
//        SetNFittedDYMC_CR(ItotIBW-IBW) ;
        SetNFittedDYMC_CR(IBW66-IBW+ N66-IContinuum66) ;
     }  
     if ( datatype=="Data" || datatype=="DATA" || datatype=="DataMu" || datatype=="DataEG") { 
         SetNFittedDATAinMz_CR(IBW);
//       NFittedDATA_CR = ItotIBW-IBW;
         NFittedDATA_CR = IBW66-IBW+ N66-IContinuum66;
         if ( DYVerbose ) { 
            std::cout<<"NFittedDATA_CR = "<<NFittedDATA_CR<<std::endl;
         }   
     }
    
}
 
// void DYEstimation::MakeDYFits_CR_WithBkgSubtraction_asym(TH1F* all, int nb, float minx ,float maxx,  vector<double> param, double minval, double maxval,string datatype, float FracBkgd){
//   
// //    int nb = 200;
// //    float minx = 0.;
// //    float maxx = 350;
// // conversion factor for the integral computation
//     float convfactor = (maxx-minx)/nb;
//     
//      vector<double> pa;
// // Fit of the DY in the Control Region 
//      if ( minval!=maxval ) {
//         int minbin = all->FindBin(minval) - 1 ;
//         int maxbin = all->FindBin(maxval) + 1 ;
//         all->GetXaxis()->SetRange(minbin,maxbin); // en numero de bins (66,91)
//      }   
//      TF1* myf = new TF1("AsymBWplusCFplusCE",FitFunctions::AsymBreitWignerPlusFixedContinuumPlusCutEffect, 10., 300., 16); 
//      if ( minval!=maxval ) {
//         myf->SetParameters(param[0], param[1], param[2] ,param[3] ,param[4] , param[5], 1., 90., 100., 1., 0.1);
//         myf->SetParameter(11,0.1);
//         myf->SetParameter(12,0.1);
//         myf->SetParameter(13,66.);
//         myf->SetParameter(14,20.);
//         myf->SetParameter(15,200.);
//      } else {  
//         myf->SetParameters(param[0], param[1], param[2] ,param[3] ,param[4] , param[5], 1., 90., 100., 1., 0.1);
//         myf->SetParameter(11,0.1);
//         myf->SetParameter(12,0.1);
//         myf->SetParameter(13,66.);
//         myf->SetParameter(14,20.);
//         myf->SetParameter(15,200.);
//      }
//      for (int j=0; j<6; ++j){
//        myf->SetParLimits(j,param[j],param[j]);
//      }       
//      myf->SetParLimits(10,0.,99999999.);
// 
// 
//      myf->SetLineWidth(3);
//      all->Fit("AsymBWplusCFplusCE", "M","same");
//      double I = myf->Integral(76.,106.);
//      double Itot = myf->Integral(0.,350.);
//      int numbin66 = all->FindBin(66.);
//      double N66 = all->Integral(0.,numbin66);
//      I = I/convfactor; 
//      Itot = Itot/convfactor; 
//      std::cout<<"Integrale totale CR asym= "<<I <<std::endl;
//      std::cout<<"Integrale [0;350] CR asym= "<< myf->Integral(0.,350.)/convfactor<<std::endl;
//      std::cout<<"N66 CR= "<<N66 <<std::endl;
// 
//      for (unsigned int j=0; j<16; ++j){
//        pa.push_back(myf->GetParameter(j));
//        std::cout<<"MakeDYFits_CR asym  parametre["<<j<<"]"<< myf->GetParameter(j)<<std::endl;
//      }
// 
//      myf->SetParameter(10,0.);
//      double IBW = myf->Integral(76.,106.);
//      double IBW66 = myf->Integral(66.,350.);
//      double ItotIBW = myf->Integral(0.,350.);
//      IBW = IBW/convfactor; 
//      ItotIBW = ItotIBW/convfactor; 
//      IBW66 = IBW66/convfactor; 
//      std::cout<<"IBW CR asym "<<IBW<<std::endl;
//      std::cout<<"IntegraleIBW [0;350] CR asym= "<< ItotIBW <<std::endl;
//      std::cout<<"IntegraleIBW [66;350] CR asym= "<< IBW66 <<std::endl;
// 
//      myf->SetParameter(10,pa[10]);
//      myf->SetParameter(9,0.);
//      myf->SetParameter(15,0.);
//      
//      for (unsigned int j=0; j<16; ++j){
//        std::cout<<"MakeDYFits_CR asym  parametre["<<j<<"] pour calcule contimuum "<< myf->GetParameter(j)<<std::endl;
//      }
//      
//      double IContinuum = myf->Integral(76.,106.);
//      double IContinuum66 = myf->Integral(0.,66.);
//      IContinuum = IContinuum/convfactor; 
//      IContinuum66 = IContinuum66/convfactor; 
//      std::cout<<"Icontinuum CR asym  "<< IContinuum<<std::endl;
//      std::cout<<"Icontinuum[0;350] CR asym "<< myf->Integral(0.,350.)/convfactor<<std::endl;
//      std::cout<<"Icontinuum[0;66] CR asym "<< IContinuum66<<std::endl;
// 
// 
//      if ( datatype=="MC" ) { 
//         SetNFittedDYMCinMz_CR(IBW) ;
//         SetNFittedDYMC_CR(ItotIBW-IBW) ;
// //        SetNFittedDYMC_CR(IBW66-IBW+ N66-IContinuum66) ;
//      }       
//      if ( datatype=="Data" || datatype=="DATA" || datatype=="DataMu" || datatype=="DataEG") { 
//          SetNFittedDATAinMz_CR(IBW);
//       NFittedDATA_CR = ItotIBW-IBW;
// //    NFittedDATA_CR = IBW66-IBW+ N66-IContinuum66;
//       std::cout<<"NFittedDATA_CR = "<<NFittedDATA_CR<<std::endl;
//      }
//     
// }

void DYEstimation::SetNFittedDYMCinMz_CR( double intval){
   NFittedDYMCinMz_CR = float(intval);
}

void DYEstimation::SetNFittedDYMC_CR( double intval){
   NFittedDYMC_CR = float(intval);
}

void DYEstimation::SetNFittedDATAinMz_CR( double intval){
  NFittedDATAinMz_CR  = float(intval);
}

float DYEstimation::GetNDYMCinMz_CR(){
   return NDYMCinMz_CR ;
}

float DYEstimation::GetNFittedDYMCinMz_CR(){
   return NFittedDYMCinMz_CR ;
}

float DYEstimation::GetNDYMC_CR(){
   return NDYMC_CR ;
}

float DYEstimation::GetNFittedDYMC_CR(){
   return NFittedDYMC_CR ;
}

float DYEstimation::GetNDYMC_SR(){
   return NDYMC_SR ;
}

void DYEstimation::SetDYVerbose( bool v){
  DYVerbose  = v;
}

void DYEstimation::SetNFittedDYMCinMz_SR( double intval){
   NFittedDYMCinMz_SR = float(intval);
}

void DYEstimation::SetNFittedDATAinMz_SR( double intval){
   NFittedDATAinMz_SR = float(intval);
}

float DYEstimation::GetNDYMCinMz_SR(){
   return NDYMCinMz_SR ;
}

float DYEstimation::GetNFittedDYMCinMz_SR(){
   return NFittedDYMCinMz_SR ;
}

float DYEstimation::GetNDATAinMz_SR(){
   return  NDATAinMz_SR;
}

float DYEstimation::GetNDATAinMz_CR(){
   return  NDATAinMz_CR;
}

float DYEstimation::GetNDATA_CR(){
   return  NDATA_CR;
}

float DYEstimation::GetNDATA_BkgSubtracted_CR(){
//   return  NDATA_CR * (NDYMC_CR/NMC_CR);  // not very good
   return  NDATAinMz_CR * (NDYMC_CR /NDYMCinMz_CR );
}

float DYEstimation::GetNFittedDATA_CR(){
   return  NFittedDATA_CR;
}

float DYEstimation::GetNFittedDATAinMz_SR(){
   return  NFittedDATAinMz_SR;
}

float DYEstimation::GetNFittedDATAinMz_CR(){
   return  NFittedDATAinMz_CR;
}

float DYEstimation::GetNDATA_SR(){
   return  NDATA_SR;
}

float DYEstimation::GetContinuumInZdata(){
   return  ContinuumInZdata;
}

void DYEstimation::ComputeDYEstimatedEvents_DATA(string typecomputation,string useMCorDATAforScaleFactor ){

  if ( typecomputation=="Counting" ) {
     if ( NDATAinMz_SR-ContinuumInZdata >0.) {
//        NDYEstimatedDATACounting = NDATA_CR * (NDYMC_CR/NMC_CR) * (NDATAinMz_SR-ContinuumInZdata) / NDATAinMz_CR;
        NDYEstimatedDATACounting = GetNDATA_BkgSubtracted_CR() * (NDATAinMz_SR-ContinuumInZdata) / NDATAinMz_CR;
     } else {
//        NDYEstimatedDATACounting = NDATA_CR * (NDYMC_CR/NMC_CR) * NDATAinMz_SR / NDATAinMz_CR;
        NDYEstimatedDATACounting = GetNDATA_BkgSubtracted_CR() * NDATAinMz_SR / NDATAinMz_CR;
     }
     if ( DYVerbose ) { 
        std::cout<<"passage Counting ComputeDYEstimatedEvents_DATA "<< NDYEstimatedDATACounting<<" "<< NDATA_CR<<" "<< NDATAinMz_SR<<" "<<ContinuumInZdata<<" "<<NDATAinMz_CR<<std::endl; 
     } 
     if ( useMCorDATAforScaleFactor == "MC" ) {
        if ( NDYMCinMz_SR<=0 ) std::cout<<"Problem with NDYMCinMz_SR==0 "<<std::endl;
        if ( NDYMCinMz_CR<=0 ) std::cout<<"Problem with NDYMCinMz_CR==0 "<<std::endl;
//        NDYEstimatedDATACounting = NDATA_CR * (NDYMC_CR/NMC_CR) * NDYMCinMz_SR / NDYMCinMz_CR;
        NDYEstimatedDATACounting = GetNDATA_BkgSubtracted_CR() * NDYMCinMz_SR / NDYMCinMz_CR;
     }
  }

  if ( typecomputation=="Fit" ) {
     NDYEstimatedDATA = NFittedDATA_CR * NFittedDATAinMz_SR / NFittedDATAinMz_CR;
     if ( DYVerbose ) { 
        std::cout<<"passage Fit ComputeDYEstimatedEvents_DATA "<< NDYEstimatedDATA<<" "<<NFittedDATA_CR <<" "<< NFittedDATAinMz_SR<<" "<<NFittedDATAinMz_CR<<std::endl; 
     }
     if ( useMCorDATAforScaleFactor == "MC" ) {
        if ( NDYMCinMz_SR<=0 ) std::cout<<"Problem with NDYMCinMz_SR==0 "<<std::endl;
        if ( NDYMCinMz_CR<=0 ) std::cout<<"Problem with NDYMCinMz_CR==0 "<<std::endl;
        NDYEstimatedDATA = NFittedDATA_CR * NDYMCinMz_SR / NDYMCinMz_CR;
//        NDYEstimatedDATA = NDATA_CR * NDYMCinMz_SR / NDYMCinMz_CR;
     }
  }

  double stat_uncert_data = 0;
  double stat_uncert_data_counting = 0;
  if ( typecomputation=="Counting" ) {
    if ( NDATAinMz_SR-ContinuumInZdata> 0 ) {
//        stat_uncert_data_counting = (NDATAinMz_CR * (NDYMC_CR /NDYMCinMz_CR )+NDATAinMz_CR)/NDATAinMz_CR*1./sqrt(NDATAinMz_CR * (NDYMC_CR /NDYMCinMz_CR ))
//                                  + 1./sqrt(NDATAinMz_SR-ContinuumInZdata);
       stat_uncert_data_counting = sqrt( ( ((NDATAinMz_CR * (NDYMC_CR /NDYMCinMz_CR )+NDATAinMz_CR)/NDATAinMz_CR)*((NDATAinMz_CR * (NDYMC_CR /NDYMCinMz_CR )+NDATAinMz_CR)/NDATAinMz_CR))
                                 * (1./(NDATAinMz_CR * (NDYMC_CR /NDYMCinMz_CR )))
                                 + 1./(NDATAinMz_SR-ContinuumInZdata) );
     } else {
//        stat_uncert_data_counting = (NDATAinMz_CR * (NDYMC_CR /NDYMCinMz_CR )+NDATAinMz_CR)/NDATAinMz_CR*1./sqrt(NDATAinMz_CR * (NDYMC_CR /NDYMCinMz_CR ))
//                                  + 1./sqrt(NDATAinMz_SR);
       stat_uncert_data_counting = sqrt( ( ((NDATAinMz_CR * (NDYMC_CR /NDYMCinMz_CR )+NDATAinMz_CR)/NDATAinMz_CR)*((NDATAinMz_CR * (NDYMC_CR /NDYMCinMz_CR )+NDATAinMz_CR)/NDATAinMz_CR) )
                                 * (1./(NDATAinMz_CR * (NDYMC_CR /NDYMCinMz_CR )))
                                 + 1./(NDATAinMz_SR) );
     }                        
  }                           
// Not the right computation but a good approximation:
  if ( typecomputation=="Fit" ) {
//     stat_uncert_data = (NFittedDATA_CR+NFittedDATAinMz_CR)/NFittedDATAinMz_CR*1./sqrt(NFittedDATA_CR)
//                               + 1./sqrt(NFittedDATAinMz_SR);
// //     stat_uncert_data = (NDATA_CR+NFittedDATAinMz_CR)/NFittedDATAinMz_CR*1./sqrt(NDATA_CR)
// //                               + 1./sqrt(NFittedDATAinMz_SR);
    stat_uncert_data = sqrt( ( ((NFittedDATA_CR+NFittedDATAinMz_CR)/NFittedDATAinMz_CR)*((NFittedDATA_CR+NFittedDATAinMz_CR)/NFittedDATAinMz_CR) )
                              * (1./(NFittedDATA_CR))
                              + 1./(NFittedDATAinMz_SR) );
  }                           

// Systematical uncertainty related to the difference of the shape in the Control and Signal Regions
// This estimated with DY MC 
// (code /opt/sbg/data/data1/cms/gele/PAT/423/V2/CMSSW_4_2_3/src/MiniTreeAnalysis/NTupleAnalysis/macros/results_v1/test.C)
//  by comparing the absolute difference of the normalized shape in the
// SR and CR region: found to be 11% (mumu) and 4% (ee) -> mean = 8%
  double sys_shape = 0.08;    
    
// Systematical uncertainty related to the presence of residual background
  double sys_bckg = 0.;
  if ( typecomputation=="Fit" ) {
    sys_bckg = fabs(( NFittedDATAinMzWithScaledMinusContinuum_SR - NFittedDATAinMz_SR )/NFittedDATAinMz_SR);
    if ( sys_bckg>fabs(( NFittedDATAinMzWithScaledPlusContinuum_SR - NFittedDATAinMz_SR )/NFittedDATAinMz_SR) ) sys_bckg = fabs(( NFittedDATAinMzWithScaledPlusContinuum_SR - NFittedDATAinMz_SR )/NFittedDATAinMz_SR);
    std::cout<<"Relative Sys_Bckg "<<sys_bckg<<std::endl;
  }


  double sys_tot_1 = 0.;
  sys_tot_1 = sys_shape + sys_bckg;

    
  NDYEstimatedStatErrorDATA = NDYEstimatedDATA * stat_uncert_data;
  NDYEstimatedSysErrorDATA  = NDYEstimatedDATA * sys_tot_1;
  NDYEstimatedStatErrorDATACounting = NDYEstimatedDATACounting * stat_uncert_data_counting;
  NDYEstimatedSysErrorDATACounting  = NDYEstimatedDATACounting * sys_tot_1;
  
}

float DYEstimation::GetNDYEstimatedDATA(){
   return  NDYEstimatedDATA;
 
} 

float DYEstimation::GetNDYEstimatedStatErrorDATA(){
   return  NDYEstimatedStatErrorDATA;
 
} 

float DYEstimation::GetNDYEstimatedDATACounting(){
   return  NDYEstimatedDATACounting;
 
} 

float DYEstimation::GetNDYEstimatedStatErrorDATACounting(){
   return  NDYEstimatedStatErrorDATACounting;
 
} 

float DYEstimation::GetNDYEstimatedSysErrorDATA(){
   return  NDYEstimatedSysErrorDATA;
 
} 
float DYEstimation::GetNDYEstimatedSysErrorDATACounting(){
   return  NDYEstimatedSysErrorDATACounting;
 
} 

