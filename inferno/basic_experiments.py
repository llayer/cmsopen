import inferno_opendata
import inferno_config

# Load the default config of the training
args = inferno_config.args
path = "/home/centos/mount_point/data/basic_experiments3/"
epochs = 100
experiments = []

# Experiment 0 - default
name = "default"
ex0 = args.copy()
ex0["outpath"] = path + name
experiments.append(ex0)

# Experiment 1 - sort output
name = "sorted"
ex1 = args.copy()
ex1["outpath"] = path + name
ex1["fit_sorted"] = True
experiments.append(ex1)

# Experiment 2 - use histogram
name = "softhist"
ex2 = args.copy()
ex2["outpath"] = path + name
ex2["use_softhist"] = True
experiments.append(ex2)

# Experiment 3 - use weights
name = "no_weights"
ex3 = args.copy()
ex3["outpath"] = path + name
ex3["use_weights"] = False
experiments.append(ex3)

# Experiment 4 - no systematics
name = "no_syst"
ex4 = args.copy()
ex4["outpath"] = path + name
ex4["shape_syst"] = []
ex4["scale_shape_norms"] = None #[("jes", 0.06)]
experiments.append(ex4)

# Experiment 5 - only 1 shape syst - JER 
name = "shape_syst_only"
ex5 = args.copy()
ex5["outpath"] = path + name
ex5["shape_syst"] = ["jer"]
ex5["scale_shape_norms"] = [("jer", 0.06)]
experiments.append(ex5)

# Experiment 6 - only 1 weight syst - btag 
name = "weight_syst_only"
ex6 = args.copy()
ex6["outpath"] = path + name
ex6["use_weights"] = True
ex6["shape_syst"] = []
ex6["scale_shape_norms"] = None
ex6["weight_syst"] = ["btag"]
experiments.append(ex6)

# Experiment 7 - 1 shape and 1 weight syst
name = "shape_weight_syst"
ex7 = args.copy()
ex7["outpath"] = path + name
ex7["weight_syst"] = ["btag"]
experiments.append(ex7)

# Experiment 8 - norm syst
name = "norm_syst"
ex8 = args.copy()
ex8["outpath"] = path + name
ex8["shape_syst"] = []
ex8["scale_shape_norms"] = None
ex8["s_norm_syst"] = ["lumi"]#["lumi", "tau_trigger"] #["lumi", 'tau_trigger']
ex8["b_norm_syst"] = ["mistag"] #[]
experiments.append(ex8)

# Experiment 9 - shape + weight + norm syst
name = "shape_weight_norm_syst"
ex9 = args.copy()
ex9["outpath"] = path + name
ex9["weight_syst"] = ["btag"]
ex9["s_norm_syst"] = ["lumi"]#["lumi", "tau_trigger"] #["lumi", 'tau_trigger']
ex9["b_norm_syst"] = ["mistag"] #[]
experiments.append(ex9)

# Experiment 10 - fast vertical interpolation
name = "fast_vertical"
ex10 = args.copy()
ex10["outpath"] = path + name
ex10["interp_algo"] = "fast_vertical"
experiments.append(ex10)

if __name__ == "__main__":
    
    
    for i, ex in enumerate(experiments):
        #print(ex)
        try:
            inferno_opendata.run_cmsopen(ex, epochs = epochs, retrain=False, do_fit = True)
        except:
            print("Ex", str(i), "failed")
   
    #print(experiments[4])
    #inferno_opendata.run_cmsopen(experiments[4], epochs = epochs, do_fit = True)





