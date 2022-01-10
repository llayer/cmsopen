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
args["bce_bins"] = 15
args["fit_floatQCD"] = False
args["fit_data"] = True

epochs = 50

"""
# No syst
nosyst_args = args.copy()
path = "/home/centos/mount_point/data/nosyst/"
nosyst_args["outpath"] = path
nosyst_args["shape_syst"] = []
samples = inferno_opendata.run_cmsopen(nosyst_args, epochs = epochs, do_fit = True)
"""

# Rate parameter
rate_param_args = args.copy()
path = "/home/centos/mount_point/data/rate_param/"
rate_param_args["outpath"] = path
rate_param_args["shape_syst"] = []
rate_param_args["b_rate_param"] = True
samples = inferno_opendata.run_cmsopen(rate_param_args, epochs = epochs, do_fit = True)


"""
for i, val in enumerate(np.linspace(0.005, 0.02, 5)):
    path = "/home/centos/mount_point/data/artificial_sig/"

    args["outpath"] = path + "shift_" + str(i)
    args["artificial_syst"] = {"TTJets_signal": [{'name':"aplanarity", 'shift':val, 'norm':0.05}]}
    args["shape_syst"] = ["artsig_aplanarity"]
    try:
        samples = inferno_opendata.run_cmsopen(args, epochs = epochs, do_fit = True)
    except:
        print("Run failed")
"""

"""
for i, val in enumerate(np.linspace(0.005, 0.02, 5)):
    
    if i==0: continue
    
    path = "/home/centos/mount_point/data/artificial_bkg/"
    art_bkg_args = args.copy()
    art_bkg_args["outpath"] = path + "shift_" + str(i)
    art_bkg_args["artificial_syst"] = {"QCD": [{'name':"aplanarity", 'shift':val, 'norm':0.05}]}
    art_bkg_args["shape_syst"] = ["artbkg_aplanarity"]
    samples = inferno_opendata.run_cmsopen(art_bkg_args, epochs = epochs, do_fit = True)
    #try:
    #    samples = inferno_opendata.run_cmsopen(args, epochs = epochs, do_fit = True)
    #except:
    #    print("Run failed")


"""

for i in range(3):
    
    nuis_var = ["artsig_aplanarity", "artsig_MET_met", "artsig_deltaPhiTauMet"]
    nuis = nuis_var[0:i+1]
    
    print( nuis )
    
    path = "/home/centos/mount_point/data/artificial_nnuis/"
    
    art_nuis_args = args.copy()
    art_nuis_args["outpath"] = path + "nnuis_" + str(i)
    art_nuis_args["artificial_syst"] = {"TTJets_signal": [{'name':"aplanarity", 'shift':0.01, 'norm':0.05},
                                                         {'name':"MET_met", 'shift':20, 'norm':0.05},
                                                         {'name':"deltaPhiTauMet", 'shift':0.05, 'norm':0.05}]
                                       }
    art_nuis_args["shape_syst"] = nuis
    samples = inferno_opendata.run_cmsopen(art_nuis_args, epochs = epochs, do_fit = True)
    

"""
for i, val in enumerate(np.linspace(0.005, 0.02, 5)):
        
    path = "/home/centos/mount_point/data/artificial_sigbkg/"
    art_sigbkg_args = args.copy()
    art_sigbkg_args["outpath"] = path + "shift_" + str(i)
    art_sigbkg_args["artificial_syst"] = {"TTJets_signal": [{'name':"aplanarity", 'shift':val, 'norm':0.05}],
                                       "QCD": [{'name':"aplanarity", 'shift':val, 'norm':0.05}]}
    art_sigbkg_args["shape_syst"] = ["artsig_aplanarity", "artbkg_aplanarity"]
    samples = inferno_opendata.run_cmsopen(art_sigbkg_args, epochs = epochs, do_fit = True)
    #try:
    #    samples = inferno_opendata.run_cmsopen(args, epochs = epochs, do_fit = True)
    #except:
    #    print("Run failed")
"""
"""
for i, val in enumerate(np.linspace(1., 5., 5)):
    
    if i==0: continue
        
    path = "/home/centos/mount_point/data/artificial_signorm/"
    signorm_args = args.copy()
    signorm_args["outpath"] = path + "norm_" + str(i)
    signorm_args["shape_syst"] = []
    signorm_args["s_norm_syst"] = ["lumi"]
    signorm_args["scale_norms_only"] = [("lumi", val)]
    #try:
    samples = inferno_opendata.run_cmsopen(signorm_args, epochs = epochs, do_fit = True)
    #except:
    #    print("Run failed")
"""

"""
for i, val in enumerate(np.linspace(1., 5., 5)):
            
    path = "/home/centos/mount_point/data/artificial_bkgnorm/"
    bkgnorm_args = args.copy()
    bkgnorm_args["outpath"] = path + "norm_" + str(i)
    bkgnorm_args["shape_syst"] = []
    bkgnorm_args["b_norm_syst"] = ["mistag"]
    bkgnorm_args["scale_norms_only"] = [("mistag", val)]
    #try:
    samples = inferno_opendata.run_cmsopen(bkgnorm_args, epochs = epochs, do_fit = True)
    #except:
    #    print("Run failed")
"""
        