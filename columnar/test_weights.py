import numpy as np
import pandas as pd
from coffea.lookup_tools import extractor
from coffea.btag_tools import BTagScaleFactor

np.random.seed(0)

ext_b = extractor()
ext_b.add_weight_sets([
    "* * data/beff_test.root"
])
ext_b.finalize()

ext_trig = extractor()
ext_trig.add_weight_sets([
    "jet40_ * rootFilesTurnOn/TriggerEffHisto_data_match40_JETLEG.root",
    "jet45_ * rootFilesTurnOn/TriggerEffHisto_data_match45_JETLEG.root",
    "tau40_ * rootFilesTurnOn/TriggerEffHisto_match40_newTauID.root",
    "tau45_ * rootFilesTurnOn/TriggerEffHisto_match45_newTauID.root"
])
ext_trig.finalize()

evaluator_trig = ext_trig.make_evaluator()
evaluator_b = ext_b.make_evaluator()
btag_sf = BTagScaleFactor("data/CSV.csv", "medium", methods='comb,comb,comb')

def btag_weights(jet):
    
    print("Test btag")
    flavour = abs(jet.flavour.content)
    light_mask = (flavour != 4) & (flavour != 5)
    light_id = np.zeros_like(flavour)
    flavour_sf = np.where(light_mask, light_id, flavour)
    flavour_eff = np.where(light_mask, light_id, flavour - 3)
    
    pt_clipped = np.where(jet.pt.content > 980., np.full((len(jet.pt.content)), 980.), jet.pt.content)
        
    b_eff = evaluator_b["eff"](pt_clipped, jet.eta.content, flavour_eff)
    b_sf = btag_sf.eval("central", flavour_sf, abs(jet.eta.content), pt_clipped, ignore_missing=True)
    b_sf_up = btag_sf.eval("up", flavour_sf, abs(jet.eta.content), pt_clipped, ignore_missing=True)
    b_sf_down = btag_sf.eval("down", flavour_sf, abs(jet.eta.content), pt_clipped, ignore_missing=True)
    
    # Method 1
    #pMC=pMC*eff
    #pData=pData*(sf*eff)
    #pMC=pMC*(1.0 - eff)
    #pData=pData*(1.0 - sf*eff)
    
    btagged = jet.csvDisc.content > 0.679
    p_mc = np.where(btagged, b_eff, 1. - b_eff)
    p_data = np.where(btagged, b_eff * b_sf, 1.0 - b_sf * b_eff)
    p_data_up = np.where(btagged, b_eff * b_sf_up, 1.0 - b_sf_up * b_eff)
    p_data_down = np.where(btagged, b_eff * b_sf_down, 1.0 - b_sf_down * b_eff)
    
    jet.add_attributes(flavour_sf = flavour_sf, flavour_eff=flavour_eff, b_eff = b_eff, b_sf = b_sf,
                       btag_pmc = p_mc, btag_pdata = p_data, btag_pdata_up = p_data_up, btag_pdata_down = p_data_down)
    
    return jet


def btag_weight_1(jet):
    
    pmc = jet.btag_pmc.prod()
    pdata = jet.btag_pdata.prod()
    
    return pdata/pmc


def btag_weight_2(jet):
    
    weight_0tags = (1 - jet.b_eff * jet.b_sf).prod()
    #print(weight_0tags)
    return 1. - weight_0tags


def count_btags(jet):
    return (jet.csvDisc > 0.679).sum()


def eval_weight(obj, key):
    
    weight = evaluator_trig[key](obj)
    mask = weight == 0.
    return np.where(mask, 1., weight)


def calc_weight(jet, tau, prefix):
    
    jet0 = eval_weight(jet.pt[:,0], "jet" + prefix + "_jet4_eff" )
    jet1 = eval_weight(jet.pt[:,1], "jet" + prefix + "_jet4_eff" )
    jet2 = eval_weight(jet.pt[:,2], "jet" + prefix + "_jet4_eff" )
    tau0 = eval_weight(tau.pt[:,0], "tau" + prefix + "_eff_tau" )
    
    jet0_err = eval_weight(jet.pt[:,0], "jet" + prefix + "_jet4_eff_error" )
    jet1_err = eval_weight(jet.pt[:,1], "jet" + prefix + "_jet4_eff_error" )
    jet2_err = eval_weight(jet.pt[:,2], "jet" + prefix + "_jet4_eff_error" )
    tau0_err = eval_weight(tau.pt[:,0], "tau" + prefix + "_eff_tau_error" )
    
    weight = 1. * jet0 * jet1 * jet2 * tau0
    weight_up = 1. * (jet0 + jet0_err) * (jet1 + jet1_err) * (jet2 + jet2_err) * (tau0 + tau0_err)
    weight_down = 1. * (jet0 - jet0_err) * (jet1 - jet1_err) * (jet2 - jet2_err) * (tau0 - tau0_err)
    
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
    
    print(weight)
    
    #return np.array([weight, weight_up, weight_down])
    return pd.DataFrame({"trigger_weight": weight, "trigger_weight_up": weight_up, "trigger_weight_down": weight_down})
