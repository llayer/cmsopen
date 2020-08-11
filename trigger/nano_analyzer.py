import ROOT
import numpy as np


ff = ROOT.TFile('/eos/user/l/llayer/opendata_files/Run2011A/Run2011A_1.root')
tree_in = ff.Get("MyModule/Events")

root_file = ROOT.TFile("flat.root", "RECREATE")

tree = ROOT.TTree("tree", "tutorial")
x = np.empty((1), dtype="float32")
y = np.empty((1), dtype="float32")
tree.Branch("x", x, "x/F")
tree.Branch("y", y, "y/F")  


for counter, e in enumerate(tree_in):

    if counter%10000 == 0:
        print counter
    x = counter
    y = counter
    tree.Fill()

tree.Write()
root_file.Close()


