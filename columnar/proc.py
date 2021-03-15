import pandas as pd
import numpy as np
import selection
import weights
import btag
import hl
import plot
import stack
import fit 
import glob
import ml
import root_pandas


data = ['Run2011A_MultiJet', 'Run2011B_MultiJet']
mc = ['T_TuneZ2_s', 'TTJets', 'WJetsToLNu', 'DYJetsToLL', 'T_TuneZ2_tW', 'T_TuneZ2_t-channel',
       'Tbar_TuneZ2_s', 'Tbar_TuneZ2_tW', 'Tbar_TuneZ2_t-channel']
corrections = ["centJER", "jes_up", "jes_down", "jes_up_old", "jes_down_old", "jer_up", "jer_down", "tau_eup", "tau_edown"]


def event_selection(outpath = "samples_corr/"):
        
    for sample in mc + data:
        
        #!!!!!!! Careful with JER application before Tau?!
        
        if "TTJets" in sample: isTT = True
        else: isTT = False
            
        if "Run2011" in sample: isData = True
        else: isData = False
        
        path = "/eos/user/l/llayer/opendata_files/preselection_merged/" + sample + ".root"
        
        df, cut_flow = selection.event_selection(path, isData = isData, isTT = isTT)
        print( cut_flow )
        df.to_hdf(outpath + sample + ".h5", "central", mode='w')
                
        if isData == False:
            
            for c in corrections:
                df, cut_flow = selection.event_selection(path, isData = isData, isTT = isTT, corrLevel = c)
                print( cut_flow )
                df.to_hdf(outpath + sample + ".h5", c, mode='a')         

    
    
def candidates(sample, key, invert_btag = False, njets=-1):
    
    if "Run2011" in sample: isData = True
    else: isData = False

    print( "Processing:", sample, "isData:", isData, "invert_btag:", invert_btag)

    df = pd.read_hdf("samples_corr/" + sample + ".h5", key=key)
    df['nJets'] = df["Jet_pt"].str.len()
    
    # b-tagging
    df["Jet_nbtags"] = df["Jet_csvDisc"].apply( lambda x : btag.count_btags(x, njets=njets) )
    if invert_btag:
        df = btag.no_tag(df)
    else:
        df = btag.at_least_1tag(df)

    # MET cut
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
        df["Jet_btag_weight2"] = df.apply(lambda ev : btag.b_weight_method2(ev, njets=njets), axis=1)
        # trigger weights
        df["trigger_weight"] = df.apply(lambda ev : weights.trigger_weight(ev, trigger_frac), axis=1)
        # normalization
        counts_path = "/eos/user/l/llayer/opendata_files/preselection_merged/" + sample + "_counts.root"
        total_counts = root_pandas.read_root(counts_path)
        xsec = weights.get_xsec(sample)
        weights.norm(df, total_counts, xsec, lumi = total_lumi)
    
    # QCD
    if isData & invert_btag:
        
        # Assume light flavour
        def lf(nJets):
            return np.zeros((nJets))
        df["Jet_flavour"] = df["nJets"].apply(lf)
        df = pd.concat([df, df.apply(lambda ev: pd.Series(btag.eval_sf_eff(ev)), axis=1)], axis=1)
        df["btag_weight"] = df.apply(lambda ev : btag.b_weight_method2(ev, njets=njets), axis=1)
        
    return df
    
    
def rearrange_samples(samples):
    
    # Concat and split MC in signal and background
    new_samples = {}
    for key in ["central"] + corrections:

        new_samples["TTJets_signal_" + key], new_samples["TTJets_bkg_" + key] = weights.classify_tt(samples["TTJets_" + key])
        new_samples["WZJets_" + key] = pd.concat([samples['WJetsToLNu_' + key], samples['DYJetsToLL_' + key]], axis=0)
        new_samples["STJets_" + key] = pd.concat([samples['T_TuneZ2_s_' + key], samples['T_TuneZ2_tW_' + key], 
                                                  samples['T_TuneZ2_t-channel_' + key], samples['Tbar_TuneZ2_s_' + key],
                                                  samples['Tbar_TuneZ2_tW_' + key], 
                                                  samples['Tbar_TuneZ2_t-channel_' + key]], axis=0)

    # Concat the data
    new_samples["Data"] = pd.concat([samples["Run2011A_MultiJet"], samples["Run2011B_MultiJet"]], axis=0)
    new_samples["QCD"] = pd.concat([samples["QCD_Run2011A_MultiJet"], samples["QCD_Run2011B_MultiJet"]], axis=0)
    
    return new_samples
    
    
def proc_candidates(outpath="candidates_corr/", njets = -1):
    
    samples = {}
        
    for sample in data:
        path = "preselection_merged/" + sample + ".root"
        samples[sample] = candidates(sample, "central", invert_btag = False, njets=njets)
        samples["QCD_" + sample] = candidates(sample, "central", invert_btag = True, njets=njets)
        
    for sample in mc:
        
        for key in ["central"] + corrections:
            samples[sample + "_" + key] = candidates(sample, key, invert_btag = False, njets=njets)   
     
    new_samples = rearrange_samples(samples)
    
    for name in ["Data", "TTJets_bkg", "WZJets", "STJets", "QCD", "TTJets_signal"]:
        for sample in new_samples:
            if name in sample:
                # DANGER!! Delete samples
                new_samples[sample].to_hdf(outpath + name + ".h5", sample, mode='a')
    
    
def plot_vars( variables, inpath = "bdt_corr/"):
    
    files = glob.glob(inpath + "*.h5")
    samples = {}
    for sample in files:
        sample_name = sample.split("/")[-1][:-3]
        print(sample_name)
        if (sample_name != "Data") & (sample_name != "QCD"):
            for key in ["central"] + corrections:
                samples[sample_name + "_" + key] = pd.read_hdf(sample, sample_name + "_" + key)
        else:
            samples[sample_name] = pd.read_hdf(sample)
    
    print( "Plotting" )
    if "bdt" in inpath:
        file_name = "bdt_corr"
        
        #samples["TTJets_signal"] = samples["TTJets_signal"][samples["TTJets_signal"]["train_flag"] == "test"]
        #print("TTJets_signal", sum(samples["TTJets_signal"]['weight']))
    else:
        file_name = "histos"
        
    plot.vars_to_histos(samples, variables, file_name = file_name)
    
    print("Plotting done")
    
    
def plot_stack(variables, file_name = "bdt_corr"):
    
    sample_names = ["TTJets_bkg", "WZJets", "STJets", "QCD", "TTJets_signal"]
    for var in variables:
        stack.plot( "histos/" + file_name + ".root", var["var_name"], var["xtitle"], sample_names, corr = "centJER" )
    
    
def bdt(outpath = "bdt_corr/"):
    
    files = glob.glob("candidates_corr/*.h5")
    samples = {}
    for sample in files:
        sample_name = sample.split("/")[-1][:-3]
        print(sample_name)
        if (sample_name != "Data") & (sample_name != "QCD"):
            for key in ["central"] + corrections:
                samples[sample_name + "_" + key] = pd.read_hdf(sample, sample_name + "_" + key)
        else:
            samples[sample_name] = pd.read_hdf(sample)
            
    ml.train(samples)
    
    for name in ["Data", "TTJets_bkg", "WZJets", "STJets", "QCD", "TTJets_signal"]:
        for sample in samples:
            if name in sample:
                # DANGER!! Delete samples
                samples[sample].to_hdf(outpath + name + ".h5", sample, mode='a')
    
    
def plot_syst(variables, file_name = "bdt_corr"):
    
    plot.syst(variables, sample = "TTJets_signal", file_name = file_name)
    
    
def fit_xsec(var = "MET_met", file_name = "bdt_corr"):
    
    sample_names = ["Data", "TTJets_bkg", "WZJets", "STJets", "QCD", "TTJets_signal"]
    sf_tt_sig, sf_qcd = fit.fit("histos/" + file_name + ".root", sample_names, var, corr="centJER")
    sfs = {}
    sfs["TTJets_signal"] = sf_tt_sig
    sfs["QCD"] = sf_qcd
    #stack.plot( "histos/" + file_name + ".root", var, sample_names[1:], sfs )
    
if __name__ == "__main__":
    
    ev_sel = False
    proc_cands = True
    do_plotting = False
    run_bdt = False
    plot_bdt = False
    do_stack = False
    do_syst = False
    do_fit = False
    
    if ev_sel:
        event_selection()

    if proc_cands:
        proc_candidates()
        
    if do_plotting:
        
        variables = [
            {"var_name" : "MET_met", "bins" : 30, "xlow" : 0., "xup" : 400, "xtitle" : "MET [GeV]"},
            {"var_name" : "Tau_pt", "bins" : 30, "xlow" : 0., "xup" : 250, "xtitle" : "p_{T}(#tau) [GeV]"},
            {"var_name" : "aplanarity", "bins" : 20, "xlow" : 0., "xup" : 0.5, "xtitle" : "aplanarity"},
            {"var_name" : "ht", "bins" : 20, "xlow" : 0., "xup" : 1600., "xtitle" : "H_{T} [GeV]"},
            {"var_name" : "chargeEta", "bins" : 20, "xlow" : -3., "xup" : 3., "xtitle" : "q #times #eta(#tau)"},
            {"var_name" : "deltaPhiTauMet", "bins" : 20, "xlow" : 0., "xup" : 3.2, "xtitle" : "#Delta#phi(#tau, MET)"},
            {"var_name" : "mt", "bins" : 20, "xlow" : 0., "xup" : 300., "xtitle" : "M_{T}(#tau, MET) [GeV]"},
            {"var_name" : "mTauJet", "bins" : 20, "xlow" : 0., "xup" : 2500., "xtitle" :"M(#tau, jets) [GeV]"},
            {"var_name" : "nJets", "bins" : 10, "xlow" : 0., "xup" : 10., "xtitle" : "N. of jets"},
            {"var_name" : "Jet_pt", "bins" : 30, "xlow" : 0., "xup" : 400., "xtitle" : "p_{T}(jet) [GeV]"},
            {"var_name" : "Jet_eta", "bins" : 30, "xlow" : -3., "xup" : 3., "xtitle" : "#eta(jet)"},
            {"var_name" : "sphericity", "bins" : 20, "xlow" : 0., "xup" : 1.0, "xtitle" : "sphericity"}
            
        ]
        plot_vars(variables)
        
    if run_bdt:
        bdt()
        plot_vars([{"var_name" : "bdt", "bins" : 15, "xlow" : 0., "xup" : 1., "xtitle" : 0.}], inpath = "bdt")
        
    if plot_bdt:
        variables = [
            {"var_name" : "MET_met", "bins" : 30, "xlow" : 0., "xup" : 400, "xtitle" : "MET [GeV]"},
            {"var_name" : "Tau_pt", "bins" : 30, "xlow" : 0., "xup" : 250, "xtitle" : "p_{T}(#tau) [GeV]"},
            {"var_name" : "aplanarity", "bins" : 20, "xlow" : 0., "xup" : 0.5, "xtitle" : "aplanarity"},
            {"var_name" : "ht", "bins" : 20, "xlow" : 0., "xup" : 1600., "xtitle" : "H_{T} [GeV]"},
            {"var_name" : "chargeEta", "bins" : 20, "xlow" : -3., "xup" : 3., "xtitle" : "q #times #eta(#tau)"},
            {"var_name" : "deltaPhiTauMet", "bins" : 20, "xlow" : 0., "xup" : 3.2, "xtitle" : "#Delta#phi(#tau, MET)"},
            {"var_name" : "mt", "bins" : 20, "xlow" : 0., "xup" : 300., "xtitle" : "M_{T}(#tau, MET) [GeV]"},
            {"var_name" : "mTauJet", "bins" : 20, "xlow" : 0., "xup" : 2500., "xtitle" :"M(#tau, jets) [GeV]"},
            {"var_name" : "nJets", "bins" : 10, "xlow" : 0., "xup" : 10., "xtitle" : "N. of jets"},
            {"var_name" : "Jet_pt", "bins" : 30, "xlow" : 0., "xup" : 400., "xtitle" : "p_{T}(jet) [GeV]"},
            {"var_name" : "Jet_eta", "bins" : 30, "xlow" : -3., "xup" : 3., "xtitle" : "#eta(jet)"},
            {"var_name" : "sphericity", "bins" : 20, "xlow" : 0., "xup" : 1.0, "xtitle" : "sphericity"},
            {"var_name" : "bdt", "bins" : 15, "xlow" : 0., "xup" : 1., "xtitle" : "bdt"}
        ]
        plot_vars(variables, inpath = "bdt_corr/")
        #plot_vars([{"var_name" : "bdt", "bins" : 15, "xlow" : 0., "xup" : 1., "xtitle" : 0.}], inpath = "bdt")
        
    if do_stack:
        
        variables = [
            {"var_name" : "MET_met", "bins" : 30, "xlow" : 0., "xup" : 400, "xtitle" : "MET [GeV]"},
            {"var_name" : "Tau_pt", "bins" : 30, "xlow" : 0., "xup" : 250, "xtitle" : "p_{T}(#tau) [GeV]"},
            {"var_name" : "aplanarity", "bins" : 20, "xlow" : 0., "xup" : 0.5, "xtitle" : "aplanarity"},
            {"var_name" : "ht", "bins" : 20, "xlow" : 0., "xup" : 1600., "xtitle" : "H_{T} [GeV]"},
            {"var_name" : "chargeEta", "bins" : 20, "xlow" : -3., "xup" : 3., "xtitle" : "q #times #eta(#tau)"},
            {"var_name" : "deltaPhiTauMet", "bins" : 20, "xlow" : 0., "xup" : 3.2, "xtitle" : "#Delta#phi(#tau, MET)"},
            {"var_name" : "mt", "bins" : 20, "xlow" : 0., "xup" : 300., "xtitle" : "M_{T}(#tau, MET) [GeV]"},
            {"var_name" : "mTauJet", "bins" : 20, "xlow" : 0., "xup" : 2500., "xtitle" :"M(#tau, jets) [GeV]"},
            {"var_name" : "nJets", "bins" : 10, "xlow" : 0., "xup" : 10., "xtitle" : "N. of jets"},
            {"var_name" : "Jet_pt", "bins" : 30, "xlow" : 0., "xup" : 400., "xtitle" : "p_{T}(jet) [GeV]"},
            {"var_name" : "Jet_eta", "bins" : 30, "xlow" : -3., "xup" : 3., "xtitle" : "#eta(jet)"},
            {"var_name" : "sphericity", "bins" : 20, "xlow" : 0., "xup" : 1.0, "xtitle" : "sphericity"},
            {"var_name" : "bdt", "bins" : 15, "xlow" : 0., "xup" : 1., "xtitle" : "bdt"}
        ]
        plot_stack(variables)
        
    if do_syst:
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
            #{"var_name" : "Jet_pt", "bins" : 30, "xlow" : 0., "xup" : 400., "xtitle" : "p_{T}(jet) [GeV]"},
            #{"var_name" : "Jet_eta", "bins" : 30, "xlow" : -3., "xup" : 3., "xtitle" : "#eta(jet)"},
            #{"var_name" : "sphericity", "bins" : 20, "xlow" : 0., "xup" : 1.0, "xtitle" : "sphericity"},
            {"var_name" : "bdt", "bins" : 15, "xlow" : 0., "xup" : 1., "xtitle" : "bdt"}
        ]
        plot_syst(variables)
        
    if do_fit:
        fit_xsec(var = "bdt", file_name = "bdt_corr")
        #fit_xsec(var = "MET_met")
        



