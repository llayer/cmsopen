import pandas as pd
import glob

data = ['Run2011A_MultiJet', 'Run2011B_MultiJet', 'Run2011A_SingleMu', 'Run2011B_SingleMu']
mc = ['TTJets', 'WJetsToLNu', 'DYJetsToLL', 'T_TuneZ2_s', 'T_TuneZ2_tW', 'T_TuneZ2_t-channel',
       'Tbar_TuneZ2_s', 'Tbar_TuneZ2_tW', 'Tbar_TuneZ2_t-channel']

in_path = "data/CMS_"

def inhash(file):
    return file.split("/")[-1][:-5]

def out_hash(file):
    f = file.split("/")[-1]
    return f.split("_")[-2]

def nano_hash(file):
    f = file.split("/")[-1]
    return f.split("_")[-3]

def out_jobid(file):
    f = file.split("/")[-1]
    return f.split("_")[-1][:-5]

#infiles = glob.glob("/afs/cern.ch/work/l/llayer/CMSSW_5_3_32/src/workspace/pattuples2011/data/CMS_MonteCarlo2011_Summer11LegDR_TTJets*")
def check_sample(sample_name = "Run2011A_MultiJet", nano = True, isData = True):

    if isData:
        infiles = glob.glob( in_path + sample_name + "*.txt" )
        #print len(infiles)
    else:
        infiles = glob.glob( in_path + "MonteCarlo2011_Summer11LegDR_" + sample_name + "*.txt" )

    print in_path + sample_name + "*.txt"

    dfs = []
    for f in infiles:
        dfs.append( pd.read_csv(f, header=None) )
    df_in = pd.concat(dfs)
    df_in.columns = ["infile"]

    df_in["hash"] = df_in["infile"].apply(inhash)

    outfiles = glob.glob("/eos/user/l/llayer/cmsopen/legacy/" + sample_name + "/*.root")
    df_out = pd.DataFrame(outfiles)
    df_out.columns = ["outfile"]


    df_out["hash"] = df_out["outfile"].apply(out_hash)
    df_out["jobid"] = df_out["outfile"].apply(out_jobid)

    df = pd.merge(df_in, df_out, on="hash", how="outer")
    """
    if nano:
        nano_path = "/afs/cern.ch/work/l/llayer/CMSSW_10_2_18/src/out/" + sample_name
        nano_files = glob.glob(nano_path + "/files/*.root")
        df_nano = pd.DataFrame(nano_files)
        df_nano.columns = ["nano_file"]
        df_nano['hash'] = df_nano["nano_file"].apply(nano_hash)
        #print df_nano.head()
        df = pd.merge(df, df_nano, on="hash", how="outer")
    """
    if len(df_in) == len(df_out):
        print "All AOD jobs finished succesfully"
    else:


        missing = df[df['outfile'].isnull()]
        """
        succesfull = df[~df.isnull().any(axis=1)]

        print "TTT", len(missing), len(succesfull), len(df)

        good_ids = set(list(succesfull['jobid'].astype(int)))
        #print "SETTT", len(good_ids)
        #print good_ids
        all_ids = set([x for x in range(0, len(df))])
        #print "SETTTaa", len(all_ids)
        #print all_ids

        missing_ids = list(good_ids ^ all_ids)
        """

        missing['infile'].to_csv('data/missing_' + sample_name + '.txt', index=False)

        print "Number of input files", len(df_in), "Number of output files", len(df_out)
        print missing
        #print "IDs of the missing jobs", missing_ids

    """
    if nano:
        if len(df_in) == len(df_nano):
            print "All nano jobs finished succesfully"
        else:
            missing = df[df['nano_file'].isnull()]
            print "Number of input files", len(df_in), "Number of output files", len(df_nano)
            print missing
            #print "IDs of the missing jobs", missing_ids
    """

if __name__ == "__main__":

    """
    for d in data:
        print d
        check_sample(sample_name = d, nano = False, isData = True)
    """

    for m in ["TTJets"]:#mc:
        print m
        check_sample(sample_name = m, nano = False, isData = False)
