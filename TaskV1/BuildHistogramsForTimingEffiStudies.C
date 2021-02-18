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
#include "TASImage.h"
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



Bool_t debug = kFALSE;
typedef TVectorT<double> TVectorD;
typedef TVectorT<float> TVectorF;

using namespace std;

void SetLogBinningTH3(TH3* histoRebin){
    TAxis *axisafter    = histoRebin->GetZaxis();
    Int_t bins          = axisafter->GetNbins();
    Double_t from       = axisafter->GetXmin();
    Double_t to         = axisafter->GetXmax();
    Double_t *newbins   = new Double_t[bins+1];
    newbins[0]          = from;
    Double_t factor     = TMath::Power(to/from, 1./bins);
    for(Int_t i=1; i<=bins; ++i) newbins[i] = factor * newbins[i-1];
    axisafter->Set(bins, newbins);
    delete [] newbins;
}

void SetLogBinningTH2(TH2* histoRebin){
    TAxis *axisafter    = histoRebin->GetYaxis();
    Int_t bins          = axisafter->GetNbins();
    Double_t from       = axisafter->GetXmin();
    Double_t to         = axisafter->GetXmax();
    Double_t *newbins   = new Double_t[bins+1];
    newbins[0]          = from;
    Double_t factor     = TMath::Power(to/from, 1./bins);
    for(Int_t i=1; i<=bins; ++i) newbins[i] = factor * newbins[i-1];
    axisafter->Set(bins, newbins);
    delete [] newbins;
}

void SetLogBinningXTH2(TH2* histoRebin){
    TAxis *axisafter    = histoRebin->GetXaxis();
    Int_t bins          = axisafter->GetNbins();
    Double_t from       = axisafter->GetXmin();
    Double_t to         = axisafter->GetXmax();
    Double_t *newbins   = new Double_t[bins+1];
    newbins[0]          = from;
    Double_t factor     = TMath::Power(to/from, 1./bins);
    for(Int_t i=1; i<=bins; ++i) newbins[i] = factor * newbins[i-1];
    axisafter->Set(bins, newbins);
    delete [] newbins;
}

void SetLogBinningTH1(TH1* histoRebin){
    TAxis *axisafter    = histoRebin->GetXaxis();
    Int_t bins          = axisafter->GetNbins();
    Double_t from       = axisafter->GetXmin();
    Double_t to         = axisafter->GetXmax();
    Double_t *newbins   = new Double_t[bins+1];
    newbins[0]          = from;
    Double_t factor     = TMath::Power(to/from, 1./bins);
    for(Int_t i=1; i<=bins; ++i) newbins[i] = factor * newbins[i-1];
    axisafter->Set(bins, newbins);
    delete [] newbins;
}

Float_t FunctionM02(Float_t E, Float_t a, Float_t b, Float_t c, Float_t d, Float_t e){
  return ( exp( a+ b*E ) + c + d*E + e/E);
}


void BuildHistogramsForTimingEffiStudies(
    TString fileName                = "LHC18b8_fastandwoSDD_AnalysisResults_Calo.root",
    TString fCutSelection           = "000a2113_4117900010022700000",
    TString fOptionEnergy           = "XeXe_5.44TeV",
    TString nameOutputBase          = "TimingEffiTree",
    Int_t mode                      = 4,
    Long64_t maxNumMesonProcess     = -1,
    Double_t minETag                = 2.0,
    Double_t maxETag                = 5.5,
    Double_t minInvMass             = 0.1,
    Double_t maxInvMass             = 0.15,
    Int_t LimitETagTiming           = 0,
    Int_t LimitETagArrayIndex       = 0,
    Int_t LimitEProbeTiming         = 0,
    Int_t LimitEProbeArrayIndex     = 0,
    TString optionCorrFrameworkDir  = "",
    TString suffix                  = "eps",
    Int_t doQuickOutput             = 0
){


    //********************************************************************************
    //*            Definition of Cuts                                                *
    //********************************************************************************

    gROOT->Reset();

    // Set common default plot style
    StyleSettingsThesis();
    SetPlotStyle();

    //********************************************************************************
    //*            File definition/ loading                                          *
    //********************************************************************************

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



    // Set collisions system
    TString collisionSystem     = ReturnFullCollisionsSystem(fOptionEnergy);
    if (collisionSystem.CompareTo("") == 0){
        cout << "No correct collision system specification, has been given" << endl;
        return;
    }
    TString detectionProcess    = ReturnFullTextReconstructionProcess(mode);

    TString centralityString    = GetCentralityString(fCutSelection);
    if (centralityString.CompareTo("pp")==0){
        centralityString    = "";
    } else {
        if ( !centralityString.Contains("0-100%") )
            collisionSystem = Form("%s %s", centralityString.Data(), collisionSystem.Data());
    }


    //Declaration of leaves types
    TTree *TimingEffiTree = (TTree*)f->Get(Form("%s ClusterTimingEff",fCutSelection.Data()));
    if(!TimingEffiTree){ cout << "tree not found... returning!"<< endl; return;}

    const Int_t iNumberOfTimingCutsArray=6;
    // const Int_t iNumberOfTimingCutsArray=13;
    Int_t iNumberOfTimingCuts=iNumberOfTimingCutsArray;
    Int_t iNumberOfTimingCutsStart=0;
    if ((LimitEProbeTiming>=1)&&(iNumberOfTimingCutsStart<LimitEProbeArrayIndex)){
        iNumberOfTimingCutsStart=LimitEProbeArrayIndex;
    }
    if (iNumberOfTimingCuts>iNumberOfTimingCutsArray){
        iNumberOfTimingCuts=iNumberOfTimingCutsArray;
    }
    Int_t iNumberOfTimingCutsStartM1=iNumberOfTimingCutsStart-1;
    Int_t iNumberOfTimingCutsM1=iNumberOfTimingCuts-1;
    //                                                     0==10000         1==9000        2==4000        3=2000         4==1000        5==500       6==250       7==150       8==100       9==50      10==30     11==25     12==12.5
    Float_t timingCuts[iNumberOfTimingCutsArray][2]    = { {-10000, 10000}, {-100, 100}, {-30, 35}, {-20, 25}, {-12.5,13}, {-6, 7}};
    // Float_t timingCuts[iNumberOfTimingCutsArray][2]    = { {-10000, 10000}, {-9000, 9000}, {-4000, 4000}, {-2000, 2000}, {-1000, 1000}, {-500, 500}, {-250, 250}, {-150, 150}, {-100, 100}, {-50, 50}, {-30, 30}, {-25, 25}, {-12.5,12.5}};
    Float_t energyCuts[6][2]    = { {0.2, 0.5}, {0.5, 1}, {1, 2}, {2, 5}, {5, -1}, {(Float_t) minETag, -1}};
    Color_t colorTimeCut[13]     = { kBlack, kBlue+2, kViolet+2, 807, kGreen+2, kCyan+2, kBlue-6, kRed-6, 800, kMagenta, kRed-7, kMagenta-8, kGreen-5 };
    Style_t styleMarker[13]      = { 24, 25, 42, 44, 29, 33, 20, 21, 27, 37, 35, 48, 28};

    TString outputDir           = Form("%s/%s", fCutSelection.Data(), suffix.Data());
    gSystem->Exec("mkdir -p "+outputDir);
    for (Int_t k=iNumberOfTimingCutsStart; k<=iNumberOfTimingCutsM1;k++){
    TString outputDiradv        = Form("%s/%dns_%dns",outputDir.Data(), (Int_t)(timingCuts[k][0]*10), (Int_t)(timingCuts[k][1]*10));
    gSystem->Exec("mkdir -p "+outputDiradv);
    }
    // some variables have a seperate input variable to allow conversion of datatype
    Float_t         fInvMassMeson;                      //!<! array buffer
    Float_t         fPtMeson;                           //!<! array buffer
    Float_t         fClusterTimeTag;                    //!<! array buffer
    Float_t         fClusterTimeProbe;                  //!<! array buffer
    Float_t         fClusterETag;                       //!<! array buffer
    Float_t         fClusterEProbe;                     //!<! array buffer


    // Set branch adresses
    TimingEffiTree->SetBranchAddress("InvMass",                &fInvMassMeson);
    TimingEffiTree->SetBranchAddress("Pt",                     &fPtMeson);
    TimingEffiTree->SetBranchAddress("ClusterTimeTag",         &fClusterTimeTag);
    TimingEffiTree->SetBranchAddress("ClusterTimeProbe",       &fClusterTimeProbe);
    TimingEffiTree->SetBranchAddress("ClusterETag",            &fClusterETag);
    TimingEffiTree->SetBranchAddress("ClusterEProbe",          &fClusterEProbe);


    //********************************************************************************
    //*            Definition of Boundaries for Histograms                           *
    //********************************************************************************

    //Pt-plots
    Int_t nBinsE                    = 500;
    Int_t nBinsELog                 = 200;
    Double_t firstBinE              = 0.0;
    Double_t lastBinE               = 50.;
    Double_t firstBinELog           = 0.2;

    const Int_t nBinsInvMass        = 50;
    Double_t firstBinInvMass        = minInvMass;
    Double_t lastBinInvMass         = maxInvMass;

    // const Int_t nBinsEInvMass       = 30;
    // Double_t firstBinEInvMass       = 0.3;
    // Double_t lastBinEInvMass        = 50.;
    const Int_t nBinsEInvMass       = 72; //58; //51;
    // const Int_t nBinsEInvMass       = 12;
    // Double_t aBinsEInvMass[nBinsEInvMass+1] = {  // size: 13
    // 1.40,  1.60,  1.80,  2.00,  2.20,  2.40,  2.60,  3.00,  3.50,  4.00,
    // 5.00,  7.00,  10.0};
    Double_t aBinsEInvMass[74] = {  // size: 13
        0.7, 0.8, 0.9, 1., 1.1, 1.2, 1.3,
        1.40,  1.50,  1.60,                      1.70,  1.80,  1.90,  2.00,  2.10,
        2.20,  2.30,  2.40,  2.50,  2.60,        2.70,  2.80,  2.90,  3.00,  3.10,
        3.20,  3.30,  3.40,  3.50,  3.60,        3.70,  3.80,  3.90,  4.00,  4.20,
        4.40,  4.60,  4.80,  5.00,  5.20,        5.40,  5.60,  5.80,  6.00,  6.20,
        6.40,  6.60,  6.80,  7.00,  7.20,        7.40,  7.60,  7.80,  8.00,  8.40,
        8.80,  9.20,  9.60,  10.0,  10.5,        11.0,  11.5,  12.0,  12.5,  13.0,
        13.5,  14.0,  15.0,  16.0,  17.0,  	     18.0,  19.0,  20.0,  21.0};
    // if (mode == 4){
    //     firstBinEInvMass       = 1.4;
    //     lastBinEInvMass        = 20.;
    // }
    Double_t ActualBinEInvMass;
    Double_t ActualBinEInvMassUp;
    Double_t ActualBinEInvMassLow;
    Int_t nBinsTime                 = 23000;
    Double_t firstBinTime           = -1500;
    Double_t lastBinTime            = 10000;
    if (mode == 4){
        nBinsTime                   = 14000;
        firstBinTime                = -1000;
        lastBinTime                 = 6000;
        firstBinELog                = 0.6;
    }
    if (mode == 5){
        if (fOptionEnergy.Contains("13TeV")){
        nBinsTime                   = 36750;
        firstBinTime                = -1500;
        lastBinTime                 = 15000;
        }
    }


    //********************************************************************************
    //*      Definition of histograms for reconstructed Conversion Points            *
    //********************************************************************************
    TH2F* histoMesonInvMassvsPt[iNumberOfTimingCutsArray]          = {NULL};
    TH2F* histoClusterTimevsETag[iNumberOfTimingCutsArray]         = {NULL};
    TH2F* histoClusterTimevsEProbe[iNumberOfTimingCutsArray]       = {NULL};
    TH2F* histoClusterTimevsEProbeWCut[iNumberOfTimingCutsArray]   = {NULL};
    TH1F* histoClusterTTag[iNumberOfTimingCutsArray]               = {NULL};
    TH1F* histoClusterTProbe[iNumberOfTimingCutsArray]             = {NULL};
    TH1F* histoClusterTProbeWCut[iNumberOfTimingCutsArray]         = {NULL};
    TH1F* histoClusterTProbeECut[iNumberOfTimingCutsArray]         = {NULL};
    TH1F* histoClusterTProbeWCutECut[iNumberOfTimingCutsArray]     = {NULL};
    TH1F* histoClusterETag[iNumberOfTimingCutsArray]               = {NULL};
    TH1F* histoClusterEProbe[iNumberOfTimingCutsArray]             = {NULL};
    TH1F* histoClusterEProbeWCut[iNumberOfTimingCutsArray]         = {NULL};
    TH1F* histoClusterTimingEff[iNumberOfTimingCutsArray]          = {NULL};
    //-SignalExtraction
    TH2F* histoMesonInvMassvsEProbe[iNumberOfTimingCutsArray]      = {NULL};
    TH1F* histoMesonInvMassvsEProbeProjX[iNumberOfTimingCutsArray][nBinsEInvMass];
    TH1F* histoMesonInvMassvsEProbeProjXLinBckSubtracted[iNumberOfTimingCutsArray][nBinsEInvMass];
    TString TStrSignalExtractionFunction;
    TF1* fitMesonInvMassvsEProbeProjX[iNumberOfTimingCutsArray][nBinsEInvMass];
    TF1* fitMesonInvMassvsEProbeProjXLin[iNumberOfTimingCutsArray][nBinsEInvMass];
    Double_t IntegratedYieldInEnergyBin[iNumberOfTimingCutsArray][nBinsEInvMass];
    Double_t IntegratedYieldInEnergyBinError[iNumberOfTimingCutsArray][nBinsEInvMass];
    TH1F* histoClusterEProbeIntegrated[iNumberOfTimingCutsArray]             = {NULL};
    TH1F* histoClusterEProbeWCutIntegrated[iNumberOfTimingCutsArray]         = {NULL};

    //here I strated
    TH1F* histoClusterTime[iNumberOfTimingCutsArray]              = {NULL};
    //and here I stopped
    TString fileNameOutput = Form("%s_Data.root",nameOutputBase.Data()) ;
    TFile* fileTimeEffiWrite = new TFile(fileNameOutput.Data(),"RECREATE");

    for (Int_t k = iNumberOfTimingCutsStart; k<iNumberOfTimingCuts; k++){
        cout << k << "\t" << timingCuts[k][0] << "\t"<< timingCuts[k][1] << endl;
        histoMesonInvMassvsPt[k]        = new TH2F(Form("histoMesonInvMassvsPt_%d", k),"",400, 0, 0.2, nBinsE, firstBinE, lastBinE);
        histoClusterTimevsETag[k]       = new TH2F(Form("histoClusterTimevsETag_%d",k),"",nBinsTime, firstBinTime, lastBinTime, nBinsE, firstBinE, lastBinE);
        histoClusterTimevsEProbe[k]     = new TH2F(Form("histoClusterTimevsEProbe_%d",k),"",nBinsTime, firstBinTime, lastBinTime, nBinsE, firstBinE, lastBinE);
        histoClusterTimevsEProbeWCut[k] = new TH2F(Form("histoClusterTimevsEProbeWCut_%d",k),"",nBinsTime, firstBinTime, lastBinTime, nBinsE, firstBinE, lastBinE);
        histoClusterTTag[k]             = new TH1F(Form("histoClusterTTag_%d", k),"", nBinsTime, firstBinTime, lastBinTime);
        histoClusterTProbe[k]           = new TH1F(Form("histoClusterTProbe_%d", k),"", nBinsTime, firstBinTime, lastBinTime);
        histoClusterTProbeWCut[k]       = new TH1F(Form("histoClusterTProbeWCut_%d", k),"", nBinsTime, firstBinTime, lastBinTime);
        histoClusterTProbeECut[k]       = new TH1F(Form("histoClusterTProbeECut_%d", k),"", nBinsTime, firstBinTime, lastBinTime);
        histoClusterTProbeWCutECut[k]   = new TH1F(Form("histoClusterTProbeWCutECut_%d", k),"", nBinsTime, firstBinTime, lastBinTime);
        histoClusterETag[k]             = new TH1F(Form("histoClusterETag_%d",k),"",nBinsELog, firstBinELog, lastBinE);
        histoClusterEProbe[k]           = new TH1F(Form("histoClusterEProbe%d",k),"",nBinsELog, firstBinELog, lastBinE); //Efficiency ==1
        histoClusterEProbeWCut[k]       = new TH1F(Form("histoClusterEProbeWCut%d",k),"",nBinsELog, firstBinELog, lastBinE); //Efficiency when Divided by histoClusterEProbe
        //-SignalExtraction
        histoMesonInvMassvsEProbe[k]    = new TH2F(Form("histoMesonInvMassvsEProbe_%d", k),"",nBinsInvMass, firstBinInvMass, lastBinInvMass, nBinsEInvMass, aBinsEInvMass);
        histoMesonInvMassvsEProbe[k]->Sumw2();
        histoClusterEProbeIntegrated[k] = new TH1F(Form("histoClusterEProbeIntegrated%d",k),"",nBinsEInvMass, aBinsEInvMass); //Integrated Efficiency ==1
        histoClusterEProbeIntegrated[k]->Sumw2();
        histoClusterEProbeWCutIntegrated[k] = new TH1F(Form("histoClusterEProbeWCutIntegrated%d",k),"",nBinsEInvMass, aBinsEInvMass); //Efficiency when Devided by histoClusterEProbeIntegrated
        histoClusterEProbeWCutIntegrated[k]->Sumw2();
        SetLogBinningTH1(histoClusterETag[k]);
        SetLogBinningTH1(histoClusterEProbe[k]);
        SetLogBinningTH1(histoClusterEProbeWCut[k]);
        SetLogBinningTH2(histoMesonInvMassvsEProbe[k]);
        SetLogBinningTH1(histoClusterEProbeIntegrated[k]);
        SetLogBinningTH1(histoClusterEProbeWCutIntegrated[k]);
    }

    //********************************************************************************
    //*      Reading of Tree with reconstructed gammas/ filling of histograms        *
    //********************************************************************************

    Long64_t nEntriesRecMeson                 = TimingEffiTree->GetEntries();
    if(maxNumMesonProcess>0 && maxNumMesonProcess<nEntriesRecMeson)
        nEntriesRecMeson = maxNumMesonProcess;
    cout << "Number of mesons to be processed: " << nEntriesRecMeson << endl;

    for (Long64_t i=0; i<nEntriesRecMeson;i++) {
        TimingEffiTree->GetEntry(i);
        if (debug) cout << "Entry: " << i << "\t" << fClusterETag << "\t" << fClusterTimeTag << "\t" << fClusterTimeProbe<< endl;

        if ( fClusterETag < minETag ){ if (debug) cout << "failed minE" << endl; continue;}
        if ( fClusterETag > maxETag ){ if (debug) cout << "failed maxE" << endl; continue;}
        if (LimitETagTiming>=1){
            if (LimitETagTiming>=10){
                if (    (fClusterTimeTag < timingCuts[LimitETagArrayIndex][1]*1e-9)&&(fClusterTimeTag > timingCuts[LimitETagArrayIndex][0]*1e-9)    ){
                    if (debug) cout << "failed fClusterTimeTag" << endl;
                    continue;
                }
            } else {
                if (    (fClusterTimeTag < timingCuts[LimitETagArrayIndex][0]*1e-9)||(fClusterTimeTag > timingCuts[LimitETagArrayIndex][1]*1e-9)    ){
                    if (debug) cout << "failed fClusterTimeTag" << endl;
                    continue;
                }
            }
        }
        if (LimitEProbeTiming>=1){
            if (    (fClusterTimeProbe < timingCuts[LimitEProbeArrayIndex][0]*1e-9)||(fClusterTimeProbe > timingCuts[LimitEProbeArrayIndex][1]*1e-9)    ){
                if (debug) cout << "failed fClusterTimeProbe" << endl;
                continue;
            }
        }
        for (Int_t k = iNumberOfTimingCutsStart; k < iNumberOfTimingCuts; k++){
            //if (! (fClusterTimeTag > timingCuts[k][1]*1e-9 || fClusterTimeTag < timingCuts[k][0]*1e-9 )  && (fInvMassMeson < maxInvMass && fInvMassMeson > minInvMass)) { <============= THIS LINE!!
            if ((fInvMassMeson < maxInvMass && fInvMassMeson > minInvMass)) {
                //histoMesonInvMassvsPt[k]->Fill(fInvMassMeson,fPtMeson);
                histoClusterTimevsETag[k]->Fill(fClusterTimeTag*1e9,fClusterETag);
                histoClusterTimevsEProbe[k]->Fill(fClusterTimeProbe*1e9,fClusterEProbe);
                histoClusterETag[k]->Fill(fClusterETag);
                histoClusterEProbe[k]->Fill(fClusterEProbe);
                histoClusterTTag[k]->Fill(fClusterTimeTag*1e9);
                histoClusterTProbe[k]->Fill(fClusterTimeProbe*1e9);
                if (fClusterEProbe > minETag){ histoClusterTProbeECut[k]->Fill(fClusterTimeProbe*1e9);}
                if (! (fClusterTimeProbe > timingCuts[k][1]*1e-9 || fClusterTimeProbe < timingCuts[k][0]*1e-9 ) ) {
                    histoClusterEProbeWCut[k]->Fill(fClusterEProbe);
                    histoClusterTProbeWCut[k]->Fill(fClusterTimeProbe*1e9);
                    histoMesonInvMassvsPt[k]->Fill(fInvMassMeson,fPtMeson);
                    histoMesonInvMassvsEProbe[k]->Fill(fInvMassMeson,fClusterEProbe);
                    if (fClusterEProbe > minETag) histoClusterTProbeWCutECut[k]->Fill(fClusterTimeProbe*1e9);
                }
            }
            if  (! (fClusterTimeProbe > timingCuts[k][1]*1e-9 || fClusterTimeProbe < timingCuts[k][0]*1e-9 )  && (fInvMassMeson < maxInvMass && fInvMassMeson > minInvMass)){
                    histoClusterTimevsEProbeWCut[k]->Fill(fClusterTimeProbe*1e9,fClusterEProbe);
            }
        }
    }
    TCanvas* CanvasSignalExtraction = new TCanvas("CanvasSignalExtraction","",200,10,1350,900);
    TStrSignalExtractionFunction=Form("(x<[1])*([0]*(TMath::Exp(-0.5*((x-[1])/[2])^2)+TMath::Exp((x-[1])/[3])*(1.-TMath::Exp(-0.5*((x-[1])/[2])^2)))+[4]+[5]*x)+(x>=[1])*([0]*TMath::Exp(-0.5*((x-[1])/[2])^2)+[4]+[5]*x)");
    // TStrSignalExtractionFunction=Form("[0]+([1]*x)+([2]*TMath::Exp(-0.5*((x-[3])/[4])^2))");
    for (Int_t k = iNumberOfTimingCutsStart; k < iNumberOfTimingCuts; k++){
        cout<<endl<<"----------------------------------------------"<< endl << "TestOutput; k: "<<k<<"("<<timingCuts[k][1]<<" ns)"<<endl;
        histoMesonInvMassvsEProbe[k]->Draw("COLZ");
        gSystem->Exec(Form("rm -rf %s/%f", outputDir.Data(), timingCuts[k][1]));
        gSystem->Exec(Form("mkdir -p %s/%f", outputDir.Data(), timingCuts[k][1]));
        CanvasSignalExtraction->SaveAs(Form("%s/InvMass_ProbTim%f.%s",outputDir.Data(), timingCuts[k][1], suffix.Data()));
        for (Int_t nBinsEInvMass_Loop=0; nBinsEInvMass_Loop<nBinsEInvMass; nBinsEInvMass_Loop++){
            cout<< endl<<"Bin:" << nBinsEInvMass_Loop <<"  "<< ActualBinEInvMassLow << "    " << ActualBinEInvMassUp<<endl;
            fitMesonInvMassvsEProbeProjX[k][nBinsEInvMass_Loop]=new TF1(Form("fitMesonInvMassvsEProbeProjX_%d_%d", k, nBinsEInvMass_Loop),Form("%s",TStrSignalExtractionFunction.Data()),0.07,0.27);
            fitMesonInvMassvsEProbeProjX[k][nBinsEInvMass_Loop]->SetParameter(5,1.);
            fitMesonInvMassvsEProbeProjX[k][nBinsEInvMass_Loop]->SetParameter(4,1.);
            fitMesonInvMassvsEProbeProjX[k][nBinsEInvMass_Loop]->SetParameter(0,1000);
            fitMesonInvMassvsEProbeProjX[k][nBinsEInvMass_Loop]->SetParameter(3,0.01);
            fitMesonInvMassvsEProbeProjX[k][nBinsEInvMass_Loop]->SetParameter(1,0.135);
            fitMesonInvMassvsEProbeProjX[k][nBinsEInvMass_Loop]->SetParameter(2,0.01);
            // fitMesonInvMassvsEProbeProjX[k][nBinsEInvMass_Loop]->SetParameter(0,1.);
            // fitMesonInvMassvsEProbeProjX[k][nBinsEInvMass_Loop]->SetParameter(1,1.);
            // fitMesonInvMassvsEProbeProjX[k][nBinsEInvMass_Loop]->SetParameter(2,1.);
            // fitMesonInvMassvsEProbeProjX[k][nBinsEInvMass_Loop]->SetParameter(3,0.135);
            // fitMesonInvMassvsEProbeProjX[k][nBinsEInvMass_Loop]->SetParameter(4,0.004);
            if (mode == 4){
                // fitMesonInvMassvsEProbeProjX[k][nBinsEInvMass_Loop]->FixParameter(0,0.);
                // fitMesonInvMassvsEProbeProjX[k][nBinsEInvMass_Loop]->FixParameter(1,0.);
                fitMesonInvMassvsEProbeProjX[k][nBinsEInvMass_Loop]->SetParLimits(2,0.,0.05);
                fitMesonInvMassvsEProbeProjX[k][nBinsEInvMass_Loop]->SetParLimits(3,0.,0.05);
                fitMesonInvMassvsEProbeProjX[k][nBinsEInvMass_Loop]->SetParLimits(1,0.12,0.15);
            } else {
                fitMesonInvMassvsEProbeProjX[k][nBinsEInvMass_Loop]->SetParLimits(3,0.13,0.14);
                fitMesonInvMassvsEProbeProjX[k][nBinsEInvMass_Loop]->SetParLimits(4,0.001,0.006);
            }
            //cout<<"Line: "<<__LINE__<<"; nBinsEInvMass_Loop: "<<nBinsEInvMass_Loop<<endl;
            ActualBinEInvMassLow=histoMesonInvMassvsEProbe[k]->GetYaxis()->GetBinLowEdge(nBinsEInvMass_Loop+1);//+1 underflow Bin
            ActualBinEInvMassUp=histoMesonInvMassvsEProbe[k]->GetYaxis()->GetBinUpEdge(nBinsEInvMass_Loop+1);//+1 underflow Bin
            ActualBinEInvMass=histoMesonInvMassvsEProbe[k]->GetYaxis()->GetBinCenter(nBinsEInvMass_Loop+1);//+1 underflow Bin
            histoMesonInvMassvsEProbeProjX[k][nBinsEInvMass_Loop]=(TH1F*)histoMesonInvMassvsEProbe[k]->ProjectionX(Form("histoMesonInvMassvsEProbe_k%d_ProjX%d", k, nBinsEInvMass_Loop), nBinsEInvMass_Loop+1, nBinsEInvMass_Loop+1);//+1 underflow Bin
            histoMesonInvMassvsEProbeProjX[k][nBinsEInvMass_Loop]->Sumw2();
            histoMesonInvMassvsEProbeProjXLinBckSubtracted[k][nBinsEInvMass_Loop]=(TH1F*) histoMesonInvMassvsEProbeProjX[k][nBinsEInvMass_Loop]->Clone(Form("histoMesonInvMassvsEProbeLinSubtracted_k%d_ProjX%d", k, nBinsEInvMass_Loop));
            histoMesonInvMassvsEProbeProjXLinBckSubtracted[k][nBinsEInvMass_Loop]->Sumw2();
            histoMesonInvMassvsEProbeProjX[k][nBinsEInvMass_Loop]->Fit(fitMesonInvMassvsEProbeProjX[k][nBinsEInvMass_Loop],"RME0","", 0.07,0.27);
            fitMesonInvMassvsEProbeProjXLin[k][nBinsEInvMass_Loop]=new TF1(Form("fitMesonInvMassvsEProbeProjXLin_%d_%d", k, nBinsEInvMass_Loop),Form("pol1"));
            fitMesonInvMassvsEProbeProjXLin[k][nBinsEInvMass_Loop]->FixParameter(0, fitMesonInvMassvsEProbeProjX[k][nBinsEInvMass_Loop]->GetParameter(4));
            fitMesonInvMassvsEProbeProjXLin[k][nBinsEInvMass_Loop]->FixParameter(1, fitMesonInvMassvsEProbeProjX[k][nBinsEInvMass_Loop]->GetParameter(5));
            DrawGammaSetMarker(histoMesonInvMassvsEProbeProjX[k][nBinsEInvMass_Loop], 24, 1.5, colorTimeCut[0], colorTimeCut[0]);
            histoMesonInvMassvsEProbeProjX[k][nBinsEInvMass_Loop]->Draw();
            fitMesonInvMassvsEProbeProjX[k][nBinsEInvMass_Loop]->Draw("same");
            fitMesonInvMassvsEProbeProjXLin[k][nBinsEInvMass_Loop]->Draw("same");
            histoMesonInvMassvsEProbeProjXLinBckSubtracted[k][nBinsEInvMass_Loop]->Add(fitMesonInvMassvsEProbeProjXLin[k][nBinsEInvMass_Loop], -1);
            DrawGammaSetMarker(histoMesonInvMassvsEProbeProjXLinBckSubtracted[k][nBinsEInvMass_Loop], 24, 1.5, colorTimeCut[1], colorTimeCut[1]);
            histoMesonInvMassvsEProbeProjXLinBckSubtracted[k][nBinsEInvMass_Loop]->Draw("same");
            CanvasSignalExtraction->SaveAs(Form("%s/%f/InvMass_ProbTim%f_ProjX%fGeV-%fGeV.%s",outputDir.Data(), timingCuts[k][1], timingCuts[k][1], ActualBinEInvMassLow, ActualBinEInvMassUp, suffix.Data()));
            // CanvasSignalExtraction->SaveAs(Form("%s/%f/InvMass_ProbTim%f_ProjX%fGeV-%fGeV_LinSubtracted.%s",outputDir.Data(), timingCuts[k][1], timingCuts[k][1], ActualBinEInvMassLow, ActualBinEInvMassUp, suffix.Data()));
            IntegratedYieldInEnergyBin[k][nBinsEInvMass_Loop]=histoMesonInvMassvsEProbeProjXLinBckSubtracted[k][nBinsEInvMass_Loop]->IntegralAndError(histoMesonInvMassvsEProbeProjXLinBckSubtracted[k][nBinsEInvMass_Loop]->FindBin(0.12), histoMesonInvMassvsEProbeProjXLinBckSubtracted[k][nBinsEInvMass_Loop]->FindBin(0.15), IntegratedYieldInEnergyBinError[k][nBinsEInvMass_Loop], "weighted");
            //histoClusterEProbeIntegrated[k]->Fill(ActualBinEInvMass,IntegratedYieldInEnergyBin[k][nBinsEInvMass_Loop]);
            if (ActualBinEInvMass == histoClusterEProbeIntegrated[k]->GetBinCenter(nBinsEInvMass_Loop+1)){
                histoClusterEProbeIntegrated[k]->SetBinContent (nBinsEInvMass_Loop+1,IntegratedYieldInEnergyBin[k][nBinsEInvMass_Loop]);//+1 Overflow Bin
                histoClusterEProbeIntegrated[k]->SetBinError (nBinsEInvMass_Loop+1,IntegratedYieldInEnergyBinError[k][nBinsEInvMass_Loop]);//+1 Overflow Bin
            } else {
                cout<<"Error in timing Cut"<<timingCuts[k][1]<<": ActualBinEInvMass ("<<ActualBinEInvMass<<" GeV) is not Center if histoClusterEProbeIntegrated for Bin nBinsEInvMass_Loop+1 ("<<histoClusterEProbeIntegrated[k]->GetBinCenter(nBinsEInvMass_Loop+1)<<" GeV)";
                cout<<"; Try Filling nBinsEInvMass_Loop"<<endl;
                if (ActualBinEInvMass == histoClusterEProbeIntegrated[k]->GetBinCenter(nBinsEInvMass_Loop)){
                    histoClusterEProbeIntegrated[k]->SetBinContent (nBinsEInvMass_Loop,IntegratedYieldInEnergyBin[k][nBinsEInvMass_Loop]);
                    histoClusterEProbeIntegrated[k]->SetBinError (nBinsEInvMass_Loop,IntegratedYieldInEnergyBinError[k][nBinsEInvMass_Loop]);
                } else {
                    cout<<"Error in timing Cut"<<timingCuts[k][1]<<": ActualBinEInvMass ("<<ActualBinEInvMass<<" GeV) is not Center if histoClusterEProbeIntegrated for Bin nBinsEInvMass_Loop ("<<histoClusterEProbeIntegrated[k]->GetBinCenter(nBinsEInvMass_Loop)<<" GeV)";
                    cout<<"; Returning!"<<endl;
                    return;
                }
            }
            //histoClusterEProbeIntegrated[k]->SetBinError( nBinsEInvMass_Loop+1,(TMath::Sqrt(IntegratedYieldInEnergyBin[k][nBinsEInvMass_Loop])));
        }
        histoClusterEProbeIntegrated[k]->Sumw2();
        histoClusterEProbeIntegrated[k]->Draw();
        CanvasSignalExtraction->SaveAs(Form("%s/histoClusterEProbeIntegrated_%f.%s",outputDir.Data(), timingCuts[k][1], suffix.Data()));
    }

    // //Draw Comparison
    // for (Int_t k = iNumberOfTimingCutsStart; k < iNumberOfTimingCuts; k++){
    //     //cout<<"TestOutput; k: "<<k<<"("<<timingCuts[k][1]<<" ns)"<<endl;
    //     gSystem->Exec(Form("rm -rf %s/%f_SignalExtractionComparison", outputDir.Data(), timingCuts[k][1]));
    //     gSystem->Exec(Form("mkdir -p %s/%f_SignalExtractionComparison", outputDir.Data(), timingCuts[k][1]));
    //     for (Int_t nBinsEInvMass_Loop=0; nBinsEInvMass_Loop<nBinsEInvMass; nBinsEInvMass_Loop++){
    //         //cout<<"TestOutput; nBinsEInvMass_Loop: "<<nBinsEInvMass_Loop<<endl;
    //         //Which PtBin is used
    //         ActualBinEInvMassLow=histoMesonInvMassvsEProbe[k]->GetYaxis()->GetBinLowEdge(nBinsEInvMass_Loop+1);//+1 underflow Bin
    //         ActualBinEInvMassUp=histoMesonInvMassvsEProbe[k]->GetYaxis()->GetBinUpEdge(nBinsEInvMass_Loop+1);//+1 underflow Bin
    //         ActualBinEInvMass=histoMesonInvMassvsEProbe[k]->GetYaxis()->GetBinCenter(nBinsEInvMass_Loop+1);//+1 underflow Bin
    //         //cout<<"TestOutput; "<<"k: "<<k<<"("<<timingCuts[k][1]<<"); ActualBinEInvMass: "<<ActualBinEInvMass<<endl;
    //         //SetLineColor and Draw
    //         //histoMesonInvMassvsEProbeProjX[iNumberOfTimingCutsStart][nBinsEInvMass_Loop]->SetLineColor(kBlack);
    //         //histoMesonInvMassvsEProbeProjX[iNumberOfTimingCutsStart][nBinsEInvMass_Loop]->Draw();
    //         histoMesonInvMassvsEProbeProjXLinBckSubtracted[iNumberOfTimingCutsStart][nBinsEInvMass_Loop]->SetLineColor(kRed);
    //         histoMesonInvMassvsEProbeProjXLinBckSubtracted[iNumberOfTimingCutsStart][nBinsEInvMass_Loop]->Draw();
    //         histoMesonInvMassvsEProbeProjX[k][nBinsEInvMass_Loop]->SetLineColor(kBlue);
    //         histoMesonInvMassvsEProbeProjX[k][nBinsEInvMass_Loop]->Draw("same");
    //         histoMesonInvMassvsEProbeProjXLinBckSubtracted[k][nBinsEInvMass_Loop]->SetLineColor(kGreen);
    //         histoMesonInvMassvsEProbeProjXLinBckSubtracted[k][nBinsEInvMass_Loop]->Draw("same");
    //         //cout<<"TestOutput; "<<"Test->SaveAs;"<<endl;
    //         CanvasSignalExtraction->SaveAs(Form("%s/%f_SignalExtractionComparison/InvMass_ProbTim%f_ProjX%fGeV-%fGeV_LinSubtracted_Comparison.%s",outputDir.Data(), timingCuts[k][1], timingCuts[k][1], ActualBinEInvMassLow, ActualBinEInvMassUp, suffix.Data()));
    //     }
    //
    // }
    //delete
    for (Int_t k = iNumberOfTimingCutsStart; k < iNumberOfTimingCuts; k++){
        for (Int_t nBinsEInvMass_Loop=0; nBinsEInvMass_Loop<nBinsEInvMass; nBinsEInvMass_Loop++){
            //cout<<"TestOutput Delete; "<<"k: "<<k<<"("<<timingCuts[k][1]<<"); ActualBinEInvMass: "<<ActualBinEInvMass<<endl;
            delete histoMesonInvMassvsEProbeProjX[k][nBinsEInvMass_Loop];
            delete histoMesonInvMassvsEProbeProjXLinBckSubtracted[k][nBinsEInvMass_Loop];
            delete fitMesonInvMassvsEProbeProjX[k][nBinsEInvMass_Loop];
            delete fitMesonInvMassvsEProbeProjXLin[k][nBinsEInvMass_Loop];
        }
    }
    delete CanvasSignalExtraction;

    for (Int_t k = iNumberOfTimingCutsStart; k<iNumberOfTimingCuts; k++){
        histoClusterETag[k]->Sumw2();
        histoClusterEProbe[k]->Sumw2();
        histoClusterEProbeWCut[k]->Sumw2();
        histoClusterTTag[k]->Sumw2();
        histoClusterTProbe[k]->Sumw2();
        histoClusterTProbeWCut[k]->Sumw2();
        histoClusterTProbeECut[k]->Sumw2();
        histoClusterTProbeWCutECut[k]->Sumw2();
        /*histoClusterTimingEff[k]    = (TH1F*)histoClusterEProbeWCut[k]->Clone(Form("histoClusterTimingEff_%d", k));
        histoClusterTimingEff[k]->Divide(histoClusterTimingEff[k],histoClusterEProbe[k],1.,1.,"B"); //<========= Efficiency Histogram*/
        histoClusterTimingEff[k]    = (TH1F*)histoClusterEProbeIntegrated[k]->Clone(Form("histoClusterTimingEff_%d", k));
        histoClusterTimingEff[k]->Sumw2();
        histoClusterTimingEff[k]->Divide(histoClusterEProbeIntegrated[k],histoClusterEProbeIntegrated[iNumberOfTimingCutsStart],1.,1.,"B"); //<========= Efficiency Histogram
        // histoClusterTimingEff[k]->Divide(histoClusterEProbeIntegrated[iNumberOfTimingCutsStart]); //<========= Efficiency Histogram
    }

        TCanvas* canvasCompEffSimple = new TCanvas("canvasCompEffSimple","",200,10,1350,900);// gives the page size
        DrawGammaCanvasSettings( canvasCompEffSimple, 0.07, 0.01, 0.01, 0.09);
        canvasCompEffSimple->SetLogx();
        //TH1F* histo1DDummy       = new TH1F("histo1DDummy","histo1DDummy",1000, firstBinELog, 50);
        TH1F* histo1DDummy       = new TH1F("histo1DDummy","histo1DDummy",nBinsEInvMass, aBinsEInvMass);
        SetStyleHistoTH1ForGraphs(histo1DDummy, "#it{E} (GeV)", "#it{#varepsilon}_{t}", 0.035 ,0.04, 0.035,0.04, 0.9, 0.85, 510, 505);
        if (mode == 5)
            histo1DDummy->GetYaxis()->SetRangeUser(0,1.2);
        else if (mode == 4)
            histo1DDummy->GetYaxis()->SetRangeUser(0.931,1.019);
            // histo1DDummy->GetYaxis()->SetRangeUser(0.5,1.1);
            // histo1DDummy->GetYaxis()->SetRangeUser(0.95,1.015);
        histo1DDummy->GetXaxis()->SetLabelOffset(-0.005);
        histo1DDummy->GetXaxis()->SetMoreLogLabels();
        histo1DDummy->DrawCopy();
        Int_t nRows = 5;
        if (mode ==4)
            nRows   = 4;

        TFile* outputFile = new TFile(Form("%s/TimingEfficiency_%s_%i.root",outputDir.Data(),fOptionEnergy.Data(),mode), "RECREATE");

        Double_t FitMinNormal=0.8;
        Double_t FitMaxNormal=7.0; //was 24 before
        Double_t FitMinHighPt=10.; //was 0.3 before
        Double_t FitMaxNHighPt=24;
        Double_t paramsTimingEffiFit[7]= {1.00437, 1.0, 0.0797873, 1.68322, 0.0806098, 244.586, 116.938};
        //Double_t paramsTimingEffiFitHighPt[8]= {1.00437, 1.0, 0.0797873, 1.68322, 0.0806098, 244.586, 116.938, 5.};
        Double_t paramsTimingEffiFitHighPt[4]= {1.00437, 1.0, 5.};
        TString TStrTimingEffiFit;
        //TStrTimingEffiFit=Form("1 / ( [0] / ( [1] *( 1. / ( 1. + [2] * exp( -x / [3] ) ) * 1. / ( 1. + [4] * exp( ( x - [5] ) / [6] ) ) ) ) )");
        TStrTimingEffiFit=Form("exp(([0]+[1]*x-[2]*x*x+x*x*x)/(1.-[3]*x+[4]*x*x+x*x*x))");
        TF1* funcTimingEffiFit   = new TF1("funcTimingEffiFit",   Form("%s", TStrTimingEffiFit.Data()),   FitMinNormal, FitMaxNormal);
        TString TStrTimingEffiFitHighPt;
        TStrTimingEffiFitHighPt=Form("1 / ( [0] / ( [1] *( 1. / ( 1. + [2] * exp( -x / [3] ) ) * 1. / ( 1. + [4] * exp( ( x - [5] ) / [6] ) ) ) ) )");
        //TStrTimingEffiFitHighPt=Form("exp(([0]+[1]*x-[2]*x*x+x*x*x)/(1.-[3]*x+[4]*x*x+x*x*x))");
        //TStrTimingEffiFitHighPt=Form("(x>[2])*([0] * exp ( -([1]*(x - [2]))) + ((x<[2])*[0])");
        TF1* funcTimingEffiFitHighPt   = new TF1("funcTimingEffiFit",   Form("%s", TStrTimingEffiFitHighPt.Data()),   FitMinHighPt, FitMaxNHighPt);
        //("FuncTimingEfficiencyMCSimCluster", "1 /([0]/([1]*(1./(1.+[2]*exp(-x/[3]))* 1./(1.+[4]*exp((x-[5])/[6])))))");
        //funcTimingEffiFit->SetParameters(paramsTimingEffiFit);
        //funcTimingEffiFitHighPt->SetParameters(paramsTimingEffiFitHighPt);

        TLegend* legendEffiSimple = GetAndSetLegend2(0.35,0.11,0.95,0.11+1.15*0.032*nRows, 1000*0.032, 2, "", 43, 0.1);
        for (Int_t k = iNumberOfTimingCutsM1; k > iNumberOfTimingCutsStartM1; k-- ){
            // if (mode == 4 && k < 1 ) continue;
            if (mode == 4 && k < 0 ) continue;
            DrawGammaSetMarker(histoClusterTimingEff[k], styleMarker[k], 1.5, colorTimeCut[k], colorTimeCut[k]);
            // histoClusterTimingEff[k]->SetLineWidth(0.);
            histoClusterTimingEff[k]->Draw("p,same");
            // histoClusterTimingEff[k]->Draw("e1p,same");
            cout << "Fitting histoClusterTimingEff " << Form("%4.1f_%4.1f",timingCuts[k][0],timingCuts[k][1]) << endl;
            histoClusterTimingEff[k]->Write(Form("histoClusterTimingEffWithoutFit_%4.1f_%4.1f",timingCuts[k][0],timingCuts[k][1]));
            //low Pt
            cout<<"Fit Low Pt for k == "<<k<<"; timingCuts[k][0]: "<<timingCuts[k][0]<<"; timingCuts[k][1]: "<<timingCuts[k][1]<<endl;
            if (k>=4){
                histoClusterTimingEff[k]->Fit(funcTimingEffiFit,"RME+","", FitMinNormal, FitMaxNormal);
                funcTimingEffiFit->Draw("same");
            }
            cout<<"Fitted Low Pt for k == "<<k<<"; timingCuts[k][0]: "<<timingCuts[k][0]<<"; timingCuts[k][1]: "<<timingCuts[k][1]<<endl;
            histoClusterTimingEff[k]->Write(Form("histoClusterTimingEff_%4.1f_%4.1f",timingCuts[k][0],timingCuts[k][1]));
            funcTimingEffiFit->Write(Form("FitClusterTimingEff_%4.1f_%4.1f",timingCuts[k][0],timingCuts[k][1]));
            //high Pt
            funcTimingEffiFitHighPt->SetLineColor(colorTimeCut[k]);
            cout<<"Fit High Pt for k == "<<k<<"; timingCuts[k][0]: "<<timingCuts[k][0]<<"; timingCuts[k][1]: "<<timingCuts[k][1]<<endl;
            if (k==8){
                histoClusterTimingEff[k]->Fit(funcTimingEffiFitHighPt,"RME+","", FitMinHighPt, FitMaxNHighPt);
                funcTimingEffiFitHighPt->Draw("same");
            }
            cout<<"Fitted High Pt for k == "<<k<<"; timingCuts[k][0]: "<<timingCuts[k][0]<<"; timingCuts[k][1]: "<<timingCuts[k][1]<<endl;
            histoClusterTimingEff[k]->Write(Form("histoClusterTimingEff_highPt_%4.1f_%4.1f",timingCuts[k][0],timingCuts[k][1]));
            funcTimingEffiFitHighPt->Write(Form("FitClusterTimingEff_highPt_%4.1f_%4.1f",timingCuts[k][0],timingCuts[k][1]));
            histoClusterTimingEff[k]->Draw("e1p,same");
            legendEffiSimple->AddEntry(histoClusterTimingEff[k], Form("%4.1f < #it{t} (ns) < %4.1f", timingCuts[k][0],timingCuts[k][1]), "p" );
        }
        legendEffiSimple->Draw();
        histo1DDummy->Draw("same,axis");

        outputFile->Close();
        delete outputFile;

        TLatex *labelEnergy = new TLatex(0.11,0.93,Form("%s",collisionSystem.Data()));
        SetStyleTLatex( labelEnergy, 0.04,4);
        labelEnergy->Draw();
        TLatex *labelDet = new TLatex(0.11,0.88,Form("%s",detectionProcess.Data()));
        SetStyleTLatex( labelDet, 0.04,4);
        labelDet->Draw();

        canvasCompEffSimple->Update();
        canvasCompEffSimple->SaveAs(Form("%s/TimingEffi.%s",outputDir.Data(),suffix.Data()));
        if (doQuickOutput>=1){
            return;
        }

        TCanvas* canvasCompEff = new TCanvas("canvasCompEff","",200,10,1550,1500);// gives the page size
        DrawGammaCanvasSettings( canvasCompEff, 0.1, 0.08, 0.02, 0.12);
        canvasCompEff->SetLogy();

        TH1F* histo2DDummy;

        histo2DDummy = new TH1F("histo2DDummy","histo2DDummy",nBinsTime, firstBinTime, lastBinTime);
        SetStyleHistoTH1ForGraphs(histo2DDummy, "#it{t} (ns)", "counts", 0.035 ,0.04, 0.035,0.04, 0.9, 0.95, 510, 505);
        ofstream output;
        for (Int_t k = iNumberOfTimingCutsM1; k > iNumberOfTimingCutsStartM1; k-- ){

            fileTimeEffiWrite->mkdir(Form("%4.1fns_%4.1fns",timingCuts[k][0], timingCuts[k][1] ));
            fileTimeEffiWrite->cd(Form("%4.1fns_%4.1fns",timingCuts[k][0], timingCuts[k][1] ));
            output.open(Form("%s/%dns_%dns/TimeCutEnlargedVisulalisation.tex",outputDir.Data(),(Int_t)(timingCuts[k][0]*10), (Int_t)(timingCuts[k][1]*10)));

            output << "\\begin{frame}" <<endl;
            output << "\\frametitle{Time-Counts-Histograms}" <<endl;
            output << "\\framesubtitle{With Time-Cut " << (Float_t) timingCuts[k][0] << " - " << (Float_t) timingCuts[k][1] << " ns}"<<endl;
            output << "\\begin{figure}" <<endl;
            output << "\\centering" <<endl;


            for (Int_t i = 0; i<6; i++){
                if (i>0) {
                    if (k<5) {
                        output << "\\only<" << i << ">{\\includegraphics[width =6 cm]{" << Form("%s/%dns_%dns/TimeCutVisulalisation_ECut:%d-%d", suffix.Data(), (Int_t)(timingCuts[k][0]*10), (Int_t)(timingCuts[k][1]*10), (Int_t) (energyCuts[i][0]*10), (Int_t) (energyCuts[i][1]*10))<< "}"<<endl;
                    } else {
                        output << "\\only<" << i << ">{\\includegraphics[width =6 cm]{" << Form("%s/%dns_%dns/TimeCutEnlargedVisulalisation_ECut:%d-%d", suffix.Data(), (Int_t)(timingCuts[k][0]*10), (Int_t)(timingCuts[k][1]*10), (Int_t) (energyCuts[i][0]*10), (Int_t) (energyCuts[i][1]*10))<< "}"<<endl;
                    }
                if (i== 4 || i==5){
                    output << "\\centering" <<endl;
                    output <<  "\\caption{Time-Count-Histogram\\\\Time-Cut: " << timingCuts[k][0] << " - " << timingCuts[k][1] << " ns\\\\Energy-Cut: " << (Float_t) energyCuts[i][0] << " - " << "$\\infty$ GeV}}" << endl;
                } else {
                    output << "\\centering" <<endl;
                    output << "\\caption{Time-Count-Histogram\\\\Time-Cut: " << timingCuts[k][0] << " - " << timingCuts[k][1] << " ns\\\\Energy-Cut: " << (Float_t) energyCuts[i][0] << " - " << (Float_t) energyCuts[i][1]<< " GeV}}" << endl;
                }}
                histo2DDummy->GetXaxis()->SetRangeUser(firstBinTime, lastBinTime);
                TH1D* histoTTagProjection     = (TH1D*)histoClusterTimevsETag[k]->ProjectionX("histoTTagProjection", histoClusterTimevsETag[k]->GetYaxis()->FindBin(energyCuts[i][0]), histoClusterTimevsETag[k]->GetYaxis()->FindBin(energyCuts[i][1]));
                TH1D* histoTProbeProjection    = (TH1D*)histoClusterTimevsEProbe[k]->ProjectionX("histoTProbeProjection", histoClusterTimevsEProbe[k]->GetYaxis()->FindBin(energyCuts[i][0]), histoClusterTimevsEProbe[k]->GetYaxis()->FindBin(energyCuts[i][1]));
                TH1D* histoTProbeProjectionWCut    = (TH1D*)histoClusterTimevsEProbeWCut[k]->ProjectionX("histoTProbeProjectionWCut", histoClusterTimevsEProbeWCut[k]->GetYaxis()->FindBin(energyCuts[i][0]), histoClusterTimevsEProbeWCut[k]->GetYaxis()->FindBin(energyCuts[i][1]));
                if (i == 4 || i==5) {
                    TH1D* histoTTagProjection     = (TH1D*)histoClusterTimevsETag[k]->ProjectionX("histoTTagProjection", histoClusterTimevsETag[k]->GetYaxis()->FindBin(energyCuts[i][0]),-1);
                    TH1D* histoTProbeProjection    = (TH1D*)histoClusterTimevsEProbe[k]->ProjectionX("histoTProbeProjection", histoClusterTimevsEProbe[k]->GetYaxis()->FindBin(energyCuts[i][0]), -1);
                    TH1D* histoTProbeProjectionWCut    = (TH1D*)histoClusterTimevsEProbeWCut[k]->ProjectionX("histoTProbeProjectionWCut", histoClusterTimevsEProbeWCut[k]->GetYaxis()->FindBin(energyCuts[i][0]), -1);
                }

                Float_t entries = histoClusterTimevsETag[k]->GetEntries();
                for (Int_t a = histoTProbeProjection->GetMinimumBin(); a< histoTProbeProjection->GetMaximumBin(); a++){
                // cout << histoTProbeProjection->GetBin(a) << endl;
                }
                histoTTagProjection->Scale(1/entries);
                histoTProbeProjection->Scale(1/entries);
                histoTProbeProjectionWCut->Scale(1/entries);
                Float_t max = 1e-100;
                Float_t min = 1e-101;
                if (histoTTagProjection->GetEntries() >0) max = histoTTagProjection->GetMaximum();
                if (histoTTagProjection->GetEntries() >0) min = histoTTagProjection->GetMinimum(0);
                if (histoTProbeProjection->GetEntries() > 0 && histoTProbeProjection->GetMaximum() > max)
                    max     = histoTProbeProjection->GetMaximum();
                if (histoTProbeProjectionWCut->GetEntries() > 0&& histoTProbeProjectionWCut->GetMinimum(0) < min)
                    min     = histoTProbeProjectionWCut->GetMinimum(0);

                if ( (max == 0) || (min == 0) ){
                    continue;
                }

                histo2DDummy->GetYaxis()->SetRangeUser(min, 10*max);
                histo2DDummy->GetXaxis()->SetRangeUser(-500, 500);
                histo2DDummy->DrawCopy();
                TLegend* legendEffi = GetAndSetLegend2(0.6,0.96,0.9,0.845,62,1, "");

                if (histoTTagProjection->GetEntries() >0){
                    DrawGammaSetMarker(histoTTagProjection, 25, 0.5, kBlack, kBlack);
                    histoTTagProjection->Draw("e1p,same");
                    legendEffi->AddEntry(histoTTagProjection,"#gamma_{Tag (WTaECut)}", "p" );
                }


                if (histoTProbeProjection->GetEntries() >0){
                    DrawGammaSetMarker(histoTProbeProjection, 24, 1., kBlue-8, kBlue-8);
                    histoTProbeProjection->Draw("e1p,same");
                    legendEffi->AddEntry(histoTProbeProjection, "#gamma_{Probe (WECut)}", "p" );
                }


                if (histoTProbeProjectionWCut->GetEntries() >0){
                    DrawGammaSetMarker(histoTProbeProjectionWCut, 20, 0.9, kRed+2, kRed+2);
                    histoTProbeProjectionWCut->Draw("e1p,same");
                    legendEffi->AddEntry(histoTProbeProjectionWCut, "#gamma_{Probe (WTaECut)}", "p" );
                }


                histo2DDummy->Draw("same,axis");


                legendEffi->Draw();
                TLatex *labelTime = new TLatex(0.14,0.93,Form("%s",collisionSystem.Data()));
                SetStyleTLatex( labelTime, 0.04,4);
                labelTime->Draw();
                TLatex *labelPlot = new TLatex(0.14,0.89,Form("%s",detectionProcess.Data()));
                SetStyleTLatex( labelPlot, 0.04,4);
                labelPlot->Draw();
                TLatex *labelcuts = new TLatex(0.14,0.85,Form("%4.1f < #it{t} (ns) < %4.1f", (timingCuts[k][0]),(timingCuts[k][1])));
                SetStyleTLatex( labelcuts, 0.04,4);
                labelcuts->Draw();
                TLatex *EnergyCut = NULL;
                TLatex *EnergyCut2 = NULL;
                if (i<4){
                    EnergyCut = new TLatex(0.14,0.81,Form("%4.1f < E (GeV) < %4.1f", energyCuts[i][0], energyCuts[i][1]));
                    SetStyleTLatex( EnergyCut, 0.04,4);
                    EnergyCut->Draw();
                } else if (i == 4 ||i == 5) {
                    EnergyCut2 = new TLatex(0.14,0.81,Form("%4.1f < E (GeV) < #infty", energyCuts[i][0]));
                    SetStyleTLatex( EnergyCut2, 0.04,4);
                    EnergyCut2->Draw();
                }

                canvasCompEff->Update();
                canvasCompEff->SaveAs(Form("%s/%dns_%dns/TimeCutVisulalisation_ECut:%d-%d.%s",outputDir.Data(), (Int_t)(timingCuts[k][0]*10), (Int_t)(timingCuts[k][1]*10), (Int_t) (energyCuts[i][0]*10), (Int_t) (energyCuts[i][1]*10), suffix.Data()));

                histo2DDummy->GetXaxis()->SetRangeUser(-40, 40);
                histo2DDummy->DrawCopy();

                if (histoTTagProjection->GetEntries() >0){
                    histoTTagProjection->Draw("e1p,same");
                }
                if (histoTProbeProjection->GetEntries() >0){
                    histoTProbeProjection->Draw("e1p,same");
                }
                if (histoTProbeProjectionWCut->GetEntries() >0){
                    histoTProbeProjectionWCut->Draw("e1p,same");
                }
                histo2DDummy->Draw("same,axis");
                legendEffi->Draw();
                labelTime->Draw();
                labelPlot->Draw();
                labelcuts->Draw();
                if (i<4){
                    EnergyCut->Draw();
                } else if (i == 4 ||i == 5) {
                    EnergyCut2->Draw();
                }
                canvasCompEff->SaveAs(Form("%s/%dns_%dns/TimeCutEnlargedVisulalisation_ECut:%d-%d.%s",outputDir.Data(), (Int_t)(timingCuts[k][0]*10), (Int_t)(timingCuts[k][1]*10), (Int_t) (energyCuts[i][0]*10), (Int_t) (energyCuts[i][1]*10), suffix.Data()));

                histoTTagProjection->Write("TestTTag",TObject::kWriteDelete);
                histoTProbeProjection->Write("TestTProbe",TObject::kWriteDelete);
                histoTProbeProjectionWCut->Write("TestTProbeWCut",TObject::kWriteDelete);
            }
            output << "\\end{figure}" << endl;
            output << "\\end{frame}" << endl;
            output.close();

        }

        TCanvas* canvasCompEffE = new TCanvas("canvasCompEffE","",200,10,1750,1500);// gives the page size
        DrawGammaCanvasSettings( canvasCompEffE, 0.1, 0.08, 0.02, 0.08);
        canvasCompEffE->SetLogy();
        canvasCompEffE->SetLogx();
        TH1F* histo3DDummy       = new TH1F("histo3DDummy","histo3DDummy",nBinsELog, firstBinE, lastBinE);
        SetStyleHistoTH1ForGraphs(histo3DDummy, "#it{E} (GeV)", "counts", 0.035 ,0.04, 0.035,0.04, 0.9, 0.95, 510, 505);
        output.open(Form("%s/EnergyCutV.tex",outputDir.Data()));
        output << "\\begin{figure}" << endl;
        output << "\\centering" << endl;
        for (Int_t k = iNumberOfTimingCutsM1; k > iNumberOfTimingCutsStartM1; k-- ){

            output << "\\only<" << k+1 << ">{\\includegraphics[width = 6 cm]{" << Form("%s/%dns_%dns/EnergyCutVisulalisation", suffix.Data(), (Int_t) (timingCuts[k][0]*10), (Int_t)(timingCuts[k][1]*10)) << "}" << endl;
            output << "\\caption{Energy-Counts-Histogram\\\\Time Cut: " << timingCuts[k][0] << " - " << timingCuts[k][1] << " ns}}" << endl;

            TH1D* histoETagProjection                  = (TH1D*)histoClusterTimevsETag[k]->ProjectionY("histoTTagProjection",0, -1);
            TH1D* histoEProbeProjection                = (TH1D*)histoClusterTimevsEProbe[k]->ProjectionY("histoTProbeProjection",0, -1);
            TH1D* histoEProbeProjectionWCut            = (TH1D*)histoClusterTimevsEProbeWCut[k]->ProjectionY("histoTProbeProjectionWCut", 0, -1);
            histo3DDummy->GetYaxis()->SetRangeUser(0.5,10*histoETagProjection->GetMaximum());
            histo3DDummy->DrawCopy();

            DrawGammaSetMarker(histoETagProjection, 25, 0.5, kBlack, kBlack);
            histoETagProjection ->Draw("e1p,same");
            DrawGammaSetMarker(histoEProbeProjection, 24, 1., kBlue-8, kBlue-8);
            histoEProbeProjection->Draw("e1p,same");
            DrawGammaSetMarker(histoEProbeProjectionWCut, 20, 0.9, kRed+2, kRed+2);
            histoEProbeProjectionWCut->Draw("e1p,same");
            histo3DDummy->Draw("same,axis");
            TLegend* legendEffi = GetAndSetLegend2(0.6,0.96,0.9,0.845,62,1, "");
            legendEffi->AddEntry(histoETagProjection,"#gamma_{Tag (WTCut)}", "p" );
            legendEffi->AddEntry(histoEProbeProjection, "#gamma_{Probe}", "p" );
            legendEffi->AddEntry( histoEProbeProjectionWCut, "#gamma_{Probe (WTCut)}", "p" );
            legendEffi->Draw();
            TLatex *labelTimeE = new TLatex(0.14,0.93,Form("%s",collisionSystem.Data()));
            SetStyleTLatex( labelTimeE, 0.04,4);
            labelTimeE->Draw();
            TLatex *labelPlotE = new TLatex(0.14,0.89,Form("%s",detectionProcess.Data()));
            SetStyleTLatex( labelPlotE, 0.04,4);
            labelPlotE->Draw();
            TLatex *labelcutsE = new TLatex(0.14,0.85,Form("%4.1f < #it{t} (ns) < %4.1f", (timingCuts[k][0]),(timingCuts[k][1])));
            SetStyleTLatex( labelcutsE, 0.04,4);
            labelcutsE->Draw();

            canvasCompEffE->Update();
            canvasCompEffE->SaveAs(Form("%s/%dns_%dns/EnergyCutVisulalisation.%s",outputDir.Data(), (Int_t)(timingCuts[k][0]*10), (Int_t)(timingCuts[k][1]*10), suffix.Data()));
        }
        output << "\\end{figure}" << endl;
        output.close();

        TCanvas* canvasCompEffInv = new TCanvas("canvasCompEffInv","",200,10,1350,900);// gives the page size
        DrawGammaCanvasSettings( canvasCompEffInv, 0.1, 0.08, 0.05, 0.08);
        TH1F* histo4DDummy = new TH1F("histo4DDummy","histo4DDummy",1000,minInvMass,maxInvMass);
        SetStyleHistoTH1ForGraphs(histo4DDummy, "#it{mc^{2}} (GeV)", "counts", 0.035 ,0.04, 0.035,0.04, 0.9, 0.85, 510, 505);
        Double_t Minimum = 0.0;
        Double_t Maximum = 0.0;
        output.open(Form("%s/InvMass.tex",outputDir.Data()));
        output << "\\begin{figure}" << endl;
            output << "\\centering" << endl;
        for (Int_t k = iNumberOfTimingCutsM1; k > iNumberOfTimingCutsStartM1; k-- ){


            output << "\\only<" << k+1 << ">{\\includegraphics[width = 6 cm]{" << Form("%s/%dns_%dns/InvMass", suffix.Data(), (Int_t) (timingCuts[k][0]*10), (Int_t)(timingCuts[k][1]*10)) << "}" << endl;
            output << "\\caption{Invariant mass-Counts-Histogram\\\\Time Cut: " << timingCuts[k][0] << " - " << timingCuts[k][1] << " ns}}" << endl;

            TH1D* histoInvMassProjection     = (TH1D*)histoMesonInvMassvsPt[k]->ProjectionX("histoInvMassProjection", histoMesonInvMassvsPt[k]->GetYaxis()->FindBin(1),histoMesonInvMassvsPt[k]->GetYaxis()->FindBin(2));
            TH1D* histo2InvMassProjection    = (TH1D*)histoMesonInvMassvsPt[k]->ProjectionX("histo2InvMassProjection", histoMesonInvMassvsPt[k]->GetYaxis()->FindBin(2),histoMesonInvMassvsPt[k]->GetYaxis()->FindBin(5));
            TH1D* histo3InvMassProjection    = (TH1D*)histoMesonInvMassvsPt[k]->ProjectionX("histo3InvMassProjection", histoMesonInvMassvsPt[k]->GetYaxis()->FindBin(5), -1);
            histoInvMassProjection->Scale(1/histoInvMassProjection->GetEntries());
            histo2InvMassProjection->Scale(1/histo2InvMassProjection->GetEntries());
            histo3InvMassProjection->Scale(1/histo3InvMassProjection->GetEntries());

            if ((histoInvMassProjection->GetMinimum(0) <= histo2InvMassProjection->GetMinimum(0)) && (histoInvMassProjection->GetMinimum(0) <= histo3InvMassProjection->GetMinimum(0))){Minimum= histoInvMassProjection->GetMinimum(0); } else
            if ((histo2InvMassProjection->GetMinimum(0) <= histoInvMassProjection->GetMinimum(0)) && (histo2InvMassProjection->GetMinimum(0) <= histo3InvMassProjection->GetMinimum(0))){Minimum= histo2InvMassProjection->GetMinimum(0); } else
           {Minimum= histo3InvMassProjection->GetMinimum(0); }

            if ((histoInvMassProjection->GetMaximum() >= histo2InvMassProjection->GetMaximum() ) &&( histoInvMassProjection->GetMaximum()  >= histo3InvMassProjection->GetMaximum() )){Maximum= histoInvMassProjection->GetMaximum() ; } else
            if ((histo2InvMassProjection->GetMaximum()  >= histoInvMassProjection->GetMaximum())  && (histo2InvMassProjection->GetMaximum()  >= histo3InvMassProjection->GetMaximum() )){Maximum= histo2InvMassProjection->GetMaximum() ; } else
           {Maximum= histo3InvMassProjection->GetMaximum() ; }

            histo4DDummy->GetYaxis()->SetRangeUser(Minimum*0.9,Maximum*1.2);
            histo4DDummy->DrawCopy();

            DrawGammaSetMarker(histoInvMassProjection, 25, 0.5, kBlack, kBlack);
            histoInvMassProjection->Draw("e1p,same");
            DrawGammaSetMarker(histo2InvMassProjection, 24, 1., kBlue-8, kBlue-8);
            histo2InvMassProjection->Draw("e1p,same");
            DrawGammaSetMarker(histo3InvMassProjection,  20, 0.9, kRed+2, kRed+2);
            histo3InvMassProjection->Draw("e1p,same");

            histo4DDummy->Draw("same,axis");

            TLegend* legendEffi = GetAndSetLegend2(0.55,0.92,0.9,0.83,31,1, "");
            legendEffi->AddEntry(histoInvMassProjection,"1 GeV/c < P_{T, #pi^{#gamma}} < 2 GeV/c", "p" );
            legendEffi->AddEntry(histo2InvMassProjection, "2 GeV/c < P_{T, #pi^{#gamma}} < 5 GeV/c" );
            legendEffi->AddEntry(histo3InvMassProjection, "5 GeV/c < P_{T, #pi^{#gamma}} < #infty GeV/c", "p" );
            legendEffi->Draw();

            TLatex *labelInv = new TLatex(0.14,0.90,Form("%s",collisionSystem.Data()));
            SetStyleTLatex( labelInv, 0.04,4);
            labelInv->Draw();
            TLatex *labelPlotInv = new TLatex(0.14,0.86,Form("%s",detectionProcess.Data()));
            SetStyleTLatex( labelPlotInv, 0.04,4);
            labelPlotInv->Draw();
            TLatex *labelcutsInv = new TLatex(0.14,0.82,Form("%4.1f < #it{t} (ns) < %4.1f", (timingCuts[k][0]),(timingCuts[k][1])));
            SetStyleTLatex( labelcutsInv, 0.04,4);
            labelcutsInv->Draw();

            TLatex *labelETag = new TLatex(0.14,0.78,Form("E_{#gamma, tag} > %4.1f GeV", minETag ));
            SetStyleTLatex( labelETag, 0.04,4);
            labelETag->Draw();

            canvasCompEffInv->Update();
            canvasCompEffInv->SaveAs(Form("%s/%dns_%dns/InvMass.%s",outputDir.Data(), (Int_t)(timingCuts[k][0]*10), (Int_t)(timingCuts[k][1]*10), suffix.Data()));
        }
        output << "\\end{figure}" << endl;
        output.close();

        output.open(Form("%s/OutputForFitParams.txt", outputDir.Data()));

        output.close();

    //********************************************************************************
    //*                  Writing histograms to outputfile                            *
    //********************************************************************************

        for (Int_t k = iNumberOfTimingCutsStart; k < iNumberOfTimingCuts; k++){

            fileTimeEffiWrite->cd(Form("%4.1fns_%4.1fns",timingCuts[k][0], timingCuts[k][1] ));
            histoMesonInvMassvsPt[k]->Write("histoMesonInvMassvsPt",TObject::kWriteDelete);
            histoClusterTimevsETag[k]->Write("histoClusterTimevsETag",TObject::kWriteDelete);
            histoClusterTimevsEProbe[k]->Write("histoClusterTimevsEProbe",TObject::kWriteDelete);
            histoClusterETag[k]->Write("histoClusterETag",TObject::kWriteDelete);
            histoClusterEProbe[k]->Write("histoClusterEProbe",TObject::kWriteDelete);
            histoClusterEProbeWCut[k]->Write("histoClusterEProbeWCut",TObject::kWriteDelete);
            histoClusterTTag[k]->Write("histoClusterTTag",TObject::kWriteDelete);
            histoClusterTProbe[k]->Write("histoClusterTProbe",TObject::kWriteDelete);
            histoClusterTProbeWCut[k]->Write("histoClusterTProbeWCut",TObject::kWriteDelete);
            histoClusterTProbeECut[k]->Write("histoClusterTProbeECut",TObject::kWriteDelete);
            histoClusterTProbeWCutECut[k]->Write("histoClusterTProbeWCutECut",TObject::kWriteDelete);
            histoClusterTimingEff[k]->Write("histoClusterTimingEff",TObject::kWriteDelete);
        }
    fileTimeEffiWrite->Write();
    fileTimeEffiWrite->Close();
    delete fileTimeEffiWrite;

    f->Close();
    delete f;

}
