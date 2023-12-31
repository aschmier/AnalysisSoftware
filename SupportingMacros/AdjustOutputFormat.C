/*****************************************************************************
******         provided by Gamma Conversion Group, PWGGA,               ******
******        Friederike Bock, friederike.bock@cern.ch                  ******
*****************************************************************************/

#include <Riostream.h>
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

//**************************************************************************************************
//**************************** Main function *******************************************************
//**************************************************************************************************
void AdjustOutputFormat(    TString energy          = "",
                            TString centrality      = "",
                            Int_t mode              = -1,
                            Int_t optMode           = -1,
                            TString meson           = "",
                            TString inputFileName1  = "",
                            TString inputFileName2  = "",
                            TString outputFileName  = "",
                            TString additionalName  = ""
                        ){

    StyleSettingsThesis();
    SetPlotStyle();

    cout << energy.Data() << "\t" << centrality << endl;
    cout << "File 1: " << inputFileName1.Data() << endl;
    cout << "File 2: " << inputFileName2.Data() << endl;
    cout << "Output file name: " << outputFileName.Data() << endl;

//     mode: 0....13 (PCM, PCM-EMC, PCM-PHOS, EMC, PHOS ...)
//     optMode:
//         0 - combine,
//         1 - copy and add hists to first folder struct
//         2 - bring to proper format
//         3 - reformat to new ProduceFinalResultsPatchedTriggersOutput

    //**********************************************************************************************************************
    if (optMode == 2){
        TFile fileOutputFile(outputFileName.Data(),"RECREATE");

        TString outputDirName           = Form("%s%s%s", meson.Data(), energy.Data(), centrality.Data());
        fileOutputFile.mkdir(outputDirName.Data());
        TDirectoryFile* directoryWrite  = (TDirectoryFile*)fileOutputFile.Get(outputDirName.Data());

        if (mode == 5 && energy.CompareTo("pPb_5.023TeV") == 0 && meson.CompareTo("Pi0") == 0){
            TFile* fileInput1                   = new TFile(inputFileName1.Data());
            TH1F* histoSysSources[7]            = {NULL, NULL, NULL, NULL, NULL, NULL};
            TString nameSourcesRead[7]          = {"Ext", "Acc", "Con", "Bge", "Non", "ESc", "TOF"};
            TString nameSourcesWrite[7]         = {"SignalExtraction", "Acceptance", "Material", "BGEstimate", "NonLinearity", "EnergyScale", "TimeOfFlight"};
            for (Int_t i= 0; i<7; i++ ){
                cout << "reading: " << Form("hSys%s",nameSourcesRead[i].Data()) << endl;
                histoSysSources[i]              = (TH1F*)fileInput1->Get(Form("hSys%s",nameSourcesRead[i].Data()));
                histoSysSources[i]->SetName(Form("histoSysSources_%s",nameSourcesWrite[i].Data()));
                directoryWrite->Add(histoSysSources[i]);
            }
            TH1F* histoSysTot                   = (TH1F*)fileInput1->Get("hSystTotal");
            histoSysTot->SetName("histoSysSources_Total");
            directoryWrite->Add(histoSysTot);
            TF1* fNonLinFunct                   = (TF1*)fileInput1->Get("fNon9");
            fNonLinFunct->SetName("NonLinearityCorrectionFunction");
            directoryWrite->Add(fNonLinFunct);
            TH1F* histoInvYieldStat             = (TH1F*)fileInput1->Get("hCor_stat");
            histoInvYieldStat->SetName("CorrectedYieldPi0");
            directoryWrite->Add(histoInvYieldStat);
            TH1F* histoInvYieldSys              = (TH1F*)fileInput1->Get("hCor_syst");
            histoInvYieldSys->SetName("histPi0SystError");
            directoryWrite->Add(histoInvYieldSys);
            TGraphAsymmErrors* graphInvYieldSys = new TGraphAsymmErrors(histoInvYieldSys);
            graphInvYieldSys->SetName("Pi0SystError");
            directoryWrite->Add(graphInvYieldSys);

            TH1F* histoMass                     = (TH1F*)fileInput1->Get("Gmean_Real");
            histoMass->SetName("Pi0_Mass_data");
            directoryWrite->Add(histoMass);
            TH1F* histoMassMC                   = (TH1F*)fileInput1->Get("Gmean_MC");
            histoMassMC->SetName("Pi0_Mass_MC");
            directoryWrite->Add(histoMassMC);
            TH1F* histoWidth                    = (TH1F*)fileInput1->Get("Gsigma_Real");
            histoWidth->SetName("Pi0_Width_data");
            TH1F* histoWidthMC                  = (TH1F*)fileInput1->Get("Gsigma_MC");
            directoryWrite->Add(histoWidth);
            histoWidthMC->SetName("Pi0_Width_MC");
            directoryWrite->Add(histoWidthMC);

            TFile* fileInput2                   = new TFile(inputFileName2.Data());
            TH1F* histoEffi                     = (TH1F*)fileInput2->Get("hEfficiency");
            histoEffi->SetName("EffTimesAccPi0");
            directoryWrite->Add(histoEffi);
            TF1* fitEffi                        = (TF1*)fileInput2->Get("fEfficiency_BC");
            fitEffi->SetName("fitEfficiencyPi0");
            directoryWrite->Add(fitEffi);

            fstream fileSysOutputCompilation;
            TString fFileSysErrName             = Form("SystematicErrorAveragedSinglePHOSPHOS_Pi0_pPb_5_023TeV_createdFromRootFile.dat");
            fileSysOutputCompilation.open(fFileSysErrName, ios::out);

            for (Int_t i = 1;i< histoInvYieldStat->GetNbinsX()+1; i++ ){
                if (i == 1) {
                    fileSysOutputCompilation << "Pt" << "\t";
                    for (Int_t k= 0; k<7; k++ ){
                        fileSysOutputCompilation << nameSourcesWrite[k] << "\t";
                    }
                    fileSysOutputCompilation << endl;
                }
                if (histoInvYieldStat->GetBinContent(i) > 0){
                    fileSysOutputCompilation <<  histoInvYieldStat->GetBinCenter(i) << "\t";
                    Double_t currentPt      = histoInvYieldStat->GetBinCenter(i);
                    Double_t currentTotSys  = 0;
                    for (Int_t k= 0; k<7; k++ ){
                        fileSysOutputCompilation << histoSysSources[k]->GetBinContent(histoSysSources[k]->FindBin(currentPt))*100 << "\t";
                        currentTotSys       = currentTotSys+TMath::Power(histoSysSources[k]->GetBinContent(histoSysSources[k]->FindBin(currentPt))*100,2);
                    }
                    fileSysOutputCompilation << TMath::Sqrt(currentTotSys) << endl;
                }
            }
        }
        directoryWrite->Write();
        fileOutputFile.Close();
    } else if (optMode == 3){

        if (mode == 1){
            TFile* fileInput1                   = new TFile(inputFileName1.Data());
            TH1D* CorrectedYieldPi0             = (TH1D*)fileInput1->Get("Pi0_pPb_5.023TeV_0-100%/CorrectedYieldPi0");
            TH1D* RAWYieldPerEventsPi0          = (TH1D*)fileInput1->Get("Pi0_pPb_5.023TeV_0-100%/RAWYieldPerEventsPi0");
            TH1D* AcceptancePi0                 = (TH1D*)fileInput1->Get("Pi0_pPb_5.023TeV_0-100%/AcceptancePi0");
            TH1D* EfficiencyPi0                 = (TH1D*)fileInput1->Get("Pi0_pPb_5.023TeV_0-100%/EfficiencyPi0");
            TH1D* FWHMPi0MeV                    = (TH1D*)fileInput1->Get("Pi0_pPb_5.023TeV_0-100%/FWHMPi0MeV");
            TH1D* MassPi0                       = (TH1D*)fileInput1->Get("Pi0_pPb_5.023TeV_0-100%/MassPi0");
            TH1D* TrueMassPi0                   = (TH1D*)fileInput1->Get("Pi0_pPb_5.023TeV_0-100%/TrueMassPi0");
            TH1D* TrueFWHMPi0MeV                = (TH1D*)fileInput1->Get("Pi0_pPb_5.023TeV_0-100%/TrueFWHMPi0MeV");
            FWHMPi0MeV->Scale(1./1000.);
            TrueFWHMPi0MeV->Scale(1./1000.);

            TGraphAsymmErrors* Pi0SystError     = (TGraphAsymmErrors*)fileInput1->Get("Pi0_pPb_5.023TeV_0-100%/Pi0SystError");
            TGraphAsymmErrors* graphCorrectedYieldPi0   = new TGraphAsymmErrors(CorrectedYieldPi0);
            while (Pi0SystError->GetY()[0] == 0) Pi0SystError->RemovePoint(0);
            while (graphCorrectedYieldPi0->GetY()[0] == 0) graphCorrectedYieldPi0->RemovePoint(0);

            TGraphAsymmErrors* graphAcceptancePi0       = new TGraphAsymmErrors(AcceptancePi0);
            TGraphAsymmErrors* graphEfficiencyPi0       = new TGraphAsymmErrors(EfficiencyPi0);
            TGraphAsymmErrors* graphMassPi0             = new TGraphAsymmErrors(MassPi0);
            TGraphAsymmErrors* graphTrueMassPi0         = new TGraphAsymmErrors(TrueMassPi0);
            TGraphAsymmErrors* graphFWHMPi0MeV          = new TGraphAsymmErrors(FWHMPi0MeV);
            TGraphAsymmErrors* graphTrueFWHMPi0MeV      = new TGraphAsymmErrors(TrueFWHMPi0MeV);

            TH1D* histoEffTimesAccPi0                       = (TH1D*)EfficiencyPi0->Clone("EffTimeAcc");
            histoEffTimesAccPi0->Multiply(AcceptancePi0);
            histoEffTimesAccPi0->Scale(1.6*2*TMath::Pi());
            TGraphAsymmErrors* graphEffTimesAccPi0          = new TGraphAsymmErrors(histoEffTimesAccPi0);

            while(graphEffTimesAccPi0->GetY()[0] == 0){
                graphMassPi0->RemovePoint(0);
                graphFWHMPi0MeV->RemovePoint(0);
                graphTrueMassPi0->RemovePoint(0);
                graphTrueFWHMPi0MeV->RemovePoint(0);
                graphAcceptancePi0->RemovePoint(0);
                graphEfficiencyPi0->RemovePoint(0);
                graphEffTimesAccPi0->RemovePoint(0);
            }

            TFile* fileOutputForComparisonFullyCorrected = new TFile(outputFileName,"RECREATE");
            fileOutputForComparisonFullyCorrected->mkdir(Form("Pi0%s%s%s",centrality.Data(),energy.Data(), additionalName.Data()));
            TDirectoryFile* directoryPi0 = (TDirectoryFile*)fileOutputForComparisonFullyCorrected->Get(Form("Pi0%s%s%s",centrality.Data(),energy.Data(), additionalName.Data()));
            fileOutputForComparisonFullyCorrected->cd(Form("Pi0%s%s%s",centrality.Data(),energy.Data(), additionalName.Data()));
                if (graphCorrectedYieldPi0)  graphCorrectedYieldPi0->Write("graphCorrectedYieldPi0",TObject::kOverwrite);
                if (CorrectedYieldPi0)  CorrectedYieldPi0->Write("CorrectedYieldPi0",TObject::kOverwrite);
                if (Pi0SystError)   Pi0SystError->Write("Pi0SystError",TObject::kOverwrite);

                if (graphAcceptancePi0)                 graphAcceptancePi0->Write("AcceptancePi0", TObject::kOverwrite);
                if (graphEfficiencyPi0)                 graphEfficiencyPi0->Write("EfficiencyPi0", TObject::kOverwrite);
                if (graphEffTimesAccPi0)                graphEffTimesAccPi0->Write("EffTimesAccPi0", TObject::kOverwrite);
                if (graphMassPi0)                       graphMassPi0->Write("Pi0_Mass_data", TObject::kOverwrite);
                if (graphTrueMassPi0)                   graphTrueMassPi0->Write("Pi0_Mass_MC", TObject::kOverwrite);
                if (graphFWHMPi0MeV)                    graphFWHMPi0MeV->Write("Pi0_Width_data", TObject::kOverwrite);
                if (graphTrueFWHMPi0MeV)                graphTrueFWHMPi0MeV->Write("Pi0_Width_MC", TObject::kOverwrite);

            fileOutputForComparisonFullyCorrected->Write();
            fileOutputForComparisonFullyCorrected->Close();

        }

        if (mode == 5 && energy.CompareTo("PbPb_5TeV")==0){

            cout << "here" << endl;
            TFile* fileInput1                         = new TFile(inputFileName1.Data());
            TFile* fileOutput                         = new TFile(outputFileName,"RECREATE");

            TString centrangePHOS[9]                  = {"0_10_V0M", "10_20_V0M", "20_40_V0M", "40_60_V0M", "60_80_V0M", "0_5_V0M", "0_90_V0M", "20_50_V0M", "30_50_V0M"};
            TString centrangePCM[9]                   = {"0-10%","10-20%", "20-40%", "40-60%", "60-80%", "0-5%", "0-90%", "20-50%", "30-50%"};

            for (Int_t i = 0; i < 9; i++){
                cout << "adjusting cent bin: "  << i << "\t" << centrangePCM[i].Data() << endl;

                TDirectoryFile*     directoryPi0    = NULL;
                directoryPi0                        = (TDirectoryFile*)fileInput1->Get(Form("Pi05TeV_%s", centrangePHOS[i].Data()));
                if (directoryPi0){
                    TH1D* CorrectedYieldPi0              = (TH1D*)directoryPi0->Get("CorrectedYieldPi0");
                    TGraphAsymmErrors* Pi0SystError      = (TGraphAsymmErrors*)directoryPi0->Get("CorrectedYieldPi0Sys");
                    TH1D* EffAccPi0                      = (TH1D*)directoryPi0->Get("EffTimesAcc");
                    TH1D* MassPi0                        = (TH1D*)directoryPi0->Get("Pi0_Mass_data");
                    TH1D* TrueMassPi0                    = (TH1D*)directoryPi0->Get("Pi0_Mass_MC");
                    TH1D* FWHMPi0MeV                     = (TH1D*)directoryPi0->Get("Pi0_Width_data");
                    TH1D* TrueFWHMPi0MeV                 = (TH1D*)directoryPi0->Get("Pi0_Width_MC");

                    TGraphAsymmErrors* graphYieldPi0            = new TGraphAsymmErrors(CorrectedYieldPi0);
                    TGraphAsymmErrors* graphAccEffPi0           = new TGraphAsymmErrors(EffAccPi0);
                    TGraphAsymmErrors* graphMassPi0             = new TGraphAsymmErrors(MassPi0);
                    TGraphAsymmErrors* graphTrueMassPi0         = new TGraphAsymmErrors(TrueMassPi0);
                    TGraphAsymmErrors* graphFWHMPi0MeV          = new TGraphAsymmErrors(FWHMPi0MeV);
                    TGraphAsymmErrors* graphTrueFWHMPi0MeV      = new TGraphAsymmErrors(TrueFWHMPi0MeV);
                    TString nameCentFolder              = Form("Pi0%sPbPb_5.02TeV_V0M",centrangePCM[i].Data() );
                    fileOutput->mkdir(nameCentFolder.Data());
                    TDirectoryFile* directoryPi0Output  = (TDirectoryFile*)fileOutput->Get(nameCentFolder.Data());
                    fileOutput->cd(nameCentFolder.Data());

                    if(CorrectedYieldPi0)       CorrectedYieldPi0->Write("CorrectedYieldPi0");
                    if (graphYieldPi0) graphYieldPi0->Write("graphCorrectedYieldPi0");
                    if(Pi0SystError)            Pi0SystError->Write("Pi0SystError");
                    if(graphAccEffPi0)               graphAccEffPi0->Write("EffTimesAccPi0");
                    if(graphMassPi0)                 graphMassPi0->Write("Pi0_Mass_data");
                    if(graphTrueMassPi0)             graphTrueMassPi0->Write("Pi0_Mass_MC");
                    if(graphFWHMPi0MeV)              graphFWHMPi0MeV->Write("Pi0_Width_data");
                    if(graphTrueFWHMPi0MeV)          graphTrueFWHMPi0MeV->Write("Pi0_Width_MC");
                }
                TDirectoryFile*     directoryEta    = NULL;
                directoryEta                        = (TDirectoryFile*)fileInput1->Get(Form("Eta5TeV_%s", centrangePHOS[i].Data()));
                if (directoryEta){
                    TH1D* CorrectedYieldEta              = (TH1D*)directoryEta->Get("CorrectedYieldEta");
                    TGraphAsymmErrors* EtaSystError      = (TGraphAsymmErrors*)directoryEta->Get("CorrectedYieldEtaSys");
                    TH1D* EffAccEta                      = (TH1D*)directoryEta->Get("EffTimesAcc");
                    TH1D* MassEta                        = (TH1D*)directoryEta->Get("Eta_Mass_data");
                    TH1D* TrueMassEta                    = (TH1D*)directoryEta->Get("Eta_Mass_MC");
                    TH1D* FWHMEtaMeV                     = (TH1D*)directoryEta->Get("Eta_Width_data");
                    TH1D* TrueFWHMEtaMeV                 = (TH1D*)directoryEta->Get("Eta_Width_MC");
                    TH1D* EtaToPi0StatError              = (TH1D*)directoryEta->Get("EtaToPi0StatError");
                    TGraphAsymmErrors* graphEtaToPi0StatError      = (TGraphAsymmErrors*)directoryEta->Get("graphEtaToPi0StatError");
                    TGraphAsymmErrors* graphEtaToPi0SystError      = (TGraphAsymmErrors*)directoryEta->Get("graphEtaToPi0SystError");

                    TGraphAsymmErrors* graphYieldEta            = new TGraphAsymmErrors(CorrectedYieldEta);
                    TGraphAsymmErrors* graphAccEffEta           = new TGraphAsymmErrors(EffAccEta);
                    TGraphAsymmErrors* graphMassEta             = new TGraphAsymmErrors(MassEta);
                    TGraphAsymmErrors* graphTrueMassEta         = new TGraphAsymmErrors(TrueMassEta);
                    TGraphAsymmErrors* graphFWHMEtaMeV          = new TGraphAsymmErrors(FWHMEtaMeV);
                    TGraphAsymmErrors* graphTrueFWHMEtaMeV      = new TGraphAsymmErrors(TrueFWHMEtaMeV);
                    TString nameCentFolder                      = Form("Eta%sPbPb_5.02TeV_V0M",centrangePCM[i].Data() );
                    fileOutput->mkdir(nameCentFolder.Data());
                    TDirectoryFile* directoryEtaOutput = (TDirectoryFile*)fileOutput->Get(nameCentFolder.Data());
                    fileOutput->cd(nameCentFolder.Data());

                    if(CorrectedYieldEta)  CorrectedYieldEta->Write("CorrectedYieldEta");
                    if (graphYieldEta) graphYieldEta->Write("graphCorrectedYieldEta");
                    if(EtaSystError)         EtaSystError->Write("EtaSystError");
                    if(graphAccEffEta) graphAccEffEta->Write("EffTimesAccEta");
                    if(graphMassEta) graphMassEta->Write("Eta_Mass_data");
                    if(graphTrueMassEta) graphTrueMassEta->Write("Eta_Mass_MC");
                    if(graphFWHMEtaMeV) graphFWHMEtaMeV->Write("Eta_Width_data");
                    if(graphTrueFWHMEtaMeV) graphTrueFWHMEtaMeV->Write("Eta_Width_MC");
                    if(EtaToPi0StatError) EtaToPi0StatError->Write("EtaToPi0StatError");
                    if(graphEtaToPi0StatError) graphEtaToPi0StatError->Write("graphEtaToPi0StatError");
                    if(graphEtaToPi0SystError) graphEtaToPi0SystError->Write("EtaToPi0SystError");
                }
            }
            fileOutput->Write();
            fileOutput->Close();

        }
    }

    //**********************************************************************************************************************

}
