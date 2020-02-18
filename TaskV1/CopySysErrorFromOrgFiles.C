//***********************************************************************************************
//**************************** CutStudiesOverview ***********************************************
//***********************************************************************************************
/************************************************************************************************
 ******     provided by Gamma Conversion Group, PWG4,                                         *****
 ******        Ana Marin, marin@physi.uni-heidelberg.de                                        *****
 ******        Friederike Bock, friederike.bock@cern.ch                                        *****
 ***********************************************************************************************/

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


void CopySysErrorFromOrgFiles(      TString pathPP                      = "",
                                    TString cutNumberPP                 = "",
                                    TString pathPPb                     = "",
                                    TString cutNumberPPb                = "",
                                    TString meson                       = "",
                                    TString optionEnergy                = "",
                                    TString triggerName                 = "",
                                    Bool_t isMC                         = kFALSE,
                                    Int_t mode                          = 0
                       ){

    
    // Define Output Directory
    TString outputDirRootFile                               = Form("CutStudies/%s",optionEnergy.Data());
    gSystem->Exec("mkdir -p "+outputDirRootFile);

    // Define input and output MC/data
    TString prefix2                                         = "";
    if (isMC)
        prefix2                                             = "MC";
    else 
        prefix2                                             = "data";
    

    // Define yield extraction error graphs
    TGraphAsymmErrors* systErrGraphNegYieldExtPP            = NULL;
    TGraphAsymmErrors* systErrGraphPosYieldExtPP            = NULL;
    TGraphAsymmErrors* systErrGraphNegYieldExtPPb           = NULL;
    TGraphAsymmErrors* systErrGraphPosYieldExtPPb           = NULL;

    TGraphAsymmErrors* systErrGraphBGEstimatePP             = NULL;
    TGraphAsymmErrors* systErrGraphBGEstimateIterationsPP   = NULL;
    TGraphAsymmErrors* systErrGraphBGEstimatePPb            = NULL;
    TGraphAsymmErrors* systErrGraphBGEstimateIterationsPPb  = NULL;
    
    // Define CutSelections
    TString fEventCutSelectionPPb                           = "";
    TString fGammaCutSelectionPPb                           = "";
    TString fClusterCutSelectionPPb                         = "";
    TString fElectronCutSelectionPPb                        = "";
    TString fMesonCutSelectionPPb                           = "";
    // disentangle cut selection
    ReturnSeparatedCutNumberAdvanced(cutNumberPPb,fEventCutSelectionPPb, fGammaCutSelectionPPb, fClusterCutSelectionPPb, fElectronCutSelectionPPb, fMesonCutSelectionPPb, mode);
    
    TString fileNameCorrectedPP                     = pathPP;        
    TFile* cutcorrfilePP                            = new TFile(fileNameCorrectedPP);
    TString fileNameCorrectedPPb                    = pathPPb;        
    TFile* cutcorrfilePPb                           = new TFile(fileNameCorrectedPPb);
    TString centralityStringPPb                     = GetCentralityString(fEventCutSelectionPPb.Data());                
    TString centralityStringPP                      = GetCentralityString(cutNumberPP.Data());                
    TString centralityStringOutput                  = GetCentralityStringOutput(fEventCutSelectionPPb.Data());
    TString simpleTriggerName                       = triggerName;
                
    // pp graphs
    systErrGraphNegYieldExtPP                       = (TGraphAsymmErrors*)cutcorrfilePP->Get(Form("%s_SystErrorRelNeg_YieldExtraction_%s",meson.Data(), centralityStringPP.Data()));
    Double_t* negErrorYieldPP                       = systErrGraphNegYieldExtPP->GetY();
    for (Int_t j = 0; j < systErrGraphNegYieldExtPP->GetN(); j++){
        negErrorYieldPP[j]                          = -1*negErrorYieldPP[j];
    }
    systErrGraphPosYieldExtPP                       = (TGraphAsymmErrors*)cutcorrfilePP->Get(Form("%s_SystErrorRelPos_YieldExtraction_%s",meson.Data(), centralityStringPP.Data()));
    systErrGraphBGEstimatePP                        = (TGraphAsymmErrors*)cutcorrfilePP->Get(Form("%s_SystErrorRel_BGEstimate_%s",meson.Data(), centralityStringPP.Data()));
    systErrGraphBGEstimateIterationsPP              = (TGraphAsymmErrors*)cutcorrfilePP->Get(Form("%s_SystErrorRel_BGEstimateIterations_%s",meson.Data(), centralityStringPP.Data()));
    if (simpleTriggerName.CompareTo("MB")!= 0 && simpleTriggerName.CompareTo("INT7")!= 0 && simpleTriggerName.CompareTo("")!= 0)
        centralityStringPP                          = simpleTriggerName+"_"+centralityStringPP;
    // pPb graphs
    systErrGraphNegYieldExtPPb                      = (TGraphAsymmErrors*)cutcorrfilePPb->Get(Form("%s_SystErrorRelNeg_YieldExtraction_%s",meson.Data(), centralityStringPPb.Data()));
    Double_t* negErrorYieldPPb                      = systErrGraphNegYieldExtPPb->GetY();
    for (Int_t j = 0; j < systErrGraphNegYieldExtPPb->GetN(); j++){
        negErrorYieldPPb[j]                         = -1*negErrorYieldPPb[j];
    }
    systErrGraphPosYieldExtPPb                      = (TGraphAsymmErrors*)cutcorrfilePPb->Get(Form("%s_SystErrorRelPos_YieldExtraction_%s",meson.Data(), centralityStringPPb.Data()));
    systErrGraphBGEstimatePPb                       = (TGraphAsymmErrors*)cutcorrfilePPb->Get(Form("%s_SystErrorRel_BGEstimate_%s",meson.Data(), centralityStringPPb.Data()));
    systErrGraphBGEstimateIterationsPPb             = (TGraphAsymmErrors*)cutcorrfilePPb->Get(Form("%s_SystErrorRel_BGEstimateIterations_%s",meson.Data(), centralityStringPPb.Data()));
    if (simpleTriggerName.CompareTo("MB")!= 0 && simpleTriggerName.CompareTo("INT7")!= 0 && simpleTriggerName.CompareTo("")!= 0)
        centralityStringPPb                         = simpleTriggerName+"_"+centralityStringPPb;
        
    // Write sys-err graph to root output file
    TString Outputname = Form("%s/%sRatio_%s_SystematicErrorCuts.root",outputDirRootFile.Data(),meson.Data(),prefix2.Data());
    TFile* SystematicErrorFile = new TFile(Outputname.Data(),"UPDATE");

        systErrGraphNegYieldExtPP->Write(Form("%sRatio_SystErrorRelNeg_YieldExtraction_%s",meson.Data(),centralityStringPP.Data()),TObject::kOverwrite);
        systErrGraphPosYieldExtPP->Write(Form("%sRatio_SystErrorRelPos_YieldExtraction_%s",meson.Data(),centralityStringPP.Data()),TObject::kOverwrite);
        if (systErrGraphBGEstimatePP) systErrGraphBGEstimatePP->Write(Form("%sRatio_SystErrorRel_BGEstimate_%s",meson.Data(),centralityStringPP.Data()),TObject::kOverwrite);
        if (systErrGraphBGEstimateIterationsPP) systErrGraphBGEstimateIterationsPP->Write(Form("%sRatio_SystErrorRel_BGEstimateIterations_%s",meson.Data(),centralityStringPP.Data()),TObject::kOverwrite);
        systErrGraphNegYieldExtPPb->Write(Form("%sRatio_SystErrorRelNeg_YieldExtraction_%s",meson.Data(),centralityStringPPb.Data()),TObject::kOverwrite);
        systErrGraphPosYieldExtPPb->Write(Form("%sRatio_SystErrorRelPos_YieldExtraction_%s",meson.Data(),centralityStringPPb.Data()),TObject::kOverwrite);
        if (systErrGraphBGEstimatePPb) systErrGraphBGEstimatePPb->Write(Form("%sRatio_SystErrorRel_BGEstimate_%s",meson.Data(),centralityStringPPb.Data()),TObject::kOverwrite);
        if (systErrGraphBGEstimateIterationsPPb) systErrGraphBGEstimateIterationsPPb->Write(Form("%sRatio_SystErrorRel_BGEstimateIterations_%s",meson.Data(),centralityStringPPb.Data()),TObject::kOverwrite);
    SystematicErrorFile->Write();
    SystematicErrorFile->Close();

}
