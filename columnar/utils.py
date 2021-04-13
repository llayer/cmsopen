import ROOT
from coffea.lookup_tools import extractor
from root_numpy import array2hist, hist2array

data_trigger = ['Run2011A_MultiJet', 'Run2011B_MultiJet', 'Run2011A_SingleMu', 'Run2011B_SingleMu']
data = ['Run2011A_MultiJet', 'Run2011B_MultiJet']
mc = ['TTJets', 'WJetsToLNu', 'DYJetsToLL', 'T_TuneZ2_s', 'T_TuneZ2_tW', 'T_TuneZ2_t-channel',
       'Tbar_TuneZ2_s', 'Tbar_TuneZ2_tW', 'Tbar_TuneZ2_t-channel']

def to_rt(sample_name):

    """
    # Trigger
    print("Merge events")
    path = "/eos/user/l/llayer/opendata_files/pretrigger/" + sample_name + "/*.root"
    chain = ROOT.TChain("Events")
    chain.Add(path)
    chain.Merge("pretrigger_merged/" + sample_name + ".root")
    """
    
    
    """
    print("Merge events")
    path = "/eos/user/l/llayer/opendata_files/preselection/" + sample_name + "/*.root"
    chain = ROOT.TChain("Events")
    chain.Add(path)
    chain.Merge("preselection_merged/" + sample_name + ".root")

    """
    
    print("Merge counts")    
    nano_path = "/eos/user/l/llayer/opendata_files/legacy_id/" + sample_name + "/*.root"
    chain = ROOT.TChain("MyModule/Info")
    chain.Add(nano_path)
    chain.Merge("preselection_merged/" + sample_name + "_counts.root")
    
    """
    file_info = []
    for f in glob.glob(path):
        id = f.split("/")[-1].split("_")[1:5]
        nano_path = "/eos/user/l/llayer/opendata_files/legacy_id/" + sample_name + "/" \
                + id[0] + "_" + id[1] + "_" + id[2] + "_" + id[3] + ".root"  
        file = ROOT.TFile(nano_path)
        tree = file.Get("MyModule/Info")
        for e in tree:
            file_info.append({ "nEventsTotal": e.nEventsTotal,
                                "nEventsFiltered": e.nEventsFiltered ,
                                "file_name": id[0] + "_" + id[1] + "_" + id[2] + "_" + id[3] + ".root"
                             })

    counts = pd.DataFrame(file_info)
    counts.to_hdf("preselection_merged/" + sample_name + "_counts.h5", "Counts", mode="w")
    """

def to_harvester(file_path, outpath):
    
    f = ROOT.TFile(file_path)
    print(f.ls())
    
    outfile = ROOT.TFile(outpath, 'RECREATE')
    outfile.mkdir("signal_region")
    outfile.cd("signal_region")
    h = f.Get("Data_bdt")
    h.Write('data_obs')
    h = f.Get("QCD_bdt")
    h.Write('QCD')
    h = f.Get("QCD_mistag_up_bdt")
    h.Write('QCD_mistagUp')
    h = f.Get("QCD_mistag_down_bdt")
    h.Write('QCD_mistagDown')
    
    
    # Loop over the samples
    for sample in [ "TTJets_bkg", "WZJets", "STJets", "TTJets_signal"]:
        # Loop over systematics
        h = f.Get(sample + "_centJER_bdt")
        h.Write(sample)  
        for sys in ["jes", "jer", "tau_e", "met"]:
            for var in ["up", "down"]:
                print(sample + "_" + sys + "_" + var + "_bdt")
                if sys == "tau_e":
                    h = f.Get(sample + "_" + sys + var + "_bdt")
                else:
                    h = f.Get(sample + "_" + sys + "_" + var + "_bdt")
                print(type(h))
                if var == "up":
                    h.Write(sample + "_" + sys + "Up")
                if var == "down":
                    h.Write(sample + "_" + sys + "Down")
                    
                if sys == "jes":
                    for c_var in ["03", "06", "09", "old"]:
                        
                        print(sample + "_" + sys + "_" + var + "_" + c_var + "_bdt")
                        h = f.Get(sample + "_" + sys + "_" + var + "_" + c_var + "_bdt")
                        if var == "up":       
                            h.Write(sample + "_" + sys + "_" + c_var + "Up")
                        if var == "down":
                            h.Write(sample + "_" + sys + "_" + c_var + "Down")
                        
                if sys == "tau_e":
                    for c_var in ["05", "07", "09"]:
                        h = f.Get(sample + "_" + sys + var + "_" + c_var + "_bdt")
                        if var == "up":       
                            h.Write(sample + "_" + sys + "_" + c_var + "Up")
                        if var == "down":
                            h.Write(sample + "_" + sys + "_" + c_var + "Down")                
                        
                    
        for sys in ["btag", "trigger", "pdf", "xsec"]:
            if (sys == "pdf") & ("TTJets" not in sample):
                continue
            for var in ["up", "down"]:
                print(sample + "_centJER_" + sys + "_" + var + "_bdt")
                h = f.Get(sample + "_centJER_" + sys + "_" + var + "_bdt")
                print(type(h))
                if var == "up":
                    h.Write(sample + "_" + sys + "Up")
                if var == "down":
                    h.Write(sample + "_" + sys + "Down")                    
         
                    
    print(outfile.ls())
    outfile.Close()

def to_eff(h1, h2):
    
    h3 = h1.Clone("h3")
    h3.Sumw2()
    h3.Divide(h2)
    return h3
    
def beff():
    
    f = ROOT.TFile("data/beff.root")
    h2_BTaggingEff_Denom_b = f.Get("h2_BTaggingEff_Denom_b")
    h2_BTaggingEff_Num_b = f.Get("h2_BTaggingEff_Num_b")
    h2_BTaggingEff_Denom_c = f.Get("h2_BTaggingEff_Denom_c")
    h2_BTaggingEff_Num_c = f.Get("h2_BTaggingEff_Num_c")
    h2_BTaggingEff_Denom_usdg = f.Get("h2_BTaggingEff_Denom_udsg")
    h2_BTaggingEff_Num_usdg = f.Get("h2_BTaggingEff_Num_udsg")
    
    outfile = ROOT.TFile("data/beff_precalc.root", "RECREATE")
    b = to_eff(h2_BTaggingEff_Num_b, h2_BTaggingEff_Denom_b)
    c = to_eff(h2_BTaggingEff_Num_c, h2_BTaggingEff_Denom_c)
    usdg = to_eff(h2_BTaggingEff_Num_usdg, h2_BTaggingEff_Denom_usdg)
    b.Write("b")
    c.Write("c")
    usdg.Write("usdg")
    outfile.Close()
    
    ff = ROOT.TFile("data/beff_precalc.root")
    ext = extractor()
    ext.add_weight_sets([
        "* * data/beff_precalc.root"
    ])
    ext.finalize()

    evaluator = ext.make_evaluator()
    
    h1 = ff.Get("b")
    h2 = ff.Get("c")
    h3 = ff.Get("usdg")
    
    b_num, edges = root_numpy.hist2array(h1, return_edges=True)
    c_num, _ = root_numpy.hist2array(h2, return_edges=True)
    usdg_num, _ = root_numpy.hist2array(h3, return_edges=True)
    
    sf = np.dstack((usdg_num, c_num, b_num ))
    hist = ROOT.TH3F("hist", "hist", 100, 0., 1000., 60, -3., 3., 3, -0.5, 2.5)
    
    _ = array2hist(sf, hist)
    
    outtest = ROOT.TFile("data/beff_test.root", "RECREATE")
    hist.Write("eff")
    outtest.Close()
    
if __name__ == "__main__":
    
    file_path = "/eos/user/l/llayer/cmsopen/columnar/syst_variation/histos/bdt.root"
    outpath = "/eos/user/l/llayer/cmsopen/columnar/syst_variation/histos/harvester_input.root"
    to_harvester(file_path, outpath)
    
    """
    for d in mc: # + data:
        print( d )
        to_rt(sample_name = d)
    """
            
    