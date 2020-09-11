import ROOT
from collections import namedtuple
import numpy as np

# Object selections - will be moved to a separate file later

def reliso03(TrkIso03, ECaloIso03, HCaloIso03, pt):
    return (TrkIso03+ECaloIso03+HCaloIso03) / pt


def select_muons(evt, eta_cut = 2.5, pt_cut = 10., reliso_cut = 0.15, vtxmatch_cut = 1.):

    Muon = namedtuple('Muon', 'pt px py pz e')
    good_muons = []

    for iMu in range(evt.nMuon):

        """
        //if((muon.MuonType-(muon.MuonType/10)*10) ==0) continue; //isTrackerMuon
        //if(muon.Chi2           >=MuonNormChi2_)         continue;
        //if(muon.NTrValidHits   <=MuonNofValidTrHits_)   continue;
        //if(muon.NValidHits     <=MuonNofValidHits_  )   continue;
        //if(fabs(muon.D0Inner)  >=MuonD0Cut_)            continue;
        """

        # Global muon
        if evt.Muon_isGlobalMuon[iMu] == False: continue
        # Eta cut
        if abs(evt.Muon_eta[iMu]) > eta_cut: continue
        # Pt cut
        if abs(evt.Muon_pt[iMu]) < pt_cut: continue
        # Isolation
        iso = reliso03(evt.Muon_TrkIso03[iMu], evt.Muon_ECaloIso03[iMu],
                       evt.Muon_HCaloIso03[iMu], evt.Muon_pt[iMu])
        if iso > reliso_cut: continue
        if abs(evt.Muon_z[iMu] - evt.PV_z) > vtxmatch_cut: continue

        good_muons.append(Muon( evt.Muon_pt[iMu], evt.Muon_px[iMu], evt.Muon_py[iMu],
                                evt.Muon_pz[iMu], evt.Muon_e[iMu] ))

    return good_muons


def select_electrons(evt, eta_cut = 2.5, pt_cut = 10., reliso_cut = 0.15, vtxmatch_cut = 1.):

    Electron = namedtuple('Electron', 'pt px py pz e')
    good_electrons = []

    for iEl in range(evt.nElectron):

        """
        bool hadId(electron.elecIdWP90_c & 0x1);
        if(!hadId) continue;
        """

        # EID
        if evt.Electron_cutbasedid[iEl] == 0: continue
        # Eta cut
        if abs(evt.Electron_eta[iEl]) > eta_cut: continue
        # Pt cut
        if abs(evt.Electron_pt[iEl]) < pt_cut: continue
        # Isolation
        iso = reliso03(evt.Electron_TrkIso03[iEl], evt.Electron_ECaloIso03[iEl],
                       evt.Electron_HCaloIso03[iEl], evt.Electron_pt[iEl])
        if iso > reliso_cut: continue
        if abs(evt.Electron_z[iEl] - evt.PV_z) > vtxmatch_cut: continue

        good_electrons.append(Electron( evt.Electron_pt[iEl], evt.Electron_px[iEl], evt.Electron_py[iEl],
                                        evt.Electron_pz[iEl], evt.Electron_e[iEl] ))

    return good_electrons


def select_taus(evt, eta_cut = 2.3, pt_cut = 10., vtxmatch_cut = 1., dxy_cut=0.04, leadTrackPt_cut=5.):

    Tau = namedtuple('Tau', 'eta pt px py pz e pxHLT pyHLT pzHLT eHLT')
    good_taus = []

    for iTau in range(evt.nTau):

        #print "Taudisc", evt.Tau_byMediumCombinedIsolationDeltaBetaCorr[iTau]
        #print "leadTrackPt", evt.Tau_leadTrackPt[iTau]

        if evt.Tau_byMediumCombinedIsolationDeltaBetaCorr[iTau] == 0: continue
        if evt.Tau_againstMuonTight[iTau] == 0: continue
        if evt.Tau_againstElectronTight[iTau] == 0: continue

        #print "Taudisc_b", evt.Tau_byMediumCombinedIsolationDeltaBetaCorr[iTau] == 0
        #print "Taudisc2", evt.Tau_byMediumCombinedIsolationDeltaBetaCorr[iTau]


        # Lead track pt
        if evt.Tau_leadTrackPt[iTau] < leadTrackPt_cut: continue
        # Eta
        if (abs(evt.Tau_eta[iTau])<1.566) & (abs(evt.Tau_eta[iTau])>1.4442): continue
        if abs(evt.Tau_eta[iTau]) > eta_cut: continue
        # Pt cut
        if abs(evt.Tau_pt[iTau]) < pt_cut: continue
        # Vtx cut
        if abs(evt.Tau_z[iTau] - evt.PV_z) > vtxmatch_cut: continue
        if abs( evt.Tau_dxy[iTau] )  >= dxy_cut: continue
        #print evt.Tau_dxy[iTau]
        if trigger == "45":
            good_taus.append(Tau( evt.Tau_eta[iTau], evt.Tau_pt[iTau], evt.Tau_px[iTau], evt.Tau_py[iTau],
                                  evt.Tau_pz[iTau], evt.Tau_e[iTau], evt.Tau_pxHLT45[iTau],
                                  evt.Tau_pyHLT45[iTau], evt.Tau_pzHLT45[iTau], evt.Tau_eHLT45[iTau]))
        else:
            good_taus.append(Tau( evt.Tau_eta[iTau], evt.Tau_pt[iTau], evt.Tau_px[iTau], evt.Tau_py[iTau],
                          evt.Tau_pz[iTau], evt.Tau_e[iTau], evt.Tau_pxHLT40[iTau],
                          evt.Tau_pyHLT40[iTau], evt.Tau_pzHLT40[iTau], evt.Tau_eHLT40[iTau]))

    return good_taus


def select_jets(evt, eta_cut = 2.5, pt_cut = 10., trigger="45"):

    Jet = namedtuple('Jet', 'eta pt px py pz e pxHLT pyHLT pzHLT eHLT')
    good_jets = []

    for iJet in range(evt.nJet):

        # Eta cut
        if abs(evt.Jet_eta[iJet]) > eta_cut: continue
        # Pt cut
        if abs(evt.Jet_pt[iJet]) < pt_cut: continue

        if trigger == "45":
            good_jets.append(Jet( evt.Jet_eta[iJet], evt.Jet_pt[iJet], evt.Jet_px[iJet], evt.Jet_py[iJet],
                                  evt.Jet_pz[iJet], evt.Jet_e[iJet], evt.Jet_pxHLT45[iJet],
                                  evt.Jet_pyHLT45[iJet], evt.Jet_pzHLT45[iJet], evt.Jet_eHLT45[iJet]))
        else:
            good_jets.append(Jet( evt.Jet_eta[iJet], evt.Jet_pt[iJet], evt.Jet_px[iJet], evt.Jet_py[iJet],
                          evt.Jet_pz[iJet], evt.Jet_e[iJet], evt.Jet_pxHLT40[iJet],
                          evt.Jet_pyHLT40[iJet], evt.Jet_pzHLT40[iJet], evt.Jet_eHLT40[iJet]))
        #print "HLT", evt.Jet_pxHLT40[iJet], evt.Jet_pxHLT45[iJet]

    return good_jets

def clean_taus(taus, muons, electrons, R=0.4):

    clean_taus = []
    for tau in taus:

        deltaRmu = 10000
        deltaRel = 10000

        tau_4vec = ROOT.TLorentzVector(tau.px, tau.py, tau.pz, tau.e)

        for mu in muons:

            mu_4vec = ROOT.TLorentzVector(mu.px, mu.py, mu.pz, mu.e)
            deltaR = tau_4vec.DeltaR(mu_4vec)
            if(deltaR < deltaRmu):
                deltaRmu = deltaR

        for el in electrons:

            el_4vec = ROOT.TLorentzVector(el.px, el.py, el.pz, el.e)
            deltaR = tau_4vec.DeltaR(el_4vec)
            if(deltaR < deltaRel):
                deltaRel = deltaR


        if ( deltaRmu > R)  & (deltaRel > R):
            clean_taus.append(tau)

    return clean_taus


def select_tauHLT( evt ):

    TauHLT = namedtuple('TauHLT', 'pt px py pz e')

    good_tauHLTs = []
    for iTauHLT in range(evt.nTauHLT):
        good_tauHLTs.append( TauHLT(evt.TauHLT_pt[iTauHLT], evt.TauHLT_px[iTauHLT], evt.TauHLT_py[iTauHLT],
                                    evt.TauHLT_pz[iTauHLT], evt.TauHLT_e[iTauHLT]) )

    return good_tauHLTs


def trigger_jets(jets):


    pass_tag_cond = []
    pass_probe_cond = False
    pass_probe_match = False
    probe_jet_pt = -9999
    # Loop over jets
    for idx, jet in enumerate(jets):

       # print jet.pt, jet.pxHLT

        if idx == 4:
            break
        elif idx == 3:
            if (jet.pt > 10.): # && eta
                pass_probe_cond = True
                probe_jet_pt = jet.pt
                if jet.pxHLT != -9999:
                    pass_probe_match = True

        else:
            if (jet.pt > 70.) & (jet.pxHLT != -9999):
                pass_tag_cond.append(True)
            else:
                pass_tag_cond.append(False)

    #print "Tag", pass_tag_cond
    #print "Probe", pass_probe_cond
    #print "Match", pass_probe_match

    if (sum(pass_tag_cond) == 3) & (pass_probe_cond == True):
        pass_trigger = True
        #print "pass trigger"
    else:
        pass_trigger = False
        #print( "fail trigger")

    return pass_trigger, pass_probe_match, probe_jet_pt


def trigger_tau(jets, tau, tausHLT, filterStatus, filterStatus2):

    pass_tag_cond = False
    pass_probe_cond = False
    probe_tau_pt = -9999

    deltaRMin = 99.
    deltaR =99.
    njet = 0
    for idx, jet in enumerate(jets):

        if (jet.pt > 10.) & (abs(jet.eta) < 2.4) & (jet.pxHLT != -9999):

            jet_vec = ROOT.TLorentzVector(jet.px, jet.py, jet.pz, jet.e)
            jetHLT_vec = ROOT.TLorentzVector(jet.pxHLT, jet.pyHLT, jet.pzHLT, jet.eHLT)

            if (jet_vec.DeltaR(jetHLT_vec)<0.4):

                njet += 1

                tau_vec = ROOT.TLorentzVector(tau.px, tau.py, tau.pz, tau.e)
                deltaR = jet_vec.DeltaR(tau_vec)
                if(deltaR < deltaRMin):
                    deltaRMin = deltaR



    #print njet, filterStatus, deltaRMin

    if (njet>=4) & (deltaRMin<0.4) & (filterStatus==1):

        pass_tag_cond = True
        probe_tau_pt = tau.pt

        tau_vec = ROOT.TLorentzVector(tau.px, tau.py, tau.pz, tau.e)

        deltaRMinTrig = 99.
        deltaRTrig =99.

        tauHLT_best = 0
        #idx = -9999
        for tauHLT in tausHLT:

            tauHLT_vec = ROOT.TLorentzVector(tauHLT.px, tauHLT.py, tauHLT.pz, tauHLT.e)
            deltaRTrig = tau_vec.DeltaR(tauHLT_vec)
            #print deltaRTrig
            if deltaRTrig<deltaRMinTrig:
                deltaRMinTrig = deltaRTrig;
                tauHLT_best = tauHLT_vec
                #print tauHLT_best

        #print tauHLT_best

        #print deltaRMinTrig, filterStatus, deltaRMin, deltaRMinTrig, filterStatus2

        if (filterStatus2==1) & (deltaRMinTrig<0.4):
            if (abs(tauHLT_best.Eta())<2.3):
                pass_probe_cond = True

    return pass_tag_cond, pass_probe_cond, probe_tau_pt



ff = ROOT.TFile('/eos/user/l/llayer/opendata_files/Run2011A_MultiJet/Run2011A_MultiJet_100.root')
tree_in = ff.Get("MyModule/Events")

jet4_ref = ROOT.TH1F("jet4_ref","jet4_ref", 19,10,200)
jet4_trig = ROOT.TH1F("jet4_trig","jet4_trig", 19,10,200)
#x2bins = np.array([0,5,10,15,20,25,30,35,40,45,50,60,70,80,90,100])
x2bins = np.array([0,5,10,15,20,25,30,32.5,35,37.5,40,42.5,45,47.5,50,60,70,80,90])
tau_ref = ROOT.TH1F("tau_ref","tau_ref", 18, x2bins)
tau_trig = ROOT.TH1F("tau_trig","tau_trig", 18, x2bins)

for counter, evt in enumerate(tree_in):

    if counter%1000 == 0:
        print counter
        #break

    # Trigger
    if (int(evt.run)<160431) | (int(evt.run)>178420): continue
    if ((int(evt.run) >= 165970) & (int(evt.run)<=166782)) | (int(evt.run)>=171050):
        trigger = "45"
    else:
        trigger = "40"

    """
    #print evt.run, trigger
    # Filter
    if trigger == "45":
        filterStatus  = evt.HLTFilter_hltQuadJet45IsoPFTau45
        filterStatus20 = evt.HLTFilter_hltFilterPFTauTrack5TightIsoL1QuadJet20CentralPFTau45
        filterStatus28 = evt.HLTFilter_hltFilterPFTauTrack5TightIsoL1QuadJet28CentralPFTau45
        filterStatus2 = (filterStatus28 == 1) | (filterStatus20 == 1)
    else:
        filterStatus  = evt.HLTFilter_hltQuadJet40IsoPFTau40
        filterStatus2 = evt.HLTFilter_hltFilterPFTauTrack5TightIsoL1QuadJet20CentralPFTau40
    #print filterStatus, filterStatus20
    passTrigger40 = evt.HLT_QuadJet40_IsoPFTau40
    passTrigger45 = evt.HLT_QuadJet45_IsoPFTau45
    passTriggerMu15 = evt.HLT_Mu15_v
    passTriggerMu20 = evt.HLT_Mu20_v
    passTriggerMu24 = evt.HLT_Mu24_v
    passTriggerMu30 = evt.HLT_Mu30_v
    passTriggerMu15_v1 = evt.HLT_Mu15_v1
    passTriggerMu15_v2 = evt.HLT_Mu15_v2
    passTriggerMu15_v3 = evt.HLT_Mu15_v3
    passTriggerMu15_v4 = evt.HLT_Mu15_v4
    passTriggerMu15_v5 = evt.HLT_Mu15_v5
    passTriggerMu20_v1 = evt.HLT_Mu20_v1
    passTriggerMu20_v2 = evt.HLT_Mu20_v2
    passTriggerMu20_v3 = evt.HLT_Mu20_v3
    passTriggerMu20_v4 = evt.HLT_Mu20_v4
    passTriggerMu20_v5 = evt.HLT_Mu20_v5
    passTriggerMu24_v1 = evt.HLT_Mu24_v1
    passTriggerMu24_v2 = evt.HLT_Mu24_v2
    passTriggerMu24_v3 = evt.HLT_Mu24_v3
    passTriggerMu24_v4 = evt.HLT_Mu24_v4
    passTriggerMu24_v5 = evt.HLT_Mu24_v5
    passTriggerMu30_v1 = evt.HLT_Mu30_v1
    passTriggerMu30_v2 = evt.HLT_Mu30_v2
    passTriggerMu30_v3 = evt.HLT_Mu30_v3
    passTriggerMu30_v4 = evt.HLT_Mu30_v4
    passTriggerMu30_v5 = evt.HLT_Mu30_v5

    pass_mu = passTriggerMu15_v1 | passTriggerMu15_v2 | passTriggerMu15_v3 | passTriggerMu15_v4 | passTriggerMu15_v5 | \
              passTriggerMu20_v1 | passTriggerMu20_v2 | passTriggerMu20_v3 | passTriggerMu20_v4 | passTriggerMu20_v5 | \
              passTriggerMu24_v1 | passTriggerMu24_v2 | passTriggerMu24_v3 | passTriggerMu24_v4 | passTriggerMu24_v5 | \
              passTriggerMu30_v1 | passTriggerMu30_v2 | passTriggerMu30_v3 | passTriggerMu30_v4 | passTriggerMu30_v5
    #pass_mu = passTriggerMu15 | passTriggerMu20 | passTriggerMu24 | passTriggerMu30
    """



    # Object selections
    muons = select_muons(evt)
    electrons = select_electrons(evt)
    taus = select_taus(evt)
    taus = clean_taus(taus, muons, electrons)
    tauHLTs = select_tauHLT(evt)
    jets = select_jets(evt, trigger = trigger)

    """
    print
    print "Trigger 40", passTrigger40, "Trigger 45", passTrigger45
    print "Muons", len(muons)
    print "Electrons", len(electrons)
    print "Taus", len(taus)
    print "Jets", len(jets)
    """

    prescales = [
    evt.Prescale_HLT_QuadJet40_IsoPFTau40_v1,evt.Prescale_HLT_QuadJet40_IsoPFTau40_v2,evt.Prescale_HLT_QuadJet40_IsoPFTau40_v3,
    evt.Prescale_HLT_QuadJet40_IsoPFTau40_v4,evt.Prescale_HLT_QuadJet40_IsoPFTau40_v5,evt.Prescale_HLT_QuadJet40_IsoPFTau40_v6,
    evt.Prescale_HLT_QuadJet40_IsoPFTau40_v7,evt.Prescale_HLT_QuadJet40_IsoPFTau40_v8,evt.Prescale_HLT_QuadJet40_IsoPFTau40_v9,
    evt.Prescale_HLT_QuadJet40_IsoPFTau40_v10,evt.Prescale_HLT_QuadJet40_IsoPFTau40_v11,evt.Prescale_HLT_QuadJet40_IsoPFTau40_v12,
    evt.Prescale_HLT_QuadJet40_IsoPFTau40_v13,evt.Prescale_HLT_QuadJet40_IsoPFTau40_v14,evt.Prescale_HLT_QuadJet40_IsoPFTau40_v15
    ]

    prescale = 0
    for ps in prescales:
        if ps != -9999:
            prescale = ps

    if int(prescale) != 1:
        continue

    if len(jets) < 4:
        continue

    if len(taus) < 1:
        continue

    #if pass_mu == False:
    #    continue

    #if evt.nTau < 1:
        #rint "no tau"
    #    continue

    #print evt.run, trigger
    #for j in jets:
    #    print j.pxHLT, j.pxHLT != -9999


    # Jet trigger
    pass_trigger, pass_probe_match, probe_jet_pt = trigger_jets(jets)

    if pass_trigger:
        #print "Pt probe", probe_jet_pt
        jet4_ref.Fill(probe_jet_pt)
    if pass_trigger & pass_probe_match:
        #print "Pt probe & match", probe_jet_pt
        jet4_trig.Fill(probe_jet_pt)


    # Check!!
    if len(taus) != 1:
        continue

    #print taus

    # Filter
    if trigger == "45":
        filterStatus  = evt.HLTFilter_hltQuadJet45IsoPFTau45
        filterStatus20 = evt.HLTFilter_hltFilterPFTauTrack5TightIsoL1QuadJet20CentralPFTau45
        filterStatus28 = evt.HLTFilter_hltFilterPFTauTrack5TightIsoL1QuadJet28CentralPFTau45
        filterStatus2 = (filterStatus28 == 1) | (filterStatus20 == 1)
    else:
        filterStatus  = evt.HLTFilter_hltQuadJet40IsoPFTau40
        filterStatus2 = evt.HLTFilter_hltFilterPFTauTrack5TightIsoL1QuadJet20CentralPFTau40

    #print filterStatus, filterStatus2

    pass_tag_cond, pass_probe_cond, probe_tau_pt = trigger_tau(jets, taus[0], tauHLTs, filterStatus, filterStatus2)

    if pass_tag_cond:
        #print "Pt probe", probe_jet_pt
        print probe_tau_pt
        tau_ref.Fill(probe_tau_pt)
    if pass_tag_cond & pass_probe_cond:
        #print "Pt probe & match", probe_jet_pt
        print probe_tau_pt
        tau_trig.Fill(probe_tau_pt)



#jet4_eff = ROOT.TH1F("jet4_eff","jet-leg efficiency vs pT", 19,10,200)
#jet4_eff.Sumw2()
#jet4_eff.Divide(jet4_trig,jet4_ref,1,1,"B")

# Write to file
f = ROOT.TFile( trigger + "_trigger_eff.root", "recreate")
f.cd()
jet4_ref.Write()
jet4_trig.Write()
tau_ref.Write()
tau_trig.Write()
#jet4_eff.Write()
f.Close()
