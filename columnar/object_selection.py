#from coffea.nanoaod import NanoEvents
import awkward
#import awkward1
import uproot_methods
import pandas as pd
import numpy as np
#from coffea.btag_tools import BTagScaleFactor


def nanoObject(tree, prefix, branches, from_cartesian = False):
    #branches = set(k.decode('ascii') for k in tree.keys() if k.decode('ascii')[:len(prefix)] == prefix)
    
    if from_cartesian:
        p4branches = [prefix + k for k in ['px', 'py', 'pz', 'e']]
        objp4 = uproot_methods.TLorentzVectorArray.from_cartesian(*[tree[b].array() for b in p4branches])
    else:
        p4branches = [prefix + k for k in ['pt', 'eta', 'phi', 'mass']]
        objp4 = uproot_methods.TLorentzVectorArray.from_ptetaphim(*[tree[b].array() for b in p4branches])
    branches = {k: tree[prefix + k].array() for k in branches}
    obj = awkward.JaggedArray.zip(p4=objp4, **branches)
    return obj


def nanoCollection(tree, prefix, branches):
    #branches = set(k.decode('ascii') for k in tree.keys() if k.decode('ascii')[:len(prefix)] == prefix)
    obj = {k: tree[prefix + k].array() for k in branches}
    return obj


def eventCollection(tree, branches):
    #branches = set(k.decode('ascii') for k in tree.keys() if k.decode('ascii') == prefix)
    obj = {k: tree[k].array() for k in branches}
    return obj


def reliso03(TrkIso03, ECaloIso03, HCaloIso03, pt):
    return (TrkIso03+ECaloIso03+HCaloIso03) / pt


def select_muon(muon, vtx, eta_cut = 2.5, pt_cut = 10., reliso_cut = 0.15, vtxmatch_cut = 1.):
    
    muon['rel_iso'] = reliso03(muon['TrkIso03'], muon['ECaloIso03'], muon['HCaloIso03'], muon['pt'])
    muon_global_cut = muon['isGlobalMuon'] > 0
    eta_cut = abs(muon['eta']) < eta_cut
    pt_cut = muon['pt'] > pt_cut
    reliso_cut = muon['rel_iso'] < reliso_cut
    vtx_cut = abs(muon["z"] - vtx["z"]) < vtxmatch_cut
    
    return muon[ eta_cut & muon_global_cut & pt_cut & reliso_cut & vtx_cut ]


def select_electron(electron, vtx, eta_cut = 2.5, pt_cut = 10., reliso_cut = 0.15, vtxmatch_cut = 1.):
    
    electron['rel_iso'] = reliso03(electron['TrkIso03'], electron['ECaloIso03'], electron['HCaloIso03'], electron['pt'])
    id_cut = electron["cutbasedid"] == 1
    eta_cut = abs(electron['eta']) < eta_cut
    pt_cut = electron['pt'] > pt_cut
    reliso_cut = electron['rel_iso'] < reliso_cut
    vtx_cut = abs(electron["z"] - vtx["z"]) < vtxmatch_cut
    
    return electron[ id_cut & eta_cut & pt_cut & reliso_cut & vtx_cut ]



def select_tau(tau, vtx, eta_cut = 2.3, pt_cut = 45., vtxmatch_cut = 1., dxy_cut=0.04, leadTrackPt_cut=10.):
    
    iso_cut = tau["byLooseCombinedIsolationDeltaBetaCorr"] == 1
    #iso_cut = tau["byMediumCombinedIsolationDeltaBetaCorr"] == 1
    nomuon_cut = tau["againstMuonTight"] == 1
    noele_cut = tau["againstElectronTight"] == 1
    # Lead track pt
    leadTrackPt_cut = tau["leadTrackPt"] > leadTrackPt_cut
    # Eta
    #if (abs(evt.Tau_eta[iTau])<1.566) & (abs(evt.Tau_eta[iTau])>1.4442): continue
    eta_cut = abs(tau['eta']) < eta_cut
    # Pt cut
    pt_cut = tau['pt'] > pt_cut
    # Vtx cut
    vtx_cut = abs(tau["z"] - vtx["z"]) < vtxmatch_cut
    dxy_cut = abs(tau["dxy"]) <= dxy_cut
    
    return tau[ iso_cut & nomuon_cut & noele_cut& leadTrackPt_cut & eta_cut & pt_cut & vtx_cut & dxy_cut]

    
def select_jet(jet, eta_cut = 2.4, pt_cut = 20.):
    
    # Eta cut
    eta_cut = abs(jet['eta']) < eta_cut
    # Pt cut
    pt_cut = jet['pt'] > pt_cut
    
    return jet[eta_cut & pt_cut]


def clean_jet(jet, tau, deltaR=0.4):
    
    jet_tau_cross = jet['p4'].cross(tau['p4'], nested=True)
    delta_jet_tau = (jet_tau_cross.i0.delta_r(jet_tau_cross.i1) > deltaR).all()
    return jet[delta_jet_tau]


def clean_tau(tau, muon, electron, deltaR=0.4):
    
    tau_mu_cross = tau['p4'].cross(muon['p4'], nested=True)
    delta_tau_mu = (tau_mu_cross.i0.delta_r(tau_mu_cross.i1) > deltaR).all()
    
    tau_ele_cross = tau['p4'].cross(electron['p4'], nested=True)
    delta_tau_ele = (tau_ele_cross.i0.delta_r(tau_ele_cross.i1) > deltaR).all()
    
    return tau[delta_tau_mu & delta_tau_ele]
