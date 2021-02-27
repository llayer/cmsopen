import pandas as pd
import numpy as np
import ROOT

"""
f_jet40 = ROOT.TFile("rootFilesTurnOn/TriggerEffHisto_data_match40_JETLEG.root")
f_jet45 = ROOT.TFile("rootFilesTurnOn/TriggerEffHisto_data_match45_JETLEG.root")
f_tau40 = ROOT.TFile("rootFilesTurnOn/TriggerEffHisto_match40_newTauID.root")
f_tau45 = ROOT.TFile("rootFilesTurnOn/TriggerEffHisto_match45_newTauID.root")
h_eff_jet40 = f_jet40.Get("jet4_eff")
h_eff_jet45 = f_jet45.Get("jet4_eff")
h_eff_tau40 = f_tau40.Get("eff_tau")
h_eff_tau45 = f_tau45.Get("eff_tau")
"""



f = ROOT.TFile("data/trigger_eff.root")
h_eff_jet40 = f.Get("jet4_eff_40")
h_eff_jet45 = f.Get("jet4_eff_45")
h_eff_tau40 = f.Get("tau_eff_40")
h_eff_tau45 = f.Get("tau_eff_45")


def trigger_weight(ev, trigger_frac=0.218):

    jet_pt0 = ev["Jet_pt"][0]
    jet_pt1 = ev["Jet_pt"][1]
    jet_pt2 = ev["Jet_pt"][2]
    tau_pt = ev["Tau_pt"][0]
    
    trigRange = np.random.uniform(low=0.0, high=1.0)
    if trigRange < trigger_frac: #0.218: #0.208 new lumi
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


def lumi():

    # HLT_QuadJet45_IsoPFTau45 171050 178420
    hlt_45_171050_178420 = 2930.246598848
    # HLT_QuadJet45_IsoPFTau45 165970 166782
    hlt_45_165970_166782 = 363.488009033
    # HLT_QuadJet40_IsoPFTau40 160431 165969
    hlt_40_160431_165969 = 357.461657608
    # HLT_QuadJet40_IsoPFTau40 166783 171049
    hlt_40_166783_171049 = 514.717054398

    hlt_45 = hlt_45_171050_178420 + hlt_45_165970_166782
    hlt_40 = hlt_40_160431_165969 + hlt_40_166783_171049

    return hlt_40, hlt_45




def get_xsec(sample):
    
    xsec = {'WJetsToLNu': [31314, 1558, 1558],
        'TTJets': [164, 0., 0.], #[177.31, 0., 0.], #DANGER!! CHECK!!
        'DYJetsToLL': [3048, 132, 132],
        'T_TuneZ2_t-channel': [42.6, 2.4, 2.3],
        'Tbar_TuneZ2_t-channel': [22.0, 0.1, 0.8],
        'T_TuneZ2_s': [2.76, 0.11 ,0.10],
        'Tbar_TuneZ2_s': [1.52, 0.09, 0.08],
        'T_TuneZ2_tW': [7.87,  0.6,  0.6],
        'Tbar_TuneZ2_tW': [7.87,  0.6,  0.6]
    }
    return xsec[sample]


def norm(candidates, counts, xsec, lumi = 3.9):

    total_events = counts['nEventsTotal'].sum()
    norm = (xsec[0] * lumi * 1000) / total_events
    candidates["norm"] = norm
    
    
def classify_tt(ttjets):

    tt_tau = ttjets[ttjets["genEvent_tmeme"] == 10000]
    tt_ee = ttjets[(ttjets["genEvent_tmeme"] == 2) | (ttjets["genEvent_tmeme"] == 10101) | (ttjets["genEvent_tmeme"] == 20200)]
    tt_mm = ttjets[(ttjets["genEvent_tmeme"] == 20) | (ttjets["genEvent_tmeme"] == 11010) | (ttjets["genEvent_tmeme"] == 22000)]
    tt_em = ttjets[(ttjets["genEvent_tmeme"] == 11) | (ttjets["genEvent_tmeme"] == 11001) | (ttjets["genEvent_tmeme"] == 10110) | (ttjets["genEvent_tmeme"] == 21100)]
    tt_etauh = ttjets[(ttjets["genEvent_tmeme"] == 10010) | (ttjets["genEvent_tmeme"] == 21000)]
    tt_mtauh = ttjets[(ttjets["genEvent_tmeme"] == 10001) | (ttjets["genEvent_tmeme"] == 20100)]
    tt_lqq = ttjets[(ttjets["genEvent_tmeme"] == 1) | (ttjets["genEvent_tmeme"] == 10) | (ttjets["genEvent_tmeme"] == 10100) | (ttjets["genEvent_tmeme"] == 11000)]
    tt_tauhtauh = ttjets[(ttjets["genEvent_tmeme"] == 20000)]
    tt_qqqq = ttjets[(ttjets["genEvent_tmeme"] == 0)]
    tt_ll = ttjets[(ttjets["genEvent_tmeme"] == 2) | (ttjets["genEvent_tmeme"] == 10101) | (ttjets["genEvent_tmeme"] == 20200) | (ttjets["genEvent_tmeme"] == 20) | \
                (ttjets["genEvent_tmeme"] == 11010) | (ttjets["genEvent_tmeme"] == 22000) | (ttjets["genEvent_tmeme"] == 11) | (ttjets["genEvent_tmeme"] == 11001) |\
                (ttjets["genEvent_tmeme"] == 10110) | (ttjets["genEvent_tmeme"] == 21100)]
    tt_bkg = pd.concat([tt_mm, tt_em, tt_etauh, tt_mtauh, tt_lqq, tt_tauhtauh, tt_qqqq, tt_ll], axis=0)

    return tt_tau, tt_bkg


def met_cut(df):
    return df[df['met']>20.]



