Double_t fHoleRadius = 2.;


Double_t AsymmGaus(Double_t *x, Double_t *par);
Double_t GausWithHole(Double_t *x, Double_t *par);
void InitializeIntRange(TString setPi0);
void PlotDCADistPtBinWithFitAndEstimateCat(TString namePlot, TString nameCanvas, TString namePad,  Int_t fRowPlot, Int_t fColumnPlot, Int_t fStartBinPtRange, Int_t fNumberPtBins, Double_t* fRangeBinsPt,  Bool_t fMonteCarloInfo,  TString dateDummy, Int_t category);
void PlotDCADistPtBinWithMCSplitCat(TString namePlot, TString nameCanvas, TString namePad,  Int_t fRowPlot, Int_t fColumnPlot, Int_t fStartBinPtRange, Int_t fNumberPtBins, Double_t* fRangeBinsPt,  Bool_t fMonteCarloInfo,  TString dateDummy, Int_t category);
void PlotDCADistPtBinSubtractedSplitCat(TString namePlot, TString nameCanvas, TString namePad,  Int_t fRowPlot, Int_t fColumnPlot, Int_t fStartBinPtRange, Int_t fNumberPtBins, Double_t* fRangeBinsPt,  Bool_t fMonteCarloInfo,  TString dateDummy, Int_t category);
void PlotDCADistPtBinSubtractedRatioSplitCat(TString namePlot, TString nameCanvas, TString namePad,  Int_t fRowPlot, Int_t fColumnPlot, Int_t fStartBinPtRange, Int_t fNumberPtBins, Double_t* fRangeBinsPt,  Bool_t fMonteCarloInfo,  TString dateDummy, Int_t category);
void PlotDCADistPtBinWithFitAndEstimate(TString namePlot, TString nameCanvas, TString namePad,  Int_t fRowPlot, Int_t fColumnPlot, Int_t fStartBinPtRange, Int_t fNumberPtBins, Double_t* fRangeBinsPt,  Bool_t fMonteCarloInfo,  TString dateDummy);
void PlotDCADistPtBinWithFitAndEstimateGoodCat(TString namePlot, TString nameCanvas, TString namePad,  Int_t fRowPlot, Int_t fColumnPlot, Int_t fStartBinPtRange, Int_t fNumberPtBins, Double_t* fRangeBinsPt,  Bool_t fMonteCarloInfo,  TString dateDummy);

void PlotDCADistPtBinWithMCSplit(TString namePlot, TString nameCanvas, TString namePad,  Int_t fRowPlot, Int_t fColumnPlot, Int_t fStartBinPtRange, Int_t fNumberPtBins, Double_t* fRangeBinsPt,  Bool_t fMonteCarloInfo,  TString dateDummy);

void PlotInvMassPtBinCat(TString namePlot, TString nameCanvas, TString namePad,  Int_t fRowPlot, Int_t fColumnPlot, Int_t fStartBinPtRange, Int_t fNumberPtBins, Double_t* fRangeBinsPt,  Bool_t fMonteCarloInfo,  TString dateDummy, Int_t category);
void PlotInvMassPtBin(TString namePlot, TString nameCanvas, TString namePad,  Int_t fRowPlot, Int_t fColumnPlot, Int_t fStartBinPtRange, Int_t fNumberPtBins, Double_t* fRangeBinsPt,  Bool_t fMonteCarloInfo,  TString dateDummy);


void DrawGammaDCAHisto( TH1* ,
             TString , TString , TString ,
             Float_t , Float_t ,Int_t ,Double_t );
void DrawGammaInvMassHisto( TH1* ,
             TString , TString , TString ,
             Float_t , Float_t ,Int_t ,Double_t );

TH1F* fHistDCAZUnderMeson_MesonPt[6][30];
TH1F* fHistDCAZUnderMesonBG1_MesonPt[6][30];
TH1F* fHistDCAZUnderMesonBG2_MesonPt[6][30];
TH1F* fHistInvMass_MesonPt[6][30];

TH1D* fHistDCAZUnderMeson_Visual_CatIter_MC_PtWise[6][30];
TH1D* fHistDCAZUnderMeson_Visual_CatIter_Data_PtWise[6][30];
TH1D* fHistDCAZBG_Visual_CatIter_Data_PtWise[6][30];
TH1D* fHistDCAZSubtractedUnderMeson_Visual_CatIter_Data_PtWise[6][30];

//
// TF1*  fFitInvMass_MesonPt[6][30];
TH1F* fHistDCAZTruePrimaryMesonGammaGamma_MesonPt[6][30];
TH1F* fHistDCAZTruePrimaryMesonDalitz_MesonPt[6][30];
TH1F* fHistDCAZTrueSecondaryMesonFromK0s_MesonPt[6][30];
TH1F* fHistDCAZTrueSecondaryMesonFromEta_MesonPt[6][30];
TH1F* fHistDCAZTrueSecondaryMesonFromSomething_MesonPt[6][30];
TH1F* fHistDCAZTrueBackground_MesonPt[6][30];
TH1F* fHistDCAZGarbage_MesonPt[6][30];

TH1F* fHistDCAZUnderMeson_MesonPt_AllCat[30];
TH1F* fHistDCAZUnderMesonBG1_MesonPt_AllCat[30];
TH1F* fHistDCAZUnderMesonBG2_MesonPt_AllCat[30];
TH1F* fHistInvMass_MesonPt_AllCat[30];
// TF1*  fFitInvMass_MesonPt_AllCat[30];
TH1F* fHistDCAZUnderMeson_MesonPt_GoodCat[30];
TH1F* fHistDCAZTruePrimaryMesonGammaGamma_MesonPt_AllCat[30];
TH1F* fHistDCAZTruePrimaryMesonDalitz_MesonPt_AllCat[30];
TH1F* fHistDCAZTrueSecondaryMesonFromK0s_MesonPt_AllCat[30];
TH1F* fHistDCAZTrueSecondaryMesonFromEta_MesonPt_AllCat[30];
TH1F* fHistDCAZTrueSecondaryMesonFromSomething_MesonPt_AllCat[30];
TH1F* fHistDCAZTrueBackground_MesonPt_AllCat[30];
TH1F* fHistDCAZGarbage_MesonPt_AllCat[30];

TH1F* fHistDCAZUnderMesonBGEstimate_MesonPt[5][6][30];
TH1F* fHistDCAZUnderMesonBGEstimate_MesonPt_AllCat[30];

TH1F* fHistDCAZUnderMeson_AllPt                     [6];
TH1F* fHistDCAZTrueBackground_AllPt                 [6];
TH1F* fHistDCAZGarbage_AllPt                        [6];
TH1F* fHistDCAZTruePrimaryMesonDalitz_AllPt         [6];
TH1F* fHistDCAZTruePrimaryMesonGammaGamma_AllPt     [6];
TH1F* fHistDCAZTrueSecondaryMesonFromEta_AllPt      [6];
TH1F* fHistDCAZTrueSecondaryMesonFromK0s_AllPt      [6];
TH1F* fHistDCAZTrueSecondaryMesonFromSomething_AllPt[6];

TH1F* fHistDCAZUnderMesonAllCat_AllPt;
TH1F* fHistDCAZTruePrimaryMesonGammaGammaAllCat_AllPt;
TH1F* fHistDCAZTruePrimaryMesonDalitzAllCat_AllPt;
TH1F* fHistDCAZTrueSecondaryMesonFromK0sAllCat_AllPt;
TH1F* fHistDCAZTrueSecondaryMesonFromEtaAllCat_AllPt;
TH1F* fHistDCAZTrueSecondaryMesonFromSomethingAllCat_AllPt;
TH1F* fHistDCAZTrueBackgroundAllCat_AllPt;
TH1F* fHistDCAZGarbageAllCat_AllPt;

TH1D* fEventQuality;

TF1* fFitDCAZUnderMesonBGEstimate_MesonPt_AllCat[30];

Double_t fYMaxMeson = 0.9;
Double_t fNEvents = 0;
Double_t fMaxDcaZPhoton = 1000;
// Int_t ** numberMeson =NULL;
Double_t *fMesonIntRange = NULL;
Double_t *fMesonIntRangeBG1 = NULL;
Double_t *fMesonIntRangeBG2 = NULL;
Int_t numberInvMassBins = 0;
TString fTextCent;
TString fEnergyFlag;
TString fEnergyText;
TString fMCFlag;
TString fPrefix2;
TString fPeriodFlag;
TString ftextDayth;
TString fdate;
TString  fCollisionSystem;
TString  fTextMeasurement;
TString fCutSelection;
TString fCutSelection2;
TString fBackgroundMultCutNumber;

Double_t fMesonMassExpect = 0;
Int_t fMesonId = 0;
Double_t    fMesonWidthExpect=               0;
Double_t    fMesonLambdaTail=             0;
Double_t    *fMesonWidthRange =           NULL;
Double_t    *fMesonLambdaTailRange =         NULL;
TString fMesonType = "";
