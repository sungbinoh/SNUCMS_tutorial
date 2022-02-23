** Analysis **
  * Heavy neutrino searches
    - Heavy neutrino is expected to be existent under a lot of BSM contexts. We look for heavy neutrinos in type-I seesaw and left-right symmetric model.
    - We focus on heavy neutrinos which masses are bigger than O(10) GeV.
    - For both type-I seesaw and left-right symmetric model, there are certain phase spaces where the heavy neutrinos (or its decay products) are boosted. Our main goal is to get the sensitivities in those regions as well.

  * AFB(Forward-backward asymmetry) in DY+b region
    - The latest talk by Won (Oct 5, 2021) : https://indico.cern.ch/event/1069312/
    - The main goal of this analysis is the measurement of the electroweak mixing angle.
    - Between the most precise results from LEP, SLD, there is 3 sigma discrepancy, and this study is expected to verify the LEP results.
    - This studies precision would be not that precise, but study about classifying b and b-bar jets is ongoing, and this is expected to improve the precision.

  * Hc to WA search
    - Searching for the H±→W±A,A→μ+μ− decaying mode brancing from tt production.
    - Many BSM models predict multiple higgs bosons, where the simplest expansion 2HDM predicts 5 Higgs bosons H±,H,h,A.
    - Since there is no mass relation in 2HDM and H±W±A coupling is independent of tanβ,this bosonic decay mode could be the most dominant decay mode if M(H±) < M(t) and M(A) < M(H±).
    - Using the perfect resolution of CMS muon detector, we can reconstruct A to extract the signal events.
    - This search can be a supplementary search for other charged higgs decay channels.

** EPR Tasks **
 * Run3 Muon Seed Classifier using BDT
    - The latest talk by Won (Oct 6, 2021) : https://indico.cern.ch/event/1083451/
    - As pileup in Run3 condition is expected to be higher, the muon reconstruction algorithm will consume more timing in the HLT than Run2.
    - Before the muon trajectory building step, the muon seed classifier can reject background-like muon seeds during the seeding step to reduce timing.
    - The muon seed training is done by XGBoost, and used the Run3 DY samples for train sets.

 * Central MC sample Validation
    - The latest talk given by Jin (Sep 6, 2021) : https://indico.cern.ch/event/1069262/
    - The main goal of this project is to check if there is any inconsistency in the inclusive and binned samples in the UL production as there were in some pre-Legacy samples.
    - Currently done for DY NLO with FxFx matching + CP5 tuned samples, compared incl. vs jet-binned, incl. vs pt-binned and incl. vs mass-binned.
    - No severe difference has been found.
