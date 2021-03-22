import ROOT


def fit(path, sample_names, fit_var, corr = "central"):

    f = ROOT.TFile(path)
        
    histos = {}
    for s in sample_names:
        if (s != "Data") & (s != "QCD"):
            if ("pdf" in corr) & ("TTJets" in s):
                hist = f.Get(s + "_" + corr +"_" + fit_var)
            elif ("pdf" in corr):
                hist = f.Get(s + "_" + "centJER" + "_" + fit_var)
            else:
                #hist = f.Get(s + "_" + corr +"_" + fit_var)
                hist = f.Get(s + "_" + fit_var)
        else:
            hist = f.Get(s + "_" + fit_var)
        hist.Sumw2()
        print( s, hist.Integral() )
        histos[s] = hist

    bkg = histos['TTJets_bkg'].Clone()
    bkg.Add(histos['WZJets'])
    bkg.Add(histos['STJets'])

    data = histos["Data"].Clone()
    data.Add(bkg, -1.)

    signal = histos["TTJets_signal"]

    qcd = histos["QCD"]

    x = ROOT.RooRealVar("x","x",0.,350.)
    rooSignal =  ROOT.RooDataHist("signal","signal",ROOT.RooArgList( x ), signal)
    rooBkg = ROOT.RooDataHist("bkg","bkg", ROOT.RooArgList( x ), qcd)
    signal_pdf = ROOT.RooHistPdf("signal","signal",ROOT.RooArgSet( x ), rooSignal)
    bkg_pdf = ROOT.RooHistPdf("bkg","bkg",ROOT.RooArgSet( x ), rooBkg)

    c0 = ROOT.RooRealVar("c0","c0",0.5,0.,1.)
    pdf = ROOT.RooAddPdf("pdf","pdf", signal_pdf,bkg_pdf, c0)

    dataFit = ROOT.RooDataHist("data","data",ROOT.RooArgList( x ), data);

    fitResult = pdf.fitTo(dataFit)
    #print( fitResult )

    sf_tt_sig = (c0.getVal() * data.Integral()) / signal.Integral() 
    sf_tt_sig_err = (c0.getError() * data.Integral()) / signal.Integral()
    
    sf_qcd = ((1-c0.getVal())*data.Integral()) / qcd.Integral()
    sf_qcd_err = (c0.getError() * data.Integral()) / qcd.Integral()
    
    
    print( "scale factor TTbar tau(h) QQ ", sf_tt_sig, "+-", sf_tt_sig_err )
    print( "scale factor MultiJet ", sf_qcd, "+-", sf_qcd_err )

    return sf_tt_sig, sf_qcd

"""
print bkg.Integral()

print histos["data"].Integral()

print data.Integral()
"""
