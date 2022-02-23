import sys
sys.path.remove('/opt/ohpc/pub/apps/root_6_12_06/lib')

from ROOT import TFile
from Plotter.DataAndMC import DataAndMC

# NOTE: This script use pyROOT with python version >= 3.6 because of f-strings
# If you want to use lower python version, change f"...{var}" to "...{}".format(var)
# In the tamsa server, setting an environment with CMSSW_11_* will be sufficient

# set global variables
histkey = "POGTightWithTightIso_Central/Chi2Cut/had_W_mass_POGTightWithTightIso_Central"
DATASTREAM = "SingleMuon"
MCs = ["ST","TTLL", "TTLJ"]

# set plotting parameters
cvs_params = {"logy": False,
              "grid": False
             }
hist_params = {"x_title": "#it{M_{jj}}",
               "x_range": [0., 200.],
               "y_title": "Events",
               "ratio_title": "Data/MC",
               "ratio_range": [0.5, 1.5],
               #"rebin" : 5,
               }
info_params = {"info": "L^{int} = 41.5 fb^{-1}",
               "cms_text": "CMS",
               "extra_text": "Work in progress"
              }

# get histograms
def get_hist(sample, histkey):
	if sample == "data":
		fkey = f"TutorialOutputs/DATA/TutorialRun_{DATASTREAM}.root"
	else:
		fkey = f"TutorialOutputs/MCSamples/TutorialRun_{sample}.root"
	#print(fkey)
	#print(histkey)
	f = TFile.Open(fkey)
	h = f.Get(histkey)
	h.SetDirectory(0)
	return h

h_data = get_hist("data", histkey)
h_mc = dict()
for mc in MCs:
	h_mc[mc] = get_hist(mc, histkey)

plotter = DataAndMC(cvs_params, hist_params, info_params)
plotter.get_hists(h_data, h_mc)
plotter.combine()
plotter.save(f"DataAndMC_{histkey.replace('/', '_')}.png")
