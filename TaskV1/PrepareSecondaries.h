// provided by Gamma Conversion Group, $ALICE_PHYSICS/PWGGA/GammaConv ;https://twiki.cern.ch/twiki/bin/view/ALICE/PWG4GammaConversion
#include "../FlexibleCocktail/cocktail.h"
#include "../FlexibleCocktail/cocktail.cpp"

//************************** Some general definitions *************************************
TString     fAnalyzedMeson                              = "";
TString     fEnergyFlag                                 = "";
TString     fPeriodFlag                                 = "";
TString     fDirectPhoton                               = "";
TString     fSuffix                                     = "";
TString     fCutSelection                               = "";
TString     fEventCutSelection                          = "";
TString     fGammaCutSelection                          = "";
TString     fClusterCutSelection                        = "";
TString     fElectronCutSelection                       = "";
TString     fMesonCutSelection                          = "";
TString     frapidityAndeta                             = "";
Double_t    ptMin                                       = 0;
Double_t    ptMax                                       = 20;
Double_t    ptPlotMin                                   = 0;
Double_t    ptPlotMax                                   = 20;
Double_t    fRapidity                                   = 0;
Double_t    fPseudoRapidity                             = 0;
Int_t       fMode                                       = 0;
Float_t     nEvents                                     = 0;

Int_t       nMotherParticleToAnalyse                    = 21;
const Int_t nMotherParticles                            = 21;
TString     motherParticles[nMotherParticles]           = {"Eta","K0s","K0l","Lambda","rho0","EtaPrime","omega","rho+","rho-","phi","J/psi","Delta-","Delta0","Delta+","Delta++","Sigma0","K+","K-","Omega+","Omega-","K*(892)0"};
TString     motherParticlesPDG[nMotherParticles]        = {"221","310","130","3122","113","331","223","213","-213","333","443","1114","2114","2214","2224","3212","321","-321","-3334","3334","313"};
TString     motherParticlesLatex[nMotherParticles]      = { "#eta","K^{0}_{S}","K^{0}_{L}","#Lambda","#rho^{0}","#eta'","#omega","#rho^{+}","#rho^{-}","#phi","J/#psi","#Delta^{-}",
                                                            "#Delta^{0}","#Delta^{+}","#Delta^{++}","#Sigma^{0}","K^{+}","K^{-}","#Omega^{+}","#Omega^{-}","K^{*}(892)^{0}"};

const Int_t nCocktailInputParticles                     = 3;
TString cocktailInputParticles[nCocktailInputParticles] = {"NKaonSubS", "NKaonSubL", "Lambda"};

//ctau given in (cm) below! - zero means: ctau < 0.01cm
Double_t    motherParticles_ctau[nMotherParticles]      = {0,2.68,1533.74,7.89,0,0,0,0,0,0,0,0,0,0,0,0,371.2,371.2,2.461,2.461,0};
Double_t    motherFactorDecayLength[nMotherParticles]   = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
Int_t       motherParticleDec[nMotherParticles]         = {2,256,65536,131072,4,16,8,8192,16384,32,64,2048,4096,1024,512,128,262144,524288,1048576,2097152,32768};
Color_t     cocktailColor[nMotherParticles]             = {kRed+2,kBlue+1,kOrange+1,kYellow+2,kAzure-2,kGreen+2,kRed-2,kViolet,kMagenta,kViolet+2,kBlue-3,kTeal+9,kCyan+2,
                                                            kMagenta+2,kCyan+4,kViolet+4,kRed+4,kBlue+3,kOrange+3,kYellow+4,kAzure-4};
Style_t     cocktailMarker[nMotherParticles]            = {20,21,24,25,20,21,24,25,20,21,24,25,20,21,24,25,20,21,24,25,20};
TString     decayChannelsLatex[nMotherParticles][18];
Double_t    decayChannelsBR[nMotherParticles][18];

// additional scaling factor (param per inel. event vs. MB event)
Double_t    eventNormScalingFactor                      = 1.;

//************************** cocktail settings ********************************************
Double_t ptGenMin                                       = 0;
Double_t ptGenMax                                       = 20;
Int_t    nParticles                                     = 1000;
Int_t    selectedMothers                                = 62591;
Bool_t   hasMother[nMotherParticles]                    = {0};
Double_t mtScaleFactor[nMotherParticles]                = {1.};

//********************** Objects for reading the cocktail files **************************

TFile*      fileCocktail                                    = NULL;
TDirectoryFile* topDirCocktail                              = NULL;
TList*      histoListCocktail                               = NULL;
TTree*      cocktailSettingsTree                            = NULL;
TList*      cocktailSettingsList                            = NULL;

//************************** Objects for Flexible Cocktail ********************************

Bool_t      doFlexCocktail                              = kFALSE;
std::map<Int_t, TF1> dndpt_hadron_func;
std::map<Int_t, TH1D> dndpt_hadron_hist;
std::map<Int_t, TH1D> dndpt_pi0_hist;
const Int_t motherParticlesSecGammas[3]                 = {310, 130, 3122};
std::map<Int_t, TH1D> dndpt_sec_gamma_hist;
cocktail ct_pi0;
cocktail ct_sec_gamma;

//************************** Declaration of histograms ************************************
TH1F*  fDeltaPt                                         = NULL;
TH1F*  histoNEvents                                     = NULL;
TH1F*  histMtScalingFactors                             = NULL;
TH1F** histoRatioPi0FromXToPi0Param                     = NULL;
TH1F** histoDecayChannels                               = NULL;
TH1F** histoDecayChannelsBR                             = NULL;

TH2F** histoMesonDaughterPtY                            = NULL;
TH2F** histoMesonDaughterPtYCorr                        = NULL;
TH2F** histoMesonDaughterPtPhi                          = NULL;
TH1F** histoMesonDaughterPtOrBin                        = NULL;
TH1F** histoMesonDaughterYOrBin                         = NULL;
TH1F** histoMesonDaughterPhiOrBin                       = NULL;

TH1F** histoMesonMotherCocktailInputPtMeasBin           = NULL;     // K0s, (K0l,) Lambda
TH1F** histoMesonMotherPtMeasBin                        = NULL;

TH2F** histoMesonMotherPtY                              = NULL;
TH2F** histoMesonMotherPtPhi                            = NULL;
TH1F** histoMesonMotherPtOrBin                          = NULL;
TH1F** histoMesonMotherYOrBin                           = NULL;
TH1F** histoMesonMotherPhiOrBin                         = NULL;

TH2F** histoGammaFromXFromMotherPtY                     = NULL;
TH2F** histoGammaFromXFromMotherPtYCorr                 = NULL;
TH2F** histoGammaFromXFromMotherPtPhi                   = NULL;
TH1F** histoGammaFromXFromMotherPtOrBin                 = NULL;
TH1F** histoGammaFromXFromMotherYOrBin                  = NULL;
TH1F** histoGammaFromXFromMotherPhiOrBin                = NULL;

//************************** Declaration of lists *****************************************
TList** listYSlicesMesonDaughter                        = NULL;
TList** listYSlicesGammaFromXFromMother                 = NULL;

//************************** Cocktail input ***********************************************
TFile* cocktailInputFile                                = NULL;
TList* cocktailInputList                                = NULL;
TF1**  cocktailInputParametrizations                    = NULL;
TF1**  cocktailInputParametrizationsMtScaled            = NULL;
TF1*   cocktailInputParametrizationPi0                  = NULL;
TF1*   paramScaleBase                                   = NULL;

//************************** Methods ******************************************************
void        Initialize                              (   TString     meson,
                                                        TString     energy,
                                                        Int_t       numberOfBins        );
void        RebinSpectrum                           (   TH1F*       Spectrum,
                                                        TH1F*       SpectrumForBinning,
                                                        TString     NewName             );
void        SaveMesonHistos                         (   TString     sEnergy             );
void        SavePhotonHistos                        (   TString     sEnergy             );
Double_t    GetMass                                 (   TString     particle            );
void        SetHistogramTitles                      (   TH1F*       input,
                                                        TString     title,
                                                        TString     xTitle,
                                                        TString     yTitle              );
TH1D*       CalculateRatioToTF1                     (   TH1D*       hist,
                                                        TF1*        func                );
void        CorrectForNonFlatRapidity               (   TH2F*       histCorr,
                                                        TH2F*       histOr,
                                                        TList*      list                );
void        CreateBRTableLatex                      (                                   );
Int_t       GetMinimumBinAboveThreshold             (   TH1F*               hist,
                                                        Double_t            thres       );
TList*      GetCocktailInputList                    (   TString             energy,
                                                        TString             centrality  );
TH1F*       GetCocktailInputSpectrum                (   TList*              list,
                                                        Int_t               particle,
                                                        TString name                    );
TH1F*       TransformGraphToTH1F                    (   TGraphErrors*       graph       );
TH1F*       TransformGraphToTH1F                    (   TGraphAsymmErrors*  graph       );


