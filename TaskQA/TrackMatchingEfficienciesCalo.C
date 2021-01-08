/***********************************************************************************************
*** provided by Gamma Conversion Group, PWGGA,                                            ******
***     Friederike Bock, fbock@cern.ch                                                    ******
************************************************************************************************
************************************************************************************************/

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
#include "../CommonHeaders/PlottingGammaConversionHistos.h"
#include "../CommonHeaders/PlottingGammaConversionAdditional.h"
#include "../CommonHeaders/FittingGammaConversion.h"
#include "../CommonHeaders/ExtractSignalBinning.h"
#include "../CommonHeaders/ConversionFunctionsBasicsAndLabeling.h"
#include "../CommonHeaders/ConversionFunctions.h"

TString textGenerator;
TString collisionSystem;
TString textALICE;
TString textPeriod;
TString textDate;
TString mesonLatex;
TString detectionProcess;

//**********************************************************************************
//******************** Helper function for 2D histo plotting ***********************
//**********************************************************************************
void DrawAutoGammaHistoPaper2D( TH2* histo1,
                    TString Title, TString XTitle, TString YTitle,
                    Bool_t YRangeMax, Double_t YMaxFactor, Double_t YMinimum,
                    Bool_t YRange, Double_t YMin ,Double_t YMax,
                    Bool_t XRange, Double_t XMin, Double_t XMax, Double_t xOffset, Double_t yOffset) {
    if (YRangeMax && !XRange){
        YRange = kFALSE;
        Double_t maxRangeR = histo1->GetMaximum();
        Double_t minRangeR = histo1->GetMinimum();
        if(YMinimum > minRangeR){minRangeR = YMinimum;}
        histo1->GetYaxis()->SetRangeUser(minRangeR, maxRangeR*YMaxFactor);
    }
    if (YRangeMax && XRange){
        YRange = kFALSE;
        Double_t maxRangeR = histo1->GetMaximum();
        Double_t minRangeR = histo1->GetMinimum();
        if(YMinimum > minRangeR){minRangeR = YMinimum;}
        histo1->GetYaxis()->SetRangeUser(minRangeR, maxRangeR*YMaxFactor);
        histo1->GetXaxis()->SetRangeUser(XMin, XMax);
    }
    if (YRange && XRange){
        histo1->GetYaxis()->SetRangeUser(YMin, YMax);
        histo1->GetXaxis()->SetRangeUser(XMin, XMax);
    }
    if (!YRangeMax && !YRange && XRange){
        histo1->GetXaxis()->SetRangeUser(XMin, XMax);
    }

    if (YRange && !XRange){
        histo1->GetYaxis()->SetRangeUser(YMin, YMax);
    }

    histo1->SetTitle(Title.Data());

    if(XTitle.CompareTo("") != 0){
        histo1->SetXTitle(XTitle.Data());
    }
    if(YTitle.CompareTo("") != 0){
        histo1->SetYTitle(YTitle.Data());
    }

    histo1->GetYaxis()->SetLabelFont(42);
    histo1->GetXaxis()->SetLabelFont(42);
    histo1->GetYaxis()->SetTitleFont(62);
    histo1->GetXaxis()->SetTitleFont(62);
    histo1->GetYaxis()->SetLabelSize(0.045);
    histo1->GetYaxis()->SetTitleSize(0.05);
    histo1->GetYaxis()->SetDecimals();
    histo1->GetXaxis()->SetTitleOffset(xOffset);
    histo1->GetYaxis()->SetTitleOffset(yOffset);
    histo1->GetXaxis()->SetTitleSize(0.05);
    histo1->GetXaxis()->SetLabelSize(0.045);
}

//**********************************************************************************
//******************** Plotting 2D resolution matrix *******************************
//**********************************************************************************
void PlotStandard2D( TH2* histo2D,
                     TString nameOutput,
                     TString title,
                     TString xTitle,
                     TString yTitle,
                     Bool_t kRangeYAutoMax,
                     Double_t maxFacY,
                     Double_t startYAuto,
                     Bool_t kRangeY,
                     Double_t startY,
                     Double_t endY,
                     Bool_t kRangeX,
                     Double_t startX,
                     Double_t endX,
                     Double_t startZ,
                     Int_t logX,
                     Int_t logY,
                     Int_t logZ,
                     Float_t* floatLogo,
                     Double_t offsetX,
                     Double_t offsetY,
                     TString additionalLabel  = "",
                     Int_t canvasSizeX        = 500,
                     Int_t canvasSizeY        = 500
                   ){

    TCanvas *canvasStandard = new TCanvas("canvasStandard","canvasStandard",canvasSizeX,canvasSizeY);
    DrawGammaCanvasSettings( canvasStandard, 0.115, 0.11, 0.02, 0.11);
    canvasStandard->SetLogx(logX);
    canvasStandard->SetLogy(logY);
    canvasStandard->SetLogz(logZ);
    canvasStandard->cd();
    if (logX) histo2D->GetXaxis()->SetLabelOffset(-0.01);

    DrawAutoGammaHistoPaper2D(  histo2D,
                                title.Data(), xTitle.Data(), yTitle.Data(),
                                kRangeYAutoMax, maxFacY, startYAuto,
                                kRangeY, startY, endY,
                                kRangeX, startX, endX,
                                offsetX, offsetY
                             );
    histo2D->GetZaxis()->SetRangeUser(startZ, histo2D->GetMaximum());

    histo2D->SetTitle(title.Data());
    histo2D->Draw("colz");

    PutProcessLabelAndEnergyOnPlot(floatLogo[0], floatLogo[1], 28, collisionSystem.Data(), detectionProcess.Data(), additionalLabel.Data(), 43, 0.03);

    canvasStandard->Update();
    canvasStandard->SaveAs(nameOutput.Data());
    delete canvasStandard;
}


//**********************************************************************************
//******************* return minimum for 2 D histo  ********************************
//**********************************************************************************
Double_t FindSmallestEntryIn2D(TH2* histo){
    Double_t minimum = 1;
    for (Int_t i = 1; i<histo->GetNbinsX(); i++){
        for (Int_t j = 1; j<histo->GetNbinsY(); j++){
            if (histo->GetBinContent(i,j) < minimum && histo->GetBinContent(i,j) > 0){
                minimum = histo->GetBinContent(i,j);
            }
        }
    }
    return minimum;
}


//**********************************************************************************
//************* Main function for track matching efficiencies **********************
//**********************************************************************************
void TrackMatchingEfficienciesCalo(   TString fileMonteCarloInput          = "",
                                      TString optionCutSelection           = "",
                                      TString suffix                       = "eps",
                                      TString optEnergy                    = "",
                                      TString optMCGenerator               = "",
                                      TString optPeriod                    = "",
                                      Int_t mode                           = 0){

    gROOT->Reset();
    gROOT->SetStyle("Plain");

    StyleSettings();
    SetPlotStyle();



    if (mode == 0 || mode == 1 || mode == 9) {
      cout << "ERROR: you are running in the PCM or PCM-Dalitz mode, this macro can't run in these modes, aborting." << endl;
      return;
    }
    InitializeClusterBinning(optEnergy, mode);

    collisionSystem                 = ReturnFullCollisionsSystem(optEnergy);
    detectionProcess                = ReturnFullTextReconstructionProcess(mode);
    // if(optEnergy.Contains("13TeV")) detectionProcess = "clusters rec. w. EMC";
    if(optEnergy.Contains("13TeV")) detectionProcess = "";
    textALICE = "ALICE simulation";
    if (collisionSystem.CompareTo("") == 0){
        cout << "No correct collision system specification, has been given" << endl;
        return;
    }

    TString centrality              = "";
    TString firstCutnumber          = optionCutSelection(GetEventSystemCutPosition(),1);
    if (firstCutnumber.CompareTo("0") != 0){
        centrality                  = GetCentralityString(optionCutSelection);
        collisionSystem             = Form("%s %s", centrality.Data(), collisionSystem.Data());
    }
    cout << centrality.Data() << endl;

    if(optMCGenerator.CompareTo("") ==0){
        textGenerator               = "";
    } else {
        textGenerator               = optMCGenerator;
    }
    textDate                        = ReturnDateString();

    //************************************ Separate cutstrings ***********************************
    TString fEventCutNumber         = "";
    TString fGammaCutNumber         = "";
    TString fClusterCutNumber       = "";
    TString fClusterMergedCutNumber = "";
    TString fMesonCutNumber         = "";
    TString dummyString             = "";
    if (mode != 10 && mode != 11 ){
        ReturnSeparatedCutNumberAdvanced( optionCutSelection,fEventCutNumber, fGammaCutNumber, fClusterCutNumber, dummyString, fMesonCutNumber, mode);
    } else {
        ReturnSeparatedCutNumberAdvanced( optionCutSelection, fEventCutNumber, fClusterCutNumber, fClusterMergedCutNumber, dummyString, fMesonCutNumber, mode);
    }
    TString trigger                             = fEventCutNumber(GetEventSelectSpecialTriggerCutPosition(),2);
    TString nameTrigger                         = ReturnTriggerName(trigger.Atoi());


    TString outputDirectory;
    if(optPeriod.CompareTo("") ==0 || optPeriod.CompareTo("All") ==0){
        textPeriod                  = "";
        outputDirectory             = Form("%s/%s/%s/TrackMatchingEfficienciesCalo",optionCutSelection.Data(),optEnergy.Data(),suffix.Data());
        gSystem->Exec("mkdir -p "+outputDirectory);
    } else {
        textPeriod                  = optPeriod;
        outputDirectory             = Form("%s/%s/%s/%s/TrackMatchingEfficienciesCalo",optionCutSelection.Data(),optEnergy.Data(),suffix.Data(),optPeriod.Data());
        gSystem->Exec("mkdir -p "+outputDirectory);
    }


      Int_t fNBinsClusterE = 45;
      Double_t fBinsClusterE[66];
      Double_t maxClusterE             = 400;
      if(optEnergy.Contains("13TeV")){
        for(Int_t i=0; i < fNBinsClusterE+1; ++i){
          if (i < 1) fBinsClusterE[i]          = 0.7*(i+1);
          else if(i<4) fBinsClusterE[i]        = 0.7+0.1*(i);
          else if(i<22) fBinsClusterE[i]       = 1.+0.5*(i-3);
          else if(i<32) fBinsClusterE[i]       = 10.+1.0*(i-21);
          else if(i<37) fBinsClusterE[i]       = 20.+2.0*(i-31);
          else if(i<41) fBinsClusterE[i]       = 30.+5.0*(i-36);
          else if(i<56) fBinsClusterE[i]      = 50.+10.0*(i-40);
          else if(i<66) fBinsClusterE[i]      = 200.+20.0*(i-55);
          else fBinsClusterE[i]                = maxClusterE;
        }
      } else {
        fNBinsClusterE = fNBinsClusterPt;
        for(Int_t i=0; i<fNBinsClusterPt+1;i++){
          fBinsClusterE[i]          = fBinsClusterPt[i];
        }
    }


    //**********************************************************************************************************************
    //******************************** Definition of some plotting variables ***********************************************
    //**********************************************************************************************************************

    //Array defintion for printing Logo
    Float_t floatLocationRightDown2D[4]     = {0.15,0.25,0.11, 0.02};
    Float_t floatLocationUpDown2D[4]        = {0.15,0.95,0.11, 0.02};
    //**********************************************************************************************************************
    //****************************************** Loading of Histograms *****************************************************
    //**********************************************************************************************************************

    TFile* fileMC                       = new TFile(fileMonteCarloInput.Data());
    TString autoDetectedMainDir         = AutoDetectMainTList(mode , fileMC);
    if (autoDetectedMainDir.CompareTo("") == 0){
        cout << "ERROR: trying to read file, which is incompatible with mode selected" << endl;;
        return;
    }

    TList* TopDir                       = (TList*)fileMC->Get(autoDetectedMainDir.Data());
    if(TopDir == NULL){
        cout<<"ERROR: TopDir not Found"<<endl;
        return;
    }
    //***************************** Histo Loading ********************************************************************
    TList* HistosMainCut                = (TList*)TopDir->FindObject(Form("Cut Number %s",optionCutSelection.Data()));

    TString clusterCuts[2]              = {fClusterCutNumber, fClusterMergedCutNumber};

    for (Int_t i = 0; i< 2; i++){
        cout << "Processing cluster cut: " << clusterCuts[i].Data() << endl;
        if (clusterCuts[i].CompareTo("") == 0 ){
            cout << "INFO: cluster cut not defined" << endl;
            continue;
        }
        TString trackMatchingCut(clusterCuts[i](GetClusterTrackMatchingCutPosition(clusterCuts[i]),1));
        TString TrackMatchingMode = AnalyseTMEOverPCut(CutNumberToInteger(trackMatchingCut), mode);
        cout<<"TrackMatchingMode: "<<TrackMatchingMode<<endl;

        TList* CaloCutContainer             = (TList*)HistosMainCut->FindObject(Form("CaloCuts_%s",clusterCuts[i].Data()));
        TH2F* histoTMEffiInput              = (TH2F*)CaloCutContainer->FindObject(Form("TMEffiInputHisto %s", clusterCuts[i].Data()));
        if (!histoTMEffiInput){
            cout << "INFO: current contained: " << clusterCuts[i].Data() << " does not contain necessary inputs, jumping to next." << endl;
            continue;
        }
        TH2F* histoClE_TrE_ChCl             = (TH2F*)CaloCutContainer->FindObject(Form("ClusterE_TrackE_ChargedCluster %s", clusterCuts[i].Data()));
        TH2F* histoClE_TrE_ChClLeadMatch    = (TH2F*)CaloCutContainer->FindObject(Form("ClusterE_TrackE_ChargedCluster_LeadMatched %s", clusterCuts[i].Data()));
        TH2F* histoClE_TrE_NeCl             = (TH2F*)CaloCutContainer->FindObject(Form("ClusterE_TrackE_NeutralCluster %s", clusterCuts[i].Data()));
        TH2F* histoClE_TrE_NeClSubCh        = (TH2F*)CaloCutContainer->FindObject(Form("ClusterE_TrackE_NeutralClusterSubCharged %s", clusterCuts[i].Data()));
        TH2F* histoClE_TrE_GaCl             = (TH2F*)CaloCutContainer->FindObject(Form("ClusterE_TrackE_GammaCluster %s", clusterCuts[i].Data()));
        TH2F* histoClE_TrE_GaClSubCh        = (TH2F*)CaloCutContainer->FindObject(Form("ClusterE_TrackE_GammaClusterSubCharged %s", clusterCuts[i].Data()));
        TH2F* histoClE_TrE_ConvCl           = (TH2F*)CaloCutContainer->FindObject(Form("ClusterE_TrackE_ConvCluster %s", clusterCuts[i].Data()));
        TH2F* histoClE_NMatch_NeCl          = (TH2F*)CaloCutContainer->FindObject(Form("ClusterE_NMatches_NeutralCluster %s", clusterCuts[i].Data()));
        TH2F* histoClE_NMatch_ChCl          = (TH2F*)CaloCutContainer->FindObject(Form("ClusterE_NMatches_ChargedCluster %s", clusterCuts[i].Data()));

        NormalizeBinWidth2d(histoClE_TrE_ChCl);
        NormalizeBinWidth2d(histoClE_TrE_ChClLeadMatch);
        NormalizeBinWidth2d(histoClE_TrE_NeCl);
        NormalizeBinWidth2d(histoClE_TrE_NeClSubCh);
        NormalizeBinWidth2d(histoClE_TrE_GaCl);
        NormalizeBinWidth2d(histoClE_TrE_GaClSubCh);
        NormalizeBinWidth2d(histoClE_TrE_ConvCl);
        NormalizeBinWidth2d(histoClE_NMatch_NeCl);
        NormalizeBinWidth2d(histoClE_NMatch_ChCl);
        // ********************************************************************************************************************
        // ****************************** Plotting 2D distributions ***********************************************************
        // ********************************************************************************************************************
        PlotStandard2D( histoClE_TrE_ChCl, Form("%s/ClusterE_TrackE_ChargedClusters_%s.%s",outputDirectory.Data(),clusterCuts[i].Data(), suffix.Data()), "",
                        "#it{E} (GeV)", "#it{E}_{tr} (GeV)",
                        0, 0, 0,
                        0, -10, 10,
                        0, 0, 50, FindSmallestEntryIn2D(histoClE_TrE_ChCl),
                        1, 1, 1, floatLocationUpDown2D, 1, 1., "lead. part. in cl.: charged ", 1000, 800);
        PlotStandard2D( histoClE_TrE_ChClLeadMatch, Form("%s/ClusterE_TrackE_ChargedClusters_MatchedLeadParticle_%s.%s",outputDirectory.Data(),clusterCuts[i].Data(), suffix.Data()), "",
                        "#it{E} (GeV)", "#it{E}_{tr} (GeV)",
                        0, 0, 0,
                        0, -10, 10,
                        0, 0, 50, FindSmallestEntryIn2D(histoClE_TrE_ChClLeadMatch),
                        1, 1, 1, floatLocationUpDown2D, 1, 1., "lead. part. in cl.: charged, matched", 1000, 800);
        PlotStandard2D( histoClE_TrE_NeCl, Form("%s/ClusterE_TrackE_NeutralClusters_%s.%s",outputDirectory.Data(),clusterCuts[i].Data(), suffix.Data()), "",
                        "#it{E} (GeV)", "#it{E}_{tr} (GeV)",
                        0, 0, 0,
                        0, -10, 10,
                        0, 0, 50, FindSmallestEntryIn2D(histoClE_TrE_NeCl),
                        1, 1, 1, floatLocationUpDown2D, 1, 1., "lead. part. in cl.: neutral", 1000, 800);
        PlotStandard2D( histoClE_TrE_NeClSubCh, Form("%s/ClusterE_TrackE_NeutralClustersSubCharged_%s.%s",outputDirectory.Data(),clusterCuts[i].Data(), suffix.Data()), "",
                        "#it{E} (GeV)", "#it{E}_{tr} (GeV)",
                        0, 0, 0,
                        0, -10, 10,
                        0, 0, 50, FindSmallestEntryIn2D(histoClE_TrE_NeClSubCh),
                        1, 1, 1, floatLocationUpDown2D, 1, 1., "lead. part. in cl.: neutral, sub charged", 1000, 800);
        PlotStandard2D( histoClE_TrE_GaCl, Form("%s/ClusterE_TrackE_GammaClusters_%s.%s",outputDirectory.Data(),clusterCuts[i].Data(), suffix.Data()), "",
                        "#it{E} (GeV)", "#it{E}_{tr} (GeV)",
                        0, 0, 0,
                        0, -10, 10,
                        0, 0, 50, FindSmallestEntryIn2D(histoClE_TrE_GaCl),
                        1, 1, 1, floatLocationUpDown2D, 1, 1., "lead. part. in cl.: #gamma", 1000, 800);
        PlotStandard2D( histoClE_TrE_GaClSubCh, Form("%s/ClusterE_TrackE_GammaClustersSubCharged_%s.%s",outputDirectory.Data(),clusterCuts[i].Data(), suffix.Data()), "",
                        "#it{E} (GeV)", "#it{E}_{tr} (GeV)",
                        0, 0, 0,
                        0, -10, 10,
                        0, 0, 50, FindSmallestEntryIn2D(histoClE_TrE_GaClSubCh),
                        1, 1, 1, floatLocationUpDown2D, 1, 1., "lead. part. in cl.: #gamma, sub charged", 1000, 800);
        PlotStandard2D( histoClE_TrE_ConvCl, Form("%s/ClusterE_TrackE_ConvClusters_%s.%s",outputDirectory.Data(),clusterCuts[i].Data(), suffix.Data()), "",
                        "#it{E} (GeV)", "#it{E}_{tr} (GeV)",
                        0, 0, 0,
                        0, -10, 10,
                        0, 0, 50, FindSmallestEntryIn2D(histoClE_TrE_ConvCl),
                        1, 1, 1, floatLocationUpDown2D, 1, 1., "lead. part. in cl.: #gamma_{conv}", 1000, 800);
        PlotStandard2D( histoClE_NMatch_ChCl, Form("%s/ClusterE_NMatches_ChargedClusters_%s.%s",outputDirectory.Data(),clusterCuts[i].Data(), suffix.Data()), "",
                        "#it{E} (GeV)", "#it{N}_{matches}",
                        0, 0, 0,
                        0, -10, 10,
                        0, 0, 50, FindSmallestEntryIn2D(histoClE_NMatch_ChCl),
                        1, 0, 1, floatLocationUpDown2D, 1, 1., "lead. part. in cl.: charged", 1000, 800);
        PlotStandard2D( histoClE_NMatch_NeCl, Form("%s/ClusterE_NMatches_NeutralClusters_%s.%s",outputDirectory.Data(),clusterCuts[i].Data(), suffix.Data()), "",
                        "#it{E} (GeV)", "#it{N}_{matches}",
                        0, 0, 0,
                        0, -10, 10,
                        0, 0, 50, FindSmallestEntryIn2D(histoClE_NMatch_NeCl),
                        1, 0, 1, floatLocationUpDown2D, 1, 1., "lead. part. in cl.: neutral", 1000, 800);

        // ********************************************************************************************************************
        // ********************************* projections for efficiency calculations ******************************************
        // ********************************************************************************************************************
        histoTMEffiInput->Sumw2();

        TH1D*   fDeltaPtCluster       = new TH1D("fDeltaPtCluster","",fNBinsClusterE,fBinsClusterE);
        for(Int_t iPt=1;iPt<fNBinsClusterE+1;iPt++){
            fDeltaPtCluster->SetBinContent(iPt,fBinsClusterE[iPt]-fBinsClusterE[iPt-1]);
            fDeltaPtCluster->SetBinError(iPt,0);
        }

        // *********** all inputs split in classes
        TH1D* histoClE_AllCl            = (TH1D*) histoTMEffiInput->ProjectionX("histoClE_AllCl",1,1,"e");
        TGraphErrors gr(histoClE_AllCl);
        gr.Print();
        TH1D* histoClE_AllClReb         = (TH1D*)histoClE_AllCl->Rebin(fNBinsClusterE,"histoClE_AllClReb",fBinsClusterE);
        histoClE_AllClReb->Divide(fDeltaPtCluster);
        TH1D* histoClE_ChCl             = (TH1D*) histoTMEffiInput->ProjectionX("histoClE_ChCl",2,2,"e");
        TH1D* histoClE_ChClReb          = (TH1D*)histoClE_ChCl->Rebin(fNBinsClusterE,"histoClE_ChClReb",fBinsClusterE);
        histoClE_ChClReb->Divide(fDeltaPtCluster);
        TH1D* histoClE_NeCl             = (TH1D*) histoTMEffiInput->ProjectionX("histoClE_NeCl",3,3,"e");
        TH1D* histoClE_NeClReb          = (TH1D*)histoClE_NeCl->Rebin(fNBinsClusterE,"histoClE_NeClReb",fBinsClusterE);
        histoClE_NeClReb->Divide(fDeltaPtCluster);
        TH1D* histoClE_NeClSubCh        = (TH1D*) histoTMEffiInput->ProjectionX("histoClE_NeClSubCh",4,4,"e");
        TH1D* histoClE_NeClSubChReb     = (TH1D*)histoClE_NeClSubCh->Rebin(fNBinsClusterE,"histoClE_NeClSubChReb",fBinsClusterE);
        histoClE_NeClSubChReb->Divide(fDeltaPtCluster);
        TH1D* histoClE_GaCl             = (TH1D*) histoTMEffiInput->ProjectionX("histoClE_GaCl",5,5,"e");
        TH1D* histoClE_GaClReb          = (TH1D*)histoClE_GaCl->Rebin(fNBinsClusterE,"histoClE_GaClReb",fBinsClusterE);
        histoClE_GaClReb->Divide(fDeltaPtCluster);
        TH1D* histoClE_GaClSubCh        = (TH1D*) histoTMEffiInput->ProjectionX("histoClE_GaClSubCh",6,6,"e");
        TH1D* histoClE_GaClSubChReb     = (TH1D*)histoClE_GaClSubCh->Rebin(fNBinsClusterE,"histoClE_GaClSubChReb",fBinsClusterE);
        histoClE_GaClSubChReb->Divide(fDeltaPtCluster);
        TH1D* histoClE_ConvCl           = (TH1D*) histoTMEffiInput->ProjectionX("histoClE_ConvCl",7,7,"e");
        TH1D* histoClE_ConvClReb        = (TH1D*)histoClE_ConvCl->Rebin(fNBinsClusterE,"histoClE_ConvClReb",fBinsClusterE);
        histoClE_ConvClReb->Divide(fDeltaPtCluster);
        TH1D* histoClE_ChPrimCl         = (TH1D*) histoTMEffiInput->ProjectionX("histoClE_ChPrimCl",8,8,"e");
        TH1D* histoClE_ChPrimClReb      = (TH1D*)histoClE_ChPrimCl->Rebin(fNBinsClusterE,"histoClE_ChPrimClReb",fBinsClusterE);
        histoClE_ChPrimClReb->Divide(fDeltaPtCluster);
        TH1D* histoClE_ElCl             = (TH1D*) histoTMEffiInput->ProjectionX("histoClE_ElCl",9,9,"e");
        TH1D* histoClE_ElClReb          = (TH1D*)histoClE_ElCl->Rebin(fNBinsClusterE,"histoClE_ElClReb",fBinsClusterE);
        histoClE_ElClReb->Divide(fDeltaPtCluster);

        TH1D* histoRatioClE_ChtoAllReb          = (TH1D*)histoClE_ChClReb->Clone("histoRatioClE_ChtoAllReb");
        histoRatioClE_ChtoAllReb->Divide(histoRatioClE_ChtoAllReb, histoClE_AllClReb, 1, 1, "B");
        TH1D* histoRatioClE_NetoAllReb          = (TH1D*)histoClE_NeClReb->Clone("histoRatioClE_NetoAllReb");
        histoRatioClE_NetoAllReb->Divide(histoRatioClE_NetoAllReb, histoClE_AllClReb, 1, 1, "B");
        TH1D* histoRatioClE_NeSubChtoAllReb     = (TH1D*)histoClE_NeClSubChReb->Clone("histoRatioClE_NeSubChtoAllReb");
        histoRatioClE_NeSubChtoAllReb->Divide(histoRatioClE_NeSubChtoAllReb, histoClE_AllClReb, 1, 1, "B");
        TH1D* histoRatioClE_GatoAllReb          = (TH1D*)histoClE_GaClReb->Clone("histoRatioClE_GatoAllReb");
        histoRatioClE_GatoAllReb->Divide(histoRatioClE_GatoAllReb, histoClE_AllClReb, 1, 1, "B");
        TH1D* histoRatioClE_GaSubChtoAllReb     = (TH1D*)histoClE_GaClSubChReb->Clone("histoRatioClE_GaSubChtoAllReb");
        histoRatioClE_GaSubChtoAllReb->Divide(histoRatioClE_GaSubChtoAllReb, histoClE_AllClReb, 1, 1, "B");
        TH1D* histoRatioClE_ChPrimtoAllReb      = (TH1D*)histoClE_ChPrimClReb->Clone("histoRatioClE_ChPrimtoAllReb");
        histoRatioClE_ChPrimtoAllReb->Divide(histoRatioClE_ChPrimtoAllReb, histoClE_AllClReb, 1, 1, "B");
        TH1D* histoRatioClE_EltoAllReb          = (TH1D*)histoClE_ElClReb->Clone("histoRatioClE_EltoAllReb");
        histoRatioClE_EltoAllReb->Divide(histoRatioClE_EltoAllReb, histoClE_AllClReb, 1, 1, "B");
        TH1D* histoRatioClE_ConvtoAllReb        = (TH1D*)histoClE_ConvClReb->Clone("histoRatioClE_ConvtoAllReb");
        histoRatioClE_ConvtoAllReb->Divide(histoRatioClE_ConvtoAllReb, histoClE_AllClReb, 1, 1, "B");

        // *********** all inputs split in classes with matches
        TH1D* histoClE_AllCl_mat        = (TH1D*) histoTMEffiInput->ProjectionX("histoClE_AllCl_mat",10,10,"e");
        TH1D* histoClE_AllCl_matReb     = (TH1D*)histoClE_AllCl_mat->Rebin(fNBinsClusterE,"histoClE_AllCl_matReb",fBinsClusterE);
        histoClE_AllCl_matReb->Divide(fDeltaPtCluster);
        TH1D* histoClE_ChCl_mat         = (TH1D*) histoTMEffiInput->ProjectionX("histoClE_ChCl_mat",11,11,"e");
        TH1D* histoClE_ChCl_matReb      = (TH1D*)histoClE_ChCl_mat->Rebin(fNBinsClusterE,"histoClE_ChCl_matReb",fBinsClusterE);
        histoClE_ChCl_matReb->Divide(fDeltaPtCluster);
        TH1D* histoClE_ChCl_leadmat     = (TH1D*) histoTMEffiInput->ProjectionX("histoClE_ChCl_leadmat",12,12,"e");
        TH1D* histoClE_ChCl_leadmatReb  = (TH1D*)histoClE_ChCl_leadmat->Rebin(fNBinsClusterE,"histoClE_ChCl_leadmatReb",fBinsClusterE);
        histoClE_ChCl_leadmatReb->Divide(fDeltaPtCluster);
        TH1D* histoClE_NeCl_mat         = (TH1D*) histoTMEffiInput->ProjectionX("histoClE_NeCl_mat",13,13,"e");
        TH1D* histoClE_NeCl_matReb      = (TH1D*)histoClE_NeCl_mat->Rebin(fNBinsClusterE,"histoClE_NeCl_matReb",fBinsClusterE);
        histoClE_NeCl_matReb->Divide(fDeltaPtCluster);
        TH1D* histoClE_NeClSubCh_mat    = (TH1D*) histoTMEffiInput->ProjectionX("histoClE_NeClSubCh_mat",14,14,"e");
        TH1D* histoClE_NeClSubCh_matReb = (TH1D*)histoClE_NeClSubCh_mat->Rebin(fNBinsClusterE,"histoClE_NeClSubCh_matReb",fBinsClusterE);
        histoClE_NeClSubCh_matReb->Divide(fDeltaPtCluster);
        TH1D* histoClE_GaCl_mat         = (TH1D*) histoTMEffiInput->ProjectionX("histoClE_GaCl_mat",15,15,"e");
        TH1D* histoClE_GaCl_matReb      = (TH1D*)histoClE_GaCl_mat->Rebin(fNBinsClusterE,"histoClE_GaCl_matReb",fBinsClusterE);
        histoClE_GaCl_matReb->Divide(fDeltaPtCluster);
        TH1D* histoClE_GaClSubCh_mat    = (TH1D*) histoTMEffiInput->ProjectionX("histoClE_GaClSubCh_mat",16,16,"e");
        TH1D* histoClE_GaClSubCh_matReb = (TH1D*)histoClE_GaClSubCh_mat->Rebin(fNBinsClusterE,"histoClE_GaClSubCh_matReb",fBinsClusterE);
        histoClE_GaClSubCh_matReb->Divide(fDeltaPtCluster);
        TH1D* histoClE_ConvCl_mat       = (TH1D*) histoTMEffiInput->ProjectionX("histoClE_ConvCl_mat",17,17,"e");
        TH1D* histoClE_ConvCl_matReb    = (TH1D*)histoClE_ConvCl_mat->Rebin(fNBinsClusterE,"histoClE_ConvCl_matReb",fBinsClusterE);
        histoClE_ConvCl_matReb->Divide(fDeltaPtCluster);
        TH1D* histoClE_ConvCl_leadmat   = (TH1D*) histoTMEffiInput->ProjectionX("histoClE_ConvCl_leadmat",18,18,"e");
        TH1D* histoClE_ConvCl_leadmatReb= (TH1D*)histoClE_ConvCl_leadmat->Rebin(fNBinsClusterE,"histoClE_ConvCl_leadmatReb",fBinsClusterE);
        histoClE_ConvCl_leadmatReb->Divide(fDeltaPtCluster);
        TH1D* histoClE_ChPrimCl_mat     = (TH1D*) histoTMEffiInput->ProjectionX("histoClE_ChPrimCl_mat",19,19,"e");
        TH1D* histoClE_ChPrimCl_matReb  = (TH1D*)histoClE_ChPrimCl_mat->Rebin(fNBinsClusterE,"histoClE_ChPrimCl_matReb",fBinsClusterE);
        histoClE_ChPrimCl_matReb->Divide(fDeltaPtCluster);
        TH1D* histoClE_ChPrimCl_leadmat = (TH1D*) histoTMEffiInput->ProjectionX("histoClE_ChPrimCl_leadmat",20,20,"e");
        TH1D* histoClE_ChPrimCl_leadmatReb  = (TH1D*)histoClE_ChPrimCl_leadmat->Rebin(fNBinsClusterE,"histoClE_ChPrimCl_leadmatReb",fBinsClusterE);
        histoClE_ChPrimCl_leadmatReb->Divide(fDeltaPtCluster);
        TH1D* histoClE_ElCl_mat         = (TH1D*) histoTMEffiInput->ProjectionX("histoClE_ElCl_mat",21,21,"e");
        TH1D* histoClE_ElCl_matReb      = (TH1D*)histoClE_ElCl_mat->Rebin(fNBinsClusterE,"histoClE_ElCl_matReb",fBinsClusterE);
        histoClE_ElCl_matReb->Divide(fDeltaPtCluster);
        TH1D* histoClE_ElCl_leadmat     = (TH1D*) histoTMEffiInput->ProjectionX("histoClE_ElCl_leadmat",22,22,"e");
        TH1D* histoClE_ElCl_leadmatReb  = (TH1D*)histoClE_ElCl_leadmat->Rebin(fNBinsClusterE,"histoClE_ElCl_leadmatReb",fBinsClusterE);
        histoClE_ElCl_leadmatReb->Divide(fDeltaPtCluster);


        TH1D* histoClE_AllClwValTrack            = (TH1D*) histoTMEffiInput->ProjectionX("histoClE_AllClwValTrack",23,23,"e");
        TH1D* histoClE_AllClwValTrackReb         = (TH1D*)histoClE_AllClwValTrack->Rebin(fNBinsClusterE,"histoClE_AllClwValTrackReb",fBinsClusterE);
        histoClE_AllClwValTrackReb->Divide(fDeltaPtCluster);
        TH1D* histoClE_ChClwValTrack             = (TH1D*) histoTMEffiInput->ProjectionX("histoClE_ChClwValTrack",24,24,"e");
        TH1D* histoClE_ChClwValTrackReb          = (TH1D*)histoClE_ChClwValTrack->Rebin(fNBinsClusterE,"histoClE_ChClwValTrackReb",fBinsClusterE);
        histoClE_ChClwValTrackReb->Divide(fDeltaPtCluster);
        TH1D* histoClE_NeClwValTrack             = (TH1D*) histoTMEffiInput->ProjectionX("histoClE_NeClwValTrack",25,25,"e");
        TH1D* histoClE_NeClwValTrackReb          = (TH1D*)histoClE_NeClwValTrack->Rebin(fNBinsClusterE,"histoClE_NeClwValTrackReb",fBinsClusterE);
        histoClE_NeClwValTrackReb->Divide(fDeltaPtCluster);
        TH1D* histoClE_NeClSubChwValTrack        = (TH1D*) histoTMEffiInput->ProjectionX("histoClE_NeClSubChwValTrack",26,26,"e");
        TH1D* histoClE_NeClSubChwValTrackReb     = (TH1D*)histoClE_NeClSubChwValTrack->Rebin(fNBinsClusterE,"histoClE_NeClSubChwValTrackReb",fBinsClusterE);
        histoClE_NeClSubChwValTrackReb->Divide(fDeltaPtCluster);
        TH1D* histoClE_GaClwValTrack             = (TH1D*) histoTMEffiInput->ProjectionX("histoClE_GaClwValTrack",27,27,"e");
        TH1D* histoClE_GaClwValTrackReb          = (TH1D*)histoClE_GaClwValTrack->Rebin(fNBinsClusterE,"histoClE_GaClwValTrackReb",fBinsClusterE);
        histoClE_GaClwValTrackReb->Divide(fDeltaPtCluster);
        TH1D* histoClE_GaClSubChwValTrack        = (TH1D*) histoTMEffiInput->ProjectionX("histoClE_GaClSubChwValTrack",28,28,"e");
        TH1D* histoClE_GaClSubChwValTrackReb     = (TH1D*)histoClE_GaClSubChwValTrack->Rebin(fNBinsClusterE,"histoClE_GaClSubChwValTrackReb",fBinsClusterE);
        histoClE_GaClSubChwValTrackReb->Divide(fDeltaPtCluster);
        TH1D* histoClE_ConvClwValTrack           = (TH1D*) histoTMEffiInput->ProjectionX("histoClE_ConvClwValTrack",29,29,"e");
        TH1D* histoClE_ConvClwValTrackReb        = (TH1D*)histoClE_ConvClwValTrack->Rebin(fNBinsClusterE,"histoClE_ConvClwValTrackReb",fBinsClusterE);
        histoClE_ConvClwValTrackReb->Divide(fDeltaPtCluster);
        TH1D* histoClE_ChPrimClwValTrack         = (TH1D*) histoTMEffiInput->ProjectionX("histoClE_ChPrimClwValTrack",30,30,"e");
        TH1D* histoClE_ChPrimClwValTrackReb      = (TH1D*)histoClE_ChPrimClwValTrack->Rebin(fNBinsClusterE,"histoClE_ChPrimClwValTrackReb",fBinsClusterE);
        histoClE_ChPrimClwValTrackReb->Divide(fDeltaPtCluster);
        TH1D* histoClE_ElClwValTrack             = (TH1D*) histoTMEffiInput->ProjectionX("histoClE_ElClwValTrack",31,31,"e");
        TH1D* histoClE_ElClwValTrackReb          = (TH1D*)histoClE_ElClwValTrack->Rebin(fNBinsClusterE,"histoClE_ElClwValTrackReb",fBinsClusterE);
        histoClE_ElClwValTrackReb->Divide(fDeltaPtCluster);

        TH1D* histoEffi_AllCl           = (TH1D*)histoClE_AllCl_mat->Clone("histoEffi_AllCl");
        histoEffi_AllCl->Divide(histoEffi_AllCl, histoClE_AllCl, 1, 1, "B");
        TH1D* histoEffi_ChCl            = (TH1D*)histoClE_ChCl_mat->Clone("histoEffi_ChCl");
        histoEffi_ChCl->Divide(histoEffi_ChCl, histoClE_ChCl, 1, 1, "B");
        TH1D* histoEffi_ChPrimCl        = (TH1D*)histoClE_ChPrimCl_mat->Clone("histoEffi_ChPrimCl");
        histoEffi_ChPrimCl->Divide(histoEffi_ChPrimCl, histoClE_ChPrimCl, 1, 1, "B");
        TH1D* histoEffi_NeCl            = (TH1D*)histoClE_NeCl_mat->Clone("histoEffi_NeCl");
        histoEffi_NeCl->Divide(histoEffi_NeCl, histoClE_NeCl, 1, 1, "B");
        TH1D* histoEffi_NeSubChCl       = (TH1D*)histoClE_NeClSubCh_mat->Clone("histoEffi_NeSubChCl");
        histoEffi_NeSubChCl->Divide(histoEffi_NeSubChCl, histoClE_NeClSubCh, 1, 1, "B");
        TH1D* histoEffi_GaCl            = (TH1D*)histoClE_GaCl_mat->Clone("histoEffi_GaCl");
        histoEffi_GaCl->Divide(histoEffi_GaCl, histoClE_GaCl, 1, 1, "B");
        TH1D* histoEffi_GaSubChCl       = (TH1D*)histoClE_GaClSubCh_mat->Clone("histoEffi_GaSubChCl");
        histoEffi_GaSubChCl->Divide(histoEffi_GaSubChCl, histoClE_GaClSubCh, 1, 1, "B");
        TH1D* histoEffi_ConvCl          = (TH1D*)histoClE_ConvCl_mat->Clone("histoEffi_ConvCl");
        histoEffi_ConvCl->Divide(histoEffi_ConvCl, histoClE_ConvCl, 1, 1, "B");
        TH1D* histoEffi_ElCl            = (TH1D*)histoClE_ElCl_mat->Clone("histoEffi_ConvCl");
        histoEffi_ElCl->Divide(histoEffi_ElCl, histoClE_ElCl, 1, 1, "B");

        TH1D* histoEffi_AllClReb        = (TH1D*)histoClE_AllCl_matReb->Clone("histoEffi_AllClReb");
        histoEffi_AllClReb->Divide(histoEffi_AllClReb, histoClE_AllClReb, 1, 1, "B");
        TH1D* histoEffi_ChClReb         = (TH1D*)histoClE_ChCl_matReb->Clone("histoEffi_ChClReb");
        histoEffi_ChClReb->Divide(histoEffi_ChClReb, histoClE_ChClReb, 1, 1, "B");
        TH1D* histoEffi_ChPrimClReb     = (TH1D*)histoClE_ChPrimCl_matReb->Clone("histoEffi_ChPrimClReb");
        histoEffi_ChPrimClReb->Divide(histoEffi_ChPrimClReb, histoClE_ChPrimClReb, 1, 1, "B");
        TH1D* histoEffi_ChCl_leadReb    = (TH1D*)histoClE_ChCl_leadmatReb->Clone("histoEffi_ChClLeadReb");
        histoEffi_ChCl_leadReb->Divide(histoEffi_ChCl_leadReb, histoClE_ChClReb, 1, 1, "B");
        TH1D* histoEffi_ChPrimCl_leadReb= (TH1D*)histoClE_ChPrimCl_leadmatReb->Clone("histoEffi_ChPrimClLeadReb");
        histoEffi_ChPrimCl_leadReb->Divide(histoEffi_ChPrimCl_leadReb, histoClE_ChPrimClReb, 1, 1, "B");

        TH1D* histoEffi_NeClReb         = (TH1D*)histoClE_NeCl_matReb->Clone("histoEffi_NeClReb");
        histoEffi_NeClReb->Divide(histoEffi_NeClReb, histoClE_NeClReb, 1, 1, "B");
        TH1D* histoEffi_NeSubChClReb    = (TH1D*)histoClE_NeClSubCh_matReb->Clone("histoEffi_NeSubChClReb");
        histoEffi_NeSubChClReb->Divide(histoEffi_NeSubChClReb, histoClE_NeClSubChReb, 1, 1, "B");
        TH1D* histoEffi_GaClReb         = (TH1D*)histoClE_GaCl_matReb->Clone("histoEffi_GaClReb");
        histoEffi_GaClReb->Divide(histoEffi_GaClReb, histoClE_GaClReb, 1, 1, "B");
        TH1D* histoEffi_GaSubChClReb    = (TH1D*)histoClE_GaClSubCh_matReb->Clone("histoEffi_GaSubChClReb");
        histoEffi_GaSubChClReb->Divide(histoEffi_GaSubChClReb, histoClE_GaClSubChReb, 1, 1, "B");
        TH1D* histoEffi_ConvClReb       = (TH1D*)histoClE_ConvCl_matReb->Clone("histoEffi_ConvCl_leadReb");
        histoEffi_ConvClReb->Divide(histoEffi_ConvClReb, histoClE_ConvClReb, 1, 1, "B");
        TH1D* histoEffi_ConvCl_leadReb  = (TH1D*)histoClE_ConvCl_leadmatReb->Clone("histoEffi_ConvClReb");
        histoEffi_ConvCl_leadReb->Divide(histoEffi_ConvCl_leadReb, histoClE_ConvClReb, 1, 1, "B");
        TH1D* histoEffi_ElClReb         = (TH1D*)histoClE_ElCl_matReb->Clone("histoEffi_ConvClReb");
        histoEffi_ElClReb->Divide(histoEffi_ElClReb, histoClE_ElClReb, 1, 1, "B");
        TH1D* histoEffi_ElCl_leadReb    = (TH1D*)histoClE_ElCl_leadmatReb->Clone("histoEffi_ElCl_leadReb");
        histoEffi_ElCl_leadReb->Divide(histoEffi_ElCl_leadReb, histoClE_ElClReb, 1, 1, "B");


        TH1D* histoEffi_AllClwValTrackReb        = (TH1D*)histoClE_AllCl_matReb->Clone("histoEffi_AllClwValTrackReb");    // All Clusters
        histoEffi_AllClwValTrackReb->Divide(histoEffi_AllClwValTrackReb, histoClE_AllClwValTrackReb, 1, 1, "B");
        TH1D* histoEffi_ChClwValTrackReb         = (TH1D*)histoClE_ChCl_matReb->Clone("histoEffi_ChClwValTrackReb");      // Charged Clusters
        histoEffi_ChClwValTrackReb->Divide(histoEffi_ChClwValTrackReb, histoClE_ChClwValTrackReb, 1, 1, "B");
        TH1D* histoEffi_ChPrimClwValTrackReb     = (TH1D*)histoClE_ChPrimCl_matReb->Clone("histoEffi_ChPrimClwValTrackReb"); // Charged Clusters (primary)
        histoEffi_ChPrimClwValTrackReb->Divide(histoEffi_ChPrimClwValTrackReb, histoClE_ChPrimClwValTrackReb, 1, 1, "B");
        TH1D* histoEffi_ChClwValTrack_leadReb    = (TH1D*)histoClE_ChCl_leadmatReb->Clone("histoEffi_ChClwValTrackLeadReb"); // Charged Clusters (secondarys)
        histoEffi_ChClwValTrack_leadReb->Divide(histoEffi_ChClwValTrack_leadReb, histoClE_ChClwValTrackReb, 1, 1, "B");
        TH1D* histoEffi_ChPrimCl_leadwValTrackReb= (TH1D*)histoClE_ChPrimCl_leadmatReb->Clone("histoEffi_ChPrimClLeadwValTrackReb");
        histoEffi_ChPrimCl_leadwValTrackReb->Divide(histoEffi_ChPrimCl_leadwValTrackReb, histoClE_ChPrimClwValTrackReb, 1, 1, "B");
        TH1D* histoEffi_ElClwValTrackReb         = (TH1D*)histoClE_ElCl_matReb->Clone("histoEffi_ElClwValTrackReb");    // electron cluster
        histoEffi_ElClwValTrackReb->Divide(histoEffi_ElClwValTrackReb, histoClE_ElClwValTrackReb, 1, 1, "B");
        TH1D* histoEffi_ConvClwValTrackReb       = (TH1D*)histoClE_ConvCl_matReb->Clone("histoEffi_ConvClwValTrac_leadReb"); // conversion cluster
        histoEffi_ConvClwValTrackReb->Divide(histoEffi_ConvClwValTrackReb, histoClE_ConvClwValTrackReb, 1, 1, "B");

        TH1D* histoEffi_NeSubChClwValTrackReb    = (TH1D*)histoClE_NeClSubCh_matReb->Clone("histoEffi_NeSubChClwValTrackReb");
        histoEffi_NeSubChClwValTrackReb->Divide(histoEffi_NeSubChClwValTrackReb, histoClE_NeClSubChwValTrackReb, 1, 1, "B");
        TH1D* histoEffi_NeClwValTrackReb         = (TH1D*)histoClE_NeCl_matReb->Clone("histoEffi_NeClwValTrackReb");
        histoEffi_NeClwValTrackReb->Divide(histoEffi_NeClwValTrackReb, histoClE_NeClwValTrackReb, 1, 1, "B");
        TH1D* histoEffi_GaClwValTrackReb         = (TH1D*)histoClE_GaCl_matReb->Clone("histoEffi_GaClwValTrackReb");
        histoEffi_GaClwValTrackReb->Divide(histoEffi_GaClwValTrackReb, histoClE_GaClwValTrackReb, 1, 1, "B");
        TH1D* histoEffi_GaSubChClwValidTrackReb    = (TH1D*)histoClE_GaClSubCh_matReb->Clone("histoEffi_GaSubChClwValidTrackReb");
        histoEffi_GaSubChClwValidTrackReb->Divide(histoEffi_GaSubChClwValidTrackReb, histoClE_GaClSubChwValTrackReb, 1, 1, "B");

        // Plot TM efficiency for different cluster type
        TCanvas* canvasTMEffi = new TCanvas("canvasTMEffi","",200,10,1250,900);  // gives the page size
        DrawGammaCanvasSettings( canvasTMEffi, 0.08, 0.01, 0.01, 0.105);

        Double_t rangeTMEffi[2]     = {0., 1.39};

        Double_t maxPt              = fBinsClusterE[fNBinsClusterE];
        Double_t minPt              = 0;

        bool doLogX = false;
        if (nameTrigger.Contains("INT") || nameTrigger.Contains("MB")){
            maxPt                   = 15;
            if(optEnergy.Contains("13TeV")){
              maxPt                   = 120;
              minPt                   = 0.6;
              doLogX                  = true;
            }
        }

        if(doLogX) canvasTMEffi->SetLogx();
        TH2F * histo2DTMEff         = new TH2F("histo2DTMEff", "histo2DTMEff",1000, minPt, maxPt , 1000, rangeTMEffi[0], rangeTMEffi[1] );
        SetStyleHistoTH2ForGraphs(  histo2DTMEff, "#it{E} (GeV)", "#scale[1.2]{#varepsilon}_{#color[0]{i}veto}",
                                    0.85*0.05, 0.05, 0.85*0.05, 0.05, 0.95, 0.75);
        histo2DTMEff->GetYaxis()->SetLabelOffset(0.001);
        if(doLogX)histo2DTMEff->GetXaxis()->SetMoreLogLabels();
        histo2DTMEff->DrawCopy();


        DrawGammaSetMarker(histoEffi_ChClReb, 24, 1.5, kAzure+2, kAzure+2);
        histoEffi_ChClReb->DrawCopy("same,e1");
        DrawGammaSetMarker(histoEffi_ElClReb, 21, 1.5, kGreen-2, kGreen-2);
        histoEffi_ElClReb->DrawCopy("same,e1");
        DrawGammaSetMarker(histoEffi_ConvClReb, 25, 1.5, kGreen+2, kGreen+2);
        histoEffi_ConvClReb->DrawCopy("same,e1");
        DrawGammaSetMarker(histoEffi_ChPrimClReb, 24, 1.5, kBlue+1, kBlue+1);
        histoEffi_ChPrimClReb->DrawCopy("same,e1");

        TLegend* legendTMEffiCh           = GetAndSetLegend2(0.62, 0.95, 0.95, 0.95-(4*0.05),42);
        legendTMEffiCh->SetNColumns(2);
        legendTMEffiCh->SetTextAlign(32);
        legendTMEffiCh->AddEntry((TObject*)0,"conv #gamma cl","");
        legendTMEffiCh->AddEntry(histoEffi_ConvClReb,"","p");
        legendTMEffiCh->AddEntry((TObject*)0,"e^{#pm} cl","");
        legendTMEffiCh->AddEntry(histoEffi_ElClReb,"","p");
        legendTMEffiCh->AddEntry((TObject*)0,"other charged cl., prod. vtx R #leq 5 cm","");
        legendTMEffiCh->AddEntry(histoEffi_ChPrimClReb,"","p");
        legendTMEffiCh->AddEntry((TObject*)0,"other charged cl., prod. vtx R > 5 cm","");
        legendTMEffiCh->AddEntry(histoEffi_ChClReb,"","p");
        legendTMEffiCh->Draw();

        PutProcessLabelAndEnergyOnPlot(0.11, 0.95, 42, textALICE.Data(), collisionSystem.Data(), detectionProcess.Data(), 43, 0.035);

        canvasTMEffi->Update();
        canvasTMEffi->SaveAs(Form("%s/TMEffiDifferentChargedClusters_%s.%s",outputDirectory.Data(),clusterCuts[i].Data(),suffix.Data()));

        histo2DTMEff->DrawCopy();

        DrawGammaSetMarker(histoEffi_ChClwValTrackReb, 24, 1.5, kAzure+2, kAzure+2);
        histoEffi_ChClwValTrackReb->DrawCopy("same,e1");
        DrawGammaSetMarker(histoEffi_ElClwValTrackReb, 21, 1.5, kGreen-2, kGreen-2);
        histoEffi_ElClwValTrackReb->DrawCopy("same,e1");
        DrawGammaSetMarker(histoEffi_ConvClwValTrackReb, 25, 1.5, kGreen+2, kGreen+2);
        histoEffi_ConvClwValTrackReb->DrawCopy("same,e1");
        DrawGammaSetMarker(histoEffi_ChPrimClwValTrackReb, 24, 1.5, kBlue+1, kBlue+1);
        histoEffi_ChPrimClwValTrackReb->DrawCopy("same,e1");


        TLegend* legendTMEffiChwValTrack           = GetAndSetLegend2(0.62, 0.95, 0.95, 0.95-(4*0.05),42);
        legendTMEffiChwValTrack->SetNColumns(2);
        legendTMEffiChwValTrack->SetTextAlign(32);
        legendTMEffiChwValTrack->AddEntry((TObject*)0,"conv #gamma cl","");
        legendTMEffiChwValTrack->AddEntry(histoEffi_ConvClwValTrackReb,"","p");
        legendTMEffiChwValTrack->AddEntry((TObject*)0,"e^{#pm} cl","");
        legendTMEffiChwValTrack->AddEntry(histoEffi_ElClwValTrackReb,"","p");
        legendTMEffiChwValTrack->AddEntry((TObject*)0,"other charged cl., prod. vtx R #leq 5 cm","");
        legendTMEffiChwValTrack->AddEntry(histoEffi_ChPrimClwValTrackReb,"","p");
        legendTMEffiChwValTrack->AddEntry((TObject*)0,"other charged cl., prod. vtx R > 5 cm","");
        legendTMEffiChwValTrack->AddEntry(histoEffi_ChClwValTrackReb,"","p");
        legendTMEffiChwValTrack->Draw();

        PutProcessLabelAndEnergyOnPlot(0.11, 0.95, 42, textALICE.Data(),  collisionSystem.Data(), detectionProcess.Data(), 43, 0.035);

        canvasTMEffi->Update();
        canvasTMEffi->SaveAs(Form("%s/TMEffiDifferentChargedClusters_withValidTrack_%s.%s",outputDirectory.Data(),clusterCuts[i].Data(),suffix.Data()));

        histo2DTMEff->DrawCopy();

        DrawGammaSetMarker(histoEffi_ChCl_leadReb, 24, 1.5, kAzure+2, kAzure+2);
        histoEffi_ChCl_leadReb->DrawCopy("same,e1");
        DrawGammaSetMarker(histoEffi_ElCl_leadReb, 21, 1.5, kGreen-2, kGreen-2);
        histoEffi_ElCl_leadReb->DrawCopy("same,e1");
        DrawGammaSetMarker(histoEffi_ConvCl_leadReb, 25, 1.5, kGreen+2, kGreen+2);
        histoEffi_ConvCl_leadReb->DrawCopy("same,e1");
        DrawGammaSetMarker(histoEffi_ChPrimCl_leadReb, 24, 1.5, kBlue+1, kBlue+1);
        histoEffi_ChPrimCl_leadReb->DrawCopy("same,e1");

        TLegend* legendTMEffiChlead           = GetAndSetLegend2(0.62, 0.95, 0.95, 0.95-(4*0.05),42);
        legendTMEffiChlead->SetNColumns(2);
        legendTMEffiChlead->SetTextAlign(32);
        legendTMEffiChlead->AddEntry((TObject*)0,"conv #gamma cl","");
        legendTMEffiChlead->AddEntry(histoEffi_ConvCl_leadReb,"","p");
        legendTMEffiChlead->AddEntry((TObject*)0,"e^{#pm} cl","");
        legendTMEffiChlead->AddEntry(histoEffi_ElCl_leadReb,"","p");
        legendTMEffiChlead->AddEntry((TObject*)0,"h^{#pm} cl., prod. vtx R #leq 5 cm","");
        legendTMEffiChlead->AddEntry(histoEffi_ChPrimCl_leadReb,"","p");
        legendTMEffiChlead->AddEntry((TObject*)0,"h^{#pm} cl., prod. vtx R > 5 cm","");
        legendTMEffiChlead->AddEntry(histoEffi_ChCl_leadReb,"","p");
        legendTMEffiChlead->Draw();

        PutProcessLabelAndEnergyOnPlot(0.11, 0.95, 42, textALICE.Data(), collisionSystem.Data(), "matched w/ lead h^{#pm} of cl.",  43, 0.035);
        PutProcessLabelAndEnergyOnPlot(0.11, 0.95 - 3.7*0.035, 42, detectionProcess.Data(), "" , "" ,  43, 0.035);

        canvasTMEffi->Update();
        canvasTMEffi->SaveAs(Form("%s/TMEffiDifferentChargedClustersMatchedWithLead_%s.%s",outputDirectory.Data(),clusterCuts[i].Data(),suffix.Data()));


        TH2F * histo2DTMEffNe        = new TH2F("histo2DTMEffNe", "histo2DTMEffNe",1000, minPt, maxPt , 1000, rangeTMEffi[0], rangeTMEffi[1] );
        SetStyleHistoTH2ForGraphs(  histo2DTMEffNe, "#it{E} (GeV)", "#scale[1.2]{#varepsilon}_{#color[0]{i}fake veto}",
                                    0.85*0.05, 0.05, 0.85*0.05, 0.05, 0.95, 0.75);
        histo2DTMEffNe->GetYaxis()->SetLabelOffset(0.001);
        if(doLogX)histo2DTMEffNe->GetXaxis()->SetMoreLogLabels();
        histo2DTMEffNe->DrawCopy();
        DrawGammaSetMarker(histoEffi_NeSubChClReb, 20, 1.5, kRed-6, kRed-6);
        histoEffi_NeSubChClReb->DrawCopy("same,e1");
        DrawGammaSetMarker(histoEffi_NeClReb, 24, 1.5, kRed+2, kRed+2);
        histoEffi_NeClReb->DrawCopy("same,e1");
        DrawGammaSetMarker(histoEffi_GaClReb, 25, 1.5, kOrange, kOrange);
        histoEffi_GaClReb->DrawCopy("same,e1");
        DrawGammaSetMarker(histoEffi_GaSubChClReb, 21, 1.5, 807, 807);
        histoEffi_GaSubChClReb->DrawCopy("same,e1");



        TLegend* legendTMEffiNe           = GetAndSetLegend2(0.62, 0.95, 0.95, 0.95-(4*0.05),42);
        legendTMEffiNe->SetNColumns(2);
        legendTMEffiNe->SetTextAlign(32);
        legendTMEffiNe->AddEntry((TObject*)0,"#gamma cl.","");
        legendTMEffiNe->AddEntry(histoEffi_GaClReb,"","p");
        legendTMEffiNe->AddEntry((TObject*)0,"#gamma cl. w/ ch. cont","");
        legendTMEffiNe->AddEntry(histoEffi_GaSubChClReb,"","p");
        legendTMEffiNe->AddEntry((TObject*)0,"other neutral cl.","");
        legendTMEffiNe->AddEntry(histoEffi_NeClReb,"", "p");
        legendTMEffiNe->AddEntry((TObject*)0,"other neutral cl. w/ ch. cont.","");
        legendTMEffiNe->AddEntry(histoEffi_NeSubChClReb,"", "p");
        legendTMEffiNe->Draw();

        PutProcessLabelAndEnergyOnPlot(0.11, 0.95, 42, textALICE.Data(), collisionSystem.Data(), detectionProcess.Data(), 43, 0.035);

        canvasTMEffi->Update();
        canvasTMEffi->SaveAs(Form("%s/TMEffiDifferentNeutralClusters_%s.%s",outputDirectory.Data(),clusterCuts[i].Data(),suffix.Data()));


        // For the neutral would it be better to plot neutrals that do not contain a track??
        SetStyleHistoTH2ForGraphs(  histo2DTMEffNe, "#it{E} (GeV)", "#scale[1.2]{#varepsilon}_{#color[0]{i}fake veto}",
                                    0.85*0.05, 0.05, 0.85*0.05, 0.05, 0.95, 0.75);
        histo2DTMEffNe->GetYaxis()->SetLabelOffset(0.001);
        histo2DTMEffNe->DrawCopy();
        DrawGammaSetMarker(histoEffi_NeSubChClwValTrackReb, 20, 1.5, kRed-6, kRed-6);
        histoEffi_NeSubChClwValTrackReb->DrawCopy("same,e1");
        DrawGammaSetMarker(histoEffi_NeClwValTrackReb, 24, 1.5, kRed+2, kRed+2);
        histoEffi_NeClwValTrackReb->DrawCopy("same,e1");
        DrawGammaSetMarker(histoEffi_GaClwValTrackReb, 25, 1.5, kOrange, kOrange);
        histoEffi_GaClwValTrackReb->DrawCopy("same,e1");
        DrawGammaSetMarker(histoEffi_GaSubChClwValidTrackReb, 21, 1.5, 807, 807);
        histoEffi_GaSubChClwValidTrackReb->DrawCopy("same,e1");



        TLegend* legendTMEffiNe2           = GetAndSetLegend2(0.62, 0.95, 0.95, 0.95-(4*0.05),42);
        legendTMEffiNe2->SetNColumns(2);
        legendTMEffiNe2->SetTextAlign(32);
        legendTMEffiNe2->AddEntry((TObject*)0,"#gamma cl.","");
        legendTMEffiNe2->AddEntry(histoEffi_GaClwValTrackReb,"","p");
        legendTMEffiNe2->AddEntry((TObject*)0,"#gamma cl. w/ ch. cont","");
        legendTMEffiNe2->AddEntry(histoEffi_GaSubChClwValidTrackReb,"","p");
        legendTMEffiNe2->AddEntry((TObject*)0,"other neutral cl.","");
        legendTMEffiNe2->AddEntry(histoEffi_NeClwValTrackReb,"", "p");
        legendTMEffiNe2->AddEntry((TObject*)0,"other neutral cl. w/ ch. cont.","");
        legendTMEffiNe2->AddEntry(histoEffi_NeSubChClwValTrackReb,"", "p");
        legendTMEffiNe2->Draw();

        PutProcessLabelAndEnergyOnPlot(0.11, 0.95, 42, textALICE.Data(), collisionSystem.Data(), detectionProcess.Data(), 43, 0.035);

    canvasTMEffi->Update();
    canvasTMEffi->SaveAs(Form("%s/TMEffiDifferentNeutralClusters_withValidTrack_%s.%s",outputDirectory.Data(),clusterCuts[i].Data(),suffix.Data()));

        TH2F * histFrac             = new TH2F("histFrac", "histFrac",1000, minPt, maxPt , 1000, 0, rangeTMEffi[1] );
        SetStyleHistoTH2ForGraphs(  histFrac, "#it{E} (GeV)", "N_{id}/N_{all}",
                                    0.85*0.05, 0.05, 0.85*0.05, 0.05, 0.95, 0.94);
        histFrac->GetYaxis()->SetLabelOffset(0.001);
        if(doLogX)histFrac->GetXaxis()->SetMoreLogLabels();
        histFrac->DrawCopy();

        DrawGammaSetMarker(histoRatioClE_ChtoAllReb, 24, 1.5, kAzure+2, kAzure+2);
        histoRatioClE_ChtoAllReb->DrawCopy("same,e1");
        DrawGammaSetMarker(histoRatioClE_EltoAllReb, 21, 1.5, kGreen-2, kGreen-2);
        histoRatioClE_EltoAllReb->DrawCopy("same,e1");
        DrawGammaSetMarker(histoRatioClE_ConvtoAllReb, 25, 1.5, kGreen+2, kGreen+2);
        histoRatioClE_ConvtoAllReb->DrawCopy("same,e1");
        DrawGammaSetMarker(histoRatioClE_ChPrimtoAllReb, 24, 1.5, kBlue+1, kBlue+1);
        histoRatioClE_ChPrimtoAllReb->DrawCopy("same,e1");

        DrawGammaSetMarker(histoRatioClE_NeSubChtoAllReb, 20, 1.5, kRed-6, kRed-6);
        histoRatioClE_NeSubChtoAllReb->DrawCopy("same,e1");
        DrawGammaSetMarker(histoRatioClE_NetoAllReb, 24, 1.5, kRed+2, kRed+2);
        histoRatioClE_NetoAllReb->DrawCopy("same,e1");
        DrawGammaSetMarker(histoRatioClE_GatoAllReb, 25, 1.5, kOrange, kOrange);
        histoRatioClE_GatoAllReb->DrawCopy("same,e1");
        DrawGammaSetMarker(histoRatioClE_GaSubChtoAllReb, 21, 1.5, 807, 807);
        histoRatioClE_GaSubChtoAllReb->DrawCopy("same,e1");

        TLegend* legendRatio           = GetAndSetLegend2(0.62, 0.95, 0.95, 0.95-(8*0.05),42);
        legendRatio->SetNColumns(2);
        legendRatio->SetTextAlign(32);
        legendRatio->AddEntry((TObject*)0,"conv #gamma cl","");
        legendRatio->AddEntry(histoEffi_ConvClReb,"","p");
        legendRatio->AddEntry((TObject*)0,"e^{#pm} cl","");
        legendRatio->AddEntry(histoEffi_ElClReb,"","p");
        legendRatio->AddEntry((TObject*)0,"other charged cl., prod. vtx R #leq 5 cm","");
        legendRatio->AddEntry(histoEffi_ChPrimClReb,"","p");
        legendRatio->AddEntry((TObject*)0,"other charged cl., prod. vtx R > 5 cm","");
        legendRatio->AddEntry(histoEffi_ChClReb,"","p");
        legendRatio->AddEntry((TObject*)0,"#gamma cl.","");
        legendRatio->AddEntry(histoEffi_GaClReb,"","p");
        legendRatio->AddEntry((TObject*)0,"#gamma cl. w/ ch. cont","");
        legendRatio->AddEntry(histoEffi_GaSubChClReb,"","p");
        legendRatio->AddEntry((TObject*)0,"other neutral cl.","");
        legendRatio->AddEntry(histoEffi_NeClReb,"","p");
        legendRatio->AddEntry((TObject*)0,"other neutral cl. w/ ch. cont.","");
        legendRatio->AddEntry(histoEffi_NeSubChClReb,"","p");
        legendRatio->Draw();

        PutProcessLabelAndEnergyOnPlot(0.11, 0.95, 42, textALICE.Data(), collisionSystem.Data(), detectionProcess.Data(),  43, 0.035);

        canvasTMEffi->Update();
        canvasTMEffi->SaveAs(Form("%s/TMFractioOfClusters_%s.%s",outputDirectory.Data(),clusterCuts[i].Data(),suffix.Data()));

        if(i == 0){
          TFile fOut(Form("%s/TMEffi.root", outputDirectory.Data()), "Recreate");
          fOut.cd();
          histoEffi_ConvClReb->Write("histoEffi_ConvClReb");
          histoEffi_ElClReb->Write("histoEffi_ElClReb");
          histoEffi_ChPrimClReb->Write("histoEffi_ChPrimClReb");
          histoEffi_ChClReb->Write("histoEffi_ChClReb");

          histoEffi_GaClReb->Write("histoEffi_GaClReb");
          histoEffi_GaSubChClReb->Write("histoEffi_GaSubChClReb");
          histoEffi_NeClReb->Write("histoEffi_NeClReb");
          histoEffi_NeSubChClReb->Write("histoEffi_NeSubChClReb");

          histoEffi_ConvClReb->Write("histoEffi_ConvClReb");
          histoEffi_ElClReb->Write("histoEffi_ElClReb");
          histoEffi_ChPrimClReb->Write("histoEffi_ChPrimClReb");
          histoEffi_ChClReb->Write("histoEffi_ChClReb");
          histoEffi_GaClReb->Write("histoEffi_GaClReb");
          histoEffi_GaSubChClReb->Write("histoEffi_GaSubChClReb");
          histoEffi_NeClReb->Write("histoEffi_NeClReb");
          histoEffi_NeSubChClReb->Write("histoEffi_NeSubChClReb");
          fOut.Close();
        }
    }


    return;

}
