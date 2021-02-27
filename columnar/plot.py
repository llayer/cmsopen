import pandas as pd
import numpy as np
import ROOT
from root_numpy import fill_hist
import btag

def stack_weight(weight, n):
    return np.full(n ,weight)

def save_var(sample, name, var_name, bins = 20, xlow = 0., xup = 350):

    hist = ROOT.TH1D(name + "_" + var_name, name + "_" + var_name, bins, xlow, xup)
    hist.Sumw2()
    # Flatten if the column is a list
    if "Jet_" in var_name:
        series = sample[var_name].apply(pd.Series).stack().reset_index(drop=True)
        if name != "Data":
            sample['weight_stacked'] = sample.apply(lambda x : stack_weight(x["weight"] ,x["nJets"]), axis=1)
            weights = sample['weight_stacked'].apply(pd.Series).stack().reset_index(drop=True)
    else:
        series = sample[var_name]
        if name != "Data":
            weights = sample["weight"]
    if name == "Data":
        fill_hist(hist, series)
    else:
        #print (name, var_name)
        #print (len(series))
        #print (len(sample["weight"]))
        fill_hist(hist, series, weights = weights)
    #print s, hist.Integral()
    hist.Write()

def vars_to_histos(samples, variables, file_name = "histos"):
    file = ROOT.TFile("histos/" + file_name + ".root", 'recreate')
    for name, sample in samples.items():
        for var in variables:
            save_var(sample, name, var["var_name"], var["bins"], var["xlow"], var["xup"])

    file.Close()
    
    
def plot_btag_weights():
    
    
    file = ROOT.TFile("histos/btag.root", 'recreate')
    var = {"var_name" : "MET_met", "bins" : 30, "xlow" : 0., "xup" : 400}
    var_weight = {"var_name" : "weight", "bins" : 30, "xlow" : 0., "xup" : 2.}
    
    # TTJets btag
    tt = pd.read_hdf("samples/" + "TTJets" + ".h5")
    tt['nJets'] = tt["Jet_pt"].str.len()
    tt["Jet_nbtags"] = tt["Jet_csvDisc"].apply( lambda x : btag.count_btags(x, njets=-1) )
    tt = pd.concat([tt, tt.apply(lambda ev: pd.Series(btag.eval_sf_eff(ev)), axis=1)], axis=1)
    tt["Jet_btag_weight1"] = tt.apply(lambda ev : btag.b_weight_method1(ev, njets=-1), axis=1)
    tt["Jet_btag_weight2"] = tt.apply(lambda ev : btag.b_weight_method2(ev, njets=-1), axis=1)
    tt_at_least_1tag = btag.at_least_1tag(tt)
    # Unweighted
    tt_at_least_1tag["weight"] = 1.
    save_var(tt_at_least_1tag, "TTJets_1tag", var["var_name"], var["bins"], var["xlow"], var["xup"])
    # Weighted by event weight
    tt_at_least_1tag["weight"] = tt_at_least_1tag["Jet_btag_weight1"]
    save_var(tt_at_least_1tag, "TTJets_1tag_weighted", var["var_name"], var["bins"], var["xlow"], var["xup"])
    # Weighted by probabilistic weight
    tt["weight"] = tt["Jet_btag_weight2"]
    save_var(tt, "TTJets_proba_weight", var["var_name"], var["bins"], var["xlow"], var["xup"])
    # Weights
    save_var(tt_at_least_1tag, "Jet_btag_weight1", var_weight["var_name"], var_weight["bins"], var_weight["xlow"], var_weight["xup"])
    save_var(tt, "Jet_btag_weight2", var_weight["var_name"], var_weight["bins"], var_weight["xlow"], var_weight["xup"])

    
    # QCD Mistag
    var = {"var_name" : "MET_met", "bins" : 30, "xlow" : 0., "xup" : 300}
    data = pd.read_hdf("samples/" + "Run2011A_MultiJet" + ".h5")
    data['nJets'] = data["Jet_pt"].str.len()
    data["Jet_nbtags"] = data["Jet_csvDisc"].apply( lambda x : btag.count_btags(x, njets=-1) )
    qcd = btag.no_tag(data)
    def lf(nJets):
        return np.zeros((nJets))
    qcd["Jet_flavour"] = qcd["nJets"].apply(lf)
    qcd = pd.concat([qcd, qcd.apply(lambda ev: pd.Series(btag.eval_sf_eff(ev)), axis=1)], axis=1)
    qcd["btag_weight"] = qcd.apply(lambda ev : btag.b_weight_method2(ev, njets=-1), axis=1)
    # Unweighted
    qcd["weight"] = 1.
    save_var(qcd, "QCD_unweighted", var["var_name"], var["bins"], var["xlow"], var["xup"])
    # Weighted by mistag prog
    qcd["weight"] = qcd["btag_weight"]
    save_var(qcd, "QCD_mistag_weight", var["var_name"], var["bins"], var["xlow"], var["xup"])
    # Weights
    save_var(qcd, "QCD_mistag_weight", var_weight["var_name"], var_weight["bins"], var_weight["xlow"], var_weight["xup"])

    file.Close()    