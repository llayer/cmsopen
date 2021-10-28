import os
import glob

files = glob.glob("missing*/*.root")

for f in files:
    dir_name = f.split("/")[0]
    new_name = f.split("/")[-1][8:]
    new_file = dir_name + "/" + new_name
    cmd = "mv " + f + " " + new_file
    print cmd 
    os.system(cmd)


