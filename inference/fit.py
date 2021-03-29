import os, commands

# Used datacard
datacard = "/afs/cern.ch/work/l/llayer/CMSSW_10_2_18/src/cmsopen/inference/DataCard_Syst/bdt/tt/taujets_bdt_signal_region_7TeV.txt"
#datacard = "/afs/cern.ch/work/l/llayer/CMSSW_10_2_18/src/cmsopen/inference/DataCard/bdt/tt/taujets_bdt_signal_region_7TeV.txt"

stat_only = True

gof = False
closure = False
impact = False
maxL = True
multi = True

print "Fitting datacard:", datacard

# Output directory
outdir = "out"
if not os.path.exists(outdir):
    os.makedirs(outdir)

if (gof):
    # Evtl. fix signal strength
    os.system("combine -M GoodnessOfFit " + datacard + ".txt --algo=KS")
    os.system("combine -M GoodnessOfFit " + datacard + ".txt --algo=KS -t 100")

if (closure):
    os.system("combine -M FitDiagnostics " + datacard + ".txt -t -1 --expectSignal 0")
    os.system("combine -M FitDiagnostics " + datacard + ".txt -t -1 --expectSignal 1")
    os.system("python diffNuisances.py fitDiagnostics.root")

if(impact):
    os.system("combineTool.py -M Impacts -d  " + datacard + ".root -m 125 --autoBoundsPOIs r  --robustFit 1 --autoRange 1 --doInitialFit")
    os.system("combineTool.py -M Impacts -d  " + datacard + ".root -m 125 --autoBoundsPOIs r  --robustFit 1 --autoRange 1 --doFits")
    os.system("combineTool.py -M Impacts -d  " + datacard + ".root -m 125 --autoBoundsPOIs r  --autoRange 1 -o prova.json")
    os.system("plotImpacts.py -i prova.json  -o impacts")
    os.system("mv impacts.pdf "+directory)

if(maxL):
    #--cminDefaultMinimizerStrategy 0 TEST
    #os.system("combine -M FitDiagnostics " + datacard + ".root --out " + outdir ) #  --saveShapes --saveWithUncertainties --plots --saveNormalizations --robustFit 1 --customStartingPoint --stepSize 0.004 --out " + directory + " --profilingMode all --keepFailures  --autoBoundsPOIs r")

    command = "combine -M FitDiagnostics " + datacard + " --out " + outdir
    if stat_only:
        command += " --freezeParameters allConstrainedNuisances"
    os.system( command )


    """
    print "Making pull distributions ..."
    os.system("./fit_macros/make_pulls.sh " + directory +"/fitDiagnostics.root")
    os.system("cat pulls.txt")
    os.system("python fit_macros/pulls.py -f pulls.txt")
    os.system("mv pulls.txt "+directory)
    os.system("mv pulls.tex "+directory)
    os.system("mv pulls_no_binbybin.png "+directory)
    os.system("mv rho_no_binbybin.png "+directory)
    """

if(multi):
    # DANGER add robust fit??
    paramRange = "--setParameterRanges r=0.5,1.5"
    #os.system("combine " + datacard + " -M MultiDimFit --algo=grid  --points 200 " + paramRange )# --robustFit 1 --stepSize 0.004 )
    command = "combine " + datacard + " -M MultiDimFit --algo=grid  --points 200 " + paramRange
    if stat_only:
        command += " --freezeParameters allConstrainedNuisances"
    os.system( command )

    if stat_only:
        os.system("mv higgsCombineTest.MultiDimFit.mH120.root " + outdir + "/multi_stat.root")
    else:
        os.system("mv higgsCombineTest.MultiDimFit.mH120.root " + outdir + "/multi_syst.root")
    #if opt.multidim == False:
    #    os.system("python fit_macros/plotScan.py -o " + directory + ' --cat ' + opt.cat)
