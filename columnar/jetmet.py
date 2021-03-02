import numpy as np
import ROOT
from coffea.lookup_tools import extractor
from coffea.analysis_objects import JaggedCandidateArray
import uproot_methods
from copy import deepcopy

ext = extractor()
ext.add_weight_sets([
    "* * data/Legacy11_V1_DATA_UncertaintySources_AK5PFchs.junc.txt",
    "* * jer_sf.root"
])
ext.finalize()

evaluator = ext.make_evaluator()


def corr_met(met, initial_p4, new_p4):
    
    # MET
    met_p4 = uproot_methods.classes.TLorentzVector.TLorentzVectorArray(met["px"], met["py"],met["pz"], met["e"])
    #met["pt"] = met_p4.pt
    met["met"] = met_p4.Et
    #met["px"] = met_p4.px
    #met["py"] = met_p4.py
    #met["pz"] = met_p4.pz
    #met["e"] = met_p4.e
    
    # set MET values
    new_x = met_p4.x - (initial_p4.x - new_p4.x).sum()
    new_y = met_p4.y - (initial_p4.y - new_p4.y).sum()
    new_met_p4 = uproot_methods.TLorentzVectorArray.from_ptetaphim(
                                                                np.sqrt(new_x**2 + new_y**2), 0,
                                                                    np.arctan2(new_y, new_x), 0 )
    """
    met_p4.content._contents = uproot_methods.TLorentzVectorArray.from_ptetaphim(
        np.sqrt(new_x**2 + new_y**2), 0,
        np.arctan2(new_y, new_x), 0
    ).content
    """
    met["met"] = new_met_p4.Et
    met["pt"] = new_met_p4.pt
    met["e"] = new_met_p4.E
    met["px"] = new_met_p4.x
    met["py"] = new_met_p4.y
    met["pz"] = new_met_p4.z
    
    return met
    
    """
    jets_sin = np.sin(jet['p4'].phi)
    jets_cos = np.cos(jet['p4'].phi)
    for name, _ in juncs:
        for shift in ['up', 'down']:
            px = met['p4'].x - (initial_p4.x - jet['pt_{0}_{1}'.format(name, shift)] * jets_cos).sum()
            py = met['p4'].y - (initial_p4.y - jet['pt_{0}_{1}'.format(name, shift)] * jets_sin).sum()
            met['pt_{0}_{1}'.format(name, shift)] = np.sqrt(px**2 + py**2)
            met['phi_{0}_{1}'.format(name, shift)] = np.arctan2(py, px)
     """
    
    return


def transform(jets, met=None, corrLevel = "cent", doJER = True, jer=0.1, forceStochastic=False):
    
    # Order of transformations:
    # 1. JEC SF already applied in PAT
    # 2. JER SF 
    # 3. JER Up/Down
    # 4. JEC Up/Down
        
    jet = JaggedCandidateArray.candidatesfromcounts(
        jets.counts,
        pt=jets.pt.flatten(),
        eta=jets.eta.flatten(),
        phi=jets.phi.flatten(),
        mass=jets.mass.flatten()
    )

    initial_p4 = jet['p4'].copy()  # keep a copy for fixing met
    genpt = np.sqrt( jets.genpx**2 + jets.genpy**2)
    
    # JEC SF is already applied in PAT
    juncs = evaluator['Legacy11_V1_DATA_UncertaintySources_AK5PFchs_Total'](abs(jet.__fast_eta), jet.__fast_pt)

    # if there's a jer and sf to apply we have to update the momentum too
    # right now only use stochastic smearing
    if doJER:
        
        jersf_cent = evaluator["jer_cent"](abs(jet.__fast_eta))
        jersf_up = evaluator["jer_up"](abs(jet.__fast_eta))
        jersf_down = evaluator["jer_down"](abs(jet.__fast_eta))

        jersmear = jer * np.random.normal(size=jersf_cent.content.size)

        ptGenJet = np.zeros_like(jet.pt.content) if forceStochastic else genpt.content # jet gen pt

        doHybrid = ptGenJet > 0
        
        jsmear_cen = np.where(doHybrid,
                              1 + (jersf_cent.content - 1.) * (jet.pt.content - ptGenJet) / jet.pt.content,
                              1. + np.sqrt(np.maximum(jersf_cent.content**2 - 1.0, 0)) * jersmear)

        jsmear_up = np.where(doHybrid,
                             1 + (jersf_up.content - 1.) * (jet.pt.content - ptGenJet) / jet.pt.content,
                             1. + np.sqrt(np.maximum(jersf_up.content**2 - 1.0, 0)) * jersmear)

        jsmear_down = np.where(doHybrid,
                               1 + (jersf_down.content - 1.) * (jet.pt.content - ptGenJet) / jet.pt.content,
                               1. + np.sqrt(np.maximum(jersf_down.content**2 - 1.0, 0)) * jersmear)

        """
        jsmear_cen = np.where(doHybrid,
                              1 + (jersf[:, 0] - 1) * (jet.pt.content - ptGenJet) / jet.pt.content,
                              1. + np.sqrt(np.maximum(jersf[:, 0]**2 - 1.0, 0)) * jersmear)

        jsmear_up = np.where(doHybrid,
                             1 + (jersf[:, 1] - 1) * (jet.pt.content - ptGenJet) / jet.pt.content,
                             1. + np.sqrt(np.maximum(jersf[:, 1]**2 - 1.0, 0)) * jersmear)

        jsmear_down = np.where(doHybrid,
                               1 + (jersf[:, -1] - 1) * (jet.pt.content - ptGenJet) / jet.pt.content,
                               1. + np.sqrt(np.maximum(jersf[:, -1]**2 - 1.0, 0)) * jersmear)

        """
        """
        # from PhysicsTools/PatUtils/interface/SmearedJetProducerT.h#L255-L264
        min_jet_pt = self.MIN_JET_ENERGY / np.cosh(jet.eta.content)
        min_jet_pt_corr = min_jet_pt / jet.pt.content
        jsmear_up = np.where(jsmear_up * jet.pt.content < min_jet_pt,
                             min_jet_pt_corr,
                             jsmear_up)
        jsmear_down = np.where(jsmear_down * jet.pt.content < min_jet_pt,
                               min_jet_pt_corr,
                               jsmear_down)
        jsmear_cen = np.where(jsmear_cen * jet.pt.content < min_jet_pt,
                              min_jet_pt_corr,
                              jsmear_cen)
        
        """
        # need to apply up and down jer-smear before applying central correction
        jet.add_attributes(pt_jer_up=jsmear_up * jet.pt.content,
                           mass_jer_up=jsmear_up * jet.mass.content,
                           pt_jer_down=jsmear_down * jet.pt.content,
                           mass_jer_down=jsmear_down * jet.mass.content)

        # finally, update the central value
        #_update_jet_ptm(jsmear_cen, jet)
        jet._content._contents['__fast_pt'] = jsmear_cen * jet.pt.content
        jet._content._contents['__fast_mass'] = jsmear_cen * jet.mass.content

    
        
    # have to apply central jersf before calculating junc
    jet.add_attributes(**{
        'pt_jes_up': juncs[:,:,0] * jet.pt,
        'mass_jes_up': juncs[:,:,0] * jet.mass,
        'pt_jes_down': juncs[:,:,1] * jet.pt,
        'mass_jes_down': juncs[:,:,1] * jet.mass
    })

    # hack to update the jet p4, we have the fully updated pt and mass here
    if corrLevel == "centJER":
        jet._content._contents['p4'] = uproot_methods.TLorentzVectorArray.from_ptetaphim(jet.pt.content,
                                                                          jet.eta.content,
                                                                          jet.phi.content,
                                                                          jet.mass.content)
    elif corrLevel == "jes_up":
        print("JESUP")
        jet._content._contents['p4'] = uproot_methods.TLorentzVectorArray.from_ptetaphim(jet.pt_jes_up.content,
                                                                      jet.eta.content,
                                                                      jet.phi.content,
                                                                      jet.mass_jes_up.content)  
    elif corrLevel == "jes_down":
        print("JESDOWN")
        jet._content._contents['p4'] = uproot_methods.TLorentzVectorArray.from_ptetaphim(jet.pt_jes_down.content,
                                                                      jet.eta.content,
                                                                      jet.phi.content,
                                                                      jet.mass_jes_down.content)     
        
    elif corrLevel == "jer_up":
        print("JERUP")
        jet._content._contents['p4'] = uproot_methods.TLorentzVectorArray.from_ptetaphim(jet.pt_jer_up.content,
                                                                      jet.eta.content,
                                                                      jet.phi.content,
                                                                      jet.mass_jer_up.content)  
        
    elif corrLevel == "jer_down":
        print("JERDOWN")
        jet._content._contents['p4'] = uproot_methods.TLorentzVectorArray.from_ptetaphim(jet.pt_jer_down.content,
                                                                      jet.eta.content,
                                                                      jet.phi.content,
                                                                      jet.mass_jer_down.content) 
        
    else: 
        print("No valid correction")
    
    #jet.add_attributes(**{
    #    'csvDisc': jets.csvDisc.content
    #})
    
    #print(jet["p4"][0].pt)
    
    jet["csvDisc"] = jets.csvDisc
    jet["flavour"] = jets.flavour
    jet["pt"] = jet["p4"].pt
    jet["px"] = jet["p4"].x
    jet["py"] = jet["p4"].y
    jet["pz"] = jet["p4"].z
    jet["e"] = jet["p4"].E
    jet["eta"] = jet["p4"].eta
    jet["mass"] = jet["p4"].mass
    jet["phi"] = jet["p4"].phi
    """
    jet._content._contents['__fast_pt'] = jet["p4"].eta
    jet._content._contents['__fast_mass'] = jet["p4"].mass
    jet._content._contents['__fast_phi'] = jet["p4"].mass
    jet._content._contents['__fast_eta'] = jet["p4"].phi
    """
    #print(jet[0].pt)
    #print(jet["pt"][0])
    
    if met is None:
        return jet
        
    met = corr_met(met, initial_p4, jet['p4'])
    return jet, met
    
    
    
def scale_tau(taus, met = None, corr = "tau_eup", percent=0.03):
    
    if corr == "tau_eup":
        sf = 1. + percent
    else:
        sf = 1. - percent
    
    tau = JaggedCandidateArray.candidatesfromcounts(
        taus.counts,
        pt=taus.pt.flatten(),
        eta=taus.eta.flatten(),
        phi=taus.phi.flatten(),
        mass=taus.mass.flatten()
    )
    
    initial_p4 = tau['p4'].copy()  # keep a copy for fixing met
    
    tau._content._contents['__fast_pt'] = sf * tau.pt.content
    tau._content._contents['__fast_mass'] = sf * tau.mass.content
    tau._content._contents['p4'] = uproot_methods.TLorentzVectorArray.from_ptetaphim(tau.pt.content,
                                                              tau.eta.content,
                                                              tau.phi.content,
                                                              tau.mass.content)  
    
    tau["pt"] = tau["p4"].pt
    tau["px"] = tau["p4"].x
    tau["py"] = tau["p4"].y
    tau["pz"] = tau["p4"].z
    tau["e"] = tau["p4"].E
    tau["eta"] = tau["p4"].eta
    tau["mass"] = tau["p4"].mass
    tau["phi"] = tau["p4"].phi
    tau["charge"] = taus["charge"]
    tau["byMediumCombinedIsolationDeltaBetaCorr"] = taus["byMediumCombinedIsolationDeltaBetaCorr"]
    tau["leadTrackPt"] = taus["leadTrackPt"]
    tau["dxy"] = taus["dxy"]
    tau["z"] = taus["z"]
    tau["againstMuonTight"] = taus["againstMuonTight"]
    tau["againstElectronTight"] = taus["againstElectronTight"]
    
    if met is None:
        return tau
    
    met = corr_met(met, initial_p4, tau['p4'])
    
    return tau, met

    
def make_jer_hists():
    
    bins = np.array([0.0, 0.5, 1.1, 1.7, 2.3, 5.0])
    cent = np.array([1.052, 1.057, 1.096, 1.134, 1.288])
    down = np.array([0.990, 1.001, 1.032, 1.042, 1.089])
    up = np.array([1.115, 1.114, 1.161, 1.228, 1.488])
    down_rel = down / cent
    up_rel = up/cent
    jer_cent = ROOT.TH1F("jer_cent","jer_cent", 5, bins)
    jer_down = ROOT.TH1F("jer_down","jer_down", 5, bins)
    jer_up = ROOT.TH1F("jer_up","jer_up", 5, bins)
    jer_down_rel = ROOT.TH1F("jer_down_rel","jer_down_rel", 5, bins)
    jer_up_rel = ROOT.TH1F("jer_up_rel","jer_up_rel", 5, bins)
    for i in range(len(jer_cent)-2):
        print (i)
        jer_cent.SetBinContent(i+1, cent[i])
        jer_down.SetBinContent(i+1, down[i])
        jer_up.SetBinContent(i+1, up[i])
        jer_down_rel.SetBinContent(i+1, down_rel[i])
        jer_up_rel.SetBinContent(i+1, up_rel[i])
    f = ROOT.TFile("jer_sf.root", "recreate")
    jer_cent.Write()
    jer_down.Write()
    jer_up.Write()
    jer_down_rel.Write()
    jer_up_rel.Write()
    f.Close()