import sys
import ROOT
import numpy as np
from hl_feat import *
from object_selection import *
#from btag import *


def pass_trigger():
    pass


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

    if ( len(jets)>=4 & njets_45>=3):
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

    # Define the input tree
    ff = ROOT.TFile('/eos/user/l/llayer/opendata_files/TTJets/TTJets_4E6F7429-77C5-E311-9481-002590A371D4_942.root') #'nano.root')#'/eos/user/l/llayer/opendata_files/TTJets/TTJets_A84A4A2E-3CC8-E311-BC2C-002590A80DFA_2037.root')
    tree_in = ff.Get("MyModule/Events")
    tree_info = ff.Get("MyModule/Info")


    # Define the output tree
    file = ROOT.TFile("flat.root", 'recreate')
    tree = ROOT.TTree("Cand", "Cand")
    print "Number of events:", tree_in.GetEntries()


    # Create the branches and assign the fill-variables to them as doubles (D)
    id = array('i',[0])
    tmeme = array('i',[0])
    nj_stored = 6
    jet_pt  = array('d',[0]*nj_stored)
    jet_eta = array('d',[0]*nj_stored)
    jet_csvDisc = array('d',[0]*nj_stored)
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

    tree.Branch("id",  id,  'id/I')
    tree.Branch("tmeme",  tmeme,  'tmeme/I')
    tree.Branch("jet_pt",  jet_pt,  'jet_pt[' + str(nj_stored) + ']/D')
    tree.Branch("jet_eta",  jet_eta,  'jet_eta[' + str(nj_stored) + ']/D')
    tree.Branch("jet_csvDisc",  jet_csvDisc,  'jet_csvDisc[' + str(nj_stored) + ']/D')
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
    count_tree = ROOT.TTree("Counts", "Counts")
    evcount_total  = array('i',[0])
    evcount_evfilter  = array('i',[0])
    evcount_trigger = array('i',[0])
    evcount_lep_veto = array('i',[0])
    evcount_jet_requirement = array('i',[0])
    evcount_tau_requirement = array('i',[0])
    evcount_met_requirement = array('i',[0])
    count_tree.Branch("evcount_total",  evcount_total,  'evcount_total/I')
    count_tree.Branch("evcount_evfilter",  evcount_evfilter,  'evcount_evfilter/I')
    count_tree.Branch("evcount_trigger",  evcount_trigger,  'evcount_trigger/I')
    count_tree.Branch("evcount_lep_veto",  evcount_lep_veto,  'evcount_lep_veto/I')
    count_tree.Branch("evcount_jet_requirement",  evcount_jet_requirement,  'evcount_jet_requirement/I')
    count_tree.Branch("evcount_tau_requirement",  evcount_tau_requirement,  'evcount_tau_requirement/I')
    count_tree.Branch("evcount_met_requirement",  evcount_met_requirement,  'evcount_met_requirement/I')
    for info in tree_info:
        evcount_total[0] = info.nEventsTotal
    evcount_evfilter[0] = 0
    evcount_trigger[0] = 0
    evcount_lep_veto[0] = 0
    evcount_jet_requirement[0] = 0
    evcount_tau_requirement[0] = 0
    evcount_met_requirement[0] = 0

    print "Start analysis"

    for counter, evt in enumerate(tree_in):


        if counter%1000 == 0:
            print counter


        # Count events
        evcount_evfilter[0] += 1

        # Object selections
        # Basic cuts:
        jet_pt_cut = 45.
        tau_pt_cut = 40.
        muons = select_muons(evt)
        electrons = select_electrons(evt)
        taus = select_taus(evt, pt_cut=tau_pt_cut)
        taus = clean_taus(taus, muons, electrons)
        #tauHLTs = select_tauHLT(evt)
        jets = select_jets(evt, pt_cut=10.)
        jets = clean_jets(jets, taus, muons, electrons)
        #jetHLTs = select_jetHLT( evt )
        MET = select_met(evt)

        # Store the b-eff
        if store_beff:
            for jet in jets:
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
            if not pass_trigger():
                continue
            else:
                evcount_trigger[0] += 1

        # Lepton veto
        if not pass_lep_veto(muons, electrons):
            continue
        else:
            evcount_lep_veto[0] += 1

        # Jet selection
        if not pass_jet_requirement(jets, pt=jet_pt_cut):
            continue
        else:
            evcount_jet_requirement[0] += 1

        # Tau selection
        if not pass_tau_requirement(taus):
            continue
        else:
            evcount_tau_requirement[0] += 1
            tau = taus[0]

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

        id[0] = evt.event
        tmeme[0] = evt.genEvent_tmeme

        for i, jet in enumerate(jets):
            if i >= nj_stored:
                break
            jet_pt[i] = jet.pt
            jet_eta[i] = jet.eta
            jet_csvDisc[i] = jet.csvDisc
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

        #print tau_pt

        tree.Fill()

    # Fill the final event counts for the cut flow
    count_tree.Fill()

    # Print the candidates tree
    tree.Print()

    # Write the trees into the output file and close the file
    #file.Write()
    tree.Write()
    count_tree.Write()
    h2_BTaggingEff_Denom_b.Write()
    h2_BTaggingEff_Denom_c.Write()
    h2_BTaggingEff_Denom_udsg.Write()
    h2_BTaggingEff_Num_b.Write()
    h2_BTaggingEff_Num_c.Write()
    h2_BTaggingEff_Num_udsg.Write()
    file.Close()


if __name__ == "__main__":

    if len(sys.argv) > 1:
        isData = sys.argv[1]
    else:
        isData = 0
    print "isData =", isData
    createTable(isData = isData)
