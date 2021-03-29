import ROOT as rt
import CombineHarvester.CombineTools.ch as ch

# Create combine harvester
cb = ch.CombineHarvester()

# Systematics
#norm_shape_systematics = [ 'btag']

specific_shape_systematics = { 'TTJets_bkg':['btag', 'tau_e'],
                               'WZJets':['btag', 'tau_e'],
                               'STJets':['btag', 'tau_e'],
                               'TTJets_signal':['btag', 'tau_e']}



# Definition of channels
chns = ['bdt']

# Definition of background processes
bkg_procs = {
'bdt' : [ "TTJets_bkg", "WZJets", "STJets", "QCD"]
}

# Definition of signal process
sig_procs = ['TTJets_signal']

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


# Add lumi uncertainty

#cb.cp().process(signal + bkg).AddSyst(
#cb, "lumi", "lnN", ch.SystMap()(1.027))


cb.cp().process(["QCD"]).AddSyst(cb, "qcd_rate", "rateParam", ch.SystMap()(1.00))

cb.PrintSysts()

print( '>> Extracting histograms from input root files...' )
for chn in chns:
    #file = 'shapes.root'
    file = "/eos/user/l/llayer/cmsopen/columnar/syst_scale/histos/harvester_input.root"
    cb.cp().channel([chn]).era(['7TeV']).backgrounds().ExtractShapes(
        file, '$BIN/$PROCESS', '$BIN/$PROCESS_$SYSTEMATIC')
    cb.cp().channel([chn]).era(['7TeV']).signals().ExtractShapes(
        file, '$BIN/$PROCESS', '$BIN/$PROCESS_$SYSTEMATIC')


writer = ch.CardWriter( 'DataCard_Syst' + '/$TAG/$MASS/$ANALYSIS_$CHANNEL_$BIN_$ERA.txt',
                        'DataCard_Syst' + '/$TAG/common/$ANALYSIS_$CHANNEL.input.root')
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

"""
os.system("text2workspace.py " + opt.datacard + opt.cat + ".txt")
"""

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
