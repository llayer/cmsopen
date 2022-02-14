import numpy  as np
import ROOT

def fill_histo(f, name, var):

    tree = f.Get("tree")

    if "bce" in var:
        h = ROOT.TH1D(name, name, 10,0.,1.)
        h.Sumw2()
        for event in tree:
            h.Fill(tree.bce, tree.weight)
        print(type(h))

    else:
        h = ROOT.TH1D(name, name, 10,0.,10.)
        h.Sumw2()
        for event in tree:
            h.Fill(tree.inferno, tree.weight)

    print(h.Integral())

    return h

def inferno_to_harvester(path, outpath=".", syst = []):

    #Path(COMBINE_PATH).mkdir(parents=True, exist_ok=True)
    outfile = ROOT.TFile(outpath, "RECREATE")#file_path)
    #outfile = ROOT.TFile(outpath + "harvester_input.root", 'RECREATE')

    for var in ["bce", "inferno"]:

        outfile.mkdir(var)
        outfile.cd(var)
        f = ROOT.TFile(path + "Data" + ".root")
        h = fill_histo(f, "Data", var)
        outfile.cd(var)
        h.Write('data_obs')


        for sample in [ "QCD", "TTJets_bkg", "WZJets", "STJets", "TTJets_signal"]:

            f = ROOT.TFile(path + sample + ".root")
            h = fill_histo(f, sample, var)
            outfile.cd(var)
            h.Write(sample)

            if sample == "TTJets_signal":

                for c in syst:#, "met"]:
                    if "jes" in c:
                        name = "jes"
                    else:
                        name = c

                    for ud, cap_ud in zip(["up", "down"], ["Up", "Down"]):

                        f = ROOT.TFile(path + sample + "_" + str(c) + "_" + ud + ".root")
                        h = fill_histo(f, sample + "_" + name + cap_ud , var)
                        outfile.cd(var)
                        h.Write(sample + '_' + name + cap_ud)

    print(outfile.ls())
    outfile.Close()

if __name__ == '__main__':

    path = "/eos/user/l/llayer/cmsopen/columnar/note_v0/input/inferno_cmsopen14/root_trees/"
    inferno_to_harvester(path, syst=["06_jes"])
