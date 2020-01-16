//  **********************************************************************************
//  ******     provided by Gamma Conversion Group, PWGGA,                        *****
//  ******     Friederike Bock, friederike.bock@cern.ch                          *****
//  **********************************************************************************

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
#include "TLatex.h"
#include "TASImage.h"
#include "TPostScript.h"
#include "TGraphErrors.h"
#include "TArrow.h"
#include "TMarker.h"
#include "TGraphAsymmErrors.h" 
#include "TFitResultPtr.h"
#include "TFitResult.h"
#include "TDecayChannel.h"
#include "../CommonHeaders/PlottingGammaConversionHistos.h"
#include "../CommonHeaders/PlottingGammaConversionAdditional.h"
#include "../CommonHeaders/FittingGammaConversion.h"
#include "../CommonHeaders/ConversionFunctions.h"
#include "../CommonHeaders/ConversionFunctionsBasicsAndLabeling.h"
// #include "CalculateGammaToPi0.h"
//#include "ExtractSignal.h" 

struct SysErrorConversion {
	Double_t value;
	Double_t error;
	// TString name;
};

void CorrectYield(TH1D* histoCorrectedYield,TH1D* histoEffiPt, TH1D* histoAcceptance, Double_t deltaRapid, Double_t scaling, Double_t nEvt, TString nameMeson,Int_t DecayChannelIndex =0){
	histoCorrectedYield->Sumw2();
	histoCorrectedYield->Scale(1./nEvt);
    histoCorrectedYield->Divide(histoCorrectedYield,histoEffiPt,1.,1.,"");
	cout << "bins corrected yield = " << histoCorrectedYield->GetNbinsX() << endl;
	for (Int_t i = 0; i < histoCorrectedYield->GetNbinsX(); i++)
	{
		cout << i  << " x: " << histoCorrectedYield->GetBinCenter(i) << " value: " << histoCorrectedYield->GetBinContent(i) << endl;
	}
	
	cout << "bins acceptance = " << histoAcceptance->GetNbinsX() << endl;
	for (Int_t i = 0; i < histoAcceptance->GetNbinsX(); i++)
	{
		cout << i  << " x: " << histoAcceptance->GetBinCenter(i) << " value: " << histoAcceptance->GetBinContent(i) << endl;
	}
	histoAcceptance->Print();
    histoCorrectedYield->Divide(histoCorrectedYield,histoAcceptance,1.,1.,"");
	histoCorrectedYield->Scale(1./deltaRapid);
	histoCorrectedYield->Scale(scaling);
	for (Int_t i = 1; i < histoCorrectedYield->GetNbinsX()+1 ; i++){
		Double_t newBinContent = histoCorrectedYield->GetBinContent(i)/histoCorrectedYield->GetBinCenter(i);
		Double_t newBinError = histoCorrectedYield->GetBinError(i)/histoCorrectedYield->GetBinCenter(i);
		histoCorrectedYield->SetBinContent(i,newBinContent);
		histoCorrectedYield->SetBinError(i,newBinError);
	}

    // Scale by Branching Ratio
    Double_t branchingRatio = 1.;

    if (nameMeson.CompareTo("Eta") == 0){
        branchingRatio = TDatabasePDG::Instance()->GetParticle(221)->DecayChannel(2)->BranchingRatio();
    }else if (nameMeson.CompareTo("Omega") == 0){
        branchingRatio = TDatabasePDG::Instance()->GetParticle(223)->DecayChannel(DecayChannelIndex)->BranchingRatio();
    }else{
        cout << "Branching Ratio not found, will be scaled with 1!" << endl;
	}
	cout << " I scaled with " << branchingRatio << endl;
     histoCorrectedYield->Scale(1./branchingRatio);
}

void CompileFullCorrectionFactor(TH1D* histoEffiPt, TH1D* histoAcceptance, Double_t deltaRapid){
	histoEffiPt->Sumw2();
	histoEffiPt->Multiply(histoEffiPt,histoAcceptance,1.,1.,"");
	histoEffiPt->Scale(deltaRapid);
}


void ScaleMCYield(TH1D* histoCorrectedToBeScaled, Double_t deltaRapid, Double_t scaling, Double_t nEvtMC, TString nameMeson, Bool_t optionDalitz,Int_t DecayChannelIndex = 0 ){
	histoCorrectedToBeScaled->Sumw2();
	histoCorrectedToBeScaled->Scale(1./deltaRapid);
	histoCorrectedToBeScaled->Scale(scaling);
	histoCorrectedToBeScaled->Scale(1./nEvtMC);
	for (Int_t i = 1; i < histoCorrectedToBeScaled->GetNbinsX()+1 ; i++){
		Double_t newBinContent = histoCorrectedToBeScaled->GetBinContent(i)/histoCorrectedToBeScaled->GetBinCenter(i);
		Double_t newBinError = histoCorrectedToBeScaled->GetBinError(i)/histoCorrectedToBeScaled->GetBinCenter(i);
		histoCorrectedToBeScaled->SetBinContent(i,newBinContent);
		histoCorrectedToBeScaled->SetBinError(i,newBinError);
	}
    // Scale by Branching Ratio
    Double_t branchingRatio = 1.;

    if (nameMeson.CompareTo("Eta") == 0){
        branchingRatio = TDatabasePDG::Instance()->GetParticle(221)->DecayChannel(2)->BranchingRatio();
        cout << "BRANCHING RATIO ETA=" << branchingRatio << endl;
    }else if (nameMeson.CompareTo("Omega") == 0){
        branchingRatio = TDatabasePDG::Instance()->GetParticle(223)->DecayChannel(DecayChannelIndex)->BranchingRatio();
    }else{
        cout << "Branching Ratio not found, will be scaled with 1!" << endl;
    }
     histoCorrectedToBeScaled->Scale(1./branchingRatio);
}


void  CorrectSignalPiPlPiMiPiZero(TString fileNameUnCorrectedFile = "myOutput",
                                  TString fileNameCorrectionFile  = "",
                                  TString fCutSelection           = "",
                                  TString suffix                  = "gif",
                                  TString nameMeson               = "",
                                  Bool_t  isMC                    = kFALSE,
                                  TString optionEnergy            = "",
                                  TString optionPeriod            = "",
                                  Int_t  optDecayChannel          = 0,
                                  Bool_t optDalitz                = kFALSE,
                                  Int_t mode                      = 9,
								  Bool_t useDiffAcceptance        = kFALSE,
								  TString fileNameAlternativeCorrectionFile  = ""
                                ){

    // ******************************************************************************************
    // ********************** general style settings ********************************************
    // ******************************************************************************************
	
	// SETTINGS FOR YIELD EXTRACTION ERROR:
    // 0: averaged back fit; 1: norm bck fit 2: true bckfit
	Int_t yieldExtractionMode = 2;

	gROOT->Reset();   
	gROOT->SetStyle("Plain");
	
	StyleSettingsThesis();  
	
	SetPlotStyle();
	
	TString fDalitz="";
	Bool_t kDalitz = optDalitz;
	if (kDalitz){
		fDalitz = "Dalitz";
	} else {
		fDalitz = "";
	}

    // *******************************************************************************************
    // *********************** setting global variables ******************************************
    // *******************************************************************************************
	
	TString outputDir = Form("%s/%s/%s/%s/CorrectSignalPiPlPiMiPiZero%s",fCutSelection.Data(),optionEnergy.Data(),optionPeriod.Data(),suffix.Data(), fDalitz.Data());
	gSystem->Exec("mkdir "+outputDir);
	
	
	TString date = ReturnDateString();
   
	//declaration for printing logo 
	Float_t  pictDrawingCoordinatesInv[9] =   {0.63, 0.8, 0.40, 0.04, 0.7, 0.43, 0.18, 0.035,0}; // kk
	TString  prefix2="";
   
	
	
	
	TString fTypeCutSelection="";
	TString fEventCutSelection="";
	TString fGammaCutSelection="";
	TString fClusterCutSelection="";
	TString fPionCutSelection="";
	TString fNeutralPionCutSelection="";
	TString fMesonCutSelection="";
	
	if (mode == 9){
		if (kDalitz){
			ReturnSeparatedCutNumber(fCutSelection, fGammaCutSelection, fPionCutSelection,fMesonCutSelection,kTRUE);
		} else {
			ReturnSeparatedCutNumber(fCutSelection, fGammaCutSelection, fPionCutSelection,fMesonCutSelection);
		}
		fEventCutSelection = fGammaCutSelection(0,7);
		fGammaCutSelection = fGammaCutSelection(7,fGammaCutSelection.Length()-7);
		cout << fEventCutSelection.Data() << "\t" << fGammaCutSelection.Data() << endl;
	} else {
        ReturnSeparatedCutNumberPiPlPiMiPiZero(fCutSelection,fTypeCutSelection,fEventCutSelection, fGammaCutSelection, fClusterCutSelection, fPionCutSelection, fNeutralPionCutSelection, fMesonCutSelection);
	}

	TString collisionSystem= ReturnFullCollisionsSystem(optionEnergy);
	Double_t energy = ReturnCollisionEnergy( optionEnergy);
	Double_t doubleAddFactorK0s = 0.;//ReturnCorrectK0ScalingFactor( optionEnergy,  fEventCutSelection);
    if (isMC == kTRUE){
		prefix2 =         "MC";
		doubleAddFactorK0s = 0.;
        cout << "running MC mode" << endl;
    } else {
		prefix2 =         "data";
	}
	cout << "The additional K0 correction factor is: "  << doubleAddFactorK0s<<endl;
    cout << "nameMeson = " << nameMeson << endl;
	TString textMeson=ReturnMesonString ( nameMeson);

	if (textMeson.CompareTo("") == 0) return;
	
	if (collisionSystem.CompareTo("") == 0){
		cout << "No correct collision system specification, has been given" << endl;
		return;     
	}

	
	TString intermediate = GetCentralityString(fGammaCutSelection);
	TString fTextCent ="";
	if (intermediate.CompareTo("pp")==0){
		fTextCent = "MinBias";  
		intermediate = "";
	} else {
		fTextCent = Form("%s central", intermediate.Data());
	}
	if (optionPeriod.CompareTo("") != 0){
		collisionSystem = Form("%s, %s",collisionSystem.Data(),optionPeriod.Data());
	}   
		
	TString rapidityRange = "";
	Double_t deltaRapid =  ReturnRapidityStringAndDouble(fMesonCutSelection, rapidityRange);
		
	//Variable defintion
	Double_t scaling = 1./(2.*TMath::Pi());

    // Variable to quickly change which type of yield is used
    TString InvMassTypeEnding = "_SubPiZero";
	TString BackFitString     = "";
    //TString InvMassTypeEnding = "";
	
	// File definitions
	cout << "fMesonCutSelection = " << fMesonCutSelection.Data() << endl;
	TFile fileUncorrected(fileNameUnCorrectedFile.Data());  
	if (fileUncorrected.IsZombie()) return;
    TH1F *histoNumberOfGoodESDTracksVtx     = (TH1F*)fileUncorrected.Get("GoodESDTracks");
    TH1D *histoEventQuality                 = (TH1D*)fileUncorrected.Get("NEvents");
    
	TString backMode = fMesonCutSelection(1,1);

	if(!backMode.CompareTo("4")) BackFitString = "BackFit";
   
    TH1D *histoUnCorrectedYield             = (TH1D*)fileUncorrected.Get(Form("histoYieldMeson%s%s",BackFitString.Data(),InvMassTypeEnding.Data()));

	TH1D *histoUnCorrectedYieldWide      ;  
    TH1D *histoUnCorrectedYieldNarrow    ;  
    TH1D *histoUnCorrectedYieldLeft      ;  
    TH1D *histoUnCorrectedYieldLeftWide  ;  
    TH1D *histoUnCorrectedYieldLeftNarrow;  
    TH1D *histoFWHMMeson                 ;  
    TH1D *histoFWHMMesonLeft             ;  
	
    TH1D *histoMassMeson                 ;  
    TH1D *histoMassMesonLeft             ;  

	// Backfit histos
	TH1D *histoUnCorrectedYieldBackFit = (TH1D*)fileUncorrected.Get(Form("histoYieldMesonBackFit%s",InvMassTypeEnding.Data()));
	TH1D *histoUnCorrectedYieldWideBackFit;  
    TH1D *histoUnCorrectedYieldNarrowBackFit;  
	TH1D *histoUnCorrectedYieldBackFitFromFit = (TH1D*)fileUncorrected.Get(Form("histoYieldMesonBackFitFromFit%s",InvMassTypeEnding.Data()));
    
	if(backMode.CompareTo("4")){ 
    	histoUnCorrectedYieldWide         = (TH1D*)fileUncorrected.Get(Form("histoYieldMesonWide%s",InvMassTypeEnding.Data()));
    	histoUnCorrectedYieldNarrow       = (TH1D*)fileUncorrected.Get(Form("histoYieldMesonNarrow%s",InvMassTypeEnding.Data()));
		histoUnCorrectedYieldWideBackFit  = (TH1D*)fileUncorrected.Get(Form("histoYieldWideMesonBackFit%s",InvMassTypeEnding.Data()));
    	histoUnCorrectedYieldNarrowBackFit = (TH1D*)fileUncorrected.Get(Form("histoYieldNarrowMesonBackFit%s",InvMassTypeEnding.Data()));
    	histoUnCorrectedYieldLeft         = (TH1D*)fileUncorrected.Get(Form("histoYieldMesonLeft%s",InvMassTypeEnding.Data()));
    	histoUnCorrectedYieldLeftWide     = (TH1D*)fileUncorrected.Get(Form("histoYieldMesonLeftWide%s",InvMassTypeEnding.Data()));
    	histoUnCorrectedYieldLeftNarrow   = (TH1D*)fileUncorrected.Get(Form("histoYieldMesonLeftNarrow%s",InvMassTypeEnding.Data()));
    	histoFWHMMeson                    = (TH1D*)fileUncorrected.Get(Form("histoFWHMMeson%s",InvMassTypeEnding.Data()));
    	histoFWHMMesonLeft                = (TH1D*)fileUncorrected.Get(Form("histoFWHMMesonLeft%s",InvMassTypeEnding.Data()));
	
    	histoMassMeson                    = (TH1D*)fileUncorrected.Get(Form("histoMassMeson%s",InvMassTypeEnding.Data()));
    	histoMassMesonLeft                = (TH1D*)fileUncorrected.Get(Form("histoMassMesonLeft%s",InvMassTypeEnding.Data()));
	} else{ // no background calculation
		histoUnCorrectedYieldWide          = (TH1D*)fileUncorrected.Get(Form("histoYieldWideMesonBackFit%s",InvMassTypeEnding.Data()));
    	histoUnCorrectedYieldNarrow        = (TH1D*)fileUncorrected.Get(Form("histoYieldNarrowMesonBackFit%s",InvMassTypeEnding.Data()));
		histoUnCorrectedYieldWideBackFit   = (TH1D*)fileUncorrected.Get(Form("histoYieldWideMesonBackFit%s",InvMassTypeEnding.Data()));
    	histoUnCorrectedYieldNarrowBackFit = (TH1D*)fileUncorrected.Get(Form("histoYieldNarrowMesonBackFit%s",InvMassTypeEnding.Data()));
    	histoUnCorrectedYieldLeft          = (TH1D*)fileUncorrected.Get(Form("histoYieldMesonLeft%s",InvMassTypeEnding.Data()));
    	histoUnCorrectedYieldLeftWide      = (TH1D*)fileUncorrected.Get(Form("histoYieldMesonLeftWide%s",InvMassTypeEnding.Data()));
    	histoUnCorrectedYieldLeftNarrow    = (TH1D*)fileUncorrected.Get(Form("histoYieldMesonLeftNarrow%s",InvMassTypeEnding.Data()));
    	histoFWHMMeson                     = (TH1D*)fileUncorrected.Get(Form("histoFWHMMeson%s",InvMassTypeEnding.Data()));
    	histoFWHMMesonLeft                 = (TH1D*)fileUncorrected.Get(Form("histoFWHMMesonLeft%s",InvMassTypeEnding.Data()));
	
    	histoMassMeson                    = (TH1D*)fileUncorrected.Get(Form("histoMassMeson%s",InvMassTypeEnding.Data()));
    	histoMassMesonLeft                = (TH1D*)fileUncorrected.Get(Form("histoMassMesonLeft%s",InvMassTypeEnding.Data()));
	}
    TH1D *histoMesonSignalFullPtInvMass     = (TH1D*)fileUncorrected.Get(Form("Mapping_GG_InvMass%s_FullPt",InvMassTypeEnding.Data()));
    TH1D *histoMesonBckNormFullPtInvMass    = (TH1D*)fileUncorrected.Get(Form("Mapping_BackNorm_InvMass%s_FullPt",InvMassTypeEnding.Data()));

    // Get background contribution true histos
    TH1D *histoYieldsMappingTruePiPlPiMiSameMother[7]                   = {NULL,NULL,NULL,NULL,NULL,NULL,NULL};
    TH1D *histoYieldsMappingTruePiMiPiZeroSameMother[5]                 = {NULL,NULL,NULL,NULL,NULL};
    TH1D *histoYieldsMappingTruePiPlPiZeroSameMother[5]                 = {NULL,NULL,NULL,NULL,NULL};

    TH1D *histoYieldsMappingTruePiPlPiMiSameMotherFraction[7]           = {NULL,NULL,NULL,NULL,NULL,NULL,NULL};
    TH1D *histoYieldsMappingTruePiMiPiZeroSameMotherFraction[5]         = {NULL,NULL,NULL,NULL,NULL};
    TH1D *histoYieldsMappingTruePiPlPiZeroSameMotherFraction[5]         = {NULL,NULL,NULL,NULL,NULL};

    TH1D *histoYieldsMappingGGInvMass                                   = NULL;
    TH1D *histoYieldsMappingGGInvMassFraction                           = NULL;
    TH1D *histoYieldsMappingTrueMeson                                   = NULL;
    TH1D *histoYieldsMappingTrueMesonFraction                           = NULL;
    TH1D *histoYieldsMappingTruePiPlPiMiPiZeroCombinatorical            = NULL;
    TH1D *histoYieldsMappingTruePiPlPiMiPiZeroCombinatoricalFraction    = NULL;
    TH1D *histoYieldsMappingTruePiPlPiMiPiZeroContamination             = NULL;
    TH1D *histoYieldsMappingTruePiPlPiMiPiZeroContaminationFraction     = NULL;

    // Read True histos
    if(isMC){
        histoYieldsMappingGGInvMass                                   = (TH1D*)fileUncorrected.Get("histoYieldsMappingGGInvMass");
        if (histoYieldsMappingGGInvMass){
            histoYieldsMappingGGInvMassFraction                           = (TH1D*)histoYieldsMappingGGInvMass->Clone("histoYieldsMappingGGInvMassFraction");
            histoYieldsMappingGGInvMassFraction->Sumw2();
            histoYieldsMappingGGInvMassFraction->Divide(histoYieldsMappingGGInvMass);
        }
        histoYieldsMappingTrueMeson                                   = (TH1D*)fileUncorrected.Get("histoYieldsMappingTrueMeson");
        if(histoYieldsMappingTrueMeson){
            histoYieldsMappingTrueMesonFraction                           = (TH1D*)histoYieldsMappingTrueMeson ->Clone("histoYieldsMappingTrueMesonFraction");
            histoYieldsMappingTrueMesonFraction->Sumw2();
            histoYieldsMappingTrueMesonFraction->Divide(histoYieldsMappingGGInvMass);
        }
        histoYieldsMappingTruePiPlPiMiPiZeroCombinatorical            = (TH1D*)fileUncorrected.Get("histoYieldsMappingTruePiPlPiMiPiZeroCombinatorical");
        if(histoYieldsMappingTruePiPlPiMiPiZeroCombinatorical){
            histoYieldsMappingTruePiPlPiMiPiZeroCombinatoricalFraction    = (TH1D*)histoYieldsMappingTruePiPlPiMiPiZeroCombinatorical->Clone("histoYieldsMappingTruePiPlPiMiPiZeroCombinatoricalFraction");
            histoYieldsMappingTruePiPlPiMiPiZeroCombinatoricalFraction->Sumw2();
            histoYieldsMappingTruePiPlPiMiPiZeroCombinatoricalFraction->Divide(histoYieldsMappingGGInvMass);
        }

        histoYieldsMappingTruePiPlPiMiPiZeroContamination             = (TH1D*)fileUncorrected.Get("histoYieldsMappingTruePiPlPiMiPiZeroContamination");
        if(histoYieldsMappingTruePiPlPiMiPiZeroContamination){
            histoYieldsMappingTruePiPlPiMiPiZeroContaminationFraction     = (TH1D*)histoYieldsMappingTruePiPlPiMiPiZeroContamination->Clone("histoYieldsMappingTruePiPlPiMiPiZeroContaminationFraction");
            histoYieldsMappingTruePiPlPiMiPiZeroContaminationFraction->Sumw2();
            histoYieldsMappingTruePiPlPiMiPiZeroContaminationFraction->Divide(histoYieldsMappingGGInvMass);
        }

      TString fNamesYieldsMappingTruePiPlPiMiSameMother[7]   = {"histoYieldsMappingTruePiPlPiMiSameMother","histoYieldsMappingTruePiPlPiMiSameMotherFromEta","histoYieldsMappingTruePiPlPiMiSameMotherFromOmega","histoYieldsMappingTruePiPlPiMiSameMotherFromRho",
                                                                "histoYieldsMappingTruePiPlPiMiSameMotherFromEtaPrime","histoYieldsMappingTruePiPlPiMiSameMotherFromK0s","histoYieldsMappingTruePiPlPiMiSameMotherFromK0l"};
      TString fNamesYieldsMappingTruePiPlPiZeroSameMother[5] = {"histoYieldsMappingTruePiPlPiZeroSameMother","histoYieldsMappingTruePiPlPiZeroSameMotherFromEta","histoYieldsMappingTruePiPlPiZeroSameMotherFromOmega","histoYieldsMappingTruePiPlPiZeroSameMotherFromRho",
                                                                "histoYieldsMappingTruePiPlPiZeroSameMotherFromK0l"};
      TString fNamesYieldsMappingTruePiMiPiZeroSameMother[5] = {"histoYieldsMappingTruePiMiPiZeroSameMother","histoYieldsMappingTruePiMiPiZeroSameMotherFromEta","histoYieldsMappingTruePiMiPiZeroSameMotherFromOmega","histoYieldsMappingTruePiMiPiZeroSameMotherFromRho",
                                                                "histoYieldsMappingTruePiMiPiZeroSameMotherFromK0l"};

      for(Int_t k = 0; k < 7; k++){
         histoYieldsMappingTruePiPlPiMiSameMother[k]              = (TH1D*)fileUncorrected.Get(fNamesYieldsMappingTruePiPlPiMiSameMother[k].Data());
         if(histoYieldsMappingTruePiPlPiMiSameMother[k]){
           histoYieldsMappingTruePiPlPiMiSameMotherFraction[k]      = (TH1D*)histoYieldsMappingTruePiPlPiMiSameMother[k]->Clone(Form("%sFraction",fNamesYieldsMappingTruePiPlPiMiSameMother[k].Data()));
           histoYieldsMappingTruePiPlPiMiSameMotherFraction[k]->Sumw2();
           histoYieldsMappingTruePiPlPiMiSameMotherFraction[k]->Divide(histoYieldsMappingGGInvMass);
         }
         if(k<5){
           histoYieldsMappingTruePiMiPiZeroSameMother[k]          = (TH1D*)fileUncorrected.Get(fNamesYieldsMappingTruePiMiPiZeroSameMother[k].Data());
           if(histoYieldsMappingTruePiMiPiZeroSameMother[k]){
             histoYieldsMappingTruePiMiPiZeroSameMotherFraction[k]  = (TH1D*)histoYieldsMappingTruePiMiPiZeroSameMother[k]->Clone(Form("%sFraction",fNamesYieldsMappingTruePiMiPiZeroSameMother[k].Data()));
             histoYieldsMappingTruePiMiPiZeroSameMotherFraction[k]->Sumw2();
             histoYieldsMappingTruePiMiPiZeroSameMotherFraction[k]->Divide(histoYieldsMappingGGInvMass);
           }

           histoYieldsMappingTruePiPlPiZeroSameMother[k]          = (TH1D*)fileUncorrected.Get(fNamesYieldsMappingTruePiPlPiZeroSameMother[k].Data());
           if(histoYieldsMappingTruePiPlPiZeroSameMother[k]){
             histoYieldsMappingTruePiPlPiZeroSameMotherFraction[k]  = (TH1D*)histoYieldsMappingTruePiPlPiZeroSameMother[k]->Clone(Form("%sFraction",fNamesYieldsMappingTruePiPlPiZeroSameMother[k].Data()));
             histoYieldsMappingTruePiPlPiZeroSameMotherFraction[k]->Sumw2();
             histoYieldsMappingTruePiPlPiZeroSameMotherFraction[k]->Divide(histoYieldsMappingGGInvMass);
           }
         }
      }
    }

    Float_t nEvt = 0;
	if (optionEnergy.Contains("PbPb") || optionEnergy.Contains("pPb")){
		nEvt = histoEventQuality->GetBinContent(1);
	} else {
		nEvt = GetNEvents(histoEventQuality);
	}
   
	TH1D *deltaPt =               (TH1D*)fileUncorrected.Get("deltaPt");
	
	for (Int_t i = 0; i < deltaPt->GetNbinsX() +1; i++){
		deltaPt->SetBinError(i, 0);
	}  
	Double_t maxPtMeson= histoUnCorrectedYield->GetXaxis()->GetBinUpEdge(histoUnCorrectedYield->GetNbinsX());
	
	
    TFile* fileCorrections =                 new TFile(fileNameCorrectionFile.Data());
	if (fileCorrections->IsZombie()) return;
    TH1F *histoEventQualityMC =             (TH1F*)fileCorrections->Get("NEvents");

	TFile* fileAltCorrections = NULL;
	if(useDiffAcceptance){
		fileAltCorrections =                 new TFile(fileNameAlternativeCorrectionFile.Data());
		if (fileAltCorrections->IsZombie()) return;
	}

    TH1D *histoEffiPt              = NULL;
    TH1D *histoEffiNarrowPt        = NULL;
    TH1D *histoEffiWidePt          = NULL;
	TH1D *histoEffiPtBackFit 	   = NULL;
	TH1D *histoEffiPtBackFitFromFit= NULL;
    TH1D *histoEffiNarrowPtBackFit = NULL;
    TH1D *histoEffiWidePtBackFit   = NULL;
    TH1D *histoEffiLeftPt          = NULL;
    TH1D *histoEffiLeftNarrowPt    = NULL;
    TH1D *histoEffiLeftWidePt      = NULL;
    TH1D *histoAcceptance          = NULL;

    histoEffiPtBackFitFromFit =              (TH1D*)fileCorrections->Get(Form("MesonEffiBackFitFromFitPt%s",InvMassTypeEnding.Data())); //not yet correct MesonEffiPt
	if(backMode.CompareTo("4")){ 
		 histoEffiPt =                     (TH1D*)fileCorrections->Get(Form("MesonEffiPt%s",InvMassTypeEnding.Data())); //not yet correct MesonEffiPt
         histoEffiNarrowPt =               (TH1D*)fileCorrections->Get(Form("MesonNarrowEffiPt%s",InvMassTypeEnding.Data()));
         histoEffiWidePt =                 (TH1D*)fileCorrections->Get(Form("MesonWideEffiPt%s",InvMassTypeEnding.Data()));
		 histoEffiPtBackFit =              (TH1D*)fileCorrections->Get(Form("MesonEffiBackFitPt%s",InvMassTypeEnding.Data())); //not yet correct MesonEffiPt
         histoEffiNarrowPtBackFit =        (TH1D*)fileCorrections->Get(Form("MesonEffiBackFitNarrowPt%s",InvMassTypeEnding.Data()));
         histoEffiWidePtBackFit =          (TH1D*)fileCorrections->Get(Form("MesonEffiBackFitWidePt%s",InvMassTypeEnding.Data()));
         histoEffiLeftPt =                 (TH1D*)fileCorrections->Get(Form("MesonLeftEffiPt%s",InvMassTypeEnding.Data()));
         histoEffiLeftNarrowPt =           (TH1D*)fileCorrections->Get(Form("MesonLeftNarrowEffiPt%s",InvMassTypeEnding.Data()));
         histoEffiLeftWidePt =             (TH1D*)fileCorrections->Get(Form("MesonLeftWideEffiPt%s",InvMassTypeEnding.Data()));

		 histoAcceptance=                  (TH1D*)fileCorrections->Get("fMCMesonAccepPt");
         if(useDiffAcceptance) {
			 TH1D* histoAcceptanceAlt=                  (TH1D*)fileAltCorrections->Get("fMCMesonAccepPt");

			// fill acceptance histo with correct values (to keep binning correct)
			for (Int_t i = 0; i < histoAcceptance->GetNbinsX()+1; i++)
			{
				Double_t center = histoAcceptance->GetBinCenter(i);
				Double_t accVal = histoAcceptanceAlt->GetBinContent(histoAcceptanceAlt->FindBin(center));
				Double_t accErr = histoAcceptanceAlt->GetBinError(histoAcceptanceAlt->FindBin(center));

				histoAcceptance->SetBinContent(i,accVal);
				histoAcceptance->SetBinError(i,accErr);
			}
			
		 }
	} else{ // no background calculation used
		 histoEffiPt =                     (TH1D*)fileCorrections->Get(Form("MesonEffiBackFitPt%s",InvMassTypeEnding.Data())); //not yet correct MesonEffiPt
         histoEffiNarrowPt =               (TH1D*)fileCorrections->Get(Form("MesonEffiBackFitNarrowPt%s",InvMassTypeEnding.Data()));
         histoEffiWidePt =                 (TH1D*)fileCorrections->Get(Form("MesonEffiBackFitWidePt%s",InvMassTypeEnding.Data()));

		 histoEffiPtBackFit =              (TH1D*)fileCorrections->Get(Form("MesonEffiBackFitPt%s",InvMassTypeEnding.Data())); //not yet correct MesonEffiPt
         histoEffiNarrowPtBackFit =        (TH1D*)fileCorrections->Get(Form("MesonEffiBackFitNarrowPt%s",InvMassTypeEnding.Data()));
         histoEffiWidePtBackFit =          (TH1D*)fileCorrections->Get(Form("MesonEffiBackFitWidePt%s",InvMassTypeEnding.Data()));

		 // not avilable for no backrgoudn calculation
         histoEffiLeftPt =                 (TH1D*)fileCorrections->Get(Form("MesonLeftEffiPt%s",InvMassTypeEnding.Data()));
         histoEffiLeftNarrowPt =           (TH1D*)fileCorrections->Get(Form("MesonLeftNarrowEffiPt%s",InvMassTypeEnding.Data()));
         histoEffiLeftWidePt =             (TH1D*)fileCorrections->Get(Form("MesonLeftWideEffiPt%s",InvMassTypeEnding.Data()));
		 histoAcceptance=                  (TH1D*)fileCorrections->Get("fMCMesonAccepPt");
         
		 if(useDiffAcceptance) {
			 TH1D* histoAcceptanceAlt=                  (TH1D*)fileAltCorrections->Get("fMCMesonAccepPt");

			// fill acceptance histo with correct values (to keep binning correct)
			for (Int_t i = 0; i < histoAcceptance->GetNbinsX()+1; i++)
			{
				Double_t center = histoAcceptance->GetBinCenter(i);
				Double_t accVal = histoAcceptanceAlt->GetBinContent(histoAcceptanceAlt->FindBin(center));
				Double_t accErr = histoAcceptanceAlt->GetBinError(histoAcceptanceAlt->FindBin(center));

				histoAcceptance->SetBinContent(i,accVal);
				histoAcceptance->SetBinError(i,accErr);
			}
			
		 }
	}
	
    TH1D *histoTrueEffiPt =                 NULL;
    TH1D *histoTrueEffiNarrowPt =           NULL;
    TH1D *histoTrueEffiWidePt =             NULL;
    histoTrueEffiPt =             (TH1D*)fileCorrections->Get(Form("TrueMesonEffiPt%s",InvMassTypeEnding.Data()));
    histoTrueEffiNarrowPt =       (TH1D*)fileCorrections->Get(Form("TrueMesonNarrowEffiPt%s",InvMassTypeEnding.Data()));
    histoTrueEffiWidePt =         (TH1D*)fileCorrections->Get(Form("TrueMesonWideEffiPt%s",InvMassTypeEnding.Data()));

	TH1D* histoInputMesonPt =           (TH1D*)fileCorrections->Get("MC_Meson_genPt");
    TH1D* histoInputMesonOldBinPt =     (TH1D*)fileCorrections->Get("MC_Meson_genPt_oldBin");
	TH1D* histoInputMesonOldBinPtWOWeights = NULL;
	TH1D* histoInputMesonOldBinPtWeights = NULL;
	histoInputMesonOldBinPtWOWeights =     (TH1D*)fileCorrections->Get("MC_Meson_genPt_WOWeights");
	histoInputMesonOldBinPtWeights =     (TH1D*)fileCorrections->Get("MC_Meson_genPt_Weights");
	TH1D* histoMCInputAddedSig = NULL;
	TH1D* histoMCInputWOWeightingAddedSig = NULL;
	TH1D* histoMCInputWeightsAddedSig = NULL;
	histoMCInputAddedSig =     (TH1D*)fileCorrections->Get("MC_Meson_genPt_oldBin_AddedSig");				//NOT THERE
	histoMCInputWOWeightingAddedSig =     (TH1D*)fileCorrections->Get("MC_Meson_genPt_WOWeights_AddedSig");	//NOT THERE
	histoMCInputWeightsAddedSig =     (TH1D*)fileCorrections->Get("MC_Meson_genPt_Weights_AddedSig");		//NOT THERE
	
	
    TH1D* histoTrueMassMeson =          (TH1D*)fileCorrections->Get(Form("histoTrueMassMeson%s",InvMassTypeEnding.Data()));
    TH1D* histoTrueFWHMMeson =          (TH1D*)fileCorrections->Get(Form("histoTrueFWHMMeson%s",InvMassTypeEnding.Data()));

	TString centralityCutNumber = fEventCutSelection(0,3);
	TString centralityString = GetCentralityString(fEventCutSelection);

	TH1D *histoTrueEffiPtFixed = (TH1D*)histoTrueEffiPt->Clone("histoTrueEffiPtFixed");
	TH1D *histoTrueEffiNarrowPtFixed = (TH1D*)histoTrueEffiNarrowPt->Clone("TrueMesonNarrowEffiPtFixed");
	TH1D *histoTrueEffiWidePtFixed = (TH1D*)histoTrueEffiWidePt->Clone("TrueMesonWideEffiPt");
	TH1D *histoEffiPtFixed = (TH1D*)histoEffiPt->Clone("histoEffiPtFixed");
	TH1D *histoEffiNarrowPtFixed = (TH1D*)histoEffiNarrowPt->Clone("MesonNarrowEffiPtFixed");
	TH1D *histoEffiWidePtFixed = (TH1D*)histoEffiWidePt->Clone("MesonWideEffiPt");

	TH1D *histoEffiPtFixedBackFit = (TH1D*)histoEffiPtBackFit->Clone("histoEffiPtFixed");
	TH1D *histoEffiNarrowPtFixedBackFit = (TH1D*)histoEffiNarrowPtBackFit->Clone("MesonNarrowEffiPtFixed");
	TH1D *histoEffiWidePtFixedBackFit = (TH1D*)histoEffiWidePtBackFit->Clone("MesonWideEffiPt");

	histoTrueEffiPtFixed = FixEfficiency(histoTrueEffiPtFixed,histoTrueEffiPt,optionEnergy,centralityString);
	histoTrueEffiNarrowPtFixed = FixEfficiency(histoTrueEffiNarrowPtFixed,histoTrueEffiNarrowPt,optionEnergy,centralityString);
	histoTrueEffiWidePtFixed = FixEfficiency(histoTrueEffiWidePtFixed,histoTrueEffiWidePt,optionEnergy,centralityString);
	histoEffiPtFixed = FixEfficiency(histoEffiPtFixed,histoEffiPt,optionEnergy,centralityString);
	histoEffiNarrowPtFixed = FixEfficiency(histoEffiNarrowPtFixed,histoEffiNarrowPt,optionEnergy,centralityString);
	histoEffiWidePtFixed = FixEfficiency(histoEffiWidePtFixed,histoEffiWidePt,optionEnergy,centralityString);

    // Calculate weighted average of effies
	TH1D* histoEffiWeightedAverage        = NULL;
	TH1D* histoEffiWeightedBackFitFromFit = NULL;
	TH1D* histoTrueEffiWeighted           = NULL;

    histoEffiWeightedAverage = (TH1D*) histoEffiPtBackFit->Clone("histoEffiWeightedAverage");
    histoEffiWeightedAverage->Sumw2();
    histoEffiWeightedAverage->SetBit(TH1::kIsAverage);

	histoEffiWeightedBackFitFromFit = (TH1D*) histoEffiPtBackFitFromFit->Clone("histoEffiWeightedBackFitFromFit");
    histoEffiWeightedBackFitFromFit->Sumw2();
    histoEffiWeightedBackFitFromFit->SetBit(TH1::kIsAverage);

	histoTrueEffiWeighted = (TH1D*) histoTrueEffiPt->Clone("histoTrueEffiWeighted");
    histoTrueEffiWeighted->Sumw2();
    histoTrueEffiWeighted->SetBit(TH1::kIsAverage);

	histoEffiWeightedAverage->Add(histoEffiWeightedBackFitFromFit);
	histoEffiWeightedAverage->Add(histoTrueEffiWeighted);


	Float_t nEvtMC = 0;
	if (optionEnergy.CompareTo("PbPb_2.76TeV") == 0){
		nEvtMC = histoEventQualityMC->GetBinContent(1);
	} else {
		nEvtMC = GetNEvents(histoEventQualityMC);
	}
			
	TH1D *histoYieldTrueGGFracMeson = NULL;
	TH1D *histoYieldTrueGGFracMesonWide = NULL;
	TH1D *histoYieldTrueGGFracMesonNarrow = NULL;
	TH1D* histoYieldGGMesonLeft = NULL;
	TH1D* histoYieldGGMeson = NULL;
	TH1D* histoYieldGGMesonLeftNarrow = NULL;
	TH1D* histoYieldGGMesonNarrow = NULL;
	TH1D* histoYieldGGMesonLeftWide = NULL;
	TH1D* histoYieldGGMesonWide = NULL;

    if(optDalitz){
        histoYieldTrueGGFracMeson =               (TH1D*)fileCorrections->Get("TrueGGFrac");
        histoYieldTrueGGFracMesonWide =        (TH1D*)fileCorrections->Get("TrueGGFracWide");
        histoYieldTrueGGFracMesonNarrow =         (TH1D*)fileCorrections->Get("TrueGGFracNarrow");
        histoYieldGGMeson = (TH1D*)histoUnCorrectedYield->Clone("GGFracMeson");
        histoYieldGGMeson->Sumw2();
        histoYieldGGMeson->Multiply(histoYieldTrueGGFracMeson);
        histoYieldGGMesonLeft = (TH1D*)histoUnCorrectedYieldLeft->Clone("GGFracMesonLeft");
        histoYieldGGMesonLeft->Sumw2();
        histoYieldGGMesonLeft->Multiply(histoYieldTrueGGFracMeson);
        histoYieldGGMesonNarrow = (TH1D*)histoUnCorrectedYieldNarrow->Clone("GGFracMesonNarrow");
        histoYieldGGMesonNarrow->Sumw2();
        histoYieldGGMesonNarrow->Multiply(histoYieldTrueGGFracMesonNarrow);
        histoYieldGGMesonLeftNarrow = (TH1D*)histoUnCorrectedYieldLeftNarrow->Clone("GGFracMesonLeftNarrow");
        histoYieldGGMesonLeftNarrow->Sumw2();
        histoYieldGGMesonLeftNarrow->Multiply(histoYieldTrueGGFracMesonNarrow);
        histoYieldGGMesonWide = (TH1D*)histoUnCorrectedYieldWide->Clone("GGFracMesonWide");
        histoYieldGGMesonWide->Sumw2();
        histoYieldGGMesonWide->Multiply(histoYieldTrueGGFracMesonWide);
        histoYieldGGMesonLeftWide = (TH1D*)histoUnCorrectedYieldLeftWide->Clone("GGFracMesonLeftWide");
        histoYieldGGMesonLeftWide->Sumw2();
        histoYieldGGMesonLeftWide->Multiply(histoYieldTrueGGFracMesonWide);
    }
	Double_t mesonMassExpect = 0;

    if(nameMeson.CompareTo("Eta") == 0 ) mesonMassExpect = TDatabasePDG::Instance()->GetParticle(221)->Mass();
	if(nameMeson.CompareTo("Omega") == 0 ) mesonMassExpect = TDatabasePDG::Instance()->GetParticle(223)->Mass();
	
	if(nameMeson.CompareTo("Omega") != 0 && nameMeson.CompareTo("Eta") != 0)
	{
		cout<<"ERROR!!! This macro is written only for Omega and Eta meson signal correction!"<<endl;
		return;
	}
	
	TString fileNameDCAData = Form("%s/%s/%s_Data_GammaConvV1DCATestAnalysed%s.root",fCutSelection.Data(),optionEnergy.Data(),nameMeson.Data(),optionPeriod.Data());
	
	TFile* fileDCAAnalysisData =         new TFile(fileNameDCAData.Data());
	Bool_t kDCAFileDataExists = kTRUE;
	if (fileDCAAnalysisData->IsZombie()) kDCAFileDataExists = kFALSE;
	cout << kDCAFileDataExists << endl;
	TH1D *histoFracCatvsPt[6];
	TH1D *histoFracIntHistBGvsPt[6];
	TH1D *histoCorrectionFactorsHistCat[6];
	for (Int_t i = 0; i < 6 ; i++){
		histoFracCatvsPt[i] = NULL;
		histoFracIntHistBGvsPt[i] = NULL;
		histoCorrectionFactorsHistCat[i] = NULL;
	}   
	TH1D* histoCorrectionFactorsHistvsPt = NULL;
	TH1D* histoCorrectionFactorsFitvsPt = NULL;
	TH1D* histoCorrectionFactorsHistvsPtCatA = NULL;
	TH1D* histoCorrectionFactorsHistvsPtCatC = NULL;
	TH1D* histoCorrectionFactorsHistvsPtCatD = NULL;
	TH1D* histoDCAZUnderMesonAllCat_AllPt = NULL;
	if (kDCAFileDataExists){
        histoCorrectionFactorsHistvsPt =                 (TH1D*)fileDCAAnalysisData->Get("fHistCorrectionFactorsHistAllCat_vsPt");
        histoCorrectionFactorsFitvsPt =                  (TH1D*)fileDCAAnalysisData->Get("fHistCorrectionFactorsFitAllCat_vsPt");
		histoCorrectionFactorsHistvsPtCatA =             (TH1D*)fileDCAAnalysisData->Get("fHistCorrectionFactorsHistvsPt_0");
		histoCorrectionFactorsHistvsPtCatC =             (TH1D*)fileDCAAnalysisData->Get("fHistCorrectionFactorsHistvsPt_1");
		histoCorrectionFactorsHistvsPtCatD =             (TH1D*)fileDCAAnalysisData->Get("fHistCorrectionFactorsHistvsPt_2");
		histoDCAZUnderMesonAllCat_AllPt =  (TH1D*)fileDCAAnalysisData->Get("HistDCAZUnderMesonAllCat_AllPt");
		for (Int_t i = 0; i < 6 ; i++){
			histoFracCatvsPt[i] = (TH1D*)fileDCAAnalysisData->Get(Form("fHistFracCat_%i_vsPt",i+1));
			histoFracIntHistBGvsPt[i] = (TH1D*)fileDCAAnalysisData->Get(Form("fHistFracIntHistBGvsPt_Cat_%i_Variant_1",i+1));
			histoCorrectionFactorsHistCat[i] = (TH1D*)fileDCAAnalysisData->Get(Form("fHistCorrectionFactorsHistCat_%i_Variant_1_vsPt",i+1));
		}
		
	}
	
	
	TString fileNameDCAMonteCarlo = Form("%s/%s/%s_MC_GammaConvV1DCATestAnalysed%s.root",fCutSelection.Data(),optionEnergy.Data(),nameMeson.Data(),optionPeriod.Data());

	TFile* fileDCAAnalysisMonteCarlo =         new TFile(fileNameDCAMonteCarlo.Data());
	Bool_t kDCAFileMCExists = kTRUE;
	if (fileDCAAnalysisMonteCarlo->IsZombie()) kDCAFileMCExists = kFALSE;
	cout << kDCAFileMCExists << endl;
	
	TH1D *histoMCFracCatvsPt[6];
	TH1D *histoMCFracIntHistBGvsPt[6];
	TH1D *histoMCCorrectionFactorsHistCat[6];
	for (Int_t i = 0; i < 6 ; i++){
		histoMCFracCatvsPt[i] = NULL;
		histoMCFracIntHistBGvsPt[i] = NULL;
		histoMCCorrectionFactorsHistCat[i] = NULL;
	}

    TH1D* histoMCDCAZUnderMesonAllCat_AllPt = NULL;
	TH1D* histoMCDCAZGarbageAllCat_AllPt= NULL;
	TH1D* histoMCDCAZTrueBackgroundAllCat_AllPt= NULL;
	TH1D* histoMCDCAZTrueSecondaryMesonFromSomethingAllCat_AllPt= NULL;
	TH1D* histoMCDCAZTrueSecondaryMesonFromEtaAllCat_AllPt= NULL;
	TH1D* histoMCDCAZTrueSecondaryMesonFromK0sAllCat_AllPt= NULL;
	TH1D* histoMCDCAZTruePrimaryMesonDalitzAllCat_AllPt= NULL;
	TH1D* histoMCDCAZTruePrimaryMesonGammaGammaAllCat_AllPt= NULL;

	if (kDCAFileMCExists){

        histoMCDCAZUnderMesonAllCat_AllPt =  (TH1D*)fileDCAAnalysisMonteCarlo->Get("HistDCAZUnderMesonAllCat_AllPt");
		histoMCDCAZGarbageAllCat_AllPt =  (TH1D*)fileDCAAnalysisMonteCarlo->Get("HistDCAZGarbageAllCat_AllPt");
		histoMCDCAZTrueBackgroundAllCat_AllPt =  (TH1D*)fileDCAAnalysisMonteCarlo->Get("HistDCAZTrueBackgroundAllCat_AllPt");
		histoMCDCAZTrueSecondaryMesonFromSomethingAllCat_AllPt =  (TH1D*)fileDCAAnalysisMonteCarlo->Get("HistDCAZTrueSecondaryMesonFromSomethingAllCat_AllPt");
		histoMCDCAZTrueSecondaryMesonFromEtaAllCat_AllPt =  (TH1D*)fileDCAAnalysisMonteCarlo->Get("HistDCAZTrueSecondaryMesonFromEtaAllCat_AllPt");
		histoMCDCAZTrueSecondaryMesonFromK0sAllCat_AllPt =  (TH1D*)fileDCAAnalysisMonteCarlo->Get("HistDCAZTrueSecondaryMesonFromK0sAllCat_AllPt");
		histoMCDCAZTruePrimaryMesonDalitzAllCat_AllPt =  (TH1D*)fileDCAAnalysisMonteCarlo->Get("HistDCAZTruePrimaryMesonDalitzAllCat_AllPt");
		histoMCDCAZTruePrimaryMesonGammaGammaAllCat_AllPt =  (TH1D*)fileDCAAnalysisMonteCarlo->Get("HistDCAZTruePrimaryMesonGammaGammaAllCat_AllPt");
		for (Int_t i = 0; i < 6 ; i++){
			histoMCFracCatvsPt[i] = (TH1D*)fileDCAAnalysisMonteCarlo->Get(Form("fHistFracCat_%i_vsPt",i+1));
			histoMCFracIntHistBGvsPt[i] = (TH1D*)fileDCAAnalysisMonteCarlo->Get(Form("fHistFracIntHistBGvsPt_Cat_%i_Variant_1",i+1));
			histoMCCorrectionFactorsHistCat[i] = (TH1D*)fileDCAAnalysisMonteCarlo->Get(Form("fHistCorrectionFactorsHistCat_%i_Variant_1_vsPt",i+1));
		}
	}
	
	
	

	Color_t  colorCatAll[3]    = {kBlack,kRed+2,kBlue+1};
	Color_t  colorCatAllMC[3]    = {kGray+2,kRed+3,kBlue+3};
    Color_t  colorCat[7]    = { kRed+1, 807, 800, kGreen+2, kCyan+2, kBlue+1,kGray+2};
	Color_t  colorCatMC[6]    = { kRed+3, 807+2, 800+2, kGreen+4, kCyan+4, kBlue+3};
    Style_t  styleCat[7]    = { 20, 21, 29, 33, 20, 21,20};
	Style_t  styleCatMC[6]    = { 24, 25, 30, 27, 24, 25};

	if (kDCAFileDataExists && kDCAFileMCExists){
		TCanvas* canvasDCAMCComponents = new TCanvas("canvasDCAMCComponents","",200,10,1350,900);  // gives the page size
		DrawGammaCanvasSettings( canvasDCAMCComponents, 0.08, 0.02, 0.02, 0.09);
		canvasDCAMCComponents->SetLogy();
			if (histoDCAZUnderMesonAllCat_AllPt){
				DrawAutoGammaMesonHistos( histoDCAZUnderMesonAllCat_AllPt, 
								"","dca_{z} #gamma (cm)", "d(dca_{z})/N_{evt}", 
								kFALSE, 2.,1, kFALSE,
								kTRUE,1e-8,2*histoMCDCAZUnderMesonAllCat_AllPt->GetMaximum(), 
								kTRUE, -6., 6.);
				DrawGammaSetMarker(histoDCAZUnderMesonAllCat_AllPt, 20, 1., kBlack, kBlack);
				histoDCAZUnderMesonAllCat_AllPt->GetYaxis()->SetTitleOffset(0.8);
				histoDCAZUnderMesonAllCat_AllPt->DrawCopy("p,e1"); 

				}
				if (histoMCDCAZUnderMesonAllCat_AllPt){
				DrawGammaSetMarker(histoMCDCAZUnderMesonAllCat_AllPt, 24, 1., kGray, kGray);
				histoMCDCAZUnderMesonAllCat_AllPt->DrawCopy("same,p,e1"); 
				}   
				if (histoMCDCAZTruePrimaryMesonGammaGammaAllCat_AllPt){
				DrawGammaSetMarker(histoMCDCAZTruePrimaryMesonGammaGammaAllCat_AllPt, 20, 1., kRed+2, kRed+2);
				histoMCDCAZTruePrimaryMesonGammaGammaAllCat_AllPt->DrawCopy("same,p,e1"); 

				} 
				if (histoMCDCAZTruePrimaryMesonDalitzAllCat_AllPt){
				DrawGammaSetMarker(histoMCDCAZTruePrimaryMesonDalitzAllCat_AllPt, 20, 1., kGreen+2, kGreen+2);
				histoMCDCAZTruePrimaryMesonDalitzAllCat_AllPt->DrawCopy("same,p,e1"); 
				}
				if (histoMCDCAZTrueBackgroundAllCat_AllPt){
					DrawGammaSetMarker(histoMCDCAZTrueBackgroundAllCat_AllPt, 20, 1., kPink+2, kPink+2);
				histoMCDCAZTrueBackgroundAllCat_AllPt->DrawCopy("same,p,e1"); 
				}
				if (histoMCDCAZTrueSecondaryMesonFromK0sAllCat_AllPt){
					DrawGammaSetMarker(histoMCDCAZTrueSecondaryMesonFromK0sAllCat_AllPt, 20, 1., 807, 807);
				histoMCDCAZTrueSecondaryMesonFromK0sAllCat_AllPt->DrawCopy("same,p,e1"); 
				}
				if (histoMCDCAZTrueSecondaryMesonFromSomethingAllCat_AllPt){
					DrawGammaSetMarker(histoMCDCAZTrueSecondaryMesonFromSomethingAllCat_AllPt, 20, 1., kViolet+2, kViolet+2);
				histoMCDCAZTrueSecondaryMesonFromSomethingAllCat_AllPt->DrawCopy("same,p,e1"); 
				}
				if (histoMCDCAZGarbageAllCat_AllPt){
				DrawGammaSetMarker(histoMCDCAZGarbageAllCat_AllPt, 20, 1., kCyan+2, kCyan+2);
				histoMCDCAZGarbageAllCat_AllPt->DrawCopy("same,p,e1"); 
				}
				
				TLatex *labelEnergy = new TLatex(0.11,0.9,Form("%s %s",intermediate.Data(),collisionSystem.Data()));
				SetStyleTLatex( labelEnergy, 0.04,4);
				labelEnergy->Draw();

			TLegend* legendDCAMCComponents0 = new TLegend(0.7,0.7,0.85,0.95);
			legendDCAMCComponents0->SetTextSize(0.04);         
			legendDCAMCComponents0->SetFillColor(0);
			legendDCAMCComponents0->SetLineColor(0);
			legendDCAMCComponents0->SetNColumns(1);
			if (histoDCAZUnderMesonAllCat_AllPt)legendDCAMCComponents0->AddEntry(histoDCAZUnderMesonAllCat_AllPt,"Data","p");
			if (histoMCDCAZUnderMesonAllCat_AllPt)legendDCAMCComponents0->AddEntry(histoMCDCAZUnderMesonAllCat_AllPt,"Total MC","p");
			if (histoMCDCAZTruePrimaryMesonGammaGammaAllCat_AllPt)legendDCAMCComponents0->AddEntry(histoMCDCAZTruePrimaryMesonGammaGammaAllCat_AllPt,Form("Prim"),"p");
			if (histoMCDCAZTruePrimaryMesonDalitzAllCat_AllPt)legendDCAMCComponents0->AddEntry(histoMCDCAZTruePrimaryMesonDalitzAllCat_AllPt,Form("Dalitz"),"l");
			if (histoMCDCAZTrueSecondaryMesonFromK0sAllCat_AllPt && histoMCDCAZTrueSecondaryMesonFromK0sAllCat_AllPt->GetEntries() > 0)legendDCAMCComponents0->AddEntry(histoMCDCAZTrueSecondaryMesonFromK0sAllCat_AllPt,"Sec. #pi^{0} from K^{0}_{s}","p");
			if (histoMCDCAZTrueSecondaryMesonFromSomethingAllCat_AllPt && histoMCDCAZTrueSecondaryMesonFromSomethingAllCat_AllPt->GetEntries() > 0)legendDCAMCComponents0->AddEntry(histoMCDCAZTrueSecondaryMesonFromSomethingAllCat_AllPt,"Sec. #pi^{0} from X","p");
			if (histoMCDCAZTrueBackgroundAllCat_AllPt)legendDCAMCComponents0->AddEntry(histoMCDCAZTrueBackgroundAllCat_AllPt,"#pi^{+}#pi^{-}#pi^{0} BG","p");
			if (histoMCDCAZGarbageAllCat_AllPt)legendDCAMCComponents0->AddEntry(histoMCDCAZGarbageAllCat_AllPt,"garbage","p");
			legendDCAMCComponents0->Draw();         
		canvasDCAMCComponents->Update(); 
		canvasDCAMCComponents->SaveAs(Form("%s/%s_MC_DCAzDecomposition.%s",outputDir.Data(),nameMeson.Data(),suffix.Data()));
			
		
		
	}   
	
	if (kDCAFileDataExists){
		TCanvas* canvasCorrFrac = new TCanvas("canvasCorrFrac","",200,10,1350,900);  // gives the page size
		DrawGammaCanvasSettings( canvasCorrFrac, 0.08, 0.02, 0.02, 0.09);

        canvasCorrFrac->cd();
		DrawAutoGammaMesonHistos( histoFracCatvsPt[0], 
								"", "p_{T,#pi^{0}} (GeV/c)", "N_{#pi^{0} per cat}/(N_{#pi^{0}}) (%)", 
								kFALSE, 2.,1e-8, kFALSE,
								kTRUE, 0, 100, 
								kTRUE, 0., 25.);
		DrawGammaSetMarker(histoFracCatvsPt[0], styleCat[0], 1., colorCat[0], colorCat[0]);      
		histoFracCatvsPt[0]->GetYaxis()->SetTitleOffset(0.8);
		histoFracCatvsPt[0]->DrawCopy("p,e1"); 
	
		if(histoMCFracCatvsPt[0]){
			DrawGammaSetMarker(histoMCFracCatvsPt[0], styleCatMC[0], 1., colorCatMC[0], colorCatMC[0]);      
			histoMCFracCatvsPt[0]->GetYaxis()->SetTitleOffset(0.8);
			histoMCFracCatvsPt[0]->DrawCopy("same,p,e1"); 
		}
		TLegend* legendFractionCat = new TLegend(0.65,0.7,0.95,0.95);
		legendFractionCat->SetTextSize(0.04);         
		legendFractionCat->SetFillColor(0);
		legendFractionCat->SetLineColor(0);
		if (kDCAFileMCExists) legendFractionCat->SetNColumns(3);
			else legendFractionCat->SetNColumns(2);
		legendFractionCat->AddEntry((TObject*)0,"Cat 1","");
		legendFractionCat->AddEntry(histoFracCatvsPt[0],"Data","p");
		if(histoMCFracCatvsPt[0])legendFractionCat->AddEntry(histoMCFracCatvsPt[0],"MC","p");
		
		for (Int_t i = 1; i< 6; i++){
			DrawGammaSetMarker(histoFracCatvsPt[i], styleCat[i], 1., colorCat[i], colorCat[i]);
			histoFracCatvsPt[i]->DrawCopy("same,p,e1"); 
			if(histoMCFracCatvsPt[i]){
				DrawGammaSetMarker(histoMCFracCatvsPt[i], styleCatMC[i], 1., colorCatMC[i], colorCatMC[i]);      
				histoMCFracCatvsPt[i]->DrawCopy("same,p,e1"); 
			}
			legendFractionCat->AddEntry((TObject*)0,Form("Cat %i",i+1),"");
			legendFractionCat->AddEntry(histoFracCatvsPt[i],"Data","p");
			if(histoMCFracCatvsPt[i])legendFractionCat->AddEntry(histoMCFracCatvsPt[i],"MC","p");
		}   
		legendFractionCat->Draw();
		canvasCorrFrac->Update(); 
		canvasCorrFrac->SaveAs(Form("%s/%s_FractionPerCatVsPt_ComparedToMC.%s",outputDir.Data(),nameMeson.Data(),suffix.Data()));
		
		
	}   
	
	if (kDCAFileDataExists){
		TCanvas* canvasCorrFrac = new TCanvas("canvasCorrFrac","",200,10,1350,900);  // gives the page size
		DrawGammaCanvasSettings( canvasCorrFrac, 0.08, 0.02, 0.02, 0.09);

		canvasCorrFrac->cd();
		DrawAutoGammaMesonHistos( histoFracIntHistBGvsPt[0], 
								"", "p_{T,#pi^{0}} (GeV/c)", "BG/Total (%)", 
								kFALSE, 2.,1e-8, kFALSE,
								kTRUE, 0, 30, 
								kTRUE, 0., 25.);
		DrawGammaSetMarker(histoFracIntHistBGvsPt[0], styleCat[0], 1., colorCat[0], colorCat[0]);      
		histoFracIntHistBGvsPt[0]->GetYaxis()->SetTitleOffset(0.8);
		histoFracIntHistBGvsPt[0]->DrawCopy("p,e1"); 
	
		if(histoMCFracIntHistBGvsPt[0]){
			DrawGammaSetMarker(histoMCFracIntHistBGvsPt[0], styleCatMC[0], 1., colorCatMC[0], colorCatMC[0]);      
			histoMCFracIntHistBGvsPt[0]->DrawCopy("same,p,e1"); 
		}
		TLegend* legendFractionCat = new TLegend(0.65,0.7,0.95,0.95);
		legendFractionCat->SetTextSize(0.04);         
		legendFractionCat->SetFillColor(0);
		legendFractionCat->SetLineColor(0);
		if (kDCAFileMCExists) legendFractionCat->SetNColumns(3);
			else legendFractionCat->SetNColumns(2);
		legendFractionCat->AddEntry((TObject*)0,"Cat 1","");
		legendFractionCat->AddEntry(histoFracIntHistBGvsPt[0],"Data","p");
		if(histoMCFracIntHistBGvsPt[0])legendFractionCat->AddEntry(histoMCFracIntHistBGvsPt[0],"MC","p");
		
		for (Int_t i = 1; i< 6; i++){
			DrawGammaSetMarker(histoFracIntHistBGvsPt[i], styleCat[i], 1., colorCat[i], colorCat[i]);
			histoFracIntHistBGvsPt[i]->DrawCopy("same,p,e1"); 
			if(histoMCFracIntHistBGvsPt[i]){
				DrawGammaSetMarker(histoMCFracIntHistBGvsPt[i], styleCatMC[i], 1., colorCatMC[i], colorCatMC[i]);      
				histoMCFracIntHistBGvsPt[i]->DrawCopy("same,p,e1"); 
			}
			legendFractionCat->AddEntry((TObject*)0,Form("Cat %i",i+1),"");
			legendFractionCat->AddEntry(histoFracIntHistBGvsPt[i],"Data","p");
			if(histoMCFracIntHistBGvsPt[i])legendFractionCat->AddEntry(histoMCFracIntHistBGvsPt[i],"MC","p");
		}   
		legendFractionCat->Draw();
		canvasCorrFrac->Update(); 
		canvasCorrFrac->SaveAs(Form("%s/%s_FracBGOverIntHist_ComparedToMC.%s",outputDir.Data(),nameMeson.Data(),suffix.Data()));
		
		
	}   
	
    //**********************************************************************************
    //******************** Background contribution plot*********************************
    //**********************************************************************************
    if(isMC && histoYieldsMappingTruePiPlPiMiPiZeroContaminationFraction && histoYieldsMappingTrueMesonFraction && histoYieldsMappingTruePiPlPiMiSameMotherFraction[0]
            && histoYieldsMappingTruePiMiPiZeroSameMotherFraction[0] && histoYieldsMappingTruePiPlPiZeroSameMotherFraction[0] && histoYieldsMappingTruePiPlPiMiPiZeroCombinatoricalFraction){
      TCanvas* canvasContributions = new TCanvas("canvasContributions","",200,10,1350,900);  // gives the page size
      DrawGammaCanvasSettings( canvasContributions, 0.13, 0.02, 0.02, 0.12);

      DrawAutoGammaMesonHistos( histoYieldsMappingTruePiPlPiMiPiZeroContaminationFraction,
                                  "", "p_{T} (GeV/c)","N_{x}/N_{S+B}",
                                  kFALSE, 0., 0.7, kFALSE,
                                  kFALSE, 0., 1.1,
                                  kFALSE, 3., 15.,62,0.05,42,0.04,0.9,0.9);

      DrawGammaSetMarker(histoYieldsMappingTruePiPlPiMiPiZeroContaminationFraction, styleCat[6], 0.8, colorCat[6], colorCat[6]);
      histoYieldsMappingTruePiPlPiMiPiZeroContaminationFraction->DrawCopy("same,e1,p");

      DrawGammaSetMarker(histoYieldsMappingTrueMesonFraction, styleCat[1], 0.8, colorCat[1], colorCat[1]);
      histoYieldsMappingTrueMesonFraction->DrawCopy("same,e1,p");

      DrawGammaSetMarker(histoYieldsMappingTruePiPlPiMiSameMotherFraction[0], styleCat[2], 0.8, colorCat[2], colorCat[2]);
      histoYieldsMappingTruePiPlPiMiSameMotherFraction[0]->DrawCopy("same,e1,p");

      DrawGammaSetMarker(histoYieldsMappingTruePiMiPiZeroSameMotherFraction[0], styleCat[3], 0.8, colorCat[3], colorCat[3]);
      histoYieldsMappingTruePiMiPiZeroSameMotherFraction[0]->DrawCopy("same,e1,p");

      DrawGammaSetMarker(histoYieldsMappingTruePiPlPiZeroSameMotherFraction[0], styleCat[4], 0.8, colorCat[4], colorCat[4]);
      histoYieldsMappingTruePiPlPiZeroSameMotherFraction[0]->DrawCopy("same,e1,p");

      DrawGammaSetMarker(histoYieldsMappingTruePiPlPiMiPiZeroCombinatoricalFraction, styleCat[5], 0.8, colorCat[5], colorCat[5]);
      histoYieldsMappingTruePiPlPiMiPiZeroCombinatoricalFraction->DrawCopy("same,e1,p");

      TLegend* legendContribution = new TLegend(0.15,0.55,0.48,0.80);
      legendContribution->SetHeader("x=");
      legendContribution->SetTextSize(0.04);
      legendContribution->SetFillColor(0);
      legendContribution->SetFillStyle(0);
      legendContribution->SetLineColor(0);
      legendContribution->AddEntry(histoYieldsMappingTrueMesonFraction,"True reconstructed #eta or #omega","lep");
      legendContribution->AddEntry(histoYieldsMappingTruePiPlPiMiSameMotherFraction[0],"#pi^{+} #pi^{-} have same mother","lep");
      legendContribution->AddEntry(histoYieldsMappingTruePiMiPiZeroSameMotherFraction[0],"#pi^{0} #pi^{-} have same mother","lep");
      legendContribution->AddEntry(histoYieldsMappingTruePiPlPiZeroSameMotherFraction[0],"#pi^{0} #pi^{+} have same mother","lep");
      legendContribution->AddEntry(histoYieldsMappingTruePiPlPiMiPiZeroCombinatoricalFraction,"pure combinatorial","lep");
      legendContribution->AddEntry(histoYieldsMappingTruePiPlPiMiPiZeroContaminationFraction,"wrong identifications","lep");

      legendContribution->Draw();

      canvasContributions->Update();

      canvasContributions->SaveAs(Form("%s/%s_%s_BackgroundContributions_%s.%s",outputDir.Data(),nameMeson.Data(),prefix2.Data(),fCutSelection.Data(),suffix.Data()));

      canvasContributions->cd();
    }
	Color_t colorMethod[5] = {kBlack, kCyan+2, kRed+2, kGreen+2, kBlue+2};
	Style_t styleMethod[5] = {20,24,21,29,33};
	TString nameMethod[5] = {"A","B","A","C","D"};

	if (kDCAFileDataExists){
		TCanvas* canvasCorrFrac = new TCanvas("canvasCorrFrac","",200,10,1350,900);  // gives the page size
		DrawGammaCanvasSettings( canvasCorrFrac, 0.08, 0.02, 0.02, 0.09);

		canvasCorrFrac->cd();
		DrawAutoGammaMesonHistos( histoCorrectionFactorsHistvsPt, 
								"", "p_{T,#pi^{0}} (GeV/c)", "Contamination from Pileup (%)", 
								kFALSE, 2.,1e-8, kFALSE,
								kTRUE, 0, 30, 
								kTRUE, 0., 25.);
		DrawGammaSetMarker(histoCorrectionFactorsHistvsPt, styleMethod[0], 1.2, colorMethod[0], colorMethod[0]);      
		histoCorrectionFactorsHistvsPt->GetYaxis()->SetTitleOffset(0.8);
		histoCorrectionFactorsHistvsPt->DrawCopy("p,e1"); 
        TF1* fitCorrectionFactorsHistvsPt = new TF1("fitCorrectionFactorsHistvsPt","[0]/TMath::Power(x,[1])+[2]");
		fitCorrectionFactorsHistvsPt->SetRange(0.4, maxPtMeson);
		TFitResultPtr resultCorrectionFactorsHistvsPt = histoCorrectionFactorsHistvsPt->Fit(fitCorrectionFactorsHistvsPt,"SINRME+","",0.4, maxPtMeson);
		TString bla= WriteParameterToFile(fitCorrectionFactorsHistvsPt);
		cout << bla.Data()<< endl;
		fitCorrectionFactorsHistvsPt->SetLineColor(colorMethod[0]);
		fitCorrectionFactorsHistvsPt->Draw("same");
		
		TF1* fitCorrectionFactorsFitvsPt = NULL;
		TFitResultPtr resultCorrectionFactorsFitvsPt ;
		if (histoCorrectionFactorsFitvsPt) {
			DrawGammaSetMarker(histoCorrectionFactorsFitvsPt, styleMethod[1], 1.2, colorMethod[1], colorMethod[1]);      
			histoCorrectionFactorsFitvsPt->DrawCopy("same,p,e1"); 
            fitCorrectionFactorsFitvsPt = new TF1("fitCorrectionFactorsFitvsPt","[0]/TMath::Power(x,[1])+[2]");
			fitCorrectionFactorsFitvsPt->SetRange(0.4, maxPtMeson);
			resultCorrectionFactorsFitvsPt = histoCorrectionFactorsFitvsPt->Fit(fitCorrectionFactorsFitvsPt,"SINRME+","",0.4, maxPtMeson);
			fitCorrectionFactorsFitvsPt->SetLineColor(colorMethod[1]);
			fitCorrectionFactorsFitvsPt->Draw("same");
		}
		DrawGammaSetMarker(histoCorrectionFactorsHistvsPtCatA, styleMethod[2], 1.2, colorMethod[2], colorMethod[2]);      
		histoCorrectionFactorsHistvsPtCatA->DrawCopy("same,p,e1"); 
        TF1* fitCorrectionFactorsHistvsPtCatA = new TF1("fitCorrectionFactorsHistvsPtCatA","[0]/TMath::Power(x,[1])+[2]");
		fitCorrectionFactorsHistvsPtCatA->SetRange(0.4, maxPtMeson);
		
		TFitResultPtr resultCorrectionFactorsHistvsPtCatA = histoCorrectionFactorsHistvsPtCatA->Fit(fitCorrectionFactorsHistvsPtCatA,"SINRME+","",0.4, maxPtMeson);
		fitCorrectionFactorsHistvsPtCatA->SetLineColor(colorMethod[2]);
		fitCorrectionFactorsHistvsPtCatA->Draw("same");

		DrawGammaSetMarker(histoCorrectionFactorsHistvsPtCatC, styleMethod[3], 1.2, colorMethod[3], colorMethod[3]);      
		histoCorrectionFactorsHistvsPtCatC->DrawCopy("same,p,e1"); 
        TF1* fitCorrectionFactorsHistvsPtCatC = new TF1("fitCorrectionFactorsHistvsPtCatC","[0]/TMath::Power(x,[1])+[2]");
		fitCorrectionFactorsHistvsPtCatC->SetRange(0.4, maxPtMeson);
		TFitResultPtr resultCorrectionFactorsHistvsPtCatC = histoCorrectionFactorsHistvsPtCatC->Fit(fitCorrectionFactorsHistvsPtCatC,"SINRME+","",0.4, maxPtMeson);
		fitCorrectionFactorsHistvsPtCatC->SetLineColor(colorMethod[3]);
		fitCorrectionFactorsHistvsPtCatC->Draw("same");

		DrawGammaSetMarker(histoCorrectionFactorsHistvsPtCatD, styleMethod[4], 1.2, colorMethod[4], colorMethod[4]);      
		histoCorrectionFactorsHistvsPtCatD->DrawCopy("same,p,e1"); 
        TF1* fitCorrectionFactorsHistvsPtCatD = new TF1("fitCorrectionFactorsHistvsPtCatD","[0]/TMath::Power(x,[1])+[2]");
		fitCorrectionFactorsHistvsPtCatD->SetRange(0.4, maxPtMeson);
		TFitResultPtr resultCorrectionFactorsHistvsPtCatD = histoCorrectionFactorsHistvsPtCatD->Fit(fitCorrectionFactorsHistvsPtCatD,"SINRME+","",0.4, maxPtMeson);
		fitCorrectionFactorsHistvsPtCatD->SetLineColor(colorMethod[4]);
		fitCorrectionFactorsHistvsPtCatD->Draw("same");
				
		TLegend* legendFractionCat = new TLegend(0.5,0.8,0.95,0.95);
		legendFractionCat->SetTextSize(0.03);         
		legendFractionCat->SetFillColor(0);
		legendFractionCat->SetLineColor(0);
		legendFractionCat->SetNColumns(2);
		legendFractionCat->AddEntry((TObject*)0,"Method A","");
		legendFractionCat->AddEntry(histoCorrectionFactorsHistvsPt,"Data","p");
		if (histoCorrectionFactorsFitvsPt) legendFractionCat->AddEntry((TObject*)0,"Method B","");
		if (histoCorrectionFactorsFitvsPt) legendFractionCat->AddEntry(histoCorrectionFactorsFitvsPt,"Data","p");
		legendFractionCat->AddEntry((TObject*)0,"Method A sep Cat","");
		legendFractionCat->AddEntry(histoCorrectionFactorsHistvsPtCatA,"Data","p");
		legendFractionCat->AddEntry((TObject*)0,"Method C sep Cat","");
		legendFractionCat->AddEntry(histoCorrectionFactorsHistvsPtCatC,"Data","p");
		legendFractionCat->AddEntry((TObject*)0,"Method D sep Cat","");
		legendFractionCat->AddEntry(histoCorrectionFactorsHistvsPtCatD,"Data","p");
	
		TLatex *labelEnergy = new TLatex(0.11,0.9,Form("%s %s",intermediate.Data(),collisionSystem.Data()));
		SetStyleTLatex( labelEnergy, 0.04,4);
		labelEnergy->Draw();
		
		legendFractionCat->Draw();
		canvasCorrFrac->Update(); 
		canvasCorrFrac->SaveAs(Form("%s/%s_FinalBGEstimate_AllMethods.%s",outputDir.Data(),nameMeson.Data(),suffix.Data()));

		canvasCorrFrac->cd();
		
	}   
	
	//**********************************************************************************
	//******************** Mass Plot *********************************************
	//**********************************************************************************
	TCanvas* canvasMass = new TCanvas("canvasMass","",200,10,1350,900);  // gives the page size
	DrawGammaCanvasSettings( canvasMass, 0.13, 0.02, 0.02, 0.09);
	
	if (nameMeson.CompareTo("Omega") == 0 ){
		histoMassMeson->GetYaxis()->SetRangeUser(0.72,0.84);	//Omega
	} else {
		histoMassMeson->GetYaxis()->SetRangeUser(0.48,0.62);	//Eta
	}               
	histoMassMeson->GetYaxis()->SetNdivisions(510); 
	
	DrawAutoGammaMesonHistos( histoMassMeson, 
								"", "p_{T} (GeV/c)", Form("Mass for %s in |y| < %s (GeV/c^{2})",textMeson.Data(), rapidityRange.Data()), 
								kFALSE, 0., 0.7, kFALSE,
								kFALSE, 0., 0.7, 
								kTRUE, 0., 25.);
	DrawGammaSetMarker(histoMassMeson, 22, 0.8, kBlack, kBlack);                
	histoMassMeson->DrawCopy("same,e1,p"); 
	
	DrawGammaSetMarker(histoTrueMassMeson, 24, 0.8, kRed+2, kRed+2);
	histoTrueMassMeson->DrawCopy("same,e1,p"); 
	
	DrawGammaLines(0., maxPtMeson,mesonMassExpect, mesonMassExpect,0.1);
	
	TLegend* legendMass = new TLegend(0.15,0.12,0.5,0.25);
	legendMass->SetTextSize(0.02);         
	legendMass->SetFillColor(0);
	legendMass->SetFillStyle(0);
	legendMass->SetLineColor(0);
	legendMass->AddEntry(histoMassMeson,"standard");

    if(nameMeson.CompareTo("Eta") == 0 ) legendMass->AddEntry(histoTrueMassMeson,"True reconstructed #eta");
	if(nameMeson.CompareTo("Omega") == 0 ) legendMass->AddEntry(histoTrueMassMeson,"True reconstructed #omega");
	
	legendMass->Draw();
	
	canvasMass->Update();
	
	canvasMass->SaveAs(Form("%s/%s_%s_Mass_%s.%s",outputDir.Data(),nameMeson.Data(),prefix2.Data(),fCutSelection.Data(),suffix.Data()));

	canvasMass->cd();
	
	if (mode==2 || mode == 3){
		if (nameMeson.CompareTo("Omega") == 0){
			histoMassMeson->GetYaxis()->SetRangeUser(0.72,0.84);
		} else {
			histoMassMeson->GetYaxis()->SetRangeUser(0.48,0.62);
		}               

		histoMassMeson->DrawCopy("e1,p"); 
		histoTrueMassMeson->DrawCopy("same,e1,p"); 
			
		TLegend* legendMass2 = new TLegend(0.55,0.12,0.95,0.25);
		legendMass2->SetTextSize(0.02);         
		legendMass2->SetFillColor(0);
		legendMass2->SetFillStyle(0);
		legendMass2->SetLineColor(0);
		legendMass2->AddEntry(histoMassMeson,"standard");
		if(nameMeson.CompareTo("Eta") == 0 ) legendMass2->AddEntry(histoTrueMassMeson,"True reconstructed #eta");
		if(nameMeson.CompareTo("Omega") == 0 ) legendMass->AddEntry(histoTrueMassMeson,"True reconstructed #omega");
		
		TH1D* histoTrueMassCaloPhotonMeson =          (TH1D*)fileCorrections->Get("histoTrueMassMesonCaloPhoton");
		if (histoTrueMassCaloPhotonMeson){
			DrawGammaSetMarker(histoTrueMassCaloPhotonMeson, 25, 0.8, kGreen+2, kGreen+2);
			histoTrueMassCaloPhotonMeson->DrawCopy("same,e1,p"); 
			if(nameMeson.CompareTo("Eta") == 0 ) legendMass2->AddEntry(histoTrueMassCaloPhotonMeson,"True reconstructed #eta, cluster real #gamma");
			if(nameMeson.CompareTo("Omega") == 0 ) legendMass2->AddEntry(histoTrueMassMeson,"True reconstructed #omega, cluster real #gamma");
		}
		TH1D* histoTrueMassCaloConvPhotonMeson =          (TH1D*)fileCorrections->Get("histoTrueMassMesonCaloConvPhoton");
		if (histoTrueMassCaloConvPhotonMeson){
			DrawGammaSetMarker(histoTrueMassCaloConvPhotonMeson, 25, 0.8, kCyan+2, kCyan+2);
			histoTrueMassCaloConvPhotonMeson->DrawCopy("same,e1,p"); 
			if(nameMeson.CompareTo("Eta") == 0 ) legendMass2->AddEntry(histoTrueMassCaloConvPhotonMeson,"True reconstructed #eta, cluster conv #gamma");
			if(nameMeson.CompareTo("Omega") == 0 ) legendMass2->AddEntry(histoTrueMassMeson,"True reconstructed #omega, cluster conv #gamma");
		}
		TH1D* histoTrueMassCaloMergedClusterMeson =          (TH1D*)fileCorrections->Get("histoTrueMassMesonCaloMergedCluster");
		if (histoTrueMassCaloMergedClusterMeson){
			DrawGammaSetMarker(histoTrueMassCaloMergedClusterMeson, 25, 0.8, kViolet+2, kViolet+2);
			histoTrueMassCaloMergedClusterMeson->DrawCopy("same,e1,p"); 
			if(nameMeson.CompareTo("Eta") == 0 ) legendMass2->AddEntry(histoTrueMassCaloMergedClusterMeson,"True reconstructed #eta, merged cluster #gamma");
			if(nameMeson.CompareTo("Omega") == 0 ) legendMass2->AddEntry(histoTrueMassMeson,"True reconstructed #omega, merged cluster #gamma");
		}

		DrawGammaLines(0., maxPtMeson,mesonMassExpect, mesonMassExpect,0.1);
		
		
		legendMass2->Draw();
		canvasMass->Update();	
		canvasMass->SaveAs(Form("%s/%s_%s_MassAddedInfos_%s.%s",outputDir.Data(),nameMeson.Data(),prefix2.Data(),fCutSelection.Data(),suffix.Data()));
	}
	delete canvasMass;
	delete legendMass;

    //**********************************************************************************
	//******************** FWHM Plot *********************************************
	//**********************************************************************************
	TCanvas* canvasFWHM = new TCanvas("canvasFWHM","",200,10,1350,900);  // gives the page size
	DrawGammaCanvasSettings( canvasFWHM, 0.13, 0.02, 0.04, 0.09);
	
	histoFWHMMeson->Sumw2();
	histoFWHMMeson->Scale(1./2.35);

	DrawAutoGammaMesonHistos( histoFWHMMeson, 
								"", "p_{T} (GeV/c)", Form("FWHM/2.35 for %s in |y| < %s (GeV/c^{2})",textMeson.Data(), rapidityRange.Data()), 
								kFALSE, 1.5,-20., kFALSE,
								kTRUE, -0.01, 0.040, 
								kTRUE, 0., 25.);  
	
	histoFWHMMeson->GetYaxis()->SetNdivisions(510); 
	DrawGammaSetMarker(histoFWHMMeson, 22, 0.8, kBlack, kBlack);                
	histoFWHMMeson->DrawCopy("same,e1,p"); 
	
	histoTrueFWHMMeson->Sumw2();
	histoTrueFWHMMeson->Scale(1./2.35);              
	DrawGammaSetMarker(histoTrueFWHMMeson, 24, 0.8, kRed+2, kRed+2);
	histoTrueFWHMMeson->DrawCopy("same,e1,p"); 
	
	DrawGammaSetMarker(histoFWHMMesonLeft, 20, 0.8, kBlue, kBlue);
	histoFWHMMesonLeft->Sumw2();
	histoFWHMMesonLeft->Scale(1./2.35);
	histoFWHMMesonLeft->DrawCopy("same,e1,p"); 
	
	TLegend* legendFWHM = new TLegend(0.15,0.1,0.5,0.2);
	legendFWHM->SetTextSize(0.02);         
	legendFWHM->SetFillColor(0);
    legendFWHM->SetBorderSize(0);
	legendFWHM->AddEntry(histoFWHMMeson,"normal");
	legendFWHM->AddEntry(histoFWHMMesonLeft,"left+right norm");

    if(nameMeson.CompareTo("Eta") == 0 ) legendFWHM->AddEntry(histoTrueFWHMMeson,"True reconstructed #eta");
	if(nameMeson.CompareTo("Omega") == 0 ) legendFWHM->AddEntry(histoTrueMassMeson,"True reconstructed #omega");
	legendFWHM->Draw();
	canvasFWHM->Update();
	
	canvasFWHM->SaveAs(Form("%s/%s_%s_FWHM_%s.%s",outputDir.Data(),nameMeson.Data(),prefix2.Data(),fCutSelection.Data(),suffix.Data()));

	if (mode==2 || mode == 3){
	
		histoFWHMMeson->GetYaxis()->SetRangeUser(-0.004, 0.030); 
		histoFWHMMeson->DrawCopy("e1,p"); 
		histoTrueFWHMMeson->DrawCopy("same,e1,p"); 
			
		TLegend* legendFWHM2 = new TLegend(0.55,0.12,0.95,0.25);
		legendFWHM2->SetTextSize(0.02);         
		legendFWHM2->SetFillColor(0);
		legendFWHM2->SetFillStyle(0);
		legendFWHM2->SetLineColor(0);
		legendFWHM2->AddEntry(histoFWHMMeson,"standard");

        if(nameMeson.CompareTo("Eta") == 0 ) legendFWHM2->AddEntry(histoTrueFWHMMeson,"True reconstructed #eta");
		if(nameMeson.CompareTo("Omega") == 0 ) legendFWHM2->AddEntry(histoTrueMassMeson,"True reconstructed #omega");
		
		TH1D* histoTrueFWHMCaloPhotonMeson =          (TH1D*)fileCorrections->Get("histoTrueFWHMMesonCaloPhoton");
		if (histoTrueFWHMCaloPhotonMeson){
			histoTrueFWHMCaloPhotonMeson->Scale(1./2.35);
			DrawGammaSetMarker(histoTrueFWHMCaloPhotonMeson, 25, 0.8, kGreen+2, kGreen+2);
			histoTrueFWHMCaloPhotonMeson->DrawCopy("same,e1,p"); 

            if(nameMeson.CompareTo("Eta") == 0 ) legendFWHM2->AddEntry(histoTrueFWHMCaloPhotonMeson,"True reconstructed #eta, cluster real #gamma");
			if(nameMeson.CompareTo("Omega") == 0 ) legendFWHM2->AddEntry(histoTrueMassMeson,"True reconstructed #omega, cluster real #gamma");
		}
		TH1D* histoTrueFWHMCaloConvPhotonMeson =          (TH1D*)fileCorrections->Get("histoTrueFWHMMesonCaloConvPhoton");
		if (histoTrueFWHMCaloConvPhotonMeson){
			histoTrueFWHMCaloConvPhotonMeson->Scale(1./2.35);
			DrawGammaSetMarker(histoTrueFWHMCaloConvPhotonMeson, 25, 0.8, kCyan+2, kCyan+2);
			histoTrueFWHMCaloConvPhotonMeson->DrawCopy("same,e1,p"); 

            if(nameMeson.CompareTo("Eta") == 0 ) legendFWHM2->AddEntry(histoTrueFWHMCaloConvPhotonMeson,"True reconstructed #eta, cluster conv #gamma");
			if(nameMeson.CompareTo("Omega") == 0 ) legendFWHM2->AddEntry(histoTrueMassMeson,"True reconstructed #omega, cluster conv #gamma");
		}
		TH1D* histoTrueFWHMCaloMergedClusterMeson =          (TH1D*)fileCorrections->Get("histoTrueFWHMMesonCaloMergedCluster");
		if (histoTrueFWHMCaloMergedClusterMeson){
			histoTrueFWHMCaloMergedClusterMeson->Scale(1./2.35);
			DrawGammaSetMarker(histoTrueFWHMCaloMergedClusterMeson, 25, 0.8, kViolet+2, kViolet+2);
			histoTrueFWHMCaloMergedClusterMeson->DrawCopy("same,e1,p"); 

            if(nameMeson.CompareTo("Eta") == 0 ) legendFWHM2->AddEntry(histoTrueFWHMCaloMergedClusterMeson,"True reconstructed #eta, merged cluster #gamma");
			if(nameMeson.CompareTo("Omega") == 0 ) legendFWHM2->AddEntry(histoTrueMassMeson,"True reconstructed #omega, merged cluster #gamma");
		}
		
		
		legendFWHM2->Draw();
		canvasFWHM->Update();	
		canvasFWHM->SaveAs(Form("%s/%s_%s_FWHMAddedInfos_%s.%s",outputDir.Data(),nameMeson.Data(),prefix2.Data(),fCutSelection.Data(),suffix.Data()));
	}

	delete canvasFWHM;
	delete legendFWHM;
	
	//**************************** Fitting efficiencies ************************************************************************
	Double_t maxPtMesonEffFit;
	Double_t minPtMesonEffFit;
	Int_t offsetCorrectionHighPt;
	TF1* fitTrueEffi;
	if (optionEnergy.CompareTo("PbPb_2.76TeV")==0){
		maxPtMesonEffFit = maxPtMeson;
		minPtMesonEffFit = 1.2;
		offsetCorrectionHighPt= 1;
        fitTrueEffi = new TF1("EffiFitDummy","1 - [0]*TMath::Exp([2]*x)+[2]");
	} else {
        maxPtMesonEffFit = maxPtMeson;
        minPtMesonEffFit = 1.2;
		if (nameMeson.Contains("Eta")){
			offsetCorrectionHighPt= -1;
		} else {
			offsetCorrectionHighPt= -2;
			cout << "doing Omega" << endl;
		}
        fitTrueEffi = new TF1("EffiFitDummy","1 - [0]*TMath::Exp([2]*x)+[2]");
	}
	fitTrueEffi->SetRange(minPtMesonEffFit,maxPtMesonEffFit);
    TFitResultPtr resultEffi = histoTrueEffiPt->Fit(fitTrueEffi,"SNRME+","",minPtMesonEffFit,maxPtMesonEffFit);
	TH1D* histoTrueEffiPtFit = (TH1D*)histoTrueEffiPt->Clone("histoTrueEffiPtFit");
	for (Int_t i = histoTrueEffiPt->GetXaxis()->FindBin(minPtMesonEffFit)+1; i < histoTrueEffiPt->GetXaxis()->FindBin(maxPtMesonEffFit)+offsetCorrectionHighPt; i++){
		Double_t ptStart = histoTrueEffiPt->GetXaxis()->GetBinLowEdge(i);
		Double_t ptEnd = histoTrueEffiPt->GetXaxis()->GetBinUpEdge(i);
		Double_t binWidth = ptEnd-ptStart;

        // needed in ROOT6
        for(UInt_t ipar = 0; ipar < resultEffi->NPar(); ipar++) fitTrueEffi->SetParameter(ipar, resultEffi->GetParams()[ipar]);

        Double_t effi = fitTrueEffi->Integral(ptStart, ptEnd) / binWidth;
		Double_t errorEffi = fitTrueEffi->IntegralError(ptStart, ptEnd, resultEffi->GetParams(), resultEffi->GetCovarianceMatrix().GetMatrixArray() ) / binWidth;
		histoTrueEffiPtFit->SetBinContent(i, effi);
		histoTrueEffiPtFit->SetBinError(i, errorEffi);
	}
	TH1D* histoRatioTrueEffiDivFitted = (TH1D*)histoTrueEffiPt->Clone("histoRatioTrueEffiDivFitted");
	histoRatioTrueEffiDivFitted->Divide(histoTrueEffiPt, histoTrueEffiPtFit, 1. ,1., "B");
	
    TF1* fitTrueEffiNarrow = new TF1("EffiNarrowFitDummy","1 - [0]*TMath::Exp([2]*x)+[2] ");
	fitTrueEffiNarrow->SetRange(minPtMesonEffFit,maxPtMesonEffFit);

    TFitResultPtr resultEffiNarrow = histoTrueEffiNarrowPt->Fit(fitTrueEffiNarrow,"SNRME+","",minPtMesonEffFit,maxPtMesonEffFit);

	TH1D* histoTrueEffiNarrowPtFit = (TH1D*)histoTrueEffiNarrowPt->Clone("histoTrueEffiNarrowPtFit");
	for (Int_t i = histoTrueEffiNarrowPt->GetXaxis()->FindBin(minPtMesonEffFit)+1; i < histoTrueEffiNarrowPt->GetXaxis()->FindBin(maxPtMesonEffFit)+offsetCorrectionHighPt; i++){
		Double_t ptStart = histoTrueEffiNarrowPt->GetXaxis()->GetBinLowEdge(i);
		Double_t ptEnd = histoTrueEffiNarrowPt->GetXaxis()->GetBinUpEdge(i);
		Double_t binWidth = ptEnd-ptStart;
        for(UInt_t ipar = 0; ipar < resultEffiNarrow->NPar(); ipar++) fitTrueEffiNarrow->SetParameter(ipar, resultEffiNarrow->GetParams()[ipar]);
        Double_t effi = fitTrueEffiNarrow->Integral(ptStart, ptEnd) / binWidth;
		Double_t errorEffiNarrow = fitTrueEffiNarrow->IntegralError(ptStart, ptEnd, resultEffiNarrow->GetParams(), resultEffiNarrow->GetCovarianceMatrix().GetMatrixArray() ) / binWidth;
		histoTrueEffiNarrowPtFit->SetBinContent(i, effi);
		histoTrueEffiNarrowPtFit->SetBinError(i, errorEffiNarrow);
	}

    TF1* fitTrueEffiWide = new TF1("EffiWideFitDummy","1 - [0]*TMath::Exp([2]*x)+[2] ");
	fitTrueEffiWide->SetRange(minPtMesonEffFit,maxPtMesonEffFit);

    TFitResultPtr resultEffiWide = histoTrueEffiWidePt->Fit(fitTrueEffiWide,"SNRME+","",minPtMesonEffFit,maxPtMesonEffFit);

	TH1D* histoTrueEffiWidePtFit = (TH1D*)histoTrueEffiWidePt->Clone("histoTrueEffiWidePtFit");
	for (Int_t i = histoTrueEffiWidePt->GetXaxis()->FindBin(minPtMesonEffFit)+1; i < histoTrueEffiWidePt->GetXaxis()->FindBin(maxPtMesonEffFit)+offsetCorrectionHighPt; i++){
		Double_t ptStart = histoTrueEffiWidePt->GetXaxis()->GetBinLowEdge(i);
		Double_t ptEnd = histoTrueEffiWidePt->GetXaxis()->GetBinUpEdge(i);
		Double_t binWidth = ptEnd-ptStart;
        for(UInt_t ipar = 0; ipar < resultEffiWide->NPar(); ipar++) fitTrueEffiWide->SetParameter(ipar, resultEffiWide->GetParams()[ipar]);
        Double_t effi = fitTrueEffiWide->Integral(ptStart, ptEnd) / binWidth;
		Double_t errorEffiWide = fitTrueEffiWide->IntegralError(ptStart, ptEnd, resultEffiWide->GetParams(), resultEffiWide->GetCovarianceMatrix().GetMatrixArray() ) / binWidth;
		histoTrueEffiWidePtFit->SetBinContent(i, effi);
		histoTrueEffiWidePtFit->SetBinError(i, errorEffiWide);
	}

	
	//**********************************************************************************
	//******************** Acceptance Plot *********************************************
	//**********************************************************************************
    if (isMC == kTRUE){
		TCanvas* canvasAcceptance = new TCanvas("canvasAcceptance","",200,10,1350,900);  // gives the page size
		DrawGammaCanvasSettings( canvasAcceptance, 0.13, 0.02, 0.02, 0.09);
		
		histoAcceptance->SetXTitle("p_{T} (GeV/c)");
		if (nameMeson.CompareTo("Pi0") == 0 || nameMeson.CompareTo("Pi0EtaBinning") == 0 ){
			if (optionEnergy.CompareTo("pPb_5.023TeV")==0) histoAcceptance->GetYaxis()->SetRangeUser(0.2,1.02);
			else histoAcceptance->GetYaxis()->SetRangeUser(0.7,1.02);   
		} else {
			if (optionEnergy.CompareTo("pPb_5.023TeV")==0) histoAcceptance->GetYaxis()->SetRangeUser(0.1,1.);
				else  histoAcceptance->GetYaxis()->SetRangeUser(0.,1.1);
		}  
		histoAcceptance->SetYTitle( Form("Geom. Acceptance for %s in |y| < %s",textMeson.Data(),rapidityRange.Data()));  
		histoAcceptance->GetXaxis()->SetLabelSize(0.03);
		histoAcceptance->GetYaxis()->SetLabelSize(0.03);
		histoAcceptance->GetYaxis()->SetTitleOffset(1.2);
		histoAcceptance->SetMarkerStyle(22);
		histoAcceptance->SetMarkerSize(1.);
		histoAcceptance->SetMarkerColor(kAzure-6);
		histoAcceptance->SetLineColor(kAzure-6);
		histoAcceptance->DrawCopy("e1"); 
		
		canvasAcceptance->Update();
			
		canvasAcceptance->SaveAs(Form("%s/%s_Acceptance_%s.%s",outputDir.Data(),nameMeson.Data(),fCutSelection.Data(),suffix.Data()));
		delete canvasAcceptance;
			
		//**********************************************************************************
		//******************** Efficiency Simple Plot **************************************
		//**********************************************************************************
		TCanvas* canvasEffSimple = new TCanvas("canvasEffSimple","",200,10,1350,900);  // gives the page size
		DrawGammaCanvasSettings( canvasEffSimple, 0.13, 0.02, 0.03, 0.09);
		//       canvasEffSimple->SetLogy(1);  
					
		DrawAutoGammaMesonHistos( histoTrueEffiPt, 
									"", "p_{T} (GeV/c)", "True Efficiency", 
									kTRUE, 1., 3e-6, kFALSE,
									kFALSE, 0., 0.7, 
									kTRUE, 0., 25.);
				
		DrawGammaSetMarker(histoTrueEffiPt, 22, 1., kBlack, kBlack);   
		histoTrueEffiPt->DrawCopy("e1");    
		canvasEffSimple->Update();
		
		canvasEffSimple->SaveAs(Form("%s/%s_TrueEffSimple_%s.%s",outputDir.Data(),nameMeson.Data(),fCutSelection.Data(),suffix.Data()));
		delete canvasEffSimple;
		

		//*********************************************************************************
		//********************** True Efficiency Plot ******************************************
		//*********************************************************************************
		
		

		TCanvas* canvasEffi = new TCanvas("canvasEffi","",1350,1200);  // gives the page size
		DrawGammaCanvasSettings( canvasEffi, 0.13, 0.02, 0.03, 0.09);

		DrawAutoGammaMesonHistos( histoTrueEffiPt, 
									"", "p_{T} (GeV/c)", "True Efficiency", 
									kFALSE, 0.75, 3e-6, kFALSE,
									kFALSE, 0., 0.7, 
									kTRUE, 0., 25.);

		DrawGammaSetMarker(histoTrueEffiPt, 22, 1., kBlack, kBlack);                               
		histoTrueEffiPt->DrawCopy("e1");    
		
		//right Side Normalization narrow
		DrawGammaSetMarker(histoTrueEffiNarrowPt, 26, 1., kGray+1, kGray+1);                               
		histoTrueEffiNarrowPt->DrawCopy("e1,same"); 
		
		//       //right Side Normalization wide
		DrawGammaSetMarker(histoTrueEffiWidePt, 26, 1., kGray+3, kGray+3);                               
		histoTrueEffiWidePt->DrawCopy("e1,same"); 
		
        if (optionEnergy.CompareTo("PbPb_2.76TeV")==0  || optionEnergy.CompareTo("7TeV")==0 ){         //|| optionEnergy.CompareTo("2.76TeV")==0
			fitTrueEffi->SetLineColor(kRed+2);
			fitTrueEffi->Draw("same");
			fitTrueEffiNarrow->SetLineColor(kRed-2);
			fitTrueEffiNarrow->Draw("same");
			fitTrueEffiWide->SetLineColor(kRed-4);
			fitTrueEffiWide->Draw("same");
			DrawGammaSetMarker(histoTrueEffiPtFit, 20, 1., kRed+2, kRed+2);                               
			histoTrueEffiPtFit->DrawCopy("e1,same"); 
			DrawGammaSetMarker(histoTrueEffiNarrowPtFit, 26, 1., kRed-2, kRed-2);                               
			histoTrueEffiNarrowPtFit->DrawCopy("e1,same"); 
			DrawGammaSetMarker(histoTrueEffiWidePtFit, 26, 1., kRed-4, kRed-4);                              
			histoTrueEffiWidePtFit->DrawCopy("e1,same"); 
		} 
		
		
		TLegend* legendTrueEff = new TLegend(0.6,0.13,0.98,0.24);
		legendTrueEff->SetTextSize(0.02);         
		legendTrueEff->SetFillColor(0);
        legendTrueEff->SetBorderSize(0);
		legendTrueEff->AddEntry(histoTrueEffiPt,"true normal");
        if (optionEnergy.CompareTo("PbPb_2.76TeV")==0 || optionEnergy.CompareTo("7TeV")==0  ) legendTrueEff->AddEntry(fitTrueEffi,"true fitted"); //|| optionEnergy.CompareTo("2.76TeV")==0
		legendTrueEff->AddEntry(histoTrueEffiWidePt,"true wide int");
        if (optionEnergy.CompareTo("PbPb_2.76TeV")==0 || optionEnergy.CompareTo("7TeV")==0 )  legendTrueEff->AddEntry(fitTrueEffiWide,"true wide fitted");
		legendTrueEff->AddEntry(histoTrueEffiNarrowPt,"true narrow int");
        if (optionEnergy.CompareTo("PbPb_2.76TeV")==0 || optionEnergy.CompareTo("7TeV")==0 )  legendTrueEff->AddEntry(fitTrueEffiNarrow,"true narrow fitted");
		legendTrueEff->Draw();
		
		canvasEffi->Update();

		canvasEffi->SaveAs(Form("%s/%s_%s_TrueEfficiency_%s.%s",outputDir.Data(),nameMeson.Data(),prefix2.Data(),fCutSelection.Data(),suffix.Data()));
		delete legendTrueEff;
		delete canvasEffi;
	}
	
	//**********************************************************************************
	//*************************** MC Yield *********************************************
	//**********************************************************************************

	TCanvas* canvasMCYieldMeson = new TCanvas("canvasMCYieldMeson","",1350,1500);  // gives the page size
	DrawGammaCanvasSettings( canvasMCYieldMeson, 0.13, 0.02, 0.02, 0.09);
	canvasMCYieldMeson->SetLogy();

	TH1D *histoMCYieldMesonOldBin = (TH1D*)histoInputMesonOldBinPt->Clone();
	histoMCYieldMesonOldBin->SetName("MCYield_Meson_oldBin");
	ScaleMCYield(histoMCYieldMesonOldBin,  deltaRapid,  scaling,  nEvtMC,  nameMeson ,optDalitz);
	DrawAutoGammaMesonHistos( histoMCYieldMesonOldBin, 
							"", "p_{T} (GeV/c)", "#frac{1}{2#pi N_{ev.}} #frac{d^{2}N}{p_{T}dp_{T}dy} (c/GeV)^{2}",
							kFALSE, 3., 4e-10, kTRUE,
							kFALSE, 0., 0.7, 
							kTRUE, 0., 25.);
	if (histoInputMesonOldBinPtWOWeights){
		ScaleMCYield(histoInputMesonOldBinPtWOWeights,  deltaRapid,  scaling,  nEvtMC,  nameMeson ,optDalitz);
		histoInputMesonOldBinPtWOWeights->SetName("MCYield_Meson_oldBinWOWeights");
	}   
	if (histoMCInputAddedSig){
		ScaleMCYield(histoMCInputAddedSig,  deltaRapid,  scaling,  nEvtMC,  nameMeson ,optDalitz);
		histoMCInputAddedSig->SetName("MCYield_Meson_oldBin_AddedSig");
	}   
	if (histoMCInputWOWeightingAddedSig){
		ScaleMCYield(histoMCInputWOWeightingAddedSig,  deltaRapid,  scaling,  nEvtMC,  nameMeson ,optDalitz);
		histoMCInputWOWeightingAddedSig->SetName("MCYield_Meson_oldBinWOWeights_AddedSig");
	}   

	TH1D *histoMCYieldMeson = (TH1D*)histoInputMesonPt->Clone();
	histoMCYieldMeson->SetName("MCYield_Meson");
	ScaleMCYield(histoMCYieldMeson,  deltaRapid,  scaling,  nEvtMC,  nameMeson ,optDalitz);

	DrawAutoGammaMesonHistos(histoMCYieldMeson , 
							"", "p_{T} (GeV/c)", "#frac{1}{2#pi N_{ev.}} #frac{d^{2}N}{p_{T}dp_{T}dy} (c/GeV)^{2}",
							kFALSE, 3., 4e-10, kTRUE,
							kFALSE, 0., 0.7, 
							kFALSE, 0., histoUnCorrectedYield->GetXaxis()->GetBinUpEdge(histoUnCorrectedYield->GetNbinsX()));

	TF1* fitTsallisMC;
	if (nameMeson.CompareTo("Omega")==0 ){
		fitTsallisMC= FitObject("l","fitTsallisMC","Omega",histoMCYieldMesonOldBin,0.3,histoUnCorrectedYield->GetXaxis()->GetBinUpEdge(histoUnCorrectedYield->GetNbinsX()),NULL,"QNRME+");
	} else { 
		fitTsallisMC= FitObject("l","fitTsallisMC","Eta",histoMCYieldMesonOldBin,0.3,histoUnCorrectedYield->GetXaxis()->GetBinUpEdge(histoUnCorrectedYield->GetNbinsX()),NULL,"QNRME+");
	}
	DrawGammaSetMarkerTF1(fitTsallisMC, 1, 1.5, kBlue);
	TString forOutput= WriteParameterToFile(fitTsallisMC);
	cout << forOutput.Data()<< endl;
	histoMCYieldMesonOldBin->Draw("same");
	fitTsallisMC->Draw("same");
	
	canvasMCYieldMeson->SaveAs(Form("%s/%s_histoMCYieldMeson_%s.%s",outputDir.Data(),nameMeson.Data(),fCutSelection.Data(),suffix.Data()));
	delete canvasMCYieldMeson;             
	

	//**********************************************************************************
	//******************** RAW Yield spectrum ******************************************
	//**********************************************************************************
		
	TCanvas* canvasRAWYield = new TCanvas("canvasRAWYield","",200,10,1350,900);  // gives the page size
	DrawGammaCanvasSettings( canvasRAWYield, 0.13, 0.02, 0.02, 0.09); 
	canvasRAWYield->SetLogy(1);         

	TH1D* histoUnCorrectedYieldDrawing = (TH1D*)histoUnCorrectedYield->Clone();
	histoUnCorrectedYieldDrawing->Scale(1./nEvt);
	DrawAutoGammaMesonHistos( histoUnCorrectedYieldDrawing, 
								"", "p_{T} (GeV/c)", "RAW Yield/ N_{Evt}", 
								kTRUE, 3., 4e-10, kTRUE,
								kFALSE, 0., 0.7, 
								kTRUE, 0., 25.);
    histoUnCorrectedYieldDrawing->SetLineWidth(1.);
	DrawGammaSetMarker(histoUnCorrectedYieldDrawing, 20, 0.5, kBlack, kBlack);                             
	histoUnCorrectedYieldDrawing->DrawCopy("e1");   

	TH1D* histoUnCorrectedYieldDrawingBackFit = (TH1D*)histoUnCorrectedYieldBackFit->Clone();
	histoUnCorrectedYieldDrawingBackFit->Scale(1./nEvt);

	    histoUnCorrectedYieldDrawingBackFit->SetLineWidth(1.);
	DrawGammaSetMarker(histoUnCorrectedYieldDrawingBackFit, 20, 0.5, kRed, kRed);                             
	histoUnCorrectedYieldDrawingBackFit->DrawCopy("e1same");   

	canvasRAWYield->Update();

	canvasRAWYield->SaveAs(Form("%s/%s_%s_RAWYieldPt_%s.%s",outputDir.Data(),nameMeson.Data(),prefix2.Data(),fCutSelection.Data(),suffix.Data()));
	delete canvasRAWYield;

	// Scale backFit uncorrected yield

    //
    // ─── CORRECTED YIELDS NORM EFFI ─────────────────────────────────────────────────
    //
	

	TH1D* histoCorrectedYieldNorm = (TH1D*)histoUnCorrectedYield->Clone();
    histoCorrectedYieldNorm->SetName(Form("CorrectedYieldNormEff%s",InvMassTypeEnding.Data()));
	TH1D* histoCorrectedYieldNormNarrow = (TH1D*)histoUnCorrectedYieldNarrow->Clone();
    histoCorrectedYieldNormNarrow->SetName(Form("CorrectedYieldNormEffNarrow%s",InvMassTypeEnding.Data()));
	TH1D* histoCorrectedYieldNormWide = (TH1D*)histoUnCorrectedYieldWide->Clone();
    histoCorrectedYieldNormWide->SetName(Form("CorrectedYieldNormEffWide%s",InvMassTypeEnding.Data()));

    TH1D* histoCorrectedYieldNormBackFit = (TH1D*)histoUnCorrectedYieldBackFit->Clone();
    histoCorrectedYieldNormBackFit->SetName(Form("CorrectedYieldNormEffBackFit%s",InvMassTypeEnding.Data()));
	TH1D* histoCorrectedYieldNormWideBackFit = (TH1D*)histoUnCorrectedYieldWideBackFit->Clone();
    histoCorrectedYieldNormWideBackFit->SetName(Form("CorrectedYieldNormEffWideBackFit%s",InvMassTypeEnding.Data()));
	TH1D* histoCorrectedYieldNormNarrowBackFit = (TH1D*)histoUnCorrectedYieldNarrowBackFit->Clone();
    histoCorrectedYieldNormNarrowBackFit->SetName(Form("CorrectedYieldNormEffNarrowBackFit%s",InvMassTypeEnding.Data()));

    // effi from fit
	 TH1D* histoCorrectedYieldNormBackFitFromFit = (TH1D*)histoUnCorrectedYieldBackFit->Clone();
    histoCorrectedYieldNormBackFitFromFit->SetName(Form("CorrectedYieldNormEffBackFitFromFit%s",InvMassTypeEnding.Data()));
	TH1D* histoCorrectedYieldNormWideBackFitFromFit = (TH1D*)histoUnCorrectedYieldWideBackFit->Clone();
    histoCorrectedYieldNormWideBackFitFromFit->SetName(Form("CorrectedYieldNormEffWideBackFitFromFit%s",InvMassTypeEnding.Data()));
	TH1D* histoCorrectedYieldNormNarrowBackFitFromFit = (TH1D*)histoUnCorrectedYieldNarrowBackFit->Clone();
    histoCorrectedYieldNormNarrowBackFitFromFit->SetName(Form("CorrectedYieldNormEffNarrowBackFitFromFit%s",InvMassTypeEnding.Data()));
	
	// averaged
	 TH1D* histoCorrectedYieldAveragedEffiBackFit = (TH1D*)histoUnCorrectedYieldBackFit->Clone();
    histoCorrectedYieldAveragedEffiBackFit->SetName(Form("CorrectedYieldAveragedEffiBackFit%s",InvMassTypeEnding.Data()));

    TH1D* histoCorrectedYieldAveragedEffiWideBackFit = (TH1D*)histoUnCorrectedYieldWideBackFit->Clone();
    histoCorrectedYieldAveragedEffiWideBackFit->SetName(Form("CorrectedYieldAveragedEffiWideBackFit%s",InvMassTypeEnding.Data()));

    TH1D* histoCorrectedYieldAveragedEffiNarrowBackFit = (TH1D*)histoUnCorrectedYieldNarrowBackFit->Clone();
    histoCorrectedYieldAveragedEffiNarrowBackFit->SetName(Form("CorrectedYieldAveragedEffiNarrowBackFit%s",InvMassTypeEnding.Data()));

    // averaged
	 TH1D* histoCorrectedYieldAveragedEffi = (TH1D*)histoUnCorrectedYield->Clone();
    histoCorrectedYieldAveragedEffi->SetName(Form("CorrectedYieldAveragedEffi%s",InvMassTypeEnding.Data()));

    TH1D* histoCorrectedYieldAveragedEffiWide = (TH1D*)histoUnCorrectedYieldWide->Clone();
    histoCorrectedYieldAveragedEffiWide->SetName(Form("CorrectedYieldAveragedEffiWide%s",InvMassTypeEnding.Data()));

    TH1D* histoCorrectedYieldAveragedEffiNarrow = (TH1D*)histoUnCorrectedYieldNarrow->Clone();
    histoCorrectedYieldAveragedEffiNarrow->SetName(Form("CorrectedYieldAveragedEffiNarrow%s",InvMassTypeEnding.Data()));

	//***********************************************************************************************
	//***************************  correction for yield with True effi ******************************
	//***********************************************************************************************
	TH1D* histoCorrectedYieldTrue = (TH1D*)histoUnCorrectedYield->Clone();
    histoCorrectedYieldTrue->SetName(Form("CorrectedYieldTrueEff%s",InvMassTypeEnding.Data()));
	TH1D* histoCorrectedYieldTrueNarrow = (TH1D*)histoUnCorrectedYieldNarrow->Clone();
    histoCorrectedYieldTrueNarrow->SetName(Form("CorrectedYieldTrueEffNarrow%s",InvMassTypeEnding.Data()));
	TH1D* histoCorrectedYieldTrueWide = (TH1D*)histoUnCorrectedYieldWide->Clone();
    histoCorrectedYieldTrueWide->SetName(Form("CorrectedYieldTrueEffWide%s",InvMassTypeEnding.Data()));
	TH1D* histoCorrectedYieldFixed = (TH1D*)histoUnCorrectedYield->Clone();
    histoCorrectedYieldFixed->SetName(Form("CorrectedYieldEffFixed%s",InvMassTypeEnding.Data()));
	TH1D* histoCorrectedYieldNarrowFixed = (TH1D*)histoUnCorrectedYieldNarrow->Clone();
    histoCorrectedYieldNarrowFixed->SetName(Form("CorrectedYieldEffNarrowFixed%s",InvMassTypeEnding.Data()));
	TH1D* histoCorrectedYieldWideFixed = (TH1D*)histoUnCorrectedYieldWide->Clone();
    histoCorrectedYieldWideFixed->SetName(Form("CorrectedYieldEffWideFixed%s",InvMassTypeEnding.Data()));
	TH1D* histoCorrectedYieldTrueFixed = (TH1D*)histoUnCorrectedYield->Clone();
    histoCorrectedYieldTrueFixed->SetName(Form("CorrectedYieldTrueEffFixed%s",InvMassTypeEnding.Data()));
	TH1D* histoCorrectedYieldTrueNarrowFixed = (TH1D*)histoUnCorrectedYieldNarrow->Clone();
    histoCorrectedYieldTrueNarrowFixed->SetName(Form("CorrectedYieldTrueEffNarrowFixed%s",InvMassTypeEnding.Data()));
	TH1D* histoCorrectedYieldTrueWideFixed = (TH1D*)histoUnCorrectedYieldWide->Clone();
    histoCorrectedYieldTrueWideFixed->SetName(Form("CorrectedYieldTrueEffWideFixed%s",InvMassTypeEnding.Data()));
	TH1D* histoCorrectedYieldTrueLeft = (TH1D*)histoUnCorrectedYieldLeft->Clone();
    histoCorrectedYieldTrueLeft->SetName(Form("CorrectedYieldTrueEffLeft%s",InvMassTypeEnding.Data()));
	TH1D* histoCorrectedYieldTrueLeftNarrow = (TH1D*)histoUnCorrectedYieldLeftNarrow->Clone();
    histoCorrectedYieldTrueLeftNarrow->SetName(Form("CorrectedYieldTrueEffLeftNarrow%s",InvMassTypeEnding.Data()));
	TH1D* histoCorrectedYieldTrueLeftWide = (TH1D*)histoUnCorrectedYieldLeftWide->Clone();
    histoCorrectedYieldTrueLeftWide->SetName(Form("CorrectedYieldTrueEffLeftWide%s",InvMassTypeEnding.Data()));
	TH1D* histoCorrectedYieldTrueFitted = (TH1D*)histoUnCorrectedYield->Clone();
    histoCorrectedYieldTrueFitted->SetName(Form("CorrectedYieldTrueEffFitted%s",InvMassTypeEnding.Data()));
	TH1D* histoCorrectedYieldTrueNarrowFitted = (TH1D*)histoUnCorrectedYieldNarrow->Clone();
    histoCorrectedYieldTrueNarrowFitted->SetName(Form("CorrectedYieldTrueEffNarrowFitted%s",InvMassTypeEnding.Data()));
	TH1D* histoCorrectedYieldTrueWideFitted = (TH1D*)histoUnCorrectedYieldWide->Clone();
    histoCorrectedYieldTrueWideFitted->SetName(Form("CorrectedYieldTrueEffWideFitted%s",InvMassTypeEnding.Data()));
	TH1D* histoCorrectedYieldTrueLeftFitted = (TH1D*)histoUnCorrectedYieldLeft->Clone();
    histoCorrectedYieldTrueLeftFitted->SetName(Form("CorrectedYieldTrueEffLeftFitted%s",InvMassTypeEnding.Data()));
	TH1D* histoCorrectedYieldTrueLeftNarrowFitted = (TH1D*)histoUnCorrectedYieldLeftNarrow->Clone();
    histoCorrectedYieldTrueLeftNarrowFitted->SetName(Form("CorrectedYieldTrueEffLeftNarrowFitted%s",InvMassTypeEnding.Data()));
	TH1D* histoCorrectedYieldTrueLeftWideFitted = (TH1D*)histoUnCorrectedYieldLeftWide->Clone();
    histoCorrectedYieldTrueLeftWideFitted->SetName(Form("CorrectedYieldTrueEffLeftWideFitted%s",InvMassTypeEnding.Data()));

    // Corrected yields for fitted background
	TH1D* histoCorrectedYieldTrueBackFit = (TH1D*)histoUnCorrectedYieldBackFit->Clone();
    histoCorrectedYieldTrueBackFit->SetName(Form("CorrectedYieldTrueEffBackFit%s",InvMassTypeEnding.Data()));
	TH1D* histoCorrectedYieldTrueNarrowBackFit = (TH1D*)histoUnCorrectedYieldNarrowBackFit->Clone();
    histoCorrectedYieldTrueNarrowBackFit->SetName(Form("CorrectedYieldTrueEffNarrowBackFit%s",InvMassTypeEnding.Data()));
	TH1D* histoCorrectedYieldTrueWideBackFit = (TH1D*)histoUnCorrectedYieldWideBackFit->Clone();
    histoCorrectedYieldTrueWideBackFit->SetName(Form("CorrectedYieldTrueEffWideBackFit%s",InvMassTypeEnding.Data()));
	TH1D* histoCorrectedYieldFixedBackFit = (TH1D*)histoUnCorrectedYieldBackFit->Clone();
    histoCorrectedYieldFixedBackFit->SetName(Form("CorrectedYieldEffFixedBackFit%s",InvMassTypeEnding.Data()));
	TH1D* histoCorrectedYieldNarrowFixedBackFit = (TH1D*)histoUnCorrectedYieldNarrowBackFit->Clone();
    histoCorrectedYieldNarrowFixedBackFit->SetName(Form("CorrectedYieldEffNarrowFixedBackFit%s",InvMassTypeEnding.Data()));
	TH1D* histoCorrectedYieldWideFixedBackFit = (TH1D*)histoUnCorrectedYieldWideBackFit->Clone();
    histoCorrectedYieldWideFixedBackFit->SetName(Form("CorrectedYieldEffWideFixedBackFit%s",InvMassTypeEnding.Data()));
	TH1D* histoCorrectedYieldTrueFixedBackFit = (TH1D*)histoUnCorrectedYieldBackFit->Clone();
    histoCorrectedYieldTrueFixedBackFit->SetName(Form("CorrectedYieldTrueEffFixedBackFit%s",InvMassTypeEnding.Data()));
	TH1D* histoCorrectedYieldTrueNarrowFixedBackFit = (TH1D*)histoUnCorrectedYieldNarrowBackFit->Clone();
    histoCorrectedYieldTrueNarrowFixedBackFit->SetName(Form("CorrectedYieldTrueEffNarrowFixedBackFit%s",InvMassTypeEnding.Data()));
	TH1D* histoCorrectedYieldTrueWideFixedBackFit = (TH1D*)histoUnCorrectedYieldWideBackFit->Clone();
    histoCorrectedYieldTrueWideFixedBackFit->SetName(Form("CorrectedYieldTrueEffWideFixedBackFit%s",InvMassTypeEnding.Data()));
	TH1D* histoCorrectedYieldTrueFittedBackFit = (TH1D*)histoUnCorrectedYieldBackFit->Clone();
    histoCorrectedYieldTrueFittedBackFit->SetName(Form("CorrectedYieldTrueEffFittedBackFit%s",InvMassTypeEnding.Data()));
	TH1D* histoCorrectedYieldTrueNarrowFittedBackFit = (TH1D*)histoUnCorrectedYieldNarrowBackFit->Clone();
    histoCorrectedYieldTrueNarrowFittedBackFit->SetName(Form("CorrectedYieldTrueEffNarrowFittedBackFit%s",InvMassTypeEnding.Data()));
	TH1D* histoCorrectedYieldTrueWideFittedBackFit = (TH1D*)histoUnCorrectedYieldWideBackFit->Clone();
    histoCorrectedYieldTrueWideFittedBackFit->SetName(Form("CorrectedYieldTrueEffWideFittedBackFit%s",InvMassTypeEnding.Data()));
	
	TH1D* histoCompleteCorr = (TH1D*)histoTrueEffiPt->Clone();

    if (!optDalitz){
		//
		// ─── CORRECTION NORM EFFI ────────────────────────────────────────
		//
        CorrectYield(histoCorrectedYieldNorm, histoEffiPt, histoAcceptance, deltaRapid, scaling, nEvt, nameMeson,optDecayChannel);
        CorrectYield(histoCorrectedYieldNormWide, histoEffiWidePt, histoAcceptance, deltaRapid, scaling, nEvt, nameMeson,optDecayChannel);
        CorrectYield(histoCorrectedYieldNormNarrow, histoEffiNarrowPt, histoAcceptance, deltaRapid, scaling, nEvt, nameMeson,optDecayChannel);

        CorrectYield(histoCorrectedYieldNormBackFit, histoEffiPtBackFit, histoAcceptance, deltaRapid, scaling, nEvt, nameMeson,optDecayChannel);
        CorrectYield(histoCorrectedYieldNormWideBackFit, histoEffiWidePtBackFit, histoAcceptance, deltaRapid, scaling, nEvt, nameMeson,optDecayChannel);
        CorrectYield(histoCorrectedYieldNormNarrowBackFit, histoEffiNarrowPtBackFit, histoAcceptance, deltaRapid, scaling, nEvt, nameMeson,optDecayChannel);

        CorrectYield(histoCorrectedYieldNormBackFitFromFit, histoEffiPtBackFitFromFit, histoAcceptance, deltaRapid, scaling, nEvt, nameMeson,optDecayChannel);
        CorrectYield(histoCorrectedYieldNormWideBackFitFromFit, histoEffiPtBackFitFromFit, histoAcceptance, deltaRapid, scaling, nEvt, nameMeson,optDecayChannel);
        CorrectYield(histoCorrectedYieldNormNarrowBackFitFromFit, histoEffiPtBackFitFromFit, histoAcceptance, deltaRapid, scaling, nEvt, nameMeson,optDecayChannel);

        CorrectYield(histoCorrectedYieldAveragedEffiBackFit, histoEffiWeightedAverage, histoAcceptance, deltaRapid, scaling, nEvt, nameMeson,optDecayChannel);
        CorrectYield(histoCorrectedYieldAveragedEffiWideBackFit, histoEffiWeightedAverage, histoAcceptance, deltaRapid, scaling, nEvt, nameMeson,optDecayChannel);
        CorrectYield(histoCorrectedYieldAveragedEffiNarrowBackFit, histoEffiWeightedAverage, histoAcceptance, deltaRapid, scaling, nEvt, nameMeson,optDecayChannel);

        CorrectYield(histoCorrectedYieldAveragedEffi, histoEffiWeightedAverage, histoAcceptance, deltaRapid, scaling, nEvt, nameMeson,optDecayChannel);
        CorrectYield(histoCorrectedYieldAveragedEffiWide, histoEffiWeightedAverage, histoAcceptance, deltaRapid, scaling, nEvt, nameMeson,optDecayChannel);
        CorrectYield(histoCorrectedYieldAveragedEffiNarrow, histoEffiWeightedAverage, histoAcceptance, deltaRapid, scaling, nEvt, nameMeson,optDecayChannel);

        //
        // ─── TRUE ────────────────────────────────────────────────────────
        //

        CorrectYield(histoCorrectedYieldTrue, histoTrueEffiPt, histoAcceptance, deltaRapid, scaling, nEvt, nameMeson,optDecayChannel);
        CompileFullCorrectionFactor( histoCompleteCorr, histoAcceptance, deltaRapid);
        CorrectYield(histoCorrectedYieldTrueNarrow, histoTrueEffiNarrowPt, histoAcceptance,  deltaRapid, scaling, nEvt, nameMeson,optDecayChannel);
        CorrectYield(histoCorrectedYieldTrueWide,  histoTrueEffiWidePt, histoAcceptance,  deltaRapid, scaling, nEvt, nameMeson,optDecayChannel);

        CorrectYield(histoCorrectedYieldFixed, histoEffiPtFixed, histoAcceptance, deltaRapid, scaling, nEvt, nameMeson,optDecayChannel);
        CorrectYield(histoCorrectedYieldNarrowFixed, histoEffiNarrowPtFixed, histoAcceptance,  deltaRapid, scaling, nEvt, nameMeson,optDecayChannel);
        CorrectYield(histoCorrectedYieldWideFixed,  histoEffiWidePtFixed, histoAcceptance,  deltaRapid, scaling, nEvt, nameMeson,optDecayChannel);

        CorrectYield(histoCorrectedYieldTrueFixed,  histoTrueEffiPtFixed, histoAcceptance, deltaRapid, scaling, nEvt, nameMeson,optDecayChannel);
        CorrectYield(histoCorrectedYieldTrueNarrowFixed, histoTrueEffiNarrowPtFixed, histoAcceptance,  deltaRapid, scaling, nEvt, nameMeson,optDecayChannel);
        CorrectYield(histoCorrectedYieldTrueWideFixed,  histoTrueEffiWidePtFixed, histoAcceptance,  deltaRapid, scaling, nEvt, nameMeson,optDecayChannel);

        CorrectYield(histoCorrectedYieldTrueLeft, histoTrueEffiPt, histoAcceptance,  deltaRapid, scaling, nEvt, nameMeson,optDecayChannel);
        CorrectYield(histoCorrectedYieldTrueLeftNarrow,  histoTrueEffiNarrowPt, histoAcceptance,  deltaRapid, scaling, nEvt, nameMeson,optDecayChannel);
        CorrectYield(histoCorrectedYieldTrueLeftWide, histoTrueEffiWidePt, histoAcceptance,  deltaRapid, scaling, nEvt, nameMeson,optDecayChannel);

        CorrectYield(histoCorrectedYieldTrueFitted, histoTrueEffiPtFit, histoAcceptance, deltaRapid, scaling, nEvt, nameMeson,optDecayChannel);
        CorrectYield(histoCorrectedYieldTrueNarrowFitted, histoTrueEffiNarrowPtFit, histoAcceptance,  deltaRapid, scaling, nEvt, nameMeson,optDecayChannel);
        CorrectYield(histoCorrectedYieldTrueWideFitted, histoTrueEffiWidePtFit, histoAcceptance,  deltaRapid, scaling, nEvt, nameMeson,optDecayChannel);
        CorrectYield(histoCorrectedYieldTrueLeftFitted, histoTrueEffiPtFit, histoAcceptance,  deltaRapid, scaling, nEvt, nameMeson,optDecayChannel);
        CorrectYield(histoCorrectedYieldTrueLeftNarrowFitted, histoTrueEffiNarrowPtFit, histoAcceptance,  deltaRapid, scaling, nEvt, nameMeson,optDecayChannel);
        CorrectYield(histoCorrectedYieldTrueLeftWideFitted, histoTrueEffiWidePtFit, histoAcceptance,  deltaRapid, scaling, nEvt, nameMeson,optDecayChannel);

        // Correct Yields for fitted background
        CorrectYield(histoCorrectedYieldTrueBackFit, histoTrueEffiPt, histoAcceptance, deltaRapid, scaling, nEvt, nameMeson,optDecayChannel);
        //CompileFullCorrectionFactor( histoCompleteCorr, histoAcceptance, deltaRapid);
        CorrectYield(histoCorrectedYieldTrueNarrowBackFit, histoTrueEffiNarrowPt, histoAcceptance,  deltaRapid, scaling, nEvt, nameMeson,optDecayChannel);
        CorrectYield(histoCorrectedYieldTrueWideBackFit,  histoTrueEffiWidePt, histoAcceptance,  deltaRapid, scaling, nEvt, nameMeson,optDecayChannel);

        CorrectYield(histoCorrectedYieldFixedBackFit, histoEffiPtFixedBackFit, histoAcceptance, deltaRapid, scaling, nEvt, nameMeson,optDecayChannel);
        CorrectYield(histoCorrectedYieldNarrowFixedBackFit, histoEffiNarrowPtFixedBackFit, histoAcceptance,  deltaRapid, scaling, nEvt, nameMeson,optDecayChannel);
        CorrectYield(histoCorrectedYieldWideFixedBackFit,  histoEffiWidePtFixedBackFit, histoAcceptance,  deltaRapid, scaling, nEvt, nameMeson,optDecayChannel);

        CorrectYield(histoCorrectedYieldTrueFixedBackFit,  histoTrueEffiPtFixed, histoAcceptance, deltaRapid, scaling, nEvt, nameMeson,optDecayChannel);
        CorrectYield(histoCorrectedYieldTrueNarrowFixedBackFit, histoTrueEffiNarrowPtFixed, histoAcceptance,  deltaRapid, scaling, nEvt, nameMeson,optDecayChannel);
        CorrectYield(histoCorrectedYieldTrueWideFixedBackFit,  histoTrueEffiWidePtFixed, histoAcceptance,  deltaRapid, scaling, nEvt, nameMeson,optDecayChannel);

        CorrectYield(histoCorrectedYieldTrueFittedBackFit, histoTrueEffiPtFit, histoAcceptance, deltaRapid, scaling, nEvt, nameMeson,optDecayChannel);
        CorrectYield(histoCorrectedYieldTrueNarrowFittedBackFit, histoTrueEffiNarrowPtFit, histoAcceptance,  deltaRapid, scaling, nEvt, nameMeson,optDecayChannel);
        CorrectYield(histoCorrectedYieldTrueWideFittedBackFit, histoTrueEffiWidePtFit, histoAcceptance,  deltaRapid, scaling, nEvt, nameMeson,optDecayChannel);
    }

	TCanvas* canvasCorrecftedYield = new TCanvas("canvasCorrecftedYield","",1350,1500);  // gives the page size
	DrawGammaCanvasSettings( canvasCorrecftedYield, 0.13, 0.02, 0.02, 0.09);   
	canvasCorrecftedYield->SetLogy();   

	TPad* padCorrectedYieldHistos = new TPad("padCorrectedYieldHistos", "", 0., 0.25, 1., 1.,-1, -1, -2);
	DrawGammaPadSettings( padCorrectedYieldHistos, 0.12, 0.02, 0.02, 0.);
	padCorrectedYieldHistos->Draw();

	TPad* padCorrectedYieldRatios = new TPad("padCorrectedYieldRatios", "", 0., 0., 1., 0.25,-1, -1, -2);
	DrawGammaPadSettings( padCorrectedYieldRatios, 0.12, 0.02, 0., 0.2);
	padCorrectedYieldRatios->Draw();

	padCorrectedYieldHistos->cd();
	padCorrectedYieldHistos->SetLogy();    

    DrawAutoGammaMesonHistos( histoCorrectedYieldTrue,
                                "", "p_{T} (GeV/c)", "#frac{1}{2#pi N_{ev.}} #frac{d^{2}N}{p_{T}dp_{T}dy} (c/GeV)^{2}",
                                kTRUE, 3., 4e-10, kFALSE,
                                kFALSE, 0., 0.7,
                                kFALSE, 0., 25.);
	DrawGammaSetMarker(histoCorrectedYieldTrue, 22, 1., kBlack, kBlack);                             
	histoCorrectedYieldTrue->DrawCopy("e1");  

    //right Side Normalization narrow
	DrawGammaSetMarker(histoCorrectedYieldTrueNarrow, 26, 1., kGray+1, kGray+1);                                
	histoCorrectedYieldTrueNarrow->DrawCopy("e1,same"); 

    //right Side Normalization wide
	DrawGammaSetMarker(histoCorrectedYieldTrueWide, 26, 1., kGray+3, kGray+3);                             
	histoCorrectedYieldTrueWide->DrawCopy("e1,same"); 

    //left Side Normalization
	DrawGammaSetMarker(histoCorrectedYieldTrueLeft, 20, 1., kBlue, kBlue);                              
	histoCorrectedYieldTrueLeft->DrawCopy("e1,same"); 

    //left Side Normalization narrow
	DrawGammaSetMarker(histoCorrectedYieldTrueLeftNarrow, 24, 1., kBlue-5, kBlue-5);                      
	histoCorrectedYieldTrueLeftNarrow->DrawCopy("e1,same"); 

    //left Side Normalization wide
	DrawGammaSetMarker(histoCorrectedYieldTrueLeftWide, 24, 1., kBlue+2, kBlue+2);                               
	histoCorrectedYieldTrueLeftWide->DrawCopy("e1,same");    
	
	TLegend* legendYield3 = new TLegend(0.15,0.03,0.66,0.19);
	legendYield3->SetTextSize(0.02);       
	legendYield3->SetFillColor(0);
    legendYield3->SetBorderSize(0);
	legendYield3->AddEntry(histoCorrectedYieldTrue,"corr true eff/right norm");
	legendYield3->AddEntry(histoCorrectedYieldTrueWide,"corr true eff wide int /right norm");
	legendYield3->AddEntry(histoCorrectedYieldTrueNarrow,"corr true eff narrow int /right norm");
	legendYield3->AddEntry(histoCorrectedYieldTrueLeft,Form("corr true eff /left+right norm"));
	legendYield3->AddEntry(histoCorrectedYieldTrueLeftWide,"corr true eff wide int /left+right norm");
	legendYield3->AddEntry(histoCorrectedYieldTrueLeftNarrow,"corr true eff narrow int /left+right norm");
	legendYield3->Draw();

	padCorrectedYieldRatios->cd();
	padCorrectedYieldRatios->SetTickx();
	padCorrectedYieldRatios->SetTicky();
	padCorrectedYieldRatios->SetLogy(0);
	TH1D *RatioTrue = (TH1D*) histoCorrectedYieldTrue->Clone(); 
	RatioTrue->Divide(RatioTrue,histoCorrectedYieldTrue,1.,1.,"");
	TH1D *RatioTrueWide = (TH1D*) histoCorrectedYieldTrue->Clone();   
	RatioTrueWide->Divide(RatioTrueWide,histoCorrectedYieldTrueWide,1.,1.,"");
	TH1D *RatioTrueNarrow = (TH1D*) histoCorrectedYieldTrue->Clone(); 
	RatioTrueNarrow->Divide(RatioTrueNarrow,histoCorrectedYieldTrueNarrow,1.,1.,"");
	TH1D *RatioTrueLeft = (TH1D*) histoCorrectedYieldTrue->Clone();   
	RatioTrueLeft->Divide(RatioTrueLeft,histoCorrectedYieldTrueLeft,1.,1.,"");
	TH1D *RatioTrueLeftWide = (TH1D*) histoCorrectedYieldTrue->Clone();  
	RatioTrueLeftWide->Divide(RatioTrueLeftWide,histoCorrectedYieldTrueLeftWide,1.,1.,"");
	TH1D *RatioTrueLeftNarrow = (TH1D*) histoCorrectedYieldTrue->Clone();   
	RatioTrueLeftNarrow->Divide(RatioTrueLeftNarrow,histoCorrectedYieldTrueLeftNarrow,1.,1.,"");
	TH1D *RatioTrueMCInput = (TH1D*) histoCorrectedYieldTrue->Clone();   
	RatioTrueMCInput->Divide(RatioTrueMCInput,histoMCYieldMeson,1.,1.,"");
	TH1D *RatioNormal = (TH1D*) histoCorrectedYieldTrue->Clone();  
	RatioNormal->Divide(RatioNormal,histoCorrectedYieldNorm,1.,1.,"");    
    TH1D *RatioNormalMCInput = (TH1D*) histoCorrectedYieldNorm->Clone();
    RatioNormalMCInput->Divide(RatioNormalMCInput,histoMCYieldMeson,1.,1.,"");


	RatioTrue->SetYTitle("#frac{standard}{modified}"); 
	RatioTrue->GetYaxis()->SetRangeUser(0.8,1.23);
	RatioTrue->GetYaxis()->SetLabelSize(0.07);
	RatioTrue->GetYaxis()->SetNdivisions(505);
	RatioTrue->GetYaxis()->SetTitleSize(0.1); 
	RatioTrue->GetYaxis()->SetDecimals();
	RatioTrue->GetYaxis()->SetTitleOffset(0.5);
	RatioTrue->GetXaxis()->SetTitleSize(0.11);   
	RatioTrue->GetXaxis()->SetLabelSize(0.08);
	RatioTrue->SetMarkerStyle(22);
	RatioTrue->SetMarkerSize(1.);
	RatioTrue->SetMarkerColor(kBlack);
	RatioTrue->SetLineColor(kBlack);
	RatioTrue->DrawCopy("p,e1"); 

	DrawGammaSetMarker(RatioTrue, 22, 1., kBlack, kBlack);                               
	RatioTrue->DrawCopy("p,e1");  

    //right Side Normalization narrow
	DrawGammaSetMarker(RatioTrueNarrow, 26, 1., kGray+1, kGray+1);                               
	RatioTrueNarrow->DrawCopy("e1,same"); 

    //right Side Normalization wide
	DrawGammaSetMarker(RatioTrueWide, 26, 1., kGray+3, kGray+3);                               
	RatioTrueWide->DrawCopy("e1,same"); 

    //left Side Normalization
	DrawGammaSetMarker(RatioTrueLeft, 20, 1., kBlue, kBlue);                             
	RatioTrueLeft->DrawCopy("e1,same"); 

    //left Side Normalization narrow
	DrawGammaSetMarker(RatioTrueLeftNarrow, 24, 1., kBlue-5, kBlue-5);                        
	RatioTrueLeftNarrow->DrawCopy("e1,same"); 

    //left Side Normalization wide
	DrawGammaSetMarker(RatioTrueLeftWide, 24, 1., kBlue+2, kBlue+2);                              
	RatioTrueLeftWide->DrawCopy("e1,same"); 

	canvasCorrecftedYield->Update();
	canvasCorrecftedYield->SaveAs(Form("%s/%s_%s_CorrectedYieldTrueEff_%s.%s",outputDir.Data(), nameMeson.Data(), prefix2.Data(),  fCutSelection.Data(), suffix.Data()));

//
	// ─── CORRECTED YIELD BACK INT RANGE COMPARISON ────────────────
	//

    TCanvas* canvasCorrectedYieldBackFitIntRangeComparison = new TCanvas("canvasCorrectedYieldBackFitIntRangeComparison","",1350,1500);  // gives the page size
	DrawGammaCanvasSettings( canvasCorrectedYieldBackFitIntRangeComparison, 0.13, 0.02, 0.02, 0.09);   
	canvasCorrectedYieldBackFitIntRangeComparison->SetLogy();   

	DrawGammaPadSettings( padCorrectedYieldHistos, 0.12, 0.02, 0.02, 0.);
	padCorrectedYieldHistos->Draw();

	DrawGammaPadSettings( padCorrectedYieldRatios, 0.12, 0.02, 0., 0.2);
	padCorrectedYieldRatios->Draw();

	padCorrectedYieldHistos->cd();
	padCorrectedYieldHistos->SetLogy();    

    DrawAutoGammaMesonHistos( histoCorrectedYieldTrueBackFit,
                                "", "p_{T} (GeV/c)", "#frac{1}{2#pi N_{ev.}} #frac{d^{2}N}{p_{T}dp_{T}dy} (c/GeV)^{2}",
                                kTRUE, 3., 4e-10, kFALSE,
                                kFALSE, 0., 0.7,
                                kFALSE, 0., 25.);
	DrawGammaSetMarker(histoCorrectedYieldTrueBackFit, 22, 1., kBlack, kBlack);                             
	histoCorrectedYieldTrueBackFit->DrawCopy("e1");  

	DrawGammaSetMarker(histoCorrectedYieldTrueWideBackFit, 26, 1., kGray+1, kGray+1);                                
	histoCorrectedYieldTrueWideBackFit->DrawCopy("e1,same"); 

	DrawGammaSetMarker(histoCorrectedYieldTrueNarrowBackFit, 20, 1., kBlue, kBlue);                              
	histoCorrectedYieldTrueNarrowBackFit->DrawCopy("e1,same"); 


	
	legendYield3 = new TLegend(0.15,0.03,0.66,0.19);
	legendYield3->SetTextSize(0.02);       
	legendYield3->SetFillColor(0);
    legendYield3->SetBorderSize(0);
	legendYield3->AddEntry(histoCorrectedYieldTrueBackFit,"norm / norm");
	legendYield3->AddEntry(histoCorrectedYieldTrueWideBackFit,"wide / norm");
	legendYield3->AddEntry(histoCorrectedYieldTrueNarrowBackFit,"narrow / norm");
	legendYield3->Draw();

	padCorrectedYieldRatios->cd();
	padCorrectedYieldRatios->SetTickx();
	padCorrectedYieldRatios->SetTicky();
	padCorrectedYieldRatios->SetLogy(0);
	RatioTrue = (TH1D*) histoCorrectedYieldTrueBackFit->Clone(); 
	RatioTrue->Divide(RatioTrue,histoCorrectedYieldTrueBackFit,1.,1.,"");
	TH1D *RatioWide = (TH1D*) histoCorrectedYieldTrueWideBackFit->Clone();   
	RatioWide->Divide(RatioWide,histoCorrectedYieldTrueBackFit,1.,1.,"");
	TH1D *RatioNarrow = (TH1D*) histoCorrectedYieldTrueNarrowBackFit->Clone(); 
	RatioNarrow->Divide(RatioNarrow,histoCorrectedYieldTrueBackFit,1.,1.,"");


	RatioTrue->SetYTitle("#frac{standard}{modified}"); 
	RatioTrue->GetYaxis()->SetRangeUser(0.8,1.23);
	RatioTrue->GetYaxis()->SetLabelSize(0.07);
	RatioTrue->GetYaxis()->SetNdivisions(505);
	RatioTrue->GetYaxis()->SetTitleSize(0.1); 
	RatioTrue->GetYaxis()->SetDecimals();
	RatioTrue->GetYaxis()->SetTitleOffset(0.5);
	RatioTrue->GetXaxis()->SetTitleSize(0.11);   
	RatioTrue->GetXaxis()->SetLabelSize(0.08);
	RatioTrue->SetMarkerStyle(22);
	RatioTrue->SetMarkerSize(1.);
	RatioTrue->SetMarkerColor(kBlack);
	RatioTrue->SetLineColor(kBlack);
	RatioTrue->DrawCopy("p,e1"); 

	DrawGammaSetMarker(RatioTrue, 22, 1., kBlack, kBlack);                               
	RatioTrue->DrawCopy("p,e1");  

	DrawGammaSetMarker(RatioWide, 26, 1., kGray+1, kGray+1);                               
	RatioWide->DrawCopy("e1,same"); 

	DrawGammaSetMarker(RatioNarrow, 26, 1., kGray+3, kGray+3);                               
	RatioNarrow->DrawCopy("e1,same"); 


	canvasCorrectedYieldBackFitIntRangeComparison->Update();
	canvasCorrectedYieldBackFitIntRangeComparison->SaveAs(Form("%s/%s_%s_CorrectedYieldBackFitIntRangeComparison_%s.%s",outputDir.Data(), nameMeson.Data(), prefix2.Data(),  fCutSelection.Data(), suffix.Data()));
	

	
	//
	// ─── CORRECTED YIELD AND CORRECTED YIELD WITH BACKFIT COMPARISON ────────────────
	//

    TCanvas* canvasCorrectedYieldBackFitComparison = new TCanvas("canvasCorrectedYieldBackFitComparison","",1350,1500);  // gives the page size
	DrawGammaCanvasSettings( canvasCorrectedYieldBackFitComparison, 0.13, 0.02, 0.02, 0.09);   
	canvasCorrectedYieldBackFitComparison->SetLogy();   

	DrawGammaPadSettings( padCorrectedYieldHistos, 0.12, 0.02, 0.02, 0.);
	padCorrectedYieldHistos->Draw();

	DrawGammaPadSettings( padCorrectedYieldRatios, 0.12, 0.02, 0., 0.2);
	padCorrectedYieldRatios->Draw();

	padCorrectedYieldHistos->cd();
	padCorrectedYieldHistos->SetLogy();    

    DrawAutoGammaMesonHistos( histoCorrectedYieldTrue,
                                "", "p_{T} (GeV/c)", "#frac{1}{2#pi N_{ev.}} #frac{d^{2}N}{p_{T}dp_{T}dy} (c/GeV)^{2}",
                                kTRUE, 3., 4e-10, kFALSE,
                                kFALSE, 0., 0.7,
                                kFALSE, 0., 25.);
	DrawGammaSetMarker(histoCorrectedYieldTrue, 22, 1., kBlack, kBlack);                             
	histoCorrectedYieldTrue->DrawCopy("e1");  

    // Normal efficiency
	DrawGammaSetMarker(histoCorrectedYieldNorm, 26, 1., kGray+1, kGray+1);                                
	histoCorrectedYieldNorm->DrawCopy("e1,same"); 


    //True efficiency back fit
	DrawGammaSetMarker(histoCorrectedYieldTrueBackFit, 20, 1., kBlue, kBlue);                              
	histoCorrectedYieldTrueBackFit->DrawCopy("e1,same"); 

    //norm efficiency back fit
	DrawGammaSetMarker(histoCorrectedYieldNormBackFit, 24, 1., kBlue-5, kBlue-5);                      
	histoCorrectedYieldNormBackFit->DrawCopy("e1,same"); 

	
	legendYield3 = new TLegend(0.15,0.03,0.66,0.19);
	legendYield3->SetTextSize(0.02);       
	legendYield3->SetFillColor(0);
    legendYield3->SetBorderSize(0);
	legendYield3->AddEntry(histoCorrectedYieldTrue,"true eff / true eff");
	legendYield3->AddEntry(histoCorrectedYieldNorm,"norm eff / true eff");
	legendYield3->AddEntry(histoCorrectedYieldTrueBackFit,"backfit true eff /true eff");
	legendYield3->AddEntry(histoCorrectedYieldNormBackFit,Form("back fit norm eff /true eff"));
	legendYield3->Draw();

	padCorrectedYieldRatios->cd();
	padCorrectedYieldRatios->SetTickx();
	padCorrectedYieldRatios->SetTicky();
	padCorrectedYieldRatios->SetLogy(0);
	RatioTrue = (TH1D*) histoCorrectedYieldTrue->Clone(); 
	RatioTrue->Divide(RatioTrue,histoCorrectedYieldTrue,1.,1.,"");
	TH1D *RatioNorm = (TH1D*) histoCorrectedYieldNorm->Clone();   
	RatioNorm->Divide(RatioNorm,histoCorrectedYieldTrue,1.,1.,"");
	TH1D *RatioBackFitTrue = (TH1D*) histoCorrectedYieldTrueBackFit->Clone(); 
	RatioBackFitTrue->Divide(RatioBackFitTrue,histoCorrectedYieldTrue,1.,1.,"");
	TH1D *RatioBackFitNorm = (TH1D*) histoCorrectedYieldNormBackFit->Clone();   
	RatioBackFitNorm->Divide(RatioBackFitNorm,histoCorrectedYieldTrue,1.,1.,"");

	RatioTrue->SetYTitle("#frac{standard}{modified}"); 
	RatioTrue->GetYaxis()->SetRangeUser(0.8,1.23);
	RatioTrue->GetYaxis()->SetLabelSize(0.07);
	RatioTrue->GetYaxis()->SetNdivisions(505);
	RatioTrue->GetYaxis()->SetTitleSize(0.1); 
	RatioTrue->GetYaxis()->SetDecimals();
	RatioTrue->GetYaxis()->SetTitleOffset(0.5);
	RatioTrue->GetXaxis()->SetTitleSize(0.11);   
	RatioTrue->GetXaxis()->SetLabelSize(0.08);
	RatioTrue->SetMarkerStyle(22);
	RatioTrue->SetMarkerSize(1.);
	RatioTrue->SetMarkerColor(kBlack);
	RatioTrue->SetLineColor(kBlack);
	RatioTrue->DrawCopy("p,e1"); 

	DrawGammaSetMarker(RatioTrue, 22, 1., kBlack, kBlack);                               
	RatioTrue->DrawCopy("p,e1");  

	DrawGammaSetMarker(RatioNorm, 26, 1., kGray+1, kGray+1);                               
	RatioNorm->DrawCopy("e1,same"); 

	DrawGammaSetMarker(RatioBackFitTrue, 26, 1., kGray+3, kGray+3);                               
	RatioBackFitTrue->DrawCopy("e1,same"); 

	DrawGammaSetMarker(RatioBackFitNorm, 20, 1., kBlue, kBlue);                             
	RatioBackFitNorm->DrawCopy("e1,same"); 

	canvasCorrectedYieldBackFitComparison->Update();
	canvasCorrectedYieldBackFitComparison->SaveAs(Form("%s/%s_%s_CorrectedYieldBackFitComparison_%s.%s",outputDir.Data(), nameMeson.Data(), prefix2.Data(),  fCutSelection.Data(), suffix.Data()));
	

	//
	// ─── CORRECTED YIELD COMPARISOn AVERAGED WITH BCKFIT VS EVENTMISIN ────────────────
	//

    TCanvas* canvasCorrectedYieldAveragedComparison = new TCanvas("canvasCorrectedYieldAveragedComparison","",1350,1500);  // gives the page size
	DrawGammaCanvasSettings( canvasCorrectedYieldAveragedComparison, 0.13, 0.02, 0.02, 0.09);   
	canvasCorrectedYieldAveragedComparison->SetLogy();   

	DrawGammaPadSettings( padCorrectedYieldHistos, 0.12, 0.02, 0.02, 0.);
	padCorrectedYieldHistos->Draw();

	DrawGammaPadSettings( padCorrectedYieldRatios, 0.12, 0.02, 0., 0.2);
	padCorrectedYieldRatios->Draw();

	padCorrectedYieldHistos->cd();
	padCorrectedYieldHistos->SetLogy();    

    DrawAutoGammaMesonHistos( histoCorrectedYieldAveragedEffiBackFit,
                                "", "p_{T} (GeV/c)", "#frac{1}{2#pi N_{ev.}} #frac{d^{2}N}{p_{T}dp_{T}dy} (c/GeV)^{2}",
                                kTRUE, 3., 4e-10, kFALSE,
                                kFALSE, 0., 0.7,
                                kFALSE, 0., 25.);
	DrawGammaSetMarker(histoCorrectedYieldAveragedEffiBackFit, 22, 1., kBlack, kBlack);                             
	histoCorrectedYieldAveragedEffiBackFit->DrawCopy("e1");  

    // Normal efficiency
	DrawGammaSetMarker(histoCorrectedYieldAveragedEffi, 26, 1., kRed+2, kRed+2);                                
	histoCorrectedYieldAveragedEffi->DrawCopy("e1,same"); 


	
	legendYield3 = new TLegend(0.15,0.03,0.66,0.19);
	legendYield3->SetTextSize(0.02);       
	legendYield3->SetFillColor(0);
    legendYield3->SetBorderSize(0);
	legendYield3->AddEntry(histoCorrectedYieldAveragedEffiBackFit,"averaged effi w back fit / averaged effi w back fit");
	legendYield3->AddEntry(histoCorrectedYieldAveragedEffi,"averaged effi w back fit / averaged effi w event mixing");

	legendYield3->Draw();

	padCorrectedYieldRatios->cd();
	padCorrectedYieldRatios->SetTickx();
	padCorrectedYieldRatios->SetTicky();
	padCorrectedYieldRatios->SetLogy(0);
	TH1D* RatioAveraged = (TH1D*) histoCorrectedYieldAveragedEffiBackFit->Clone(); 
	RatioAveraged->Divide(RatioAveraged,histoCorrectedYieldAveragedEffiBackFit,1.,1.,"");
	TH1D *RatioAveragedOverEvtMix = (TH1D*) histoCorrectedYieldAveragedEffiBackFit->Clone();   
	RatioAveragedOverEvtMix->Divide(RatioAveragedOverEvtMix,histoCorrectedYieldAveragedEffi,1.,1.,"");


	RatioAveraged->SetYTitle("#frac{standard}{modified}"); 
	RatioAveraged->GetYaxis()->SetRangeUser(0.8,1.23);
	RatioAveraged->GetYaxis()->SetLabelSize(0.07);
	RatioAveraged->GetYaxis()->SetNdivisions(505);
	RatioAveraged->GetYaxis()->SetTitleSize(0.1); 
	RatioAveraged->GetYaxis()->SetDecimals();
	RatioAveraged->GetYaxis()->SetTitleOffset(0.5);
	RatioAveraged->GetXaxis()->SetTitleSize(0.11);   
	RatioAveraged->GetXaxis()->SetLabelSize(0.08);
	RatioAveraged->SetMarkerStyle(22);
	RatioAveraged->SetMarkerSize(1.);
	RatioAveraged->SetMarkerColor(kBlack);
	RatioAveraged->SetLineColor(kBlack);
	RatioAveraged->DrawCopy("p,e1"); 

	DrawGammaSetMarker(RatioAveraged, 22, 1., kBlack, kBlack);                               
	RatioAveraged->DrawCopy("p,e1");  

	DrawGammaSetMarker(RatioAveragedOverEvtMix, 26, 1., kRed+2, kRed+2);                               
	RatioAveragedOverEvtMix->DrawCopy("e1,same"); 



	canvasCorrectedYieldAveragedComparison->Update();
	canvasCorrectedYieldAveragedComparison->SaveAs(Form("%s/%s_%s_CorrectedYieldAveragedBackComparison_%s.%s",outputDir.Data(), nameMeson.Data(), prefix2.Data(),  fCutSelection.Data(), suffix.Data()));
	
	//
	// ─── BACK FIT ONLY CORRECTED YIELDS ─────────────────────────────────────────────
	//

		
	
	TCanvas* canvasCorrectedYieldBackFitOnlyComparison = new TCanvas("canvasCorrectedYieldBackFitOnlyComparison","",1350,1500);  // gives the page size
	DrawGammaCanvasSettings( canvasCorrectedYieldBackFitOnlyComparison, 0.13, 0.02, 0.02, 0.09);   
	canvasCorrectedYieldBackFitOnlyComparison->SetLogy();   

	DrawGammaPadSettings( padCorrectedYieldHistos, 0.12, 0.02, 0.02, 0.);
	padCorrectedYieldHistos->Draw();

	DrawGammaPadSettings( padCorrectedYieldRatios, 0.12, 0.02, 0., 0.2);
	padCorrectedYieldRatios->Draw();

	padCorrectedYieldHistos->cd();
	padCorrectedYieldHistos->SetLogy();    

    DrawAutoGammaMesonHistos( histoCorrectedYieldTrueBackFit,
                                "", "p_{T} (GeV/c)", "#frac{1}{2#pi N_{ev.}} #frac{d^{2}N}{p_{T}dp_{T}dy} (c/GeV)^{2}",
                                kTRUE, 3., 4e-10, kFALSE,
                                kFALSE, 0., 0.7,
                                kFALSE, 0., 25.);
	DrawGammaSetMarker(histoCorrectedYieldTrueBackFit, 22, 1., kBlack, kBlack);                             
	histoCorrectedYieldTrueBackFit->DrawCopy("e1");  

    // True efficiency
	DrawGammaSetMarker(histoCorrectedYieldNormBackFit, 26, 1., kGray+1, kGray+1);                                
	histoCorrectedYieldNormBackFit->DrawCopy("e1,same"); 

	
	legendYield3 = new TLegend(0.15,0.03,0.66,0.19);
	legendYield3->SetTextSize(0.02);       
	legendYield3->SetFillColor(0);
    legendYield3->SetBorderSize(0);
	legendYield3->AddEntry(histoCorrectedYieldTrueBackFit,"backfit true eff /true eff");
	legendYield3->AddEntry(histoCorrectedYieldNormBackFit,Form("back fit norm eff /true eff"));
	legendYield3->Draw();

	padCorrectedYieldRatios->cd();
	padCorrectedYieldRatios->SetTickx();
	padCorrectedYieldRatios->SetTicky();
	padCorrectedYieldRatios->SetLogy(0);
	
	RatioNorm = (TH1D*) histoCorrectedYieldNormBackFit->Clone();   
	RatioNorm->Divide(RatioNorm,histoCorrectedYieldTrueBackFit,1.,1.,"");
	
	RatioNorm->SetYTitle("#frac{standard}{modified}"); 
	RatioNorm->GetYaxis()->SetRangeUser(0.7,1.3);
	RatioNorm->GetYaxis()->SetLabelSize(0.07);
	RatioNorm->GetYaxis()->SetNdivisions(505);
	RatioNorm->GetYaxis()->SetTitleSize(0.1); 
	RatioNorm->GetYaxis()->SetDecimals();
	RatioNorm->GetYaxis()->SetTitleOffset(0.5);
	RatioNorm->GetXaxis()->SetTitleSize(0.11);   
	RatioNorm->GetXaxis()->SetLabelSize(0.08);
	RatioNorm->SetMarkerStyle(22);
	RatioNorm->SetMarkerSize(1.);
	RatioNorm->SetMarkerColor(kBlack);
	RatioNorm->SetLineColor(kBlack);
	RatioNorm->DrawCopy("p,e1"); 

	DrawGammaSetMarker(RatioNorm, 22, 1., kBlack, kBlack);                               
	RatioNorm->DrawCopy("p,e1");  


	canvasCorrectedYieldBackFitOnlyComparison->Update();
	canvasCorrectedYieldBackFitOnlyComparison->SaveAs(Form("%s/%s_%s_CorrectedYieldBackFitOnlyComparison_%s.%s",outputDir.Data(), nameMeson.Data(), prefix2.Data(),  fCutSelection.Data(), suffix.Data()));
	

    if (isMC == kTRUE){
		canvasCorrecftedYield->cd();   

		padCorrectedYieldHistos->cd();
		padCorrectedYieldHistos->SetLogy();    

        DrawGammaSetMarker(histoCorrectedYieldTrue, 22, 1., kBlack, kBlack);
        histoCorrectedYieldTrue->DrawCopy("e1");
        DrawGammaSetMarker(histoMCYieldMeson, 24, 1., kRed+2, kRed+2);
        histoMCYieldMeson->DrawCopy("e1,same");
        DrawGammaSetMarker(histoCorrectedYieldNorm, 24, 1., kGreen+2, kGreen+2);
        histoCorrectedYieldNorm->DrawCopy("e1,same");
		
        TLegend* legendYield3 = new TLegend(0.15,0.03,0.85,0.15);
        legendYield3->SetTextSize(0.02);
        legendYield3->SetBorderSize(0);
		legendYield3->SetFillColor(0);
        legendYield3->SetNColumns(2);
        legendYield3->AddEntry((TObject*)0,"Yields (top):","");
        legendYield3->AddEntry((TObject*)0,"Ratios (bottom):","");
        legendYield3->AddEntry(histoCorrectedYieldTrue,"corr true eff");
        legendYield3->AddEntry(RatioNormalMCInput,"corr normal eff / MC Input");

        legendYield3->AddEntry(histoMCYieldMeson,"MC input ");
        legendYield3->AddEntry(RatioNormal,"corr true eff / corr normal eff");

        legendYield3->AddEntry(histoCorrectedYieldNorm,"normal eff ");
        legendYield3->AddEntry(RatioTrueMCInput,"corr true eff / MC Input");

		legendYield3->Draw();

		padCorrectedYieldRatios->cd();
		padCorrectedYieldRatios->SetTickx();
		padCorrectedYieldRatios->SetTicky();
		padCorrectedYieldRatios->SetLogy(0);
		
        DrawGammaSetMarker(RatioNormalMCInput, 22, 1., kBlack, kBlack);
        RatioNormalMCInput->SetYTitle("#frac{standard}{modified}");
        RatioNormalMCInput->GetYaxis()->SetRangeUser(0.1,2.0);
        RatioNormalMCInput->GetYaxis()->SetLabelSize(0.07);
        RatioNormalMCInput->GetYaxis()->SetNdivisions(505);
        RatioNormalMCInput->GetYaxis()->SetTitleSize(0.1);
        RatioNormalMCInput->GetYaxis()->SetDecimals();
        RatioNormalMCInput->GetYaxis()->SetTitleOffset(0.5);
        RatioNormalMCInput->GetXaxis()->SetTitleSize(0.11);
        RatioNormalMCInput->GetXaxis()->SetLabelSize(0.08);
        RatioNormalMCInput->GetXaxis()->SetTitle("p_{T} (GeV/c)");
        RatioNormalMCInput->GetXaxis()->SetRange(0.,12.);
        RatioNormalMCInput->DrawCopy("p,e1"); // normal/ MC
        DrawGammaSetMarker(RatioNormal, 24, 1., kGreen+2, kGreen+2);
        RatioNormal->DrawCopy("e1,same"); // True / Norm
        DrawGammaSetMarker(RatioTrueMCInput, 24, 1., kRed+2, kRed+2);
        RatioTrueMCInput->DrawCopy("e1,same"); // true/ mc


		canvasCorrecftedYield->Update();
		canvasCorrecftedYield->SaveAs(Form("%s/%s_%s_CorrectedYield_SanityCheck_%s.%s",outputDir.Data(), nameMeson.Data(), prefix2.Data(),  fCutSelection.Data(), suffix.Data()));
			
		
	}

    // Plot Sanity check but now histoMCYield meson in the old pT binning will be plottet aswelll
    if (isMC == kTRUE){
        canvasCorrecftedYield->cd();

        padCorrectedYieldHistos->cd();
        padCorrectedYieldHistos->SetLogy();

        DrawGammaSetMarker(histoCorrectedYieldTrue, 22, 1., kBlack, kBlack);
        histoCorrectedYieldTrue->DrawCopy("e1");
        DrawGammaSetMarker(histoMCYieldMeson, 24, 1., kRed+2, kRed+2);
        histoMCYieldMeson->DrawCopy("e1,same");
        DrawGammaSetMarker(histoCorrectedYieldNorm, 24, 1., kGreen+2, kGreen+2);
        histoCorrectedYieldNorm->DrawCopy("e1,same");
        DrawGammaSetMarker(histoMCYieldMesonOldBin, 24, 1., kBlue+2, kBlue+2);
        histoMCYieldMesonOldBin->DrawCopy("e1,same");

        TLegend* legendYield3 = new TLegend(0.15,0.03,0.85,0.20);
        legendYield3->SetTextSize(0.02);
        legendYield3->SetBorderSize(0);
        legendYield3->SetFillColor(0);
        legendYield3->SetNColumns(2);
        legendYield3->AddEntry((TObject*)0,"Yields (top):","");
        legendYield3->AddEntry((TObject*)0,"Ratios (bottom):","");
        legendYield3->AddEntry(histoCorrectedYieldTrue,"corr true eff");
        legendYield3->AddEntry(RatioNormalMCInput,"corr normal eff / MC Input");

        legendYield3->AddEntry(histoMCYieldMeson,"MC input ");
        legendYield3->AddEntry(RatioNormal,"corr true eff / corr normal eff");

        legendYield3->AddEntry(histoCorrectedYieldNorm,"normal eff ");
        legendYield3->AddEntry(RatioTrueMCInput,"corr true eff / MC Input");

        legendYield3->AddEntry(histoMCYieldMesonOldBin,"MC Input (original pt binning)");
        legendYield3->AddEntry((TObject*)0,"","");

        legendYield3->Draw();

        padCorrectedYieldRatios->cd();
        padCorrectedYieldRatios->SetTickx();
        padCorrectedYieldRatios->SetTicky();
        padCorrectedYieldRatios->SetLogy(0);

        DrawGammaSetMarker(RatioNormalMCInput, 22, 1., kBlack, kBlack);
        RatioNormalMCInput->DrawCopy("p,e1"); // normal/ MC
        DrawGammaSetMarker(RatioNormal, 24, 1., kGreen+2, kGreen+2);
        RatioNormal->DrawCopy("e1,same"); // True / Norm
        DrawGammaSetMarker(RatioTrueMCInput, 24, 1., kRed+2, kRed+2);
        RatioTrueMCInput->DrawCopy("e1,same"); // true/ mc


        canvasCorrecftedYield->Update();
        canvasCorrecftedYield->SaveAs(Form("%s/%s_%s_CorrectedYield_SanityCheck_MCInputOldBinning_%s.%s",outputDir.Data(), nameMeson.Data(), prefix2.Data(),  fCutSelection.Data(), suffix.Data()));


    }

	delete canvasCorrecftedYield;
	delete legendYield3;

    //**********************************************************************************
    //******************** Simple Efficiency Comparison ********************************
    //**********************************************************************************
    TCanvas* canvasEffiComparison = new TCanvas("canvasEffComparison","",200,10,1350,900);  // gives the page size
    DrawGammaCanvasSettings( canvasEffiComparison, 0.13, 0.02, 0.03, 0.09);
    //       canvasEffSimple->SetLogy(1);

    DrawAutoGammaMesonHistos( histoEffiPt,
                                "", "p_{T} (GeV/c)", "Efficiency",
                                kFALSE, 1., 1e-5, kFALSE,
                                kFALSE, 0., 0.7,
                                kTRUE, 0., 25.);

    DrawGammaSetMarker(histoEffiPt, 22, 1., kBlack, kBlack);
    histoEffiPt->DrawCopy("e1");
    DrawGammaSetMarker(histoTrueEffiPt, 22, 1., kRed+2, kRed+2);
    histoTrueEffiPt->DrawCopy("e1,same");

    TLegend* legendEffiComparison= new TLegend(0.6,0.2,0.97,0.3);
    legendEffiComparison->SetTextSize(0.03);
    legendEffiComparison->SetFillColor(0);
    legendEffiComparison->SetBorderSize(0);

    legendEffiComparison->AddEntry(histoEffiPt, "Normal efficiency");
    legendEffiComparison->AddEntry(histoTrueEffiPt, "True efficiency");
    legendEffiComparison->Draw();
    canvasEffiComparison->Update();

    canvasEffiComparison->SaveAs(Form("%s/%s_TrueNormEffComparison_%s.%s",outputDir.Data(),nameMeson.Data(),fCutSelection.Data(),suffix.Data()));
    delete canvasEffiComparison;

	    //**********************************************************************************
    //******************** Simple Efficiency Comparison  back fit********************************
    //**********************************************************************************
    TCanvas* canvasEffiComparisonBackFit = new TCanvas("canvasEffComparison","",200,10,1350,900);  // gives the page size
    DrawGammaCanvasSettings( canvasEffiComparisonBackFit, 0.13, 0.02, 0.03, 0.09);
    //       canvasEffSimple->SetLogy(1);

    DrawAutoGammaMesonHistos( histoEffiPtBackFit,
                                "", "p_{T} (GeV/c)", "Efficiency",
                                kFALSE, 1., 1e-5, kFALSE,
                                kFALSE, 0., 0.7,
                                kTRUE, 0., 25.);

    DrawGammaSetMarker(histoEffiPtBackFit, 22, 1., kBlack, kBlack);
    histoEffiPtBackFit->DrawCopy("e1");
    DrawGammaSetMarker(histoTrueEffiPt, 22, 1., kRed+2, kRed+2);
    histoTrueEffiPt->DrawCopy("e1,same");

	DrawGammaSetMarker(histoEffiPtBackFitFromFit, 22, 1., kBlue, kBlue);
    histoEffiPtBackFitFromFit->DrawCopy("e1,same");
	
	DrawGammaSetMarker(histoEffiWeightedAverage, 22, 1., kGreen, kGreen);
    histoEffiWeightedAverage->DrawCopy("e1,same");

    TLegend* legendEffiComparisonBackFit= new TLegend(0.6,0.2,0.97,0.3);
    legendEffiComparisonBackFit->SetTextSize(0.03);
    legendEffiComparisonBackFit->SetFillColor(0);
    legendEffiComparisonBackFit->SetBorderSize(0);

    legendEffiComparisonBackFit->AddEntry(histoEffiPtBackFit, "Normal back fit efficiency");
    legendEffiComparisonBackFit->AddEntry(histoTrueEffiPt, "True efficiency");
    legendEffiComparisonBackFit->AddEntry(histoEffiPtBackFitFromFit, "back fit efficiency from fit");
    legendEffiComparisonBackFit->AddEntry(histoEffiWeightedAverage, "average");
    legendEffiComparisonBackFit->Draw();
    canvasEffiComparisonBackFit->Update();

    canvasEffiComparisonBackFit->SaveAs(Form("%s/%s_TrueNormEffComparisonBackFit_%s.%s",outputDir.Data(),nameMeson.Data(),fCutSelection.Data(),suffix.Data()));
    delete canvasEffiComparisonBackFit;

	//***********************************************************************************************
	//***************************  Secondary RAW Yield  *********************************************
	//***********************************************************************************************
	if (!optDalitz && doubleAddFactorK0s >= 0){
		TCanvas* canvasRAWYieldSec = new TCanvas("canvasRAWYieldSec","",200,10,1350,900);  // gives the page size
		DrawGammaCanvasSettings( canvasRAWYieldSec, 0.13, 0.02, 0.02, 0.09); 
		canvasRAWYieldSec->SetLogy(1);         

		DrawGammaSetMarker(histoUnCorrectedYieldDrawing, 20, 1., kBlack, kBlack);                              
		histoUnCorrectedYieldDrawing->Draw("e1");

		TLegend* legendSecRAWYield = new TLegend(0.6,0.8,0.97,0.95);
		legendSecRAWYield->SetTextSize(0.03);        
		legendSecRAWYield->SetFillColor(0);
		legendSecRAWYield->SetBorderSize(0);
		legendSecRAWYield->AddEntry(histoUnCorrectedYieldDrawing,"RAW yield");
		legendSecRAWYield->Draw();

		canvasRAWYieldSec->Update();
		canvasRAWYieldSec->SaveAs(Form("%s/%s_%s_RAWYieldSecPt_%s.%s",outputDir.Data(),nameMeson.Data(),prefix2.Data(),fCutSelection.Data(),suffix.Data()));
		delete canvasRAWYieldSec;
	} else if (optDalitz){
		TCanvas* canvasRAWYieldSec = new TCanvas("canvasRAWYieldSec","",200,10,1350,900);  // gives the page size
		DrawGammaCanvasSettings( canvasRAWYieldSec, 0.13, 0.02, 0.02, 0.09); 
		canvasRAWYieldSec->SetLogy(1);         

		DrawGammaSetMarker(histoUnCorrectedYieldDrawing, 20, 1., kBlack, kBlack);                              
		histoUnCorrectedYieldDrawing->Draw("e1");
		histoYieldGGMeson->Scale(1./nEvt);
		DrawGammaSetMarker(histoYieldGGMeson, 22, 1., kBlue, kBlue);                               
		histoYieldGGMeson->DrawCopy("same,e1");   
			
		TLegend* legendSecRAWYield = new TLegend(0.6,0.8,0.97,0.95);
		legendSecRAWYield->SetTextSize(0.03);        
		legendSecRAWYield->SetFillColor(0);
		legendSecRAWYield->SetBorderSize(0);
		legendSecRAWYield->AddEntry(histoUnCorrectedYieldDrawing,"RAW yield");
		legendSecRAWYield->AddEntry(histoYieldGGMeson,"Contamination from #pi^{+}#pi^{-}#pi^{0}");
		legendSecRAWYield->Draw();

		canvasRAWYieldSec->Update();
		canvasRAWYieldSec->SaveAs(Form("%s/%s_%s_RAWYieldContGGPt_%s.%s",outputDir.Data(),nameMeson.Data(),prefix2.Data(),fCutSelection.Data(),suffix.Data()));
		delete canvasRAWYieldSec;
	}

	Int_t nBinsPt =   histoCorrectedYieldTrue->GetNbinsX();

	

	if (!kDalitz){
		//***********************************************************************************************
		//***************************  correction for yield in mt bins **********************************
		//***********************************************************************************************
		

		Double_t * binsMt = NULL;
		binsMt=        new Double_t[nBinsPt+1];
		binsMt[0] =       mesonMassExpect;


		for(Int_t iPt=1;iPt<nBinsPt+1;iPt++){
            binsMt[iPt]=TMath::Power((histoCorrectedYieldTrue->GetXaxis()->GetBinUpEdge(iPt)*histoCorrectedYieldTrue->GetXaxis()->GetBinUpEdge(iPt)+mesonMassExpect*mesonMassExpect),0.5);
			cout << "recalculation pt to mt:    " << iPt<<"     pt      "<< histoCorrectedYieldTrue->GetXaxis()->GetBinUpEdge(iPt)<< "      mt    " << binsMt[iPt]<< endl;
		}

			
		//***********************************************************************************************
		//***************************  correction for yield in Xt bins **********************************
		//***********************************************************************************************
		Double_t * binsXt = NULL;
		binsXt=  new Double_t[nBinsPt+1];
			
		for(Int_t iPt=0;iPt<nBinsPt+1;iPt++){
			binsXt[iPt]=2*histoCorrectedYieldTrue->GetXaxis()->GetBinUpEdge(iPt)/energy;
	//          cout << "recalculation pt to xt:    " << iPt<<"     pt      "<< histoCorrectedYieldTrue->GetXaxis()->GetBinUpEdge(iPt)<< "      xt    " << binsXt[iPt]<< endl;
		}
	}
	//*************************************************************************************************
	//******************** Output of the systematic Error due to Signal extraction ********************
	//*************************************************************************************************
	Double_t  binsXCenter[50];
	Double_t  binsXWidth[50];

    binsXWidth[0]=       0.;

    for (Int_t i = 1; i < nBinsPt +1; i++){
		if(yieldExtractionMode==0){
			binsXCenter[i] =  histoCorrectedYieldAveragedEffiBackFit->GetBinCenter(i);
			binsXWidth[i]=    histoCorrectedYieldAveragedEffiBackFit->GetBinWidth(i)/2.;
		} else if (yieldExtractionMode==1){
			binsXCenter[i] =  histoCorrectedYieldNormBackFit->GetBinCenter(i);
			binsXWidth[i]=    histoCorrectedYieldNormBackFit->GetBinWidth(i)/2.;
		} else if (yieldExtractionMode==2){
			binsXCenter[i] =  histoCorrectedYieldTrueBackFit->GetBinCenter(i);
			binsXWidth[i]=    histoCorrectedYieldTrueBackFit->GetBinWidth(i)/2.;
		} else{
			cout << "Invalid mode for sys calculation of yield extraction was used !" << endl;
			break;
		}
	}


	SysErrorConversion sysErrNormal[50];
	SysErrorConversion sysErrLeft[50];
	SysErrorConversion sysErrWide[50];
	SysErrorConversion sysErrNarrow[50];
	SysErrorConversion sysErrLeftWide[50];
	SysErrorConversion sysErrLeftNarrow[50];

	for (Int_t i = 1; i < nBinsPt +1; i++){
		if(yieldExtractionMode==0){
            sysErrNormal[i].value = histoCorrectedYieldAveragedEffiBackFit->GetBinContent(i);
			sysErrNormal[i].error = histoCorrectedYieldAveragedEffiBackFit->GetBinError(i);
			//sysErrLeft[i].value = histoCorrectedYieldTrueLeft->GetBinContent(i);
			//sysErrLeft[i].error = histoCorrectedYieldTrueLeft->GetBinError(i);
			sysErrNarrow[i].value = histoCorrectedYieldAveragedEffiNarrowBackFit->GetBinContent(i);
			sysErrNarrow[i].error = histoCorrectedYieldAveragedEffiNarrowBackFit->GetBinError(i);
			//sysErrLeftNarrow[i].value = histoCorrectedYieldTrueLeftNarrow->GetBinContent(i);
			//sysErrLeftNarrow[i].error = histoCorrectedYieldTrueLeftNarrow->GetBinError(i);
			sysErrWide[i].value = histoCorrectedYieldAveragedEffiWideBackFit->GetBinContent(i);
			sysErrWide[i].error = histoCorrectedYieldAveragedEffiWideBackFit->GetBinError(i);
			//sysErrLeftWide[i].value = histoCorrectedYieldTrueLeftWide->GetBinContent(i);
			//sysErrLeftWide[i].error = histoCorrectedYieldTrueLeftWide->GetBinError(i); 
		} else if(yieldExtractionMode==1){
            sysErrNormal[i].value = histoCorrectedYieldNormBackFit->GetBinContent(i);
			sysErrNormal[i].error = histoCorrectedYieldNormBackFit->GetBinError(i);
			//sysErrLeft[i].value = histoCorrectedYieldTrueLeft->GetBinContent(i);
			//sysErrLeft[i].error = histoCorrectedYieldTrueLeft->GetBinError(i);
			sysErrNarrow[i].value = histoCorrectedYieldNormNarrowBackFit->GetBinContent(i);
			sysErrNarrow[i].error = histoCorrectedYieldNormNarrowBackFit->GetBinError(i);
			//sysErrLeftNarrow[i].value = histoCorrectedYieldTrueLeftNarrow->GetBinContent(i);
			//sysErrLeftNarrow[i].error = histoCorrectedYieldTrueLeftNarrow->GetBinError(i);
			sysErrWide[i].value = histoCorrectedYieldNormWideBackFit->GetBinContent(i);
			sysErrWide[i].error = histoCorrectedYieldNormWideBackFit->GetBinError(i);
			//sysErrLeftWide[i].value = histoCorrectedYieldTrueLeftWide->GetBinContent(i);
			//sysErrLeftWide[i].error = histoCorrectedYieldTrueLeftWide->GetBinError(i); 
		} else if(yieldExtractionMode==2){
            sysErrNormal[i].value = histoCorrectedYieldTrueBackFit->GetBinContent(i);
			sysErrNormal[i].error = histoCorrectedYieldTrueBackFit->GetBinError(i);
			//sysErrLeft[i].value = histoCorrectedYieldTrueLeft->GetBinContent(i);
			//sysErrLeft[i].error = histoCorrectedYieldTrueLeft->GetBinError(i);
			sysErrNarrow[i].value = histoCorrectedYieldTrueNarrowBackFit->GetBinContent(i);
			sysErrNarrow[i].error = histoCorrectedYieldTrueNarrowBackFit->GetBinError(i);
			//sysErrLeftNarrow[i].value = histoCorrectedYieldTrueLeftNarrow->GetBinContent(i);
			//sysErrLeftNarrow[i].error = histoCorrectedYieldTrueLeftNarrow->GetBinError(i);
			sysErrWide[i].value = histoCorrectedYieldTrueWideBackFit->GetBinContent(i);
			sysErrWide[i].error = histoCorrectedYieldTrueWideBackFit->GetBinError(i);
			//sysErrLeftWide[i].value = histoCorrectedYieldTrueLeftWide->GetBinContent(i);
			//sysErrLeftWide[i].error = histoCorrectedYieldTrueLeftWide->GetBinError(i); 
		}
	}
	Double_t differenceLeft[50];
	Double_t differenceLeftWide[50];
	Double_t differenceLeftNarrow[50];
	Double_t differenceWide[50];
	Double_t differenceNarrow[50];
	Double_t largestDifferenceNeg[50];
	Double_t largestDifferencePos[50];
	Double_t differenceLeftError[50];
	Double_t differenceLeftWideError[50];
	Double_t differenceLeftNarrowError[50];
	Double_t differenceWideError[50];
	Double_t differenceNarrowError[50];
	Double_t largestDifferenceNegError[50];
	Double_t largestDifferencePosError[50];
	Double_t relDifferenceLeft[50];
	Double_t relDifferenceLeftWide[50];
	Double_t relDifferenceLeftNarrow[50];
	Double_t relDifferenceWide[50];
	Double_t relDifferenceNarrow[50];
	Double_t relLargestDifferenceNeg[50];
	Double_t relLargestDifferencePos[50];
	Double_t relDifferenceLeftError[50];
	Double_t relDifferenceLeftWideError[50];
	Double_t relDifferenceLeftNarrowError[50];
	Double_t relDifferenceWideError[50];
	Double_t relDifferenceNarrowError[50];
	Double_t relLargestDifferenceNegError[50];
	Double_t relLargestDifferencePosError[50];
	for (Int_t i = 1; i < nBinsPt +1; i++){
		largestDifferenceNeg[i] =     0;
		largestDifferencePos[i] =     0;
		largestDifferenceNegError[i] =   0;
		largestDifferencePosError[i] =   0;
		relLargestDifferenceNeg[i] =     0;
		relLargestDifferencePos[i] =     0;
		relLargestDifferenceNegError[i] =   0;
		relLargestDifferencePosError[i] =   0;

        //Calculate differences
		// differenceLeft[i] =        sysErrLeft[i].value - sysErrNormal[i].value;
		// differenceLeftError[i] =      TMath::Sqrt(TMath::Abs(TMath::Power(sysErrLeft[i].error,2)-TMath::Power(sysErrNormal[i].error,2)));
		// differenceLeftNarrow[i] =     sysErrLeftNarrow[i].value - sysErrNormal[i].value;
		// differenceLeftNarrowError[i] =   TMath::Sqrt(TMath::Abs(TMath::Power(sysErrLeftNarrow[i].error,2)-TMath::Power(sysErrNormal[i].error,2)));
		// differenceLeftWide[i] =          sysErrLeftWide[i].value - sysErrNormal[i].value;
		// differenceLeftWideError[i] =     TMath::Sqrt(TMath::Abs(TMath::Power(sysErrLeftWide[i].error,2)-TMath::Power(sysErrNormal[i].error,2)));
		differenceNarrow[i] =         sysErrNarrow[i].value - sysErrNormal[i].value;
		differenceNarrowError[i] =       TMath::Sqrt(TMath::Abs(TMath::Power(sysErrNarrow[i].error,2)-TMath::Power(sysErrNormal[i].error,2)));
		differenceWide[i] =        sysErrWide[i].value - sysErrNormal[i].value;
		differenceWideError[i] =      TMath::Sqrt(TMath::Abs(TMath::Power(sysErrWide[i].error,2)-TMath::Power(sysErrNormal[i].error,2)));

		if (sysErrNormal[i].value != 0){
			// relDifferenceLeft[i] =     (differenceLeft[i]/sysErrNormal[i].value)*100.;
			// relDifferenceLeftError[i] =   (differenceLeftError[i]/sysErrNormal[i].value)*100.;
			// relDifferenceLeftNarrow[i] =  (differenceLeftNarrow[i]/sysErrNormal[i].value)*100.;
			// relDifferenceLeftNarrowError[i] = (differenceLeftNarrowError[i]/sysErrNormal[i].value)*100.;
			// relDifferenceLeftWide[i] =    (differenceLeftWide[i]/sysErrNormal[i].value)*100.;
			// relDifferenceLeftWideError[i] = (differenceLeftWideError[i]/sysErrNormal[i].value)*100.;
			relDifferenceNarrow[i] =   (differenceNarrow[i]/sysErrNormal[i].value)*100.;
			relDifferenceNarrowError[i] = (differenceNarrowError[i]/sysErrNormal[i].value)*100.;
			relDifferenceWide[i] =     (differenceWide[i]/sysErrNormal[i].value)*100.;
			relDifferenceWideError[i] =   (differenceWideError[i]/sysErrNormal[i].value)*100.;
		} else {
			// relDifferenceLeft[i] =     0.;
			// relDifferenceLeftError[i] =   0.;
			// relDifferenceLeftNarrow[i] =  0.;
			// relDifferenceLeftNarrowError[i] = 0.;
			// relDifferenceLeftWide[i] =    0.;
			// relDifferenceLeftWideError[i] = 0.;
			relDifferenceNarrow[i] =      0.;
			relDifferenceNarrowError[i] = 0.;
			relDifferenceWide[i] =     0.;
			relDifferenceWideError[i] =   0.;
		}

		// //Find biggest Deviation
        // if (TMath::Abs(relDifferenceLeft[i]) < 75. ){
        //     if(differenceLeft[i] < 0){
        //         largestDifferenceNeg[i] =     differenceLeft[i];
        //         largestDifferenceNegError[i] =   differenceLeftError[i];
        //         relLargestDifferenceNeg[i] =     relDifferenceLeft[i];
        //         relLargestDifferenceNegError[i] =   relDifferenceLeftError[i];
        //     }else{
        //         largestDifferencePos[i] =     differenceLeft[i];
        //         largestDifferencePosError[i] =   differenceLeftError[i];
        //         relLargestDifferencePos[i] =     relDifferenceLeft[i];
        //         relLargestDifferencePosError[i] =   relDifferenceLeftError[i];
        //     }
        // }
		if (TMath::Abs(relDifferenceNarrow[i]) < 75.){
			if(differenceNarrow[i] < 0){
				if(differenceNarrow[i] < largestDifferenceNeg[i]){
				largestDifferenceNeg[i] =     differenceNarrow[i]; 
				largestDifferenceNegError[i] =   differenceNarrowError[i];  
				relLargestDifferenceNeg[i] =     relDifferenceNarrow[i]; 
				relLargestDifferenceNegError[i] =   relDifferenceNarrowError[i];  
				}
			} else {
				if(differenceNarrow[i] > largestDifferencePos[i]){
				largestDifferencePos[i] =     differenceNarrow[i];
				largestDifferencePosError[i] =   differenceNarrowError[i];
				relLargestDifferencePos[i] =     relDifferenceNarrow[i];
				relLargestDifferencePosError[i] =   relDifferenceNarrowError[i];
				}
			}  
		}
		if (TMath::Abs(relDifferenceWide[i]) < 75.){
			if(differenceWide[i] < 0){
				if(differenceWide[i] < largestDifferenceNeg[i]){
				largestDifferenceNeg[i] =     differenceWide[i];
				largestDifferenceNegError[i] =   differenceWideError[i];
				relLargestDifferenceNeg[i] =     relDifferenceWide[i];
				relLargestDifferenceNegError[i] =   relDifferenceWideError[i];
				}
			} else {
				if(differenceWide[i] > largestDifferencePos[i]) {
				largestDifferencePos[i] =     differenceWide[i];   
				largestDifferencePosError[i] =   differenceWideError[i]; 
				relLargestDifferencePos[i] =     relDifferenceWide[i];   
				relLargestDifferencePosError[i] =   relDifferenceWideError[i]; 
				}
			}
        }
	}  

	cout << "done filling" << endl;
	const char *nameFileSysErrDat = Form("%s/%s/%s_%s_SystematicErrorYieldExtraction_%s.dat",fCutSelection.Data(),optionEnergy.Data() ,nameMeson.Data(),prefix2.Data(),fCutSelection.Data());
	fstream fileSysErrDat;
	fileSysErrDat.open(nameFileSysErrDat, ios::out);
	fileSysErrDat << "Calculation of the systematic error due to the yield extraction" << endl;
	fileSysErrDat << "Bin" << "\t" << "Normal value" << "\t" << "Normal error" << endl;
	for(Int_t i = 1; i < (nBinsPt +1); i++){
		fileSysErrDat << i << "\t" << sysErrNormal[i].value << "\t" << sysErrNormal[i].error << endl;
	}
	fileSysErrDat << endl;
	fileSysErrDat << "Bin" << "\t" << "Narrow value" << "\t" << "Narrow error" << "\t" << "Diff to Norm" << endl;
	for(Int_t i = 1; i < (nBinsPt +1); i++){
		fileSysErrDat << i << "\t" << sysErrNarrow[i].value << "\t" << sysErrNarrow[i].error << "\t" << differenceNarrow[i] << "\t" << differenceNarrowError[i] << "\t" << relDifferenceNarrow[i] << "\t" << relDifferenceNarrowError[i] <<endl;
	}
	fileSysErrDat << endl;
	fileSysErrDat << "Bin" << "\t" << "Wide value" << "\t" << "Wide error" << "\t" << "Diff to Norm" << endl;
	for(Int_t i = 1; i < (nBinsPt +1); i++){
		fileSysErrDat << i << "\t" << sysErrWide[i].value << "\t" << sysErrWide[i].error << "\t" << differenceWide[i] << "\t" << differenceWideError[i] << "\t" << relDifferenceWide[i] << "\t" << relDifferenceWideError[i] <<endl;
	}
	// fileSysErrDat << endl;
	// fileSysErrDat << "Bin" << "\t" << "Left value" << "\t" << "Left error" << "\t" << "Diff to Norm" << endl;
	// for(Int_t i = 1; i < (nBinsPt +1); i++){
	// 	fileSysErrDat << i << "\t" << sysErrLeft[i].value << "\t" << sysErrLeft[i].error << "\t" << differenceLeft[i] << "\t" << differenceLeftError[i] << "\t" << relDifferenceLeft[i] << "\t" << relDifferenceLeftError[i] <<endl;
	// }
	// fileSysErrDat << endl;
	// fileSysErrDat << "Bin" << "\t" << "Left Narrow value" << "\t" << "Left Narrow error" << "\t" << "Diff to Norm" << endl;
	// for(Int_t i = 1; i < (nBinsPt +1); i++){
	// 	fileSysErrDat << i << "\t" << sysErrLeftNarrow[i].value << "\t" << sysErrLeftNarrow[i].error << "\t" << differenceLeftNarrow[i] <<  "\t" << differenceLeftNarrowError[i] << "\t" << relDifferenceLeftNarrow[i] << "\t" << relDifferenceLeftNarrowError[i] <<endl;
	// }
	// fileSysErrDat << endl;
	// fileSysErrDat << "Bin" << "\t" << "Left Wide value" << "\t" << "Left Wide error" << "\t" << "Diff to Norm" << endl;
	// for(Int_t i = 1; i < (nBinsPt +1); i++){
	// 	fileSysErrDat << i << "\t" << sysErrLeftWide[i].value << "\t" << sysErrLeftWide[i].error << "\t" << differenceLeftWide[i] << "\t" << differenceLeftWideError[i] << "\t" << relDifferenceLeftWide[i] << "\t" << relDifferenceLeftWideError[i] <<endl;
	// }
	fileSysErrDat << endl;

	fileSysErrDat << "Bin" << "\t" << "Largest Dev Neg" << "\t" << "Largest Dev Pos"  << endl;
	for(Int_t i = 1; i < (nBinsPt +1); i++){
		fileSysErrDat << i << "\t" << largestDifferenceNeg[i] <<  "\t" << largestDifferenceNegError[i] << "\t" << largestDifferencePos[i] << "\t" << largestDifferencePosError[i]<<endl;
	}
	fileSysErrDat << "Bin" << "\t" << "Largest Rel Dev Neg" << "\t" << "Largest Rel Dev Pos"  << endl;
	for(Int_t i = 1; i < (nBinsPt +1); i++){
		fileSysErrDat << i << "\t" << relLargestDifferenceNeg[i] <<  "\t" << relLargestDifferenceNegError[i] << "\t" << relLargestDifferencePos[i] << "\t" << relLargestDifferencePosError[i]<<endl;
	}

	fileSysErrDat.close();
	
	TGraphAsymmErrors* SystErrGraphNeg = new TGraphAsymmErrors(nBinsPt+1, binsXCenter, relLargestDifferenceNeg, binsXWidth, binsXWidth, relLargestDifferenceNegError, relLargestDifferenceNegError);
	TGraphAsymmErrors* SystErrGraphPos = new TGraphAsymmErrors(nBinsPt+1, binsXCenter, relLargestDifferencePos, binsXWidth, binsXWidth, relLargestDifferencePosError, relLargestDifferencePosError);

	//*************************************************************************************************
	//******************** Output of the systematic Error due to reconstruction efficiency ************
	//*************************************************************************************************
	Double_t  binsXCenterEffi[50];
	Double_t  binsXWidthEffi[50];

    binsXWidthEffi[0]=       0.;

    for (Int_t i = 1; i < nBinsPt +1; i++){
		binsXCenterEffi[i] =  histoCorrectedYieldAveragedEffiBackFit->GetBinCenter(i);
		binsXWidthEffi[i]=    histoCorrectedYieldAveragedEffiBackFit->GetBinWidth(i)/2.;
	}


	SysErrorConversion sysErrAverageEffi[50];
	SysErrorConversion sysErrTrueEffi[50];
	SysErrorConversion sysErrNormEffi[50];
	SysErrorConversion sysErrNormFromFit[50];

	for (Int_t i = 1; i < nBinsPt +1; i++){
        sysErrAverageEffi[i].value = histoCorrectedYieldAveragedEffiBackFit->GetBinContent(i);
		sysErrAverageEffi[i].error = histoCorrectedYieldAveragedEffiBackFit->GetBinError(i);
		sysErrNormEffi[i].value = histoCorrectedYieldNormBackFit->GetBinContent(i);
		sysErrNormEffi[i].error = histoCorrectedYieldNormBackFit->GetBinError(i);
		sysErrTrueEffi[i].value = histoCorrectedYieldTrueBackFit->GetBinContent(i);
		sysErrTrueEffi[i].error = histoCorrectedYieldTrueBackFit->GetBinError(i);
		sysErrNormFromFit[i].value = histoCorrectedYieldNormBackFitFromFit->GetBinContent(i);
		sysErrNormFromFit[i].error = histoCorrectedYieldNormBackFitFromFit->GetBinError(i); 
	}
	Double_t differenceNormFromFit[50];
	Double_t differenceTrue[50];
	Double_t differenceNorm[50];
	Double_t largestDifferenceNegEffi[50];
	Double_t largestDifferencePosEffi[50];
	Double_t differenceNormFromFitError[50];
	Double_t differenceTrueError[50];
	Double_t differenceNormError[50];
	Double_t largestDifferenceNegEffiError[50];
	Double_t largestDifferencePosEffiError[50];
	Double_t relDifferenceNormFromFit[50];
	Double_t relDifferenceTrue[50];
	Double_t relDifferenceNorm[50];
	Double_t rellargestDifferenceNegEffi[50];
	Double_t rellargestDifferencePosEffi[50];
	Double_t relDifferenceNormFromFitError[50];
	Double_t relDifferenceTrueError[50];
	Double_t relDifferenceNormError[50];
	Double_t rellargestDifferenceNegEffiError[50];
	Double_t rellargestDifferencePosEffiError[50];
	for (Int_t i = 1; i < nBinsPt +1; i++){
		largestDifferenceNegEffi[i] =     0;
		largestDifferencePosEffi[i] =     0;
		largestDifferenceNegEffiError[i] =   0;
		largestDifferencePosEffiError[i] =   0;
		rellargestDifferenceNegEffi[i] =     0;
		rellargestDifferencePosEffi[i] =     0;
		rellargestDifferenceNegEffiError[i] =   0;
		rellargestDifferencePosEffiError[i] =   0;

        //Calculate differences
		differenceNormFromFit[i] =          sysErrNormFromFit[i].value - sysErrAverageEffi[i].value;
		differenceNormFromFitError[i] =     TMath::Sqrt(TMath::Abs(TMath::Power(sysErrNormFromFit[i].error,2)-TMath::Power(sysErrAverageEffi[i].error,2)));
		differenceNorm[i] =         sysErrNormEffi[i].value - sysErrAverageEffi[i].value;
		differenceNormError[i] =       TMath::Sqrt(TMath::Abs(TMath::Power(sysErrNormEffi[i].error,2)-TMath::Power(sysErrAverageEffi[i].error,2)));
		differenceTrue[i] =        sysErrTrueEffi[i].value - sysErrAverageEffi[i].value;
		differenceTrueError[i] =      TMath::Sqrt(TMath::Abs(TMath::Power(sysErrTrueEffi[i].error,2)-TMath::Power(sysErrAverageEffi[i].error,2)));

		if (sysErrAverageEffi[i].value != 0){
			relDifferenceNormFromFit[i] =    (differenceNormFromFit[i]/sysErrAverageEffi[i].value)*100.;
			relDifferenceNormFromFitError[i] = (differenceNormFromFitError[i]/sysErrAverageEffi[i].value)*100.;
			relDifferenceNorm[i] =   (differenceNorm[i]/sysErrAverageEffi[i].value)*100.;
			relDifferenceNormError[i] = (differenceNormError[i]/sysErrAverageEffi[i].value)*100.;
			relDifferenceTrue[i] =     (differenceTrue[i]/sysErrAverageEffi[i].value)*100.;
			relDifferenceTrueError[i] =   (differenceTrueError[i]/sysErrAverageEffi[i].value)*100.;
		} else {
			relDifferenceNormFromFit[i] =    0.;
			relDifferenceNormFromFitError[i] = 0.;
			relDifferenceNorm[i] =      0.;
			relDifferenceNormError[i] = 0.;
			relDifferenceTrue[i] =     0.;
			relDifferenceTrueError[i] =   0.;
		}

		if (TMath::Abs(relDifferenceNorm[i]) < 75.){
			if(differenceNorm[i] < 0){
				if(differenceNorm[i] < largestDifferenceNegEffi[i]){
				largestDifferenceNegEffi[i] =     differenceNorm[i]; 
				largestDifferenceNegEffiError[i] =   differenceNormError[i];  
				rellargestDifferenceNegEffi[i] =     relDifferenceNorm[i]; 
				rellargestDifferenceNegEffiError[i] =   relDifferenceNormError[i];  
				}
			} else {
				if(differenceNorm[i] > largestDifferencePosEffi[i]){
				largestDifferencePosEffi[i] =     differenceNorm[i];
				largestDifferencePosEffiError[i] =   differenceNormError[i];
				rellargestDifferencePosEffi[i] =     relDifferenceNorm[i];
				rellargestDifferencePosEffiError[i] =   relDifferenceNormError[i];
				}
			}  
		}
		if (TMath::Abs(relDifferenceTrue[i]) < 75.){
			if(differenceTrue[i] < 0){
				if(differenceTrue[i] < largestDifferenceNegEffi[i]){
				largestDifferenceNegEffi[i] =     differenceTrue[i];
				largestDifferenceNegEffiError[i] =   differenceTrueError[i];
				rellargestDifferenceNegEffi[i] =     relDifferenceTrue[i];
				rellargestDifferenceNegEffiError[i] =   relDifferenceTrueError[i];
				}
			} else {
				if(differenceTrue[i] > largestDifferencePosEffi[i]) {
				largestDifferencePosEffi[i] =     differenceTrue[i];   
				largestDifferencePosEffiError[i] =   differenceTrueError[i]; 
				rellargestDifferencePosEffi[i] =     relDifferenceTrue[i];   
				rellargestDifferencePosEffiError[i] =   relDifferenceTrueError[i]; 
				}
			}
        }
	}  

	cout << "done filling" << endl;
	const char *nameFileSysErrDatEffi = Form("%s/%s/%s_%s_SystematicErrorEfficiency_%s.dat",fCutSelection.Data(),optionEnergy.Data() ,nameMeson.Data(),prefix2.Data(),fCutSelection.Data());
	fstream fileSysErrDatEffi;
	fileSysErrDatEffi.open(nameFileSysErrDatEffi, ios::out);
	fileSysErrDatEffi << "Calculation of the systematic error due to the reco efficiency" << endl;
	fileSysErrDatEffi << "Bin" << "\t" << "average reco value" << "\t" << " error" << endl;
	for(Int_t i = 1; i < (nBinsPt +1); i++){
		fileSysErrDatEffi << i << "\t" << sysErrAverageEffi[i].value << "\t" << sysErrAverageEffi[i].error << endl;
	}
	fileSysErrDatEffi << endl;
	fileSysErrDatEffi << "Bin" << "\t" << "Norm value" << "\t" << "Norm error" << "\t" << "diff to average" << endl;
	for(Int_t i = 1; i < (nBinsPt +1); i++){
		fileSysErrDatEffi << i << "\t" << sysErrNormEffi[i].value << "\t" << sysErrNormEffi[i].error << "\t" << differenceNorm[i] << "\t" << differenceNormError[i] << "\t" << relDifferenceNorm[i] << "\t" << relDifferenceNormError[i] <<endl;
	}
	fileSysErrDatEffi << endl;
	fileSysErrDatEffi << "Bin" << "\t" << "True value" << "\t" << "True error" << "\t" << "diff to average" << endl;
	for(Int_t i = 1; i < (nBinsPt +1); i++){
		fileSysErrDatEffi << i << "\t" << sysErrTrueEffi[i].value << "\t" << sysErrTrueEffi[i].error << "\t" << differenceTrue[i] << "\t" << differenceTrueError[i] << "\t" << relDifferenceTrue[i] << "\t" << relDifferenceTrueError[i] <<endl;
	}
	fileSysErrDatEffi << endl;
	fileSysErrDatEffi << "Bin" << "\t" << "norm from fit value" << "\t" << "error" << "\t" << "diff to average" << endl;
	for(Int_t i = 1; i < (nBinsPt +1); i++){
		fileSysErrDatEffi << i << "\t" << sysErrNormFromFit[i].value << "\t" << sysErrNormFromFit[i].error << "\t" << differenceNormFromFit[i] << "\t" << differenceNormFromFitError[i] << "\t" << relDifferenceNormFromFit[i] << "\t" << relDifferenceNormFromFitError[i] <<endl;
	}
	fileSysErrDatEffi << endl;

	fileSysErrDatEffi << "Bin" << "\t" << "Largest Dev Neg" << "\t" << "Largest Dev Pos"  << endl;
	for(Int_t i = 1; i < (nBinsPt +1); i++){
		fileSysErrDatEffi << i << "\t" << largestDifferenceNegEffi[i] <<  "\t" << largestDifferenceNegEffiError[i] << "\t" << largestDifferencePosEffi[i] << "\t" << largestDifferencePosEffiError[i]<<endl;
	}
	fileSysErrDatEffi << "Bin" << "\t" << "Largest Rel Dev Neg" << "\t" << "Largest Rel Dev Pos"  << endl;
	for(Int_t i = 1; i < (nBinsPt +1); i++){
		fileSysErrDatEffi << i << "\t" << rellargestDifferenceNegEffi[i] <<  "\t" << rellargestDifferenceNegEffiError[i] << "\t" << rellargestDifferencePosEffi[i] << "\t" << rellargestDifferencePosEffiError[i]<<endl;
	}

	fileSysErrDatEffi.close();
	
	TGraphAsymmErrors* SystErrGraphNegEffi = new TGraphAsymmErrors(nBinsPt+1, binsXCenterEffi, rellargestDifferenceNegEffi, binsXWidthEffi, binsXWidthEffi, rellargestDifferenceNegEffiError, rellargestDifferenceNegEffiError);
	TGraphAsymmErrors* SystErrGraphPosEffi = new TGraphAsymmErrors(nBinsPt+1, binsXCenterEffi, rellargestDifferencePosEffi, binsXWidthEffi, binsXWidthEffi, rellargestDifferencePosEffiError, rellargestDifferencePosEffiError);

	const char* nameOutput2 = Form("%s/%s/%s_%s_GammaConv_OnlyCorrectionFactor%s_%s.root",fCutSelection.Data(),optionEnergy.Data(),nameMeson.Data(),prefix2.Data(),optionPeriod.Data(),fCutSelection.Data());
	TFile* correctedOutput2 = new TFile(nameOutput2,"RECREATE");     
		histoAcceptance->Write();
        histoTrueEffiPt->Write(Form("TrueMesonEffiPt%s",InvMassTypeEnding.Data()));
        histoEffiPt->Write(Form("MesonEffiPtNorm%s",InvMassTypeEnding.Data()));
        histoEffiPtBackFit->Write(Form("MesonEffiPtNormBackFit%s",InvMassTypeEnding.Data()));
        histoEffiPtBackFitFromFit->Write(Form("MesonEffiPtNormBackFitFromFit%s",InvMassTypeEnding.Data()));
        histoEffiWeightedAverage->Write(Form("MesonEffiPtAveraged%s",InvMassTypeEnding.Data()));
        histoCompleteCorr->Write(Form("EffiTimesAcceptanceTimesDeltaY%s",InvMassTypeEnding.Data()));
	correctedOutput2->Write();
	correctedOutput2->Close();
	
	
	const char* nameOutput = Form("%s/%s/%s_%s_GammaConvV1%sCorrection%s_%s.root",fCutSelection.Data(),optionEnergy.Data(),nameMeson.Data(),prefix2.Data(),fDalitz.Data(),optionPeriod.Data(),fCutSelection.Data());
	TFile* correctedOutput = new TFile(nameOutput,"RECREATE");     

	histoCorrectedYieldNorm->Write();
	histoCorrectedYieldNormWide->Write();
	histoCorrectedYieldNormNarrow->Write();

	histoCorrectedYieldNormBackFitFromFit->Write();
	histoCorrectedYieldNormWideBackFitFromFit->Write();
	histoCorrectedYieldNormNarrowBackFitFromFit->Write();

	histoTrueEffiPt->Write(Form("TrueMesonEffiPt%s",InvMassTypeEnding.Data()));
    histoEffiPt->Write(Form("MesonEffiPtNorm%s",InvMassTypeEnding.Data()));
    histoEffiPtBackFit->Write(Form("MesonEffiPtNormBackFit%s",InvMassTypeEnding.Data()));
    histoEffiPtBackFitFromFit->Write(Form("MesonEffiPtNormBackFitFromFit%s",InvMassTypeEnding.Data()));
    histoEffiWeightedAverage->Write(Form("MesonEffiPtAveraged%s",InvMassTypeEnding.Data()));

	histoCorrectedYieldAveragedEffiBackFit->Write();
	histoCorrectedYieldAveragedEffiWideBackFit->Write();
	histoCorrectedYieldAveragedEffiNarrowBackFit->Write();

	histoCorrectedYieldAveragedEffi->Write();
	histoCorrectedYieldAveragedEffiWide->Write();
	histoCorrectedYieldAveragedEffiNarrow->Write();
	SystErrGraphPos->Write(Form("%s_SystErrorRelPos_YieldExtraction_%s",nameMeson.Data(),centralityString.Data()),TObject::kOverwrite);
	SystErrGraphNeg->Write(Form("%s_SystErrorRelNeg_YieldExtraction_%s",nameMeson.Data(),centralityString.Data()),TObject::kOverwrite);

	SystErrGraphPosEffi->Write(Form("%s_SystErrorRelPos_Efficiency_%s",nameMeson.Data(),centralityString.Data()),TObject::kOverwrite);
	SystErrGraphNegEffi->Write(Form("%s_SystErrorRelNeg_Efficiency_%s",nameMeson.Data(),centralityString.Data()),TObject::kOverwrite);
	if(histoCorrectionFactorsHistvsPtCatA)histoCorrectionFactorsHistvsPtCatA->Write("PileupContamination");
    histoCorrectedYieldTrue->Write();
	histoCorrectedYieldTrueNarrow->Write();
    histoCorrectedYieldTrueWide->Write();
	histoCorrectedYieldFixed->Write();
	histoCorrectedYieldNarrowFixed->Write();
	histoCorrectedYieldWideFixed->Write();
	histoCorrectedYieldTrueFixed->Write();
	histoCorrectedYieldTrueNarrowFixed->Write();
	histoCorrectedYieldTrueWideFixed->Write();
	histoCorrectedYieldTrueLeft->Write();
	histoCorrectedYieldTrueLeftWide->Write();
	histoCorrectedYieldTrueLeftNarrow->Write();
	histoCorrectedYieldTrueFitted->Write();
	histoCorrectedYieldTrueNarrowFitted->Write();
	histoCorrectedYieldTrueWideFitted->Write();
	histoCorrectedYieldTrueLeftFitted->Write();
	histoCorrectedYieldTrueLeftWideFitted->Write();
	histoCorrectedYieldTrueLeftNarrowFitted->Write();

    // Write back fit histos
	histoCorrectedYieldNormBackFit->Write();
	histoCorrectedYieldNormWideBackFit->Write();
	histoCorrectedYieldNormNarrowBackFit->Write();
	//SystErrGraphPos->Write(Form("%s_SystErrorRelPos_YieldExtraction_%s",nameMeson.Data(),centralityString.Data()),TObject::kOverwrite);
	//SystErrGraphNeg->Write(Form("%s_SystErrorRelNeg_YieldExtraction_%s",nameMeson.Data(),centralityString.Data()),TObject::kOverwrite);
	//if(histoCorrectionFactorsHistvsPtCatA)histoCorrectionFactorsHistvsPtCatA->Write("PileupContamination");
    histoCorrectedYieldTrueBackFit->Write();
	histoCorrectedYieldTrueNarrowBackFit->Write();
    histoCorrectedYieldTrueWideBackFit->Write();
	histoCorrectedYieldFixedBackFit->Write();
	histoCorrectedYieldNarrowFixedBackFit->Write();
	histoCorrectedYieldWideFixedBackFit->Write();
	histoCorrectedYieldTrueFixedBackFit->Write();
	histoCorrectedYieldTrueNarrowFixedBackFit->Write();
	histoCorrectedYieldTrueWideFixedBackFit->Write();
	histoCorrectedYieldTrueFittedBackFit->Write();
	histoCorrectedYieldTrueNarrowFittedBackFit->Write();
	histoCorrectedYieldTrueWideFittedBackFit->Write();
	

	histoUnCorrectedYield->Write();
	histoUnCorrectedYieldBackFit->Write();

	histoFWHMMeson->Write();
	histoMassMeson->Write();
	
	histoAcceptance->Write();
    histoTrueEffiPt->Write(Form("TrueMesonEffiPt%s",InvMassTypeEnding.Data()));
	//    histoCompleteCorr->Write("EffiTimesAcceptanceTimesDeltaY");
    histoTrueEffiPtFit->Write(Form("TrueMesonEffiPtFitted%s",InvMassTypeEnding.Data()));
    histoRatioTrueEffiDivFitted->Write(Form("TrueMesonEffiPtDivFittedEffi%s",InvMassTypeEnding.Data()));
	histoFWHMMeson->Write();
	histoTrueFWHMMeson->Write();
	histoTrueMassMeson->Write();
    histoMesonSignalFullPtInvMass->SetName(Form("FullInvariantMass%s",InvMassTypeEnding.Data()));
	histoMesonSignalFullPtInvMass->Write();
	histoEventQuality->Write();
	histoNumberOfGoodESDTracksVtx->Write();
	histoInputMesonPt->Write();
	histoMCYieldMeson->Write();
	cout<<"save"<<endl;
	histoMCYieldMesonOldBin->Write();
	if (histoInputMesonOldBinPtWOWeights) histoInputMesonOldBinPtWOWeights->Write();
	if (histoInputMesonOldBinPtWeights) histoInputMesonOldBinPtWeights->Write("WeightsMeson");
	if (histoMCInputAddedSig)   histoMCInputAddedSig->Write();
	if (histoMCInputWOWeightingAddedSig)   histoMCInputWOWeightingAddedSig->Write();
	if (histoMCInputWeightsAddedSig) histoMCInputWeightsAddedSig->Write("WeightsMeson_AddedSig");
	
    histoUnCorrectedYieldDrawing->SetName(Form("histoYieldMesonPerEvent%s",InvMassTypeEnding.Data()));
	histoUnCorrectedYieldDrawing->Write();

	histoUnCorrectedYieldDrawingBackFit->SetName(Form("histoYieldMesonPerEventBackFit%s",InvMassTypeEnding.Data()));
	histoUnCorrectedYieldDrawingBackFit->Write();
	deltaPt->Write("deltaPt");
	correctedOutput->Write();
	correctedOutput->Close();
	
	if (histoEffiNarrowPt || histoEffiWidePt || histoEffiLeftPt || histoEffiLeftNarrowPt || histoEffiLeftWidePt){}
}
