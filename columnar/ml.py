import pandas as pd
import numpy as np
import ROOT
from root_numpy import fill_hist
import xgboost as xgb
from sklearn.model_selection import train_test_split
import glob
import random
import matplotlib.pyplot as plt
import pickle



# Overtraining
def compare_train_test(clf, X_train, y_train, X_test, y_test, outpath, bins=15):

    decisions = []
    for X,y in ((X_train, y_train), (X_test, y_test)):
        d1 = clf.predict_proba(X[y>0.5])[:,1].ravel()
        d2 = clf.predict_proba(X[y<0.5])[:,1].ravel()
        decisions += [d1, d2]

    low = min(np.min(d) for d in decisions)
    high = max(np.max(d) for d in decisions)
    low_high = (low,high)

    plt.hist(decisions[0],
             color='r', alpha=0.5, range=low_high, bins=bins,
             histtype='stepfilled', density=True,
             label='S (train)')
    plt.hist(decisions[1],
             color='b', alpha=0.5, range=low_high, bins=bins,
             histtype='stepfilled', density=True,
             label='B (train)')

    hist, bins = np.histogram(decisions[2],
                              bins=bins, range=low_high, density=True)
    scale = len(decisions[2]) / sum(hist)
    err = np.sqrt(hist * scale) / scale

    width = (bins[1] - bins[0])
    center = (bins[:-1] + bins[1:]) / 2
    plt.errorbar(center, hist, yerr=err, fmt='o', c='r', label='S (test)')

    hist, bins = np.histogram(decisions[3],
                              bins=bins, range=low_high, density=True)
    scale = len(decisions[2]) / sum(hist)
    err = np.sqrt(hist * scale) / scale

    plt.errorbar(center, hist, yerr=err, fmt='o', c='b', label='B (test)')

    plt.xlabel("Output")
    plt.ylabel("Arbitrary units")
    plt.yscale("log")
    plt.legend(loc='best')
    plt.savefig(outpath + "/overtrain.png")
    
    
def train_test_split(df, n = 5000):

    df.reset_index(drop=True, inplace=True)
    indices = list(df.index)
    train_id = random.sample(indices, n)
    test_id = list(set(indices) - set(train_id))
    train_mask = df.index.isin(train_id)
    test_mask = df.index.isin(test_id)
    cond = [train_mask, test_mask]
    choice = ["train", "test"]
    df['train_flag'] = np.select(cond, choice)
    
    
def train(samples, outpath, n_sig = 4000, n_bkg = 4000, ntrees=1000, lr = 0.01, random_state = 0):
    
    print("Set random state")
    random.seed(random_state)
    
    print("Prepare training data")
    
    signal = samples["TTJets_signal"] #samples["TTJets_centJER_signal"] #samples["TTJets_signal_central"]
    bkg = samples["QCD"]
    
    train_test_split(signal, n_sig)
    train_test_split(bkg, n_bkg)
    
    print(list(signal))
    
    signal["label"] = 0
    bkg["label"] = 1
    
    signal_train = signal[signal["train_flag"] == "train"]
    signal_train["weights"] = signal_train['trigger_weight'] * signal_train['btag_weight1']
    signal_train["weights"] = signal_train["weights"] * (1. / np.mean(signal_train["weights"]))
    signal_test = signal[signal["train_flag"] == "test"]
    bkg_train = bkg[bkg["train_flag"] == "train"]
    bkg_train["weights"] = bkg_train['btag_weight2']
    bkg_train["weights"] = bkg_train["weights"] * (1. / np.mean(bkg_train["weights"]))
    bkg_test = bkg[bkg["train_flag"] == "test"]
    
    train_data = pd.concat([signal_train, bkg_train], axis=0)
    test_data = pd.concat([signal_test, bkg_test], axis=0)
    
    features = ['ht', 'aplanarity', 'sphericity', 'chargeEta', 'met', 'deltaPhiTauMet', 'mt', 'mTauJet']

    X_train = train_data[features].values
    y_train = train_data["label"].values.ravel()
    weights = train_data["weights"]
    #print(weights)
    
    X_test = test_data[features].values
    y_test = test_data["label"].values.ravel()   
    
    print("Train model")
    
    # Define model
    bdt = xgb.XGBClassifier(n_estimators=ntrees, learning_rate = lr, n_jobs = 1, random_state=random_state)
    # Fit
    bdt.fit(X_train, y_train, sample_weight = weights, eval_metric=["logloss"], verbose=False) # sample_weight
    
    # Save
    pickle.dump(bdt, open(outpath + "/bdt.pkl", "wb"))
    
    # Plot
    compare_train_test(bdt, X_train, y_train, X_test, y_test, outpath, bins=15)
    #bdt.feature_names = features
    
    importance = bdt.get_booster().get_score(importance_type="weight")
    mapper = {'f{0}'.format(i): v for i, v in enumerate(features)}
    mapped = {mapper[k]: v for k, v in importance.items()}
    keys = list(mapped.keys())
    values = list(mapped.values())
    data = pd.DataFrame(data=values, index=keys, columns=["f-score"]).sort_values(by = "f-score", ascending=False)
    data.plot(kind='barh')
    plt.savefig(outpath + "/importance.png")
    
    """
    fig, ax = plt.subplots(figsize=(8,6))
    xgb.plot_importance(bdt, height=0.8, ax=ax)
    fig.savefig(outpath + "/importance.png")
    
    importance = bdt.feature_importances_
    print( "Importance" )
    for feat, imp in zip(features, importance):
        print(feat, imp)
    """
    
    # Predict
    print("Predicting...") 
    
    for sample in samples:
        samples[sample]["bdt"] = bdt.predict_proba(samples[sample][features].values)[:,0]    
    

