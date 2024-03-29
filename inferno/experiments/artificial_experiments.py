import sys
sys.path.insert(0,'..')

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
args["shape_syst"] = []
args["add_pdf_weights"] = False
args["scale_shape_norms"] = None#[("jes", 0.05)]
args["scale_norms_only"] = None #[("lumi", 2.)]
args["ignore_shape_norm"] = False
args["asymm_shape_norm"] = True
args["inferno_neurons"] = 60
args["temperature"] = 0.1
args["store_significance"] = False
args["interp_algo"] = "default"#"fast_vertical"
args["bce_bins"] = 15
args["fit_floatQCD"] = False
args["fit_data"] = True

basepath = "/home/centos/mount_point/data/artificial_experiments/"
epochs = 100
exp = "no_syst"

# No systematic uncertainties
if exp == "no_syst":
    nosyst_args = args.copy()
    path = basepath  + "nosyst/"
    nosyst_args["outpath"] = path
    samples = inferno_opendata.run_cmsopen(nosyst_args, epochs = epochs, do_fit = True)

# Scan bins for BCE and INF
if exp == "bins":
    for b in np.linspace(6,40, 20): #[32, 34, 36, 38, 40]: #[6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30]
        nosystbin_args = args.copy()
        path = basepath  + "bins_nosyst/bin_" + str(b) + "/"
        nosystbin_args["outpath"] = path
        nosystbin_args["bce_bins"] = b
        nosystbin_args["inferno_bins"] = b
        samples = inferno_opendata.run_cmsopen(nosystbin_args, epochs = epochs, do_fit = True)

# Rate parameter
if exp == "rate":     
    rate_param_args = args.copy()
    path = basepath + "rate_param/"
    rate_param_args["outpath"] = path
    rate_param_args["b_rate_param"] = True
    rate_param_args["fit_floatQCD"] = True
    samples = inferno_opendata.run_cmsopen(rate_param_args, epochs = epochs, do_fit = True)
    
    
# Artificial shift with 10 models
if exp == "ten_shifts":
    for i in range(10):
        shifts = np.linspace(0.005, 0.02, 5)
        path = basepath + "ten_shifts/"
        args_tenshift  = args.copy()
        args_tenshift["outpath"] = path + "shift_" + str(i)
        args_tenshift["artificial_syst"] = {"TTJets_signal": [{'name':"aplanarity", 'shift':shifts[2], 'norm':0.05}]}
        args_tenshift["shape_syst"] = ["artsig_aplanarity"]
        samples = inferno_opendata.run_cmsopen(args_tenshift, epochs = epochs, do_fit = True)


# Retrain most promising with 10 models
if exp == "ten_all":
    for i in range(10):
        path = basepath + "ten_all/"
        args_tenshift  = args.copy()
        args_tenshift["outpath"] = path + "run_" + str(i)
        args_tenshift["bce_bins"] = 15
        args_tenshift["fit_floatQCD"] = False
        args_tenshift["fit_data"] = True
        args_tenshift["artificial_syst"] = None #{"TTJets_signal": [{'name':"aplanarity", 'shift':shifts[2], 'norm':0.05}]}
        args_tenshift["weight_syst"] = ["pdf", "btag", "trigger"]
        args_tenshift["shape_syst"] = ["jes", "jer", "taue"]
        try:
            samples = inferno_opendata.run_cmsopen(args_tenshift, epochs = epochs, do_fit = True)
        except:
            print("Run failed")

# Artificial shift of signal
if exp == "artsig_shift":
    for i, val in enumerate(np.linspace(0.005, 0.02, 5)):

        path = basepath + "artificial_sigshift/"
        args_sigshift  = args.copy()
        args_sigshift["outpath"] = path + "shift_" + str(i)
        args_sigshift["artificial_syst"] = {"TTJets_signal": [{'name':"aplanarity", 'shift':val, 'norm':0.05}]}
        args_sigshift["shape_syst"] = ["artsig_aplanarity"]
        samples = inferno_opendata.run_cmsopen(args_sigshift, epochs = epochs, do_fit = True)


# Artificial shift of bkg
if exp == "artbkg_shift":
    for i, val in enumerate(np.linspace(0.005, 0.02, 5)):
        path = basepath + "artificial_bkgshift/"
        art_bkg_args = args.copy()
        art_bkg_args["outpath"] = path + "shift_" + str(i)
        art_bkg_args["artificial_syst"] = {"QCD": [{'name':"aplanarity", 'shift':val, 'norm':0.05}]}
        art_bkg_args["shape_syst"] = ["artbkg_aplanarity"]
        samples = inferno_opendata.run_cmsopen(art_bkg_args, epochs = epochs, do_fit = True)

# Artificial shift of signal with three nuisances
if exp == "artsig_nnuis":
    for i in range(3):

        nuis_var = ["artsig_aplanarity", "artsig_MET_met", "artsig_deltaPhiTauMet"]
        nuis = nuis_var[0:i+1]

        print( nuis )

        path = basepath + "artificial_nnuis/"

        art_nuis_args = args.copy()
        art_nuis_args["outpath"] = path + "nnuis_" + str(i)
        art_nuis_args["artificial_syst"] = {"TTJets_signal": [{'name':"aplanarity", 'shift':0.01, 'norm':0.05},
                                                             {'name':"MET_met", 'shift':20, 'norm':0.05},
                                                             {'name':"deltaPhiTauMet", 'shift':0.05, 'norm':0.05}]
                                           }
        art_nuis_args["shape_syst"] = nuis
        samples = inferno_opendata.run_cmsopen(art_nuis_args, epochs = epochs, do_fit = True)

        
# Artificial shift of signal and background
if exp == "artsigbkg_shift":
    for i, val in enumerate(np.linspace(0.005, 0.02, 5)):

        path = basepath + "artificial_sigbkg/"
        art_sigbkg_args = args.copy()
        art_sigbkg_args["outpath"] = path + "shift_" + str(i)
        art_sigbkg_args["artificial_syst"] = {"TTJets_signal": [{'name':"aplanarity", 'shift':val, 'norm':0.05}],
                                           "QCD": [{'name':"aplanarity", 'shift':val, 'norm':0.05}]}
        art_sigbkg_args["shape_syst"] = ["artsig_aplanarity", "artbkg_aplanarity"]
        samples = inferno_opendata.run_cmsopen(art_sigbkg_args, epochs = epochs, do_fit = True)

# Artificial shift of signal norm
if exp == "artsig_norm":
    for i, val in enumerate(np.linspace(1., 5., 5)):
        path = basepath + "artificial_signorm/"
        signorm_args = args.copy()
        signorm_args["outpath"] = path + "norm_" + str(i)
        signorm_args["shape_syst"] = []
        signorm_args["s_norm_syst"] = ["lumi"]
        signorm_args["scale_norms_only"] = [("lumi", val)]
        samples = inferno_opendata.run_cmsopen(signorm_args, epochs = epochs, do_fit = True)

# Artificial shift of bkg norm
if exp == "artbkg_norm":
    for i, val in enumerate(np.linspace(1., 5., 5)):

        path = basepath + "artificial_bkgnorm/"
        bkgnorm_args = args.copy()
        bkgnorm_args["outpath"] = path + "norm_" + str(i)
        bkgnorm_args["shape_syst"] = []
        bkgnorm_args["b_norm_syst"] = ["mistag"]
        bkgnorm_args["scale_norms_only"] = [("mistag", val)]
        samples = inferno_opendata.run_cmsopen(bkgnorm_args, epochs = epochs, do_fit = True)        

# Scan the shape syst
if exp == "scan_shape":
    for syst in ["jes", "jer", "taue"]:

        path = basepath + "shape_syst/"
        shape_args = args.copy()
        shape_args["outpath"] = path + syst
        shape_args["shape_syst"] = [syst]
        samples = inferno_opendata.run_cmsopen(shape_args, epochs = epochs, do_fit = True)

    
# Scan the weight syst
if exp == "scan_weight":

    for syst in ["btag", "trigger_jet", "trigger_tau"]:
        path = basepath + "weight_syst/"
        weight_args = args.copy()
        weight_args["outpath"] = path + syst
        weight_args["shape_syst"] = []
        weight_args["weight_syst"] = [syst]
        samples = inferno_opendata.run_cmsopen(weight_args, epochs = epochs, do_fit = True)

# Run most impact
if exp == "most_impact":   
    # JES, TauE, Trigger
    path = basepath + "most_impact/"
    most_impact_args = args.copy()
    most_impact_args["outpath"] = path
    most_impact_args["shape_syst"] = ["jes", "taue"]
    most_impact_args["weight_syst"] = ["trigger_jet"]
    samples = inferno_opendata.run_cmsopen(most_impact_args, epochs = epochs, do_fit = True)

    # JES, TauE, Trigger rate
    path = basepath + "most_impact_rate/"
    most_impact_rate_args = args.copy()
    most_impact_rate_args["outpath"] = path
    most_impact_rate_args["shape_syst"] = ["jes", "taue"]
    most_impact_rate_args["weight_syst"] = ["trigger_jet"]
    most_impact_rate_args["b_rate_param"] = True
    most_impact_rate_args["fit_floatQCD"] = True
    samples = inferno_opendata.run_cmsopen(most_impact_rate_args, epochs = epochs, do_fit = True)

# Run with all
if exp == "all":
    path = basepath + "all/"
    all_args = args.copy()
    all_args["outpath"] = path
    all_args["shape_syst"] = ["jes", "jer", "taue"]
    all_args["weight_syst"] = ["btag", "trigger_jet", "trigger_tau"]
    all_args["s_norm_syst"] = ["lumi"]#["tau_trigger"]#["lumi"]#["lumi", "tau_trigger"] #["lumi", 'tau_trigger']
    all_args["b_norm_syst"] = ["mistag"] #[]
    #all_args["b_rate_param"] = True
    #all_args["fit_floatQCD"] = True
    samples = inferno_opendata.run_cmsopen(all_args, epochs = epochs, do_fit = True)


# Full fit with all variables
# Eval data
# JES, TauE, Trigger fit
if exp == "fit_most_impact":
    path = basepath + "most_impact/"
    most_impact_args = args.copy()
    most_impact_args["sample_path"] = path
    most_impact_args["outpath"] = path + "fit_complete_floatqcd/"
    most_impact_args["shape_syst"] = ["jes", "taue"]
    most_impact_args["weight_syst"] = ["trigger_jet"]
    most_impact_args["fit_shape_systs"] = ["btag", "jer", "trigger_tau"]
    most_impact_args["fit_norm_syst"] = ["lumi", "tau_id", "xsec", "tau_trigger", "ttmass", "ttq2", "ttparton"] 
    most_impact_args["fit_model"] = "sig_bkg"
    most_impact_args["fit_floatQCD"] = True
    most_impact_args["add_pdf_weights"] = False
    args["fit_data"] = True
    args["add_stat_only"] = False
    samples = inferno_opendata.run_cmsopen(most_impact_args, epochs = epochs, retrain=False, do_fit = True)

if exp == "fit_all":

    #path = basepath + "shape_syst/jes/"
    path = basepath + "all/"#"most_impact_rate/"
    most_impact_args = args.copy()
    most_impact_args["sample_path"] = path
    most_impact_args["outpath"] = path + "fit_trigger/"
    #most_impact_args["shape_syst"] = ["jes", "taue"]
    #most_impact_args["weight_syst"] = ["trigger_jet"]
    most_impact_args["fit_shape_systs"] = ["jes", "taue", "btag", "jer", "trigger"]
    most_impact_args["fit_norm_syst"] = ["lumi", "tau_id", "xsec", "tau_trigger", "ttmass", "ttq2", "ttparton", "mistag"] 
    most_impact_args["fit_model"] = "sig_bkg"
    most_impact_args["fit_floatQCD"] = True
    most_impact_args["add_pdf_weights"] = True
    most_impact_args["fit_data"] = True
    most_impact_args["add_stat_only"] = True
    samples = inferno_opendata.run_cmsopen(most_impact_args, epochs = epochs, retrain=False, do_fit = True)

