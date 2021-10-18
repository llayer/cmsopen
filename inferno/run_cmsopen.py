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

import os
import copy
from functools import reduce


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

    # Split in training and test
    train_test_split(signal, n_sig)
    train_test_split(bkg, n_bkg)
    train_idx = get_train_evts(signal)  
    
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
    for syst in shape_syst:
        for ud in ["_up", "_down"]:    
            feat_syst = [x + "_" + syst + ud for x in features]
            signal_train_syst.append(scaler.transform(train_data[feat_syst].values))
            signal_test_syst.append(scaler.transform(test_data[feat_syst].values))
    
    # Add the weights
    for syst in weight_syst:
        for ud in ["_up", "_down"]:
            signal_train_syst.append(X_train*train_data[syst + ud].values.reshape((-1,1)))
            signal_test_syst.append(X_test*test_data[syst + ud].values.reshape((-1,1)))
    
    X_train = np.stack((X_train, *signal_train_syst), axis=2)
    X_test = np.stack((X_test, *signal_test_syst), axis=2)
    
    if use_weights == True:
        trn = (X_train, y_train, weights_train)  
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




def run_inferno(data, epochs=20, inferno_args=None, train_args=None):
    
    class ApproxCMSOpenInferno(AbsApproxInferno):
        r'''Inheriting class for dealing with INFERNO paper synthetic problem'''
        @delegates(AbsApproxInferno, but=['b_shape_alpha', 's_shape_alpha'])
        def __init__(self, b_true:float=2800, mu_true:float=300,  s_shape_alpha=True, **kwargs):
            super().__init__(b_true=b_true, mu_true=mu_true, **kwargs)
            print("*********************")
            print("Summary INFERNO setup")
            print("b_true", self.b_true)
            print("mu_true", self.mu_true)
            print("nshape_alphas", self.n_shape_alphas)
            print("shape_aux", self.shape_aux)
            print("s_norm_aux", self.s_norm_aux)
            print("n_alpha", self.n_alpha)
            print("*********************")

        def on_train_begin(self) -> None:
            super().on_train_begin()
            pass

        def _get_up_down(self, x_s:Tensor, x_b:Tensor, **kwargs) -> Tuple[Tuple[Optional[Tensor],Optional[Tensor]],Tuple[Optional[Tensor],Optional[Tensor]]]:

            u,d = [],[]

            # modified template variations
            for i in range(self.n_shape_alphas):

                idx_up = 1 + 2*i
                idx_down = 2 + 2*i

                up_batch = self.to_shape(self.wrapper.model(x_s[:,:,idx_up]))
                down_batch = self.to_shape(self.wrapper.model(x_s[:,:,idx_down]))

                u.append(up_batch)
                d.append(down_batch)    

            return (torch.stack(u),torch.stack(d)), (None,None)

        
    if train_args is not None:
        lr = train_args["lr"]
        n_feats = train_args["n_feats"]
        neurons = train_args["neurons"]
        bins = train_args["bins"]
        temperature = train_args["temperature"]
        weights = train_args["weights"]
    else:
        lr = 1e-3
        n_feats = 4
        neurons = 100
        bins = 10
        temperature = 0.1
        weights = False

    print("*********************")
    print("Summary NN setup")
    print("learning rate", lr)
    print("n_feats", n_feats)
    print("neurons", neurons)
    print("temperature", temperature)
    print("bins", bins)
    print("weights", weights)
    print("*********************")
    
    # Set up network
    net_inferno = nn.Sequential(nn.Linear(n_feats,neurons),  nn.ReLU(),
                    nn.Linear(neurons,neurons), nn.ReLU(),
                    nn.Linear(neurons,bins), VariableSoftmax(temperature))
    lt = LossTracker()
    #init_net(net_inferno)
    model_inferno = ModelWrapper(net_inferno)
    
    
    if inferno_args is not None:
        # Signal and bkg
        b_true = inferno_args["b_true"]
        mu_true = inferno_args["mu_true"]
        # Number of shape nuisances
        n_shape_alphas = inferno_args["n_shape_alphas"]
        # Constraints on the nuisance parameters - typically should be N(0,1) if +-1sigma templates are provided
        shape_aux = inferno_args["shape_aux"]
        # Uncertainties on signal norm - should be Normal(0,s) with s in absolute event numbers
        s_norm_aux = inferno_args["s_norm_aux"]
        # Boolean whether to let the background float
        nonaux_b_norm = inferno_args["nonaux_b_norm"]
    else:
        b_true = 2800
        mu_true = 300
        n_shape_alphas = 2
        shape_aux = None  
        s_norm_aux = None
        nonaux_b_norm = False
    
    model_inferno.fit(epochs, data=data, opt=partialler(optim.Adam,lr=lr), loss=None,
                      cbs=[ApproxCMSOpenInferno(b_true=b_true, mu_true=mu_true, n_shape_alphas=n_shape_alphas, 
                                                shape_aux=shape_aux, s_norm_aux=s_norm_aux, nonaux_b_norm=nonaux_b_norm), lt])  
    
    return model_inferno, lt



def train_bce(data, epochs=50, lr=1e-3):
    
    net_bce = nn.Sequential(nn.Linear(4,12),  nn.ReLU(),
                            nn.Linear(12,8), nn.ReLU(),
                            nn.Linear(8,1),  nn.Sigmoid())
    #init_net(net)    
    lt = LossTracker()
    model_bce = ModelWrapper(net_bce)
    model_bce.fit(epochs, data=data, opt=partialler(optim.Adam), loss=nn.BCELoss(),
                  cbs=[lt])  
    return model_bce, lt



def pred_nominal(samples, features, model, scaler, name):
    #"TTJets_signal"
    for s in samples:
        X = samples[s][features].values
        X = scaler.transform(X)
        loader = WeightedDataLoader(DataSet(X, None, None), batch_size=256)
        if "bce" in name:
            samples[s][name] = model._predict_dl(loader)
        else:
            samples[s][name] = model._predict_dl(loader, pred_cb=InfernoPred())

        
def plot_loss(lt, outpath, name="inferno", store=True):
    
    plt.plot(lt.losses["trn"], label="train")
    plt.plot(lt.losses["val"], label="val")
    plt.ylabel(r"$\sigma^2(\mu)$")
    plt.xlabel(r"epoch")
    plt.legend(loc="upper right")
    if store:
        plt.savefig(outpath + "/loss_" + name + ".png")
    plt.show()
        
        
def plot_predictions(model, test_dl, outpath, name="inferno", store=True):

    if name == "inferno":
        preds = model._predict_dl(test_dl, pred_cb=InfernoPred())
    else:
        preds = model._predict_dl(test_dl).squeeze()
        
    df = pd.DataFrame({'pred':preds})
    df['gen_target'] = test_dl.dataset.y
    
    sig = df[df["gen_target"]==1]["pred"]
    bkg = df[df["gen_target"]==0]["pred"]
    if name == "inferno":
        hist_range=(0,10)
    else:
        hist_range=(0,1.)
    plt.hist(sig, density=True, alpha=0.5, bins=10, range=hist_range, label="Signal")
    plt.hist(bkg, density=True, alpha=0.5, bins=10, range=hist_range, label="Background")
    plt.legend(loc="upper left")
    if store:
        plt.savefig(outpath + "/preds_" + name + ".png")    
    plt.show()
        
        
def train(outpath, features, shape_syst, weight_syst, path = "/home/centos/data/bdt_rs5/", bs=256, epochs = 50, 
          n_sig = 5000, n_bkg = 5000, inferno_args = None, train_args = None, use_weights = False, store=True):
    
    if not os.path.exists(outpath):
        os.makedirs(outpath)
    
    samples = {}
    for s in ["TTJets_bkg", "WZJets", "STJets", "QCD", "TTJets_signal", "Data"]:
        samples[s] = pd.read_hdf(path + s + ".h5")
        if s == "TTJets_signal":
            for syst in["jes", "jer", "met", "taue"]:
                samples[s + "_" + syst + "_up"] = pd.read_hdf(path + s + "_" + syst + "_up" + ".h5")
                samples[s + "_" + syst + "_down"] = pd.read_hdf(path + s + "_" + syst + "_down" + ".h5")

    trn, val, scaler = get_cmsopen_data(samples, features, shape_syst, weight_syst, 
                                        n_sig = n_sig, n_bkg = n_bkg, use_weights = use_weights)    
    trn_dl = WeightedDataLoader(DataSet(*trn), batch_size=bs, shuffle=True, drop_last=True)
    val_dl = WeightedDataLoader(DataSet(*val), batch_size=bs, shuffle=True)
    test_dl = WeightedDataLoader(DataSet(*val), batch_size=bs)
    data = DataPair(trn_dl, val_dl)
    
    print("*********************")
    print("Summary dataloader")
    print("batch size", bs)
    print("epochs", epochs)
    print("x", trn_dl.dataset[0][0].shape)
    print("y", trn_dl.dataset[0][1])
    print("w", trn_dl.dataset[0][2])
    print("*********************")

    # Run INFERNO
    
    model_inferno, lt_inferno = run_inferno(data, epochs=epochs, inferno_args = inferno_args, train_args = train_args)
    # Monitor training
    plot_predictions(model_inferno, test_dl, outpath, name="inferno", store=store)
    plot_loss(lt_inferno, outpath, name="inferno", store=store)
    
    """
    # BCE for comparison
    model_bce, lt_bce = train_bce(data, epochs=50)
    # Monitor training
    plot_predictions(model_bce, test_dl, outpath, name="bce", store=store)
    plot_loss(lt_bce, outpath, name="bce", store=store)
    # Predict the shapes
    pred_nominal(samples, features, model_bce, scaler, name="bce")        # BCE shift               
    """
     
    
    # Store
    if store:
        # Predict the shapes
        pred_nominal(samples, features, model_inferno, scaler, name='inferno')
        for s in samples:
            samples[s].to_hdf(outpath + "/" + s + ".h5", "frame")
    
    return lt_inferno
    






