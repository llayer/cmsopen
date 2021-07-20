import create_dc
import fit
import plotScan
import os

BASE_DIR = "/eos/user/l/llayer/cmsopen/columnar/note_v0/"
COMBINE_DIR = BASE_DIR + "combine/"
FIT_DIR = COMBINE_DIR + "inferno/"
#DC_DIR = FIT_DIR + "/DataCard/bdt/tt/taujets_bdt_signal_region_7TeV"

dc = False
run_combine = True
stat_only = False
gof = False
closure = False
impact = True
maxL = True
multi = True
plot_ll = False

if __name__ == "__main__":

    if dc:
        f = COMBINE_DIR + "inferno_systematic.root"
        #nuis
        if not os.path.exists(FIT_DIR):
            os.makedirs(FIT_DIR)
        for i in range(10):
        #print("Write datacard")
            create_dc.inferno_dc(f, FIT_DIR, 'bce', '_shift_'+str(i))
            create_dc.inferno_dc(f, FIT_DIR, 'inferno', '_shift_'+str(i))
        """
        # shift
        for nnuiis in range(1,5):
        #print("Write datacard")
            #create_dc.inferno_dc(f, FIT_DIR, 'bce', '_nuis_'+str(nnuiis))
            create_dc.inferno_dc(f, FIT_DIR, 'inferno', '_nuis_'+str(nnuiis))
        """


    if run_combine:

        #if not os.path.exists(FIT_DIR):
        #    os.makedirs(FIT_DIR)

        print("Fitting")

        """
        for i in range(10):
            outdir = FIT_DIR + "Fit/bce_shift_" + str(i)
            DC_DIR = FIT_DIR + "/DataCard/bce_shift_" + str(i) + "/tt/taujets_bce_shift_" + str(i) + "_bce_7TeV"
            fit.fit_dc(DC_DIR, outdir, stat_only=stat_only, impact=impact, maxL=maxL, multi=multi)


        for i in range(10):
            outdir = FIT_DIR + "Fit/inferno_shift_" + str(i)
            DC_DIR = FIT_DIR + "/DataCard/inferno_shift_" + str(i) + \
                        "/tt/taujets_inferno_shift_" + str(i) + "_inferno_shift_" + str(i) + "_7TeV"
            fit.fit_dc(DC_DIR, outdir, stat_only=stat_only, impact=impact, maxL=maxL, multi=multi)


        for nnuiis in range(1,5):
            outdir = FIT_DIR + "Fit/bce_nuis_" + str(nnuiis)
            DC_DIR = FIT_DIR + "/DataCard/bce_nuis_" + str(nnuiis) + "/tt/taujets_bce_nuis_" + str(nnuiis) + "_bce_7TeV"
            fit.fit_dc(DC_DIR, outdir, stat_only=stat_only, impact=impact, maxL=maxL, multi=multi)

        """

        for nnuis in range(1,5):
            outdir = FIT_DIR + "Fit/inferno_nuis_" + str(nnuis)
            DC_DIR = FIT_DIR + "/DataCard/inferno_nuis_" + str(nnuis) + \
                        "/tt/taujets_inferno_nuis_" + str(nnuis) + "_inferno_nuis_" + str(nnuis) + "_7TeV"
            fit.fit_dc(DC_DIR, outdir, stat_only=stat_only, impact=impact, maxL=maxL, multi=multi)

    if plot_ll:
        print("Plotting")
        plotScan.plotNLL(FIT_DIR)
