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
TF1* FitGaussian(TH1D* histo, Double_t fitRangeMin, Double_t fitRangeMax, Int_t mode, Double_t ptcenter, Int_t iDataMC);
TF1* FitExpPlusGaussian(TH1D* histo, Double_t fitRangeMin, Double_t fitRangeMax, Int_t mode, Double_t ptcenter, Int_t iDataMC);
TF1* FitDExpPlusGaussian(TH1D* histo, Double_t fitRangeMin, Double_t fitRangeMax, Int_t mode, Double_t ptcenter, Int_t iDataMC);
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


Double_t ScalingFactor(Double_t *x, Double_t *par) {
    Double_t massPi0            = 0.1349766;
    Float_t beta = 1./sqrt(1+(massPi0*massPi0)/(x[0]*x[0]));
    Float_t gamma = 1./sqrt(1.-beta*beta);
    Float_t CosTheta = x[0]*x[0]/(x[0]*x[0]+gamma*gamma*beta*beta*massPi0*massPi0*0.5*0.5);
    
    return (4 * TMath::Sqrt2() * x[0] / TMath::Sqrt(1-CosTheta));
}
//****************************************************************************
//************** Function to Correct CaloNonLinearity4 ***********************
//****************************************************************************
void CorrectPCMSmearingV0(
                                TString configFileName  = "config.txt",
                                TString suffix          = "eps",
                                Bool_t enableAddCouts   = kFALSE,
                                Bool_t enablePol1Scaling   = kTRUE
                              ){
    gROOT->Reset();
    gPrintViaErrorHandler = kTRUE;

    StyleSettingsThesis();
    SetPlotStyle();


    // General options
    TString select              = "";
    TString optionEnergy        = "";
    Int_t mode                  = -1;
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
    Int_t firstPtBinSet[6]      = { -1, -1, -1, -1, -1,     -1};
    Double_t ptBinsForRebin[10] = { -1, -1, -1, -1, -1,     -1, -1, -1, -1, -1 };
    Int_t rebin[10]             = { 1, 1, 1, 1, 1,  1, 1, 1, 1, 1};
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
        // reading min and max bin for pt
        } else if (tempValue.BeginsWith("ptRangeBins",TString::kIgnoreCase)){
            if (enableAddCouts) cout << "setting ptBin bin Range" << endl;
            ptBinRange[0]               = ((TString)((TObjString*)tempArr->At(1))->GetString()).Atoi();
            ptBinRange[1]               = ((TString)((TObjString*)tempArr->At(2))->GetString()).Atoi();
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
    cout << "pt bin range set: " << ptBinRange[0] << " - " << ptBinRange[1] << endl;

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
    TString nameSignalHisto      = "ESD_Mother_InvMass_E_Calib_PCM";
    TString nameBGHisto          = "ESD_Background_InvMass_E_Calib_PCM";

    //*******************************************************************************
    //**********************  create output histos **********************************
    //*******************************************************************************
    TH1D* histMCResults         = new TH1D("Sigma MC","; #it{E}_{PCM #gamma} (GeV); mean sigma MC",fNBinsPt,fBinsPt);
    TH1D* histDataResults       = new TH1D("Sigma Data","; #it{E}_{PCM #gamma} (GeV); mean sigma Data",fNBinsPt,fBinsPt);
    TH1D* histDataMCResults     = new TH1D("Sigma ratio MC/Data","; #it{E}_{PCM #gamma} (GeV); mean sigma ratio (MC/Data)",fNBinsPt,fBinsPt);
    TH1D* histMCAmplitude         = new TH1D("Amplitude","; #it{E}_{PCM #gamma} (GeV); Amplitude",fNBinsPt,fBinsPt);
    TH1D* histDataAmplitude       = new TH1D("Amplitude","; #it{E}_{PCM #gamma} (GeV); Amplitude",fNBinsPt,fBinsPt);
    TH1D* histMCTail         = new TH1D("Tail","; #it{E}_{PCM #gamma} (GeV); Tail",fNBinsPt,fBinsPt);
    TH1D* histDataTail       = new TH1D("Tail","; #it{E}_{PCM #gamma} (GeV); Tail",fNBinsPt,fBinsPt);
    TH1D* histMCMass         = new TH1D("Mean Mass MC","; #it{E}_{PCM #gamma} (GeV); Sigma",fNBinsPt,fBinsPt);
    TH1D* histDataMass       = new TH1D("Mean Mass Data","; #it{E}_{PCM #gamma} (GeV); Sigma",fNBinsPt,fBinsPt);    
    
    // TH1D* histMCResultsEta         = new TH1D("Sigma MC","; #it{E}_{PCM #gamma} (GeV); mean sigma MC",fNBinsPt,fBinsPt);
    // TH1D* histDataResultsEta       = new TH1D("Sigma Data","; #it{E}_{PCM #gamma} (GeV); mean sigma Data",fNBinsPt,fBinsPt);
    // TH1D* histDataMCResultsEta     = new TH1D("Sigma ratio MC/Data","; #it{E}_{PCM #gamma} (GeV); mean sigma ratio (MC/Data)",fNBinsPt,fBinsPt);
    // TH1D* histMCAmplitudeEta         = new TH1D("Amplitude","; #it{E}_{PCM #gamma} (GeV); Amplitude",fNBinsPt,fBinsPt);
    // TH1D* histDataAmplitudeEta       = new TH1D("Amplitude","; #it{E}_{PCM #gamma} (GeV); Amplitude",fNBinsPt,fBinsPt);
    // TH1D* histMCTailEta         = new TH1D("Tail","; #it{E}_{PCM #gamma} (GeV); Tail",fNBinsPt,fBinsPt);
    // TH1D* histDataTailEta       = new TH1D("Tail","; #it{E}_{PCM #gamma} (GeV); Tail",fNBinsPt,fBinsPt);
    // TH1D* histMCMassEta         = new TH1D("Mean Mass MC","; #it{E}_{PCM #gamma} (GeV); Sigma",fNBinsPt,fBinsPt);
    // TH1D* histDataMassEta       = new TH1D("Mean Mass Data","; #it{E}_{PCM #gamma} (GeV); Sigma",fNBinsPt,fBinsPt);
    histMCResults->SetDirectory(0);
    histDataResults->SetDirectory(0);
    histMCResults->GetXaxis()->SetRangeUser(fBinsPt[ptBinRange[0]],fBinsPt[ptBinRange[1]]);
    histDataResults->GetXaxis()->SetRangeUser(fBinsPt[ptBinRange[0]],fBinsPt[ptBinRange[1]]);
    histMCResults->GetYaxis()->SetRangeUser(0.12,0.142);
    histDataResults->GetYaxis()->SetRangeUser(0.12,0.142);
    TF1* fFitReco;
    TF1* fFitsigma;
    TF1* fFitsigma2;
    TF1* fFitsigma3;
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


    // storing 2D hists in output root file
    fOutput->cd();
    dataInvsigmaPt->Write("Data - ESD_Mother_Invsigma");
    dataBGInvsigmaPt->Write("Data - ESD_Background_Invsigma");
    mcInvsigmaPt->Write("MC - ESD_Mother_Invsigma");
    mcBGInvsigmaPt->Write("MC - ESD_Background_Invsigma");

    TString dataMC[2]           = {"Data","MC"};
    Int_t triggerSel            = 0;
    Int_t rebinWindow           = 0;
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
            if(mode==2||mode==3){
              projectMin            = Hist2D->GetYaxis()->FindBin(fBinsPt[iClusterPt]+0.001);
              projectMax            = Hist2D->GetYaxis()->FindBin(fBinsPt[iClusterPt+1]-0.001);
            }else{
              projectMin            = Hist2D->GetYaxis()->FindBin((fBinsPt[iClusterPt]*2)+0.001);
              projectMax            = Hist2D->GetYaxis()->FindBin((fBinsPt[iClusterPt+1]*2)-0.001);
            }
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
            sliceBGHist->GetXaxis()->SetRangeUser(0.0,0.3);
            sliceBGHist->Write(Form("slice%sAlpha_%f-%f-BG",dataMC[iDataMC].Data(),fBinsPt[iClusterPt],fBinsPt[iClusterPt+1]));
            fFitBckPol1->SetLineColor(kGreen+2);
            fFitBckPol1->Write(Form("slice%sAlpha_%f-%f-BGFit",dataMC[iDataMC].Data(),fBinsPt[iClusterPt],fBinsPt[iClusterPt+1]));
            sliceHist->SetLineColor(kRed+2);
            sliceHist->GetXaxis()->SetRangeUser(0.0,0.3);
            sliceHist->Write(Form("slice%sAlpha_%f-%f-withRemainingBckg",dataMC[iDataMC].Data(),fBinsPt[iClusterPt],fBinsPt[iClusterPt+1]));
            if(enablePol1Scaling){
                sliceHistRatio->SetLineColor(kBlack+2);
                sliceHistRatio->GetXaxis()->SetRangeUser(0.0,0.3);
                sliceHistRatio->Write(Form("slice%sAlpha_%f-%f-sliceHistRatio",dataMC[iDataMC].Data(),fBinsPt[iClusterPt],fBinsPt[iClusterPt+1]));
            }

            sliceHistCopy->GetYaxis()->SetRangeUser(sliceHist->GetMinimum(),sliceHistCopy->GetMaximum());
            sliceHistCopy->GetXaxis()->SetRangeUser(0.0,0.3);
            sliceHistCopy->DrawCopy();
            // sliceBGHist->DrawCopy("same");
            fFitBckPol1->DrawCopy("same");
            sliceHist->DrawCopy("same");
            DrawGammaLines(0., 0.3, 0, 0, 1, kGray+1, 7);
            canvas->SetLogx(0); canvas->SetLogy(0); canvas->SetLogz(0); canvas->Update();

            for (Int_t i = 0; i < nExampleBins; i++ ){
                if(iClusterPt==exampleBin[i] ){
                    canvas->SaveAs(Form("%s/ExampleBin_%sAlpha_%.02f-%.02f-withBckgAndFit.%s",outputDirSample.Data(),dataMC[iDataMC].Data(),fBinsPt[iClusterPt],fBinsPt[iClusterPt+1],suffix.Data()));
                }
            }
            canvas->Clear();
            sliceHist->GetXaxis()->SetRangeUser(0.0,0.3);

            if(enablePol1Scaling){
                sliceHistRatio->GetYaxis()->SetRangeUser(sliceHistRatio->GetMinimum()-0.2,sliceHistRatio->GetMaximum()+0.2);
                sliceHistRatio->GetXaxis()->SetRangeUser(0.0,0.3);
                sliceHistRatio->DrawCopy();
                fFitBckPol1->DrawCopy("same");
                for (Int_t i = 0; i < nExampleBins; i++ ){
                    if(iClusterPt==exampleBin[i] ){
                        canvas->SaveAs(Form("%s/ExampleBin_%sAlpha_%.02f-%.02f-sliceHistRatio.%s",outputDirSample.Data(),dataMC[iDataMC].Data(),fBinsPt[iClusterPt],fBinsPt[iClusterPt+1],suffix.Data()));
                    }
                }
                canvas->Clear();
            }
            
            // //***********************************  Eta   ************************************
            // TH1D* sliceHistEta         = (TH1D*) Hist2D->ProjectionX(Form("sliceEta%sAlpha_%f-%f",dataMC[iDataMC].Data(),fBinsPt[iClusterPt],fBinsPt[iClusterPt+1]),projectMin,projectMax);
            // sliceHistEta->SetDirectory(0);
            // sliceHistEta->SetTitle(Form("Signal %s - %.02f < #it{E}_{PCM #gamma} < %.02f (GeV)",dataMC[iDataMC].Data(),fBinsPt[iClusterPt],fBinsPt[iClusterPt+1]));
            // sliceHistEta->GetYaxis()->SetTitle("#frac{d#it{M}_{inv}}{dN}");
            // sliceHistEta->Sumw2();
            // TH1D* sliceBGHistEta       = (TH1D*) HistBG2D->ProjectionX(Form("sliceBGEta%sAlpha_%f-%f",dataMC[iDataMC].Data(),fBinsPt[iClusterPt],fBinsPt[iClusterPt+1]),projectMin,projectMax);
            // sliceBGHistEta->SetDirectory(0);
            // sliceBGHistEta->SetTitle(Form("BG %s - %.02f < #it{E}_{PCM #gamma} < %.02f (GeV)",dataMC[iDataMC].Data(),fBinsPt[iClusterPt],fBinsPt[iClusterPt+1]));
            // sliceBGHistEta->GetYaxis()->SetTitle("#frac{d#it{M}_{inv}}{dN}");
            // sliceBGHistEta->Sumw2();
            // // Background subtraction ranges
            // Double_t minMGGBGEta           = 0.3;
            // Double_t integralSigAndBGEta   = sliceHistEta->Integral(sliceHistEta->FindBin(minMGGBGEta), sliceHistEta->FindBin(1.));
            // // Double_t integralBG         = sliceBGHist->Integral(sliceBGHist->FindBin(minMGGBG), sliceBGHist->FindBin(0.28));
            // // cout << "BG scaling: " << integralSigAndBG << "\t" << integralBG << "\t" << integralSigAndBG/ integralBG << endl;
            // TH1D* sliceHistRatioEta         = (TH1D*)sliceHistEta->Clone("sliceHistRatioEta");


            // TF1* fFitBckPol2Eta = new TF1("FitBckPol2Eta",fline2, 0.3, 1., 3);
            // fFitBckPol2Eta->SetParameter(0, integralSigAndBGEta);
            // fFitBckPol2Eta->SetParameter(1, 0.);
            // fFitBckPol2Eta->SetParameter(2, 0.);
            // sliceHistEta->Fit(fFitBckPol2Eta,"QRME0", "", 0.3, 1.);
            // fFitBckPol2Eta->SetNpx(100000);
            // TF1* fFitBckPol1Eta = new TF1("FitBckPol1Eta","[0]+[1]*x+[2]*x*x", 0., 2.);
            // fFitBckPol1Eta->SetNpx(100000);
            // fFitBckPol1Eta->SetParameter(0, fFitBckPol2Eta->GetParameter(0));
            // fFitBckPol1Eta->SetParameter(1, fFitBckPol2Eta->GetParameter(1));
            // fFitBckPol1Eta->SetParameter(2, fFitBckPol2Eta->GetParameter(2));

            // TH1D* sliceHistCopyEta         = (TH1D*)sliceHistEta->Clone("SliceCopyEta");
            // sliceHistEta->Add( fFitBckPol1Eta, -1);

            // for (Int_t i = 1; i< sliceHistEta->GetNbinsX()+1; i++){
            //     if (sliceHistEta->GetBinContent(i) == 0)
            //         sliceHistEta->SetBinError(i,1.);
            // }


            // sliceHistCopyEta->GetXaxis()->SetRangeUser(0.3,1.);
            // sliceHistCopyEta->Write(Form("sliceEta%sAlpha_%f-%f-withWithBG",dataMC[iDataMC].Data(),fBinsPt[iClusterPt],fBinsPt[iClusterPt+1]));
            // sliceBGHistEta->SetLineColor(kGreen+2);
            // sliceBGHistEta->GetXaxis()->SetRangeUser(0.3,1.);
            // sliceBGHistEta->Write(Form("sliceEta%sAlpha_%f-%f-BG",dataMC[iDataMC].Data(),fBinsPt[iClusterPt],fBinsPt[iClusterPt+1]));
            // fFitBckPol1Eta->SetLineColor(kGreen+2);
            // fFitBckPol1Eta->Write(Form("sliceEta%sAlpha_%f-%f-BGFit",dataMC[iDataMC].Data(),fBinsPt[iClusterPt],fBinsPt[iClusterPt+1]));
            // sliceHistEta->SetLineColor(kRed+2);
            // sliceHistEta->GetXaxis()->SetRangeUser(0.3,1.);
            // sliceHistEta->Write(Form("sliceEta%sAlpha_%f-%f-withRemainingBckg",dataMC[iDataMC].Data(),fBinsPt[iClusterPt],fBinsPt[iClusterPt+1]));
            // if(enablePol1Scaling){
            //     sliceHistRatioEta->SetLineColor(kBlack+2);
            //     sliceHistRatioEta->GetXaxis()->SetRangeUser(0.3,1.);
            //     sliceHistRatioEta->Write(Form("sliceEta%sAlpha_%f-%f-sliceHistRatio",dataMC[iDataMC].Data(),fBinsPt[iClusterPt],fBinsPt[iClusterPt+1]));
            // }

            // sliceHistCopyEta->GetYaxis()->SetRangeUser(sliceHistEta->GetMinimum(),sliceHistCopyEta->GetMaximum());
            // sliceHistCopyEta->GetXaxis()->SetRangeUser(0.3,1.);
            // sliceHistCopyEta->DrawCopy();
            // // sliceBGHist->DrawCopy("same");
            // fFitBckPol1Eta->DrawCopy("same");
            // sliceHistEta->DrawCopy("same");
            // DrawGammaLines(0., 0.3, 0, 0, 1, kGray+1, 7);
            // canvas->SetLogx(0); canvas->SetLogy(0); canvas->SetLogz(0); canvas->Update();

            // for (Int_t i = 0; i < nExampleBins; i++ ){
            //     if(iClusterPt==exampleBin[i] ){
            //         canvas->SaveAs(Form("%s/ExampleBinEta_%sAlpha_%.02f-%.02f-withBckgAndFit.%s",outputDirSample.Data(),dataMC[iDataMC].Data(),fBinsPt[iClusterPt],fBinsPt[iClusterPt+1],suffix.Data()));
            //     }
            // }
            // canvas->Clear();
            // sliceHistEta->GetXaxis()->SetRangeUser(0.3,1.);

            // if(enablePol1Scaling){
            //     sliceHistRatioEta->GetYaxis()->SetRangeUser(sliceHistRatioEta->GetMinimum()-0.2,sliceHistRatioEta->GetMaximum()+0.2);
            //     sliceHistRatioEta->GetXaxis()->SetRangeUser(0.3,1.);
            //     sliceHistRatioEta->DrawCopy();
            //     fFitBckPol1Eta->DrawCopy("same");
            //     for (Int_t i = 0; i < nExampleBins; i++ ){
            //         if(iClusterPt==exampleBin[i] ){
            //             canvas->SaveAs(Form("%s/ExampleBinEta_%sAlpha_%.02f-%.02f-sliceHistRatio.%s",outputDirSample.Data(),dataMC[iDataMC].Data(),fBinsPt[iClusterPt],fBinsPt[iClusterPt+1],suffix.Data()));
            //         }
            //     }
            //     canvas->Clear();
            // }

            //*******************************************************************************
            // adjust min and max fitting range for inv sigma fits
            //*******************************************************************************
            Double_t minMax[2]={0.04,0.3};
            // special setting for PCM-EMC
            if( mode == 2 || mode == 13 || mode == 14 ){
                if (optionEnergy.Contains("PbPb") || optionEnergy.Contains("XeXe")){
                    minMax[1]   = 0.3;
                } else if (optionEnergy.Contains("pPb_5.023TeVRun2") ){
                    if (fBinsPt[iClusterPt] < 1.5)
                        minMax[1]       = 0.18;
                    else if (fBinsPt[iClusterPt] < 2)
                        minMax[1]       = 0.20;
                    else if (fBinsPt[iClusterPt] < 3)
                        minMax[1]       = 0.25;
                } else if (optionEnergy.Contains("8TeV") ){
                        minMax[1]       = 0.29;
                } else {
                    if (fBinsPt[iClusterPt] < 1)
                        minMax[1]       = 0.2;
                    else if (fBinsPt[iClusterPt] < 2)
                        minMax[1]       = 0.22;
                    else if (fBinsPt[iClusterPt] < 3)
                        minMax[1]       = 0.25;
                }
                minMax[0]       = 0.02;
                Double_t min    = 0.002*fBinsPt[iClusterPt] - 0.001;
                if (min > minMax[0])
                    minMax[0]   = min;

            // special setting for PCM-PHOS
            } else if( mode == 3 ){
                minMax[0]       = 0.03;
                if (optionEnergy.Contains("pPb_5.023TeVRun2") ){
                    if (fBinsPt[iClusterPt] < 1.5)
                        minMax[1]       = 0.18;
                    else if (fBinsPt[iClusterPt] < 2)
                        minMax[1]       = 0.20;
                    else if (fBinsPt[iClusterPt] < 3)
                        minMax[1]       = 0.25;
                } else if (optionEnergy.Contains("13TeV") ){
                        minMax[0]       = 0.08;
                        minMax[1]       = 0.18;
                } else {
                    if (fBinsPt[iClusterPt] < 1)
                        minMax[1]       = 0.20;
                    else
                        minMax[1]       = 0.25;
                }


            // special setting for EMC
          } else if( mode == 4 || mode == 12  || mode == 15){
                minMax[1]       = 0.25;
                Double_t min    = 0.02*fBinsPt[iClusterPt] - 0.001;
                if (min > minMax[0])
                    minMax[0]   = min;

                if (optionEnergy.Contains("PbPb") || optionEnergy.Contains("XeXe")){
                    minMax[1]   = 0.3;
                } else if (optionEnergy.Contains("pPb_5.023TeVRun2") ){
                    if (fBinsPt[iClusterPt] < 1.5)
                        minMax[1]       = 0.18;
                    else if (fBinsPt[iClusterPt] < 2)
                        minMax[1]       = 0.20;
                    else if (fBinsPt[iClusterPt] < 3)
                        minMax[1]       = 0.25;
                    else
                        minMax[1]       = 0.3;
                }

            // special setting for PHOS
            } else if( mode == 5){
                minMax[0]       = 0.04;
                minMax[1]       = 0.25;
                if (optionEnergy.Contains("13TeV") ){
                    minMax[0]       = 0.07;
                    minMax[1]       = 0.2;
                }
                if (fBinsPt[iClusterPt] < 0.7){
                    minMax[1]   = 0.2;
                    if (optionEnergy.Contains("13TeV") ){
                        minMax[1]       = 0.19;
                    }
                }
                Double_t min    = 0.005*fBinsPt[iClusterPt] - 0.001;
                if (min > minMax[0])
                    minMax[0]   = min;
            }
            cout << "invsigma fit range: \t" << minMax[0] << "\t" << minMax[1] << endl;
            //*******************************************************************************
            // Fit
            //*******************************************************************************
            if(optionEnergy.Contains("8TeV") || optionEnergy.Contains("7TeV"))
                fFitReco = FitGaussian (sliceHist, minMax[0], minMax[1], mode, (fBinsPt[iClusterPt]+fBinsPt[iClusterPt+1])/2, iDataMC);
                // fFitReco = FitDExpPlusGaussian (sliceHist, minMax[0], minMax[1], mode, (fBinsPt[iClusterPt]+fBinsPt[iClusterPt+1])/2, iDataMC); // possibility to use double exponential fit
            else
                fFitReco = FitExpPlusGaussian (sliceHist, minMax[0], minMax[1], mode, (fBinsPt[iClusterPt]+fBinsPt[iClusterPt+1])/2, iDataMC);

            if(iDataMC==0) {
                histDataMass->SetBinContent(iClusterPt+1,fFitReco->GetParameter(1));
                histDataMass->SetBinError(iClusterPt+1,fFitReco->GetParError(1));
                histDataAmplitude->SetBinContent(iClusterPt+1,fFitReco->GetParameter(0));
                histDataAmplitude->SetBinError(iClusterPt+1,fFitReco->GetParError(0));
                histDataTail->SetBinContent(iClusterPt+1,fFitReco->GetParameter(3));
                histDataTail->SetBinError(iClusterPt+1,fFitReco->GetParError(3));
                histDataResults->SetBinContent(iClusterPt+1,fFitReco->GetParameter(2));
                histDataResults->SetBinError(iClusterPt+1,fFitReco->GetParError(2));
            }
            else if(iDataMC==1) {
                histMCMass->SetBinContent(iClusterPt+1,fFitReco->GetParameter(1));
                histMCMass->SetBinError(iClusterPt+1,fFitReco->GetParError(1));
                histMCAmplitude->SetBinContent(iClusterPt+1,fFitReco->GetParameter(0));
                histMCAmplitude->SetBinError(iClusterPt+1,fFitReco->GetParError(0));
                histMCTail->SetBinContent(iClusterPt+1,fFitReco->GetParameter(3));
                histMCTail->SetBinError(iClusterPt+1,fFitReco->GetParError(3));
                histMCResults->SetBinContent(iClusterPt+1,fFitReco->GetParameter(2));
                histMCResults->SetBinError(iClusterPt+1,fFitReco->GetParError(2));
            }
            fFitReco->SetNpx(100000);

            sliceHist->GetListOfFunctions()->Add(fFitReco);
            sliceHist->GetXaxis()->SetRangeUser(0.0,0.3);
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


            // TF1* fFitRecoEta;
            // if(optionEnergy.Contains("8TeV") || optionEnergy.Contains("7TeV"))
            //     fFitRecoEta = FitGaussian (sliceHistEta, minMax[0], minMax[1], mode, (fBinsPt[iClusterPt]+fBinsPt[iClusterPt+1])/2, iDataMC);
            //     // fFitRecoEta = FitDExpPlusGaussian (sliceHist, minMax[0], minMax[1], mode, (fBinsPt[iClusterPt]+fBinsPt[iClusterPt+1])/2, iDataMC); // possibility to use double exponential fit
            // else
            //     fFitRecoEta = FitExpPlusGaussian (sliceHistEta, minMax[0], minMax[1], mode, (fBinsPt[iClusterPt]+fBinsPt[iClusterPt+1])/2, iDataMC);

            // if(iDataMC==0) {
            //     histDataMassEta->SetBinContent(iClusterPt+1,fFitRecoEta->GetParameter(1));
            //     histDataMassEta->SetBinError(iClusterPt+1,fFitRecoEta->GetParError(1));
            //     histDataAmplitudeEta->SetBinContent(iClusterPt+1,fFitRecoEta->GetParameter(0));
            //     histDataAmplitudeEta->SetBinError(iClusterPt+1,fFitRecoEta->GetParError(0));
            //     histDataTailEta->SetBinContent(iClusterPt+1,fFitRecoEta->GetParameter(3));
            //     histDataTailEta->SetBinError(iClusterPt+1,fFitRecoEta->GetParError(3));
            //     histDataResultsEta->SetBinContent(iClusterPt+1,fFitRecoEta->GetParameter(2));
            //     histDataResultsEta->SetBinError(iClusterPt+1,fFitRecoEta->GetParError(2));
            // }
            // else if(iDataMC==1) {
            //     histMCMassEta->SetBinContent(iClusterPt+1,fFitRecoEta->GetParameter(1));
            //     histMCMassEta->SetBinError(iClusterPt+1,fFitRecoEta->GetParError(1));
            //     histMCAmplitudeEta->SetBinContent(iClusterPt+1,fFitRecoEta->GetParameter(0));
            //     histMCAmplitudeEta->SetBinError(iClusterPt+1,fFitRecoEta->GetParError(0));
            //     histMCTailEta->SetBinContent(iClusterPt+1,fFitRecoEta->GetParameter(3));
            //     histMCTailEta->SetBinError(iClusterPt+1,fFitRecoEta->GetParError(3));
            //     histMCResultsEta->SetBinContent(iClusterPt+1,fFitRecoEta->GetParameter(2));
            //     histMCResultsEta->SetBinError(iClusterPt+1,fFitRecoEta->GetParError(2));
            // }
            // fFitRecoEta->SetNpx(100000);

            // sliceHistEta->GetListOfFunctions()->Add(fFitRecoEta);
            // sliceHistEta->GetXaxis()->SetRangeUser(0.3,1.);
            // cout << "mean: " << sliceHistEta->GetMean() << "\tmaximum: " << sliceHistEta->GetMaximum() << endl;
            // sliceHistEta->DrawCopy();
            // DrawGammaLines(0., 0.3, 0, 0, 1, kGray+1, 7);
            // canvas->SetLogx(0); canvas->SetLogy(0); canvas->SetLogz(0); canvas->Update();
            // for (Int_t i = 0; i < nExampleBins; i++ ){
            //     if(iClusterPt==exampleBin[i] ){
            //         canvas->SaveAs(Form("%s/ExampleBinEta_%sAlpha_%.02f-%.02f.%s",outputDirSample.Data(),dataMC[iDataMC].Data(),fBinsPt[iClusterPt],fBinsPt[iClusterPt+1],suffix.Data()));
            //     }
            // }
            // canvas->Clear();
        }
    }
    delete canvas;

    cout << endl;
    cout << "-----------------------------------------------------" << endl;
    cout << "-----------------------------------------------------" << endl;
    histDataMCResults->Add(histDataResults,histMCResults,1,-1);
    TF1* fSigmaPCM          = new TF1("fSigmaPCM", "pol1" ,rangeHighPtFitRatio[0],rangeHighPtFitRatio[1]);
    fSigmaPCM->SetParameters(0.00204649,0.000193091);


    TF1* fScalingFactor          = new TF1("fScalingFactor", ScalingFactor ,rangeHighPtFitRatio[0],rangeHighPtFitRatio[1], 0);
    histDataMCResults->Multiply(fScalingFactor,1.);
    histDataMCResults->Divide(fSigmaPCM,1.);
    // histDataMCResults->Multiply(histDataMCResults,histDataResults,1.,1.,"B");
    DrawGammaSetMarker(histDataMCResults, 24, 2, kBlack, kBlack);

    Double_t minPlotY = -0.0005;
    // Double_t minPlotY = 0.;
    Double_t maxPlotY = 0.0032;
    // Double_t maxPlotY = 1.4;

    Double_t minsigma  = 0.;
    Double_t maxsigma  = 0.02;
    //*********************************************************************************************************************************
    //************************************ Write mean sigma for MC and data into output file *******************************************
    //*********************************************************************************************************************************
    SetStyleHistoTH1ForGraphs(histMCResults, "#it{E}_{PCM #gamma} (GeV)","#sigma_{#pi^{0} (MC)} ",0.035,0.043, 0.035,0.043, 1.,1.);
    DrawGammaSetMarker(histMCResults, markerStyle[1], 1, color[1], color[1]);
    SetStyleHistoTH1ForGraphs(histDataResults, "#it{E}_{PCM #gamma} (GeV)","#sigma_{#pi^{0} (data)} ",0.035,0.043, 0.035,0.043, 1.,1.);
    DrawGammaSetMarker(histDataResults, markerStyle[0], 1, color[0], color[0]);

    // SetStyleHistoTH1ForGraphs(histMCResultsEta, "#it{E}_{PCM #gamma} (GeV)","#sigma_{#eta^{0} (MC)} ",0.035,0.043, 0.035,0.043, 1.,1.);
    // DrawGammaSetMarker(histMCResultsEta, markerStyle[1], 1, color[1], color[1]);
    // SetStyleHistoTH1ForGraphs(histDataResultsEta, "#it{E}_{PCM #gamma} (GeV)","#sigma_{#eta^{0} (data)} ",0.035,0.043, 0.035,0.043, 1.,1.);
    // DrawGammaSetMarker(histDataResultsEta, markerStyle[0], 1, color[0], color[0]);

    //*********************************************************************************************************************************
    //*********************************** Plotting Mean sigma for data and MC vs PDG value *********************************************
    //*********************************************************************************************************************************
    cout<<"Plotting Mean sigma for data and MC vs PDG value"<<endl;
    TCanvas *canvassigmaPDG = new TCanvas("canvassigmaPDG","",200,10,1350,900);  // gives the page size
    DrawGammaCanvasSettings(canvassigmaPDG, 0.08, 0.02, 0.055, 0.08);
    canvassigmaPDG->SetLogx(1);
    canvassigmaPDG->SetLogy(0);

    TH2F * histoDummyMeansigmaVsPDG;
    histoDummyMeansigmaVsPDG = new TH2F("histoDummyMeansigmaVsPDG", "histoDummyMeansigmaVsPDG", 11000, fBinsPt[ptBinRange[0]]/1.5, fBinsPt[ptBinRange[1]]*1.5, 1000, minsigma, maxsigma);
    SetStyleHistoTH2ForGraphs(histoDummyMeansigmaVsPDG, "#it{E}_{PCM #gamma} (GeV)"," #sigma_{#pi^{0} (MC/data)} ", 0.035, 0.043, 0.035, 0.043, 0.82, 0.9);
    histoDummyMeansigmaVsPDG->GetXaxis()->SetMoreLogLabels();
    histoDummyMeansigmaVsPDG->GetXaxis()->SetLabelOffset(-0.01);
    histoDummyMeansigmaVsPDG->DrawCopy("");

    Double_t standardrangeExponent0 =   -0.5;
    Double_t standardrangeExponent1 =   -0.08;
    Double_t rangeMult0             =   -0.2;
    Double_t rangeMult1             =   -0.001;
    Double_t rangeExponent[2]       =   {standardrangeExponent0, standardrangeExponent1};
    Double_t rangeMult[2]           =   {rangeMult0, rangeMult1};

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

    // // fitting data sigma positions
    // FittingFunction="[0] + [1]*TMath::Power(x,[2])";
    // if (mode==3) {
    //     rangeExponent[0]   = 0.08;
    //     rangeExponent[1]   = 2.5;
    //     rangeMult[0]       = 0.001;
    //     rangeMult[1]       = 0.3;
    // }
    // TF1* fitsigmaDataVsPDG       = new TF1("fitsigmaDataVsPDG", FittingFunction ,fBinsPt[ptBinRange[0]],fBinsPt[ptBinRange[1]]);
    // if (mode != 5){
    //     fitsigmaDataVsPDG->SetParLimits(1, rangeMult[0], rangeMult[1]);
    //     fitsigmaDataVsPDG->SetParLimits(2, rangeExponent[0], rangeExponent[1]);
    // }
    // histDataResults->Fit(fitsigmaDataVsPDG,"QRME0");
    // cout << WriteParameterToFile(fitsigmaDataVsPDG) << endl;

    // FittingFunction="[0]";
    // if (mode==3) {
    //     rangeExponent[0]   = standardrangeExponent0;
    //     rangeExponent[1]   = standardrangeExponent1;
    //     rangeMult[0]       = rangeMult0;
    //     rangeMult[1]       = rangeMult1;
    // }
    // TF1* fitsigmaDataVsPDGConst  = new TF1("fitsigmaDataVsPDGConst", FittingFunction ,rangeHighPtFitsigma[0],rangeHighPtFitsigma[1]);
    // histDataResults->Fit(fitsigmaDataVsPDGConst,"QRME0");
    // cout << WriteParameterToFile(fitsigmaDataVsPDGConst) << endl;

    // FittingFunction="[0]-[3]*TMath::Exp(-[1]*x+[2])";
    // if (mode==3) {
    //     rangeExponent[0]   = standardrangeExponent0;
    //     rangeExponent[1]   = standardrangeExponent1;
    //     rangeMult[0]       = rangeMult0;
    //     rangeMult[1]       = rangeMult1;
    // }
    // TF1* fitsigmaDataVsPDG2      = new TF1("fitsigmaDataVsPDG2", FittingFunction ,fBinsPt[ptBinRange[0]],fBinsPt[ptBinRange[1]]);
    // fitsigmaDataVsPDG2->SetParameter(0, fitsigmaDataVsPDGConst->GetParameter(0));
    // if ( (mode == 2 || mode == 4 || mode == 12 || mode == 14 || mode == 15) && (optionEnergy.Contains("PbPb") || optionEnergy.Contains("XeXe")  ) ){
    //     fitsigmaDataVsPDG2->SetParLimits(0, fitsigmaDataVsPDGConst->GetParameter(0)-0.1*fitsigmaDataVsPDGConst->GetParError(0), fitsigmaDataVsPDGConst->GetParameter(0)+0.1*fitsigmaDataVsPDGConst->GetParError(0));
    // } else if (mode == 2 || mode == 4 || mode == 12 || mode == 14 || mode == 15){
    //     fitsigmaDataVsPDG2->SetParLimits(0, fitsigmaDataVsPDGConst->GetParameter(0)-3*fitsigmaDataVsPDGConst->GetParError(0), fitsigmaDataVsPDGConst->GetParameter(0)+3*fitsigmaDataVsPDGConst->GetParError(0));
    // } else {
    //   fitsigmaDataVsPDG2->SetParLimits(0, fitsigmaDataVsPDGConst->GetParameter(0)-0.5*fitsigmaDataVsPDGConst->GetParError(0), fitsigmaDataVsPDGConst->GetParameter(0)+0.5*fitsigmaDataVsPDGConst->GetParError(0));
    // }
    // fitsigmaDataVsPDG2->FixParameter(3,1.);
    // if (mode == 5) fitsigmaDataVsPDG2->FixParameter(3,-1.);

    // histDataResults->Fit(fitsigmaDataVsPDG2,"QRME0");

    // // fitting MC sigma positions
    // FittingFunction="[0] + [1]*TMath::Power(x,[2])";
    // if (mode==3) {
    //     rangeExponent[0]   = standardrangeExponent0;
    //     rangeExponent[1]   = standardrangeExponent1;
    //     //rangeMult[0]       = rangeMult0;
    //     //rangeMult[1]       = rangeMult1;
    //     rangeMult[0]       = -rangeMult1;
    //     rangeMult[1]       = -rangeMult0;
    // }
    // if (mode==5){
    //     rangeExponent[0]  = -3.;
    //     rangeExponent[1]  = standardrangeExponent1;
    //     rangeMult[0]      = -rangeMult1;
    //     rangeMult[1]      = -rangeMult0;
    // }
    // TF1* fitsigmaMCVsPDG = new TF1("fitsigmaMCVsPDG", FittingFunction ,fBinsPt[ptBinRange[0]],fBinsPt[ptBinRange[1]]);
    // if (mode != 5){
    //     fitsigmaMCVsPDG->SetParLimits(1, rangeMult[0], rangeMult[1]);
    //     fitsigmaMCVsPDG->SetParLimits(2, rangeExponent[0], rangeExponent[1]);
    // }
    // histMCResults->Fit(fitsigmaMCVsPDG,"QRME0");
    // cout << WriteParameterToFile(fitsigmaMCVsPDG) << endl;

    // FittingFunction="[0]";
    // if (mode==3) {
    //     rangeExponent[0]   = standardrangeExponent0;
    //     rangeExponent[1]   = standardrangeExponent1;
    //     rangeMult[0]       = rangeMult0;
    //     rangeMult[1]       = rangeMult1;
    // }
    // TF1* fitsigmaMCVsPDGConst  = new TF1("fitsigmaMCVsPDGConst", FittingFunction ,rangeHighPtFitsigma[2],rangeHighPtFitsigma[3]);
    // histMCResults->Fit(fitsigmaMCVsPDGConst,"QRME0");
    // cout << WriteParameterToFile(fitsigmaMCVsPDGConst) << endl;

    // FittingFunction="[0]-[3]*TMath::Exp(-[1]*x+[2])";
    // if (mode==3) {
    //     rangeExponent[0]   = standardrangeExponent0;
    //     rangeExponent[1]   = standardrangeExponent1;
    //     rangeMult[0]       = rangeMult0;
    //     rangeMult[1]       = rangeMult1;
    // }
    // if (mode==5){
    //     rangeExponent[0]  = -10.;
    //     rangeExponent[1]  = 10.;
    //     rangeMult[0]      = -10.;
    //     rangeMult[1]      = 10.;
    // }
    // TF1* fitsigmaMCVsPDG2 = new TF1("fitsigmaMCVsPDG2", FittingFunction ,fBinsPt[ptBinRange[0]],fBinsPt[ptBinRange[1]]);
    // fitsigmaMCVsPDG2->SetParameter(0, fitsigmaMCVsPDGConst->GetParameter(0));
    // if ( (mode == 2 || mode == 4 || mode == 12 || mode == 14 || mode == 15) && (optionEnergy.Contains("PbPb") || optionEnergy.Contains("XeXe")  ) ){
    //     fitsigmaMCVsPDG2->SetParLimits(0, fitsigmaMCVsPDGConst->GetParameter(0)-0.1*fitsigmaMCVsPDGConst->GetParError(0), fitsigmaMCVsPDGConst->GetParameter(0)+0.1*fitsigmaMCVsPDGConst->GetParError(0));
    // } else if (mode == 2 || mode == 4 || mode == 12 || mode == 14 || mode == 15){
    //     fitsigmaMCVsPDG2->SetParLimits(0, fitsigmaMCVsPDGConst->GetParameter(0)-2*fitsigmaMCVsPDGConst->GetParError(0), fitsigmaMCVsPDGConst->GetParameter(0)+2*fitsigmaMCVsPDGConst->GetParError(0));
    // // } else if (mode == 5){
    // //     fitsigmaMCVsPDG2->SetParLimits(0, fitsigmaMCVsPDGConst->GetParameter(0)-0.5*fitsigmaMCVsPDGConst->GetParError(0), fitsigmaMCVsPDGConst->GetParameter(0)+0.5*fitsigmaMCVsPDGConst->GetParError(0));
    // //     fitsigmaMCVsPDG2->SetParLimits(1, -10., 10.);
    // //     fitsigmaMCVsPDG2->SetParLimits(2, -10., 10.);
    // } else {
    //     fitsigmaMCVsPDG2->SetParLimits(0, fitsigmaMCVsPDGConst->GetParameter(0)-0.5*fitsigmaMCVsPDGConst->GetParError(0), fitsigmaMCVsPDGConst->GetParameter(0)+0.5*fitsigmaMCVsPDGConst->GetParError(0));
    //     cout << fitsigmaMCVsPDGConst->GetParameter(0)-0.5*fitsigmaMCVsPDGConst->GetParError(0) << " " << fitsigmaMCVsPDGConst->GetParameter(0)+0.5*fitsigmaMCVsPDGConst->GetParError(0) << endl;
    // }
    // fitsigmaMCVsPDG2->FixParameter(3,1.);


    // histMCResults->Fit(fitsigmaMCVsPDG2,"QRME0");
    // cout << WriteParameterToFile(fitsigmaMCVsPDG2) << endl;


    // // draw data graphs and fits
    // DrawGammaSetMarkerTF1( fitsigmaDataVsPDG, 1, 2, kBlack);
    //  if (!(select.Contains("LHC11cd") && mode == 2))fitsigmaDataVsPDG->Draw("same");
    // DrawGammaSetMarkerTF1( fitsigmaDataVsPDGConst, 7, 2, kGray);
    // fitsigmaDataVsPDGConst->Draw("same");
    // DrawGammaSetMarkerTF1( fitsigmaDataVsPDG2, 7, 2, kGray+1);
    // fitsigmaDataVsPDG2->Draw("same");
    histDataResults->DrawCopy("same");
    legend->AddEntry(histDataResults,"Data");

    // DrawGammaSetMarkerTF1( fitsigmaMCVsPDG, 1, 2, kRed+2);
    // DrawGammaSetMarkerTF1( fitsigmaMCVsPDGConst, 7, 2, kRed-8);
    // fitsigmaMCVsPDGConst->Draw("same");
    // DrawGammaSetMarkerTF1( fitsigmaMCVsPDG2, 7, 2, kRed-6);
    // fitsigmaMCVsPDG2->Draw("same");
    histMCResults->DrawCopy("same");
    legend->AddEntry(histMCResults,"MC");

    PutProcessLabelAndEnergyOnPlot(0.94, 0.915, 0.03, fCollisionSystem.Data(), fTextMeasurement.Data(), recGamma.Data(), 42, 0.03, "", 1, 1.25, 31);
    for (Int_t i = 0; i < nSets; i++){
       PutProcessLabelAndEnergyOnPlot(0.12, 0.915-2*0.03*(i), 0.03, fPlotLabelsRatio[i].Data(),"", "", 42, 0.03, "", 1, 1.25, 11);
    }

    // TLegend *legendFits   = GetAndSetLegend2(0.12, 0.12 , 0.37, 0.12 + 3*0.03, 0.03, 2, "", 42, 0.35);
    // legendFits->AddEntry(fitsigmaDataVsPDG, " ", "l");
    // legendFits->AddEntry(fitsigmaMCVsPDG, "powerlaw fit","l" );
    // legendFits->AddEntry(fitsigmaDataVsPDG2, " ", "l");
    // legendFits->AddEntry(fitsigmaMCVsPDG2, "exponential fit","l" );
    // legendFits->AddEntry(fitsigmaDataVsPDGConst, " ", "l");
    // legendFits->AddEntry(fitsigmaMCVsPDGConst, "high #it{p}_{T} const.","l" );
    // legendFits->Draw("same");

    legend->Draw("same");
    canvassigmaPDG->Update();
    canvassigmaPDG->SaveAs(Form("%s/Sigma_Pi0_%s.%s", outputDirSampleSummary.Data(), select.Data(), suffix.Data()));
    canvassigmaPDG->Clear();
    delete canvassigmaPDG;

    Double_t textsize = 0.07;
    TCanvas *canvasmonitor = new TCanvas("canvasmonitor","",200,10,1400,1400);  // gives the page size
    TLegend *legendmonitorMass   = GetAndSetLegend2(0.4, 0.91 , 0.7, 0.98, textsize, 2, "", 42, 0.35);
    TLegend *legendmonitorTail   = GetAndSetLegend2(0.4, 0.91 , 0.7, 0.98, textsize, 2, "", 42, 0.35);
    TLegend *legendmonitorAmplitude   = GetAndSetLegend2(0.4, 0.91 , 0.7, 0.98, textsize, 2, "", 42, 0.35);
    TPad *padmonitorMass;
    TPad *padmonitorTail;
    TPad *padmonitorAmplitude;

    DrawGammaCanvasSettings(canvasmonitor, 0.08, 0.02, 0.055, 0.08);
    canvasmonitor->cd();

    padmonitorMass = new TPad("padmonitorMass","",0,0.66,1,0.99);
    padmonitorMass->SetTopMargin(0.1);
    padmonitorMass->SetBottomMargin(0.1);
    padmonitorMass->SetRightMargin(0.02);
    padmonitorMass->SetLeftMargin(0.07);
    padmonitorMass->SetTickx();
    padmonitorMass->SetTicky();
    padmonitorMass->cd();
    padmonitorMass->SetLogx(1);
    padmonitorMass->SetLogy(0);

    padmonitorTail = new TPad("padmonitorTail","",0,0.33,1,0.66);
    padmonitorTail->SetTopMargin(0.1);
    padmonitorTail->SetBottomMargin(0.1);
    padmonitorTail->SetRightMargin(0.02);
    padmonitorTail->SetLeftMargin(0.07);
    padmonitorTail->SetTickx();
    padmonitorTail->SetTicky();
    padmonitorTail->cd();
    padmonitorTail->SetLogx(1);
    padmonitorTail->SetLogy(0);

    padmonitorAmplitude = new TPad("padmonitorAmplitude","",0,0.,1,0.33);
    padmonitorAmplitude->SetTopMargin(0.1);
    padmonitorAmplitude->SetBottomMargin(0.1);
    padmonitorAmplitude->SetRightMargin(0.02);
    padmonitorAmplitude->SetLeftMargin(0.07);
    padmonitorAmplitude->SetTickx();
    padmonitorAmplitude->SetTicky();
    padmonitorAmplitude->cd();
    padmonitorAmplitude->SetLogx(1);
    padmonitorAmplitude->SetLogy(1);

    padmonitorAmplitude->cd();
    legendmonitorMass->AddEntry(histDataAmplitude,"Data", "pl");
    SetStyleHistoTH1ForGraphs(histDataAmplitude, "#it{E}_{PCM #gamma} (GeV)","Amplitude",textsize,textsize,textsize,textsize,0.5,0.4);
    DrawGammaSetMarker(histDataAmplitude, markerStyle[0], 2., color[2], color[2]);
    histDataAmplitude->DrawCopy("pl");
    legendmonitorMass->AddEntry(histMCAmplitude,"MC", "pl");
    SetStyleHistoTH1ForGraphs(histMCAmplitude, "#it{E}_{PCM #gamma} (GeV)","Amplitude",textsize,textsize,textsize,textsize,0.5,0.4);
    DrawGammaSetMarker(histMCAmplitude, markerStyle[1], 2., color[5], color[5]);
    histMCAmplitude->DrawCopy("pl,same");
    legendmonitorMass->Draw();


    padmonitorTail->cd();
    legendmonitorTail->AddEntry(histDataTail,"Data", "pl");
    SetStyleHistoTH1ForGraphs(histDataTail, "#it{E}_{PCM #gamma} (GeV)","Tail",textsize,textsize,textsize,textsize,0.5,0.4);
    DrawGammaSetMarker(histDataTail, markerStyle[0], 2., color[2], color[2]);
    histDataTail->DrawCopy("pl");
    legendmonitorTail->AddEntry(histMCTail,"MC", "pl");
    SetStyleHistoTH1ForGraphs(histMCTail, "#it{E}_{PCM #gamma} (GeV)","Tail",textsize,textsize,textsize,textsize,0.5,0.4);
    DrawGammaSetMarker(histMCTail, markerStyle[1], 2., color[5], color[5]);
    histMCTail->DrawCopy("pl,same");
    legendmonitorTail->Draw();

    padmonitorMass->cd();
    legendmonitorAmplitude->AddEntry(histDataMass,"Data", "pl");
    SetStyleHistoTH1ForGraphs(histDataMass, "#it{E}_{PCM #gamma} (GeV)","Mass",textsize,textsize,textsize,textsize,0.5,0.4);
    DrawGammaSetMarker(histDataMass, markerStyle[0], 2., color[2], color[2]);
    histDataMass->DrawCopy("pl");
    legendmonitorAmplitude->AddEntry(histMCMass,"MC", "pl");
    SetStyleHistoTH1ForGraphs(histMCMass, "#it{E}_{PCM #gamma} (GeV)","Mass",textsize,textsize,textsize,textsize,0.5,0.4);
    DrawGammaSetMarker(histMCMass, markerStyle[1], 2., color[5], color[5]);
    histMCMass->DrawCopy("pl,same");
    legendmonitorAmplitude->Draw();

    canvasmonitor->cd();
    padmonitorMass->Draw();
    padmonitorTail->Draw();
    padmonitorAmplitude->Draw();

    canvasmonitor->Update();
    canvasmonitor->SaveAs(Form("%s/Monitor_%s.%s", outputDirSampleSummary.Data(), select.Data(), suffix.Data()));
    canvasmonitor->Clear();
    delete canvasmonitor;

    //*********************************************************************************************************************************
    //****************************** Fitting ratio of mean sigma position in MC/data ***************************************************
    //*********************************************************************************************************************************
    TF1* fFitConst          = new TF1("DataMCConst", "[0]" ,rangeHighPtFitRatio[0],rangeHighPtFitRatio[1]);
    TF1* fFitConstFull      = new TF1("ConstFullPtRange", "[0]" ,fBinsPt[ptBinRange[0]], fBinsPt[ptBinRange[1]]);
    histDataMCResults->Fit(fFitConst,"QRME0");
    histDataMCResults->Fit(fFitConstFull,"QRME0");
    Double_t highPtConst            = fixedOffSet;
    if (highPtConst == -1){
        highPtConst = fFitConst->GetParameter(0);
    }

    TF1* fFitConstFullInv   = new TF1("ConstFullPtRangeInverted", "[0]" ,fBinsPt[ptBinRange[0]], fBinsPt[ptBinRange[1]]);
    fFitConstFullInv->SetParameter(0, 1./fFitConstFull->GetParameter(0));

    // creating real fit functions
    fFitsigma = FitDataMC(histDataMCResults, fBinsPt[ptBinRange[0]], fBinsPt[ptBinRange[1]], select,  highPtConst, mode);
    fFitsigma2 = FitDataMC(histDataMCResults, fBinsPt[ptBinRange[0]], fBinsPt[ptBinRange[1]], select,  highPtConst, mode,1);
    fFitsigma3 = FitDataMC(histDataMCResults, fBinsPt[ptBinRange[0]], fBinsPt[ptBinRange[1]], select,  highPtConst, mode,2);




    fstream fLog;
    fLog.open(Form("%s/CorrectPCMSmearing_%s.log",outputDirSampleSummary.Data(),select.Data()), ios::out);
    fLog << "-----------------------------------" << endl;
    fLog << "Exponential function fitted" << endl;
    fLog << "-----------------------------------" << endl;
    fLog << "FitDataMC results ([0]+[3]*TMath::Exp([1]+([2]*x))" << endl;
    for(Int_t i=0;i<=3;i++) fLog << "Par " << i << ": " << fFitsigma->GetParameter(i) << " +- " << fFitsigma->GetParError(i) << endl;
    fLog << "-----------------------------------" << endl;
    fLog << "Pol2 fitted, results:" << endl;
    fLog << "-----------------------------------" << endl;
    for(Int_t i=0;i<=2;i++) fLog << "Par " << i << ": " << fFitsigma2->GetParameter(i) << " +- " << fFitsigma2->GetParError(i) << endl;
    fLog << "-----------------------------------" << endl;
    fLog << "Pol1 fitted, results:" << endl;
    fLog << "-----------------------------------" << endl;
    for(Int_t i=0;i<=1;i++) fLog << "Par " << i << ": " << fFitsigma3->GetParameter(i) << " +- " << fFitsigma3->GetParError(i) << endl;
    cout << "-----------------------------------" << endl;
    cout << "Exponential function fitted" << endl;
    cout << "-----------------------------------" << endl;
    cout << "FitDataMC results ([0]+[3]*TMath::Exp([1]+([2]*x))" << endl;
    for(Int_t i=0;i<=3;i++) cout << "Par " << i << ": " << fFitsigma->GetParameter(i) << " +- " << fFitsigma->GetParError(i) << endl;
    cout << "-----------------------------------" << endl;
    cout << "Pol2 fitted, results:" << endl;
    cout << "-----------------------------------" << endl;
    for(Int_t i=0;i<=2;i++) cout << "Par " << i << ": " << fFitsigma2->GetParameter(i) << " +- " << fFitsigma2->GetParError(i) << endl;
    cout << "-----------------------------------" << endl;
    cout << "Pol1 fitted, results:" << endl;
    cout << "-----------------------------------" << endl;
    for(Int_t i=0;i<=1;i++) cout << "Par " << i << ": " << fFitsigma3->GetParameter(i) << " +- " << fFitsigma3->GetParError(i) << endl;
    cout << "-----------------------------------" << endl;
    cout << "-----------------------------------" << endl;
    fLog << WriteParameterToFile(fFitConst) << endl;
    fLog << WriteParameterToFile(fFitConstFull) << endl;
    fLog << WriteParameterToFile(fFitConstFullInv) << endl;
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
    SetStyleHistoTH2ForGraphs(histoDummyDataMCRatio, "#it{E}_{PCM #gamma} (GeV)"," #sigma_{E_{#gamma PCM}}^{Data MC} relativ change", 0.035, 0.043, 0.035, 0.043, 0.82, 0.9);
    // SetStyleHistoTH2ForGraphs(histoDummyDataMCRatio, "#it{E}_{PCM #gamma} (GeV)"," #Delta #sigma_{#pi^{0} (data-MC)} * 4 * #sqrt{2} / (1-cos(#Theta)) #frac{ #sigma_{m_{inv}PCMEDC} }{ #sigma_{m_{inv}PCM} } ", 0.035, 0.043, 0.035, 0.043, 0.82, 0.9);
    histoDummyDataMCRatio->GetXaxis()->SetMoreLogLabels();
    histoDummyDataMCRatio->GetXaxis()->SetLabelOffset(-0.01);
    histoDummyDataMCRatio->DrawCopy("");

    DrawGammaSetMarker(histDataMCResults, 20, 1, color[0], color[0]);
//     histDataMCResults->Draw("same");

    Int_t nCorrections      = 3;
    if (isNotFirstIte){
        DrawGammaSetMarkerTF1( fFitConstFull, 9, 2, 807);
        fFitConstFull->Draw("same");
        nCorrections++;
    }

    fFitsigma->SetRange(fBinsPt[ptBinRange[0]]/1.5, fBinsPt[ptBinRange[1]]*1.5);
    fFitsigma2->SetRange(fBinsPt[ptBinRange[0]]/1.5, fBinsPt[ptBinRange[1]]*1.5);
    fFitsigma3->SetRange(fBinsPt[ptBinRange[0]]/1.5, fBinsPt[ptBinRange[1]]*1.5);

    DrawGammaSetMarkerTF1( fFitsigma, 1, 2, kRed+1);
    DrawGammaSetMarkerTF1( fFitsigma2, 7, 2, kGreen+2);
    DrawGammaSetMarkerTF1( fFitsigma3, 8, 2, kBlue+2);

    fFitsigma->Draw("same");
    fFitsigma2->Draw("same");
    fFitsigma3->Draw("same");
    histDataMCResults->Draw("same,pe");

    PutProcessLabelAndEnergyOnPlot(0.94, 0.96, 0.03, fCollisionSystem.Data(), fTextMeasurement.Data(), recGamma.Data(), 42, 0.03, "", 1, 1.25, 31);
    for (Int_t i = 0; i < nSets; i++){
       PutProcessLabelAndEnergyOnPlot(0.12, 0.945-2*0.03*(i), 0.03, fPlotLabelsRatio[i].Data(),"", "", 42, 0.03, "", 1, 1.25, 11);
    }

    TLegend* legendCorrectionFunctions = GetAndSetLegend2(0.125,0.8-nCorrections*1.1*0.03, 0.4,0.8, 0.03, 1, "", 42, 0.15);
    legendCorrectionFunctions->AddEntry(fFitsigma,"Exponential function fitted","l");
    legendCorrectionFunctions->AddEntry(fFitsigma2,"pol2 fitted","l");
    legendCorrectionFunctions->AddEntry(fFitsigma3,"pol1 fitted","l");
    if(isNotFirstIte) legendCorrectionFunctions->AddEntry(fFitConstFull,"Constant fitted","l");
    legendCorrectionFunctions->Draw();

    DrawGammaLines(fBinsPt[ptBinRange[0]]/1.5, fBinsPt[ptBinRange[1]]*1.5, 1.0, 1.0, 1, kGray+2, 2);

    canvassigmaRatioMCData->Update();
    canvassigmaRatioMCData->SaveAs(Form("%s/SigmaDiff_%s.%s", outputDirSampleSummary.Data(), select.Data(), suffix.Data()));
    canvassigmaRatioMCData->Clear();

    //*******************************************************************************
    // plotting total correction
    //*******************************************************************************
    canvassigmaRatioMCData->cd();
    Double_t maxXTotalCorrPlot = 50;
    if(select.Contains("pPb")) maxXTotalCorrPlot = 200;
    TH1D* totalCorrection = new TH1D("Total Correction","; #it{E}_{PCM #gamma} (GeV); correction factor",1000,0.3,maxXTotalCorrPlot);
    SetStyleHistoTH1ForGraphs(totalCorrection, "#it{E}_{PCM #gamma} (GeV)","correction factor",0.035,0.043, 0.035,0.043, 1.,0.9);
    totalCorrection->GetYaxis()->SetRangeUser(minPlotY+0.031,1.1);
    if(select.Contains("LHC11cd") && mode==2)totalCorrection->GetYaxis()->SetRangeUser(0.98,1.15);
    totalCorrection->GetXaxis()->SetMoreLogLabels();
    totalCorrection->GetXaxis()->SetLabelOffset(-0.01);
    SetLogBinningXTH(totalCorrection);
    totalCorrection->DrawCopy("p");


    fFitConstFullInv->SetRange(0.3,maxXTotalCorrPlot);

    DrawGammaSetMarkerTF1( fFitConstFullInv, 9, 2, 807);

    if(isNotFirstIte) fFitConstFullInv->Draw("same");



    TLegend *legend2 = GetAndSetLegend2(0.45,0.15, 0.725,0.15+nCorrections*1.1*0.03, 0.03, 1, "", 42, 0.15);//GetAndSetLegend2(0.2, 0.2, 0.4, 0.29, 0.03, 1, "", 42);
    if(isNotFirstIte) legend2->AddEntry(fFitConstFullInv,"Constant fitted","l");
    legend2->Draw("same");

    DrawGammaLines(0.3, maxXTotalCorrPlot,1.0, 1.0, 1, kGray+2, 2);

    PutProcessLabelAndEnergyOnPlot(0.94, 0.96, 0.03, fCollisionSystem.Data(), fTextMeasurement.Data(), recGamma.Data(), 42, 0.03, "", 1, 1.25, 31);
    for (Int_t i = 0; i < nSets; i++){
       PutProcessLabelAndEnergyOnPlot(0.12, 0.945-2*0.03*(i), 0.03, fPlotLabelsRatio[i].Data(),"", "", 42, 0.03, "", 1, 1.25, 11);
    }

    canvassigmaRatioMCData->Update();
    canvassigmaRatioMCData->SaveAs(Form("%s/TotalCorrection_%s.%s", outputDir.Data(), select.Data(), suffix.Data()));
    canvassigmaRatioMCData->Clear();
    delete canvassigmaRatioMCData;

    cout << "-----------------------------------------------------" << endl;
    cout << "-----------------------------------------------------" << endl;

    histDataResults->Write("Mean sigma Data");
    histMCResults->Write("Mean sigma MC");
    histDataResults->Write();
    histMCResults->Write();
    histDataMCResults->Write("MeansigmaRatioMCData");
    fFitConstFullInv->Write("CONST_TotalCorr");

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
        fFitReco = new TF1("DataMC", "[0]+[3]*TMath::Exp([1]+([2]*x))" ,minFit,maxFit);
        fFitReco->SetParameter(0,-2.43458e-06);
        fFitReco->SetParLimits(1,0.,10000000.);
        fFitReco->SetParameter(1,0.886265);
        fFitReco->SetParameter(2,-24.0344);
        fFitReco->SetParameter(3,-0.0405148);
    } else if (fitmode == 1) {
        fFitReco = new TF1("DataMC", "[0]+[1]*x+[2]*x*x" ,minFit,maxFit);
        fFitReco->SetParameter(0, -0.000117765);
        fFitReco->SetParameter(1, 0.000215411);
        fFitReco->SetParameter(2, 1.5076e-06);
    } else {
        fFitReco = new TF1("DataMC", "[0]+[1]*x" ,minFit,maxFit);
        fFitReco->SetParameter(0, -0.000123638);
        fFitReco->SetParameter(1, 0.000222327);
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
TF1* FitGaussian(TH1D* histo, Double_t fitRangeMin, Double_t fitRangeMax, Int_t mode, Double_t ptcenter , Int_t iDataMC){

    Double_t mesonAmplitude = 0;
    for(Int_t i = histo->FindBin(0.07); i < histo->FindBin(0.2) ; i++ ){
      if(histo->GetBinContent(i) > mesonAmplitude) mesonAmplitude = histo->GetBinContent(i);
    }
    cout << "mesonAmplitude = " << mesonAmplitude << endl;
    Double_t mesonAmplitudeMin;
    Double_t mesonAmplitudeMax;

    // special setting for PCM-EMC
    if(iDataMC==0) mesonAmplitudeMin = mesonAmplitude*90./100.;
    if(iDataMC>0)  mesonAmplitudeMin = mesonAmplitude*80./100.;

    mesonAmplitudeMax = mesonAmplitude*110./100.;

    cout << "mesonAmplitudeMin = " << mesonAmplitudeMin << endl;
    cout << "mesonAmplitudeMax = " << mesonAmplitudeMax << endl;

    Double_t rmsScaling = 0.4;
    if(ptcenter > 3.0) rmsScaling = 0.7;
    if(ptcenter > 5.0) rmsScaling = 1.0;
    Double_t rmsHisto = histo->GetRMS();
    if(rmsHisto<0.01) rmsHisto = 0.01;

    // necessary to exclude low sigma peak
    histo->GetXaxis()->SetRangeUser(0.07,0.2);

    fitRangeMin = histo->GetBinCenter(histo->GetMaximumBin())-rmsScaling*rmsHisto;
    fitRangeMax = histo->GetBinCenter(histo->GetMaximumBin())+rmsScaling*rmsHisto;
    histo->GetXaxis()->SetRangeUser(0.00,0.3);

    // TF1* fFitReco    = new TF1("fGaussExp","(x<[1])*([0]*(TMath::Exp(-0.5*((x-[1])/[2])^2)+TMath::Exp((x-[1])/[3])*(1.-TMath::Exp(-0.5*((x-[1])/[2])^2)))+[4]+[5]*x)+(x>=[1])*([0]*TMath::Exp(-0.5*((x-[1])/[2])^2)+[4]+[5]*x)",
    TF1* fFitReco    = new TF1("fGauss","[0]*TMath::Exp(-0.5*((x-[1])/[2])^2)+[3]", fitRangeMin, fitRangeMax);
    Double_t fMesonsigmaExpect = TDatabasePDG::Instance()->GetParticle(111)->Mass();

    fFitReco->SetParameter(0, mesonAmplitude);
    fFitReco->SetParameter(1, fMesonsigmaExpect);
    fFitReco->SetParameter(2, 0.01);
    fFitReco->SetParameter(3, 0);

    fFitReco->SetParLimits(0, mesonAmplitudeMin, mesonAmplitudeMax);
    if (mode == 4 || mode == 12 || mode == 15){
        fFitReco->SetParLimits(1, fMesonsigmaExpect*0.65, fMesonsigmaExpect*1.6);
    } else {
        fFitReco->SetParLimits(1, fMesonsigmaExpect*0.9, fMesonsigmaExpect*1.1);
    }
    fFitReco->SetParLimits(2, 0.001, 0.1);


    histo->Fit(fFitReco,"QRME0");
    Double_t rmsScalingRefit = 1.1;
    if(ptcenter > 3.0) rmsScalingRefit = 1.3;
    if(ptcenter > 5.0) rmsScalingRefit = 1.6;
    fFitReco->SetRange(fFitReco->GetParameter(1)-rmsScalingRefit*fFitReco->GetParameter(2),fFitReco->GetParameter(1)+rmsScalingRefit*fFitReco->GetParameter(2));
    histo->Fit(fFitReco,"QRME0","",fFitReco->GetParameter(1)-rmsScalingRefit*fFitReco->GetParameter(2),fFitReco->GetParameter(1)+rmsScalingRefit*fFitReco->GetParameter(2));

    fFitReco->SetLineColor(kRed+1);
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
TF1* FitExpPlusGaussian(TH1D* histo, Double_t fitRangeMin, Double_t fitRangeMax, Int_t mode, Double_t ptcenter , Int_t iDataMC){

    Double_t mesonAmplitude = 0;
    for(Int_t i = histo->FindBin(0.07); i < histo->FindBin(0.2) ; i++ ){
      if(histo->GetBinContent(i) > mesonAmplitude) mesonAmplitude = histo->GetBinContent(i);
    }
    cout << "mesonAmplitude = " << mesonAmplitude << endl;
    Double_t mesonAmplitudeMin;
    Double_t mesonAmplitudeMax;

    // special setting for PCM-EMC
    if (mode == 2 || mode == 14){
        if(iDataMC==0) mesonAmplitudeMin = mesonAmplitude*90./100.;
        if(iDataMC>0) mesonAmplitudeMin = mesonAmplitude*80./100.;
        // if(iDataMC>0&& ptcenter > 14.5) mesonAmplitudeMin = mesonAmplitude*65./100.;
        // if (ptcenter > 4.5)
        //     mesonAmplitudeMin = mesonAmplitude*55./100.;
        // else
        //     mesonAmplitudeMin = mesonAmplitude*80./100.;
        // if(iDataMC>0) mesonAmplitudeMin = mesonAmplitude*95./100.;
        // if(iDataMC>0&& ptcenter > 14.5) mesonAmplitudeMin = mesonAmplitude*65./100.;
    // special setting for PCM-PHOS
    } else if (mode == 3){
        if (ptcenter > 1.)
            mesonAmplitudeMin = mesonAmplitude*20./100.;
        else
            mesonAmplitudeMin = mesonAmplitude*10./100.;

          fitRangeMin = 0.09;
          fitRangeMax = 0.19;
    // special setting for EMC
    } else if (mode == 4 || mode == 12 || mode == 15){
        mesonAmplitudeMin = mesonAmplitude*20./100.;
        fitRangeMin = 0.08;
    // special setting for PHOS
    } else if (mode == 5){
      fitRangeMin = 0.09;
      fitRangeMax = 0.25;
        if (ptcenter > 1.)
            mesonAmplitudeMin = mesonAmplitude*80./100.;
        else
            mesonAmplitudeMin = mesonAmplitude*1./100.;

        if(ptcenter < 0.35) fitRangeMax = 0.2;
    } else {
        mesonAmplitudeMin = mesonAmplitude*50./100.;
    }
    mesonAmplitudeMax = mesonAmplitude*110./100.;

    cout << "mesonAmplitudeMin = " << mesonAmplitudeMin << endl;
    cout << "mesonAmplitudeMax = " << mesonAmplitudeMax << endl;

    TF1* fFitReco    = new TF1("fGaussExp","(x<[1])*([0]*(TMath::Exp(-0.5*((x-[1])/[2])^2)+TMath::Exp((x-[1])/[3])*(1.-TMath::Exp(-0.5*((x-[1])/[2])^2)))+[4]+[5]*x)+(x>=[1])*([0]*TMath::Exp(-0.5*((x-[1])/[2])^2)+[4]+[5]*x)",
                               fitRangeMin, fitRangeMax);
    Double_t fMesonsigmaExpect = TDatabasePDG::Instance()->GetParticle(111)->Mass();

    fFitReco->SetParameter(0, mesonAmplitude);
    fFitReco->SetParameter(1, fMesonsigmaExpect);
    fFitReco->SetParameter(2, 0.01);
    fFitReco->SetParameter(3, 0.012);

    fFitReco->SetParLimits(0, mesonAmplitudeMin, mesonAmplitudeMax);
    if (mode == 4 || mode == 12 || mode == 15){
        fFitReco->SetParLimits(1, fMesonsigmaExpect*0.65, fMesonsigmaExpect*1.6);
    } else {
        fFitReco->SetParLimits(1, fMesonsigmaExpect*0.9, fMesonsigmaExpect*1.1);
    }
    fFitReco->SetParLimits(2, 0.001, 0.1);
    fFitReco->SetParLimits(3, 0.001, 0.09);
    if (mode == 5){
      fFitReco->SetParLimits(1, fMesonsigmaExpect*0.8, fMesonsigmaExpect*1.8);
      fFitReco->SetParLimits(2, 0.001, 0.01);
    }

    histo->Fit(fFitReco,"QRME0");
    histo->Fit(fFitReco,"QRME0");

    fFitReco->SetLineColor(kRed+1);
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
TF1* FitDExpPlusGaussian(TH1D* histo, Double_t fitRangeMin, Double_t fitRangeMax, Int_t mode, Double_t ptcenter, Int_t iDataMC ){


    Double_t mesonAmplitude = 0;
    for(Int_t i = histo->FindBin(0.07); i < histo->FindBin(0.2) ; i++ ){
      if(histo->GetBinContent(i) > mesonAmplitude) mesonAmplitude = histo->GetBinContent(i);
    }
    cout << "mesonAmplitude = " << mesonAmplitude << endl;
    Double_t mesonAmplitudeMin;
    Double_t mesonAmplitudeMax;

    // special setting for PCM-EMC
    if (mode == 2 || mode == 14){
        if (ptcenter > 1.5)
            mesonAmplitudeMin = mesonAmplitude*95./100.;
        else
            mesonAmplitudeMin = mesonAmplitude*90./100.;
    // special setting for PCM-PHOS
    } else if (mode == 3){
        if (ptcenter > 1.)
            mesonAmplitudeMin = mesonAmplitude*20./100.;
        else
            mesonAmplitudeMin = mesonAmplitude*10./100.;

          //fitRangeMin = 0.09; // The Framework is designed to Load this function with defined Ranges for each Setting; see around line 700; The needed Variable is minMax[0] and minMax[1]
          //fitRangeMax = 0.19;
    // special setting for EMC
    } else if (mode == 4 || mode == 12 || mode == 15){
        mesonAmplitudeMin = mesonAmplitude*20./100.;
        fitRangeMin = 0.08;
    // special setting for PHOS
    } else if (mode == 5){
      //fitRangeMin = 0.09;// The Framework is designed to Load this function with defined Ranges for each Setting; see around line 700; The needed Variable is minMax[0] and minMax[1]
      //fitRangeMax = 0.25;
        if (ptcenter > 1.)
            mesonAmplitudeMin = mesonAmplitude*80./100.;
        else
            mesonAmplitudeMin = mesonAmplitude*1./100.;

        if(ptcenter < 0.35) fitRangeMax = 0.2;
    } else {
        mesonAmplitudeMin = mesonAmplitude*50./100.;
    }
    mesonAmplitudeMax = mesonAmplitude*110./100.;

    cout << "mesonAmplitudeMin = " << mesonAmplitudeMin << endl;
    cout << "mesonAmplitudeMax = " << mesonAmplitudeMax << endl;

    TF1* fFitReco = new TF1("GaussExpLinear","(x<[1])*([0]*(TMath::Exp(-0.5*((x-[1])/[2])^2)+TMath::Exp((x-[1])/[3])*(1.-TMath::Exp(-0.5*((x-[1])/[2])^2)))+[4]+[5]*x)+(x>=[1])*([0]*(TMath::Exp(-0.5*((x-[1])/[2])^2)+TMath::Exp(-(x-[1])/[6])*(1.-TMath::Exp(-0.5*((x-[1])/[2])^2)))+[4]+[5]*x)",fitRangeMin, fitRangeMax);
    Double_t fMesonsigmaExpect = TDatabasePDG::Instance()->GetParticle(111)->Mass();

    fFitReco->SetParameter(0, mesonAmplitude);
    fFitReco->SetParameter(1, fMesonsigmaExpect);
    fFitReco->SetParameter(2, 0.01);
    fFitReco->SetParameter(3, 0.012);

    fFitReco->SetParameter(6,0.015);
    if(iDataMC)
        fFitReco->SetParLimits(6,0.010,0.030);

    fFitReco->SetParLimits(0, mesonAmplitudeMin, mesonAmplitudeMax);
    if (mode == 4 || mode == 12 || mode == 15){
        fFitReco->SetParLimits(1, fMesonsigmaExpect*0.65, fMesonsigmaExpect*1.6);
    } else {
        fFitReco->SetParLimits(1, fMesonsigmaExpect*0.9, fMesonsigmaExpect*1.1);
    }
    fFitReco->SetParLimits(2, 0.001, 0.1);
    fFitReco->SetParLimits(3, 0.001, 0.18);
    if (mode == 5){
      fFitReco->SetParLimits(1, fMesonsigmaExpect*0.8, fMesonsigmaExpect*1.8);
      fFitReco->SetParLimits(2, 0.001, 0.01);
    }

    histo->Fit(fFitReco,"QRME0");

    fFitReco->SetLineColor(kRed+1);
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
