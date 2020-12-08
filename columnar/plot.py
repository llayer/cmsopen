import pandas as pd
import numpy as np
import ROOT
from root_numpy import fill_hist

def save_var(sample, name, var_name, bins = 20, xlow = 0., xup = 350):

    hist = ROOT.TH1D(name + "_" + var_name, name + "_" + var_name, bins, xlow, xup)
    hist.Sumw2()
    if name == "Data":
        fill_hist(hist, sample[var_name])
    else:
        fill_hist(hist, sample[var_name], weights = sample["weight"])
    #print s, hist.Integral()
    hist.Write()

def vars_to_histos(samples, variables, file_name = "histos"):
    file = ROOT.TFile("histos/" + file_name + ".root", 'recreate')
    for name, sample in samples.items():
        for var in variables:
            save_var(sample, name, var["var_name"], var["bins"], var["xlow"], var["xup"])

    file.Close()