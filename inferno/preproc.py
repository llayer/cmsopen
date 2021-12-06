import pandas as pd
import numpy as np
import random
import json
import os
from functools import reduce
from sklearn import preprocessing
from pytorch_inferno.data import *
import inferno_config

pd.options.mode.chained_assignment = None 

#
# Load norms and set names for INFERNO training
#

def get_shape_norm(samples, shape_syst, weight_syst, asymm = False, sample="TTJets_signal"):
    
    norm = []
    for syst in shape_syst:
        nominal = samples[sample]["weight"].sum()
        up = samples[sample + "_" + syst + "_up"]["weight"].sum()
        down = samples[sample + "_" + syst + "_down"]["weight"].sum()
        if asymm == False:
            std = (0.5*(abs(up - nominal) + abs(nominal-down))) / nominal
            norm.append(std)
        else:
            norm.append((down/nominal, up/nominal))
    
    weight_norm = []
    for syst in weight_syst:
        nominal = samples[sample]["weight"].sum()
        up = samples[sample]["weight_" + syst + "_up"].sum()
        down = samples[sample]["weight_" + syst + "_down"].sum()
        std = (0.5*(abs(up - nominal) + abs(nominal-down))) / nominal
        if asymm == False:
            std = (0.5*(abs(up - nominal) + abs(nominal-down))) / nominal
            norm.append(std)
        else:
            norm.append((down/nominal, up/nominal))
        
    return norm


def get_norm_nuisance(norm_syst):
       
    return {k: inferno_config.norms[k] for k in norm_syst}

    
def calc_xsec_uncertainty(samples, args):
   
    xsec_std = {}
    for s in args["mc"]:
        
        nom = samples[s]["norm"].sum()
        up = samples[s]["norm_up"].sum()
        down = samples[s]["norm_down"].sum()
        std = (0.5*(abs(up - nom) + abs(nom-down))) / nom
        xsec_std[s] = std
    return xsec_std    

def adjust_naming(syst_names):
    syst = []
    # Adjust the naming for combine
    for s in syst_names:
        if "jes" in s:
            syst.append('jes')
        elif "taue" in s:
            syst.append('taue')
        else:
            syst.append(s)
    return syst

#
# Set the weights of the MC samples
#
def set_weights(samples, weight_systs = []):
    
    for s in samples:
        
        sample = samples[s]
        if s == "Data":
            sample["weight"] = 1.
        elif s == "QCD":
            scale_qcd = 4.
            sample["weight"]  = sample['btag_weight2'] * scale_qcd
        else:
            sample["weight"] = sample['norm'] * sample['trigger_weight'] * sample['btag_weight1']
            if ('up' in s) | ('down' in s): continue
            for syst in weight_systs:
                for ud in ["up", "down"]:
                    if syst == "btag":
                        sample["weight_btag_" + ud] = sample['norm'] * sample['trigger_weight'] * sample['btag_weight1_'+ud]
                        sample["btag_"+ud] = sample['btag_weight1_'+ud] / sample['btag_weight1']
                    if syst == "trigger":
                        sample["weight_trigger_"+ud] = sample['norm'] * sample['trigger_weight_'+ud] * sample['btag_weight1']   
                        sample["trigger_"+ud] = sample['trigger_weight_'+ud] / sample['trigger_weight']
                if (s == "TTJets_signal") & (syst == "pdf"):
                    # PDF Up
                    sample["pdf_up"] = 1+sample["pdf_up"].fillna(0.)
                    sample["weight_pdf_up"] = sample["weight"] * sample["pdf_up"]
                    # PDF Down
                    sample["pdf_down"] = 1-sample["pdf_down"].fillna(0.)
                    sample["weight_pdf_down"] = sample["weight"] * sample["pdf_down"]
        print("Normalization", s, samples[s]["weight"].sum())
                    
def set_normalization(sample, factor):
    
    sample["weight"] *= factor                     

def print_normalization(samples):
    
    for s in samples:
        print("Normalization", s, samples[s]["weight"].sum())
    
def scale_shape_norm(samples, nuisance, value, args):
    
    renamed_nuis = adjust_naming([nuisance])[0]
    if renamed_nuis in ["jes", "jer", "taue"]:
        for s in samples:
            if renamed_nuis in s:
                pre_norm = samples[s]["weight"].sum()
                if "up" in s:
                    set_normalization(samples[s], factor=1+value)
                if "down" in s:
                    set_normalization(samples[s], factor=1-value)
                post_norm = samples[s]["weight"].sum()
                print("Scaling", s, "from", pre_norm, "to", post_norm)
    elif renamed_nuis in ["trigger", "btag", "pdf"]: 
        for s in samples:
            if s in args["mc"]:
                if renamed_nuis == "pdf":
                    if s == "TTJets_signal":
                        samples[s]["weight_" + renamed_nuis + "_up"] *= 1+values
                        samples[s]["weight_" + renamed_nuis + "_down"] *= 1-values
                else:
                        samples[s]["weight_" + renamed_nuis + "_up"] *= 1+values
                        samples[s]["weight_" + renamed_nuis + "_down"] *= 1-values
    else:
        raise ValueError("Specified NP", nuisance, "can't be scaled")
    
def scale_norm_only(nuis, value, args):
    if nuis == "mistag":
        args["b_norm_sigma"][nuis] *= value
    else:
        args["s_norm_sigma"][nuis] *= value
    print("Scale", nuis, "to", args["s_norm_sigma"][nuis])
    
def set_fit_norm_nuis(args):
    fit_norm_syst = get_norm_nuisance(args["fit_norm_syst"])
    for nuis in args["s_norm_sigma"]:
        fit_norm_syst[nuis] = args["s_norm_sigma"][nuis]
    for nuis in args["b_norm_sigma"]:
        fit_norm_syst[nuis] = args["b_norm_sigma"][nuis]
    return fit_norm_syst
            
    
def exclude_train(samples):
    
    for s in samples:
        if ( "QCD" in s) | ("TTJets_signal" in s):
            n_samples_pre = len(samples[s])
            norm_pre = samples[s]["weight"].sum()
            samples[s] = samples[s][samples[s]["is_train"] == False]
            norm_post = samples[s]["weight"].sum()
            reweight_factor = norm_pre / float(norm_post)
            set_normalization(samples[s], reweight_factor) 
            assert(norm_pre - samples[s]["weight"].sum() < 0.0001)
    
def downsample_data(samples, sample_factor):
    
    for s in samples:
        if s == "Data":
            n_samples_pre = len(samples[s])
            samples[s] = samples[s].sample(frac=sample_factor)
            n_samples_post = len(samples[s])
            print("Downsampled data from", n_samples_pre, "to", n_samples_post)
        else:
            set_normalization(samples[s], sample_factor)

def get_true_values(samples, args):
    
    # Set the normalizations for the training:
    shape_norm_sigma = get_shape_norm(samples, args["shape_syst"], args["weight_syst"], asymm = args["asymm_shape_norm"] ) 
    #print( args["shape_norm_sigma"] )
    #[0.05, 0.02] # CHECK adjust for correct values
    # Signal and bkg
    mu_true = samples["TTJets_signal"]["weight"].sum()
    #print(mu_true)
    b_true = samples["QCD"]["weight"].sum() 
    return b_true, mu_true, shape_norm_sigma
    
#
# Preparation of training data
#
def add_key(df):
    df["event_id"] = df["event"].astype(str) + df["luminosityBlock"].astype(str)

    
def train_test_split(df, n = 5000):

    df.reset_index(drop=True, inplace=True)
    indices = list(df.index)
    train_id = random.sample(indices, n)
    test_id = list(set(indices) - set(train_id))
    train_mask = df.index.isin(train_id)
    test_mask = df.index.isin(test_id)
    cond = [train_mask, test_mask]
    choice = ["train", "test"]
    df['train_flag'] = np.select(cond, choice)
    
    
def get_train_evts(sample):
    return sample[sample["train_flag"]=="train"]["event_id"]    
    
    
def get_train_data(samples, features, shape_syst, weight_syst, n_sig = 20000, n_bkg = 10000, use_weights = False):
    
    # Add a unique event key
    for s in samples:
        add_key(samples[s])
    
    # Add a suffix to be able to merge the frames
    for syst in shape_syst:
        for ud in ["_up", "_down"]:    
            samples["TTJets_signal_" + syst + ud] = samples["TTJets_signal_" + syst + ud].add_suffix("_" + syst + ud)
            samples["TTJets_signal_" + syst + ud].rename(columns={'event_id_' + syst + ud: 'event_id'}, inplace=True) 
            #print(list(samples["TTJets_signal_" + syst + ud]))
    
    # Merge the nominal and systematic frames
    dfs = []
    dfs.append(samples["TTJets_signal"].copy())
    #print(samples["TTJets_signal"]["weight"].head())
    for syst in shape_syst:
        for ud in ["_up", "_down"]:    
            dfs.append(samples["TTJets_signal_" + syst + ud].copy())
            #samples["TTJets_signal_" + syst + ud]["weight_" + syst + ud]  * \
            #(1. / samples["TTJets_signal_" + syst + ud]["weight_" + syst + ud])
            #print(samples["TTJets_signal_" + syst + ud]["weight_" + syst + ud].head())
    signal = reduce(lambda  left,right: pd.merge(left,right,how="inner", on="event_id"), dfs)
    # Split in training and test
    train_test_split(signal, n_sig)
    train_idx = get_train_evts(signal) 
    train_test_split(samples["QCD"], n_bkg)
    bkg = samples["QCD"].copy()
        
    #Weights
    #signal["weights"] = signal['trigger_weight'] * signal['btag_weight1']
    signal["weight"] *= (1. / np.mean(signal["weight"]))
    for syst in shape_syst:
        for ud in ["_up", "_down"]:
            signal["weight_" + syst + ud] *= (1. / np.mean(signal["weight_" + syst + ud]))
    #bkg["weights"] = bkg['btag_weight2']
    #print( list( zip( signal["weight"], signal["weight_06_jes_up"], signal["weight_06_jes_down"])))                               
    bkg["weight"] = bkg["weight"] * (1. / np.mean(bkg["weight"])) 
    
    # Add labels
    signal["label"] = 1
    bkg["label"] = 0
        
    # Select test and train and add weights
    signal_train = signal[signal["train_flag"] == "train"]
    signal_test = signal[signal["train_flag"] == "test"]
    bkg_train = bkg[bkg["train_flag"] == "train"]
    bkg_test = bkg[bkg["train_flag"] == "test"]
    
    # Concat the signal and background
    train_data = pd.concat([signal_train, bkg_train], axis=0)
    test_data = pd.concat([signal_test, bkg_test], axis=0)
    
    # Select features, transform and select the labels
    X_train = train_data[features].values
    scaler = preprocessing.StandardScaler().fit(X_train)
    X_train = scaler.transform(X_train)
    y_train = train_data["label"].values.reshape((-1,1))
    weights_train = train_data["weight"].values.reshape((-1,1))
    X_test = test_data[features].values
    X_test = scaler.transform(X_test)
    y_test = test_data["label"].values.reshape((-1,1))   
    weights_test = test_data["weight"].values.reshape((-1,1))
    
    # Add the systematic shape variations
    train_syst = []
    test_syst = []
    for syst in shape_syst:
        for ud in ["_up", "_down"]:    
            feat_syst = [x + "_" + syst + ud for x in features]
            train_syst.append(scaler.transform(train_data[feat_syst].values))
            test_syst.append(scaler.transform(test_data[feat_syst].values))
    
    # Add the weights
    weights_train_syst = []
    weights_test_syst = []
    for syst in shape_syst:
        for ud in ["_up", "_down"]:
            w_train = train_data["weight_" + syst + ud]
            #print(list(zip(train_data[syst+ud], train_data[syst], w_train)))
            w_test = test_data["weight_" + syst + ud]
            weights_train_syst.append(  w_train.values.reshape((-1,1)) )
            weights_test_syst.append( w_test.values.reshape((-1,1)) )

    for syst in weight_syst:
        for ud in ["_up", "_down"]:
            w_train = train_data[syst + ud]
            #print(list(zip(train_data[syst+ud], train_data[syst], w_train)))
            w_test = test_data[syst + ud]
            weights_train_syst.append(  w_train.values.reshape((-1,1)) )
            weights_test_syst.append( w_test.values.reshape((-1,1)) )
    
    X_train = np.stack((X_train, *train_syst), axis=2)
    X_test = np.stack((X_test, *test_syst), axis=2)
    
    if use_weights == True:
        weights_train = np.stack((weights_train, *weights_train_syst), axis=2)
        weights_test = np.stack((weights_test, *weights_test_syst), axis=2)
        #print(weights_train.shape)
        trn = (X_train, y_train, weights_train) 
        val = (X_test, y_test, weights_test)
    else:
        trn = (X_train, y_train) 
        val = (X_test, y_test)
        
    # rename the syst frames:
    for syst in shape_syst:
        for ud in ["_up", "_down"]:
            #samples["TTJets_signal" + syst].columns = samples["TTJets_signal" + syst].columns.str.rstrip(syst)
            samples["TTJets_signal_" + syst + ud].columns = samples["TTJets_signal_" + syst + ud].columns.str.replace(
                "_" + syst + ud + r'$', '')
            #print(list(samples["TTJets_signal_" + syst + ud]))
        
    for s in samples:
        if "TTJets_signal" in s:
            #print(s)
            #print(list(samples[s]))
            samples[s]["is_train"] = samples[s].event_id.isin(train_idx)
            print("DDDDD", samples[s]["weight"].sum())
        if "QCD" in s:
            samples[s]["is_train"] = samples[s]["train_flag"] == "train"
    
    print("*********************")
    print("Summary training data")
    print("Features", features)
    print("Shape systematics", shape_syst)
    print("Weight systematics", weight_syst)
    print("Use weights", use_weights)
    n_sig_train = sum(samples["TTJets_signal"]["is_train"]==1)
    print("Number of signal training / test events:", n_sig_train, len(samples["TTJets_signal"]) - n_sig_train)
    n_bkg_train = sum(samples["QCD"]["is_train"]==1)
    print("Number of bkg training / test events:", n_bkg_train, len(samples["QCD"]) - n_bkg_train)
    print("*********************")
        
    return trn, val, scaler



#
# Generate artificial systematic variation
#
def shift_var(X, col, shift = 0.5):
          
    X_up = X.copy()
    X_up[col] += shift
    X_down = X.copy()
    X_down[col] -= shift
    return X_up, X_down

def gen_artificial_systs(samples, artificial_syst):
    
    for s in artificial_syst:
        for var in artificial_syst[s]:
            col = var['name']
            shift = var['shift']
            norm = var['norm']
            print(col, shift, norm)
            X_up, X_down = shift_var(samples[s], col, shift)
            set_normalization(X_up, factor = 1. + norm)
            set_normalization(X_down, factor = 1. - norm)
            samples[s + "_art_" + col + "_up"] = X_up
            samples[s + "_art_" + col + "_down"] = X_down
            
            #print(list(zip(samples[s][col], samples[s + "_art_" + col + "_up"][col], samples[s + "_art_" + col + "_down"][col])))

#
# Data loading
#
def assert_weight_syst(weight_syst):
    
    allowed_systs = ["btag", "trigger", "pdf"]
    for syst in weight_syst:
        if syst not in allowed_systs:
            raise ValueError("Specified weight sytematic not allowed:", syst)
            
def assert_shape_syst(shape_syst):
    
    allowed_systs = ["jes", "jer", "taue"]
    for syst in adjust_naming(shape_syst):
        if syst not in allowed_systs:
            raise ValueError("Specified weight sytematic not allowed:", syst)

def load_samples(path, shape_systs=[]):
    
    samples = {}
    for s in ["TTJets_bkg", "WZJets", "STJets", "QCD", "TTJets_signal", "Data"]:
        samples[s] = pd.read_hdf(path + s + ".h5")
        
        if ("Data" not in s) & ("QCD" not in s):
            for syst in shape_systs:
                samples[s + "_" + syst + "_up"] = pd.read_hdf(path + s + "_" + syst + "_up" + ".h5")
                samples[s + "_" + syst + "_down"] = pd.read_hdf(path + s + "_" + syst + "_down" + ".h5")
        
    return samples

        
def load_data(features, shape_syst, weight_syst, path = "/home/centos/data/bdt_rs5/", bs=256, 
              n_sig = 5000, n_bkg = 5000, use_weights = False, art_syst=None):
    
    # Check that the specified systematics are allowed:
    assert_shape_syst(shape_syst)
    assert_weight_syst(weight_syst)
    
    # Load the samples
    samples = load_samples(path, shape_syst)
    
    # Set the weights
    set_weights(samples, weight_systs = weight_syst)
    
    # Create artificial systs
    if art_syst is not None:
        gen_artificial_systs(samples, art_syst)
        print(list(samples))
        
    # Print the normalizations
    # print_normalization(samples)

    # Get the training data            
    trn, val, scaler = get_train_data(samples, features, shape_syst, weight_syst, 
                                      n_sig = n_sig, n_bkg = n_bkg, use_weights = use_weights)   
    trn_dl = WeightedDataLoader(DataSet(*trn), batch_size=bs, shuffle=True, drop_last=True)
    val_dl = WeightedDataLoader(DataSet(*val), batch_size=bs, shuffle=True)
    test_dl = WeightedDataLoader(DataSet(*val), batch_size=bs)
    data = DataPair(trn_dl, val_dl)
   
    print("*********************")
    print("Summary data")
    print("batch size", bs)
    print("x", trn_dl.dataset[0][0].shape)
    print("y", trn_dl.dataset[0][1])
    if trn_dl.dataset[0][2] is not None:
        print("w", trn_dl.dataset[0][2].shape)
    else:
        print("w", trn_dl.dataset[0][2])
    print("*********************")

    return data, test_dl, samples, scaler
   
#
# Data storing
#
def store_samples(samples, outpath):  
    
    for s in samples:
        samples[s].to_hdf(outpath + "/samples/" + s + ".h5", "frame", mode='w')

       




