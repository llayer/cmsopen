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
def create_config(path, fit_var, bins):
    
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
    config.update({
       "Samples":[
          {
             "Name": "Data",
             "Tree": "tree",
             "SamplePath": "Data.root",
             "Data": True                       # observed data is handled differently, need to distinguish
          },
          {
             "Name": "Signal",
             "Tree": "tree",
             "SamplePath": "TTJets_signal.root",
             "Weight": "weight"
          },
          {
             "Name": "QCD",
             "Tree": "tree",
             "SamplePath": "QCD.root",
             "Weight": "weight"
          },
          {
             "Name": "TTJets_bkg",
             "Tree": "tree",
             "SamplePath": "TTJets_bkg.root",
             "Weight": "weight"
          },
          {
             "Name": "WZJets",
             "Tree": "tree",
             "SamplePath": "WZJets.root",
             "Weight": "weight"
          },
          {
             "Name": "STJets",
             "Tree": "tree",
             "SamplePath": "STJets.root",
             "Weight": "weight"
          }

       ]
    })
    
    # Shape systematics
    config.update({"Systematics":[
        {
        "Name": "JES",
            "Up": {"SamplePath": "TTJets_signal_06_jes_up.root", 
                   #"Tree": "tree", "Weight": "weight"
                  },
            "Down": {"SamplePath": "TTJets_signal_06_jes_down.root", 
                     #"Tree": "tree", "Weight": "weight"
                    },
            "Type" : "NormPlusShape",
            "Samples": "Signal"
        },
        {
        "Name": "btag",
            "Up": {"SamplePath": "TTJets_signal_btag_up.root", 
                   #"Tree": "tree",
                   #"Weight": "btag_up"
                  },
            "Down": {"SamplePath": "TTJets_signal_btag_down.root", 
                     #"Tree": "tree",
                     #"Weight": "btag_down"
                    },
            "Type" : "NormPlusShape",
            "Samples": "Signal"
        },
        
        #{
        #    "Name": "Lumi",
        # 'Up': {'Normalization': 0.05},
        #  'Down': {'Normalization': -0.05},
        #  'Type': 'Normalization',
           #"Samples": ["Signal", "QCD"]
        #}
        
        ]
        }
    )
    
    # Rate parameters
    config.update({
       "NormFactors":[
          {
             "Name": "Signal_norm",
             "Samples": "Signal",    # we want this parameter to scale the signal
             "Nominal": 1,
             "Bounds": [-5, 10]
          },
          {
             "Name": "QCD_norm",
             "Samples": "QCD",    # we want this parameter to scale the signal
             "Nominal": 1,
             "Bounds": [0.5, 1.5]
          }
       ]
    })

    if cabinetry.configuration.validate(config):
        return config
    else:
        print("Config not valid")
        
        
        
        