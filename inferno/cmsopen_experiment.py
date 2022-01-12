import inferno_opendata
import inferno_config
import numpy as np

# Load the default config of the training
args = inferno_config.args
args["use_softhist"] = False
args["fit_sorted"] = False
args["s_norm_syst"] = []#["tau_trigger"]#["lumi"]#["lumi", "tau_trigger"] #["lumi", 'tau_trigger']
args["b_norm_syst"] = []#["mistag"] #[]
args["weight_syst"] = []#["btag"]#["pdf"] #,
args["add_pdf_weights"] = False
args["scale_shape_norms"] = None#[("jes", 0.05)]
args["scale_norms_only"] = None #[("lumi", 2.)]
args["ignore_shape_norm"] = False
args["asymm_shape_norm"] = True
args["inferno_neurons"] = 60
args["temperature"] = 0.1
args["store_significance"] = False
args["interp_algo"] = "default"#"fast_vertical"
args["fit_data"] = True

epochs = 50

# Scan the shape syst
"""
for syst in ["jes", "jer", "taue"]:
    
    if syst == 'jes':
        continue
    
    path = "/home/centos/mount_point/data/shape_syst/"
    shape_args = args.copy()
    shape_args["outpath"] = path + syst
    shape_args["shape_syst"] = [syst]
    samples = inferno_opendata.run_cmsopen(shape_args, epochs = epochs, do_fit = True)
"""
    
# Scan the weight syst
"""
for syst in ["btag", "trigger_jet", "trigger_tau"]:
    
    path = "/home/centos/mount_point/data/weight_syst/"
    weight_args = args.copy()
    weight_args["outpath"] = path + syst
    weight_args["shape_syst"] = []
    weight_args["weight_syst"] = [syst]
    samples = inferno_opendata.run_cmsopen(weight_args, epochs = epochs, do_fit = True)
"""
# Most promising

# JES, TauE, Trigger

# Final run with all

# JES, TauE, Trigger, Rate param

# Try one of the previous for the full fit...
# Full fit with all variables
# Eval data