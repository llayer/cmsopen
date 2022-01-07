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

        args["lr"] = p["lr"]
        args["neurons"] = p["neurons"]
        args["temperature"] = p["temperature"]
        
        try:
            inferno_model, inferno_info = train.train_inferno(opendata, args, epochs = epochs)
            score = min(inferno_info.losses["val"])
        except:
            score = 10000

        
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

        args["lr"] = p["lr"]
        args["neurons"] = p["neurons"]
        
        try:
            bce_model, bce_info = train.train_bce(opendata, args, epochs = epochs)
            score = min(bce_info.losses["val"])
        except:
            score = 10000

        
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
        Real(        low=1e-4, high=1e-2, prior='log-uniform', name='lr'),
        Real(        low=0.01, high=0.99, name='temperature'),
        Integer(     low=50, high=200,  name='neurons')
        ]
    initial_param = {'lr': 1e-3, 'temperature':0.2, 'neurons' : 80}
    
    search_result, prior_names = find_optimal_parameters_inferno(opendata, train_args, epochs, skopt_dim_nominal, 
                                                                 initial_param, num_calls=30)


def run_bce_opt(opendata, args, epochs):
    
    train_args = args.copy()
    
    skopt_dim_nominal = [
        Real(        low=1e-4, high=1e-2, prior='log-uniform', name='lr'),
        Integer(     low=50, high=200,  name='neurons')
        ]
    initial_param = {'lr': 1e-3, 'neurons' : 80}
    
    search_result, prior_names = find_optimal_parameters_bce(opendata, train_args, epochs, skopt_dim_nominal, 
                                                             initial_param, num_calls=30)

    