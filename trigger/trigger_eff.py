import ROOT
import pandas as pd
import numpy as np
import root_pandas
from root_numpy import fill_hist

def runs_40(run_number):

    if ( run_number < 165970 ) | ((run_number > 166782) & (run_number<171050)):
        return True
    else:
        return False

def runs_45(run_number):

    if ((run_number>=165970) & (run_number<=166782)) | ((run_number>=171050) & (run_number<178421)):
        return True
    else:
        return False

def restrict_runs(data):

    data["40"] = data.apply(lambda x: runs_40(x.run_number), axis=1)
    data["45"] = data.apply(lambda x: runs_45(x.run_number), axis=1)
    #return data[ (data["40"] == True) | (data["45"] == True) ]


def mu_fired(trigger_mu):
    if sum(trigger_mu) > 0:
        return True
    else:
        return False

def mu_v1to5_fired(trigger_mu_v1to5):
    if sum(trigger_mu_v1to5) > 0:
        return True
    else:
        return False


def eff_jet(trigger_mu, trig_type = 0, fired=0):

    trigger = trigger_mu[trigger_mu['trig_type']==trig_type]
    if fired == 0:
        trigger = trigger[trigger['mu_fired']]
    elif fired == 1:
        trigger = trigger[trigger['mu_v1to5_fired']]
    else:
        pass

    tag_trigger = trigger[trigger['jet_probe_pxHLT'] != -9999]

    #denom, edges = np.histogram(trigger['jet_probe_pt'].values, bins=19, range=(10,200))
    #nom, edges = np.histogram(tag_trigger['jet_probe_pt'].values, bins=19, range=(10,200))

    if trig_type == 0:
        name = "40"
    else:
        name = "45"

    denom = ROOT.TH1F("jet4_denom_" + name,"jet4_denom", 19,10,200)
    denom.Sumw2()
    fill_hist(denom, trigger['jet_probe_pt'].values)

    nom = ROOT.TH1F("jet4_nom_" + name,"jet4_nom", 19,10,200)
    nom.Sumw2()
    fill_hist(nom, tag_trigger['jet_probe_pt'].values)

    eff = ROOT.TH1F("jet4_eff_" + name,"jet-leg efficiency vs pT", 19,10,200);
    eff.Sumw2()
    eff.Divide(nom,denom,1,1,"B")

    return eff


def eff_tau(trigger_multi, trig_type = 0):

    trigger = trigger_multi[trigger_multi['trig_type']==trig_type]


    tag_trigger = trigger[(trigger['tau_filterStatus2'] ==1) & (trigger['tau_probe_hlt_eta'] < 2.3) & \
                          (trigger['tau_probe_match_dR'] < 0.05)]

    #denom, edges = np.histogram(trigger['tau_probe_pt'].values, bins=15, range=(0,90))
    #nom, edges = np.histogram(tag_trigger['tau_probe_pt'].values, bins=15, range=(0,90))

    if trig_type == 0:
        name = "40"
    else:
        name = "45"

    bins = np.array([0,5,10,15,20,25,30,32.5,35,37.5,40,42.5,45,47.5,50,60,70,80,90])

    denom = ROOT.TH1F("tau_denom_"  + name,"jet4_denom", 18, bins)
    denom.Sumw2()
    fill_hist(denom, trigger['tau_probe_pt'].values)

    nom = ROOT.TH1F("tau_nom_" + name,"tau_nom", 18, bins)
    nom.Sumw2()
    fill_hist(nom, tag_trigger['tau_probe_pt'].values)

    eff = ROOT.TH1F("tau_eff_" + name,"tau-leg efficiency vs pT", 18, bins)
    eff.Sumw2()
    eff.Divide(nom,denom,1,1,"B")

    return eff


def eff():

    # Trigger efficiency for jets
    print "Jet efficiencies"
    trigger_muA = root_pandas.read_root( "trigger_files/Run2011A_SingleMu.root", "Events")
    trigger_muB = root_pandas.read_root( "trigger_files/Run2011B_SingleMu.root", "Events")
    trigger_mu = pd.concat([trigger_muA, trigger_muB], axis=0)
    trigger_mu['mu_fired'] = trigger_mu['trigger_mu'].apply(mu_fired)
    trigger_mu['mu_v1to5_fired'] = trigger_mu['trigger_mu_v1to5'].apply(mu_v1to5_fired)

    jet_eff_40 = eff_jet(trigger_mu, trig_type = 0, fired = 1)
    jet_eff_45 = eff_jet(trigger_mu, trig_type = 1, fired = 1)

    # Trigger efficiency for tau
    print "Tau efficiencies"
    trigger_multijetA = root_pandas.read_root( "trigger_files/Run2011A_MultiJet.root", "Events")
    trigger_multijetB = root_pandas.read_root( "trigger_files/Run2011B_MultiJet.root", "Events")
    trigger_multijet = pd.concat([trigger_multijetA, trigger_multijetB], axis=0)

    tau_eff_40 = eff_tau(trigger_multijet, trig_type = 0)
    tau_eff_45 = eff_tau(trigger_multijet, trig_type = 1)
    #restrict_runs(trigger_mu)

    print "Write output"
    file = ROOT.TFile("trigger_eff.root", 'recreate')
    jet_eff_40.Write()
    jet_eff_45.Write()
    tau_eff_40.Write()
    tau_eff_45.Write()
    file.Close()

if __name__ == "__main__":
    eff()
