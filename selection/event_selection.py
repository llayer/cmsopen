import ROOT
import numpy as np
from hl_feat import *
from object_selection import *
from btag import *


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

def pass_tau_requirement(taus, pt):

    # Tau multiplicity DANGER check commented code - matching
    if len(taus)!=1:
        return False
    else:
        return True
        #isMatched = tau_match(tau, tauHLTs)

def pass_met_requiremet(MET, et=20.):

    pass
    #if evt.MET_pt < 20:



def createTable():

    # Define the input tree
    ff = ROOT.TFile('nano.root')#'/eos/user/l/llayer/opendata_files/TTJets/TTJets_A84A4A2E-3CC8-E311-BC2C-002590A80DFA_2037.root')
    tree_in = ff.Get("MyModule/Events")

    # Define the output tree
    file = ROOT.TFile("flat.root", 'recreate')
    tree = ROOT.TTree("Cand", "Cand")

    # Dict to store the event counts
    event_count = {"events_file" : 0,
                   "pass_trigger" : 0,
                   "pass_lep_veto" : 0,
                   "pass_jet_requirement" : 0,
                   "pass_tau_requirement" : 0,
                   "pass_met_requirement" : 0}

    # create the branches and assign the fill-variables to them as doubles (D)
    id = array('i',[0])
    tmeme = array('i',[0])
    nj_stored = 6
    jet_pt  = array('d',[0]*nj_stored)
    jet_eta = array('d',[0]*nj_stored)
    jet_csvDisc = array('d',[0]*nj_stored)
    jet_btagSF = array('d',[0]*nj_stored)
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
    tree.Branch("jet_btagSF",  jet_btagSF,  'jet_btagSF[' + str(nj_stored) + ']/D')
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

    print "Start analysis"

    for counter, evt in enumerate(tree_in):


        if counter%1000 == 0:
            print counter


        # Count events
        event_count['events_file'] += 1

        # Object selections
        muons = select_muons(evt)
        electrons = select_electrons(evt)
        taus = select_taus(evt, pt_cut=40.)
        taus = clean_taus(taus, muons, electrons)
        #tauHLTs = select_tauHLT(evt)
        jets = select_jets(evt, pt_cut=10.)
        jets = clean_jets(jets, taus, muons, electrons)
        #jetHLTs = select_jetHLT( evt )
        MET = select_met(evt)

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
        if not pass_trigger():
            continue
        else:
            event_count['pass_trigger'] += 1

        # Lepton veto
        if not pass_lep_veto(muons, electrons):
            continue
        else:
            event_count['pass_lep_veto'] += 1

        # Jet selection
        if not pass_jet_requirement(jets, pt-45.):
            continue
        else:
            event_count['pass_jet_requirement'] += 1

        # Tau selection
        if not pass_tau_requirement(taus):
            continue
        else:
            event_count['pass_tau_requirement'] += 1
            tau = taus[0]

        # MET selection
        if not pass_met_requirement(MET):
            continue
        else:
            event_count['pass_jet_requirement'] += 1

        # Create the HL features
        hl = hlFeatures(jets, tau, MET)
        """

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
            jet_btagSF[i] = eval_btagSF(jet.flavour, jet.eta, jet.pt)

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

        """
        # B-tagging
        bjets = []
        bjets_disc = []
        for j in jets:
            if j.bdisc > btagDiscriCut_:
                bjets.append(j)
                bjets_disc.append(j.bdisc)

        if isMC:
            if bjets < 1:
                continue
            else:
                bweight = getWeightAtLeast1bjet()
                tauweight = getWeightTau() # CHECK!!!!
                tauleg = getWeightTauLeg()
                jetleg = getWeightJetLegs()

        else:
            if bjets < 1:
                isqcd = True
            else:
                isdata = True

        """

    tree.Print()

    # write the tree into the output file and close the file
    file.Write()
    file.Close()


if __name__ == "__main__":

    createTable()
