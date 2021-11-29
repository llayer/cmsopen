import inferno_opendata
import inferno_config

# Load the default config of the training
args = inferno_config.args
path = "/home/centos/mount_point/data/basic_experiments/"
epochs = 100
experiments = []

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
name = "weights"
ex3 = args.copy()
ex3["outpath"] = path + name
ex3["use_weights"] = True
experiments.append(ex3)

# Experiment 4 - no systematics
name = "no_syst"
ex4 = args.copy()
ex4["outpath"] = path + name
ex4["shape_syst"] = []
ex4["shape_norm_sigma"] = []
experiments.append(ex4)

# Experiment 5 - only 1 shape syst - JES 
name = "shape_syst_only"
ex5 = args.copy()
ex5["outpath"] = path + name
ex5["shape_syst"] = ["06_jes"]
ex5["shape_norm_sigma"] = [0.05]
experiments.append(ex5)

# Experiment 6 - only 1 weight syst - btag 
name = "weight_syst_only"
ex6 = args.copy()
ex6["outpath"] = path + name
ex6["use_weights"] = True
ex6["shape_syst"] = []
ex6["weight_syst"] = ["btag"]
ex6["shape_norm_sigma"] = [0.02]
experiments.append(ex6)

# Experiment 7 - 1 shape and 1 weight syst
name = "shape_weight_syst"
ex7 = args.copy()
ex7["outpath"] = path + name
ex7["shape_syst"] = ["06_jes"]
ex7["weight_syst"] = ["btag"]
ex7["shape_norm_sigma"] = [0.05, 0.02]
ex7["use_weights"] = True
experiments.append(ex7)

# Experiment 8 - fast vertical interpolation
name = "fast_vertical"
ex8 = args.copy()
ex8["outpath"] = path + name
ex8["interp_algo"] = "fast_vertical"
experiments.append(ex8)

if __name__ == "__main__":
    
    """
    for i, ex in enumerate(experiments[4]):
        print(ex)
        try:
            inferno_opendata.run_cmsopen(ex, epochs = epochs, do_fit = True)
        except:
            print("Ex", str(i), "failed")
    """
    print(experiments[4])
    inferno_opendata.run_cmsopen(experiments[4], epochs = epochs, do_fit = True)





