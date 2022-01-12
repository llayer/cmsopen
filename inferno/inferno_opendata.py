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
import stack
import bayes_opt


def fit_cmsopen(args, fitvar, asimov = False):
    
    print("*********************")
    if asimov == True:
        print( "Fit", fitvar, "asimov")
    else:
        print( "Fit", fitvar, "data")        
    
    if (fitvar == "bce"):
        bins = args["bce_fit_bins"]
    else:
        bins = args["inferno_fit_bins"]        
                    
    # Get fit model
    corr_shape_systs, uncorr_shape_systs, norm_syst = fit.get_fit_model(args)
            
    # Create config    
    config = fit.create_config(args["outpath"], fitvar, bins, args["sample_names"], corr_shape_systs, 
                               uncorr_shape_systs, norm_syst, float_qcd=args["fit_floatQCD"])
    if args["print_config"]: print(config)        
    # Create workspace
    postfix = "_asimov" if asimov==True else ""
    path = args["outpath"] + "/fit/" + fitvar + postfix
    create_dir(path)
    ws_path = path + "/workspace.json"
    ws = fit.create_ws(config, workspace_path = ws_path, prune_stat=args["prune_stat"])
    if args["print_ws"]: print(ws)
    fit_results, scan_results = fit.fit_ws(ws, config, args, path, asimov=asimov) 
    if (len(args["fit_shape_systs"]) + len(args["fit_norm_syst"])) > 0:
        if args["add_stat_only"] == True:
            fit.stat_only(config, fit_results, path = path, asimov=asimov, store=args["store"], 
                          prune_stat=args["prune_stat"], n_steps = args["n_steps"])
    
    return ws
            
def train_cmsopen(opendata, test, args, epochs):
    
    #
    # Train INFERNO
    #
    inferno_model, inferno_info = train.train_inferno(opendata, args, epochs = epochs)
    
    # Predict test set - eventually add weights
    df_inf, order_d = train.pred_test(inferno_model, test, use_hist = args["use_softhist"], 
                                          name="inferno", bins=args["inferno_bins"])
        
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
    names = ["mu"] + args["systnames"] + args["s_norm_syst"] + args["b_norm_syst"]
    plot.plot_cov(bce_info, inferno_info, names, args)
    
    return bce_model, inferno_model, order_d


def create_dir(path):
    if not os.path.exists(path):
        os.makedirs(path)
        
def store_args(args, path):
    with open(path + '/args.json', 'w') as outfile:
        json.dump(args, outfile)
        

def run_cmsopen( input_args, epochs=1, retrain = True, do_fit = False):
    
    args = input_args.copy()
    
    print("*********************")
    if (retrain == True) & (do_fit == True):
        print("Training and fitting INFERNO and binary-cross entropy (BCE) model")
    elif (retrain == True):
        print("Training INFERNO and binary-cross entropy (BCE) model")
    else:
        print("Fitting INFERNO and binary-cross entropy (BCE) model") 
    print("*********************")
    print()
        
    # Create folders
    if args["store"] == True:
        create_dir(args["outpath"])
        create_dir(args["outpath"] + "/samples")
        create_dir(args["outpath"] + "/root_trees")
        create_dir(args["outpath"] + "/train/bce")
        create_dir(args["outpath"] + "/train/inferno")
        create_dir(args["outpath"] + "/fit")
        if args["artificial_syst"] is not None:
            create_dir(args["outpath"] + "/artificial")
        if args["run_skopt"] == True:
            create_dir(args["outpath"] + "/skopt")
        
        # Store the config file:
        if (retrain == True):
            store_args(args, args["outpath"])
        print("*********************")        
        print("Outpath", args["outpath"])
    
    # Set the systematics 
    preproc.set_systs(args)
        
    if retrain == True:
         
        # Load data
        opendata, test, samples, scaler = preproc.load_data( features = args["features"], 
                                                             shape_syst = args["shape_syst"],
                                                             weight_syst = args["weight_syst"],
                                                             all_weight_syst = args["all_weight_syst"],
                                                             all_shape_syst = args["all_shape_syst"],
                                                             bs = args["bs"], n_sig = args["n_sig"], 
                                                             n_bkg = args["n_bkg"], 
                                                             use_weights = args["use_weights"], 
                                                             art_syst = args["artificial_syst"],
                                                             rs = args["rs"])

        # Scale the norms of the shape nuisances if specified
        if args["scale_shape_norms"] is not None:
            preproc.scale_shape(samples, args["scale_shape_norms"])
        # Set the true values for the training:
        args["b_true"], args["mu_true"], args["shape_norm_sigma"] = preproc.get_true_values(samples, args)            
        # Downsample data
        if args["downsample_factor"] is not None:
            preproc.downsample_data(samples, args["downsample_factor"], weight_syst = args["all_weight_syst"])
            args["b_true"] *= args["downsample_factor"]
            args["mu_true"] *= args["downsample_factor"]
        preproc.print_normalization(samples)
        
        # Plot the articial syst
        if args["artificial_syst"] is not None:
            stack.plot_art_syst(samples, args["artificial_syst"], path=args["outpath"], store=args["store"])
        
        # Optimize
        if args["run_skopt"] == True:
            bayes_opt.run_inferno_opt(opendata, args, epochs)
            bayes_opt.run_bce_opt(opendata, args, epochs)
        
        # Train
        bce_model, inferno_model, order_d = train_cmsopen(opendata, test, args, epochs)
        
        # Predict INFERNO
        train.pred_nominal(samples, args["features"], inferno_model, scaler, sort_bins = args["fit_sorted"],
                               use_hist = args["use_softhist"], name='inferno', order_d = order_d)
        # Predict BCE
        train.pred_nominal(samples, args["features"], bce_model, scaler, use_hist = True, name="bce")
        if args["store"] == True:
            preproc.store_samples(samples, args["outpath"])
        
        print("*********************")
        print("Finished training")
        print("*********************")
            
    else:
        
        # Load samples with predictions
        print("*********************")
        print( "Loading samples from path", args["outpath"])
        path = args["outpath"] if args["sample_path"] is None else args["sample_path"]
            
        samples = preproc.load_samples( path + "/samples/", 
                                        shape_systs = preproc.adjust_naming(args["all_shape_syst"]))      
        print(list(samples))
    
    if do_fit:   
        
        # Exclude the events used in the training from further processing   
        if args["exclude_train"] is True:
            preproc.exclude_train(samples, weight_syst = args["all_weight_syst"])
            
        if args["add_pdf_weights"] is True:
            preproc.pdf_weights(samples)
        
        # Set binning and range for the fit
        fit.set_bce_fit_bins(samples, args)
        fit.set_inferno_fit_bins(samples, args)
        
        # Convert samples to ROOT trees
        fit.to_root(samples, path=args["outpath"], systs = args["all_weight_syst"], include_pdf=args["add_pdf_weights"])
                
        # Print the fit arguments
        fit.print_fit_args(args)
            
        # Asimov
        if args["fit_asimov"]:
            fit_cmsopen(args, fitvar="bce", asimov=True)
            fit_cmsopen(args, fitvar="inferno", asimov=True)

        # Data 
        if args["fit_data"]:
            fit_cmsopen(args, fitvar="bce")
            fit_cmsopen(args, fitvar="inferno")
        
        # Load results and compare
        fit.compare_results(args)
        
        
    return samples
        
        
        
