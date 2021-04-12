#from coffea.nanoaod import NanoEvents
import awkward
#import awkward1
import uproot_methods
import uproot
import root_pandas
import pandas as pd
import numpy as np
import glob
import jetmet
import test_weights
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


def trigger_40(evt):

    pass_trigger_40 = evt["HLT_QuadJet40_IsoPFTau40"] == 1
    pass_run_selection = ( evt["run"] < 165970 ) | ((evt["run"] > 166782) & (evt["run"]<171050))
    return pass_trigger_40 & pass_run_selection

def trigger_45(evt):

    pass_trigger_45 = evt["HLT_QuadJet45_IsoPFTau45"] == 1
    pass_run_selection = (( evt["run"] >= 165970) & ( evt["run"] <= 166782)) | (( evt["run"] >= 171050) & ( evt["run"] <178421))
    return pass_trigger_45 & pass_run_selection


def runs_40(evt):

    pass_run_selection = ( evt["run"] < 165970 ) | ((evt["run"] > 166782) & (evt["run"]<171050))
    return pass_run_selection

def runs_45(evt):

    pass_run_selection = (( evt["run"] >= 165970) & ( evt["run"] <= 166782)) | (( evt["run"] >= 171050) & ( evt["run"] <178421))
    return pass_run_selection

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

    """
    pt_jet0 = jet["pt"][:,0] > pt_cut
    pt_jet1 = jet["pt"][:,1] > pt_cut
    pt_jet2 = jet["pt"][:,2] > pt_cut
    
    return pt_jet0 & pt_jet1 & pt_jet2
    """


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
    event["evt"]["h"] = jet_tau_p4.E
    event["evt"]["ht"] = jet_tau_p4.Et
    event["evt"]["h_jet"] = event["jet"]["p4"].sum().E
    event["evt"]["ht_jet"] = event["jet"]["p4"].sum().Et
    event["evt"]["chargeEta"] = event["tau"]["charge"][:,0] * abs(event["tau"]["eta"])[:,0]
    event["evt"]['met'] = event["met"]["met"]
    event["evt"]['mTauJet'] = jet_tau_p4.mass
    met_p4 = uproot_methods.classes.TLorentzVector.TLorentzVectorArray(event["met"]["px"], event["met"]["py"],
                                                                       event["met"]["pz"], event["met"]["e"])
    event["evt"]['mt'] = np.sqrt(((event["tau"]["p4"][:,0].Et + met_p4.Et)**2) - 
                                   ((event["tau"]["p4"][:,0].x + met_p4.x)**2) -
                                   ((event["tau"]["p4"][:,0].y + met_p4.y)**2))
    
    
    # Variables based on momentum tensor
    vec3 = uproot_methods.TVector3Array.from_cartesian(event["jet"]["p4"].x, event["jet"]["p4"].y, event["jet"]["p4"].z)
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
    event["evt"] = pd.DataFrame(eventCollection(events, event_vars))
 
    # Load objects
    event["vtx"] = pd.DataFrame(nanoCollection(events, "PV_", ["z"]))
    jet_vars = ['pt', 'px', 'py', 'pz', 'eta', 'phi', 'mass', 'e', 'csvDisc']
    if not isData:
        jet_vars += [ 'flavour', 'genpx', 'genpy' ]
    event["jet"] = nanoObject(events, "Jet_", jet_vars)
    electron_vars = ['TrkIso03', 'ECaloIso03', 'cutbasedid', 'HCaloIso03', 'pt', 'eta', 'z']
    event["electron"] = nanoObject(events, "Electron_", electron_vars)
    muon_vars = ['TrkIso03', 'ECaloIso03', 'isGlobalMuon', 'HCaloIso03', 'pt', 'eta', 'z']
    event["muon"] = nanoObject(events, "Muon_", muon_vars)
    tau_vars = ['pt', 'px', 'py', 'pz', 'eta', 'phi', 'mass', 'e', 'charge', 'byMediumCombinedIsolationDeltaBetaCorr',
                'byLooseCombinedIsolationDeltaBetaCorr','z', 'leadTrackPt', 'dxy', 'againstMuonTight', 'againstElectronTight']
    event["tau"] = nanoObject(events, "Tau_", tau_vars) 
    met_vars = ['pt', 'px', 'py', 'pz', 'e']
    event["met"] = pd.DataFrame(nanoCollection(events, "MET_", met_vars))
    #event["met"]["met"] = event["met"]["p4"].Et
    #if isData:
    event["tau_hlt"] = nanoObject(events, "TauHLT_", branches = [], from_cartesian=True)
    event["jet_hlt"] = nanoObject(events, "JetHLT_", branches = [], from_cartesian=True)
        
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
    event["muon"] = select_muon(event["muon"], event["vtx"])
    event["electron"] = select_electron(event["electron"], event["vtx"])
    event["tau"] = select_tau(event["tau"], event["vtx"])
    event["jet"] = select_jet(event["jet"])    
    
    #print( list(zip(good_muon.counts, good_electron.counts, good_tau.counts, good_jet.counts)))
    
    # Clean jet
    event["jet"] = clean_jet(event["jet"], event["tau"])
    
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
    
    if not isData:
        # Normalization
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
            #test_weights.classify_tt(event["evt"])         
        
    # HL features
    hl_features(event)
    
    # To pandas
    #jet_out_vars = ['pt', 'px', 'py', 'pz', 'e', 'eta', 'phi', 'mass', 'csvDisc']
    #if not isData:
    #    jet_out_vars.append( 'flavour' )
    
    jet_out_vars = ['pt', 'e', 'eta', 'phi', 'mass']
    tau_out_vars = ['pt', 'e', 'eta', 'phi', 'mass']
    met_out_vars = ['met']
    
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
    evt = pd.DataFrame(eventCollection(events, event_vars))
    jet_vars = ['pt', 'px', 'py', 'pz', 'eta', 'phi', 'mass', 'e', 'pxHLT40', 'pxHLT45']
    jet = nanoObject(events, "Jet_", jet_vars)
    
    if hlt40:
        mask_trigger = runs_40(evt)
    else:
        mask_trigger = runs_45(evt)

    # Trigger selection
    jet = jet[mask_trigger]
    evt = evt[mask_trigger]  
    
    # Jet selection
    good_jet = select_jet(jet, pt_cut = 10.)
    
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


def clean_tau(tau, muon, electron, deltaR=0.4):
    
    tau_mu_cross = tau['p4'].cross(muon['p4'], nested=True)
    delta_tau_mu = (tau_mu_cross.i0.delta_r(tau_mu_cross.i1) > deltaR).all()
    
    tau_ele_cross = tau['p4'].cross(electron['p4'], nested=True)
    delta_tau_ele = (tau_ele_cross.i0.delta_r(tau_ele_cross.i1) > deltaR).all()
    
    return tau[delta_tau_mu & delta_tau_ele]



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
    event["evt"] = pd.DataFrame(eventCollection(events, event_vars))
 
    # Load objects
    event["vtx"] = pd.DataFrame(nanoCollection(events, "PV_", ["z"]))
    jet_vars = ['pt', 'px', 'py', 'pz', 'eta', 'phi', 'mass', 'e', 'csvDisc']
    jet_vars += [ 'pxHLT40', 'pyHLT40', 'pzHLT40', 'eHLT40', 'pxHLT45', 'pyHLT45', 'pzHLT45', 'eHLT45' ]
        
    event["jet"] = nanoObject(events, "Jet_", jet_vars)
    electron_vars = ['TrkIso03', 'ECaloIso03', 'cutbasedid', 'HCaloIso03', 'pt', 'eta', 'z']
    event["electron"] = nanoObject(events, "Electron_", electron_vars)
    muon_vars = ['TrkIso03', 'ECaloIso03', 'isGlobalMuon', 'HCaloIso03', 'pt', 'eta', 'z']
    event["muon"] = nanoObject(events, "Muon_", muon_vars)
    tau_vars = ['pt', 'px', 'py', 'pz', 'eta', 'phi', 'mass', 'e', 'charge', 'byMediumCombinedIsolationDeltaBetaCorr',
                'z', 'leadTrackPt', 'dxy', 'againstMuonTight', 'againstElectronTight']
    event["tau"] = nanoObject(events, "Tau_", tau_vars) 
    met_vars = ['pt', 'px', 'py', 'pz', 'e']
    event["met"] = pd.DataFrame(nanoCollection(events, "MET_", met_vars))
    event["tau_hlt"] = nanoObject(events, "TauHLT_", branches = [], from_cartesian=True)
    event["jet_hlt"] = nanoObject(events, "JetHLT_", branches = [], from_cartesian=True)  
        
    print( "Total events", len(event["evt"]) )
    
    #
    # Object selections
    #
    event["muon"] = select_muon(event["muon"], event["vtx"])
    event["electron"] = select_electron(event["electron"], event["vtx"])
    event["tau"] = select_tau(event["tau"], event["vtx"], pt_cut=10., leadTrackPt_cut=5.)
    event["jet"] = select_jet(event["jet"], pt_cut=10.)
    
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
    event["tau"] = clean_tau(event["tau"], event["muon"], event["electron"])
    
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
    
    
    
    
    
    
    
