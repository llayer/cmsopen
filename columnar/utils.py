import ROOT
from coffea.lookup_tools import extractor
from root_numpy import array2hist, hist2array
import pandas as pd

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

def inferno_to_harvester(variables, file_path, outpath):
    
    f = ROOT.TFile(file_path)
    
    print(f.ls())
    
    outfile = ROOT.TFile(outpath, 'RECREATE')
    
    for var in variables:
        outfile.mkdir(var)
        outfile.cd(var)
        h = f.Get("Data" + "_" + var)
        h.Write('data_obs')
        h = f.Get("QCD" + "_" + var)
        h.Write('QCD')
        """
        h = f.Get("QCD_" + var + "_up")
        h.Write('QCD_VarUp')
        h = f.Get("QCD_" + var + "_down")
        h.Write('QCD_VarDown')   
        """
        
        for sample in [ "TTJets_bkg", "WZJets", "STJets", "TTJets_signal"]:
            # Loop over systematics
            h = f.Get(sample + "_" + var)
            h.Write(sample)
            if sample == "TTJets_signal":   
                try:
                    h = f.Get("TTJets_signal_" + var + "_up")
                    h.Write('TTJets_signal_VarUp')
                    h = f.Get("TTJets_signal_" + var + "_down")
                    h.Write('TTJets_signal_VarDown') 
                except:
                    print("TTJets_signal_" + var + "_up")
                
            
    print(outfile.ls())
    outfile.Close()
        
    
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
        h = f.Get(sample + "_bdt")
        h.Write(sample)  
        for sys in ["jes", "jes_old", "jer", "taue", "met", "btag", "trigger", "pdf", "xsec"]:
            
            if (sys == "pdf") & ("TTJets" not in sample):
                continue
            for var, upperVar in zip(["up", "down"], ["Up", "Down"]):
                print(sample + "_" + sys + "_" + var + "_bdt")
                h = f.Get(sample + "_" + sys + "_" + var + "_bdt")
                print(type(h))
                h.Write(sample + "_" + sys + upperVar)
                    
                if sys == "jes":
                    for c_var in ["03", "06", "09"]:
                        
                        print(sample + "_" + c_var + "_" + sys + "_" + var + "_bdt")
                        h = f.Get(sample + "_" + c_var + "_" + sys + "_" + var + "_bdt")
                        print(type(h))
                        h.Write(sample + "_" + sys + "_" + c_var + upperVar)
                        
                if sys == "taue":
                    for c_var in ["05", "07", "09"]:
                        print(sample + "_" + c_var + "_" + sys + "_" + var + "_bdt")
                        h = f.Get(sample + "_" + c_var + "_" + sys + "_" + var + "_bdt")
                        print(type(h))
                        h.Write(sample + "_" + sys + "_" + c_var + upperVar)
                    
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

    
    
def print_yields(path):
    
    # Load df
    # TO IMPLEMENT
    df = pd.read_hdf(path)
    
    df_data = df[df["tau_requirement_w"].isna() == True]
    df_mc = df[df["tau_requirement_w"].isna() == False]
    df_tt_sig = df[df["btag_w_sig"].isna() == False]
    df_tt_bkg = df[df["btag_w_bkg"].isna() == False]
    
    df_data = df_data.astype(str)
    def format_yield(col, col_err):
        return '{0:.1f}'.format(col) + " $\pm$ " + '{0:.1f}'.format(col_err)
    out_cols = []
    for cut in ["tau_requirement_w", "lep_veto_w", "met_w", "btag_w"]:
        df_mc[cut[:-2]] = df_mc.apply(lambda row : format_yield(row[cut], row[cut + "_err"]), axis=1) 
        df_tt_sig[cut[:-2]] = df_tt_sig.apply(lambda row : format_yield(row[cut + "_sig"], row[cut + "_err_sig"]), axis=1)
        df_tt_bkg[cut[:-2]] = df_tt_sig.apply(lambda row : format_yield(row[cut + "_bkg"], row[cut + "_err_bkg"]), axis=1)  
    df_tt_sig["sample"]  = "TTJets_sig"
    df_tt_bkg["sample"]  = "TTJets_bkg"        
    df_out = pd.concat([df_mc, df_data, df_tt_sig, df_tt_bkg])
    
    sample_names = df_out["sample"]
    df_out = df_out[["tau_requirement", "lep_veto", "met", "btag"]].transpose()
    sample_names.name = "Cut"
    df_out.columns = sample_names
    
    row = {"tau_requirement":r"4 jets $+\tau_{h}$", "lep_veto":r"$\mathrm{e}, \mu$ veto" , 
           "met":r"$E_{\mathrm{T}}^{\mathrm{miss}}$", "btag":r"$\geq 1 \mathrm{btag}$"}
    df_out = df_out.rename(row)
    
    cols1 = [
     'Run2011A_MultiJet', 'Run2011B_MultiJet', 'TTJets_sig', 'TTJets_bkg']
    header1 = [ 'Run A', 'Run B', r"$\mathrm{t} \overline{\mathrm{t}}\rightarrow \tau_{h}+$jets",
     r"$\mathrm{t} \overline{\mathrm{t}}\rightarrow \mathrm{X}$"]    
    cols2 = ['WJetsToLNu', 'DYJetsToLL', 'T_TuneZ2_tW', 'Tbar_TuneZ2_tW']
    header2 = ['W + jets', 'Z + jets', "s-t (tW)", r"s-$\overline{\mathrm{t}}$ (tW)"]
    cols3 = ['T_TuneZ2_t-channel', 'Tbar_TuneZ2_t-channel', 'T_TuneZ2_s', 'Tbar_TuneZ2_s']
    header3 = [ "s-t (t)", r"s-$\overline{\mathrm{t}}$ (t)", "s-t (s)", r"s-$\overline{\mathrm{t}}$ (s)"]    

    
    print(df_out[cols1].to_latex(escape=False, header=header1))  
    print(df_out[cols2].to_latex(escape=False, header=header2))
    print(df_out[cols3].to_latex(escape=False, header=header3))

    
    
    
    
if __name__ == "__main__":
    
    file_path = "/eos/user/l/llayer/cmsopen/columnar/syst_variation/histos/bdt.root"
    outpath = "/eos/user/l/llayer/cmsopen/columnar/syst_variation/histos/harvester_input.root"
    to_harvester(file_path, outpath)
    
    """
    for d in mc: # + data:
        print( d )
        to_rt(sample_name = d)
    """
            
    