import warnings
warnings.simplefilter(action='ignore', category=FutureWarning)

import os
import numpy as np
import pandas as pd
import preproc
import hep_model
import train
import fit
import json
import plot


def fit_cmsopen(args, fitvar, asimov = True):
    
    if (fitvar == "bce") or (args["use_softhist"] == True):
        bins = np.linspace(0,1,args["bins"])
    else:
        bins = np.linspace(0,args["bins"],args["bins"])
           
    # Create config    
    config = fit.create_config(args["outpath"], fitvar, bins, args["sample_names"], args["corr_shape_systs"], 
                               args["uncorr_shape_systs"], args["norm_syst"], float_qcd=args["fit_floatQCD"])
        
    # Create workspace
    postfix = "_asimov" if asimov==True else ""
    path = args["outpath"] + "/fit/" + fitvar + postfix
    create_dir(path)
    ws_path = path + "/workspace.json"
    ws = fit.create_ws(config, workspace_path = ws_path)
    if args["print_ws"]: print(ws)
    fit_results, scan_results = fit.fit_ws(ws, config, args, path, asimov=asimov)   
    return ws
            
def train_cmsopen(opendata, test, args, epochs):
    
    #
    # Train INFERNO
    #
    inferno_model, inferno_info = train.train_inferno(opendata, args, epochs = epochs)
    
    # Predict test set - eventually add weights
    df_inf, order_d = train.pred_test(inferno_model, test, use_hist = args["use_softhist"], 
                                          name="inferno", bins=args["bins"])
        
    # Plot the results
    plot.plot_inferno(df_inf, inferno_info, args, order_d)
        
    #
    # Train BCE
    #
    bce_model, bce_info = train.train_bce(opendata, args, epochs=epochs)
    
    # Predict test set - eventually add weights
    df_bce, _ = train.pred_test(bce_model, test, use_hist=True, name="bce")
    
    # Plot the results
    plot.plot_bce(df_bce, bce_info, args)
    
    #
    # Compare the covariance matrices
    #
    #names = ["mu","JES", "JER"]#,'b-tag']
    names = preproc.adjust_naming(["mu"] + args["systnames"])
    plot.plot_cov(bce_info, inferno_info, names, args)
    
    return bce_model, inferno_model, order_d


def create_dir(path):
    if not os.path.exists(path):
        os.makedirs(path)
        
def store_args(args, path):
    with open(path + '/args.json', 'w') as outfile:
        json.dump(args, outfile)    


def run_cmsopen( args, epochs=1, retrain = True, do_fit = False):
     
    # Create folders
    if args["store"] == True:
        create_dir(args["outpath"])
        create_dir(args["outpath"] + "/samples")
        create_dir(args["outpath"] + "/root_trees")
        create_dir(args["outpath"] + "/train/bce")
        create_dir(args["outpath"] + "/train/inferno")
        create_dir(args["outpath"] + "/fit")
        
        # Store the config file:
        store_args(args, args["outpath"])
    
    # Set names
    args["systnames"] = preproc.adjust_naming(args["shape_syst"] + args["weight_syst"])
    
    if retrain == True:
         
        # Load data
        opendata, test, samples, scaler = preproc.load_data( features = args["features"], 
                                                             shape_syst = args["shape_syst"],
                                                             weight_syst = args["weight_syst"],
                                                             bs = args["bs"], n_sig = args["n_sig"], 
                                                             n_bkg = args["n_bkg"], 
                                                             use_weights = args["use_weights"], 
                                                             art_syst = args["artificial_syst"])
        # Downsample data
        if args["downsample_factor"] is not None:
            preproc.downsample_data(samples, args["downsample_factor"])
            args["b_true"] *= args["downsample_factor"]
            args["mu_true"] *= args["downsample_factor"]
        # Exclude the events used in the training from further processing   
        if args["exclude_train"] is True:
            preproc.exclude_train(samples)
        
        # Train
        bce_model, inferno_model, order_d = train_cmsopen(opendata, test, args, epochs)
        
        # Predict INFERNO
        train.pred_nominal(samples, args["features"], inferno_model, scaler, sort_bins = args["fit_sorted"],
                               use_hist = args["use_softhist"], name='inferno', order_d = order_d)
        # Predict BCE
        train.pred_nominal(samples, args["features"], bce_model, scaler, use_hist = True, name="bce")
        if args["store"] == True:
            preproc.store_samples(samples, args["outpath"])
            
        print("Finished training")
            
    else:
        
        # Load samples with predictions
        print( "Loading samples from path", args["outpath"])
        samples = preproc.load_samples( args["outpath"] + "/samples/", shape_systs = args["shape_syst"])
        
    
    if do_fit:   
        
        # Convert samples to ROOT trees
        print( "Create root trees")
        fit.to_root(samples, path=args["outpath"], systs = args["weight_syst"])
        
        # Set nuisances:
        args["uncorr_shape_systs"] = {"TTJets_signal" : args["shape_syst"] + args["weight_syst"]}
        
        # Asimov
        if args["fit_asimov"]:
            print( "Fit BCE Asimov")
            fit_cmsopen(args, fitvar="bce", asimov=True)
            print( "Fit INFERNO Asimov")
            fit_cmsopen(args, fitvar="inferno", asimov=True)

        # Data 
        if args["fit_data"]:
            print( "Fit BCE Data")
            #fit_cmsopen(args, fitvar="bce")
            print( "Fit INFERNO Data")
            #fit_cmsopen(args, fitvar="inferno")
        
        # Load results and compare
        fit.compare_results(args)
        
        
    return samples
        
        
        
