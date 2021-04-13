import awkward
import uproot_methods
import uproot
import root_pandas
import pandas as pd
import numpy as np
import object_selection
import glob
import jetmet
import test_weights


def trigger_40(evt):

    pass_trigger_40 = evt["HLT_QuadJet40_IsoPFTau40"] == 1
    pass_run_selection = ( evt["run"] < 165970 ) | ((evt["run"] > 166782) & (evt["run"]<171050))
    return pass_trigger_40 & pass_run_selection

def trigger_45(evt):

    pass_trigger_45 = evt["HLT_QuadJet45_IsoPFTau45"] == 1
    pass_run_selection = (( evt["run"] >= 165970) & ( evt["run"] <= 166782)) | (( evt["run"] >= 171050) & ( evt["run"] <178421))
    return pass_trigger_45 & pass_run_selection


def pass_trigger(evt):
    
    pass_40 = trigger_40(evt)
    pass_45 = trigger_45(evt)
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
    
    return (jet["pt"] > pt_cut).sum() >= 3


def met_requirement(met, met_cut = 20.):
    
    return met > met_cut


def hlt_requirement(tau_hlt, jet_hlt):
    
    tau_mask = tau_hlt.counts > 0
    jet_mask = jet_hlt.counts > 0
    
    return jet_mask & tau_mask


def hlt_match(obj, obj_hlt, deltaR = 0.4):
    
    cross = obj['p4'].cross(obj_hlt['p4'], nested=True)
    mask = (cross.i0.delta_r(cross.i1) < deltaR).any()
    return obj[mask]


def to_np(df, prefix):
    col_names = list(df)
    cols = list(k for k in col_names if k[:len(prefix)] == prefix)
    for col in cols:
        df[col] = np.array(df[col])

        
def hl_features(event):
    
    # HL features
    jet_tau_p4 = event["jet"]["p4"].sum() + event["tau"]["p4"][:,0]
    event["evt"]["h"] = event["jet"]["p4"].E.sum() + event["tau"]["p4"][:,0].E
    event["evt"]["ht"] = event["jet"]["p4"].Et.sum() + event["tau"]["p4"][:,0].Et
    event["evt"]["h_jet"] = event["jet"]["p4"].E.sum()
    event["evt"]["ht_jet"] = event["jet"]["p4"].Et.sum()
    event["evt"]["chargeEta"] = event["tau"]["charge"][:,0] * abs(event["tau"]["eta"])[:,0]
    event["evt"]['met'] = event["met"]["met"]
    event["evt"]['mTauJet'] = jet_tau_p4.mass
    met_p4 = uproot_methods.classes.TLorentzVector.TLorentzVectorArray(event["met"]["px"], event["met"]["py"],
                                                                       event["met"]["pz"], event["met"]["e"])
    event["evt"]['mt'] = np.sqrt(((event["tau"]["p4"][:,0].Et + met_p4.Et)**2) - 
                                   ((event["tau"]["p4"][:,0].x + met_p4.x)**2) -
                                   ((event["tau"]["p4"][:,0].y + met_p4.y)**2))
    event["evt"]["deltaPhiTauMet"] = abs(event["tau"]["p4"][:,0].delta_phi(met_p4))
    
    
    # Variables based on momentum tensor
    vec3_jet = uproot_methods.TVector3Array.from_cartesian(event["jet"]["p4"].x, event["jet"]["p4"].y, event["jet"]["p4"].z)
    vec3_tau = uproot_methods.TVector3Array.from_cartesian(event["tau"]["p4"].x, event["tau"]["p4"].y, event["tau"]["p4"].z)
    vec3 = awkward.concatenate([vec3_jet , vec3_tau], axis=1)

    momentumTensor = np.zeros((len(vec3),3,3))
    momentumTensor[:,0,0] = (vec3.x * vec3.x).sum() 
    momentumTensor[:,0,1] = (vec3.x * vec3.y).sum() 
    momentumTensor[:,0,2] = (vec3.x * vec3.z).sum()
    momentumTensor[:,1,0] = (vec3.y * vec3.x).sum()
    momentumTensor[:,1,1] = (vec3.y * vec3.y).sum()
    momentumTensor[:,1,2] = (vec3.y * vec3.z).sum()
    momentumTensor[:,2,0] = (vec3.z * vec3.x).sum()
    momentumTensor[:,2,1] = (vec3.z * vec3.y).sum()
    momentumTensor[:,2,2] = (vec3.z * vec3.z).sum()
    norm = 1 + vec3.dot(vec3).sum()
    
    norm_reshaped = np.tile(norm,(9,1)).T.reshape((len(vec3),3,3))
    momentumTensor = momentumTensor / norm_reshaped
    eigenValues, eigenVectors = np.linalg.eig(momentumTensor)
    ev_sorted = np.sort(eigenValues, axis=1)
    event["evt"]['aplanarity'] = 1.5 * ev_sorted[:,0]
    event["evt"]['sphericity'] = 1.5 * (ev_sorted[:,0] + ev_sorted[:,1])        
    
       
def apply_mask(event, mask):
     
    event["evt"] = event["evt"][mask] 
    event["vtx"] = event["vtx"][mask]
    event["jet"] = event["jet"][mask]
    event["electron"] = event["electron"][mask]
    event["muon"] = event["muon"][mask]
    event["tau"] = event["tau"][mask]
    event["met"] = event["met"][mask]
    event["tau_hlt"] = event["tau_hlt"][mask]
    event["jet_hlt"] = event["jet_hlt"][mask]

    
def event_selection(file_path, isData = False, isTT = False, invert_btag = False, corrLevel = "cent", 
                    tau_factor = 0.03, jes_factor = 0.):
        
    sample = file_path.split("/")[-1][:-5]
    print( "Processing:", sample, "isData:", isData, "isTT:", isTT, "corrLevel", corrLevel)
        
    # Load file
    f = uproot.open(file_path)
    events = f['Events']

    event_counts = {}
    event_counts["preselected"] = len(events)
    
    event = {}
    
    # Load event vars
    event_vars = ["event", "run", "luminosityBlock", "HLT_QuadJet40_IsoPFTau40", "HLT_QuadJet45_IsoPFTau45", "PV_npvs" ]
    if isTT:
        event_vars.append("genEvent_tmeme")
    event["evt"] = pd.DataFrame(object_selection.eventCollection(events, event_vars))
 
    # Load objects
    event["vtx"] = pd.DataFrame(object_selection.nanoCollection(events, "PV_", ["z"]))
    jet_vars = ['pt', 'px', 'py', 'pz', 'eta', 'phi', 'mass', 'e', 'csvDisc']
    if not isData:
        jet_vars += [ 'flavour', 'genpx', 'genpy' ]
    event["jet"] = object_selection.nanoObject(events, "Jet_", jet_vars)
    electron_vars = ['TrkIso03', 'ECaloIso03', 'cutbasedid', 'HCaloIso03', 'pt', 'eta', 'z']
    event["electron"] = object_selection.nanoObject(events, "Electron_", electron_vars)
    muon_vars = ['TrkIso03', 'ECaloIso03', 'isGlobalMuon', 'HCaloIso03', 'pt', 'eta', 'z']
    event["muon"] = object_selection.nanoObject(events, "Muon_", muon_vars)
    tau_vars = ['pt', 'px', 'py', 'pz', 'eta', 'phi', 'mass', 'e', 'charge', 'byMediumCombinedIsolationDeltaBetaCorr',
                'byLooseCombinedIsolationDeltaBetaCorr','z', 'leadTrackPt', 'dxy', 'againstMuonTight', 'againstElectronTight']
    event["tau"] = object_selection.nanoObject(events, "Tau_", tau_vars) 
    met_vars = ['pt', 'px', 'py', 'pz', 'e']
    event["met"] = pd.DataFrame(object_selection.nanoCollection(events, "MET_", met_vars))
    #event["met"]["met"] = event["met"]["p4"].Et
    #if isData:
    event["tau_hlt"] = object_selection.nanoObject(events, "TauHLT_", branches = [], from_cartesian=True)
    event["jet_hlt"] = object_selection.nanoObject(events, "JetHLT_", branches = [], from_cartesian=True)
        
    #
    # JETMET
    #
    
    if (isData == False) & (corrLevel != "cent") :
        
        if "tau" in corrLevel:
            event["tau"],  event["met"] = jetmet.scale_tau(event["tau"], event["met"], corr = corrLevel, percent = tau_factor)
            event["jet"],  event["met"] = jetmet.transform(event["jet"], event["met"], corrLevel = "centJER")

        else:
            event["jet"],  event["met"] = jetmet.transform(event["jet"], event["met"], corrLevel = corrLevel, jes_factor = jes_factor)
        
            
        #Add btag weights
        #event["jet"] = test_weights.btag_weights(event["jet"])

        # Btag:
        """
        print("Test btag")
        flavour = abs(event["jet"].flavour.content)
        light_mask = (flavour != 4) & (flavour != 5)
        light_id = np.zeros_like(flavour)
        flavour_sf = np.where(light_mask, light_id, flavour)
        flavour_eff = np.where(light_mask, light_id, flavour - 3)
        event["jet"].add_attributes(flavour_sf = flavour_sf, flavour_eff=flavour_eff)
        """
    else:
        met_p4 = uproot_methods.classes.TLorentzVector.TLorentzVectorArray(event["met"]["px"], event["met"]["py"],
                                                                           event["met"]["pz"], event["met"]["e"])
        event["met"]["met"] = met_p4.Et
        
    
    #
    # Object selections
    #
    event["muon"] = object_selection.select_muon(event["muon"], event["vtx"])
    event["electron"] = object_selection.select_electron(event["electron"], event["vtx"])
    event["tau"] = object_selection.select_tau(event["tau"], event["vtx"])
    event["jet"] = object_selection.select_jet(event["jet"])    
    
    #print( list(zip(good_muon.counts, good_electron.counts, good_tau.counts, good_jet.counts)))
    
    # Clean jet
    event["jet"] = object_selection.clean_jet(event["jet"], event["tau"])
    
    #
    # Event selections
    #
    
    # Selections for real data
    if isData:
        
        # Trigger selection
        mask_trigger = pass_trigger(event["evt"])
        apply_mask(event, mask_trigger)
        event_counts["trigger"] = len(event["evt"])
        
    # HLT matching
    hlt_mask = hlt_requirement(event["tau_hlt"], event["jet_hlt"])
    apply_mask(event, hlt_mask)
    event_counts["hlt"] = len(event["evt"])

    event["jet"] = hlt_match(event["jet"], event["jet_hlt"])
    event["tau"] = hlt_match(event["tau"], event["tau_hlt"])
    
    # Four jets
    mask_four_jet = four_jets(event["jet"])
    apply_mask(event, mask_four_jet) 
    
    # Jet requirement
    mask_jet = jet_requirement(event["jet"])
    apply_mask(event, mask_jet) 
    event_counts["jet_requirement"] = len(event["evt"])
    
    # Tau requirement
    mask_tau = tau_requirement(event["tau"])
    apply_mask(event, mask_tau)
    event_counts["tau_requirement"] = len(event["evt"])
    
    # Normalization
    if not isData:
        
        hlt_40, hlt_45 = test_weights.lumi()
        total_lumi = hlt_40 + hlt_45
        trigger_frac = hlt_40 / float(hlt_45)
        counts_path = "/eos/user/l/llayer/opendata_files/preselection_merged/" + sample + "_counts.root"
        total_counts = root_pandas.read_root(counts_path)
        xsec = test_weights.get_xsec(sample)
        test_weights.norm(event["evt"], total_counts, xsec, total_lumi)

        
        # Trigger weights
        trigger_w = test_weights.trigger_weight(event["jet"], event["tau"])
        print(event["evt"].shape, trigger_w.shape)
        event["evt"] = pd.concat([event["evt"], trigger_w.set_index(event["evt"].index)], axis=1)
        #print(event["evt"]["trigger_weight"])
        
        if isTT:
            
            # PDF weights
            pdf = pd.read_hdf("TTJets_pdfweights.h5")
            df = pd.merge(event["evt"], pdf, how="left", on=["event", "luminosityBlock", "run"])
            
            # Classify signal and bkg
            test_weights.classify_tt(event["evt"])  
    
    # Lepton veto
    mask_lep_veto = lep_veto(event["muon"], event["electron"])
    apply_mask(event, mask_lep_veto)
    event_counts["lep_veto"] = len(event["evt"])
    
    # MET
    mask_met = met_requirement(event["met"]["met"])
    apply_mask(event, mask_met)
    event_counts["met"] = len(event["evt"])
    
    # B-tagging
    if isData and invert_btag:
        mask_btag = test_weights.no_btag(event["jet"])
        apply_mask(event, mask_btag)
        btag_weight = test_weights.btag_weights(event["jet"], isQCD=True)
        event["evt"] = pd.concat([event["evt"], btag_weight.set_index(event["evt"].index)], axis=1)
    else:
        mask_btag = test_weights.at_least_one_btag(event["jet"])
        apply_mask(event, mask_btag)
        # B-tagging weights
        if not isData:
            btag_weight = test_weights.btag_weights(event["jet"])
            #btag_1 = test_weights.btag_weight_1(event["jet"])
            #btag_2 = test_weights.btag_weight_2(event["jet"])
            event["evt"] = pd.concat([event["evt"], btag_weight.set_index(event["evt"].index)], axis=1)
                 
    event_counts["btag"] = len(event["evt"])       
        
    # HL features
    hl_features(event)
    
    # To pandas
    jet_out_vars = ['pt', 'px', 'py', 'pz', 'e', 'eta', 'phi', 'mass', 'csvDisc']
    if not isData:
        jet_out_vars.append( 'flavour' )
    
    #jet_out_vars = ['pt', 'e', 'eta', 'phi', 'mass']
    tau_out_vars = ['pt', 'px', 'py', 'pz', 'charge', 'e', 'eta', 'phi', 'mass']
    met_out_vars = ['pt', 'px', 'py', 'pz', 'e', 'met']
    
    df_jet = awkward.topandas(event["jet"][jet_out_vars], flatten=False)
    df_jet = df_jet.add_prefix('Jet_')
    df_tau = awkward.topandas(event["tau"][tau_out_vars], flatten=False)
    df_tau = df_tau.add_prefix('Tau_') 
    met = event["met"][met_out_vars].add_prefix('MET_') 

    #df = pd.concat([sel2_evt, df_jet, df_tau], axis=1)
    df = pd.concat([df_jet, df_tau.set_index(df_jet.index), met.set_index(df_jet.index), event["evt"].set_index(df_jet.index)], axis=1)

    to_np(df, "Jet_")
    to_np(df, "Tau_")
    
    return df, event["jet"], event["tau"], event_counts
    #return df, event["jet"], event_counts
    #return df, event_counts
    
    
    
    
    
    
    
