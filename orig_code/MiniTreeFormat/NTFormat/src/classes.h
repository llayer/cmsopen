#include "MiniTreeFormat/NTFormat/interface/NTLepton.h"
#include "MiniTreeFormat/NTFormat/interface/NTElectron.h"
#include "MiniTreeFormat/NTFormat/interface/NTMuon.h"
#include "MiniTreeFormat/NTFormat/interface/NTTau.h"
#include "MiniTreeFormat/NTFormat/interface/NTJet.h"
#include "MiniTreeFormat/NTFormat/interface/NTMET.h"
#include "MiniTreeFormat/NTFormat/interface/NTJetMet.h"
#include "MiniTreeFormat/NTFormat/interface/NTVertex.h"
#include "MiniTreeFormat/NTFormat/interface/NTTrack.h"
#include "MiniTreeFormat/NTFormat/interface/NTPhoton.h"
#include "MiniTreeFormat/NTFormat/interface/NTEvent.h"
#include "MiniTreeFormat/NTFormat/interface/NTCollection.h"
#include "MiniTreeFormat/NTFormat/interface/topDecaysMC.h"
#include "MiniTreeFormat/NTFormat/interface/WDecaysMC.h"
#include "MiniTreeFormat/NTFormat/interface/ZDecaysMC.h"
#include "MiniTreeFormat/NTFormat/interface/NTPU.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"//needed to know edm namespace

#include <vector>

namespace{
  namespace{
    edm::Wrapper<std::map<int   , std::vector<int> > >       dummy1;
    edm::Wrapper<std::pair<float, std::string> >             dummy2;
    edm::Wrapper<std::vector<std::pair<float, std::string> > > dummy3;
    
    edm::Wrapper<TopTree::NTLepton>      NTLep;
    edm::Wrapper<TopTree::NTElectron>    NTElec;
    edm::Wrapper<TopTree::NTMuon>    NTMuon;
    edm::Wrapper<TopTree::NTTau>    NTTau;
    edm::Wrapper<TopTree::NTJet>    NTJet;
    edm::Wrapper<TopTree::NTMET>    NTMET;
    edm::Wrapper<TopTree::NTJetMet>   NTJetMet;
    edm::Wrapper<TopTree::NTVertex>   NTVertex;
    edm::Wrapper<TopTree::NTTrack>    NTTrack;
    edm::Wrapper<TopTree::NTPhoton>   NTPhoton;
    edm::Wrapper<TopTree::TopDecaysMC> TopDecays;
    edm::Wrapper<TopTree::WDecaysMC> WDecays;
    edm::Wrapper<TopTree::ZDecaysMC> ZDecays;
    edm::Wrapper<std::vector<TopTree::NTJetMet> >     VecNTJetMet;
    edm::Wrapper<std::vector<TopTree::NTJet> >        VecNTJet; 
    edm::Wrapper<std::vector<TopTree::NTElectron> >   VecNTElectron;
    edm::Wrapper<std::vector<TopTree::NTMuon> >       VecNTMuon;
    edm::Wrapper<std::vector<TopTree::NTTau> >        VecNTTau;
    edm::Wrapper<std::vector<TopTree::NTTrack> >      VecNTTrack;
    edm::Wrapper<std::vector<TopTree::NTPhoton> >     VecNTPhoton;
    edm::Wrapper<std::vector<TopTree::NTVertex> >      VecNTVertex;
    edm::Wrapper<TopTree::NTCollection<TopTree::NTTau > > NTCollTau;
    edm::Wrapper<std::vector<TopTree::NTCollection<TopTree::NTTau > > > VecNTCollectionTau;
    edm::Wrapper<TopTree::NTCollection<TopTree::NTJet> > NTCollJet;
    edm::Wrapper<std::vector<TopTree::TopDecaysMC> > VecTopDecays;
    edm::Wrapper<std::vector<TopTree::WDecaysMC> > VecWDecays;
    edm::Wrapper<std::vector<TopTree::ZDecaysMC> > VecZDecays;
    edm::Wrapper<TopTree::NTEvent>    NTEvent;
    edm::Wrapper<TopTree::NTPU>   NTPU;
  }
}
