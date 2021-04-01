import ROOT as rt
import CombineHarvester.CombineTools.ch as ch
import os

# Create combine harvester
cb = ch.CombineHarvester()

# Input
file = "/eos/user/l/llayer/cmsopen/columnar/syst_variation/histos/harvester_input.root"

# Systematics

norm_systematics = ['xsec']

#norm_shape_systematics = [ 'btag']

specific_shape_systematics = { 'TTJets_bkg':['btag', 'pdf', 'jer', 'trigger', 'met', "tau_e_09", "jes"],
                               'WZJets':['btag', 'jer', 'trigger', 'met', "tau_e_09", "jes"],
                               'STJets':['btag', 'jer', 'trigger', 'met', "tau_e_09", "jes"],
                               'TTJets_signal':['btag', 'pdf', 'jer', 'trigger', 'met', "tau_e_09", "jes"]}

for name, syst in specific_shape_systematics.iteritems():

    f = rt.TFile(file)
    #print(f.ls())
    print "Norm variations"
    for s in syst:
        print name + "_" + s + "Up"
        print name, s, f.Get("signal_region/" + name + "_" + s + "Up").Integral()
        print name, s, f.Get("signal_region/" + name + "_" + s + "Down").Integral()

# Definition of channels
chns = ['bdt']

# Definition of background processes
bkg_procs = {
'bdt' : [ "TTJets_bkg", "WZJets", "STJets", "QCD"]
}

# Definition of signal process
sig_procs = ['TTJets_signal']

# MC processes
mc = [ "TTJets_signal", "TTJets_bkg", "WZJets", "STJets"]

# Categories
cat_names = ["signal_region"]
cats = []
for counter, c in enumerate(cat_names):
    cats.append( (counter, c) )

# Add the processes and observations
for chn in chns:
    cb.AddObservations(  ['tt'],  ['taujets'], ['7TeV'], [chn],                 cats      )
    for key, value in cats:
        cb.AddProcesses(     ['tt'],  ['taujets'], ['7TeV'], [chn], bkg_procs[chn], [cats[key]], False  )

cb.AddProcesses( ['tt'], ['taujets'], ['7TeV'], [chn], sig_procs, cats, True )

# Get the processes
signal = cb.cp().signals().process_set()
bkg = cb.cp().backgrounds().process_set()
all_pr = signal + bkg

# File with the processes
#f = rt.TFile('shapes.root')

# Add the shape systematics
"""
for sys in norm_shape_systematics:
    if sys == '':
        continue
    cb.cp().process(signal + bkg).AddSyst(
    cb, sys, "shape", ch.SystMap()(1.00))
"""
# Systs that only affect a part of the bkg

for key, value in specific_shape_systematics.iteritems():

    for sys in value:
        cb.cp().process([key]).AddSyst(
        cb, sys, "shape", ch.SystMap()(1.00))

# Norm systs
def getSys(hNominal, hUp, hDown):

    nominal = hNominal.Integral()
    up = hUp.Integral()
    down = hDown.Integral()

    syst = 1 + 0.5 * (abs(up - nominal) + abs(down - nominal)) / nominal

    return syst

# Add the rate systematics
f = rt.TFile(file)
for pr in mc:

    hNominal = f.Get('signal_region/' + pr)

    for sys in norm_systematics:

        hUp = f.Get('signal_region/' + pr + '_' + sys +'Up')
        hDown = f.Get('signal_region/' + pr + '_' + sys +'Down')

        syst_uncertainty = round( getSys(hNominal, hUp, hDown), 3)
        print pr, syst_uncertainty

        cb.cp().process([pr]).AddSyst(
        cb, "xsec", "lnN", ch.SystMap()(syst_uncertainty))
        #cb.cp().process([pr]).AddSyst(
        #cb, sys, "lnN", ch.SystMap('channel', 'era', 'bin_id')
        #(['bdt'], ['13TeV'],  [0],  syst_uncertainty))


cb.cp().process(["QCD"]).AddSyst(
cb, "mistag", "lnN", ch.SystMap()(1.05))

# Add lumi uncertainty
cb.cp().process(mc).AddSyst(
cb, "lumi", "lnN", ch.SystMap()(1.02))

# Add tau trigger leg uncertainty
cb.cp().process(mc).AddSyst(
cb, "tau_trigger", "lnN", ch.SystMap()(1.05))

# Add tau id uncertainty
cb.cp().process(mc).AddSyst(
cb, "tau_id", "lnN", ch.SystMap()(1.06))

# Add TTJets variation uncertainties
# Mass
cb.cp().process(["TTJets_signal", "TTJets_bkg"]).AddSyst(
cb, "mass", "lnN", ch.SystMap()(1.03))
# Q2
cb.cp().process(["TTJets_signal", "TTJets_bkg"]).AddSyst(
cb, "q2", "lnN", ch.SystMap()(1.02))
# Parton match
cb.cp().process(["TTJets_signal", "TTJets_bkg"]).AddSyst(
cb, "parton", "lnN", ch.SystMap()(1.03))

"""
cb.cp().process(["TTJets_signal"]).AddSyst(
cb, "lumi", "lnN", ch.SystMap()(1.1))
cb.cp().process(["TTJets_signal"]).AddSyst(
cb, "lumi2", "lnN", ch.SystMap()(1.1))
cb.cp().process(["TTJets_signal"]).AddSyst(
cb, "lumi3", "lnN", ch.SystMap()(1.1))
cb.cp().process(["TTJets_signal"]).AddSyst(
cb, "lumi4", "lnN", ch.SystMap()(1.1))
cb.cp().process(["TTJets_signal"]).AddSyst(
cb, "lumi5", "lnN", ch.SystMap()(1.1))
"""

cb.cp().process(["QCD"]).AddSyst(cb, "qcd_rate", "rateParam", ch.SystMap()(1.00))

cb.PrintSysts()

print( '>> Extracting histograms from input root files...' )
for chn in chns:
    #file = 'shapes.root'
    cb.cp().channel([chn]).era(['7TeV']).backgrounds().ExtractShapes(
        file, '$BIN/$PROCESS', '$BIN/$PROCESS_$SYSTEMATIC')
    cb.cp().channel([chn]).era(['7TeV']).signals().ExtractShapes(
        file, '$BIN/$PROCESS', '$BIN/$PROCESS_$SYSTEMATIC')


writer = ch.CardWriter( 'DataCard_SystVar' + '/$TAG/$MASS/$ANALYSIS_$CHANNEL_$BIN_$ERA.txt',
                        'DataCard_SystVar' + '/$TAG/common/$ANALYSIS_$CHANNEL.input.root')
#writer.SetVerbosity(1)
writer.WriteCards('cmb', cb)
for chn in chns: writer.WriteCards(chn,cb.cp().channel([chn]))


# Combine cards

"""
comb_cards = ""
for cat in cat_names:
    comb_cards +=  opt.datacard +  "taujets_st_" + cat + "_7TeV.txt "
comb_cards += " > " + opt.datacard + opt.cat + ".txt"

print 'Creating workspace'
os.system("combineCards.py " + comb_cards)
"""
"""
if opt.mc == 'mc_cb':
    os.system("echo \"* autoMCStats 1\" >> " + opt.datacard + opt.cat + ".txt")
"""

dc = "DataCard_SystVar/bdt/tt/taujets_bdt_signal_region_7TeV.txt"
os.system("text2workspace.py " + dc)


print()
print()
print()
print( "***********************************************************************************")
print()
print( "Datacard produced" )
print()
print( "***********************************************************************************" )
print()
print()
print()
