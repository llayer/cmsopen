import ROOT

path = "/eos/user/l/llayer/cmsopen/study_inferno/results/combine/inferno_cmsopen12/qcd_fixed/DataCard/bce/tt/taujets_bce_bce_7TeV.root"
f = ROOT.TFile(path)
w = f.Get("w")
w.Print()
