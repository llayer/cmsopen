from coffea.nanoaod import NanoEvents
import awkward
import uproot_methods
import uproot
import root_pandas
import pandas as pd
import numpy as np
import glob
from coffea.btag_tools import BTagScaleFactor


def nanoObject(tree, prefix, from_cartesian = False):
    branches = set(k.decode('ascii') for k in tree.keys() if k.decode('ascii')[:len(prefix)] == prefix)
    if from_cartesian:
        p4branches = [prefix + k for k in ['px', 'py', 'pz', 'e']]
        objp4 = uproot_methods.TLorentzVectorArray.from_cartesian(*[tree[b].array() for b in p4branches])
    else:
        p4branches = [prefix + k for k in ['pt', 'eta', 'phi', 'mass']]
        objp4 = uproot_methods.TLorentzVectorArray.from_ptetaphim(*[tree[b].array() for b in p4branches])
    branches = {k[len(prefix):]: tree[k].array() for k in branches}
    obj = awkward.JaggedArray.zip(p4=objp4, **branches)
    return obj


def nanoCollection(tree, prefix):
    branches = set(k.decode('ascii') for k in tree.keys() if k.decode('ascii')[:len(prefix)] == prefix)
    obj = {k[len(prefix):]: tree[k].array() for k in branches}
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
    vtx_cut = (muon["z"] - vtx["z"]) < vtxmatch_cut
    
    return muon[ eta_cut & muon_global_cut & pt_cut & reliso_cut & vtx_cut ]


def select_electron(electron, vtx, eta_cut = 2.5, pt_cut = 10., reliso_cut = 0.15, vtxmatch_cut = 1.):
    
    electron['rel_iso'] = reliso03(electron['TrkIso03'], electron['ECaloIso03'], electron['HCaloIso03'], electron['pt'])
    id_cut = electron["cutbasedid"] == 1
    eta_cut = abs(electron['eta']) < eta_cut
    pt_cut = electron['pt'] > pt_cut
    reliso_cut = electron['rel_iso'] < reliso_cut
    vtx_cut = (electron["z"] - vtx["z"]) < vtxmatch_cut
    
    return electron[ id_cut & eta_cut & pt_cut & reliso_cut & vtx_cut ]



def select_tau(tau, vtx, eta_cut = 2.3, pt_cut = 45., vtxmatch_cut = 1., dxy_cut=0.04, leadTrackPt_cut=10.):
    
    iso_cut = tau["byMediumCombinedIsolationDeltaBetaCorr"] == 1
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
    vtx_cut = (tau["z"] - vtx["z"]) < vtxmatch_cut
    dxy_cut = tau["dxy"] <= dxy_cut
    
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


def runs_40(evt):

    pass_trigger_40 = evt["HLT_QuadJet40_IsoPFTau40"] == 1
    pass_run_selection = ( evt["run"] < 165970 ) | ((evt["run"] > 166782) & (evt["run"]<171050))
    return pass_trigger_40 & pass_run_selection

def runs_45(evt):

    pass_trigger_45 = evt["HLT_QuadJet45_IsoPFTau45"] == 1
    pass_run_selection = (( evt["run"] >= 165970) & ( evt["run"] <= 166782)) | (( evt["run"] >= 171050) & ( evt["run"] <178421))
    return pass_trigger_45 & pass_run_selection


def pass_trigger(evt):
    
    pass_40 = runs_40(evt)
    pass_45 = runs_45(evt)
    return pass_40 | pass_45


def lep_veto(muon, electron):
    
    no_muon = muon.counts == 0
    no_electron = electron.counts == 0    
    
    return no_muon & no_electron


def tau_requirement(tau):
    
    one_tau = tau.counts == 1
    
    return one_tau


def four_jets(jet):
    return jet.counts > 3


def jet_requirement(jet, pt_cut=45.):
    
    pt_jet0 = jet["pt"][:,0] > pt_cut
    pt_jet1 = jet["pt"][:,1] > pt_cut
    pt_jet2 = jet["pt"][:,2] > pt_cut
    
    return pt_jet0 & pt_jet1 & pt_jet2


def met_requirement(df, met_cut = 20.):
    
    return df[df['MET_met']>met_cut]


def hlt_requirement(tau_hlt, jet_hlt):
    
    tau_mask = tau_hlt.counts > 0
    jet_mask = jet_hlt.counts > 0
    
    return jet_mask & tau_mask


def hlt_match(obj, obj_hlt, deltaR = 0.5):
    
    cross = obj['p4'].cross(obj_hlt['p4'], nested=True)
    mask = (cross.i0.delta_r(cross.i1) < deltaR).any()
    return obj[mask]


def to_np(df, prefix):
    col_names = list(df)
    cols = list(k for k in col_names if k[:len(prefix)] == prefix)
    for col in cols:
        df[col] = np.array(df[col])


def event_selection(file_path = "TTJets/TTJets_FEC891EB-8CC6-E311-9BD5-002590A88802_3079.root", isData = False, isTT = False):
        
    in_file = file_path.split("/")[-1]
    print( "Processing:", in_file, "isData:", isData, "isTT:", isTT)
        
    # Load file
    f = uproot.open(file_path)
    events = f['Events']

    event_counts = {}
    event_counts["preselected"] = len(events)
    
    #arr = tt.arrays()
    
    # Load event vars
    event_vars = ["event", "run", "luminosityBlock", "HLT_QuadJet40_IsoPFTau40", "HLT_QuadJet45_IsoPFTau45"]
    if isTT:
        event_vars.append("genEvent_tmeme")
    evt = pd.DataFrame(eventCollection(events, event_vars))
    
    # Load objects
    vtx = nanoCollection(events, "PV_")
    jet = nanoObject(events, "Jet_")
    electron = nanoObject(events, "Electron_")
    muon = nanoObject(events, "Muon_")
    tau = nanoObject(events, "Tau_") 
    met = pd.DataFrame(nanoCollection(events, "MET_"))
    #if isData:
    tau_hlt = nanoObject(events, "TauHLT_", from_cartesian=True)
    jet_hlt = nanoObject(events, "JetHLT_", from_cartesian=True)

    #
    # Object selections
    #
    good_muon = select_muon(muon, vtx)
    good_electron = select_electron(electron, vtx)
    good_tau = select_tau(tau, vtx)
    good_jet = select_jet(jet)
    
    #print( list(zip(good_muon.counts, good_electron.counts, good_tau.counts, good_jet.counts)))
    
    
    # Clean jet
    good_clean_jet = clean_jet(good_jet, good_tau)
    
    #
    # Event selections
    #
    
    # Selections for real data
    if isData:
        
        # Trigger selection
        mask_trigger = pass_trigger(evt)
        good_tau = good_tau[mask_trigger]
        good_muon = good_muon[mask_trigger]
        good_electron = good_electron[mask_trigger]
        good_clean_jet = good_clean_jet[mask_trigger]
        jet_hlt = jet_hlt[mask_trigger]   
        tau_hlt = tau_hlt[mask_trigger]  
        met = met[mask_trigger]   
        evt = evt[mask_trigger]     
        event_counts["trigger"] = len(evt)
        
    # HLT matching

    hlt_mask = hlt_requirement(tau_hlt, jet_hlt)
    good_tau = good_tau[hlt_mask]
    good_muon = good_muon[hlt_mask]
    good_electron = good_electron[hlt_mask]
    good_clean_jet = good_clean_jet[hlt_mask]
    jet_hlt = jet_hlt[hlt_mask]   
    tau_hlt = tau_hlt[hlt_mask]  
    met = met[hlt_mask]   
    evt = evt[hlt_mask]  
    event_counts["hlt"] = len(evt)

    good_clean_jet = hlt_match(good_clean_jet, jet_hlt)
    good_tau = hlt_match(good_tau, tau_hlt)
        
    
    # Lepton veto
    mask_lep_veto = lep_veto(good_muon, good_electron)
    good_tau = good_tau[mask_lep_veto]
    good_clean_jet = good_clean_jet[mask_lep_veto]
    met = met[mask_lep_veto]
    evt = evt[mask_lep_veto]
    
    event_counts["lep_veto"] = len(evt)
    
    
    # Four jets
    mask_four_jet = four_jets(good_clean_jet)
    good_tau = good_tau[mask_four_jet]
    good_clean_jet = good_clean_jet[mask_four_jet]
    met = met[mask_four_jet]
    evt = evt[mask_four_jet]   
    
    # Jet requirement
    mask_jet = jet_requirement(good_clean_jet)
    good_tau = good_tau[mask_jet]
    good_clean_jet = good_clean_jet[mask_jet]
    met = met[mask_jet]
    evt = evt[mask_jet]
    
    event_counts["jet_requirement"] = len(evt)
    
    # Tau requirement
    mask_tau = tau_requirement(good_tau)
    good_tau = good_tau[mask_tau]
    good_clean_jet = good_clean_jet[mask_tau]
    met = met[mask_tau]
    evt = evt[mask_tau]
    
    # MET
    met_4vec = uproot_methods.classes.TLorentzVector.TLorentzVectorArray(met["px"], met["py"], met["pz"], met["e"])
    met["met"] = met_4vec.Et
    
    event_counts["tau_requirement"] = len(evt)
    
    # To pandas
    jet_out_vars = ['pt', 'px', 'py', 'pz', 'eta', 'phi', 'mass', 'e', 'csvDisc']
    if not isData:
        jet_out_vars.append( 'flavour' )
    tau_out_vars = ['pt', 'px', 'py', 'pz', 'eta', 'phi', 'mass', 'e', 'charge']
    met_out_vars = ['pt', 'px', 'py', 'pz', 'e', 'met']
    
    df_jet = awkward.topandas(good_clean_jet[jet_out_vars], flatten=False)
    df_jet = df_jet.add_prefix('Jet_')
    df_tau = awkward.topandas(good_tau[tau_out_vars], flatten=False)
    df_tau = df_tau.add_prefix('Tau_') 
    met = met[met_out_vars].add_prefix('MET_') 
    
    #df = pd.concat([sel2_evt, df_jet, df_tau], axis=1)
    df = pd.concat([df_jet, df_tau.set_index(df_jet.index), met.set_index(df_jet.index), evt.set_index(df_jet.index)], axis=1)

    to_np(df, "Jet_")
    to_np(df, "Tau_")
    
    return df, event_counts



