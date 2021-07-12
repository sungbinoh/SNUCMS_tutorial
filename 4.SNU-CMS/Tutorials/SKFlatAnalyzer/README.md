# SKFlatAnalyzer Tutorial
#### [Contents]
- Introduction
- Environment setup
- Job submission
- Comparing data & mc

## Introduction
In this tutorial, we will check the existence of a massive neutral force carrier, the Z boson. 
The lifetime of the boson is very short,

```math
\Delta t \le 1/\Delta m \sim 10^{-26}s
```

which cannot be detected directly by the CMS detector. 
One way to check the existence is to reconstruct the lorentz invariant mass from its remnants. 
If the boson exists, we can detect a large increase of production rate around its mass,
   
```math
\sigma(q\bar{q} \rightarrow Z \rightarrow \mu^+ \mu^-) \sim |{1 \over {s - m_Z^2+im_z\Gamma_Z}}|^2
```

We call this method a resonant search.

Please confirm the contents of TutorialRun.C before the submission. It selects the events with two opposite sign muons with the invariant mass within 15 GeV of the Z mass.

There are also many processes that can make the same signature of the Drell-Yan process. 
In this tutorial, let's call the Drell-Yan process as the signal process and the other processes as backgrounds. Here are some examples of other processes:
- TTbar production
- Diboson production
- Single Top associated with W boson

In the data, it is not possible to clarify each event from certain process if many processes share the same final phase space. Therefore, it is also important to estimate how many events are coming from the background processes as well as from the signal process.

## Environment setup
Follow the instruction in https://github.com/CMSSNU/SKFlatAnalyzer
Check whether the test job runs fine. Tips: you can use --reduction option for the test run:
```
SKFlat.py -a ExampleRun -i DYJets -n 1 -y 2016 --reduction 100 &
```

In this tutorial, we will use TutorialRun analyzer. I put TutorialRun.h and TutorialRun.C in the TutorialRun directory. Let's move them to the SKFlatAnalyzer:
```
cd /data6/Users/${USER}/SKFlatAnalyzer
source setup.sh
export TUTORIALRUN="/data6/Users/snu_public/4.SNU-CMS/Tutorials/SKFlatAnalyzer"
cp ${TUTORIALRUN}/TutorialRun/TutorialRun.h $SKFlat_WD/Analyzers/include/
cp ${TUTORIALRUN/TutorialRun/TutorialRun.C $SKFlat_WD/Analyzers/src/
```
and add
```
#pragma link C++ class TutorialRun+;
```
in $SKFlat_WD/Analyzers/include/Analyzers_LinkDef.h

Finally, compile the analyzer:
```
cd $SKFlat_WD
make
```

## Job Submission
You can use either TutorialRun/submit.sh or submit directly in the SKFlatAnalyzer directory:
```
cd $SKFlat_WD
SKFlat.py -a TutorialRun -i SingleMuon -n 10 -y 2017 &
SKFlat.py -a TutorialRun -l MCList_tutorial.txt -n 30 -y 2017 &
```
It will submit the jobs to condor. To check whether your jobs are running okay, use:
```
condor_q $USER	# you can change $USER to your user name, for example condor_q choij
```

After jobs are finished, it will send the outputs to the /data6/Users/${USER}/SKFlatOutput/Run2Legacy_v4/2017/TutorialRun/... 
If any problem occurs, check the log file in /data6/Users/${USER}/SKFlatRunlog/...

> Problem 1
> Let's see how many events can survice after each step, such as trigger or MET filter.
> The ratio of events before and after each cut is called efficiency of the cut,
> and the table of the number of events for all the cuts is called a cutflow.
> In TutorialRun.C, I made a skeleton of cutflow using FillHist function in ${SKFlat_WD}/Analyzers/src/AnalyzerCore.C
> Complete the cutflow of TutorialRun.C


## Comparing data & mc
You can directly compare the number of observed(from data) and expected(from MCs) events within the Z-mass window. 
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

Let's move the output root files from SKFlatAnalyzer to the CMSSW directory. Also, let's get the plotting scripts from $TUTORIALRUN directory.
```
cp -r /data6/Users/${USER}/output/Run2Legacy_v4/TutorialRun/2017/ ${CMSSW_BASE}
cp -r ${TUTORIALRUN}/Plotter ${CMSSW_BASE}
cp ${TUTORIALRUN}/plot.py ${CMSSW_BASE}
```

hadd(add up) the outputs of the TutorialRun
```
cd ${CMSSW_BASE}
mv 2017 TutorialOutput && cd TutorialOutput
cp ${TUTORIALRUN}/hadd.sh .
source hadd.sh
cd ..
```

Finally, let's draw the plot:
```
python3 plot.py
```
The script will create the Z mass plot named DataAndMC_POGMedium_ZCand_mass.png. You can check about 10% discripency between the data and MCs. There could be some other processes responsible for the discrepancy(maybe BSM?), but it is reasonable to suspect the difference in efficiencies of IDs, triggers, B-taggings... between the data and MCs, or any mismodelled effect in the simulations(Is it leading order process? Next leading Order? Any jet-matching algorithm applied? PDFs? How large are statistic and systematic errors?) fisrt.

> Problem 2. Let's see the kinematic distributions of decayed products(i.e. two muons).
> Fill histograms for the leading & subleading muons. Also, fill in the kinematic variables of jets.

> Problem 3. After comparing data and MCs, you can check different kinematics of Drell-Yan and other processes.
> Some variables can be used to suppress non-DY backgrounds (such as ttbar process).
> Can you implement additional cuts to suppress background further?
> HINT: Check the distributions of jet & b-jet multiplicity(size) and MET (Missing transverse energy).

TODO: Add tutorials to set weights such as ID/Trigger SFs, PileUp weights, B-tagging SFs...

> Author: Jin Choi (choij@cern.ch)
> Modified: July 7th, 2021
