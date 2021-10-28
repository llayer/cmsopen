import numpy as np
import matplotlib as mpl
mpl.use('Agg')
import matplotlib.pyplot as plt


def plot(inpath):

    with open(inpath + "/pulls.txt") as f:
        fullcontent = f.readlines()
    #print(fullcontent)
    names = []
    sb_mean = []
    sb_pull = []
    for i, s in enumerate(fullcontent):
        if i==0: continue
        l = s.split()
        #print(l)
        names.append(l[0])
        sb_mean.append(float(l[4]))
        sb_pull.append(float(l[6]))
    print(list(zip(names, sb_mean, sb_pull)))

    x = [0.5 + i for i in range(len(names))]
    plt.rcParams.update({'font.size': 15})
    plt.figure()
    plt.errorbar(x, sb_mean, yerr=sb_pull, fmt='o', capsize=10.)
    plt.xticks(x, names, rotation=0, size=15)
    plt.xlim(0, len(names))
    plt.ylim(-2, 2)

    #plt.axhline(y=1, color='black', linestyle='dashed')
    #plt.axhline(y=-1, color='black', linestyle='dashed')
    plt.axhline(y=0, color='black', linestyle='dashed')
    plt.axhspan(-1, 1, alpha=0.2, color='red')
    plt.savefig(inpath + "/pulls.png")
