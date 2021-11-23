import matplotlib.pyplot as plt
import numpy as np


def plot_loss(lt, outpath=".", name="inferno", store=True):
    
    plt.plot(lt.losses["trn"], label="train")
    plt.plot(lt.losses["val"], label="val")
    if name == "inferno":
        plt.ylabel(r"$\sigma^2(\mu)$")
    else:
        plt.ylabel(r"bce")
    plt.xlabel(r"epoch")
    plt.legend(loc="upper right")
    if store:
        plt.savefig(outpath + "/loss_" + name + ".png")
    plt.show()


def plot_predictions(df, plot_sorted = False, outpath=".", name="inferno", store=False):
    
    if plot_sorted:
        sig = df[df["gen_target"]==1]["pred_sorted"]
        bkg = df[df["gen_target"]==0]["pred_sorted"]
    else:
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


def correlation_from_covariance(covariance):
    v = np.sqrt(np.diag(covariance))
    outer_v = np.outer(v, v)
    correlation = covariance / outer_v
    correlation[covariance == 0] = 0
    return correlation

def get_corr(covs):

    corrs = []
    for cov in covs:
        corrs.append(correlation_from_covariance(cov))
    return corrs
        
def get_cov_entry(matrix, i, j):
    values = []
    for mat in matrix:
        values.append(mat[i,j])
    return np.array(values)


def plot_cov_trnval(trn_covs, val_covs, names, stddev=False):
    
    trn_corrs = get_corr(trn_covs)
    val_corrs = get_corr(val_covs)
    
    fig, ax = plt.subplots(nrows=3, ncols=3, figsize=(10,10))

    for i, row in enumerate(ax):
        for j, col in enumerate(row):
            if i == j:
                trn = get_cov_entry(trn_covs, i, j)
                val = get_cov_entry(val_covs, i, j)
                if stddev == True:
                    trn = np.sqrt(trn)
                    val = np.sqrt(val)
                plt.text(0.8, 0.8,names[i],
                 horizontalalignment='center',
                 verticalalignment='center',
                 transform = col.transAxes, size=15,
                 bbox=dict(facecolor='red', edgecolor=None, alpha=0.2))
                if i==0:
                    lims = np.array([500,1500])
                    if stddev == True:
                        lims = np.sqrt(lims)
                else:
                    lims = (0., 1.5)
            else:
                trn = get_cov_entry(trn_corrs, i, j)
                val = get_cov_entry(val_corrs, i, j)
                lims = (-.5, .5)
            col.set_ylim(lims)
            col.plot(trn, label="train")
            col.plot(val, label="val")  
            if (i==0) & (j==2):
                col.legend(loc="upper right", prop={'size': 16})

            
def plot_cov_infbce(bce_covs, inf_covs, names, stddev=False):

    bce_corrs = get_corr(bce_covs)
    inf_corrs = get_corr(inf_covs)    
    
    fig, ax = plt.subplots(nrows=3, ncols=3, figsize=(10,10))

    for i, row in enumerate(ax):
        for j, col in enumerate(row):
            if i == j:
                bce = get_cov_entry(bce_covs, i, j)
                inf = get_cov_entry(inf_covs, i, j)
                if stddev == True:
                    bce = np.sqrt(bce)
                    inf = np.sqrt(val)
                plt.text(0.8, 0.8, names[i],
                 horizontalalignment='center',
                 verticalalignment='center',
                 transform = col.transAxes, size=15,
                 bbox=dict(facecolor='red', edgecolor=None, alpha=0.2))
                if i==0:
                    lims = (500,1500)
                else:
                    lims = (0., 1.5)
            else:
                bce = get_cov_entry(bce_corrs, i, j)
                inf = get_cov_entry(inf_corrs, i, j)
                lims = (-.5, .5)
            col.plot(inf, label="inferno val")
            col.plot(bce, label="bce val")
            col.set_ylim(lims)
            if (i==0) & (j==2):
                col.legend(loc="upper right", prop={'size': 16})
            
            
            
            