import warnings
warnings.simplefilter(action='ignore', category=FutureWarning)

import os
import numpy as np
import pandas as pd
import preproc
import hep_model
import plot
import train
import fit

def fit_cmsopen(fitvar, path):
    
    if fitvar == "inferno":
        bins = np.linspace(0,10,10)
    elif fitvar == "inferno_sorted":
        bins = np.linspace(0,10,10)
    else:
        bins = np.linspace(0,1,10)
           
    # Create config
    sample_names = ["Data", "QCD", "TTJets_bkg", "WZJets", "STJets", "TTJets_signal"]
    mc = ["TTJets_bkg", "WZJets", "STJets", "TTJets_signal"]
    corr_shape_systs = {"TTJets_signal" : ["btag"], "TTJets_bkg" : ["btag"]}
    uncorr_shape_systs = {"TTJets_signal" : ["06_jes"]}
    norm_syst ={}# {"lumi":{ "samples" : mc, "value" : 0.02 }, }
    
    config = fit.create_config(path, fitvar, bins, sample_names, corr_shape_systs, 
                               uncorr_shape_systs, norm_syst, float_qcd=True)
    
    print(config)
    
    # Create workspace
    ws_path = path + "/workspace_bce.json"
    ws = fit.create_ws(config, workspace_path = ws_path)
    
    fit_results, scan_results = fit.fit_ws(ws, config, asimov=True)
    return fit_results, scan_results
   
        

def train_cmsopen(opendata, test, args, epochs):
    
    #
    # Train INFERNO
    #
    inferno_model, inferno_info = train.train_inferno(opendata, args, epochs = epochs)
    
    # Predict test set - eventually add weights
    pred_sigmoid = True if args["use_softhist"] == True else False
    df_inf, order_d = hep_model.pred_test(inferno_model, test, pred_sigmoid=pred_sigmoid, name="inferno")
        
    # Plot the results
    plot.plot_inferno(df_inf, inferno_info, use_softhist = args["use_softhist"])
        
    #
    # Train BCE
    #
    bce_model, bce_info = train.train_bce(opendata, args, epochs=epochs)
    
    # Predict test set - eventually add weights
    df_bce, _ = hep_model.pred_test(bce_model, test, pred_sigmoid=True, name="bce")
    
    # Plot the results
    plot.plot_bce(df_bce, bce_info)
    
    #
    # Compare the covariance matrices
    #
    names = ["mu","JES", "JER"]#,'b-tag']
    plot.plot_cov(bce_info, inferno_info, names)
    
    return bce_model, inferno_model, order_d


def run_cmsopen( args, epochs=1, retrain = True, do_fit = False, store=True, outpath="."):
     
    # Create folder
    if args["store"] == True:
        if not os.path.exists(args["outpath"]):
            os.makedirs(args["outpath"])
        
    if retrain == True:
         
        # Load data
        opendata, test, samples, scaler = preproc.load_data( features = args["features"], 
                                                             shape_syst = args["shape_syst"],
                                                             weight_syst = args["weight_syst"],
                                                             bs = args["bs"], n_sig = args["n_sig"], 
                                                             n_bkg = args["n_bkg"], 
                                                             use_weights = args["use_weights"])
        # Train
        bce_model, inferno_model, order_d = train_cmsopen(opendata, test, args, epochs)
        
        # Predict INFERNO
        hep_model.pred_nominal(samples, features, inferno_model, scaler, name='inferno', order_d = order_d)
        # Predict BCE
        hep_model.pred_nominal(samples, features, bce_model, scaler, name="bce")
        if args["store"] == True:
            preproc.store_samples(samples, args["outpath"])
            
    else:
        
        # Load samples with predictions
        print( "Loading samples from path", outpath)
        samples = preproc.load_samples(outpath + "/")
        
    
    if do_fit:   
        
        # Convert samples to ROOT trees
        print( "Create root trees")
        fit.to_root(samples, path=outpath, systs = ["btag", "pdf", "trigger"])
        
        # Fit 
        print( "Fit BCE")
        fit_results_bce, scan_results_bce = fit_cmsopen(fitvar="bce", path=outpath)
        
        # Fit 
        print( "Fit INFERNO")
        fit_results_inf, scan_results_inf  = fit_cmsopen(fitvar="inferno_sorted", path=outpath)        
        
        return samples, fit_results_bce, scan_results_bce, fit_results_inf, scan_results_inf
        
        
        
        
        
