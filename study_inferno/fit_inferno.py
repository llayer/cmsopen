import sys
sys.path.append('../inference')

import create_dc
import fit
import plotScan
import os
import numpy as np

NAME = "inferno_cmsopen6"
BASE_DIR = "/eos/user/l/llayer/cmsopen/study_inferno/results/combine/"
FIT_DIR = BASE_DIR + NAME + "/"

dc = True
run_combine = True
stat_only = False
gof = False
closure = False
impact = True
maxL = True
multi = True
plot_ll = False
#asimov = True

if dc:
    f = FIT_DIR + "harvester_input.root"
    create_dc.inferno_full(f, FIT_DIR, 'bce', ["bce_jes", "bce_btag"])
    create_dc.inferno_full(f, FIT_DIR, 'inferno', ["inferno_jes", "inferno_btag"])


if run_combine:

    print("Fitting")

    for var in ["bce", "inferno"]:
        outdir = FIT_DIR + "Fit/" + var
        DC_DIR = FIT_DIR + "/DataCard/" + var + "/tt/taujets_" + var + "_" + var + "_7TeV"
        fit.fit_dc(DC_DIR, outdir, stat_only=stat_only, impact=impact, maxL=maxL, multi=multi, asimov=False)
        fit.fit_dc(DC_DIR, outdir, stat_only=stat_only, impact=impact, maxL=maxL, multi=multi, asimov=True)

if plot_ll:
    print("Plotting")
    plotScan.plotNLL(FIT_DIR)
