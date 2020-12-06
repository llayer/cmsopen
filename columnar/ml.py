import pandas as pd
import numpy as np
import ROOT
from root_numpy import fill_hist
import xgboost as xgb
from sklearn.model_selection import train_test_split

sample_names = [ "data", "qcd", "signal", "tt_bkg", "wz", "single_t"]
cols = ['ht', 'aplanarity', 'sphericity', 'chargeEta', 'met', 'deltaPhiTauMet', 'mt', 'mTauJet', 'label']

samples = {}

for s in sample_names:
    samples[s] = pd.read_hdf("/afs/cern.ch/work/l/llayer/CMSSW_10_2_18/src/samples/" + s +".h5")
    if s == "data":
        pass
    elif s == "qcd":
        samples[s]['weight'] = 0.24
    else:
        samples[s]['weight'] = samples[s]['norm'] * samples[s]['trigger_weight_new'] #* samples[s]["btag_weight1"]


def prepare_data( sig, bkg, n_sig=5000, n_bkg=5000, test_size = 0.5 ):

    sig["label"] = 1
    bkg["label"] = 0
    data = pd.concat( [sig[cols][0:n_sig], bkg[cols][0:n_bkg]], axis=0)

    features = data.drop(['label'], axis=1).values
    labels = data[['label']].values.ravel()
    return train_test_split(features, labels, test_size=test_size, random_state=42)

# Overtraining
def compare_train_test(clf, X_train, y_train, X_test, y_test, bins=15):

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

if __name__ == "__main__":

    X_train, X_test, y_train, y_test = prepare_data()
    # Define model
    model_bdt = xgb.XGBClassifier(n_estimators=1000, learning_rate = 0.01, n_jobs = 1)
    # Fit
    model_bdt.fit(X_train, y_train, eval_metric=["logloss"], verbose=False)
    # Predict
    for s in samples:
        samples[s]["xgb"] = model_bdt.predict_proba(samples[s][var].values)[:,0]
