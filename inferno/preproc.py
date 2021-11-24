import pandas as pd
import numpy as np
import random
import json

from functools import reduce
from sklearn import preprocessing
from pytorch_inferno.data import *

pd.options.mode.chained_assignment = None 

def get_norm(norm_syst, path="/home/centos/data/bdt_rs5/norms.h5"):
    
    df_norm = pd.read_hdf( path )
    mu = df_norm["TTJets_signal"][0]
    print("mu", mu)
    qcd = df_norm["QCD"][0] 
    print("QCD", qcd)
    sig_norm = []
    for s in norm_syst:
        if s == "btag_weight1":
            syst_name = "btag"
        else:
            syst_name = s
        up = df_norm["TTJets_signal_" + syst_name + "Up"][0] 
        nominal = df_norm["TTJets_signal"][0] 
        down = df_norm["TTJets_signal_" + syst_name + "Down"][0]
        print("up", up)
        print("nominal", nominal)
        print("down", down)  
        std = 0.5*(abs(up - nominal) + abs(nominal-down)) #CHECK!!
        print(syst_name, std)
        sig_norm.append(std)
    return mu, qcd, sig_norm


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
    
    
def get_cmsopen_data(samples, features, shape_syst, weight_syst, n_sig = 20000, n_bkg = 10000, use_weights = False):
    
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
    dfs.append(samples["TTJets_signal"])
    for syst in shape_syst:
        for ud in ["_up", "_down"]:    
            dfs.append(samples["TTJets_signal_" + syst + ud])
    signal = reduce(lambda  left,right: pd.merge(left,right,how="inner", on="event_id"), dfs)
    bkg = samples["QCD"]
    
    #Weights
    signal["weights"] = signal['trigger_weight'] * signal['btag_weight1']
    signal["weights"] = signal["weights"] * (1. / np.mean(signal["weights"]))
    bkg["weights"] = bkg['btag_weight2']
    bkg["weights"] = bkg["weights"] * (1. / np.mean(bkg["weights"]))


    # Split in training and test
    train_test_split(signal, n_sig)
    train_test_split(bkg, n_bkg)
    train_idx = get_train_evts(signal)  
    
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
    weights_train = train_data["weights"].values.reshape((-1,1))
    X_test = test_data[features].values
    X_test = scaler.transform(X_test)
    y_test = test_data["label"].values.reshape((-1,1))   
    weights_test = test_data["weights"].values.reshape((-1,1))
    
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
    for syst in weight_syst:
        for ud in ["_up", "_down"]:
            w_train = train_data[syst + ud] / train_data[syst]
            #print(list(zip(train_data[syst+ud], train_data[syst], w_train)))
            w_test = test_data[syst + ud] / test_data[syst]
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
        
    #data = DataPair(trn_dl, val_dl)
    #test = DataPair(WeightedDataLoader(DataSet(*trn), batch_size=bs), 
    #                WeightedDataLoader(DataSet(*val), batch_size=bs))

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
    
    print("*********************")
    print("Summary training data")
    print("Features", features)
    print("Shape systematics", shape_syst)
    print("Weight systematics", weight_syst)
    print("Use weights", use_weights)
    n_sig_train = sum(samples["TTJets_signal"]["is_train"]==1)
    print("Number of signal training / test events:", n_sig_train, len(samples["TTJets_signal"]) - n_sig_train)
    n_bkg_train = sum(samples["QCD"]["train_flag"]=="train")
    print("Number of bkg training / test events:", n_bkg_train, len(samples["QCD"]) - n_bkg_train)
    print("*********************")
        
    return trn, val, scaler

        
def load_data(features, shape_syst, weight_syst, norm_syst, path = "/home/centos/data/bdt_rs5/", bs=256, 
              n_sig = 5000, n_bkg = 5000, use_weights = False):
    
    #if not os.path.exists(outpath):
    #    os.makedirs(outpath)
    
    samples = {}
    for s in ["TTJets_bkg", "WZJets", "STJets", "QCD", "TTJets_signal", "Data"]:
        samples[s] = pd.read_hdf(path + s + ".h5")
        
        if ("Data" not in s) & ("QCD" not in s):
            for syst in ["06_jes", "05_taue", "jer"]:
                samples[s + "_" + syst + "_up"] = pd.read_hdf(path + s + "_" + syst + "_up" + ".h5")
                samples[s + "_" + syst + "_down"] = pd.read_hdf(path + s + "_" + syst + "_down" + ".h5")
                
    trn, val, scaler = get_cmsopen_data(samples, features, shape_syst, weight_syst, 
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
   







