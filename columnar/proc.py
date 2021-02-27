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


def event_selection(outpath = "samples/"):
        
    for sample in mc + data:
        
        if "TTJets" in sample: isTT = True
        else: isTT = False
            
        if "Run2011" in sample: isData = True
        else: isData = False
        
        path = "/eos/user/l/llayer/opendata_files/preselection_merged/" + sample + ".root"
        df, cut_flow = selection.event_selection(path, isData = isData, isTT = isTT)
        
        """
        df = selection.met_requirement(df)
        cut_flow["met"] = len(df)
        
        df["Jet_nbtags"] = df["Jet_csvDisc"].apply( lambda x : btag.count_btags(x, njets=-1) )
        df = btag.at_least_1tag(df)
        cut_flow["btag"] = len(df)
        """
        print( cut_flow )
        
        df.to_hdf(outpath + sample + ".h5", "frame", mode='w')
    
    
def candidates(sample, invert_btag = False, njets=-1):
    
    if "Run2011" in sample: isData = True
    else: isData = False

    print( "Processing:", sample, "isData:", isData, "invert_btag:", invert_btag)

    df = pd.read_hdf("samples/" + sample + ".h5")
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
    new_samples["TTJets_signal"], new_samples["TTJets_bkg"] = weights.classify_tt(samples["TTJets"])
    new_samples["WZJets"] = pd.concat([samples['WJetsToLNu'], samples['DYJetsToLL']], axis=0)
    new_samples["STJets"] = pd.concat([samples['T_TuneZ2_s'], samples['T_TuneZ2_tW'], samples['T_TuneZ2_t-channel'], 
                              samples['Tbar_TuneZ2_s'], samples['Tbar_TuneZ2_tW'], 
                              samples['Tbar_TuneZ2_t-channel']], axis=0)

    # Concat the data
    new_samples["Data"] = pd.concat([samples["Run2011A_MultiJet"], samples["Run2011B_MultiJet"]], axis=0)
    new_samples["QCD"] = pd.concat([samples["QCD_Run2011A_MultiJet"], samples["QCD_Run2011B_MultiJet"]], axis=0)
    
    return new_samples
    
    
def proc_candidates(outpath="candidates/", njets = -1):
    
    samples = {}
        
    for sample in data:
        path = "preselection_merged/" + sample + ".root"
        samples[sample] = candidates(sample, invert_btag = False, njets=njets)
        samples["QCD_" + sample] = candidates(sample, invert_btag = True, njets=njets)
        
    for sample in mc:
        samples[sample] = candidates(sample, invert_btag = False, njets=njets)   
     
    new_samples = rearrange_samples(samples)
    
    for sample in new_samples:
        new_samples[sample].to_hdf(outpath + sample + ".h5", "frame", mode='w')
    
    
def plot_vars( variables, inpath = "candidates"):
    
    files = glob.glob(inpath + "/*.h5")
    samples = {}
    for sample in files:
        sample_name = sample.split("/")[-1][:-3]
        #print(sample_name)
        samples[sample_name] = pd.read_hdf(sample)
        
    # Final weights
    for sample in samples:
        
        if sample == "Data":
            pass
        elif sample == "QCD":
            if inpath == "bdt":
                scale_qcd = 9.
            else:
                scale_qcd = 4.3
            samples[sample]['weight'] = samples[sample]['btag_weight'] * scale_qcd
        else:
            #samples[sample]['new_trigger_weight'] = new_samples[sample].apply(lambda ev : weights.trigger_weight(ev), axis=1)
            samples[sample]['weight'] = samples[sample]['norm'] * (1/1000) * samples[sample]['trigger_weight'] * samples[sample]['Jet_btag_weight1']
            print(sample, sum(samples[sample]['weight']))
            #new_samples[sample]['btag_weight2']
    
    print( "Plotting" )
    if inpath == "bdt":
        file_name = "bdt"
        
        #samples["TTJets_signal"] = samples["TTJets_signal"][samples["TTJets_signal"]["train_flag"] == "test"]
        #print("TTJets_signal", sum(samples["TTJets_signal"]['weight']))
        samples["QCD"] = samples["QCD"][samples["QCD"]["train_flag"] == "test"]
    else:
        file_name = "histos"
    plot.vars_to_histos(samples, variables, file_name = file_name)
    sample_names = ["TTJets_bkg", "WZJets", "STJets", "QCD", "TTJets_signal"]
    for var in variables:
        stack.plot( "histos/" + file_name + ".root", var["var_name"], var["xtitle"], sample_names )
    
    print("Plotting done")
    
def bdt():
    
    ml.train()
            
    
    
def fit_xsec(var = "MET_met", file_name = "histos"):
    
    sample_names = ["Data", "TTJets_bkg", "WZJets", "STJets", "QCD", "TTJets_signal"]
    sf_tt_sig, sf_qcd = fit.fit("histos/" + file_name + ".root", sample_names, var)
    sfs = {}
    sfs["TTJets_signal"] = sf_tt_sig
    sfs["QCD"] = sf_qcd
    stack.plot( "histos/" + file_name + ".root", var, sample_names[1:], sfs )
    
if __name__ == "__main__":
    
    ev_sel = False
    proc_cands = False
    do_plotting = True
    run_bdt = False
    plot_bdt = False
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
        plot_vars([{"var_name" : "bdt", "bins" : 15, "xlow" : 0., "xup" : 1., "xtitle" : 0.}], inpath = "bdt")
        
    if do_fit:
        fit_xsec(var = "bdt", file_name = "bdt")
        #fit_xsec(var = "MET_met")
        



