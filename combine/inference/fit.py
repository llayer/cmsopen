import os, commands
import plot_pulls
# Used datacard
#datacard = "/afs/cern.ch/work/l/llayer/CMSSW_10_2_18/src/cmsopen/inference/DataCard_SystVar/bdt/tt/taujets_bdt_signal_region_7TeV"
#datacard = "/afs/cern.ch/work/l/llayer/CMSSW_10_2_18/src/cmsopen/inference/DataCard/bdt/tt/taujets_bdt_signal_region_7TeV.txt"

def fit_dc( datacard, outpath, stat_only=False, impact=False, maxL=False, multi=False, robust=True, asimov=False ):

    print "Fitting datacard:", datacard

    # Output directory
    if stat_only == True:
        outdir = outpath + "/stat"
    else:
        outdir = outpath + "/syst"

    if asimov == True:
        print("FITTING WITH ASIMOV DATA")
        suffix = " -t -1 --expectSignal 1"
        outdir += "_asimov"
    else:
        suffix = ""

    if robust == True:
        suffix += " --robustFit 1"

    print("Suffix:", suffix)

    if not os.path.exists(outdir):
        os.makedirs(outdir)


    if(impact):

        os.system("text2workspace.py " + datacard + ".txt")
        para  = ""#" --parallel 8"

        # CHECK --squareDistPoiStep and--autoRange
        os.system("combineTool.py -M Impacts -d  " + datacard + ".root -m 125 --autoBoundsPOIs r  --autoRange 2 --doInitialFit" + suffix + para)
        os.system("combineTool.py -M Impacts -d  " + datacard + ".root -m 125 --autoBoundsPOIs r  --autoRange 2  --doFits" + suffix + para)
        os.system("combineTool.py -M Impacts -d  " + datacard + ".root -m 125 --autoBoundsPOIs r  --autoRange 2 --squareDistPoiStep -o" + outdir + "/prova.json" + suffix)


        os.system("plotImpacts.py -i " + outdir + "/prova.json -o impacts --pullDef relDiffAsymErrs")
        os.system("mv impacts.pdf " + outdir)
        os.system("rm *.root")



    if(maxL):

        command = "combine -M FitDiagnostics " + datacard + ".txt --out " + outdir + " --saveNormalizations  --saveShapes" + suffix
        #command = "combine -M FitDiagnostics " + datacard + ".txt" + suffix
        if stat_only:
            command += " --freezeParameters allConstrainedNuisances"
        os.system( command )

        command = "python ..//..//HiggsAnalysis/CombinedLimit/test/diffNuisances.py " + outdir + "/fitDiagnosticsTest.root" + \
                  " --pullDef relDiffAsymErrs  | sed -e 's/!/ /g' -e 's/,/ /g' | sed -e 's/*/ /g' -e 's/,/ /g' | tail -n +3 > " + outdir + "/pulls.txt"
        #print( command )
        os.system( command )
        os.system("cat " + outdir + "/pulls.txt")
        plot_pulls.plot(outdir)


    if(multi):
        # DANGER add robust fit??
        paramRange = "--setParameterRanges r=0.,2."
        #os.system("combine " + datacard + " -M MultiDimFit --algo=grid  --points 200 " + paramRange )# --robustFit 1 --stepSize 0.004 )
        command = "combine " + datacard + ".txt -M MultiDimFit --algo=grid  --points 200 " + paramRange + suffix
        if stat_only:
            command += " --freezeParameters allConstrainedNuisances"
        os.system( command )

        os.system("mv higgsCombineTest.MultiDimFit.mH120.root " + outdir)
