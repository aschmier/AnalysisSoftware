//**************************************************************************************************
//**************************** CompareDifferentDirectories *****************************************
//**************************************************************************************************

/***************************************************************************************************
 ******                         provided by PCM Group, PWGGA,                                  *****
 ******                         Friederike Bock, friederike.bock@cern.ch                       *****
 ****************************************************************************************************/

#include <Riostream.h>
#include <fstream>
#include "TMath.h"
#include <stdio.h>
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
#include "TLatex.h"
#include "TASImage.h"
#include "TPostScript.h"
#include "TGraphErrors.h"
#include "TArrow.h"
#include "TMarker.h"
#include "TGraphAsymmErrors.h"
#include "../CommonHeaders/PlottingGammaConversionHistos.h"
#include "../CommonHeaders/PlottingGammaConversionAdditional.h"
#include "../CommonHeaders/FittingGammaConversion.h"
#include "../CommonHeaders/ConversionFunctionsBasicsAndLabeling.h"
#include "../CommonHeaders/ConversionFunctions.h"
#include "../CommonHeaders/CombinationFunctions.h"

struct SysErrorConversion {
   Double_t value;
   Double_t error;
   //   TString name;
};

void CompareDifferentDirectories(   TString FolderList              = "",
                                    TString suffix                  = "gif",
                                    TString meson                   = "",
                                    Bool_t kIsMC                    = 0,
                                    TString optionEnergy            = "",
                                    Int_t NumberOfCuts              = 1,
                                    TString optionPeriod            = "No",
                                    Int_t mode                      = 9,
                                    TString cutVariationName        = "NonLinearity",
                                    Bool_t setFullPathInInputFile   = kFALSE
                                ){
    Int_t DebugOutputLevel = 0;
    if (DebugOutputLevel>=1){cout << "Debug; CompareDifferentDirectories.C, line " << __LINE__ <<
                                     "; FolderList: " << FolderList.Data() << "; suffix: " << suffix.Data() << "; meson: " << meson.Data() <<
                                     "; kIsMC: " << kIsMC << "; optionEnergy: "<< optionEnergy.Data() << "; NumberOfCuts: " << NumberOfCuts <<
                                     "; optionPeriod: " << optionPeriod.Data() << "; mode: " << mode << "; cutVariationName: " << cutVariationName.Data() <<
                                     "; setFullPathInInputFile: " << setFullPathInInputFile << endl;}

    //Number of Mode
    Double_t modePCM_Omega=60;
    Double_t modePCMEMCAL_Omega=61;
    Double_t modePCMPHOS_Omega=62;
    Double_t modeEMCAL_Omega=64;
    Double_t modePHOS_Omega=65;

    //Enable or Disable certain histograms
    Bool_t doCorrectedYieldWOSecCut;
    Bool_t doAlternativeEfficiency;
    Bool_t doAlternativeCorrectedYield;
    Bool_t doMCInput;
    Bool_t doCorrectedYieldAverage;
    Int_t useOmegaEfficiency=2;
    if ( mode == modePCM_Omega || mode == modePCMEMCAL_Omega || mode == modePCMPHOS_Omega || mode == modeEMCAL_Omega || mode == modePHOS_Omega ){
        doCorrectedYieldWOSecCut            = kFALSE;
        if (useOmegaEfficiency==2){
            doAlternativeCorrectedYield     = kTRUE;
            doAlternativeEfficiency         = kTRUE;
            doCorrectedYieldAverage         = kTRUE;
            doMCInput                       = kTRUE;
        } else {
            doAlternativeCorrectedYield     = kFALSE;
            doAlternativeEfficiency         = kFALSE;
            doCorrectedYieldAverage         = kFALSE;
            doMCInput                       = kFALSE;
        }
    } else {
        doCorrectedYieldWOSecCut            = kTRUE;
        doAlternativeCorrectedYield         = kFALSE;
        doAlternativeEfficiency             = kFALSE;
        doCorrectedYieldAverage             = kFALSE;
        doMCInput                           = kFALSE;
    }

    if (kIsMC==kFALSE){
        doMCInput                           = kFALSE;
    }
    // Initialize arrays
    TString fileDirectory[50];
    TString cutNumber[50];
    TString cutStringsName[50];

    if (DebugOutputLevel>=1){cout << "Debug; CompareDifferentDirectories.C, line " << __LINE__ << "; " << endl;}
    // prepare nice plotting algorithms
    StyleSettingsThesis();
    SetPlotStyle();

    // Set output folder
    TString outputDir = Form("CutStudies/%s/%s",optionEnergy.Data(), cutVariationName.Data());
    TString outputDirRootFile = Form("CutStudies/%s",optionEnergy.Data());
    gSystem->Exec("mkdir -p "+outputDir);

    // Set meson name for plotting
    TString textMeson;
    Bool_t isEta                                                = kFALSE;
    if (meson.CompareTo("Pi0")==0 || meson.CompareTo("Pi0EtaBinning")==0){
        textMeson                                               = "#pi^{0}";
    } else if (meson.CompareTo("Omega")==0){
        textMeson                                               = "#omega";
    } else {
        textMeson                                               = "#eta";
        isEta                                                   = kTRUE;
    }
    // Define input and output MC/data
    TString prefix2                                             = "";
    if (kIsMC){
        prefix2                                                 = "MC";
    } else {
        prefix2                                                 = "data";
    }
    Bool_t isEDC        = kFALSE;

    if (DebugOutputLevel>=1){cout << "Debug; CompareDifferentDirectories.C, line " << __LINE__ << "; " << endl;}
    // Set collisions system
    TString collisionSystem     = ReturnFullCollisionsSystem(optionEnergy);
    if (collisionSystem.CompareTo("") == 0){
        cout << "No correct collision system specification, has been given" << endl;
        return;
    }
    TString detectionProcess    = ReturnFullTextReconstructionProcess(mode);
    TString process             = Form("%s #rightarrow #gamma#gamma", textMeson.Data());

    // Define colors for comparisons
    Color_t color[20] = {kBlack, kAzure, kGreen+2,kOrange+2,kRed, kViolet,  kBlue-9, kSpring+10,
                        kCyan+3, kCyan-10, kCyan, kGreen+4, kGreen-9,
                        kGreen,  kYellow+4, kYellow+3, kMagenta+4,
                        kMagenta-8, kGray, kGray+3};

    // read folder and name from file
    ifstream in(FolderList.Data());
    cout<<"Available Cuts:"<<endl;
    TString folderName;
    TString cutName;
    TString cutNr;
    Int_t Number = 0;
    while(!in.eof() ){
        in >> folderName >> cutNr >> cutName;
        cutName.ReplaceAll("_"," ");
        fileDirectory[Number] = folderName;
        cutStringsName[Number] = cutName;
        cutNumber[Number] = cutNr;
        cout<< fileDirectory[Number]<< "\t" << cutNumber[Number]<< "\t"<< cutStringsName[Number] <<endl;
        Number++;
    }
    cout<<"=========================="<<endl;

    // Definition of necessary histogram arrays
    if (DebugOutputLevel>=1){cout << "Debug; CompareDifferentDirectories.C, line " << __LINE__ << "; " << endl;}
    const Int_t ConstNumberOfCuts = 22;
    TString FileNameCorrected[ConstNumberOfCuts];
    TString FileNameUnCorrected[ConstNumberOfCuts];

    TFile *Cutcorrfile[ConstNumberOfCuts];
    TFile *Cutuncorrfile[ConstNumberOfCuts];

    TH1D *histoCorrectedYieldCut[ConstNumberOfCuts];
    TH1D *histoCorrectedYieldWOSecCut[ConstNumberOfCuts];
    TH1D *histoAltCorrectedYieldCut[ConstNumberOfCuts];
    TH1D *histoMCInputYieldCut[ConstNumberOfCuts];
    TH1D *histoCorrectedYieldCutAverage;
    TH1D *histoCorrectedYieldCutAverage_MAD;
    TH1D *histoCorrectedYieldCutAverage_MAD_NormWStatErr;
    TH1D *histoCorrectedYieldCutMCInput_MAD;
    TH1D *histoCorrectedYieldCutMCInput_MAD_NormWStatErr;

    TH1D *histoAltCorrectedYieldCutAverage;
    TH1D *histoAltCorrectedYieldCutAverage_MAD;
    TH1D *histoAltCorrectedYieldCutAverage_MAD_NormWStatErr;
    TH1D *histoAltCorrectedYieldCutMCInput_MAD;
    TH1D *histoAltCorrectedYieldCutMCInput_MAD_NormWStatErr;

    TH1D *histoTrueEffiCut[ConstNumberOfCuts];
    TH1D *histoAltEffiCut[ConstNumberOfCuts];
    TH1D *histoAcceptanceCut[ConstNumberOfCuts];
    TH1D *histoRawYieldCut[ConstNumberOfCuts];
    TH1D *histoMassCut[ConstNumberOfCuts];
    TH1D *histoWidthCut[ConstNumberOfCuts];
    TH1D *histoSBCut[ConstNumberOfCuts];
    TH1D *histoClusterE[ConstNumberOfCuts];
    Bool_t isClusterE = kTRUE;

    TH1D *histoRatioCorrectedYieldCut[ConstNumberOfCuts];
    TH1D *histoRatioCorrectedYieldWOSecCut[ConstNumberOfCuts];
    TH1D *histoRatioAltCorrectedYieldCut[ConstNumberOfCuts];
    TH1D *histoRatioMCInputYieldCut[ConstNumberOfCuts];
    TH1D *histoRatioToCorYieldMCInputYieldCut[ConstNumberOfCuts];
    TH1D *histoRatioToAltCorYieldMCInputYieldCut[ConstNumberOfCuts];
    TH1D *histoRatioCorrectedYieldCutAverage;
    TH1D *histoRatioAltCorrectedYieldCutAverage;
    TH1D *histoRatioTrueEffiCut[ConstNumberOfCuts];
    TH1D *histoRatioAltEffiCut[ConstNumberOfCuts];
    TH1D *histoRatioTrueEffDivAltEffiCut[ConstNumberOfCuts];
    TH1D *histoRatioOfRatioTrueEffDivAltEffiCut[ConstNumberOfCuts];
    TH1D *histoRatioAcceptanceCut[ConstNumberOfCuts];
    TH1D *histoRatioRawYieldCut[ConstNumberOfCuts];
    TH1D *histoRatioMassCut[ConstNumberOfCuts];
    TH1D *histoRatioWidthCut[ConstNumberOfCuts];
    TH1D *histoRatioSBCut[ConstNumberOfCuts];
    TH1D *histoRatioClusterE[ConstNumberOfCuts];

    Double_t maxPt	= 0;

    Bool_t readCorrectedFile[ConstNumberOfCuts];
    Bool_t plotOnlyUncorrectedOutput = kFALSE;
    if (DebugOutputLevel>=1){cout << "Debug; CompareDifferentDirectories.C, line " << __LINE__ << "; " << endl;}

    Double_t PlotRangeYWidthMin=0.;
    Double_t PlotRangeYWidthMax=0.05;
    if ( mode == modePCM_Omega || mode == modeEMCAL_Omega || mode == modePHOS_Omega || mode == modePCMEMCAL_Omega || mode == modePCMPHOS_Omega ){
                PlotRangeYWidthMax=0.1;
    }

    for (Int_t i=0; i< NumberOfCuts; i++){

        cout << "-----------------------------------------------------------------------------------------" << endl;
        cout << "Cut Number: " << i << endl;
        // Decode individual cutnumber
        TString fEventCutSelection="";
        TString fGammaCutSelection="";
        TString fClusterCutSelection="";
        TString fElectronCutSelection="";
        TString fMesonCutSelection="";
        cout << cutNumber[i].Data() << endl;
        ReturnSeparatedCutNumberAdvanced(cutNumber[i].Data(),fEventCutSelection, fGammaCutSelection, fClusterCutSelection, fElectronCutSelection, fMesonCutSelection, mode);

        if (mode == 4 || mode == 2 || mode == modePCMEMCAL_Omega || mode == modeEMCAL_Omega ){
            if (fClusterCutSelection.BeginsWith("4")) isEDC = kTRUE;
        }
        // read file with corrections
        if(setFullPathInInputFile)
            FileNameCorrected[i] = Form("%s/%s_%s_GammaConvV1Correction_%s.root", fileDirectory[i].Data(), meson.Data(), prefix2.Data(), cutNumber[i].Data());
        else
            FileNameCorrected[i] = Form("%s%s/%s/%s_%s_GammaConvV1Correction_%s.root", fileDirectory[i].Data(), cutNumber[i].Data(), optionEnergy.Data(), meson.Data(), prefix2.Data(), cutNumber[i].Data());
        cout<< "FileNameCorrected["<<i<<"]: " << FileNameCorrected[i] << endl;
        Cutcorrfile[i] = new TFile(FileNameCorrected[i]);
        if (Cutcorrfile[i]->IsZombie()){
          readCorrectedFile[i] = kFALSE;
          plotOnlyUncorrectedOutput = kTRUE;
        }else readCorrectedFile[i] = kTRUE;
        // read file without corrections
        if(setFullPathInInputFile)
            FileNameUnCorrected[i] = Form("%s/%s_%s_GammaConvV1WithoutCorrection_%s.root", fileDirectory[i].Data(), meson.Data(), prefix2.Data(), cutNumber[i].Data());
        else
            FileNameUnCorrected[i] = Form("%s%s/%s/%s_%s_GammaConvV1WithoutCorrection_%s.root", fileDirectory[i].Data(), cutNumber[i].Data(), optionEnergy.Data(), meson.Data(), prefix2.Data(), cutNumber[i].Data());
        cout<< "FileNameUnCorrected[" << i << "]: " << FileNameUnCorrected[i] << endl;
        Cutuncorrfile[i] = new TFile(FileNameUnCorrected[i]);
        if (Cutuncorrfile[i]->IsZombie()) return;

        // Set correct histogram name for corrected yield and efficiency
        if (DebugOutputLevel>=2){cout << "Debug; CompareDifferentDirectories.C, line " << __LINE__ << "; Set correct histogram name for corrected yield and efficiency" << endl;}
        TString nameCorrectedYield;
        TString nameEfficiency;
        TString nameAlternativeEfficiency;
        TString nameCorrectedYieldWOSec;
        TString nameAlternativeCorrectedYield;
        TString nameMCInputYield;
        TString nameAcceptance;
        TString nameRawYield;
        TString nameMassCut;
        TString nameWidthCut;
        TString nameSBCut;
        TString nameClusterE;

        TString InvMassTypeEnding = "_SubPiZero";
        Bool_t useBackFitOutput=kTRUE;
        nameCorrectedYield = "CorrectedYieldTrueEff";
        nameEfficiency = "TrueMesonEffiPt";
        nameAlternativeEfficiency = "MesonEffiPt";
        nameCorrectedYieldWOSec = "CorrectedYieldWOSecTrueEff";
        nameAlternativeCorrectedYield="CorrectedYieldNormEff";
        nameAcceptance = "fMCMesonAccepPt";
        nameRawYield = "histoYieldMesonPerEvent";
        nameMassCut = "histoMassGaussianMeson";
        nameWidthCut = "histoFWHMMeson";
        nameSBCut = "histoSBdefaultMeson";
        nameClusterE = "ClusterEPerEvent";
        nameMCInputYield = "MCYield_Meson";
        TString bckfit = "";
        if ( mode == 4 || mode == 5 ){
            nameCorrectedYield = "CorrectedYieldNormEff";
            nameEfficiency = "MesonEffiPt";
            nameCorrectedYieldWOSec = "CorrectedYieldWOSecNormEff";
            nameAlternativeEfficiency="TrueMesonEffiPt";
            nameAlternativeCorrectedYield="CorrectedYieldTrueEff";
        } else if ( mode == modePCM_Omega || mode == modeEMCAL_Omega || mode == modePHOS_Omega || mode == modePCMEMCAL_Omega || mode == modePCMPHOS_Omega ){
            if(useOmegaEfficiency==1){
                nameCorrectedYield                          = Form("CorrectedYieldNormEff%s",InvMassTypeEnding.Data());
                nameEfficiency                              = Form("MesonEffiPtNorm%s",InvMassTypeEnding.Data());
                nameAlternativeEfficiency                   = Form("TrueMesonEffiPt%s",InvMassTypeEnding.Data());
                nameAlternativeCorrectedYield               = Form("CorrectedYieldTrueEff%s",InvMassTypeEnding.Data());
                if(useBackFitOutput){
                    cout << "using normal efficiency back fit" << endl;
                    nameCorrectedYield                          = Form("CorrectedYieldNormEffBackFit%s",InvMassTypeEnding.Data());
                    nameAlternativeCorrectedYield               = Form("CorrectedYieldTrueEffBackFit%s",InvMassTypeEnding.Data());
                    nameEfficiency                              = Form("MesonEffiPtNormBackFit%s",InvMassTypeEnding.Data());
                } else{
                    cout << "using normal efficiency event mixing" << endl;
                }
            } else if(useOmegaEfficiency==2){
                nameCorrectedYield                          = Form("CorrectedYieldTrueEff%s",InvMassTypeEnding.Data());
                nameEfficiency                              = Form("TrueMesonEffiPt%s",InvMassTypeEnding.Data());
                nameAlternativeEfficiency                   = Form("MesonEffiPtNorm%s",InvMassTypeEnding.Data());
                nameAlternativeCorrectedYield               = Form("CorrectedYieldNormEff%s",InvMassTypeEnding.Data());
                if(useBackFitOutput){
                    cout << "using true efficiency backfit" << endl;
                    nameCorrectedYield                          = Form("CorrectedYieldTrueEffBackFit%s",InvMassTypeEnding.Data());
                    nameAlternativeEfficiency                   = Form("MesonEffiPtNormBackFit%s",InvMassTypeEnding.Data());
                    nameAlternativeCorrectedYield               = Form("CorrectedYieldNormEffBackFit%s",InvMassTypeEnding.Data());
                } else{
                    cout << "using true efficiency event mixing" << endl;
                }
            } else {//case 0
                nameCorrectedYield                          = Form("CorrectedYieldAveragedEffi%s",InvMassTypeEnding.Data());
                nameEfficiency                              = Form("MesonEffiPtAveraged%s",InvMassTypeEnding.Data());
                if(useBackFitOutput){
                    cout << "using Averaged Effi Back Fit" << endl;
                    nameCorrectedYield                          = Form("CorrectedYieldAveragedEffiBackFit%s",InvMassTypeEnding.Data());
                    nameEfficiency                              = Form("MesonEffiPtAveraged%s",InvMassTypeEnding.Data());
                } else{
                    cout << "using Averaged Effi event mixing" << endl;
                }
                // averaged is for now always done with bck fit
            } //useOmegaEfficiency
            if (useBackFitOutput){
                bckfit = "BackFit";
            } else {
                bckfit = "";
            }
            nameMassCut                                  = Form("histoMassMeson%s%s",bckfit.Data(),InvMassTypeEnding.Data());
            nameWidthCut                                 = Form("histoFWHMMeson%s%s",bckfit.Data(),InvMassTypeEnding.Data());
            nameSBCut                                    = Form("histoSBdefaultMeson%s%s",bckfit.Data(),InvMassTypeEnding.Data());
            nameRawYield                                 = Form("histoYieldMesonPerEvent%s%s",bckfit.Data(),InvMassTypeEnding.Data());
        } //Omega Modes

        // Read histograms and rename them from the original files for each cut
        if (DebugOutputLevel>=2){cout << "Debug; CompareDifferentDirectories.C, line " << __LINE__ << "; Read histograms and rename them from the original files for each cut" << endl;}
        if(readCorrectedFile[i]){
          if (DebugOutputLevel>=2){cout << "Debug; CompareDifferentDirectories.C, line " << __LINE__ << "; histoCorrectedYieldCut: " << nameCorrectedYield.Data() << endl;}
          histoCorrectedYieldCut[i]   = (TH1D*)Cutcorrfile[i]->Get(nameCorrectedYield.Data());
          histoCorrectedYieldCut[i]->SetName(Form("%s_%s",nameCorrectedYield.Data(),cutStringsName[i].Data()));
          if (doCorrectedYieldWOSecCut){
              if (DebugOutputLevel>=2){cout << "Debug; CompareDifferentDirectories.C, line " << __LINE__ << "; readCorrectedFile histoCorrectedYieldWOSecCut: " << nameCorrectedYieldWOSec.Data() << endl;}
              histoCorrectedYieldWOSecCut[i]   = (TH1D*)Cutcorrfile[i]->Get(nameCorrectedYieldWOSec.Data());
              histoCorrectedYieldWOSecCut[i]->SetName(Form("%s_%s",nameCorrectedYieldWOSec.Data(),cutStringsName[i].Data()));
          } else {
              histoCorrectedYieldWOSecCut[i]   = NULL;
          }
          if (doAlternativeCorrectedYield){
              if (DebugOutputLevel>=2){cout << "Debug; CompareDifferentDirectories.C, line " << __LINE__ << "; histoRatioAltCorrectedYieldCut: " << nameAlternativeCorrectedYield.Data() << endl;}
              histoAltCorrectedYieldCut[i]   = (TH1D*)Cutcorrfile[i]->Get(nameAlternativeCorrectedYield.Data());
              histoAltCorrectedYieldCut[i]->SetName(Form("%s_%s",nameAlternativeCorrectedYield.Data(),cutStringsName[i].Data()));
          } else {
              histoAltCorrectedYieldCut[i]   = NULL;
          }
          if (doMCInput){
              if (DebugOutputLevel>=2){cout << "Debug; CompareDifferentDirectories.C, line " << __LINE__ << "; MCInputYield : " << nameMCInputYield.Data() << endl;}
              histoMCInputYieldCut[i]         = (TH1D*)Cutcorrfile[i]->Get(nameMCInputYield.Data());
              histoMCInputYieldCut[i]->SetName(Form("%s_%s",nameMCInputYield.Data(), cutStringsName[i].Data()));
          } else {
              histoMCInputYieldCut[i]   = NULL;
          }
          if (DebugOutputLevel>=2){cout << "Debug; CompareDifferentDirectories.C, line " << __LINE__ << "; histoTrueEffiCut : " << nameEfficiency.Data() << endl;}
          histoTrueEffiCut[i]         = (TH1D*)Cutcorrfile[i]->Get(nameEfficiency.Data());
          histoTrueEffiCut[i]->SetName(Form("%s_%s",nameEfficiency.Data(), cutStringsName[i].Data()));
          if (doAlternativeEfficiency){
              if (DebugOutputLevel>=2){cout << "Debug; CompareDifferentDirectories.C, line " << __LINE__ << "; histoAltEffiCut : " << nameAlternativeEfficiency.Data() << endl;}
              histoAltEffiCut[i]         = (TH1D*)Cutcorrfile[i]->Get(nameAlternativeEfficiency.Data());
              histoAltEffiCut[i]->SetName(Form("%s_%s",nameAlternativeEfficiency.Data(), cutStringsName[i].Data()));
          }
          if (DebugOutputLevel>=2){cout << "Debug; CompareDifferentDirectories.C, line " << __LINE__ << "; histoAcceptanceCut : " << nameAcceptance.Data() << endl;}
          histoAcceptanceCut[i]       =(TH1D*)Cutcorrfile[i]->Get(nameAcceptance.Data());
          histoAcceptanceCut[i]->SetName(Form("AcceptPt_%s", cutStringsName[i].Data()));
        }
        if (DebugOutputLevel>=2){cout << "Debug; CompareDifferentDirectories.C, line " << __LINE__ << "; histoRawYieldCut: " << nameRawYield.Data() << endl;}
        histoRawYieldCut[i]         = (TH1D*)Cutuncorrfile[i]->Get(nameRawYield.Data());
        histoRawYieldCut[i]->SetName(Form("histoYieldMesonPerEvent_%s",cutStringsName[i].Data()));
        if (DebugOutputLevel>=2){cout << "Debug; CompareDifferentDirectories.C, line " << __LINE__ << "; histoMassCut: " << nameMassCut.Data() << endl;}
        histoMassCut[i]             = (TH1D*)Cutuncorrfile[i]->Get(nameMassCut.Data());
        histoMassCut[i]->SetName(Form("histoMassGaussianMeson_%s",cutStringsName[i].Data()));
        if (DebugOutputLevel>=2){cout << "Debug; CompareDifferentDirectories.C, line " << __LINE__ << "; histoWidthCut: " << nameWidthCut.Data() << endl;}
        histoWidthCut[i]             = (TH1D*)Cutuncorrfile[i]->Get(nameWidthCut.Data());
        histoWidthCut[i]->SetName(Form("histoWidthGaussianMeson_%s",cutStringsName[i].Data()));
        if (DebugOutputLevel>=2){cout << "Debug; CompareDifferentDirectories.C, line " << __LINE__ << "; histoSBCut: " << nameSBCut.Data() << endl;}
        histoSBCut[i]               = (TH1D*)Cutuncorrfile[i]->Get(nameSBCut.Data());
        histoSBCut[i]->SetName(Form("histoSBdefaultMeson_%s",cutNumber[i].Data()));
        if (DebugOutputLevel>=2){cout << "Debug; CompareDifferentDirectories.C, line " << __LINE__ << "; histoClusterE: " << nameClusterE.Data() << endl;}
        histoClusterE[i]       =(TH1D*)Cutuncorrfile[i]->Get(nameClusterE.Data());

        if (DebugOutputLevel>=2){cout << "Debug; CompareDifferentDirectories.C, line " << __LINE__ << "; " << endl;}
        if(histoClusterE[i]) histoClusterE[i]->SetName(Form("ClusterEPerEvent_%s", cutStringsName[i].Data()));
        else isClusterE = kFALSE;

        // Calculate ratios for comparisons
        if (DebugOutputLevel>=2){cout << "Debug; CompareDifferentDirectories.C, line " << __LINE__ << "; Calculate ratios for comparisons" << endl;}
        if(readCorrectedFile[i]){
          histoRatioCorrectedYieldCut[i] = (TH1D*) histoCorrectedYieldCut[i]->Clone(Form("histoRatioCorrectedYieldCut_%s",cutStringsName[i].Data()));
          histoRatioCorrectedYieldCut[i]->Divide(histoRatioCorrectedYieldCut[i],histoCorrectedYieldCut[0],1.,1.,"B");
          if (i > 0){
              maxPt= histoCorrectedYieldCut[i]->GetBinCenter(histoCorrectedYieldCut[i]->GetNbinsX()) + 0.5* histoCorrectedYieldCut[i]->GetBinWidth(histoCorrectedYieldCut[i]->GetNbinsX());
          }
          if (doCorrectedYieldWOSecCut){
            histoRatioCorrectedYieldWOSecCut[i] = (TH1D*) histoCorrectedYieldWOSecCut[i]->Clone(Form("histoRatioCorrectedYieldWOSecCut_%s",cutStringsName[i].Data()));
            histoRatioCorrectedYieldWOSecCut[i]->Divide(histoRatioCorrectedYieldWOSecCut[i],histoCorrectedYieldWOSecCut[0],1.,1.,"B");
          } else {
              histoRatioCorrectedYieldWOSecCut[i] = NULL;
          }
          if (doAlternativeCorrectedYield){
              histoRatioAltCorrectedYieldCut[i] = (TH1D*) histoAltCorrectedYieldCut[i]->Clone(Form("histoRatioAltCorrectedYieldCut_%s",cutStringsName[i].Data()));
              histoRatioAltCorrectedYieldCut[i]->Divide(histoRatioAltCorrectedYieldCut[i],histoAltCorrectedYieldCut[0],1.,1.,"B");
          } else {
              histoRatioAltCorrectedYieldCut[i] = NULL;
          }
          if (doMCInput){
              histoRatioMCInputYieldCut[i] = (TH1D*) histoMCInputYieldCut[i]->Clone(Form("histoRatioMCInputYieldCut_%s",cutStringsName[i].Data()));
              histoRatioMCInputYieldCut[i]->Divide(histoRatioMCInputYieldCut[i],histoMCInputYieldCut[0],1.,1.,"B");

              histoRatioToCorYieldMCInputYieldCut[i] = (TH1D*) histoMCInputYieldCut[i]->Clone(Form("histoRatioToCorYieldMCInputYieldCut%s",cutStringsName[i].Data()));
              histoRatioToCorYieldMCInputYieldCut[i]->Divide(histoRatioToCorYieldMCInputYieldCut[i],histoCorrectedYieldCut[0],1.,1.,"B");
              if (doAlternativeCorrectedYield){
                  histoRatioToAltCorYieldMCInputYieldCut[i] = (TH1D*) histoMCInputYieldCut[i]->Clone(Form("histoRatioToAltCorYieldMCInputYieldCut%s",cutStringsName[i].Data()));
                  histoRatioToAltCorYieldMCInputYieldCut[i]->Divide(histoRatioToAltCorYieldMCInputYieldCut[i],histoAltCorrectedYieldCut[0],1.,1.,"B");
              }
          } else {
              histoRatioMCInputYieldCut[i] = NULL;
              histoRatioToCorYieldMCInputYieldCut[i] = NULL;
              histoRatioToAltCorYieldMCInputYieldCut[i] = NULL;

          }
          histoRatioTrueEffiCut[i]    = (TH1D*) histoTrueEffiCut[i]->Clone(Form("histoRatioTrueEffiCut_%s",cutStringsName[i].Data()));
          histoRatioTrueEffiCut[i]->Divide(histoRatioTrueEffiCut[i],histoTrueEffiCut[0],1.,1.,"B");
          if (doAlternativeEfficiency){
              histoRatioAltEffiCut[i]    = (TH1D*) histoAltEffiCut[i]->Clone(Form("histoRatioAltEffiCut_%s",cutStringsName[i].Data()));
              histoRatioAltEffiCut[i]->Divide(histoRatioAltEffiCut[i],histoAltEffiCut[0],1.,1.,"B");

              histoRatioTrueEffDivAltEffiCut[i]    = (TH1D*) histoTrueEffiCut[i]->Clone(Form("histoRatioTrueEffDivAltEffiCut_%s",cutStringsName[i].Data()));
              histoRatioTrueEffDivAltEffiCut[i]->Divide(histoRatioTrueEffDivAltEffiCut[i],histoAltEffiCut[i],1.,1.,"B");

              histoRatioOfRatioTrueEffDivAltEffiCut[i]    = (TH1D*) histoRatioTrueEffDivAltEffiCut[i]->Clone(Form("histoRatioTrueEffDivAltEffiCut_%s",cutStringsName[i].Data()));
              histoRatioOfRatioTrueEffDivAltEffiCut[i]->Divide(histoRatioOfRatioTrueEffDivAltEffiCut[i], histoRatioTrueEffDivAltEffiCut[0],1.,1.,"B");
          }
          histoRatioAcceptanceCut[i]  = (TH1D*) histoAcceptanceCut[i]->Clone(Form("histoRatioAcceptanceCut_%s",cutStringsName[i].Data()));
          histoRatioAcceptanceCut[i]->Divide(histoRatioAcceptanceCut[i],histoAcceptanceCut[0],1.,1.,"B");
        }
        if (DebugOutputLevel>=2){cout << "Debug; CompareDifferentDirectories.C, line " << __LINE__ << "; " << endl;}

        histoRatioRawYieldCut[i]    = (TH1D*) histoRawYieldCut[i]->Clone(Form("histoRatioRawYieldCut_%s",cutStringsName[i].Data()));
        histoRatioRawYieldCut[i]->Divide(histoRatioRawYieldCut[i],histoRawYieldCut[0],1.,1.,"B");
        histoRatioMassCut[i]        = (TH1D*) histoMassCut[i]->Clone(Form("histoRatioMassCut_%s",cutStringsName[i].Data()));
        histoRatioMassCut[i]->Divide(histoRatioMassCut[i],histoMassCut[0],1.,1.,"B");
        histoRatioWidthCut[i]        = (TH1D*) histoWidthCut[i]->Clone(Form("histoRatioWidthCut_%s",cutStringsName[i].Data()));
        histoRatioWidthCut[i]->Divide(histoRatioWidthCut[i],histoWidthCut[0],1.,1.,"B");

        histoRatioSBCut[i]          = (TH1D*) histoSBCut[i]->Clone(Form("histoRatioSBCut_%s", cutStringsName[i].Data()));
        histoRatioSBCut[i]->Divide(histoRatioSBCut[i],histoSBCut[0],1.,1.,"B");

        if(isClusterE){
          histoRatioClusterE[i]       =(TH1D*)histoClusterE[i]->Clone(Form("histoRatioClusterEPerEvent_%s", cutStringsName[i].Data()));
          histoRatioClusterE[i]->Divide(histoRatioClusterE[i],histoClusterE[0],1.,1.,"B");
        }  
        if (DebugOutputLevel>=2){cout << "Debug; CompareDifferentDirectories.C, line " << __LINE__ << "; " << endl;}

    }
    cout<<"=========================="<<endl;

    //**************************************************************************************
    //****************** Calculate Average Values of Corrected-Yield ***********************
    //**************************************************************************************
    histoCorrectedYieldCutAverage = (TH1D*) histoCorrectedYieldCut[0]->Clone(Form("histoCorrectedYieldCutAverage_%s",cutStringsName[0].Data()));
    histoCorrectedYieldCutAverage_MAD =
            new TH1D ("histoCorrectedYieldCutAverage_MAD", "histoCorrectedYieldCutAverage_MAD", NumberOfCuts, +0.5, NumberOfCuts+0.5);
    histoCorrectedYieldCutAverage_MAD_NormWStatErr =
            new TH1D ("histoCorrectedYieldCutAverage_MAD_NormWStatErr", "histoCorrectedYieldCutAverage_MAD_NormWStatErr", NumberOfCuts, +0.5, NumberOfCuts+0.5);
    histoCorrectedYieldCutMCInput_MAD =
            new TH1D ("histoCorrectedYieldCutMCInput_MAD", "histoCorrectedYieldCutMCInput_MAD", NumberOfCuts, +0.5, NumberOfCuts+0.5);
    histoCorrectedYieldCutMCInput_MAD_NormWStatErr =
            new TH1D ("histoCorrectedYieldCutMCInput_MAD_NormWStatErr", "histoCorrectedYieldCutMCInput_MAD_NormWStatErr", NumberOfCuts, +0.5, NumberOfCuts+0.5);
    if (doCorrectedYieldAverage){
        if (DebugOutputLevel>=1){cout << "Debug; CompareDifferentDirectories.C, line " << __LINE__ << "; Calculate Average Values of Corrected-Yield" << endl;}
        Double_t verySmallNumber                            = 0.0000000000000001;
        Int_t iNBinsCorrectedYieldAverage                   = histoCorrectedYieldCutAverage->GetNbinsX();
        Int_t iStartBinCorrectedYieldAverage                = 1;
        Int_t iEndBinCorrectedYieldAverage                  = iNBinsCorrectedYieldAverage;
        Int_t iLastBinWithContent                           = iEndBinCorrectedYieldAverage;
        Double_t dBinContentCorrectedYield;
        Double_t dBinErrorCorrectedYield;
        Double_t dSumBinContentsCorrectedYield;
        Double_t dCorrectedYieldAverageValue;
        Double_t dCorrectedYieldMCInputValue;

        Double_t dBinContentCorrectedYieldMinusAverage;
        Double_t dBinContentCorrectedYieldMinusAverageSquared;
        Double_t dSumBinContentsCorrectedYieldMinusAverageSquared;
        Double_t dCorrectedYieldVarianceValue;
        Double_t dCorrectedYieldStandardDeviationValue;

        Double_t dDeviationCorrectedYieldAverageValue;
        Double_t dDeviationCorrectedYieldAverageSquaredValue;
        Double_t dDeviationCorrectedYieldAverageSquaredValueDivVar;
        Double_t dSumAbsDeviationCorrectedYieldAverageValue;
        Double_t dCorrectedYieldAverage_MADValue;
        Double_t dDeviationCorrectedYieldAverageSquaredValueDivVar_NormWStatErr;
        Double_t dSumAbsDeviationCorrectedYieldAverageValue_NormWStatErr;
        Double_t dCorrectedYieldAverage_MADValue_NormWStatErr;

        Double_t dDeviationCorrectedYieldMCInputValue;
        Double_t dDeviationCorrectedYieldMCInputSquaredValue;
        Double_t dDeviationCorrectedYieldMCInputSquaredValueDivVar;
        Double_t dSumAbsDeviationCorrectedYieldMCInputValue;
        Double_t dCorrectedYieldMCInput_MADValue;
        Double_t dDeviationCorrectedYieldMCInputSquaredValueDivVar_NormWStatErr;
        Double_t dSumAbsDeviationCorrectedYieldMCInputValue_NormWStatErr;
        Double_t dCorrectedYieldMCInput_MADValue_NormWStatErr;
        for (Int_t iBinCorrectedYieldAverage=iStartBinCorrectedYieldAverage; iBinCorrectedYieldAverage<=iEndBinCorrectedYieldAverage; iBinCorrectedYieldAverage++){
            if (DebugOutputLevel>=2){cout << "Debug; CompareDifferentDirectories.C, line " << __LINE__ << "; " << endl;}
            //For MAD Calculation Remove first points of histogram which are 0
            dBinContentCorrectedYield                           =
                    histoCorrectedYieldCut[0]->GetBinContent(iBinCorrectedYieldAverage);
            if (iBinCorrectedYieldAverage == iStartBinCorrectedYieldAverage){
                if ((dBinContentCorrectedYield <= 0+verySmallNumber)&&(dBinContentCorrectedYield >= 0-verySmallNumber)){
                    iStartBinCorrectedYieldAverage++;
                    continue;
                }
            } else {
                if ((dBinContentCorrectedYield <= 0+verySmallNumber)&&(dBinContentCorrectedYield >= 0-verySmallNumber)){
                    if (iLastBinWithContent > iBinCorrectedYieldAverage){
                        iLastBinWithContent = iBinCorrectedYieldAverage;
                    }
                    continue;
                }
            }
            iLastBinWithContent                                 = iBinCorrectedYieldAverage; //If content is 0 for one bin but greater than 0 for later bins
            dSumBinContentsCorrectedYield                       = 0;
            for (Int_t i=0; i< NumberOfCuts; i++){
                //BinValues
                dBinContentCorrectedYield                       =
                        histoCorrectedYieldCut[i]->GetBinContent(iBinCorrectedYieldAverage);
                //Calculations wit Bin Value
                dSumBinContentsCorrectedYield                   += dBinContentCorrectedYield;
            }
            dCorrectedYieldAverageValue                         = dSumBinContentsCorrectedYield / NumberOfCuts;
            //Calculate Variance
            dSumBinContentsCorrectedYieldMinusAverageSquared=0;
            for (Int_t i=0; i< NumberOfCuts; i++){
                //BinValues
                dBinContentCorrectedYield                       =
                        histoCorrectedYieldCut[i]->GetBinContent(iBinCorrectedYieldAverage);
                //Calculations with Bin Values
                dBinContentCorrectedYieldMinusAverage           = dBinContentCorrectedYield-dCorrectedYieldAverageValue;
                dBinContentCorrectedYieldMinusAverageSquared    = dBinContentCorrectedYieldMinusAverage*dBinContentCorrectedYieldMinusAverage;
                dSumBinContentsCorrectedYieldMinusAverageSquared += dBinContentCorrectedYieldMinusAverageSquared;
            }
            dCorrectedYieldVarianceValue                        = dSumBinContentsCorrectedYieldMinusAverageSquared / (NumberOfCuts-1);
            dCorrectedYieldStandardDeviationValue               = TMath::Sqrt(dCorrectedYieldVarianceValue);
            histoCorrectedYieldCutAverage                       ->SetBinContent(iBinCorrectedYieldAverage, dCorrectedYieldAverageValue);
            histoCorrectedYieldCutAverage                       ->SetBinError(iBinCorrectedYieldAverage, dCorrectedYieldStandardDeviationValue);
        }

        //Calculate MAD
        if (DebugOutputLevel>=2){cout << "Debug; CompareDifferentDirectories.C, line " << __LINE__ << "; Calculate MAD" << endl;}
        for (Int_t i=0; i< NumberOfCuts; i++){
            dSumAbsDeviationCorrectedYieldAverageValue                              = 0;
            dSumAbsDeviationCorrectedYieldAverageValue_NormWStatErr                 = 0;
            for (Int_t iBinCorrectedYieldAverage=iStartBinCorrectedYieldAverage; iBinCorrectedYieldAverage<=iLastBinWithContent; iBinCorrectedYieldAverage++){
                //BinValues
                dBinContentCorrectedYield                                           =
                        histoCorrectedYieldCut[i]->GetBinContent(iBinCorrectedYieldAverage);
                dBinErrorCorrectedYield                                             =
                        histoCorrectedYieldCut[i]->GetBinError(iBinCorrectedYieldAverage);
                dCorrectedYieldAverageValue                                         =
                        histoCorrectedYieldCutAverage->GetBinContent(iBinCorrectedYieldAverage);
                dCorrectedYieldStandardDeviationValue                               =
                        histoCorrectedYieldCutAverage->GetBinError(iBinCorrectedYieldAverage);
                //Calculations with Bin Values
                dCorrectedYieldVarianceValue                                        =
                        dCorrectedYieldStandardDeviationValue*dCorrectedYieldStandardDeviationValue;
                dDeviationCorrectedYieldAverageValue                                =
                        dBinContentCorrectedYield-dCorrectedYieldAverageValue;
                dDeviationCorrectedYieldAverageSquaredValue                         =
                        dDeviationCorrectedYieldAverageValue*dDeviationCorrectedYieldAverageValue;
                dDeviationCorrectedYieldAverageSquaredValueDivVar                   =
                        dDeviationCorrectedYieldAverageSquaredValue/dCorrectedYieldVarianceValue;
                dDeviationCorrectedYieldAverageSquaredValueDivVar_NormWStatErr      =
                        dDeviationCorrectedYieldAverageSquaredValue/(dCorrectedYieldVarianceValue+(dBinErrorCorrectedYield*dBinErrorCorrectedYield));

                dSumAbsDeviationCorrectedYieldAverageValue                          +=
                        dDeviationCorrectedYieldAverageSquaredValueDivVar;
                dSumAbsDeviationCorrectedYieldAverageValue_NormWStatErr             +=
                        dDeviationCorrectedYieldAverageSquaredValueDivVar_NormWStatErr;
            }
            dCorrectedYieldAverage_MADValue                                         =
                    dSumAbsDeviationCorrectedYieldAverageValue / (iEndBinCorrectedYieldAverage-iStartBinCorrectedYieldAverage+1);
            dCorrectedYieldAverage_MADValue_NormWStatErr                           =
                    dSumAbsDeviationCorrectedYieldAverageValue_NormWStatErr / (iEndBinCorrectedYieldAverage-iStartBinCorrectedYieldAverage+1);

            histoCorrectedYieldCutAverage_MAD                                       ->GetXaxis()->SetBinLabel(i+1, cutStringsName[i].Data());
            histoCorrectedYieldCutAverage_MAD                                       ->SetBinContent(i+1, dCorrectedYieldAverage_MADValue);
            histoCorrectedYieldCutAverage_MAD_NormWStatErr                          ->GetXaxis()->SetBinLabel(i+1, cutStringsName[i].Data());
            histoCorrectedYieldCutAverage_MAD_NormWStatErr                          ->SetBinContent(i+1, dCorrectedYieldAverage_MADValue_NormWStatErr);
        }
        //Calculate MAD with MCInput
        for (Int_t i=0; i< NumberOfCuts; i++){
            if (doMCInput){
                if (DebugOutputLevel>=1){cout << "Debug; CompareDifferentDirectories.C, line " << __LINE__ << "; Calculate MAD with MCInput" << endl;}
                for (Int_t i=0; i< NumberOfCuts; i++){
                    dSumAbsDeviationCorrectedYieldMCInputValue                              = 0;
                    dSumAbsDeviationCorrectedYieldMCInputValue_NormWStatErr                 = 0;
                    for (Int_t iBinCorrectedYieldAverage=iStartBinCorrectedYieldAverage; iBinCorrectedYieldAverage<=iLastBinWithContent; iBinCorrectedYieldAverage++){
                        //BinValues
                        dBinContentCorrectedYield                                           =
                                histoCorrectedYieldCut[i]->GetBinContent(iBinCorrectedYieldAverage);
                        dBinErrorCorrectedYield                                             =
                                histoCorrectedYieldCut[i]->GetBinError(iBinCorrectedYieldAverage);
                        dCorrectedYieldAverageValue                                         =
                                histoCorrectedYieldCutAverage->GetBinContent(iBinCorrectedYieldAverage);
                        dCorrectedYieldStandardDeviationValue                               =
                                histoCorrectedYieldCutAverage->GetBinError(iBinCorrectedYieldAverage);
                        dCorrectedYieldMCInputValue                                         =
                                histoMCInputYieldCut[0]->GetBinContent(iBinCorrectedYieldAverage);
                        dCorrectedYieldMCInputValue                                         =
                                histoMCInputYieldCut[0]->GetBinContent(iBinCorrectedYieldAverage);
                        //Calculations with Bin Values
                        dCorrectedYieldVarianceValue                                        =
                                dCorrectedYieldStandardDeviationValue*dCorrectedYieldStandardDeviationValue;
                        dDeviationCorrectedYieldMCInputValue                                =
                                dBinContentCorrectedYield-dCorrectedYieldMCInputValue;
                        dDeviationCorrectedYieldMCInputSquaredValue                         =
                                dDeviationCorrectedYieldMCInputValue*dDeviationCorrectedYieldMCInputValue;
                        dDeviationCorrectedYieldMCInputSquaredValueDivVar                   =
                                dDeviationCorrectedYieldMCInputSquaredValue/dCorrectedYieldVarianceValue;
                        dDeviationCorrectedYieldMCInputSquaredValueDivVar_NormWStatErr      =
                                dDeviationCorrectedYieldMCInputSquaredValue/(dCorrectedYieldVarianceValue+(dBinErrorCorrectedYield*dBinErrorCorrectedYield));

                        dSumAbsDeviationCorrectedYieldMCInputValue                          +=
                                dDeviationCorrectedYieldMCInputSquaredValueDivVar;
                        dSumAbsDeviationCorrectedYieldMCInputValue_NormWStatErr             +=
                                dDeviationCorrectedYieldMCInputSquaredValueDivVar_NormWStatErr;
                    }
                    dCorrectedYieldMCInput_MADValue                                         =
                            dSumAbsDeviationCorrectedYieldMCInputValue / (iEndBinCorrectedYieldAverage-iStartBinCorrectedYieldAverage+1);
                    dCorrectedYieldMCInput_MADValue_NormWStatErr                           =
                            dSumAbsDeviationCorrectedYieldMCInputValue_NormWStatErr / (iEndBinCorrectedYieldAverage-iStartBinCorrectedYieldAverage+1);

                    histoCorrectedYieldCutMCInput_MAD                                       ->GetXaxis()->SetBinLabel(i+1, cutStringsName[i].Data());
                    histoCorrectedYieldCutMCInput_MAD                                       ->SetBinContent(i+1, dCorrectedYieldMCInput_MADValue);
                    histoCorrectedYieldCutMCInput_MAD_NormWStatErr                          ->GetXaxis()->SetBinLabel(i+1, cutStringsName[i].Data());
                    histoCorrectedYieldCutMCInput_MAD_NormWStatErr                          ->SetBinContent(i+1, dCorrectedYieldMCInput_MADValue_NormWStatErr);
                }
            }
        }
    }
    histoRatioCorrectedYieldCutAverage                                              =
            (TH1D*) histoCorrectedYieldCutAverage->Clone(Form("histoRatioCorrectedYieldCutAverage_%s",cutStringsName[0].Data()));
    if (doCorrectedYieldAverage){
        histoRatioCorrectedYieldCutAverage                                          ->Divide(histoRatioCorrectedYieldCutAverage, histoCorrectedYieldCut[0],1.,1.,"B");
    }

    //**************************************************************************************
    //********************* Plotting RAW-Yield *********************************************
    //**************************************************************************************
    if (DebugOutputLevel>=1){cout << "Debug; CompareDifferentDirectories.C, line " << __LINE__ << "; Plotting RAW-Yield" << endl;}

        // Canvas Definition
        TCanvas* canvasRawYieldMeson = new TCanvas("canvasRawYieldMeson","",1350,1500);
        DrawGammaCanvasSettings( canvasRawYieldMeson,  0.13, 0.02, 0.02, 0.09);
        // Upper pad definition
        TPad* padRawYield = new TPad("padRawYield", "", 0., 0.33, 1., 1.,-1, -1, -2);
        DrawGammaPadSettings( padRawYield, 0.12, 0.02, 0.02, 0.);
        padRawYield->SetLogy();
        padRawYield->Draw();
        // lower pad definition
        TPad* padRawYieldRatios = new TPad("padRawYieldRatios", "", 0., 0., 1., 0.33,-1, -1, -2);
        DrawGammaPadSettings( padRawYieldRatios, 0.12, 0.02, 0.0, 0.2);
        padRawYieldRatios->Draw();

        // Plot raw yield in uppper panel
        padRawYield->cd();
        TLegend* legendRawMeson = GetAndSetLegend2(0.15,0.02,0.3,0.02+1.15*0.032*NumberOfCuts, 1500*0.75*0.032);
        if (cutVariationName.Contains("dEdxPi")){
            legendRawMeson->SetTextSize(0.02);
        }

        for(Int_t i = 0; i< NumberOfCuts; i++){
            if(i == 0){
                Double_t scaleFactorRaw = 5.;
                DrawGammaSetMarker(histoRawYieldCut[i], 20, 1., color[0], color[0]);
                DrawAutoGammaMesonHistos( histoRawYieldCut[i],
                                        "", "#it{p}_{T} (GeV/#it{c})", Form("%s RAW Yield/(#it{N}_{ev} #it{N}_{coll})",textMeson.Data()),
                                        kTRUE, scaleFactorRaw, 10e-10, kTRUE,
                                        kFALSE, 0.0, 0.030,
                                        kFALSE, 0., 10.);
                legendRawMeson->AddEntry(histoRawYieldCut[i],Form("standard: %s",cutStringsName[i].Data()));
            }
            else {
                if(i<20){
                    DrawGammaSetMarker(histoRawYieldCut[i], 20+i, 1.,color[i],color[i]);
                } else {
                    DrawGammaSetMarker(histoRawYieldCut[i], 20+i, 1.,color[i-20],color[i-20]);
                }
                histoRawYieldCut[i]->DrawCopy("same,e1,p");
                legendRawMeson->AddEntry(histoRawYieldCut[i],cutStringsName[i].Data());
            }
        }
        legendRawMeson->Draw();
        // Labeling of plot

        PutProcessLabelAndEnergyOnPlot( 0.94, 0.95, 0.032, collisionSystem, process, detectionProcess, 42, 0.03, optionPeriod, 1, 1.25, 31);


        // Plot ratio of raw yields in lower panel
        padRawYieldRatios->cd();
        for(Int_t i = 0; i< NumberOfCuts; i++){
            if(i==0){
                // Set ratio min and max
                Double_t minYRatio = 0.45;
                Double_t maxYRatio = 1.55;
                if (cutVariationName.Contains("PhotonQuality")){
                    minYRatio = 0.001;
                    maxYRatio = 2;
                    padRawYieldRatios->SetLogy(1);
                } else if (cutVariationName.Contains("LHC12NL") || cutVariationName.Contains("LHC12-MultWeight")){
                    minYRatio = 0.9;
                    maxYRatio = 1.1;
                } else if (cutVariationName.Contains("AODvalidation")){
                    minYRatio = 0.95;
                    maxYRatio = 1.05;
                } else if (cutVariationName.Contains("PHOS_Combined")) {
                   minYRatio = 0.90;
                   maxYRatio = 1.10;
                }
                SetStyleHistoTH1ForGraphs(histoRatioRawYieldCut[i], "#it{p}_{T} (GeV/#it{c})", "#frac{modified}{standard}", 0.08, 0.11, 0.07, 0.1, 0.75, 0.5, 510,505);
                DrawGammaSetMarker(histoRatioRawYieldCut[i], 20, 1.,color[0],color[0]);
                histoRatioRawYieldCut[i]->GetYaxis()->SetRangeUser(minYRatio,maxYRatio);
                histoRatioRawYieldCut[i]->DrawCopy("p,e1");
            } else{
                if(i<20){
                    DrawGammaSetMarker(histoRatioRawYieldCut[i], 20+i, 1.,color[i],color[i]);
                } else {
                    DrawGammaSetMarker(histoRatioRawYieldCut[i], 20+i, 1.,color[i-20],color[i-20]);
                }
                histoRatioRawYieldCut[i]->DrawCopy("same,e1,p");
            }
        }
        DrawGammaLines(0., maxPt,1., 1.,1);

        canvasRawYieldMeson->Update();
        canvasRawYieldMeson->SaveAs(Form("%s/%s_%s_RAWYield.%s",outputDir.Data(),meson.Data(),prefix2.Data(),suffix.Data()));
        delete canvasRawYieldMeson;


    //*****************************************************************************************
    //******************* Compare Corrected Yields ********************************************
    //*****************************************************************************************
    if (DebugOutputLevel>=1){cout << "Debug; CompareDifferentDirectories.C, line " << __LINE__ << "; Compare Corrected Yields" << endl;}
    // Define canvas
    if(!plotOnlyUncorrectedOutput){
      TCanvas* canvasCorrectedYieldMeson = new TCanvas("canvasCorrectedYieldMeson","",1350,1500);
      DrawGammaCanvasSettings( canvasCorrectedYieldMeson,  0.13, 0.02, 0.02, 0.09);
      // Define upper panel
      TPad* padCorrectedYield = new TPad("padCorrectedYield", "", 0., 0.33, 1., 1.,-1, -1, -2);
      DrawGammaPadSettings( padCorrectedYield, 0.12, 0.02, 0.02, 0.);
      padCorrectedYield->SetLogy(1);
      padCorrectedYield->Draw();
      // Define lower panel
      TPad* padCorrectedYieldRatios = new TPad("padCorrectedYieldRatios", "", 0., 0., 1., 0.33,-1, -1, -2);
      DrawGammaPadSettings( padCorrectedYieldRatios, 0.12, 0.02, 0.0, 0.2);
      padCorrectedYieldRatios->SetLogy(0);
      padCorrectedYieldRatios->Draw();

      // Plot corrected yield in upper panel
      padCorrectedYield->cd();
          TLegend* legendCorrectedYieldMeson = GetAndSetLegend2(0.15,0.02,0.3,0.02+1.15*0.032*NumberOfCuts, 1500*0.75*0.032);
          for(Int_t i = 0; i< NumberOfCuts; i++){
              if(i == 0){
                  DrawAutoGammaMesonHistos( histoCorrectedYieldCut[i],
                                          "", "#it{p}_{T} (GeV/#it{c})", "#frac{1}{2#pi #it{N}_{ev.}} #frac{d^{2}#it{N}}{#it{p}_{T}d#it{p}_{T}d#it{y}} (#it{c}/GeV)",
                                          kTRUE, 5., 5e-10,kTRUE,
                                          kFALSE, 0.0, 0.030,
                                          kFALSE, 0., 10.);
                  DrawGammaSetMarker(histoCorrectedYieldCut[i], 20, 1., color[0], color[0]);
                  histoCorrectedYieldCut[i]->DrawCopy("e1,p");
                  legendCorrectedYieldMeson->AddEntry(histoCorrectedYieldCut[i], Form("standard: %s",cutStringsName[i].Data()));

                  if ((doCorrectedYieldAverage)&&(histoCorrectedYieldCutAverage)){
                      Int_t ColorAndMarkerNumber=NumberOfCuts;
                      if(NumberOfCuts<20){
                          DrawGammaSetMarker(histoCorrectedYieldCutAverage, 20+ColorAndMarkerNumber, 1.,color[ColorAndMarkerNumber],color[ColorAndMarkerNumber]);
                      } else {
                          DrawGammaSetMarker(histoCorrectedYieldCutAverage, 20+ColorAndMarkerNumber, 1.,color[ColorAndMarkerNumber-20],color[ColorAndMarkerNumber-20]);
                      }
                      histoCorrectedYieldCutAverage->DrawCopy("same,e1,p");
                      legendCorrectedYieldMeson->AddEntry(histoCorrectedYieldCutAverage, "Average Values");
                  }
                  if (doMCInput){
                      Int_t ColorAndMarkerNumber=NumberOfCuts;
                      if ((doCorrectedYieldAverage)&&(histoRatioCorrectedYieldCutAverage)){
                        ColorAndMarkerNumber=NumberOfCuts+1;
                      }
                      if(NumberOfCuts<20){
                          DrawGammaSetMarker(histoMCInputYieldCut[0], 20+ColorAndMarkerNumber, 1.,color[ColorAndMarkerNumber],color[ColorAndMarkerNumber]);
                      } else {
                          DrawGammaSetMarker(histoMCInputYieldCut[0], 20+ColorAndMarkerNumber, 1.,color[ColorAndMarkerNumber-20],color[ColorAndMarkerNumber-20]);
                      }
                      histoMCInputYieldCut[0]->DrawCopy("same,e1,p");
                      legendCorrectedYieldMeson->AddEntry(histoMCInputYieldCut[0], "MC Input");
                  }
              }
              else{
                  if(i<20){
                      DrawGammaSetMarker(histoCorrectedYieldCut[i], 20+i, 1.,color[i],color[i]);
                  } else {
                      DrawGammaSetMarker(histoCorrectedYieldCut[i], 20+i, 1.,color[i-20],color[i-20]);
                  }
                  histoCorrectedYieldCut[i]->DrawCopy("same,e1,p");
                  legendCorrectedYieldMeson->AddEntry(histoCorrectedYieldCut[i], cutStringsName[i].Data());
              }

          }
          legendCorrectedYieldMeson->Draw();
          // Labeling of plot
          PutProcessLabelAndEnergyOnPlot( 0.94, 0.95, 0.032, collisionSystem, process, detectionProcess, 42, 0.03, optionPeriod, 1, 1.25, 31);

          // plot ratio of corrected yields in lower panel
          padCorrectedYieldRatios->cd();
          for(Int_t i = 0; i< NumberOfCuts; i++){
              if(i==0){
                  // Set ratio min and max
                  Double_t minYRatio = 0.75;
                  Double_t maxYRatio = 1.25;
                  if (mode != 0 && mode!= 1 ){
                      minYRatio = 0.75;
                      maxYRatio = 1.55;
                  }
                  if (cutVariationName.Contains("LHC12NL") || cutVariationName.Contains("LHC12-MultWeight")){
                      minYRatio = 0.81;
                      maxYRatio = 1.19;
                  } else if (cutVariationName.Contains("AODvalidation")){
                      minYRatio = 0.8;
                      maxYRatio = 1.2;
                  } else if (cutVariationName.Contains("PHOS_Combined")) {
                      minYRatio = 0.90;
                      maxYRatio = 1.15;
                  }  else if (meson.Contains("Omega")) {
                      minYRatio = 0.1;
                      maxYRatio = 2.55;
                  }
                  SetStyleHistoTH1ForGraphs(histoRatioCorrectedYieldCut[i], "#it{p}_{T} (GeV/#it{c})", "#frac{modified}{standard}", 0.08, 0.11, 0.07, 0.1, 0.75, 0.5, 510,505);
                  DrawGammaSetMarker(histoRatioCorrectedYieldCut[i], 20, 1.,color[0],color[0]);
                  histoRatioCorrectedYieldCut[i]->GetYaxis()->SetRangeUser(minYRatio,maxYRatio);
                  histoRatioCorrectedYieldCut[i]->DrawCopy("p,e1");

                  if ((doCorrectedYieldAverage)&&(histoRatioCorrectedYieldCutAverage)){
                      Int_t ColorAndMarkerNumber=NumberOfCuts;
                      if(ColorAndMarkerNumber<20){
                          DrawGammaSetMarker(histoRatioCorrectedYieldCutAverage, 20+ColorAndMarkerNumber, 1.,color[ColorAndMarkerNumber],color[ColorAndMarkerNumber]);
                      } else {
                          DrawGammaSetMarker(histoRatioCorrectedYieldCutAverage, 20+ColorAndMarkerNumber, 1.,color[ColorAndMarkerNumber-20],color[ColorAndMarkerNumber-20]);
                      }
                      histoRatioCorrectedYieldCutAverage->DrawCopy("same,e1,p");
                  }
                  if (doMCInput){
                      Int_t ColorAndMarkerNumber=NumberOfCuts;
                      if ((doCorrectedYieldAverage)&&(histoRatioCorrectedYieldCutAverage)){
                        ColorAndMarkerNumber=NumberOfCuts+1;
                      }
                      if(ColorAndMarkerNumber<20){
                          DrawGammaSetMarker(histoRatioToCorYieldMCInputYieldCut[0], 20+ColorAndMarkerNumber, 1.,color[ColorAndMarkerNumber],color[ColorAndMarkerNumber]);
                      } else {
                          DrawGammaSetMarker(histoRatioToCorYieldMCInputYieldCut[0], 20+ColorAndMarkerNumber, 1.,color[ColorAndMarkerNumber-20],color[ColorAndMarkerNumber-20]);
                      }
                      histoRatioToCorYieldMCInputYieldCut[0]->DrawCopy("same,e1,p");
                  }
              }
              else{
                  if(i<20){
                      DrawGammaSetMarker(histoRatioCorrectedYieldCut[i], 20+i, 1.,color[i],color[i]);
                  } else {
                      DrawGammaSetMarker(histoRatioCorrectedYieldCut[i], 20+i, 1.,color[i-20],color[i-20]);
                  }
                  histoRatioCorrectedYieldCut[i]->DrawCopy("same,e1,p");
              }
          }
          DrawGammaLines(0., maxPt,1., 1.,1);

      canvasCorrectedYieldMeson->Update();
      canvasCorrectedYieldMeson->SaveAs(Form("%s/%s_%s_CorrectedYield.%s",outputDir.Data(), meson.Data(),prefix2.Data(),suffix.Data()));
      delete canvasCorrectedYieldMeson;



    }

    //*****************************************************************************************
    //******************* Compare Alternative Corrected Yields ********************************
    //*****************************************************************************************
    if (doAlternativeCorrectedYield){

        //Calculate Average Values of Alternative Corrected-Yield
        histoAltCorrectedYieldCutAverage = (TH1D*) histoAltCorrectedYieldCut[0]->Clone(Form("histoAltCorrectedYieldCutAverage_%s",cutStringsName[0].Data()));
        histoAltCorrectedYieldCutAverage_MAD =
                new TH1D ("histoAltCorrectedYieldCutAverage_MAD", "histoAltCorrectedYieldCutAverage_MAD", NumberOfCuts, +0.5, NumberOfCuts+0.5);
        histoAltCorrectedYieldCutAverage_MAD_NormWStatErr =
                new TH1D ("histoAltCorrectedYieldCutAverage_MAD_NormWStatErr", "histoAltCorrectedYieldCutAverage_MAD_NormWStatErr", NumberOfCuts, +0.5, NumberOfCuts+0.5);
        histoAltCorrectedYieldCutMCInput_MAD =
                new TH1D ("histoAltCorrectedYieldCutMCInput_MAD", "histoAltCorrectedYieldCutMCInput_MAD", NumberOfCuts, +0.5, NumberOfCuts+0.5);
        histoAltCorrectedYieldCutMCInput_MAD_NormWStatErr =
                new TH1D ("histoAltCorrectedYieldCutMCInput_MAD_NormWStatErr", "histoAltCorrectedYieldCutMCInput_MAD_NormWStatErr", NumberOfCuts, +0.5, NumberOfCuts+0.5);
        if (doCorrectedYieldAverage){
            if (DebugOutputLevel>=1){cout << "Debug; CompareDifferentDirectories.C, line " << __LINE__ << "; Calculate Average Values of Corrected-Yield" << endl;}
            Double_t verySmallNumber                            = 0.0000000000000001;
            Int_t iNBinsCorrectedYieldAverage                   = histoAltCorrectedYieldCutAverage->GetNbinsX();
            Int_t iStartBinCorrectedYieldAverage                = 1;
            Int_t iEndBinCorrectedYieldAverage                  = iNBinsCorrectedYieldAverage;
            Int_t iLastBinWithContent                           = iEndBinCorrectedYieldAverage;
            Double_t dBinContentCorrectedYield;
            Double_t dBinErrorCorrectedYield;
            Double_t dSumBinContentsCorrectedYield;
            Double_t dCorrectedYieldAverageValue;
            Double_t dCorrectedYieldMCInputValue;

            Double_t dBinContentCorrectedYieldMinusAverage;
            Double_t dBinContentCorrectedYieldMinusAverageSquared;
            Double_t dSumBinContentsCorrectedYieldMinusAverageSquared;
            Double_t dCorrectedYieldVarianceValue;
            Double_t dCorrectedYieldStandardDeviationValue;

            Double_t dDeviationCorrectedYieldAverageValue;
            Double_t dDeviationCorrectedYieldAverageSquaredValue;
            Double_t dDeviationCorrectedYieldAverageSquaredValueDivVar;
            Double_t dSumAbsDeviationCorrectedYieldAverageValue;
            Double_t dCorrectedYieldAverage_MADValue;
            Double_t dDeviationCorrectedYieldAverageSquaredValueDivVar_NormWStatErr;
            Double_t dSumAbsDeviationCorrectedYieldAverageValue_NormWStatErr;
            Double_t dCorrectedYieldAverage_MADValue_NormWStatErr;

            Double_t dDeviationCorrectedYieldMCInputValue;
            Double_t dDeviationCorrectedYieldMCInputSquaredValue;
            Double_t dDeviationCorrectedYieldMCInputSquaredValueDivVar;
            Double_t dSumAbsDeviationCorrectedYieldMCInputValue;
            Double_t dCorrectedYieldMCInput_MADValue;
            Double_t dDeviationCorrectedYieldMCInputSquaredValueDivVar_NormWStatErr;
            Double_t dSumAbsDeviationCorrectedYieldMCInputValue_NormWStatErr;
            Double_t dCorrectedYieldMCInput_MADValue_NormWStatErr;
            for (Int_t iBinCorrectedYieldAverage=iStartBinCorrectedYieldAverage; iBinCorrectedYieldAverage<=iEndBinCorrectedYieldAverage; iBinCorrectedYieldAverage++){
                if (DebugOutputLevel>=2){cout << "Debug; CompareDifferentDirectories.C, line " << __LINE__ << "; " << endl;}
                //For MAD Calculation Remove first points of histogram which are 0
                dBinContentCorrectedYield                           =
                        histoAltCorrectedYieldCut[0]->GetBinContent(iBinCorrectedYieldAverage);
                if (iBinCorrectedYieldAverage == iStartBinCorrectedYieldAverage){
                    if ((dBinContentCorrectedYield <= 0+verySmallNumber)&&(dBinContentCorrectedYield >= 0-verySmallNumber)){
                        iStartBinCorrectedYieldAverage++;
                        continue;
                    }
                } else {
                    if ((dBinContentCorrectedYield <= 0+verySmallNumber)&&(dBinContentCorrectedYield >= 0-verySmallNumber)){
                        if (iLastBinWithContent > iBinCorrectedYieldAverage){
                            iLastBinWithContent = iBinCorrectedYieldAverage;
                        }
                        continue;
                    }
                }
                iLastBinWithContent                                 = iBinCorrectedYieldAverage; //If content is 0 for one bin but greater than 0 for later bins
                dSumBinContentsCorrectedYield                       = 0;
                for (Int_t i=0; i< NumberOfCuts; i++){
                    //BinValues
                    dBinContentCorrectedYield                       =
                            histoAltCorrectedYieldCut[i]->GetBinContent(iBinCorrectedYieldAverage);
                    //Calculations wit Bin Value
                    dSumBinContentsCorrectedYield                   += dBinContentCorrectedYield;
                }
                dCorrectedYieldAverageValue                         = dSumBinContentsCorrectedYield / NumberOfCuts;
                //Calculate Variance
                dSumBinContentsCorrectedYieldMinusAverageSquared    = 0;
                for (Int_t i=0; i< NumberOfCuts; i++){
                    //BinValues
                    dBinContentCorrectedYield                       =
                            histoAltCorrectedYieldCut[i]->GetBinContent(iBinCorrectedYieldAverage);
                    //Calculations with Bin Values
                    dBinContentCorrectedYieldMinusAverage           = dBinContentCorrectedYield-dCorrectedYieldAverageValue;
                    dBinContentCorrectedYieldMinusAverageSquared    = dBinContentCorrectedYieldMinusAverage*dBinContentCorrectedYieldMinusAverage;
                    dSumBinContentsCorrectedYieldMinusAverageSquared += dBinContentCorrectedYieldMinusAverageSquared;
                }
                dCorrectedYieldVarianceValue                        = dSumBinContentsCorrectedYieldMinusAverageSquared / (NumberOfCuts-1);
                dCorrectedYieldStandardDeviationValue               = TMath::Sqrt(dCorrectedYieldVarianceValue);
                histoAltCorrectedYieldCutAverage                    ->SetBinContent(iBinCorrectedYieldAverage, dCorrectedYieldAverageValue);
                histoAltCorrectedYieldCutAverage                    ->SetBinError(iBinCorrectedYieldAverage, dCorrectedYieldStandardDeviationValue);
            }

            //Calculate MAD
            if (DebugOutputLevel>=2){cout << "Debug; CompareDifferentDirectories.C, line " << __LINE__ << "; Calculate MAD" << endl;}
            for (Int_t i=0; i< NumberOfCuts; i++){
                dSumAbsDeviationCorrectedYieldAverageValue                              = 0;
                dSumAbsDeviationCorrectedYieldAverageValue_NormWStatErr                 = 0;
                for (Int_t iBinCorrectedYieldAverage=iStartBinCorrectedYieldAverage; iBinCorrectedYieldAverage<=iLastBinWithContent; iBinCorrectedYieldAverage++){
                    //BinValues
                    dBinContentCorrectedYield                                           =
                            histoAltCorrectedYieldCut[i]->GetBinContent(iBinCorrectedYieldAverage);
                    dBinErrorCorrectedYield                                             =
                            histoAltCorrectedYieldCut[i]->GetBinError(iBinCorrectedYieldAverage);
                    dCorrectedYieldAverageValue                                         =
                            histoAltCorrectedYieldCutAverage->GetBinContent(iBinCorrectedYieldAverage);
                    dCorrectedYieldStandardDeviationValue                               =
                            histoAltCorrectedYieldCutAverage->GetBinError(iBinCorrectedYieldAverage);
                    //Calculations with Bin Values
                    dCorrectedYieldVarianceValue                                        =
                            dCorrectedYieldStandardDeviationValue*dCorrectedYieldStandardDeviationValue;
                    dDeviationCorrectedYieldAverageValue                                =
                            dBinContentCorrectedYield-dCorrectedYieldAverageValue;
                    dDeviationCorrectedYieldAverageSquaredValue                         =
                            dDeviationCorrectedYieldAverageValue*dDeviationCorrectedYieldAverageValue;
                    if (dCorrectedYieldVarianceValue>0){
                        dDeviationCorrectedYieldAverageSquaredValueDivVar                   =
                                dDeviationCorrectedYieldAverageSquaredValue/dCorrectedYieldVarianceValue;
                    } else {
                        dDeviationCorrectedYieldAverageSquaredValueDivVar                   = 0.;
                    }
                    dDeviationCorrectedYieldAverageSquaredValueDivVar_NormWStatErr      =
                            dDeviationCorrectedYieldAverageSquaredValue/(dCorrectedYieldVarianceValue+(dBinErrorCorrectedYield*dBinErrorCorrectedYield));

                    dSumAbsDeviationCorrectedYieldAverageValue                          +=
                            dDeviationCorrectedYieldAverageSquaredValueDivVar;
                    dSumAbsDeviationCorrectedYieldAverageValue_NormWStatErr             +=
                            dDeviationCorrectedYieldAverageSquaredValueDivVar_NormWStatErr;
                }
                dCorrectedYieldAverage_MADValue                                         =
                        dSumAbsDeviationCorrectedYieldAverageValue / (iEndBinCorrectedYieldAverage-iStartBinCorrectedYieldAverage+1);
                dCorrectedYieldAverage_MADValue_NormWStatErr                           =
                        dSumAbsDeviationCorrectedYieldAverageValue_NormWStatErr / (iEndBinCorrectedYieldAverage-iStartBinCorrectedYieldAverage+1);


                histoAltCorrectedYieldCutAverage_MAD                                       ->GetXaxis()->SetBinLabel(i+1, cutStringsName[i].Data());
                histoAltCorrectedYieldCutAverage_MAD                                       ->SetBinContent(i+1, dCorrectedYieldAverage_MADValue);
                histoAltCorrectedYieldCutAverage_MAD_NormWStatErr                          ->GetXaxis()->SetBinLabel(i+1, cutStringsName[i].Data());
                histoAltCorrectedYieldCutAverage_MAD_NormWStatErr                          ->SetBinContent(i+1, dCorrectedYieldAverage_MADValue_NormWStatErr);
            }
            //Calculate MAD with MCInput
            for (Int_t i=0; i< NumberOfCuts; i++){
                if (doMCInput){
                    if (DebugOutputLevel>=1){cout << "Debug; CompareDifferentDirectories.C, line " << __LINE__ << "; Calculate MAD with MCInput" << endl;}
                    for (Int_t i=0; i< NumberOfCuts; i++){
                        dSumAbsDeviationCorrectedYieldMCInputValue                              = 0;
                        dSumAbsDeviationCorrectedYieldMCInputValue_NormWStatErr                 = 0;
                        for (Int_t iBinCorrectedYieldAverage=iStartBinCorrectedYieldAverage; iBinCorrectedYieldAverage<=iLastBinWithContent; iBinCorrectedYieldAverage++){
                            //BinValues
                            dBinContentCorrectedYield                                           =
                                    histoAltCorrectedYieldCut[i]->GetBinContent(iBinCorrectedYieldAverage);
                            dBinErrorCorrectedYield                                             =
                                    histoAltCorrectedYieldCut[i]->GetBinError(iBinCorrectedYieldAverage);
                            dCorrectedYieldAverageValue                                         =
                                    histoAltCorrectedYieldCutAverage->GetBinContent(iBinCorrectedYieldAverage);
                            dCorrectedYieldStandardDeviationValue                               =
                                    histoAltCorrectedYieldCutAverage->GetBinError(iBinCorrectedYieldAverage);
                            dCorrectedYieldMCInputValue                                         =
                                    histoMCInputYieldCut[0]->GetBinContent(iBinCorrectedYieldAverage);
                            dCorrectedYieldMCInputValue                                         =
                                    histoMCInputYieldCut[0]->GetBinContent(iBinCorrectedYieldAverage);
                            //Calculations with Bin Values
                            dCorrectedYieldVarianceValue                                        =
                                    dCorrectedYieldStandardDeviationValue*dCorrectedYieldStandardDeviationValue;
                            dDeviationCorrectedYieldMCInputValue                                =
                                    dBinContentCorrectedYield-dCorrectedYieldMCInputValue;
                            dDeviationCorrectedYieldMCInputSquaredValue                         =
                                    dDeviationCorrectedYieldMCInputValue*dDeviationCorrectedYieldMCInputValue;
                            if (dCorrectedYieldVarianceValue>0){
                                dDeviationCorrectedYieldMCInputSquaredValueDivVar               =
                                        dDeviationCorrectedYieldMCInputSquaredValue/dCorrectedYieldVarianceValue;
                            } else {
                                dDeviationCorrectedYieldMCInputSquaredValueDivVar               = 0.;
                            }
                            dDeviationCorrectedYieldMCInputSquaredValueDivVar_NormWStatErr      =
                                    dDeviationCorrectedYieldMCInputSquaredValue/(dCorrectedYieldVarianceValue+(dBinErrorCorrectedYield*dBinErrorCorrectedYield));

                            dSumAbsDeviationCorrectedYieldMCInputValue                          +=
                                    dDeviationCorrectedYieldMCInputSquaredValueDivVar;
                            dSumAbsDeviationCorrectedYieldMCInputValue_NormWStatErr             +=
                                    dDeviationCorrectedYieldMCInputSquaredValueDivVar_NormWStatErr;
                        }
                        dCorrectedYieldMCInput_MADValue                                         =
                                dSumAbsDeviationCorrectedYieldMCInputValue / (iEndBinCorrectedYieldAverage-iStartBinCorrectedYieldAverage+1);
                        dCorrectedYieldMCInput_MADValue_NormWStatErr                            =
                                dSumAbsDeviationCorrectedYieldMCInputValue_NormWStatErr / (iEndBinCorrectedYieldAverage-iStartBinCorrectedYieldAverage+1);

                        histoAltCorrectedYieldCutMCInput_MAD                                    ->GetXaxis()->SetBinLabel(i+1, cutStringsName[i].Data());
                        histoAltCorrectedYieldCutMCInput_MAD                                    ->SetBinContent(i+1, dCorrectedYieldMCInput_MADValue);
                        histoAltCorrectedYieldCutMCInput_MAD_NormWStatErr                       ->GetXaxis()->SetBinLabel(i+1, cutStringsName[i].Data());
                        histoAltCorrectedYieldCutMCInput_MAD_NormWStatErr                       ->SetBinContent(i+1, dCorrectedYieldMCInput_MADValue_NormWStatErr);
                    }
                }
            }
        }
        histoRatioAltCorrectedYieldCutAverage                                              =
                (TH1D*) histoAltCorrectedYieldCutAverage->Clone(Form("histoRatioAltCorrectedYieldCutAverage_%s",cutStringsName[0].Data()));
        if (doCorrectedYieldAverage){
            histoRatioAltCorrectedYieldCutAverage                                          ->Divide(histoRatioAltCorrectedYieldCutAverage, histoAltCorrectedYieldCut[0],1.,1.,"B");
        }



        if (DebugOutputLevel>=1){cout << "Debug; CompareDifferentDirectories.C, line " << __LINE__ << "; Compare Alternative Corrected Yields" << endl;}
        // Define canvas
        if(!plotOnlyUncorrectedOutput){
            TCanvas* canvasAltCorrectedYieldMeson = new TCanvas("canvasAltCorrectedYieldMeson","",1350,1500);
            DrawGammaCanvasSettings( canvasAltCorrectedYieldMeson,  0.13, 0.02, 0.02, 0.09);
            // Define upper panel
            TPad* padAltCorrectedYield = new TPad("padAltCorrectedYield", "", 0., 0.33, 1., 1.,-1, -1, -2);
            DrawGammaPadSettings( padAltCorrectedYield, 0.12, 0.02, 0.02, 0.);
            padAltCorrectedYield->SetLogy(1);
            padAltCorrectedYield->Draw();
            // Define lower panel
            TPad* padAltCorrectedYieldRatios = new TPad("padAltCorrectedYieldRatios", "", 0., 0., 1., 0.33,-1, -1, -2);
            DrawGammaPadSettings( padAltCorrectedYieldRatios, 0.12, 0.02, 0.0, 0.2);
            padAltCorrectedYieldRatios->SetLogy(0);
            padAltCorrectedYieldRatios->Draw();

            // Plot AltCorrected yield in upper panel
            padAltCorrectedYield->cd();
            TLegend* legendAltCorrectedYieldMeson = GetAndSetLegend2(0.15,0.02,0.3,0.02+1.15*0.032*NumberOfCuts, 1500*0.75*0.032);
            for(Int_t i = 0; i< NumberOfCuts; i++){
                if(i == 0){
                    DrawAutoGammaMesonHistos( histoAltCorrectedYieldCut[i],
                                              "", "#it{p}_{T} (GeV/#it{c})", "#frac{1}{2#pi #it{N}_{ev.}} #frac{d^{2}#it{N}}{#it{p}_{T}d#it{p}_{T}d#it{y}} (#it{c}/GeV)",
                                              kTRUE, 5., 5e-10,kTRUE,
                                              kFALSE, 0.0, 0.030,
                                              kFALSE, 0., 10.);
                    DrawGammaSetMarker(histoAltCorrectedYieldCut[i], 20, 1., color[0], color[0]);
                    histoAltCorrectedYieldCut[i]->DrawCopy("e1,p");
                    legendAltCorrectedYieldMeson->AddEntry(histoAltCorrectedYieldCut[i], Form("standard: %s",cutStringsName[i].Data()));

                    if ((doCorrectedYieldAverage)&&(histoAltCorrectedYieldCutAverage)){
                        Int_t ColorAndMarkerNumber=NumberOfCuts;
                        if(NumberOfCuts<20){
                            DrawGammaSetMarker(histoAltCorrectedYieldCutAverage, 20+ColorAndMarkerNumber, 1.,color[ColorAndMarkerNumber],color[ColorAndMarkerNumber]);
                        } else {
                            DrawGammaSetMarker(histoAltCorrectedYieldCutAverage, 20+ColorAndMarkerNumber, 1.,color[ColorAndMarkerNumber-20],color[ColorAndMarkerNumber-20]);
                        }
                        histoAltCorrectedYieldCutAverage->DrawCopy("same,e1,p");
                        legendAltCorrectedYieldMeson->AddEntry(histoAltCorrectedYieldCutAverage, "Average Values");
                    }
                    if (doMCInput){
                        Int_t ColorAndMarkerNumber=NumberOfCuts;
                        if ((doCorrectedYieldAverage)&&(histoAltCorrectedYieldCutAverage)){
                          ColorAndMarkerNumber=NumberOfCuts+1;
                        }
                        if(NumberOfCuts<20){
                            DrawGammaSetMarker(histoMCInputYieldCut[0], 20+ColorAndMarkerNumber, 1.,color[ColorAndMarkerNumber],color[ColorAndMarkerNumber]);
                        } else {
                            DrawGammaSetMarker(histoMCInputYieldCut[0], 20+ColorAndMarkerNumber, 1.,color[ColorAndMarkerNumber-20],color[ColorAndMarkerNumber-20]);
                        }
                        histoMCInputYieldCut[0]->DrawCopy("same,e1,p");
                        legendAltCorrectedYieldMeson->AddEntry(histoMCInputYieldCut[0], "MC Input");
                    }
                }
                else{
                    if(i<20){
                        DrawGammaSetMarker(histoAltCorrectedYieldCut[i], 20+i, 1.,color[i],color[i]);
                    } else {
                        DrawGammaSetMarker(histoAltCorrectedYieldCut[i], 20+i, 1.,color[i-20],color[i-20]);
                    }
                    histoAltCorrectedYieldCut[i]->DrawCopy("same,e1,p");
                    legendAltCorrectedYieldMeson->AddEntry(histoAltCorrectedYieldCut[i], cutStringsName[i].Data());
                }

            }
            legendAltCorrectedYieldMeson->Draw();
            // Labeling of plot
            PutProcessLabelAndEnergyOnPlot( 0.94, 0.95, 0.032, collisionSystem, process, detectionProcess, 42, 0.03, optionPeriod, 1, 1.25, 31);

            // plot ratio of AltCorrected yields in lower panel
            padAltCorrectedYieldRatios->cd();
            for(Int_t i = 0; i< NumberOfCuts; i++){
                if(i==0){
                    // Set ratio min and max
                    Double_t minYRatio = 0.75;
                    Double_t maxYRatio = 1.25;
                    if (mode != 0 && mode!= 1 ){
                        minYRatio = 0.75;
                        maxYRatio = 1.55;
                    }
                    if (cutVariationName.Contains("LHC12NL") || cutVariationName.Contains("LHC12-MultWeight")){
                        minYRatio = 0.81;
                        maxYRatio = 1.19;
                    } else if (cutVariationName.Contains("AODvalidation")){
                        minYRatio = 0.8;
                        maxYRatio = 1.2;
                    } else if (cutVariationName.Contains("PHOS_Combined")) {
                        minYRatio = 0.90;
                        maxYRatio = 1.15;
                    }  else if (meson.Contains("Omega")) {
                        minYRatio = 0.1;
                        maxYRatio = 2.55;
                    }
                    SetStyleHistoTH1ForGraphs(histoRatioAltCorrectedYieldCut[i], "#it{p}_{T} (GeV/#it{c})", "#frac{modified}{standard}", 0.08, 0.11, 0.07, 0.1, 0.75, 0.5, 510,505);
                    DrawGammaSetMarker(histoRatioAltCorrectedYieldCut[i], 20, 1.,color[0],color[0]);
                    histoRatioAltCorrectedYieldCut[i]->GetYaxis()->SetRangeUser(minYRatio,maxYRatio);
                    histoRatioAltCorrectedYieldCut[i]->DrawCopy("p,e1");

                    if ((doCorrectedYieldAverage)&&(histoRatioAltCorrectedYieldCutAverage)){
                        Int_t ColorAndMarkerNumber=NumberOfCuts;
                        if(ColorAndMarkerNumber<20){
                            DrawGammaSetMarker(histoRatioAltCorrectedYieldCutAverage, 20+ColorAndMarkerNumber, 1.,color[ColorAndMarkerNumber],color[ColorAndMarkerNumber]);
                        } else {
                            DrawGammaSetMarker(histoRatioAltCorrectedYieldCutAverage, 20+ColorAndMarkerNumber, 1.,color[ColorAndMarkerNumber-20],color[ColorAndMarkerNumber-20]);
                        }
                        histoRatioAltCorrectedYieldCutAverage->DrawCopy("same,e1,p");
                    }
                    if (doMCInput){
                        Int_t ColorAndMarkerNumber=NumberOfCuts;
                        if ((doCorrectedYieldAverage)&&(histoRatioAltCorrectedYieldCutAverage)){
                          ColorAndMarkerNumber=NumberOfCuts+1;
                        }
                        if(ColorAndMarkerNumber<20){
                            DrawGammaSetMarker(histoRatioToAltCorYieldMCInputYieldCut[0], 20+ColorAndMarkerNumber, 1.,color[ColorAndMarkerNumber],color[ColorAndMarkerNumber]);
                        } else {
                            DrawGammaSetMarker(histoRatioToAltCorYieldMCInputYieldCut[0], 20+ColorAndMarkerNumber, 1.,color[ColorAndMarkerNumber-20],color[ColorAndMarkerNumber-20]);
                        }
                        histoRatioToAltCorYieldMCInputYieldCut[0]->DrawCopy("same,e1,p");
                    }
                }
                else{
                    if(i<20){
                        DrawGammaSetMarker(histoRatioAltCorrectedYieldCut[i], 20+i, 1.,color[i],color[i]);
                    } else {
                        DrawGammaSetMarker(histoRatioAltCorrectedYieldCut[i], 20+i, 1.,color[i-20],color[i-20]);
                    }
                    histoRatioAltCorrectedYieldCut[i]->DrawCopy("same,e1,p");
                }
            }
            DrawGammaLines(0., maxPt,1., 1.,1);

            canvasAltCorrectedYieldMeson->Update();
            canvasAltCorrectedYieldMeson->SaveAs(Form("%s/%s_%s_AltCorrectedYield.%s",outputDir.Data(), meson.Data(),prefix2.Data(),suffix.Data()));
            delete canvasAltCorrectedYieldMeson;
        }
    }

    //*****************************************************************************************
    //******************* Write Corrected Yields Average to File*******************************
    //*****************************************************************************************
    if (doCorrectedYieldAverage){
        TString FileName_CorrectedYieldAverage_MAD    = "CorrectedYieldAverage_MAD";
        TFile *OutputFile_CorrectedYieldAverage_MAD   = new TFile(Form("%s/%s.root", outputDir.Data(), FileName_CorrectedYieldAverage_MAD.Data()), "RECREATE");

        TCanvas* canvasCorrectedYieldMesonAverage_MAD = new TCanvas("canvasCorrectedYieldMesonAverage_MAD","",1350,1500);
        histoCorrectedYieldCutAverage_MAD->GetYaxis()->SetTitle("#Sum ((CorrYield - Average)^2 / #sigma^2)");
        histoCorrectedYieldCutAverage_MAD->Draw();
        canvasCorrectedYieldMesonAverage_MAD->Update();
        canvasCorrectedYieldMesonAverage_MAD->SaveAs(Form("%s/%s_%s_CorrectedYieldAverage_MAD.%s",outputDir.Data(), meson.Data(),prefix2.Data(),suffix.Data()));
        if ( OutputFile_CorrectedYieldAverage_MAD->IsOpen() ){histoCorrectedYieldCutAverage_MAD->Write();}
        delete canvasCorrectedYieldMesonAverage_MAD;

        TCanvas* canvasCorrectedYieldMesonAverage_MAD_NormWStatErr = new TCanvas("canvasCorrectedYieldMesonAverage_MAD_NormWStatErr","",1350,1500);
        histoCorrectedYieldCutAverage_MAD_NormWStatErr->GetYaxis()->SetTitle("#Sum ((CorrYield - Average)^2 / (#sigma^2+statErr^2)");
        histoCorrectedYieldCutAverage_MAD_NormWStatErr->Draw();
        canvasCorrectedYieldMesonAverage_MAD_NormWStatErr->Update();
        canvasCorrectedYieldMesonAverage_MAD_NormWStatErr->SaveAs(Form("%s/%s_%s_CorrectedYieldAverage_MAD_NormWStatErr.%s",outputDir.Data(), meson.Data(),prefix2.Data(),suffix.Data()));
        if ( OutputFile_CorrectedYieldAverage_MAD->IsOpen() ){histoCorrectedYieldCutAverage_MAD_NormWStatErr->Write();}
        delete canvasCorrectedYieldMesonAverage_MAD_NormWStatErr;

        if (doMCInput){
            TCanvas* canvasCorrectedYieldMesonMCInput_MAD = new TCanvas("canvasCorrectedYieldMesonMCInput_MAD","",1350,1500);
            histoCorrectedYieldCutMCInput_MAD->GetYaxis()->SetTitle("#Sum ((CorrYield - MCInput)^2 / #sigma^2)");
            histoCorrectedYieldCutMCInput_MAD->Draw();
            canvasCorrectedYieldMesonMCInput_MAD->Update();
            canvasCorrectedYieldMesonMCInput_MAD->SaveAs(Form("%s/%s_%s_CorrectedYieldMCInput_MAD.%s",outputDir.Data(), meson.Data(),prefix2.Data(),suffix.Data()));
            if ( OutputFile_CorrectedYieldAverage_MAD->IsOpen() ){histoCorrectedYieldCutMCInput_MAD->Write();}
            delete canvasCorrectedYieldMesonMCInput_MAD;

            TCanvas* canvasCorrectedYieldMesonMCInput_MAD_NormWStatErr = new TCanvas("canvasCorrectedYieldMesonMCInput_MAD_NormWStatErr","",1350,1500);
            histoCorrectedYieldCutMCInput_MAD_NormWStatErr->GetYaxis()->SetTitle("#Sum ((CorrYield - MCInput)^2 / (#sigma^2+statErr^2)");
            histoCorrectedYieldCutMCInput_MAD_NormWStatErr->Draw();
            canvasCorrectedYieldMesonMCInput_MAD_NormWStatErr->Update();
            canvasCorrectedYieldMesonMCInput_MAD_NormWStatErr->SaveAs(Form("%s/%s_%s_CorrectedYieldMCInput_MAD_NormWStatErr.%s",outputDir.Data(), meson.Data(),prefix2.Data(),suffix.Data()));
            if ( OutputFile_CorrectedYieldAverage_MAD->IsOpen() ){histoCorrectedYieldCutMCInput_MAD_NormWStatErr->Write();}
            delete canvasCorrectedYieldMesonMCInput_MAD_NormWStatErr;
        }

        if (doAlternativeCorrectedYield){
            TCanvas* canvasAltCorrectedYieldMesonAverage_MAD = new TCanvas("canvasAltCorrectedYieldMesonAverage_MAD","",1350,1500);
            histoAltCorrectedYieldCutAverage_MAD->GetYaxis()->SetTitle("#Sum ((CorrYield - Average)^2 / #sigma^2)");
            histoAltCorrectedYieldCutAverage_MAD->Draw();
            canvasAltCorrectedYieldMesonAverage_MAD->Update();
            if (!kIsMC){ //Makes no real sense as Values are set 0, else division through zero; therefore not plotted
                canvasAltCorrectedYieldMesonAverage_MAD->SaveAs(Form("%s/%s_%s_AltCorrectedYieldAverage_MAD.%s",outputDir.Data(), meson.Data(),prefix2.Data(),suffix.Data()));
                if ( OutputFile_CorrectedYieldAverage_MAD->IsOpen() ){histoAltCorrectedYieldCutAverage_MAD->Write();}
            }
            delete canvasAltCorrectedYieldMesonAverage_MAD;

            TCanvas* canvasAltCorrectedYieldMesonAverage_MAD_NormWStatErr = new TCanvas("canvasAltCorrectedYieldMesonAverage_MAD_NormWStatErr","",1350,1500);
            histoAltCorrectedYieldCutAverage_MAD_NormWStatErr->GetYaxis()->SetTitle("#Sum ((CorrYield - Average)^2 / (#sigma^2+statErr^2)");
            histoAltCorrectedYieldCutAverage_MAD_NormWStatErr->Draw();
            canvasAltCorrectedYieldMesonAverage_MAD_NormWStatErr->Update();
            canvasAltCorrectedYieldMesonAverage_MAD_NormWStatErr->SaveAs(Form("%s/%s_%s_AltCorrectedYieldAverage_MAD_NormWStatErr.%s",outputDir.Data(), meson.Data(),prefix2.Data(),suffix.Data()));
            if ( OutputFile_CorrectedYieldAverage_MAD->IsOpen() ){histoAltCorrectedYieldCutAverage_MAD_NormWStatErr->Write();}
            delete canvasAltCorrectedYieldMesonAverage_MAD_NormWStatErr;

            if (doMCInput){
                TCanvas* canvasAltCorrectedYieldMesonMCInput_MAD = new TCanvas("canvasAltCorrectedYieldMesonMCInput_MAD","",1350,1500);
                histoAltCorrectedYieldCutMCInput_MAD->GetYaxis()->SetTitle("#Sum ((CorrYield - MCInput)^2 / #sigma^2)");
                histoAltCorrectedYieldCutMCInput_MAD->Draw();
                canvasAltCorrectedYieldMesonMCInput_MAD->Update();
                if (!kIsMC){ //Makes no real sense as Values are set 0, else division through zero; therefore not plotted
                    canvasAltCorrectedYieldMesonMCInput_MAD->SaveAs(Form("%s/%s_%s_AltCorrectedYieldMCInput_MAD.%s",outputDir.Data(), meson.Data(),prefix2.Data(),suffix.Data()));
                    if ( OutputFile_CorrectedYieldAverage_MAD->IsOpen() ){histoAltCorrectedYieldCutMCInput_MAD->Write();}
                }
                delete canvasAltCorrectedYieldMesonMCInput_MAD;

                TCanvas* canvasAltCorrectedYieldMesonMCInput_MAD_NormWStatErr = new TCanvas("canvasAltCorrectedYieldMesonMCInput_MAD_NormWStatErr","",1350,1500);
                histoAltCorrectedYieldCutMCInput_MAD_NormWStatErr->GetYaxis()->SetTitle("#Sum ((CorrYield - MCInput)^2 / (#sigma^2+statErr^2)");
                histoAltCorrectedYieldCutMCInput_MAD_NormWStatErr->Draw();
                canvasAltCorrectedYieldMesonMCInput_MAD_NormWStatErr->Update();
                canvasAltCorrectedYieldMesonMCInput_MAD_NormWStatErr->SaveAs(Form("%s/%s_%s_AltCorrectedYieldMCInput_MAD_NormWStatErr.%s",outputDir.Data(), meson.Data(),prefix2.Data(),suffix.Data()));
                if ( OutputFile_CorrectedYieldAverage_MAD->IsOpen() ){histoAltCorrectedYieldCutMCInput_MAD_NormWStatErr->Write();}
                delete canvasAltCorrectedYieldMesonMCInput_MAD_NormWStatErr;
            }

        }

        if (OutputFile_CorrectedYieldAverage_MAD) delete OutputFile_CorrectedYieldAverage_MAD;
    }


    //*****************************************************************************************
    //***************** Compare Corrected Yields without Secondary Correction *****************
    //*****************************************************************************************
    if (DebugOutputLevel>=1){cout << "Debug; CompareDifferentDirectories.C, line " << __LINE__ << "; Compare Corrected Yields without Secondary Correction" << endl;}
    // Define canvas{
    if((!plotOnlyUncorrectedOutput)&&(doCorrectedYieldWOSecCut)){
      TCanvas* canvasCorrectedYieldWOSecMeson = new TCanvas("canvasCorrectedYieldWOSecMeson","",1350,1500);
      DrawGammaCanvasSettings( canvasCorrectedYieldWOSecMeson,  0.13, 0.02, 0.02, 0.09);
      // Define upper panel
      TPad* padCorrectedYieldWOSec = new TPad("padCorrectedYieldWOSec", "", 0., 0.33, 1., 1.,-1, -1, -2);
      DrawGammaPadSettings( padCorrectedYieldWOSec, 0.12, 0.02, 0.02, 0.);
      padCorrectedYieldWOSec->SetLogy(1);
      padCorrectedYieldWOSec->Draw();
      // Define lower panel
      TPad* padCorrectedYieldWOSecRatios = new TPad("padCorrectedYieldWOSecRatios", "", 0., 0., 1., 0.33,-1, -1, -2);
      DrawGammaPadSettings( padCorrectedYieldWOSecRatios, 0.12, 0.02, 0.0, 0.2);
      padCorrectedYieldWOSecRatios->SetLogy(0);
      padCorrectedYieldWOSecRatios->Draw();

      // Plot corrected yield without secondary correction in upper panel
      padCorrectedYieldWOSec->cd();
          TLegend* legendCorrectedYieldWOSecMeson = GetAndSetLegend2(0.15,0.02,0.3,0.02+1.15*0.032*NumberOfCuts, 1500*0.75*0.032);
          for(Int_t i = 0; i< NumberOfCuts; i++){
              if(i == 0){
                  DrawAutoGammaMesonHistos( histoCorrectedYieldWOSecCut[i],
                                          "", "#it{p}_{T} (GeV/#it{c})", "#frac{1}{2#pi #it{N}_{ev.}} #frac{d^{2}#it{N}}{#it{p}_{T}d#it{p}_{T}d#it{y}} (#it{c}/GeV)",
                                          kTRUE, 5., 5e-10,kTRUE,
                                          kFALSE, 0.0, 0.030,
                                          kFALSE, 0., 10.);
                  DrawGammaSetMarker(histoCorrectedYieldWOSecCut[i], 20, 1., color[0], color[0]);
                  histoCorrectedYieldWOSecCut[i]->DrawCopy("e1,p");
                  legendCorrectedYieldWOSecMeson->AddEntry(histoCorrectedYieldWOSecCut[i], Form("standard: %s",cutStringsName[i].Data()));
              }
              else{
                  if(i<20){
                      DrawGammaSetMarker(histoCorrectedYieldWOSecCut[i], 20+i, 1.,color[i],color[i]);
                  } else {
                      DrawGammaSetMarker(histoCorrectedYieldWOSecCut[i], 20+i, 1.,color[i-20],color[i-20]);
                  }
                  histoCorrectedYieldWOSecCut[i]->DrawCopy("same,e1,p");
                  legendCorrectedYieldWOSecMeson->AddEntry(histoCorrectedYieldWOSecCut[i], cutStringsName[i].Data());
              }

          }
          legendCorrectedYieldWOSecMeson->Draw();
          // Labeling of plot
          PutProcessLabelAndEnergyOnPlot( 0.94, 0.95, 0.032, collisionSystem, process, detectionProcess, 42, 0.03, optionPeriod, 1, 1.25, 31);

          // plot ratio of corrected yields without secondary correction in lower panel
          padCorrectedYieldWOSecRatios->cd();
          for(Int_t i = 0; i< NumberOfCuts; i++){
              if(i==0){
                  // Set ratio min and max
                  Double_t minYRatio = 0.45;
                  Double_t maxYRatio = 1.55;
                  if (mode != 0 && mode!= 1 ){
                      minYRatio = 0.75;
                      maxYRatio = 1.55;
                  }
                  if (cutVariationName.Contains("LHC12NL") || cutVariationName.Contains("LHC12-MultWeight")){
                      minYRatio = 0.81;
                      maxYRatio = 1.19;
                  } else if (cutVariationName.Contains("AODvalidation")){
                      minYRatio = 0.8;
                      maxYRatio = 1.2;
                  } else if (cutVariationName.Contains("PHOS_Combined")) {
                      minYRatio = 0.90;
                      maxYRatio = 1.15;
                  }
                  SetStyleHistoTH1ForGraphs(histoRatioCorrectedYieldWOSecCut[i], "#it{p}_{T} (GeV/#it{c})", "#frac{modified}{standard}", 0.08, 0.11, 0.07, 0.1, 0.75, 0.5, 510,505);
                  DrawGammaSetMarker(histoRatioCorrectedYieldWOSecCut[i], 20, 1.,color[0],color[0]);
                  histoRatioCorrectedYieldWOSecCut[i]->GetYaxis()->SetRangeUser(minYRatio,maxYRatio);
                  histoRatioCorrectedYieldWOSecCut[i]->DrawCopy("p,e1");
              }
              else{
                  if(i<20){
                      DrawGammaSetMarker(histoRatioCorrectedYieldWOSecCut[i], 20+i, 1.,color[i],color[i]);
                  } else {
                      DrawGammaSetMarker(histoRatioCorrectedYieldWOSecCut[i], 20+i, 1.,color[i-20],color[i-20]);
                  }
                  histoRatioCorrectedYieldWOSecCut[i]->DrawCopy("same,e1,p");
              }
          }
          DrawGammaLines(0., maxPt,1., 1.,1);

      canvasCorrectedYieldWOSecMeson->Update();
      canvasCorrectedYieldWOSecMeson->SaveAs(Form("%s/%s_%s_CorrectedYieldWOSec.%s",outputDir.Data(), meson.Data(),prefix2.Data(),suffix.Data()));
      delete canvasCorrectedYieldWOSecMeson;
    }

    //*****************************************************************************************
    //********************** Compare MC Input Yield *******************************************
    //*****************************************************************************************
    if (doMCInput){
        if (DebugOutputLevel>=1){cout << "Debug; CompareDifferentDirectories.C, line " << __LINE__ << "; Compare Alternative Corrected Yields" << endl;}
        // Define canvas
        if(!plotOnlyUncorrectedOutput){
            TCanvas* canvasMCInputYieldMeson = new TCanvas("canvasMCInputYieldMeson","",1350,1500);
            DrawGammaCanvasSettings( canvasMCInputYieldMeson,  0.13, 0.02, 0.02, 0.09);
            // Define upper panel
            TPad* padMCInputYield = new TPad("padMCInputYield", "", 0., 0.33, 1., 1.,-1, -1, -2);
            DrawGammaPadSettings( padMCInputYield, 0.12, 0.02, 0.02, 0.);
            padMCInputYield->SetLogy(1);
            padMCInputYield->Draw();
            // Define lower panel
            TPad* padMCInputYieldRatios = new TPad("padMCInputYieldRatios", "", 0., 0., 1., 0.33,-1, -1, -2);
            DrawGammaPadSettings( padMCInputYieldRatios, 0.12, 0.02, 0.0, 0.2);
            padMCInputYieldRatios->SetLogy(0);
            padMCInputYieldRatios->Draw();

            // Plot AltCorrected yield in upper panel
            padMCInputYield->cd();
            TLegend* legendMCInputYieldMeson = GetAndSetLegend2(0.15,0.02,0.3,0.02+1.15*0.032*NumberOfCuts, 1500*0.75*0.032);
            for(Int_t i = 0; i< NumberOfCuts; i++){
                if(i == 0){
                    DrawAutoGammaMesonHistos( histoMCInputYieldCut[i],
                                              "", "#it{p}_{T} (GeV/#it{c})", "#frac{1}{2#pi #it{N}_{ev.}} #frac{d^{2}#it{N}}{#it{p}_{T}d#it{p}_{T}d#it{y}} (#it{c}/GeV)",
                                              kTRUE, 5., 5e-10,kTRUE,
                                              kFALSE, 0.0, 0.030,
                                              kFALSE, 0., 10.);
                    DrawGammaSetMarker(histoMCInputYieldCut[i], 20, 1., color[0], color[0]);
                    histoMCInputYieldCut[i]->DrawCopy("e1,p");
                    legendMCInputYieldMeson->AddEntry(histoMCInputYieldCut[i], Form("standard: %s",cutStringsName[i].Data()));
                }
                else{
                    if(i<20){
                        DrawGammaSetMarker(histoMCInputYieldCut[i], 20+i, 1.,color[i],color[i]);
                    } else {
                        DrawGammaSetMarker(histoMCInputYieldCut[i], 20+i, 1.,color[i-20],color[i-20]);
                    }
                    histoMCInputYieldCut[i]->DrawCopy("same,e1,p");
                    legendMCInputYieldMeson->AddEntry(histoMCInputYieldCut[i], cutStringsName[i].Data());
                }

            }
            legendMCInputYieldMeson->Draw();
            // Labeling of plot
            PutProcessLabelAndEnergyOnPlot( 0.94, 0.95, 0.032, collisionSystem, process, detectionProcess, 42, 0.03, optionPeriod, 1, 1.25, 31);

            // plot ratio of AltCorrected yields in lower panel
            padMCInputYieldRatios->cd();
            for(Int_t i = 0; i< NumberOfCuts; i++){
                if(i==0){
                    // Set ratio min and max
                    Double_t minYRatio = 0.75;
                    Double_t maxYRatio = 1.25;
                    if (mode != 0 && mode!= 1 ){
                        minYRatio = 0.75;
                        maxYRatio = 1.55;
                    }
                    if (cutVariationName.Contains("LHC12NL") || cutVariationName.Contains("LHC12-MultWeight")){
                        minYRatio = 0.81;
                        maxYRatio = 1.19;
                    } else if (cutVariationName.Contains("AODvalidation")){
                        minYRatio = 0.8;
                        maxYRatio = 1.2;
                    } else if (cutVariationName.Contains("PHOS_Combined")) {
                        minYRatio = 0.90;
                        maxYRatio = 1.15;
                    }  else if (meson.Contains("Omega")) {
                        minYRatio = 0.1;
                        maxYRatio = 2.55;
                    }
                    SetStyleHistoTH1ForGraphs(histoRatioMCInputYieldCut[i], "#it{p}_{T} (GeV/#it{c})", "#frac{modified}{standard}", 0.08, 0.11, 0.07, 0.1, 0.75, 0.5, 510,505);
                    DrawGammaSetMarker(histoRatioMCInputYieldCut[i], 20, 1.,color[0],color[0]);
                    histoRatioMCInputYieldCut[i]->GetYaxis()->SetRangeUser(minYRatio,maxYRatio);
                    histoRatioMCInputYieldCut[i]->DrawCopy("p,e1");
                }
                else{
                    if(i<20){
                        DrawGammaSetMarker(histoRatioMCInputYieldCut[i], 20+i, 1.,color[i],color[i]);
                    } else {
                        DrawGammaSetMarker(histoRatioMCInputYieldCut[i], 20+i, 1.,color[i-20],color[i-20]);
                    }
                    histoRatioMCInputYieldCut[i]->DrawCopy("same,e1,p");
                }
            }
            DrawGammaLines(0., maxPt,1., 1.,1);

            canvasMCInputYieldMeson->Update();
            canvasMCInputYieldMeson->SaveAs(Form("%s/%s_%s_MCInputYield.%s",outputDir.Data(), meson.Data(),prefix2.Data(),suffix.Data()));
            delete canvasMCInputYieldMeson;
        }
    }
    //**************************************************************************************
    //********************* Plotting Efficiency *********************************************
    //**************************************************************************************
    if (DebugOutputLevel>=1){cout << "Debug; CompareDifferentDirectories.C, line " << __LINE__ << "; Plotting Efficiency" << endl;}
    TLatex *labelCollisionSystem4 = new TLatex(0.55,0.10,collisionSystem.Data());
    SetStyleTLatex( labelCollisionSystem4, 0.038,4);
    TLatex* labelDetProcess2 = NULL;
    // Define canvas
    if(!plotOnlyUncorrectedOutput){
      TCanvas* canvasTrueEffiMeson = new TCanvas("canvasTrueEffiMeson","",1350,1500);  // gives the page size
      DrawGammaCanvasSettings( canvasTrueEffiMeson,  0.13, 0.02, 0.02, 0.09);
      // Define upper panel
      TPad* padTrueEffi = new TPad("padTrueEffi", "", 0., 0.4, 1., 1.,-1, -1, -2);
      DrawGammaPadSettings( padTrueEffi, 0.12, 0.02, 0.04, 0.);
      padTrueEffi->Draw();
      // Define lower panel
      TPad* padTrueEffiRatios = new TPad("padTrueEffiRatios", "", 0., 0., 1., 0.4,-1, -1, -2);
      DrawGammaPadSettings( padTrueEffiRatios, 0.12, 0.02, 0.0, 0.2);
      padTrueEffiRatios->Draw();

        // draw efficiency in upper panel
        padTrueEffi->cd();
  //       if (mode == 2 || mode == 3 ) padTrueEffi->SetLogy(1);
  //       else padTrueEffi->SetLogy(0);

        TLegend* legendEffiMeson = GetAndSetLegend2(0.15,0.93-0.03*NumberOfCuts,0.3,0.93, 1500*0.75*0.032);
        for(Int_t i = 0; i< NumberOfCuts; i++){
            if(i == 0){
                DrawGammaSetMarker(histoTrueEffiCut[i], 20, 1., color[0], color[0]);
                DrawAutoGammaMesonHistos( histoTrueEffiCut[i],
                                          "", "#it{p}_{T} (GeV/#it{c})", Form("#epsilon_{%s}",textMeson.Data()),
                                          kTRUE, 5., 10e-10,kFALSE,
                                          kTRUE, -0.1, 0.00030,
                                          kFALSE, 0., 10.);
                if (mode == 9 || mode == 0 )histoTrueEffiCut[i]->GetYaxis()->SetRangeUser(0.0,0.003);
                if (mode == 2 || mode == 3 )histoTrueEffiCut[i]->GetYaxis()->SetRangeUser(0,0.1);
                if (mode == 4 || mode == 5 ){
                    if (optionEnergy.Contains("PbPb_2") && meson.Contains("Pi0") )
                        histoTrueEffiCut[i]->GetYaxis()->SetRangeUser(0,0.4);
                    else if (optionEnergy.Contains("PbPb_5") && meson.Contains("Pi0") )
                        histoTrueEffiCut[i]->GetYaxis()->SetRangeUser(-0.05,0.25);
                    else if (optionEnergy.Contains("pPb") && meson.Contains("Pi0") )
                        histoTrueEffiCut[i]->GetYaxis()->SetRangeUser(0.,0.4);
                    else if (optionEnergy.Contains("PbPb_2.76TeV") )
                        histoTrueEffiCut[i]->GetYaxis()->SetRangeUser(0,0.6);
                    else
                        histoTrueEffiCut[i]->GetYaxis()->SetRangeUser(0,0.6);
                } else if ( mode == modePCM_Omega || mode == modePCMEMCAL_Omega || mode == modePCMPHOS_Omega || mode == modeEMCAL_Omega || mode == modePHOS_Omega ){
                    if (meson.Contains("Omega") ){
                        histoTrueEffiCut[i]->GetYaxis()->SetRangeUser(-0.01,0.15);
                    }
                }
                histoTrueEffiCut[i]->DrawCopy("e1,p");
                legendEffiMeson->AddEntry(histoTrueEffiCut[i],Form("standard: %s",cutStringsName[i].Data()));
            } else {
                if(i<20){
                    DrawGammaSetMarker(histoTrueEffiCut[i], 20+i, 1.,color[i],color[i]);
                } else {
                    DrawGammaSetMarker(histoTrueEffiCut[i], 20+i, 1.,color[i-20],color[i-20]);
                }
                histoTrueEffiCut[i]->DrawCopy("same,e1,p");
                legendEffiMeson->AddEntry(histoTrueEffiCut[i],cutStringsName[i].Data());
            }
        }
        legendEffiMeson->Draw();

        // Efficiency plot labeling
        PutProcessLabelAndEnergyOnPlot( 0.94, 0.2, 0.032, collisionSystem, process, detectionProcess, 42, 0.03, optionPeriod, 1, 1.25, 31);

        // Draw ratio of efficiencies in lower panel
        padTrueEffiRatios->cd();
        if( optionEnergy.Contains("Pb") ) padTrueEffiRatios->SetLogy(0);
        else padTrueEffiRatios->SetLogy(0);

        for(Int_t i = 0; i< NumberOfCuts; i++){
            if(i==0){
                Double_t minYRatio = 0.45;
                Double_t maxYRatio = 1.55;
                if (cutVariationName.Contains("Weighting")){
                      minYRatio = 0.75;
                      maxYRatio = 1.25;
                }
                if (cutVariationName.Contains("LHC12NL") || cutVariationName.Contains("LHC12-MultWeight")){
                    minYRatio = 0.81;
                    maxYRatio = 1.19;
                }else if (cutVariationName.Contains("AODvalidation")){
                      minYRatio = 0.9;
                      maxYRatio = 1.09;
                } else if (cutVariationName.Contains("PHOS_Combined")) {
                    minYRatio = 0.90;
                    maxYRatio = 1.15;
                } else if (meson.Contains("Omega")) {
                    minYRatio = 0.6;
                    maxYRatio = 1.4;
                }
                SetStyleHistoTH1ForGraphs(histoRatioTrueEffiCut[i], "#it{p}_{T} (GeV/#it{c})", "#frac{modified}{standard}", 0.05, 0.07, 0.05, 0.07, 0.75, 0.5, 510,505);
                DrawGammaSetMarker(histoRatioTrueEffiCut[i], 20, 1.,color[0],color[0]);
                histoRatioTrueEffiCut[i]->GetYaxis()->SetRangeUser(minYRatio,maxYRatio);
                histoRatioTrueEffiCut[i]->DrawCopy("p,e1");
            } else{
                if(i<20){
                    DrawGammaSetMarker(histoRatioTrueEffiCut[i], 20+i, 1.,color[i],color[i]);
                } else {
                    DrawGammaSetMarker(histoRatioTrueEffiCut[i], 20+i, 1.,color[i-20],color[i-20]);
                }
                histoRatioTrueEffiCut[i]->DrawCopy("same,e1,p");
            }
            DrawGammaLines(0., maxPt,1., 1.,1);
            DrawGammaLines(0., maxPt, 1.1, 1.1, 1, kGray+1, 7);
            DrawGammaLines(0., maxPt, 0.9, 0.9, 1, kGray+1, 7);
            if (cutVariationName.Contains("Weighting")){
              DrawGammaLines(0., maxPt, 1.05, 1.05, 1, kGray+1, 9);
              DrawGammaLines(0., maxPt, 0.95, 0.95, 1, kGray+1, 9);
            }
        }

      canvasTrueEffiMeson->Update();
      canvasTrueEffiMeson->SaveAs(Form("%s/%s_%s_Efficiencies.%s",outputDir.Data(),meson.Data(),prefix2.Data(),suffix.Data()));
      delete canvasTrueEffiMeson;
    }

    //**************************************************************************************
    //********************* Plotting Alternative Efficiency ********************************
    //**************************************************************************************
    if (doAlternativeEfficiency){
        if (DebugOutputLevel>=1){cout << "Debug; CompareDifferentDirectories.C, line " << __LINE__ << "; Plotting Alternative Efficiency" << endl;}
        // Define canvas
        if(!plotOnlyUncorrectedOutput){
          TCanvas* canvasAltEffiMeson = new TCanvas("canvasAltEffiMeson","",1350,1500);  // gives the page size
          DrawGammaCanvasSettings( canvasAltEffiMeson,  0.13, 0.02, 0.02, 0.09);
          // Define upper panel
          TPad* padAltEffi = new TPad("padAltEffi", "", 0., 0.4, 1., 1.,-1, -1, -2);
          DrawGammaPadSettings( padAltEffi, 0.12, 0.02, 0.04, 0.);
          padAltEffi->Draw();
          // Define lower panel
          TPad* padAltEffiRatios = new TPad("padAltEffiRatios", "", 0., 0., 1., 0.4,-1, -1, -2);
          DrawGammaPadSettings( padAltEffiRatios, 0.12, 0.02, 0.0, 0.2);
          padAltEffiRatios->Draw();

            // draw efficiency in upper panel
            padAltEffi->cd();
      //       if (mode == 2 || mode == 3 ) padAltEffi->SetLogy(1);
      //       else padAltEffi->SetLogy(0);

            TLegend* legendEffiMeson = GetAndSetLegend2(0.15,0.93-0.03*NumberOfCuts,0.3,0.93, 1500*0.75*0.032);
            for(Int_t i = 0; i< NumberOfCuts; i++){
                if(i == 0){
                    DrawGammaSetMarker(histoAltEffiCut[i], 20, 1., color[0], color[0]);
                    DrawAutoGammaMesonHistos( histoAltEffiCut[i],
                                              "", "#it{p}_{T} (GeV/#it{c})", Form("#epsilon_{%s}",textMeson.Data()),
                                              kTRUE, 5., 10e-10,kFALSE,
                                              kTRUE, -0.1, 0.00030,
                                              kFALSE, 0., 10.);
                    if (mode == 9 || mode == 0 )histoAltEffiCut[i]->GetYaxis()->SetRangeUser(0.0,0.003);
                    if (mode == 2 || mode == 3 )histoAltEffiCut[i]->GetYaxis()->SetRangeUser(0,0.1);
                    if (mode == 4 || mode == 5 ){
                        if (optionEnergy.Contains("PbPb_2") && meson.Contains("Pi0") )
                            histoAltEffiCut[i]->GetYaxis()->SetRangeUser(0,0.4);
                        else if (optionEnergy.Contains("PbPb_5") && meson.Contains("Pi0") )
                            histoAltEffiCut[i]->GetYaxis()->SetRangeUser(-0.05,0.25);
                        else if (optionEnergy.Contains("pPb") && meson.Contains("Pi0") )
                            histoAltEffiCut[i]->GetYaxis()->SetRangeUser(0.,0.4);
                        else if (optionEnergy.Contains("PbPb_2.76TeV") )
                            histoAltEffiCut[i]->GetYaxis()->SetRangeUser(0,0.6);
                        else
                            histoAltEffiCut[i]->GetYaxis()->SetRangeUser(0,0.6);
                    } else if ( mode == modePCM_Omega || mode == modePCMEMCAL_Omega || mode == modePCMPHOS_Omega || mode == modeEMCAL_Omega || mode == modePHOS_Omega ){
                        if (meson.Contains("Omega") ){
                            histoAltEffiCut[i]->GetYaxis()->SetRangeUser(-0.01,0.15);
                        }
                    }
                    histoAltEffiCut[i]->DrawCopy("e1,p");
                    legendEffiMeson->AddEntry(histoAltEffiCut[i],Form("standard: %s",cutStringsName[i].Data()));
                } else {
                    if(i<20){
                        DrawGammaSetMarker(histoAltEffiCut[i], 20+i, 1.,color[i],color[i]);
                    } else {
                        DrawGammaSetMarker(histoAltEffiCut[i], 20+i, 1.,color[i-20],color[i-20]);
                    }
                    histoAltEffiCut[i]->DrawCopy("same,e1,p");
                    legendEffiMeson->AddEntry(histoAltEffiCut[i],cutStringsName[i].Data());
                }
            }
            legendEffiMeson->Draw();

            // Efficiency plot labeling
            PutProcessLabelAndEnergyOnPlot( 0.94, 0.2, 0.032, collisionSystem, process, detectionProcess, 42, 0.03, optionPeriod, 1, 1.25, 31);

            // Draw ratio of efficiencies in lower panel
            padAltEffiRatios->cd();
            if( optionEnergy.Contains("Pb") ) padAltEffiRatios->SetLogy(0);
            else padAltEffiRatios->SetLogy(0);

            for(Int_t i = 0; i< NumberOfCuts; i++){
                if(i==0){
                    Double_t minYRatio = 0.45;
                    Double_t maxYRatio = 1.55;
                    if (cutVariationName.Contains("Weighting")){
                          minYRatio = 0.75;
                          maxYRatio = 1.25;
                    }
                    if (cutVariationName.Contains("LHC12NL") || cutVariationName.Contains("LHC12-MultWeight")){
                        minYRatio = 0.81;
                        maxYRatio = 1.19;
                    }else if (cutVariationName.Contains("AODvalidation")){
                          minYRatio = 0.9;
                          maxYRatio = 1.09;
                    } else if (cutVariationName.Contains("PHOS_Combined")) {
                        minYRatio = 0.90;
                        maxYRatio = 1.15;
                    } else if (meson.Contains("Omega")) {
                        minYRatio = 0.6;
                        maxYRatio = 1.4;
                    }
                    SetStyleHistoTH1ForGraphs(histoRatioAltEffiCut[i], "#it{p}_{T} (GeV/#it{c})", "#frac{modified}{standard}", 0.05, 0.07, 0.05, 0.07, 0.75, 0.5, 510,505);
                    DrawGammaSetMarker(histoRatioAltEffiCut[i], 20, 1.,color[0],color[0]);
                    histoRatioAltEffiCut[i]->GetYaxis()->SetRangeUser(minYRatio,maxYRatio);
                    histoRatioAltEffiCut[i]->DrawCopy("p,e1");
                } else{
                    if(i<20){
                        DrawGammaSetMarker(histoRatioAltEffiCut[i], 20+i, 1.,color[i],color[i]);
                    } else {
                        DrawGammaSetMarker(histoRatioAltEffiCut[i], 20+i, 1.,color[i-20],color[i-20]);
                    }
                    histoRatioAltEffiCut[i]->DrawCopy("same,e1,p");
                }
                DrawGammaLines(0., maxPt,1., 1.,1);
                DrawGammaLines(0., maxPt, 1.1, 1.1, 1, kGray+1, 7);
                DrawGammaLines(0., maxPt, 0.9, 0.9, 1, kGray+1, 7);
                if (cutVariationName.Contains("Weighting")){
                  DrawGammaLines(0., maxPt, 1.05, 1.05, 1, kGray+1, 9);
                  DrawGammaLines(0., maxPt, 0.95, 0.95, 1, kGray+1, 9);
                }
            }

          canvasAltEffiMeson->Update();
          canvasAltEffiMeson->SaveAs(Form("%s/%s_%s_AlternativeEfficiencies.%s",outputDir.Data(),meson.Data(),prefix2.Data(),suffix.Data()));
          delete canvasAltEffiMeson;
        }
    }

    //**************************************************************************************
    //********************* Plotting Alternative Efficiency Ratio **************************
    //**************************************************************************************
    if (doAlternativeEfficiency){
        if (DebugOutputLevel>=1){cout << "Debug; CompareDifferentDirectories.C, line " << __LINE__ << "; Plotting Alternative Efficiency" << endl;}
        // Define canvas
        if(!plotOnlyUncorrectedOutput){
          TCanvas* canvasRatioTrueEffDivAltEffiMeson = new TCanvas("canvasRatioTrueEffDivAltEffiMeson","",1350,1500);  // gives the page size
          DrawGammaCanvasSettings( canvasRatioTrueEffDivAltEffiMeson,  0.13, 0.02, 0.02, 0.09);
          // Define upper panel
          TPad* padRatioTrueEffDivAltEffi = new TPad("padRatioTrueEffDivAltEffi", "", 0., 0.4, 1., 1.,-1, -1, -2);
          DrawGammaPadSettings( padRatioTrueEffDivAltEffi, 0.12, 0.02, 0.04, 0.);
          padRatioTrueEffDivAltEffi->Draw();
          // Define lower panel
          TPad* padRatioTrueEffDivAltEffiRatios = new TPad("padRatioTrueEffDivAltEffiRatios", "", 0., 0., 1., 0.4,-1, -1, -2);
          DrawGammaPadSettings( padRatioTrueEffDivAltEffiRatios, 0.12, 0.02, 0.0, 0.2);
          padRatioTrueEffDivAltEffiRatios->Draw();

            // draw efficiency in upper panel
            padRatioTrueEffDivAltEffi->cd();
      //       if (mode == 2 || mode == 3 ) padRatioTrueEffDivAltEffi->SetLogy(1);
      //       else padRatioTrueEffDivAltEffi->SetLogy(0);

            TLegend* legendEffiMeson = GetAndSetLegend2(0.15,0.93-0.03*NumberOfCuts,0.3,0.93, 1500*0.75*0.032);
            for(Int_t i = 0; i< NumberOfCuts; i++){
                if(i == 0){
                    DrawGammaSetMarker(histoRatioTrueEffDivAltEffiCut[i], 20, 1., color[0], color[0]);
                    DrawAutoGammaMesonHistos( histoRatioTrueEffDivAltEffiCut[i],
                                              "", "#it{p}_{T} (GeV/#it{c})", Form("#epsilon_{%s}",textMeson.Data()),
                                              kTRUE, 5., 10e-10,kFALSE,
                                              kTRUE, -0.1, 0.00030,
                                              kFALSE, 0., 10.);
                    histoRatioTrueEffDivAltEffiCut[i]->GetYaxis()->SetRangeUser(0.4,2.0);
                    histoRatioTrueEffDivAltEffiCut[i]->DrawCopy("e1,p");
                    legendEffiMeson->AddEntry(histoRatioTrueEffDivAltEffiCut[i],Form("standard: %s",cutStringsName[i].Data()));
                } else {
                    if(i<20){
                        DrawGammaSetMarker(histoRatioTrueEffDivAltEffiCut[i], 20+i, 1.,color[i],color[i]);
                    } else {
                        DrawGammaSetMarker(histoRatioTrueEffDivAltEffiCut[i], 20+i, 1.,color[i-20],color[i-20]);
                    }
                    histoRatioTrueEffDivAltEffiCut[i]->DrawCopy("same,e1,p");
                    legendEffiMeson->AddEntry(histoRatioTrueEffDivAltEffiCut[i],cutStringsName[i].Data());
                }
                DrawGammaLines(0., maxPt,1., 1.,1);
                DrawGammaLines(0., maxPt, 1.1, 1.1, 1, kGray+1, 7);
                DrawGammaLines(0., maxPt, 0.9, 0.9, 1, kGray+1, 7);
            }
            legendEffiMeson->Draw();

            // Efficiency plot labeling
            PutProcessLabelAndEnergyOnPlot( 0.94, 0.2, 0.032, collisionSystem, process, detectionProcess, 42, 0.03, optionPeriod, 1, 1.25, 31);

            // Draw ratio of efficiencies in lower panel
            padRatioTrueEffDivAltEffiRatios->cd();
            if( optionEnergy.Contains("Pb") ) padRatioTrueEffDivAltEffiRatios->SetLogy(0);
            else padRatioTrueEffDivAltEffiRatios->SetLogy(0);

            for(Int_t i = 0; i< NumberOfCuts; i++){
                if(i==0){
                    Double_t minYRatio = 0.4;
                    Double_t maxYRatio = 2.0;
                    SetStyleHistoTH1ForGraphs(histoRatioOfRatioTrueEffDivAltEffiCut[i], "#it{p}_{T} (GeV/#it{c})", "#frac{modified}{standard}", 0.05, 0.07, 0.05, 0.07, 0.75, 0.5, 510,505);
                    DrawGammaSetMarker(histoRatioOfRatioTrueEffDivAltEffiCut[i], 20, 1.,color[0],color[0]);
                    histoRatioOfRatioTrueEffDivAltEffiCut[i]->GetYaxis()->SetRangeUser(minYRatio,maxYRatio);
                    histoRatioOfRatioTrueEffDivAltEffiCut[i]->DrawCopy("p,e1");
                } else{
                    if(i<20){
                        DrawGammaSetMarker(histoRatioOfRatioTrueEffDivAltEffiCut[i], 20+i, 1.,color[i],color[i]);
                    } else {
                        DrawGammaSetMarker(histoRatioOfRatioTrueEffDivAltEffiCut[i], 20+i, 1.,color[i-20],color[i-20]);
                    }
                    histoRatioOfRatioTrueEffDivAltEffiCut[i]->DrawCopy("same,e1,p");
                }
                DrawGammaLines(0., maxPt,1., 1.,1);
                DrawGammaLines(0., maxPt, 1.1, 1.1, 1, kGray+1, 7);
                DrawGammaLines(0., maxPt, 0.9, 0.9, 1, kGray+1, 7);
            }

          canvasRatioTrueEffDivAltEffiMeson->Update();
          canvasRatioTrueEffDivAltEffiMeson->SaveAs(Form("%s/%s_%s_EfficiencyRatios.%s",outputDir.Data(),meson.Data(),prefix2.Data(),suffix.Data()));
          delete canvasRatioTrueEffDivAltEffiMeson;
        }
    }

    //**************************************************************************************
    //********************* Plotting Acceptance *********************************************
    //**************************************************************************************
    if (DebugOutputLevel>=1){cout << "Debug; CompareDifferentDirectories.C, line " << __LINE__ << "; Plotting Acceptance" << endl;}
    // Define canvas
    if(!plotOnlyUncorrectedOutput){
      TCanvas* canvasAcceptanceMeson = new TCanvas("canvasAcceptanceMeson","",1350,1500);  // gives the page size
      DrawGammaCanvasSettings( canvasAcceptanceMeson,  0.13, 0.02, 0.02, 0.09);
      // Define upper panel
      TPad* padAcceptance = new TPad("padAcceptance", "", 0., 0.25, 1., 1.,-1, -1, -2);
      DrawGammaPadSettings( padAcceptance, 0.12, 0.02, 0.04, 0.);
      padAcceptance->Draw();
      // Define lower panel
      TPad* padAcceptanceRatios = new TPad("padAcceptanceRatios", "", 0., 0., 1., 0.25,-1, -1, -2);
      DrawGammaPadSettings( padAcceptanceRatios, 0.12, 0.02, 0.0, 0.2);
      padAcceptanceRatios->Draw();

        // draw efficiency in upper panel
        padAcceptance->cd();
  //       if (mode == 2 || mode == 3 ) padAcceptance->SetLogy(1);
  //       else padAcceptance->SetLogy(0);

        Double_t minAccept = 1;
        for (Int_t j = 1; j < histoAcceptanceCut[0]->GetNbinsX()+1; j++){
            if (histoAcceptanceCut[0]->GetBinContent(j) > 0){
              if (histoAcceptanceCut[0]->GetBinContent(j) < minAccept) minAccept = histoAcceptanceCut[0]->GetBinContent(j);
            }
        }

        TLegend* legendAcceptance = GetAndSetLegend2(0.55, 0.93-(1.1*NumberOfCuts*0.038), 0.95, 0.93,38);
        for(Int_t i = 0; i< NumberOfCuts; i++){
            if(i == 0){
                Double_t minY = 0.0001;
                Double_t maxY = 1;
                if (isEDC && mode == 4) maxY = 0.6;
                if (isEDC && mode == 2) maxY = 0.9;
                DrawGammaSetMarker(histoAcceptanceCut[i], 20, 1., color[0], color[0]);
                DrawAutoGammaMesonHistos( histoAcceptanceCut[i],
                                        "", "#it{p}_{T} (GeV/#it{c})", Form("A_{%s}",textMeson.Data()),
                                        kTRUE, 1.2, minAccept/1.5, kFALSE,
                                        kFALSE, -0.1, 0.00030,
                                        kFALSE, 0., 10.);
  //               if (mode == 9 || mode == 0 )histoAcceptanceCut[i]->GetYaxis()->SetRangeUser(0.0,0.003);
  //               if (mode == 2 || mode == 3 )histoAcceptanceCut[i]->GetYaxis()->SetRangeUser(1.e-7,2);
                histoAcceptanceCut[i]->GetYaxis()->SetRangeUser(minY, maxY);
                histoAcceptanceCut[i]->DrawCopy("e1,p");
                legendAcceptance->AddEntry(histoAcceptanceCut[i],Form("standard: %s",cutStringsName[i].Data()));
            } else {
                if(i<20){
                    DrawGammaSetMarker(histoAcceptanceCut[i], 20+i, 1.,color[i],color[i]);
                } else {
                    DrawGammaSetMarker(histoAcceptanceCut[i], 20+i, 1.,color[i-20],color[i-20]);
                }
                histoAcceptanceCut[i]->DrawCopy("same,e1,p");
                legendAcceptance->AddEntry(histoAcceptanceCut[i],cutStringsName[i].Data());
            }
        }
        legendAcceptance->Draw();

        // Acceptance plot labeling
        PutProcessLabelAndEnergyOnPlot( 0.94, 0.20, 0.032, collisionSystem, process, detectionProcess, 42, 0.03, optionPeriod, 1, 1.25, 31);

        // Draw ratio of efficiencies in lower panel
        padAcceptanceRatios->cd();
        if( optionEnergy.Contains("Pb") ) padAcceptanceRatios->SetLogy(0);
        else padAcceptanceRatios->SetLogy(0);

        for(Int_t i = 0; i< NumberOfCuts; i++){
            if(i==0){
                Double_t minYRatio = 0.45;
                Double_t maxYRatio = 1.05;
                SetStyleHistoTH1ForGraphs(histoRatioAcceptanceCut[i], "#it{p}_{T} (GeV/#it{c})", "#frac{modified}{standard}", 0.08, 0.11, 0.07, 0.1, 0.75, 0.5, 510,505);
                DrawGammaSetMarker(histoRatioAcceptanceCut[i], 20, 1.,color[0],color[0]);
                histoRatioAcceptanceCut[i]->GetYaxis()->SetRangeUser(minYRatio,maxYRatio);
                histoRatioAcceptanceCut[i]->DrawCopy("p,e1");
            } else{
                if(i<20){
                    DrawGammaSetMarker(histoRatioAcceptanceCut[i], 20+i, 1.,color[i],color[i]);
                } else {
                    DrawGammaSetMarker(histoRatioAcceptanceCut[i], 20+i, 1.,color[i-20],color[i-20]);
                }
                histoRatioAcceptanceCut[i]->DrawCopy("same,e1,p");
            }
            DrawGammaLines(0., maxPt,1., 1.,1);
        }

      canvasAcceptanceMeson->Update();
      canvasAcceptanceMeson->SaveAs(Form("%s/%s_%s_Acceptance.%s",outputDir.Data(),meson.Data(),prefix2.Data(),suffix.Data()));
      delete canvasAcceptanceMeson;
    }

    //**************************************************************************************
    //********************* Plotting Mass **************************************************
    //**************************************************************************************
    if (DebugOutputLevel>=1){cout << "Debug; CompareDifferentDirectories.C, line " << __LINE__ << "; Plotting Mass" << endl;}

    TCanvas* canvasMassMeson = new TCanvas("canvasMassMeson","",1350,1500);  // gives the page size
    DrawGammaCanvasSettings( canvasMassMeson,  0.13, 0.02, 0.02, 0.09);
    // Define upper panel
    TPad* padMass = new TPad("padMass", "", 0., 0.25, 1., 1.,-1, -1, -2);
    DrawGammaPadSettings( padMass, 0.12, 0.02, 0.02, 0.);
    padMass->Draw();
    // Define lower panel
    TPad* padMassRatios = new TPad("padMassRatios", "", 0., 0., 1., 0.25,-1, -1, -2);
    DrawGammaPadSettings( padMassRatios, 0.12, 0.02, 0.0, 0.2);
    padMassRatios->Draw();

    // draw efficiency in upper panel
    padMass->cd();
    if ( (mode == 2 || mode == 3) && optionEnergy.CompareTo("8TeV")!=0 ) padMass->SetLogy(1);
    else padMass->SetLogy(0);

    TLegend* legendMass = GetAndSetLegend2(0.15,0.93-0.03*NumberOfCuts,0.3,0.93, 1500*0.75*0.032);
    for(Int_t i = 0; i< NumberOfCuts; i++){
        if(i == 0){
            DrawGammaSetMarker(histoMassCut[i], 20, 1., color[0], color[0]);
            Double_t massMin = 0.1275;
            if(mode == 2 || mode == 3 || mode == 4) massMin = 0.12;
            DrawAutoGammaMesonHistos( histoMassCut[i],
                                    "", "#it{p}_{T} (GeV/#it{c})", Form("#it{m}_{%s} (GeV/c^{2})",textMeson.Data()),
                                    kFALSE, 5., 10e-10,kFALSE,
                                    kTRUE, massMin, 0.1425,
                                    kFALSE, 0., 10.);
            if (optionEnergy.CompareTo("8TeV") == 0) histoMassCut[i]->GetYaxis()->SetRangeUser(massMin,0.15);
            if (optionEnergy.CompareTo("8TeV") == 0 && cutVariationName.Contains("Calo") && cutVariationName.Contains("EGA")) histoMassCut[i]->GetYaxis()->SetRangeUser(0.12,0.20);
            if ( !meson.Contains("Pi0") )histoMassCut[i]->GetYaxis()->SetRangeUser(0.4,0.6);
            if ( meson.Contains("Omega") )histoMassCut[i]->GetYaxis()->SetRangeUser(0.7,0.85);
            histoMassCut[i]->GetYaxis()->SetTitleOffset(1.4);
            histoMassCut[i]->DrawCopy("e1,p");
            legendMass->AddEntry(histoMassCut[i],Form("standard: %s",cutStringsName[i].Data()));
        } else {
            if(i<20){
                DrawGammaSetMarker(histoMassCut[i], 20+i, 1.,color[i],color[i]);
            } else {
                DrawGammaSetMarker(histoMassCut[i], 20+i, 1.,color[i-20],color[i-20]);
            }
            histoMassCut[i]->DrawCopy("same,e1,p");
            legendMass->AddEntry(histoMassCut[i],cutStringsName[i].Data());
        }
    }
    legendMass->Draw();
    PutProcessLabelAndEnergyOnPlot( 0.94, 0.20, 0.032, collisionSystem, process, detectionProcess, 42, 0.03, optionPeriod, 1, 1.25, 31);

    // Draw ratio of efficiencies in lower panel
    padMassRatios->cd();
    if( optionEnergy.Contains("Pb") ) padMassRatios->SetLogy(0);
    else padMassRatios->SetLogy(0);

    for(Int_t i = 0; i< NumberOfCuts; i++){
        if(i==0){
            Double_t minYRatio = 0.95;
            Double_t maxYRatio = 1.05;
            SetStyleHistoTH1ForGraphs(histoRatioMassCut[i], "#it{p}_{T} (GeV/#it{c})", "#frac{modified}{standard}", 0.08, 0.11, 0.08, 0.1, 0.75, 0.5, 510,505);
            DrawGammaSetMarker(histoRatioMassCut[i], 20, 1.,color[0],color[0]);
            histoRatioMassCut[i]->GetYaxis()->SetRangeUser(minYRatio,maxYRatio);
            histoRatioMassCut[i]->DrawCopy("p,e1");
        } else{
            if(i<20){
                DrawGammaSetMarker(histoRatioMassCut[i], 20+i, 1.,color[i],color[i]);
            } else {
                DrawGammaSetMarker(histoRatioMassCut[i], 20+i, 1.,color[i-20],color[i-20]);
            }
            histoRatioMassCut[i]->DrawCopy("same,e1,p");
        }
        DrawGammaLines(0., maxPt,1., 1.,1);
    }

    canvasMassMeson->Update();
    canvasMassMeson->SaveAs(Form("%s/%s_%s_Mass.%s",outputDir.Data(),meson.Data(),prefix2.Data(),suffix.Data()));
    delete canvasMassMeson;

    //**************************************************************************************
    //********************* Plotting Width **************************************************
    //**************************************************************************************
    if (DebugOutputLevel>=1){cout << "Debug; CompareDifferentDirectories.C, line " << __LINE__ << "; Plotting Width" << endl;}

    TCanvas* canvasWidthMeson = new TCanvas("canvasWidthMeson","",1350,1500);  // gives the page size
    DrawGammaCanvasSettings( canvasWidthMeson,  0.13, 0.02, 0.02, 0.09);
    // Define upper panel
    TPad* padWidth = new TPad("padWidth", "", 0., 0.25, 1., 1.,-1, -1, -2);
    DrawGammaPadSettings( padWidth, 0.12, 0.02, 0.02, 0.);
    padWidth->Draw();
    // Define lower panel
    TPad* padWidthRatios = new TPad("padWidthRatios", "", 0., 0., 1., 0.25,-1, -1, -2);
    DrawGammaPadSettings( padWidthRatios, 0.12, 0.02, 0.0, 0.2);
    padWidthRatios->Draw();

    // draw efficiency in upper panel
    padWidth->cd();
    if ( (mode == 2 || mode == 3) && optionEnergy.CompareTo("8TeV")!=0 ) padWidth->SetLogy(1);
    else padWidth->SetLogy(0);

    TLegend* legendWidth = GetAndSetLegend2(0.15,0.93-0.03*NumberOfCuts,0.3,0.93, 1500*0.75*0.032);
    for(Int_t i = 0; i< NumberOfCuts; i++){
        if(i == 0){
            DrawGammaSetMarker(histoWidthCut[i], 20, 1., color[0], color[0]);
            DrawAutoGammaMesonHistos( histoWidthCut[i],
                                    "", "#it{p}_{T} (GeV/#it{c})", Form("#it{#sigma}_{%s} (GeV/c^{2})",textMeson.Data()),
                                    kFALSE, 5., 10e-10,kFALSE,
                                    kTRUE, PlotRangeYWidthMin, PlotRangeYWidthMax,
                                    kFALSE, 0., 10.);
            if (optionEnergy.CompareTo("8TeV") == 0 && cutVariationName.Contains("Calo") && cutVariationName.Contains("EGA")) histoWidthCut[i]->GetYaxis()->SetRangeUser(0.,0.10);
            histoWidthCut[i]->GetYaxis()->SetTitleOffset(1.4);
            histoWidthCut[i]->DrawCopy("e1,p");
            legendWidth->AddEntry(histoWidthCut[i],Form("standard: %s",cutStringsName[i].Data()));
        } else {
            if(i<20){
                DrawGammaSetMarker(histoWidthCut[i], 20+i, 1.,color[i],color[i]);
            } else {
                DrawGammaSetMarker(histoWidthCut[i], 20+i, 1.,color[i-20],color[i-20]);
            }
            histoWidthCut[i]->DrawCopy("same,e1,p");
            legendWidth->AddEntry(histoWidthCut[i],cutStringsName[i].Data());
        }
    }
    legendWidth->Draw();
    PutProcessLabelAndEnergyOnPlot( 0.94, 0.95, 0.032, collisionSystem, process, detectionProcess, 42, 0.03, optionPeriod, 1, 1.25, 31);

    // Draw ratio of efficiencies in lower panel
    padWidthRatios->cd();
    if( optionEnergy.Contains("Pb") ) padWidthRatios->SetLogy(0);
    else padWidthRatios->SetLogy(0);

    for(Int_t i = 0; i< NumberOfCuts; i++){
        if(i==0){
            Double_t minYRatio = 0.6;
            Double_t maxYRatio = 1.2;
            SetStyleHistoTH1ForGraphs(histoRatioWidthCut[i], "#it{p}_{T} (GeV/#it{c})", "#frac{modified}{standard}", 0.08, 0.11, 0.08, 0.1, 0.75, 0.5, 510,505);
            DrawGammaSetMarker(histoRatioWidthCut[i], 20, 1.,color[0],color[0]);
            histoRatioWidthCut[i]->GetYaxis()->SetRangeUser(minYRatio,maxYRatio);
            histoRatioWidthCut[i]->DrawCopy("p,e1");
        } else{
            if(i<20){
                DrawGammaSetMarker(histoRatioWidthCut[i], 20+i, 1.,color[i],color[i]);
            } else {
                DrawGammaSetMarker(histoRatioWidthCut[i], 20+i, 1.,color[i-20],color[i-20]);
            }
            histoRatioWidthCut[i]->DrawCopy("same,e1,p");
        }
        DrawGammaLines(0., maxPt,1., 1.,1);
    }

    canvasWidthMeson->Update();
    canvasWidthMeson->SaveAs(Form("%s/%s_%s_Width.%s",outputDir.Data(),meson.Data(),prefix2.Data(),suffix.Data()));
    delete canvasWidthMeson;

    //**************************************************************************************
    //************************ Plotting SB  ************************************************
    //**************************************************************************************
    if (DebugOutputLevel>=1){cout << "Debug; CompareDifferentDirectories.C, line " << __LINE__ << "; Plotting SB" << endl;}

        TCanvas* canvasSBMeson = new TCanvas("canvasSBMeson","",1350,1500);
        DrawGammaCanvasSettings( canvasSBMeson,  0.13, 0.02, 0.02, 0.09);
        // Upper pad definition
        TPad* padSB = new TPad("padSB", "", 0., 0.33, 1., 1.,-1, -1, -2);
        DrawGammaPadSettings( padSB, 0.12, 0.02, 0.02, 0.);
        padSB->SetLogy();
        padSB->Draw();
        // lower pad definition
        TPad* padSBRatios = new TPad("padSBRatios", "", 0., 0., 1., 0.33,-1, -1, -2);
        DrawGammaPadSettings( padSBRatios, 0.12, 0.02, 0.0, 0.2);
        padSBRatios->Draw();

        padSB->cd();
        padSB->SetTickx();
        padSB->SetTicky();

        // Plot SB in uppper panel
        padSB->cd();

        TLegend* legendSB = GetAndSetLegend2(0.15,0.93-0.03*NumberOfCuts,0.3,0.93, 1500*0.75*0.032);
        for(Int_t i = 0; i< NumberOfCuts; i++){
            if(i == 0){
                DrawGammaSetMarker(histoSBCut[i], 20, 1., color[0], color[0]);
                DrawAutoGammaMesonHistos( histoSBCut[i],
                                        "", "#it{p}_{T} (GeV/#it{c})", Form("%s S/B",textMeson.Data()),
                                        kTRUE, 10., 1e-4, kTRUE,
                                        kFALSE, 0.0, 0.030,
                                        kFALSE, 0., 10.);
                legendSB->AddEntry(histoSBCut[i],Form("standard: %s",cutStringsName[i].Data()));
            }
            else {
                if(i<20){
                    DrawGammaSetMarker(histoSBCut[i], 20+i, 1.,color[i],color[i]);
                } else {
                    DrawGammaSetMarker(histoSBCut[i], 20+i, 1.,color[i-20],color[i-20]);
                }
                histoSBCut[i]->DrawCopy("same,e1,p");
                legendSB->AddEntry(histoSBCut[i],cutStringsName[i].Data());
            }

        }
        legendSB->Draw();
        PutProcessLabelAndEnergyOnPlot( 0.94, 0.95, 0.032, collisionSystem, process, detectionProcess, 42, 0.03, optionPeriod, 1, 1.25, 31);

        padSBRatios->cd();
        for(Int_t i = 0; i< NumberOfCuts; i++){
            if(i==0){
                // Set ratio min and max
                Double_t minYRatio = 0.7;
                Double_t maxYRatio = 1.3; //qui
                if (cutVariationName.Contains("LHC12DistBC")){
                    minYRatio = 0.81;
                    maxYRatio = 1.29;
                }
                SetStyleHistoTH1ForGraphs(histoRatioSBCut[i], "#it{p}_{T} (GeV/#it{c})", "#frac{modified}{standard}", 0.08, 0.11, 0.07, 0.1, 0.75, 0.5, 510,505);
                DrawGammaSetMarker(histoRatioSBCut[i], 20, 1.,color[0],color[0]);
                histoRatioSBCut[i]->GetYaxis()->SetRangeUser(minYRatio,maxYRatio);
                histoRatioSBCut[i]->DrawCopy("p,e1");
            } else{
                if(i<20){
                    DrawGammaSetMarker(histoRatioSBCut[i], 20+i, 1.,color[i],color[i]);
                } else {
                    DrawGammaSetMarker(histoRatioSBCut[i], 20+i, 1.,color[i-20],color[i-20]);
                }
                histoRatioSBCut[i]->DrawCopy("same,e1,p");
            }
            DrawGammaLines(0., maxPt,1., 1.,1);
        }

        canvasSBMeson->Update();
        canvasSBMeson->SaveAs(Form("%s/%s_%s_SB.%s",outputDir.Data(),meson.Data(),prefix2.Data(),suffix.Data()));
        delete canvasSBMeson;


        //**************************************************************************************
        //************************ Plotting ClusterE *******************************************
        //**************************************************************************************

        if(isClusterE){
          TCanvas* canvasClusEMeson = new TCanvas("canvasClusEMeson","",1350,1500);
          DrawGammaCanvasSettings( canvasClusEMeson,  0.13, 0.02, 0.02, 0.09);
          // Upper pad definition
          TPad* padClusE = new TPad("padClusE", "", 0., 0.33, 1., 1.,-1, -1, -2);
          DrawGammaPadSettings( padClusE, 0.12, 0.02, 0.02, 0.);
          padClusE->SetLogy();
          padClusE->Draw();
          // lower pad definition
          TPad* padClusERatios = new TPad("padClusERatios", "", 0., 0., 1., 0.33,-1, -1, -2);
          DrawGammaPadSettings( padClusERatios, 0.12, 0.02, 0.0, 0.2);
          padClusERatios->Draw();

          padClusE->cd();
          padClusE->SetTickx();
          padClusE->SetTicky();

          // Plot ClusE in uppper panel
          padClusE->cd();

          TLegend* legendClusE = GetAndSetLegend2(0.35,0.93-0.03*NumberOfCuts,0.5,0.93, 1500*0.75*0.032);
          for(Int_t i = 0; i< NumberOfCuts; i++){
            if(i == 0){
              DrawGammaSetMarker(histoClusterE[i], 20, 1., color[0], color[0]);
              DrawAutoGammaMesonHistos( histoClusterE[i],
                                        "", "cluster E (GeV)", "N_{cluster}/#it{N}_{ev}",
                                        kTRUE, 10., 1e-10, kTRUE,
                                        kFALSE, 0.0, 0.030,
                                        kFALSE, 0., 10.);
              legendClusE->AddEntry(histoClusterE[i],Form("standard: %s",cutStringsName[i].Data()));
            }
            else {
              if(i<20){
                DrawGammaSetMarker(histoClusterE[i], 20+i, 1.,color[i],color[i]);
              } else {
                DrawGammaSetMarker(histoClusterE[i], 20+i, 1.,color[i-20],color[i-20]);
              }
              histoClusterE[i]->DrawCopy("same,e1,p");
              legendClusE->AddEntry(histoClusterE[i],cutStringsName[i].Data());
            }

          }
          legendClusE->Draw();
          PutProcessLabelAndEnergyOnPlot( 0.94, 0.95, 0.032, collisionSystem, "#gamma candidates", detectionProcess, 42, 0.03, optionPeriod, 1, 1.25, 31);

          padClusERatios->cd();
          for(Int_t i = 0; i< NumberOfCuts; i++){
            if(i==0){
              // Set ratio min and max
              Double_t minYRatio = 0.85;
              Double_t maxYRatio = 1.059; //qui
              SetStyleHistoTH1ForGraphs(histoRatioClusterE[i], "cluster E (GeV)", "#frac{modified}{standard}", 0.08, 0.11, 0.07, 0.1, 0.75, 0.5, 510,505);
              DrawGammaSetMarker(histoRatioClusterE[i], 20, 1.,color[0],color[0]);
              histoRatioClusterE[i]->GetYaxis()->SetRangeUser(minYRatio,maxYRatio);
              histoRatioClusterE[i]->DrawCopy("p,e1");
            } else{
              if(i<20){
                DrawGammaSetMarker(histoRatioClusterE[i], 20+i, 1.,color[i],color[i]);
              } else {
                DrawGammaSetMarker(histoRatioClusterE[i], 20+i, 1.,color[i-20],color[i-20]);
              }
              histoRatioClusterE[i]->DrawCopy("same,e1,p");
            }
            DrawGammaLines(0., maxPt,1., 1.,1);
          }

          canvasClusEMeson->Update();
          canvasClusEMeson->SaveAs(Form("%s/%s_%s_ClusE.%s",outputDir.Data(),meson.Data(),prefix2.Data(),suffix.Data()));
          delete canvasClusEMeson;
        }


  //*************************************************************************************************
  //******************** Output of the systematic Error due to Signal extraction for Pi0 ************
  //*************************************************************************************************
  if (DebugOutputLevel>=1){cout << "Debug; CompareDifferentDirectories.C, line " << __LINE__ << "; Output of the systematic Error due to Signal extraction for Pi0" << endl;}
  if(!plotOnlyUncorrectedOutput){
    // Determine number of bins
    Int_t NBinsPt = histoCorrectedYieldCut[0]->GetNbinsX();
    const Int_t NBinstPtConst = NBinsPt+1;

    // Create array of bin boundaries
    Double_t  BinsXCenter[NBinstPtConst];
    Double_t  BinsXWidth[NBinstPtConst];
    BinsXCenter[0] = 0;
    BinsXWidth[0]=0.;
    for (Int_t i = 1; i < NBinsPt +1; i++){
        BinsXCenter[i] = histoCorrectedYieldCut[0]->GetBinCenter(i);
        BinsXWidth[i]= histoCorrectedYieldCut[0]->GetBinWidth(i)/2.;
    }

    // Create array of Sys Err Objects and fill them
    SysErrorConversion SysErrCut[ConstNumberOfCuts][NBinstPtConst];
    SysErrorConversion SysErrCutRaw[ConstNumberOfCuts][NBinstPtConst];
    for (Int_t j = 0; j < NumberOfCuts; j++){
        for (Int_t i = 1; i < NBinsPt +1; i++){
            SysErrCut[j][i].value = histoCorrectedYieldCut[j]->GetBinContent(i);
            SysErrCut[j][i].error = histoCorrectedYieldCut[j]->GetBinError(i);
            SysErrCutRaw[j][i].value = histoRawYieldCut[j]->GetBinContent(i);
            SysErrCutRaw[j][i].error = histoRawYieldCut[j]->GetBinError(i);
        }
    }

    // Create Difference arrays
    Double_t DifferenceCut[ConstNumberOfCuts][NBinstPtConst];
    Double_t DifferenceErrorCut[ConstNumberOfCuts][NBinstPtConst];
    Double_t RelDifferenceCut[ConstNumberOfCuts][NBinstPtConst];
    Double_t RelDifferenceErrorCut[ConstNumberOfCuts][NBinstPtConst];
    Double_t RelDifferenceRawCut[ConstNumberOfCuts][NBinstPtConst];

    // Create largest difference array
    Double_t LargestDiffNeg[NBinstPtConst];
    Double_t LargestDiffPos[NBinstPtConst];
    Double_t LargestDiffErrorNeg[NBinstPtConst];
    Double_t LargestDiffErrorPos[NBinstPtConst];
    Double_t LargestDiffRelNeg[NBinstPtConst];
    Double_t LargestDiffRelPos[NBinstPtConst];
    Double_t LargestDiffRelErrorNeg[NBinstPtConst];
    Double_t LargestDiffRelErrorPos[NBinstPtConst];

    // Initialize all differences with 0
    for (Int_t j = 1; j < NumberOfCuts; j++){
        for ( Int_t i = 1; i < NBinstPtConst; i++) {
            DifferenceCut[j][i]=0.;
            DifferenceErrorCut[j][i]=0.;
            LargestDiffNeg[i]=0.;
            LargestDiffPos[i]=0.;
            LargestDiffErrorNeg[i]=0.;
            LargestDiffErrorPos[i]=0.;
            RelDifferenceCut[j][i]=0.;
            RelDifferenceRawCut[j][i]=0.;
            RelDifferenceErrorCut[j][i]=0.;
        }
    }

    // Calculate largest difference among cut variation
    for(Int_t j = 1; j < NumberOfCuts; j++){
        for (Int_t i = 1; i < NBinsPt +1; i++){
            // Calculate difference (rel/abs) and error for corrected yield
            DifferenceCut[j][i] = SysErrCut[j][i].value - SysErrCut[0][i].value;
            DifferenceErrorCut[j][i] = TMath::Sqrt(TMath::Abs(TMath::Power(SysErrCut[j][i].error,2)-TMath::Power(SysErrCut[0][i].error,2)));
            if(SysErrCut[0][i].value != 0){
                RelDifferenceCut[j][i] = DifferenceCut[j][i]/SysErrCut[0][i].value*100. ;
                RelDifferenceErrorCut[j][i] = DifferenceErrorCut[j][i]/SysErrCut[0][i].value*100. ;
            } else {
                RelDifferenceCut[j][i] = -10000.;
                RelDifferenceErrorCut[j][i] = 100. ;
            }
            // Calculate relativ difference for raw yield
            if(SysErrCutRaw[0][i].value != 0){
                RelDifferenceRawCut[j][i] = (SysErrCutRaw[j][i].value - SysErrCutRaw[0][i].value)/SysErrCutRaw[0][i].value*100. ;
            } else {
                RelDifferenceRawCut[j][i] = -10000.;
            }
            // Calculate largest differences in positiv and negative direction
            if(DifferenceCut[j][i] < 0){ // largest negativ deviation
                // Take deviation if larger than previous largest deviation
                // and relative raw yield loss less than 75%
                if (TMath::Abs(LargestDiffNeg[i]) < TMath::Abs(DifferenceCut[j][i]) && RelDifferenceRawCut[j][i] > -75.){
                    LargestDiffNeg[i] = DifferenceCut[j][i];
                    LargestDiffErrorNeg[i] = DifferenceErrorCut[j][i];
                }
            } else { // largest positive deviation
                // Take deviation if larger than previous largest deviation
                // and relative raw yield loss less than 75%
                if (TMath::Abs(LargestDiffPos[i]) < TMath::Abs(DifferenceCut[j][i]) && RelDifferenceRawCut[j][i] > -75.){
                    LargestDiffPos[i] = DifferenceCut[j][i];
                    LargestDiffErrorPos[i] = DifferenceErrorCut[j][i];
                }
            }
        }
    }

    // Write systematic error input to log file
    TString SysErrDatname = Form("%s/%s_%s_SystematicErrorCutStudies.dat",outputDir.Data(),meson.Data(),prefix2.Data());
    fstream SysErrDat;
    SysErrDat.open(SysErrDatname.Data(), ios::out);
    SysErrDat << "Calculation of the systematic error due to the yield cuts" << endl;

    for (Int_t l=0; l< NumberOfCuts; l++){
        if (l == 0) {
            SysErrDat << endl <<"Bin" << "\t" << cutNumber[l] << "\t" <<endl;
            for(Int_t i = 1; i < (NBinsPt +1); i++){
                SysErrDat << BinsXCenter[i] << "\t" << SysErrCut[l][i].value << "\t" << SysErrCut[l][i].error << endl;
            }
        } else{
            SysErrDat << endl <<"Bin" << "\t" << cutNumber[l] << "\t" << "Error " << "\t Dif to Cut1" << endl;
            for(Int_t i = 1; i < (NBinsPt +1); i++){
                if (RelDifferenceRawCut[l][i] > -75.){
                    SysErrDat << BinsXCenter[i] << "\t" << SysErrCut[l][i].value << "\t" << SysErrCut[l][i].error << "\t" <<  DifferenceCut[l][i] << "\t"<< DifferenceErrorCut[l][i] << "\t"<< RelDifferenceCut[l][i] <<  "\t" << RelDifferenceErrorCut[l][i] <<"\t" << RelDifferenceRawCut[l][i]<< endl;
                } else {
                    SysErrDat << BinsXCenter[i] << "\t" << SysErrCut[l][i].value << "\t" << SysErrCut[l][i].error << "\t" <<  DifferenceCut[l][i] << "\t"<< DifferenceErrorCut[l][i] << "\t"<< RelDifferenceCut[l][i] <<  "\t" << RelDifferenceErrorCut[l][i] <<"\t" << RelDifferenceRawCut[l][i]  <<"\t not considered in largest dev" <<endl;
                }
            }
        }
    }
    SysErrDat << endl;
    SysErrDat << endl;
    SysErrDat << "Bin" << "\t" << "Largest Dev Neg" << "\t" << "Largest Dev Pos"  << endl;
    for(Int_t i = 1; i < (NBinsPt +1); i++){
        SysErrDat << BinsXCenter[i]  << "\t" << LargestDiffNeg[i] << "\t" <<LargestDiffErrorNeg[i]<< "\t" << LargestDiffPos[i] << "\t" << LargestDiffErrorPos[i]<<endl;
    }
    SysErrDat << endl << endl <<"Bin" << "\t" << "Largest Dev Neg rel" << "\t" << "Largest Dev Pos rel"  << endl;
    // Calculate largest relative deviations
    for(Int_t i = 0; i < (NBinsPt +1); i++){
        if ( SysErrCut[0][i].value != 0.){
            LargestDiffRelNeg[i] = - LargestDiffNeg[i]/SysErrCut[0][i].value*100.;
            LargestDiffRelPos[i] = LargestDiffPos[i]/SysErrCut[0][i].value*100.;
            LargestDiffRelErrorNeg[i] = - LargestDiffErrorNeg[i]/SysErrCut[0][i].value*100.;
            LargestDiffRelErrorPos[i] = LargestDiffErrorPos[i]/SysErrCut[0][i].value*100.;
            if (i > 0){
              SysErrDat << BinsXCenter[i] << "\t" << LargestDiffNeg[i]/SysErrCut[0][i].value*100. << "\t" << LargestDiffErrorNeg[i]/SysErrCut[0][i].value*100. << "\t" << LargestDiffPos[i]/SysErrCut[0][i].value*100. << "\t" << LargestDiffErrorPos[i]/SysErrCut[0][i].value*100.<<endl;
            } else {
              LargestDiffRelNeg[i] = 0.;
              LargestDiffRelPos[i] = 0.;
              LargestDiffRelErrorNeg[i] = 0.;
              LargestDiffRelErrorPos[i] = 0.;
            }
        } else {
            LargestDiffRelNeg[i] = 0.;
            LargestDiffRelPos[i] = 0.;
            LargestDiffRelErrorNeg[i] = 0.;
            LargestDiffRelErrorPos[i] = 0.;
        }
    }
    SysErrDat.close();

    // Create sys-err graphs
    TGraphAsymmErrors* SystErrGraphNeg = new TGraphAsymmErrors(NBinsPt+1, BinsXCenter, LargestDiffRelNeg, BinsXWidth, BinsXWidth, LargestDiffRelErrorNeg, LargestDiffRelErrorNeg);
    SystErrGraphNeg->SetName(Form("%s_SystErrorRelNeg_%s",meson.Data(),cutVariationName.Data()));
    TGraphAsymmErrors* SystErrGraphPos = new TGraphAsymmErrors(NBinsPt+1, BinsXCenter, LargestDiffRelPos, BinsXWidth, BinsXWidth, LargestDiffRelErrorPos, LargestDiffRelErrorPos);
    SystErrGraphPos->SetName(Form("%s_SystErrorRelPos_%s",meson.Data(),cutVariationName.Data()));

    // Write sys-err graph to root output file
    TString Outputname = Form("%s/%s_%s_SystematicErrorCuts.root",outputDirRootFile.Data(),meson.Data(),prefix2.Data());
    TFile* SystematicErrorFile = new TFile(Outputname.Data(),"UPDATE");
        SystErrGraphPos->Write(Form("%s_SystErrorRelPos_%s",meson.Data(),cutVariationName.Data()),TObject::kOverwrite);
        SystErrGraphNeg->Write(Form("%s_SystErrorRelNeg_%s",meson.Data(),cutVariationName.Data()),TObject::kOverwrite);
    SystematicErrorFile->Write();
    SystematicErrorFile->Close();
  }
}
