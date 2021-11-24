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
        bins = np.linspace(0,1,10)
    else:
        bins = np.linspace(0,10,10)
           
    # Create config
    sample_names = ["Data", "QCD", "TTJets_bkg", "WZJets", "STJets", "TTJets_signal"]
    mc = ["TTJets_bkg", "WZJets", "STJets", "TTJets_signal"]
    corr_shape_systs = {"TTJets_signal" : ["btag"], "TTJets_bkg" : ["btag"]}
    uncorr_shape_systs = {"TTJets_signal" : ["06_jes"]}
    norm_syst ={}# {"lumi":{ "samples" : mc, "value" : 0.02 }, }
    
    config = fit.create_config(path, fit_var, bins, sample_names, corr_shape_systs, 
                               uncorr_shape_systs, norm_syst, float_qcd=True)
    
    print(config)
    
    # Create workspace
    ws_path = path + "/workspace_bce.json"
    ws = fit.create_ws(config, workspace_path = ws_path)
    
    fit.fit_ws(ws, config, asimov=True)
   
        

def train_cmsopen(opendata, test, inferno_args, epochs, use_softhist):
    
    #
    # Train INFERNO
    #
    inferno_model, inferno_info = train.train_inferno(opendata, test, epochs=epochs, inferno_args = inferno_args, 
                                                      use_softhist=use_softhist)
    
        
    # Predict test set - eventually add weights
    df_inf, order_d = hep_model.pred_test(inferno_model, test, pred_sigmoid=False, name="inferno")
        
    # Plot the results
    plot.plot_inferno(df_inf, inferno_info, use_softhist  = use_softhist)
        
    #
    # Train BCE
    #
    bce_model, bce_info = train.train_bce(opendata, epochs=epochs, inferno_args = inferno_args)
    
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


def run_cmsopen(features, shape_syst = [], weight_syst = [], bs = 1000, n_sig = 20000, use_weights = False,
                inferno_args = None, epochs=1, use_softhist = False, retrain = True, do_fit = False):
     
    if retrain == True:
         
        # Load data
        opendata, test, samples, scaler = preproc.load_data( features = features, 
                                                                shape_syst = shape_syst,
                                                                weight_syst = weight_syst,
                                                                bs=bs, n_sig=n_sig, 
                                                                use_weights = use_weights)
        # Train
        bce_model, inferno_model, order_d = train_cmsopen(opendata, test, inferno_args, epochs, use_softhist)
        
        # Predict INFERNO
        hep_model.pred_nominal(samples, features, inferno_model, scaler, name='inferno', order_d = order_d)
        # Predict BCE
        hep_model.pred_nominal(samples, features, bce_model, scaler, name="bce")
        
    else:
        
        # Load samples with predictions
        samples = preproc.load_samples(path)
        
    
    if do_fit:   
        
        # Convert samples to ROOT trees
        fit.to_root(samples, path=path, systs = ["btag", "pdf", "trigger"])
        
        # Fit 
        fit_cmsopen(fitvar="bce")
        
        # Fit 
        fit_cmsopen(fitvar="inferno")        
        
        
        
        
        
        
