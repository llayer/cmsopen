# The comments below build on ones in scripts earlier in this
# tutorial; please see them (e.g. particle_list.py for all the initial
# process configuration) for reference.

import FWCore.ParameterSet.Config as cms




# Somewhat different things happen for simulation than data, most of
# them buried inside helper functions that come with CMSSW. You'll
# need to twiddle this flag if you run this script on a data file. (By
# the way, do not refer to simulated events ("MC") as "data" -- that
# word is reserved for events recorded by the real detector.)

runOnMC = True





# The process name here 'PAT' will be included in the names of all the
# branches produced by this process, written out by the OutputModule
# below.

process = cms.Process('PAT')




# The Summary is even more helpful here since we run a number of
# Paths; the result of the Paths due to Filters run in them is taken
# overall by the OutputModule to decide whether a particular event is
# written out.

process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(False))




# Our standard stuff from before (again, see particle_list.py for
# reference).

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(1000))
process.source = cms.Source('PoolSource', fileNames = cms.untracked.vstring(
"root://eospublic.cern.ch//eos/opendata/cms/MonteCarlo2011/Summer11LegDR/TTJets_TuneZ2_7TeV-madgraph-tauola/AODSIM/PU_S13_START53_LV6-v1/00000/0005D1FB-4BCF-E311-9FE4-002590A8312A.root"
    )
                            )
process.load('FWCore.MessageLogger.MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 10000




# This is the first example of extra configuruation for the
# MessageLogger -- here we set it up so that a particular summary
# table from the PAT sequence gets printed out, showing the statistics
# for the physics objects' collections: minima/maxima/average numbers
# of objects per event.
#
# See https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideMessageLogger
# for (way) more details about MessageLogger. Particularly
# https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideEnablingLogDebugForModules
# can come in handy for debugging reconstruction code, but you
# probably won't need that just yet.

process.MessageLogger.cerr.threshold = 'INFO'
process.MessageLogger.categories.append('PATSummaryTables')
process.MessageLogger.cerr.PATSummaryTables = cms.untracked.PSet(limit = cms.untracked.int32(-1))




# This job will access the detailed geometry ("alignment") and
# magnetic field records from the database, so we need to load
# particular "ESProducers" (event setup producers) that provide these
# to the various modules that will be run.

process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')




# There are many sets of the above records, each corresponding to a
# particular configuration and knowledge of the detector. For example,
# there are different versions of the alignment of the tracking
# modules, and different sets of jet energy corrections. The
# individual ones are referred to by particular "tags"; in turn, a
# "GlobalTag" is a particular collection of those tags that describes
# the entire state of the detector.
#
# They are different for data and MC -- in MC we know the absolute
# truth, but we attempt to "smear" the results so that the
# distributions of the reconstructed quantities are similar to those
# in the data.
#
# You can look at
# https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideFrontierConditions
# to see a list and descriptions of all the GlobalTags, but it is
# rather impenetrable if you don't know what all the details are.

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'START53_V7E::All' if runOnMC else 'FT_53_V6_AN1::All'

# Event cleaning: some events in data samples are affected by the
# sometimes noisy LHC and CMS environment. To remove them, we can
# insert EDFilters into the Paths, which process an event in a method
# filter() analogous to EDAnalyzer::analyze(), but filter() returns a
# boolean flag whether to keep the event.
#
# For example, if you have a filter A along with an analyzer B, and
# you only want to run B's analyze() method if A returns true, you
# would write a Path like this:
#   cms.Path(process.A + process.B)
#
# There are more (complicated) examples below. See
# https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideEDMPathsAndTriggerBits
# for way more information.
#
# There are stock filters designed to remove the noisy events:
# consider reading
# https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookCollisionsDataAnalysis
# when you start looking at data, but they're here for completeness --
# they only remove a tiny fraction of MC events.


# hltPhysicsDeclared is a simple filter to remove events taken while
# not all of the detector was on and ready for physics. (Find out
# about "DCS" to see how that works.)

process.load('HLTrigger.special.hltPhysicsDeclared_cfi')


# FilterOutScraping removes events where the beam "scraped" the pipe
# or other elements of the machine, producing a spray of noise. These
# events are detected by the quantity and quality of reconstructed
# tracks.

from DPGAnalysis.Skims.goodvertexSkim_cff import noscraping as FilterOutScraping
process.FilterOutScraping = FilterOutScraping


# We also remove backgrounds due to cosmics/the beam by requiring the
# presence of a reconstructed primary vertex -- a collection of tracks
# that appear to point to a single location along the nominal beam
# axis, fit to provide the precise value of that point.
#
# An EDFilter, in addition to returning a flag whether the event
# passes certain criteria, can also write new branches ("products")
# into the Event. goodOfflinePrimaryVertices uses this capability to
# apply cuts to the input set of primary vertices and copy the
# selected ones into a new branch.

process.load('CommonTools.ParticleFlow.goodOfflinePrimaryVertices_cfi')
process.goodOfflinePrimaryVertices.filter = cms.bool(True)


# Recommendations from the MET group for cleaning events.  These
# remove events with anomalously high MET due to particular detector
# effects (e.g. noise). See
# https://twiki.cern.ch/twiki/bin/view/CMS/MissingETOptionalFilters
# for further description.

"""
process.load('CommonTools.RecoAlgos.HBHENoiseFilter_cfi')
process.load('RecoMET.METFilters.hcalLaserEventFilter_cfi')
process.hcalLaserEventFilter.vetoByRunEventNumber = False
process.hcalLaserEventFilter.vetoByHBHEOccupancy  = True
process.load('RecoMET.METFilters.EcalDeadCellTriggerPrimitiveFilter_cfi')
process.EcalDeadCellTriggerPrimitiveFilter.tpDigiCollection = cms.InputTag('ecalTPSkimNA')
process.load('RecoMET.METFilters.trackingFailureFilter_cfi')
process.trackingFailureFilter.VertexSource = cms.InputTag('goodOfflinePrimaryVertices')
process.load('RecoMET.METFilters.eeBadScFilter_cfi')
process.load('RecoMET.METAnalyzers.CSCHaloFilter_cfi')




# Instead of applying the above filters on noisy events at tupling
# time (once you decide not to write an event, it's gone, but they're
# easy to throw out later), schedule separate paths for all the "good
# data" filters. This way, the results of each get stored in a small
# TriggerResults::PAT object in the event, which can be accessed by
# future analysis code for filtering the event then. (See
# plugins/PAT2Ntuple.cc and test/PAT2Ntuple.py for an example of
# accessing them, as well as using "real" trigger paths.)
#
# Make one path for each so they can be accessed separately in the
# TriggerResults object; the "All" path that is the product of all of
# the filters isn't necessary (can be calculated from the logical AND
# of all the others), but it can be nice for convenience.
#
# Two of the event noise filters don't work on fastsim (since the
# effects they clean against aren't implemented in fastsim versus full
# sim.)
process.eventCleaninghltPhysicsDeclared                 = cms.Path(process.hltPhysicsDeclared)
process.eventCleaningFilterOutScraping                  = cms.Path(process.FilterOutScraping)
process.eventCleaninggoodOfflinePrimaryVertices         = cms.Path(process.goodOfflinePrimaryVertices)
process.eventCleaninghcalLaserEventFilter               = cms.Path(process.hcalLaserEventFilter)
process.eventCleaningEcalDeadCellTriggerPrimitiveFilter = cms.Path(process.EcalDeadCellTriggerPrimitiveFilter)
process.eventCleaningtrackingFailureFilter              = cms.Path(process.trackingFailureFilter)
process.eventCleaningeeBadScFilter                      = cms.Path(process.eeBadScFilter)

process.eventCleaningAll = cms.Path(process.hltPhysicsDeclared +
                                    process.FilterOutScraping +
                                    process.goodOfflinePrimaryVertices +
                                    process.hcalLaserEventFilter +
                                    process.EcalDeadCellTriggerPrimitiveFilter +
                                    process.trackingFailureFilter +
                                    process.eeBadScFilter)

if not isFastsim:
    process.eventCleaningHBHENoiseFilter    = cms.Path(process.HBHENoiseFilter)
    process.eventCleaningCSCTightHaloFilter = cms.Path(process.CSCTightHaloFilter)
    process.eventCleaningAll += process.HBHENoiseFilter + process.CSCTightHaloFilter

"""


# Go ahead and define the OutputModule here, since some of the PAT
# tools expect it to be defined before calling them.
#
# The OutputModule determines to what file, which events, and which
# branches are written out. The file name is obvious -- the path is
# relative to your current working directory.
#
# The logical OR of the paths listed in SelectEvents (which will be
# redefined much farther below) determines whether a given event is
# written to disk.
#
# outputCommands tells the OutputModule which branches to keep. We'll
# redefine that at the end of the file, since the PAT tools mess with
# it themselves -- we'll override their intelligence.

process.out = cms.OutputModule('PoolOutputModule',
                               fileName = cms.untracked.string('pat.root'),
                               SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring('hadronic')),
                               outputCommands = cms.untracked.vstring('keep *'),
                               )

#
# Trigger
#

# PAT Layer 0+1
process.load("PhysicsTools.PatAlgos.patSequences_cff")
process.load("PhysicsTools.PatAlgos.triggerLayer1.triggerProducer_cff")

#trigger matching & dynamic prescales stuff
from PhysicsTools.PatAlgos.tools.trigTools import *
switchOnTrigger( process )



################################################################################################################################################


####################################
#  For jet matching at HLC level
#  Used by tau+jets analysis
####################################
process.jetMatchHLTJets = cms.EDProducer(
  # matching in DeltaR, sorting by best DeltaR
  "PATTriggerMatcherDRLessByR"
  # matcher input collections
, src     = cms.InputTag( 'selectedPatJetsPF' )
#, src     = cms.InputTag( 'selectedPatTausPF2PAT' )
, matched = cms.InputTag( 'patTrigger' )
  # selections of trigger objects
#, matchedCuts = cms.string( 'type( "TriggerTau" ) && path( "HLT_QuadJet40_IsoPFTau40_v*" )' )
, matchedCuts = cms.string( 'type( "TriggerJet" ) && path( "HLT_QuadJet40_IsoPFTau40_v*", 0, 0) && filter( "hltQuadJet40IsoPFTau40" )') 
  # selection of matches
, maxDPtRel   = cms.double( 0.5 ) # no effect here
, maxDeltaR   = cms.double( 0.5 )
, maxDeltaEta = cms.double( 0.2 ) # no effect here
  # definition of matcher output
, resolveAmbiguities    = cms.bool( True )
, resolveByMatchQuality = cms.bool( True )
)
switchOnTriggerMatching( process, triggerMatchers = [ 'jetMatchHLTJets' ] )
################################################################################################################################################





# Now the actual PAT configuration. This is accomplished mostly by
# stock functions in CMSSW, but they require some parameters to be
# passed in, as well as configuration of the added modules after the
# fact.

# The jet energy correction levels we want applied. PAT will take care
# of the rest, once we tell it this plus what set of corrections
# (related to what jets we'll use). Data requires one extra level. See
# https://twiki.cern.ch/twiki/bin/view/CMS/JetEnergyScale for links to
# descriptions, particularly
# https://twiki.cern.ch/twiki/bin/view/CMS/IntroToJEC

jecLevels = ['L1FastJet', 'L2Relative', 'L3Absolute']
#jecLevels = ['L1Offset', 'L2Relative', 'L3Absolute']
if not runOnMC:
    jecLevels.append('L2L3Residual')
#jecLevels += ['useL5Flavor', 'useL7Parton']








# Load the PAT modules and sequences. Use this as a black box for now,
# but you can read about it at
# https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuidePAT

process.load('PhysicsTools.PatAlgos.patSequences_cff')




# PF2PAT (I think this is soon to be renamed "PFBRECO") is the
# implementation of the Global Event Description (GED). The GED uses
# the entire event to improve upon the reconstruction of particular
# physics objects (e.g. using the reconstructed muon info to improve
# the MET resolution). As input to this, the reconstruction uses
# particle flow (PF) techniques to uniquely identify a particular set
# of reconstructed leptons, photons, and charged and neutral
# hadrons.
#
# For an intro to PF in CMS, see
# https://indico.cern.ch/getFile.py/access?contribId=11&resId=0&materialId=slides&confId=199210
# (accessible, but at a higher level without a concrete example, also
# describes the "top projections" enabled below)
#
# and
# https://indico.cern.ch/getFile.py/access?contribId=10&resId=2&materialId=slides&confId=199210
# (denser with a focus on the actual software implementation, but has a
# particular example of the way the tracker and calorimeter info are
# combined for an example event).
#
# The parameters below are mostly self-explanatory given the above
# comments or their names referring to physics objects' algorithms,
# but there is one left that is less obvious.
#
# "postfix" determines what the module names will be postfixed
# with. This is handy for e.g. comparing the output of two different
# jet reconstruction strategies -- you would run usePF2PAT twice with
# different parameters and different postfixes, and then configure the
# remaining modules as below, but twice with different
# choices. Typically the lines like
# "process.selectedPatMuonsPF.someParameter = number" below (where
# 'PF' is the postfix we set in the next few lines) are accessed in a
# more general but complicated way, so that the postfix is easily
# changed for this type of workflow. I have removed that here to make
# the configuration less opaque. If you need to do a workflow with two
# or more postfixes, you can ask how to make it more general again.
#
# jetAlgo = 'AK5' and jetCorrections = 'AK5PFchs' specify that we want
# anti-kT R=0.5 clustering of PF candidates with charged hadron
# subtraction (CHS) applied. The latter is used to identify and remove
# particles from pileup, by associating their tracks with the non-main
# primary vertex. (The primary vertex collection is sorted by
# decreasing sum pT^2; the first one in the collection is usually
# taken as *the* primary vertex.) CHS is what is meant when the name
# "PFnoPileUp" is used.
#
# typeIMetCorrections = True indicates we want to apply the jet energy
# corrections in the MET calculation.

from PhysicsTools.PatAlgos.tools.pfTools import usePF2PAT
usePF2PAT(process,
          runPF2PAT = True,
          postfix = 'PF',
          runOnMC = runOnMC,
          pvCollection = cms.InputTag('goodOfflinePrimaryVertices'),
          jetAlgo = 'AK5',
          jetCorrections = ('AK5PFchs', jecLevels),
          typeIMetCorrections = True,
          )




# Turn on the "top projections". This has nothing to do with top quark
# physics, but rather refers to separating sequentially out from the
# entire list of PF candidates the ones identified as pileup (from
# "wrong" primary vertices), muons, electrons, jets, and taus. The
# "No" in each collection name indicates the thing referred to has
# been removed. (See the above first set of slides for a example
# picture.) The input to the jet clustering is then taken from the
# pfNoTauPF -> pfJetsPF collection.

process.pfNoPileUpPF  .enable = True
process.pfNoMuonPF    .enable = True
process.pfNoElectronPF.enable = True
process.pfNoJetPF     .enable = True
process.pfNoTauPF     .enable = True





# Next comes a bunch of particular configuration of the PF selection
# cuts -- treat as given for now, but this is the standard
# configuration used by the ttbar group.

if 'L1FastJet' in jecLevels:
    process.pfPileUpIsoPF.checkClosestZVertex = True # new PF isolation with L1Fastjet CHS

process.pfMuonsFromVertexPF.d0Cut = 0.2
process.pfMuonsFromVertexPF.dzCut = 0.5
process.pfSelectedMuonsPF.cut = 'pt > 5.'
#process.pfSelectedMuonsPF.cut += ' && ' + muonCut # can use minimal (veto) muon selection on top of base pf cut
process.pfIsolatedMuonsPF.isolationCut = 0.2

process.pfElectronsFromVertexPF.d0Cut = 0.2
process.pfElectronsFromVertexPF.dzCut = 0.5
process.pfSelectedElectronsPF.cut = 'pt > 5. && gsfTrackRef.isNonnull && gsfTrackRef.trackerExpectedHitsInner.numberOfLostHits < 2'
#process.pfSelectedElectronsPF.cut += ' && ' + electronCut # can use minimal (veto) electron selection cut on top of base pf cut
process.pfIsolatedElectronsPF.isolationCut = 0.2

process.pfIsolatedElectronsPF.isolationValueMapsCharged   = cms.VInputTag('elPFIsoValueCharged03PFIdPF')
process.pfIsolatedElectronsPF.deltaBetaIsolationValueMap  = 'elPFIsoValuePU03PFIdPF'
process.pfIsolatedElectronsPF.isolationValueMapsNeutral   = cms.VInputTag('elPFIsoValueNeutral03PFIdPF', 'elPFIsoValueGamma03PFIdPF')
process.pfElectronsPF.isolationValueMapsCharged           = cms.VInputTag('elPFIsoValueCharged03PFIdPF')
process.pfElectronsPF.deltaBetaIsolationValueMap          = 'elPFIsoValuePU03PFIdPF'
process.pfElectronsPF.isolationValueMapsNeutral           = cms.VInputTag('elPFIsoValueNeutral03PFIdPF', 'elPFIsoValueGamma03PFIdPF')
process.patElectronsPF.isolationValues.pfNeutralHadrons   = 'elPFIsoValueNeutral03PFIdPF'
process.patElectronsPF.isolationValues.pfChargedAll       = 'elPFIsoValueChargedAll03PFIdPF'
process.patElectronsPF.isolationValues.pfPUChargedHadrons = 'elPFIsoValuePU03PFIdPF'
process.patElectronsPF.isolationValues.pfPhotons          = 'elPFIsoValueGamma03PFIdPF'
process.patElectronsPF.isolationValues.pfChargedHadrons   = 'elPFIsoValueCharged03PFIdPF'

"""
Add tau cleaning!!
"""


# Sometimes you'll need to patch the current version of CMSSW to
# include the latest features while waiting for a new version to be
# assembled. In setup_for_pat.csh, one particular thing that was added
# were some electron id files; here they get used.

process.load('EgammaAnalysis.ElectronTools.electronIdMVAProducer_cfi')
#process.eidMVASequence = cms.Sequence(process.mvaTrigV0 + process.mvaNonTrigV0)
#process.patElectronsPF.electronIDSources.mvaTrigV0    = cms.InputTag("mvaTrigV0")
#process.patElectronsPF.electronIDSources.mvaNonTrigV0 = cms.InputTag("mvaNonTrigV0")




# A Muon object consists of three track fits: one in the inner
# tracker, one in the outer muon chambers, and a combined "global"
# fit. The latter two are muon specific tracks and so there are only a
# few per event, so it would not be painful to keep their entire
# branches, but the PATMuonProducer is configured by default to
# "embed" the tracks in the pat::Muons, letting us drop the original
# recoTracks_globalMuons_* and recoTracks_standAloneMuons_*
# branches. For some curious reason, the inner tracker tracks are not
# embedded by default, so if you do not turn this on you have to keep
# around a very large (expensive in terms of disk space) branch of
# "generalTracks".

process.patMuonsPF.embedTrack = True




# Also embed in the pat::Jets some extra information related to
# b-tagging, namely the results of the secondary vertex (SV)
# reconstruction.  This includes flight distance/direction and
# information about the tracks used to form the SV. See
# http://cmssdt.cern.ch/SDT/doxygen/CMSSW_5_2_3/doc/html/dd/d9b/classreco_1_1SecondaryVertexTagInfo.html
# for info on what you can get out of this object.

process.patJetsPF.addTagInfos = True




# After producing the physics objects (by modules called e.g.
# "patElectronsPF"), there are EDProducers that select a certain
# subset of them given a cut string. We configure that cut for electrons,
# muons, and jets here. For electrons and muons, the selected ones
# form a set we typically call "veto" leptons, which are used e.g. to
# pick out hadronic events in which there are no leptons from the top
# decay chain. The veto selection is also used in the semi-leptonic
# and di-leptonic cases: after selecting the "signal" lepton(s) for
# the particular channel (using a more restrictive set of cuts), you
# check that there are no other veto leptons in the event.
#
# We'll use this below in the OutputModule to select events to be
# written, but the electrons and muons that are saved will only be the
# loose veto collections. Then at analysis level, if you want to study
# semi-leptonic events, you would re-apply the particular
# cuts. (Alternatively, you could just write out the branches semilep*
# and dilep* produced below and directly access them later, but I find
# having just one collection of electrons and one of muons in the
# event makes things simpler.)
#
# (Python syntax note: an \ at the end of a line is the continuation
# character, which lets you break up what should be all one long line
# into several for ease of reading. Also, putting two strings next to
# each other such as 'foo ' 'bar' is equivalent to just writing 'foo
# bar'.)



#'electronID("mvaTrigV0") > 0. && ' \
process.selectedPatElectronsPF.cut = 'pt > 20. && ' \
                                     'abs(eta) < 2.5 && ' \
                                     '(chargedHadronIso + max(0.,neutralHadronIso) + photonIso - 0.5*puChargedHadronIso)/et < 0.2'

process.selectedPatMuonsPF.cut = 'isPFMuon && ' \
                                 '(isGlobalMuon || isTrackerMuon) && ' \
                                 'pt > 10. && ' \
                                 'abs(eta) < 2.5 && ' \
                                 '(chargedHadronIso+neutralHadronIso+photonIso-0.5*puChargedHadronIso)/pt < 0.2'

process.selectedPatJetsPF.cut = 'pt > 20. && ' \
                                'abs(eta) < 2.5 && ' \
                                'numberOfDaughters > 1 && ' \
                                'neutralHadronEnergyFraction < 0.99 && ' \
                                'neutralEmEnergyFraction < 0.99 && ' \
                                '(abs(eta) >= 2.4 || (chargedEmEnergyFraction < 0.99 && chargedHadronEnergyFraction > 0. && chargedMultiplicity > 0))'


process.selectedPatTausPF.cut = 'pt > 45. &&' \
                                ' tauID("byMediumCombinedIsolationDeltaBetaCorr3Hits") > 0.5'




# The PAT producers need to know whether we're running on MC events or
# data; for the latter, this next part is mainly to remove the MC
# truth matching. (Each reconstructed PAT object stores a pointer to
# the generator-level object that is its closest match, with matching
# done in delta-R = sqrt((delta eta)^2 + (delta phi)^2; see
# plugins/PAT2Ntuple.cc for an example of accessing these matches.)

from PhysicsTools.PatAlgos.tools.coreTools import runOnData
if not runOnMC:
    runOnData(process, names = ['PFAll'], postfix = 'PF')




# We remove PAT photons here because they are not fully commisssioned
# in PF. (You could turn them on to check their performance.) This
# also turns off a cleaning step, which in the default PAT workflow
# tries to remove overlap between the physics object collections
# (e.g. removing muons from being reconstructed as degenerate "jets"),
# but this is not necessary with PF/the GED, especially with the top
# projections described above enabled.

from PhysicsTools.PatAlgos.tools.coreTools import removeSpecificPATObjects
removeSpecificPATObjects(process, names = ['Photons'], postfix = 'PF')




# Now we configure the job to keep ttbar events in all three decay
# modes. For hadronic, we will require that there be four
# reconstructed jets (the main hadronic trigger requires this anyway).
#
# For leptonic and dileptonic, we want to keep events with either one
# or two leptons (electrons or muons -- tau reconstruction is
# harder). What we will call a lepton depends on the mode: with one
# lepton, we need to cut a bit harder to increase the ttbar signal
# over the (W+jets) background. If we have two leptons, we don't have
# to cut as hard.
#
# The next eight statements make separate EDProducers for each cut
# case so that a simple EDFilter module later ("countPatLeptons*") can
# count the number that pass. As discussed above, the lepton
# collections would be written out to disk, but we disable them being
# written out below to have only one collection each of electrons and
# muons.
#
# Specific cuts are those from the TOP reference selection, which can
# be seen at
# https://twiki.cern.ch/twiki/bin/viewauth/CMS/TWikiTopRefEventSel .

"""
process.semilepMuonsPF     = cms.EDFilter('PATMuonSelector',     src = cms.InputTag('selectedPatMuonsPF'))
process.semilepElectronsPF = cms.EDFilter('PATElectronSelector', src = cms.InputTag('selectedPatElectronsPF'))

process.semilepMuonsPF.cut = cms.string('isPFMuon && ' \
                                        'isGlobalMuon && ' \
                                        'pt > 26. && ' \
                                        'abs(eta) < 2.1 && ' \
                                        'globalTrack.normalizedChi2 < 10. && ' \
                                        'track.hitPattern.trackerLayersWithMeasurement > 5 && ' \
                                        'globalTrack.hitPattern.numberOfValidMuonHits > 0 && ' \
                                        'innerTrack.hitPattern.numberOfValidPixelHits > 0 && ' \
                                        'numberOfMatchedStations > 1 && ' \
                                        '(chargedHadronIso+neutralHadronIso+photonIso-0.5*puChargedHadronIso)/pt < 0.12')


process.semilepElectronsPF.cut = cms.string('pt > 30. && ' \
                                            'abs(eta) < 2.5 && ' \
                                            'abs(superCluster.eta) < 1.4442 && ' \
                                            'abs(superCluster.eta) > 1.5660 && ' \
                                            'passConversionVeto && ' \
                                            '(chargedHadronIso + max(0.,neutralHadronIso) + photonIso - 0.5*puChargedHadronIso)/et < 0.1')







# Here are the EDFilters promised above to count the leptons. On the
# second one, we use the clone() method to fully copy the
# configuration from the first, changing the parameters listed.
# (Remember: you can use the LXR to find the source code for
# "PATLeptonCountFilter".)

process.countPatLeptonsSemileptonicPF = cms.EDFilter("PATLeptonCountFilter",
                                                     minNumber = cms.uint32(1),
                                                     maxNumber = cms.uint32(1),
                                                     countElectrons = cms.bool(True),
                                                     #electronSource = cms.InputTag("selectedPatElectronsPF"),
                                                     electronSource = cms.InputTag("semilepElectronsPF"),
                                                     countMuons = cms.bool(True),
                                                     #muonSource = cms.InputTag("selectedPatMuonsPF"),
                                                     muonSource = cms.InputTag("semilepMuonsPF"),
                                                     countTaus = cms.bool(False),
                                                     tauSource = cms.InputTag("selectedPatTausPF"),
                                                     )

"""




# We require numbers of jets based on the trigger: hadronic channel
# will have at least a 4-jet trigger (maybe 6!), while semileptonic
# uses a 3-jet trigger. Dileptonic has no jets in trigger, but we'll
# require at least one b-tag anyway, so require one jet there.


process.countPatJetsSemileptonicPF = process.countPatJetsPF.clone(minNumber = 4)





# Next we construct the Paths that will be run to produce all the PAT
# collections, then check for the minimum multiplicities we prescribed
# above. Remember that the result of a Path is true if all the modules
# on it that are EDFilters return true (unless they are explicity
# ignored, see the note about cms.ignore below).
#
# For the PAT to work as configured above, we need to run three other
# non-PAT-specific collections: the "good" offline primary vertices
# (imported above in the event cleaning list, but here we actually
# need the copies of the PV objects), and two for electron
# identification that uses a multivariate technique. (The electron MVA
# ids need to be re-run because we want to use some weights that are
# updated with respect to the CMSSW version we're using --
# setup_for_pat.csh downloaded them.)
#
# Then the three paths each require all the PAT collections, so we
# specify the "patPF2PATSequencePF" to run them. The PAT tools took
# care of setting up all the modules on that path -- to see what's
# included (it's a monster), you can open up this file with the python
# interpreter as described in the slides.
#
# The cms.ignore sequence tells CMSSW that we don't want to use any of
# the filter decisions, if there are any, from the objects
# specified. This lets us be sure that if an object is not written to
# disk, it is because it failed our own jet/lepton multiplicity
# filters.
#
# And as a final note, just because all these modules are on three
# paths, they won't be run three times per event. CMSSW knows to run
# e.g. patJetsPF only once, then reuse the results. We specify it this
# way to be clear to CMSSW that each path needs to have the PAT
# modules ran before running the modules specific to each Path.
isttbar = True
if isttbar:
    process.load("TopQuarkAnalysis.TopEventProducers.sequences.ttGenEvent_cff")

#cms.ignore(process.mvaTrigV0) + \
#cms.ignore(process.mvaNonTrigV0) + \
base_path = cms.ignore(process.goodOfflinePrimaryVertices) + \
            process.patPF2PATSequencePF



#process.pSemileptonic = cms.Path(base_path + process.patJetPartonMatch1 + process.countPatJetsSemileptonicPF + process.semilepMuonsPF + process.semilepElectronsPF + process.countPatLeptonsSemileptonicPF)


process.hadronic = cms.Path(base_path + process.countPatJetsSemileptonicPF + process.makeGenEvt)


# As mentioned above, the OutputModule needs to know that we want to
# use the OR of the three Paths we just made. This syntax does that.

#process.out.SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring('pSemileptonic'))

process.out.SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring('hadronic'))


# Nearly done: now come the outputCommands for the OutputModule, as
# promised above. The order of the commands matters: the first line
# says drop all branches, but then the subsequent lines re-enable
# keeping particular branches. Wildcards ("*") mean any matching text.

"""
process.out.outputCommands = cms.untracked.vstring(
    'keep *'
)
"""

process.out.outputCommands = cms.untracked.vstring(
    'drop *',
    'keep *_selectedPatElectrons*_*_*',
    'keep *_selectedPatMuons*_*_*',
    'keep *_semilepMuons*_*_*',
    'keep *_selectedPatTaus*_*_*',
    'keep *_semilepElectrons*_*_*',
    'keep *_selectedPatJets*_*_*',
    'keep *_selectedPatJets*_genJets_*',
    'drop *_selectedPatJets*_pfCandidates_*',
    'drop *_selectedPatJetsForMETtype1p2CorrPF_*_*',
    'drop *_selectedPatJetsForMETtype2CorrPF_*_*',
    'drop CaloTowers_*_*_*',
    'keep *_patMETs*_*_*',
    'keep *_patPFMetPF*_*_*',
    'keep *_patType1CorrectedPFMet*_*_*',
    'keep *_patType2CorrectedPFMet*_*_*',
    'keep recoGenParticles_genParticles_*_*',
    'keep GenEventInfoProduct_*_*_*',
    'keep GenRunInfoProduct_*_*_*',
    'keep *_offlineBeamSpot_*_*',
    'keep *_goodOfflinePrimaryVertices_*_*',
    'keep edmTriggerResults_TriggerResults__HLT*',
    'keep edmTriggerResults_TriggerResults__REDIGI*',
    'keep edmTriggerResults_TriggerResults__PAT', # for post-tuple filtering on the goodData paths
    'keep *_hltTriggerSummaryAOD__HLT*',
    'keep *_hltTriggerSummaryAOD__REDIGI*',
      ## genEvt
    'keep *_decaySubset_*_*',
    'keep *_initSubset_*_*',
    'keep *_genEvt_*_*',
    'keep *_patTrigger*_*_*',
    )

# And finally, since we're writing out to the disk, we need an EndPath
# to run the OutputModule. An EndPath runs the specified modules after
# every other module on normal Paths have been run on it. The
# OutputModule needs to be on an EndPath since it has to have all the
# information from every module run to be able to write out a
# TriggerResults object, which stores the result of each Path.
#
# This is part of how the high-level trigger (HLT) is implemented both
# in simulation, or when re-run offline on data to test the effects of
# a new menu of triggers. This also allows well as storing the results
# of any Paths you run -- e.g. the eventCleaning* paths above. See
# plugins/PAT2Ntuple.cc for an example.

process.outp = cms.EndPath(process.out)
