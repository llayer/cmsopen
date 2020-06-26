#include "TopQuarkAnalysis/MiniTreeProducer/interface/MiniTreeProducer.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
//#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/MuonReco/interface/MuonMETCorrectionData.h"
#include "DataFormats/Math/interface/Point3D.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"

#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "CondFormats/JetMETObjects/interface/FactorizedJetCorrector.h"
#include <SimDataFormats/GeneratorProducts/interface/PdfInfo.h>
#include <SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h>
#include <RecoEgamma/EgammaTools/interface/ConversionFinder.h>

#include "PhysicsTools/PatUtils/interface/TriggerHelper.h"

//For electron-conversion
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "MagneticField/Engine/interface/MagneticField.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/Scalers/interface/DcsStatus.h"

#include "DataFormats/PatCandidates/interface/TriggerObject.h"


typedef math::XYZPoint Point;
using namespace std;


MiniTreeProducer::MiniTreeProducer (const edm::ParameterSet & iConfig)
{
  cout << "Constructor of MiniTreeProducer" << endl;

  // -------------  get cfg file information:

  verbose         = iConfig.getParameter < int >              ("verbose");
  readPDFWeights  = iConfig.getParameter < bool >             ("readPDFWeights");
  TriggerMenu     = iConfig.getParameter < InputTag >         ("TriggerMenu");
  TriggerInfo     = iConfig.getParameter < bool >             ("TriggerInfo");
  saveAllTriggers = iConfig.getParameter < bool >             ("saveAllTriggers");
  triggerList     = iConfig.getParameter < vector < string > >("triggerList");
  prescaleList    = iConfig.getParameter < vector < string > >("prescaleList");
  hltJetsMatcher  = iConfig.getParameter < string >           ("hltJetsMatcher");
  hltTausMatcher  = iConfig.getParameter < string >           ("hltTausMatcher");
  hltJetsMatcher45= iConfig.getParameter < string >           ("hltJetsMatcher45");
  hltTausMatcher45= iConfig.getParameter < string >           ("hltTausMatcher45");

  electronProducer  = iConfig.getParameter < InputTag > ("electronProducer");
  doElectrons       = iConfig.getParameter < bool >     ("doElectrons");
  electron_cut_pt   = iConfig.getParameter < double >   ("electron_cut_pt");
  electron_cut_eta  = iConfig.getParameter < double >   ("electron_cut_eta");

  photonProducer  = iConfig.getParameter < InputTag > ("photonProducer");
  doPhotons       = iConfig.getParameter < bool >     ("doPhotons");
  photon_cut_pt   = iConfig.getParameter < double >   ("photon_cut_pt");
  photon_cut_eta  = iConfig.getParameter < double >   ("photon_cut_eta");

  muonProducer = iConfig.getParameter < InputTag > ("muonProducer");
  doMuons      = iConfig.getParameter < bool >     ("doMuons");
  muon_cut_pt  = iConfig.getParameter < double >   ("muon_cut_pt");
  muon_cut_eta = iConfig.getParameter < double >   ("muon_cut_eta");

  tauProducer  = iConfig.getParameter < vector < string> >("tauProducer");
  doTaus       = iConfig.getParameter < bool >     ("doTaus");
  tau_cut_pt   = iConfig.getParameter < double >   ("tau_cut_pt");
  tau_cut_eta  = iConfig.getParameter < double >   ("tau_cut_eta");

  trackProducer    = iConfig.getParameter < InputTag > ("trackProducer");
  doTracks         = iConfig.getParameter < bool >     ("doTracks");
  track_cut_pt     = iConfig.getParameter < double >   ("track_cut_pt");
  track_cut_eta    = iConfig.getParameter < double >   ("track_cut_eta");
  beamSpotProducer = iConfig.getParameter < InputTag > ("beamSpotProducer");

  jetmetProducer = iConfig.getParameter < VParameters > ("jetmetProducer");
  jet_cut_pt     = iConfig.getParameter < double >      ("jet_cut_pt");
  jet_cut_eta    = iConfig.getParameter < double >      ("jet_cut_eta");

  rho_PUUE_dens  =  iConfig.getParameter < InputTag >      ("rho_PUUE_dens");
  neutralRho_PUUE_dens  =  iConfig.getParameter < InputTag >      ("neutralRho_PUUE_dens");

  vertexProducer = iConfig.getParameter < InputTag > ("vertexProducer");
  saveAllVertex  = iConfig.getParameter < bool >     ("saveAllVertex");


  doGenParticleCollection         = iConfig.getParameter < bool >     ("doGenParticleCollection");
  //register product
  produces < TopTree::MTEvent > ();
  isAOD           = iConfig.getParameter < bool >             ("isAOD");
  isData          = iConfig.getParameter < bool >             ("isData");
  cout << "End of MiniTreeProducer constructor" << endl;
}


MiniTreeProducer::~MiniTreeProducer ()
{
  //TopTree objects
  vertexAlgo = 0;
  delete vertexAlgo;
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void MiniTreeProducer::produce (edm::Event & iEvent, const edm::EventSetup & iSetup)
{
  using namespace edm;
  using namespace IPTools;

  if (verbose > 0) {
    cout << "--------- new event ---------" << endl;
  }






  if (verbose > 1)
    cout << "Initializing TopTree objects" << endl;

  // --------------------
  //    TopTree objects
  // --------------------
  //TopTreeTrack    *tttrack  = 0;
  //TopTreeBS       *bsp      = 0;
  //TopTreePVx      *pvx      = 0;
  //TopTreeElectron *electron = 0;
  //TopTreeMuon *muon         = 0;
  //TopTreeTau *tau           = 0;
  //TopTreeJetMet *cjetmet    = 0;
  //TopTreeJet *caloJet       = 0;
  //TopTreeTrack *asstrack    = 0;
  //TopTreePU *pu_info        = 0;

  // --------------------
  //    Fill Event info
  // --------------------
  if (verbose > 1)
    cout << "Filling event info" << endl;
  TopTree::MTEvent cev;
  cev.runNb = iEvent.id ().run ();
  cev.eventNb = iEvent.id ().event ();
  cev.lumiblock = iEvent.id ().luminosityBlock ();
  cev.eventWeight = 1;

  // --------------------
  //    Fill MC Truth
  // --------------------
  if (verbose > 1)
    cout << "Filling MC truth info" << endl;
  if ( isData !=  1 ) {




    //********************************************************
    // Calculate PDF weight
    //********************************************************
    edm::Handle<GenEventInfoProduct> theGenEventInfo;
    iEvent.getByType( theGenEventInfo );



    cev.partonFlavor1 = theGenEventInfo->pdf()->id.first;
    cev.partonFlavor2 = theGenEventInfo->pdf()->id.second;
    cev.x1 = theGenEventInfo->pdf()->x.first;
    cev.x2 = theGenEventInfo->pdf()->x.second;
    cev.Q_scale = theGenEventInfo->pdf()->scalePDF;


    if(readPDFWeights){
      edm::InputTag pdfWeightTag("pdfWeights:cteq66"); // or any other PDF set
      edm::Handle<std::vector<double> > weightHandle;
      iEvent.getByLabel(pdfWeightTag, weightHandle);

      std::vector<double> weights = (*weightHandle);
      std::cout << "Event weight for central PDF:" << weights[0] << std::endl;

       unsigned int nmembers = weights.size();
       for (unsigned int j=0; j<nmembers; j++) {
           //std::cout << "Event weight for PDF variation +" << (j+1)/2 << ": " << weights[j] << std::endl;
           //std::cout << "Event weight for PDF variation -" << (j+1)/2 << ": " << weights[j+1] << std::endl;
           cev.pdfWeights.push_back(float(weights[j]));

      }
      /*
      // stupid CWR !!
      edm::InputTag pdfWeightTag2("pdfWeights:MRST2006nnlo"); // or any other PDF set
      edm::Handle<std::vector<double> > weightHandle2;
      iEvent.getByLabel(pdfWeightTag2, weightHandle2);

      std::vector<double> weights2 = (*weightHandle2);
      //std::cout << "Event weight for central PDF:" << weights[0] << std::endl;

       unsigned int nmembers2 = weights2.size();
       for (unsigned int j=0; j<nmembers2; j++) {
           //std::cout << "Event weight for PDF variation +" << (j+1)/2 << ": " << weights[j] << std::endl;
           //std::cout << "Event weight for PDF variation -" << (j+1)/2 << ": " << weights[j+1] << std::endl;
           cev.pdfWeights.push_back(float(weights2[j]));

      }
      */
      // stupid CWR !!
      edm::InputTag pdfWeightTag3("pdfWeights:NNPDF10"); // or any other PDF set
      edm::Handle<std::vector<double> > weightHandle3;
      iEvent.getByLabel(pdfWeightTag3, weightHandle3);

      std::vector<double> weights3 = (*weightHandle3);
      std::cout << "Event weight for central PDF MRST:" << weights3[0] << std::endl;

       unsigned int nmembers3 = weights3.size();
       for (unsigned int j=0; j<nmembers3; j++) {
           //std::cout << "Event weight for PDF variation +" << (j+1)/2 << ": " << weights[j] << std::endl;
           //std::cout << "Event weight for PDF variation -" << (j+1)/2 << ": " << weights[j+1] << std::endl;
           cev.pdfWeights.push_back(float(weights3[j]));

      }



    }
std::cout << "Pileup"<< std::endl;

    //********************************************************
    // Pileup summary info
    //********************************************************

     edm::InputTag PileupSrc_("addPileupInfo");
     Handle<std::vector< PileupSummaryInfo > >  PupInfo;
     iEvent.getByLabel(PileupSrc_, PupInfo);

     std::vector<PileupSummaryInfo>::const_iterator PVI;

     //std::cout << "pu summary 189 " << PupInfo->size() << std::endl;
     // (then, for example, you can do)
     for(PVI = PupInfo->begin(); PVI != PupInfo->end(); ++PVI) {
       int n_bc=PVI->getBunchCrossing();
       //std::cout << " Pileup Information: bunchXing, nvtx, size : " << PVI->getBunchCrossing() << " "
       //<< PVI->getPU_NumInteractions()  << " "
       //<< PVI->getPU_zpositions().size() << std::endl;

       if (n_bc==0 )  cev.num_pileup_bc0    = PVI->getPU_NumInteractions();
       if (n_bc==1 )  cev.num_pileup_bcp1   = PVI->getPU_NumInteractions();
       if (n_bc==-1)  cev.num_pileup_bcm1   = PVI->getPU_NumInteractions();

       cev.runNb = PVI->getTrueNumInteractions();


    }

     //std::cout << "pu summary 207 " << std::endl;
    //********************************************************
    // Fill skimmed GenparticleCollection
    //********************************************************

    Handle < reco::GenParticleCollection > genParticles;
    iEvent.getByLabel ("genParticles", genParticles);


    if (!doGenParticleCollection)
      if (verbose > 1)
        cout << "Do not fill skimmed GenParticleCollection " << endl;
    if (doGenParticleCollection) {

      edm::Handle<GenEventInfoProduct> genInfos;
      iEvent.getByLabel("generator", genInfos);
      if(genInfos->binningValues().size() > 0 ) cev.ptHat = genInfos->binningValues()[0];

      cev.fillGenInfo(genParticles);
      if (verbose > 1)
        cout << "Filling skimmed GenParticleCollection" << endl;
      std::map < int, std::vector < int > >daughtermap;
      std::map < int, std::vector < int > >mothermap;
      size_t MaxLim = 100;
      bool maxlimfound = false;
      for (size_t i = 0; i < genParticles->size (); ++i) {
        const GenParticle & paIt = (*genParticles)[i];
        if (maxlimfound)
          continue;
        if (abs (paIt.pdgId ()) == 92 && MaxLim <= 100) {
          MaxLim = i;
          maxlimfound = true;
        }
      }
      for (size_t i = 0; i < MaxLim; ++i) {
        const GenParticle & paIt = (*genParticles)[i];
        // daughters
        std::vector < int >list_index_dau;
        list_index_dau.clear ();
        for (unsigned int j = 0; j < paIt.numberOfDaughters (); j++) {
          const reco::Candidate * d = paIt.daughter (j);
          if(d == NULL) continue;

          for (size_t k = i + 1; k < genParticles->size (); ++k) {
            const GenParticle & p = (*genParticles)[k];
            if (p.p4 ().e () == d->p4 ().e () && p.p4 ().pz () == d->p4 ().pz () && p.pdgId () == d->pdgId () && p.status () == d->status ())
              list_index_dau.push_back (k);
          }
          daughtermap[i] = list_index_dau;
          d = 0 ;
          delete d;
        }
        cev.GPC.push_back (paIt);
        // mothers
        std::vector < int >list_index_mot;
        list_index_mot.clear ();
        for (unsigned int j = 0; j < paIt.numberOfMothers (); j++) {
          const reco::Candidate * m = paIt.mother (j);
          if(m == NULL) continue;

          for (size_t k = 0; k < i; ++k) {
            const GenParticle & p = (*genParticles)[k];
            if (p.p4 ().e () == m->p4 ().e () && p.pdgId () == m->pdgId () && p.status () == m->status ())
              list_index_mot.push_back (k);
          }
          mothermap[i] = list_index_mot;
          m = 0 ;
          delete m;
        }

      }


      cev.GPC_list_index_dau = daughtermap;
      cev.GPC_list_index_mot = mothermap;
    }
  }

  //********************************************************
  //  trigger query
  //********************************************************
  TriggerResults tr;
  Handle < TriggerResults > h_trigRes;
  //iEvent.getByLabel(InputTag("TriggerResults::HLT"), h_trigRes);     // Trigger menu 1E31
  iEvent.getByLabel (TriggerMenu, h_trigRes);   //Trigger menu E29
  tr = *h_trigRes;

  if (TriggerInfo) {
    vector < string > tList;
    Service < service::TriggerNamesService > tns;
    bool foundNames = tns->getTrigPaths (tr, tList);
    if (!foundNames)
      std::cout << "Could not get trigger names!\n";
    if (tr.size () != tList.size ())
      std::cout << "ERROR: length of names and paths not the same: " << tList.size () << "," << tr.size () << endl;
    if (saveAllTriggers) {
        for (unsigned int i = 0; i < tr.size (); i++) {
        //std::cout <<"tList[i] "<<tList[i]<< std::endl;
           if (tr[i].accept () == 1)
                cev.triggers.push_back (pair < string, bool > (tList[i], true));
           else
                cev.triggers.push_back (pair < string, bool > (tList[i], false));
        }
    }
    else {
        for (unsigned int i = 0; i < tr.size (); i++) {
                for (unsigned int j = 0; j < triggerList.size (); j++) {
                        if (tList[i] == triggerList[j]) {
                                if (tr[i].accept () == 1)
                                        cev.triggers.push_back (pair < string, bool > (tList[i], true));
                                else
                                        cev.triggers.push_back (pair < string, bool > (tList[i], false));
                        }
                }
        }
   }
  }

  //********************************************************
  //  trigger matching with offline object
  //********************************************************

  // get Trigger summary from Event
  edm::Handle<trigger::TriggerEvent> summary;
  std::string T = "";

  T = TriggerMenu.process();


  edm::InputTag summaryTag_("hltTriggerSummaryAOD","",T);
  iEvent.getByLabel(summaryTag_,summary);

  std::string::size_type n(0);
  std::string::size_type index(0);

  //hltL1NonIsoHLTNonIsoSinglePhotonEt10HcalIsolFilter
  edm::InputTag memberTagElectron_("hltL1NonIsoHLTNonIsoSingleElectronEt10PixelMatchFilter","",T);

  index=summary->filterIndex(memberTagElectron_);
  // get all trigger objects corresponding to this module.
  // loop through them and see how many objects match the selection
  double match_pTHLTElectron15 = -1;
  if (index<summary->sizeFilters()) {
    const trigger::Keys& KEYS_el (summary->filterKeys(index));
    const std::string::size_type n1_el(KEYS_el.size());
    for (std::string::size_type i=0; i!=n1_el; ++i) {
      const trigger::TriggerObject& triggerObject_el( summary->getObjects().at(KEYS_el[i]) );
      //std::cout<<"je teste electron trigger object "<<std::endl;
      if (triggerObject_el.pt() >= 15 && iEvent.id ().run () < 138000 ) {
        n++;
        match_pTHLTElectron15 = triggerObject_el.pt();

      }
    }
  }

  cev.match_HLT_Ele10_LW_L1R_recoEl = match_pTHLTElectron15;

  //********************************************************
  // Retrieve dynamic prescales for triggers
  //********************************************************

  pat::TriggerEvent pTrigEvt;
  //if (isData) {

    Handle< pat::TriggerEvent > patTriggerEvent;
    iEvent.getByLabel("patTriggerEvent", patTriggerEvent );
    pTrigEvt = *patTriggerEvent;

    for (unsigned int j = 0; j < prescaleList.size (); j++) {
       if(pTrigEvt.path(prescaleList[j])!=0)
        cev.prescales.push_back(pair < string, int > (prescaleList[j], pTrigEvt.path(prescaleList[j])->prescale()));
       else
        cev.prescales.push_back(pair < string, int > (prescaleList[j], -99));
     }
  //}
  //default initialisation for MC
  //else cev.prescales.push_back(pair < string, int > ("empty",-99));

    bool filter_status = -99;
    const pat::TriggerFilter* filter_1 = pTrigEvt.filter("hltQuadJet40IsoPFTau40");
    if (filter_1) filter_status = filter_1->status();
    cev.prescales.push_back (pair < string, int > ("hltQuadJet40IsoPFTau40", filter_status));

    filter_status = -99;
    const pat::TriggerFilter* filter_2 = pTrigEvt.filter("HLTPFTauTightIsoSequence");
    if (filter_2) filter_status = filter_2->status();
    cev.prescales.push_back (pair < string, int > ("HLTPFTauTightIsoSequence", filter_status));

    filter_status = -99;
    const pat::TriggerFilter* filter_3 = pTrigEvt.filter("hltPFTau5Track");
    if (filter_3) filter_status = filter_3->status();
    cev.prescales.push_back (pair < string, int > ("hltPFTau5Track", filter_status));

    filter_status = -99;
    const pat::TriggerFilter* filter_4 = pTrigEvt.filter("hltPFTau5Track5");
    if (filter_4) filter_status = filter_4->status();
    cev.prescales.push_back (pair < string, int > ("hltPFTau5Track5", filter_status));

    filter_status = -99;
    const pat::TriggerFilter* filter_5 = pTrigEvt.filter("hltFilterPFTauTrack5TightIsoL1QuadJet20Central");
    if (filter_5) filter_status = filter_5->status();
    cev.prescales.push_back (pair < string, int > ("hltFilterPFTauTrack5TightIsoL1QuadJet20Central", filter_status));

    filter_status = -99;
    const pat::TriggerFilter* filter_6 = pTrigEvt.filter("hltFilterPFTauTrack5TightIsoL1QuadJet20CentralPFTau40");
    if (filter_6) filter_status = filter_6->status();
    cev.prescales.push_back (pair < string, int > ("hltFilterPFTauTrack5TightIsoL1QuadJet20CentralPFTau40", filter_status));

    filter_status = -99;
    const pat::TriggerFilter* filter_7 = pTrigEvt.filter("hltQuadJet45IsoPFTau45");
    if (filter_7) filter_status = filter_7->status();
    cev.prescales.push_back (pair < string, int > ("hltQuadJet45IsoPFTau45", filter_status));

    filter_status = -99;
    const pat::TriggerFilter* filter_8 = pTrigEvt.filter("hltFilterPFTauTrack5TightIsoL1QuadJet20CentralPFTau45");
    if (filter_8) filter_status = filter_8->status();
    cev.prescales.push_back (pair < string, int > ("hltFilterPFTauTrack5TightIsoL1QuadJet20CentralPFTau45", filter_status));

    filter_status = -99;
    const pat::TriggerFilter* filter_9 = pTrigEvt.filter("hltFilterPFTauTrack5TightIsoL1QuadJet28CentralPFTau45");
    if (filter_9) filter_status = filter_9->status();
    cev.prescales.push_back (pair < string, int > ("hltFilterPFTauTrack5TightIsoL1QuadJet28CentralPFTau45", filter_status));


  //********************************************************
  // PAT trigger helper for trigger matching information
  //********************************************************
  const pat::helper::TriggerMatchHelper matchHelper;

  const pat::TriggerObjectRefVector trigRefs( patTriggerEvent->objects( trigger::TriggerJet ) );
  for ( pat::TriggerObjectRefVector::const_iterator iTrig = trigRefs.begin(); iTrig != trigRefs.end(); ++iTrig )
  {
   //std::cout <<"HLT jet "<< (*iTrig)->pt() << " " << (*iTrig)->eta() <<std::endl;
   //std::cout << pTrigEvt.objectInFilter( (*iTrig), "hltQuadJet40IsoPFTau40") << std::endl;
   if (pTrigEvt.objectInFilter( (*iTrig), "hltQuadJet40IsoPFTau40") ||
       pTrigEvt.objectInFilter( (*iTrig), "hltQuadJet45IsoPFTau45"))
   {
    TLorentzVector p4((*iTrig)->px(),(*iTrig)->py(),(*iTrig)->pz(),(*iTrig)->energy());
    cev.jetObjTrig.push_back(p4);
    }
  }

  const pat::TriggerObjectRefVector trigRefs2( patTriggerEvent->objects( trigger::TriggerTau ) );
  for ( pat::TriggerObjectRefVector::const_iterator iTrig = trigRefs2.begin(); iTrig != trigRefs2.end(); ++iTrig )
  {
   //std::cout <<"HLT tau "<< (*iTrig)->pt() << " " << (*iTrig)->eta() <<std::endl;
   //std::cout << pTrigEvt.objectInFilter( (*iTrig), "hltFilterPFTauTrack5TightIsoL1QuadJet20CentralPFTau40") << std::endl;
    if (pTrigEvt.objectInFilter( (*iTrig), "hltFilterPFTauTrack5TightIsoL1QuadJet20CentralPFTau40") ||
        pTrigEvt.objectInFilter( (*iTrig), "hltFilterPFTauTrack5TightIsoL1QuadJet20CentralPFTau45") ||
        pTrigEvt.objectInFilter( (*iTrig), "hltFilterPFTauTrack5TightIsoL1QuadJet28CentralPFTau45")
       )
   {
    TLorentzVector p4((*iTrig)->px(),(*iTrig)->py(),(*iTrig)->pz(),(*iTrig)->energy());
    cev.tauObjTrig.push_back(p4);
    }
   }


  // -------------------------
  // get the beamspot from the Event:
  // -------------------------
  if (verbose > 1)
    cout << "Working on beamspot related variable" << endl;

  edm::Handle < reco::BeamSpot > recoBeamSpotHandle;
  iEvent.getByType (recoBeamSpotHandle);
  const BeamSpot bs = *recoBeamSpotHandle;
  cev.p3BS.SetXYZ (bs.x0 (), bs.y0 (), bs.z0 ());
  cev.p3BSErr.SetXYZ (bs.x0Error (), bs.y0Error (), bs.z0Error ());


// for compatibilty
  reco::BeamSpot beamSpot;
  Handle < reco::BeamSpot > beamSpotHa;
  iEvent.getByLabel (beamSpotProducer, beamSpotHa);
  Point bspot = beamSpotHa.isValid ()? beamSpotHa->position () : Point (0, 0, 0);
  // -------------------------

  Handle < vector < reco::Vertex > >vertices;
  iEvent.getByLabel (vertexProducer, vertices);
  int dum = 0;
  const reco::Vertex * vp1 = 0;

  for (vector < reco::Vertex >::const_iterator itv = vertices->begin (), edv = vertices->end (); itv != edv; ++itv) {
      if (dum == 0) vp1 = &*itv;
      TopTreePVx* pvx = cev.NewVertex ();
      pvx->chi2 = itv->chi2 ();
      pvx->isFake = itv->isFake ();
      pvx->isValid = itv->isValid ();
      pvx->ndof = itv->ndof ();
      pvx->normalizedChi2 = itv->normalizedChi2 ();
      pvx->NumberOfTracks = itv->tracksSize ();
      pvx->p3.SetXYZ (itv->x (), itv->y (), itv->z ());
      pvx->rho = itv->position ().rho ();
      pvx->p3Err.SetXYZ (itv->xError (), itv->yError (), itv->zError ());
      pvx = 0;
      delete pvx;
      dum++;
      if (dum == 1 && !saveAllVertex)
      break;
  }



  //***************************************************************************
  // Photons ----------------------------------------------------------------
  //***************************************************************************


  if (!doPhotons)
    if (verbose > 1)
      cout << "Do not fill the photons" << endl;
  if (doPhotons) {
    if (verbose > 1)
      cout << "Filling photons" << endl;

      Handle < vector < pat::Photon > >phHa;
      iEvent.getByLabel (photonProducer, phHa);
      const vector < pat::Photon > *gamma = phHa.product ();

      for (vector < pat::Photon >::const_iterator it = gamma->begin (); it != gamma->end (); it++) {

        const pat::Photon * thephoton = &*it;
        if (!thephoton)
          continue;             // useless ?
        // Applying preselection based on pT, eta, ID
        if (thephoton->p4().pt () < photon_cut_pt || fabs (thephoton->p4().eta ()) > photon_cut_eta)
        continue;

        TopTreePhoton *photon = cev.NewPhoton ();

        photon->p4.SetPxPyPzE (thephoton->p4().px (), thephoton->p4().py (), thephoton->p4().pz (), thephoton->p4().energy ());

        photon->ECaloIso = thephoton->ecalIso();
        photon->HCaloIso = thephoton->hcalIso();
        photon->TrkIso   = thephoton->trackIso();

        photon->isEB = thephoton->isEB();
        photon->e1x5 = thephoton->e1x5();
        photon->e2x5 = thephoton->e2x5();
        photon->e3x3 = thephoton->e3x3();
        photon->e5x5 = thephoton->e5x5();
        thephoton = 0;
        delete thephoton;
        photon = 0;
        delete photon;
      }
    gamma = 0;
    delete gamma;
  }







  // -------------------------
  //      Leptons (ele, mu, tau)
  // -------------------------




  //***************************************************************************
  // Electrons ----------------------------------------------------------------
  //***************************************************************************

  if (!doElectrons)
    if (verbose > 1)
      cout << "Do not fill the electrons" << endl;
  if (doElectrons) {
    if (verbose > 1)
      cout << "Filling electrons" << endl;

    Handle < vector < pat::Electron > >elHa;
    iEvent.getByLabel (electronProducer, elHa);
    const vector < pat::Electron > *ele = elHa.product ();

    //edm::Handle < EcalRecHitCollection > recHits;
    //iEvent.getByLabel (edm::InputTag ("ecalRecHit", "EcalRecHitsEB"), recHits);
    //const EcalRecHitCollection *myRecHits = recHits.product ();

    for (vector < pat::Electron >::const_iterator it = ele->begin (); it != ele->end (); it++) {

      const pat::Electron * elec = &*it;
      if (!elec)
        continue;               // useless ?
      // Applying preselection based on pT, eta, ID
      if (elec->pt () < electron_cut_pt || fabs (elec->eta ()) > electron_cut_eta)
        continue;

      TopTreeElectron *electron = cev.NewElectron ();

      electron->p4.SetPxPyPzE (elec->px (), elec->py (), elec->pz (), elec->energy ());
      electron->vertex.SetXYZ (elec->vx (), elec->vy (), elec->vz ());
      electron->Charge = elec->charge ();
      electron->addInfo.DB = elec->dB ();
      electron->nlost = elec->gsfTrack ()->trackerExpectedHitsInner ().numberOfLostHits ();
      //cout<<"nlost "<<elec->gsfTrack ()->trackerExpectedHitsInner ().numberOfLostHits ()<<endl;

      /////////////////////////////////
      //        Conversion
      ////////////////////////////////
     float evt_bField=0;
     // need the magnetic field
     //
     // if isData then derive bfield using the
     // magnet current from DcsStatus
     // otherwise take it from the IdealMagneticFieldRecord

     if (isData) {
     // scale factor = 3.801/18166.0 which are
     // average values taken over a stable two
     // week period

    edm::Handle<DcsStatusCollection> dcsHandle;
    iEvent.getByLabel("scalersRawToDigi", dcsHandle);

    if (dcsHandle.isValid())
     {
       if ((*dcsHandle).size()==0)
         {
           edm::LogError("DetStatus") << "Error! dcsStatus has size 0, bField set to magneticField->inTesla(GlobalPoint(0.,0.,0.)).z()" ;
            ESHandle<MagneticField> magneticField;
            iSetup.get<IdealMagneticFieldRecord>().get(magneticField);
            evt_bField = magneticField->inTesla(GlobalPoint(0.,0.,0.)).z();

         }
       else {
     float currentToBFieldScaleFactor = 2.09237036221512717e-04;
     float current = (*dcsHandle)[0].magnetCurrent();
     evt_bField = current*currentToBFieldScaleFactor;
        }
    }
   }
   else {
    ESHandle<MagneticField> magneticField;
    iSetup.get<IdealMagneticFieldRecord>().get(magneticField);

    evt_bField = magneticField->inTesla(GlobalPoint(0.,0.,0.)).z();
   }

          //Get the CTF tracks
      Handle<reco::TrackCollection> tracks_h;
      iEvent.getByLabel("generalTracks", tracks_h);

      ConversionFinder convFinder;
      //returns the best candidate partner (see text below)
      ConversionInfo convInfo = convFinder.getConversionInfo(*elec, tracks_h, evt_bField);
      electron->deltaDistance = convInfo.dist();
      electron->deltaCotTheta = convInfo.dcot();
      //cout<<"elec: "<<convInfo.dist()<<" "<<convInfo.dcot()<<endl;

    //  if(elec->gsfTrack()) electron->isGsfElectron = true;
       bool isGsfElectron = true;
       if (!elec->gsfTrack()) isGsfElectron = false;
        electron->isGsfElectron = isGsfElectron;
      // Swiss cross for Ecal spkie cleaning
      double myswissCross = -999;
      // Cut only on EB, ecal-seeded electrons
      //if (elec->ecalDrivenSeed () > 0 && fabs (elec->superCluster ()->eta ()) < 1.4442) {
        //const reco::CaloClusterPtr seed = elec->superCluster ()->seed ();     // seed cluster
        //const DetId seedId = seed->seed ();
        //EcalSeverityLevelAlgo severity;
        //myswissCross = severity.swissCross (seedId, *myRecHits);
      //}
      electron->elspike = myswissCross;
      // end swisscross

      //ECAL - HCAL Energy
      electron->EmEnergy = elec->ecalEnergy();
      electron->HadEnergy = elec->ecalEnergy()*elec->hcalOverEcal();

      //handle the electron-ID
      electron->elecIdLoose = elec->electronID ("eidLoose");
      electron->elecIdTight = elec->electronID ("eidTight");
      electron->elecIdRobustLoose = elec->electronID ("eidRobustLoose");
      electron->elecIdRobustTight = elec->electronID ("eidRobustTight");
      electron->elecIdWP90_r       = elec->electronID("simpleEleId90relIso");
      electron->elecIdWP90_c       = elec->electronID("simpleEleId90cIso");
      //new e-id
      electron->cicVeryLoose     =  elec->electronID("cicVeryLoose");
      electron->cicLoose         =  elec->electronID("cicLoose");
      electron->cicMedium        =  elec->electronID("cicMedium");
      electron->cicTight         =  elec->electronID("cicTight");
      electron->cicSuperTight    =  elec->electronID("cicSuperTight");
      electron->cicHyperTight1    =  elec->electronID("cicHyperTight1");
      electron->cicHyperTight2    =  elec->electronID("cicHyperTight2");
      electron->cicHyperTight3    =  elec->electronID("cicHyperTight3");
      electron->cicVeryLooseMC     =  elec->electronID("cicVeryLooseMC");
      electron->cicLooseMC         =  elec->electronID("cicLooseMC");
      electron->cicMediumMC        =  elec->electronID("cicMediumMC");
      electron->cicTightMC         =  elec->electronID("cicTightMC");
      electron->cicSuperTightMC    =  elec->electronID("cicSuperTightMC");
      electron->cicHyperTight1MC    =  elec->electronID("cicHyperTight1MC");
      electron->cicHyperTight2MC    =  elec->electronID("cicHyperTight2MC");
      electron->cicHyperTight3MC    =  elec->electronID("cicHyperTight3MC");






      electron->TrkZ0 = elec->TrackPositionAtVtx ().Z ();
      electron->TrkPt = sqrt (elec->trackMomentumAtVtx ().perp2 ());
      electron->TrkIso04 = elec->trackIso ();
      electron->CaloIso04 = elec->caloIso ();
      electron->ECaloIso04 = elec->ecalIso ();
      electron->HCaloIso04 = elec->hcalIso ();
      electron->TrkIso03 = elec->dr03TkSumPt ();

      electron->CaloIso03 = elec->dr03EcalRecHitSumEt () + elec->dr03HcalTowerSumEt ();

      electron->ECaloIso03 = elec->dr03EcalRecHitSumEt ();
      electron->HCaloIso03 = elec->dr03HcalTowerSumEt ();

      //PF isolation

      electron->PATNeutralHadronIso =  elec->neutralHadronIso ();
      electron->PATChargedHadronIso =  elec->chargedHadronIso () ;
      electron->PATPhotonIso        =  elec->photonIso () ;
      electron->PATTrackIso         =  elec->trackIso ();


      electron->caloEnergy = elec->caloEnergy ();
      //electron->eSuperClusterOverP = elec->eSuperClusterOverP ();
      //electron->deltaEtaSeedClusterTrackAtCalo = elec->deltaEtaSeedClusterTrackAtCalo ();
      //electron->deltaPhiSeedClusterTrackAtCalo = elec->deltaPhiSeedClusterTrackAtCalo ();
      //electron->deltaEtaSuperClusterTrackAtVtx = elec->deltaEtaSuperClusterTrackAtVtx ();
      //electron->deltaPhiSuperClusterTrackAtVtx = elec->deltaPhiSuperClusterTrackAtVtx ();
      //electron->hadronicOverEm = elec->hadronicOverEm ();
      //electron->hcalOverEcal = elec->hcalOverEcal ();
      //electron->sigmaIetaIeta = elec->sigmaIetaIeta ();
      //electron->e2x5Max = elec->e2x5Max ();
      //electron->e5x5 = elec->e5x5 ();
      electron->fbrem = elec->fbrem ();
      electron->classification = elec->classification ();
      electron->isEB = elec->isEB();
      electron->isEcalDriven = elec->ecalDrivenSeed ();
      electron->rawSCEnergy = elec->superCluster ()->rawEnergy ();
      double theta = 2*atan(exp(-1*elec->superCluster()->eta()));
      electron->ET_SC = elec->superCluster()->energy()*sin(theta);

      // Information relative to track
      electron->D0      = elec->gsfTrack ()->dxy (bspot);
      electron->Chi2    = elec->gsfTrack ()->normalizedChi2 ();

      edm::ESHandle < TransientTrackBuilder > trackBuilder;
      iSetup.get < TransientTrackRecord > ().get ("TransientTrackBuilder", trackBuilder);
      TransientTrack tt = trackBuilder->build (it->gsfTrack ());
      if (vp1) {
        double ip = IPTools::absoluteImpactParameter3D (tt, *vp1).second.value ();
        electron->IP3D = ip;
        double ipe = IPTools::absoluteImpactParameter3D (tt, *vp1).second.error ();
        electron->IP3DErr = ipe;
      }

      // MC information
      if ( isData != 1 ) {
        if ((elec->genLepton () != NULL)) {
          electron->addInfo.p4Gen.SetPxPyPzE (elec->genLepton ()->px (), elec->genLepton ()->py (), elec->genLepton ()->pz (), elec->genLepton ()->energy ());
        }

        Handle < reco::GenParticleCollection > genParticles;
        iEvent.getByLabel ("genParticles", genParticles);

        reco::GenParticle genmother;
        reco::GenParticle gengrandmother;
        reco::GenParticle genggrandmother;
        math::XYZTLorentzVector Zero (0, 0, 0, 0);

        genmother.setCharge (-99);
        genmother.setP4 (Zero);
        gengrandmother.setCharge (-99);
        gengrandmother.setP4 (Zero);
        genggrandmother.setCharge (-99);
        genggrandmother.setP4 (Zero);

        electron->LeptonOrigin = getElectronOrigin (genParticles, elec, genmother, gengrandmother, genggrandmother);
        electron->addInfo.GenMother = genmother;
        electron->addInfo.GenGrandMother = gengrandmother;
        electron->addInfo.GenGGrandMother = genggrandmother;
      }
      // REMARK: pour avoir acces a la trace il faut lors de la creation du patlayer1 mettre a true l'acces a la trace!
      electron->TrackEl = *(it->gsfTrack ());

      elec = 0;
      delete elec;
      electron = 0;
      delete electron;
    }                           // end electron loop
    ele = 0;
    delete ele;
  }

  //***************************************************************************
  // Muons --------------------------------------------------------------------
  //***************************************************************************


  if (!doMuons)
    if (verbose > 1)
      cout << "Do not fill the muons" << endl;
  if (doMuons) {
    if (verbose > 1)
      cout << "Filling muons" << endl;
    Handle < vector < pat::Muon > >muHa;
    iEvent.getByLabel (muonProducer, muHa);
    const vector < pat::Muon > *mus = muHa.product ();


    for (vector < pat::Muon >::const_iterator it = mus->begin (); it != mus->end (); it++) {
      const pat::Muon * mu = &*it;

      // Applying a preselectin on muons based on lep pT, eta, iso
      if (mu->pt () < muon_cut_pt || fabs (mu->eta ()) > muon_cut_eta || !(mu->isGlobalMuon () || mu->isTrackerMuon ()))
        continue;

      TopTreeMuon* muon = cev.NewMuon ();

      muon->p4.SetPxPyPzE (mu->px (), mu->py (), mu->pz (), mu->energy ());
      muon->vertex.SetXYZ (mu->vx (), mu->vy (), mu->vz ());
      muon->Charge = mu->charge ();
      muon->MuonType = mu->isGlobalMuon () * 100 + mu->isStandAloneMuon () * 10 + mu->isTrackerMuon ();
      muon->GlobalMuonPromptTight = mu->muonID ("GlobalMuonPromptTight");
      muon->addInfo.DB = mu->dB ();
      muon->TrkIso03 = mu->trackIso ();
      muon->CaloIso03 = mu->caloIso ();
      muon->ECaloIso03 = mu->ecalIso ();
      muon->HCaloIso03 = mu->hcalIso ();

      muon->PATNeutralHadronIso =  mu->neutralHadronIso ();
      muon->PATChargedHadronIso =  mu->chargedHadronIso () ;
      muon->PATPhotonIso        =  mu->photonIso () ;
      muon->PATTrackIso         =  mu->trackIso ();
      // REMARK: pour avoir acces a la trace il faut lors de la creation du patlayer1 mettre a true l'acces a la trace!
      // Information related to the tracks

      Point bspot = beamSpotHa.isValid ()? beamSpotHa->position () : Point (0, 0, 0);
      const reco::TrackRef globalTrack = mu->globalTrack ();
      if (globalTrack.isNonnull ()) {
        muon->D0 = globalTrack->dxy (bspot);
        muon->Chi2 = globalTrack->normalizedChi2 ();
      }
      if (it->innerTrack ().isNonnull ()) {
        muon->D0Inner = it->innerTrack ()->dxy (bspot);
      }

      muon->TrackMu = *(it->track ());
      if (mu->isStandAloneMuon ())
        muon->StaMu = *(it->standAloneMuon ());
      if (mu->isGlobalMuon ())
        muon->ComMu = *(it->combinedMuon ());

      muon->NTrValidHits = it->innerTrack()->numberOfValidHits();
      muon->NValidHits   = -999;
      muon->NOfMatches = it->numberOfMatches();
      if (mu->isGlobalMuon ())  muon->NValidHits = it->globalTrack()->hitPattern().numberOfValidMuonHits();
      edm::ESHandle < TransientTrackBuilder > trackBuilder;
      iSetup.get < TransientTrackRecord > ().get ("TransientTrackBuilder", trackBuilder);
      TransientTrack tt = trackBuilder->build (it->innerTrack ());
      if (vp1) {
        double ip = IPTools::absoluteImpactParameter3D (tt, *vp1).second.value ();
        muon->IP3D = ip;
        double ipe = IPTools::absoluteImpactParameter3D (tt, *vp1).second.error ();
        muon->IP3DErr = ipe;
      }

      //MC-info
      if (isData != 1 ) {
        if ((mu->genLepton () != NULL)) {
          muon->addInfo.p4Gen.SetPxPyPzE (mu->genLepton ()->px (), mu->genLepton ()->py (), mu->genLepton ()->pz (), mu->genLepton ()->energy ());
        }
        Handle < reco::GenParticleCollection > genParticles;
        iEvent.getByLabel ("genParticles", genParticles);
        reco::GenParticle genmother;
        reco::GenParticle gengrandmother;
        reco::GenParticle genggrandmother;
        math::XYZTLorentzVector Zero (0, 0, 0, 0);
        genmother.setCharge (-99);
        genmother.setP4 (Zero);
        gengrandmother.setCharge (-99);
        gengrandmother.setP4 (Zero);
        genggrandmother.setCharge (-99);
        genggrandmother.setP4 (Zero);
        muon->LeptonOrigin = getMuonOrigin (genParticles, mu, genmother, gengrandmother, genggrandmother);
        muon->addInfo.GenMother = genmother;
        muon->addInfo.GenGrandMother = gengrandmother;
        muon->addInfo.GenGGrandMother = genggrandmother;
      }
      mu = 0;
      delete mu;
      muon = 0;
      delete muon;
    }                           // end muon loop
    mus = 0;
    delete mus;
  }

  //***************************************************************************
  // Taus ----------------------------------------------------------------
  //***************************************************************************
if (!doTaus)
    if (verbose > 1)
      cout << "Do not fill the taus" << endl;
  if (doTaus) {
    if (verbose > 1)
      cout << "Filling taus" << endl;


   for (unsigned int m = 0 ; m<tauProducer.size(); ++m )
   {

    Handle < vector < pat::Tau > >tauHa;
    iEvent.getByLabel (tauProducer.at(m), tauHa);
    const vector < pat::Tau > *taus = tauHa.product ();

    TopTree::MTCollection<TopTreeTau> *ctau = cev.NewTauColl();

    ctau->algo = tauProducer.at(m);
    int iTau = 0 ;
    for (vector < pat::Tau >::const_iterator it = taus->begin (); it != taus->end (); it++) {

      const pat::Tau * patTau = &*it;
      if (!patTau)
        continue;               // useless ?
      // Applying preselection based  on pT, eta, ID
      if (patTau->pt () < tau_cut_pt || fabs (patTau->eta ()) > tau_cut_eta)
        continue;

      TopTreeTau *tau = new TopTreeTau ();
      tau->p4.SetPxPyPzE (patTau->px (), patTau->py (), patTau->pz (), patTau->energy ());
      tau->vertex.SetXYZ (patTau->vx (), patTau->vy (), patTau->vz ());
      tau->Charge = patTau->charge ();
      //tau->Dxy_BS = patTau->dB();

      /*const pat::TriggerObjectRef trigRef( matchHelper.triggerMatchObject( tauHa, iTau, hltTausMatcher, iEvent, pTrigEvt ) );
      if ( trigRef.isAvailable() ) {
         tau->p4HLT.SetPxPyPzE (trigRef->px (), trigRef->py (), trigRef->pz (), trigRef->energy ());
      } */


      const pat::TriggerObjectRef trigRef45( matchHelper.triggerMatchObject( tauHa, iTau, hltTausMatcher45, iEvent, pTrigEvt ) );
      if ( trigRef45.isAvailable() ) {
         tau->p4HLT.SetPxPyPzE (trigRef45->px (), trigRef45->py (), trigRef45->pz (), trigRef45->energy ());
      }

      if( patTau->leadPFChargedHadrCand().isAvailable()) {
        if (patTau->leadPFChargedHadrCand()->trackRef().isNonnull())
        {
         tau->leadTrackPt = patTau->leadPFChargedHadrCand()->trackRef()->pt();
         tau->D0 = patTau->leadPFChargedHadrCand()->trackRef()->dxy(bspot);}
        else if  (patTau->leadPFChargedHadrCand()->gsfTrackRef().isNonnull())
        {
         tau->leadTrackPt = patTau->leadPFChargedHadrCand()->gsfTrackRef()->pt();
         tau->D0 = patTau->leadPFChargedHadrCand()->gsfTrackRef()->dxy(bspot);}
        else
        {
         tau->leadTrackPt = -999.;
         tau->D0 = -999.;}
      }
      else {
        tau->leadTrackPt = -999.;
        tau->D0 = -999.;
      }

      tau->numSigConeTracks = patTau->signalTracks ().size ();
      tau->numIsoConeTracks = patTau->isolationTracks ().size ();

      tau->isolationPFChargedHadrCandsPtSum = patTau->isolationPFChargedHadrCandsPtSum ();
      tau->isolationPFGammaCandsEtSum = patTau->isolationPFGammaCandsEtSum ();
      tau->maximumHCALPFClusterEt = patTau->maximumHCALPFClusterEt ();
      tau->emFraction = patTau->emFraction ();
      tau->hcalTotOverPLead = patTau->hcalTotOverPLead ();
      tau->hcalMaxOverPLead = patTau->hcalMaxOverPLead ();
      tau->hcal3x3OverPLead = patTau->hcal3x3OverPLead ();
      tau->ecalStripSumEOverPLead = patTau->ecalStripSumEOverPLead ();

      tau->decayMode = patTau->decayMode ();
      tau->tauIDs= patTau->tauIDs();

      //MC-info
      if (isData != 1 ) {
      if ((patTau->genLepton () != NULL)) {
        tau->addInfo.p4Gen.SetPxPyPzE (patTau->genLepton ()->px (), patTau->genLepton ()->py (), patTau->genLepton ()->pz (), patTau->genLepton ()->energy ());
      }

      /*
      std::vector<reco::GenParticleRef> associatedGenParticles = patTau->genParticleRefs();
      for ( std::vector<reco::GenParticleRef>::const_iterator it = associatedGenParticles.begin();
       it != associatedGenParticles.end(); ++it ) {
       if ( it->isAvailable() ) {
       const reco::GenParticleRef& genParticle = (*it);
       std::cout << "genParticle->pdgId() "<< genParticle->pdgId() <<" "<<genParticle->pt()<<" "<<genParticle->eta()<<std::endl;
         }
      }*/

      /*const reco::GenJet * gjet = patTau->genJet();
      if (gjet)
      { // valid only if hadronic decay mode
       //std::vector <const GenParticle*> vgjet = gjet->getGenConstituents ();
       //for (unsigned int i=0; i<vgjet.size(); i++)
       // std::cout <<"gen jet pdgid " << vgjet[i]->pdgId() <<std::endl;
       }
      */

      //
      tau->LeptonOrigin = 0;
      if (isData != 1 ) {
        reco::GenParticleRef tauref = patTau->genParticleById(15,0,false);
        if (tauref.isNonnull()) { tau->LeptonOrigin = 15;   }
        reco::GenParticleRef tauref2 = patTau->genParticleById(-15,0,false);
        if (tauref2.isNonnull()){ tau->LeptonOrigin = -15;  }

        reco::GenParticleRef elref = patTau->genParticleById(11,0,false);
        if (elref.isNonnull()  && elref->pt()>10.)  { tau->LeptonOrigin = 11;  }
        reco::GenParticleRef elref2 = patTau->genParticleById(-11,0,false);
        if (elref2.isNonnull() && elref2->pt()>10.) { tau->LeptonOrigin = -11; }
        reco::GenParticleRef muref = patTau->genParticleById(13,0,false);
        if (muref.isNonnull()  && muref->pt()>10.)  { tau->LeptonOrigin = 13;  }
        reco::GenParticleRef muref2 = patTau->genParticleById(-13,0,false);
        if (muref2.isNonnull() && muref2->pt()>10.) { tau->LeptonOrigin = -13; }
      }

      Handle < reco::GenParticleCollection > genParticles;
      iEvent.getByLabel ("genParticles", genParticles);

      reco::GenParticle genmother;
      reco::GenParticle gengrandmother;
      reco::GenParticle genggrandmother;
      math::XYZTLorentzVector Zero (0, 0, 0, 0);

      genmother.setCharge (-99);
      genmother.setP4 (Zero);
      gengrandmother.setCharge (-99);
      gengrandmother.setP4 (Zero);
      genggrandmother.setCharge (-99);
      genggrandmother.setP4 (Zero);
      //tau->LeptonOrigin = -1;
      tau->decay = getTauDecay(genParticles,patTau);
      tau->addInfo.GenMother = genmother;
      tau->addInfo.GenGrandMother = gengrandmother;
      tau->addInfo.GenGGrandMother = genggrandmother;

      }

      ctau->objects.push_back(*tau);
      patTau = 0;
      iTau++;
      delete patTau;
      tau = 0;
      delete tau;
    }                           // end tau loop
    taus = 0;
    delete taus;
   }
  }


  //****************************************************************************
  // Tracks --------------------------------------------------------------------
  //****************************************************************************


  if (!doTracks)
    if (verbose > 1)
      cout << "Do not fill the tracks" << endl;
  if (doTracks) {
    if (verbose > 1)
      cout << "Filling tracks" << endl;
    Handle < std::vector < reco::Track > >trackHa;
    iEvent.getByLabel (trackProducer, trackHa);
    const std::vector < reco::Track > *tracks = trackHa.product ();

    for (std::vector < reco::Track >::const_iterator it = tracks->begin (); it != tracks->end (); it++) {
      const reco::Track * track = &*it;

      // Applying a preselection based on on pT, eta
      if (track->pt () < track_cut_pt || fabs (track->eta ()) > track_cut_eta)
        continue;

      TopTreeTrack    *tttrack = cev.NewTrack ();
      tttrack->p4.SetPxPyPzE (track->px (), track->py (), track->pz (), track->p ());
      tttrack->Chi2 = track->chi2 () / track->ndof ();
      tttrack->nHitTracker = track->hitPattern ().numberOfValidHits ();
      tttrack->d0 = track->d0 ();
      tttrack->d0Error = track->d0Error ();
      tttrack->dz = track->dz ();
      tttrack->dzError = track->dzError ();
      tttrack->dxy = track->dxy ();
      tttrack->dxyError = track->dxyError ();
      tttrack->charge = track->charge ();
      if (beamSpotHa.isValid ()) {
        beamSpot = *beamSpotHa;
        tttrack->dxy_BS = track->dxy (beamSpot.position ());
      }
      track = 0;
      delete track;
      tttrack = 0;
      delete tttrack;
    }                           // end tracks loop
    tracks = 0;
    delete tracks;
  }



  //****************************************************************************
  // JetMET --------------------------------------------------------------------
  //****************************************************************************


  //prepare residual corrections
  //string JEC_PATH("CondFormats/JetMETObjects/data/");
  /*
  edm::FileInPath fipRes_PF(JEC_PATH+"Spring10DataV2_L2L3Residual_AK5PF.txt");
  JetCorrectorParameters *ResJetCorPar_PF = new JetCorrectorParameters(fipRes_PF.fullPath());
  std::vector<JetCorrectorParameters> vparam_PF;
  vparam_PF.push_back(*ResJetCorPar_PF);
  FactorizedJetCorrector *resJEC_PF = new FactorizedJetCorrector(vparam_PF);


  edm::FileInPath fipRes_JPT(JEC_PATH+"Spring10DataV2_L2L3Residual_AK5JPT.txt");
  JetCorrectorParameters *ResJetCorPar_JPT = new JetCorrectorParameters(fipRes_JPT.fullPath());
  std::vector<JetCorrectorParameters> vparam_JPT;
  vparam_JPT.push_back(*ResJetCorPar_JPT);
  FactorizedJetCorrector *resJEC_JPT = new FactorizedJetCorrector(vparam_JPT);


  edm::FileInPath fipRes_Calo(JEC_PATH+"Spring10DataV2_L2L3Residual_AK5Calo.txt");
  JetCorrectorParameters *ResJetCorPar_Calo = new JetCorrectorParameters(fipRes_Calo.fullPath());
  std::vector<JetCorrectorParameters> vparam_Calo;
  vparam_Calo.push_back(*ResJetCorPar_Calo);
  FactorizedJetCorrector *resJEC_Calo = new FactorizedJetCorrector(vparam_Calo);

  */

  //get Rho (PU/UE densities)
  edm::Handle<double> h_rho;
  iEvent.getByLabel( rho_PUUE_dens, h_rho);
  //iEvent.getByLabel( edm::InputTag("kt6PFJets", "rho"), h_rho);
  //                    edm::InputTag("kt6PFJets", "rho")

  cev.rho_PUUE_dens = (*h_rho);


  //get Rho (PU/UE densities)
  edm::Handle<double> h_neutralRho;
  iEvent.getByLabel( neutralRho_PUUE_dens, h_neutralRho);

  cev.neutralRho_PUUE_dens = (*h_neutralRho);


  if (verbose > 1)
    cout << "Filling JETMET" << endl;
  // Compute correction needed for tcMET

  Handle < edm::ValueMap < reco::MuonMETCorrectionData > >tcMet_ValueMap_Handle;
  iEvent.getByLabel ("muonTCMETValueMapProducer", "muCorrData", tcMet_ValueMap_Handle);

  Handle < vector < reco::Muon > >muon_h;
  iEvent.getByLabel ("muons", muon_h);

  float SumMuMetCorrX = 0.;
  float SumMuMetCorrY = 0.;

  const unsigned int nMuons = muon_h->size ();
  for (unsigned int mus = 0; mus < nMuons; mus++) {
    reco::MuonRef muref (muon_h, mus);
    reco::MuonMETCorrectionData muCorrData = (*tcMet_ValueMap_Handle)[muref];
    if (muCorrData.type () == 0 || muCorrData.type () == 4) {
      SumMuMetCorrX += muCorrData.corrX () - muref->px ();
      SumMuMetCorrY += muCorrData.corrY () - muref->py ();
    }
  }


  VParameters::iterator ijetmet = jetmetProducer.begin ();
  VParameters::iterator ijetmetEnd = jetmetProducer.end ();

  for (; ijetmet != ijetmetEnd; ++ijetmet) {
    std::string jet_ = ijetmet->getUntrackedParameter < std::string > ("jet");
    std::string met_ = ijetmet->getUntrackedParameter < std::string > ("met");
    std::string algo_ = ijetmet->getUntrackedParameter < std::string > ("algo");

    TopTreeJetMet *cjetmet = cev.NewJetMet ();
    cjetmet->algo = algo_;

    edm::Handle < edm::View < pat::MET > >metHandle;
    iEvent.getByLabel (met_, metHandle);
    edm::View < pat::MET > mets = *metHandle;
    if (metHandle->size () != 0) {
      const pat::MET met = mets.front ();


      cjetmet->met.p4.SetPxPyPzE (met.px (), met.py (), met.pz (), met.energy());       // This is global (JES+muon) corrected MET
      cjetmet->met.misEt = met.pt ();
      cjetmet->met.uncmisEt = met.uncorrectedPt ();
      cjetmet->met.sumEt = met.sumEt ();
      // NB: met - globaluncorrection = uncorrectedmet : met.px() - met.corEx(uc0) = uncmisEt.px(), idem for py

      pat::MET::UncorrectionType uc0;   // { uncorrALL, uncorrJES, uncorrMUON, uncorrMAXN }
      uc0 = pat::MET::UncorrectionType (0);
      pat::MET::UncorrectionType uc1;
      uc1 = pat::MET::UncorrectionType (1);
      pat::MET::UncorrectionType uc2;
      uc2 = pat::MET::UncorrectionType (2);

      double cor_0_x = met.corEx (uc0);
      double cor_0_y = met.corEy (uc0);

      // MET global uncorrections (to be added if needed):
      cjetmet->met.dmEx = cor_0_x;
      cjetmet->met.dmEy = cor_0_y;

      if (algo_ == "tc") {
        cjetmet->met.doCorrection (SumMuMetCorrX, SumMuMetCorrY);
        cjetmet->met.p4MuonCorr.SetPxPyPzE (met.px () + SumMuMetCorrX, met.py () + SumMuMetCorrY, 0.0, 0.0);
      }

    }


    //****************************************************************************
    // Jets ----------------------------------------------------------------------
    //****************************************************************************


    if (verbose > 1)
      cout << "Filling jets" << endl;
    Handle < vector < pat::Jet > >jetHa;
    iEvent.getByLabel (jet_, jetHa);
    const vector < pat::Jet > *jets = jetHa.product ();


    JetIDSelectionFunctor jetIDLoose (JetIDSelectionFunctor::PURE09, JetIDSelectionFunctor::LOOSE);
    PFJetIDSelectionFunctor PfjetIDLoose (PFJetIDSelectionFunctor::FIRSTDATA, PFJetIDSelectionFunctor::LOOSE);
    JetIDSelectionFunctor jetIDTight (JetIDSelectionFunctor::PURE09, JetIDSelectionFunctor::TIGHT);
    PFJetIDSelectionFunctor PfjetIDTight (PFJetIDSelectionFunctor::FIRSTDATA, PFJetIDSelectionFunctor::TIGHT);

    int iJet = 0;
    for (vector < pat::Jet >::const_iterator it = jets->begin (); it != jets->end (); it++) {

      double scale = -99;
      double jetPT = it->pt ();
//modifdg
      //if (it->corrStep () == "abs" || it->corrStep () == "ABS") {
        scale = it->jecFactor("Uncorrected");      // scale factor from current jet with correction (abs=L1+L2+L3) to raw jet
         //}

      //Applying a preselection based on pT, eta
      if (jetPT < jet_cut_pt || abs (it->eta ()) > jet_cut_eta)
        continue;

      //create Jet --------------
      TopTreeJet *caloJet = new TopTreeJet ();

      //fill jet infos --------------------
      caloJet->p4.SetPxPyPzE (it->px (), it->py (), it->pz (), it->energy ());
       if(!isData && it->genJet() != 0) caloJet->p4Gen.SetPxPyPzE (it->genJet()->px (), it->genJet()->py (), it->genJet()->pz (), it->genJet()->energy ());
       if(!isData && it->genParton() != 0 ) caloJet->p4Parton.SetPxPyPzE (it->genParton()->px (), it->genParton()->py (), it->genParton()->pz (), it->genParton()->energy ());
        caloJet->scale = scale;
      reco::JetID jid = it->jetID ();
      caloJet->n90Hits = jid.n90Hits;
      caloJet->fHPD = jid.fHPD;

      // add uncorr jet

      caloJet->uncorrJet = it->correctedP4(0);
      caloJet->jetArea = it->jetArea();
      caloJet->corrJES = (it->correctedJet("Uncorrected")).pt()/it->pt();




      //caloJet->resCorr = -1;

      pat::strbitset ret = jetIDLoose.getBitTemplate ();
      pat::strbitset retpf = PfjetIDLoose.getBitTemplate ();
      ret.set (false);
      retpf.set (false);
      pat::strbitset rett = jetIDTight.getBitTemplate ();
      pat::strbitset rettpf = PfjetIDTight.getBitTemplate ();
      rett.set (false);
      rettpf.set (false);

      // Tc jet ID not supported
      if (it->isCaloJet () || it->isJPTJet ()) {
        caloJet->isLOOSE = jetIDLoose (*it, ret);
        caloJet->isTIGHT = jetIDTight (*it, rett);
        //get residual corr
        if (it->isCaloJet ()){
          //resJEC_Calo->setJetEta(it->eta());
          //resJEC_Calo->setJetPt(it->pt());
          //caloJet->resCorr = resJEC_Calo->getCorrection();
        }else{
          //resJEC_JPT->setJetEta(it->eta());
          //resJEC_JPT->setJetPt(it->pt());
          //caloJet->resCorr = resJEC_JPT->getCorrection();
        }

      }
      if (it->isPFJet ()) {
        caloJet->isLOOSE = PfjetIDLoose (*it, retpf);
        caloJet->isTIGHT = PfjetIDTight (*it, rettpf);
        //get residual corr
        //resJEC_PF->setJetEta(it->eta());
        ///resJEC_PF->setJetPt(it->pt());
        //caloJet->resCorr = resJEC_PF->getCorrection();
      }

      if (it->isCaloJet () || it->isJPTJet () ) {
        caloJet->emEnergyFraction = it->emEnergyFraction ();
        caloJet->hadEnergyFraction = it->energyFractionHadronic ();
      }
      else if (it->isPFJet ()) {
        caloJet->emEnergyFraction = -99;
        caloJet->hadEnergyFraction = -99;
        caloJet->CHEF = it->chargedHadronEnergyFraction ();
        caloJet->CEEF = it->chargedEmEnergyFraction ();
        caloJet->NHEF = it->neutralHadronEnergyFraction ();
        caloJet->NEEF = it->neutralEmEnergyFraction ();
      }
      caloJet->nTracks = it->associatedTracks().size();
      float sumPtTracks = 0;
      for(unsigned int tr=0;tr<it->associatedTracks().size();tr++) sumPtTracks+=it->associatedTracks()[tr]->pt();
      caloJet->sumPtTracks = sumPtTracks;

      //modifdg
      if (isData != 1 ) {

        caloJet->partonFlavour = it->partonFlavour ();

      }
      //Here
      const reco::TrackRefVector & TrkJet = it->associatedTracks ();
      for (unsigned int itk = 0; itk < TrkJet.size (); itk++) {
        const reco::Track tk = *TrkJet[itk];
      }
      if(!isAOD){

        const reco::TrackIPTagInfo * TIP = it->tagInfoTrackIP ();
        const reco::TrackRefVector & T = TIP->selectedTracks ();

        std::vector < reco::TrackIPTagInfo::TrackIPData > ip = TIP->impactParameterData ();

        // retrieve the "secondary vertex tag infos"
        const reco::SecondaryVertexTagInfo * SVT = it->tagInfoSecondaryVertex ();
        const reco::TrackRefVector & S = SVT->vertexTracks();

        vector < float >prob2d, prob3d;

        prob2d.clear ();
        prob3d.clear ();

        if (TIP->hasProbabilities ()) {
          prob2d = TIP->probabilities (0);
          prob3d = TIP->probabilities (1);
        }
        for (unsigned int itk = 0; itk < T.size (); itk++) {

          const reco::Track tk = *T[itk];
          TopTreeTrack *asstrack = caloJet->NewTrack ();
          asstrack->p4.SetPxPyPzE (tk.px (), tk.py (), tk.pz (), tk.p ());
          asstrack->Chi2 = tk.chi2 () / tk.ndof ();
          asstrack->nHitTracker = tk.hitPattern ().numberOfValidHits ();
          asstrack->nHitPixel = tk.hitPattern ().numberOfValidPixelHits ();
          asstrack->withFirstPixel = tk.hitPattern ().hasValidHitInFirstPixelBarrel ();
          asstrack->nHitPXB = tk.hitPattern ().numberOfValidPixelBarrelHits ();
          asstrack->nHitPXF = tk.hitPattern ().numberOfValidPixelEndcapHits ();
          asstrack->nHitTIB = tk.hitPattern ().numberOfValidStripTIBHits ();
          asstrack->nHitTOB = tk.hitPattern ().numberOfValidStripTOBHits ();
          asstrack->nHitTID = tk.hitPattern ().numberOfValidStripTIDHits ();
          asstrack->nHitTEC = tk.hitPattern ().numberOfValidStripTECHits ();
          asstrack->charge = tk.charge ();
          asstrack->IP3Dsig = ip[itk].ip3d.significance ();
          asstrack->IP3D = ip[itk].ip3d.value ();
          asstrack->IP3Derr = ip[itk].ip3d.error ();
          asstrack->IP2Dsig = ip[itk].ip2d.significance ();
          asstrack->IP2D = ip[itk].ip2d.value ();
          asstrack->IP2Derr = ip[itk].ip2d.error ();
          if (itk < prob3d.size ())
            asstrack->Prob3D = prob3d[itk];
          if (itk < prob2d.size ())
            asstrack->Prob2D = prob2d[itk];
            GlobalPoint pv (TIP->primaryVertex ()->position ().x (), TIP->primaryVertex ()->position ().y (), TIP->primaryVertex ()->position ().z ());
            asstrack->DecayLength = (ip[itk].closestToJetAxis - pv).mag ();
            asstrack = 0;
            delete asstrack;

        }

        std::vector < std::size_t > sortedIndices = TIP->sortedIndexes (reco::TrackIPTagInfo::IP3DSig);

        GlobalPoint pv (TIP->primaryVertex ()->position ().x (), TIP->primaryVertex ()->position ().y (), TIP->primaryVertex ()->position ().z ());
        //const reco::TrackRefVector & TS = TIP-> sortedTracks (sortedIndices);

        caloJet->nTracksSVX = S.size ();
        caloJet->nSVX = SVT->nVertices ();
        if (SVT->nVertices () >= 1) {
          // pick the first secondary vertex (the "best" one)
          const reco::Vertex & sv = SVT->secondaryVertex (0);
          caloJet->chi2SVX = sv.chi2 ();
          caloJet->ndofSVX = sv.ndof ();

          caloJet->flightDistanceSVX = SVT->flightDistance (0, false).value ();
          caloJet->flightDistanceSigSVX = SVT->flightDistance (0, false).significance ();
          caloJet->flightDistanceErrSVX = SVT->flightDistance (0, false).error ();
          // compute the invariant mass from a four-vector sum
          TLorentzVector trackFourVectorSum;

          // loop over all tracks in the vertex
          for (reco::Vertex::trackRef_iterator track = sv.tracks_begin (); track != sv.tracks_end (); ++track) {
            TLorentzVector vec;
            vec.SetXYZM ((*track)->px (), (*track)->py (), (*track)->pz (), 0.13957);   // pion mass
            trackFourVectorSum += vec;
          }
          caloJet->MassSVX = trackFourVectorSum.M ();

        }

        TIP = 0;
        delete TIP;
        SVT = 0;
        delete SVT;
      }


      caloJet->DiscriPair = it->getPairDiscri ();
      caloJet->vertex.SetXYZ (it->vx (), it->vy (), it->vz ());
      caloJet->TCDiscri = it->bDiscriminator ("trackCountingHighEffBJetTags");
      // ATTENTION : Pour certain processing des data reelles il faut utiliser l'une ou lautre de ces 2 lignes
      double SV1 = it->bDiscriminator ("simpleSecondaryVertexBJetTags");
      double SV2 = it->bDiscriminator ("simpleSecondaryVertexHighEffBJetTags");
      if (SV2 == -1000) SV2 = SV1;
      caloJet->SVDiscri = SV2;

      caloJet->SMDiscri = it->bDiscriminator ("softMuonBJetTags");
      caloJet->setTracks (it->associatedTracks ());
      /*
      const pat::TriggerObjectRef trigRef( matchHelper.triggerMatchObject( jetHa, iJet, hltJetsMatcher, iEvent, pTrigEvt ) );
      if ( trigRef.isAvailable() ) {
         caloJet->p4HLT.SetPxPyPzE (trigRef->px (), trigRef->py (), trigRef->pz (), trigRef->energy ());}
      */

      const pat::TriggerObjectRef trigRef45( matchHelper.triggerMatchObject( jetHa, iJet, hltJetsMatcher45, iEvent, pTrigEvt ) );
      if ( trigRef45.isAvailable() ) {
         caloJet->p4HLT.SetPxPyPzE (trigRef45->px (), trigRef45->py (), trigRef45->pz (), trigRef45->energy ());
      }

      cjetmet->objects.push_back (*caloJet);

      cjetmet->sumEtJet += it->et ();

      cjetmet->sumEtJetRaw += it->et () / scale;

      iJet++;

      //TIP = 0;
      //delete TIP;
      //SVT = 0;
      //delete SVT;
      delete caloJet;
    }                           // end jet loop
    cjetmet = 0;// other delete object belonging to cevt
    delete cjetmet;
    jets = 0;// handle on file
    delete jets;
  }



  //ResJetCorPar_PF = 0 ;
  //delete ResJetCorPar_PF ;
  //resJEC_PF = 0  ;
  //delete resJEC_PF ;


 // ResJetCorPar_JPT = 0  ;
 // delete ResJetCorPar_JPT ;
 // resJEC_JPT = 0  ;
 // delete resJEC_JPT ;


 // ResJetCorPar_Calo = 0  ;
  //delete ResJetCorPar_Calo ;
  //resJEC_Calo  = 0 ;
 // delete resJEC_Calo ;





  // is put into the Event
  std::auto_ptr < TopTree::MTEvent > pOut (new TopTree::MTEvent (cev));
  iEvent.put (pOut);
  if (verbose > 0)
    cout << "Event registered in the MiniTree" << endl;

  // --------------------
  //    Deleting some pointers
  // --------------------
  vp1 = 0;
  delete vp1;

}

// ------------ method called once each job just before starting event loop  ------------
void MiniTreeProducer::beginJob ()
{
  cout << "BeginJob for MiniTreeProducer " << endl;
}

// ------------ method called once each job just after ending the event loop  ------------
void MiniTreeProducer::endJob ()
{
  cout << "EndJob for MiniTreeProducer" << endl;
}


bool MiniTreeProducer::MyJetCleaner (const vector < pat::Electron > *electrons, pat::Jet J)
{
  bool passed = true;
  //     for(vector<pat::Electron>::const_iterator it = electrons->begin(); it!=electrons->end(); it++){
  //       const pat::Electron* elec = &*it;
  //       std::cout<<"test MyJetCleaner0 "<<elec->pt()<< " "<<elec->trackIso()<< " "<<elec->caloIso()<<std::endl;
  //     }
  return passed;
}


int
  MiniTreeProducer::getMuonOrigin (edm::Handle < reco::GenParticleCollection > genParticles, const pat::Muon * thePatMuon,
                                   reco::GenParticle & MotherGenMuon, reco::GenParticle & GrandMotherGenMuon, reco::GenParticle & GGrandMotherGenMuon)
{

  int muonOrigin = -99;
  const reco::Candidate * theGenMuon;
  reco::GenParticle test;
  bool matchedGenLepton = false;

  for (reco::GenParticleCollection::const_iterator p = genParticles->begin (); p != genParticles->end (); ++p) {
    const reco::Candidate * aGenMuon = (dynamic_cast < reco::Candidate * >(const_cast < reco::GenParticle * >(&*p)));

    if (abs (p->pdgId ()) == 13 && p->status () == 1) {
      if ((thePatMuon->genLepton () != NULL) && abs (thePatMuon->genLepton ()->pt () - aGenMuon->pt ()) < 0.00001) {
        theGenMuon = aGenMuon;
        matchedGenLepton = true;
      }
    }
  }

  if (matchedGenLepton) {
    muonOrigin = 0;
    bool isFromBDecay = false;
    bool isFromCDecay = false;
    if (theGenMuon->mother () != 0 && abs (theGenMuon->pdgId ()) == 13) {
      const reco::Candidate * aMotherGenMuon1 = theGenMuon->mother ();
      const reco::Candidate * aMotherGenMuon2 = theGenMuon->mother ();
      setGenParticleFromCandidate (MotherGenMuon, theGenMuon->mother ());
      //                        MotherGenMuon = *theGenMuon->mother();
      int gm = 0;
      while (aMotherGenMuon2->mother () != 0) {
        gm++;
        aMotherGenMuon2 = aMotherGenMuon2->mother ();
        if (abs (aMotherGenMuon2->pdgId ()) == 24 && abs (aMotherGenMuon1->pdgId ()) == 13)
          muonOrigin = muonOrigin + 1;  // muon from W
        if (abs (aMotherGenMuon2->pdgId ()) == 24 && abs (aMotherGenMuon1->pdgId ()) == 15)
          muonOrigin = muonOrigin + 1;  // muon from W->tau
        if (abs (aMotherGenMuon2->pdgId ()) == 23 && abs (aMotherGenMuon1->pdgId ()) == 13)
          muonOrigin = muonOrigin + 10;
        if (abs (aMotherGenMuon1->pdgId ()) == 4 || (abs (aMotherGenMuon1->pdgId ()) > 39 && abs (aMotherGenMuon1->pdgId ()) < 50)
            || (abs (aMotherGenMuon1->pdgId ()) > 390 && abs (aMotherGenMuon1->pdgId ()) < 500) || (abs (aMotherGenMuon1->pdgId ()) > 3900 && abs (aMotherGenMuon1->pdgId ()) < 5000))
          isFromCDecay = true;
        if (abs (aMotherGenMuon1->pdgId ()) == 5 || (abs (aMotherGenMuon1->pdgId ()) > 49 && abs (aMotherGenMuon1->pdgId ()) < 60)
            || (abs (aMotherGenMuon1->pdgId ()) > 490 && abs (aMotherGenMuon1->pdgId ()) < 600) || (abs (aMotherGenMuon1->pdgId ()) > 4900 && abs (aMotherGenMuon1->pdgId ()) < 6000))
          isFromBDecay = true;
        //                              if (gm==1) GrandMotherGenMuon = *aMotherGenMuon2;
        //                              if (gm==2) GGrandMotherGenMuon = *aMotherGenMuon2;
        if (gm == 1)
          setGenParticleFromCandidate (GrandMotherGenMuon, aMotherGenMuon2);
        if (gm == 2)
          setGenParticleFromCandidate (GGrandMotherGenMuon, aMotherGenMuon2);
      }
      aMotherGenMuon1 = aMotherGenMuon2;
    }
    if (isFromCDecay)
      muonOrigin = muonOrigin + 1000;
    if (isFromBDecay)
      muonOrigin = muonOrigin + 100;
  }
  return muonOrigin;            //REMARK : cbZW format!
}

int
  MiniTreeProducer::getElectronOrigin (edm::Handle < reco::GenParticleCollection > genParticles, const pat::Electron * thePatElectron, reco::GenParticle & MotherGenElectron,
                                       reco::GenParticle & GrandMotherGenElectron, reco::GenParticle & GGrandMotherGenElectron)
{

  int electronOrigin = -99;
  reco::Candidate * theGenElectron;
  bool matchedGenLepton = false;

  for (reco::GenParticleCollection::const_iterator p = genParticles->begin (); p != genParticles->end (); ++p) {
    reco::Candidate * aGenElectron = (dynamic_cast < reco::Candidate * >(const_cast < reco::GenParticle * >(&*p)));

    if (abs (p->pdgId ()) == 11 && p->status () == 1) {
      if ((thePatElectron->genLepton () != NULL) && abs (thePatElectron->genLepton ()->pt () - aGenElectron->pt ()) < 0.00001) {
        theGenElectron = aGenElectron;
        matchedGenLepton = true;
      }
    }
  }

  if (matchedGenLepton) {
    electronOrigin = 0;
    bool isFromBDecay = false;
    bool isFromCDecay = false;
    if (theGenElectron->mother () != 0 && abs (theGenElectron->pdgId ()) == 11) {
      const reco::Candidate * aMotherGenElectron1 = theGenElectron->mother ();
      const reco::Candidate * aMotherGenElectron2 = theGenElectron->mother ();
      setGenParticleFromCandidate (MotherGenElectron, theGenElectron->mother ());
      //                        MotherGenElectron = *theGenElectron->mother();
      int gm = 0;
      while (aMotherGenElectron2->mother () != 0) {
        gm++;
        aMotherGenElectron2 = aMotherGenElectron2->mother ();
        if (abs (aMotherGenElectron2->pdgId ()) == 24 && abs (aMotherGenElectron1->pdgId ()) == 11)
          electronOrigin = electronOrigin + 1;
        if (abs (aMotherGenElectron2->pdgId ()) == 24 && abs (aMotherGenElectron1->pdgId ()) == 15)
          electronOrigin = electronOrigin + 1;
        if (abs (aMotherGenElectron2->pdgId ()) == 23 && abs (aMotherGenElectron1->pdgId ()) == 11)
          electronOrigin = electronOrigin + 10;
        if (abs (aMotherGenElectron1->pdgId ()) == 4 || (abs (aMotherGenElectron1->pdgId ()) > 39 && abs (aMotherGenElectron1->pdgId ()) < 50)
            || (abs (aMotherGenElectron1->pdgId ()) > 390 && abs (aMotherGenElectron1->pdgId ()) < 500) || (abs (aMotherGenElectron1->pdgId ()) > 3900 && abs (aMotherGenElectron1->pdgId ()) < 5000))
          isFromCDecay = true;
        if (abs (aMotherGenElectron1->pdgId ()) == 5 || (abs (aMotherGenElectron1->pdgId ()) > 49 && abs (aMotherGenElectron1->pdgId ()) < 60)
            || (abs (aMotherGenElectron1->pdgId ()) > 490 && abs (aMotherGenElectron1->pdgId ()) < 600) || (abs (aMotherGenElectron1->pdgId ()) > 4900 && abs (aMotherGenElectron1->pdgId ()) < 6000))
          isFromBDecay = true;
        //                              if (gm==1) GrandMotherGenElectron = *aMotherGenElectron2;
        //                              if (gm==2) GGrandMotherGenElectron = *aMotherGenElectron2;
        if (gm == 1)
          setGenParticleFromCandidate (GrandMotherGenElectron, aMotherGenElectron2);
        if (gm == 2)
          setGenParticleFromCandidate (GGrandMotherGenElectron, aMotherGenElectron2);
      }
      aMotherGenElectron1 = aMotherGenElectron2;
    }

    if (isFromCDecay)
      electronOrigin = electronOrigin + 1000;
    if (isFromBDecay)
      electronOrigin = electronOrigin + 100;
  }
  return electronOrigin;
}

int
  MiniTreeProducer::getTauDecay(edm::Handle<reco::GenParticleCollection> genParticles,
                                const pat::Tau *thePatTau)
{

  int tauDecay = -999;
  bool matchedGenLepton = false;
  reco::GenParticleCollection::const_iterator pMatched;

  for (reco::GenParticleCollection::const_iterator p = genParticles->begin(); p != genParticles->end(); ++p){
    reco::Candidate * aGenTau = (dynamic_cast<reco::Candidate *>(const_cast<reco::GenParticle *>(&*p)));


    if (abs(p->pdgId()) == 15 && p->status() == 2){
#if DEBUG
      cout << "getTauOrigin: thePatElectron->genLepton() " << thePatTau->genLepton() <<endl;
#endif
      if ((thePatTau->genLepton() != NULL) && abs(thePatTau->genLepton()->pt()-aGenTau->pt()) < 0.00001){
        matchedGenLepton = true;
        pMatched = p;
      }
    }
  }

#if DEBUG
  cout << "getTauDecay: matchedGenLepton " << matchedGenLepton<<endl;
#endif
  if (matchedGenLepton){
    tauDecay = -99;

    int tau_children_n = pMatched->numberOfDaughters();
    int sumPdgId = 0;

       for (int k=0; k<tau_children_n; k++)
       {
         int dpdgId = abs(pMatched->daughter(k)->pdgId());
         sumPdgId += dpdgId;

         if (dpdgId == 223 || dpdgId == 221 || dpdgId == 213 || dpdgId == 113 || dpdgId == 323)
         {
           if(pMatched->daughter(k)->status() != 1)
           {
           sumPdgId -= dpdgId;

           for (unsigned int ww=0; ww<pMatched->daughter(k)->numberOfDaughters(); ww++)
           {
            sumPdgId += abs(pMatched->daughter(k)->daughter(ww)->pdgId());

            if (abs(pMatched->daughter(k)->daughter(ww)->pdgId())==311 && pMatched->daughter(k)->daughter(ww)->status()!=1)
            {
              sumPdgId -= 311;
              for (unsigned int v=0; v<pMatched->daughter(k)->daughter(ww)->numberOfDaughters(); v++)
              {
               sumPdgId += pMatched->daughter(k)->daughter(ww)->daughter(v)->pdgId();
              }
             }
            }
           }
          }
         }


      if (sumPdgId ==227)                                                                       { tauDecay = 0;  }//pi+nu
      if (sumPdgId ==229)                                                                       { tauDecay = 1;  }//pi+pi0nu
      if (sumPdgId ==449 || sumPdgId ==338 || sumPdgId ==340)                                   { tauDecay = 2;  }//pi+2pi0nu
      if (sumPdgId ==560)                                                                       { tauDecay = 3;  }//pi+3pi0nu
      if (sumPdgId ==671)                                                                       { tauDecay = 4;  }//pi+4pi0nu
      if (sumPdgId ==315)                                                                       { tauDecay = 5;  }//pi+gammanu
      if (sumPdgId ==360 || sumPdgId ==382)                                                     { tauDecay = 6;  }//pi+pi0nugamma(s)
      if (sumPdgId ==537 || sumPdgId ==357 || sumPdgId ==538)                                   { tauDecay = 7;  }//pi+k0nu
      if (sumPdgId ==468 || sumPdgId ==648 || sumPdgId ==487 || sumPdgId==667 || sumPdgId ==847){ tauDecay = 8;  }//pi+2n(w K0)nu
      if (sumPdgId ==760 || sumPdgId ==769 || sumPdgId ==759)                                   { tauDecay = 9;  }//pi+3n(w pi0)nu
      if (sumPdgId ==471)                                                                       { tauDecay = 10; }//pi+2pi0nugamma

      if (sumPdgId ==649)                                                                       { tauDecay = 30; }//3pi+nu
      if (sumPdgId ==760)                                                                       { tauDecay = 31; }//3pi+pi0nu
      if (sumPdgId ==782)                                                                       { tauDecay = 34; }//3pi+pi0nugamma
      if (sumPdgId ==871)                                                                       { tauDecay = 32; }//3pi+2pi0nu
      if (sumPdgId ==982)                                                                       { tauDecay = 33; }//3pi+3pi0nu

      if (sumPdgId ==337)                                                                       { tauDecay = 20; }//k+nu
      if (sumPdgId ==448)                                                                       { tauDecay = 21; }//k+pi0nu
      if (sumPdgId ==467 || sumPdgId ==647)                                                     { tauDecay = 22; }//k+k0nu
      if (sumPdgId ==559)                                                                       { tauDecay = 23; }//k+2pi0nu
      if (sumPdgId ==578 || sumPdgId ==758)                                                     { tauDecay = 24; }//k+pi0k0nu
      if (sumPdgId ==670)                                                                       { tauDecay = 25; }//k+3pi0nu

      if (sumPdgId ==869)                                                                       { tauDecay = 40; }//k+k+pi+nu

      if (sumPdgId ==1071)                                                                      { tauDecay = 50; }//5pinu
      if (sumPdgId ==1182)                                                                      { tauDecay = 51; }//5pipi0nu

      if (sumPdgId ==39)                                                                        { tauDecay = 100;}//enunu
      if (sumPdgId ==43)                                                                        { tauDecay = 200;}//mununu

   }

  return tauDecay;
}

void MiniTreeProducer::setGenParticleFromCandidate (reco::GenParticle & genp, const reco::Candidate * recoc)
{
  genp.setCharge (recoc->charge ());
  genp.setP4 (recoc->p4 ());
  genp.setMass (recoc->mass ());
  genp.setPdgId (recoc->pdgId ());
  genp.setStatus (recoc->status ());
  genp.setVertex (recoc->vertex ());
}

//define this as a plug-in
DEFINE_FWK_MODULE (MiniTreeProducer);
