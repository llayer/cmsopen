import ROOT

# Load the btag reader
ROOT.gSystem.Load('libCondFormatsBTauObjects')
ROOT.gSystem.Load('libCondToolsBTau')

# get the sf data loaded
calib = ROOT.BTagCalibration('csv', 'CSV.csv')
# making a std::vector<std::string>> in python is a bit awkward,
# but works with root (needed to load other sys types):
v_sys = getattr(ROOT, 'vector<string>')()
v_sys.push_back('up')
v_sys.push_back('down')

# make a reader instance and load the sf data
reader = ROOT.BTagCalibrationReader(
    0,              # 0 is for loose op, 1: medium, 2: tight, 3: discr. reshaping
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
