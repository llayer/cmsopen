import cabinetry
import numpy as np
import matplotlib.pyplot as plt
import matplotlib as mpl


def plot_shape(bkg, sig, up, down, edges, centers, var):
    
    fig, (ax1, ax2) = plt.subplots(nrows=2, gridspec_kw={'height_ratios': [3,1]}, figsize=(8,6))
    ax1.stairs(bkg, edges, label="bkg", color="blue")
    ax1.stairs(sig, edges, label="sig", color="orange")
    ax1.stairs(up, edges, label="sig up", color="green")
    ax1.stairs(down, edges, label="sig down", color="red")
    ax1.legend(loc="upper right")
    
    ax2.scatter(centers, np.array(up) / np.array(sig), color="green")
    ax2.scatter(centers, np.array(down) / np.array(sig), color="red")
    ax2.hlines(1., 0, edges[-1], linestyle="dotted", color="black")
    ax2.set_ylim((0,2))
    ax2.set_xlabel(var)
          
    plt.show()    
    

def plot_var_shape(samples, shape_syst, weight_syst, var = "MET_met", bins=20, range=(0., 350.)):

    for syst in shape_syst:

        bkg, bkg_weight = samples["QCD"][var], samples["QCD"]["weight"]
        sig, sig_weight = samples["TTJets_signal"][var], samples["TTJets_signal"]["weight"]
        sig_up, sig_up_weight = samples["TTJets_signal_" + syst + "_up"][var], samples["TTJets_signal_" + syst + "_up"]["weight"]
        sig_down, sig_down_weight = samples["TTJets_signal_" + syst + "_down"][var], samples["TTJets_signal_" + syst + "_down"]["weight"]

        bkg_shape, edges = np.histogram(bkg, bins=bins, weights=bkg_weight, range=range, density=True)
        sig_shape = np.histogram(sig, weights=sig_weight, bins=bins, range=range, density=True)[0]
        sig_up_shape = np.histogram(sig_up, weights=sig_up_weight, bins=bins, range=range, density=True)[0]
        sig_down_shape = np.histogram(sig_down, weights=sig_down_weight, bins=bins, range=range, density=True)[0]

        centers = edges[:-1] + (range[1]/float(bins))/float(2)
        plot_shape(bkg_shape, sig_shape, sig_up_shape, sig_down_shape, edges, centers, var)

    for syst in weight_syst:

        bkg, bkg_weight = samples["QCD"][var], samples["QCD"]["weight"]
        sig, sig_weight = samples["TTJets_signal"][var], samples["TTJets_signal"]["weight"]
        sig_up, sig_up_weight = samples["TTJets_signal"][var], samples["TTJets_signal"]["weight_" + syst + "_up"]
        sig_down, sig_down_weight = samples["TTJets_signal"][var], samples["TTJets_signal"]["weight_" + syst + "_down"]

        bkg_shape, edges = np.histogram(bkg, bins=bins, weights=bkg_weight, range=range, density=True)
        sig_shape = np.histogram(sig, weights=sig_weight, bins=bins, range=range, density=True)[0]
        sig_up_shape = np.histogram(sig_up, weights=sig_up_weight, bins=bins, range=range, density=True)[0]
        sig_down_shape = np.histogram(sig_down, weights=sig_down_weight, bins=bins, range=range, density=True)[0]

        centers = edges[:-1] + (range[1]/float(bins))/float(2)
        plot_shape(bkg_shape, sig_shape, sig_up_shape, sig_down_shape, edges, centers, var)



def get_yield(sample, var, bins=30, range=(0,350.), log_scale = True, store=False, path=""):
    
    n, bins = np.histogram(sample[var], bins=bins, range=range, weights=sample['weight'])
    n_err = np.sqrt(np.histogram(sample[var], bins=bins, weights=sample['weight']**2)[0])
    
    return n, n_err, bins

def plot_from_pd(samples, variable = "MET_met", bins=30, range=(0,350.)):
    
    mc_histograms_yields, mc_colors, mc_labels = [], [], []
    total_model_unc = 0
    data_histogram_yields, data_histogram_stdev = 0,0 
    data_label = ""
    bin_edges = 0
    colors = ['k', 'C0', "C3", "C2", "C1", "C4"]

    for i, s in enumerate(args["sample_names"]):

        n, n_err, bin_edges = get_yield(samples[s], variable, bins=bins, range=range)

        if "Data" not in s:
            mc_histograms_yields.append(n)
            mc_labels.append(s)
            mc_colors.append(colors[i])
            total_model_unc += n_err
        else:
            data_histogram_yields = n
            data_histogram_stdev = n_err
            data_label = s    
            
    plot_stack(mc_histograms_yields, mc_colors, mc_labels, total_model_unc, 
                data_histogram_yields, data_histogram_stdev, data_label,
                variable, bin_edges, log_scale = log_scale, store=store, path=path)


def plot_from_model(model_pred, data, config=None, log_scale = True, store=False, path=""):
    
    plt.rcParams.update(plt.rcParamsDefault)
    
    data_yields = cabinetry.model_utils._data_per_channel(model_pred.model, data)[0]
    
    if config is not None:
        # get the region dictionary from the config for binning / variable name
        # for histogram inputs this information is not available, so need to fall
        # back to defaults
        region_dict = cabinetry.configuration.region_dict(config, "Signal_region")
        if region_dict.get("Binning", None) is not None:
            bin_edges = cabinetry.templates.builder._binning(region_dict)
        else:
            bin_edges = np.arange(len(data_yields[i_chan]) + 1)
        variable = region_dict.get("Variable", "bin")
    else:
        # fall back to defaults if no config is specified
        bin_edges = np.arange(len(data_yields[i_chan]) + 1)
        variable = "bin"
        
    histogram_dict_list = []
    #colors = ['C0', "green", "orange", "red", "purple"]
    colors = ['C0', "C3", "C2", "C1", "C4"]
    for i_sam, sample_name in enumerate(model_pred.model.config.samples):
        if sample_name == "TTJets_signal": 
            i_sig = i_sam
        else:
            histogram_dict_list.append(
                {
                    "label": sample_name,
                    "isData": False,
                    "yields": model_pred.model_yields[0][i_sam],
                    "color":colors[i_sam],
                    "variable": variable,
                }
            )
            
    histogram_dict_list.append(
        {
            "label": "TTJets_signal",
            "isData": False,
            "yields": model_pred.model_yields[0][i_sig],
            "color":colors[i_sig],
            "variable": variable,
        }
    )
    histogram_dict_list.append(
        {
            "label": "Data",
            "isData": True,
            "yields": data_yields,
            "variable": variable,
        }
    )            
    
    total_model_unc = np.asarray(model_pred.total_stdev_model_bins[0])
    
    mc_histograms_yields = []
    mc_colors = []
    mc_labels = []
    for h in histogram_dict_list:
        if h["isData"]:
            data_histogram_yields = h["yields"]
            data_histogram_stdev = np.sqrt(data_histogram_yields)
            data_label = h["label"]
        else:
            mc_histograms_yields.append(h["yields"])
            mc_labels.append(h["label"])
            mc_colors.append(h["color"])
            
    plot_stack(mc_histograms_yields, mc_colors, mc_labels, total_model_unc, 
               data_histogram_yields, data_histogram_stdev, data_label,
               variable, bin_edges,
               log_scale = log_scale, store=store, path=path)

            
def plot_stack(mc_histograms_yields, mc_colors, mc_labels, total_model_unc, 
               data_histogram_yields, data_histogram_stdev, data_label, variable, bin_edges,
               log_scale = True, store=False, path=""):
            
    fig = plt.figure(figsize=(6, 5))
    gs = fig.add_gridspec(nrows=2, ncols=1, hspace=0, height_ratios=[3, 1])
    ax1 = fig.add_subplot(gs[0])
    ax2 = fig.add_subplot(gs[1])

    # increase font sizes
    for item in (
        [ax1.yaxis.label, ax2.xaxis.label, ax2.yaxis.label]
        + ax1.get_yticklabels()
        + ax2.get_xticklabels()
        + ax2.get_yticklabels()
    ):
        item.set_fontsize("large")

    # minor ticks on all axes
    for axis in [ax1.xaxis, ax1.yaxis, ax2.xaxis, ax2.yaxis]:
        axis.set_minor_locator(mpl.ticker.AutoMinorLocator())

    # plot MC stacked together
    total_yield = np.zeros_like(mc_histograms_yields[0])
    bin_right_edges = bin_edges[1:]
    bin_left_edges = bin_edges[:-1]
    bin_width = bin_right_edges - bin_left_edges
    bin_centers = 0.5 * (bin_left_edges + bin_right_edges)
    # center data visually in bins if horizontal log scale is used
    bin_centers_data = (
        bin_centers
    )
    mc_containers = []
    for mc_sample_yield, color in zip(mc_histograms_yields, mc_colors):
        mc_container = ax1.bar(
            bin_centers, mc_sample_yield, width=bin_width, bottom=total_yield, color=color,
        )
        mc_containers.append(mc_container)

        # add a black line on top of each sample
        line_x = [y for y in bin_edges for _ in range(2)][1:-1]
        line_y = [y for y in (mc_sample_yield + total_yield) for _ in range(2)]
        ax1.plot(line_x, line_y, "-", color="black", linewidth=0.5)

        total_yield += mc_sample_yield

    # add total MC uncertainty
    mc_unc_container = ax1.bar(
        bin_centers,
        2 * total_model_unc,
        width=bin_width,
        bottom=total_yield - total_model_unc,
        fill=False,
        linewidth=0,
        edgecolor="gray",
        hatch=3 * "/",
    )

    # plot data
    data_container = ax1.errorbar(
        bin_centers_data,
        data_histogram_yields,
        yerr=data_histogram_stdev,
        fmt="o",
        color="k",
    )

    # ratio plot
    ax2.plot(
        [bin_left_edges[0], bin_right_edges[-1]],
        [1, 1],
        "--",
        color="black",
        linewidth=1,
    )  # reference line along y=1

    n_zero_pred = sum(total_yield == 0.0)  # number of bins with zero predicted yields
    if n_zero_pred > 0:
        print("predicted yield is zero in {n_zero_pred} bin(s), excluded from ratio plot")
        
    nonzero_model_yield = total_yield != 0.0

    # add uncertainty band around y=1
    rel_mc_unc = total_model_unc / total_yield
    # do not show band in bins where total model yield is 0
    ax2.bar(
        bin_centers[nonzero_model_yield],
        2 * rel_mc_unc[nonzero_model_yield],
        width=bin_width[nonzero_model_yield],
        bottom=1.0 - rel_mc_unc[nonzero_model_yield],
        fill=False,
        linewidth=0,
        edgecolor="gray",
        hatch=3 * "/",
    )

    # data in ratio plot
    data_model_ratio = data_histogram_yields / total_yield
    data_model_ratio_unc = data_histogram_stdev / total_yield
    # mask data in bins where total model yield is 0
    ax2.errorbar(
        bin_centers_data[nonzero_model_yield],
        data_model_ratio[nonzero_model_yield],
        yerr=data_model_ratio_unc[nonzero_model_yield],
        fmt="o",
        color="k",
    )

    # get the highest single bin yield, from the sum of MC or data
    y_max = max(np.amax(total_yield), np.amax(data_histogram_yields))
    # lowest model yield in single bin (not considering empty bins)
    y_min = np.amin(total_yield[np.nonzero(total_yield)])

    # use log scale if it is requested, otherwise determine scale setting:
    # if yields vary over more than 2 orders of magnitude, set y-axis to log scale
    if log_scale or (log_scale is None and (y_max / y_min) > 100):
        # log vertical axis scale and limits
        ax1.set_yscale("log")
        if variable == "inferno":
            ax1.set_ylim([y_min / 10, y_max * 100])
        else:
            ax1.set_ylim([y_min / 10, y_max * 10])            
    else:
        # do not use log scale
        ax1.set_ylim([0, y_max * 1.5])  # 50% headroom

    # MC contributions in inverse order, such that first legend entry corresponds to
    # the last (highest) contribution to the stack
    all_containers = mc_containers[::-1] + [mc_unc_container, data_container]
    all_labels = mc_labels[::-1] + ["Uncertainty", data_label]
    ax1.legend(
        all_containers, all_labels, frameon=False, fontsize=10, loc="upper right", ncol=2, #handleheight=0.6
    )

    #plt.legend(fontsize='xx-large', ncol=2,handleheight=2.4, labelspacing=0.05)

    ax1.set_xlim(bin_edges[0], bin_edges[-1])
    ax1.set_ylabel("events")
    ax1.set_xticklabels([])
    ax1.set_xticklabels([], minor=True)
    ax1.tick_params(axis="both", which="major", pad=8)  # tick label - axis padding
    ax1.tick_params(direction="in", top=True, right=True, which="both")
    plt.annotate('CMS Open Data',
                xy=(0., 1.02), xycoords=('axes fraction', 'figure fraction'),
                verticalalignment='top',
                fontsize=20, )
    plt.annotate(r'4.1 $\mathrm{fb}^{-1}$ (7TeV)',
                xy=(1., 1.02), xycoords=('axes fraction', 'figure fraction'),
                verticalalignment='top', horizontalalignment="right",
                fontsize=16)

    ax2.set_xlim(bin_edges[0], bin_edges[-1])
    ax2.set_ylim([0., 2.])
    ax2.set_xlabel(variable)#histogram_dict_list[0]["variable"])
    ax2.set_ylabel("data / model")
    #ax2.set_yticks([0.5, 0.75, 1.0, 1.25, 1.5])
    #ax2.set_yticklabels([0.5, 0.75, 1.0, 1.25, ""])
    ax2.set_yticks([0.5, 1.0, 1.5, 2.])
    ax2.set_yticklabels([0.5, 1.0, 1.5, ""])
    ax2.tick_params(axis="both", which="major", pad=8)
    ax2.tick_params(direction="in", top=True, right=True, which="both")

    fig.set_tight_layout(True)
    if store == True:
        fig.savefig(path+"/data_mc.png", bbox_inches="tight") 
    plt.show()
            
            