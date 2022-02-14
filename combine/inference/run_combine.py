import create_dc
import fit
import plotScan

BASE_DIR = "/eos/user/l/llayer/cmsopen/columnar/note_v0/"
COMBINE_DIR = BASE_DIR + "combine/"
FIT_DIR = COMBINE_DIR + "nominal"
DC_DIR = FIT_DIR + "/DataCard/bdt/tt/taujets_bdt_signal_region_7TeV"

dc = False
run_combine = False
stat_only = False
gof = False
closure = False
impact = True
maxL = False
multi = False
plot_ll = True

if __name__ == "__main__":

    if dc:
        print("Write datacard")
        f = COMBINE_DIR + "harvester_input.root"
        create_dc.run_harvester(f, FIT_DIR)

    if run_combine:
        print("Fitting")
        fit.fit_dc(DC_DIR, FIT_DIR, stat_only=stat_only, impact=impact,
                    maxL=maxL, multi=multi)

    if plot_ll:
        print("Plotting")
        plotScan.plotNLL(FIT_DIR)
