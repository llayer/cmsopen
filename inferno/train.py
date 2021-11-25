from pytorch_inferno.inferno import *
from pytorch_inferno.callback import *
from pytorch_inferno.model_wrapper import *
from fastcore.all import partialler
from torch import optim, autograd, nn, Tensor

import hep_model
import plot

def train_inferno(data, args, epochs=100 ):
    
    # Set up network
    lr = args["inferno_lr"]
    temp = args["temperature"]
    neurons = args["inferno_neurons"]
    bins = args["bins"]
    nfeat = args["nfeat"]
    if args["use_softhist"] == False:
        
        net_inferno = nn.Sequential(nn.Linear(nfeat,neurons),  nn.ReLU(),
                        nn.Linear(neurons,neurons), nn.ReLU(),
                        nn.Linear(neurons,bins), VariableSoftmax(temp))

    else:
        net_inferno = nn.Sequential(nn.Linear(nfeat,neurons),  nn.ReLU(),
                                nn.Linear(neurons,neurons), nn.ReLU(),
                                nn.Linear(neurons,1),  nn.Sigmoid())

    
    lt = LossTracker()
    hep_inf = hep_model.HEPInferno(b_true=args["b_true"], 
                                   mu_true=args["mu_true"],
                                   n_shape_systs=args["n_shape_systs"],
                                   n_weight_systs=args["n_weight_systs"],
                                   shape_norm_sigma=args["shape_norm_sigma"],
                                   s_norm_sigma = args["s_norm_sigma"],
                                   b_norm_sigma = args["b_norm_sigma"],
                                   b_rate_param = args["b_rate_param"],
                                   use_hist=args["use_softhist"],
                                   bins = args["bins"],
                                   sigmoid_delta = args["sigmoid_delta"],
                                   interp_algo=args["interp_algo"])
    #init_net(net_inferno)
    model_inferno = ModelWrapper(net_inferno)

    model_inferno.fit(epochs, data=data, opt=partialler(optim.Adam,lr=lr), loss=None,
                      cbs=[hep_inf,  lt])
        
    return model_inferno, {"loss":lt, "covs": hep_inf.covs}


def train_bce(data, args, epochs=100):
    
    lr = args["bce_lr"]
    neurons = args["bce_neurons"]
    nfeat = args["nfeat"]   
    neurons = args["bce_neurons"]
    net_bce = nn.Sequential(nn.Linear(nfeat,neurons),  nn.ReLU(),
                        nn.Linear(neurons,8), nn.ReLU(),
                        nn.Linear(8,1),  nn.Sigmoid())
    #init_net(net)    
    ct = hep_model.HEPInferno(b_true=inferno_args["b_true"], 
                              mu_true=inferno_args["mu_true"],
                               n_shape_systs=inferno_args["n_shape_systs"],
                               n_weight_systs=inferno_args["n_weight_systs"],
                               shape_norm_sigma=inferno_args["shape_norm_sigma"],
                               s_norm_sigma = inferno_args["s_norm_sigma"],
                               b_norm_sigma = inferno_args["b_norm_sigma"],
                               b_rate_param = inferno_args["b_rate_param"],
                               bins = args["bins"],
                               use_hist=True,
                               interp_algo="default")
    lt = LossTracker()
    model_bce = ModelWrapper(net_bce)
    model_bce.fit(epochs, data=data, opt=partialler(optim.Adam, lr=lr), loss=nn.BCELoss(),
                  cbs=[lt, ct])
    
    #bce_trn_covs = ct.covs["trn"]
    #bce_val_covs = ct.covs["val"]
    return model_bce, {"loss":lt, "covs": ct.covs}


    