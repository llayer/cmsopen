from pytorch_inferno.inferno import *
from pytorch_inferno.callback import *
from pytorch_inferno.model_wrapper import *
from pytorch_inferno.data import *
from fastcore.all import partialler
from torch import optim, autograd, nn, Tensor
import pandas as pd
import numpy as np

import training.hep_model as hep_model

def train_inferno(data, args, epochs=100 ):
    
    # Set up network
    lr = args["inferno_lr"]
    temp = args["temperature"]
    neurons = args["inferno_neurons"]
    bins = args["inferno_bins"]
    nfeat = len(args["features"])
    
    print("*********************")
    print("Summary INFERNO model")
    print("*********************")
    print("Learning rate", lr)
    print("Temperature", temp)
    print("Neurons", neurons)
    print("Bins", bins)
    
    if args["use_softhist"] == False:
        
        net_inferno = nn.Sequential(nn.Linear(nfeat,neurons),  nn.ReLU(),
                        nn.Linear(neurons,neurons), nn.ReLU(),
                        nn.Linear(neurons,bins), VariableSoftmax(temp))

    else:
        net_inferno = nn.Sequential(nn.Linear(nfeat,neurons),  nn.ReLU(),
                                nn.Linear(neurons,neurons), nn.ReLU(),
                                nn.Linear(neurons,1),  nn.Sigmoid())
    
    lt = LossTracker()
    sb = SaveBest(args["outpath"] + "/weights/best_inferno.h5")
    hep_inf = hep_model.HEPInferno(b_true=args["b_true"], 
                                   mu_true=args["mu_true"],
                                   n_shape_systs=len(args["shape_syst"]),
                                   n_weight_systs=len(args["weight_syst"]),
                                   shape_norm_sigma=args["shape_norm_sigma"],
                                   is_sig_shape = args["is_sig_shape"],
                                   s_norm_sigma = list(args["s_norm_sigma"].values()),
                                   b_norm_sigma = list(args["b_norm_sigma"].values()),
                                   b_rate_param = args["b_rate_param"],
                                   use_hist=args["use_softhist"],
                                   bins = bins,
                                   sigmoid_delta = args["sigmoid_delta"],
                                   ignore_shape_norm=args["ignore_shape_norm"],
                                   asymm_shape_norm = args["asymm_shape_norm"],
                                   store_significance = args["store_significance"],
                                   interp_algo=args["interp_algo"])
    #init_net(net_inferno)
    model_inferno = ModelWrapper(net_inferno)

    model_inferno.fit(epochs, data=data, opt=partialler(optim.Adam,lr=lr), loss=None,
                      cbs=[hep_inf,  lt, sb])        
    
    idx_best = lt.losses["val"].index(sb.min_loss)
    print("Evaluating best INFERNO model from epoch", idx_best, "with loss", lt.losses["val"][idx_best])
    
    shapes = {"bkg" : hep_inf.val_shapes["bkg"],
              "sig" : hep_inf.val_shapes["sig"],
              "bkg_trn": hep_inf.trn_shapes["bkg"],
              "sig_trn": hep_inf.trn_shapes["sig"],
              #"sig_up" : hep_inf.val_shapes["sig_up"],
              #"sig_down" : hep_inf.val_shapes["sig_down"]
              "up" : hep_inf.val_shapes["up"],
              "down" : hep_inf.val_shapes["down"]
             }
        
    return model_inferno, {"loss":lt, "idx_best": idx_best, "covs": hep_inf.covs, "shapes" : shapes}


def train_bce(data, args, epochs=100):
    
    lr = args["bce_lr"]
    neurons = args["bce_neurons"]
    nfeat = len(args["features"])  
    neurons = args["bce_neurons"]
    bins = args["bce_bins"]
    
    """
    net_bce = nn.Sequential(nn.Linear(nfeat,neurons),  nn.ReLU(),
                        nn.Linear(neurons,8), nn.ReLU(),
                        nn.Linear(8,1),  nn.Sigmoid())
    """
    net_bce = nn.Sequential(nn.Linear(nfeat,neurons),  nn.ReLU(),
                        nn.Linear(neurons,neurons), nn.ReLU(),
                        nn.Linear(neurons,1),  nn.Sigmoid())
    
    print("*********************")
    print("Summary BCE model")
    print("*********************")
    print("Learning rate", lr)
    print("Neurons", neurons)
    print("Bins", bins)
    
    #init_net(net)    
    ct = hep_model.HEPInferno(b_true=args["b_true"], 
                              mu_true=args["mu_true"],
                               n_shape_systs=len(args["shape_syst"]),
                               n_weight_systs=len(args["weight_syst"]),
                               shape_norm_sigma=args["shape_norm_sigma"],
                               is_sig_shape = args["is_sig_shape"],
                               s_norm_sigma = list(args["s_norm_sigma"].values()),
                               b_norm_sigma = list(args["b_norm_sigma"].values()),
                               b_rate_param = args["b_rate_param"],
                               bins = bins,
                               use_hist=True,
                               ignore_loss=True,
                               ignore_shape_norm=args["ignore_shape_norm"],
                               asymm_shape_norm = args["asymm_shape_norm"],
                               store_significance = args["store_significance"],
                               interp_algo=args["interp_algo"])
    lt = LossTracker()
    sb = SaveBest(args["outpath"] + "/weights/best_bce.h5")
    model_bce = ModelWrapper(net_bce)
    model_bce.fit(epochs, data=data, opt=partialler(optim.Adam, lr=lr), loss=nn.BCELoss(),
                  cbs=[lt, ct, sb])

    idx_best = lt.losses["val"].index(sb.min_loss)
    print("Evaluating best BCE model from epoch", idx_best, "with loss", lt.losses["val"][idx_best])
    
    shapes = {"bkg" : ct.val_shapes["bkg"],
              "sig" : ct.val_shapes["sig"],
              "bkg_trn": ct.trn_shapes["bkg"],
              "sig_trn": ct.trn_shapes["sig"],
              #"sig_up" : ct.val_shapes["sig_up"],
              #"sig_down" : ct.val_shapes["sig_down"],
              "up" : ct.val_shapes["up"],
              "down" : ct.val_shapes["down"],
             }
    
    #bce_trn_covs = ct.covs["trn"]
    #bce_val_covs = ct.covs["val"]
    return model_bce, {"loss":lt, "idx_best": idx_best, "covs": ct.covs, "shapes": shapes}



#
# Predict test set
#

def pred_test(model, test_dl, use_hist=False, name="inferno", bins=10.):

    if use_hist == True:
        preds = model._predict_dl(test_dl).squeeze()        
    else:
        preds = model._predict_dl(test_dl, pred_cb=InfernoPred())
        
    df = pd.DataFrame({'pred':preds})
    df['gen_target'] = test_dl.dataset.y
        
    if ("inferno" in name) & (use_hist == False):
        
        #Sort according to signal fraction
        sig = df[df["gen_target"]==1]["pred"]
        bkg = df[df["gen_target"]==0]["pred"]
        x_range = (0.,bins)
        sig_h = np.histogram(sig, bins=bins, range=x_range, density=True)[0]
        bkg_h = np.histogram(bkg, bins=bins, range=x_range, density=True)[0]
        sig_bkg = sig_h/(bkg_h+10e-7)
        sor = np.argsort(sig_bkg)
        inv_d = dict(enumerate(np.argsort(sig_bkg)))  
        order_d = {v: k for k, v in inv_d.items()}
        df['pred_sorted'] = df["pred"].replace(order_d)
    else:
        order_d = {}

    return df, order_d


#
# Predict the nominal samples
#

def pred_nominal(samples, features, model, scaler, name, sort_bins = False, use_hist = False, order_d = None):
    
    print("*********************")
    print("Predicting", name, "samples")
    
    #"TTJets_signal"
    for s in samples:
        X = samples[s][features].values
        X = scaler.transform(X)
        loader = WeightedDataLoader(DataSet(X, None, None), batch_size=1000)
        if use_hist == True:
            samples[s][name] = model._predict_dl(loader)
        else:
            samples[s][name] = model._predict_dl(loader, pred_cb=InfernoPred())
            if sort_bins == True:
                samples[s][name] = samples[s][name].replace(order_d)
            
            




