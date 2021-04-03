import ROOT
#from tdrstyle import setTDRStyle
from CMS_lumi import CMS_lumi

#ROOT.gROOT.SetBatch()

"""
def setStyle():

    ROOT.gROOT.Reset()
    ROOT.gROOT.SetStyle('Plain')
    ROOT.gStyle.SetPalette(1)
    ROOT.gStyle.SetOptStat(0)
    #ROOT.gROOT.SetBatch()
    # don't pop up canvases
    ROOT.TH1.SetDefaultSumw2()
    #    ROOT.TH1.AddDirectory(False)
    ROOT.TGaxis.SetMaxDigits(3)
    #    gROOT.ForceStyle()
    setTDRStyle()
"""

def plot(infile, var, xtitle, sample_names, outpath, sfs = None, corr="central", post_fit = False):

    #setStyle()

    #if not os.path.exists("stacks"):
    #    os.system("mkdir -p stacks")

    f = ROOT.TFile(infile, "READ")

    histo = []
    h = ROOT.TH1F()
    h_err = ROOT.TH1F()
    h_bkg_err = ROOT.TH1F()
    err_up = []
    err_down =  []

    #histoname = "h_"+ self.lep  + '_' + self.category + '_' + directory
    #stackname = "Stack_"+ self.lep  + '_' + self.category + '_' + directory
    canvasname = "canvas"

    stack = ROOT.THStack("stack", "stack")
    leg_stack = ROOT.TLegend(0.75,0.75,0.9,0.9)
    signal=True
    infile =[]
    #Adding data file
    
    if post_fit == False:
        hdata = f.Get( "Data_" + var )
        hdata.SetLineColor(ROOT.kBlack)
        hdata.SetMarkerStyle(20)
        hdata.SetMarkerSize(0.9)
    else:
        #file_path = "/eos/user/l/llayer/cmsopen/columnar/syst_variation/histos/harvester_input.root"
        #f_dummy = ROOT.TFile(file_path, "READ")
        data_gr = f.Get("shapes_fit_s/signal_region/data")
        h_sig = f.Get('shapes_fit_s/signal_region/total_signal')
        hdata = h_sig.Clone("Data")
        n_points = data_gr.GetN()
        for i in range(n_points):
            y = data_gr.GetPointY(i)
            err_y = (data_gr.GetErrorYhigh(i) + data_gr.GetErrorYlow(i))/2.
            hdata.SetBinContent(i+1, y)
            hdata.SetBinError(i+1, err_y)
        
        hdata.SetLineColor(ROOT.kBlack)
        hdata.SetMarkerStyle(20)
        hdata.SetMarkerSize(0.9) 
        """
        hdata.GetXaxis().SetRangeUser(0,15)
        gr_data = f.Get("shapes_fit_s/signal_region/data")
        """
        

    #setTDRStyle()
    leg_stack.AddEntry(hdata, "Data", "lp")

    hratio = hdata.Clone("")
    hratio.Reset("ICES")

    # Loop over samples
    colors = [ROOT.kOrange +7, ROOT.kOrange, ROOT.kRed, ROOT.kGreen, ROOT.kBlue]

    for s, c in zip(sample_names, colors):

        # Get histogram
        if post_fit == False:
            if s == "QCD":
                hist = f.Get(s + '_' + var)#path + '/' + s.label)
            else:
                hist = f.Get(s + '_' + corr + "_" + var)#path + '/' + s.label)
                #hist = f.Get(s + '_' + var)#path + '/' + s.label)
        else:
            hist = f.Get("shapes_fit_s/signal_region/" + s)
        print( s, type(hist) )
        hist.SetOption("HIST SAME")
        hist.SetLineColor(ROOT.kBlack)
        hist.SetTitle("")
        hist.SetName(s)
        #hist.SetFillColor(c)
        hist.SetFillColorAlpha(c, 0.6)

        if sfs is not None:
            if s in sfs.keys():
                print("Scaling", s, "by", sfs[s])
                hist.Scale(sfs[s])
        
        stack.Add(hist.Clone(""))
        hratio.Add(hist)
        leg_stack.AddEntry(hist, hist.GetName(), "f")


    #style options
    leg_stack.SetNColumns(2)
    leg_stack.SetFillColor(0)
    leg_stack.SetFillStyle(0)
    leg_stack.SetTextFont(42)
    leg_stack.SetTextSize(20)
    leg_stack.SetBorderSize(0)
    leg_stack.SetTextSize(0.055)
    c1 = ROOT.TCanvas(canvasname,"c1",50,50,700,600)
    c1.SetFillColor(0)
    c1.SetBorderMode(0)
    c1.SetFrameFillStyle(0)
    c1.SetFrameBorderMode(0)
    c1.SetLeftMargin( 0.12 )
    c1.SetRightMargin( 0.9 )
    c1.SetTopMargin( 1 )
    c1.SetBottomMargin(-1)
    c1.SetTickx(1)
    c1.SetTicky(1)
    c1.cd()

    pad1= ROOT.TPad("pad1", "pad1", 0, 0.31 , 1, 1)
    pad1.SetTopMargin(0.1)
    pad1.SetBottomMargin(0)
    pad1.SetLeftMargin(0.12)
    pad1.SetRightMargin(0.05)
    pad1.SetBorderMode(0)
    pad1.SetTickx(1)
    pad1.SetTicky(1)
    pad1.Draw()
    pad1.cd()

    #leg_stack.SetX1(.46)
    #leg_stack.SetY1(.56)
    #leg_stack.SetX2(.95)
    #leg_stack.SetY2(.88)

    leg_stack.SetX1(.5)
    leg_stack.SetY1(.6)
    leg_stack.SetX2(.95)
    leg_stack.SetY2(.88)

    maximum = max(stack.GetMaximum(),hdata.GetMaximum())
    minimum = min(stack.GetMinimum(),hdata.GetMinimum())
    #stack.SetMaximum(maximum*1.55)
    stack.SetMaximum(maximum*15)
    stack.SetMinimum(1.)
    stack.Draw("HIST")
    stack.GetYaxis().SetTitle("Events / bin")
    stack.GetYaxis().SetTitleFont(42)
    stack.GetXaxis().SetLabelOffset(1.8)
    stack.GetYaxis().SetTitleOffset(0.8)
    stack.GetXaxis().SetLabelSize(0.15)
    stack.GetYaxis().SetLabelSize(0.07)
    stack.GetYaxis().SetTitleSize(0.07)
    stack.SetTitle("")
    pad1.SetLogy(1)

    leg_stack.Draw("same")
    h_err = stack.GetStack().Last().Clone("h_err")
    h_err.SetLineWidth(100)
    h_err.SetFillStyle(3154)
    h_err.SetMarkerSize(0)
    h_err.SetFillColor(ROOT.kGray+2)
    
    if post_fit == False:
        h_err.Draw("e2same0")

    #if post_fit == False:
    hdata.Draw("eSAMEpx0")
    #else:
    #    gr_data.Draw('PSAME')

    
    lumi_sqrtS = " 4.2 fb^{-1}  (7 TeV)"
    #    print lumi_sqrtS
    iPos = 0
    # writing the lumi information and the CMS "logo"
    # Ratio Check HERE
    CMS_lumi(pad1, lumi_sqrtS, iPos)
    

    hratio=stack.GetStack().Last()

    c1.cd()
    pad2= ROOT.TPad("pad2", "pad2", 0, 0.01 , 1, 0.30)
    pad2.SetTopMargin(0.05)
    pad2.SetBottomMargin(0.45)
    pad2.SetLeftMargin(0.12)
    pad2.SetRightMargin(0.05)
    ROOT.gStyle.SetHatchesSpacing(2)
    ROOT.gStyle.SetHatchesLineWidth(2)
    c1.cd()
    pad2.Draw()
    pad2.cd()
    ratio = hdata.Clone("ratio")
    ratio.SetLineColor(ROOT.kBlack)
    ratio.SetMaximum(1.5)
    ratio.SetMinimum(0.5)
    ratio.Sumw2()
    ratio.SetStats(0)
    #hratio.Sumw2()
    ratio.Divide(hratio)
    ratio.SetMarkerStyle(20)
    ratio.SetMarkerSize(0.9)
    ratio.Draw("epx0e")
    ratio.SetTitle("")

    h_bkg_err = hratio.Clone("h_err")
    h_bkg_err.Reset()
    h_bkg_err.Sumw2()

    for i in range(1,hratio.GetNbinsX()+1):
        h_bkg_err.SetBinContent(i,1)
        if(hratio.GetBinContent(i)):
            h_bkg_err.SetBinError(i, (hratio.GetBinError(i)/hratio.GetBinContent(i)))
        else:
            h_bkg_err.SetBinError(i, 0)
    h_bkg_err.SetLineWidth(100)
#    h_bkg_err.SetFillStyle(3001)
    h_bkg_err.SetMarkerSize(0)
    h_bkg_err.SetFillColor(ROOT.kGray+1)
    if post_fit == False:
        h_bkg_err.Draw("e20same")

    #f1 = ROOT.TLine(0, 1., 1,1.)
    print("DDDD", ratio.GetXaxis().GetXmax())
    f1 = ROOT.TLine(ratio.GetXaxis().GetXmin(), 1., ratio.GetXaxis().GetXmax(),1.)
    f1.SetLineColor(ROOT.kBlack)
    f1.SetLineStyle(ROOT.kDashed)
    f1.Draw("same")

    #    ratio.Draw("esamepx0e0") #0epx0same
    #print "ciao 3"
    #ratio.Sumw2()
    ratio.GetYaxis().SetTitle("Data / Bkg")
    ratio.GetYaxis().SetNdivisions(503)
    ratio.GetXaxis().SetLabelFont(42)
    ratio.GetYaxis().SetLabelFont(42)
    ratio.GetXaxis().SetTitleFont(42)
    ratio.GetYaxis().SetTitleFont(42)
    ratio.GetXaxis().SetTitleOffset(1.1)
    ratio.GetYaxis().SetTitleOffset(0.35)
    ratio.GetXaxis().SetLabelSize(0.15)
    ratio.GetYaxis().SetLabelSize(0.15)
    ratio.GetXaxis().SetTitleSize(0.16)
    ratio.GetYaxis().SetTitleSize(0.16)
    ratio.GetYaxis().SetRangeUser(0.5,1.5)
    ratio.GetXaxis().SetTitle(xtitle)
    ratio.GetXaxis().SetLabelOffset(0.04)
    ratio.GetYaxis().SetLabelOffset(0.01)
    ratio.Draw("epx0esame")
    
    ROOT.gStyle.SetHistMinimumZero() 
     
    c1.cd()
    ROOT.TGaxis.SetMaxDigits(3)
    c1.RedrawAxis()
    pad2.RedrawAxis()
    c1.Update()
    #    fout_name = "prova/"+outfile
    #c1.Print(outdir + '/' + "Stack/"+canvasname+".pdf")
    if (sfs is not None) | (post_fit == True):
        c1.Print(outpath + "/" + var + "_" + corr + "_" + "postfit.png")
    else:
        c1.Print(outpath + "/" + var + "_" + corr + ".png")
            
    f.Close()

if __name__ == "__main__":

    """
    variables = [
        {"var_name" : "MET_met", "bins" : 30, "xlow" : 0., "xup" : 400, "xtitle" : "MET [GeV]"}
    ]
    sample_names = ["TTJets_bkg", "WZJets", "STJets", "QCD", "TTJets_signal"]
    for var in variables:
        plot( "histos_test.root", var["var_name"], var["xtitle"], sample_names, corr = "centJER" )
    """
    infile = "fitDiagnosticsTest.root"
    sample_names = ["TTJets_bkg", "WZJets", "STJets", "QCD", "TTJets_signal"]
    plot( infile, "bdt", "bdt", sample_names, ".", post_fit = True )

    
    