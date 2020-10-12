import root_pandas
import pandas as pd
from btag import *

print "Load frame"
#df = root_pandas.read_root( "TTJets.root", "Cand")
df = root_pandas.read_root( "out/WJetsToLNu/WJetsToLNu.root", "Cand")

#df_A = root_pandas.read_root( "out/Run2011A_MultiJet/Run2011A_MultiJet.root", "Cand")
#df_B = root_pandas.read_root( "out/Run2011B_MultiJet/Run2011B_MultiJet.root", "Cand")

#df = pd.concat([df_A, df_B], axis=0)

#
# Final cuts
#
met_cut = 20.
df = df[df['met']>met_cut]

#
# B-tagging
#

def count_btags(jet_csvDisc):
    return sum(jet_csvDisc[0:4]>0.679)

def get_sf(jet_pt, jet_eta, jet_flavour):

    sfs = []
    for pt, eta, flav, in zip(jet_pt, jet_eta, jet_flavour):
        sfs.append( eval_btagSF(flav, eta, pt) )

    return sfs

print "Calculate the b-tags"
df['jet_nbtags'] = df.apply(lambda x: count_btags(x.jet_csvDisc), axis=1)

"""
print "Calculate the b-tagging scale factor"
df['jet_btagSF'] = df.apply(lambda x: get_sf(x.jet_pt, x.jet_eta, x.jet_flavour), axis=1)
print "Calculate the b-tagging weight with method 1"
df['btag_weight1'] = df.apply(lambda x: b_weight_method1(x.jet_pt, x.jet_eta, x.jet_flavour,  x.jet_csvDisc,  x.jet_btagSF), axis=1)
print "Calculate the b-tagging weight with method 2"
df['btag_weight2'] = df.apply(lambda x: b_weight_method2(x.jet_pt, x.jet_eta, x.jet_flavour,  x.jet_csvDisc,  x.jet_btagSF), axis=1)
"""
print "Store the frame as hdf5"
print df.head()

# Flatten the arrays
jet_names = [ 'jet_pt', 'jet_eta', 'jet_csvDisc', 'jet_flavour' ]#, 'jet_btagSF' ]
for j in jet_names:
    names = []
    for counter in range(6):
        names.append( j+str(counter) )
    df[names]=pd.DataFrame(df[j].values.tolist(),index=df.index)
    df = df.drop([j], axis=1)

#df.to_hdf("Data.h5", "frame", format='table')
df.to_hdf("WJetsToLNu.h5", "frame", format='table')
