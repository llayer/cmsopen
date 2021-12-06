# Normalization uncertainties

xsec_std = {
    'TTJets_bkg': 0.06097560975609759,
     'WZJets': 0.04770462700154329,
     'STJets': 0.0666983590353544,
     'TTJets_signal': 0.0609756097560975
}

norms = {"lumi": 0.02,
        "mistag":0.05,
        "tau_trigger":0.05,
        "tau_id":0.06,
        "ttmass":0.03,
        "ttq2":0.02,
        "ttparton":0.03,
        "xsec": xsec_std["TTJets_signal"] # returns by default xsec uncertainty of signal
        }

# Parameters for INFERNO training
args = {}

#
# General args
#
args["outpath"] = "/home/centos/mount_point/data/inferno_cmsopen0" #"/home/centos/data/inferno_cmsopen16"
args["store"] = True
args["features"] = ['aplanarity', 'chargeEta', 'MET_met', 'deltaPhiTauMet']
args["sample_names"] = ["Data", "QCD", "TTJets_bkg", "WZJets", "STJets", "TTJets_signal"]
args["mc"] = ["TTJets_bkg", "WZJets", "STJets", "TTJets_signal"]
args["exclude_train"] = False
# Shape systs can be jes, jer, taue
args["shape_syst"] = []#["06_jes", "07_taue"] #jer
# Weight syst can be btag, trigger, pdf
args["weight_syst"] = []#["btag_weight1"]
# Norm syst can be lumi, mistag, tau_trigger, tau_id, ttmass, ttq2, ttparton
args["s_norm_syst"] = ["lumi"]
args["b_norm_syst"] = ["mistag"]
# Scale syst
args["scale_norms_only"] = None #[("lumi", 2.)] #Pure norm syst
args["scale_shape_norms"] = None #[("jes", 0.05)]# None
# Ignore norms of shape
args["ignore_shape_norm"] = False
# Artificial systs
args["artificial_syst"] = None#{"TTJets_signal": [{'name':"aplanarity", 'shift':0.5, 'norm':0.05}]}
# Downsampling
args["downsample_factor"] = None
# Common training args
args["bs"] = 1000 
args["n_sig"] = 20000
args["n_bkg"] = 5000
args["bins"] = 10
args["use_weights"] = False
#
# INFERNO args
#
# Asymmetric shape uncertainties
args["asymm_shape_norm"] = False
#Sort the bins according to signal purity
args["fit_sorted"] = False
# Model parameter
args["inferno_lr"] = 1e-3
args["inferno_neurons"] = 100
args["temperature"] = 0.1
# Boolean whether to let the background float
args["b_rate_param"] = False
# Interpolation algo
args["interp_algo"] = "default"
# Soft hist instead of softmax
args["use_softhist"] = False
args["sigmoid_delta"] = 200.
#
# BCE args
#
args["bce_lr"] = 1e-3 #CHECK if correct
args["bce_neurons"] = 20 # CHECK only affects first layer
#
# FIT args
#
args["fit_asimov"] = True
args["fit_data"] = False
args["prune_stat"] = True
args["minos"] = ["mu"]
args["fit_sig_lim"] = False
args["print_ws"] = True
args["print_yields"] = True
args["fit_floatQCD"] = False
args["fit_model"] = "signal_only"
args["fit_shape_systs"] = []#{"TTJets_signal" : ["btag"], "TTJets_bkg" : ["btag"]}
args["fit_norm_syst"] = [] #["tau_trigger"]# {"lumi":{ "samples" : mc, "value" : 0.02 }, }
args["n_steps"] = 200
#
# Plotting style
#
args["create_gifs"] = False






