import ROOT
import root_numpy
import numpy as np
from coffea.btag_tools import BTagScaleFactor


def ratio(h2_denom, h2_nom):

    denom = root_numpy.hist2array(h2_denom, return_edges=True)
    num = root_numpy.hist2array(h2_nom, return_edges=True)
    denom = np.nan_to_num(denom)
    num = np.nan_to_num(num)
    eff = np.divide(num[0], denom[0])
    eff = np.nan_to_num(eff)
    xedges, yedges = denom[1][0], denom[1][1]

    return eff, xedges, yedges

f_beff = ROOT.TFile("data/beff_test.root")
effs = {}
effs['b'] = f_beff.Get("b")
effs['c'] = f_beff.Get("c")
effs['usdg'] = f_beff.Get("usdg")


"""
def load_eff_maps(filename):

    f = ROOT.TFile(filename)
    h2_BTaggingEff_Denom_b = f.Get("h2_BTaggingEff_Denom_b")
    h2_BTaggingEff_Num_b = f.Get("h2_BTaggingEff_Num_b")
    h2_BTaggingEff_Denom_c = f.Get("h2_BTaggingEff_Denom_c")
    h2_BTaggingEff_Num_c = f.Get("h2_BTaggingEff_Num_c")
    h2_BTaggingEff_Denom_usdg = f.Get("h2_BTaggingEff_Denom_udsg")
    h2_BTaggingEff_Num_usdg = f.Get("h2_BTaggingEff_Num_udsg")

    effs = {}
    effs['b'], xedges, yedges = ratio(h2_BTaggingEff_Denom_b, h2_BTaggingEff_Num_b)
    effs['c'], xedges, yedges = ratio(h2_BTaggingEff_Denom_c, h2_BTaggingEff_Num_c)
    effs['usdg'], xedges, yedges = ratio(h2_BTaggingEff_Denom_usdg, h2_BTaggingEff_Num_usdg)

    return effs, xedges, yedges

effs, xedges, yedges = load_eff_maps('data/beff.root')
"""


btag_sf = BTagScaleFactor("data/CSV.csv", "medium", methods='comb,comb,comb')

def eval_sf_eff(ev):

    pt = ev["Jet_pt"]
    eta = ev["Jet_eta"]
    flavour = ev["Jet_flavour"]
    
    eff = []
    sf = []
    sf_up = []
    sf_down = []
    
    for i in range(len(pt)):
    
        if pt[i] > 1000:
            pt_clipped = 980.
        else:
            pt_clipped = pt[i]

        if abs(flavour[i])==5:
            flav='b'
            flav_id = 5
        elif abs(flavour[i])==4:
            flav='c'
            flav_id = 4
        else:
            flav='usdg'
            flav_id = 0
        
        """
        ind_pt = np.digitize(pt_clipped, xedges)
        ind_eta = np.digitize(eta[i], yedges)
        """
        
        try:
            #eff.append( effs[flav][ind_pt, ind_eta] )
            eff.append( effs[flav].GetBinContent( effs[flav].GetXaxis().FindBin(pt_clipped),
                                                  effs[flav].GetYaxis().FindBin(eta[i])) )
            sf.append(btag_sf.eval("central", flav_id, abs(eta[i]), pt_clipped, ignore_missing=True))
            sf_up.append(btag_sf.eval("up", flav_id, abs(eta[i]), pt_clipped, ignore_missing=True))
            sf_down.append(btag_sf.eval("down", flav_id, abs(eta[i]), pt_clipped, ignore_missing=True))
        except:
            print( "Error in calculating the b-tagging SF:", pt_clipped, eta[i], flavour[i] )
            
    return {"Jet_btagSF" : sf, "Jet_btagSF_up" : sf_up, "Jet_btagSF_down" : sf_down, "Jet_beff" : eff}

"""
def eval_btagSF(ev):
    
    # !! CHECK FLAVOUR
    pt = ev["Jet_pt"]
    eta = ev["Jet_eta"]
    flavour = ev["Jet_flavour"]
    
    sf = btag_sf.eval("central", flavour, eta, pt, ignore_missing=True)
    
    return sf
"""


# Corresponds to https://twiki.cern.ch/twiki/bin/viewauth/CMS/BTagSFMethods 1A
def b_weight_method1(ev, syst='cent', njets = -1):

    jet_eff = ev["Jet_beff"]
    if syst == 'cent':
        jet_btagSF = ev["Jet_btagSF"]
    elif syst == 'up':
        jet_btagSF = ev["Jet_btagSF_up"]
    elif syst == 'down':
        jet_btagSF = ev["Jet_btagSF_down"]
    else:
        print("No valid b-tag variation!!")
    jet_csvDisc = ev["Jet_csvDisc"]
    
    pMC=1.0
    pData=1.0
    weight=1.0

    iJet = 0
    for eff, csvDisc, sf in zip(jet_eff, jet_csvDisc, jet_btagSF):
        
        if iJet == njets:
            break

        if csvDisc > 0.679:
            pMC=pMC*eff
            pData=pData*(sf*eff)

        else:
            pMC=pMC*(1.0 - eff)
            pData=pData*(1.0 - sf*eff)

        iJet += 1

    if pMC != 0.0:
        weight = pData/pMC

    return weight


# Corresponds to https://twiki.cern.ch/twiki/bin/viewauth/CMS/BTagSFMethods 1B
def b_weight_method2(ev, syst='cent', njets = -1):

    
    jet_eff = ev["Jet_beff"]
    if syst == 'cent':
        jet_btagSF = ev["Jet_btagSF"]
    elif syst == 'up':
        jet_btagSF = ev["Jet_btagSF_up"]
    elif syst == 'down':
        jet_btagSF = ev["Jet_btagSF_down"]
    else:
        print("No valid b-tag variation!!")
    jet_csvDisc = ev["Jet_csvDisc"]
    
    weight_0tags = 1.0

    iJet = 0
    for eff, csvDisc, sf in zip(jet_eff, jet_csvDisc, jet_btagSF):

        if iJet == njets:
            break
        try:
            weight_0tags = weight_0tags * (1 - sf * eff)
        except:
            print("sf", sf, "eff", eff)

        iJet += 1

    weight_at_least_1tag = 1.0 - weight_0tags

    return weight_at_least_1tag


def count_btags(jet_csvDisc, njets = -1):
    
    if njets < 0:
        return sum(jet_csvDisc>0.679)
    else:
        return sum(jet_csvDisc[0:njets] > 0.679)


def at_least_1tag(df):
    return df[df["Jet_nbtags"]>0]

def no_tag(df):
    return df[df["Jet_nbtags"]<1]