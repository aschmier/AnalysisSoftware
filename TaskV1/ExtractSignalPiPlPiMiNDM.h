// provided by Gamma Conversion Group, $ALICE_ROOT/PWG4/GammaConv ;https://twiki.cern.ch/twiki/bin/view/ALICE/PWG4GammaConversion

// Double, Int, etc.
TDatime	now;
std::fstream fFileErrLog;
std::fstream fFileDataLog;
std::fstream fFileDataTestLog;

TString fTypeCutSelection                                   = "";
TString fEventCutSelection                                  = "";
TString fGammaCutSelection                                  = "";
TString fClusterCutSelection                                = "";
TString fPionCutSelection                                   = "";
TString fNeutralPionCutSelection                            = "";
TString fMesonCutSelection                                  = "";

Int_t fCrysFitting                                          = 0;
Int_t fIsMC                                                 = 0;
Int_t fMesonId                                              = 0;
Float_t fBackgroundMultNumber;
Int_t fTotalBackFitMode                                     = 1; // 0: pol1 only; 1: pol2 only; 2: gauss+pol1; 3: gauss+pol2; 4: RooFit; 5: pol3; 6: gauss+pol3; 7: exp only; 8: gauss+exp
Int_t fFitRangeForMaximumMode                               = 0; // MesonRange for Maximum extraction for Fit: 0==Meson, 1==Peak, 2==Fit

Double_t fYMaxMeson                                         = 0.9;
Double_t fMesonMassExpect                                   = 0;   // expected meson mass
Double_t fNEvents                                           = 0;
Double_t fNorm                                              = 1;   // used in ProcessEM to make scaling factor globally available (will be overwritten each time ProcessEM was run)

Double_t *fPeakRange;
Double_t *fPeakRange_SubPiZero;
Double_t *fPeakRange_FixedPzPiZero;
Double_t *fIntFixedRange;
Double_t *fFitRange;
Double_t *fFitRange_SubPiZero;
Double_t *fFitRange_FixedPzPiZero;
Double_t *fBGFitRange                                       = NULL;
Double_t *fMesonIntRange                                    = NULL;

//Monitoring
Bool_t  DoMonitoring                                        = kTRUE;
Bool_t  DoMonitoringBackMixing                              = kTRUE;
Bool_t  DoMonitoringBackFit                                 = kTRUE;
Bool_t  DoMonitoring_Lambda                                 = kTRUE;
Bool_t  DoMonitoring_Mass                                   = kTRUE;
Bool_t  DoMonitoring_WidthFWHM                              = kTRUE;
Bool_t  DoMonitoring_WidthFWHMToSigma                       = kTRUE;
Bool_t  DoMonitoring_WidthSigma                             = kTRUE;
Bool_t  DoMonitoring_Amplitude                              = kTRUE;
Bool_t  DoMonitoring_BackFitParameters                      = kTRUE;

// TSring
TString fTextCent;
TString fEnergyFlag;
TString fPrefix;
TString fPrefix2;
TString fPeriodFlag;
TString ftextDayth;
TString fdate;
TString fCollisionSystem;
TString fTextMeasurement;
TString fDetectionProcess;
TString fCutSelection;
TString fBackgroundMultCutNumber;
Int_t   fMode;
Int_t   fTriggerInt;


TString fdirectphoton;
TString fThesis                                             = "";

// Output Files
TFile*  fOutput                                             = NULL;
TFile*  fOutput1                                            = NULL;
TFile*  fOutput2                                            = NULL;

Int_t       iBckSwitch                                      = 0;//5;
const Int_t       iNumberOfOtherSigToBckRatioFits           = 2; //If you change this value remember to also change: fHistoChi2SigToBckFit, fHistoChi2SigToBckFit, colorFitSigToBckFit, styleFitSigToBckFit, fSigToBckFitChi2, fFitAllOtherSigToBckFits, labelsOtherFitsRatio
//TH1D
TH1D*   fBckNorm                                            = NULL;
TH1D*   fSignal                                             = NULL;
TH1D*   fRatioSB                                            = NULL;
Double_t*   fMesonChi2[6]                                               = {NULL};  // 0-2: event mixing (normal, SubPiZero,FixedPzPiZero)
TH1D*       fHistoChi2[6]                                   = {NULL};
Double_t*   fSigToBckFitChi2[iNumberOfOtherSigToBckRatioFits+1] = {nullptr, nullptr, nullptr};
TH1D*       fHistoChi2SigToBckFit[iNumberOfOtherSigToBckRatioFits+1] = { nullptr, nullptr, nullptr };
Double_t*   fSigToBckFitChi2_SubPiZero[iNumberOfOtherSigToBckRatioFits+1] = {nullptr, nullptr, nullptr};
TH1D*       fHistoChi2SigToBckFit_SubPiZero[iNumberOfOtherSigToBckRatioFits+1] = { nullptr, nullptr, nullptr };
Double_t*   fSigToBckFitChi2_FixedPzPiZero[iNumberOfOtherSigToBckRatioFits+1] = {nullptr, nullptr, nullptr};
TH1D*       fHistoChi2SigToBckFit_FixedPzPiZero[iNumberOfOtherSigToBckRatioFits+1] = { nullptr, nullptr, nullptr };
TH1D*   fFittingHistMidPtSignal                             = NULL;
TH1D*   fFittingHistMidPtSignal_SubPiZero                   = NULL;
TH1D*   fFittingHistMidPtSignal_FixedPzPiZero               = NULL;
TH1D*   fFittingHistMidPtBackground[5]                      = {NULL,NULL,NULL,NULL,NULL};
TH1D*   fFittingHistMidPtBackground_SubPiZero[5]            = {NULL,NULL,NULL,NULL,NULL};
TH1D*   fFittingHistMidPtBackground_FixedPzPiZero[5]        = {NULL,NULL,NULL,NULL,NULL};
TH1D*   fFittingHistMidPtSignalSub                          = NULL;
TH1D*   fFittingHistMidPtSignalSub_SubPiZero                = NULL;
TH1D*   fFittingHistMidPtSignalSub_FixedPzPiZero            = NULL;
TH1D*   fMesonFullPtSignal                                  = NULL;
TH1D*   fMesonFullPtSignal_SubPiZero                        = NULL;
TH1D*   fMesonFullPtSignal_FixedPzPiZero                    = NULL;
TH1D*   fMesonFullPtBackground[5]                           = {NULL,NULL,NULL,NULL,NULL};
TH1D*   fMesonFullPtBackground_SubPiZero[5]                 = {NULL,NULL,NULL,NULL,NULL};
TH1D*   fMesonFullPtBackground_FixedPzPiZero[5]             = {NULL,NULL,NULL,NULL,NULL};
TH1D*   fOmegaFullPtSignal                                  = NULL;
TH1D*   fOmegaFullPtSignal_SubPiZero                        = NULL;
TH1D*   fOmegaFullPtSignal_FixedPzPiZero                    = NULL;
TH2D*   fOmegaFullPtSignalR2                                = NULL;
TH1D*   fOmegaFullPtSignalR1                                = NULL;
TH1D*   fOmegaFullPtBackNorm                                = NULL;
TH1D*   fOmegaFullPtBackNorm_SubPiZero                      = NULL;
TH1D*   fOmegaFullPtBackNorm_FixedPzPiZero                  = NULL;
TH1D*   fOmegaFullPtBack[4]                                 = {NULL,NULL,NULL,NULL};
TH1D*   fOmegaFullPtBack_SubPiZero[4]                       = {NULL,NULL,NULL,NULL};
TH1D*   fOmegaFullPtBack_FixedPzPiZero[4]                   = {NULL,NULL,NULL,NULL};
TH1D*   fRelation                                           = NULL;
TH1D*   fMesonFullPtBackNorm                                = NULL;
TH1D*   fMesonFullPtBackNorm_SubPiZero                      = NULL;
TH1D*   fMesonFullPtBackNorm_FixedPzPiZero                  = NULL;
TH1D*   fHistoMotherZMProj                                  = NULL;
TH1D*   fHistoBckZMProj                                     = NULL;
TH1D*   fNumberOfGoodESDTracks                              = NULL;
TH1D*   fEventQuality                                       = NULL;
TH1D*   fHistoMappingBackNormInvMass                        = NULL;
TH1D*   fHistoMappingBackNormInvMass_SubPiZero              = NULL;
TH1D*   fHistoMappingBackNormInvMass_FixedPzPiZero          = NULL;
TH1D*   fHistoMappingSignalInvMass                          = NULL;
TH1D*   fHistoMappingSignalInvMass_SubPiZero                = NULL;
TH1D*   fHistoMappingSignalInvMass_FixedPzPiZero            = NULL;
TH1D*   fHistoYieldMeson[6]                                 = {NULL, NULL, NULL, NULL, NULL, NULL};
TH1D*   fHistoYieldMeson_SubPiZero[6]                       = {NULL, NULL, NULL, NULL, NULL, NULL};
TH1D*   fHistoYieldMeson_FixedPzPiZero[6]                   = {NULL, NULL, NULL, NULL, NULL, NULL};
TH1D*   fHistoYieldMesonBackFit[3]                          = {NULL, NULL, NULL};
TH1D*   fHistoYieldMesonBackFit_SubPiZero[3]                = {NULL, NULL, NULL};
TH1D*   fHistoYieldMesonBackFit_FixedPzPiZero[3]            = {NULL, NULL, NULL};
TH1D*   fHistoYieldMesonBackFitFromFit                         = NULL;
TH1D*   fHistoYieldMesonBackFitFromFit_SubPiZero               = NULL;
TH1D*   fHistoYieldMesonBackFitFromFit_FixedPzPiZero           = NULL;
TH1D*   fHistoYieldMesonPerEvent[6]                         = {NULL, NULL, NULL, NULL, NULL, NULL};
TH1D*   fHistoYieldMesonPerEvent_SubPiZero[6]               = {NULL, NULL, NULL, NULL, NULL, NULL};
TH1D*   fHistoYieldMesonPerEvent_FixedPzPiZero[6]           = {NULL, NULL, NULL, NULL, NULL, NULL};
TH1D*   fHistoYieldMesonPerEventBackFit[3]                  = {NULL, NULL, NULL};
TH1D*   fHistoYieldMesonPerEventBackFit_SubPiZero[3]        = {NULL, NULL, NULL};
TH1D*   fHistoYieldMesonPerEventBackFit_FixedPzPiZero[3]    = {NULL, NULL, NULL};
TH1D*   fHistoYieldMesonPerEventBackFitFromFit           = NULL;
TH1D*   fHistoYieldMesonPerEventBackFitFromFit_SubPiZero       = NULL;
TH1D*   fHistoYieldMesonPerEventBackFitFromFit_FixedPzPiZero   = NULL;

TH1D*   fHistoClustersPt    = NULL;
TH1D*   fHistoClustersE     = NULL;

// True Yield histos for BG Analysis
TH1D*   fHistoYieldsMappingGGInvMass                           = NULL;
TH1D*   fHistoYieldsMappingGGInvMass_SubPiZero                 = NULL;
TH1D*   fHistoYieldsMappingGGInvMass_FixedPzPiZero             = NULL;
TH1D*   fHistoYieldsMappingTrueMeson                           = NULL;
TH1D*   fHistoYieldsMappingTrueMeson_SubPiZero                 = NULL;
TH1D*   fHistoYieldsMappingTrueMeson_FixedPzPiZero             = NULL;
TH1D*   fHistoYieldsMappingTruePiPlPiMiPiZeroCombinatorical    = NULL;
TH1D*   fHistoYieldsMappingTruePiPlPiMiPiZeroContamination     = NULL;
TH1D*   fHistoYieldsMappingTruePiPlPiMiSameMother[7]           = {NULL,NULL,NULL,NULL,NULL,NULL,NULL};
TH1D*   fHistoYieldsMappingTruePiPlPiZeroSameMother[5]         = {NULL,NULL,NULL,NULL,NULL};
TH1D*   fHistoYieldsMappingTruePiMiPiZeroSameMother[5]         = {NULL,NULL,NULL,NULL,NULL};


TH1D*   fHistoSBdefaultMeson[6]                             = { NULL, NULL, NULL, NULL, NULL, NULL};
TH1D*   fHistoSBdefaultMeson_SubPiZero[6]                   = { NULL, NULL, NULL, NULL, NULL, NULL};
TH1D*   fHistoSBdefaultMeson_FixedPzPiZero[6]               = { NULL, NULL, NULL, NULL, NULL, NULL};
TH1D*   fHistoSigndefaultMeson[6]                           = { NULL, NULL, NULL, NULL, NULL, NULL};
TH1D*   fHistoSigndefaultMeson_SubPiZero[6]                 = { NULL, NULL, NULL, NULL, NULL, NULL};
TH1D*   fHistoSigndefaultMeson_FixedPzPiZero[6]             = { NULL, NULL, NULL, NULL, NULL, NULL};
TH1D*   fHistoTrueSignMeson[3]                              = { NULL, NULL, NULL};
TH1D*   fHistoTrueSignMeson_SubPiZero[3]                    = { NULL, NULL, NULL};
TH1D*   fHistoTrueSignMeson_FixedPzPiZero[3]                = { NULL, NULL, NULL};
TH1D*   fHistoTrueSBMeson[3]                                = { NULL, NULL, NULL};
TH1D*   fHistoTrueSBMeson_SubPiZero[3]                      = { NULL, NULL, NULL};
TH1D*   fHistoTrueSBMeson_FixedPzPiZero[3]                  = { NULL, NULL, NULL};
TH1D*   fHistoMassPosition=									NULL;
//Fit Parameters
TH1D*   fHistoMassMeson=									NULL;
TH1D*   fHistoMassMeson_SubPiZero=							NULL;
TH1D*   fHistoMassMeson_FixedPzPiZero=						NULL;
TH1D*   fHistoMassMesonBackFit=							    NULL;
TH1D*   fHistoMassMesonBackFit_SubPiZero=					NULL;
TH1D*   fHistoMassMesonBackFit_FixedPzPiZero=				NULL;

TH1D*   fHistoLambdaMeson=									NULL;
TH1D*   fHistoLambdaMeson_SubPiZero=						NULL;
TH1D*   fHistoLambdaMeson_FixedPzPiZero=					NULL;
TH1D*   fHistoLambdaMesonBackFit=							NULL;
TH1D*   fHistoLambdaMesonBackFit_SubPiZero=					NULL;
TH1D*   fHistoLambdaMesonBackFit_FixedPzPiZero=				NULL;

TH1D*   fHistoWidthMeson=									NULL;
TH1D*   fHistoWidthMeson_SubPiZero=							NULL;
TH1D*   fHistoWidthMeson_FixedPzPiZero=						NULL;
TH1D*   fHistoWidthMesonBackFit=							NULL;
TH1D*   fHistoWidthMesonBackFit_SubPiZero=					NULL;
TH1D*   fHistoWidthMesonBackFit_FixedPzPiZero=				NULL;

TH1D*   fHistoFWHMMeson=									NULL;
TH1D*   fHistoFWHMMeson_SubPiZero=							NULL;
TH1D*   fHistoFWHMMeson_FixedPzPiZero=						NULL;
TH1D*   fHistoFWHMMesonBackFit=								NULL;
TH1D*   fHistoFWHMMesonBackFit_SubPiZero=				    NULL;
TH1D*   fHistoFWHMMesonBackFit_FixedPzPiZero=				NULL;

TH1D*   fHistoFWHMToSigmaMeson=								NULL;
TH1D*   fHistoFWHMToSigmaMeson_SubPiZero=					NULL;
TH1D*   fHistoFWHMToSigmaMeson_FixedPzPiZero=				NULL;
TH1D*   fHistoFWHMToSigmaMesonBackFit=						NULL;
TH1D*   fHistoFWHMToSigmaMesonBackFit_SubPiZero=            NULL;
TH1D*   fHistoFWHMToSigmaMesonBackFit_FixedPzPiZero=		NULL;

TH1D*   fHistoAmplitudeMeson=								NULL;
TH1D*   fHistoAmplitudeMeson_SubPiZero=						NULL;
TH1D*   fHistoAmplitudeMeson_FixedPzPiZero=					NULL;
TH1D*   fHistoAmplitudeMesonBackFit=						NULL;
TH1D*   fHistoAmplitudeMesonBackFit_SubPiZero=				NULL;
TH1D*   fHistoAmplitudeMesonBackFit_FixedPzPiZero=			NULL;
//Left Fit Parameters
TH1D*   fHistoMassMesonLeft=								NULL;
TH1D*   fHistoMassMesonLeft_SubPiZero=						NULL;
TH1D*   fHistoMassMesonLeft_FixedPzPiZero=					NULL;

TH1D*   fHistoLambdaMesonLeft=								NULL;
TH1D*   fHistoLambdaMesonLeft_SubPiZero=					NULL;
TH1D*   fHistoLambdaMesonLeft_FixedPzPiZero=				NULL;

TH1D*   fHistoWidthMesonLeft=								NULL;
TH1D*   fHistoWidthMesonLeft_SubPiZero=						NULL;
TH1D*   fHistoWidthMesonLeft_FixedPzPiZero=					NULL;

TH1D*   fHistoFWHMMesonLeft=								NULL;
TH1D*   fHistoFWHMMesonLeft_SubPiZero=						NULL;
TH1D*   fHistoFWHMMesonLeft_FixedPzPiZero=					NULL;

TH1D*   fHistoFWHMToSigmaMesonLeft=							NULL;
TH1D*   fHistoFWHMToSigmaMesonLeft_SubPiZero=				NULL;
TH1D*   fHistoFWHMToSigmaMesonLeft_FixedPzPiZero=			NULL;

TH1D*   fHistoAmplitudeMesonLeft=							NULL;
TH1D*   fHistoAmplitudeMesonLeft_SubPiZero=					NULL;
TH1D*   fHistoAmplitudeMesonLeft_FixedPzPiZero=				NULL;
//True Fit Parameters
TH1D*   fHistoTrueMassMeson=								NULL;
TH1D*   fHistoTrueMassMeson_SubPiZero=						NULL;
TH1D*   fHistoTrueMassMeson_FixedPzPiZero=					NULL;
TH1D*   fHistoTrueMassMesonReweighted=						NULL;
TH1D*   fHistoTrueMassMesonReweighted_SubPiZero=			NULL;
TH1D*   fHistoTrueMassMesonReweighted_FixedPzPiZero=		NULL;

TH1D*   fHistoTrueLambdaMeson=								NULL;
TH1D*   fHistoTrueLambdaMeson_SubPiZero=					NULL;
TH1D*   fHistoTrueLambdaMeson_FixedPzPiZero=				NULL;
TH1D*   fHistoTrueLambdaMesonReweighted=					NULL;
TH1D*   fHistoTrueLambdaMesonReweighted_SubPiZero=			NULL;
TH1D*   fHistoTrueLambdaMesonReweighted_FixedPzPiZero=		NULL;

TH1D*   fHistoTrueWidthMeson=								NULL;
TH1D*   fHistoTrueWidthMeson_SubPiZero=						NULL;
TH1D*   fHistoTrueWidthMeson_FixedPzPiZero=					NULL;
TH1D*   fHistoTrueWidthMesonReweighted=						NULL;
TH1D*   fHistoTrueWidthMesonReweighted_SubPiZero=			NULL;
TH1D*   fHistoTrueWidthMesonReweighted_FixedPzPiZero=		NULL;

TH1D*   fHistoTrueFWHMMeson=								NULL;
TH1D*   fHistoTrueFWHMMeson_SubPiZero=						NULL;
TH1D*   fHistoTrueFWHMMeson_FixedPzPiZero=					NULL;
TH1D*   fHistoTrueFWHMMesonReweighted=						NULL;
TH1D*   fHistoTrueFWHMMesonReweighted_SubPiZero=			NULL;
TH1D*   fHistoTrueFWHMMesonReweighted_FixedPzPiZero=    	NULL;

TH1D*   fHistoTrueFWHMToSigmaMeson=                         NULL;
TH1D*   fHistoTrueFWHMToSigmaMeson_SubPiZero=				NULL;
TH1D*   fHistoTrueFWHMToSigmaMeson_FixedPzPiZero=			NULL;
TH1D*   fHistoTrueFWHMToSigmaMesonReweighted=				NULL;
TH1D*   fHistoTrueFWHMToSigmaMesonReweighted_SubPiZero=		NULL;
TH1D*   fHistoTrueFWHMToSigmaMesonReweighted_FixedPzPiZero= NULL;

TH1D*   fHistoTrueAmplitudeMeson=							NULL;
TH1D*   fHistoTrueAmplitudeMeson_SubPiZero=					NULL;
TH1D*   fHistoTrueAmplitudeMeson_FixedPzPiZero=				NULL;
TH1D*   fHistoTrueAmplitudeMesonReweighted=					NULL;
TH1D*   fHistoTrueAmplitudeMesonReweighted_SubPiZero=		NULL;
TH1D*   fHistoTrueAmplitudeMesonReweighted_FixedPzPiZero=	NULL;

TH1D*   fDeltaPt=											NULL;
TH1D*   fHistoMCMesonPtWithinAcceptance=					NULL;
TH1D*   fHistoMCMesonPt=									NULL;
TH1D*   fHistoMCMesonPtWOWeights=							NULL;
TH1D*   fHistoMCMesonPtWeights=								NULL;
TH1D*   fHistoMCMesonWithinAccepPt=							NULL; // Proper bins in Pt
TH1D*   fHistoMCMesonPt1=									NULL; // Proper bins in Pt
TH1D*   fHistoYieldTrueMeson[3]                             = {NULL, NULL, NULL};
TH1D*   fHistoYieldTrueMeson_SubPiZero[3]                   = {NULL, NULL, NULL};
TH1D*   fHistoYieldTrueMeson_FixedPzPiZero[3]               = {NULL, NULL, NULL};
TH1D*   fHistoYieldTrueMesonReweighted[3]                   = {NULL, NULL, NULL};
TH1D*   fHistoYieldTrueMesonReweighted_SubPiZero[3]         = {NULL, NULL, NULL};
TH1D*   fHistoYieldTrueMesonReweighted_FixedPzPiZero[3]     = {NULL, NULL, NULL};

//BackFit
const Int_t     fMaxPossibleNumOfBackgroundFits=            5;
Int_t           fMaxNumOfBackgroundFits=                    fMaxPossibleNumOfBackgroundFits;
TH1D*           fHistoBackgroundFitPolParameter[fMaxPossibleNumOfBackgroundFits]=
                        {NULL, NULL, NULL, NULL, NULL};
TH1D*           fHistoBackgroundFitPolParameter_SubPiZero[fMaxPossibleNumOfBackgroundFits]=
                        {NULL, NULL, NULL, NULL, NULL};
TH1D*           fHistoBackgroundFitPolParameter_FixedPzPiZero[fMaxPossibleNumOfBackgroundFits]=
                        {NULL, NULL, NULL, NULL, NULL};

TH1D*   fHistoMCMesonAcceptPt                               = NULL;
TH1D*   fHistoMCMesonEffiPt                                 = NULL;
TH1D*   fHistoMCMesonEffiFitPt                              = NULL;
TH1D*   fHistoMonteMesonEffiPt[6]                           = {NULL, NULL, NULL, NULL, NULL, NULL};
TH1D*   fHistoMonteMesonEffiPt_SubPiZero[6]                 = {NULL, NULL, NULL, NULL, NULL, NULL};
TH1D*   fHistoMonteMesonEffiPt_FixedPzPiZero[6]             = {NULL, NULL, NULL, NULL, NULL, NULL};
TH1D*   fHistoMonteMesonEffiBackFitPt[3]                    = {NULL, NULL, NULL};
TH1D*   fHistoMonteMesonEffiBackFitPt_SubPiZero[3]          = {NULL, NULL, NULL};
TH1D*   fHistoMonteMesonEffiBackFitPt_FixedPzPiZero[3]      = {NULL, NULL, NULL};
TH1D*   fHistoMonteMesonEffiBackFitFromFitPt                = NULL;
TH1D*   fHistoMonteMesonEffiBackFitFromFitPt_SubPiZero      = NULL;
TH1D*   fHistoMonteMesonEffiBackFitFromFitPt_FixedPzPiZero  = NULL;
TH1D*   fHistoMonteMesonEffiMCAll                           = NULL;
TH1D*   fHistoMCTrueMesonEffiPt[3]                          = {NULL, NULL, NULL};
TH1D*   fHistoMCTrueMesonEffiPt_SubPiZero[3]                = {NULL, NULL, NULL};
TH1D*   fHistoMCTrueMesonEffiPt_FixedPzPiZero[3]            = {NULL, NULL, NULL};
TH1D*   fHistoMCTrueMesonEffiPtReweighted[3]                = {NULL, NULL, NULL};
TH1D*   fHistoMCTrueMesonEffiPtReweighted_SubPiZero[3]      = {NULL, NULL, NULL};
TH1D*   fHistoMCTrueMesonEffiPtReweighted_FixedPzPiZero[3]  = {NULL, NULL, NULL};
TH1D*   fHistoMCTrueMesonEffiFitPt[3]                       =  {NULL, NULL, NULL};

TH1D** 	fHistoMappingTrueMesonInvMassPtBins=				NULL;
TH1D** 	fHistoMappingTrueMesonInvMassPtBins_SubPiZero=		NULL;
TH1D** 	fHistoMappingTrueMesonInvMassPtBins_FixedPzPiZero=	NULL;
TH1D**	fHistoMappingTrueMesonInvMassPtReweightedBins=		NULL;
TH1D**	fHistoMappingTrueMesonInvMassPtReweightedBins_SubPiZero=       NULL;
TH1D**	fHistoMappingTrueMesonInvMassPtReweightedBins_FixedPzPiZero=   NULL;
TH1D** 	fHistoMappingTrueGGBckInvMassPtBins=				NULL;
TH1D** 	fHistoMappingTrueContBckInvMassPtBins=				NULL;
TH1D** 	fHistoMappingTrueAllBckInvMassPtBins=				NULL;
TH1D** 	fHistoMappingGGInvMassPtBin=						NULL;
TH1D** 	fHistoMappingGGInvMassPtBin_SubPiZero=  			NULL;
TH1D** 	fHistoMappingGGInvMassPtBin_FixedPzPiZero=  		NULL;
TH1D** 	fHistoMappingGGInvMassBackFitPtBin=					NULL;
TH1D** 	fHistoMappingGGInvMassBackFitPtBin_SubPiZero=		NULL;
TH1D** 	fHistoMappingGGInvMassBackFitPtBin_FixedPzPiZero=	NULL;
TH1D** 	fHistoMappingGGInvMassBackFitWithoutSignalPtBin=	NULL;
TH1D**  fHistoMappingTruePiPlPiMiPiZeroCombinatoricalPtBin= NULL;
TH1D**  fHistoMappingTruePiPlPiMiPiZeroContaminationPtBin=  NULL;
TH1D**  fHistoMapping_bck_Combinatorical=                   NULL;
TH1D**  fHistoMapping_bck_Combinatorical_SubPiZero=         NULL;
TH1D**  fHistoMapping_bck_Combinatorical_FixedPzPiZero=     NULL;
TH1D**  fHistoMapping_bck_Contamination=                    NULL;

TH1D**  fHistoMapping_bck_TrueContamination=                NULL;
TH1D**  fHistoMapping_bck_TrueCombinatorical=               NULL;

TH1D** 	fHistoMappingBackInvMassPtBin[5]=				      {NULL,NULL,NULL,NULL,NULL};
TH1D** 	fHistoMappingBackInvMassPtBin_SubPiZero[5]=		      {NULL,NULL,NULL,NULL,NULL};
TH1D** 	fHistoMappingBackInvMassPtBin_FixedPzPiZero[5]=	      {NULL,NULL,NULL,NULL,NULL};

TH1D** 	fHistoMappingTruePiPlPiMiSameMotherInvMassPtBin[7]  = {NULL,NULL,NULL,NULL,NULL,NULL,NULL};
TH1D** 	fHistoMappingTruePiMiPiZeroSameMotherInvMassPtBin[5]= {NULL,NULL,NULL,NULL,NULL};
TH1D** 	fHistoMappingTruePiPlPiZeroSameMotherInvMassPtBin[5]= {NULL,NULL,NULL,NULL,NULL};

TH1D** 	fHistoMappingBackNormInvMassPtBin[5]=			   {NULL,NULL,NULL,NULL,NULL};
TH1D** 	fHistoMappingBackNormInvMassPtBin_SubPiZero[5]=	   {NULL,NULL,NULL,NULL,NULL};
TH1D** 	fHistoMappingBackNormInvMassPtBin_FixedPzPiZero[5]={NULL,NULL,NULL,NULL,NULL};
TH1D** 	fHistoMappingBackSameNormInvMassPtBin[5]=		   {NULL,NULL,NULL,NULL,NULL}; // Contains the scaled background histos, but in this case they were all scaled with the same factor (obtained from scaling of tot. bck.)
TH1D** 	fHistoMappingBackSameNormInvMassPtBin_SubPiZero[5]=            {NULL,NULL,NULL,NULL,NULL}; // Contains the scaled background histos, but in this case they were all scaled with the same factor (obtained from scaling of tot. bck.)
TH1D** 	fHistoMappingBackSameNormInvMassPtBin_FixedPzPiZero[5]=		   {NULL,NULL,NULL,NULL,NULL}; // Contains the scaled background histos, but in this case they were all scaled with the same factor (obtained from scaling of tot. bck.)
TH1D** 	fHistoMappingSignalInvMassPtBin=				   NULL;
TH1D** 	fHistoMappingSignalInvMassPtBin_SubPiZero=		   NULL;
TH1D** 	fHistoMappingSignalInvMassPtBin_FixedPzPiZero=	   NULL;
TH1D** 	fHistoMappingSignalInvMassPtBinResSubtracted=				   NULL;
TH1D** 	fHistoMappingSignalInvMassPtBinResSubtracted_SubPiZero=		   NULL;
TH1D** 	fHistoMappingSignalInvMassPtBinResSubtracted_FixedPzPiZero=	   NULL;
//TH1D** 	fRatio=                                             NULL;
TH1D** 	fHistoMappingRatioSBInvMassPtBin[5]=			   {NULL,NULL,NULL,NULL,NULL};
TH1D** 	fHistoMappingRatioSBInvMassPtBin_SubPiZero[5]=	   {NULL,NULL,NULL,NULL,NULL};
TH1D** 	fHistoMappingRatioSBInvMassPtBin_FixedPzPiZero[5]= {NULL,NULL,NULL,NULL,NULL};
TH1D** 	fHistoMappingPeakPosInvMassPtBin=					NULL;
// Histograms for normalization on the left of the peak
TH1D** 	fHistoMappingBackNormInvMassLeftPtBin=                          NULL;
TH1D** 	fHistoMappingBackNormInvMassLeftPtBin_SubPiZero=				NULL;
TH1D** 	fHistoMappingBackNormInvMassLeftPtBin_FixedPzPiZero=			NULL;
TH1D** 	fHistoMappingSignalInvMassLeftPtBin=				NULL;
TH1D** 	fHistoMappingSignalInvMassLeftPtBin_SubPiZero=				    NULL;
TH1D** 	fHistoMappingSignalInvMassLeftPtBin_FixedPzPiZero=				NULL;
TH1D** 	fHistoMappingTrueMesonCaloPhotonInvMassPtBins =					NULL;
TH1D** 	fHistoMappingTrueMesonCaloConvPhotonInvMassPtBins =				NULL;
TH1D** 	fHistoMappingTrueMesonCaloElectronInvMassPtBins =				NULL;
TH1D** 	fHistoMappingTrueMesonCaloMergedClusterInvMassPtBins =			NULL;
TH1D** 	fHistoMappingTrueMesonCaloMergedClusterPartConvInvMassPtBins =	NULL;
TH1D** 	fHistoMappingTrueMesonCaloEMNonLeadingInvMassPtBins =			NULL;

TF1**  fBackgroundFitPol=                                             NULL;
TF1**  fBackgroundFitPol_SubPiZero=                                   NULL;
TF1**  fBackgroundFitPol_FixedPzPiZero=                               NULL;
TF1**  fBackgroundFitGaussPol=                                             NULL;
TF1**  fBackgroundFitGaussPol_SubPiZero=                                   NULL;
TF1**  fBackgroundFitGaussPol_FixedPzPiZero=                               NULL;
TH1D** fHistoBckFitConfidence =                               NULL;
TH1D** fHistoBckFitConfidence_SubPiZero =                               NULL;
TH1D** fHistoBckFitConfidence_FixedPzPiZero =                               NULL;
TH1D** fHistoBckFitWithGaussConfidence =                               NULL;
TH1D** fHistoBckFitWithGaussConfidence_SubPiZero =                               NULL;
TH1D** fHistoBckFitWithGaussConfidence_FixedPzPiZero =                               NULL;
TF1 * 	fFitReco=											NULL;
TF1 * 	fFitGausExp=										NULL;
TF1 * 	fFitLinearBck=										NULL;
TH1F * 	fHistSignalOnly=	     							NULL;
TF1 * 	fFitLinearBckCheck=									NULL;
TF1 * 	fFitLinearBckExcl=									NULL;
TF1 * 	fFitPol2BckExcl=									NULL;
TF1 * 	fFitLinearBckOut=									NULL;
TF1 * 	fFitSigBckPol1 =                                    NULL;
TF1 * 	fFitSigBckPol2 =                                    NULL;
TF1 * 	fFitSigBckPol3 =                                    NULL;
TF1 * 	fFitSigBckPol1_Prefit =                             NULL;
TF1 * 	fFitSigBckPol2_Prefit =                             NULL;
TF1 * 	fFitSigBckPol3_Prefit =                             NULL;
TF1 * 	fFitSigBckPol1_withGaus =                           NULL;
TF1 * 	fFitSigBckPol2_withGaus =                           NULL;
TF1 * 	fFitSigBckPol3_withGaus =                           NULL;
TF1 * 	fFitSignalInvMassMidPt=								NULL;
TF1 * 	fFitSignalInvMassMidPt_SubPiZero=					NULL;
TF1 * 	fFitSignalInvMassMidPt_FixedPzPiZero=				NULL;
TF1**   fFitAllOtherSigToBckFits[iNumberOfOtherSigToBckRatioFits] = { nullptr, nullptr };
TF1**   fFitAllOtherSigToBckFits_withGaus[iNumberOfOtherSigToBckRatioFits] = { nullptr, nullptr };
TF1**   fFitPolSigToBckFitPtBin=                            nullptr;
TF1**   fFitPolSigToBckFitPtBin_withGaus=                   nullptr;
TF1**   fFitAllOtherSigToBckFits_FixedPzPiZero[iNumberOfOtherSigToBckRatioFits] = { nullptr, nullptr };
TF1**   fFitAllOtherSigToBckFits_withGaus_FixedPzPiZero[iNumberOfOtherSigToBckRatioFits] = { nullptr, nullptr };
TF1**   fFitPolSigToBckFitPtBin_FixedPzPiZero=              nullptr;
TF1**   fFitPolSigToBckFitPtBin_withGaus_FixedPzPiZero=     nullptr;
TF1**   fFitAllOtherSigToBckFits_SubPiZero[iNumberOfOtherSigToBckRatioFits] = { nullptr, nullptr };
TF1**   fFitAllOtherSigToBckFits_withGaus_SubPiZero[iNumberOfOtherSigToBckRatioFits] = { nullptr, nullptr };
TF1**   fFitPolSigToBckFitPtBin_SubPiZero=                  nullptr;
TF1**   fFitPolSigToBckFitPtBin_withGaus_SubPiZero=         nullptr;

Double_t    fYields                                         = 0;
Double_t    fYieldsError                                    = 0;
Double_t    fYieldsFunc                                     = 0;
Double_t 	fFWHMFunc;
Double_t 	fFWHMFuncError;
Double_t 	fIntLinearBck;
Double_t 	fIntLinearBckError;
Float_t 	fIntGaussExp;
Float_t 	fIntGaussExpError;
Double_t 	fIntLinearBckOut;
Double_t 	fIntLinearBckErrorOut;

Float_t 	pictDrawingCoordinatesFWHM[9] = 				{0.6, 0.8, 0.30, 0.04, 0.15,0.7, 0.1, 0.035,0};
Int_t 	fNRebinGlobal = 									2;

// common meson analysis variables (So it is easy to switch between the pi0 and the eta without changing the code)
Double_t 	*fBGFitRangeLeft = 								NULL;
Double_t 	*fBGFitRangeLeft_SubPiZero = 					NULL;
Double_t 	*fBGFitRangeLeft_FixedPzPiZero =				NULL;
Double_t 	*fBGFitRange_ContamintationFit =                NULL;
Double_t 	*fBGFitRange_SubPiZero_ContamintationFit =      NULL;
Double_t 	*fBGFitRange_FixedPzPiZero_ContamintationFit =  NULL;
Double_t 	*fBGFitRangeLeft_ContamintationFit =    		NULL;
Double_t 	*fBGFitRangeLeft_SubPiZero_ContamintationFit = 	NULL;
Double_t 	*fBGFitRangeLeft_FixedPzPiZero_ContamintationFit = NULL;
Double_t 	*fMesonPlotRange = 								NULL;

// Names
TString     nameIntRange[6]                                             = {"", "Wide", "Narrow", "Left", "LeftWide", "LeftNarrow"};
TString     nameIntBck[6]                                               = {"Minpol2","Pluspol2","Minexp","Plusexp","Minexp2","Plusexp2"};
TString     nameIntBckRatios[6]                                         = {"RatioMinpol2","RatioPluspol2","RatioMinexp","RatioPlusexp","RatioMinexp2","RatioPlusexp2"};
TString     nameIntBckResult[3]                                         = {"pol2_normal","exp_normal","exp2_normal"};


// Added because of change of how integration ranges are stored
Bool_t      useFitIntDeltaRange =                           kFALSE;
Int_t       useFitIntDeltaRange_FunctionNumber =            0;
Int_t       useFitIntDeltaRange_ParameterNumber =           0;
Double_t    *fMesonIntDeltaRange =                          NULL;
Double_t    *fMesonIntDeltaRangeWide =                      NULL;
Double_t    *fMesonIntDeltaRangeNarrow =                    NULL;
//
Double_t 	*fMesonIntRangeWide = 							NULL;
Double_t 	*fMesonIntRangeNarrow = 						NULL;
Double_t    *fMesonCurIntRange[6] =                         {NULL, NULL, NULL, NULL, NULL, NULL};
Double_t    *fMesonCurIntRange_SubPiZero[6] =               {NULL, NULL, NULL, NULL, NULL, NULL};
Double_t    *fMesonCurIntRange_FixedPzPiZero[6] =           {NULL, NULL, NULL, NULL, NULL, NULL};
Double_t 	*fMesonCurIntRangeBackFit[6] = 					{NULL, NULL, NULL, NULL, NULL, NULL};
Double_t 	*fMesonCurIntRangeBackFit_SubPiZero[6] =          	{NULL, NULL, NULL, NULL, NULL, NULL};
Double_t 	*fMesonCurIntRangeBackFit_FixedPzPiZero[6] = 		{NULL, NULL, NULL, NULL, NULL, NULL};
Double_t 	*fMesonTrueIntRange[3] =						{NULL, NULL, NULL};
Double_t 	*fMesonTrueIntRange_SubPiZero[3] =				{NULL, NULL, NULL};
Double_t 	*fMesonTrueIntRange_FixedPzPiZero[3] =			{NULL, NULL, NULL};
Double_t    *fMesonTrueIntReweightedRange[3] =				{NULL, NULL, NULL};
Double_t    *fMesonTrueIntReweightedRange_SubPiZero[3] =	{NULL, NULL, NULL};
Double_t    *fMesonTrueIntReweightedRange_FixedPzPiZero[3] ={NULL, NULL, NULL};

Double_t 	*fMesonMassRange = 								NULL;
Double_t 	*fMesonMassRange_SubPiZero = 					NULL;
Double_t 	*fMesonMassRange_FixedPzPiZero =				NULL;
Double_t    *fMesonMassPlotRange =                          NULL;
Double_t    *fMesonMassPlotRange_SubPiZero =                NULL;
Double_t    *fMesonMassPlotRange_FixedPzPiZero =            NULL;
Double_t 	*fMesonFitRange = 								NULL;
Double_t 	*fMesonFitRange_SubPiZero = 					NULL;
Double_t 	*fMesonFitRange_FixedPzPiZero =					NULL;
Double_t 	*fMesonFitRangeWithoutPeak = 					NULL;

Double_t 	fMesonWidthExpect=								0;
Double_t 	fMesonWidthExpectMC=							0;
Double_t 	fMesonLambdaTail=								0;
Double_t 	fMesonLambdaTailTrue=   						0;
Double_t 	fMesonLambdaTailMC=								0;
Double_t 	*fMesonWidthRange = 							NULL;
Double_t 	*fMesonWidthRangeTrue = 						NULL; // with range used for fitting of true
Double_t 	*fMesonWidthRangeMC = 							NULL;
Double_t 	*fMesonLambdaTailRange = 						NULL;
Double_t 	*fMesonLambdaTailRangeTrue =					NULL;
Double_t 	*fMidPt = 										NULL;
Double_t 	*fFullPt = 										NULL;
Double_t FitRangeSigBckRatioOption =                        0;
Double_t usePolNForBackgroundScaling =                      0;

// end common meson analysis variables

//Background histograms in different M and Z bins

TH2D* fHistoMotherZM;
TH2D* fHistoBckZM;
TH2D* fPi0ResponseMatrix;
TH2D* fPi0ResponseMatrixRebin;
TH2D* fPeakPosAlpha01;

Double_t 	fScalingFactorBck[7][4];
Double_t 	fCBAlpha;
Double_t 	fCBn;

///////////////////////////
TString 	fNameHistoGG;
TString 	fNameHistoBack;
TString 	fNameHistoPP;
TString 	fNameHistoBackNormOut;
TString 	fNameHistoBackNormLeftOut;
TString 	fNameHistoTrue;
TString 	fNameHistoTrueGGBck;
TString 	fNameHistoTrueContBck;
TString 	fNameHistoTrueAllBck;
TString 	fNameHistoEffi;
TString 	fNameHistoFrac;
TString 	fNameHistoMotherZM;
TString 	fNameHistoBckZM;
TString 	fNameFitSignalPos;

Double_t* fGGYields[6] =                                    {NULL, NULL, NULL, NULL, NULL, NULL};
Double_t* fGGYields_SubPiZero[6] =                          {NULL, NULL, NULL, NULL, NULL, NULL};
Double_t* fGGYields_FixedPzPiZero[6] =                      {NULL, NULL, NULL, NULL, NULL, NULL};
Double_t* fBckYields[6] =                                   {NULL, NULL, NULL, NULL, NULL, NULL};
Double_t* fBckYields_SubPiZero[6] =                         {NULL, NULL, NULL, NULL, NULL, NULL};
Double_t* fBckYields_FixedPzPiZero[6] =                     {NULL, NULL, NULL, NULL, NULL, NULL};
Double_t* fTotalBckYields[6]= 								{NULL, NULL, NULL, NULL, NULL, NULL};
Double_t* fTotalBckYields_SubPiZero[6]= 					{NULL, NULL, NULL, NULL, NULL, NULL};
Double_t* fTotalBckYields_FixedPzPiZero[6]=					{NULL, NULL, NULL, NULL, NULL, NULL};
Double_t* fMesonYields[6]=                                  {NULL, NULL, NULL, NULL, NULL, NULL};
Double_t* fMesonYields_SubPiZero[6]=                        {NULL, NULL, NULL, NULL, NULL, NULL};
Double_t* fMesonYields_FixedPzPiZero[6]=                    {NULL, NULL, NULL, NULL, NULL, NULL};
Double_t* fMesonYieldsBackFit[3]= 							{NULL, NULL, NULL};
Double_t* fMesonYieldsBackFit_SubPiZero[3]= 				{NULL, NULL, NULL};
Double_t* fMesonYieldsBackFit_FixedPzPiZero[3]=			    {NULL, NULL, NULL};
Double_t* fMesonYieldsBackFitFromFit= 						 NULL;
Double_t* fMesonYieldsBackFitFromFit_SubPiZero= 			 NULL;
Double_t* fMesonYieldsBackFitFromFit_FixedPzPiZero=			 NULL;
Double_t* fMesonYieldsBackFitFromFitError= 						 NULL;
Double_t* fMesonYieldsBackFitFromFitError_SubPiZero= 			 NULL;
Double_t* fMesonYieldsBackFitFromFitError_FixedPzPiZero=			 NULL;
Double_t* fMesonYieldsBackFitFromFitPerEvent= 						 NULL;
Double_t* fMesonYieldsBackFitFromFitPerEvent_SubPiZero= 			 NULL;
Double_t* fMesonYieldsBackFitFromFitPerEvent_FixedPzPiZero=			 NULL;
Double_t* fMesonYieldsBackFitFromFitPerEventError= 						 NULL;
Double_t* fMesonYieldsBackFitFromFitPerEventError_SubPiZero= 			 NULL;
Double_t* fMesonYieldsBackFitFromFitPerEventError_FixedPzPiZero=			 NULL;
Double_t* fMesonTrueYields[3]= 								{NULL, NULL, NULL};
Double_t* fMesonTrueYields_SubPiZero[3]=					{NULL, NULL, NULL};
Double_t* fMesonTrueYields_FixedPzPiZero[3]=				{NULL, NULL, NULL};
Double_t* fMesonTrueYieldsReweighted[3]=                    {NULL, NULL, NULL};
Double_t* fMesonTrueYieldsReweighted_SubPiZero[3]=          {NULL, NULL, NULL};
Double_t* fMesonTrueYieldsReweighted_FixedPzPiZero[3]=      {NULL, NULL, NULL};
Double_t* fMesonYieldsFunc[6] =                             {NULL, NULL, NULL, NULL, NULL, NULL};
Double_t* fMesonYieldsResidualBckFunc[6]= 					{NULL, NULL, NULL, NULL, NULL, NULL};
Double_t* fMesonYieldsResidualBckFunc_SubPiZero[6]= 		{NULL, NULL, NULL, NULL, NULL, NULL};
Double_t* fMesonYieldsResidualBckFunc_FixedPzPiZero[6]=		{NULL, NULL, NULL, NULL, NULL, NULL};
Double_t* fMesonYieldsResidualBckFuncBackFit= 				NULL;
Double_t* fMesonYieldsResidualBckFuncBackFit_SubPiZero=		NULL;
Double_t* fMesonYieldsResidualBckFuncBackFit_FixedPzPiZero=	NULL;
Double_t* fMesonYieldsCorResidualBckFunc[6]=                    {NULL, NULL, NULL, NULL, NULL, NULL};
Double_t* fMesonYieldsCorResidualBckFunc_SubPiZero[6]=          {NULL, NULL, NULL, NULL, NULL, NULL};
Double_t* fMesonYieldsCorResidualBckFunc_FixedPzPiZero[6]=      {NULL, NULL, NULL, NULL, NULL, NULL};
Double_t* fMesonYieldsCorResidualBckFuncBackFit[3]=                 {NULL, NULL, NULL};
Double_t* fMesonYieldsCorResidualBckFuncBackFit_SubPiZero[3]=		{NULL, NULL, NULL};
Double_t* fMesonYieldsCorResidualBckFuncBackFit_FixedPzPiZero[3]=	{NULL, NULL, NULL};
Double_t* fMesonYieldsPerEvent[6]= 							{NULL, NULL, NULL, NULL, NULL, NULL};
Double_t* fMesonYieldsPerEvent_SubPiZero[6]=				{NULL, NULL, NULL, NULL, NULL, NULL};
Double_t* fMesonYieldsPerEvent_FixedPzPiZero[6]=			{NULL, NULL, NULL, NULL, NULL, NULL};
Double_t* fMesonYieldsPerEventBackFit[3]= 						{NULL, NULL, NULL};
Double_t* fMesonYieldsPerEventBackFit_SubPiZero[3]= 			{NULL, NULL, NULL};
Double_t* fMesonYieldsPerEventBackFit_FixedPzPiZero[3]=		{NULL, NULL, NULL};

// True yields for bck contribution comparison
Double_t* fYieldsMappingTruePiPlPiMiSameMother[7]=          {NULL, NULL, NULL, NULL, NULL, NULL, NULL};
Double_t* fYieldsMappingTruePiPlPiMiSameMotherError[7]=          {NULL, NULL, NULL, NULL, NULL, NULL, NULL};
Double_t* fYieldsMappingTruePiMiPiZeroSameMother[5]= 		{NULL, NULL, NULL, NULL, NULL};
Double_t* fYieldsMappingTruePiMiPiZeroSameMotherError[5]= 		{NULL, NULL, NULL, NULL, NULL};
Double_t* fYieldsMappingTruePiPlPiZeroSameMother[5]= 		{NULL, NULL, NULL, NULL, NULL};
Double_t* fYieldsMappingTruePiPlPiZeroSameMotherError[5]= 		{NULL, NULL, NULL, NULL, NULL};
Double_t* fYieldsMappingTruePiPlPiMiPiZeroCombinatorical=         NULL;
Double_t* fYieldsMappingTruePiPlPiMiPiZeroCombinatoricalError=   NULL;
Double_t* fYieldsMappingTruePiPlPiMiPiZeroContamination=          NULL;
Double_t* fYieldsMappingTruePiPlPiMiPiZeroContaminationError=    NULL;

//Fit Parameters
Double_t* fMesonMass= 										NULL;
Double_t* fMesonMass_SubPiZero= 							NULL;
Double_t* fMesonMass_FixedPzPiZero=							NULL;
Double_t* fMesonMassBackFit= 								NULL;
Double_t* fMesonMassBackFit_SubPiZero= 						NULL;
Double_t* fMesonMassBackFit_FixedPzPiZero=					NULL;
Double_t* fMesonLambdaTailpar= 								NULL;
Double_t* fMesonLambdaTailpar_SubPiZero= 					NULL;
Double_t* fMesonLambdaTailpar_FixedPzPiZero=				NULL;
Double_t* fMesonLambdaTailparBackFit= 						NULL;
Double_t* fMesonLambdaTailparBackFit_SubPiZero= 			NULL;
Double_t* fMesonLambdaTailparBackFit_FixedPzPiZero=			NULL;
Double_t* fMesonWidth= 										NULL;
Double_t* fMesonWidth_SubPiZero=							NULL;
Double_t* fMesonWidth_FixedPzPiZero=						NULL;
Double_t* fMesonWidthBackFit= 								NULL;
Double_t* fMesonWidthBackFit_SubPiZero= 					NULL;
Double_t* fMesonWidthBackFit_FixedPzPiZero= 				NULL;
//Double_t* fMesonSB= 										NULL;
Double_t* fMesonSBdefault[3]                                = { NULL, NULL, NULL};
Double_t* fMesonSBdefault_SubPiZero[3]                      = { NULL, NULL, NULL};
Double_t* fMesonSBdefault_FixedPzPiZero[3]                  = { NULL, NULL, NULL};
Double_t* fMesonSigndefault[3]                              = { NULL, NULL, NULL};
Double_t* fMesonSigndefault_SubPiZero[3]                    = { NULL, NULL, NULL};
Double_t* fMesonSigndefault_FixedPzPiZero[3]                = { NULL, NULL, NULL};
Double_t* fMesonSBdefaultError[3]                           = { NULL, NULL, NULL};
Double_t* fMesonSBdefaultError_SubPiZero[3]                 = { NULL, NULL, NULL};
Double_t* fMesonSBdefaultError_FixedPzPiZero[3]             = { NULL, NULL, NULL};
Double_t* fMesonSigndefaultError[3]                         = { NULL, NULL, NULL};
Double_t* fMesonSigndefaultError_SubPiZero[3]               = { NULL, NULL, NULL};
Double_t* fMesonSigndefaultError_FixedPzPiZero[3]           = { NULL, NULL, NULL};
Double_t* fMesonTrueSB[3] 									= { NULL, NULL, NULL};
Double_t* fMesonTrueSB_SubPiZero[3] 					    = { NULL, NULL, NULL};
Double_t* fMesonTrueSB_FixedPzPiZero[3]  				    = { NULL, NULL, NULL};
Double_t* fMesonTrueSign[3]  							    = { NULL, NULL, NULL};
Double_t* fMesonTrueSign_SubPiZero[3]						= { NULL, NULL, NULL};
Double_t* fMesonTrueSign_FixedPzPiZero[3]                   = { NULL, NULL, NULL};
//Double_t* fMesonSign= 										NULL;
Double_t* fMesonFWHM= 										NULL;
Double_t* fMesonFWHM_SubPiZero= 							NULL;
Double_t* fMesonFWHM_FixedPzPiZero= 						NULL;
Double_t* fMesonFWHMBackFit= 								NULL;
Double_t* fMesonFWHMBackFit_SubPiZero=						NULL;
Double_t* fMesonFWHMBackFit_FixedPzPiZero=					NULL;
Double_t* fMesonAmplitude= 									NULL;
Double_t* fMesonAmplitude_SubPiZero= 						NULL;
Double_t* fMesonAmplitude_FixedPzPiZero= 					NULL;
Double_t* fMesonAmplitudeBackFit= 							NULL;
Double_t* fMesonAmplitudeBackFit_SubPiZero=					NULL;
Double_t* fMesonAmplitudeBackFit_FixedPzPiZero=				NULL;

//Left Fit Parameters
Double_t* fMesonMassLeft=									NULL;
Double_t* fMesonMassLeft_SubPiZero=							NULL;
Double_t* fMesonMassLeft_FixedPzPiZero=						NULL;
Double_t* fMesonLambdaTailparLeft= 							NULL;
Double_t* fMesonLambdaTailparLeft_SubPiZero= 				NULL;
Double_t* fMesonLambdaTailparLeft_FixedPzPiZero=			NULL;
Double_t* fMesonWidthLeft=									NULL;
Double_t* fMesonWidthLeft_SubPiZero=						NULL;
Double_t* fMesonWidthLeft_FixedPzPiZero=					NULL;
Double_t* fMesonFWHMLeft=									NULL;
Double_t* fMesonFWHMLeft_SubPiZero=         				NULL;
Double_t* fMesonFWHMLeft_FixedPzPiZero=						NULL;
Double_t* fMesonAmplitudeLeft=								NULL;
Double_t* fMesonAmplitudeLeft_SubPiZero=         			NULL;
Double_t* fMesonAmplitudeLeft_FixedPzPiZero=				NULL;

//True Fit Parameters
Double_t* fMesonTrueMass= 									NULL;
Double_t* fMesonTrueMass_SubPiZero=							NULL;
Double_t* fMesonTrueMass_FixedPzPiZero=						NULL;
Double_t* fMesonTrueMassBackFit=							NULL;
Double_t* fMesonTrueMassBackFit_SubPiZero=					NULL;
Double_t* fMesonTrueMassBackFit_FixedPzPiZero=				NULL;
Double_t* fMesonTrueMassCaloPhoton= 						NULL;
Double_t* fMesonTrueMassCaloElectron= 						NULL;
Double_t* fMesonTrueMassCaloConvPhoton=						NULL;
Double_t* fMesonTrueMassCaloMergedCluster= 					NULL;
Double_t* fMesonTrueMassCaloMergedClusterPartConv= 			NULL;
Double_t* fMesonTrueMassCaloEMNonLeading= 					NULL;
Double_t* fMesonTrueMassReweighted=							NULL;
Double_t* fMesonTrueMassReweighted_SubPiZero=				NULL;
Double_t* fMesonTrueMassReweighted_FixedPzPiZero=			NULL;
Double_t* fMesonTrueLambdaTailpar= 							NULL;
Double_t* fMesonTrueLambdaTailpar_SubPiZero= 				NULL;
Double_t* fMesonTrueLambdaTailpar_FixedPzPiZero=			NULL;
Double_t* fMesonTrueLambdaTailparBackFit= 					NULL;
Double_t* fMesonTrueLambdaTailparBackFit_SubPiZero= 		NULL;
Double_t* fMesonTrueLambdaTailparBackFit_FixedPzPiZero=		NULL;
Double_t* fMesonTrueLambdaTailparReweighted=				NULL;
Double_t* fMesonTrueLambdaTailparReweighted_SubPiZero=		NULL;
Double_t* fMesonTrueLambdaTailparReweighted_FixedPzPiZero=	NULL;
Double_t* fMesonTrueWidth= 									NULL;
Double_t* fMesonTrueWidth_SubPiZero=						NULL;
Double_t* fMesonTrueWidth_FixedPzPiZero=					NULL;
Double_t* fMesonTrueWidthBackFit=							NULL;
Double_t* fMesonTrueWidthBackFit_SubPiZero=					NULL;
Double_t* fMesonTrueWidthBackFit_FixedPzPiZero=				NULL;
Double_t* fMesonTrueWidthReweighted=						NULL;
Double_t* fMesonTrueWidthReweighted_SubPiZero=				NULL;
Double_t* fMesonTrueWidthReweighted_FixedPzPiZero=			NULL;
Double_t* fMesonTrueFWHM= 									NULL;
Double_t* fMesonTrueFWHM_SubPiZero= 						NULL;
Double_t* fMesonTrueFWHM_FixedPzPiZero=  					NULL;
Double_t* fMesonTrueFWHMBackFit= 							NULL;
Double_t* fMesonTrueFWHMBackFit_SubPiZero= 			     	NULL;
Double_t* fMesonTrueFWHMBackFit_FixedPzPiZero=  			NULL;
Double_t* fMesonTrueFWHMCaloPhoton= 						NULL;
Double_t* fMesonTrueFWHMCaloElectron= 						NULL;
Double_t* fMesonTrueFWHMCaloConvPhoton=						NULL;
Double_t* fMesonTrueFWHMCaloMergedCluster= 					NULL;
Double_t* fMesonTrueFWHMCaloMergedClusterPartConv= 			NULL;
Double_t* fMesonTrueFWHMCaloEMNonLeading= 					NULL;
Double_t* fMesonTrueFWHMReweighted=							NULL;
Double_t* fMesonTrueFWHMReweighted_SubPiZero=				NULL;
Double_t* fMesonTrueFWHMReweighted_FixedPzPiZero=			NULL;
Double_t* fMesonFWHMAlpha01= 								NULL;
Double_t* fMesonTrueAmplitude= 								NULL;
Double_t* fMesonTrueAmplitude_SubPiZero= 					NULL;
Double_t* fMesonTrueAmplitude_FixedPzPiZero=  				NULL;
Double_t* fMesonTrueAmplitudeBackFit= 						NULL;
Double_t* fMesonTrueAmplitudeBackFit_SubPiZero= 			NULL;
Double_t* fMesonTrueAmplitudeBackFit_FixedPzPiZero=  		NULL;
Double_t* fMesonTrueAmplitudeReweighted=					NULL;
Double_t* fMesonTrueAmplitudeReweighted_SubPiZero=			NULL;
Double_t* fMesonTrueAmplitudeReweighted_FixedPzPiZero=		NULL;

//BackFit
Double_t*           fBackgroundFitPolParameter[fMaxPossibleNumOfBackgroundFits]=
                        {NULL, NULL, NULL, NULL, NULL};
Double_t*           fBackgroundFitPolParameter_SubPiZero[fMaxPossibleNumOfBackgroundFits]=
                        {NULL, NULL, NULL, NULL, NULL};
Double_t*           fBackgroundFitPolParameter_FixedPzPiZero[fMaxPossibleNumOfBackgroundFits]=
                        {NULL, NULL, NULL, NULL, NULL};
Double_t*           fBackgroundFitPolParameterErr[fMaxPossibleNumOfBackgroundFits]=
                        {NULL, NULL, NULL, NULL, NULL};
Double_t*           fBackgroundFitPolParameterErr_SubPiZero[fMaxPossibleNumOfBackgroundFits]=
                        {NULL, NULL, NULL, NULL, NULL};
Double_t*           fBackgroundFitPolParameterErr_FixedPzPiZero[fMaxPossibleNumOfBackgroundFits]=
                        {NULL, NULL, NULL, NULL, NULL};
Double_t           fBackgroundFitPolParameterMaximumValue[fMaxPossibleNumOfBackgroundFits]=
                        {0., 0., 0., 0., 0.};
Double_t           fBackgroundFitPolParameterMaximumValue_SubPiZero[fMaxPossibleNumOfBackgroundFits]=
                        {0., 0., 0., 0., 0.};
Double_t           fBackgroundFitPolParameterMaximumValue_FixedPzPiZero[fMaxPossibleNumOfBackgroundFits]=
                        {0., 0., 0., 0., 0.};
Double_t           fBackgroundFitPolParameterMinimumValue[fMaxPossibleNumOfBackgroundFits]=
                        {0., 0., 0., 0., 0.};
Double_t           fBackgroundFitPolParameterMinimumValue_SubPiZero[fMaxPossibleNumOfBackgroundFits]=
                        {0., 0., 0., 0., 0.};
Double_t           fBackgroundFitPolParameterMinimumValue_FixedPzPiZero[fMaxPossibleNumOfBackgroundFits]=
                        {0., 0., 0., 0., 0.};

Double_t fScaleFac  =                                         1.;

Double_t* fGGYieldsError[6]                             = {NULL, NULL, NULL, NULL, NULL, NULL};
Double_t* fGGYieldsError_SubPiZero[6]                   = {NULL, NULL, NULL, NULL, NULL, NULL};
Double_t* fGGYieldsError_FixedPzPiZero[6]               = {NULL, NULL, NULL, NULL, NULL, NULL};
Double_t* fBckYieldsError[6] =                            {NULL, NULL, NULL, NULL, NULL, NULL};
Double_t* fBckYieldsError_SubPiZero[6] =                  {NULL, NULL, NULL, NULL, NULL, NULL};
Double_t* fBckYieldsError_FixedPzPiZero[6] =              {NULL, NULL, NULL, NULL, NULL, NULL};
Double_t* fMesonYieldsError[6]=							   {NULL, NULL, NULL, NULL, NULL, NULL};
Double_t* fMesonYieldsError_SubPiZero[6]=				   {NULL, NULL, NULL, NULL, NULL, NULL};
Double_t* fMesonYieldsError_FixedPzPiZero[6]=			   {NULL, NULL, NULL, NULL, NULL, NULL};
Double_t* fMesonYieldsBackFitError[3]=				       {NULL, NULL, NULL};
Double_t* fMesonYieldsBackFitError_SubPiZero[3]=	       {NULL, NULL, NULL};
Double_t* fMesonYieldsBackFitError_FixedPzPiZero[3]=	   {NULL, NULL, NULL};
Double_t* fMesonTrueYieldsError[3]=                        {NULL, NULL, NULL};
Double_t* fMesonTrueYieldsError_SubPiZero[3]=              {NULL, NULL, NULL};
Double_t* fMesonTrueYieldsError_FixedPzPiZero[3]=          {NULL, NULL, NULL};
Double_t* fMesonTrueYieldsReweightedError[3]=              {NULL, NULL, NULL};
Double_t* fMesonTrueYieldsReweightedError_SubPiZero[3]=        {NULL, NULL, NULL};
Double_t* fMesonTrueYieldsReweightedError_FixedPzPiZero[3]=    {NULL, NULL, NULL};
Double_t* fMesonYieldsFuncError[6]=							{NULL, NULL, NULL, NULL, NULL, NULL};
Double_t* fMesonYieldsResidualBckFuncError[6]=              {NULL, NULL, NULL, NULL, NULL, NULL};
Double_t* fMesonYieldsResidualBckFuncError_SubPiZero[6]=     {NULL, NULL, NULL, NULL, NULL, NULL};
Double_t* fMesonYieldsResidualBckFuncError_FixedPzPiZero[6]= {NULL, NULL, NULL, NULL, NULL, NULL};
Double_t* fMesonYieldsResidualBckFuncBackFitError=			    NULL;
Double_t* fMesonYieldsResidualBckFuncBackFitError_SubPiZero=	NULL;
Double_t* fMesonYieldsResidualBckFuncBackFitError_FixedPzPiZero=NULL;
Double_t* fMesonYieldsCorResidualBckFuncError[6]=                       {NULL, NULL, NULL, NULL, NULL, NULL};
Double_t* fMesonYieldsCorResidualBckFuncError_SubPiZero[6]=             {NULL, NULL, NULL, NULL, NULL, NULL};
Double_t* fMesonYieldsCorResidualBckFuncError_FixedPzPiZero[6]=			{NULL, NULL, NULL, NULL, NULL, NULL};
Double_t* fMesonYieldsCorResidualBckFuncBackFitError[3]=                {NULL, NULL, NULL};
Double_t* fMesonYieldsCorResidualBckFuncBackFitError_SubPiZero[3]=      {NULL, NULL, NULL};
Double_t* fMesonYieldsCorResidualBckFuncBackFitError_FixedPzPiZero[3]=  {NULL, NULL, NULL};
Double_t* fMesonYieldsPerEventError[6]=                       			{NULL, NULL, NULL, NULL, NULL, NULL};
Double_t* fMesonYieldsPerEventError_SubPiZero[6]=						{NULL, NULL, NULL, NULL, NULL, NULL};
Double_t* fMesonYieldsPerEventError_FixedPzPiZero[6]=					{NULL, NULL, NULL, NULL, NULL, NULL};
Double_t* fMesonYieldsPerEventBackFitError[3]=                         	{NULL, NULL, NULL};
Double_t* fMesonYieldsPerEventBackFitError_SubPiZero[3]=				{NULL, NULL, NULL};
Double_t* fMesonYieldsPerEventBackFitError_FixedPzPiZero[3]=			{NULL, NULL, NULL};

//Fit Parameter Errors
Double_t* fMesonMassError=									NULL;
Double_t* fMesonMassError_SubPiZero=						NULL;
Double_t* fMesonMassError_FixedPzPiZero=        			NULL;
Double_t* fMesonMassBackFitError=							NULL;
Double_t* fMesonMassBackFitError_SubPiZero=					NULL;
Double_t* fMesonMassBackFitError_FixedPzPiZero=				NULL;
Double_t* fMesonLambdaTailparError=							NULL;
Double_t* fMesonLambdaTailparError_SubPiZero=				NULL;
Double_t* fMesonLambdaTailparError_FixedPzPiZero=        	NULL;
Double_t* fMesonLambdaTailparBackFitError=					NULL;
Double_t* fMesonLambdaTailparBackFitError_SubPiZero=		NULL;
Double_t* fMesonLambdaTailparBackFitError_FixedPzPiZero=	NULL;
Double_t* fMesonWidthError=									NULL;
Double_t* fMesonWidthError_SubPiZero=						NULL;
Double_t* fMesonWidthError_FixedPzPiZero=					NULL;
Double_t* fMesonWidthBackFitError=							NULL;
Double_t* fMesonWidthBackFitError_SubPiZero=				NULL;
Double_t* fMesonWidthBackFitError_FixedPzPiZero=      		NULL;
Double_t* fMesonFWHMError=									NULL;
Double_t* fMesonFWHMError_SubPiZero=						NULL;
Double_t* fMesonFWHMError_FixedPzPiZero=   					NULL;
Double_t* fMesonFWHMBackFitError=							NULL;
Double_t* fMesonFWHMBackFitError_SubPiZero=					NULL;
Double_t* fMesonFWHMBackFitError_FixedPzPiZero=   			NULL;
Double_t* fMesonAmplitudeError=								NULL;
Double_t* fMesonAmplitudeError_SubPiZero=					NULL;
Double_t* fMesonAmplitudeError_FixedPzPiZero=   			NULL;
Double_t* fMesonAmplitudeBackFitError=						NULL;
Double_t* fMesonAmplitudeBackFitError_SubPiZero=			NULL;
Double_t* fMesonAmplitudeBackFitError_FixedPzPiZero=   		NULL;

// Left Fit Parameter Errors
Double_t* fMesonMassLeftError=                              NULL;
Double_t* fMesonMassLeftError_SubPiZero=                    NULL;
Double_t* fMesonMassLeftError_FixedPzPiZero=                NULL;
Double_t* fMesonLambdaTailparLeftError=						NULL;
Double_t* fMesonLambdaTailparLeftError_SubPiZero= 			NULL;
Double_t* fMesonLambdaTailparLeftError_FixedPzPiZero=		NULL;
Double_t* fMesonWidthLeftError=								NULL;
Double_t* fMesonWidthLeftError_SubPiZero=					NULL;
Double_t* fMesonWidthLeftError_FixedPzPiZero=				NULL;
Double_t* fMesonFWHMLeftError=                              NULL;
Double_t* fMesonFWHMLeftError_SubPiZero=                    NULL;
Double_t* fMesonFWHMLeftError_FixedPzPiZero=                NULL;
Double_t* fMesonAmplitudeLeftError=                         NULL;
Double_t* fMesonAmplitudeLeftError_SubPiZero=               NULL;
Double_t* fMesonAmplitudeLeftError_FixedPzPiZero=           NULL;

//True Fit Parameter Errors
Double_t* fMesonTrueMassError=								NULL;
Double_t* fMesonTrueMassError_SubPiZero=					NULL;
Double_t* fMesonTrueMassError_FixedPzPiZero=				NULL;
Double_t* fMesonTrueMassBackFitError=						NULL;
Double_t* fMesonTrueMassBackFitError_SubPiZero=				NULL;
Double_t* fMesonTrueMassBackFitError_FixedPzPiZero=			NULL;
Double_t* fMesonTrueMassReweightedError=					NULL;
Double_t* fMesonTrueMassReweightedError_SubPiZero=			NULL;
Double_t* fMesonTrueMassReweightedError_FixedPzPiZero=		NULL;
Double_t* fMesonTrueLambdaTailparError=						NULL;
Double_t* fMesonTrueLambdaTailparError_SubPiZero=			NULL;
Double_t* fMesonTrueLambdaTailparError_FixedPzPiZero=		NULL;
Double_t* fMesonTrueLambdaTailparBackFitError=				NULL;
Double_t* fMesonTrueLambdaTailparBackFitError_SubPiZero=	NULL;
Double_t* fMesonTrueLambdaTailparBackFitError_FixedPzPiZero= NULL;
Double_t* fMesonTrueLambdaTailparReweightedError=			NULL;
Double_t* fMesonTrueLambdaTailparReweightedError_SubPiZero=	NULL;
Double_t* fMesonTrueLambdaTailparReweightedError_FixedPzPiZero=	NULL;
Double_t* fMesonTrueWidthError=								NULL;
Double_t* fMesonTrueWidthError_SubPiZero=					NULL;
Double_t* fMesonTrueWidthError_FixedPzPiZero=				NULL;
Double_t* fMesonTrueWidthBackFitError=						NULL;
Double_t* fMesonTrueWidthBackFitError_SubPiZero=			NULL;
Double_t* fMesonTrueWidthBackFitError_FixedPzPiZero=		NULL;
Double_t* fMesonTrueWidthReweightedError=					NULL;
Double_t* fMesonTrueWidthReweightedError_SubPiZero=			NULL;
Double_t* fMesonTrueWidthReweightedError_FixedPzPiZero=		NULL;
Double_t* fMesonTrueFWHMError=								NULL;
Double_t* fMesonTrueFWHMError_SubPiZero=					NULL;
Double_t* fMesonTrueFWHMError_FixedPzPiZero=				NULL;
Double_t* fMesonTrueFWHMBackFitError=						NULL;
Double_t* fMesonTrueFWHMBackFitError_SubPiZero=				NULL;
Double_t* fMesonTrueFWHMBackFitError_FixedPzPiZero=			NULL;
Double_t* fMesonTrueFWHMReweightedError=					NULL;
Double_t* fMesonTrueFWHMReweightedError_SubPiZero=			NULL;
Double_t* fMesonTrueFWHMReweightedError_FixedPzPiZero=		NULL;
Double_t* fMesonTrueAmplitudeError=							NULL;
Double_t* fMesonTrueAmplitudeError_SubPiZero=				NULL;
Double_t* fMesonTrueAmplitudeError_FixedPzPiZero=			NULL;
Double_t* fMesonTrueAmplitudeBackFitError=					NULL;
Double_t* fMesonTrueAmplitudeBackFitError_SubPiZero=		NULL;
Double_t* fMesonTrueAmplitudeBackFitError_FixedPzPiZero=	NULL;
Double_t* fMesonTrueAmplitudeReweightedError=				NULL;
Double_t* fMesonTrueAmplitudeReweightedError_SubPiZero=		NULL;
Double_t* fMesonTrueAmplitudeReweightedError_FixedPzPiZero=	NULL;

Double_t* fMesonSBError=									NULL;
Double_t* fMesonSignError=									NULL;
Double_t* fMesonTrueSBError[3] 								= {NULL,NULL,NULL};
Double_t* fMesonTrueSBError_SubPiZero[3] 					= {NULL,NULL,NULL};
Double_t* fMesonTrueSBError_FixedPzPiZero[3]				= {NULL,NULL,NULL};
Double_t* fMesonTrueSignError[3]							= {NULL,NULL,NULL};
Double_t* fMesonTrueSignError_SubPiZero[3]					= {NULL,NULL,NULL};
Double_t* fMesonTrueSignError_FixedPzPiZero[3]				= {NULL,NULL,NULL};

Double_t* fTotalBckYieldsError[6]=							{NULL, NULL, NULL, NULL, NULL, NULL};
Double_t* fTotalBckYieldsError_SubPiZero[6]=				{NULL, NULL, NULL, NULL, NULL, NULL};
Double_t* fTotalBckYieldsError_FixedPzPiZero[6]=			{NULL, NULL, NULL, NULL, NULL, NULL};

TF1** 	fFitSignalInvMassPtBin=								NULL;
TF1** 	fFitSignalInvMassPtBin_SubPiZero=					NULL;
TF1** 	fFitSignalInvMassPtBin_FixedPzPiZero=				NULL;
TF1** 	fFitSignalInvMassBackFitPtBin=						NULL;
TF1** 	fFitSignalInvMassBackFitPtBin_SubPiZero=			NULL;
TF1** 	fFitSignalInvMassBackFitPtBin_FixedPzPiZero=    	NULL;
TF1** 	fFitTrueSignalInvMassPtBin=							NULL;
TF1** 	fFitTrueSignalInvMassPtBin_SubPiZero=				NULL;
TF1** 	fFitTrueSignalInvMassPtBin_FixedPzPiZero=			NULL;
TF1**	fFitTrueSignalInvMassPtReweightedBin=				NULL;
TF1**	fFitTrueSignalInvMassPtReweightedBin_SubPiZero=		NULL;
TF1**	fFitTrueSignalInvMassPtReweightedBin_FixedPzPiZero=	NULL;
TF1** 	fFitBckInvMassPtBin=								NULL;
TF1** 	fFitBckInvMassPtBin_SubPiZero=          			NULL;
TF1** 	fFitBckInvMassPtBin_FixedPzPiZero=					NULL;
TF1** 	fFitBckInvMassBackFitPtBin=							NULL;
TF1** 	fFitBckInvMassBackFitPtBin_SubPiZero=				NULL;
TF1** 	fFitBckInvMassBackFitPtBin_FixedPzPiZero=			NULL;
TF1** 	fFitRatioInvMassPtBin=								NULL;
TF1**	fFitWithPol2ForBG = 								NULL;
TF1**	fFitWithPol2ForBG_SubPiZero =						NULL;
TF1**	fFitWithPol2ForBG_FixedPzPiZero =					NULL;

TF1** 	fFitInvMassLeftPtBin=								NULL;
TF1** 	fFitInvMassLeftPtBin_SubPiZero=						NULL;
TF1** 	fFitInvMassLeftPtBin_FixedPzPiZero=					NULL;
TF1** 	fFitSignalPeakPosInvMassLeftPtBin=					NULL;
TF1** 	fFitSignalPeakPosInvMassLeftPtBin_SubPiZero=		NULL;
TF1** 	fFitSignalPeakPosInvMassLeftPtBin_FixedPzPiZero=	NULL;
TF1** 	fFitBckInvMassLeftPtBin=							NULL;
TF1** 	fFitBckInvMassLeftPtBin_SubPiZero=              	NULL;
TF1** 	fFitBckInvMassLeftPtBin_FixedPzPiZero=				NULL;

TH2D* 	fHistoTrueMesonInvMassVSPt=							NULL;
TH2D* 	fHistoTrueMesonInvMassVSPt_SubNDM=				    NULL;
TH2D* 	fHistoTrueMesonInvMassVSPt_FixedPzNDM=	  		    NULL;
TH2D*	fHistoTrueMesonInvMassVSPtWOWeights=				NULL;
TH2D*	fHistoTrueMesonInvMassVSPtWOWeights_SubNDM=			NULL;
TH2D*	fHistoTrueMesonInvMassVSPtWOWeights_FixedPzNDM=		NULL;
TH2D*	fHistoTrueMesonInvMassVSPtReweighted=				NULL;
TH2D*	fHistoTrueMesonInvMassVSPtReweighted_SubNDM=		NULL;
TH2D*	fHistoTrueMesonInvMassVSPtReweighted_FixedPzNDM=	NULL;
TProfile2D*	fProfileTrueMesonInvMassVSPtWeights=		 	NULL;

TH2D*	fGammaGammaInvMassVSPt=								NULL;
TH2D*	fGammaGammaInvMassVSPt_SubPiZero=					NULL;
TH2D*	fGammaGammaInvMassVSPt_FixedPzPiZero=				NULL;
TH2D*   hist_bck[4]=                                        {NULL,NULL,NULL,NULL};
TH2D*   hist_true_PiPlPiMi_SameMother[6]=                   {NULL,NULL,NULL,NULL,NULL,NULL};
TH2D*   hist_true_PiMiPiZero_SameMother[4]=                   {NULL,NULL,NULL,NULL};
TH2D*   hist_true_PiPlPiZero_SameMother[4]=                   {NULL,NULL,NULL,NULL};
TH2D*   hist_bck_SubPiZero[4]=                              {NULL,NULL,NULL,NULL};
TH2D*   hist_bck_FixedPzPiZero[4]=                          {NULL,NULL,NULL,NULL};
TH2D*   hist_bck_Signal=                                    NULL;
TH2D*   hist_bck_True=                                      NULL;
TH2D*   hist_bck_SubPiZero_Signal=                          NULL;
TH2D*   hist_bck_SubPiZero_True=                            NULL;
TH2D*   hist_bck_FixedPzPiZero_Signal=                      NULL;
TH2D*   hist_bck_FixedPzPiZero_True=                        NULL;
TH2D*   hist_bck_Combinatorical=                            NULL;
TH2D*   hist_bck_Combinatorical_SubPiZero=                  NULL;
TH2D*   hist_bck_Combinatorical_FixedPzPiZero=              NULL;
TH2D*   hist_bck_Contamination=                             NULL;
TH2D*   hist_bck_TrueContamination=                         NULL;
TH2D*   hist_bck_TrueCombinatorical=                        NULL;
TH2D*   hist_bck_Combinatorical_SameMother=                 NULL;
TH2D*	fBckInvMassVSPt[5]=									{NULL,NULL,NULL,NULL,NULL}; // 0: Background summed 1: Background Group 1 2: Background Group 2 ...
TH2D*	fBckInvMassVSPt_SubPiZero[5]=			            {NULL,NULL,NULL,NULL,NULL}; // 0: Background summed 1: Background Group 1 2: Background Group 2 ...
TH2D*	fBckInvMassVSPt_FixedPzPiZero[5]=                 	{NULL,NULL,NULL,NULL,NULL}; // 0: Background summed 1: Background Group 1 2: Background Group 2 ...

TH2D*	fTruePiPlPiMiSameMotherInvMassVSPt[7]=				{NULL,NULL,NULL,NULL,NULL,NULL,NULL}; // 0: True PiPlPiMi have same mother
                                                                                                  // 1: True PiPlPiMi have same mother (which is an Eta)
                                                                                                  // 2: True PiPlPiMi have same mother (which is a Omega)
                                                                                                  // 3: True PiPlPiMi have same mother (which is a Rho)
                                                                                                  // 4: True PiPlPiMi have same mother (which is a EtaPrime)
                                                                                                  // 5: True PiPlPiMi have same mother (which is a K0s)
                                                                                                  // 6: True PiPlPiMi have same mother (which is a K0l)
TH2D*	fTruePiMiPiZeroSameMotherInvMassVSPt[5]=              {NULL,NULL,NULL,NULL,NULL};           // 0: True PiMiPiZero have same mother
                                                                                                  // 1: eta 2: omega: 3: rho 4:K0l
TH2D*	fTruePiPlPiZeroSameMotherInvMassVSPt[5]=              {NULL,NULL,NULL,NULL,NULL};           // 0: True PiPlPiZero have same mother
                                                                                                  // 1: eta 2: omega: 3: rho 4:K0l
TH2D*   fTruePiPlPiMiPiZeroCombinatorical_InvMassPt=    NULL;
TH2D*   fTruePiPlPiMiPiZeroContamination_InvMassPt=    NULL;
TH2F**	fHistoWeightsBGZbinVsMbin = 						0x0;
TH2F**	fHistoFillPerEventBGZbinVsMbin = 					0x0;


TString ObjectNameTrue;
TString ObjectNameTrueWOWeights;
TString ObjectNameProfileWeights;
TString ObjectNameMCPi0Acc;
TString ObjectNameMCEtaAcc;
TString ObjectNameMCOmegaAcc;
TString ObjectNameMCPi0;
TString ObjectNameMCPi0WOWeights;
TString ObjectNameMCEta;
TString ObjectNameMCEtaWOWeights;
TString ObjectNameMCOmega;
TString ObjectNameMCOmegaWOWeights;


Bool_t fAdvancedMesonQA = kFALSE;
Bool_t fEstimateTrainPileUp = kFALSE;

void ProcessEM(TH1D*,TH1D*,Double_t *);
void ProcessEMLeftRight(    TH1D* , TH1D*, Double_t*, Double_t* );
//void ProcessBckFitSubtraction(TH1D *fGammaGamma, Int_t i, Double_t * fPeakRangeDummy, Double_t *fFitRangeDummy, TString energy, TString suffix, TString cutSelection, TString meson, Int_t InvMassType);
void ProcessBckFitSubtraction(TH1D*, Int_t, Double_t* ,Double_t*, TString, TString, TString, TString, Int_t);
void ProcessBckFitSubtraction_CombinatoricsAndContamination(TH1D*, TH1D*, TH1D*, Int_t, Double_t* ,Double_t*, TString, TString, TString, TString, Int_t, TH1D*, TH1D*);
void ProcessEM_switch(TH1D*,TH1D*,Double_t *);
Double_t FitFunctionSigBckRatio(Double_t *, Double_t *);
Double_t FitFunctionSigBckRatioPol1(Double_t *, Double_t *);
Double_t FitFunctionSigBckRatioPol2(Double_t *, Double_t *);
Double_t FitFunctionSigBckRatioPol3(Double_t *, Double_t *);
Double_t FitFunctionSigBckRatioPol1_withGaus(Double_t *, Double_t *);
Double_t FitFunctionSigBckRatioPol2_withGaus(Double_t *, Double_t *);
Double_t FitFunctionSigBckRatioPol3_withGaus(Double_t *, Double_t *);
Double_t FitFunctionSigBckRatioPol1_withGausExp(Double_t *, Double_t *);
Double_t FitFunctionSigBckRatioPol2_withGausExp(Double_t *, Double_t *);
Double_t FitFunctionSigBckRatioPol3_withGausExp(Double_t *, Double_t *);
void ProcessEM_FitBins(TH1D*,TH1D*,Double_t *);
void ProcessRatioSignalBackground(TH1D* , TH1D* );
void FillMassHistosArray(TH2D* fGammaGammaInvMassVSPtDummy, TH2D *fGammaGammaInvMassVSPtDummy_SubPiZero, TH2D *fGammaGammaInvMassVSPtDummy_FixedPzPiZero);
void ProjectHistogramInPtBins(TH2D* HistogramToProject, TH1D **ProjectedHistograms);
void FillMassMCTrueMesonHistosArray(TH2D*);
void FillMassMCTrueReweightedMesonHistosArray(TH2D*);
void CreatePtHistos();
void FillPtHistos();
void FitSubtractedInvMassInPtBins(TH1D * , Double_t *fMesonIntDeltaRangeFit, Int_t, Bool_t , Int_t InvMassType);  // Fits the Invariant Mass histos with a given function
void FitTrueInvMassInPtBins(TH1D * , Double_t *fMesonIntDeltaRangeFit, Int_t, Bool_t,Int_t);  // Fits the Invariant Mass histos with a given function
void FitCBSubtractedInvMassInPtBins(TH1D * , Double_t *fMesonIntDeltaRangeFit, Int_t, Bool_t , TString , Int_t InvMassType);  // Fits the Invariant Mass histos with a given function;
void FitWithPol2ForBG(TH1D*, Double_t*fMesonFitRangeCur, Int_t, Bool_t,Int_t);
// void ProduceBckProperWeighting(TList*, TList* );
void ProduceBckWithoutWeighting(TH2D **, TH2D **fBckInvMassVSPtDummy_SubPiZero = NULL, TH2D **fBckInvMassVSPtDummy_FixedPzPiZero = NULL);
void ProduceBckWithoutWeightingMinimal(TH2D *,TH1D **);
void IntegrateHistoInvMassStream(TH1D * , Double_t *);
void IntegrateHistoInvMass(TH1D * , Double_t *);
void IntegrateFitFunc(TF1 * , TH1D *, Double_t *);
void FillHistosArrayMC(TH1D* , TH1D*, TH1D* ,TString);
void CalculateMesonAcceptance();
TH1D* CalculateMesonEfficiency(TH1D*, TH1D*, TString);
void SaveHistos(Int_t, TString, TString);
void SaveCorrectionHistos(TString , TString);
void Initialize(TString setPi0, Int_t);
void CalculateFWHM(TF1 *, Double_t *InputMesonFitRange, Bool_t useGaussOnly = kFALSE);
Double_t LinearBGExclusion(Double_t *,Double_t *);                                                          // Definition of linear BG with excluded region
Double_t Pol2BGExclusion(Double_t *,Double_t *);                                                          // Definition of pol2 BG with excluded region
Double_t CrystalBallBck(Double_t *,Double_t *);
Double_t CrystalBall(Double_t *,Double_t *);
void SetCorrectMCHistogrammNames();
Int_t SetMesonIntDeltaRange(Int_t ptBinNumber, Int_t FitFunctionNumber=0, Int_t ParameterNumber=0, Int_t iDebugLevel=0);
void Delete();

TString centralityString = 										"";

void InitializeWindows(TString setPi0, Int_t mode, TString trigger, Int_t triggerSet = -1){

    fPeakRange                         = new Double_t[2];
    fPeakRange_SubPiZero               = new Double_t[2];
    fPeakRange_FixedPzPiZero           = new Double_t[2];
    fIntFixedRange                     = new Double_t[2]; // not yet implemented
    fFitRange                          = new Double_t[2];
    fFitRange_SubPiZero                = new Double_t[2];
    fFitRange_FixedPzPiZero            = new Double_t[2];
    fBGFitRange                        = new Double_t[2];
    fBGFitRangeLeft                    = new Double_t[2];
    fBGFitRangeLeft_SubPiZero          = new Double_t[2];
    fBGFitRangeLeft_FixedPzPiZero      = new Double_t[2];
    fBGFitRange_ContamintationFit      = new Double_t[2];
    fBGFitRange_SubPiZero_ContamintationFit = new Double_t[2];
    fBGFitRange_FixedPzPiZero_ContamintationFit = new Double_t[2];
    fBGFitRangeLeft_ContamintationFit  = new Double_t[2];
    fBGFitRangeLeft_SubPiZero_ContamintationFit = new Double_t[2];
    fBGFitRangeLeft_FixedPzPiZero_ContamintationFit = new Double_t[2];
    fMesonPlotRange                    = new Double_t[2];
    fMesonIntDeltaRange                = new Double_t[2];
    fMesonIntDeltaRangeWide            = new Double_t[2];
    fMesonIntDeltaRangeNarrow          = new Double_t[2];
    fMesonMassRange                    = new Double_t[2];
    fMesonMassRange_SubPiZero          = new Double_t[2];
    fMesonMassRange_FixedPzPiZero      = new Double_t[2];
    fMesonMassPlotRange                = new Double_t[2];
    fMesonMassPlotRange_SubPiZero      = new Double_t[2];
    fMesonMassPlotRange_FixedPzPiZero  = new Double_t[2];
    fMesonFitRange                     = new Double_t[2];
    fMesonFitRange_SubPiZero           = new Double_t[2];
    fMesonFitRange_FixedPzPiZero       = new Double_t[2];
    fMesonWidthRange                   = new Double_t[2];
    fMesonWidthRangeTrue               = new Double_t[2];
    fMesonLambdaTailRange              = new Double_t[2];
    fMesonLambdaTailRangeTrue          = new Double_t[2];
    fMidPt                             = new Double_t[2];
    fMesonWidthRangeMC                 = new Double_t[2];
    fFullPt                            = new Double_t[2];
    //****************************************************************************************************
    // Initialization for Omega meson
    //****************************************************************************************************
    if (setPi0.CompareTo("Omega") == 0){

        // set meson ID according to PDG
        fMesonId                  = 223;

        // what function is used to fit total background (alternative to event mixing)
        fTotalBackFitMode = 3; // 0: pol1 only; 1: pol2 only; 2: gauss+pol1; 3: gauss+pol2; 4: RooFit; 5: pol3; 6: gauss+pol3; 7: exp only; 8: gauss+exp
        fFitRangeForMaximumMode = 0; //Meson Range
        if (fEnergyFlag.Contains("13TeV")){
            if(mode == 60 || mode == 61 || mode == 62 || mode == 64 || mode == 65){
               fTotalBackFitMode = 6;
               fFitRangeForMaximumMode = 0; //1 for pure fit
            }
        }

        // set medium pt range (currently for all modes the same)
        if(mode == 40 || mode == 41 || mode == 42 || mode == 44 || mode == 45){
           fMidPt[0]                   = 1.5;
           fMidPt[1]                   = 2.5;
        }else if(mode == 60 || mode == 61 || mode == 62 || mode == 64 || mode == 65){
           fMidPt[0]                   = 1.5;
           fMidPt[1]                   = 2.5;
         }

        // Initialize peak range
        if(mode == 40 || mode == 41 || mode == 42 || mode == 44 || mode == 45 ||
           mode == 60 || mode == 61 || mode == 62 || mode == 64 || mode == 65){
             fPeakRange[0]               = 0.75;
             fPeakRange[1]               = 0.84;

             fPeakRange_SubPiZero[0]     = fPeakRange[0];
             fPeakRange_SubPiZero[1]     = fPeakRange[1];

             fPeakRange_FixedPzPiZero[0] = fPeakRange[0];
             fPeakRange_FixedPzPiZero[1] = fPeakRange[1];
         }

        // Initialze fit range
        if(mode == 40 || mode == 41 || mode == 42 || mode == 44 || mode == 45 ||
           mode == 60 || mode == 61 || mode == 62 || mode == 64 || mode == 65){
            fFitRange[0]             = 0.60;
            fFitRange[1]             = 0.99;
            if (fEnergyFlag.Contains("13TeV")){
                fFitRange[0]             = 0.60;
                fFitRange[1]             = 0.99;
            }
            // fFitRange[0]             = 0.60;
            // fFitRange[1]             = 0.89;
            fFitRange_SubPiZero[0]   = fFitRange[0];
            fFitRange_SubPiZero[1]   = fFitRange[1];
            fFitRange_FixedPzPiZero[0]             = fFitRange[0];
            fFitRange_FixedPzPiZero[1]             = fFitRange[1];
            fIntFixedRange[0]        = 0.615; // not yet implemented
            fIntFixedRange[1]        = 0.89;  // not yet implemented
        }

        // Initialize default BG fit range right & left
        if(mode == 40 || mode == 60){ //PCM
            fBGFitRange[0]                = 0.83;
            fBGFitRange[1]                = 0.88;
            fBGFitRange_SubPiZero[0]      = fBGFitRange[0]; //
            fBGFitRange_SubPiZero[1]      = fBGFitRange[1];
            fBGFitRange_FixedPzPiZero[0]  = fBGFitRange[0];
            fBGFitRange_FixedPzPiZero[1]  = fBGFitRange[1];
            fBGFitRangeLeft[0]         = 0.695;
            fBGFitRangeLeft[1]         = 0.73;
            fBGFitRangeLeft_SubPiZero[0]             = fBGFitRangeLeft[0];
            fBGFitRangeLeft_SubPiZero[1]             = fBGFitRangeLeft[1];
            fBGFitRangeLeft_FixedPzPiZero[0]         = fBGFitRangeLeft[0];
            fBGFitRangeLeft_FixedPzPiZero[1]         = fBGFitRangeLeft[1];

            fBGFitRange_ContamintationFit[0]                        = fBGFitRange[0];
            fBGFitRange_ContamintationFit[1]                        = fBGFitRange[1];
            fBGFitRange_SubPiZero_ContamintationFit[0]              = fBGFitRange_ContamintationFit[0]; //
            fBGFitRange_SubPiZero_ContamintationFit[1]              = fBGFitRange_ContamintationFit[1];
            fBGFitRange_FixedPzPiZero_ContamintationFit[0]          = fBGFitRange_ContamintationFit[0];
            fBGFitRange_FixedPzPiZero_ContamintationFit[1]          = fBGFitRange_ContamintationFit[1];
            fBGFitRangeLeft_ContamintationFit[0]                    = fBGFitRangeLeft[0];
            fBGFitRangeLeft_ContamintationFit[1]                    = fBGFitRangeLeft[1];
            fBGFitRangeLeft_SubPiZero_ContamintationFit[0]          = fBGFitRangeLeft_ContamintationFit[0];
            fBGFitRangeLeft_SubPiZero_ContamintationFit[1]          = fBGFitRangeLeft_ContamintationFit[1];
            fBGFitRangeLeft_FixedPzPiZero_ContamintationFit[0]      = fBGFitRangeLeft_ContamintationFit[0];
            fBGFitRangeLeft_FixedPzPiZero_ContamintationFit[1]      = fBGFitRangeLeft_ContamintationFit[1];
        } else if (mode == 41 || mode == 61){ //PCM-EMCAL
            fBGFitRange[0]                = 0.825;
            fBGFitRange[1]                = 0.865;
            fBGFitRange_SubPiZero[0]      = fBGFitRange[0]; //
            fBGFitRange_SubPiZero[1]      = fBGFitRange[1];
            fBGFitRange_FixedPzPiZero[0]  = fBGFitRange[0];
            fBGFitRange_FixedPzPiZero[1]  = fBGFitRange[1];
            fBGFitRangeLeft[0]         = 0.67;
            fBGFitRangeLeft[1]         = 0.71;
            fBGFitRangeLeft_SubPiZero[0]             = fBGFitRangeLeft[0];
            fBGFitRangeLeft_SubPiZero[1]             = fBGFitRangeLeft[1];
            fBGFitRangeLeft_FixedPzPiZero[0]         = fBGFitRangeLeft[0];
            fBGFitRangeLeft_FixedPzPiZero[1]         = fBGFitRangeLeft[1];

            fBGFitRange_ContamintationFit[0]                        = fBGFitRange[0];
            fBGFitRange_ContamintationFit[1]                        = fBGFitRange[1];
            fBGFitRange_SubPiZero_ContamintationFit[0]              = fBGFitRange_ContamintationFit[0]; //
            fBGFitRange_SubPiZero_ContamintationFit[1]              = fBGFitRange_ContamintationFit[1];
            fBGFitRange_FixedPzPiZero_ContamintationFit[0]          = fBGFitRange_ContamintationFit[0];
            fBGFitRange_FixedPzPiZero_ContamintationFit[1]          = fBGFitRange_ContamintationFit[1];
            fBGFitRangeLeft_ContamintationFit[0]                    = fBGFitRangeLeft[0];
            fBGFitRangeLeft_ContamintationFit[1]                    = fBGFitRangeLeft[1];
            fBGFitRangeLeft_SubPiZero_ContamintationFit[0]          = fBGFitRangeLeft_ContamintationFit[0];
            fBGFitRangeLeft_SubPiZero_ContamintationFit[1]          = fBGFitRangeLeft_ContamintationFit[1];
            fBGFitRangeLeft_FixedPzPiZero_ContamintationFit[0]      = fBGFitRangeLeft_ContamintationFit[0];
            fBGFitRangeLeft_FixedPzPiZero_ContamintationFit[1]      = fBGFitRangeLeft_ContamintationFit[1];
        } else if (mode == 42 || mode == 62){ //PCM-PHOS
            fBGFitRange[0]                = 0.815;
            fBGFitRange[1]                = 0.85;
            fBGFitRange_SubPiZero[0]      = fBGFitRange[0]; //
            fBGFitRange_SubPiZero[1]      = fBGFitRange[1];
            fBGFitRange_FixedPzPiZero[0]  = fBGFitRange[0];
            fBGFitRange_FixedPzPiZero[1]  = fBGFitRange[1];
            fBGFitRangeLeft[0]         = 0.71;
            fBGFitRangeLeft[1]         = 0.755;
            fBGFitRangeLeft_SubPiZero[0]             = fBGFitRangeLeft[0];
            fBGFitRangeLeft_SubPiZero[1]             = fBGFitRangeLeft[1];
            fBGFitRangeLeft_FixedPzPiZero[0]         = fBGFitRangeLeft[0];
            fBGFitRangeLeft_FixedPzPiZero[1]         = fBGFitRangeLeft[1];

            fBGFitRange_ContamintationFit[0]                        = fBGFitRange[0];
            fBGFitRange_ContamintationFit[1]                        = fBGFitRange[1];
            fBGFitRange_SubPiZero_ContamintationFit[0]              = fBGFitRange_ContamintationFit[0]; //
            fBGFitRange_SubPiZero_ContamintationFit[1]              = fBGFitRange_ContamintationFit[1];
            fBGFitRange_FixedPzPiZero_ContamintationFit[0]          = fBGFitRange_ContamintationFit[0];
            fBGFitRange_FixedPzPiZero_ContamintationFit[1]          = fBGFitRange_ContamintationFit[1];
            fBGFitRangeLeft_ContamintationFit[0]                    = fBGFitRangeLeft[0];
            fBGFitRangeLeft_ContamintationFit[1]                    = fBGFitRangeLeft[1];
            fBGFitRangeLeft_SubPiZero_ContamintationFit[0]          = fBGFitRangeLeft_ContamintationFit[0];
            fBGFitRangeLeft_SubPiZero_ContamintationFit[1]          = fBGFitRangeLeft_ContamintationFit[1];
            fBGFitRangeLeft_FixedPzPiZero_ContamintationFit[0]      = fBGFitRangeLeft_ContamintationFit[0];
            fBGFitRangeLeft_FixedPzPiZero_ContamintationFit[1]      = fBGFitRangeLeft_ContamintationFit[1];
        } else if (mode == 44 || mode == 64){ //EMCAL
            //fBGFitRange[0]                = 0.88;
            //fBGFitRange[1]                = 0.97;
            fBGFitRange[0]                = 0.83;
            fBGFitRange[1]                = 0.86;
            fBGFitRange_SubPiZero[0]      = fBGFitRange[0]; //
            fBGFitRange_SubPiZero[1]      = fBGFitRange[1];
            fBGFitRange_FixedPzPiZero[0]  = fBGFitRange[0];
            fBGFitRange_FixedPzPiZero[1]  = fBGFitRange[1];
            fBGFitRangeLeft[0]         = 0.66;
            fBGFitRangeLeft[1]         = 0.71;
            fBGFitRangeLeft_SubPiZero[0]             = fBGFitRangeLeft[0];
            fBGFitRangeLeft_SubPiZero[1]             = fBGFitRangeLeft[1];
            fBGFitRangeLeft_FixedPzPiZero[0]         = fBGFitRangeLeft[0];
            fBGFitRangeLeft_FixedPzPiZero[1]         = fBGFitRangeLeft[1];

            //For Background: Combination and Contamination
            fBGFitRangeLeft_ContamintationFit[0]                    = 0.6; //fBGFitRangeLeft[0];
            fBGFitRangeLeft_ContamintationFit[1]                    = 0.70;
            fBGFitRangeLeft_SubPiZero_ContamintationFit[0]          = fBGFitRangeLeft_ContamintationFit[0];
            fBGFitRangeLeft_SubPiZero_ContamintationFit[1]          = fBGFitRangeLeft_ContamintationFit[1];
            fBGFitRangeLeft_FixedPzPiZero_ContamintationFit[0]      = fBGFitRangeLeft_ContamintationFit[0];
            fBGFitRangeLeft_FixedPzPiZero_ContamintationFit[1]      = fBGFitRangeLeft_ContamintationFit[1];

            fBGFitRange_ContamintationFit[0]                        = 0.85;
            fBGFitRange_ContamintationFit[1]                        = 0.95; //fBGFitRange[1];
            fBGFitRange_SubPiZero_ContamintationFit[0]              = fBGFitRange_ContamintationFit[0]; //
            fBGFitRange_SubPiZero_ContamintationFit[1]              = fBGFitRange_ContamintationFit[1];
            fBGFitRange_FixedPzPiZero_ContamintationFit[0]          = fBGFitRange_ContamintationFit[0];
            fBGFitRange_FixedPzPiZero_ContamintationFit[1]          = fBGFitRange_ContamintationFit[1];

        } else if (mode == 45 || mode == 65){ //PHOS
            fBGFitRange[0]                = 0.81;
            fBGFitRange[1]                = 0.85;
            fBGFitRange_SubPiZero[0]      = fBGFitRange[0]; //
            fBGFitRange_SubPiZero[1]      = fBGFitRange[1];
            fBGFitRange_FixedPzPiZero[0]  = fBGFitRange[0];
            fBGFitRange_FixedPzPiZero[1]  = fBGFitRange[1];
            fBGFitRangeLeft[0]         = 0.66;
            fBGFitRangeLeft[1]         = 0.71;
            fBGFitRangeLeft_SubPiZero[0]             = fBGFitRangeLeft[0];
            fBGFitRangeLeft_SubPiZero[1]             = fBGFitRangeLeft[1];
            fBGFitRangeLeft_FixedPzPiZero[0]         = fBGFitRangeLeft[0];
            fBGFitRangeLeft_FixedPzPiZero[1]         = fBGFitRangeLeft[1];

            fBGFitRange_ContamintationFit[0]                        = fBGFitRange[0];
            fBGFitRange_ContamintationFit[1]                        = fBGFitRange[1];
            fBGFitRange_SubPiZero_ContamintationFit[0]              = fBGFitRange_ContamintationFit[0]; //
            fBGFitRange_SubPiZero_ContamintationFit[1]              = fBGFitRange_ContamintationFit[1];
            fBGFitRange_FixedPzPiZero_ContamintationFit[0]          = fBGFitRange_ContamintationFit[0];
            fBGFitRange_FixedPzPiZero_ContamintationFit[1]          = fBGFitRange_ContamintationFit[1];
            fBGFitRangeLeft_ContamintationFit[0]                    = fBGFitRangeLeft[0];
            fBGFitRangeLeft_ContamintationFit[1]                    = fBGFitRangeLeft[1];
            fBGFitRangeLeft_SubPiZero_ContamintationFit[0]          = fBGFitRangeLeft_ContamintationFit[0];
            fBGFitRangeLeft_SubPiZero_ContamintationFit[1]          = fBGFitRangeLeft_ContamintationFit[1];
            fBGFitRangeLeft_FixedPzPiZero_ContamintationFit[0]      = fBGFitRangeLeft_ContamintationFit[0];
            fBGFitRangeLeft_FixedPzPiZero_ContamintationFit[1]      = fBGFitRangeLeft_ContamintationFit[1];
        }

        // Initialize default Plot range for meson
        if(mode == 40 || mode == 41 || mode == 42 || mode == 44 || mode == 45 ||
           mode == 60 || mode == 61 || mode == 62 || mode == 64 || mode == 65){
            fMesonPlotRange[0]         = 0.75;
            fMesonPlotRange[1]         = 0.79;
        }

        // Initialize default Plot default integration ranges
        if(mode == 40 || mode == 60){ //PCM
            fMesonIntDeltaRange[0]      = -0.013*2; // determined using 2 * sigma in MC
            fMesonIntDeltaRange[1]      =  0.013*2;
            fMesonIntDeltaRangeWide[0]  = fMesonIntDeltaRange[0] * 1.5;
            fMesonIntDeltaRangeWide[1]  = fMesonIntDeltaRange[1] * 1.5;
            fMesonIntDeltaRangeNarrow[0]= fMesonIntDeltaRange[0] * 0.7;
            fMesonIntDeltaRangeNarrow[1]= fMesonIntDeltaRange[1] * 0.7;
        } else if(mode == 41 || mode == 61){ //PCM-EMCAL
            fMesonIntDeltaRange[0]      = -0.013*2;
            fMesonIntDeltaRange[1]      =  0.013*2;
            fMesonIntDeltaRangeWide[0]  = fMesonIntDeltaRange[0] * 1.5;
            fMesonIntDeltaRangeWide[1]  = fMesonIntDeltaRange[1] * 1.5;
            fMesonIntDeltaRangeNarrow[0]= fMesonIntDeltaRange[0] * 0.7;
            fMesonIntDeltaRangeNarrow[1]= fMesonIntDeltaRange[1] * 0.7;
        } else if(mode == 42 || mode == 62){ //PCM-PHOS
            fMesonIntDeltaRange[0]      = -0.03;
            fMesonIntDeltaRange[1]      =  0.03;
            fMesonIntDeltaRangeWide[0]  = -0.05;
            fMesonIntDeltaRangeWide[1]  = 0.05;
            fMesonIntDeltaRangeNarrow[0]= -0.02;
            fMesonIntDeltaRangeNarrow[1]= 0.02;
        } else if(mode == 44 || mode == 64){ //EMCAL
            // fMesonIntDeltaRange[0]      = -0.019*2;
            // fMesonIntDeltaRange[1]      =  0.019*2;
            // FWHM should 0.027
            fMesonIntDeltaRange[0]      = -0.030;
            fMesonIntDeltaRange[1]      =  0.030;
            // fMesonIntDeltaRange[0]      = -0.023*2;//ok !
            // fMesonIntDeltaRange[1]      =  0.021*2;//ok !
            // fMesonIntDeltaRange[0]      = -0.025*2;//ok
            // fMesonIntDeltaRange[1]      =  0.021*2;//ok
            fMesonIntDeltaRangeWide[0]  = fMesonIntDeltaRange[0] * 1.5;
            fMesonIntDeltaRangeWide[1]  = fMesonIntDeltaRange[1] * 1.5;
            // fMesonIntDeltaRangeNarrow[0]= fMesonIntDeltaRange[0] * 0.6;
            fMesonIntDeltaRangeNarrow[0]= fMesonIntDeltaRange[0] * 0.7;
            fMesonIntDeltaRangeNarrow[1]= fMesonIntDeltaRange[1] * 0.7;

            useFitIntDeltaRange = kTRUE;
            useFitIntDeltaRange_FunctionNumber = 0;
            useFitIntDeltaRange_ParameterNumber = 0;
        } else if( mode == 45 || mode == 65){ //PHOS
            fMesonIntDeltaRange[0]      = -0.04;
            fMesonIntDeltaRange[1]      =  0.04;
            fMesonIntDeltaRangeWide[0]  = -0.06;
            fMesonIntDeltaRangeWide[1]  = 0.08;
            fMesonIntDeltaRangeNarrow[0]= -0.03;
            fMesonIntDeltaRangeNarrow[1]= 0.03;
        }

        // Set meson mass ranges (here same for fitting and plotting)
        if(mode == 40 || mode == 42 ||  mode == 45 ||
           mode == 60 || mode == 62 ||  mode == 65){
            fMesonMassPlotRange[0]                = 0.65;
            fMesonMassPlotRange[1]                = 0.89;
            fMesonMassPlotRange_SubPiZero[0]      = fMesonMassPlotRange[0];
            fMesonMassPlotRange_SubPiZero[1]      = fMesonMassPlotRange[1];
            fMesonMassPlotRange_FixedPzPiZero[0]  = fMesonMassPlotRange[0];
            fMesonMassPlotRange_FixedPzPiZero[1]  = fMesonMassPlotRange[1];

            fMesonMassRange[0]          = 0.65;
            fMesonMassRange[1]          = 0.89;
            fMesonMassRange_SubPiZero[0]          = fMesonMassRange[0];
            fMesonMassRange_SubPiZero[1]          = fMesonMassRange[1];
            fMesonMassRange_FixedPzPiZero[0]      = fMesonMassRange[0];
            fMesonMassRange_FixedPzPiZero[1]      = fMesonMassRange[1];
        } else if(mode == 41 || mode == 61){
            fMesonMassPlotRange[0]                = 0.65;
            fMesonMassPlotRange[1]                = 0.89;
            fMesonMassPlotRange_SubPiZero[0]      = fMesonMassPlotRange[0];
            fMesonMassPlotRange_SubPiZero[1]      = fMesonMassPlotRange[1];
            fMesonMassPlotRange_FixedPzPiZero[0]  = fMesonMassPlotRange[0];
            fMesonMassPlotRange_FixedPzPiZero[1]  = fMesonMassPlotRange[1];

            fMesonMassRange[0]          = 0.66;
            fMesonMassRange[1]          = 0.89;
            fMesonMassRange_SubPiZero[0]          = fMesonMassRange[0];
            fMesonMassRange_SubPiZero[1]          = fMesonMassRange[1];
            fMesonMassRange_FixedPzPiZero[0]      = fMesonMassRange[0];
            fMesonMassRange_FixedPzPiZero[1]      = fMesonMassRange[1];
        } else if(mode == 44 || mode == 64){ //EMCAL
            fMesonMassPlotRange[0]                = 0.6;
            fMesonMassPlotRange[1]                = 0.99;
            fMesonMassPlotRange_SubPiZero[0]      = fMesonMassPlotRange[0];
            fMesonMassPlotRange_SubPiZero[1]      = fMesonMassPlotRange[1];
            fMesonMassPlotRange_FixedPzPiZero[0]  = fMesonMassPlotRange[0];
            fMesonMassPlotRange_FixedPzPiZero[1]  = fMesonMassPlotRange[1];

            fMesonMassRange[0]          = 0.6;
            fMesonMassRange[1]          = 0.99;
            fMesonMassRange_SubPiZero[0]          = fMesonMassRange[0];
            fMesonMassRange_SubPiZero[1]          = fMesonMassRange[1];
            fMesonMassRange_FixedPzPiZero[0]      = fMesonMassRange[0];
            fMesonMassRange_FixedPzPiZero[1]      = fMesonMassRange[1];
        }

         // Set meson fit range
         if(mode == 40 || mode == 60){
             fMesonFitRange[0]                 = 0.60;
             fMesonFitRange[1]                 = 0.99;

             fMesonFitRange_SubPiZero[0]       = fMesonFitRange[0];
             fMesonFitRange_SubPiZero[1]       = fMesonFitRange[1];

             fMesonFitRange_FixedPzPiZero[0]   = fMesonFitRange[0];
             fMesonFitRange_FixedPzPiZero[1]   = fMesonFitRange[1];
         } else if (mode == 41 || mode == 61){
             fMesonFitRange[0]                 = 0.60;
             fMesonFitRange[1]                 = 0.99;

             fMesonFitRange_SubPiZero[0]       = fMesonFitRange[0];
             fMesonFitRange_SubPiZero[1]       = fMesonFitRange[1];

             fMesonFitRange_FixedPzPiZero[0]   = fMesonFitRange[0];
             fMesonFitRange_FixedPzPiZero[1]   = fMesonFitRange[1];
         } else if (mode == 42 || mode == 62){ //PCM-PHOS
             fMesonFitRange[0]                 = 0.68;
             fMesonFitRange[1]                 = 0.85;

             fMesonFitRange_SubPiZero[0]       = fMesonFitRange[0];
             fMesonFitRange_SubPiZero[1]       = fMesonFitRange[1];

             fMesonFitRange_FixedPzPiZero[0]   = fMesonFitRange[0];
             fMesonFitRange_FixedPzPiZero[1]   = fMesonFitRange[1];
         } else if (mode == 44 || mode == 64){
             fMesonFitRange[0]                 = 0.60;
             fMesonFitRange[1]                 = 0.99;

             fMesonFitRange_SubPiZero[0]       = fMesonFitRange[0];
             fMesonFitRange_SubPiZero[1]       = fMesonFitRange[1];

             fMesonFitRange_FixedPzPiZero[0]   = fMesonFitRange[0] ;
             fMesonFitRange_FixedPzPiZero[1]   = fMesonFitRange[1] ;

         } else if (mode == 45 || mode == 65){ //PHOS
             fMesonFitRange[0]                 = 0.70;
             fMesonFitRange[1]                 = 0.87;

             fMesonFitRange_SubPiZero[0]       = fMesonFitRange[0];
             fMesonFitRange_SubPiZero[1]       = fMesonFitRange[1];

             fMesonFitRange_FixedPzPiZero[0]   = fMesonFitRange[0] ;
             fMesonFitRange_FixedPzPiZero[1]   = fMesonFitRange[1] ;
         }

         // Set remaining parameters for fitting
         if(mode == 40 || mode == 60){ //PCM
             FitRangeSigBckRatioOption   = 20; //xy; x: 1== Gaus, 2==GausExp ;y: 0 == wide range, 1 == narrow range, 2 = mid range
             usePolNForBackgroundScaling = 1;
             fMesonWidthExpect            = 0.010;
             fMesonWidthRange[0]          = 0.001;
             fMesonWidthRange[1]          = 0.100;
             fMesonWidthRangeTrue[0]      = 0.005;
             fMesonWidthRangeTrue[1]      = 0.100;
             fMesonLambdaTail             = 0.0007;
             fMesonLambdaTailTrue         = 0.0007;
             fMesonLambdaTailRange[0]     = 0.0007;
             fMesonLambdaTailRange[1]     = 0.0007;
             fMesonLambdaTailRangeTrue[0] = 0.0005;
             fMesonLambdaTailRangeTrue[1] = 0.0020;

             fFullPt[0]                   = 0.4;
             fFullPt[1]                   = 15;

             // Settings for MC
             fMesonLambdaTailMC    = fMesonLambdaTail;
             fMesonWidthExpectMC   = fMesonWidthExpect;
             fMesonWidthRangeMC[0] = fMesonWidthRange[0];
             fMesonWidthRangeMC[1] = fMesonWidthRange[1];

         } else if(mode == 41 || mode == 61){ //PCM-EMCAL
             FitRangeSigBckRatioOption   = 20; //xy; x: 1== Gaus, 2==GausExp ;y: 0 == wide range, 1 == narrow range, 2 = mid range
             usePolNForBackgroundScaling = 1;
             fMesonWidthExpect           = 0.012;
             fMesonWidthRange[0]         = 0.001;
             fMesonWidthRange[1]         = 0.055;
             fMesonWidthRangeTrue[0]      = 0.005;
             fMesonWidthRangeTrue[1]      = 0.100;
             fMesonLambdaTail            = 0.0007;
             fMesonLambdaTailRange[0]    = 0.0007;
             fMesonLambdaTailRange[1]    = 0.0007;
             fMesonLambdaTailTrue         = 0.0007;
             fMesonLambdaTailRangeTrue[0] = 0.0005;
             fMesonLambdaTailRangeTrue[1] = 0.0020;

             fFullPt[0]                  = 0.4;
             fFullPt[1]                  = 15;

             // Settings for MC
             fMesonLambdaTailMC    = fMesonLambdaTail;
             fMesonWidthExpectMC   = fMesonWidthExpect;
             fMesonWidthRangeMC[0] = fMesonWidthRange[0];
             fMesonWidthRangeMC[1] = fMesonWidthRange[1];
         } else if(mode == 42 || mode == 62){ //PCM-PHOS
             FitRangeSigBckRatioOption   = 20; //xy; x: 1== Gaus, 2==GausExp ;y: 0 == wide range, 1 == narrow range, 2 = mid range
             usePolNForBackgroundScaling = 1;
             fMesonWidthExpect           = 0.01;
             fMesonWidthRange[0]         = 0.001;
             fMesonWidthRange[1]         = 0.070;
             fMesonWidthRangeTrue[0]      = 0.005;
             fMesonWidthRangeTrue[1]      = 0.100;
             fMesonLambdaTail            = 0.0007;
             fMesonLambdaTailRange[0]    = 0.0007;
             fMesonLambdaTailRange[1]    = 0.0007;
             fMesonLambdaTailTrue         = 0.0007;
             fMesonLambdaTailRangeTrue[0] = 0.0005;
             fMesonLambdaTailRangeTrue[1] = 0.0020;

             fFullPt[0]                  = 0.4;
             fFullPt[1]                  = 15;

             // Settings for MC
             fMesonLambdaTailMC    = fMesonLambdaTail;
             fMesonWidthExpectMC   = fMesonWidthExpect;
             fMesonWidthRangeMC[0] = fMesonWidthRange[0];
             fMesonWidthRangeMC[1] = fMesonWidthRange[1];
         } else if(mode == 44 || mode == 64){ //EMCAL
             FitRangeSigBckRatioOption   = 23; //xy; x: 1== Gaus, 2==GausExp ;y: 0 == wide range, 1 == narrow range, 2 = mid range
             usePolNForBackgroundScaling = 2;
             fMesonWidthExpect           = 0.06;
             fMesonWidthRange[0]         = 0.005;
             fMesonWidthRange[1]         = 0.030;
             fMesonWidthRangeTrue[0]      = 0.005;
             fMesonWidthRangeTrue[1]      = 0.100;
             fMesonLambdaTail            = 0.0007;
             fMesonLambdaTailRange[0]    = 0.0007;
             fMesonLambdaTailRange[1]    = 0.0007;
             fMesonLambdaTailTrue         = 0.0007;
             fMesonLambdaTailRangeTrue[0] = 0.0005;
             fMesonLambdaTailRangeTrue[1] = 0.0020;

             fFullPt[0]                  = 0.4;
             fFullPt[1]                  = 15;

             if (fEnergyFlag.Contains("13TeV")){
                 fMesonWidthRange[1]         = 0.050;
             }

             // Settings for MC
             fMesonLambdaTailMC    = fMesonLambdaTail;
             fMesonWidthExpectMC   = fMesonWidthExpect;
             fMesonWidthRangeMC[0] = fMesonWidthRange[0];
             fMesonWidthRangeMC[1] = fMesonWidthRange[1];
         } else if(mode == 45 || mode == 65){ //PHOS
             FitRangeSigBckRatioOption   = 20; //xy; x: 1== Gaus, 2==GausExp ;y: 0 == wide range, 1 == narrow range, 2 = mid range
             usePolNForBackgroundScaling = 1;
             fMesonWidthExpect           = 0.040;
             fMesonWidthRange[0]         = 0.005;
             fMesonWidthRange[1]         = 0.120;
             fMesonWidthRangeTrue[0]      = 0.005;
             fMesonWidthRangeTrue[1]      = 0.100;
             fMesonLambdaTail            = 0.0007;
             fMesonLambdaTailTrue         = 0.0007;
             fMesonLambdaTailRange[0]    = 0.0007;
             fMesonLambdaTailRange[1]    = 0.0007;
             fMesonLambdaTailRangeTrue[0] = 0.0005;
             fMesonLambdaTailRangeTrue[1] = 0.0020;

             fFullPt[0]                  = 0.4;
             fFullPt[1]                  = 15;

             // Settings for MC
             fMesonLambdaTailMC    = fMesonLambdaTail;
             fMesonWidthExpectMC   = fMesonWidthExpect;
             fMesonWidthRangeMC[0] = fMesonWidthRange[0];
             fMesonWidthRangeMC[1] = fMesonWidthRange[1];
        }
    }
    if (setPi0.CompareTo("EtaPrime") == 0){

        // set meson ID according to PDG
        fMesonId                  = 331;

        if(mode == 60 || mode == 61 || mode == 62 || mode == 64 || mode == 65){
        // set medium pt range (currently for all modes the same)
           fMidPt[0]                   = 1.5;
           fMidPt[1]                   = 2.5;

        // Initialize peak range
             fPeakRange[0]               = 0.86;
             fPeakRange[1]               = 1.06;

             fPeakRange_SubPiZero[0]     = fPeakRange[0];
             fPeakRange_SubPiZero[1]     = fPeakRange[1];

             fPeakRange_FixedPzPiZero[0] = fPeakRange[0];
             fPeakRange_FixedPzPiZero[1] = fPeakRange[1];

        // Initialze fit range
            fFitRange[0]             = 0.75;
            fFitRange[1]             = 1.1;
            fFitRange_SubPiZero[0]   = fFitRange[0];
            fFitRange_SubPiZero[1]   = fFitRange[1];
            fFitRange_FixedPzPiZero[0]             = fFitRange[0];
            fFitRange_FixedPzPiZero[1]             = fFitRange[1];
            fIntFixedRange[0]        = 0.75; // not yet implemented
            fIntFixedRange[1]        = 1.1;  // not yet implemented



            // Initialize default BG fit range right & left
            fBGFitRange[0]                = 1.07;
            fBGFitRange[1]                = 1.1;
            fBGFitRange_SubPiZero[0]      = fBGFitRange[0]; //
            fBGFitRange_SubPiZero[1]      = fBGFitRange[1];
            fBGFitRange_FixedPzPiZero[0]  = fBGFitRange[0];
            fBGFitRange_FixedPzPiZero[1]  = fBGFitRange[1];
            fBGFitRangeLeft[0]         = 0.8;
            fBGFitRangeLeft[1]         = 0.85;
            fBGFitRangeLeft_SubPiZero[0]             = fBGFitRangeLeft[0];
            fBGFitRangeLeft_SubPiZero[1]             = fBGFitRangeLeft[1];
            fBGFitRangeLeft_FixedPzPiZero[0]         = fBGFitRangeLeft[0];
            fBGFitRangeLeft_FixedPzPiZero[1]         = fBGFitRangeLeft[1];



        // Initialize default Plot range for meson
            fMesonPlotRange[0]         = 0.6;
            fMesonPlotRange[1]         = 1.2;

        // Initialize default Plot default integration ranges
            fMesonIntDeltaRange[0]      = -0.1;
            fMesonIntDeltaRange[1]      =  0.1;
            fMesonIntDeltaRangeWide[0]  = -0.15;
            fMesonIntDeltaRangeWide[1]  = 0.15;
            fMesonIntDeltaRangeNarrow[0]= -0.08;
            fMesonIntDeltaRangeNarrow[1]= 0.08;

        // Set meson mass ranges (here same for fitting and plotting)
             fMesonMassPlotRange[0]                = 0.6;
             fMesonMassPlotRange[1]                = 1.2;
             fMesonMassPlotRange_SubPiZero[0]      = fMesonMassPlotRange[0];
             fMesonMassPlotRange_SubPiZero[1]      = fMesonMassPlotRange[1];
             fMesonMassPlotRange_FixedPzPiZero[0]  = fMesonMassPlotRange[0];
             fMesonMassPlotRange_FixedPzPiZero[1]  = fMesonMassPlotRange[1];

             fMesonMassRange[0]          = 0.71;
             fMesonMassRange[1]          = 1.1;
             fMesonMassRange_SubPiZero[0]          = fMesonMassRange[0];
             fMesonMassRange_SubPiZero[1]          = fMesonMassRange[1];
             fMesonMassRange_FixedPzPiZero[0]      = fMesonMassRange[0];
             fMesonMassRange_FixedPzPiZero[1]      = fMesonMassRange[1];

             fMesonFitRange[0]                 = 0.75;
             fMesonFitRange[1]                 = 1.1;

             fMesonFitRange_SubPiZero[0]       = fMesonFitRange[0];
             fMesonFitRange_SubPiZero[1]       = fMesonFitRange[1];

             fMesonFitRange_FixedPzPiZero[0]   = fMesonFitRange[0];
             fMesonFitRange_FixedPzPiZero[1]   = fMesonFitRange[1];

         // Set remaining parameters for fitting
             fMesonWidthExpect            = 0.050;
             fMesonWidthRange[0]          = 0.002;
             fMesonWidthRange[1]          = 0.100;
             fMesonWidthRangeTrue[0]      = 0.005;
             fMesonWidthRangeTrue[1]      = 0.100;
             fMesonLambdaTail             = 0.0007;
             fMesonLambdaTailTrue         = 0.0007;
             fMesonLambdaTailRange[0]     = 0.0007;
             fMesonLambdaTailRange[1]     = 0.03;
             fMesonLambdaTailRangeTrue[0] = 0.0005;
             fMesonLambdaTailRangeTrue[1] = 0.03;

             fFullPt[0]                   = 0.4;
             fFullPt[1]                   = 15;

             // Settings for MC
             fMesonLambdaTailMC    = fMesonLambdaTail;
             fMesonWidthExpectMC   = fMesonWidthExpect;
             fMesonWidthRangeMC[0] = fMesonWidthRange[0];
             fMesonWidthRangeMC[1] = fMesonWidthRange[1];
         }
    }
    //****************************************************************************************************
    // Initialization for Eta meson
    //****************************************************************************************************
    if (setPi0.CompareTo("Eta") == 0){

        // set meson ID according to PDG
        fMesonId                  = 221;

        // set medium pt range (currently for all modes the same)
        if(mode == 40 || mode == 41 || mode == 42 || mode == 44 || mode == 45 ||
           mode == 60 || mode == 61 || mode == 62 || mode == 64 || mode == 65){
           fMidPt[0]                   = 1.5;
           fMidPt[1]                   = 2.5;
         }

        // Initialize peak range
        if(mode == 40 || mode == 41 || mode == 42 || mode == 44 || mode == 45 ||
           mode == 60 || mode == 61 || mode == 62 || mode == 64 || mode == 65){
             fPeakRange[0]                           = 0.48;
             fPeakRange[1]                           = 0.58;
             fPeakRange_SubPiZero[0]                 = fPeakRange[0];
             fPeakRange_SubPiZero[1]                 = fPeakRange[1];
             fPeakRange_FixedPzPiZero[0]             = fPeakRange[0];
             fPeakRange_FixedPzPiZero[1]             = fPeakRange[1];
         }

        // Initialze fit range
        if(mode == 40 || mode == 41 || mode == 42 || mode == 44 || mode == 45 ||
           mode == 60 || mode == 61 || mode == 62 || mode == 64 || mode == 65){
            fFitRange[0]                = 0.40;
            fFitRange[1]                = 0.65;
            fFitRange_SubPiZero[0]      = fFitRange[0] ;
            fFitRange_SubPiZero[1]      = fFitRange[1] ;
            fFitRange_FixedPzPiZero[0]  = fFitRange[0] ;
            fFitRange_FixedPzPiZero[1]  = fFitRange[1] ;
            fIntFixedRange[0]           = 0.48; // not yet implemented
            fIntFixedRange[1]           = 0.58;  // not yet implemented
        }

        // Initialize default BG fit range right & left
        if(mode == 40 || mode == 41 || mode == 45 ||
           mode == 60 || mode == 61 || mode == 65){
            fBGFitRange[0]                       = 0.557;
            fBGFitRange[1]                       = 0.57;
            fBGFitRange_SubPiZero[0]             = fBGFitRange[0];
            fBGFitRange_SubPiZero[1]             = fBGFitRange[1];
            fBGFitRange_FixedPzPiZero[0]         = fBGFitRange[0];
            fBGFitRange_FixedPzPiZero[1]         = fBGFitRange[1];
            fBGFitRangeLeft[0]                   = 0.48;
            fBGFitRangeLeft[1]                   = 0.52;
            fBGFitRangeLeft_SubPiZero[0]                       = fBGFitRangeLeft[0];
            fBGFitRangeLeft_SubPiZero[1]                       = fBGFitRangeLeft[1];
            fBGFitRangeLeft_FixedPzPiZero[0]                   = fBGFitRangeLeft[0];
            fBGFitRangeLeft_FixedPzPiZero[1]                   = fBGFitRangeLeft[1];
        } else if(mode == 42 || mode == 62){
            fBGFitRange[0]                       = 0.56;
            fBGFitRange[1]                       = 0.59;
            fBGFitRange_SubPiZero[0]             = fBGFitRange[0];
            fBGFitRange_SubPiZero[1]             = fBGFitRange[1];
            fBGFitRange_FixedPzPiZero[0]         = fBGFitRange[0];
            fBGFitRange_FixedPzPiZero[1]         = fBGFitRange[1];
            fBGFitRangeLeft[0]                   = 0.495;
            fBGFitRangeLeft[1]                   = 0.525;
            fBGFitRangeLeft_SubPiZero[0]                       = fBGFitRangeLeft[0];
            fBGFitRangeLeft_SubPiZero[1]                       = fBGFitRangeLeft[1];
            fBGFitRangeLeft_FixedPzPiZero[0]                   = fBGFitRangeLeft[0];
            fBGFitRangeLeft_FixedPzPiZero[1]                   = fBGFitRangeLeft[1];
        } else if(mode == 44 || mode == 64){
            fBGFitRange[0]                       = 0.57;
            fBGFitRange[1]                       = 0.64;
            fBGFitRange_SubPiZero[0]             = fBGFitRange[0];
            fBGFitRange_SubPiZero[1]             = fBGFitRange[1];
            fBGFitRange_FixedPzPiZero[0]         = fBGFitRange[0];
            fBGFitRange_FixedPzPiZero[1]         = fBGFitRange[1];
            fBGFitRangeLeft[0]                   = 0.48;
            fBGFitRangeLeft[1]                   = 0.52;
            fBGFitRangeLeft_SubPiZero[0]                       = fBGFitRangeLeft[0];
            fBGFitRangeLeft_SubPiZero[1]                       = fBGFitRangeLeft[1];
            fBGFitRangeLeft_FixedPzPiZero[0]                   = fBGFitRangeLeft[0];
            fBGFitRangeLeft_FixedPzPiZero[1]                   = fBGFitRangeLeft[1];
        }

        // Initialize default Plot range for meson
        if(mode == 40 || mode == 41 || mode == 42 || mode == 44 || mode == 45 ||
           mode == 60 || mode == 61 || mode == 62 || mode == 64 || mode == 65){
            fMesonPlotRange[0]         = 0.53;
            fMesonPlotRange[1]         = 0.56;
        }

        // Initialize default Plot default integration ranges
        if(mode == 40 || mode == 60){
            fMesonIntDeltaRange[0]      = -0.015;
            fMesonIntDeltaRange[1]      =  0.015;
            fMesonIntDeltaRangeWide[0]  = -0.030;
            fMesonIntDeltaRangeWide[1]  =  0.030;
            fMesonIntDeltaRangeNarrow[0]= -0.010;
            fMesonIntDeltaRangeNarrow[1]=  0.010;
        } else if (mode == 41 || mode == 61){
            fMesonIntDeltaRange[0]      = -0.020;
            fMesonIntDeltaRange[1]      =  0.020;
            fMesonIntDeltaRangeWide[0]  = -0.030;
            fMesonIntDeltaRangeWide[1]  =  0.030;
            fMesonIntDeltaRangeNarrow[0]= -0.014;
            fMesonIntDeltaRangeNarrow[1]=  0.014;
        } else if (mode == 42 || mode == 62){
            fMesonIntDeltaRange[0]      = -0.013;
            fMesonIntDeltaRange[1]      =  0.013;
            fMesonIntDeltaRangeWide[0]  = -0.020;
            fMesonIntDeltaRangeWide[1]  =  0.020;
            fMesonIntDeltaRangeNarrow[0]= -0.010;
            fMesonIntDeltaRangeNarrow[1]=  0.010;
        } else if (mode == 44 || mode == 64){
            fMesonIntDeltaRange[0]      = -0.026;
            fMesonIntDeltaRange[1]      =  0.017;
            fMesonIntDeltaRangeWide[0]  = -0.03;
            fMesonIntDeltaRangeWide[1]  =  0.025;
            fMesonIntDeltaRangeNarrow[0]= -0.017;
            fMesonIntDeltaRangeNarrow[1]=  0.012;
        } else if (mode == 45 || mode == 65){
            fMesonIntDeltaRange[0]      = -0.014;
            fMesonIntDeltaRange[1]      =  0.014;
            fMesonIntDeltaRangeWide[0]  = -0.021;
            fMesonIntDeltaRangeWide[1]  =  0.021;
            fMesonIntDeltaRangeNarrow[0]= -0.008;
            fMesonIntDeltaRangeNarrow[1]=  0.008;
        }


        // Set meson mass ranges (here same for fitting and plotting)
         if(mode == 40 || mode == 41 || mode == 42 || mode == 44 || mode == 45 ||
            mode == 60 || mode == 61 || mode == 62 || mode == 64 || mode == 65){
             fMesonMassPlotRange[0]                = 0.47;
             fMesonMassPlotRange[1]                = 0.61;
             fMesonMassPlotRange_SubPiZero[0]      = fMesonMassPlotRange[0];
             fMesonMassPlotRange_SubPiZero[1]      = fMesonMassPlotRange[1];
             fMesonMassPlotRange_FixedPzPiZero[0]  = fMesonMassPlotRange[0];
             fMesonMassPlotRange_FixedPzPiZero[1]  = fMesonMassPlotRange[1];
             fMesonMassRange[0]                    = 0.47;
             fMesonMassRange[1]                    = 0.65;
             fMesonMassRange_SubPiZero[0]          = fMesonMassRange[0];
             fMesonMassRange_SubPiZero[1]          = fMesonMassRange[1];
             fMesonMassRange_FixedPzPiZero[0]      = fMesonMassRange[0];
             fMesonMassRange_FixedPzPiZero[1]      = fMesonMassRange[1];
         }

         // Set meson fit range
         if(mode == 40 || mode == 60){
             fMesonFitRange[0]                     = 0.48;
             fMesonFitRange[1]                     = 0.61;
             fMesonFitRange_SubPiZero[0]           = fMesonFitRange[0];
             fMesonFitRange_SubPiZero[1]           = fMesonFitRange[1];
             fMesonFitRange_FixedPzPiZero[0]       = fMesonFitRange[0];
             fMesonFitRange_FixedPzPiZero[1]       = fMesonFitRange[1];
         } else if(mode == 41 || mode == 42 || mode == 44 || mode == 45 ||
                   mode == 61 || mode == 62 || mode == 64 || mode == 65){
             fMesonFitRange[0]                     = 0.49;
             fMesonFitRange[1]                     = 0.59;
             fMesonFitRange_SubPiZero[0]           = fMesonFitRange[0];
             fMesonFitRange_SubPiZero[1]           = fMesonFitRange[1];
             fMesonFitRange_FixedPzPiZero[0]       = fMesonFitRange[0];
             fMesonFitRange_FixedPzPiZero[1]       = fMesonFitRange[1];
         }

         // Set remaining parameters for fitting
         if(mode == 40 || mode == 60){
             FitRangeSigBckRatioOption   = 20; //xy; x: 1== Gaus, 2==GausExp ;y: 0 == wide range, 1 == narrow range, 2 = mid range
             usePolNForBackgroundScaling = 1;
             fMesonWidthExpect           = 0.010;
             fMesonWidthRange[0]         = 0.004;
             fMesonWidthRange[1]         = 0.030;
             fMesonLambdaTail            = 0.0007;
             fMesonLambdaTailRange[0]    = 0.0007;
             fMesonLambdaTailRange[1]    = 0.0007;

             fFullPt[0]                  = 0.4;
             fFullPt[1]                  = 15;

             // Settings for MC
             fMesonLambdaTailMC    = fMesonLambdaTail;
             fMesonWidthExpectMC   = fMesonWidthExpect;
             fMesonWidthRangeMC[0] = fMesonWidthRange[0];
             fMesonWidthRangeMC[1] = fMesonWidthRange[1];

             // Settings for true
             fMesonWidthRangeTrue[0]      = 0.0005;
             fMesonWidthRangeTrue[1]      = 0.100;
             fMesonLambdaTailTrue         = 0.0007;
             fMesonLambdaTailRangeTrue[0] = fMesonLambdaTailRange[0];
             fMesonLambdaTailRangeTrue[1] = fMesonLambdaTailRange[1];
         } else if(mode == 41 || mode == 61){
             FitRangeSigBckRatioOption   = 20; //xy; x: 1== Gaus, 2==GausExp ;y: 0 == wide range, 1 == narrow range, 2 = mid range
             usePolNForBackgroundScaling = 1;
             fMesonWidthExpect           = 0.010;
             fMesonWidthRange[0]         = 0.004;
             fMesonWidthRange[1]         = 0.030;
             fMesonLambdaTail            = 0.003;
             fMesonLambdaTailRange[0]    = 0.003;
             fMesonLambdaTailRange[1]    = 0.003;

             fFullPt[0]                  = 0.4;
             fFullPt[1]                  = 15;

             // Settings for MC
             fMesonLambdaTailMC    = fMesonLambdaTail;
             fMesonWidthExpectMC   = fMesonWidthExpect;
             fMesonWidthRangeMC[0] = fMesonWidthRange[0];
             fMesonWidthRangeMC[1] = fMesonWidthRange[1];

             // Settings for true
             fMesonWidthRangeTrue[0]      = 0.0005;
             fMesonWidthRangeTrue[1]      = 0.100;
             fMesonLambdaTailTrue         = 0.0007;
             fMesonLambdaTailRangeTrue[0] = fMesonLambdaTailRange[0];
             fMesonLambdaTailRangeTrue[1] = fMesonLambdaTailRange[1];
         } else if(mode == 42 || mode == 62){
             FitRangeSigBckRatioOption   = 20; //xy; x: 1== Gaus, 2==GausExp ;y: 0 == wide range, 1 == narrow range, 2 = mid range
             usePolNForBackgroundScaling = 1;
             fMesonWidthExpect           = 0.005;
             fMesonWidthRange[0]         = 0.004;
             fMesonWidthRange[1]         = 0.070;
             fMesonLambdaTail            = 0.0007;
             fMesonLambdaTailRange[0]    = 0.0007;
             fMesonLambdaTailRange[1]    = 0.0007;

             fFullPt[0]                  = 0.4;
             fFullPt[1]                  = 15;

             // Settings for MC
             fMesonLambdaTailMC    = fMesonLambdaTail;
             fMesonWidthExpectMC   = fMesonWidthExpect;
             fMesonWidthRangeMC[0] = fMesonWidthRange[0];
             fMesonWidthRangeMC[1] = fMesonWidthRange[1];

             // Settings for true
             fMesonWidthRangeTrue[0]      = 0.0005;
             fMesonWidthRangeTrue[1]      = 0.100;
             fMesonLambdaTailTrue         = 0.0007;
             fMesonLambdaTailRangeTrue[0] = fMesonLambdaTailRange[0];
             fMesonLambdaTailRangeTrue[1] = fMesonLambdaTailRange[1];
         } else if (mode == 44 || mode == 64){
             FitRangeSigBckRatioOption   = 20; //xy; x: 1== Gaus, 2==GausExp ;y: 0 == wide range, 1 == narrow range, 2 = mid range
             usePolNForBackgroundScaling = 1;
             fMesonWidthExpect           = 0.006;
             fMesonWidthRange[0]         = 0.001;
             fMesonWidthRange[1]         = 0.1;
             fMesonLambdaTail            = 0.001;
             fMesonLambdaTailRange[0]    = 0.001;
             fMesonLambdaTailRange[1]    = 0.001;

             fFullPt[0]                  = 0.4;
             fFullPt[1]                  = 15;

             // Settings for MC
             fMesonLambdaTailMC    = fMesonLambdaTail;
             fMesonWidthExpectMC   = fMesonWidthExpect;
             fMesonWidthRangeMC[0] = fMesonWidthRange[0];
             fMesonWidthRangeMC[1] = fMesonWidthRange[1];

             // Settings for true
             fMesonWidthRangeTrue[0]      = 0.0005;
             fMesonWidthRangeTrue[1]      = 0.100;
             fMesonLambdaTailTrue         = 0.0007;
             fMesonLambdaTailRangeTrue[0] = fMesonLambdaTailRange[0];
             fMesonLambdaTailRangeTrue[1] = fMesonLambdaTailRange[1];
         } else if (mode == 45 || mode == 65){
             FitRangeSigBckRatioOption   = 20; //xy; x: 1== Gaus, 2==GausExp ;y: 0 == wide range, 1 == narrow range, 2 = mid range
             usePolNForBackgroundScaling = 1;
             fMesonWidthExpect           = 0.006;
             fMesonWidthRange[0]         = 0.001;
             fMesonWidthRange[1]         = 0.1;
             fMesonLambdaTail            = 0.001;
             fMesonLambdaTailRange[0]    = 0.001;
             fMesonLambdaTailRange[1]    = 0.001;

             fFullPt[0]                  = 0.4;
             fFullPt[1]                  = 15;

             // Settings for MC
             fMesonLambdaTailMC    = fMesonLambdaTail;
             fMesonWidthExpectMC   = fMesonWidthExpect;
             fMesonWidthRangeMC[0] = fMesonWidthRange[0];
             fMesonWidthRangeMC[1] = fMesonWidthRange[1];

             // Settings for true
             fMesonWidthRangeTrue[0]      = 0.0005;
             fMesonWidthRangeTrue[1]      = 0.100;
             fMesonLambdaTailTrue         = 0.0007;
             fMesonLambdaTailRangeTrue[0] = fMesonLambdaTailRange[0];
             fMesonLambdaTailRangeTrue[1] = fMesonLambdaTailRange[1];
         }

    }
}


class BackgroundClass
{
public:
  BackgroundClass(TH1 *Comb, TH1 *Cont, Double_t ignoreMin0, Double_t ignoreMax0, Double_t ignoreMin1, Double_t ignoreMax1, Double_t ignoreMin2, Double_t ignoreMax2)
  {
    fCombinatorical     = Comb;
    fContamination      = Cont;

    ignoreMinX0         = ignoreMin0;
    ignoreMinX0         = ignoreMax0;
    ignoreMinX1         = ignoreMin1;
    ignoreMinX1         = ignoreMax1;
    ignoreMinX2         = ignoreMin2;
    ignoreMinX2         = ignoreMax2;
  }

  double operator()(double *xvals, double *pars)
  {
    Double_t totScale       = pars[0];       // total scale parameter
    Double_t contScale      = pars[1];       // scale of contamination contribution
    Double_t x              = xvals[0];

    Int_t binCombinatorial  = fCombinatorical->FindBin(x);
    Int_t binContamination  = fContamination->FindBin(x);
    Double_t CombVal           = fCombinatorical->GetBinContent(binCombinatorial);
    Double_t ContVal           = fContamination->GetBinContent(binContamination);

    if ((x > ignoreMinX0 && x < ignoreMaxX0)||(x > ignoreMinX1 && x < ignoreMaxX1)||(x > ignoreMinX2 && x < ignoreMaxX2)){
        TF1::RejectPoint();
    }
    return totScale *( CombVal + (contScale * ContVal));
  }
  TH1 *fCombinatorical;
  TH1 *fContamination;

  Int_t ignoreMinX0;
  Int_t ignoreMaxX0;
  Int_t ignoreMinX1;
  Int_t ignoreMaxX1;
  Int_t ignoreMinX2;
  Int_t ignoreMaxX2;
};
