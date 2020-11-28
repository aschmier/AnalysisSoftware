//***********************************************************************************************
//**************************** WeightStudiesOverview ***********************************************
//***********************************************************************************************
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

struct SysErrorConversion {
   Double_t value;
   Double_t error;
   //    TString name;
};


void WeightStudiesOverview(TString CombineFilesName             = "CombineCuts.dat",
                        TString suffix                          = "gif",
                        TString optionEnergy                    = "",
                        TString cutVariationName                = "",
                        Int_t NumberOfCuts                      = 1,
                        Int_t mode                              = 9,
                        Int_t sequence                          = 0,
                        Bool_t doBarlow                         = kFALSE
                       ){

  // order of cuts should be if sequence =
  // 0: Onfly Pythia (default), Onfly Phojet, Offline Pythia, Offline Phojet
  // 1: Onfly Pythia 13TeV, Onfly Phojet 13TeV, Onfly Pythia 5TeV, Onfly Phojet 5TeV, idem with Offline
  // 2: Onfly Pythia 5TeV2017, Offline Pythia 5TeV2017, Onfly Pythia 5TeV2015, Offline Pythia 5TeV2015

    gROOT->Reset();
    gROOT->SetStyle("Plain");
    StyleSettingsThesis();
    SetPlotStyle();


    // Define global arrays
    TString     cutNumber               [50];
    TString     cutNumberAdv            [50];

    // Set common default plot style
    StyleSettingsThesis();
    SetPlotStyle();
    Size_t sizeTextNameBins              = 0.05;
   
    TString arrayRangesRBins[12]      = { "0 cm < R #leq 1.5 cm",     //0
					  "1.5 cm < R #leq 5. cm",    //1
					  "5. cm < R #leq 8.5 cm",    //2
					  "8.5 cm < R #leq 13 cm",    //3
					  "13 cm < R #leq 21 cm",     //4
					  "21 cm < R #leq 33.5 cm",   //5
					  "33.5 cm < R #leq 41 cm",   //6
					  "41 cm < R #leq 55 cm",     //7
					  "55 cm < R #leq 72 cm",     //8
					  "72 cm < R #leq 95 cm",     //9
					  "95 cm < R #leq 145 cm",    //10
					  "145 cm < R #leq 180 cm"    //11
    };

    // Set cutvariation-name to "" for no explicit name
    if (cutVariationName.CompareTo("None")==0) cutVariationName = "";

    // Define Output Directory
    TString outputDir                                    = Form("WeightStudies/%s",optionEnergy.Data());
    if (cutVariationName.CompareTo("None")!=0) outputDir = Form("WeightStudies/%s/%s",optionEnergy.Data(),cutVariationName.Data());
    TString outputDirRootFile                            = Form("WeightStudies/%s",optionEnergy.Data());
    gSystem->Exec("mkdir -p "+outputDir);

    Double_t maxR = 180.;
    // Define colors for differnt cuts
    Color_t color[12] = { kBlack, kAzure, kGreen+2, kOrange+2,
                          kRed+1, kCyan+2, kYellow+2, kViolet-3,
                          kSpring+10, kMagenta-8, kGray, kGray+3};

    TString label5cm = "#rightarrow R = 5 cm";

    // Set collisions system
    TString collisionSystem     = ReturnFullCollisionsSystem(optionEnergy);
    if (collisionSystem.CompareTo("") == 0){
        cout << "No correct collision system specification, has been given" << endl;
        return;
    }
    TString detectionProcess    = ReturnFullTextReconstructionProcess(mode);

    // Define necessary histogram/file/string arrays
    const Int_t ConstNumberOfCuts = NumberOfCuts;
    const Int_t MaxNumberOfCuts = 12;
    const Int_t nBinsR = 12;
    if(ConstNumberOfCuts > MaxNumberOfCuts){
        cout << "Too many cuts, beware!" << endl;
        return;
    }

    Double_t ptMinFitPowerLaw[nBinsR]= {0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.2,0.15,0.15,0.15};
    TF1* fitToRatio;            
    Int_t fitStatus;


    TString FileNames[MaxNumberOfCuts];
    TString FileNamesAdditional[MaxNumberOfCuts];

    // Read cuts from CutSelection file
    ifstream in(CombineFilesName.Data());
    cout<<"Available Cuts:"<<endl;
    string TempFile;
    Int_t Number = 0;
    while(getline(in, TempFile)){
        TString tempFile                                = TempFile;
        cout<< tempFile<<endl;
        FileNames[Number]=tempFile;
        cutNumber[Number]=tempFile;
        cutNumber[Number].Remove(0,32);
        FileNamesAdditional[Number] = Form("AdditionalMBHistos%s",cutNumber[Number].Data());
        cout<< cutNumber[Number].Length()-5<< endl;
        cutNumber[Number].Remove(cutNumber[Number].Length()-5,5);
        cout<<	FileNames[Number]<< "  " << cutNumber[Number].Data() << endl;
	cout<<  FileNamesAdditional[Number] << endl;
        Number++;
    }

    TString period[50];
    TString periodName[50];
    TString clusterName[50];
    TString generatorName[50];
    TString cutString[50];
    TString V0ReaderName[50];
    Marker_t marker[50];
    Int_t counterOnfly = 0;
    Int_t counterOffline = 0;
    for(Int_t i=0; i<Number; i++){
        TObjArray *arr;
        arr = cutNumber[i].Tokenize("_");
        TObjString* string1;
        TObjString* string2;
        TObjString* string3;
        TObjString* string4;
        string1 = (TObjString*)arr->At(0);
        period[i] = string1->GetString();
        if(period[i].Contains("LHC16")) periodName[i] = "13 TeV (Ar)";
        else if(period[i].Contains("LHC17")) periodName[i] = "  5 TeV (Ne)";
        else periodName[i] = "  5 TeV (Ar)";
        string2 = (TObjString*)arr->At(1);
        clusterName[i] = string2->GetString();
        if(clusterName[i].Contains("fast") || clusterName[i].Contains("wSDD") || clusterName[i].Contains("woSDD")){
            string3 = (TObjString*)arr->At(2);
            string4 = (TObjString*)arr->At(4);
        } else {
            clusterName[i] = "";
            string3 = (TObjString*)arr->At(1);
            string4 = (TObjString*)arr->At(3);
        }
        generatorName[i] = string3->GetString();
        cutString[i] = string4->GetString();
        TString fV0Reader                                   = cutString[i](GetPhotonV0FinderCutPosition(cutString[i]),1);
        V0ReaderName[i]                                   = AnalyseV0ReaderCut(CutNumberToInteger(fV0Reader));
        if(V0ReaderName[i].Contains("Onfly")){
            V0ReaderName[i] = "On-the-Fly V0";
            marker[i] = 20;
            counterOnfly++;
        } else if (V0ReaderName[i].Contains("Offline")){
            V0ReaderName[i] = "Offline V0";
            marker[i] = 21;
            counterOffline++;
        }

	//	cout << "period " << periodName[i] << " " << clusterName[i] << endl;
	//        cout << "generator " << generatorName[i] << endl;
	//	cout << "V0Reader " << V0ReaderName[i] << endl;
    }

    cout<<"=========================="<<endl;
    cout << "analysing " << cutVariationName << " cut variations" << endl;
    cout << " " << endl;
    TFile*  WeightFile                 [ConstNumberOfCuts];
    TProfile* profileWeight            [ConstNumberOfCuts];
    TH1F * histWeight                  [ConstNumberOfCuts];
    TH1F * histoRatioWeightCut         [ConstNumberOfCuts];
    TH1F * histoRatioWeightEnergyCut   [ConstNumberOfCuts];
    TH1F * histoDiffWeightCut          [ConstNumberOfCuts];
    TH1F * histRelUncWeightCut         [ConstNumberOfCuts];
    TH1F * rData [ConstNumberOfCuts];

    TFile*  WeightFileAdditional       [ConstNumberOfCuts];
    TH1F * histoWeightsEachRPtMin[nBinsR][ConstNumberOfCuts];
    TH1F * histoWeightsEachRPtMinSecSub[nBinsR][ConstNumberOfCuts];   // Still a test
    TH1F * histoWeightsEachRPtMinSecSubUsingCocktail[nBinsR][ConstNumberOfCuts];   // Still a test
    TH1F * histoWeightsEachRPtDiffSecSubUsingCocktail[nBinsR][ConstNumberOfCuts];  
    TH1F * histoPurityPtEachRBin[nBinsR][ConstNumberOfCuts];
    TH1F * histoTrueRecEffEachRBin[nBinsR][ConstNumberOfCuts];
    TH1F * histoWeightNch              [ConstNumberOfCuts];
    TH1F * histoWeightNchPtBin1        [ConstNumberOfCuts];
    TH1F * histoWeightNchPtBin2        [ConstNumberOfCuts];
    TH1F * histoWeightNchPtBin3        [ConstNumberOfCuts];
    TH1F * histoRatioWeightNchCut      [ConstNumberOfCuts];
    TH1F * histoRatioWeightNchEnergyCut[ConstNumberOfCuts];
    TH1F * histoDiffWeightNchCut       [ConstNumberOfCuts];



    for (Int_t i=0; i< NumberOfCuts; i++){

        WeightFile[i] = TFile::Open(FileNames[i]);
        profileWeight[i]=  (TProfile*)WeightFile[i]->Get("profileContainingMaterialBudgetWeights_manyRadialBins");
	//histWeight[i]   = (TH1F*)WeightFile[i]->Get("histoDataMCRatioRinPtBinScaledToGasPtBin3");
	//AM: Take the scondary subtracted histogram
        histWeight[i]   = (TH1F*)WeightFile[i]->Get("histoDataMCRatioRScaledToGasSecSubPtBin3");
        rData[i]        = (TH1F*)WeightFile[i]->Get("Data");

	WeightFileAdditional[i] = TFile::Open(FileNamesAdditional[i]);

        // Calculate ratios
        histoRatioWeightCut[i] = (TH1F*) histWeight[i]->Clone("histoRatioWeights");
        histoRatioWeightCut[i]->Sumw2();
        histoDiffWeightCut[i] = (TH1F*)histWeight[i]->Clone("histoDiffWeights");
        histoDiffWeightCut[i]->Sumw2();
        histoRatioWeightEnergyCut[i] = (TH1F*) histWeight[i]->Clone("histoRatioWeightsByEnergy");
        histoRatioWeightEnergyCut[i]->Sumw2();

	// Read histograms from additional file
	for (Int_t j=0; j< nBinsR; j++){
	  //	  cout<< "name:"<< Form("histoWeightsEachRPtMin%i",j)<< endl;
	  histoWeightsEachRPtMin[j][i] = (TH1F*)WeightFileAdditional[i]->Get(Form("histoWeightsEachRPtMin%i",j));
	  histoWeightsEachRPtMinSecSub[j][i] = (TH1F*)WeightFileAdditional[i]->Get(Form("histoWeightsEachRPtMinSecSub%i",j));
	  histoWeightsEachRPtMinSecSubUsingCocktail[j][i] = (TH1F*)WeightFileAdditional[i]->Get(Form("histoWeightsEachRPtMinSecSubUsingCocktail%i",j));
	  histoWeightsEachRPtDiffSecSubUsingCocktail[j][i] = (TH1F*)WeightFileAdditional[i]->Get(Form("histoWeightsEachRPtDiffSecSubUsingCocktail%i",j));

	  histoPurityPtEachRBin[j][i] = (TH1F*)WeightFileAdditional[i]->Get(Form("histoPurityPtEachRBin_%i",j)); ;
	  histoTrueRecEffEachRBin[j][i] = (TH1F*)WeightFileAdditional[i]->Get(Form("histoTrueRecEffPtEachRBin_%i",j)); ;
	  //	  cout<< "mean" <<histoWeightsEachRPtMin[i][j]->GetMean() <<endl;
	}
        histoWeightNch[i]   = (TH1F*)WeightFileAdditional[i]->Get("histoDataMCRatioRRebinFullRange");
        histoWeightNchPtBin1[i]   = (TH1F*)WeightFileAdditional[i]->Get("histoDataMCRatioRRebinPtBin1");
        histoWeightNchPtBin2[i]   = (TH1F*)WeightFileAdditional[i]->Get("histoDataMCRatioRRebinPtBin2");
        histoWeightNchPtBin3[i]   = (TH1F*)WeightFileAdditional[i]->Get("histoDataMCRatioRRebinPtBin3");
        histoRatioWeightNchCut[i] = (TH1F*) histoWeightNch[i]->Clone("histoRatioWeightsNch");
        histoRatioWeightNchCut[i]->Sumw2();
        histoDiffWeightNchCut[i] = (TH1F*)histoWeightNch[i]->Clone("histoDiffWeightsNch");
        histoDiffWeightNchCut[i]->Sumw2();

        histoRatioWeightNchEnergyCut[i] = (TH1F*) histoWeightNch[i]->Clone("histoRatioWeightsNchByEnergy");
        histoRatioWeightNchEnergyCut[i]->Sumw2();


        if(sequence==0){
            if (i != 3){
                    histoRatioWeightCut[i]->Sumw2();
                    histoRatioWeightCut[i]->Divide(histWeight[i],histWeight[0],1.,1.,"");

                    //      histoDiffWeightCut[i]         = (TH1D*) histWeight[i]->Clone(Form("histoDiffWeights%d",i));
                    histoDiffWeightCut[i] = (TH1F*)histWeight[i]->Clone("histoDiffWeights");
                    histoDiffWeightCut[i]->Sumw2();
                    histoDiffWeightCut[i]->Add(histWeight[0],-1.);

		    histoRatioWeightNchCut[i]->Sumw2();
                    histoRatioWeightNchCut[i]->Divide(histoWeightNch[i],histoWeightNch[0],1.,1.,"");
		    histoDiffWeightNchCut[i] = (TH1F*)histoWeightNch[i]->Clone("histoDiffWeights");
                    histoDiffWeightNchCut[i]->Sumw2();
                    histoDiffWeightNchCut[i]->Add(histoWeightNch[0],-1.);



            } else {
                    histoRatioWeightCut[i]->Sumw2();
                    histoRatioWeightCut[i]->Divide(histWeight[i],histWeight[2],1.,1.,"");

                    //      histoDiffWeightCut[i]         = (TH1D*) histWeight[i]->Clone(Form("histoDiffWeights%d",i));
                    histoDiffWeightCut[i] = (TH1F*)histWeight[i]->Clone("histoDiffWeights");
                    histoDiffWeightCut[i]->Sumw2();
                    histoDiffWeightCut[i]->Add(histWeight[2],-1.);

                    histoRatioWeightNchCut[i]->Sumw2();
                    histoRatioWeightNchCut[i]->Divide(histoWeightNch[i],histoWeightNch[2],1.,1.,"");
                    histoDiffWeightNchCut[i] = (TH1F*)histoWeightNch[i]->Clone("histoDiffWeightsNch");
                    histoDiffWeightNchCut[i]->Sumw2();
                    histoDiffWeightNchCut[i]->Add(histoWeightNch[2],-1.);


            }

        } else if(sequence==1){
	  //default
            if ( i <= 4 || V0ReaderName[i].Contains("On-the-Fly") ){
                histoRatioWeightCut[i]->Divide(histWeight[i],histWeight[0],1.,1.,"");
                histoDiffWeightCut[i]->Add(histWeight[0],-1.);
	    	histoRatioWeightNchCut[i]->Divide(histoWeightNch[i],histoWeightNch[0],1.,1.,"");
	    	histoDiffWeightNchCut[i]->Add(histoWeightNch[0],-1.);
            } else if( i > 4 && V0ReaderName[i].Contains("Offline") ){
                histoRatioWeightCut[i]->Divide(histWeight[i],histWeight[4],1.,1.,"");
                histoDiffWeightCut[i]->Add(histWeight[4],-1.);
                histoRatioWeightNchCut[i]->Divide(histoWeightNch[i],histoWeightNch[4],1.,1.,"");
                histoDiffWeightNchCut[i]->Add(histoWeightNch[4],-1.);
            }


            // if ( i == 0  ){
            //     histoRatioWeightCut[i]->Divide(histWeight[i],histWeight[0],1.,1.,"");
            //     histoDiffWeightCut[i]->Add(histWeight[0],-1.);
	    // 	histoRatioWeightNchCut[i]->Divide(histoWeightNch[i],histoWeightNch[0],1.,1.,"");
	    // 	histoDiffWeightNchCut[i]->Add(histoWeightNch[0],-1.);
	    // }	else if ( i == 1  ){
            //     histoRatioWeightCut[i]->Divide(histWeight[i],histWeight[1],1.,1.,"");
            //     histoDiffWeightCut[i]->Add(histWeight[0],-1.);
	    // 	histoRatioWeightNchCut[i]->Divide(histoWeightNch[i],histoWeightNch[1],1.,1.,"");
	    // 	histoDiffWeightNchCut[i]->Add(histoWeightNch[0],-1.);

	    // }	else if ( i == 2 ){
            //     histoRatioWeightCut[i]->Divide(histWeight[i],histWeight[0],1.,1.,"");
            //     histoDiffWeightCut[i]->Add(histWeight[0],-1.);
	    // 	histoRatioWeightNchCut[i]->Divide(histoWeightNch[i],histoWeightNch[0],1.,1.,"");
	    // 	histoDiffWeightNchCut[i]->Add(histoWeightNch[0],-1.);
	    // }	else if ( i == 3 ){
            //     histoRatioWeightCut[i]->Divide(histWeight[i],histWeight[1],1.,1.,"");
            //     histoDiffWeightCut[i]->Add(histWeight[0],-1.);
	    // 	histoRatioWeightNchCut[i]->Divide(histoWeightNch[i],histoWeightNch[1],1.,1.,"");
	    // 	histoDiffWeightNchCut[i]->Add(histoWeightNch[0],-1.);
	    // }	else if ( i == 4 ){
            //     histoRatioWeightCut[i]->Divide(histWeight[i],histWeight[4],1.,1.,"");
            //     histoDiffWeightCut[i]->Add(histWeight[0],-1.);
	    // 	histoRatioWeightNchCut[i]->Divide(histoWeightNch[i],histoWeightNch[4],1.,1.,"");
	    // 	histoDiffWeightNchCut[i]->Add(histoWeightNch[0],-1.);
	    // }	else if ( i == 5 ){
            //     histoRatioWeightCut[i]->Divide(histWeight[i],histWeight[5],1.,1.,"");
            //     histoDiffWeightCut[i]->Add(histWeight[0],-1.);
	    // 	histoRatioWeightNchCut[i]->Divide(histoWeightNch[i],histoWeightNch[5],1.,1.,"");
	    // 	histoDiffWeightNchCut[i]->Add(histoWeightNch[0],-1.);
	    // }	else if ( i == 6 ){
            //     histoRatioWeightCut[i]->Divide(histWeight[i],histWeight[4],1.,1.,"");
            //     histoDiffWeightCut[i]->Add(histWeight[0],-1.);
	    // 	histoRatioWeightNchCut[i]->Divide(histoWeightNch[i],histoWeightNch[4],1.,1.,"");
	    // 	histoDiffWeightNchCut[i]->Add(histoWeightNch[0],-1.);
	    // }	else if ( i == 7 ){
            //     histoRatioWeightCut[i]->Divide(histWeight[i],histWeight[5],1.,1.,"");
            //     histoDiffWeightCut[i]->Add(histWeight[0],-1.);
	    // 	histoRatioWeightNchCut[i]->Divide(histoWeightNch[i],histoWeightNch[5],1.,1.,"");
	    // 	histoDiffWeightNchCut[i]->Add(histoWeightNch[0],-1.);
	    // }



            if(period[i].Contains("LHC16")){
                cout << i <<  " 13 TeV" << endl;
                if ( i != 5 || V0ReaderName[i].Contains("On-the-Fly") ){
                    histoRatioWeightEnergyCut[i]->Divide(histWeight[i],histWeight[0],1.,1.,"");
                    histoRatioWeightNchEnergyCut[i]->Divide(histoWeightNch[i],histoWeightNch[0],1.,1.,"");
                } else if( i == 5 && V0ReaderName[i].Contains("Offline") ){
                    histoRatioWeightEnergyCut[i]->Divide(histWeight[i],histWeight[4],1.,1.,"");
                    histoRatioWeightNchEnergyCut[i]->Divide(histoWeightNch[i],histoWeightNch[4],1.,1.,"");
		}
            } else if(period[i].Contains("LHC17")){
                cout << i <<  " 5 TeV" << endl;
                if ( i != 7 || V0ReaderName[i].Contains("On-the-Fly") ){
                    histoRatioWeightEnergyCut[i]->Divide(histWeight[i],histWeight[2],1.,1.,"");
                    histoRatioWeightNchEnergyCut[i]->Divide(histoWeightNch[i],histoWeightNch[2],1.,1.,"");
                }else if( i == 7 && V0ReaderName[i].Contains("Offline") ){
                    histoRatioWeightEnergyCut[i]->Divide(histWeight[i],histWeight[6],1.,1.,"");
                    histoRatioWeightNchEnergyCut[i]->Divide(histoWeightNch[i],histoWeightNch[6],1.,1.,"");
		}
            }

        } else if(sequence==2){

            if (V0ReaderName[i].Contains("On-the-Fly") ){
                histoRatioWeightCut[i]->Divide(histWeight[i],histWeight[0],1.,1.,"");
                histoDiffWeightCut[i]->Add(histWeight[0],-1.);
                histoRatioWeightNchCut[i]->Divide(histoWeightNch[i],histoWeightNch[0],1.,1.,"");
                histoDiffWeightNchCut[i]->Add(histoWeightNch[0],-1.);
            } else if(V0ReaderName[i].Contains("Offline") ){
                histoRatioWeightCut[i]->Divide(histWeight[i],histWeight[4],1.,1.,"");
                histoDiffWeightCut[i]->Add(histWeight[2],-1.);
                histoRatioWeightNchCut[i]->Divide(histoWeightNch[i],histoWeightNch[4],1.,1.,"");
                histoDiffWeightNchCut[i]->Add(histoWeightNch[2],-1.);
            }
            if(period[i].Contains("LHC17")){
                cout << i <<  " 5 TeV" << endl;
                if ( i != 7 || V0ReaderName[i].Contains("On-the-Fly") ){
                    histoRatioWeightEnergyCut[i]->Divide(histWeight[i],histWeight[2],1.,1.,"");
		    histoRatioWeightNchEnergyCut[i]->Divide(histoWeightNch[i],histoWeightNch[2],1.,1.,"");
                 }else if( i == 7 && V0ReaderName[i].Contains("Offline") ){
                    histoRatioWeightEnergyCut[i]->Divide(histWeight[i],histWeight[6],1.,1.,"");
                    histoRatioWeightNchEnergyCut[i]->Divide(histoWeightNch[i],histoWeightNch[6],1.,1.,"");
		}
            } else  if(period[i].Contains("LHC15")){
                cout << i <<  " 5 TeV" << endl;
                if ( i != 4 || V0ReaderName[i].Contains("On-the-Fly") ){
                    histoRatioWeightEnergyCut[i]->Divide(histWeight[i],histWeight[0],1.,1.,"");
                    histoRatioWeightNchEnergyCut[i]->Divide(histoWeightNch[i],histoWeightNch[0],1.,1.,"");
		}else if( i == 4 && V0ReaderName[i].Contains("Offline") ){
                    histoRatioWeightEnergyCut[i]->Divide(histWeight[i],histWeight[1],1.,1.,"");
                    histoRatioWeightNchEnergyCut[i]->Divide(histoWeightNch[i],histoWeightNch[1],1.,1.,"");
		}
            }

        }

        histRelUncWeightCut[i] = (TH1F*)histWeight[i]->Clone("histRelUncWeight");
        for (Int_t k = 1; k < histRelUncWeightCut[i]->GetNbinsX()+1; k++){
            if (histRelUncWeightCut[i]->GetBinContent(k) != 0){
                histRelUncWeightCut[i]->SetBinContent(k, histRelUncWeightCut[i]->GetBinError(k)/histRelUncWeightCut[i]->GetBinContent(k)*100);
                histRelUncWeightCut[i]->SetBinError(k, 0);
            } else {
                histRelUncWeightCut[i]->SetBinContent(k, 0);
                histRelUncWeightCut[i]->SetBinError(k, 0);
            }
        }

    }
    cout << "line " << __LINE__ << endl;

    // Calculation of total systematics per bin
    Double_t totalWeightError[histoDiffWeightCut[0]->GetNbinsX()];
    Double_t totalWeightSys[histoDiffWeightCut[0]->GetNbinsX()];
    Double_t weightSta[histoDiffWeightCut[0]->GetNbinsX()];
    Double_t weightValue[histoDiffWeightCut[0]->GetNbinsX()];
    Double_t xError[histoDiffWeightCut[0]->GetNbinsX()];
 
    for (Int_t j=1; j<histoDiffWeightCut[0]->GetNbinsX()+1; j++){
        weightValue[j-1] = histWeight[0]->GetBinContent(j);
        weightSta[j-1] = histWeight[0]->GetBinError(j);
        if (histoDiffWeightCut[1]->GetBinCenter(j) < 5){
            totalWeightSys[j-1] = TMath::Sqrt(histoDiffWeightCut[1]->GetBinContent(j)* histoDiffWeightCut[1]->GetBinContent(j));
        } else if(histoDiffWeightCut[1]->GetBinCenter(j) > 5 &&  histoDiffWeightCut[1]->GetBinCenter(j) < 13) {
            totalWeightSys[j-1] = TMath::Sqrt(1.*histoDiffWeightCut[1]->GetBinContent(j)* histoDiffWeightCut[1]->GetBinContent(j) +
                                              1.*histoDiffWeightCut[2]->GetBinContent(j)* histoDiffWeightCut[2]->GetBinContent(j));
        } else {
        //					histoDiffWeightCut[2]->GetBinContent(j)* histoDiffWeightCut[2]->GetBinContent(j));
            totalWeightSys[j-1] = TMath::Sqrt(histoDiffWeightCut[1]->GetBinContent(j)* histoDiffWeightCut[1]->GetBinContent(j) +
                                              histoDiffWeightCut[2]->GetBinContent(j)* histoDiffWeightCut[2]->GetBinContent(j));
        }
        totalWeightError[j-1] = TMath::Sqrt( weightSta[j-1]* weightSta[j-1]+ totalWeightSys[j-1] *totalWeightSys[j-1] );

	std::cout << std::setprecision(4)  << "R = " << histoDiffWeightCut[1]->GetBinCenter(j) 
		  << "cm, Weight = "<< std::setprecision(4) << weightValue[j-1] 
		  << ", Stat = "    << std::setprecision(3) << weightSta[j-1] 
		  << " (Stat = "    << std::setprecision(3) << (weightSta[j-1]/histWeight[0]->GetBinContent(j))*100 
		  << "%), Sys = "   << std::setprecision(3) << totalWeightSys[j-1] 
		  << " (Sys = "     << std::setprecision(3) << (totalWeightSys[j-1]/histWeight[0]->GetBinContent(j))*100 
		  << "%), Tot = "   << std::setprecision(3) << totalWeightError[j-1] 
		  << " (Tot = "     << std::setprecision(3) << (totalWeightError[j-1]/histWeight[0]->GetBinContent(j))*100 << "%)" << endl ;
    }

    cout << "line " << __LINE__ << endl;
    // Calculation of average sys. for 1 photon based on the R weight beyond 5 cm
    Int_t       nBins = histWeight[0]->GetNbinsX();
    Double_t*   binsR = new Double_t[nBins+1];
    for (Int_t i=0; i<nBins+1; i++) {
        if (i<nBins)    binsR[i]   = histWeight[0]->GetXaxis()->GetBinLowEdge(i+1);
        else            binsR[i]   = histWeight[0]->GetXaxis()->GetBinUpEdge(i);
    }
    cout << "line " << __LINE__ << endl;
    TH1F * histWeightStaErr = new TH1F("weightStaErr","weightStaErr",nBins,binsR);
    TH1F * histWeightSysErr = new TH1F("weightSysErr","weightSysErr",nBins,binsR);
    TH1F * histWeightTotErr = new TH1F("weightTotErr","weightTotErr",nBins,binsR);
    TProfile* fProfileContainingMaterialBudgetWeights = new TProfile("profileContainingMaterialBudgetWeights_manyRadialBins","profileContainingMaterialBudgetWeights_manyRadialBins",nBins,binsR);
    TProfile* fProfileContainingMaterialBudgetWeightsUpTotErr = new TProfile("profileContainingMaterialBudgetWeights_manyRadialBinsUpTotErr","profileContainingMaterialBudgetWeights_manyRadialBinsUpTotErr",nBins,binsR);
    TProfile* fProfileContainingMaterialBudgetWeightsDownTotErr = new TProfile("profileContainingMaterialBudgetWeights_manyRadialBinsDownTotErr","profileContainingMaterialBudgetWeights_manyRadialBinsDownTotErr",nBins,binsR);

    for (Int_t j=1; j<histoDiffWeightCut[0]->GetNbinsX()+1; j++){
        histWeightStaErr->SetBinContent(j,weightValue[j-1]);
        histWeightStaErr->SetBinError(j,weightSta[j-1]);

        histWeightSysErr->SetBinContent(j,weightValue[j-1]);
        histWeightSysErr->SetBinError(j,totalWeightSys[j-1]);

        histWeightTotErr->SetBinContent(j,weightValue[j-1]);
        histWeightTotErr->SetBinError(j,totalWeightError[j-1]);
        fProfileContainingMaterialBudgetWeights->Fill(binsR[j-1], weightValue[j-1]);
        fProfileContainingMaterialBudgetWeightsUpTotErr->Fill(binsR[j-1], weightValue[j-1]+totalWeightError[j-1]);
        fProfileContainingMaterialBudgetWeightsDownTotErr->Fill(binsR[j-1], weightValue[j-1]-totalWeightError[j-1]);
    }

    // cout<< "Bins::"<< rData[0]->GetXaxis()->FindBin(5.+0.001)<< endl;
    // cout<< "Bins::"<< rData[0]->GetXaxis()->FindBin(180.-0.001)<< endl;
    // cout << "Bin width::"<<     rData[0]->GetBinWidth(1)<< " " <<rData[0]->GetBinWidth(10)<< " " << endl;
    Double_t totInRRange = rData[0]->Integral(rData[0]->GetXaxis()->FindBin(5.+0.001),rData[0]->GetXaxis()->FindBin(180.-0.001),"width");
    Double_t averageWeightSys=0.;
    Double_t averageWeightSta=0.;
    Double_t averageWeightTot=0.;
    Double_t averageWeight=0.;
    Int_t rminBin=0;
    Int_t rmaxBin=0;

    for (Int_t j=2; j<nBins; j++){

        rminBin=rData[0]->GetXaxis()->FindBin(binsR[j]+0.0001);
        rmaxBin=rData[0]->GetXaxis()->FindBin(binsR[j+1]-0.0001);
        Double_t totInRBin= rData[0]->Integral(rminBin,rmaxBin,"width");
        //     cout<<"R Limits"<< binsR[j] << "  " << binsR[j+1]<< " " << rminBin<<"  " << rmaxBin<< " " << totInRBin  << "  " << totInRRange<< "  " << histWeight[0]->GetBinContent(j+1)<< endl;
        //      cout<<"R Limits::"<<  binsR[j] << "  " << binsR[j+1]<< " " << totInRBin  << "  " << totInRBin/totInRRange<< "  "<<  histWeight[0]->GetBinContent(j+1)<< endl;
        averageWeightSta+=weightSta[j]*(totInRBin/totInRRange);
        averageWeightSys+=totalWeightSys[j]*(totInRBin/totInRRange);
        averageWeightTot+=totalWeightError[j]*(totInRBin/totInRRange);
        averageWeight+=weightValue[j] *(totInRBin/totInRRange);
    }
    std::cout<< std::setprecision(4)  << "Average Weight = " << averageWeight << ", Stat = " << (averageWeightSta/averageWeight)*100 << "%, Sys = " << (averageWeightSys/averageWeight)*100 << "%, Tot = " << (averageWeightTot/averageWeight)*100 << "%" << endl;

    cout << "line " << __LINE__ << endl;
    cout << " " << endl;


    // Nch 
   // Calculation of total systematics per bin
    Double_t totalWeightNchError[histoDiffWeightCut[0]->GetNbinsX()];
    Double_t totalWeightNchSys[histoDiffWeightCut[0]->GetNbinsX()];
    Double_t weightNchSta[histoDiffWeightCut[0]->GetNbinsX()];
    Double_t weightNchValue[histoDiffWeightCut[0]->GetNbinsX()];
    Double_t xErrorNch[histoDiffWeightCut[0]->GetNbinsX()];
 
    for (Int_t j=1; j<histoDiffWeightNchCut[0]->GetNbinsX()+1; j++){
        weightNchValue[j-1] = histoWeightNch[0]->GetBinContent(j);
        weightNchSta[j-1] = histoWeightNch[0]->GetBinError(j);
        if (histoDiffWeightNchCut[1]->GetBinCenter(j) < 5){
            totalWeightSys[j-1] = TMath::Sqrt(histoDiffWeightNchCut[1]->GetBinContent(j)* histoDiffWeightNchCut[1]->GetBinContent(j));
        } else if(histoDiffWeightNchCut[1]->GetBinCenter(j) > 5 &&  histoDiffWeightNchCut[1]->GetBinCenter(j) < 13) {
            totalWeightNchSys[j-1] = TMath::Sqrt(1.*histoDiffWeightNchCut[1]->GetBinContent(j)* histoDiffWeightNchCut[1]->GetBinContent(j) +
                                              1.*histoDiffWeightNchCut[2]->GetBinContent(j)* histoDiffWeightNchCut[2]->GetBinContent(j));
        } else {
        //					histoDiffWeightCut[2]->GetBinContent(j)* histoDiffWeightCut[2]->GetBinContent(j));
            totalWeightNchSys[j-1] = TMath::Sqrt(histoDiffWeightNchCut[1]->GetBinContent(j)* histoDiffWeightNchCut[1]->GetBinContent(j) +
                                              histoDiffWeightNchCut[2]->GetBinContent(j)* histoDiffWeightNchCut[2]->GetBinContent(j));
        }
        totalWeightNchError[j-1] = TMath::Sqrt( weightNchSta[j-1]* weightNchSta[j-1]+ totalWeightNchSys[j-1] *totalWeightNchSys[j-1] );

	std::cout << std::setprecision(4)  << "R = " << histoDiffWeightNchCut[1]->GetBinCenter(j) 
		  << "cm, Weight = "<< std::setprecision(4) << weightNchValue[j-1] 
		  << ", Stat = "    << std::setprecision(3) << weightNchSta[j-1] 
		  << " (Stat = "    << std::setprecision(3) << (weightNchSta[j-1]/histoWeightNch[0]->GetBinContent(j))*100 
		  << "%), Sys = "   << std::setprecision(3) << totalWeightNchSys[j-1] 
		  << " (Sys = "     << std::setprecision(3) << (totalWeightNchSys[j-1]/histoWeightNch[0]->GetBinContent(j))*100 
		  << "%), Tot = "   << std::setprecision(3) << totalWeightNchError[j-1] 
		  << " (Tot = "     << std::setprecision(3) << (totalWeightNchError[j-1]/histoWeightNch[0]->GetBinContent(j))*100 << "%)" << endl ;
    }

    cout<< "   "<< endl;

    cout<< "$omega$"<< endl;
    for (Int_t j=1; j<histoDiffWeightCut[0]->GetNbinsX()+1; j++){
      std::cout <<"[" <<histoDiffWeightCut[1]->GetBinLowEdge(j)<< ","<< histoDiffWeightCut[1]->GetBinLowEdge(j+1)<< "["<< std::setprecision(4)<< "  &  "   
                << histoDiffWeightCut[1]->GetBinCenter(j) << " &  "
		<< std::setprecision(4) << weightValue[j-1] << " $pm$ "    << std::setprecision(3) << weightSta[j-1] << " $pm$ " << std::setprecision(3) << totalWeightSys[j-1] << "  &  " 
		<< std::setprecision(4) << weightValue[j-1] << " $pm$  "   << std::setprecision(3) << totalWeightError[j-1] << endl ;
    }

    cout<< "   "<< endl;
    cout<< "   "<< endl;    
    cout<< "$Omega$"<< endl;
    for (Int_t j=1; j<histoDiffWeightNchCut[0]->GetNbinsX()+1; j++){
      std::cout <<"[" <<histoDiffWeightNchCut[1]->GetBinLowEdge(j)<< ","<< histoDiffWeightNchCut[1]->GetBinLowEdge(j+1)<< "["<< std::setprecision(4)<< " & "   
                << histoDiffWeightNchCut[1]->GetBinCenter(j) << "  &  "
		<< std::setprecision(4) << weightNchValue[j-1] << " $pm$ "    << std::setprecision(3) << weightNchSta[j-1] << " $pm$ " << std::setprecision(3) << totalWeightNchSys[j-1] <<"  &  " 
		<< std::setprecision(4) << weightNchValue[j-1] << " $pm$  "   << std::setprecision(3) << totalWeightNchError[j-1] << endl ;
    }
    cout<< "   "<< endl;
    cout<< "   "<< endl;    
    
    Double_t averageWeightNchSys=0.;
    Double_t averageWeightNchSta=0.;
    Double_t averageWeightNchTot=0.;
    Double_t averageWeightNch=0.;

    for (Int_t j=2; j<nBins; j++){

        rminBin=rData[0]->GetXaxis()->FindBin(binsR[j]+0.0001);
        rmaxBin=rData[0]->GetXaxis()->FindBin(binsR[j+1]-0.0001);
        Double_t totNchInRBin= rData[0]->Integral(rminBin,rmaxBin,"width");
        //     cout<<"R Limits"<< binsR[j] << "  " << binsR[j+1]<< " " << rminBin<<"  " << rmaxBin<< " " << totInRBin  << "  " << totInRRange<< "  " << histWeight[0]->GetBinContent(j+1)<< endl;
        //      cout<<"R Limits::"<<  binsR[j] << "  " << binsR[j+1]<< " " << totInRBin  << "  " << totInRBin/totInRRange<< "  "<<  histWeight[0]->GetBinContent(j+1)<< endl;
        averageWeightNchSta+=weightNchSta[j]*(totNchInRBin/totInRRange);
        averageWeightNchSys+=totalWeightNchSys[j]*(totNchInRBin/totInRRange);
        averageWeightNchTot+=totalWeightNchError[j]*(totNchInRBin/totInRRange);
        averageWeightNch+=weightNchValue[j] *(totNchInRBin/totInRRange);
    }
    std::cout<< std::setprecision(4)  << "Average Weight Nch = " << averageWeightNch << ", Stat = " << (averageWeightNchSta/averageWeightNch)*100 << "%, Sys = " << (averageWeightNchSys/averageWeightNch)*100 << "%, Tot = " << (averageWeightNchTot/averageWeightNch)*100 << "%" << endl;

  


    cout << "line " << __LINE__ << endl;




    // plotting
    // All weights together: Pythia, Phojet, Onfly, Offline, and ratios

    Double_t minYRatio = 0.9;
    Double_t maxYRatio = 1.5;
    TCanvas* canvasWeight = new TCanvas("canvasWeight","",1500,1300);
    DrawGammaCanvasSettings( canvasWeight,  0.13, 0.02, 0.02, 0.09);
    TPad* padWeight = new TPad("padWeight", "", 0., 0.33, 1., 1.,-1, -1, -2);
    DrawGammaPadSettings( padWeight, 0.1, 0.03, 0.02, 0.);
    padWeight->Draw();
    
    TPad* padWeightRatios = new TPad("padWeightRatios", "", 0., 0., 1., 0.33,-1, -1, -2);
    DrawGammaPadSettings( padWeightRatios, 0.1, 0.03, 0.0, 0.2);
    padWeightRatios->Draw();
    padWeight->cd();
    padWeight->SetTickx();
    padWeight->SetTicky();
    padWeight->cd();
    
    TH2F * histo2DDummy = new TH2F("","",1000,0.,180.,1000,0.,1.5);
    SetStyleHistoTH2ForGraphs(histo2DDummy, "R (cm)", "#omega_{i}",0.04,0.04, 0.04,0.04, 1.,1.);
    //         histo2DDummy->GetXaxis()->SetMoreLogLabels();
    //         histo2DDummy->GetXaxis()->SetLabelOffset(-0.01);
    histo2DDummy->GetYaxis()->SetRangeUser(0.875,1.35);
    histo2DDummy->Draw("copy");
    
    DrawGammaLines(5., 5.,0.875,1.35,1.1,kGray+2,1);
    TLatex *labelR5cm = new TLatex(0.13,0.10,label5cm.Data());
    SetStyleTLatex( labelR5cm,0.04,4);
    labelR5cm->Draw();
    
    TLegend* legendWeightOnfly= new TLegend(0.45,0.93-(1+counterOnfly)*0.04,0.7,0.93); //0.17,0.13,0.5,0.24);
    legendWeightOnfly->SetFillColor(0);
    legendWeightOnfly->SetMargin(0.17);
    legendWeightOnfly->SetLineColor(0);
    legendWeightOnfly->SetTextFont(42);
    legendWeightOnfly->SetTextSize(0.035);
    legendWeightOnfly->SetHeader("On-the-Fly V0 finder");
    
    TLegend* legendWeightOffline= new TLegend(0.7,0.93-(1+counterOffline)*0.04,0.93,0.93); //0.17,0.13,0.5,0.24);
    legendWeightOffline->SetFillColor(0);
    legendWeightOffline->SetMargin(0.17);
    legendWeightOffline->SetLineColor(0);
    legendWeightOffline->SetTextFont(42);
    legendWeightOffline->SetTextSize(0.035);
    legendWeightOffline->SetHeader("Offline V0 finder");
    
    TLegend* legendWeight = GetAndSetLegend2(0.3,0.93-1.15*0.04*NumberOfCuts,0.65,0.93,40);
    DrawGammaLines(0., maxR,1., 1.,1.1,kGray+1,2);
    for(Int_t i = 0; i< NumberOfCuts; i++){
      
      DrawGammaSetMarker(histWeight[i], marker[i], 1.5,color[i],color[i]);
      histWeight[i]->Draw("same,c,p");
      
      if(V0ReaderName[i].Contains("Offline"))
	legendWeightOffline->AddEntry(histWeight[i],Form("%s, %s",periodName[i].Data(),generatorName[i].Data()));
      else
	legendWeightOnfly->AddEntry(histWeight[i],Form("%s, %s",periodName[i].Data(),generatorName[i].Data()));
      
      legendWeightOnfly->Draw();
      legendWeightOffline->Draw();
      
      
      legendWeight->AddEntry(histWeight[i],Form("%s, (%s) - %s",periodName[i].Data(),generatorName[i].Data(),V0ReaderName[i].Data()));   //" %s",cutNumber[i].Data()));
      //                 legendWeight->AddEntry(histWeight[i],Form("%s, %s (%s) - %s",periodName[i].Data(),clusterName[i].Data(),generatorName[i].Data(),V0ReaderName[i].Data()));   //" %s",cutNumber[i].Data()));
      //             legendWeight->Draw();
    }
    
    histo2DDummy->DrawCopy("axis,same");
    padWeightRatios->cd();
    minYRatio = 0.92;
    maxYRatio = 1.065; //qui
    TH2F * histo2DDummyRatio = new TH2F("","",1000,0.,180.,1000,0.,2.);
    SetStyleHistoTH2ForGraphs(histo2DDummyRatio, "R (cm)", "#omega_{i} #frac{modified}{standard}",0.08,0.085, 0.08,0.085,0.9,0.55);
    histo2DDummyRatio->GetYaxis()->SetRangeUser(minYRatio,maxYRatio);
    histo2DDummyRatio->Draw("copy");
    
    DrawGammaLines(0., maxR,1., 1.,1.1,kGray+1,2);
    DrawGammaLines(0., maxR,1.02, 1.02,1.1,kGray+1,2);
    DrawGammaLines(0., maxR,0.98, 0.98,1.1,kGray+1,2);
    
    DrawGammaLines(5., 5.,minYRatio,maxYRatio,1.1,kGray+2,1);
    
    cout << "line " << __LINE__ << endl;

    for(Int_t i = 0; i< NumberOfCuts; i++){
      if(i<12)  DrawGammaSetMarker(histoRatioWeightCut[i], marker[i], 1.5,color[i],color[i]);
      else      DrawGammaSetMarker(histoRatioWeightCut[i], marker[i], 1.5,color[i-12],color[i-12]);
      histoRatioWeightCut[i]->Draw("same,c,p");
    }

    histo2DDummyRatio->DrawCopy("axis,same");
    canvasWeight->Update();
    canvasWeight->SaveAs(Form("%s/Weight_%s.%s",outputDir.Data(),cutVariationName.Data(),suffix.Data()));

    canvasWeight->cd();
    padWeight->cd();

    histo2DDummy->Draw("copy");
    DrawGammaLines(0., maxR,1., 1.,1.1,kGray+1,2);
    DrawGammaLines(5., 5.,0.875,1.35,1.1,kGray+2,1);
    labelR5cm->Draw();
    
    TLegend* legendStd= new TLegend(0.6,0.93-3.5*0.04,0.93,0.93); //0.17,0.13,0.5,0.24);
    legendStd->SetFillColor(0);
    legendStd->SetMargin(0.17);
    legendStd->SetLineColor(0);
    legendStd->SetTextFont(42);
    legendStd->SetTextSize(0.04);
    legendStd->SetHeader("Pythia, On-the-Fly V0 finder");
    
    for(Int_t i = 0; i< NumberOfCuts; i++){
      if(generatorName[i].Contains("Pythia") && V0ReaderName[i].Contains("On-the-Fly")){
	
	DrawGammaSetMarker(histWeight[i], marker[i], 1.5,color[i],color[i]);
	histWeight[i]->Draw("same,c,p");
	
	legendStd->AddEntry(histWeight[i],Form("%s",periodName[i].Data()));
      }
    }
    legendStd->Draw();
    
    histo2DDummy->DrawCopy("axis,same");
    padWeightRatios->cd();
    histo2DDummyRatio->Draw("copy");
    
    DrawGammaLines(0., maxR,1., 1.,1.1,kGray+1,2);
    DrawGammaLines(0., maxR,1.02, 1.02,1.1,kGray+1,2);
    DrawGammaLines(0., maxR,0.98, 0.98,1.1,kGray+1,2);
    DrawGammaLines(5., 5.,minYRatio,maxYRatio,1.1,kGray+2,1);
    //         TLatex *labelR5cm = new TLatex(0.13,0.9,label5cm.Data());
    SetStyleTLatex( labelR5cm,0.04,4);
    
    for(Int_t i = 0; i< NumberOfCuts; i++){
      if(generatorName[i].Contains("Pythia") && V0ReaderName[i].Contains("On-the-Fly")){
	DrawGammaSetMarker(histoRatioWeightCut[i], marker[i], 1.5,color[i],color[i]);
	histoRatioWeightCut[i]->Draw("same,c,p");
      }
    }
    
    histo2DDummyRatio->DrawCopy("axis,same");
    canvasWeight->Update();
    canvasWeight->SaveAs(Form("%s/WeightStandard_%s.%s",outputDir.Data(),cutVariationName.Data(),suffix.Data()));
    delete canvasWeight;
    

    TCanvas* canvasWeightDoublePad = new TCanvas("canvasWeightDoublePad","",1500,1300);
    DrawGammaCanvasSettings( canvasWeightDoublePad,  0.13, 0.02, 0.02, 0.03);

    TPad* padWeight2PadRatio1 = new TPad("padWeight2PadRatio1", "", 0., 0.5, 1., 1.,-1, -1, -2);
    DrawGammaPadSettings( padWeight2PadRatio1, 0.12, 0.03, 0.02, 0.);
    padWeight2PadRatio1->Draw();

    TPad* padWeight2PadRatio2 = new TPad("padWeight2PadRatio2", "", 0., 0., 1., 0.5,-1, -1, -2);
    DrawGammaPadSettings( padWeight2PadRatio2, 0.12, 0.03, 0.0, 0.15);
    padWeight2PadRatio2->Draw();


    padWeight2PadRatio1->cd();
    padWeight2PadRatio1->SetTickx();
    padWeight2PadRatio1->SetTicky();
    padWeight2PadRatio1->cd();

        TH2F * histo2DDummyUpper = new TH2F("","",1000,0.,180.,1000,0.,2);
        SetStyleHistoTH2ForGraphs(histo2DDummyUpper, "R (cm)", "#omega_{i} #frac{modified}{standard}",0.06,0.07, 0.06,0.07,0.9,0.8);
        histo2DDummyUpper->GetYaxis()->SetRangeUser(0.93,1.13);
        histo2DDummyUpper->Draw("copy");

        DrawGammaLines(0., maxR,1., 1.,1.1,kGray+1,2);
        DrawGammaLines(0., maxR,1.02, 1.02,1.1,kGray+1,2);
        DrawGammaLines(0., maxR,0.98, 0.98,1.1,kGray+1,2);

        DrawGammaLines(5., 5.,0.93,1.13,1.1,kGray+2,1);
        labelR5cm = new TLatex(0.15,0.88,label5cm.Data());
        SetStyleTLatex( labelR5cm,0.06,4);
        labelR5cm->Draw();

        TLegend* legend13TeV = new TLegend(0.6,0.93-(1+counterOnfly)*0.06,0.93,0.93); //0.17,0.13,0.5,0.24);
        legend13TeV->SetFillColor(0);
        legend13TeV->SetMargin(0.17);
        legend13TeV->SetLineColor(0);
//         legend13TeV->SetNColumns(2);
        legend13TeV->SetTextFont(42);
        legend13TeV->SetTextSize(0.06);
        legend13TeV->SetHeader("pp, #sqrt{s} = 13 TeV");
        for(Int_t i = 0; i< NumberOfCuts; i++){
            if(period[i].Contains("LHC16")){
                DrawGammaSetMarker(histoRatioWeightEnergyCut[i], marker[i], 1.5,color[i],color[i]);
                histoRatioWeightEnergyCut[i]->Draw("same,c,p");

                legend13TeV->AddEntry(histoRatioWeightEnergyCut[i],Form("%s, %s",generatorName[i].Data(), V0ReaderName[i].Data()));
            }
        }
        legend13TeV->Draw();

        histo2DDummyUpper->DrawCopy("axis,same");
    padWeight2PadRatio2->cd();

        TH2F * histo2DDummyLower = new TH2F("","",1000,0.,180.,1000,0.,2.);
        SetStyleHistoTH2ForGraphs(histo2DDummyLower, "R (cm)", "#omega_{i} #frac{modified}{standard}",0.06,0.07, 0.06,0.07,0.9,0.8);
        histo2DDummyLower->GetYaxis()->SetRangeUser(0.93,1.13);
        histo2DDummyLower->Draw("copy");

        DrawGammaLines(0., maxR,1., 1.,1.1,kGray+1,2);
        DrawGammaLines(0., maxR,1.02, 1.02,1.1,kGray+1,2);
        DrawGammaLines(0., maxR,0.98, 0.98,1.1,kGray+1,2);

        DrawGammaLines(5., 5.,0.93,1.13,1.1,kGray+2,1);
        labelR5cm = new TLatex(0.15,0.88,label5cm.Data());
        SetStyleTLatex( labelR5cm,0.06,4);
        labelR5cm->Draw();

        TLegend* legend5TeV = new TLegend(0.6,0.93-(1+counterOnfly)*0.06,0.93,0.93); //0.17,0.13,0.5,0.24);
        legend5TeV->SetFillColor(0);
        legend5TeV->SetMargin(0.17);
        legend5TeV->SetLineColor(0);
//         legend5TeV->SetNColumns(2);
        legend5TeV->SetTextFont(42);
        legend5TeV->SetTextSize(0.06);
        legend5TeV->SetHeader("pp, #sqrt{s} = 5 TeV");
        for(Int_t i = 0; i< NumberOfCuts; i++){
            if(period[i].Contains("LHC17")){
                DrawGammaSetMarker(histoRatioWeightEnergyCut[i], marker[i], 1.5,color[i-2],color[i-2]);
                histoRatioWeightEnergyCut[i]->Draw("same,c,p");

                legend5TeV->AddEntry(histoRatioWeightEnergyCut[i],Form("%s, %s",generatorName[i].Data(), V0ReaderName[i].Data()));
            }
        }
        legend5TeV->Draw();

        histo2DDummyRatio->DrawCopy("axis,same");
    canvasWeightDoublePad->Update();
    canvasWeightDoublePad->SaveAs(Form("%s/Weight2PadRatio_%s.%s",outputDir.Data(),cutVariationName.Data(),suffix.Data()));
    delete canvasWeightDoublePad;

    TCanvas* canvasRelUncWeight = new TCanvas("canvasRelUncWeight","",1300,900);  // gives the page size
    DrawGammaCanvasSettings( canvasRelUncWeight,  0.08, 0.03, 0.02, 0.08);
//     canvasRelUncWeight->SetGridy();

        Double_t maxYRel    = 0;
        for(Int_t i = 0; i< NumberOfCuts; i++){
            if (histRelUncWeightCut[i]->GetMaximum() > maxYRel)
                maxYRel = histRelUncWeightCut[i]->GetMaximum();
        }
        if (maxYRel > 60) maxYRel = 60;
        TH2F* histo1DDummy2              = new TH2F("histo1DDummy2","histo1DDummy2",1000, 0., 180.,1000,0,10.);
        SetStyleHistoTH1ForGraphs(histo1DDummy2,  "R (cm)", Form("Weight rel unc. (%s)", "%"), 0.035 ,0.04, 0.035,0.04, 0.9, .9);//,510,540);
        histo1DDummy2->GetYaxis()->SetRangeUser(0.,4.5/*maxYRel*1.05*/);
        histo1DDummy2->DrawCopy();

        DrawGammaLines(0., maxR,1., 1.,1.1,kGray+2,2);
        DrawGammaLines(0., maxR,1.5, 1.5,1.1,kGray+2,2);
        DrawGammaLines(0., maxR,2., 2.,1.1,kGray+2,2);
        DrawGammaLines(0., maxR,2.5, 2.5,1.1,kGray+2,2);

        DrawGammaLines(5., 5.,0.,4.5,1.1,kGray+2,1);
        labelR5cm = new TLatex(0.11,0.88,label5cm.Data());
        SetStyleTLatex( labelR5cm,0.035,4);
        labelR5cm->Draw();

        for(Int_t i = 0; i< NumberOfCuts; i++){
            if(i<12) DrawGammaSetMarker(histRelUncWeightCut[i], marker[i], 1.5,color[i],color[i]);
            else     DrawGammaSetMarker(histRelUncWeightCut[i], marker[i], 1.5,color[i-12],color[i-12]);
            histRelUncWeightCut[i]->DrawCopy("same,c,p");
        }
//         legendWeight->Draw();
            legendWeightOnfly->Draw();
            legendWeightOffline->Draw();

        histo1DDummy2->DrawCopy("axis,same");
    canvasRelUncWeight->Update();
    canvasRelUncWeight->SaveAs(Form("%s/WeightRelUncertainty_%s.%s",outputDir.Data(),cutVariationName.Data(),suffix.Data()));
    delete canvasRelUncWeight;
//
    TCanvas* canvasWeightDiff = new TCanvas("canvasWeightDiff","",1300,900);
    DrawGammaCanvasSettings( canvasWeightDiff, 0.08, 0.03, 0.035, 0.09);

        Double_t minYDiff = -0.15;
        Double_t maxYDiff = 0.15; //qui
        TH2F * histo2DDummyDiff = new TH2F("","",1000,0.,180.,1000,-1.5,1.);
        SetStyleHistoTH2ForGraphs(histo2DDummyDiff, "R (cm)", "#omega_{i} modified - standard",0.04,0.04, 0.04,0.04, 1.,1.);
        histo2DDummyDiff->GetYaxis()->SetRangeUser(minYDiff,maxYDiff);
        histo2DDummyDiff->Draw("copy");

        DrawGammaLines(0., maxR,1., 1.,1.1,kGray+1,2);
        DrawGammaLines(0., maxR,0.02, 0.02,1.1,kGray+1,2);
        DrawGammaLines(0., maxR,-0.02, -0.02,1.1,kGray+1,2);

        DrawGammaLines(5., 5.,minYDiff,maxYDiff,1.1,kGray+2,1);
        labelR5cm = new TLatex(0.11,0.88,label5cm.Data());
        SetStyleTLatex( labelR5cm,0.04,4);
        labelR5cm->Draw();

        for(Int_t i = 0; i< NumberOfCuts; i++){
            if(i<12) DrawGammaSetMarker(histoDiffWeightCut[i], marker[i], 1.5,color[i],color[i]);
            else     DrawGammaSetMarker(histoDiffWeightCut[i], marker[i], 1.5,color[i-12],color[i-12]);
            histoDiffWeightCut[i]->Draw("same");
        }
//         legendWeight->Draw();
        legendWeightOnfly->Draw();
        legendWeightOffline->Draw();

        histo2DDummyDiff->DrawCopy("axis,same");
    canvasWeightDiff->Update();
    canvasWeightDiff->SaveAs(Form("%s/WeightDiff_%s.%s",outputDir.Data(),cutVariationName.Data(),suffix.Data()));
    delete canvasWeightDiff;

    cout << "line " << __LINE__ << endl;

    //--------------------------
    // case Nch

    // plotting
    // All weights together: Pythia, Phojet, Onfly, Offline, and ratios

    //    Double_t minYRatio = 0.9;
    //Double_t maxYRatio = 1.5;
    TCanvas* canvasWeightNch = new TCanvas("canvasWeightNch","",1500,1300);
    DrawGammaCanvasSettings( canvasWeightNch,  0.13, 0.02, 0.02, 0.09);
    TPad* padWeightNch = new TPad("padWeightNch", "", 0., 0.33, 1., 1.,-1, -1, -2);
    DrawGammaPadSettings( padWeightNch, 0.1, 0.03, 0.02, 0.);
    padWeightNch->SetTickx();
    padWeightNch->SetTicky();
    padWeightNch->Draw();
    
    TPad* padWeightRatiosNch = new TPad("padWeightRatiosNch", "", 0., 0., 1., 0.33,-1, -1, -2);
    DrawGammaPadSettings( padWeightRatiosNch, 0.1, 0.03, 0.0, 0.2);
    padWeightRatiosNch->Draw();
    padWeightNch->cd();
    
    TH2F * histo2DDummyNch = new TH2F("","",1000,0.,180.,1000,0.,1.5);
    SetStyleHistoTH2ForGraphs(histo2DDummyNch, "R (cm)", "#tilde{#Omega_{i}}",0.04,0.04, 0.04,0.04, 1.,1.);
    //         histo2DDummy->GetXaxis()->SetMoreLogLabels();
    //         histo2DDummy->GetXaxis()->SetLabelOffset(-0.01);
    histo2DDummyNch->GetYaxis()->SetRangeUser(0.85,1.35);
    histo2DDummyNch->Draw("copy");
    
    DrawGammaLines(5., 5.,0.875,1.35,1.1,kGray+2,1);
    TLatex *labelR5cmNch = new TLatex(0.13,0.10,label5cm.Data());
    SetStyleTLatex( labelR5cmNch,0.04,4);
    labelR5cmNch->Draw();
    
    TLegend* legendWeightNchOnfly= new TLegend(0.45,0.93-(1+counterOnfly)*0.04,0.7,0.93); //0.17,0.13,0.5,0.24);
    legendWeightNchOnfly->SetFillColor(0);
    legendWeightNchOnfly->SetMargin(0.17);
    legendWeightNchOnfly->SetLineColor(0);
    legendWeightNchOnfly->SetTextFont(42);
    legendWeightNchOnfly->SetTextSize(0.035);
    legendWeightNchOnfly->SetHeader("On-the-Fly V0 finder");
    
    TLegend* legendWeightNchOffline= new TLegend(0.7,0.93-(1+counterOffline)*0.04,0.93,0.93); //0.17,0.13,0.5,0.24);
    legendWeightNchOffline->SetFillColor(0);
    legendWeightNchOffline->SetMargin(0.17);
    legendWeightNchOffline->SetLineColor(0);
    legendWeightNchOffline->SetTextFont(42);
    legendWeightNchOffline->SetTextSize(0.035);
    legendWeightNchOffline->SetHeader("Offline V0 finder");
    
    TLegend* legendWeightNch = GetAndSetLegend2(0.3,0.93-1.15*0.04*NumberOfCuts,0.65,0.93,40);
    DrawGammaLines(0., maxR,1., 1.,1.1,kGray+1,2);
    for(Int_t i = 0; i< NumberOfCuts; i++){
      
      DrawGammaSetMarker(histoWeightNch[i], marker[i], 1.5,color[i],color[i]);
      histoWeightNch[i]->Draw("same,c,p");
      
      if(V0ReaderName[i].Contains("Offline"))
	legendWeightNchOffline->AddEntry(histoWeightNch[i],Form("%s, %s",periodName[i].Data(),generatorName[i].Data()));
      else
	legendWeightNchOnfly->AddEntry(histoWeightNch[i],Form("%s, %s",periodName[i].Data(),generatorName[i].Data()));
      
      legendWeightNchOnfly->Draw();
      legendWeightNchOffline->Draw();
      
      
      legendWeight->AddEntry(histoWeightNch[i],Form("%s, (%s) - %s",periodName[i].Data(),generatorName[i].Data(),V0ReaderName[i].Data()));   //" %s",cutNumber[i].Data()));
      //                 legendWeight->AddEntry(histWeight[i],Form("%s, %s (%s) - %s",periodName[i].Data(),clusterName[i].Data(),generatorName[i].Data(),V0ReaderName[i].Data()));   //" %s",cutNumber[i].Data()));
      //             legendWeight->Draw();
    }
    
    histo2DDummyNch->DrawCopy("axis,same");
    DrawGammaLines(0., maxR,0.98, 0.98,1.1,kRed+1,2);
    

    padWeightRatiosNch->cd();
    //minYRatio = 0.72;
    // maxYRatio = 1.285; //qui
    minYRatio = 0.92;
    maxYRatio = 1.065; //qui
 
    TH2F * histo2DDummyRatioNch = new TH2F("","",1000,0.,180.,1000,0.,2.);
    SetStyleHistoTH2ForGraphs(histo2DDummyRatioNch, "R (cm)", "#tilde{#Omega_{i}}  #frac{modified}{standard}",0.08,0.085, 0.08,0.085,0.9,0.55);
    histo2DDummyRatioNch->GetYaxis()->SetRangeUser(minYRatio,maxYRatio);
    histo2DDummyRatioNch->Draw("copy");
    
    DrawGammaLines(0., maxR,1., 1.,1.1,kGray+1,2);
    DrawGammaLines(0., maxR,1.02, 1.02,1.1,kGray+1,2);
    DrawGammaLines(0., maxR,0.98, 0.98,1.1,kGray+1,2);
    
    DrawGammaLines(5., 5.,minYRatio,maxYRatio,1.1,kGray+2,1);
    
    cout << "line " << __LINE__ << endl;

    for(Int_t i = 0; i< NumberOfCuts; i++){
      if(i<12)  DrawGammaSetMarker(histoRatioWeightNchCut[i], marker[i], 1.5,color[i],color[i]);
      else      DrawGammaSetMarker(histoRatioWeightNchCut[i], marker[i], 1.5,color[i-12],color[i-12]);
      histoRatioWeightNchCut[i]->Draw("same,c,p");
    }

    histo2DDummyRatioNch->DrawCopy("axis,same");
    canvasWeightNch->Update();
    canvasWeightNch->SaveAs(Form("%s/WeightNch_%s.%s",outputDir.Data(),cutVariationName.Data(),suffix.Data()));
    //------------------------------------



    canvasWeightNch->cd();
    padWeightNch->cd();

    histo2DDummyNch->Draw("copy");
    DrawGammaLines(0., maxR,1., 1.,1.1,kGray+1,2);
    DrawGammaLines(5., 5.,0.875,1.35,1.1,kGray+2,1);
    labelR5cm->Draw();
    DrawGammaLines(0.,1.,0.98, 0.98,1.,kGray,1);

    TLegend* legendStdNch= new TLegend(0.6,0.93-3.5*0.04,0.93,0.93); //0.17,0.13,0.5,0.24);
    legendStdNch->SetFillColor(0);
    legendStdNch->SetMargin(0.17);
    legendStdNch->SetLineColor(0);
    legendStdNch->SetTextFont(42);
    legendStdNch->SetTextSize(0.04);
    legendStdNch->SetHeader("Pythia, On-the-Fly V0 finder");
    
    for(Int_t i = 0; i< NumberOfCuts; i++){
      if(generatorName[i].Contains("Pythia") && V0ReaderName[i].Contains("On-the-Fly")){
	
	DrawGammaSetMarker(histoWeightNch[i], marker[i], 1.5,color[i],color[i]);
	histoWeightNch[i]->Draw("same,c,p");
	
	legendStdNch->AddEntry(histoWeightNch[i],Form("%s",periodName[i].Data()));
      }
    }
    legendStdNch->Draw();
    
    histo2DDummyNch->DrawCopy("axis,same");
    DrawGammaLines(0., maxR,0.98, 0.98,1.1,kRed+1,2);
    
    padWeightRatiosNch->cd();
    histo2DDummyRatioNch->Draw("copy");
    
    DrawGammaLines(0., maxR,1., 1.,1.1,kGray+1,2);
    DrawGammaLines(0., maxR,1.02, 1.02,1.1,kGray+1,2);
    DrawGammaLines(0., maxR,0.98, 0.98,1.1,kGray+1,2);
    DrawGammaLines(5., 5.,minYRatio,maxYRatio,1.1,kGray+2,1);
    //         TLatex *labelR5cm = new TLatex(0.13,0.9,label5cm.Data());
    SetStyleTLatex( labelR5cm,0.04,4);
    
    for(Int_t i = 0; i< NumberOfCuts; i++){
      if(generatorName[i].Contains("Pythia") && V0ReaderName[i].Contains("On-the-Fly")){
	DrawGammaSetMarker(histoRatioWeightNchCut[i], marker[i], 1.5,color[i],color[i]);
	histoRatioWeightNchCut[i]->Draw("same,c,p");
      }
    }
    
    histo2DDummyRatioNch->DrawCopy("axis,same");
    canvasWeightNch->Update();
    canvasWeightNch->SaveAs(Form("%s/WeightStandardNch_%s.%s",outputDir.Data(),cutVariationName.Data(),suffix.Data()));
    //    delete canvasWeightNch;
    
    // Comparison Phojet and 2 energies

    canvasWeightNch->cd();
    padWeightNch->cd();

    histo2DDummyNch->Draw("copy");
    DrawGammaLines(0., maxR,1., 1.,1.1,kGray+1,2);
    DrawGammaLines(5., 5.,0.875,1.35,1.1,kGray+2,1);
    labelR5cm->Draw();
    
    TLegend* legendStdNchPho= new TLegend(0.6,0.93-3.5*0.04,0.93,0.93); //0.17,0.13,0.5,0.24);
    legendStdNchPho->SetFillColor(0);
    legendStdNchPho->SetMargin(0.17);
    legendStdNchPho->SetLineColor(0);
    legendStdNchPho->SetTextFont(42);
    legendStdNchPho->SetTextSize(0.04);
    legendStdNchPho->SetHeader("Phojet, On-the-Fly V0 finder");
    
    for(Int_t i = 0; i< NumberOfCuts; i++){
      if(generatorName[i].Contains("Phojet") && V0ReaderName[i].Contains("On-the-Fly")){
	
	DrawGammaSetMarker(histoWeightNch[i], marker[i], 1.5,color[i],color[i]);
	histoWeightNch[i]->Draw("same,c,p");
	
	legendStdNchPho->AddEntry(histoWeightNch[i],Form("%s",periodName[i].Data()));
      }
    }
    legendStdNchPho->Draw();
    
    histo2DDummyNch->DrawCopy("axis,same");
    padWeightRatiosNch->cd();
    histo2DDummyRatioNch->Draw("copy");
    
    DrawGammaLines(0., maxR,1., 1.,1.1,kGray+1,2);
    DrawGammaLines(0., maxR,1.02, 1.02,1.1,kGray+1,2);
    DrawGammaLines(0., maxR,0.98, 0.98,1.1,kGray+1,2);
    DrawGammaLines(5., 5.,minYRatio,maxYRatio,1.1,kGray+2,1);
    //         TLatex *labelR5cm = new TLatex(0.13,0.9,label5cm.Data());
    SetStyleTLatex( labelR5cm,0.04,4);
    
    for(Int_t i = 0; i< NumberOfCuts; i++){
      if(generatorName[i].Contains("Phojet") && V0ReaderName[i].Contains("On-the-Fly")){
	DrawGammaSetMarker(histoRatioWeightNchCut[i], marker[i], 1.5,color[i],color[i]);
	histoRatioWeightNchCut[i]->Draw("same,c,p");
      }
    }
    
    histo2DDummyRatioNch->DrawCopy("axis,same");
    canvasWeightNch->Update();
    //    canvasWeightNch->SaveAs(Form("%s/WeightStandardNchPhojet_%s.%s",outputDir.Data(),cutVariationName.Data(),suffix.Data()));
    //    delete canvasWeightNch;
    
    //   Pythia offline

   canvasWeightNch->cd();
    padWeightNch->cd();

    histo2DDummyNch->Draw("copy");
    DrawGammaLines(0., maxR,1., 1.,1.1,kGray+1,2);
    DrawGammaLines(5., 5.,0.875,1.35,1.1,kGray+2,1);
    labelR5cm->Draw();
    
    TLegend* legendStdNchOff= new TLegend(0.6,0.93-3.5*0.04,0.93,0.93); //0.17,0.13,0.5,0.24);
    legendStdNchOff->SetFillColor(0);
    legendStdNchOff->SetMargin(0.17);
    legendStdNchOff->SetLineColor(0);
    legendStdNchOff->SetTextFont(42);
    legendStdNchOff->SetTextSize(0.04);
    legendStdNchOff->SetHeader("Pythia, Offline finder");
    
    for(Int_t i = 0; i< NumberOfCuts; i++){
      if(generatorName[i].Contains("Pythia") && V0ReaderName[i].Contains("Offline")){
	
	DrawGammaSetMarker(histoWeightNch[i], marker[i], 1.5,color[i],color[i]);
	histoWeightNch[i]->Draw("same,c,p");
	
	legendStdNchOff->AddEntry(histoWeightNch[i],Form("%s",periodName[i].Data()));
      }
    }
    legendStdNchOff->Draw();
    
    histo2DDummyNch->DrawCopy("axis,same");
    padWeightRatiosNch->cd();
    histo2DDummyRatioNch->Draw("copy");
    
    DrawGammaLines(0., maxR,1., 1.,1.1,kGray+1,2);
    DrawGammaLines(0., maxR,1.02, 1.02,1.1,kGray+1,2);
    DrawGammaLines(0., maxR,0.98, 0.98,1.1,kGray+1,2);
    DrawGammaLines(5., 5.,minYRatio,maxYRatio,1.1,kGray+2,1);
    //         TLatex *labelR5cm = new TLatex(0.13,0.9,label5cm.Data());
    SetStyleTLatex( labelR5cm,0.04,4);
    
    for(Int_t i = 0; i< NumberOfCuts; i++){
      if(generatorName[i].Contains("Pythia") && V0ReaderName[i].Contains("Offline")){
	DrawGammaSetMarker(histoRatioWeightNchCut[i], marker[i], 1.5,color[i],color[i]);
	histoRatioWeightNchCut[i]->Draw("same,c,p");
      }
    }
    
    histo2DDummyRatioNch->DrawCopy("axis,same");
    canvasWeightNch->Update();
    //    canvasWeightNch->SaveAs(Form("%s/WeightStandardNchPythiaOff_%s.%s",outputDir.Data(),cutVariationName.Data(),suffix.Data()));
    //    delete canvasWeightNch;
    



    //   Phojet offline

   canvasWeightNch->cd();
    padWeightNch->cd();

    histo2DDummyNch->Draw("copy");
    DrawGammaLines(0., maxR,1., 1.,1.1,kGray+1,2);
    DrawGammaLines(5., 5.,0.875,1.35,1.1,kGray+2,1);
    labelR5cm->Draw();
    
    TLegend* legendStdNchPhoOff= new TLegend(0.6,0.93-3.5*0.04,0.93,0.93); //0.17,0.13,0.5,0.24);
    legendStdNchPhoOff->SetFillColor(0);
    legendStdNchPhoOff->SetMargin(0.17);
    legendStdNchPhoOff->SetLineColor(0);
    legendStdNchPhoOff->SetTextFont(42);
    legendStdNchPhoOff->SetTextSize(0.04);
    legendStdNchPhoOff->SetHeader("Phojet, Offline finder");
    
    for(Int_t i = 0; i< NumberOfCuts; i++){
      if(generatorName[i].Contains("Phojet") && V0ReaderName[i].Contains("Offline")){
	
	DrawGammaSetMarker(histoWeightNch[i], marker[i], 1.5,color[i],color[i]);
	histoWeightNch[i]->Draw("same,c,p");
	
	legendStdNchPhoOff->AddEntry(histoWeightNch[i],Form("%s",periodName[i].Data()));
      }
    }
    legendStdNchPhoOff->Draw();
    
    histo2DDummyNch->DrawCopy("axis,same");
    padWeightRatiosNch->cd();
    histo2DDummyRatioNch->Draw("copy");
    
    DrawGammaLines(0., maxR,1., 1.,1.1,kGray+1,2);
    DrawGammaLines(0., maxR,1.02, 1.02,1.1,kGray+1,2);
    DrawGammaLines(0., maxR,0.98, 0.98,1.1,kGray+1,2);
    DrawGammaLines(5., 5.,minYRatio,maxYRatio,1.1,kGray+2,1);
    //         TLatex *labelR5cm = new TLatex(0.13,0.9,label5cm.Data());
    SetStyleTLatex( labelR5cm,0.04,4);
    
    for(Int_t i = 0; i< NumberOfCuts; i++){
      if(generatorName[i].Contains("Phojet") && V0ReaderName[i].Contains("Offline")){
	DrawGammaSetMarker(histoRatioWeightNchCut[i], marker[i], 1.5,color[i],color[i]);
	histoRatioWeightNchCut[i]->Draw("same,c,p");
      }
    }
    
    histo2DDummyRatioNch->DrawCopy("axis,same");
    canvasWeightNch->Update();
    //    canvasWeightNch->SaveAs(Form("%s/WeightStandardNchPhojetOff_%s.%s",outputDir.Data(),cutVariationName.Data(),suffix.Data()));
    delete canvasWeightNch;
    

    // ------------------


    // double pad - Nch

    TCanvas* canvasWeightDoublePadNch = new TCanvas("canvasWeightDoublePadNch","",1500,1300);
    DrawGammaCanvasSettings( canvasWeightDoublePadNch,  0.13, 0.02, 0.02, 0.03);

    TPad* padWeight2PadRatio1Nch = new TPad("padWeight2PadRatio1Nch", "", 0., 0.5, 1., 1.,-1, -1, -2);
    DrawGammaPadSettings( padWeight2PadRatio1Nch, 0.12, 0.03, 0.02, 0.);
    padWeight2PadRatio1Nch->Draw();

    TPad* padWeight2PadRatio2Nch = new TPad("padWeight2PadRatio2Nch", "", 0., 0., 1., 0.5,-1, -1, -2);
    DrawGammaPadSettings( padWeight2PadRatio2Nch, 0.12, 0.03, 0.0, 0.15);
    padWeight2PadRatio2Nch->Draw();


    padWeight2PadRatio1Nch->cd();
    padWeight2PadRatio1Nch->SetTickx();
    padWeight2PadRatio1Nch->SetTicky();
    padWeight2PadRatio1Nch->cd();

        TH2F * histo2DDummyUpperNch = new TH2F("","",1000,0.,180.,1000,0.,2);
        SetStyleHistoTH2ForGraphs(histo2DDummyUpperNch, "R (cm)", "#tilde{#Omega_{i}} #frac{modified}{standard}",0.06,0.07, 0.06,0.07,0.9,0.8);
        histo2DDummyUpperNch->GetYaxis()->SetRangeUser(0.85,1.13);
        histo2DDummyUpperNch->Draw("copy");

        DrawGammaLines(0., maxR,1., 1.,1.1,kGray+1,2);
        DrawGammaLines(0., maxR,1.02, 1.02,1.1,kGray+1,2);
        DrawGammaLines(0., maxR,0.98, 0.98,1.1,kGray+1,2);

        DrawGammaLines(5., 5.,0.93,1.13,1.1,kGray+2,1);
        labelR5cm = new TLatex(0.15,0.88,label5cm.Data());
        SetStyleTLatex( labelR5cm,0.06,4);
        labelR5cm->Draw();

        TLegend* legend13TeVNch = new TLegend(0.6,0.93-(1+counterOnfly)*0.06,0.93,0.93); //0.17,0.13,0.5,0.24);
        legend13TeVNch->SetFillColor(0);
        legend13TeVNch->SetMargin(0.17);
        legend13TeVNch->SetLineColor(0);
//         legend13TeV->SetNColumns(2);
        legend13TeVNch->SetTextFont(42);
        legend13TeVNch->SetTextSize(0.06);
        legend13TeVNch->SetHeader("pp, #sqrt{s} = 13 TeV");
        for(Int_t i = 0; i< NumberOfCuts; i++){
            if(period[i].Contains("LHC16")){
                DrawGammaSetMarker(histoRatioWeightNchEnergyCut[i], marker[i], 1.5,color[i],color[i]);
                histoRatioWeightNchEnergyCut[i]->Draw("same,c,p");

                legend13TeVNch->AddEntry(histoRatioWeightNchEnergyCut[i],Form("%s, %s",generatorName[i].Data(), V0ReaderName[i].Data()));
            }
        }
        legend13TeVNch->Draw();

        histo2DDummyUpper->DrawCopy("axis,same");

    padWeight2PadRatio2Nch->cd();
        TH2F * histo2DDummyLowerNch = new TH2F("","",1000,0.,180.,1000,0.,2.);
        SetStyleHistoTH2ForGraphs(histo2DDummyLowerNch, "R (cm)", "#tilde{#Omega_{i}} #frac{modified}{standard}",0.06,0.07, 0.06,0.07,0.9,0.8);
        histo2DDummyLowerNch->GetYaxis()->SetRangeUser(0.85,1.13);
        histo2DDummyLowerNch->Draw("copy");

        DrawGammaLines(0., maxR,1., 1.,1.1,kGray+1,2);
        DrawGammaLines(0., maxR,1.02, 1.02,1.1,kGray+1,2);
        DrawGammaLines(0., maxR,0.98, 0.98,1.1,kGray+1,2);

        DrawGammaLines(5., 5.,0.93,1.13,1.1,kGray+2,1);
        labelR5cm = new TLatex(0.15,0.88,label5cm.Data());
        SetStyleTLatex( labelR5cm,0.06,4);
        labelR5cm->Draw();

        TLegend* legend5TeVNch = new TLegend(0.6,0.52-(1+counterOnfly)*0.06,0.93,0.52); //0.17,0.13,0.5,0.24);
        legend5TeVNch->SetFillColor(0);
        legend5TeVNch->SetMargin(0.17);
        legend5TeVNch->SetLineColor(0);
//         legend5TeV->SetNColumns(2);
        legend5TeVNch->SetTextFont(42);
        legend5TeVNch->SetTextSize(0.06);
        legend5TeVNch->SetHeader("pp, #sqrt{s} = 5 TeV");
        for(Int_t i = 0; i< NumberOfCuts; i++){
            if(period[i].Contains("LHC17")){
                DrawGammaSetMarker(histoRatioWeightNchEnergyCut[i], marker[i], 1.5,color[i-2],color[i-2]);
                histoRatioWeightNchEnergyCut[i]->Draw("same,c,p");

                legend5TeVNch->AddEntry(histoRatioWeightNchEnergyCut[i],Form("%s, %s",generatorName[i].Data(), V0ReaderName[i].Data()));
            }
        }
        legend5TeVNch->Draw();

        histo2DDummyRatioNch->DrawCopy("axis,same");
    canvasWeightDoublePadNch->Update();
    canvasWeightDoublePadNch->SaveAs(Form("%s/Weight2PadRatioNch_%s.%s",outputDir.Data(),cutVariationName.Data(),suffix.Data()));
    delete canvasWeightDoublePadNch;






   //_______________________ Ploting weights vs pT in each R bin On-the-fly finder__________________________

    TCanvas *canvasMBWeightEachROnfly          = new TCanvas("canvasMBWeighEachROnfly","",1400,900);  // gives the page size
    DrawGammaCanvasSettings( canvasMBWeightEachROnfly, 0, 0, 0, 0);
    canvasMBWeightEachROnfly->cd();
    TPad * padMBWeightEachROnfly               = new TPad("padMBWeightEachROnfly","",-0.0,0.0,1.,1.,0);   // gives the size of the histo areas
    DrawGammaPadSettings( padMBWeightEachROnfly, 0, 0, 0, 0);
    padMBWeightEachROnfly->Divide(4,3,0.0,0.0);
    padMBWeightEachROnfly->Draw();
    Int_t place  = 0;
    TH2F *histoDummyWeightEachROnfly =  new TH2F("histoDummyWeightEachROnfly","histoDummyWeightEachROnfly",1000,-0.05,1.,1000,0.5,2.);
    SetStyleHistoTH2ForGraphs(histoDummyWeightEachROnfly, "#it{p}_{T}^{Min} (GeV/c)","w_i", 0.05,0.05, 0.05,0.05);

    TLegend* legendWeightPtOnfly= new TLegend(0.5,0.31-(1+counterOnfly)*0.05,0.8,0.31); //0.17,0.13,0.5,0.24);
    legendWeightPtOnfly->SetFillColor(0);
    legendWeightPtOnfly->SetMargin(0.17);
    legendWeightPtOnfly->SetLineColor(0);
    legendWeightPtOnfly->SetTextFont(42);
    legendWeightPtOnfly->SetTextSize(0.05);
    legendWeightPtOnfly->SetHeader("On-the-Fly V0 finder");
    for(Int_t i = 0; i< 2; i++){
        if(V0ReaderName[i].Contains("On-the-Fly"))
            legendWeightPtOnfly->AddEntry(histWeight[i],Form("%s, %s",periodName[i].Data(),generatorName[i].Data()));
    }

    for(Int_t j=0; j < nBinsR; j++){
        place  = place + 1;
        padMBWeightEachROnfly->cd(place);
        padMBWeightEachROnfly->cd(place)->SetTopMargin(0.04);
        padMBWeightEachROnfly->cd(place)->SetBottomMargin(0.15);
        padMBWeightEachROnfly->cd(place)->SetLeftMargin(0.15);
        padMBWeightEachROnfly->cd(place)->SetRightMargin(0.05);
        histoDummyWeightEachROnfly->GetYaxis()->SetNdivisions(504);

        if(j>1 && j<=3)      histoDummyWeightEachROnfly->GetYaxis()->SetRangeUser(0.9,1.3);
        else if(j>3 && j<=6) histoDummyWeightEachROnfly->GetYaxis()->SetRangeUser(0.9,1.2);
        else if(j>7 )        histoDummyWeightEachROnfly->GetYaxis()->SetRangeUser(0.9,1.1);

        histoDummyWeightEachROnfly->DrawCopy();
        DrawGammaLines(0.,1.,1., 1.,1.,kGray,1);
        DrawGammaLines(0.4,0.4,0.82,1.3,1.,kGray,2);
	DrawGammaLines(0.1,0.1,0.5, 10.1,1.,kGray,1);
	DrawGammaLines(0.2,0.2,0.5, 10.1,1.,kGray,1);
	DrawGammaLines(0.3,0.3,0.5, 10.1,1.,kGray,1);
	DrawGammaLines(0.05,0.05,0.5, 10.1,1.,kGray,1);


        for(Int_t i = 0; i< 2; i++){
            if(i<12) DrawGammaSetMarker(histoWeightsEachRPtMin[j][i], marker[i], 0.5,color[i],color[i]);
            else     DrawGammaSetMarker(histoWeightsEachRPtMin[j][i], marker[i], 0.5,color[i-12],color[i-12]);
	    if(j<2) histoWeightsEachRPtMin[j][i]->SetLineStyle(3);
	    histoWeightsEachRPtMin[j][i]->Draw("same");
	    // if(i<12) DrawGammaSetMarker(histoWeightsEachRPtMinSecSub[j][i], marker[i], 0.5,6,color[i]);
            // else     DrawGammaSetMarker(histoWeightsEachRPtMinSecSub[j][i], marker[i], 0.5,6,color[i-12]);
            // histoWeightsEachRPtMinSecSub[j][i]->Draw("same");
        }
        if (j==0)  legendWeightPtOnfly->Draw();
	TLatex *latexBinning = new TLatex(0.25,0.8,Form("R: %s ",arrayRangesRBins[j].Data()));
	SetStyleTLatex( latexBinning, sizeTextNameBins,2);
	latexBinning->Draw();
    }

    canvasMBWeightEachROnfly->Print(Form("%s/MBWeightVSPtMinOnflyEachR_%s.%s",outputDir.Data(),cutVariationName.Data(),suffix.Data()));


    cout << "line " << __LINE__ << endl;
 //_______________________ Ploting weights vs pT in each R bin Offline finder__________________________

    TCanvas *canvasMBWeightEachROffline          = new TCanvas("canvasMBWeighEachROffline","",1400,900);  // gives the page size
    DrawGammaCanvasSettings( canvasMBWeightEachROffline, 0, 0, 0, 0);
    canvasMBWeightEachROffline->cd();
    TPad * padMBWeightEachROffline               = new TPad("padMBWeightEachROffline","",-0.0,0.0,1.,1.,0);   // gives the size of the histo areas
    DrawGammaPadSettings( padMBWeightEachROffline, 0, 0, 0, 0);
    padMBWeightEachROffline->Divide(4,3,0.0,0.0);
    padMBWeightEachROffline->Draw();
     place  = 0;
    TH2F *histoDummyWeightEachROffline =  new TH2F("histoDummyWeightEachROffline","histoDummyWeightEachROffline",1000,-0.05,1.,1000,0.5,2.);
    SetStyleHistoTH2ForGraphs(histoDummyWeightEachROffline, "#it{p}_{T}^{Min} (GeV/c)","w_i", 0.05,0.05, 0.05,0.05);

    TLegend* legendWeightPtOffline= new TLegend(0.5,0.31-(1+counterOnfly)*0.05,0.8,0.31); //0.17,0.13,0.5,0.24);
    legendWeightPtOffline->SetFillColor(0);
    legendWeightPtOffline->SetMargin(0.17);
    legendWeightPtOffline->SetLineColor(0);
    legendWeightPtOffline->SetTextFont(42);
    legendWeightPtOffline->SetTextSize(0.05);
    legendWeightPtOffline->SetHeader("Offline V0 finder");
    for(Int_t i = 2; i< NumberOfCuts; i++){
        if(V0ReaderName[i].Contains("Offline"))
            legendWeightPtOffline->AddEntry(histWeight[i],Form("%s, %s",periodName[i].Data(),generatorName[i].Data()));
    }

    for(Int_t j=0; j < nBinsR; j++){
        place  = place + 1;
        padMBWeightEachROffline->cd(place);
        padMBWeightEachROffline->cd(place)->SetTopMargin(0.04);
        padMBWeightEachROffline->cd(place)->SetBottomMargin(0.15);
        padMBWeightEachROffline->cd(place)->SetLeftMargin(0.15);
        padMBWeightEachROffline->cd(place)->SetRightMargin(0.05);
        histoDummyWeightEachROffline->GetYaxis()->SetNdivisions(504);

//         if(j>1 && j<=3)      histoDummyWeightEachROffline->GetYaxis()->SetRangeUser(0.9,1.3);
//         else if(j>3 && j<=6) histoDummyWeightEachROffline->GetYaxis()->SetRangeUser(0.9,1.2);
//         else if(j>7 )       histoDummyWeightEachROffline->GetYaxis()->SetRangeUser(0.9,1.1);
        histoDummyWeightEachROffline->GetYaxis()->SetRangeUser(0.82,1.5);

        histoDummyWeightEachROffline->DrawCopy();
        DrawGammaLines(0.,1.,1., 1.,1.,kGray,1);
        DrawGammaLines(0.4,0.4,0.85, 1.5,1.,kGray,2);
	//	DrawGammaLines(0.1,0.1,0.001, 10.1,1.,kGray,1);
	DrawGammaLines(0.05,0.05,0.5, 10.1,1.,kGray,1);
	DrawGammaLines(0.1,0.1,0.5, 10.1,1.,kGray,1);
	DrawGammaLines(0.2,0.2,0.5, 10.1,1.,kGray,1);
	DrawGammaLines(0.3,0.3,0.5, 10.1,1.,kGray,1);
	DrawGammaLines(0.05,0.05,0.5, 10.1,1.,kGray,1);



        for(Int_t i = 2; i< NumberOfCuts; i++){
	  if(i<12) DrawGammaSetMarker(histoWeightsEachRPtMin[j][i], marker[i], 0.5,color[i],color[i]);
	  else     DrawGammaSetMarker(histoWeightsEachRPtMin[j][i], marker[i], 0.5,color[i-12],color[i-12]);
	  if(j<2) histoWeightsEachRPtMin[j][i]->SetLineStyle(3);
	  histoWeightsEachRPtMin[j][i]->Draw("same");
	  // if(i<12) DrawGammaSetMarker(histoWeightsEachRPtMinSecSub[j][i], marker[i], 0.5,6,color[i]);
	  // else     DrawGammaSetMarker(histoWeightsEachRPtMinSecSub[j][i], marker[i], 0.5,6,color[i-12]);
	  // histoWeightsEachRPtMinSecSub[j][i]->Draw("same");
        }

        if (j==0)  legendWeightPtOffline->Draw();
	TLatex *latexBinning = new TLatex(0.25,0.8,Form("R: %s ",arrayRangesRBins[j].Data()));
	SetStyleTLatex( latexBinning, sizeTextNameBins,2);
	latexBinning->Draw();
    }
    canvasMBWeightEachROffline->Print(Form("%s/MBWeightVSPtMinOfflineEachR_%s.%s",outputDir.Data(),cutVariationName.Data(),suffix.Data()));


   //____________________________________________________________________________________
    cout << "line " << __LINE__ << endl;

   //_______________________ Ploting weights vs pT in each R bin On-the-fly finder Secondary Subtracted usingCocktail__________________________

    TCanvas *canvasMBWeightEachROnflySecSubCock          = new TCanvas("canvasMBWeighEachROnflySecSubCock","",1400,900);  // gives the page size
    DrawGammaCanvasSettings( canvasMBWeightEachROnflySecSubCock, 0, 0, 0, 0);
    canvasMBWeightEachROnflySecSubCock->cd();
    TPad * padMBWeightEachROnflySecSubCock               = new TPad("padMBWeightEachROnflySecSubCock","",-0.0,0.0,1.,1.,0);   // gives the size of the histo areas
    DrawGammaPadSettings( padMBWeightEachROnflySecSubCock, 0, 0, 0, 0);
    padMBWeightEachROnflySecSubCock->Divide(4,3,0.0,0.0);
    padMBWeightEachROnflySecSubCock->Draw();
    place  = 0;
    TH2F *histoDummyWeightEachROnflySecSubCock =  new TH2F("histoDummyWeightEachROnflySecSubCock","histoDummyWeightEachROnflySecSubCock",1000,-0.05,1.,1000,0.5,2.);
    SetStyleHistoTH2ForGraphs(histoDummyWeightEachROnflySecSubCock, "#it{p}_{T}^{Min} (GeV/c)","w_i", 0.05,0.05, 0.05,0.05);

    TLegend* legendWeightPtOnflySecSubCock= new TLegend(0.3,0.31-(1+counterOnfly)*0.05,0.8,0.31); //0.17,0.13,0.5,0.24);
    legendWeightPtOnflySecSubCock->SetFillColor(0);
    legendWeightPtOnflySecSubCock->SetMargin(0.17);
    legendWeightPtOnflySecSubCock->SetLineColor(0);
    legendWeightPtOnflySecSubCock->SetTextFont(42);
    legendWeightPtOnflySecSubCock->SetTextSize(0.05);
    legendWeightPtOnflySecSubCock->SetHeader("On-the-Fly V0 finder SecSubCock");
    for(Int_t i = 0; i< 2; i++){
        if(V0ReaderName[i].Contains("On-the-Fly"))
            legendWeightPtOnflySecSubCock->AddEntry(histWeight[i],Form("%s, %s",periodName[i].Data(),generatorName[i].Data()));
    }

    for(Int_t j=0; j < nBinsR; j++){
        place  = place + 1;
        padMBWeightEachROnflySecSubCock->cd(place);
        padMBWeightEachROnflySecSubCock->cd(place)->SetTopMargin(0.04);
        padMBWeightEachROnflySecSubCock->cd(place)->SetBottomMargin(0.15);
        padMBWeightEachROnflySecSubCock->cd(place)->SetLeftMargin(0.15);
        padMBWeightEachROnflySecSubCock->cd(place)->SetRightMargin(0.05);
        histoDummyWeightEachROnflySecSubCock->GetYaxis()->SetNdivisions(504);

//         if(j>1 && j<=3)      histoDummyWeightEachROnfly->GetYaxis()->SetRangeUser(0.9,1.3);
//         else if(j>3 && j<=6) histoDummyWeightEachROnfly->GetYaxis()->SetRangeUser(0.9,1.2);
//         else if(j>7 )        histoDummyWeightEachROnfly->GetYaxis()->SetRangeUser(0.9,1.1);
        histoDummyWeightEachROnflySecSubCock->GetYaxis()->SetRangeUser(0.82,1.5);

        histoDummyWeightEachROnflySecSubCock->DrawCopy();
        DrawGammaLines(0.,1.,1., 1.,1.,kGray,1);
        DrawGammaLines(0.4,0.4,0.82,1.3,1.,kGray,2);
	//	DrawGammaLines(0.1,0.1,0.001, 10.1,1.,kGray,1);
	DrawGammaLines(0.05,0.05,0.5, 10.1,1.,kGray,1);
	DrawGammaLines(0.1,0.1,0.5, 10.1,1.,kGray,1);
	DrawGammaLines(0.2,0.2,0.5, 10.1,1.,kGray,1);
	DrawGammaLines(0.3,0.3,0.5, 10.1,1.,kGray,1);
	DrawGammaLines(0.05,0.05,0.5, 10.1,1.,kGray,1);



        for(Int_t i = 0; i< 2; i++){
            if(i<12) DrawGammaSetMarker(histoWeightsEachRPtMinSecSubUsingCocktail[j][i], marker[i], 0.5,color[i],color[i]);
            else     DrawGammaSetMarker(histoWeightsEachRPtMinSecSubUsingCocktail[j][i], marker[i], 0.5,color[i-12],color[i-12]);
	    if(j<2) histoWeightsEachRPtMinSecSubUsingCocktail[j][i]->SetLineStyle(3);
	    histoWeightsEachRPtMinSecSubUsingCocktail[j][i]->Draw("same");

	    // remove plotting the purity simultaneously, creates too busy plot   
	    // if(i<12) DrawGammaSetMarker(histoPurityPtEachRBin[j][i], marker[i], 0.5,color[i],color[i]);
            // else     DrawGammaSetMarker(histoPurityPtEachRBin[j][i], marker[i], 0.5,color[i-12],color[i-12]);
	    // histoPurityPtEachRBin[j][i]->SetLineStyle(3);
	    // histoPurityPtEachRBin[j][i]->Draw("same");
	    // if(i<12) DrawGammaSetMarker(histoWeightsEachRPtMinSecSub[j][i], marker[i], 0.5,6,color[i]);
            // else     DrawGammaSetMarker(histoWeightsEachRPtMinSecSub[j][i], marker[i], 0.5,6,color[i-12]);
            // histoWeightsEachRPtMinSecSub[j][i]->Draw("same");
        }
        if (j==0)  legendWeightPtOnflySecSubCock->Draw();
	TLatex *latexBinning = new TLatex(0.25,0.8,Form("R: %s ",arrayRangesRBins[j].Data()));
	SetStyleTLatex( latexBinning, sizeTextNameBins,2);
	latexBinning->Draw();
    }

    canvasMBWeightEachROnflySecSubCock->Print(Form("%s/MBWeightVSPtMinOnflyEachRSecSubCock_%s.%s",outputDir.Data(),cutVariationName.Data(),suffix.Data()));



 //_______________________ Ploting weights vs pT in each R bin Offline findersecondary subtracted using cocktail__________________________

    TCanvas *canvasMBWeightEachROfflineSecSubCock          = new TCanvas("canvasMBWeighEachROfflineSecSubCock","",1400,900);  // gives the page size
    DrawGammaCanvasSettings( canvasMBWeightEachROfflineSecSubCock, 0, 0, 0, 0);
    canvasMBWeightEachROfflineSecSubCock->cd();
    TPad * padMBWeightEachROfflineSecSubCock               = new TPad("padMBWeightEachROfflineSecSubCock","",-0.0,0.0,1.,1.,0);   // gives the size of the histo areas
    DrawGammaPadSettings( padMBWeightEachROfflineSecSubCock, 0, 0, 0, 0);
    padMBWeightEachROfflineSecSubCock->Divide(4,3,0.0,0.0);
    padMBWeightEachROfflineSecSubCock->Draw();
    place  = 0;
    TH2F *histoDummyWeightEachROfflineSecSubCock =  new TH2F("histoDummyWeightEachROfflineSecSubCock","histoDummyWeightEachROfflineSecSubCock",1000,-0.05,1.,1000,0.5,2.);
    SetStyleHistoTH2ForGraphs(histoDummyWeightEachROfflineSecSubCock, "#it{p}_{T}^{Min} (GeV/c)","w_i", 0.05,0.05, 0.05,0.05);

    TLegend* legendWeightPtOfflineSecSubCock = new TLegend(0.4,0.31-(1+counterOnfly)*0.05,0.8,0.31); //0.17,0.13,0.5,0.24);
    legendWeightPtOfflineSecSubCock->SetFillColor(0);
    legendWeightPtOfflineSecSubCock->SetMargin(0.17);
    legendWeightPtOfflineSecSubCock->SetLineColor(0);
    legendWeightPtOfflineSecSubCock->SetTextFont(42);
    legendWeightPtOfflineSecSubCock->SetTextSize(0.05);
    legendWeightPtOfflineSecSubCock->SetHeader("Offline V0 finder SecSubCock");
    for(Int_t i = 2; i< NumberOfCuts; i++){
        if(V0ReaderName[i].Contains("Offline"))
            legendWeightPtOfflineSecSubCock->AddEntry(histWeight[i],Form("%s, %s",periodName[i].Data(),generatorName[i].Data()));
    }


    for(Int_t j=0; j < nBinsR; j++){
        place  = place + 1;
        padMBWeightEachROfflineSecSubCock->cd(place);
        padMBWeightEachROfflineSecSubCock->cd(place)->SetTopMargin(0.04);
        padMBWeightEachROfflineSecSubCock->cd(place)->SetBottomMargin(0.15);
        padMBWeightEachROfflineSecSubCock->cd(place)->SetLeftMargin(0.15);
        padMBWeightEachROfflineSecSubCock->cd(place)->SetRightMargin(0.05);
        histoDummyWeightEachROfflineSecSubCock->GetYaxis()->SetNdivisions(504);

//         if(j>1 && j<=3)      histoDummyWeightEachROffline->GetYaxis()->SetRangeUser(0.9,1.3);
//         else if(j>3 && j<=6) histoDummyWeightEachROffline->GetYaxis()->SetRangeUser(0.9,1.2);
//         else if(j>7 )       histoDummyWeightEachROffline->GetYaxis()->SetRangeUser(0.9,1.1);
        histoDummyWeightEachROfflineSecSubCock->GetYaxis()->SetRangeUser(0.82,4.5);

        histoDummyWeightEachROfflineSecSubCock->DrawCopy();
        DrawGammaLines(0.,1.,1., 1.,1.,kGray,1);
        DrawGammaLines(0.4,0.4,0.85, 1.5,1.,kGray,2);
	//	DrawGammaLines(0.1,0.1,0.001, 10.1,1.,kGray,1);
	DrawGammaLines(0.05,0.05,0.5, 10.1,1.,kGray,1);
	DrawGammaLines(0.1,0.1,0.5, 10.1,1.,kGray,1);
	DrawGammaLines(0.2,0.2,0.5, 10.1,1.,kGray,1);
	DrawGammaLines(0.3,0.3,0.5, 10.1,1.,kGray,1);
	DrawGammaLines(0.05,0.05,0.5, 10.1,1.,kGray,1);


        for(Int_t i = 2; i< NumberOfCuts; i++){
	  if(i<12) DrawGammaSetMarker(histoWeightsEachRPtMinSecSubUsingCocktail[j][i], marker[i], 0.5,color[i],color[i]);
	  else     DrawGammaSetMarker(histoWeightsEachRPtMinSecSubUsingCocktail[j][i], marker[i], 0.5,color[i-12],color[i-12]);
	  if(j<2)histoWeightsEachRPtMinSecSubUsingCocktail[j][i]->SetLineStyle(3);
	  histoWeightsEachRPtMinSecSubUsingCocktail[j][i]->Draw("same");
	  // if(i<12) DrawGammaSetMarker(histoPurityPtEachRBin[j][i], marker[i], 0.5,color[i],color[i]);
	  // else     DrawGammaSetMarker(histoPurityPtEachRBin[j][i], marker[i], 0.5,color[i-12],color[i-12]);
	  // histoPurityPtEachRBin[j][i]->SetLineStyle(3);
	  // histoPurityPtEachRBin[j][i]->Draw("same");

	  // if(i<12) DrawGammaSetMarker(histoWeightsEachRPtMinSecSub[j][i], marker[i], 0.5,6,color[i]);
	  // else     DrawGammaSetMarker(histoWeightsEachRPtMinSecSub[j][i], marker[i], 0.5,6,color[i-12]);
	  // histoWeightsEachRPtMinSecSub[j][i]->Draw("same");
        }

        if (j==0)  legendWeightPtOfflineSecSubCock->Draw();
	TLatex *latexBinning = new TLatex(0.25,0.8,Form("R: %s ",arrayRangesRBins[j].Data()));
	SetStyleTLatex( latexBinning, sizeTextNameBins,2);
	latexBinning->Draw();
    }
    canvasMBWeightEachROfflineSecSubCock->Print(Form("%s/MBWeightVSPtMinOfflineEachRSecSubCock_%s.%s",outputDir.Data(),cutVariationName.Data(),suffix.Data()));


   //____________________________________________________________________________________


    //------   Differential studies------------//

   //_______________________ Ploting weights vs pT in each R bin On-the-fly finder Secondary Subtracted usingCocktail__________________________

    TCanvas *canvasMBWeightEachRDiffOnflySecSubCock          = new TCanvas("canvasMBWeighEachRDiffOnflySecSubCock","",1400,900);  // gives the page size
    DrawGammaCanvasSettings( canvasMBWeightEachRDiffOnflySecSubCock, 0, 0, 0, 0);
    canvasMBWeightEachRDiffOnflySecSubCock->cd();
    TPad * padMBWeightEachRDiffOnflySecSubCock               = new TPad("padMBWeightEachRDiffOnflySecSubCock","",-0.0,0.0,1.,1.,0);   // gives the size of the histo areas
    DrawGammaPadSettings( padMBWeightEachRDiffOnflySecSubCock, 0, 0, 0, 0);
    padMBWeightEachRDiffOnflySecSubCock->Divide(4,3,0.0,0.0);
    padMBWeightEachRDiffOnflySecSubCock->Draw();
    place  = 0;
    TH2F *histoDummyWeightEachRDiffOnflySecSubCock =  new TH2F("histoDummyWeightEachRDiffOnflySecSubCock","histoDummyWeightEachRDiffOnflySecSubCock",1000,-0.05,1.,1000,0.5,10.);
    SetStyleHistoTH2ForGraphs(histoDummyWeightEachRDiffOnflySecSubCock, "#it{p}_{T} (GeV/c)","d#Omega_{i}/d #it{p}_{T} ", 0.05,0.05, 0.05,0.05);
      histoDummyWeightEachRDiffOnflySecSubCock->GetYaxis()->SetRangeUser(0.75,1.5);


    TLegend* legendWeightPtDiffOnflySecSubCock= new TLegend(0.3,0.31-(1+counterOnfly)*0.05,0.99,0.31); //0.17,0.13,0.5,0.24);
    legendWeightPtDiffOnflySecSubCock->SetFillColor(0);
    legendWeightPtDiffOnflySecSubCock->SetMargin(0.17);
    legendWeightPtDiffOnflySecSubCock->SetLineColor(0);
    legendWeightPtDiffOnflySecSubCock->SetTextFont(42);
    legendWeightPtDiffOnflySecSubCock->SetTextSize(0.05);
    legendWeightPtDiffOnflySecSubCock->SetHeader("On-the-Fly V0 finder SecSubCock");
        for(Int_t i = 0; i< 2; i++){
    // for(Int_t i = 0; i< NumberOfCuts; i++){
        if(V0ReaderName[i].Contains("On-the-Fly"))
            legendWeightPtDiffOnflySecSubCock->AddEntry(histWeight[i],Form("%s, %s",periodName[i].Data(),generatorName[i].Data()));
    }

    for(Int_t j=0; j < nBinsR; j++){
        place  = place + 1;
        padMBWeightEachRDiffOnflySecSubCock->cd(place);
        padMBWeightEachRDiffOnflySecSubCock->cd(place)->SetTopMargin(0.04);
        padMBWeightEachRDiffOnflySecSubCock->cd(place)->SetBottomMargin(0.15);
        padMBWeightEachRDiffOnflySecSubCock->cd(place)->SetLeftMargin(0.15);
        padMBWeightEachRDiffOnflySecSubCock->cd(place)->SetRightMargin(0.05);
        histoDummyWeightEachRDiffOnflySecSubCock->GetYaxis()->SetNdivisions(504);
	padMBWeightEachRDiffOnflySecSubCock->cd(place)->SetLogy(1);

//         if(j>1 && j<=3)      histoDummyWeightEachROnfly->GetYaxis()->SetRangeUser(0.9,1.3);
//         else if(j>3 && j<=6) histoDummyWeightEachROnfly->GetYaxis()->SetRangeUser(0.9,1.2);
//         else if(j>7 )        histoDummyWeightEachROnfly->GetYaxis()->SetRangeUser(0.9,1.1);
//        histoDummyWeightEachRDiffOnflySecSubCock->GetYaxis()->SetRangeUser(0.5,8.5);

	 // if(j>=0 && j<=3)     histoDummyWeightEachRDiffOnflySecSubCock->GetYaxis()->SetRangeUser(0.5,10.8);
	 // else if(j>3 && j<=6) histoDummyWeightEachRDiffOnflySecSubCock->GetYaxis()->SetRangeUser(0.5,10.5);
	 // else if(j>7)         histoDummyWeightEachRDiffOnflySecSubCock->GetYaxis()->SetRangeUser(0.5,10.5);

	 if(j>=0 && j<=3)     histoDummyWeightEachRDiffOnflySecSubCock->GetYaxis()->SetRangeUser(0.5,5.5);
	 else if(j>3 && j<=6) histoDummyWeightEachRDiffOnflySecSubCock->GetYaxis()->SetRangeUser(0.5,8.5);
	 else if(j>7)         histoDummyWeightEachRDiffOnflySecSubCock->GetYaxis()->SetRangeUser(0.5,2.5);


        histoDummyWeightEachRDiffOnflySecSubCock->DrawCopy();
        DrawGammaLines(0.,1.,1., 1.,1.,kGray,1);
        DrawGammaLines(0.4,0.4,0.82,1.3,1.,kGray,2);
	//	DrawGammaLines(0.1,0.1,0.001, 10.1,1.,kGray,1);
	DrawGammaLines(0.05,0.05,0.5, 10.1,1.,kGray,1);
	DrawGammaLines(0.1,0.1,0.5, 10.1,1.,kGray,1);
	DrawGammaLines(0.2,0.2,0.5, 10.1,1.,kGray,1);
	DrawGammaLines(0.3,0.3,0.5, 10.1,1.,kGray,1);
	DrawGammaLines(0.05,0.05,0.5, 10.1,1.,kGray,1);


	for(Int_t i = 0; i< 2; i++){
	//  for(Int_t i = 0; i< NumberOfCuts; i++){
            if(i<12) DrawGammaSetMarker(histoWeightsEachRPtDiffSecSubUsingCocktail[j][i], marker[i], 0.5,color[i],color[i]);
            else     DrawGammaSetMarker(histoWeightsEachRPtDiffSecSubUsingCocktail[j][i], marker[i], 0.5,color[i-12],color[i-12]);
	    if(j<2) histoWeightsEachRPtDiffSecSubUsingCocktail[j][i]->SetLineStyle(3);
	    histoWeightsEachRPtDiffSecSubUsingCocktail[j][i]->Draw("same");

	    fitToRatio             = new TF1("fitRatio", "1.0+[0]/TMath::Power((x-[1]), [2])", ptMinFitPowerLaw[j], 0.7);
	    fitToRatio->SetParameters(1., -1., 1.);

	    fitToRatio->SetName(Form("%s_fit", histoWeightsEachRPtDiffSecSubUsingCocktail[j][i]->GetName()));
	    //          TFitResultPtr fitToRatioResult      = histoWeightsEachRPtDiffSecSubUsingCocktail[j][i]->Fit(fitToRatio, "SIMNRE");
	    //  TFitResultPtr fitToRatioResult      = histoWeightsEachRPtDiffSecSubUsingCocktail[j][i]->Fit(fitToRatio, "SIMRE");
	    //fitStatus                           = fitToRatioResult;



	    DrawGammaLines(0.0,1.,1.1,1.1,1.,kGray,2);
	    DrawGammaLines(0.0,1.,1.05,1.05,1.,kGray,2);
	    DrawGammaLines(0.0,1.,1.2,1.2,1.,kGray,2);
	    DrawGammaLines(0.0,1.,1.5,1.5,1.,kGray,2);
	    DrawGammaLines(0.0,1.,0.9,0.9,1.,kGray,2);
	    DrawGammaLines(0.0,1.,0.95,0.95,1.,kGray,2);
	    DrawGammaLines(0.0,1.,0.8,0.8,1.,kGray,2);
	    DrawGammaLines(0., maxR,0.98, 0.98,1.1,kRed+1,2);
    

	    // remove plotting the purity simultaneously, creates too busy plot   
	    // if(i<12) DrawGammaSetMarker(histoPurityPtEachRBin[j][i], marker[i], 0.5,color[i],color[i]);
            // else     DrawGammaSetMarker(histoPurityPtEachRBin[j][i], marker[i], 0.5,color[i-12],color[i-12]);
	    // histoPurityPtEachRBin[j][i]->SetLineStyle(3);
	    // histoPurityPtEachRBin[j][i]->Draw("same");
	    // if(i<12) DrawGammaSetMarker(histoWeightsEachRPtMinSecSub[j][i], marker[i], 0.5,6,color[i]);
            // else     DrawGammaSetMarker(histoWeightsEachRPtMinSecSub[j][i], marker[i], 0.5,6,color[i-12]);
            // histoWeightsEachRPtMinSecSub[j][i]->Draw("same");
        }
        if (j==0)  legendWeightPtOnflySecSubCock->Draw();
	TLatex *latexBinning = new TLatex(0.45,0.85,Form("R: %s ",arrayRangesRBins[j].Data()));
	SetStyleTLatex( latexBinning, sizeTextNameBins,2);
	latexBinning->Draw();
    }

    canvasMBWeightEachRDiffOnflySecSubCock->Print(Form("%s/MBWeightVSPtDiffOnflyEachRSecSubCock_%s.%s",outputDir.Data(),cutVariationName.Data(),suffix.Data()));




 //_______________________ Ploting weights vs pT in each R bin Offline findersecondary subtracted using cocktail__________________________

    TCanvas *canvasMBWeightEachRDiffOfflineSecSubCock          = new TCanvas("canvasMBWeighEachRDiffOfflineSecSubCock","",1400,900);  // gives the page size
    DrawGammaCanvasSettings( canvasMBWeightEachRDiffOfflineSecSubCock, 0, 0, 0, 0);
    canvasMBWeightEachRDiffOfflineSecSubCock->cd();
    TPad * padMBWeightEachRDiffOfflineSecSubCock               = new TPad("padMBWeightEachRDiffOfflineSecSubCock","",-0.0,0.0,1.,1.,0);   // gives the size of the histo areas
    DrawGammaPadSettings( padMBWeightEachRDiffOfflineSecSubCock, 0, 0, 0, 0);
    padMBWeightEachRDiffOfflineSecSubCock->Divide(4,3,0.0,0.0);
    padMBWeightEachRDiffOfflineSecSubCock->Draw();
    place  = 0;
    TH2F *histoDummyWeightEachRDiffOfflineSecSubCock =  new TH2F("histoDummyWeightEachRDiffOfflineSecSubCock","histoDummyWeightEachRDiffOfflineSecSubCock",1000,-0.05,1.,1000,0.5,10.);
    SetStyleHistoTH2ForGraphs(histoDummyWeightEachRDiffOfflineSecSubCock, "#it{p}_{T} (GeV/c)","d#Omega_{i}/d#it{p}_{T}", 0.05,0.05, 0.05,0.05);
    // histoDummyWeightEachRDiffOfflineSecSubCock->GetYaxis()->SetRangeUser(0.75,1.5);

    TLegend* legendWeightPtDiffOfflineSecSubCock = new TLegend(0.3,0.31-(1+counterOnfly)*0.05,0.8,0.31); //0.17,0.13,0.5,0.24);
    legendWeightPtDiffOfflineSecSubCock->SetFillColor(0);
    legendWeightPtDiffOfflineSecSubCock->SetMargin(0.17);
    legendWeightPtDiffOfflineSecSubCock->SetLineColor(0);
    legendWeightPtDiffOfflineSecSubCock->SetTextFont(42);
    legendWeightPtDiffOfflineSecSubCock->SetTextSize(0.05);
    legendWeightPtDiffOfflineSecSubCock->SetHeader("Offline V0 finder SecSubCock");
    for(Int_t i = 2; i< NumberOfCuts; i++){
        if(V0ReaderName[i].Contains("Offline"))
            legendWeightPtDiffOfflineSecSubCock->AddEntry(histWeight[i],Form("%s, %s",periodName[i].Data(),generatorName[i].Data()));
    }

    for(Int_t j=0; j < nBinsR; j++){
        place  = place + 1;
        padMBWeightEachRDiffOfflineSecSubCock->cd(place);
        padMBWeightEachRDiffOfflineSecSubCock->cd(place)->SetTopMargin(0.04);
        padMBWeightEachRDiffOfflineSecSubCock->cd(place)->SetBottomMargin(0.15);
        padMBWeightEachRDiffOfflineSecSubCock->cd(place)->SetLeftMargin(0.15);
        padMBWeightEachRDiffOfflineSecSubCock->cd(place)->SetRightMargin(0.05);
        histoDummyWeightEachRDiffOfflineSecSubCock->GetYaxis()->SetNdivisions(504);
	padMBWeightEachRDiffOfflineSecSubCock->cd(place)->SetLogy(1);

//         if(j>1 && j<=3)      histoDummyWeightEachROffline->GetYaxis()->SetRangeUser(0.9,1.3);
//         else if(j>3 && j<=6) histoDummyWeightEachROffline->GetYaxis()->SetRangeUser(0.9,1.2);
//         else if(j>7 )       histoDummyWeightEachROffline->GetYaxis()->SetRangeUser(0.9,1.1);
	if(j>=0 && j<=3)     histoDummyWeightEachRDiffOfflineSecSubCock->GetYaxis()->SetRangeUser(0.5,10.8);
	else if(j>3 && j<=6) histoDummyWeightEachRDiffOfflineSecSubCock->GetYaxis()->SetRangeUser(0.5,10.5);
	else if(j>7)     histoDummyWeightEachRDiffOfflineSecSubCock->GetYaxis()->SetRangeUser(0.5,10.5);

        histoDummyWeightEachRDiffOfflineSecSubCock->DrawCopy();
        DrawGammaLines(0.,1.,1., 1.,1.,kGray,1);
        DrawGammaLines(0.4,0.4,0.85, 1.5,1.,kGray,2);
	DrawGammaLines(0., maxR,0.98, 0.98,1.1,kRed+1,2);
	// 	DrawGammaLines(0.1,0.1,0.001, 10.1,1.,kGray,1);
	DrawGammaLines(0.05,0.05,0.5, 10.1,1.,kGray,1);
	DrawGammaLines(0.1,0.1,0.5, 10.1,1.,kGray,1);
	DrawGammaLines(0.2,0.2,0.5, 10.1,1.,kGray,1);
	DrawGammaLines(0.3,0.3,0.5, 10.1,1.,kGray,1);
	DrawGammaLines(0.05,0.05,0.5, 10.1,1.,kGray,1);

   
        for(Int_t i = 2; i< NumberOfCuts; i++){
	  if(i<12) DrawGammaSetMarker(histoWeightsEachRPtDiffSecSubUsingCocktail[j][i], marker[i], 0.5,color[i],color[i]);
	  else     DrawGammaSetMarker(histoWeightsEachRPtDiffSecSubUsingCocktail[j][i], marker[i], 0.5,color[i-12],color[i-12]);
	  if(j<2)histoWeightsEachRPtDiffSecSubUsingCocktail[j][i]->SetLineStyle(3);
	  histoWeightsEachRPtDiffSecSubUsingCocktail[j][i]->Draw("same");
	  // if(i<12) DrawGammaSetMarker(histoPurityPtEachRBin[j][i], marker[i], 0.5,color[i],color[i]);
	  // else     DrawGammaSetMarker(histoPurityPtEachRBin[j][i], marker[i], 0.5,color[i-12],color[i-12]);
	  // histoPurityPtEachRBin[j][i]->SetLineStyle(3);
	  // histoPurityPtEachRBin[j][i]->Draw("same");

	  // if(i<12) DrawGammaSetMarker(histoWeightsEachRPtMinSecSub[j][i], marker[i], 0.5,6,color[i]);
	  // else     DrawGammaSetMarker(histoWeightsEachRPtMinSecSub[j][i], marker[i], 0.5,6,color[i-12]);
	  // histoWeightsEachRPtMinSecSub[j][i]->Draw("same");
        }

        if (j==0)  legendWeightPtDiffOfflineSecSubCock->Draw();
	TLatex *latexBinning = new TLatex(0.45,0.85,Form("R: %s ",arrayRangesRBins[j].Data()));
	SetStyleTLatex( latexBinning, sizeTextNameBins,2);
	latexBinning->Draw();
    }
    canvasMBWeightEachRDiffOfflineSecSubCock->Print(Form("%s/MBWeightVSPtDiffOfflineEachRSecSubCock_%s.%s",outputDir.Data(),cutVariationName.Data(),suffix.Data()));


   //____________________________________________________________________________________




    // Comparison two energies
   //------   Differential studies------------//

   //_______________________ Ploting weights vs pT in each R bin On-the-fly finder Secondary Subtracted usingCocktail__________________________

    TCanvas *canvasMBWeightEachRDiffOnflySecSubCockSta          = new TCanvas("canvasMBWeighEachRDiffOnflySecSubCockSta","",1400,900);  // gives the page size
    DrawGammaCanvasSettings( canvasMBWeightEachRDiffOnflySecSubCockSta, 0, 0, 0, 0);
    canvasMBWeightEachRDiffOnflySecSubCockSta->cd();
    TPad * padMBWeightEachRDiffOnflySecSubCockSta               = new TPad("padMBWeightEachRDiffOnflySecSubCockSta","",-0.0,0.0,1.,1.,0);   // gives the size of the histo areas
    DrawGammaPadSettings( padMBWeightEachRDiffOnflySecSubCockSta, 0, 0, 0, 0);
    padMBWeightEachRDiffOnflySecSubCockSta->Divide(4,3,0.0,0.0);
    padMBWeightEachRDiffOnflySecSubCockSta->Draw();
    place  = 0;
    TH2F *histoDummyWeightEachRDiffOnflySecSubCockSta =  new TH2F("histoDummyWeightEachRDiffOnflySecSubCockSta","histoDummyWeightEachRDiffOnflySecSubCock",1000,-0.05,1.,1000,0.5,10.);
    SetStyleHistoTH2ForGraphs(histoDummyWeightEachRDiffOnflySecSubCockSta, "#it{p}_{T} (GeV/c)","d#Omega_{i}/d #it{p}_{T} ", 0.05,0.05, 0.05,0.05);
    //histoDummyWeightEachRDiffOnflySecSubCockSta->GetYaxis()->SetRangeUser(0.75,1.5);

    TLegend* legendWeightPtDiffOnflySecSubCockSta = new TLegend(0.3,0.31-(1+counterOnfly)*0.05,0.99,0.31); //0.17,0.13,0.5,0.24);
    legendWeightPtDiffOnflySecSubCockSta->SetFillColor(0);
    legendWeightPtDiffOnflySecSubCockSta->SetMargin(0.17);
    legendWeightPtDiffOnflySecSubCockSta->SetLineColor(0);
    legendWeightPtDiffOnflySecSubCockSta->SetTextFont(42);
    legendWeightPtDiffOnflySecSubCockSta->SetTextSize(0.05);
    legendWeightPtDiffOnflySecSubCockSta->SetHeader("On-the-Fly V0 finder SecSubCock");
    //        for(Int_t i = 0; i< 2; i++){
    for(Int_t i = 0; i< NumberOfCuts; i++){
      if( generatorName[i].Contains("Pythia")  && V0ReaderName[i].Contains("On-the-Fly")){
            legendWeightPtDiffOnflySecSubCockSta->AddEntry(histWeight[i],Form("%s, %s",periodName[i].Data(),generatorName[i].Data()));
      }
    }

    for(Int_t j=0; j < nBinsR; j++){
        place  = place + 1;
        padMBWeightEachRDiffOnflySecSubCockSta->cd(place);
        padMBWeightEachRDiffOnflySecSubCockSta->cd(place)->SetTopMargin(0.04);
        padMBWeightEachRDiffOnflySecSubCockSta->cd(place)->SetBottomMargin(0.15);
        padMBWeightEachRDiffOnflySecSubCockSta->cd(place)->SetLeftMargin(0.15);
        padMBWeightEachRDiffOnflySecSubCockSta->cd(place)->SetRightMargin(0.05);
        histoDummyWeightEachRDiffOnflySecSubCockSta->GetYaxis()->SetNdivisions(504);
	//	padMBWeightEachRDiffOnflySecSubCockSta->cd(place)->SetLogy(1);
//         if(j>1 && j<=3)      histoDummyWeightEachROnfly->GetYaxis()->SetRangeUser(0.9,1.3);
//         else if(j>3 && j<=6) histoDummyWeightEachROnfly->GetYaxis()->SetRangeUser(0.9,1.2);
//         else if(j>7 )        histoDummyWeightEachROnfly->GetYaxis()->SetRangeUser(0.9,1.1);
        histoDummyWeightEachRDiffOnflySecSubCockSta->GetYaxis()->SetRangeUser(0.5,8.5);

	histoDummyWeightEachRDiffOnflySecSubCockSta->GetYaxis()->SetRangeUser(0.8,1.6);

	// if(j>=0 && j<=3)     histoDummyWeightEachRDiffOnflySecSubCockSta->GetYaxis()->SetRangeUser(0.5,10.8);
	// else if(j>3 && j<=6) histoDummyWeightEachRDiffOnflySecSubCockSta->GetYaxis()->SetRangeUser(0.5,10.5);
	// else if(j>7)         histoDummyWeightEachRDiffOnflySecSubCockSta->GetYaxis()->SetRangeUser(0.5,10.5);


        histoDummyWeightEachRDiffOnflySecSubCockSta->DrawCopy();
        DrawGammaLines(0.,1.,1., 1.,1.,kGray,1);
        DrawGammaLines(0.,1.,0.98, 0.98,1.,kGray,1);
        DrawGammaLines(0.4,0.4,0.82,1.3,1.,kGray,2);
	DrawGammaLines(0., maxR,0.98, 0.98,1.1,kRed+1,2);
	//    	DrawGammaLines(0.1,0.1,0.001, 10.1,1.,kGray,1);
	DrawGammaLines(0.05,0.05,0.5, 10.1,1.,kGray,1);
	DrawGammaLines(0.1,0.1,0.5, 10.1,1.,kGray,1);
	DrawGammaLines(0.2,0.2,0.5, 10.1,1.,kGray,1);
	DrawGammaLines(0.3,0.3,0.5, 10.1,1.,kGray,1);
	DrawGammaLines(0.05,0.05,0.5, 10.1,1.,kGray,1);



	//	for(Int_t i = 0; i< 2; i++){
	for(Int_t i = 0; i< NumberOfCuts; i++){
            if(i<12) DrawGammaSetMarker(histoWeightsEachRPtDiffSecSubUsingCocktail[j][i], marker[i], 0.5,color[i],color[i]);
            else     DrawGammaSetMarker(histoWeightsEachRPtDiffSecSubUsingCocktail[j][i], marker[i], 0.5,color[i-12],color[i-12]);
	    if(j<2) histoWeightsEachRPtDiffSecSubUsingCocktail[j][i]->SetLineStyle(3);
	    if(generatorName[i].Contains("Pythia") && V0ReaderName[i].Contains("On-the-Fly")){
	      histoWeightsEachRPtDiffSecSubUsingCocktail[j][i]->Draw("same");
	      Double_t level=histoWeightNch[i]->GetBinContent(j+1);
	      // cout << "level::"<< level<< " " << j << "  "  <<  histoWeightNch[i]->GetBinCenter(j+1)<< endl;
	      DrawGammaLines(0.2,0.7,level, level,1.,color[i],1);
	      // histoWeightsEachRPtMinSecSubUsingCocktail[j][i]->Draw("same");
	    }
	    // remove plotting the purity simultaneously, creates too busy plot   
	    // if(i<12) DrawGammaSetMarker(histoPurityPtEachRBin[j][i], marker[i], 0.5,color[i],color[i]);
            // else     DrawGammaSetMarker(histoPurityPtEachRBin[j][i], marker[i], 0.5,color[i-12],color[i-12]);
	    // histoPurityPtEachRBin[j][i]->SetLineStyle(3);
	    // histoPurityPtEachRBin[j][i]->Draw("same");
	    // if(i<12) DrawGammaSetMarker(histoWeightsEachRPtMinSecSub[j][i], marker[i], 0.5,6,color[i]);
            // else     DrawGammaSetMarker(histoWeightsEachRPtMinSecSub[j][i], marker[i], 0.5,6,color[i-12]);
            // histoWeightsEachRPtMinSecSub[j][i]->Draw("same");
        }
        if (j==0) legendWeightPtDiffOnflySecSubCockSta->Draw();
	TLatex *latexBinning = new TLatex(0.45,0.85,Form("R: %s ",arrayRangesRBins[j].Data()));
	SetStyleTLatex( latexBinning, sizeTextNameBins,2);
	latexBinning->Draw();
    }

    canvasMBWeightEachRDiffOnflySecSubCockSta->Print(Form("%s/MBWeightVSPtDiffOnflyEachRSecSubCock_Standard_%s.%s",outputDir.Data(),cutVariationName.Data(),suffix.Data()));




 //_______________________ Ploting weights vs pT in each R bin Offline findersecondary subtracted using cocktail__________________________

    TCanvas *canvasMBWeightEachRDiffOfflineSecSubCockSta          = new TCanvas("canvasMBWeighEachRDiffOfflineSecSubCockSta","",1400,900);  // gives the page size
    DrawGammaCanvasSettings( canvasMBWeightEachRDiffOfflineSecSubCockSta, 0, 0, 0, 0);
    canvasMBWeightEachRDiffOfflineSecSubCockSta->cd();
    TPad * padMBWeightEachRDiffOfflineSecSubCockSta              = new TPad("padMBWeightEachRDiffOfflineSecSubCockSta","",-0.0,0.0,1.,1.,0);   // gives the size of the histo areas
    DrawGammaPadSettings( padMBWeightEachRDiffOfflineSecSubCockSta, 0, 0, 0, 0);
    padMBWeightEachRDiffOfflineSecSubCockSta->Divide(4,3,0.0,0.0);
    padMBWeightEachRDiffOfflineSecSubCockSta->Draw();
    place  = 0;
    TH2F *histoDummyWeightEachRDiffOfflineSecSubCockSta =  new TH2F("histoDummyWeightEachRDiffOfflineSecSubCockSta","histoDummyWeightEachRDiffOfflineSecSubCock",1000,-0.05,1.,1000,0.5,8.);
    SetStyleHistoTH2ForGraphs(histoDummyWeightEachRDiffOfflineSecSubCockSta, "#it{p}_{T} (GeV/c)","d#Omega_{i}/d#it{p}_{T}", 0.05,0.05, 0.05,0.05);
    histoDummyWeightEachRDiffOfflineSecSubCockSta->GetYaxis()->SetRangeUser(0.75,1.5);

    TLegend* legendWeightPtDiffOfflineSecSubCockSta = new TLegend(0.3,0.31-(1+counterOnfly)*0.05,0.8,0.31); //0.17,0.13,0.5,0.24);
    legendWeightPtDiffOfflineSecSubCockSta->SetFillColor(0);
    legendWeightPtDiffOfflineSecSubCockSta->SetMargin(0.17);
    legendWeightPtDiffOfflineSecSubCockSta->SetLineColor(0);
    legendWeightPtDiffOfflineSecSubCockSta->SetTextFont(42);
    legendWeightPtDiffOfflineSecSubCockSta->SetTextSize(0.05);
    legendWeightPtDiffOfflineSecSubCockSta->SetHeader("Offline V0 finder SecSubCock");
    for(Int_t i = 1; i< NumberOfCuts; i++){
        if(generatorName[i].Contains("Pythia") &&  V0ReaderName[i].Contains("Offline"))
            legendWeightPtDiffOfflineSecSubCock->AddEntry(histWeight[i],Form("%s, %s",periodName[i].Data(),generatorName[i].Data()));
    }

    for(Int_t j=0; j < nBinsR; j++){
        place  = place + 1;
        padMBWeightEachRDiffOfflineSecSubCockSta->cd(place);
        padMBWeightEachRDiffOfflineSecSubCockSta->cd(place)->SetTopMargin(0.04);
        padMBWeightEachRDiffOfflineSecSubCockSta->cd(place)->SetBottomMargin(0.15);
        padMBWeightEachRDiffOfflineSecSubCockSta->cd(place)->SetLeftMargin(0.15);
        padMBWeightEachRDiffOfflineSecSubCockSta->cd(place)->SetRightMargin(0.05);
        histoDummyWeightEachRDiffOfflineSecSubCock->GetYaxis()->SetNdivisions(504);
	padMBWeightEachRDiffOfflineSecSubCockSta->cd(place)->SetLogy(1);

//         if(j>1 && j<=3)      histoDummyWeightEachROffline->GetYaxis()->SetRangeUser(0.9,1.3);
//         else if(j>3 && j<=6) histoDummyWeightEachROffline->GetYaxis()->SetRangeUser(0.9,1.2);
//         else if(j>7 )       histoDummyWeightEachROffline->GetYaxis()->SetRangeUser(0.9,1.1);
	if(j>=0 && j<=3)     histoDummyWeightEachRDiffOfflineSecSubCockSta->GetYaxis()->SetRangeUser(0.5,10.8);
	else if(j>3 && j<=6) histoDummyWeightEachRDiffOfflineSecSubCockSta->GetYaxis()->SetRangeUser(0.5,10.5);
	else if(j>7)     histoDummyWeightEachRDiffOfflineSecSubCockSta->GetYaxis()->SetRangeUser(0.5,10.5);

        histoDummyWeightEachRDiffOfflineSecSubCockSta->DrawCopy();
        DrawGammaLines(0.,1.,1., 1.,1.,kGray,1);
        DrawGammaLines(0.4,0.4,0.85, 1.5,1.,kGray,2);
	DrawGammaLines(0., maxR,0.98, 0.98,1.1,kRed+1,2);
	//	DrawGammaLines(0.1,0.1,0.001, 10.1,1.,kGray,1);
	DrawGammaLines(0.05,0.05,0.5, 10.1,1.,kGray,1);
	DrawGammaLines(0.1,0.1,0.5, 10.1,1.,kGray,1);
	DrawGammaLines(0.2,0.2,0.5, 10.1,1.,kGray,1);
	DrawGammaLines(0.3,0.3,0.5, 10.1,1.,kGray,1);
	DrawGammaLines(0.05,0.05,0.5, 10.1,1.,kGray,1);

        for(Int_t i = 1; i< NumberOfCuts; i++){
	  if(i<12) DrawGammaSetMarker(histoWeightsEachRPtDiffSecSubUsingCocktail[j][i], marker[i], 0.5,color[i],color[i]);
	  else     DrawGammaSetMarker(histoWeightsEachRPtDiffSecSubUsingCocktail[j][i], marker[i], 0.5,color[i-12],color[i-12]);
	  if(j<2)histoWeightsEachRPtDiffSecSubUsingCocktail[j][i]->SetLineStyle(3);
	  if(generatorName[i].Contains("Pythia") && V0ReaderName[i].Contains("Offline")){
	    histoWeightsEachRPtDiffSecSubUsingCocktail[j][i]->Draw("same");
	    //histoWeightsEachRPtMinSecSubUsingCocktail[j][i]->Draw("same");
	  }
	  // if(i<12) DrawGammaSetMarker(histoPurityPtEachRBin[j][i], marker[i], 0.5,color[i],color[i]);
	  // else     DrawGammaSetMarker(histoPurityPtEachRBin[j][i], marker[i], 0.5,color[i-12],color[i-12]);
	  // histoPurityPtEachRBin[j][i]->SetLineStyle(3);
	  // histoPurityPtEachRBin[j][i]->Draw("same");

	  // if(i<12) DrawGammaSetMarker(histoWeightsEachRPtMinSecSub[j][i], marker[i], 0.5,6,color[i]);
	  // else     DrawGammaSetMarker(histoWeightsEachRPtMinSecSub[j][i], marker[i], 0.5,6,color[i-12]);
	  // histoWeightsEachRPtMinSecSub[j][i]->Draw("same");
        }

        if (j==0)  legendWeightPtDiffOfflineSecSubCockSta->Draw();
	TLatex *latexBinning = new TLatex(0.45,0.85,Form("R: %s ",arrayRangesRBins[j].Data()));
	SetStyleTLatex( latexBinning, sizeTextNameBins,2);
	latexBinning->Draw();
    }
    canvasMBWeightEachRDiffOfflineSecSubCockSta->Print(Form("%s/MBWeightVSPtDiffOfflineEachRSecSubCockStandard_%s.%s",outputDir.Data(),cutVariationName.Data(),suffix.Data()));


   //____________________________________________________________________________________








    cout << "line " << __LINE__ << endl;

   //_______________________ Ploting efficiency vs pT in each R bin On-the-fly finder__________________________

    TCanvas *canvasMBEffiEachROnfly          = new TCanvas("canvasMBEffiEachROnfly","",1400,900);  // gives the page size
    DrawGammaCanvasSettings( canvasMBEffiEachROnfly, 0, 0, 0, 0);
    canvasMBEffiEachROnfly->cd();
    TPad * padMBEffiEachROnfly               = new TPad("padMBEffiEachROnfly","",-0.0,0.0,1.,1.,0);   // gives the size of the histo areas
    DrawGammaPadSettings( padMBEffiEachROnfly, 0, 0, 0, 0);
    padMBEffiEachROnfly->Divide(4,3,0.0,0.0);
    padMBEffiEachROnfly->Draw();
    Int_t place1  = 0;
    TH2F *histoDummyEffiEachROnfly =  new TH2F("histoDummyEffitEachROnfly","histoDummyEffiEachROnfly",1000,0.,10.,1000,1.e-5,10.5);
    SetStyleHistoTH2ForGraphs(histoDummyEffiEachROnfly, "#it{p}_{T}^{Min} (GeV/c)","Effi_i", 0.05,0.05, 0.05,0.05);
    cout << "line " << __LINE__ << endl;
    TLegend* legendEffiPtOnfly= new TLegend(0.5,0.51-(1+counterOnfly)*0.05,0.4,0.51); //0.17,0.13,0.5,0.24);
    legendEffiPtOnfly->SetFillColor(0);
    legendEffiPtOnfly->SetMargin(0.17);
    legendEffiPtOnfly->SetLineColor(0);
    legendEffiPtOnfly->SetTextFont(42);
    legendEffiPtOnfly->SetTextSize(0.05);
    legendEffiPtOnfly->SetHeader("On-the-Fly V0 finder");
    for(Int_t i = 0; i< 2; i++){
        if(V0ReaderName[i].Contains("On-the-Fly"))
            legendEffiPtOnfly->AddEntry(histWeight[i],Form("%s, %s",periodName[i].Data(),generatorName[i].Data()));
    }

    for(Int_t j=0; j < nBinsR; j++){
        place1  = place1 + 1;
        padMBEffiEachROnfly->cd(place1);
        padMBEffiEachROnfly->cd(place1)->SetTopMargin(0.04);
        padMBEffiEachROnfly->cd(place1)->SetBottomMargin(0.15);
        padMBEffiEachROnfly->cd(place1)->SetLeftMargin(0.15);
        padMBEffiEachROnfly->cd(place1)->SetRightMargin(0.05);
	padMBEffiEachROnfly->cd(place1)->SetLogy(1);
	padMBEffiEachROnfly->cd(place1)->SetLogx(1);
        histoDummyEffiEachROnfly->GetYaxis()->SetNdivisions(504);

//         if(j>1 && j<=3)      histoDummyWeightEachROnfly->GetYaxis()->SetRangeUser(0.9,1.3);
//         else if(j>3 && j<=6) histoDummyWeightEachROnfly->GetYaxis()->SetRangeUser(0.9,1.2);
//         else if(j>7 )        histoDummyWeightEachROnfly->GetYaxis()->SetRangeUser(0.9,1.1);
	histoDummyEffiEachROnfly->GetYaxis()->SetRangeUser(1.e-5,10.2);
        histoDummyEffiEachROnfly->GetXaxis()->SetRangeUser(0.03,10.);

        histoDummyEffiEachROnfly->DrawCopy();
	//cout << "line " << __LINE__ << endl;
        DrawGammaLines(0.,10.,1., 1.,1.,kGray,1);
        DrawGammaLines(0.1,0.1,0.001, 1.1,1.,kGray,1);
	DrawGammaLines(0.2,0.2,0.001, 1.1,1.,kGray,1);
	DrawGammaLines(0.3,0.3,0.001, 1.1,1.,kGray,1);
	DrawGammaLines(0.05,0.05,0.001, 1.1,1.,kGray,1);
	DrawGammaLines(0.1,0.1,0.5, 10.1,1.,kGray,1);
	DrawGammaLines(0.2,0.2,0.5, 10.1,1.,kGray,1);
	DrawGammaLines(0.3,0.3,0.5, 10.1,1.,kGray,1);
	DrawGammaLines(0.05,0.05,0.5, 10.1,1.,kGray,1);


	//        DrawGammaLines(0.4,0.4,0.82,1.3,1.,kGray,2);
	//cout << "line " << __LINE__ << endl;
        for(Int_t i = 0; i< 4; i++){
            if(i<12) DrawGammaSetMarker(histoTrueRecEffEachRBin[j][i] , marker[i], 0.5,color[i],color[i]);
            else     DrawGammaSetMarker(histoTrueRecEffEachRBin[j][i] , marker[i], 0.5,color[i-12],color[i-12]);
	    //if(j<2) histoTrueRecEffEachRBin[j][i]->SetLineStyle(3);
	    // cout << "line " << __LINE__ << "nBinsR::"<< j<< endl;
	    histoTrueRecEffEachRBin[j][i]->Draw("same");
	    if(i<2) histoWeightsEachRPtDiffSecSubUsingCocktail[j][i]->Draw("same");
	    // if(i<12) DrawGammaSetMarker(histoWeightsEachRPtMinSecSub[j][i], marker[i], 0.5,6,color[i]);
            // else     DrawGammaSetMarker(histoWeightsEachRPtMinSecSub[j][i], marker[i], 0.5,6,color[i-12]);
            // histoWeightsEachRPtMinSecSub[j][i]->Draw("same");
        }
	if (j==0)  legendEffiPtOnfly->Draw();
	TLatex *latexBinning = new TLatex(0.25,0.8,Form("R: %s ",arrayRangesRBins[j].Data()));
	SetStyleTLatex( latexBinning, sizeTextNameBins,2);
	latexBinning->Draw();
    }

    cout << "line " << __LINE__ << endl;
    canvasMBEffiEachROnfly->Print(Form("%s/MBEffiVSPtMinOnflyEachR_%s.%s",outputDir.Data(),cutVariationName.Data(),suffix.Data()));

    //-------------------------------------------------
    // Plotting efficiency and Omega in one single plot

   TCanvas *canvasMBEffiEachROnflySingle          = new TCanvas("canvasMBEffiEachROnflySingle","",900,1500);  // gives the page size
    DrawGammaCanvasSettings( canvasMBEffiEachROnflySingle, 0.18, 0.03, 0.035, 0.19);
    // canvasMBEffiEachROnflySingle->cd();
    // TPad * padMBEffiEachROnflySingle               = new TPad("padMBEffiEachROnflySingle","",-0.0,0.0,1.,1.,0);   // gives the size of the histo areas
    // DrawGammaPadSettings( padMBEffiEachROnflySingle, 0, 0, 0, 0);
    // padMBEffiEachROnflySingle->SetLogy(1);
    // padMBEffiEachROnflySingle->SetLogx(1);
    // padMBEffiEachROnflySingle->Draw();

    canvasMBEffiEachROnflySingle->SetLogy(1);
    canvasMBEffiEachROnflySingle->SetLogx(1);

    TH2F *histoDummyEffiEachROnflySingle =  new TH2F("histoDummyEffitEachROnflySingle","histoDummyEffiEachROnflySingle",1000,0.,10.,1000,1.e-5,10.5);
    SetStyleHistoTH2ForGraphs(histoDummyEffiEachROnflySingle, "#it{p}_{T} (GeV/c)","Effi_{i},d#Omega/d#it{p}_{T}", 0.05,0.05, 0.05,0.05);
    cout << "line " << __LINE__ << endl;
    TLegend* legendEffiPtOnflySingle= new TLegend(0.6,0.25,0.9,0.71); //0.17,0.13,0.5,0.24);
    legendEffiPtOnflySingle->SetFillColor(0);
    legendEffiPtOnflySingle->SetMargin(0.17);
    legendEffiPtOnflySingle->SetLineColor(0);
    legendEffiPtOnflySingle->SetTextFont(42);
    legendEffiPtOnflySingle->SetTextSize(0.02);
    legendEffiPtOnflySingle->SetHeader(Form("On-the-Fly V0 finder, %s",optionEnergy.Data()));

    histoDummyEffiEachROnflySingle->GetYaxis()->SetNdivisions(504);
    histoDummyEffiEachROnflySingle->GetYaxis()->SetRangeUser(3.e-5,8.);
    histoDummyEffiEachROnflySingle->GetXaxis()->SetRangeUser(0.03,5.);

    histoDummyEffiEachROnflySingle->DrawCopy(); 
    for(Int_t j=1; j < nBinsR; j++){
    //for(Int_t j=1; j < 3; j++){
      // place1  = place1 + 1;
        // padMBEffiEachROnfly->cd(place1);
        // padMBEffiEachROnfly->cd(place1)->SetTopMargin(0.04);
        // padMBEffiEachROnfly->cd(place1)->SetBottomMargin(0.15);
        // padMBEffiEachROnfly->cd(place1)->SetLeftMargin(0.15);
        // padMBEffiEachROnfly->cd(place1)->SetRightMargin(0.05);
	// padMBEffiEachROnfly->cd(place1)->SetLogy(1);
	// padMBEffiEachROnfly->cd(place1)->SetLogx(1);


//         if(j>1 && j<=3)      histoDummyWeightEachROnfly->GetYaxis()->SetRangeUser(0.9,1.3);
//         else if(j>3 && j<=6) histoDummyWeightEachROnfly->GetYaxis()->SetRangeUser(0.9,1.2);
//         else if(j>7 )        histoDummyWeightEachROnfly->GetYaxis()->SetRangeUser(0.9,1.1);

 
	//cout << "line " << __LINE__ << endl;
        DrawGammaLines(0.,10.,1., 1.,1.,kGray,1);
	// DrawGammaLines(0.05,0.05,0.001, 1.1,1.,kGray,1);
	// DrawGammaLines(0.1,0.1,0.5, 10.1,1.,kGray,1);
	// DrawGammaLines(0.2,0.2,0.5, 10.1,1.,kGray,1);
	// DrawGammaLines(0.3,0.3,0.5, 10.1,1.,kGray,1);



	//        DrawGammaLines(0.4,0.4,0.82,1.3,1.,kGray,2);
	//cout << "line " << __LINE__ << endl;
        for(Int_t i = 0; i< 1; i++){
            // if(i<12) DrawGammaSetMarker(histoTrueRecEffEachRBin[j][i] , marker[i], 0.5,color[i],color[i]);
            // else     DrawGammaSetMarker(histoTrueRecEffEachRBin[j][i] , marker[i], 0.5,color[i-12],color[i-12]);
	  Int_t iMark= 16+j;
	  //	  Int_t iCol= 29+j;
	  Int_t iCol= 1;
	  DrawGammaSetMarker(histoTrueRecEffEachRBin[j][i] , iMark, 1.1, color[j],color[j] );
	  DrawGammaSetMarker(histoWeightsEachRPtDiffSecSubUsingCocktail[j][i] , iMark, 1.1,color[j],color[j]);

	    //if(j<2) histoTrueRecEffEachRBin[j][i]->SetLineStyle(3);
	    // cout << "line " << __LINE__ << "nBinsR::"<< j<< endl;
	  histoTrueRecEffEachRBin[j][i]->DrawCopy("same");
	  histoWeightsEachRPtDiffSecSubUsingCocktail[j][i]->DrawCopy("same");
	  legendEffiPtOnflySingle->AddEntry(histoWeightsEachRPtDiffSecSubUsingCocktail[j][i],Form(" %s",arrayRangesRBins[j].Data()));
	  // legendEffiPtOnflySingle->AddEntry(histoTrueRecEffEachRBin[j][i],"#epsilon");
	    // if(i<12) DrawGammaSetMarker(histoWeightsEachRPtMinSecSub[j][i], marker[i], 0.5,6,color[i]);
            // else     DrawGammaSetMarker(histoWeightsEachRPtMinSecSub[j][i], marker[i], 0.5,6,color[i-12]);
            // histoWeightsEachRPtMinSecSub[j][i]->Draw("same");
        }
	//	if (j==0)  
	// TLatex *latexBinning = new TLatex(0.25,0.8,Form("R: %s ",arrayRangesRBins[j].Data()));
	// SetStyleTLatex( latexBinning, sizeTextNameBins,2);
	// latexBinning->Draw();
    }
    legendEffiPtOnflySingle->Draw();

    canvasMBEffiEachROnflySingle->Update();

    cout << "line " << __LINE__ << endl;
    canvasMBEffiEachROnflySingle->Print(Form("%s/MBEffiVSPtMinOnflyEachRSingle_%s.%s",outputDir.Data(),cutVariationName.Data(),suffix.Data()));





    //-------------------------------------------------
    // Plotting  Omega in one single plot

    TCanvas *canvasMBOmegaEachROnflySingle          = new TCanvas("canvasMBOmegaEachROnflySingle","",900,1500);  // gives the page size
    DrawGammaCanvasSettings( canvasMBOmegaEachROnflySingle, 0.18, 0.03, 0.035, 0.19);
    canvasMBOmegaEachROnflySingle->SetLogy(1);
    canvasMBOmegaEachROnflySingle->SetLogx(1);

    TH2F *histoDummyOmegaEachROnflySingle =  new TH2F("histoDummyOmegatEachROnflySingle","histoDummyOmegaEachROnflySingle",1000,0.,10.,1000,1.e-5,10.5);
    SetStyleHistoTH2ForGraphs(histoDummyOmegaEachROnflySingle, "#it{p}_{T} (GeV/c)","d#Omega/d#it{p}_{T}", 0.05,0.05, 0.05,0.05);
    cout << "line " << __LINE__ << endl;
    TLegend* legendOmegaPtOnflySingle= new TLegend(0.7,0.45,0.9,0.91); //0.17,0.13,0.5,0.24);
    legendOmegaPtOnflySingle->SetFillColor(0);
    legendOmegaPtOnflySingle->SetMargin(0.17);
    legendOmegaPtOnflySingle->SetLineColor(0);
    legendOmegaPtOnflySingle->SetTextFont(42);
    legendOmegaPtOnflySingle->SetTextSize(0.02);
    legendOmegaPtOnflySingle->SetHeader(Form("On-the-Fly V0 finder, %s",optionEnergy.Data()));

    histoDummyOmegaEachROnflySingle->GetYaxis()->SetNdivisions(504);
    histoDummyOmegaEachROnflySingle->GetYaxis()->SetRangeUser(0.8,8.);
    histoDummyOmegaEachROnflySingle->GetXaxis()->SetRangeUser(0.03,1.);
    histoDummyOmegaEachROnflySingle->DrawCopy(); 

    for(Int_t j=1; j < nBinsR; j++){

        DrawGammaLines(0.,10.,1., 1.,1.,kGray,1);

        for(Int_t i = 0; i< 1; i++){
            // if(i<12) DrawGammaSetMarker(histoTrueRecEffEachRBin[j][i] , marker[i], 0.5,color[i],color[i]);
            // else     DrawGammaSetMarker(histoTrueRecEffEachRBin[j][i] , marker[i], 0.5,color[i-12],color[i-12]);
	  Int_t iMark= 16+j;
	  //	  Int_t iCol= 29+j;
	  Int_t iCol= 1;
	  //	  DrawGammaSetMarker(histoTrueRecEffEachRBin[j][i] , iMark, 1.1, color[j],color[j] );
	  DrawGammaSetMarker(histoWeightsEachRPtDiffSecSubUsingCocktail[j][i] , iMark, 1.1,color[j],color[j]);
	  histoWeightsEachRPtDiffSecSubUsingCocktail[j][i]->DrawCopy("same");
	  legendOmegaPtOnflySingle->AddEntry(histoWeightsEachRPtDiffSecSubUsingCocktail[j][i],Form(" %s",arrayRangesRBins[j].Data()));

        }

    }
    legendOmegaPtOnflySingle->Draw();

    canvasMBOmegaEachROnflySingle->Update();

    cout << "line " << __LINE__ << endl;
    canvasMBOmegaEachROnflySingle->Print(Form("%s/MBOmegaVSPtMinOnflyEachRSingle_%s.%s",outputDir.Data(),cutVariationName.Data(),suffix.Data()));








    TFile outFile(Form("%s/weightsWithErrors_Test.root",outputDir.Data()) ,"RECREATE");
    histWeightStaErr->Write();
    histWeightSysErr->Write();
    histWeightTotErr->Write();
    fProfileContainingMaterialBudgetWeights->Write();
    fProfileContainingMaterialBudgetWeightsUpTotErr->Write();
    fProfileContainingMaterialBudgetWeightsDownTotErr->Write();
    outFile.Close();

 }
