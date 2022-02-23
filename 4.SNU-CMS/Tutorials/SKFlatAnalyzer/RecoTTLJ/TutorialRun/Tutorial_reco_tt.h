#ifndef Tutorial_reco_tt_h
#define Tutorial_reco_tt_h

#include "AnalyzerCore.h"

class Tutorial_reco_tt : public AnalyzerCore {

  // particle mass in MC setting
  // will be initialized in constructor
  const double const_top_mass;
  const double const_top_width;
  const double const_w_mass;
  const double const_w_width;

public:

  void initializeAnalyzer();
  void executeEventFromParameter(AnalyzerParameter param);
  void executeEvent();

  bool UndoJERC;

  TString IsoMuTriggerName;
  double TriggerSafePtCut;

  vector<TString> MuonIDs, MuonIDSFKeys;
  vector<Muon> AllMuons;
  vector<Electron> AllElectrons;
  vector<Jet> AllJets;

  double weight_Prefire;

  Tutorial_reco_tt();
  ~Tutorial_reco_tt();

private:

  // if a class or struct defined in the class
  // it shares class name as namespace
  // had_ : hadronic, i.e. decaying hadronically
  // lep_ : leptonic, i.e. decaying leptonically
  struct ttCombinatoric{

    Lepton*          lepton;
    vector<Jet>*     jets;
    TLorentzVector*  met;

    unsigned int had_W_jet_idx_1;
    unsigned int had_W_jet_idx_2;
    unsigned int had_top_b_jet_idx;
    unsigned int lep_top_b_jet_idx;

    double had_W_mass;
    double had_top_mass;
    double had_W_b_tagger_score_1; // will not be used in this tutorial, a slot for future extension
    double had_W_b_tagger_score_2;

    std::vector<double> neu_pz; // neutrino pz solution, or given pz values for scanning
    std::vector<double> lep_top_mass;
    std::vector<double> lep_W_mass;
    std::vector<double> chi2;

    double best_neu_pz;
    double best_lep_top_mass;
    double best_lep_W_mass;
    double best_chi2;

    void EvalHadronicPart();
    void EvalLeptonicPart();

  };

  void EvalChi2(ttCombinatoric& tt_combinatoric);
  double Chi2Function(double had_top_mass, double had_W_mass, double lep_top_mass, double lep_W_mass);
};



#endif

