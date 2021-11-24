import cabinetry
import pyhf
import uproot3
import numpy as np


#
# Convert data to ROOT format
#

def create_tree(path, s, sample):
    
    file = uproot3.recreate(path + "/root_trees/" + s + ".root")
    file["tree"] = uproot3.newtree(
        {
        "inferno": np.float32,
        "inferno_sorted": np.float32,
        "bce": np.float32,
        "weight": np.float32,    
        "event": np.float32
        }
    )    
    file["tree"].extend({
        "inferno": sample["inferno"].values ,
        "inferno_sorted": sample["inferno_sorted"].values ,
        "bce": sample["bce"].values ,
        "weight": sample["weight"].values,
        "event": sample["event"].values
    })
    file.close()    

    
def to_root(samples, systs = ["btag"], path = "/home/centos/data/inferno_cmsopen13/root_trees"):
    
    for s in samples:
        
        sample = samples[s]
        if s == "Data":
            sample["weight"] = 1.
            create_tree(path, s, sample)
        elif s == "QCD":
            scale_qcd = 4.
            sample["weight"]  = sample['btag_weight2'] * scale_qcd
            create_tree(path, s, sample)
        else:
            
            sample["weight"] = sample['norm'] * sample['trigger_weight'] * sample['btag_weight1']
            create_tree(path, s, sample)
            if ('up' in s) | ('down' in s): continue
            for syst in systs:
                                
                for ud in ["up", "down"]:
                    if syst == "btag":
                        sample["weight"] = sample['norm'] * sample['trigger_weight'] * sample['btag_weight1_' + ud]
                        create_tree(path,  s + "_" + syst + "_" + ud, sample)
                    if syst == "trigger":
                        sample["weight"] = sample['norm'] * sample['trigger_weight_'+ud] * sample['btag_weight1']
                        create_tree(path,  s + "_" + syst + "_" + ud, sample)
    
                if (s == "TTJets_signal") & (syst == "pdf"):
                    # PDF Up
                    sample["pdf_up"] = sample["pdf_up"].fillna(0.)
                    sample["weight"] = sample['norm'] * sample['trigger_weight'] * sample['btag_weight1'] * (1+sample["pdf_up"])
                    create_tree(path,  s + "_" + syst + "_up", sample)
                    # PDF Down
                    sample["pdf_down"] = sample["pdf_down"].fillna(0.)
                    sample["weight"] = sample['norm'] * sample['trigger_weight'] * sample['btag_weight1'] * (1-sample["pdf_down"])
                    create_tree(path,  s + "_" + syst + "_down", sample)
                                                                                                 
#
# Write and fit workspace
#

def create_ws(config, workspace_path = "", postproc=True):
    cabinetry.templates.build(config, method="uproot")
    cabinetry.templates.postprocess(config)
    ws = cabinetry.workspace.build(config)
    cabinetry.workspace.save(ws, workspace_path)
    spec = dict(pyhf.Workspace(ws).prune(modifier_types=["staterror"]))
    return spec
    
    
def fit_ws(ws, config, asimov = True):
    
    model, data = cabinetry.model_utils.model_and_data(ws, asimov=asimov)
    model_pred = cabinetry.model_utils.prediction(model)
    figures = cabinetry.visualize.data_mc(model_pred, data, config=config, log_scale=True)
    fit_results = cabinetry.fit.fit(model, data)
    return fit_results
                        
#
# Create config
#
   
    
def add_samples(sample_names):
    
    samples = []
    
    for s in sample_names:
        
        if "Data" in s:
            samples.append(
              {
                 "Name": "Data",
                 "Tree": "tree",
                 "SamplePath": "Data.root",
                 "Data": True                       # observed data is handled differently, need to distinguish
              }
            )
 
        else:
            samples.append(
            {
             "Name": s,
             "Tree": "tree",
             "SamplePath": s+".root",
             "Weight": "weight"
            }
            )
        
    return{"Samples" : samples} 
    
def add_syst(corr_shape_systs, uncorr_shape_systs, norm_syst):
    
    systs = []
    for sample in corr_shape_systs:
        for syst in corr_shape_systs[sample]:
            systs.append(
                {
                "Name": sample+ "_" + syst,
                    "Up": {"SamplePath": sample + "_" + syst + "_up.root"},
                    "Down": {"SamplePath": sample + "_" + syst + "_down.root"},
                    "Type" : "NormPlusShape",
                    "Samples": sample,
                    "ModifierName": syst
                }        
            )
            
    for sample in uncorr_shape_systs:
        for syst in uncorr_shape_systs[sample]:
            systs.append(
                {
                "Name": sample+ "_" + syst,
                    "Up": {"SamplePath": sample + "_" + syst + "_up.root"},
                    "Down": {"SamplePath": sample + "_" + syst + "_down.root"},
                    "Type" : "NormPlusShape",
                    "Samples": sample,
                }        
            )    
            
    for syst in norm_syst:
        
            systs.append(
                {
                "Name": "Lumi",
                'Up': {'Normalization': norm_syst[syst]["value"]},
                'Down': {'Normalization': -norm_syst[syst]["value"]},
                'Type': 'Normalization',
                "Samples": norm_syst[syst]["samples"]
                }        
            )          
            
    return{"Systematics" : systs} 


def create_config(path, fit_var, bins, sample_names, corr_shape_systs={}, uncorr_shape_systs={}, norm_syst={}, float_qcd=True):
    
    # General setup
    config = {
       "General":{
          "Measurement": "CMSOpenData",
          "POI": "Signal_norm",               # parameter of interest, which we want to measure 
          "InputPath": path +"/root_trees/{SamplePath}", # where to find input data
          "HistogramFolder": path + "/histograms/"
       }
    }
    
    # Region and fit variable
    config.update({
   "Regions":[
      {
         "Name": "Signal_region",
         "Variable": fit_var,                 # which variable we bin histograms in
         "Binning": list(bins), #[0,0.1,0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.],
         #"Filter": "bdt > -1"           # event selection 
          }
       ]
    })
    
    # Samples
    config.update(add_samples(sample_names))
    
    # Shape systematics
    config.update(add_syst(corr_shape_systs, uncorr_shape_systs, norm_syst))
    
    # Rate parameters
    config.update({
       "NormFactors":[
          {
             "Name": "Signal_norm",
             "Samples": "TTJets_signal",    # we want this parameter to scale the signal
             "Nominal": 1,
             "Bounds": [-5, 10]
          }
       ]
    })
    
    if float_qcd:
        config["NormFactors"].append(
              {
             "Name": "QCD_norm",
             "Samples": "QCD",   
             "Nominal": 1,
             "Bounds": [0.5, 1.5]
              }
        )

    if cabinetry.configuration.validate(config):
        return config
    else:
        print("Config not valid")
        
        
        
        