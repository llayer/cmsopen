import ROOT
import pandas as pd
import glob

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
    
    
if __name__ == "__main__":
        
    
    for d in mc: # + data:
        print( d )
        to_rt(sample_name = d)
    
