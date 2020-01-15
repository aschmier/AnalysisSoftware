

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
#include "TH3F.h"
#include "TF1.h"
#include "TVirtualFitter.h"
#include "TObject.h"
#include "TCanvas.h"
#include "TMultiGraph.h"
#include "TLegend.h"
#include "TTree.h"
#include "TMinuit.h"
#include "TLatex.h"
#include "TMath.h"
#include "TPostScript.h"
#include "TGraphErrors.h"
#include "TArrow.h"
#include "TGraphAsymmErrors.h"
#include "TGaxis.h"
#include "TMarker.h"
#include "TVectorT.h"
#include "TArc.h"
#include "../CommonHeaders/PlottingGammaConversionHistos.h"
#include "../CommonHeaders/PlottingGammaConversionAdditional.h"
#include "../CommonHeaders/FittingGammaConversion.h"
#include "../CommonHeaders/ConversionFunctionsBasicsAndLabeling.h"
#include "../CommonHeaders/ConversionFunctions.h"
#include "../CommonHeaders/CombinationFunctions.h"

 

void MakeAcceptanceFromKinematicsV2(
    TString fileName                = "LHC18b8_fastandwoSDD_AnalysisResults_Calo.root",
    TString meson                  = "Pi0",
    Long64_t maxNumPartProcess      = -1,
    TString energy                  = "8TeV"
){
    // ********************************************************************************
    // *            Definition of Cuts                                                *
    // ********************************************************************************
    TString suffix          = "pdf";

    gROOT->Reset();
    gROOT->SetStyle("Plain");

    StyleSettingsThesis();
    SetPlotStyle();
    TString dateForOutput                           = ReturnDateStringForOutput();
    cout << dateForOutput.Data() << endl;
    TString collisionSystem         = ReturnFullCollisionsSystem(energy);
    TString outputDir                               = Form("%s/%s/MakeAcceptanceFromKinematics",suffix.Data(),dateForOutput.Data());
    cout << outputDir.Data() << endl;

    gSystem->Exec("mkdir -p "+outputDir);
    // ********************************************************************************
    // *            File definition/ loading                                          *
    // ********************************************************************************

    TFile *f                        = (TFile*)gROOT->GetListOfFiles()->FindObject(fileName.Data());
    if (!f) {
        f                           = new TFile(fileName.Data());
    }
    if (!f) cout << "main List not found" << endl;
    if (f->IsZombie()) {
        cout <<fileName.Data() <<" file does not exist" << endl;
        f->Close();
        delete f;
        return;
    }


    //Declaration of leaves types

    TTree *AcceptanceTree = (TTree*)f->Get("MotherDecayTree");
    if(!AcceptanceTree){ cout << "tree not found... returning!"<< endl; return;}

    if(maxNumPartProcess>0) AcceptanceTree->SetEntries(maxNumPartProcess);
    Float_t         fBuffer_MotherPt;
    Float_t         fBuffer_MotherE;
    Float_t         fBuffer_MotherEta;
    Float_t         fBuffer_MotherY;
    Float_t         fBuffer_MotherPhi;

    Float_t         fBuffer_Daughter1Pt;
    Float_t         fBuffer_Daughter1E;
    Float_t         fBuffer_Daughter1Eta;
    Float_t         fBuffer_Daughter1Phi;

    Float_t         fBuffer_Daughter2Pt;
    Float_t         fBuffer_Daughter2E;
    Float_t         fBuffer_Daughter2Eta;
    Float_t         fBuffer_Daughter2Phi;

    // Set branch adresses
    AcceptanceTree->SetBranchAddress("MotherPt",                  &fBuffer_MotherPt);
    AcceptanceTree->SetBranchAddress("MotherE",                   &fBuffer_MotherE);
    AcceptanceTree->SetBranchAddress("MotherEta",                 &fBuffer_MotherEta);
    AcceptanceTree->SetBranchAddress("MotherY",                   &fBuffer_MotherY);
    AcceptanceTree->SetBranchAddress("MotherPhi",                 &fBuffer_MotherPhi);

    AcceptanceTree->SetBranchAddress("Daughter1Pt",                  &fBuffer_Daughter1Pt);
    AcceptanceTree->SetBranchAddress("Daughter1E",                   &fBuffer_Daughter1E);
    AcceptanceTree->SetBranchAddress("Daughter1Eta",                 &fBuffer_Daughter1Eta);
    AcceptanceTree->SetBranchAddress("Daughter1Phi",                 &fBuffer_Daughter1Phi);

    AcceptanceTree->SetBranchAddress("Daughter2Pt",                  &fBuffer_Daughter2Pt);
    AcceptanceTree->SetBranchAddress("Daughter2E",                   &fBuffer_Daughter2E);
    AcceptanceTree->SetBranchAddress("Daughter2Eta",                 &fBuffer_Daughter2Eta);
    AcceptanceTree->SetBranchAddress("Daughter2Phi",                 &fBuffer_Daughter2Phi);
    // ********************************************************************************
    // *            Definition of Boundaries for Histograms                           *
    // ********************************************************************************

    //Pt-plots
    Int_t nBinsPt                    = 5000;
    Double_t firstBinPt              = 0.0;
    Double_t lastBinPt               = 250.;

    // ********************************************************************************
    // *      Definition of histograms for reconstructed Conversion Points            *
    // ********************************************************************************
    TH1F* histoAcceptance_09_PCM       = new TH1F(Form("histoAcceptance_09_%s_PCM", meson.Data()),       Form("histoAcceptance_09_%s_PCM", meson.Data()),nBinsPt, firstBinPt, lastBinPt);
    TH1F* histoAcceptance_09_PCMEDC    = new TH1F(Form("histoAcceptance_09_%s_PCMEDC", meson.Data()),    Form("histoAcceptance_09_%s_PCMEDC", meson.Data()),nBinsPt, firstBinPt, lastBinPt);
    TH1F* histoAcceptance_09_PCMEMCR1  = new TH1F(Form("histoAcceptance_09_%s_PCMEMCR1", meson.Data()),    Form("histoAcceptance_09_%s_PCMEMCR1", meson.Data()),nBinsPt, firstBinPt, lastBinPt);
    TH1F* histoAcceptance_09_PCMEMC    = new TH1F(Form("histoAcceptance_09_%s_PCMEMC", meson.Data()),    Form("histoAcceptance_09_%s_PCMEMC", meson.Data()),nBinsPt, firstBinPt, lastBinPt);
    TH1F* histoAcceptance_09_PCMDMC    = new TH1F(Form("histoAcceptance_09_%s_PCMDMC", meson.Data()),    Form("histoAcceptance_09_%s_PCMDMC", meson.Data()),nBinsPt, firstBinPt, lastBinPt);
    TH1F* histoAcceptance_09_PCMPHOS   = new TH1F(Form("histoAcceptance_09_%s_PCMPHOS", meson.Data()),   Form("histoAcceptance_09_%s_PCMPHOS", meson.Data()),nBinsPt, firstBinPt, lastBinPt);
    TH1F* histoAcceptance_09_PCMPHOSR2 = new TH1F(Form("histoAcceptance_09_%s_PCMPHOSR2", meson.Data()), Form("histoAcceptance_09_%s_PCMPHOSR2", meson.Data()),nBinsPt, firstBinPt, lastBinPt);
    TH1F* histoAcceptance_08_PCM       = new TH1F(Form("histoAcceptance_08_%s_PCM", meson.Data()),       Form("histoAcceptance_08_%s_PCM", meson.Data()),nBinsPt, firstBinPt, lastBinPt);
    TH1F* histoAcceptance_08_PCMEDC    = new TH1F(Form("histoAcceptance_08_%s_PCMEDC", meson.Data()),    Form("histoAcceptance_08_%s_PCMEDC", meson.Data()),nBinsPt, firstBinPt, lastBinPt);
    TH1F* histoAcceptance_08_PCMEMCR1  = new TH1F(Form("histoAcceptance_08_%s_PCMEMCR1", meson.Data()),    Form("histoAcceptance_08_%s_PCMEMCR1", meson.Data()),nBinsPt, firstBinPt, lastBinPt);
    TH1F* histoAcceptance_08_PCMEMC    = new TH1F(Form("histoAcceptance_08_%s_PCMEMC", meson.Data()),    Form("histoAcceptance_08_%s_PCMEMC", meson.Data()),nBinsPt, firstBinPt, lastBinPt);
    TH1F* histoAcceptance_08_PCMDMC    = new TH1F(Form("histoAcceptance_08_%s_PCMDMC", meson.Data()),    Form("histoAcceptance_08_%s_PCMDMC", meson.Data()),nBinsPt, firstBinPt, lastBinPt);
    TH1F* histoAcceptance_08_PCMPHOS   = new TH1F(Form("histoAcceptance_08_%s_PCMPHOS", meson.Data()),   Form("histoAcceptance_08_%s_PCMPHOS", meson.Data()),nBinsPt, firstBinPt, lastBinPt);
    TH1F* histoAcceptance_08_PCMPHOSR2 = new TH1F(Form("histoAcceptance_08_%s_PCMPHOSR2", meson.Data()), Form("histoAcceptance_08_%s_PCMPHOSR2", meson.Data()),nBinsPt, firstBinPt, lastBinPt);
    TH1F* histoAcceptance_EDC       = new TH1F(Form("histoAcceptance_%s_EDC", meson.Data()),       Form("histoAcceptance_%s_EDC", meson.Data()),nBinsPt, firstBinPt, lastBinPt);
    TH1F* histoAcceptance_EMCR1     = new TH1F(Form("histoAcceptance_%s_EMCR1", meson.Data()),       Form("histoAcceptance_%s_EMCR1", meson.Data()),nBinsPt, firstBinPt, lastBinPt);
    TH1F* histoAcceptance_mEMC      = new TH1F(Form("histoAcceptance_%s_mEMC", meson.Data()),       Form("histoAcceptance_%s_mEMC", meson.Data()),nBinsPt, firstBinPt, lastBinPt);
    TH1F* histoAcceptance_mDMC      = new TH1F(Form("histoAcceptance_%s_mDMC", meson.Data()),       Form("histoAcceptance_%s_mDMC", meson.Data()),nBinsPt, firstBinPt, lastBinPt);
    TH1F* histoAcceptance_mEDC      = new TH1F(Form("histoAcceptance_%s_mEDC", meson.Data()),       Form("histoAcceptance_%s_mEDC", meson.Data()),nBinsPt, firstBinPt, lastBinPt);
    TH1F* histoAcceptance_EMC       = new TH1F(Form("histoAcceptance_%s_EMC", meson.Data()),       Form("histoAcceptance_%s_EMC", meson.Data()),nBinsPt, firstBinPt, lastBinPt);
    TH1F* histoAcceptance_DMC       = new TH1F(Form("histoAcceptance_%s_DMC", meson.Data()),       Form("histoAcceptance_%s_DMC", meson.Data()),nBinsPt, firstBinPt, lastBinPt);
    TH1F* histoAcceptance_PHOS      = new TH1F(Form("histoAcceptance_%s_PHOS", meson.Data()),      Form("histoAcceptance_%s_PHOS", meson.Data()),nBinsPt, firstBinPt, lastBinPt);
    TH1F* histoAcceptance_PHOSR2    = new TH1F(Form("histoAcceptance_%s_PHOSR2", meson.Data()),    Form("histoAcceptance_%s_PHOSR2", meson.Data()),nBinsPt, firstBinPt, lastBinPt);

    TH1F* histoGenerated_09_PCM       = new TH1F(Form("histoGenerated_09_%s_PCM", meson.Data()),       Form("histoGenerated_09_%s_PCM", meson.Data()),nBinsPt, firstBinPt, lastBinPt);
    TH1F* histoGenerated_09_PCMEDC    = new TH1F(Form("histoGenerated_09_%s_PCMEDC", meson.Data()),    Form("histoGenerated_09_%s_PCMEDC", meson.Data()),nBinsPt, firstBinPt, lastBinPt);
    TH1F* histoGenerated_09_PCMEMCR1  = new TH1F(Form("histoGenerated_09_%s_PCMEMCR1", meson.Data()),    Form("histoGenerated_09_%s_PCMEMCR1", meson.Data()),nBinsPt, firstBinPt, lastBinPt);
    TH1F* histoGenerated_09_PCMEMC    = new TH1F(Form("histoGenerated_09_%s_PCMEMC", meson.Data()),    Form("histoGenerated_09_%s_PCMEMC", meson.Data()),nBinsPt, firstBinPt, lastBinPt);
    TH1F* histoGenerated_09_PCMDMC    = new TH1F(Form("histoGenerated_09_%s_PCMDMC", meson.Data()),    Form("histoGenerated_09_%s_PCMDMC", meson.Data()),nBinsPt, firstBinPt, lastBinPt);
    TH1F* histoGenerated_09_PCMPHOS   = new TH1F(Form("histoGenerated_09_%s_PCMPHOS", meson.Data()),   Form("histoGenerated_09_%s_PCMPHOS", meson.Data()),nBinsPt, firstBinPt, lastBinPt);
    TH1F* histoGenerated_09_PCMPHOSR2 = new TH1F(Form("histoGenerated_09_%s_PCMPHOSR2", meson.Data()), Form("histoGenerated_09_%s_PCMPHOSR2", meson.Data()),nBinsPt, firstBinPt, lastBinPt);
    TH1F* histoGenerated_08_PCM       = new TH1F(Form("histoGenerated_08_%s_PCM", meson.Data()),       Form("histoGenerated_08_%s_PCM", meson.Data()),nBinsPt, firstBinPt, lastBinPt);
    TH1F* histoGenerated_08_PCMEDC    = new TH1F(Form("histoGenerated_08_%s_PCMEDC", meson.Data()),    Form("histoGenerated_08_%s_PCMEDC", meson.Data()),nBinsPt, firstBinPt, lastBinPt);
    TH1F* histoGenerated_08_PCMEMCR1  = new TH1F(Form("histoGenerated_08_%s_PCMEMCR1", meson.Data()),    Form("histoGenerated_08_%s_PCMEMCR1", meson.Data()),nBinsPt, firstBinPt, lastBinPt);
    TH1F* histoGenerated_08_PCMEMC    = new TH1F(Form("histoGenerated_08_%s_PCMEMC", meson.Data()),    Form("histoGenerated_08_%s_PCMEMC", meson.Data()),nBinsPt, firstBinPt, lastBinPt);
    TH1F* histoGenerated_08_PCMDMC    = new TH1F(Form("histoGenerated_08_%s_PCMDMC", meson.Data()),    Form("histoGenerated_08_%s_PCMDMC", meson.Data()),nBinsPt, firstBinPt, lastBinPt);
    TH1F* histoGenerated_08_PCMPHOS   = new TH1F(Form("histoGenerated_08_%s_PCMPHOS", meson.Data()),   Form("histoGenerated_08_%s_PCMPHOS", meson.Data()),nBinsPt, firstBinPt, lastBinPt);
    TH1F* histoGenerated_08_PCMPHOSR2 = new TH1F(Form("histoGenerated_08_%s_PCMPHOSR2", meson.Data()), Form("histoGenerated_08_%s_PCMPHOSR2", meson.Data()),nBinsPt, firstBinPt, lastBinPt);
    TH1F* histoGenerated_EDC       = new TH1F(Form("histoGenerated_%s_EDC", meson.Data()),       Form("histoGenerated_%s_EDC", meson.Data()),nBinsPt, firstBinPt, lastBinPt);
    TH1F* histoGenerated_EMCR1     = new TH1F(Form("histoGenerated_%s_EMCR1", meson.Data()),       Form("histoGenerated_%s_EMCR1", meson.Data()),nBinsPt, firstBinPt, lastBinPt);
    TH1F* histoGenerated_mEMC      = new TH1F(Form("histoGenerated_%s_mEMC", meson.Data()),       Form("histoGenerated_%s_mEMC", meson.Data()),nBinsPt, firstBinPt, lastBinPt);
    TH1F* histoGenerated_mDMC      = new TH1F(Form("histoGenerated_%s_mDMC", meson.Data()),       Form("histoGenerated_%s_mDMC", meson.Data()),nBinsPt, firstBinPt, lastBinPt);
    TH1F* histoGenerated_mEDC      = new TH1F(Form("histoGenerated_%s_mEDC", meson.Data()),       Form("histoGenerated_%s_mEDC", meson.Data()),nBinsPt, firstBinPt, lastBinPt);
    TH1F* histoGenerated_EMC       = new TH1F(Form("histoGenerated_%s_EMC", meson.Data()),       Form("histoGenerated_%s_EMC", meson.Data()),nBinsPt, firstBinPt, lastBinPt);
    TH1F* histoGenerated_DMC       = new TH1F(Form("histoGenerated_%s_DMC", meson.Data()),       Form("histoGenerated_%s_DMC", meson.Data()),nBinsPt, firstBinPt, lastBinPt);
    TH1F* histoGenerated_PHOS      = new TH1F(Form("histoGenerated_%s_PHOS", meson.Data()),      Form("histoGenerated_%s_PHOS", meson.Data()),nBinsPt, firstBinPt, lastBinPt);
    TH1F* histoGenerated_PHOSR2    = new TH1F(Form("histoGenerated_%s_PHOSR2", meson.Data()),    Form("histoGenerated_%s_PHOSR2", meson.Data()),nBinsPt, firstBinPt, lastBinPt);

    TH2F* histoGammaStudyRvsMesonPt   = new TH2F("histoGammaStudy1","histoGammaStudy1",nBinsPt, firstBinPt, lastBinPt,1000,0,1);
    TH2F* histoGammaStudyEtavsMesonPt   = new TH2F("histoGammaStudy2","histoGammaStudy2",nBinsPt, firstBinPt, lastBinPt,1000,0,5);
    TH2F* histoGammaStudyPhivsMesonPt   = new TH2F("histoGammaStudy3","histoGammaStudy3",nBinsPt, firstBinPt, lastBinPt,1000,0,TMath::Pi());

    // ********************************************************************************
    // *      Reading of Tree with reconstructed gammas/ filling of histograms        *
    // ********************************************************************************
    Double_t cutEtaMinPCM = -0.9;
    Double_t cutEtaMaxPCM = 0.9;
    Double_t cutEtaMinPCM2 = -0.8;
    Double_t cutEtaMaxPCM2 = 0.8;

    Double_t cutEtaMinEMC = -0.6687;
    Double_t cutEtaMaxEMC = 0.66465;

    Double_t cutEtaMinPHOS = -0.13;
    Double_t cutEtaMaxPHOS = 0.13;

    Double_t cutEtaMinDMC = -0.66112;
    Double_t cutEtaMaxDMC = 0.66112;
    Double_t cutEtaMinEdgeDMC = -0.227579;
    Double_t cutEtaMaxEdgeDMC = 0.227579;

    Double_t cutPhiMaxEDC = 3.28-TMath::Pi();
    Double_t cutPhiMaxDMCStripe = 5.70-TMath::Pi();

    Double_t cutPhiMinPHOSRun1 = 4.54-TMath::Pi();
    Double_t cutPhiMinPHOSRun2 = 4.36-TMath::Pi();
    Double_t cutPhiMaxPHOS = 5.59-TMath::Pi();

    Double_t cutPhiMinEMC = 1.39626-TMath::Pi();
    Double_t cutPhiMaxEMCR1 = 2.09-TMath::Pi();
    Double_t cutPhiMaxEMC = 3.15-TMath::Pi();
    Double_t cutPhiMinDMC = 4.5572-TMath::Pi();
    Double_t cutPhiMaxDMC = 5.5658-TMath::Pi();

    Long64_t nEntriesRecClus                 = AcceptanceTree->GetEntries();
    cout << "Number of Mesons in tree: " << nEntriesRecClus << endl;
    for (Long64_t i=0; i<nEntriesRecClus;i++) {
        if(i>0 && i%(nEntriesRecClus/(100)) ==0) cout << "//processed " << 100*(i)/nEntriesRecClus << "%"  << endl;
        AcceptanceTree->GetEntry(i);

        if((fBuffer_MotherY < 0.8) && (fBuffer_MotherY > -0.8)){

            // PCM
            histoGenerated_08_PCM->Fill(fBuffer_MotherPt);
            histoGenerated_09_PCM->Fill(fBuffer_MotherPt);
            if((fBuffer_Daughter1Eta > cutEtaMinPCM2)  && (fBuffer_Daughter1Eta < cutEtaMaxPCM2) && (fBuffer_Daughter2Eta > cutEtaMinPCM2) && (fBuffer_Daughter2Eta < cutEtaMaxPCM2)    ){
                histoAcceptance_08_PCM->Fill(fBuffer_MotherPt);
            }
            if((fBuffer_Daughter1Eta > cutEtaMinPCM)  && (fBuffer_Daughter1Eta < cutEtaMaxPCM) && (fBuffer_Daughter2Eta > cutEtaMinPCM) && (fBuffer_Daughter2Eta < cutEtaMaxPCM)    ){
                histoAcceptance_09_PCM->Fill(fBuffer_MotherPt);
            }

            histoGenerated_PHOS->Fill(fBuffer_MotherPt);
            if( (fBuffer_Daughter1Eta > cutEtaMinPHOS) && (fBuffer_Daughter1Eta < cutEtaMaxPHOS) && (fBuffer_Daughter1Phi > cutPhiMinPHOSRun1) && (fBuffer_Daughter1Phi < cutPhiMaxPHOS) &&
                (fBuffer_Daughter2Eta > cutEtaMinPHOS) && (fBuffer_Daughter2Eta < cutEtaMaxPHOS) && (fBuffer_Daughter2Phi > cutPhiMinPHOSRun1) && (fBuffer_Daughter2Phi < cutPhiMaxPHOS)    ){
                histoAcceptance_PHOS->Fill(fBuffer_MotherPt);
            }
            histoGenerated_PHOSR2->Fill(fBuffer_MotherPt);
            if( (fBuffer_Daughter1Eta > cutEtaMinPHOS) && (fBuffer_Daughter1Eta < cutEtaMaxPHOS) && (fBuffer_Daughter1Phi > cutPhiMinPHOSRun2) && (fBuffer_Daughter1Phi < cutPhiMaxPHOS) &&
                (fBuffer_Daughter2Eta > cutEtaMinPHOS) && (fBuffer_Daughter2Eta < cutEtaMaxPHOS) && (fBuffer_Daughter2Phi > cutPhiMinPHOSRun2) && (fBuffer_Daughter2Phi < cutPhiMaxPHOS)    ){
                histoAcceptance_PHOSR2->Fill(fBuffer_MotherPt);
            }
            histoGenerated_EMCR1->Fill(fBuffer_MotherPt);
            if( (fBuffer_Daughter1Eta > cutEtaMinEMC) && (fBuffer_Daughter1Eta < cutEtaMaxEMC) && (fBuffer_Daughter1Phi > cutPhiMinEMC) && (fBuffer_Daughter1Phi < cutPhiMaxEMCR1) &&
                (fBuffer_Daughter2Eta > cutEtaMinEMC) && (fBuffer_Daughter2Eta < cutEtaMaxEMC) && (fBuffer_Daughter2Phi > cutPhiMinEMC) && (fBuffer_Daughter2Phi < cutPhiMaxEMCR1)    ){
                histoAcceptance_EMCR1->Fill(fBuffer_MotherPt);
            }
            histoGenerated_EMC->Fill(fBuffer_MotherPt);
            if( (fBuffer_Daughter1Eta > cutEtaMinEMC) && (fBuffer_Daughter1Eta < cutEtaMaxEMC) && (fBuffer_Daughter1Phi > cutPhiMinEMC) && (fBuffer_Daughter1Phi < cutPhiMaxEMC) &&
                (fBuffer_Daughter2Eta > cutEtaMinEMC) && (fBuffer_Daughter2Eta < cutEtaMaxEMC) && (fBuffer_Daughter2Phi > cutPhiMinEMC) && (fBuffer_Daughter2Phi < cutPhiMaxEMC)    ){
                histoAcceptance_EMC->Fill(fBuffer_MotherPt);
            }
            histoGenerated_DMC->Fill(fBuffer_MotherPt);
            if( (fBuffer_Daughter1Eta > cutEtaMinDMC) && (fBuffer_Daughter1Eta < cutEtaMaxDMC) && (fBuffer_Daughter1Phi > cutPhiMinDMC) && (fBuffer_Daughter1Phi < cutPhiMaxDMC) &&
                (fBuffer_Daughter2Eta > cutEtaMinDMC) && (fBuffer_Daughter2Eta < cutEtaMaxDMC) && (fBuffer_Daughter2Phi > cutPhiMinDMC) && (fBuffer_Daughter2Phi < cutPhiMaxDMC)    ){
                histoAcceptance_DMC->Fill(fBuffer_MotherPt);
            }
            histoGenerated_EDC->Fill(fBuffer_MotherPt);
            if( (fBuffer_Daughter1Eta > cutEtaMinEMC) && (fBuffer_Daughter1Eta < cutEtaMaxEMC) &&
                (((fBuffer_Daughter1Phi > cutPhiMinEMC) && (fBuffer_Daughter1Phi < cutPhiMaxEDC)) ||
                ((fBuffer_Daughter1Phi > cutPhiMinDMC) && (fBuffer_Daughter1Phi < cutPhiMaxDMCStripe)))
                &&
                (fBuffer_Daughter2Eta > cutEtaMinEMC) && (fBuffer_Daughter2Eta < cutEtaMaxEMC) &&
                (((fBuffer_Daughter2Phi > cutPhiMinEMC) && (fBuffer_Daughter2Phi < cutPhiMaxEDC)) ||
                ((fBuffer_Daughter2Phi > cutPhiMinDMC) && (fBuffer_Daughter2Phi < cutPhiMaxDMCStripe)))  ){
                histoAcceptance_EDC->Fill(fBuffer_MotherPt);
            }
            histoGenerated_08_PCMEDC->Fill(fBuffer_MotherPt);
            if( (fBuffer_Daughter1Eta > cutEtaMinPCM2)  && (fBuffer_Daughter1Eta < cutEtaMaxPCM2) && (fBuffer_Daughter2Eta > cutEtaMinPCM2) && (fBuffer_Daughter2Eta < cutEtaMaxPCM2)){
                if( ((fBuffer_Daughter1Eta > cutEtaMinEMC) && (fBuffer_Daughter1Eta < cutEtaMaxEMC) &&
                (((fBuffer_Daughter1Phi > cutPhiMinEMC) && (fBuffer_Daughter1Phi < cutPhiMaxEDC)) ||
                ((fBuffer_Daughter1Phi > cutPhiMinDMC) && (fBuffer_Daughter1Phi < cutPhiMaxDMCStripe))))
                ||
                ((fBuffer_Daughter2Eta > cutEtaMinEMC) && (fBuffer_Daughter2Eta < cutEtaMaxEMC) &&
                (((fBuffer_Daughter2Phi > cutPhiMinEMC) && (fBuffer_Daughter2Phi < cutPhiMaxEDC)) ||
                ((fBuffer_Daughter2Phi > cutPhiMinDMC) && (fBuffer_Daughter2Phi < cutPhiMaxDMCStripe))))  ){
                    histoAcceptance_08_PCMEDC->Fill(fBuffer_MotherPt);
                }
            }
            histoGenerated_09_PCMEDC->Fill(fBuffer_MotherPt);
            if( (fBuffer_Daughter1Eta > cutEtaMinPCM)  && (fBuffer_Daughter1Eta < cutEtaMaxPCM) && (fBuffer_Daughter2Eta > cutEtaMinPCM) && (fBuffer_Daughter2Eta < cutEtaMaxPCM)){
                if( ((fBuffer_Daughter1Eta > cutEtaMinEMC) && (fBuffer_Daughter1Eta < cutEtaMaxEMC) &&
                (((fBuffer_Daughter1Phi > cutPhiMinEMC) && (fBuffer_Daughter1Phi < cutPhiMaxEDC)) ||
                ((fBuffer_Daughter1Phi > cutPhiMinDMC) && (fBuffer_Daughter1Phi < cutPhiMaxDMCStripe))))
                ||
                ((fBuffer_Daughter2Eta > cutEtaMinEMC) && (fBuffer_Daughter2Eta < cutEtaMaxEMC) &&
                (((fBuffer_Daughter2Phi > cutPhiMinEMC) && (fBuffer_Daughter2Phi < cutPhiMaxEDC)) ||
                ((fBuffer_Daughter2Phi > cutPhiMinDMC) && (fBuffer_Daughter2Phi < cutPhiMaxDMCStripe))))  ){
                    histoAcceptance_09_PCMEDC->Fill(fBuffer_MotherPt);
                }
            }
            histoGenerated_08_PCMPHOS->Fill(fBuffer_MotherPt);
            if( (fBuffer_Daughter1Eta > cutEtaMinPCM2)  && (fBuffer_Daughter1Eta < cutEtaMaxPCM2) && (fBuffer_Daughter2Eta > cutEtaMinPCM2) && (fBuffer_Daughter2Eta < cutEtaMaxPCM2)){
                if( ((fBuffer_Daughter1Eta > cutEtaMinPHOS) && (fBuffer_Daughter1Eta < cutEtaMaxPHOS) && (fBuffer_Daughter1Phi > cutPhiMinPHOSRun1) && (fBuffer_Daughter1Phi < cutPhiMaxPHOS)) ||
                ((fBuffer_Daughter2Eta > cutEtaMinPHOS) && (fBuffer_Daughter2Eta < cutEtaMaxPHOS) && (fBuffer_Daughter2Phi > cutPhiMinPHOSRun1) && (fBuffer_Daughter2Phi < cutPhiMaxPHOS))  ){
                    histoAcceptance_08_PCMPHOS->Fill(fBuffer_MotherPt);
                }
            }
            histoGenerated_09_PCMPHOS->Fill(fBuffer_MotherPt);
            if( (fBuffer_Daughter1Eta > cutEtaMinPCM)  && (fBuffer_Daughter1Eta < cutEtaMaxPCM) && (fBuffer_Daughter2Eta > cutEtaMinPCM) && (fBuffer_Daughter2Eta < cutEtaMaxPCM)){
                if( ((fBuffer_Daughter1Eta > cutEtaMinPHOS) && (fBuffer_Daughter1Eta < cutEtaMaxPHOS) && (fBuffer_Daughter1Phi > cutPhiMinPHOSRun1) && (fBuffer_Daughter1Phi < cutPhiMaxPHOS)) ||
                ((fBuffer_Daughter2Eta > cutEtaMinPHOS) && (fBuffer_Daughter2Eta < cutEtaMaxPHOS) && (fBuffer_Daughter2Phi > cutPhiMinPHOSRun1) && (fBuffer_Daughter2Phi < cutPhiMaxPHOS))  ){
                    histoAcceptance_09_PCMPHOS->Fill(fBuffer_MotherPt);
                }
            }
            histoGenerated_08_PCMPHOSR2->Fill(fBuffer_MotherPt);
            if( (fBuffer_Daughter1Eta > cutEtaMinPCM2)  && (fBuffer_Daughter1Eta < cutEtaMaxPCM2) && (fBuffer_Daughter2Eta > cutEtaMinPCM2) && (fBuffer_Daughter2Eta < cutEtaMaxPCM2)){
                if( ((fBuffer_Daughter1Eta > cutEtaMinPHOS) && (fBuffer_Daughter1Eta < cutEtaMaxPHOS) && (fBuffer_Daughter1Phi > cutPhiMinPHOSRun2) && (fBuffer_Daughter1Phi < cutPhiMaxPHOS)) ||
                    ((fBuffer_Daughter2Eta > cutEtaMinPHOS) && (fBuffer_Daughter2Eta < cutEtaMaxPHOS) && (fBuffer_Daughter2Phi > cutPhiMinPHOSRun2) && (fBuffer_Daughter2Phi < cutPhiMaxPHOS))  ){
                    histoAcceptance_08_PCMPHOSR2->Fill(fBuffer_MotherPt);
                }
            }
            histoGenerated_09_PCMPHOSR2->Fill(fBuffer_MotherPt);
            if( (fBuffer_Daughter1Eta > cutEtaMinPCM)  && (fBuffer_Daughter1Eta < cutEtaMaxPCM) && (fBuffer_Daughter2Eta > cutEtaMinPCM) && (fBuffer_Daughter2Eta < cutEtaMaxPCM)){
                if( ((fBuffer_Daughter1Eta > cutEtaMinPHOS) && (fBuffer_Daughter1Eta < cutEtaMaxPHOS) && (fBuffer_Daughter1Phi > cutPhiMinPHOSRun2) && (fBuffer_Daughter1Phi < cutPhiMaxPHOS)) ||
                    ((fBuffer_Daughter2Eta > cutEtaMinPHOS) && (fBuffer_Daughter2Eta < cutEtaMaxPHOS) && (fBuffer_Daughter2Phi > cutPhiMinPHOSRun2) && (fBuffer_Daughter2Phi < cutPhiMaxPHOS))  ){
                    histoAcceptance_09_PCMPHOSR2->Fill(fBuffer_MotherPt);
                }
            }
            histoGenerated_08_PCMEMCR1->Fill(fBuffer_MotherPt);
            if( (fBuffer_Daughter1Eta > cutEtaMinPCM2)  && (fBuffer_Daughter1Eta < cutEtaMaxPCM2) && (fBuffer_Daughter2Eta > cutEtaMinPCM2) && (fBuffer_Daughter2Eta < cutEtaMaxPCM2)){
                if( ((fBuffer_Daughter1Eta > cutEtaMinEMC) && (fBuffer_Daughter1Eta < cutEtaMaxEMC) && (fBuffer_Daughter1Phi > cutPhiMinEMC) && (fBuffer_Daughter1Phi < cutPhiMaxEMCR1)) ||
                    ((fBuffer_Daughter2Eta > cutEtaMinEMC) && (fBuffer_Daughter2Eta < cutEtaMaxEMC) && (fBuffer_Daughter2Phi > cutPhiMinEMC) && (fBuffer_Daughter2Phi < cutPhiMaxEMCR1))){
                    histoAcceptance_08_PCMEMCR1->Fill(fBuffer_MotherPt);
                }
            }
            histoGenerated_09_PCMEMCR1->Fill(fBuffer_MotherPt);
            if( (fBuffer_Daughter1Eta > cutEtaMinPCM)  && (fBuffer_Daughter1Eta < cutEtaMaxPCM) && (fBuffer_Daughter2Eta > cutEtaMinPCM) && (fBuffer_Daughter2Eta < cutEtaMaxPCM)){
                if( ((fBuffer_Daughter1Eta > cutEtaMinEMC) && (fBuffer_Daughter1Eta < cutEtaMaxEMC) && (fBuffer_Daughter1Phi > cutPhiMinEMC) && (fBuffer_Daughter1Phi < cutPhiMaxEMCR1)) ||
                    ((fBuffer_Daughter2Eta > cutEtaMinEMC) && (fBuffer_Daughter2Eta < cutEtaMaxEMC) && (fBuffer_Daughter2Phi > cutPhiMinEMC) && (fBuffer_Daughter2Phi < cutPhiMaxEMCR1))){
                    histoAcceptance_09_PCMEMCR1->Fill(fBuffer_MotherPt);
                }
            }
            histoGenerated_08_PCMEMC->Fill(fBuffer_MotherPt);
            if( (fBuffer_Daughter1Eta > cutEtaMinPCM2)  && (fBuffer_Daughter1Eta < cutEtaMaxPCM2) && (fBuffer_Daughter2Eta > cutEtaMinPCM2) && (fBuffer_Daughter2Eta < cutEtaMaxPCM2)){
                if( ((fBuffer_Daughter1Eta > cutEtaMinEMC) && (fBuffer_Daughter1Eta < cutEtaMaxEMC) && (fBuffer_Daughter1Phi > cutPhiMinEMC) && (fBuffer_Daughter1Phi < cutPhiMaxEMC)) ||
                    ((fBuffer_Daughter2Eta > cutEtaMinEMC) && (fBuffer_Daughter2Eta < cutEtaMaxEMC) && (fBuffer_Daughter2Phi > cutPhiMinEMC) && (fBuffer_Daughter2Phi < cutPhiMaxEMC))){
                    histoAcceptance_08_PCMEMC->Fill(fBuffer_MotherPt);
                }
            }
            histoGenerated_09_PCMEMC->Fill(fBuffer_MotherPt);
            if( (fBuffer_Daughter1Eta > cutEtaMinPCM)  && (fBuffer_Daughter1Eta < cutEtaMaxPCM) && (fBuffer_Daughter2Eta > cutEtaMinPCM) && (fBuffer_Daughter2Eta < cutEtaMaxPCM)){
                if( ((fBuffer_Daughter1Eta > cutEtaMinEMC) && (fBuffer_Daughter1Eta < cutEtaMaxEMC) && (fBuffer_Daughter1Phi > cutPhiMinEMC) && (fBuffer_Daughter1Phi < cutPhiMaxEMC)) ||
                    ((fBuffer_Daughter2Eta > cutEtaMinEMC) && (fBuffer_Daughter2Eta < cutEtaMaxEMC) && (fBuffer_Daughter2Phi > cutPhiMinEMC) && (fBuffer_Daughter2Phi < cutPhiMaxEMC))){
                    histoAcceptance_09_PCMEMC->Fill(fBuffer_MotherPt);
                }
            }
            histoGenerated_08_PCMDMC->Fill(fBuffer_MotherPt);
            if( (fBuffer_Daughter1Eta > cutEtaMinPCM2)  && (fBuffer_Daughter1Eta < cutEtaMaxPCM2) && (fBuffer_Daughter2Eta > cutEtaMinPCM2) && (fBuffer_Daughter2Eta < cutEtaMaxPCM2)){
                if( ((fBuffer_Daughter1Eta > cutEtaMinDMC) && (fBuffer_Daughter1Eta < cutEtaMaxDMC) && (fBuffer_Daughter1Phi > cutPhiMinDMC) && (fBuffer_Daughter1Phi < cutPhiMaxDMC)) ||
                    ((fBuffer_Daughter2Eta > cutEtaMinDMC) && (fBuffer_Daughter2Eta < cutEtaMaxDMC) && (fBuffer_Daughter2Phi > cutPhiMinDMC) && (fBuffer_Daughter2Phi < cutPhiMaxDMC))  ){
                    histoAcceptance_08_PCMDMC->Fill(fBuffer_MotherPt);
                }
            }
            histoGenerated_09_PCMDMC->Fill(fBuffer_MotherPt);
            if( (fBuffer_Daughter1Eta > cutEtaMinPCM)  && (fBuffer_Daughter1Eta < cutEtaMaxPCM) && (fBuffer_Daughter2Eta > cutEtaMinPCM) && (fBuffer_Daughter2Eta < cutEtaMaxPCM)){
                if( ((fBuffer_Daughter1Eta > cutEtaMinDMC) && (fBuffer_Daughter1Eta < cutEtaMaxDMC) && (fBuffer_Daughter1Phi > cutPhiMinDMC) && (fBuffer_Daughter1Phi < cutPhiMaxDMC)) ||
                    ((fBuffer_Daughter2Eta > cutEtaMinDMC) && (fBuffer_Daughter2Eta < cutEtaMaxDMC) && (fBuffer_Daughter2Phi > cutPhiMinDMC) && (fBuffer_Daughter2Phi < cutPhiMaxDMC))  ){
                    histoAcceptance_09_PCMDMC->Fill(fBuffer_MotherPt);
                }
            }



        }
        if((fBuffer_MotherY < 0.6) && (fBuffer_MotherY > -0.6)){
            histoGenerated_mEMC->Fill(fBuffer_MotherPt);
            if( ((fBuffer_Daughter1Eta > cutEtaMinEMC) && (fBuffer_Daughter1Eta < cutEtaMaxEMC) && (fBuffer_Daughter1Phi > cutPhiMinEMC) && (fBuffer_Daughter1Phi < cutPhiMaxEMC)) ||
                ((fBuffer_Daughter2Eta > cutEtaMinEMC) && (fBuffer_Daughter2Eta < cutEtaMaxEMC) && (fBuffer_Daughter2Phi > cutPhiMinEMC) && (fBuffer_Daughter2Phi < cutPhiMaxEMC))    ){
                histoAcceptance_mEMC->Fill(fBuffer_MotherPt);
            }
            histoGenerated_mDMC->Fill(fBuffer_MotherPt);
            if( ((fBuffer_Daughter1Eta > cutEtaMinDMC) && (fBuffer_Daughter1Eta < cutEtaMaxDMC) && (fBuffer_Daughter1Phi > cutPhiMinDMC) && (fBuffer_Daughter1Phi < cutPhiMaxDMC)) ||
                ((fBuffer_Daughter2Eta > cutEtaMinDMC) && (fBuffer_Daughter2Eta < cutEtaMaxDMC) && (fBuffer_Daughter2Phi > cutPhiMinDMC) && (fBuffer_Daughter2Phi < cutPhiMaxDMC))    ){
                histoAcceptance_mDMC->Fill(fBuffer_MotherPt);
            }
            histoGenerated_mEDC->Fill(fBuffer_MotherPt);
            if( ((fBuffer_Daughter1Eta > cutEtaMinEMC) && (fBuffer_Daughter1Eta < cutEtaMaxEMC) &&
                (((fBuffer_Daughter1Phi > cutPhiMinEMC) && (fBuffer_Daughter1Phi < cutPhiMaxEDC)) ||
                ((fBuffer_Daughter1Phi > cutPhiMinDMC) && (fBuffer_Daughter1Phi < cutPhiMaxDMCStripe))))
                ||
                ((fBuffer_Daughter2Eta > cutEtaMinEMC) && (fBuffer_Daughter2Eta < cutEtaMaxEMC) &&
                (((fBuffer_Daughter2Phi > cutPhiMinEMC) && (fBuffer_Daughter2Phi < cutPhiMaxEDC)) ||
                ((fBuffer_Daughter2Phi > cutPhiMinDMC) && (fBuffer_Daughter2Phi < cutPhiMaxDMCStripe))))  ){
                histoAcceptance_mEDC->Fill(fBuffer_MotherPt);
            }
        }

        if( ((fBuffer_Daughter1E>fBuffer_Daughter2E)&&(fBuffer_Daughter1Eta > cutEtaMinEMC) && (fBuffer_Daughter1Eta < cutEtaMaxEMC) && (fBuffer_Daughter1Phi > cutPhiMinEMC) && (fBuffer_Daughter1Phi < cutPhiMaxEMC))||
            ((fBuffer_Daughter2E>fBuffer_Daughter1E)&&(fBuffer_Daughter2Eta > cutEtaMinEMC) && (fBuffer_Daughter2Eta < cutEtaMaxEMC) && (fBuffer_Daughter2Phi > cutPhiMinEMC) && (fBuffer_Daughter2Phi < cutPhiMaxEMC))){ // higher pt gamma on EMC -> then save R of gamma candidates vs pi0 momentum
            histoGammaStudyRvsMesonPt->Fill(fBuffer_MotherPt,TMath::Sqrt(TMath::Power(fBuffer_Daughter1Eta-fBuffer_Daughter2Eta,2)+TMath::Power(fBuffer_Daughter1Phi-fBuffer_Daughter2Phi,2)));
            histoGammaStudyEtavsMesonPt->Fill(fBuffer_MotherPt,TMath::Abs(fBuffer_Daughter1Eta-fBuffer_Daughter2Eta));
            histoGammaStudyPhivsMesonPt->Fill(fBuffer_MotherPt,TMath::Abs(fBuffer_Daughter1Phi-fBuffer_Daughter2Phi));
        }

    }
    TH1D* histoAcceptanceCalc_08_PCM = (TH1D*) histoAcceptance_08_PCM->Clone("histoAcceptanceCalc_08_PCM");
    histoAcceptanceCalc_08_PCM->Divide(histoGenerated_08_PCM);
    histoAcceptanceCalc_08_PCM->Print("all");
    TH1D* histoAcceptanceCalc_08_PCMEMCR1 = (TH1D*) histoAcceptance_08_PCMEMCR1->Clone("histoAcceptanceCalc_08_PCMEMCR1");
    histoAcceptanceCalc_08_PCMEMCR1->Divide(histoGenerated_08_PCMEMCR1);
    histoAcceptanceCalc_08_PCMEMCR1->Print("all");
    TH1D* histoAcceptanceCalc_08_PCMEMC = (TH1D*) histoAcceptance_08_PCMEMC->Clone("histoAcceptanceCalc_08_PCMEMC");
    histoAcceptanceCalc_08_PCMEMC->Divide(histoGenerated_08_PCMEMC);
    histoAcceptanceCalc_08_PCMEMC->Print("all");
    TH1D* histoAcceptanceCalc_08_PCMPHOS = (TH1D*) histoAcceptance_08_PCMPHOS->Clone("histoAcceptanceCalc_08_PCMPHOS");
    histoAcceptanceCalc_08_PCMPHOS->Divide(histoGenerated_08_PCMPHOS);
    histoAcceptanceCalc_08_PCMPHOS->Print("all");
    TH1D* histoAcceptanceCalc_08_PCMPHOSR2 = (TH1D*) histoAcceptance_08_PCMPHOSR2->Clone("histoAcceptanceCalc_08_PCMPHOSR2");
    histoAcceptanceCalc_08_PCMPHOSR2->Divide(histoGenerated_08_PCMPHOSR2);
    histoAcceptanceCalc_08_PCMPHOSR2->Print("all");
    TH1D* histoAcceptanceCalc_08_PCMDMC = (TH1D*) histoAcceptance_08_PCMDMC->Clone("histoAcceptanceCalc_08_PCMDMC");
    histoAcceptanceCalc_08_PCMDMC->Divide(histoGenerated_08_PCMDMC);
    histoAcceptanceCalc_08_PCMDMC->Print("all");
    TH1D* histoAcceptanceCalc_08_PCMEDC = (TH1D*) histoAcceptance_08_PCMEDC->Clone("histoAcceptanceCalc_08_PCMEDC");
    histoAcceptanceCalc_08_PCMEDC->Divide(histoGenerated_08_PCMEDC);
    histoAcceptanceCalc_08_PCMEDC->Print("all");
    TH1D* histoAcceptanceCalc_09_PCM = (TH1D*) histoAcceptance_09_PCM->Clone("histoAcceptanceCalc_09_PCM");
    histoAcceptanceCalc_09_PCM->Divide(histoGenerated_09_PCM);
    histoAcceptanceCalc_09_PCM->Print("all");
    TH1D* histoAcceptanceCalc_09_PCMEMCR1 = (TH1D*) histoAcceptance_09_PCMEMCR1->Clone("histoAcceptanceCalc_09_PCMEMCR1");
    histoAcceptanceCalc_09_PCMEMCR1->Divide(histoGenerated_09_PCMEMCR1);
    histoAcceptanceCalc_09_PCMEMCR1->Print("all");
    TH1D* histoAcceptanceCalc_09_PCMEMC = (TH1D*) histoAcceptance_09_PCMEMC->Clone("histoAcceptanceCalc_09_PCMEMC");
    histoAcceptanceCalc_09_PCMEMC->Divide(histoGenerated_09_PCMEMC);
    histoAcceptanceCalc_09_PCMEMC->Print("all");
    TH1D* histoAcceptanceCalc_09_PCMPHOS = (TH1D*) histoAcceptance_09_PCMPHOS->Clone("histoAcceptanceCalc_09_PCMPHOS");
    histoAcceptanceCalc_09_PCMPHOS->Divide(histoGenerated_09_PCMPHOS);
    histoAcceptanceCalc_09_PCMPHOS->Print("all");
    TH1D* histoAcceptanceCalc_09_PCMPHOSR2 = (TH1D*) histoAcceptance_09_PCMPHOSR2->Clone("histoAcceptanceCalc_09_PCMPHOSR2");
    histoAcceptanceCalc_09_PCMPHOSR2->Divide(histoGenerated_09_PCMPHOSR2);
    histoAcceptanceCalc_09_PCMPHOSR2->Print("all");
    TH1D* histoAcceptanceCalc_09_PCMDMC = (TH1D*) histoAcceptance_09_PCMDMC->Clone("histoAcceptanceCalc_09_PCMDMC");
    histoAcceptanceCalc_09_PCMDMC->Divide(histoGenerated_09_PCMDMC);
    histoAcceptanceCalc_09_PCMDMC->Print("all");
    TH1D* histoAcceptanceCalc_09_PCMEDC = (TH1D*) histoAcceptance_09_PCMEDC->Clone("histoAcceptanceCalc_09_PCMEDC");
    histoAcceptanceCalc_09_PCMEDC->Divide(histoGenerated_09_PCMEDC);
    histoAcceptanceCalc_09_PCMEDC->Print("all");
    TH1D* histoAcceptanceCalc_PHOS = (TH1D*) histoAcceptance_PHOS->Clone("histoAcceptanceCalc_PHOS");
    histoAcceptanceCalc_PHOS->Divide(histoGenerated_PHOS);
    histoAcceptanceCalc_PHOS->Print("all");
    TH1D* histoAcceptanceCalc_PHOSR2 = (TH1D*) histoAcceptance_PHOSR2->Clone("histoAcceptanceCalc_PHOSR2");
    histoAcceptanceCalc_PHOSR2->Divide(histoGenerated_PHOSR2);
    histoAcceptanceCalc_PHOSR2->Print("all");
    TH1D* histoAcceptanceCalc_EMC = (TH1D*) histoAcceptance_EMC->Clone("histoAcceptanceCalc_EMC");
    histoAcceptanceCalc_EMC->Divide(histoGenerated_EMC);
    histoAcceptanceCalc_EMC->Print("all");
    TH1D* histoAcceptanceCalc_EMCR1 = (TH1D*) histoAcceptance_EMCR1->Clone("histoAcceptanceCalc_EMCR1");
    histoAcceptanceCalc_EMCR1->Divide(histoGenerated_EMCR1);
    histoAcceptanceCalc_EMCR1->Print("all");
    TH1D* histoAcceptanceCalc_DMC = (TH1D*) histoAcceptance_DMC->Clone("histoAcceptanceCalc_DMC");
    histoAcceptanceCalc_DMC->Divide(histoGenerated_DMC);
    histoAcceptanceCalc_DMC->Print("all");
    TH1D* histoAcceptanceCalc_EDC = (TH1D*) histoAcceptance_EDC->Clone("histoAcceptanceCalc_EDC");
    histoAcceptanceCalc_EDC->Divide(histoGenerated_EDC);
    histoAcceptanceCalc_EDC->Print("all");
    TH1D* histoAcceptanceCalc_mEMC = (TH1D*) histoAcceptance_mEMC->Clone("histoAcceptanceCalc_mEMC");
    histoAcceptanceCalc_mEMC->Divide(histoGenerated_mEMC);
    histoAcceptanceCalc_mEMC->Print("all");
    TH1D* histoAcceptanceCalc_mDMC = (TH1D*) histoAcceptance_mDMC->Clone("histoAcceptanceCalc_mDMC");
    histoAcceptanceCalc_mDMC->Divide(histoGenerated_mDMC);
    histoAcceptanceCalc_mDMC->Print("all");
    TH1D* histoAcceptanceCalc_mEDC = (TH1D*) histoAcceptance_mEDC->Clone("histoAcceptanceCalc_mEDC");
    histoAcceptanceCalc_mEDC->Divide(histoGenerated_mEDC);
    histoAcceptanceCalc_mEDC->Print("all");


    TCanvas *canvasAcceptance                            = new TCanvas("canvasAcceptance","",200,10,1350,900);
    DrawGammaCanvasSettings( canvasAcceptance, 0.1, 0.01, 0.01, 0.125);
    canvasAcceptance->cd();
    canvasAcceptance->SetLogx();
   Double_t textSizeLabelsPixel                 = 54;
   Double_t txtsizelabelsAcc = 0;
   Double_t txtsizefacAcc    = 0;
   if (canvasAcceptance->XtoPixel(canvasAcceptance->GetX2()) <canvasAcceptance->YtoPixel(canvasAcceptance->GetY1()) ){
       txtsizelabelsAcc      = (Double_t)textSizeLabelsPixel/canvasAcceptance->XtoPixel(canvasAcceptance->GetX2()) ;
       txtsizefacAcc         = (Double_t)1./canvasAcceptance->XtoPixel(canvasAcceptance->GetX2()) ;
   } else {
       txtsizelabelsAcc      = (Double_t)textSizeLabelsPixel/canvasAcceptance->YtoPixel(canvasAcceptance->GetY1());
       txtsizefacAcc         = (Double_t)1./canvasAcceptance->YtoPixel(canvasAcceptance->GetY1());
   }
   cout << txtsizelabelsAcc << endl;

   txtsizelabelsAcc*=0.9;
    TH2F * histAccDummy                           = new TH2F("histAccDummy","histAccDummy",25000,0.3,250,2500,0,1);
    SetStyleHistoTH2ForGraphs(histAccDummy, "#it{p}_{T} (GeV/#it{c})","#it{A}", 0.85*txtsizelabelsAcc, txtsizelabelsAcc,0.85*txtsizelabelsAcc,1.1*txtsizelabelsAcc, 0.9, 0.65, 510, 510);
    histAccDummy->GetXaxis()->SetNoExponent();
    histAccDummy->GetYaxis()->SetNdivisions(505,kTRUE);
    histAccDummy->GetXaxis()->SetMoreLogLabels(kTRUE);

    histAccDummy->DrawCopy();
    if(histoAcceptanceCalc_08_PCM){
        DrawGammaSetMarker(histoAcceptanceCalc_08_PCM, 24, 2, kBlack, kBlack);
        histoAcceptanceCalc_08_PCM->Draw("same");
    }
    if(histoAcceptanceCalc_EDC){
        DrawGammaSetMarker(histoAcceptanceCalc_EDC, 24, 2, kGreen+2, kGreen+2);
        histoAcceptanceCalc_EDC->Draw("same");
    }
    if(histoAcceptanceCalc_EMC){
        DrawGammaSetMarker(histoAcceptanceCalc_EMC, 24, 2, kGreen-8, kGreen-8);
        histoAcceptanceCalc_EMC->Draw("same");
    }
    if(histoAcceptanceCalc_EMCR1){
        DrawGammaSetMarker(histoAcceptanceCalc_EMCR1, 24, 2, kGreen-2, kGreen-2);
        histoAcceptanceCalc_EMCR1->Draw("same");
    }
    if(histoAcceptanceCalc_DMC){
        DrawGammaSetMarker(histoAcceptanceCalc_DMC, 24, 2, kSpring+2, kSpring+2);
        histoAcceptanceCalc_DMC->Draw("same");
    }
    if(histoAcceptanceCalc_08_PCMEMC){
        DrawGammaSetMarker(histoAcceptanceCalc_08_PCMEMC, 24, 2, kBlue-8, kBlue-8);
        histoAcceptanceCalc_08_PCMEMC->Draw("same");
    }
    if(histoAcceptanceCalc_08_PCMEMCR1){
        DrawGammaSetMarker(histoAcceptanceCalc_08_PCMEMCR1, 24, 2, kBlue-2, kBlue-2);
        histoAcceptanceCalc_08_PCMEMCR1->Draw("same");
    }
    if(histoAcceptanceCalc_PHOS){
        DrawGammaSetMarker(histoAcceptanceCalc_PHOS, 24, 2, kRed+2, kRed+2);
        histoAcceptanceCalc_PHOS->Draw("same");
    }
    if(histoAcceptanceCalc_PHOSR2){
        DrawGammaSetMarker(histoAcceptanceCalc_PHOSR2, 24, 2, kRed-8, kRed-8);
        histoAcceptanceCalc_PHOSR2->Draw("same");
    }
    if(histoAcceptanceCalc_08_PCMPHOS){
        DrawGammaSetMarker(histoAcceptanceCalc_08_PCMPHOS, 24, 2, kOrange+2, kOrange+2);
        histoAcceptanceCalc_08_PCMPHOS->Draw("same");
    }
    if(histoAcceptanceCalc_08_PCMPHOSR2){
        DrawGammaSetMarker(histoAcceptanceCalc_08_PCMPHOSR2, 24, 2, kOrange-8, kOrange-8);
        histoAcceptanceCalc_08_PCMPHOSR2->Draw("same");
    }
    if(histoAcceptanceCalc_08_PCMEDC){
        DrawGammaSetMarker(histoAcceptanceCalc_08_PCMEDC, 24, 2, kBlue+2, kBlue+2);
        histoAcceptanceCalc_08_PCMEDC->Draw("same");
    }
    if(histoAcceptanceCalc_08_PCMDMC){
        DrawGammaSetMarker(histoAcceptanceCalc_08_PCMDMC, 24, 2, kCyan+2, kCyan+2);
        histoAcceptanceCalc_08_PCMDMC->Draw("same");
    }
    canvasAcceptance->Print(Form("%s/Acceptance_%s.pdf",outputDir.Data(),meson.Data()));


    // ********************************************************************************
    // *                  Writing histograms to outputfile                            *
    // ********************************************************************************
    TString fileNameOutput= Form("%s/Acceptance%s%s.root",outputDir.Data(),energy.Data(),meson.Data()) ;
    TFile* filePhotoQAWrite = new TFile(fileNameOutput.Data(),"RECREATE");

    if(histoAcceptanceCalc_mEDC)histoAcceptanceCalc_mEDC->Write(Form("histo%sAcceptance_mEDC",meson.Data()),TObject::kWriteDelete);
    if(histoAcceptanceCalc_mEMC)histoAcceptanceCalc_mEMC->Write(Form("histo%sAcceptance_mEMC",meson.Data()),TObject::kWriteDelete);
    if(histoAcceptanceCalc_mDMC)histoAcceptanceCalc_mDMC->Write(Form("histo%sAcceptance_mDMC",meson.Data()),TObject::kWriteDelete);
    if(histoAcceptanceCalc_EDC)histoAcceptanceCalc_EDC->Write(Form("histo%sAcceptance_EDC",meson.Data()),TObject::kWriteDelete);
    if(histoAcceptanceCalc_EMC)histoAcceptanceCalc_EMC->Write(Form("histo%sAcceptance_EMC",meson.Data()),TObject::kWriteDelete);
    if(histoAcceptanceCalc_EMCR1)histoAcceptanceCalc_EMCR1->Write(Form("histo%sAcceptance_EMCR1",meson.Data()),TObject::kWriteDelete);
    if(histoAcceptanceCalc_DMC)histoAcceptanceCalc_DMC->Write(Form("histo%sAcceptance_DMC",meson.Data()),TObject::kWriteDelete);
    if(histoAcceptanceCalc_PHOS)histoAcceptanceCalc_PHOS->Write(Form("histo%sAcceptance_PHOS",meson.Data()),TObject::kWriteDelete);
    if(histoAcceptanceCalc_PHOSR2)histoAcceptanceCalc_PHOSR2->Write(Form("histo%sAcceptance_PHOSR2",meson.Data()),TObject::kWriteDelete);
    if(histoAcceptanceCalc_08_PCM)histoAcceptanceCalc_08_PCM->Write(Form("histo%sAcceptance_08_PCM",meson.Data()),TObject::kWriteDelete);
    if(histoAcceptanceCalc_08_PCMEDC)histoAcceptanceCalc_08_PCMEDC->Write(Form("histo%sAcceptance_08_PCMEDC",meson.Data()),TObject::kWriteDelete);
    if(histoAcceptanceCalc_08_PCMEMC)histoAcceptanceCalc_08_PCMEMC->Write(Form("histo%sAcceptance_08_PCMEMC",meson.Data()),TObject::kWriteDelete);
    if(histoAcceptanceCalc_08_PCMEMCR1)histoAcceptanceCalc_08_PCMEMCR1->Write(Form("histo%sAcceptance_08_PCMEMCR1",meson.Data()),TObject::kWriteDelete);
    if(histoAcceptanceCalc_08_PCMDMC)histoAcceptanceCalc_08_PCMDMC->Write(Form("histo%sAcceptance_08_PCMDMC",meson.Data()),TObject::kWriteDelete);
    if(histoAcceptanceCalc_08_PCMPHOS)histoAcceptanceCalc_08_PCMPHOS->Write(Form("histo%sAcceptance_08_PCMPHOS",meson.Data()),TObject::kWriteDelete);
    if(histoAcceptanceCalc_08_PCMPHOSR2)histoAcceptanceCalc_08_PCMPHOSR2->Write(Form("histo%sAcceptance_08_PCMPHOSR2",meson.Data()),TObject::kWriteDelete);
    if(histoAcceptanceCalc_09_PCM)histoAcceptanceCalc_09_PCM->Write(Form("histo%sAcceptance_09_PCM",meson.Data()),TObject::kWriteDelete);
    if(histoAcceptanceCalc_09_PCMEDC)histoAcceptanceCalc_09_PCMEDC->Write(Form("histo%sAcceptance_09_PCMEDC",meson.Data()),TObject::kWriteDelete);
    if(histoAcceptanceCalc_09_PCMEMC)histoAcceptanceCalc_09_PCMEMC->Write(Form("histo%sAcceptance_09_PCMEMC",meson.Data()),TObject::kWriteDelete);
    if(histoAcceptanceCalc_09_PCMEMCR1)histoAcceptanceCalc_09_PCMEMCR1->Write(Form("histo%sAcceptance_09_PCMEMCR1",meson.Data()),TObject::kWriteDelete);
    if(histoAcceptanceCalc_09_PCMDMC)histoAcceptanceCalc_09_PCMDMC->Write(Form("histo%sAcceptance_09_PCMDMC",meson.Data()),TObject::kWriteDelete);
    if(histoAcceptanceCalc_09_PCMPHOS)histoAcceptanceCalc_09_PCMPHOS->Write(Form("histo%sAcceptance_09_PCMPHOS",meson.Data()),TObject::kWriteDelete);
    if(histoAcceptanceCalc_09_PCMPHOSR2)histoAcceptanceCalc_09_PCMPHOSR2->Write(Form("histo%sAcceptance_09_PCMPHOSR2",meson.Data()),TObject::kWriteDelete);

    if(histoAcceptance_mEDC)histoAcceptance_mEDC->Write(Form("histo%sInAcc_mEDC",meson.Data()),TObject::kWriteDelete);
    if(histoAcceptance_mEMC)histoAcceptance_mEMC->Write(Form("histo%sInAcc_mEMC",meson.Data()),TObject::kWriteDelete);
    if(histoAcceptance_mDMC)histoAcceptance_mDMC->Write(Form("histo%sInAcc_mDMC",meson.Data()),TObject::kWriteDelete);
    if(histoAcceptance_EDC)histoAcceptance_EDC->Write(Form("histo%sInAcc_EDC",meson.Data()),TObject::kWriteDelete);
    if(histoAcceptance_EMC)histoAcceptance_EMC->Write(Form("histo%sInAcc_EMC",meson.Data()),TObject::kWriteDelete);
    if(histoAcceptance_EMCR1)histoAcceptance_EMCR1->Write(Form("histo%sInAcc_EMCR1",meson.Data()),TObject::kWriteDelete);
    if(histoAcceptance_DMC)histoAcceptance_DMC->Write(Form("histo%sInAcc_DMC",meson.Data()),TObject::kWriteDelete);
    if(histoAcceptance_PHOS)histoAcceptance_PHOS->Write(Form("histo%sInAcc_PHOS",meson.Data()),TObject::kWriteDelete);
    if(histoAcceptance_PHOSR2)histoAcceptance_PHOSR2->Write(Form("histo%sInAcc_PHOSR2",meson.Data()),TObject::kWriteDelete);
    if(histoAcceptance_08_PCM)histoAcceptance_08_PCM->Write(Form("histo%sInAcc_08_PCM",meson.Data()),TObject::kWriteDelete);
    if(histoAcceptance_08_PCMEDC)histoAcceptance_08_PCMEDC->Write(Form("histo%sInAcc_08_PCMEDC",meson.Data()),TObject::kWriteDelete);
    if(histoAcceptance_08_PCMEMC)histoAcceptance_08_PCMEMC->Write(Form("histo%sInAcc_08_PCMEMC",meson.Data()),TObject::kWriteDelete);
    if(histoAcceptance_08_PCMEMCR1)histoAcceptance_08_PCMEMCR1->Write(Form("histo%sInAcc_08_PCMEMCR1",meson.Data()),TObject::kWriteDelete);
    if(histoAcceptance_08_PCMDMC)histoAcceptance_08_PCMDMC->Write(Form("histo%sInAcc_08_PCMDMC",meson.Data()),TObject::kWriteDelete);
    if(histoAcceptance_08_PCMPHOS)histoAcceptance_08_PCMPHOS->Write(Form("histo%sInAcc_08_PCMPHOS",meson.Data()),TObject::kWriteDelete);
    if(histoAcceptance_08_PCMPHOSR2)histoAcceptance_08_PCMPHOSR2->Write(Form("histo%sInAcc_08_PCMPHOSR2",meson.Data()),TObject::kWriteDelete);
    if(histoAcceptance_09_PCM)histoAcceptance_09_PCM->Write(Form("histo%sInAcc_09_PCM",meson.Data()),TObject::kWriteDelete);
    if(histoAcceptance_09_PCMEDC)histoAcceptance_09_PCMEDC->Write(Form("histo%sInAcc_09_PCMEDC",meson.Data()),TObject::kWriteDelete);
    if(histoAcceptance_09_PCMEMC)histoAcceptance_09_PCMEMC->Write(Form("histo%sInAcc_09_PCMEMC",meson.Data()),TObject::kWriteDelete);
    if(histoAcceptance_09_PCMEMCR1)histoAcceptance_09_PCMEMCR1->Write(Form("histo%sInAcc_09_PCMEMCR1",meson.Data()),TObject::kWriteDelete);
    if(histoAcceptance_09_PCMDMC)histoAcceptance_09_PCMDMC->Write(Form("histo%sInAcc_09_PCMDMC",meson.Data()),TObject::kWriteDelete);
    if(histoAcceptance_09_PCMPHOS)histoAcceptance_09_PCMPHOS->Write(Form("histo%sInAcc_09_PCMPHOS",meson.Data()),TObject::kWriteDelete);
    if(histoAcceptance_09_PCMPHOSR2)histoAcceptance_09_PCMPHOSR2->Write(Form("histo%sInAcc_09_PCMPHOSR2",meson.Data()),TObject::kWriteDelete);

    if(histoGenerated_mEDC)histoGenerated_mEDC->Write(Form("histo%sGen_mEDC",meson.Data()),TObject::kWriteDelete);
    if(histoGenerated_mEMC)histoGenerated_mEMC->Write(Form("histo%sGen_mEMC",meson.Data()),TObject::kWriteDelete);
    if(histoGenerated_mDMC)histoGenerated_mDMC->Write(Form("histo%sGen_mDMC",meson.Data()),TObject::kWriteDelete);
    if(histoGenerated_EDC)histoGenerated_EDC->Write(Form("histo%sGen_EDC",meson.Data()),TObject::kWriteDelete);
    if(histoGenerated_EMC)histoGenerated_EMC->Write(Form("histo%sGen_EMC",meson.Data()),TObject::kWriteDelete);
    if(histoGenerated_EMCR1)histoGenerated_EMCR1->Write(Form("histo%sGen_EMCR1",meson.Data()),TObject::kWriteDelete);
    if(histoGenerated_DMC)histoGenerated_DMC->Write(Form("histo%sGen_DMC",meson.Data()),TObject::kWriteDelete);
    if(histoGenerated_PHOS)histoGenerated_PHOS->Write(Form("histo%sGen_PHOS",meson.Data()),TObject::kWriteDelete);
    if(histoGenerated_PHOSR2)histoGenerated_PHOSR2->Write(Form("histo%sGen_PHOSR2",meson.Data()),TObject::kWriteDelete);
    if(histoGenerated_08_PCM)histoGenerated_08_PCM->Write(Form("histo%sGen_08_PCM",meson.Data()),TObject::kWriteDelete);
    if(histoGenerated_08_PCMEDC)histoGenerated_08_PCMEDC->Write(Form("histo%sGen_08_PCMEDC",meson.Data()),TObject::kWriteDelete);
    if(histoGenerated_08_PCMEMC)histoGenerated_08_PCMEMC->Write(Form("histo%sGen_08_PCMEMC",meson.Data()),TObject::kWriteDelete);
    if(histoGenerated_08_PCMEMCR1)histoGenerated_08_PCMEMCR1->Write(Form("histo%sGen_08_PCMEMCR1",meson.Data()),TObject::kWriteDelete);
    if(histoGenerated_08_PCMDMC)histoGenerated_08_PCMDMC->Write(Form("histo%sGen_08_PCMDMC",meson.Data()),TObject::kWriteDelete);
    if(histoGenerated_08_PCMPHOS)histoGenerated_08_PCMPHOS->Write(Form("histo%sGen_08_PCMPHOS",meson.Data()),TObject::kWriteDelete);
    if(histoGenerated_08_PCMPHOSR2)histoGenerated_08_PCMPHOSR2->Write(Form("histo%sGen_08_PCMPHOSR2",meson.Data()),TObject::kWriteDelete);
    if(histoGenerated_09_PCM)histoGenerated_09_PCM->Write(Form("histo%sGen_09_PCM",meson.Data()),TObject::kWriteDelete);
    if(histoGenerated_09_PCMEDC)histoGenerated_09_PCMEDC->Write(Form("histo%sGen_09_PCMEDC",meson.Data()),TObject::kWriteDelete);
    if(histoGenerated_09_PCMEMC)histoGenerated_09_PCMEMC->Write(Form("histo%sGen_09_PCMEMC",meson.Data()),TObject::kWriteDelete);
    if(histoGenerated_09_PCMEMCR1)histoGenerated_09_PCMEMCR1->Write(Form("histo%sGen_09_PCMEMCR1",meson.Data()),TObject::kWriteDelete);
    if(histoGenerated_09_PCMDMC)histoGenerated_09_PCMDMC->Write(Form("histo%sGen_09_PCMDMC",meson.Data()),TObject::kWriteDelete);
    if(histoGenerated_09_PCMPHOS)histoGenerated_09_PCMPHOS->Write(Form("histo%sGen_09_PCMPHOS",meson.Data()),TObject::kWriteDelete);
    if(histoGenerated_09_PCMPHOSR2)histoGenerated_09_PCMPHOSR2->Write(Form("histo%sGen_09_PCMPHOSR2",meson.Data()),TObject::kWriteDelete);

    if(histoGammaStudyRvsMesonPt)histoGammaStudyRvsMesonPt->Write("histoGammaStudyRvsMesonPt",TObject::kWriteDelete);
    if(histoGammaStudyEtavsMesonPt)histoGammaStudyEtavsMesonPt->Write("histoGammaStudyEtavsMesonPt",TObject::kWriteDelete);
    if(histoGammaStudyPhivsMesonPt)histoGammaStudyPhivsMesonPt->Write("histoGammaStudyPhivsMesonPt",TObject::kWriteDelete);
    filePhotoQAWrite->Write();
    filePhotoQAWrite->Close();

    f->Close();
    delete f;
}
