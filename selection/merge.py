import os
import glob

dirs = glob.glob("out/*")

for d in dirs:
    proc = d.split("/")[-1]
    os.system("mkdir " + d + "/files")
    os.system("mv " + d + "/*.root " + d + "/files")
    os.system("hadd " + d + "/" + proc + ".root " + d + "/files/*.root")


