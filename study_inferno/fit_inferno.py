import sys
sys.path.append('../inference')

import create_dc
import fit
import plotScan
import os
import numpy as np
import pandas as pd
import json

NAME = "inferno_cmsopen11"
INPUT_PATH = "/eos/user/l/llayer/cmsopen/columnar/note_v0/input/"+ NAME + "/"
BASE_DIR = "/eos/user/l/llayer/cmsopen/study_inferno/results/combine/"
FIT_DIR = BASE_DIR + NAME + "/"

# Check Robust Fit
dc = True
run_combine = True
stat_only = False
impact = True
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
    for s in shape_syst + weight_syst:
        if "jes" in s:
            syst.append('jes')
        else:
            syst.append(s)
    return syst

if dc:

    summary = load_summary(INPUT_PATH)
    print(summary)

    shape_syst = [str(x) for x in summary["shape_syst"]]
    weight_syst = [str(x) for x in summary["weight_syst"]]
    norm_syst = [str(x) for x in summary["norm_syst"]]
    #float_qcd = summary["nonaux_b_norm"]
    # only relevant for JES and TAU
    syst = adjust_naming(shape_syst + weight_syst)
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
        plotScan.plot_comp_nll(indir)
