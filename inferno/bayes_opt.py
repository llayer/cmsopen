import train
import pandas as pd
import numpy as np
from skopt.space import Real, Categorical, Integer
from skopt.utils import use_named_args
from skopt import gp_minimize


def store_results(search_result, prior_names):

    params = pd.DataFrame(search_result['x_iters'])
    params.columns = [*prior_names]
    params = params.rename_axis('call').reset_index()
    scores = pd.DataFrame(search_result['func_vals'])
    scores.columns = ['score']
    result = pd.concat([params, scores], axis=1)
    result = result.sort_values(by=['score'])

    #result.to_hdf(self.path + 'skopt.h5', 'frame')

    return result 


def find_optimal_parameters_inferno(opendata, args, epochs, dimensions, initial_param, num_calls=12): 

    prior_values = []
    prior_names = []
    for var in dimensions:
        name = var.name
        print( name )
        prior_names.append(name)
        prior_values.append(initial_param[name])


    global num_skopt_call
    num_skopt_call = 0
    cv_results = []

    @use_named_args(dimensions)
    def fitness(**p): 

        global num_skopt_call

        print('\n \t ::: {} SKOPT CALL ::: \n'.format(num_skopt_call+1))
        print(p)

        args["inferno_lr"] = p["inferno_lr"]
        args["inferno_neurons"] = p["inferno_neurons"]
        args["temperature"] = p["temperature"]
                
        inferno_model, inferno_info = train.train_inferno(opendata, args, epochs = epochs)
        score = min(inferno_info["loss"].losses["val"])

        
        num_skopt_call += 1

        return score

    search_result = gp_minimize( func = fitness, dimensions = dimensions,
                                 acq_func = 'EI', # Expected Improvement
                                 n_calls = num_calls, x0 = prior_values )

    #result = store_results(search_result, prior_names)

    return search_result, prior_names



def find_optimal_parameters_bce(opendata, args, epochs, dimensions, initial_param, num_calls=12): 

    prior_values = []
    prior_names = []
    for var in dimensions:
        name = var.name
        print( name )
        prior_names.append(name)
        prior_values.append(initial_param[name])


    global num_skopt_call
    num_skopt_call = 0
    cv_results = []

    @use_named_args(dimensions)
    def fitness(**p): 

        global num_skopt_call

        print('\n \t ::: {} SKOPT CALL ::: \n'.format(num_skopt_call+1))
        print(p)

        args["bce_lr"] = p["bce_lr"]
        args["bce_neurons"] = p["bce_neurons"]
        
        print(args)
        
        bce_model, bce_info = train.train_bce(opendata, args, epochs = epochs)
        score = min(bce_info["loss"].losses["val"])
        
        num_skopt_call += 1

        return score

    search_result = gp_minimize( func = fitness, dimensions = dimensions,
                                 acq_func = 'EI', # Expected Improvement
                                 n_calls = num_calls, x0 = prior_values )

    #result = store_results(search_result, prior_names)

    return search_result, prior_names



def run_inferno_opt(opendata, args, epochs):
    
    train_args = args.copy()
    
    skopt_dim_nominal = [
        Real(        low=1e-4, high=1e-2, prior='log-uniform', name='inferno_lr'),
        Real(        low=0.01, high=0.99, name='temperature'),
        Integer(     low=50, high=200,  name='inferno_neurons')
        ]
    initial_param = {'inferno_lr': 1e-3, 'temperature':0.2, 'inferno_neurons' : 80}
    
    search_result, prior_names = find_optimal_parameters_inferno(opendata, train_args, epochs, skopt_dim_nominal, 
                                                                 initial_param, num_calls=12)
    results = store_results(search_result, prior_names)
    results.to_hdf(args["outpath"] + "/skopt/inferno.h5", "frame")
    print(results)
    args["inferno_lr"] = results.iloc[0]["inferno_lr"]
    args["inferno_neurons"] = int(results.iloc[0]["inferno_neurons"])
    args["temperature"] = results.iloc[0]["temperature"]

def run_bce_opt(opendata, args, epochs):
    
    train_args = args.copy()
    
    skopt_dim_nominal = [
        Real(        low=1e-4, high=1e-2, prior='log-uniform', name='bce_lr'),
        Integer(     low=50, high=200,  name='bce_neurons')
        ]
    initial_param = {'bce_lr': 1e-3, 'bce_neurons' : 80}
    
    search_result, prior_names = find_optimal_parameters_bce(opendata, train_args, epochs, skopt_dim_nominal, 
                                                             initial_param, num_calls=12)
    results = store_results(search_result, prior_names)
    results.to_hdf(args["outpath"] + "/skopt/bce.h5", "frame")
    print(results)
    print(results.iloc[0]["bce_lr"], results.iloc[0]["bce_neurons"])
    args["bce_lr"] = results.iloc[0]["bce_lr"]
    args["bce_neurons"] = int(results.iloc[0]["bce_neurons"])
    