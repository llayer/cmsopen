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
    
    if name == "Data":
        pass
    elif name == "QCD":
        if var_name == "bdt":
            scale_qcd = 9.
            sample = sample[sample["train_flag"] == "test"]
        else:
            scale_qcd = 4.3
        sample['weight'] = sample['btag_weight'] * scale_qcd
    else:
        #samples[sample]['new_trigger_weight'] = new_samples[sample].apply(lambda ev : weights.trigger_weight(ev), axis=1)
        sample['weight'] = sample['norm'] * (1/1000) * sample['trigger_weight'] * sample['Jet_btag_weight1']
        #print(sample, sum(samples[sample]['weight']))
        #new_samples[sample]['btag_weight2']
    
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
    
    
def createRatio(h1, h2):
    
    h3 = h1.Clone("h3")
    h3.SetLineColor(ROOT.kBlack)
    h3.SetMarkerStyle(20)
    h3.SetMarkerSize(0.5)
    h3.SetTitle("")
    h3.SetMinimum(0.5)
    h3.SetMaximum(1.5)
    # Set up plot for markers and errors
    h3.Sumw2()
    h3.SetStats(0)
    h3.Divide(h2)

    
    # Adjust y-axis settings
    y = h3.GetYaxis()
    y.SetTitle("Ratio")
    y.SetNdivisions(505)
    y.SetTitleSize(20)
    y.SetTitleFont(43)
    y.SetTitleOffset(1.55)
    y.SetLabelFont(43)
    y.SetLabelSize(15)
 
    # Adjust x-axis settings
    x = h3.GetXaxis()
    x.SetTitleSize(20)
    x.SetTitleFont(43)
    x.SetTitleOffset(4.)
    x.SetLabelFont(43)
    x.SetLabelSize(15)
    
    return h3

def createCanvasPads():

    c = ROOT.TCanvas("c", "canvas", 800, 800)
    # Upper histogram plot is pad1
    pad1 = ROOT.TPad("pad1", "pad1", 0, 0.51, 1, 1.0)
    pad1.SetBottomMargin(0)  # joins upper and lower plot
    #pad1.SetGridx()
    pad1.Draw()
    # Lower ratio plot is pad2
    c.cd()  # returns to main canvas before defining pad2
    pad2 = ROOT.TPad("pad2", "pad2", 0, 0.3, 1, 0.5)
    pad2.SetTopMargin(0)  # joins upper and lower plot
    pad2.SetBottomMargin(0.3)
    #pad2.SetGridx()
    pad2.Draw()

    return c, pad1, pad2
    
def plot_variation(var, xlabel, corr, cent, up, down):

    ROOT.gROOT.SetBatch()
    ROOT.gStyle.SetOptStat(0)
    
    ratio_up = createRatio(up, cent)
    ratio_down = createRatio(down, cent)
    
    c, pad1, pad2 = createCanvasPads()

    # draw everything
    pad1.cd()
    cent.Draw()
    up.Draw("SAME")
    up.SetLineColor(ROOT.kRed+2)
    down.Draw("SAME")
    down.SetLineColor(ROOT.kGreen+2)

    cent.SetTitle(var + " " + corr)
    print(var, xlabel)
    leg = ROOT.TLegend(0.6, 0.65, .8, .89);
    leg.AddEntry(cent, "central", "l");
    leg.AddEntry(up, "up", "l");
    leg.AddEntry(down, "down", "l");
    leg.SetBorderSize(0)
    leg.Draw("SAME")
    
    pad2.cd()
    ratio_up.Draw("ep")
    ratio_up.SetLineColor(ROOT.kRed+2)
    ratio_up.SetMarkerColor(ROOT.kRed+2)
    ratio_down.Draw("epSAME")    
    ratio_down.SetMarkerColor(ROOT.kGreen+2)
    ratio_down.SetLineColor(ROOT.kGreen+2)
    ratio_down.GetXaxis().SetTitle(xlabel)
    
    f1 = ROOT.TLine(0, 1., 1,1.)
    f1.SetLineColor(ROOT.kBlack)
    f1.SetLineStyle(ROOT.kDashed)
    f1.Draw("same")


    """
    c = ROOT.TCanvas()
    ROOT.gStyle.SetOptStat(0)
    cent.Draw()
    up.Draw("SAME")
    up.SetLineColor(ROOT.kRed+2)
    down.Draw("SAME")
    down.SetLineColor(ROOT.kGreen+2)

    cent.SetTitle(var + " " + corr)
    print(var, xlabel)
    cent.GetXaxis().SetTitle(xlabel)

    leg = ROOT.TLegend(0.6, 0.7, .89, .89);
    leg.AddEntry(cent, "central", "l");
    leg.AddEntry(up, "up", "l");
    leg.AddEntry(down, "down", "l");
    leg.SetBorderSize(0)
    leg.Draw("SAME")
    """
    c.Print("syst/" + var + "_" + corr + ".png")
    
    
def syst(variables, sample = "TTJets", file_name = "bdt_corr"):
    
    path = "histos/" + file_name + ".root"
    f = ROOT.TFile(path)
    
    
    for var in variables:
        print(sample + "_centJER_" + var["var_name"])
        hist_cent = f.Get(sample + "_centJER_" + var["var_name"])
        print(hist_cent.Integral())
        hist_jes_up = f.Get(sample + "_jes_up_" + var["var_name"])
        hist_jes_down = f.Get(sample + "_jes_down_" + var["var_name"])
        hist_jer_up = f.Get(sample + "_jer_up_" + var["var_name"])
        hist_jer_down = f.Get(sample + "_jer_down_" + var["var_name"])        
        hist_tau_eup = f.Get(sample + "_tau_eup_" + var["var_name"])
        hist_tau_edown = f.Get(sample + "_tau_edown_" + var["var_name"])  
        hist_jes_up_old = f.Get(sample + "_jes_up_old_" + var["var_name"])
        hist_jes_down_old = f.Get(sample + "_jes_down_old_" + var["var_name"])
        
        # JES
        plot_variation(var["var_name"], var["xtitle"], "JES", hist_cent, hist_jes_up, hist_jes_down)
        plot_variation(var["var_name"], var["xtitle"], "JER", hist_cent, hist_jer_up, hist_jes_down)
        plot_variation(var["var_name"], var["xtitle"], "Tau Scale", hist_cent, hist_tau_eup, hist_tau_edown)
        plot_variation(var["var_name"], var["xtitle"], "JES old", hist_cent, hist_jes_up_old, hist_jes_down_old)