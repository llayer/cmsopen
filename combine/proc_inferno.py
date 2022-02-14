import sys
sys.path.append('../analysis/plotting')

from plotting import plot_artificial_syst
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import glob as glob
from pathlib import Path
import stack
import json
import ROOT

ROOT.gROOT.SetBatch(True)

NAME = "inferno_cmsopen13"
INPUT_PATH = "/eos/user/l/llayer/cmsopen/columnar/note_v0/input/"+ NAME + "/"
HISTO_PATH = "/eos/user/l/llayer/cmsopen/study_inferno/results/histos/"+ NAME + ".root"
STACK_PATH = "/eos/user/l/llayer/cmsopen/study_inferno/results/stacks/"+ NAME + "/"
SYST_PATH = "/eos/user/l/llayer/cmsopen/study_inferno/results/systs/"+ NAME + "/"
COMBINE_PATH = "/eos/user/l/llayer/cmsopen/study_inferno/results/combine/"+ NAME + "/"

def main():

    samples = load_samples()

    summary = load_summary(INPUT_PATH)
    print(summary)
    shape_syst = summary["shape_syst"]
    weight_syst = summary["weight_syst"]
    norm_syst = summary["norm_syst"]

    variables = get_variables(samples)
    plot_artificial_syst.vars_to_histos(samples, variables, HISTO_PATH, corrs = weight_syst)
    #stack_vars = [ x for x in variables if "nominal" in x["var_name"]]
    plot_stack(variables)
    plot_all_systs(syst = shape_syst + weight_syst)
    inferno_to_harvester(HISTO_PATH, COMBINE_PATH, syst = shape_syst + weight_syst)


def load_summary(inpath):
    with open(inpath + "summary.json") as json_file:
        data = json.load(json_file)
    return data


def load_samples(inpath = INPUT_PATH):
    files = glob.glob(inpath + "*.h5")
    samples = {}
    for sample in files:

        if "summary" in sample: continue
        sample_name = sample.split("/")[-1][:-3]
        print(sample_name)
        samples[sample_name] = pd.read_hdf(sample)

    return samples


def get_variables(samples):
    bce = [ x for x in list(samples["TTJets_signal"]) if "bce" in x]
    inferno = [ x for x in list(samples["TTJets_signal"]) if "inferno" in x]
    inferno_var = []
    for infer in inferno:
        inferno_var.append({"var_name" : infer, "bins" : 10, "xlow" : 0, "xup" : 10, "xtitle" : infer, "max_sf": 50})
    bce_var = []
    for b in bce:
        bce_var.append({"var_name" : b, "bins" : 10, "xlow" : 0, "xup" : 1., "xtitle" : b, "max_sf": 50})
    return inferno_var + bce_var


def plot_stack(variables, file_name = HISTO_PATH, outdir=STACK_PATH):

    Path(outdir).mkdir(parents=True, exist_ok=True)

    import stack
    sample_names = ["TTJets_bkg", "WZJets", "STJets", "QCD", "TTJets_signal"]
    for var in variables:
        stack.plot( file_name, var["var_name"], var["xtitle"], sample_names,
                   outdir, max_sf=var["max_sf"])


def plot_syst(f, nominal, up, down, sample = "TTJets_signal", name="test", bce=True):

    cent = f.Get(sample + "_" + nominal)
    up = f.Get(sample + "_" + up)
    down = f.Get(sample + "_" + down)
    plot_artificial_syst.plot_variation(SYST_PATH, name, name, cent, up, down, bce=bce)


def plot_all_systs(syst):
    Path(SYST_PATH).mkdir(parents=True, exist_ok=True)
    f = ROOT.TFile(HISTO_PATH, "READ")
    for c in syst:
        plot_syst(f, 'bce', str(c) + '_up' + "_bce",
                  str(c) + '_down' + "_bce", name='bce_' + str(c), bce=True)
        plot_syst(f, 'inferno', str(c) + '_up' + "_inferno", str(c) + '_down' + "_inferno", name='inferno_' + str(c), bce=False)




def inferno_to_harvester(file_path, outpath, syst = []):

    Path(COMBINE_PATH).mkdir(parents=True, exist_ok=True)
    f = ROOT.TFile(file_path)
    outfile = ROOT.TFile(outpath + "harvester_input.root", 'RECREATE')

    for var in ["bce", "inferno"]:

        outfile.mkdir(var)
        outfile.cd(var)
        h = f.Get("Data" + "_" + var)
        h.Write('data_obs')

        for sample in [ "QCD", "TTJets_bkg", "WZJets", "STJets", "TTJets_signal"]:
            h = f.Get(sample + "_" + var)
            h.Write(sample)

            if sample == "TTJets_signal":

                for c in syst:#, "met"]:
                    if "jes" in c:
                        name = "jes"
                    else:
                        name = c
                    h = f.Get(sample + "_" + str(c) + '_up' + "_bce")
                    h.Write(sample + "_" + name + "Up")
                    h = f.Get(sample + "_" + str(c) + '_down' + "_bce")
                    h.Write(sample + "_" + name + "Down")

    print(outfile.ls())
    outfile.Close()


if __name__ == "__main__":

    main()
