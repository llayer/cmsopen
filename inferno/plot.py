import matplotlib.pyplot as plt
import numpy as np


def plot_cov(bce_info, inferno_info, names, args):
    
    inferno_trn_covs, inferno_val_covs = inferno_info["covs"]["trn"], inferno_info["covs"]["val"]
    bce_trn_covs, bce_val_covs = bce_info["covs"]["trn"], bce_info["covs"]["val"]
    # Compare train / validation INFERNO
    plot_cov_trnval(inferno_trn_covs, inferno_val_covs, names, stddev=False, outpath=args["outpath"], store=args["store"])
    # Compare validation BCE - INFERNO
    plot_cov_infbce(bce_val_covs, inferno_val_covs, names, stddev=False, outpath=args["outpath"], store=args["store"])


def plot_inferno(df_inf, info, args, order_d):
    
    # Plot loss
    plot_loss(info["loss"], outpath=args["outpath"], store=args["store"])
    
    # Plot test predictions
    plot_predictions(df_inf, bins = args["bins"], use_hist = args["use_softhist"], plot_sorted=args["fit_sorted"], 
                     name="inferno", outpath=args["outpath"], store=args["store"])
        
    # Plot systematic variations
    shapes = info["shapes"]
    for i, syst_name in enumerate(args["systnames"]):
        plot_shapes(shapes["bkg"], shapes["sig"], shapes["sig_up"][i], shapes["sig_down"][i], "inferno", syst_name, 
                    plot_sorted=args["fit_sorted"], order_d = order_d, use_hist = args["use_softhist"], 
                    outpath=args["outpath"], store=args["store"])
    
    
def plot_bce(df_bce, info, args):
    
    # Plot loss
    plot_loss(info["loss"], name="bce", outpath=args["outpath"], store=args["store"])
    
    # Plot predictions
    plot_predictions(df_bce, bins = args["bins"], use_hist = True, name="bce", outpath=args["outpath"], store=args["store"])
    
    # Plot systematic variations
    shapes = info["shapes"]
    for i, syst_name in enumerate(args["systnames"]):
        plot_shapes(shapes["bkg"], shapes["sig"], shapes["sig_up"][i], shapes["sig_down"][i], "bce", syst_name, 
                    use_hist = True, outpath=args["outpath"], store=args["store"])

        
def plot_loss(lt, outpath=".", name="inferno", store=False):
    
    plt.plot(lt.losses["trn"], label="train")
    plt.plot(lt.losses["val"], label="val")
    if name == "inferno":
        plt.ylabel(r"$\sigma^2(\mu)$")
    else:
        plt.ylabel(r"bce")
    plt.xlabel(r"epoch")
    plt.legend(loc="upper right")
    if store:
        plt.savefig(outpath + "/train/" + name + "/loss.png")
    plt.show()

    
def plot_shapes(bkg, sig, sig_up, sig_down, name, syst_name, use_hist = False, plot_sorted = False, order_d = None,
                outpath=".", store=False):
                
    if plot_sorted == True:
        bkg = bkg[list(order_d.keys())]
        sig = sig[list(order_d.keys())]
        sig_up = sig_up[list(order_d.keys())]
        sig_down  = sig_down[list(order_d.keys())]
        
    bins = len(bkg)
    if use_hist == True:
        xmax = 1.
    else:
        xmax = bins
    edges = np.linspace(0, xmax, bins + 1)
    centers = edges[:-1] + (xmax/float(bins))/float(2)
    
    fig, (ax1, ax2) = plt.subplots(nrows=2, gridspec_kw={'height_ratios': [3,1]}, figsize=(8,6))
    ax1.stairs(bkg, edges, label="bkg", color="blue")
    ax1.stairs(sig, edges, label="sig", color="orange")
    ax1.stairs(sig_up, edges, label="sig up", color="green")
    ax1.stairs(sig_down, edges, label="sig down", color="red")
    ax1.legend(loc="upper right")
    
    plt.text(0.05, 0.9,syst_name,
     horizontalalignment='center',
     verticalalignment='center',
     transform = ax1.transAxes, size=15,
     bbox=dict(facecolor='red', edgecolor=None, alpha=0.2))

    ax2.scatter(centers, np.array(sig_up) / np.array(sig), color="green")
    ax2.scatter(centers, np.array(sig_down) / np.array(sig), color="red")
    ax2.hlines(1., 0, xmax, linestyle="dotted", color="black")
    ax2.set_ylim((0,2))
    ax2.set_xlabel(name)
    
    if store:
        plt.savefig(outpath + "/train/" + name + "/shapes_" + syst_name + ".png")    
    
    plt.show()
            

def plot_predictions(df, bins=10, plot_sorted = False, use_hist = False, outpath=".", name="inferno", store=False):
    
    if plot_sorted:
        sig = df[df["gen_target"]==1]["pred_sorted"]
        bkg = df[df["gen_target"]==0]["pred_sorted"]
    else:
        sig = df[df["gen_target"]==1]["pred"]
        bkg = df[df["gen_target"]==0]["pred"]
    
    if use_hist == True:
        hist_range=(0.,1.)
    else:
        hist_range=(0.,bins)
    plt.hist(sig, density=True, alpha=0.5, bins=bins, range=hist_range, label="Signal")
    plt.hist(bkg, density=True, alpha=0.5, bins=bins, range=hist_range, label="Background")
    plt.legend(loc="upper left")
    if store:
        plt.savefig(outpath + "/train/" + name + "/predictions.png")    
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


def plot_cov_trnval(trn_covs, val_covs, names, stddev=False, outpath=".", store=False):
    
    trn_corrs = get_corr(trn_covs)
    val_corrs = get_corr(val_covs)
    n_par = len(names)
    
    fig, ax = plt.subplots(nrows=n_par, ncols=n_par, figsize=(10,10))

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
    if store:
        plt.savefig(outpath + "/train/inferno/cov_trnval.png")    
    plt.show()
    
            
def plot_cov_infbce(bce_covs, inf_covs, names, stddev=False, outpath=".", store=False):

    bce_corrs = get_corr(bce_covs)
    inf_corrs = get_corr(inf_covs)    
    n_par = len(names)
    
    fig, ax = plt.subplots(nrows=n_par, ncols=n_par, figsize=(10,10))

    for i, row in enumerate(ax):
        for j, col in enumerate(row):
            if i == j:
                bce = get_cov_entry(bce_covs, i, j)
                inf = get_cov_entry(inf_covs, i, j)
                if stddev == True:
                    bce = np.sqrt(bce)
                    inf = np.sqrt(inf)
                plt.text(0.8, 0.8, names[i],
                 horizontalalignment='center',
                 verticalalignment='center',
                 transform = col.transAxes, size=15,
                 bbox=dict(facecolor='red', edgecolor=None, alpha=0.2))
                if i==0:
                    lims = (500,1500)
                    if stddev == True:
                        lims = np.sqrt(lims)
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
            
    if store:
        plt.savefig(outpath + "/train/cov_infbce.png")
    plt.show()            
            
    
def plot_scan(bce, inferno, path="", asimov = True, store=False):
                
    plt.figure()#dpi=150)
    plt.plot(bce["parameter_values"], bce["delta_nlls"], label="BCE")
    plt.plot(inferno["parameter_values"], inferno["delta_nlls"], label="INFERNO")
    plt.ylim(0,5)
    plt.legend(loc="upper left")
    plt.ylabel(r'$2\Delta$ NLL')
    plt.xlabel(r'$\mu$')
    plt.title("Comparison INFERNO - BCE")
    if store==True:
        postfix = "_asimov" if asimov==True else ""
        plt.savefig(path + "/mu_scan" + postfix + ".png")
    plt.show()
    
    