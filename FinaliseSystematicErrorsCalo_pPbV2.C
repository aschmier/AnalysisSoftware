#include <Riostream.h>
#include <fstream>
#include "TMath.h"
#include <stdlib.h>
#include <fstream>
#include <math.h>
#include <TROOT.h>
#include <TApplication.h>
#include <TPaveLabel.h>
#include <TSystem.h>
#include <TFrame.h>
#include <TStyle.h>
#include <TString.h>
#include "TGaxis.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH1D.h"
#include "TH2F.h"
#include "TF1.h"
#include "TVirtualFitter.h"
#include "TObject.h"
#include "TCanvas.h"
#include "TMultiGraph.h"
#include "TLegend.h"
#include "TDatabasePDG.h"
#include "TMinuit.h"
#include "TBenchmark.h"
#include "TRandom.h"
#include "TLatex.h"
#include "TASImage.h"
#include "TPostScript.h"
#include "TGraphErrors.h"
#include "TArrow.h"
#include "TGraphAsymmErrors.h"
#include "TGaxis.h"
#include "TMarker.h"
#include "CommonHeaders/PlottingGammaConversionHistos.h"
#include "CommonHeaders/PlottingGammaConversionAdditional.h"
#include "CommonHeaders/FittingGammaConversion.h"
#include "CommonHeaders/ConversionFunctionsBasicsAndLabeling.h"
#include "CommonHeaders/ConversionFunctions.h"

void FinaliseSystematicErrorsCalo_pPbV2(    const char* nameDataFileErrors  = "",
                                            TString energy                  = "",
                                            TString meson                   = "",
                                            Int_t numberOfPtBins            = 1,
                                            Int_t numberCutStudies          = 1,
                                            Float_t startPtSys              = 0,
                                            TString additionalName          = "pp",
                                            TString additionalNameOutput    = "",
                                            TString suffix                  = "eps",
                                            Int_t mode                      = 4,
                                            Bool_t useMBSyst                = kFALSE
                                        ){

 // ***************************************************************************************************
    // ****************************** General style settings *********************************************
    // ***************************************************************************************************
    StyleSettingsThesis();
    SetPlotStyle();

    // ***************************************************************************************************
    // ****************************** Create output directory ********************************************
    // ***************************************************************************************************
    TString additionalName2 = additionalName;
    if(additionalName.Contains("ZNA") || additionalName.Contains("CL1")) additionalName2.Replace(3,1,"_");
    cout << additionalName2.Data() << endl;
    gSystem->Exec("mkdir -p SystematicErrorsCalculatedCalo");
    gSystem->Exec(Form("mkdir -p SystematicErrorsCalculatedCalo/%s",additionalName2.Data()));

    // ***************************************************************************************************
    // ***************************** labeling and color settings *****************************************
    // ***************************************************************************************************
    TString date                            = ReturnDateString();
    TString dateForOutput                   = ReturnDateStringForOutput();
    TString collisionSystem                 = ReturnFullCollisionsSystem(energy);
    TString detectionSystem                 = ReturnTextReconstructionProcess(4);
    TString energyForOutput                 = energy;
    energyForOutput.ReplaceAll(".","_");

    // ***************************************************************************************************
    // ******************************* general variable definition  **************************************
    // ***************************************************************************************************
    Int_t   numberOfEntriesPos              = 0;
    Int_t   numberOfEntriesNeg              = 0;
    const Int_t nPtBins                     = numberOfPtBins;
    const Int_t nCuts                       = numberCutStudies;
    Double_t* ptBins                        = NULL;
    Double_t* ptBinsErr                     = NULL;
    TString nameCutVariation[14];
    TString nameCutVariationSC[14];
    Color_t color[20];
    Style_t markerStyle[20];

    TString nameCutVariationSCCurrent[14]   = { "YieldExtraction", "OpeningAngle", "ClusterMinEnergy", "ClusterNCells", "ClusterNonLinearity",
                                                "ClusterTrackMatchingCalo", "ClusterM02","ClusterMaterialTRD", "Rapidity", "ClusterEnergyScale",
                                                "Efficiency", "Trigger", "Alpha", "YieldExtractionPi0"};

    if (meson.Contains("Ratio"))
        nameCutVariationSCCurrent[13]       = "YieldExtraction_pp";
                                                
    for (Int_t k = 0; k < 14; k++){
        color[k]                            = GetColorSystematics( nameCutVariationSCCurrent[k], 4);
        markerStyle[k]                      = GetMarkerStyleSystematics( nameCutVariationSCCurrent[k], 4);
    }
    for (Int_t i = 0; i < numberCutStudies; i++){
        nameCutVariation[i]                 = GetSystematicsName(nameCutVariationSCCurrent[i]);
        nameCutVariationSC[i]               = nameCutVariationSCCurrent[i];
    }
    if (meson.CompareTo("EtaToPi0") == 0){
        nameCutVariation[0]                 = "yield extraction #eta";
    }


    // ***************************************************************************************************
    // ******************************** Booleans for smoothing *******************************************
    // ***************************************************************************************************
    Bool_t bsmooth[14]                      = { 0, 0, 0, 0, 0,      0, 0, 0, 0, 0,      0, 0, 0, 0 };
    Bool_t bsmoothMBPi05TeV[3][14]          = { { 1, 1, 1, 1, 1,      1, 1, 1, 1, 1,      1, 1, 1, 1 },
                                                { 1, 1, 1, 1, 1,      1, 1, 1, 1, 1,      1, 1, 1, 1 },
                                                { 1, 1, 1, 1, 1,      1, 1, 1, 1, 1,      1, 1, 1, 1 } } ;
    Bool_t bsmoothMBEta5TeV[3][14]          = { { 1, 1, 1, 1, 1,      1, 1, 1, 1, 1,      1, 1, 1, 1 },
                                                { 1, 1, 1, 1, 1,      1, 1, 1, 1, 1,      1, 1, 1, 1 },
                                                { 1, 1, 1, 1, 1,      1, 1, 1, 1, 1,      1, 1, 1, 1 } };
    Bool_t bsmoothMBPi0Ratio5TeV[3][14]     = { { 1, 1, 1, 1, 1,      1, 1, 1, 1, 1,      1, 1, 1, 1 },
                                                { 1, 1, 1, 1, 1,      1, 1, 1, 1, 1,      1, 1, 1, 0 },
                                                { 1, 1, 1, 1, 1,      1, 1, 1, 1, 1,      1, 1, 1, 0 } } ;
    Bool_t bsmoothMBEtaRatio5TeV[3][14]     = { { 1, 1, 1, 1, 1,      1, 1, 1, 1, 1,      1, 1, 1, 1 },
                                                { 1, 1, 1, 1, 1,      1, 1, 1, 1, 1,      1, 1, 1, 1 },
                                                { 0, 1, 1, 1, 1,      1, 1, 1, 1, 1,      1, 1, 1, 1 } };
    Bool_t bsmoothMBEtaToPi05TeV[3][14]     = { { 1, 1, 1, 1, 1,      1, 1, 1, 1, 1,      1, 1, 1, 1 },
                                                { 1, 1, 1, 1, 1,      1, 1, 1, 1, 1,      1, 1, 1, 1 },
                                                { 1, 1, 1, 1, 1,      1, 1, 1, 1, 1,      1, 1, 1, 1 } };
    Bool_t bsmoothCentPi05TeV[3][14]        = { { 0, 1, 1, 1, 1,      1, 1, 1, 1, 1,      1, 1, 1, 1 },
                                                { 0, 1, 1, 1, 1,      1, 1, 1, 1, 1,      1, 1, 1, 1 },
                                                { 0, 1, 1, 1, 1,      1, 1, 1, 1, 1,      1, 1, 1, 1 } };
    Bool_t bsmoothCentEta5TeV[3][14]        = { { 0, 1, 1, 1, 1,      1, 1, 1, 1, 1,      1, 1, 1, 1 },
                                                { 0, 1, 1, 1, 1,      1, 1, 1, 1, 1,      1, 1, 1, 1 },
                                                { 0, 1, 1, 1, 1,      1, 1, 1, 1, 1,      1, 1, 1, 1 } };
    Bool_t bsmoothCentEtaToPi05TeV[3][14]   = { { 0, 1, 1, 1, 1,      1, 1, 1, 1, 1,      1, 1, 1, 0 },
                                                { 0, 1, 1, 1, 1,      1, 1, 1, 1, 1,      1, 1, 1, 0 },
                                                { 0, 1, 1, 1, 1,      1, 1, 1, 1, 1,      1, 1, 1, 0 } };
    Bool_t notContainedInFile[3][14]        = { { 0, 0, 0, 0, 0,      0, 0, 1, 0, 1,      1, 1, 0, 0 },
                                                { 0, 0, 0, 0, 0,      0, 0, 1, 0, 1,      1, 1, 0, 0 },
                                                { 0, 1, 0, 0, 0,      0, 0, 1, 1, 1,      1, 1, 1, 0 } };
    Bool_t nonZero[14]                      = { 0, 0, 0, 0, 0,      0, 0, 0, 0, 0,      0, 0, 0, 0};

    Int_t trigger   = 0;
    TString simpleTrigger = "";
    if ( additionalName.Contains("EG2") ){
        trigger   = 1;
        simpleTrigger = "EG2_";
    } else if ( additionalName.Contains("EG1") ){
        trigger   = 2;
        simpleTrigger = "EG1_";
    }
    cout << "choosing trigger: " << trigger << endl;
                                                
    for (Int_t i = 0; i < numberCutStudies; i++){
        if (energy.CompareTo("pPb_5.023TeV") == 0 || energy.CompareTo("pPb_5.023TeVCent") == 0 ){
            if ( (additionalNameOutput.CompareTo("") == 0 || additionalNameOutput.CompareTo("00100") == 0) && meson.CompareTo("Pi0")==0){
                bsmooth[i]                      = bsmoothMBPi05TeV[trigger][i];
            } else if ((additionalNameOutput.CompareTo("") == 0 || additionalNameOutput.CompareTo("00100") == 0) && meson.CompareTo("Eta")==0){
                bsmooth[i]                      = bsmoothMBEta5TeV[trigger][i];
            } else if ((additionalNameOutput.CompareTo("") == 0 || additionalNameOutput.CompareTo("00100") == 0) && meson.CompareTo("EtaToPi0")==0){
                bsmooth[i]                      = bsmoothMBEtaToPi05TeV[trigger][i];
            } else if ( (additionalNameOutput.CompareTo("") == 0 || additionalNameOutput.CompareTo("00100") == 0) && meson.CompareTo("Pi0Ratio")==0){
                bsmooth[i]                      = bsmoothMBPi0Ratio5TeV[trigger][i];
            } else if ((additionalNameOutput.CompareTo("") == 0 || additionalNameOutput.CompareTo("00100") == 0) && meson.CompareTo("EtaRatio")==0){
                bsmooth[i]                      = bsmoothMBEtaRatio5TeV[trigger][i];
            // for multiplicity bins
            } else if (additionalNameOutput.CompareTo("") && meson.CompareTo("Pi0")==0){
                bsmooth[i]                      = bsmoothCentPi05TeV[trigger][i];
            } else if (additionalNameOutput.CompareTo("") && meson.CompareTo("Eta")==0){
                bsmooth[i]                      = bsmoothCentEta5TeV[trigger][i];
            } else if (additionalNameOutput.CompareTo("") && meson.CompareTo("EtaToPi0")==0){
                bsmooth[i]                      = bsmoothCentEtaToPi05TeV[trigger][i];
            }
        }
    }


    // ***************************************************************************************************
    // ****************************** Initialize error vectors & graphs **********************************
    // ***************************************************************************************************

    Double_t* errorsNeg                     [nCuts];
    Double_t errorsNegCorr                  [nCuts][nPtBins];
    Double_t errorsNegSummed                [nPtBins];
    Double_t errorsNegCorrSummed            [nPtBins];
    Double_t errorsNegCorrMatSummed         [nPtBins];

    Double_t* errorsNegErr                  [nCuts];
    Double_t errorsNegErrCorr               [nCuts][nPtBins];
    Double_t errorsNegErrSummed             [nPtBins];
    Double_t errorsNegErrCorrSummed         [nPtBins];

    Double_t* errorsPos                     [nCuts];
    Double_t errorsPosCorr                  [nCuts][nPtBins];
    Double_t errorsPosSummed                [nPtBins];
    Double_t errorsPosCorrSummed            [nPtBins];
    Double_t errorsPosCorrMatSummed         [nPtBins];

    Double_t* errorsPosErr                  [nCuts];
    Double_t errorsPosErrSummed             [nPtBins];
    Double_t errorsPosErrCorr               [nCuts][nPtBins];
    Double_t errorsPosErrCorrSummed         [nPtBins];

    Double_t errorsMean                     [nCuts][nPtBins];
    Double_t errorsMeanCorr                 [nCuts][nPtBins];
    Double_t errorsMeanSummed               [nPtBins];
    Double_t errorsMeanCorrSummed           [nPtBins];
    Double_t errorsMeanCorrMatSummed        [nPtBins];

    Double_t errorsMeanErr                  [nCuts][nPtBins];
    Double_t errorsMeanErrCorr              [nCuts][nPtBins];
    Double_t errorsMeanErrSummed            [nPtBins];
    Double_t errorsMeanErrCorrSummed        [nPtBins];
    Double_t errorsMeanErrCorrMatSummed     [nPtBins];

    TGraphErrors* negativeErrors            [nCuts];
    TGraphErrors* positiveErrors            [nCuts];
    TGraphErrors* negativeErrorsCorr        [nCuts];
    TGraphErrors* positiveErrorsCorr        [nCuts];
    TGraphErrors* meanErrors                [nCuts];
    TGraphErrors* meanErrorsCorr            [nCuts];

    TGraphErrors* negativeErrorsSummed;
    TGraphErrors* positiveErrorsSummed;
    TGraphErrors* negativeErrorsCorrSummed;
    TGraphErrors* positiveErrorsCorrSummed;
    TGraphErrors* meanErrorsSummed;
    TGraphErrors* meanErrorsCorrSummed;
    TGraphErrors* meanErrorsCorrSummedIncMat;

    for (Int_t l = 0;l < nPtBins;l++){
        errorsPosSummed[l]              = 0.;
        errorsNegSummed[l]              = 0.;
        errorsMeanSummed[l]             = 0.;
        errorsPosCorrSummed[l]          = 0.;
        errorsNegCorrSummed[l]          = 0.;
        errorsMeanCorrSummed[l]         = 0.;
    }

    // ***************************************************************************************************
    // ****************************** Read & process data from file **************************************
    // ***************************************************************************************************
    TFile* fileErrorInput= new TFile(nameDataFileErrors);

    for (Int_t i = 0;i < nCuts;i++){
        Double_t minPt      = -10;
        Double_t errorReset = -10000;

        // read data
        TGraphAsymmErrors* graphPosErrors;
        TGraphAsymmErrors* graphNegErrors;
        TString nameGraphPos    = "";
        TString nameGraphNeg    = "";
    
        if (i == 0 || notContainedInFile[trigger][i] ||  (i!=13 && useMBSyst) ){
            if ( meson.CompareTo("EtaToPi0") != 0 ){
                nameGraphPos        = Form("%s_SystErrorRelPos_YieldExtraction_%s",meson.Data(),additionalName.Data() );
                nameGraphNeg        = Form("%s_SystErrorRelNeg_YieldExtraction_%s",meson.Data(),additionalName.Data() );
            } else {
                nameGraphPos        = Form("Eta_SystErrorRelPos_YieldExtraction_%s",additionalName.Data() );
                nameGraphNeg        = Form("Eta_SystErrorRelNeg_YieldExtraction_%s",additionalName.Data() );
            }
        } else if ( i == 13 && !meson.Contains("Ratio")) { // special treatment for eta to pi0 ratio
            nameGraphPos    = Form("Pi0EtaBinning_SystErrorRelPos_YieldExtraction_%s",additionalName.Data() );
            nameGraphNeg    = Form("Pi0EtaBinning_SystErrorRelNeg_YieldExtraction_%s",additionalName.Data() );
        } else if ( i == 13 && meson.Contains("Ratio")) { // special treatment for RpA
            nameGraphPos    = Form("%s_SystErrorRelPos_YieldExtraction_%s%s",meson.Data(),simpleTrigger.Data(), "pp" );
            nameGraphNeg    = Form("%s_SystErrorRelNeg_YieldExtraction_%s%s",meson.Data(),simpleTrigger.Data(), "pp"  );
        } else {// read graphs from input file
            nameGraphPos    = Form("%s_SystErrorRelPos_%s%s",meson.Data(),nameCutVariationSC[i].Data(),additionalName.Data()  );
            nameGraphNeg    = Form("%s_SystErrorRelNeg_%s%s",meson.Data(),nameCutVariationSC[i].Data(),additionalName.Data()  );
        }
        cout << "Cutstudies " << i<< "\t" <<nameGraphPos.Data() << "\t" << nameGraphNeg.Data()<<  endl;
        graphPosErrors          = (TGraphAsymmErrors*)fileErrorInput->Get(nameGraphPos.Data());
        graphNegErrors          = (TGraphAsymmErrors*)fileErrorInput->Get(nameGraphNeg.Data());
        cout << " graphPosErrors " << graphPosErrors << endl;
        cout << " graphNegErrors " << graphNegErrors << endl;

        // take out offsets
        while (graphPosErrors->GetX()[0] < startPtSys){
            graphPosErrors->RemovePoint(0);
            graphNegErrors->RemovePoint(0);
        }

        // Filling arrays
        if (i == 0) {
            ptBins      = graphNegErrors->GetX();
            ptBinsErr   = graphNegErrors->GetEXhigh();
        }
        errorsNeg[i]    = graphNegErrors->GetY();
        errorsNegErr[i] = graphNegErrors->GetEYhigh();
        errorsPos[i]    = graphPosErrors->GetY();
        errorsPosErr[i] = graphPosErrors->GetEYhigh();

        cout << nameCutVariationSC[i].Data() << endl;
        // Averaging of upper and lower errors
        CalculateMeanSysErr(errorsMean[i], errorsMeanErr[i], errorsPos[i], errorsNeg[i], nPtBins);
        // Automatic smoothing of 0 bins according to adjoining bins
        CorrectSystematicErrorsWithMean(errorsPos[i],errorsPosErr[i], errorsPosCorr[i], errorsPosErrCorr[i], nPtBins);
        CorrectSystematicErrorsWithMean(errorsNeg[i],errorsNegErr[i], errorsNegCorr[i], errorsNegErrCorr[i], nPtBins);
        CorrectSystematicErrorsWithMean(errorsMean[i], errorsMeanErr[i], errorsMeanCorr[i], errorsMeanErrCorr[i], nPtBins);

        // Routing for manual smoothing of systematic errors
        // ATTTENTION! you have to do this manually for each data set/trigger never trust the values mentioned here
        if (bsmooth[i]){
            // manual smoothing for Yield extraction errors - variation 0
            if  (nameCutVariationSC[i].CompareTo("YieldExtraction") == 0){
                cout << "Yield extraction smoothing" << endl;
                for (Int_t k = 0;k < nPtBins;k++){
                    if (meson.CompareTo("Pi0") == 0 || meson.CompareTo("Pi0Ratio") == 0 ){
                        if (additionalName.Contains("EG2") )
                            errorReset = -0.4+0.03*ptBins[k]+0.033*pow(ptBins[k],2);
                        else if ( additionalName.Contains("EG1"))
                            errorReset = -3.1+0.012*ptBins[k]+0.042*pow(ptBins[k],2);
                        else 
                            errorReset = 35.5*pow(0.25,ptBins[k])+0.3+0.005*ptBins[k]+0.01*pow(ptBins[k],2);
                        
                    } else if (meson.CompareTo("Eta") == 0 || (meson.CompareTo("EtaToPi0") == 0 || meson.CompareTo("EtaRatio") == 0 ) ){
                        if (additionalName.Contains("EG2") || additionalName.Contains("EG1"))
                            errorReset = 3+0.02*ptBins[k];
                        else 
                            errorReset = 120.5*pow(0.3,ptBins[k])+3;
                    }
                    errorsMeanCorr[i][k]            = errorReset;
                }
            }
            if  (nameCutVariationSC[i].CompareTo("YieldExtractionPi0") == 0){
                cout << "Yield extraction smoothing" << endl;
                for (Int_t k = 0;k < nPtBins;k++){
                    if (additionalName.Contains("EG2") )
                        errorReset = -0.4+0.03*ptBins[k]+0.033*pow(ptBins[k],2);
                    else if ( additionalName.Contains("EG1"))
                        errorReset = -3.1+0.012*ptBins[k]+0.042*pow(ptBins[k],2);
                    else 
                        errorReset = 35.5*pow(0.25,ptBins[k])+0.3+0.005*ptBins[k]+0.01*pow(ptBins[k],2);
                    errorsMeanCorr[i][k]            = errorReset;
                }
                
            }
            // manual smoothing for Yield extraction errors - variation 1
            if  (nameCutVariationSC[i].CompareTo("OpeningAngle") == 0){
                cout << "Opening Angle smoothing" << endl;
                for (Int_t k = 0;k < nPtBins;k++){
                    if (meson.CompareTo("Pi0") == 0 || meson.CompareTo("Pi0Ratio") == 0){
                        if (additionalName.Contains("EG2") || additionalName.Contains("EG1"))
                            errorReset              = 0.1+0.02*ptBins[k]+0.005*pow(ptBins[k],2);
                        else 
                            errorReset              = 0.1-0.02*ptBins[k]+0.003*pow(ptBins[k],2);
                    } else if (meson.CompareTo("Eta") == 0 || meson.CompareTo("EtaRatio") == 0){
                        if (additionalName.Contains("EG2") || additionalName.Contains("EG1"))
                            errorReset          = 1.2;
                        else
                            errorReset          = 0.7;
                    } else if (meson.CompareTo("EtaToPi0") == 0){
                        if (additionalName.Contains("EG2") || additionalName.Contains("EG1"))
                            errorReset          = 1.8;
                        else
                            errorReset          = 0.7;
                    }
                    errorsMeanCorr[i][k]            = errorReset;
                }
            }

            // manual smoothing for minimum cluster energy errors - variation 2
            if (nameCutVariationSC[i].CompareTo("ClusterMinEnergy")==0  ){
                cout << "Cluster minimum energy smoothing" << endl;
//                 Double_t error = 0.5;
//                 if (meson.CompareTo("Eta") == 0 )       error = error*3.;
//                 if (meson.CompareTo("EtaToPi0") == 0 )  error = 2.;
                for (Int_t k = 0;k < nPtBins;k++){
                    if ( meson.CompareTo("Pi0Ratio") == 0)
                        errorReset              = 0.2;
                    else if ( meson.CompareTo("EtaRatio") == 0)
                        errorReset              = 0.5;
                    else 
                        errorReset              = 140.5*pow(0.1,ptBins[k])+0.5;
                    errorsMeanCorr[i][k]        = errorReset;
                }
            }
            // manual smoothing for minimum number of cells in cluster errors - variation 3
            if (nameCutVariationSC[i].CompareTo("ClusterNCells")==0 ){
                cout << "Cluster NCells smoothing" << endl;
                minPt               = 1.2; 
                errorReset          = 1.5;  // EG2 seems to be 3%
                if (meson.CompareTo("EtaToPi0") == 0)
                    errorReset          = TMath::Sqrt(2.)*1.5;;   
            }
            
            // manual smoothing for energy calibration errors - variation 4
            if (nameCutVariationSC[i].CompareTo("ClusterNonLinearity")==0 ){//&& meson.Contains("Pi0")
                cout << "Cluster non linearity smoothing" << endl;
                for (Int_t k = 0;k < nPtBins;k++){
                    if (meson.CompareTo("Pi0") == 0 || meson.CompareTo("Pi0Ratio") == 0)
                        errorReset              = 0.0104652+0.086*ptBins[k]+0.0032*pow(ptBins[k],2);
                    else if (meson.CompareTo("Eta") == 0 || meson.CompareTo("EtaRatio") == 0)
                        errorReset                   = 0.0104652+0.05*ptBins[k];
                    else if (meson.CompareTo("EtaToPi0") == 0 )
                        errorReset                   = 0.0104652+0.05*ptBins[k];
                    errorsMeanCorr[i][k]            = errorReset;
                }
            }
            // manual smoothing for cluster matching errors - variation 5
            if (nameCutVariationSC[i].CompareTo("ClusterTrackMatchingCalo")==0 ){
                cout << "Cluster track matching smoothing" << endl;
                    for (Int_t k = 0;k < nPtBins;k++){
                        if ( meson.CompareTo("Pi0") == 0 || meson.CompareTo("Pi0Ratio") == 0){
                            if (additionalName.Contains("EG1"))
                                errorReset          = 0.8+0.003*pow(ptBins[k],2);
                            else 
                                errorReset          = 0.2+0.003*pow(ptBins[k],2);
                        } else if (meson.CompareTo("Eta") == 0 || meson.CompareTo("EtaRatio") == 0){
                            errorReset          = 0.6+0.003*pow(ptBins[k],2);
//                             errorReset          = errorReset*1.5;
                        } else if( meson.CompareTo("EtaToPi0") == 0 ){
//                             errorReset               = errorReset*1.5;
                            errorReset          = 0.6+0.003*pow(ptBins[k],2);
                        }
                        errorsMeanCorr[i][k]        = errorReset;
                    }
            }
           // manual smoothing for cluster shape errors - variation 6
            if (nameCutVariationSC[i].CompareTo("ClusterM02")==0 ){//&& meson.Contains("Pi0")
                cout << "Cluster M02 smoothing" << endl;
                for (Int_t k = 0;k < nPtBins;k++){
                    if ( meson.CompareTo("Pi0") == 0 ){
                        if (additionalName.Contains("EG2"))
                            errorReset = 1+0.012*ptBins[k]+0.007*pow(ptBins[k],2); 
                        else if (additionalName.Contains("EG1"))
                            errorReset = 3;  //                 
                        else 
                            errorReset = 1.0+0.012*ptBins[k]+0.007*pow(ptBins[k],2);
                    } else if ( meson.CompareTo("Pi0Ratio") == 0){
                        if (additionalName.Contains("EG2"))
                            errorReset = 0.8+0.012*ptBins[k]+0.007*pow(ptBins[k],2);  
                        else if (additionalName.Contains("EG1"))
                            errorReset = 3;  //                 
                        else 
                            errorReset = 0.4+0.012*ptBins[k]+0.011*pow(ptBins[k],2);
                    } else if (meson.CompareTo("Eta") == 0 ) {
                        if (additionalName.Contains("EG2"))
                            errorReset = 2+0.005*ptBins[k]+0.007*pow(ptBins[k],2);
                        else if (additionalName.Contains("EG1"))
                            errorReset = 2.4;
                        else 
                            errorReset = 300.5*pow(0.1,ptBins[k])+1.5+0.012*ptBins[k]+0.011*pow(ptBins[k],2);
                    } else if ( meson.CompareTo("EtaRatio") == 0) {
                        if (additionalName.Contains("EG2"))
                            errorReset = 2+0.005*ptBins[k]+0.007*pow(ptBins[k],2);
                        else if (additionalName.Contains("EG1"))
                            errorReset = 2.4;
                        else 
                            errorReset = 500.5*pow(0.1,ptBins[k])+1.9+0.012*ptBins[k]+0.011*pow(ptBins[k],2);
                    } else if( meson.CompareTo("EtaToPi0") == 0 ){
                        if (additionalName.Contains("EG2"))
                            errorReset = 2.5+0.005*ptBins[k]+0.007*pow(ptBins[k],2);
                        else if (additionalName.Contains("EG1"))
                            errorReset = 2.5;
                        else 
                            errorReset = 400.5*pow(0.1,ptBins[k])+1.5+0.012*ptBins[k]+0.011*pow(ptBins[k],2);
                    }
                  errorsMeanCorr[i][k]            = errorReset;
                }
            }
            // manual smoothing for Material infront of EMC - variation 7
            if (nameCutVariationSC[i].CompareTo("ClusterMaterialTRD")==0 ){
                cout << "Material smoothing" << endl;
                minPt               = 1.2; 
                errorReset          = 4.24;
                if (meson.CompareTo("EtaToPi0") == 0 || meson.Contains("Ratio"))
                    errorReset          = 0;    // cancels fully for eta/pi0 & RpA
            }

            // manual smoothing for energy scale errors (derived from mass difference MC & Data) - variation 8
            if (nameCutVariationSC[i].CompareTo("ClusterEnergyScale")==0 ){//&& meson.Contains("Pi0")
                cout << "Cluster non linearity smoothing" << endl;
                minPt               = 1.2; 
                errorReset          = 0.2*7.2;
                if (meson.CompareTo("Eta") == 0)
                    errorReset          = 2* errorReset;
                else if (meson.CompareTo("EtaToPi0") == 0)
                    errorReset          = 2* errorReset;
                else if (meson.Contains("Ratio"))
                    errorReset          = 0;
            }


            // manual smoothing for Trigger normalization uncertainties - variation 9
            if (nameCutVariationSC[i].CompareTo("Trigger") == 0){
                cout << "Trigger smoothing" << endl;
                minPt               = 1.2;
                
                if (meson.CompareTo("EtaToPi0") == 0)
                    errorReset          = 0.;
                else if (meson.Contains("Ratio")){
                    if (additionalName.Contains("EG1"))
                        errorReset          = TMath::Sqrt(6.09*6.09+4.2*4.2);
                    else if (additionalName.Contains("EG2"))
                        errorReset          = TMath::Sqrt(6.09*6.09+4.2*4.2);
                    else 
                        errorReset          = 0.;                    
                } else {
                    if (additionalName.Contains("EG1"))
                        errorReset          = 6.09;
                    else if (additionalName.Contains("EG2"))
                        errorReset          = 4.18;
                    else 
                        errorReset          = 0.;
                }
            }
            // manual smoothing for Efficiency uncertainties - variation 10
            if (nameCutVariationSC[i].CompareTo("Efficiency") == 0){
                cout << "Efficiency smoothing" << endl;
                minPt               = 1.2; 
                errorReset          = 2.0;
                if (meson.CompareTo("Eta") == 0 )
                    errorReset          = 4;
                else if ( meson.CompareTo("EtaToPi0") == 0)
                    errorReset          = TMath::Sqrt(2*2+4*4);
            }

            // manual smoothing for Rapidity
            if (nameCutVariationSC[i].CompareTo("Rapidity")==0 ){
                cout << "Rapidity" << endl;
                minPt               = 1.2; 
                errorReset          = 0.75;
                if (meson.CompareTo("Eta") == 0 || meson.CompareTo("EtaRatio") == 0 )
                    errorReset          = 1;
                else if (meson.CompareTo("EtaToPi0") == 0)
                    errorReset          = TMath::Sqrt(1*1+0.75*0.75);
            }

            // manual smoothing for Rapidity
            if (nameCutVariationSC[i].CompareTo("Alpha")==0 ){
                cout << "Alpha" << endl;
                minPt               = 1.2; 
                if (meson.CompareTo("Eta") == 0 || meson.CompareTo("EtaToPi0") == 0 || meson.CompareTo("EtaRatio") == 0 ){
                    if (additionalName.Contains("EG2") || additionalName.Contains("EG1") )
                        errorReset          = 2;
                    else 
                        errorReset          = 1;
                } else {
                    if (additionalName.Contains("EG1") || additionalName.Contains("EG2"))
                        errorReset          = 0.7;
                    else 
                        errorReset          = 0.2;
                }
            }

            if  (nameCutVariationSC[i].CompareTo("YieldExtraction_pp") == 0){
                cout << "Yield extraction smoothing" << endl;
                for (Int_t k = 0;k < nPtBins;k++){
                    if (meson.CompareTo("Pi0Ratio") == 0){
                        if (additionalName.Contains("EG2") )
                            errorReset = 20.5*pow(0.72,ptBins[k])+0.004*pow(ptBins[k],2)+0.00008*pow(ptBins[k],4);
                        else if ( additionalName.Contains("EG1"))
                            errorReset = 35.5*pow(0.8,ptBins[k])-7.8+0.048*pow(ptBins[k],2);
                        else 
                            errorReset = 35.5*pow(0.25,ptBins[k])+0.3+0.005*ptBins[k]+0.01*pow(ptBins[k],2);
                    } else if (meson.CompareTo("EtaRatio") == 0 ){
                        if (additionalName.Contains("EG2") )
                            errorReset = 3.2;// 0.02*ptBins[k]+0.002*pow(ptBins[k],2);
                        else if ( additionalName.Contains("EG1"))
                            errorReset = 4.5+0.02*ptBins[k]+0.0017*pow(ptBins[k],2);
                        else 
                            errorReset = 150.5*pow(0.3,ptBins[k])+4;
                    }
                    errorsMeanCorr[i][k]            = errorReset;
                }
            }

            // reset to a constant if enabled
            if (minPt != -10 && errorReset != -10000 ){
                for (Int_t k = 0; k < nPtBins; k++){
                    if (ptBins[k] > minPt){
                        errorsMeanErr[i][k]         = errorReset*0.01;
                        errorsMeanCorr[i][k]        = errorReset;
                        errorsMeanErrCorr[i][k]     = errorReset*0.01;
                    }
                }
            } else {
                for (Int_t k = 0; k < nPtBins; k++){
                    errorsMeanErr[i][k]         = errorsMeanErr[i][k]*0.01;
                    errorsMeanErrCorr[i][k]     = errorsMeanCorr[i][k]*0.01;
                }
            }
        } else {
            for (Int_t k = 0;k < nPtBins;k++){
                errorsMeanErr[i][k]         = 0.03;
                errorsMeanErrCorr[i][k]     = 0.03;
            }
        }
        // Quadratic sum of errors except material error infront of EMCal & inner material
        cout << "errors added quadratically" << endl;
        for (Int_t l = 0;l < nPtBins;l++){
            errorsPosSummed[l]      = errorsPosSummed[l]+pow(errorsPos[i][l],2);
            errorsNegSummed[l]      = errorsNegSummed[l]+ pow(errorsNeg[i][l],2);
            errorsMeanSummed[l]     = errorsMeanSummed[l]+ pow(errorsMean[i][l],2);
            errorsPosCorrSummed[l]  = errorsPosCorrSummed[l]+pow(errorsPosCorr[i][l],2);
            errorsNegCorrSummed[l]  = errorsNegCorrSummed[l] +pow(errorsNegCorr[i][l],2);
            errorsMeanCorrSummed[l] = errorsMeanCorrSummed[l]+ pow(errorsMeanCorr[i][l],2);
        }
        for (Int_t l = 0; l < nPtBins; l++){
            if ( errorsMeanCorr[i][l] > 0 ) nonZero[i] = 1;
        }

        // fill error graphs for plotting
        negativeErrors[i]       = new TGraphErrors(nPtBins,ptBins ,errorsNeg[i] ,ptBinsErr ,errorsNegErr[i] );
        meanErrors[i]           = new TGraphErrors(nPtBins,ptBins ,errorsMean[i] ,ptBinsErr ,errorsMeanErr[i] );
        positiveErrors[i]       = new TGraphErrors(nPtBins,ptBins ,errorsPos[i] ,ptBinsErr ,errorsPosErr[i] );
        negativeErrorsCorr[i]   = new TGraphErrors(nPtBins,ptBins ,errorsNegCorr[i] ,ptBinsErr ,errorsNegErrCorr[i] );
        meanErrorsCorr[i]       = new TGraphErrors(nPtBins,ptBins ,errorsMeanCorr[i] ,ptBinsErr ,errorsMeanErrCorr[i] );
        positiveErrorsCorr[i]   = new TGraphErrors(nPtBins,ptBins ,errorsPosCorr[i] ,ptBinsErr ,errorsPosErrCorr[i] );

    }

    Int_t enabledCuts = 0;
    for (Int_t i = 0; i < numberCutStudies; i++)
        if (nonZero[i]) enabledCuts++;
    
    // Error for inner material budget
    Double_t errorMaterial = 0;

    // Calculate sqrt of summed errors for final errors, add material budget errors
    for (Int_t l = 0;l < nPtBins;l++){
        errorsPosSummed[l]              = pow(errorsPosSummed[l],0.5);
        errorsMeanSummed[l]             = pow(errorsMeanSummed[l],0.5);
        errorsPosErrSummed[l]           = errorsPosSummed[l]*0.001;
        errorsMeanErrSummed[l]          = errorsMeanSummed[l]*0.001;
        errorsNegSummed[l]              = -pow(errorsNegSummed[l],0.5);
        errorsNegErrSummed[l]           = errorsNegSummed[l]*0.001;

        // add EMCal material errors
        errorsPosCorrMatSummed[l]       = pow(errorsPosCorrSummed[l]+ pow(errorMaterial ,2.),0.5);
        errorsMeanCorrMatSummed[l]      = pow(errorsMeanCorrSummed[l]+ pow(errorMaterial ,2.),0.5);
        cout << " quad sum " << errorsMeanCorrMatSummed[l] << endl;
        errorsNegCorrMatSummed[l]       = -pow(errorsNegCorrSummed[l]+ pow(errorMaterial ,2.),0.5);

        errorsPosCorrSummed[l]          = pow(errorsPosCorrSummed[l],0.5);
        errorsMeanCorrSummed[l]         = pow(errorsMeanCorrSummed[l],0.5);
        errorsPosErrCorrSummed[l]       = errorsPosCorrSummed[l]*0.001;
        errorsMeanErrCorrSummed[l]      = errorsMeanCorrSummed[l]*0.001;
        errorsMeanErrCorrMatSummed[l]   = errorsMeanCorrMatSummed[l]*0.001;
        errorsNegCorrSummed[l]          = -pow(errorsNegCorrSummed[l],0.5);
        errorsNegErrCorrSummed[l]       = errorsNegCorrSummed[l]*0.001;
    }

    // Create all other summed graphs
    cout << __LINE__ << endl;
    negativeErrorsSummed        = new TGraphErrors(nPtBins,ptBins ,errorsNegSummed ,ptBinsErr ,errorsNegErrSummed );
    negativeErrorsCorrSummed    = new TGraphErrors(nPtBins,ptBins ,errorsNegCorrSummed ,ptBinsErr ,errorsNegErrCorrSummed );
    positiveErrorsSummed        = new TGraphErrors(nPtBins,ptBins ,errorsPosSummed ,ptBinsErr ,errorsPosErrSummed );
    positiveErrorsCorrSummed    = new TGraphErrors(nPtBins,ptBins ,errorsPosCorrSummed ,ptBinsErr ,errorsPosErrCorrSummed );
    meanErrorsSummed            = new TGraphErrors(nPtBins,ptBins ,errorsMeanSummed ,ptBinsErr ,errorsMeanErrSummed );
    meanErrorsCorrSummed        = new TGraphErrors(nPtBins,ptBins ,errorsMeanCorrSummed ,ptBinsErr ,errorsMeanErrCorrSummed );
    meanErrorsCorrSummedIncMat  = new TGraphErrors(nPtBins,ptBins ,errorsMeanCorrMatSummed ,ptBinsErr ,errorsMeanErrCorrMatSummed );

    cout << __LINE__ << endl;

    // Give legend position for plotting
    Double_t minXLegend     = 0.12;
    Double_t maxYLegend     = 0.95;
    if (meson.CompareTo("Eta") == 0){
        minXLegend          = 0.23;
    }
    Double_t widthLegend    = 0.25;
    if (enabledCuts> 7)
        widthLegend         = 0.5;
    Double_t heightLegend   = 1.15 * 0.035 * (enabledCuts+3);
    if (enabledCuts> 7)
        heightLegend        = 1.15 * 0.035 * (enabledCuts/2+2);

    // ***************************************************************************************************
    // ********************* Plot all mean erros separately after smoothing ******************************
    // ***************************************************************************************************
    TCanvas* canvasNewSysErrMean = new TCanvas("canvasNewSysErrMean","",200,10,1350,900);// gives the page size
    DrawGammaCanvasSettings( canvasNewSysErrMean, 0.08, 0.01, 0.015, 0.09);

        // create dummy histo
        TH2D *histo2DNewSysErrMean ;
        if ( meson.CompareTo("Pi0") == 0 ){
            histo2DNewSysErrMean = new TH2D("histo2DNewSysErrMean", "", 20,0.,ptBins[nPtBins-1]+ptBinsErr[nPtBins-1],1000.,-0.5,24.5);
        } else {
            histo2DNewSysErrMean = new TH2D("histo2DNewSysErrMean", "", 20,0.,ptBins[nPtBins-1]+ptBinsErr[nPtBins-1],1000.,-0.5,34.5);
        }
        SetStyleHistoTH2ForGraphs( histo2DNewSysErrMean, "#it{p}_{T} (GeV/#it{c})", "mean smoothed systematic Err %", 0.03, 0.04, 0.03, 0.04,
                                1,0.9, 510, 510);
        histo2DNewSysErrMean->Draw();

        // create legend
        TLegend* legendMeanNew = GetAndSetLegend2(minXLegend,maxYLegend-heightLegend,minXLegend+widthLegend,maxYLegend, 30);
        legendMeanNew->SetMargin(0.1);
        if (enabledCuts> 7) legendMeanNew->SetNColumns(2);

        for(Int_t i = 0;i< numberCutStudies ;i++){
            cout << i << "\t"<< additionalNameOutput.Data() << endl;

            if ( !nonZero[i] ){
                cout << "not drawing: " << nameCutVariation[i].Data() << endl;
                continue;
            }

            DrawGammaSetMarkerTGraphErr(meanErrorsCorr[i], markerStyle[i], 1.,color[i],color[i]);
            meanErrorsCorr[i]->Draw("pX0,csame");
            legendMeanNew->AddEntry(meanErrorsCorr[i],nameCutVariation[i].Data(),"p");
        }

        DrawGammaSetMarkerTGraphErr(meanErrorsCorrSummedIncMat, 20, 1.,kBlack,kBlack);
        meanErrorsCorrSummedIncMat->Draw("p,csame");
        legendMeanNew->AddEntry(meanErrorsCorrSummedIncMat,"quad. sum.","p");
        legendMeanNew->Draw();

        // plot labeling
        TLatex *labelMeson  = NULL;
        if (meson.CompareTo("EtaToPi0") == 0){
            labelMeson= new TLatex(0.94,0.89,Form("#eta/#pi^{0} %s",detectionSystem.Data()));
        } else if (meson.CompareTo("Pi0")==0){
            labelMeson= new TLatex(0.94,0.89,Form("#pi^{0} %s",detectionSystem.Data()));
        } else if (meson.CompareTo("Pi0Ratio")==0){
            labelMeson= new TLatex(0.94,0.89,Form("R_{pA} #pi^{0} %s",detectionSystem.Data()));
        } else if (meson.CompareTo("EtaRatio")==0){
            labelMeson= new TLatex(0.94,0.89,Form("R_{pA} #eta %s",detectionSystem.Data()));
        } else {
            labelMeson= new TLatex(0.94,0.89,Form("#eta  %s",detectionSystem.Data()));
        }
        SetStyleTLatex( labelMeson, 0.038, 4, 1, 42, kTRUE, 31);
        labelMeson->Draw();

        TLatex *labelCentrality = new TLatex(0.94,0.93,Form("%s %s",additionalName.Data(),collisionSystem.Data() ));
        SetStyleTLatex( labelCentrality, 0.038, 4, 1, 42, kTRUE, 31);
        labelCentrality->Draw();

        TLatex *labelTrig   = NULL;
        if (additionalNameOutput.CompareTo("")==0){
            labelTrig= new TLatex(0.94,0.84,Form("MB LHC13b-f+LHC16q+t"));
        }
        SetStyleTLatex( labelTrig, 0.038, 4, 1, 42, kTRUE, 31);
        labelTrig->Draw();

    canvasNewSysErrMean->Update();
    canvasNewSysErrMean->SaveAs(Form("SystematicErrorsCalculatedCalo/%s/SysMeanNewWithMean_%s_%s%s_%s.%s",additionalName2.Data(),meson.Data(), energyForOutput.Data(), additionalNameOutput.Data(), dateForOutput.Data(), suffix.Data()));

    // ***************************************************************************************************
    // ********************* Plot unsmoothed errors with fits ********************************************
    // ***************************************************************************************************
    for (Int_t cut =0 ;cut < numberCutStudies;cut++ ){
        if (!nonZero[cut]) continue;
        
        canvasNewSysErrMean->cd();
        if (meson.CompareTo("Pi0Ratio") == 0)
            histo2DNewSysErrMean->GetYaxis()->SetRangeUser(-0.5,15);
        histo2DNewSysErrMean->Draw();
            
            cout <<endl << endl<<  "variation: " << cut << " \t"<< nameCutVariation[cut].Data() << endl;
            Double_t minPt = startPtSys;
            Double_t maxPt = ptBins[nPtBins-2]+1;

            TLatex *labelVar = new TLatex(0.96,0.93,Form("%s",nameCutVariation[cut].Data() ));
            SetStyleTLatex( labelVar, 0.038,4,1, 42, kTRUE, 31);
            labelVar->Draw();

            if (bsmooth[cut] && !notContainedInFile[trigger][cut]){
                DrawGammaSetMarkerTGraphErr(meanErrors[cut], 24, 1.,color[cut],color[cut]);
                meanErrors[cut]->Draw("p,csame");
            } else if (!notContainedInFile[trigger][cut]){
                TF1* pol0 = new TF1("pol0","[0]",minPt,maxPt); //
                TF1* pol1 = new TF1("pol1","[0]+[1]*x",minPt,maxPt); //
                TF1* pol2 = new TF1("pol2","[0]+[1]*x+[2]*x*x",minPt,maxPt); //
                TF1* pol4 = new TF1("pol4","[0]+[1]*x+[2]*x*x+[3]*x*x*x*x",minPt,maxPt); //
                pol4->SetParLimits(3,0,10);
                if (cut == 13) pol2->SetParLimits(2,0,0.1);

                meanErrorsCorr[cut]->Fit(pol4,"NRMEX0+","",minPt,maxPt);
                meanErrorsCorr[cut]->Fit(pol2,"NRMEX0+","",minPt,maxPt);
                meanErrorsCorr[cut]->Fit(pol1,"NRMEX0+","",minPt,maxPt);
                meanErrorsCorr[cut]->Fit(pol0,"NRMEX0+","",minPt,maxPt);
                pol4->SetLineColor(kRed+2);
                pol2->SetLineColor(kBlue+2);
                pol1->SetLineColor(kCyan+2);
                pol0->SetLineColor(kBlack);
                pol4->Draw("same");
                pol2->Draw("same");
                pol1->Draw("same");
                pol0->Draw("same");                
            }

            DrawGammaSetMarkerTGraphErr(meanErrorsCorr[cut], 20, 1.,color[cut],color[cut]);
            meanErrorsCorr[cut]->Draw("p,csame");

        canvasNewSysErrMean->SaveAs(Form("SystematicErrorsCalculatedCalo/%s/SysMeanNewWithMeanSingle_%s_%s%s_%s_Variation%d_%s.%s",additionalName2.Data(),meson.Data(), energyForOutput.Data(), additionalNameOutput.Data(), dateForOutput.Data(), cut, nameCutVariationSCCurrent[cut].Data(), suffix.Data()));
        histo2DNewSysErrMean->GetYaxis()->UnZoom();
    }

    // ***************************************************************************************************
    // ********************* Create output files with errors *********************************************
    // ***************************************************************************************************
    const char *SysErrDatnameMean = Form("SystematicErrorsCalculatedCalo/%s/SystematicErrorAveragedEMCEMC_%s_%s%s_%s.dat",additionalName2.Data(), meson.Data(), energyForOutput.Data(), additionalNameOutput.Data(), dateForOutput.Data());
    fstream SysErrDatAver;
    cout << SysErrDatnameMean << endl;
    SysErrDatAver.open(SysErrDatnameMean, ios::out);
    for (Int_t l=0;l< nPtBins;l++){
        SysErrDatAver << ptBins[l] << "\t" << "-"<< errorsMeanCorrMatSummed[l] << "\t" <<errorsMeanCorrMatSummed[l] << "\t"  << "-"<< errorsMeanCorrSummed[l] << "\t" <<errorsMeanCorrSummed[l]  << endl;
    }

    SysErrDatAver.close();

    const char *SysErrDatnameMeanSingleErr = Form("SystematicErrorsCalculatedCalo/%s/SystematicErrorAveragedSingleEMCEMC_%s_%s%s_%s.dat",additionalName2.Data(), meson.Data(), energyForOutput.Data(), additionalNameOutput.Data(), dateForOutput.Data());
    fstream SysErrDatAverSingle;
    cout << SysErrDatnameMeanSingleErr << endl;
    SysErrDatAverSingle.open(SysErrDatnameMeanSingleErr, ios::out);
    SysErrDatAverSingle << "Pt bin\t" ;
    for (Int_t i= 0; i< numberCutStudies; i++){
        SysErrDatAverSingle << nameCutVariationSC[i] << "\t";
    }
    SysErrDatAverSingle << endl;

    for (Int_t l=0;l< nPtBins;l++){
        SysErrDatAverSingle << ptBins[l] << "\t";
        for (Int_t i= 0; i< numberCutStudies; i++){
            SysErrDatAverSingle << errorsMeanCorr[i][l] << "\t";
        }
        SysErrDatAverSingle << "\t" << errorsMeanCorrMatSummed[l] << endl;
    }
    SysErrDatAverSingle.close();


    // ***************************************************************************************************
    // ********************* Group errors according to topic *********************************************
    // ***************************************************************************************************
    Double_t errorsMeanCorrSignalExtraction[nPtBins];
    Double_t errorsMeanCorrClusterEnergy[nPtBins];
    Double_t errorsMeanCorrClusterDescription[nPtBins];

    for (Int_t l=0;l< nPtBins;l++){
//      "YieldExtraction"-0,"OpeningAngle"-1, "ClusterMinEnergy"-2, "ClusterNCells"-3, "NonLinearity"-4, "ClusterTrackMatchingCalo" -5, "ClusterM02" -6, "ClusterMaterialTRD" -7, "ClusterEnergyScale" -8
        // grouping:
        // Signal extraction: Yield extraction 0, Open-Angle 1, Yield extraction pi0 (for eta/pi0) 13, alpha 12
        if (meson.CompareTo("EtaToPi0")){
            errorsMeanCorrSignalExtraction[l] = TMath::Sqrt(errorsMeanCorr[0][l]*errorsMeanCorr[0][l]+errorsMeanCorr[1][l]*errorsMeanCorr[1][l]+errorsMeanCorr[12][l]*errorsMeanCorr[12][l]);
        } else {
            errorsMeanCorrSignalExtraction[l] = TMath::Sqrt(errorsMeanCorr[0][l]*errorsMeanCorr[0][l]+errorsMeanCorr[1][l]*errorsMeanCorr[1][l]+errorsMeanCorr[12][l]*errorsMeanCorr[12][l]+errorsMeanCorr[13][l]*errorsMeanCorr[13][l]);
        }
        // Cluster energy: NonLinearity 5 , Energy Scale 8
        //errorsMeanCorrClusterEnergy[l]      = TMath::Sqrt(errorsMeanCorr[4][l]*errorsMeanCorr[4][l]+errorsMeanCorr[8][l]*errorsMeanCorr[8][l]);
        errorsMeanCorrClusterEnergy[l]      = TMath::Sqrt(errorsMeanCorr[4][l]*errorsMeanCorr[4][l]);//+errorsMeanCorr[8][l]*errorsMeanCorr[8][l]);
        // cluster description in MC: ClusterMinEnergy 2, ClusterNCells 3, ClusterM02 6
        errorsMeanCorrClusterDescription[l] = TMath::Sqrt(errorsMeanCorr[2][l]*errorsMeanCorr[2][l]+errorsMeanCorr[3][l]*errorsMeanCorr[3][l]+errorsMeanCorr[6][l]*errorsMeanCorr[6][l]);
    }


    TGraphErrors* meanErrorsSignalExtraction    = new TGraphErrors(nPtBins,ptBins ,errorsMeanCorrSignalExtraction ,ptBinsErr ,errorsMeanErrCorrSummed );
    TGraphErrors* meanErrorsClusterDescrip      = new TGraphErrors(nPtBins,ptBins ,errorsMeanCorrClusterDescription ,ptBinsErr ,errorsMeanErrCorrSummed );
    TGraphErrors* meanErrorsClusterEnergy       = new TGraphErrors(nPtBins,ptBins ,errorsMeanCorrClusterEnergy ,ptBinsErr ,errorsMeanErrCorrSummed );

    // ***************************************************************************************************
    // ********************* Plot grouped errors for better understanding ********************************
    // ***************************************************************************************************
    Double_t minXLegend2 = 0.13;
    Double_t maxYLegend2 = 0.95;
    if (meson.CompareTo("Eta") == 0){
        minXLegend2 = 0.20;
    }
    Double_t widthLegend2 = 0.52;
    Double_t heightLegend2 = 0.15;

    TCanvas* canvasSummedErrMean = new TCanvas("canvasSummedErrMean","",200,10,1350,900);// gives the page size
    DrawGammaCanvasSettings( canvasSummedErrMean, 0.08, 0.01, 0.015, 0.09);

        // create dummy histo
        TH2D *histo2DSummedErrMean ;
        if (meson.Contains("Pi0") ){
            histo2DSummedErrMean = new TH2D("histo2DSummedErrMean", "", 20,0.,ptBins[nPtBins-1]+ptBinsErr[nPtBins-1],1000.,-0.5,30.);
        } else {
            histo2DSummedErrMean = new TH2D("histo2DSummedErrMean", "", 20,0.,ptBins[nPtBins-1]+ptBinsErr[nPtBins-1],1000.,-0.5,30.);
        }
        SetStyleHistoTH2ForGraphs( histo2DSummedErrMean, "#it{p}_{T} (GeV/#it{c})", "mean smoothed systematic Err %", 0.03, 0.04, 0.03, 0.04,
                                1,0.9, 510, 510);
        histo2DSummedErrMean->Draw();

        // create legend
        TLegend* legendSummedMeanNew = GetAndSetLegend2(minXLegend2,maxYLegend2-heightLegend2,minXLegend2+widthLegend2,maxYLegend2, 30);
        legendSummedMeanNew->SetNColumns(2);
        legendSummedMeanNew->SetMargin(0.1);

        // 0   "YieldExtraction"
        // 1   "OpeningAngle"
        // 2   "ClusterMinEnergy"
        // 3   "ClusterNCells"
        // 4   "ClusterNonLinearity"
        // 5   "ClusterTrackMatchingCalo"
        // 6   "ClusterM02"
        // 7   "ClusterMaterialTRD"
        // 8   "Rapidity"
        // 9   "ClusterEnergyScale"
        // 10  "Efficiency"
        // 11  "Trigger"
        // 12  "YieldExtractionPi0"

        // Signal extraction error
        DrawGammaSetMarkerTGraphErr(meanErrorsSignalExtraction, 20, 1.,color[0],color[0]);
        meanErrorsSignalExtraction->Draw("pX0,csame");
        // Cluster description in MC
        DrawGammaSetMarkerTGraphErr(meanErrorsClusterDescrip, 22, 1.,color[1],color[1]);
        // Track matching to EMCAL
        DrawGammaSetMarkerTGraphErr(meanErrorsCorr[5], 25, 1.,color[5],color[5]);
        // Material infront of EMCAL
        DrawGammaSetMarkerTGraphErr(meanErrorsCorr[7], 21, 1.,color[7],color[7]);
        // Cluster energy description
        DrawGammaSetMarkerTGraphErr(meanErrorsClusterEnergy, 20, 1.,color[4],color[4]);
        meanErrorsClusterEnergy->Draw("pX0,csame");
        if(nonZero[7]) meanErrorsCorr[7]->Draw("pX0,csame");
        if(nonZero[5]) meanErrorsCorr[5]->Draw("pX0,csame");
        meanErrorsClusterDescrip->Draw("pX0,csame");
        if (numberCutStudies>10 && nonZero[10]){
            DrawGammaSetMarkerTGraphErr(meanErrorsCorr[10], 23, 1.,color[8],color[8]);
            meanErrorsCorr[10]->Draw("pX0,csame");
        }
        if (numberCutStudies>11 && nonZero[11] ){
            DrawGammaSetMarkerTGraphErr(meanErrorsCorr[11], 25, 1.,color[11],color[11]);
            meanErrorsCorr[11]->Draw("pX0,csame");
        }
        legendSummedMeanNew->AddEntry(meanErrorsSignalExtraction,"Signal Extraction","p");
        legendSummedMeanNew->AddEntry(meanErrorsClusterDescrip,"Cluster Description","p");
        legendSummedMeanNew->AddEntry(meanErrorsClusterEnergy,"Cluster Energy Description","p");
        if(nonZero[5]) legendSummedMeanNew->AddEntry(meanErrorsCorr[5],"track match. to cluster","p");
        if(nonZero[7]) legendSummedMeanNew->AddEntry(meanErrorsCorr[7],"Mat. infront of EMCal","p");
        if (numberCutStudies>10 && nonZero[10]) legendSummedMeanNew->AddEntry(meanErrorsCorr[10],"Efficiency","p");
        if (numberCutStudies>11 && nonZero[11] )
            legendSummedMeanNew->AddEntry(meanErrorsCorr[11],"Trigger normalization","p");
        DrawGammaSetMarkerTGraphErr(meanErrorsCorrSummedIncMat, 20, 1.,kBlack,kBlack);
        meanErrorsCorrSummedIncMat->Draw("p,csame");
        legendSummedMeanNew->AddEntry(meanErrorsCorrSummedIncMat,"quad. sum.","p");
        legendSummedMeanNew->Draw();

        labelMeson->Draw();
        labelCentrality->Draw();
        labelTrig->Draw();

    canvasSummedErrMean->Update();
    canvasSummedErrMean->SaveAs(Form("SystematicErrorsCalculatedCalo/%s/SysErrorSummedVisu_%s_%s%s_%s.%s",additionalName2.Data(),meson.Data(), energyForOutput.Data(), additionalNameOutput.Data(), dateForOutput.Data(), suffix.Data()));

    delete canvasSummedErrMean;

//  const char *SysErrDatnameMeanPaper = Form("SystematicErrorsCalculatedConvCalo/SystematicErrorAveragedPaper_%s_%s%s_%s.dat", meson.Data(), energyForOutput.Data(), additionalNameOutput.Data(), dateForOutput.Data());
//  fstream SysErrDatAverPaper;
//  cout << SysErrDatnameMeanPaper << endl;
//  SysErrDatAverPaper.open(SysErrDatnameMeanPaper, ios::out);
//  SysErrDatAverPaper  << "#it{p}_{T}" << "\t Material \t Yield Extraction \t PID \t photon reco \t track recon \t summed" <<  endl;
//  for (Int_t l=0;l< nPtBins;l++){
//      SysErrDatAverPaper << ptBins[l] <<"\t" << errorMaterial*2 << "\t" << errorsMeanCorrSignalExtraction[l] << "\t" << errorsMeanCorrPID[l]<< "\t" << errorsMeanCorrPhotonReco[l]<< "\t" <<errorsMeanCorrTrackReco[l] <<"\t" << errorsMeanCorrMatSummed[l]<< endl;
//  }
//
//  SysErrDatAverPaper.close();

}
