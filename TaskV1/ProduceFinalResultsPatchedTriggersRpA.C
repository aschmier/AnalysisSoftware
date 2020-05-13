/****************************************************************************************************************************
 ******     provided by Gamma Conversion Group, PWGGA,                                                                  *****
 ******        Friederike Bock, friederike.bock@cern.ch                                                                 *****
 ******        Daniel Muehlheim, d.muehlheim@cern.ch                                                                    *****
 *****************************************************************************************************************************/

#include <Riostream.h>
#include <fstream>
#include "TMath.h"
#include <stdlib.h>
#include <fstream>
#include <math.h>
#include <string>
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
#include "TFitResultPtr.h"
#include "TFitResult.h"
#include "TMatrixDSym.h"
#include "TArrow.h"
#include "TGraphAsymmErrors.h"
#include "TGaxis.h"
#include "TMarker.h"
#include "../CommonHeaders/PlottingGammaConversionHistos.h"
#include "../CommonHeaders/PlottingGammaConversionAdditional.h"
#include "../CommonHeaders/FittingGammaConversion.h"
#include "../CommonHeaders/ExtractSignalBinning.h"
// #include "../CommonHeaders/ConversionFunctionsBasicsAndLabeling.h"
#include "../CommonHeaders/ConversionFunctions.h"
#include "../CommonHeaders/CombinationFunctions.h"

extern TRandom*    gRandom;
extern TBenchmark* gBenchmark;
extern TSystem*    gSystem;
extern TMinuit*    gMinuit;

struct SysErrorConversion {
    Double_t value;
    Double_t error;
    //    TString name;
};

bool replace(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos) return false;
    str.replace(start_pos, from.length(), to);
    return true;
}
Int_t GetBinPosInVec( std::vector<TString> *vec, Int_t size, Double_t lookup){
    for(Int_t i=0; i<size; i++){
        if(((TString)vec[i].at(0)).Atof() == lookup) return i;
    }
    return -1;
}

TGraphAsymmErrors * DivideTGraphAsymErrorByTGraphAsymError(TGraphAsymmErrors* graph1, TGraphAsymmErrors* graph2, TString nameGraph){
	TGraphAsymmErrors* dummyGraph = (TGraphAsymmErrors*)graph1->Clone(Form("%s_R1",graph1->GetName()));
	Int_t nPoints              = dummyGraph->GetN();
	Double_t * xValue1         = dummyGraph->GetX();
	Double_t * yValueTemp      = dummyGraph->GetY();
	Double_t * yValue1         = dummyGraph->GetY();
	Double_t* xErrorLow1       = dummyGraph->GetEXlow();
	Double_t* xErrorHigh1      = dummyGraph->GetEXhigh();
	Double_t* yErrorLow1       = dummyGraph->GetEYlow();
	Double_t* yErrorHigh1      = dummyGraph->GetEYhigh();

	Double_t * yValue2         = graph2->GetY();
	Double_t* yErrorLow2       = graph2->GetEYlow();
	Double_t* yErrorHigh2      = graph2->GetEYhigh();

	for (Int_t i = 0; i < nPoints; i++){
        if(yValue2[i]!=0)
            yValueTemp[i]               = yValue1[i]/yValue2[i];
        else
            yValueTemp[i]               = 0;
		yErrorLow1[i]            = yValueTemp[i]*TMath::Sqrt( TMath::Power(yErrorLow1[i]/yValue1[i],2) + TMath::Power(yErrorLow2[i]/yValue2[i],2));
		yErrorHigh1[i]           = yValueTemp[i]*TMath::Sqrt( TMath::Power(yErrorHigh1[i]/yValue1[i],2) + TMath::Power(yErrorHigh2[i]/yValue2[i],2));
	}

	TGraphAsymmErrors* returnGraph =  new TGraphAsymmErrors(nPoints,xValue1,yValueTemp,xErrorLow1,xErrorHigh1,yErrorLow1,yErrorHigh1);
    returnGraph->SetName(nameGraph.Data());
	return returnGraph;
}

TGraphAsymmErrors* ScaleGraphAsym (TGraphAsymmErrors* graph, Double_t scaleFac){
	TGraphAsymmErrors* dummyGraph = (TGraphAsymmErrors*)graph->Clone(Form("%s_Scaled",graph->GetName()));
	Double_t * xValue         = dummyGraph->GetX();
	Double_t * yValue         = dummyGraph->GetY();
	Double_t* xErrorLow       = dummyGraph->GetEXlow();
	Double_t* xErrorHigh      = dummyGraph->GetEXhigh();
	Double_t* yErrorLow       = dummyGraph->GetEYlow();
	Double_t* yErrorHigh      = dummyGraph->GetEYhigh();
	Int_t nPoints             = dummyGraph->GetN();
	for (Int_t i = 0; i < nPoints; i++){
		yValue[i]               = yValue[i]*scaleFac;
		yErrorLow[i]            = yErrorLow[i]*scaleFac;
		yErrorHigh[i]           = yErrorHigh[i]*scaleFac;
	}
	TGraphAsymmErrors* returnGraph =  new TGraphAsymmErrors(nPoints,xValue,yValue,xErrorLow,xErrorHigh,yErrorLow,yErrorHigh);
	return returnGraph;
}
//***************************************************************************************************************
//************************* Get correct order of triggers for combinations **************************************
//***************************************************************************************************************
Int_t GetOrderedTrigger(TString triggerNameDummy){
    if ((triggerNameDummy.CompareTo("MB") == 0 || triggerNameDummy.CompareTo("INT1") == 0  || triggerNameDummy.CompareTo("MB_NLM2") == 0 || triggerNameDummy.CompareTo("INT1_NLM2") == 0 ) ){
        return 0;
    } else if ((triggerNameDummy.CompareTo("INT7") == 0 || triggerNameDummy.CompareTo("INT7_NLM2") == 0) ){
        return 1;
    } else if ((triggerNameDummy.CompareTo("EMC1") == 0 || triggerNameDummy.CompareTo("EMC1_NLM2") == 0 ) ){
        return 2;
    } else if ((triggerNameDummy.CompareTo("EMC7") == 0 || triggerNameDummy.CompareTo("EMC7_NLM2") == 0 ) ){
        return 3;
    } else if ((triggerNameDummy.CompareTo("EG2") == 0 || triggerNameDummy.CompareTo("EG2_NLM2") == 0 ||  triggerNameDummy.CompareTo("EGA") == 0) ){
        return 4;
    } else if ((triggerNameDummy.CompareTo("EG1") == 0 || triggerNameDummy.CompareTo("EG1_NLM2") == 0 ) ){
        return 5;
    } else if ((triggerNameDummy.CompareTo("MB_NLM1") == 0 || triggerNameDummy.CompareTo("INT1_NLM1") == 0  ) ){
        return 6;
    } else if (triggerNameDummy.CompareTo("INT7_NLM1") == 0 || triggerNameDummy.CompareTo("INT7_CF") == 0){
        return 7;
    } else if (triggerNameDummy.CompareTo("EMC1_NLM1") == 0 ){
        return 8;
    } else if (triggerNameDummy.CompareTo("EMC7_NLM1") == 0 ){
        return 9;
    } else if (triggerNameDummy.CompareTo("EG2_NLM1") == 0 || triggerNameDummy.CompareTo("EJ2") == 0){
        return 10;
    } else if (triggerNameDummy.CompareTo("EG1_NLM1") == 0  || triggerNameDummy.CompareTo("EJ1") == 0){
        return 11;
    } else
    return -1;
}

//***************************************************************************************************************
//***************************** Main function *******************************************************************
//***************************************************************************************************************
void  ProduceFinalResultsPatchedTriggersRpA(
    TString fileListNamePi0     = "triggerFileListPi0.txt",
    Int_t   mode                = 4,
    Int_t   numberOfTrigg       = 6,
    TString suffix              = "eps",
    TString isMC                = "",
    TString optionEnergy        = "",
    TString period              = "",
    Float_t maxPtGlobalPi0      = 16.,
    Bool_t  averagedPi0         = kFALSE,
    Bool_t  enableEta           = kTRUE,
    Float_t maxPtGlobalEta      = 14.,
    Bool_t  averagedEta         = kFALSE,
    TString fileInputCorrFactors= "",
    Bool_t  isNSD               = kTRUE,
    TString fileInputBinShift   = "",
    TString fileInputBinShiftRef   = ""
){

    //***************************************************************************************************************
    //************************************ Layouting preparations & general setup ***********************************
    //***************************************************************************************************************
    gROOT->Reset();
    gROOT->SetStyle("Plain");

    StyleSettingsThesis();
    SetPlotStyle();

    TString dateForOutput           = ReturnDateStringForOutput();
    TString collisionSystem         = ReturnFullCollisionsSystem(optionEnergy);
    TString detectionProcess        = ReturnFullTextReconstructionProcess(mode);
    TString detectionProcessClus    = ReturnFullTextReconstructionProcess(mode,2);

    const Int_t MaxNumberOfFiles    = 12;
    Size_t textSizeSpectra          = 0.04;
    Int_t textSizePixelSpectra      = textSizeSpectra*1000;

    TString nameTriggerWeighted[MaxNumberOfFiles]  = {
        "INT1", "INT7", "EMC1", "EMC7", "EG2", "EG1",
        "INT1_NLM1", "INT7_NLM1", "EMC1_NLM1", "EMC7_NLM1", "EG2_NLM1", "EG1_NLM1"
    };
    if(optionEnergy.CompareTo("2.76TeV")!=0){
        nameTriggerWeighted[10] = "EJ2";
        nameTriggerWeighted[11] = "EJ1";
    }

    TString system              = "PCM";
    if (mode == 2) system       = "PCM-EMCAL";
    if (mode == 3) system       = "PCM-PHOS";
    if (mode == 4) system       = "EMCAL-EMCAL";
    if (mode == 5) system       = "PHOS-PHOS";
    if (mode == 10) system      = "EMC-merged";
    if (mode == 11) system      = "PHOS-merged";

    TString eventCutNumber       [MaxNumberOfFiles];
    TString inputFileName       [MaxNumberOfFiles];
    TString inputFolderName [MaxNumberOfFiles];
    TString triggerName     [MaxNumberOfFiles];
    TString triggerNameLabel[MaxNumberOfFiles];
    TString inputFileNameRef     [MaxNumberOfFiles];
    TString inputFolderNameRef [MaxNumberOfFiles];
    TString triggerNameRef   [MaxNumberOfFiles];
    TString triggerNameRefLabel[MaxNumberOfFiles];
    Float_t ptFromSpecPi0   [MaxNumberOfFiles][2];
    Float_t ptFromSpecEta   [MaxNumberOfFiles][2];
    Bool_t maskedFullyPi0   [MaxNumberOfFiles];
    Bool_t maskedFullyEta   [MaxNumberOfFiles];
    TString sysFilePi0      [MaxNumberOfFiles];
    TString sysFileEta      [MaxNumberOfFiles];

    //***************************************************************************************************************
    //*************************** read setting from configuration file **********************************************
    //***************************************************************************************************************
    ifstream in(fileListNamePi0.Data());
    cout<<"Available Triggers:"<<endl;
    // general number of triggers set
    Int_t nrOfTrigToBeComb      = 0;
    // number of triggers which are really used for the respective analysis
    Int_t nrOfTrigToBeCombPi0Red        = 0;
    Int_t nrOfTrigToBeCombEtaRed        = 0;
    Int_t nrOfTrigToBeCombEtaToPi0Red   = 0;
    while(!in.eof() && nrOfTrigToBeComb<numberOfTrigg ){
        in >> eventCutNumber[nrOfTrigToBeComb] >> inputFileName[nrOfTrigToBeComb] >> inputFolderName[nrOfTrigToBeComb]  >> triggerName[nrOfTrigToBeComb] >> inputFileNameRef[nrOfTrigToBeComb] >> inputFolderNameRef[nrOfTrigToBeComb] >> triggerNameRef[nrOfTrigToBeComb] >> ptFromSpecPi0[nrOfTrigToBeComb][0] >> ptFromSpecPi0[nrOfTrigToBeComb][1]
        >> ptFromSpecEta[nrOfTrigToBeComb][0] >> ptFromSpecEta[nrOfTrigToBeComb][1] >> sysFilePi0[nrOfTrigToBeComb] >> sysFileEta[nrOfTrigToBeComb];
        cout<< inputFileName[nrOfTrigToBeComb]<< "\t"<< inputFolderName[nrOfTrigToBeComb] <<"\t"<< triggerName[nrOfTrigToBeComb] <<endl;
        cout<< inputFileNameRef[nrOfTrigToBeComb]<< "\t"<< inputFolderNameRef[nrOfTrigToBeComb]<< "\t"<< triggerNameRef[nrOfTrigToBeComb] <<endl;
        cout<< "Pi0: " << ptFromSpecPi0[nrOfTrigToBeComb][0] << " < pT < " << ptFromSpecPi0[nrOfTrigToBeComb][1] << "\t sys input: " << sysFilePi0[nrOfTrigToBeComb]<<endl;
        cout<< "Eta: " << ptFromSpecEta[nrOfTrigToBeComb][0] << " < pT < " << ptFromSpecEta[nrOfTrigToBeComb][1] << "\t sys input: " << sysFileEta[nrOfTrigToBeComb]<<endl;
        nrOfTrigToBeComb++;
    }
    nrOfTrigToBeCombPi0Red      = nrOfTrigToBeComb;
    nrOfTrigToBeCombEtaRed      = nrOfTrigToBeComb;

    for (Int_t i = 0; i < nrOfTrigToBeComb; i++){
        // figure out which triggers are fully masked for the pi0
        if ((ptFromSpecPi0[i][1] == -1 && ptFromSpecPi0[i][0] == -1 )|| (ptFromSpecPi0[i][0] == 0 && ptFromSpecPi0[i][1] == 0)){
            maskedFullyPi0[i]       = kTRUE;
            nrOfTrigToBeCombPi0Red--;
        } else {
            maskedFullyPi0[i]       = kFALSE;
        }
        // figure out which triggers are fully masked for the eta
        if ((ptFromSpecEta[i][1] == -1 && ptFromSpecEta[i][0] == -1 )|| (ptFromSpecEta[i][0] == 0 && ptFromSpecEta[i][1] == 0)){
            maskedFullyEta[i]       = kTRUE;
            nrOfTrigToBeCombEtaRed--;
        } else {
            maskedFullyEta[i]       = kFALSE;
        }
    }

    cout << __LINE__<<endl;

    Double_t minPtGlobalPi0         = ptFromSpecPi0[0][0];
    Double_t minPtGlobalEta         = ptFromSpecEta[0][0];
    if (maskedFullyPi0[0])
        minPtGlobalPi0              = 12;
    if (maskedFullyEta[0])
        minPtGlobalEta              = 12;


    for (Int_t j = 1; j < nrOfTrigToBeComb; j++){
        if (minPtGlobalPi0 > ptFromSpecPi0[j][0] && !maskedFullyPi0[j] )
            minPtGlobalPi0 = ptFromSpecPi0[j][0];
        if (minPtGlobalEta > ptFromSpecEta[j][0] && !maskedFullyEta[j] )
            minPtGlobalEta = ptFromSpecEta[j][0];
    }
    cout << "global minimum pT for pi0: " << minPtGlobalPi0 << endl;
    cout << "global minimum pT for eta: " << minPtGlobalEta << endl;

    Double_t xsection =0;
    Double_t xsectionReference =0;
    Double_t nuclOverlap = 0;
    if(optionEnergy.Contains("pPb_5.023TeV")){
        xsection = 2.095;
        xsectionReference = ReturnCorrectXSection("5TeV",1);
        nuclOverlap = GetTAAFromName("","pPb_5.023TeV");
    }
    if(optionEnergy.Contains("pPb_8TeV")){
        xsection = 2.095;
        xsectionReference = ReturnCorrectXSection("8TeV",1);
        nuclOverlap = GetTAAFromName("","pPb_8TeV");
    }
    if(xsectionReference==0){
        cout << "Cross section for reference or spectrum not set... returning! Please adjust loading of reference xsection in line " << __LINE__ << endl;
        return;
    }
    if(nuclOverlap==0){
        cout << "TAA for reference not set... returning! Please adjust loading of nucl. overlap in line " << __LINE__ << endl;
        return;
    }
    Double_t scalingToNSD                       = 0.97;
    if(optionEnergy.Contains("pPb_8TeV")) scalingToNSD  = 0.983;
    if (!isNSD)
        scalingToNSD                            = 1;

    // output naming variables
    TString fCent           = "";
    TString fCentOutput     = "";
    TString centEstimator   = "";
    TString fMultiplicityCut                    = "";

    // put correct color setting for different triggers
    Color_t colorTrigg      [MaxNumberOfFiles];
        for(Int_t set=0;set<MaxNumberOfFiles;set++) colorTrigg[set]= kBlack;
    Color_t colorTriggShade [MaxNumberOfFiles];
        for(Int_t set=0;set<MaxNumberOfFiles;set++) colorTriggShade[set]= kGray+1;
    Marker_t markerTrigg    [MaxNumberOfFiles];
        for(Int_t set=0;set<MaxNumberOfFiles;set++) markerTrigg[set]= 20;
    Marker_t markerTriggMC  [MaxNumberOfFiles];
        for(Int_t set=0;set<MaxNumberOfFiles;set++) markerTriggMC[set]= 24;
    Size_t sizeTrigg        [MaxNumberOfFiles];
        for(Int_t set=0;set<MaxNumberOfFiles;set++) sizeTrigg[set]= 1.5;

    for (Int_t i = 0; i < numberOfTrigg; i++){
        colorTrigg[i]       = GetDefaultTriggerColorName(triggerName[i], 0, optionEnergy);
        colorTriggShade[i]  = GetDefaultTriggerColorName(triggerName[i], 1, optionEnergy);
        markerTrigg[i]      = GetDefaultTriggerMarkerStyleName(triggerName[i], 0);
        markerTriggMC[i]    = GetDefaultTriggerMarkerStyleName(triggerName[i], 1);
        sizeTrigg[i]        = GetDefaultTriggerMarkerSizeName(triggerName[i], 0);
        triggerNameLabel[i] = Form("%s & %s",triggerName[i].Data(), triggerNameRef[i].Data());

        if ((optionEnergy.Contains("Pb") || optionEnergy.Contains("Xe")) && i == 0 ){
            fCent                                   = GetCentralityString(eventCutNumber[i]);
            centEstimator                           = GetCentralityEstimatorString(eventCutNumber[i]);
            fCentOutput                             = GetCentralityStringOutput(eventCutNumber[i]);
            collisionSystem                         = fCent+ " "+collisionSystem;
        }
    }
    cout << __LINE__<<endl;

    //***************************************************************************************************************
    //*************************** set common binning ****************************************************************
    //***************************************************************************************************************
    Int_t combTriggerSet            = -1;
    if (( optionEnergy.CompareTo("pPb_5.023TeV") == 0 || optionEnergy.CompareTo("5TeVRefpPb") == 0 )&& numberOfTrigg == 1 && mode == 2) // need to make exception for RpA as pp only has MB
        combTriggerSet              = 0;

    Double_t binningPi0[400];
    Int_t maxNBinsPi0Abs            = 0;
    Int_t maxNBinsPi0               = GetBinning( binningPi0, maxNBinsPi0Abs, "Pi0", optionEnergy, mode, combTriggerSet, kFALSE, fCent );
    Int_t maxNAllowedPi0            = 0;
    Int_t nRealTriggers             = 0;
    cout << "binning pi0" << endl;
    while (binningPi0[maxNAllowedPi0] < maxPtGlobalPi0 ) maxNAllowedPi0++;
    for (Int_t i= 0; i< maxNAllowedPi0+1; i++){
        cout << binningPi0[i] << ", ";
    }
    cout << endl;

    Double_t binningEta[400];
    Int_t maxNBinsEtaAbs            = 0;
    Int_t maxNBinsEta               = GetBinning( binningEta, maxNBinsEtaAbs, "Eta", optionEnergy, mode, combTriggerSet, kFALSE, fCent );
    Int_t maxNAllowedEta            = 0;
    if (enableEta){
        cout << "binning eta" << endl;
        while (binningEta[maxNAllowedEta] < maxPtGlobalEta ) maxNAllowedEta++;
        for (Int_t i= 0; i< maxNAllowedEta+1; i++){
            cout << binningEta[i] << ", ";
        }
        cout << endl;
    }

    //***************************************************************************************************************
    // defining output directory
    //***************************************************************************************************************
    TString outputDir =    Form("%s/%s/%s/FinalResultsTriggersPatchedNuclModFac%s", suffix.Data(),optionEnergy.Data(),dateForOutput.Data(),system.Data());
    TString outputDirDay =    Form("%s/%s/%s", suffix.Data(),optionEnergy.Data(),dateForOutput.Data());
    // if (optionEnergy.Contains("Pb") || optionEnergy.Contains("Xe")){
    //     outputDir       = outputDir+"_"+fCentOutput;
    // }

    gSystem->Exec("mkdir -p "+outputDir);
    cout << __LINE__<<endl;

    gSystem->Exec(Form("cp %s %s/configurationFile.txt", fileListNamePi0.Data(), outputDir.Data()));

    TString nameFinalResDat                     = Form("%s/FitResults%s.dat",outputDir.Data(),isMC.Data());
    fstream  fileFitsOutput;
    fileFitsOutput.open(nameFinalResDat.Data(), ios::out);
    TString sysStringComb = "PCM";
    if(mode == 2) sysStringComb = "PCMEMC";
    else if(mode == 3) sysStringComb = "PCMPHOS";
    else if(mode == 4) sysStringComb = "EMCEMC";
    else if(mode == 5) sysStringComb = "PHOS";
    else if(mode == 10) sysStringComb = "mEMC";

    //***************************************************************************************************************
    //******************************** Load Pi0 histograms **********************************************************
    //***************************************************************************************************************
    TFile*  fileInput        [MaxNumberOfFiles];
    TFile*  fileInputRef      [MaxNumberOfFiles];

    TH1D*   histoRawYieldsPi0[MaxNumberOfFiles] = {NULL};
    TGraphAsymmErrors*   graphCorrectedYieldPi0[MaxNumberOfFiles] = {NULL};
    TGraphAsymmErrors*   graphCorrectedYieldPi0Scaled[MaxNumberOfFiles] = {NULL};
    TGraphAsymmErrors*   graphCorrectedYieldPi0Ref[MaxNumberOfFiles] = {NULL};
    TGraphAsymmErrors*   graphCorrectedYieldPi0RefScaled[MaxNumberOfFiles] = {NULL};
    TGraphAsymmErrors*   graphInvCrossSectionPi0Ref[MaxNumberOfFiles] = {NULL};
    TGraphAsymmErrors*   graphInvCrossSectionPi0RefScaled[MaxNumberOfFiles] = {NULL};

    TH1D*   histoRawYieldsEta[MaxNumberOfFiles] = {NULL};
    TGraphAsymmErrors*   graphCorrectedYieldEta[MaxNumberOfFiles] = {NULL};
    TGraphAsymmErrors*   graphCorrectedYieldEtaScaled[MaxNumberOfFiles] = {NULL};
    TGraphAsymmErrors*   graphCorrectedYieldEtaRef[MaxNumberOfFiles] = {NULL};
    TGraphAsymmErrors*   graphCorrectedYieldEtaRefScaled[MaxNumberOfFiles] = {NULL};
    TGraphAsymmErrors*   graphInvCrossSectionEtaRef[MaxNumberOfFiles] = {NULL};
    TGraphAsymmErrors*   graphInvCrossSectionEtaRefScaled[MaxNumberOfFiles] = {NULL};

    for (Int_t i=0; i< nrOfTrigToBeComb; i++){
        fileInput[i]                                 = new TFile(inputFileName[i]);
        if (fileInput[i]->IsZombie()){
            cout << "pPb or PbPb file " << inputFileName[i].Data() << " not found!...returning!" << endl;
            return;
        }
        fileInputRef[i]                                 = new TFile(inputFileNameRef[i]);
        if (fileInputRef[i]->IsZombie()){
            cout << "pp file " << inputFileNameRef[i].Data() << " not found!...returning!" << endl;
            return;
        }
        gSystem->Exec(Form("cp %s %s/InputFile%d.root", inputFileName[i].Data(), outputDir.Data(), i));
        gSystem->Exec(Form("cp %s %s/InputFileRef%d.root", inputFileNameRef[i].Data(), outputDir.Data(), i));


        histoRawYieldsPi0[i] = (TH1D*)fileInput[i]->Get(Form("Pi0%s/RAWYieldPerEventsPi0_%s",inputFolderName[i].Data(),triggerName[i].Data()));
        graphCorrectedYieldPi0[i] = (TGraphAsymmErrors*)fileInput[i]->Get(Form("Pi0%s/CorrectedYieldPi0_%s",inputFolderName[i].Data(),triggerName[i].Data()));
        if(!graphCorrectedYieldPi0[i]){
            cout << "missing corrected yield input for pPb/PbPb: " << Form("Pi0%s/CorrectedYieldPi0_%s",inputFolderName[i].Data(),triggerName[i].Data()) << endl;
            return;
        } else {
            cout << "successfully loaded  pPb/PbPb input " << Form("Pi0%s/CorrectedYieldPi0_%s",inputFolderName[i].Data(),triggerName[i].Data()) << endl;
        }
        graphCorrectedYieldPi0[i]->SetName(Form("CorrectedYield_%s",triggerName[i].Data()));

        graphCorrectedYieldPi0Ref[i] = (TGraphAsymmErrors*)fileInputRef[i]->Get(Form("Pi0%s/CorrectedYieldPi0_%s",inputFolderNameRef[i].Data(),triggerNameRef[i].Data()));
        if(!graphCorrectedYieldPi0Ref[i]){
            cout << "missing corrected yield input for pp: " << Form("Pi0%s/CorrectedYieldPi0_%s",inputFolderNameRef[i].Data(),triggerNameRef[i].Data()) << endl;
            return;
        } else {
            cout << "successfully loaded pp input " << Form("Pi0%s/CorrectedYieldPi0_%s",inputFolderNameRef[i].Data(),triggerNameRef[i].Data()) << endl;
        }
        graphCorrectedYieldPi0Ref[i]->SetName(Form("CorrectedYieldRef_%s",triggerNameRef[i].Data()));

        if(enableEta){
            histoRawYieldsEta[i] = (TH1D*)fileInput[i]->Get(Form("Eta%s/RAWYieldPerEventsEta_%s",inputFolderName[i].Data(),triggerName[i].Data()));
            graphCorrectedYieldEta[i] = (TGraphAsymmErrors*)fileInput[i]->Get(Form("Eta%s/CorrectedYieldEta_%s",inputFolderName[i].Data(),triggerName[i].Data()));
            if(!graphCorrectedYieldEta[i]){
                cout << "missing corrected yield input for pPb/PbPb: " << Form("Eta%s/CorrectedYieldEta_%s",inputFolderName[i].Data(),triggerName[i].Data()) << endl;
                return;
            } else {
                cout << "successfully loaded  pPb/PbPb input " << Form("Eta%s/CorrectedYieldEta_%s",inputFolderName[i].Data(),triggerName[i].Data()) << endl;
            }
            graphCorrectedYieldEta[i]->SetName(Form("CorrectedYield_%s",triggerName[i].Data()));

            graphCorrectedYieldEtaRef[i] = (TGraphAsymmErrors*)fileInputRef[i]->Get(Form("Eta%s/CorrectedYieldEta_%s",inputFolderNameRef[i].Data(),triggerNameRef[i].Data()));
            if(!graphCorrectedYieldEtaRef[i]){
                cout << "missing corrected yield input for pp: " << Form("Eta%s/CorrectedYieldEta_%s",inputFolderNameRef[i].Data(),triggerNameRef[i].Data()) << endl;
                return;
            } else {
                cout << "successfully loaded pp input " << Form("Eta%s/CorrectedYieldEta_%s",inputFolderNameRef[i].Data(),triggerNameRef[i].Data()) << endl;
            }
            graphCorrectedYieldEtaRef[i]->SetName(Form("CorrectedYieldRef_%s",triggerNameRef[i].Data()));
        }
    }

    TFile*  fileBinShift = NULL;
    TFile*  fileBinShiftRef = NULL;
    TF1*   funcTCMFitShiftPi0 = NULL;
    TF1*   funcTCMFitShiftPi0Ref = NULL;
    TF1*   funcTCMFitShiftEta = NULL;
    TF1*   funcTCMFitShiftEtaRef = NULL;
    Bool_t doBinShift = kFALSE;
    if(fileInputBinShift.CompareTo("") && fileInputBinShiftRef.CompareTo("")){
        fileBinShift                                 = new TFile(fileInputBinShift);
        if (fileBinShift->IsZombie()){
            cout << "pPb or PbPb file for bin shift " << fileInputBinShift.Data() << " not found!...returning!" << endl;
            return;
        } else {
            if(optionEnergy.CompareTo("pPb_8TeV")==0){
                funcTCMFitShiftPi0 = (TF1*)fileBinShift->Get("Pi0pPb8TeV/TwoComponentModelFitPi0");
                funcTCMFitShiftEta = (TF1*)fileBinShift->Get("EtapPb8TeV/TwoComponentModelFitEta");
                cout << "loaded binshift fits for pPb part!" << endl;
            }
            if(!funcTCMFitShiftPi0){cout << "pPb shift fit not found" << endl; return;}
            if(!funcTCMFitShiftEta && mode!=10){cout << "pPb shift fit for Eta not found" << endl; return;}
        }
        fileBinShiftRef                                 = new TFile(fileInputBinShiftRef);
        if (fileBinShiftRef->IsZombie()){
            cout << "pp file for bin shift " << fileInputBinShiftRef.Data() << " not found!...returning!" << endl;
            return;
        } else {
            if(optionEnergy.CompareTo("pPb_8TeV")==0){
                funcTCMFitShiftPi0Ref = (TF1*)fileBinShiftRef->Get("Pi08TeV/TwoComponentModelFitPi0");
                funcTCMFitShiftEtaRef = (TF1*)fileBinShiftRef->Get("Eta8TeV/TwoComponentModelFitEta");
                cout << "loaded binshift fits for reference!" << endl;
            }
            if(!funcTCMFitShiftPi0Ref){cout << "pp shift fit not found" << endl; return;}
            if(!funcTCMFitShiftEtaRef && mode!=10){cout << "pp shift fit for Eta not found" << endl; return;}
        }
        doBinShift = kTRUE;
    }
    //***************************************************************************************************************
    //************************************Plotting invariant yield Pi0 *************************************
    //***************************************************************************************************************
    TCanvas* canvasCorrSpectra = new TCanvas("canvasCorrSpectra","",0,0,1000,1350);// gives the page size
    DrawGammaCanvasSettings( canvasCorrSpectra, 0.15, 0.017, 0.015, 0.07);
    canvasCorrSpectra->SetLogy();

    Double_t minCorrYieldSpectra   = 2e-10;
    Double_t maxCorrYieldSpectra   = 1e2;
    if(mode==10){
        minCorrYieldSpectra   = 2e-15;
        maxCorrYieldSpectra   = 1e-5;
    }

    TH2F * histo2DInvYieldSpectra = new TH2F("histo2DInvYieldSpectra","histo2DInvYieldSpectra",1000,0., maxPtGlobalPi0,10000,5e-15,5e15);
    SetStyleHistoTH2ForGraphs(histo2DInvYieldSpectra, "#it{p}_{T} (GeV/#it{c})","#frac{1}{2#pi #it{N}_{ev}} #frac{d^{2}#it{N}}{#it{p}_{T}d#it{p}_{T}d#it{y}} (#it{c}/GeV)^{2}",
                            0.85*textSizeSpectra,0.04, 0.85*textSizeSpectra,textSizeSpectra, 0.8,1.55);
    histo2DInvYieldSpectra->GetYaxis()->SetRangeUser(minCorrYieldSpectra,maxCorrYieldSpectra);
    histo2DInvYieldSpectra->DrawCopy();
    Double_t minXLegendRaw = 0.62;
    Int_t nColumnsRaw      = 1;
    Double_t maxYLegendRaw = 0.95;
    Double_t minYLegendRaw = 0.95-(nrOfTrigToBeComb/nColumnsRaw*0.75*textSizeSpectra);
    TLegend* legendSpectra = GetAndSetLegend2(minXLegendRaw, minYLegendRaw, 0.95, maxYLegendRaw,0.85*textSizePixelSpectra);
    legendSpectra->SetNColumns(nColumnsRaw);
    for (Int_t i = 0; i< nrOfTrigToBeComb; i++){
        DrawGammaSetMarkerTGraphAsym(graphCorrectedYieldPi0[i], markerTrigg[i], sizeTrigg[i], colorTrigg[i], colorTrigg[i]);
        graphCorrectedYieldPi0[i]->Draw("e1,p,same");
        DrawGammaSetMarkerTGraphAsym(graphCorrectedYieldPi0Ref[i], markerTriggMC[i], sizeTrigg[i], colorTriggShade[i], colorTriggShade[i]);
        graphCorrectedYieldPi0Ref[i]->Draw("e1,p,same");
        legendSpectra->AddEntry(graphCorrectedYieldPi0[i],triggerNameLabel[i].Data(),"p");
    }
    legendSpectra->Draw();

    if(doBinShift){
        DrawGammaSetMarkerTF1(funcTCMFitShiftPi0Ref, 7, 2, kBlue+2);
        funcTCMFitShiftPi0Ref->SetRange(0.3,250);
        funcTCMFitShiftPi0Ref->Draw("same");
    }

    TLatex *labelEnergySpectra = new TLatex(0.2, 0.12+(2*textSizeSpectra*0.85*0.75),collisionSystem.Data());
    SetStyleTLatex( labelEnergySpectra, 0.85*textSizeSpectra,4);
    labelEnergySpectra->Draw();

    TLatex *labelPi0Spectra = new TLatex(0.2, 0.12+textSizeSpectra*0.85*0.75,"#pi^{0} #rightarrow #gamma#gamma");
    SetStyleTLatex( labelPi0Spectra, 0.85*textSizeSpectra,4);
    labelPi0Spectra->Draw();

    TLatex *labelDetProcSpectra = new TLatex(0.2, 0.12,detectionProcess.Data());
    SetStyleTLatex( labelDetProcSpectra, 0.85*textSizeSpectra,4);
    labelDetProcSpectra->Draw();

    canvasCorrSpectra->Update();
    canvasCorrSpectra->SaveAs(Form("%s/Pi0_%s_CorrectedYieldTrigg.%s",outputDir.Data(),isMC.Data(),suffix.Data()));


    //****************************************************************************************************************
    //************* Processing of each individual trigger, reducing ranges & adding systematics **********************
    //****************************************************************************************************************
    Double_t ptSysRelPi0                                    [MaxNumberOfFiles][400];
    Double_t yErrorSysLowRelPi0                             [MaxNumberOfFiles][400];
    Double_t yErrorSysHighRelPi0                            [MaxNumberOfFiles][400];
    Bool_t sysAvailPi0                                      [MaxNumberOfFiles];

    Bool_t sysAvailSinglePi0                                [MaxNumberOfFiles];
    Int_t numberBinsSysAvailSinglePi0                       [MaxNumberOfFiles];

    TH1D* histoNuclearModFactorPi0                          [MaxNumberOfFiles];
    TGraphAsymmErrors* graphsNuclearModFactorPi0            [MaxNumberOfFiles];
    TGraphAsymmErrors* graphsNuclearModFactorSysPi0         [MaxNumberOfFiles];
    TH1D* histoNuclearModFactorPi0Shrunk                          [MaxNumberOfFiles];
    TGraphAsymmErrors* graphsNuclearModFactorPi0Shrunk            [MaxNumberOfFiles];
    TGraphAsymmErrors* graphsNuclearModFactorSysPi0Shrunk         [MaxNumberOfFiles];

    vector<TString>** ptSysDetail     = new vector<TString>*[MaxNumberOfFiles];
    for(Int_t iR=0; iR<nrOfTrigToBeComb; iR++) ptSysDetail[iR] = new vector<TString>[400];

    // definition of predefined arrays for trigger correlation filling
    TH1D*               histoStatPi0    [MaxNumberOfFiles];
    TGraphAsymmErrors*  graphSystPi0    [MaxNumberOfFiles];
    TH1D*               histoRelStatPi0 [MaxNumberOfFiles];
    TGraphAsymmErrors*  graphRelStatPi0 [MaxNumberOfFiles];
    TGraphAsymmErrors*  graphRelSystPi0 [MaxNumberOfFiles];

    Int_t offSetsPi0[MaxNumberOfFiles];
        for(Int_t set=0;set<MaxNumberOfFiles;set++) offSetsPi0[set]= 0;
    Int_t offSetsPi0Sys[MaxNumberOfFiles];
        for(Int_t set=0;set<MaxNumberOfFiles;set++) offSetsPi0Sys[set]= 0;
    if(optionEnergy.CompareTo("pPb_8TeV")==0){
        if(mode == 2){
            offSetsPi0[1] = 3; //INT7
            offSetsPi0[4] = 0; //EG2
            offSetsPi0[5] = 2; //EG1
        }else if(mode == 4){
            offSetsPi0[1] = 0; //INT7
            offSetsPi0[4] = 0; //EG2
            offSetsPi0[5] = 4; //EG1
        }
    } else if(optionEnergy.CompareTo("pPb_5.023TeV")==0){
        if(mode == 2){
            offSetsPi0[1] = 0; //INT7
            offSetsPi0[4] = 30; //EG2
            offSetsPi0[5] = 38; //EG1
        }else if(mode == 4){
            offSetsPi0[1] = 0; //INT7
            if (fCentOutput.Contains("00100") ){
                offSetsPi0[4] = 31; //EG2
                offSetsPi0[5] = 34; //EG1
            } else {
                offSetsPi0[4] = 22; //EG2
                offSetsPi0[5] = 24; //EG1            
            }
        }
    }
    
    for (Int_t i = 0; i< nrOfTrigToBeComb; i++){
        // read systematics, if fileName is set to "bla" no action has to be performed and systematics will be disabled for the rest of the analysis
        if (sysFilePi0[i].CompareTo("bla") != 0){
            sysAvailPi0[i]                = kTRUE;
            ifstream  fileSysErrPi0;
            fileSysErrPi0.open(sysFilePi0[i].Data(),ios_base::in);
            cout << sysFilePi0[i].Data() << endl;
            gSystem->Exec(Form("cp %s %s/SystematicErrorAveraged_Pi0_%s.txt", sysFilePi0[i].Data(), outputDir.Data(),triggerName[i].Data()));


            Int_t iPtBin = 0;
            cout << "reading sys file summed" << endl;
            while(!fileSysErrPi0.eof() && iPtBin < 400){
                Double_t garbage = 0;
                fileSysErrPi0 >>ptSysRelPi0[i][iPtBin] >> yErrorSysLowRelPi0[i][iPtBin] >> yErrorSysHighRelPi0[i][iPtBin]>>    garbage >> garbage;
                cout << iPtBin << "\t"<< ptSysRelPi0[i][iPtBin]<< "\t"  << yErrorSysLowRelPi0[i][iPtBin] << "\t"  <<yErrorSysHighRelPi0[i][iPtBin] << "\t"  << endl;;
                iPtBin++;
            }
            fileSysErrPi0.close();
         // read in detailed systematics
            string sysFilePi0Det = sysFilePi0[i].Data();

            if(!replace(sysFilePi0Det, "Averaged", "AveragedSingle")){
                cout << "WARNING: could not find detailed systematics file " << sysFilePi0Det << ", skipping... " << endl;
                sysAvailSinglePi0[i] = kFALSE;
            }else{
                ifstream fileSysErrDetailedPi0;
                fileSysErrDetailedPi0.open(sysFilePi0Det,ios_base::in);
                if(fileSysErrDetailedPi0.is_open()) {
                    sysAvailSinglePi0[i] = kTRUE;
                    gSystem->Exec(Form("cp %s %s/SystematicErrorAveragedSingle_Pi0_%s.txt", ((TString)sysFilePi0Det).Data(), outputDir.Data(),triggerName[i].Data()));
                } else{
                    sysAvailSinglePi0[i] = kFALSE;
                    cout << "No single errors were found" << endl;
                }

                if (sysAvailSinglePi0[i]){
                    cout << sysFilePi0Det << endl;
                    iPtBin = 0;
                    string line;
                    Int_t iPtBinColumn = 0;
                    while (getline(fileSysErrDetailedPi0, line) && iPtBin < 400) {
                        istringstream ss(line);
                        TString temp="";
                        iPtBinColumn = 0;
                        while(ss && iPtBinColumn < 400){
                            ss >> temp;
                            if( !(iPtBin==0 && temp.CompareTo("bin")==0) && !temp.IsNull()){
                                ptSysDetail[i][iPtBin].push_back(temp);
                                iPtBinColumn++;
                            }
                        }
                        if(iPtBin == 0){
                            ptSysDetail[i][iPtBin++].push_back("TotalError");
                            iPtBinColumn++;
                        }else iPtBin++;
                    }
                    numberBinsSysAvailSinglePi0[i] = iPtBin;
                    fileSysErrDetailedPi0.close();
                 }
             }
        } else {
            sysAvailPi0[i]             = kFALSE;
            sysAvailSinglePi0[i]       = kFALSE;
        }
        cout << sysAvailPi0[i] << "\t" << sysAvailSinglePi0[i] << endl;

        // calculate RpA by dividing spectra and using nuclear overlap function
        // scale pA or AA measurement to NSD (if needed)
        if(doBinShift){
            for (Int_t i = 0; i< nrOfTrigToBeComb; i++){
                if(graphCorrectedYieldPi0[i])graphCorrectedYieldPi0[i]     = ApplyYshiftIndividualSpectra( graphCorrectedYieldPi0[i], funcTCMFitShiftPi0);
            }
        }
        graphCorrectedYieldPi0Scaled[i] = (TGraphAsymmErrors*) graphCorrectedYieldPi0[i]->Clone(Form("graphCorrectedYieldPi0Scaled%d",i));
        if(optionEnergy.Contains("pPb_8TeV")){ // calculate RpA via cross section
            graphCorrectedYieldPi0Scaled[i] = ScaleGraphAsym(graphCorrectedYieldPi0Scaled[i], xsection*recalcBarn);
        } else { // calculate RpA via nuclear overlap function
            graphCorrectedYieldPi0Scaled[i] = ScaleGraphAsym(graphCorrectedYieldPi0Scaled[i], scalingToNSD); // std
        }
        cout << "pPb spectrum for trigger " << triggerName[i].Data() << endl;
        graphCorrectedYieldPi0Scaled[i]->Print();

        graphCorrectedYieldPi0RefScaled[i] = (TGraphAsymmErrors*) graphCorrectedYieldPi0Ref[i]->Clone(Form("graphCorrectedYieldPi0RefScaled%d",i));
        graphCorrectedYieldPi0RefScaled[i] = ScaleGraphAsym(graphCorrectedYieldPi0RefScaled[i], xsectionReference*recalcBarn);
        if(doBinShift){
            for (Int_t i = 0; i< nrOfTrigToBeComb; i++){
                if(graphCorrectedYieldPi0RefScaled[i])graphCorrectedYieldPi0RefScaled[i]     = ApplyYshiftIndividualSpectra( graphCorrectedYieldPi0RefScaled[i], funcTCMFitShiftPi0Ref);
            }
        }
        if(optionEnergy.Contains("pPb_8TeV")){ // calculate RpA via cross section
            graphCorrectedYieldPi0RefScaled[i] = ScaleGraphAsym(graphCorrectedYieldPi0RefScaled[i], 208);
        } else { // calculate RpA via nuclear overlap function
            graphCorrectedYieldPi0RefScaled[i] = ScaleGraphAsym(graphCorrectedYieldPi0RefScaled[i], nuclOverlap); // std
        }
        if(optionEnergy.Contains("pPb_8TeV")){
            TF1* centerOfMassScalingFunc8000to8160GeV = new TF1("centerOfMassScalingFunc8000to8160GeV","1 / (1.026252 + 0.001554 * x)",0,200);
            graphCorrectedYieldPi0RefScaled[i]   = CalculateGraphErrRatioToFit(graphCorrectedYieldPi0RefScaled[i], centerOfMassScalingFunc8000to8160GeV);
        }
        cout << "pp reference spectrum for trigger " << triggerName[i].Data() << endl;
        graphCorrectedYieldPi0RefScaled[i]->Print();

        graphsNuclearModFactorPi0[i] = CalculateAsymGraphRatioToGraph(graphCorrectedYieldPi0Scaled[i],graphCorrectedYieldPi0RefScaled[i]);
        cout << "nuclear modification factor for trigger " << triggerName[i].Data() << endl;
        graphsNuclearModFactorPi0[i]->Print();
        graphsNuclearModFactorSysPi0[i] = (TGraphAsymmErrors*) graphsNuclearModFactorPi0[i]->Clone(Form("graphsNuclearModFactorSysPi0%s",triggerName[i].Data()));
        // if ( maskedFullyPi0[i] ){
        //     graphsNuclearModFactorPi0[i] = NULL;
        //     graphsNuclearModFactorSysPi0[i] = NULL;
        // }


        // put systematics on graphs
        if (graphsNuclearModFactorSysPi0[i]){
            // if (sysAvailSinglePi0[i]){
            //     nRelSysErrPi0Sources                    = (Int_t)ptSysDetail[i][0].size()-1;
            //     for (Int_t k = 0; k < nRelSysErrPi0Sources; k++ ){
            //         graphRelSysErrPi0Source[k][i]       = (TGraphAsymmErrors*) graphsCorrectedYieldSysRemoved0Pi0[i]->Clone(Form("RelSysErrPi0Source%s_%s",((TString)ptSysDetail[i][0].at(k+1)).Data(), triggerName[i].Data()));
            //         cout << Form("RelSysErrSource%s_%s",((TString)ptSysDetail[i][0].at(k+1)).Data(), triggerName[i].Data()) << endl;
            //     }
            // }
            for (Int_t j = 0; j< graphsNuclearModFactorSysPi0[i]->GetN(); j++){
                if (sysAvailPi0[i]){
                    Int_t counter = 0;
                    while(counter < 400 && TMath::Abs(graphsNuclearModFactorSysPi0[i]->GetX()[j] - ptSysRelPi0[i][counter])> 0.001) counter++;
                    if (counter < 400){
                        cout << ptSysRelPi0[i][counter]<< "\t found it" << endl;
                        Double_t yErrorSysLowDummy  = TMath::Abs(yErrorSysLowRelPi0[i][counter]/100*graphsNuclearModFactorSysPi0[i]->GetY()[j]);
                        Double_t yErrorSysHighDummy = yErrorSysHighRelPi0[i][counter]/100*graphsNuclearModFactorSysPi0[i]->GetY()[j];
                        graphsNuclearModFactorSysPi0[i]->SetPointEYlow(j,yErrorSysLowDummy);
                        graphsNuclearModFactorSysPi0[i]->SetPointEYhigh(j,yErrorSysHighDummy);
                        // if (sysAvailSinglePi0[i]){
                        //     for (Int_t k = 0; k < nRelSysErrPi0Sources; k++ ){
                        //         graphRelSysErrPi0Source[k][i]->SetPoint(j, graphsNuclearModFactorSysPi0[i]->GetX()[j] ,((TString)ptSysDetail[i][counter+1].at(k+1)).Atof());
                        //         graphRelSysErrPi0Source[k][i]->SetPointEYhigh(j,0);
                        //         graphRelSysErrPi0Source[k][i]->SetPointEYlow(j,0);
                        //     }
                        // }
                    } else {
                        graphsNuclearModFactorSysPi0[i]->SetPointEYlow(j,0);
                        graphsNuclearModFactorSysPi0[i]->SetPointEYhigh(j,0);
                        // if (sysAvailSinglePi0[i]){
                        //     for (Int_t k = 0; k < nRelSysErrPi0Sources; k++ ){
                        //         graphRelSysErrPi0Source[k][i]->SetPoint(j, graphsNuclearModFactorSysPi0[i]->GetX()[j] ,0);
                        //         graphRelSysErrPi0Source[k][i]->SetPointEYlow(j,0);
                        //         graphRelSysErrPi0Source[k][i]->SetPointEYhigh(j,0);
                        //     }
                        // }
                    }
                } else {
                    graphsNuclearModFactorSysPi0[i]->SetPointEYlow(j,0);
                    graphsNuclearModFactorSysPi0[i]->SetPointEYhigh(j,0);
                    averagedPi0 = kFALSE;
                    // if (sysAvailSinglePi0[i]){
                    //     for (Int_t k = 0; k < nRelSysErrPi0Sources; k++ ){
                    //         graphRelSysErrPi0Source[k][i]->SetPoint(j, graphsNuclearModFactorSysPi0[i]->GetX()[j] ,0);
                    //         graphRelSysErrPi0Source[k][i]->SetPointEYlow(j,0);
                    //         graphRelSysErrPi0Source[k][i]->SetPointEYhigh(j,0);
                    //     }
                    // }
                }
            }
        }

        cout << "step 3" << endl;
        cout << "range to be accepted: " << ptFromSpecPi0[i][0] << "\t-\t" << ptFromSpecPi0[i][1] << endl;
        if(!maskedFullyPi0[i]){
            graphsNuclearModFactorPi0Shrunk[i] = (TGraphAsymmErrors*)graphsNuclearModFactorPi0[i]->Clone(Form("graphsNuclearModFactorPi0Shrunk%s",triggerName[i].Data()));
            graphsNuclearModFactorSysPi0Shrunk[i] = (TGraphAsymmErrors*)graphsNuclearModFactorSysPi0[i]->Clone(Form("graphsNuclearModFactorSysPi0Shrunk%s",triggerName[i].Data()));

            // remove points at beginning according to ranges set for individual triggers
            while(graphsNuclearModFactorPi0Shrunk[i]->GetX()[0] < ptFromSpecPi0[i][0])
                graphsNuclearModFactorPi0Shrunk[i]->RemovePoint(0);
            while(graphsNuclearModFactorSysPi0Shrunk[i]->GetX()[0] < ptFromSpecPi0[i][0])
                graphsNuclearModFactorSysPi0Shrunk[i]->RemovePoint(0);
            // remove points at the end according to ranges set for individual triggers
            while (graphsNuclearModFactorPi0Shrunk[i]->GetX()[graphsNuclearModFactorPi0Shrunk[i]->GetN()-1] > ptFromSpecPi0[i][1])
                graphsNuclearModFactorPi0Shrunk[i]->RemovePoint(graphsNuclearModFactorPi0Shrunk[i]->GetN()-1);
            while (graphsNuclearModFactorSysPi0Shrunk[i]->GetX()[graphsNuclearModFactorSysPi0Shrunk[i]->GetN()-1] > ptFromSpecPi0[i][1])
                graphsNuclearModFactorSysPi0Shrunk[i]->RemovePoint(graphsNuclearModFactorSysPi0Shrunk[i]->GetN()-1);
        }

        // }
        histoNuclearModFactorPi0[i] = (TH1D*)histoRawYieldsPi0[i]->Clone(Form("histoNuclearModFactorPi0%s",triggerName[i].Data()));
        histoNuclearModFactorPi0Shrunk[i] = (TH1D*)histoRawYieldsPi0[i]->Clone(Form("histoNuclearModFactorPi0Shrunk%s",triggerName[i].Data()));
        for (Int_t j = 0; j< histoNuclearModFactorPi0[i]->GetNbinsX()+1; j++){
            histoNuclearModFactorPi0[i]->SetBinContent(j,0);
            histoNuclearModFactorPi0[i]->SetBinError(j,0);
            histoNuclearModFactorPi0Shrunk[i]->SetBinContent(j,0);
            histoNuclearModFactorPi0Shrunk[i]->SetBinError(j,0);
        }
        for (Int_t j = 0; j < graphsNuclearModFactorPi0[i]->GetN(); j++){
            Int_t bin = histoNuclearModFactorPi0[i]->GetXaxis()->FindBin(graphsNuclearModFactorPi0[i]->GetX()[j]);
            histoNuclearModFactorPi0[i]->SetBinContent(bin,graphsNuclearModFactorPi0[i]->GetY()[j]);
            histoNuclearModFactorPi0[i]->SetBinError(bin,graphsNuclearModFactorPi0[i]->GetEYlow()[j]);
        }
        if(!maskedFullyPi0[i]){
            for (Int_t j = 0; j < graphsNuclearModFactorPi0Shrunk[i]->GetN(); j++){
                Int_t bin = histoNuclearModFactorPi0Shrunk[i]->GetXaxis()->FindBin(graphsNuclearModFactorPi0Shrunk[i]->GetX()[j]);
                histoNuclearModFactorPi0Shrunk[i]->SetBinContent(bin,graphsNuclearModFactorPi0Shrunk[i]->GetY()[j]);
                histoNuclearModFactorPi0Shrunk[i]->SetBinError(bin,graphsNuclearModFactorPi0Shrunk[i]->GetEYlow()[j]);
            }
            cout << "histo NuclModFac shrunk for trigg " << triggerName[i].Data() << endl;
            histoNuclearModFactorPi0Shrunk[i]->Print("all");
        }
        // return;
        // Set correct trigger order for combination function
        Int_t nCorrOrder    = GetOrderedTrigger(triggerName[i]);
        if (nCorrOrder == -1){
            cout << "ERROR: trigger name not defined" << endl;
            return;
        }

        if ( graphsNuclearModFactorSysPi0Shrunk[i] && !maskedFullyPi0[i]){
            histoStatPi0[nCorrOrder]    = histoNuclearModFactorPi0Shrunk[i];
            graphSystPi0[nCorrOrder]    = graphsNuclearModFactorSysPi0Shrunk[i];
            offSetsPi0Sys[nCorrOrder]   = histoStatPi0[nCorrOrder]->GetXaxis()->FindBin(graphSystPi0[nCorrOrder]->GetX()[0])-1;
        }
    }
    // set correct systematics offsets
    if (optionEnergy.Contains("pPb_8TeV")){
        if (mode == 2){
            offSetsPi0Sys[1]+=3;
            offSetsPi0Sys[5]+=2;
        } else if (mode == 4) {
            offSetsPi0Sys[5]+=4;
        }
    } else if (optionEnergy.Contains("pPb_5.023TeV")){
        if (mode == 4) {
            offSetsPi0Sys[4]+=offSetsPi0[4];
            offSetsPi0Sys[5]+=offSetsPi0[5];
        }
    }

    Double_t minpTscaledyield = 0;
    Double_t maxpTscaledyield = maxPtGlobalPi0;
    Double_t minscaledyield = 1;
    Double_t maxscaledyield = 5e14;
    if(mode==10){
        minpTscaledyield = 8;
        maxpTscaledyield = 230;
        minscaledyield = 1e-1;
        maxscaledyield = 5e7;
        canvasCorrSpectra->SetLogx(kTRUE);
    }
    TH2F * histo2DInvXSecSpectra = new TH2F("histo2DInvXSecSpectra","histo2DInvXSecSpectra",1000,minpTscaledyield, maxPtGlobalPi0,10000,minscaledyield,maxscaledyield);
    SetStyleHistoTH2ForGraphs(histo2DInvXSecSpectra, "#it{p}_{T} (GeV/#it{c})","#frac{1}{2#pi #it{N}_{ev}} #frac{d^{2}#it{N}}{#it{p}_{T}d#it{p}_{T}d#it{y}} (#it{c}/GeV)^{2}",
                            0.85*textSizeSpectra,0.04, 0.85*textSizeSpectra,textSizeSpectra, 0.8,1.55);
    // histo2DInvXSecSpectra->GetYaxis()->SetRangeUser(minCorrYieldSpectra,maxCorrYieldSpectra);
    histo2DInvXSecSpectra->DrawCopy();
        histo2DInvXSecSpectra->DrawCopy();
        legendSpectra = GetAndSetLegend2(minXLegendRaw, minYLegendRaw, 0.95, maxYLegendRaw,0.85*textSizePixelSpectra);
        legendSpectra->SetNColumns(nColumnsRaw);
        for (Int_t i = 0; i< nrOfTrigToBeComb; i++){
            DrawGammaSetMarkerTGraphAsym(graphCorrectedYieldPi0Scaled[i], markerTrigg[i], sizeTrigg[i], colorTrigg[i], colorTrigg[i]);
            graphCorrectedYieldPi0Scaled[i]->Draw("e1,p,same");
            DrawGammaSetMarkerTGraphAsym(graphCorrectedYieldPi0RefScaled[i], markerTriggMC[i], sizeTrigg[i], colorTriggShade[i], colorTriggShade[i]);
            graphCorrectedYieldPi0RefScaled[i]->Draw("e1,p,same");
            legendSpectra->AddEntry(graphCorrectedYieldPi0Scaled[i],triggerNameLabel[i].Data(),"p");
            cout << "scaled yield for pPb trigger " << triggerNameLabel[i].Data() << endl;
            graphCorrectedYieldPi0Scaled[i]->Print();
            cout << "scaled yield for pp trigger " << triggerNameLabel[i].Data() << endl;
            graphCorrectedYieldPi0RefScaled[i]->Print();
        }
        legendSpectra->Draw();

        labelEnergySpectra->Draw();
        labelPi0Spectra->Draw();
        labelDetProcSpectra->Draw();

        canvasCorrSpectra->Update();
        canvasCorrSpectra->SaveAs(Form("%s/Pi0_%s_CorrectedYieldScaledTrigg.%s",outputDir.Data(),isMC.Data(),suffix.Data()));
        canvasCorrSpectra->SetLogx(kFALSE);

    // create weighted graphs for spectra and supporting graphs
    TString nameWeightsLogFilePi0 =     Form("%s/weightsPi0_%s.dat",outputDir.Data(),isMC.Data());
    TGraphAsymmErrors* graphNuclModFactorWeightedAveragePi0Stat    = NULL;
    TGraphAsymmErrors* graphNuclModFactorWeightedAveragePi0Sys     = NULL;
    TGraphAsymmErrors* graphNuclModFactorWeightedAveragePi0Tot     = NULL;

    // Calculate averaged nuclear modification factor spectrum & respective supporting graphs according to statistical and systematic errors taking correctly into account the cross correlations
    if (averagedPi0){
        cout << maxNAllowedPi0 << endl;
        // Calculate average pi0 spectrum
        graphNuclModFactorWeightedAveragePi0Tot = CombinePtPointsSpectraTriggerCorrMat(
            histoStatPi0, graphSystPi0,
            binningPi0,  maxNAllowedPi0,
            offSetsPi0 ,offSetsPi0Sys,
            graphNuclModFactorWeightedAveragePi0Stat, graphNuclModFactorWeightedAveragePi0Sys,
            nameWeightsLogFilePi0.Data(),
            mode, optionEnergy, "Pi0Ratio", kTRUE,
            fileInputCorrFactors
        );
    }
    graphNuclModFactorWeightedAveragePi0Tot->Print();
    // return;
    while (graphNuclModFactorWeightedAveragePi0Tot->GetY()[0] <= 0.0000001) graphNuclModFactorWeightedAveragePi0Tot->RemovePoint(0);
    while (graphNuclModFactorWeightedAveragePi0Stat->GetY()[0] <= 0.0000001) graphNuclModFactorWeightedAveragePi0Stat->RemovePoint(0);
    while (graphNuclModFactorWeightedAveragePi0Sys->GetY()[0] <= 0.0000001) graphNuclModFactorWeightedAveragePi0Sys->RemovePoint(0);
    
    
    if (averagedPi0){
             // preparations for weight readout
        Double_t xValuesReadPi0[400];
        Double_t weightsReadPi0[MaxNumberOfFiles][400];
        Int_t availableMeasPi0[MaxNumberOfFiles];
            for(Int_t set=0;set<MaxNumberOfFiles;set++) availableMeasPi0[set]= -1;
        Int_t nMeasSetPi0               = nrOfTrigToBeCombPi0Red;
        Int_t nPtBinsReadPi0            = 0;

        // labeling and plotting settings
        Color_t colorTriggWeighted[MaxNumberOfFiles];
            for(Int_t set=0;set<MaxNumberOfFiles;set++) colorTriggWeighted[set]= GetDefaultTriggerColorName(nameTriggerWeighted[set], 0);
        Marker_t markerTriggWeighted[MaxNumberOfFiles];
            for(Int_t set=0;set<MaxNumberOfFiles;set++) markerTriggWeighted[set]= GetDefaultTriggerMarkerStyleName(nameTriggerWeighted[set], 0);

        // Reading weights from output file for plotting
        ifstream fileWeightsPi0;
        fileWeightsPi0.open(nameWeightsLogFilePi0,ios_base::in);
        cout << "reading" << nameWeightsLogFilePi0 << endl;

        while(!fileWeightsPi0.eof() && nPtBinsReadPi0 < 400){
            TString garbage = "";
            if (nPtBinsReadPi0 == 0){
                fileWeightsPi0 >> garbage ;//>> availableMeas[0] >> availableMeas[1] >> availableMeas[2] >> availableMeas[3];
                for (Int_t i = 0; i < nMeasSetPi0; i++){
                    fileWeightsPi0 >> availableMeasPi0[i] ;
                }
                cout << "read following measurements: ";
                for (Int_t i = 0; i < nMeasSetPi0; i++){
                    cout << availableMeasPi0[i] << "\t" ;
                }
                cout << endl;
            } else {
                fileWeightsPi0 >> xValuesReadPi0[nPtBinsReadPi0-1];
                for (Int_t i = 0; i < nMeasSetPi0; i++){
                    fileWeightsPi0 >> weightsReadPi0[availableMeasPi0[i]][nPtBinsReadPi0-1] ;
                }
                cout << "read: "<<  nPtBinsReadPi0 << "\t"<< xValuesReadPi0[nPtBinsReadPi0-1] << "\t" ;
                for (Int_t i = 0; i < nMeasSetPi0; i++){
                    cout << weightsReadPi0[availableMeasPi0[i]][nPtBinsReadPi0-1] << "\t";
                }
                cout << endl;
            }
            nPtBinsReadPi0++;
        }
        nPtBinsReadPi0 = nPtBinsReadPi0-2 ;
        fileWeightsPi0.close();

        // creating & filling the weight graphs
        TGraph* graphWeightsPi0[MaxNumberOfFiles];
        for (Int_t i = 0; i < MaxNumberOfFiles; i++){
            graphWeightsPi0[i]                    = NULL;
        }
        for (Int_t i = 0; i < nMeasSetPi0; i++){
            cout << i << "\t" << availableMeasPi0[i] << endl;
            graphWeightsPi0[availableMeasPi0[i]]  = new TGraph(nPtBinsReadPi0,xValuesReadPi0,weightsReadPi0[availableMeasPi0[i]]);
            Int_t bin = 0;
            for (Int_t n = 0; n< nPtBinsReadPi0; n++){
                if (graphWeightsPi0[availableMeasPi0[i]]->GetY()[bin] == 0) graphWeightsPi0[availableMeasPi0[i]]->RemovePoint(bin);
                else bin++;
            }
            graphWeightsPi0[availableMeasPi0[i]]->Print();
        }


        //  **********************************************************************************************************************
        //  ******************************************* Plotting weights Pi0 *****************************************************
        //  **********************************************************************************************************************
        Int_t textSizeLabelsPixel = 900*0.04;

        TCanvas* canvasWeights = new TCanvas("canvasWeights","",200,10,1350,900);// gives the page size
        DrawGammaCanvasSettings( canvasWeights, 0.08, 0.02, 0.035, 0.09);

        TH2F * histo2DWeights;
        histo2DWeights = new TH2F("histo2DWeights","histo2DWeights",11000,0.,maxPtGlobalPi0,1000,-0.5,1.1);
        SetStyleHistoTH2ForGraphs(histo2DWeights, "#it{p}_{T} (GeV/#it{c})","#omega_{a} for BLUE",0.035,0.04, 0.035,0.04, 1.,1.);
        histo2DWeights->Draw("copy");

            TLegend* legendWeightsPi0 = GetAndSetLegend2(0.12, 0.14, 0.55, 0.14+(0.035*nMeasSetPi0/2*1.35), 32);
            legendWeightsPi0->SetNColumns(2);
            for (Int_t i = 0; i < nMeasSetPi0; i++){
                DrawGammaSetMarkerTGraph(graphWeightsPi0[availableMeasPi0[i]],markerTriggWeighted[availableMeasPi0[i]], 1.5*sizeTrigg[availableMeasPi0[i]],
                                         colorTriggWeighted[availableMeasPi0[i]], colorTriggWeighted[availableMeasPi0[i]]);
                graphWeightsPi0[availableMeasPi0[i]]->Draw("p,same,e1");
                legendWeightsPi0->AddEntry(graphWeightsPi0[availableMeasPi0[i]],nameTriggerWeighted[availableMeasPi0[i]],"p");
            }
            legendWeightsPi0->Draw();

            TLatex *labelWeightsEnergy = new TLatex(0.95,0.24,collisionSystem.Data());
            SetStyleTLatex( labelWeightsEnergy, 0.85*textSizeLabelsPixel,4,1,42,kTRUE, 31);
            labelWeightsEnergy->SetTextFont(43);
            labelWeightsEnergy->Draw();
            TLatex *labelWeightsPi0 = new TLatex(0.95,0.20,"#pi^{0} #it{R}_{pA}");
            SetStyleTLatex( labelWeightsPi0, 0.85*textSizeLabelsPixel,4,1,42,kTRUE, 31);
            labelWeightsPi0->SetTextFont(43);
            labelWeightsPi0->Draw();
            TLatex *labelDetProcWeights    = new TLatex(0.95, 0.16,detectionProcess.Data());
            SetStyleTLatex( labelDetProcWeights, 0.85*textSizeLabelsPixel,4,1,42,kTRUE, 31);
            labelDetProcWeights->SetTextFont(43);
            labelDetProcWeights->Draw();


    //      DrawGammaLines(0.23, 70. , 0.8, 0.8,1, kGray, 3);
            DrawGammaLines(0.23, 70. , 0.5, 0.5,1, kGray, 7);
            DrawGammaLines(0.23, 70. , 0.4, 0.4,1, kGray, 1);
            DrawGammaLines(0.23, 70. , 0.3, 0.3,1, kGray, 7);
            DrawGammaLines(0.23, 70. , 0.2, 0.2,1, kGray, 3);

        canvasWeights->SaveAs(Form("%s/%s_WeightsPi0RpATriggers.%s",outputDir.Data(), isMC.Data(), suffix.Data()));
        delete canvasWeights;

        // Calculating relative error for pi0
        for (Int_t i = 0; i < MaxNumberOfFiles; i++){
            if (histoStatPi0[i]){
                histoRelStatPi0[i]      = CalculateRelErrUpTH1D( histoStatPi0[i], Form("relativeStatErrorPi0_%s", nameTriggerWeighted[i].Data()));
                graphRelStatPi0[i]      = new TGraphAsymmErrors(histoRelStatPi0[i]);
                while (graphRelStatPi0[i]->GetY()[0] < 0  || graphRelStatPi0[i]->GetY()[0] == 0) graphRelStatPi0[i]->RemovePoint(0);
                graphRelStatPi0[i]->Print();
            }
            if (graphSystPi0[i])
                graphRelSystPi0[i]      = CalculateRelErrUpAsymmGraph( graphSystPi0[i], Form("relativeSysErrorPi0_%s", nameTriggerWeighted[i].Data()));
        }

        TGraphAsymmErrors* graphRelErrorPi0Tot        = CalculateRelErrUpAsymmGraph( graphNuclModFactorWeightedAveragePi0Tot, "relativeTotalErrorPi0");
        while (graphRelErrorPi0Tot->GetY()[0] < 0  || graphRelErrorPi0Tot->GetY()[0] == 0) graphRelErrorPi0Tot->RemovePoint(0);

        TGraphAsymmErrors* graphRelErrorPi0Stat       = CalculateRelErrUpAsymmGraph( graphNuclModFactorWeightedAveragePi0Stat, "relativeStatErrorPi0");
        while (graphRelErrorPi0Stat->GetY()[0] < 0 || graphRelErrorPi0Stat->GetY()[0] == 0) graphRelErrorPi0Stat->RemovePoint(0);

        TGraphAsymmErrors* graphRelErrorPi0Sys        = CalculateRelErrUpAsymmGraph( graphNuclModFactorWeightedAveragePi0Sys, "relativeSysErrorPi0");
        while (graphRelErrorPi0Sys->GetY()[0] < 0 || graphRelErrorPi0Sys->GetY()[0] == 0 ) graphRelErrorPi0Sys->RemovePoint(0);

        const char *SysErrDatnameMeanSingleErrCheck = Form("%s/SystematicErrorAveragedSingle%s_Pi0RpA_%s_Check.dat",outputDir.Data(),sysStringComb.Data(),optionEnergy.Data());
        fstream SysErrDatAverSingleCheck;
        SysErrDatAverSingleCheck.precision(4);
        cout << SysErrDatnameMeanSingleErrCheck << endl;
        if(sysAvailSinglePi0[0]){
          SysErrDatAverSingleCheck.open(SysErrDatnameMeanSingleErrCheck, ios::out);
          SysErrDatAverSingleCheck << "pt \t Stat err \t sys err \t tot err " << endl;
          for (Int_t i = 0; i < graphRelErrorPi0Tot->GetN(); i++){
              if (graphRelErrorPi0Stat->GetY()[i] > 0) SysErrDatAverSingleCheck << graphRelErrorPi0Stat->GetX()[i] << "\t" << graphRelErrorPi0Stat->GetY()[i] <<"\t" << graphRelErrorPi0Sys->GetY()[i] <<  "\t" << graphRelErrorPi0Tot->GetY()[i] << endl;
          }
          SysErrDatAverSingleCheck << endl;
          SysErrDatAverSingleCheck.close();
        }

        // plot sys relative errors for individual triggers
        TCanvas* canvasRelSysErr            = new TCanvas("canvasRelSysErr","",200,10,1350,900);  // gives the page size
        DrawGammaCanvasSettings( canvasRelSysErr, 0.08, 0.02, 0.035, 0.09);

        TH2F * histo2DRelSysErr;
        histo2DRelSysErr                    = new TH2F("histo2DRelSysErr","histo2DRelSysErr",11000,0.,maxPtGlobalPi0,1000,0,60.5);
        SetStyleHistoTH2ForGraphs(histo2DRelSysErr, "#it{p}_{T} (GeV/#it{c})","sys Err (%)",0.035,0.04, 0.035,0.04, 1.,1.);
        histo2DRelSysErr->Draw("copy");
            TLegend* legendRelSysErr        = GetAndSetLegend2(0.62, 0.92-(0.035*(nMeasSetPi0+1)/2), 0.95, 0.92, 32);
            legendRelSysErr->SetNColumns(2);
            for (Int_t i = 0; i < nMeasSetPi0; i++){
                cout << "plotting graph: " << availableMeasPi0[i] << "\t" <<graphRelSystPi0[availableMeasPi0[i]]->GetName() << endl;
                DrawGammaSetMarkerTGraph(graphRelSystPi0[availableMeasPi0[i]], markerTriggWeighted[availableMeasPi0[i]], sizeTrigg[availableMeasPi0[i]],
                                         colorTriggWeighted[availableMeasPi0[i]], colorTriggWeighted[availableMeasPi0[i]]);
                graphRelSystPi0[availableMeasPi0[i]]->Draw("p,same,z");
                legendRelSysErr->AddEntry(graphRelSystPi0[availableMeasPi0[i]],nameTriggerWeighted[availableMeasPi0[i]],"p");
            }
            legendRelSysErr->Draw();

            TLatex *labelRelErrEnergy    = new TLatex(0.15,0.89,collisionSystem.Data());
            SetStyleTLatex( labelRelErrEnergy, 0.85*textSizeLabelsPixel,4);
            labelRelErrEnergy->SetTextFont(43);
            labelRelErrEnergy->Draw();
            TLatex *labelRelErrPi0       = new TLatex(0.15,0.85,"#pi^{0} #rightarrow #gamma#gamma");
            SetStyleTLatex( labelRelErrPi0, 0.85*textSizeLabelsPixel,4);
            labelRelErrPi0->SetTextFont(43);
            labelRelErrPi0->Draw();
            TLatex *labelDetProcRelErr    = new TLatex(0.15, 0.81,detectionProcess.Data());
            SetStyleTLatex( labelDetProcRelErr, 0.85*textSizeLabelsPixel,4);
            labelDetProcRelErr->SetTextFont(43);
            labelDetProcRelErr->Draw();

        canvasRelSysErr->SaveAs(Form("%s/Pi0RpA_RelSysErr_SingleMeas.%s",outputDir.Data(),suffix.Data()));


            DrawGammaSetMarkerTGraphAsym(graphRelErrorPi0Sys, 24, 1.5, kGray+1 , kGray+1);
//             graphRelErrorPi0Sys->SetLineStyle(7);
            graphRelErrorPi0Sys->Draw("same,pze1");
            legendRelSysErr->AddEntry(graphRelErrorPi0Sys,"average","p");
            legendRelSysErr->Draw();

            labelRelErrEnergy->Draw();
            labelRelErrPi0->Draw();
            labelDetProcRelErr->Draw();

        canvasRelSysErr->SaveAs(Form("%s/Pi0RpA_RelSysErrWithAverage_SingleMeas.%s",outputDir.Data(),suffix.Data()));


        // plot stat relative errors for individual triggers
        TCanvas* canvasRelStatErr           = new TCanvas("canvasRelStatErr","",200,10,1350,900);  // gives the page size
        DrawGammaCanvasSettings( canvasRelStatErr, 0.08, 0.02, 0.035, 0.09);

        TH2F * histo2DRelStatErr;
        histo2DRelStatErr                   = new TH2F("histo2DRelStatErr","histo2DRelStatErr",11000,0.,maxPtGlobalPi0,1000,0,60.5);
        SetStyleHistoTH2ForGraphs(histo2DRelStatErr, "#it{p}_{T} (GeV/#it{c})","stat Err (%)",0.035,0.04, 0.035,0.04, 1.,1.);
        histo2DRelStatErr->Draw("copy");

            TLegend* legendRelStatErr       = GetAndSetLegend2(0.62, 0.92-(0.035*nMeasSetPi0/2), 0.95, 0.92, 32);
            legendRelStatErr->SetNColumns(2);
            for (Int_t i = 0; i < nMeasSetPi0; i++){
                 cout << "plotting graph: " << availableMeasPi0[i] << "\t" <<graphRelStatPi0[availableMeasPi0[i]]->GetName() << endl;
                if (graphRelStatPi0[availableMeasPi0[i]] ){
//                     TGraphAsymmErrors* dummyGraph = new TGraphAsymmErrors(histoRelStatPi0[availableMeasPi0[i]]);
                    graphRelStatPi0[availableMeasPi0[i]]->Print();
                    DrawGammaSetMarkerTGraph(graphRelStatPi0[availableMeasPi0[i]], markerTriggWeighted[availableMeasPi0[i]], sizeTrigg[availableMeasPi0[i]],
                                         colorTriggWeighted[availableMeasPi0[i]], colorTriggWeighted[availableMeasPi0[i]]);
                    graphRelStatPi0[availableMeasPi0[i]]->Draw("p,same");
                    legendRelStatErr->AddEntry(graphRelStatPi0[availableMeasPi0[i]],nameTriggerWeighted[availableMeasPi0[i]],"p");

                     for (Int_t j = 1; j < histoRelStatPi0[availableMeasPi0[i]]->GetNbinsX()+1; j++){
                        cout << j << ": " << histoRelStatPi0[availableMeasPi0[i]]->GetBinContent(j) << endl;
                     }
                } else if (histoRelStatPi0[availableMeasPi0[i]]) {
                    DrawGammaSetMarker(histoRelStatPi0[availableMeasPi0[i]],markerTriggWeighted[availableMeasPi0[i]], sizeTrigg[availableMeasPi0[i]],
                                            colorTriggWeighted[availableMeasPi0[i]], colorTriggWeighted[availableMeasPi0[i]]);
                    histoRelStatPi0[availableMeasPi0[i]]->DrawCopy("p,same,z");
                    legendRelStatErr->AddEntry(histoRelStatPi0[availableMeasPi0[i]],nameTriggerWeighted[availableMeasPi0[i]],"p");
                }
            }
            legendRelStatErr->Draw();

            labelRelErrEnergy->Draw();
            labelRelErrPi0->Draw();
            labelDetProcRelErr->Draw();

        canvasRelStatErr->SaveAs(Form("%s/Pi0RpA_RelStatErr_SingleMeas.%s",outputDir.Data(),suffix.Data()));

        // plot full error for final result decomposed
        TCanvas* canvasRelTotErr            = new TCanvas("canvasRelTotErr","",200,10,1350,900);  // gives the page size
        DrawGammaCanvasSettings( canvasRelTotErr, 0.08, 0.02, 0.035, 0.09);

        TH2F * histo2DRelTotErrPi0;
        histo2DRelTotErrPi0                 = new TH2F("histo2DRelTotErrPi0","histo2DRelTotErrPi0",11000,0.,maxPtGlobalPi0,1000,0,40.5);
        SetStyleHistoTH2ForGraphs(histo2DRelTotErrPi0, "#it{p}_{T} (GeV/#it{c})","Err (%)",0.035,0.04, 0.035,0.04, 1.,1.);
        histo2DRelTotErrPi0->Draw("copy");

            DrawGammaSetMarkerTGraphAsym(graphRelErrorPi0Tot, 20, 1.5, kBlack , kBlack);
            graphRelErrorPi0Tot->Draw("p,same,z");
            DrawGammaSetMarkerTGraphAsym(graphRelErrorPi0Stat, 24, 1.5, kGray+2 , kGray+2);
            graphRelErrorPi0Stat->Draw("l,x0,same,e1");
            DrawGammaSetMarkerTGraphAsym(graphRelErrorPi0Sys, 24, 1.5, kGray+1 , kGray+1);
            graphRelErrorPi0Sys->SetLineStyle(7);
            graphRelErrorPi0Sys->Draw("l,x0,same,e1");

            TLegend* legendRelTotErr2       = GetAndSetLegend2(0.72, 0.92-(0.035*3), 0.9, 0.92, 32);
            legendRelTotErr2->AddEntry(graphRelErrorPi0Tot,"tot","p");
            legendRelTotErr2->AddEntry(graphRelErrorPi0Stat,"stat","l");
            legendRelTotErr2->AddEntry(graphRelErrorPi0Sys,"sys","l");
            legendRelTotErr2->Draw();

            labelRelErrEnergy->Draw();
            labelRelErrPi0->Draw();
            labelDetProcRelErr->Draw();

        canvasRelTotErr->SaveAs(Form("%s/Pi0RpA_RelErrorsFulldecomp.%s",outputDir.Data(),suffix.Data()));
           
    }
    
    
    // return;
    //***************************************************************************************************************
    //************************************Plotting nuclear modification factors  *************************************
    //***************************************************************************************************************
    TCanvas* canvasNuclModFac = new TCanvas("canvasNuclModFac","",200,10,1350,900);
    DrawGammaCanvasSettings( canvasNuclModFac, 0.08, 0.01, 0.01, 0.12);

    Int_t textSizePixelNuclModFac      = 54;
    Double_t textSizeNuclModFac          = 0.04;
    Double_t textSizeFacNuclModFac          = 0.04;
    if (canvasNuclModFac->XtoPixel(canvasNuclModFac->GetX2()) <canvasNuclModFac->YtoPixel(canvasNuclModFac->GetY1()) ){
    textSizeNuclModFac            = (Double_t)textSizePixelNuclModFac/canvasNuclModFac->XtoPixel(canvasNuclModFac->GetX2()) ;
    textSizeFacNuclModFac               = (Double_t)1./canvasNuclModFac->XtoPixel(canvasNuclModFac->GetX2()) ;
    } else {
    textSizeNuclModFac            = (Double_t)textSizePixelNuclModFac/canvasNuclModFac->YtoPixel(canvasNuclModFac->GetY1());
    textSizeFacNuclModFac               = (Double_t)1./canvasNuclModFac->YtoPixel(canvasNuclModFac->GetY1());
    }
    cout << textSizeNuclModFac << endl;

    Double_t minNuclModFacSpectra   = 0.0;
    Double_t maxNuclModFacSpectra   = 1.9;
    TH2F * histo2DNuclModFac = new TH2F("histo2DNuclModFac","histo2DNuclModFac",1000,0., maxPtGlobalPi0,10000,minNuclModFacSpectra,maxNuclModFacSpectra);
    SetStyleHistoTH2ForGraphs(histo2DNuclModFac,  "#it{p}_{T} (GeV/#it{c})","#it{R}_{pA}",
                            0.85*textSizeNuclModFac,textSizeNuclModFac, 0.85*textSizeNuclModFac,textSizeNuclModFac, 0.9, 0.55, 510, 505);
    histo2DNuclModFac->GetXaxis()->SetMoreLogLabels();
    histo2DNuclModFac->GetXaxis()->SetNoExponent(kTRUE);
    histo2DNuclModFac->DrawCopy();
    Double_t minXLegendModFac = 0.62;
    Int_t nColumnsModFac      = 1;
    Double_t maxYLegendModFac = 0.95;
    Double_t minYLegendModFac = 0.95-(nrOfTrigToBeComb/nColumnsModFac*0.75*textSizeNuclModFac);
    TLegend* legendNuclModFac = GetAndSetLegend2(minXLegendModFac, minYLegendModFac, 0.95, maxYLegendModFac,0.85*textSizePixelNuclModFac);
    legendNuclModFac->SetNColumns(nColumnsModFac);
    for (Int_t i = 0; i< nrOfTrigToBeComb; i++){
        if (graphsNuclearModFactorSysPi0[i])
            DrawGammaSetMarkerTGraphAsym(graphsNuclearModFactorSysPi0[i], markerTrigg[i], sizeTrigg[i], colorTrigg[i], colorTrigg[i], 1, kTRUE);
        if (graphsNuclearModFactorPi0[i])
            DrawGammaSetMarkerTGraphAsym(graphsNuclearModFactorPi0[i], markerTrigg[i], sizeTrigg[i], colorTrigg[i], colorTrigg[i]);
        if (graphsNuclearModFactorPi0[i])graphsNuclearModFactorPi0[i]->Draw("e1,same");
        if (graphsNuclearModFactorSysPi0[i])graphsNuclearModFactorSysPi0[i]->Draw("p,E2,same");
        legendNuclModFac->AddEntry(graphsNuclearModFactorPi0[i],triggerNameLabel[i].Data(),"p");
        // if ( !maskedFullyPi0[i] )graphsNuclearModFactorPi0[i]->Draw("e1,same");
        // if ( !maskedFullyPi0[i] && graphsNuclearModFactorSysPi0[i])graphsNuclearModFactorSysPi0[i]->Draw("p,E2,same");
        // if ( !maskedFullyPi0[i] )legendNuclModFac->AddEntry(graphsNuclearModFactorPi0[i],triggerNameLabel[i].Data(),"p");
    }
    legendNuclModFac->Draw();

    DrawGammaLines(0., maxPtGlobalPi0 , 1, 1 ,1, kGray+1, 7);

    TLatex *labelEnergyNuclModFac = new TLatex(0.13, 0.91,collisionSystem.Data());
    SetStyleTLatex( labelEnergyNuclModFac, 0.85*textSizeNuclModFac,4);
    labelEnergyNuclModFac->Draw();

    TLatex *labelPi0NuclModFac = new TLatex(0.13, 0.91-textSizeNuclModFac*0.85*0.85,"#pi^{0} #rightarrow #gamma#gamma");
    SetStyleTLatex( labelPi0NuclModFac, 0.85*textSizeNuclModFac,4);
    labelPi0NuclModFac->Draw();

    TLatex *labelDetProcNuclModFac = new TLatex(0.13, 0.91-(2*textSizeNuclModFac*0.85*0.85),detectionProcess.Data());
    SetStyleTLatex( labelDetProcNuclModFac, 0.85*textSizeNuclModFac,4);
    labelDetProcNuclModFac->Draw();

    canvasNuclModFac->Update();
    canvasNuclModFac->SaveAs(Form("%s/Pi0_%s_NuclModFactor_Triggers.%s",outputDir.Data(),isMC.Data(),suffix.Data()));

    histo2DNuclModFac->DrawCopy();
    for (Int_t i = 0; i< nrOfTrigToBeComb; i++){
        if (graphsNuclearModFactorSysPi0Shrunk[i])
            DrawGammaSetMarkerTGraphAsym(graphsNuclearModFactorSysPi0Shrunk[i], markerTrigg[i], sizeTrigg[i], colorTrigg[i], colorTrigg[i], 1, kTRUE);
        if (graphsNuclearModFactorPi0Shrunk[i])
            DrawGammaSetMarkerTGraphAsym(graphsNuclearModFactorPi0Shrunk[i], markerTrigg[i], sizeTrigg[i], colorTrigg[i], colorTrigg[i]);
        if ( !maskedFullyPi0[i] )graphsNuclearModFactorPi0Shrunk[i]->Draw("e1,same");
        if ( !maskedFullyPi0[i] && graphsNuclearModFactorSysPi0Shrunk[i])graphsNuclearModFactorSysPi0Shrunk[i]->Draw("p,E2,same");
    }
    legendNuclModFac->Draw();

    DrawGammaLines(0., maxPtGlobalPi0 , 1, 1 ,1, kGray+1, 7);
    labelEnergyNuclModFac->Draw();
    labelPi0NuclModFac->Draw();
    labelDetProcNuclModFac->Draw();

    canvasNuclModFac->Update();
    canvasNuclModFac->SaveAs(Form("%s/Pi0_%s_NuclModFactor_TriggersUsed.%s",outputDir.Data(),isMC.Data(),suffix.Data()));


    histo2DNuclModFac->DrawCopy();
    if (graphNuclModFactorWeightedAveragePi0Sys){
        DrawGammaSetMarkerTGraphAsym(graphNuclModFactorWeightedAveragePi0Sys, 24, 1.5, kBlack, kBlack, 1, kTRUE);
        graphNuclModFactorWeightedAveragePi0Sys->Draw("p,E2,same");
    }
    if (graphNuclModFactorWeightedAveragePi0Stat){
        DrawGammaSetMarkerTGraphAsym(graphNuclModFactorWeightedAveragePi0Stat, 24, 1.5, kBlack, kBlack);
        graphNuclModFactorWeightedAveragePi0Stat->Draw("e1,same");
    }

    DrawGammaLines(0., maxPtGlobalPi0 , 1, 1 ,1, kGray+1, 7);

    labelEnergyNuclModFac->Draw();
    labelPi0NuclModFac->Draw();
    labelDetProcNuclModFac->Draw();

    canvasNuclModFac->Update();
    canvasNuclModFac->SaveAs(Form("%s/Pi0_%s_NuclModFactor.%s",outputDir.Data(),isMC.Data(),suffix.Data()));



    if(enableEta){
        //***************************************************************************************************************
        //************************************Plotting invariant yield Eta *************************************
        //***************************************************************************************************************
        TCanvas* canvasCorrSpectraEta = new TCanvas("canvasCorrSpectraEta","",0,0,1000,1350);// gives the page size
        DrawGammaCanvasSettings( canvasCorrSpectraEta, 0.15, 0.017, 0.015, 0.07);
        canvasCorrSpectraEta->SetLogy();

        Double_t minCorrYieldSpectraEta   = 2e-10;
        Double_t maxCorrYieldSpectraEta   = 1e2;

        TH2F * histo2DInvYieldSpectraEta = new TH2F("histo2DInvYieldSpectraEta","histo2DInvYieldSpectraEta",1000,0., maxPtGlobalEta,10000,minCorrYieldSpectraEta,maxCorrYieldSpectraEta);
        SetStyleHistoTH2ForGraphs(histo2DInvYieldSpectraEta, "#it{p}_{T} (GeV/#it{c})","#frac{1}{2#pi #it{N}_{ev}} #frac{d^{2}#it{N}}{#it{p}_{T}d#it{p}_{T}d#it{y}} (#it{c}/GeV)^{2}",
                                0.85*textSizeSpectra,0.04, 0.85*textSizeSpectra,textSizeSpectra, 0.8,1.55);
        histo2DInvYieldSpectraEta->DrawCopy();
        Double_t minXLegendRawEta = 0.62;
        Int_t nColumnsRawEta      = 1;
        Double_t maxYLegendRawEta = 0.95;
        Double_t minYLegendRawEta = 0.95-(nrOfTrigToBeComb/nColumnsRaw*0.75*textSizeSpectra);
        TLegend* legendSpectraEta = GetAndSetLegend2(minXLegendRawEta, minYLegendRawEta, 0.95, maxYLegendRawEta,0.85*textSizePixelSpectra);
        legendSpectraEta->SetNColumns(nColumnsRaw);
        for (Int_t i = 0; i< nrOfTrigToBeComb; i++){
            DrawGammaSetMarkerTGraphAsym(graphCorrectedYieldEta[i], markerTrigg[i], sizeTrigg[i], colorTrigg[i], colorTrigg[i]);
            graphCorrectedYieldEta[i]->Draw("e1,p,same");
            DrawGammaSetMarkerTGraphAsym(graphCorrectedYieldEtaRef[i],  markerTriggMC[i], sizeTrigg[i], colorTriggShade[i], colorTriggShade[i]);
            graphCorrectedYieldEtaRef[i]->Draw("e1,p,same");
            legendSpectraEta->AddEntry(graphCorrectedYieldEta[i],triggerNameLabel[i].Data(),"p");
        }
        legendSpectraEta->Draw();


        TLatex *labelEnergySpectraEta = new TLatex(0.2, 0.12+(2*textSizeSpectra*0.85*0.75),collisionSystem.Data());
        SetStyleTLatex( labelEnergySpectraEta, 0.85*textSizeSpectra,4);
        labelEnergySpectraEta->Draw();

        TLatex *labelEtaSpectra = new TLatex(0.2, 0.12+textSizeSpectra*0.85*0.75,"#eta #rightarrow #gamma#gamma");
        SetStyleTLatex( labelEtaSpectra, 0.85*textSizeSpectra,4);
        labelEtaSpectra->Draw();

        TLatex *labelDetProcSpectraEta = new TLatex(0.2, 0.12,detectionProcess.Data());
        SetStyleTLatex( labelDetProcSpectraEta, 0.85*textSizeSpectra,4);
        labelDetProcSpectraEta->Draw();

        canvasCorrSpectraEta->Update();
        canvasCorrSpectraEta->SaveAs(Form("%s/Eta_%s_CorrectedYieldTrigg.%s",outputDir.Data(),isMC.Data(),suffix.Data()));
    }





    //****************************************************************************************************************
    //************* Processing of each individual trigger, reducing ranges & adding systematics **********************
    //****************************************************************************************************************
    Double_t ptSysRelEta                                    [MaxNumberOfFiles][400];
    Double_t yErrorSysLowRelEta                             [MaxNumberOfFiles][400];
    Double_t yErrorSysHighRelEta                            [MaxNumberOfFiles][400];
    Bool_t sysAvailEta                                      [MaxNumberOfFiles];

    Bool_t sysAvailSingleEta                                [MaxNumberOfFiles];
    Int_t numberBinsSysAvailSingleEta                       [MaxNumberOfFiles];

    TH1D* histoNuclearModFactorEta                          [MaxNumberOfFiles];
    TGraphAsymmErrors* graphsNuclearModFactorEta            [MaxNumberOfFiles];
    TGraphAsymmErrors* graphsNuclearModFactorSysEta         [MaxNumberOfFiles];
    TH1D* histoNuclearModFactorEtaShrunk                          [MaxNumberOfFiles];
    TGraphAsymmErrors* graphsNuclearModFactorEtaShrunk            [MaxNumberOfFiles];
    TGraphAsymmErrors* graphsNuclearModFactorSysEtaShrunk         [MaxNumberOfFiles];

    vector<TString>** ptSysDetailEta     = new vector<TString>*[MaxNumberOfFiles];
    for(Int_t iR=0; iR<nrOfTrigToBeComb; iR++) ptSysDetailEta[iR] = new vector<TString>[400];

    // definition of predefined arrays for trigger correlation filling
    TH1D*               histoStatEta    [MaxNumberOfFiles];
    TGraphAsymmErrors*  graphSystEta    [MaxNumberOfFiles];
    TH1D*               histoRelStatEta [MaxNumberOfFiles];
    TGraphAsymmErrors*  graphRelSystEta [MaxNumberOfFiles];

    Int_t offSetsEta[MaxNumberOfFiles];
        for(Int_t set=0;set<MaxNumberOfFiles;set++) offSetsEta[set]= 0;
    Int_t offSetsEtaSys[MaxNumberOfFiles];
        for(Int_t set=0;set<MaxNumberOfFiles;set++) offSetsEtaSys[set]= 0;
    if(optionEnergy.CompareTo("pPb_8TeV")==0){
        if(mode == 2){
            offSetsEta[1] = -1; //INT7
            offSetsEta[4] = -2; //EG2
            offSetsEta[5] = -2; //EG1
            // offSetsEta[5] = 2; //EG1
        }else if(mode == 4){
            offSetsEta[1] = -1; //INT7
            offSetsEta[4] = -2; //EG2
            offSetsEta[5] = -2; //EG1
        }
    } else if(optionEnergy.CompareTo("pPb_5.023TeV")==0 ){
        if(mode == 2){
            offSetsEta[1] = 0; //INT7
            if (fCentOutput.Contains("00100") ){
                offSetsEta[4] = 16; //EG2
                offSetsEta[5] = 21; //EG1
            } else {
                offSetsEta[4] = 7; //EG2
                offSetsEta[5] = 11; //EG1                
            }
        } else if(mode == 4){
            offSetsEta[1] = 0; //INT7
            if (fCentOutput.Contains("00100") ){
                offSetsEta[4] = 13; //EG2
                offSetsEta[5] = 17; //EG1
            } else {
                offSetsEta[4] = 7; //EG2
                offSetsEta[5] = 11; //EG1                
            }
        }
    }
    if(enableEta){
        for (Int_t i = 0; i< nrOfTrigToBeComb; i++){
            // read systematics, if fileName is set to "bla" no action has to be performed and systematics will be disabled for the rest of the analysis
            if (sysFileEta[i].CompareTo("bla") != 0){
                sysAvailEta[i]                = kTRUE;
                ifstream  fileSysErrEta;
                fileSysErrEta.open(sysFileEta[i].Data(),ios_base::in);
                cout << sysFileEta[i].Data() << endl;
                gSystem->Exec(Form("cp %s %s/SystematicErrorAveraged_Eta_%s.txt", sysFileEta[i].Data(), outputDir.Data(),triggerName[i].Data()));


                Int_t iPtBin = 0;
                cout << "reading sys file summed" << endl;
                while(!fileSysErrEta.eof() && iPtBin < 400){
                    Double_t garbage = 0;
                    fileSysErrEta >>ptSysRelEta[i][iPtBin] >> yErrorSysLowRelEta[i][iPtBin] >> yErrorSysHighRelEta[i][iPtBin]>>    garbage >> garbage;
                    cout << iPtBin << "\t"<< ptSysRelEta[i][iPtBin]<< "\t"  << yErrorSysLowRelEta[i][iPtBin] << "\t"  <<yErrorSysHighRelEta[i][iPtBin] << "\t"  << endl;;
                    iPtBin++;
                }
                fileSysErrEta.close();
            // read in detailed systematics
                string sysFileEtaDet = sysFileEta[i].Data();

                if(!replace(sysFileEtaDet, "Averaged", "AveragedSingle")){
                    cout << "WARNING: could not find detailed systematics file " << sysFileEtaDet << ", skipping... " << endl;
                    sysAvailSingleEta[i] = kFALSE;
                }else{
                    ifstream fileSysErrDetailedEta;
                    fileSysErrDetailedEta.open(sysFileEtaDet,ios_base::in);
                    if(fileSysErrDetailedEta.is_open()) {
                        sysAvailSingleEta[i] = kTRUE;
                        gSystem->Exec(Form("cp %s %s/SystematicErrorAveragedSingle_Eta_%s.txt", ((TString)sysFileEtaDet).Data(), outputDir.Data(),triggerName[i].Data()));
                    } else{
                        sysAvailSingleEta[i] = kFALSE;
                        cout << "No single errors were found" << endl;
                    }

                    if (sysAvailSingleEta[i]){
                        cout << sysFileEtaDet << endl;
                        iPtBin = 0;
                        string line;
                        Int_t iPtBinColumn = 0;
                        while (getline(fileSysErrDetailedEta, line) && iPtBin < 400) {
                            istringstream ss(line);
                            TString temp="";
                            iPtBinColumn = 0;
                            while(ss && iPtBinColumn < 400){
                                ss >> temp;
                                if( !(iPtBin==0 && temp.CompareTo("bin")==0) && !temp.IsNull()){
                                    ptSysDetailEta[i][iPtBin].push_back(temp);
                                    iPtBinColumn++;
                                }
                            }
                            if(iPtBin == 0){
                                ptSysDetailEta[i][iPtBin++].push_back("TotalError");
                                iPtBinColumn++;
                            }else iPtBin++;
                        }
                        numberBinsSysAvailSingleEta[i] = iPtBin;
                        fileSysErrDetailedEta.close();
                    }
                }
            } else {
                sysAvailEta[i]             = kFALSE;
                sysAvailSingleEta[i]       = kFALSE;
            }
            cout << sysAvailEta[i] << "\t" << sysAvailSingleEta[i] << endl;

            // calculate RpA by dividing spectra and using nuclear overlap function
            // scale pA or AA measurement to NSD (if needed)
            if(doBinShift){
                for (Int_t i = 0; i< nrOfTrigToBeComb; i++){
                    if(graphCorrectedYieldEta[i])graphCorrectedYieldEta[i]     = ApplyYshiftIndividualSpectra( graphCorrectedYieldEta[i], funcTCMFitShiftEta);
                }
            }
            graphCorrectedYieldEtaScaled[i] = (TGraphAsymmErrors*) graphCorrectedYieldEta[i]->Clone(Form("graphCorrectedYieldEtaScaled%d",i));
            if(optionEnergy.Contains("pPb_8TeV")){
                graphCorrectedYieldEtaScaled[i] = ScaleGraphAsym(graphCorrectedYieldEtaScaled[i], xsection*recalcBarn);
            } else {
                graphCorrectedYieldEtaScaled[i] = ScaleGraphAsym(graphCorrectedYieldEtaScaled[i], scalingToNSD); //std
            }
            cout << "pPb spectrum for trigger " << triggerName[i].Data() << endl;
            graphCorrectedYieldEtaScaled[i]->Print();

            graphCorrectedYieldEtaRefScaled[i] = (TGraphAsymmErrors*) graphCorrectedYieldEtaRef[i]->Clone(Form("graphCorrectedYieldEtaRefScaled%d",i));
            graphCorrectedYieldEtaRefScaled[i] = ScaleGraphAsym(graphCorrectedYieldEtaRefScaled[i], xsectionReference*recalcBarn);
            if(doBinShift){
                for (Int_t i = 0; i< nrOfTrigToBeComb; i++){
                    if(graphCorrectedYieldEtaRefScaled[i])graphCorrectedYieldEtaRefScaled[i]     = ApplyYshiftIndividualSpectra( graphCorrectedYieldEtaRefScaled[i], funcTCMFitShiftEtaRef);
                }
            }
            if(optionEnergy.Contains("pPb_8TeV")){
                graphCorrectedYieldEtaRefScaled[i] = ScaleGraphAsym(graphCorrectedYieldEtaRefScaled[i], 208);
            } else {
                graphCorrectedYieldEtaRefScaled[i] = ScaleGraphAsym(graphCorrectedYieldEtaRefScaled[i], nuclOverlap); //std
            }
            cout << "pp reference spectrum for trigger " << triggerName[i].Data() << endl;
            graphCorrectedYieldEtaRefScaled[i]->Print();
            if(optionEnergy.Contains("pPb_8TeV")){
                TF1* centerOfMassScalingFunc8000to8160GeVEta = new TF1("centerOfMassScalingFunc8000to8160GeVEta","1 / (1.044597 + 0.001417 * x)",0,200);
                graphCorrectedYieldEtaRefScaled[i]   = CalculateGraphErrRatioToFit(graphCorrectedYieldEtaRefScaled[i], centerOfMassScalingFunc8000to8160GeVEta);
            }
            graphsNuclearModFactorEta[i] = CalculateAsymGraphRatioToGraph(graphCorrectedYieldEtaScaled[i],graphCorrectedYieldEtaRefScaled[i]);

            cout << "nuclear modification factor for trigger " << triggerName[i].Data() << endl;
            graphsNuclearModFactorEta[i]->Print();

            graphsNuclearModFactorSysEta[i] = (TGraphAsymmErrors*) graphsNuclearModFactorEta[i]->Clone(Form("graphsNuclearModFactorSysEta%s",triggerName[i].Data()));
            if ( maskedFullyEta[i] ){
                graphsNuclearModFactorEta[i] = NULL;
                graphsNuclearModFactorSysEta[i] = NULL;
            }
            // put systematics on graphs
            if (graphsNuclearModFactorSysEta[i]){
                // if (sysAvailSingleEta[i]){
                //     nRelSysErrEtaSources                    = (Int_t)ptSysDetailEta[i][0].size()-1;
                //     for (Int_t k = 0; k < nRelSysErrEtaSources; k++ ){
                //         graphRelSysErrEtaSource[k][i]       = (TGraphAsymmErrors*) graphsCorrectedYieldSysRemoved0Eta[i]->Clone(Form("RelSysErrEtaSource%s_%s",((TString)ptSysDetailEta[i][0].at(k+1)).Data(), triggerName[i].Data()));
                //         cout << Form("RelSysErrSource%s_%s",((TString)ptSysDetailEta[i][0].at(k+1)).Data(), triggerName[i].Data()) << endl;
                //     }
                // }
                for (Int_t j = 0; j< graphsNuclearModFactorSysEta[i]->GetN(); j++){
                    if (sysAvailEta[i]){
                        Int_t counter = 0;
                        while(counter < 400 && TMath::Abs(graphsNuclearModFactorSysEta[i]->GetX()[j] - ptSysRelEta[i][counter])> 0.001) counter++;
                        if (counter < 400){
                            cout << ptSysRelEta[i][counter]<< "\t found it" << endl;
                            Double_t yErrorSysLowDummy  = TMath::Abs(yErrorSysLowRelEta[i][counter]/100*graphsNuclearModFactorSysEta[i]->GetY()[j]);
                            Double_t yErrorSysHighDummy = yErrorSysHighRelEta[i][counter]/100*graphsNuclearModFactorSysEta[i]->GetY()[j];
                            graphsNuclearModFactorSysEta[i]->SetPointEYlow(j,yErrorSysLowDummy);
                            graphsNuclearModFactorSysEta[i]->SetPointEYhigh(j,yErrorSysHighDummy);
                            // if (sysAvailSingleEta[i]){
                            //     for (Int_t k = 0; k < nRelSysErrEtaSources; k++ ){
                            //         graphRelSysErrEtaSource[k][i]->SetPoint(j, graphsNuclearModFactorSysEta[i]->GetX()[j] ,((TString)ptSysDetailEta[i][counter+1].at(k+1)).Atof());
                            //         graphRelSysErrEtaSource[k][i]->SetPointEYhigh(j,0);
                            //         graphRelSysErrEtaSource[k][i]->SetPointEYlow(j,0);
                            //     }
                            // }
                        } else {
                            graphsNuclearModFactorSysEta[i]->SetPointEYlow(j,0);
                            graphsNuclearModFactorSysEta[i]->SetPointEYhigh(j,0);
                            // if (sysAvailSingleEta[i]){
                            //     for (Int_t k = 0; k < nRelSysErrEtaSources; k++ ){
                            //         graphRelSysErrEtaSource[k][i]->SetPoint(j, graphsNuclearModFactorSysEta[i]->GetX()[j] ,0);
                            //         graphRelSysErrEtaSource[k][i]->SetPointEYlow(j,0);
                            //         graphRelSysErrEtaSource[k][i]->SetPointEYhigh(j,0);
                            //     }
                            // }
                        }
                    } else {
                        graphsNuclearModFactorSysEta[i]->SetPointEYlow(j,0);
                        graphsNuclearModFactorSysEta[i]->SetPointEYhigh(j,0);
                        averagedEta = kFALSE;
                        // if (sysAvailSingleEta[i]){
                        //     for (Int_t k = 0; k < nRelSysErrEtaSources; k++ ){
                        //         graphRelSysErrEtaSource[k][i]->SetPoint(j, graphsNuclearModFactorSysEta[i]->GetX()[j] ,0);
                        //         graphRelSysErrEtaSource[k][i]->SetPointEYlow(j,0);
                        //         graphRelSysErrEtaSource[k][i]->SetPointEYhigh(j,0);
                        //     }
                        // }
                    }
                }
            }

            cout << "step 3" << endl;
            cout << "range to be accepted: " << ptFromSpecEta[i][0] << "\t-\t" << ptFromSpecEta[i][1] << endl;

            graphsNuclearModFactorEtaShrunk[i] = (TGraphAsymmErrors*)graphsNuclearModFactorEta[i]->Clone(Form("graphsNuclearModFactorEtaShrunk%s",triggerName[i].Data()));
            graphsNuclearModFactorSysEtaShrunk[i] = (TGraphAsymmErrors*)graphsNuclearModFactorSysEta[i]->Clone(Form("graphsNuclearModFactorSysEtaShrunk%s",triggerName[i].Data()));

            // remove points at beginning according to ranges set for individual triggers
            while(graphsNuclearModFactorEtaShrunk[i]->GetX()[0] < ptFromSpecEta[i][0])
                graphsNuclearModFactorEtaShrunk[i]->RemovePoint(0);
            while(graphsNuclearModFactorSysEtaShrunk[i]->GetX()[0] < ptFromSpecEta[i][0])
                graphsNuclearModFactorSysEtaShrunk[i]->RemovePoint(0);
            // remove points at the end according to ranges set for individual triggers
            while (graphsNuclearModFactorEtaShrunk[i]->GetX()[graphsNuclearModFactorEtaShrunk[i]->GetN()-1] > ptFromSpecEta[i][1])
                graphsNuclearModFactorEtaShrunk[i]->RemovePoint(graphsNuclearModFactorEtaShrunk[i]->GetN()-1);
            while (graphsNuclearModFactorSysEtaShrunk[i]->GetX()[graphsNuclearModFactorSysEtaShrunk[i]->GetN()-1] > ptFromSpecEta[i][1])
                graphsNuclearModFactorSysEtaShrunk[i]->RemovePoint(graphsNuclearModFactorSysEtaShrunk[i]->GetN()-1);



            histoNuclearModFactorEta[i] = (TH1D*)histoRawYieldsEta[i]->Clone(Form("histoNuclearModFactorEta%s",triggerName[i].Data()));
            histoNuclearModFactorEtaShrunk[i] = (TH1D*)histoRawYieldsEta[i]->Clone(Form("histoNuclearModFactorEtaShrunk%s",triggerName[i].Data()));
            for (Int_t j = 0; j< histoNuclearModFactorEta[i]->GetNbinsX()+1; j++){
                histoNuclearModFactorEta[i]->SetBinContent(j,0);
                histoNuclearModFactorEta[i]->SetBinError(j,0);
                histoNuclearModFactorEtaShrunk[i]->SetBinContent(j,0);
                histoNuclearModFactorEtaShrunk[i]->SetBinError(j,0);
            }
            for (Int_t j = 0; j < graphsNuclearModFactorEta[i]->GetN(); j++){
                Int_t bin = histoNuclearModFactorEta[i]->GetXaxis()->FindBin(graphsNuclearModFactorEta[i]->GetX()[j]);
                histoNuclearModFactorEta[i]->SetBinContent(bin,graphsNuclearModFactorEta[i]->GetY()[j]);
                histoNuclearModFactorEta[i]->SetBinError(bin,graphsNuclearModFactorEta[i]->GetEYlow()[j]);
            }
            for (Int_t j = 0; j < graphsNuclearModFactorEtaShrunk[i]->GetN(); j++){
                Int_t bin = histoNuclearModFactorEtaShrunk[i]->GetXaxis()->FindBin(graphsNuclearModFactorEtaShrunk[i]->GetX()[j]);
                histoNuclearModFactorEtaShrunk[i]->SetBinContent(bin,graphsNuclearModFactorEtaShrunk[i]->GetY()[j]);
                histoNuclearModFactorEtaShrunk[i]->SetBinError(bin,graphsNuclearModFactorEtaShrunk[i]->GetEYlow()[j]);
            }
            cout << "histo NuclModFac shrunk for trigg " << triggerName[i].Data() << endl;
            histoNuclearModFactorEtaShrunk[i]->Print("all");

            // Set correct trigger order for combination function
            Int_t nCorrOrder    = GetOrderedTrigger(triggerName[i]);
            if (nCorrOrder == -1){
                cout << "ERROR: trigger name not defined" << endl;
                return;
            }

            if ( graphsNuclearModFactorSysEtaShrunk[i]){
                histoStatEta[nCorrOrder]    = histoNuclearModFactorEtaShrunk[i];
                graphSystEta[nCorrOrder]    = graphsNuclearModFactorSysEtaShrunk[i];
                offSetsEtaSys[nCorrOrder]   = histoStatEta[nCorrOrder]->GetXaxis()->FindBin(graphSystEta[nCorrOrder]->GetX()[0])-1;
            }
        }
    }
    if (optionEnergy.CompareTo("pPb_8TeV") == 0){
        if (mode == 2 || mode == 4){
            offSetsEtaSys[1]+=-1; //EG1
            offSetsEtaSys[4]+=-2; //EG1
            offSetsEtaSys[5]+=-2; //EG1
        }
    } else if (optionEnergy.Contains("pPb_5.023TeV")){
        if (mode == 4) {
            offSetsEtaSys[4]+=offSetsEta[4];
            offSetsEtaSys[5]+=offSetsEta[5];
        }
    }
    // create weighted graphs for spectra and supporting graphs
    TString nameWeightsLogFileEta =     Form("%s/weightsEta_%s.dat",outputDir.Data(),isMC.Data());
    TGraphAsymmErrors* graphNuclModFactorWeightedAverageEtaStat    = NULL;
    TGraphAsymmErrors* graphNuclModFactorWeightedAverageEtaSys     = NULL;
    TGraphAsymmErrors* graphNuclModFactorWeightedAverageEtaTot     = NULL;
    if(enableEta){
        // Calculate averaged nuclear modification factor spectrum & respective supporting graphs according to statistical and systematic errors taking correctly into account the cross correlations
        if (averagedEta){
            cout << maxNAllowedEta << endl;
            // Calculate average Eta spectrum
            graphNuclModFactorWeightedAverageEtaTot = CombinePtPointsSpectraTriggerCorrMat(
                histoStatEta, graphSystEta,
                binningEta,  maxNAllowedEta,
                offSetsEta ,offSetsEtaSys,
                graphNuclModFactorWeightedAverageEtaStat, graphNuclModFactorWeightedAverageEtaSys,
                nameWeightsLogFileEta.Data(),
                mode, optionEnergy, "EtaRatio", kTRUE,
                fileInputCorrFactors
            );
        }
        graphNuclModFactorWeightedAverageEtaTot->Print();
        while (graphNuclModFactorWeightedAverageEtaTot->GetY()[0] <= 0) graphNuclModFactorWeightedAverageEtaTot->RemovePoint(0);
        while (graphNuclModFactorWeightedAverageEtaStat->GetY()[0] <= 0) graphNuclModFactorWeightedAverageEtaStat->RemovePoint(0);
        while (graphNuclModFactorWeightedAverageEtaSys->GetY()[0] <= 0) graphNuclModFactorWeightedAverageEtaSys->RemovePoint(0);



        if (averagedEta){
                // preparations for weight readout
            Double_t xValuesReadEta[400];
            Double_t weightsReadEta[MaxNumberOfFiles][400];
            Int_t availableMeasEta[MaxNumberOfFiles];
                for(Int_t set=0;set<MaxNumberOfFiles;set++) availableMeasEta[set]= -1;
            Int_t nMeasSetEta               = nrOfTrigToBeCombEtaRed;
            Int_t nPtBinsReadEta            = 0;

            // labeling and plotting settings
            Color_t colorTriggWeighted[MaxNumberOfFiles];
                for(Int_t set=0;set<MaxNumberOfFiles;set++) colorTriggWeighted[set]= GetDefaultTriggerColorName(nameTriggerWeighted[set], 0);
            Marker_t markerTriggWeighted[MaxNumberOfFiles];
                for(Int_t set=0;set<MaxNumberOfFiles;set++) markerTriggWeighted[set]= GetDefaultTriggerMarkerStyleName(nameTriggerWeighted[set], 0);

            // Reading weights from output file for plotting
            ifstream fileWeightsEta;
            fileWeightsEta.open(nameWeightsLogFileEta,ios_base::in);
            cout << "reading" << nameWeightsLogFileEta << endl;

            while(!fileWeightsEta.eof() && nPtBinsReadEta < 400){
                TString garbage = "";
                if (nPtBinsReadEta == 0){
                    fileWeightsEta >> garbage ;//>> availableMeas[0] >> availableMeas[1] >> availableMeas[2] >> availableMeas[3];
                    for (Int_t i = 0; i < nMeasSetEta; i++){
                        fileWeightsEta >> availableMeasEta[i] ;
                    }
                    cout << "read following measurements: ";
                    for (Int_t i = 0; i < nMeasSetEta; i++){
                        cout << availableMeasEta[i] << "\t" ;
                    }
                    cout << endl;
                } else {
                    fileWeightsEta >> xValuesReadEta[nPtBinsReadEta-1];
                    for (Int_t i = 0; i < nMeasSetEta; i++){
                        fileWeightsEta >> weightsReadEta[availableMeasEta[i]][nPtBinsReadEta-1] ;
                    }
                    cout << "read: "<<  nPtBinsReadEta << "\t"<< xValuesReadEta[nPtBinsReadEta-1] << "\t" ;
                    for (Int_t i = 0; i < nMeasSetEta; i++){
                        cout << weightsReadEta[availableMeasEta[i]][nPtBinsReadEta-1] << "\t";
                    }
                    cout << endl;
                }
                nPtBinsReadEta++;
            }
            nPtBinsReadEta = nPtBinsReadEta-2 ;
            fileWeightsEta.close();

            // creating & filling the weight graphs
            TGraph* graphWeightsEta[MaxNumberOfFiles];
            for (Int_t i = 0; i < MaxNumberOfFiles; i++){
                graphWeightsEta[i]                    = NULL;
            }
            for (Int_t i = 0; i < nMeasSetEta; i++){
                cout << i << "\t" << availableMeasEta[i] << endl;
                graphWeightsEta[availableMeasEta[i]]  = new TGraph(nPtBinsReadEta,xValuesReadEta,weightsReadEta[availableMeasEta[i]]);
                Int_t bin = 0;
                for (Int_t n = 0; n< nPtBinsReadEta; n++){
                    if (graphWeightsEta[availableMeasEta[i]]->GetY()[bin] == 0) graphWeightsEta[availableMeasEta[i]]->RemovePoint(bin);
                    else bin++;
                }
                graphWeightsEta[availableMeasEta[i]]->Print();
            }


            //  **********************************************************************************************************************
            //  ******************************************* Plotting weights Eta *****************************************************
            //  **********************************************************************************************************************
            Int_t textSizeLabelsPixel = 900*0.04;

            TCanvas* canvasWeights = new TCanvas("canvasWeights","",200,10,1350,900);// gives the page size
            DrawGammaCanvasSettings( canvasWeights, 0.08, 0.02, 0.035, 0.09);

            TH2F * histo2DWeights;
            histo2DWeights = new TH2F("histo2DWeights","histo2DWeights",11000,0.,maxPtGlobalEta,1000,-0.5,1.1);
            SetStyleHistoTH2ForGraphs(histo2DWeights, "#it{p}_{T} (GeV/#it{c})","#omega_{a} for BLUE",0.035,0.04, 0.035,0.04, 1.,1.);
            histo2DWeights->Draw("copy");

                TLegend* legendWeightsEta = GetAndSetLegend2(0.12, 0.14, 0.55, 0.14+(0.035*nMeasSetEta/2*1.35), 32);
                legendWeightsEta->SetNColumns(2);
                for (Int_t i = 0; i < nMeasSetEta; i++){
                    DrawGammaSetMarkerTGraph(graphWeightsEta[availableMeasEta[i]],markerTriggWeighted[availableMeasEta[i]], 1.5*sizeTrigg[availableMeasEta[i]],
                                            colorTriggWeighted[availableMeasEta[i]], colorTriggWeighted[availableMeasEta[i]]);
                    graphWeightsEta[availableMeasEta[i]]->Draw("p,same,e1");
                    legendWeightsEta->AddEntry(graphWeightsEta[availableMeasEta[i]],nameTriggerWeighted[availableMeasEta[i]],"p");
                }
                legendWeightsEta->Draw();

                TLatex *labelWeightsEnergy = new TLatex(0.95,0.24,collisionSystem.Data());
                SetStyleTLatex( labelWeightsEnergy, 0.85*textSizeLabelsPixel,4,1,42,kTRUE, 31);
                labelWeightsEnergy->SetTextFont(43);
                labelWeightsEnergy->Draw();
                TLatex *labelWeightsEta = new TLatex(0.95,0.20,"#pi^{0} #it{R}_{pA}");
                SetStyleTLatex( labelWeightsEta, 0.85*textSizeLabelsPixel,4,1,42,kTRUE, 31);
                labelWeightsEta->SetTextFont(43);
                labelWeightsEta->Draw();
                TLatex *labelDetProcWeights    = new TLatex(0.95, 0.16,detectionProcess.Data());
                SetStyleTLatex( labelDetProcWeights, 0.85*textSizeLabelsPixel,4,1,42,kTRUE, 31);
                labelDetProcWeights->SetTextFont(43);
                labelDetProcWeights->Draw();


        //      DrawGammaLines(0.23, 70. , 0.8, 0.8,1, kGray, 3);
                DrawGammaLines(0.23, 70. , 0.5, 0.5,1, kGray, 7);
                DrawGammaLines(0.23, 70. , 0.4, 0.4,1, kGray, 1);
                DrawGammaLines(0.23, 70. , 0.3, 0.3,1, kGray, 7);
                DrawGammaLines(0.23, 70. , 0.2, 0.2,1, kGray, 3);

            canvasWeights->SaveAs(Form("%s/%s_WeightsEtaRpATriggers.%s",outputDir.Data(), isMC.Data(), suffix.Data()));
            delete canvasWeights;

            // Calculating relative error for pi0
            for (Int_t i = 0; i < MaxNumberOfFiles; i++){
                if (histoStatEta[i]){
                    histoRelStatEta[i]      = CalculateRelErrUpTH1D( histoStatEta[i], Form("relativeStatErrorEta_%s", nameTriggerWeighted[i].Data()));
                    graphRelStatEta[i]      = new TGraphAsymmErrors(histoRelStatEta[i]);
                    while (graphRelStatEta[i]->GetY()[0] < 0  || graphRelStatEta[i]->GetY()[0] == 0) graphRelStatEta[i]->RemovePoint(0);
                    graphRelStatEta[i]->Print();
                }
                if (graphSystEta[i])
                    graphRelSystEta[i]      = CalculateRelErrUpAsymmGraph( graphSystEta[i], Form("relativeSysErrorEta_%s", nameTriggerWeighted[i].Data()));
            }

            TGraphAsymmErrors* graphRelErrorEtaTot        = CalculateRelErrUpAsymmGraph( graphNuclModFactorWeightedAverageEtaTot, "relativeTotalErrorEta");
            while (graphRelErrorEtaTot->GetY()[0] < 0  || graphRelErrorEtaTot->GetY()[0] == 0) graphRelErrorEtaTot->RemovePoint(0);

            TGraphAsymmErrors* graphRelErrorEtaStat       = CalculateRelErrUpAsymmGraph( graphNuclModFactorWeightedAverageEtaStat, "relativeStatErrorEta");
            while (graphRelErrorEtaStat->GetY()[0] < 0 || graphRelErrorEtaStat->GetY()[0] == 0) graphRelErrorEtaStat->RemovePoint(0);

            TGraphAsymmErrors* graphRelErrorEtaSys        = CalculateRelErrUpAsymmGraph( graphNuclModFactorWeightedAverageEtaSys, "relativeSysErrorEta");
            while (graphRelErrorEtaSys->GetY()[0] < 0 || graphRelErrorEtaSys->GetY()[0] == 0 ) graphRelErrorEtaSys->RemovePoint(0);

            const char *SysErrDatnameMeanSingleErrCheck = Form("%s/SystematicErrorAveragedSingle%s_EtaRpA_%s_Check.dat",outputDir.Data(),sysStringComb.Data(),optionEnergy.Data());
            fstream SysErrDatAverSingleCheck;
            SysErrDatAverSingleCheck.precision(4);
            cout << SysErrDatnameMeanSingleErrCheck << endl;
            if(sysAvailSingleEta[0]){
            SysErrDatAverSingleCheck.open(SysErrDatnameMeanSingleErrCheck, ios::out);
            SysErrDatAverSingleCheck << "pt \t Stat err \t sys err \t tot err " << endl;
            for (Int_t i = 0; i < graphRelErrorEtaTot->GetN(); i++){
                if (graphRelErrorEtaStat->GetY()[i] > 0) SysErrDatAverSingleCheck << graphRelErrorEtaStat->GetX()[i] << "\t" << graphRelErrorEtaStat->GetY()[i] <<"\t" << graphRelErrorEtaSys->GetY()[i] <<  "\t" << graphRelErrorEtaTot->GetY()[i] << endl;
            }
            SysErrDatAverSingleCheck << endl;
            SysErrDatAverSingleCheck.close();
            }

            // plot sys relative errors for individual triggers
            TCanvas* canvasRelSysErr            = new TCanvas("canvasRelSysErr","",200,10,1350,900);  // gives the page size
            DrawGammaCanvasSettings( canvasRelSysErr, 0.08, 0.02, 0.035, 0.09);

            TH2F * histo2DRelSysErr;
            histo2DRelSysErr                    = new TH2F("histo2DRelSysErr","histo2DRelSysErr",11000,0.,maxPtGlobalEta,1000,0,60.5);
            SetStyleHistoTH2ForGraphs(histo2DRelSysErr, "#it{p}_{T} (GeV/#it{c})","sys Err (%)",0.035,0.04, 0.035,0.04, 1.,1.);
            histo2DRelSysErr->Draw("copy");
                TLegend* legendRelSysErr        = GetAndSetLegend2(0.62, 0.92-(0.035*(nMeasSetEta+1)/2), 0.95, 0.92, 32);
                legendRelSysErr->SetNColumns(2);
                for (Int_t i = 0; i < nMeasSetEta; i++){
                    cout << "plotting graph: " << availableMeasEta[i] << "\t" <<graphRelSystEta[availableMeasEta[i]]->GetName() << endl;
                    DrawGammaSetMarkerTGraph(graphRelSystEta[availableMeasEta[i]], markerTriggWeighted[availableMeasEta[i]], sizeTrigg[availableMeasEta[i]],
                                            colorTriggWeighted[availableMeasEta[i]], colorTriggWeighted[availableMeasEta[i]]);
                    graphRelSystEta[availableMeasEta[i]]->Draw("p,same,z");
                    legendRelSysErr->AddEntry(graphRelSystEta[availableMeasEta[i]],nameTriggerWeighted[availableMeasEta[i]],"p");
                }
                legendRelSysErr->Draw();

                TLatex *labelRelErrEnergy    = new TLatex(0.15,0.89,collisionSystem.Data());
                SetStyleTLatex( labelRelErrEnergy, 0.85*textSizeLabelsPixel,4);
                labelRelErrEnergy->SetTextFont(43);
                labelRelErrEnergy->Draw();
                TLatex *labelRelErrEta       = new TLatex(0.15,0.85,"#pi^{0} #rightarrow #gamma#gamma");
                SetStyleTLatex( labelRelErrEta, 0.85*textSizeLabelsPixel,4);
                labelRelErrEta->SetTextFont(43);
                labelRelErrEta->Draw();
                TLatex *labelDetProcRelErr    = new TLatex(0.15, 0.81,detectionProcess.Data());
                SetStyleTLatex( labelDetProcRelErr, 0.85*textSizeLabelsPixel,4);
                labelDetProcRelErr->SetTextFont(43);
                labelDetProcRelErr->Draw();

            canvasRelSysErr->SaveAs(Form("%s/EtaRpA_RelSysErr_SingleMeas.%s",outputDir.Data(),suffix.Data()));


                DrawGammaSetMarkerTGraphAsym(graphRelErrorEtaSys, 24, 1.5, kGray+1 , kGray+1);
    //             graphRelErrorEtaSys->SetLineStyle(7);
                graphRelErrorEtaSys->Draw("same,pze1");
                legendRelSysErr->AddEntry(graphRelErrorEtaSys,"average","p");
                legendRelSysErr->Draw();

                labelRelErrEnergy->Draw();
                labelRelErrEta->Draw();
                labelDetProcRelErr->Draw();

            canvasRelSysErr->SaveAs(Form("%s/EtaRpA_RelSysErrWithAverage_SingleMeas.%s",outputDir.Data(),suffix.Data()));


            // plot stat relative errors for individual triggers
            TCanvas* canvasRelStatErr           = new TCanvas("canvasRelStatErr","",200,10,1350,900);  // gives the page size
            DrawGammaCanvasSettings( canvasRelStatErr, 0.08, 0.02, 0.035, 0.09);

            TH2F * histo2DRelStatErr;
            histo2DRelStatErr                   = new TH2F("histo2DRelStatErr","histo2DRelStatErr",11000,0.,maxPtGlobalEta,1000,0,60.5);
            SetStyleHistoTH2ForGraphs(histo2DRelStatErr, "#it{p}_{T} (GeV/#it{c})","stat Err (%)",0.035,0.04, 0.035,0.04, 1.,1.);
            histo2DRelStatErr->Draw("copy");

                TLegend* legendRelStatErr       = GetAndSetLegend2(0.62, 0.92-(0.035*nMeasSetEta/2), 0.95, 0.92, 32);
                legendRelStatErr->SetNColumns(2);
                for (Int_t i = 0; i < nMeasSetEta; i++){
                    cout << "plotting graph: " << availableMeasEta[i] << "\t" <<graphRelStatEta[availableMeasEta[i]]->GetName() << endl;
                    if (graphRelStatEta[availableMeasEta[i]] ){
    //                     TGraphAsymmErrors* dummyGraph = new TGraphAsymmErrors(histoRelStatEta[availableMeasEta[i]]);
                        graphRelStatEta[availableMeasEta[i]]->Print();
                        DrawGammaSetMarkerTGraph(graphRelStatEta[availableMeasEta[i]], markerTriggWeighted[availableMeasEta[i]], sizeTrigg[availableMeasEta[i]],
                                            colorTriggWeighted[availableMeasEta[i]], colorTriggWeighted[availableMeasEta[i]]);
                        graphRelStatEta[availableMeasEta[i]]->Draw("p,same");
                        legendRelStatErr->AddEntry(graphRelStatEta[availableMeasEta[i]],nameTriggerWeighted[availableMeasEta[i]],"p");

                        for (Int_t j = 1; j < histoRelStatEta[availableMeasEta[i]]->GetNbinsX()+1; j++){
                            cout << j << ": " << histoRelStatEta[availableMeasEta[i]]->GetBinContent(j) << endl;
                        }
                    } else if (histoRelStatEta[availableMeasEta[i]]) {
                        DrawGammaSetMarker(histoRelStatEta[availableMeasEta[i]],markerTriggWeighted[availableMeasEta[i]], sizeTrigg[availableMeasEta[i]],
                                                colorTriggWeighted[availableMeasEta[i]], colorTriggWeighted[availableMeasEta[i]]);
                        histoRelStatEta[availableMeasEta[i]]->DrawCopy("p,same,z");
                        legendRelStatErr->AddEntry(histoRelStatEta[availableMeasEta[i]],nameTriggerWeighted[availableMeasEta[i]],"p");
                    }
                }
                legendRelStatErr->Draw();

                labelRelErrEnergy->Draw();
                labelRelErrEta->Draw();
                labelDetProcRelErr->Draw();

            canvasRelStatErr->SaveAs(Form("%s/EtaRpA_RelStatErr_SingleMeas.%s",outputDir.Data(),suffix.Data()));

            // plot full error for final result decomposed
            TCanvas* canvasRelTotErr            = new TCanvas("canvasRelTotErr","",200,10,1350,900);  // gives the page size
            DrawGammaCanvasSettings( canvasRelTotErr, 0.08, 0.02, 0.035, 0.09);

            TH2F * histo2DRelTotErrEta;
            histo2DRelTotErrEta                 = new TH2F("histo2DRelTotErrEta","histo2DRelTotErrEta",11000,0.,maxPtGlobalEta,1000,0,40.5);
            SetStyleHistoTH2ForGraphs(histo2DRelTotErrEta, "#it{p}_{T} (GeV/#it{c})","Err (%)",0.035,0.04, 0.035,0.04, 1.,1.);
            histo2DRelTotErrEta->Draw("copy");

                DrawGammaSetMarkerTGraphAsym(graphRelErrorEtaTot, 20, 1.5, kBlack , kBlack);
                graphRelErrorEtaTot->Draw("p,same,z");
                DrawGammaSetMarkerTGraphAsym(graphRelErrorEtaStat, 24, 1.5, kGray+2 , kGray+2);
                graphRelErrorEtaStat->Draw("l,x0,same,e1");
                DrawGammaSetMarkerTGraphAsym(graphRelErrorEtaSys, 24, 1.5, kGray+1 , kGray+1);
                graphRelErrorEtaSys->SetLineStyle(7);
                graphRelErrorEtaSys->Draw("l,x0,same,e1");

                TLegend* legendRelTotErr2       = GetAndSetLegend2(0.72, 0.92-(0.035*3), 0.9, 0.92, 32);
                legendRelTotErr2->AddEntry(graphRelErrorEtaTot,"tot","p");
                legendRelTotErr2->AddEntry(graphRelErrorEtaStat,"stat","l");
                legendRelTotErr2->AddEntry(graphRelErrorEtaSys,"sys","l");
                legendRelTotErr2->Draw();

                labelRelErrEnergy->Draw();
                labelRelErrEta->Draw();
                labelDetProcRelErr->Draw();

            canvasRelTotErr->SaveAs(Form("%s/EtaRpA_RelErrorsFulldecomp.%s",outputDir.Data(),suffix.Data()));
            
        }


        // return;
        //***************************************************************************************************************
        //************************************Plotting nuclear modification factors  *************************************
        //***************************************************************************************************************
        cout << textSizeNuclModFac << endl;

        TCanvas* canvasNuclModFacEta = new TCanvas("canvasNuclModFacEta","",200,10,1350,900);
        DrawGammaCanvasSettings( canvasNuclModFacEta, 0.08, 0.01, 0.01, 0.12);

        Double_t minNuclModFacSpectraEta   = 0.0;
        Double_t maxNuclModFacSpectraEta   = 1.9;
        TH2F * histo2DNuclModFacEta = new TH2F("histo2DNuclModFacEta","histo2DNuclModFacEta",1000,0., maxPtGlobalEta,10000,minNuclModFacSpectraEta,maxNuclModFacSpectraEta);
        SetStyleHistoTH2ForGraphs(histo2DNuclModFacEta,  "#it{p}_{T} (GeV/#it{c})","#it{R}_{pA}",
                                0.85*textSizeNuclModFac,textSizeNuclModFac, 0.85*textSizeNuclModFac,textSizeNuclModFac, 0.9, 0.55, 510, 505);
        histo2DNuclModFacEta->GetXaxis()->SetMoreLogLabels();
        histo2DNuclModFacEta->GetXaxis()->SetNoExponent(kTRUE);
        histo2DNuclModFacEta->DrawCopy();
        TLegend* legendNuclModFacEta = GetAndSetLegend2(minXLegendModFac, minYLegendModFac, 0.95, maxYLegendModFac,0.85*textSizePixelNuclModFac);
        legendNuclModFacEta->SetNColumns(nColumnsModFac);
        for (Int_t i = 0; i< nrOfTrigToBeComb; i++){
            if (graphsNuclearModFactorSysEta[i])
                DrawGammaSetMarkerTGraphAsym(graphsNuclearModFactorSysEta[i], markerTrigg[i], sizeTrigg[i], colorTrigg[i], colorTrigg[i], 1, kTRUE);
            if (graphsNuclearModFactorEta[i])
                DrawGammaSetMarkerTGraphAsym(graphsNuclearModFactorEta[i], markerTrigg[i], sizeTrigg[i], colorTrigg[i], colorTrigg[i]);
            if ( !maskedFullyEta[i] )graphsNuclearModFactorEta[i]->Draw("e1,same");
            if ( !maskedFullyEta[i] && graphsNuclearModFactorSysEta[i])graphsNuclearModFactorSysEta[i]->Draw("p,E2,same");
            if ( !maskedFullyEta[i] )legendNuclModFacEta->AddEntry(graphsNuclearModFactorEta[i],triggerNameLabel[i].Data(),"p");
        }
        legendNuclModFacEta->Draw();

        DrawGammaLines(0., maxPtGlobalEta , 1, 1 ,1, kGray+1, 7);

        labelEnergyNuclModFac->Draw();

        TLatex *labelEtaNuclModFac = new TLatex(0.13, 0.91-textSizeNuclModFac*0.85*0.85,"#eta #rightarrow #gamma#gamma");
        SetStyleTLatex( labelEtaNuclModFac, 0.85*textSizeNuclModFac,4);
        labelEtaNuclModFac->Draw();
        labelDetProcNuclModFac->Draw();

        canvasNuclModFacEta->Update();
        canvasNuclModFacEta->SaveAs(Form("%s/Eta_%s_NuclModFactor_Triggers.%s",outputDir.Data(),isMC.Data(),suffix.Data()));

        histo2DNuclModFacEta->DrawCopy();
        for (Int_t i = 0; i< nrOfTrigToBeComb; i++){
            if (graphsNuclearModFactorSysEtaShrunk[i])
                DrawGammaSetMarkerTGraphAsym(graphsNuclearModFactorSysEtaShrunk[i], markerTrigg[i], sizeTrigg[i], colorTrigg[i], colorTrigg[i], 1, kTRUE);
            if (graphsNuclearModFactorEtaShrunk[i])
                DrawGammaSetMarkerTGraphAsym(graphsNuclearModFactorEtaShrunk[i], markerTrigg[i], sizeTrigg[i], colorTrigg[i], colorTrigg[i]);
            if ( !maskedFullyEta[i] )graphsNuclearModFactorEtaShrunk[i]->Draw("e1,same");
            if ( !maskedFullyEta[i] && graphsNuclearModFactorSysEtaShrunk[i])graphsNuclearModFactorSysEtaShrunk[i]->Draw("p,E2,same");
        }
        legendNuclModFacEta->Draw();

        DrawGammaLines(0., maxPtGlobalEta , 1, 1 ,1, kGray+1, 7);
        labelEnergyNuclModFac->Draw();
        labelEtaNuclModFac->Draw();
        labelDetProcNuclModFac->Draw();

        canvasNuclModFacEta->Update();
        canvasNuclModFacEta->SaveAs(Form("%s/Eta_%s_NuclModFactor_TriggersUsed.%s",outputDir.Data(),isMC.Data(),suffix.Data()));


        histo2DNuclModFacEta->DrawCopy();
        if (graphNuclModFactorWeightedAverageEtaSys){
            DrawGammaSetMarkerTGraphAsym(graphNuclModFactorWeightedAverageEtaSys, 24, 1.5, kBlack, kBlack, 1, kTRUE);
            graphNuclModFactorWeightedAverageEtaSys->Draw("p,E2,same");
        }
        if (graphNuclModFactorWeightedAverageEtaStat){
            DrawGammaSetMarkerTGraphAsym(graphNuclModFactorWeightedAverageEtaStat, 24, 1.5, kBlack, kBlack);
            graphNuclModFactorWeightedAverageEtaStat->Draw("e1,same");
        }

        DrawGammaLines(0., maxPtGlobalEta , 1, 1 ,1, kGray+1, 7);

        labelEnergyNuclModFac->Draw();
        labelEtaNuclModFac->Draw();
        labelDetProcNuclModFac->Draw();

        canvasNuclModFacEta->Update();
        canvasNuclModFacEta->SaveAs(Form("%s/Eta_%s_NuclModFactor.%s",outputDir.Data(),isMC.Data(),suffix.Data()));
    }





    TH1D* histoNuclModFactorWeightedAveragePi0Stat                   = NULL;
    if (graphNuclModFactorWeightedAveragePi0Stat){
        histoNuclModFactorWeightedAveragePi0Stat                  = new TH1D("histoNuclModFactorWeightedAveragePi0Stat", "", maxNAllowedPi0, binningPi0);
        Int_t firstBinPi0 = 1;
        while (histoNuclModFactorWeightedAveragePi0Stat->GetBinCenter(firstBinPi0) < graphNuclModFactorWeightedAveragePi0Stat->GetX()[0]){
            histoNuclModFactorWeightedAveragePi0Stat->SetBinContent(firstBinPi0, 0);
            histoNuclModFactorWeightedAveragePi0Stat->SetBinError(firstBinPi0, 0);
            firstBinPi0++;
        }
        for (Int_t i = 0; i < graphNuclModFactorWeightedAveragePi0Stat->GetN(); i++){
            histoNuclModFactorWeightedAveragePi0Stat->SetBinContent(i+firstBinPi0, graphNuclModFactorWeightedAveragePi0Stat->GetY()[i]);
            histoNuclModFactorWeightedAveragePi0Stat->SetBinError(i+firstBinPi0, graphNuclModFactorWeightedAveragePi0Stat->GetEYlow()[i]);
        }
    }
    TH1D* histoNuclModFactorWeightedAverageEtaStat                   = NULL;
    if (graphNuclModFactorWeightedAverageEtaStat){
        histoNuclModFactorWeightedAverageEtaStat                  = new TH1D("histoNuclModFactorWeightedAverageEtaStat", "", maxNAllowedEta, binningEta);
        Int_t firstBinEta = 1;
        while (histoNuclModFactorWeightedAverageEtaStat->GetBinCenter(firstBinEta) < graphNuclModFactorWeightedAverageEtaStat->GetX()[0]){
            histoNuclModFactorWeightedAverageEtaStat->SetBinContent(firstBinEta, 0);
            histoNuclModFactorWeightedAverageEtaStat->SetBinError(firstBinEta, 0);
            firstBinEta++;
        }
        for (Int_t i = 0; i < graphNuclModFactorWeightedAverageEtaStat->GetN(); i++){
            histoNuclModFactorWeightedAverageEtaStat->SetBinContent(i+firstBinEta, graphNuclModFactorWeightedAverageEtaStat->GetY()[i]);
            histoNuclModFactorWeightedAverageEtaStat->SetBinError(i+firstBinEta, graphNuclModFactorWeightedAverageEtaStat->GetEYlow()[i]);
        }
    }




    cout << "Writing output file" << endl;
    TString fileNameOutputComp  = Form("%s/%s_%sResultsFullCorrection_PP.root",outputDir.Data(),isMC.Data(),system.Data());
    if (optionEnergy.Contains("pPb"))
        fileNameOutputComp      = Form("%s/%s_%sResultsFullCorrection_pPb.root",outputDirDay.Data(),isMC.Data(),system.Data());
    else if (optionEnergy.Contains("PbPb"))
        fileNameOutputComp      = Form("%s/%s_%sResultsFullCorrection_PbPb.root",outputDirDay.Data(),isMC.Data(),system.Data());
    else if (optionEnergy.Contains("XeXe"))
        fileNameOutputComp      = Form("%s/%s_%sResultsFullCorrection_XeXe.root",outputDirDay.Data(),isMC.Data(),system.Data());

    TFile* fileOutputForComparisonFullyCorrected = new TFile(fileNameOutputComp,"UPDATE");
    if(fCent.Contains("ZNA ") || fCent.Contains("CL1 ")) fCent.Replace(0,4,"");
    fileOutputForComparisonFullyCorrected->mkdir(Form("Pi0%s%s%s",fCent.Data(),optionEnergy.Data(),centEstimator.Data()));
    TDirectoryFile* directoryPi0 = (TDirectoryFile*)fileOutputForComparisonFullyCorrected->Get(Form("Pi0%s%s%s",fCent.Data(),optionEnergy.Data(),centEstimator.Data()));
    fileOutputForComparisonFullyCorrected->cd(Form("Pi0%s%s%s",fCent.Data(),optionEnergy.Data(),centEstimator.Data()));

    if (histoNuclModFactorWeightedAveragePi0Stat)  histoNuclModFactorWeightedAveragePi0Stat->Write("histoNuclearModFactorPi0Stat",TObject::kOverwrite);
    if (graphNuclModFactorWeightedAveragePi0Stat)  graphNuclModFactorWeightedAveragePi0Stat->Write("graphNuclearModFactorPi0Stat",TObject::kOverwrite);
    if (graphNuclModFactorWeightedAveragePi0Tot)   graphNuclModFactorWeightedAveragePi0Tot->Write("graphNuclearModFactorPi0Tot",TObject::kOverwrite);
    if (graphNuclModFactorWeightedAveragePi0Sys)   graphNuclModFactorWeightedAveragePi0Sys->Write("graphNuclearModFactorPi0Sys",TObject::kOverwrite);

    for (Int_t i=0; i< nrOfTrigToBeComb; i++){
        cout << "trigger output Pi0: " << triggerName[i].Data() << endl;
        if (graphsNuclearModFactorPi0[i])     graphsNuclearModFactorPi0[i]->Write(Form("graphNuclearModFactorPi0Stat_%s",triggerName[i].Data()),TObject::kOverwrite);
        if (graphsNuclearModFactorSysPi0[i])  graphsNuclearModFactorSysPi0[i]->Write(Form("graphNuclearModFactorPi0Sys_%s",triggerName[i].Data()),TObject::kOverwrite);
    }

    if (enableEta && mode != 10){
        fileOutputForComparisonFullyCorrected->mkdir(Form("Eta%s%s%s",fCent.Data(),optionEnergy.Data(),centEstimator.Data()));
        TDirectoryFile* directoryEta = (TDirectoryFile*)fileOutputForComparisonFullyCorrected->Get(Form("Eta%s%s%s",fCent.Data(),optionEnergy.Data(),centEstimator.Data()));
        fileOutputForComparisonFullyCorrected->cd(Form("Eta%s%s%s",fCent.Data(),optionEnergy.Data(),centEstimator.Data()));

        if (histoNuclModFactorWeightedAverageEtaStat)  histoNuclModFactorWeightedAverageEtaStat->Write("histoNuclearModFactorEtaStat",TObject::kOverwrite);
        if (graphNuclModFactorWeightedAverageEtaStat)  graphNuclModFactorWeightedAverageEtaStat->Write("graphNuclearModFactorEtaStat",TObject::kOverwrite);
        if (graphNuclModFactorWeightedAverageEtaTot)   graphNuclModFactorWeightedAverageEtaTot->Write("graphNuclearModFactorEtaTot",TObject::kOverwrite);
        if (graphNuclModFactorWeightedAverageEtaSys)   graphNuclModFactorWeightedAverageEtaSys->Write("graphNuclearModFactorEtaSys",TObject::kOverwrite);

        for (Int_t i=0; i< nrOfTrigToBeComb; i++){
            cout << "trigger output Eta: " << triggerName[i].Data() << endl;
            if (graphsNuclearModFactorEta[i])     graphsNuclearModFactorEta[i]->Write(Form("graphNuclearModFactorEtaStat_%s",triggerName[i].Data()),TObject::kOverwrite);
            if (graphsNuclearModFactorSysEta[i])  graphsNuclearModFactorSysEta[i]->Write(Form("graphNuclearModFactorEtaSys_%s",triggerName[i].Data()),TObject::kOverwrite);
        }
    }

    fileOutputForComparisonFullyCorrected->Write();
    fileOutputForComparisonFullyCorrected->Close();
}
