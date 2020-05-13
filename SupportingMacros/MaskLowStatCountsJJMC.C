/*****************************************************************************
******         provided by Gamma Conversion Group, PWGGA,               ******
******        Friederike Bock, friederike.bock@cern.ch                  ******
*****************************************************************************/

#include <Riostream.h>
#include <string.h>
#include "TChain.h"
#include "TH1.h"
#include "TTree.h"
#include "TKey.h"
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
#include "TASImage.h"
#include "TPostScript.h"
#include "TGraphErrors.h"
#include "TArrow.h"
#include "TGraphAsymmErrors.h"
#include "TGaxis.h"
#include "TMarker.h"
#include "Math/WrappedTF1.h"
#include "Math/BrentRootFinder.h"
#include "../CommonHeaders/PlottingGammaConversionHistos.h"
#include "../CommonHeaders/PlottingGammaConversionAdditional.h"
#include "../CommonHeaders/ExtractSignalBinning.h"
#include "../CommonHeaders/ConversionFunctionsBasicsAndLabeling.h"
#include "../CommonHeaders/ConversionFunctions.h"



void MaskForPtBinsForHists (TObject* object, TList* listIn, TList* listOut, Double_t maxPtPi0, Double_t maxPtEta) {

    TString nameHistoESD        = "ESD_Mother_InvMass_Pt, ESD_Background_InvMass_Pt";
    TString nameHistsMCPi0      = "MC_Pi0_Pt, MC_Pi0_WOWeights_Pt, MC_Pi0InAcc_Pt, MC_Pi0_WOEventWeights_Pt, MC_Pi0WOEvtWeightInAcc_Pt";
    TString nameHistsMCPi02D    = "MC_SecPi0_Pt_Source, MC_SecPi0InAcc_Pt_Source";
    TString nameHistsMCEta      = "MC_Eta_Pt, MC_Eta_WOWeights_Pt, MC_EtaInAcc_Pt, MC_Eta_WOEventWeights_Pt, MC_EtaWOEvtWeightInAcc_Pt, MC_SecEta_Pt";
    TString nameHistsTruePi0    = "ESD_TruePi0_InvMass_Pt, ESD_TrueDoubleCountPi0_InvMass_Pt, ESD_TruePrimaryPi0_InvMass_Pt, ESD_TruePrimaryPi0W0Weights_InvMass_Pt, ESD_TrueSecondaryPi0_InvMass_Pt, ESD_TrueSecondaryPi0FromK0s_InvMass_Pt, ESD_TrueSecondaryPi0FromK0l_InvMass_Pt, ESD_TrueSecondaryPi0FromEta_InvMass_Pt, ESD_TrueSecondaryPi0FromLambda_InvMass_Pt, ESD_TruePi0_InvMass_vs_Pt_Alpha, ESD_TruePi0PureGamma_InvMass_vs_Pt_Alpha, ESD_Mother_InvMass_E_Calib, ESD_Background_InvMass_E_Calib";
    TString nameProfileTruePi0  = "ESD_TruePrimaryPi0Weights_InvMass_Pt";
    TString nameHistsTrueEta    = "ESD_TrueEta_InvMass_Pt, ESD_TrueDoubleCountEta_InvMass_Pt, ESD_TruePrimaryEta_InvMass_Pt, ESD_TruePrimaryEtaW0Weights_InvMass_Pt";
    TString nameProfileTrueEta  = "ESD_TruePrimaryEtaWeights_InvMass_Pt";

    
    if (nameHistoESD.Contains(object->GetName())){
        TH2F* h2    = (TH2F*)listIn->FindObject(object->GetName());
        for (Int_t ptBin = 1; ptBin < h2->GetNbinsY()+1; ptBin++){
            if (h2->GetYaxis()->GetBinCenter(ptBin) < maxPtPi0 && h2->GetYaxis()->GetBinCenter(ptBin) < maxPtEta )
                continue;
            for (Int_t invBin = 1; invBin < h2->GetNbinsX()+1; invBin++){
                if (h2->GetXaxis()->GetBinCenter(invBin) > 0.3){
                   if (h2->GetYaxis()->GetBinCenter(ptBin) > maxPtEta){
                       h2->SetBinContent(invBin, ptBin, 0);
                       h2->SetBinError(invBin, ptBin, 0);
                   }
                } else {
                   if (h2->GetYaxis()->GetBinCenter(ptBin) > maxPtPi0){
                       h2->SetBinContent(invBin, ptBin, 0);
                       h2->SetBinError(invBin, ptBin, 0);
                   }                    
                }
            }
        }
        listOut->Add(h2);
    } else if (nameHistsTruePi0.Contains(object->GetName())){
        TH2F* h2    = (TH2F*)listIn->FindObject(object->GetName());
        for (Int_t ptBin = 1; ptBin < h2->GetNbinsY()+1; ptBin++){
            if (h2->GetYaxis()->GetBinCenter(ptBin) < maxPtPi0 )
                continue;
            for (Int_t invBin = 1; invBin < h2->GetNbinsX()+1; invBin++){
                if (h2->GetYaxis()->GetBinCenter(ptBin) > maxPtPi0){
                    h2->SetBinContent(invBin, ptBin, 0);
                    h2->SetBinError(invBin, ptBin, 0);
                }
            }
        }
        listOut->Add(h2);
    } else if (nameHistsMCPi02D.Contains(object->GetName())){
        TH2F* h2    = (TH2F*)listIn->FindObject(object->GetName());
        for (Int_t ptBin = 1; ptBin < h2->GetNbinsX()+1; ptBin++){
            if (h2->GetXaxis()->GetBinCenter(ptBin) < maxPtPi0 )
                continue;
            for (Int_t sbin = 1; sbin < h2->GetNbinsY()+1; sbin++){
                if (h2->GetXaxis()->GetBinCenter(ptBin) > maxPtPi0){
                    h2->SetBinContent(ptBin, sbin, 0);
                    h2->SetBinError(ptBin, sbin, 0);
                }
            }
        }
        listOut->Add(h2);    
    } else if (nameHistsMCPi0.Contains(object->GetName())){
        TH1F* h1    = (TH1F*)listIn->FindObject(object->GetName());
        for (Int_t ptBin = 1; ptBin < h1->GetNbinsX()+1; ptBin++){
            if (h1->GetXaxis()->GetBinCenter(ptBin) > maxPtPi0){
                h1->SetBinContent(ptBin, 0);
                h1->SetBinError(ptBin, 0);
            }
        }
        listOut->Add(h1);    
    } else if (nameHistsTrueEta.Contains(object->GetName())){
        TH2F* h2    = (TH2F*)listIn->FindObject(object->GetName());
        for (Int_t ptBin = 1; ptBin < h2->GetNbinsY()+1; ptBin++){
            if (h2->GetYaxis()->GetBinCenter(ptBin) < maxPtEta )
                continue;
            for (Int_t invBin = 1; invBin < h2->GetNbinsX()+1; invBin++){
                if (h2->GetYaxis()->GetBinCenter(ptBin) > maxPtEta){
                    h2->SetBinContent(invBin, ptBin, 0);
                    h2->SetBinError(invBin, ptBin, 0);
                }
            }
        }
        listOut->Add(h2);
    } else if (nameHistsMCEta.Contains(object->GetName())){
        TH1F* h1    = (TH1F*)listIn->FindObject(object->GetName());
        for (Int_t ptBin = 1; ptBin < h1->GetNbinsX()+1; ptBin++){
            if (h1->GetXaxis()->GetBinCenter(ptBin) > maxPtEta){
                h1->SetBinContent(ptBin, 0);
                h1->SetBinError(ptBin, 0);
            }
        }
        listOut->Add(h1);    
    } else {
        listOut->Add(object);
    }    
    
}

void MaskLowStatCountsJJMC( TString fileNameInput       = "",
                            TString fileNameOutput      = "",
                            Int_t mode                  = 0, 
                            TString optionEnergy        = ""
                         ){

    if ( !(!(mode == 10 || mode == 11) && !(mode >= 60 && mode < 70)) ){
        cout << "macro currently not defined for mode: " << mode << " Aborting!" << endl;
        return;
    }
    
    // read trees
    TFile* filesIn                  = new TFile(fileNameInput.Data());
    
    TString mesonName = "Pi0";
    if(mode >= 60 && mode < 70) mesonName = "HNM";

    TString fCent = "";
    if ((optionEnergy.Contains("Pb") || optionEnergy.Contains("Xe")) )
        fCent = "0-100%";
    
    Double_t binningPi0[400];
    Int_t maxNBinsPi0Abs            = 0;
    Int_t maxNBinsPi0               = GetBinning( binningPi0, maxNBinsPi0Abs, "Pi0", optionEnergy, mode, -1, kFALSE, fCent, kFALSE );
    Int_t maxNAllowedPi0            = 0;
    Int_t nRealTriggers             = 0;
    cout << "binning pi0" << endl;
    for (Int_t i= 0; i< maxNBinsPi0+1; i++){
        cout << binningPi0[i] << ", ";
    }
    cout << endl;

    Double_t binningEta[400];
    Int_t maxNBinsEtaAbs            = 0;
    Int_t maxNBinsEta               = GetBinning( binningEta, maxNBinsEtaAbs, "Eta", optionEnergy, mode, -1, kFALSE, fCent, kFALSE );
    cout << "binning eta" << endl;
    for (Int_t i= 0; i< maxNBinsEta+1; i++){
        cout << binningEta[i] << ", ";
    }
    cout << endl;

    
    TString autoDetectedMainDir     = AutoDetectMainTList(mode , filesIn);
    if (autoDetectedMainDir.CompareTo("") == 0){
        cout << "ERROR: trying to read file, which is incompatible with mode selected" << endl;;
        return;
    }
    TList *listMain                 = (TList*) filesIn->Get(autoDetectedMainDir.Data());

    TString cutNumber[40];
    Int_t nCuts                     = 0;
    for(Int_t i = 0; i<listMain->GetEntries(); i++){
        TList *l2 = (TList*) listMain->At(i);
        TString dirname = l2->GetName();
        if(dirname.BeginsWith("Cut") == 1){
            TString tempCut(dirname(11,dirname.Length()-11));
            cout << "detected: \t" << tempCut.Data() <<endl;
            cutNumber[nCuts]        = tempCut;
            nCuts++;
        }
    }

    TTree *treeCopyPhoton[40]       = {NULL};        
    TTree *treeCopyMeson[40]        = {NULL};       
    
    for (Int_t i = 0; i < nCuts; i++){
        treeCopyPhoton[i] = (TTree*)filesIn->Get(Form("%s Photon DCA tree",cutNumber[i].Data() ));
        if (treeCopyPhoton[i]){
            cout << "found tree" << endl;
            treeCopyPhoton[i]->SetName(Form("%s Photon DCA tree",cutNumber[i].Data() ));
        }
        treeCopyMeson[i] = (TTree*)filesIn->Get(Form("%s Meson DCA tree",cutNumber[i].Data() ));
        if (treeCopyMeson[i]){
            cout << "found tree" << endl;
            treeCopyMeson[i]->SetName(Form("%s Meson DCA tree",cutNumber[i].Data() ));
        }
    }
    
    TString listNameOutMain = GetDefaultMainTListName(mode);
    TFile *fileInter1        = new TFile(fileNameOutput.Data(),"RECREATE");
    TList *listInter1        = new TList();
    listInter1->SetName(listNameOutMain.Data());
    TList *listToCopyInter1[40]   = {NULL};        
    TList *listSubDirCopy1[200]   = {NULL};
    
    for (Int_t i = 0; i < nCuts; i++){
        TString nameCutFolder       = Form("Cut Number %s",cutNumber[i].Data());
        TList *l2                   = (TList*)listMain->FindObject(nameCutFolder.Data());

        listToCopyInter1[i]         = new TList();
        listToCopyInter1[i]->SetName(l2->GetName());
        
        TList *ESDContainer         = (TList*)l2->FindObject(Form("%s ESD histograms",cutNumber[i].Data()));
        if(ESDContainer == NULL){
            cout<<"ERROR: " << Form("ESD histograms %s",cutNumber[i].Data()) << " not Found in File"<<endl;
            return;
        }
        TH1F* histoNEventsWWeight   = (TH1F*)ESDContainer->FindObject("NEvents");
        TH1F* histoNEvents          = (TH1F*)ESDContainer->FindObject("NEventsWOWeight");
        if(!histoNEvents){
            cout << "WARNING: could not finde NEventsWOWeight. Will use NEvents as well instead." << endl;
            histoNEvents            = (TH1F*)histoNEventsWWeight->Clone();
        }
        Double_t weightApplied      = histoNEventsWWeight->GetBinContent(1)/histoNEvents->GetBinContent(1);

        
        TList *TrueContainer        = (TList*)l2->FindObject(Form("%s True histograms",cutNumber[i].Data()));
        if(TrueContainer == NULL){
            cout<<"ERROR: " << Form("True histograms %s",cutNumber[i].Data()) << " not Found in File"<<endl;
            return;
        }
        Double_t maxPtTrustPi0 = 100;
        Double_t maxPtTrustEta = 100;

        TH2F* histoTruePi0InvMassPt = (TH2F*)TrueContainer->FindObject(Form("ESD_True%s_InvMass_Pt",mesonName.Data()));
        TH1D* histoTruePi0Pt        = (TH1D*)histoTruePi0InvMassPt->ProjectionY("TruePi0Pt",histoTruePi0InvMassPt->GetXaxis()->FindBin(0.05), histoTruePi0InvMassPt->GetXaxis()->FindBin(0.18), "e");
        histoTruePi0Pt->Scale(1./weightApplied);
        histoTruePi0Pt      = (TH1D*)histoTruePi0Pt->Rebin(maxNBinsPi0,Form("TruePi0PtOrg_%s",cutNumber[i].Data()),binningPi0); 
        Int_t k = histoTruePi0Pt->GetNbinsX();
        while (histoTruePi0Pt->GetBinContent(k) < 10 && k > 0) k--;   
        maxPtTrustPi0 = histoTruePi0Pt->GetXaxis()->GetBinUpEdge(k);
        if ( !(mode == 10 || mode == 11) && !(mode >= 60 && mode < 70) ){
            TH2F* histoTrueEtaInvMassPt = (TH2F*)TrueContainer->FindObject("ESD_TrueEta_InvMass_Pt");
            TH1D* histoTrueEtaPt        = (TH1D*)histoTrueEtaInvMassPt->ProjectionY("TrueEtaPt",histoTrueEtaInvMassPt->GetXaxis()->FindBin(0.45), histoTrueEtaInvMassPt->GetXaxis()->FindBin(0.6), "e");
            histoTrueEtaPt->Scale(1./weightApplied);
            histoTrueEtaPt      = (TH1D*)histoTrueEtaPt->Rebin(maxNBinsEta,Form("TrueEtaPtOrg_%s",cutNumber[i].Data()),binningEta); 
            k = histoTrueEtaPt->GetNbinsX();
            while (histoTrueEtaPt->GetBinContent(k) < 10 && k > 0) k--;   
            maxPtTrustEta = histoTrueEtaPt->GetXaxis()->GetBinUpEdge(k);
        }
        cout << i << "\t" << maxPtTrustPi0 << "\t" << maxPtTrustEta << endl; 

        if (listToCopyInter1[i]){
            for (Int_t k = 0; k < l2->GetEntries();k++){
                TList *l3 = (TList*)l2->At(k);
                TString listName = l3->GetName();
                if (listName.BeginsWith(cutNumber[i].Data())){
                    Int_t cEnt = i*(Int_t)(l2->GetEntries()) + k;
                    listSubDirCopy1[cEnt] = new TList();                        
                    listSubDirCopy1[cEnt]->SetName(listName.Data());
                    Int_t maxObjects = l3->GetEntries();
                    for (Int_t m = 0; m < maxObjects; m++ ){
                        TObject* o1 = (TObject*)l3->At(m);
                        MaskForPtBinsForHists (o1, l3, listSubDirCopy1[cEnt], maxPtTrustPi0, maxPtTrustEta);
                    }
                    listToCopyInter1[i]->Add(listSubDirCopy1[cEnt]);
                } else {
                    l3->SetName(listName.Data());
                    listToCopyInter1[i]->Add(l3);
                }
            }
        } 
        listInter1->Add(listToCopyInter1[i]);
    }
    fileInter1->cd();
    listInter1->Write("",TObject::kSingleKey);
    for (Int_t i = 0;  i < nCuts; i++){
        if (treeCopyPhoton[i]) treeCopyPhoton[i]->CloneTree(-1,"fast");
        if (treeCopyMeson[i]) treeCopyMeson[i]->CloneTree(-1,"fast");
    }
    fileInter1->Write();
    fileInter1->Close();
    
    if (filesIn) delete filesIn;
    
}
