import ROOT
import numpy as np
import copy

def eigenvalues_momtensor(input_vectors, r=2):

    #momentumTensor = ROOT.TMatrixDSym(3)
    #momentumTensor.Zero()

    momentumTensor = np.zeros((3,3))

    if  len(input_vectors) < 2:
        return momentumTensor

    # fill momentumTensor from inputVectors
    norm = 1.
    for vec in input_vectors:
        #print vec, np.dot(vec, vec)
        p2 = np.dot(vec, vec)

        # DANGER CHECK ifs
        if r == 2:
            pR = p2
            pRminus2 = 1.
        else:
            pR = np.power(p2, 0.5*r)
            pRminus2 = np.power(p2, 0.5*r - 1.)

        #print pR, pRminus2

        norm += pR
        momentumTensor[0][0] += pRminus2*vec[0]*vec[0]
        momentumTensor[0][1] += pRminus2*vec[0]*vec[1]
        momentumTensor[0][2] += pRminus2*vec[0]*vec[2]
        momentumTensor[1][0] += pRminus2*vec[1]*vec[0]
        momentumTensor[1][1] += pRminus2*vec[1]*vec[1]
        momentumTensor[1][2] += pRminus2*vec[1]*vec[2]
        momentumTensor[2][0] += pRminus2*vec[2]*vec[0]
        momentumTensor[2][1] += pRminus2*vec[2]*vec[1]
        momentumTensor[2][2] += pRminus2*vec[2]*vec[2]

    # return momentumTensor normalized to determinant 1
    momentumTensor = (1./norm)*momentumTensor

    #print momentumTensor

    # CV: TMatrixDSym::EigenVectors returns eigen-values and eigen-vectors
    # ordered by descending eigen-values, so no need to do any sorting H_ere...
    #return eigenValues
    # Check if symmetric
    if np.all(np.abs(momentumTensor-momentumTensor.T) < 1e-8):
        if not 0 in momentumTensor:
            #print momentumTensor
            eigenValues, eigenVectors = np.linalg.eig(momentumTensor)
            # the largest (smallest) eigen-value is stored at index position 0 (2)
            idx = eigenValues.argsort()[::-1]
            eigenValues = eigenValues[idx]
            eigenVectors = eigenVectors[:,idx]
            #print eigenValues, eigenVectors
            return eigenValues
        else:
            print( "0 in momentum tensor" )
    else:
        print( "Asymmetric momentum tensor" )
    # DANGER check return val
    return 0


def eval_aplanarity(eigenValues):
    # 1.5*q1 wH_ere 0<=q1<=q2<=q3 are tH_e eigenvalues of tH_e momentum tensor sum{p_j[a]*p_j[b]}/sum{p_j**2}
    # normalized to 1. Return values are 0.5 for spH_erical and 0 for plane and linear events
    return 1.5 * eigenValues[2]

def eval_sphericity(eigenValues):
    # 1.5*(q1+q2) wH_ere 0<=q1<=q2<=q3 are tH_e eigenvalues of tH_e momentum tensor sum{p_j[a]*p_j[b]}/sum{p_j**2}
    # normalized to 1. Return values are 1 for spH_erical, 3/4 for plane and 0 for linear events
    return 1.5*(eigenValues[1] + eigenValues[2])

"""
def circularity(input_vectors, numberOfSteps=1000):

    #the return value is 1 for spherical and 0 linear events in r-phi. TH_is function needs tH_e
    #number of steps to determine H_ow fine tH_e granularity of tH_e algoritH_m in pH_i sH_ould be
    deltaPH_i = 2 * np.pi / numberOfSteps
    circularity=-1
    pH_i=0
    area = 0
    for vec in input_vectors:
        area += np.sqrt(vec[0]*vec[0] + vec[1]*vec[1])
    for in in range(numberOfSteps):
        pH_i += deltaPH_i
        sum=0
        tmp=0.
        for vec in input_vectors:
            sum += abs(np.cos(pH_i)*vec[0] + np.sin(pH_i)*vec[1])
        tmp = (np.pi/2)*(sum/area)
        if( circularity<0 || tmp<circularity ){
          circularity=tmp
    return circularity


def C(eigenValues):
    # 3.*(q1*q2+q1*q3+q2*q3) wH_ere 0<=q1<=q2<=q3 are tH_e eigenvalues of tH_e momentum tensor sum{p_j[a]*p_j[b]}/sum{p_j**2}
    # normalized to 1. Return value is between 0 and 1
    # and measures tH_e 3-jet structure of tH_e event (C vanisH_es for a "perfect" 2-jet event)
    return 3.*(eigenValues[0]*eigenValues[1] + eigenValues[0]*eigenValues[2] + eigenValues[1]*eigenValues[2])

def D(eigenValues):
    # 27.*(q1*q2*q3) wH_ere 0<=q1<=q2<=q3 are tH_e eigenvalues of tH_e momemtum tensor sum{p_j[a]*p_j[b]}/sum{p_j**2}
    # normalized to 1. Return value is between 0 and 1
    # and measures tH_e 4-jet structure of tH_e event (D vanisH_es for a planar event)
    return 27.*eigenValues(0)*eigenValues(1)*eigenValues(2);

"""


def hlFeatures(ev, njets=-1):

    hl = {}
    HT, H = 0., 0
    input_vectors = []
    
    # Tau 4vector
    tau_vec = ROOT.TLorentzVector(ev["Tau_px"][0], ev["Tau_py"][0], ev["Tau_pz"][0], ev["Tau_e"][0])
    p4 = copy.deepcopy(tau_vec)
    input_vectors.append(np.array([ev["Tau_px"][0], ev["Tau_py"][0], ev["Tau_pz"][0]]))
    
    # Jet 4vectors
    iJet = 0
    for i in range(len(ev["Jet_pt"])):
        
        if iJet == njets:
            break
            
        jet_4vec = ROOT.TLorentzVector(ev["Jet_px"][i], ev["Jet_py"][i], ev["Jet_pz"][i], ev["Jet_e"][i])
        input_vectors.append(np.array([ev["Jet_px"][i], ev["Jet_py"][i], ev["Jet_pz"][i]]))
        H += jet_4vec.E()
        HT += jet_4vec.Et()
        p4 += jet_4vec
        
        iJet += 1        
        

    # MET
    met_vec = ROOT.TLorentzVector(ev["MET_px"], ev["MET_py"], ev["MET_pz"], ev["MET_e"])

    # Event shape variables - needed!
    hl["ht"] = HT

    eigenValues = eigenvalues_momtensor(input_vectors)

    hl['aplanarity'] = eval_aplanarity(eigenValues)
    hl['sphericity']  = eval_sphericity(eigenValues)
    hl['chargeEta']   = ev["Tau_charge"][0] * abs(ev["Tau_eta"][0])
    hl['met']         = met_vec.Et()
    hl['deltaPhiTauMet'] = abs(tau_vec.DeltaPhi(met_vec))
    # !!
    #print ((tau_vec.Et() + met_vec.Et())**2)
    #print ((tau.px + met_vec.Px())**2)
    #print ((tau.py + met_vec.Py())**2)
    hl['mt'] = np.sqrt(((tau_vec.Et() + met_vec.Et())**2) - ((tau_vec.Px() + met_vec.Px())**2) -
                        ((tau_vec.Py() + met_vec.Py())**2))
    hl['mTauJet'] = p4.M()

    return hl
