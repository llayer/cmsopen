import matplotlib.pyplot as plt
import numpy as np
import os
import shutil 

#
# Define plots for INFERNO and BCE training
#
def plot_cov(bce_info, inferno_info, names, args):
    
    inferno_trn_covs, inferno_val_covs = inferno_info["covs"]["trn"], inferno_info["covs"]["val"]
    bce_trn_covs, bce_val_covs = bce_info["covs"]["trn"], bce_info["covs"]["val"]
    # Compare train / validation INFERNO
    plot_cov_trnval(inferno_trn_covs, inferno_val_covs, names, stddev=False, outpath=args["outpath"], store=args["store"])
    # Compare validation BCE - INFERNO
    plot_cov_infbce(bce_val_covs, inferno_val_covs, names, stddev=False, outpath=args["outpath"], store=args["store"])


def plot_inferno(df_inf, info, args, order_d):
    
    plt.rcParams.update(plt.rcParamsDefault)
    
    # Plot loss
    plot_loss(info["loss"], outpath=args["outpath"], store=args["store"])
    
    # Plot test predictions
    plot_predictions(df_inf, bins = args["inferno_bins"], use_hist = args["use_softhist"], plot_sorted=args["fit_sorted"], 
                     name="inferno", outpath=args["outpath"], store=args["store"])
    
    # Plot ovetrain
    shapes = info["shapes"]
    plot_overtrain(shapes, "inferno", plot_sorted=args["fit_sorted"], epoch_idx = info["idx_best"],
                    order_d = order_d, use_hist = args["use_softhist"], outpath=args["outpath"], store=args["store"])
        
    # Plot systematic variations
    for i, syst_name in enumerate(args["systnames"]):
        plot_shapes(shapes, "inferno", syst_name, syst_idx=i, plot_sorted=args["fit_sorted"], epoch_idx = info["idx_best"],
                    order_d = order_d, use_hist = args["use_softhist"], outpath=args["outpath"], store=args["store"])
        if args["create_gifs"] == True:
            make_gif(shapes, "inferno", plot_sorted=args["fit_sorted"], 
                    order_d = order_d, use_hist = args["use_softhist"], outpath=args["outpath"])
            
    # Store validation loss and cov
    if args["store"] == True:
        store_loss_cov(info, outpath=args["outpath"], name="inferno")
    
    
def plot_bce(df_bce, info, args):
    
    plt.rcParams.update(plt.rcParamsDefault)
    
    # Plot loss
    plot_loss(info["loss"], name="bce", outpath=args["outpath"], store=args["store"])
    
    # Plot predictions
    shapes = info["shapes"]
    plot_predictions(df_bce, bins = args["bce_bins"], use_hist = True, name="bce", outpath=args["outpath"], store=args["store"])
    
    # Plot overtrain
    plot_overtrain(shapes, "bce", use_hist = True, outpath=args["outpath"], epoch_idx = info["idx_best"], store=args["store"])
    
    # Plot systematic variations
    for i, syst_name in enumerate(args["systnames"]):
        plot_shapes(shapes, "bce", syst_name, syst_idx=i, use_hist = True, epoch_idx = info["idx_best"], 
                    outpath=args["outpath"], store=args["store"])

    # Store validation loss and cov
    if args["store"] == True:
        store_loss_cov(info, outpath=args["outpath"], name="bce")
        
#
# Store validation loss and covariance matrix
#
def store_loss_cov(info, outpath=".", name="inferno"):
    
    val_loss = info["loss"].losses["val"]
    val_cov = info["covs"]["val"]
    np.save(outpath + "/train/" + name +"/val_loss.npy", val_loss)
    np.save(outpath + "/train/" + name +"/val_cov.npy", val_cov)    
    
#
# Plotting functions for loss and shapes
#
def plot_loss(lt, outpath=".", name="inferno", store=False):
    
    plt.figure()#dpi=150)
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
    plt.figure()#dpi=150)
    plt.hist(sig, density=True, alpha=0.5, bins=bins, range=hist_range, label="Signal")
    plt.hist(bkg, density=True, alpha=0.5, bins=bins, range=hist_range, label="Background")
    plt.legend(loc="upper left")
    if store:
        plt.savefig(outpath + "/train/" + name + "/predictions.png")    
    plt.show()
    
    
def plot_shapes(shapes, name, syst_name, syst_idx=0, epoch_idx = -1, use_hist = False, plot_sorted = False, order_d = None,
                outpath=".", store=False):
        
    bkg = shapes["bkg"][epoch_idx]
    sig = shapes["sig"][epoch_idx]
    sig_up = shapes["up"][epoch_idx][syst_idx]
    sig_down = shapes["down"][epoch_idx][syst_idx]
    
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
    ax1.stairs(sig_up, edges, label="up", color="green")
    ax1.stairs(sig_down, edges, label="down", color="red")
    ax1.legend(loc="upper right")
    
    plt.text(0.05, 0.9,syst_name,
     horizontalalignment='center',
     verticalalignment='center',
     transform = ax1.transAxes, size=15,
     bbox=dict(facecolor='red', edgecolor=None, alpha=0.2))

    if "bkg" in syst_name:
        nom = bkg
    else:
        nom = sig
    
    ax2.scatter(centers, np.array(sig_up) / np.array(nom), color="green")
    ax2.scatter(centers, np.array(sig_down) / np.array(nom), color="red")
    ax2.hlines(1., 0, xmax, linestyle="dotted", color="black")
    ax2.set_ylim((0,2))
    ax2.set_xlabel(name)
    
    if store:
        plt.savefig(outpath + "/train/" + name + "/shapes_" + syst_name + ".png")    
      
    plt.show()
    
    
def plot_overtrain(shapes, name, epoch_idx=-1, use_hist = False, plot_sorted = False, order_d = None,
                outpath=".", store=False):
        
    trn_bkg = shapes["bkg_trn"][epoch_idx]
    trn_sig = shapes["sig_trn"][epoch_idx]
    val_bkg = shapes["bkg"][epoch_idx]
    val_sig = shapes["sig"][epoch_idx]
    
    if plot_sorted == True:
        trn_bkg = trn_bkg[list(order_d.keys())]
        trn_sig = trn_sig[list(order_d.keys())]
        val_bkg = val_bkg[list(order_d.keys())]
        val_sig = val_sig[list(order_d.keys())]
        
    bins = len(trn_bkg)
    if use_hist == True:
        xmax = 1.
    else:
        xmax = bins
    edges = np.linspace(0, xmax, bins + 1)
    centers = edges[:-1] + (xmax/float(bins))/float(2)
    
    fig, (ax1, ax2) = plt.subplots(nrows=2, gridspec_kw={'height_ratios': [3,1]}, figsize=(8,6))
    ax1.stairs(trn_bkg, edges, label="bkg train")
    ax1.stairs(trn_sig, edges, label="sig train")
    ax1.scatter(centers, val_bkg)
    ax1.scatter(centers, val_sig)
    #ax1.set_ylim((0,1))
    ax1.legend(loc="upper right")    

    ax2.scatter(centers, val_bkg/trn_bkg)
    ax2.scatter(centers, val_sig/trn_sig)
    ax2.hlines(1., 0, xmax, linestyle="dotted", color="black")
    ax2.set_ylim((0,2))
    ax2.set_xlabel(name)
    
    if store:
        plt.savefig(outpath + "/train/" + name + "/overtrain.png")    
      
    plt.show()

        
#
# Create a GIf for the NN predictions during training
#
def plot_sig_bkg(shapes, name, epoch_idx = -1, use_hist = False, plot_sorted = False, order_d = None, outpath="."):

    bkg = shapes["bkg"][epoch_idx]
    sig = shapes["sig"][epoch_idx]
    
    if plot_sorted == True:
        bkg = bkg[list(order_d.keys())]
        sig = sig[list(order_d.keys())]
        
    bins = len(bkg)
    if use_hist == True:
        xmax = 1.
    else:
        xmax = bins
    edges = np.linspace(0, xmax, bins + 1)
    centers = edges[:-1] + (xmax/float(bins))/float(2)
    
    fig, ax1 = plt.subplots()
    ax1.stairs(bkg, edges, label="bkg", color="blue")
    ax1.stairs(sig, edges, label="sig", color="orange")
    ax1.set_ylim((0,1))
    ax1.set_xlabel(name)
    ax1.legend(loc="upper right")    
    
    plt.text(0.15, 0.9,"epoch " + str(epoch_idx),
     horizontalalignment='center',
     verticalalignment='center',
     transform = ax1.transAxes, size=15,
     bbox=dict(facecolor='red', edgecolor=None, alpha=0.2))
    
    plt.savefig(outpath + "/train/gif/shapes_" + name + str(epoch_idx) + ".png")     
    plt.close()
    
    
def make_gif(shapes, name, use_hist = False, plot_sorted = False, order_d = None,
                outpath=".",):

    import imageio
    
    if not os.path.exists(outpath + "/train/gif"):
        os.makedirs(outpath + "/train/gif")
        
    filenames = []
    for i in range(len(shapes["bkg"])):
        # plot charts
        pngname = outpath + "/train/gif/shapes_" + name + str(i) + ".png"
        
        plot_sig_bkg(shapes, name, epoch_idx = i, plot_sorted=plot_sorted,
                     use_hist=use_hist, order_d=order_d, outpath=outpath)
        filenames.append(pngname)    
        
    # build gif
    """
    with imageio.get_writer('mygif_' + syst_name +'.gif', mode='I') as writer:
        for filename in filenames:
            image = imageio.imread(filename)
            writer.append_data(image) 
    """
    images = list(map(lambda filename: imageio.imread(filename), filenames))
    imageio.mimsave(os.path.join('mygif.gif'), images, duration = 0.3) # modify the frame duration as needed

    shutil.rmtree(outpath + "/train/gif")
    

#
# Compare the covariance / correlation matrix during training
#
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
    if n_par == 1: ax = [[ax]]
        
    for i, row in enumerate(ax):
        for j, col in enumerate(row):
            if i == j:
                trn = get_cov_entry(trn_covs, i, j)
                val = get_cov_entry(val_covs, i, j)
                if stddev == True:
                    trn = np.sqrt(trn)
                    val = np.sqrt(val)
                plt.text(0.8, 0.85,names[i],
                 horizontalalignment='center',
                 verticalalignment='center',
                 transform = col.transAxes, size=15,
                 bbox=dict(facecolor='red', edgecolor=None, alpha=0.2))
                if i==0:
                    lims_low = np.min(trn) - 0.1 * np.min(trn)
                    lims_up = np.min(val) + 0.5 * np.min(val) 
                    lims = (lims_low, lims_up)
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
    fig.tight_layout()
    if store:
        plt.savefig(outpath + "/train/inferno/cov_trnval.png")    
    plt.close()
    
            
def plot_cov_infbce(bce_covs, inf_covs, names, stddev=False, outpath=".", store=False):

    bce_corrs = get_corr(bce_covs)
    inf_corrs = get_corr(inf_covs)    
    n_par = len(names)
    
    fig, ax = plt.subplots(nrows=n_par, ncols=n_par, figsize=(10,10))
    if n_par == 1: ax = [[ax]]
    
    for i, row in enumerate(ax):
        for j, col in enumerate(row):
            if i == j:
                bce = get_cov_entry(bce_covs, i, j)
                inf = get_cov_entry(inf_covs, i, j)
                if stddev == True:
                    bce = np.sqrt(bce)
                    inf = np.sqrt(inf)
                
                col.text(0.8, 0.85, names[i],
                 horizontalalignment='center',
                 verticalalignment='center',
                 transform = col.transAxes, #size=15,
                 bbox=dict(facecolor='red', edgecolor=None, alpha=0.2))
                
                if i==0:
                    #lims = (500,1500)
                    lims_low = np.min(inf) - 0.1 * np.min(inf)
                    lims_up = np.min(bce) + 0.5 * np.min(bce) 
                    lims = (lims_low, lims_up)
                    if stddev == True:
                        lims = np.sqrt(lims)
                else:
                    lims = (0., 1.5)
            else:
                bce = get_cov_entry(bce_corrs, i, j)
                inf = get_cov_entry(inf_corrs, i, j)
                lims = (-1., 1)
            col.plot(inf, label="inferno")
            col.plot(bce, label="bce")
            col.set_ylim(lims)
            if (i==0) & (j==n_par-1):
                col.legend(loc="upper right")
                #col.legend(loc="upper right", prop={'size': 16})
    fig.tight_layout()            
    if store:
        plt.savefig(outpath + "/train/cov_infbce.png")
    plt.show()            
            

#
# Compare the likelihood scan for INFERNO and BCE
#     
def plot_scan(bce, inferno, bce_stat = None, inferno_stat  = None, path="", asimov = True, store=False):
          
    plt.rcParams.update(plt.rcParamsDefault)
        
    plt.figure()#dpi=150)
    plt.plot(bce["parameter_values"], bce["delta_nlls"], label="BCE")
    plt.plot(inferno["parameter_values"], inferno["delta_nlls"], label="INFERNO")
    if bce_stat is not None:
        #plt.plot(bce_stat["parameter_values"], bce_stat["delta_nlls"], label="BCE Stat")
        plt.plot(inferno_stat["parameter_values"], inferno_stat["delta_nlls"], label="Stat only")    
    plt.ylim(0,5)
    plt.legend(loc="upper left")
    plt.ylabel(r'$2\Delta$ NLL')
    plt.xlabel(r'$\mu$')
    plt.title("Comparison INFERNO - BCE")
    plt.axhline(y=1, linestyle="dashed", color="grey")
    plt.axhline(y=4, linestyle="dashed", color="grey")
    if store==True:
        postfix = "_asimov" if asimov==True else ""
        plt.savefig(path + "/mu_scan" + postfix + ".png")
    plt.show()
    
    