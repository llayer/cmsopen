import os
import cabinetry
import pyhf
import uproot3
import numpy as np
import json
import logging
import plot
import stack
import inferno_config

    
logging.basicConfig(format="%(levelname)s - %(name)s - %(message)s")
#cabinetry.set_logging()

#
# Exclude zero bins to avoid problems with fit
#
def rebin_if_zero(samples, args):
    
    counts = []
    for s in args["sample_names"]:
        counts.append(np.histogram(samples[s]["inferno"], range=(0,args["inferno_bins"]), bins=args["inferno_bins"])[0])
    total_counts = np.array(counts).sum(0)
    
    if 0 in total_counts:
        print("*********************")
        print( "WARNING: Rebinning INFERNO due to zero bins")        
        rebin = {}
        nzeros = 0
        for ibin, count in enumerate(total_counts):
            if count == 0:
                nzeros += 1 
            rebin[ibin] = ibin - nzeros

        for s in samples:
            samples[s]["inferno"] = samples[s]["inferno"].replace(rebin)    
        args["inferno_bins"] = args["inferno_bins"] - nzeros
        
def get_nonzero_bins(samples, args, nbins, var="bce", ):
    counts = []
    for s in args["sample_names"]:
        content, edges = np.histogram(samples[s][var], range=(0.,1.), bins=nbins)
        counts.append(content)
    total_counts = np.array(counts).sum(0)
    
    nonzero_bins = []
    for i, c in enumerate(total_counts):
        if c != 0:
            nonzero_bins.append(i)
                        
    lower = edges[min(nonzero_bins)]
    upper = edges[max(nonzero_bins)+1]
    nbins = len(nonzero_bins)
    return lower, upper, nbins
    
def set_bce_fit_bins(samples, args):
    
    if args["exclude_zero"] == True:
        lower, upper, nbins = get_nonzero_bins(samples, args, args["bce_bins"], var="bce")
        #print("LOWER",lower, "UPPER",upper, "BINS", bins)
        #bins = np.linspace(0,1,args["bins"]+1) 
    else:
        lower, upper, nbins = 0, 1, args["bce_bins"]
        
    if args["rebin_hist"] is not None:
        nbins = args["rebin_hist"]
   
    args["bce_fit_bins"] = np.linspace(lower,upper,nbins+1)
    
def set_inferno_fit_bins(samples, args):
    
    if args["use_softhist"] == True:
        if args["exclude_zero"] == True:
            lower, upper, nbins = get_nonzero_bins(samples, args, args["inferno_bins"], var="inferno")
        else:
            lower, upper, nbins = 0, 1, args["inferno_bins"]
        if args["rebin_hist"] is not None:
            nbins = args["rebin_hist"]
        args["inferno_fit_bins"] = np.linspace(lower,upper,nbins+1)

    else:
        if args["exclude_zero"] == True:
            rebin_if_zero(samples, args)
        args["inferno_fit_bins"] = np.linspace(0,args["inferno_bins"],args["inferno_bins"]+1)
        
#
# Convert data to ROOT format
#

def create_tree(path, s, sample, weight="weight"):
    
    #print("Normalization", s, sample[weight].sum())
    
    file = uproot3.recreate(path + "/root_trees/" + s + ".root")
    file["tree"] = uproot3.newtree(
        {
        "inferno": np.float32,
        #"inferno_sorted": np.float32,
        "bce": np.float32,
        "weight": np.float32,    
        "event": np.float32
        }
    )    
    file["tree"].extend({
        "inferno": sample["inferno"].values ,
        #"inferno_sorted": sample["inferno_sorted"].values ,
        "bce": sample["bce"].values ,
        "weight": sample[weight].values,
        "event": sample["event"].values
    })
    file.close()    

    
def to_root(samples, systs = [], include_pdf=False, path = "/home/centos/data/inferno_cmsopen13/root_trees"):

    print("*********************")
    print( "Create ROOT trees")
        
    for s in samples:
        sample = samples[s]
        create_tree(path, s, sample)
        if ("Data" in s) | ("QCD" in s): continue
        if ('up' in s) | ('down' in s): continue
        for syst in systs:
            
            for ud in ["up", "down"]:
                #if "pdf" in syst: continue
                #create_tree(path,  s + "_" + syst + "_" + ud, sample, weight = "weight_"+ syst + "_" + ud )
                if ("pdf" in syst):
                    if (s == "TTJets_signal") | (s == "TTJets_bkg"):
                        create_tree(path,  s + "_" + syst + "_" + ud, sample, weight = "weight_"+ syst + "_" + ud )
                else:
                    create_tree(path,  s + "_" + syst + "_" + ud, sample, weight = "weight_"+ syst + "_" + ud )
                
        if include_pdf == True:
            if (s == "TTJets_signal") | (s == "TTJets_bkg"): 
                for i in range(22):
                    for ud in ["up", "down"]:
                        create_tree(path, s + "_pdf_" + str(i) + "_" + ud, sample, weight = "weight_pdf_" + str(i) + "_" + ud )
                    
#
# Write and fit workspace
#
def check_zero_bins(model_pred, data):
    mc_total = np.array(model_pred.model_yields[0]).sum(axis=0)
    return 0 in mc_total

def create_ws(config, workspace_path = "", postproc=True, prune_stat=True):
    cabinetry.templates.build(config, method="uproot")
    cabinetry.templates.postprocess(config)
    ws = cabinetry.workspace.build(config)
    cabinetry.workspace.save(ws, workspace_path)
    if prune_stat:
        ws = dict(pyhf.Workspace(ws).prune(modifier_types=["staterror"]))
    return ws
    
def fit_ws(ws, config, args, path, asimov = True):
    
    model, data = cabinetry.model_utils.model_and_data(ws, asimov=asimov)
    model_pred = cabinetry.model_utils.prediction(model)
    #figures = cabinetry.visualize.data_mc(model_pred, data, config=config, log_scale=True,
    #                                      save_figure=args["store"], figure_folder=path)
    stack.plot_from_model(model_pred, data, config=config, log_scale=True, store=args["store"], path=path)
    cabinetry.visualize.templates(config, save_figure=args["store"], close_figure = True, figure_folder=path)
    logging.getLogger("cabinetry").setLevel(logging.INFO)
    if args["print_yields"]: cabinetry.tabulate.yields(model_pred, data, per_bin=False, per_channel=True)
    fit_results = cabinetry.fit.fit(model, data, minos=args["minos"])
    #print(fit_results)
    logging.getLogger("cabinetry").setLevel(logging.WARNING)
     
    if (len(args["fit_shape_systs"]) + len(args["fit_norm_syst"])) > 0:
        cabinetry.visualize.pulls(fit_results, exclude=["mu"], save_figure=args["store"], 
                                  close_figure = False, figure_folder=path)
        cabinetry.visualize.correlation_matrix(fit_results, save_figure=args["store"], 
                                               close_figure = True, figure_folder=path)
        
        ranking_results = cabinetry.fit.ranking(model, data, fit_results=fit_results)
        #print(ranking_results)
        cabinetry.visualize.ranking(ranking_results, save_figure=args["store"], close_figure = False, figure_folder=path)
        
    scan_results = cabinetry.fit.scan(model, data, "mu", n_steps=args["n_steps"])
    
    #print(scan_results)
    #cabinetry.visualize.scan(scan_results)
    if args["store"] == True:
        store_fitresults(fit_results, path = path)
        store_scan(scan_results, path = path)        

    if args["fit_sig_lim"] == True:
        #limit_result = cabinetry.fit.limit(model, data, bracket=(0.5, 1.5))
        #cabinetry.visualize.limit(limit_result ,save_figure=args["store"], figure_folder=path)
        #print(limit_result)
        significance_result = cabinetry.fit.significance(model, data)
        #print(significance_result)
        if args["store"] == True:
            store_sig_lim_results(significance_result, lim_results=None, path = path)
    
    print("HAAALLLOOOO")
    
    return fit_results, scan_results

def stat_only(config, fit_results, path="", shape_syst = [], asimov = True, store=True, prune_stat=True, n_steps=200):
    
    fix = []
    for label, best in zip(fit_results.labels, fit_results.bestfit.tolist()):
        if (label == "mu") | (label == "QCD_norm"): continue
        if label in shape_syst:
            fix.append({"Name": "TTJets_signal_" + label, "Value": best})
        else:
            fix.append({"Name": label, "Value": best})   
    print(config)
    print({"Fixed":fix})   
    
    config["General"].update({"Fixed":fix})
    #print(config)
    ws = cabinetry.workspace.build(config)
    if prune_stat:
        ws = dict(pyhf.Workspace(ws).prune(modifier_types=["staterror"]))
    
    #print(ws)
    
    model, data = cabinetry.model_utils.model_and_data(ws, asimov=asimov)
    model_pred = cabinetry.model_utils.prediction(model)
    logging.getLogger("cabinetry").setLevel(logging.INFO)
    fit_results_stat = cabinetry.fit.fit(model, data, minos=["mu"]) 
    logging.getLogger("cabinetry").setLevel(logging.WARNING)
    scan_results = cabinetry.fit.scan(model, data, "mu", n_steps=n_steps)
    if store == True:
        store_fitresults(fit_results_stat, name = 'fit_results_stat', path = path)
        store_scan(scan_results, name = 'mu_scan_stat' , path = path)
    #print(fit_results)

#
# Store fit results
#
def store_fitresults(fit_results, name = 'fit_results', path=""):
    results = {}
    results["labels"] = fit_results.labels
    results["bestfit"] = fit_results.bestfit.tolist()
    results["uncertainty"] = fit_results.uncertainty.tolist()
    print(fit_results.minos_uncertainty)
    results["minos_mu"] = fit_results.minos_uncertainty["mu"]
    results["corr_mat"] = fit_results.corr_mat.flatten().tolist()
    with open(path + '/' + name + '.json', 'w') as outfile:
        json.dump(results, outfile)
        
def store_scan(scan_results, name = "mu_scan", path=""):
    
    results = {}
    results["name"] = scan_results.name
    results["bestfit"] = scan_results.bestfit
    results["uncertainty"] = scan_results.uncertainty
    results["parameter_values"] = scan_results.parameter_values.tolist()
    results["delta_nlls"] = scan_results.delta_nlls.tolist()
    with open(path + '/' + name + '.json', 'w') as outfile:
        json.dump(results, outfile)
        
def store_sig_lim_results(sig_results, lim_results=None, path=""):

    results = {}
    results["observed_p_value"] = sig_results.observed_p_value
    results["observed_significance"] = sig_results.observed_significance
    results["expected_p_value"] = sig_results.expected_p_value
    results["expected_significance"] = sig_results.expected_significance
    with open(path + '/sig_lim.json', 'w') as outfile:
        json.dump(results, outfile)    
        
#
# Load fit results
#
def load_fitresults(path=""):
    
    with open(path + '/fit_results.json') as json_file:
        results = json.load(json_file)
    results["bestfit"] = np.array(results["bestfit"])
    results["uncertainty"] = np.array(results["uncertainty"])
    results["minos_mu"] = np.array(results["minos_mu"])
    dim = int(np.sqrt(len(results["corr_mat"])))
    results["corr_mat"] = np.array(results["corr_mat"]).reshape(dim, dim)
    
    return results

def load_scan(path="", name='mu_scan'):
    
    with open(path + '/' + name + '.json') as json_file:
        results = json.load(json_file)
    results["parameter_values"] = np.array(results["parameter_values"])
    results["delta_nlls"] = np.array(results["delta_nlls"])

    return results   

def load_sig_lim(path=""):
    
    with open(path + '/sig_lim.json') as json_file:
        results = json.load(json_file)
    return results 

#
# Print summary and compare results
#

def print_fit_args(args):
    
    print("*********************")
    print("Summary fit args")
    print("*********************")
    print("Fit model", args["fit_model"])
    print("Fit norms", args["fit_norm_sigma"])
    print("Fit shape systematics", args["fit_shape_systs"])
    print("Fit asimov", args["fit_asimov"])
    print("Fit data", args["fit_data"])
    print("Fit significance", args["fit_sig_lim"])
    print("Fit float QCD", args["fit_floatQCD"])
    print("Prune stat error", args["prune_stat"])
    print("Minos", args["minos"])
    print("Scan n steps", args["n_steps"])          
          
def print_summary(results, sig_lim=None, name=""):
    
    print("*****************")
    print("Summary", name)
    for lab, best, std in zip(results["labels"], results["bestfit"], results["uncertainty"]):
        print(lab, round(best,3), "+-", round(std, 3))
    print("Minos uncertainty mu", results["minos_mu"][0], results["minos_mu"][1])
    print("Correlation matrix")
    print(results["corr_mat"])
    if sig_lim is not None:
        print("Significance expected", sig_lim["expected_significance"])
        print("Significance observed", sig_lim["observed_significance"])
    print("*****************")

def compare_results(args):
    
    if args["fit_asimov"]:
        # BCE
        bce_asimov_res = load_fitresults( args["outpath"] + "/fit/bce_asimov" )
        bce_asimov_scan = load_scan( args["outpath"] + "/fit/bce_asimov" )
        bce_sig_lim = load_sig_lim( args["outpath"] + "/fit/bce_asimov" ) if args["fit_sig_lim"] == True else None
        print_summary(bce_asimov_res, bce_sig_lim, "bce asimov")
        # INFERNO
        inferno_asimov_res = load_fitresults( args["outpath"] + "/fit/inferno_asimov")
        inferno_asimov_scan = load_scan( args["outpath"] + "/fit/inferno_asimov")
        inferno_sig_lim = load_sig_lim( args["outpath"] + "/fit/inferno_asimov" ) if args["fit_sig_lim"] == True else None
        print_summary(inferno_asimov_res, inferno_sig_lim, "inferno asimov")
        # Plot the comparison of the scan
        if ((len(args["fit_shape_systs"]) + len(args["fit_norm_syst"])) > 0) and (args["add_stat_only"]==True):
            bce_asimov_scan_stat = load_scan( args["outpath"] + "/fit/bce_asimov", name='mu_scan_stat' )
            inferno_asimov_scan_stat = load_scan( args["outpath"] + "/fit/inferno_asimov", name='mu_scan_stat')
        else:
            bce_asimov_scan_stat, inferno_asimov_scan_stat = None, None
        plot.plot_scan(bce_asimov_scan, inferno_asimov_scan, bce_asimov_scan_stat, inferno_asimov_scan_stat,
                       path=args["outpath"] + "/fit", asimov=True, store=args["store"])

    if args["fit_data"]:
        
        #BCE
        bce_res = load_fitresults( args["outpath"] + "/fit/bce" )
        bce_sig_lim = load_sig_lim( args["outpath"] + "/fit/bce" ) if args["fit_sig_lim"] == True else None
        bce_scan = load_scan( args["outpath"] + "/fit/bce" )
        print_summary(bce_res, bce_sig_lim, "bce")

        # INFERNO
        inferno_res = load_fitresults( args["outpath"] + "/fit/inferno" )
        inferno_scan = load_scan( args["outpath"] + "/fit/inferno")
        inferno_sig_lim = load_sig_lim( args["outpath"] + "/fit/inferno" ) if args["fit_sig_lim"] == True else None
        print_summary(inferno_res, inferno_sig_lim, "inferno")

        # Plot likelihood scans
        plot.plot_scan(bce_scan, inferno_scan, path=args["outpath"] + "/fit", asimov=False, store=args["store"])

        
#
# Config setup
#
def get_xsec_uncertainty(sample):
    
    return inferno_config.xsec_std[sample]

def get_fit_model(args, fitmodel):
    
    corr_shape_systs = {}
    uncorr_shape_systs = {}
    norm_syst = {}
    
    sig_shape_systs, bkg_shape_syst = [], []
    for syst in args["fit_shape_systs"]:
        if "bkg" in syst:
            bkg_shape_syst.append(syst)
        else:
            sig_shape_systs.append(syst)
            
    if fitmodel == "signal_only":
        # Set nuisances:
        #uncorr_shape_systs = {"TTJets_signal" : args["fit_shape_systs"].copy()}
        uncorr_shape_systs = {"TTJets_signal" : sig_shape_systs.copy(),
                              "QCD" : bkg_shape_syst.copy()}
        # Add pdf
        #if args["add_pdf_weights"] == True:
        #    uncorr_shape_systs["TTJets_signal"] += ["pdf_" + str(i) for i in range(22)]
            
        for norm in args["fit_norm_sigma"]:
            if norm == "mistag":
                norm_syst[norm] = { "samples" : "QCD", "value" : args["fit_norm_sigma"][norm] }
            else:
                norm_syst[norm] = { "samples" : "TTJets_signal", "value" : args["fit_norm_sigma"][norm] }
    elif fitmodel == "sig_bkg":
        for s in args["mc"]: 
            corr_shape_systs[s] = sig_shape_systs.copy()
        if len(bkg_shape_syst) > 0: uncorr_shape_systs = {"QCD" : bkg_shape_syst.copy()}
        if args["add_pdf_weights"] == True:
            #corr_shape_systs["TTJets_signal"] += ["pdf_" + str(i) for i in range(22)]
            #corr_shape_systs["TTJets_bkg"] += ["pdf_" + str(i) for i in range(22)]
            corr_shape_systs["TTJets_signal"] += ["pdf"]
            corr_shape_systs["TTJets_bkg"] += ["pdf"]
 
        for norm in args["fit_norm_sigma"]:
            if "tt" in norm:
                norm_syst[norm] = { "samples" : ["TTJets_signal", "TTJets_bkg"], "value" : args["fit_norm_sigma"][norm] }
            elif norm == "mistag":
                norm_syst[norm] = { "samples" : "QCD", "value" : args["fit_norm_sigma"][norm] }
            elif norm == "xsec":
                for s in args["mc"]: 
                    norm_syst[norm] = { "samples" : s, "value" : get_xsec_uncertainty(s) }                
            else:
                norm_syst[norm] = { "samples" : args["mc"], "value" : args["fit_norm_sigma"][norm] } 
    else:
        raise ValueError("No valid fit model")
       
    
    return corr_shape_systs, uncorr_shape_systs, norm_syst 
        
    
#
# Create config
# 
def add_samples(sample_names):
    
    samples = []
    
    for s in sample_names:
        
        if "Data" in s:
            samples.append(
              {
                 "Name": "Data",
                 "Tree": "tree",
                 "SamplePath": "Data.root",
                 "Data": True                       # observed data is handled differently, need to distinguish
              }
            )
 
        else:
            samples.append(
            {
             "Name": s,
             "Tree": "tree",
             "SamplePath": s+".root",
             "Weight": "weight"
            }
            )
        
    return{"Samples" : samples} 
    
def add_syst(corr_shape_systs, uncorr_shape_systs, norm_syst):
        
    systs = []
    for sample in corr_shape_systs:
        for syst in corr_shape_systs[sample]:
            systs.append(
                {
                "Name": sample+ "_" + syst,
                    "Up": {"SamplePath": sample + "_" + syst + "_up.root"},
                    "Down": {"SamplePath": sample + "_" + syst + "_down.root"},
                    "Type" : "NormPlusShape",
                    "Samples": sample,
                    "ModifierName": syst
                }        
            )
            
    for sample in uncorr_shape_systs:
        for syst in uncorr_shape_systs[sample]:
            systs.append(
                {
                "Name": sample+ "_" + syst,
                    "Up": {"SamplePath": sample + "_" + syst + "_up.root"},
                    "Down": {"SamplePath": sample + "_" + syst + "_down.root"},
                    "Type" : "NormPlusShape",
                    "Samples": sample,
                }        
            )    
            
    for syst in norm_syst:
        
            systs.append(
                {
                "Name": syst,
                'Up': {'Normalization': norm_syst[syst]["value"]},
                'Down': {'Normalization': -norm_syst[syst]["value"]},
                'Type': 'Normalization',
                "Samples": norm_syst[syst]["samples"]
                }        
            )          
            
    return{"Systematics" : systs} 


def create_config(path, fit_var, bins, sample_names, corr_shape_systs={}, uncorr_shape_systs={}, norm_syst={}, float_qcd=True):
    
    
    # General setup
    config = {
       "General":{
          "Measurement": "CMSOpenData",
          "POI": "mu",               # parameter of interest, which we want to measure 
          "InputPath": path +"/root_trees/{SamplePath}", # where to find input data
          "HistogramFolder": path + "/histograms/"
       }
    }
        
    # Region and fit variable
    config.update({
   "Regions":[
      {
         "Name": "Signal_region",
         "Variable": fit_var,                 # which variable we bin histograms in
         "Binning": list(bins), #[0,0.1,0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.],
         #"Filter": "bdt > -1"           # event selection 
          }
       ]
    })
    
    # Samples
    config.update(add_samples(sample_names))
    
    # Shape systematics
    config.update(add_syst(corr_shape_systs, uncorr_shape_systs, norm_syst))
    
    # Rate parameters
    config.update({
       "NormFactors":[
          {
             "Name": "mu",
             "Samples": "TTJets_signal",    # we want this parameter to scale the signal
             "Nominal": 1,
             "Bounds": [-5, 10]
          }
       ]
    })
    
    if float_qcd:
        config["NormFactors"].append(
              {
             "Name": "QCD_norm",
             "Samples": "QCD",   
             "Nominal": 1,
             "Bounds": [0.5, 1.5]
              }
        )
        
    if cabinetry.configuration.validate(config):
        return config
    else:
        print("Config not valid")
        
        
        
        