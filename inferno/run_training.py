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

#features = ['ht', 'aplanarity', 'sphericity', 'chargeEta', 'MET_met', 'deltaPhiTauMet', 'mt', 'mTauJet']
features = ['aplanarity', 'chargeEta', 'MET_met', 'deltaPhiTauMet']


def shift_pred(X, axis, shift):
    X_var = copy.deepcopy(X)
    X_var[:,axis] += shift
    return X_var


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
    
def get_cmsopen_data(samples, n_sig = 20000, n_bkg = 10000, bs=1000):
        
    #signal = samples["TTJets_signal"] #samples["TTJets_centJER_signal"] #samples["TTJets_signal_central"]
    #bkg = samples["QCD"]
    
    signal = samples["TTJets_signal"] #samples["TTJets_centJER_signal"] #samples["TTJets_signal_central"]
    bkg = samples["QCD"]
    
    train_test_split(signal, n_sig)
    train_test_split(bkg, n_bkg)
    
    print(list(signal))
    
    signal["label"] = 1
    bkg["label"] = 0
    
    signal_train = signal[signal["train_flag"] == "train"]
    signal_train["weights"] = signal_train['trigger_weight'] * signal_train['btag_weight1']
    signal_train["weights"] = signal_train["weights"] * (1. / np.mean(signal_train["weights"]))
    signal_test = signal[signal["train_flag"] == "test"]
    bkg_train = bkg[bkg["train_flag"] == "train"]
    bkg_train["weights"] = bkg_train['btag_weight2']
    bkg_train["weights"] = bkg_train["weights"] * (1. / np.mean(bkg_train["weights"]))
    bkg_test = bkg[bkg["train_flag"] == "test"]
    
    train_data = pd.concat([signal_train, bkg_train], axis=0)
    test_data = pd.concat([signal_test, bkg_test], axis=0)
    
    X_train = train_data[features].values
    # Scale
    scaler = preprocessing.StandardScaler().fit(X_train)
    X_train = scaler.transform(X_train)
    
    y_train = train_data["label"].values.reshape((-1,1))
    weights_train = train_data["weights"].values.reshape((-1,1))
    
    X_test = test_data[features].values
    X_test = scaler.transform(X_test)
    y_test = test_data["label"].values.reshape((-1,1))
    #weights_test= test_data["weights"]

    
    trn = (X_train, y_train) #, weights_train)
    val = (X_test, y_test)
    
    trn_dl = WeightedDataLoader(DataSet(*trn), batch_size=bs, shuffle=True, drop_last=True)
    val_dl = WeightedDataLoader(DataSet(*val), batch_size=bs, shuffle=True)
    data = DataPair(trn_dl, val_dl)
    test = DataPair(WeightedDataLoader(DataSet(*trn), batch_size=bs), 
                    WeightedDataLoader(DataSet(*val), batch_size=bs))
    
    return data, test, scaler


def train_bce(data, epochs=50, lr=1e-3):
    
    net_bce = nn.Sequential(nn.Linear(4,12),  nn.ReLU(),
                            nn.Linear(12,8), nn.ReLU(),
                            nn.Linear(8,1),  nn.Sigmoid())
    #init_net(net)    
    model_bce = ModelWrapper(net_bce)
    model_bce.fit(epochs, data=data, opt=partialler(optim.Adam), loss=nn.BCELoss(),
                  cbs=[LossTracker()])  
    return model_bce


def train_inferno(data, approx=False, n_shape_alphas=1, shift=0.3, epochs=100, lr=1e-3):
    
    
    class ApproxCMSOpenInferno(AbsApproxInferno):
        r'''Inheriting class for dealing with INFERNO paper synthetic problem'''
        @delegates(AbsApproxInferno, but=['b_shape_alpha', 's_shape_alpha'])
        def __init__(self, b_true:float=2800, mu_true:float=300,  s_shape_alpha=True, shift = 0.5, **kwargs):
            super().__init__(b_true=b_true, mu_true=mu_true, **kwargs)
            self.shift = shift
            print("shift", self.shift)
            print("nshape_alphas", self.n_shape_alphas)
            print("shape_aux", self.shape_aux)
            
        def on_train_begin(self) -> None:
            super().on_train_begin()
            pass
        
        def _get_up_down(self, x_s:Tensor, x_b:Tensor, **kwargs) -> Tuple[Tuple[Optional[Tensor],Optional[Tensor]],Tuple[Optional[Tensor],Optional[Tensor]]]:
            
            u,d = [],[]

            # up variations
            for axis in range(self.n_shape_alphas):
                x_s_up = x_s.detach().clone()
                x_s_up[:,axis] += self.shift
                u.append(self.to_shape(self.wrapper.model(x_s_up)))

                # down variations
                x_s_down = x_s.detach().clone()
                x_s_down[:,axis] -= self.shift
                d.append(self.to_shape(self.wrapper.model(x_s_down)))
                
                #print("axis = ", axis, "nom", x_s[0:10], "up", x_s_up[0:10],  "down",  x_s_down[0:10])
                      
            return (torch.stack(u),torch.stack(d)), (None,None)
    
    
    class CMSOpenInferno(AbsInferno):
        r'''Inheriting class for dealing with INFERNO paper synthetic problem'''
        @delegates(AbsInferno, but=['b_shape_alpha', 's_shape_alpha'])
        def __init__(self, b_true:float=2800, mu_true:float=300, **kwargs):
            super().__init__(b_true=b_true, mu_true=mu_true, s_shape_alpha=True, **kwargs)
            print("nshape_alphas", self.n_shape_alphas)
            print("shape_aux", self.shape_aux)

        def _aug_data(self, x:Tensor) -> None:

            x[~self.b_mask,0] += self.alpha[self.shape_idxs[0]]
            if n_shape_alphas > 1:
                x[~self.b_mask,1] += self.alpha[self.shape_idxs[1]]
            if n_shape_alphas > 2:
                x[~self.b_mask,2] += self.alpha[self.shape_idxs[2]]
            if n_shape_alphas > 3:
                x[~self.b_mask,3] += self.alpha[self.shape_idxs[3]]
                        
    net_inferno = nn.Sequential(nn.Linear(4,100),  nn.ReLU(),
                        nn.Linear(100,100), nn.ReLU(),
                        nn.Linear(100,10), VariableSoftmax(0.1))
    #init_net(net)
    model_inferno = ModelWrapper(net_inferno)

    lt = LossTracker()
    shape_aux=[]
    for _ in range(n_shape_alphas):
        shape_aux.append(Normal(0,shift))
    if approx == False:
        model_inferno.fit(epochs, data=data, opt=partialler(optim.Adam,lr=lr), loss=None,
                          cbs=[CMSOpenInferno(shape_aux=shape_aux, n_shape_alphas=n_shape_alphas), lt])    
    else:
        print("approx")
        model_inferno.fit(epochs, data=data, opt=partialler(optim.Adam,lr=lr), loss=None,
                          cbs=[ApproxCMSOpenInferno(n_shape_alphas=n_shape_alphas, shift=shift), lt])    
    
    return model_inferno
                    

def pred_nominal(samples, model, scaler, name):
    #"TTJets_signal"
    for s in samples:
        X = samples[s][features].values
        X = scaler.transform(X)
        loader = WeightedDataLoader(DataSet(X, None, None), batch_size=256)
        if "bce" in name:
            samples[s][name] = model._predict_dl(loader)
        else:
            samples[s][name] = model._predict_dl(loader, pred_cb=InfernoPred())

    
def pred_shifted(sample, model, scaler, axis = 0, shift = 0.5, name='bce'):
    
    print("shifting by", shift, "along", axis)
      
    X = sample[features].values
    X = scaler.transform(X)
    X_up = shift_pred(X,axis,shift)
    X_down = shift_pred(X,axis,-shift)
    if "bce" in name:
        sample[name + "_up"] = model._predict_dl(WeightedDataLoader(DataSet(X_up, None, None), batch_size=256))
        sample[name + "_down"] = model._predict_dl(WeightedDataLoader(DataSet(X_down, None, None), batch_size=256))               
    else:
        sample[name + "_up"] = model._predict_dl(WeightedDataLoader(DataSet(X_up, None, None), batch_size=256), 
                                                     pred_cb=InfernoPred())
        sample[name + "_down"] = model._predict_dl(WeightedDataLoader(DataSet(X_down, None, None), batch_size=256),
                                                   pred_cb=InfernoPred())


        
def train(path = "/home/centos/data/bdt_rs5/", store=False):
    
    # Load samples
    samples = {}
    for s in ["TTJets_bkg", "WZJets", "STJets", "QCD", "TTJets_signal", "Data"]:
        samples[s] = pd.read_hdf(path + s + ".h5")
    data, test, scaler = get_cmsopen_data(samples, n_sig = 5000, n_bkg = 5000, bs=256)
    
    # Train models
    
    model_bce = train_bce(data, epochs=50)
    pred_nominal(samples, model_bce, scaler, name="bce_nominal")
    
    #model_inferno = train_inferno(data, approx=True,  n_shape_alphas=2, shift=0.3, epochs=10)
    # Train INFERNO models with shift in one var
    
    
    for counter, s in enumerate(np.linspace(0.1,1.,10)):
        
        for approx in [False, True]:
            model_inferno = train_inferno(data, approx=approx, shift=s, epochs=50)
            if approx: suffix = "approx" 
            else: suffix = "analytical"
            # INFERNO nominal
            pred_nominal(samples, model_inferno, scaler, name='inferno_shift_' + suffix + '_nominal_' + str(counter))
            # INFERNO shift
            pred_shifted(samples["TTJets_signal"], model_inferno, scaler, axis = 0, shift = s, 
                         name='inferno_shift_' + suffix + '_' + str(counter))
        # BCE shift
        pred_shifted(samples["TTJets_signal"], model_bce, scaler, axis = 0, shift = s, name='bce_shift_' + str(counter))
        
    
    
    # Train INFERNO with shift in several nuisances
    for n_nuis in range(1,5):
        
        shift=0.35
        for approx in [False, True]:
            if approx: suffix = "approx" 
            else: suffix = "analytical"
                
            model_inferno = train_inferno(data, approx=approx, n_shape_alphas=n_nuis, shift=shift, epochs=50)
            
            # INFERNO nominal
            pred_nominal(samples, model_inferno, scaler, name='inferno_nuis_' + suffix + '_nominal_' + str(n_nuis))

            # INFERNO shift
            for iNuis in range(n_nuis):
                pred_shifted(samples["TTJets_signal"], model_inferno, scaler, axis = iNuis, shift = shift, 
                             name='inferno_nuis_' + suffix + '_' + str(n_nuis) + "_var_" + str(iNuis))
        # BCE shift
        pred_shifted(samples["TTJets_signal"], model_bce, scaler, axis = n_nuis-1, shift = shift, name='bce_nuis_' + str(n_nuis))
    
    
    # Store
    if store:
        outpath = "/home/centos/data/inferno_systematic5"
        for s in samples:
            samples[s].to_hdf(outpath + "/" + s + ".h5", "frame")
    
    print(list(samples["TTJets_signal"]))
   
    return samples
    

if __name__ == "__main__":

    train(store=True)
    
    
    
    
