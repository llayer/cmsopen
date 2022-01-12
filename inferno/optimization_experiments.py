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
args["run_skopt"] = True
args["bce_bins"] = 15
args["fit_floatQCD"] = True
args["fit_data"] = True

basepath = "/home/centos/mount_point/data/artificial_experiments/"
epochs = 150

nosyst_args = args.copy()
path = basepath + "optimizatio_nosyst/"
nosyst_args["outpath"] = path
nosyst_args["shape_syst"] = []
samples = inferno_opendata.run_cmsopen(nosyst_args, epochs = epochs, do_fit = True)


