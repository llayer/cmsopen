import ROOT
import sys



def pass_tau_iso(evt, iTau):

    if (evt.Tau_byLooseCombinedIsolationDeltaBetaCorr[iTau] == 1):
        return True
    if (evt.Tau_byLooseCombinedIsolationDeltaBetaCorr3Hits[iTau] == 1):
        return True
    #if (evt.Tau_byLooseIsolationMVA2[iTau] == 1):
    #    return True

    return False

def pass_against_ele(evt, iTau):

    if evt.Tau_againstElectronLoose[iTau] == 1:
        return True
    if evt.Tau_againstElectronLooseMVA3[iTau] == 1:
        return True
    return False

def pass_against_mu(evt, iTau):

    if evt.Tau_againstMuonLoose[iTau] == 1:
        return True
    if evt.Tau_againstMuonLoose3[iTau] == 1:
        return True
    return False


def pass_pre_sel(evt, isData, pt_cut = 40.):

    if isData:
        if not ((evt.HLT_QuadJet40_IsoPFTau40 == 1) | (evt.HLT_QuadJet45_IsoPFTau45 == 1)):
            return False

    njets = 0
    for pt in evt.Jet_pt:
        if pt > pt_cut:
            njets += 1
    if not ( njets>=3 ):
        return False

    ntaus = 0
    for iTau in range(evt.nTau):
        if evt.Tau_pt[iTau] > pt_cut:
            if pass_tau_iso(evt, iTau):
                if pass_against_mu(evt, iTau):
                    if pass_against_ele(evt, iTau):
                        ntaus += 1
    if not ( ntaus>=1 ):
        return False

    return True



def preselect(isData):

    ff = ROOT.TFile( '/eos/user/l/llayer/opendata_files/legacy_id/Run2011A_MultiJet/Run2011A_MultiJet_564CBC7A-C143-E311-895A-C860001BD936_348.root')  #ff = ROOT.TFile( '/eos/user/l/llayer/opendata_files/legacy/Run2011A_MultiJet/Run2011A_MultiJet_56D66460-C743-E311-B818-BCAEC518FF62_3089.root') # ff = ROOT.TFile( '/eos/user/l/llayer/opendata_files/legacy/DYJetsToLL/DYJetsToLL_AA11C768-AEBC-E311-B184-0025905A60BE_4018.root')
    #'/eos/user/l/llayer/opendata_files/legacy/Run2011A_MultiJet/Run2011A_MultiJet_56D66460-C743-E311-B818-BCAEC518FF62_3089.root')
    tree_in = ff.Get("MyModule/Events")
    tree_info = ff.Get("MyModule/Info")

    file = ROOT.TFile("presel.root", 'recreate')
    newtree = tree_in.CloneTree(0)

    for counter, evt in enumerate(tree_in):

        if not pass_pre_sel(evt, isData):
            continue
        else:
            newtree.Fill()

    newtree.Print()
    print tree_in.GetEntriesFast(), newtree.GetEntriesFast()
    newtree.Write()
    tree_info.Write()
    file.Close()

if __name__ == "__main__":

    if len(sys.argv) > 1:
        isData = sys.argv[1]
    else:
        isData = 1
    print "isData =", isData

    preselect(isData = int(isData))
