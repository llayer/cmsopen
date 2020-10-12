import sys
import ROOT
from collections import namedtuple
import numpy as np
from object_selection import *


def select_jets_hack(evt, isData = False, eta_cut = 2.5, pt_cut = 10., trigger="45"):

    Jet = namedtuple('Jet', 'eta pt px py pz e pxHLT pyHLT pzHLT eHLT')
    #Jet = namedtuple('Jet', 'eta pt px py pz e pxHLT pyHLT pzHLT eHLT')
    good_jets = []

    for iJet in range(evt.nJet):

        #print "csvDisc", round(evt.Jet_csvDisc[iJet], 2), "Flavour", abs(evt.Jet_flavour[iJet]), "Pt", round(evt.Jet_pt[iJet],2), "Eta", round(abs(evt.Jet_eta[iJet]),2)

        # Eta cut
        if abs(evt.Jet_eta[iJet]) > eta_cut: continue
        # Pt cut
        if abs(evt.Jet_pt[iJet]) < pt_cut: continue


        if trigger == "45":
            hlt = [evt.Jet_pxHLT45[iJet], evt.Jet_pyHLT45[iJet], evt.Jet_pzHLT45[iJet], evt.Jet_eHLT45[iJet]]
        else:
            hlt = [evt.Jet_pxHLT40[iJet], evt.Jet_pyHLT40[iJet], evt.Jet_pzHLT40[iJet], evt.Jet_eHLT40[iJet]]

        if isData:
            flavour = -9999.
        else:
            flavour = evt.Jet_flavour[iJet]

        good_jets.append(Jet( evt.Jet_eta[iJet], evt.Jet_pt[iJet], evt.Jet_px[iJet], evt.Jet_py[iJet],
                              evt.Jet_pz[iJet], evt.Jet_e[iJet], hlt[0], hlt[1], hlt[2], hlt[3]))

    return good_jets



def mu_trigger(evt):

    trigger_mu = []
    trigger_mu_v1to5 = []

    trigger_mu.append( evt.HLT_Mu15_v )
    trigger_mu.append( evt.HLT_Mu20_v )
    trigger_mu.append( evt.HLT_Mu24_v )
    trigger_mu.append( evt.HLT_Mu30_v )
    trigger_mu_v1to5.append( evt.HLT_Mu15_v1 )
    trigger_mu_v1to5.append( evt.HLT_Mu15_v2 )
    trigger_mu_v1to5.append( evt.HLT_Mu15_v3 )
    trigger_mu_v1to5.append( evt.HLT_Mu15_v4 )
    trigger_mu_v1to5.append( evt.HLT_Mu15_v5 )
    trigger_mu_v1to5.append( evt.HLT_Mu20_v1 )
    trigger_mu_v1to5.append( evt.HLT_Mu20_v2 )
    trigger_mu_v1to5.append( evt.HLT_Mu20_v3 )
    trigger_mu_v1to5.append( evt.HLT_Mu20_v4 )
    trigger_mu_v1to5.append( evt.HLT_Mu20_v5 )
    trigger_mu_v1to5.append( evt.HLT_Mu24_v1 )
    trigger_mu_v1to5.append( evt.HLT_Mu24_v2 )
    trigger_mu_v1to5.append( evt.HLT_Mu24_v3 )
    trigger_mu_v1to5.append( evt.HLT_Mu24_v4 )
    trigger_mu_v1to5.append( evt.HLT_Mu24_v5 )
    trigger_mu_v1to5.append( evt.HLT_Mu30_v1 )
    trigger_mu_v1to5.append( evt.HLT_Mu30_v2 )
    trigger_mu_v1to5.append( evt.HLT_Mu30_v3 )
    trigger_mu_v1to5.append( evt.HLT_Mu30_v4 )
    trigger_mu_v1to5.append( evt.HLT_Mu30_v5 )

    return trigger_mu, trigger_mu_v1to5


def prescales( evt ):

    ps = [
    evt.Prescale_HLT_QuadJet40_IsoPFTau40_v1,evt.Prescale_HLT_QuadJet40_IsoPFTau40_v2,evt.Prescale_HLT_QuadJet40_IsoPFTau40_v3,
    evt.Prescale_HLT_QuadJet40_IsoPFTau40_v4,evt.Prescale_HLT_QuadJet40_IsoPFTau40_v5,evt.Prescale_HLT_QuadJet40_IsoPFTau40_v6,
    evt.Prescale_HLT_QuadJet40_IsoPFTau40_v7,evt.Prescale_HLT_QuadJet40_IsoPFTau40_v8,evt.Prescale_HLT_QuadJet40_IsoPFTau40_v9,
    evt.Prescale_HLT_QuadJet40_IsoPFTau40_v10,evt.Prescale_HLT_QuadJet40_IsoPFTau40_v11,evt.Prescale_HLT_QuadJet40_IsoPFTau40_v12,
    evt.Prescale_HLT_QuadJet40_IsoPFTau40_v13,evt.Prescale_HLT_QuadJet40_IsoPFTau40_v14,evt.Prescale_HLT_QuadJet40_IsoPFTau40_v15
    ]

    return ps


def pass_jet_tag(jets):

    n_tag_jets = 0
    for i in range(3):
        if (jets[i].pt > 70.) & (jets[i].pxHLT != -9999):
            n_tag_jets += 1

    if n_tag_jets == 3:
        return True
    else:
        return False


def pass_fourjet_tag(jets, tau, filterStatus):

    # Look for four PF jets matched to the HLT jets
    deltaRMin = 99.
    deltaR =99.
    njet = 0
    for idx, jet in enumerate(jets):

        if (jet.pt > 10.) & (abs(jet.eta) < 2.4) & (jet.pxHLT != -9999):

            jet_vec = ROOT.TLorentzVector(jet.px, jet.py, jet.pz, jet.e)
            jetHLT_vec = ROOT.TLorentzVector(jet.pxHLT, jet.pyHLT, jet.pzHLT, jet.eHLT)

            if (jet_vec.DeltaR(jetHLT_vec)<0.4):

                njet += 1

                tau_vec = ROOT.TLorentzVector(tau.px, tau.py, tau.pz, tau.e)
                deltaR = jet_vec.DeltaR(tau_vec)
                if(deltaR < deltaRMin):
                    deltaRMin = deltaR


    # Only continue if 4 HLT jets are in the event and the trigger bit has fired
    if not (njet>=4) & (deltaRMin<0.4) & (filterStatus==1):
        return False
    else:
        return True


def hlt_match_tau(tau, tauHLTs):

    tau_vec = ROOT.TLorentzVector(tau.px, tau.py, tau.pz, tau.e)

    deltaRMinTrig = 99.
    deltaRTrig =99.

    tauHLT_best_eta = -9999.
    tauHLT_best_pt = -9999.
    for tauHLT in tauHLTs:

        tauHLT_vec = ROOT.TLorentzVector(tauHLT.px, tauHLT.py, tauHLT.pz, tauHLT.e)
        deltaRTrig = tau_vec.DeltaR(tauHLT_vec)
        #print deltaRTrig
        if deltaRTrig<deltaRMinTrig:
            deltaRMinTrig = deltaRTrig;
            tauHLT_best_eta = tauHLT_vec.Eta()
            tauHLT_best_pt = tauHLT_vec.Pt()

    return deltaRMinTrig, tauHLT_best_eta, tauHLT_best_pt




def createTable(type):

    # Define the input tree
    ff = ROOT.TFile( '/eos/user/l/llayer/opendata_files/Run2011A_MultiJet/Run2011A_MultiJet_00C33FCA-6E43-E311-BE16-003048F1BFB6_631.root' )# '/eos/user/l/llayer/opendata_files/Run2011A_SingleMu/Run2011A_SingleMu_100.root')
    tree_in = ff.Get("MyModule/Events")

    # Define the output tree
    file = ROOT.TFile("trigger.root", 'recreate')
    tree = ROOT.TTree("Events", "Events")

    # create the branches and assign the fill-variables to them as doubles (D)
    id = array('i',[0])
    run = array('i',[0])
    trig_type = array('i',[0])
    #ps = array('d',[0]*15)
    n_taus = array('i',[0])
    n_taus40 = array('i',[0])
    tree.Branch("id",  id,  'id/I')
    tree.Branch("run",  run,  'run/I')
    tree.Branch("trig_type",  trig_type,  'trig_type/I')
    #tree.Branch("ps",  ps,  'ps[15]/D')
    tree.Branch("n_taus",  n_taus,  'n_taus/I')
    tree.Branch("n_taus40",  n_taus40,  'n_taus40/I')

    if type == "jet":
        trigger_mu = array('i',[0]*4)
        trigger_mu_v1to5 = array('i',[0]*20)
        jet_probe_pt = array('d',[0])
        jet_probe_eta = array('d',[0])
        jet_probe_pxHLT = array('d',[0])
        tree.Branch("trigger_mu",  trigger_mu,  'trigger_mu[4]/I')
        tree.Branch("trigger_mu_v1to5",  trigger_mu_v1to5,  'trigger_mu_v1to5[20]/I')
        tree.Branch("jet_probe_pt",  jet_probe_pt,  'jet_probe_pt/D')
        tree.Branch("jet_probe_eta",  jet_probe_eta,  'jet_probe_eta/D')
        tree.Branch("jet_probe_pxHLT",  jet_probe_pxHLT,  'jet_probe_pxHLT/D')
    elif type == "tau":
        tau_filterStatus1 = array('i',[0])
        tau_filterStatus2 = array('i',[0])
        tau_probe_pt = array('d',[0])
        tau_probe_eta = array('d',[0])
        tau_probe_match_dR = array('d',[0])
        tau_probe_hlt_eta = array('d',[0])
        tau_probe_hlt_pt = array('d',[0])
        tree.Branch("tau_filterStatus1",  tau_filterStatus1,  'tau_filterStatus1/I')
        tree.Branch("tau_filterStatus2",  tau_filterStatus2,  'tau_filterStatus2/I')
        tree.Branch("tau_probe_pt",  tau_probe_pt,  'tau_probe_pt/D')
        tree.Branch("tau_probe_eta",  tau_probe_eta,  'tau_probe_eta/D')
        tree.Branch("tau_probe_match_dR",  tau_probe_match_dR,  'tau_probe_match_dR/D')
        tree.Branch("tau_probe_hlt_eta",  tau_probe_hlt_eta,  'tau_probe_hlt_eta/D')
        tree.Branch("tau_probe_hlt_pt",  tau_probe_hlt_pt,  'tau_probe_hlt_pt/D')
    else:
        print "No valid selection"



    # Loop over events
    for counter, evt in enumerate(tree_in):

        """
        if counter%1000 == 0:
            print counter
            #break
        """
        if counter == 1000:
            break

        # Run number selection
        if (int(evt.run)<160431) | (int(evt.run)>178420): continue
        if ((int(evt.run) >= 165970) & (int(evt.run)<=166782)) | (int(evt.run)>=171050):
            trigger = "45"
        else:
            trigger = "40"

        # Store event info
        id[0] = evt.event
        run[0] = evt.run
        if trigger == "45":
            trig_type[0] = 1
        else:
            trig_type[0] = 0

        # Object selections
        muons = select_muons(evt)
        electrons = select_electrons(evt)
        taus = select_taus(evt)
        taus = clean_taus(taus, muons, electrons)
        tauHLTs = select_tauHLT(evt)
        jets = select_jets_hack(evt, isData = True, trigger = trigger)

        # Event selection
        if len(jets) < 4:
            continue

        # Count number of taus above 40 GeV
        n_taus[0] = len(taus)
        nt40 = 0
        if len(taus) > 0:
            for tau in taus:
                if tau.pt>40.:
                    nt40 += 1
        n_taus40[0] = nt40

        if type == 'jet':

            # Jet trigger
            mu, mu_v1to5 = mu_trigger(evt)
            for i, t in enumerate(mu):
                trigger_mu[i] = mu[i]
            for i, t in enumerate(mu_v1to5):
                trigger_mu_v1to5[i] = mu_v1to5[i]

            #ps = prescales(evt)
            if pass_jet_tag(jets):
                # Store probe jet
                jet_probe_pt[0] = jets[3].pt
                jet_probe_eta[0] = jets[3].eta
                jet_probe_pxHLT[0] = jets[3].pxHLT
                tree.Fill()

        elif type == 'tau':

            # Taus
            if len(taus) < 1:
                continue

            # Tau trigger
            if trigger == "45":
                filterStatus  = evt.HLTFilter_hltQuadJet45IsoPFTau45
                filterStatus20 = evt.HLTFilter_hltFilterPFTauTrack5TightIsoL1QuadJet20CentralPFTau45
                filterStatus28 = evt.HLTFilter_hltFilterPFTauTrack5TightIsoL1QuadJet28CentralPFTau45
                filterStatus2 = (filterStatus28 == 1) | (filterStatus20 == 1)
            else:
                filterStatus  = evt.HLTFilter_hltQuadJet40IsoPFTau40
                filterStatus2 = evt.HLTFilter_hltFilterPFTauTrack5TightIsoL1QuadJet20CentralPFTau40

            if pass_fourjet_tag(jets, taus[0], filterStatus):
                deltaRMinTrig, tauHLT_eta, tauHLT_pt = hlt_match_tau(taus[0], tauHLTs)
                tau_filterStatus1[0] = filterStatus
                tau_filterStatus2[0] = filterStatus2
                tau_probe_pt[0] = taus[0].pt
                tau_probe_eta[0] = taus[0].eta
                tau_probe_match_dR[0] = deltaRMinTrig
                tau_probe_hlt_eta[0] = tauHLT_eta
                tau_probe_hlt_pt[0] = tauHLT_pt
                tree.Fill()

        else:
            print "No valid selection"

    tree.Print()

    # write the tree into the output file and close the file
    file.Write()
    file.Close()


if __name__ == "__main__":

    if len(sys.argv) > 1:
        type = int(sys.argv[1])
    else:
        type = 1
    print "type =", type
    if type == 0:
        createTable(type="jet")
    else:
        createTable(type="tau")
