import pandas as pd
import glob

mc = ['TTJets']#, 'WJetsToLNu', 'DYJetsToLL', 'T_TuneZ2_s', 'T_TuneZ2_tW', 'T_TuneZ2_t-channel', 'Tbar_TuneZ2_s', 'Tbar_TuneZ2_tW', 'Tbar_TuneZ2_t-channel']

in_path = "/afs/cern.ch/work/l/llayer/CMSSW_5_3_32/src/workspace/pattuples2011/data/CMS_"

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
def check_sample(sample_name = "TTJets"):

    infiles = glob.glob( in_path + "MonteCarlo2011_Summer11LegDR_" + sample_name + "*.txt" )

    print in_path + sample_name + "*.txt"

    dfs = []
    for f in infiles:
        dfs.append( pd.read_csv(f, header=None) )
    df_in = pd.concat(dfs)
    df_in.columns = ["infile"]

    df_in["hash"] = df_in["infile"].apply(inhash)

    outfiles = glob.glob("/afs/cern.ch/work/l/llayer/CMSSW_5_3_32/src/workspace/pdfweights/out/" + sample_name + "/" + sample_name + "*.root")
    df_out = pd.DataFrame(outfiles)
    df_out.columns = ["outfile"]


    df_out["hash"] = df_out["outfile"].apply(out_hash)
    df_out["jobid"] = df_out["outfile"].apply(out_jobid)

    df = pd.merge(df_in, df_out, on="hash", how="outer")

    if len(df_in) == len(df_out):
        print "All AOD jobs finished succesfully"
    else:

        missing = df[df['outfile'].isnull()]

        missing['infile'].to_csv('data/missing_' + sample_name + '.txt', index=False)

        print "Number of input files", len(df_in), "Number of output files", len(df_out)
        print missing
        #print "IDs of the missing jobs", missing_ids


if __name__ == "__main__":

    for m in mc:
        print m
        check_sample(sample_name = m)
