import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
from typing import *
import torch
from torch import Tensor
from torch.distributions import Distribution
from torch.distributions import Normal
from pytorch_inferno.inference import *
from pytorch_inferno.inferno import *
from pytorch_inferno.callback import *
from fastcore.all import partialler

#
# Morphing
#
def smooth_function(x, smoothRegion = 1):
    
    mask_smooth_region = torch.abs(x) > smoothRegion
    xnorm = x / smoothRegion
    xnorm2 = xnorm*xnorm
    
    in_smooth = 0.125 * xnorm * (xnorm2 * (3.*xnorm2 - 10.) + 15)
    out_smooth = torch.where(x>0, torch.ones(x.shape).to(x.device), -1*torch.ones(x.shape).to(x.device))
    
    return torch.where(mask_smooth_region, out_smooth, in_smooth)

def morph(x, nominal, lo, hi):
        
    dhi = hi - nominal
    dlo = lo - nominal
    #print(dhi)
    #print(dlo)
    diff_h = dhi - dlo
    sum_h = dhi + dlo
    alpha = x * 0.5 * ((diff_h) + (sum_h)*smooth_function(x))
    return alpha

def fast_vertical(alpha:Tensor, f_nom:Tensor, f_up:Tensor, f_dw:Tensor):
    
    f_nom_rep = f_nom.unsqueeze(0).repeat(alpha.shape[0], 1)
    return f_nom + morph(alpha.unsqueeze(1), f_nom_rep, f_dw, f_up).sum(0)


#
# Differentiable histogram
#
#https://discuss.pytorch.org/t/differentiable-torch-histc/25865
# CAREFUL - weights implementation should be checked
class SoftHistogram(torch.nn.Module):
    def __init__(self, bins, min, max, sigma, device=None):
        super(SoftHistogram, self).__init__()
        self.bins = bins
        self.min = min
        self.max = max
        self.sigma = sigma
        self.delta = float(max - min) / float(bins)
        self.centers = float(min) + self.delta * (torch.arange(bins).float() + 0.5)
        if device is not None:
            self.centers = self.centers.to(device)

    def forward(self, x, weights=None):
        x = torch.unsqueeze(torch.squeeze(x), 0) - torch.unsqueeze(self.centers, 1)
        x = torch.sigmoid(self.sigma * (x + self.delta/2)) - torch.sigmoid(self.sigma * (x - self.delta/2))
        if weights is not None:
            x = (x*weights).sum(dim=1)
        else:
            x = x.sum(dim=1)
        return x

#
# Constraints
#

def normal(theta, std):
    return 1. + theta*std

def log_normal(theta, kappa):
    return torch.exp(theta * torch.log(kappa))

def logKappaForX(x, logkappa_lo, logkappa_hi):
    
    logKhi =  logkappa_hi
    logKlo = -logkappa_lo

    kappa = torch.where(x >= 0, logkappa_hi, -logkappa_lo)
    
    avg = 0.5*(logKhi + logKlo) 
    halfdiff = 0.5*(logKhi - logKlo)
    twox = x+x 
    twox2 = twox*twox
    alpha = 0.125 * twox * (twox2 * (3*twox2 - 10.) + 15.)
    ret = avg + alpha*halfdiff
    return torch.where(torch.abs(x) >= 0.5, kappa, ret)


#
# Log-likelihood for HEP like systematics
#

def hep_nll(s_true:float, b_true:float, mu:Tensor, f_s_nom:Tensor, f_b_nom:Tensor,
             shape_alpha:Optional[Tensor]=None, s_norm_alpha:Optional[Tensor]=None, 
             b_norm_alpha:Optional[Tensor]=None, b_rate_param_alpha:Optional[Tensor]=None,
             f_s_up:Optional[Tensor]=None, f_s_dw:Optional[Tensor]=None,
             f_b_up:Optional[Tensor]=None, f_b_dw:Optional[Tensor]=None,
             shape_norm_sigma:Optional[Tensor]=None, s_norm_sigma:Optional[Tensor]=None, 
             b_norm_sigma:Optional[Tensor]=None, 
             interp_algo:str="fast_vertical") -> Tensor:
    r'''Compute negative log-likelihood for specified parameters.'''
    
    #  Interpolate shapes
    if interp_algo == "fast_vertical":
        f_s = fast_vertical(shape_alpha, f_s_nom, f_s_up, f_s_dw) if shape_alpha is not None and f_s_up is not None else f_s_nom
        f_b = fast_vertical(shape_alpha, f_b_nom, f_b_up, f_b_dw) if shape_alpha is not None and f_b_up is not None else f_b_nom        
    else:
        #print(shape_alpha)
        #print(f_s_nom, f_s_up, f_s_dw)
        f_s = interp_shape(shape_alpha, f_s_nom, f_s_up, f_s_dw) if shape_alpha is not None and f_s_up is not None else f_s_nom
        f_b = interp_shape(shape_alpha, f_b_nom, f_b_up, f_b_dw) if shape_alpha is not None and f_b_up is not None else f_b_nom
        
    # Normalizations !!! careful if signal and background shapes mix!!
    #print("shape_norm_sigma", shape_norm_sigma)
    s_exp, b_exp = mu, b_true
    if len(shape_alpha) > 0:
        #print("Norms shape", normal(shape_alpha, shape_norm_sigma))
        s_exp *= normal(shape_alpha, shape_norm_sigma).prod()  
    if len(s_norm_alpha) > 0:
        s_exp *= normal(s_norm_alpha, s_norm_sigma).prod()
    if len(b_norm_alpha) > 0:
        b_exp *= normal(b_norm_alpha, b_norm_sigma).prod()
    if (b_rate_param_alpha  is not None) and (len(b_rate_param_alpha) > 0):
        b_exp *= b_rate_param_alpha
        
    #  Compute NLL
    t_exp = (s_exp*f_s)+(b_exp*f_b)
    asimov = (s_true*f_s_nom)+(b_true*f_b_nom)
        
    nll = -torch.distributions.Poisson(t_exp, False).log_prob(asimov).sum()

    # Constrain shape +norm nuisances
    for a in shape_alpha: nll = nll - Normal(0,1).log_prob(a)
    for a in b_norm_alpha: nll = nll - Normal(0,1).log_prob(a)
    for a in s_norm_alpha: nll = nll - Normal(0,1).log_prob(a)
    return nll


#
# INFERNO for HEP like systematics
#

class HEPInferno(AbsCallback):
    r'''Implementation of INFERNO with HEP like systematics'''
    def __init__(self, b_true:float, mu_true:float, n_shape_systs:int=0, n_weight_systs:int=0,
                 interp_algo:str="default", 
                 shape_norm_sigma:Optional[List[float]]=None,
                 s_norm_sigma:Optional[List[float]]=None, b_norm_sigma:Optional[List[float]]=None, 
                 b_rate_param:bool=False, use_hist:bool=False, bins:int=10, sigmoid_delta:float=200.,
                 ignore_loss:bool=False, 
                 **kwargs):
        
        self.ignore_loss = ignore_loss
        self.use_hist = use_hist
        self.bins = bins
        self.sigmoid_delta = sigmoid_delta
        self.mu_true = mu_true
        self.b_true = b_true
        self.n_shape_systs = n_shape_systs
        self.n_weight_systs = n_weight_systs
        self.n_shape_alphas = n_shape_systs + n_weight_systs
        self.interp_algo = interp_algo
        self.shape_norm_sigma = shape_norm_sigma #torch.Tensor(shape_norm_sigma)
        self.s_norm_sigma = s_norm_sigma
        self.b_norm_sigma = b_norm_sigma
        self.b_rate_param = b_rate_param
        
        # Compute nuisance indeces
        self.poi_idx = [0]
        self.n_alpha = 1
        # Shape + norm
        if self.shape_norm_sigma is not None and len(self.shape_norm_sigma) != self.n_shape_alphas: raise \
            ValueError("Number of norm uncertainties on shape nuisances must match the number of shape nuisance parameters")
        if self.n_shape_alphas > 0:
            self.shape_idxs = list(range(1,self.n_shape_alphas+1))
            self.n_alpha += self.n_shape_alphas
        else:
            self.shape_idxs = []
        # Signal norms
        if self.s_norm_sigma is not None and len(self.s_norm_sigma) > 0:
            self.s_norm_idxs = list(range(self.n_alpha, self.n_alpha+len(self.s_norm_sigma)))
            self.n_alpha += len(self.s_norm_sigma)
        else:
            self.s_norm_idxs = []
        # Background norms + possible rate param
        if self.b_norm_sigma is not None and len(self.b_norm_sigma) > 0:
            self.b_norm_idxs = list(range(self.n_alpha, self.n_alpha+len(self.b_norm_sigma)))
            self.n_alpha += len(self.b_norm_sigma)
        else:
            self.b_norm_idxs = []
        if self.b_rate_param:
            self.b_rate_param_idx = list(self.n_alpha, self.n_alpha+1)
            self.n_alpha += 1
        else:
            self.b_rate_param_idx = []
        
        # Store covariance matrix
        self.covs, self.cov, self.cnt = {'trn':[], 'val':[]}, 0, 0
        self.val_shapes = {'sig':[], 'bkg':[], "sig_up":[], "sig_down":[]}
        self.sig_shape, self.bkg_shape = 0, 0
        self.sig_shape_up = [0. for i in range(self.n_shape_alphas)]
        self.sig_shape_down = [0. for i in range(self.n_shape_alphas)]

        print("*********************")
        print("Summary INFERNO setup")
        print("b_true", self.b_true)
        print("mu_true", self.mu_true)
        print("n_shape_systs", self.n_shape_systs)
        print("n_weight_systs", self.n_weight_systs)
        print("nshape_alphas", self.n_shape_alphas)
        print("shape idx", self.shape_idxs)
        print("shape_norm_sigma", self.shape_norm_sigma)
        print("s_norm_sigma", self.s_norm_sigma)
        print("s_norm_idxs", self.s_norm_idxs)
        print("b_norm_sigma", self.b_norm_sigma)
        print("b_norm_idxs", self.b_norm_idxs)
        print("b_rate_param", self.b_rate_param)
        print("b_rate_param_idx", self.b_rate_param_idx)
        print("n_alpha", self.n_alpha)
        print("interp_algo", self.interp_algo)
        print("use_hist", self.use_hist)
        print("ignore_loss", self.ignore_loss)
        print("*********************")

    def _aug_data(self): pass  # Override abs method
    def on_batch_begin(self) -> None: pass
    def on_batch_end(self) -> None: pass
    
    def on_epoch_begin(self) -> None: 
        self.cov, self.cnt = 0, 0
        self.sig_shape, self.bkg_shape = 0, 0
        self.sig_shape_up = [0 for i in range(self.n_shape_alphas)]
        self.sig_shape_down = [0 for i in range(self.n_shape_alphas)]
        
    def on_epoch_end(self) -> None:
        if self.wrapper.state == 'train':
            self.covs['trn'].append(  self.cov / self.cnt  )
        else:
            self.covs['val'].append(  self.cov / self.cnt  )
            self.val_shapes['bkg'].append( self.bkg_shape / self.cnt )
            self.val_shapes['sig'].append( self.sig_shape / self.cnt )            
            self.val_shapes['sig_up'].append( [shape / self.cnt for shape in self.sig_shape_up] )
            self.val_shapes['sig_down'].append( [shape / self.cnt for shape in self.sig_shape_down] )

    def on_train_begin(self) -> None:
        
        if self.ignore_loss == False:
            self.wrapper.loss_func = None  # Ensure loss function is skipped, callback computes loss value in `on_forwards_end`
            
        for c in self.wrapper.cbs:
            if hasattr(c, 'loss_is_meaned'): c.loss_is_meaned = False  # Ensure that average losses are correct        
        if self.shape_norm_sigma is not None: self.shape_norm_sigma = torch.Tensor(self.shape_norm_sigma).to(self.wrapper.device)
        if self.s_norm_sigma is not None: self.s_norm_sigma = torch.Tensor(self.s_norm_sigma).to(self.wrapper.device)
        if self.b_norm_sigma is not None: self.b_norm_sigma = torch.Tensor(self.b_norm_sigma).to(self.wrapper.device)

    def store_shapes(self, f_s_nom:Tensor, f_b_nom:Tensor, f_s_up:Optional[Tensor], f_s_dw:Optional[Tensor], 
                    f_b_up:Optional[Tensor], f_b_dw:Optional[Tensor]) -> None:
        
        with torch.no_grad(): 
            self.sig_shape += f_s_nom.detach().cpu().numpy()
            self.bkg_shape += f_b_nom.detach().cpu().numpy()   
            for i in range(self.n_shape_alphas):
                self.sig_shape_up[i] += f_s_up[i].detach().cpu().numpy()
                self.sig_shape_down[i] += f_s_dw[i].detach().cpu().numpy()
            
    def to_shape(self, p:Tensor, w:Optional[Tensor]=None) -> Tensor:
        
        eps=1e-7
        if self.use_hist == False:
            f = (p*w).sum(0)+1e-7 if w is not None else p.sum(0)+eps
            return f/f.sum()   
        else:
                        
            #h = torch.histc(p, bins=10, min=0., max=1.)
            #h = torch.histogram(p.cpu(), bins=10, range=(0., 1.)).hist
            #print(h)
            #h = h.div(h.sum())+eps
            #return h.to(self.wrapper.device)
            
            if self.ignore_loss:
                if w is not None:
                    h = torch.histogram(p.cpu(), bins=self.bins, range=(0., 1.), weight=w.cpu()).hist.to(self.wrapper.device)
                else:
                    h = torch.histc(p, bins=self.bins, min=0., max=1.)
            else:
                hist = SoftHistogram(bins=self.bins, min=0., max=1., sigma=self.sigmoid_delta, device=self.wrapper.device)
                h = hist(p, w)
            h = h.div(h.sum())+eps
            return h
            
            
    def _get_up_down(self, x_s:Tensor, x_b:Tensor, w_s:Optional[Tensor]=None, w_b:Optional[Tensor]=None) -> Tuple[Tuple[Optional[Tensor],Optional[Tensor]],Tuple[Optional[Tensor],Optional[Tensor]]]:

        if self.n_shape_systs != ((x_s.shape[-1]-1)/2): 
            raise ValueError("Number of specified shape nuisances n_shape_systs", self.n_shape_systs,
                             "must match the number of systematic variations:", (x_s.shape[-1]-1)/2 )
        
        if w_s is not None:
            if self.n_weight_systs != ((w_s.shape[-1]-1)/2): 
                raise ValueError("Number of specified weight nuisances n_weight_systs", self.n_weight_systs,
                                 "must match the number of systematic variations:", (w_s.shape[-1]-1)/2 )
        else:
            if self.n_weight_systs > 0: 
                raise ValueError("Specified number of weight nuisances", self.n_weight_systs, "but no weights given")
                                 
            
        
        # Nominal weights
        w_s_nom = w_s[:,:,0] if w_s is not None else None
        w_b_nom = w_s[:,:,0] if w_b is not None else None
        
        u,d = [],[]
        # Loop over shape systematics
        for i in range(self.n_shape_systs):
            idx_up = 1 + 2*i
            idx_down = 2 + 2*i
            up_batch = self.to_shape(self.wrapper.model(x_s[:,:,idx_up]), w_s_nom)
            down_batch = self.to_shape(self.wrapper.model(x_s[:,:,idx_down]), w_b_nom)
            
            #print([list(zip(x_s[:,:,0][0], x_s[:,:,idx_up][0], x_s[:,:,idx_down][0]))])
            
            u.append(up_batch)
            d.append(down_batch)    
        
        #Loop over weight systematics
        for i in range(self.n_weight_systs):
            idx_up = 1 + 2*i
            idx_down = 2 + 2*i
            up_batch = self.to_shape(self.wrapper.model(x_s[:,:,0]), w_s_nom * w_s[:,:,idx_up])
            down_batch = self.to_shape(self.wrapper.model(x_s[:,:,0]), w_s_down * w_s[:,:,idx_down])
            u.append(up_batch)
            d.append(down_batch)             
            
        return (torch.stack(u),torch.stack(d)), (None,None)

    def get_ikk(self, f_s_nom:Tensor, f_b_nom:Tensor, f_s_up:Optional[Tensor], f_s_dw:Optional[Tensor], 
                f_b_up:Optional[Tensor], f_b_dw:Optional[Tensor]) -> Tensor:
        r'''Compute full hessian at true param values, or at random starting values with Newton updates'''
        alpha = torch.zeros((self.n_alpha), requires_grad=True, device=self.wrapper.device)
        with torch.no_grad(): alpha[self.poi_idx] += self.mu_true
        #print("alpha", alpha)
        get_nll = partialler(hep_nll, s_true=self.mu_true, b_true=self.b_true, # Expectation values
                             f_s_nom=f_s_nom, f_b_nom=f_b_nom, # Nominal shapes
                             f_s_up=f_s_up, f_s_dw=f_s_dw, # Signal shapes
                             f_b_up=f_b_up, f_b_dw=f_b_dw, #Background shapes
                             shape_norm_sigma = self.shape_norm_sigma, # Norm unct on shapes
                             s_norm_sigma = self.s_norm_sigma, b_norm_sigma = self.b_norm_sigma # Norm unct on sig and bkg
                             ) 
        nll = get_nll(mu=alpha[self.poi_idx], s_norm_alpha=alpha[self.s_norm_idxs], 
                      b_norm_alpha=alpha[self.b_norm_idxs], shape_alpha=alpha[self.shape_idxs],
                      b_rate_param_alpha = alpha[self.b_rate_param_idx],
                      interp_algo = self.interp_algo)
        _,h = calc_grad_hesse(nll, alpha, create_graph=True)
        cov = torch.inverse(h)        
        with torch.no_grad(): 
            self.cov += cov.detach().cpu().numpy()
        self.cnt += 1
        return cov[self.poi_idx,self.poi_idx]

    def on_forwards_end(self) -> None:
        r'''Compute loss and replace wrapper loss value'''
        b = self.wrapper.y.squeeze() == 0
        
        #print(self.wrapper.w)
        
        if self.wrapper.w is not None:
            w_s = self.wrapper.w[~b]
            w_b = self.wrapper.w[b]
            w_s_nom = w_s[:,:,0]
            w_b_nom = w_b[:,:,0]
        else:
            w_s, w_b, w_s_nom, w_b_nom = None, None, None, None
        
        f_s = self.to_shape(self.wrapper.y_pred[~b], w_s_nom)
        f_b = self.to_shape(self.wrapper.y_pred[b], w_b_nom)
        
        if len(self.shape_idxs) > 0:
            (f_s_up,f_s_dw),(f_b_up,f_b_dw)= self._get_up_down(self.wrapper.x[~b], self.wrapper.x[b], w_s, w_b)
        else:
            (f_s_up,f_s_dw),(f_b_up,f_b_dw)=(None,None), (None,None)
        self.store_shapes(f_s_nom=f_s, f_b_nom=f_b, f_s_up=f_s_up, f_s_dw=f_s_dw, f_b_up=f_b_up, f_b_dw=f_b_dw)
        inferno_loss = self.get_ikk(f_s_nom=f_s, f_b_nom=f_b, f_s_up=f_s_up, f_s_dw=f_s_dw, f_b_up=f_b_up, f_b_dw=f_b_dw)
        
        if self.ignore_loss == False:
            self.wrapper.loss_val = inferno_loss

            
            