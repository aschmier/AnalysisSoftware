// provided by Gamma Conversion Group, $ALICE_PHYSICS/PWGGA/GammaConv ;https://twiki.cern.ch/twiki/bin/view/ALICE/PWG4GammaConversion

//************************** Some general definitions *************************************
//TString   fDate                                           = "";
TString     fEnergyFlag                                     = "";
TString     fPeriodFlag                                     = "";
TString     fdirectphoton                                   = "";
TString     fSuffix                                         = "";
TString     fCutSelection                                   = "";
TString     fEventCutSelection                              = "";
TString     fGammaCutSelection                              = "";
TString     fClusterCutSelection                            = "";
TString     fElectronCutSelection                           = "";
TString     fMesonCutSelection                              = "";
Double_t    ptMin                                           = 0;
Double_t    ptMax                                           = 20;
Double_t    ptPlotMin                                       = 0;
Double_t    ptPlotMax                                       = 20;
Double_t    fRapidity                                       = 0;
Int_t       fMode                                           = 0;
Float_t     nEvents                                         = 0;

const Int_t nMotherParticles                                = 17;
TString     motherParticles[nMotherParticles]               = {"Pi0","Eta","EtaPrim","omega","rho0","rho+","rho-","phi","J/psi","Delta-","Delta0","Delta+","Delta++","Sigma0","K0s","K0l","Lambda"};
TString     motherParticlesPDG[nMotherParticles]            = {"111","221","331","223","113","213","-213","333","443","1114","2114","2214","2224","3212","310","130","3122"};
TString     motherParticlesLatex[nMotherParticles]          = {"#pi^{0}","#eta","#eta'","#omega","#rho^{0}","#rho^{+}","#rho^{-}","#phi","J/#psi","#Delta^{-}","#Delta^{0}","#Delta^{+}",
                                                                "#Delta^{++}","#Sigma^{0}","K^{0}_{S}","K^{0}_{L}","#Lambda"};
Int_t       motherParticleDec[nMotherParticles]             = {1,2,16,8,4,8192,16384,32,64,2048,4096,1024,512,128,256,65536,131072};
Color_t     cocktailColor[nMotherParticles]                 = {kRed+2,kBlue+1,kOrange+1,kYellow+2,kAzure-2,kGreen+2,kRed-2,kViolet,kMagenta,kViolet+2,kBlue-3,kTeal+9,kCyan+2,kMagenta+2,kCyan+4,
                                                                kViolet+4,kAzure-4};
Style_t     cocktailMarker[nMotherParticles]                = {20,21,24,25,20,21,24,25,20,21,24,25,20,21,24,25,20};
TString     decayChannelsLatex[nMotherParticles][18];
Double_t    decayChannelsBR[nMotherParticles][18];

// this switch is to check the scaling factors by producing the gamma pt spec also from the gamma pt vs phi dist
Bool_t      doGammaPtOrBinFromPtPhi                         = kTRUE;

//************************** cocktail settings ********************************************
Double_t    ptGenMin                                        = 0;
Double_t    ptGenMax                                        = 20;
Int_t       nParticles                                      = 1000;
Int_t       selectedMothers                                 = 62591;
Bool_t      hasMother[nMotherParticles]                     = {0};
Double_t    mtScaleFactor[nMotherParticles]                 = {1.};

//************************** Declaration of histograms ************************************
TH1F*       fDeltaPt                                        = NULL;
TH1F*       histoNEvents                                    = NULL;
TH1F*       histMtScalingFactors                            = NULL;
TH1D*       histoPi0YieldData                               = NULL;
TH1F**      histoDecayChannels                              = NULL;
TH1F**      histoDecayChannelsBR                            = NULL;
TH2F*       histoGammaSumPtY                                = NULL;
TH2F*       histoGammaSumPtPhi                              = NULL;
TH1F*       histoGammaSumPtOrBin                            = NULL;
TH1F*       histoGammaSumPtOrBin2                           = NULL;
TH1F*       histoGammaSumPt                                 = NULL;
TH1F*       histoGammaSumYOrBin                             = NULL;
TH1F*       histoGammaSumPhiOrBin                           = NULL;
TH2F**      histoGammaPtY                                   = NULL;
TH2F**      histoGammaPtPhi                                 = NULL;
TH1F**      histoGammaPtOrBin                               = NULL;
TH1F**      histoGammaPtOrBin2                              = NULL;
TH1F**      histoGammaPt                                    = NULL;
TH1F**      histoGammaYOrBin                                = NULL;
TH1F**      histoGammaPhiOrBin                              = NULL;
TH2F**      histoGammaMotherPtY                             = NULL;
TH1F**      histoGammaMotherPtOrBin                         = NULL;
TH1F**      histoGammaMotherPt                              = NULL;
TH1F**      histoGammaMotherYOrBin                          = NULL;
TH2F**      histoGammaMotherPtPhi                           = NULL;
TH1F**      histoGammaMotherPhiOrBin                        = NULL;

//************************** Cocktail input ***********************************************
TFile*      cocktailInputFile                               = NULL;
TList*      cocktailInputList                               = NULL;
TF1*        cocktailInputParametrizationProton              = NULL;
TF1**       cocktailInputParametrizations                   = NULL;
TF1**       cocktailInputParametrizationsMtScaled           = NULL;
TF1*        paramScaleBase                                  = NULL;


//************************** Methods ******************************************************
void        Initialize                                  (   TString energy,
                                                            Int_t   numberOfBins        );
void        RebinSpectrum                               (   TH1F*   Spectrum,
                                                            TString NewName             );
TH1F*       ConvertYieldHisto                           (   TH1F*   input               );
void        SaveHistos                                  (                               );
TF1*        MtScaledParam                               (   TF1*    param,
                                                            Int_t   particleNumber      );
Double_t    GetMass                                     (   TString particle            );
void        SetHistogramTitles                          (   TH1F*   input,
                                                            TString title,
                                                            TString xTitle,
                                                            TString yTitle              );
void        DeleteObjects                               (                               );
TH1D*       CalculateRatioToTF1                         (   TH1D*   hist,
                                                            TF1*    func                );
void        CreateBRTableLatex                          (                               );
