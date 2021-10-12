import pandas as pd
import numpy as np
import ROOT
from root_numpy import fill_hist
#import btag


def save_var(sample, name, var_name, bins = 20, xlow = 0., xup = 350, corr=None):

    print( name, var_name, corr )
    
    if corr is None:
        hist = ROOT.TH1D(name + "_" + var_name, name + "_" + var_name, bins, xlow, xup)
    else:
        hist = ROOT.TH1D(name + "_" + corr + "_" + var_name, name + "_" + var_name + "_" + corr, bins, xlow, xup)
    hist.Sumw2()
    
    if name == "Data":
        pass
    elif name == "QCD":
        scale_qcd = 9. * 0.73 * 1.1
        sample = sample[sample["train_flag"] == "test"]
        sample['weight'] = sample['btag_weight2'] * scale_qcd
    else:
        if corr is None:
            sample['weight'] = sample['norm'] * sample['trigger_weight'] * sample['btag_weight1']
        elif corr == "btag_up":
            sample['weight'] = sample['norm'] * sample['trigger_weight'] * sample['btag_weight1_up']
        elif corr == "btag_down":
            sample['weight'] = sample['norm'] * sample['trigger_weight'] * sample['btag_weight1_down']        
        if "TTJets_signal" in name:
            dummy = ROOT.TH1D("dummy", "dummy", bins, xlow, xup)
            fill_hist(dummy, sample[var_name], weights = sample['weight'])
            #sample = sample[sample["train_flag"] == "test"]
            sample = sample[sample["is_train"] == 0]
            
        
    series = sample[var_name]
    if name != "Data":
        weights = sample["weight"]
    if name == "Data":
        fill_hist(hist, series)
    else:
        fill_hist(hist, series, weights = weights)
        # Renorm the test templates used in the ml
        if ("TTJets_signal" in name):
            print("RENORM TEMPLATE", name)
            renorm_factor = float(dummy.Integral()) / float(hist.Integral())
            hist.Scale(renorm_factor)
            print(renorm_factor, hist.Integral(), dummy.Integral())
    #print s, hist.Integral()
    hist.Write()

def vars_to_histos(samples, variables, file_path, weight_syst=True):
    
    file = ROOT.TFile(file_path, 'recreate')
    for name, sample in samples.items():
        
        """
        # Change name of TTJets samples to be consistent
        if ("TTJets" in name) & (name is not "TTJets_bkg") & (name is not "TTJets_signal"):
            print("Change name")
            tmp = name.split("_")
            pre = tmp[0]
            post = tmp[-1]
            middle = '_'.join(tmp[1:-1])
            #print( name[0] + "_" + name[-1] + "_" + name[1] + "_" + name[2] )
            name = pre + "_"+ post + "_" + middle
            print(name)
        """
        
        for var in variables:
            if (("up" in var["var_name"]) | ("down" in var["var_name"])) & (name != "TTJets_signal"):
                continue
            else:
                print(var, name)
            save_var(sample, name, var["var_name"], var["bins"], var["xlow"], var["xup"])
            if weight_syst == True:
                for corr in ["btag_up", "btag_down"]:#, "trigger_up", "trigger_down", "xsec_up", "xsec_down"]:
                    save_var(sample, name, var["var_name"], var["bins"], var["xlow"], var["xup"], corr = corr)

            """
            if "norm" in var["var_name"]:
                if "bce" in var["var_name"]:
                    save_var(sample, name, var["var_name"], var["bins"], var["xlow"], var["xup"], norm_name=norm_name)
                else:
                    idx = var["var_name"].split("_")[-1]
                    norm_name = 
                    save_var(sample, name, var["var_name"], var["bins"], var["xlow"], var["xup"], norm_name=norm_name)
            else:
                save_var(sample, name, var["var_name"], var["bins"], var["xlow"], var["xup"])
            """
    file.Close()
    
    
    
def createRatio(h1, h2):
    
    h3 = h1.Clone("h3")
    h3.SetLineColor(ROOT.kBlack)
    h3.SetMarkerStyle(20)
    h3.SetMarkerSize(0.5)
    h3.SetTitle("")
    h3.SetMinimum(0.)
    h3.SetMaximum(2.)
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
    pad1 = ROOT.TPad("pad1", "pad1", 0, 0.5, 1, 1.0)
    pad1.SetBottomMargin(0.05)  # joins upper and lower plot
    #pad1.SetGridx()
    pad1.Draw()
    # Lower ratio plot is pad2
    c.cd()  # returns to main canvas before defining pad2
    pad2 = ROOT.TPad("pad2", "pad2", 0, 0.3, 1, 0.5)
    pad2.SetTopMargin(0.05)  # joins upper and lower plot
    pad2.SetBottomMargin(0.3)
    #pad2.SetGridx()
    pad2.Draw()

    return c, pad1, pad2
    
def plot_variation(outpath, var, xlabel, corr, cent, up, down, bce=False):

    ROOT.gROOT.SetBatch()
    ROOT.gStyle.SetOptStat(0)
    
    ratio_up = createRatio(up, cent)
    ratio_down = createRatio(down, cent)
    
    c, pad1, pad2 = createCanvasPads()
    #return c, pad1, pad2, ratio_up, ratio_down

    pad1.cd()
    cent.SetLineWidth(2)
    up.Draw()
    cent.Draw("SAME")
    up.SetLineColor(ROOT.kRed+2)
    up.SetLineWidth(2)
    up.SetTitle("")
    up.GetYaxis().SetLabelSize(0.05)
    up.GetXaxis().SetLabelSize(0.)
    up.GetYaxis().CenterTitle()
    up.GetYaxis().SetTitle("Events / bin")
    up.GetYaxis().SetTitleSize(0.07)
    up.GetYaxis().SetTitleOffset(0.7)
    
    if bce == False:
        maximum = up.GetMaximum()
        max_sf=1.5
        up.SetMaximum(maximum*max_sf)
        print("BCE", bce) 
    
    down.Draw("SAME")
    down.SetLineColor(ROOT.kGreen+2)
    down.SetLineWidth(2)

    #cent.SetTitle(var + " " + corr)
    #print(var, xlabel)
    leg = ROOT.TLegend(0.35, 0.65, .65, .89);
    leg.AddEntry(cent, "central", "l");
    leg.AddEntry(up, "up", "l");
    leg.AddEntry(down, "down", "l");
    leg.SetBorderSize(0)
    leg.SetTextSize(0.07)
    leg.Draw("SAME")

    pad2.cd()
    ratio_up.Draw("ep")
    ratio_up.GetXaxis().SetTitle(xlabel)
    ratio_up.SetLineColor(ROOT.kRed+2)
    ratio_up.SetLineWidth(2)
    ratio_up.GetYaxis().SetTitle("Ratio")
    ratio_up.GetYaxis().SetTitleSize(30)
    ratio_up.GetYaxis().CenterTitle()
    ratio_up.GetYaxis().SetTitleOffset(1.2)
    ratio_up.GetXaxis().SetLabelSize(20)
    ratio_up.GetXaxis().SetTitleSize(25)
    ratio_up.SetMarkerColor(ROOT.kRed+2)
    ratio_down.Draw("epSAME")    
    ratio_down.SetMarkerColor(ROOT.kGreen+2)
    ratio_down.SetLineWidth(2)
    ratio_down.SetLineColor(ROOT.kGreen+2)
    #ratio_up.GetXaxis().SetTitle(xlabel)
    #ratio_up.GetYaxis().SetTitleSize(0.05)
    #ratio_up.GetXaxis().SetLabelSize(0.1)
    ratio_up.GetYaxis().SetLabelSize(20)
    ratio_up.GetYaxis().SetNdivisions(504)

    x0 = ratio_up.GetXaxis().GetXmin()
    x1 = ratio_up.GetXaxis().GetXmax()
    f1 = ROOT.TLine(x0, 1., x1,1.)
    f1.SetLineColor(ROOT.kBlack)
    f1.SetLineStyle(ROOT.kDashed)
    f1.Draw("same")
    
    c.Print( outpath + "/" + var + "_" + corr + ".png")
    
    
def syst(variables, sample, file_path, outpath):
    
    
    f = ROOT.TFile(file_path, "READ")
       
    h = f.Get("TTJets_signal_centJER_btag_down_bdt")
    print(type(h))
    
    for var in variables:
        print(sample + "_" + var["var_name"])
        hist_cent = f.Get(sample + "_" + var["var_name"])
        
        for c in ["btag", "trigger", "xsec", "pdf", "met", "jer", "jes", "jes_old", "taue"]:
            hist_up = f.Get(sample + "_" + c + "_up_" + var["var_name"])
            print(type(hist_up))
            print(sample + "_" + c + "_up_" + var["var_name"])
            print(sample + "_" + c + "_down_" + var["var_name"])
            hist_down = f.Get(sample + "_" + c + "_down_" + var["var_name"])
            print(type(hist_down))
            plot_variation(outpath, var["var_name"], var["xtitle"], c, hist_cent, hist_up, hist_down)
            
            if c == "jes":
                for c_var in ["03", "06", "09"]:
                    
                    print(sample + "_" + c_var + "_" + c + "_up_" + var["var_name"])
                    hist_up = f.Get(sample + "_" + c_var + "_" + c + "_up_" + var["var_name"])
                    print(type(hist_up))
                    hist_down = f.Get(sample + "_" + c_var + "_" + c + "_down_" + var["var_name"])
                    plot_variation(outpath, var["var_name"], var["xtitle"], c + "_" + c_var, hist_cent, hist_up, hist_down)
        
            if c == "tau_e":
                for c_var in ["05", "07", "09"]:
                    
                    hist_up = f.Get(sample + "_" + c_var + "_" + c + "_up_" + var["var_name"])
                    hist_down = f.Get(sample + "_" + c_var + "_" + c + "_down_" + var["var_name"])
                    plot_variation(outpath, var["var_name"], var["xtitle"], c + "_" + c_var, hist_cent, hist_up, hist_down)
            
            
        """
        #print(hist_cent.Integral())
        for c in ["jes", "jer","met", "taue"]:
            
            #  Stupid naming - FIX!
            if c == "tau_e":
                prefix = ""
            else:
                prefix = "_"
            hist_up = f.Get(sample + "_" + c + prefix + "up_" + var["var_name"])
            hist_down = f.Get(sample + "_" + c + prefix + "down_" + var["var_name"])
            plot_variation(outpath, var["var_name"], var["xtitle"], c, hist_cent, hist_up, hist_down)
            
            if c == "jes":
                for c_var in ["03", "06", "09", "old"]:
                    
                    hist_up = f.Get(sample + "_" + c + "_up_" + c_var + "_" + var["var_name"])
                    hist_down = f.Get(sample + "_" + c + "_down_" + c_var + "_" + var["var_name"])
                    plot_variation(outpath, var["var_name"], var["xtitle"], c + "_" + c_var, hist_cent, hist_up, hist_down)
        
            if c == "tau_e":
                for c_var in ["05", "07", "09"]:
                    
                    hist_up = f.Get(sample + "_" + c + "up_" + c_var + "_" + var["var_name"])
                    hist_down = f.Get(sample + "_" + c + "down_" + c_var + "_" + var["var_name"])
                    plot_variation(outpath, var["var_name"], var["xtitle"], c + "_" + c_var, hist_cent, hist_up, hist_down)

        """            
        """
        hist_jes_up = f.Get(sample + "_jes_up_" + var["var_name"])
        hist_jes_down = f.Get(sample + "_jes_down_" + var["var_name"])
        hist_jer_up = f.Get(sample + "_jer_up_" + var["var_name"])
        hist_jer_down = f.Get(sample + "_jer_down_" + var["var_name"])        
        hist_tau_eup = f.Get(sample + "_tau_eup_" + var["var_name"])
        hist_tau_edown = f.Get(sample + "_tau_edown_" + var["var_name"])  
        hist_jes_up_old = f.Get(sample + "_jes_up_old_" + var["var_name"])
        hist_jes_down_old = f.Get(sample + "_jes_down_old_" + var["var_name"])
        hist_met_up = f.Get(sample + "_met_up_" + var["var_name"])
        hist_met_down = f.Get(sample + "_met_down_" + var["var_name"])
        
        
        # JES
        plot_variation(outpath, var["var_name"], var["xtitle"], "JES", hist_cent, hist_jes_up, hist_jes_down)
        plot_variation(outpath, var["var_name"], var["xtitle"], "JER", hist_cent, hist_jer_up, hist_jer_down)
        plot_variation(outpath, var["var_name"], var["xtitle"], "Tau_scale", hist_cent, hist_tau_eup, hist_tau_edown)
        plot_variation(outpath, var["var_name"], var["xtitle"], "JES_old", hist_cent, hist_jes_up_old, hist_jes_down_old)
        plot_variation(outpath, var["var_name"], var["xtitle"], "MET", hist_cent, hist_met_up, hist_met_down)
        """
        