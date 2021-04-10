import pandas as pd
import numpy as np
import selection
import weights
import hl
import plot
import stack
import fit 
import glob
import ml
import utils
import root_pandas
import h5py
from pathlib import Path

BASE_DIR = "/eos/user/l/llayer/cmsopen/columnar/syst_variation/"
SAMPLES_DIR = BASE_DIR + "samples"
CAND_DIR = BASE_DIR + "cand"
HIST_DIR = BASE_DIR + "histos"
STACK_DIR = BASE_DIR + "stack"
SYST_DIR = BASE_DIR + "syst"
BDT_DIR = BASE_DIR + "bdt_rs5"
COMBINE_DIR = BASE_DIR + "combine"

ev_sel = False
proc_cands = False
do_plotting = False
do_stack = False
do_syst = True
run_bdt = False
plot_bdt = False
do_fit = False
to_hv = False

"""
variables = [
    {"var_name" : "MET_met", "bins" : 30, "xlow" : 0., "xup" : 400, "xtitle" : "MET [GeV]"},
]
"""
bdt_var = [
    {"var_name" : "bdt", "bins" : 20, "xlow" : 0., "xup" : 1., "xtitle" : "bdt"}
]
variables = [
    #{"var_name" : "MET_met", "bins" : 30, "xlow" : 0., "xup" : 400, "xtitle" : "MET [GeV]"},
    #{"var_name" : "Tau_pt", "bins" : 30, "xlow" : 0., "xup" : 250, "xtitle" : "p_{T}(#tau) [GeV]"},
    #{"var_name" : "aplanarity", "bins" : 20, "xlow" : 0., "xup" : 0.5, "xtitle" : "aplanarity"},
    #{"var_name" : "ht", "bins" : 20, "xlow" : 0., "xup" : 1600., "xtitle" : "H_{T} [GeV]"},
    #{"var_name" : "chargeEta", "bins" : 20, "xlow" : -3., "xup" : 3., "xtitle" : "q #times #eta(#tau)"},
    #{"var_name" : "deltaPhiTauMet", "bins" : 20, "xlow" : 0., "xup" : 3.2, "xtitle" : "#Delta#phi(#tau, MET)"},
    #{"var_name" : "mt", "bins" : 20, "xlow" : 0., "xup" : 300., "xtitle" : "M_{T}(#tau, MET) [GeV]"},
    #{"var_name" : "mTauJet", "bins" : 20, "xlow" : 0., "xup" : 2500., "xtitle" :"M(#tau, jets) [GeV]"},
    #{"var_name" : "nJets", "bins" : 10, "xlow" : 0., "xup" : 10., "xtitle" : "N. of jets"},
    {"var_name" : "Jet_pt", "bins" : 30, "xlow" : 0., "xup" : 400., "xtitle" : "p_{T}(jet) [GeV]"},
    {"var_name" : "Jet_eta", "bins" : 30, "xlow" : -3., "xup" : 3., "xtitle" : "#eta(jet)"},
    {"var_name" : "sphericity", "bins" : 20, "xlow" : 0., "xup" : 1.0, "xtitle" : "sphericity"}
]    

data = ['Run2011A_MultiJet', 'Run2011B_MultiJet']
mc = ['T_TuneZ2_s', 'WJetsToLNu', 'DYJetsToLL', 'T_TuneZ2_tW', 'T_TuneZ2_t-channel',
       'Tbar_TuneZ2_s', 'Tbar_TuneZ2_tW', 'Tbar_TuneZ2_t-channel', 'TTJets']
corrections = ["centJER", "jes_up", "jes_down", "jes_up_old", "jes_down_old", "jer_up", "jer_down", "tau_eup", "tau_edown"]




def check_keys():
    
    for sample in mc:
        f = h5py.File(SAMPLES_DIR + "/" + sample + ".h5", 'r')
        #print(sample, f.keys())
        
        missing = False
        for k in corrections + ["central"]:
            if not (k in f.keys()):
                missing = True
                print()
                print("MISSING")
                print(sample, k)
            else:
                try:
                    pd.read_hdf(SAMPLES_DIR + "/" + sample + ".h5", k)
                except:
                    print()
                    print("Fail to read")
                    print(sample, k)                    
        if missing == False:
            print("No key missing")

def event_selection(outpath = SAMPLES_DIR):
    
    for sample in mc + data:
        
        #!!!!!!! Careful with JER application before Tau?!
        
        if "TTJets" in sample: isTT = True
        else: isTT = False
            
        if "Run2011" in sample: isData = True
        else: isData = False
        
        path = "/eos/user/l/llayer/opendata_files/preselection_merged/" + sample + ".root"
        
        df, cut_flow = selection.event_selection(path, isData = isData, isTT = isTT)
        print( cut_flow )
        df.to_hdf(outpath + "/" + sample + ".h5", "frame")
                
        if isData == False:
            
            for c in corrections:
                df, cut_flow = selection.event_selection(path, isData = isData, isTT = isTT, corrLevel = c)
                print( cut_flow )
                df.to_hdf(outpath + "/" + sample + "_" + c + ".h5", "frame")    
                
                # Vary the size of jes / tau scale
                if (c == "jes_up") | (c == "jes_down"):
                    for scale in [0.03, 0.06, 0.09]:
                        df, cut_flow = selection.event_selection(path, isData = isData, isTT = isTT, corrLevel = c, 
                                                                 jes_factor = scale)
                        print( cut_flow )
                        df.to_hdf(outpath + "/" + sample + "_" + c + "_" + str(scale)[2:] +".h5", "frame") 
                        
                if (c == "tau_eup") | (c == "tau_edown"):
                    for scale in [0.05, 0.07, 0.09]:
                        df, cut_flow = selection.event_selection(path, isData = isData, isTT = isTT, corrLevel = c, 
                                                                 tau_factor = scale)
                        print( cut_flow )
                        df.to_hdf(outpath + "/" + sample + "_" + c + "_" + str(scale)[2:] +".h5", "frame")                    
    #check_keys()

    
    
def candidates(sample, file, invert_btag = False, njets=-1, scale_met=None):
    
    if "Run2011" in sample: isData = True
    else: isData = False

    print( "Processing:", sample, "File", file ,"isData:", isData, "invert_btag:", invert_btag)

    #df = pd.read_hdf( SAMPLES_DIR + "/" + sample + ".h5", key=key)
    df = pd.read_hdf(file)
    df['nJets'] = df["Jet_pt"].str.len()
    
    # b-tagging
    df["Jet_nbtags"] = df["Jet_csvDisc"].apply( lambda x : btag.count_btags(x, njets=njets) )
    if invert_btag:
        df = btag.no_tag(df)
    else:
        df = btag.at_least_1tag(df)

    # MET cut
    if scale_met != None:
        df['MET_met'] = df['MET_met'] * scale_met
    df = selection.met_requirement(df)

    
    # HL features
    df = pd.concat([df, df.apply(lambda ev : pd.Series(hl.hlFeatures(ev, njets=njets)), axis=1)], axis=1)
    

    # MC weights
    if not isData:

        hlt_40, hlt_45 = weights.lumi()
        total_lumi = hlt_40 + hlt_45
        trigger_frac = hlt_40 / float(hlt_45)
        df = pd.concat([df, df.apply(lambda ev: pd.Series(btag.eval_sf_eff(ev)), axis=1)], axis=1)
        df["Jet_btag_weight1"] = df.apply(lambda ev : btag.b_weight_method1(ev, njets=njets), axis=1)
        if ( "centJER" in file):
            df["Jet_btag_weight1_up"] = df.apply(lambda ev : btag.b_weight_method1(ev, syst='up', njets=njets), axis=1)
            df["Jet_btag_weight1_down"] = df.apply(lambda ev : btag.b_weight_method1(ev, syst='down', njets=njets), axis=1)
        #df["Jet_btag_weight2"] = df.apply(lambda ev : btag.b_weight_method2(ev, njets=njets), axis=1)
        # trigger weights
        #df["trigger_weight"] = df.apply(lambda ev : weights.trigger_weight(ev, trigger_frac), axis=1)
        df = pd.concat([df, df.apply(lambda ev: pd.Series(weights.trigger_weight(ev, trigger_frac)), axis=1)], axis=1)
        # normalization
        counts_path = "/eos/user/l/llayer/opendata_files/preselection_merged/" + sample + "_counts.root"
        total_counts = root_pandas.read_root(counts_path)
        xsec = weights.get_xsec(sample)
        weights.norm(df, total_counts, xsec, lumi = total_lumi)
        
        # PDF
        if ("TTJets" in sample) & ( "centJER" in file):
            pdf = pd.read_hdf("TTJets_pdfweights.h5")
            df = pd.merge(df, pdf, how="left", on=["event", "luminosityBlock", "run"])

    # QCD
    if isData & invert_btag:
        
        # Assume light flavour
        def lf(nJets):
            return np.zeros((nJets))
        df["Jet_flavour"] = df["nJets"].apply(lf)
        df = pd.concat([df, df.apply(lambda ev: pd.Series(btag.eval_sf_eff(ev)), axis=1)], axis=1)
        df["btag_weight"] = df.apply(lambda ev : btag.b_weight_method2(ev, njets=njets), axis=1)
        df["btag_weight_up"] = df.apply(lambda ev : btag.b_weight_method2(ev, syst='up', njets=njets), axis=1)
        df["btag_weight_down"] = df.apply(lambda ev : btag.b_weight_method2(ev, syst='down', njets=njets), axis=1)

    return df
    
    
def rearrange_samples(samples):
    
    # Concat and split MC in signal and background
    new_samples = {}
    
    for key in ["central", "met_up", "met_down"] + corrections:

        #new_samples["TTJets_signal_" + key], new_samples["TTJets_bkg_" + key] = weights.classify_tt(samples["TTJets_" + key])
        new_samples["WZJets_" + key] = pd.concat([samples['WJetsToLNu_' + key], samples['DYJetsToLL_' + key]], axis=0)
        new_samples["STJets_" + key] = pd.concat([samples['T_TuneZ2_s_' + key], samples['T_TuneZ2_tW_' + key], 
                                                  samples['T_TuneZ2_t-channel_' + key], samples['Tbar_TuneZ2_s_' + key],
                                                  samples['Tbar_TuneZ2_tW_' + key], 
                                                  samples['Tbar_TuneZ2_t-channel_' + key]], axis=0)

    # Concat the data
    """
    new_samples["Data"] = pd.concat([samples["Run2011A_MultiJet"], samples["Run2011B_MultiJet"]], axis=0)
    new_samples["QCD"] = pd.concat([samples["QCD_Run2011A_MultiJet"], samples["QCD_Run2011B_MultiJet"]], axis=0)
    """
    return new_samples
    
    
def proc_candidates(outpath=CAND_DIR, njets = -1):
    
    import btag
    
    #
    # Data
    #
    """
    data_samples = {}
    for sample in data:
        file_path = SAMPLES_DIR + "/" + sample + ".h5"
        data_samples[sample] = candidates(sample, file_path, invert_btag = False, njets=njets)
        data_samples["QCD_" + sample] = candidates(sample, file_path, invert_btag = True, njets=njets) 
    # Write to disc
    df_data = pd.concat([data_samples["Run2011A_MultiJet"], data_samples["Run2011B_MultiJet"]], axis=0)
    df_qcd = pd.concat([data_samples["QCD_Run2011A_MultiJet"], data_samples["QCD_Run2011B_MultiJet"]], axis=0)
    df_data.to_hdf(outpath + "/Data.h5", "frame")
    df_qcd.to_hdf(outpath + "/QCD.h5", "frame")
    """
    
    #
    # MC
    #
    
    sample_files = {}
    for sample in mc:
        sample_files[sample] = glob.glob(SAMPLES_DIR + "/" + sample + "*.h5")
    
    #
    # TTJets
    #
    """
    for file in sample_files["TTJets"]:
        name = file.split("/")[-1][:-3]
        if name == "TTJets":
            continue
        df = candidates("TTJets", file, invert_btag = False, njets=njets) 
        tt_sig, tt_bkg = weights.classify_tt(df)    
        tt_sig.to_hdf(outpath + "/" + name + "_signal" +".h5", "frame")
        tt_bkg.to_hdf(outpath + "/" + name + "_bkg" +".h5", "frame")
    # MET
    for var in ["up", "down"]:
        file_path = SAMPLES_DIR + "/TTJets_centJER.h5"
        if var == "up":
            df = candidates("TTJets", file_path, invert_btag = False, njets=njets, scale_met=1.1)
        else:
            df = candidates("TTJets", file_path, invert_btag = False, njets=njets, scale_met=0.9)
        tt_sig, tt_bkg = weights.classify_tt(df)    
        tt_sig.to_hdf(outpath + "/" + "TTJets_met_" + var + "_signal" +".h5", "frame")
        tt_bkg.to_hdf(outpath + "/" + "TTJets_met_" + var + "_bkg" +".h5", "frame")
    """ 
    
    mc_samples = {}
    for sample in ['T_TuneZ2_s', 'WJetsToLNu', 'DYJetsToLL', 'T_TuneZ2_tW', 'T_TuneZ2_t-channel',
                   'Tbar_TuneZ2_s', 'Tbar_TuneZ2_tW', 'Tbar_TuneZ2_t-channel']:
        for file in sample_files[sample]:
            name = file.split("/")[-1][:-3]
            mc_samples[name] = candidates(sample, file, invert_btag = False, njets=njets) 
        for var in ["up", "down"]:
            file_path = SAMPLES_DIR + "/" + sample + "_centJER.h5"
            if var == "up":
                mc_samples[sample + "_met_" + var] = candidates(sample, file_path, 
                                                                invert_btag = False, njets=njets, scale_met=1.1)
            else:
                mc_samples[sample + "_met_" + var] = candidates(sample, file_path, 
                                                                invert_btag = False, njets=njets, scale_met=0.9) 
               
    tau_var = ["_tau_eup_" + str(scale)[2:] for scale in [0.05, 0.07, 0.09]] + \
              ["_tau_edown_" + str(scale)[2:] for scale in [0.05, 0.07, 0.09]]
    jes_var = ["_jes_up_" + str(scale)[2:] for scale in [0.03, 0.06, 0.09]] + \
              ["_jes_down_" + str(scale)[2:] for scale in [0.03, 0.06, 0.09]]
    corr = ["_" + c for c in corrections]
    
    new_samples = {}
    for key in ["", "_met_up", "_met_down"] + corr + jes_var + tau_var:

        #new_samples["TTJets_signal_" + key], new_samples["TTJets_bkg_" + key] = weights.classify_tt(samples["TTJets_" + key])
        new_samples["WZJets" + key] = pd.concat([mc_samples['WJetsToLNu' + key], mc_samples['DYJetsToLL' + key]], axis=0)
        new_samples["STJets" + key] = pd.concat([mc_samples['T_TuneZ2_s' + key], mc_samples['T_TuneZ2_tW' + key], 
                                                  mc_samples['T_TuneZ2_t-channel' + key], mc_samples['Tbar_TuneZ2_s' + key],
                                                  mc_samples['Tbar_TuneZ2_tW' + key], 
                                                  mc_samples['Tbar_TuneZ2_t-channel' + key]], axis=0)
        
    #new_samples = rearrange_samples(samples)
    for sample in new_samples:
        new_samples[sample].to_hdf(outpath + "/" + sample + ".h5", "frame")
        
    """
    for name in ["Data", "TTJets_bkg", "WZJets", "STJets", "QCD", "TTJets_signal"]:
        for sample in new_samples:
            if name in sample:
                # DANGER!! Delete samples
                new_samples[sample].to_hdf(outpath + "/" + name + ".h5", sample, mode='a')
    """
           
    
def plot_vars( variables, inpath ):
    
    
    files = glob.glob(inpath + "*.h5")
    samples = {}
    for sample in files:
       
        sample_name = sample.split("/")[-1][:-3]
        print(sample_name)
        samples[sample_name] = pd.read_hdf(sample)
        
        """
        if (sample_name != "Data") & (sample_name != "QCD"):
            #samples[sample_name] = pd.read_hdf(sample, sample_name + "_central")
            #samples[sample_name] = pd.read_hdf(sample, sample_name + "_centJER")
            for key in ["central", "met_up", "met_down"] + corrections:
                samples[sample_name + "_" + key] = pd.read_hdf(sample, sample_name + "_" + key)
        else:
            samples[sample_name] = pd.read_hdf(sample)
        """
        
    print( "Plotting" )
    if "bdt" in inpath:
        file_name = "bdt"
        
        #samples["TTJets_signal"] = samples["TTJets_signal"][samples["TTJets_signal"]["train_flag"] == "test"]
        #print("TTJets_signal", sum(samples["TTJets_signal"]['weight']))
    else:
        file_name = "histos"
    
    file_path = HIST_DIR + "/" + file_name + ".root"
    plot.vars_to_histos(samples, variables, file_path)
    
    print("Plotting done")
    
    
def plot_stack(variables, file_name):
    
    sample_names = ["TTJets_bkg", "WZJets", "STJets", "QCD", "TTJets_signal"]
    for var in variables:
        stack.plot( HIST_DIR + "/" + file_name + ".root", var["var_name"], var["xtitle"], sample_names, STACK_DIR,
                   corr = "centJER" )
    
    
def bdt( outpath = BDT_DIR ):
    
    files = glob.glob( CAND_DIR + "/*.h5")
    samples = {}
    for sample in files:
        sample_name = sample.split("/")[-1][:-3]
        print(sample_name)
        samples[sample_name] = pd.read_hdf(sample)
        """
        if (sample_name != "Data") & (sample_name != "QCD"):
            for key in ["central", "met_up", "met_down"] + corrections:
                samples[sample_name + "_" + key] = pd.read_hdf(sample, sample_name + "_" + key)
        else:
            samples[sample_name] = pd.read_hdf(sample)
        """
        
    #ml.train(samples, outpath, n_sig=4000, n_bkg=4000, ntrees=1000, lr=0.01)
    ml.train(samples, outpath, n_sig=5000, n_bkg=5000, ntrees=500, lr=0.01, random_state=5)
    
    dropvars = ['Jet_pt', 'Jet_px', 'Jet_py', 'Jet_pz', 'Jet_e', 'Jet_eta', 'Jet_phi',
       'Jet_mass', 'Jet_csvDisc', 'Jet_flavour', 'Tau_pt', 'Tau_px', 'Tau_py',
       'Tau_pz', 'Tau_e', 'Tau_eta', 'Tau_phi', 'Tau_mass', 'Tau_charge',
       'MET_pt', 'MET_px', 'MET_py', 'MET_pz', 'MET_e']
    
    #for name in ["Data", "TTJets_bkg", "WZJets", "STJets", "QCD", "TTJets_signal"]:
    for sample in samples:
        samples[sample].drop(dropvars, axis=1, errors='ignore').to_hdf(outpath + "/" + sample + ".h5", "frame")
    
    
def plot_syst(variables, file_name):
    
    file_path = HIST_DIR + "/" + file_name + ".root"
    outpath = SYST_DIR
    print(file_path)
    plot.syst(variables, "TTJets_signal", file_path, outpath)
    
    
def fit_xsec(var = "MET_met", file_name = "bdt_corr", syst=False):
    
    sample_names = ["Data", "TTJets_bkg", "WZJets", "STJets", "QCD", "TTJets_signal"]
    sf_tt_sig, sf_qcd = fit.fit(HIST_DIR + "/" + file_name + ".root", sample_names, var, corr="centJER")
    sfs = {}
    sfs["TTJets_signal"] = sf_tt_sig
    sfs["QCD"] = sf_qcd
    #stack.plot( HIST_DIR + "/" + file_name + ".root", var, var, sample_names[1:], STACK_DIR, sfs = sfs, corr = "central" )
    
    if syst:
        
        # PU - not applied
        # JES/JER/TauScale/MET
        for c in  ["jes_up", "jes_down", "jes_up_old", "jes_down_old", "jer_up", "jer_down", "tau_eup", "tau_edown",
                   "met_up", "met_down"]:
            sf_tt_sig, sf_qcd = fit.fit("histos/" + file_name + ".root", sample_names, var, corr=c)     
            sfs["TTJets_signal_" + c] = sf_tt_sig
            
        # B-tagging, Trigger stat, XSEC theory, PDF
        for c in ["btag_up", "btag_down", "trigger_up", "trigger_down", "xsec_up", "xsec_down", "pdf_up", "pdf_down"]:
            sf_tt_sig, sf_qcd = fit.fit("histos/" + file_name + ".root", sample_names, var, corr = "centJER_" + c)     
            sfs["TTJets_signal_" + c] = sf_tt_sig
            
        # PDF
        
        # QCD reweighting
        
        
        # Tau trigger leg  5%
        
        # Lumi 2%
        
        # 
            
        # 
        
        #
        # TODO rerun with real samples!
        #
        # Top mass
        sfs["TTJets_signal_topmass_down"] = sfs["TTJets_signal"] - 0.028 * sfs["TTJets_signal"]
        sfs["TTJets_signal_topmass_up"] = sfs["TTJets_signal"] + 0.028 * sfs["TTJets_signal"]

        # Top scale
        sfs["TTJets_signal_qscale_down"] = sfs["TTJets_signal"] - 0.022 * sfs["TTJets_signal"]
        sfs["TTJets_signal_qscale_up"] = sfs["TTJets_signal"] + 0.022 * sfs["TTJets_signal"]

        # Top mass
        sfs["TTJets_signal_partonmatch_down"] = sfs["TTJets_signal"] - 0.03 * sfs["TTJets_signal"]
        sfs["TTJets_signal_partonmatch_up"] = sfs["TTJets_signal"] + 0.03 * sfs["TTJets_signal"]

    for name, value in sfs.items():
        
        print(name, value)
    
    
    
if __name__ == "__main__":
    
    
    if ev_sel:
        Path(SAMPLES_DIR).mkdir(parents=True, exist_ok=True)
        event_selection()
        #check_keys()

    if proc_cands:
        Path(CAND_DIR).mkdir(parents=True, exist_ok=True)
        print("Process candidates")
        proc_candidates()
    
    if do_plotting:
        Path(HIST_DIR).mkdir(parents=True, exist_ok=True)
        plot_vars(variables, inpath = CAND_DIR + "/")
    
    if do_stack:
        Path(STACK_DIR).mkdir(parents=True, exist_ok=True)
        plot_stack(variables, "histos")
        
    if do_syst:
        Path(SYST_DIR).mkdir(parents=True, exist_ok=True)
        plot_syst(variables, "histos")
    
    if run_bdt:
        Path(BDT_DIR).mkdir(parents=True, exist_ok=True)
        bdt()
        #plot_vars([{"var_name" : "bdt", "bins" : 15, "xlow" : 0., "xup" : 1., "xtitle" : 0.}], inpath = "bdt")
        
    if plot_bdt:
        #plot_vars(bdt_var, inpath = BDT_DIR + "/")
        #plot_stack(bdt_var, "bdt")
        plot_syst(bdt_var, "bdt")
        
    if do_fit:
        #fit_xsec(var = "MET_met", file_name = "histos")
        fit_xsec(var = "bdt", file_name = "bdt")
        #fit_xsec(var = "MET_met")
        
    if to_hv:
        Path(COMBINE_DIR).mkdir(parents=True, exist_ok=True)
        outpath = COMBINE_DIR + "/harvester_input.root"
        inpath = HIST_DIR + "/bdt.root"
        utils.to_harvester(inpath, outpath)

