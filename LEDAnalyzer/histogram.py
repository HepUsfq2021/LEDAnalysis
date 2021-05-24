# Based on https://github.com/cms-opendata-analyses/HiggsTauTauNanoAODOutreachAnalysis
# Implementation of the plotting step of the analysis
#
# The plotting combines the histograms to plots which allow us to study the
# inital dataset based on observables motivated through physics.


import ROOT
ROOT.gROOT.SetBatch(True)


# Specify the color for each process
colors = {
        "data": ROOT.TColor.GetColor("#00A88F"),
        "sim": ROOT.TColor.GetColor("#BF2229"),
        }


# Retrieve a histogram from the input file based on the process and the variable
# name
def getHistogram(tfile, name):
    name = "{}".format(name)
    h = tfile.Get(name)
    if not h:
        raise Exception("Failed to load histogram {}.".format(name))
    return h

def main():
    tfileData = ROOT.TFile("histoData.root", "READ")
    tfileSim = ROOT.TFile("histoSim.root", "READ")

    # Styles
    ROOT.gStyle.SetOptStat(0)

    ROOT.gStyle.SetCanvasBorderMode(0)
    ROOT.gStyle.SetCanvasColor(ROOT.kWhite)
    ROOT.gStyle.SetCanvasDefH(600)
    ROOT.gStyle.SetCanvasDefW(600)
    ROOT.gStyle.SetCanvasDefX(0)
    ROOT.gStyle.SetCanvasDefY(0)

    ROOT.gStyle.SetPadTopMargin(0.08)
    ROOT.gStyle.SetPadBottomMargin(0.13)
    ROOT.gStyle.SetPadLeftMargin(0.16)
    ROOT.gStyle.SetPadRightMargin(0.05)

    ROOT.gStyle.SetHistLineColor(1)
    ROOT.gStyle.SetHistLineStyle(0)
    ROOT.gStyle.SetHistLineWidth(1)
    ROOT.gStyle.SetEndErrorSize(2)
    ROOT.gStyle.SetMarkerStyle(20)

    ROOT.gStyle.SetOptTitle(0)
    ROOT.gStyle.SetTitleFont(42)
    ROOT.gStyle.SetTitleColor(1)
    ROOT.gStyle.SetTitleTextColor(1)
    ROOT.gStyle.SetTitleFillColor(10)
    ROOT.gStyle.SetTitleFontSize(0.05)

    ROOT.gStyle.SetTitleColor(1, "XYZ")
    ROOT.gStyle.SetTitleFont(42, "XYZ")
    ROOT.gStyle.SetTitleSize(0.05, "XYZ")
    ROOT.gStyle.SetTitleXOffset(1.00)
    ROOT.gStyle.SetTitleYOffset(1.60)

    ROOT.gStyle.SetLabelColor(1, "XYZ")
    ROOT.gStyle.SetLabelFont(42, "XYZ")
    ROOT.gStyle.SetLabelOffset(0.007, "XYZ")
    ROOT.gStyle.SetLabelSize(0.04, "XYZ")

    ROOT.gStyle.SetAxisColor(1, "XYZ")
    ROOT.gStyle.SetStripDecimals(True)
    ROOT.gStyle.SetTickLength(0.03, "XYZ")
    ROOT.gStyle.SetNdivisions(510, "XYZ")
    ROOT.gStyle.SetPadTickX(1)
    ROOT.gStyle.SetPadTickY(1)

    ROOT.gStyle.SetPaperSize(20., 20.)
    ROOT.gStyle.SetHatchesLineWidth(5)
    ROOT.gStyle.SetHatchesSpacing(0.05)

    ROOT.TGaxis.SetExponentOffset(-0.08, 0.01, "Y")

    # Simulation
    sim = getHistogram(tfileSim, "invMassSim")

    # Data
    data = getHistogram(tfileData, "invMassData")
    
    # Draw histograms
    data.SetLineColor(colors["data"])
    sim.SetLineColor(colors["sim"])

    stack = ROOT.THStack("", "")
    stack.Add(data)
    stack.Add(sim)

    c = ROOT.TCanvas("", "", 600, 600)
    c.SetLogy()
    stack.Draw("hist")
    stack.GetXaxis().SetTitle("M_{e^{-}e^{+}}[GeV]")
    stack.GetYaxis().SetTitle("Events")
    stack.SetMaximum(max(stack.GetMaximum(), data.GetMaximum()) * 1.4)
    stack.SetMinimum(1.0)

    data.Draw("HIST SAME")
    sim.Draw("HIST SAME")

    # Add legend
    legend = ROOT.TLegend(0.4, 0.73, 0.90, 0.88)
    legend.SetNColumns(1)
    legend.AddEntry(data, "Z/#gamma*#rightarrow ee", "l")
    legend.AddEntry(sim, "ADD, #Lambda_T = 2.8TeV", "l")
    legend.SetBorderSize(0)
    legend.Draw()

    # Add title
    latex = ROOT.TLatex()
    latex.SetNDC()
    latex.SetTextSize(0.04)
    latex.SetTextFont(42)
    latex.DrawLatex(0.6, 0.935, "2.1 fb^{-1} (2011, 7 TeV)")
    latex.DrawLatex(0.16, 0.935, "#bf{CMS Open Data}")

    # Save
    c.SaveAs("ADD3.pdf")
    c.SaveAs("ADD3.png")

main()