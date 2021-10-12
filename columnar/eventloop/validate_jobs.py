import pandas as pd
import glob

infiles = glob.glob("/afs/cern.ch/work/l/llayer/CMSSW_5_3_32/src/workspace/pattuples2011/data/CMS_Run2011A_MultiJet*")
#infiles = glob.glob("/afs/cern.ch/work/l/llayer/CMSSW_5_3_32/src/workspace/pattuples2011/data/CMS_MonteCarlo2011_Summer11LegDR_TTJets*")
dfs = []
for f in infiles:
    dfs.append( pd.read_csv(f, header=None) )
df_in = pd.concat(dfs)
df_in.columns = ["infile"]

def inhash(file):
    return file.split("/")[-1][:-5]

df_in["hash"] = df_in["infile"].apply(inhash)

outfiles = glob.glob("/eos/user/l/llayer/opendata_files/Run2011A_MultiJet/*.root")
df_out = pd.DataFrame(outfiles)
df_out.columns = ["outfile"]

def out_hash(file):
    f = file.split("/")[-1]
    return f.split("_")[-2]

def out_jobid(file):
    f = file.split("/")[-1]
    return f.split("_")[-1][:-5]

df_out["hash"] = df_out["outfile"].apply(out_hash)
df_out["jobid"] = df_out["outfile"].apply(out_jobid)

df = pd.merge(df_in, df_out, on="hash", how="outer")

missing = df[df.isnull().any(axis=1)]
succesfull = df[~df.isnull().any(axis=1)]

print "TTT", len(missing), len(succesfull), len(df)

good_ids = set(list(succesfull['jobid'].astype(int)))
print "SETTT", len(good_ids)
print good_ids
all_ids = set([x for x in range(0, len(df))])
print "SETTTaa", len(all_ids)
print all_ids

missing_ids = list(good_ids ^ all_ids) 

missing['infile'].to_csv('data/missing_Run2011A_MultiJet.txt', index=False)

if len(df_in) == len(df_out):
    print "All jobs finished succesfully"
else:
    print "Number of input files", len(df_in), "Number of output files", len(df_out)
    print missing
    print "IDs of the missing jobs", missing_ids
