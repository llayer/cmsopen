from pytorch_inferno.inferno import *
from pytorch_inferno.callback import *
from pytorch_inferno.model_wrapper import *
from fastcore.all import partialler
from torch import optim, autograd, nn, Tensor

import hep_model
import plot

def train_inferno(data, test, epochs=100, inferno_args=None, use_softhist=False):
    
    # Set up network
    if use_softhist == False:
        net_inferno = nn.Sequential(nn.Linear(4,100),  nn.ReLU(),
                        nn.Linear(100,100), nn.ReLU(),
                        nn.Linear(100,10), VariableSoftmax(0.1))

    else:
        net_inferno = nn.Sequential(nn.Linear(4,100),  nn.ReLU(),
                                nn.Linear(100,100), nn.ReLU(),
                                nn.Linear(100,1),  nn.Sigmoid())

    
    lt = LossTracker()
    hep_inf = hep_model.HEPInferno(b_true=inferno_args["b_true"], 
                                   mu_true=inferno_args["mu_true"],
                                   n_shape_systs=inferno_args["n_shape_systs"],
                                   n_weight_systs=inferno_args["n_weight_systs"],
                                   shape_norm_sigma=inferno_args["shape_norm_sigma"],
                                   #s_norm_sigma=s_norm_sigma, 
                                   use_hist=use_softhist,
                                   interp_algo="default")
    #init_net(net_inferno)
    model_inferno = ModelWrapper(net_inferno)

    model_inferno.fit(epochs, data=data, opt=partialler(optim.Adam,lr=1e-3), loss=None,
                      cbs=[hep_inf,  lt])
        
    return model_inferno, {"loss":lt, "covs": hep_inf.covs}


def train_bce(data, epochs=100, inferno_args=None):
    
    net_bce = nn.Sequential(nn.Linear(4,12),  nn.ReLU(),
                        nn.Linear(12,8), nn.ReLU(),
                        nn.Linear(8,1),  nn.Sigmoid())
    #init_net(net)    
    ct = hep_model.HEPInferno(b_true=inferno_args["b_true"], 
                              mu_true=inferno_args["mu_true"],
                              n_shape_systs=inferno_args["n_shape_systs"],
                              n_weight_systs=inferno_args["n_weight_systs"],
                              shape_norm_sigma=inferno_args["shape_norm_sigma"], 
                              interp_algo="default", ignore_loss=True,
                              use_hist=True)
    lt = LossTracker()
    model_bce = ModelWrapper(net_bce)
    model_bce.fit(epochs, data=data, opt=partialler(optim.Adam), loss=nn.BCELoss(),
                  cbs=[lt, ct])
    
    #bce_trn_covs = ct.covs["trn"]
    #bce_val_covs = ct.covs["val"]
    return model_bce, {"loss":lt, "covs": ct.covs}


    