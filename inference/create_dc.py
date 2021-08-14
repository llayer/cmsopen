import ROOT as rt
import CombineHarvester.CombineTools.ch as ch
import os



def inferno_full(file, outpath, variable, variation='_norm_0', norm=1.):

    cb = ch.CombineHarvester()

    # Systematics
    # specific_shape_systematics = { 'TTJets_signal':['Var'] }
    if "inferno" in variable:
        specific_shape_systematics = { 'TTJets_signal':[variable + variation] } #Artificial variation
    else:
        specific_shape_systematics = { 'TTJets_signal':[variable + "_norm"] } #Artificial variation

    # Definition of channels
    chns = [variable + variation]

    # Definition of bacxkground processes
    bkg_procs = {
    variable + variation : [ "TTJets_bkg", "WZJets", "STJets", "QCD"]
    }

    # Definition of signal process
    sig_procs = ['TTJets_signal']

    # MC processes
    mc = [ "TTJets_signal", "TTJets_bkg", "WZJets", "STJets"]

    # Categories
    if 'inferno' in variable:
        cat_names = [variable + variation]
    else:
        cat_names = [variable]
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


    for key, value in specific_shape_systematics.iteritems():

        for sys in value:
            cb.cp().process([key]).AddSyst(
            cb, sys, "shape", ch.SystMap()(1.00))


    cb.cp().process(["TTJets_signal"]).AddSyst(
    cb, "norm", "lnN", ch.SystMap()(norm))

    cb.cp().process(["QCD"]).AddSyst(cb, "qcd_rate", "rateParam", ch.SystMap()(1.00))

    cb.PrintSysts()

    print( '>> Extracting histograms from input root files...' )
    for chn in chns:
        #file = 'shapes.root'
        cb.cp().channel([chn]).era(['7TeV']).backgrounds().ExtractShapes(
            file, '$BIN/$PROCESS', '$BIN/$PROCESS_$SYSTEMATIC')
        cb.cp().channel([chn]).era(['7TeV']).signals().ExtractShapes(
            file, '$BIN/$PROCESS', '$BIN/$PROCESS_$SYSTEMATIC')


    writer = ch.CardWriter( outpath + 'DataCard' + '/$TAG/$MASS/$ANALYSIS_$CHANNEL_$BIN_$ERA.txt',
                            outpath + 'DataCard' + '/$TAG/common/$ANALYSIS_$CHANNEL.input.root')
    #writer.SetVerbosity(1)
    writer.WriteCards('cmb', cb)
    for chn in chns: writer.WriteCards(chn,cb.cp().channel([chn]))

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


def inferno_norm_dc(file, outpath, variable, variation='_norm_0', norm=1.):

    cb = ch.CombineHarvester()

    # Systematics
    # specific_shape_systematics = { 'TTJets_signal':['Var'] }
    if "inferno" in variable:
        specific_shape_systematics = { 'TTJets_signal':[variable + variation] } #Artificial variation
    else:
        specific_shape_systematics = { 'TTJets_signal':[variable + "_norm"] } #Artificial variation

    # Definition of channels
    chns = [variable + variation]

    # Definition of background processes
    bkg_procs = {
    variable + variation : [ "TTJets_bkg", "WZJets", "STJets", "QCD"]
    }

    # Definition of signal process
    sig_procs = ['TTJets_signal']

    # MC processes
    mc = [ "TTJets_signal", "TTJets_bkg", "WZJets", "STJets"]

    # Categories
    if 'inferno' in variable:
        cat_names = [variable + variation]
    else:
        cat_names = [variable]
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


    for key, value in specific_shape_systematics.iteritems():

        for sys in value:
            cb.cp().process([key]).AddSyst(
            cb, sys, "shape", ch.SystMap()(1.00))


    cb.cp().process(["TTJets_signal"]).AddSyst(
    cb, "norm", "lnN", ch.SystMap()(norm))

    cb.cp().process(["QCD"]).AddSyst(cb, "qcd_rate", "rateParam", ch.SystMap()(1.00))

    cb.PrintSysts()

    print( '>> Extracting histograms from input root files...' )
    for chn in chns:
        #file = 'shapes.root'
        cb.cp().channel([chn]).era(['7TeV']).backgrounds().ExtractShapes(
            file, '$BIN/$PROCESS', '$BIN/$PROCESS_$SYSTEMATIC')
        cb.cp().channel([chn]).era(['7TeV']).signals().ExtractShapes(
            file, '$BIN/$PROCESS', '$BIN/$PROCESS_$SYSTEMATIC')


    writer = ch.CardWriter( outpath + 'DataCard' + '/$TAG/$MASS/$ANALYSIS_$CHANNEL_$BIN_$ERA.txt',
                            outpath + 'DataCard' + '/$TAG/common/$ANALYSIS_$CHANNEL.input.root')
    #writer.SetVerbosity(1)
    writer.WriteCards('cmb', cb)
    for chn in chns: writer.WriteCards(chn,cb.cp().channel([chn]))

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



def inferno_dc(file, outpath, variable, variation='_shift_0'):


    cb = ch.CombineHarvester()

    # Systematics
    # specific_shape_systematics = { 'TTJets_signal':['Var'] }
    if "shift" in variation:
        specific_shape_systematics = { 'TTJets_signal':[variable + variation] } #Artificial variation
    if "nuis" in variation:
        vars = []
        if "inferno" in variable:
            for i in range(int(variation[-1])):
                vars.append(variable + variation + "_var_" + str(i))
        else:
            for i in range(1, int(variation[-1])+1):
                vars.append(variable + "_nuis_" + str(i))

        specific_shape_systematics = { 'TTJets_signal':vars }
    # Definition of channels
    chns = [variable + variation]

    # Definition of background processes
    bkg_procs = {
    variable + variation : [ "TTJets_bkg", "WZJets", "STJets", "QCD"]
    }

    # Definition of signal process
    sig_procs = ['TTJets_signal']

    # MC processes
    mc = [ "TTJets_signal", "TTJets_bkg", "WZJets", "STJets"]

    # Categories
    if 'inferno' in variable:
        cat_names = [variable + variation]
    else:
        cat_names = [variable]
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


    for key, value in specific_shape_systematics.iteritems():

        for sys in value:
            cb.cp().process([key]).AddSyst(
            cb, sys, "shape", ch.SystMap()(1.00))


    cb.cp().process(["QCD"]).AddSyst(cb, "qcd_rate", "rateParam", ch.SystMap()(1.00))

    cb.PrintSysts()

    print( '>> Extracting histograms from input root files...' )
    for chn in chns:
        #file = 'shapes.root'
        cb.cp().channel([chn]).era(['7TeV']).backgrounds().ExtractShapes(
            file, '$BIN/$PROCESS', '$BIN/$PROCESS_$SYSTEMATIC')
        cb.cp().channel([chn]).era(['7TeV']).signals().ExtractShapes(
            file, '$BIN/$PROCESS', '$BIN/$PROCESS_$SYSTEMATIC')


    writer = ch.CardWriter( outpath + 'DataCard' + '/$TAG/$MASS/$ANALYSIS_$CHANNEL_$BIN_$ERA.txt',
                            outpath + 'DataCard' + '/$TAG/common/$ANALYSIS_$CHANNEL.input.root')
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

    #dc = outpath + "/DataCard/bdt/tt/taujets_bdt_signal_region_7TeV.txt"
    #os.system("text2workspace.py " + dc)


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




def run_harvester(file, outpath):
    # Create combine harvester
    cb = ch.CombineHarvester()

    # Systematics

    norm_systematics = ['xsec']

    #norm_shape_systematics = [ 'btag']

    specific_shape_systematics = { 'TTJets_bkg':['btag', 'pdf', 'jer', 'met', "taue", "jes", "trigger"], #trigger
                                   'WZJets':['btag', 'jer', 'met', "taue", "jes", "trigger"], #trigger
                                   'STJets':['btag', 'jer', 'met', "taue", "jes", "trigger"], #trigger
                                   'TTJets_signal':['btag', 'pdf', 'jer', 'met', "taue", "jes", "trigger"]} #trigger

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


    writer = ch.CardWriter( outpath + '/DataCard' + '/$TAG/$MASS/$ANALYSIS_$CHANNEL_$BIN_$ERA.txt',
                            outpath + '/DataCard' + '/$TAG/common/$ANALYSIS_$CHANNEL.input.root')
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

    dc = outpath + "/DataCard/bdt/tt/taujets_bdt_signal_region_7TeV.txt"
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


if __name__ == "__main__":

    """
    file_path = "/eos/user/l/llayer/cmsopen/columnar/syst_variation/combine/harvester_input.root"
    outpath = "/eos/user/l/llayer/cmsopen/columnar/syst_variation/combine"
    run_harvester(file_path, outpath)
    """

    file_path = "/eos/user/l/llayer/cmsopen/columnar/note_v0/combine/harvester_inferno_signal.root"
    outpath = "/eos/user/l/llayer/cmsopen/columnar/note_v0/combine"
    inferno_dc(file_path, outpath)
