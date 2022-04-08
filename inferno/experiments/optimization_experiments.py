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

exp="fit_69"

# Skopt without systematics
if exp == "skopt_nosyst":
    nosyst_args = args.copy()
    path = basepath + "optimizatio_nosyst/"
    nosyst_args["outpath"] = path
    nosyst_args["shape_syst"] = []
    nosyst_args["run_skopt"] = True
    samples = inferno_opendata.run_cmsopen(nosyst_args, epochs = epochs, do_fit = True)



# optimize with multiple iter models
if exp == "opt_multiple_iter":
    lr_param = 0.001
    neurons_param = [20, 40, 60, 80, 100]
    temperature_param = [0.01, 0.1, 0.9]
    n_iter = 3

    i_exp = 0
    for neurons in neurons_param:
        for temp in temperature_param:
            #print(lr, neurons, temp)
            for i in range(n_iter):
                all_args = args.copy()
                path = basepath + "final_opt_all/"
                all_args["outpath"] = path + "opt_" + str(i_exp) + "_run" + str(i)
                all_args["shape_syst"] = ["jes"]
                all_args["inferno_lr"] = lr_param
                all_args["inferno_neurons"] = neurons
                all_args["temperature"] = temp
                all_args["bce_bins"] = 20
                all_args["fit_floatQCD"] = False
                all_args["fit_data"] = True
                all_args["artificial_syst"] = None #{"TTJets_signal": [{'name':"aplanarity", 'shift':shifts[2], 'norm':0.05}]}
                all_args["weight_syst"] = ["pdf", "btag", "trigger"]
                all_args["shape_syst"] = ["jes", "jer", "taue"]
                try:
                    samples = inferno_opendata.run_cmsopen(all_args, epochs = epochs, do_fit = True)
                except:
                    print("Run failed")
            i_exp += 1





lr_param = [0.0001, 0.001, 0.01]
neurons_param = [20, 40, 60, 80, 100]
temperature_param = [0.01, 0.1, 0.5, 0.9, 0.99]

# optimize most promising with grid search
if exp == "opt_most_prom":
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

# optimize all with grid search
if exp == "opt_all":
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
                
# optimize jes with grid search
if exp == "opt_jes":

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
                
                
# fit jes with grid search
if exp == "fit_jes":                
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

# Promising run 44           
if exp == "fit_44": 
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



# Fit of training with all syst           
if exp == "fit_all": 
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

            
# Fit of PDF run           
if exp == "fit_pdf": 
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
    

# Fit of training with all syst           
if exp == "fit_mostimpact": 
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
   


 
#Run69 Basic fit for train vars
if exp == "fit_69": 

    all_args = args.copy()
    path = basepath + "optimization_all/run_69/"
    all_args["sample_path"] = path
    all_args["outpath"] = path + "fit_final2/"
    all_args["bce_bins"] = 20
    all_args["fit_floatQCD"] = False
    all_args["fit_data"] = True
    all_args["add_pdf_weights"] = True
    all_args["artificial_syst"] = None #{"TTJets_signal": [{'name':"aplanarity", 'shift':shifts[2], 'norm':0.05}]}
    all_args["weight_syst"] = ["pdf", "btag", "trigger"]
    all_args["shape_syst"] = ["jes", "jer", "taue"]
    samples = inferno_opendata.run_cmsopen(all_args, epochs = epochs, retrain=False, do_fit = True)


#Run69 Basic fit for all vars
if exp == "fit_all69": 
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

# Fit without syst
if exp == "fit_nosyst": 
    all_args = args.copy()
    path = basepath + "nosyst/"
    all_args["sample_path"] = path
    all_args["outpath"] = path + "fit_final2/"
    all_args["bce_bins"] = 20
    all_args["fit_floatQCD"] = True
    all_args["fit_data"] = True
    all_args["add_pdf_weights"] = True
    all_args["artificial_syst"] = None #{"TTJets_signal": [{'name':"aplanarity", 'shift':shifts[2], 'norm':0.05}]}
    all_args["weight_syst"] = []#"pdf", "btag", "trigger"]
    all_args["shape_syst"] = []#"jes", "jer", "taue"]
    samples = inferno_opendata.run_cmsopen(all_args, epochs = epochs, retrain=False, do_fit = True)


