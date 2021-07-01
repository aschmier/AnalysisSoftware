#include <stdlib.h>
#include <iostream>
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
#include "TProfile2D.h"
#include "TF1.h"
#include "TVirtualFitter.h"
#include "TObject.h"
#include "TCanvas.h"
#include "TMultiGraph.h"
#include "TLegend.h"
#include "TDatabasePDG.h"
#include "TMinuit.h"
#include "TLatex.h"
#include "TASImage.h"
#include "TMath.h"
#include "TPostScript.h"
#include "TGraphErrors.h"
#include "TArrow.h"
#include "TGraphAsymmErrors.h"
#include "TGaxis.h"
#include "TMarker.h"
#include "TFitResultPtr.h"
#include "TFitResult.h"
#include "TLorentzVector.h"

#include "../CommonHeaders/PlottingGammaConversionHistos.h"
#include "../CommonHeaders/PlottingGammaConversionAdditional.h"
#include "../CommonHeaders/ConversionFunctionsBasicsAndLabeling.h"
#include "../CommonHeaders/FittingGammaConversion.h"
#include "../CommonHeaders/ConversionFunctions.h"

TF1* FitRecursiveGaussian (TH1* histo, Double_t precision, Double_t correctRange, Double_t fitRangeMin, Double_t fitRangeMax);
TF1* FitGaussian(TH1D* histo, Double_t fitRangeMin, Double_t fitRangeMax, Bool_t modeEta, Double_t ptcenter, Int_t iDataMC);
TF1* FitExpPlusGaussian(TH1D* histo, Double_t fitRangeMin, Double_t fitRangeMax, Bool_t modeEta, Double_t ptcenter, Int_t iDataMC);
TF1* FitDExpPlusGaussian(TH1D* histo, Double_t fitRangeMin, Double_t fitRangeMax, Bool_t modeEta, Double_t ptcenter, Int_t iDataMC);
TF1* FitBckg(TH1* fHisto, Double_t minFit, Double_t maxFit);
TF1* FitDataMC(TH1* fHisto, Double_t minFit, Double_t maxFit, TString selection, Double_t constPar = -1, Int_t mode = 2, Int_t fitmode = 0);
Float_t    FunctionNL_kSDM(Float_t e, Float_t p0, Float_t p1, Float_t p2);
template<class ForwardIt>

void SetLogBinningXTH(ForwardIt* histoRebin){
    TAxis *axisafter    = histoRebin->GetXaxis();
    Int_t bins          = axisafter->GetNbins();
    Double_t from       = axisafter->GetXmin();
    Double_t to         = axisafter->GetXmax();
    Double_t *newbins   = new Double_t[bins+1];
    newbins[0]          = from;
    Double_t factor     = TMath::Power(to/from, 1./bins);
    for(Int_t i=1; i<=bins; ++i)
        newbins[i]      = factor * newbins[i-1];
    axisafter->Set(bins, newbins);
    delete [] newbins;
    return;
}

Double_t fline(Double_t *x, Double_t *par)
{
   if (x[0] > 0.05 && x[0] < 0.17) {
      TF1::RejectPoint();
      return 0;
   }
   if (x[0] < 0.01 || x[0] > 0.3) {
      TF1::RejectPoint();
      return 0;
   }
   return par[0] + par[1]*x[0] + par[2]*x[0]*x[0];
}

Double_t fline2(Double_t *x, Double_t *par)
{
   if (x[0] > 0.5 && x[0] < 0.6) {
      TF1::RejectPoint();
      return 0;
   }
   if (x[0] < 0.3 || x[0] > 1.) {
      TF1::RejectPoint();
      return 0;
   }
   return par[0] + par[1]*x[0] + par[2]*x[0]*x[0];
}


// Double_t ScalingFactor(Double_t *x, Double_t *par) {
//     Double_t massPi0            = 0.1349766;
//     Float_t beta = 1./sqrt(1+(massPi0*massPi0)/(x[0]*x[0]));
//     Float_t gamma = 1./sqrt(1.-beta*beta);
//     // Float_t CosTheta = x[0]*x[0]/(x[0]*x[0]+gamma*gamma*beta*beta*massPi0*massPi0*0.5*0.5);
//     Float_t CosTheta = ((gamma * beta * massPi0 / 2)*(gamma * beta * massPi0 / 2) - (massPi0*massPi0/2/2))/((gamma * beta * massPi0 / 2)*(gamma * beta * massPi0 / 2) + (massPi0*massPi0/2/2));
    
//     return ( 1 / ( x[0] * TMath::Sqrt(1-CosTheta) ) );
// }
Double_t ScalingFactor(Double_t *x, Double_t *par) {
    Double_t massPi0            = 0.1349766;
    // Float_t beta = x[0] / massPi0 ;
    // Float_t beta = 1./sqrt(1+(massPi0*massPi0)/(x[0]*x[0]));
    // Float_t gamma = 1./sqrt(1.-beta*beta);
    // Float_t CosTheta = x[0]*x[0]/(x[0]*x[0]+gamma*gamma*beta*beta*massPi0*massPi0*0.5*0.5);
    // Float_t CosTheta = ((gamma * beta * massPi0 / 2)*(gamma * beta * massPi0 / 2) - (massPi0*massPi0/2/2))/((gamma * beta * massPi0 / 2)*(gamma * beta * massPi0 / 2) + (massPi0*massPi0/2/2));
    // Float_t CosTheta = 2*beta*beta -1;
    
    // return ( 1 / 2 * TMath::Sqrt(1-CosTheta)  );
    return ( 1/ ( massPi0*massPi0/x[0]/x[0] ) );

}
Double_t ScalingFactorEta(Double_t *x, Double_t *par) {
    Double_t massEta            = 0.547862;
    return ( 1/ ( massEta*massEta/x[0]/x[0] ) );

}
//****************************************************************************
//************** Function to Correct CaloNonLinearity4 ***********************
//****************************************************************************
void CorrectPCMSmearingV0(
                                TString configFileName  = "config.txt",
                                TString suffix          = "eps",
                                Bool_t enableAddCouts   = kFALSE,
                                Bool_t enablePol1Scaling   = kTRUE,
                                Int_t switchPeakFunction = 0
                              ){
    gROOT->Reset();
    gPrintViaErrorHandler = kTRUE;

    StyleSettingsThesis();
    SetPlotStyle();


    // General options
    TString select              = "";
    TString optionEnergy        = "";
    Int_t mode                  = 0;
    TString FittingFunction    ="";
    TString cfSetting    ="";
    TString cfSettingMC    ="";

    // variables for data set indentifiers and maximum number of sets
    Int_t nSets                 = 0;
    TString fPlotLabelsRatio[6] = {"", "", "", "", "", ""};
    TString strDataFile[6]      = {"", "", "", "", "", ""};
    TString strMCFile[6]        = {"", "", "", "", "", ""};
    TString dataCut[6]          = {"", "", "", "", "", ""};
    TString mcCut[6]            = {"", "", "", "", "", ""};

    // color settings
    const Int_t nColor          = 13;
    const Int_t nStyle          = 7;
    Color_t color[nColor]       = {kBlack,633,807,/*800,*/418,/*kGreen+4,*/435,601,879,806,852,kCyan+3,426};
    Int_t markerStyle[nStyle]   = {24,25,27,28,29,30,31};
    Double_t sigmaPi0            = 0.1349766;

    // pT range for sigma fitting
    Int_t ptBinRange[2]         = {0, 17};
    Int_t ptBinRangeEta[2]         = {0, 17};
    Int_t firstPtBinSet[6]      = { -1, -1, -1, -1, -1,     -1};
    Double_t ptBinsForRebin[10] = { -1, -1, -1, -1, -1,     -1, -1, -1, -1, -1 };
    Double_t ptBinsForRebinEta[10] = { -1, -1, -1, -1, -1,     -1, -1, -1, -1, -1 };
    Int_t rebin[10]             = { 1, 1, 1, 1, 1,  1, 1, 1, 1, 1};
    Int_t rebinEta[10]             = { 1, 1, 1, 1, 1,  1, 1, 1, 1, 1};
    Int_t exampleBin[40]        = { -1, -1, -1, -1, -1,     -1, -1, -1, -1, -1,
                                    -1, -1, -1, -1, -1,     -1, -1, -1, -1, -1,
                                    -1, -1, -1, -1, -1,     -1, -1, -1, -1, -1,
                                    -1, -1, -1, -1, -1,     -1, -1, -1, -1, -1 };
    Int_t nExampleBins          = 0;
    Int_t fixedOffSet           = -1;


    // pT binning general initialization
    Int_t fNBinsPt              = 300;
    Double_t fBinsPt[301];
    for (Int_t i = 0; i< 301; i++){
        fBinsPt[i]              = -1.;
    }
    Int_t fEtaNBinsPt              = 300;
    Double_t fEtaBinsPt[301];
    for (Int_t i = 0; i< 301; i++){
        fEtaBinsPt[i]              = -1.;
    }
    Double_t rangeHighPtFitsigma[4]  = {5, 10, 5, 10};
    Double_t rangeHighPtFitRatio[2] = {3, 10};
    Bool_t isNotFirstIte        = kFALSE;
    //**************************************************************************************************************
    //******************************* Read config file for detailed settings ***************************************
    //**************************************************************************************************************
    // ATTENTION: The data set has to be separated with either tabs or spaces a mixture of
    //            both will most likely lead to misconfigurations
    //**************************************************************************************************************

    cout << "INFO: You have chosen the following config file: " << configFileName.Data() << endl;
    ifstream fileConfigSmearing;
    fileConfigSmearing.open(configFileName,ios_base::in);
    if (!fileConfigSmearing) {
        cout << "ERROR: settings " << configFileName.Data() << " not found!" << endl;
        return;
    }

    // read settings from file
    for( TString tempLine; tempLine.ReadLine(fileConfigSmearing, kTRUE); ) {
        // check if line should be considered
        if (tempLine.BeginsWith("%") || tempLine.BeginsWith("#")){
            continue;
        }
        if (enableAddCouts) cout << tempLine.Data() << endl;

        // Separate the string according to tabulators
        TObjArray *tempArr  = tempLine.Tokenize("\t");
        if(tempArr->GetEntries()<1){
            cout << "nothing to be done" << endl;
            delete tempArr;
            continue;
        } else if (tempArr->GetEntries() == 1){
            // Separate the string according to space
            tempArr       = tempLine.Tokenize(" ");
            if(tempArr->GetEntries()<1){
                cout << "nothing to be done" << endl;
                delete tempArr;
                continue;
            } else if (tempArr->GetEntries() == 1 ) {
                cout << ((TString)((TObjString*)tempArr->At(0))->GetString()).Data() << " has not be reset, no value given!" << endl;
                delete tempArr;
                continue;
            }
        }

        // Put them to the correct variables
        TString tempValue   = (TString)((TObjString*)tempArr->At(0))->GetString();
        // reading selection string
        if (tempValue.BeginsWith("select",TString::kIgnoreCase)){
            select          = (TString)((TObjString*)tempArr->At(1))->GetString();
        // reading energy setup
        } else if (tempValue.BeginsWith("energy",TString::kIgnoreCase)){
            optionEnergy    = (TString)((TObjString*)tempArr->At(1))->GetString();
        // reading energy setup
        } else if (tempValue.BeginsWith("cfSettingMC",TString::kIgnoreCase)){
            cfSettingMC    = (TString)((TObjString*)tempArr->At(1))->GetString();
            if (enableAddCouts) cout << "setting cfSettingMC to " << cfSetting.Data() << endl;
        } else if (tempValue.BeginsWith("cfSetting",TString::kIgnoreCase)){
            cfSetting    = (TString)((TObjString*)tempArr->At(1))->GetString();
            if (enableAddCouts) cout << "setting cfSetting to " << cfSetting.Data() << endl;
        // reading mode
        } else if (tempValue.BeginsWith("mode",TString::kIgnoreCase)){
            mode            = ((TString)((TObjString*)tempArr->At(1))->GetString()).Atoi();
        // reading number of data sets to be compared
        } else if (tempValue.BeginsWith("nSets",TString::kIgnoreCase)){
            nSets            = ((TString)((TObjString*)tempArr->At(1))->GetString()).Atoi();
        // enable add plotting if not first iteration
        } else if (tempValue.BeginsWith("notFirstIteration",TString::kIgnoreCase)){
            if (((TString)((TObjString*)tempArr->At(1))->GetString()).CompareTo("1") == 0)
                isNotFirstIte    = kTRUE;
        // reading maximum number of pt bins
        } else if (tempValue.BeginsWith("maxNPtBins",TString::kIgnoreCase)){
            fNBinsPt        = ((TString)((TObjString*)tempArr->At(1))->GetString()).Atoi();
        // reading maximum number of pt bins
        } else if (tempValue.BeginsWith("maxEtaNPtBins",TString::kIgnoreCase)){
            fEtaNBinsPt        = ((TString)((TObjString*)tempArr->At(1))->GetString()).Atoi();
        // reading ptBins
        } else if (tempValue.BeginsWith("fBinsPt",TString::kIgnoreCase)){
            if (enableAddCouts) cout << "setting ptBins" << endl;
            for(Int_t i = 1; i<tempArr->GetEntries() && i < 302 ; i++){
                if (enableAddCouts) cout << i << "\t" <<((TString)((TObjString*)tempArr->At(i))->GetString()).Data() << endl;
                if (((TString)((TObjString*)tempArr->At(i))->GetString()).CompareTo("stop",TString::kIgnoreCase))
                    fBinsPt[i-1]        = ((TString)((TObjString*)tempArr->At(i))->GetString()).Atof();
                else
                    i                   = tempArr->GetEntries();
            }
        // reading ptBins
        } else if (tempValue.BeginsWith("fEtaBinsPt",TString::kIgnoreCase)){
            if (enableAddCouts) cout << "setting ptBins Eta" << endl;
            for(Int_t i = 1; i<tempArr->GetEntries() && i < 302 ; i++){
                if (enableAddCouts) cout << i << "\t" <<((TString)((TObjString*)tempArr->At(i))->GetString()).Data() << endl;
                if (((TString)((TObjString*)tempArr->At(i))->GetString()).CompareTo("stop",TString::kIgnoreCase))
                    fEtaBinsPt[i-1]        = ((TString)((TObjString*)tempArr->At(i))->GetString()).Atof();
                else
                    i                   = tempArr->GetEntries();
            }
        // reading min and max bin for pt
        } else if (tempValue.BeginsWith("ptRangeBins",TString::kIgnoreCase)){
            if (enableAddCouts) cout << "setting ptBin bin Range" << endl;
            ptBinRange[0]               = ((TString)((TObjString*)tempArr->At(1))->GetString()).Atoi();
            ptBinRange[1]               = ((TString)((TObjString*)tempArr->At(2))->GetString()).Atoi();
        // reading min and max bin for pt
        } else if (tempValue.BeginsWith("ptEtaRangeBins",TString::kIgnoreCase)){
            if (enableAddCouts) cout << "setting ptBin bin Range Eta" << endl;
            ptBinRangeEta[0]               = ((TString)((TObjString*)tempArr->At(1))->GetString()).Atoi();
            ptBinRangeEta[1]               = ((TString)((TObjString*)tempArr->At(2))->GetString()).Atoi();
        // reading range for high pt const fit to ratio
        } else if (tempValue.BeginsWith("rangeHighPtFitRatio",TString::kIgnoreCase)){
            rangeHighPtFitRatio[0]      = ((TString)((TObjString*)tempArr->At(1))->GetString()).Atof();
            rangeHighPtFitRatio[1]      = ((TString)((TObjString*)tempArr->At(2))->GetString()).Atof();
        // reading min and max pt range for sigma fits
        } else if (tempValue.BeginsWith("rangeHighPtFitsigma",TString::kIgnoreCase)){
            if (enableAddCouts) cout << "setting min and max pt range for sigma fits" << endl;
            rangeHighPtFitsigma[0]       = ((TString)((TObjString*)tempArr->At(1))->GetString()).Atof();
            rangeHighPtFitsigma[1]       = ((TString)((TObjString*)tempArr->At(2))->GetString()).Atof();
            rangeHighPtFitsigma[2]       = ((TString)((TObjString*)tempArr->At(3))->GetString()).Atof();
            rangeHighPtFitsigma[3]       = ((TString)((TObjString*)tempArr->At(4))->GetString()).Atof();
        // read data file names
        } else if (tempValue.BeginsWith("dataFileNames",TString::kIgnoreCase)){
            if (enableAddCouts) cout << "setting data file paths" << endl;
            for(Int_t i = 1; i<tempArr->GetEntries() && i < 6+1 ; i++){
                if (enableAddCouts) cout << i << "\t" <<((TString)((TObjString*)tempArr->At(i))->GetString()).Data() << endl;
                if (((TString)((TObjString*)tempArr->At(i))->GetString()).CompareTo("stop",TString::kIgnoreCase))
                    strDataFile[i-1]    = ((TString)((TObjString*)tempArr->At(i))->GetString());
                else
                    i                   = tempArr->GetEntries();
            }
        // read cut string for data files
        } else if (tempValue.BeginsWith("cutData",TString::kIgnoreCase)){
            if (enableAddCouts) cout << "setting cutstring in data files" << endl;
            for(Int_t i = 1; i<tempArr->GetEntries() && i < 6+1 ; i++){
                if (enableAddCouts) cout << i << "\t" <<((TString)((TObjString*)tempArr->At(i))->GetString()).Data() << endl;
                if (((TString)((TObjString*)tempArr->At(i))->GetString()).CompareTo("stop",TString::kIgnoreCase))
                    dataCut[i-1]        = ((TString)((TObjString*)tempArr->At(i))->GetString());
                else
                    i                   = tempArr->GetEntries();
            }
        // read MC file names
        } else if (tempValue.BeginsWith("MCFileNames",TString::kIgnoreCase)){
            if (enableAddCouts) cout << "setting MC file paths" << endl;
            for(Int_t i = 1; i<tempArr->GetEntries() && i < 6+1 ; i++){
                if (enableAddCouts) cout << i << "\t" <<((TString)((TObjString*)tempArr->At(i))->GetString()).Data() << endl;
                if (((TString)((TObjString*)tempArr->At(i))->GetString()).CompareTo("stop",TString::kIgnoreCase))
                    strMCFile[i-1]    = ((TString)((TObjString*)tempArr->At(i))->GetString());
                else
                    i                   = tempArr->GetEntries();
            }
        // read cut string for MC files
        } else if (tempValue.BeginsWith("cutMC",TString::kIgnoreCase)){
            if (enableAddCouts) cout << "setting cutstring in MC files" << endl;
            for(Int_t i = 1; i<tempArr->GetEntries() && i < 6+1 ; i++){
                if (enableAddCouts) cout << i << "\t" <<((TString)((TObjString*)tempArr->At(i))->GetString()).Data() << endl;
                if (((TString)((TObjString*)tempArr->At(i))->GetString()).CompareTo("stop",TString::kIgnoreCase))
                    mcCut[i-1]        = ((TString)((TObjString*)tempArr->At(i))->GetString());
                else
                    i                   = tempArr->GetEntries();
            }
        // read plot labels
        } else if (tempValue.BeginsWith("plotLabels",TString::kIgnoreCase)){
            if (enableAddCouts) cout << "setting cutstring in plot labels for different data combinations" << endl;
            for(Int_t i = 1; i<tempArr->GetEntries() && i < 6+1 ; i++){
                if (enableAddCouts) cout << i << "\t" <<((TString)((TObjString*)tempArr->At(i))->GetString()).Data() << endl;
                if (((TString)((TObjString*)tempArr->At(i))->GetString()).CompareTo("stop",TString::kIgnoreCase))
                    fPlotLabelsRatio[i-1]   = ((TString)((TObjString*)tempArr->At(i))->GetString());
                else
                    i                       = tempArr->GetEntries();
            }
        // read example bins
        } else if (tempValue.BeginsWith("exampleBins",TString::kIgnoreCase)){
            if (enableAddCouts) cout << "setting exampleBins" << endl;
            for(Int_t i = 1; i<tempArr->GetEntries() && i < 40+1 ; i++){
                if (enableAddCouts) cout << i << "\t" <<((TString)((TObjString*)tempArr->At(i))->GetString()).Data() << endl;
                if (((TString)((TObjString*)tempArr->At(i))->GetString()).CompareTo("stop",TString::kIgnoreCase))
                    exampleBin[i-1]         = (((TString)((TObjString*)tempArr->At(i))->GetString())).Atoi();
                else
                    i                       = tempArr->GetEntries();
            }
        // read rebin pt boundaries
        } else if (tempValue.BeginsWith("rebinPt",TString::kIgnoreCase)){
            if (enableAddCouts) cout << "setting rebinPt boundaries for rebins" << endl;
            for(Int_t i = 1; i<tempArr->GetEntries() && i < 10+1 ; i++){
                if (enableAddCouts) cout << i << "\t" <<((TString)((TObjString*)tempArr->At(i))->GetString()).Data() << endl;
                if (((TString)((TObjString*)tempArr->At(i))->GetString()).CompareTo("stop",TString::kIgnoreCase))
                    ptBinsForRebin[i-1]     = (((TString)((TObjString*)tempArr->At(i))->GetString())).Atof();
                else
                    i                       = tempArr->GetEntries();
            }
        // read rebin values
        } else if (tempValue.BeginsWith("rebinValue",TString::kIgnoreCase)){
            if (enableAddCouts) cout << "setting value for rebin" << endl;
            for(Int_t i = 1; i<tempArr->GetEntries() && i < 10+1 ; i++){
                if (enableAddCouts) cout << i << "\t" <<((TString)((TObjString*)tempArr->At(i))->GetString()).Data() << endl;
                if (((TString)((TObjString*)tempArr->At(i))->GetString()).CompareTo("stop",TString::kIgnoreCase))
                    rebin[i-1]              = (((TString)((TObjString*)tempArr->At(i))->GetString())).Atoi();
                else
                    i                       = tempArr->GetEntries();
            }
        // read rebin pt boundaries
        } else if (tempValue.BeginsWith("EtarebinPt",TString::kIgnoreCase)){
            if (enableAddCouts) cout << "setting rebinPt boundaries for rebins for Eta" << endl;
            for(Int_t i = 1; i<tempArr->GetEntries() && i < 10+1 ; i++){
                if (enableAddCouts) cout << i << "\t" <<((TString)((TObjString*)tempArr->At(i))->GetString()).Data() << endl;
                if (((TString)((TObjString*)tempArr->At(i))->GetString()).CompareTo("stop",TString::kIgnoreCase))
                    ptBinsForRebinEta[i-1]     = (((TString)((TObjString*)tempArr->At(i))->GetString())).Atof();
                else
                    i                       = tempArr->GetEntries();
            }
        // read rebin values
        } else if (tempValue.BeginsWith("EtarebinValue",TString::kIgnoreCase)){
            if (enableAddCouts) cout << "setting value for rebin for Eta" << endl;
            for(Int_t i = 1; i<tempArr->GetEntries() && i < 10+1 ; i++){
                if (enableAddCouts) cout << i << "\t" <<((TString)((TObjString*)tempArr->At(i))->GetString()).Data() << endl;
                if (((TString)((TObjString*)tempArr->At(i))->GetString()).CompareTo("stop",TString::kIgnoreCase))
                    rebinEta[i-1]              = (((TString)((TObjString*)tempArr->At(i))->GetString())).Atoi();
                else
                    i                       = tempArr->GetEntries();
            }
        // read first bin of every trigger set
        } else if (tempValue.BeginsWith("firstPtBinSet",TString::kIgnoreCase)){
            if (enableAddCouts) cout << "setting value for switches for trigger bins" << endl;
            for(Int_t i = 1; i<tempArr->GetEntries() && i < 6+1 ; i++){
                if (enableAddCouts) cout << i << "\t" <<((TString)((TObjString*)tempArr->At(i))->GetString()).Data() << endl;
                if (((TString)((TObjString*)tempArr->At(i))->GetString()).CompareTo("stop",TString::kIgnoreCase))
                    firstPtBinSet[i-1]      = (((TString)((TObjString*)tempArr->At(i))->GetString())).Atoi();
                else
                    i                       = tempArr->GetEntries();
            }
        }
        delete tempArr;
    }

    //**************************************************************************************************************
    //******************************* Check wether settings were valid *********************************************
    //**************************************************************************************************************

    cout << "**************************************************************************" << endl;
    cout << "**************** Settings found in config file ***************************" << endl;
    cout << "**************************************************************************" << endl;
    cout << "select:\t"<< select.Data() << endl;
    cout << "energyFlag:\t"<< optionEnergy.Data() << endl;
    cout << "mode:\t"<< mode << endl;
    if (isNotFirstIte) cout << "This is not the first iteration" << endl;
    cout << "(if used) loading for CF output data: " << cfSetting.Data() << endl;
    cout << "(if used) loading for CF output MC: " << cfSettingMC.Data() << endl;
    cout << "fNBinsPt:\t" << fNBinsPt << endl;
    cout << "ptBinning:" << endl;
    for (Int_t i = 0; i < fNBinsPt+1; i++){
        cout <<  fBinsPt[i] << "\t" ;
    }
    cout << endl << "fBinsPt Eta:\t" << fEtaNBinsPt << endl;
    cout << "ptBinning Eta:" << endl;
    for (Int_t i = 0; i < fEtaNBinsPt+1; i++){
        cout <<  fEtaBinsPt[i] << "\t" ;
    }
    cout << endl;
    if (ptBinsForRebin[0] != -1 ){
        cout << "setting rebin values" << endl;
        for (Int_t i = 0; (ptBinsForRebin[i] != -1 && i < 10); i++ ){
            cout << "pt range:\t"<< ptBinsForRebin[i] ;
            if (ptBinsForRebin[i+1] != -1)
                cout << " - " << ptBinsForRebin[i+1] << "\t" ;
            else
                cout << " - " << fBinsPt[fNBinsPt] << "\t" ;
            cout << "rebin invsigma: \t" << rebin[i] << endl;
        }
    }
    if (ptBinsForRebinEta[0] != -1 ){
        cout << "setting rebin values for Eta" << endl;
        for (Int_t i = 0; (ptBinsForRebinEta[i] != -1 && i < 10); i++ ){
            cout << "pt range:\t"<< ptBinsForRebinEta[i] ;
            if (ptBinsForRebinEta[i+1] != -1)
                cout << " - " << ptBinsForRebinEta[i+1] << "\t" ;
            else
                cout << " - " << fBinsPt[fNBinsPt] << "\t" ;
            cout << "rebin invsigma: \t" << rebinEta[i] << endl;
        }
    }
    cout << "pt bin range set: " << ptBinRange[0] << " - " << ptBinRange[1] << endl;
    cout << "pt bin range set Eta: " << ptBinRangeEta[0] << " - " << ptBinRangeEta[1] << endl;

    cout << "exampleBins:" << endl;
    for (Int_t i = 0; exampleBin[i] != -1; i++){
        cout <<  exampleBin[i] << "\t" ;
        nExampleBins++;
    }
    cout << endl;
    cout << "nSets:\t" << nSets << endl;
    if ( !optionEnergy.CompareTo("") || mode == -1 || nSets == 0 || !select.CompareTo("") ){
        cout << "ABORTING: You are missing the select, nSets, energy or mode setting, can't continue like that" << endl;
        return;
    }
    cout << "**************************************************************************" << endl;
    cout << "Data set setup: " << endl;
    for (Int_t i = 0; i < nSets; i++){
        if ( strDataFile[i].CompareTo("") != 0 && strMCFile[i].CompareTo("") != 0 ){
            if (dataCut[i].CompareTo("") == 0 ){
                cout << "ERROR: you forgot to tell us which cut, aborting" << endl;
                return;
            }
            if (mcCut[i].CompareTo("") == 0){
                cout << "----------" << endl;
                cout << "ATTENTION: setting same cut as data for MC" << endl;
                cout << "----------" << endl;
                mcCut[i]    = dataCut[i];
            }
            if (fPlotLabelsRatio[i].CompareTo("") == 0){
                cout << "----------" << endl;
                cout << "ATTENTION: no label for plotting has been set for this combination" << endl;
                cout << "----------" << endl;
            }
            if (firstPtBinSet[i] == -1 || firstPtBinSet[i] > fNBinsPt){
                cout << "----------" << endl;
                cout << "ATTENTION: will never switch to this trigger" << endl;
                cout << "----------" << endl;
            }
            cout << i << "\t" << fPlotLabelsRatio[i].Data() << "\t" << strDataFile[i].Data() << "\t" << dataCut[i].Data() << "\t" << strMCFile[i].Data() << "\t" << mcCut[i].Data() << "\t switch at:\t" << fBinsPt[firstPtBinSet[i]] << endl;
        } else {
            cout << "ERROR: no correct data set combi was set, aborting" << endl;
            cout << strDataFile[i].Data() << "\t" <<  strMCFile[i].Data() << endl;
            return;
        }
    }
    cout << "**************************************************************************" << endl;
    cout << "**************************************************************************" << endl;


    //*******************************************************************************
    //***************** create outputfolder and root file ***************************
    //*******************************************************************************
    TString outputDir               = "CorrectPCMSmearing";
    gSystem->Exec("mkdir -p "+outputDir);
    TString outputDirSampleSummary  = Form("%s/%s",outputDir.Data(),((TString)ReturnTextReconstructionProcess(mode)).Data());
    TString outputDirSample         = Form("%s/%s",outputDirSampleSummary.Data(), select.Data());
    gSystem->Exec("mkdir -p "+outputDirSampleSummary);
    gSystem->Exec("mkdir -p "+outputDirSample);
    // Output - root file
    TString nameOutput              = Form("%s/CorrectPCMSmearing_%s.root",outputDirSampleSummary.Data(),select.Data());
    TFile* fOutput                = new TFile(nameOutput,"RECREATE");


    //*******************************************************************************
    //***************** set up proper labeling **************************************
    //*******************************************************************************
    for (Int_t i = 0; i < nSets; i++){
        if (firstPtBinSet[i+1] != -1)
            fPlotLabelsRatio[i] = Form("%0.2f < #it{E}_{PCM #gamma} < %0.2f GeV : %s",fBinsPt[firstPtBinSet[i]],fBinsPt[firstPtBinSet[i+1]],fPlotLabelsRatio[i].Data());
        else
            fPlotLabelsRatio[i] = Form("#it{E}_{PCM #gamma} #geq %0.2f GeV : %s",fBinsPt[firstPtBinSet[i]],fPlotLabelsRatio[i].Data());
    }

    TString recGamma            = ReturnFullTextReconstructionProcess(mode);
    TString fTextMeasurement    = Form("#pi^{0} #rightarrow #gamma#gamma");
    TString fCollisionSystem    = ReturnFullCollisionsSystem(optionEnergy);
    if (fCollisionSystem.CompareTo("") == 0){
        cout << "No correct collision system specification, has been given" << endl;
        return;
    }

    //*******************************************************************************
    //********************* setting hist names to be loaded *************************
    //*******************************************************************************
    TString nameSignalHisto;
    TString nameBGHisto;
    if (mode == 0) {
        nameSignalHisto      = "ESD_Mother_InvMass_Pt_Calib";
        nameBGHisto          = "ESD_Background_InvMass_Pt_Calib";
    } else {
        nameSignalHisto      = "ESD_Mother_InvMass_E_Calib_PCM";
        nameBGHisto          = "ESD_Background_InvMass_E_Calib_PCM";
    }
    

    //*******************************************************************************
    //**********************  create output histos **********************************
    //*******************************************************************************
    TH1D* histMCResults         = new TH1D("Sigma MC","; #it{E}_{PCM #gamma} (GeV); mean sigma MC",fNBinsPt,fBinsPt);
    TH1D* histDataResults       = new TH1D("Sigma Data","; #it{E}_{PCM #gamma} (GeV); mean sigma Data",fNBinsPt,fBinsPt);
    TH1D* histDataMCResults     = new TH1D("Sigma ratio MC/Data","; #it{E}_{PCM #gamma} (GeV); mean sigma ratio (MC/Data)",fNBinsPt,fBinsPt);
    TH1D* histMCAmplitude         = new TH1D("Amplitude","; #it{E}_{PCM #gamma} (GeV); Amplitude",fNBinsPt,fBinsPt);
    TH1D* histDataAmplitude       = new TH1D("Amplitude","; #it{E}_{PCM #gamma} (GeV); Amplitude",fNBinsPt,fBinsPt);
    TH1D* histMCTail         = new TH1D("Tail","; #it{E}_{PCM #gamma} (GeV); Tail",fNBinsPt,fBinsPt);
    TH1D* histMCTailRight         = new TH1D("TailRight","; #it{E}_{PCM #gamma} (GeV); Right Tail",fNBinsPt,fBinsPt);
    TH1D* histDataTail       = new TH1D("Tail","; #it{E}_{PCM #gamma} (GeV); Tail",fNBinsPt,fBinsPt);
    TH1D* histDataTailRight       = new TH1D("TailRight","; #it{E}_{PCM #gamma} (GeV); Right Tail",fNBinsPt,fBinsPt);
    TH1D* histMCMass         = new TH1D("Mean Mass MC","; #it{E}_{PCM #gamma} (GeV); Sigma",fNBinsPt,fBinsPt);
    TH1D* histDataMass       = new TH1D("Mean Mass Data","; #it{E}_{PCM #gamma} (GeV); Sigma",fNBinsPt,fBinsPt);    
    
    TH1D* histMCResultsEta         = new TH1D("Sigma MC","; #it{E}_{PCM #gamma} (GeV); mean sigma MC",fEtaNBinsPt,fEtaBinsPt);
    TH1D* histDataResultsEta       = new TH1D("Sigma Data","; #it{E}_{PCM #gamma} (GeV); mean sigma Data",fEtaNBinsPt,fEtaBinsPt);
    TH1D* histDataMCResultsEta     = new TH1D("Sigma ratio MC/Data","; #it{E}_{PCM #gamma} (GeV); mean sigma ratio (MC/Data)",fEtaNBinsPt,fEtaBinsPt);
    TH1D* histMCAmplitudeEta         = new TH1D("Amplitude","; #it{E}_{PCM #gamma} (GeV); Amplitude",fEtaNBinsPt,fEtaBinsPt);
    TH1D* histDataAmplitudeEta       = new TH1D("Amplitude","; #it{E}_{PCM #gamma} (GeV); Amplitude",fEtaNBinsPt,fEtaBinsPt);
    TH1D* histMCTailEta         = new TH1D("Tail","; #it{E}_{PCM #gamma} (GeV); Tail",fEtaNBinsPt,fEtaBinsPt);
    TH1D* histDataTailEta       = new TH1D("Tail","; #it{E}_{PCM #gamma} (GeV); Tail",fEtaNBinsPt,fEtaBinsPt);
    TH1D* histMCMassEta         = new TH1D("Mean Mass MC","; #it{E}_{PCM #gamma} (GeV); Sigma",fEtaNBinsPt,fEtaBinsPt);
    TH1D* histDataMassEta       = new TH1D("Mean Mass Data","; #it{E}_{PCM #gamma} (GeV); Sigma",fEtaNBinsPt,fEtaBinsPt);
    TH1D* histMCTailRightEta         = new TH1D("TailRight","; #it{E}_{PCM #gamma} (GeV); Right Tail",fNBinsPt,fBinsPt);
    TH1D* histDataTailRightEta       = new TH1D("TailRight","; #it{E}_{PCM #gamma} (GeV); Right Tail",fNBinsPt,fBinsPt);

    histMCResults->SetDirectory(0);
    histDataResults->SetDirectory(0);
    histMCResults->GetXaxis()->SetRangeUser(fBinsPt[ptBinRange[0]],fBinsPt[ptBinRange[1]]);
    histDataResults->GetXaxis()->SetRangeUser(fBinsPt[ptBinRange[0]],fBinsPt[ptBinRange[1]]);
    histMCResults->GetYaxis()->SetRangeUser(0.12,0.142);
    histDataResults->GetYaxis()->SetRangeUser(0.12,0.142);    
    histMCResultsEta->SetDirectory(0);
    histDataResultsEta->SetDirectory(0);
    histMCResultsEta->GetXaxis()->SetRangeUser(fEtaBinsPt[ptBinRangeEta[0]],fEtaBinsPt[ptBinRangeEta[1]]);
    histDataResultsEta->GetXaxis()->SetRangeUser(fEtaBinsPt[ptBinRangeEta[0]],fEtaBinsPt[ptBinRangeEta[1]]);
    // histMCResultsEta->GetYaxis()->SetRangeUser(0.12,0.142);
    // histDataResultsEta->GetYaxis()->SetRangeUser(0.12,0.142);
    TF1* fFitReco;
    TF1* fFitsigma;
    TF1* fFitsigma2;
    TF1* fFitsigma3;
    TF1* fFitsigma4;
    TF1* fFitsigma5;
    TCanvas *canvas             = new TCanvas("canvas","",200,0,1350,900);  // gives the page size
    DrawGammaCanvasSettings(canvas, 0.1, 0.02, 0.06, 0.1);


    //*******************************************************************************
    //**************************** read Input ***************************************
    //*******************************************************************************

    TFile* dataFile             = new TFile(strDataFile[0].Data(),"READ");
    if(dataFile->IsZombie()) {cout << "Info: ROOT file '" << strDataFile[0].Data() << "' could not be openend, return!" << endl; return;}
    TString mainDirNameData     =  AutoDetectMainTList(mode , dataFile, "", cfSetting);
    TList* dataTopDir           = (TList*) dataFile->Get(mainDirNameData.Data());
    if(dataTopDir == NULL) {cout << "ERROR: dataTopDir not Found"<<endl; return;}
    TList* dataTopContainer     = (TList*) dataTopDir->FindObject(Form("Cut Number %s",dataCut[0].Data()));
    if(dataTopContainer == NULL) {cout << "ERROR: " << Form("Cut Number '%s'",dataCut[0].Data()) << " not found in Data-File" << endl; return;}
    TList* dataESDContainer     = (TList*) dataTopContainer->FindObject(Form("%s ESD histograms",dataCut[0].Data()));
    if(dataESDContainer == NULL) {cout << "ERROR: " << Form("'%s' ESD histograms",dataCut[0].Data()) << " not found in Data-File" << endl; return;}

    TFile* mcFile               = new TFile(strMCFile[0].Data(),"READ");
    if(mcFile->IsZombie()) {cout << "Info: ROOT file '" << strMCFile[0].Data() << "' could not be openend, return!" << endl; return;}
    TString mainDirNameMC       =  AutoDetectMainTList(mode , mcFile, "", cfSettingMC);
    TList* mcTopDir             = (TList*) mcFile->Get(mainDirNameMC.Data());
    if(mcTopDir == NULL) {cout << "ERROR: mcTopDir not Found"<<endl; return;}
    TList* mcTopContainer       = (TList*) mcTopDir->FindObject(Form("Cut Number %s",mcCut[0].Data()));
    if(mcTopContainer == NULL) {cout << "ERROR: " << Form("Cut Number '%s'",mcCut[0].Data()) << " not found in MC-File" << endl; return;}
    TList* mcESDContainer       = (TList*) mcTopContainer->FindObject(Form("%s ESD histograms",mcCut[0].Data()));
    if(mcESDContainer == NULL) {cout << "ERROR: " << Form("'%s' ESD histograms",mcCut[0].Data()) << " not found in MC-File" << endl; return;}

    // Getting correct hist input
    TH2F* dataInvsigmaPt    = (TH2F*) dataESDContainer->FindObject(nameSignalHisto.Data());
    TH2F* dataBGInvsigmaPt  = (TH2F*) dataESDContainer->FindObject(nameBGHisto.Data());
    TH2F* mcInvsigmaPt      = (TH2F*) mcESDContainer->FindObject(nameSignalHisto.Data());
    TH2F* mcBGInvsigmaPt    = (TH2F*) mcESDContainer->FindObject(nameBGHisto.Data());
    if(!dataInvsigmaPt){
        cout << "did not find ESD_Mother_InvMass_E_Calib_PCM in data..." << endl;
    }
    if(!dataBGInvsigmaPt){
        cout << "did not find ESD_Background_InvMass_E_Calib_PCM in data..." << endl;
    }
    if(!mcInvsigmaPt){
        cout << "did not find ESD_Mother_InvMass_E_Calib_PCM in MC..." << endl;
    }
    if(!mcBGInvsigmaPt){
        cout << "did not find ESD_Background_InvMass_E_Calib_PCM in MC..." << endl;
    }


    // storing 2D hists in output root file
    fOutput->cd();
    dataInvsigmaPt->Write("Data - ESD_Mother_Invsigma");
    dataBGInvsigmaPt->Write("Data - ESD_Background_Invsigma");
    mcInvsigmaPt->Write("MC - ESD_Mother_Invsigma");
    mcBGInvsigmaPt->Write("MC - ESD_Background_Invsigma");

    TString dataMC[2]           = {"Data","MC"};
    Int_t triggerSel            = 0;
    Int_t rebinWindow           = 0;

    Int_t Nbins           = ptBinRange[1]-1-ptBinRange[0];
    Int_t NbinsX           = (Int_t) TMath::Sqrt(Nbins)+1;
    Int_t NbinsY           = (Int_t) TMath::Sqrt(Nbins);
    TCanvas *canvasAllData1          = new TCanvas("canvasAllData1","",1400,900);  // gives the page size
    DrawGammaCanvasSettings( canvasAllData1, 0, 0, 0, 0);
    TPad * padAllData1               = new TPad("padAllData1","",-0.0,0.0,1.,1.,0);   // gives the size of the histo areas
    DrawGammaPadSettings( padAllData1, 0, 0, 0, 0);
    padAllData1->Divide(NbinsX , NbinsY);
    canvasAllData1->cd();
    padAllData1->Draw();
    
    TCanvas *canvasAllData2          = new TCanvas("canvasAllData2","",1400,900);  // gives the page size
    DrawGammaCanvasSettings( canvasAllData2, 0, 0, 0, 0);
    TPad * padAllData2               = new TPad("padAllData2","",-0.0,0.0,1.,1.,0);   // gives the size of the histo areas
    DrawGammaPadSettings( padAllData2, 0, 0, 0, 0);
    padAllData2->Divide(NbinsX , NbinsY);
    canvasAllData2->cd();
    padAllData2->Draw();
    
    TCanvas *canvasAllMC1          = new TCanvas("canvasAllMC1","",1400,900);  // gives the page size
    DrawGammaCanvasSettings( canvasAllMC1, 0, 0, 0, 0);
    TPad * padAllMC1               = new TPad("padAllMC1","",-0.0,0.0,1.,1.,0);   // gives the size of the histo areas
    DrawGammaPadSettings( padAllMC1, 0, 0, 0, 0);
    padAllMC1->Divide(NbinsX , NbinsY);
    canvasAllMC1->cd();
    padAllMC1->Draw();
    
    TCanvas *canvasAllMC2          = new TCanvas("canvasAllMC2","",1400,900);  // gives the page size
    DrawGammaCanvasSettings( canvasAllMC2, 0, 0, 0, 0);
    TPad * padAllMC2               = new TPad("padAllMC2","",-0.0,0.0,1.,1.,0);   // gives the size of the histo areas
    DrawGammaPadSettings( padAllMC2, 0, 0, 0, 0);
    padAllMC2->Divide(NbinsX , NbinsY);
    canvasAllMC2->cd();
    padAllMC2->Draw();


    // ####################
    TCanvas *canvasAllData1Eta          = new TCanvas("canvasAllData1Eta","",1400,900);  // gives the page size
    DrawGammaCanvasSettings( canvasAllData1Eta, 0, 0, 0, 0);
    TPad * padAllData1Eta               = new TPad("padAllData1Eta","",-0.0,0.0,1.,1.,0);   // gives the size of the histo areas
    DrawGammaPadSettings( padAllData1Eta, 0, 0, 0, 0);
    padAllData1Eta->Divide(NbinsX , NbinsY);
    canvasAllData1Eta->cd();
    padAllData1Eta->Draw();
    
    TCanvas *canvasAllData2Eta          = new TCanvas("canvasAllData2Eta","",1400,900);  // gives the page size
    DrawGammaCanvasSettings( canvasAllData2Eta, 0, 0, 0, 0);
    TPad * padAllData2Eta               = new TPad("padAllData2Eta","",-0.0,0.0,1.,1.,0);   // gives the size of the histo areas
    DrawGammaPadSettings( padAllData2Eta, 0, 0, 0, 0);
    padAllData2Eta->Divide(NbinsX , NbinsY);
    canvasAllData2Eta->cd();
    padAllData2Eta->Draw();
    
    TCanvas *canvasAllMC1Eta          = new TCanvas("canvasAllMC1Eta","",1400,900);  // gives the page size
    DrawGammaCanvasSettings( canvasAllMC1Eta, 0, 0, 0, 0);
    TPad * padAllMC1Eta               = new TPad("padAllMC1Eta","",-0.0,0.0,1.,1.,0);   // gives the size of the histo areas
    DrawGammaPadSettings( padAllMC1Eta, 0, 0, 0, 0);
    padAllMC1Eta->Divide(NbinsX , NbinsY);
    canvasAllMC1Eta->cd();
    padAllMC1Eta->Draw();
    
    TCanvas *canvasAllMC2Eta          = new TCanvas("canvasAllMC2Eta","",1400,900);  // gives the page size
    DrawGammaCanvasSettings( canvasAllMC2Eta, 0, 0, 0, 0);
    TPad * padAllMC2Eta               = new TPad("padAllMC2Eta","",-0.0,0.0,1.,1.,0);   // gives the size of the histo areas
    DrawGammaPadSettings( padAllMC2Eta, 0, 0, 0, 0);
    padAllMC2Eta->Divide(NbinsX , NbinsY);
    canvasAllMC2Eta->cd();
    padAllMC2Eta->Draw();

    cout << "Nbins: " << Nbins/2 +1 << "  " << Nbins/2  << endl;
    canvas->cd();


    for(Int_t iClusterPt=ptBinRange[0]; iClusterPt<ptBinRange[1]; iClusterPt++){
        if(iClusterPt==firstPtBinSet[triggerSel+1]){
            // switching to trigger
            cout << endl;
            cout << "-----------------------------------------------------" << endl;
            cout << "\t Closing open files, switching to Trigger!" << endl;
            cout << "bin: " << firstPtBinSet[triggerSel] << endl;
            cout << "-----------------------------------------------------" << endl;

            if(!strDataFile[triggerSel+1].IsNull()){
                dataESDContainer->Clear(); dataTopContainer->Clear(); dataTopDir->Clear(); dataFile->Delete();
                dataFile            = new TFile(strDataFile[triggerSel+1].Data(),"READ");
                if(dataFile->IsZombie()) {cout << "Info: ROOT file '" << strDataFile[triggerSel+1].Data() << "' could not be openend, return!" << endl; return;}
                mainDirNameData     =  AutoDetectMainTList(mode , dataFile, "", cfSetting);
                dataTopDir          = (TList*) dataFile->Get(mainDirNameData.Data());
                if(dataTopDir == NULL) {cout << "ERROR: dataTopDir not Found"<<endl; return;}
                dataTopContainer    = (TList*) dataTopDir->FindObject(Form("Cut Number %s",dataCut[triggerSel+1].Data()));
                if(dataTopContainer == NULL) {cout << "ERROR: " << Form("Cut Number '%s'",dataCut[triggerSel+1].Data()) << " not found in File" << endl; return;}
                dataESDContainer    = (TList*) dataTopContainer->FindObject(Form("%s ESD histograms",dataCut[triggerSel+1].Data()));
                if(dataESDContainer == NULL) {cout << "ERROR: " << Form("'%s' ESD histograms",dataCut[triggerSel+1].Data()) << " not found in File" << endl; return;}
            }
            if(!strMCFile[triggerSel+1].IsNull()){
                mcESDContainer->Clear(); mcTopContainer->Clear(); mcTopDir->Clear(); mcFile->Delete();
                mcFile              = new TFile(strMCFile[triggerSel+1].Data(),"READ");
                if(mcFile->IsZombie()) {cout << "Info: ROOT file '" << strMCFile[triggerSel+1].Data() << "' could not be openend, return!" << endl; return;}
                mainDirNameMC       =  AutoDetectMainTList(mode , mcFile, "", cfSettingMC);
                mcTopDir            = (TList*) mcFile->Get(mainDirNameMC.Data());
                if(mcTopDir == NULL) {cout << "ERROR: mcTopDir not Found"<<endl; return;}
                mcTopContainer      = (TList*) mcTopDir->FindObject(Form("Cut Number %s",mcCut[triggerSel+1].Data()));
                if(mcTopContainer == NULL) {cout << "ERROR: " << Form("Cut Number '%s'",mcCut[triggerSel+1].Data()) << " not found in File" << endl; return;}
                mcESDContainer      = (TList*) mcTopContainer->FindObject(Form("%s ESD histograms",mcCut[triggerSel+1].Data()));
                if(mcESDContainer == NULL) {cout << "ERROR: " << Form("'%s' ESD histograms",mcCut[triggerSel+1].Data()) << " not found in File" << endl; return;}
            }

            dataInvsigmaPt      = (TH2F*) dataESDContainer->FindObject(nameSignalHisto.Data());
            dataBGInvsigmaPt    = (TH2F*) dataESDContainer->FindObject(nameBGHisto.Data());
            mcInvsigmaPt        = (TH2F*) mcESDContainer->FindObject(nameSignalHisto.Data());
            mcBGInvsigmaPt      = (TH2F*) mcESDContainer->FindObject(nameBGHisto.Data());

            if(!dataInvsigmaPt){
                cout << "did not find ESD_Mother_InvMass_E_Calib_PCM in data... trying ESD_Mother_Invsigma_vs_Pt_Alpha instead" << endl;
            }
            if(!dataBGInvsigmaPt){
                cout << "did not find ESD_Background_InvMass_E_Calib_PCM in data... trying ESD_Background_Invsigma_vs_Pt_Alpha instead" << endl;
            }
            if(!mcInvsigmaPt){
                cout << "did not find ESD_Mother_InvMass_E_Calib_PCM in MC... trying ESD_Mother_Invsigma_vs_Pt_Alpha instead" << endl;
            }
            if(!mcBGInvsigmaPt){
                cout << "did not find ESD_Background_InvMass_E_Calib_PCM in MC... trying ESD_Background_Invsigma_vs_Pt_Alpha instead" << endl;
            }


            triggerSel++;
            fOutput->cd();
        }

        cout << endl;
        cout << "-----------------------------------------------------" << endl;
        cout << "\t MC/Data Fitting sigma positions" << endl;
        cout << "loop: " << iClusterPt << ", " << fBinsPt[iClusterPt] << " - " << fBinsPt[iClusterPt+1] << " GeV" << endl;
        cout << "-----------------------------------------------------" << endl;

        TH2* Hist2D;
        TH2* HistBG2D;

        for(Int_t iDataMC = 0; iDataMC < 2; iDataMC++){
            if(iDataMC==0) {
                Hist2D              = dataInvsigmaPt;
                HistBG2D            = dataBGInvsigmaPt;
            } else if(iDataMC==1) {
                Hist2D              = mcInvsigmaPt;
                HistBG2D            = mcBGInvsigmaPt;
            } else {
                cout << "ERROR: data/mc loop, returning..." << endl; return;
            }
            Hist2D->Sumw2();
            HistBG2D->Sumw2();

            Double_t projectMin;
            Double_t projectMax;
            projectMin            = Hist2D->GetYaxis()->FindBin((fBinsPt[iClusterPt]*2)+0.001);
            projectMax            = Hist2D->GetYaxis()->FindBin((fBinsPt[iClusterPt+1]*2)-0.001);
            TH1D* sliceHist         = (TH1D*) Hist2D->ProjectionX(Form("slice%sAlpha_%f-%f",dataMC[iDataMC].Data(),fBinsPt[iClusterPt],fBinsPt[iClusterPt+1]),projectMin,projectMax);
            sliceHist->SetDirectory(0);
            sliceHist->SetTitle(Form("Signal %s - %.02f < #it{E}_{PCM #gamma} < %.02f (GeV)",dataMC[iDataMC].Data(),fBinsPt[iClusterPt],fBinsPt[iClusterPt+1]));
            sliceHist->GetYaxis()->SetTitle("#frac{d#it{M}_{inv}}{dN}");
            sliceHist->Sumw2();
            TH1D* sliceBGHist       = (TH1D*) HistBG2D->ProjectionX(Form("sliceBG%sAlpha_%f-%f",dataMC[iDataMC].Data(),fBinsPt[iClusterPt],fBinsPt[iClusterPt+1]),projectMin,projectMax);
            sliceBGHist->SetDirectory(0);
            sliceBGHist->SetTitle(Form("BG %s - %.02f < #it{E}_{PCM #gamma} < %.02f (GeV)",dataMC[iDataMC].Data(),fBinsPt[iClusterPt],fBinsPt[iClusterPt+1]));
            sliceBGHist->GetYaxis()->SetTitle("#frac{d#it{M}_{inv}}{dN}");
            sliceBGHist->Sumw2();

            //*******************************************************************************
            // Rebin in sigma hist
            if (ptBinsForRebin[rebinWindow+1] != -1 && rebinWindow < 10){
                cout << ptBinsForRebin[rebinWindow+1] << "\t" << fBinsPt[iClusterPt] << endl;
                if (ptBinsForRebin[rebinWindow+1] < fBinsPt[iClusterPt]){
                    rebinWindow++;
                    cout << "changed rebin factor: \t" << rebin[rebinWindow-1] << "\t -> \t" << rebin[rebinWindow] << endl;
                }
            }

            sliceHist->Rebin(rebin[rebinWindow]);
            sliceBGHist->Rebin(rebin[rebinWindow]);

            
            //***********************************  Pion  ***************************************
            // Background subtraction ranges
            Double_t minMGGBG           = 0.17;
            if (optionEnergy.Contains("PbPb") || optionEnergy.Contains("XeXe"))
                minMGGBG                = 0.22;
            if (optionEnergy.Contains("LHC17TB") )
                minMGGBG                = 0.25;
            Double_t integralSigAndBG   = sliceHist->Integral(sliceHist->FindBin(minMGGBG), sliceHist->FindBin(0.28));
            // Double_t integralBG         = sliceBGHist->Integral(sliceBGHist->FindBin(minMGGBG), sliceBGHist->FindBin(0.28));
            // cout << "BG scaling: " << integralSigAndBG << "\t" << integralBG << "\t" << integralSigAndBG/ integralBG << endl;
            TH1D* sliceHistRatio         = (TH1D*)sliceHist->Clone("sliceHistRatio");
            // TF1* fFitBckPol1 = new TF1("FitBckPol1","[0]+[1]*x");
            // if(enablePol1Scaling){
            //     sliceHistRatio->Divide(sliceHist,sliceBGHist,1,1);
            //     fFitBckPol1->SetParameter(0, integralSigAndBG/ integralBG);
            //     fFitBckPol1->SetParameter(1, 0.);
            //     sliceHistRatio->Fit(fFitBckPol1,"QRME0", "", minMGGBG, 0.28);

            //     printf("\t %f \t %f \n", fFitBckPol1->GetParameter(0), fFitBckPol1->GetParameter(1));
            //     if (integralBG != 0){
            //         sliceBGHist->Multiply( fFitBckPol1, 1.);
            //         cout << "scaled BG fith pol1" << endl;
            //     }
            // } else {
            //     if (integralBG != 0){
            //         sliceBGHist->Scale( integralSigAndBG/ integralBG );
            //         cout << "scaled BG" << endl;
            //     }
            // }

            // TH1D* sliceHistCopy         = (TH1D*)sliceHist->Clone("SliceCopy");
            // if (integralBG != 0){// && (fBinsPt[iClusterPt]<20.0)){
            //     sliceHist->Add( sliceBGHist, -1);
            // }

            TF1* fFitBckPol2 = new TF1("FitBckPol2",fline, 0., 0.35, 3);
            fFitBckPol2->SetParameter(0, integralSigAndBG);
            fFitBckPol2->SetParameter(1, 0.);
            fFitBckPol2->SetParameter(2, 0.);
            sliceHist->Fit(fFitBckPol2,"QRME0", "", 0.01, 0.3);
            fFitBckPol2->SetNpx(100000);
            TF1* fFitBckPol1 = new TF1("FitBckPol1","[0]+[1]*x+[2]*x*x", 0., 2.);
            fFitBckPol1->SetNpx(100000);
            fFitBckPol1->SetParameter(0, fFitBckPol2->GetParameter(0));
            fFitBckPol1->SetParameter(1, fFitBckPol2->GetParameter(1));
            fFitBckPol1->SetParameter(2, fFitBckPol2->GetParameter(2));

            TH1D* sliceHistCopy         = (TH1D*)sliceHist->Clone("SliceCopy");
            sliceHist->Add( fFitBckPol1, -1);

            for (Int_t i = 1; i< sliceHist->GetNbinsX()+1; i++){
                if (sliceHist->GetBinContent(i) == 0)
                    sliceHist->SetBinError(i,1.);
            }


            sliceHistCopy->GetXaxis()->SetRangeUser(0.0,0.3);
            sliceHistCopy->Write(Form("slice%sAlpha_%f-%f-withWithBG",dataMC[iDataMC].Data(),fBinsPt[iClusterPt],fBinsPt[iClusterPt+1]));
            sliceBGHist->SetLineColor(kGreen+2);
            sliceBGHist->SetMarkerColor(kGreen+2);
            sliceBGHist->GetXaxis()->SetRangeUser(0.0,0.3);
            sliceBGHist->Write(Form("slice%sAlpha_%f-%f-BG",dataMC[iDataMC].Data(),fBinsPt[iClusterPt],fBinsPt[iClusterPt+1]));
            fFitBckPol1->SetLineColor(kGreen+2);
            fFitBckPol1->Write(Form("slice%sAlpha_%f-%f-BGFit",dataMC[iDataMC].Data(),fBinsPt[iClusterPt],fBinsPt[iClusterPt+1]));
            sliceHist->SetLineColor(kRed+2);
            sliceHist->SetMarkerColor(kRed+2);
            sliceHist->GetXaxis()->SetRangeUser(0.0,0.3);
            sliceHist->Write(Form("slice%sAlpha_%f-%f-withRemainingBckg",dataMC[iDataMC].Data(),fBinsPt[iClusterPt],fBinsPt[iClusterPt+1]));
            if(enablePol1Scaling){
                sliceHistRatio->SetLineColor(kBlack+2);
                sliceHistRatio->SetMarkerColor(kBlack+2);
                sliceHistRatio->GetXaxis()->SetRangeUser(0.0,0.3);
                sliceHistRatio->Write(Form("slice%sAlpha_%f-%f-sliceHistRatio",dataMC[iDataMC].Data(),fBinsPt[iClusterPt],fBinsPt[iClusterPt+1]));
            }

            sliceHistCopy->GetYaxis()->SetRangeUser(sliceHist->GetMinimum(),sliceHistCopy->GetMaximum());
            sliceHistCopy->GetXaxis()->SetRangeUser(0.0,0.3);
            // sliceHistCopy->DrawCopy();
            // // sliceBGHist->DrawCopy("same");
            // fFitBckPol1->DrawCopy("same");
            // sliceHist->DrawCopy("same");
            // DrawGammaLines(0., 0.3, 0, 0, 1, kGray+1, 7);
            // canvas->SetLogx(0); canvas->SetLogy(0); canvas->SetLogz(0); canvas->Update();

            // for (Int_t i = 0; i < nExampleBins; i++ ){
            //     if(iClusterPt==exampleBin[i] ){
            //         canvas->SaveAs(Form("%s/ExampleBin_%sAlpha_%.02f-%.02f-withBckgAndFit.%s",outputDirSample.Data(),dataMC[iDataMC].Data(),fBinsPt[iClusterPt],fBinsPt[iClusterPt+1],suffix.Data()));
            //     }
            // }
            canvas->Clear();
            if(iDataMC==0) cout << "+++++++++++++  " << iClusterPt-ptBinRange[0]+1 << endl;
            if(iDataMC==0) padAllData1->cd(iClusterPt-ptBinRange[0]+1);
            else padAllMC1->cd(iClusterPt-ptBinRange[0]+1);
            sliceHistCopy->DrawCopy("p");
            fFitBckPol1->DrawCopy("same");
            sliceHist->DrawCopy("same");
            canvas->cd();
            sliceHist->GetXaxis()->SetRangeUser(0.0,0.3);

            // if(enablePol1Scaling){
            //     sliceHistRatio->GetYaxis()->SetRangeUser(sliceHistRatio->GetMinimum()-0.2,sliceHistRatio->GetMaximum()+0.2);
            //     sliceHistRatio->GetXaxis()->SetRangeUser(0.0,0.3);
            //     sliceHistRatio->DrawCopy();
            //     fFitBckPol1->DrawCopy("same");
            //     for (Int_t i = 0; i < nExampleBins; i++ ){
            //         if(iClusterPt==exampleBin[i] ){
            //             canvas->SaveAs(Form("%s/ExampleBin_%sAlpha_%.02f-%.02f-sliceHistRatio.%s",outputDirSample.Data(),dataMC[iDataMC].Data(),fBinsPt[iClusterPt],fBinsPt[iClusterPt+1],suffix.Data()));
            //         }
            //     }
            //     canvas->Clear();
            // }
   

            //*******************************************************************************
            // adjust min and max fitting range for inv sigma fits
            //*******************************************************************************
            Double_t minMax[2]={0.07,0.2};
            
            cout << "invsigma fit range: \t" << minMax[0] << "\t" << minMax[1] << endl;
            //*******************************************************************************
            // Fit
            //*******************************************************************************
            if ( switchPeakFunction == 0 ) {
                cout << "using FitExpPluusGaussian   " << switchPeakFunction << endl;
                fFitReco = FitExpPlusGaussian (sliceHist, minMax[0], minMax[1], 0, (fBinsPt[iClusterPt]+fBinsPt[iClusterPt+1])/2, iDataMC);
            } else if ( switchPeakFunction == 1 ) {
                cout << "using FitGaussian   " << switchPeakFunction << endl;
                fFitReco = FitGaussian (sliceHist, minMax[0], minMax[1], 0, (fBinsPt[iClusterPt]+fBinsPt[iClusterPt+1])/2, iDataMC);
            } else if ( switchPeakFunction == 2 ) {
                cout << "using FitDExpPlusGaussian   " << switchPeakFunction << endl;
                fFitReco = FitDExpPlusGaussian (sliceHist, minMax[0], minMax[1], 0, (fBinsPt[iClusterPt]+fBinsPt[iClusterPt+1])/2, iDataMC); // possibility to use double exponential fit
            } else {
                fFitReco = FitExpPlusGaussian (sliceHist, minMax[0], minMax[1], 0, (fBinsPt[iClusterPt]+fBinsPt[iClusterPt+1])/2, iDataMC);
            }

            if(iDataMC==0) {
                histDataMass->SetBinContent(iClusterPt+1,fFitReco->GetParameter(1));
                histDataMass->SetBinError(iClusterPt+1,fFitReco->GetParError(1));
                histDataAmplitude->SetBinContent(iClusterPt+1,fFitReco->GetParameter(0));
                histDataAmplitude->SetBinError(iClusterPt+1,fFitReco->GetParError(0));
                if ( switchPeakFunction != 1 ) {
                    histDataTail->SetBinContent(iClusterPt+1,fFitReco->GetParameter(3));
                    histDataTail->SetBinError(iClusterPt+1,fFitReco->GetParError(3));
                    if ( switchPeakFunction == 2 ) {
                        histDataTailRight->SetBinContent(iClusterPt+1,fFitReco->GetParameter(6));
                        histDataTailRight->SetBinError(iClusterPt+1,fFitReco->GetParError(6));
                    }
                }
                histDataResults->SetBinContent(iClusterPt+1,fFitReco->GetParameter(2));
                histDataResults->SetBinError(iClusterPt+1,fFitReco->GetParError(2));
            }
            else if(iDataMC==1) {
                histMCMass->SetBinContent(iClusterPt+1,fFitReco->GetParameter(1));
                histMCMass->SetBinError(iClusterPt+1,fFitReco->GetParError(1));
                histMCAmplitude->SetBinContent(iClusterPt+1,fFitReco->GetParameter(0));
                histMCAmplitude->SetBinError(iClusterPt+1,fFitReco->GetParError(0));
                if ( switchPeakFunction != 1 ) {
                    histMCTail->SetBinContent(iClusterPt+1,fFitReco->GetParameter(3));
                    histMCTail->SetBinError(iClusterPt+1,fFitReco->GetParError(3));
                    if ( switchPeakFunction == 2 ) {
                        histMCTailRight->SetBinContent(iClusterPt+1,fFitReco->GetParameter(6));
                        histMCTailRight->SetBinError(iClusterPt+1,fFitReco->GetParError(6));
                    }
                }
                histMCResults->SetBinContent(iClusterPt+1,fFitReco->GetParameter(2));
                histMCResults->SetBinError(iClusterPt+1,fFitReco->GetParError(2));
            }
            fFitReco->SetNpx(100000);

            sliceHist->GetListOfFunctions()->Add(fFitReco);
            sliceHist->GetXaxis()->SetRangeUser(0.0,0.3);
            sliceHist->GetYaxis()->SetRangeUser(-fFitReco->GetParameter(0)*0.1,fFitReco->GetParameter(0)*1.);
            cout << "mean: " << sliceHist->GetMean() << "\tmaximum: " << sliceHist->GetMaximum() << endl;
            sliceHist->DrawCopy();
            DrawGammaLines(0., 0.3, 0, 0, 1, kGray+1, 7);
            canvas->SetLogx(0); canvas->SetLogy(0); canvas->SetLogz(0); canvas->Update();
            for (Int_t i = 0; i < nExampleBins; i++ ){
                if(iClusterPt==exampleBin[i] ){
                    canvas->SaveAs(Form("%s/ExampleBin_%sAlpha_%.02f-%.02f.%s",outputDirSample.Data(),dataMC[iDataMC].Data(),fBinsPt[iClusterPt],fBinsPt[iClusterPt+1],suffix.Data()));
                }
            }
            canvas->Clear();
            if(iDataMC==0) padAllData2->cd(iClusterPt-ptBinRange[0]+1);
            else padAllMC2->cd(iClusterPt-ptBinRange[0]+1);
            sliceHist->DrawCopy("p");
        }
    }

         
            //***********************************  Eta   ************************************
    for(Int_t iClusterPt=ptBinRangeEta[0]; iClusterPt<ptBinRangeEta[1]; iClusterPt++){
        if(iClusterPt==firstPtBinSet[triggerSel+1]){
            // switching to trigger
            cout << endl;
            cout << "-----------------------------------------------------" << endl;
            cout << "\t Closing open files, switching to Trigger!" << endl;
            cout << "bin: " << firstPtBinSet[triggerSel] << endl;
            cout << "-----------------------------------------------------" << endl;

            if(!strDataFile[triggerSel+1].IsNull()){
                dataESDContainer->Clear(); dataTopContainer->Clear(); dataTopDir->Clear(); dataFile->Delete();
                dataFile            = new TFile(strDataFile[triggerSel+1].Data(),"READ");
                if(dataFile->IsZombie()) {cout << "Info: ROOT file '" << strDataFile[triggerSel+1].Data() << "' could not be openend, return!" << endl; return;}
                mainDirNameData     =  AutoDetectMainTList(mode , dataFile, "", cfSetting);
                dataTopDir          = (TList*) dataFile->Get(mainDirNameData.Data());
                if(dataTopDir == NULL) {cout << "ERROR: dataTopDir not Found"<<endl; return;}
                dataTopContainer    = (TList*) dataTopDir->FindObject(Form("Cut Number %s",dataCut[triggerSel+1].Data()));
                if(dataTopContainer == NULL) {cout << "ERROR: " << Form("Cut Number '%s'",dataCut[triggerSel+1].Data()) << " not found in File" << endl; return;}
                dataESDContainer    = (TList*) dataTopContainer->FindObject(Form("%s ESD histograms",dataCut[triggerSel+1].Data()));
                if(dataESDContainer == NULL) {cout << "ERROR: " << Form("'%s' ESD histograms",dataCut[triggerSel+1].Data()) << " not found in File" << endl; return;}
            }
            if(!strMCFile[triggerSel+1].IsNull()){
                mcESDContainer->Clear(); mcTopContainer->Clear(); mcTopDir->Clear(); mcFile->Delete();
                mcFile              = new TFile(strMCFile[triggerSel+1].Data(),"READ");
                if(mcFile->IsZombie()) {cout << "Info: ROOT file '" << strMCFile[triggerSel+1].Data() << "' could not be openend, return!" << endl; return;}
                mainDirNameMC       =  AutoDetectMainTList(mode , mcFile, "", cfSettingMC);
                mcTopDir            = (TList*) mcFile->Get(mainDirNameMC.Data());
                if(mcTopDir == NULL) {cout << "ERROR: mcTopDir not Found"<<endl; return;}
                mcTopContainer      = (TList*) mcTopDir->FindObject(Form("Cut Number %s",mcCut[triggerSel+1].Data()));
                if(mcTopContainer == NULL) {cout << "ERROR: " << Form("Cut Number '%s'",mcCut[triggerSel+1].Data()) << " not found in File" << endl; return;}
                mcESDContainer      = (TList*) mcTopContainer->FindObject(Form("%s ESD histograms",mcCut[triggerSel+1].Data()));
                if(mcESDContainer == NULL) {cout << "ERROR: " << Form("'%s' ESD histograms",mcCut[triggerSel+1].Data()) << " not found in File" << endl; return;}
            }

            dataInvsigmaPt      = (TH2F*) dataESDContainer->FindObject(nameSignalHisto.Data());
            dataBGInvsigmaPt    = (TH2F*) dataESDContainer->FindObject(nameBGHisto.Data());
            mcInvsigmaPt        = (TH2F*) mcESDContainer->FindObject(nameSignalHisto.Data());
            mcBGInvsigmaPt      = (TH2F*) mcESDContainer->FindObject(nameBGHisto.Data());

            if(!dataInvsigmaPt){
                cout << "did not find ESD_Mother_InvMass_E_Calib_PCM in data... trying ESD_Mother_Invsigma_vs_Pt_Alpha instead" << endl;
            }
            if(!dataBGInvsigmaPt){
                cout << "did not find ESD_Background_InvMass_E_Calib_PCM in data... trying ESD_Background_Invsigma_vs_Pt_Alpha instead" << endl;
            }
            if(!mcInvsigmaPt){
                cout << "did not find ESD_Mother_InvMass_E_Calib_PCM in MC... trying ESD_Mother_Invsigma_vs_Pt_Alpha instead" << endl;
            }
            if(!mcBGInvsigmaPt){
                cout << "did not find ESD_Background_InvMass_E_Calib_PCM in MC... trying ESD_Background_Invsigma_vs_Pt_Alpha instead" << endl;
            }


            triggerSel++;
            fOutput->cd();
        }

        cout << endl;
        cout << "-----------------------------------------------------" << endl;
        cout << "\t MC/Data Fitting sigma Eta" << endl;
        cout << "loop: " << iClusterPt << ", " << fEtaBinsPt[iClusterPt] << " - " << fEtaBinsPt[iClusterPt+1] << " GeV" << endl;
        cout << "-----------------------------------------------------" << endl;

        TH2* Hist2D;
        TH2* HistBG2D;

        for(Int_t iDataMC = 0; iDataMC < 2; iDataMC++){
            if(iDataMC==0) {
                Hist2D              = dataInvsigmaPt;
                HistBG2D            = dataBGInvsigmaPt;
            } else if(iDataMC==1) {
                Hist2D              = mcInvsigmaPt;
                HistBG2D            = mcBGInvsigmaPt;
            } else {
                cout << "ERROR: data/mc loop, returning..." << endl; return;
            }
            Hist2D->Sumw2();
            HistBG2D->Sumw2();
            Double_t projectMinEta;
            Double_t projectMaxEta;
            projectMinEta            = Hist2D->GetYaxis()->FindBin((fEtaBinsPt[iClusterPt]*2)+0.001);
            projectMaxEta            = Hist2D->GetYaxis()->FindBin((fEtaBinsPt[iClusterPt+1]*2)-0.001);
            TH1D* sliceHistEta         = (TH1D*) Hist2D->ProjectionX(Form("sliceEta%sAlpha_%f-%f",dataMC[iDataMC].Data(),fEtaBinsPt[iClusterPt],fEtaBinsPt[iClusterPt+1]),projectMinEta,projectMaxEta);
            sliceHistEta->SetDirectory(0);
            sliceHistEta->SetTitle(Form("Signal %s - %.02f < #it{E}_{PCM #gamma} < %.02f (GeV)",dataMC[iDataMC].Data(),fEtaBinsPt[iClusterPt],fEtaBinsPt[iClusterPt+1]));
            sliceHistEta->GetYaxis()->SetTitle("#frac{d#it{M}_{inv}}{dN}");
            sliceHistEta->Sumw2();
            TH1D* sliceBGHistEta       = (TH1D*) HistBG2D->ProjectionX(Form("sliceBGEta%sAlpha_%f-%f",dataMC[iDataMC].Data(),fEtaBinsPt[iClusterPt],fEtaBinsPt[iClusterPt+1]),projectMinEta,projectMaxEta);
            sliceBGHistEta->SetDirectory(0);
            sliceBGHistEta->SetTitle(Form("BG %s - %.02f < #it{E}_{PCM #gamma} < %.02f (GeV)",dataMC[iDataMC].Data(),fEtaBinsPt[iClusterPt],fEtaBinsPt[iClusterPt+1]));
            sliceBGHistEta->GetYaxis()->SetTitle("#frac{d#it{M}_{inv}}{dN}");
            sliceBGHistEta->Sumw2();

            // Rebin in sigma hist Eta
            rebinWindow=0;
            if (ptBinsForRebinEta[rebinWindow+1] != -1 && rebinWindow < 10){
                cout << ptBinsForRebinEta[rebinWindow+1] << "\t" << fEtaBinsPt[iClusterPt] << endl;
                if (ptBinsForRebinEta[rebinWindow+1] < fEtaBinsPt[iClusterPt]){
                    rebinWindow++;
                    cout << "changed rebin factor: \t" << rebinEta[rebinWindow-1] << "\t -> \t" << rebinEta[rebinWindow] << endl;
                }
            }


            sliceHistEta->Rebin(rebinEta[rebinWindow]);
            sliceBGHistEta->Rebin(rebinEta[rebinWindow]);


            // Background subtraction ranges
            Double_t minMGGBGEta           = 0.3;
            Double_t integralSigAndBGEta   = sliceHistEta->Integral(sliceHistEta->FindBin(minMGGBGEta), sliceHistEta->FindBin(1.));
            // Double_t integralBG         = sliceBGHist->Integral(sliceBGHist->FindBin(minMGGBG), sliceBGHist->FindBin(0.28));
            // cout << "BG scaling: " << integralSigAndBG << "\t" << integralBG << "\t" << integralSigAndBG/ integralBG << endl;
            TH1D* sliceHistRatioEta         = (TH1D*)sliceHistEta->Clone("sliceHistRatioEta");


            TF1* fFitBckPol2Eta = new TF1("FitBckPol2Eta",fline2, 0.3, 1., 3);
            fFitBckPol2Eta->SetParameter(0, integralSigAndBGEta);
            fFitBckPol2Eta->SetParameter(1, 0.);
            fFitBckPol2Eta->SetParameter(2, 0.);
            fFitBckPol2Eta->SetNpx(100000);
            sliceHistEta->Fit(fFitBckPol2Eta,"QRME0", "", 0.3, 1.);
            
            TF1* fFitBckPol1Eta = new TF1("FitBckPol1Eta","[0]+[1]*x+[2]*x*x", 0., 2.);
            fFitBckPol1Eta->SetNpx(100000);
            fFitBckPol1Eta->SetParameter(0, fFitBckPol2Eta->GetParameter(0));
            fFitBckPol1Eta->SetParameter(1, fFitBckPol2Eta->GetParameter(1));
            fFitBckPol1Eta->SetParameter(2, fFitBckPol2Eta->GetParameter(2));
            

            TH1D* sliceHistCopyEta         = (TH1D*)sliceHistEta->Clone("SliceCopyEta");
            sliceHistEta->Add( fFitBckPol1Eta, -1);

            for (Int_t i = 1; i< sliceHistEta->GetNbinsX()+1; i++){
                if (sliceHistEta->GetBinContent(i) == 0)
                    sliceHistEta->SetBinError(i,1.);
            }


            sliceHistCopyEta->GetXaxis()->SetRangeUser(0.3,1.);
            sliceHistCopyEta->Write(Form("sliceEta%sAlpha_%f-%f-withWithBG",dataMC[iDataMC].Data(),fEtaBinsPt[iClusterPt],fEtaBinsPt[iClusterPt+1]));
            sliceBGHistEta->SetLineColor(kGreen+2);
            sliceBGHistEta->SetMarkerColor(kGreen+2);
            sliceBGHistEta->GetXaxis()->SetRangeUser(0.3,1.);
            sliceBGHistEta->Write(Form("sliceEta%sAlpha_%f-%f-BG",dataMC[iDataMC].Data(),fEtaBinsPt[iClusterPt],fEtaBinsPt[iClusterPt+1]));
            fFitBckPol1Eta->SetLineColor(kGreen+2);
            fFitBckPol1Eta->Write(Form("sliceEta%sAlpha_%f-%f-BGFit",dataMC[iDataMC].Data(),fEtaBinsPt[iClusterPt],fEtaBinsPt[iClusterPt+1]));
            sliceHistEta->SetLineColor(kRed+2);
            sliceHistEta->SetMarkerColor(kRed+2);
            sliceHistEta->GetXaxis()->SetRangeUser(0.3,1.);
            sliceHistEta->Write(Form("sliceEta%sAlpha_%f-%f-withRemainingBckg",dataMC[iDataMC].Data(),fEtaBinsPt[iClusterPt],fEtaBinsPt[iClusterPt+1]));
            if(enablePol1Scaling){
                sliceHistRatioEta->SetLineColor(kBlack+2);
                sliceHistRatioEta->SetMarkerColor(kBlack+2);
                sliceHistRatioEta->GetXaxis()->SetRangeUser(0.3,1.);
                sliceHistRatioEta->Write(Form("sliceEta%sAlpha_%f-%f-sliceHistRatio",dataMC[iDataMC].Data(),fEtaBinsPt[iClusterPt],fEtaBinsPt[iClusterPt+1]));
            }

            // sliceHistCopyEta->DrawCopy();
            // // sliceBGHist->DrawCopy("same");
            // fFitBckPol1Eta->DrawCopy("same");
            // sliceHistEta->DrawCopy("same");
            // DrawGammaLines(0., 0.3, 0, 0, 1, kGray+1, 7);
            // canvas->SetLogx(0); canvas->SetLogy(0); canvas->SetLogz(0); canvas->Update();

            // for (Int_t i = 0; i < nExampleBins; i++ ){
            //     if(iClusterPt==exampleBin[i] ){
            //         canvas->SaveAs(Form("%s/ExampleBinEta_%sAlpha_%.02f-%.02f-withBckgAndFit.%s",outputDirSample.Data(),dataMC[iDataMC].Data(),fEtaBinsPt[iClusterPt],fEtaBinsPt[iClusterPt+1],suffix.Data()));
            //     }
            // }
            canvas->Clear();

            // if(enablePol1Scaling){
            //     sliceHistRatioEta->GetYaxis()->SetRangeUser(sliceHistRatioEta->GetMinimum()-0.2,sliceHistRatioEta->GetMaximum()+0.2);
            //     sliceHistRatioEta->GetXaxis()->SetRangeUser(0.3,1.);
            //     sliceHistRatioEta->DrawCopy();
            //     fFitBckPol1Eta->DrawCopy("same");
            //     for (Int_t i = 0; i < nExampleBins; i++ ){
            //         if(iClusterPt==exampleBin[i] ){
            //             canvas->SaveAs(Form("%s/ExampleBinEta_%sAlpha_%.02f-%.02f-sliceHistRatio.%s",outputDirSample.Data(),dataMC[iDataMC].Data(),fEtaBinsPt[iClusterPt],fEtaBinsPt[iClusterPt+1],suffix.Data()));
            //         }
            //     }
            //     canvas->Clear();
            // }
            
            Double_t minMax[2]={0.4,0.7};

            TF1* fFitRecoEta;
            if ( switchPeakFunction == 0 ) {
                cout << "using FitExpPluusGaussian   " << switchPeakFunction << endl;
                fFitRecoEta = FitExpPlusGaussian (sliceHistEta, minMax[0], minMax[1], 1, (fEtaBinsPt[iClusterPt]+fEtaBinsPt[iClusterPt+1])/2, iDataMC);
            } else if ( switchPeakFunction == 1 ) {
                cout << "using FitGaussian   " << switchPeakFunction << endl;
                fFitRecoEta = FitGaussian (sliceHistEta, minMax[0], minMax[1], 1, (fEtaBinsPt[iClusterPt]+fEtaBinsPt[iClusterPt+1])/2, iDataMC);
            } else if ( switchPeakFunction == 2 ) {
                cout << "using FitDExpPlusGaussian   " << switchPeakFunction << endl;
                fFitRecoEta = FitDExpPlusGaussian (sliceHistEta, minMax[0], minMax[1], 1, (fEtaBinsPt[iClusterPt]+fEtaBinsPt[iClusterPt+1])/2, iDataMC); // possibility to use double exponential fit
            } else {
                fFitRecoEta = FitExpPlusGaussian (sliceHistEta, minMax[0], minMax[1], 1, (fEtaBinsPt[iClusterPt]+fEtaBinsPt[iClusterPt+1])/2, iDataMC);
            }

            if(iDataMC==0) {
                histDataMassEta->SetBinContent(iClusterPt+1,fFitRecoEta->GetParameter(1));
                histDataMassEta->SetBinError(iClusterPt+1,fFitRecoEta->GetParError(1));
                histDataAmplitudeEta->SetBinContent(iClusterPt+1,fFitRecoEta->GetParameter(0));
                histDataAmplitudeEta->SetBinError(iClusterPt+1,fFitRecoEta->GetParError(0));
                histDataTailEta->SetBinContent(iClusterPt+1,fFitRecoEta->GetParameter(3));
                histDataTailEta->SetBinError(iClusterPt+1,fFitRecoEta->GetParError(3));
                histDataResultsEta->SetBinContent(iClusterPt+1,fFitRecoEta->GetParameter(2));
                histDataResultsEta->SetBinError(iClusterPt+1,fFitRecoEta->GetParError(2));
                if ( switchPeakFunction == 2 ) {
                    histDataTailRightEta->SetBinContent(iClusterPt+1,fFitRecoEta->GetParameter(6));
                    histDataTailRightEta->SetBinError(iClusterPt+1,fFitRecoEta->GetParError(6));
                }
            }
            else if(iDataMC==1) {
                histMCMassEta->SetBinContent(iClusterPt+1,fFitRecoEta->GetParameter(1));
                histMCMassEta->SetBinError(iClusterPt+1,fFitRecoEta->GetParError(1));
                histMCAmplitudeEta->SetBinContent(iClusterPt+1,fFitRecoEta->GetParameter(0));
                histMCAmplitudeEta->SetBinError(iClusterPt+1,fFitRecoEta->GetParError(0));
                histMCTailEta->SetBinContent(iClusterPt+1,fFitRecoEta->GetParameter(3));
                histMCTailEta->SetBinError(iClusterPt+1,fFitRecoEta->GetParError(3));
                if ( switchPeakFunction == 2 ) {
                    histMCTailRightEta->SetBinContent(iClusterPt+1,fFitRecoEta->GetParameter(6));
                    histMCTailRightEta->SetBinError(iClusterPt+1,fFitRecoEta->GetParError(6));                    
                }
                histMCResultsEta->SetBinContent(iClusterPt+1,fFitRecoEta->GetParameter(2));
                histMCResultsEta->SetBinError(iClusterPt+1,fFitRecoEta->GetParError(2));
            }
            fFitRecoEta->SetNpx(100000);


            if(iDataMC==0) cout << "+++++++++++++  " << iClusterPt-ptBinRangeEta[0]+1 << "  Eta" << endl;
            if(iDataMC==0) padAllData1Eta->cd(iClusterPt-ptBinRangeEta[0]+1);
            else padAllMC1Eta->cd(iClusterPt-ptBinRangeEta[0]+1);
            Double_t mesonAmplitude=0;
            for(Int_t i = sliceHistCopyEta->FindBin(minMax[0]); i < sliceHistCopyEta->FindBin(minMax[1]) ; i++ ){
                if(sliceHistCopyEta->GetBinContent(i) > mesonAmplitude) mesonAmplitude = sliceHistCopyEta->GetBinContent(i);
            }
            sliceHistCopyEta->GetYaxis()->SetRangeUser(-mesonAmplitude*0.1,mesonAmplitude);
            sliceHistCopyEta->GetXaxis()->SetRangeUser(0.3,1.);
            sliceHistCopyEta->DrawCopy("p");
            fFitBckPol1Eta->DrawCopy("same");
            sliceHistEta->DrawCopy("same");
            canvas->cd();

            sliceHistEta->GetListOfFunctions()->Add(fFitRecoEta);
            sliceHistEta->GetYaxis()->SetRangeUser(-fFitRecoEta->GetParameter(0)*0.1,fFitRecoEta->GetParameter(0)*1.);
            sliceHistEta->GetXaxis()->SetRangeUser(0.3,1.);
            for (Int_t i = 0; i < nExampleBins; i++ ){
                if(iClusterPt==exampleBin[i] ){
                    cout << "mean: " << sliceHistEta->GetMean() << "\tmaximum: " << sliceHistEta->GetMaximum() << endl;
                    canvas->SetLogx(0); canvas->SetLogy(0); canvas->SetLogz(0); canvas->Update();
                    sliceHistEta->DrawCopy();
                    DrawGammaLines(0., 0.3, 0, 0, 1, kGray+1, 7);
                    canvas->SaveAs(Form("%s/ExampleBinEta_%sAlpha_%.02f-%.02f.%s",outputDirSample.Data(),dataMC[iDataMC].Data(),fEtaBinsPt[iClusterPt],fEtaBinsPt[iClusterPt+1],suffix.Data()));
                }
            }
            canvas->Clear();
            if(iDataMC==0) padAllData2Eta->cd(iClusterPt-ptBinRangeEta[0]+1);
            else padAllMC2Eta->cd(iClusterPt-ptBinRangeEta[0]+1);
            sliceHistEta->DrawCopy("p");
            canvas->cd();
        }
    }
    delete canvas;

    canvasAllData1->Update();
    canvasAllData1->Print(Form("%s/Monitor_allbinsData_withbck_%s.%s", outputDirSampleSummary.Data(), select.Data(), suffix.Data()));
    canvasAllData2->Update();
    canvasAllData2->Print(Form("%s/Monitor_allbinsData_fits_%s.%s", outputDirSampleSummary.Data(), select.Data(), suffix.Data()));

    canvasAllMC1->Update();
    canvasAllMC1->Print(Form("%s/Monitor_allbinsMC_withbck_%s.%s", outputDirSampleSummary.Data(), select.Data(), suffix.Data()));
    canvasAllMC2->Update();
    canvasAllMC2->Print(Form("%s/Monitor_allbinsMC_fits_%s.%s", outputDirSampleSummary.Data(), select.Data(), suffix.Data()));

    canvasAllData1Eta->Update();
    canvasAllData1Eta->Print(Form("%s/Monitor_Eta_allbinsData_withbck_%s.%s", outputDirSampleSummary.Data(), select.Data(), suffix.Data()));
    canvasAllData2Eta->Update();
    canvasAllData2Eta->Print(Form("%s/Monitor_Eta_allbinsData_fits_%s.%s", outputDirSampleSummary.Data(), select.Data(), suffix.Data()));

    canvasAllMC1Eta->Update();
    canvasAllMC1Eta->Print(Form("%s/Monitor_Eta_allbinsMC_withbck_%s.%s", outputDirSampleSummary.Data(), select.Data(), suffix.Data()));
    canvasAllMC2Eta->Update();
    canvasAllMC2Eta->Print(Form("%s/Monitor_Eta_allbinsMC_fits_%s.%s", outputDirSampleSummary.Data(), select.Data(), suffix.Data()));

    cout << endl;
    cout << "-----------------------------------------------------" << endl;
    cout << "-----------------------------------------------------" << endl;
    // if (mode == 0)
    // {
        TF1* fScalingFactor          = new TF1("fScalingFactor", ScalingFactor ,0,100, 0);
        histDataMCResults         = (TH1D*)histDataResults->Clone("sliceHistRatio");
        histDataMCResults->Multiply(histDataMCResults, histDataMCResults);
        TH1D* TMPsquare = (TH1D*) histMCResults->Clone("TMPsquare");
        TMPsquare->Multiply(TMPsquare, TMPsquare);
        histDataMCResults->Add(histDataMCResults,TMPsquare,1,-1);
        histDataMCResults->Multiply(fScalingFactor,1.);
        cout << "fScalingFactor:  " << fScalingFactor->Eval(0.5) << "   " << fScalingFactor->Eval(1.) << "   " << fScalingFactor->Eval(1.5) << "   "  << endl;
        
        
        TF1* fScalingFactorEta          = new TF1("fScalingFactorEta", ScalingFactorEta ,0,100, 0);
        histDataMCResultsEta         = (TH1D*)histDataResultsEta->Clone("sliceHistRatioEta");
        histDataMCResultsEta->Multiply(histDataMCResultsEta, histDataMCResultsEta);
        TH1D* TMPsquareEta = (TH1D*) histMCResultsEta->Clone("TMPsquare");
        TMPsquareEta->Multiply(TMPsquareEta, TMPsquareEta);
        histDataMCResultsEta->Add(histDataMCResultsEta,TMPsquareEta,1,-1);
        histDataMCResultsEta->Multiply(fScalingFactorEta,1.);
    // } else {
    //     histDataMCResults->Add(histDataResults,histMCResults,1,-1);
    //     TF1* fSigmaPCM          = new TF1("fSigmaPCM", "pol1" ,rangeHighPtFitRatio[0],rangeHighPtFitRatio[1]);
    //     fSigmaPCM->SetParameters(0.00204649,0.000193091);


    //     TF1* fScalingFactor          = new TF1("fScalingFactor", ScalingFactor ,rangeHighPtFitRatio[0],rangeHighPtFitRatio[1], 0);
    //     histDataMCResults->Multiply(fScalingFactor,1.);
    //     histDataMCResults->Divide(fSigmaPCM,1.);
    //     // histDataMCResults->Multiply(histDataMCResults,histDataResults,1.,1.,"B");
    // }
    
    DrawGammaSetMarker(histDataMCResults, 24, 2, kBlack, kBlack);

    Double_t minPlotY = -0.0002;
    Double_t maxPlotY = 0.001;
    
    Double_t minsigma  = 0.;
    Double_t maxsigma  = 0.006;
    
    //*********************************************************************************************************************************
    //************************************ Write mean sigma for MC and data into output file *******************************************
    //*********************************************************************************************************************************
    SetStyleHistoTH1ForGraphs(histMCResults, "#it{E}_{PCM #gamma} (GeV)","#sigma_{m_{inv} (MC)} ",0.035,0.043, 0.035,0.043, 1.,1.);
    DrawGammaSetMarker(histMCResults, markerStyle[1], 1, color[1], color[1]);
    SetStyleHistoTH1ForGraphs(histDataResults, "#it{E}_{PCM #gamma} (GeV)","#sigma_{m_{inv} (data)} ",0.035,0.043, 0.035,0.043, 1.,1.);
    DrawGammaSetMarker(histDataResults, markerStyle[0], 1, color[0], color[0]);

    SetStyleHistoTH1ForGraphs(histMCResultsEta, "#it{E}_{PCM #gamma} (GeV)","#sigma_{#eta^{0} (MC)} ",0.035,0.043, 0.035,0.043, 1.,1.);
    DrawGammaSetMarker(histMCResultsEta, markerStyle[1], 1, color[1], color[1]);
    SetStyleHistoTH1ForGraphs(histDataResultsEta, "#it{E}_{PCM #gamma} (GeV)","#sigma_{#eta^{0} (data)} ",0.035,0.043, 0.035,0.043, 1.,1.);
    DrawGammaSetMarker(histDataResultsEta, markerStyle[0], 1, color[0], color[0]);

    //*********************************************************************************************************************************
    //*********************************** Plotting Mean sigma for data and MC vs PDG value *********************************************
    //*********************************************************************************************************************************
    cout<<"Plotting sigma for Pi0"<<endl;
    TCanvas *canvassigmaPDG = new TCanvas("canvassigmaPDG","",200,10,1350,900);  // gives the page size
    DrawGammaCanvasSettings(canvassigmaPDG, 0.08, 0.02, 0.055, 0.08);
    canvassigmaPDG->SetLogx(1);
    canvassigmaPDG->SetLogy(0);

    TH2F * histoDummySigma;
    histoDummySigma = new TH2F("histoDummySigma", "histoDummySigma", 11000, fBinsPt[ptBinRange[0]]/1.5, fBinsPt[ptBinRange[1]]*1.5, 1000, minsigma, maxsigma);
    SetStyleHistoTH2ForGraphs(histoDummySigma, "#it{E}_{PCM #gamma} (GeV)"," #sigma_{#it{E}_{PCM #gamma} (MC/data)} ", 0.035, 0.043, 0.035, 0.043, 0.82, 0.9);
    histoDummySigma->GetXaxis()->SetMoreLogLabels();
    histoDummySigma->GetXaxis()->SetLabelOffset(-0.01);
    histoDummySigma->DrawCopy("");


    TLegend *legend = GetAndSetLegend2(0.15, 0.95, 0.95, 0.99, 0.043, 2, "", 42);

    // create scaled sigma vs pt histos
    SetStyleHistoTH1ForGraphs(histDataResults, "#it{E}_{PCM #gamma} (GeV)","#sigma_{#pi^{0} (data)} ",0.035,0.043, 0.035,0.043, 1.,1.);
    if(mode == 2 || mode == 3 || mode == 14) {
      histDataResults->SetXTitle("#it{E}_{PCM #gamma} (GeV)");
      histDataResults->SetYTitle("#LT #sigma^{2}_{#pi^{0} (data)} #GT / #sigma^{2}_{#pi^{0} (PDG)}");
    }
    DrawGammaSetMarker(histDataResults, markerStyle[0], 1, color[0], color[0]);

    SetStyleHistoTH1ForGraphs(histMCResults, "#it{E}_{PCM #gamma} (GeV)","#sigma_{#pi^{0} (MC)} ",0.035,0.043, 0.035,0.043, 1.,1.);
    if(mode == 2 || mode == 3 || mode == 14) {
      histMCResults->SetXTitle("#it{E}_{PCM #gamma} (GeV)");
      histMCResults->SetYTitle("#LT #sigma^{2}_{#pi^{0} (MC)} #GT / #sigma^{2}_{#pi^{0} (PDG)}");
    }
    DrawGammaSetMarker(histMCResults, markerStyle[1], 1, color[1], color[1]);


    histDataResults->DrawCopy("same");
    legend->AddEntry(histDataResults,"Data");

    histMCResults->DrawCopy("same");
    legend->AddEntry(histMCResults,"MC");

    PutProcessLabelAndEnergyOnPlot(0.94, 0.915, 0.03, fCollisionSystem.Data(), fTextMeasurement.Data(), recGamma.Data(), 42, 0.03, "", 1, 1.25, 31);


    legend->Draw("same");
    canvassigmaPDG->Update();
    canvassigmaPDG->SaveAs(Form("%s/Sigma_Pi0_%s.%s", outputDirSampleSummary.Data(), select.Data(), suffix.Data()));
    canvassigmaPDG->Clear();

    cout<<"Plotting sigma for Eta"<<endl;

    Double_t minsigmaEta  = 0.;
    Double_t maxsigmaEta  = 0.006;


    TH2F * histoDummySigmaEta;
    histoDummySigmaEta = new TH2F("histoDummySigmaEta", "histoDummySigmaEta", 11000, fEtaBinsPt[ptBinRangeEta[0]]/1.5, fEtaBinsPt[ptBinRangeEta[1]]*1.5, 1000, minsigmaEta, maxsigmaEta);
    SetStyleHistoTH2ForGraphs(histoDummySigmaEta, "#it{E}_{PCM #gamma} (GeV)"," #sigma_{#it{E}_{PCM #gamma} (MC/data)} ", 0.035, 0.043, 0.035, 0.043, 0.82, 0.9);
    histoDummySigmaEta->GetXaxis()->SetMoreLogLabels();
    histoDummySigmaEta->GetXaxis()->SetLabelOffset(-0.01);
    histoDummySigmaEta->DrawCopy("");


    TLegend *legendEta = GetAndSetLegend2(0.15, 0.95, 0.95, 0.99, 0.043, 2, "", 42);

    // create scaled sigma vs pt histos
    SetStyleHistoTH1ForGraphs(histDataResultsEta, "#it{E}_{PCM #gamma} (GeV)","#sigma_{#eta^{0} (data)} ",0.035,0.043, 0.035,0.043, 1.,1.);
    if(mode == 2 || mode == 3 || mode == 14) {
      histDataResultsEta->SetXTitle("#it{E}_{PCM #gamma} (GeV)");
      histDataResultsEta->SetYTitle("#LT #sigma^{2}_{#eta^{0} (data)} #GT / #sigma^{2}_{#eta^{0} (PDG)}");
    }
    DrawGammaSetMarker(histDataResultsEta, markerStyle[0], 1, color[0], color[0]);

    SetStyleHistoTH1ForGraphs(histMCResultsEta, "#it{E}_{PCM #gamma} (GeV)","#sigma_{#eta^{0} (MC)} ",0.035,0.043, 0.035,0.043, 1.,1.);
    if(mode == 2 || mode == 3 || mode == 14) {
      histMCResultsEta->SetXTitle("#it{E}_{PCM #gamma} (GeV)");
      histMCResultsEta->SetYTitle("#LT #sigma^{2}_{#eta^{0} (MC)} #GT / #sigma^{2}_{#eta^{0} (PDG)}");
    }
    DrawGammaSetMarker(histMCResultsEta, markerStyle[1], 1, color[1], color[1]);


    histDataResultsEta->DrawCopy("same");
    legendEta->AddEntry(histDataResultsEta,"Data");

    histMCResultsEta->DrawCopy("same");
    legendEta->AddEntry(histMCResultsEta,"MC");

    PutProcessLabelAndEnergyOnPlot(0.94, 0.915, 0.03, fCollisionSystem.Data(), fTextMeasurement.Data(), recGamma.Data(), 42, 0.03, "", 1, 1.25, 31);


    legendEta->Draw("same");
    canvassigmaPDG->Update();
    canvassigmaPDG->SaveAs(Form("%s/Sigma_Eta_%s.%s", outputDirSampleSummary.Data(), select.Data(), suffix.Data()));
    canvassigmaPDG->Clear();

    delete canvassigmaPDG;

    Double_t textsize = 0.07;
    TCanvas *canvasmonitor = new TCanvas("canvasmonitor","",200,10,1400,1400);  // gives the page size
    TLegend *legendmonitorMass   = GetAndSetLegend2(0.4, 0.91 , 0.7, 0.98, textsize, 2, "", 42, 0.35);
    TLegend *legendmonitorSigma   = GetAndSetLegend2(0.4, 0.91 , 0.7, 0.98, textsize, 2, "", 42, 0.35);
    TLegend *legendmonitorAmplitude   = GetAndSetLegend2(0.4, 0.91 , 0.7, 0.98, textsize, 2, "", 42, 0.35);
    TLegend *legendmonitorTail   = GetAndSetLegend2(0.4, 0.91 , 0.7, 0.98, textsize, 2, "", 42, 0.35);
    TLegend *legendmonitorTailRight   = GetAndSetLegend2(0.4, 0.91 , 0.7, 0.98, textsize, 2, "", 42, 0.35);
    TPad *padmonitorMass;
    TPad *padmonitorSigma;
    TPad *padmonitorAmplitude;
    TPad *padmonitorTail;
    TPad *padmonitorTailRight;

    TCanvas *canvasmonitorEta = new TCanvas("canvasmonitorEta","",200,10,1400,1400);  // gives the page size
    TPad *padmonitorMassEta;
    TPad *padmonitorSigmaEta;
    TPad *padmonitorAmplitudeEta;
    TPad *padmonitorTailEta;
    TPad *padmonitorTailRightEta;
    DrawGammaCanvasSettings(canvasmonitorEta, 0.08, 0.02, 0.055, 0.08);
    canvasmonitorEta->cd();


    Double_t NPads = 3.;
    if ( switchPeakFunction == 0 ) NPads = 4.;
    if ( switchPeakFunction == 1 ) NPads = 3.;
    if ( switchPeakFunction == 2 ) NPads = 5.;


    padmonitorMassEta = new TPad("padmonitorMassEta","",0,2./NPads,1,3./NPads);
    padmonitorMassEta->SetTopMargin(0.1);
    padmonitorMassEta->SetBottomMargin(0.1);
    padmonitorMassEta->SetRightMargin(0.02);
    padmonitorMassEta->SetLeftMargin(0.07);
    padmonitorMassEta->SetTickx();
    padmonitorMassEta->SetTicky();
    padmonitorMassEta->cd();
    padmonitorMassEta->SetLogx(1);
    padmonitorMassEta->SetLogy(0);

    padmonitorSigmaEta = new TPad("padmonitorSigmaEta","",0,1./NPads,1,2./NPads);
    padmonitorSigmaEta->SetTopMargin(0.1);
    padmonitorSigmaEta->SetBottomMargin(0.1);
    padmonitorSigmaEta->SetRightMargin(0.02);
    padmonitorSigmaEta->SetLeftMargin(0.07);
    padmonitorSigmaEta->SetTickx();
    padmonitorSigmaEta->SetTicky();
    padmonitorSigmaEta->cd();
    padmonitorSigmaEta->SetLogx(1);
    padmonitorSigmaEta->SetLogy(0);

    if ( switchPeakFunction == 0 || switchPeakFunction == 2) {
        padmonitorTailEta = new TPad("padmonitorTailEta","",0,3./NPads,1,4./NPads);
        padmonitorTailEta->SetTopMargin(0.1);
        padmonitorTailEta->SetBottomMargin(0.1);
        padmonitorTailEta->SetRightMargin(0.02);
        padmonitorTailEta->SetLeftMargin(0.07);
        padmonitorTailEta->SetTickx();
        padmonitorTailEta->SetTicky();
        padmonitorTailEta->cd();
        padmonitorTailEta->SetLogx(1);
        padmonitorTailEta->SetLogy(0);
    }

    if ( switchPeakFunction == 2 ) {
        padmonitorTailRightEta = new TPad("padmonitorTailRightEta","",0,4./NPads,1,5./NPads);
        padmonitorTailRightEta->SetTopMargin(0.1);
        padmonitorTailRightEta->SetBottomMargin(0.1);
        padmonitorTailRightEta->SetRightMargin(0.02);
        padmonitorTailRightEta->SetLeftMargin(0.07);
        padmonitorTailRightEta->SetTickx();
        padmonitorTailRightEta->SetTicky();
        padmonitorTailRightEta->cd();
        padmonitorTailRightEta->SetLogx(1);
        padmonitorTailRightEta->SetLogy(0);
    }

    padmonitorAmplitudeEta = new TPad("padmonitorAmplitudeEta","",0,0.,1,1./NPads);
    padmonitorAmplitudeEta->SetTopMargin(0.1);
    padmonitorAmplitudeEta->SetBottomMargin(0.1);
    padmonitorAmplitudeEta->SetRightMargin(0.02);
    padmonitorAmplitudeEta->SetLeftMargin(0.07);
    padmonitorAmplitudeEta->SetTickx();
    padmonitorAmplitudeEta->SetTicky();
    padmonitorAmplitudeEta->cd();
    padmonitorAmplitudeEta->SetLogx(1);
    padmonitorAmplitudeEta->SetLogy(1);


    DrawGammaCanvasSettings(canvasmonitor, 0.08, 0.02, 0.055, 0.08);
    canvasmonitor->cd();

    padmonitorMass = new TPad("padmonitorMass","",0,2./NPads,1,3./NPads);
    padmonitorMass->SetTopMargin(0.1);
    padmonitorMass->SetBottomMargin(0.1);
    padmonitorMass->SetRightMargin(0.02);
    padmonitorMass->SetLeftMargin(0.07);
    padmonitorMass->SetTickx();
    padmonitorMass->SetTicky();
    padmonitorMass->cd();
    padmonitorMass->SetLogx(1);
    padmonitorMass->SetLogy(0);

    padmonitorSigma = new TPad("padmonitorSigma","",0,1./NPads,1,2./NPads);
    padmonitorSigma->SetTopMargin(0.1);
    padmonitorSigma->SetBottomMargin(0.1);
    padmonitorSigma->SetRightMargin(0.02);
    padmonitorSigma->SetLeftMargin(0.07);
    padmonitorSigma->SetTickx();
    padmonitorSigma->SetTicky();
    padmonitorSigma->cd();
    padmonitorSigma->SetLogx(1);
    padmonitorSigma->SetLogy(0);

    if ( switchPeakFunction == 0 || switchPeakFunction == 2) {
        padmonitorTail = new TPad("padmonitorTail","",0,3./NPads,1,4./NPads);
        padmonitorTail->SetTopMargin(0.1);
        padmonitorTail->SetBottomMargin(0.1);
        padmonitorTail->SetRightMargin(0.02);
        padmonitorTail->SetLeftMargin(0.07);
        padmonitorTail->SetTickx();
        padmonitorTail->SetTicky();
        padmonitorTail->cd();
        padmonitorTail->SetLogx(1);
        padmonitorTail->SetLogy(0);
    }
    if ( switchPeakFunction == 2 ) {
        padmonitorTailRight = new TPad("padmonitorTailRight","",0,4./NPads,1,5./NPads);
        padmonitorTailRight->SetTopMargin(0.1);
        padmonitorTailRight->SetBottomMargin(0.1);
        padmonitorTailRight->SetRightMargin(0.02);
        padmonitorTailRight->SetLeftMargin(0.07);
        padmonitorTailRight->SetTickx();
        padmonitorTailRight->SetTicky();
        padmonitorTailRight->cd();
        padmonitorTailRight->SetLogx(1);
        padmonitorTailRight->SetLogy(0);
    }

    padmonitorAmplitude = new TPad("padmonitorAmplitude","",0,0.,1,1./NPads);
    padmonitorAmplitude->SetTopMargin(0.1);
    padmonitorAmplitude->SetBottomMargin(0.1);
    padmonitorAmplitude->SetRightMargin(0.02);
    padmonitorAmplitude->SetLeftMargin(0.07);
    padmonitorAmplitude->SetTickx();
    padmonitorAmplitude->SetTicky();
    padmonitorAmplitude->cd();
    padmonitorAmplitude->SetLogx(1);
    padmonitorAmplitude->SetLogy(1);

    cout<<"Plotting Monitor sigma for Pi0"<<endl;

    padmonitorAmplitude->cd();
    legendmonitorAmplitude->AddEntry(histDataAmplitude,"Data", "pl");
    SetStyleHistoTH1ForGraphs(histDataAmplitude, "#it{E}_{PCM #gamma} (GeV)","Amplitude",textsize,textsize,textsize,textsize,0.5,0.4);
    DrawGammaSetMarker(histDataAmplitude, markerStyle[0], 2., color[0], color[0]);
    Double_t MaxAmplitude = histDataAmplitude->GetMaximum();
    if (histMCAmplitude->GetMaximum() > MaxAmplitude)
    {
        MaxAmplitude = histMCAmplitude->GetMaximum();
    }
    Double_t MinAmplitude = histDataAmplitude->GetMinimum();
    if (histMCAmplitude->GetMinimum() < MinAmplitude)
    {
        MinAmplitude = histMCAmplitude->GetMinimum();
    }
    if (MinAmplitude < 1) MinAmplitude = 1;
    histDataAmplitude->GetXaxis()->SetRangeUser(fBinsPt[ptBinRange[0]],fBinsPt[ptBinRange[1]]);
    histDataAmplitude->GetYaxis()->SetRangeUser(MinAmplitude/10, MaxAmplitude*10);
    histDataAmplitude->DrawCopy("pl");
    legendmonitorAmplitude->AddEntry(histMCAmplitude,"MC", "pl");
    SetStyleHistoTH1ForGraphs(histMCAmplitude, "#it{E}_{PCM #gamma} (GeV)","Amplitude",textsize,textsize,textsize,textsize,0.5,0.4);
    DrawGammaSetMarker(histMCAmplitude, markerStyle[1], 2., color[1], color[1]);
    histMCAmplitude->DrawCopy("pl,same");
    legendmonitorAmplitude->Draw();

    if ( switchPeakFunction == 0 || switchPeakFunction == 2) {
        padmonitorTail->cd();
        legendmonitorTail->AddEntry(histDataTail,"Data", "pl");
        SetStyleHistoTH1ForGraphs(histDataTail, "#it{E}_{PCM #gamma} (GeV)","Tail",textsize,textsize,textsize,textsize,0.5,0.4);
        DrawGammaSetMarker(histDataTail, markerStyle[0], 2., color[0], color[0]);
        histDataTail->GetXaxis()->SetRangeUser(fBinsPt[ptBinRange[0]],fBinsPt[ptBinRange[1]]);
        histDataTail->GetYaxis()->SetRangeUser(0.003, 0.01);
        histDataTail->DrawCopy("pl");
        legendmonitorTail->AddEntry(histMCTail,"MC", "pl");
        SetStyleHistoTH1ForGraphs(histMCTail, "#it{E}_{PCM #gamma} (GeV)","Tail",textsize,textsize,textsize,textsize,0.5,0.4);
        DrawGammaSetMarker(histMCTail, markerStyle[1], 2., color[1], color[1]);
        histMCTail->DrawCopy("pl,same");
        legendmonitorTail->Draw();
    }

    if ( switchPeakFunction == 2 ) {
        padmonitorTailRight->cd();
        legendmonitorTailRight->AddEntry(histDataTailRight,"Data", "pl");
        SetStyleHistoTH1ForGraphs(histDataTailRight, "#it{E}_{PCM #gamma} (GeV)","Right Tail",textsize,textsize,textsize,textsize,0.5,0.4);
        DrawGammaSetMarker(histDataTailRight, markerStyle[0], 2., color[0], color[0]);
        histDataTailRight->GetXaxis()->SetRangeUser(fBinsPt[ptBinRange[0]],fBinsPt[ptBinRange[1]]);
        histDataTailRight->GetYaxis()->SetRangeUser(0.00, 0.01);
        histDataTailRight->DrawCopy("pl");
        legendmonitorTailRight->AddEntry(histMCTailRight,"MC", "pl");
        SetStyleHistoTH1ForGraphs(histMCTailRight, "#it{E}_{PCM #gamma} (GeV)","Right Tail",textsize,textsize,textsize,textsize,0.5,0.4);
        DrawGammaSetMarker(histMCTailRight, markerStyle[1], 2., color[1], color[1]);
        histMCTailRight->DrawCopy("pl,same");
        legendmonitorTailRight->Draw();
    }

    padmonitorMass->cd();
    legendmonitorMass->AddEntry(histDataMass,"Data", "pl");
    SetStyleHistoTH1ForGraphs(histDataMass, "#it{E}_{PCM #gamma} (GeV)","Mass",textsize,textsize,textsize,textsize,0.5,0.4);
    DrawGammaSetMarker(histDataMass, markerStyle[0], 2, color[0], color[0]);
    histDataMass->GetXaxis()->SetRangeUser(fBinsPt[ptBinRange[0]],fBinsPt[ptBinRange[1]]);
    histDataMass->GetYaxis()->SetRangeUser(0.12, 0.15);
    histDataMass->DrawCopy("pl");
    legendmonitorMass->AddEntry(histMCMass,"MC", "pl");
    SetStyleHistoTH1ForGraphs(histMCMass, "#it{E}_{PCM #gamma} (GeV)","Mass",textsize,textsize,textsize,textsize,0.5,0.4);
    DrawGammaSetMarker(histMCMass, markerStyle[1], 2, color[1], color[1]);
    histMCMass->DrawCopy("pl,same");
    legendmonitorMass->Draw();

    padmonitorSigma->cd();
    legendmonitorSigma->AddEntry(histDataResults,"Data", "pl");
    SetStyleHistoTH1ForGraphs(histDataResults, "#it{E}_{PCM #gamma} (GeV)","Sigma",textsize,textsize,textsize,textsize,0.5,0.4);
    DrawGammaSetMarker(histDataResults, markerStyle[0], 2, color[0], color[0]);
    histDataResults->GetXaxis()->SetRangeUser(fBinsPt[ptBinRange[0]],fBinsPt[ptBinRange[1]]);
    histDataResults->GetYaxis()->SetRangeUser(0., 0.01);
    histDataResults->DrawCopy("pl");
    legendmonitorSigma->AddEntry(histMCResults,"MC", "pl");
    SetStyleHistoTH1ForGraphs(histMCResults, "#it{E}_{PCM #gamma} (GeV)","Sigma",textsize,textsize,textsize,textsize,0.5,0.4);
    DrawGammaSetMarker(histMCResults, markerStyle[1], 2, color[1], color[1]);
    histMCResults->DrawCopy("pl,same");
    legendmonitorSigma->Draw();

    canvasmonitor->cd();
    padmonitorMass->Draw();
    padmonitorSigma->Draw();
    if ( switchPeakFunction == 0 || switchPeakFunction == 2) padmonitorTail->Draw();
    if ( switchPeakFunction == 2 ) padmonitorTailRight->Draw();
    padmonitorAmplitude->Draw();

    canvasmonitor->Update();
    canvasmonitor->SaveAs(Form("%s/Monitor_%s.%s", outputDirSampleSummary.Data(), select.Data(), suffix.Data()));
    canvasmonitor->Clear();

//  #############################

    cout<<"Plotting Monitor sigma for Eta"<<endl;

    canvasmonitorEta->cd();

    padmonitorAmplitudeEta->cd();
    MaxAmplitude = histDataAmplitudeEta->GetMaximum();
    if (histMCAmplitudeEta->GetMaximum() > MaxAmplitude)
    {
        MaxAmplitude = histMCAmplitudeEta->GetMaximum();
    }
    MinAmplitude = histDataAmplitudeEta->GetMinimum();
    if (histMCAmplitudeEta->GetMinimum() < MinAmplitude)
    {
        MinAmplitude = histMCAmplitudeEta->GetMinimum();
    }
    if (MinAmplitude < 1) MinAmplitude = 1;
    histDataAmplitudeEta->GetYaxis()->SetRangeUser(MinAmplitude/10, MaxAmplitude*10);
    SetStyleHistoTH1ForGraphs(histDataAmplitudeEta, "#it{E}_{PCM #gamma} (GeV)","Amplitude",textsize,textsize,textsize,textsize,0.5,0.4);
    DrawGammaSetMarker(histDataAmplitudeEta, markerStyle[0], 2., color[0], color[0]);
    SetStyleHistoTH1ForGraphs(histMCAmplitudeEta, "#it{E}_{PCM #gamma} (GeV)","Amplitude",textsize,textsize,textsize,textsize,0.5,0.4);
    DrawGammaSetMarker(histMCAmplitudeEta, markerStyle[1], 2., color[1], color[1]);
    histDataAmplitudeEta->DrawCopy("pl");
    histMCAmplitudeEta->DrawCopy("pl,same");
    legendmonitorAmplitude->Draw();

    if ( switchPeakFunction == 0 || switchPeakFunction == 2) {
        padmonitorTailEta->cd();
        // legendmonitorTail->AddEntry(histDataTailEta,"Data", "pl");
        SetStyleHistoTH1ForGraphs(histDataTailEta, "#it{E}_{PCM #gamma} (GeV)","Tail",textsize,textsize,textsize,textsize,0.5,0.4);
        DrawGammaSetMarker(histDataTailEta, markerStyle[0], 2., color[0], color[0]);
        histDataTailEta->GetYaxis()->SetRangeUser(0.00, 0.05);
        histDataTailEta->DrawCopy("pl");
        // legendmonitorTail->AddEntry(histMCTailEta,"MC", "pl");
        SetStyleHistoTH1ForGraphs(histMCTailEta, "#it{E}_{PCM #gamma} (GeV)","Tail",textsize,textsize,textsize,textsize,0.5,0.4);
        DrawGammaSetMarker(histMCTailEta, markerStyle[1], 2., color[1], color[1]);
        histMCTailEta->DrawCopy("pl,same");
        legendmonitorTail->Draw();
    }


    if ( switchPeakFunction == 2 ) {
        padmonitorTailRightEta->cd();
        SetStyleHistoTH1ForGraphs(histDataTailRightEta, "#it{E}_{PCM #gamma} (GeV)","Right Tail",textsize,textsize,textsize,textsize,0.5,0.4);
        DrawGammaSetMarker(histDataTailRightEta, markerStyle[0], 2., color[0], color[0]);
        histDataTailRightEta->GetXaxis()->SetRangeUser(fEtaBinsPt[ptBinRangeEta[0]],fEtaBinsPt[ptBinRangeEta[1]]);
        histDataTailRightEta->GetYaxis()->SetRangeUser(0.00, 0.01);
        histDataTailRightEta->DrawCopy("pl");
        SetStyleHistoTH1ForGraphs(histMCTailRightEta, "#it{E}_{PCM #gamma} (GeV)","Right Tail",textsize,textsize,textsize,textsize,0.5,0.4);
        DrawGammaSetMarker(histMCTailRightEta, markerStyle[1], 2., color[1], color[1]);
        histMCTailRightEta->DrawCopy("pl,same");
        legendmonitorTailRight->Draw();
    }

    padmonitorMassEta->cd();
    // legendmonitorMass->AddEntry(histDataMassEta,"Data", "pl");
    SetStyleHistoTH1ForGraphs(histDataMassEta, "#it{E}_{PCM #gamma} (GeV)","Mass",textsize,textsize,textsize,textsize,0.5,0.4);
    DrawGammaSetMarker(histDataMassEta, markerStyle[0], 2, color[0], color[0]);
    histDataMassEta->GetYaxis()->SetRangeUser(0.5, 0.6);
    histDataMassEta->DrawCopy("pl");
    // legendmonitorMass->AddEntry(histMCMassEta,"MC", "pl");
    SetStyleHistoTH1ForGraphs(histMCMassEta, "#it{E}_{PCM #gamma} (GeV)","Mass",textsize,textsize,textsize,textsize,0.5,0.4);
    DrawGammaSetMarker(histMCMassEta, markerStyle[1], 2, color[1], color[1]);
    histMCMassEta->DrawCopy("pl,same");
    legendmonitorMass->Draw();

    padmonitorSigmaEta->cd();
    // legendmonitorSigma->AddEntry(histDataResultsEta,"Data", "pl");
    SetStyleHistoTH1ForGraphs(histDataResultsEta, "#it{E}_{PCM #gamma} (GeV)","Sigma",textsize,textsize,textsize,textsize,0.5,0.4);
    DrawGammaSetMarker(histDataResultsEta, markerStyle[0], 2, color[0], color[0]);
    histDataResultsEta->GetYaxis()->SetRangeUser(0., 0.01);
    histDataResultsEta->DrawCopy("pl");
    // legendmonitorSigma->AddEntry(histMCResultsEta,"MC", "pl");
    SetStyleHistoTH1ForGraphs(histMCResultsEta, "#it{E}_{PCM #gamma} (GeV)","Sigma",textsize,textsize,textsize,textsize,0.5,0.4);
    DrawGammaSetMarker(histMCResultsEta, markerStyle[1], 2, color[1], color[1]);
    histMCResultsEta->DrawCopy("pl,same");
    legendmonitorSigma->Draw();

    canvasmonitor->cd();
    canvasmonitor->Clear();
    padmonitorAmplitudeEta->Draw();
    padmonitorMassEta->Draw();
    padmonitorSigmaEta->Draw();
    if ( switchPeakFunction == 0 || switchPeakFunction == 2) padmonitorTailEta->Draw();
    if ( switchPeakFunction == 2 ) padmonitorTailRightEta->Draw();

    canvasmonitor->Update();
    canvasmonitor->SaveAs(Form("%s/Monitor_Eta_%s.%s", outputDirSampleSummary.Data(), select.Data(), suffix.Data()));
    canvasmonitor->Clear();

    // ################

    delete canvasmonitor;

    //*********************************************************************************************************************************
    //****************************** Fitting ratio of mean sigma position in MC/data ***************************************************
    //*********************************************************************************************************************************
    Double_t highPtConst            = 0.;

    // creating real fit functions
    fFitsigma = FitDataMC(histDataMCResults, fBinsPt[ptBinRange[0]], fBinsPt[ptBinRange[1]], select,  highPtConst, mode);
    fFitsigma2 = FitDataMC(histDataMCResults, fBinsPt[ptBinRange[0]], fBinsPt[ptBinRange[1]], select,  highPtConst, mode,1);
    fFitsigma3 = FitDataMC(histDataMCResults, fBinsPt[ptBinRange[0]], fBinsPt[ptBinRange[1]], select,  highPtConst, mode,2);
    fFitsigma4 = FitDataMC(histDataMCResults, fBinsPt[ptBinRange[0]], fBinsPt[ptBinRange[1]], select,  highPtConst, mode,4);
    fFitsigma5 = FitDataMC(histDataMCResults, fBinsPt[ptBinRange[0]], fBinsPt[ptBinRange[1]], select,  highPtConst, mode,5);




    fstream fLog;
    fLog.open(Form("%s/CorrectPCMSmearing_%s.log",outputDirSampleSummary.Data(),select.Data()), ios::out);
    fLog << "-----------------------------------" << endl;
    fLog << "Pol0 fitted, results:" << endl;
    fLog << "-----------------------------------" << endl;
    fLog << "Par " << 0 << ": " << fFitsigma->GetParameter(0) << " +- " << fFitsigma->GetParError(0) << endl;
    fLog << "-----------------------------------" << endl;
    fLog << "Pol2 fitted, results:" << endl;
    fLog << "-----------------------------------" << endl;
    for(Int_t i=0;i<=2;i++) fLog << "Par " << i << ": " << fFitsigma2->GetParameter(i) << " +- " << fFitsigma2->GetParError(i) << endl;
    fLog << "-----------------------------------" << endl;
    fLog << "Pol1 fitted, results:" << endl;
    fLog << "-----------------------------------" << endl;
    for(Int_t i=0;i<=1;i++) fLog << "Par " << i << ": " << fFitsigma3->GetParameter(i) << " +- " << fFitsigma3->GetParError(i) << endl;
    fLog << "-----------------------------------" << endl;
    fLog << "exp fitted, results:" << endl;
    fLog << "-----------------------------------" << endl;
    for(Int_t i=0;i<=4;i++) fLog << "Par " << i << ": " << fFitsigma4->GetParameter(i) << " +- " << fFitsigma4->GetParError(i) << endl;
    fLog << "-----------------------------------" << endl;
    fLog << "pow fitted, results:" << endl;
    fLog << "-----------------------------------" << endl;
    for(Int_t i=0;i<=2;i++) fLog << "Par " << i << ": " << fFitsigma5->GetParameter(i) << " +- " << fFitsigma5->GetParError(i) << endl;
    cout << "-----------------------------------" << endl;
    cout << "Pol0 fitted, results:" << endl;
    cout << "-----------------------------------" << endl;
    cout << "Par " << 0 << ": " << fFitsigma->GetParameter(0) << " +- " << fFitsigma->GetParError(0) << endl;
    cout << "-----------------------------------" << endl;
    cout << "Pol2 fitted, results:" << endl;
    cout << "-----------------------------------" << endl;
    for(Int_t i=0;i<=1;i++) cout << "Par " << i << ": " << fFitsigma2->GetParameter(i) << " +- " << fFitsigma2->GetParError(i) << endl;
    cout << "-----------------------------------" << endl;
    cout << "Pol1 fitted, results:" << endl;
    cout << "-----------------------------------" << endl;
    for(Int_t i=0;i<=1;i++) cout << "Par " << i << ": " << fFitsigma3->GetParameter(i) << " +- " << fFitsigma3->GetParError(i) << endl;
    cout << "-----------------------------------" << endl;
    cout << "exp fitted, results:" << endl;
    cout << "-----------------------------------" << endl;
    for(Int_t i=0;i<=4;i++) cout << "Par " << i << ": " << fFitsigma4->GetParameter(i) << " +- " << fFitsigma4->GetParError(i) << endl;
    cout << "pow fitted, results:" << endl;
    cout << "-----------------------------------" << endl;
    for(Int_t i=0;i<=2;i++) cout << "Par " << i << ": " << fFitsigma5->GetParameter(i) << " +- " << fFitsigma5->GetParError(i) << endl;
    cout << "-----------------------------------" << endl;
    fLog.close();

    //*******************************************************************************
    // plotting sigma ratios
    //*******************************************************************************
    TCanvas *canvassigmaRatioMCData = new TCanvas("canvassigmaRatioMCData","",200,10,1350,900);  // gives the page size
    DrawGammaCanvasSettings(canvassigmaRatioMCData, 0.1, 0.012, 0.035, 0.1);
    canvassigmaRatioMCData->SetLogx(1);
    canvassigmaRatioMCData->SetLogy(0);

    

    TH2F * histoDummyDataMCRatio;
    histoDummyDataMCRatio = new TH2F("histoDummyDataMCRatio","histoDummyDataMCRatio", 11000, fBinsPt[ptBinRange[0]]/1.5, fBinsPt[ptBinRange[1]]*1.5, 1000, minPlotY, maxPlotY);
    SetStyleHistoTH2ForGraphs(histoDummyDataMCRatio, "#it{E}_{PCM #gamma} (GeV)"," #sigma_{smearing E_{#gamma PCM}}^{2} ", 0.035, 0.043, 0.035, 0.043, 0.82, 0.9);
    // SetStyleHistoTH2ForGraphs(histoDummyDataMCRatio, "#it{E}_{PCM #gamma} (GeV)"," #Delta #sigma_{#pi^{0} (data-MC)} * 4 * #sqrt{2} / (1-cos(#Theta)) #frac{ #sigma_{m_{inv}PCMEDC} }{ #sigma_{m_{inv}PCM} } ", 0.035, 0.043, 0.035, 0.043, 0.82, 0.9);
    histoDummyDataMCRatio->GetXaxis()->SetMoreLogLabels();
    histoDummyDataMCRatio->GetXaxis()->SetLabelOffset(-0.01);
    histoDummyDataMCRatio->DrawCopy("");

    DrawGammaSetMarker(histDataMCResults, 20, 1, color[0], color[0]);
    DrawGammaSetMarker(histDataMCResultsEta, 20, 1, color[1], color[1]);
//     histDataMCResults->Draw("same");


    fFitsigma->SetRange(fBinsPt[ptBinRange[0]]/1.5, fBinsPt[ptBinRange[1]]*1.5);
    fFitsigma2->SetRange(fBinsPt[ptBinRange[0]]/1.5, fBinsPt[ptBinRange[1]]*1.5);
    fFitsigma3->SetRange(fBinsPt[ptBinRange[0]]/1.5, fBinsPt[ptBinRange[1]]*1.5);
    fFitsigma4->SetRange(fBinsPt[ptBinRange[0]]/1.5, fBinsPt[ptBinRange[1]]*1.5);
    fFitsigma5->SetRange(fBinsPt[ptBinRange[0]]/1.5, fBinsPt[ptBinRange[1]]*1.5);

    DrawGammaSetMarkerTF1( fFitsigma, 1, 2, kRed+1);
    DrawGammaSetMarkerTF1( fFitsigma2, 7, 2, kGreen+2);
    DrawGammaSetMarkerTF1( fFitsigma3, 8, 2, kBlue+2);
    DrawGammaSetMarkerTF1( fFitsigma4, 9, 2, kCyan+2);
    DrawGammaSetMarkerTF1( fFitsigma5, 9, 2, kYellow+2);

    fFitsigma->Draw("same");
    fFitsigma2->Draw("same");
    fFitsigma3->Draw("same");
    fFitsigma4->Draw("same");
    fFitsigma5->Draw("same");
    histDataMCResults->Draw("same,pe");
    histDataMCResultsEta->Draw("same,pe");

    PutProcessLabelAndEnergyOnPlot(0.94, 0.96, 0.03, fCollisionSystem.Data(), fTextMeasurement.Data(), recGamma.Data(), 42, 0.03, "", 1, 1.25, 31);
    // for (Int_t i = 0; i < nSets; i++){
    //    PutProcessLabelAndEnergyOnPlot(0.12, 0.945-2*0.03*(i), 0.03, fPlotLabelsRatio[i].Data(),"", "", 42, 0.03, "", 1, 1.25, 11);
    // }

    TLegend* legendCorrectionFunctions = GetAndSetLegend2(0.125,0.8-3*1.1*0.03, 0.4,0.8, 0.03, 1, "", 42, 0.15);
    legendCorrectionFunctions->AddEntry(fFitsigma,"constant","l");
    legendCorrectionFunctions->AddEntry(fFitsigma2,"pol2 ","l");
    legendCorrectionFunctions->AddEntry(fFitsigma3,"pol1 ","l");
    legendCorrectionFunctions->AddEntry(fFitsigma4,"exp ","l");
    legendCorrectionFunctions->AddEntry(fFitsigma5,"pow ","l");
    legendCorrectionFunctions->Draw();

    DrawGammaLines(fBinsPt[ptBinRange[0]]/1.5, fBinsPt[ptBinRange[1]]*1.5, 1.0, 1.0, 1, kGray+2, 2);

    canvassigmaRatioMCData->Update();
    canvassigmaRatioMCData->SaveAs(Form("%s/SigmaDiff_%s.%s", outputDirSampleSummary.Data(), select.Data(), suffix.Data()));
    canvassigmaRatioMCData->Clear();

    delete canvassigmaRatioMCData;

    cout << "-----------------------------------------------------" << endl;
    cout << "-----------------------------------------------------" << endl;

    histDataResults->Write("Mean sigma Data");
    histMCResults->Write("Mean sigma MC");
    // histDataResults->Write();
    // histMCResults->Write();
    histDataMCResults->Write("MeansigmaRatioMCData");
    histDataMCResultsEta->Write("MeansigmaRatioMCDataEta");

    fOutput->Write();
    fOutput->Close();
    return;
}

//*******************************************************************************
//****************** functional form for SDM correction *************************
//*******************************************************************************

Float_t FunctionNL_kSDM(Float_t e, Float_t p0, Float_t p1, Float_t p2){
    return ( p0 + TMath::Exp( p1 + ( p2 * e ) ) );
}

//*******************************************************************************
//************** define exclusion function for BG fitting ***********************
//*******************************************************************************
Double_t fitExcludeSignal(Double_t *x, Double_t *par)
{
    if (x[0] > 0.08 && x[0] < 0.17) {
      TF1::RejectPoint();
      return 0;
   }
   return par[0] + par[1]*x[0]; //+ par[2]*x[0]*x[0]
}

//*******************************************************************************
//************** fit BG only ****************************************************
//*******************************************************************************
TF1* FitBckg(TH1* fHisto, Double_t minFit, Double_t maxFit){
    TF1* fFitBckg = new TF1("fFitBckg",fitExcludeSignal,minFit,maxFit,3);
    fFitBckg->SetLineColor(kBlue);
    fFitBckg->SetLineWidth(2);
    fFitBckg->SetLineStyle(1);
    fHisto->Fit(fFitBckg,"QRME0");
    return fFitBckg;
}

//*******************************************************************************
//************** fit ratio of data and MC sigma positions ************************
//*******************************************************************************
TF1* FitDataMC(TH1* fHisto, Double_t minFit, Double_t maxFit, TString selection, Double_t constPar, Int_t mode, Int_t fitmode){

    cout << "running standard fit from " <<  minFit << "\t"<<  maxFit << endl;
    TF1* fFitReco;
    if (fitmode == 0) {
        fFitReco = new TF1("DataMC", "[0]" ,minFit,maxFit);
        fFitReco->SetParameter(0, 0.0001);
    } else if (fitmode == 1) {
        fFitReco = new TF1("DataMC", "[0]+[1]*x*x" ,minFit,maxFit);
        fFitReco->SetParameter(0, 0.03*0.03);
        fFitReco->SetParameter(1, 0.025*0.025);
    } else if (fitmode == 2) {
        fFitReco = new TF1("DataMC", "[0]+[1]*x" ,minFit,maxFit);
        fFitReco->SetParameter(0, 0.0001);
        fFitReco->SetParameter(1, 0.000222327);
    } else if (fitmode == 3) {
        // TF1* fFitRecotmp = new TF1("DataMCtmp", "[0]" ,minFit,maxFit);
        // fFitRecotmp->SetParameter(0, 0.0001);
        // fHisto->Fit(fFitRecotmp,"QRME0");
        fFitReco = new TF1("DataMC", "[0]+[1]*TMath::Exp([2]*(x+[3])+[4])" ,minFit,maxFit);
        // fFitReco->FixParameter(0, fFitRecotmp->GetParameter(0));
        // fFitReco->SetParameter(0,fFitRecotmp->GetParameter(0));
        fFitReco->SetParameter(0,0.00001);
        // fFitReco->SetParLimits(0,fFitRecotmp->GetParameter(0)*0.9,fFitRecotmp->GetParameter(0)*1.1);
        fFitReco->SetParameter(1,0.000000001);
        fFitReco->SetParLimits(1,0.0000000001,10000000.);
        fFitReco->SetParameter(2,0.0001);
        // fFitReco->SetParLimits(2,0.,10000000.);
        fFitReco->SetParameter(3,0.000);
        fFitReco->SetParameter(4,0.000);        
    } else {
        fFitReco = new TF1("DataMC", "[0]+[1]*TMath::Power(x,[2])" ,minFit,maxFit);
        fFitReco->SetParameter(0,0.00001);
        fFitReco->SetParameter(1,0.000000001);
        fFitReco->SetParameter(2,0.0001);
    }


    fHisto->Fit(fFitReco,"QRME0");

    fFitReco->SetLineColor(kRed);
    fFitReco->SetLineWidth(2);
    fFitReco->SetLineStyle(1);

    if(TString(gMinuit->fCstatu.Data()).Contains("CONVERGED") == 1 || TString(gMinuit->fCstatu.Data()).Contains("SUCCESSFUL") == 1 || TString(gMinuit->fCstatu.Data()).Contains("PROBLEMS") == 1){
        cout << "DataMC fitting successfull with status " << gMinuit->fCstatu.Data() <<endl << endl;
    } else {
        cout << "DataMC fitting failed with status " << gMinuit->fCstatu.Data() <<endl << endl;
    }

    return fFitReco;
}

//*******************************************************************************
//************** Definition of fitting with pure Gaussian ***********************
//*******************************************************************************
TF1* FitGaussian(TH1D* histo, Double_t fitRangeMin, Double_t fitRangeMax, Bool_t modeEta, Double_t ptcenter , Int_t iDataMC){

    Double_t mesonAmplitude = 0;
    for(Int_t i = histo->FindBin(fitRangeMin); i < histo->FindBin(fitRangeMax) ; i++ ){
      if(histo->GetBinContent(i) > mesonAmplitude) mesonAmplitude = histo->GetBinContent(i);
    }
    cout << "mesonAmplitude = " << mesonAmplitude << endl;
    Double_t mesonAmplitudeMin;
    Double_t mesonAmplitudeMax;

    mesonAmplitudeMin = mesonAmplitude*80./100.;

    mesonAmplitudeMax = mesonAmplitude*110./100.;

    cout << "mesonAmplitudeMin = " << mesonAmplitudeMin << endl;
    cout << "mesonAmplitudeMax = " << mesonAmplitudeMax << endl;

    Double_t massEta            = 0.547862;

    // necessary to exclude low sigma peak
    // histo->GetXaxis()->SetRangeUser(fitRangeMin, fitRangeMax);

    // TF1* fFitReco    = new TF1("fGaussExp","(x<[1])*([0]*(TMath::Exp(-0.5*((x-[1])/[2])^2)+TMath::Exp((x-[1])/[3])*(1.-TMath::Exp(-0.5*((x-[1])/[2])^2)))+[4]+[5]*x)+(x>=[1])*([0]*TMath::Exp(-0.5*((x-[1])/[2])^2)+[4]+[5]*x)",
    TF1* fFitReco    = new TF1("fGauss","[0]*TMath::Exp(-0.5*((x-[1])/[2])^2)+[3]+[4]*x", fitRangeMin, fitRangeMax);
    Double_t fMesonmassExpect;
    if (modeEta){
        fMesonmassExpect = 0.547862;
    } else {
        fMesonmassExpect = TDatabasePDG::Instance()->GetParticle(111)->Mass();
    }

    fFitReco->SetParameter(0, mesonAmplitude);
    fFitReco->SetParameter(1, fMesonmassExpect);
    fFitReco->SetParameter(3, 0.);
    fFitReco->SetParameter(4, 0.);

    fFitReco->SetParLimits(0, mesonAmplitudeMin, mesonAmplitudeMax);
    fFitReco->SetParLimits(1, fMesonmassExpect*0.95, fMesonmassExpect*1.05);

    if (modeEta){
        fFitReco->SetParameter(2, 0.005);
        fFitReco->SetParLimits(2, 0.002, 0.015);
    } else {
        fFitReco->SetParameter(2, 0.002);
        fFitReco->SetParLimits(2, 0.0005, 0.005);
    }


    histo->Fit(fFitReco,"QRME0");
    histo->Fit(fFitReco,"QRME0");
    // Double_t rmsScalingRefit = 1.1;
    // if(ptcenter > 3.0) rmsScalingRefit = 1.3;
    // if(ptcenter > 5.0) rmsScalingRefit = 1.6;
    // fFitReco->SetRange(fFitReco->GetParameter(1)-rmsScalingRefit*fFitReco->GetParameter(2),fFitReco->GetParameter(1)+rmsScalingRefit*fFitReco->GetParameter(2));
    // histo->Fit(fFitReco,"QRME0","",fFitReco->GetParameter(1)-rmsScalingRefit*fFitReco->GetParameter(2),fFitReco->GetParameter(1)+rmsScalingRefit*fFitReco->GetParameter(2));

    fFitReco->SetLineColor(kBlue+1);
    fFitReco->SetLineWidth(1);
    fFitReco->SetLineStyle(1);

    if(TString(gMinuit->fCstatu.Data()).Contains("CONVERGED") == 1 || TString(gMinuit->fCstatu.Data()).Contains("SUCCESSFUL") == 1 || TString(gMinuit->fCstatu.Data()).Contains("PROBLEMS") == 1){
        cout << "Parameter for exponential+Gaussian "<< endl;
        cout << gMinuit->fCstatu.Data() << endl;
        cout << "Gausexp: \t" << fFitReco->GetParameter(0) <<"+-" << fFitReco->GetParError(0) << "\t " << fFitReco->GetParameter(1)<<"+-" << fFitReco->GetParError(1) << "\t "<< fFitReco->GetParameter(2) <<"+-" << fFitReco->GetParError(2)<<endl;
        cout << "chi2/ndf:" << fFitReco->GetChisquare()/fFitReco->GetNDF() << endl;
    } else {
        cout << "Exp+Gaussian fitting failed in with status " << gMinuit->fCstatu.Data() <<endl << endl;
    }
    return fFitReco;
}
//*******************************************************************************
//************** Definition of fitting with pure Gaussian ***********************
//*******************************************************************************
TF1* FitExpPlusGaussian(TH1D* histo, Double_t fitRangeMin, Double_t fitRangeMax, Bool_t modeEta, Double_t ptcenter , Int_t iDataMC){

    Double_t mesonAmplitude = 0;
    for(Int_t i = histo->FindBin(fitRangeMin); i < histo->FindBin(fitRangeMax) ; i++ ){
      if(histo->GetBinContent(i) > mesonAmplitude) mesonAmplitude = histo->GetBinContent(i);
    }
    cout << "mesonAmplitude = " << mesonAmplitude << endl;
    Double_t mesonAmplitudeMin;
    Double_t mesonAmplitudeMax;

    // special setting for PCM-EMC
    
    mesonAmplitudeMin = mesonAmplitude*50./100.;    
    mesonAmplitudeMax = mesonAmplitude*110./100.;

    cout << "mesonAmplitudeMin = " << mesonAmplitudeMin << endl;
    cout << "mesonAmplitudeMax = " << mesonAmplitudeMax << endl;

    TF1* fFitReco    = new TF1("fGaussExp","(x<[1])*([0]*(TMath::Exp(-0.5*((x-[1])/[2])^2)+TMath::Exp((x-[1])/[3])*(1.-TMath::Exp(-0.5*((x-[1])/[2])^2)))+[4]+[5]*x)+(x>=[1])*([0]*TMath::Exp(-0.5*((x-[1])/[2])^2)+[4]+[5]*x)",
                               fitRangeMin, fitRangeMax);
    Double_t fMesonmassExpect;
    if (modeEta){
        fMesonmassExpect = 0.547862;
    } else {
        fMesonmassExpect = TDatabasePDG::Instance()->GetParticle(111)->Mass();
    }

    fFitReco->SetParameter(0, mesonAmplitude);
    fFitReco->SetParameter(1, fMesonmassExpect);

    fFitReco->SetParLimits(0, mesonAmplitudeMin, mesonAmplitudeMax);
    fFitReco->SetParLimits(1, fMesonmassExpect*0.9, fMesonmassExpect*1.1);

    if (modeEta){
        fFitReco->SetParameter(2, 0.005);
        fFitReco->SetParLimits(2, 0.002, 0.015);
        fFitReco->SetParameter(3, 0.001);
        fFitReco->SetParLimits(3, 0.002, 0.05);
    } else {
        fFitReco->SetParameter(2, 0.002);
        fFitReco->SetParLimits(2, 0.0005, 0.005);
        fFitReco->SetParameter(3, 0.012);
        fFitReco->SetParLimits(3, 0.001, 0.09);
    }

    histo->Fit(fFitReco,"QRME0");
    histo->Fit(fFitReco,"QRME0");

    fFitReco->SetLineColor(kBlue+1);
    fFitReco->SetLineWidth(1);
    fFitReco->SetLineStyle(1);

    if(TString(gMinuit->fCstatu.Data()).Contains("CONVERGED") == 1 || TString(gMinuit->fCstatu.Data()).Contains("SUCCESSFUL") == 1 || TString(gMinuit->fCstatu.Data()).Contains("PROBLEMS") == 1){
        cout << "Parameter for exponential+Gaussian "<< endl;
        cout << gMinuit->fCstatu.Data() << endl;
        cout << "Gausexp: \t" << fFitReco->GetParameter(0) <<"+-" << fFitReco->GetParError(0) << "\t " << fFitReco->GetParameter(1)<<"+-" << fFitReco->GetParError(1) << "\t "<< fFitReco->GetParameter(2) <<"+-" << fFitReco->GetParError(2)<<endl;
        cout << "chi2/ndf:" << fFitReco->GetChisquare()/fFitReco->GetNDF() << endl;
    } else {
        cout << "Exp+Gaussian fitting failed in with status " << gMinuit->fCstatu.Data() <<endl << endl;
    }
    return fFitReco;
}

//*******************************************************************************
//*********** Definition of fitting with double exp Gaussian ********************
//*******************************************************************************
TF1* FitDExpPlusGaussian(TH1D* histo, Double_t fitRangeMin, Double_t fitRangeMax, Bool_t modeEta, Double_t ptcenter, Int_t iDataMC ){


    Double_t mesonAmplitude = 0;
    for(Int_t i = histo->FindBin(0.07); i < histo->FindBin(0.2) ; i++ ){
      if(histo->GetBinContent(i) > mesonAmplitude) mesonAmplitude = histo->GetBinContent(i);
    }
    cout << "mesonAmplitude = " << mesonAmplitude << endl;
    Double_t mesonAmplitudeMin;
    Double_t mesonAmplitudeMax;
    mesonAmplitudeMin = mesonAmplitude*50./100.;
    mesonAmplitudeMax = mesonAmplitude*150./100.;

    TF1* fFitReco = new TF1("GaussExpLinear","(x<[1])*([0]*(TMath::Exp(-0.5*((x-[1])/[2])^2)+TMath::Exp((x-[1])/[3])*(1.-TMath::Exp(-0.5*((x-[1])/[2])^2)))+[4]+[5]*x)+(x>=[1])*([0]*(TMath::Exp(-0.5*((x-[1])/[2])^2)+TMath::Exp(-(x-[1])/[6])*(1.-TMath::Exp(-0.5*((x-[1])/[2])^2)))+[4]+[5]*x)",fitRangeMin, fitRangeMax);
    Double_t fMesonmassExpect;
    if (modeEta){
        fMesonmassExpect = 0.547862;
    } else {
        fMesonmassExpect = TDatabasePDG::Instance()->GetParticle(111)->Mass();
    }

    fFitReco->SetParameter(0, mesonAmplitude);
    fFitReco->SetParameter(1, fMesonmassExpect);
    fFitReco->SetParameter(4, 0.);
    fFitReco->SetParameter(5, 0.);

    // fFitReco->SetParLimits(0, mesonAmplitudeMin, mesonAmplitudeMax);
    fFitReco->SetParLimits(1, fMesonmassExpect*0.95, fMesonmassExpect*1.05);

    if (modeEta){
        fFitReco->SetParameter(2, 0.005);
        fFitReco->SetParLimits(2, 0.002, 0.015);
        fFitReco->SetParameter(3, 0.001);
        fFitReco->SetParLimits(3, 0.002, 0.05);
        fFitReco->SetParameter(6, 0.015);
        fFitReco->SetParLimits(6, 0.001,0.02);
    } else {
        fFitReco->SetParameter(2, 0.002);
        fFitReco->SetParLimits(2, 0.0005, 0.005);
        fFitReco->SetParameter(3, 0.012);
        fFitReco->SetParLimits(3, 0.001, 0.09);
        fFitReco->SetParameter(6, 0.015);
        fFitReco->SetParLimits(6,0.001,0.030);
    }

    histo->Fit(fFitReco,"QRME0");

    fFitReco->SetLineColor(kBlue+1);
    fFitReco->SetLineWidth(1);
    fFitReco->SetLineStyle(1);

    if(TString(gMinuit->fCstatu.Data()).Contains("CONVERGED") == 1 || TString(gMinuit->fCstatu.Data()).Contains("SUCCESSFUL") == 1 || TString(gMinuit->fCstatu.Data()).Contains("PROBLEMS") == 1){
        cout << "Parameter for exponential+Gaussian "<< endl;
        cout << gMinuit->fCstatu.Data() << endl;
        cout << "Gausexp: \t" << fFitReco->GetParameter(0) <<"+-" << fFitReco->GetParError(0) << "\t " << fFitReco->GetParameter(1)<<"+-" << fFitReco->GetParError(1) << "\t "<< fFitReco->GetParameter(2) <<"+-" << fFitReco->GetParError(2)<<endl;
        cout << "chi2/ndf:" << fFitReco->GetChisquare()/fFitReco->GetNDF() << endl;
    } else {
        cout << "Exp+Gaussian fitting failed in with status " << gMinuit->fCstatu.Data() <<endl << endl;
    }
    return fFitReco;
}

//*******************************************************************************
//************** Definition of fitting with Gaussian + Exp tail *****************
//*******************************************************************************
TF1* FitRecursiveGaussian (TH1* histo, Double_t precision, Double_t correctRange, Double_t fitRangeMin, Double_t fitRangeMax ){
    TF1 *f0             = new TF1("f0", "gaus", fitRangeMin,fitRangeMax);
    histo->Fit(f0,"0RMEQ");
    Double_t rp         = f0->GetParameter(2);
    Double_t mp         = f0->GetParameter(1);
    Double_t ymin       = mp -(rp * correctRange);
    Double_t ymax       = mp + (rp * correctRange);
    Double_t deviation  = 100;
    Int_t counter       = 0;
    TF1* f1             = new TF1 ("f1", "gaus", ymin, ymax);
    while(deviation > precision && counter < 100){
        f1->SetRange(ymin,ymax);
        histo->Fit(f1,"0RMEQ");
        Double_t rp2    = f1->GetParameter(2);
        if (rp2>rp){
            deviation   = rp2-rp;
        } else {
            deviation   = rp-rp2 ;
        }
        rp              = rp2 ;
        mp              = f1->GetParameter(1);
        ymin            = mp -(rp * correctRange);
        ymax            = mp +(rp * correctRange);
        counter++;
    }
    delete f0;

    if(TString(gMinuit->fCstatu.Data()).Contains("CONVERGED") == 1 || TString(gMinuit->fCstatu.Data()).Contains("SUCCESSFUL") == 1 || TString(gMinuit->fCstatu.Data()).Contains("PROBLEMS") == 1){
        cout << "Parameters for FitRecursiveGaussian: " << endl;
        for(Int_t i=0;i<=2;i++) cout << "Par " << i << ": " << f1->GetParameter(i) << " +- " << f1->GetParError(i) << endl;
    } else {
        cout << "FitRecursiveGaussian fitting failed with status " << gMinuit->fCstatu.Data() <<endl << endl;
    }

    return f1;
}
