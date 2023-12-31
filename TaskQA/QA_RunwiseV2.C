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
#include "EventQA_Runwise.C"
#include "JetQA_Runwise.C"
#include "JetQA_Runwise_pPb.C"
#include "PhotonQA_Runwise.C"
// #include "ClusterQA_Runwise.C"
#include "ClusterQA_Runwise_V2.C"
#include "PrimaryTrackQA_Runwise.C"

void QA_RunwiseV2(  TString configFileName  = "configRunwise.txt",  // set selected config file
                    Bool_t  doEventQA       = kFALSE,               // switch on EventQA
                    Bool_t  doPhotonQA      = kFALSE,               // switch on PCM-PhotonQA
                    Bool_t  doClusterQA     = kFALSE,               // switch on ClusterQA
                    Bool_t  doMergedQA      = kFALSE,               // switch on merged ClusterQA
                    Bool_t doPrimaryTrackQA = kFALSE,               // switch on primary electron and pion QA
                    Bool_t doJetQA          = kFALSE,               // switch on Jet QA
                    Bool_t doJetQApPb       = kFALSE,
                    Int_t   doExtQA         = 2,                    // 0: switched off, 1: normal extQA, 2: with Cell level plots
                    TString suffix          = "eps"                 // output format of plots
){
    //**************************************************************************************************************
    //******************* global settings **************************************************************************
    //**************************************************************************************************************
    TString     folderRunlists          = "DownloadAndDataPrep/runlists";
    const Int_t maxSets                 = 40;
    Int_t       cutNr                   = -1;               // if -1 & not overwritten in configFile: you have to choose number at runtime
    TString     jetType                 = "";               // for jet QA only: Full, Charged, or Neutral
    Int_t       mode                    = -1;               // will abort if not set in file

    Int_t       nSets                   = 0;                // nSets == 0 is always data!
    Int_t       nData                   = 0;
    TString     fEnergyFlag             = "";
    TString     filePath                = "";
    TString     filePathMC              = "";
    TString     fileName                = "";
    TString     fileNameMC              = "";
    TString     fileNamePhoton          = "";
    TString     fileNamePhotonMC        = "";
    TString     filePathPhoton          = "";
    TString     select                  = "";
    TString     addPhotonCutNr          = "";
    TString     addLabelRunlist         = "";
    TString     addLabelRunlistMC       = "";
    TString     mcFolderExtension       = "";
    // initialize arrays
    TString     DataSets[maxSets];
    TString     plotDataSets[maxSets];
    for (Int_t i=0; i<maxSets; i++) {
        DataSets[i]                     = "";
        plotDataSets[i]                 = "";
    }

    Size_t      markerSize              = 1.25;

    Bool_t      doEquidistantXaxis      = kFALSE;
    Bool_t      doTrigger               = kFALSE; // was: kTRUE
    Bool_t      doHistsForEverySet      = kFALSE; // was: kTRUE
    Bool_t      onlytrending            = kFALSE; // was: kTRUE
    Bool_t      useDataRunListForMC     = kFALSE;
    Bool_t      addSubFolder            = kFALSE;
    Int_t       nSigmasBadRun[8]        = {2,2,2,2,2,2,2,2};
    TString     fixedTopDir             = "";
    Bool_t      SwithManyRuns           = kFALSE; // was: kTRUE

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
                cout << ((TString)((TObjString*)tempArr->At(0))->GetString()).Data() << " has not been reset, no value given!" << endl;
                delete tempArr;
                continue;
            }
        }

        // Put them to the correct variables
        TString tempValue       = (TString)((TObjString*)tempArr->At(0))->GetString();
        if (tempValue.BeginsWith("select",TString::kIgnoreCase)){
            select              = (TString)((TObjString*)tempArr->At(1))->GetString();
        } else if (tempValue.BeginsWith("folderRunlists",TString::kIgnoreCase)){
            folderRunlists      = ((TString)((TObjString*)tempArr->At(1))->GetString());
        } else if (tempValue.BeginsWith("nSets",TString::kIgnoreCase)){
            nSets               = ((TString)((TObjString*)tempArr->At(1))->GetString()).Atoi();
        } else if (tempValue.BeginsWith("nData",TString::kIgnoreCase)){
            nData               = ((TString)((TObjString*)tempArr->At(1))->GetString()).Atoi();
        } else if (tempValue.BeginsWith("energy",TString::kIgnoreCase)){
            fEnergyFlag         = (TString)((TObjString*)tempArr->At(1))->GetString();
        } else if (tempValue.BeginsWith("filePathPhoton",TString::kIgnoreCase)){
            filePathPhoton      = (TString)((TObjString*)tempArr->At(1))->GetString();
        } else if ((tempValue.BeginsWith("filePath",TString::kIgnoreCase))&&!(tempValue.BeginsWith("filePathMC",TString::kIgnoreCase))){
            filePath            = (TString)((TObjString*)tempArr->At(1))->GetString();
        } else if (tempValue.BeginsWith("filePathMC",TString::kIgnoreCase)){
            filePathMC          = (TString)((TObjString*)tempArr->At(1))->GetString();
        } else if ((tempValue.BeginsWith("fileNamePhoton",TString::kIgnoreCase))&&!(tempValue.BeginsWith("fileNamePhotonMC",TString::kIgnoreCase))){
            fileNamePhoton      = (TString)((TObjString*)tempArr->At(1))->GetString();
        } else if (tempValue.BeginsWith("fileNamePhotonMC",TString::kIgnoreCase)){
            fileNamePhotonMC    = (TString)((TObjString*)tempArr->At(1))->GetString();
        } else if ((tempValue.BeginsWith("fileName",TString::kIgnoreCase))&&!(tempValue.BeginsWith("fileNameMC",TString::kIgnoreCase))){
            fileName            = (TString)((TObjString*)tempArr->At(1))->GetString();
        } else if (tempValue.BeginsWith("fileNameMC",TString::kIgnoreCase)){
            fileNameMC          = (TString)((TObjString*)tempArr->At(1))->GetString();
        } else if (tempValue.BeginsWith("addPhotonCutNr",TString::kIgnoreCase)){
            addPhotonCutNr      = (TString)((TObjString*)tempArr->At(1))->GetString();
        } else if ((tempValue.BeginsWith("addLabelRunlist",TString::kIgnoreCase))&&!(tempValue.BeginsWith("addLabelRunlistMC",TString::kIgnoreCase))){
            addLabelRunlist     = (TString)((TObjString*)tempArr->At(1))->GetString();
        } else if (tempValue.BeginsWith("addLabelRunlistMC",TString::kIgnoreCase)){
            addLabelRunlistMC   = (TString)((TObjString*)tempArr->At(1))->GetString();
        } else if (tempValue.BeginsWith("mcFolderExtension",TString::kIgnoreCase)){
            mcFolderExtension   = (TString)((TObjString*)tempArr->At(1))->GetString();
        } else if (tempValue.BeginsWith("cutNr",TString::kIgnoreCase)){
            cutNr               = ((TString)((TObjString*)tempArr->At(1))->GetString()).Atoi();
                } else if (tempValue.BeginsWith("jetType",TString::kIgnoreCase)){
            jetType             = ((TString)((TObjString*)tempArr->At(1))->GetString()) + " Jets";
        } else if (tempValue.BeginsWith("mode",TString::kIgnoreCase)){
            mode                = ((TString)((TObjString*)tempArr->At(1))->GetString()).Atoi();
        } else if (tempValue.BeginsWith("markerSize",TString::kIgnoreCase)){
            markerSize          = (Size_t)((TString)((TObjString*)tempArr->At(1))->GetString()).Atoi();
        } else if (tempValue.BeginsWith("enableSubfolder",TString::kIgnoreCase)){
            addSubFolder        = kTRUE;
        } else if (tempValue.BeginsWith("doEquidistantXaxis",TString::kIgnoreCase)){
            doEquidistantXaxis  = kTRUE;
        } else if (tempValue.BeginsWith("doTrigger",TString::kIgnoreCase)){
            doTrigger           = kTRUE;
        } else if (tempValue.BeginsWith("doHistsForEverySet",TString::kIgnoreCase)){
            doHistsForEverySet  = kTRUE;
        } else if (tempValue.BeginsWith("onlytrending",TString::kIgnoreCase)){
            onlytrending  = kTRUE;
        } else if (tempValue.BeginsWith("SwithManyRuns",TString::kIgnoreCase)){
            SwithManyRuns  = kTRUE;
        } else if (tempValue.BeginsWith("useDataRunListForMC",TString::kIgnoreCase)){
            useDataRunListForMC = kTRUE;
        } else if (tempValue.BeginsWith("fixedTopDir",TString::kIgnoreCase)){
            fixedTopDir     = (TString)((TObjString*)tempArr->At(1))->GetString();
        } else if (tempValue.BeginsWith("DataSets",TString::kIgnoreCase)){
            for(Int_t i = 1; i<tempArr->GetEntries() && i < maxSets ; i++){
                if (((TString)((TObjString*)tempArr->At(i))->GetString()).CompareTo("stop",TString::kIgnoreCase))
                    DataSets[i-1]       = ((TString)((TObjString*)tempArr->At(i))->GetString());
                else
                    i                   = tempArr->GetEntries();
            }
        } else if (tempValue.BeginsWith("plotDataSets",TString::kIgnoreCase)){
            for(Int_t i = 1; i<tempArr->GetEntries() && i < maxSets ; i++){
                if (((TString)((TObjString*)tempArr->At(i))->GetString()).CompareTo("stop",TString::kIgnoreCase))
                    plotDataSets[i-1]   = ((TString)((TObjString*)tempArr->At(i))->GetString());
                else
                    i                   = tempArr->GetEntries();
            }
        }

        delete tempArr;
    }

    if (fileNamePhoton.CompareTo("") == 0) fileNamePhoton = fileName;
    if (fileNameMC.CompareTo("") == 0) fileNameMC = fileName;

    //**************************************************************************************************************
    //******************************* Check wether settings were valid *********************************************
    //**************************************************************************************************************
    cout << "**************************************************************************" << endl;
    cout << "**************** Settings found in config file ***************************" << endl;
    cout << "**************************************************************************" << endl;
    cout << "configFileName:        " << configFileName.Data()      << endl;
    cout << "fEnergyFlag:           " << fEnergyFlag.Data()         << endl;
    cout << "select:                " << select.Data()              << endl;
    cout << endl;
    cout << "folderRunlists:        " << folderRunlists.Data()      << endl;
    cout << "fileName:              " << fileName.Data()            << endl;
    cout << "fileNameMC:            " << fileNameMC.Data()            << endl;
    cout << "filePath:              " << filePath.Data()            << endl;
    cout << "filePathMC:            " << filePathMC.Data()            << endl;
    cout << "filePathPhoton:        " << filePathPhoton.Data()      << endl;
    cout << "fileNamePhoton:        " << fileNamePhoton.Data()      << endl;
    cout << "cutNr:                 " << cutNr                      << endl;
    cout << "jetType:               " << jetType                    << endl;
    cout << "mode:                  " << mode                       << endl;
    cout << endl;
    cout << "markerSize:            " << markerSize                 << endl;
    cout << endl;
    cout << "doEquidistantXaxis:    " << doEquidistantXaxis         << endl;
    cout << "doTrigger:             " << doTrigger                  << endl;
    cout << "doHistsForEverySet:    " << doHistsForEverySet         << endl;
    cout << "onlytrending:          " << onlytrending         << endl;
    cout << "SwithManyRuns:         " << SwithManyRuns         << endl;
    cout << "useDataRunListForMC:   " << useDataRunListForMC        << endl;
    cout << "addSubFolder:          " << addSubFolder               << endl;
    cout << "addLabelRunlist:       " << addLabelRunlist            << endl;
    cout << "addPhotonCutNr:        " << addPhotonCutNr             << endl;
    cout << endl;
    cout << "nSets:                 " << nSets                      << endl;
    cout << "nData:                 " << nData                      << endl;

    if (nSets == 0 || !fEnergyFlag.CompareTo("") || mode == -1 ){
        cout << "ABORTING: You are missing the nSets, energy or mode setting, can't continue like that" << endl;
        return;
    }

    if (mode == -2 ){
        cout << "Mode is set to -2. This setting is for Jet QA only. Macro will fail if any other QA is attemted." << endl;
    }

    cout << "**************************************************************************" << endl;
    cout << "Data set setup: " << endl;
    for (Int_t i = 0; i < nSets; i++){
        if (!plotDataSets[i].CompareTo(""))
            plotDataSets[i]             = DataSets[i];
        cout << i << "\t" << DataSets[i].Data() << "\t" << plotDataSets[i].Data() << endl;
    }
    cout << "**************************************************************************" << endl;
    cout << "**************************************************************************" << endl;


    //**************************************************************************************************************
    //******************************  Starting individual QA macros ***********************************************
    //**************************************************************************************************************
    if (doEventQA)      EventQA_Runwise(    nSets, nData, fEnergyFlag, filePath, fileName, fileNameMC, DataSets, plotDataSets, mode, cutNr,
                                            doExtQA,doEquidistantXaxis, doTrigger, doHistsForEverySet, addSubFolder, useDataRunListForMC, markerSize, suffix, folderRunlists, nSigmasBadRun,
                                            addLabelRunlist, fixedTopDir );
    if (doJetQA)      JetQA_Runwise(    nSets, nData, fEnergyFlag, filePath, filePathMC, fileName, fileNameMC, DataSets, plotDataSets, cutNr,
                                            jetType, doExtQA,doEquidistantXaxis, doTrigger, doHistsForEverySet, addSubFolder, useDataRunListForMC, markerSize, suffix, folderRunlists, nSigmasBadRun,
                                            addLabelRunlist, fixedTopDir );
    if (doJetQApPb)      JetQA_Runwise_pPb(    nSets, nData, fEnergyFlag, filePath, filePathMC, mcFolderExtension, fileName, fileNameMC, DataSets, plotDataSets, cutNr,
                                            jetType, doExtQA,doEquidistantXaxis, doTrigger, doHistsForEverySet, addSubFolder, useDataRunListForMC, markerSize, suffix, folderRunlists, nSigmasBadRun,
                                            addLabelRunlist, addLabelRunlistMC, fixedTopDir );
    if (doPhotonQA) {
        TString                         path = filePath;
        if(!filePathPhoton.IsNull())    path = filePathPhoton;
        PhotonQA_Runwise(   nSets, nData, fEnergyFlag, path, fileNamePhoton, DataSets, plotDataSets, mode, cutNr,
                            doExtQA, doEquidistantXaxis, doTrigger, doHistsForEverySet, addSubFolder, useDataRunListForMC, markerSize, suffix, folderRunlists, addLabelRunlist, addPhotonCutNr);
    }
    if (doClusterQA){
//       if (SwithManyRuns)
        ClusterQA_Runwise_V2(  nSets, nData, fEnergyFlag, filePath, fileName, fileNameMC, DataSets, plotDataSets, mode, cutNr, doExtQA, doEquidistantXaxis, doTrigger, doHistsForEverySet, addSubFolder, useDataRunListForMC, markerSize, suffix, folderRunlists, addLabelRunlist, kFALSE, 80, onlytrending );
//       else
//           ClusterQA_Runwise(  nSets, nData, fEnergyFlag, filePath, fileName, fileNameMC, DataSets, plotDataSets, mode, cutNr, doExtQA, doEquidistantXaxis, doTrigger, doHistsForEverySet, addSubFolder, useDataRunListForMC, markerSize, suffix, folderRunlists, addLabelRunlist );
    }
    if (doMergedQA){
      ClusterQA_Runwise_V2(  nSets, nData, fEnergyFlag, filePath, fileName, fileNameMC, DataSets, plotDataSets, mode, cutNr,
                             1, doEquidistantXaxis, doTrigger, doHistsForEverySet, addSubFolder, useDataRunListForMC, markerSize, suffix, folderRunlists, addLabelRunlist, kFALSE, 80, onlytrending );

//       ClusterQA_Runwise(  nSets, nData, fEnergyFlag, filePath, fileName, fileNameMC, DataSets, plotDataSets, mode, cutNr,
//                           1, doEquidistantXaxis, doTrigger, doHistsForEverySet, addSubFolder, useDataRunListForMC, markerSize, suffix, folderRunlists, addLabelRunlist, kTRUE );
    }
    if ( doPrimaryTrackQA ) {
        PrimaryTrackQA_Runwise (nSets, nData, fEnergyFlag, filePath, fileName, DataSets,  plotDataSets, mode, cutNr, doExtQA, doEquidistantXaxis, doTrigger, doHistsForEverySet, addSubFolder, useDataRunListForMC, markerSize, suffix, folderRunlists, addLabelRunlist);
    }
    return;
}
