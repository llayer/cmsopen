from pytorch_inferno.model_wrapper import ModelWrapper
from pytorch_inferno.callback import *
from pytorch_inferno.data import get_paper_data
from pytorch_inferno.plotting import *
from pytorch_inferno.inference import *
from pytorch_inferno.utils import *
from pytorch_inferno.inferno import *

from fastcore.all import partialler
from fastcore.all import store_attr, delegates, is_close
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
import itertools
from typing import *
from collections import OrderedDict
from fastcore.all import store_attr
from abc import abstractmethod

import torch.nn.functional as F
from torch import optim, autograd, nn, Tensor
import torch
from torch.distributions import Normal

pd.options.mode.chained_assignment = None    
    
from sklearn import preprocessing
from pytorch_inferno.data import *
import random

import copy
from functools import reduce

#features = ['ht', 'aplanarity', 'sphericity', 'chargeEta', 'MET_met', 'deltaPhiTauMet', 'mt', 'mTauJet']
features = ['aplanarity', 'chargeEta', 'MET_met', 'deltaPhiTauMet']
systematics = ["_jes_up", "_jes_down"]

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
    
    
def get_cmsopen_data(samples, n_sig = 20000, n_bkg = 10000, bs=256):
        
    # Add a unique event key
    for s in samples:
        add_key(samples[s])
    
    # Add a suffix to be able to merge the framees
    for syst in systematics:
        samples["TTJets_signal" + syst] = samples["TTJets_signal" + syst].add_suffix(syst)
        samples["TTJets_signal" + syst].rename(columns={'event_id' + syst: 'event_id'}, inplace=True)        
    
    # Merge the nominal and systematic frames
    dfs = []
    dfs.append(samples["TTJets_signal"])
    for syst in systematics:
        dfs.append(samples["TTJets_signal" + syst])
        
    signal = reduce(lambda  left,right: pd.merge(left,right,how="inner", on="event_id"), dfs)
    
    # Split in training and test
    bkg = samples["QCD"]
    train_test_split(signal, n_sig)
    train_test_split(bkg, n_bkg)
    
    # Add labels
    signal["label"] = 1
    bkg["label"] = 0
    
    # Select test and train and add weights
    signal_train = signal[signal["train_flag"] == "train"]
    signal_train["weights"] = signal_train['trigger_weight'] * signal_train['btag_weight1']
    signal_train["weights"] = signal_train["weights"] * (1. / np.mean(signal_train["weights"]))
    signal_test = signal[signal["train_flag"] == "test"]
    bkg_train = bkg[bkg["train_flag"] == "train"]
    bkg_train["weights"] = bkg_train['btag_weight2']
    bkg_train["weights"] = bkg_train["weights"] * (1. / np.mean(bkg_train["weights"]))
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
    
    # Add the systematic shape variations
    signal_train_syst= []
    signal_test_syst = []
    for syst in systematics:
        feat_syst = [x + syst for x in features]
        signal_train_syst.append(scaler.transform(train_data[feat_syst].values))
        signal_test_syst.append(scaler.transform(test_data[feat_syst].values))
    
    # Add the weights
    for syst in ["btag_weight1_up", "btag_weight1_down"]:
        signal_train_syst.append(X_train*train_data[syst].values.reshape((-1,1)))
        signal_test_syst.append(X_test*test_data[syst].values.reshape((-1,1)))
    
    X_train = np.stack((X_train, *signal_train_syst), axis=2)
    X_test = np.stack((X_test, *signal_test_syst), axis=2)
    
    trn = (X_train, y_train) #, weights_train)    
    val = (X_test, y_test)
    
    trn_dl = WeightedDataLoader(DataSet(*trn), batch_size=bs, shuffle=True, drop_last=True)
    val_dl = WeightedDataLoader(DataSet(*val), batch_size=bs, shuffle=True)
    
    #data = DataPair(trn_dl, val_dl)
    #test = DataPair(WeightedDataLoader(DataSet(*trn), batch_size=bs), 
    #                WeightedDataLoader(DataSet(*val), batch_size=bs))

    return trn_dl, val_dl


def train(path = "/home/centos/data/bdt_rs5/", store=False):
    
    
    samples = {}
    for s in ["TTJets_bkg", "WZJets", "STJets", "QCD", "TTJets_signal", "Data"]:
        samples[s] = pd.read_hdf(path + s + ".h5")
        if s == "TTJets_signal":
            for syst in["jes", "jer", "met", "taue"]:
                samples[s + "_" + syst + "_up"] = pd.read_hdf(path + s + "_" + syst + "_up" + ".h5")
                samples[s + "_" + syst + "_down"] = pd.read_hdf(path + s + "_" + syst + "_down" + ".h5")


    trn_dl, val_dl = get_cmsopen_data(samples, n_sig = 5000, n_bkg = 5000, bs=256)    

    return trn_dl, val_dl
    






