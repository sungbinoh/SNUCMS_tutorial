#include "Tutorial_reco_tt.h"

void Tutorial_reco_tt::initializeAnalyzer(){

  //==== if you use "--userflags UndoJERC" with SKFlat.py, HasFlag("UndoJERC") will return "true"
  UndoJERC = HasFlag("UndoJERC");
  cout << "[ExampleRun_kinFitter::initializeAnalyzer] UndoJERC = " << UndoJERC << endl;

  MuonIDs = {
    //"POGMedium",
    //"POGTight",
    "POGTightWithTightIso"
  };

  //==== corresponding Muon ID SF Keys for mcCorr->MuonID_SF()
  MuonIDSFKeys = {
    //"NUM_MediumID_DEN_genTracks",
    "NUM_TightID_DEN_genTracks",
  };

  //==== 
  if(DataYear==2016){
    IsoMuTriggerName = "HLT_IsoMu24_v";
    TriggerSafePtCut = 26.;
  }
  else if(DataYear==2017){
    IsoMuTriggerName = "HLT_IsoMu27_v";
    TriggerSafePtCut = 29.;
  }

  cout << "[ExampleRun_kinFitter::initializeAnalyzer] IsoMuTriggerName = " << IsoMuTriggerName << endl;
  cout << "[ExampleRun_kinFitter::initializeAnalyzer TriggerSafePtCut = " << TriggerSafePtCut << endl;

  //==== B-Tagging SF
  //==== add taggers and WP that you want to use in analysis
  std::vector<JetTagging::Parameters> jtps;
  //==== If you want to use 1a or 2a method,
  jtps.push_back( JetTagging::Parameters(JetTagging::DeepCSV, JetTagging::Medium, JetTagging::incl, JetTagging::comb) );
  //==== set
  mcCorr->SetJetTaggingParameters(jtps);

}

void Tutorial_reco_tt::executeEvent(){

  AllMuons = GetAllMuons();
  AllElectrons = GetAllElectrons();
  //=== Jets too
  AllJets = GetAllJets();

  //==== Get L1Prefire reweight
  //==== If data, 1.;
  //==== If MC && DataYear > 2017, 1.;
  //==== If MC && DataYear <= 2017, we have to reweight the event with this value
  weight_Prefire = GetPrefireWeight(0);

  //==== Declare AnalyzerParameter

  AnalyzerParameter param;

  //==== Loop over muon IDs

  for(unsigned int it_MuonID=0; it_MuonID<MuonIDs.size(); it_MuonID++){

    TString MuonID = MuonIDs.at(it_MuonID);
    TString MuonIDSFKey = MuonIDSFKeys.at(it_MuonID);

    //==== 1) First, let's run Central values of the systematics

    //==== clear parameter set
    param.Clear();

    //==== set which systematic sources you want to run this time
    //==== default syst_ is AnalyzerParameter::Central
    param.syst_ = AnalyzerParameter::Central;

    //==== set name of the parameter set
    //==== this will be used for the directory name of histograms
    param.Name = MuonID+"_"+"Central";

    //==== You can define lepton ID string here
    param.Muon_Tight_ID = MuonID;
    param.Muon_ID_SF_Key = MuonIDSFKey;

    //==== And, Jet ID
    param.Jet_ID = "tight";

    //==== Now, all parameters are set. Run executeEventFromParameter() with this parameter set
    executeEventFromParameter(param);

    //==== 2) Now, loop over systematic sources
    //==== without --userflag UndoJERC, this will not be ran

    if(UndoJERC){

      // undo JEC
      param.Name = MuonID+"_"+"UndoJEC";
      executeEventFromParameter(param);
      // undo JER SF
      param.Name = MuonID+"_"+"UndoJERSF";
      executeEventFromParameter(param);


    }

  }


}

void Tutorial_reco_tt::executeEventFromParameter(AnalyzerParameter param){

  //=============
  //==== No Cut
  //=============

  FillHist(param.Name+"/NoCut_"+param.Name, 0., 1., 1, 0., 1.);

  //========================
  //==== MET Filter
  //========================

  if(!PassMETFilter()) return;

  Event ev = GetEvent();
  Particle METv = ev.GetMETVector();

  //==============
  //==== Trigger
  //==============
  if(! (ev.PassTrigger(IsoMuTriggerName) )) return;



  //======================
  //==== Copy AllObjects
  //======================

  vector<Muon> this_AllMuons = AllMuons;
  vector<Electron> this_AllElectrons = AllElectrons;
  vector<Jet> this_AllJets = AllJets;

  //==== Undo JERC
  // only Central systematic flag is available now

  if(param.syst_ == AnalyzerParameter::Central){

    if(param.Name.Contains("_Central")){
      //pass
    }
    else if(param.Name.Contains("_UndoJEC")){
      // undo JEC
      this_AllJets = SmearJets( this_AllJets, +1 ); //XXX fix later
    }
    else if(param.Name.Contains("_UndoJERSF")){
      // undo JER smearing
      this_AllJets = SmearJets( this_AllJets, +1 ); //XXX fix later
    }
    else{
      // a exception handling
      cout << "[ExampleRun_kinFitter::executeEventFromParameter] Wrong param Name for undo JERC" << endl;
      exit(EXIT_FAILURE);
    }

  }
  else{
    // a exception handling
    cout << "[ExampleRun_kinFitter::executeEventFromParameter] Wrong syst" << endl;
    exit(EXIT_FAILURE);
  }
  //==================================================
  //==== Then, apply ID selections using this_AllXXX
  //==================================================

  // baseline lepton selection
  //
  // muon:     POG Tight ID + tight iso is still efficient (~95%) for veto ID
  // electron: POG Loose ID has ~90% of efficiency
  vector<Muon> muons = SelectMuons(this_AllMuons, param.Muon_Tight_ID, 15., 2.4);
  vector<Electron> electrons_loose = SelectElectrons(this_AllElectrons, "passLooseID", 15., 2.5);

  //veto event if not exactly 1 (baseline) lepton
  if(muons.size() + electrons_loose.size() != 1) return;
  FillHist(param.Name+"/BaseLineLeptonSelection_"+param.Name, 0., 1., 1, 0., 1.);


  vector<Jet> jets = SelectJets(this_AllJets, param.Jet_ID, 30., 2.4);
  // veto lepton with delta R < 0.3 matching
  jets             = JetsVetoLeptonInside(jets, electrons_loose, muons, 0.3);  

  TLorentzVector met;
  met    = GetEvent().GetMETVector();

  //=======================
  //==== Sort in pt-order
  //=======================

  //==== 1) leptons : after scaling/smearing, pt ordring can differ from MINIAOD
  std::sort(muons.begin(), muons.end(), PtComparing);
  //==== 2) jets : similar, but also when applying new JEC, ordering is changes. This is important if you use leading jets
  std::sort(jets.begin(), jets.end(), PtComparing);


  int NBJets(0);
  //JetTagging::Parameters jtp_DeepCSV_Medium = JetTagging::Parameters(JetTagging::DeepCSV,
  //                                                                   JetTagging::Medium,
  //                                                                   JetTagging::incl, JetTagging::comb);

  //==== b tagging

  //==== method 1a)
  //==== multiply "btagWeight" to the event weight
  //TODO
  //double btagWeight = mcCorr->GetBTaggingReweight_1a(jets, jtp_DeepCSV_Medium);
  std::vector<bool> btag_vector{};

  //==== method 2a)
  for(unsigned int ij = 0 ; ij < jets.size(); ij++){

    double this_discr = jets.at(ij).GetTaggerResult(JetTagging::DeepCSV);
    if( this_discr > mcCorr->GetJetTaggingCutValue(JetTagging::DeepCSV, JetTagging::Medium) ){
      NBJets++;
      btag_vector.push_back(true);
    }
    else{
      btag_vector.push_back(false);
    }

  }

  
  //=========================
  //==== Event selections..
  //=========================

  //==== single muon
  if(muons.size() != 1) return;

  //==== leading muon has trigger-safe pt
  if( muons.at(0).Pt() <= TriggerSafePtCut ) return;

  if(jets.size()<4) return;
  if(NBJets!=2) return;
  if(met.Pt()<=20) return;

  FillHist(param.Name+"/BaseLineCut_"+param.Name, 0., 1., 1, 0., 1.);


  //=======================
  //==== Take leading four jets in pT
  //=======================
  std::vector<unsigned int> top_b_jet_candidates;
  std::vector<unsigned int> had_W_candidates;

  for(unsigned int ij(0); ij < 4; ij++){

    // check if b-tagged jet
    if(btag_vector.at(ij)){
      if(top_b_jet_candidates.size()<2){
        top_b_jet_candidates.push_back(ij);
      }  
    }
    else{
      if(had_W_candidates.size()<2){
        had_W_candidates.push_back(ij);
      }
    }

  }

  if( had_W_candidates.size()<2 || top_b_jet_candidates.size()<2 ) return;
  FillHist(param.Name+"/PassLeading4Jets_"+param.Name, 0., 1., 1, 0., 1.);

  //=======================
  //==== Find minimum chi2 jet combination
  //=======================
  std::vector<Tutorial_reco_tt::ttCombinatoric> tt_combinatorics;
  Tutorial_reco_tt::ttCombinatoric tt_combinatoric_1, tt_combinatoric_2;
  

  // set first combinatoric
  tt_combinatoric_1.lepton            = &(muons.at(0));
  tt_combinatoric_1.jets              = &jets;
  tt_combinatoric_1.met               = &met;
  tt_combinatoric_1.had_W_jet_idx_1   = had_W_candidates.at(0);
  tt_combinatoric_1.had_W_jet_idx_2   = had_W_candidates.at(1);
  tt_combinatoric_1.had_top_b_jet_idx = top_b_jet_candidates.at(0);
  tt_combinatoric_1.lep_top_b_jet_idx = top_b_jet_candidates.at(1);
 
  // set second combinatoric
  tt_combinatoric_2.lepton            = &(muons.at(0));
  tt_combinatoric_2.jets              = &jets;
  tt_combinatoric_2.met               = &met;
  tt_combinatoric_2.had_W_jet_idx_1   = had_W_candidates.at(0);
  tt_combinatoric_2.had_W_jet_idx_2   = had_W_candidates.at(1);
  tt_combinatoric_2.had_top_b_jet_idx = top_b_jet_candidates.at(1); // flip b-tagged jet assignment
  tt_combinatoric_2.lep_top_b_jet_idx = top_b_jet_candidates.at(0);

  this->EvalChi2(tt_combinatoric_1); // 'this->EvalChi2(...)' is same as 'EvalChi2(...)'
  this->EvalChi2(tt_combinatoric_2); // I prefer to use 'this pointer' to clarify where the method belongs to. 


  Tutorial_reco_tt::ttCombinatoric* best_combinatoric;
  // compare best_chi2 between combinatorics
  if(tt_combinatoric_1.best_chi2 < tt_combinatoric_2.best_chi2){
    best_combinatoric = &tt_combinatoric_1;
  }
  else{
    best_combinatoric = &tt_combinatoric_2;
  }


  //===================
  //==== Event weight
  //===================

  double baseWeight  = 1.;
  //double weight = 1.;
  //==== If MC
  if(!IsDATA){

    //==== weight_norm_1invpb is set to be event weight normalized to 1 pb-1
    //==== So, you have to multiply trigger luminosity
    //==== you can pass trigger names to ev.GetTriggerLumi(), but if you are using unprescaled trigger, simply pass "Full"

    baseWeight *= weight_norm_1invpb*ev.GetTriggerLumi("Full");

    //==== MCweight is +1 or -1. Should be multiplied if you are using e.g., aMC@NLO NLO samples
    baseWeight *= ev.MCweight();

    ////==== L1Prefire reweight
    //weight *= weight_Prefire;

    ////==== Example of applying Muon scale factors
    //for(unsigned int i=0; i<muons.size(); i++){

    //  double this_idsf  = mcCorr->MuonID_SF (param.Muon_ID_SF_Key,  muons.at(i).Eta(), muons.at(i).MiniAODPt());

    //  //==== If you have iso SF, do below. Here we don't.
    //  //double this_isosf = mcCorr->MuonISO_SF(param.Muon_ISO_SF_Key, muons.at(i).Eta(), muons.at(i).MiniAODPt());
    //  double this_isosf = 1.;

    //  weight *= this_idsf*this_isosf;

    //}

  }

  FillHist(param.Name+"/noChi2Cut/had_W_mass_"+param.Name,   best_combinatoric->had_W_mass,        baseWeight, 40, 0., 200.);
  FillHist(param.Name+"/noChi2Cut/had_top_mass_"+param.Name, best_combinatoric->had_top_mass,      baseWeight, 80, 0., 400.);
  FillHist(param.Name+"/noChi2Cut/lep_W_mass_"+param.Name,   best_combinatoric->best_lep_W_mass,   baseWeight, 40, 0., 200.);
  FillHist(param.Name+"/noChi2Cut/lep_top_mass_"+param.Name, best_combinatoric->best_lep_top_mass, baseWeight, 80, 0., 400.);
  FillHist(param.Name+"/noChi2Cut/chi2_"+param.Name,         best_combinatoric->best_chi2,         baseWeight, 1000, 0., 100000.);

  if(best_combinatoric->best_chi2 >= 2e3) return; // chi2 cut above 2000
  FillHist(param.Name+"/PassChi2Cut_"+param.Name, 0., 1., 1, 0., 1.);
  
  FillHist(param.Name+"/Chi2Cut/had_W_mass_"+param.Name,   best_combinatoric->had_W_mass,        baseWeight, 40, 0., 200.);
  FillHist(param.Name+"/Chi2Cut/had_top_mass_"+param.Name, best_combinatoric->had_top_mass,      baseWeight, 80, 0., 400.);
  FillHist(param.Name+"/Chi2Cut/lep_W_mass_"+param.Name,   best_combinatoric->best_lep_W_mass,   baseWeight, 40, 0., 200.);
  FillHist(param.Name+"/Chi2Cut/lep_top_mass_"+param.Name, best_combinatoric->best_lep_top_mass, baseWeight, 80, 0., 400.);
  FillHist(param.Name+"/Chi2Cut/chi2_"+param.Name,         best_combinatoric->best_chi2,         baseWeight, 2000, 0., 2000.);

  

}

void Tutorial_reco_tt::EvalChi2(ttCombinatoric& tt_combinatoric){

  
  tt_combinatoric.EvalHadronicPart();
  tt_combinatoric.EvalLeptonicPart();


  tt_combinatoric.best_chi2 = 1e9;
  for(unsigned int i(0); i<tt_combinatoric.neu_pz.size(); i++){

    double chi2 = this->Chi2Function(
			    tt_combinatoric.had_top_mass,
			    tt_combinatoric.had_W_mass,
			    tt_combinatoric.lep_top_mass.at(i),
			    tt_combinatoric.lep_W_mass.at(i)
			    );

    tt_combinatoric.chi2.push_back(chi2);

    // updates best chi2
    if(chi2 < tt_combinatoric.best_chi2){
      tt_combinatoric.best_lep_top_mass = tt_combinatoric.lep_top_mass.at(i);
      tt_combinatoric.best_lep_W_mass   = tt_combinatoric.lep_W_mass  .at(i);
      tt_combinatoric.best_neu_pz       = tt_combinatoric.neu_pz      .at(i);
      tt_combinatoric.best_chi2         = chi2;
    }
  
  }
  

}

double Tutorial_reco_tt::Chi2Function(double had_top_mass, double had_W_mass, double lep_top_mass, double lep_W_mass){

  double chi2 = 0.;
  chi2 += TMath::Power( (had_top_mass - const_top_mass )/const_top_width,  2);
  chi2 += TMath::Power( (had_W_mass   - const_w_mass   )/const_w_width,    2);
  chi2 += TMath::Power( (lep_top_mass - const_top_mass )/const_top_width,  2);
  chi2 += TMath::Power( (lep_W_mass   - const_w_mass   )/const_w_width,    2);

  return chi2;

}

Tutorial_reco_tt::Tutorial_reco_tt() :
	const_top_mass(172.5),
	const_top_width(1.5),
	const_w_mass(80.4),
	const_w_width(2.085)
{

}

Tutorial_reco_tt::~Tutorial_reco_tt(){

}


//=== define Tutorial_reco_tt::ttCombinatoric

void Tutorial_reco_tt::ttCombinatoric::EvalHadronicPart(){

  TLorentzVector had_W_vector   = static_cast<TLorentzVector>(jets->at( had_W_jet_idx_1 ))
	                        + static_cast<TLorentzVector>(jets->at( had_W_jet_idx_2 ));
	  

  TLorentzVector had_top_vector = had_W_vector
	                        + static_cast<TLorentzVector>(jets->at( had_top_b_jet_idx ));

  had_W_mass   = had_W_vector.M();
  had_top_mass = had_top_vector.M();


}

void Tutorial_reco_tt::ttCombinatoric::EvalLeptonicPart(){

  //clear vector
  lep_top_mass.clear();
  lep_W_mass  .clear();
  neu_pz      .clear();
  chi2        .clear();


  //set neu_pz
  //scan -700 GeV to 700 GeV in 5 GeV step
  double step = 5.;
  double pz   = -700.;
  while(pz<=700){
    neu_pz.push_back(pz);
    pz += step;
  }


  // iterate over pz
  for(auto& a_neu_pz : neu_pz){
    double E_neu = TMath::Sqrt( met->E() * met->E() + a_neu_pz * a_neu_pz );
    //define neutrino vector
    TLorentzVector neutrino_vector(met->Px(),met->Py(), a_neu_pz, E_neu);

    TLorentzVector lep_W_vector   = neutrino_vector
 	                          + static_cast<TLorentzVector>(*lepton);
    TLorentzVector lep_top_vector = lep_W_vector
	                          + static_cast<TLorentzVector>(jets->at( lep_top_b_jet_idx ));

    lep_W_mass  .push_back(lep_W_vector.M()  );
    lep_top_mass.push_back(lep_top_vector.M());
  
  }
  

}
