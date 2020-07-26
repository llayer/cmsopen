#include "../interface/NTEvent.h"
using namespace TopTree;



NTEvent::NTEvent ():
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
  num_pileup_bc0(-1),
  num_pileup_bcp1(-1),
  num_pileup_bcm1(-1),
  partonFlavor1(- 999),
  partonFlavor2(- 999),
  x1(- 999),
  x2(- 999), 
  Q_scale(- 999)
{
}

NTEvent::~NTEvent ()
{
}

void
NTEvent::Reset ()
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
  match_HLT_Ele10_LW_L1R_recoEl = 0 ;
  p3BS.SetXYZ(0.0,0.0,0.0);
  p3BSErr.SetXYZ(0.0,0.0,0.0);
  triggers.clear();
  prescales.clear(); 
  tauObjTrig.clear();
  jetObjTrig.clear();
  electrons.clear ();
  muons.clear ();
  photons.clear ();
  tausVec.clear ();
  tracks.clear ();
  jetMetVec.clear ();
  VertexVec.clear ();
  Generatedtaus.clear ();
  GeneratedAtaus.clear ();
  genBquarks.clear ();
  genCquarks.clear ();
  genLquarks.clear ();
  genGquarks.clear ();
  wAndDecays.clear ();
  zAndDecays.clear ();
  topAndDecays.clear ();
  pdfWeights.clear ();
  pileup.clear ();
  rho_PUUE_dens = - 999;
  neutralRho_PUUE_dens = - 999;
  num_pileup_bc0  = -1;
  num_pileup_bcm1 = -1;
  num_pileup_bcp1 = -1;
  partonFlavor1 =- 999;
  partonFlavor2 =- 999;
  x1 =- 999;
  x2 =- 999;
  Q_scale =- 999;
}

NTTrack * NTEvent::NewTrack ()
{
  NTTrack evtrack;
  evtrack.Reset ();
  tracks.push_back (evtrack);
  return &(tracks.back ());
}

void NTEvent::NewTrack (const NTTrack& track)
{
  tracks.push_back (track);
}

NTElectron *
NTEvent::NewElectron ()
{
  NTElectron lepton;
  lepton.Reset ();
  electrons.push_back (lepton);
  return &(electrons.back ());

}


void NTEvent::NewElectron (const NTElectron& electron)
{
  electrons.push_back (electron);
}


NTPhoton *
NTEvent::NewPhoton ()
{
  NTPhoton thephoton;
  thephoton.Reset ();
  photons.push_back (thephoton);
  return &(photons.back ());

}

void NTEvent::NewPhoton (const NTPhoton& photon)
{
  photons.push_back (photon);
}




NTMuon *
NTEvent::NewMuon ()
{
  NTMuon lepton;
  lepton.Reset ();
  muons.push_back (lepton);
  return &(muons.back ());

}


void NTEvent::NewMuon (const NTMuon& muon)
{
  muons.push_back (muon);
}

TopTree::NTCollection<NTTau> *
NTEvent::NewTauColl ()
 { 
   TopTree::NTCollection<NTTau> lepton;
   lepton.Reset ();   
   tausVec.push_back (lepton);
   return &(tausVec.back ());
 
}

void NTEvent::NewTauColl (const TopTree::NTCollection<NTTau>& tau)
{
  tausVec.push_back (tau);
}

NTJetMet *
NTEvent::NewJetMet ()
{
  NTJetMet jetMet;
  jetMet.Reset ();
  jetMetVec.push_back (jetMet);
  return &(jetMetVec.back ());
}

NTVertex *
NTEvent::NewVertex ()
{
  NTVertex pvx;
  pvx.Reset ();
  VertexVec.push_back (pvx);
  return &(VertexVec.back ());
}

void NTEvent::NewVertex (const NTVertex& vertex)
{
  VertexVec.push_back (vertex);
}

NTPU *
NTEvent::NewPU ()
{
  NTPU newpu;
  newpu.Reset ();
  pileup.push_back (newpu);
  return &(pileup.back ());

}
void NTEvent::NewPU (const NTPU& pile)
{
  pileup.push_back (pile);
}



    
void NTEvent::NewJetMet(const NTJetMet& jetmet){
  jetMetVec.push_back(jetmet);
}

void NTEvent::PrintTriggerPassed (std::ostream & os){
        os << "Triggers passed : ";
        for(unsigned int i=0;i<triggers.size();i++){
                if(triggers[i].second) os << triggers[i].first << " ";
        }
        os <<endl;
}

void NTEvent::PrintTriggerList (std::ostream & os){
        os << "List of triggers"<<endl;
        for(unsigned int i=0;i<triggers.size();i++){
                os << triggers[i].first << " ";
        }
        os << endl;
}

bool NTEvent::TriggerPassed(std::string trigName){
        for(unsigned int i=0;i<triggers.size();i++){
                if(triggers[i].first == trigName)
                        return triggers[i].second;
        }
        return false;
}

bool NTEvent::TriggerStored(std::string trigName){
        for(unsigned int i=0;i<triggers.size();i++){
                if(triggers[i].first == trigName)
                        return true;
        }
        return false;
}

void
NTEvent::Print (std::ostream & os)
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
              os << tausVec[i].objects.size() << " " << tausVec[i].algo<<"  -"<<endl;
        }
        os << tracks.size() << " selected tracks -";
        os << endl;
}

