import ROOT
import argparse

ROOT.gROOT.SetBatch(1)
ROOT.gStyle.SetOptStat(0)

parser = argparse.ArgumentParser(description='ROOT plotter')
parser.add_argument('--me', dest='meroot', help='ME level ROOT file')
parser.add_argument('--ps', dest='psroot', help='PS level ROOT file')
args = parser.parse_args()

fileME = ROOT.TFile.Open(args.meroot)
filePS = ROOT.TFile.Open(args.psroot)

print "########## Running the Plotter ##########"
print "# ME : "+args.meroot
print "# PS : "+args.psroot

myTreeME = fileME.Get("LHEF")
myTreePS = filePS.Get("Events")

histME = []; histPS = [];
hists = ["massll", "pTll", "pTl"];
histMEmassll = ROOT.TH1D("histMEmassll","histMEmassll",40,0.,200.)
histPSmassll = ROOT.TH1D("histPSmassll","histPSmassll",40,0.,200.)
histMEpTll = ROOT.TH1D("histMEpTll","histMEpTll",20,0.,50.)
histPSpTll = ROOT.TH1D("histPSpTll","histPSpTll",20,0.,50.)
histMEpTl = ROOT.TH1D("histMEpTl","histMEpTl",20,0.,100.)
histPSpTl = ROOT.TH1D("histPSpTl","histPSpTl",20,0.,100.)

for ievent in range(0, myTreeME.GetEntries()):

    if(ievent < 10000):
        if(ievent%1000==0): print "Running over ME event "+str(ievent)+"/"+str(myTreeME.GetEntries())
    else:
        if(ievent%5000==0): print "Running over ME event "+str(ievent)+"/"+str(myTreeME.GetEntries())

    myTreeME.GetEntry(ievent)

    iEle = []; iPos = [];
    for i in range(0,int(myTreeME.GetLeaf("Particle_size").GetValue())):
        if (myTreeME.GetLeaf("Particle.PID").GetValue(i) == 11):
                iEle.append(i)
        if (myTreeME.GetLeaf("Particle.PID").GetValue(i) == -11):
                iPos.append(i)

    Ele = ROOT.TLorentzVector(); Pos = ROOT.TLorentzVector();
    Ele.SetPxPyPzE(myTreeME.GetLeaf("Particle.Px").GetValue(iEle[0]),myTreeME.GetLeaf("Particle.Py").GetValue(iEle[0]),myTreeME.GetLeaf("Particle.Pz").GetValue(iEle[0]),myTreeME.GetLeaf("Particle.E").GetValue(iEle[0]))
    Pos.SetPxPyPzE(myTreeME.GetLeaf("Particle.Px").GetValue(iPos[0]),myTreeME.GetLeaf("Particle.Py").GetValue(iPos[0]),myTreeME.GetLeaf("Particle.Pz").GetValue(iPos[0]),myTreeME.GetLeaf("Particle.E").GetValue(iPos[0]))

    candZ = ROOT.TLorentzVector()
    candZ = Ele + Pos

    histMEmassll.Fill(candZ.M())
    histMEpTll.Fill(candZ.Pt())
    histMEpTl.Fill(Ele.Pt())
    histMEpTl.Fill(Pos.Pt())

histME.append(histMEmassll)
histME.append(histMEpTll)
histME.append(histMEpTl)

psevcount = 0
for event in myTreePS:

    if(psevcount < 10000):
        if(psevcount%1000==0): print "Running over PS event "+str(psevcount)+"/"+str(myTreePS.GetEntries())
    else:
        if(psevcount%5000==0): print "Running over PS event "+str(psevcount)+"/"+str(myTreePS.GetEntries())
    psevcount += 1

    iEle = []; iPos = [];
    for i in range(0,event.Event_numberP):

        if (event.Particle_status[i] == 1):
            if (event.Particle_pid[i] == 11):
                iEle.append(i)
            if (event.Particle_pid[i] == -11):
                iPos.append(i)

    Ele = ROOT.TLorentzVector(); Pos = ROOT.TLorentzVector();
    Ele.SetPxPyPzE(event.Particle_px[iEle[0]],event.Particle_py[iEle[0]],event.Particle_pz[iEle[0]],event.Particle_energy[iEle[0]])
    Pos.SetPxPyPzE(event.Particle_px[iPos[0]],event.Particle_py[iPos[0]],event.Particle_pz[iPos[0]],event.Particle_energy[iPos[0]])

    candZ = ROOT.TLorentzVector()
    candZ = Ele + Pos
    
    histPSmassll.Fill(candZ.M())
    histPSpTll.Fill(candZ.Pt())
    histPSpTl.Fill(Ele.Pt())
    histPSpTl.Fill(Pos.Pt())

histPS.append(histPSmassll)
histPS.append(histPSpTll)
histPS.append(histPSpTl)

for iHist in range(0,len(hists)):

    histME[iHist].SetLineColor(ROOT.kRed+1)
    histME[iHist].SetFillColorAlpha(ROOT.kRed+1,0.3)

    histPS[iHist].SetLineColor(ROOT.kGreen+1)
    histPS[iHist].SetFillColorAlpha(ROOT.kGreen+1,0.3)

for iHist in range(0,len(hists)):

    canvas = ROOT.TCanvas("canvas","canvas", 1000, 1000)

    histME[iHist].Draw("hist")
    histPS[iHist].Draw("histsame")

    histME[iHist].SetTitle("")
    histME[iHist].GetXaxis().SetTitle(hists[iHist])

    legend = ROOT.TLegend(0.6, 0.6, 0.8, 0.8)
    legend.AddEntry(histME[iHist], "ME", "lf")
    legend.AddEntry(histPS[iHist], "ME+PS", "lf")
    legend.Draw("same")

    canvas.SaveAs(hists[iHist]+".pdf")
    del canvas

