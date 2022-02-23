# SKFlatAnalyzer Tutorial
#### [Contents]
- Introduction
- Environment setup
- Job submission
- Comparing data & mc

## Introduction

## Environment setup
Follow the instruction in https://github.com/CMSSNU/SKFlatAnalyzer
Check whether the test job runs fine. Tips: you can use --reduction option for the test run:
```
SKFlat.py -a ExampleRun -i TTLJ_powheg -n 1 -y 2017 --reduction 100 &
```

In this tutorial, we will use TutorialRun analyzer. I put Tutorial_reco_tt.h and Tutorial_reco_tt.C in the TutorialRun directory. Let's move them to the SKFlatAnalyzer:
```
cd /data6/Users/${USER}/SKFlatAnalyzer
source setup.sh
export TUTORIALRUN_TT="<Your RecoTTLJ path>"
ex) export TUTORIALRUN_TT="/data6/Users/snu_public/4.SNU-CMS/Tutorials/SKFlatAnalyzer/RecoTTLJ"
cp ${TUTORIALRUN_TT}/TutorialRun/Tutorial_reco_tt.h $SKFlat_WD/Analyzers/include/
cp ${TUTORIALRUN_TT/TutorialRun/Tutorial_reco_tt.C $SKFlat_WD/Analyzers/src/
```
and add
```
#pragma link C++ class Tutorial_reco_tt+;
```
in $SKFlat_WD/Analyzers/include/Analyzers_LinkDef.h

Finally, compile the analyzer:
```
cd $SKFlat_WD
make
```

## Job Submission
You can use either TutorialRun/submit_tt.sh or submit directly in the SKFlatAnalyzer directory:
```
cd $SKFlat_WD
cp ${TUTORIALRUN_TT}/TutorialRun/submit_tt.sh .
cp ${TUTORIALRUN_TT}/TutorialRun/MCList_tutorial_tt.txt . 
SKFlat.py -a Tutorial_reco_tt -i SingleMuon -n 10 -y 2017 &
SKFlat.py -a Tutorial_reco_tt -l MCList_tutorial_tt.txt -n 30 -y 2017 &
```
It will submit the jobs to condor. To check whether your jobs are running okay, use:
```
condor_q $USER	# you can change $USER to your user name, for example condor_q choij
watch -n 10 condor_q $USER # to monitor job in every 10 seconds
```

After jobs are finished, it will send the outputs to the /data6/Users/${USER}/SKFlatOutput/Run2Legacy_v4/2017/Tutorial_reco_tt/... 
If any problem occurs, check the log file in /data6/Users/${USER}/SKFlatRunlog/...


## Comparing data & mc
You can directly compare the number of observed(from data) and expected(from MCs) events after tt reconstruction.
But it is better to compare the histograms of the data and the (stack of) MCs. 
In this example, we will use pyROOT3 with python >= 3.6 to draw the plots. 
One way to set the environment in the tamsa server is using the CMSSW (CMS-SoftWare):
```
cd /data6/Users/${USER}/
source /cvmfs/cms.cern.ch/cmsset_default.sh
cmsrel CMSSW_11_3_0_pre2
cd CMSSW_11_3_0_pre2/src
cmsenv
```
In the local machine, I recommend to use anaconda environment since it automatically install the pyROOT3 while installing ROOT6(Additionally, you can isolate each setup using virtual environment).
For brew(on mac) or yum/dnf(on centos/fedora), you should install pyROOT3 additionally after the installation of ROOT(for example, `dnf install python3-root`. Please check the information in https://root.cern/install/.

Let's move the output root files from SKFlatAnalyzer to the CMSSW directory. Also, let's get the plotting scripts from $TUTORIALRUN_TT directory.
```
cp -r /data6/Users/${USER}/SKFlatOutput/Run2Legacy_v4/Tutorial_reco_tt/2017/ ${CMSSW_BASE}
cp -r ${TUTORIALRUN_TT}/Plotter ${CMSSW_BASE}
cp ${TUTORIALRUN_TT}/plot.py ${CMSSW_BASE}
cp ${TUTORIALRUN_TT}/hadd.sh ${CMSSW_BASE}/2017
```

hadd(add up) the outputs of the TutorialRun
```
cd ${CMSSW_BASE}
mv 2017 TutorialOutputs && cd TutorialOutputs
cp ${TUTORIALRUN_TT}/hadd.sh .
source hadd.sh
cd ..
```

Finally, let's draw the plot:
```
python3 plot.py
```
The script will create the hadronic W mass plot named DataAndMC_POGTightWithTightIso_Central_Chi2Cut_had_W_mass_POGTightWithTightIso_Central.png


TODO: Add tutorials to set weights such as ID/Trigger SFs, PileUp weights, B-tagging SFs...

> Copied from Z mass tutorial by Jin Choi (choij@cern.ch)
> Modified by Byunghun Oh (bh.oh@cern.sh), Sep 28th, 2021
