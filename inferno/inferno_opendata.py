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


def compare_results(args):
    
    if args["fit_asimov"]:
        # BCE
        bce_asimov_res = fit.load_fitresults( args["outpath"] + "/fit/bce_asimov" )
        bce_asimov_scan = fit.load_scan( args["outpath"] + "/fit/bce_asimov" )
        fit.print_summary(bce_asimov_res, "bce asimov")
        # INFERNO
        inferno_asimov_res = fit.load_fitresults( args["outpath"] + "/fit/inferno_asimov")
        inferno_asimov_scan = fit.load_scan( args["outpath"] + "/fit/inferno_asimov")
        fit.print_summary(inferno_asimov_res, "inferno asimov")
        plot.plot_scan(bce_asimov_scan, inferno_asimov_scan, path=args["outpath"] + "/fit", asimov=True, store=args["store"])

    if args["fit_data"]:
        #BCE
        #bce_res = fit.load_fitresults( args["outpath"] + "/fit/bce" )
        #fit.print_summary(bce_res, "bce")

        # INFERNO
        #inferno_res = fit.load_fitresults( args["outpath"] + "/fit/inferno" )
        #fit.print_summary(inferno_res, "inferno")

        # Plot likelihood scans
        #plot.plot_scan(bce_res, inferno_res, path=args["outpath"] + "/fit", asimov=False, store=args["store"])

def fit_cmsopen(args, fitvar, asimov = True):
    
    if (fitvar == "bce") or (args["use_softhist"] == True):
        bins = np.linspace(0,1,args["bins"])
    else:
        bins = np.linspace(0,args["bins"],args["bins"])
           
    # Create config    
    config = fit.create_config(args["outpath"], fitvar, bins, args["sample_names"], args["corr_shape_systs"], 
                               args["uncorr_shape_systs"], args["norm_syst"], float_qcd=args["fit_floatQCD"])
    
    if args["print_config"]: print(config)
    
    # Create workspace
    postfix = "_asimov" if asimov==True else ""
    path = args["outpath"] + "/fit/" + fitvar + postfix
    create_dir(path)
    ws_path = path + "/workspace.json"
    ws = fit.create_ws(config, workspace_path = ws_path)
    fit_results, scan_results = fit.fit_ws(ws, config, args, path, asimov=asimov)
    print(fit_results)
    if args["store"] == True:
        fit.store_fitresults(fit_results, path = path)
        fit.store_scan(scan_results, path)
            

def train_cmsopen(opendata, test, args, epochs):
    
    #
    # Train INFERNO
    #
    inferno_model, inferno_info = train.train_inferno(opendata, args, epochs = epochs)
    
    # Predict test set - eventually add weights
    df_inf, order_d = hep_model.pred_test(inferno_model, test, use_hist = args["use_softhist"], 
                                          name="inferno", bins=args["bins"])
        
    # Plot the results
    plot.plot_inferno(df_inf, inferno_info, args)
        
    #
    # Train BCE
    #
    bce_model, bce_info = train.train_bce(opendata, args, epochs=epochs)
    
    # Predict test set - eventually add weights
    df_bce, _ = hep_model.pred_test(bce_model, test, use_hist=True, name="bce")
    
    # Plot the results
    plot.plot_bce(df_bce, bce_info, args)
    
    #
    # Compare the covariance matrices
    #
    #names = ["mu","JES", "JER"]#,'b-tag']
    names = ["mu"] + args["shape_syst"] + args["weight_syst"]
    plot.plot_cov(bce_info, inferno_info, names, args)
    
    return bce_model, inferno_model, order_d


def create_dir(path):
    if not os.path.exists(path):
        os.makedirs(path)


def run_cmsopen( args, epochs=1, retrain = True, do_fit = False):
     
    # Create folders
    if args["store"] == True:
        create_dir(args["outpath"])
        create_dir(args["outpath"] + "/samples")
        create_dir(args["outpath"] + "/root_trees")
        create_dir(args["outpath"] + "/train_plots")
        create_dir(args["outpath"] + "/fit")
        
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
        hep_model.pred_nominal(samples, args["features"], inferno_model, scaler, sort_bins = args["fit_sorted"],
                               use_hist = args["use_softhist"], name='inferno', order_d = order_d)
        # Predict BCE
        hep_model.pred_nominal(samples, args["features"], bce_model, scaler, use_hist = True, name="bce")
        if args["store"] == True:
            preproc.store_samples(samples, args["outpath"])
            
    else:
        
        # Load samples with predictions
        print( "Loading samples from path", args["outpath"])
        samples = preproc.load_samples( args["outpath"] + "/samples/")
        
    
    if do_fit:   
        
        # Convert samples to ROOT trees
        print( "Create root trees")
        fit.to_root(samples, path=args["outpath"], systs = ["btag", "pdf", "trigger"])
        
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
        compare_results(args)
        
        
    return samples
        
        
        
