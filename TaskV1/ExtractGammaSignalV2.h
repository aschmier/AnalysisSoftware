// provided by Gamma Conversion Group, $ALICE_ROOT/PWG4/GammaConv ;https://twiki.cern.ch/twiki/bin/view/ALICE/PWG4GammaConversion


//!! -> what is commented here is defined in ExtractSignal.h
Bool_t      fEnablePCM                                              = 0;
Bool_t      fEnableCalo                                             = 0;
Bool_t      fUseCocktail                                            = 0;
Bool_t      fUseDataDrivenPurity                                    = 0;

TString     fDate                                                   = "";
TString     fDirectPhoton                                           = "";

TString     fOutputDir                                              = "";
TString     fSuffix                                                 = "";
TString     fMeson                                                  = "";

TString     fCutSelectionRead                                       = ""; 
TString     fEventCutSelectionRead                                  = "";
TString     fGammaCutSelectionRead                                  = "";
TString     fMesonCutSelectionRead                                  = "";

TString     fHistogramDimension                                     = "";
Int_t       nHistogramDimension                                     = 1;
Int_t       nPileupMethodUsed                                       = 0;

Int_t       fNBinsPtDummy                                           = 0;
Double_t*   fBinsPtDummy                                            = NULL;

Bool_t      fEnableDCConv                                           = kFALSE;

TString     fDecays[7]                                              = { "Pi0",  "Eta",   "Etap", "Omega",   "Rho", 
                                                                        "Phi",  "Sigma"
                                                                      };
Int_t       nCombinatorics                                          = 16;
TString     fCombinatorics[17]                                      = { "Elec+Elec",     "Elec+Pion",   "Elec+Kaon",        "Elec+Proton",   "Elec+Muon", 
                                                                        "Pion+Pion",     "Pion+Kaon",   "Pion+Proton",      "Pion+Muon",     "Kaon+Kaon",
                                                                        "Kaon+Proton",   "Kaon+Muon",   "Proton+Proton",    "Proton+Muon",   "Muon+Muon",
                                                                        "Rest",          "All"
                                                                      };
Int_t       nContamination                                          = 10;
TString     fContamination[11]                                      = { "Electron", "Pion", "Proton",   "Kaon", "Neutron", "K0s",
                                                                        "Lambda",   "Muon", "K0l",      "Rest", "All"
                                                                      };

// exemplary pt bins for DCAz distributions
Int_t       exemplaryLowPtBin                                       = 1;
Int_t       exemplaryHighPtBin                                      = 15;

// ShowBackground arguments
Int_t nIterationsShowBackground[4]                                  = {0};
TString optionShowBackground[3]                                     = {""};

// binning
TH1D*       fDeltaPtDummy                                           = NULL;
                                                                      
//*****************************************************************************************************
//******************** Histograms for conversion analysis *********************************************
//*****************************************************************************************************
TH1D*       fHistoPurityKappaTemplates                                                          = NULL;

TH1D*       fHistoGammaConvPt                                                                   = NULL;
TH1D*       fHistoGammaConvPtOrBin                                                              = NULL;
TH2D*       f2DHistoSecondaryGammaMCConvPt                                                      = NULL;
TH2D*       f2DHistoAllSecondaryGammaMCPt                                                       = NULL;
TH1D*       fHistoGammaMCrecConvPt                                                              = NULL;
TH1D*       fHistoGammaMCrecConvPtOrBin                                                         = NULL;
TH1D*       fHistoGammaMCConvPt                                                                 = NULL;
TH1D*       fHistoGammaMCConvPtOrBin                                                            = NULL;
TH1D*       fHistoGammaMCConvRSPt                                                               = NULL;
TH1D*       fHistoGammaMCAllPt                                                                  = NULL;
TH1D*       fHistoGammaMCAllPtOrBin                                                             = NULL;

TH1D*       fHistoSecondaryGammaConvFromXFromK0sPt                                              = NULL;
TH1D*       fHistoSecondaryGammaConvFromXFromK0lPt                                              = NULL;
TH1D*       fHistoSecondaryGammaConvFromXFromLambdaPt                                           = NULL;
TH1D*       fHistoSecondaryGammaConvFromXFromK0sPtOrBin                                         = NULL;
TH1D*       fHistoSecondaryGammaConvFromXFromK0lPtOrBin                                         = NULL;
TH1D*       fHistoSecondaryGammaConvFromXFromLambdaPtOrBin                                      = NULL;

TH1D*       fHistoSecondaryGammaCocktailFromXFromK0sPt                                          = NULL;
TH1D*       fHistoSecondaryGammaCocktailFromXFromK0lPt                                          = NULL;
TH1D*       fHistoSecondaryGammaCocktailFromXFromLambdaPt                                       = NULL;
TH1D*       fHistoSecondaryGammaCocktailFromXFromK0sPtOrBin                                     = NULL;
TH1D*       fHistoSecondaryGammaCocktailFromXFromK0lPtOrBin                                     = NULL;
TH1D*       fHistoSecondaryGammaCocktailFromXFromLambdaPtOrBin                                  = NULL;

TH1D*       fHistoAllSecondaryGammaFromXFromK0sPt                                               = NULL;
TH1D*       fHistoAllSecondaryGammaFromXFromK0lPt                                               = NULL;
TH1D*       fHistoAllSecondaryGammaFromXFromLambdaPt                                            = NULL;
TH1D*       fHistoAllSecondaryGammaFromXFromK0sPtOrBin                                          = NULL;
TH1D*       fHistoAllSecondaryGammaFromXFromK0lPtOrBin                                          = NULL;
TH1D*       fHistoAllSecondaryGammaFromXFromLambdaPtOrBin                                       = NULL;

TH1D*       fHistoGammaTrueConvPt                                                               = NULL;
TH1D*       fHistoGammaTrueConvPtOrBin                                                          = NULL;
TH1D*       fHistoGammaTruePrimaryConvPt                                                        = NULL;
TH1D*       fHistoGammaTruePrimaryConvPtOrBin                                                   = NULL;
TH1D*       fHistoGammaTrueSecondaryConvPt                                                      = NULL;
TH2D*       f2DHistoGammaTrueSecondaryConvPt                                                    = NULL;
TH2D*       f2DHistoGammaTrueSecondaryConvMCPt                                                  = NULL;
TH1D*       fHistoGammaTrueSecondaryConvPtOrBin                                                 = NULL;
TH1D*       fHistoGammaTrueSecondaryConvGammaFromXFromK0sMCPt                                   = NULL;
TH1D*       fHistoGammaTrueSecondaryConvGammaFromXFromK0lMCPt                                   = NULL;
TH1D*       fHistoGammaTrueSecondaryConvGammaFromXFromLambdaMCPt                                = NULL;
TH1D*       fHistoGammaTrueSecondaryConvGammaFromXFromK0sMCPtOrBin                              = NULL;
TH1D*       fHistoGammaTrueSecondaryConvGammaFromXFromK0lMCPtOrBin                              = NULL;
TH1D*       fHistoGammaTrueSecondaryConvGammaFromXFromLambdaMCPtOrBin                           = NULL;
TH1D*       fHistoGammaTrueSecondaryConvGammaFromXFromK0sPt                                     = NULL;
TH1D*       fHistoGammaTrueSecondaryConvGammaFromXFromK0sPtOrBin                                = NULL;
TH1D*       fHistoGammaTrueSecondaryConvGammaFromXFromK0lPt                                     = NULL;
TH1D*       fHistoGammaTrueSecondaryConvGammaFromXFromK0lPtOrBin                                = NULL;
TH1D*       fHistoGammaTrueSecondaryConvGammaFromXFromLambdaPt                                  = NULL;
TH1D*       fHistoGammaTrueSecondaryConvGammaFromXFromLambdaPtOrBin                             = NULL;
TH1D*       fHistoGammaTrueSecondaryConvGammaRestPt                                             = NULL;
TH1D*       fHistoGammaTrueSecondaryConvGammaRestPtOrBin                                        = NULL;
TH2D*       fHistoGammaTrueSecondaryFromXFromK0sConv_MCPt_recPt_MC                              = NULL;
TH2D*       fHistoGammaTrueSecondaryFromXFromK0lConv_MCPt_recPt_MC                              = NULL;
TH2D*       fHistoGammaTrueSecondaryFromXFromLambdaConv_MCPt_recPt_MC                           = NULL;

TH1D**      fHistoGammaMCDecayPt                                                                = NULL;
TH1D*       fHistoGammaTruePrimaryConvMCPt                                                      = NULL;
TH1D*       fHistoGammaTruePrimaryConvMCPtOrBin                                                 = NULL;

TH1D*       fHistoFracAllGammaToSecOrBin                                                        = NULL;
TH1D*       fHistoFracAllGammaToSec                                                             = NULL;
TH1D*       fHistoFracAllGammaToSecFromXFromK0s                                                 = NULL;
TH1D*       fHistoFracAllGammaToSecFromXFromK0sOrBin                                            = NULL;
TH1D*       fHistoFracAllGammaToSecFromXFromK0l                                                 = NULL;
TH1D*       fHistoFracAllGammaToSecFromXFromK0lOrBin                                            = NULL;
TH1D*       fHistoFracAllGammaToSecFromXFromLambda                                              = NULL;
TH1D*       fHistoFracAllGammaToSecFromXFromLambdaOrBin                                         = NULL;
TH1D*       fHistoFracAllGammaToSecRest                                                         = NULL;
TH1D*       fHistoFracAllGammaToSecRestOrBin                                                    = NULL;
TH1D*       fHistoGammaMCConvProb                                                               = NULL;
TH1D*       fHistoGammaMCConvProbOrBin                                                          = NULL;
TH1D*       fHistoSecondaryGammaFromXFromK0sMCConvProb                                          = NULL;
TH1D*       fHistoSecondaryGammaFromXFromK0lMCConvProb                                          = NULL;
TH1D*       fHistoSecondaryGammaFromXFromLambdaMCConvProb                                       = NULL;
TH1D*       fHistoSecondaryGammaFromXFromK0sMCConvProbOrBin                                     = NULL;
TH1D*       fHistoSecondaryGammaFromXFromK0lMCConvProbOrBin                                     = NULL;
TH1D*       fHistoSecondaryGammaFromXFromLambdaMCConvProbOrBin                                  = NULL;
TH1D*       fHistoGammaMCPurity                                                                 = NULL;
TH1D*       fHistoGammaMCTruePurityOrBin                                                        = NULL;
TH1D*       fHistoGammaMCrecPrimaryConvPt                                                       = NULL;
TH1D*       fHistoGammaMCTruePurity                                                             = NULL;
TH1D*       fHistoGammaMCrecPrimaryConvPtOrBin                                                  = NULL;
TH1D*       fHistoGammaMCRecoEff                                                                = NULL;
TH1D*       fHistoGammaMCPrimaryRecoEff                                                         = NULL;
TH1D*       fHistoGammaMCPrimaryRecoEffOrBin                                                    = NULL;
TH1D*       fHistoGammaMCPrimaryRecoEffMCPt                                                     = NULL;
TH1D*       fHistoGammaMCPrimaryRecoEffMCPtOrBin                                                = NULL;
TH1D*       fHistoGammaMCBackground                                                             = NULL;

TH1D*       fHistoSecondaryGammaFromXFromK0sMCRecoEffMCPt                                       = NULL;
TH1D*       fHistoSecondaryGammaFromXFromK0lMCRecoEffMCPt                                       = NULL;
TH1D*       fHistoSecondaryGammaFromXFromLambdaMCRecoEffMCPt                                    = NULL;
TH1D*       fHistoSecondaryGammaFromXFromK0sMCRecoEffMCPtOrBin                                  = NULL;
TH1D*       fHistoSecondaryGammaFromXFromK0lMCRecoEffMCPtOrBin                                  = NULL;
TH1D*       fHistoSecondaryGammaFromXFromLambdaMCRecoEffMCPtOrBin                               = NULL;

TH1D*       fHistoSecondaryGammaFromXFromK0sMCRecoEffPt                                         = NULL;
TH1D*       fHistoSecondaryGammaFromXFromK0lMCRecoEffPt                                         = NULL;
TH1D*       fHistoSecondaryGammaFromXFromLambdaMCRecoEffPt                                      = NULL;
TH1D*       fHistoSecondaryGammaFromXFromK0sMCRecoEffPtOrBin                                    = NULL;
TH1D*       fHistoSecondaryGammaFromXFromK0lMCRecoEffPtOrBin                                    = NULL;
TH1D*       fHistoSecondaryGammaFromXFromLambdaMCRecoEffPtOrBin                                 = NULL;

TH2D*       fHistoGammaTrueSecondaryFromXFromK0sConv_MCPt_recPt_MC_Rebin                        = NULL;
TH2D*       fHistoGammaTrueSecondaryFromXFromK0lConv_MCPt_recPt_MC_Rebin                        = NULL;
TH2D*       fHistoGammaTrueSecondaryFromXFromLambdaConv_MCPt_recPt_MC_Rebin                     = NULL;

TH2D*       fHistoCombinatorialBackground                                                       = NULL;
TH2D*       fHistoGammaTruePrimaryConv_recPt_MCPt_MC                                            = NULL;
TH2D*       fHistoGammaTruePrimaryConv_recPt_MCPt_MC_Rebin                                      = NULL;

TH2F*       fMCrecGammaPtDCAz                                                                   = NULL;
TH2F**      fESDGammaPtDCAz                                                                     = NULL;
TH1D***     fESDGammaPtDCAzBins                                                                 = NULL;
TH1D****    fESDGammaPtDCAzBinsBack                                                             = NULL;
TH1D****    fESDSubGammaPtDCAzBins                                                              = NULL;

TH1D***     fMCrecGammaPtDCAzBins                                                               = NULL;
TH1D***     fMCrecGammaPtDCAzBinsBack                                                           = NULL;
TH1D***     fMCrecSubGammaPtDCAzBins                                                            = NULL;

TH2F**      fTruePrimaryPhotonPtDCAz                                                            = NULL;
TH1D***     fTruePrimaryGammaPtDCAzBins                                                         = NULL;
TH1D***     fTruePrimarySubGammaPtDCAzBins                                                      = NULL;
TH2F**      fTrueSecondaryPhotonPtDCAz                                                          = NULL;
TH1D***     fTrueSecondaryGammaPtDCAzBins                                                       = NULL;
TH1D***     fTrueSecondarySubGammaPtDCAzBins                                                    = NULL;
TH2F**      fTrueSecondaryPhotonFromXFromK0sPtDCAz                                              = NULL;
TH1D***     fTrueSecondaryGammaFromXFromK0sPtDCAzBins                                           = NULL;
TH1D***     fTrueSecondarySubGammaFromXFromK0sPtDCAzBins                                        = NULL;
TH1D***     fTrueBackgroundPtDCAzBins                                                           = NULL;
TH1D***     fTrueGammaPtDCAzBins                                                                = NULL;
TH1D***     fTrueSubGammaPtDCAzBins                                                             = NULL;
TH1D**      fHistoCombinatorialSpecies                                                          = NULL;

TH1D**      fESDGammaPerCatPtDCAzBins                                                           = NULL;
TH1D**      fESDGammaRatioCatToCombinedPtDCAzBins                                               = NULL;

TH1D*       fESDGammaPtPileUpAllCat                                                             = NULL;
TH1D*       fESDGammaPtPileUp                                                                   = NULL;
TH1D**      fESDGammaPtRatioWithWithoutPileUpDCAzDistBinningAllCat                              = NULL;
TH1D**      fESDGammaPtRatioWithWithoutPileUpDCAzDistBinning                                    = NULL;
TF1**       fESDGammaPtRatioWithWithoutPileUpFitDCAzDistBinningAllCat                           = NULL;
TF1**       fESDGammaPtRatioWithWithoutPileUpFitDCAzDistBinning                                 = NULL;

TH1D*       fMCrecGammaPtPileUpAllCat                                                           = NULL;
TH1D*       fMCrecGammaPtPileUp                                                                 = NULL;
TH1D*       fTruePrimaryConvGammaPtPileUpAllCat                                                 = NULL;
TH1D*       fTruePrimaryConvGammaPtPileUp                                                       = NULL;
TH1D*       fTrueSecondaryConvGammaPtPileUpAllCat                                               = NULL;
TH1D*       fTrueSecondaryConvGammaPtPileUp                                                     = NULL;
TH1D*       fTrueSecondaryFromXFromK0sConvGammaPtPileUpAllCat                                   = NULL;
TH1D*       fTrueSecondaryFromXFromK0sConvGammaPtPileUp                                         = NULL;

TH1D*       fMCrecGammaPtRatioWithWithoutPileUpDCAzDistBinningAllCat                            = NULL;
TH1D*       fMCrecGammaPtRatioWithWithoutPileUpDCAzDistBinning                                  = NULL;
TF1*        fMCrecGammaPtRatioWithWithoutPileUpFitDCAzDistBinningAllCat                         = NULL;
TF1*        fMCrecGammaPtRatioWithWithoutPileUpFitDCAzDistBinning                               = NULL;

TH1D*       fTruePrimaryConvGammaPtRatioWithWithoutPileUpDCAzDistBinningAllCat                  = NULL;
TH1D*       fTruePrimaryConvGammaPtRatioWithWithoutPileUpDCAzDistBinning                        = NULL;
TF1*        fTruePrimaryConvGammaPtRatioWithWithoutPileUpFitDCAzDistBinningAllCat               = NULL;
TF1*        fTruePrimaryConvGammaPtRatioWithWithoutPileUpFitDCAzDistBinning                     = NULL;

TH1D*       fTrueSecondaryConvGammaPtRatioWithWithoutPileUpDCAzDistBinningAllCat                = NULL;
TH1D*       fTrueSecondaryConvGammaPtRatioWithWithoutPileUpDCAzDistBinning                      = NULL;
TF1*        fTrueSecondaryConvGammaPtRatioWithWithoutPileUpFitDCAzDistBinningAllCat             = NULL;
TF1*        fTrueSecondaryConvGammaPtRatioWithWithoutPileUpFitDCAzDistBinning                   = NULL;

TH1D*       fTrueSecondaryFromXFromK0sConvGammaPtRatioWithWithoutPileUpDCAzDistBinningAllCat    = NULL;
TH1D*       fTrueSecondaryFromXFromK0sConvGammaPtRatioWithWithoutPileUpDCAzDistBinning          = NULL;
TF1*        fTrueSecondaryFromXFromK0sConvGammaPtRatioWithWithoutPileUpFitDCAzDistBinningAllCat = NULL;
TF1*        fTrueSecondaryFromXFromK0sConvGammaPtRatioWithWithoutPileUpFitDCAzDistBinning       = NULL;

TH1D**      fESDGammaPileUpCorrFactorAllCat                                                     = NULL;
TH1D**      fESDGammaPileUpCorrFactor                                                           = NULL;
TH1D*       fMCrecGammaPileUpCorrFactorAllCat                                                   = NULL;
TH1D*       fMCrecGammaPileUpCorrFactor                                                         = NULL;
TH1D*       fTruePrimaryConvGammaPileUpCorrFactorAllCat                                         = NULL;
TH1D*       fTruePrimaryConvGammaPileUpCorrFactor                                               = NULL;
TH1D*       fTrueSecondaryConvGammaPileUpCorrFactorAllCat                                       = NULL;
TH1D*       fTrueSecondaryConvGammaPileUpCorrFactor                                             = NULL;
TH1D*       fTrueSecondaryFromXFromK0sConvGammaPileUpCorrFactorAllCat                           = NULL;
TH1D*       fTrueSecondaryFromXFromK0sConvGammaPileUpCorrFactor                                 = NULL;

TH1D*       fHistoFracAllGammaToSecPileUp                                                       = NULL;
TH1D*       fHistoFracAllGammaToSecFromXFromK0sPileUp                                           = NULL;
TH1D*       fHistoGammaMCPurityPileUp                                                           = NULL;
TH1D*       fHistoGammaMCrecPrimaryConvPtPileUp                                                 = NULL;
TH1D*       fHistoGammaMCTruePurityPileUp                                                       = NULL;
TH1D*       fHistoGammaMCRecoEffPileUp                                                          = NULL;
TH1D*       fHistoGammaMCPrimaryRecoEffPileUp                                                   = NULL;

TH1D*       fHistoPhotonIsSelected                                                              = NULL;

//******************* tagging outputHistograms ********************************************************
TH1D**      fHistoGGInvMassPtGConvBin                                                           = NULL;
TH1D**      fHistoSignalInvMassPtGConvBin                                                       = NULL;
TH1D**      fHistoSignalInvMassLeftPtGConvBin                                                   = NULL;
TH1D**      fHistoBackInvMassPtGconvBin                                                         = NULL;
TH1D**      fHistoBackNormInvMassPtGconvBin                                                     = NULL;
TH1D**      fHistoBackNormInvMassLeftPtGconvBin                                                 = NULL;
TH1D**      fHistoTruePrimMesonInvMassPtBins                                                    = NULL;  
TH1D**      fHistoTrueFullMesonInvMassPtBins                                                    = NULL;  
TH1D**      fHistoTrueSecMesonInvMassPtBins[4]                                                  = {NULL, NULL, NULL, NULL};    
TH2D*       fHistoTruePrimMesonInvMassVSPt                                                      = NULL;
TF1**       fFitTrueFullSignalInvMassPtBin                                                      = NULL;

//*****************************************************************************************************
//******************** Check multiple counts of photons ***********************************************
//*****************************************************************************************************
TH2D*       fHistoTrueGammaConvDCRVSPt                                                          = NULL;
TH1D*       fHistoTrueGammaConvDCR                                                              = NULL;
TH1D*       fHistoTrueGammaConvDCPt                                                             = NULL;
TH1F*       fHistoTrueGammaConvMultipleCount                                                    = NULL;
TString     ObjectNameDCGammaConvRPt                                                            = "";
TString     ObjectNameGammaConvMultipleCount                                                    = "";

TTree*      dcaTree                                                                             = NULL;
TList*      DCAContainer                                                                        = NULL;
Bool_t      pileUpCorrection                                                                    = kFALSE;

//*****************************************************************************************************
//******************** Histograms for calo analysis ***************************************************
//*****************************************************************************************************
TList*      CaloContainer                                                                       = NULL;
TH1D*       fHistoGammaCaloPt                                                                   = NULL;
TH1D*       fHistoGammaCaloPtOrBin                                                              = NULL;
TH1D*       fHistoGammaMCrecCaloPt                                                              = NULL;
TH1D*       fHistoGammaMCrecCaloPtOrBin                                                         = NULL;
TH1D*       fHistoGammaMCAllInEMCAccPt                                                          = NULL;
TH1D*       fHistoGammaMCAllInEMCAccPtOrBin                                                     = NULL;
TH1D*       fHistoGammaTrueCaloPt                                                               = NULL;
TH1D*       fHistoGammaTrueCaloPtOrBin                                                          = NULL;
TH1D*       fHistoGammaTruePrimaryCaloPt                                                        = NULL;
TH1D*       fHistoGammaTruePrimaryCaloPtOrBin                                                   = NULL;
TH1D*       fHistoGammaTrueSecondaryCaloPt                                                      = NULL;
TH1D*       fHistoGammaTrueSecondaryCaloMCPt                                                    = NULL;
TH1D*       fHistoGammaTrueSecondaryCaloPtOrBin                                                 = NULL;
TH1D*       fHistoGammaTrueSecondaryCaloMCPtOrBin                                               = NULL;
TH1D*       fHistoGammaTrueSecondaryCaloFromXFromK0sPt                                          = NULL;
TH1D*       fHistoGammaTrueSecondaryCaloFromXFromK0sMCPt                                        = NULL;
TH1D*       fHistoGammaTrueSecondaryCaloFromXFromK0sPtOrBin                                     = NULL;
TH1D*       fHistoGammaTrueSecondaryCaloFromXFromK0sMCPtOrBin                                   = NULL;
TH1D*       fHistoGammaTrueSecondaryCaloFromXFromK0lPt                                          = NULL;
TH1D*       fHistoGammaTrueSecondaryCaloFromXFromK0lMCPt                                        = NULL;
TH1D*       fHistoGammaTrueSecondaryCaloFromXFromK0lPtOrBin                                     = NULL;
TH1D*       fHistoGammaTrueSecondaryCaloFromXFromK0lMCPtOrBin                                   = NULL;
TH1D*       fHistoGammaTrueSecondaryCaloFromXFromLambdaPt                                       = NULL;
TH1D*       fHistoGammaTrueSecondaryCaloFromXFromLambdaMCPt                                     = NULL;
TH1D*       fHistoGammaTrueSecondaryCaloFromXFromLambdaPtOrBin                                  = NULL;
TH1D*       fHistoGammaTrueSecondaryCaloFromXFromLambdaMCPtOrBin                                = NULL;
TH1D*       fHistoGammaTrueSecondaryCaloFromXFromEtaPt                                          = NULL;
TH1D*       fHistoGammaTrueSecondaryCaloFromXFromEtaMCPt                                        = NULL;
TH1D*       fHistoGammaTrueSecondaryCaloFromXFromEtaPtOrBin                                     = NULL;
TH1D*       fHistoGammaTrueSecondaryCaloFromXFromEtaMCPtOrBin                                   = NULL;
TH1D*       fHistoGammaTrueSecondaryCaloRestPt                                                  = NULL;
TH1D*       fHistoGammaTrueSecondaryCaloRestMCPt                                                = NULL;
TH1D*       fHistoGammaTrueSecondaryCaloRestPtOrBin                                             = NULL;
TH1D*       fHistoGammaTrueSecondaryCaloRestMCPtOrBin                                           = NULL;
TH1D*       fHistoGammaTrueSecondaryCaloRestNoEtaPt                                             = NULL;
TH1D*       fHistoGammaTrueSecondaryCaloRestNoEtaMCPt                                           = NULL;
TH1D*       fHistoGammaTrueSecondaryCaloRestNoEtaPtOrBin                                        = NULL;
TH1D*       fHistoGammaTrueSecondaryCaloRestNoEtaMCPtOrBin                                      = NULL;

TH2D*       fHistoGammaTruePrimaryCalo_recPt_MCPt_MC                                            = NULL;
TH2D*       fHistoGammaTruePrimaryCalo_recPt_MCPt_MC_Rebin                                      = NULL;
TH2D*       fHistoGammaTrueSecondaryFromXFromK0sCaloUnConv_MCPt_recPt_MC                        = NULL;
TH2D*       fHistoGammaTrueSecondaryFromXFromK0sCaloConv_MCPt_recPt_MC                          = NULL;
TH2D*       fHistoGammaTrueSecondaryFromXFromK0sCalo_MCPt_recPt_MC                              = NULL;
TH2D*       fHistoGammaTrueSecondaryFromXFromK0sCalo_MCPt_recPt_MC_Rebin                        = NULL;
TH2D*       fHistoGammaTrueSecondaryFromXFromK0lCaloUnConv_MCPt_recPt_MC                        = NULL;
TH2D*       fHistoGammaTrueSecondaryFromXFromK0lCaloConv_MCPt_recPt_MC                          = NULL;
TH2D*       fHistoGammaTrueSecondaryFromXFromK0lCalo_MCPt_recPt_MC                              = NULL;
TH2D*       fHistoGammaTrueSecondaryFromXFromK0lCalo_MCPt_recPt_MC_Rebin                        = NULL;
TH2D*       fHistoGammaTrueSecondaryFromXFromLambdaCaloUnConv_MCPt_recPt_MC                     = NULL;
TH2D*       fHistoGammaTrueSecondaryFromXFromLambdaCaloConv_MCPt_recPt_MC                       = NULL;
TH2D*       fHistoGammaTrueSecondaryFromXFromLambdaCalo_MCPt_recPt_MC                           = NULL;
TH2D*       fHistoGammaTrueSecondaryFromXFromLambdaCalo_MCPt_recPt_MC_Rebin                     = NULL;

TH1D*       fHistoGammaTruePrimaryCaloMCPt                                                      = NULL;
TH1D*       fHistoGammaTruePrimaryCaloMCPtOrBin                                                 = NULL;
TH1D*       fHistoFracAllGammaCaloToSec                                                         = NULL;
TH1D*       fHistoFracAllGammaCaloToSecOrBin                                                    = NULL;
TH1D*       fHistoFracAllGammaCaloToSecFromXFromK0s                                             = NULL;
TH1D*       fHistoFracAllGammaCaloToSecFromXFromK0sOrBin                                        = NULL;
TH1D*       fHistoFracAllGammaCaloToSecFromXFromK0l                                             = NULL;
TH1D*       fHistoFracAllGammaCaloToSecFromXFromK0lOrBin                                        = NULL;
TH1D*       fHistoFracAllGammaCaloToSecFromXFromLambda                                          = NULL;
TH1D*       fHistoFracAllGammaCaloToSecFromXFromLambdaOrBin                                     = NULL;

TH1D*       fHistoGammaCaloMCPurity                                                             = NULL;
TH1D*       fHistoGammaMCrecPrimaryCaloPt                                                       = NULL;
TH1D*       fHistoGammaMCrecPrimaryCaloPtOrBin                                                  = NULL;
TH1D*       fHistoGammaCaloMCTruePurity                                                         = NULL;
TH1D*       fHistoGammaCaloMCTruePurityOrBin                                                    = NULL;
TH1D*       fHistoGammaCaloMCRecoEff                                                            = NULL;
TH1D*       fHistoGammaCaloMCPrimaryRecoEff                                                     = NULL;
TH1D*       fHistoGammaCaloMCPrimaryRecoEffMCPt                                                 = NULL;
TH1D*       fHistoGammaCaloMCBackground                                                         = NULL;

TH1D*       fHistoGammaTrueCaloConvPt                                                           = NULL;
TH1D*       fHistoGammaTrueCaloConvPtOrBin                                                      = NULL;
TH1D*       fHistoGammaTruePrimaryCaloConvPt                                                    = NULL;
TH1D*       fHistoGammaTruePrimaryCaloConvPtOrBin                                               = NULL;
TH1D*       fHistoGammaTrueSecondaryCaloConvPt                                                  = NULL;
TH1D*       fHistoGammaTrueSecondaryCaloConvPtOrBin                                             = NULL;
TH1D*       fHistoGammaTrueSecondaryCaloConvFromXFromK0sPt                                      = NULL;
TH1D*       fHistoGammaTrueSecondaryCaloConvFromXFromK0sPtOrBin                                 = NULL;
TH1D*       fHistoGammaTrueSecondaryCaloConvFromXFromLambdaPt                                   = NULL;
TH1D*       fHistoGammaTrueSecondaryCaloConvFromXFromLambdaPtOrBin                              = NULL;
TH2D*       fHistoGammaTruePrimaryCaloConv_recPt_MCPt_MC                                        = NULL;
TH2D*       fHistoGammaTruePrimaryCaloConv_recPt_MCPt_MC_Rebin                                  = NULL;
TH1D*       fHistoGammaTruePrimaryCaloConvMCPt                                                  = NULL;
TH1D*       fHistoGammaTruePrimaryCaloConvMCPtOrBin                                             = NULL;

TH1D*       fHistoGammaTrueCaloUnConvPt                                                         = NULL;
TH1D*       fHistoGammaTrueCaloUnConvPtOrBin                                                    = NULL;
TH1D*       fHistoGammaTruePrimaryCaloUnConvPt                                                  = NULL;
TH1D*       fHistoGammaTruePrimaryCaloUnConvPtOrBin                                             = NULL;
TH1D*       fHistoGammaTrueSecondaryCaloUnConvPt                                                = NULL;
TH1D*       fHistoGammaTrueSecondaryCaloUnConvPtOrBin                                           = NULL;
TH1D*       fHistoGammaTrueSecondaryCaloUnConvFromXFromK0sPt                                    = NULL;
TH1D*       fHistoGammaTrueSecondaryCaloUnConvFromXFromK0sPtOrBin                               = NULL;
TH1D*       fHistoGammaTrueSecondaryCaloUnConvFromXFromLambdaPt                                 = NULL;
TH1D*       fHistoGammaTrueSecondaryCaloUnConvFromXFromLambdaPtOrBin                            = NULL;
TH2D*       fHistoGammaTruePrimaryCaloUnConv_recPt_MCPt_MC                                      = NULL;
TH2D*       fHistoGammaTruePrimaryCaloUnConv_recPt_MCPt_MC_Rebin                                = NULL;
TH1D*       fHistoGammaTruePrimaryCaloUnConvMCPt                                                = NULL;
TH1D*       fHistoGammaTruePrimaryCaloUnConvMCPtOrBin                                           = NULL;

TH2D*       f2DHistoGammaTrueSecondaryCaloUnConvPt                                              = NULL;
TH2D*       f2DHistoGammaTrueSecondaryCaloUnConvMCPt                                            = NULL;
TH2D*       f2DHistoGammaTrueSecondaryCaloConvPt                                                = NULL;
TH2D*       f2DHistoGammaTrueSecondaryCaloConvMCPt                                              = NULL;
TH2D*       f2DHistoGammaTrueSecondaryCaloPt                                                    = NULL;
TH2D*       f2DHistoGammaTrueSecondaryCaloMCPt                                                  = NULL;



//******************** Definition of functions ****************************
void     RebinSpectrum                      (   TH1D*       Spectrum, 
                                                TString     NewName = ""                        );
void     RebinSpectrumToDCAzDistBinning     (   TH1D*       Spectrum,
                                                TString     NewName = ""                        );
void     CalculatePileUpBackground          (   Bool_t      doMC                                );
void     CalculateGammaCorrection           (                                                   );
void     CalculatePileUpGammaCorrection     (                                                   );
void     Initialize                         (   TString     setPi0,
                                                TString     Energy,
                                                Int_t       numberOfBins,
                                                Int_t       mode,
                                                Bool_t      addSig                              );
void     SaveHistos                         (   Int_t       isMC,
                                                TString     fCutID, 
                                                TString     fPrefix3,
                                                Bool_t      PileUpCorrection                    );
void     SaveCorrectionHistos               (   TString     fCutID, 
                                                TString     fPrefix3,
                                                Bool_t      PileUpCorrection                    );
void     SaveDCAHistos                      (   Int_t       isMC, 
                                                TString     fCutID, 
                                                TString     fPrefix3                            );
void     FillDCAHistogramsFromTree          (   TTree*      dcaTree,
                                                Bool_t      isMC                                );
void     FillMassHistosArray                (   TH2D*       fGammaGammaInvMassVSPtDummy         );
void     ProduceBckProperWeighting          (   TH2D*       fHistoBckZM, 
                                                TH2D*       fHistoMotherZM                      );
Bool_t   CompareArrays                      (   Int_t       nEntriesA,
                                                Double_t*   arrayA,
                                                Int_t       nEntriesB,
                                                Double_t*   arrayB                              );
Bool_t   CalculatePileUpSubtractedDCAz      (   TH1D*       trueGamma,
                                                TH1D*       trueSubGamma,
                                                TH1D*       trueGammaX,
                                                TH1D*       &trueSubGammaX                      );
Bool_t   CalculateDCAzDistributionRatio     (   TH1D***     numerator,
                                                TH1D***     denominator,
                                                Int_t       categoryFirst,
                                                Int_t       categoryLast,
                                                TH1D*       &ratio                              );
Bool_t   CalculateDCAzDistributionRatio     (   TH1D***     numerator,
                                                TH1D****     denominator,
                                                Int_t       categoryFirst,
                                                Int_t       backgroundExtractionMethod,
                                                Int_t       categoryLast,
                                                TH1D*       &ratio                              );
Bool_t    CalculatePileUpCorrectionFactor   (   TH1D*       ratioWithWithoutPileUp,
                                                TH1D*       &pileupCorrectionFactor,
                                                TF1*        &fitToRatio                         );
Bool_t   LoadSecondariesFromCocktailFile    (   TString, 
                                                TString                                         );     // Loads secondary neutral pion input graphs from file
void     Delete                             ();
void     FillMassMCTrueMesonHistosArrays    (   TH2D* fHistoTrueMesonPrimInvMassVSPtFill, 
                                                TH2D** fHistoTrueMesonSecInvMassVSPtFill        );
void     CheckForNULLForPointer             (   TH1D* fDummy1                                   );
void     PlotAdditionalDCAz                 (   Int_t       isMC, 
                                                TString     fCutID                              );
