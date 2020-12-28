/****************************************************************************************************************************
******      provided by Gamma Conversion Group, PWGGA,                                                                  *****
******      Friederike Bock, friederike.bock@cern.ch                                                                    *****
*****************************************************************************************************************************/

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
#include "CommonHeaders/PlottingGammaConversionHistos.h"
#include "CommonHeaders/PlottingGammaConversionAdditional.h"
#include "CommonHeaders/ConversionFunctions.h"

extern TRandom*    gRandom;
extern TBenchmark*    gBenchmark;
extern TSystem*    gSystem;
extern TMinuit*      gMinuit;

void RemoveUpperLimits(TGraphErrors* tg){
    int N = tg->GetN();
    double *x = tg->GetX();
    double *y = tg->GetY();
    double *ey = tg->GetEY();
    for(int i = 0; i < N; i++){
        if(ey[i]/y[i] > 0.8){
            y[i]=0;
            ey[i]=0;
        }
    }

}


void ProduceExperimentalDataGraphsPP(){

    // LMEE pp
    //==========================================================================================
    //                                         ALICE 7TeV
    //==========================================================================================
    // arXiv:1805.04391
    TString aliceLMEE7TeVHEPDataFile        = "ExternalInput/OtherParticles/LMEE_7TeV_Rgamma_HEPData-ins1672792-v1-Table_11.csv" ;
    TGraphAsymmErrors* RGammaStat_7TeV_LMEE_ALICE   = ParseHEPData(aliceLMEE7TeVHEPDataFile, 8, 0, 1, 2, 3, 4, 5, kFALSE, kTRUE, kFALSE);
    TGraphAsymmErrors* RGammaSys_7TeV_LMEE_ALICE    = ParseHEPData(aliceLMEE7TeVHEPDataFile, 8, 0, 1, 2, 3, 6, 7, kFALSE, kTRUE, kFALSE);
    TGraphAsymmErrors* RGammaTot_7TeV_LMEE_ALICE    = AddErrorsOfGraphsQuadratically(RGammaStat_7TeV_LMEE_ALICE, RGammaSys_7TeV_LMEE_ALICE);

    // Photons pp
    //==========================================================================================
    //                                         ALICE prelim isolated
    //==========================================================================================
//     Measurement:
//     Isolation Criteria: Charged+neutral Iso <2 GeV/c
//     measured in |eta|<0.27
    Float_t xbins[10]={10., 12., 14., 16., 18., 20., 25., 30., 40., 60.};
    Double_t ptisoALICE[9];
    Double_t ptisoALICEerr[9];
    for (Int_t i = 0; i < 9; i++){
        ptisoALICE[i]       = (xbins[i]+xbins[i+1])/2;
        ptisoALICEerr[i]    = (xbins[i]-xbins[i+1])/2;
    }
    Double_t sigma[9]={31.6,19.18,10.7,5.8,4.68,1.98,0.66,0.378,0.059};
    Double_t estatsigma[9]={2.84,1.81,1.07,0.8,0.67,0.26,0.21,0.09,0.024};
    Double_t esystlsigma[9]={4.74,2.58,1.4,0.8,0.8,0.39,0.155,0.08,0.011};
//
    TGraphAsymmErrors *tStat_7TeV_IsoPh_ALICE       = new TGraphAsymmErrors(9, ptisoALICE, sigma, ptisoALICEerr, ptisoALICEerr, estatsigma, estatsigma);
    TGraphAsymmErrors *tSys_7TeV_IsoPh_ALICE        = new TGraphAsymmErrors(9, ptisoALICE, sigma, ptisoALICEerr, ptisoALICEerr, esystlsigma, esystlsigma);
//     ScaleWithPtGraph(tStat_7TeV_IsoPh_ALICE);
//     ScaleWithPtGraph(tSys_7TeV_IsoPh_ALICE);
    TGraphAsymmErrors* tTot_7TeV_IsoPh_ALICE        = AddErrorsOfGraphsQuadratically(tStat_7TeV_IsoPh_ALICE, tSys_7TeV_IsoPh_ALICE);
    TGraphAsymmErrors *tTotxT_7TeV_IsoPh_ALICE      = xTScalePhoton(tTot_7TeV_IsoPh_ALICE, 7000);

    //==========================================================================================
    //                                         CMS
    //==========================================================================================
    // Phys.Rev. D84 (2011) 052011
    // Phys.Rev.Lett. 106 (2011) 082001
    const int N_CMS = 11;
    double pT_CMS[N_CMS] = {
        22, 24.5, 28.0, 32.5, 37.5, 42.5, 47.5, 55., 72.5, 102.5, 210.
    };
    double y_CMS[N_CMS] = {
        2.17, 1.39, 0.774, 0.402, 0.209, 0.1244, 0.0740, 0.0403, 0.01236, 0.00243, 0.000188
    };
    double ey_CMS[N_CMS] = {
        0.03, 0.02, 0.010, 0.006, 0.004, 0.0028, 0.0021, 0.0010, 0.00035, 0.00012, 0.000013
    };
    double sUPy_CMS[N_CMS] = {
        13.0, 13.0, 13.0, 13.0, 13.0, 13.0, 13.0, 13.0, 14.0, 14.0, 13.0
    };
    double sLOWy_CMS[N_CMS] = {
        16.0, 16.0, 16.0, 16.0, 16.0, 16.0, 16.0, 16.0, 16.0, 9.0, 9.0
    };
    double E_CMS = 7000.;

    double sUy_CMS[N_CMS] = {0};
    double sLy_CMS[N_CMS] = {0};
    for(int i = 0; i < N_CMS; i++){
        sUy_CMS[i] = y_CMS[i]*sUPy_CMS[i];
        sLy_CMS[i] = y_CMS[i]*sLOWy_CMS[i];
    }
    TGraphErrors *tE_DirPh_CMS          = new TGraphErrors(N_CMS, pT_CMS, y_CMS, 0, ey_CMS);
//     ScaleWithPtGraph(tE_DirPh_CMS);
    TGraphAsymmErrors *tS_DirPh_CMS     = new TGraphAsymmErrors(N_CMS, pT_CMS, y_CMS, 0, 0, sLy_CMS, sUy_CMS);
//     ScaleWithPtGraph(tS_DirPh_CMS);
    TGraphErrors *tExT_DirPh_CMS        = xTScalePhoton(tE_DirPh_CMS, E_CMS);

    //Phys.Rev.Lett. 106 (2011) 082001, 2011
    TString cms7TeVHEPDataFile        = "ExternalInput/OtherExperiments/CMS_7TeV_HEPData-ins922830-v1-Table_1.csv" ;
    TGraphAsymmErrors* tStat_7TeV_DirPh_CMS   = ParseHEPData(cms7TeVHEPDataFile, 8, 0, 1, 2, 3, 4, 5, kFALSE, kTRUE, kFALSE);
    TGraphAsymmErrors* tSys_7TeV_DirPh_CMS    = ParseHEPData(cms7TeVHEPDataFile, 8, 0, 1, 2, 3, 6, 7, kFALSE, kTRUE, kFALSE);
//     ScaleWithPtGraph(tStat_7TeV_DirPh_CMS);
//     ScaleWithPtGraph(tSys_7TeV_DirPh_CMS);
    TGraphAsymmErrors* tTot_7TeV_DirPh_CMS    = AddErrorsOfGraphsQuadratically(tStat_7TeV_DirPh_CMS, tSys_7TeV_DirPh_CMS);
    TGraphAsymmErrors *tTotxT_7TeV_DirPh_CMS  = xTScalePhoton(tTot_7TeV_DirPh_CMS, E_CMS);
    ProduceGraphAsymmWithoutXErrors( tTotxT_7TeV_DirPh_CMS);

    //Phys.Lett. B710 (2012) 256-277, 2012
    TString cms2760GeVHEPDataFile        = "ExternalInput/OtherExperiments/CMS_2760GeV_HEPData-ins1084729-v1-Table_3.csv" ;
    TGraphAsymmErrors* tStat_2760GeV_DirPh_CMS   = ParseHEPData(cms2760GeVHEPDataFile, 8, 0, 1, 2, 3, 4, 5, kFALSE, kTRUE, kFALSE);
    TGraphAsymmErrors* tSys_2760GeV_DirPh_CMS    = ParseHEPData(cms2760GeVHEPDataFile, 8, 0, 1, 2, 3, 6, 7, kFALSE, kTRUE, kFALSE);
//     ScaleWithPtGraph(tStat_2760GeV_DirPh_CMS);
//     ScaleWithPtGraph(tSys_2760GeV_DirPh_CMS);
    tStat_2760GeV_DirPh_CMS                      = ScaleGraph(tStat_2760GeV_DirPh_CMS, 1e-3);
    tSys_2760GeV_DirPh_CMS                       = ScaleGraph(tSys_2760GeV_DirPh_CMS, 1e-3);
//     tStat_2760GeV_DirPh_CMS                      = ScaleGraph(tStat_2760GeV_DirPh_CMS, 1e-3/(2*1.45));
//     tSys_2760GeV_DirPh_CMS                       = ScaleGraph(tSys_2760GeV_DirPh_CMS, 1e-3/(2*1.45));
    TGraphAsymmErrors* tTot_2760GeV_DirPh_CMS    = AddErrorsOfGraphsQuadratically(tStat_2760GeV_DirPh_CMS, tSys_2760GeV_DirPh_CMS);


    tTot_2760GeV_DirPh_CMS->Print();
    TGraphAsymmErrors *tTotxT_2760GeV_DirPh_CMS  = xTScalePhoton(tTot_2760GeV_DirPh_CMS, 2760.);
    ProduceGraphAsymmWithoutXErrors( tTotxT_2760GeV_DirPh_CMS);
    tTotxT_2760GeV_DirPh_CMS->Print();
    //==========================================================================================
    //                                         ATLAS
    //==========================================================================================
    // Phys.Lett. B706 (2011) 150-167, 2011
    const int N_ATLAS = 8;
    double pT_ATLAS[N_ATLAS] = { 50.0, 62.5, 77.5, 92.5, 112.5, 137.5, 175.0, 300.0 };
    double y_ATLAS[N_ATLAS] = { 83.3, 32.7, 12.3, 5.3, 2.2, 0.8, 0.26, 0.028 };
    double sy_ATLAS[N_ATLAS] = { 7.210409142344143, 2.6851443164195103, 0.938083151964686, 0.4123105625617661, 0.20615528128088306, 0.06324555320336758, 0.022360679774997897, 0.0031304951684997056 };
    double ey_ATLAS[N_ATLAS] = { 0.5, 0.3, 0.2, 0.1, 0.05, 0.03, 0.01, 0.002 };
    double E_ATLAS = 7000.;

    TGraphErrors *tE_DirPh_ATLAS    = new TGraphErrors(N_ATLAS, pT_ATLAS, y_ATLAS, 0, ey_ATLAS);
    TGraphErrors *tS_DirPh_ATLAS    = new TGraphErrors(N_ATLAS, pT_ATLAS, y_ATLAS, 0, sy_ATLAS);
    tE_DirPh_ATLAS                  = ScaleGraph(tE_DirPh_ATLAS, 0.001);
//     ScaleWithPtGraph(tE_DirPh_ATLAS);
    tS_DirPh_ATLAS                  = ScaleGraph(tS_DirPh_ATLAS, 0.001);
//     ScaleWithPtGraph(tS_DirPh_ATLAS);
    TGraphErrors *tExT_DirPh_ATLAS  = xTScalePhoton(tE_DirPh_ATLAS, E_ATLAS);

    TString atlas7TeVHEPDataFile0        = "ExternalInput/OtherExperiments/IsoGamma/ATLAS_7TeV_2011_HEPData-ins921594-v1-csv/Table1_mod.csv" ;
    TGraphAsymmErrors* tStat_7TeV_DirPh_ATLAS0   = ParseHEPData(atlas7TeVHEPDataFile0, 10, 0, 1, 2, 3, 4, 5, kFALSE, kTRUE, kFALSE);
    TGraphAsymmErrors* tSys_7TeV_DirPh_ATLAS0    = ParseHEPData(atlas7TeVHEPDataFile0, 10, 0, 1, 2, 3, 6, 7, kFALSE, kTRUE, kFALSE);
    tStat_7TeV_DirPh_ATLAS0                      = ScaleGraph(tStat_7TeV_DirPh_ATLAS0, 0.001/(2*0.6));
//     ScaleWithPtGraph(tStat_7TeV_DirPh_ATLAS0);
    tSys_7TeV_DirPh_ATLAS0                       = ScaleGraph(tSys_7TeV_DirPh_ATLAS0, 0.001/(2*0.6));
//     ScaleWithPtGraph(tSys_7TeV_DirPh_ATLAS0);
    TGraphAsymmErrors* tTot_7TeV_DirPh_ATLAS0    = AddErrorsOfGraphsQuadratically(tStat_7TeV_DirPh_ATLAS0, tSys_7TeV_DirPh_ATLAS0);
    TGraphAsymmErrors *tTotxT_7TeV_DirPh_ATLAS0  = xTScalePhoton(tTot_7TeV_DirPh_ATLAS0, 7000.);
    ProduceGraphAsymmWithoutXErrors( tTotxT_7TeV_DirPh_ATLAS0);

    // Phys.Rev. D83 (2011) 052005, 2011
    TString atlas7TeVHEPDataFile        = "ExternalInput/OtherExperiments/ATLAS_7TeV_HEPData-ins882463-v1-Table_1.csv" ;
    TGraphAsymmErrors* tStat_7TeV_DirPh_ATLAS   = ParseHEPData(atlas7TeVHEPDataFile, 10, 0, 1, 2, 3, 4, 5, kFALSE, kTRUE, kFALSE);
    TGraphAsymmErrors* tSys_7TeV_DirPh_ATLAS    = ParseHEPData(atlas7TeVHEPDataFile, 10, 0, 1, 2, 3, 6, 7, kFALSE, kTRUE, kFALSE);
    tStat_7TeV_DirPh_ATLAS                      = ScaleGraph(tStat_7TeV_DirPh_ATLAS, 1/(2*0.6));
//     ScaleWithPtGraph(tStat_7TeV_DirPh_ATLAS);
    tSys_7TeV_DirPh_ATLAS                       = ScaleGraph(tSys_7TeV_DirPh_ATLAS, 1/(2*0.6));
//     ScaleWithPtGraph(tSys_7TeV_DirPh_ATLAS);
    TGraphAsymmErrors* tTot_7TeV_DirPh_ATLAS    = AddErrorsOfGraphsQuadratically(tStat_7TeV_DirPh_ATLAS, tSys_7TeV_DirPh_ATLAS);
    TGraphAsymmErrors *tTotxT_7TeV_DirPh_ATLAS  = xTScalePhoton(tTot_7TeV_DirPh_ATLAS, E_ATLAS);
    ProduceGraphAsymmWithoutXErrors( tTotxT_7TeV_DirPh_ATLAS);

    // Phys.Rev. D89 (2014) 052004, 2014
    TString atlas7TeVHEPDataFile2        = "ExternalInput/OtherExperiments/IsoGamma/ATLAS_7TeV_HEPData-ins1263495-v1-csv/Table1.csv" ;
    TGraphAsymmErrors* tStat_7TeV_DirPh_ATLAS2   = ParseHEPData(atlas7TeVHEPDataFile2, 8, 0, 1, 2, 3, 4, 5, kFALSE, kTRUE, kFALSE);
    TGraphAsymmErrors* tSys_7TeV_DirPh_ATLAS2    = ParseHEPData(atlas7TeVHEPDataFile2, 8, 0, 1, 2, 3, 6, 7, kFALSE, kTRUE, kFALSE);
    tStat_7TeV_DirPh_ATLAS2                      = ScaleGraph(tStat_7TeV_DirPh_ATLAS2, 0.001/(2*1.37));
//     ScaleWithPtGraph(tStat_7TeV_DirPh_ATLAS2);
    tSys_7TeV_DirPh_ATLAS2                       = ScaleGraph(tSys_7TeV_DirPh_ATLAS2, 0.001/(2*1.37));
//     ScaleWithPtGraph(tSys_7TeV_DirPh_ATLAS2);
    TGraphAsymmErrors* tTot_7TeV_DirPh_ATLAS2    = AddErrorsOfGraphsQuadratically(tStat_7TeV_DirPh_ATLAS2, tSys_7TeV_DirPh_ATLAS2);
    TGraphAsymmErrors *tTotxT_7TeV_DirPh_ATLAS2  = xTScalePhoton(tTot_7TeV_DirPh_ATLAS2, 7000.);
    ProduceGraphAsymmWithoutXErrors( tTotxT_7TeV_DirPh_ATLAS2);

    //JHEP 1608 (2016) 005, 2016
    TString atlas8TeVHEPDataFile        = "ExternalInput/OtherExperiments/IsoGamma/ATLAS_8TeV_HEPData-ins1457605-v1-csv/Table1_eta06_mod.csv" ;
    TGraphAsymmErrors* tStat_8TeV_DirPh_ATLAS   = ParseHEPData(atlas8TeVHEPDataFile, 10, 0, 1, 2, 3, 4, 5, kFALSE, kTRUE, kFALSE);
    TGraphAsymmErrors* tSys_8TeV_DirPh_ATLAS    = ParseHEPData(atlas8TeVHEPDataFile, 10, 0, 1, 2, 3, 6, 7, kFALSE, kTRUE, kFALSE);
    tStat_8TeV_DirPh_ATLAS                      = ScaleGraph(tStat_8TeV_DirPh_ATLAS, 0.001/(2*0.6));
//     ScaleWithPtGraph(tStat_8TeV_DirPh_ATLAS);
    tSys_8TeV_DirPh_ATLAS                       = ScaleGraph(tSys_8TeV_DirPh_ATLAS, 0.001/(2*0.6));
//     ScaleWithPtGraph(tSys_8TeV_DirPh_ATLAS);
    TGraphAsymmErrors* tTot_8TeV_DirPh_ATLAS    = AddErrorsOfGraphsQuadratically(tStat_8TeV_DirPh_ATLAS, tSys_8TeV_DirPh_ATLAS);
    TGraphAsymmErrors *tTotxT_8TeV_DirPh_ATLAS  = xTScalePhoton(tTot_8TeV_DirPh_ATLAS, 8000.);
    ProduceGraphAsymmWithoutXErrors( tTotxT_8TeV_DirPh_ATLAS);

    // Phys.Lett. B770 (2017) 473-493
    TString atlas13TeVHEPDataFile        = "ExternalInput/OtherExperiments/IsoGamma/atlas13TeV_HEPData_mod.csv" ;
    TGraphAsymmErrors* tStat_13TeV_DirPh_ATLAS   = ParseHEPData(atlas13TeVHEPDataFile, 10, 0, 1, 2, 3, 4, 5, kFALSE, kTRUE, kFALSE);
    TGraphAsymmErrors* tSys_13TeV_DirPh_ATLAS    = ParseHEPData(atlas13TeVHEPDataFile, 10, 0, 1, 2, 3, 6, 7, kFALSE, kTRUE, kFALSE);
    tStat_13TeV_DirPh_ATLAS                      = ScaleGraph(tStat_13TeV_DirPh_ATLAS, 0.001/(2*0.6));
//     ScaleWithPtGraph(tStat_13TeV_DirPh_ATLAS);
    tSys_13TeV_DirPh_ATLAS                       = ScaleGraph(tSys_13TeV_DirPh_ATLAS, 0.001/(2*0.6));
//     ScaleWithPtGraph(tSys_13TeV_DirPh_ATLAS);
    TGraphAsymmErrors* tTot_13TeV_DirPh_ATLAS    = AddErrorsOfGraphsQuadratically(tStat_13TeV_DirPh_ATLAS, tSys_13TeV_DirPh_ATLAS);
    TGraphAsymmErrors *tTotxT_13TeV_DirPh_ATLAS  = xTScalePhoton(tTot_13TeV_DirPh_ATLAS, 13000.);
    ProduceGraphAsymmWithoutXErrors( tTotxT_13TeV_DirPh_ATLAS);

//     // Phys.Lett. B770 (2017) 473-493
// //     TFile* fileatlas13TeV                       = new TFile("ExternalInput/OtherExperiments/ATLAS_13TeV_compilation_Hendrik.root");
// //     TGraphAsymmErrors* tTot_13TeV_DirPh_ATLAS   = (TGraphAsymmErrors*)fileatlas13TeV->Get("atlas2017_pt/atlas2017_pt_data_rap1");
// //     tTot_13TeV_DirPh_ATLAS                      = ScaleGraph(tTot_13TeV_DirPh_ATLAS, 1e-3);
// //     TGraphAsymmErrors *tTotxT_13TeV_DirPh_ATLAS = xTScalePhoton(tTot_13TeV_DirPh_ATLAS, 13000.);
//
//     TFile* fileatlas13TeVStat                   = new TFile("ExternalInput/OtherExperiments/xsections_incphoton_ATLAS_13TeV.root");
//     TH1D* histStat_13TeV_DirPh_ATLAS            = (TH1D*)fileatlas13TeVStat->Get("hxsect_eta_0");
//     TGraphAsymmErrors* tStat_13TeV_DirPh_ATLAS  = new TGraphAsymmErrors(histStat_13TeV_DirPh_ATLAS);
//     RemoveZerosAtBeginningAndEndFromGraph(tStat_13TeV_DirPh_ATLAS);
//     TFile* fileatlas13TeVSys                    = new TFile("ExternalInput/OtherExperiments/total_syst_incphoton_ATLAS_13TeV.root");
//     TH1D* histRelSysDown_13TeV_DirPh_ATLAS      = (TH1D*)fileatlas13TeVSys->Get("histo_totalup_eta_0");
//     TGraphAsymmErrors* tRelSysDown13TeVATLAS    = new TGraphAsymmErrors(histRelSysDown_13TeV_DirPh_ATLAS);
//     RemoveZerosAtBeginningAndEndFromGraph(tRelSysDown13TeVATLAS);
//     TH1D* histRelSysUp_13TeV_DirPh_ATLAS        = (TH1D*)fileatlas13TeVSys->Get("histo_totaldown_eta_0");
//     TGraphAsymmErrors* tRelSysUp13TeVATLAS    = new TGraphAsymmErrors(histRelSysUp_13TeV_DirPh_ATLAS);
//     RemoveZerosAtBeginningAndEndFromGraph(tRelSysUp13TeVATLAS);
//     TGraphAsymmErrors* tSys_13TeV_DirPh_ATLAS   = (TGraphAsymmErrors*)tStat_13TeV_DirPh_ATLAS->Clone("tSys_13TeV_DirPh_ATLAS");
//     for (Int_t i = 0; i< tSys_13TeV_DirPh_ATLAS->GetN(); i++){
//         tSys_13TeV_DirPh_ATLAS->SetPointError(i, 0, 0, tRelSysDown13TeVATLAS->GetY()[i]*tSys_13TeV_DirPh_ATLAS->GetY()[i], tRelSysUp13TeVATLAS->GetY()[i]*tSys_13TeV_DirPh_ATLAS->GetY()[i] );
//     }
//
//     TGraphAsymmErrors* tTot_13TeV_DirPh_ATLAS   = AddErrorsOfGraphsQuadratically(tStat_13TeV_DirPh_ATLAS, tSys_13TeV_DirPh_ATLAS);
//     tTot_13TeV_DirPh_ATLAS                      = ScaleGraph(tTot_13TeV_DirPh_ATLAS, 1e-3);
//     TGraphAsymmErrors *tTotxT_13TeV_DirPh_ATLAS = xTScalePhoton(tTot_13TeV_DirPh_ATLAS, 13000.);

    //==========================================================================================
    //                                         D0
    //==========================================================================================
    // Phys. Rev. Lett. 84 (2000) 2786
    TString d01940GeVHEPDataFile        = "ExternalInput/OtherExperiments/IsoGamma/D0_1940GeV_HEPData-ins698784-v1/Table1.csv";
    TGraphAsymmErrors* tStat_1940GeV_DirPh_D0   = ParseHEPData(d01940GeVHEPDataFile, 8, 0, 1, 2, 3, 4, 5, kFALSE, kTRUE, kFALSE);
    TGraphAsymmErrors* tSys_1940GeV_DirPh_D0    = ParseHEPData(d01940GeVHEPDataFile, 8, 0, 1, 2, 3, 6, 7, kFALSE, kTRUE, kFALSE);
    tStat_1940GeV_DirPh_D0                      = ScaleGraph(tStat_1940GeV_DirPh_D0, 0.001*2*TMath::Pi());
    //     ScaleWithPtGraph(tStat_1940GeV_DirPh_D0);
    tSys_1940GeV_DirPh_D0                       = ScaleGraph(tSys_1940GeV_DirPh_D0, 0.001*2*TMath::Pi());
    //     ScaleWithPtGraph(tSys_1940GeV_DirPh_D0);
    TGraphAsymmErrors* tTot_1940GeV_DirPh_D0    = AddErrorsOfGraphsQuadratically(tStat_1940GeV_DirPh_D0, tSys_1940GeV_DirPh_D0);
    cout << "D0" << endl;
    tTot_1940GeV_DirPh_D0->Print();
    TGraphAsymmErrors *tTotxT_1940GeV_DirPh_D0  = xTScalePhoton(tTot_1940GeV_DirPh_D0, 1940);
    ProduceGraphAsymmWithoutXErrors( tTotxT_1940GeV_DirPh_D0);


    // Phys.Rev.Lett. 77 (1996) 5011-5015, 1996
    const int N_D0 = 23;
    double pT_D0[N_D0] = { 10.5, 13.5, 16.5, 19.5, 22.5, 25.5, 28.5, 31.5, 37.4,
    40.5, 43.5, 46.5, 49.5, 52.5, 55.5, 58.5, 61.5, 65.7, 72.0,
    78.0, 85.1, 94.4, 108.4 };
    double y_D0[N_D0] = { 14200.0, 4010.0, 1290.0, 661.0, 315.0, 157.0, 105.0, 63.9, 28.1,
    19.7, 14.2, 11.1, 7.76, 5.83, 4.72, 3.3, 2.98, 2.17, 1.39,
    0.88, 0.67, 0.28, 0.11 };
    double sy_D0[N_D0] = { 5262.052546297879, 1013.61383179197, 261.00766272276377, 113.81124724736128, 40.85339643163099, 21.95449840010015, 15.0, 9.859513172565876, 2.4738633753705965,
    1.772004514666935, 1.2649110640673518, 1.077032961426901, 0.73824115301167, 0.5730619512757762, 0.47169905660283024, 0.35608987629529715, 0.33421549934136807, 0.23021728866442678, 0.1562049935181331,
    0.1131370849898476, 0.08485281374238571, 0.044721359549995794, 0.022360679774997897 };
    double ey_D0[N_D0] = { 291.0, 153.0, 90.0, 67.0, 15.0, 11.0, 9.0, 7.5, 0.6,
    0.5, 0.4, 0.4, 0.31, 0.28, 0.25, 0.22, 0.21, 0.13, 0.1,
    0.08, 0.06, 0.04, 0.02 };
    double E_D0 = 1800.;

    TGraphErrors *tE_DirPh_D0 = new TGraphErrors(N_D0, pT_D0, y_D0, 0, ey_D0);
    TGraphErrors *tS_DirPh_D0 = new TGraphErrors(N_D0, pT_D0, y_D0, 0, sy_D0);
//     ScaleWithPtGraph(tE_DirPh_D0);
//     ScaleWithPtGraph(tS_DirPh_D0);
    tE_DirPh_D0               = ScaleGraph(tE_DirPh_D0, 0.001*2*TMath::Pi());
    tS_DirPh_D0               = ScaleGraph(tS_DirPh_D0, 0.001*2*TMath::Pi());
    //     styleme(tE_DirPh_D0, 25);
    TGraphErrors *tExT_DirPh_D0  = xTScalePhoton(tE_DirPh_D0, E_D0);


    // Phys. Rev. Lett. 87 (2001) 251805
    TString d0630GeVHEPDataFile        = "ExternalInput/OtherExperiments/IsoGamma/D0_630_1800GeV_HEPData-ins557816-v1/Table1.csv";
    TGraphAsymmErrors* tStat_630GeV_DirPh_D0   = ParseHEPData(d0630GeVHEPDataFile, 8, 0, 1, 2, 3, 4, 5, kFALSE, kTRUE, kFALSE);
    TGraphAsymmErrors* tSys_630GeV_DirPh_D0    = ParseHEPData(d0630GeVHEPDataFile, 8, 0, 1, 2, 3, 6, 7, kFALSE, kTRUE, kFALSE);
    tStat_630GeV_DirPh_D0                      = ScaleGraph(tStat_630GeV_DirPh_D0, 0.001*2*TMath::Pi());
    //     ScaleWithPtGraph(tStat_630GeV_DirPh_D0);
    tSys_630GeV_DirPh_D0                       = ScaleGraph(tSys_630GeV_DirPh_D0, 0.001*2*TMath::Pi());
    //     ScaleWithPtGraph(tSys_630GeV_DirPh_D0);
    TGraphAsymmErrors* tTot_630GeV_DirPh_D0    = AddErrorsOfGraphsQuadratically(tStat_630GeV_DirPh_D0, tSys_630GeV_DirPh_D0);
    cout << "D0" << endl;
    tTot_630GeV_DirPh_D0->Print();
    TGraphAsymmErrors *tTotxT_630GeV_DirPh_D0  = xTScalePhoton(tTot_630GeV_DirPh_D0, 630);
    ProduceGraphAsymmWithoutXErrors( tTotxT_630GeV_DirPh_D0);

    //==========================================================================================
    //                                         CDF
    //==========================================================================================

    // Phys Rev. D80 (2009) 111106
    TString cdf1940GeVHEPDataFile        = "ExternalInput/OtherExperiments/IsoGamma/CDF_1940GeV_HEPData-ins834437-v1-csv/Table1.csv";
    TGraphAsymmErrors* tStat_1940GeV_DirPh_CDF   = ParseHEPData(cdf1940GeVHEPDataFile, 6, 0, -1, -1, 1, 2, 3, kFALSE, kTRUE, kFALSE);
    TGraphAsymmErrors* tSys_1940GeV_DirPh_CDF    = ParseHEPData(cdf1940GeVHEPDataFile, 6, 0, -1, -1, 1, 4, 5, kFALSE, kTRUE, kFALSE);
    tStat_1940GeV_DirPh_CDF                      = ScaleGraph(tStat_1940GeV_DirPh_CDF, 0.001*2*TMath::Pi());
    //     ScaleWithPtGraph(tStat_1940GeV_DirPh_CDF);
    tSys_1940GeV_DirPh_CDF                       = ScaleGraph(tSys_1940GeV_DirPh_CDF, 0.001*2*TMath::Pi());
    //     ScaleWithPtGraph(tSys_1940GeV_DirPh_CDF);
    TGraphAsymmErrors* tTot_1940GeV_DirPh_CDF    = AddErrorsOfGraphsQuadratically(tStat_1940GeV_DirPh_CDF, tSys_1940GeV_DirPh_CDF);
    TGraphAsymmErrors *tTotxT_1940GeV_DirPh_CDF  = xTScalePhoton(tTot_1940GeV_DirPh_CDF, 1940);
    ProduceGraphAsymmWithoutXErrors( tTotxT_1940GeV_DirPh_CDF);


    // Phys.Rev.Lett. 73 (1994) 2662-2666, 1994
    const int N_CDF = 16;
    double pT_CDF[N_CDF] = { 12.3, 17.0, 19.0, 21.0, 23.0, 25.0, 27.0, 29.0, 31.0,
    33.9, 37.9, 41.9, 48.9, 62.4, 80.8, 114.7 };
    double y_CDF[N_CDF] = { 4460.0, 1300.0, 805.0, 458.0, 308.0, 226.0, 163.0, 106.0, 76.7,
    53.7, 30.9, 20.5, 7.61, 3.09, 0.911, 0.163 };
    double sy_CDF[N_CDF] = { 825.8456272209716, 160.5615146914104, 91.44397191723465, 48.38388161361178, 33.24154027718932, 25.079872407968907, 17.88854381999832, 12.529964086141668, 8.823831367382313,
    5.768882040742383, 3.6878177829171546, 2.6172504656604803, 1.0748023074035522, 0.4455333881989093, 0.18319934497699494, 0.04477722635447622 };
    double ey_CDF[N_CDF] = { 415.0, 38.0, 21.0, 15.0, 12.0, 10.0, 8.0, 6.0, 5.5,
    3.2, 2.4, 1.9, 0.76, 0.32, 0.159, 0.041 };
    double E_CDF = 1800;
    TGraphErrors *tE_DirPh_CDF  = new TGraphErrors(N_CDF, pT_CDF, y_CDF, 0, ey_CDF);
    TGraphErrors *tS_DirPh_CDF  = new TGraphErrors(N_CDF, pT_CDF, y_CDF, 0, sy_CDF);
//     ScaleWithPtGraph(tE_DirPh_CDF);
//     ScaleWithPtGraph(tS_DirPh_CDF);
    tE_DirPh_CDF                = ScaleGraph(tE_DirPh_CDF, 0.001*2*TMath::Pi());
    tS_DirPh_CDF                = ScaleGraph(tS_DirPh_CDF, 0.001*2*TMath::Pi());
    //     styleme(tE_DirPh_CDF, 24);
    TGraphErrors *tExT_DirPh_CDF  = xTScalePhoton(tE_DirPh_CDF, E_CDF);


    // Phys. Rev. D65 (2002) 112003
    TString cdf1800GeVHEPDataFile        = "ExternalInput/OtherExperiments/IsoGamma/CDF_630_1800GeV_HEPData-ins581379-v1/Table1.csv";
    TGraphAsymmErrors* tStat_1800GeV_DirPh_CDF   = ParseHEPData(cdf1800GeVHEPDataFile, 6, 0, -1, -1, 1, 2, 3, kFALSE, kTRUE, kFALSE);
    TGraphAsymmErrors* tSys_1800GeV_DirPh_CDF    = ParseHEPData(cdf1800GeVHEPDataFile, 6, 0, -1, -1, 1, 4, 5, kFALSE, kTRUE, kFALSE);
    tStat_1800GeV_DirPh_CDF                      = ScaleGraph(tStat_1800GeV_DirPh_CDF, 0.001*2*TMath::Pi());
//     ScaleWithPtGraph(tStat_1800GeV_DirPh_CDF);
    tSys_1800GeV_DirPh_CDF                       = ScaleGraph(tSys_1800GeV_DirPh_CDF, 0.001*2*TMath::Pi());
//     ScaleWithPtGraph(tSys_1800GeV_DirPh_CDF);
    TGraphAsymmErrors* tTot_1800GeV_DirPh_CDF    = AddErrorsOfGraphsQuadratically(tStat_1800GeV_DirPh_CDF, tSys_1800GeV_DirPh_CDF);
    TGraphAsymmErrors *tTotxT_1800GeV_DirPh_CDF  = xTScalePhoton(tTot_1800GeV_DirPh_CDF, 1800);
    ProduceGraphAsymmWithoutXErrors( tTotxT_1800GeV_DirPh_CDF);

    // Phys. Rev. D65 (2002) 112003
    TString cdf630GeVHEPDataFile        = "ExternalInput/OtherExperiments/IsoGamma/CDF_630_1800GeV_HEPData-ins581379-v1/Table2.csv";
    TGraphAsymmErrors* tStat_630GeV_DirPh_CDF   = ParseHEPData(cdf630GeVHEPDataFile, 6, 0, -1, -1, 1, 2, 3, kFALSE, kTRUE, kFALSE);
    TGraphAsymmErrors* tSys_630GeV_DirPh_CDF    = ParseHEPData(cdf630GeVHEPDataFile, 6, 0, -1, -1, 1, 4, 5, kFALSE, kTRUE, kFALSE);
    tStat_630GeV_DirPh_CDF                      = ScaleGraph(tStat_630GeV_DirPh_CDF, 0.001*2*TMath::Pi());
    //     ScaleWithPtGraph(tStat_630GeV_DirPh_CDF);
    tSys_630GeV_DirPh_CDF                       = ScaleGraph(tSys_630GeV_DirPh_CDF, 0.001*2*TMath::Pi());
    //     ScaleWithPtGraph(tSys_630GeV_DirPh_CDF);
    TGraphAsymmErrors* tTot_630GeV_DirPh_CDF    = AddErrorsOfGraphsQuadratically(tStat_630GeV_DirPh_CDF, tSys_630GeV_DirPh_CDF);
    TGraphAsymmErrors *tTotxT_630GeV_DirPh_CDF  = xTScalePhoton(tTot_630GeV_DirPh_CDF, 630);
    ProduceGraphAsymmWithoutXErrors( tTotxT_630GeV_DirPh_CDF);


    //==========================================================================================
    //                                         UA1_0
    //==========================================================================================
    // Phys.Lett. B209 (1988) 385-396
    const int N_UA1_0 = 16;
    double pT_UA1_0[N_UA1_0] = { 17.0, 19.0, 21.0, 23.0, 25.0, 27.0, 29.0, 31.5, 34.5,
    37.5, 40.5, 46.0, 55.0, 65.0, 75.0, 90.0 };
    double y_UA1_0[N_UA1_0] = { 6.42, 3.3, 1.54, 0.74, 0.5, 0.381, 0.246, 0.123, 0.056,
    0.051, 0.03, 0.0111, 0.0039, 0.0037, 0.0013, 2.0E-4 };
    double sy_UA1_0[N_UA1_0] = { 1.2567020331009258, 0.6158733636065129, 0.2973213749463701, 0.1140175425099138, 0.07810249675906655, 0.0604400529450463, 0.043046486500061765, 0.023259406699226014, 0.014560219778561038,
    0.013341664064126334, 0.01019803902718557, 0.003522782990761708, 0.0018027756377319946, 0.0016, 9.0E-4, 2.0E-4 };
    double ey_UA1_0[N_UA1_0] = { 0.57, 0.33, 0.2, 0.07, 0.05, 0.047, 0.037, 0.021, 0.014,
    0.013, 0.01, 0.0035, 0.0018, 0.0016, 9.0E-4, 2.0E-4 };
    double E_UA1_0 = 630;
    TGraphErrors *tE_DirPh_UA1_0 = new TGraphErrors(N_UA1_0, pT_UA1_0, y_UA1_0, 0, ey_UA1_0);
    TGraphErrors *tS_DirPh_UA1_0 = new TGraphErrors(N_UA1_0, pT_UA1_0, y_UA1_0, 0, sy_UA1_0);
    RemoveUpperLimits(tE_DirPh_UA1_0);
    RemoveUpperLimits(tS_DirPh_UA1_0);
//     ScaleWithPtGraph(tE_DirPh_UA1_0);
//     ScaleWithPtGraph(tS_DirPh_UA1_0);
    //     styleme(tE_DirPh_UA1_0, 27);
    TGraphErrors *tExT_DirPh_UA1_0  = xTScalePhoton(tE_DirPh_UA1_0, E_UA1_0);

    //==========================================================================================
    //                                         UA1_1
    //==========================================================================================
    // Phys.Lett. B209 (1988) 385-396
    // CERN-EP-89-138
    const int N_UA1_1 = 6;
    double pT_UA1_1[N_UA1_1] = { 17.0, 19.0, 21.0, 25.0, 34.5, 46.0 };
    double y_UA1_1[N_UA1_1] = { 3.91, 1.74, 1.12, 0.38, 0.049, 0.0084 };
    double sy_UA1_1[N_UA1_1] = { 0.5748912940721924, 0.29410882339705485, 0.21470910553583888, 0.0670820393249937, 0.013152946437965905, 0.006003332407921454 };
    double ey_UA1_1[N_UA1_1] = { 0.37, 0.24, 0.19, 0.06, 0.013, 0.006 };
    double E_UA1_1 = 546;
    TGraphErrors *tE_DirPh_UA1_1 = new TGraphErrors(N_UA1_1, pT_UA1_1, y_UA1_1, 0, ey_UA1_1);
    TGraphErrors *tS_DirPh_UA1_1 = new TGraphErrors(N_UA1_1, pT_UA1_1, y_UA1_1, 0, sy_UA1_1);
    RemoveUpperLimits(tE_DirPh_UA1_1);
    RemoveUpperLimits(tS_DirPh_UA1_1);
//     ScaleWithPtGraph(tE_DirPh_UA1_1);
//     ScaleWithPtGraph(tS_DirPh_UA1_1);
    //     styleme(tE_DirPh_UA1_1, 28);
    TGraphErrors *tExT_DirPh_UA1_1   = xTScalePhoton(tE_DirPh_UA1_1, E_UA1_1);
    //==========================================================================================
    //                                         UA2
    //==========================================================================================
    // Phys.Lett. B263 (1991) 544-550
    // ????: Phys.Lett. B288 (1992) 386-394
    const int N_UA2 = 13;
    double pT_UA2[N_UA2] = { 15.9, 17.9, 19.9, 21.9, 23.9, 25.9, 28.7, 33.5, 38.6,
    46.3, 54.1, 64.5, 82.3 };
    double y_UA2[N_UA2] = { 7.46, 3.97, 1.79, 0.992, 0.615, 0.366, 0.151, 0.0657, 0.0179,
    0.00694, 0.00231, 4.84E-4, 1.51E-4 };
    double sy_UA2[N_UA2] = { 1.4684004903295285, 0.7164091010030512, 0.337249166047894, 0.17425466995176916, 0.0937469466169432, 0.05783121994217311, 0.024233035303073365, 0.010589357865328755, 0.00403624825797423,
    0.0018672439583514522, 9.989479465918132E-4, 2.780319406111463E-4, 1.0094681768139103E-4 };
    double ey_UA2[N_UA2] = { 0.41, 0.251, 0.156, 0.0713, 0.05, 0.0362, 0.016, 0.00728, 0.00367,
    0.00171, 9.36E-4, 2.72E-4, 9.99E-5 };
    double E_UA2 = 630;
    TGraphErrors *tE_DirPh_UA2 = new TGraphErrors(N_UA2, pT_UA2, y_UA2, 0, ey_UA2);
    TGraphErrors *tS_DirPh_UA2 = new TGraphErrors(N_UA2, pT_UA2, y_UA2, 0, sy_UA2);
    RemoveUpperLimits(tE_DirPh_UA2);
    RemoveUpperLimits(tS_DirPh_UA2);
//     ScaleWithPtGraph(tE_DirPh_UA2);
//     ScaleWithPtGraph(tS_DirPh_UA2);
    //     styleme(tE_DirPh_UA2, 26);
    TGraphErrors *tExT_DirPh_UA2 = xTScalePhoton(tE_DirPh_UA2, E_UA2);

    //==========================================================================================
    //                                         PHENIX
    //==========================================================================================
    // Phys. Rev. D86, 072008 (2012)
    const int N_PHENIX = 24;
    double pT_PHENIX[N_PHENIX] = {1.18449, 1.69126, 2.19773, 2.70364, 3.34880, 4.37494, 5.25,5.75,6.25,6.75,7.25,7.75,8.25,8.75,9.25,9.75,11.00,13.00,15.00,17.00,19.00,21.00,23.00,25.00};
    double y_PHENIX[N_PHENIX] = {0.001489490e+09, 0.000541715e+09, 0.000111656e+09,  4.84535e+04, 9.89977e+03, 3.03996e+03, 1.14e+03,6.13e+02,3.48e+02,2.31e+02,1.36e+02,9.29e+01,6.70e+01,4.83e+01,3.21e+01,2.04e+01,9.81e+00,2.97e+00,1.06e+00,3.38e-01,1.73e-01,8.82e-02,4.22e-02,2.87e-02};
    double ey_PHENIX[N_PHENIX] = {0.000758668e+09, 0.000163956e+09, 3.04992e+04, 1.18865e+04, 3.26577e+03, 1.01686e+03, 3.04e+01,1.92e+01,1.27e+01,8.50e+00,6.12e+00,4.41e+00,3.22e+00,2.45e+00,1.89e+00,1.46e+00,4.23e-01,1.89e-01,9.85e-02,5.51e-02,3.37e-02,2.06e-02,1.52e-02,1.41e-02};
    double sy_PHENIX[N_PHENIX] = {0.002163150e+09, 0.000304584e+09, 5.79046e+04, 1.65357e+04, 3.86545e+03, 6.28280e+02, 4.78e+02, 2.21e+02, 1.01e+02, 6.24e+01, 3.13e+01, 1.95e+01, 1.34e+01, 9.18e+00, 6.10e+00, 3.68e+00, 1.67e+00, 4.75e-01, 1.69e-01, 5.42e-02, 2.77e-02, 1.50e-02, 7.18e-03,4.30e-03};
    double E_PHENIX = 200;

    TGraphErrors *tE_DirPh_PHENIX = new TGraphErrors(N_PHENIX, pT_PHENIX, y_PHENIX, 0, ey_PHENIX);
    TGraphErrors *tS_DirPh_PHENIX = new TGraphErrors(N_PHENIX, pT_PHENIX, y_PHENIX, 0, sy_PHENIX);
//     styleme(tE_DirPh_PHENIX, 21);
    RemoveUpperLimits(tE_DirPh_PHENIX);
    RemoveUpperLimits(tS_DirPh_PHENIX);
//     ScaleWithPtGraph(tE_DirPh_PHENIX);
//     ScaleWithPtGraph(tS_DirPh_PHENIX);
    TGraphErrors *tExT_DirPh_PHENIX = xTScalePhoton(tE_DirPh_PHENIX, E_PHENIX);
    // Phys. Rev. C 81, 034911 (2010)
    // Phys. Rev. Lett. 98, 012002 (2007)

    //==========================================================================================
    //                                         R110
    //==========================================================================================
    // Nucl. Phys B. 327, 541 (1989) - CMOR collab
    const int N_R110 = 7;
    double pT_R110[N_R110] = { 4.75, 5.25, 5.75, 6.25, 6.75, 7.5, 9.0 };
    double y_R110[N_R110] = { 2.25E-34, 1.41E-34, 6.56E-35, 3.94E-35, 1.96E-35, 7.21E-36, 1.37E-36 };
    double sy_R110[N_R110] = { 3.3E-35, 4.2E-35, 2.42E-35, 6.3E-36, 6.1E-36, 2.72E-36, 8.6E-37 };
    double ey_R110[N_R110] = { 3.3E-35, 4.2E-35, 2.42E-35, 6.3E-36, 6.1E-36, 2.72E-36, 8.6E-37 };
    double E_R110 = 63;
    //==========================================================================================
    //                                         R806
    //==========================================================================================
    // Z.Phys. C13 (1982) 277-289

    //==========================================================================================
    //                                         R807
    //==========================================================================================
    // Sov. J. Nucl. Phys. 51, 836 (1990)
    const int N_R807 = 11;
    double pT_R807[N_R807] = { 4.75, 5.25, 5.73, 6.23, 6.74, 7.23, 7.72, 8.22, 8.74,
    9.44, 10.36 };
    double y_R807[N_R807] = { 314.0, 122.0, 59.6, 32.0, 18.7, 9.07, 6.2, 3.81, 2.54,
    1.26, 0.605 };
    double sy_R807[N_R807] = { 79.0, 27.0, 12.2, 6.3, 3.5, 1.82, 1.26, 0.86, 0.63,
    0.32, 0.192 };
    double ey_R807[N_R807] = { 79.0, 27.0, 12.2, 6.3, 3.5, 1.82, 1.26, 0.86, 0.63,
    0.32, 0.192 };
    double E_R807 = 63;

    TGraphErrors *tE_DirPh_R807 = new TGraphErrors(N_R807, pT_R807, y_R807, 0, ey_R807);
    TGraphErrors *tS_DirPh_R807 = new TGraphErrors(N_R807, pT_R807, y_R807, 0, sy_R807);
//     styleme(tE_DirPh_R807, 25);
    RemoveUpperLimits(tE_DirPh_R807);
    RemoveUpperLimits(tS_DirPh_R807);
    TGraphErrors *tExT_DirPh_R807 = xTScalePhoton(tE_DirPh_R807, E_R807);
    //==========================================================================================
    //                                         R108
    //==========================================================================================
    // Phys.Lett. 94B (1980) 106-112
    const int N_R108 = 8;
    double pT_R108[N_R108] = { 5.4, 6.4, 7.4, 8.4, 9.4, 10.4, 11.4, 12.4 };
    double y_R108[N_R108] = { 1.3E-34, 1.5E-35, 4.3E-36, 1.9E-36, 5.3E-37, 4.5E-37, 2.4E-37, 1.1E-37 };
    double ey_R108[N_R108] = { 8.0E-35, 1.0E-35, 1.1E-36, 5.0E-37, 1.6E-37, 1.0E-37, 6.0E-38, 4.0E-38 };
    double sy_R108[N_R108] = { 8.0E-35, 1.0E-35, 1.1E-36, 5.0E-37, 1.6E-37, 1.0E-37, 6.0E-38, 4.0E-38 };
    double E_R108 = 62.4;
    TGraphErrors *tE_DirPh_R108 = new TGraphErrors(N_R108, pT_R108, y_R108, 0, ey_R108);
    TGraphErrors *tS_DirPh_R108 = new TGraphErrors(N_R108, pT_R108, y_R108, 0, sy_R108);
    tE_DirPh_R108               = ScaleGraph(tE_DirPh_R108, 1e36);
    tS_DirPh_R108               = ScaleGraph(tS_DirPh_R108, 1e36);
    RemoveUpperLimits(tE_DirPh_R108);
    RemoveUpperLimits(tS_DirPh_R108);
    //     styleme(tE_DirPh_R108, 24);
    TGraphErrors *tExT_DirPh_R108 = xTScalePhoton(tE_DirPh_R108, E_R108);
    //==========================================================================================
    //                                         E706
    //==========================================================================================
    // Phys.Rev. D72 (2005) 032003
    // Phys.Rev. D70 (2004) 092009
    // Phys.Rev. D48 (1993) 5-28
    const int N_E706 = 9;
    double pT_E706[N_E706] = { 3.75, 4.25, 4.75, 5.25, 5.75, 6.5, 7.5, 9.0, 11.0 };
    double y_E706[N_E706] = { 1880.0, 600.0, 225.5, 82.2, 35.0, 9.8, 1.73, 0.339, 0.017 };
    double sy_E706[N_E706] = { 484.1487374764082, 109.2520022699813, 35.27548723972498, 11.84736257569591, 5.011985634456668, 1.4098226838861687, 0.3440930106817051, 0.08013114251026252, 0.01414213562373095 };
    double ey_E706[N_E706] = { 300.0, 44.0, 9.4, 4.4, 2.4, 0.74, 0.28, 0.07, 0.014 };
    double E_E706 = 38.8;
    TGraphErrors *tE_DirPh_E706 = new TGraphErrors(N_E706, pT_E706, y_E706, 0, ey_E706);
    TGraphErrors *tS_DirPh_E706 = new TGraphErrors(N_E706, pT_E706, y_E706, 0, sy_E706);
    RemoveUpperLimits(tE_DirPh_E706);
    RemoveUpperLimits(tS_DirPh_E706);
    //     styleme(tE_DirPh_E706, 32);
    TGraphErrors *tExT_DirPh_E706 = xTScalePhoton(tE_DirPh_E706, E_E706);
    const int N_E706_0 = 8;
    double pT_E706_0[N_E706_0] = { 3.75, 4.25, 4.75, 5.25, 5.75, 6.5, 7.5, 9.0 };
    double y_E706_0[N_E706_0] = { 1160.0, 375.0, 107.6, 33.6, 11.4, 3.43, 0.42, 0.01 };
    double sy_E706_0[N_E706_0] = { 290.68883707497264, 68.87670143089025, 16.62077013859466, 4.919349550499538, 1.7804493814764855, 0.5597320787662612, 0.12083045973594572, 0.015033296378372907 };
    double ey_E706_0[N_E706_0] = { 190.0, 30.0, 4.5, 2.2, 1.1, 0.37, 0.11, 0.015 };
    double E_E706_0 = 31.6;
    TGraphErrors *tE_DirPh_E706_0 = new TGraphErrors(N_E706_0, pT_E706_0, y_E706_0, 0, ey_E706_0);
    TGraphErrors *tS_DirPh_E706_0 = new TGraphErrors(N_E706_0, pT_E706_0, y_E706_0, 0, sy_E706_0);
    RemoveUpperLimits(tE_DirPh_E706_0);
    RemoveUpperLimits(tS_DirPh_E706_0);
    //     styleme(tE_DirPh_E706_0, 31);
    TGraphErrors *tExT_DirPh_E706_0 = xTScalePhoton(tE_DirPh_E706_0, E_E706_0);
    //==========================================================================================
    //                                         E704
    //==========================================================================================
    // Phys.Lett. B345 (1995) 569-575
    const int N_E704 = 5;
    double pT_E704[N_E704] = { 2.59, 2.79, 2.99, 3.24, 3.59 };
    double y_E704[N_E704] = { 4.9, 1.58, 1.15, 0.539, 0.312 };
    double sy_E704[N_E704] = { 0.7824321056807421, 0.45486261662176636, 0.29966648127543394, 0.17053152201279387, 0.09492101980067429 };
    double ey_E704[N_E704] = { 0.61, 0.38, 0.27, 0.16, 0.091 };
    double E_E704 = 19.4;
    TGraphErrors *tE_DirPh_E704 = new TGraphErrors(N_E704, pT_E704, y_E704, 0, ey_E704);
    TGraphErrors *tS_DirPh_E704 = new TGraphErrors(N_E704, pT_E704, y_E704, 0, sy_E704);
    tE_DirPh_E704               = ScaleGraph(tE_DirPh_E704, 1000);
    tS_DirPh_E704               = ScaleGraph(tS_DirPh_E704, 1000);
    RemoveUpperLimits(tE_DirPh_E704);
    RemoveUpperLimits(tS_DirPh_E704);
    TGraphErrors *tExT_DirPh_E704 = xTScalePhoton(tE_DirPh_E704, E_E704);
    tExT_DirPh_E704->Print();
    //==========================================================================================
    //                                         NA24
    //==========================================================================================
    // Phys.Rev. D36 (1987) 8
    const int N_NA24 = 5;
    double pT_NA24[N_NA24] = { 3.25, 3.75, 4.25, 5.0, 6.0 };
    double y_NA24[N_NA24] = { 3.75E-34, 1.21E-34, 2.5E-35, 5.48E-36, 9.5E-37 };
    double sy_NA24[N_NA24] = { 1.9377564346429095E-34, 4.860041152089147E-35, 8.499999999999999E-36, 1.6278820596099707E-36, 3.9458839313897716E-37 };
    double ey_NA24[N_NA24] = { 9.3E-35, 3.9E-35, 4.0E-36, 1.2E-36, 3.9E-37 };
    double E_NA24 = 23.8;
    TGraphErrors *tE_DirPh_NA24 = new TGraphErrors(N_NA24, pT_NA24, y_NA24, 0, ey_NA24);
    TGraphErrors *tS_DirPh_NA24 = new TGraphErrors(N_NA24, pT_NA24, y_NA24, 0, sy_NA24);
    tE_DirPh_NA24               = ScaleGraph(tE_DirPh_NA24, 1e36);
    tS_DirPh_NA24               = ScaleGraph(tS_DirPh_NA24, 1e36);
    RemoveUpperLimits(tE_DirPh_NA24);
    RemoveUpperLimits(tS_DirPh_NA24);
    //     styleme(tE_DirPh_NA24, 21, kGray);
    TGraphErrors *tExT_DirPh_NA24 = xTScalePhoton(tE_DirPh_NA24, E_NA24);

    //==========================================================================================
    //                                         WA70
    //==========================================================================================
    // Z.Phys. C38 (1988) 371
    const int N_WA70 = 5;
    double pT_WA70[N_WA70] = { 4.11, 4.36, 4.7, 5.2, 5.7 };
    double y_WA70[N_WA70] = { 32.6, 24.0, 10.5, 3.92, 0.68 };
    double sy_WA70[N_WA70] = { 8.5,  5.7 , 2.1, 0.94, 0.22 };
    double ey_WA70[N_WA70] = { 3.9, 3.3, 1.4, 0.87, 0.32 };
    double E_WA70 = 23.0;
    TGraphErrors *tE_DirPh_WA70 = new TGraphErrors(N_WA70, pT_WA70, y_WA70, 0, ey_WA70);
    TGraphErrors *tS_DirPh_WA70 = new TGraphErrors(N_WA70, pT_WA70, y_WA70, 0, sy_WA70);
    tE_DirPh_WA70               = ScaleGraph(tE_DirPh_WA70, 1e0);
    tS_DirPh_WA70               = ScaleGraph(tS_DirPh_WA70, 1e0);
    RemoveUpperLimits(tE_DirPh_WA70);
    RemoveUpperLimits(tS_DirPh_WA70);

    //     styleme(tE_DirPh_WA70, 22, kGray);
    TGraphErrors *tExT_DirPh_WA70 = xTScalePhoton(tE_DirPh_WA70, E_WA70);

    //==========================================================================================
    //                                         UA6_0
    //==========================================================================================
    // Phys.Lett. B317 (1993) 243-249
    const int N_UA6_0 = 10;
    double pT_UA6_0[N_UA6_0] = { 4.19, 4.39, 4.59, 4.79, 4.99, 5.19, 5.46, 5.89, 6.32,
    7.07 };
    double y_UA6_0[N_UA6_0] = { 56.3, 40.3, 24.1, 16.7, 7.7, 5.2, 2.37, 0.76, 0.44,
    0.0 };
    double ey_UA6_0[N_UA6_0] = { 4.9, 3.7, 2.7, 2.2, 1.5, 1.2, 0.53, 0.29, 0.15,
    0.0 };
    double sy_UA6_0[N_UA6_0] = { 4.9, 3.7, 2.7, 2.2, 1.5, 1.2, 0.53, 0.29, 0.15,
    0.03 };
    double E_UA6_0 = 24.3;
    TGraphErrors *tE_DirPh_UA6_0 = new TGraphErrors(N_UA6_0, pT_UA6_0, y_UA6_0, 0, ey_UA6_0);
    TGraphErrors *tS_DirPh_UA6_0 = new TGraphErrors(N_UA6_0, pT_UA6_0, y_UA6_0, 0, sy_UA6_0);
    RemoveUpperLimits(tS_DirPh_UA6_0);
    RemoveUpperLimits(tE_DirPh_UA6_0);
    //     styleme(tE_DirPh_UA6_0, 33);
    TGraphErrors *tExT_DirPh_UA6_0 = xTScalePhoton(tE_DirPh_UA6_0, E_UA6_0);

    //==========================================================================================
    //                                         UA6_1
    //==========================================================================================
    // Phys.Lett. B436 (1998) 222-230
    const int N_UA6_1 = 6;
    double pT_UA6_1[N_UA6_1] = { 4.199999999999999, 4.4, 4.6, 4.800000000000001, 5.1, 5.699999999999999 };
    double y_UA6_1[N_UA6_1] = { 119.1, 66.6, 44.6, 26.8, 7.7, 5.4 };
    double sy_UA6_1[N_UA6_1] = { 28.207268566807386, 18.11104635298579, 12.70944530654269, 9.033825324855467, 3.6999999999999997, 1.6124515496597098 };
    double ey_UA6_1[N_UA6_1] = { 21.8, 15.1, 10.8, 8.1, 3.5, 1.4 };
    double E_UA6_1 = 24.3;

    TGraphErrors *tE_DirPh_UA6_1 = new TGraphErrors(N_UA6_1, pT_UA6_1, y_UA6_1, 0, ey_UA6_1);
    TGraphErrors *tS_DirPh_UA6_1 = new TGraphErrors(N_UA6_1, pT_UA6_1, y_UA6_1, 0, sy_UA6_1);
    RemoveUpperLimits(tS_DirPh_UA6_1);
    RemoveUpperLimits(tE_DirPh_UA6_1);
    //     styleme(tE_DirPh_UA6_1, 34);
    TGraphErrors *tExT_DirPh_UA6_1 = xTScalePhoton(tE_DirPh_UA6_1, E_UA6_1);


    //==========================================================================================
    //                          pi0 measurements
    //==========================================================================================
    
    
    TFile* filePi0200GeVPHENIX                                    = new TFile("ExternalInput/OtherExperiments/PHENIX_pp_200GeV_pi0Xsection_Run5.root");
    TGraphAsymmErrors* pi0Phenix200GeVGraphStat                   = (TGraphAsymmErrors*)filePi0200GeVPHENIX->Get("Run5pp");
    TGraphAsymmErrors* pi0Phenix200GeVGraphSys                    = (TGraphAsymmErrors*)filePi0200GeVPHENIX->Get("Run5ppSys");
    pi0Phenix200GeVGraphStat->SetName("pi0PHENIX_200GeV_Stat");
    pi0Phenix200GeVGraphSys->SetName("pi0PHENIX_200GeV_Sys");
//     https://www.phenix.bnl.gov/phenix/WWW/info/data/ppg063_data.html
//     https://www.phenix.bnl.gov/phenix/WWW/info/data/ppg107_data.html
//     @article{Adare:2010cy,
//     author = "Adare, A. and others",
//     collaboration = "PHENIX",
//     title = "{Cross section and double helicity asymmetry for $\eta$ mesons and their comparison to neutral pion production in p+p collisions at $sqrt(s)=200 GeV$}",
//     eprint = "1009.6224",
//     archivePrefix = "arXiv",
//     primaryClass = "hep-ex",
//     doi = "10.1103/PhysRevD.83.032001",
//     journal = "Phys. Rev. D",
//     volume = "83",
//     pages = "032001",
//     year = "2011"
// } 
// @article{Adare:2007dg,
//     author = "Adare, A. and others",
//     collaboration = "PHENIX",
//     title = "{Inclusive cross-section and double helicity asymmetry for $\pi^0$ production in $p + p$ collisions at $\sqrt{s} =$ 200 GeV: Implications for the polarized gluon distribution in the proton}",
//     eprint = "0704.3599",
//     archivePrefix = "arXiv",
//     primaryClass = "hep-ex",
//     doi = "10.1103/PhysRevD.76.051106",
//     journal = "Phys. Rev. D",
//     volume = "76",
//     pages = "051106",
//     year = "2007"
// }
//     @article{Adler:2003pb,
//     author = "Adler, S.S. and others",
//     collaboration = "PHENIX",
//     title = "{Mid-rapidity neutral pion production in proton proton collisions at $\sqrt{s}$ = 200-GeV}",
//     eprint = "hep-ex/0304038",
//     archivePrefix = "arXiv",
//     doi = "10.1103/PhysRevLett.91.241803",
//     journal = "Phys. Rev. Lett.",
//     volume = "91",
//     pages = "241803",
//     year = "2003"
// }
    
    
//     WA98 p-C nColl = 1.7+-0.2
//     https://arxiv.org/pdf/0708.2630.pdf, https://arxiv.org/abs/1108.5400
//     @article{Albrecht:1988ra,
//     author = "Albrecht, R. and others",
//     collaboration = "WA80",
//     title = "{Photon and Neutral Pion Distributions in 60-agev and 200-agev $^{16}$O + Nucleus and Proton + Nucleus Reactions}",
//     doi = "10.1016/0370-2693(88)91161-6",
//     journal = "Phys. Lett. B",
//     volume = "201",
//     pages = "390--396",
//     year = "1988"
// }
//     @article{Aggarwal:2011ns,
//     author = "Aggarwal, M.M. and others",
//     collaboration = "WA98",
//     title = "{Photon and Eta Production in p+Pb and p+C Collisions at $\sqrt{s_{NN}} = 17.4$ GeV}",
//     eprint = "1108.5400",
//     archivePrefix = "arXiv",
//     primaryClass = "nucl-ex",
//     doi = "10.1016/j.nuclphysa.2012.11.010",
//     journal = "Nucl. Phys. A",
//     volume = "898",
//     pages = "14--23",
//     year = "2013"
// }
    
//     STAR 200 GeV
//     https://arxiv.org/pdf/0912.3838.pdf
    
//      PHENIX 510 GeV
//     https://www.phenix.bnl.gov/phenix/WWW/info/data/ppg186_data.html
//     @article{Adare:2015ozj,
//     author = "Adare, A. and others",
//     collaboration = "PHENIX",
//     title = "{Inclusive cross section and double-helicity asymmetry for $\pi^{0}$ production at midrapidity in $p$$+$$p$ collisions at $\sqrt{s}=510$ GeV}",
//     eprint = "1510.02317",
//     archivePrefix = "arXiv",
//     primaryClass = "hep-ex",
//     doi = "10.1103/PhysRevD.93.011501",
//     journal = "Phys. Rev. D",
//     volume = "93",
//     number = "1",
//     pages = "011501",
//     year = "2016"
// }

// PHENIX 62.4GeV
// https://www.phenix.bnl.gov/phenix/WWW/info/data/ppg087/data_table_62.txt
//     @article{Adare:2008qb,
//     author = "Adare, A. and others",
//     collaboration = "PHENIX",
//     title = "{Inclusive cross section and double helicity asymmetry for pi^0 production in $p^+ p$ collisions at $\sqrt{s}=62.4$ GeV}",
//     eprint = "0810.0701",
//     archivePrefix = "arXiv",
//     primaryClass = "hep-ex",
//     doi = "10.1103/PhysRevD.79.012003",
//     journal = "Phys. Rev. D",
//     volume = "79",
//     pages = "012003",
//     year = "2009"
// }
    
    // n measurements 200/62.4 & 200/39
    // https://www.phenix.bnl.gov/phenix/WWW/info/data/ppg138_data.html
//     @article{Adare:2012uk,
//     author = "Adare, A. and others",
//     collaboration = "PHENIX",
//     title = "{Evolution of $\pi^0$ suppression in Au+Au collisions from $\sqrt{s_{NN}} = 39$ to 200 GeV}",
//     eprint = "1204.1526",
//     archivePrefix = "arXiv",
//     primaryClass = "nucl-ex",
//     doi = "10.1103/PhysRevLett.109.152301",
//     journal = "Phys. Rev. Lett.",
//     volume = "109",
//     pages = "152301",
//     year = "2012"
// }
    
    //==========================================================================================
    //                          eta meson measurements
    //==========================================================================================
    // 200 GeV PHENIX
//     https://www.phenix.bnl.gov/phenix/WWW/info/data/ppg107_data.html
//     @article{Adare:2010cy,
//     author = "Adare, A. and others",
//     collaboration = "PHENIX",
//     title = "{Cross section and double helicity asymmetry for $\eta$ mesons and their comparison to neutral pion production in p+p collisions at $sqrt(s)=200 GeV$}",
//     eprint = "1009.6224",
//     archivePrefix = "arXiv",
//     primaryClass = "hep-ex",
//     doi = "10.1103/PhysRevD.83.032001",
//     journal = "Phys. Rev. D",
//     volume = "83",
//     pages = "032001",
//     year = "2011"
// }
    
    //==========================================================================================
    //                          eta/ pi0 measurements
    //==========================================================================================
    

    Double_t pt[400];
    Double_t value[400];
    Double_t totErr[400];
    Double_t xErr[400];
    Double_t statErr[400];
    Double_t sysErr[400];
    Double_t sysErrB[400];
    Double_t a = 0;
    Double_t b = 0;
    Double_t c = 0;
    Double_t d = 0;

    // Create 1 graph containing all point above 4 GeV
    vector<Double_t> *valuesEtaToPi0            = new vector<Double_t>[4];     // iCent x iParticle x nMeasurements matrix with theory curves
    Int_t currentNumberOfPoint                  = 0;
    vector<Double_t> *valuesEtaToPi0ALICE       = new vector<Double_t>[4];     // iCent x iParticle x nMeasurements matrix with theory curves
    Int_t currentNumberOfPointALICE             = 0;

    //Donaldson100GeV
//         @article{Donaldson:1977jt,
//             author         = "Donaldson, G. J. and others",
//             title          = "{Inclusive eta Production at Large Transverse Momenta}",
//             journal        = "Phys. Rev. Lett.",
//             volume         = "40",
//             year           = "1978",
//             pages          = "684",
//             doi            = "10.1103/PhysRevLett.40.684",
//             reportNumber   = "BNL-23702",
//             SLACcitation   = "%%CITATION = PRLTA,40,684;%%"
//         }

    ifstream Donaldson100GeV;
    Donaldson100GeV.open("ExternalInput/OtherExperiments/donaldson78_pp_plab100GeV_eta_pi0_ratio.txt");
    cout << "donaldson78_pp_plab100GeV_eta_pi0_ratio.txt" << endl;
    Int_t lines = 0;

    while(!Donaldson100GeV.eof()){
        Donaldson100GeV >> a>> b >> value[lines] >> totErr[lines] >> c >> d;
        pt[lines] = (a+b)/2;
        xErr[lines] = 0.;
        lines++;
    }
    Donaldson100GeV.close();

    TGraph *Donaldson100GeVGraph = new TGraphErrors(lines-1,pt,value,xErr,totErr);
    Donaldson100GeVGraph->SetName("donaldson100GeV");
    Donaldson100GeVGraph->SetTitle("p+p (#sqrt{#it{s}}= 13.8 GeV)");
    Donaldson100GeVGraph->Print();
    for (Int_t i= 0; i< Donaldson100GeVGraph->GetN(); i++){
        if (Donaldson100GeVGraph->GetX()[i] > 4){
            valuesEtaToPi0[0].push_back(Donaldson100GeVGraph->GetX()[i]);
            valuesEtaToPi0[1].push_back(Donaldson100GeVGraph->GetY()[i]);
            valuesEtaToPi0[2].push_back(Donaldson100GeVGraph->GetEY()[i]);
            valuesEtaToPi0[3].push_back(Donaldson100GeVGraph->GetEY()[i]);
            currentNumberOfPoint++;
        }
    }
    cout << "Number of pT bins: "<< currentNumberOfPoint <<  endl;

    //Donaldson200GeV
    ifstream Donaldson200GeV;
    Donaldson200GeV.open("ExternalInput/OtherExperiments/donaldson78_pp_plab200GeV_eta_pi0_ratio.txt");
    cout << "donaldson78_pp_plab200GeV_eta_pi0_ratio.txt" << endl;
    lines = 0;

    while(!Donaldson200GeV.eof()){
        Donaldson200GeV >> a>> b >> value[lines] >> totErr[lines] >> c >> d;
        pt[lines] = (a+b)/2;
        xErr[lines] = 0.;
        lines++;
    }
    Donaldson200GeV.close();
    TGraph *Donaldson200GeVGraph = new TGraphErrors(lines-1,pt,value,xErr,totErr);
    Donaldson200GeVGraph->SetName("donaldson200GeV");
    Donaldson200GeVGraph->SetTitle("p+p (#sqrt{#it{s}}= 19.4 GeV)");
    Donaldson200GeVGraph->Print();
    for (Int_t i= 0; i< Donaldson200GeVGraph->GetN(); i++){
        if (Donaldson200GeVGraph->GetX()[i] > 4){
            valuesEtaToPi0[0].push_back(Donaldson200GeVGraph->GetX()[i]);
            valuesEtaToPi0[1].push_back(Donaldson200GeVGraph->GetY()[i]);
            valuesEtaToPi0[2].push_back(Donaldson200GeVGraph->GetEY()[i]);
            valuesEtaToPi0[3].push_back(Donaldson200GeVGraph->GetEY()[i]);
            currentNumberOfPoint++;
        }
    }
    cout << "Number of pT bins: "<< currentNumberOfPoint <<  endl;


    // 		"bonesi89_pp_plab280GeV_wa70_eta_pi0_ratio.txt","p+p (#sqrt{#it{s}}= 23 GeV)",
//     @article{Bonesini:1988rd,
//         author         = "Bonesini, M. and others",
//         title          = "{High Transverse Momentum $\eta$ Production in $\pi^- p$,
//         $\pi^+ p$ and $p p$ Interactions at 280-{GeV}/$c$}",
//         collaboration  = "WA70",
//         journal        = "Z. Phys.",
//         volume         = "C42",
//         year           = "1989",
//         pages          = "527",
//         doi            = "10.1007/BF01557657",
//         reportNumber   = "CERN-EP/88-178",
//         SLACcitation   = "%%CITATION = ZEPYA,C42,527;%%"
//     }
    ifstream Bonesi280GeV;
    Bonesi280GeV.open("ExternalInput/OtherExperiments/bonesi89_pp_plab280GeV_wa70_eta_pi0_ratio.txt");
    cout << "bonesi89_pp_plab280GeV_wa70_eta_pi0_ratio.txt" << endl;
    lines = 0;

    while(!Bonesi280GeV.eof()){
        Bonesi280GeV >> pt[lines] >> value[lines] >> totErr[lines];
        xErr[lines] = 0.;
        lines++;
    }
    Bonesi280GeV.close();
    TGraph *Bonesi280GeVGraph = new TGraphErrors(lines-1,pt,value,xErr,totErr);
    Bonesi280GeVGraph->SetName("Bonesi280GeV");
    Bonesi280GeVGraph->SetTitle("p+p (#sqrt{#it{s}}= 23 GeV)");
    Bonesi280GeVGraph->Print();
    for (Int_t i= 0; i< Bonesi280GeVGraph->GetN(); i++){
        if (Bonesi280GeVGraph->GetX()[i] > 4){
            valuesEtaToPi0[0].push_back(Bonesi280GeVGraph->GetX()[i]);
            valuesEtaToPi0[1].push_back(Bonesi280GeVGraph->GetY()[i]);
            valuesEtaToPi0[2].push_back(Bonesi280GeVGraph->GetEY()[i]);
            valuesEtaToPi0[3].push_back(Bonesi280GeVGraph->GetEY()[i]);
            currentNumberOfPoint++;
        }
    }
    cout << "Number of pT bins: "<< currentNumberOfPoint <<  endl;


// 		"antille87_pp_24.3GeV_eta_pi0_ratio.txt","p+p (#sqrt{#it{s}}= 24.3 GeV)",
//     @article{Antille:1987kr,
//         author         = "Antille, J. and others",
//         title          = "{A Measurement of the Inclusive $\pi^0$ and $\eta$
//         Production Cross-sections at High $p_T$ in $p \bar{p}$ and
//         $p p$ Collisions at $\sqrt{s}=24$.3-{GeV}}",
//         collaboration  = "CERN-Lausanne-Michigan-Rockefeller, UA6",
//         journal        = "Phys. Lett.",
//         volume         = "B194",
//         year           = "1987",
//         pages          = "568",
//         doi            = "10.1016/0370-2693(87)90236-X",
//         reportNumber   = "CERN-EP-87-121",
//         SLACcitation   = "%%CITATION = PHLTA,B194,568;%%"
//     }

    ifstream Antille87pp;
    Antille87pp.open("ExternalInput/OtherExperiments/antille87_pp_24.3GeV_eta_pi0_ratio.txt");
    cout << "antille87_pp_24.3GeV_eta_pi0_ratio.txt" << endl;
    lines = 0;

    while(!Antille87pp.eof()){
        Antille87pp >> pt[lines] >> value[lines] >> totErr[lines];
        xErr[lines] = 0.;
        lines++;
    }
    Antille87pp.close();
    TGraph *Antille87ppGraph = new TGraphErrors(lines-1,pt,value,xErr,totErr);
    Antille87ppGraph->SetName("Antille24.3GeVpp");
    Antille87ppGraph->SetTitle("p+p (#sqrt{#it{s}}= 24.3 GeV)");
    Antille87ppGraph->Print();
    for (Int_t i= 0; i< Antille87ppGraph->GetN(); i++){
        if (Antille87ppGraph->GetX()[i] > 4){
            valuesEtaToPi0[0].push_back(Antille87ppGraph->GetX()[i]);
            valuesEtaToPi0[1].push_back(Antille87ppGraph->GetY()[i]);
            valuesEtaToPi0[2].push_back(Antille87ppGraph->GetEY()[i]);
            valuesEtaToPi0[3].push_back(Antille87ppGraph->GetEY()[i]);
            currentNumberOfPoint++;
        }
    }
    cout << "Number of pT bins: "<< currentNumberOfPoint <<  endl;

// 		"antille87_ppbar_24.3GeV_eta_pi0_ratio.txt","#bar{p}+p (#sqrt{#it{s}}= 24.3 GeV)",
    ifstream Antille87ppbar;
    Antille87ppbar.open("ExternalInput/OtherExperiments/antille87_ppbar_24.3GeV_eta_pi0_ratio.txt");
    cout << "antille87_ppbar_24.3GeV_eta_pi0_ratio.txt" << endl;
    lines = 0;

    while(!Antille87ppbar.eof()){
        Antille87ppbar >> pt[lines] >> value[lines] >> totErr[lines];
        xErr[lines] = 0.;
        lines++;
    }
    Antille87ppbar.close();
    TGraph *Antille87ppbarGraph = new TGraphErrors(lines-1,pt,value,xErr,totErr);
    Antille87ppbarGraph->SetName("Antille24.3GeVppbar");
    Antille87ppbarGraph->SetTitle("#bar{p}+p (#sqrt{#it{s}}= 24.3 GeV)");
    Antille87ppbarGraph->Print();
    for (Int_t i= 0; i< Antille87ppbarGraph->GetN(); i++){
        if (Antille87ppbarGraph->GetX()[i] > 4){
            valuesEtaToPi0[0].push_back(Antille87ppbarGraph->GetX()[i]);
            valuesEtaToPi0[1].push_back(Antille87ppbarGraph->GetY()[i]);
            valuesEtaToPi0[2].push_back(Antille87ppbarGraph->GetEY()[i]);
            valuesEtaToPi0[3].push_back(Antille87ppbarGraph->GetEY()[i]);
            currentNumberOfPoint++;
        }
    }
    cout << "Number of pT bins: "<< currentNumberOfPoint <<  endl;

    // 		"aguilar91_pp_plab400GeV_na27_eta_pi0_ratio.txt","p+p (#sqrt{#it{s}}= 27.5 GeV)",
//     @article{AguilarBenitez:1991yy,
//         author         = "Aguilar-Benitez, M. and others",
//         title          = "{Inclusive particle production in 400-GeV/c p p
//         interactions}",
//         journal        = "Z. Phys.",
//         volume         = "C50",
//         year           = "1991",
//         pages          = "405-426",
//         doi            = "10.1007/BF01551452",
//         reportNumber   = "CERN-PPE-91-21, EXP-CERN-NA27",
//         SLACcitation   = "%%CITATION = ZEPYA,C50,405;%%"
//     }
    ifstream Aguilar400GeV;
    Aguilar400GeV.open("ExternalInput/OtherExperiments/aguilar91_pp_plab400GeV_na27_eta_pi0_ratio.txt");
    cout << "aguilar91_pp_plab400GeV_na27_eta_pi0_ratio.txt" << endl;
    lines = 0;

    while(!Aguilar400GeV.eof()){
        Aguilar400GeV >> pt[lines] >> value[lines] >> totErr[lines];
        xErr[lines] = 0.;
        lines++;
    }
    Aguilar400GeV.close();
    TGraph *Aguilar400GeVGraph = new TGraphErrors(lines-1,pt,value,xErr,totErr);
    Aguilar400GeVGraph->SetName("Aguilar400GeV");
    Aguilar400GeVGraph->SetTitle("p+p (#sqrt{#it{s}}= 27.5 GeV)");
    Aguilar400GeVGraph->Print();
    for (Int_t i= 0; i< Aguilar400GeVGraph->GetN(); i++){
        if (Aguilar400GeVGraph->GetX()[i] > 4){
            valuesEtaToPi0[0].push_back(Aguilar400GeVGraph->GetX()[i]);
            valuesEtaToPi0[1].push_back(Aguilar400GeVGraph->GetY()[i]);
            valuesEtaToPi0[2].push_back(Aguilar400GeVGraph->GetEY()[i]);
            valuesEtaToPi0[3].push_back(Aguilar400GeVGraph->GetEY()[i]);
            currentNumberOfPoint++;
        }
    }
    cout << "Number of pT bins: "<< currentNumberOfPoint <<  endl;

// 		"amaldi79_pp_30.6GeV_isr_eta_pi0_ratio.txt","p+p (#sqrt{#it{s}}= 30.6 GeV)",
//     @article{Amaldi:1979zk,
//         author         = "Amaldi, E. and others",
//         title          = "{INCLUSIVE ETA PRODUCTION IN P P COLLISION AT ISR
//         ENERGIES}",
//         journal        = "Nucl. Phys.",
//         volume         = "B158",
//         year           = "1979",
//         pages          = "1-10",
//         doi            = "10.1016/0550-3213(79)90183-4",
//         SLACcitation   = "%%CITATION = NUPHA,B158,1;%%"
//     }
    ifstream Amaldi79pp;
    Amaldi79pp.open("ExternalInput/OtherExperiments/amaldi79_pp_30.6GeV_isr_eta_pi0_ratio.txt");
    cout << "amaldi79_pp_30.6GeV_isr_eta_pi0_ratio.txt" << endl;
    lines = 0;

    while(!Amaldi79pp.eof()){
        Amaldi79pp >> pt[lines] >> value[lines] >> totErr[lines];
        xErr[lines] = 0.;
        lines++;
    }
    Amaldi79pp.close();
    TGraph *Amaldi79ppGraph = new TGraphErrors(lines-1,pt,value,xErr,totErr);
    Amaldi79ppGraph->SetName("Amaldi30.6GeVpp");
    Amaldi79ppGraph->SetTitle("p+p (#sqrt{#it{s}}= 30.6 GeV)");
    Amaldi79ppGraph->Print();
    for (Int_t i= 0; i< Amaldi79ppGraph->GetN(); i++){
        if (Amaldi79ppGraph->GetX()[i] > 4){
            valuesEtaToPi0[0].push_back(Amaldi79ppGraph->GetX()[i]);
            valuesEtaToPi0[1].push_back(Amaldi79ppGraph->GetY()[i]);
            valuesEtaToPi0[2].push_back(Amaldi79ppGraph->GetEY()[i]);
            valuesEtaToPi0[3].push_back(Amaldi79ppGraph->GetEY()[i]);
            currentNumberOfPoint++;
        }
    }
    cout << "Number of pT bins: "<< currentNumberOfPoint <<  endl;


// 		"kourkou79_pp_30.6GeV_eta_pi0_ratio.txt","p+p (#sqrt{#it{s}}= 30.6 GeV)",

//     @article{Kourkoumelis:1979tr,
//         author         = "Kourkoumelis, C. and others",
//         title          = "{Inclusive $\pi^0$ Production at Very Large $p_T$ at the
//         {ISR}}",
//         journal        = "Phys. Lett.",
//         volume         = "84B",
//         year           = "1979",
//         pages          = "271-276",
//         doi            = "10.1016/0370-2693(79)90301-0",
//         reportNumber   = "CERN-EP-79-29, BNL-26126",
//         SLACcitation   = "%%CITATION = PHLTA,84B,271;%%"
//     }
//     @article{Kourkoumelis:1979ts,
//         author         = "Kourkoumelis, C. and others",
//         title          = "{Inclusive $\eta$ Production at High $p_T$ at the {ISR}}",
//         journal        = "Phys. Lett.",
//         volume         = "84B",
//         year           = "1979",
//         pages          = "277-280",
//         doi            = "10.1016/0370-2693(79)90302-2",
//         reportNumber   = "CERN-EP-79-30",
//         SLACcitation   = "%%CITATION = PHLTA,84B,277;%%"
//     }
    ifstream Kourkou79pp;
    Kourkou79pp.open("ExternalInput/OtherExperiments/kourkou79_pp_30.6GeV_eta_pi0_ratio.txt");
    cout << "kourkou79_pp_30.6GeV_eta_pi0_ratio.txt" << endl;
    lines = 0;

    while(!Kourkou79pp.eof()){
        Kourkou79pp >> pt[lines] >> value[lines] >> totErr[lines];
        xErr[lines] = 0.;
        lines++;
    }
    Kourkou79pp.close();
    TGraph *Kourkou79ppGraph = new TGraphErrors(lines-1,pt,value,xErr,totErr);
    Kourkou79ppGraph->SetName("Kourkou30.6GeVpp");
    Kourkou79ppGraph->SetTitle("p+p (#sqrt{#it{s}}= 30.6 GeV)");
    Kourkou79ppGraph->Print();
    for (Int_t i= 0; i< Kourkou79ppGraph->GetN(); i++){
        if (Kourkou79ppGraph->GetX()[i] > 4){
            valuesEtaToPi0[0].push_back(Kourkou79ppGraph->GetX()[i]);
            valuesEtaToPi0[1].push_back(Kourkou79ppGraph->GetY()[i]);
            valuesEtaToPi0[2].push_back(Kourkou79ppGraph->GetEY()[i]);
            valuesEtaToPi0[3].push_back(Kourkou79ppGraph->GetEY()[i]);
            currentNumberOfPoint++;
        }
    }
    cout << "Number of pT bins: "<< currentNumberOfPoint <<  endl;


// 		"apana02_pp_plab530GeV_fnal706_eta_pi0_ratio.txt","p+p (#sqrt{#it{s}}= 31.6 GeV)",
    ifstream Apana530GeV;
    Apana530GeV.open("ExternalInput/OtherExperiments/apana02_pp_plab530GeV_fnal706_eta_pi0_ratio.txt");
    cout << "apana02_pp_plab530GeV_fnal706_eta_pi0_ratio.txt" << endl;
    lines = 0;
//     @article{Apanasevich:2002wt,
//         author         = "Apanasevich, L. and others",
//         title          = "{Production of pi0 and eta mesons at large transverse
//         momenta in pp and p Be interactions at 530-GeV/c and
//         800-GeV/c}",
//         collaboration  = "Fermilab E706",
//         journal        = "Phys. Rev.",
//         volume         = "D68",
//         year           = "2003",
//         pages          = "052001",
//         doi            = "10.1103/PhysRevD.68.052001",
//         eprint         = "hep-ex/0204031",
//         archivePrefix  = "arXiv",
//         primaryClass   = "hep-ex",
//         reportNumber   = "FERMILAB-PUB-01-021-E",
//         SLACcitation   = "%%CITATION = HEP-EX/0204031;%%"
//     }
    while(!Apana530GeV.eof()){
        Apana530GeV >> pt[lines] >> value[lines] >> totErr[lines];
        xErr[lines] = 0.;
        lines++;
    }
    Apana530GeV.close();
    TGraph *Apana530GeVGraph = new TGraphErrors(lines-1,pt,value,xErr,totErr);
    Apana530GeVGraph->SetName("Apana530GeV");
    Apana530GeVGraph->SetTitle("p+p (#sqrt{#it{s}}= 31.6 GeV)");
    Apana530GeVGraph->Print();
    for (Int_t i= 0; i< Apana530GeVGraph->GetN(); i++){
        if (Apana530GeVGraph->GetX()[i] > 4){
            valuesEtaToPi0[0].push_back(Apana530GeVGraph->GetX()[i]);
            valuesEtaToPi0[1].push_back(Apana530GeVGraph->GetY()[i]);
            valuesEtaToPi0[2].push_back(Apana530GeVGraph->GetEY()[i]);
            valuesEtaToPi0[3].push_back(Apana530GeVGraph->GetEY()[i]);
            currentNumberOfPoint++;
        }
    }
    cout << "Number of pT bins: "<< currentNumberOfPoint <<  endl;

    // 		"apana02_pp_plab800GeV_fnal706_eta_pi0_ratio.txt","p+p (#sqrt{#it{s}}= 38.8 GeV)",

    ifstream Apana800GeV;
    Apana800GeV.open("ExternalInput/OtherExperiments/apana02_pp_plab800GeV_fnal706_eta_pi0_ratio.txt");
    cout << "apana02_pp_plab800GeV_fnal706_eta_pi0_ratio.txt" << endl;
    lines = 0;

    while(!Apana800GeV.eof()){
        Apana800GeV >> pt[lines] >> value[lines] >> totErr[lines];
        xErr[lines] = 0.;
        lines++;
    }
    Apana800GeV.close();
    TGraph *Apana800GeVGraph = new TGraphErrors(lines-1,pt,value,xErr,totErr);
    Apana800GeVGraph->SetName("Apana800GeV");
    Apana800GeVGraph->SetTitle("p+p (#sqrt{#it{s}}= 38.8 GeV)");
    Apana800GeVGraph->Print();
    for (Int_t i= 0; i< Apana800GeVGraph->GetN(); i++){
        if (Apana800GeVGraph->GetX()[i] > 4){
            valuesEtaToPi0[0].push_back(Apana800GeVGraph->GetX()[i]);
            valuesEtaToPi0[1].push_back(Apana800GeVGraph->GetY()[i]);
            valuesEtaToPi0[2].push_back(Apana800GeVGraph->GetEY()[i]);
            valuesEtaToPi0[3].push_back(Apana800GeVGraph->GetEY()[i]);
            currentNumberOfPoint++;
        }
    }
    cout << "Number of pT bins: "<< currentNumberOfPoint <<  endl;

// 		"kourkou79_pp_52.7GeV_eta_pi0_ratio.txt","p+p (#sqrt{#it{s}}= 52.7 GeV)",
    ifstream Kourkou79pp52;
    Kourkou79pp52.open("ExternalInput/OtherExperiments/kourkou79_pp_52.7GeV_eta_pi0_ratio.txt");
    cout << "kourkou79_pp_52.7GeV_eta_pi0_ratio.txt" << endl;
    lines = 0;

    while(!Kourkou79pp52.eof()){
        Kourkou79pp52 >> pt[lines] >> value[lines] >> totErr[lines];
        xErr[lines] = 0.;
        lines++;
    }
    Kourkou79pp52.close();
    TGraph *Kourkou79pp52Graph = new TGraphErrors(lines-1,pt,value,xErr,totErr);
    Kourkou79pp52Graph->SetName("Kourkou52.7GeVpp");
    Kourkou79pp52Graph->SetTitle("p+p (#sqrt{#it{s}}= 52.7 GeV)");
    Kourkou79pp52Graph->Print();
    for (Int_t i= 0; i< Kourkou79pp52Graph->GetN(); i++){
        if (Kourkou79pp52Graph->GetX()[i] > 4){
            valuesEtaToPi0[0].push_back(Kourkou79pp52Graph->GetX()[i]);
            valuesEtaToPi0[1].push_back(Kourkou79pp52Graph->GetY()[i]);
            valuesEtaToPi0[2].push_back(Kourkou79pp52Graph->GetEY()[i]);
            valuesEtaToPi0[3].push_back(Kourkou79pp52Graph->GetEY()[i]);
            currentNumberOfPoint++;
        }
    }
    cout << "Number of pT bins: "<< currentNumberOfPoint <<  endl;


// 		"akesson85_pbarp_53GeV_eta_pi0_ratio.txt","#bar{p}+p (#sqrt{#it{s}}= 53 GeV)",
//     @article{Akesson:1985za,
//         author         = "Akesson, T. and others",
//         title          = "{A comparison of direct photon, pi0, and eta production
//         in p anti-p and pp interactions at the CERN ISR}",
//         collaboration  = "Axial Field Spectrometer",
//         journal        = "Phys. Lett.",
//         volume         = "158B",
//         year           = "1985",
//         pages          = "282-288",
//         doi            = "10.1016/0370-2693(85)90971-2",
//         reportNumber   = "CERN-EP-85-55",
//         SLACcitation   = "%%CITATION = PHLTA,158B,282;%%"
//     }
    ifstream Akesson53GeVppbar;
    Akesson53GeVppbar.open("ExternalInput/OtherExperiments/akesson85_pbarp_53GeV_eta_pi0_ratio.txt");
    cout << "akesson85_pbarp_53GeV_eta_pi0_ratio.txt" << endl;
    lines = 0;

    while(!Akesson53GeVppbar.eof()){
        Akesson53GeVppbar >> pt[lines] >> value[lines] >> totErr[lines];
        xErr[lines] = 0.;
        lines++;
    }
    Akesson53GeVppbar.close();
    TGraph *Akesson53GeVppbarGraph = new TGraphErrors(lines-1,pt,value,xErr,totErr);
    Akesson53GeVppbarGraph->SetName("Akesson53GeVppbar");
    Akesson53GeVppbarGraph->SetTitle("#bar{p}+p (#sqrt{#it{s}}= 53 GeV)");
    Akesson53GeVppbarGraph->Print();
    for (Int_t i= 0; i< Akesson53GeVppbarGraph->GetN(); i++){
        if (Akesson53GeVppbarGraph->GetX()[i] > 4){
            valuesEtaToPi0[0].push_back(Akesson53GeVppbarGraph->GetX()[i]);
            valuesEtaToPi0[1].push_back(Akesson53GeVppbarGraph->GetY()[i]);
            valuesEtaToPi0[2].push_back(Akesson53GeVppbarGraph->GetEY()[i]);
            valuesEtaToPi0[3].push_back(Akesson53GeVppbarGraph->GetEY()[i]);
            currentNumberOfPoint++;
        }
    }
    cout << "Number of pT bins: "<< currentNumberOfPoint <<  endl;


    // 		"akesson85_pp_53GeV_eta_pi0_ratio.txt","p+p (#sqrt{#it{s}}= 53 GeV)",
//     @article{Akesson:1986nx,
//         author         = "Akesson, T. and others",
//         title          = "{Inclusive $\eta$ Production at Low Transverse Momentum
//         in 63-{GeV} $p p$ Collisions at the {CERN} Intersecting
//         Storage Rings}",
//         booktitle      = "{Proceedings, 23RD International Conference on High
//         Energy Physics, JULY 16-23, 1986, Berkeley, CA}",
//         collaboration  = "Axial Field Spectrometer",
//         journal        = "Phys. Lett.",
//         volume         = "B178",
//         year           = "1986",
//         pages          = "447",
//         doi            = "10.1016/0370-2693(86)91409-7",
//         reportNumber   = "CERN-EP-86-84",
//         SLACcitation   = "%%CITATION = PHLTA,B178,447;%%"
//     }
    ifstream Akesson53GeVpp;
    Akesson53GeVpp.open("ExternalInput/OtherExperiments/akesson85_pp_53GeV_eta_pi0_ratio.txt");
    cout << "akesson85_pp_53GeV_eta_pi0_ratio.txt" << endl;
    lines = 0;

    while(!Akesson53GeVpp.eof()){
        Akesson53GeVpp >> pt[lines] >> value[lines] >> totErr[lines];
        xErr[lines] = 0.;
        lines++;
    }
    Akesson53GeVpp.close();
    TGraph *Akesson53GeVppGraph = new TGraphErrors(lines-1,pt,value,xErr,totErr);
    Akesson53GeVppGraph->SetName("Akesson53GeVpp");
    Akesson53GeVppGraph->SetTitle("p+p (#sqrt{#it{s}}= 53 GeV)");
    Akesson53GeVppGraph->Print();
    for (Int_t i= 0; i< Akesson53GeVppGraph->GetN(); i++){
        if (Akesson53GeVppGraph->GetX()[i] > 4){
            valuesEtaToPi0[0].push_back(Akesson53GeVppGraph->GetX()[i]);
            valuesEtaToPi0[1].push_back(Akesson53GeVppGraph->GetY()[i]);
            valuesEtaToPi0[2].push_back(Akesson53GeVppGraph->GetEY()[i]);
            valuesEtaToPi0[3].push_back(Akesson53GeVppGraph->GetEY()[i]);
            currentNumberOfPoint++;
        }
    }
    cout << "Number of pT bins: "<< currentNumberOfPoint <<  endl;

    // 		"amaldi79_pp_53GeV_isr_eta_pi0_ratio.txt","p+p (#sqrt{#it{s}}= 53.2 GeV)",
    ifstream Amaldi53GeVpp;
    Amaldi53GeVpp.open("ExternalInput/OtherExperiments/amaldi79_pp_53GeV_isr_eta_pi0_ratio.txt");
    cout << "amaldi79_pp_53GeV_isr_eta_pi0_ratio.txt" << endl;
    lines = 0;

    while(!Amaldi53GeVpp.eof()){
        Amaldi53GeVpp >> pt[lines] >> value[lines] >> totErr[lines];
        xErr[lines] = 0.;
        lines++;
    }
    Amaldi53GeVpp.close();
    TGraph *Amaldi53GeVppGraph = new TGraphErrors(lines-1,pt,value,xErr,totErr);
    Amaldi53GeVppGraph->SetName("Amaldi53GeVpp");
    Amaldi53GeVppGraph->SetTitle("p+p (#sqrt{#it{s}}= 53.2 GeV)");
    Amaldi53GeVppGraph->Print();
    cout << "Number of pT bins: "<< currentNumberOfPoint <<  endl;
    for (Int_t i= 0; i< Amaldi53GeVppGraph->GetN(); i++){
        if (Amaldi53GeVppGraph->GetX()[i] > 4){
            valuesEtaToPi0[0].push_back(Amaldi53GeVppGraph->GetX()[i]);
            valuesEtaToPi0[1].push_back(Amaldi53GeVppGraph->GetY()[i]);
            valuesEtaToPi0[2].push_back(Amaldi53GeVppGraph->GetEY()[i]);
            valuesEtaToPi0[3].push_back(Amaldi53GeVppGraph->GetEY()[i]);
            currentNumberOfPoint++;
        }
    }
    cout << "Number of pT bins: "<< currentNumberOfPoint <<  endl;


    // 		"kourkou79_pp_62.4GeV_eta_pi0_ratio.txt","p+p (#sqrt{#it{s}}= 62.4 GeV)",
    ifstream Kourkou79pp62;
    Kourkou79pp62.open("ExternalInput/OtherExperiments/kourkou79_pp_62.4GeV_eta_pi0_ratio.txt");
    cout << "kourkou79_pp_62.4GeV_eta_pi0_ratio.txt" << endl;
    lines = 0;

    while(!Kourkou79pp62.eof()){
        Kourkou79pp62 >> pt[lines] >> value[lines] >> totErr[lines];
        xErr[lines] = 0.;
        lines++;
    }
    Kourkou79pp62.close();
    TGraph *Kourkou79pp62Graph = new TGraphErrors(lines-1,pt,value,xErr,totErr);
    Kourkou79pp62Graph->SetName("Kourkou79pp62");
    Kourkou79pp62Graph->SetTitle("p+p (#sqrt{#it{s}}= 62.4 GeV)");
    Kourkou79pp62Graph->Print();
    for (Int_t i= 0; i< Kourkou79pp62Graph->GetN(); i++){
        if (Kourkou79pp62Graph->GetX()[i] > 4){
            valuesEtaToPi0[0].push_back(Kourkou79pp62Graph->GetX()[i]);
            valuesEtaToPi0[1].push_back(Kourkou79pp62Graph->GetY()[i]);
            valuesEtaToPi0[2].push_back(Kourkou79pp62Graph->GetEY()[i]);
            valuesEtaToPi0[3].push_back(Kourkou79pp62Graph->GetEY()[i]);
            currentNumberOfPoint++;
        }
    }
    cout << "Number of pT bins: "<< currentNumberOfPoint <<  endl;

    // 		"akesson83_pp_63GeV_eta_pi0_ratio.txt","p+p (#sqrt{#it{s}}= 63 GeV)",
    ifstream Akesson63GeVpp;
    Akesson63GeVpp.open("ExternalInput/OtherExperiments/akesson83_pp_63GeV_eta_pi0_ratio.txt");
    cout << "akesson83_pp_63GeV_eta_pi0_ratio.txt" << endl;
    lines = 0;
//     @article{Akesson:1983xq,
//         author         = "Akesson, T. and others",
//         title          = "{Production of $\pi^0$ and Eta0 at 11-degrees in $p p$
//         Collisions at $\sqrt{s}=63$-{GeV}}",
//         collaboration  = "Axial Field Spectrometer",
//         journal        = "Z. Phys.",
//         volume         = "C18",
//         year           = "1983",
//         pages          = "5",
//         doi            = "10.1007/BF01571698",
//         reportNumber   = "CERN-EP-83-17",
//         SLACcitation   = "%%CITATION = ZEPYA,C18,5;%%"
//     }
    while(!Akesson63GeVpp.eof()){
        Akesson63GeVpp >> a >> b >> value[lines] >> totErr[lines];
        pt[lines]   = (a+b)/2;
        xErr[lines] = 0.;
        lines++;
    }
    Akesson63GeVpp.close();
    TGraph *Akesson63GeVppGraph = new TGraphErrors(lines-1,pt,value,xErr,totErr);
    Akesson63GeVppGraph->SetName("Akesson63GeVpp");
    Akesson63GeVppGraph->SetTitle("p+p (#sqrt{#it{s}}= 63 GeV)");
    Akesson63GeVppGraph->Print();
    for (Int_t i= 0; i< Akesson63GeVppGraph->GetN(); i++){
        if (Akesson63GeVppGraph->GetX()[i] > 4){
            valuesEtaToPi0[0].push_back(Akesson63GeVppGraph->GetX()[i]);
            valuesEtaToPi0[1].push_back(Akesson63GeVppGraph->GetY()[i]);
            valuesEtaToPi0[2].push_back(Akesson63GeVppGraph->GetEY()[i]);
            valuesEtaToPi0[3].push_back(Akesson63GeVppGraph->GetEY()[i]);
            currentNumberOfPoint++;
        }
    }
    cout << "Number of pT bins: "<< currentNumberOfPoint <<  endl;



// 		"phenix_pp_200GeV_eta_pi0_ratio.txt","p+p (#sqrt{#it{s}}= 200 GeV)",
//     @Article{Adler:2006bv,
//         Title                    = {{High transverse momentum $\eta$ meson production in $p^+ p$, $d^+$ Au and Au+Au collisions at $\sqrt{s_{\mbox{\tiny NN}}}$ = 200 GeV}},
//         Author                   = {Adler, S.S. and others},
//         Journal                  = {Phys.Rev.},
//         Year                     = {2007},
//         Pages                    = {024909},
//         Volume                   = {C75},
//
//         Archiveprefix            = {arXiv},
//         Collaboration            = {PHENIX Collaboration},
//         Doi                      = {10.1103/PhysRevC.75.024909},
//         Eprint                   = {nucl-ex/0611006},
//         Primaryclass             = {nucl-ex},
//         Slaccitation             = {%%CITATION = NUCL-EX/0611006;%%}
//     }
//
    ifstream Phenix200GeV;
    Phenix200GeV.open("ExternalInput/OtherExperiments/phenix_pp_200GeV_eta_pi0_ratio.txt");
    cout << "phenix_pp_200GeV_eta_pi0_ratio.txt" << endl;
    lines = 0;


    while(!Phenix200GeV.eof()){
        Phenix200GeV >> pt[lines] >> value[lines] >> statErr[lines] >> sysErr[lines] >> sysErrB[lines];
        totErr[lines] = TMath::Sqrt(statErr[lines]*statErr[lines]+ sysErr[lines]*sysErr[lines] + sysErrB[lines]*sysErrB[lines]);
        xErr[lines] = 0.;
        lines++;
    }
    Phenix200GeV.close();
    TGraph *Phenix200GeVGraph = new TGraphErrors(lines-1,pt,value,xErr,totErr);
    Phenix200GeVGraph->SetName("Phenix200GeV");
    Phenix200GeVGraph->SetTitle("p+p (#sqrt{#it{s}}= 200 GeV)");
    Phenix200GeVGraph->Print();
    for (Int_t i= 0; i< Phenix200GeVGraph->GetN(); i++){
        if (Phenix200GeVGraph->GetX()[i] > 4){
            valuesEtaToPi0[0].push_back(Phenix200GeVGraph->GetX()[i]);
            valuesEtaToPi0[1].push_back(Phenix200GeVGraph->GetY()[i]);
            valuesEtaToPi0[2].push_back(Phenix200GeVGraph->GetEY()[i]);
            valuesEtaToPi0[3].push_back(Phenix200GeVGraph->GetEY()[i]);
            currentNumberOfPoint++;
        }
    }

// 		"banner85_ppbar_540GeV_UA2_eta_pi0_ratio.txt","#bar{p}+p (#sqrt{#it{s}}= 540 GeV)",
//     @article{Banner:1984wh,
//         author         = "Banner, M. and others",
//         title          = "{Inclusive Particle Production in the Transverse Momentum
//         Range Between 0.25-{GeV}/$c$ and 40-{GeV}/$c$ at the
//         {CERN} S $p \bar{p}$ S Collider}",
//         collaboration  = "UA2, Bern-CERN-Copenhagen-Orsay-Pavia-Saclay",
//         journal        = "Z. Phys.",
//         volume         = "C27",
//         year           = "1985",
//         pages          = "329",
//         doi            = "10.1007/BF01548636",
//         reportNumber   = "SACLAY-DPHPE-84-07",
//         SLACcitation   = "%%CITATION = ZEPYA,C27,329;%%"
//     }
    ifstream Banner540GeV;
    Banner540GeV.open("ExternalInput/OtherExperiments/banner85_ppbar_540GeV_UA2_eta_pi0_ratio.txt");
    cout << "banner85_ppbar_540GeV_UA2_eta_pi0_ratio.txt" << endl;
    lines = 0;

    while(!Banner540GeV.eof()){
        Banner540GeV >> pt[lines] >> value[lines] >> totErr[lines];
        xErr[lines] = 0.;
        lines++;
    }
    Banner540GeV.close();
    TGraph *Banner540GeVGraph = new TGraphErrors(lines-1,pt,value,xErr,totErr);
    Banner540GeVGraph->SetName("Banner540GeV");
    Banner540GeVGraph->SetTitle("#bar{p}+p (#sqrt{#it{s}}= 540 GeV)");
    Banner540GeVGraph->Print();
    for (Int_t i= 0; i< Banner540GeVGraph->GetN(); i++){
        if (Banner540GeVGraph->GetX()[i] > 4){
            valuesEtaToPi0[0].push_back(Banner540GeVGraph->GetX()[i]);
            valuesEtaToPi0[1].push_back(Banner540GeVGraph->GetY()[i]);
            valuesEtaToPi0[2].push_back(Banner540GeVGraph->GetEY()[i]);
            valuesEtaToPi0[3].push_back(Banner540GeVGraph->GetEY()[i]);
            currentNumberOfPoint++;
        }
    }
    cout << "Number of pT bins: "<< currentNumberOfPoint <<  endl;


    // eta/pi0 7TeV ALICE
    // Experiment: CERN-LHC-ALICE (ALICE)
    // Published in PL B717,162 (DOI:10.1016/j.physletb.2012.09.015)
    // Preprinted as CERN-PH-EP-2012-001
    // Archived as: ARXIV:1205.5724
//     @Article{Abelev:2012cn,
//         Title                    = {{Neutral pion and $\eta$ meson production in proton-proton collisions at $\sqrt{s}=0.9$ TeV and $\sqrt{s}=7$ TeV}},
//         Author                   = {Abelev, B. and others},
//         Journal                  = {Phys.Lett.},
//         Year                     = {2012},
//         Pages                    = {162-172},
//         Volume                   = {B717},
//
//         Archiveprefix            = {arXiv},
//         Collaboration            = {ALICE Collaboration},
//         Doi                      = {10.1016/j.physletb.2012.09.015},
//         Eprint                   = {1205.5724},
//         Primaryclass             = {hep-ex},
//         Reportnumber             = {CERN-PH-EP-2012-001},
//         Slaccitation             = {%%CITATION = ARXIV:1205.5724;%%}
//     }
    cout << "7TeV measurements" << endl;
    double Alice7TeV_xval[] = { 0.55, 0.85, 1.2, 1.6, 2.0, 2.4, 2.8, 3.25, 3.75,
        5.0, 7.0, 9.0, 12.5 };
    double Alice7TeV_xerrminus[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0 };
    double Alice7TeV_xerrplus[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0 };
    double Alice7TeV_yval[] = { 0.1067, 0.1655, 0.2501, 0.2866, 0.3514, 0.3547, 0.3757, 0.3987, 0.4655,
        0.4686, 0.5014, 0.7082, 0.5205 };
    double Alice7TeV_yerrminus[] = { 0.03347013594235912, 0.02529268669002959, 0.02603113520382851, 0.026801865606707307, 0.030696090956341654, 0.02972574641619618, 0.035979021665409415, 0.03927250946909301, 0.04585193561890272,
        0.04081041533726409, 0.08140718641495971, 0.18510386273657284, 0.27690873947927325 };
    double Alice7TeV_yerrplus[] = { 0.03347013594235912, 0.02529268669002959, 0.02603113520382851, 0.026801865606707307, 0.030696090956341654, 0.02972574641619618, 0.035979021665409415, 0.03927250946909301, 0.04585193561890272,
        0.04081041533726409, 0.08140718641495971, 0.18510386273657284, 0.27690873947927325 };
    double Alice7TeV_ystatminus[] = { 0.0259, 0.0234, 0.0179, 0.0175, 0.0201, 0.0219, 0.0232, 0.0267, 0.0336,
        0.0282, 0.0588, 0.1538, 0.1679 };
    double Alice7TeV_ystatplus[] = { 0.0259, 0.0234, 0.0179, 0.0175, 0.0201, 0.0219, 0.0232, 0.0267, 0.0336,
        0.0282, 0.0588, 0.1538, 0.1679 };
    int Alice7TeV_numpoints = 13;
    TGraphAsymmErrors* Alice7TeVGraph = new TGraphAsymmErrors(Alice7TeV_numpoints, Alice7TeV_xval, Alice7TeV_yval, Alice7TeV_xerrminus, Alice7TeV_xerrplus, Alice7TeV_yerrminus, Alice7TeV_yerrplus);
    Alice7TeVGraph->SetName("Alice7TeV");
    TFile* file7TeVAlice                                    = new TFile("ExternalInput/CombNeutralMesons/CombinedResultsPP_ShiftedX_PaperRAA_16_May_2014_including7TeVand900GeVpublished.root");
    TGraphAsymmErrors* Alice7TeVGraphStat                   = (TGraphAsymmErrors*)file7TeVAlice->Get("graphEtaToPi0Comb7TeVStat");
    TGraphAsymmErrors* Alice7TeVGraphSys                    = (TGraphAsymmErrors*)file7TeVAlice->Get("graphEtaToPi0Comb7TeVSys");
    Alice7TeVGraphStat->SetName("Alice7TeV_Stat");
    Alice7TeVGraphSys->SetName("Alice7TeV_Sys");
    for (Int_t i= 0; i< Alice7TeVGraph->GetN(); i++){
        if (Alice7TeVGraph->GetX()[i] > 4){
            valuesEtaToPi0[0].push_back(Alice7TeVGraph->GetX()[i]);
            valuesEtaToPi0[1].push_back(Alice7TeVGraph->GetY()[i]);
            valuesEtaToPi0[2].push_back(Alice7TeVGraph->GetEYlow()[i]);
            valuesEtaToPi0[3].push_back(Alice7TeVGraph->GetEYhigh()[i]);
            currentNumberOfPoint++;
            valuesEtaToPi0ALICE[0].push_back(Alice7TeVGraph->GetX()[i]);
            valuesEtaToPi0ALICE[1].push_back(Alice7TeVGraph->GetY()[i]);
            valuesEtaToPi0ALICE[2].push_back(Alice7TeVGraph->GetEYlow()[i]);
            valuesEtaToPi0ALICE[3].push_back(Alice7TeVGraph->GetEYhigh()[i]);
            currentNumberOfPointALICE++;
            cout << "adding points" << endl;
        }
    }
    cout << "Number of pT bins: "<< currentNumberOfPoint <<  endl;
    cout << "Number of pT bins ALICE: "<< currentNumberOfPointALICE <<  endl;

    // eta/pi0 2.76TeV ALICE
    // Experiment: CERN-LHC-ALICE (ALICE)
    // @Article{Acharya:2017hyu,
    //         Title                    = {{Production of ${\pi ^0}$ and $\eta $ mesons up to high
    //         transverse momentum in pp collisions at 2.76 TeV}},
    //         Author                   = {Acharya, Shreyasi and others},
    //         Journal                  = {Eur. Phys. J.},
    //         Year                     = {2017},
    //         Number                   = {5},
    //         Pages                    = {339},
    //         Volume                   = {C77},
    //
    //         Archiveprefix            = {arXiv},
    //         Collaboration            = {ALICE},
    //         Doi                      = {10.1140/epjc/s10052-017-4890-x},
    //         Eprint                   = {1702.00917},
    //         Primaryclass             = {hep-ex},
    //         Reportnumber             = {CERN-EP-2017-019},
    //         Slaccitation             = {%%CITATION = ARXIV:1702.00917;%%}
    //     }
    TGraphAsymmErrors* Alice2760GeVGraph        = NULL;
    TGraphAsymmErrors* Alice2760GeVGraphStat    = NULL;
    TGraphAsymmErrors* Alice2760GeVGraphSys     = NULL;
    cout << "2.76TeV measurements" << endl;
    TFile* file2760GeVAlice                 = new TFile("ExternalInput/CombNeutralMesons/CombinedResultsPaperPP2760GeV_2017_07_10_Pub2017.root");
    Alice2760GeVGraph                       = (TGraphAsymmErrors*)file2760GeVAlice->Get("Eta2.76TeV/graphRatioEtaToPi0Comb2760GeVTotErr");
    Alice2760GeVGraphStat                   = (TGraphAsymmErrors*)file2760GeVAlice->Get("Eta2.76TeV/graphRatioEtaToPi0Comb2760GeVStatErr");
    Alice2760GeVGraphSys                    = (TGraphAsymmErrors*)file2760GeVAlice->Get("Eta2.76TeV/graphRatioEtaToPi0Comb2760GeVSysErr");
    Alice2760GeVGraph->SetName("Alice2760GeV");
    Alice2760GeVGraphStat->SetName("Alice2760GeV_Stat");
    Alice2760GeVGraphSys->SetName("Alice2760GeV_Sys");
    for (Int_t i= 0; i< Alice2760GeVGraph->GetN(); i++){
        if (Alice2760GeVGraph->GetX()[i] > 4){
            valuesEtaToPi0[0].push_back(Alice2760GeVGraph->GetX()[i]);
            valuesEtaToPi0[1].push_back(Alice2760GeVGraph->GetY()[i]);
            valuesEtaToPi0[2].push_back(Alice2760GeVGraph->GetEYlow()[i]);
            valuesEtaToPi0[3].push_back(Alice2760GeVGraph->GetEYhigh()[i]);
            currentNumberOfPoint++;
            valuesEtaToPi0ALICE[0].push_back(Alice2760GeVGraph->GetX()[i]);
            valuesEtaToPi0ALICE[1].push_back(Alice2760GeVGraph->GetY()[i]);
            valuesEtaToPi0ALICE[2].push_back(Alice2760GeVGraph->GetEYlow()[i]);
            valuesEtaToPi0ALICE[3].push_back(Alice2760GeVGraph->GetEYhigh()[i]);
            currentNumberOfPointALICE++;

        }
    }
    cout << "Number of pT bins: "<< currentNumberOfPoint <<  endl;
    cout << "Number of pT bins ALICE: "<< currentNumberOfPointALICE <<  endl;


    TGraphAsymmErrors* Alice8TeVGraph        = NULL;
    TGraphAsymmErrors* Alice8TeVGraphStat    = NULL;
    TGraphAsymmErrors* Alice8TeVGraphSys     = NULL;
    cout << "8TeV measurements" << endl;
    TFile* file8TeVAlice                 = new TFile("ExternalInput/CombNeutralMesons/CombinedResultsPaperPP8TeV_2017_11_16.root");
    Alice8TeVGraph                       = (TGraphAsymmErrors*)file8TeVAlice->Get("Eta8TeV/graphRatioEtaToPi0Comb8TeVTotErr");
    Alice8TeVGraphStat                   = (TGraphAsymmErrors*)file8TeVAlice->Get("Eta8TeV/graphRatioEtaToPi0Comb8TeVStatErr");
    Alice8TeVGraphSys                    = (TGraphAsymmErrors*)file8TeVAlice->Get("Eta8TeV/graphRatioEtaToPi0Comb8TeVSysErr");
    Alice8TeVGraph->SetName("Alice8TeV");
    Alice8TeVGraphStat->SetName("Alice8TeV_Stat");
    Alice8TeVGraphSys->SetName("Alice8TeV_Sys");
    for (Int_t i= 0; i< Alice8TeVGraph->GetN(); i++){
        if (Alice8TeVGraph->GetX()[i] > 4){
            valuesEtaToPi0[0].push_back(Alice8TeVGraph->GetX()[i]);
            valuesEtaToPi0[1].push_back(Alice8TeVGraph->GetY()[i]);
            valuesEtaToPi0[2].push_back(Alice8TeVGraph->GetEYlow()[i]);
            valuesEtaToPi0[3].push_back(Alice8TeVGraph->GetEYhigh()[i]);
            currentNumberOfPoint++;
            valuesEtaToPi0ALICE[0].push_back(Alice8TeVGraph->GetX()[i]);
            valuesEtaToPi0ALICE[1].push_back(Alice8TeVGraph->GetY()[i]);
            valuesEtaToPi0ALICE[2].push_back(Alice8TeVGraph->GetEYlow()[i]);
            valuesEtaToPi0ALICE[3].push_back(Alice8TeVGraph->GetEYhigh()[i]);
            currentNumberOfPointALICE++;
        }
    }
    cout << "Number of pT bins: "<< currentNumberOfPoint <<  endl;
    cout << "Number of pT bins ALICE: "<< currentNumberOfPointALICE <<  endl;


    TGraphAsymmErrors* graphWorldHigh               = new TGraphAsymmErrors(currentNumberOfPoint);
    graphWorldHigh->SetName("AllWorldDataAbove4GeV");
    for (Int_t k = 0; k< currentNumberOfPoint; k++){
        graphWorldHigh->SetPoint(k, valuesEtaToPi0[0].at(k), valuesEtaToPi0[1].at(k));
        graphWorldHigh->SetPointError(k, 0.00, 0.00, valuesEtaToPi0[2].at(k), valuesEtaToPi0[3].at(k));
    }
    graphWorldHigh->Print();
    graphWorldHigh->Sort();
    cout << "after sorting" << endl;
    graphWorldHigh->Print();

    TF1* etaToPi0ConstData  = new TF1("etaToPi0ConstData","[0]",4,20);
    graphWorldHigh->Fit(etaToPi0ConstData,"QRME0","",4,20);

    cout  << "***********************************************************************************************************" << endl;
    cout  << "high pt eta/pi0 - data, tot: " << etaToPi0ConstData->GetParameter(0) << "+-"<< etaToPi0ConstData->GetParError(0) << endl;
    cout  << "***********************************************************************************************************" << endl;
    cout  << "***********************************************************************************************************" << endl;

    TGraphAsymmErrors* graphWorldHighALICE               = new TGraphAsymmErrors(currentNumberOfPointALICE);
    graphWorldHighALICE->SetName("AllWorldDataAbove4GeVALICE");
    for (Int_t k = 0; k< currentNumberOfPointALICE; k++){
        graphWorldHighALICE->SetPoint(k, valuesEtaToPi0ALICE[0].at(k), valuesEtaToPi0ALICE[1].at(k));
        graphWorldHighALICE->SetPointError(k, 0.00, 0.00, valuesEtaToPi0ALICE[2].at(k), valuesEtaToPi0ALICE[3].at(k));
    }
    graphWorldHighALICE->Print();
    graphWorldHighALICE->Sort();
    cout << "after sorting" << endl;
    graphWorldHighALICE->Print();

    TF1* etaToPi0ConstDataALICE  = new TF1("etaToPi0ConstDataALICE","[0]",4,20);
    graphWorldHighALICE->Fit(etaToPi0ConstDataALICE,"QRME0","",4,20);

    cout  << "***********************************************************************************************************" << endl;
    cout  << "ALICE high pt eta/pi0 - data, tot: " << etaToPi0ConstDataALICE->GetParameter(0) << "+-"<< etaToPi0ConstDataALICE->GetParError(0) << endl;
    cout  << "***********************************************************************************************************" << endl;
    cout  << "***********************************************************************************************************" << endl;
    
    
    TFile fileDataGraphs("ExternalInput/OtherExperiments/DataCompilationFromOtherEnergiesPPnew.root","UPDATE");
    fileDataGraphs.mkdir("Pi0");
    fileDataGraphs.cd("Pi0");
        pi0Phenix200GeVGraphStat->Write(pi0Phenix200GeVGraphStat->GetName(),TObject::kOverwrite)
        pi0Phenix200GeVGraphSys->Write(pi0Phenix200GeVGraphSys->GetName(),TObject::kOverwrite)
        
    fileDataGraphs.mkdir("Eta");
    fileDataGraphs.cd("Eta");

    fileDataGraphs.mkdir("EtaToPi0");
    fileDataGraphs.cd("EtaToPi0");
        Donaldson100GeVGraph->Write(Donaldson100GeVGraph->GetName(),TObject::kOverwrite);
        Donaldson200GeVGraph->Write(Donaldson200GeVGraph->GetName(),TObject::kOverwrite);;
        Bonesi280GeVGraph->Write(Bonesi280GeVGraph->GetName(),TObject::kOverwrite);
        Antille87ppGraph->Write(Antille87ppGraph->GetName(),TObject::kOverwrite);
        Antille87ppbarGraph->Write(Antille87ppbarGraph->GetName(),TObject::kOverwrite);
        Aguilar400GeVGraph->Write(Aguilar400GeVGraph->GetName(),TObject::kOverwrite);
        Amaldi79ppGraph->Write(Amaldi79ppGraph->GetName(),TObject::kOverwrite);
        Kourkou79ppGraph->Write(Kourkou79ppGraph->GetName(),TObject::kOverwrite);
        Apana530GeVGraph->Write(Apana530GeVGraph->GetName(),TObject::kOverwrite);
        Apana800GeVGraph->Write(Apana800GeVGraph->GetName(),TObject::kOverwrite);
        Kourkou79pp52Graph->Write(Kourkou79pp52Graph->GetName(),TObject::kOverwrite);
        Akesson53GeVppbarGraph->Write(Akesson53GeVppbarGraph->GetName(),TObject::kOverwrite);
        Akesson53GeVppGraph->Write(Akesson53GeVppGraph->GetName(),TObject::kOverwrite);
        Amaldi53GeVppGraph->Write(Amaldi53GeVppGraph->GetName(),TObject::kOverwrite);
        Kourkou79pp62Graph->Write(Kourkou79pp62Graph->GetName(),TObject::kOverwrite);
        Akesson63GeVppGraph->Write(Akesson63GeVppGraph->GetName(),TObject::kOverwrite);
        Phenix200GeVGraph->Write(Phenix200GeVGraph->GetName(),TObject::kOverwrite);
        Banner540GeVGraph->Write(Banner540GeVGraph->GetName(),TObject::kOverwrite);
        Alice7TeVGraph->Write(Alice7TeVGraph->GetName(),TObject::kOverwrite);
        Alice7TeVGraphStat->Write(Alice7TeVGraphStat->GetName(),TObject::kOverwrite);
        Alice7TeVGraphSys->Write(Alice7TeVGraphSys->GetName(),TObject::kOverwrite);
        Alice2760GeVGraph->Write(Alice2760GeVGraph->GetName(),TObject::kOverwrite);
        Alice2760GeVGraphStat->Write(Alice2760GeVGraphStat->GetName(),TObject::kOverwrite);
        Alice2760GeVGraphSys->Write(Alice2760GeVGraphSys->GetName(),TObject::kOverwrite);
        Alice8TeVGraph->Write(Alice8TeVGraph->GetName(),TObject::kOverwrite);
        Alice8TeVGraphStat->Write(Alice8TeVGraphStat->GetName(),TObject::kOverwrite);
        Alice8TeVGraphSys->Write(Alice8TeVGraphSys->GetName(),TObject::kOverwrite);
        // high pt graphs
        graphWorldHigh->Write(graphWorldHigh->GetName(),TObject::kOverwrite);
        etaToPi0ConstData->Write(etaToPi0ConstData->GetName(),TObject::kOverwrite);
        // high pt graphs ALICE
        graphWorldHighALICE->Write(graphWorldHighALICE->GetName(),TObject::kOverwrite);
        etaToPi0ConstDataALICE->Write(etaToPi0ConstDataALICE->GetName(),TObject::kOverwrite);
    
    fileDataGraphs.mkdir("Gamma");
    fileDataGraphs.cd("Gamma");
        tE_DirPh_ATLAS->Write("ATLAS_tot_Gamma_7TeV", TObject::kOverwrite);
        tTot_7TeV_DirPh_ATLAS0->Write("ATLAS_0_tot_Gamma_7TeV", TObject::kOverwrite);
        tTot_7TeV_DirPh_ATLAS->Write("ATLAS_1_tot_Gamma_7TeV", TObject::kOverwrite);
        tTot_7TeV_DirPh_ATLAS2->Write("ATLAS_2_tot_Gamma_7TeV", TObject::kOverwrite);
        tTot_8TeV_DirPh_ATLAS->Write("ATLAS_tot_Gamma_8TeV", TObject::kOverwrite);
        tTot_13TeV_DirPh_ATLAS->Write("ATLAS_tot_Gamma_13TeV", TObject::kOverwrite);
        tTot_2760GeV_DirPh_CMS->Write("CMS_tot_Gamma_2.76TeV", TObject::kOverwrite);
        tTot_7TeV_IsoPh_ALICE->Write("ALICE_tot_Gamma_7TeV", TObject::kOverwrite);
        tE_DirPh_CMS->Write("CMS_tot_Gamma_7TeV", TObject::kOverwrite);
        tTot_7TeV_DirPh_CMS->Write("CMS_1_tot_Gamma_7TeV", TObject::kOverwrite);
        tTot_1940GeV_DirPh_CDF->Write("CDF_tot_Gamma_1.94TeV", TObject::kOverwrite);
        tE_DirPh_CDF->Write("CDF_0_tot_Gamma_1.8TeV", TObject::kOverwrite);
        tTot_1800GeV_DirPh_CDF->Write("CDF_tot_Gamma_1.8TeV", TObject::kOverwrite);
        tTot_630GeV_DirPh_CDF->Write("CDF_tot_Gamma_630GeV", TObject::kOverwrite);
        tTot_1940GeV_DirPh_D0->Write("D0_tot_Gamma_1.94GeV", TObject::kOverwrite);
        tE_DirPh_D0->Write("D0_0_tot_Gamma_1.8TeV", TObject::kOverwrite);
        tTot_630GeV_DirPh_D0->Write("D0_tot_Gamma_630GeV", TObject::kOverwrite);
        tE_DirPh_UA1_0->Write("UA1_tot_Gamma_630GeV", TObject::kOverwrite);
        tE_DirPh_UA1_1->Write("UA1_tot_Gamma_546GeV", TObject::kOverwrite);
        tE_DirPh_UA2->Write("UA2_tot_Gamma_630GeV", TObject::kOverwrite);
        tE_DirPh_PHENIX->Write("PHENIX_tot_Gamma_200GeV", TObject::kOverwrite);
        tE_DirPh_R807->Write("R807_tot_Gamma_63GeV", TObject::kOverwrite);
        tE_DirPh_R108->Write("R807_tot_Gamma_62.4GeV", TObject::kOverwrite);
        tE_DirPh_E706->Write("E706_tot_Gamma_38.8GeV", TObject::kOverwrite);
        tE_DirPh_E706_0->Write("E706_tot_Gamma_31.6GeV", TObject::kOverwrite);
        tE_DirPh_E704->Write("E704_tot_Gamma_19.4GeV", TObject::kOverwrite);
        tE_DirPh_NA24->Write("NA24_tot_Gamma_23.8GeV", TObject::kOverwrite);
        tE_DirPh_WA70->Write("WA70_tot_Gamma_23GeV", TObject::kOverwrite);
        tE_DirPh_UA6_0->Write("UA6_0_tot_Gamma_24.3GeV", TObject::kOverwrite);
        tE_DirPh_UA6_1->Write("UA6_1_tot_Gamma_24.3GeV", TObject::kOverwrite);

        tExT_DirPh_ATLAS->Write("ATLAS_tot_Gamma_7TeV_xT", TObject::kOverwrite);
        tTotxT_7TeV_DirPh_ATLAS0->Write("ATLAS_0_tot_Gamma_7TeV_xT", TObject::kOverwrite);
        tTotxT_7TeV_DirPh_ATLAS->Write("ATLAS_1_tot_Gamma_7TeV_xT", TObject::kOverwrite);
        tTotxT_7TeV_DirPh_ATLAS2->Write("ATLAS_2_tot_Gamma_7TeV_xT", TObject::kOverwrite);
        tTotxT_8TeV_DirPh_ATLAS->Write("ATLAS_tot_Gamma_8TeV_xT", TObject::kOverwrite);
        tTotxT_13TeV_DirPh_ATLAS->Write("ATLAS_tot_Gamma_13TeV_xT", TObject::kOverwrite);
        tExT_DirPh_CMS->Write("CMS_tot_Gamma_7TeV_xT", TObject::kOverwrite);
        tTotxT_7TeV_IsoPh_ALICE->Write("ALICE_tot_Gamma_7TeV_xT", TObject::kOverwrite);
        tTotxT_7TeV_DirPh_CMS->Write("CMS_1_tot_Gamma_7TeV_xT", TObject::kOverwrite);
        tTotxT_2760GeV_DirPh_CMS->Write("CMS_tot_Gamma_2.76TeV_xT", TObject::kOverwrite);
        tTotxT_1940GeV_DirPh_CDF->Write("CDF_tot_Gamma_1.94TeV_xT", TObject::kOverwrite);
        tExT_DirPh_CDF->Write("CDF_0_tot_Gamma_1.8TeV_xT", TObject::kOverwrite);
        tTotxT_1800GeV_DirPh_CDF->Write("CDF_tot_Gamma_1.8TeV_xT", TObject::kOverwrite);
        tTotxT_630GeV_DirPh_CDF->Write("CDF_tot_Gamma_630GeV_xT", TObject::kOverwrite);
        tTotxT_1940GeV_DirPh_D0->Write("D0_tot_Gamma_1.94TeV_xT", TObject::kOverwrite);
        tExT_DirPh_D0->Write("D0_0_tot_Gamma_1.8TeV_xT", TObject::kOverwrite);
        tTotxT_630GeV_DirPh_D0->Write("D0_tot_Gamma_630GeV_xT", TObject::kOverwrite);
        tExT_DirPh_UA1_0->Write("UA1_tot_Gamma_630GeV_xT", TObject::kOverwrite);
        tExT_DirPh_UA1_1->Write("UA1_tot_Gamma_546GeV_xT", TObject::kOverwrite);
        tExT_DirPh_UA2->Write("UA2_tot_Gamma_630GeV_xT", TObject::kOverwrite);
        tExT_DirPh_PHENIX->Write("PHENIX_tot_Gamma_200GeV_xT", TObject::kOverwrite);
        tExT_DirPh_R807->Write("R807_tot_Gamma_63GeV_xT", TObject::kOverwrite);
        tExT_DirPh_R108->Write("R108_tot_Gamma_62.4GeV_xT", TObject::kOverwrite);
        tExT_DirPh_E706->Write("E706_tot_Gamma_38.8GeV_xT", TObject::kOverwrite);
        tExT_DirPh_E706_0->Write("E706_tot_Gamma_31.6GeV_xT", TObject::kOverwrite);
        tExT_DirPh_E704->Write("E704_tot_Gamma_19.4GeV_xT", TObject::kOverwrite);
        tExT_DirPh_NA24->Write("NA24_tot_Gamma_23.8GeV_xT", TObject::kOverwrite);
        tExT_DirPh_WA70->Write("WA70_tot_Gamma_23GeV_xT", TObject::kOverwrite);
        tExT_DirPh_UA6_0->Write("UA6_0_tot_Gamma_24.3GeV_xT", TObject::kOverwrite);
        tExT_DirPh_UA6_1->Write("UA6_1_tot_Gamma_24.3GeV_xT", TObject::kOverwrite);

        RGammaStat_7TeV_LMEE_ALICE->Write("RGamma_stat_LMEE_7TeV", TObject::kOverwrite);
        RGammaSys_7TeV_LMEE_ALICE->Write("RGamma_sys_LMEE_7TeV", TObject::kOverwrite);
        RGammaTot_7TeV_LMEE_ALICE->Write("RGamma_tot_LMEE_7TeV", TObject::kOverwrite);

    fileDataGraphs.Close();
}
