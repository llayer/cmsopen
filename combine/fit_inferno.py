from inference import create_dc
from inference import to_harvester
from inference import fit
from plotting import plot
import os
import numpy as np
import pandas as pd
import json

NAME = "inferno_cmsopen14"
INPUT_PATH = "/eos/user/l/llayer/cmsopen/columnar/note_v0/input/"+ NAME + "/"
TREE_DIR = INPUT_PATH + "root_trees/"
BASE_DIR = "/eos/user/l/llayer/cmsopen/study_inferno/results/combine/"
FIT_DIR = BASE_DIR + NAME + "/"

# Check Robust Fit
to_hv = False
dc = False
run_combine = True
stat_only = False
impact = False
maxL = True
multi = True
float_qcd = True
robust = True

def load_summary(inpath):
    with open(inpath + "summary.json") as json_file:
        data = json.load(json_file)
    return data

def adjust_naming(syst_names):
    syst = []
    # Adjust the naming for combine
    for s in syst_names:
        if "jes" in s:
            syst.append('jes')
        else:
            syst.append(s)
    return syst

syst=["06_jes", "btag"]

if to_hv:

    if not os.path.exists(FIT_DIR):
        os.makedirs(FIT_DIR)
    to_harvester.inferno_to_harvester(TREE_DIR, outpath=FIT_DIR + 'harvester_input.root', syst = syst)


if dc:

    """
    summary = load_summary(INPUT_PATH)
    print(summary)

    shape_syst = [str(x) for x in summary["shape_syst"]]
    weight_syst = [str(x) for x in summary["weight_syst"]]
    norm_syst = [str(x) for x in summary["norm_syst"]]
    #float_qcd = summary["nonaux_b_norm"]
    # only relevant for JES and TAU
    """
    syst = adjust_naming(syst)#(shape_syst + weight_syst)
    print(syst)


    f = FIT_DIR + "harvester_input.root"
    create_dc.inferno_full(f, FIT_DIR, 'bce', syst , float_qcd=float_qcd)
    create_dc.inferno_full(f, FIT_DIR, 'inferno', syst ,float_qcd=float_qcd)

if run_combine:

    print("Fitting")

    for var in ["bce", "inferno"]:
        outdir = FIT_DIR + "Fit/" + var
        DC_DIR = FIT_DIR + "/DataCard/" + var + "/tt/taujets_" + var + "_" + var + "_7TeV"
        fit.fit_dc(DC_DIR, outdir, stat_only=stat_only, impact=impact, maxL=maxL, multi=multi, robust=robust, asimov=False)
        fit.fit_dc(DC_DIR, outdir, stat_only=stat_only, impact=impact, maxL=maxL, multi=multi, robust=robust, asimov=True)

    if multi:
        print("Plotting likelihood scans")
        indir = FIT_DIR + "Fit/"
        plot.plot_comp_nll(indir)