import ROOT
import pandas as pd
import numpy as np
import root_pandas
from root_numpy import fill_hist
import object_selection
import uproot_methods
#import selection


def runs_40(evt):

    pass_run_selection = ( evt["run"] < 165970 ) | ((evt["run"] > 166782) & (evt["run"]<171050))
    return pass_run_selection

def runs_45(evt):

    pass_run_selection = (( evt["run"] >= 165970) & ( evt["run"] <= 166782)) | (( evt["run"] >= 171050) & ( evt["run"] <178421))
    return pass_run_selection

def jet_trigger_requirement(jet, hlt40, pt_cut=70.):
    
    pt_jet0 = jet["pt"][:,0] > pt_cut
    pt_jet1 = jet["pt"][:,1] > pt_cut
    pt_jet2 = jet["pt"][:,2] > pt_cut
    if hlt40:
        col = "pxHLT40"
    else:
        col = "pxHLT45"
    hlt_jet0 = jet[col][:,0] != -9999
    hlt_jet1 = jet[col][:,1] != -9999
    hlt_jet2 = jet[col][:,2] != -9999  
    
    return pt_jet0 & pt_jet1 & pt_jet2 & hlt_jet0 & hlt_jet1 & hlt_jet2


def jet_trigger_selection(file_path, hlt40=False):
    
    print("sample", file_path, "isHLT40", hlt40) 
    
    # Load file
    f = uproot.open(file_path)
    events = f['Events']
        
    interestingTriggers = [
        "HLT_QuadJet40_IsoPFTau40", "HLT_QuadJet45_IsoPFTau45", "HLT_Mu15_v", "HLT_Mu20_v", "HLT_Mu24_v", "HLT_Mu30_v",
        "HLT_Mu15_v1", "HLT_Mu15_v2", "HLT_Mu15_v3", "HLT_Mu15_v4", "HLT_Mu15_v5",
        "HLT_Mu20_v1", "HLT_Mu20_v2", "HLT_Mu20_v3", "HLT_Mu20_v4", "HLT_Mu20_v5",
        "HLT_Mu24_v1", "HLT_Mu24_v2", "HLT_Mu24_v3", "HLT_Mu24_v4", "HLT_Mu24_v5",
        "HLT_Mu30_v1", "HLT_Mu30_v2", "HLT_Mu30_v3", "HLT_Mu30_v4", "HLT_Mu30_v5"
      ]
    event_vars = ["event", "run", "luminosityBlock"]
    event_vars += interestingTriggers
    evt = pd.DataFrame(object_selection.eventCollection(events, event_vars))
    jet_vars = ['pt', 'px', 'py', 'pz', 'eta', 'phi', 'mass', 'e', 'pxHLT40', 'pxHLT45']
    jet = object_selection.nanoObject(events, "Jet_", jet_vars)
    
    if hlt40:
        mask_trigger = runs_40(evt)
    else:
        mask_trigger = runs_45(evt)

    # Trigger selection
    jet = jet[mask_trigger]
    evt = evt[mask_trigger]  
    
    # Jet selection
    good_jet = object_selection.select_jet(jet, pt_cut = 10.)
    
    # Four jets
    mask_four_jet = four_jets(good_jet)
    good_jet = good_jet[mask_four_jet]
    evt = evt[mask_four_jet] 
    
    # Jet trigger requirement
    mask_jet = jet_trigger_requirement(good_jet, hlt40)
    good_jet = good_jet[mask_jet]
    evt = evt[mask_jet]  

    # Write to pandas
    evt["pt_tag"] = good_jet["pt"][:,3]
    if hlt40:
        col = "pxHLT40"
    else:
        col = "pxHLT45"
    evt["pxHLT_tag"] = good_jet[col][:,3]
    jet_out_vars = ['pt', col]
    df_jet = awkward.topandas(good_jet[jet_out_vars], flatten=False)
    df_jet = df_jet.add_prefix('Jet_')
    df = pd.concat([df_jet, evt.set_index(df_jet.index)], axis=1)
    to_np(df, "Jet_")

    return df


def fourjet_tag(jet, jetHLT_4vec, hlt40, deltaR=0.4):
    
    if hlt40:
        jet_hlt = jet.pxHLT40 != -9999
    else:
        jet_hlt = jet.pxHLT45 != -9999
    delta = jet["p4"].delta_r(jetHLT_4vec)
    mask = delta < deltaR
    return jet[jet_hlt & mask], delta


def match_tau_jets(tau, jets, deltaR = 0.4):
    
    cross = tau['p4'].cross(jets['p4'], nested=False)
    delta = cross.i0.delta_r(cross.i1)
    mask = (delta < deltaR).any()
    return mask, delta.min()


def best_hlt_match(tau, tau_hlt):
    
    cross = tau['p4'].cross(tau_hlt['p4'], nested=False)
    delta = cross.i0.delta_r(cross.i1)
    
    min_delta = delta.min() == delta
    return tau_hlt[ min_delta ], delta.min()


def tau_trigger_selection(file_path, hlt40=False):
    
    print("sample", file_path, "isHLT40", hlt40) 
    
    # Load file
    f = uproot.open(file_path)
    events = f['Events']
    
    event = {}
    
    # Load objects
    event_vars = ["event", "run", "luminosityBlock",
                  "HLTFilter_hltQuadJet45IsoPFTau45", "HLTFilter_hltFilterPFTauTrack5TightIsoL1QuadJet20CentralPFTau45",
                  "HLTFilter_hltFilterPFTauTrack5TightIsoL1QuadJet28CentralPFTau45",
                  "HLTFilter_hltQuadJet40IsoPFTau40", "HLTFilter_hltFilterPFTauTrack5TightIsoL1QuadJet20CentralPFTau40"]
    event["evt"] = pd.DataFrame(object_selection.eventCollection(events, event_vars))
 
    # Load objects
    event["vtx"] = pd.DataFrame(object_selection.nanoCollection(events, "PV_", ["z"]))
    jet_vars = ['pt', 'px', 'py', 'pz', 'eta', 'phi', 'mass', 'e', 'csvDisc']
    jet_vars += [ 'pxHLT40', 'pyHLT40', 'pzHLT40', 'eHLT40', 'pxHLT45', 'pyHLT45', 'pzHLT45', 'eHLT45' ]
        
    event["jet"] = object_selection.nanoObject(events, "Jet_", jet_vars)
    electron_vars = ['TrkIso03', 'ECaloIso03', 'cutbasedid', 'HCaloIso03', 'pt', 'eta', 'z']
    event["electron"] = object_selection.nanoObject(events, "Electron_", electron_vars)
    muon_vars = ['TrkIso03', 'ECaloIso03', 'isGlobalMuon', 'HCaloIso03', 'pt', 'eta', 'z']
    event["muon"] = object_selection.nanoObject(events, "Muon_", muon_vars)
    tau_vars = ['pt', 'px', 'py', 'pz', 'eta', 'phi', 'mass', 'e', 'charge', 'byMediumCombinedIsolationDeltaBetaCorr',
                'z', 'leadTrackPt', 'dxy', 'againstMuonTight', 'againstElectronTight']
    event["tau"] = object_selection.nanoObject(events, "Tau_", tau_vars) 
    met_vars = ['pt', 'px', 'py', 'pz', 'e']
    event["met"] = pd.DataFrame(nanoCollection(events, "MET_", met_vars))
    event["tau_hlt"] = object_selection.nanoObject(events, "TauHLT_", branches = [], from_cartesian=True)
    event["jet_hlt"] = object_selection.nanoObject(events, "JetHLT_", branches = [], from_cartesian=True)  
        
    print( "Total events", len(event["evt"]) )
    
    #
    # Object selections
    #
    event["muon"] = object_selection.select_muon(event["muon"], event["vtx"])
    event["electron"] = object_selection.select_electron(event["electron"], event["vtx"])
    event["tau"] = object_selection.select_tau(event["tau"], event["vtx"], pt_cut=10., leadTrackPt_cut=5.)
    event["jet"] = object_selection.select_jet(event["jet"], pt_cut=10.)
    
    #
    # Trigger selection
    #
    if hlt40:
        mask_trigger = runs_40(event["evt"])
    else:
        mask_trigger = runs_45(event["evt"])
    apply_mask(event, mask_trigger)
    
    print( "Trigger selection", len(event["evt"]) )
    
    #
    # Clean taus
    #
    event["tau"] = object_selection.clean_tau(event["tau"], event["muon"], event["electron"])
    
    #print(list(zip(good_jet.counts, good_clean_tau.counts, tau_hlt.counts)))
    
    #
    # At least four jets
    #
    mask_four_jet = four_jets(event["jet"])
    apply_mask(event, mask_four_jet)
    
    print( "Four jets", len(event["evt"]) ) 
    
    #
    # Exactly one tau
    #
    mask_one_tau = event["tau"].counts == 1
    apply_mask(event, mask_one_tau)
    print( "One tau", len(event["evt"]) ) 
    
    #
    # Conditions for trigger
    #
    if hlt40:
        filter1_mask = event["evt"]["HLTFilter_hltQuadJet40IsoPFTau40"] > 0
    else:
        filter1_mask = event["evt"]["HLTFilter_hltQuadJet45IsoPFTau45"] > 0
    apply_mask(event, filter1_mask)
    print( "Filter", len(event["evt"]) )
    
    
    #
    # Match four HLT jets
    #s
    if hlt40:
        jetHLT_4vec = uproot_methods.classes.TLorentzVector.TLorentzVectorArray.from_cartesian(event["jet"]["pxHLT40"], 
                                                                                           event["jet"]["pyHLT40"], 
                                                                                           event["jet"]["pzHLT40"], 
                                                                                           event["jet"]["eHLT40"])
    else:
        jetHLT_4vec = uproot_methods.classes.TLorentzVector.TLorentzVectorArray.from_cartesian(event["jet"]["pxHLT45"], 
                                                                                           event["jet"]["pyHLT45"], 
                                                                                           event["jet"]["pzHLT45"], 
                                                                                           event["jet"]["eHLT45"])
        
    good_matched_jet, delta_jet_hlt = fourjet_tag(event["jet"], jetHLT_4vec, hlt40, deltaR=0.4)
    event["evt"]["delta_jet_hlt"] = delta_jet_hlt
    
    # Multiplicities
    mask_four_jets = event["jet"].counts > 3
    apply_mask(event, mask_four_jets)
        
    print( "Four jet matches", len(event["evt"]) )
    
    #
    # Tau - Jet match
    #
    tau_match_mask, delta_tau_jet = match_tau_jets(event["tau"], event["jet"], deltaR = 0.4)
    event["evt"]["delta_tau_jet"] = delta_tau_jet
    apply_mask(event, tau_match_mask)
            
    print( "HLT match", len(event["evt"]) )
    
    """
    mask_hlt_taus = tau_hlt.counts > 0
    mult_mask = mask_hlt_taus
        
    evt = evt[mult_mask] 
    good_matched_jet = good_matched_jet[mult_mask] 
    good_clean_tau = good_clean_tau[mult_mask] 
    tau_hlt = tau_hlt[mult_mask] 
    
    print( "Multiplicity selection", len(evt) )
    """
    
    # Match best HLT tau
    tau_hlt_best, min_dR = best_hlt_match(event["tau"], event["tau_hlt"])
    
    # Write to pandas
    tau_hlt_best_pt = tau_hlt_best["p4"].pt
    tau_hlt_best_pt = tau_hlt_best_pt.pad(1)
    tau_hlt_best_pt = awkward.topandas(tau_hlt_best_pt, flatten=False)
    tau_hlt_best_pt = np.array(tau_hlt_best_pt)
    tau_hlt_best_eta = tau_hlt_best["p4"].eta
    tau_hlt_best_eta = tau_hlt_best_eta.pad(1)
    tau_hlt_best_eta = awkward.topandas(tau_hlt_best_eta, flatten=False)
    tau_hlt_best_eta = np.array(tau_hlt_best_eta)
    
    event["evt"]["tau_pt"] = event["tau"]["pt"][:,0]
    event["evt"]["tau_eta"] = event["tau"]["eta"][:,0]
    event["evt"]["hlt_dR"] = min_dR
    event["evt"]["tau_hlt_pt"] = tau_hlt_best_pt
    event["evt"]["tau_hlt_eta"] = tau_hlt_best_eta
 
    event["evt"].loc[:, 'tau_hlt_pt'] = event["evt"].tau_hlt_pt.map(lambda x: x[0])
    event["evt"].loc[:, 'tau_hlt_eta'] = event["evt"].tau_hlt_eta.map(lambda x: x[0])
    
    # Write to pandas
    jet_out_vars = ['pt', 'eta','pxHLT40', 'pxHLT45']
    df_jet = awkward.topandas(event["jet"][jet_out_vars], flatten=False)
    df_jet = df_jet.add_prefix('Jet_')
    df = pd.concat([df_jet, event["evt"].set_index(df_jet.index)], axis=1)
    to_np(df, "Jet_")

    return df
    #return evt, good_matched_jet, good_clean_tau, tau_hlt_best, min_dR



def selection_jet():

    runA_40 = selection.jet_trigger_selection("/eos/user/l/llayer/opendata_files/pretrigger_merged/Run2011A_SingleMu.root", 
                                              hlt40=True)
    runB_40 = selection.jet_trigger_selection("/eos/user/l/llayer/opendata_files/pretrigger_merged/Run2011B_SingleMu.root", 
                                              hlt40=True)
    runA_45 = selection.jet_trigger_selection("/eos/user/l/llayer/opendata_files/pretrigger_merged/Run2011A_SingleMu.root", 
                                              hlt40=False)
    runB_45 = selection.jet_trigger_selection("/eos/user/l/llayer/opendata_files/pretrigger_merged/Run2011B_SingleMu.root", 
                                              hlt40=False)
    runA_40.to_hdf("trigger/jet_RunA_40.h5" , "frame")
    runB_40.to_hdf("trigger/jet_RunB_40.h5" , "frame")
    runA_45.to_hdf("trigger/jet_RunA_45.h5" , "frame")
    runB_45.to_hdf("trigger/jet_RunB_45.h5" , "frame")

def selection_tau():
    
    runA_40 = selection.tau_trigger_selection("/eos/user/l/llayer/opendata_files/pretrigger_merged/Run2011A_MultiJet.root", 
                                              hlt40=True)
    runB_40 = selection.tau_trigger_selection("/eos/user/l/llayer/opendata_files/pretrigger_merged/Run2011B_MultiJet.root",
                                              hlt40=True)
    
    runA_45 = selection.tau_trigger_selection("/eos/user/l/llayer/opendata_files/pretrigger_merged/Run2011A_MultiJet.root", 
                                              hlt40=False)
    runB_45 = selection.tau_trigger_selection("/eos/user/l/llayer/opendata_files/pretrigger_merged/Run2011B_MultiJet.root",
                                              hlt40=False)
    
    runA_40.to_hdf("trigger/tau_RunA_40.h5" , "frame")
    runB_40.to_hdf("trigger/tau_RunB_40.h5" , "frame")
    runA_45.to_hdf("trigger/tau_RunA_45.h5" , "frame")
    runB_45.to_hdf("trigger/tau_RunB_45.h5" , "frame")


def trigger_info(df):
    
    trigger_names = ['HLT_Mu15_v','HLT_Mu20_v', 'HLT_Mu24_v', 'HLT_Mu30_v']
    trigger_v_names = ['HLT_Mu15_v1', 'HLT_Mu15_v2',
       'HLT_Mu15_v3', 'HLT_Mu15_v4', 'HLT_Mu15_v5', 'HLT_Mu20_v1',
       'HLT_Mu20_v2', 'HLT_Mu20_v3', 'HLT_Mu20_v4', 'HLT_Mu20_v5',
       'HLT_Mu24_v1', 'HLT_Mu24_v2', 'HLT_Mu24_v3', 'HLT_Mu24_v4',
       'HLT_Mu24_v5', 'HLT_Mu30_v1', 'HLT_Mu30_v2', 'HLT_Mu30_v3',
       'HLT_Mu30_v4', 'HLT_Mu30_v5']
    
    df["mu_fired"] = 0
    df["mu_v1to5_fired"] = 0
    
    for t in trigger_names:
        df["mu_fired"] += df[t]
        
    for t in trigger_v_names:
        df["mu_v1to5_fired"] += df[t]  
        
        
def load_jet_sel():
    
    runA_40 = pd.read_hdf("trigger/jet_RunA_40.h5" , "frame")
    runB_40 = pd.read_hdf("trigger/jet_RunB_40.h5" , "frame")
    runA_45 = pd.read_hdf("trigger/jet_RunA_45.h5" , "frame")
    runB_45 = pd.read_hdf("trigger/jet_RunB_45.h5" , "frame")
    
    jet_40 = pd.concat([runA_40, runB_40], axis=0)
    trigger_info(jet_40)
    jet_45 = pd.concat([runA_45, runB_45], axis=0)
    trigger_info(jet_45)
    
    return jet_40, jet_45


def load_tau_sel():
    
    runA_40 = pd.read_hdf("trigger/tau_RunA_40.h5" , "frame")
    runB_40 = pd.read_hdf("trigger/tau_RunB_40.h5" , "frame")
    runA_45 = pd.read_hdf("trigger/tau_RunA_45.h5" , "frame")
    runB_45 = pd.read_hdf("trigger/tau_RunB_45.h5" , "frame")
    
    tau_40 = pd.concat([runA_40, runB_40], axis=0)
    tau_45 = pd.concat([runA_45, runB_45], axis=0)
    
    return tau_40, tau_45


def eff_jet(jet, fired=0, name = "40"):

    if fired == 0:
        probe = jet[jet['mu_fired'] > 0]
    elif fired == 1:
        probe = jet[jet['mu_v1to5_fired'] > 0]
    else:
        probe = jet

    tag = probe[probe['pxHLT_tag'] != -9999]

    #denom, edges = np.histogram(trigger['jet_probe_pt'].values, bins=19, range=(10,200))
    #nom, edges = np.histogram(tag_trigger['jet_probe_pt'].values, bins=19, range=(10,200))

    denom = ROOT.TH1F("jet4_denom_" + name,"jet4_denom", 19,10,200)
    denom.Sumw2()
    fill_hist(denom, probe['pt_tag'].values)

    nom = ROOT.TH1F("jet4_nom_" + name,"jet4_nom", 19,10,200)
    nom.Sumw2()
    fill_hist(nom, tag['pt_tag'].values)

    eff = ROOT.TH1F("jet4_eff_" + name,"jet-leg efficiency vs pT", 19,10,200);
    eff.Sumw2()
    eff.Divide(nom,denom,1,1,"B")

    return eff


def eff_tau(tau, deltaR = 0.4, name = "40"):   #CHECK delta r

    print(len(tau))
    
    if name == "40":
        mask_filter = tau['HLTFilter_hltFilterPFTauTrack5TightIsoL1QuadJet20CentralPFTau40'] == 1
    else:
        mask20 = tau['HLTFilter_hltFilterPFTauTrack5TightIsoL1QuadJet20CentralPFTau45'] > 0
        mask28 = tau['HLTFilter_hltFilterPFTauTrack5TightIsoL1QuadJet28CentralPFTau45'] > 0
        mask_filter = mask20 | mask28
    
    
    mask_hlt_eta = abs(tau["tau_hlt_eta"]) < 2.3
    mask_hlt_deltaR = tau['hlt_dR'] < deltaR
    print ("hlt_dR", len(tau[mask_hlt_deltaR]))
    print ("Filter2", len(tau[mask_filter & mask_hlt_deltaR]))

    tag = tau[mask_filter & mask_hlt_deltaR & mask_hlt_eta]

    print( "taueta", len(tag) )
    
    #denom, edges = np.histogram(trigger['tau_probe_pt'].values, bins=15, range=(0,90))
    #nom, edges = np.histogram(tag_trigger['tau_probe_pt'].values, bins=15, range=(0,90))

    #bins = np.array( [0,5,10,15,20,25,30,35,40,45,50,60,70,80,90] )
    #bins = np.array([0,5,10,15,20,25,30,32.5,35,37.5,40,42.5,45,47.5,50,60,70,80,90])
    bins = np.array([0,5,10,15,20,25,30.5,35,40,45,50,60,70,80,90,100])


    denom = ROOT.TH1F("tau_denom_"  + name,"jet4_denom", 15, bins)
    denom.Sumw2()
    fill_hist(denom, tau['tau_pt'].values)

    nom = ROOT.TH1F("tau_nom_" + name,"tau_nom", 15, bins)
    nom.Sumw2()
    fill_hist(nom, tag['tau_pt'].values)

    eff = ROOT.TH1F("tau_eff_" + name,"tau-leg efficiency vs pT", 15, bins)
    eff.Sumw2()
    eff.Divide(nom,denom,1,1,"B")

    return eff


def eff():

    # Trigger efficiency for jets
    print( "Jet efficiencies" )
    jet_40, jet_45 = load_jet_sel()
    jet_eff_40 = eff_jet(jet_40, fired = 1, name = "40")
    jet_eff_45 = eff_jet(jet_45, fired = 1, name = "45")

    # Trigger efficiency for tau
    print( "Tau efficiencies" )
    tau_40, tau_45 = load_tau_sel()
    tau_eff_40 = eff_tau(tau_40, deltaR = 0.4, name = "40")
    tau_eff_45 = eff_tau(tau_45, deltaR = 0.4, name = "45")

    
    print( "Write output" )
    file = ROOT.TFile("trigger_eff.root", 'recreate')
    jet_eff_40.Write()
    jet_eff_45.Write()
    tau_eff_40.Write()
    tau_eff_45.Write()
    file.Close()
    
    
    
    
    

if __name__ == "__main__":
    
    #print( "Jet trigger event selection" )
    #selection_jet()
    print( "Tau trigger event selection" )
    selection_tau()
    #spass
    #eff()
