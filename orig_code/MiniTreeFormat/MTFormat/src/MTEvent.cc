#include "../interface/MTEvent.h"

using namespace TopTree;

MTEvent::MTEvent ():
  runNb(0),
  eventNb(0),
  eventWeight(-999.),
  lumiblock(-999),
  procId(- 999),
  eventType(""),
  TMEME(0),
  ptHat(0),
  pdgIdPartons(pair<int,int>(0,0)),
  match_HLT_Ele10_LW_L1R_recoEl(0),
  p3BS(0.0,0.0,0.0),
  p3BSErr(0.0,0.0,0.0), 
  rho_PUUE_dens(- 999), 
  neutralRho_PUUE_dens(- 999),
  num_pileup_bc0(0),
  partonFlavor1(- 999),
  partonFlavor2(- 999),
  x1(- 999),
  x2(- 999),
  Q_scale(- 999)
 
  
  /*
  ttbarevtmc (0), 
  wjetsevtmc (0), 
  zjetsevtmc (0), 
  wwevtmc (0)
  */
{
}

MTEvent::~MTEvent ()
{
/*
    delete ttbarevtmc;                      
    delete wjetsevtmc;
    delete zjetsevtmc;
    delete wwevtmc;
*/
}

void
MTEvent::Reset ()
{
  runNb = 0;
  eventNb = 0;
  eventWeight = -999.;
  lumiblock = -999;
  procId = - 999;
  eventType = "";
  TMEME = 0;
  ptHat = 0;
  pdgIdPartons = pair<int,int>(0,0);
  match_HLT_Ele10_LW_L1R_recoEl = 0;
  p3BS.SetXYZ(0.0,0.0,0.0);
  p3BSErr.SetXYZ(0.0,0.0,0.0);
  partonFlavor1 =- 999;
  partonFlavor2 = - 999;
  x1 = - 999;
  x2 = - 999;
  Q_scale = - 999;
  rho_PUUE_dens= - 999;
  neutralRho_PUUE_dens= - 999;
  num_pileup_bc0 = 0;
/*
  delete ttbarevtmc;                      
  delete wjetsevtmc;
  delete zjetsevtmc;
  delete wwevtmc;
*/
  //ttbarevtmc.Reset();                      
  //wjetsevtmc.Reset();
  //zjetsevtmc.Reset();
  //wwevtmc.Reset();
  
  triggers.clear(); 
  prescales.clear(); 
  tauObjTrig.clear();
  jetObjTrig.clear(); 
  electrons.clear ();
  photons.clear ();
  muons.clear ();
  tausVec.clear ();
  tracks.clear ();
  jetMetVec.clear ();
  VertexVec.clear ();
  GPC.clear();
  wAndDecays.clear ();
  zAndDecays.clear ();
  topAndDecays.clear ();
  genBquarks.clear ();
  genCquarks.clear ();
  genLquarks.clear ();
  genGquarks.clear ();
  Generatedtaus.clear ();
  GeneratedAtaus.clear ();
  pileup.clear ();
  pdfWeights.clear ();
}

MTTrack * MTEvent::NewTrack ()
{
  MTTrack evtrack;
  evtrack.Reset ();
  tracks.push_back (evtrack);
  return &(tracks.back ());
}


MTElectron *
MTEvent::NewElectron ()
{
  MTElectron lepton;
  lepton.Reset ();
  electrons.push_back (lepton);
  return &(electrons.back ());

}


MTPhoton *
MTEvent::NewPhoton ()
{
  MTPhoton thephoton;
  thephoton.Reset ();
  photons.push_back (thephoton);
  return &(photons.back ());

}

MTMuon *
MTEvent::NewMuon ()
{
  MTMuon lepton;
  lepton.Reset ();
  muons.push_back (lepton);
  return &(muons.back ());

}

MTCollection<MTTau> *
MTEvent::NewTauColl ()
{
  MTCollection<MTTau> lepton;
  lepton.Reset ();
  tausVec.push_back (lepton);
  return &(tausVec.back ());

}

MTJetMet *
MTEvent::NewJetMet ()
{
  MTJetMet jetMet;
  jetMet.Reset ();
  jetMetVec.push_back (jetMet);
  return &(jetMetVec.back ());
}

MTVertex *
MTEvent::NewVertex ()
{
  MTVertex pvx;
  pvx.Reset ();
  VertexVec.push_back (pvx);
  return &(VertexVec.back ());
}

MTPU *
MTEvent::NewPU ()
{
  MTPU newpu;
  newpu.Reset ();
  pileup.push_back (newpu);
  return &(pileup.back ());

}

/*
TTbarEvtMC *
MTEvent::NewTTbarEvtMC ()
{
  ttbarevtmc = new TTbarEvtMC ();
  return ttbarevtmc;
}

WJetsEvtMC *
MTEvent::NewWJetsEvtMC ()
{
  wjetsevtmc = new WJetsEvtMC ();
  return wjetsevtmc;
}

ZJetsEvtMC *
MTEvent::NewZJetsEvtMC ()
{
  zjetsevtmc = new ZJetsEvtMC ();
  return zjetsevtmc;
}

WWEvtMC *
MTEvent::NewWWEvtMC ()
{
  wwevtmc = new WWEvtMC ();
  return wwevtmc;
}
*/

void MTEvent::PrintTriggerPassed (std::ostream & os){
        os << "Triggers passed : ";
        for(unsigned int i=0;i<triggers.size();i++){
                if(triggers[i].second) os << triggers[i].first << " ";
        }
        os <<endl;
}

void MTEvent::PrintTriggerList (std::ostream & os){
        os << "List of triggers"<<endl;
        for(unsigned int i=0;i<triggers.size();i++){
                os << triggers[i].first << " ";
        }
        os << endl;
}

bool MTEvent::TriggerPassed(std::string trigName){
        for(unsigned int i=0;i<triggers.size();i++){
                if(triggers[i].first == trigName)
                        return triggers[i].second;
        }
        return false;
}

bool MTEvent::TriggerStored(std::string trigName){
        for(unsigned int i=0;i<triggers.size();i++){
                if(triggers[i].first == trigName)
                        return true;
        }
        return false;
}

void
MTEvent::Print (std::ostream & os)
{
        os << " -- Event "<< eventNb <<" from run "<<runNb<<" & lumiBlock "<<lumiblock<<endl;
        os << " EventType: "<<eventType<<" ProcessId = "<<procId<<" eventWeight = "<<eventWeight<<" TMEME = "<<TMEME;
        PrintTriggerPassed(os); 
        os << "Initial partons (pdgId) "<<pdgIdPartons.first<<" & "<<pdgIdPartons.second<<endl; 
        os << electrons.size() << " selected electrons - ";
        os << muons.size() << " selected muons - ";
        os << endl;
        for (unsigned int i=0;i<jetMetVec.size();i++){
                os << jetMetVec[i].objects.size() << " " << jetMetVec[i].algo<<" jets -"<<endl;
        }
        for (unsigned int i=0;i<tausVec.size();i++){
               os << tausVec[i].objects.size() << " " << tausVec[i].algo<<" -"<<endl;
        }
        os << tracks.size() << " selected tracks -";
        os << endl;
}





void
MTEvent::fillGenInfo(edm::Handle < reco::GenParticleCollection > genParticles){
 
 
 int tmeme = 0;
 int idum = 0;
 for (GenParticleCollection::const_iterator p = genParticles->begin (); p != genParticles->end (); ++p) {
    
    const reco::Candidate *W = &(*genParticles)[idum];
    idum +=1;
   
    TLorentzVector thequark;
    thequark.SetPxPyPzE (p->p4 ().Px (), p->p4 ().Py (), p->p4 ().Pz (), p->p4 ().E ());
    //if( abs(p->pdgId () ) == 5 ) cout << p->status() << endl;
    if ( abs(p->pdgId () ) == 5 && p->status() == 3 ) genBquarks.push_back(thequark);
    if ( abs(p->pdgId () ) == 4 && p->status() == 3 ) genCquarks.push_back(thequark);
    if ((abs(p->pdgId () ) == 1 
        || abs(p->pdgId () ) == 2 
        || abs(p->pdgId () ) == 3 )
        &&p->status() == 3 && p->p4().Pt() > 10) genLquarks.push_back(thequark);
    if ( abs(p->pdgId () ) == 22 && p->p4().Pt() > 10 ) genGquarks.push_back(thequark);
    
    
   //******************************
   //****** add top
   //******************************
   if ( abs(p->pdgId () ) == 6 ){
     // rewritte the info twice top/anti-top but it was checked that their mothers are the same
     if(p->numberOfMothers()>=1) this->pdgIdPartons.first = p->mother(0)->pdgId();
     if(p->numberOfMothers()>=2) this->pdgIdPartons.second = p->mother(1)->pdgId();
     TopDecaysMC topQuark;
     topQuark.p4_t_gen.SetPxPyPzE (p->p4 ().Px (), p->p4 ().Py (), p->p4 ().Pz (), p->p4 ().E ());
     topQuark.Q_gen = p->charge();
     const GenParticle & paIt = *p;
     for (unsigned int j = 0; j < paIt.numberOfDaughters (); j++) {
       if ( abs(paIt.daughter (j)->pdgId ()) == 5) topQuark.p4_b_gen.SetPxPyPzE (paIt.p4 ().Px (), paIt.p4 ().Py (), paIt.p4 ().Pz (), paIt.p4 ().E ());
     }
     this->topAndDecays.push_back(topQuark);
   }
   
   //******************************
   //****** add Z
   //******************************
 
 
   if (abs (p->pdgId ()) == 23 && p->status() == 3){ 
     
     const reco::Candidate * ZDaughter = 0;
     const reco::Candidate * Tau = 0;
     const reco::Candidate * TauDaughter = 0;
     const reco::Candidate * TauNu1 = 0;
     const reco::Candidate * TauNu2 = 0;
     const reco::Candidate * TauANu1 = 0;
     const reco::Candidate * TauANu2 = 0;
     TLorentzVector v;
     
     bool Ztobb = false;
     ZDecaysMC zBoson;
     zBoson.p4_Z_gen.SetPxPyPzE (p->p4 ().Px (), p->p4 ().Py (), p->p4 ().Pz (), p->p4 ().E ());
    
     for (unsigned int d = 0; d < p->numberOfDaughters (); d++) {
       //ZDaughter = const_cast<reco::Candidate*> (p->daughter(d));
       ZDaughter = p->daughter (d);
       if (abs (ZDaughter->pdgId ()) == 5)
         Ztobb = true;
       if (ZDaughter->status () == 3) {
         if (ZDaughter->pdgId () == 11 || ZDaughter->pdgId () == 13 || ZDaughter->pdgId () == 15) {
           zBoson.p4_Lep1_gen.SetPxPyPzE (ZDaughter->p4 ().Px (), ZDaughter->p4 ().Py (), ZDaughter->p4 ().Pz (), ZDaughter->p4 ().E ());
           zBoson.Q_Lep1_gen = -1;
           zBoson.Lep1_pdgID = ZDaughter->pdgId ();        
         }
         if (ZDaughter->pdgId () == -11 || ZDaughter->pdgId () == -13 || ZDaughter->pdgId () == -15) {
           zBoson.p4_Lep2_gen.SetPxPyPzE (ZDaughter->p4 ().Px (), ZDaughter->p4 ().Py (), ZDaughter->p4 ().Pz (), ZDaughter->p4 ().E ());
           zBoson.Q_Lep2_gen = 1;
           zBoson.Lep2_pdgID = ZDaughter->pdgId (); 
         }
           
         if (abs (ZDaughter->pdgId ()) == 11)
           tmeme = tmeme + 1;
         if (abs (ZDaughter->pdgId ()) == 13)
           tmeme = tmeme + 10;
         
         if (abs (ZDaughter->pdgId ()) == 15) {
           Tau = ZDaughter; 
           TauDaughter = ZDaughter->daughter (0);
           //while (abs (TauDaughter->pdgId ()) == 15 && TauDaughter->numberOfDaughters ()>0) {
             while (abs (TauDaughter->pdgId ()) == 15 && TauDaughter->numberOfDaughters()>0) {       
             Tau = TauDaughter;
             //TauDaughter = const_cast<reco::Candidate*> (TauDaughter->daughter(0));
             TauDaughter = TauDaughter->daughter (0);
           }
            
           if (Tau) {
             bool lepDecay = false; 
             bool stableTau = false;
            
             for (unsigned int k = 0; k < Tau->numberOfDaughters (); k++) {
               //if      ( Tau->daughter(k)->pdgId() == 16)  { TauNu1  = const_cast<reco::Candidate*> (Tau->daughter(k));
               if (Tau->daughter (k)->pdgId () == 16) {
                 TauNu1 = Tau->daughter (k);
               }
               //else if ( Tau->daughter(k)->pdgId() == -16) { TauANu1 = const_cast<reco::Candidate*> (Tau->daughter(k));
               else if (Tau->daughter (k)->pdgId () == -16) {
                 TauANu1 = Tau->daughter (k);
               }
               else if (Tau->daughter (k)->pdgId () == 12 ||
                        //Tau->daughter(k)->pdgId() == 14)   { TauNu2 = const_cast<reco::Candidate*> (Tau->daughter(k));
                        Tau->daughter (k)->pdgId () == 14) {
                 TauNu2 = Tau->daughter (k);
               }
               else if (Tau->daughter (k)->pdgId () == -12 ||
                        //Tau->daughter(k)->pdgId() == -14)  { TauANu2 = const_cast<reco::Candidate*> (Tau->daughter(k));
                        Tau->daughter (k)->pdgId () == -14) {
                 TauANu2 = Tau->daughter (k);
               }
               else if (abs (Tau->daughter (k)->pdgId ()) == 11) {
                 tmeme = tmeme + 10100;
                 lepDecay = true;
               }
               else if (abs (Tau->daughter (k)->pdgId ()) == 13) {
                 tmeme = tmeme + 11000;
                 lepDecay = true;
               }
               else if  (abs (Tau->daughter (k)->pdgId ()) == 15){        
                 stableTau = true;
               }
             }
              
             if (lepDecay == false && stableTau == false) {
               tmeme = tmeme + 10000;
             }
             if (stableTau == true) {
               tmeme += -99999;
             }
             
           }
         }
       }
     }
     
     this->zAndDecays.push_back(zBoson);
     ZDaughter = 0;
     Tau = 0;
     TauDaughter = 0;
     TauNu1 = 0;
     TauNu2 = 0;
     TauANu1 = 0;
     TauANu2 = 0;
   
     delete ZDaughter;
     delete Tau;
     delete TauDaughter;
     delete TauNu1;
     delete TauNu2;
     delete TauANu1;
     delete TauANu2;
   }
   
   //******************************
   //****** add W
   //******************************
   if ( abs(p->pdgId () ) == 24 && p->status() == 3){
   
     const Candidate *WDaughter = 0;
     const Candidate *TauDaughter = 0;
     const Candidate *Tau = 0;
     const Candidate *TauNu1 = 0;
     const Candidate *TauNu2 = 0;
     
     WDecaysMC wBosons;
     wBosons.p4_W_gen.SetPxPyPzE (p->p4 ().Px (), p->p4 ().Py (), p->p4 ().Pz (), p->p4 ().E ());
     if(p->pdgId () == 24) wBosons.Q_Lep_gen = -1;
     else                  wBosons.Q_Lep_gen = 1;
     
     for (unsigned int d = 0; d < p->numberOfDaughters (); d++) {
       WDaughter = p->daughter (d);
      
       if (WDaughter->status () == 3) {
       
         if (abs (WDaughter->pdgId ()) == 11 || abs (WDaughter->pdgId ()) == 13 || abs (WDaughter->pdgId ()) == 15) {
           wBosons.p4_Lep_gen.SetPxPyPzE (WDaughter->p4 ().Px (), WDaughter->p4 ().Py (), WDaughter->p4 ().Pz (), WDaughter->p4 ().E ());
           wBosons.Q_Lep_gen = (WDaughter->pdgId () > 0) ? -1 : 1;
           wBosons.mcLepId = WDaughter->pdgId ();
           if (abs (WDaughter->pdgId ()) == 11)
             tmeme = tmeme + 1;
           if (abs (WDaughter->pdgId ()) == 13)
             tmeme = tmeme + 10;
         }
         
         if (abs (WDaughter->pdgId ()) == 12 || abs (WDaughter->pdgId ()) == 14 || abs (WDaughter->pdgId ()) == 16) {
           wBosons.p4_Neu_gen.SetPxPyPzE (WDaughter->p4 ().Px (), WDaughter->p4 ().Py (), WDaughter->p4 ().Pz (), WDaughter->p4 ().E ());
         }
         
         if (abs (WDaughter->pdgId ()) == 15) {
           Tau = WDaughter->daughter (0);
           TauDaughter = WDaughter->daughter (0);
           while (abs (TauDaughter->pdgId ()) == 15 && TauDaughter->numberOfDaughters()>0) {
             Tau = TauDaughter;
             TauDaughter = TauDaughter->daughter (0);
            
           }
           
           if (Tau) {
             bool lepDecay = false;
             bool stableTau = false;
             
             for (unsigned int k = 0; k < Tau->numberOfDaughters (); k++) { 
                
               if (abs (Tau->daughter (k)->pdgId ()) == 11) {
                 tmeme = tmeme + 10100;
                 lepDecay = true;
               }
               else if (abs (Tau->daughter (k)->pdgId ()) == 13) {
                 tmeme = tmeme + 11000;
                 lepDecay = true;
               }
               else if (abs (Tau->daughter (k)->pdgId ()) == 16) {
                 TauNu1 = Tau->daughter (k);//what for, one should store anti-nu as well... 
               }
               
               else if (abs (Tau->daughter (k)->pdgId ()) == 12 || abs (Tau->daughter (k)->pdgId () == 14)) {
                 TauNu2 = Tau->daughter (k);//what for, one should store anti-nu as well...
               }
               else if (abs (Tau->daughter (k)->pdgId ()) == 15) {      
                 stableTau = true;
               }             
              }
             if (lepDecay == false)
               tmeme = tmeme + 10000;
             if (stableTau == true)
               tmeme = -99999;       
           }
                   
         }
          
         /*
        int indxtau = -1;
        for (unsigned int k = 0; k < W->numberOfDaughters (); k++) { 
        std::cout << "Second Loop over W daughters " << W->daughter (k)->pdgId ()<< " " <<  W->daughter (k)->status ()<<std::endl; 
          if (abs (W->daughter (k)->pdgId ()) == 15)
            indxtau = k;
        }
        while (indxtau >= 0) {
          if (!W)
            cout << "NULL " << endl;
            std::cout <<"indxtau " << std::endl;
          bool FoundTau = false;
          for (unsigned int k = 0; k < W->numberOfDaughters (); k++) {
            if (abs (W->daughter (k)->pdgId ()) == 24)
              continue;
            if (abs (W->daughter (k)->pdgId ()) == 11 || abs (W->daughter (k)->pdgId ()) == 13) {
              if (abs (W->daughter (k)->pdgId ()) == 11)
                tmeme += 100;
              if (abs (W->daughter (k)->pdgId ()) == 13)
                tmeme += 1000;
              indxtau = -1;
            }
            if (abs (W->daughter (k)->pdgId ()) == 15) {
              indxtau = k;
              FoundTau = true;
            }

            if (abs (W->pdgId ()) == 15) {
              if ( abs(W->daughter (k)->pdgId ()) == 16) {
                TauNu1 = W->daughter (k);
              }
//            else if (W->daughter (k)->pdgId () == -16) {
//              TauANu1 = W->daughter (k);
//            }
              else if (abs(W->daughter (k)->pdgId ()) == 12 || abs(W->daughter (k)->pdgId ()) == 14) {
                TauNu2 = W->daughter (k);
              }
//            else if (W->daughter (k)->pdgId () == -12 || W->daughter (k)->pdgId () == -14) {
//              TauANu2 = W->daughter (k);
//            }
            }
          }
          if (FoundTau) {
            W = W->daughter (indxtau);
          }
          else {
            indxtau = -1;
          }
        }*/

      }
     } 
     
     if(TauNu1!=0 && TauNu2!=0)
     wBosons.p4_SumTauNeu_gen.SetPxPyPzE(TauNu1->p4().Px()+TauNu2->p4().Px(),TauNu1->p4().Py()+TauNu2->p4().Py(),TauNu1->p4().Pz()+TauNu2->p4().Pz(), TauNu1->p4().E()+TauNu2->p4().E());
     else if (TauNu1!=0 && TauNu2==0) 
     wBosons.p4_SumTauNeu_gen.SetPxPyPzE(TauNu1->p4().Px(),TauNu1->p4().Py(),TauNu1->p4().Pz(), TauNu1->p4().E());
     else wBosons.p4_SumTauNeu_gen.SetPxPyPzE(0.,0.,0.,0.);
    
     this->wAndDecays.push_back(wBosons);
     WDaughter = 0;
     TauDaughter = 0;
     Tau = 0;
     TauNu1 = 0;
     TauNu2 = 0;
     delete WDaughter;
     delete TauDaughter;
     delete Tau;
     delete TauNu1;
     delete TauNu2;
   }
   
   
    //******************************
   //****** add taus
   //******************************
   
   if ( p->pdgId () == 15 ){  
     TLorentzVector p4(p->px(),p->py(),p->pz(),p->energy());
     this->Generatedtaus.push_back(p4);}
   
   if ( p->pdgId () == -15 ){  
     TLorentzVector p4(p->px(),p->py(),p->pz(),p->energy());
     this->GeneratedAtaus.push_back(p4);}
   
  }
 
  this->TMEME = tmeme;
 
}


