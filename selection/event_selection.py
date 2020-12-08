import sys
import ROOT
import numpy as np
from hl_feat import *
from object_selection import *
#from btag import *


f_jet40 = ROOT.TFile("/afs/cern.ch/work/l/llayer/CMSSW_5_3_32/src/workspace/selection/data/rootFilesTurnOn/TriggerEffHisto_data_match40_JETLEG.root")
f_jet45 = ROOT.TFile("/afs/cern.ch/work/l/llayer/CMSSW_5_3_32/src/workspace/selection/data/rootFilesTurnOn/TriggerEffHisto_data_match45_JETLEG.root")
f_tau40 = ROOT.TFile("/afs/cern.ch/work/l/llayer/CMSSW_5_3_32/src/workspace/selection/data/rootFilesTurnOn/TriggerEffHisto_match40_newTauID.root")
f_tau45 = ROOT.TFile("/afs/cern.ch/work/l/llayer/CMSSW_5_3_32/src/workspace/selection/data/rootFilesTurnOn/TriggerEffHisto_match45_newTauID.root")
h_eff_jet40 = f_jet40.Get("jet4_eff")
h_eff_jet45 = f_jet45.Get("jet4_eff")
h_eff_tau40 = f_tau40.Get("eff_tau")
h_eff_tau45 = f_tau45.Get("eff_tau")


def eval_trigger_weight(jet_pt0, jet_pt1, jet_pt2, tau_pt):

    trigRange = np.random.uniform(low=0.0, high=1.0)
    if trigRange<0.218: #0.208 new lumi
        jet_hist = h_eff_jet40
        tau_hist = h_eff_tau40
    else:
        jet_hist = h_eff_jet45
        tau_hist = h_eff_tau45

    jet0_weight = jet_hist.GetBinContent( jet_hist.GetXaxis().FindBin(jet_pt0) )
    jet1_weight = jet_hist.GetBinContent( jet_hist.GetXaxis().FindBin(jet_pt1) )
    jet2_weight = jet_hist.GetBinContent( jet_hist.GetXaxis().FindBin(jet_pt2) )
    tau_weight = tau_hist.GetBinContent( tau_hist.GetXaxis().FindBin(tau_pt) )

    return 1. * jet0_weight * jet1_weight * jet2_weight * tau_weight


def pass_pre_sel(evt, isData):

    if isData:
        if not ((evt.HLT_QuadJet40_IsoPFTau40 == 1) | (evt.HLT_QuadJet45_IsoPFTau45 == 1)):
            return False

    njets = 0
    for pt in evt.Jet_pt:
        if pt > 40.:
            njets += 1
    if not ( njets>=3 ):
        return False

    ntaus = 0
    for pt in evt.Tau_pt:
        if pt > 40.:
            ntaus += 1
    if not ( ntaus>=1 ):
        return False

    return True


def runs_40(evt):

    if evt.HLT_QuadJet40_IsoPFTau40 != 1:
        return False
    if ( evt.run < 165970 ) | ((evt.run > 166782) & (evt.run<171050)):
        return True
    else:
        return False

def runs_45(evt):

    if evt.HLT_QuadJet45_IsoPFTau45 != 1:
        return False
    if ((evt.run>=165970) & (evt.run<=166782)) | ((evt.run>=171050) & (evt.run<178421)):
        return True
    else:
        return False

def pass_trigger(evt):

    """
    # HLT_QuadJet40_IsoPFTau40
    if (evt.run<165970 ) | ((evt.run>166782 & evt.run<171050)):
        if evt.HLT_QuadJet40_IsoPFTau40 == 1:
            return True
        else:
            return False
    # HLT_QuadJet45_IsoPFTau45
    elif ((evt.run>=165970) & (evt.run<=166782)) | ((evt.run>=171050) & (evt.run<178421)):
        if evt.HLT_QuadJet45_IsoPFTau45 == 1:
            return True
        else:
            return False
    else:
        return False
    """
    """
    if (evt.HLT_QuadJet40_IsoPFTau40 == 1) | (evt.HLT_QuadJet45_IsoPFTau45 == 1):
        return True
    else:
        return False
    """

    pass_40 = runs_40(evt)
    pass_45 = runs_45(evt)

    if (pass_40 == True) | (pass_45 == True):
        return True
    else:
        return False



def pass_lep_veto(muons, electrons):

    if (len(muons) > 0) | (len(electrons) > 0):
        return False
    else:
        return True

def pass_jet_requirement(jets, pt = 45.):

    njets_45 = 0
    for jet in jets:
        if (jet.pt > pt):
            #isMatched = jet_match(jet, jetHLTs)
            njets_45 += 1

    if ( len(jets)>=4 ) & (njets_45>=3):
        return True
    else:
        return False

def pass_tau_requirement(taus, pt=0.):

    # Tau multiplicity DANGER check commented code - matching
    if len(taus)!=1:
        return False
    else:
        return True
        #isMatched = tau_match(tau, tauHLTs)

def pass_met_requiremet(MET, et=20.):

    pass
    #if evt.MET_pt < 20:



def createTable(isData, store_beff=True):

    # Set some variables
    if isData:
        store_beff = False

    # Define the input tree
    ff = ROOT.TFile( "/eos/user/l/llayer/opendata_files/preselection/Run2011A_MultiJet/pre_Run2011A_MultiJet_3E5961E5-3F43-E311-B290-0025B3203716_1633_920.root" )#"/eos/user/l/llayer/opendata_files/preselection/TTJets/pre_TTJets_F86622BF-3FC7-E311-9BA5-002590A80D9C_2997_7193.root") #ff = ROOT.TFile( '/eos/user/l/llayer/opendata_files/legacy/TTJets/TTJets_54887428-76C6-E311-BE32-0025902009B0_1020.root')  # ff = ROOT.TFile( '/eos/user/l/llayer/opendata_files/legacy/DYJetsToLL/DYJetsToLL_AA11C768-AEBC-E311-B184-0025905A60BE_4018.root')
    #tree_in = ff.Get("MyModule/Events")
    #tree_info = ff.Get("MyModule/Info")

    tree_in = ff.Get("Events")
    tree_info = ff.Get("Counts")

    # Define the output tree
    file = ROOT.TFile("flat.root", 'recreate')
    tree = ROOT.TTree("Cand", "Cand")
    print "Number of events:", tree_in.GetEntries()


    # Create the branches and assign the fill-variables to them as doubles (D)
    event_id = array('d',[0])
    run_number = array('i',[0])
    lumi_block = array('i',[0])
    tmeme = array('i',[0])
    nj_stored = 6
    jet_pt  = array('d',[0]*nj_stored)
    jet_eta = array('d',[0]*nj_stored)
    jet_csvDisc = array('d',[0]*nj_stored)
    jet_flavour = array('d',[0]*nj_stored)
    #jet_btagSF = array('d',[0]*nj_stored)
    tau_pt = array('d',[0])
    tau_eta = array('d',[0])
    ht = array('d',[0])
    aplanarity = array('d',[0])
    sphericity = array('d',[0])
    chargeEta = array('d',[0])
    met = array('d',[0])
    deltaPhiTauMet = array('d',[0])
    mt = array('d',[0])
    mTauJet = array('d',[0])

    tree.Branch("event_id",  event_id,  'id/D')
    tree.Branch("run_number",  run_number,  'id/I')
    tree.Branch("lumi_block",  lumi_block,  'id/I')
    tree.Branch("tmeme",  tmeme,  'tmeme/I')
    tree.Branch("jet_pt",  jet_pt,  'jet_pt[' + str(nj_stored) + ']/D')
    tree.Branch("jet_eta",  jet_eta,  'jet_eta[' + str(nj_stored) + ']/D')
    tree.Branch("jet_csvDisc",  jet_csvDisc,  'jet_csvDisc[' + str(nj_stored) + ']/D')
    tree.Branch("jet_flavour",  jet_flavour,  'jet_flavour[' + str(nj_stored) + ']/D')
    #tree.Branch("jet_btagSF",  jet_btagSF,  'jet_btagSF[' + str(nj_stored) + ']/D')
    tree.Branch("tau_pt",  tau_pt,  'tau_pt/D')
    tree.Branch("tau_eta",  tau_eta,  'tau_eta/D')
    tree.Branch("ht",  tau_pt,  'tau_pt/D')
    tree.Branch("aplanarity",  aplanarity,  'aplanarity/D')
    tree.Branch("sphericity",  sphericity,  'sphericity/D')
    tree.Branch("chargeEta",  chargeEta,  'chargeEta/D')
    tree.Branch("met",  met,  'met/D')
    tree.Branch("deltaPhiTauMet",  deltaPhiTauMet,  'deltaPhiTauMet/D')
    tree.Branch("mt",  mt,  'mt/D')
    tree.Branch("mTauJet",  mTauJet,  'mTauJet/D')

    if not isData:
        trig_weight = array('d',[0])
        tree.Branch("trig_weight",  trig_weight,  'trig_weight/D')
    else:
        HLT_QuadJet40_IsoPFTau40 = array('i',[0])
        HLT_QuadJet45_IsoPFTau45 = array('i',[0])
        tree.Branch("HLT_QuadJet40_IsoPFTau40",  HLT_QuadJet40_IsoPFTau40,  'HLT_QuadJet40_IsoPFTau40/I')
        tree.Branch("HLT_QuadJet45_IsoPFTau45",  HLT_QuadJet45_IsoPFTau45,  'HLT_QuadJet45_IsoPFTau45/I')

    if store_beff:
        # Histos to store the b-tag efficiency
        # https://github.com/rappoccio/usercode/blob/Dev_53x/EDSHyFT/plugins/BTaggingEffAnalyzer.cc
        # DANGER check the Disc value!! set to medium
        discriminatorValue = 0.679
        ptNBins            = 100
        ptMin              = 0.
        ptMax              = 1000.
        etaNBins           = 60
        etaMin             = -3.
        etaMax             = 3.
        h2_BTaggingEff_Denom_b    = ROOT.TH2D("h2_BTaggingEff_Denom_b", ";p_{T} [GeV];#eta", ptNBins, ptMin, ptMax, etaNBins, etaMin, etaMax)
        h2_BTaggingEff_Denom_c    = ROOT.TH2D("h2_BTaggingEff_Denom_c", ";p_{T} [GeV];#eta", ptNBins, ptMin, ptMax, etaNBins, etaMin, etaMax)
        h2_BTaggingEff_Denom_udsg = ROOT.TH2D("h2_BTaggingEff_Denom_udsg", ";p_{T} [GeV];#eta", ptNBins, ptMin, ptMax, etaNBins, etaMin, etaMax)
        h2_BTaggingEff_Num_b    = ROOT.TH2D("h2_BTaggingEff_Num_b", ";p_{T} [GeV];#eta", ptNBins, ptMin, ptMax, etaNBins, etaMin, etaMax)
        h2_BTaggingEff_Num_c    = ROOT.TH2D("h2_BTaggingEff_Num_c", ";p_{T} [GeV];#eta", ptNBins, ptMin, ptMax, etaNBins, etaMin, etaMax)
        h2_BTaggingEff_Num_udsg = ROOT.TH2D("h2_BTaggingEff_Num_udsg", ";p_{T} [GeV];#eta", ptNBins, ptMin, ptMax, etaNBins, etaMin, etaMax)

    # Tree to store the event counts
    btag = ROOT.TTree("btag", "btag")
    b_flavour  = array('i',[0])
    b_pt  = array('d',[0])
    b_eta = array('d',[0])
    b_csvDisc = array('d',[0])
    btag.Branch("flavour",  b_flavour,  'b_flavour/I')
    btag.Branch("pt",  b_pt,  'b_pt/D')
    btag.Branch("eta",  b_eta,  'b_eta/D')
    btag.Branch("csvDisc",  b_csvDisc,  'b_csvDisc/D')


    # Tree to store the event counts
    count_tree = ROOT.TTree("Counts", "Counts")
    evcount_total  = array('i',[0])
    evcount_evfilter  = array('i',[0])
    evcount_presel  = array('i',[0])
    evcount_trigger = array('i',[0])
    evcount_jet_requirement = array('i',[0])
    evcount_tau_requirement = array('i',[0])
    evcount_lep_veto = array('i',[0])
    evcount_met_requirement = array('i',[0])
    count_tree.Branch("evcount_total",  evcount_total,  'evcount_total/I')
    count_tree.Branch("evcount_evfilter",  evcount_evfilter,  'evcount_evfilter/I')
    count_tree.Branch("evcount_presel",  evcount_presel,  'evcount_presel/I')
    count_tree.Branch("evcount_trigger",  evcount_trigger,  'evcount_trigger/I')
    count_tree.Branch("evcount_jet_requirement",  evcount_jet_requirement,  'evcount_jet_requirement/I')
    count_tree.Branch("evcount_tau_requirement",  evcount_tau_requirement,  'evcount_tau_requirement/I')
    count_tree.Branch("evcount_lep_veto",  evcount_lep_veto,  'evcount_lep_veto/I')
    count_tree.Branch("evcount_met_requirement",  evcount_met_requirement,  'evcount_met_requirement/I')
    #for info in tree_info:
    #    evcount_total[0] = info.nEventsTotal
    evcount_evfilter[0] = 0
    evcount_presel[0] = 0
    evcount_trigger[0] = 0
    evcount_lep_veto[0] = 0
    evcount_jet_requirement[0] = 0
    evcount_tau_requirement[0] = 0
    evcount_met_requirement[0] = 0

    if not isData:
        evcount_taujet_w = array('d',[0])
        evcount_taujet_lepveto_w = array('d',[0])
        count_tree.Branch("evcount_taujet_w",  evcount_taujet_w,  'evcount_taujet_w/D')
        count_tree.Branch("evcount_taujet_lepveto_w",  evcount_taujet_lepveto_w,  'evcount_taujet_lepveto_w/D')
        evcount_taujet_w[0] = 0.
        evcount_taujet_lepveto_w[0] = 0.

        # Tree to store the signal counts
        signal_tree = ROOT.TTree("Signal", "Signal")
        evsignal_total  = array('i',[0])
        evsignal_evfilter  = array('i',[0])
        evsignal_trigger = array('i',[0])
        evsignal_jet_requirement = array('i',[0])
        evsignal_tau_requirement = array('i',[0])
        evsignal_lep_veto = array('i',[0])
        evsignal_met_requirement = array('i',[0])
        signal_tree.Branch("evsignal_total",  evsignal_total,  'evsignal_total/I')
        signal_tree.Branch("evsignal_evfilter",  evsignal_evfilter,  'evsignal_evfilter/I')
        signal_tree.Branch("evsignal_trigger",  evsignal_trigger,  'evsignal_trigger/I')
        signal_tree.Branch("evsignal_jet_requirement",  evsignal_jet_requirement,  'evsignal_jet_requirement/I')
        signal_tree.Branch("evsignal_tau_requirement",  evsignal_tau_requirement,  'evsignal_tau_requirement/I')
        signal_tree.Branch("evsignal_lep_veto",  evsignal_lep_veto,  'evsignal_lep_veto/I')
        signal_tree.Branch("evsignal_met_requirement",  evsignal_met_requirement,  'evsignal_met_requirement/I')
        #for info in tree_info:
        #    evsignal_total[0] = info.nEventsTotal
        evsignal_evfilter[0] = 0
        evsignal_trigger[0] = 0
        evsignal_lep_veto[0] = 0
        evsignal_jet_requirement[0] = 0
        evsignal_tau_requirement[0] = 0
        evsignal_met_requirement[0] = 0
        evsignal_taujet_w = array('d',[0])
        evsignal_taujet_lepveto_w = array('d',[0])
        signal_tree.Branch("evsignal_taujet_w",  evsignal_taujet_w,  'evsignal_taujet_w/D')
        signal_tree.Branch("evsignal_taujet_lepveto_w",  evsignal_taujet_lepveto_w,  'evsignal_taujet_lepveto_w/D')
        evsignal_taujet_w[0] = 0.
        evsignal_taujet_lepveto_w[0] = 0.

    print "Start analysis"

    ev_counter = 0

    for counter, evt in enumerate(tree_in):


        if counter%1000 == 0:
            print counter


        # Count events
        evcount_evfilter[0] += 1
        if not isData:
            if evt.genEvent_tmeme == 10000:
                evsignal_evfilter[0] += 1

        # Preselection:
        """
        if not pass_pre_sel(evt, isData):
            continue
        else:
            evcount_presel[0] += 1
        """

        # Object selections
        # Basic cuts:
        jet_pt_cut = 45.
        muons = select_muons(evt)
        electrons = select_electrons(evt)
        taus = select_taus(evt)
        #tauHLTs = select_tauHLT(evt)
        jets = select_jets(evt, isData = isData)

        taus = clean_taus(taus, muons, electrons)
        jets = clean_jets(jets, taus, muons, electrons)
        #jetHLTs = select_jetHLT( evt )
        MET = select_met(evt)




        """
        if len(evt.Tau_pt) > 0:
            print "Tau"
            for t in evt.Tau_pt:
                print t



        if len(taus) > 0:
            print
            print "Tau preselected"
            for t in evt.Tau_pt:
                print t
            print "Tau selected"
            for t in taus:
                print t.pt
        """


        if store_beff:
            for jet in jets:

                """
                b_flavour[0] = jet.flavour
                b_pt[0] = jet.pt
                b_eta[0] = jet.eta
                b_csvDisc[0] = jet.csvDisc
                btag.Fill()
                """

                if( abs(jet.flavour)==5 ):
                    h2_BTaggingEff_Denom_b.Fill(jet.pt, jet.eta)
                    if( jet.csvDisc >= discriminatorValue ):
                        h2_BTaggingEff_Num_b.Fill(jet.pt, jet.eta)
                elif( abs(jet.flavour)==4 ):
                    h2_BTaggingEff_Denom_c.Fill(jet.pt, jet.eta)
                    if( jet.csvDisc >= discriminatorValue ):
                        h2_BTaggingEff_Num_c.Fill(jet.pt, jet.eta)
                else:
                    h2_BTaggingEff_Denom_udsg.Fill(jet.pt, jet.eta)
                    if( jet.csvDisc >= discriminatorValue ):
                        h2_BTaggingEff_Num_udsg.Fill(jet.pt, jet.eta)

        """
        #### TEMP #####
        if len(taus) < 1:
            continue
        else:
            tau = taus[0]

        hl = hlFeatures(jets, tau, MET)
        #print hl

        """

        # Event selections
        # Trigger
        if isData:
            if not pass_trigger(evt):
                continue
            else:
                evcount_trigger[0] += 1

        ev_counter +=1

        # Jet selection
        if not pass_jet_requirement(jets, pt=jet_pt_cut):
            continue
        else:
            evcount_jet_requirement[0] += 1
            if not isData:
                if evt.genEvent_tmeme == 10000:
                    evsignal_jet_requirement[0] += 1



        # Tau selection
        if not pass_tau_requirement(taus):
            continue
        else:
            evcount_tau_requirement[0] += 1
            if not isData:
                if evt.genEvent_tmeme == 10000:
                    evsignal_tau_requirement[0] += 1
            tau = taus[0]

        # Trigger weight for MC
        if not isData:
            trigger_weight = eval_trigger_weight(jets[0].pt, jets[1].pt, jets[2].pt, tau.pt)
            evcount_taujet_w[0] += trigger_weight
            if evt.genEvent_tmeme == 10000:
                evsignal_taujet_w[0] += trigger_weight

        # Lepton veto
        if not pass_lep_veto(muons, electrons):
            continue
        else:
            evcount_lep_veto[0] += 1
            if not isData:
                evcount_taujet_lepveto_w[0] += trigger_weight
                if evt.genEvent_tmeme == 10000:
                    evsignal_lep_veto[0] += 1
                    evsignal_taujet_lepveto_w[0] += trigger_weight

        # MET selection
        """
        if not pass_met_requirement(MET):
            continue
        else:
            evcount_met_requirement[0] += 1
        """

        # Create the HL features
        hl = hlFeatures(jets, tau, MET)


        # Create the table:
        print "Fill table"

        event_id[0] = evt.event
        run_number[0] = evt.run
        lumi_block[0] = evt.luminosityBlock
        if isData:
            tmeme[0] = -9999
        else:
            tmeme[0] = evt.genEvent_tmeme

        for i, jet in enumerate(jets):
            if i >= nj_stored:
                break
            jet_pt[i] = jet.pt
            jet_eta[i] = jet.eta
            jet_csvDisc[i] = jet.csvDisc
            jet_flavour[i] = jet.flavour
            """
            if isData:
                jet_btagSF[i] = eval_btagSF(1, jet.eta, jet.pt)
            else:
                jet_btagSF[i] = eval_btagSF(jet.flavour, jet.eta, jet.pt)
            """

        tau_pt[0] = tau.pt
        tau_eta[0] = tau.eta
        ht[0] = hl['ht']
        aplanarity[0] = hl['aplanarity']
        sphericity[0] = hl['sphericity']
        chargeEta[0] = hl['chargeEta']
        met[0] = hl['met']
        deltaPhiTauMet[0] = hl['deltaPhiTauMet']
        mt[0] = hl['mt']
        mTauJet[0] = hl['mTauJet']
        if not isData:
            trig_weight[0] = trigger_weight
        else:
            HLT_QuadJet40_IsoPFTau40[0] = evt.HLT_QuadJet40_IsoPFTau40
            HLT_QuadJet45_IsoPFTau45[0] = evt.HLT_QuadJet45_IsoPFTau45

        #print tau_pt

        tree.Fill()

    # Fill the final event counts for the cut flow
    count_tree.Fill()

    if not isData:
        signal_tree.Fill()

    # Print the candidates tree
    tree.Print()

    # Write the trees into the output file and close the file
    #file.Write()
    tree.Write()
    #btag.Write()
    count_tree.Write()
    if not isData:
        signal_tree.Write()
    if store_beff:
        h2_BTaggingEff_Denom_b.Write()
        h2_BTaggingEff_Denom_c.Write()
        h2_BTaggingEff_Denom_udsg.Write()
        h2_BTaggingEff_Num_b.Write()
        h2_BTaggingEff_Num_c.Write()
        h2_BTaggingEff_Num_udsg.Write()
    file.Close()

    print "Counter", ev_counter


if __name__ == "__main__":

    if len(sys.argv) > 1:
        isData = sys.argv[1]
    else:
        isData = 0
    print "isData =", isData

    createTable(isData = int(isData))
