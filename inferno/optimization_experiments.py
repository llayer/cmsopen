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
args["run_skopt"] = False
args["bce_bins"] = 15
args["fit_floatQCD"] = True
args["fit_data"] = True

basepath = "/home/centos/mount_point/data/artificial_experiments/"
epochs = 100

"""
nosyst_args = args.copy()
path = basepath + "optimizatio_nosyst/"
nosyst_args["outpath"] = path
nosyst_args["shape_syst"] = []
nosyst_args["run_skopt"] = True
samples = inferno_opendata.run_cmsopen(nosyst_args, epochs = epochs, do_fit = True)
"""

# optimize bins as well -- missing

# optimize JES with grid search
lr_param = [0.0001, 0.001, 0.01]
neurons_param = [20, 40, 60, 80, 100]
temperature_param = [0.01, 0.1, 0.5, 0.9, 0.99]

"""
i_exp = 0
for lr in lr_param:
    for neurons in neurons_param:
        for temp in temperature_param:
            print(lr, neurons, temp)

            all_args = args.copy()
            path = basepath + "optimization_pdftauejes/"
            all_args["outpath"] = path + "run_" + str(i_exp)
            all_args["shape_syst"] = ["jes"]
            all_args["inferno_lr"] = lr
            all_args["inferno_neurons"] = neurons
            all_args["temperature"] = temp
            all_args["bce_bins"] = 20
            all_args["fit_floatQCD"] = False
            all_args["fit_data"] = True
            all_args["artificial_syst"] = None #{"TTJets_signal": [{'name':"aplanarity", 'shift':shifts[2], 'norm':0.05}]}
            all_args["weight_syst"] = ["pdf"]
            all_args["shape_syst"] = ["jes", "taue"]
            try:
                samples = inferno_opendata.run_cmsopen(all_args, epochs = epochs, do_fit = True)
            except:
                print("Run failed")
            i_exp += 1

"""

"""
i_exp = 0
for lr in lr_param:
    for neurons in neurons_param:
        for temp in temperature_param:
            print(lr, neurons, temp)

            all_args = args.copy()
            path = basepath + "optimization_all/"
            all_args["outpath"] = path + "run_" + str(i_exp)
            all_args["shape_syst"] = ["jes"]
            all_args["inferno_lr"] = lr
            all_args["inferno_neurons"] = neurons
            all_args["temperature"] = temp
            all_args["bce_bins"] = 15
            all_args["fit_floatQCD"] = True
            all_args["fit_data"] = True
            all_args["artificial_syst"] = None #{"TTJets_signal": [{'name':"aplanarity", 'shift':shifts[2], 'norm':0.05}]}
            all_args["weight_syst"] = ["pdf", "btag", "trigger"]
            all_args["shape_syst"] = ["jes", "jer", "taue"]
            try:
                samples = inferno_opendata.run_cmsopen(all_args, epochs = epochs, do_fit = True)
            except:
                print("Run failed")
            i_exp += 1
"""

"""
i_exp = 0
for lr in lr_param:
    for neurons in neurons_param:
        for temp in temperature_param:
            print(lr, neurons, temp)

            jes_syst_args = args.copy()
            path = basepath + "optimizatio_jes/"
            jes_syst_args["outpath"] = path + "run_" + str(i_exp)
            jes_syst_args["shape_syst"] = ["jes"]
            jes_syst_args["inferno_lr"] = lr
            jes_syst_args["inferno_neurons"] = neurons
            jes_syst_args["temperature"] = temp
            try:
                samples = inferno_opendata.run_cmsopen(jes_syst_args, epochs = epochs, do_fit = True)
            except:
                print("Run failed")
            i_exp += 1
"""

"""
for i in range(75):
    #path = basepath + "shape_syst/jes/"
    path = basepath + "/optimizatio_jes/run_"+str(i)+"/" #"most_impact_rate/"
    most_impact_args = args.copy()
    most_impact_args["sample_path"] = path
    most_impact_args["outpath"] = path + "fit_complete_nnopdf/"
    #most_impact_args["shape_syst"] = ["jes", "taue"]
    #most_impact_args["weight_syst"] = ["trigger_jet"]
    most_impact_args["fit_shape_systs"] = ["jes", "taue", "btag", "jer", "trigger_jet", "trigger_tau"]
    most_impact_args["fit_norm_syst"] = ["lumi", "tau_id", "xsec", "tau_trigger", "ttmass", "ttq2", "ttparton"] 
    most_impact_args["fit_model"] = "sig_bkg"
    most_impact_args["fit_floatQCD"] = True
    most_impact_args["add_pdf_weights"] = False
    most_impact_args["fit_data"] = True
    most_impact_args["add_stat_only"] = True
    try:
        samples = inferno_opendata.run_cmsopen(most_impact_args, epochs = epochs, retrain=False, do_fit = True)
    except:
        print("Fit failed")
"""
"""
# Promising run 44
#path = basepath + "shape_syst/jes/"
path = basepath + "/optimizatio_jes/run_44/" #"most_impact_rate/"
most_impact_args = args.copy()
most_impact_args["sample_path"] = path
most_impact_args["outpath"] = path + "fit_complete_impacts/"
#most_impact_args["shape_syst"] = ["jes", "taue"]
#most_impact_args["weight_syst"] = ["trigger_jet"]
most_impact_args["fit_shape_systs"] = ["jes", "taue", "btag", "jer", "trigger_jet", "trigger_tau"]
most_impact_args["fit_norm_syst"] = ["lumi", "tau_id", "xsec", "tau_trigger", "ttmass", "ttq2", "ttparton"] 
most_impact_args["fit_model"] = "sig_bkg"
most_impact_args["fit_floatQCD"] = True
most_impact_args["add_pdf_weights"] = True
most_impact_args["fit_data"] = True
most_impact_args["add_stat_only"] = True
samples = inferno_opendata.run_cmsopen(most_impact_args, epochs = epochs, retrain=False, do_fit = True)
"""

"""
# Promising run 44 - try with single pdf param
#path = basepath + "shape_syst/jes/"
path = basepath + "/optimizatio_jes/run_44/" #"most_impact_rate/"
most_impact_args = args.copy()
most_impact_args["sample_path"] = path
most_impact_args["outpath"] = path + "fit_single_pdf/"
#most_impact_args["shape_syst"] = ["jes", "taue"]
#most_impact_args["weight_syst"] = ["trigger_jet"]
most_impact_args["fit_shape_systs"] = ["jes", "taue", "btag", "jer", "trigger_jet", "trigger_tau"]
most_impact_args["fit_norm_syst"] = ["lumi", "tau_id", "xsec", "tau_trigger", "ttmass", "ttq2", "ttparton"] 
most_impact_args["fit_model"] = "sig_bkg"
most_impact_args["fit_floatQCD"] = True
most_impact_args["add_pdf_weights"] = True
most_impact_args["fit_data"] = True
most_impact_args["add_stat_only"] = True
samples = inferno_opendata.run_cmsopen(most_impact_args, epochs = epochs, retrain=False, do_fit = True)
"""


"""
for i in range(75):
    #path = basepath + "shape_syst/jes/"
    path = basepath + "/optimization_all/run_"+str(i)+"/" #"most_impact_rate/"
    most_impact_args = args.copy()
    most_impact_args["sample_path"] = path
    most_impact_args["outpath"] = path + "fit_complete_bins20/"
    #most_impact_args["shape_syst"] = ["jes", "taue"]
    #most_impact_args["weight_syst"] = ["trigger_jet"]
    most_impact_args["fit_shape_systs"] = ["jes", "taue", "btag", "jer", "trigger"]
    most_impact_args["fit_norm_syst"] = ["lumi", "tau_id", "xsec", "tau_trigger", "ttmass", "ttq2", "ttparton"] 
    most_impact_args["fit_model"] = "sig_bkg"
    most_impact_args["fit_floatQCD"] = True
    most_impact_args["add_pdf_weights"] = True
    most_impact_args["fit_data"] = True
    most_impact_args["add_stat_only"] = True
    most_impact_args["bce_bins"] = 20
    #samples = inferno_opendata.run_cmsopen(most_impact_args, epochs = epochs, retrain=False, do_fit = True)
    try:
        samples = inferno_opendata.run_cmsopen(most_impact_args, epochs = epochs, retrain=False, do_fit = True)
    except:
        print("Fit failed")
"""
"""
path = "/home/centos/mount_point/data/pdf/" #"most_impact_rate/"
most_impact_args = args.copy()
most_impact_args["sample_path"] = path
most_impact_args["outpath"] = path + "fit_complete_taue_bins25/"
#most_impact_args["shape_syst"] = ["jes", "taue"]
#most_impact_args["weight_syst"] = ["trigger_jet"]
most_impact_args["fit_shape_systs"] = ["jes", "taue", "btag", "jer", "trigger"]
most_impact_args["fit_norm_syst"] = ["lumi", "tau_id", "xsec", "tau_trigger", "ttmass", "ttq2", "ttparton"] 
most_impact_args["fit_model"] = "sig_bkg"
most_impact_args["fit_floatQCD"] = True
most_impact_args["add_pdf_weights"] = True
most_impact_args["fit_data"] = True
most_impact_args["bce_bins"] = 25
most_impact_args["add_stat_only"] = True
#samples = inferno_opendata.run_cmsopen(most_impact_args, epochs = epochs, retrain=False, do_fit = True)
try:
    samples = inferno_opendata.run_cmsopen(most_impact_args, epochs = epochs, retrain=False, do_fit = True)
except:
    print("Fit failed")
    
"""

"""
for i in range(75):
    #path = basepath + "shape_syst/jes/"
    path = basepath + "/optimization_pdftauejes//run_"+str(i)+"/" #"most_impact_rate/"
    most_impact_args = args.copy()
    most_impact_args["sample_path"] = path
    most_impact_args["outpath"] = path + "fit_complete_bins20/"
    #most_impact_args["shape_syst"] = ["jes", "taue"]
    #most_impact_args["weight_syst"] = ["trigger_jet"]
    most_impact_args["fit_shape_systs"] = ["jes", "taue", "btag", "jer", "trigger"]
    most_impact_args["fit_norm_syst"] = ["lumi", "tau_id", "xsec", "tau_trigger", "ttmass", "ttq2", "ttparton"] 
    most_impact_args["fit_model"] = "sig_bkg"
    most_impact_args["fit_floatQCD"] = True
    most_impact_args["add_pdf_weights"] = True
    most_impact_args["fit_data"] = True
    most_impact_args["add_stat_only"] = True
    most_impact_args["bce_bins"] = 20
    #samples = inferno_opendata.run_cmsopen(most_impact_args, epochs = epochs, retrain=False, do_fit = True)
    try:
        samples = inferno_opendata.run_cmsopen(most_impact_args, epochs = epochs, retrain=False, do_fit = True)
    except:
        print("Fit failed")
"""   


 
#Run69 Basic fit for train vars
"""
all_args = args.copy()
path = basepath + "optimization_all/run_69/"
all_args["sample_path"] = path
all_args["outpath"] = path + "fit_final/"
all_args["bce_bins"] = 20
all_args["fit_floatQCD"] = True
all_args["fit_data"] = True
all_args["add_pdf_weights"] = True
all_args["artificial_syst"] = None #{"TTJets_signal": [{'name':"aplanarity", 'shift':shifts[2], 'norm':0.05}]}
all_args["weight_syst"] = ["pdf", "btag", "trigger"]
all_args["shape_syst"] = ["jes", "jer", "taue"]
samples = inferno_opendata.run_cmsopen(all_args, epochs = epochs, retrain=False, do_fit = True)
"""
#Run69 Basic fit for all vars
all_args = args.copy()
path = basepath + "optimization_all/run_69/"
all_args["sample_path"] = path
all_args["outpath"] = path + "fit_final_all/"
all_args["fit_shape_systs"] = ["jes", "taue", "btag", "jer", "trigger"]
all_args["fit_norm_syst"] = ["lumi", "tau_id", "xsec", "tau_trigger", "ttmass", "ttq2", "ttparton"] 
all_args["fit_model"] = "sig_bkg"
all_args["fit_floatQCD"] = True
all_args["add_pdf_weights"] = True
all_args["fit_data"] = True
all_args["add_stat_only"] = True
all_args["bce_bins"] = 20
samples = inferno_opendata.run_cmsopen(all_args, epochs = epochs, retrain=False, do_fit = True)


""" NoSYST
all_args = args.copy()
path = basepath + "nosyst/"
all_args["sample_path"] = path
all_args["outpath"] = path + "fit_final/"
all_args["bce_bins"] = 20
all_args["fit_floatQCD"] = True
all_args["fit_data"] = True
all_args["add_pdf_weights"] = True
all_args["artificial_syst"] = None #{"TTJets_signal": [{'name':"aplanarity", 'shift':shifts[2], 'norm':0.05}]}
all_args["weight_syst"] = []#"pdf", "btag", "trigger"]
all_args["shape_syst"] = []#"jes", "jer", "taue"]
samples = inferno_opendata.run_cmsopen(all_args, epochs = epochs, retrain=False, do_fit = True)
"""

