import uproot
import numpy as np
import pandas as pd

def get_hist_vals(h):
    vals = {}
    vals["vals"] = h.values()
    vals["vals"][vals["vals"] == 0] = 1.
    vals["errors"] = h.errors()
    vals["edges"] = h.axis().edges()
    return vals

def get_trigger_hists(weight_type = "new"):
    if weight_type == "new":
        f = uproot.open("/home/centos/cmsopen/columnar/data/trigger_eff.root")
        tau45 = f["tau_eff_45"]
        tau40 = f["tau_eff_40"]
        jet45 = f["jet4_eff_45"]
        jet40 = f["jet4_eff_40"]
    else:
        jet40 = uproot.open("/home/centos/cmsopen/columnar/data/rootFilesTurnOn/TriggerEffHisto_data_match40_JETLEG.root")["jet4_eff"]
        jet45 = uproot.open("/home/centos/cmsopen/columnar/data/rootFilesTurnOn/TriggerEffHisto_data_match45_JETLEG.root")["jet4_eff"]
        tau40 = uproot.open("/home/centos/cmsopen/columnar/data/rootFilesTurnOn/TriggerEffHisto_match40_newTauID.root")["eff_tau"]
        tau45 = uproot.open("/home/centos/cmsopen/columnar/data/rootFilesTurnOn/TriggerEffHisto_match45_newTauID.root")["eff_tau"]
    tau40_vals = get_hist_vals(tau40)
    tau45_vals = get_hist_vals(tau45)
    jet40_vals = get_hist_vals(jet40)
    jet45_vals = get_hist_vals(jet45)    
    return tau40_vals, tau45_vals, jet40_vals, jet45_vals

tau40_vals, tau45_vals, jet40_vals, jet45_vals = get_trigger_hists()

def to_col(x, idx=0):
    return x[int(idx)]

def apply_weight(df, tau40_vals, tau45_vals, jet40_vals, jet45_vals):
    
    df["Tau_pt0"] = df["Tau_pt"].apply(to_col, args=[0])
    df["Jet_pt0"] = df["Jet_pt"].apply(to_col, args=[0])
    df["Jet_pt1"] = df["Jet_pt"].apply(to_col, args=[1])
    df["Jet_pt2"] = df["Jet_pt"].apply(to_col, args=[2])
    
    df["Tau_pt0_idx"] = np.digitize(df["Tau_pt0"], tau40_vals["edges"])
    df["Jet_pt0_idx"] = np.digitize(df["Jet_pt0"], jet40_vals["edges"])
    df["Jet_pt1_idx"] = np.digitize(df["Jet_pt1"], jet40_vals["edges"])
    df["Jet_pt2_idx"] = np.digitize(df["Jet_pt2"], jet40_vals["edges"]) 
    
    df["Tau_pt0_idx"] = df["Tau_pt0_idx"].replace(19, 18)
    df["Jet_pt0_idx"] = df["Jet_pt0_idx"].replace(20, 19)
    df["Jet_pt1_idx"] = df["Jet_pt1_idx"].replace(20, 19)
    df["Jet_pt2_idx"] = df["Jet_pt2_idx"].replace(20, 19)
    
    mask = df["trigger_is40"]
    df["Tau_pt0_errors"] = np.where(mask, tau40_vals["errors"][df["Tau_pt0_idx"]-1], tau45_vals["errors"][df["Tau_pt0_idx"]-1])
    df["Tau_pt0_vals"] = np.where(mask, tau40_vals["vals"][df["Tau_pt0_idx"]-1], tau45_vals["vals"][df["Tau_pt0_idx"]-1])
    df["Jet_pt0_errors"] = np.where(mask, jet40_vals["errors"][df["Jet_pt0_idx"]-1], jet45_vals["errors"][df["Jet_pt0_idx"]-1])
    df["Jet_pt0_vals"] = np.where(mask, jet40_vals["vals"][df["Jet_pt0_idx"]-1], jet45_vals["vals"][df["Jet_pt0_idx"]-1])
    df["Jet_pt1_errors"] = np.where(mask, jet40_vals["errors"][df["Jet_pt1_idx"]-1], jet45_vals["errors"][df["Jet_pt1_idx"]-1])
    df["Jet_pt1_vals"] = np.where(mask, jet40_vals["vals"][df["Jet_pt1_idx"]-1], jet45_vals["vals"][df["Jet_pt1_idx"]-1])
    df["Jet_pt2_errors"] = np.where(mask, jet40_vals["errors"][df["Jet_pt2_idx"]-1], jet45_vals["errors"][df["Jet_pt2_idx"]-1])
    df["Jet_pt2_vals"] = np.where(mask, jet40_vals["vals"][df["Jet_pt2_idx"]-1], jet45_vals["vals"][df["Jet_pt2_idx"]-1])
    
"""    
apply_weight(df, tau40_vals, tau45_vals, jet40_vals, jet45_vals)

w = df["Tau_pt0_vals"] * df["Jet_pt0_vals"] * df["Jet_pt1_vals"] * df["Jet_pt2_vals"]
w_err = w*(1+df["Tau_pt0_errors"]) * (1+df["Jet_pt0_errors"]) * (1+df["Jet_pt1_errors"]) * (1+df["Jet_pt2_errors"])
w_err_tau = w *(1+df["Tau_pt0_errors"])
w_err_jet = w *(1+df["Jet_pt0_errors"]) * (1+df["Jet_pt1_errors"]) * (1+df["Jet_pt2_errors"])
"""

f = uproot.open("/home/centos/data/TTJets_pdf.root")
pdf = f["pdfana/Events"].arrays(library="pd")

def rename_pdf(df):
    rename_dict = {"MCEvt_cteq66_pdf_weights[0]":"pdf"}
    i_weight = 0
    for i in range(1,45):
        name = "MCEvt_cteq66_pdf_weights[" + str(i) + "]"
        df[name] = df[name] / df["MCEvt_cteq66_pdf_weights[0]"]
        if i%2 == 0:

            rename_dict[name] = "pdf_weight_" + str(i_weight) + "_down"
            i_weight += 1
        else:
            rename_dict[name] = "pdf_weight_" + str(i_weight) + "_up"
    df = df.rename(columns=rename_dict)
    df.to_hdf("/home/centos/data/TTJets_pdf_renamed.root", "frame")

def get_summed_pdf(pdf):
    
    nominal = pdf["MCEvt_cteq66_pdf_weights[0]"]
    ups, downs = [], []
    for i in range(1,45):
        if i%2 == 0:
            #print( "Down" , x[i])
            downs.append((pdf["MCEvt_cteq66_pdf_weights[" + str(i) + "]"] / nominal)-1)
        else:
            #print( "Up" , x[i])
            ups.append((pdf["MCEvt_cteq66_pdf_weights[" + str(i) + "]"] / nominal) -1 )
    
    #weight_up = np.sqrt( np.sum(np.array(ups)**2 ) )
    #weight_down = np.sqrt( np.sum(np.array(downs)**2 ) )
    return ups, downs

u, d = get_summed_pdf(pdf)
weight_up = np.sqrt( np.sum(np.array(u)**2, axis=0 ) )
weight_down = np.sqrt( np.sum(np.array(d)**2, axis=0 ) )