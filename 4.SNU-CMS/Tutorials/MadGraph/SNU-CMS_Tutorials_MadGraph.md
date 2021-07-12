Monte Carlo event generators are essential components of almost all experimental analyses and are also widely used by theorists and experiments to make predictions and preparations for future experiments. MCEG are divided into two big categories for its purposes : matrix-element (ME) level calculations and parton showerings (PS). In CMS, we use various generators such as POWHEG, SHERPA, PYTHIA, HERWIG, and MADGRAPH. Here, we will learn about MCEG using MadGraph-Pythia which are the most used generators in CMS. Useful document to read is in *https://arxiv.org/abs/1304.6677*  
  
**1. Setting up MadGraph and Pythia.**  
  
Install MadGraph from CMS database.  
```sh
wget https://cms-project-generators.web.cern.ch/cms-project-generators/MG5_aMC_v2.6.5.tar.gz
tar -xavf MG5_aMC_v2.6.5.tar.gz
cd MG5_aMC_v2_6_5
```
  
Adjust settings in `input/mg5_configuration.txt`. Make the changes listed below.  
- `# auto_update = 7` to `auto_update = 999999` to disable auto updates
- `# text_editor = None` to `text_editor = [choose either vi OR emacs]` to select the default text editor.
- `# automatic_html_opening = True` to `automatic_html_opening = False` to disable html opening.
  
Now execute MadGraph and install Pythia packages.  
```sh
# Executing MadGraph will open the MadGraph shell MG5_aMC>.
./bin/mg5_aMC

# Install Pythia and the MadGraph-Pythia interface. Compilation itself automatically. Tested in cms1 server (147.47.242.34) 2021/06/24.
install pythia8
# If you execute the command above it will ask whether you want to install LHAPDF6. DO NOT ATTEMPT TO INSTALL LHAPDF as the compilation is not so easy.
```
If Pythia is installed and compiled successfully, the setups to start the tutorial are ready.  
  
  
**2. Running MadGraph and Pythia.**  
  
Initiate the DY physics process to generate events.  
```sh
# Type in the targeting physics process. Here DY is taken as an example.
generate p p > e+ e- 
# Setup the working directory named Tutorial1
output Tutorial1
launch
```
  
Configure the external programs.  
```sh
# Turn on PS generator to shower the events.
shower=Pythia8

# It should look like below if Pythia is compiled correctly. Otherwise, check if the Pythia compilation was successful or not.
# /===========================================================================\
# | 1. Choose the shower/hadronization program     shower = Pythia8           |
# | 2. Choose the detector simulation program    detector = Not Avail.        |
# | 3. Choose an analysis package (plot/convert) analysis = Not Avail.        |
# | 4. Decay onshell particles                    madspin = OFF               |
# | 5. Add weights to events for new hypp.       reweight = Not Avail.        |
# \===========================================================================/
```
For advanced studies, it's worth checking the links below.  
- Detector simulation program : Delphes *https://arxiv.org/abs/1307.6346*
- Analysis package : MadAnalysis *https://arxiv.org/abs/1206.1599*
- Resonance decayer : MadSpin *https://cp3.irmp.ucl.ac.be/projects/madgraph/wiki/MadSpin*
- Reweight : *https://www.google.com/search?client=firefox-b-d&q=reweight+madgraph*
  
Configure the physics settings.  
```sh
# /------------------------------------------------------------\
# |  1. param   : param_card.dat                               |
# |  2. run     : run_card.dat                                 |
# |  3. pythia8 : pythia8_card.dat                             |
# \------------------------------------------------------------/

# Things that could be changed in the cards.
# param : particle mass, particle width, physics constant, etc.
# run : phase space cut (dilepton mass, lepton pt, jet pt, ...), beam setting (COM energy, PDF set, ...), number of events, etc.
# pythia8 : NOT USUALLY MODIFIED, shower settings.

# First check which phase space cuts are applied by looking at the run card.
# Remove all cuts from the run card
set no_parton_cut
# Now check whether the cuts are really removed or not.

# Set m(ll) > 10 GeV cut
set run_card mmll 10
# Now check whether the cut is set or not. It should look like below.
#   10.0  = mmll ! min invariant mass of l+l- (same flavour) lepton pair

# Disable systematic runs as LHAPDF is not installed
set run_card use_syst False
```
  
Finally, you can launch the jobs and generate events. It will first give you the cross sections from ME level calculations and then the results from PS.  
```sh
# Below is the ME, results are stored in Tutorial1/Events/run_01/unweighted_events.lhe.gz.
#   === Results Summary for run: run_01 tag: tag_1 ===
#
#     Cross-section :   6382 +- 10.81 pb
#     Nb of events :  10000

# Below is the PS, results are stored in Tutorial1/Events/run_01/tag_1_pythia8_events.hepmc.gz
#  === Results Summary for run: run_01 tag: tag_1 ===
#
#     Cross-section :   6382 +- 10.81 pb
#     Nb of events :  10000

# Now exit from the MadGraph shell.
exit

# Check out the outputs from MadGraph and Pythia.
ls -lah Tutorial1/Events/run_01/
# -rw-r--r-- 1 shjeon shjeon 461M Jun 24 21:12 tag_1_pythia8_events.hepmc.gz
# -rw-r--r-- 1 shjeon shjeon 1.4M Jun 24 21:12 unweighted_events.lhe.gz
```
  
> Ex1) Why are the `tag_1_pythia8_events.hepmc.gz` and `unweighted_events.lhe.gz` so different in size?  
  
> Ex2) Why does the cross section stays the same before and after shower? Multiple photons could be attached to the outgoing electrons by PS. Why won't this add more cross sections to ME?  
  
  
**3. Checking the results.**  
  
ROOT file format is the most widely used data analysis framework. Since ME and PS output formats are LHE and HepMC, respectively, LHE/HepMC -> ROOT conversion is needed.  
```sh
# CMSSW setup for better python environment (cms1 default is rusty).
source /cvmfs/cms.cern.ch/cmsset_default.sh
cmsrel CMSSW_9_0_0
cd CMSSW_9_0_0/src/
cmsenv

# Convert LHE to ROOT.
# LHE file needs to be unzipped to run this code.
wget http://madgraph.physics.illinois.edu/Downloads/ExRootAnalysis/ExRootAnalysis_V1.1.5.tar.gz
tar -xavf ExRootAnalysis_V1.1.5.tar.gz
cd ExRootAnalysis
make
./ExRootLHEFConverter <input.lhe> ME_DY.root
mv ME_DY.root ../
cd ..

# Convert HepMC to ROOT.
# HepMC file needs to be unzipped to run this code.
git clone git@github.com:hbprosper/hepmc2root.git
cd hepmc2root
source setup.sh
hepmc2root.py <input.hepmc> PS_DY.root
mv PS_DY.root ../
cd ..
```
   
Now using ROOT files you can select events and draw the histograms. Here we will select events with 2 electrons and compare ME and PS outputs' pT(ll) and m(ll) distributions.  
```sh
# Get the plotter codes.
wget https://gitlab.com/snucms/project/-/raw/main/4.SNU-CMS/Tutorials/MadGraph/plotter.py

# Run the plotter to get the histograms.
python plotter.py --me ME_DY.root --ps PS_DY.root
```
  
> Ex3) The codes will automatically draw m(ll), pT(ll), pT(l) histograms and now you can compare before and after PS. Among 3 histograms, which shows the most difference? What could be the reason for it? (HINT : Connect your answer with the definition of parton distribution function)  
  
> Ex4) Repeat the above studies twice varying the m(ll) cuts to 5 and 50 GeV. How did the cross sections change? DY process should not be generated with m(ll) 0 GeV cut. Think of why based on the m(ll) histograms.  
  
> [TBD] Ex5) Assume you are doing a BSM search for the Z' particle that decays into pair of electrons where m(Z') = 1000 GeV with a very narrow resonance width. And you are now considering to generate SM DY process for background studies.  
