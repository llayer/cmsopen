import ROOT
import root_numpy
import numpy as np

# Load the btag reader
ROOT.gSystem.Load('libCondFormatsBTauObjects')
ROOT.gSystem.Load('libCondToolsBTau')

# get the sf data loaded
calib = ROOT.BTagCalibration('csv', '/afs/cern.ch/work/l/llayer/CMSSW_10_2_18/src/CSV.csv')
# making a std::vector<std::string>> in python is a bit awkward,
# but works with root (needed to load other sys types):
v_sys = getattr(ROOT, 'vector<string>')()
v_sys.push_back('up')
v_sys.push_back('down')

# make a reader instance and load the sf data
reader = ROOT.BTagCalibrationReader(
    1,              # 0 is for loose op, 1: medium, 2: tight, 3: discr. reshaping
    "central",      # central systematic type
    v_sys,          # vector of other sys. types
)
reader.load(
    calib,
    0,          # 0 is for b flavour, 1: FLAV_C, 2: FLAV_UDSG
    "comb"      # measurement type
)


def eval_btagSF(flavour, eta, pt):

    if abs(flavour)==5:
        flav=0
    elif abs(flavour)==4:
        flav=1
    else:
        flav=2

    #print flavour, abs(eta), pt
    btagSF = reader.eval_auto_bounds('central',       # systematic (here also 'up'/'down' possible)
                                      flav,    # jet flavor
                                      abs(eta),   # absolute value of eta
                                      pt          # pt
                                    )
    #print "I'm a btag SF", btagSF
    return btagSF


def ratio(h2_denom, h2_nom):

    denom = root_numpy.hist2array(h2_denom, return_edges=True)
    num = root_numpy.hist2array(h2_nom, return_edges=True)
    eff = num[0] / denom[0]
    xedges, yedges = denom[1][0], denom[1][1]

    return eff, xedges, yedges


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

effs, xedges, yedges = load_eff_maps('/eos/user/l/llayer/opendata_files/flat/TTJets.root')


def get_eff(eta, pt, flavour):

    if pt > 1000:
        pt = 980.

    if abs(flavour)==5:
        flav='b'
    elif abs(flavour)==4:
        flav='c'
    else:
        flav='usdg'

    ind_pt = np.digitize(pt, xedges)
    ind_eta = np.digitize(eta, yedges)

    try:
        return effs[flav][ind_pt, ind_eta]
    except:
        print pt, eta, flavour


# Corresponds to https://twiki.cern.ch/twiki/bin/viewauth/CMS/BTagSFMethods 1A
def b_weight_method1(jet_pt, jet_eta, jet_flavour, jet_csvDisc, jet_btagSF):

    pMC=1.0
    pData=1.0
    weight=1.0

    iJet = 0
    for pt, eta, flavour, csvDisc, sf in zip(jet_pt, jet_eta, jet_flavour, jet_csvDisc, jet_btagSF):

        if iJet == 4:
            break
        eff = get_eff(eta, pt, flavour)

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
def b_weight_method2(jet_pt, jet_eta, jet_flavour, jet_csvDisc, jet_btagSF):

    weight_0tags = 1.0

    iJet = 0
    for pt, eta, flavour, csvDisc, sf in zip(jet_pt, jet_eta, jet_flavour, jet_csvDisc, jet_btagSF):

        if iJet == 4:
            break
        eff = get_eff(eta, pt, flavour)
        weight_0tags = weight_0tags * (1 - sf * eff)

        iJet += 1

    weight_at_least_1tag = 1.0 - weight_0tags

    return weight_at_least_1tag
