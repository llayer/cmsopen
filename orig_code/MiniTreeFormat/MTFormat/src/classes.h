#include "MiniTreeFormat/MTFormat/interface/ZJetsEvtMC.h"
#include "MiniTreeFormat/MTFormat/interface/WJetsEvtMC.h"
#include "MiniTreeFormat/MTFormat/interface/TTbarEvtMC.h"
#include "MiniTreeFormat/MTFormat/interface/WWEvtMC.h"

#include "MiniTreeFormat/MTFormat/interface/MTLepton.h"
#include "MiniTreeFormat/MTFormat/interface/MTElectron.h"
#include "MiniTreeFormat/MTFormat/interface/MTMuon.h"
#include "MiniTreeFormat/MTFormat/interface/MTTau.h"
#include "MiniTreeFormat/MTFormat/interface/MTJet.h"
#include "MiniTreeFormat/MTFormat/interface/MTMET.h"
#include "MiniTreeFormat/MTFormat/interface/MTJetMet.h"
#include "MiniTreeFormat/MTFormat/interface/MTVertex.h"
#include "MiniTreeFormat/MTFormat/interface/MTTrack.h"
#include "MiniTreeFormat/MTFormat/interface/MTPhoton.h"
#include "MiniTreeFormat/MTFormat/interface/MTEvent.h"
#include "MiniTreeFormat/MTFormat/interface/MTCollection.h"
#include "MiniTreeFormat/MTFormat/interface/MTPU.h"

#include <vector>

namespace{
  namespace{
    edm::Wrapper<std::vector<TLorentzVector > > dummy;
    edm::Wrapper<TopTree::MTLepton>      MTLep;
    edm::Wrapper<TopTree::MTElectron>    MTElec;
    edm::Wrapper<TopTree::MTMuon>    MTMuon;
    edm::Wrapper<TopTree::MTTau>    MTTau;
    edm::Wrapper<TopTree::MTJet>    MTJet;
    edm::Wrapper<TopTree::MTMET>    MTMET;
    edm::Wrapper<TopTree::MTJetMet>    MTJetMet;
    edm::Wrapper<TopTree::MTVertex>    MTVertex;
    edm::Wrapper<TopTree::MTTrack>    MTTrack;
    edm::Wrapper<TopTree::MTPhoton>    MTPhoton;
    edm::Wrapper<std::vector<TopTree::MTJetMet> >     VecMTJetMet;
    edm::Wrapper<std::vector<TopTree::MTJet> >        VecMTJet; 
    edm::Wrapper<std::vector<TopTree::MTElectron> >   VecMTElectron;
    edm::Wrapper<std::vector<TopTree::MTMuon> >       VecMTMuon;
    edm::Wrapper<std::vector<TopTree::MTTau> >        VecMTTau;
    edm::Wrapper<std::vector<TopTree::MTTrack> >      VecMTTrack;
    edm::Wrapper<std::vector<TopTree::MTPhoton> >      VecMTPhoton;
    edm::Wrapper<std::vector<TopTree::MTVertex> >      VecMTVertex;
    edm::Wrapper<TopTree::MTEvent>    MTEvent;
    edm::Wrapper<TopTree::TTbarEvtMC>               ttbarmc;
    edm::Wrapper<TopTree::WJetsEvtMC>               wjetsmc;
    edm::Wrapper<TopTree::ZJetsEvtMC>               zjetsmc;
    edm::Wrapper<TopTree::WWEvtMC>                  wwjetsmc;
    edm::Wrapper<TopTree::MTPU>    MTPU;
    
    
    edm::Wrapper<TopTree::MTCollection<TopTree::MTJet> > MTCollJet;
    edm::Wrapper<TopTree::MTCollection<TopTree::MTTau> > MTCollectionTau;
    edm::Wrapper<std::vector<TopTree::MTCollection<TopTree::MTTau> > >  VecMTCollectionTau;
    
  }
}

