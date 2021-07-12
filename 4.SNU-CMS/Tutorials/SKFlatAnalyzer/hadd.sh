#!/bin/sh

# data
hadd DATA/TutorialRun_DoubleMuon.root DATA/*.root

# MC
mkdir MCSamples
cp TutorialRun_DYJets.root MCSamples/TutorialRun_DY.root
cp TutorialRun_TTLL_powheg.root MCSamples/TutorialRun_TTLL.root
hadd MCSamples/TutorialRun_VV.root TutorialRun_*_pythia.root
hadd MCSamples/TutorialRun_ST.root TutorialRun_SingleTop_*.root
