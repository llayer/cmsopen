import ROOT

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
    
if __name__ == "__main__":
    
    file_path = "/eos/user/l/llayer/cmsopen/columnar/syst_variation/histos/bdt.root"
    outpath = "/eos/user/l/llayer/cmsopen/columnar/syst_variation/histos/harvester_input.root"
    to_harvester(file_path, outpath)
    
            
    