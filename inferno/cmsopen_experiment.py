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
for syst in ["jes", "jer", "taue"]:
    
    path = "/home/centos/mount_point/data/shape_syst/"

    args["outpath"] = path + syst
    args["shape_syst"] = [syst]
    samples = inferno_opendata.run_cmsopen(args, epochs = epochs, do_fit = True)
    
    
# Scan the weight syst
for syst in ["btag", "trigger"]:
    
    path = "/home/centos/mount_point/data/weight_syst/"

    args["outpath"] = path + syst
    args["shape_syst"] = [syst]
    samples = inferno_opendata.run_cmsopen(args, epochs = epochs, do_fit = True)

# Most promising

# Final run with all

# Full fit with all variables
# Eval data