import ROOT
from collections import namedtuple
import numpy as np
from array import array
import copy


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


def select_taus(evt, eta_cut = 2.3, pt_cut = 10., vtxmatch_cut = 1., dxy_cut=0.04, leadTrackPt_cut=5., trigger="45"):

    Tau = namedtuple('Tau', 'eta pt px py pz e pxHLT pyHLT pzHLT eHLT charge')
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
                                  evt.Tau_pyHLT45[iTau], evt.Tau_pzHLT45[iTau], evt.Tau_eHLT45[iTau], evt.Tau_charge[iTau]))
        else:
            good_taus.append(Tau( evt.Tau_eta[iTau], evt.Tau_pt[iTau], evt.Tau_px[iTau], evt.Tau_py[iTau],
                          evt.Tau_pz[iTau], evt.Tau_e[iTau], evt.Tau_pxHLT40[iTau],
                          evt.Tau_pyHLT40[iTau], evt.Tau_pzHLT40[iTau], evt.Tau_eHLT40[iTau], evt.Tau_charge[iTau]))

    return good_taus


def select_jets(evt, eta_cut = 2.5, pt_cut = 10., trigger="45"):

    Jet = namedtuple('Jet', 'eta pt px py pz e pxHLT pyHLT pzHLT eHLT csvDisc tcDisc flavour')
    good_jets = []

    for iJet in range(evt.nJet):


        #print evt.Jet_csvDisc[iJet], evt.Jet_tcDisc[iJet]


        # Eta cut
        if abs(evt.Jet_eta[iJet]) > eta_cut: continue
        # Pt cut
        if abs(evt.Jet_pt[iJet]) < pt_cut: continue

        if trigger == "45":
            good_jets.append(Jet( evt.Jet_eta[iJet], evt.Jet_pt[iJet], evt.Jet_px[iJet], evt.Jet_py[iJet],
                                  evt.Jet_pz[iJet], evt.Jet_e[iJet], evt.Jet_pxHLT45[iJet],
                                  evt.Jet_pyHLT45[iJet], evt.Jet_pzHLT45[iJet], evt.Jet_eHLT45[iJet],
                                  evt.Jet_csvDisc[iJet], evt.Jet_tcDisc[iJet], evt.Jet_flavour[iJet]))
        else:
            good_jets.append(Jet( evt.Jet_eta[iJet], evt.Jet_pt[iJet], evt.Jet_px[iJet], evt.Jet_py[iJet],
                          evt.Jet_pz[iJet], evt.Jet_e[iJet], evt.Jet_pxHLT40[iJet],
                          evt.Jet_pyHLT40[iJet], evt.Jet_pzHLT40[iJet], evt.Jet_eHLT40[iJet],
                          evt.Jet_csvDisc[iJet], evt.Jet_tcDisc[iJet], evt.Jet_flavour[iJet]))
        #print "HLT", evt.Jet_pxHLT40[iJet], evt.Jet_pxHLT45[iJet]

    return good_jets


def select_met(evt):

    MET = namedtuple('MET', 'px py pz e')

    return MET(evt.MET_px, evt.MET_py, evt.MET_pz, evt.MET_e)



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



def clean_jets(jets, taus, muons, electrons, R=0.4):

    good_jets = []
    for jet in jets:

        deltaRmu = 10000
        deltaRel = 10000

        jet_4vec = ROOT.TLorentzVector(jet.px, jet.py, jet.pz, jet.e)

        for mu in muons:

            mu_4vec = ROOT.TLorentzVector(mu.px, mu.py, mu.pz, mu.e)
            deltaR = jet_4vec.DeltaR(mu_4vec)
            if(deltaR < deltaRmu):
                deltaRmu = deltaR

        for el in electrons:

            el_4vec = ROOT.TLorentzVector(el.px, el.py, el.pz, el.e)
            deltaR = jet_4vec.DeltaR(el_4vec)
            if(deltaR < deltaRel):
                deltaRel = deltaR

        for tau in taus:

            tau_4vec = ROOT.TLorentzVector(tau.px, tau.py, tau.pz, tau.e)
            deltaR = jet_4vec.DeltaR(tau_4vec)
            if(deltaR < deltaRel):
                deltaRel = deltaR

        if ( deltaRmu > R)  & (deltaRel > R):
            good_jets.append(jet)

    return good_jets



def select_tauHLT( evt ):

    TauHLT = namedtuple('TauHLT', 'pt px py pz e')

    good_tauHLTs = []
    for iTauHLT in range(evt.nTauHLT):
        good_tauHLTs.append( TauHLT(evt.TauHLT_pt[iTauHLT], evt.TauHLT_px[iTauHLT], evt.TauHLT_py[iTauHLT],
                                    evt.TauHLT_pz[iTauHLT], evt.TauHLT_e[iTauHLT]) )

    return good_tauHLTs


def select_jetHLT( evt ):

    JetHLT = namedtuple('JetHLT', 'pt px py pz e')

    good_jetHLTs = []
    for iJetHLT in range(evt.nJetHLT):
        good_jetHLTs.append( JetHLT(evt.JetHLT_pt[iJetHLT], evt.JetHLT_px[iJetHLT], evt.JetHLT_py[iJetHLT],
                                    evt.JetHLT_pz[iJetHLT], evt.TauHLT_e[iJetHLT]) )

    return good_tauHLTs


def jet_match(jet, jetHLTs):

    jet_4vec = ROOT.TLorentzVector(jet.px, jet.py, jet.pz, jet.e)
    deltaRMinJetTrig, deltaRJetTrig = 99., 99.
    for jetHLT in jetHLTs:
        jetHLT_vec = ROOT.TLorentzVector(jetHLT.px, jetHLT.py, jetHLT.pz, jetHLT.e)
        deltaRJetTrig = jet_4vec.DeltaR(jetHLT_vec)
        if deltaRJetTrig< deltaRMinJetTrig:
            deltaRMinJetTrig = deltaRJetTrig
    if(deltaRMinJetTrig<0.4):
        isMatched = True
    else:
        isMatched = False
    return isMatched


def tau_match(tau, tauHLTs):

    isMatched = tau_match(tau, tauHLTs)
    tau_vec = ROOT.TLorentzVector(tau.px, tau.py, tau.pz, tau.e)
    deltaRMinTauTrig, deltaRTauTrig = 99., 99.;
    for tauHLT in tauHLTs:
        tauHLT_vec = ROOT.TLorentzVector(tauHLT.px, tauHLT.py, tauHLT.pz, tauHLT.e)
        deltaRTauTrig = tau_vec.DeltaR(tauHLT_vec)
        if(deltaRTauTrig<deltaRMinTauTrig):
             deltaRMinTauTrig = deltaRTauTrig
    if deltaRMinTauTrig<0.4:
        isMatched = True
    else:
        isMatched = False
    return isMatched


         
