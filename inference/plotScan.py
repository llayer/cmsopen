import ROOT
from array import array

ROOT.gStyle.SetOptStat(0)

def get_graph(file_path):

    r_obs = array('d',[])
    nll_obs = array('d',[])
    zeros = array('d',[])

    #rootfile = "higgsCombineTest.MultiDimFit.mH120.root"
    rootfile = file_path

    f_obs = ROOT.TFile(rootfile,"READ")
    t_obs = f_obs.Get("limit")
    for i in xrange(1,t_obs.GetEntries()):
        t_obs.GetEntry(i)
        r_obs.append(t_obs.r)
        nll_obs.append(2.0*t_obs.deltaNLL)
        zeros.append(0.0)

    v_r_obs = ROOT.TVectorD(len(r_obs),r_obs)
    v_nll_obs = ROOT.TVectorD(len(nll_obs),nll_obs)
    v_zeros = ROOT.TVectorD(len(zeros),zeros)

    gr_obs = ROOT.TGraphAsymmErrors(v_r_obs,v_nll_obs,v_zeros,v_zeros,v_zeros,v_zeros)

    return gr_obs, r_obs

def plotNLL(cat="signal"):

    gr_syst, r_obs = get_graph("out/multi_syst.root")
    gr_stat, _ = get_graph("out/multi_stat.root")

    c = ROOT.TCanvas("c","c",800, 800)
    c.SetRightMargin(0.06)
    c.SetLeftMargin(0.15)
    c.SetBottomMargin(0.15)
    c.SetTopMargin(0.15)

    dummy = ROOT.TH1D("dummy","dummy", 1, r_obs[0], r_obs[-1])
    dummy.SetBinContent(1,0.0)
    dummy.GetXaxis().SetTitle('#sigma/#sigma_{SM}')
    dummy.GetXaxis().SetTitleSize(0.05)
    dummy.GetYaxis().SetTitle('-2 #Delta lnL')
    dummy.GetYaxis().SetTitleSize(0.05)
    dummy.GetYaxis().SetTitleOffset(0.8)
    dummy.GetXaxis().SetLabelSize(0.05)
    dummy.GetXaxis().SetNdivisions(505)
    dummy.GetYaxis().SetLabelSize(0.05)
    dummy.GetYaxis().SetNdivisions(505)
    dummy.SetLineColor(0)
    dummy.SetLineWidth(0)
    dummy.SetFillColor(0)
    dummy.SetMinimum(0.0)
    dummy.SetMaximum(5.0)
    dummy.SetTitle("")
    dummy.Draw()

    #latexf = TLatex()
    #latexf.SetTextSize(0.5*c.GetTopMargin())
    #latexf.SetTextColor(2)
    f1 = ROOT.TF1("f1","1.0",r_obs[0], r_obs[-1])
    f1.SetLineColor(2)
    f1.SetLineWidth(2)
    f1.Draw("lsame")
    #latexf.DrawLatex(2.5, 1.1,"68% CL")
    f2 = ROOT.TF1("f1","3.84",r_obs[0], r_obs[-1])
    f2.SetLineColor(2)
    f2.SetLineWidth(2)
    f2.Draw("lsame")
    #latexf.DrawLatex(2.5, 3.94,"95% CL")

    gr_syst.SetLineColor(ROOT.kGreen+2)
    gr_syst.SetLineWidth(3)
    gr_syst.Draw("Lsame")

    gr_stat.SetLineColor(ROOT.kBlue)
    gr_stat.SetLineWidth(3)
    gr_stat.Draw("Lsame")

    leg = ROOT.TLegend(0.7, 0.3, .88, .5);
    leg.AddEntry(gr_stat, "Stat", "l");
    leg.AddEntry(gr_syst, "Stat + Syst", "l");
    leg.SetBorderSize(0)
    leg.Draw("SAME")

    """
    latex2 = ROOT.TLatex()
    latex2.SetNDC()
    latex2.SetTextSize(0.5*c.GetTopMargin())
    latex2.SetTextFont(42)
    latex2.SetTextAlign(31) # align right
    latex2.DrawLatex(0.7, 0.91, cat)
    """

    ROOT.gPad.RedrawAxis()

    c.Print("nll.png")


plotNLL()