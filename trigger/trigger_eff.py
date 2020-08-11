import ROOT
from collections import namedtuple

# Object selections - will be moved to a separate file later

def reliso03(TrkIso03, ECaloIso03, HCaloIso03, pt):
    return (TrkIso03+ECaloIso03+HCaloIso03) / pt


def select_muons(evt, eta_cut = 2.5, pt_cut = 10., reliso_cut = 0.15, vtxmatch_cut = 1.):

    Muon = namedtuple('Muon', 'pt px py pz e')
    good_muons = []

    for iMu in range(evt.nMuon):

        """
        //if((muon.MuonType-(muon.MuonType/10)*10) ==0) continue; //isTrackerMuon
        //if(muon.Chi2           >=MuonNormChi2_)         continue;
        //if(muon.NTrValidHits   <=MuonNofValidTrHits_)   continue;
        //if(muon.NValidHits     <=MuonNofValidHits_  )   continue;
        //if(fabs(muon.D0Inner)  >=MuonD0Cut_)            continue;
        """

        # Global muon
        if evt.Muon_isGlobalMuon[iMu] == False: continue
        # Eta cut
        if abs(evt.Muon_eta[iMu]) > eta_cut: continue
        # Pt cut
        if abs(evt.Muon_pt[iMu]) < pt_cut: continue
        # Isolation
        iso = reliso03(evt.Muon_TrkIso03[iMu], evt.Muon_ECaloIso03[iMu],
                       evt.Muon_HCaloIso03[iMu], evt.Muon_pt[iMu])
        if iso > reliso_cut: continue
        if abs(evt.Muon_z - evt.PV_z) > vtxmatch_cut: continue

        good_muons.append(Muon( evt.Muon_pt[iMu], evt.Muon_px[iMu], evt.Muon_py[iMu],
                                evt.Muon_pz[iMu], evt.Muon_e[iMu] ))

    return good_muons


def select_electrons(evt, eta_cut = 2.5, pt_cut = 10., reliso_cut = 0.15, vtxmatch_cut = 1.):

    Electron = namedtuple('Electron', 'pt px py pz e')
    good_electrons = []

    for iEl in range(evt.nElectron):

        """
        bool hadId(electron.elecIdWP90_c & 0x1);
        if(!hadId) continue;
        """
        # Eta cut
        if abs(evt.Electron_eta[iEl]) > eta_cut: continue
        # Pt cut
        if abs(evt.Electron_pt[iEl]) < pt_cut: continue
        # Isolation
        iso = reliso03(evt.Electron_TrkIso03[iEl], evt.Electron_ECaloIso03[iEl],
                       evt.Electron_HCaloIso03[iEl], evt.Electron_pt[iEl])
        if iso > reliso_cut: continue
        if abs(evt.Electron_z[iEl] - evt.PV_z) > vtxmatch_cut: continue

        good_electrons.append(Electron( evt.Electron_pt[iEl], evt.Electron_px[iEl], evt.Electron_py[iEl],
                                        evt.Electron_pz[iEl], evt.Electron_e[iEl] ))

    return good_electrons


def select_taus(evt, eta_cut = 2.3, pt_cut = 10., vtxmatch_cut = 1., d0_cut=0.04):

    Tau = namedtuple('Tau', 'pt px py pz e')
    good_taus = []

    """
    if ( tau.leadTrackPt    <= TauLeadTrkPtCut_ ) continue
    """

    for iTau in range(evt.nTau):

        if evt.Tau_byMediumCombinedIsolationDeltaBetaCorr[iTau] == False: continue
        if evt.Tau_againstMuonTight[iTau] == False: continue
        if evt.Tau_againstElectronTight[iTau] == False: continue

        # Eta
        # if(fabs(tau.p4.Eta())<1.566 && fabs(tau.p4.Eta())>1.4442)continue;
        if abs(evt.Tau_eta[iTau]) > eta_cut: continue
        # Pt cut
        if abs(evt.Tau_pt[iTau]) < pt_cut: continue
        # Vtx cut
        if abs(evt.Tau_z[iTau] - evt.PV_z) > vtxmatch_cut: continue
        #if abs( evt.Tau_D0 )  >= d0_cut:
        #    continue

        good_taus.append(Tau( evt.Tau_pt[iTau], evt.Tau_px[iTau], evt.Tau_py[iTau],
                            evt.Tau_pz[iTau], evt.Tau_e[iTau] ))

    return good_taus


def select_jets(evt, eta_cut = 2.5, pt_cut = 10., trigger="45"):

    Jet = namedtuple('Jet', 'pt px py pz e pxHLT pyHLT pzHLT eHLT')
    good_jets = []

    for iJet in range(evt.nJet):

        # Eta cut
        if abs(evt.Jet_eta[iJet]) > eta_cut: continue
        # Pt cut
        if abs(evt.Jet_pt[iJet]) < pt_cut: continue

        if trigger == "45":
            good_jets.append(Jet( evt.Jet_pt[iJet], evt.Jet_px[iJet], evt.Jet_py[iJet],
                                  evt.Jet_pz[iJet], evt.Jet_e[iJet], evt.Jet_pxHLT45[iJet],
                                  evt.Jet_pyHLT45[iJet], evt.Jet_pzHLT45[iJet], evt.Jet_eHLT45[iJet]))
        else:
            good_jets.append(Jet( evt.Jet_pt[iJet], evt.Jet_px[iJet], evt.Jet_py[iJet],
                          evt.Jet_pz[iJet], evt.Jet_e[iJet], evt.Jet_pxHLT40[iJet],
                          evt.Jet_pyHLT40[iJet], evt.Jet_pzHLT40[iJet], evt.Jet_eHLT40[iJet]))
        #print "HLT", evt.Jet_pxHLT40[iJet], evt.Jet_pxHLT45[iJet]

    return good_jets


def clean_taus(taus, muons, electrons, R=0.4):

    clean_taus = []
    for tau in taus:

        deltaRmu = 10000
        deltaRel = 10000

        tau_4vec = ROOT.TLorentzVector(tau.px, tau.py, tau.pz, tau.e)

        for mu in muons:

            mu_4vec = ROOT.TLorentzVector(mu.px, mu.py, mu.pz, mu.e)
            deltaR = tau_4vec.DeltaR(mu_4vec)
            if(deltaR < deltaRmu):
                deltaRmu = deltaR

        for el in electrons:

            el_4vec = ROOT.TLorentzVector(el.px, el.py, el.pz, el.e)
            deltaR = tau_4vec.DeltaR(el_4vec)
            if(deltaR < deltaRel):
                deltaRel = deltaR


        if ( deltaRmu > R)  & (deltaRel > R):
            clean_taus.append(tau)

    return clean_taus


def trigger_jets(jets):


    pass_tag_cond = []
    pass_probe_cond = False
    pass_probe_match = False
    probe_jet_pt = -9999
    # Loop over jets
    for idx, jet in enumerate(jets):

       # print jet.pt, jet.pxHLT

        if idx == 4:
            break
        elif idx == 3:
            if (jet.pt > 10.): # && eta
                pass_probe_cond = True
                probe_jet_pt = jet.pt
                if jet.pxHLT != -9999:
                    pass_probe_match = True

        else:
            if (jet.pt > 70.) & (jet.pxHLT != -9999):
                pass_tag_cond.append(True)
            else:
                pass_tag_cond.append(False)

    #print "Tag", pass_tag_cond
    #print "Probe", pass_probe_cond
    #print "Match", pass_probe_match

    if (sum(pass_tag_cond) == 3) & (pass_probe_cond == True):
        pass_trigger = True
        #print "pass trigger"
    else:
        pass_trigger = False
        #print( "fail trigger")

    return pass_trigger, pass_probe_match, probe_jet_pt


ff = ROOT.TFile('/eos/user/l/llayer/opendata_files/Run2011A/Run2011A_1.root')
tree_in = ff.Get("MyModule/Events")

jet4_ref = ROOT.TH1F("jet4_ref","jet4_ref", 19,10,200)
jet4_trig = ROOT.TH1F("jet4_trig","jet4_trig", 19,10,200)


for counter, evt in enumerate(tree_in):

    if counter%1000 == 0:
        print counter
        #break

    # Trigger
    if ((int(evt.run) >= 165970) & (int(evt.run)<=166782)) | (int(evt.run)>=171050):
        trigger = "45"
    else:
        trigger = "40"

    passTrigger40 = evt.HLT_QuadJet40_IsoPFTau40
    passTrigger45 = evt.HLT_QuadJet45_IsoPFTau45

    # Object selections
    #muons = select_muons(evt)
    #electrons = select_electrons(evt)
    #taus = select_taus(evt)
    #taus = clean_taus(taus, muons, electrons)
    jets = select_jets(evt, trigger = trigger)

    """
    print
    print "Trigger 40", passTrigger40, "Trigger 45", passTrigger45
    print "Muons", len(muons)
    print "Electrons", len(electrons)
    print "Taus", len(taus)
    print "Jets", len(jets)
    """

    if len(jets) < 4:
        continue


    #print evt.run, trigger
    #for j in jets:
    #    print j.pxHLT, j.pxHLT != -9999



    # Jet trigger
    pass_trigger, pass_probe_match, probe_jet_pt = trigger_jets(jets)

    if pass_trigger:
        #print "Pt probe", probe_jet_pt
        jet4_ref.Fill(probe_jet_pt)
    if pass_trigger & pass_probe_match:
        #print "Pt probe & match", probe_jet_pt
        jet4_trig.Fill(probe_jet_pt)

jet4_eff = ROOT.TH1F("jet4_eff","jet-leg efficiency vs pT", 19,10,200)
jet4_eff.Sumw2()
jet4_eff.Divide(jet4_trig,jet4_ref,1,1,"B")

# Write to file
f = ROOT.TFile( trigger + "_trigger_eff.root", "recreate")
f.cd()
jet4_ref.Write()
jet4_trig.Write()
jet4_eff.Write()
f.Close()
