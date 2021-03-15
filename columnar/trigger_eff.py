import ROOT
import pandas as pd
import numpy as np
import root_pandas
from root_numpy import fill_hist
import selection


def selection_jet():

    runA_40 = selection.jet_trigger_selection("/eos/user/l/llayer/opendata_files/pretrigger_merged/Run2011A_SingleMu.root", 
                                              hlt40=True)
    runB_40 = selection.jet_trigger_selection("/eos/user/l/llayer/opendata_files/pretrigger_merged/Run2011B_SingleMu.root", 
                                              hlt40=True)
    runA_45 = selection.jet_trigger_selection("/eos/user/l/llayer/opendata_files/pretrigger_merged/Run2011A_SingleMu.root", 
                                              hlt40=False)
    runB_45 = selection.jet_trigger_selection("/eos/user/l/llayer/opendata_files/pretrigger_merged/Run2011B_SingleMu.root", 
                                              hlt40=False)
    runA_40.to_hdf("trigger/jet_RunA_40.h5" , "frame")
    runB_40.to_hdf("trigger/jet_RunB_40.h5" , "frame")
    runA_45.to_hdf("trigger/jet_RunA_45.h5" , "frame")
    runB_45.to_hdf("trigger/jet_RunB_45.h5" , "frame")

def selection_tau():
    
    runA_40 = selection.tau_trigger_selection("/eos/user/l/llayer/opendata_files/pretrigger_merged/Run2011A_MultiJet.root", 
                                              hlt40=True)
    runB_40 = selection.tau_trigger_selection("/eos/user/l/llayer/opendata_files/pretrigger_merged/Run2011B_MultiJet.root",
                                              hlt40=True)
    
    runA_45 = selection.tau_trigger_selection("/eos/user/l/llayer/opendata_files/pretrigger_merged/Run2011A_MultiJet.root", 
                                              hlt40=False)
    runB_45 = selection.tau_trigger_selection("/eos/user/l/llayer/opendata_files/pretrigger_merged/Run2011B_MultiJet.root",
                                              hlt40=False)
    
    runA_40.to_hdf("trigger/tau_RunA_40.h5" , "frame")
    runB_40.to_hdf("trigger/tau_RunB_40.h5" , "frame")
    runA_45.to_hdf("trigger/tau_RunA_45.h5" , "frame")
    runB_45.to_hdf("trigger/tau_RunB_45.h5" , "frame")


def trigger_info(df):
    
    trigger_names = ['HLT_Mu15_v','HLT_Mu20_v', 'HLT_Mu24_v', 'HLT_Mu30_v']
    trigger_v_names = ['HLT_Mu15_v1', 'HLT_Mu15_v2',
       'HLT_Mu15_v3', 'HLT_Mu15_v4', 'HLT_Mu15_v5', 'HLT_Mu20_v1',
       'HLT_Mu20_v2', 'HLT_Mu20_v3', 'HLT_Mu20_v4', 'HLT_Mu20_v5',
       'HLT_Mu24_v1', 'HLT_Mu24_v2', 'HLT_Mu24_v3', 'HLT_Mu24_v4',
       'HLT_Mu24_v5', 'HLT_Mu30_v1', 'HLT_Mu30_v2', 'HLT_Mu30_v3',
       'HLT_Mu30_v4', 'HLT_Mu30_v5']
    
    df["mu_fired"] = 0
    df["mu_v1to5_fired"] = 0
    
    for t in trigger_names:
        df["mu_fired"] += df[t]
        
    for t in trigger_v_names:
        df["mu_v1to5_fired"] += df[t]  
        
        
def load_jet_sel():
    
    runA_40 = pd.read_hdf("trigger/jet_RunA_40.h5" , "frame")
    runB_40 = pd.read_hdf("trigger/jet_RunB_40.h5" , "frame")
    runA_45 = pd.read_hdf("trigger/jet_RunA_45.h5" , "frame")
    runB_45 = pd.read_hdf("trigger/jet_RunB_45.h5" , "frame")
    
    jet_40 = pd.concat([runA_40, runB_40], axis=0)
    trigger_info(jet_40)
    jet_45 = pd.concat([runA_45, runB_45], axis=0)
    trigger_info(jet_45)
    
    return jet_40, jet_45


def load_tau_sel():
    
    runA_40 = pd.read_hdf("trigger/tau_RunA_40.h5" , "frame")
    runB_40 = pd.read_hdf("trigger/tau_RunB_40.h5" , "frame")
    runA_45 = pd.read_hdf("trigger/tau_RunA_45.h5" , "frame")
    runB_45 = pd.read_hdf("trigger/tau_RunB_45.h5" , "frame")
    
    tau_40 = pd.concat([runA_40, runB_40], axis=0)
    tau_45 = pd.concat([runA_45, runB_45], axis=0)
    
    return tau_40, tau_45


def eff_jet(jet, fired=0, name = "40"):

    if fired == 0:
        probe = jet[jet['mu_fired'] > 0]
    elif fired == 1:
        probe = jet[jet['mu_v1to5_fired'] > 0]
    else:
        probe = jet

    tag = probe[probe['pxHLT_tag'] != -9999]

    #denom, edges = np.histogram(trigger['jet_probe_pt'].values, bins=19, range=(10,200))
    #nom, edges = np.histogram(tag_trigger['jet_probe_pt'].values, bins=19, range=(10,200))

    denom = ROOT.TH1F("jet4_denom_" + name,"jet4_denom", 19,10,200)
    denom.Sumw2()
    fill_hist(denom, probe['pt_tag'].values)

    nom = ROOT.TH1F("jet4_nom_" + name,"jet4_nom", 19,10,200)
    nom.Sumw2()
    fill_hist(nom, tag['pt_tag'].values)

    eff = ROOT.TH1F("jet4_eff_" + name,"jet-leg efficiency vs pT", 19,10,200);
    eff.Sumw2()
    eff.Divide(nom,denom,1,1,"B")

    return eff


def eff_tau(tau, deltaR = 0.4, name = "40"):   #CHECK delta r

    print(len(tau))
    
    if name == "40":
        mask_filter = tau['HLTFilter_hltFilterPFTauTrack5TightIsoL1QuadJet20CentralPFTau40'] == 1
    else:
        mask20 = tau['HLTFilter_hltFilterPFTauTrack5TightIsoL1QuadJet20CentralPFTau45'] > 0
        mask28 = tau['HLTFilter_hltFilterPFTauTrack5TightIsoL1QuadJet28CentralPFTau45'] > 0
        mask_filter = mask20 | mask28
    
    
    mask_hlt_eta = abs(tau["tau_hlt_eta"]) < 2.3
    mask_hlt_deltaR = tau['hlt_dR'] < deltaR
    print ("hlt_dR", len(tau[mask_hlt_deltaR]))
    print ("Filter2", len(tau[mask_filter & mask_hlt_deltaR]))

    tag = tau[mask_filter & mask_hlt_deltaR & mask_hlt_eta]

    print( "taueta", len(tag) )
    
    #denom, edges = np.histogram(trigger['tau_probe_pt'].values, bins=15, range=(0,90))
    #nom, edges = np.histogram(tag_trigger['tau_probe_pt'].values, bins=15, range=(0,90))

    #bins = np.array( [0,5,10,15,20,25,30,35,40,45,50,60,70,80,90] )
    #bins = np.array([0,5,10,15,20,25,30,32.5,35,37.5,40,42.5,45,47.5,50,60,70,80,90])
    bins = np.array([0,5,10,15,20,25,30.5,35,40,45,50,60,70,80,90,100])


    denom = ROOT.TH1F("tau_denom_"  + name,"jet4_denom", 15, bins)
    denom.Sumw2()
    fill_hist(denom, tau['tau_pt'].values)

    nom = ROOT.TH1F("tau_nom_" + name,"tau_nom", 15, bins)
    nom.Sumw2()
    fill_hist(nom, tag['tau_pt'].values)

    eff = ROOT.TH1F("tau_eff_" + name,"tau-leg efficiency vs pT", 15, bins)
    eff.Sumw2()
    eff.Divide(nom,denom,1,1,"B")

    return eff


def eff():

    # Trigger efficiency for jets
    print( "Jet efficiencies" )
    jet_40, jet_45 = load_jet_sel()
    jet_eff_40 = eff_jet(jet_40, fired = 1, name = "40")
    jet_eff_45 = eff_jet(jet_45, fired = 1, name = "45")

    # Trigger efficiency for tau
    print( "Tau efficiencies" )
    tau_40, tau_45 = load_tau_sel()
    tau_eff_40 = eff_tau(tau_40, deltaR = 0.4, name = "40")
    tau_eff_45 = eff_tau(tau_45, deltaR = 0.4, name = "45")

    
    print( "Write output" )
    file = ROOT.TFile("trigger_eff.root", 'recreate')
    jet_eff_40.Write()
    jet_eff_45.Write()
    tau_eff_40.Write()
    tau_eff_45.Write()
    file.Close()
    
    
    
    
    

if __name__ == "__main__":
    
    #print( "Jet trigger event selection" )
    #selection_jet()
    print( "Tau trigger event selection" )
    selection_tau()
    #spass
    #eff()
