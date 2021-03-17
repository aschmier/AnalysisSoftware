// provided by Gamma Conversion Group, $ALICE_ROOT/PWG4/GammaConv ;https://twiki.cern.ch/twiki/bin/view/ALICE/PWG4GammaConversion

//*****************************************************************************//
/*
This macro is similar to ExtractSignalV2.C but instead of scaling the background
and additionally fitting a pol1,2 etc. templete fits are used to scale the background
The true signal of the pi0 and eta is taken into account in the fir such that the
"scaling" of the background is also done in the signal region.
Several options are provided for the fit:
- "Template" is the standard option. The same evt. distribution is fitted with Pi0 and eta signal and the background
- "TemplateScale" is similar to the standard but the eta/pi0 ratio is fixed (This is important above 10GeV for the eta, because the pi0 signal comes into the eta region)
  The value for the eta/pi0 ratio has to be included in this file (fScaleFacEtaToPi0 = XYZ) for the respective energy and system
- "Template(Scale)Pol1" Background template will additinally fitted with a pol1 (Background*Pol1)
- "Template(Scale)PlusPol1" An additional linear background is added  (Background + Pol1)
- "TemplateMC" the MC true background will be taken as background template for both data and MC
 */
//*****************************************************************************//


#include "ExtractSignalTemplates.h"
#include "TMinuit.h"
extern TMinuit*         gMinuit;

using namespace std;

//****************************************************************************
//************** Main function for extraction of signal **********************
//****************************************************************************
void ExtractSignalTemplates(
  TString meson                   = "",
  TString fileData                = "",
  TString fileMC                  = "",
  TString cutSelection            = "",
  TString Suffix                  = "",
  TString optionMC                = "",
  TString optionEnergy            = "",
  TString optionCrystalBall       = "",
  TString directphotonPlots       = "",
  TString optionUseMinBiasEff     = "",
  TString optionPeriod            = "",
  TString optionAdvancedMesonQA   = "",
  Int_t numberOfBins              = 30,
  Bool_t addSig                   = kFALSE,
  Int_t mode                      = 9,
  Bool_t UseTHnSparse             = kTRUE,
  Int_t triggerSet                = -1,
  TString optionCorrFrameworkDir  = "",
  Bool_t useExtAccept             = kFALSE
) {
  gROOT->Reset();


  fMode = mode;
  fModeHeavy = mode;

  TString fDecayChannel = "#gamma#gamma";

  if (meson.Contains("Pi0")){
      SetCorrectMCHistogrammNames("Pi0");
  } else if (meson.CompareTo("Eta") == 0 ){
      SetCorrectMCHistogrammNames("Eta");
  } else if (meson.CompareTo("EtaPrime") == 0 ){
      SetCorrectMCHistogrammNames("EtaPrime");
  }
  cout << " MC histo names Set " << endl;

  TFile* f = new TFile(fileData.Data());
  TFile* fMC = new TFile(fileMC.Data());

  TString autoDetectedMainDir = AutoDetectMainTList(mode,f,"",optionCorrFrameworkDir); // heavy meson analysis
  TString autoDetectedMainDirMC = AutoDetectMainTList(mode,fMC,"",optionCorrFrameworkDir); // heavy meson analysis
  if (autoDetectedMainDir.CompareTo("") == 0){
      cout << "ERROR: trying to read file, which is incompatible with mode selected (mode " << mode << ")" << endl;;
      return;
  }
  TList *TopDir                   = (TList*)f->Get(autoDetectedMainDir.Data());
  TList *TopDirMC                     = (TList*)fMC->Get(autoDetectedMainDirMC.Data());
  if(TopDir == NULL){
      cout<<"ERROR: TopDir not Found"<<endl;
      return;
  }
  if(TopDirMC == NULL){
      cout<<"ERROR: TopDirMC not Found"<<endl;
      return;
  }
  cout << "Reading from Data TopDir \"" << autoDetectedMainDir << "\"" << endl;
  if(debugOutputLevel) cerr<<"Debug in ExtractSignalTemplate; LINE: "<<__LINE__<<endl;
  // adjusting Cutnumber
  fCutSelection = cutSelection;
  TString fCutSelectionRead = cutSelection;
  if (mode == 9){
      ReturnSeparatedCutNumber(cutSelection, fGammaCutSelection, fElectronCutSelection,fMesonCutSelection);
      fEventCutSelection  = fGammaCutSelection(0,7);
      fGammaCutSelection  = fGammaCutSelection(7,fGammaCutSelection.Length()-7);
      cout << fEventCutSelection.Data() << "\t" << fGammaCutSelection.Data() << endl;
  } else {
    cout<<cutSelection<<endl;
      ReturnSeparatedCutNumberAdvanced(cutSelection,fEventCutSelection, fGammaCutSelection, fClusterCutSelection, fElectronCutSelection, fMesonCutSelection, mode);
  }
  if(debugOutputLevel) cerr<<"Debug in ExtractSignalTemplate; LINE: "<<__LINE__<<endl;

  //
  //****************************** calculate meson mass from pdg code ************************************************
  cout << (TDatabasePDG::Instance())->GetParticle(fMesonId) << endl;
  fMesonMassExpect                            = (TDatabasePDG::Instance())->GetParticle(fMesonId)->Mass();

  //****************************** Specification of collision system ************************************************
  fEnergyFlag         = optionEnergy;
  fPrefix             = meson;

  fPeriodFlag         = optionPeriod;
  fDirectPhoton       = directphotonPlots;
  cout<<"fDirectPhoton: "<<fDirectPhoton<<endl;

  TString textProcess = ReturnMesonString (fPrefix);
  if(textProcess.CompareTo("") == 0 ){
      cout << "Meson unknown" << endl;
      return ;
  }

  fTextMeasurement    = Form("%s #rightarrow #gamma#gamma", textProcess.Data());
  fCollisionSystem    = ReturnFullCollisionsSystem(fEnergyFlag);
  if (fCollisionSystem.CompareTo("") == 0){
      cout << "No correct collision system specification, has been given" << endl;
      return;
  }
  fDetectionProcess   = ReturnFullTextReconstructionProcess(mode);

  //***************************** Specification Data/MC ************************************************************
  if(optionMC.CompareTo("kTRUE") == 0){
      fIsMC               = 1;
      fPrefix2            = "MC";
  } else {
      fIsMC               = 0;
      fPrefix2            = "data";
  }


  //****************************** Choice of Fitting procedure ******************************************************
  optionOtherResBckAsStd = -2;
  if(optionCrystalBall.Contains("PlusPol1")){           // background + pol1
      optionTemplateFit   = 2;
    } else if(optionCrystalBall.Contains("Pol1")){      // background * pol1
      optionTemplateFit = 1;
    } else {                                            // background
      optionTemplateFit  = 0;
  }
  optionBackgroundMethod = 0;                           // mixed evt.
  if (GetMesonBGSchemeIsRotation(fMesonCutSelection(GetMesonBGSchemeCutPosition(),1))){
    optionBackgroundMethod = 1;                         // rotation back.
  } else if(optionCrystalBall.Contains("MC")){
      optionBackgroundMethod = 2;                         // MC true background
  }

  //**************************** Determine Centrality *************************************************************
  centralityString        = GetCentralityString(fEventCutSelection);
  cout << "Centrality: " << centralityString.Data() << endl;
  if (centralityString.CompareTo("pp")!=0 && centralityString.CompareTo("0-100%") != 0){
      fCollisionSystem    = Form("%s %s", centralityString.Data(), fCollisionSystem.Data());
  }
  cout << "Collisions system: " << fCollisionSystem.Data() << endl;


  // ******************************* Adjust cutstrings if needed **************************************************
  TString fEventCutSelectionRead  = fEventCutSelection.Data();
  TString fGammaCutSelectionRead  = fGammaCutSelection.Data();
  TString fMesonCutSelectionRead  = fMesonCutSelection.Data();
  TString fClusterCutSelectionRead= fClusterCutSelection.Data();


  //******************************* Setting background option *******************************************************
  if(optionCrystalBall.Contains("Scale")){
    if(fEnergyFlag.Contains("13TeV")){
      fScaleFacEtaToPi0 = 0.379828/0.435;
    }
  }



  if(debugOutputLevel) cerr<<"Debug in ExtractSignalTemplate; LINE: "<<__LINE__<<endl;
  //******************************************************************************************************
  //***************************** Get Main folder for cut ************************************************
  //******************************************************************************************************
  cout<<Form("Cut Number %s",fCutSelectionRead.Data())<<endl;
  TList *HistosGammaConversion       = (TList*)TopDir->FindObject(Form("Cut Number %s",fCutSelectionRead.Data()));
  // check if cutnumber available, otherwise adjust for pileup cut
  if (HistosGammaConversion == NULL){
    if(debugOutputLevel) cerr<<"Debug in ExtractSignalTemplate; LINE: "<<__LINE__<<endl;
      TString fEventCutSelectionPileUpRejection   = fEventCutSelection(5,1);
      cout << "cutnumber for PileUpRejection is: " << fEventCutSelectionPileUpRejection << endl;
      if( CutNumberToInteger(fEventCutSelectionPileUpRejection) > 1  && optionMC.CompareTo("kTRUE") == 0){
      cout << "changing PileUpCut for MC" << endl;
      cout << fEventCutSelectionRead.Data() << endl;
      fEventCutSelectionRead.Replace(GetEventRemovePileUpCutPosition(),1,"1");
      cout << fEventCutSelectionRead.Data() << endl;
      fClusterCutSelectionRead  = fClusterCutSelection;
      if (mode==0 || mode==100)
          fCutSelectionRead       = Form("%s_%s_%s",fEventCutSelectionRead.Data(), fGammaCutSelection.Data(), fMesonCutSelection.Data());
      if (mode==2 || mode==3 || mode==102 || mode==103 || mode==100 || mode==14)
          fCutSelectionRead       = Form("%s_%s_%s_%s",fEventCutSelectionRead.Data(), fGammaCutSelection.Data(), fClusterCutSelection.Data(), fMesonCutSelection.Data());
      if (mode==4 || mode==5 || mode==104 || mode==105)
          fCutSelectionRead       = Form("%s_%s_%s",fEventCutSelectionRead.Data(), fClusterCutSelection.Data(), fMesonCutSelection.Data());
      cout << fCutSelectionRead.Data() << endl;
      }
  }
  TList *HistosGammaConversionMC       = (TList*)TopDirMC->FindObject(Form("Cut Number %s",fCutSelectionRead.Data()));
  if (HistosGammaConversionMC == NULL){
    if(debugOutputLevel) cerr<<"Debug in ExtractSignalTemplate; LINE: "<<__LINE__<<endl;
      TString fEventCutSelectionBaseEvent     = fEventCutSelectionRead(GetEventSystemCutPosition(),1);
      cout << "cutnumber for basic event selection is: " << fEventCutSelectionBaseEvent << endl;
      if ( CutNumberToInteger(fEventCutSelectionBaseEvent) == 1 && optionMC.CompareTo("kTRUE") == 0){
          cout << "changing basic event cut for MC" << endl;
          cout << fEventCutSelectionRead.Data() << endl;
          fEventCutSelectionRead.Replace(GetEventSystemCutPosition(),1,"5");
          cout << fEventCutSelectionRead.Data() << endl;
          fClusterCutSelectionRead  = fClusterCutSelection;
          if (mode==0 || mode==100)
              fCutSelectionRead       = Form("%s_%s_%s",fEventCutSelectionRead.Data(), fGammaCutSelection.Data(), fMesonCutSelection.Data());
          if (mode==2 || mode==3 || mode==102 || mode==103)
              fCutSelectionRead       = Form("%s_%s_%s_%s",fEventCutSelectionRead.Data(), fGammaCutSelection.Data(), fClusterCutSelection.Data(), fMesonCutSelection.Data());
          if (mode==4 || mode==5 || mode==104 || mode==105)
              fCutSelectionRead       = Form("%s_%s_%s",fEventCutSelectionRead.Data(), fClusterCutSelection.Data(), fMesonCutSelection.Data());
          cout << fCutSelectionRead.Data() << endl;
      } else if ( CutNumberToInteger(fEventCutSelectionBaseEvent) == 5 && optionMC.CompareTo("kTRUE") == 0){
          cout << "changing basic event cut for MC" << endl;
          cout << fEventCutSelectionRead.Data() << endl;
          fEventCutSelectionRead.Replace(GetEventSystemCutPosition(),1,"1");
          cout << fEventCutSelectionRead.Data() << endl;
          fClusterCutSelectionRead  = fClusterCutSelection;
          if (mode==0 || mode==100)
              fCutSelectionRead       = Form("%s_%s_%s",fEventCutSelectionRead.Data(), fGammaCutSelection.Data(), fMesonCutSelection.Data());
          if (mode==2 || mode==3 || mode==102 || mode==103 || mode==14)
              fCutSelectionRead       = Form("%s_%s_%s_%s",fEventCutSelectionRead.Data(), fGammaCutSelection.Data(), fClusterCutSelection.Data(), fMesonCutSelection.Data());
          if (mode==4 || mode==5 || mode==104 || mode==105)
              fCutSelectionRead       = Form("%s_%s_%s",fEventCutSelectionRead.Data(), fClusterCutSelection.Data(), fMesonCutSelection.Data());
          cout << fCutSelectionRead.Data() << endl;
      }
  }


  // Prepend digit in case of heavy meson analysis
  if( mode >= 100) fEventCutSelectionRead.Prepend( Form("%d_",GetHeavyMesonDigit(meson)) );
  StyleSettingsThesis(Suffix);
  SetPlotStyle();

  if(meson.CompareTo("Pi0") == 0){
      cout << "entering Pi0" << endl;
      Initialize("Pi0",numberOfBins, triggerSet);
      InitializeTemplates("Pi0",numberOfBins, triggerSet);
  } else if (meson.CompareTo("Eta") == 0) {
    Initialize("Eta",numberOfBins, triggerSet);
      InitializeTemplates("Eta",numberOfBins, triggerSet);
  } else if (meson.CompareTo("EtaPrime") == 0) {
      InitializeTemplates("EtaPrime",numberOfBins, triggerSet);
  } else if(meson.CompareTo("Pi0EtaBinning") == 0) {
      InitializeTemplates("Pi0EtaBinning",numberOfBins, triggerSet);
  } else if(meson.CompareTo("Pi0OmegaBinning") == 0) {
      InitializeTemplates("Pi0OmegaBinning",numberOfBins, triggerSet);
  } else   {
      cout<<"ERROR: First argument in the ExtractSignal(....) has to be either Pi0 or Eta or Pi0EtaBinning  or EtaPrime"<<endl;
      return;
  }

  CreatePtHistos();
  // CreatePtHistosTemp();


  if(debugOutputLevel) cerr<<"Debug in ExtractSignalTemplate; LINE: "<<__LINE__<<endl;
  TString outputDir   = Form("%s/%s/%s/ExtractSignal",cutSelection.Data(),optionEnergy.Data(),Suffix.Data());
  TString outputDirMon= Form("%s/%s/%s/ExtractSignal/Monitoring/",cutSelection.Data(),optionEnergy.Data(),Suffix.Data());
  gSystem->Exec("mkdir -p "+outputDir);
  gSystem->Exec("mkdir -p "+outputDirMon);

  cout<<"Pictures are saved as "<< Suffix.Data()<< endl;
  fDate = ReturnDateString();

  if(debugOutputLevel) cerr<<"Debug in ExtractSignalTemplate; LINE: "<<__LINE__<<endl;
  TList *ESDContainer                 = (TList*) HistosGammaConversion->FindObject(Form("%s ESD histograms",fCutSelectionRead.Data()));
  TList *ESDContainerMC                 = (TList*) HistosGammaConversionMC->FindObject(Form("%s ESD histograms",fCutSelectionRead.Data()));
  TList *MCContainerTemp                  = (TList*) HistosGammaConversionMC->FindObject(Form("%s MC histograms",fCutSelectionRead.Data()));
  TList *TrueConversionContainerTemp           = (TList*) HistosGammaConversionMC->FindObject(Form("%s True histograms",fCutSelectionRead.Data()));
  if(debugOutputLevel) cerr<<"Debug in ExtractSignalTemplate; LINE: "<<__LINE__<<endl;



  //****************************** Get ESD, MC and TRUE Containers  ************************************************
  if (fMode == 2 || fMode == 13 || fMode == 3 ){
      TList *ClusterContainer             = (TList*) HistosGammaConversion->FindObject(Form("%s Cluster Output",fCutSelectionRead.Data()));
      if (ClusterContainer){
          fHistoClustersPt                = (TH1D*)ClusterContainer->FindObject("ClusGamma_Pt");
          fHistoClustersE                 = (TH1D*)ClusterContainer->FindObject("ClusGamma_E");
          fHistoClustersOverlapHeadersPt  = (TH1D*)ClusterContainer->FindObject("ClusGammaOverlapHeaders_Pt");
          TH2F* fHistoTrue2DGammaDCClusPt = (TH2F*)ClusterContainer->FindObject(ObjectNameDCGammaClusPt.Data());
          if (fHistoTrue2DGammaDCClusPt!=NULL) fEnableDCCluster= kTRUE;
          if (fEnableDCCluster){
              fHistoTrue2DGammaDCClusPt->Sumw2();
              fHistoTrueGammaDCClusPt             = (TH1D*)fHistoTrue2DGammaDCClusPt->ProjectionX("TrueClusGamma_Pt",0,-1,"e");
              cout << "Cluster DC found " << endl;
              fHistoTrueGammaClusMultipleCount    = (TH1F*)ClusterContainer->FindObject(ObjectNameGammaClusMultipleCount.Data());
              fHistoTrueGammaClusPt               = (TH1F*)ClusterContainer->FindObject("TrueClusGamma_Pt");
          }
      }
  }
  //****************************** Get Cluster Histograms  ************************************************
  if(debugOutputLevel) cerr<<"Debug in ExtractSignalTemplate; LINE: "<<__LINE__<<endl;
  if ( fMode == 4 || fMode == 12  || fMode == 5 || fMode==14){
      fHistoClustersPt                = (TH1D*)ESDContainer->FindObject("ClusGamma_Pt");
      fHistoClustersE                 = (TH1D*)ESDContainer->FindObject("ClusGamma_E");
      fHistoClustersOverlapHeadersPt  = (TH1D*)ESDContainer->FindObject("ClusGammaOverlapHeaders_Pt");
  }
  if ( fMode == 0 ){
      fHistoClustersPt                = (TH1D*)ESDContainer->FindObject("ClusGamma_Pt");
      fHistoClustersE                 = (TH1D*)ESDContainer->FindObject("ClusGamma_E");
      if (fHistoClustersPt){
          cout << "INFO: found cluster output in PCM stream, adding it to the raw data file." << endl;
      }

  }

  //****************************** Get some Event Histograms  ************************************************
  if(debugOutputLevel) cerr<<"Debug in ExtractSignalTemplate; LINE: "<<__LINE__<<endl;
  TList* EventCuts                    = (TList*)HistosGammaConversion->FindObject(Form("ConvEventCuts_%s",fEventCutSelectionRead.Data()));
  if (EventCuts){
    fHistoPileUpVertexDistance               = (TH1F*) EventCuts->FindObject(Form("PileupVertexDistance %s",fEventCutSelectionRead.Data()));
    fHistoPileUpVertexDistance_SPDPileup     = (TH1F*) EventCuts->FindObject(Form("PileupVertexDistance_SPDPileup %s",fEventCutSelectionRead.Data()));
    fHistoPileUpVertexDistance_TrackletHits  = (TH1F*) EventCuts->FindObject(Form("PileupVertexDistance_TrackletvsHits %s",fEventCutSelectionRead.Data()));
  }


  //****************************** calculate number of events for normalization ************************************************
  fNumberOfGoodESDTracks              = (TH1D*)ESDContainer->FindObject("GoodESDTracks");
  fEventQuality                       = (TH1D*)ESDContainer->FindObject("NEvents");
  if (fEnergyFlag.Contains("PbPb")|| fEnergyFlag.Contains("pPb")){
      fNEvents        = fEventQuality->GetBinContent(1);
  } else {
      fNEvents        = GetNEvents(fEventQuality);
  }
  //
  //****************************** Load true MC histograms for templates ************************************************
  TString ObjectNameTruePi0               = "ESD_TruePi0_InvMass_Pt";
  TString ObjectNameTrueEta               = "ESD_TrueEta_InvMass_Pt";

  fTruePi0InvMassVSPt                     = (TH2D*)TrueConversionContainerTemp->FindObject(ObjectNameTruePi0.Data());
  fTrueEtaInvMassVSPt                     = (TH2D*)TrueConversionContainerTemp->FindObject(ObjectNameTrueEta.Data());
  fTruePi0InvMassVSPt->Sumw2();
  fTrueEtaInvMassVSPt->Sumw2();
  if(debugOutputLevel) cerr<<"Debug in ExtractSignalTemplate; LINE: "<<__LINE__<<endl;

  //
  //****************************** LOAD INV. Mass Histograms ************************************************
  TString ObjectNameSameEvt               = "ESD_Mother_InvMass_Pt";
  TString ObjectNameBck                   = "ESD_Background_InvMass_Pt";

  fGammaGammaInvMassVSPt                  = (TH2D*)ESDContainer->FindObject(ObjectNameSameEvt.Data());
  fBckInvMassVSPt                         = (TH2D*)ESDContainer->FindObject(ObjectNameBck.Data());
  if(optionCrystalBall.Contains("MCBack")){ // MC true background
      fBckInvMassVSPt = (TH2D*) ESDContainerMC->FindObject(ObjectNameSameEvt.Data());
      fBckInvMassVSPt->Add(fTruePi0InvMassVSPt, -1);
      fBckInvMassVSPt->Add(fTrueEtaInvMassVSPt, -1);
  }
  fGammaGammaInvMassVSPt->Sumw2();
  fBckInvMassVSPt->Sumw2();

  //************************** MC true background histogram ***************************
  fTrueBckInvMassVSPt = (TH2D*) ESDContainerMC->FindObject(ObjectNameSameEvt.Data());
  fTrueBckInvMassVSPt->Add(fTruePi0InvMassVSPt, -1);
  fTrueBckInvMassVSPt->Add(fTrueEtaInvMassVSPt, -1);
  fTrueBckInvMassVSPt->Sumw2();


  if(debugOutputLevel) cerr<<"Debug in ExtractSignalTemplate; LINE: "<<__LINE__<<endl;


  //
  //******************Function to Project the 2D histos InvariantMass VS Pt into Invariant Mass spectrum**************
  FillMassHistosArray(fGammaGammaInvMassVSPt);
  ProduceBckProperWeighting(ESDContainer,ESDContainer, nullptr, nullptr ,UseTHnSparse);
  FillMassHistosArrayTemp();

  if(debugOutputLevel) cerr<<"Debug in ExtractSignalTemplate; LINE: "<<__LINE__<<endl;

  // enter pure simulation routines
  if(fIsMC){
      // load containers for simulation
      TList *MCContainer              = (TList*)HistosGammaConversion->FindObject(Form("%s MC histograms",fCutSelectionRead.Data()));
      TList *TrueConversionContainer  = (TList*)HistosGammaConversion->FindObject(Form("%s True histograms",fCutSelectionRead.Data()));

      // loading histograms for pi0
      if( fMesonId == 111){
          fHistoMCMesonPt                 = (TH1D*)MCContainer->FindObject(ObjectNameMCPi0.Data());   // Not the best; better having a 2D Pt_vs_Rapid in case we change limits

          fHistoMCMesonPtWOWeights            = (TH1D*)MCContainer->FindObject(ObjectNameMCPi0WOWeights.Data());
          fHistoMCMesonPtWOEvtWeights         = (TH1D*)MCContainer->FindObject(ObjectNameMCPi0WOEvtWeights.Data());

          fHistoMCMesonPtWithinAcceptance     = (TH1D*)MCContainer->FindObject(ObjectNameMCPi0Acc.Data());
          fHistoMCMesonPtWithinAcceptanceWOWeights    = (TH1D*)MCContainer->FindObject(ObjectNameMCPi0AccWOWeights.Data());
          fHistoMCMesonPtWithinAcceptanceWOEvtWeights = (TH1D*)MCContainer->FindObject(ObjectNameMCPi0AccWOEvtWeights.Data());

          // secondary neutral pions histograms
          fHistoMCSecPi0SourcePt              = (TH2D*)MCContainer->FindObject(ObjectNameMCSecPi0.Data());
          fHistoMCSecPi0WAccSourcePt          = (TH2D*)MCContainer->FindObject(ObjectNameMCSecPi0Acc.Data());
          if (fHistoMCSecPi0SourcePt && fHistoMCSecPi0WAccSourcePt){
              fHistoMCSecPi0SourcePt->Sumw2();
              fHistoMCSecPi0WAccSourcePt->Sumw2();
              fNewMCOutput                     =  kTRUE;
          }

          if (debugOutputLevel>=1){cout << "Debug; ExtractSignalV2.C, line " << __LINE__ << endl;}
      }
      // Loading histograms for Eta
      else if( fMesonId == 221){
          // Histograms without acceptance requirement
          fHistoMCMesonPtWOWeights            = (TH1D*)MCContainer->FindObject(ObjectNameMCEtaWOWeights.Data());
          fHistoMCMesonPtWOEvtWeights         = (TH1D*)MCContainer->FindObject(ObjectNameMCEtaWOEvtWeights.Data());

          fHistoMCMesonPtWithinAcceptance     = (TH1D*)MCContainer->FindObject(ObjectNameMCEtaAcc.Data());
          fHistoMCMesonPt                     = (TH1D*)MCContainer->FindObject(ObjectNameMCEta.Data()); // (not the best; better having a 2D Pt_vs_Rapid in case we change limits)

          fHistoMCMesonPtWithinAcceptanceWOWeights    = (TH1D*)MCContainer->FindObject(ObjectNameMCEtaAccWOWeights.Data());
          fHistoMCMesonPtWithinAcceptanceWOEvtWeights = (TH1D*)MCContainer->FindObject(ObjectNameMCEtaAccWOEvtWeights.Data());
      }

      // Loading histograms for EtaPrime
      else if( fMesonId == 331 ){
          // Histograms without acceptance requirement
          fHistoMCMesonPt                     = (TH1D*)MCContainer->FindObject(ObjectNameMCEtaPrime.Data()); // (not the best; better having a 2D Pt_vs_Rapid in case we change limits)
          fHistoMCMesonPtWOWeights            = (TH1D*)MCContainer->FindObject(ObjectNameMCEtaPrimeWOWeights.Data());
          fHistoMCMesonPtWOEvtWeights         = (TH1D*)MCContainer->FindObject(ObjectNameMCEtaPrimeWOEvtWeights.Data());
          // Histograms with gammas in acceptance
          fHistoMCMesonPtWithinAcceptance     = (TH1D*)MCContainer->FindObject(ObjectNameMCEtaPrimeAcc.Data());
          fHistoMCMesonPtWithinAcceptanceWOWeights    = (TH1D*)MCContainer->FindObject(ObjectNameMCEtaPrimeAccWOWeights.Data());
          fHistoMCMesonPtWithinAcceptanceWOEvtWeights = (TH1D*)MCContainer->FindObject(ObjectNameMCEtaPrimeAccWOEvtWeights.Data());
          cout<<  ObjectNameMCEtaPrime.Data() << "\t" << fHistoMCMesonPt << "\t" <<  ObjectNameMCEtaPrimeWOWeights.Data() << "\t"<< fHistoMCMesonPtWOEvtWeights << endl;
      }

      // prepare histos for correct error calculation
      fHistoMCMesonPt->Sumw2();
      fHistoMCMesonPtWithinAcceptance->Sumw2();
      if (fHistoMCMesonPtWithinAcceptanceWOWeights) fHistoMCMesonPtWithinAcceptanceWOWeights->Sumw2();
      if (fHistoMCMesonPtWithinAcceptanceWOEvtWeights) fHistoMCMesonPtWithinAcceptanceWOEvtWeights->Sumw2();

      // calculate applied weights
      if (fHistoMCMesonPtWOWeights){
          fHistoMCMesonPtWeights              = (TH1D*)fHistoMCMesonPtWOWeights->Clone("WeightsMeson");
          fHistoMCMesonPtWeights->Divide(fHistoMCMesonPt,fHistoMCMesonPtWOWeights, 1.,1.,"B");
      }

      // load double counting histograms for Calo mode
      if (fMode == 4 || fMode == 12 || fMode == 5 || fMode==14){
          TH2F* fHistoTrue2DGammaDCClusPt     = (TH2F*)TrueConversionContainer->FindObject(ObjectNameDCGammaClusPt.Data());
          if (fHistoTrue2DGammaDCClusPt!=NULL) fEnableDCCluster= kTRUE;
          if (fEnableDCCluster){
              fHistoTrue2DGammaDCClusPt->Sumw2();
              fHistoTrueGammaDCClusPt         = (TH1D*)fHistoTrue2DGammaDCClusPt->ProjectionX("TrueClusGamma_Pt",0,-1,"e");
              if (debugOutputLevel>=1){cout << "Debug; ExtractSignalV2.C, line " << __LINE__ << endl;}
              cout << "Cluster DC found " << endl;
              fHistoTrueGammaClusMultipleCount    = (TH1F*)TrueConversionContainer->FindObject(ObjectNameGammaClusMultipleCount.Data());
              fHistoTrueGammaClusPt               = (TH1F*)TrueConversionContainer->FindObject("TrueClusGamma_Pt");
          }
      }

      fHistoTrueFullMesonInvMassVSPt              = (TH2D*)TrueConversionContainer->FindObject(ObjectNameTrueFull.Data());
      fHistoTrueMesonInvMassVSPt                  = (TH2D*)TrueConversionContainer->FindObject(ObjectNameTrue.Data());

      fHistoTrueMesonInvMassVSPtWOWeights         = (TH2D*)TrueConversionContainer->FindObject(ObjectNameTrueWOWeights.Data());
      fProfileTrueMesonInvMassVSPtWeights         = (TProfile2D*)TrueConversionContainer->FindObject(ObjectNameProfileWeights.Data());
      fHistoTrueMesonInvMassVSPtReweighted        = (TH2D*)fHistoTrueMesonInvMassVSPtWOWeights->Clone("Reweighted");
      fHistoTrueMesonInvMassVSPt->Sumw2();
      fHistoTrueFullMesonInvMassVSPt->Sumw2();
      fHistoTrueMesonInvMassVSPtWOWeights->Sumw2();
      fProfileTrueMesonInvMassVSPtWeights->Sumw2();
      fHistoTrueMesonInvMassVSPtReweighted->Sumw2();
      fHistoTrueMesonInvMassVSPtReweighted->Multiply(fProfileTrueMesonInvMassVSPtWeights);

      cout << ObjectNameTrue.Data() << endl;
      FillMassMCTrueMesonHistosArray(fHistoTrueMesonInvMassVSPt);
      FillMassMCTrueFullMesonHistosArray(fHistoTrueFullMesonInvMassVSPt);
      FillMassMCTrueReweightedMesonHistosArray(fHistoTrueMesonInvMassVSPtReweighted);
      FillMassMCTrueUnweightedMesonHistosArray(fHistoTrueMesonInvMassVSPtWOWeights);

      if (debugOutputLevel>=1){cout << "Debug; ExtractSignalV2.C, line " << __LINE__ << endl;}

      cout << "Mode: " << fMode << endl;

      if (fMode == 2 || fMode == 13 || fMode == 3 || fMode == 4 || fMode == 12 || fMode == 5 || fMode==14){
          fHistoTrueMesonCaloPhotonInvMassVSPt                = (TH2D*)TrueConversionContainer->FindObject(ObjectNameTrueCaloPhoton.Data());
          if (fHistoTrueMesonCaloPhotonInvMassVSPt==NULL) fAdvancedMesonQA = kFALSE;
          else fAdvancedMesonQA = kTRUE;
          cout << fAdvancedMesonQA << endl;
      }

      if (fMode == 0 || fMode == 1 || fMode == 9){
          fHistoTrueContBckInvMassVSPt                        = (TH2D*)TrueConversionContainer->FindObject(ObjectNameTrueContBck.Data());
          if (fHistoTrueContBckInvMassVSPt==NULL) fAdvancedMesonQA = kFALSE;
      }

      if (fAdvancedMesonQA) {
          if (fMode == 2 || fMode == 13 || fMode == 3){
              fHistoTrueMesonCaloPhotonInvMassVSPt                = (TH2D*)TrueConversionContainer->FindObject(ObjectNameTrueCaloPhoton.Data());
              FillMassMCTrueMesonCaloPhotonHistosArray(fHistoTrueMesonCaloPhotonInvMassVSPt);
              fHistoTrueMesonCaloConvPhotonInvMassVSPt            = (TH2D*)TrueConversionContainer->FindObject(ObjectNameTrueCaloConvPhoton.Data());
              FillMassMCTrueMesonCaloConvPhotonHistosArray(fHistoTrueMesonCaloConvPhotonInvMassVSPt);
              fHistoTrueMesonMergedClusterInvMassVSPt               = (TH2D*)TrueConversionContainer->FindObject(ObjectNameTrueCaloMerged.Data());
              FillMassMCTrueMesonCaloMergedClusterHistosArray(fHistoTrueMesonMergedClusterInvMassVSPt);
              fHistoTrueMesonMergedClusterPartConvInvMassVSPt     = (TH2D*)TrueConversionContainer->FindObject(ObjectNameTrueCaloMergedPartConv.Data());
              FillMassMCTrueMesonCaloMergedClusterPartConvHistosArray(fHistoTrueMesonMergedClusterPartConvInvMassVSPt);
          } else if (fMode == 4 || fMode == 12 || fMode == 5 || fMode==14){
              fHistoTrueMesonCaloPhotonInvMassVSPt                = (TH2D*)TrueConversionContainer->FindObject(ObjectNameTrueCaloPhoton.Data());
              FillMassMCTrueMesonCaloPhotonHistosArray(fHistoTrueMesonCaloPhotonInvMassVSPt);
              fHistoTrueMesonCaloConvPhotonInvMassVSPt            = (TH2D*)TrueConversionContainer->FindObject(ObjectNameTrueCaloConvPhoton.Data());
              FillMassMCTrueMesonCaloConvPhotonHistosArray(fHistoTrueMesonCaloConvPhotonInvMassVSPt);
              fHistoTrueMesonMixedCaloConvPhotonInvMassVSPt       = (TH2D*)TrueConversionContainer->FindObject(ObjectNameTrueMixedCaloConvPhoton.Data());
              FillMassMCTrueMesonMixedCaloConvPhotonHistosArray(fHistoTrueMesonMixedCaloConvPhotonInvMassVSPt);
              fHistoTrueMesonMergedClusterInvMassVSPt             = (TH2D*)TrueConversionContainer->FindObject(ObjectNameTrueCaloMerged.Data());
              FillMassMCTrueMesonCaloMergedClusterHistosArray(fHistoTrueMesonMergedClusterInvMassVSPt);
              fHistoTrueMesonMergedClusterPartConvInvMassVSPt     = (TH2D*)TrueConversionContainer->FindObject(ObjectNameTrueCaloMergedPartConv.Data());
              FillMassMCTrueMesonCaloMergedClusterPartConvHistosArray(fHistoTrueMesonMergedClusterPartConvInvMassVSPt);
          }

          fHistoYieldK0sWithPi0DaughterRec                        = (TH1D*)TrueConversionContainer->FindObject(ObjectNameK0sRecPi0.Data());
          if(fHistoYieldK0sWithPi0DaughterRec) fHistoYieldK0sWithPi0DaughterRec->Sumw2();
          fHistoYieldLambdaWithPi0DaughterRec                     = (TH1D*)TrueConversionContainer->FindObject(ObjectNameLambdaRecPi0.Data());
          if(fHistoYieldLambdaWithPi0DaughterRec) fHistoYieldLambdaWithPi0DaughterRec->Sumw2();
      }

      fHistoTrueContBckInvMassVSPt                        = (TH2D*)TrueConversionContainer->FindObject(ObjectNameTrueContBck.Data());
      fHistoTrueGGBckInvMassVSPt                          = (TH2D*)TrueConversionContainer->FindObject(ObjectNameTrueGGBck.Data());
      if(fHistoTrueContBckInvMassVSPt && fHistoTrueGGBckInvMassVSPt){
          fEnableNormBckHistoComparisonToTrueBck = kTRUE;
          FillMassMCTrueContBckHistosArray(fHistoTrueContBckInvMassVSPt);
          FillMassMCTrueGGBckHistosArray(fHistoTrueGGBckInvMassVSPt);
          fHistoTrueAllBckInvMassVSPt                         = (TH2D*)fHistoTrueGGBckInvMassVSPt->Clone(ObjectNameTrueAllBck.Data());
          fHistoTrueAllBckInvMassVSPt->Sumw2();
          fHistoTrueAllBckInvMassVSPt->Add(fHistoTrueContBckInvMassVSPt);
          FillMassMCTrueAllBckHistosArray(fHistoTrueAllBckInvMassVSPt);
          fHistoTrueFullMesonContainedInvMassVSPt      = (TH2D*)TrueConversionContainer->FindObject(ObjectNameTrueBckFullMesonContained.Data());
          fHistoTrueAsymEClusInvMassVSPt               = (TH2D*)TrueConversionContainer->FindObject(ObjectNameTrueBckAsymEClus.Data());
          if(fHistoTrueFullMesonContainedInvMassVSPt) FillMassMCTrueFullMesonContainedHistosArray(fHistoTrueFullMesonContainedInvMassVSPt);
          if(fHistoTrueAsymEClusInvMassVSPt) FillMassMCTrueAsymEClusHistosArray(fHistoTrueAsymEClusInvMassVSPt);
      }

      // histograms for secondaries
      if (meson.Contains("Pi0")){
          fHistoTrueSecMesonInvMassVSPt[0]                        = (TH2D*)TrueConversionContainer->FindObject(ObjectNameTrueSecFromK0S.Data());
          fHistoTrueSecMesonInvMassVSPt[1]                        = (TH2D*)TrueConversionContainer->FindObject(ObjectNameTrueSecFromLambda.Data());
          fHistoTrueSecMesonInvMassVSPt[2]                        = (TH2D*)TrueConversionContainer->FindObject(ObjectNameTrueSecFromK0L.Data());
          fHistoTrueSecMesonInvMassVSPt[3]                        = (TH2D*)TrueConversionContainer->FindObject(ObjectNameTrueSec.Data());

          for (Int_t j = 0; j<3; j++){
              if (fHistoTrueSecMesonInvMassVSPt[j]) fHistoTrueSecMesonInvMassVSPt[3]->Add(fHistoTrueSecMesonInvMassVSPt[j],-1);
          }
          FillMassMCTrueSecMesonHistosArray(fHistoTrueSecMesonInvMassVSPt);

      }

      if (debugOutputLevel>=1){cout << "Debug; ExtractSignalV2.C, line " << __LINE__ << endl;}
      fHistoTrueMesonDCInvMassVSPt                                = (TH2D*)TrueConversionContainer->FindObject(ObjectNameDCMesonInvMassPt.Data());
      if (fHistoTrueMesonDCInvMassVSPt!= NULL) fEnableDCMeson = kTRUE;
      if (debugOutputLevel>=1){cout << "Debug; ExtractSignalV2.C, line " << __LINE__ << endl;}
      if (fEnableDCMeson){
          FillMassMCTrueMesonDCHistosArray(fHistoTrueMesonDCInvMassVSPt);
          fHistoTrueMesonMultipleCount = (TH1F*) TrueConversionContainer->FindObject(ObjectNameMesonMultipleCount.Data());
      }

      if (debugOutputLevel>=1){cout << "Debug; ExtractSignalV2.C, line " << __LINE__ << endl;}
  }


  if(debugOutputLevel) cerr<<"Debug in ExtractSignalTemplate; LINE: "<<__LINE__<<endl;


  const char* FileDataLogname         = Form("%s/%s/%s_%s_EffiCheck_Templates_RAWDATA%s_%s.dat", cutSelection.Data(), fEnergyFlag.Data(), fPrefix.Data(), fPrefix2.Data(), fPeriodFlag.Data(),
                                      fCutSelectionRead.Data());
  fFileDataLog.open(FileDataLogname, ios::out);



    for(Int_t iPt=fStartPtBin;iPt<fNBinsPt;iPt++){ // BEGIN ANALYSIS for each Pt bin

      cout << "---------------------------------------------------------------------------------" << endl;
      cout << "Begin Analysis Pt Bin " << iPt << "\t range: " << fBinsPt[iPt] <<"-" << fBinsPt[iPt+1] << " rebin: " << fNRebin[iPt]  <<endl;
      cout << "---------------------------------------------------------------------------------" << endl;
      // Function to subtract GG minus Bck
      fFileDataLog << "---------------------------------------------------------------------------------" << endl;
      fFileDataLog << "----------------------------------new pT bin ------------------------------------" << endl;
      fFileDataLog << "---------------------------------------------------------------------------------" << endl;


      MakeFit(fHistoMappingGGInvMassPtBin[iPt], fHistoMappingBackInvMassPtBin[iPt], fHistoMappingTruePi0InvMassPtBins[iPt], fHistoMappingTrueEtaInvMassPtBins[iPt], false, iPt);

      //**********************************************//
      // ******  Get correct scaling parameters  *****//
      //**********************************************//
      int NFitPars = fTempFit->GetNpar();
      double scaleBack = 0;
      TF1* funcScaleBack = nullptr;
      double scalePi0 = 0;
      double scaleEta = 0;
      if(optionTemplateFit == 0){
        if(NFitPars == 3){
          scaleBack = fTempFit->GetParameter(0);
          scalePi0 = fTempFit->GetParameter(1);
          scaleEta = fTempFit->GetParameter(2);
          if(fScaleFacEtaToPi0 > 0 && fPrefix.CompareTo("Eta") == 0){
            scaleBack = fTempFit->GetParameter(0);
            scalePi0 = fTempFit->GetParameter(1);
            scaleEta = fTempFit->GetParameter(1) * fScaleFacEtaToPi0;
          }
        } else if(NFitPars == 2 && fPrefix.CompareTo("Eta") == 0){
          scaleBack = fTempFit->GetParameter(0);
          scalePi0 = 0;
          scaleEta = fTempFit->GetParameter(1);
        } else if(NFitPars == 2 && fPrefix.Contains("Pi0")){
          scaleBack = fTempFit->GetParameter(0);
          scalePi0 = fTempFit->GetParameter(1);
          scaleEta = 0;
        }
      } else if(optionTemplateFit == 1){
        if(NFitPars == 4){
          funcScaleBack = new TF1("funcScaleBack", "[0]*x+[1]", 0, 1);
          funcScaleBack->SetParameters(fTempFit->GetParameter(2), fTempFit->GetParameter(3));
          cout<<"fTempFit->GetParameter(0) "<<fTempFit->GetParameter(0)<<endl;
          scalePi0 = fTempFit->GetParameter(0);
          scaleEta = fTempFit->GetParameter(0) * fScaleFacEtaToPi0;
        }
      } else if(optionTemplateFit == 2){
        if(NFitPars == 5){
          scaleBack = fTempFit->GetParameter(2);
          funcScaleBack = new TF1("funcScaleBack", "[0]*x+[1]", 0, 1);
          funcScaleBack->SetParameters(fTempFit->GetParameter(3), fTempFit->GetParameter(4));
          cout<<"fTempFit->GetParameter(0) "<<fTempFit->GetParameter(0)<<endl;
          scalePi0 = fTempFit->GetParameter(0);
          scaleEta = fTempFit->GetParameter(0) * fScaleFacEtaToPi0;
        }
      }

      if(debugOutputLevel) cerr<<"Debug in ExtractSignalTemplate; LINE: "<<__LINE__<<endl;
      cout<<"\n\n";

      if(NFitPars == 4 && optionTemplateFit == 1 && funcScaleBack) fHistoMappingBackNormInvMassPtBin[iPt]->Multiply(funcScaleBack);
      else fHistoMappingBackNormInvMassPtBin[iPt]->Scale(scaleBack);
      if(NFitPars == 5 && optionTemplateFit == 2 && funcScaleBack) fHistoMappingBackNormInvMassPtBin[iPt]->Add(funcScaleBack);
      fHistoMappingTruePi0ScaledInvMassPtBins[iPt]->Scale(scalePi0);
      fHistoMappingTrueEtaScaledInvMassPtBins[iPt]->Scale(scaleEta);

      if(debugOutputLevel) cerr<<"Debug in ExtractSignalTemplate; LINE: "<<__LINE__<<endl;
      fHistoScaleFacBack->SetBinContent(iPt + 1,scaleBack);
      fHistoScaleFacBack->SetBinError(iPt + 1,fTempFit->GetParError(0));
      fHistoScaleFacPi0->SetBinContent(iPt + 1,scalePi0);
      fHistoScaleFacPi0->SetBinError(iPt + 1,fTempFit->GetParError(1));
      fHistoScaleFacEta->SetBinContent(iPt + 1,scaleEta);
      fHistoScaleFacEta->SetBinError(iPt + 1,fTempFit->GetParError(2));

      if(debugOutputLevel) cerr<<"Debug in ExtractSignalTemplate; LINE: "<<__LINE__<<endl;
      fHistoMappingSignalInvMassPtBin[iPt] = SubtractBackground(fHistoMappingGGInvMassPtBin[iPt], fHistoMappingBackNormInvMassPtBin[iPt], fHistoMappingTruePi0ScaledInvMassPtBins[iPt], fHistoMappingTrueEtaScaledInvMassPtBins[iPt]);

      fHistoMappingSignalInvMassPtBin[iPt]->SetName(Form("fHistoMappingSignalInvMass_in_Pt_Bin%02d",iPt));


      //*********************************************************************************
      // Right Side
      //*********************************************************************************
        if(debugOutputLevel) cerr<<"Debug in ExtractSignalTemplate; LINE: "<<__LINE__<<endl;
        if(!fHistoMappingSignalInvMassPtBin[iPt]) cout<<"shit...\n";
        FitSubtractedInvMassInPtBinsWOLinear(fHistoMappingSignalInvMassPtBin[iPt], fMesonIntDeltaRange, iPt, false);
        if(debugOutputLevel) cerr<<"Debug in ExtractSignalTemplate; LINE: "<<__LINE__<<endl;
        fFitSignalInvMassPtBin[iPt]                 = fFitReco;
        fFitSignalPeakPosInvMassPtBin[iPt]          = fFitGausExp;
        fFitBckInvMassPtBin[iPt]                    = fFitLinearBck;
        fMesonYieldsResidualBckFunc[0][iPt]         = fIntLinearBck;
        fMesonYieldsResidualBckFuncError[0][iPt]    = fIntLinearBckError;
        if (fFitReco)
            fMesonChi2[0][iPt]                      = fFitReco->GetChisquare()/fFitReco->GetNDF();
        else
            fMesonChi2[0][iPt]                      = -1  ;
        fMesonMass[iPt] = fFitReco->GetParameter(1);
        fMesonMassError[iPt] = fFitReco->GetParError(1);

        MakeBGFitSubtractedInvMassInPtBins(fHistoMappingSignalInvMassPtBin[iPt], fMesonIntDeltaRange,iPt,kFALSE,Form("ExpFitFuncNormalBin%02d",iPt),kFALSE);
        fHistoMappingSignalRemainingBGSubInvMassPtBin[iPt]  = (TH1D*)fCopySignal->Clone(Form("histoSignalRemainingBGSubtractedBin%02d",iPt));
        fHistoMappingRemainingBGInvMassPtBin[iPt]           = (TH1D*)fCopyOnlyBG->Clone(Form("histoRemainingBGBin%02d",iPt));
        fFitBGSubtractedSignalInvMassPtBin[iPt]             = fFitReco;
        fFitRemainingBGInvMassPtBin[iPt]                    = fFitLinearBck;
        fFitBGSubtractedSignalPeakPosInvMassPtBin[iPt]      = fFitGausExp;
        if (debugOutputLevel>=1){cout<<"Debug Output; ExtractSignalV2.C; Line: "<<__LINE__<<" "<<endl;}


        // assign the correct histos (there are multiple cses in ExtractSignalV2. Here this would not be necessary but this way they are compatible)
        fFitSignalInvMassPtBinStd[iPt]              = fFitSignalInvMassPtBin[iPt];
        fFitBckInvMassPtBinStd[iPt]                 = fFitBckInvMassPtBin[iPt];
        fFitSignalPeakPosInvMassPtBinStd[iPt]       = fFitSignalPeakPosInvMassPtBin[iPt];
        fMesonYieldsResidualBckFuncStd[0][iPt]      = fMesonYieldsResidualBckFunc[0][iPt];
        fMesonYieldsResidualBckFuncErrorStd[0][iPt] = fMesonYieldsResidualBckFuncError[0][iPt];

        //Get FWHM
        CalculateFWHM(fFitSignalInvMassPtBinStd[iPt]);
        fMesonFWHM[iPt] = fFWHMFunc;
        fMesonFWHMError[iPt] = fFWHMFuncError;
        if (fFitSignalInvMassPtBinStd[iPt] !=0x00){
            fMesonMass[iPt]                 = fFitSignalInvMassPtBinStd[iPt]->GetParameter(1);
            fMesonMassError[iPt]            = fFitSignalInvMassPtBinStd[iPt]->GetParError(1);

            fMesonLambdaTailpar[iPt]        = fFitSignalInvMassPtBinStd[iPt]->GetParameter(3);
            fMesonLambdaTailparError[iPt]   = fFitSignalInvMassPtBinStd[iPt]->GetParError(3);

            if(fFitSignalInvMassPtBinStd[iPt]->GetNpar() == 7){
              fMesonLambdaTailRightpar[iPt]        = fFitSignalInvMassPtBinStd[iPt]->GetParameter(6);
              fMesonLambdaTailRightparError[iPt]   = fFitSignalInvMassPtBinStd[iPt]->GetParError(6);
            }

            fMesonAmplitudepar[iPt]         = fFitSignalInvMassPtBinStd[iPt]->GetParameter(0);
            fMesonAmplitudeparError[iPt]    = fFitSignalInvMassPtBinStd[iPt]->GetParError(0);
            fMesonSigmapar[iPt]             = fFitSignalInvMassPtBinStd[iPt]->GetParameter(2);
            fMesonSigmaparError[iPt]        = fFitSignalInvMassPtBinStd[iPt]->GetParError(2);

            fMesonCurIntRange[0][0]         = fMesonMass[iPt] + fMesonIntDeltaRange[0];
            fMesonCurIntRange[1][0]         = fMesonMass[iPt] + fMesonIntDeltaRangeWide[0];
            fMesonCurIntRange[2][0]         = fMesonMass[iPt] + fMesonIntDeltaRangeNarrow[0];
            fMesonCurIntRange[0][1]         = fMesonMass[iPt] + fMesonIntDeltaRange[1];
            fMesonCurIntRange[1][1]         = fMesonMass[iPt] + fMesonIntDeltaRangeWide[1];
            fMesonCurIntRange[2][1]         = fMesonMass[iPt] + fMesonIntDeltaRangeNarrow[1];

        } else {
            fMesonMass[iPt]                 = fMesonMassExpect;
            fMesonMassError[iPt]            = 0.;
            fMesonCurIntRange[0][0]         = fMesonMassExpect + fMesonIntDeltaRange[0];
            fMesonCurIntRange[1][0]         = fMesonMassExpect + fMesonIntDeltaRangeWide[0];
            fMesonCurIntRange[2][0]         = fMesonMassExpect + fMesonIntDeltaRangeNarrow[0];
            fMesonCurIntRange[0][1]         = fMesonMassExpect + fMesonIntDeltaRange[1];
            fMesonCurIntRange[1][1]         = fMesonMassExpect + fMesonIntDeltaRangeWide[1];
            fMesonCurIntRange[2][1]         = fMesonMassExpect + fMesonIntDeltaRangeNarrow[1];
            fMesonLambdaTailpar[iPt]        = -1;
            fMesonLambdaTailparError[iPt]   = 1;

            fMesonLambdaTailRightpar[iPt]        = -1;
            fMesonLambdaTailRightparError[iPt]   = 1;

            fMesonAmplitudepar[iPt]         = -1;
            fMesonAmplitudeparError[iPt]    = 1;
            fMesonSigmapar[iPt]             = -1;
            fMesonSigmaparError[iPt]        = 1;

        }

        if (debugOutputLevel>=1){cout<<"Debug Output; ExtractSignalV2.C; Line: "<<__LINE__<<" "<<endl;}
        for (Int_t k = 0; k < 3; k++){
            fMassWindowHigh[k][iPt]        = fMesonCurIntRange[k][1];
            fMassWindowLow[k][iPt]         = fMesonCurIntRange[k][0];
        }

        if (debugOutputLevel>=1){cout<<"Debug Output; ExtractSignalV2.C; Line: "<<__LINE__<<" "<<endl;}

        FitSubtractedPureGaussianInvMassInPtBins(fHistoMappingSignalInvMassPtBin[iPt],iPt);
        if (debugOutputLevel>=1){cout<<"Debug Output; ExtractSignalV2.C; Line: "<<__LINE__<<" "<<endl;}
        fFitSignalGaussianInvMassPtBin[iPt] = fFitReco;
        if (fFitSignalGaussianInvMassPtBin[iPt] !=0x00){
            fMesonMassGaussian[iPt]         = fFitSignalGaussianInvMassPtBin[iPt]->GetParameter(1);
            fMesonMassGaussianError[iPt]    = fFitSignalGaussianInvMassPtBin[iPt]->GetParError(1);
            fMesonWidthGaussian[iPt]        = fFitSignalGaussianInvMassPtBin[iPt]->GetParameter(2);
            fMesonWidthGaussianError[iPt]   = fFitSignalGaussianInvMassPtBin[iPt]->GetParError(2);
        } else {
            fMesonMassGaussian[iPt]         = 0.;
            fMesonMassGaussianError[iPt]    = 0.;
            fMesonWidthGaussian[iPt]        = 0.;
            fMesonWidthGaussianError[iPt]   = 0.;
        }
        if (debugOutputLevel>=1){cout<<"Debug Output; ExtractSignalV2.C; Line: "<<__LINE__<<" "<<endl;}


        for (Int_t k = 0; k < 3; k++){
          if (debugOutputLevel>=1){cout<<"Debug Output; ExtractSignalV2.C; Line: "<<__LINE__<<" "<<endl;}
            IntegrateHistoInvMass( fHistoMappingGGInvMassPtBin[iPt], fMesonCurIntRange[k]);
            fGGYields[k][iPt]               = fYields;
            fGGYieldsError[k][iPt]          = fYieldsError;
            if (debugOutputLevel>=1){cout<<"Debug Output; ExtractSignalV2.C; Line: "<<__LINE__<<" "<<endl;}

            if (k == 0){
                fHistoMappingBackNormInvMassPtBin[iPt]->Sumw2();
                fHistoMappingBackNormInvMassPtBin[iPt]->Add(fHistoMappingRemainingBGInvMassPtBin[iPt],1);
            }
            if (debugOutputLevel>=1){cout<<"Debug Output; ExtractSignalV2.C; Line: "<<__LINE__<<" "<<endl;}
            IntegrateHistoInvMass( fHistoMappingBackNormInvMassPtBin[iPt], fMesonCurIntRange[k]);
            if (debugOutputLevel>=1){cout<<"Debug Output; ExtractSignalV2.C; Line: "<<__LINE__<<" "<<endl;}
            fBckYields[k][iPt]              = fYields;
            fBckYieldsError[k][iPt]         = fYieldsError;

            // Integrate the signal histo
            fFileDataLog<< endl <<"Signal histo "<< nameIntRange[k].Data() << ":\t" << fBinsPt[iPt] <<"-" << fBinsPt[iPt+1] << endl;
            if (debugOutputLevel>=1){cout<<"Debug Output; ExtractSignalV2.C; Line: "<<__LINE__<<" "<<endl;}
            IntegrateHistoInvMass( fHistoMappingSignalRemainingBGSubInvMassPtBin[iPt], fMesonCurIntRange[k]);
            if (debugOutputLevel>=1){cout<<"Debug Output; ExtractSignalV2.C; Line: "<<__LINE__<<" "<<endl;}
            fMesonYields[k][iPt]            = fYields;
            fMesonYieldsError[k][iPt]       = fYieldsError;
            fFileDataLog << "Integrated value: \t" << fYields <<"+-" <<fYieldsError <<endl;
        }


        if (debugOutputLevel>=1){cout<<"Debug Output; ExtractSignalV2.C; Line: "<<__LINE__<<" "<<endl;}



        if(fIsMC){
            if (debugOutputLevel>=1){cout<<"Debug Output; ExtractSignalV2.C; Line: "<<__LINE__<<" "<<endl;}
            fFileDataLog<< endl <<"True histo normal range" << fBinsPt[iPt] <<"-" << fBinsPt[iPt+1]<< endl;
            fFitTrueSignalInvMassPtBin[iPt]=0x00;
            if(fCrysFitting==0){
                fFileErrLog << "Using exp fit"<<endl;
                cout << "default" << endl;
                if (debugOutputLevel>=1){cout<<"Debug Output; ExtractSignalV2.C; Line: "<<__LINE__<<" "<<endl;}
                FitTrueInvMassInPtBins(fHistoMappingTrueMesonInvMassPtBins[iPt], fMesonIntDeltaRange,iPt,kFALSE);
                if (debugOutputLevel>=1){cout<<"Debug Output; ExtractSignalV2.C; Line: "<<__LINE__<<" "<<endl;}
            } else {
                fFileErrLog << "Using Crystal Ball function"<<endl;
                FitCBSubtractedInvMassInPtBins(fHistoMappingTrueMesonInvMassPtBins[iPt], fMesonIntDeltaRange,iPt,kFALSE,Form("CBFitFuncMCTrueBin%02d",iPt),kTRUE);
            }

            if (fHistoMappingTrueMesonInvMassPtBins[iPt]->GetEntries() !=0){
                fFitTrueSignalInvMassPtBin[iPt]     = fFitReco;
                if (fFitTrueSignalInvMassPtBin[iPt] != 0x00){
                    fMesonTrueAmplitudepar[iPt]     = fFitTrueSignalInvMassPtBin[iPt]->GetParameter(0);
                    fMesonTrueAmplitudeparError[iPt] = fFitTrueSignalInvMassPtBin[iPt]->GetParError(0);
                    fMesonTrueMass[iPt]             = fFitTrueSignalInvMassPtBin[iPt]->GetParameter(1);
                    fMesonTrueMassError[iPt]        = fFitTrueSignalInvMassPtBin[iPt]->GetParError(1);
                    fMesonTrueSigmapar[iPt]         = fFitTrueSignalInvMassPtBin[iPt]->GetParameter(2);
                    fMesonTrueSigmaparError[iPt]    = fFitTrueSignalInvMassPtBin[iPt]->GetParError(2);
                    fMesonLambdaTailMCpar[iPt]      = fFitTrueSignalInvMassPtBin[iPt]->GetParameter(3);
                    fMesonLambdaTailMCparError[iPt] = fFitTrueSignalInvMassPtBin[iPt]->GetParError(3);
                    if(fFitTrueSignalInvMassPtBin[iPt]->GetNpar() == 7){
                      fMesonLambdaTailRightMCpar[iPt]      = fFitTrueSignalInvMassPtBin[iPt]->GetParameter(6);
                      fMesonLambdaTailRightMCparError[iPt] = fFitTrueSignalInvMassPtBin[iPt]->GetParError(6);
                    }

                    CalculateFWHM(fFitTrueSignalInvMassPtBin[iPt]);
                    fMesonTrueFWHM[iPt]             = fFWHMFunc;
                    fMesonTrueFWHMError[iPt]        = fFWHMFuncError;
                    fFileDataLog << "TrueFWHM \t" << fMesonTrueFWHM[iPt] << "\t +-" << fMesonTrueFWHMError[iPt] << endl;
                    fMesonTrueIntRange[0][0]        = fMesonTrueMass[iPt] + fMesonIntDeltaRange[0];
                    fMesonTrueIntRange[1][0]        = fMesonTrueMass[iPt] + fMesonIntDeltaRangeWide[0];
                    fMesonTrueIntRange[2][0]        = fMesonTrueMass[iPt] + fMesonIntDeltaRangeNarrow[0];
                    fMesonTrueIntRange[0][1]        = fMesonTrueMass[iPt] + fMesonIntDeltaRange[1] ;
                    fMesonTrueIntRange[1][1]        = fMesonTrueMass[iPt] + fMesonIntDeltaRangeWide[1];
                    fMesonTrueIntRange[2][1]        = fMesonTrueMass[iPt] + fMesonIntDeltaRangeNarrow[1];
                } else {
                    fMesonTrueMass[iPt]             = 0.;
                    fMesonTrueMassError[iPt]        = 1.;
                    fMesonTrueFWHM[iPt]             = 0.;
                    fMesonTrueFWHMError[iPt]        = 0.;
                    fMesonTrueIntRange[0][0]        = fMesonMassExpect + fMesonIntDeltaRange[0];
                    fMesonTrueIntRange[1][0]        = fMesonMassExpect + fMesonIntDeltaRangeWide[0];
                    fMesonTrueIntRange[2][0]        = fMesonMassExpect + fMesonIntDeltaRangeNarrow[0];
                    fMesonTrueIntRange[0][1]        = fMesonMassExpect + fMesonIntDeltaRange[1];
                    fMesonTrueIntRange[1][1]        = fMesonMassExpect + fMesonIntDeltaRangeWide[1];
                    fMesonTrueIntRange[2][1]        = fMesonMassExpect + fMesonIntDeltaRangeNarrow[1];
                }
            }
            if (debugOutputLevel>=1){cout<<"Debug Output; ExtractSignalV2.C; Line: "<<__LINE__<<" "<<endl;}
            IntegrateHistoInvMassStream( fHistoMappingTrueFullMesonInvMassPtBins[iPt], fIntFixedRange);
            fMesonTrueYieldFixedWindow[iPt]         = fYields;
            fMesonTrueYieldErrorFixedWindow[iPt]    = fYieldsError;

            TFitResultPtr lFitResult = nullptr;
            FitTrueInvMassPureGaussianInPtBins(fHistoMappingTrueMesonInvMassPtBins[iPt],iPt,lFitResult);
            if (debugOutputLevel>=1){cout<<"Debug Output; ExtractSignalV2.C; Line: "<<__LINE__<<" "<<endl;}
            if (fHistoMappingTrueMesonInvMassPtBins[iPt]->GetEntries() !=0){
                if (debugOutputLevel>=1){cout<<"Debug Output; ExtractSignalV2.C; Line: "<<__LINE__<<" "<<endl;}
                fFitTrueSignalGaussianInvMassPtBin[iPt] = fFitReco;
                if (fFitTrueSignalGaussianInvMassPtBin[iPt] !=0x00){
                    if (debugOutputLevel>=1){cout<<"Debug Output; ExtractSignalV2.C; Line: "<<__LINE__<<" "<<endl;}
                    fMesonTrueMassGaussian[iPt]         = fFitTrueSignalGaussianInvMassPtBin[iPt]->GetParameter(1);
                    fMesonTrueMassGaussianError[iPt]    = fFitTrueSignalGaussianInvMassPtBin[iPt]->GetParError(1);
                    fMesonTrueWidthGaussian[iPt]        = fFitTrueSignalGaussianInvMassPtBin[iPt]->GetParameter(2);
                    fMesonTrueWidthGaussianError[iPt]   = fFitTrueSignalGaussianInvMassPtBin[iPt]->GetParError(2);
                } else {
                    if (debugOutputLevel>=1){cout<<"Debug Output; ExtractSignalV2.C; Line: "<<__LINE__<<" "<<endl;}
                    fMesonTrueMassGaussian[iPt]         = 0.;
                    fMesonTrueMassGaussianError[iPt]    = 0.;
                    fMesonTrueWidthGaussian[iPt]        = 0.;
                    fMesonTrueWidthGaussianError[iPt]   = 0.;
                }
                if (debugOutputLevel>=1){cout<<"Debug Output; ExtractSignalV2.C; Line: "<<__LINE__<<" "<<endl;}
                for (Int_t k = 0; k< 3;k++){
                    if (debugOutputLevel>=1){cout<<"Debug Output; ExtractSignalV2.C; Line: "<<__LINE__<<" "<<endl;}
                    // cout<< endl <<"True histo " << nameIntRange[k].Data() << " integration from fit, range" << fBinsPt[iPt] <<"-" << fBinsPt[iPt+1]<< endl;
                    IntegrateFitFuncAndError( fFitTrueSignalGaussianInvMassPtBin[iPt], lFitResult, fHistoMappingTrueMesonInvMassPtBins[iPt], fMesonTrueIntRange[k]);
                    if (debugOutputLevel>=1){cout<<"Debug Output; ExtractSignalV2.C; Line: "<<__LINE__<<" "<<endl;}
                    fMesonTrueYieldsFromFit[k][iPt]                        = fYieldsFunc;
                    if (debugOutputLevel>=1){cout<<"Debug Output; ExtractSignalV2.C; Line: "<<__LINE__<<" "<<endl;}
                    fMesonTrueYieldsFromFitError[k][iPt]                   = fYieldsFuncError;
                    // cout << "Integrated value: \t" << fYieldsFunc <<"+-" <<fYieldsFuncError<<endl;
                }
                if (debugOutputLevel>=1){cout<<"Debug Output; ExtractSignalV2.C; Line: "<<__LINE__<<" "<<endl;}
            }

            fFitTrueSignalInvMassPtReweightedBin[iPt]   = 0x00;
            if (debugOutputLevel>=1){cout<<"Debug Output; ExtractSignalV2.C; Line: "<<__LINE__<<" "<<endl;}
            if(fHistoMappingTrueMesonInvMassPtReweightedBins[iPt]){
                cout << "Using exp fit"<<endl;
                fFileErrLog << "Using exp fit"<<endl;
                cout << "reweighted" << endl;
                if(fCrysFitting==0){
                    fFileErrLog << "Using exp fit"<<endl;
                    FitTrueInvMassInPtBins(fHistoMappingTrueMesonInvMassPtReweightedBins[iPt], fMesonIntDeltaRange,iPt,kFALSE);
                } else {
                    fFileErrLog << "Using Crystal Ball function"<<endl;
                    FitCBSubtractedInvMassInPtBins(fHistoMappingTrueMesonInvMassPtReweightedBins[iPt], fMesonIntDeltaRange,iPt,kFALSE,Form("CBFitFuncMCTrueBinReweighted%02d",iPt),kTRUE);
                }

                if (fHistoMappingTrueMesonInvMassPtReweightedBins[iPt]->GetEntries() !=0){
                    fFitTrueSignalInvMassPtReweightedBin[iPt]   = fFitReco;
                    if (fFitTrueSignalInvMassPtReweightedBin[iPt] != 0x00){
                        fMesonTrueMassReweighted[iPt]           = fFitTrueSignalInvMassPtReweightedBin[iPt]->GetParameter(1);
                        fMesonTrueMassReweightedError[iPt]      = fFitTrueSignalInvMassPtReweightedBin[iPt]->GetParError(1);
                        CalculateFWHM(fFitTrueSignalInvMassPtReweightedBin[iPt]);
                        fMesonTrueFWHMReweighted[iPt]           = fFWHMFunc;
                        fMesonTrueFWHMReweightedError[iPt]      = fFWHMFuncError;
                        fMesonTrueIntReweightedRange[0][0]      = fMesonTrueMassReweighted[iPt] + fMesonIntDeltaRange[0];
                        fMesonTrueIntReweightedRange[1][0]      = fMesonTrueMassReweighted[iPt] + fMesonIntDeltaRangeWide[0];
                        fMesonTrueIntReweightedRange[2][0]      = fMesonTrueMassReweighted[iPt] + fMesonIntDeltaRangeNarrow[0];
                        fMesonTrueIntReweightedRange[0][1]      = fMesonTrueMassReweighted[iPt] + fMesonIntDeltaRange[1];
                        fMesonTrueIntReweightedRange[1][1]      = fMesonTrueMassReweighted[iPt] + fMesonIntDeltaRangeWide[1];
                        fMesonTrueIntReweightedRange[2][1]      = fMesonTrueMassReweighted[iPt] + fMesonIntDeltaRangeNarrow[1];
                    } else {
                        fMesonTrueMassReweighted[iPt]           = 0.;
                        fMesonTrueMassReweightedError[iPt]      = 1.;
                        fMesonTrueFWHMReweighted[iPt]           = 0.;
                        fMesonTrueFWHMReweightedError[iPt]      = 0.;
                        fMesonTrueIntReweightedRange[0][0]      = fMesonMassExpect + fMesonIntDeltaRange[0];
                        fMesonTrueIntReweightedRange[1][0]      = fMesonMassExpect + fMesonIntDeltaRangeWide[0];
                        fMesonTrueIntReweightedRange[2][0]      = fMesonMassExpect + fMesonIntDeltaRangeNarrow[0];
                        fMesonTrueIntReweightedRange[0][1]      = fMesonMassExpect + fMesonIntDeltaRange[1];
                        fMesonTrueIntReweightedRange[1][1]      = fMesonMassExpect + fMesonIntDeltaRangeWide[1];
                        fMesonTrueIntReweightedRange[2][1]      = fMesonMassExpect + fMesonIntDeltaRangeNarrow[1];

                    }
                }
            }

            fFitTrueSignalInvMassPtUnweightedBin[iPt]=0x00;
            //cout << "Debug; ExtractSignalV2.C, line " << __LINE__ << endl;
            if(fHistoMappingTrueMesonInvMassPtUnweightedBins[iPt]){
                cout << "Using exp fit"<<endl;
                cout << "unweighted" << endl;
                fFileErrLog << "Using exp fit"<<endl;
                if(fCrysFitting==0){
                    fFileErrLog << "Using exp fit"<<endl;
                    FitTrueInvMassInPtBins(fHistoMappingTrueMesonInvMassPtUnweightedBins[iPt], fMesonIntDeltaRange,iPt,kFALSE);
                } else {
                    fFileErrLog << "Using Crystal Ball function"<<endl;
                    FitCBSubtractedInvMassInPtBins(fHistoMappingTrueMesonInvMassPtUnweightedBins[iPt], fMesonIntDeltaRange,iPt,kFALSE,Form("CBFitFuncMCTrueBinUnweighted%02d",iPt),kTRUE);
                }

                if (fHistoMappingTrueMesonInvMassPtUnweightedBins[iPt]->GetEntries() !=0){
                    fFitTrueSignalInvMassPtUnweightedBin[iPt] = fFitReco;
                    if (fFitTrueSignalInvMassPtUnweightedBin[iPt] != 0x00){
                    fMesonTrueMassUnweighted[iPt]               = fFitTrueSignalInvMassPtUnweightedBin[iPt]->GetParameter(1);
                    fMesonTrueMassUnweightedError[iPt]          = fFitTrueSignalInvMassPtUnweightedBin[iPt]->GetParError(1);
                    CalculateFWHM(fFitTrueSignalInvMassPtUnweightedBin[iPt]);
                    fMesonTrueFWHMUnweighted[iPt]               = fFWHMFunc;
                    fMesonTrueFWHMUnweightedError[iPt]          = fFWHMFuncError;
                        fMesonTrueIntUnweightedRange[0][0]      = fMesonTrueMassUnweighted[iPt] + fMesonIntDeltaRange[0];
                        fMesonTrueIntUnweightedRange[1][0]      = fMesonTrueMassUnweighted[iPt] + fMesonIntDeltaRangeWide[0];
                        fMesonTrueIntUnweightedRange[2][0]      = fMesonTrueMassUnweighted[iPt] + fMesonIntDeltaRangeNarrow[0];
                        fMesonTrueIntUnweightedRange[0][1]      = fMesonTrueMassUnweighted[iPt] + fMesonIntDeltaRange[1];
                        fMesonTrueIntUnweightedRange[1][1]      = fMesonTrueMassUnweighted[iPt] + fMesonIntDeltaRangeWide[1];
                        fMesonTrueIntUnweightedRange[2][1]      = fMesonTrueMassUnweighted[iPt] + fMesonIntDeltaRangeNarrow[1];
                    } else {
                        fMesonTrueMassUnweighted[iPt]           = 0.;
                        fMesonTrueMassUnweightedError[iPt]      = 1.;
                        fMesonTrueFWHMUnweighted[iPt]           = 0.;
                        fMesonTrueFWHMUnweightedError[iPt]      = 0.;
                        fMesonTrueIntUnweightedRange[0][0]      = fMesonMassExpect + fMesonIntDeltaRange[0];
                        fMesonTrueIntUnweightedRange[1][0]      = fMesonMassExpect + fMesonIntDeltaRangeWide[0];
                        fMesonTrueIntUnweightedRange[2][0]      = fMesonMassExpect + fMesonIntDeltaRangeNarrow[0];
                        fMesonTrueIntUnweightedRange[0][1]      = fMesonMassExpect + fMesonIntDeltaRange[1];
                        fMesonTrueIntUnweightedRange[1][1]      = fMesonMassExpect + fMesonIntDeltaRangeWide[1];
                        fMesonTrueIntUnweightedRange[2][1]      = fMesonMassExpect + fMesonIntDeltaRangeNarrow[1];

                    }
                }
            }
            if (fAdvancedMesonQA && (fMode == 2 || fMode == 13 || fMode == 3 || fMode == 4 || fMode == 12 || fMode == 5 || fMode==14)){
                if (fHistoMappingTrueMesonCaloPhotonInvMassPtBins[iPt]->GetEntries() !=0){
                    if(fCrysFitting==0){
                        fFileErrLog << "Using exp fit"<<endl;
                        FitTrueInvMassInPtBins(fHistoMappingTrueMesonCaloPhotonInvMassPtBins[iPt], fMesonIntDeltaRange,iPt,kFALSE);
                    } else {
                        fFileErrLog << "Using Crystal Ball function"<<endl;
                        FitCBSubtractedInvMassInPtBins(fHistoMappingTrueMesonCaloPhotonInvMassPtBins[iPt], fMesonIntDeltaRange,iPt,kFALSE,Form("CBFitFuncMCTrueBinCaloPhoton%02d",iPt),kTRUE);
                    }

                    fFitTrueSignalCaloPhotonInvMassPtBin[iPt]   = fFitReco;
                    if (fFitTrueSignalCaloPhotonInvMassPtBin[iPt] != 0x00){
                        fMesonTrueMassCaloPhoton[iPt]           = fFitTrueSignalCaloPhotonInvMassPtBin[iPt]->GetParameter(1);
                        fMesonTrueMassErrorCaloPhoton[iPt]      = fFitTrueSignalCaloPhotonInvMassPtBin[iPt]->GetParError(1);
                        CalculateFWHM(fFitTrueSignalCaloPhotonInvMassPtBin[iPt]);
                        fMesonTrueFWHMCaloPhoton[iPt]           = fFWHMFunc;
                        fMesonTrueFWHMErrorCaloPhoton[iPt]      = fFWHMFuncError;
                    } else {
                        fMesonTrueMassCaloPhoton[iPt]           = 0.;
                        fMesonTrueMassErrorCaloPhoton[iPt]      = 1.;
                        fMesonTrueFWHMCaloPhoton[iPt]           = 0.;
                        fMesonTrueFWHMErrorCaloPhoton[iPt]      = 0.;
                    }

                    IntegrateHistoInvMassStream( fHistoMappingTrueMesonCaloPhotonInvMassPtBins[iPt], fIntFixedRange);
                    fMesonTrueYieldGammaFixedWindow[iPt]        = fYields;
                    fMesonTrueYieldGammaErrorFixedWindow[iPt]   = fYieldsError;

                }
                if (fHistoMappingTrueMesonCaloConvPhotonInvMassPtBins[iPt]->GetEntries() !=0){
                    if(fCrysFitting==0){
                        fFileErrLog << "Using exp fit"<<endl;
                        FitTrueInvMassInPtBins(fHistoMappingTrueMesonCaloConvPhotonInvMassPtBins[iPt], fMesonIntDeltaRange,iPt,kFALSE);
                    } else {
                        fFileErrLog << "Using Crystal Ball function"<<endl;
                        FitCBSubtractedInvMassInPtBins(fHistoMappingTrueMesonCaloConvPhotonInvMassPtBins[iPt],
                                                    fMesonIntDeltaRange,iPt,kFALSE,Form("CBFitFuncMCTrueBinConvCaloPhoton%02d",iPt),kTRUE);
                    }
                    fFitTrueSignalCaloConvPhotonInvMassPtBin[iPt]   = fFitReco;
                    if (fFitTrueSignalCaloConvPhotonInvMassPtBin[iPt] != 0x00){
                        fMesonTrueMassCaloConvPhoton[iPt]           = fFitTrueSignalCaloConvPhotonInvMassPtBin[iPt]->GetParameter(1);
                        fMesonTrueMassErrorCaloConvPhoton[iPt]      = fFitTrueSignalCaloConvPhotonInvMassPtBin[iPt]->GetParError(1);
                        CalculateFWHM(fFitTrueSignalCaloConvPhotonInvMassPtBin[iPt]);
                        fMesonTrueFWHMCaloConvPhoton[iPt]           = fFWHMFunc;
                        fMesonTrueFWHMErrorCaloConvPhoton[iPt]      = fFWHMFuncError;
                    } else {
                        fMesonTrueMassCaloConvPhoton[iPt]           = 0.;
                        fMesonTrueMassErrorCaloConvPhoton[iPt]      = 1.;
                        fMesonTrueFWHMCaloConvPhoton[iPt]           = 0.;
                        fMesonTrueFWHMErrorCaloConvPhoton[iPt]      = 0.;
                    }

                    IntegrateHistoInvMassStream( fHistoMappingTrueMesonCaloConvPhotonInvMassPtBins[iPt], fIntFixedRange);
                    if (fMode == 4 || fMode == 12 || fMode ==5 || fMode==14){
                        fMesonTrueYieldConvGammaConvGammaFixedWindow[iPt]       = fYields;
                        fMesonTrueYieldConvGammaConvGammaErrorFixedWindow[iPt]  = fYieldsError;
                    } else {
                        fMesonTrueYieldGammaConvGammaFixedWindow[iPt]           = fYields;
                        fMesonTrueYieldGammaConvGammaErrorFixedWindow[iPt]      = fYieldsError;
                        fMesonTrueYieldConvGammaConvGammaFixedWindow[iPt]       = 0;
                        fMesonTrueYieldConvGammaConvGammaErrorFixedWindow[iPt]  = 0;
                    }

                }
                if (fHistoMappingTrueMesonCaloMergedClusterInvMassPtBins[iPt]->GetEntries() !=0){
                    if(fCrysFitting==0){
                        fFileErrLog << "Using exp fit"<<endl;
                        FitTrueInvMassInPtBins(fHistoMappingTrueMesonCaloMergedClusterInvMassPtBins[iPt], fMesonIntDeltaRange,iPt,kFALSE);
                    } else {
                        fFileErrLog << "Using Crystal Ball function"<<endl;
                        FitCBSubtractedInvMassInPtBins(fHistoMappingTrueMesonCaloMergedClusterInvMassPtBins[iPt],
                                                    fMesonIntDeltaRange,iPt,kFALSE,Form("CBFitFuncMCTrueBinCaloMergedCluster%02d",iPt),kTRUE);
                    }

                    fFitTrueSignalCaloMergedClusterInvMassPtBin[iPt]    = fFitReco;
                    if (fFitTrueSignalCaloMergedClusterInvMassPtBin[iPt] != 0x00){
                        fMesonTrueMassCaloMergedCluster[iPt]            = fFitTrueSignalCaloMergedClusterInvMassPtBin[iPt]->GetParameter(1);
                        fMesonTrueMassErrorCaloMergedCluster[iPt]       = fFitTrueSignalCaloMergedClusterInvMassPtBin[iPt]->GetParError(1);
                        CalculateFWHM(fFitTrueSignalCaloMergedClusterInvMassPtBin[iPt]);
                        fMesonTrueFWHMCaloMergedCluster[iPt]            = fFWHMFunc;
                        fMesonTrueFWHMErrorCaloMergedCluster[iPt]       = fFWHMFuncError;
                    } else {
                        fMesonTrueMassCaloMergedCluster[iPt]            = 0.;
                        fMesonTrueMassErrorCaloMergedCluster[iPt]       = 1.;
                        fMesonTrueFWHMCaloMergedCluster[iPt]            = 0.;
                        fMesonTrueFWHMErrorCaloMergedCluster[iPt]       = 0.;
                    }
                }
                if (fHistoMappingTrueMesonCaloMergedClusterPartConvInvMassPtBins[iPt]->GetEntries() !=0){
                    if(fCrysFitting==0){
                        fFileErrLog << "Using exp fit"<<endl;
                        FitTrueInvMassInPtBins(fHistoMappingTrueMesonCaloMergedClusterPartConvInvMassPtBins[iPt], fMesonIntDeltaRange,iPt,kFALSE);
                    } else {
                        fFileErrLog << "Using Crystal Ball function"<<endl;
                        FitCBSubtractedInvMassInPtBins(fHistoMappingTrueMesonCaloMergedClusterPartConvInvMassPtBins[iPt],
                                                    fMesonIntDeltaRange,iPt,kFALSE,Form("CBFitFuncMCTrueBinCaloMergedClusterPartConv%02d",iPt),kTRUE);
                    }

                    fFitTrueSignalCaloMergedClusterPartConvInvMassPtBin[iPt]    = fFitReco;
                    if (fFitTrueSignalCaloMergedClusterPartConvInvMassPtBin[iPt] != 0x00){
                        fMesonTrueMassCaloMergedClusterPartConv[iPt]            = fFitTrueSignalCaloMergedClusterPartConvInvMassPtBin[iPt]->GetParameter(1);
                        fMesonTrueMassErrorCaloMergedClusterPartConv[iPt]       = fFitTrueSignalCaloMergedClusterPartConvInvMassPtBin[iPt]->GetParError(1);
                        CalculateFWHM(fFitTrueSignalCaloMergedClusterPartConvInvMassPtBin[iPt]);
                        fMesonTrueFWHMCaloMergedClusterPartConv[iPt]            = fFWHMFunc;
                        fMesonTrueFWHMErrorCaloMergedClusterPartConv[iPt]       = fFWHMFuncError;
                    } else {
                        fMesonTrueMassCaloMergedClusterPartConv[iPt]            = 0.;
                        fMesonTrueMassErrorCaloMergedClusterPartConv[iPt]       = 1.;
                        fMesonTrueFWHMCaloMergedClusterPartConv[iPt]            = 0.;
                        fMesonTrueFWHMErrorCaloMergedClusterPartConv[iPt]       = 0.;
                    }
                }
            }
            if (fAdvancedMesonQA && (fMode == 4 || fMode == 12 || fMode == 5 || fMode==14)){
                if (fHistoMappingTrueMesonMixedCaloConvPhotonInvMassPtBins[iPt]->GetEntries() !=0){
                    if(fCrysFitting==0){
                        fFileErrLog << "Using exp fit"<<endl;
                        FitTrueInvMassInPtBins(fHistoMappingTrueMesonMixedCaloConvPhotonInvMassPtBins[iPt], fMesonIntDeltaRange,iPt,kFALSE);
                    } else {
                        fFileErrLog << "Using Crystal Ball function"<<endl;
                        FitCBSubtractedInvMassInPtBins(fHistoMappingTrueMesonMixedCaloConvPhotonInvMassPtBins[iPt],
                                                    fMesonIntDeltaRange,iPt,kFALSE,Form("CBFitFuncMCTrueBinCaloMixedCaloConvPhoton%02d",iPt),kTRUE);
                    }

                    IntegrateHistoInvMassStream( fHistoMappingTrueMesonMixedCaloConvPhotonInvMassPtBins[iPt], fIntFixedRange);
                    fMesonTrueYieldGammaConvGammaFixedWindow[iPt]           = fYields;
                    fMesonTrueYieldGammaConvGammaErrorFixedWindow[iPt]      = fYieldsError;

                    fFitTrueSignalMixedCaloConvPhotonInvMassPtBin[iPt]      = fFitReco;
                    if (fFitTrueSignalMixedCaloConvPhotonInvMassPtBin[iPt] != 0x00){
                        fMesonTrueMassMixedCaloConvPhoton[iPt]              = fFitTrueSignalMixedCaloConvPhotonInvMassPtBin[iPt]->GetParameter(1);
                        fMesonTrueMassErrorMixedCaloConvPhoton[iPt]         = fFitTrueSignalMixedCaloConvPhotonInvMassPtBin[iPt]->GetParError(1);
                        CalculateFWHM(fFitTrueSignalMixedCaloConvPhotonInvMassPtBin[iPt]);
                        fMesonTrueFWHMMixedCaloConvPhoton[iPt]              = fFWHMFunc;
                        fMesonTrueFWHMErrorMixedCaloConvPhoton[iPt]         = fFWHMFuncError;
                    } else {
                        fMesonTrueMassMixedCaloConvPhoton[iPt]              = 0.;
                        fMesonTrueMassErrorMixedCaloConvPhoton[iPt]         = 1.;
                        fMesonTrueFWHMMixedCaloConvPhoton[iPt]              = 0.;
                        fMesonTrueFWHMErrorMixedCaloConvPhoton[iPt]         = 0.;
                    }
                }
            }

            // fill pt array with integrated validated MC yield for different integration windows: normal, wide, narrow
            for (Int_t k = 0; k< 3;k++){
                if (k > 0 )
                    fFileDataLog<< endl <<"True histo " << nameIntRange[k].Data() << " range" << fBinsPt[iPt] <<"-" << fBinsPt[iPt+1]<< endl;
                IntegrateHistoInvMassStream( fHistoMappingTrueMesonInvMassPtBins[iPt], fMesonTrueIntRange[k]);
                fMesonTrueYields[k][iPt]                        = fYields;
                fMesonTrueYieldsError[k][iPt]                   = fYieldsError;
                fFileDataLog << "Integrated value: \t" << fYields <<"+-" <<fYieldsError<<endl;

                fFileDataLog<< endl <<"True histo " << nameIntRange[k].Data() << " range reweighted" << fBinsPt[iPt] <<"-" << fBinsPt[iPt+1]<< endl;
                IntegrateHistoInvMassStream( fHistoMappingTrueMesonInvMassPtReweightedBins[iPt], fMesonTrueIntReweightedRange[k]);
                fMesonTrueYieldsReweighted[k][iPt]              = fYields;
                fMesonTrueYieldsReweightedError[k][iPt]         = fYieldsError;
                fFileDataLog << "Integrated value: \t" << fYields <<"+-" <<fYieldsError<<endl;

                fFileDataLog<< endl <<"True histo " << nameIntRange[k].Data() << " range unweighted" << fBinsPt[iPt] <<"-" << fBinsPt[iPt+1]<< endl;
                IntegrateHistoInvMassStream( fHistoMappingTrueMesonInvMassPtUnweightedBins[iPt], fMesonTrueIntUnweightedRange[k]);
                fMesonTrueYieldsUnweighted[k][iPt]              = fYields;
                fMesonTrueYieldsUnweightedError[k][iPt]         = fYieldsError;
                fFileDataLog << "Integrated value: \t" << fYields <<"+-" <<fYieldsError<<endl;


            }

            // fill double counting pt-array  if available
            if (fEnableDCMeson){
                IntegrateHistoInvMassStream( fHistoMappingTrueMesonDCInvMassPtBins[iPt], fMesonTrueIntRange[0]);
                fMesonTrueYieldsDC[iPt]                         = fYields;
                fMesonTrueYieldsDCError[iPt]                    = fYieldsError;
                fFileDataLog << "Integrated value: \t" << fYields <<"+-" <<fYieldsError<<endl;
                cout << "***********************************************" << endl;
                cout << "***********************************************" << endl;
                cout << "***********************************************" << endl;
                cout << "***********************************************" << endl;
            }
            // fill secondary yield pt-arrays for neutral pion for different sources and integration windows
            if (meson.Contains("Pi0")){
                for (Int_t j = 0; j < 4; j++){
                    if (fHistoMappingTrueSecMesonInvMassPtBins[j][iPt]){
                        for (Int_t k = 0; k < 3; k++){
                            fFileDataLog<< endl <<"TrueSec " << nameSecondaries[j].Data() << " histo " << nameIntRange[k].Data() << ":\t" << fBinsPt[iPt] <<"-" << fBinsPt[iPt+1]<< endl;
                            IntegrateHistoInvMassStream( fHistoMappingTrueSecMesonInvMassPtBins[j][iPt], fMesonTrueIntRange[k]);
                            fMesonTrueSecYields[k][j][iPt]              = fYields;
                            fMesonTrueSecYieldsError[k][j][iPt]         = fYieldsError;
                            fFileDataLog << "Integrated value: \t" << fYields <<"+-" <<fYieldsError<<endl;
                        }
                    } else {
                        for (Int_t k= 0; k< 3; k++){
                            fMesonTrueSecYields[k][j][iPt]          = 0;
                            fMesonTrueSecYieldsError[k][j][iPt]     = 0;
                        }
                    }
                }
            } else {
                for (Int_t j = 0; j < 4; j++){
                    for (Int_t k= 0; k< 3; k++){
                        fMesonTrueSecYields[k][j][iPt]              = 0;
                        fMesonTrueSecYieldsError[k][j][iPt]         = 0;
                    }
                }
            }


            if( (fGGYields[0][iPt] - fMesonTrueYields[0][iPt]) > 0) {
                fMesonTrueSB[iPt]               = fMesonTrueYields[0][iPt] / ( fGGYields[0][iPt] - fMesonTrueYields[0][iPt] );
                fMesonTrueSign[iPt]             = fMesonTrueYields[0][iPt] / TMath::Power( ( fGGYields[0][iPt] - fMesonTrueYields[0][iPt] ) , 0.5);
                fMesonTrueSBError[iPt]          = 0;
                fMesonTrueSignError[iPt]        = 0;
            }
            else {
                fMesonTrueSB[iPt]               = 0.;
                fMesonTrueSign[iPt]             = 0.;
                fMesonTrueSBError[iPt]          = 0.;
                fMesonTrueSignError[iPt]        = 0.;
            }
        }


        for (Int_t k = 0; k < 3; k++){
            if (k > 0){ // Wide and narrow integration windows: fit and background integration like for normal window done above
                fFileErrLog << fBinsPt[iPt] <<"-" << fBinsPt[iPt+1] << "\t" << nameIntRange[k].Data()<<  endl;
                Double_t intRange[2]    = {0,0};
                if (k == 1){
                    cout << "Wide integration range" << endl;
                    intRange[0]         = fMesonIntDeltaRangeWide[0];
                    intRange[1]         = fMesonIntDeltaRangeWide[1];
                } else if ( k == 2){
                    cout << "Narrow integration range" << endl;
                    intRange[0]         = fMesonIntDeltaRangeNarrow[0];
                    intRange[1]         = fMesonIntDeltaRangeNarrow[1];
                }
                if(fCrysFitting==0){
                    fFileErrLog << "Using exp fit"<<endl;
                      FitSubtractedInvMassInPtBinsWOLinear(fHistoMappingSignalInvMassPtBin[iPt], intRange,iPt,kFALSE);
                      fMesonYieldsResidualBckFuncStd[k][iPt]         = 0;
                      fMesonYieldsResidualBckFuncErrorStd[k][iPt]    = 0;
                } else {
                    fMesonYieldsResidualBckFunc[k][iPt]         = 0;
                    fMesonYieldsResidualBckFuncError[k][iPt]    = 0;
                }
            }

            fFileDataLog << "Residual Background leftover " << nameIntRange[k].Data() << " in iPt " << fBinsPt[iPt] <<"-" << fBinsPt[iPt+1] << ":\t"
                         << fMesonYieldsResidualBckFuncStd[k][iPt] << "\t +- \t" << fMesonYieldsResidualBckFuncErrorStd[k][iPt] << endl<< endl;
            /////////////////////// added to check yields //////////////////////////////////////////////////////////
            fTotalBckYields[k][iPt]                         = fBckYields[k][iPt] + fMesonYieldsResidualBckFuncStd[k][iPt];
            fTotalBckYieldsError[k][iPt]                    = TMath::Power(fBckYieldsError[k][iPt]*fBckYieldsError[k][iPt] + fMesonYieldsResidualBckFuncErrorStd[k][iPt]*fMesonYieldsResidualBckFuncErrorStd[k][iPt],0.5);

            fFileDataLog << "Total Background " << nameIntRange[k].Data() << " in iPt " << fBinsPt[iPt] <<"-" << fBinsPt[iPt+1] << ":\t"
                         << fTotalBckYields[k][iPt] << "\t +- \t" << fTotalBckYieldsError[k][iPt] << endl<< endl;
            fFileDataLog << "Background " << nameIntRange[k].Data() << " in iPt " << fBinsPt[iPt] <<"-" << fBinsPt[iPt+1] << ":\t"
                         << fBckYields[k][iPt] << "\t +- \t" << fBckYieldsError[k][iPt] << endl<< endl;
            ///////////////////////////////////////////////////////////////////////////////////////////////////////

            fMesonYieldsCorResidualBckFunc[k][iPt]          = fMesonYields[k][iPt] - fMesonYieldsResidualBckFuncStd[k][iPt];
            fMesonYieldsCorResidualBckFuncError[k][iPt]     = TMath::Power(( fMesonYieldsError[k][iPt]*fMesonYieldsError[k][iPt]
                                                                    + fMesonYieldsResidualBckFuncErrorStd[k][iPt]*fMesonYieldsResidualBckFuncErrorStd[k][iPt]),0.5);
            fMesonYieldsPerEvent[k][iPt]                    = fMesonYieldsCorResidualBckFunc[k][iPt]/fNEvents;
            fMesonYieldsPerEventError[k][iPt]               = fMesonYieldsCorResidualBckFuncError[k][iPt]/fNEvents;
            if(fDoJetAnalysis) fMesonYieldsPerJetEvent[k][iPt]       = fMesonYieldsCorResidualBckFunc[k][iPt]/fNJetEvents;
            if(fDoJetAnalysis) fMesonYieldsPerJetEventError[k][iPt]  = fMesonYieldsCorResidualBckFuncError[k][iPt]/fNJetEvents;

            //Integrate Fit Function
            IntegrateFitFunc( fFitSignalPeakPosInvMassPtBinStd[iPt], fHistoMappingSignalInvMassPtBin[iPt], fMesonCurIntRange[k]);
            fMesonYieldsFunc[k][iPt]                        = fYieldsFunc;
            //SB default
            if (fTotalBckYields[k][iPt] != 0){
                fMesonSBdefault[k][iPt]                     = fMesonYieldsCorResidualBckFunc[k][iPt]/fTotalBckYields[k][iPt];
                fMesonSBdefaultError[k][iPt]                = TMath::Power( TMath::Power(fMesonYieldsCorResidualBckFuncError[k][iPt]/fTotalBckYields[k][iPt], 2.) +
                                                            TMath::Power((fTotalBckYieldsError[k][iPt]*fMesonYieldsCorResidualBckFunc[k][iPt])/(fTotalBckYields[k][iPt] *fTotalBckYields[k][iPt]), 2.), 0.5);
            } else {
                fMesonSBdefault[k][iPt]         = 0.;
                fMesonSBdefaultError[k][iPt]    = 0.;
            }
            //Significance default
            if ( TMath::Power(fMesonYieldsCorResidualBckFunc[k][iPt] + fTotalBckYields[k][iPt],0.5) != 0){
                fMesonSigndefault[k][iPt]                   = fMesonYieldsCorResidualBckFunc[k][iPt]/TMath::Power(fMesonYieldsCorResidualBckFunc[k][iPt] + fTotalBckYields[k][iPt],0.5);
                Double_t a                                  = ( TMath::Power(fMesonYieldsCorResidualBckFunc[k][iPt] + fTotalBckYields[k][iPt], -0.5) -
                                                                0.5*fMesonYieldsCorResidualBckFunc[k][iPt]*TMath::Power(fMesonYieldsCorResidualBckFunc[k][iPt] +
                                                                fTotalBckYields[k][iPt], -1.5) * fMesonYieldsCorResidualBckFuncError[k][iPt]);
                Double_t b                                  = 0.5*fMesonYieldsCorResidualBckFunc[k][iPt]*TMath::Power(fMesonYieldsCorResidualBckFunc[k][iPt]
                                                              + fTotalBckYields[k][iPt],-1.5) * fTotalBckYieldsError[k][iPt];
                fMesonSigndefaultError[k][iPt]              = TMath::Power( a*a + b*b, 0.5);
            } else {
                fMesonSigndefault[k][iPt]       = 0.;
                fMesonSigndefaultError[k][iPt]  = 0.;
            }
        }


      if(debugOutputLevel) cerr<<"Debug in ExtractSignalTemplate; LINE: "<<__LINE__<<endl;
      // Integrate(iPt, 3);

      //*********************************************************************************
      // Left Side
      //*********************************************************************************
      cout << "Left normalization" << endl;

      MakeFit(fHistoMappingGGInvMassPtBin[iPt], fHistoMappingBackInvMassPtBin[iPt], fHistoMappingTruePi0InvMassPtBins[iPt], fHistoMappingTrueEtaInvMassPtBins[iPt], true, iPt);


      if(optionTemplateFit == 0){
        if(NFitPars == 3){
          scaleBack = fTempFit->GetParameter(0);
          scalePi0 = fTempFit->GetParameter(1);
          scaleEta = fTempFit->GetParameter(2);
          if(fScaleFacEtaToPi0 > 0 && fPrefix.CompareTo("Eta") == 0){
            scaleBack = fTempFit->GetParameter(0);
            scalePi0 = fTempFit->GetParameter(1);
            scaleEta = fTempFit->GetParameter(1) * fScaleFacEtaToPi0;
          }
        } else if(NFitPars == 2 && fPrefix.CompareTo("Eta") == 0){
          scaleBack = fTempFit->GetParameter(0);
          scalePi0 = 0;
          scaleEta = fTempFit->GetParameter(1);
        } else if(NFitPars == 2 && fPrefix.Contains("Pi0")){
          scaleBack = fTempFit->GetParameter(0);
          scalePi0 = fTempFit->GetParameter(1);
          scaleEta = 0;
        }
      } else if(optionTemplateFit == 1){
        if(NFitPars == 4){
          funcScaleBack = new TF1("funcScaleBack", "[0]*x+[1]", 0, 1);
          funcScaleBack->SetParameters(fTempFit->GetParameter(2), fTempFit->GetParameter(3));
          cout<<"fTempFit->GetParameter(0) "<<fTempFit->GetParameter(0)<<endl;
          scalePi0 = fTempFit->GetParameter(0);
          scaleEta = fTempFit->GetParameter(0) * fScaleFacEtaToPi0;
        }
      } else if(optionTemplateFit == 2){
        if(NFitPars == 4){
          funcScaleBack = new TF1("funcScaleBack", "[0]*x+[1]", 0, 1);
          funcScaleBack->SetParameters(fTempFit->GetParameter(3), fTempFit->GetParameter(4));
          cout<<"fTempFit->GetParameter(0) "<<fTempFit->GetParameter(0)<<endl;
          scalePi0 = fTempFit->GetParameter(0);
          scaleEta = fTempFit->GetParameter(0) * fScaleFacEtaToPi0;
        }
      }
      if(debugOutputLevel) cerr<<"Debug in ExtractSignalTemplate; LINE: "<<__LINE__<<endl;

      if(NFitPars == 4 && optionTemplateFit == 1 && funcScaleBack) fHistoMappingBackNormInvMassLeftPtBin[iPt]->Multiply(funcScaleBack);
      else fHistoMappingBackNormInvMassLeftPtBin[iPt]->Scale(scaleBack);
      if(NFitPars == 5 && optionTemplateFit == 2 && funcScaleBack) fHistoMappingBackNormInvMassLeftPtBin[iPt]->Add(funcScaleBack);
      fHistoMappingTruePi0ScaledInvMassLeftPtBins[iPt]->Scale(scalePi0);
      fHistoMappingTrueEtaScaledInvMassLeftPtBins[iPt]->Scale(scaleEta);

      fHistoScaleFacLeftBack->SetBinContent(iPt + 1,scaleBack);
      fHistoScaleFacLeftBack->SetBinError(iPt + 1,fTempFit->GetParError(0));
      fHistoScaleFacLeftPi0->SetBinContent(iPt + 1,scalePi0);
      fHistoScaleFacLeftPi0->SetBinError(iPt + 1,fTempFit->GetParError(1));
      fHistoScaleFacLeftEta->SetBinContent(iPt + 1,scaleEta);
      fHistoScaleFacLeftEta->SetBinError(iPt + 1,fTempFit->GetParError(2));


      // SubtractBackground(iPt, 1);
      fHistoMappingSignalInvMassLeftPtBin[iPt] = SubtractBackground(fHistoMappingGGInvMassPtBin[iPt], fHistoMappingBackNormInvMassLeftPtBin[iPt], fHistoMappingTruePi0ScaledInvMassLeftPtBins[iPt], fHistoMappingTrueEtaScaledInvMassLeftPtBins[iPt]);
      FitSubtractedInvMassInPtBinsWOLinear(fHistoMappingSignalInvMassLeftPtBin[iPt], fMesonIntDeltaRange, iPt, false);
      fFitSignalInvMassPtBin[iPt]                 = fFitReco;
      fFitSignalPeakPosInvMassPtBin[iPt]          = fFitGausExp;
      fFitBckInvMassPtBin[iPt]                    = fFitLinearBck;
      fMesonYieldsResidualBckFunc[0][iPt]         = fIntLinearBck;
      fMesonYieldsResidualBckFuncError[0][iPt]    = fIntLinearBckError;
      if (fFitReco)
          fMesonChi2[0][iPt]                      = fFitReco->GetChisquare()/fFitReco->GetNDF();
      else
          fMesonChi2[0][iPt]                      = -1  ;
      fMesonMassLeft[iPt] = fFitReco->GetParameter(1);
      fMesonMassLeftError[iPt] = fFitReco->GetParError(1);

      fHistoMappingSignalInvMassLeftPtBin[iPt]->SetName(Form("fHistoMappingSignalInvMassLeft_in_Pt_Bin%02d",iPt));


      // Function to subtract GG minus Bck
      // cout << fBinsPt[iPt] <<"-" << fBinsPt[iPt+1] << endl;
      // if (debugOutputLevel>=1){cout << "Debug; ExtractSignalV2.C, line " << __LINE__ << endl;}
      // ProcessEM_switch( fHistoMappingGGInvMassPtBin[iPt], fHistoMappingBackInvMassPtBin[iPt], fBGFitRangeLeft);
      // if (debugOutputLevel>=1){cout << "Debug; ExtractSignalV2.C, line " << __LINE__ << endl;}
      // fHistoMappingSignalInvMassLeftPtBin[iPt]        = fSignal;
      // fHistoMappingBackNormInvMassLeftPtBin[iPt]      = fBckNorm;

      if (debugOutputLevel>=1){       cout<< "iPt"<< iPt<< " "<< "standard range"<<endl;}
      // Fitting the subtracted spectra
      fFileErrLog << fBinsPt[iPt] <<"-" << fBinsPt[iPt+1] << "\t" << "normal range/left normalization" << endl;

      if (debugOutputLevel>=1){cout<< fBinsPt[iPt] <<"-" << fBinsPt[iPt+1] << "\t" << "normal range/left normalization" << endl;}
      fFitInvMassLeftPtBin[iPt] =0x00;

      if (debugOutputLevel>=1){cout << "Debug; ExtractSignalV2.C, line " << __LINE__ <<"; Using exp fit "<<endl;}
      FitSubtractedInvMassInPtBins(fHistoMappingSignalInvMassLeftPtBin[iPt], fMesonIntDeltaRange,iPt,kFALSE);

      fFitInvMassLeftPtBin[iPt]                               = fFitReco;
      fFitSignalPeakPosInvMassLeftPtBin[iPt]                  = fFitGausExp;
      fFitBckInvMassLeftPtBin[iPt]                            = fFitLinearBck;
      fMesonYieldsResidualBckFuncStd[3][iPt]                  = fIntLinearBck;
      fMesonYieldsResidualBckFuncErrorStd[3][iPt]             = fIntLinearBckError;

      if (debugOutputLevel>=1){cout << "Debug; ExtractSignalV2.C, line " << __LINE__ << endl;}
      CalculateFWHM(fFitInvMassLeftPtBin[iPt]);
      if (debugOutputLevel>=1){cout << "Debug; ExtractSignalV2.C, line " << __LINE__ << endl;}
      fMesonFWHMLeft[iPt]         = fFWHMFunc;
      fMesonFWHMLeftError[iPt]    = fFWHMFuncError;
      if (debugOutputLevel>=1){cout << "Debug; ExtractSignalV2.C, line " << __LINE__ << endl;}

      if (fFitInvMassLeftPtBin[iPt] !=0x00){
          if (debugOutputLevel>=1){cout << "Debug; ExtractSignalV2.C, line " << __LINE__ << endl;}
          fMesonMassLeft[iPt]             = fFitInvMassLeftPtBin[iPt]->GetParameter(1);
          fMesonMassLeftError[iPt]        = fFitInvMassLeftPtBin[iPt]->GetParError(1);
          fMesonCurIntRange[3][0]         = fMesonMassLeft[iPt] + fMesonIntDeltaRange[0];
          fMesonCurIntRange[4][0]         = fMesonMassLeft[iPt] + fMesonIntDeltaRangeWide[0];
          fMesonCurIntRange[5][0]         = fMesonMassLeft[iPt] + fMesonIntDeltaRangeNarrow[0];
          fMesonCurIntRange[3][1]         = fMesonMassLeft[iPt] + fMesonIntDeltaRange[1];
          fMesonCurIntRange[4][1]         = fMesonMassLeft[iPt] + fMesonIntDeltaRangeWide[1];
          fMesonCurIntRange[5][1]         = fMesonMassLeft[iPt] + fMesonIntDeltaRangeNarrow[1];
      } else {
          if (debugOutputLevel>=1){cout << "Debug; ExtractSignalV2.C, line " << __LINE__ << endl;}
          fMesonMassLeft[iPt]             = 0.;
          fMesonMassLeftError[iPt]        = 0.;
          fMesonCurIntRange[3][0]         = fMesonMassExpect + fMesonIntDeltaRange[0];
          fMesonCurIntRange[4][0]         = fMesonMassExpect + fMesonIntDeltaRangeWide[0];
          fMesonCurIntRange[5][0]         = fMesonMassExpect + fMesonIntDeltaRangeNarrow[0];
          fMesonCurIntRange[3][1]         = fMesonMassExpect + fMesonIntDeltaRange[1];
          fMesonCurIntRange[4][1]         = fMesonMassExpect + fMesonIntDeltaRangeWide[1];
          fMesonCurIntRange[5][1]         = fMesonMassExpect + fMesonIntDeltaRangeNarrow[1];
      }

      // Integrate the bck histo
      if (debugOutputLevel>=1){cout << "Debug; ExtractSignalV2.C, line " << __LINE__ << endl;}
      if (fCrysFitting == 0){
          for (Int_t k = 0; k < 3; k++){
              if (debugOutputLevel>=3){cout << "Debug; ExtractSignalV2.C, line " << __LINE__ << endl;}
              IntegrateHistoInvMass( fHistoMappingBackNormInvMassLeftPtBin[iPt], fMesonCurIntRange[k+3]);
              fBckYields[k+3][iPt]              = fYields;
              fBckYieldsError[k+3][iPt]         = fYieldsError;

              fFileDataLog<< endl <<"Signal histo " << nameIntRange[k+3].Data() << ":\t" << fBinsPt[iPt] <<"-" << fBinsPt[iPt+1]<< endl;
              IntegrateHistoInvMass( fHistoMappingSignalInvMassLeftPtBin[iPt], fMesonCurIntRange[k+3]);
              fMesonYields[k+3][iPt]            = fYields;
              fMesonYieldsError[k+3][iPt]       = fYieldsError;
              fFileDataLog << "Integrated value: \t" << fYields <<"+-" <<fYieldsError <<endl;

          }
      } else {
          for (Int_t k = 0; k < 3; k++){
              if (debugOutputLevel>=3){cout << "Debug; ExtractSignalV2.C, line " << __LINE__ << endl;}
              if (k == 0){
                  fHistoMappingBackNormInvMassLeftPtBin[iPt]->Sumw2();
                  fHistoMappingBackNormInvMassLeftPtBin[iPt]->Add(fHistoMappingRemainingBGInvMassLeftPtBin[iPt],1);
              }
              IntegrateHistoInvMass( fHistoMappingBackNormInvMassLeftPtBin[iPt], fMesonCurIntRange[k+3]);
              fBckYields[k+3][iPt]              = fYields;
              fBckYieldsError[k+3][iPt]         = fYieldsError;

              fFileDataLog<< endl <<"Signal histo " << nameIntRange[k+3].Data() << ":\t" << fBinsPt[iPt] <<"-" << fBinsPt[iPt+1]<< endl;
              IntegrateHistoInvMassStream( fHistoMappingSignalInvMassLeftPtBin[iPt], fMesonCurIntRange[k+3]);
              fMesonYields[k+3][iPt]            = fYields;
              fMesonYieldsError[k+3][iPt]       = fYieldsError;
              fFileDataLog << "Integrated value: \t" << fYields <<"+-" <<fYieldsError <<endl;

          }
      }
      for (Int_t k = 0; k < 3; k++){
          if (debugOutputLevel>=3){cout << "Debug; ExtractSignalV2.C, line " << __LINE__ << endl;}
          if (k > 0){
              fFileErrLog << fBinsPt[iPt] <<"-" << fBinsPt[iPt+1] << "\t" << nameIntRange[k+3].Data()<<  endl;
              Double_t intRange[2]    = {0,0};
              if (k == 1){
                  cout << "Wide integration range" << endl;
                  intRange[0]         = fMesonIntDeltaRangeWide[0];
                  intRange[1]         = fMesonIntDeltaRangeWide[1];
              } else if ( k == 2){
                  cout << "Narrow integration range" << endl;
                  intRange[0]         = fMesonIntDeltaRangeNarrow[0];
                  intRange[1]         = fMesonIntDeltaRangeNarrow[1];
              }

              FitSubtractedInvMassInPtBinsWOLinear(fHistoMappingSignalInvMassLeftPtBin[iPt], intRange, iPt, kFALSE);

              // fMesonYieldsResidualBckFuncStd[k+3][iPt]         = fIntLinearBck;
              // fMesonYieldsResidualBckFuncErrorStd[k+3][iPt]    = fIntLinearBckError;
              // if (optionTemplateFit == -2){
                fMesonYieldsResidualBckFuncStd[k+3][iPt]         = 0;
                fMesonYieldsResidualBckFuncErrorStd[k+3][iPt]    = 0;
              // }

          }
          if (debugOutputLevel>=1){cout << "Debug; ExtractSignalV2.C, line " << __LINE__ << endl;}

          fFileDataLog << "Residual Background leftover " << nameIntRange[k+3].Data() << " in iPt " << fBinsPt[iPt] <<"-" << fBinsPt[iPt+1] << ":\t"
                       << fMesonYieldsResidualBckFuncStd[k+3][iPt] << "\t +- \t" << fMesonYieldsResidualBckFuncErrorStd[k+3][iPt] << endl<< endl;

          fTotalBckYields[k+3][iPt]                       = fBckYields[k+3][iPt] + fMesonYieldsResidualBckFuncStd[k+3][iPt];
          fTotalBckYieldsError[k+3][iPt]                  = TMath::Power(fBckYieldsError[k+3][iPt]*fBckYieldsError[k+3][iPt] + fMesonYieldsResidualBckFuncErrorStd[k+3][iPt]*fMesonYieldsResidualBckFuncErrorStd[k+3][iPt],0.5);
          fFileDataLog << "Total Background " << nameIntRange[k+3].Data() << " in iPt " << fBinsPt[iPt] <<"-" << fBinsPt[iPt+1] << ":\t"
                       << fTotalBckYields[k+3][iPt] << "\t +- \t" << fTotalBckYieldsError[k+3][iPt] << endl<< endl;
          fFileDataLog << "Background " << nameIntRange[k+3].Data() << " in iPt " << fBinsPt[iPt] <<"-" << fBinsPt[iPt+1] << ":\t"
                       << fBckYields[k+3][iPt] << "\t +- \t" << fBckYieldsError[k+3][iPt] << endl<< endl;


          fMesonYieldsCorResidualBckFunc[k+3][iPt]        = fMesonYields[k+3][iPt]; //- fMesonYieldsResidualBckFuncStd[k+3][iPt];
          fMesonYieldsCorResidualBckFuncError[k+3][iPt]   = TMath::Power(( fMesonYieldsError[k+3][iPt]*fMesonYieldsError[k+3][iPt]
                                                                           + fMesonYieldsResidualBckFuncErrorStd[k+3][iPt]*fMesonYieldsResidualBckFuncErrorStd[k+3][iPt]),0.5);
          fMesonYieldsPerEvent[k+3][iPt]                  = fMesonYieldsCorResidualBckFunc[k+3][iPt]/fNEvents;
          fMesonYieldsPerEventError[k+3][iPt]             = fMesonYieldsCorResidualBckFuncError[k+3][iPt]/fNEvents;
          if(fDoJetAnalysis) fMesonYieldsPerJetEvent[k+3][iPt]       = fMesonYieldsCorResidualBckFunc[k+3][iPt]/fNJetEvents;
          if(fDoJetAnalysis) fMesonYieldsPerJetEventError[k+3][iPt]  = fMesonYieldsCorResidualBckFuncError[k+3][iPt]/fNJetEvents;

          //Integrate Fit Function
          if (debugOutputLevel>=1){cout << "Debug; ExtractSignalV2.C, line " << __LINE__ << endl;}
          IntegrateFitFunc( fFitSignalPeakPosInvMassLeftPtBin[iPt], fHistoMappingSignalInvMassLeftPtBin[iPt], fMesonCurIntRange[k+3]);
          fMesonYieldsFunc[k+3][iPt]                      = fYieldsFunc;
      }


      if(debugOutputLevel) cerr<<"Debug in ExtractSignalTemplate; LINE: "<<__LINE__<<endl;
      // fill secondary yield pt-arrays for neutral pion for different sources and integration windows
      if (meson.Contains("Pi0")){
          for (Int_t j = 0; j < 4; j++){
            if(debugOutputLevel) cerr<<"Debug in ExtractSignalTemplate; LINE: "<<__LINE__<<endl;
              if (fHistoMappingTrueSecMesonInvMassPtBins[j][iPt]){
                if(debugOutputLevel) cerr<<"Debug in ExtractSignalTemplate; LINE: "<<__LINE__<<endl;
                  for (Int_t k = 0; k < 3; k++){
                      fFileDataLog<< endl <<"TrueSec " << nameSecondaries[j].Data() << " histo " << nameIntRange[k].Data() << ":\t" << fBinsPt[iPt] <<"-" << fBinsPt[iPt+1]<< endl;
                      IntegrateHistoInvMass( fHistoMappingTrueSecMesonInvMassPtBins[j][iPt], fMesonTrueIntRange[k]);
                      fMesonTrueSecYields[k][j][iPt]              = fYields;
                      fMesonTrueSecYieldsError[k][j][iPt]         = fYieldsError;
                      fFileDataLog << "Integrated value: \t" << fYields <<"+-" <<fYieldsError<<endl;
                  }
              } else {
                  for (Int_t k= 0; k< 3; k++){
                      fMesonTrueSecYields[k][j][iPt]          = 0;
                      fMesonTrueSecYieldsError[k][j][iPt]     = 0;
                  }
              }
          }
      } else {
          for (Int_t j = 0; j < 4; j++){
              for (Int_t k= 0; k< 3; k++){
                  fMesonTrueSecYields[k][j][iPt]              = 0;
                  fMesonTrueSecYieldsError[k][j][iPt]         = 0;
              }
          }
      }
    // end MC case

      ////////////////////////////////
      // END ANALYSIS FOR EACH PT BIN
      ///////////////////////////////
    }
    cout<<"Fill Pt Histos\n";
    FillPtHistos();

    // **************************************************************************************************************
    // Filling MC hists
    // **************************************************************************************************************
    if(fIsMC){
        // Rebin MC histograms for acceptance and input with possible weights
        FillHistosArrayMC(fHistoMCMesonPtWithinAcceptance, fHistoMCMesonPt, fDeltaPt);
        // Rebin MC histograms for acceptance and input without weights
        if (fHistoMCMesonPtWithinAcceptanceWOWeights) FillHistosArrayMCWOWeights(fHistoMCMesonPtWithinAcceptanceWOWeights, fHistoMCMesonPtWOWeights, fDeltaPt);
        if (fHistoMCMesonPtWithinAcceptanceWOEvtWeights) FillHistosArrayMCWOEvtWeights(fHistoMCMesonPtWithinAcceptanceWOEvtWeights, fHistoMCMesonPtWOEvtWeights, fDeltaPt);

        // Calculation of meson acceptance with possible weighted input
        CalculateMesonAcceptance();
        if(useExtAccept){
            if(LoadMesonAcceptance(fDeltaPt)){
                cout << "loaded external acceptance sucessfully" << endl;
            } else {
                cout << "failed to load external acceptance" << endl; return;
            }
        }
        // Calculation of meson acceptance without weights for input
        if (fHistoMCMesonPtWithinAcceptanceWOWeights) CalculateMesonAcceptanceWOWeights();
        if (fHistoMCMesonPtWithinAcceptanceWOEvtWeights) CalculateMesonAcceptanceWOEvtWeights();

        if (fNewMCOutput){
            FillMCSecondaryHistAndCalculateAcceptance(fHistoMCSecPi0SourcePt, fHistoMCSecPi0WAccSourcePt);
        }

        // calculate pure rec efficiency as in data with fully unweighted histograms if possible
        // ATTENTION: if unweighted histograms are not available this efficiency should not be used for anything!!!!

        for (Int_t k = 0; k < 3; k++){
            fNameHistoEffi                      = Form("Meson%sEffiPt",nameIntRange[k].Data());
            cout << fNameHistoEffi.Data() << endl;
            if (fHistoMCMesonPtWithinAcceptanceWOWeights)
                fHistoMonteMesonEffiPt[k]       = CalculateMesonEfficiency(fHistoYieldMeson[k], fHistoYieldTrueSecMeson[k], fHistoMCMesonWithinAccepPtWOWeights, fNameHistoEffi);
            else
                fHistoMonteMesonEffiPt[k]       = CalculateMesonEfficiency(fHistoYieldMeson[k], fHistoYieldTrueSecMeson[k], fHistoMCMesonWithinAccepPt, fNameHistoEffi);

            fNameHistoEffi                      = Form("Meson%sEffiPt",nameIntRange[k+3].Data());
            cout << fNameHistoEffi.Data() << endl;
            if (fHistoMCMesonPtWithinAcceptanceWOWeights)
                fHistoMonteMesonEffiPt[k+3]       = CalculateMesonEfficiency(fHistoYieldMeson[k+3], fHistoYieldTrueSecMeson[k], fHistoMCMesonWithinAccepPtWOWeights, fNameHistoEffi);
            else
                fHistoMonteMesonEffiPt[k+3]       = CalculateMesonEfficiency(fHistoYieldMeson[k+3], fHistoYieldTrueSecMeson[k], fHistoMCMesonWithinAccepPt, fNameHistoEffi);
        }

        for (Int_t k = 0; k < 3; k++){
            // True meson efficiencies for fully unweighted MC, to be compared to MesonEffiPt if those have been created with unweighted histograms
            fNameHistoEffi                          = Form("TrueMeson%sEffiPtUnweighted",nameIntRange[k].Data());
            cout << fNameHistoEffi.Data() << endl;
            if (fHistoMCMesonPtWithinAcceptanceWOWeights)
                fHistoMCTrueMesonEffiPtUnweighted[k]= CalculateMesonEfficiency(fHistoYieldTrueMesonUnweighted[k], NULL, fHistoMCMesonWithinAccepPtWOWeights, fNameHistoEffi);

            // True meson efficiencies with possibly fully weighted inputs on a meson by meson basis in the aliphysics task, should always be used if you start weighting the MC
            // True Meson (only once case, because no normalization)
            fNameHistoEffi                          = Form("TrueMeson%sEffiPt",nameIntRange[k].Data());
            cout << fNameHistoEffi.Data() << endl;
            fHistoMCTrueMesonEffiPt[k]              = CalculateMesonEfficiency(fHistoYieldTrueMeson[k], NULL, fHistoMCMesonWithinAccepPt, fNameHistoEffi);

            // True meson efficiencies with possibly fully weighted inputs taking the average weight per inv mass bin in the original binning of the TrueMesonInvMass vs pT plot
            // should give on average the same as TrueMesonEffiPt
            fNameHistoEffi                          = Form("TrueMeson%sEffiPtReweighted",nameIntRange[k].Data());
            cout << fNameHistoEffi.Data() << endl;
            fHistoMCTrueMesonEffiPtReweighted[k]    = CalculateMesonEfficiency(fHistoYieldTrueMesonReweighted[k], NULL, fHistoMCMesonWithinAccepPt, fNameHistoEffi);

            if (fNewMCOutput){
                for (Int_t j = 0; j < 4; j++){
                    fNameHistoEffi                          = Form("TrueSecFrom%s%sEffiPt",nameSecondaries[j].Data(), nameIntRange[k].Data());
                    cout << "trying to create: "<< fNameHistoEffi.Data() << endl;
                    if (fHistoYieldTrueSecMeson[k][j] && fHistoMCSecPi0PtWAccReb[j]){
                        fHistoMCTrueSecMesonEffiPt[k][j]        = CalculateMesonEfficiency(fHistoYieldTrueSecMeson[k][j], NULL, fHistoMCSecPi0PtWAccReb[j], fNameHistoEffi);
                    } else {
                        cout << Form("TrueSecFrom%s%sEffiPtReweighted",nameSecondaries[j].Data(), nameIntRange[k].Data()) << " could not be created " << endl;
                        if (!fHistoYieldTrueSecMeson[k][j])
                            cout << "true rec yield for " << nameSecondaries[j].Data() << " in int range: " << nameIntRange[k].Data() << " was missing" << endl;
                        if (!fHistoMCSecPi0PtWAccReb[j])
                            cout << "MC yield for " << nameSecondaries[j].Data() << " in acceptance was missing" << endl;
                    }
                }
            }
        }

        // Calculation of secondary fractions using unweighted histograms, as secondaries are never weighted
        TH1D* fHistoYieldTrueMesonSecPlusPrim[3];
        for (Int_t k = 0; k < 3; k++){
            fHistoYieldTrueMesonSecPlusPrim[k]          = (TH1D*)fHistoYieldTrueMesonUnweighted[k]->Clone(Form("fHistoYieldTrueMesonSecPlusPrim%s",nameIntRange[k].Data()));
            for (Int_t j = 0; j< 4; j++){
                fHistoYieldTrueMesonSecPlusPrim[k]->Add(fHistoYieldTrueSecMeson[k][j]);
            }
            for (Int_t j = 0; j< 4; j++){
                fHistoYieldTrueSecFracMeson[k][j] = CalculateSecondaryFractions(    fHistoYieldTrueMesonSecPlusPrim[k], fHistoYieldTrueSecMeson[k][j],
                                                                                    Form("TrueSecFrom%sFrac%s",nameSecondaries[j].Data(),nameIntRange[k].Data()));
            }
        }
        cout<<"Save Correction Hists\n";
        SaveCorrectionHistos(fCutSelection, fPrefix2);
    }

    cout<<"Save Hists\n";
    SaveHistos(fIsMC, cutSelection, fPrefix2, false);



    //////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////


    TString nameMesonSub    = "";
    TString nameCanvasSub   = "";
    TString namePadSub      = "";

    TString labelUsedFitsRatio="pol2";
    TString labelsOtherFitsRatio[iNumberOfOtherSigToBckRatioFits]= {"pol1 SigToBG Fit", "pol3 SigToBG Fit"};  // array of size iNumberOfOtherSigToBckRatioFits;
    TString labelsOtherFitsRatio_withGaus[iNumberOfOtherSigToBckRatioFits]= {"pol1 SigToBG Fit", "pol3 SigToBG Fit"};  // array of size iNumberOfOtherSigToBckRatioFits;
    if ( usePolNForBackgroundScaling == 1){
        labelUsedFitsRatio="pol1";
        labelsOtherFitsRatio[0]="pol2 SigToBG Fit";
        labelsOtherFitsRatio[1]="pol3 SigToBG Fit";
        if ((FitRangeSigBckRatioOption)<20){
            labelsOtherFitsRatio_withGaus[0]="pol2 with Gaus SigToBG Fit";
            labelsOtherFitsRatio_withGaus[1]="pol3 with Gaus SigToBG Fit";
        } else {
            labelsOtherFitsRatio_withGaus[0]="pol2 with GausExp SigToBG Fit";
            labelsOtherFitsRatio_withGaus[1]="pol3 with GausExp SigToBG Fit";
        }
    } else if ( usePolNForBackgroundScaling == 3){
        labelUsedFitsRatio="pol3";
        labelsOtherFitsRatio[0]="pol1 SigToBG Fit";
        labelsOtherFitsRatio[1]="pol2 SigToBG Fit";
        if ((FitRangeSigBckRatioOption)<20){
            labelsOtherFitsRatio_withGaus[0]="pol1 with Gaus SigToBG Fit";
            labelsOtherFitsRatio_withGaus[1]="pol2 with Gaus SigToBG Fit";
        } else {
            labelsOtherFitsRatio_withGaus[0]="pol1 with GausExp SigToBG Fit";
            labelsOtherFitsRatio_withGaus[1]="pol2 with GausExp SigToBG Fit";
        }
    } else {
        labelUsedFitsRatio="pol2";
        labelsOtherFitsRatio[0]="pol1 SigToBG Fit";
        labelsOtherFitsRatio[1]="pol3 SigToBG Fit";
        if ((FitRangeSigBckRatioOption)<20){
            labelsOtherFitsRatio_withGaus[0]="pol1 with Gaus SigToBG Fit";
            labelsOtherFitsRatio_withGaus[1]="pol3 with Gaus SigToBG Fit";
        } else {
            labelsOtherFitsRatio_withGaus[0]="pol1 with GausExp SigToBG Fit";
            labelsOtherFitsRatio_withGaus[1]="pol3 with GausExp SigToBG Fit";
        }
    }

    TString optionBackgroundStr = "mixed evt. subtr. #it{M}_{#gamma#gamma}";
    if(optionBackgroundMethod == 1){
      optionBackgroundStr = "rotation bck. subtr. #it{M}_{#gamma#gamma}";
    }else if(optionBackgroundMethod == 2){
      optionBackgroundStr = "MC true bck. subtr. #it{M}_{#gamma#gamma}";
    }

    TString plotPrefix  = Form("%s/%s_%s",outputDir.Data(),fPrefix.Data(),fPrefix2.Data());
    TString plotSuffix  = Form("%s_%s.%s",fPeriodFlag.Data(),fCutSelection.Data(),Suffix.Data());
    TString nameMeson   = Form("%s_MesonWithBck%s", plotPrefix.Data(), plotSuffix.Data());
    TString nameCanvas  = "MesonWithBckCanvas";
    TString namePad     = "MesonWithBckPad";
    cout << nameMeson.Data() << endl;

    PlotInvMassInPtBins( fHistoMappingGGInvMassPtBin, fHistoMappingBackNormInvMassPtBin, nameMeson, nameCanvas, namePad, fMesonMassPlotRange, fDate, fPrefix, fRow, fColumn, fStartPtBin, fNBinsPt,
      fBinsPt, fTextMeasurement, fIsMC ,fDecayChannel, fDetectionProcess, fCollisionSystem, ((TString)optionBackgroundStr.Copy()).Remove(optionBackgroundStr.Length() - 22, 22 ) );


    if (fCrysFitting == 0){
        // plot Templates
        nameMesonSub    = Form("%s_TemplateStacked%s", plotPrefix.Data(), plotSuffix.Data());
        nameCanvasSub   = "CanvasTemplateStacked";
        namePadSub      = "PadTemplateStacked";
        cout << nameMesonSub.Data() << endl;
        PlotTemplatesInvMassInPtBins( fHistoMappingGGInvMassPtBin, fHistoMappingBackNormInvMassPtBin, fHistoMappingTruePi0ScaledInvMassPtBins, fHistoMappingTrueEtaScaledInvMassPtBins, nameMesonSub, nameCanvasSub, namePadSub,
                                              fMesonMassPlotRange, fDate, fPrefix, fRow, fColumn, fStartPtBin, fNBinsPt, fBinsPt, fTextMeasurement, fIsMC,fDecayChannel, fDetectionProcess,
                                              fCollisionSystem, "MC validated", kTRUE, optionBackgroundStr);
        nameMesonSub    = Form("%s_TemplateStacked_Left%s", plotPrefix.Data(), plotSuffix.Data());
        nameCanvasSub   = "CanvasTemplateStackedLeft";
        namePadSub      = "PadTemplateStackedLeft";
        cout << nameMesonSub.Data() << endl;
        PlotTemplatesInvMassInPtBins( fHistoMappingGGInvMassPtBin, fHistoMappingBackNormInvMassLeftPtBin, fHistoMappingTruePi0ScaledInvMassLeftPtBins, fHistoMappingTrueEtaScaledInvMassLeftPtBins, nameMesonSub, nameCanvasSub, namePadSub,
                                              fMesonMassPlotRange, fDate, fPrefix, fRow, fColumn, fStartPtBin, fNBinsPt, fBinsPt, fTextMeasurement, fIsMC,fDecayChannel, fDetectionProcess,
                                              fCollisionSystem, "MC validated", kTRUE, optionBackgroundStr);
        // plot with std fit
        nameMesonSub    = Form("%s_MesonSubtracted%s", plotPrefix.Data(), plotSuffix.Data());
        nameCanvasSub   = "MesonCanvasSubtracted";
        namePadSub      = "MesonPadSubtracted";
        cout << nameMesonSub.Data() << endl;
        PlotWithFitSubtractedInvMassInPtBins( fHistoMappingSignalInvMassPtBin, fHistoMappingTrueMesonInvMassPtBins, fFitSignalInvMassPtBinStd, nameMesonSub, nameCanvasSub, namePadSub,
                                              fMesonMassPlotRange, fDate, fPrefix, fRow, fColumn, fStartPtBin, fNBinsPt, fBinsPt, fTextMeasurement, fIsMC,fDecayChannel, fDetectionProcess,
                                              fCollisionSystem, "MC validated", kTRUE, "Fit", optionBackgroundStr);

        // plot with fit based on linear remaining BG
        if(optionTemplateFit > -1){ // plotted additionally only if not standard
            nameMesonSub    = Form("%s_MesonSubtractedWithLinBG%s", plotPrefix.Data(), plotSuffix.Data());
            nameCanvasSub   = "MesonCanvasSubtractedWithLinBG";
            namePadSub      = "MesonPadSubtractedWithLinBG";
            cout << nameMesonSub.Data() << endl;
            PlotWithFitSubtractedInvMassInPtBins( fHistoMappingSignalInvMassPtBin, fHistoMappingTrueMesonInvMassPtBins, fFitSignalInvMassPtBin, nameMesonSub, nameCanvasSub, namePadSub,
                                                  fMesonMassPlotRange, fDate, fPrefix, fRow, fColumn, fStartPtBin, fNBinsPt, fBinsPt, fTextMeasurement, fIsMC,fDecayChannel, fDetectionProcess,
                                                  fCollisionSystem, "MC validated", kTRUE, "Fit", optionBackgroundStr);
        }

        // plot with different fits based on all possible remaining BG functions
        nameMesonSub    = Form("%s_MesonSubtractedWithOther%s", plotPrefix.Data(), plotSuffix.Data());
        nameCanvasSub   = "MesonCanvasSubtractedWithOther";
        namePadSub      = "MesonPadSubtractedWithOther";
        cout << nameMesonSub.Data() << endl;
        PlotWithManyFitSubtractedInvMassInPtBins(   fHistoMappingSignalInvMassPtBin, fFitSignalInvMassPtBin, fFitSignalWithOtherBGInvMassPtBin, nOtherFits, labelsOtherFits, nameMesonSub,
                                                    nameCanvasSub, namePadSub, fMesonMassPlotRange, fDate, fPrefix, fRow, fColumn, fStartPtBin, fNBinsPt, fBinsPt,
                                                    fTextMeasurement, fIsMC,fDecayChannel, fDetectionProcess, fCollisionSystem, "MC validated", kTRUE, "pol1 BG", optionBackgroundStr);

        // plot with only remaing BG parts of the different fits (all possible remaining BG functions)
        nameMesonSub    = Form("%s_MesonSubtractedWithOtherOnlyBGFits%s", plotPrefix.Data(), plotSuffix.Data());
        nameCanvasSub   = "MesonCanvasSubtractedWithOtherOnlyBGFits";
        namePadSub      = "MesonPadSubtractedWithOtherOnlyBGFits";
        cout << nameMesonSub.Data() << endl;
        PlotWithManyFitSubtractedInvMassInPtBins(   fHistoMappingSignalInvMassPtBin, fFitBckInvMassPtBin, fFitBckOtherInvMassPtBin, nOtherFits, labelsOtherFits, nameMesonSub,
                                                    nameCanvasSub, namePadSub, fMesonMassPlotRange, fDate, fPrefix, fRow, fColumn, fStartPtBin, fNBinsPt, fBinsPt,
                                                    fTextMeasurement, fIsMC,fDecayChannel, fDetectionProcess, fCollisionSystem, "MC validated", kTRUE, "pol1 BG", optionBackgroundStr);

        nameMesonSub    = Form("%s_MesonSubtractedRemaingBGSubtracted%s", plotPrefix.Data(), plotSuffix.Data());
        nameCanvasSub   = "MesonCanvasSubtracted";
        namePadSub      = "MesonPadSubtracted";
        cout << nameMesonSub.Data() << endl;
        PlotWithFitSubtractedInvMassInPtBins( fHistoMappingSignalRemainingBGSubInvMassPtBin, fHistoMappingTrueMesonInvMassPtBins, fFitBGSubtractedSignalInvMassPtBin, nameMesonSub, nameCanvasSub, namePadSub,
                                            fMesonMassPlotRange, fDate, fPrefix, fRow, fColumn, fStartPtBin, fNBinsPt, fBinsPt, fTextMeasurement, fIsMC,fDecayChannel, fDetectionProcess,
                                            fCollisionSystem,"MC validated", kTRUE, "Fit", optionBackgroundStr);

        if (iBckSwitch == 5){
            nameMesonSub    = Form("%s_MesonSignalBckRatioFits%s", plotPrefix.Data(), plotSuffix.Data());
            nameCanvasSub   = "MesonCanvasSignalBckRatioFits";
            namePadSub      = "MesonPadSignalBckRatioFits";
            cout << nameMesonSub.Data() << endl;

            PlotWithManyFitSigToBckRatioInPtBins(   fHistoMappingRatioSBInvMassPtBin, fFitPHOSPolSigToBckFitPtBin, fFitPHOSAllOtherSigToBckFits, iNumberOfOtherSigToBckRatioFits, labelsOtherFitsRatio, nameMesonSub,
                                                    nameCanvasSub, namePadSub, fMesonMassPlotRange, fDate, fPrefix, fRow, fColumn, fStartPtBin, fNBinsPt, fBinsPt,
                                                    fTextMeasurement, fIsMC,fDecayChannel, fDetectionProcess, fCollisionSystem, "MC validated", kTRUE, Form("%s SigToBG Fit", labelUsedFitsRatio.Data()));

            nameMesonSub    = Form("%s_MesonSignalBckRatioFits_withGaus%s", plotPrefix.Data(), plotSuffix.Data());
            nameCanvasSub   = "MesonCanvasSignalBckRatioFits_withGaus";
            namePadSub      = "MesonPadSignalBckRatioFits_withGaus";
            cout << nameMesonSub.Data() << endl;

            PlotWithManyFitSigToBckRatioInPtBins(   fHistoMappingRatioSBInvMassPtBin, fFitPHOSPolSigToBckFitPtBin_withGaus, fFitPHOSAllOtherSigToBckFits_withGaus, iNumberOfOtherSigToBckRatioFits, labelsOtherFitsRatio_withGaus, nameMesonSub,
                                                    nameCanvasSub, namePadSub, fMesonMassPlotRange, fDate, fPrefix, fRow, fColumn, fStartPtBin, fNBinsPt, fBinsPt,
                                                    fTextMeasurement, fIsMC,fDecayChannel, fDetectionProcess, fCollisionSystem, "MC validated", kTRUE, Form("%s withGaus SigToBG Fit", labelUsedFitsRatio.Data()));

        }

        // plot with full fit and remaining BG fit (std)
        nameMesonSub    = Form("%s_MesonSubtractedWithFits%s", plotPrefix.Data(), plotSuffix.Data());
        nameCanvasSub   = "MesonCanvasSubtractedWithFits";
        namePadSub      = "MesonPadSubtractedWithFits";
        PlotWith2FitsSubtractedInvMassInPtBins( fHistoMappingSignalInvMassPtBin, fHistoMappingTrueMesonInvMassPtBins, fFitSignalInvMassPtBinStd, fFitBckInvMassPtBinStd, nameMesonSub, nameCanvasSub, namePadSub,
                                                fMesonMassPlotRange, fDate, fPrefix, fRow, fColumn, fStartPtBin, fNBinsPt, fBinsPt, fTextMeasurement, fIsMC,fDecayChannel, fDetectionProcess,
                                                fCollisionSystem, "MC validated", kTRUE, "Fit", optionBackgroundStr);


        // plot with full fit and remaining BG fit (linear)
        if(optionTemplateFit > -1){ // plotted additionally if not standard
            nameMesonSub    = Form("%s_MesonSubtractedWithLinBGFits%s", plotPrefix.Data(), plotSuffix.Data());
            nameCanvasSub   = "MesonCanvasSubtractedWithLinBGFits";
            namePadSub      = "MesonPadSubtractedWithLinBGFits";
            PlotWith2FitsSubtractedInvMassInPtBins( fHistoMappingSignalInvMassPtBin, fHistoMappingTrueMesonInvMassPtBins, fFitSignalInvMassPtBin, fFitBckInvMassPtBin, nameMesonSub, nameCanvasSub, namePadSub,
                                                    fMesonMassPlotRange, fDate, fPrefix, fRow, fColumn, fStartPtBin, fNBinsPt, fBinsPt, fTextMeasurement, fIsMC,fDecayChannel, fDetectionProcess,
                                                    fCollisionSystem, "MC validated", kTRUE, "Fit", optionBackgroundStr);
        }

        // plot with full fit (pure gaussian)
        nameMesonSub    = Form("%s_MesonSubtractedPureGaussianFit%s", plotPrefix.Data(), plotSuffix.Data());
        nameCanvasSub   = "MesonCanvasSubtractedPureGaussianFit";
        namePadSub      = "MesonPadSubtractedPureGaussianFit";
        cout << nameMesonSub.Data() << endl;
        PlotWithFitSubtractedInvMassInPtBins( fHistoMappingSignalInvMassPtBin, fHistoMappingTrueMesonInvMassPtBins, fFitSignalGaussianInvMassPtBin, nameMesonSub, nameCanvasSub, namePadSub,
                                              fMesonMassPlotRange, fDate, fPrefix, fRow, fColumn, fStartPtBin, fNBinsPt, fBinsPt, fTextMeasurement, fIsMC,fDecayChannel, fDetectionProcess,
                                              fCollisionSystem, "MC validated", kTRUE, "Fit", optionBackgroundStr);

        // plot with full fit (std, left normalization)
        nameMesonSub    = Form("%s_MesonSubtractedLeft%s", plotPrefix.Data(), plotSuffix.Data());
        nameCanvasSub   = "MesonCanvasSubtractedLeft";
        namePadSub      = "MesonPadSubtractedLeft";
        cout << nameMesonSub.Data() << endl;
        PlotWithFitSubtractedInvMassInPtBins( fHistoMappingSignalInvMassLeftPtBin, fHistoMappingTrueMesonInvMassPtBins, fFitInvMassLeftPtBin, nameMesonSub, nameCanvasSub, namePadSub,
                                              fMesonMassPlotRange, fDate, fPrefix, fRow, fColumn, fStartPtBin, fNBinsPt, fBinsPt, fTextMeasurement, fIsMC, fDecayChannel, fDetectionProcess,
                                              fCollisionSystem, "MC validated", kTRUE, "Fit", optionBackgroundStr);

        // plot example bin including std choice of remaining background
        cout << "Example bin (" << meson << "): " << fExampleBin << endl;
        TString triggerInt;
        if(fModeHeavy<100) triggerInt = fEventCutSelectionRead(GetEventSelectSpecialTriggerCutPosition(),     2);
        else               triggerInt = fEventCutSelectionRead(GetEventSelectSpecialTriggerCutPositionHeavy(),2);
        PlotExampleInvMassBinsV2( fHistoMappingGGInvMassPtBin[fExampleBin], fHistoMappingSignalInvMassPtBin[fExampleBin], fHistoMappingBackNormInvMassPtBin[fExampleBin],
                                  fFitSignalInvMassPtBinStd[fExampleBin], fExampleBin, outputDir.Data(),Suffix.Data(), fMesonMassPlotRange, pictDrawingCoordinatesFWHM,
                                  fNEvents, fDate, fPrefix, fPrefix2, fThesis, fCollisionSystem, fBinsPt, fDecayChannel, fDetectionProcess, triggerInt.Atoi(), fExampleBinScaleFac,
                                  fMode, addSig,
                                  kFALSE, NULL, optionOtherResBckAsStd, optionBackgroundMethod );

        PlotExampleInvMassBinsTemplate( fHistoMappingGGInvMassPtBin[fExampleBin], fHistoMappingBackNormInvMassPtBin[fExampleBin], fHistoMappingTruePi0ScaledInvMassPtBins[fExampleBin], fHistoMappingTrueEtaScaledInvMassPtBins[fExampleBin],
                                  fExampleBin, outputDir.Data(),Suffix.Data(), fMesonMassPlotRange, pictDrawingCoordinatesFWHM,
                                  fNEvents, fDate, fPrefix, fPrefix2, fThesis, fCollisionSystem, fBinsPt, fDecayChannel, fDetectionProcess, triggerInt, fExampleBinScaleFac,
                                  fMode , optionBackgroundMethod, optionTemplateFit);

    } else {
        nameMesonSub    = Form("%s_MesonSubtracted%s", plotPrefix.Data(), plotSuffix.Data());
        nameCanvasSub   = "MesonCanvasSubtracted";
        namePadSub      = "MesonPadSubtracted";
        cout << nameMesonSub.Data() << endl;
        PlotWithBGFitSubtractedInvMassInPtBins(fHistoMappingSignalInvMassPtBin, fHistoMappingRemainingBGInvMassPtBin, fHistoMappingSignalRemainingBGSubInvMassPtBin, fFitRemainingBGInvMassPtBin,
                                               nameMesonSub, nameCanvasSub, namePadSub, fMesonMassPlotRange, fDate, fPrefix, fRow, fColumn, fStartPtBin, fNBinsPt, fBinsPt, fTextMeasurement, fIsMC,
                                               fDecayChannel, fDetectionProcess, fCollisionSystem );

        nameMesonSub    = Form("%s_MesonSubtractedLeft%s", plotPrefix.Data(), plotSuffix.Data());
        nameCanvasSub   = "MesonCanvasSubtracted";
        namePadSub      = "MesonPadSubtracted";
        cout << nameMesonSub.Data() << endl;
        PlotWithBGFitSubtractedInvMassInPtBins( fHistoMappingSignalInvMassLeftPtBin, fHistoMappingRemainingBGInvMassLeftPtBin, fHistoMappingSignalRemainingBGSubInvMassLeftPtBin, fFitRemainingBGInvMassLeftPtBin,
                                                nameMesonSub, nameCanvasSub, namePadSub, fMesonMassPlotRange, fDate, fPrefix, fRow, fColumn, fStartPtBin, fNBinsPt, fBinsPt, fTextMeasurement, fIsMC,
                                                fDecayChannel, fDetectionProcess, fCollisionSystem );

        nameMesonSub    = Form("%s_MesonSubtractedRemaingBGSubtracted%s", plotPrefix.Data(), plotSuffix.Data());
        nameCanvasSub   = "MesonCanvasSubtracted";
        namePadSub      = "MesonPadSubtracted";
        cout << nameMesonSub.Data() << endl;
        PlotWithFitSubtractedInvMassInPtBins( fHistoMappingSignalRemainingBGSubInvMassPtBin, fHistoMappingTrueMesonInvMassPtBins, fFitSignalInvMassPtBin, nameMesonSub, nameCanvasSub, namePadSub,
                                            fMesonMassPlotRange, fDate, fPrefix, fRow, fColumn, fStartPtBin, fNBinsPt, fBinsPt, fTextMeasurement, fIsMC,fDecayChannel, fDetectionProcess,
                                            fCollisionSystem,"MC validated", kTRUE, "Fit", optionBackgroundStr);

        nameMesonSub    = Form("%s_MesonSubtractedRemaingBGSubtractedLeft%s", plotPrefix.Data(), plotSuffix.Data());
        nameCanvasSub   = "MesonCanvasSubtractedLeft";
        namePadSub      = "MesonPadSubtractedLeft";
        cout << nameMesonSub.Data() << endl;
        PlotWithFitSubtractedInvMassInPtBins(
            fHistoMappingSignalRemainingBGSubInvMassLeftPtBin, fHistoMappingTrueMesonInvMassPtBins, fFitInvMassLeftPtBin, nameMesonSub, nameCanvasSub, namePadSub,
            fMesonMassPlotRange, fDate, fPrefix, fRow, fColumn, fStartPtBin, fNBinsPt, fBinsPt, fTextMeasurement, fIsMC, fDecayChannel, fDetectionProcess,
            fCollisionSystem, "MC validated", kTRUE, "Fit", optionBackgroundStr);

        PlotExampleInvMassBinsV2(
            fHistoMappingGGInvMassPtBin[fExampleBin], fHistoMappingSignalRemainingBGSubInvMassPtBin[fExampleBin], fHistoMappingBackNormInvMassPtBin[fExampleBin],
            fFitSignalInvMassPtBin[fExampleBin], fExampleBin, outputDir.Data(),Suffix.Data(), fMesonMassPlotRange, pictDrawingCoordinatesFWHM, fNEvents, fDate, fPrefix, fPrefix2,
            fThesis, fCollisionSystem, fBinsPt, fDecayChannel, fDetectionProcess, triggerSet, fExampleBinScaleFac, fMode);
    }
    nameMeson       = Form("%s_MesonWithBckLeft%s", plotPrefix.Data(), plotSuffix.Data());
    nameCanvas      = "MesonWithBckCanvasLeft";
    namePad         = "MesonWithBckPadLeft";
    cout << nameMeson.Data() << endl;

    PlotInvMassInPtBins( fHistoMappingGGInvMassPtBin, fHistoMappingBackNormInvMassLeftPtBin, nameMeson, nameCanvas, namePad,  fMesonMassPlotRange, fDate, fPrefix, fRow, fColumn, fStartPtBin,
                        fNBinsPt, fBinsPt, fTextMeasurement, fIsMC, fDecayChannel, fDetectionProcess, fCollisionSystem, ((TString)optionBackgroundStr.Copy()).Remove(optionBackgroundStr.Length() - 22, 22 ));
    if(fIsMC){
        TString nameMesonTrue   = Form("%s_TrueMesonFitted%s", plotPrefix.Data(), plotSuffix.Data());
        TString nameCanvasTrue  = "TrueMesonCanvasFitted";
        TString namePadTrue     = "TrueMesonPadFitted";
        cout << nameMesonTrue.Data() << endl;
        PlotWithFitSubtractedInvMassInPtBins(fHistoMappingTrueMesonInvMassPtBins, fHistoMappingTrueMesonInvMassPtBins, fFitTrueSignalInvMassPtBin, nameMesonTrue, nameCanvasTrue, namePadTrue,
                                            fMesonMassPlotRange, fDate, fPrefix, fRow, fColumn, fStartPtBin, fNBinsPt, fBinsPt, fTextMeasurement, fIsMC,fDecayChannel, fDetectionProcess,
                                            fCollisionSystem, "MC validated weighted",kFALSE, "Fit", optionBackgroundStr);

        nameMesonTrue           = Form("%s_TrueMesonReweightedFitted%s", plotPrefix.Data(), plotSuffix.Data());
        nameCanvasTrue          = "TrueMesonCanvasReweightedFitted";
        namePadTrue             = "TrueMesonPadReweightedFitted";
        cout << nameMesonTrue.Data() << endl;
        PlotWithFitSubtractedInvMassInPtBins(fHistoMappingTrueMesonInvMassPtReweightedBins, fHistoMappingTrueMesonInvMassPtReweightedBins, fFitTrueSignalInvMassPtReweightedBin, nameMesonTrue,
                                            nameCanvasTrue, namePadTrue, fMesonMassPlotRange, fDate, fPrefix, fRow, fColumn, fStartPtBin, fNBinsPt, fBinsPt, fTextMeasurement, fIsMC, fDecayChannel,
                                            fDetectionProcess, fCollisionSystem, "MC validated reweighted",kFALSE, "Fit", optionBackgroundStr);

        nameMesonTrue           = Form("%s_TrueMesonUnweightedFitted%s", plotPrefix.Data(), plotSuffix.Data());
        nameCanvasTrue          = "TrueMesonCanvasUnweightedFitted";
        namePadTrue             = "TrueMesonPadUnweightedFitted";
        cout << nameMesonTrue.Data() << endl;
        PlotWithFitSubtractedInvMassInPtBins(fHistoMappingTrueMesonInvMassPtUnweightedBins, fHistoMappingTrueMesonInvMassPtUnweightedBins, fFitTrueSignalInvMassPtUnweightedBin, nameMesonTrue,
                                            nameCanvasTrue, namePadTrue, fMesonMassPlotRange, fDate, fPrefix, fRow, fColumn, fStartPtBin, fNBinsPt, fBinsPt, fTextMeasurement, fIsMC, fDecayChannel,
                                            fDetectionProcess, fCollisionSystem, "MC validated unweighted",kFALSE, "Fit", optionBackgroundStr);

        nameMesonTrue           = Form("%s_TrueMesonFittedPureGaussian%s", plotPrefix.Data(), plotSuffix.Data());
        nameCanvasTrue          = "TrueMesonCanvasFittedPureGaussian";
        namePadTrue             = "TrueMesonPadFittedPureGaussian";
        cout << nameMesonTrue.Data() << endl;
        PlotWithFitSubtractedInvMassInPtBins(fHistoMappingTrueMesonInvMassPtBins, fHistoMappingTrueMesonInvMassPtBins, fFitTrueSignalGaussianInvMassPtBin, nameMesonTrue, nameCanvasTrue,
                                            namePadTrue, fMesonMassPlotRange, fDate, fPrefix, fRow, fColumn, fStartPtBin, fNBinsPt, fBinsPt, fTextMeasurement, fIsMC,fDecayChannel,
                                            fDetectionProcess, fCollisionSystem, "MC validated weighted",kFALSE, "Fit", optionBackgroundStr);

        nameMesonTrue           = Form("%s_BackRatioToTrueBack%s", plotPrefix.Data(), plotSuffix.Data());
        nameCanvasTrue          = "CanvasBackRatioToTrueBack";
        namePadTrue             = "PadBackRatioToTrueBack";
        cout << nameMesonTrue.Data() << endl;
        PlotRatioToTrueBackInvMassInPtBins(fHistoMappingBackNormInvMassPtBin, fHistoMappingTrueBackgroundInvMassPtBins, nameMesonTrue, nameCanvasTrue,
                                            namePadTrue, fMesonMassPlotRange, fDate, fPrefix, fRow, fColumn, fStartPtBin, fNBinsPt, fBinsPt, fTextMeasurement, fIsMC,fDecayChannel,
                                            fDetectionProcess, fCollisionSystem, "MC validated weighted",kFALSE, ((TString)optionBackgroundStr.Copy()).Remove(optionBackgroundStr.Length() - 28, 28 ));

        if (meson.Contains("Pi0")){
            nameMesonTrue       = Form("%s_TrueMesonSecondary%s", plotPrefix.Data(), plotSuffix.Data());
            nameCanvasTrue      = "TrueMesonCanvasSec";
            namePadTrue         = "TrueMesonPadSec";
            cout << nameMesonTrue.Data() << endl;
            PlotInvMassSecondaryInPtBins( fHistoMappingTrueMesonInvMassPtBins, fHistoMappingTrueSecMesonInvMassPtBins[3], fHistoMappingTrueSecMesonInvMassPtBins[1], nameMesonTrue, nameCanvasTrue,
                                        namePadTrue, fMesonMassPlotRange, fDate, fPrefix, fRow, fColumn, fStartPtBin, fNBinsPt, fBinsPt, fTextMeasurement, fIsMC, fDecayChannel, fDetectionProcess,
                                        fCollisionSystem);
        }

        if (fEnableDCMeson){
            nameMesonTrue       = Form("%s_TrueMesonDoubleCounting%s", plotPrefix.Data(), plotSuffix.Data());
            nameCanvasTrue      = "TrueMesonCanvasDC";
            namePadTrue         = "TrueMesonPadDC";
            cout << nameMesonTrue.Data() << endl;
            PlotInvMassDoubleCountingInPtBins( fHistoMappingTrueMesonInvMassPtBins, fHistoMappingTrueMesonDCInvMassPtBins, nameMesonTrue, nameCanvasTrue,
                                        namePadTrue, fMesonMassPlotRange, fDate, fPrefix, fRow, fColumn, fStartPtBin, fNBinsPt, fBinsPt, fTextMeasurement, fIsMC, fDecayChannel, fDetectionProcess,
                                        fCollisionSystem);
        }

        if (fAdvancedMesonQA && (fMode == 2 || fMode == 13 || fMode == 3 || fMode == 4 || fMode == 12 || fMode == 5 || fMode==14)){

            nameMesonTrue       = Form("%s_TrueMesonCaloPhoton%s", plotPrefix.Data(), plotSuffix.Data());
            nameCanvasTrue      = "TrueMesonCaloPhotonCanvasFitted";
            namePadTrue         = "TrueMesonCaloPhotonPadFitted";
            cout << nameMesonTrue.Data() << endl;
            PlotWithFitSubtractedInvMassInPtBins(fHistoMappingTrueMesonCaloPhotonInvMassPtBins, fHistoMappingTrueMesonCaloPhotonInvMassPtBins, fFitTrueSignalCaloPhotonInvMassPtBin, nameMesonTrue,
                                                nameCanvasTrue, namePadTrue, fMesonMassPlotRange, fDate, fPrefix, fRow, fColumn, fStartPtBin, fNBinsPt, fBinsPt, fTextMeasurement, fIsMC,
                                                fDecayChannel, fDetectionProcess, fCollisionSystem, "validated #gamma#gamma",kFALSE);

            nameMesonTrue       = Form("%s_TrueMesonCaloConvPhoton%s", plotPrefix.Data(), plotSuffix.Data());
            nameCanvasTrue      = "TrueMesonCaloConvPhotonCanvasFitted";
            namePadTrue         = "TrueMesonCaloConvPhotonPadFitted";
            cout << nameMesonTrue.Data() << endl;
            PlotWithFitSubtractedInvMassInPtBins(fHistoMappingTrueMesonCaloConvPhotonInvMassPtBins, fHistoMappingTrueMesonCaloConvPhotonInvMassPtBins, fFitTrueSignalCaloConvPhotonInvMassPtBin,
                                                nameMesonTrue, nameCanvasTrue, namePadTrue, fMesonMassPlotRange, fDate, fPrefix, fRow, fColumn, fStartPtBin, fNBinsPt, fBinsPt, fTextMeasurement,
                                                fIsMC, fDecayChannel, fDetectionProcess, fCollisionSystem, "validated #gamma_{conv}#gamma_{conv}",kFALSE);

            nameMesonTrue       = Form("%s_TrueMesonCaloMergedCluster%s", plotPrefix.Data(), plotSuffix.Data());
            nameCanvasTrue      = "TrueMesonCaloMergedClusterCanvasFitted";
            namePadTrue         = "TrueMesonCaloMergedClusterPadFitted";
            cout << nameMesonTrue.Data() << endl;
            PlotWithFitSubtractedInvMassInPtBins(fHistoMappingTrueMesonCaloMergedClusterInvMassPtBins, fHistoMappingTrueMesonCaloMergedClusterInvMassPtBins,
                                                fFitTrueSignalCaloMergedClusterInvMassPtBin, nameMesonTrue, nameCanvasTrue, namePadTrue, fMesonMassPlotRange, fDate, fPrefix, fRow, fColumn,
                                                fStartPtBin, fNBinsPt, fBinsPt, fTextMeasurement, fIsMC, fDecayChannel, fDetectionProcess, fCollisionSystem, "validated #gamma's merged",kFALSE);

            nameMesonTrue       = Form("%s_TrueMesonCaloMergedClusterPartConv%s", plotPrefix.Data(), plotSuffix.Data());
            nameCanvasTrue      = "TrueMesonCaloMergedClusterPartConvCanvasFitted";
            namePadTrue         = "TrueMesonCaloMergedClusterPartConvPadFitted";
            cout << nameMesonTrue.Data() << endl;
            PlotWithFitSubtractedInvMassInPtBins(fHistoMappingTrueMesonCaloMergedClusterPartConvInvMassPtBins, fHistoMappingTrueMesonCaloMergedClusterPartConvInvMassPtBins,
                                                fFitTrueSignalCaloMergedClusterPartConvInvMassPtBin, nameMesonTrue, nameCanvasTrue, namePadTrue, fMesonMassPlotRange, fDate, fPrefix, fRow, fColumn,
                                                fStartPtBin, fNBinsPt, fBinsPt, fTextMeasurement, fIsMC, fDecayChannel, fDetectionProcess, fCollisionSystem, "val. #gamma's mer., part. conv",
                                                kFALSE);


            nameMesonTrue       = Form("%s_TrueMesonDecomposedMerged%s", plotPrefix.Data(), plotSuffix.Data());
            cout << nameMesonTrue.Data() << endl;
            PlotTrueInvMassSplittedInMergedInPtBins(fHistoMappingTrueFullMesonInvMassPtBins, fHistoMappingTrueMesonCaloMergedClusterInvMassPtBins,
                                                    fHistoMappingTrueMesonCaloMergedClusterPartConvInvMassPtBins, nameMesonTrue, nameCanvasTrue, namePadTrue, fMesonMassPlotRange, fDate, fPrefix,
                                                    fRow, fColumn, fStartPtBin, fNBinsPt, fBinsPt, fTextMeasurement, fIsMC, fDecayChannel, fDetectionProcess, fCollisionSystem, fMode);

        }

        //cout << "Debug; ExtractSignalV2.C, line " << __LINE__ << endl;
        if (fAdvancedMesonQA && (fMode == 4 || fMode == 12 || fMode == 5 || fMode==14)){
            nameMesonTrue       = Form("%s_TrueMesonMixedCaloConvPhoton%s", plotPrefix.Data(), plotSuffix.Data());
            nameCanvasTrue      = "TrueMesonMixedCaloConvPhotonCanvasFitted";
            namePadTrue         = "TrueMesonMixedCaloConvPhotonPadFitted";
            cout << nameMesonTrue.Data() << endl;
            PlotWithFitSubtractedInvMassInPtBins(fHistoMappingTrueMesonMixedCaloConvPhotonInvMassPtBins, fHistoMappingTrueMesonMixedCaloConvPhotonInvMassPtBins,
                                                fFitTrueSignalMixedCaloConvPhotonInvMassPtBin, nameMesonTrue, nameCanvasTrue, namePadTrue, fMesonMassPlotRange, fDate, fPrefix, fRow, fColumn,
                                                fStartPtBin, fNBinsPt, fBinsPt, fTextMeasurement, fIsMC, fDecayChannel, fDetectionProcess, fCollisionSystem, "validated #gamma#gamma_{conv}",
                                                kFALSE);

            nameMesonTrue       = Form("%s_TrueMesonDecomposedPhotonsAndElectron%s", plotPrefix.Data(), plotSuffix.Data());
            cout << nameMesonTrue.Data() << endl;
            PlotTrueInvMassSplittedInPhotonAndElectronInPtBins(
                fHistoMappingTrueFullMesonInvMassPtBins, fHistoMappingTrueMesonCaloPhotonInvMassPtBins,  NULL,
                fHistoMappingTrueMesonCaloConvPhotonInvMassPtBins, fHistoMappingTrueMesonMixedCaloConvPhotonInvMassPtBins, nameMesonTrue,
                nameCanvasTrue, namePadTrue, fMesonMassPlotRange, fDate, fPrefix, fRow, fColumn, fStartPtBin, fNBinsPt, fBinsPt, fTextMeasurement,
                fIsMC, fDecayChannel, fDetectionProcess, fCollisionSystem, fMode );
            TString triggerInt;
            if(fModeHeavy<100) triggerInt = fEventCutSelectionRead(GetEventSelectSpecialTriggerCutPosition(),     2);
            else               triggerInt = fEventCutSelectionRead(GetEventSelectSpecialTriggerCutPositionHeavy(),2);
            PlotExampleInvMassBinsMC(
                fHistoMappingTrueFullMesonInvMassPtBins[fExampleBin], fHistoMappingTrueMesonCaloPhotonInvMassPtBins[fExampleBin],  NULL, fHistoMappingTrueMesonCaloConvPhotonInvMassPtBins[fExampleBin], fHistoMappingTrueMesonMixedCaloConvPhotonInvMassPtBins[fExampleBin], fExampleBin, outputDir.Data(),Suffix.Data(), fMesonMassPlotRange, pictDrawingCoordinatesFWHM, fNEvents, fDate, fPrefix, fPrefix2, fThesis, fCollisionSystem, fBinsPt, fDecayChannel, fDetectionProcess, triggerInt.Atoi(), fMode, addSig );


        } else if (fAdvancedMesonQA && (fMode == 2 || fMode == 13 || fMode == 3 || fMode==14) ) {
            nameMesonTrue       = Form("%s_TrueMesonDecomposedPhotonsAndElectron%s", plotPrefix.Data(), plotSuffix.Data());
            cout << nameMesonTrue.Data() << endl;
            PlotTrueInvMassSplittedInPhotonAndElectronInPtBins(
                fHistoMappingTrueFullMesonInvMassPtBins, fHistoMappingTrueMesonCaloPhotonInvMassPtBins, nullptr, fHistoMappingTrueMesonCaloConvPhotonInvMassPtBins, NULL, nameMesonTrue, nameCanvasTrue, namePadTrue, fMesonMassPlotRange, fDate, fPrefix, fRow, fColumn, fStartPtBin, fNBinsPt, fBinsPt, fTextMeasurement, fIsMC, fDecayChannel, fDetectionProcess, fCollisionSystem, fMode );
            TString triggerInt;
            if(fModeHeavy<100) triggerInt = fEventCutSelectionRead(GetEventSelectSpecialTriggerCutPosition(),     2);
            else               triggerInt = fEventCutSelectionRead(GetEventSelectSpecialTriggerCutPositionHeavy(),2);
            PlotExampleInvMassBinsMC(
                fHistoMappingTrueFullMesonInvMassPtBins[fExampleBin], fHistoMappingTrueMesonCaloPhotonInvMassPtBins[fExampleBin], nullptr, fHistoMappingTrueMesonCaloConvPhotonInvMassPtBins[fExampleBin], NULL, fExampleBin, outputDir.Data(),Suffix.Data(), fMesonMassPlotRange, pictDrawingCoordinatesFWHM, fNEvents, fDate, fPrefix, fPrefix2, fThesis, fCollisionSystem, fBinsPt, fDecayChannel, fDetectionProcess, triggerInt.Atoi(), fMode, addSig );
        }

        if( fEnableNormBckHistoComparisonToTrueBck ){
            nameMeson       = Form("%s_MesonWithBckAndTrueBck%s", plotPrefix.Data(), plotSuffix.Data());
            nameCanvas      = "MesonWithBckAndTrueBckCanvas";
            namePad         = "MesonWithBckAndTrueBckPad";
            cout << nameMeson.Data() << endl;

            PlotInvMassInPtBins(
                fHistoMappingBackNormAndRemainingBGInvMassPtBin, fHistoMappingBackNormInvMassPtBin, fHistoMappingTrueAllBckInvMassPtBins, fHistoMappingTrueGGBckInvMassPtBins,
                fHistoMappingTrueContBckInvMassPtBins, fHistoMappingTrueMesonContainedInvMassPtBins, fHistoMappingTrueAsymEClusInvMassPtBins, nameMeson, nameCanvas, namePad,
                fMesonMassPlotRange, fDate, fPrefix, fRow, fColumn, fStartPtBin, fNBinsPt, fBinsPt, fTextMeasurement, fIsMC, fDecayChannel, fDetectionProcess, fCollisionSystem,
                ((TString)optionBackgroundStr.Copy()).Remove(optionBackgroundStr.Length() - 22, 22 ) );
        }
    }



    // **************************************************************************************************************
    // ************************ Scaling factors for template fits      *********************************************
    // **************************************************************************************************************
      TCanvas* canvasScalingFac = new TCanvas("canvasScalingFac","",200,10,1350,900);  // gives the page size
      DrawGammaCanvasSettings( canvasScalingFac, 0.092, 0.01, 0.02, 0.082);

      TLegend* legendScalingFac = GetAndSetLegend2(0.75, 0.95-(0.035*4), 0.95, 0.95, 0.035, 1, "", 42, 0.25);

      Double_t maxScalingFac    = fHistoScaleFacBack->GetMaximum()* 1.5;
      fHistoScaleFacBack->GetYaxis()->SetRangeUser(0, maxScalingFac);
      DrawAutoGammaMesonHistos(   fHistoScaleFacBack,
              "", "#it{p}_{T} (GeV/#it{c})", "scaling factor back.",
              kFALSE, 0., 0.7, kFALSE,
              kFALSE, 0., 0.7,
              kFALSE, 0., 10.);
      DrawGammaSetMarker(fHistoScaleFacBack, 20, 2, kBlack, kBlack);
      DrawGammaSetMarker(fHistoScaleFacLeftBack, 24, 2, kRed + 2, kRed + 2);
      fHistoScaleFacBack->Draw("e1,p");
      legendScalingFac->AddEntry(fHistoScaleFacBack,"norm.","p");
      fHistoScaleFacLeftBack->Draw("same,e1,p");
      legendScalingFac->AddEntry(fHistoScaleFacLeftBack,"left.","p");

      legendScalingFac->Draw();

      PutProcessLabelAndEnergyOnPlot(0.15, 0.25, 0.035, fCollisionSystem.Data(), fTextMeasurement.Data(), fDetectionProcess.Data());
      canvasScalingFac->Update();
      if (fIsMC) canvasScalingFac->SaveAs(Form("%s/%s_MC_ScaleFactorBack_%s.%s",outputDirMon.Data(),fPrefix.Data(),fCutSelection.Data(),Suffix.Data()));
      else canvasScalingFac->SaveAs(Form("%s/%s_data_ScaleFactorBack_%s.%s",outputDirMon.Data(),fPrefix.Data(),fCutSelection.Data(),Suffix.Data()));


      maxScalingFac    = fHistoScaleFacPi0->GetMaximum()* 1.5;
      fHistoScaleFacPi0->GetYaxis()->SetRangeUser(0, maxScalingFac);
      DrawAutoGammaMesonHistos(   fHistoScaleFacBack,
              "", "#it{p}_{T} (GeV/#it{c})", "scaling factor back.",
              kFALSE, 0., 0.7, kFALSE,
              kFALSE, 0., 0.7,
              kFALSE, 0., 10.);
      DrawGammaSetMarker(fHistoScaleFacPi0, 20, 2, kBlack, kBlack);
      DrawGammaSetMarker(fHistoScaleFacLeftPi0, 24, 2, kRed + 2, kRed + 2);
      fHistoScaleFacPi0->Draw("e1,p");
      fHistoScaleFacLeftPi0->Draw("same,e1,p");

      legendScalingFac->Draw();

      PutProcessLabelAndEnergyOnPlot(0.15, 0.25, 0.035, fCollisionSystem.Data(), fTextMeasurement.Data(), fDetectionProcess.Data());
      canvasScalingFac->Update();
      if (fIsMC) canvasScalingFac->SaveAs(Form("%s/%s_MC_ScaleFactorPi0_%s.%s",outputDirMon.Data(),fPrefix.Data(),fCutSelection.Data(),Suffix.Data()));
      else canvasScalingFac->SaveAs(Form("%s/%s_data_ScaleFactorPi0_%s.%s",outputDirMon.Data(),fPrefix.Data(),fCutSelection.Data(),Suffix.Data()));

      maxScalingFac    = fHistoScaleFacEta->GetMaximum()* 1.5;
      fHistoScaleFacEta->GetYaxis()->SetRangeUser(0, maxScalingFac);
      DrawAutoGammaMesonHistos(   fHistoScaleFacBack,
              "", "#it{p}_{T} (GeV/#it{c})", "scaling factor back.",
              kFALSE, 0., 0.7, kFALSE,
              kFALSE, 0., 0.7,
              kFALSE, 0., 10.);
      DrawGammaSetMarker(fHistoScaleFacEta, 20, 2, kBlack, kBlack);
      DrawGammaSetMarker(fHistoScaleFacLeftEta, 24, 2, kRed + 2, kRed + 2);
      fHistoScaleFacEta->Draw("e1,p");
      fHistoScaleFacLeftEta->Draw("same,e1,p");

      legendScalingFac->Draw();

      PutProcessLabelAndEnergyOnPlot(0.15, 0.25, 0.035, fCollisionSystem.Data(), fTextMeasurement.Data(), fDetectionProcess.Data());
      canvasScalingFac->Update();
      if (fIsMC) canvasScalingFac->SaveAs(Form("%s/%s_MC_ScaleFactorEta_%s.%s",outputDirMon.Data(),fPrefix.Data(),fCutSelection.Data(),Suffix.Data()));
      else canvasScalingFac->SaveAs(Form("%s/%s_data_ScaleFactorEta_%s.%s",outputDirMon.Data(),fPrefix.Data(),fCutSelection.Data(),Suffix.Data()));

 }


 //****************************************************************************
 //***** Template Fit of Background + Signal(s) to InvMass distribution *******
 //****************************************************************************
 void MakeFit(TH1D* hGG, TH1D* hBack, TH1D* hTruePi0, TH1D* hTrueEta, bool fdoLeft, int iPt){


     Double_t fitRangeDown = fBGFitRange[0];
     Double_t fitRangeUp =  fBGFitRange[1];
     // if(fdoLeft){
     //   fitRangeDown = fBGFitRangeLeft[0];
     //   fitRangeUp =  fBGFitRangeLeft[1];
     // }

   //-------------------------------------------
   // Setting up the fit
   // //-------------------------------------------
   Int_t nPar = 3;
   // Standard fit with constant scaling factors
   cout<<"optionTemplateFit: "<<optionTemplateFit<<endl;
   cout<<"fScaleFacEtaToPi0: "<<fScaleFacEtaToPi0<<endl;
   if(optionTemplateFit == 0){
     if(fPrefix.Contains("Pi0")){
       nPar = 2;
       fTempFit = new TF1("fTempFit", FitSimple2, fitRangeDown, fitRangeUp, nPar);
     } else {
       nPar = 3;
       if(fScaleFacEtaToPi0 > 0){ // Fix pi0 to eta/pi0 ratio measured in data
         fTempFit = new TF1("fTempFit", FitSimple2Pi0Fixed, fitRangeDown, fitRangeUp, nPar);
         if(fEnergyFlag.Contains("13TeV")){
           if(fIsMC){
             fTempFit->FixParameter(2, 1);
           } else {
             fTempFit->FixParameter(2, fScaleFacEtaToPi0);
           }
         }
       } else {
         fTempFit = new TF1("fTempFit", FitSimple3, fitRangeDown, fitRangeUp, nPar);
       }
     }
     // pol1 scaling for background template to account for wrong background
   } else if(optionTemplateFit == 1){
     nPar = 4;
     fTempFit = new TF1("fTempFit", FitLinearPi0Fixed, fitRangeDown, fitRangeUp, nPar);
     if(fIsMC){
       fTempFit->FixParameter(1, 1.);
     } else {
       fTempFit->FixParameter(1, fScaleFacEtaToPi0);
     }
     fTempFit->SetParameter(2, 1);
     fTempFit->SetParameter(3, 1);
     // pol1 scaling for background template to account for wrong background
   } else if(optionTemplateFit == 2){
     nPar = 5;
     fTempFit = new TF1("fTempFit", FitPlusLinearPi0Fixed, fitRangeDown, fitRangeUp, nPar);
     if(fIsMC){
       fTempFit->FixParameter(1, 1.);
     } else {
       fTempFit->FixParameter(1, fScaleFacEtaToPi0);
     }
     fTempFit->SetParameter(3, 1);
     fTempFit->SetParameter(4, 1);
   }

   //-------------------------------------------
   // Setting up the histos for template fits
   //-------------------------------------------
   if(debugOutputLevel) cerr<<"Debug in ExtractSignalTemplate; LINE: "<<__LINE__<<endl;
   hDataBack1dFit = (TH1D*) hBack->Clone(Form("TempFitHist%s_Back%i", (fdoLeft > 0 ? "Left" : ""), iPt));
   if(debugOutputLevel) cerr<<"Debug in ExtractSignalTemplate; LINE: "<<__LINE__<<endl;
   hMCSignalPi01dFit = (TH1D*) hTruePi0->Clone(Form("TempFitHist%s_TruePi0%i", (fdoLeft > 0 ? "Left" : ""), iPt));
   if(debugOutputLevel) cerr<<"Debug in ExtractSignalTemplate; LINE: "<<__LINE__<<endl;
   hMCSignalEta1dFit = (TH1D*) hTrueEta->Clone(Form("TempFitHist%s_TrueEta%i", (fdoLeft > 0 ? "Left" : ""), iPt));
   if(debugOutputLevel) cerr<<"Debug in ExtractSignalTemplate; LINE: "<<__LINE__<<endl;


   //-------------------------------------------
   // Fit
   //-------------------------------------------
   fTempFit->SetParameter(0, 1);
   // fTempFit->SetParameter(1, 1);
   hGG->Fit(fTempFit, "MR0");
   hGG->Fit(fTempFit, "MR0");
   if(debugOutputLevel) cerr<<"Debug in ExtractSignalTemplate; LINE: "<<__LINE__<<endl;

 }




void ScaleHistos(Int_t iPt, Int_t mode){

  if(debugOutputLevel) cerr<<"Debug in ExtractSignalTemplate; LINE: "<<__LINE__<<endl;
  int NFitPars = fTempFit->GetNpar();
  double scaleBack = 0;
  double scalePi0 = 0;
  double scaleEta = 0;
  if(NFitPars == 3){
    scaleBack = fTempFit->GetParameter(0);
    scalePi0 = fTempFit->GetParameter(1);
    scaleEta = fTempFit->GetParameter(2);
    if(fScaleFacEtaToPi0 > 0){
      scaleBack = fTempFit->GetParameter(0);
      scalePi0 = fTempFit->GetParameter(1);
      scaleEta = fTempFit->GetParameter(1) * fScaleFacEtaToPi0;
    }
  } else if(NFitPars == 2 && fPrefix.CompareTo("Eta") == 0){
    scaleBack = fTempFit->GetParameter(0);
    scaleEta = fTempFit->GetParameter(1);
  } else if(NFitPars == 2 && fPrefix.Contains("Pi0") == 0){
    scaleBack = fTempFit->GetParameter(0);
    scalePi0 = fTempFit->GetParameter(1);
  }
  cout<<"\n\n";
  cout<<"scaleBack: "<<scaleBack<<endl;
  cout<<"scalePi0: "<<scalePi0<<endl;
  cout<<"scaleEta: "<<scaleEta<<endl;
  if(fdoLeft == 0){
    if(debugOutputLevel) cerr<<"Debug in ExtractSignalTemplate; LINE: "<<__LINE__<<endl;
    fHistoMappingBackNormInvMassPtBin[iPt]->Scale(scaleBack);
    fHistoMappingTruePi0InvMassPtBins[iPt]->Scale(scalePi0);
    fHistoMappingTrueEtaInvMassPtBins[iPt]->Scale(scaleEta);

    if(debugOutputLevel) cerr<<"Debug in ExtractSignalTemplate; LINE: "<<__LINE__<<endl;
    fHistoScaleFacBack->SetBinContent(iPt + 1,scaleBack);
    fHistoScaleFacBack->SetBinError(iPt + 1,fTempFit->GetParError(0));
    fHistoScaleFacPi0->SetBinContent(iPt + 1,scalePi0);
    fHistoScaleFacPi0->SetBinError(iPt + 1,fTempFit->GetParError(1));
    fHistoScaleFacEta->SetBinContent(iPt + 1,scaleEta);
    fHistoScaleFacEta->SetBinError(iPt + 1,fTempFit->GetParError(2));
    if(debugOutputLevel) cerr<<"Debug in ExtractSignalTemplate; LINE: "<<__LINE__<<endl;
  } else {
    if(debugOutputLevel) cerr<<"Debug in ExtractSignalTemplate; LINE: "<<__LINE__<<endl;
    fHistoMappingBackNormInvMassLeftPtBin[iPt]->Scale(scaleBack);
    fHistoMappingTruePi0ScaledInvMassLeftPtBins[iPt]->Scale(scalePi0);
    fHistoMappingTrueEtaScaledInvMassLeftPtBins[iPt]->Scale(scaleEta);

    fHistoScaleFacLeftBack->SetBinContent(iPt + 1,scaleBack);
    fHistoScaleFacLeftBack->SetBinError(iPt + 1,fTempFit->GetParError(0));
    fHistoScaleFacLeftPi0->SetBinContent(iPt + 1,scalePi0);
    fHistoScaleFacLeftPi0->SetBinError(iPt + 1,fTempFit->GetParError(1));
    fHistoScaleFacLeftEta->SetBinContent(iPt + 1,scaleEta);
    fHistoScaleFacLeftEta->SetBinError(iPt + 1,fTempFit->GetParError(2));
    if(debugOutputLevel) cerr<<"Debug in ExtractSignalTemplate; LINE: "<<__LINE__<<endl;
  }

}

TH1D* SubtractBackground(TH1D* hGG, TH1D* hBack, TH1D* hTruePi0, TH1D* hTrueEta){
  if(debugOutputLevel) cerr<<"Debug in ExtractSignalTemplate; LINE: "<<__LINE__<<endl;

  TH1D* hSignalSub = (TH1D*) hGG->Clone("hSignal");
  hSignalSub->Add(hBack, -1);
  if(fPrefix.CompareTo("Eta") == 0)hSignalSub->Add(hTruePi0, -1);
  if(fPrefix.Contains("Pi0"))hSignalSub->Add(hTrueEta, -1);
  if(debugOutputLevel) cerr<<"Debug in ExtractSignalTemplate; LINE: "<<__LINE__<<endl;
  return hSignalSub;

}




void Integrate(Int_t iPt, Int_t mode){
  if(debugOutputLevel) cerr<<"Debug in ExtractSignalTemplate; LINE: "<<__LINE__<<endl;



  if(!fdoLeft){
    fMesonCurIntRange[0][0]         = fMesonMass[iPt] + fMesonIntDeltaRange[0];
    fMesonCurIntRange[1][0]         = fMesonMass[iPt] + fMesonIntDeltaRangeWide[0];
    fMesonCurIntRange[2][0]         = fMesonMass[iPt] + fMesonIntDeltaRangeNarrow[0];
    fMesonCurIntRange[0][1]         = fMesonMass[iPt] + fMesonIntDeltaRange[1];
    fMesonCurIntRange[1][1]         = fMesonMass[iPt] + fMesonIntDeltaRangeWide[1];
    fMesonCurIntRange[2][1]         = fMesonMass[iPt] + fMesonIntDeltaRangeNarrow[1];
  } else {
    fMesonCurIntRange[3][0]         = fMesonMassLeft[iPt] + fMesonIntDeltaRange[0];
    fMesonCurIntRange[4][0]         = fMesonMassLeft[iPt] + fMesonIntDeltaRangeWide[0];
    fMesonCurIntRange[5][0]         = fMesonMassLeft[iPt] + fMesonIntDeltaRangeNarrow[0];
    fMesonCurIntRange[3][1]         = fMesonMassLeft[iPt] + fMesonIntDeltaRange[1];
    fMesonCurIntRange[4][1]         = fMesonMassLeft[iPt] + fMesonIntDeltaRangeWide[1];
    fMesonCurIntRange[5][1]         = fMesonMassLeft[iPt] + fMesonIntDeltaRangeNarrow[1];
  }

  if(debugOutputLevel) cerr<<"Debug in ExtractSignalTemplate; LINE: "<<__LINE__<<endl;

  //
  //************************************* Left Side Integration ******************************************
  if(fdoLeft){
    for(int k = 3; k < 6 ; k++){   // loop over diff. Integration ranges
      IntegrateHistoInvMass( fHistoMappingSignalInvMassLeftPtBin[iPt], fMesonCurIntRange[k]);
      // TGraph gr(fHistoMappingSignalInvMassLeftPtBin[iPt]);
      // gr.Print();
      cout<<"fYields left: "<<fYields<<endl;
      fMesonYields[k][iPt] = fYields;
      fMesonYieldsError[k][iPt] = fYieldsError;
    }
  //
  //************************************* Right Side Integration ******************************************
  } else {
    for(int k = 0; k < 3 ; k++){   // loop over diff. Integration ranges
      IntegrateHistoInvMass( fHistoMappingSignalInvMassPtBin[iPt], fMesonCurIntRange[k]);
      cout<<"fYields right: "<<fYields<<endl;
      fMesonYields[k][iPt] = fYields;
      fMesonYieldsError[k][iPt] = fYieldsError;
    }
  }
}


//  //****************************************************************************
//  //******* Fill array of invariant mass histograms in pT slices ***************
//  //****************************************************************************
 void FillMassHistosArrayTemp() {

     // same evt.
     cerr<<"Filling same evt. InvMass histos..."<<endl;
     for(Int_t iPt=fStartPtBin;iPt<fNBinsPt;iPt++){
         fNameHistoGG    = Form("Mapping_GG_InvMass_in_Pt_Bin%02d", iPt);
         CheckForNULLForPointer(fHistoMappingGGInvMassPtBin[iPt]);
         fHistoMappingGGInvMassPtBin[iPt]=  FillProjectionX(fGammaGammaInvMassVSPt, fNameHistoGG, fBinsPt[iPt], fBinsPt[iPt+1], fNRebin[iPt]);
      }
     // Background
     // cerr<<"Filling background InvMass histos..."<<endl;
     for(Int_t iPt=fStartPtBin;iPt<fNBinsPt;iPt++){
         fNameHistoGG    = Form("Mapping_Back_InvMass_in_Pt_Bin%02d", iPt);
         CheckForNULLForPointer(fHistoMappingBackInvMassPtBin[iPt]);
         fHistoMappingBackInvMassPtBin[iPt]=  FillProjectionX(fBckInvMassVSPt, fNameHistoGG, fBinsPt[iPt], fBinsPt[iPt+1], fNRebin[iPt]);
         // For normailzed background
         fNameHistoGG    = Form("Mapping_Back_Norm_InvMass_in_Pt_Bin%02d", iPt);
         CheckForNULLForPointer(fHistoMappingBackNormInvMassPtBin[iPt]);
         fHistoMappingBackNormInvMassPtBin[iPt]=  FillProjectionX(fBckInvMassVSPt, fNameHistoGG, fBinsPt[iPt], fBinsPt[iPt+1], fNRebin[iPt]);
         // For normailzed background left/alternative range
         fNameHistoGG    = Form("Mapping_Back_Norm_Left_InvMass_in_Pt_Bin%02d", iPt);
         CheckForNULLForPointer(fHistoMappingBackNormInvMassLeftPtBin[iPt]);
         fHistoMappingBackNormInvMassLeftPtBin[iPt]=  FillProjectionX(fBckInvMassVSPt, fNameHistoGG, fBinsPt[iPt], fBinsPt[iPt+1], fNRebin[iPt]);
     }

     // true pi0s
     cerr<<"Filling true pi0 InvMass histos..."<<endl;
     for(Int_t iPt=fStartPtBin;iPt<fNBinsPt;iPt++){
         fNameHistoGG    = Form("Mapping_TruePi0_InvMass_in_Pt_Bin%02d", iPt);
         CheckForNULLForPointer(fHistoMappingTruePi0InvMassPtBins[iPt]);
         fHistoMappingTruePi0InvMassPtBins[iPt]=  FillProjectionX(fTruePi0InvMassVSPt, fNameHistoGG, fBinsPt[iPt], fBinsPt[iPt+1], fNRebin[iPt]);
         fNameHistoGG    = Form("Mapping_TruePi0_Scaled_InvMass_in_Pt_Bin%02d", iPt);
         CheckForNULLForPointer(fHistoMappingTruePi0ScaledInvMassPtBins[iPt]);
         fHistoMappingTruePi0ScaledInvMassPtBins[iPt]=  FillProjectionX(fTruePi0InvMassVSPt, fNameHistoGG, fBinsPt[iPt], fBinsPt[iPt+1], fNRebin[iPt]);
         fNameHistoGG    = Form("Mapping_TruePi0_Left_InvMass_in_Pt_Bin%02d", iPt);
         CheckForNULLForPointer(fHistoMappingTruePi0InvMassLeftPtBins[iPt]);
         fHistoMappingTruePi0InvMassLeftPtBins[iPt]=  FillProjectionX(fTruePi0InvMassVSPt, fNameHistoGG, fBinsPt[iPt], fBinsPt[iPt+1], fNRebin[iPt]);
         fNameHistoGG    = Form("Mapping_TruePi0_Scaled_Left_InvMass_in_Pt_Bin%02d", iPt);
         CheckForNULLForPointer(fHistoMappingTruePi0ScaledInvMassLeftPtBins[iPt]);
         fHistoMappingTruePi0ScaledInvMassLeftPtBins[iPt]=  FillProjectionX(fTruePi0InvMassVSPt, fNameHistoGG, fBinsPt[iPt], fBinsPt[iPt+1], fNRebin[iPt]);
     }
     // true eta
     cerr<<"Filling true eta InvMass histos..."<<endl;
     for(Int_t iPt=fStartPtBin;iPt<fNBinsPt;iPt++){
         fNameHistoGG    = Form("Mapping_TrueEta_InvMass_in_Pt_Bin%02d", iPt);
         CheckForNULLForPointer(fHistoMappingTrueEtaInvMassPtBins[iPt]);
         fHistoMappingTrueEtaInvMassPtBins[iPt]=  FillProjectionX(fTrueEtaInvMassVSPt, fNameHistoGG, fBinsPt[iPt], fBinsPt[iPt+1], fNRebin[iPt]);
         fNameHistoGG    = Form("Mapping_TrueEta_Scaled_InvMass_in_Pt_Bin%02d", iPt);
         CheckForNULLForPointer(fHistoMappingTrueEtaScaledInvMassPtBins[iPt]);
         fHistoMappingTrueEtaScaledInvMassPtBins[iPt]=  FillProjectionX(fTrueEtaInvMassVSPt, fNameHistoGG, fBinsPt[iPt], fBinsPt[iPt+1], fNRebin[iPt]);
         fNameHistoGG    = Form("Mapping_TrueEta_Left_InvMass_in_Pt_Bin%02d", iPt);
         CheckForNULLForPointer(fHistoMappingTrueEtaInvMassLeftPtBins[iPt]);
         fHistoMappingTrueEtaInvMassLeftPtBins[iPt]=  FillProjectionX(fTrueEtaInvMassVSPt, fNameHistoGG, fBinsPt[iPt], fBinsPt[iPt+1], fNRebin[iPt]);
         fNameHistoGG    = Form("Mapping_TrueEta_Scaled_Left_InvMass_in_Pt_Bin%02d", iPt);
         CheckForNULLForPointer(fHistoMappingTrueEtaScaledInvMassLeftPtBins[iPt]);
         fHistoMappingTrueEtaScaledInvMassLeftPtBins[iPt]=  FillProjectionX(fTrueEtaInvMassVSPt, fNameHistoGG, fBinsPt[iPt], fBinsPt[iPt+1], fNRebin[iPt]);
     }

     cerr<<"Filling true background InvMass histos..."<<endl;
     for(Int_t iPt=fStartPtBin;iPt<fNBinsPt;iPt++){
         fNameHistoGG    = Form("Mapping_TrueBack_InvMass_in_Pt_Bin%02d", iPt);
         CheckForNULLForPointer(fHistoMappingTrueBackgroundInvMassPtBins[iPt]);
         fHistoMappingTrueBackgroundInvMassPtBins[iPt]=  FillProjectionX(fTrueBckInvMassVSPt, fNameHistoGG, fBinsPt[iPt], fBinsPt[iPt+1], fNRebin[iPt]);
     }

     cout<<"done"<<endl;
 }


 void InitializeTemplates(TString setPi0, Int_t numberOfBins, Int_t triggerSet){

   cout << "meson in intialize function: " <<  setPi0.Data() << "; numberOfBins: " << numberOfBins << "; triggerSet: " << triggerSet << endl;
   InitializeBinning(setPi0, numberOfBins, fEnergyFlag, fDirectPhoton, fMode, fEventCutSelection, fClusterCutSelection, triggerSet, kFALSE, "", "", fGammaCutSelection, fDoJetAnalysis);

   TString trigger         = fEventCutSelection(GetEventSelectSpecialTriggerCutPosition(),2);
   // Bool_t isRotationBG     = GetMesonBGSchemeIsRotation(fMesonCutSelection(GetMesonBGSchemeCutPosition(),1));
   InitializeWindows(setPi0, fMode, trigger, triggerSet, "template");

   cout<<"fNBinsPt: "<<fNBinsPt<<endl;
   // fHistoMappingGGInvMassPtBin                                     = new TH1D*[fNBinsPt];
   // fHistoMappingBackInvMassPtBin                                   = new TH1D*[fNBinsPt];
   fHistoMappingBackNormInvMassPtBin                               = new TH1D*[fNBinsPt];
   fHistoMappingTruePi0InvMassPtBins                               = new TH1D*[fNBinsPt];
   fHistoMappingTruePi0ScaledInvMassPtBins                         = new TH1D*[fNBinsPt];
   fHistoMappingTrueEtaInvMassPtBins                               = new TH1D*[fNBinsPt];
   fHistoMappingTrueEtaScaledInvMassPtBins                         = new TH1D*[fNBinsPt];
   fHistoMappingSignalInvMassPtBin                                 = new TH1D*[fNBinsPt];
   fHistoMappingBackNormInvMassLeftPtBin                           = new TH1D*[fNBinsPt];
   fHistoMappingTruePi0InvMassLeftPtBins                           = new TH1D*[fNBinsPt];
   fHistoMappingTruePi0ScaledInvMassLeftPtBins                     = new TH1D*[fNBinsPt];
   fHistoMappingTrueEtaInvMassLeftPtBins                           = new TH1D*[fNBinsPt];
   fHistoMappingTrueEtaScaledInvMassLeftPtBins                     = new TH1D*[fNBinsPt];
   fHistoMappingSignalInvMassLeftPtBin                             = new TH1D*[fNBinsPt];
   fHistoMappingTrueBackgroundInvMassPtBins                        = new TH1D*[fNBinsPt];

   for(Int_t i = 0;i<fNBinsPt; i++){
     // fHistoMappingGGInvMassPtBin[i]                               = nullptr;
     // fHistoMappingBackInvMassPtBin[i]                             = nullptr;
     fHistoMappingBackNormInvMassPtBin[i]                         = nullptr;
     fHistoMappingTruePi0InvMassPtBins[i]                         = nullptr;
     fHistoMappingTruePi0ScaledInvMassPtBins[i]                   = nullptr;
     fHistoMappingTrueEtaInvMassPtBins[i]                         = nullptr;
     fHistoMappingTrueEtaScaledInvMassPtBins[i]                   = nullptr;
     fHistoMappingSignalInvMassPtBin[i]                           = nullptr;

     fHistoMappingBackNormInvMassLeftPtBin[i]                     = nullptr;
     fHistoMappingTruePi0InvMassLeftPtBins[i]                     = nullptr;
     fHistoMappingTruePi0ScaledInvMassLeftPtBins[i]               = nullptr;
     fHistoMappingTrueEtaInvMassLeftPtBins[i]                     = nullptr;
     fHistoMappingTrueEtaScaledInvMassLeftPtBins[i]               = nullptr;
     fHistoMappingSignalInvMassLeftPtBin[i]                       = nullptr;
     fHistoMappingTrueBackgroundInvMassPtBins[i]                  = nullptr;
   }
}
