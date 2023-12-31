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
#include <vector>
#include "../CommonHeaders/ConversionFunctionsBasicsAndLabeling.h"

void prepareMultWeightFile_pp(TString inputPath = "/Users/marin/analysis/multWeightFile",
                              TString eventAndphotonCutNo = "00010103_0d000009266300008850404000",
                              TString targetFileName = "histosForMultWeighting_pp.root",
                              TString energy = ""){

    // Macro copied  from prepareMultWeightFile() that is for PbPb. Meike Danisch
    // Adapted for pp. Ana Marin
    // this macro reads GoodESDTracks histograms from GammaConv files
    // for different centrality classes, for data and different MCs
    // and saves them in a root file which has to be handed over to the addtask to apply multiplicity weighting
    // has to be compiled!
    // current settings are for LHC15o
    // cureent settings are for LHC16d


    //######### settings#################
    const Int_t nProductions       = 3;
    const Int_t nProductionsUsed   = 3;
    const Int_t nCentralityClasses = 1;

    TString centralityClasses1[nCentralityClasses] = {"000"};   // config 23

    // file names for Pythia - Phojet - all  MB
    TString productionNames13TeV[nProductions] = {"LHC16d", "LHC16P1Pyt8","LHC18d6a2" };
    TString fileNames13TeV[nProductions]       = {"GammaConv_Material_LHC16d_23.root",  // LHC16d
                                                  "GammaConv_Material_MC_LHC17f6total_23.root", // Pythia 13 TeV
                                                  "GammaConv_Material_MC_LHC18d6a2_23.root" };  // Phojet 13 TeV

    TString productionNames5TeV[nProductions] = {"LHC17p", "LHC17l3b","LHC18d6b" };
    TString fileNames5TeV[nProductions]       = {"MaterialBudget_LHC17p_fastwoSDD_LowInt_23.root",  //
                                                 "MaterialBudget_Pythia_fastwoSDD_LowInt_23.root", //
                                                 "MaterialBudget_Phojet_fastwoSDD_LowInt_23.root" };  //

    TString productions;
    TString fileNames;
    TString centralityClass;

    //########## start ########################
    TFile targetFile( Form("%s/%s", inputPath.Data(),targetFileName.Data()),"RECREATE");

    // go through productions j
    for(Int_t j=0; j<nProductionsUsed; j++){
        if(energy.Contains("5TeV")){
            productions = productionNames5TeV[j];
            fileNames   = fileNames5TeV[j];
        } else if (energy.Contains("13TeV")){
            productions = productionNames13TeV[j];
            fileNames   = fileNames13TeV[j];
        }
        cout << productions.Data() << endl;
        centralityClass   = centralityClasses1[0];

        TString fullCutString = Form("%s", eventAndphotonCutNo.Data());
        TString inputFileName = Form("%s/%s/%s", inputPath.Data(), productions.Data(), fileNames.Data());
        if(energy.Contains("5TeV")) inputFileName = Form("%s/%s", inputPath.Data(), fileNames.Data());
        cout<< inputFileName.Data()<< endl;
        cout<< fullCutString.Data() << endl;

        TFile inputFile(inputFileName, "READ");
        //	TList *topList        = (TList*)inputFile.Get("GammaConvV1");
        TList *topList        = (TList*)inputFile.Get("GammaConvMaterial");
        TList *cutList        = (TList*)topList->FindObject(Form("Cut Number %s",fullCutString.Data()));
        TList *subList        = (TList*)cutList->FindObject(Form("%s ESD histograms", fullCutString.Data()));

        TH1D *histoTracks     = (TH1D*)subList->FindObject("GoodESDTracksEta08");
        topList->SetOwner(kTRUE);  // enables recursive free of memory
        cutList->SetOwner(kTRUE);
        subList->SetOwner(kTRUE);
        TH1D* histoTracksSave = (TH1D*)histoTracks->Clone();
        histoTracksSave->Sumw2();
        cout<< histoTracksSave->GetEntries()<< endl;
        if(histoTracksSave->GetEntries()>0) histoTracksSave->Scale(1./histoTracksSave->GetEntries());
        //	histoTracksSave->SetTitle(Form("GoodESDTracks %s %s", productions.Data(), centForTitle.Data()));
        histoTracksSave->SetTitle(Form("GoodESDTracks08 %s", productions.Data() ));
        histoTracksSave->SetName(Form("%s_%s", productions.Data(), centralityClass.Data()));
        targetFile.cd();
        histoTracksSave->Write();
        cout << "\t done"<< endl;


	// V0 Mult

        TH1D *histoV0Tracks     = (TH1D*)subList->FindObject("V0 Multiplicity");
        topList->SetOwner(kTRUE);  // enables recursive free of memory
        cutList->SetOwner(kTRUE);
        subList->SetOwner(kTRUE);
        TH1D* histoV0TracksSave = (TH1D*)histoV0Tracks->Clone();
        histoV0TracksSave->Sumw2();
        cout<< histoV0TracksSave->GetEntries()<< endl;
        if(histoV0TracksSave->GetEntries()>0) histoV0TracksSave->Scale(1./histoV0TracksSave->GetEntries());
        //	histoTracksSave->SetTitle(Form("GoodESDTracks %s %s", productions.Data(), centForTitle.Data()));
        histoV0TracksSave->SetTitle(Form("V0Multiplicity %s", productions.Data() ));
        histoV0TracksSave->SetName(Form("V0M_%s_%s", productions.Data(), centralityClass.Data()));
        targetFile.cd();
        histoV0TracksSave->Write();
        cout << "\t done"<< endl;

        // clean up
        inputFile.Close();
        delete topList;   // needs to be deleted because Close() does not free list memory correctly

    }
    cout << "to " <<  targetFileName.Data() << endl;
    targetFile.Close();
}
