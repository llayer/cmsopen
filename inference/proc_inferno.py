import create_dc
import fit
import plotScan
import os
import numpy as np

BASE_DIR = "/eos/user/l/llayer/cmsopen/columnar/note_v0/"
COMBINE_DIR = BASE_DIR + "combine/"
FIT_DIR = COMBINE_DIR + "inferno_systematic5/" #"inferno_cmsopen/" #"inferno_systematic7/"
#DC_DIR = FIT_DIR + "/DataCard/bdt/tt/taujets_bdt_signal_region_7TeV"

dc = False
run_combine = True
stat_only = False
gof = False
closure = False
impact = False
maxL = True
multi = False
plot_ll = False
asimov = True

if __name__ == "__main__":

    if dc:
        f = COMBINE_DIR + "inferno_cmsopen.root" #"inferno_systematic7.root"
        #shift
        if not os.path.exists(FIT_DIR):
            os.makedirs(FIT_DIR)

        """
        for i in range(10):
        #print("Write datacard")
            create_dc.inferno_dc(f, FIT_DIR, 'bce', '_shift_'+str(i))
            for suffix in ["approx", "analytical"]:
                create_dc.inferno_dc(f, FIT_DIR, 'inferno', '_shift_' + suffix + '_' +str(i))

        """
        """
        # nuis
        for nnuiis in range(1,5):
        #print("Write datacard")
            create_dc.inferno_dc(f, FIT_DIR, 'bce', '_nuis_'+str(nnuiis))
            for suffix in ["approx", "analytical"]:
                create_dc.inferno_dc(f, FIT_DIR, 'inferno', '_nuis_' + suffix + '_' +str(nnuiis))
        """

        """
        for i, norm in enumerate(np.linspace(0.02,0.2,10)):
        #print("Write datacard")
            create_dc.inferno_norm_dc(f, FIT_DIR, 'bce', '_norm' + '_' + str(i), norm=1.+norm)
            for suffix in ["approx", "analytical"]:
                create_dc.inferno_norm_dc(f, FIT_DIR, 'inferno', '_norm_' + suffix + '_' +str(i), norm=1.+norm)
        """

        #create_dc.inferno_full(f, FIT_DIR, 'bce', ["bce_jes", "bce_btag"])
        #create_dc.inferno_full(f, FIT_DIR, 'inferno', ["inferno_jes", "inferno_btag"])


    if run_combine:

        #if not os.path.exists(FIT_DIR):
        #    os.makedirs(FIT_DIR)

        print("Fitting")

        """
        for i in range(10):
            for suffix in ["approx", "analytical"]:
                outdir = FIT_DIR + "Fit/inferno_shift_" + suffix + "_" + str(i)
                DC_DIR = FIT_DIR + "/DataCard/inferno_shift_" + suffix + "_" + str(i) + \
                            "/tt/taujets_inferno_shift_" + suffix + "_" + str(i) + "_inferno_shift_" + suffix + "_"  + str(i) + "_7TeV"
                fit.fit_dc(DC_DIR, outdir, stat_only=stat_only, impact=impact, maxL=maxL, multi=multi, asimov=asimov)
        """
        for i in range(10):
            outdir = FIT_DIR + "Fit/bce_shift_" + str(i)
            DC_DIR = FIT_DIR + "/DataCard/bce_shift_" + str(i) + "/tt/taujets_bce_shift_" + str(i) + "_bce_7TeV"
            fit.fit_dc(DC_DIR, outdir, stat_only=stat_only, impact=impact, maxL=maxL, multi=multi, asimov=asimov)
        """

        for nnuis in range(1,5):
            for suffix in ["approx", "analytical"]:
                outdir = FIT_DIR + "Fit/inferno_nuis_" + suffix + "_" + str(nnuis)
                DC_DIR = FIT_DIR + "/DataCard/inferno_nuis_" + suffix + "_" + str(nnuis) + \
                            "/tt/taujets_inferno_nuis_" + suffix + "_" + str(nnuis) + "_inferno_nuis_" + suffix + "_"  + str(nnuis) + "_7TeV"
                fit.fit_dc(DC_DIR, outdir, stat_only=stat_only, impact=impact, maxL=maxL, multi=multi)

        for nnuiis in range(1,5):
            outdir = FIT_DIR + "Fit/bce_nuis_" + str(nnuiis)
            DC_DIR = FIT_DIR + "/DataCard/bce_nuis_" + str(nnuiis) + "/tt/taujets_bce_nuis_" + str(nnuiis) + "_bce_7TeV"
            fit.fit_dc(DC_DIR, outdir, stat_only=stat_only, impact=impact, maxL=maxL, multi=multi)
        """

        """
        for i in range(10):
            for suffix in ["approx", "analytical"]:
                outdir = FIT_DIR + "Fit/inferno_norm_" + suffix + "_" + str(i)
                DC_DIR = FIT_DIR + "/DataCard/inferno_norm_" + suffix + "_" + str(i) + \
                            "/tt/taujets_inferno_norm_" + suffix + "_" + str(i) + "_inferno_norm_" + suffix + "_"  + str(i) + "_7TeV"
                fit.fit_dc(DC_DIR, outdir, stat_only=stat_only, impact=impact, maxL=maxL, multi=multi)
        """
        """
        for i in range(10):
            outdir = FIT_DIR + "Fit/bce_norm_" + str(i)
            DC_DIR = FIT_DIR + "/DataCard/bce_norm_" + str(i) + "/tt/taujets_bce_norm_" + str(i) + "_bce_7TeV"
            fit.fit_dc(DC_DIR, outdir, stat_only=stat_only, impact=impact, maxL=maxL, multi=multi)
        """

        """
        for var in ["bce", "inferno"]:
            outdir = FIT_DIR + "Fit/" + var
            DC_DIR = FIT_DIR + "/DataCard/" + var + "/tt/taujets_" + var + "_" + var + "_7TeV"
            fit.fit_dc(DC_DIR, outdir, stat_only=stat_only, impact=impact, maxL=maxL, multi=multi, asimov=asimov)
        """

    if plot_ll:
        print("Plotting")
        plotScan.plotNLL(FIT_DIR)
