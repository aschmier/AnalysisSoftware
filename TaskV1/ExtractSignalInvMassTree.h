//****************************************************************************
//********* provided by Gamma Conversion Group *******************************
//********* $ALICE_ROOT/PWGGA/GammaConv **************************************
//***** https://twiki.cern.ch/twiki/bin/view/ALICE/PWG4GammaConversion *******
//****************************************************************************

//****************************************************************************
//******************** Global variable for setup of macro ********************
//****************************************************************************
Float_t     fCutTheta                                                   = 0.0202;
TDatime     now;
fstream     fFileErrLog;
fstream     fFileDataLog;
TString     fTextCent                                                   = "";
TString     fEnergyFlag                                                 = "";
TString     fPrefix                                                     = "";
TString     fPrefix2                                                    = "";
TString     fPeriodFlag                                                 = "";
TString     ftextDayth                                                  = "";
TString     fdate                                                       = "";
TString     fCollisionSystem                                            = "";
TString     fTextMeasurement                                            = "";
TString     fDetectionProcess                                           = "";
TString     fCutSelection                                               = "";
TString     fBackgroundMultCutNumber                                    = "";
Int_t       fMode                                                       = -1;
TString     fDirectPhoton                                               = "";
TString     fThesis                                                     = "";
Bool_t      fAdvancedMesonQA                                            = kFALSE;
Bool_t      fEstimateTrainPileUp                                        = kFALSE;
Bool_t      fEnableDCMeson                                              = kFALSE;
Bool_t      fEnableDCCluster                                            = kFALSE;
Bool_t      fUseRPBackground                                            = kFALSE;
Bool_t      fNewMCOutput                                                = kFALSE;

TString     fEventCutSelection                                          = "";
TString     fGammaCutSelection                                          = "";
TString     fClusterCutSelection                                        = "";
TString     fElectronCutSelection                                       = "";
TString     fMesonCutSelection                                          = "";

Int_t       fCrysFitting                                                = 0;
Int_t       fIsMC                                                       = 0;
Int_t       fMesonId                                                    = 0;
Float_t     fBackgroundMultNumber                                       = 0.;
Double_t    fYMaxMeson                                                  = 0.9;
Double_t    fMesonMassExpect                                            = 0;   // expected meson mass
Double_t    fNEvents                                                    = 0;
Double_t*   fPeakRange                                                  = NULL;
Double_t*   fFitRange                                                   = NULL;
Double_t*   fBGFitRange                                                 = NULL;
Double_t*   fMesonIntDeltaRange                                         = NULL;
TString     fcMonth[12]                                                 = {"Jan","Feb","Mar","Apr","May","Jun",
                                                                           "Jul","Aug","Sep","Oct","Nov","Dec"};
TString     centralityString                                            = "";
TH1D*       fCopySignal                                                 = NULL;
TH1D*       fCopyOnlyBG                                                 = NULL;
Double_t    fScalingFactorBck[7][4];
Double_t    fScalingFactorBckFullPt;
Double_t    fScalingFactorBckMidPt;
Double_t    fCBAlpha                                                    = 0;
Double_t    fCBn                                                        = 0;
Float_t     pictDrawingCoordinatesFWHM[9]                               = {0.6, 0.8, 0.30, 0.04, 0.15,0.7, 0.1, 0.035,0};
Int_t       fNRebinGlobal                                               = 2;
TString     nameSecondaries[4]                                          = {"K0S", "Lambda", "K0L", "Rest"};
TString     nameSecondariesCocktail[4]                                  = {"K0s", "Lambda", "K0l", "Rest"};
TString     nameIntRange[6]                                             = {"", "Wide", "Narrow", "Left", "LeftWide", "LeftNarrow"};

//****************************************************************************
//******************************** Output files ******************************
//****************************************************************************
TFile*      fOutput                                                     = NULL;
TFile*      fOutput1                                                    = NULL;
TFile*      fOutput2                                                    = NULL;

//****************************************************************************
//******************************** Object names ******************************
//****************************************************************************
TString     ObjectNameTrue                                              = "";
TString     ObjectNameTrueFull                                          = "";
TString     ObjectNameTrueWOWeights                                     = "";
TString     ObjectNameProfileWeights                                    = "";
TString     ObjectNameTrueSec                                           = "";
TString     ObjectNameTrueSecFromK0S                                    = "";
TString     ObjectNameTrueSecFromK0L                                    = "";
TString     ObjectNameTrueSecFromLambda                                 = "";
TString     ObjectNameMCPi0Acc                                          = "";
TString     ObjectNameMCPi0AccWOWeights                                 = "";
TString     ObjectNameMCPi0AccWOEvtWeights                              = "";
TString     ObjectNameMCEtaAcc                                          = "";
TString     ObjectNameMCEtaAccWOWeights                                 = "";
TString     ObjectNameMCEtaAccWOEvtWeights                              = "";
TString     ObjectNameMCPi0                                             = "";
TString     ObjectNameMCPi0WOWeights                                    = "";
TString     ObjectNameMCPi0WOEvtWeights                                 = "";
TString     ObjectNameMCEta                                             = "";
TString     ObjectNameMCEtaWOWeights                                    = "";
TString     ObjectNameMCEtaWOEvtWeights                                 = "";
TString     ObjectNameTrueGGBck                                         = "";
TString     ObjectNameTrueContBck                                       = "";
TString     ObjectNameTrueAllBck                                        = "";
TString     ObjectNameTrueCaloPhoton                                    = "";
TString     ObjectNameTrueCaloConvPhoton                                = "";
TString     ObjectNameTrueCaloMerged                                    = "";
TString     ObjectNameTrueMixedCaloConvPhoton                           = "";
TString     ObjectNameTrueCaloMergedPartConv                            = "";
TString     ObjectNameK0sRecPi0                                         = "";
TString     ObjectNameLambdaRecPi0                                      = "";
TString     ObjectNameDCMesonInvMassPt                                  = "";
TString     ObjectNameDCGammaClusPt                                     = "";
TString     ObjectNameMesonMultipleCount                                = "";
TString     ObjectNameGammaClusMultipleCount                            = "";
// object names for secondary MC histos
TString     ObjectNameMCSecPi0Acc                                       = "";
TString     ObjectNameMCSecPi0                                          = "";

TString     fNameHistoGG                                                = "";
TString     fNameHistoBack                                              = "";
TString     fNameHistoPP                                                = "";
TString     fNameHistoBackNormOut                                       = "";
TString     fNameHistoBackNormLeftOut                                   = "";
TString     fNameHistoTrue                                              = "";
TString     fNameHistoTrueGGBck                                         = "";
TString     fNameHistoTrueContBck                                       = "";
TString     fNameHistoTrueAllBck                                        = "";
TString     fNameHistoTrueSec                                           = "";
TString     fNameHistoEffi                                              = "";
TString     fNameHistoFrac                                              = "";
TString     fNameHistoMotherZM                                          = "";
TString     fNameHistoBckZM                                             = "";
TString     fNameFitSignalPos                                           = "";

//****************************************************************************
//************************ global variable ranges ****************************
//****************************************************************************
Double_t*    fBGFitRangeLeft                                            = NULL;
Double_t*    fMesonPlotRange                                            = NULL;
Double_t*    fIntFixedRange                                             = NULL;
Double_t*    fMesonIntDeltaRangeWide                                    = NULL;
Double_t*    fMesonIntDeltaRangeNarrow                                  = NULL;
Double_t*    fMesonCurIntRange[6]                                       = {NULL, NULL, NULL, NULL, NULL, NULL};
Double_t*    fMesonTrueIntRange[3]                                      = {NULL, NULL, NULL};
Double_t*    fMesonTrueIntReweightedRange[3]                            = {NULL, NULL, NULL};
Double_t*    fMesonTrueIntUnweightedRange[3]                            = {NULL, NULL, NULL};
Double_t*    fMesonMassRange                                            = NULL;
Double_t*    fMesonMassPlotRange                                        = NULL;
Double_t*    fMesonFitRange                                             = NULL;
Double_t*    fMesonFitRangeWithoutPeak                                  = NULL;
Double_t     fMesonWidthExpect                                          = 0;
Double_t     fMesonWidthExpectMC                                        = 0;
Double_t     fMesonLambdaTail                                           = 0;
Double_t     fMesonLambdaTailMC                                         = 0;
Double_t*    fMesonWidthRange                                           = NULL;
Double_t*    fMesonWidthRangeMC                                         = NULL;
Double_t*    fMesonLambdaTailRange                                      = NULL;
Double_t*    fMidPt                                                     = NULL;
Double_t*    fFullPt                                                    = NULL;

//****************************************************************************
//************************ correction histograms *****************************
//****************************************************************************
TH1D*     fDeltaPt                                                        = NULL;

//****************************************************************************
//******************************** Functions *********************************
//****************************************************************************
void ProcessEM(TH1D*,TH1D*,Double_t *);                                                                     // Normalization of Signal and BG
void ProcessRatioSignalBackground(TH1D* , TH1D* );                                                          // Calculate Ratio of Signal and BG in momentum slices
void FillMassHistosArray(TH2D*);                                                                            // Fill invariant mass histograms for Signal and Backgrounf
void FillThetaHistosArray(TH2D*);                                                                            // Fill invariant mass histograms for Signal and Backgrounf
void FillAlphaHistosArray(TH2D*);                                                                            // Fill invariant mass histograms for Signal and Backgrounf
void FillInvMassThetaHistosArray(TH3D*);                                                                            // Fill invariant mass histograms for Signal and Backgrounf
void FillInvMassAlphaHistosArray(TH3D*);                                                                            // Fill invariant mass histograms for Signal and Backgrounf
void FillThetaBckHistosArray(TH2D*);                                                                            // Fill invariant mass histograms for Signal and Backgrounf
void FillAlphaBckHistosArray(TH2D*);                                                                            // Fill invariant mass histograms for Signal and Backgrounf
TH1D* FillProjectionX (TH2*, TString, Double_t, Double_t, Int_t);                                           // Fill Projection in according to Y bins
TH2D* FillProjectionYX (TH3*, TString, Double_t, Double_t);                                           // Fill Projection in according to Y bins
void FillMassMCTrueMesonHistosArray(TH2D*);                                                                 // Fill invariant mass histograms for validated mesons
void FillMassMCTrueFullMesonHistosArray(TH2D* );                                                            // Fill invariant mass histograms for validated mesons full momentum range
void FillMassMCTrueMesonDCHistosArray(TH2D*);                                                               // Fill invariant mass histograms for double counted validated mesons 
void FillMassMCTrueMesonCaloPhotonHistosArray(TH2D*);                                                       // Fill invariant mass histograms for validated mesons true calo photons
void FillMassMCTrueMesonCaloConvPhotonHistosArray(TH2D*);                                                   // Fill invariant mass histograms for validated mesons true calo conv photons
void FillMassMCTrueMesonCaloElectronHistosArray(TH2D*);                                                     // Fill invariant mass histograms for validated mesons true calo electrons
void FillMassMCTrueMesonMixedCaloConvPhotonHistosArray(TH2D*);                                              // Fill invariant mass histograms for validated mesons true calo photons, conv photons
void FillMassMCTrueMesonCaloMergedClusterHistosArray(TH2D*);                                                // Fill invariant mass histograms for validated mesons true calo merged 
void FillMassMCTrueMesonCaloMergedClusterPartConvHistosArray(TH2D*);                                        // Fill invariant mass histograms for validated mesons true calo merged part conv
void FillMassMCTrueReweightedMesonHistosArray(TH2D*);                                                       // Fill invariant mass histograms for validated mesons reweighted
void FillMassMCTrueUnweightedMesonHistosArray(TH2D*);                                                       // Fill invariant mass histograms for validated mesons unweighted
void FillMassMCTrueGGBckHistosArray(TH2D*);                                                                 // Fill invariant mass histograms for validated gamma gamma BG
void FillMassMCTrueContBckHistosArray(TH2D*);                                                               // Fill invariant mass histograms for validated contamination
void FillMassMCTrueAllBckHistosArray(TH2D*);                                                                // Fill invariant mass histograms for validated BG
void FillMassMCTrueSecMesonHistosArray(TH2D**);                                                             // Fill invariant mass histograms for validated secondaries
TH1D* CalculateSecondaryFractions(TH1D* histoRawYield, TH1D* histoRawYieldSec, TString nameHistoFrac);      // Calculate fraction of secondaries
void CreatePtHistos();                                                                                      // Creat pt dependent histograms
void FillPtHistos();                                                                                        // Fill pt dependent histograms
void FitSubtractedInvMassInPtBins(TH1D * ,Double_t *, Int_t, Bool_t );                                      // Fits the invariant mass histos with a gaussian plus exponential plus lin BG
void FitSubtractedPol2InvMassInPtBins(TH1D*, Double_t*, Int_t, Bool_t );                                    // Fits the invariant mass histos with a gaussian plus exponential plus pol2 BG
void FitSubtractedExp1InvMassInPtBins(TH1D* , Double_t* , Int_t , Bool_t );                                 // Fits the invariant mass histos with a gaussian plus exponential plus exp BG
void FitSubtractedExp2InvMassInPtBins(TH1D* , Double_t* , Int_t , Bool_t );                                 // Fits the invariant mass histos with a gaussian plus exponential plus exp BG
void FitSubtractedPureGaussianInvMassInPtBins(TH1D*, Int_t);                                                // Fits the invariant mass histos with a pure gaussian plus lin BG
void FitTrueInvMassInPtBins(TH1D * ,Double_t *, Int_t, Bool_t);                                             // Fits the true invariant mass histos with a gaussian plus exponential plus lin BG
void FitTrueInvMassPureGaussianInPtBins(TH1D * , Int_t);                                                    // Fits the true invariant mass histos with a gaussian plus lin BG
void FitCBSubtractedInvMassInPtBins(TH1D* ,Double_t * , Int_t ,Bool_t,TString, Bool_t );                    // Fits the invariant mass histos with a CB function
void ProduceBckProperWeighting(TList*, TList*,Bool_t );                                                     // Create BG with proper weighting
void ProduceBckWithoutWeighting(TH2D *);                                                                    // Create BG without proper weighting
void IntegrateHistoInvMassStream(TH1D * , Double_t *);                                                      // Integrate invariant mass histogram with output to ifstream
void IntegrateHistoInvMass(TH1D * , Double_t *);                                                            // Integrate invariant mass histogram
void IntegrateFitFunc(TF1 * , TH1D *, Double_t *);                                                          // Integrate fit function
void FillHistosArrayMC(TH1D* , TH1D*, TH1D*);                                                               // Fill MC input histograms
void FillHistosArrayMCWOWeights(TH1D* , TH1D*, TH1D*);                                                      // Fill MC input histograms
void FillHistosArrayMCWOEvtWeights(TH1D* , TH1D*, TH1D*);                                                   // Fill MC input histograms
void CalculateMesonAcceptance();                                                                            // Calculation of meson acceptance
void CalculateMesonAcceptanceWOWeights();                                                                   // Calculation of meson acceptance
void CalculateMesonAcceptanceWOEvtWeights();                                                                // Calculation of meson acceptance
TH1D* CalculateMesonEfficiency(TH1D*, TH1D**, TH1D*, TString);                                              // Calculation of meson efficiencies 
void SaveHistos(Int_t, TString, TString, Bool_t);                                                           // Saving standard histograms to a file
void SaveCorrectionHistos(TString , TString);                                                               // Saving correction histograms to a file
void Initialize(TString setPi0, Int_t, Int_t);                                                              // Initialization of global variables depending on meson analysed 
void CalculateFWHM(TF1 *);                                                                                  // Calculation of FWHM
Double_t CrystalBallBck(Double_t *,Double_t *);                                                             // Definition of CrystalBall with linear BG
Double_t LinearBackground(Double_t *,Double_t *);                                                           // Definition of linear BG
Double_t LinearBGExclusion(Double_t *,Double_t *);                                                          // Definition of linear BG with excluded region
Double_t LinearBGExclusionnew(Double_t *,Double_t *);                                                       // Definition of linear BG with excluded region
Double_t CrystalBall(Double_t *,Double_t *);                                                                // Definition of CrystalBall
void Delete();                                                                                              // Deleting all pointers
void SetCorrectMCHistogrammNames(TString);                                                                  // Setting correct histogram names
void FillMCSecondaryHistAndCalculateAcceptance(TH2D*, TH2D*);                                               // Fill secondary MC input histograms and calculate respective acceptance            
Bool_t LoadSecondaryPionsFromExternalFile();                                                                // Loads secondary neutral pion input graphs from file
Bool_t LoadSecondaryPionsFromCocktailFile(TString, TString);                                                // Loads secondary neutral pion input graphs from file

//****************************************************************************
//************************** input histograms ********************************
//****************************************************************************
TH2D*       fHistoTrueMesonInvMassVSPt                                  = NULL;
TH2D*       fHistoTrueFullMesonInvMassVSPt                              = NULL;
TH2D*       fHistoTrueMesonInvMassVSPtWOWeights                         = NULL;
TH2D*       fHistoTrueMesonInvMassVSPtReweighted                        = NULL;
TH2D*       fHistoTrueMesonInvMassVSPtUnweighted                        = NULL;
TProfile2D* fProfileTrueMesonInvMassVSPtWeights                         = NULL;
TH2D*       fHistoTrueMesonInvMassVSPtSec                               = NULL;
TH2D*       fHistoTrueGGBckInvMassVSPt                                  = NULL;
TH2D*       fHistoTrueContBckInvMassVSPt                                = NULL;
TH2D*       fHistoTrueAllBckInvMassVSPt                                 = NULL;
TH2D*       fHistoTrueSecMesonInvMassVSPt[4]                            = { NULL, NULL, NULL, NULL };
TH2D*       fHistoTrueMesonCaloPhotonInvMassVSPt                        = NULL;
TH2D*       fHistoTrueMesonCaloConvPhotonInvMassVSPt                    = NULL;
TH2D*       fHistoTrueMesonCaloElectronInvMassVSPt                      = NULL;
TH2D*       fHistoTrueMesonMergedClusterInvMassVSPt                     = NULL;
TH2D*       fHistoTrueMesonMergedClusterPartConvInvMassVSPt             = NULL;
TH2D*       fHistoTrueMesonMixedCaloConvPhotonInvMassVSPt               = NULL;
TH2D*       fGammaGammaInvMassVSPt                                      = NULL;
TH2D*       fBckInvMassVSPt                                             = NULL;
TH2D*       fGammaGammaAlphaVSPt                                        = NULL;
TH3D*       fGammaGammaInvMassVSAlpha                                   = NULL;
TH2D*       fGammaGammaThetaVSPt                                        = NULL;
TH3D*       fGammaGammaInvMassVSTheta                                   = NULL;
TH2D*       fBckAlphaVSPt                                               = NULL;
TH2D*       fBckThetaVSPt                                               = NULL;
TH1D*       fNumberOfGoodESDTracks                                      = NULL;
TH1D*       fEventQuality                                               = NULL;
TH1D*       fHistoYieldK0sWithPi0DaughterRec                            = NULL;
TH1D*       fHistoYieldLambdaWithPi0DaughterRec                         = NULL;
TH2D*       fPi0ResponseMatrix                                          = NULL;
TH2D*       fPi0ResponseMatrixRebin                                     = NULL;
TH2D*       fHistoMotherZM                                              = NULL;
TH2D*       fHistoBckZM                                                 = NULL;

//****************************************************************************
//************************** background histograms ***************************
//****************************************************************************
TH2F**      fHistoWeightsBGZbinVsMbin                                   = 0x0;
TH2F**      fHistoFillPerEventBGZbinVsMbin                              = 0x0;
TH2F**      fHistoWeightsBGZbinVsPsibin                                 = 0x0;
TH2F**      fHistoFillPerEventBGZbinVsPsibin                            = 0x0;

//****************************************************************************
//************************ sample histograms for inv Mass ********************
//****************************************************************************
TH1D*       fBckNorm                                                    = NULL;
TH1D*       fSignal                                                     = NULL;
TH1D*       fRatioSB                                                    = NULL;
TH1D*       fFittingHistMidPtSignal                                     = NULL;
TH1D*       fFittingHistMidPtBackground                                 = NULL;
TH1D*       fFittingHistMidPtSignalSub                                  = NULL;
TH1D*       fMesonFullPtSignal                                          = NULL;
TH1D*       fMesonFullPtBackground                                      = NULL;
TH1D*       fMesonFullPtBackNorm                                        = NULL;
TF1 *       fFitSignalInvMassMidPt                                      = NULL;
TF1 *       fFitSignalInvMassMidPt2                                     = NULL;

//****************************************************************************
//************************ inv mass histograms in pT bins ********************
//****************************************************************************
TH1D*       fHistoMotherZMProj                                          = NULL;
TH1D*       fHistoBckZMProj                                             = NULL;
TH1D*       fHistoMotherZMProjFullPt                                    = NULL;
TH1D*       fHistoBckZMProjFullPt                                       = NULL;
TH1D*       fHistoMotherZMProjMidPt                                     = NULL;
TH1D*       fHistoBckZMProjMidPt                                        = NULL;
TH1D*       fHistoMotherZPsiProj                                        = NULL;
TH1D*       fHistoBckZPsiProj                                           = NULL;
TH1D*       fHistoMotherZPsiProjFullPt                                  = NULL;
TH1D*       fHistoBckZPsiProjFullPt                                     = NULL;
TH1D*       fHistoMotherZPsiProjMidPt                                   = NULL;
TH1D*       fHistoBckZPsiProjMidPt                                      = NULL;
TH1D**      fHistoMappingTrueMesonInvMassPtBins                         = NULL;  
TH1D**      fHistoMappingTrueFullMesonInvMassPtBins                     = NULL;  
TH1D**      fHistoMappingTrueMesonInvMassPtReweightedBins               = NULL;    
TH1D**      fHistoMappingTrueMesonInvMassPtUnweightedBins               = NULL;    
TH1D**      fHistoMappingTrueGGBckInvMassPtBins                         = NULL;   
TH1D**      fHistoMappingTrueContBckInvMassPtBins                       = NULL;    
TH1D**      fHistoMappingTrueAllBckInvMassPtBins                        = NULL;    
TH1D**      fHistoMappingTrueSecMesonInvMassPtBins[4]                   = {NULL, NULL, NULL, NULL};    
TH1D**      fHistoMappingGGInvMassPtBin                                 = NULL;    
TH1D**      fHistoMappingGGAlphaPtBin                                   = NULL;
TH1D**      fHistoMappingGGThetaPtBin                                   = NULL;
TH2D**      fHistoMappingGGInvMassAlphaPtBin                            = NULL;
TH2D**      fHistoMappingGGInvMassThetaPtBin                            = NULL;
TH1D**      fHistoMappingBackInvMassPtBin                               = NULL;
TH1D**      fHistoMappingBackAlphaPtBin                                 = NULL;
TH1D**      fHistoMappingBackThetaPtBin                                 = NULL;
TH1D**      fHistoMappingBackNormInvMassPtBin                           = NULL;
TH1D**      fHistoMappingBackNormAlphaPtBin                             = NULL;
TH1D**      fHistoMappingBackNormThetaPtBin                             = NULL;
TH1D**      fHistoMappingRatioSBInvMassPtBin                            = NULL;
TH1D**      fHistoMappingSignalInvMassPtBin                             = NULL;
TH1D**      fHistoMappingSignalAlphaPtBin                               = NULL;
TH1D**      fHistoMappingSignalThetaPtBin                               = NULL;
TH1D**      fHistoMappingRemainingBGInvMassPtBin                        = NULL;
TH1D**      fHistoMappingSignalRemainingBGSubInvMassPtBin               = NULL;
TH1D**      fHistoMappingBackNormInvMassLeftPtBin                       = NULL;
TH1D**      fHistoMappingSignalInvMassLeftPtBin                         = NULL;
TH1D**      fHistoMappingSignalRemainingBGSubInvMassLeftPtBin           = NULL;
TH1D**      fHistoMappingRemainingBGInvMassLeftPtBin                    = NULL;
TH1D**      fHistoMappingTrueMesonCaloPhotonInvMassPtBins               = NULL;    
TH1D**      fHistoMappingTrueMesonCaloConvPhotonInvMassPtBins           = NULL;    
TH1D**      fHistoMappingTrueMesonCaloElectronInvMassPtBins             = NULL;    
TH1D**      fHistoMappingTrueMesonCaloMergedClusterInvMassPtBins        = NULL;    
TH1D**      fHistoMappingTrueMesonCaloMergedClusterPartConvInvMassPtBins= NULL;    
TH1D**      fHistoMappingTrueMesonMixedCaloConvPhotonInvMassPtBins      = NULL;

//****************************************************************************
//**************************** global fit functions **************************
//****************************************************************************
TF1 *       fFitReco                                                    = NULL;
TF1 *       fFitGausExp                                                 = NULL;
TF1 *       fFitLinearBck                                               = NULL;
TF1 *       fFitLinearBckExcl                                           = NULL;
TF1 *       fFitLinearBckOut                                            = NULL;
Double_t    fYields;
Double_t    fYieldsError;
Double_t    fFWHMFunc;
Double_t    fFWHMFuncError;
Double_t    fYieldsFunc;
Double_t    fYieldsFuncError;
Double_t    fIntLinearBck;
Double_t    fIntLinearBckError;
Double_t    fIntLinearBckOut;
Double_t    fIntLinearBckErrorOut;

//****************************************************************************
//******************** yield extraction standard window **********************
//****************************************************************************
Double_t*   fGGYields[6]                                                = {NULL, NULL, NULL, NULL, NULL, NULL};
Double_t*   fBckYields[6]                                               = {NULL, NULL, NULL, NULL, NULL, NULL};
Double_t*   fTotalBckYields[6]                                          = {NULL, NULL, NULL, NULL, NULL, NULL};
Double_t*   fMesonYields[6]                                             = {NULL, NULL, NULL, NULL, NULL, NULL};
Double_t*   fGGYieldsError[6]                                           = {NULL, NULL, NULL, NULL, NULL, NULL};
Double_t*   fBckYieldsError[6]                                          = {NULL, NULL, NULL, NULL, NULL, NULL};
Double_t*   fTotalBckYieldsError[6]                                     = {NULL, NULL, NULL, NULL, NULL, NULL};
Double_t*   fMesonYieldsError[6]                                        = {NULL, NULL, NULL, NULL, NULL, NULL};
TH1D*       fHistoYieldMeson[6]                                         = {NULL, NULL, NULL, NULL, NULL, NULL};
TH1D*       fHistoYieldMesonPerEvent[6]                                 = {NULL, NULL, NULL, NULL, NULL, NULL};
TH1D*       fHistoYieldTrueMeson[6]                                     = {NULL, NULL, NULL, NULL, NULL, NULL};
Double_t*   fMesonYieldsFunc[6]                                         = {NULL, NULL, NULL, NULL, NULL, NULL};
Double_t*   fMesonYieldsResidualBckFunc[6]                              = {NULL, NULL, NULL, NULL, NULL, NULL};
Double_t*   fMesonYieldsCorResidualBckFunc[6]                           = {NULL, NULL, NULL, NULL, NULL, NULL};
Double_t*   fMesonYieldsPerEvent[6]                                     = {NULL, NULL, NULL, NULL, NULL, NULL};
Double_t*   fMesonYieldsFuncError[6]                                    = {NULL, NULL, NULL, NULL, NULL, NULL};
Double_t*   fMesonYieldsResidualBckFuncError[6]                         = {NULL, NULL, NULL, NULL, NULL, NULL};
Double_t*   fMesonYieldsCorResidualBckFuncError[6]                      = {NULL, NULL, NULL, NULL, NULL, NULL};
Double_t*   fMesonYieldsPerEventError[6]                                = {NULL, NULL, NULL, NULL, NULL, NULL};

Double_t*   fMesonTrueYields[3]                                         = {NULL, NULL, NULL};
Double_t*   fMesonTrueYieldsError[3]                                    = {NULL, NULL, NULL};
Double_t*   fMesonTrueYieldsReweighted[3]                               = {NULL, NULL, NULL};
Double_t*   fMesonTrueYieldsReweightedError[3]                          = {NULL, NULL, NULL};
Double_t*   fMesonTrueYieldsUnweighted[3]                               = {NULL, NULL, NULL};
Double_t*   fMesonTrueYieldsUnweightedError[3]                          = {NULL, NULL, NULL};
TH1D*       fHistoYieldTrueMesonReweighted[3]                           = {NULL, NULL, NULL};
TH1D*       fHistoYieldTrueMesonUnweighted[3]                           = {NULL, NULL, NULL};

Double_t*   fMesonYieldsResBckOtherFunc[3]                              = { NULL, NULL, NULL};
Double_t*   fMesonYieldsResBckOtherFuncError[3]                         = { NULL, NULL, NULL};

//****************************************************************************
//************* histos, fits, doubles for pure Standard fitting **************
//****************************************************************************
TF1**       fFitSignalInvMassPtBin                                      = NULL;
TF1**       fFitSignalWithOtherBGInvMassPtBin[3]                        = { NULL, NULL, NULL };
TF1**       fFitRemainingBGInvMassPtBin                                 = NULL;
TF1**       fFitBckInvMassPtBin                                         = NULL;
TF1**       fFitBckOtherInvMassPtBin[3]                                 = { NULL, NULL, NULL };
TF1**       fFitTrueSignalInvMassPtBin                                  = NULL;
TF1**       fFitTrueSignalInvMassPtReweightedBin                        = NULL;
TF1**       fFitTrueSignalInvMassPtUnweightedBin                        = NULL;
Double_t*   fMesonMass                                                  = NULL;
Double_t*   fMesonMassError                                             = NULL;
Double_t*   fMesonTrueMass                                              = NULL;
Double_t*   fMesonTrueMassError                                         = NULL;
Double_t*   fMesonFWHM                                                  = NULL;
Double_t*   fMesonFWHMError                                             = NULL;
Double_t*   fMesonTrueFWHM                                              = NULL;
Double_t*   fMesonTrueFWHMError                                         = NULL;
Double_t*   fMesonLambdaTailpar                                         = NULL;
Double_t*   fMesonLambdaTailparError                                    = NULL;
Double_t*   fMesonLambdaTailMCpar                                       = NULL;
Double_t*   fMesonLambdaTailMCparError                                  = NULL;
Double_t*   fMesonResidualBGlin                                         = NULL;
Double_t*   fMesonResidualBGlinError                                    = NULL;
Double_t*   fMesonResidualBGcon                                         = NULL;
Double_t*   fMesonResidualBGconError                                    = NULL;
Double_t*   fMesonChi2[4]                                               = {NULL, NULL, NULL, NULL};

TH1D*       fHistoMassMeson                                             = NULL;
TH1D*       fHistoFWHMMeson                                             = NULL;
TH1D*       fHistoTrueMassMeson                                         = NULL;
TH1D*       fHistoTrueFWHMMeson                                         = NULL;
Double_t*   fMesonTrueMassReweighted                                    = NULL;
Double_t*   fMesonTrueFWHMReweighted                                    = NULL;
Double_t*   fMesonTrueMassReweightedError                               = NULL;
Double_t*   fMesonTrueFWHMReweightedError                               = NULL;
TH1D*       fHistoTrueMassMesonReweighted                               = NULL;
TH1D*       fHistoTrueFWHMMesonReweighted                               = NULL;
Double_t*   fMesonTrueMassUnweighted                                    = NULL;
Double_t*   fMesonTrueFWHMUnweighted                                    = NULL;
Double_t*   fMesonTrueMassUnweightedError                               = NULL;
Double_t*   fMesonTrueFWHMUnweightedError                               = NULL;
TH1D*       fHistoTrueMassMesonUnweighted                               = NULL;
TH1D*       fHistoTrueFWHMMesonUnweighted                               = NULL;

//****************************************************************************
//********* histos, fits, doubles for pure Standard fitting left *************
//****************************************************************************
TF1**       fFitInvMassLeftPtBin                                        = NULL;
TF1**       fFitInvMassLeftPtBin2                                       = NULL;
TF1**       fFitRemainingBGInvMassLeftPtBin                             = NULL;
TF1**       fFitRemainingBGInvMassLeftPtBin2                            = NULL;
TF1**       fFitBckInvMassLeftPtBin                                     = NULL;
TF1**       fFitBckInvMassLeftPtBin2                                    = NULL;
Double_t*   fMesonMassLeft                                              = NULL;
Double_t*   fMesonFWHMLeft                                              = NULL;
Double_t*   fMesonMassLeftError                                         = NULL;
Double_t*   fMesonFWHMLeftError                                         = NULL;
TH1D*       fHistoMassMesonLeft                                         = NULL;
TH1D*       fHistoFWHMMesonLeft                                         = NULL;

//****************************************************************************
//************** histos, fits, doubles for pure Gaussian fitting *************
//****************************************************************************
TF1**       fFitSignalGaussianInvMassPtBin                              = NULL;
TF1**       fFitTrueSignalGaussianInvMassPtBin                          = NULL;
Double_t*   fMesonMassGaussian                                          = NULL;
Double_t*   fMesonMassGaussianError                                     = NULL;
Double_t*   fMesonTrueMassGaussian                                      = NULL;
Double_t*   fMesonTrueMassGaussianError                                 = NULL;
Double_t*   fMesonWidthGaussian                                         = NULL;
Double_t*   fMesonWidthGaussianError                                    = NULL;
Double_t*   fMesonTrueWidthGaussian                                     = NULL;
Double_t*   fMesonTrueWidthGaussianError                                = NULL;
TH1D*       fHistoMassGaussianMeson                                     = NULL;
TH1D*       fHistoTrueMassGaussianMeson                                 = NULL;
TH1D*       fHistoWidthGaussianMeson                                    = NULL;
TH1D*       fHistoTrueWidthGaussianMeson                                = NULL;

//****************************************************************************
//******************** Peak position and calibration *************************
//****************************************************************************
TF1**       fFitSignalPeakPosInvMassLeftPtBin                           = NULL;
TF1**       fFitSignalPeakPosInvMassPtBin                               = NULL;

//****************************************************************************
//**************** Decomposition of signal for calo clusters *****************
//****************************************************************************
TF1**       fFitTrueSignalCaloPhotonInvMassPtBin                        = NULL;
TF1**       fFitTrueSignalCaloConvPhotonInvMassPtBin                    = NULL;
TF1**       fFitTrueSignalCaloElectronInvMassPtBin                      = NULL;
TF1**       fFitTrueSignalMixedCaloConvPhotonInvMassPtBin               = NULL;
TF1**       fFitTrueSignalCaloMergedClusterInvMassPtBin                 = NULL;
TF1**       fFitTrueSignalCaloMergedClusterPartConvInvMassPtBin         = NULL;
Double_t*   fMesonTrueMassCaloPhoton                                    = NULL;
Double_t*   fMesonTrueMassCaloElectron                                  = NULL;
Double_t*   fMesonTrueMassCaloConvPhoton                                = NULL;
Double_t*   fMesonTrueMassCaloMergedCluster                             = NULL;
Double_t*   fMesonTrueMassCaloMergedClusterPartConv                     = NULL;
Double_t*   fMesonTrueMassMixedCaloConvPhoton                           = NULL;
Double_t*   fMesonTrueFWHMCaloPhoton                                    = NULL;
Double_t*   fMesonTrueFWHMCaloElectron                                  = NULL;
Double_t*   fMesonTrueFWHMCaloConvPhoton                                = NULL;
Double_t*   fMesonTrueFWHMCaloMergedCluster                             = NULL;
Double_t*   fMesonTrueFWHMCaloMergedClusterPartConv                     = NULL;
Double_t*   fMesonTrueFWHMMixedCaloConvPhoton                           = NULL;
Double_t*   fMesonTrueMassErrorCaloPhoton                               = NULL;
Double_t*   fMesonTrueMassErrorCaloElectron                             = NULL;
Double_t*   fMesonTrueMassErrorCaloConvPhoton                           = NULL;
Double_t*   fMesonTrueMassErrorCaloMergedCluster                        = NULL;
Double_t*   fMesonTrueMassErrorCaloMergedClusterPartConv                = NULL;
Double_t*   fMesonTrueMassErrorMixedCaloConvPhoton                      = NULL;
Double_t*   fMesonTrueFWHMErrorCaloPhoton                               = NULL;
Double_t*   fMesonTrueFWHMErrorCaloElectron                             = NULL;
Double_t*   fMesonTrueFWHMErrorCaloConvPhoton                           = NULL;
Double_t*   fMesonTrueFWHMErrorCaloMergedCluster                        = NULL;
Double_t*   fMesonTrueFWHMErrorCaloMergedClusterPartConv                = NULL;
Double_t*   fMesonTrueFWHMErrorMixedCaloConvPhoton                      = NULL;
TH1D*       fHistoTrueMassMesonCaloPhoton                               = NULL;
TH1D*       fHistoTrueMassMesonCaloElectron                             = NULL;
TH1D*       fHistoTrueMassMesonCaloConvPhoton                           = NULL;
TH1D*       fHistoTrueMassMesonCaloMergedCluster                        = NULL;
TH1D*       fHistoTrueMassMesonCaloMergedPartConvCluster                = NULL;
TH1D*       fHistoTrueMassMesonMixedCaloConvPhoton                      = NULL;
TH1D*       fHistoTrueFWHMMesonCaloPhoton                               = NULL;
TH1D*       fHistoTrueFWHMMesonCaloElectron                             = NULL;
TH1D*       fHistoTrueFWHMMesonCaloConvPhoton                           = NULL;
TH1D*       fHistoTrueFWHMMesonCaloMergedCluster                        = NULL;
TH1D*       fHistoTrueFWHMMesonCaloMergedPartConvCluster                = NULL;
TH1D*       fHistoTrueFWHMMesonMixedCaloConvPhoton                      = NULL;

//****************************************************************************
//****************** significance & S/B doubles, histograms ******************
//****************************************************************************
Double_t*   fMesonSBdefault[3]                                          = { NULL, NULL, NULL};
Double_t*   fMesonSigndefault[3]                                        = { NULL, NULL, NULL};
Double_t*   fMesonSBdefaultError[3]                                     = { NULL, NULL, NULL};
Double_t*   fMesonSigndefaultError[3]                                   = { NULL, NULL, NULL};
Double_t*   fMesonTrueSB                                                = NULL;
Double_t*   fMesonTrueSign                                              = NULL;
Double_t*   fMesonTrueSBError                                           = NULL;
Double_t*   fMesonTrueSignError                                         = NULL;
TH1D*       fHistoSBdefaultMeson[3]                                     = { NULL, NULL, NULL};
TH1D*       fHistoSigndefaultMeson[3]                                   = { NULL, NULL, NULL};
TH1D*       fHistoTrueSignMeson                                         = NULL;
TH1D*       fHistoTrueSBMeson                                           = NULL;
TH1D*       fHistoLambdaTail                                            = NULL;
TH1D*       fHistoResidualBGlin                                         = NULL;
TH1D*       fHistoResidualBGcon                                         = NULL;
TH1D*       fHistoChi2[4]                                               = { NULL, NULL, NULL, NULL };
TH1D*       fHistoRatioResBGYield                                       = NULL;
TH1D*       fHistoRatioResBGYieldToSPlusResBG                           = NULL;
TH1D*       fHistoResBGYield[4]                                         = { NULL, NULL, NULL, NULL };

Double_t*   fMassWindowHigh[3]                                          = { NULL, NULL, NULL};
Double_t*   fMassWindowLow[3]                                           = { NULL, NULL, NULL};
TH1D*       fHistoMassWindowHigh[3]                                     = { NULL, NULL, NULL};
TH1D*       fHistoMassWindowLow[3]                                      = { NULL, NULL, NULL};

//****************************************************************************
//**************************** MC input histograms ***************************
//****************************************************************************
TH1D*       fHistoMCMesonPtWithinAcceptance                             = NULL;
TH1D*       fHistoMCMesonPtWithinAcceptanceWOWeights                    = NULL;
TH1D*       fHistoMCMesonPtWithinAcceptanceWOEvtWeights                 = NULL;
TH1D*       fHistoMCMesonPt                                             = NULL;
TH1D*       fHistoMCMesonPtWOWeights                                    = NULL;
TH1D*       fHistoMCMesonPtWOEvtWeights                                 = NULL;
TH1D*       fHistoMCMesonPtWeights                                      = NULL;
TH1D*       fHistoMCMesonWithinAccepPt                                  = NULL;
TH1D*       fHistoMCMesonWithinAccepPtWOWeights                         = NULL;
TH1D*       fHistoMCMesonWithinAccepPtWOEvtWeights                      = NULL;
TH1D*       fHistoMCMesonPt1                                            = NULL;
TH1D*       fHistoMCMesonPt1WOWeights                                   = NULL;
TH1D*       fHistoMCMesonPt1WOEvtWeights                                = NULL;
//****************************************************************************
//************************ sec MC input histograms ***************************
//****************************************************************************
TH2D*       fHistoMCSecPi0SourcePt                                      = NULL;
TH2D*       fHistoMCSecPi0WAccSourcePt                                  = NULL;
TH1D*       fHistoMCSecPi0Pt[4]                                         = { NULL, NULL, NULL, NULL};
TH1D*       fHistoMCSecPi0PtWAcc[4]                                     = { NULL, NULL, NULL, NULL};
TH1D*       fHistoMCSecPi0PtReb[4]                                      = { NULL, NULL, NULL, NULL};
TH1D*       fHistoMCSecPi0PtWAccReb[4]                                  = { NULL, NULL, NULL, NULL};
TH1D*       fHistoMCSecPi0AcceptPt[4]                                   = { NULL, NULL, NULL, NULL};

//****************************************************************************
//*********************** MC efficiency histograms ***************************
//****************************************************************************
TH1D*       fHistoMCMesonAcceptPt                                       = NULL;
TH1D*       fHistoMCMesonAcceptPtWOWeights                              = NULL;
TH1D*       fHistoMCMesonAcceptPtWOEvtWeights                           = NULL;

TH1D*       fHistoMonteMesonEffiPt[6]                                   = {NULL, NULL, NULL, NULL, NULL, NULL};
TH1D*       fHistoMCTrueMesonEffiPt[3]                                  = {NULL, NULL, NULL};
TH1D*       fHistoMCTrueMesonEffiPtReweighted[3]                        = {NULL, NULL, NULL};
TH1D*       fHistoMCTrueMesonEffiPtUnweighted[3]                        = {NULL, NULL, NULL};
TH1D*       fHistoMCTrueSecMesonEffiPt[3][4]                            = {{NULL, NULL, NULL, NULL},{NULL, NULL, NULL, NULL},{NULL, NULL, NULL, NULL}};

//****************************************************************************
//**************************** MC rec sec mesons  ****************************
//****************************************************************************
Double_t*   fMesonTrueSecYields[3][4]                                   = {{NULL, NULL, NULL, NULL},{NULL, NULL, NULL, NULL},{NULL, NULL, NULL, NULL}};
Double_t*   fMesonTrueSecYieldsError[3][4]                              = {{NULL, NULL, NULL, NULL},{NULL, NULL, NULL, NULL},{NULL, NULL, NULL, NULL}};
TH1D*       fHistoYieldTrueSecMeson[3][4]                               = {{NULL, NULL, NULL, NULL},{NULL, NULL, NULL, NULL},{NULL, NULL, NULL, NULL}};
TH1D*       fHistoYieldTrueSecFracMeson[3][4]                           = {{NULL, NULL, NULL, NULL},{NULL, NULL, NULL, NULL},{NULL, NULL, NULL, NULL}};

//****************************************************************************
//******************* yield extraction fixed windows val. MC *****************
//****************************************************************************
Double_t*   fMesonTrueYieldFixedWindow                                  = NULL;
Double_t*   fMesonTrueYieldGammaFixedWindow                             = NULL;
Double_t*   fMesonTrueYieldGammaConvGammaFixedWindow                    = NULL;
Double_t*   fMesonTrueYieldConvGammaConvGammaFixedWindow                = NULL;
Double_t*   fMesonTrueYieldErrorFixedWindow                             = NULL;
Double_t*   fMesonTrueYieldGammaErrorFixedWindow                        = NULL;
Double_t*   fMesonTrueYieldGammaConvGammaErrorFixedWindow               = NULL;
Double_t*   fMesonTrueYieldConvGammaConvGammaErrorFixedWindow           = NULL;
TH1D*       fHistoYieldTrueMesonFixedWindow                             = NULL;
TH1D*       fHistoYieldTrueMesonGammaFixedWindow                        = NULL;
TH1D*       fHistoYieldTrueMesonGammaConvGammaFixedWindow               = NULL;
TH1D*       fHistoYieldTrueMesonConvGammaConvGammaFixedWindow           = NULL;

//*****************************************************************************
//*********************** dedicated cluster histograms ************************
//*****************************************************************************
TH1D*       fHistoClustersPt                                            = NULL;
TH1D*       fHistoClustersOverlapHeadersPt                              = NULL;

//*****************************************************************************
//******** Check multiple counts of neutral mesons, clusters ******************
//*****************************************************************************
TH2D*       fHistoTrueMesonDCInvMassVSPt                                = NULL;
TH1D**      fHistoMappingTrueMesonDCInvMassPtBins                       = NULL;  
TH1F*       fHistoTrueMesonMultipleCount                                = NULL;
Double_t*   fMesonTrueYieldsDC                                          = NULL;
Double_t*   fMesonTrueYieldsDCError                                     = NULL;
TH1D*       fHistoYieldTrueMesonDC                                      = NULL;
TH1F*       fHistoTrueGammaClusPt                                       = NULL;
TH1D*       fHistoTrueGammaDCClusPt                                     = NULL;
TH1F*       fHistoTrueGammaClusMultipleCount                            = NULL;

//*****************************************************************************
//************ Load secondary pion histograms from external file **************
//*****************************************************************************
Bool_t      fHaveToyMCInputForSec                                       = kFALSE;
Bool_t      fHaveCocktailInputForSec                                    = kFALSE;
TFile*      fFileToyMCInput[3]                                          = {NULL, NULL, NULL};
TFile*      fFileCocktailInput                                          = NULL;
TH1D*       fHistoYieldExternSecInput[3]                                = {NULL, NULL, NULL};
TH1D*       fHistoYieldExternSecInputReb[3]                             = {NULL, NULL, NULL};

//__________________________________________ Plotting all Invariant Mass bins _______________________________________________
void PlotAlphaInPtBins(   TH1D** fHistoMappingGGInvMassPtBinPlot,
                            TH1D** fHistoMappingBackNormInvMassPtBinPlot,
                            TString namePlot,
                            TString nameCanvas,
                            TString namePad,
                            Double_t* fPlottingRangeMeson,
                            TString dateDummy,
                            TString fMesonType,
                            Int_t fRowPlot,
                            Int_t fColumnPlot,
                            Int_t fStartBinPtRange,
                            Int_t fNumberPtBins,
                            Double_t* fRangeBinsPt,
                            TString fDecayChannel,
                            Bool_t fMonteCarloInfo,
                            TString decayChannel,
                            TString fDetectionChannel,
                            TString fEnergy){
    TGaxis::SetMaxDigits(3);

    TCanvas * canvasDataSpectra     = new TCanvas(nameCanvas.Data(),"",1400,900);  // gives the page size
    canvasDataSpectra->SetTopMargin(0.0);
    canvasDataSpectra->SetBottomMargin(0.0);
    canvasDataSpectra->SetRightMargin(0.0);
    canvasDataSpectra->SetLeftMargin(0.0);

    TPad * padDataSpectra           = new TPad(namePad.Data(),"",0.0,0.0,1.,1.,0);   // gives the size of the histo areas
    padDataSpectra->SetFillColor(0);
    padDataSpectra->GetFrame()->SetFillColor(0);
    padDataSpectra->SetBorderMode(0);
    padDataSpectra->SetLogy(0);
    padDataSpectra->Divide(fColumnPlot,fRowPlot,0.0,0.0);
    padDataSpectra->Draw();

    cout<<"fColumnPlot: "<<fColumnPlot<<" fRowPlot: "<<fRowPlot<<endl;
    cout << fMesonType.Data() << endl;
    Int_t place                     = 0;
    for(Int_t iPt=fStartBinPtRange;iPt<fNumberPtBins;iPt++){
        cout<<"Pt: "<<iPt<<" of "<<fNumberPtBins<<endl;
        Double_t startPt            = fRangeBinsPt[iPt];
        Double_t endPt              = fRangeBinsPt[iPt+1];

        place                       = place + 1; //give the right place in the page
        if (place == fColumnPlot){
            iPt--;
            padDataSpectra->cd(place);

            cout << "entered ALICE plotting" << endl;
            TString textAlice       = "ALICE performance";
            TString textEvents;
            if(fMonteCarloInfo){
                textEvents          = "MC";
            } else {
                textEvents          = "Data";
            }
            Double_t nPixels        = 13;
            Double_t textHeight     = 0.08;
            if (padDataSpectra->cd(place)->XtoPixel(padDataSpectra->cd(place)->GetX2()) < padDataSpectra->cd(place)->YtoPixel(padDataSpectra->cd(place)->GetY1())){
                textHeight          = (Double_t)nPixels/padDataSpectra->cd(place)->XtoPixel(padDataSpectra->cd(place)->GetX2()) ;
            } else {
                textHeight          = (Double_t)nPixels/padDataSpectra->cd(place)->YtoPixel(padDataSpectra->cd(place)->GetY1());
            }

            Double_t startTextX     = 0.10;
            Double_t startTextY     = 0.9;
            Double_t differenceText = textHeight*1.25;

            TLatex *alice           = new TLatex(startTextX, startTextY, Form("%s",textAlice.Data()));
            TLatex *latexDate       = new TLatex(startTextX, (startTextY-1.25*differenceText), dateDummy.Data());
            TLatex *energy          = new TLatex(startTextX, (startTextY-2.25*differenceText), fEnergy);
            TLatex *process         = new TLatex(startTextX, (startTextY-3.25*differenceText), fDecayChannel);
            TLatex *detprocess      = new TLatex(startTextX, (startTextY-4.25*differenceText), fDetectionChannel);
            TLatex *events          = new TLatex(startTextX, (startTextY-5.25*differenceText), Form("%s: %2.1e events",textEvents.Data(), fNEvents));

            alice->SetNDC();
            alice->SetTextColor(1);
            alice->SetTextSize(textHeight*1.3);
            alice->Draw();

            latexDate->SetNDC();
            latexDate->SetTextColor(1);
            latexDate->SetTextSize(textHeight);
            latexDate->Draw();

            energy->SetNDC();
            energy->SetTextColor(1);
            energy->SetTextSize(textHeight);
            energy->Draw();

            process->SetNDC();
            process->SetTextColor(1);
            process->SetTextSize(textHeight);
            process->Draw();

            detprocess->SetNDC();
            detprocess->SetTextColor(1);
            detprocess->SetTextSize(textHeight);
            detprocess->Draw();

            events->SetNDC();
            events->SetTextColor(1);
            events->SetTextSize(textHeight);
            events->Draw();

            TLegend* legendData     = new TLegend(startTextX,startTextY-5.75*differenceText,1,startTextY-(5.75+2.)*differenceText);
            legendData->SetTextSize(textHeight);
            legendData->SetTextFont(62);
            legendData->SetFillColor(0);
            legendData->SetFillStyle(0);
            legendData->SetLineWidth(0);
            legendData->SetLineColor(0);
            legendData->SetMargin(0.15);
            Size_t markersize       = fHistoMappingGGInvMassPtBinPlot[iPt]->GetMarkerSize();
            fHistoMappingGGInvMassPtBinPlot[iPt]->SetMarkerSize(3*markersize);
            legendData->AddEntry(fHistoMappingGGInvMassPtBinPlot[iPt],"same evt. #alpha (BG+Signal)","ep");
            Size_t linesize         = fHistoMappingBackNormInvMassPtBinPlot[iPt]->GetLineWidth();
            fHistoMappingBackNormInvMassPtBinPlot[iPt]->SetLineWidth(5*linesize);
            legendData->AddEntry(fHistoMappingBackNormInvMassPtBinPlot[iPt],"mixed evt. #alpha","l");
            legendData->Draw();
        } else {

            padDataSpectra->cd(place);
            padDataSpectra->cd(place)->SetTopMargin(0.12);
            padDataSpectra->cd(place)->SetBottomMargin(0.15);
            padDataSpectra->cd(place)->SetRightMargin(0.02);
            cout << "place: " << place << endl;
            int remaining           = (place-1)%fColumnPlot;
            if (remaining > 0) padDataSpectra->cd(place)->SetLeftMargin(0.15);
            else padDataSpectra->cd(place)->SetLeftMargin(0.25);
            cout << "here" << endl;
            DrawGammaHisto( fHistoMappingGGInvMassPtBinPlot[iPt],
                            Form("%3.2f GeV/#it{c} < #it{p}_{T} < %3.2f GeV/#it{c}",0.,1.),
                            Form("#alpha_{%s}",decayChannel.Data()), Form("dN_{%s}/d#it{M}_{%s}",decayChannel.Data(), decayChannel.Data()),
                            0.,1.,0);
            cout << "here" << endl;
            DrawGammaHisto( fHistoMappingBackNormInvMassPtBinPlot[iPt],
                            Form("%3.2f GeV/#it{c} < #it{p}_{T} < %3.2f GeV/#it{c}",0.,1.),
                            Form("#alpha_{%s}",decayChannel.Data()), Form("dN_{%s}/d#it{M}_{%s}",decayChannel.Data(), decayChannel.Data()),
                            0.,1.,1);
            cout << "here" << endl;
            Double_t fBGFitRangeLow     = fBGFitRange[0];
            Double_t fBGFitRangeHigh    = fBGFitRange[1];
            if (namePlot.Contains("Left")){
                fBGFitRangeLow          = fBGFitRangeLeft[0];
                fBGFitRangeHigh         = fBGFitRangeLeft[1];
            }
            TBox *box               = new TBox(fBGFitRangeLow,fHistoMappingGGInvMassPtBinPlot[iPt]->GetMaximum()*0.93,fBGFitRangeHigh,fHistoMappingGGInvMassPtBinPlot[iPt]->GetMaximum()*0.91);
            box->SetFillStyle(1001);
            box->SetFillColor(kAzure+9);
            box->Draw("same");
        }
    }
    canvasDataSpectra->Print(namePlot.Data());
    delete padDataSpectra;
    delete canvasDataSpectra;
}

//__________________________________________ Plotting all Invariant Mass bins _______________________________________________
void PlotThetaInPtBins(   TH1D** fHistoMappingGGInvMassPtBinPlot,
                            TH1D** fHistoMappingBackNormInvMassPtBinPlot,
                            TString namePlot,
                            TString nameCanvas,
                            TString namePad,
                            Double_t* fPlottingRangeMeson,
                            TString dateDummy,
                            TString fMesonType,
                            Int_t fRowPlot,
                            Int_t fColumnPlot,
                            Int_t fStartBinPtRange,
                            Int_t fNumberPtBins,
                            Double_t* fRangeBinsPt,
                            TString fDecayChannel,
                            Bool_t fMonteCarloInfo,
                            TString decayChannel,
                            TString fDetectionChannel,
                            TString fEnergy){
    TGaxis::SetMaxDigits(3);

    TCanvas * canvasDataSpectra     = new TCanvas(nameCanvas.Data(),"",1400,900);  // gives the page size
    canvasDataSpectra->SetTopMargin(0.0);
    canvasDataSpectra->SetBottomMargin(0.0);
    canvasDataSpectra->SetRightMargin(0.0);
    canvasDataSpectra->SetLeftMargin(0.0);

    TPad * padDataSpectra           = new TPad(namePad.Data(),"",0.0,0.0,1.,1.,0);   // gives the size of the histo areas
    padDataSpectra->SetFillColor(0);
    padDataSpectra->GetFrame()->SetFillColor(0);
    padDataSpectra->SetBorderMode(0);
    padDataSpectra->SetLogy(0);
    padDataSpectra->Divide(fColumnPlot,fRowPlot,0.0,0.0);
    padDataSpectra->Draw();

    cout<<"fColumnPlot: "<<fColumnPlot<<" fRowPlot: "<<fRowPlot<<endl;
    cout << fMesonType.Data() << endl;
    Int_t place                     = 0;
    for(Int_t iPt=fStartBinPtRange;iPt<fNumberPtBins;iPt++){
        cout<<"Pt: "<<iPt<<" of "<<fNumberPtBins<<endl;
        Double_t startPt            = fRangeBinsPt[iPt];
        Double_t endPt              = fRangeBinsPt[iPt+1];

        place                       = place + 1; //give the right place in the page
        if (place == fColumnPlot){
            iPt--;
            padDataSpectra->cd(place);

            cout << "entered ALICE plotting" << endl;
            TString textAlice       = "ALICE performance";
            TString textEvents;
            if(fMonteCarloInfo){
                textEvents          = "MC";
            } else {
                textEvents          = "Data";
            }
            Double_t nPixels        = 13;
            Double_t textHeight     = 0.08;
            if (padDataSpectra->cd(place)->XtoPixel(padDataSpectra->cd(place)->GetX2()) < padDataSpectra->cd(place)->YtoPixel(padDataSpectra->cd(place)->GetY1())){
                textHeight          = (Double_t)nPixels/padDataSpectra->cd(place)->XtoPixel(padDataSpectra->cd(place)->GetX2()) ;
            } else {
                textHeight          = (Double_t)nPixels/padDataSpectra->cd(place)->YtoPixel(padDataSpectra->cd(place)->GetY1());
            }

            Double_t startTextX     = 0.10;
            Double_t startTextY     = 0.9;
            Double_t differenceText = textHeight*1.25;

            TLatex *alice           = new TLatex(startTextX, startTextY, Form("%s",textAlice.Data()));
            TLatex *latexDate       = new TLatex(startTextX, (startTextY-1.25*differenceText), dateDummy.Data());
            TLatex *energy          = new TLatex(startTextX, (startTextY-2.25*differenceText), fEnergy);
            TLatex *process         = new TLatex(startTextX, (startTextY-3.25*differenceText), fDecayChannel);
            TLatex *detprocess      = new TLatex(startTextX, (startTextY-4.25*differenceText), fDetectionChannel);
            TLatex *events          = new TLatex(startTextX, (startTextY-5.25*differenceText), Form("%s: %2.1e events",textEvents.Data(), fNEvents));

            alice->SetNDC();
            alice->SetTextColor(1);
            alice->SetTextSize(textHeight*1.3);
            alice->Draw();

            latexDate->SetNDC();
            latexDate->SetTextColor(1);
            latexDate->SetTextSize(textHeight);
            latexDate->Draw();

            energy->SetNDC();
            energy->SetTextColor(1);
            energy->SetTextSize(textHeight);
            energy->Draw();

            process->SetNDC();
            process->SetTextColor(1);
            process->SetTextSize(textHeight);
            process->Draw();

            detprocess->SetNDC();
            detprocess->SetTextColor(1);
            detprocess->SetTextSize(textHeight);
            detprocess->Draw();

            events->SetNDC();
            events->SetTextColor(1);
            events->SetTextSize(textHeight);
            events->Draw();

            TLegend* legendData     = new TLegend(startTextX,startTextY-5.75*differenceText,1,startTextY-(5.75+2.)*differenceText);
            legendData->SetTextSize(textHeight);
            legendData->SetTextFont(62);
            legendData->SetFillColor(0);
            legendData->SetFillStyle(0);
            legendData->SetLineWidth(0);
            legendData->SetLineColor(0);
            legendData->SetMargin(0.15);
            Size_t markersize       = fHistoMappingGGInvMassPtBinPlot[iPt]->GetMarkerSize();
            fHistoMappingGGInvMassPtBinPlot[iPt]->SetMarkerSize(3*markersize);
            legendData->AddEntry(fHistoMappingGGInvMassPtBinPlot[iPt],"same evt. #theta (BG+Signal)","ep");
            Size_t linesize         = fHistoMappingBackNormInvMassPtBinPlot[iPt]->GetLineWidth();
            fHistoMappingBackNormInvMassPtBinPlot[iPt]->SetLineWidth(5*linesize);
            legendData->AddEntry(fHistoMappingBackNormInvMassPtBinPlot[iPt],"mixed evt. #theta","l");
            legendData->Draw();
        } else {

            padDataSpectra->cd(place);
            padDataSpectra->cd(place)->SetTopMargin(0.12);
            padDataSpectra->cd(place)->SetBottomMargin(0.15);
            padDataSpectra->cd(place)->SetRightMargin(0.02);
            cout << "place: " << place << endl;
            int remaining           = (place-1)%fColumnPlot;
            if (remaining > 0) padDataSpectra->cd(place)->SetLeftMargin(0.15);
            else padDataSpectra->cd(place)->SetLeftMargin(0.25);
            cout << "here" << endl;
            DrawGammaHisto( fHistoMappingGGInvMassPtBinPlot[iPt],
                            Form("%3.2f GeV/#it{c} < #it{p}_{T} < %3.2f GeV/#it{c}",startPt,endPt),
                            Form("#theta_{%s}",decayChannel.Data()), Form("dN_{%s}/d#it{M}_{%s}",decayChannel.Data(), decayChannel.Data()),
                            0.,3.,0);
            cout << "here" << endl;
            DrawGammaHisto( fHistoMappingBackNormInvMassPtBinPlot[iPt],
                            Form("%3.2f GeV/#it{c} < #it{p}_{T} < %3.2f GeV/#it{c}",startPt,endPt),
                            Form("#theta_{%s}",decayChannel.Data()), Form("dN_{%s}/d#it{M}_{%s}",decayChannel.Data(), decayChannel.Data()),
                            0.,3.,1);
            cout << "here" << endl;
            Double_t fBGFitRangeLow     = fBGFitRange[0];
            Double_t fBGFitRangeHigh    = fBGFitRange[1];
            if (namePlot.Contains("Left")){
                fBGFitRangeLow          = fBGFitRangeLeft[0];
                fBGFitRangeHigh         = fBGFitRangeLeft[1];
            }
            TBox *box               = new TBox(fBGFitRangeLow,fHistoMappingGGInvMassPtBinPlot[iPt]->GetMaximum()*0.93,fBGFitRangeHigh,fHistoMappingGGInvMassPtBinPlot[iPt]->GetMaximum()*0.91);
            box->SetFillStyle(1001);
            box->SetFillColor(kAzure+9);
            box->Draw("same");
        }
    }
    canvasDataSpectra->Print(namePlot.Data());
    delete padDataSpectra;
    delete canvasDataSpectra;
}

//__________________________________________ Plotting all Invariant Mass bins _______________________________________________
void Plot2DInPtBins(   TH2D** fHistoMappingGGInvMassPtBinPlot,
                              TString plot,
                            TString namePlot,
                            TString nameCanvas,
                            TString namePad,
                            Double_t* fPlottingRangeMeson,
                            TString dateDummy,
                            TString fMesonType,
                            Int_t fRowPlot,
                            Int_t fColumnPlot,
                            Int_t fStartBinPtRange,
                            Int_t fNumberPtBins,
                            Double_t* fRangeBinsPt,
                            TString fDecayChannel,
                            Bool_t fMonteCarloInfo,
                            TString decayChannel,
                            TString fDetectionChannel,
                            TString fEnergy){
    TGaxis::SetMaxDigits(3);

    TCanvas * canvasDataSpectra     = new TCanvas(nameCanvas.Data(),"",1400,900);  // gives the page size
    canvasDataSpectra->SetTopMargin(0.0);
    canvasDataSpectra->SetBottomMargin(0.0);
    canvasDataSpectra->SetRightMargin(0.0);
    canvasDataSpectra->SetLeftMargin(0.0);

    TPad * padDataSpectra           = new TPad(namePad.Data(),"",0.0,0.0,1.,1.,0);   // gives the size of the histo areas
    padDataSpectra->SetFillColor(0);
    padDataSpectra->GetFrame()->SetFillColor(0);
    padDataSpectra->SetBorderMode(0);
    padDataSpectra->SetLogy(0);
    padDataSpectra->Divide(fColumnPlot,fRowPlot,0.0,0.0);
    padDataSpectra->Draw();

    cout<<"fColumnPlot: "<<fColumnPlot<<" fRowPlot: "<<fRowPlot<<endl;
    cout << fMesonType.Data() << endl;
    Int_t place                     = 0;
    for(Int_t iPt=fStartBinPtRange;iPt<fNumberPtBins;iPt++){
        cout<<"Pt: "<<iPt<<" of "<<fNumberPtBins<<endl;
        Double_t startPt            = fRangeBinsPt[iPt];
        Double_t endPt              = fRangeBinsPt[iPt+1];

        place                       = place + 1; //give the right place in the page
        if (place == fColumnPlot){
            iPt--;
            padDataSpectra->cd(place);

            cout << "entered ALICE plotting" << endl;
            TString textAlice       = "ALICE performance";
            TString textEvents;
            if(fMonteCarloInfo){
                textEvents          = "MC";
            } else {
                textEvents          = "Data";
            }
            Double_t nPixels        = 13;
            Double_t textHeight     = 0.08;
            if (padDataSpectra->cd(place)->XtoPixel(padDataSpectra->cd(place)->GetX2()) < padDataSpectra->cd(place)->YtoPixel(padDataSpectra->cd(place)->GetY1())){
                textHeight          = (Double_t)nPixels/padDataSpectra->cd(place)->XtoPixel(padDataSpectra->cd(place)->GetX2()) ;
            } else {
                textHeight          = (Double_t)nPixels/padDataSpectra->cd(place)->YtoPixel(padDataSpectra->cd(place)->GetY1());
            }

            Double_t startTextX     = 0.10;
            Double_t startTextY     = 0.9;
            Double_t differenceText = textHeight*1.25;

            TLatex *alice           = new TLatex(startTextX, startTextY, Form("%s",textAlice.Data()));
            TLatex *latexDate       = new TLatex(startTextX, (startTextY-1.25*differenceText), dateDummy.Data());
            TLatex *energy          = new TLatex(startTextX, (startTextY-2.25*differenceText), fEnergy);
            TLatex *process         = new TLatex(startTextX, (startTextY-3.25*differenceText), fDecayChannel);
            TLatex *detprocess      = new TLatex(startTextX, (startTextY-4.25*differenceText), fDetectionChannel);
            TLatex *events          = new TLatex(startTextX, (startTextY-5.25*differenceText), Form("%s: %2.1e events",textEvents.Data(), fNEvents));

            alice->SetNDC();
            alice->SetTextColor(1);
            alice->SetTextSize(textHeight*1.3);
            alice->Draw();

            latexDate->SetNDC();
            latexDate->SetTextColor(1);
            latexDate->SetTextSize(textHeight);
            latexDate->Draw();

            energy->SetNDC();
            energy->SetTextColor(1);
            energy->SetTextSize(textHeight);
            energy->Draw();

            process->SetNDC();
            process->SetTextColor(1);
            process->SetTextSize(textHeight);
            process->Draw();

            detprocess->SetNDC();
            detprocess->SetTextColor(1);
            detprocess->SetTextSize(textHeight);
            detprocess->Draw();

            events->SetNDC();
            events->SetTextColor(1);
            events->SetTextSize(textHeight);
            events->Draw();

            TLegend* legendData     = new TLegend(startTextX,startTextY-5.75*differenceText,1,startTextY-(5.75+2.)*differenceText);
            legendData->SetTextSize(textHeight);
            legendData->SetTextFont(62);
            legendData->SetFillColor(0);
            legendData->SetFillStyle(0);
            legendData->SetLineWidth(0);
            legendData->SetLineColor(0);
            legendData->SetMargin(0.15);
            Size_t markersize       = fHistoMappingGGInvMassPtBinPlot[iPt]->GetMarkerSize();
            fHistoMappingGGInvMassPtBinPlot[iPt]->SetMarkerSize(3*markersize);
            legendData->AddEntry(fHistoMappingGGInvMassPtBinPlot[iPt],"same evt. #it{M}_{#gamma#gamma} (BG+Signal)","ep");
            legendData->Draw();
        } else {

            padDataSpectra->cd(place);
            padDataSpectra->cd(place)->SetTopMargin(0.12);
            padDataSpectra->cd(place)->SetBottomMargin(0.15);
            padDataSpectra->cd(place)->SetRightMargin(0.02);
            cout << "place: " << place << endl;
            int remaining           = (place-1)%fColumnPlot;
            if (remaining > 0) padDataSpectra->cd(place)->SetLeftMargin(0.15);
            else padDataSpectra->cd(place)->SetLeftMargin(0.25);
            cout << "here" << endl;
            DrawAutoGammaHisto2D( fHistoMappingGGInvMassPtBinPlot[iPt],
                            Form("%3.2f GeV/#it{c} < #it{p}_{T} < %3.2f GeV/#it{c}",startPt,endPt),
                            Form("#it{M}_{%s} (GeV/#it{c}^{2})",decayChannel.Data()),
                            Form("%s",plot.Data()),
                            "",
                            kFALSE, 0., 1.,
                            kTRUE, fPlottingRangeMeson[0],fPlottingRangeMeson[1]);
            cout << "here" << endl;
            Double_t fBGFitRangeLow     = fBGFitRange[0];
            Double_t fBGFitRangeHigh    = fBGFitRange[1];
            if (namePlot.Contains("Left")){
                fBGFitRangeLow          = fBGFitRangeLeft[0];
                fBGFitRangeHigh         = fBGFitRangeLeft[1];
            }
        }
    }
    canvasDataSpectra->Print(namePlot.Data());
    delete padDataSpectra;
    delete canvasDataSpectra;
}
