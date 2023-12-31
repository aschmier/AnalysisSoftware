/*******************************************************************************
 ******  provided by Gamma Conversion Group, PWGGA,                        *****
 ******     Daniel Muehlheim, d.muehlheim@cern.ch                          *****
 *******************************************************************************/

#include "TH1F.h"
#include "TH1D.h"
#include "TH2F.h"
#include "TProfile2D.h"
#include "TF1.h"
#include <TStyle.h>
#include <TImage.h>
#include <TASImage.h>
#include <Riostream.h>
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
#include "TPostScript.h"
#include "TGraphErrors.h"
#include "TFitResultPtr.h"
#include "TFitResult.h"
#include "TMatrixDSym.h"
#include "TArrow.h"
#include "TGraphAsymmErrors.h"
#include "TGaxis.h"
#include "TMarker.h"
#include "TASImage.h"
#include "TRandom3.h"
#include <queue>
#include "QA.h"
#include "EventQA.C"
#include "JetQA.C"
#include "PhotonQA.C"
#include "ClusterQA.C"
#include "PrimaryTrackQA.C"
#include "ClusterQA_DeadCellCompareV2.C"
#include "ClusterQA_HotCellCompareV2.C"
#include "ClusterQA_CellCompareV2.C"
#include "ClusterQA_CleanCellLogs.C"

void QAV2(      TString configFileName  = "config.txt",     // set selected
                Bool_t doEventQA        = kFALSE,           // switch on EventQA
                Bool_t doPhotonQA       = kFALSE,           // switch on PCM-PhotonQA
                Bool_t doClusterQA      = kFALSE,           // switch on ClusterQA
                Bool_t doMergedQA       = kFALSE,           // switch on merged ClusterQA
                Bool_t doPrimaryTrackQA = kFALSE,           // switch on primary electron and pion QA
                Bool_t doJetQA          = kFALSE,           // switch on jet QA
                Int_t doExtQA           = 2,                // 0: switched off, 1: normal extQA, 2: with Cell level plots
                TString suffix          = "eps",            // output format of plots
                Bool_t doCellQASummary  = kFALSE           // enables the summary for the cell QA
         ){

    //**************************************************************************************************************
    //******************* global settings **************************************************************************
    //**************************************************************************************************************
    const Int_t maxSets             = 40;
    Int_t cutNr                     = -1;               // if -1 & not overwritten in configFile: you have to choose number at runtime
    TString jetType                 = "";
    Int_t mode                      = -1;               // will abort if not set in file

    //nSets == 0 is always data!
    Int_t nSets                     = 0;
    TString fEnergyFlag             = "";
    TString labelData               = "";
    Bool_t addSubfolder             = kFALSE;
    TString select                  = "";
    TString addPhotonCutNr          = "";
    TString DataSets            [maxSets];
    TString plotDataSets        [maxSets];
    TString pathDataSets        [maxSets];
    TString pathPhotonQA        [maxSets];
    TString fixedCutSelections  [maxSets];
    Bool_t diffPhotonQAPath         = kFALSE;
    Int_t fuseClusterQAValues       = 0;
    Double_t arrQAEnergy[4];
    Double_t arrQATime[4];
    Double_t arrQAHotCells1D[4];
    Double_t arrmin2D[9];
    Double_t arrmax2D[9];

    // initialize arrays
    for (Int_t i = 0; i< maxSets; i++){
        DataSets[i]                 = "";
        plotDataSets[i]             = "";
        pathDataSets[i]             = "";
        pathPhotonQA[i]             = "";
        fixedCutSelections[i]       = "";
    }

    // basic dead cell settings
    Int_t nSetsDeadCell             = 0;
    Int_t nMCSetsDeadCell           = 0;
    Bool_t doDeadCellCompare        = kFALSE;
    Int_t nSetsHotCell              = 0;
    Bool_t doHotCellCompare         = kFALSE;
    Bool_t doCellCompare            = kFALSE;
    Int_t CellCompareNSets          = 0;
    Bool_t doCellCleaning           = kFALSE;
    Bool_t doCellCleaningTemp       = kFALSE;
    Bool_t useConsistentCut         = kTRUE;

    //**************************************************************************************************************
    //******************************* Read config file for detailed settings ***************************************
    //**************************************************************************************************************
    // ATTENTION: The data set has to be separated with either tabs or spaces a mixture of
    //            both will most likely lead to misconfigurations
    //**************************************************************************************************************

    cout << "INFO: You have chosen the given the following config file: " << configFileName.Data() << endl;
    ifstream fileConfigQA;
    fileConfigQA.open(configFileName,ios_base::in);
    if (!fileConfigQA) {
        cout << "ERROR: settings " << configFileName.Data() << " not found!" << endl;
        return;
    }

    // read settings from file
    for( TString tempLine; tempLine.ReadLine(fileConfigQA, kTRUE); ) {
        // check if line should be considered
        if (tempLine.BeginsWith("%") || tempLine.BeginsWith("#")){
            continue;
        }
        cout << tempLine.Data() << endl;

        // Separate the string according to tabulators
        TObjArray *tempArr  = tempLine.Tokenize("\t");
        if(tempArr->GetEntries()<1){
            cout << "nothing to be done" << endl;
            delete tempArr;
            continue;
        } else if (tempArr->GetEntries() == 1 && !((TString)((TObjString*)tempArr->At(0))->GetString()).BeginsWith("enableSubfolder")){
            // Separate the string according to space
            tempArr       = tempLine.Tokenize(" ");
            if(tempArr->GetEntries()<1){
                cout << "nothing to be done" << endl;
                delete tempArr;
                continue;
            } else if (tempArr->GetEntries() == 1 && !((TString)((TObjString*)tempArr->At(0))->GetString()).BeginsWith("enableSubfolder") ) {
                cout << ((TString)((TObjString*)tempArr->At(0))->GetString()).Data() << " has not be reset, no value given!" << endl;
                delete tempArr;
                continue;
            }
        }

        // Put them to the correct variables
        TString tempValue   = (TString)((TObjString*)tempArr->At(0))->GetString();
        if (tempValue.BeginsWith("select",TString::kIgnoreCase)){
            select          = (TString)((TObjString*)tempArr->At(1))->GetString();
        } else if (tempValue.BeginsWith("nSets",TString::kIgnoreCase)){
            nSets           = ((TString)((TObjString*)tempArr->At(1))->GetString()).Atoi();
        } else if (tempValue.BeginsWith("energy",TString::kIgnoreCase)){
            fEnergyFlag     = (TString)((TObjString*)tempArr->At(1))->GetString();
        } else if (tempValue.BeginsWith("labelData",TString::kIgnoreCase)){
            labelData       = (TString)((TObjString*)tempArr->At(1))->GetString();
        } else if (tempValue.BeginsWith("cutNr",TString::kIgnoreCase)){
            cutNr           = ((TString)((TObjString*)tempArr->At(1))->GetString()).Atoi();
        } else if (tempValue.BeginsWith("jetType",TString::kIgnoreCase)){
            jetType         = ((TString)((TObjString*)tempArr->At(1))->GetString());
        } else if (tempValue.BeginsWith("mode",TString::kIgnoreCase)){
            mode            = ((TString)((TObjString*)tempArr->At(1))->GetString()).Atoi();
        } else if (tempValue.BeginsWith("addPhotonCutNr",TString::kIgnoreCase)){
            addPhotonCutNr  = (TString)((TObjString*)tempArr->At(1))->GetString();
        } else if (tempValue.BeginsWith("useConsistentCut",TString::kIgnoreCase)){
            useConsistentCut = kTRUE;
        } else if (tempValue.BeginsWith("enableSubfolder",TString::kIgnoreCase)){
            addSubfolder    = kTRUE;
        } else if (tempValue.BeginsWith("pathDataSets",TString::kIgnoreCase)){
            cout << "setting paths" << endl;
            for(Int_t i = 1; i<tempArr->GetEntries() && i < maxSets ; i++){
                cout << i << "\t" <<((TString)((TObjString*)tempArr->At(i))->GetString()).Data() << endl;
                if (((TString)((TObjString*)tempArr->At(i))->GetString()).CompareTo("stop",TString::kIgnoreCase))
                    pathDataSets[i-1]   = ((TString)((TObjString*)tempArr->At(i))->GetString());
                else
                    i                   = tempArr->GetEntries();
            }
        } else if (tempValue.BeginsWith("pathPhotonQA",TString::kIgnoreCase)){
            cout << "setting paths" << endl;
            diffPhotonQAPath            = kTRUE;
            for(Int_t i = 1; i<tempArr->GetEntries() && i < maxSets ; i++){
                cout << i << "\t" <<((TString)((TObjString*)tempArr->At(i))->GetString()).Data() << endl;
                if (((TString)((TObjString*)tempArr->At(i))->GetString()).CompareTo("stop",TString::kIgnoreCase))
                    pathPhotonQA[i-1]   = ((TString)((TObjString*)tempArr->At(i))->GetString());
                else
                    i                   = tempArr->GetEntries();
            }
        } else if (tempValue.BeginsWith("DataSetNamesPlot",TString::kIgnoreCase)){
            for(Int_t i = 1; i<tempArr->GetEntries() && i < maxSets ; i++){
                if (((TString)((TObjString*)tempArr->At(i))->GetString()).CompareTo("stop",TString::kIgnoreCase))
                    plotDataSets[i-1]   = ((TString)((TObjString*)tempArr->At(i))->GetString());
                else
                    i                   = tempArr->GetEntries();
            }
        } else if (tempValue.BeginsWith("DataSetNames",TString::kIgnoreCase)){
            for(Int_t i = 1; i<tempArr->GetEntries() && i < maxSets ; i++){
                if (((TString)((TObjString*)tempArr->At(i))->GetString()).CompareTo("stop",TString::kIgnoreCase))
                    DataSets[i-1]       = ((TString)((TObjString*)tempArr->At(i))->GetString());
                else
                    i                   = tempArr->GetEntries();
            }
        } else if (tempValue.BeginsWith("fixedCutSelections",TString::kIgnoreCase)){
            for(Int_t i = 1; i<tempArr->GetEntries() && i < maxSets ; i++){
                if (((TString)((TObjString*)tempArr->At(i))->GetString()).CompareTo("stop",TString::kIgnoreCase))
                    fixedCutSelections[i-1]       = ((TString)((TObjString*)tempArr->At(i))->GetString());
                else
                    i                   = tempArr->GetEntries();
            }
        } else if (tempValue.BeginsWith("enableCellQACuts",TString::kIgnoreCase)){
            fuseClusterQAValues          = ((TString)((TObjString*)tempArr->At(1))->GetString()).Atoi();
        } else if (tempValue.BeginsWith("setQAEnergy",TString::kIgnoreCase)){
            for(Int_t i = 1; i<tempArr->GetEntries() && i < 5; i++){
                if (((TString)((TObjString*)tempArr->At(i))->GetString()).CompareTo("stop",TString::kIgnoreCase)){
                    arrQAEnergy[i-1]            = ((TString)((TObjString*)tempArr->At(i))->GetString()).Atof();
                }
                else
                    i                   = tempArr->GetEntries();
            }
        } else if (tempValue.BeginsWith("setQATime",TString::kIgnoreCase)){
            for(Int_t i = 1; i<tempArr->GetEntries() && i < 5; i++){
                if (((TString)((TObjString*)tempArr->At(i))->GetString()).CompareTo("stop",TString::kIgnoreCase)){
                    arrQATime[i-1]              = ((TString)((TObjString*)tempArr->At(i))->GetString()).Atof();
                }
                else
                    i                   = tempArr->GetEntries();
            }
        } else if (tempValue.BeginsWith("setQAHotCells1D",TString::kIgnoreCase)){
            for(Int_t i = 1; i<tempArr->GetEntries() && i < 5; i++){
                if (((TString)((TObjString*)tempArr->At(i))->GetString()).CompareTo("stop",TString::kIgnoreCase)){
                    arrQAHotCells1D[i-1]        = ((TString)((TObjString*)tempArr->At(i))->GetString()).Atof();
                }
                else
                    i                   = tempArr->GetEntries();
            }
        } else if (tempValue.BeginsWith("min2D",TString::kIgnoreCase)){
            for(Int_t i = 1; i<tempArr->GetEntries() && i < 10; i++){
                if (((TString)((TObjString*)tempArr->At(i))->GetString()).CompareTo("stop",TString::kIgnoreCase)){
                    arrmin2D[i-1]               = ((TString)((TObjString*)tempArr->At(i))->GetString()).Atof();
                }
                else
                    i                   = tempArr->GetEntries();
            }
        } else if (tempValue.BeginsWith("max2D",TString::kIgnoreCase)){
            for(Int_t i = 1; i<tempArr->GetEntries() && i < 10; i++){
                if (((TString)((TObjString*)tempArr->At(i))->GetString()).CompareTo("stop",TString::kIgnoreCase)){
                    arrmax2D[i-1]               = ((TString)((TObjString*)tempArr->At(i))->GetString()).Atof();
                }
                else
                    i                   = tempArr->GetEntries();
            }
        // checking if basic dead cell processing variable are around
        } else if (tempValue.BeginsWith("deadCellNSets",TString::kIgnoreCase)){
            nSetsDeadCell   = ((TString)((TObjString*)tempArr->At(1))->GetString()).Atoi();
        } else if (tempValue.BeginsWith("deadCellNMCSets",TString::kIgnoreCase)){
            nMCSetsDeadCell = ((TString)((TObjString*)tempArr->At(1))->GetString()).Atoi();
        // checking if basic hot cell processing variable are around
        } else if (tempValue.BeginsWith("hotCellNSets",TString::kIgnoreCase)){
            nSetsHotCell    = ((TString)((TObjString*)tempArr->At(1))->GetString()).Atoi();
        }  else if (tempValue.BeginsWith("CellCompareNSets",TString::kIgnoreCase)){
            CellCompareNSets    = ((TString)((TObjString*)tempArr->At(1))->GetString()).Atoi();
        // checking for variables needed for cell cleaning
        } else if (tempValue.BeginsWith("cellCleaningUseMaybe",TString::kIgnoreCase)){
            doCellCleaningTemp = kTRUE;
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
    cout << "nSets:\t"<< nSets << endl;
    cout << "fEnergyFlag:\t"<< fEnergyFlag.Data() << endl;
    cout << "cutNr:\t"<< cutNr << endl;
    cout << "jetType:\t"<< jetType << endl;
    cout << "mode:\t"<< mode << endl;
    cout << "addSubfolder:\t"<< addSubfolder << endl;
    cout << "labelData:\t" << labelData.Data() << endl;
    cout << "addPhotonCutNr:\t" << addPhotonCutNr << endl;
    if (nSets == 0 || !fEnergyFlag.CompareTo("") || mode == -1 ){
        cout << "ABORTING: You are missing the nSets, energy or mode setting, can't continue like that" << endl;
        return;
    }
    if (!diffPhotonQAPath){
        cout << "WARNING: photonQA disabled, no path set."<< endl;
        doPhotonQA          = kFALSE;
    }
    cout << "**************************************************************************" << endl;
    cout << "Data set setup: " << endl;
    for (Int_t i = 0; i < nSets; i++){
        if (!plotDataSets[i].CompareTo(""))
            plotDataSets[i]             = DataSets[i];
        cout << i << "\t" << DataSets[i].Data() << "\t" << plotDataSets[i].Data() << "\t" << pathDataSets[i].Data();
        if (doPhotonQA && diffPhotonQAPath) cout << "\t" << pathPhotonQA[i].Data();
        cout << endl;
    }
    if (doCellQASummary){
        if (nSetsDeadCell > 0 || nMCSetsDeadCell > 0){
            cout << "INFO: enabling also DeadCell QA" << endl;
            doDeadCellCompare               = kTRUE;
        } else {
            cout << "WARNING: you requested to run the dead cell QA, but neither the 'deadCellNSets' nor the 'deadCellNMCSets' has been provided...\n disabled running DeadCellCompare-macro... " << endl;

        }

        if (nSetsHotCell > 0 ){
            cout << "INFO: enabling also HotCell QA" << endl;
            doHotCellCompare                = kTRUE;
        } else {
            cout << "WARNING: you requested to run the hot cell QA, but the 'hotCellNSets' has not been provided...\n disabled running HotCellCompare-macro... " << endl;
        }
        cout << "**************************************************************************" << endl;

        cout << "**************************************************************************" << endl;
        cout << "**************************************************************************" << endl;
        cout << "**************************************************************************" << endl;
        if (doCellCleaningTemp){
            doCellCleaning                  = kTRUE;
            cout << "INFO: enabling cleaning of log files " << endl;
        }
        cout << "**************************************************************************" << endl;

        cout << "**************************************************************************" << endl;
        if (CellCompareNSets > 0 ){
            cout << "INFO: enabling also Cell Compare QA" << endl;
            doCellCompare                = kTRUE;
        } else {
            cout << "WARNING: you requested to run the Cell Compare QA, but the 'CellCompareNSets' has not been provided...\n disabled running CellCompare-macro... " << endl;
        }
        cout << "**************************************************************************" << endl;
        cout << "**************************************************************************" << endl;
    }


    //**************************************************************************************************************
    //******************************  Starting individual QA macros ***********************************************
    //**************************************************************************************************************
    if ( doEventQA )    EventQA     (nSets, fEnergyFlag, DataSets, plotDataSets, pathDataSets, fixedCutSelections, mode, cutNr, suffix, labelData, addSubfolder);
    if ( doJetQA )      JetQA     (nSets, fEnergyFlag, DataSets, plotDataSets, pathDataSets, cutNr, jetType, suffix, labelData, addSubfolder);
    if ( doPhotonQA )   PhotonQA    (nSets, fEnergyFlag, DataSets, plotDataSets, pathPhotonQA, mode, cutNr, suffix, labelData, addSubfolder, addPhotonCutNr, useConsistentCut);
    if ( doClusterQA){
        if(fuseClusterQAValues==1)
            ClusterQA   (nSets, fEnergyFlag, DataSets, plotDataSets, pathDataSets, mode, cutNr, doExtQA, suffix, labelData, addSubfolder, kFALSE, fuseClusterQAValues, arrQAEnergy, arrQATime, arrQAHotCells1D, arrmin2D, arrmax2D);
        else
            ClusterQA   (nSets, fEnergyFlag, DataSets, plotDataSets, pathDataSets, mode, cutNr, doExtQA, suffix, labelData, addSubfolder);
    }
    if ( doMergedQA )   ClusterQA   (nSets, fEnergyFlag, DataSets, plotDataSets, pathDataSets, mode, cutNr, doExtQA, suffix, labelData, addSubfolder, kTRUE);
    if ( doPrimaryTrackQA ) PrimaryTrackQA (nSets, fEnergyFlag, DataSets, plotDataSets, pathDataSets, mode, cutNr, suffix, labelData, addSubfolder);

    // **************************************************************************************************************
    // ************************** Cell comparison macros ************************************************************
    //               CAVEAT: These can only be started once the runwise cell QA had been run
    // **************************************************************************************************************
    if ( doDeadCellCompare ) ClusterQA_DeadCellCompareV2(configFileName,suffix);
    if ( doHotCellCompare ) ClusterQA_HotCellCompareV2(configFileName,suffix);
    if ( doCellCleaning ) ClusterQA_CleanCellLogs(configFileName,suffix);
    if ( doCellCompare) ClusterQA_CellCompareV2(configFileName, suffix);
    return;

}
