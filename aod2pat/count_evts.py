import glob

data = ['Run2011A_MultiJet', 'Run2011B_MultiJet', 'Run2011A_SingleMu', 'Run2011B_SingleMu']
mc = ['TTJets', 'WJetsToLNu', 'DYJetsToLL', 'T_TuneZ2_s', 'T_TuneZ2_tW', 'T_TuneZ2_t-channel',
       'Tbar_TuneZ2_s', 'Tbar_TuneZ2_tW', 'Tbar_TuneZ2_t-channel']


def count_files(samples, in_path):
    for set in samples:

        files = glob.glob(in_path +  set + "*.txt")
        tot_count = 0
        for f in files:
            num_lines = sum(1 for line in open(f))
            tot_count += num_lines
        #print files
        print set, tot_count

count_files(data, 'data/CMS_')
count_files(mc, 'data/CMS_MonteCarlo2011_Summer11LegDR_')

# CTEQ6L1 is the PDF

# CHECK!! https://twiki.cern.ch/twiki/bin/viewauth/CMS/StandardModelCrossSections#Central_values_and_errors
xsec = {'WJetsToLNu': [31314, 1558, 1558],
        'TTJets': [177.31, 0., 0.], #DANGER!! CHECK!!
        'DYJetsToLL': [3048, 132, 132],
        'T_TuneZ2_t-channel': [42.6, 2.4, 2.3],
        'Tbar_TuneZ2_t-channel': [22.0, 0.1, 0.8],
        'T_TuneZ2_s': [2.76, 0.11 ,0.10],
        'Tbar_TuneZ2_s': [1.52, 0.09, 0.08],
        'T_TuneZ2_tW': [7.87,  0.6,  0.6],
        'Tbar_TuneZ2_tW': [7.87,  0.6,  0.6]
}
