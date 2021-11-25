from preproc import get_norm

# Load norms
norm_syst = ["jes_06", "taue", "btag_weight1"]
mu, qcd, sig_norm = get_norm(norm_syst)

# Parameters for INFERNO training
args = {}

#
# General args
#
args["outpath"] = "/home/centos/data/inferno_cmsopen16"
args["store"] = True
args["features"] = ['aplanarity', 'chargeEta', 'MET_met', 'deltaPhiTauMet']
args["shape_syst"] = ["06_jes", "jer"] #, "taue"]
args["weight_syst"] = []#["btag_weight1"]
args["shape_norm_sigma"] = [0.05, 0.02] # CHECK adjust for correct values
# Common training args
args["bs"] = 1000 
args["n_sig"] = 20000
args["n_bkg"] = 5000
args["nfeat"] = len(args["features"])
args["bins"] = 10
args["use_weights"] = False
#
# INFERNO args
#
# Model parameter
args["inferno_lr"] = 1e-3
args["inferno_neurons"] = 100
args["temperature"] = 0.1
# Signal and bkg
args["b_true"] = qcd
args["mu_true"] = mu
# Number of shape and weight nuisances
args["n_shape_systs"] = len(args["shape_syst"] )
args["n_weight_systs"] = len(args["weight_syst"])
# Normalization standard error on the nuisance parameters 
args["s_norm_sigma"] = []
args["b_norm_sigma"] = []
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
args["bce_neurons"] = 12 # CHECK only affects first layer
#
# FIT args
#
args["fit_floatQCD"] = True
args["sample_names"] = ["Data", "QCD", "TTJets_bkg", "WZJets", "STJets", "TTJets_signal"]
args["mc"] = ["TTJets_bkg", "WZJets", "STJets", "TTJets_signal"]
args["corr_shape_systs"] = {"TTJets_signal" : ["btag"], "TTJets_bkg" : ["btag"]}
args["uncorr_shape_systs"] = {"TTJets_signal" : ["06_jes"]}
args["norm_syst"] ={}# {"lumi":{ "samples" : mc, "value" : 0.02 }, }
args["n_steps"] = 200







