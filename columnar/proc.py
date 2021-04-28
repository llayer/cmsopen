import pandas as pd
import numpy as np
import glob
import utils
from pathlib import Path

BASE_DIR = "/eos/user/l/llayer/cmsopen/columnar/note_v0/"
SAMPLES_DIR = BASE_DIR + "samples"
HIST_DIR = BASE_DIR + "histos"
STACK_DIR = BASE_DIR + "stack"
SYST_DIR = BASE_DIR + "syst"
BDT_DIR = BASE_DIR + "bdt_rs5"
COMBINE_DIR = BASE_DIR + "combine"

# TODO CHECK MET BUG

ev_sel = False
print_cutflow = False
do_plotting = False
do_stack = False
do_syst = False
run_bdt = False
plot_bdt = False
do_fit = False
to_hv = False
postfit = True

"""
variables = [
    {"var_name" : "MET_met", "bins" : 30, "xlow" : 0., "xup" : 400, "xtitle" : "MET [GeV]"},
]
"""
bdt_var = [
    {"var_name" : "bdt", "bins" : 20, "xlow" : 0., "xup" : 1., "xtitle" : "bdt", "max_sf": 15}
]

variables = [
    {"var_name" : "PV_npvs", "bins" : 30, "xlow" : 0., "xup" : 30, "xtitle" : "primary vertices", "max_sf": 50},
    {"var_name" : "MET_met", "bins" : 30, "xlow" : 0., "xup" : 400, "xtitle" : "MET [GeV]", "max_sf": 1.5},
    {"var_name" : "Tau_pt", "bins" : 30, "xlow" : 0., "xup" : 300, "xtitle" : "p_{T}(#tau) [GeV]", "max_sf": 15},
    {"var_name" : "Tau_eta", "bins" : 30, "xlow" : -3., "xup" : 3., "xtitle" : "#eta(#tau)", "max_sf": 50},
    {"var_name" : "aplanarity", "bins" : 20, "xlow" : 0., "xup" : 0.5, "xtitle" : "aplanarity", "max_sf": 15},
    {"var_name" : "ht", "bins" : 20, "xlow" : 0., "xup" : 1600., "xtitle" : "H_{T} [GeV]", "max_sf": 15},
    {"var_name" : "chargeEta", "bins" : 20, "xlow" : -3., "xup" : 3., "xtitle" : "q #times #eta(#tau)", "max_sf": 50},
    {"var_name" : "deltaPhiTauMet", "bins" : 20, "xlow" : 0., "xup" : 3.2, "xtitle" : "#Delta#phi(#tau, MET)", "max_sf": 50},
    {"var_name" : "mt", "bins" : 20, "xlow" : 0., "xup" : 300., "xtitle" : "M_{T}(#tau, MET) [GeV]", "max_sf": 15},
    {"var_name" : "mTauJet", "bins" : 20, "xlow" : 0., "xup" : 2500., "xtitle" :"M(#tau, jets) [GeV]", "max_sf": 15},
    {"var_name" : "nJets", "bins" : 10, "xlow" : 3., "xup" : 13., "xtitle" : "N. of jets", "max_sf": 50},
    {"var_name" : "Jet_pt", "bins" : 30, "xlow" : 0., "xup" : 500., "xtitle" : "p_{T}(jet) [GeV]", "max_sf": 15},
    {"var_name" : "Jet_eta", "bins" : 30, "xlow" : -3., "xup" : 3., "xtitle" : "#eta(jet)", "max_sf": 50},
    {"var_name" : "sphericity", "bins" : 20, "xlow" : 0., "xup" : 1.0, "xtitle" : "sphericity", "max_sf": 15}
]    


def run_taujet_sel():
    
    import taujet_selection

    outpath = SAMPLES_DIR
    taujet_selection.nominal(outpath)
    #taujet_selection.syst(outpath)
    #taujet_selection.syst_variations(outpath)

    
def plot_vars( variables, inpath ):
    
    import plot
    
    files = glob.glob(inpath + "*.h5")
    samples = {}
    for sample in files:
        
        if "cutflow" in sample: continue
        sample_name = sample.split("/")[-1][:-3]
        print(sample_name)
        samples[sample_name] = pd.read_hdf(sample)
        
    if "bdt" in inpath:
        file_name = "bdt"
    else:
        file_name = "histos"
    
    file_path = HIST_DIR + "/" + file_name + ".root"
    plot.vars_to_histos(samples, variables, file_path)
    
    print("Plotting done")
    
    
def plot_stack(variables, file_name):
    
    import stack
    
    sample_names = ["TTJets_bkg", "WZJets", "STJets", "QCD", "TTJets_signal"]
    for var in variables:
        stack.plot( HIST_DIR + "/" + file_name + ".root", var["var_name"], var["xtitle"], sample_names, 
                   STACK_DIR, max_sf=var["max_sf"])
    
    
def plot_syst(variables, file_name):
    
    import plot
    
    file_path = HIST_DIR + "/" + file_name + ".root"
    outpath = SYST_DIR
    print(file_path)
    plot.syst(variables, "TTJets_signal", file_path, outpath)
    
    
def bdt( outpath = BDT_DIR ):
    
    import ml
    
    files = glob.glob( SAMPLES_DIR + "/*.h5")
    samples = {}
    for sample in files:
        if "cutflow" in sample: continue
        sample_name = sample.split("/")[-1][:-3]
        print(sample_name)
        samples[sample_name] = pd.read_hdf(sample)
        
    #ml.train(samples, outpath, n_sig=4000, n_bkg=4000, ntrees=1000, lr=0.01)
    #ml.train(samples, outpath, n_sig=5000, n_bkg=5000, ntrees=500, lr=0.01, random_state=5)
    ml.train(samples, outpath, n_sig=5000, n_bkg=5000, ntrees=500, lr=0.01, random_state=5)
    
    dropvars = ['Jet_pt', 'Jet_px', 'Jet_py', 'Jet_pz', 'Jet_e', 'Jet_eta', 'Jet_phi',
       'Jet_mass', 'Jet_csvDisc', 'Jet_flavour', 'Tau_pt', 'Tau_px', 'Tau_py',
       'Tau_pz', 'Tau_e', 'Tau_eta', 'Tau_phi', 'Tau_mass', 'Tau_charge',
       'MET_pt', 'MET_px', 'MET_py', 'MET_pz', 'MET_e']
    
    for sample in samples:
        samples[sample].drop(dropvars, axis=1, errors='ignore').to_hdf(outpath + "/" + sample + ".h5", "frame")
    
        
def fit_xsec(var = "MET_met", file_name = "bdt_corr"):
    
    import fit 
    
    sample_names = ["Data", "TTJets_bkg", "WZJets", "STJets", "QCD", "TTJets_signal"]
    result = fit.fit(HIST_DIR + "/" + file_name + ".root", sample_names, var, corr="centJER")
    pd.DataFrame([result]).to_hdf(COMBINE_DIR + "/roofit/result.h5", "frame")
    #stack.plot( HIST_DIR + "/" + file_name + ".root", var, var, sample_names[1:], STACK_DIR, sfs = sfs, corr = "central" )
        
        
def plot_postfit(variables, file_name):

    import stack
    
    sample_names = ["TTJets_bkg", "WZJets", "STJets", "QCD", "TTJets_signal"]
    sfs = pd.read_hdf(COMBINE_DIR + "/roofit/result.h5")
    for var in variables:
        stack.plot( HIST_DIR + "/" + file_name + ".root", var["var_name"], var["xtitle"], sample_names, 
                   COMBINE_DIR + "/roofit", sfs = sfs, max_sf=var["max_sf"])        
        stack.plot( HIST_DIR + "/" + file_name + ".root", var["var_name"], var["xtitle"], sample_names, 
                   COMBINE_DIR + "/roofit", sfs = sfs, max_sf=var["max_sf"]) 
    infile = COMBINE_DIR + "/nominal/fit/syst/fitDiagnosticsTest.root"
    stack.plot( infile, "bdt", "bdt", sample_names, COMBINE_DIR + "/nominal", post_fit = True )
    
if __name__ == "__main__":
    
    
    if ev_sel:
        print("Start taujet event selection")
        Path(SAMPLES_DIR).mkdir(parents=True, exist_ok=True)
        run_taujet_sel()

    if print_cutflow:
        
        utils.print_yields(SAMPLES_DIR + "/cutflow.h5")
        
    if do_plotting:
        print( "Plotting" )
        Path(HIST_DIR).mkdir(parents=True, exist_ok=True)
        plot_vars(variables, inpath = SAMPLES_DIR + "/")
    
    if do_stack:
        print("Plot stack")
        Path(STACK_DIR).mkdir(parents=True, exist_ok=True)
        plot_stack(variables, "histos")
        
    if do_syst:
        print("Plot syst")
        Path(SYST_DIR).mkdir(parents=True, exist_ok=True)
        plot_syst(variables, "histos")
    
    if run_bdt:
        print("Run BDT")
        Path(BDT_DIR).mkdir(parents=True, exist_ok=True)
        bdt()
        #plot_vars([{"var_name" : "bdt", "bins" : 15, "xlow" : 0., "xup" : 1., "xtitle" : 0.}], inpath = "bdt")
        
    if plot_bdt:
        print("Plot BDT")
        plot_vars(bdt_var, inpath = BDT_DIR + "/")
        #plot_stack(bdt_var, "bdt")
        plot_syst(bdt_var, "bdt")
        
    if do_fit:
        Path(COMBINE_DIR + "/roofit").mkdir(parents=True, exist_ok=True)
        print("Fit with original script")
        #fit_xsec(var = "MET_met", file_name = "histos")
        fit_xsec(var = "bdt", file_name = "bdt")
        #fit_xsec(var = "MET_met")
        
    if to_hv:
        Path(COMBINE_DIR).mkdir(parents=True, exist_ok=True)
        outpath = COMBINE_DIR + "/harvester_input.root"
        inpath = HIST_DIR + "/bdt.root"
        utils.to_harvester(inpath, outpath)
         
        
    if postfit:
        
        plot_postfit(bdt_var, "bdt")
        
        
        

