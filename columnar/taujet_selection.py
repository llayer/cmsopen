import pandas as pd
import numpy as np
import selection

data = ['Run2011A_MultiJet', 'Run2011B_MultiJet']
mc = [ 'TTJets', 'T_TuneZ2_s', 'WJetsToLNu', 'DYJetsToLL', 'T_TuneZ2_tW', 'T_TuneZ2_t-channel',
       'Tbar_TuneZ2_s', 'Tbar_TuneZ2_tW', 'Tbar_TuneZ2_t-channel']
corrections = [ "jes_up", "jes_down", "jes_old_up", "jes_old_down", "jer_up", "jer_down", 
                 "taue_up", "taue_down", "met_up", "met_down"]


def rearrange_mc(samples):
    
    # Concat and split MC in signal and background
    new_samples = {}
    new_samples["TTJets_signal"] = samples["TTJets"][samples["TTJets"]["isSignal"] == True]
    new_samples["TTJets_bkg"] = samples["TTJets"][samples["TTJets"]["isBkg"] == True]
    new_samples["WZJets"] = pd.concat([samples['WJetsToLNu'], samples['DYJetsToLL']], axis=0)
    new_samples["STJets"] = pd.concat([samples['T_TuneZ2_s'], samples['T_TuneZ2_tW'], samples['T_TuneZ2_t-channel'], 
                              samples['Tbar_TuneZ2_s'], samples['Tbar_TuneZ2_tW'], 
                              samples['Tbar_TuneZ2_t-channel']], axis=0)

    # Concat the data
    #new_samples["Data"] = pd.concat([samples["Run2011A_MultiJet"], samples["Run2011B_MultiJet"]], axis=0)
    #new_samples["QCD"] = pd.concat([samples["Run2011A_MultiJet_QCD"], samples["Run2011B_MultiJet_QCD"]], axis=0)
    
    return new_samples


def nominal(outpath):
    
    cut_flow = []
    samples = {}
    
    """
    # Process data
    for sample in data:
        
        path = "/eos/user/l/llayer/opendata_files/preselection_merged/" + sample + ".root"
        for inv_btag, suffix in zip([False, True], ["", "_QCD"]):
            df, _, _, ev_count = selection.event_selection(path, isData = True, isTT = False, 
                                                                      invert_btag = inv_btag)
            samples[sample + suffix] = df
            ev_count["sample"] = sample + suffix
            cut_flow.append(ev_count)     
    """
        
    # Process nominal MC  
    for sample in mc:
        
        if "TTJets" in sample: isTT = True
        else: isTT = False
            
        path = "/eos/user/l/llayer/opendata_files/preselection_merged/" + sample + ".root"
        
        df, _, _, ev_count = selection.event_selection(path, isData = False, isTT = isTT, corrLevel="centJER")
        samples[sample] = df
        ev_count["sample"] = sample
        cut_flow.append(ev_count)        
    
    print("Done")
    
    # Save data
    # Cutflow
    pd.DataFrame.from_dict(cut_flow).to_hdf(outpath + "/cutflow.h5", "frame")
    # Rearange samples
    print("arranging samples")
    new_samples = rearrange_mc(samples)
    new_samples["Data"] = pd.concat([samples["Run2011A_MultiJet"], samples["Run2011B_MultiJet"]], axis=0)
    new_samples["QCD"] = pd.concat([samples["Run2011A_MultiJet_QCD"], samples["Run2011B_MultiJet_QCD"]], axis=0)
    for s, df in new_samples.items():
        df.to_hdf(outpath + "/" + s + ".h5", "frame")
    
    
def syst(outpath):       
    
    # TODO - implement MET correclty in JETMET to respect 4vec variations!
    
    for corr in corrections:
        samples = {}
        for sample in mc:

            if "TTJets" in sample: isTT = True
            else: isTT = False

            path = "/eos/user/l/llayer/opendata_files/preselection_merged/" + sample + ".root"
            df, _, _, _ = selection.event_selection(path, isData = False, isTT = isTT, corrLevel=corr)
            samples[sample] = df  
            
        samples = rearrange_mc(samples)
        for s, df in samples.items():
            df.to_hdf(outpath + "/" + s + "_"+ corr + ".h5", "frame")
        
        
def syst_variations(outpath):
    
    # Vary the size of jes / tau scale
    for c in ["jes_up", "jes_down"]:
        for scale in [0.03, 0.06, 0.09]:
            samples = {}
            for sample in mc:

                if "TTJets" in sample: isTT = True
                else: isTT = False
                path = "/eos/user/l/llayer/opendata_files/preselection_merged/" + sample + ".root"
                df, _, _, _ = selection.event_selection(path, isData = False, isTT = isTT, corrLevel = c, 
                                                         jes_factor = scale)
                samples[sample] = df  
            samples = rearrange_mc(samples)
            for s, df in samples.items():
                df.to_hdf(outpath + "/" + s + "_" + str(scale)[2:] + "_" + c +".h5", "frame") 
    
    for c in ["taue_up", "taue_down"]:
        for scale in [0.05, 0.07, 0.09]:
            
            samples = {}
            for sample in mc:

                if "TTJets" in sample: isTT = True
                else: isTT = False
                path = "/eos/user/l/llayer/opendata_files/preselection_merged/" + sample + ".root"
                df, _, _, _ = selection.event_selection(path, isData = False, isTT = isTT, corrLevel = c, 
                                                         tau_factor = scale)
                samples[sample] = df  
            samples = rearrange_mc(samples)
            for s, df in samples.items():
                df.to_hdf(outpath + "/" + s + "_" + str(scale)[2:] + "_" + c +".h5", "frame")                    
    
