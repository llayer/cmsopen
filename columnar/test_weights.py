import numpy as np
import pandas as pd
from coffea.lookup_tools import extractor
from coffea.btag_tools import BTagScaleFactor
from coffea.analysis_objects import JaggedCandidateArray

BTAG_WP = 0.679

np.random.seed(0)

ext_b = extractor()
ext_b.add_weight_sets([
    "* * data/beff_test.root"
])
ext_b.finalize()

ext_trig = extractor()
"""
ext_trig.add_weight_sets([
    "jet40_ * rootFilesTurnOn/TriggerEffHisto_data_match40_JETLEG.root",
    "jet45_ * rootFilesTurnOn/TriggerEffHisto_data_match45_JETLEG.root",
    "tau40_ * rootFilesTurnOn/TriggerEffHisto_match40_newTauID.root",
    "tau45_ * rootFilesTurnOn/TriggerEffHisto_match45_newTauID.root"
])
"""
ext_trig.add_weight_sets([
    "* * data/trigger_eff.root"
])

ext_trig.finalize()

evaluator_trig = ext_trig.make_evaluator()
evaluator_b = ext_b.make_evaluator()
btag_sf = BTagScaleFactor("data/CSV.csv", "medium", methods='comb,comb,comb')

def btag_weights(jets, isQCD = False):

    bjet = JaggedCandidateArray.candidatesfromcounts(
        jets.counts,
        pt=jets.pt.flatten(),
        eta=jets.eta.flatten(),
        phi=jets.phi.flatten(),
        mass=jets.mass.flatten()
    )
    
    bjet["csvDisc"] = jets.csvDisc
    
    pt_clipped = np.where(bjet.pt.content > 980., np.full((len(bjet.pt.content)), 980.), bjet.pt.content)
    
    if not isQCD:
        bjet["flavour"] = jets.flavour
        flavour = abs(bjet.flavour.content)
        light_mask = (flavour != 4) & (flavour != 5)
        light_id = np.zeros_like(flavour)
        flavour_sf = np.where(light_mask, light_id, flavour)
        flavour_eff = np.where(light_mask, light_id, flavour - 3)
        #flavour = abs(jets.flavour.content)
    else:
        flavour_eff = np.zeros_like(pt_clipped)
        flavour_sf = np.zeros_like(pt_clipped)
        
        
    b_eff = evaluator_b["eff"](pt_clipped, bjet.eta.content, flavour_eff)
    b_sf = btag_sf.eval("central", flavour_sf, abs(bjet.eta.content), pt_clipped, ignore_missing=True)
    b_sf_up = btag_sf.eval("up", flavour_sf, abs(bjet.eta.content), pt_clipped, ignore_missing=True)
    b_sf_down = btag_sf.eval("down", flavour_sf, abs(bjet.eta.content), pt_clipped, ignore_missing=True)
    
    btagged = bjet.csvDisc.content > BTAG_WP
    p_mc = np.where(btagged, b_eff, 1. - b_eff)
    p_data = np.where(btagged, b_eff * b_sf, 1.0 - b_sf * b_eff)
    p_data_up = np.where(btagged, b_eff * b_sf_up, 1.0 - b_sf_up * b_eff)
    p_data_down = np.where(btagged, b_eff * b_sf_down, 1.0 - b_sf_down * b_eff)
    
    bjet.add_attributes(flavour_sf = flavour_sf, flavour_eff=flavour_eff, b_eff = b_eff, b_sf = b_sf, 
                        b_sf_up = b_sf_up, b_sf_down = b_sf_down, btag_pmc = p_mc, btag_pdata = p_data, 
                        btag_pdata_up = p_data_up, btag_pdata_down = p_data_down)
    
    #print("eff", bjet.b_eff)
    
    if isQCD:
        weight2 = btag_weight_2(bjet)
        return weight2
    else:
        weight1 = btag_weight_1(bjet)
        weight2 = btag_weight_2(bjet)
        return pd.concat([weight1, weight2.set_index(weight1.index)], axis=1)

def btag_weight_1(jet):
    
    pmc = jet.btag_pmc.prod()
    pdata = jet.btag_pdata.prod()
    pdata_up = jet.btag_pdata_up.prod()
    pdata_down = jet.btag_pdata_down.prod()
    
    weights = pd.DataFrame({"btag_weight1" : pdata/pmc, "btag_weight1_up" : pdata_up/pmc, 
                            "btag_weight1_down" : pdata_down/pmc})
    weights = weights.fillna(1.)
    return weights


def btag_weight_2(jet):
    
    weight_0tags = (1 - jet.b_eff * jet.b_sf).prod()
    weight_0tags_up = (1 - jet.b_eff * jet.b_sf_up).prod()
    weight_0tags_down = (1 - jet.b_eff * jet.b_sf_down).prod()
    return pd.DataFrame({"btag_weight2" : 1. - weight_0tags, "btag_weight2_up" : 1. - weight_0tags_up, 
                         "btag_weight2_down" : 1. - weight_0tags_down})


def at_least_one_btag(jet):
    return (jet.csvDisc > BTAG_WP).sum()  > 0

def no_btag(jet):
    return (jet.csvDisc > BTAG_WP).sum()  < 1


def eval_weight(obj, key):
    
    weight = evaluator_trig[key](obj)
    if "error" in key:
        return weight
    else:
        mask = weight == 0.
        return np.where(mask, 1., weight)


def calc_weight(jet, tau, prefix, original_w = False):
    
    # DANGER how to treat overflow bin
    """
    jet0 = np.where(jet.pt[:,0] < 200., eval_weight(jet.pt[:,0], "jet" + prefix + "_jet4_eff" ), np.ones_like(len(jet.pt[:,0])))
    jet1 = np.where(jet.pt[:,1] < 200., eval_weight(jet.pt[:,1], "jet" + prefix + "_jet4_eff" ), np.ones_like(len(jet.pt[:,1])))
    jet2 = np.where(jet.pt[:,2] < 200., eval_weight(jet.pt[:,2], "jet" + prefix + "_jet4_eff" ), np.ones_like(len(jet.pt[:,2])))
    tau0 = np.where(tau.pt[:,0] < 100., eval_weight(tau.pt[:,0], "tau" + prefix + "_eff_tau" ), 
                    eval_weight(95., "tau" + prefix + "_eff_tau" ))
    """
    
    #last_bin = eval_weight(195., "jet" + prefix + "_jet4_eff" )
    if original_w:
        if prefix == "45":
            last_bin = np.full(len(jet), 0.9523809552192688)
        else:
            last_bin = eval_weight(195., "jet" + prefix + "_jet4_eff" )
        jet0 = np.where(jet.pt[:,0] < 200., eval_weight(jet.pt[:,0], "jet" + prefix + "_jet4_eff" ), last_bin)
        jet1 = np.where(jet.pt[:,1] < 200., eval_weight(jet.pt[:,1], "jet" + prefix + "_jet4_eff" ), last_bin)
        jet2 = np.where(jet.pt[:,2] < 200., eval_weight(jet.pt[:,2], "jet" + prefix + "_jet4_eff" ), last_bin)
        tau0 = np.where(tau.pt[:,0] < 100., eval_weight(tau.pt[:,0], "tau" + prefix + "_eff_tau" ),
                        eval_weight(95., "tau" + prefix + "_eff_tau" ))    
        jet0_err = eval_weight(jet.pt[:,0], "jet" + prefix + "_jet4_eff_error" )
        jet1_err = eval_weight(jet.pt[:,1], "jet" + prefix + "_jet4_eff_error" )
        jet2_err = eval_weight(jet.pt[:,2], "jet" + prefix + "_jet4_eff_error" )
        tau0_err = eval_weight(tau.pt[:,0], "tau" + prefix + "_eff_tau_error" )
    else:
        
        #if prefix == "45":
        #    last_bin = np.full(len(jet), 0.9523809552192688)
        #else:
        last_bin = eval_weight(195., "jet4_eff_" + prefix )
        jet0 = np.where(jet.pt[:,0] < 200., eval_weight(jet.pt[:,0], "jet4_eff_" + prefix ), last_bin)
        jet1 = np.where(jet.pt[:,1] < 200., eval_weight(jet.pt[:,1], "jet4_eff_" + prefix ), last_bin)
        jet2 = np.where(jet.pt[:,2] < 200., eval_weight(jet.pt[:,2], "jet4_eff_" + prefix ), last_bin)
        tau0 = np.where(tau.pt[:,0] < 100., eval_weight(tau.pt[:,0], "tau_eff_" + prefix ),
                        eval_weight(95., "tau_eff_" + prefix ))    
        jet0_err = eval_weight(jet.pt[:,0], "jet4_eff_" + prefix + "_error" )
        jet1_err = eval_weight(jet.pt[:,1], "jet4_eff_" + prefix + "_error" )
        jet2_err = eval_weight(jet.pt[:,2], "jet4_eff_" + prefix + "_error" )
        tau0_err = eval_weight(tau.pt[:,0], "tau_eff_" + prefix + "_error" )

    
    print(list(zip(jet.pt[:,0], jet0_err, jet1_err, jet2_err, tau0_err)))
    
    weight = 1. * jet0 * jet1 * jet2 * tau0
    weight_up = 1. * (jet0 + jet0_err) * (jet1 + jet1_err) * (jet2 + jet2_err) * (tau0 + tau0_err)
    weight_down = 1. * (jet0 - jet0_err) * (jet1 - jet1_err) * (jet2 - jet2_err) * (tau0 - tau0_err)
    
    #print(list(zip(jet.pt[:,0], jet0, jet1, jet2, tau0)))
    
    return weight, weight_up, weight_down
    
    
def trigger_weight(jet, tau, frac=0.218):
    
    # Randomly select 40/45 trigger according to lumi fraction
    rand = np.random.random((len(jet)))
    mask_is40 = rand < frac
    
    weight_40, weight_up_40, weight_down_40 = calc_weight(jet, tau, "40")
    weight_45, weight_up_45, weight_down_45 = calc_weight(jet, tau, "45")
    
    weight = np.where(mask_is40, weight_40, weight_45)
    weight_up = np.where(mask_is40, weight_up_40, weight_up_45)
    weight_down = np.where(mask_is40, weight_down_45, weight_down_45)
    
    #print(weight)
    
    #return np.array([weight, weight_up, weight_down])
    return pd.DataFrame({"trigger_weight": weight, "trigger_weight_up": weight_up, "trigger_weight_down": weight_down,
                         "trigger_is40" : mask_is40})



def lumi():

    # HLT_QuadJet45_IsoPFTau45 171050 178420
    hlt_45_171050_178420 = 2930.246598848
    # HLT_QuadJet45_IsoPFTau45 165970 166782
    hlt_45_165970_166782 = 363.488009033
    # HLT_QuadJet40_IsoPFTau40 160431 165969
    hlt_40_160431_165969 = 357.461657608
    # HLT_QuadJet40_IsoPFTau40 166783 171049
    hlt_40_166783_171049 = 514.717054398

    hlt_45 = hlt_45_171050_178420 + hlt_45_165970_166782
    hlt_40 = hlt_40_160431_165969 + hlt_40_166783_171049

    return hlt_40, hlt_45


def get_xsec(sample):
    
    xsec = {'WJetsToLNu': [31314, 1558, 1558],
        'TTJets': [164, 10., 10.], #[177.31, 0., 0.], #DANGER!! CHECK!!
        'DYJetsToLL': [3048, 132, 132],
        'T_TuneZ2_t-channel': [42.6, 2.4, 2.3],
        'Tbar_TuneZ2_t-channel': [22.0, 0.1, 0.8],
        'T_TuneZ2_s': [2.76, 0.11 ,0.10],
        'Tbar_TuneZ2_s': [1.52, 0.09, 0.08],
        'T_TuneZ2_tW': [7.87,  0.6,  0.6],
        'Tbar_TuneZ2_tW': [7.87,  0.6,  0.6]
    }
    return xsec[sample]


def norm(evt, counts, xsec, lumi):

    total_events = counts['nEventsTotal'].sum()
    norm = (xsec[0] * lumi) / total_events
    norm_up = ((xsec[0] + xsec[1]) * lumi) / total_events
    norm_down = ((xsec[0] - xsec[2]) * lumi) / total_events
    evt["norm"] = norm
    evt["norm_up"] = norm_up
    evt["norm_down"] = norm_down
    
    
def classify_tt(evt):
    
    tt_tau = evt["genEvent_tmeme"] == 10000
    
    tt_ee = (evt["genEvent_tmeme"] == 2) | (evt["genEvent_tmeme"] == 10101) | (evt["genEvent_tmeme"] == 20200)
    tt_mm = (evt["genEvent_tmeme"] == 20) | (evt["genEvent_tmeme"] == 11010) | (evt["genEvent_tmeme"] == 22000)
    tt_em = (evt["genEvent_tmeme"] == 11) | (evt["genEvent_tmeme"] == 11001) | \
            (evt["genEvent_tmeme"] == 10110) | (evt["genEvent_tmeme"] == 21100)
    
    tt_etauh = (evt["genEvent_tmeme"] == 10010) | (evt["genEvent_tmeme"] == 21000)
    tt_mtauh = (evt["genEvent_tmeme"] == 10001) | (evt["genEvent_tmeme"] == 20100)
    tt_lqq = (evt["genEvent_tmeme"] == 1) | (evt["genEvent_tmeme"] == 10) | \
             (evt["genEvent_tmeme"] == 10100) | (evt["genEvent_tmeme"] == 11000)
    
    tt_tauhtauh = (evt["genEvent_tmeme"] == 20000)
    tt_qqqq = (evt["genEvent_tmeme"] == 0)
    tt_ll = (evt["genEvent_tmeme"] == 2) | (evt["genEvent_tmeme"] == 10101) | \
            (evt["genEvent_tmeme"] == 20200) | (evt["genEvent_tmeme"] == 20) |\
            (evt["genEvent_tmeme"] == 11010) | (evt["genEvent_tmeme"] == 22000) | \
            (evt["genEvent_tmeme"] == 11) | (evt["genEvent_tmeme"] == 11001) | \
            (evt["genEvent_tmeme"] == 10110) | (evt["genEvent_tmeme"] == 21100)
    
    tt_bkg = tt_ee | tt_mm | tt_em | tt_etauh | tt_mtauh | tt_lqq | tt_tauhtauh | tt_qqqq | tt_ll
    
    evt["isSignal"] = tt_tau
    evt["isBkg"] = tt_bkg

