/****************************************************************************************************************************
******         provided by Gamma Conversion Group, PWG4,                                                     *****
******        Friederike Bock, friederike.bock@cern.ch                                                    *****
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
#include "TFitResultPtr.h"
#include "TFitResult.h"
#include "CommonHeaders/PlottingGammaConversionHistos.h"
#include "CommonHeaders/PlottingGammaConversionAdditional.h"
#include "CommonHeaders/FittingGammaConversion.h"
#include "CommonHeaders/ConversionFunctionsBasicsAndLabeling.h"
#include "CommonHeaders/ConversionFunctions.h"
#include "CommonHeaders/CombinationFunctions.h"

extern TRandom*    gRandom;
extern TBenchmark* gBenchmark;
extern TSystem*    gSystem;
extern TMinuit*    gMinuit;

struct SysErrorConversion {
    Double_t value;
    Double_t error;
    //    TString name;
};

/*void drawLatexAdd(TString latextext, Double_t textcolumn, Double_t textrow, Double_t textSizePixel,Bool_t setFont = kFALSE, Bool_t setFont2 = kFALSE, Bool_t alignRight = kFALSE, Color_t textcolor = kBlack){
    TLatex *latexDummy                  = new TLatex(textcolumn ,textrow,latextext);
    SetStyleTLatex( latexDummy, textSizePixel,4);
    if(setFont)
        latexDummy->SetTextFont(62);
    if(setFont2)
        latexDummy->SetTextFont(43);
    if(alignRight)
        latexDummy->SetTextAlign(31);
    latexDummy->SetTextColor(textcolor);
    latexDummy->Draw();
}*/


void CombineMesonMeasurementsPiPlPiMiPiZero(      TString fileNamePCM     = "",
                                        TString fileNamePHOS    = "",
                                        TString fileNameEMCal   = "/home/nschmidt/AnalysisSoftware/pdf/5TeV/2017_08_19/FinalResultsTriggersPatched_EMC/data_EMCAL-EMCALResultsFullCorrection_PP.root",
                                        TString fileNamePCMPHOS = "",
                                        TString fileNamePCMEMCal= "/home/nschmidt/AnalysisSoftware/pdf/5TeV/2017_08_20/FinalResultsTriggersPatched/data_PCM-EMCALResultsFullCorrection_PP.root",
                                        TString bWCorrection       = "",
                                        TString suffix          = "pdf",
                                        Int_t numbersofmeas     = 5,
                                        Bool_t usePaperInput  = kFALSE,
                                        Bool_t doEta            = kFALSE,
                                        Bool_t doOmegaToPi0     = kTRUE,
                                        Bool_t useFitForRatio = kTRUE
                                    ){

    // Settings
    Bool_t drawPHOSPN = kTRUE;


    TString date                                = ReturnDateString();

    gROOT->Reset();
    gROOT->SetStyle("Plain");

    StyleSettingsThesis();
    SetPlotStyle();

    TString dateForOutput                       = ReturnDateStringForOutput();
    cout << dateForOutput.Data() << endl;
    //___________________________________ Declaration of files _____________________________________________
    TString collisionSystem7TeV               = "pp, #sqrt{#it{s}} = 7 TeV";
    TString outputDir                           = Form("%s/%s/CombineMesonMeasurements7TeV",suffix.Data(),dateForOutput.Data());
    cout << outputDir.Data() << endl;
    cout << fileNamePCM.Data() << endl;

    TString fileNameTheory		        			= "ExternalInput/Theory/TheoryCompilationPP.root";
    // Theory file for eta in correct binning
    TString fileNameTheoryEtaBinning       			= "/media/florianjonas/dataslave/data/alice/pp7TeV/ThesisExternal/pythia8_compilation_7tev_monash_etaBinning.root";

    TString fileNamePHOSExternal					= "/media/florianjonas/dae8586b-f761-47b0-b8ef-93a00caf0e11/analysis/pp7TeV_OmegaFinal/ExternalInput/om_spectrum.root";
    TString fileNamePHOSExternalRatio				= "/media/florianjonas/dae8586b-f761-47b0-b8ef-93a00caf0e11/analysis/pp7TeV_OmegaFinal/ExternalInput/om_pi_ratio.root";

    // Use Nico instead of published
    TString fileNameCombPi07TeVPublished			= "ExternalInput/CombNeutralMesons/CombinedResultsPaperPP7TeV_2017_11_17.root";

    TString fileNameCombPi07TevNico                 = "/media/florianjonas/dae8586b-f761-47b0-b8ef-93a00caf0e11/analysis/pp7TeV_OmegaFinal/ExternalInput/CombinedResultsPaperPP7TeV_2019_12_03.root";

    TString fileNameSysCorrelation                  = "/media/florianjonas/dae8586b-f761-47b0-b8ef-93a00caf0e11/analysis/pp7TeV_OmegaFinal/CorrelationFactors/pdf/2019_11_15/ComputeCorrelationFactors_pp7TeV/pp7TeV.root";

    gSystem->Exec("mkdir -p "+outputDir);
    if(fileNamePCM.CompareTo(""))
      gSystem->Exec(Form("cp %s %s/InputPCM.root", fileNamePCM.Data(), outputDir.Data()));
    if(fileNamePHOS.CompareTo(""))
      gSystem->Exec(Form("cp %s %s/InputPHOS.root", fileNamePHOS.Data(), outputDir.Data()));
    if(fileNameEMCal.CompareTo(""))
      gSystem->Exec(Form("cp %s %s/InputEMCal.root", fileNameEMCal.Data(), outputDir.Data()));
    if(fileNamePCMPHOS.CompareTo(""))
      gSystem->Exec(Form("cp %s %s/InputPCMPHOS.root", fileNamePCMPHOS.Data(), outputDir.Data()));
    if(fileNamePCMEMCal.CompareTo(""))
      gSystem->Exec(Form("cp %s %s/InputPCMEMCal.root", fileNamePCMEMCal.Data(), outputDir.Data()));


    if(fileNameTheory.CompareTo(""))
      gSystem->Exec(Form("cp %s %s/InputTheory.root", fileNameTheory.Data(), outputDir.Data()));
    if(fileNamePHOSExternal.CompareTo(""))
      gSystem->Exec(Form("cp %s %s/InputPHOSExternal.root", fileNamePHOSExternal.Data(), outputDir.Data()));
    if(fileNameCombPi07TeVPublished.CompareTo(""))
      gSystem->Exec(Form("cp %s %s/InputPi07TeVPublished.root", fileNameCombPi07TeVPublished.Data(), outputDir.Data()));
    if(fileNameCombPi07TevNico.CompareTo("") && !usePaperInput)
      gSystem->Exec(Form("cp %s %s/InputPi07TeVNico.root", fileNameCombPi07TevNico.Data(), outputDir.Data()));


    fstream fLog;
    fLog.open(Form("%s/CombineMeson7TeV.log",outputDir.Data()), ios::out);
    fLog << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    fLog << dateForOutput.Data() << endl;

    Double_t mesonMassExpectOmega                = TDatabasePDG::Instance()->GetParticle(223)->Mass();
    Double_t mesonMassExpectEta                 = TDatabasePDG::Instance()->GetParticle(221)->Mass();

    Width_t widthLinesBoxes                     = 1;
    Width_t widthCommonFit                      = 2;

    // Definition of colors, styles and markers sizes
    Color_t colorComb                           = kBlue+2;
    Style_t markerStyleComb                     = 20;
    Size_t  markerSizeComb                      = 2;

    TString nameMeasGlobal[11]                  = {"PCM", "PHOS", "EMCal", "PCM-PHOS", "PCM-EMCal", "PCM-Dalitz", "PHOS-Dalitz", "EMCal-Dalitz", "EMCal high pT", "EMCal merged", "PCMOtherDataset"};
    Color_t colorDet[11];
    Color_t colorDetMC[11];
    Style_t markerStyleDet[11];
    Style_t markerStyleDetMC[11];
    Size_t  markerSizeDet[11];
    Size_t  markerSizeDetMC[11];

    Size_t  sizeMarkerNLO                       = 1;
    Width_t widthLineNLO                        = 2.;
    
    Double_t minPtOmega                         = 1.3;
    Double_t maxPtOmega                         = 30.5;

    TString localOutputQuantity                             = "#frac{1}{N_{ev}} #frac{1}{2#pi#it{p}_{T}} #frac{d#it{N}^{2}}{d#it{p}_{T}dy} ((GeV/#it{c})^{-2})";

    // load external files

    TFile* fFileOmegaMesonPHOS7TeV                         = new TFile(fileNamePHOSExternal);
    TFile* fFileOmegaMesonRatioPHOS7TeV                    = new TFile(fileNamePHOSExternalRatio);
    TFile* fFileOmegaMesonSim7TeV                          = new TFile(fileNameTheory);
    TFile* fFileEtaMesonSim7TeV                            = new TFile(fileNameTheoryEtaBinning);
    TFile* fFilePi0Comb7TeVPublished                                = new TFile(fileNameCombPi07TeVPublished);
    TFile* fFilePi0Comb7TeVNico                 = new TFile(fileNameCombPi07TevNico);


    // load external histos
    TGraphErrors* graphOmegaXSecPHOS7TeVStat               = (TGraphErrors*)fFileOmegaMesonPHOS7TeV->Get("gr_om");
    TGraphAsymmErrors* graphOmegaXSecPHOS7TeVSys           = (TGraphAsymmErrors*)fFileOmegaMesonPHOS7TeV->Get("gr_om_sys");
    TGraphAsymmErrors* graphOmegaToPi0PHOS7TeVStat         = (TGraphAsymmErrors*)fFileOmegaMesonRatioPHOS7TeV->Get("gr_om_rat");

    // load theory histos Monash
    TH1F*  histoOmegaXSecSim7TeV                            = (TH1F*) fFileOmegaMesonSim7TeV->Get("histoInvSecPythia8Monash2013Omega7TeV");
    TH1F*  histoPi0XSecSim7TeV                              = (TH1F*) fFileOmegaMesonSim7TeV->Get("histoInvSecPythia8Monash2013LegoPi07TeV");
    TH1F*  histoPi0XSecSim7TeVEtaBinning                    = (TH1F*) fFileEtaMesonSim7TeV->Get("h_invXsec_pi0primary_yDefault");
    TH1F*  histoEtaXSecSim7TeV                              = (TH1F*) fFileEtaMesonSim7TeV->Get("h_invXsec_eta_yDefault");

    TGraphErrors* graphPythia8InvXSectionOmega               = new TGraphErrors((TH1F*) fFileOmegaMesonSim7TeV->Get("histoInvSecPythia8Monash2013Omega7TeV"));
    while(graphPythia8InvXSectionOmega->GetX()[0] < minPtOmega) graphPythia8InvXSectionOmega->RemovePoint(0);
    while(graphPythia8InvXSectionOmega->GetX()[graphPythia8InvXSectionOmega->GetN()-1] > maxPtOmega) graphPythia8InvXSectionOmega->RemovePoint(graphPythia8InvXSectionOmega->GetN()-1);
    TGraphErrors* graphPythia8InvXSectionPi0            = new TGraphErrors((TH1F*) fFileOmegaMesonSim7TeV->Get("histoInvSecPythia8Monash2013LegoPi07TeV"));
    while(graphPythia8InvXSectionPi0->GetX()[0] < minPtOmega) graphPythia8InvXSectionPi0->RemovePoint(0);
    while(graphPythia8InvXSectionPi0->GetX()[graphPythia8InvXSectionPi0->GetN()-1] > maxPtOmega) graphPythia8InvXSectionPi0->RemovePoint(graphPythia8InvXSectionPi0->GetN()-1);
    TH1F* histoPythia8OmegaToPi0                          = (TH1F*) histoOmegaXSecSim7TeV->Clone("Pythia8OmegaToPi0");
    histoPythia8OmegaToPi0->Divide(histoPi0XSecSim7TeV);
    //histoPythia8OmegaToPi0->GetXaxis()->SetRangeUser(0.1,18.);
    TGraphErrors* graphPythia8OmegaToPi0                  = new TGraphErrors(histoPythia8OmegaToPi0);
    while(graphPythia8OmegaToPi0->GetX()[0] < 0.4) graphPythia8OmegaToPi0->RemovePoint(0);
    while(graphPythia8OmegaToPi0->GetX()[graphPythia8OmegaToPi0->GetN()-1] > 15.) graphPythia8OmegaToPi0->RemovePoint(graphPythia8OmegaToPi0->GetN()-1);

    // load theory histos tune4C
    TH1F*  histoOmegaXSecSimTune4C7TeV                            = (TH1F*) fFileOmegaMesonSim7TeV->Get("histoInvSecPythia8tune4COmega7TeV");
    TH1F*  histoPi0XSecSimTune4C7TeV                              = (TH1F*) fFileOmegaMesonSim7TeV->Get("histoInvSecPythia8tune4CLegoPi07TeV");
    TH1F*  histoPi0XSecSimTune4C7TeVEtaBinning                    = (TH1F*) fFileEtaMesonSim7TeV->Get("h_invXsec_pi0primary_yDefault");
    TH1F*  histoEtaXSecSimTune4c7TeV                              = (TH1F*) fFileEtaMesonSim7TeV->Get("h_invXsec_eta_yDefault");

    TGraphErrors* graphPythia8Tune4CInvXSectionOmega               = new TGraphErrors((TH1F*) fFileOmegaMesonSim7TeV->Get("histoInvSecPythia8tune4COmega7TeV"));
    while(graphPythia8Tune4CInvXSectionOmega->GetX()[0] < minPtOmega) graphPythia8Tune4CInvXSectionOmega->RemovePoint(0);
    while(graphPythia8Tune4CInvXSectionOmega->GetX()[graphPythia8Tune4CInvXSectionOmega->GetN()-1] > maxPtOmega) graphPythia8Tune4CInvXSectionOmega->RemovePoint(graphPythia8Tune4CInvXSectionOmega->GetN()-1);
    TGraphErrors* graphPythia8Tune4CInvXSectionPi0            = new TGraphErrors((TH1F*) fFileOmegaMesonSim7TeV->Get("histoInvSecPythia8tune4CLegoPi07TeV"));
    while(graphPythia8Tune4CInvXSectionPi0->GetX()[0] < minPtOmega) graphPythia8Tune4CInvXSectionPi0->RemovePoint(0);
    while(graphPythia8Tune4CInvXSectionPi0->GetX()[graphPythia8Tune4CInvXSectionPi0->GetN()-1] > maxPtOmega) graphPythia8Tune4CInvXSectionPi0->RemovePoint(graphPythia8Tune4CInvXSectionPi0->GetN()-1);
    TH1F* histoPythia8Tune4COmegaToPi0                          = (TH1F*) histoOmegaXSecSim7TeV->Clone("Pythia8Tune4COmegaToPi0");
    histoPythia8Tune4COmegaToPi0->Divide(histoPi0XSecSimTune4C7TeV);
    //histoPythia8Tune4COmegaToPi0->GetXaxis()->SetRangeUser(0.1,18.);
    TGraphErrors* graphPythia8Tune4COmegaToPi0                  = new TGraphErrors(histoPythia8Tune4COmegaToPi0);
    while(graphPythia8Tune4COmegaToPi0->GetX()[0] < 0.4) graphPythia8Tune4COmegaToPi0->RemovePoint(0);
    while(graphPythia8Tune4COmegaToPi0->GetX()[graphPythia8Tune4COmegaToPi0->GetN()-1] > 15.) graphPythia8Tune4COmegaToPi0->RemovePoint(graphPythia8Tune4COmegaToPi0->GetN()-1);


    // Get NLO Calculations
    TGraphErrors* graphNLOInvXSectionOmega  = (TGraphErrors*) fFileOmegaMesonSim7TeV->Get("graphNLOCalcOmega7000GeV");
    while(graphNLOInvXSectionOmega->GetX()[0] < minPtOmega) graphNLOInvXSectionOmega->RemovePoint(0);
    while(graphNLOInvXSectionOmega->GetX()[graphNLOInvXSectionOmega->GetN()-1] > maxPtOmega) graphNLOInvXSectionOmega->RemovePoint(graphNLOInvXSectionOmega->GetN()-1);

    TGraphAsymmErrors* graphPi0XSecComb7TeVStat        = NULL;
    TGraphAsymmErrors* graphPi0XSecComb7TeVSys         = NULL;
    TGraphAsymmErrors* graphPi0XSecComb7TeVTot         = NULL;

    TGraphAsymmErrors* graphEtaXSecComb7TeVStat        = NULL;
    TGraphAsymmErrors* graphEtaXSecComb7TeVSys         = NULL;
    TGraphAsymmErrors* graphEtaXSecComb7TeVTot         = NULL;

    TGraphAsymmErrors* graphEtaToPi0Comb7TeVStat        = NULL;
    TGraphAsymmErrors* graphEtaToPi0Comb7TeVSys         = NULL;
    TGraphAsymmErrors* graphEtaToPi0Comb7TeVTot         = NULL;

    graphPi0XSecComb7TeVStat        = (TGraphAsymmErrors*)fFilePi0Comb7TeVPublished->Get("graphInvCrossSectionPi0Comb7TeVStatErr");
    graphPi0XSecComb7TeVSys         = (TGraphAsymmErrors*)fFilePi0Comb7TeVPublished->Get("graphInvCrossSectionPi0Comb7TeVSysErr");
    graphPi0XSecComb7TeVTot         = (TGraphAsymmErrors*)fFilePi0Comb7TeVPublished->Get("graphInvCrossSectionPi0Comb7TeV");

    graphEtaXSecComb7TeVStat        = (TGraphAsymmErrors*)fFilePi0Comb7TeVPublished->Get("graphInvCrossSectionEtaComb7TeVStatErr");
    graphEtaXSecComb7TeVSys         = (TGraphAsymmErrors*)fFilePi0Comb7TeVPublished->Get("graphInvCrossSectionEtaComb7TeVSysErr");
    graphEtaXSecComb7TeVTot         = (TGraphAsymmErrors*)fFilePi0Comb7TeVPublished->Get("graphInvCrossSectionEtaComb7TeV");

    // Load Daniel thesis histos, or from ExternalInput folder
    TF1* fitTsallisPi0XSecComb7TeVThesis = NULL;
    TF1* fitTsallisEtaXSecComb7TeVThesis = NULL;
    if(usePaperInput){
        graphPi0XSecComb7TeVStat  = (TGraphAsymmErrors*)( (TDirectory*) fFilePi0Comb7TeVPublished->Get("Pi07TeV"))->Get("graphInvCrossSectionPi0Comb7TeVStatErr_yShifted");
        graphPi0XSecComb7TeVSys   = (TGraphAsymmErrors*)( (TDirectory*) fFilePi0Comb7TeVPublished->Get("Pi07TeV"))->Get("graphInvCrossSectionPi0Comb7TeVSysErr_yShifted");
        graphPi0XSecComb7TeVTot   = (TGraphAsymmErrors*)( (TDirectory*) fFilePi0Comb7TeVPublished->Get("Pi07TeV"))->Get("graphInvCrossSectionPi0Comb7TeV_yShifted");
        
        // Get Fit from Nico because old Tsallis is broken (root5)
        fitTsallisPi0XSecComb7TeVThesis               = (TF1*)( (TDirectory*) fFilePi0Comb7TeVNico->Get("Pi07TeV"))->Get("TsallisFitPi0");

        cout << __LINE__ << endl;
        cout << fitTsallisPi0XSecComb7TeVThesis << endl;

        graphEtaXSecComb7TeVStat  = (TGraphAsymmErrors*)( (TDirectory*) fFilePi0Comb7TeVPublished->Get("Eta7TeV"))->Get("graphInvCrossSectionEtaComb7TeVStatErr_yShifted");
        graphEtaXSecComb7TeVSys   = (TGraphAsymmErrors*)( (TDirectory*) fFilePi0Comb7TeVPublished->Get("Eta7TeV"))->Get("graphInvCrossSectionEtaComb7TeVSysErr_yShifted");
        graphEtaXSecComb7TeVTot   = (TGraphAsymmErrors*)( (TDirectory*) fFilePi0Comb7TeVPublished->Get("Eta7TeV"))->Get("graphInvCrossSectionEtaComb7TeV_yShifted");
        fitTsallisEtaXSecComb7TeVThesis               = (TF1*)( (TDirectory*) fFilePi0Comb7TeVNico->Get("Eta7TeV"))->Get("TsallisFitEta");

        graphEtaToPi0Comb7TeVStat        = (TGraphAsymmErrors*)fFilePi0Comb7TeVPublished->Get("graphRatioEtaToPi0Comb7TeVStatErr");
        graphEtaToPi0Comb7TeVSys         = (TGraphAsymmErrors*)fFilePi0Comb7TeVPublished->Get("graphRatioEtaToPi0Comb7TeVSysErr");
        graphEtaToPi0Comb7TeVTot         = (TGraphAsymmErrors*)fFilePi0Comb7TeVPublished->Get("graphRatioEtaToPi0Comb7TeVTotErr");
    } else{
        fitTsallisPi0XSecComb7TeVThesis               = (TF1*)( (TDirectory*) fFilePi0Comb7TeVNico->Get("Pi07TeV"))->Get("TsallisFitPi0");

        graphPi0XSecComb7TeVStat        = (TGraphAsymmErrors*)fFilePi0Comb7TeVNico->Get("graphInvCrossSectionPi0Comb7TeVStatErr");
        graphPi0XSecComb7TeVSys         = (TGraphAsymmErrors*)fFilePi0Comb7TeVNico->Get("graphInvCrossSectionPi0Comb7TeVSysErr");
        graphPi0XSecComb7TeVTot         = (TGraphAsymmErrors*)fFilePi0Comb7TeVNico->Get("graphInvCrossSectionPi0Comb7TeV");

        graphEtaXSecComb7TeVStat        = (TGraphAsymmErrors*)fFilePi0Comb7TeVNico->Get("graphInvCrossSectionEtaComb7TeVStatErr");
        graphEtaXSecComb7TeVSys         = (TGraphAsymmErrors*)fFilePi0Comb7TeVNico->Get("graphInvCrossSectionEtaComb7TeVSysErr");
        graphEtaXSecComb7TeVTot         = (TGraphAsymmErrors*)fFilePi0Comb7TeVNico->Get("graphInvCrossSectionEtaComb7TeV");

        graphEtaToPi0Comb7TeVStat        = (TGraphAsymmErrors*)fFilePi0Comb7TeVNico->Get("graphEtaToPi0Comb7TeVStat");
        graphEtaToPi0Comb7TeVSys         = (TGraphAsymmErrors*)fFilePi0Comb7TeVNico->Get("graphEtaToPi0Comb7TeVSys");
        graphEtaToPi0Comb7TeVTot         = (TGraphAsymmErrors*)fFilePi0Comb7TeVNico->Get("graphEtaToPi0Comb7TeV");
    }


    TGraphErrors* graphOmegaToPi0PHOSPublic7TeVStat           = (TGraphErrors*)fFileOmegaMesonRatioPHOS7TeV->Get("gr_om_rat");
    TGraphAsymmErrors* graphOmegaToPi0PHOSPublic7TeVSys       = (TGraphAsymmErrors*)fFileOmegaMesonRatioPHOS7TeV->Get("gr_om_rat_sys");

    // Manually load PHENIX 200 GeV omega/pi0 ratios: taken from https://www.phenix.bnl.gov/phenix/WWW/info/data/ppg118_data.html
    Double_t xValue200GeVee[9]      ={0.125,0.375,0.625,0.875,1.125,1.375,1.75,2.5,3.5};
    Double_t yValue200GeVee[9]      ={0.0274625,0.0775713,0.209742,0.277281,0.418289,0.47334, 0.54105,0.612235,0.714046 };
    Double_t yStatError200GeVee[9]  ={0.00874824,0.0145487,0.022483, 0.0365503,0.0550088,0.073987,0.091216,0.115561,0.247288 };
    Double_t xStatError200GeVee[9]  ={0,0,0,0,0,0,0,0,0};
    Double_t ySysError200GeVee[9]   ={0.0062165,0.0157521,0.0346946,0.0469135,0.0706437,0.0788823,0.088133,0.110047,0.130358 };
    Double_t xSysError200GeVee[9]   ={0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1};

    Double_t xValue200GeVpipipi[20]      ={2.25,2.75,3.25,3.75,4.25,4.75,5.25,5.75,6.25,6.75,7.25,7.75,8.25,8.75,9.25,9.75,10.25,10.75,11.25,12.00};
    Double_t yValue200GeVpipipi[20]      ={0.877423,0.994651,0.945003,0.903472,0.742332,0.788344,0.814015,0.71074, 0.69682,0.761297,0.863745,0.652674,0.775114,0.64524, 1.1310, 1.0159, 0.89945,0.85533, 1.07303,0.860426};
    Double_t yStatError200GeVpipipi[20]  ={0.209278,   0.103237,   0.0693594,   0.0462791,   0.0430213,   0.069176,   0.0553398,   0.0609064,   0.103044,   0.150899,   0.15363,   0.107463,   0.174188,   0.146433,   0.206244,   0.284549,   0.277659,   0.257244,   0.292399,   0.284385};
    Double_t ySysError200GeVpipipi[20]     ={0.196965,   0.204638,   0.186131,   0.161472,   0.126264,   0.134575,   0.139265,   0.121728,   0.106482,   0.12334,   0.147959,   0.124096,   0.154794,   0.14128,   0.269633,   0.252251,   0.2236,   0.212935,   0.267555,   0.18234};
    Double_t xStatError200GeVpipipi[20]  ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    Double_t xSysError200GeVpipipi[20]   ={0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1};

    Double_t xValue200GeVpi0Gamma[8]      ={2.5,3.5,4.5,5.5,6.5,7.5,9.0,11.0};
    Double_t yValue200GeVpi0Gamma[8]      ={0.760013,  0.898841,  0.686148,  0.762621,  0.862995,  0.638679,  0.718389,  0.918832};
    Double_t yStatError200GeVpi0Gamma[8]  ={0.06456,   0.038146,   0.0558147,   0.117929,   0.107197,   0.1618,   0.231785,   0.304524};
    Double_t ySysError200GeVpi0Gamma[8]   ={0.148646,   0.16022,   0.11694,   0.12346,   0.15594,   0.121424,   0.157363,   0.220082};
    Double_t xStatError200GeVpi0Gamma[8]  ={0,0,0,0,0,0,0,0};
    Double_t xSysError200GeVpi0Gamma[8]   ={0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1};


    Double_t xValue62GeVpi0Gamma[2]      ={3.,6.};
    Double_t yValue62GeVpi0Gamma[2]      ={0.87,0.90};
    Double_t yStatError62GeVpi0Gamma[2]  ={0.17,0.18};
    Double_t xStatError62GeVpi0Gamma[2]  ={0,0};
    Double_t xSysError62GeVpi0Gamma[2]   ={0.1,0.1};

    TGraphAsymmErrors* graphOmegaeeToPi0200GeVStat = new TGraphAsymmErrors(9,xValue200GeVee,yValue200GeVee,xStatError200GeVee,xStatError200GeVee,yStatError200GeVee,yStatError200GeVee);
    TGraphAsymmErrors* graphOmegaeeToPi0200GeVSys  = new TGraphAsymmErrors(9,xValue200GeVee,yValue200GeVee,xSysError200GeVee,xSysError200GeVee,ySysError200GeVee,ySysError200GeVee);

    TGraphAsymmErrors* graphOmegapipipiToPi0200GeVStat = new TGraphAsymmErrors(20,xValue200GeVpipipi,yValue200GeVpipipi,xStatError200GeVpipipi,xStatError200GeVpipipi,yStatError200GeVpipipi,yStatError200GeVpipipi);
    TGraphAsymmErrors* graphOmegapipipiToPi0200GeVSys  = new TGraphAsymmErrors(20,xValue200GeVpipipi,yValue200GeVpipipi,xSysError200GeVpipipi,xSysError200GeVpipipi,ySysError200GeVpipipi,ySysError200GeVpipipi);

    TGraphAsymmErrors* graphOmegapi0GammaToPi0200GeVStat = new TGraphAsymmErrors(8,xValue200GeVpi0Gamma,yValue200GeVpi0Gamma,xStatError200GeVpi0Gamma,xStatError200GeVpi0Gamma,yStatError200GeVpi0Gamma,yStatError200GeVpi0Gamma);
    TGraphAsymmErrors* graphOmegapi0GammaToPi0200GeVSys  = new TGraphAsymmErrors(8,xValue200GeVpi0Gamma,yValue200GeVpi0Gamma,xSysError200GeVpi0Gamma,xSysError200GeVpi0Gamma,ySysError200GeVpi0Gamma,ySysError200GeVpi0Gamma);

    TGraphAsymmErrors* graphOmegapi0GammaToPi062GeVStat = new TGraphAsymmErrors(2,xValue62GeVpi0Gamma,yValue62GeVpi0Gamma,xStatError62GeVpi0Gamma,xStatError62GeVpi0Gamma,yStatError62GeVpi0Gamma,yStatError62GeVpi0Gamma);

    for (Int_t i = 0; i < 11; i++){
        colorDet[i]                             = GetDefaultColorDiffDetectors(nameMeasGlobal[i].Data(), kFALSE, kFALSE, kTRUE);
        colorDetMC[i]                           = GetDefaultColorDiffDetectors(nameMeasGlobal[i].Data(), kTRUE, kFALSE, kTRUE);
        markerStyleDet[i]                       = GetDefaultMarkerStyleDiffDetectors(nameMeasGlobal[i].Data(), kFALSE);
        markerStyleDetMC[i]                     = GetDefaultMarkerStyleDiffDetectors(nameMeasGlobal[i].Data(), kTRUE);
        markerSizeDet[i]                        = GetDefaultMarkerSizeDiffDetectors(nameMeasGlobal[i].Data(), kFALSE)*2;
        markerSizeDetMC[i]                      = GetDefaultMarkerSizeDiffDetectors(nameMeasGlobal[i].Data(), kTRUE)*2;
    }

    TFile* inputFile[10];
    TFile* inputFilePi0[10];
        inputFile[0]                            = new TFile(fileNamePCM.Data());
        inputFile[1]                            = new TFile(fileNamePHOS.Data());
        inputFile[2]                            = new TFile(fileNameEMCal.Data());
        inputFile[3]                            = new TFile(fileNamePCMPHOS.Data());
        inputFile[4]                            = new TFile(fileNamePCMEMCal.Data());



    TDirectory* directoryOmega[10];
    TDirectory* directoryEta[10];
    for(Int_t i=0;i<numbersofmeas;i++){
      if(!inputFile[i]->IsZombie()){
        cout << "loading directories for " <<  nameMeasGlobal[i] << endl;
        directoryOmega[i]                     = (TDirectory*)inputFile[i]->Get("Omega7TeV");
        if(doEta) directoryEta[i]                       = (TDirectory*)inputFile[i]->Get("Eta7TeV");
      } else{
         cout << "directories for " <<  nameMeasGlobal[i] << "could not be loaded" << endl;
         directoryOmega[i] = NULL;
      }
    }
    cout << __LINE__<<endl;
    TH1D* histoOmegaMass[11] = {NULL};
    TH1D* histoOmegaFWHMMeV[11] = {NULL};
    TH1D* histoOmegaTrueMass[11] = {NULL};
    TH1D* histoOmegaTrueFWHMMeV[11] = {NULL};
    TH1D* histoEtaMass[11] = {NULL};
    TH1D* histoEtaFWHMMeV[11] = {NULL};
    TH1D* histoEtaTrueMass[11] = {NULL};
    TH1D* histoEtaTrueFWHMMeV[11] = {NULL};
    TH1D* histoOmegaAcc[11] = {NULL};
    TH1D* histoOmegaTrueEffPt[11] = {NULL};
    TH1D* histoOmegaAccTimesEff[11] = {NULL};

    TH1D* histoOmegaAcc_EMC7[11] = {NULL};
    TH1D* histoOmegaTrueEffPt_EMC7[11] = {NULL};
    TH1D* histoOmegaAccTimesEff_EMC7[11] = {NULL};

    TH1D* histoEtaAcc[11] = {NULL};
    TH1D* histoEtaTrueEffPt[11] = {NULL};
    TH1D* histoEtaAccTimesEff[11] = {NULL};
    TH1D* histoEtaAccTimesEffBR[11] = {NULL};
    TH1D* histoOmegaInvCrossSection[11] = {NULL};
    TH1D* histoEtaInvCrossSection[11] = {NULL};
    TH1D* histoOmegaToPi0Stat[11] = {NULL};
    TGraphAsymmErrors* graphOmegaInvCrossSectionSys[11] = {NULL};
    TGraphAsymmErrors* graphOmegaInvCrossSectionStat[11] = {NULL};
    TGraphAsymmErrors* graphEtaInvCrossSectionStat[11] = {NULL};
    TGraphAsymmErrors* graphEtaInvCrossSectionSys[11] = {NULL};
    TGraphAsymmErrors* graphOmegaToPi0Stat[11] = {NULL};
    TGraphAsymmErrors* graphOmegaToPi0Sys[11] = {NULL};
    TGraphAsymmErrors* graphEtaToOmegaPCMStat;
    TGraphAsymmErrors* graphEtaToOmegaPCMSys;

    // Get combined Eta crosssection
    TGraphAsymmErrors* graphInvCrossSectionEtaComb7TeVA        = NULL;
    TGraphAsymmErrors* graphInvCrossSectionEtaComb7TeVAStatErr = NULL;
    TGraphAsymmErrors* graphInvCrossSectionEtaComb7TeVASysErr  = NULL;

    Double_t branchingEtaGG     = 0.3941;
    Double_t branchingEtaPiPiPi = 0.2292;

    Double_t rapidityMeas[10]                   = {0.85*2, 0.85*2,0.85*2, 0.85*2,0.85*2,0.85*2,0.85*2};
    Double_t availableMeas[10]                  = {kFALSE,kFALSE,kFALSE,kFALSE,kFALSE,kFALSE,kFALSE};
    Int_t nMeasSet                              = 0;

    for (Int_t i = 0; i < 5; i++){
      if(inputFile[i]->IsZombie())
        continue;
      else
        cout << "loading  histos for " << nameMeasGlobal[i] << endl;
        
    //______________________________ Omega cross section
      graphOmegaInvCrossSectionSys[i]       = (TGraphAsymmErrors*)directoryOmega[i]->Get("InvCrossSectionOmegaSys");
      histoOmegaInvCrossSection[i]          = (TH1D*)directoryOmega[i]->Get("InvCrossSectionOmega");
      graphOmegaInvCrossSectionStat[i]      = new TGraphAsymmErrors(histoOmegaInvCrossSection[i]);
      if(!graphOmegaInvCrossSectionSys[i] || !histoOmegaInvCrossSection[i]){
        cout << "missing invariant cross section histograms of Omega... returning!" << endl;
        //return;
      } else {
        availableMeas[i]                  = kTRUE;
        nMeasSet++;
        cout << "loaded " << nameMeasGlobal[i] << " Omega invariant cross section" << endl;
      }
      
      //______________________________ Omega invariant mass peak pos and FWHM
      histoOmegaMass[i]                     = (TH1D*)directoryOmega[i]->Get("Omega_Mass_data_INT1");
      histoOmegaFWHMMeV[i]                  = (TH1D*)directoryOmega[i]->Get("Omega_Width_data_INT1");
      histoOmegaTrueMass[i]                 = (TH1D*)directoryOmega[i]->Get("Omega_Mass_MC_INT1");
      histoOmegaTrueFWHMMeV[i]              = (TH1D*)directoryOmega[i]->Get("Omega_Width_MC_INT1");
      if(!histoOmegaMass[i] || !histoOmegaFWHMMeV[i] || !histoOmegaFWHMMeV[i] || !histoOmegaFWHMMeV[i]){
        cout << "missing mass or width histograms... returning!" << endl;
        //return;
      } else {
        // scaling peak pos and FWHM to go from GeV/c2 to MeV/c2
        histoOmegaMass[i]                   ->Scale(1000);
        histoOmegaFWHMMeV[i]                ->Scale(1000);
        histoOmegaTrueMass[i]               ->Scale(1000);
        histoOmegaTrueFWHMMeV[i]            ->Scale(1000);
        cout << "loaded " << nameMeasGlobal[i] << " mass and width" << endl;
      }
      
      //______________________________ Omega acceptance and efficiency and calculate acc*eff*y*2pi
      histoOmegaAcc[i]                      = (TH1D*)directoryOmega[i]->Get("AcceptanceOmega_INT1");
      histoOmegaTrueEffPt[i]                = (TH1D*)directoryOmega[i]->Get("EfficiencyOmega_INT1");    

      histoOmegaAcc_EMC7[i]                      = (TH1D*)directoryOmega[i]->Get("AcceptanceOmega_EMC7");
      histoOmegaTrueEffPt_EMC7[i]                = (TH1D*)directoryOmega[i]->Get("EfficiencyOmega_EMC7");      
      // calculating for pi0 for later comparison

      if(!histoOmegaAcc[i] || !histoOmegaTrueEffPt[i]){
        cout << "missing acceptance or efficiency histograms... returning!" << endl;
        //return;
      } else {
        // calculating acceptance times efficiency
        histoOmegaAccTimesEff[i]            = (TH1D*)histoOmegaTrueEffPt[i]->Clone(Form("histoOmegaAccTimesEff%s",nameMeasGlobal[i].Data()));
        histoOmegaAccTimesEff[i]->Multiply(histoOmegaAcc[i]);
        histoOmegaAccTimesEff[i]->Scale(2*TMath::Pi()*rapidityMeas[i]);
        cout << "loaded " << nameMeasGlobal[i] << "acceptance and efficiency" << endl;

        if(histoOmegaTrueEffPt_EMC7[i] && histoOmegaAcc_EMC7[i]){
            histoOmegaAccTimesEff_EMC7[i]            = (TH1D*)histoOmegaTrueEffPt_EMC7[i]->Clone(Form("histoOmegaAccTimesEff%s_EMC7",nameMeasGlobal[i].Data()));
            histoOmegaAccTimesEff_EMC7[i]->Multiply(histoOmegaAcc_EMC7[i]);
            histoOmegaAccTimesEff_EMC7[i]->Scale(2*TMath::Pi()*rapidityMeas[i]);
            cout << "FOR EMC7: loaded " << nameMeasGlobal[i] << "acceptance and efficiency" << endl;
        }
      }

       //______________________________ Eta meson invariant cross section
        if(doEta){
            histoEtaInvCrossSection[i]        = (TH1D*)directoryEta[i]->Get("InvCrossSectionEta");
            graphEtaInvCrossSectionStat[i]    = (TGraphAsymmErrors*)directoryEta[i]->Get("graphInvCrossSectionEta");
            graphEtaInvCrossSectionSys[i]     = (TGraphAsymmErrors*)directoryEta[i]->Get("InvCrossSectionEtaSys");
            graphEtaToOmegaPCMStat              = (TGraphAsymmErrors*)directoryEta[i]->Get("graphEtaToOmegaStatError");
            graphEtaToOmegaPCMSys               = (TGraphAsymmErrors*)directoryEta[i]->Get("EtaToOmegaSystError");
        

            if(!graphEtaInvCrossSectionSys[i] || !histoEtaInvCrossSection[i]){
            cout << "missing invariant cross section histograms of eta... returning!" << endl;
            return;
            } else {
            cout << "loaded " << nameMeasGlobal[i] << " eta invariant cross section" << endl;
            }
            
            //______________________________ Eta meson invariant mass peak pos and FWHM
            histoEtaMass[i]                     = (TH1D*)directoryEta[i]->Get("Eta_Mass_data_INT1");
            histoEtaFWHMMeV[i]                  = (TH1D*)directoryEta[i]->Get("Eta_Width_data_INT1");
            histoEtaTrueMass[i]                 = (TH1D*)directoryEta[i]->Get("Eta_Mass_MC_INT1");
            histoEtaTrueFWHMMeV[i]              = (TH1D*)directoryEta[i]->Get("Eta_Width_MC_INT1");
            if(!histoEtaMass[i] || !histoEtaFWHMMeV[i] || !histoEtaFWHMMeV[i] || !histoEtaFWHMMeV[i]){
            cout << "missing mass or width histograms... returning!" << endl;
            return;
            } else {
            // scaling peak pos and FWHM to go from GeV/c2 to MeV/c2
            histoEtaMass[i]                   ->Scale(1000);
            histoEtaFWHMMeV[i]                ->Scale(1000);
            histoEtaTrueMass[i]               ->Scale(1000);
            histoEtaTrueFWHMMeV[i]            ->Scale(1000);
            cout << "loaded " << nameMeasGlobal[i] << " mass and width" << endl;
            }
            
            //______________________________ Neutral pion acceptance and efficiency and calculate acc*eff*y*2pi
            histoEtaAcc[i]                      = (TH1D*)directoryEta[i]->Get("AcceptanceEta_INT1");
            histoEtaTrueEffPt[i]                = (TH1D*)directoryEta[i]->Get("EfficiencyEta_INT1");
            if(!histoEtaAcc[i] || !histoEtaTrueEffPt[i]){
            cout << "missing acceptance or efficiency histograms... returning!" << endl;
            return;
            } else {
            // calculating acceptance times efficiency
            histoEtaAccTimesEff[i]            = (TH1D*)histoEtaTrueEffPt[i]->Clone(Form("histoEtaAccTimesEff%s",nameMeasGlobal[i].Data()));
            histoEtaAccTimesEffBR[i]          = (TH1D*)histoEtaTrueEffPt[i]->Clone(Form("histoEtaAccTimesEffBR%s",nameMeasGlobal[i].Data()));
            histoEtaAccTimesEff[i]->Multiply(histoEtaAcc[i]);
            histoEtaAccTimesEffBR[i]->Multiply(histoEtaAcc[i]);
            histoEtaAccTimesEff[i]->Scale(2*TMath::Pi()*rapidityMeas[i]);
            histoEtaAccTimesEffBR[i]->Scale(2*TMath::Pi()*rapidityMeas[i]*branchingEtaPiPiPi);
            cout << "loaded " << nameMeasGlobal[i] << "acceptance and efficiency" << endl;
            }
        }

        //
        // ─── LOAD OMEGA TO PI0 RATIO HISTOS ──────────────────────────────
        //
        if(doOmegaToPi0){
            histoOmegaToPi0Stat[i]               = (TH1D*)directoryOmega[i]->Get("OmegaToPi0YShiftedStatError");
            graphOmegaToPi0Stat[i]               = (TGraphAsymmErrors*)directoryOmega[i]->Get("graphOmegaToPi0YShiftedStatError");
            graphOmegaToPi0Sys[i]                = (TGraphAsymmErrors*)directoryOmega[i]->Get("OmegaToPi0YShiftedSystError");
            
        }
    }

    // calculation of relative statistical and systematic uncertainties
    // FOR Omega:
    TH1D* statErrorCollection[11];
    TGraphAsymmErrors* sysErrorCollection[11];
    TH1D* statErrorRelCollection[11];
    TGraphAsymmErrors* sysErrorRelCollection[11];
    for (Int_t i = 0; i< 11; i++){
      // initialize all histograms and graphs as NULL
        statErrorCollection[i]                  = NULL;
        sysErrorCollection[i]                   = NULL;
        statErrorRelCollection[i]               = NULL;
        sysErrorRelCollection[i]                = NULL;
        // add available measurements to the collection
        if(i<numbersofmeas && availableMeas[i]){
            statErrorCollection[i]              = (TH1D*)histoOmegaInvCrossSection[i]->Clone(Form("statErr%sOmega",nameMeasGlobal[i].Data()));
            sysErrorCollection[i]               = (TGraphAsymmErrors*)graphOmegaInvCrossSectionSys[i]->Clone(Form("sysErr%sOmega",nameMeasGlobal[i].Data()));
            cout << "calculating Omega relative uncertainties for " << nameMeasGlobal[i] << endl;
        }
        // calculate relative errors for the available measurements
        if (statErrorCollection[i]) statErrorRelCollection[i] = CalculateRelErrUpTH1D( statErrorCollection[i], Form("relativeStatErrorOmega_%s", nameMeasGlobal[i].Data()));
        if (sysErrorCollection[i]) sysErrorRelCollection[i]   = CalculateRelErrUpAsymmGraph( sysErrorCollection[i], Form("relativeSysErrorOmega_%s", nameMeasGlobal[i].Data()));
    }

    statErrorCollection[0]->Print();

    // FOR eta:
    TH1D* statErrorCollectionEta[11];
    TGraphAsymmErrors* sysErrorCollectionEta[11];
    TH1D* statErrorRelCollectionEta[11];
    TGraphAsymmErrors* sysErrorRelCollectionEta[11];

    TH1D* statErrorCollectionEtaGG[11];
    TGraphAsymmErrors* sysErrorCollectionEtaGG[11];
    TH1D* statErrorRelCollectionEtaGG[11];
    TGraphAsymmErrors* sysErrorRelCollectionEtaGG[11];
    if(doEta){
        for (Int_t i = 0; i< 11; i++){
        // initialize all histograms and graphs as NULL
            statErrorCollectionEta[i]                  = NULL;
            sysErrorCollectionEta[i]                   = NULL;
            statErrorRelCollectionEta[i]               = NULL;
            sysErrorRelCollectionEta[i]                = NULL;
            // add available measurements to the collection
            if(i<numbersofmeas && availableMeas[i]){
                statErrorCollectionEta[i]              = (TH1D*)histoEtaInvCrossSection[i]->Clone(Form("statErr%sEta",nameMeasGlobal[i].Data()));
                sysErrorCollectionEta[i]               = (TGraphAsymmErrors*)graphEtaInvCrossSectionSys[i]->Clone(Form("sysErr%sEta",nameMeasGlobal[i].Data()));
            }
            // calculate relative errors for the available measurements
            if (statErrorCollectionEta[i]) statErrorRelCollectionEta[i] = CalculateRelErrUpTH1D( statErrorCollectionEta[i], Form("relativeStatErrorEta_%s", nameMeasGlobal[i].Data()));
            if (sysErrorCollectionEta[i]) sysErrorRelCollectionEta[i]   = CalculateRelErrUpAsymmGraph( sysErrorCollectionEta[i], Form("relativeSysErrorEta_%s", nameMeasGlobal[i].Data()));  
        }
    }

    // calculation of relative statistical and systematic uncertainties for omega/ pi0
    TH1D* statErrorCollectionOmegaToPi0[11];
    TGraphAsymmErrors* sysErrorCollectionOmegaToPi0[11];
    TH1D* statErrorRelCollectionOmegaToPi0[11];
    TGraphAsymmErrors* sysErrorRelCollectionOmegaToPi0[11];
    if(doOmegaToPi0){
        for (Int_t i = 0; i< 11; i++){
        // initialize all histograms and graphs as NULL
            statErrorCollectionOmegaToPi0[i]                  = NULL;
            sysErrorCollectionOmegaToPi0[i]                   = NULL;
            statErrorRelCollectionOmegaToPi0[i]               = NULL;
            sysErrorRelCollectionOmegaToPi0[i]                = NULL;
            // add available measurements to the collectionOmegaToPi0
            if(i<numbersofmeas && availableMeas[i]){
                statErrorCollectionOmegaToPi0[i]              = (TH1D*)histoOmegaToPi0Stat[i]->Clone(Form("statErr%sOmegaToPi0",nameMeasGlobal[i].Data()));
                sysErrorCollectionOmegaToPi0[i]               = (TGraphAsymmErrors*)graphOmegaToPi0Sys[i]->Clone(Form("sysErr%sOmegaToPi0",nameMeasGlobal[i].Data()));
                cout << "calculating Omega relative uncertainties for " << nameMeasGlobal[i] << endl;
            }
            // calculate relative errors for the available measurements
            if (statErrorCollectionOmegaToPi0[i]) statErrorRelCollectionOmegaToPi0[i] = CalculateRelErrUpTH1D( statErrorCollectionOmegaToPi0[i], Form("relativeStatErrorOmegaToPi0_%s", nameMeasGlobal[i].Data()));
            if (sysErrorCollectionOmegaToPi0[i]) {
                sysErrorRelCollectionOmegaToPi0[i]   = CalculateRelErrUpAsymmGraph( sysErrorCollectionOmegaToPi0[i], Form("relativeSysErrorOmegaToPi0_%s", nameMeasGlobal[i].Data()));
                cout << "printing for i = " << i << endl;
                sysErrorCollectionOmegaToPi0[i]->Print();
                sysErrorRelCollectionOmegaToPi0[i]->Print();
            }
        
        }
        statErrorCollectionOmegaToPi0[0]->Print();
    }


    // *******************************************************************************************************
    // ************************** Combination of different measurements **************************************
    // *******************************************************************************************************

    // Int_t nBinsOmega = 15;
    Int_t nBinsOmega = 17;
    // Double_t xPtLimits[18]                      =  {0,1,1.4,1.6, 1.8, 2.0,3.0,4,5.,6.,8.
    //                                                 ,10,12,14,16,18,20,22};

    Double_t xPtLimits[18]                      =  {0,1,1.4,1.6, 1.8, 2.0,3.0,4,5.,6.,8.
                                                    ,10,12,14,17,18,20,22};

    Int_t nBinsEta = 16;
    Double_t xPtLimitsEta[16]                   =  { 0.0,1., 1.2,1.4, 1.5, 2.0, 2.5, 3.0,
                                                     3.5, 4.0, 5.0, 6.0, 8.0, 12.0};

    Double_t xPtLimitsOmegaToPi0[15]                      =  {0,1,1.4,1.6, 1.8, 2.0,3.0,4,5.,6.,8.
                                                    ,10,12,14,17};
    // Double_t xPtLimitsOmegaToPi0[16]                      =  {0,1,1.4,1.6, 1.8, 2.0, 2.5,3.0,3.5,4,5.,6.
    //                                                 ,10,14,90,100};


    // *******************************************************************************************************
    // ************************** Combination of different measurements **************************************
    // *******************************************************************************************************
    // REMARKS:
    //       - order of measurements defined in CombinePtPointsSpectraFullCorrMat from CombinationFunctions.h
    //       - correlations are defined in CombinePtPointsSpectraFullCorrMat from CombinationFunctions.h
    //       - currently only PCM-EMCAL vs others fully implemeted energy independent
    //       - extendable to other energies
    //       - offsets have to be determined manually, see cout's in shell from combination function

 

    // Definition of offsets for stat & sys see output of function in shell, make sure pt bins match
    //                                            PCM,PHOS,EMC,PCMPHOS,PCMEMC,        EMC
    Int_t offSets[11]                           =  {0,    5,  1,     0,     0, 0,0,0,   6,0,0};
    Int_t offSetsSys[11]                        =  {5,    5, 6,     5,     6, 0,0,0,   6,0,0};


    // needed for later binshifting
    Int_t offSetOmegaShifting[11]     = { 0,  0,  0,  0,  0,
                                        0,  0,  0,  0,  0,
                                        0 };
    Int_t nComBinsOmegaShifting[11]   = { 0,  0,  0,  0,  0,
                                        0,  0,  0,  0,  0,
                                        0 };

    //                                            PCM,PHOS,EMC,PCMPHOS,PCMEMC,         EMC
    Int_t offSetsEta[11]                        =  {0,    2,  2,     0,     2, 0,0,0,   6,0,0};
    Int_t offSetsSysEta[11]                     =  {4,    6, 10,     5,     9, 0,0,0,   6,0,0};


    Int_t offSetEtaShifting[11]     = { 0,  0,  0,  0,  0,
                                        0,  0,  0,  0,  0,
                                        0 };
    Int_t nComBinsEtaShifting[11]   = { 0,  0,  0,  0,  0,
                                        0,  0,  0,  0,  0,
                                        0 };


    // Definition of offsets for stat & sys see output of function in shell, make sure pt bins match
    //                                            PCM,PHOS,EMC,PCMPHOS,PCMEMC,        EMC

    // CHANGE BACK!!!!!
    Int_t offSetsOmegaToPi0[11]                           =  {0,    5,  1,     0,     0, 0,0,0,   6,0,0};
    Int_t offSetsSysOmegaToPi0[11]                        =  {5,    5, 6,     5,     6, 0,0,0,   6,0,0};

    //**********************************************************************************************************************
    //**********************************************************************************************************************
    //**********************************************************************************************************************


    TGraph* graphWeights[11];
    TGraph* graphWeightsEta[11];
    TGraph* graphWeightsOmegaToPi0[11];
    for (Int_t i = 0; i< 11; i++){
        graphWeights[i] = NULL;
        graphWeightsEta[i] = NULL;
        graphWeightsOmegaToPi0[i] = NULL;
    }

    // Declaration & calculation of combined spectrum
    TString fileNameOutputWeighting                       = Form("%s/Weighting.dat",outputDir.Data());
    TString fileNameOutputWeightingEta                    = Form("%s/WeightingEta.dat",outputDir.Data());
    TString fileNameOutputWeightingOmegaToPi0               = Form("%s/WeightingOmegaToPi0.dat",outputDir.Data());

    TGraphAsymmErrors* graphCombOmegaInvXSectionStat= NULL;
    TGraphAsymmErrors* graphCombOmegaInvXSectionSys = NULL;
    cout << " -------------------------------" << endl;
    cout << " Combining omega lol" << endl;
    TGraphAsymmErrors* graphCombOmegaInvXSectionTot = CombinePtPointsSpectraFullCorrMat( statErrorCollection, sysErrorCollection,
                                                                                           xPtLimits, nBinsOmega,
                                                                                           offSets, offSetsSys,
                                                                                           graphCombOmegaInvXSectionStat, graphCombOmegaInvXSectionSys,
                                                                                           fileNameOutputWeighting,"7TeV", "Omega", kTRUE,
                                                                                           0x0, fileNameSysCorrelation,"",40
                                                                                          );
    while (graphCombOmegaInvXSectionTot->GetX()[0] < 2.){
        graphCombOmegaInvXSectionTot->RemovePoint(0);
        cout << "removed first point from tot-graph" << endl;
    }
    while (graphCombOmegaInvXSectionStat->GetX()[0] < 2.){
        graphCombOmegaInvXSectionStat->RemovePoint(0);
        cout << "removed first point from stat-graph" << endl;
    }
    while (graphCombOmegaInvXSectionSys->GetX()[0] < 2.){
        graphCombOmegaInvXSectionSys->RemovePoint(0);
        cout << "removed first point from sys-graph" << endl;
    }

    //return;
    graphCombOmegaInvXSectionStat->Print();
    
    TGraphAsymmErrors* graphCombEtaInvXSectionStat= NULL;
    TGraphAsymmErrors* graphCombEtaInvXSectionSys = NULL;
    TGraphAsymmErrors* graphCombEtaInvXSectionTot = NULL;
    if(doEta){
        graphCombEtaInvXSectionTot = CombinePtPointsSpectraFullCorrMat( statErrorCollectionEta, sysErrorCollectionEta,
                                                                                            xPtLimitsEta, nBinsEta,
                                                                                            offSetsEta, offSetsSysEta,
                                                                                            graphCombEtaInvXSectionStat, graphCombEtaInvXSectionSys,
                                                                                            fileNameOutputWeightingEta,"7TeV", "Eta", kTRUE,
                                                                                            0x0, fileNameSysCorrelation,"",40
                                                                                            );

        graphCombEtaInvXSectionStat->RemovePoint(0);
        graphCombEtaInvXSectionStat->RemovePoint(0);
        graphCombEtaInvXSectionStat->RemovePoint(0);
        graphCombEtaInvXSectionSys->RemovePoint(0);
        graphCombEtaInvXSectionSys->RemovePoint(0);
        graphCombEtaInvXSectionSys->RemovePoint(0);
        graphCombEtaInvXSectionTot->RemovePoint(0);
        graphCombEtaInvXSectionTot->RemovePoint(0);
        graphCombEtaInvXSectionTot->RemovePoint(0);
    //    //return;
        graphCombEtaInvXSectionStat->Print();
    }
   TGraphAsymmErrors* graphCombOmegaToPi0Stat= NULL;
   TGraphAsymmErrors* graphCombOmegaToPi0Sys = NULL;
   TGraphAsymmErrors* graphCombOmegaToPi0Tot = NULL;
   if(doOmegaToPi0){
        graphCombOmegaToPi0Tot = CombinePtPointsSpectraFullCorrMat( statErrorCollectionOmegaToPi0, sysErrorCollectionOmegaToPi0,
                                                                                        xPtLimitsOmegaToPi0, nBinsOmega-1,
                                                                                        offSetsOmegaToPi0, offSetsSysOmegaToPi0,
                                                                                        graphCombOmegaToPi0Stat, graphCombOmegaToPi0Sys,
                                                                                        fileNameOutputWeightingOmegaToPi0,"7TeV", "OmegaToPi0", kTRUE,
                                                                                        0x0, ""
                                                                                    );                                                                                    
        graphCombOmegaToPi0Stat->Print();
        statErrorCollectionOmegaToPi0[0]->Print("all");
        histoOmegaToPi0Stat[0]->Print("all");
        //return;
   }



    Double_t minX                               = 1.5;
    Double_t maxX                               = 22.5;

    Double_t minXSectionOmega               = 1e2;
    Double_t maxXSectionOmega               = 9e8;

    Double_t minPtEta                         = 1.3;
    Double_t maxPtEta                         = 20.5;
    Double_t minXSectionEta               = 1e3;
    Double_t maxXSectionEta               = 9e8;

    //**********************************************************************************************************************
    //**********************************************************************************************************************
    //**********************************************************************************************************************
    // plot weights + unc. for Omega
    //**********************************************************************************************************************
    //**********************************************************************************************************************
    //**********************************************************************************************************************

    // Reading weights from output file for plotting
    ifstream fileWeightsRead;
    fileWeightsRead.open(fileNameOutputWeighting,ios_base::in);
    cout << "reading" << fileNameOutputWeighting << endl;
    Double_t xValuesRead[50];
    Double_t weightsRead[11][50];
    Int_t availableMeasWeight[11]                     = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    Int_t nPtBinsRead                           = 0;
    while(!fileWeightsRead.eof() && nPtBinsRead < 50){
        TString garbage                         = "";
        if (nPtBinsRead == 0){
            fileWeightsRead >> garbage ;
            for (Int_t i = 0; i < nMeasSet; i++){
                fileWeightsRead >> availableMeasWeight[i] ;
            }
            cout << "read following measurements: ";
            for (Int_t i = 0; i < 11; i++){
                cout << availableMeasWeight[i] << "\t" ;
            }
            cout << endl;
        } else {
            fileWeightsRead >> xValuesRead[nPtBinsRead-1];
            for (Int_t i = 0; i < nMeasSet; i++){
                fileWeightsRead >> weightsRead[availableMeasWeight[i]][nPtBinsRead-1] ;
            }
            cout << "read: "<<  nPtBinsRead << "\t"<< xValuesRead[nPtBinsRead-1] << "\t" ;
            for (Int_t i = 0; i < nMeasSet; i++){
                cout << weightsRead[availableMeasWeight[i]][nPtBinsRead-1] << "\t";
            }
            cout << endl;
        }
        nPtBinsRead++;
    }
    nPtBinsRead = nPtBinsRead-2 ;
    fileWeightsRead.close();

    for (Int_t i = 0; i < nMeasSet; i++){
        graphWeights[availableMeasWeight[i]]  = new TGraph(nPtBinsRead,xValuesRead,weightsRead[availableMeasWeight[i]]);
        Int_t bin = 0;
        for (Int_t n = 0; n< nPtBinsRead; n++){
            if (graphWeights[availableMeasWeight[i]]->GetY()[bin] == 0) graphWeights[availableMeasWeight[i]]->RemovePoint(bin);
            else bin++;
        }
    }

    //**********************************************************************************************************************
    //******************************************* Plotting weights for Omega  ************************************************
    //**********************************************************************************************************************

    Int_t textSizeLabelsPixel                   = 900*0.04;
    Double_t textSizeLabelsRel      = 50./1200;

    TCanvas* canvasWeights = new TCanvas("canvasWeights","",200,10,1350,900);  // gives the page size
    DrawGammaCanvasSettings( canvasWeights, 0.08, 0.02, 0.035, 0.09);
    canvasWeights->SetLogx();

    TH2F * histo2DWeights = new TH2F("histo2DWeights","histo2DWeights",11000,minPtOmega,maxPtOmega,1000,-0.5,1.1);
    SetStyleHistoTH2ForGraphs(histo2DWeights, "#it{p}_{T} (GeV/#it{c})","#omega_{a} for BLUE",0.035,0.04, 0.035,0.04, 1.,1.);
    histo2DWeights->GetXaxis()->SetMoreLogLabels();
    histo2DWeights->GetXaxis()->SetNoExponent();
    canvasWeights->cd();
    histo2DWeights->Draw("copy");

        TLegend* legendAccWeights               = GetAndSetLegend2(0.12, 0.14, 0.45, 0.14+(0.035*nMeasSet*1.35), 32);
        for (Int_t i = 0; i < nMeasSet; i++){
            DrawGammaSetMarkerTGraph(graphWeights[availableMeasWeight[i]],
                                    markerStyleDet[availableMeasWeight[i]],
                                    markerSizeDet[availableMeasWeight[i]]*0.5,
                                    colorDet[availableMeasWeight[i]] ,
                                    colorDet[availableMeasWeight[i]]);
            graphWeights[availableMeasWeight[i]]->Draw("p,same,e1");
            legendAccWeights->AddEntry(graphWeights[availableMeasWeight[i]],nameMeasGlobal[availableMeasWeight[i]],"p");
        }
        legendAccWeights->Draw();
        drawLatexAdd("ALICE this thesis",0.93,0.16+(2*0.05),textSizeLabelsRel,kFALSE,kFALSE,kTRUE);
        drawLatexAdd(collisionSystem7TeV,0.93,0.16+0.05,textSizeLabelsRel,kFALSE,kFALSE,kTRUE);
        drawLatexAdd("#omega #rightarrow #pi^{+}#pi^{-}#pi^{0}",0.93,0.16,textSizeLabelsRel,kFALSE,kFALSE,kTRUE);

//        DrawGammaLines(0.23, 70. , 0.5, 0.5,0.1, kGray, 7);
//        DrawGammaLines(0.23, 70. , 0.4, 0.4,0.1, kGray, 1);
//        DrawGammaLines(0.23, 70. , 0.3, 0.3,0.1, kGray, 7);
//        DrawGammaLines(0.23, 70. , 0.2, 0.2,0.1, kGray, 3);

    canvasWeights->SaveAs(Form("%s/Omega_WeightsMethods.%s",outputDir.Data(),suffix.Data()));


    //*********************************************************************************************************************
    //************************************ Visualize relative errors ******************************************************
    //*********************************************************************************************************************

    TCanvas* canvasRelSysErr                    = new TCanvas("canvasRelSysErr","",200,10,1350,900);  // gives the page size
    DrawGammaCanvasSettings( canvasRelSysErr, 0.08, 0.02, 0.035, 0.09);
    canvasRelSysErr->SetLogx();

    TH2F * histo2DRelSysErr = new TH2F("histo2DRelSysErr","histo2DRelSysErr",11000,minPtOmega,maxPtOmega,1000,0,80.5);
    SetStyleHistoTH2ForGraphs(histo2DRelSysErr, "#it{p}_{T} (GeV/#it{c})","Systematic uncertainty (%)",0.035,0.04, 0.035,0.04, 1.,1.);
    histo2DRelSysErr->GetXaxis()->SetMoreLogLabels();
    histo2DRelSysErr->GetXaxis()->SetNoExponent();
    histo2DRelSysErr->GetYaxis()->SetRangeUser(0,45.5);
    histo2DRelSysErr->Draw("copy");

        TLegend* legendRelSysErr                = GetAndSetLegend2(0.62, 0.94-(0.035*nMeasSet*1.35), 0.95, 0.94, 32);
        for (Int_t i = 0; i < nMeasSet; i++){
            DrawGammaSetMarkerTGraph(sysErrorRelCollection[availableMeasWeight[i]], markerStyleDet[availableMeasWeight[i]], markerSizeDet[availableMeasWeight[i]]*0.5, colorDet[availableMeasWeight[i]],
                                    colorDet[availableMeasWeight[i]]);
            sysErrorRelCollection[availableMeasWeight[i]]->Draw("p,same,e1");
            legendRelSysErr->AddEntry(sysErrorRelCollection[availableMeasWeight[i]],nameMeasGlobal[availableMeasWeight[i]],"p");
        }
        legendRelSysErr->Draw();

        drawLatexAdd("ALICE this thesis",0.93,0.92,textSizeLabelsRel,kFALSE,kFALSE,kTRUE);
        drawLatexAdd(collisionSystem7TeV,0.93,0.87,textSizeLabelsRel,kFALSE,kFALSE,kTRUE);
        drawLatexAdd("#omega #rightarrow #pi^{+}#pi^{-}#pi^{0}",0.93,0.82,textSizeLabelsRel,kFALSE,kFALSE,kTRUE);

    canvasRelSysErr->SaveAs(Form("%s/Omega_RelSysErr.%s",outputDir.Data(),suffix.Data()));

    //*********************************************************************************************************************
    //************************************ Visualize relative errors ******************************************************
    //*********************************************************************************************************************

    TCanvas* canvasRelStatErr                   = new TCanvas("canvasRelStatErr","",200,10,1350,900);  // gives the page size
    DrawGammaCanvasSettings( canvasRelStatErr, 0.08, 0.02, 0.035, 0.09);
    canvasRelStatErr->SetLogx();

    TH2F * histo2DRelStatErr = new TH2F("histo2DRelStatErr","histo2DRelStatErr",11000,minPtOmega,maxPtOmega,1000,0,80.5);
    SetStyleHistoTH2ForGraphs(histo2DRelStatErr, "#it{p}_{T} (GeV/#it{c})","Statistical uncertainty (%)",0.035,0.04, 0.035,0.04, 1.,1.);
    histo2DRelStatErr->GetXaxis()->SetMoreLogLabels();
    histo2DRelStatErr->GetXaxis()->SetNoExponent();
    histo2DRelStatErr->GetYaxis()->SetRangeUser(0,47.5);
    histo2DRelStatErr->Draw("copy");

        TLegend* legendRelStatErr               = GetAndSetLegend2(0.10, 0.4-(0.035*nMeasSet*1.35), 0.41, 0.4, 32);
        for (Int_t i = 0; i < nMeasSet; i++){
                DrawGammaSetMarker(statErrorRelCollection[availableMeasWeight[i]], markerStyleDet[availableMeasWeight[i]], markerSizeDet[availableMeasWeight[i]]*0.5, colorDet[availableMeasWeight[i]] ,
                            colorDet[availableMeasWeight[i]]);
                TGraphAsymmErrors* graphDummy   = new TGraphAsymmErrors(statErrorRelCollection[availableMeasWeight[i]]);
                DrawGammaSetMarkerTGraphAsym(graphDummy, markerStyleDet[availableMeasWeight[i]], markerSizeDet[availableMeasWeight[i]]*0.5, colorDet[availableMeasWeight[i]],
                                    colorDet[availableMeasWeight[i]]);
                graphDummy->Draw("same,p,e1");
                legendRelStatErr->AddEntry(graphDummy,nameMeasGlobal[availableMeasWeight[i]],"p");
        }
        legendRelStatErr->Draw();

        drawLatexAdd("ALICE this thesis",0.93,0.89,textSizeLabelsRel,kFALSE,kFALSE,kTRUE);
        drawLatexAdd(collisionSystem7TeV,0.93,0.84,textSizeLabelsRel,kFALSE,kFALSE,kTRUE);
        drawLatexAdd("#omega #rightarrow #pi^{+}#pi^{-}#pi^{0}",0.93,0.79,textSizeLabelsRel,kFALSE,kFALSE,kTRUE);

    canvasRelStatErr->SaveAs(Form("%s/Omega_RelStatErr.%s",outputDir.Data(),suffix.Data()));


    //**********************************************************************************************************************
    //**********************************************************************************************************************

    TGraphAsymmErrors* graphCombOmegaInvXSectionRelStat     = CalculateRelErrUpAsymmGraph( graphCombOmegaInvXSectionStat, "graphCombOmegaInvXSectionRelStat");
    TGraphAsymmErrors* graphCombOmegaInvXSectionRelSys      = CalculateRelErrUpAsymmGraph( graphCombOmegaInvXSectionSys, "graphCombOmegaInvXSectionRelSys");
    TGraphAsymmErrors* graphCombOmegaInvXSectionRelTot      = CalculateRelErrUpAsymmGraph( graphCombOmegaInvXSectionTot, "graphCombOmegaInvXSectionRelTot");

    TCanvas* canvasRelTotErr                    = new TCanvas("canvasRelTotErr","",200,10,1350,900);  // gives the page size
    DrawGammaCanvasSettings( canvasRelTotErr, 0.08, 0.02, 0.035, 0.09);
    canvasRelTotErr->SetLogx();

    TH2F * histo2DRelTotErr;
    histo2DRelTotErr                            = new TH2F("histo2DRelTotErr","histo2DRelTotErr",11000,minPtOmega,maxPtOmega,1000,0,67.5);
    SetStyleHistoTH2ForGraphs(histo2DRelTotErr, "#it{p}_{T} (GeV/#it{c})","Total uncertainty (%)",0.035,0.04, 0.035,0.04, 1.,1.);
    histo2DRelTotErr->GetXaxis()->SetMoreLogLabels();
    histo2DRelTotErr->GetXaxis()->SetNoExponent();

    histo2DRelTotErr->GetYaxis()->SetRangeUser(0,65.5);
    histo2DRelTotErr->GetYaxis()->SetTitle("Uncertainty (%)");
    histo2DRelTotErr->Draw("copy");

    DrawGammaSetMarkerTGraphAsym(graphCombOmegaInvXSectionRelTot, markerStyleComb, markerSizeComb, colorComb , colorComb);
    graphCombOmegaInvXSectionRelTot->Draw("p,same,e1");
    DrawGammaSetMarkerTGraphAsym(graphCombOmegaInvXSectionRelStat, markerStyleComb, markerSizeComb, colorComb-6 , colorComb-6);
    graphCombOmegaInvXSectionRelStat->Draw("l,x0,same,e1");
    DrawGammaSetMarkerTGraphAsym(graphCombOmegaInvXSectionRelSys, markerStyleComb, markerSizeComb, colorComb+2, colorComb+2);
    graphCombOmegaInvXSectionRelSys->SetLineStyle(7);
    graphCombOmegaInvXSectionRelSys->Draw("l,x0,same,e1");

    TLegend* legendRelTotErr2 = GetAndSetLegend2(0.14, 0.94-(0.035*3*1.35), 0.45, 0.94, 32);
    legendRelTotErr2->AddEntry(graphCombOmegaInvXSectionRelTot,"Total","p");
    legendRelTotErr2->AddEntry(graphCombOmegaInvXSectionRelStat,"Statistical","l");
    legendRelTotErr2->AddEntry(graphCombOmegaInvXSectionRelSys,"Systematic","l");
    legendRelTotErr2->Draw();

    drawLatexAdd("ALICE this thesis",0.93,0.90,textSizeLabelsRel,kFALSE,kFALSE,kTRUE);
    drawLatexAdd(collisionSystem7TeV,0.93,0.85,textSizeLabelsRel,kFALSE,kFALSE,kTRUE);
    drawLatexAdd("#omega #rightarrow #pi^{+}#pi^{-}#pi^{0}",0.93,0.8,textSizeLabelsRel,kFALSE,kFALSE,kTRUE);

    canvasRelTotErr->SaveAs(Form("%s/Omega_Reldecomp.%s",outputDir.Data(),suffix.Data()));

    //**********************************************************************************************************************
    //**********************************************************************************************************************
    //**********************************************************************************************************************
    // plot weights + unc. for eta
    //**********************************************************************************************************************
    //**********************************************************************************************************************
    //**********************************************************************************************************************

    // Reading weights from output file for plotting
    if(doEta){
        ifstream fileWeightsReadEta;
        fileWeightsReadEta.open(fileNameOutputWeightingEta,ios_base::in);
        cout << "reading" << fileNameOutputWeightingEta << endl;
        Double_t xValuesReadEta[50];
        Double_t weightsReadEta[11][50];
        Int_t availableMeasWeightEta[11]                  = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
        Int_t nPtBinsReadEta                        = 0;
        while(!fileWeightsReadEta.eof() && nPtBinsReadEta < 50){
            TString garbage                         = "";
            if (nPtBinsReadEta == 0){
                fileWeightsReadEta >> garbage ;
                for (Int_t i = 0; i < nMeasSet; i++){
                    fileWeightsReadEta >> availableMeasWeightEta[i] ;
                }
                cout << "read following measurements: ";
                for (Int_t i = 0; i < 11; i++){
                    cout << availableMeasWeightEta[i] << "\t" ;
                }
                cout << endl;
            } else {
                fileWeightsReadEta >> xValuesReadEta[nPtBinsReadEta-1];
                for (Int_t i = 0; i < nMeasSet; i++){
                    fileWeightsReadEta >> weightsReadEta[availableMeasWeightEta[i]][nPtBinsReadEta-1] ;
                }
                cout << "read: "<<  nPtBinsReadEta << "\t"<< xValuesReadEta[nPtBinsReadEta-1] << "\t" ;
                for (Int_t i = 0; i < nMeasSet; i++){
                    cout << weightsReadEta[availableMeasWeightEta[i]][nPtBinsReadEta-1] << "\t";
                }
                cout << endl;
            }
            nPtBinsReadEta++;
        }
        nPtBinsReadEta = nPtBinsReadEta-2 ;
        fileWeightsReadEta.close();

        for (Int_t i = 0; i < nMeasSet; i++){
            graphWeightsEta[availableMeasWeightEta[i]]  = new TGraph(nPtBinsReadEta,xValuesReadEta,weightsReadEta[availableMeasWeightEta[i]]);
            Int_t bin = 0;
            for (Int_t n = 0; n< nPtBinsReadEta; n++){
                if (graphWeightsEta[availableMeasWeightEta[i]]->GetY()[bin] == 0) graphWeightsEta[availableMeasWeightEta[i]]->RemovePoint(bin);
                else bin++;
            }
        }
    
        //**********************************************************************************************************************
        //******************************************* Plotting weights for Eta  ************************************************
        //**********************************************************************************************************************

        TCanvas* canvasWeightsEta = new TCanvas("canvasWeightsEta","",200,10,1350,900);  // gives the page size
        DrawGammaCanvasSettings( canvasWeightsEta, 0.08, 0.02, 0.035, 0.09);
        canvasWeightsEta->SetLogx();

        TH2F * histo2DWeightsEta;
        histo2DWeightsEta                              = new TH2F("histo2DWeightsEta","histo2DWeightsEta",11000,minPtEta,maxPtEta,1000,-0.5,1.1);
        SetStyleHistoTH2ForGraphs(histo2DWeightsEta, "#it{p}_{T} (GeV/#it{c})","#omega_{a} for BLUE",0.035,0.04, 0.035,0.04, 1.,1.);
        histo2DWeightsEta->GetXaxis()->SetMoreLogLabels();
        histo2DWeightsEta->GetXaxis()->SetNoExponent();
        canvasWeightsEta->cd();
        histo2DWeightsEta->Draw("copy");

            TLegend* legendAccWeightsEta               = GetAndSetLegend2(0.12, 0.14, 0.45, 0.14+(0.035*nMeasSet*1.35), 32);
            for (Int_t i = 0; i < nMeasSet; i++){
                DrawGammaSetMarkerTGraph(graphWeightsEta[availableMeasWeightEta[i]],
                                        markerStyleDet[availableMeasWeightEta[i]],
                                        markerSizeDet[availableMeasWeightEta[i]]*0.5,
                                        colorDet[availableMeasWeightEta[i]] ,
                                        colorDet[availableMeasWeightEta[i]]);
                graphWeightsEta[availableMeasWeightEta[i]]->Draw("p,same,e1");
                legendAccWeightsEta->AddEntry(graphWeightsEta[availableMeasWeightEta[i]],nameMeasGlobal[availableMeasWeightEta[i]],"p");
            }
            legendAccWeightsEta->Draw();
            drawLatexAdd("ALICE this thesis",0.93,0.16+(2*0.05),textSizeLabelsRel,kFALSE,kFALSE,kTRUE);
            drawLatexAdd(collisionSystem7TeV,0.93,0.16+0.05,textSizeLabelsRel,kFALSE,kFALSE,kTRUE);
            drawLatexAdd("#eta #rightarrow #pi^{+}#pi^{-}#pi^{0}",0.93,0.16,textSizeLabelsRel,kFALSE,kFALSE,kTRUE);

    //        DrawGammaLines(0.23, 70. , 0.5, 0.5,0.1, kGray, 7);
    //        DrawGammaLines(0.23, 70. , 0.4, 0.4,0.1, kGray, 1);
    //        DrawGammaLines(0.23, 70. , 0.3, 0.3,0.1, kGray, 7);
    //        DrawGammaLines(0.23, 70. , 0.2, 0.2,0.1, kGray, 3);

        canvasWeightsEta->SaveAs(Form("%s/Eta_WeightsMethods.%s",outputDir.Data(),suffix.Data()));


        //*********************************************************************************************************************
        //************************************ Visualize relative errors ******************************************************
        //*********************************************************************************************************************

        TCanvas* canvasRelSysErrEta                    = new TCanvas("canvasRelSysErrEta","",200,10,1350,900);  // gives the page size
        DrawGammaCanvasSettings( canvasRelSysErrEta, 0.08, 0.02, 0.035, 0.09);
        canvasRelSysErrEta->SetLogx();

        TH2F * histo2DRelSysErrEta = new TH2F("histo2DRelSysErrEta","histo2DRelSysErrEta",11000,minPtEta,maxPtEta,1000,0,80.5);
        SetStyleHistoTH2ForGraphs(histo2DRelSysErrEta, "#it{p}_{T} (GeV/#it{c})","Systematic uncertainty (%)",0.035,0.04, 0.035,0.04, 1.,1.);
        histo2DRelSysErr->GetXaxis()->SetMoreLogLabels();
        histo2DRelSysErr->GetXaxis()->SetNoExponent();
        histo2DRelSysErr->GetYaxis()->SetRangeUser(0,55.5);
        histo2DRelSysErr->Draw("copy");

            TLegend* legendRelSysErrEta                = GetAndSetLegend2(0.62, 0.94-(0.035*nMeasSet*1.35), 0.95, 0.94, 32);
            for (Int_t i = 0; i < nMeasSet; i++){
                DrawGammaSetMarkerTGraph(sysErrorRelCollectionEta[availableMeasWeightEta[i]], markerStyleDet[availableMeasWeightEta[i]], markerSizeDet[availableMeasWeightEta[i]]*0.5, colorDet[availableMeasWeightEta[i]],
                                        colorDet[availableMeasWeightEta[i]]);
                sysErrorRelCollectionEta[availableMeasWeightEta[i]]->Draw("p,same,e1");
                legendRelSysErrEta->AddEntry(sysErrorRelCollectionEta[availableMeasWeightEta[i]],nameMeasGlobal[availableMeasWeightEta[i]],"p");
            }
            legendRelSysErrEta->Draw();
            drawLatexAdd("ALICE this thesis",0.93,0.92,textSizeLabelsRel,kFALSE,kFALSE,kTRUE);
            drawLatexAdd(collisionSystem7TeV,0.93,0.87,textSizeLabelsRel,kFALSE,kFALSE,kTRUE);
            drawLatexAdd("#eta #rightarrow #pi^{+}#pi^{-}#pi^{0}",0.93,0.82,textSizeLabelsRel,kFALSE,kFALSE,kTRUE);

        canvasRelSysErrEta->SaveAs(Form("%s/Eta_RelSysErr.%s",outputDir.Data(),suffix.Data()));

        //*********************************************************************************************************************
        //************************************ Visualize relative errors ******************************************************
        //*********************************************************************************************************************

        TCanvas* canvasRelStatErrEta                   = new TCanvas("canvasRelStatErrEta","",200,10,1350,900);  // gives the page size
        DrawGammaCanvasSettings( canvasRelStatErrEta, 0.08, 0.02, 0.035, 0.09);
        canvasRelStatErrEta->SetLogx();

        TH2F * histo2DRelStatErrEta = new TH2F("histo2DRelStatErrEta","histo2DRelStatErrEta",11000,minPtEta,maxPtEta,1000,0,80.5);
        SetStyleHistoTH2ForGraphs(histo2DRelStatErrEta, "#it{p}_{T} (GeV/#it{c})","Statistical uncertainty (%)",0.035,0.04, 0.035,0.04, 1.,1.);
        histo2DRelStatErrEta->GetXaxis()->SetMoreLogLabels();
        histo2DRelStatErrEta->GetXaxis()->SetNoExponent();
        histo2DRelStatErrEta->GetYaxis()->SetRangeUser(0,55.5);
        histo2DRelStatErrEta->Draw("copy");

            TLegend* legendRelStatErrEta               = GetAndSetLegend2(0.14, 0.94-(0.035*nMeasSet*1.35), 0.45, 0.94, 32);
            for (Int_t i = 0; i < nMeasSet; i++){
                    DrawGammaSetMarker(statErrorRelCollectionEta[availableMeasWeightEta[i]], markerStyleDet[availableMeasWeightEta[i]], markerSizeDet[availableMeasWeightEta[i]]*0.5, colorDet[availableMeasWeightEta[i]] ,
                                colorDet[availableMeasWeightEta[i]]);
                    TGraphAsymmErrors* graphDummy   = new TGraphAsymmErrors(statErrorRelCollectionEta[availableMeasWeightEta[i]]);
                    DrawGammaSetMarkerTGraphAsym(graphDummy, markerStyleDet[availableMeasWeightEta[i]], markerSizeDet[availableMeasWeightEta[i]]*0.5, colorDet[availableMeasWeightEta[i]],
                                        colorDet[availableMeasWeightEta[i]]);
                    graphDummy->Draw("same,p,e1");
                    legendRelStatErrEta->AddEntry(graphDummy,nameMeasGlobal[availableMeasWeightEta[i]],"p");
            }
            legendRelStatErrEta->Draw();

            drawLatexAdd("ALICE this thesis",0.93,0.92,textSizeLabelsRel,kFALSE,kFALSE,kTRUE);
            drawLatexAdd(collisionSystem7TeV,0.93,0.87,textSizeLabelsRel,kFALSE,kFALSE,kTRUE);
            drawLatexAdd("#eta #rightarrow #pi^{+}#pi^{-}#pi^{0}",0.93,0.82,textSizeLabelsRel,kFALSE,kFALSE,kTRUE);

        canvasRelStatErrEta->SaveAs(Form("%s/Eta_RelStatErr.%s",outputDir.Data(),suffix.Data()));


        //************************************************************************************************************************
        //************************************************************************************************************************

        TGraphAsymmErrors* graphCombEtaInvXSectionRelStat     = CalculateRelErrUpAsymmGraph( graphCombEtaInvXSectionStat, "graphCombEtaInvXSectionRelStat");
        TGraphAsymmErrors* graphCombEtaInvXSectionRelSys      = CalculateRelErrUpAsymmGraph( graphCombEtaInvXSectionSys, "graphCombEtaInvXSectionRelSys");
        TGraphAsymmErrors* graphCombEtaInvXSectionRelTot      = CalculateRelErrUpAsymmGraph( graphCombEtaInvXSectionTot, "graphCombEtaInvXSectionRelTot");

        TCanvas* canvasRelTotErrEta                    = new TCanvas("canvasRelTotErrEta","",200,10,1350,900);  // gives the page size
        DrawGammaCanvasSettings( canvasRelTotErrEta, 0.08, 0.02, 0.035, 0.09);
        canvasRelTotErrEta->SetLogx();

        TH2F * histo2DRelTotErrEta = new TH2F("histo2DRelTotErrEta","histo2DRelTotErrEta",11000,minPtEta,maxPtEta,1000,0,67.5);
        SetStyleHistoTH2ForGraphs(histo2DRelTotErrEta, "#it{p}_{T} (GeV/#it{c})","Total uncertainty (%)",0.035,0.04, 0.035,0.04, 1.,1.);
        histo2DRelTotErrEta->GetXaxis()->SetMoreLogLabels();
        histo2DRelTotErrEta->GetXaxis()->SetNoExponent();

        histo2DRelTotErrEta->GetYaxis()->SetRangeUser(0,65.5);
        histo2DRelTotErrEta->GetYaxis()->SetTitle("Uncertainty (%)");
        histo2DRelTotErrEta->Draw("copy");

        DrawGammaSetMarkerTGraphAsym(graphCombEtaInvXSectionRelTot, markerStyleComb, markerSizeComb, colorComb , colorComb);
        graphCombEtaInvXSectionRelTot->Draw("p,same,e1");
        DrawGammaSetMarkerTGraphAsym(graphCombEtaInvXSectionRelStat, markerStyleComb, markerSizeComb, colorComb-6 , colorComb-6);
        graphCombEtaInvXSectionRelStat->Draw("l,x0,same,e1");
        DrawGammaSetMarkerTGraphAsym(graphCombEtaInvXSectionRelSys, markerStyleComb, markerSizeComb, colorComb+2, colorComb+2);
        graphCombEtaInvXSectionRelSys->SetLineStyle(7);
        graphCombEtaInvXSectionRelSys->Draw("l,x0,same,e1");

        TLegend* legendRelTotErr2Eta = GetAndSetLegend2(0.14, 0.94-(0.035*3*1.35), 0.45, 0.94, 32);
        legendRelTotErr2Eta->AddEntry(graphCombEtaInvXSectionRelTot,"Total","p");
        legendRelTotErr2Eta->AddEntry(graphCombEtaInvXSectionRelStat,"Statistical","l");
        legendRelTotErr2Eta->AddEntry(graphCombEtaInvXSectionRelSys,"Systematic","l");
        legendRelTotErr2Eta->Draw();

        drawLatexAdd("ALICE this thesis",0.93,0.90,textSizeLabelsRel,kFALSE,kFALSE,kTRUE);
        drawLatexAdd(collisionSystem7TeV,0.93,0.85,textSizeLabelsRel,kFALSE,kFALSE,kTRUE);
        drawLatexAdd("#eta #rightarrow #pi^{+}#pi^{-}#pi^{0}",0.93,0.80,textSizeLabelsRel,kFALSE,kFALSE,kTRUE);

        canvasRelTotErrEta->SaveAs(Form("%s/Eta_Reldecomp.%s",outputDir.Data(),suffix.Data()));
    }

    //**********************************************************************************************************************
    //**********************************************************************************************************************
    //**********************************************************************************************************************
    // plot weights + unc. for omega/pi0
    //**********************************************************************************************************************
    //**********************************************************************************************************************
    //**********************************************************************************************************************

    if(doOmegaToPi0){
        // Reading weights from output file for plotting
        ifstream fileWeightsReadOmegaToPi0;
        fileWeightsReadOmegaToPi0.open(fileNameOutputWeightingOmegaToPi0,ios_base::in);
        cout << "reading" << fileNameOutputWeightingOmegaToPi0 << endl;
        Double_t xValuesReadOmegaToPi0[50];
        Double_t weightsReadOmegaToPi0[11][50];
        Int_t availableMeasOmegaToPi0[11]                  = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
        Int_t nPtBinsReadOmegaToPi0                        = 0;
        while(!fileWeightsReadOmegaToPi0.eof() && nPtBinsReadOmegaToPi0 < 50){
            TString garbage                         = "";
            if (nPtBinsReadOmegaToPi0 == 0){
                fileWeightsReadOmegaToPi0 >> garbage ;
                for (Int_t i = 0; i < nMeasSet; i++){
                    fileWeightsReadOmegaToPi0 >> availableMeasOmegaToPi0[i] ;
                }
                cout << "read following measurements: ";
                for (Int_t i = 0; i < 11; i++){
                    cout << availableMeasOmegaToPi0[i] << "\t" ;
                }
                cout << endl;
            } else {
                fileWeightsReadOmegaToPi0 >> xValuesReadOmegaToPi0[nPtBinsReadOmegaToPi0-1];
                for (Int_t i = 0; i < nMeasSet; i++){
                    fileWeightsReadOmegaToPi0 >> weightsReadOmegaToPi0[availableMeasOmegaToPi0[i]][nPtBinsReadOmegaToPi0-1] ;
                }
                cout << "read: "<<  nPtBinsReadOmegaToPi0 << "\t"<< xValuesReadOmegaToPi0[nPtBinsReadOmegaToPi0-1] << "\t" ;
                for (Int_t i = 0; i < nMeasSet; i++){
                    cout << weightsReadOmegaToPi0[availableMeasOmegaToPi0[i]][nPtBinsReadOmegaToPi0-1] << "\t";
                }
                cout << endl;
            }
            nPtBinsReadOmegaToPi0++;
        }
        nPtBinsReadOmegaToPi0 = nPtBinsReadOmegaToPi0-2 ;
        fileWeightsReadOmegaToPi0.close();

        for (Int_t i = 0; i < nMeasSet; i++){
            graphWeightsOmegaToPi0[availableMeasOmegaToPi0[i]]  = new TGraph(nPtBinsReadOmegaToPi0,xValuesReadOmegaToPi0,weightsReadOmegaToPi0[availableMeasOmegaToPi0[i]]);
            Int_t bin = 0;
            for (Int_t n = 0; n< nPtBinsReadOmegaToPi0; n++){
                if (graphWeightsOmegaToPi0[availableMeasOmegaToPi0[i]]->GetY()[bin] == 0) graphWeightsOmegaToPi0[availableMeasOmegaToPi0[i]]->RemovePoint(bin);
                else bin++;
            }
        }

        //**********************************************************************************************************************
        //******************************************* Plotting weights for OmegaToPi0  *****************************************
        //**********************************************************************************************************************

        TCanvas* canvasWeightsOmegaToPi0 = new TCanvas("canvasWeightsOmegaToPi0","",200,10,1350,900);  // gives the page size
        DrawGammaCanvasSettings( canvasWeightsOmegaToPi0, 0.08, 0.02, 0.035, 0.09);
        canvasWeightsOmegaToPi0->SetLogx();

        TH2F * histo2DWeightsOmegaToPi0;
        histo2DWeightsOmegaToPi0                              = new TH2F("histo2DWeightsOmegaToPi0","histo2DWeightsOmegaToPi0",11000,minPtEta,maxPtEta,1000,-0.5,1.1);
        SetStyleHistoTH2ForGraphs(histo2DWeightsOmegaToPi0, "#it{p}_{T} (GeV/#it{c})","#omega_{a} for BLUE",0.035,0.04, 0.035,0.04, 1.,1.);
        histo2DWeightsOmegaToPi0->GetXaxis()->SetMoreLogLabels();
        histo2DWeightsOmegaToPi0->GetXaxis()->SetNoExponent();
        canvasWeightsOmegaToPi0->cd();
        histo2DWeightsOmegaToPi0->Draw("copy");

            TLegend* legendAccWeightsOmegaToPi0               = GetAndSetLegend2(0.12, 0.14, 0.45, 0.14+(0.035*nMeasSet*1.35), 32);
            for (Int_t i = 0; i < nMeasSet; i++){
                DrawGammaSetMarkerTGraph(graphWeightsOmegaToPi0[availableMeasOmegaToPi0[i]],
                                        markerStyleDet[availableMeasOmegaToPi0[i]],
                                        markerSizeDet[availableMeasOmegaToPi0[i]]*0.5,
                                        colorDet[availableMeasOmegaToPi0[i]] ,
                                        colorDet[availableMeasOmegaToPi0[i]]);
                graphWeightsOmegaToPi0[availableMeasOmegaToPi0[i]]->Draw("p,same,e1");
                legendAccWeightsOmegaToPi0->AddEntry(graphWeightsOmegaToPi0[availableMeasOmegaToPi0[i]],nameMeasGlobal[availableMeasOmegaToPi0[i]],"p");
            }
            legendAccWeightsOmegaToPi0->Draw();
            TLatex *labelWeightsEnergyOmegaToPi0              = new TLatex(0.7,0.20,collisionSystem7TeV.Data());
            SetStyleTLatex( labelWeightsEnergyOmegaToPi0, 0.85*textSizeLabelsPixel,4);
            labelWeightsEnergyOmegaToPi0->SetTextFont(43);
            labelWeightsEnergyOmegaToPi0->Draw();
            TLatex *labelWeightsOmegaToPi0                 = new TLatex(0.7,0.16,"#omega/#pi^{0}");
            SetStyleTLatex( labelWeightsOmegaToPi0, 0.85*textSizeLabelsPixel,4);
            labelWeightsOmegaToPi0->SetTextFont(43);
            labelWeightsOmegaToPi0->Draw();

            DrawGammaLines(0.23, 70. , 0.5, 0.5,0.1, kGray, 7);
            DrawGammaLines(0.23, 70. , 0.4, 0.4,0.1, kGray, 1);
            DrawGammaLines(0.23, 70. , 0.3, 0.3,0.1, kGray, 7);
            DrawGammaLines(0.23, 70. , 0.2, 0.2,0.1, kGray, 3);

        canvasWeightsOmegaToPi0->SaveAs(Form("%s/OmegaToPi0_WeightsMethods.%s",outputDir.Data(),suffix.Data()));


        //********************************************************************************************************************
        //************************************ Visualize relative errors ******************************************************
        //*********************************************************************************************************************

        TCanvas* canvasRelSysErrOmegaToPi0                    = new TCanvas("canvasRelSysErrOmegaToPi0","",200,10,1350,900);  // gives the page size
        DrawGammaCanvasSettings( canvasRelSysErrOmegaToPi0, 0.08, 0.02, 0.035, 0.09);
        canvasRelSysErrOmegaToPi0->SetLogx();

        TH2F * histo2DRelSysErrOmegaToPi0 = new TH2F("histo2DRelSysErrOmegaToPi0","histo2DRelSysErrOmegaToPi0",11000,minPtEta,maxPtEta,1000,0,80.5);
        SetStyleHistoTH2ForGraphs(histo2DRelSysErrOmegaToPi0, "#it{p}_{T} (GeV/#it{c})","Systematic uncertainty (%)",0.035,0.04, 0.035,0.04, 1.,1.);
        histo2DRelSysErr->GetXaxis()->SetMoreLogLabels();
        histo2DRelSysErr->GetXaxis()->SetNoExponent();
        histo2DRelSysErr->GetYaxis()->SetRangeUser(0,45.5);
        histo2DRelSysErr->Draw("copy");

            TLegend* legendRelSysErrOmegaToPi0                = GetAndSetLegend2(0.62, 0.94-(0.035*nMeasSet*1.35), 0.95, 0.94, 32);
            cout << "nMeasSet =" << nMeasSet << endl;
            for (Int_t i = 0; i < nMeasSet; i++){
                cout << i << endl;
                cout << "availMeasurement is " << availableMeasOmegaToPi0[i] << endl;
                cout << sysErrorRelCollectionOmegaToPi0[availableMeasOmegaToPi0[i]] << endl;
                sysErrorRelCollectionOmegaToPi0[availableMeasOmegaToPi0[i]]->Print();
                DrawGammaSetMarkerTGraph(sysErrorRelCollectionOmegaToPi0[availableMeasOmegaToPi0[i]], markerStyleDet[availableMeasOmegaToPi0[i]], markerSizeDet[availableMeasOmegaToPi0[i]]*0.5, colorDet[availableMeasOmegaToPi0[i]],
                                        colorDet[availableMeasOmegaToPi0[i]]);
                sysErrorRelCollectionOmegaToPi0[availableMeasOmegaToPi0[i]]->Draw("p,same,e1");
                legendRelSysErrOmegaToPi0->AddEntry(sysErrorRelCollectionOmegaToPi0[availableMeasOmegaToPi0[i]],nameMeasGlobal[availableMeasOmegaToPi0[i]],"p");
            }
            legendRelSysErrOmegaToPi0->Draw();

            TLatex *labelRelSysErrEnergyOmegaToPi0            = new TLatex(0.15,0.89,collisionSystem7TeV.Data());
            SetStyleTLatex( labelRelSysErrEnergyOmegaToPi0, 0.85*textSizeLabelsPixel,4);
            labelRelSysErrEnergyOmegaToPi0->SetTextFont(43);
            labelRelSysErrEnergyOmegaToPi0->Draw();
            TLatex *labelRelSysErrOmegaToPi0               = new TLatex(0.15,0.85,"#omega/#pi^{0}");
            SetStyleTLatex( labelRelSysErrOmegaToPi0, 0.85*textSizeLabelsPixel,4);
            labelRelSysErrOmegaToPi0->SetTextFont(43);
            labelRelSysErrOmegaToPi0->Draw();

        canvasRelSysErrOmegaToPi0->SaveAs(Form("%s/OmegaToPi0_RelSysErr.%s",outputDir.Data(),suffix.Data()));

        //*********************************************************************************************************************
        //************************************ Visualize relative errors ******************************************************
        //*********************************************************************************************************************

        TCanvas* canvasRelStatErrOmegaToPi0                   = new TCanvas("canvasRelStatErrOmegaToPi0","",200,10,1350,900);  // gives the page size
        DrawGammaCanvasSettings( canvasRelStatErrOmegaToPi0, 0.08, 0.02, 0.035, 0.09);
        canvasRelStatErrOmegaToPi0->SetLogx();

        TH2F * histo2DRelStatErrOmegaToPi0 = new TH2F("histo2DRelStatErrOmegaToPi0","histo2DRelStatErrOmegaToPi0",11000,minPtEta,maxPtEta,1000,0,80.5);
        SetStyleHistoTH2ForGraphs(histo2DRelStatErrOmegaToPi0, "#it{p}_{T} (GeV/#it{c})","Statistical uncertainty (%)",0.035,0.04, 0.035,0.04, 1.,1.);
        histo2DRelStatErrOmegaToPi0->GetXaxis()->SetMoreLogLabels();
        histo2DRelStatErrOmegaToPi0->GetXaxis()->SetNoExponent();
        histo2DRelStatErrOmegaToPi0->GetYaxis()->SetRangeUser(0,45.5);
        histo2DRelStatErrOmegaToPi0->Draw("copy");

            TLegend* legendRelStatErrOmegaToPi0               = GetAndSetLegend2(0.14, 0.94-(0.035*nMeasSet*1.35), 0.45, 0.94, 32);
            for (Int_t i = 0; i < nMeasSet; i++){
                    DrawGammaSetMarker(statErrorRelCollectionOmegaToPi0[availableMeasOmegaToPi0[i]], markerStyleDet[availableMeasOmegaToPi0[i]], markerSizeDet[availableMeasOmegaToPi0[i]]*0.5, colorDet[availableMeasOmegaToPi0[i]] ,
                                colorDet[availableMeasOmegaToPi0[i]]);
                    TGraphAsymmErrors* graphDummy   = new TGraphAsymmErrors(statErrorRelCollectionOmegaToPi0[availableMeasOmegaToPi0[i]]);
                    DrawGammaSetMarkerTGraphAsym(graphDummy, markerStyleDet[availableMeasOmegaToPi0[i]], markerSizeDet[availableMeasOmegaToPi0[i]]*0.5, colorDet[availableMeasOmegaToPi0[i]],
                                        colorDet[availableMeasOmegaToPi0[i]]);
                    graphDummy->Draw("same,p,e1");
                    legendRelStatErrOmegaToPi0->AddEntry(graphDummy,nameMeasGlobal[availableMeasOmegaToPi0[i]],"p");
            }
            legendRelStatErrOmegaToPi0->Draw();

            TLatex *labelRelStatErrEnergyOmegaToPi0           = new TLatex(0.75,0.89,collisionSystem7TeV.Data());
            SetStyleTLatex( labelRelStatErrEnergyOmegaToPi0, 0.85*textSizeLabelsPixel,4);
            labelRelStatErrEnergyOmegaToPi0->SetTextFont(43);
            labelRelStatErrEnergyOmegaToPi0->Draw();
            TLatex *labelRelStatErrOmegaToPi0              = new TLatex(0.75,0.85,"#omega/#pi^{0}");
            SetStyleTLatex( labelRelStatErrOmegaToPi0, 0.85*textSizeLabelsPixel,4);
            labelRelStatErrOmegaToPi0->SetTextFont(43);
            labelRelStatErrOmegaToPi0->Draw();

        canvasRelStatErrOmegaToPi0->SaveAs(Form("%s/OmegaToPi0_RelStatErr.%s",outputDir.Data(),suffix.Data()));


        //************************************************************************************************************************
        //************************************************************************************************************************

        TGraphAsymmErrors* graphCombOmegaToPi0RelStat     = CalculateRelErrUpAsymmGraph( graphCombOmegaToPi0Stat, "graphCombOmegaToPi0RelStat");
        TGraphAsymmErrors* graphCombOmegaToPi0RelSys      = CalculateRelErrUpAsymmGraph( graphCombOmegaToPi0Sys, "graphCombOmegaToPi0RelSys");
        TGraphAsymmErrors* graphCombOmegaToPi0RelTot      = CalculateRelErrUpAsymmGraph( graphCombOmegaToPi0Tot, "graphCombOmegaToPi0RelTot");

        TCanvas* canvasRelTotErrOmegaToPi0                    = new TCanvas("canvasRelTotErrOmegaToPi0","",200,10,1350,900);  // gives the page size
        DrawGammaCanvasSettings( canvasRelTotErrOmegaToPi0, 0.08, 0.02, 0.035, 0.09);
        canvasRelTotErrOmegaToPi0->SetLogx();

        TH2F * histo2DRelTotErrOmegaToPi0 = new TH2F("histo2DRelTotErrOmegaToPi0","histo2DRelTotErrOmegaToPi0",11000,minPtEta,maxPtEta,1000,0,57.5);
        SetStyleHistoTH2ForGraphs(histo2DRelTotErrOmegaToPi0, "#it{p}_{T} (GeV/#it{c})","Total uncertainty (%)",0.035,0.04, 0.035,0.04, 1.,1.);
        histo2DRelTotErrOmegaToPi0->GetXaxis()->SetMoreLogLabels();
        histo2DRelTotErrOmegaToPi0->GetXaxis()->SetNoExponent();

        histo2DRelTotErrOmegaToPi0->GetYaxis()->SetRangeUser(0,57.5);
        histo2DRelTotErrOmegaToPi0->GetYaxis()->SetTitle("Uncertainty (%)");
        histo2DRelTotErrOmegaToPi0->Draw("copy");

        DrawGammaSetMarkerTGraphAsym(graphCombOmegaToPi0RelTot, markerStyleComb, markerSizeComb, colorComb , colorComb);
        graphCombOmegaToPi0RelTot->Draw("p,same,e1");
        DrawGammaSetMarkerTGraphAsym(graphCombOmegaToPi0RelStat, markerStyleComb, markerSizeComb, colorComb-6 , colorComb-6);
        graphCombOmegaToPi0RelStat->Draw("l,x0,same,e1");
        DrawGammaSetMarkerTGraphAsym(graphCombOmegaToPi0RelSys, markerStyleComb, markerSizeComb, colorComb+2, colorComb+2);
        graphCombOmegaToPi0RelSys->SetLineStyle(7);
        graphCombOmegaToPi0RelSys->Draw("l,x0,same,e1");

        TLegend* legendRelTotErr2OmegaToPi0 = GetAndSetLegend2(0.14, 0.94-(0.035*3*1.35), 0.45, 0.94, 32);
        legendRelTotErr2OmegaToPi0->AddEntry(graphCombOmegaToPi0RelTot,"Total","p");
        legendRelTotErr2OmegaToPi0->AddEntry(graphCombOmegaToPi0RelStat,"Statistical","l");
        legendRelTotErr2OmegaToPi0->AddEntry(graphCombOmegaToPi0RelSys,"Systematic","l");
        legendRelTotErr2OmegaToPi0->Draw();

        TLatex *labelRelTotErrEnergyOmegaToPi0            = new TLatex(0.75,0.89,collisionSystem7TeV.Data());
        SetStyleTLatex( labelRelTotErrEnergyOmegaToPi0, 0.85*textSizeLabelsPixel,4);
        labelRelTotErrEnergyOmegaToPi0->SetTextFont(43);
        labelRelTotErrEnergyOmegaToPi0->Draw();
        TLatex *labelRelTotErrOmegaToPi0               = new TLatex(0.75,0.85,"#omega/#pi^{0}");
        SetStyleTLatex( labelRelTotErrOmegaToPi0, 0.85*textSizeLabelsPixel,4);
        labelRelTotErrOmegaToPi0->SetTextFont(43);
        labelRelTotErrOmegaToPi0->Draw();

        canvasRelTotErrOmegaToPi0->SaveAs(Form("%s/OmegaToPi0_Reldecomp.%s",outputDir.Data(),suffix.Data()));

    }


    //**********************************************************************************************************************
    //************************************* Calculating bin shifted spectra & fitting **************************************
    //**********************************************************************************************************************
    graphCombOmegaInvXSectionStat->Print();
    // Cloning spectra
    TGraphAsymmErrors* graphCombOmegaInvXSectionTotUnShifted    = (TGraphAsymmErrors*)graphCombOmegaInvXSectionTot->Clone("Unshifted");
    TGraphAsymmErrors* graphCombOmegaInvXSectionStatUnShifted   = (TGraphAsymmErrors*)graphCombOmegaInvXSectionStat->Clone("UnshiftedStat");
    TGraphAsymmErrors* graphCombOmegaInvXSectionSysUnShifted    = (TGraphAsymmErrors*)graphCombOmegaInvXSectionSys->Clone("UnshiftedSys");


    // Calculating binshifts
   // Double_t paramGraph[3]                      = {1.0e12, 8., 0.13};
    Double_t paramGraph[3]                      = {1.86e11, 6.75, 0.13};

    // levy
    TF1* fitInvXSectionOmega              = FitObject("l","fitInvXSectionOmega","Omega",graphCombOmegaInvXSectionTot,2.,22.5,paramGraph,"QNRMEX0+");
    if(bWCorrection.Contains("X")){
        TF1* fitTsallisOmegaPtMult        = FitObject("tmpt","TsallisMultWithPtOmega7TeV","Omega");
        fitTsallisOmegaPtMult->SetParameters(paramGraph[0],paramGraph[1], paramGraph[2]) ; // standard parameter optimize if necessary
        fitTsallisOmegaPtMult->SetRange(2.,22.5);

        cout << "Graph before shifting" << endl;
        graphCombOmegaInvXSectionTot->Print();
        graphCombOmegaInvXSectionTot      = ApplyXshift(graphCombOmegaInvXSectionTot, fitTsallisOmegaPtMult, "Omega", kTRUE);

        cout << "comb" << endl;
        graphCombOmegaInvXSectionStat     = ApplyXshiftIndividualSpectra (graphCombOmegaInvXSectionTot,
                                                                        graphCombOmegaInvXSectionStat,
                                                                        fitTsallisOmegaPtMult,
                                                                        0, graphCombOmegaInvXSectionStat->GetN());
        graphCombOmegaInvXSectionSys      = ApplyXshiftIndividualSpectra (graphCombOmegaInvXSectionTot,
                                                                        graphCombOmegaInvXSectionSys,
                                                                        fitTsallisOmegaPtMult,
                                                                        0, graphCombOmegaInvXSectionSys->GetN());

        TF1* fitTsallisOmegaPtMultFromShift                 = FitObject("tmpt","TsallisMultWithPtOmegaFromShift","Omega");
        fitTsallisOmegaPtMultFromShift->SetRange(2.5,22.);
        fitTsallisOmegaPtMultFromShift->SetParameters(fitTsallisOmegaPtMult->GetParameter(0),fitTsallisOmegaPtMult->GetParameter(1), fitTsallisOmegaPtMult->GetParameter(2));

        TF1* fitTsallisOmegaPtMultFromShiftScaled = new TF1("TsallisMultWithPtOmegaFromShiftScaled","(1/x)*TsallisMultWithPtOmegaFromShift",2.,22.5);

        //***************************************************************************************************************
        //************************************Plotting binshift corrections *********************************************
        //***************************************************************************************************************
        Double_t textSizeLabelsPixel             = 50;
        Double_t textSizeLabelsRel      = 50./1200;

        Double_t minPtOmega = 2.0;
        Double_t maxPtOmega = 22.5;
        Double_t minXSectionOmega = 1e2;
        Double_t maxXSectionOmega = 9e8;
        TCanvas* canvasShift = new TCanvas("canvasShift","",0,0,1000,900);// gives the page size
        DrawGammaCanvasSettings( canvasShift, 0.10, 0.017, 0.015, 0.1);
        canvasShift->SetLogx(1);

        Size_t textSizeSpectra          = 0.04;
        TH1F * histoBinShift = new TH1F("histoBinShift","histoBinShift",1000,minPtOmega, maxPtOmega);
        SetStyleHistoTH1ForGraphs(histoBinShift, "#it{p}_{T} (GeV/#it{c})","bin shifted #it{p}_{T} / no shift",
                                0.85*textSizeSpectra,textSizeSpectra, 0.85*textSizeSpectra,textSizeSpectra, 1.1, 1.2);
        histoBinShift->GetXaxis()->SetMoreLogLabels(1);
        histoBinShift->GetXaxis()->SetNoExponent(kTRUE);
        histoBinShift->GetYaxis()->SetRangeUser(0.95,1.05);
        histoBinShift->DrawCopy();

        TGraphAsymmErrors* graphCombOmegaInvXSectionTotUnShifted_clone = (TGraphAsymmErrors*) graphCombOmegaInvXSectionTotUnShifted->Clone("graphCombOmegaInvXSectionTotUnShifted_clone");

        Int_t numberPoints   = graphCombOmegaInvXSectionTotUnShifted_clone->GetN();
        Double_t *xPoint     = graphCombOmegaInvXSectionTotUnShifted_clone->GetX();
        Double_t* xvalueErrUp  = graphCombOmegaInvXSectionTotUnShifted_clone->GetEXhigh();
        Double_t* xvalueErrLow = graphCombOmegaInvXSectionTotUnShifted_clone->GetEXlow();
        Double_t *xPointShift= graphCombOmegaInvXSectionTot->GetX();
        for (Int_t i=0; i<numberPoints; i++) {
          graphCombOmegaInvXSectionTotUnShifted_clone->SetPoint(i,xPoint[i],xPointShift[i]/xPoint[i]);
          graphCombOmegaInvXSectionTotUnShifted_clone->SetPointError(i,xvalueErrLow[i],xvalueErrUp[i],0,0);
        }
        DrawGammaSetMarkerTGraphAsym(graphCombOmegaInvXSectionTotUnShifted_clone, markerStyleComb, markerSizeComb, colorComb , colorComb);
        graphCombOmegaInvXSectionTotUnShifted_clone->Draw("p same");

        drawLatexAdd("ALICE this thesis",0.93,0.92,textSizeLabelsRel,kFALSE,kFALSE,kTRUE);
        drawLatexAdd(collisionSystem7TeV,0.93,0.87,textSizeLabelsRel,kFALSE,kFALSE,kTRUE);
        drawLatexAdd("#omega #rightarrow #pi^{+}#pi^{-}#pi^{0}",0.93,0.82,textSizeLabelsRel,kFALSE,kFALSE,kTRUE);

        canvasShift->Update();
        canvasShift->SaveAs(Form("%s/BinShiftCorrection_Omega.%s",outputDir.Data(),suffix.Data()));
        canvasShift->SetLogx(0);

        // *************************************************************************************************************
        // Plot control graphs
        // *************************************************************************************************************

        TCanvas* canvasDummy2       = new TCanvas("canvasDummy2","",200,10,1200,1100);  // gives the page size
        DrawGammaCanvasSettings( canvasDummy2,  0.15, 0.01, 0.015, 0.08);
        canvasDummy2->SetLogy();
        canvasDummy2->SetLogx();
        TH2F * histo2DDummy2;
        histo2DDummy2               = new TH2F("histo2DDummy2","histo2DDummy2",1000,minPtOmega, maxPtOmega,1000,minXSectionOmega*0.01,maxXSectionOmega);
        SetStyleHistoTH2ForGraphs(histo2DDummy2, "#it{p}_{T} (GeV/#it{c})","#it{E} #frac{d^{3}#sigma}{d#it{p}^{3}} (pb GeV^{-2} #it{c}^{3} )", 0.032,0.04, 0.04,0.04, 0.8,1.55);
        histo2DDummy2->DrawCopy();

        DrawGammaSetMarkerTGraphAsym(graphCombOmegaInvXSectionStatUnShifted, 20, 1.5, kRed, kRed, widthLinesBoxes, kTRUE);
        graphCombOmegaInvXSectionStatUnShifted->Draw("pEsame");


        TGraphAsymmErrors* test = (TGraphAsymmErrors*) graphCombOmegaInvXSectionStatUnShifted->Clone("test");
        ScaleWithPtGraph(test);
        DrawGammaSetMarkerTGraphAsym(test, 20, 1.5, kGreen+2, kGreen+2, widthLinesBoxes, kTRUE);
        test->Draw("pEsame");


        DrawGammaSetMarkerTGraphAsym(graphCombOmegaInvXSectionStat, 24, 1.5, kBlack, kBlack, widthLinesBoxes, kTRUE);
        graphCombOmegaInvXSectionStat->Draw("pEsame");

        DrawGammaSetMarkerTGraphAsym(graphCombOmegaInvXSectionTot, 24, 1.5, kCyan, kCyan, widthLinesBoxes, kTRUE);
        graphCombOmegaInvXSectionTot->Draw("pEsame");

        fitInvXSectionOmega->SetLineColor(kBlue+2);
        fitInvXSectionOmega->Draw("same");
        fitTsallisOmegaPtMult->SetLineColor(kGreen);
        fitTsallisOmegaPtMult->Draw("same");

        canvasDummy2->Update();
        canvasDummy2->SaveAs(Form("%s/ComparisonShiftedOmega_7TeV.%s",outputDir.Data(),suffix.Data()));
        delete canvasDummy2;
    }
    graphCombOmegaInvXSectionStat->Print();
    TGraphAsymmErrors* graphCombOmegaInvXSectionStat_WOXErr = (TGraphAsymmErrors*) graphCombOmegaInvXSectionStat->Clone("graphCombOmegaInvXSectionStatA_WOXErr");
    ProduceGraphAsymmWithoutXErrors(graphCombOmegaInvXSectionStat_WOXErr);
    TGraphAsymmErrors* graphOmegaInvXSectionStat_WOXErr[10];
    for (Int_t i = 0; i < numbersofmeas; i++){
      if(directoryOmega[i]){
        cout << i << ": is "  << graphOmegaInvCrossSectionStat[i] << endl;
        graphOmegaInvXSectionStat_WOXErr[i] = (TGraphAsymmErrors*) graphOmegaInvCrossSectionStat[i]->Clone(Form("graphOmegaInvXSectionStat_%i_WOXErr",i));
        ProduceGraphAsymmWithoutXErrors(graphOmegaInvXSectionStat_WOXErr[i]);
      }
    }

    // *************************************************************************************************************
    // redo fitting after binshifts
    // *************************************************************************************************************
    // Tsallis function
    graphCombOmegaInvXSectionTot->Fit(fitInvXSectionOmega,"QNRMEX0+","",2.,28.);
    fitInvXSectionOmega           = FitObject("l","fitInvXSectionOmega7TeV","Omega",graphCombOmegaInvXSectionTot,0.3,30.,paramGraph,"QNRMEX0+");
    cout << WriteParameterToFile(fitInvXSectionOmega)<< endl;

    //Two component model from Bylinkin
    Double_t paramTCMOmegaNew[5]  = { graphCombOmegaInvXSectionTot->GetY()[2],0.2,
                                    graphCombOmegaInvXSectionTot->GetY()[3],0.75,3.0};
    TF1* fitTCMInvXSectionOmega        = FitObject("tcm","fitTCMInvXSectionOmega7TeV","Omega",graphCombOmegaInvXSectionTot,0.3,25. ,paramTCMOmegaNew,"QNRMEX0+","", kFALSE);
    cout << WriteParameterToFile(fitTCMInvXSectionOmega)<< endl;

    Double_t paramOmegaPower[3] = {1E11,0.5,6.5};
    TF1* fitPowInvXSectionOmega   = FitObject("powPure","fitPowInvXSectionOmega7TeV","Omega",graphCombOmegaInvXSectionTot,3.5,25. ,paramOmegaPower,"QNRMEX0+","", kFALSE);
    cout << WriteParameterToFile(fitPowInvXSectionOmega)<< endl;

    TF1* fitPowInvXSectionOmegaStat   = FitObject("powPure","fitPowInvXSectionOmega7TeVStat","Omega",graphCombOmegaInvXSectionStat,3.5,25. ,paramOmegaPower,"QNRMEX0+","", kFALSE);
    cout << WriteParameterToFile(fitPowInvXSectionOmegaStat)<< endl;

    Double_t paramOmegaHageDorn[5] = {1E11,0.3,-0.1,0.5,5.95};
    TF1* fitOHagInvYieldOmegaTot   = FitObject("oHag","fitOHagInvYieldOmega7TeV","Omega",graphCombOmegaInvXSectionTot,0.3,25. ,paramOmegaHageDorn,"QNRMEX0+","", kFALSE);
    cout << WriteParameterToFile(fitOHagInvYieldOmegaTot)<< endl;

    fLog << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    fLog << "Omega - Tsallis" << endl;
    fLog << WriteParameterToFile(fitInvXSectionOmega)<< endl;
    fLog << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    fLog << "Omega - TCM" << endl;
    fLog << WriteParameterToFile(fitTCMInvXSectionOmega) << endl;
    fLog << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    fLog << "Omega - Hagedorn" << endl;
    fLog << WriteParameterToFile(fitOHagInvYieldOmegaTot) << endl;
    fLog << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    fLog << "Omega - PowerLaw" << endl;
    fLog << WriteParameterToFile(fitPowInvXSectionOmega) << endl;
    fLog << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    fLog << "Omega - PowerLaw - Stat" << endl;
    fLog << WriteParameterToFile(fitPowInvXSectionOmegaStat) << endl;

    TGraphAsymmErrors* graphRatioCombCombFitTot7TeV 	= (TGraphAsymmErrors*)graphCombOmegaInvXSectionTot->Clone();
    graphRatioCombCombFitTot7TeV						= CalculateGraphErrRatioToFit(graphRatioCombCombFitTot7TeV, fitTCMInvXSectionOmega);
    TGraphAsymmErrors* graphRatioCombCombFitStat7TeV 	= (TGraphAsymmErrors*)  graphCombOmegaInvXSectionStat->Clone();
    graphRatioCombCombFitStat7TeV						= CalculateGraphErrRatioToFit(  graphRatioCombCombFitStat7TeV, fitTCMInvXSectionOmega);
    TGraphAsymmErrors* graphRatioCombCombFitSys7TeV 	= (TGraphAsymmErrors*)graphCombOmegaInvXSectionSys->Clone();
    graphRatioCombCombFitSys7TeV 						= CalculateGraphErrRatioToFit(graphRatioCombCombFitSys7TeV, fitTCMInvXSectionOmega);

    //**********************************************************************************************************************
    //************************************* Calculating bin shifted spectra & fitting for Eta*******************************
    //**********************************************************************************************************************

    TF1* fitTCMInvXSectionEta = NULL;
    TF1* fitInvXSectionEta    = NULL;
    TGraphAsymmErrors* graphCombEtaInvXSectionTotUnShifted    = NULL;
    TGraphAsymmErrors* graphCombEtaInvXSectionStatUnShifted   = NULL;
    TGraphAsymmErrors* graphCombEtaInvXSectionSysUnShifted    = NULL;
    if(doEta){
        // Cloning spectra
        graphCombEtaInvXSectionTotUnShifted    = (TGraphAsymmErrors*)graphCombEtaInvXSectionTot->Clone("Unshifted");
        graphCombEtaInvXSectionStatUnShifted   = (TGraphAsymmErrors*)graphCombEtaInvXSectionStat->Clone("UnshiftedStat");
        graphCombEtaInvXSectionSysUnShifted    = (TGraphAsymmErrors*)graphCombEtaInvXSectionSys->Clone("UnshiftedSys");


        // Calculating binshifts
        //Double_t paramGraph[3]                      = {1.0e12, 8., 0.13};
        Double_t paramGraphEta[3]                     = {1.0e10, 8., 0.13};

        // levy
        fitInvXSectionEta              = FitObject("l","fitInvXSectionEta","Eta",graphCombEtaInvXSectionTot,0.3,20.,paramGraphEta,"QNRMEX0+");

        if(bWCorrection.Contains("X")){
            TF1* fitTsallisEtaPtMult        = FitObject("tmpt","TsallisMultWithPtEta7TeV","Eta");
            fitTsallisEtaPtMult->SetParameters(fitInvXSectionEta->GetParameter(0),fitInvXSectionEta->GetParameter(1), fitInvXSectionEta->GetParameter(2)) ; // standard parameter optimize if necessary

            graphCombEtaInvXSectionTot      = ApplyXshift(graphCombEtaInvXSectionTot, fitTsallisEtaPtMult, "Eta", kTRUE);

            cout << "comb" << endl;
            graphCombEtaInvXSectionStat     = ApplyXshiftIndividualSpectra (graphCombEtaInvXSectionTot,
                                                                            graphCombEtaInvXSectionStat,
                                                                            fitTsallisEtaPtMult,
                                                                            0, graphCombEtaInvXSectionStat->GetN());
            graphCombEtaInvXSectionSys      = ApplyXshiftIndividualSpectra (graphCombEtaInvXSectionTot,
                                                                            graphCombEtaInvXSectionSys,
                                                                            fitTsallisEtaPtMult,
                                                                            0, graphCombEtaInvXSectionSys->GetN());

            TF1* fitTsallisEtaPtMultFromShift                 = FitObject("tmpt","TsallisMultWithPtEtaFromShift","Eta");
            fitTsallisEtaPtMultFromShift->SetRange(0.3,20.);
            fitTsallisEtaPtMultFromShift->SetParameters(fitTsallisEtaPtMult->GetParameter(0),fitTsallisEtaPtMult->GetParameter(1), fitTsallisEtaPtMult->GetParameter(2));

            TF1* fitTsallisEtaPtMultFromShiftScaled = new TF1("TsallisMultWithPtEtaFromShiftScaled","(1/x)*TsallisMultWithPtEtaFromShift",0.3,25.);

            //***************************************************************************************************************
            //************************************Plotting binshift corrections for Eta**************************************
            //***************************************************************************************************************

            Double_t textSizeLabelsPixel             = 50;
            Double_t textSizeLabelsRel      = 50./1200;

            Double_t minPtEta = 1.5;
            Double_t maxPtEta = 20.;
            Double_t minXSectionEta = 1e3;
            Double_t maxXSectionEta = 9e8;
            TCanvas* canvasShiftEta = new TCanvas("canvasShiftEta","",0,0,1000,900);// gives the page size
            DrawGammaCanvasSettings( canvasShiftEta, 0.10, 0.017, 0.015, 0.1);
            canvasShiftEta->SetLogx(1);

            Size_t textSizeSpectra          = 0.04;
            TH1F * histoBinShiftEta = new TH1F("histoBinShiftEta","histoBinShiftEta",1000,minPtEta, maxPtEta);
            SetStyleHistoTH1ForGraphs(histoBinShiftEta, "#it{p}_{T} (GeV/#it{c})","bin shifted #it{p}_{T} / no shift",
                                    0.85*textSizeSpectra,textSizeSpectra, 0.85*textSizeSpectra,textSizeSpectra, 1.1, 1.2);
            histoBinShiftEta->GetXaxis()->SetMoreLogLabels(1);
            histoBinShiftEta->GetXaxis()->SetNoExponent(kTRUE);
            histoBinShiftEta->GetYaxis()->SetRangeUser(0.95,1.05);
            histoBinShiftEta->DrawCopy();

            TGraphAsymmErrors* graphCombEtaInvXSectionTotUnShifted_clone = (TGraphAsymmErrors*) graphCombEtaInvXSectionTotUnShifted->Clone("graphCombEtaInvXSectionTotUnShifted_clone");

            Int_t numberPoints   = graphCombEtaInvXSectionTotUnShifted_clone->GetN();
            Double_t *xPoint     = graphCombEtaInvXSectionTotUnShifted_clone->GetX();
            Double_t* xvalueErrUp  = graphCombEtaInvXSectionTotUnShifted_clone->GetEXhigh();
            Double_t* xvalueErrLow = graphCombEtaInvXSectionTotUnShifted_clone->GetEXlow();
            Double_t *xPointShift= graphCombEtaInvXSectionTot->GetX();
            for (Int_t i=0; i<numberPoints; i++) {
            graphCombEtaInvXSectionTotUnShifted_clone->SetPoint(i,xPoint[i],xPointShift[i]/xPoint[i]);
            graphCombEtaInvXSectionTotUnShifted_clone->SetPointError(i,xvalueErrLow[i],xvalueErrUp[i],0,0);
            }
            DrawGammaSetMarkerTGraphAsym(graphCombEtaInvXSectionTotUnShifted_clone, markerStyleComb, markerSizeComb, colorComb , colorComb);
            graphCombEtaInvXSectionTotUnShifted_clone->Draw("p same");

            drawLatexAdd("ALICE this thesis",0.93,0.92,textSizeLabelsRel,kFALSE,kFALSE,kTRUE);
            drawLatexAdd(collisionSystem7TeV,0.93,0.87,textSizeLabelsRel,kFALSE,kFALSE,kTRUE);
            drawLatexAdd("#eta #rightarrow #pi^{+}#pi^{-}#pi^{0}",0.93,0.82,textSizeLabelsRel,kFALSE,kFALSE,kTRUE);

            canvasShiftEta->Update();
            canvasShiftEta->SaveAs(Form("%s/BinShiftCorrection_Eta.%s",outputDir.Data(),suffix.Data()));
            canvasShiftEta->SetLogx(0);

            // *************************************************************************************************************
            // Plot control graphs
            // *************************************************************************************************************

            TCanvas* canvasDummy2       = new TCanvas("canvasDummy2","",200,10,1200,1100);  // gives the page size
            DrawGammaCanvasSettings( canvasDummy2,  0.15, 0.01, 0.015, 0.08);
            canvasDummy2->SetLogy();
            canvasDummy2->SetLogx();
            TH2F * histo2DDummy2;
            histo2DDummy2               = new TH2F("histo2DDummy2","histo2DDummy2",1000,minPtEta, maxPtEta,1000,minXSectionEta,maxXSectionEta);
            SetStyleHistoTH2ForGraphs(histo2DDummy2, "#it{p}_{T} (GeV/#it{c})","#it{E} #frac{d^{3}#sigma}{d#it{p}^{3}} (pb GeV^{-2} #it{c}^{3} )", 0.032,0.04, 0.04,0.04, 0.8,1.55);
            histo2DDummy2->DrawCopy();

            DrawGammaSetMarkerTGraphAsym(graphCombEtaInvXSectionStatUnShifted, 20, 1.5, kRed, kRed, widthLinesBoxes, kTRUE);
            graphCombEtaInvXSectionStatUnShifted->Draw("pEsame");
            DrawGammaSetMarkerTGraphAsym(graphCombEtaInvXSectionStat, 24, 1.5, kBlack, kBlack, widthLinesBoxes, kTRUE);
            graphCombEtaInvXSectionStat->Draw("pEsame");

            fitInvXSectionEta->SetLineColor(kBlue+2);
            fitInvXSectionEta->Draw("same");

            fitTsallisEtaPtMultFromShiftScaled->SetLineColor(kRed+2);
            fitTsallisEtaPtMultFromShiftScaled->Draw("same");

            canvasDummy2->Update();
            canvasDummy2->SaveAs(Form("%s/ComparisonShiftedEta_7TeV.%s",outputDir.Data(),suffix.Data()));
            delete canvasDummy2;
        }


        TGraphAsymmErrors* graphCombEtaInvXSectionStat_WOXErr = (TGraphAsymmErrors*) graphCombEtaInvXSectionStat->Clone("graphCombEtaInvXSectionStatA_WOXErr");
        ProduceGraphAsymmWithoutXErrors(graphCombEtaInvXSectionStat_WOXErr);

        TGraphAsymmErrors* graphEtaInvXSectionStat_WOXErr[10];
        for (Int_t i = 0; i < numbersofmeas; i++){
        if(directoryEta[i]){
            graphEtaInvXSectionStat_WOXErr[i] = (TGraphAsymmErrors*) graphEtaInvCrossSectionStat[i]->Clone(Form("graphEtaInvXSectionStat_%i_WOXErr",i));
            ProduceGraphAsymmWithoutXErrors(graphEtaInvXSectionStat_WOXErr[i]);
        }
        }

        // *************************************************************************************************************
        // redo fitting after binshifts
        // *************************************************************************************************************
        // Tsallis function
        graphCombEtaInvXSectionTot->Fit(fitInvXSectionEta,"QNRMEX0+","",0.3,25.);
        fitInvXSectionEta           = FitObject("l","fitInvXSectionEta7TeV","Eta",graphCombEtaInvXSectionTot,0.3,25.,paramGraphEta,"QNRMEX0+");
        cout << WriteParameterToFile(fitInvXSectionEta)<< endl;

        //Two component model from Bylinkin
        Double_t paramTCMEtaNew[5]  = { graphCombEtaInvXSectionTot->GetY()[2],0.2,
                                        graphCombEtaInvXSectionTot->GetY()[3],0.75,3.0};
        fitTCMInvXSectionEta        = FitObject("tcm","fitTCMInvXSectionEta7TeV","Eta",graphCombEtaInvXSectionTot,0.3,25. ,paramTCMEtaNew,"QNRMEX0+","", kFALSE);
        cout << WriteParameterToFile(fitTCMInvXSectionEta)<< endl;

        Double_t paramEtaPower[3] = {1E11,0.5,6.5};
        TF1* fitPowInvXSectionEta   = FitObject("powPure","fitPowInvXSectionEta7TeV","Eta",graphCombEtaInvXSectionTot,3.5,25. ,paramEtaPower,"QNRMEX0+","", kFALSE);
        cout << WriteParameterToFile(fitPowInvXSectionEta)<< endl;

        TF1* fitPowInvXSectionEtaStat   = FitObject("powPure","fitPowInvXSectionEta7TeVStat","Eta",graphCombEtaInvXSectionStat,3.5,25. ,paramEtaPower,"QNRMEX0+","", kFALSE);
        cout << WriteParameterToFile(fitPowInvXSectionEtaStat)<< endl;

        Double_t paramEtaHageDorn[5] = {1E11,0.3,-0.1,0.5,5.95};
        TF1* fitOHagInvYieldEtaTot   = FitObject("oHag","fitOHagInvYieldEta7TeV","Eta",graphCombEtaInvXSectionTot,0.3,25. ,paramEtaHageDorn,"QNRMEX0+","", kFALSE);
        cout << WriteParameterToFile(fitOHagInvYieldEtaTot)<< endl;

        fLog << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
        fLog << "Eta - Tsallis" << endl;
        fLog << WriteParameterToFile(fitInvXSectionEta)<< endl;
        fLog << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
        fLog << "Eta - TCM" << endl;
        fLog << WriteParameterToFile(fitTCMInvXSectionEta) << endl;
        fLog << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
        fLog << "Eta - Hagedorn" << endl;
        fLog << WriteParameterToFile(fitOHagInvYieldEtaTot) << endl;
        fLog << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
        fLog << "Eta - PowerLaw" << endl;
        fLog << WriteParameterToFile(fitPowInvXSectionEta) << endl;
        fLog << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
        fLog << "Eta - PowerLaw - Stat" << endl;
        fLog << WriteParameterToFile(fitPowInvXSectionEtaStat) << endl;

        TGraphAsymmErrors* graphRatioCombCombFitTotEta7TeV 	= (TGraphAsymmErrors*)graphCombEtaInvXSectionTot->Clone();
        graphRatioCombCombFitTot7TeV						= CalculateGraphErrRatioToFit(graphRatioCombCombFitTot7TeV, fitTCMInvXSectionEta);
        TGraphAsymmErrors* graphRatioCombCombFitStatEta7TeV 	= (TGraphAsymmErrors*)  graphCombEtaInvXSectionStat->Clone();
        graphRatioCombCombFitStat7TeV						= CalculateGraphErrRatioToFit(  graphRatioCombCombFitStat7TeV, fitTCMInvXSectionEta);
        TGraphAsymmErrors* graphRatioCombCombFitSysEta7TeV 	= (TGraphAsymmErrors*)graphCombEtaInvXSectionSys->Clone();
        graphRatioCombCombFitSys7TeV 						= CalculateGraphErrRatioToFit(graphRatioCombCombFitSys7TeV, fitTCMInvXSectionEta);
    }
    
    //********************************************************************************************************
    // Plotting simple comparison of data vs fit to Omega meson spec
    //********************************************************************************************************
    textSizeLabelsPixel             = 54;
    textSizeLabelsRel      = 54./1200;

    TCanvas* canvasDummy2       = new TCanvas("canvasDummy2","",200,10,1200,1100);  // gives the page size
    DrawGammaCanvasSettings( canvasDummy2,  0.15, 0.01, 0.015, 0.08);
    canvasDummy2->SetLogy();
    canvasDummy2->SetLogx();
    TH2F* histo2DDummy3;
    histo2DDummy3               = new TH2F("histo2DDummy3","histo2DDummy3",1000,minPtOmega,maxPtOmega,1000,minXSectionOmega,maxXSectionOmega);
    SetStyleHistoTH2ForGraphs(histo2DDummy3, "#it{p}_{T} (GeV/#it{c})","#it{E} #frac{d^{3}#sigma}{d#it{p}^{3}} (pb GeV^{-2} #it{c}^{3} )", 0.032,0.04, 0.04,0.04, 0.8,1.55);

    TF1* fitTCMDecomposedOmegaL                 = new TF1("twoCompModel_DecLow",Form("[0]*exp(-(TMath::Sqrt(x*x+%.10f*%.10f)-%.10f)/[1])",mesonMassExpectOmega,mesonMassExpectOmega,mesonMassExpectOmega));
    fitTCMDecomposedOmegaL->SetParameters(fitTCMInvXSectionOmega->GetParameter(0),fitTCMInvXSectionOmega->GetParameter(1));
    fitTCMDecomposedOmegaL->SetRange(minPtOmega,maxPtOmega);
    TF1 *fitTCMDecomposedOmegaH                 = new TF1("twoCompModel_DecH","[0]/(TMath::Power(1+x*x/([1]*[1]*[2]),[2]))");
   //      graphCombEtaInvXSectionTotA->Fit(fitTCMDecomposedH,"QNRMEX0+","",5,20);
    fitTCMDecomposedOmegaH->SetParameters(fitTCMInvXSectionOmega->GetParameter(2),fitTCMInvXSectionOmega->GetParameter(3), fitTCMInvXSectionOmega->GetParameter(4));
    fitTCMDecomposedOmegaH->SetRange(minPtOmega,maxPtOmega);

    histo2DDummy3               = new TH2F("histo2DDummy2","histo2DDummy2",1000,minPtOmega,maxPtOmega,1000,minXSectionOmega,maxXSectionOmega);
    SetStyleHistoTH2ForGraphs(histo2DDummy3, "#it{p}_{T} (GeV/#it{c})","#it{E} #frac{d^{3}#sigma}{d#it{p}^{3}} (pb GeV^{-2} #it{c}^{3} )", 0.032,0.04, 0.04,0.04, 0.8,1.55);
    histo2DDummy3->DrawCopy();

    DrawGammaSetMarkerTGraphAsym(graphCombOmegaInvXSectionStatUnShifted, 20, 1.5, kRed, kRed, widthLinesBoxes, kTRUE);
    graphCombOmegaInvXSectionStatUnShifted->Draw("pEsame");
    DrawGammaSetMarkerTGraphAsym(graphCombOmegaInvXSectionStat, 24, 1.5, kBlack, kBlack, widthLinesBoxes, kTRUE);
    graphCombOmegaInvXSectionStat->Draw("pEsame");

    fitTCMInvXSectionOmega->SetLineColor(kRed+2);
    fitTCMInvXSectionOmega->SetRange(minPtOmega,maxPtOmega);
    fitTCMInvXSectionOmega->Draw("same");

    fitTCMDecomposedOmegaL->SetLineColor(kAzure);
    fitTCMDecomposedOmegaL->SetLineStyle(2);
    fitTCMDecomposedOmegaL->Draw("same");
    fitTCMDecomposedOmegaH->SetLineColor(kGreen+2);
    fitTCMDecomposedOmegaH->SetLineStyle(8);
    fitTCMDecomposedOmegaH->Draw("same");

    TLatex *labelTCMOmega1= new TLatex(0.48, 0.94, Form("TCM low:"));
    TLatex *labelTCMOmega2= new TLatex(0.48, 0.90, Form("A_{1}: (%.1e #pm %.1e) - T_{e}: (%.3f #pm %.3f)",fitTCMInvXSectionOmega->GetParameter(0),fitTCMInvXSectionOmega->GetParError(0),fitTCMInvXSectionOmega->GetParameter(1),fitTCMInvXSectionOmega->GetParError(1)));
    TLatex *labelTCMOmega3= new TLatex(0.48, 0.86, Form("TCM high:"));
    TLatex *labelTCMOmega4= new TLatex(0.48, 0.82, Form("A_{2}: (%.1e #pm %.1e) - T: (%.3f #pm %.3f) - n: (%.3f #pm %.3f)",fitTCMInvXSectionOmega->GetParameter(2),fitTCMInvXSectionOmega->GetParError(2),abs(fitTCMInvXSectionOmega->GetParameter(3)),fitTCMInvXSectionOmega->GetParError(3),fitTCMInvXSectionOmega->GetParameter(4),fitTCMInvXSectionOmega->GetParError(4)));

    TLatex *labelTCMOmega5= new TLatex(0.55, 0.75, Form("Bylinkin-Rostovtsev:"));
    TLatex *labelTCMOmega6= new TLatex(0.55, 0.71, Form("#it{A}_{1} exp(-#it{E}_{T, kin}/#it{T}_{e}) + #it{A}_{2}/#(){1 + #frac{#it{p}_{T}^{2}}{#it{T}^{2}#upoint n}}^{n}"));

    SetStyleTLatex( labelTCMOmega1, 0.03,4);
    labelTCMOmega1->Draw();
    SetStyleTLatex( labelTCMOmega2, 0.02,4);
    labelTCMOmega2->Draw();
    SetStyleTLatex( labelTCMOmega3, 0.03,4);
    labelTCMOmega3->Draw();
    SetStyleTLatex( labelTCMOmega4, 0.02,4);
    labelTCMOmega4->Draw();
    SetStyleTLatex( labelTCMOmega5, 0.03,4);
    labelTCMOmega5->Draw();
    SetStyleTLatex( labelTCMOmega6, 0.03,4);
    labelTCMOmega6->Draw();

    TLatex *labelRelSysErrEnergyC    = new TLatex(0.18,0.94,collisionSystem7TeV.Data());
    SetStyleTLatex( labelRelSysErrEnergyC, 0.85*textSizeLabelsPixel,4);
    labelRelSysErrEnergyC->SetTextFont(43);
    labelRelSysErrEnergyC->Draw();
    TLatex *labelRelSysErrOmegaC       = new TLatex(0.18,0.9,"#omega #rightarrow #pi^{+}#pi^{-}#pi^{0}");
    SetStyleTLatex( labelRelSysErrOmegaC, 0.85*textSizeLabelsPixel,4);
    labelRelSysErrOmegaC->SetTextFont(43);
    labelRelSysErrOmegaC->Draw();

    TLegend* legendWithFit   = GetAndSetLegend2(0.17, 0.14, 0.5, 0.14+(0.035*3), 32);
    legendWithFit->AddEntry(fitTCMDecomposedOmegaL,"TCM low","l");
    legendWithFit->AddEntry(fitTCMDecomposedOmegaH,"TCM high","l");
    legendWithFit->AddEntry(fitTCMInvXSectionOmega,"Bylinkin-Rostovtsev (TCM)","l");
    legendWithFit->Draw();

    canvasDummy2->Update();
    canvasDummy2->Print(Form("%s/ComparisonWithFitOmega_7TeV.%s",outputDir.Data(),suffix.Data()));
    //********************************************************************************************************
    canvasDummy2->Clear();
    histo2DDummy3->DrawCopy();

    graphCombOmegaInvXSectionStatUnShifted->Draw("pEsame");
    graphCombOmegaInvXSectionStat->Draw("pEsame");

    fitInvXSectionOmega->SetLineColor(kRed+2);
    fitInvXSectionOmega->Draw("same");

    TLatex *labelTCMOmega20 = new TLatex(0.35, 0.90, Form("dN/dy: (%.1e #pm %.1e) - n: (%.3f #pm %.3f) - T_{Levy} (GeV/c): (%.3f #pm %.3f)",fitInvXSectionOmega->GetParameter(0),fitInvXSectionOmega->GetParError(0),fitInvXSectionOmega->GetParameter(1),fitInvXSectionOmega->GetParError(1),fitInvXSectionOmega->GetParameter(2),fitInvXSectionOmega->GetParError(2)));
    SetStyleTLatex( labelTCMOmega20, 0.02,4);
    labelTCMOmega20->Draw();

    labelRelSysErrEnergyC->Draw();
    labelRelSysErrOmegaC->Draw();

    TLegend* legendWithFitOmega2   = GetAndSetLegend2(0.17, 0.14, 0.5, 0.14+(0.035*3), 32);
    legendWithFitOmega2->AddEntry(fitInvXSectionOmega,"Tsallis","l");
    legendWithFitOmega2->Draw();

    canvasDummy2->Update();
    canvasDummy2->Print(Form("%s/ComparisonWithFit_Tsallis_Omega_7TeV.%s",outputDir.Data(),suffix.Data()));

    delete canvasDummy2;
    delete histo2DDummy3;

    if(doEta){
        //********************************************************************************************************
        // Plotting simple comparison of data vs fit to Eta meson spec
        //********************************************************************************************************
        textSizeLabelsPixel             = 54;
        textSizeLabelsRel      = 54./1200;

        canvasDummy2       = new TCanvas("canvasDummy2","",200,10,1200,1100);  // gives the page size
        DrawGammaCanvasSettings( canvasDummy2,  0.15, 0.01, 0.015, 0.08);
        canvasDummy2->SetLogy();
        canvasDummy2->SetLogx();
        histo2DDummy3               = new TH2F("histo2DDummy3","histo2DDummy3",1000,minPtEta,maxPtEta,1000,minXSectionEta,maxXSectionEta);
        SetStyleHistoTH2ForGraphs(histo2DDummy3, "#it{p}_{T} (GeV/#it{c})","#it{E} #frac{d^{3}#sigma}{d#it{p}^{3}} (pb GeV^{-2} #it{c}^{3} )", 0.032,0.04, 0.04,0.04, 0.8,1.55);

        TF1* fitTCMDecomposedEtaL                 = new TF1("twoCompModel_DecLow",Form("[0]*exp(-(TMath::Sqrt(x*x+%.10f*%.10f)-%.10f)/[1])",mesonMassExpectEta,mesonMassExpectEta,mesonMassExpectEta));
        fitTCMDecomposedEtaL->SetParameters(fitTCMInvXSectionEta->GetParameter(0),fitTCMInvXSectionEta->GetParameter(1));
        fitTCMDecomposedEtaL->SetRange(minPtEta,maxPtEta);
        TF1 *fitTCMDecomposedEtaH                 = new TF1("twoCompModel_DecH","[0]/(TMath::Power(1+x*x/([1]*[1]*[2]),[2]))");
    //      graphCombEtaInvXSectionTotA->Fit(fitTCMDecomposedH,"QNRMEX0+","",5,20);
        fitTCMDecomposedEtaH->SetParameters(fitTCMInvXSectionEta->GetParameter(2),fitTCMInvXSectionEta->GetParameter(3), fitTCMInvXSectionEta->GetParameter(4));
        fitTCMDecomposedEtaH->SetRange(minPtEta,maxPtEta);

        histo2DDummy3               = new TH2F("histo2DDummy2","histo2DDummy2",1000,minPtEta,maxPtEta,1000,minXSectionEta,maxXSectionEta);
        SetStyleHistoTH2ForGraphs(histo2DDummy3, "#it{p}_{T} (GeV/#it{c})","#it{E} #frac{d^{3}#sigma}{d#it{p}^{3}} (pb GeV^{-2} #it{c}^{3} )", 0.032,0.04, 0.04,0.04, 0.8,1.55);
        histo2DDummy3->DrawCopy();

        DrawGammaSetMarkerTGraphAsym(graphCombEtaInvXSectionStatUnShifted, 20, 1.5, kRed, kRed, widthLinesBoxes, kTRUE);
        graphCombEtaInvXSectionStatUnShifted->Draw("pEsame");
        DrawGammaSetMarkerTGraphAsym(graphCombEtaInvXSectionStat, 24, 1.5, kBlack, kBlack, widthLinesBoxes, kTRUE);
        graphCombEtaInvXSectionStat->Draw("pEsame");

        fitTCMInvXSectionEta->SetLineColor(kRed+2);
        fitTCMInvXSectionEta->SetRange(minPtEta,maxPtEta);
        fitTCMInvXSectionEta->Draw("same");

        fitTCMDecomposedEtaL->SetLineColor(kAzure);
        fitTCMDecomposedEtaL->SetLineStyle(2);
        fitTCMDecomposedEtaL->Draw("same");
        fitTCMDecomposedEtaH->SetLineColor(kGreen+2);
        fitTCMDecomposedEtaH->SetLineStyle(8);
        fitTCMDecomposedEtaH->Draw("same");

        TLatex *labelTCMEta1= new TLatex(0.48, 0.94, Form("TCM low:"));
        TLatex *labelTCMEta2= new TLatex(0.48, 0.90, Form("A_{1}: (%.1e #pm %.1e) - T_{e}: (%.3f #pm %.3f)",fitTCMInvXSectionEta->GetParameter(0),fitTCMInvXSectionEta->GetParError(0),fitTCMInvXSectionEta->GetParameter(1),fitTCMInvXSectionEta->GetParError(1)));
        TLatex *labelTCMEta3= new TLatex(0.48, 0.86, Form("TCM high:"));
        TLatex *labelTCMEta4= new TLatex(0.48, 0.82, Form("A_{2}: (%.1e #pm %.1e) - T: (%.3f #pm %.3f) - n: (%.3f #pm %.3f)",fitTCMInvXSectionEta->GetParameter(2),fitTCMInvXSectionEta->GetParError(2),abs(fitTCMInvXSectionEta->GetParameter(3)),fitTCMInvXSectionEta->GetParError(3),fitTCMInvXSectionEta->GetParameter(4),fitTCMInvXSectionEta->GetParError(4)));

        TLatex *labelTCMEta5= new TLatex(0.55, 0.75, Form("Bylinkin-Rostovtsev:"));
        TLatex *labelTCMEta6= new TLatex(0.55, 0.71, Form("#it{A}_{1} exp(-#it{E}_{T, kin}/#it{T}_{e}) + #it{A}_{2}/#(){1 + #frac{#it{p}_{T}^{2}}{#it{T}^{2}#upoint n}}^{n}"));

        SetStyleTLatex( labelTCMEta1, 0.03,4);
        labelTCMEta1->Draw();
        SetStyleTLatex( labelTCMEta2, 0.02,4);
        labelTCMEta2->Draw();
        SetStyleTLatex( labelTCMEta3, 0.03,4);
        labelTCMEta3->Draw();
        SetStyleTLatex( labelTCMEta4, 0.02,4);
        labelTCMEta4->Draw();
        SetStyleTLatex( labelTCMEta5, 0.03,4);
        labelTCMEta5->Draw();
        SetStyleTLatex( labelTCMEta6, 0.03,4);
        labelTCMEta6->Draw();

        labelRelSysErrEnergyC    = new TLatex(0.18,0.94,collisionSystem7TeV.Data());
        SetStyleTLatex( labelRelSysErrEnergyC, 0.85*textSizeLabelsPixel,4);
        labelRelSysErrEnergyC->SetTextFont(43);
        labelRelSysErrEnergyC->Draw();
        TLatex *labelRelSysErrEtaC       = new TLatex(0.18,0.9,"#eta #rightarrow #pi^{+}#pi^{-}#pi^{0}");
        SetStyleTLatex( labelRelSysErrEtaC, 0.85*textSizeLabelsPixel,4);
        labelRelSysErrEtaC->SetTextFont(43);
        labelRelSysErrEtaC->Draw();

        legendWithFit   = GetAndSetLegend2(0.17, 0.14, 0.5, 0.14+(0.035*3), 32);
        legendWithFit->AddEntry(fitTCMDecomposedEtaL,"TCM low","l");
        legendWithFit->AddEntry(fitTCMDecomposedEtaH,"TCM high","l");
        legendWithFit->AddEntry(fitTCMInvXSectionEta,"Bylinkin-Rostovtsev (TCM)","l");
        legendWithFit->Draw();

        canvasDummy2->Update();
        canvasDummy2->Print(Form("%s/ComparisonWithFitEta_7TeV.%s",outputDir.Data(),suffix.Data()));
        //********************************************************************************************************
        canvasDummy2->Clear();
        histo2DDummy3->DrawCopy();

        graphCombEtaInvXSectionStatUnShifted->Draw("pEsame");
        graphCombEtaInvXSectionStat->Draw("pEsame");

        fitInvXSectionEta->SetLineColor(kRed+2);
        fitInvXSectionEta->Draw("same");

        TLatex *labelTCMEta20 = new TLatex(0.35, 0.90, Form("dN/dy: (%.1e #pm %.1e) - n: (%.3f #pm %.3f) - T_{Levy} (GeV/c): (%.3f #pm %.3f)",fitInvXSectionEta->GetParameter(0),fitInvXSectionEta->GetParError(0),fitInvXSectionEta->GetParameter(1),fitInvXSectionEta->GetParError(1),fitInvXSectionEta->GetParameter(2),fitInvXSectionEta->GetParError(2)));
        SetStyleTLatex( labelTCMEta20, 0.02,4);
        labelTCMEta20->Draw();

        labelRelSysErrEnergyC->Draw();
        labelRelSysErrEtaC->Draw();

        TLegend* legendWithFitEta2   = GetAndSetLegend2(0.17, 0.14, 0.5, 0.14+(0.035*3), 32);
        legendWithFitEta2->AddEntry(fitInvXSectionEta,"Tsallis","l");
        legendWithFitEta2->Draw();

        canvasDummy2->Update();
        canvasDummy2->Print(Form("%s/ComparisonWithFit_Tsallis_Eta_7TeV.%s",outputDir.Data(),suffix.Data()));

        delete canvasDummy2;
        delete histo2DDummy3;
    }

    // **********************************************************************************************************************
    // ******************************************* Calculate Ratio of Comb  to Comb fit*******************************
    // **********************************************************************************************************************

    TGraphAsymmErrors* graphRatioCombCombFitStat   = NULL;
    TGraphAsymmErrors* graphRatioCombCombFitSys    = NULL;

    graphRatioCombCombFitStat                      = (TGraphAsymmErrors*)graphCombOmegaInvXSectionStat->Clone();
    graphRatioCombCombFitStat                      = CalculateGraphErrRatioToFit(graphRatioCombCombFitStat, fitInvXSectionOmega);
    graphRatioCombCombFitSys                       = (TGraphAsymmErrors*)graphCombOmegaInvXSectionSys->Clone();
    graphRatioCombCombFitSys                       = CalculateGraphErrRatioToFit(graphRatioCombCombFitSys, fitInvXSectionOmega);

    // for eta
    TGraphAsymmErrors* graphRatioCombEtaCombFitStat   = NULL;
    TGraphAsymmErrors* graphRatioCombEtaCombFitSys    = NULL;
    if(doEta){    
        graphRatioCombEtaCombFitStat                      = (TGraphAsymmErrors*)graphCombEtaInvXSectionStat->Clone();
        graphRatioCombEtaCombFitStat                      = CalculateGraphErrRatioToFit(graphRatioCombEtaCombFitStat, fitInvXSectionEta);
        graphRatioCombEtaCombFitSys                       = (TGraphAsymmErrors*)graphCombEtaInvXSectionSys->Clone();
        graphRatioCombEtaCombFitSys                       = CalculateGraphErrRatioToFit(graphRatioCombEtaCombFitSys, fitInvXSectionEta);
    }
    // **********************************************************************************************************************
    // ******************************************* Calculate Ratio of Measurements to Comb fit*******************************
    // **********************************************************************************************************************

    TGraphAsymmErrors* graphRatioCombFitStat[11];
    TGraphAsymmErrors* graphRatioCombFitSys[11];
    for (Int_t i = 0; i < 11; i++){
      if(i<numbersofmeas && availableMeas[i]){
        graphRatioCombFitStat[i]                = (TGraphAsymmErrors*)graphOmegaInvCrossSectionStat[i]->Clone();
        graphRatioCombFitStat[i]                = CalculateGraphErrRatioToFit(graphRatioCombFitStat[i], fitInvXSectionOmega);
        graphRatioCombFitSys[i]                 = (TGraphAsymmErrors*)graphOmegaInvCrossSectionSys[i]->Clone();
        graphRatioCombFitSys[i]                 = CalculateGraphErrRatioToFit(graphRatioCombFitSys[i], fitInvXSectionOmega);
      }
    }

    TGraphAsymmErrors* graphRatioCombFitStat_WOXErr[11];
    TGraphAsymmErrors* graphRatioCombFitSys_WOXErr[11];
    for (Int_t i = 0; i < 11; i++){
      if(i<numbersofmeas && availableMeas[i]){
        graphRatioCombFitStat_WOXErr[i] = (TGraphAsymmErrors*) graphRatioCombFitStat[i]->Clone(Form("graphRatioCombFitStat_WOXErr_%i",i));
        ProduceGraphAsymmWithoutXErrors(graphRatioCombFitStat_WOXErr[i]);
        graphRatioCombFitSys_WOXErr[i] = (TGraphAsymmErrors*) graphRatioCombFitSys[i]->Clone(Form("graphRatioCombCombFitStat_WOXErr_%i",i));
        ProduceGraphAsymmWithoutXErrors(graphRatioCombFitSys_WOXErr[i]);
      }
    }

    // Do for Eta
    // ----------------------------------
    TGraphAsymmErrors* graphRatioCombEtaFitStat[11];
    TGraphAsymmErrors* graphRatioCombEtaFitSys[11];
    if(doEta){
        for (Int_t i = 0; i < 11; i++){
        if(i<numbersofmeas && availableMeas[i]){
            graphRatioCombEtaFitStat[i]                = (TGraphAsymmErrors*)graphEtaInvCrossSectionStat[i]->Clone();
            graphRatioCombEtaFitStat[i]                = CalculateGraphErrRatioToFit(graphRatioCombEtaFitStat[i], fitInvXSectionEta);
            graphRatioCombEtaFitSys[i]                 = (TGraphAsymmErrors*)graphEtaInvCrossSectionSys[i]->Clone();
            graphRatioCombEtaFitSys[i]                 = CalculateGraphErrRatioToFit(graphRatioCombEtaFitSys[i], fitInvXSectionEta);
        }
        }

        TGraphAsymmErrors* graphRatioCombEtaFitStat_WOXErr[11];
        TGraphAsymmErrors* graphRatioCombEtaFitSys_WOXErr[11];
        for (Int_t i = 0; i < 11; i++){
        if(i<numbersofmeas && availableMeas[i]){
            graphRatioCombEtaFitStat_WOXErr[i] = (TGraphAsymmErrors*) graphRatioCombEtaFitStat[i]->Clone(Form("graphRatioCombEtaFitStat_WOXErr_%i",i));
            ProduceGraphAsymmWithoutXErrors(graphRatioCombEtaFitStat_WOXErr[i]);
            graphRatioCombEtaFitSys_WOXErr[i] = (TGraphAsymmErrors*) graphRatioCombEtaFitSys[i]->Clone(Form("graphRatioCombCombFitStat_WOXErr_%i",i));
            ProduceGraphAsymmWithoutXErrors(graphRatioCombEtaFitSys_WOXErr[i]);
        }
        }
    }

    // **********************************************************************************************************************
    // ******************************************* Calculate Ratio of Public Note to Comb fit*******************************
    // **********************************************************************************************************************

    TGraphErrors* graphRatioPHOSPublicNoteCombFitStat   = NULL;
    TGraphAsymmErrors* graphRatioPHOSPublicNoteCombFitSys    = NULL;

    graphRatioPHOSPublicNoteCombFitStat                      = (TGraphErrors*)graphOmegaXSecPHOS7TeVStat->Clone();
    graphRatioPHOSPublicNoteCombFitStat                      = CalculateGraphErrRatioToFit(graphRatioPHOSPublicNoteCombFitStat, fitInvXSectionOmega);
    graphRatioPHOSPublicNoteCombFitSys                       = (TGraphAsymmErrors*)graphOmegaXSecPHOS7TeVSys->Clone();
    graphRatioPHOSPublicNoteCombFitSys                       = CalculateGraphErrRatioToFit(graphRatioPHOSPublicNoteCombFitSys, fitInvXSectionOmega);

    // **********************************************************************************************************************
    // ******************************************* Calculate Ratio of published Eta to Comb fit*******************************
    // **********************************************************************************************************************
    TGraphAsymmErrors* graphRatioEtaGGCombFitStat   = NULL;
    TGraphAsymmErrors* graphRatioEtaGGCombFitSys    = NULL;
    if(doEta){

        graphRatioEtaGGCombFitStat                      = (TGraphAsymmErrors*)graphEtaXSecComb7TeVStat ->Clone();
        graphRatioEtaGGCombFitStat                      = CalculateGraphErrRatioToFit(graphRatioEtaGGCombFitStat, fitInvXSectionEta);
        graphRatioEtaGGCombFitSys                       = (TGraphAsymmErrors*)graphEtaXSecComb7TeVSys->Clone();
        graphRatioEtaGGCombFitSys                       = CalculateGraphErrRatioToFit(graphRatioEtaGGCombFitSys, fitInvXSectionEta);
    }

    // **********************************************************************************************************************
    // ******************************************* Calculate Ratio of meson / Pi0 Comb Fit    *******************************
    // **********************************************************************************************************************

//    TGraphAsymmErrors* graphOmegaXSecPi0Comb7TeVStat        = (TGraphAsymmErrors*)fFilePi0Comb7TeV->Get("graphInvCrossSectionPi0Comb7TeVStatErr");
//    TGraphAsymmErrors* graphOmegaXSecPi0Comb7TeVSys         = (TGraphAsymmErrors*)fFilePi0Comb7TeV->Get("graphInvCrossSectionPi0Comb7TeVSysErr");
//    TGraphAsymmErrors* graphOmegaXSecPi0Comb7TeVTot         = (TGraphAsymmErrors*)fFilePi0Comb7TeV->Get("graphInvCrossSectionPi0Comb7TeV");
    // Fit Spectrum with Tsallis

    TGraphAsymmErrors* graphRatioOmegaCombPi0FitStat   = NULL;
    TGraphAsymmErrors* graphRatioOmegaCombPi0FitSys    = NULL;
    TGraphAsymmErrors* graphRatioOmegaCombPi0FitTot    = NULL;

    TGraphAsymmErrors* graphRatioOmegaEMCPi0FitStat   = NULL;
    TGraphAsymmErrors* graphRatioOmegaEMCPi0FitSys    = NULL;
    TGraphAsymmErrors* graphRatioOmegaEMCPi0FitTot    = NULL;

    // Just a test
    TF1* funcRatioOmegaFitToPi0Fit = NULL;
    TF1* funcRatioEtaFitToPi0Fit   = NULL;

    TGraphAsymmErrors* graphRatioEtaCombPi0FitStat   = NULL;
    TGraphAsymmErrors* graphRatioEtaCombPi0FitSys    = NULL;
    TGraphAsymmErrors* graphRatioEtaCombPi0FitTot    = NULL;
    TF1* fitInvXSectionPi0 = NULL;
    if(!usePaperInput){
        // Use Nico output

        // Calculate Ratio
        graphRatioOmegaCombPi0FitStat                      = (TGraphAsymmErrors*)graphCombOmegaInvXSectionStat->Clone();
        graphRatioOmegaCombPi0FitStat                      = CalculateGraphErrRatioToFit(graphRatioOmegaCombPi0FitStat, fitTsallisPi0XSecComb7TeVThesis);
        graphRatioOmegaCombPi0FitSys                       = (TGraphAsymmErrors*)graphCombOmegaInvXSectionSys->Clone();
        graphRatioOmegaCombPi0FitSys                       = CalculateGraphErrRatioToFit(graphRatioOmegaCombPi0FitSys, fitTsallisPi0XSecComb7TeVThesis);
        graphRatioOmegaCombPi0FitTot                       = (TGraphAsymmErrors*)graphCombOmegaInvXSectionTot->Clone();
        graphRatioOmegaCombPi0FitTot                       = CalculateGraphErrRatioToFit(graphRatioOmegaCombPi0FitTot, fitTsallisPi0XSecComb7TeVThesis);

        if(doEta){
            graphRatioEtaCombPi0FitStat                      = (TGraphAsymmErrors*)graphCombEtaInvXSectionStat->Clone();
            graphRatioEtaCombPi0FitStat                      = CalculateGraphErrRatioToFit(graphRatioEtaCombPi0FitStat, fitTsallisPi0XSecComb7TeVThesis);
            graphRatioEtaCombPi0FitSys                       = (TGraphAsymmErrors*)graphCombEtaInvXSectionSys->Clone();
            graphRatioEtaCombPi0FitSys                       = CalculateGraphErrRatioToFit(graphRatioEtaCombPi0FitSys, fitTsallisPi0XSecComb7TeVThesis);
            graphRatioEtaCombPi0FitTot                       = (TGraphAsymmErrors*)graphCombEtaInvXSectionTot->Clone();
            graphRatioEtaCombPi0FitTot                       = CalculateGraphErrRatioToFit(graphRatioEtaCombPi0FitTot, fitTsallisPi0XSecComb7TeVThesis);

            funcRatioEtaFitToPi0Fit                       = DivideTF1(fitInvXSectionEta,fitTsallisPi0XSecComb7TeVThesis,"funcRatioEtaFitToPi0Fit");
        }
        funcRatioOmegaFitToPi0Fit                       = DivideTF1(fitInvXSectionOmega,fitTsallisPi0XSecComb7TeVThesis,"funcRatioOmegaFitToPi0Fit");

    } else{
        // Use Tsallis from paper (parameters set manually)

        fitTsallisPi0XSecComb7TeVThesis->Print();
        cout << fitTsallisPi0XSecComb7TeVThesis->GetParameter(0) << endl;
        cout << fitTsallisPi0XSecComb7TeVThesis->GetParameter(1) << endl;
        cout << fitTsallisPi0XSecComb7TeVThesis->GetParameter(2) << endl;
        TCanvas* c1 = new TCanvas();
        c1->SetLogy();

        fitTsallisPi0XSecComb7TeVThesis->DrawCopy("");
        TF1* tempxsec = (TF1*) fitTsallisPi0XSecComb7TeVThesis->Clone("tempxsec");
        tempxsec->DrawCopy("");
        tempxsec->SetParameter(0,2.4); // A
        tempxsec->SetParError(0,0.15);  // A
        tempxsec->SetParameter(1,6.88); // n
        tempxsec->SetParError(1,0.07);  // n
        tempxsec->SetParameter(2,0.139); // C
        tempxsec->SetParError(2,0.004);  // C

        TF1* fitPho= (TF1*) fitTsallisPi0XSecComb7TeVThesis->Clone("tempxsec");
        tempxsec->DrawCopy("");
        tempxsec->SetParameter(0,2.4); // A
        tempxsec->SetParError(0,0.15);  // A
        tempxsec->SetParameter(1,6.88); // n
        tempxsec->SetParError(1,0.07);  // n
        tempxsec->SetParameter(2,0.139); // C
        tempxsec->SetParError(2,0.004);  // C

        TF1* fitTsallisPi0XSecComb7TeVPaper = ScaleTF1(tempxsec,xSection7TeV*recalcBarn,"fitTsallisPi0XSecComb7TeVPaper");

        TF1* ratio = DivideTF1(fitTsallisPi0XSecComb7TeVPaper,fitTsallisPi0XSecComb7TeVThesis,"ratio");


        fitTsallisPi0XSecComb7TeVPaper->SetLineColor(kRed);
        fitTsallisPi0XSecComb7TeVPaper->DrawCopy("same");
        c1->SaveAs(Form("%s/Pi0_TsallisBeforevsAfter.%s",outputDir.Data(),suffix.Data()));

        TCanvas* c2 = new TCanvas();
        //c2->SetLogy();
        ratio->GetXaxis()->SetRangeUser(1.5,25.);
        ratio->Draw("");
        c2->SaveAs(Form("%s/Pi0_TsallisPaperVsNewRatiio.%s",outputDir.Data(),suffix.Data()));

        // Compare different measurements of pi0 and fit

        // load paper graphs (again)
        TGraphAsymmErrors* graphPi0XSecComb7TeVStatPaper  = (TGraphAsymmErrors*)( (TDirectory*) fFilePi0Comb7TeVPublished->Get("Pi07TeV"))->Get("graphInvCrossSectionPi0Comb7TeVAStatErr");
        TGraphAsymmErrors* graphPi0XSecComb7TeVSysPaper   = (TGraphAsymmErrors*)( (TDirectory*) fFilePi0Comb7TeVPublished->Get("Pi07TeV"))->Get("graphInvCrossSectionPi0Comb7TeVASysErr");
        TGraphAsymmErrors* graphPi0XSecComb7TeVTotPaper   = (TGraphAsymmErrors*)( (TDirectory*) fFilePi0Comb7TeVPublished->Get("Pi07TeV"))->Get("graphInvCrossSectionPi0Comb7TeVA");
       
        // load Nico graphs (again)
        TGraphAsymmErrors* graphPi0XSecComb7TeVStatNico  = (TGraphAsymmErrors*)( (TDirectory*) fFilePi0Comb7TeVNico->Get("Pi07TeV"))->Get("graphInvCrossSectionPi0Comb7TeVAStatErr");
        TGraphAsymmErrors* graphPi0XSecComb7TeVSysNico   = (TGraphAsymmErrors*)( (TDirectory*) fFilePi0Comb7TeVNico->Get("Pi07TeV"))->Get("graphInvCrossSectionPi0Comb7TeVASysErr");
        TGraphAsymmErrors* graphPi0XSecComb7TeVTotNico   = (TGraphAsymmErrors*)( (TDirectory*) fFilePi0Comb7TeVNico->Get("Pi07TeV"))->Get("graphInvCrossSectionPi0Comb7TeVA");

       
       
        TCanvas* comparisonpaper      = new TCanvas("comparisonpaper","",0,0,1200,2000);  // gives the page size
        DrawGammaCanvasSettings( comparisonpaper,  0.18, 0.04, 0.03, 0.06);

        //comparisonpaper->SetLogx(1);
        comparisonpaper->SetLogy(1);
        TH2F* histoDummyCrossSectionPaperComparison  = new TH2F("histoDummyCrossSectionPaperComparison", "histoDummyCrossSectionPaperComparison",1000, minPtOmega,  maxPtOmega, 1000, 1e1, 9e8 );
        SetStyleHistoTH2ForGraphs( histoDummyCrossSectionPaperComparison, "#it{p}_{T} (GeV/#it{c})", "#it{E}#frac{d^{3}#sigma}{d#it{p}^{3}} (pb GeV^{-2}#it{c}^{3})",
                            0.85*textSizeLabelsRel, textSizeLabelsRel, 0.85*textSizeLabelsRel, textSizeLabelsRel, 0.5, 1.4);//(#times #epsilon_{pur})

        histoDummyCrossSectionPaperComparison->GetYaxis()->SetLabelOffset(0.001);
        histoDummyCrossSectionPaperComparison->GetXaxis()->SetLabelOffset(-0.01);
        histoDummyCrossSectionPaperComparison->GetXaxis()->SetMoreLogLabels(kTRUE);
        histoDummyCrossSectionPaperComparison->DrawCopy();
        
        
        DrawGammaSetMarkerTGraphAsym(graphPi0XSecComb7TeVSysPaper, markerStyleDet[0] ,markerSizeDet[0]*0.5, kBlack, kBlack, widthLinesBoxes, kTRUE);
        DrawGammaSetMarkerTGraphAsym(graphPi0XSecComb7TeVStatPaper, markerStyleDet[0] ,markerSizeDet[0]*0.5, kBlack, kBlack);
        graphPi0XSecComb7TeVSysPaper->Draw("E2same");
        graphPi0XSecComb7TeVStatPaper->Draw("p,same,e");

        DrawGammaSetMarkerTGraphAsym(graphPi0XSecComb7TeVSysNico, markerStyleDet[0] ,markerSizeDet[0]*0.5, kRed-2,  kRed-2, widthLinesBoxes, kTRUE);
        DrawGammaSetMarkerTGraphAsym(graphPi0XSecComb7TeVStatNico, markerStyleDet[0] ,markerSizeDet[0]*0.5,  kRed-2,  kRed-2);
        graphPi0XSecComb7TeVSysNico->Draw("E2same");
        graphPi0XSecComb7TeVStatNico->Draw("p,same,e");

        DrawGammaSetMarkerTF1( fitTsallisPi0XSecComb7TeVPaper, 3, 2, kGray+2);
        fitTsallisPi0XSecComb7TeVPaper->Draw("same");

        DrawGammaSetMarkerTF1( fitTsallisPi0XSecComb7TeVThesis, 3, 2, kRed+2);
        fitTsallisPi0XSecComb7TeVThesis->Draw("same");

        TLegend* legendCrossSectionOmegaCombined2           = GetAndSetLegend2(0.2, 0.07, 0.5, 0.04+(4.*textSizeLabelsRel),textSizeLabelsPixel);
        legendCrossSectionOmegaCombined2->AddEntry(graphPi0XSecComb7TeVSysPaper,"paper","fp");
        legendCrossSectionOmegaCombined2->AddEntry(graphPi0XSecComb7TeVSysNico,"Nico","fp");
        legendCrossSectionOmegaCombined2->AddEntry(fitTsallisPi0XSecComb7TeVPaper,"Paper Tsallis fit","l");
        legendCrossSectionOmegaCombined2->AddEntry(fitTsallisPi0XSecComb7TeVThesis,"Nico Tsallis fit","l");
        legendCrossSectionOmegaCombined2->Draw("");

        comparisonpaper->SaveAs(Form("%s/Pi0_PaperVsNewPi0.%s",outputDir.Data(),suffix.Data()));
        cout << __LINE__ << endl;
        // Do Plotting

        graphRatioOmegaCombPi0FitTot                       = (TGraphAsymmErrors*)graphCombOmegaInvXSectionTot->Clone();
        graphRatioOmegaCombPi0FitTot                       = CalculateGraphErrRatioToFit(graphRatioOmegaCombPi0FitTot, fitTsallisPi0XSecComb7TeVPaper);
        graphRatioOmegaCombPi0FitStat                      = (TGraphAsymmErrors*)graphCombOmegaInvXSectionStat->Clone();
        graphRatioOmegaCombPi0FitStat                      = CalculateGraphErrRatioToFit(graphRatioOmegaCombPi0FitStat, fitTsallisPi0XSecComb7TeVPaper);
        graphRatioOmegaCombPi0FitSys                       = (TGraphAsymmErrors*)graphCombOmegaInvXSectionSys->Clone();
        graphRatioOmegaCombPi0FitSys                       = CalculateGraphErrRatioToFit(graphRatioOmegaCombPi0FitSys, fitTsallisPi0XSecComb7TeVPaper);

        // EMC only
        graphRatioOmegaEMCPi0FitStat                      = (TGraphAsymmErrors*)graphOmegaInvCrossSectionStat[2]->Clone();
        graphRatioOmegaEMCPi0FitStat                      = CalculateGraphErrRatioToFit(graphRatioOmegaEMCPi0FitStat, fitTsallisPi0XSecComb7TeVPaper);
        graphRatioOmegaEMCPi0FitSys                       = (TGraphAsymmErrors*)graphOmegaInvCrossSectionSys[2]->Clone();
        graphRatioOmegaEMCPi0FitSys                       = CalculateGraphErrRatioToFit(graphRatioOmegaEMCPi0FitSys, fitTsallisPi0XSecComb7TeVPaper);



        if(doEta){
            graphRatioEtaCombPi0FitStat                      = (TGraphAsymmErrors*)graphCombEtaInvXSectionStat->Clone();
            graphRatioEtaCombPi0FitStat                      = CalculateGraphErrRatioToFit(graphRatioEtaCombPi0FitStat, fitTsallisPi0XSecComb7TeVThesis);
            graphRatioEtaCombPi0FitSys                       = (TGraphAsymmErrors*)graphCombEtaInvXSectionSys->Clone();
            graphRatioEtaCombPi0FitSys                       = CalculateGraphErrRatioToFit(graphRatioEtaCombPi0FitSys, fitTsallisPi0XSecComb7TeVThesis);
            funcRatioEtaFitToPi0Fit                       = DivideTF1(fitInvXSectionEta,fitTsallisPi0XSecComb7TeVThesis,"funcRatioEtaFitToPi0Fit");
        }
        funcRatioOmegaFitToPi0Fit                       = DivideTF1(fitInvXSectionOmega,fitTsallisPi0XSecComb7TeVThesis,"funcRatioOmegaFitToPi0Fit");
    }

    // ***************************************************************************************************************
    // ******************************** fitting omega/pi0 actual measurement******************************************
    // ***************************************************************************************************************

    TF1 *fitOmegaToPi0 = NULL;
    TGraphAsymmErrors* comOmegaPi0 = NULL;
    TGraphAsymmErrors* comOmegaPi0Tot = NULL;

    if(doOmegaToPi0){
        fitOmegaToPi0 = new TF1("fitOmegaToPi0","[0]",3.5,25.);
        fitOmegaToPi0->SetParameter(0,0.48);

        comOmegaPi0 = (TGraphAsymmErrors*) graphCombOmegaToPi0Stat->Clone();
        comOmegaPi0->Fit(fitOmegaToPi0,"QNRMEX0+","",3.5,25.);
        cout << "\n\n\n\n\n++++++++++++++++++++++++++++++++" << endl;
        cout << fitOmegaToPi0->GetParameter(0) << ", +- " << fitOmegaToPi0->GetParError(0) << endl;
        cout << "++++++++++++++++++++++++++++++++\n\n\n\n\n" << endl;

        fLog << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
        fLog << "Omega/Pi0 - Fit pol0: 3.5 < pT < 25.0" << endl;
        fLog << fitOmegaToPi0->GetParameter(0) << ", +- " << fitOmegaToPi0->GetParError(0) << endl;

        comOmegaPi0Tot = (TGraphAsymmErrors*) graphCombOmegaToPi0Tot->Clone();
        comOmegaPi0Tot->Fit(fitOmegaToPi0,"QNRMEX0+","",3.5,25.);
        cout << "\n\n\n\n\n++++++++++++++++++++++++++++++++" << endl;
        cout << fitOmegaToPi0->GetParameter(0) << ", +- " << fitOmegaToPi0->GetParError(0) << endl;
        cout << "++++++++++++++++++++++++++++++++\n\n\n\n\n" << endl;

        fLog << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
        fLog << "Omega/Pi0 - Fit pol0: 3.5 < pT < 25.0" << endl;
        fLog << fitOmegaToPi0->GetParameter(0) << ", +- " << fitOmegaToPi0->GetParError(0) << endl;
    }

    // ***************************************************************************************************************
    // ******************************** fitting Eta/pi0 **************************************************************
    // ***************************************************************************************************************
    if(doEta){
        TF1 *fitEtaToPi0 = new TF1("fitEtaToPi0","[0]",3.5,18.);
        fitEtaToPi0->SetParameter(0,0.48);

        TGraphAsymmErrors* comEtaPi0 = (TGraphAsymmErrors*) graphRatioEtaCombPi0FitStat->Clone();
        comEtaPi0->Fit(fitEtaToPi0,"QNRMEX0+","",3.5,18.);
        cout << "\n\n\n\n\n++++++++++++++++++++++++++++++++" << endl;
        cout << fitEtaToPi0->GetParameter(0) << ", +- " << fitEtaToPi0->GetParError(0) << endl;
        cout << "++++++++++++++++++++++++++++++++\n\n\n\n\n" << endl;

        fLog << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
        fLog << "Eta/Pi0 - Fit pol0: 3.5 < pT < 18.0" << endl;
        fLog << fitEtaToPi0->GetParameter(0) << ", +- " << fitEtaToPi0->GetParError(0) << endl;

        Float_t errorStat = fitEtaToPi0->GetParError(0) ;

        TGraphAsymmErrors* comEtaPi0Tot = (TGraphAsymmErrors*) graphRatioEtaCombPi0FitTot->Clone();
        comEtaPi0Tot->Fit(fitEtaToPi0,"QNRMEX0+","",3.5,18.);
        cout << "\n\n\n\n\n++++++++++++++++++++++++++++++++" << endl;
        cout << fitEtaToPi0->GetParameter(0) << ", +- " << fitEtaToPi0->GetParError(0) << endl;
        cout << "++++++++++++++++++++++++++++++++\n\n\n\n\n" << endl;

        fLog << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
        fLog << "Eta/Pi0 - Fit pol0: 3.5 < pT < 18.0" << endl;
        fLog << fitEtaToPi0->GetParameter(0) << ", +- " << fitEtaToPi0->GetParError(0) << endl;

        Float_t errorTot = fitEtaToPi0->GetParError(0) ;

        cout << "++++++++++++++++++++++++++++++++\n\n\n\n\n" << endl;
        cout << "Eta/pi0 ratio:  " << fitEtaToPi0->GetParameter(0) << "  +- " << errorStat << " (stat) +- " << TMath::Sqrt(pow(errorTot,2)-pow(errorStat,2))<<" (sys) "<<endl;
        cout << "++++++++++++++++++++++++++++++++\n\n\n\n\n" << endl;
    }


    // ***************************************************************************************************************
    // ******************************** fitting omega/pi0 from fit****************************************************
    // ***************************************************************************************************************

    TF1 *fitOmegaToPi0FromFit = new TF1("fitOmegaToPi0FromFit","[0]",3.5,22.);
    fitOmegaToPi0FromFit->SetParameter(0,0.8);

    TGraphAsymmErrors* comOmegaPi0FromFit = (TGraphAsymmErrors*) graphRatioOmegaCombPi0FitStat->Clone();
    comOmegaPi0FromFit->Fit(fitOmegaToPi0FromFit,"QNRMEX0+","",3.5,22.);
    cout << "\n\n\n\n\n++++++++++++++++++++++++++++++++" << endl;
    cout << fitOmegaToPi0FromFit->GetParameter(0) << ", +- " << fitOmegaToPi0FromFit->GetParError(0) << endl;
    cout << "++++++++++++++++++++++++++++++++\n\n\n\n\n" << endl;
    fLog << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    fLog << "omega/Pi0 - Fit pol0: 3.5 < pT < 18.0" << endl;
    fLog << fitOmegaToPi0FromFit->GetParameter(0) << ", +- " << fitOmegaToPi0FromFit->GetParError(0) << endl;
    cout << __LINE__ << endl;
    Double_t errorStat = fitOmegaToPi0FromFit->GetParError(0) ;
    cout << graphRatioOmegaCombPi0FitTot << endl;
    TGraphAsymmErrors* comOmegaPi0FromFitTot = (TGraphAsymmErrors*) graphRatioOmegaCombPi0FitTot->Clone();
    comOmegaPi0FromFitTot->Fit(fitOmegaToPi0FromFit,"QNRMEX0+","",3.5,22.);
    cout << "\n\n\n\n\n++++++++++++++++++++++++++++++++" << endl;
    cout << fitOmegaToPi0FromFit->GetParameter(0) << ", +- " << fitOmegaToPi0FromFit->GetParError(0) << endl;
    cout << "++++++++++++++++++++++++++++++++\n\n\n\n\n" << endl;

    fLog << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    fLog << "omega/Pi0 - Fit pol0: 3.5 < pT < 18.0" << endl;
    fLog << fitOmegaToPi0FromFit->GetParameter(0) << ", +- " << fitOmegaToPi0FromFit->GetParError(0) << endl;

    Double_t errorTot = fitOmegaToPi0FromFit->GetParError(0) ;

    cout << "++++++++++++++++++++++++++++++++\n\n\n\n\n" << endl;
    cout << "omega/pi0 ratio:  " << fitOmegaToPi0FromFit->GetParameter(0) << "  +- " << errorStat << " (stat) +- " << TMath::Sqrt(pow(errorTot,2)-pow(errorStat,2))<<" (sys) "<<endl;
    cout << "++++++++++++++++++++++++++++++++\n\n\n\n\n" << endl;

    // ***************************************************************************************************************
    // ******************************** fitting Eta/pi0 **************************************************************
    // ***************************************************************************************************************
    if(doEta){
        TF1 *fitEtaToPi0 = new TF1("fitEtaToPi0","[0]",3.5,18.);
        fitEtaToPi0->SetParameter(0,0.48);

        TGraphAsymmErrors* comEtaPi0 = (TGraphAsymmErrors*) graphRatioEtaCombPi0FitStat->Clone();
        comEtaPi0->Fit(fitEtaToPi0,"QNRMEX0+","",3.5,18.);
        cout << "\n\n\n\n\n++++++++++++++++++++++++++++++++" << endl;
        cout << fitEtaToPi0->GetParameter(0) << ", +- " << fitEtaToPi0->GetParError(0) << endl;
        cout << "++++++++++++++++++++++++++++++++\n\n\n\n\n" << endl;

        fLog << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
        fLog << "Eta/Pi0 - Fit pol0: 3.5 < pT < 18.0" << endl;
        fLog << fitEtaToPi0->GetParameter(0) << ", +- " << fitEtaToPi0->GetParError(0) << endl;

        errorStat = fitEtaToPi0->GetParError(0) ;

        TGraphAsymmErrors* comEtaPi0Tot = (TGraphAsymmErrors*) graphRatioEtaCombPi0FitTot->Clone();
        comEtaPi0Tot->Fit(fitEtaToPi0,"QNRMEX0+","",3.5,18.);
        cout << "\n\n\n\n\n++++++++++++++++++++++++++++++++" << endl;
        cout << fitEtaToPi0->GetParameter(0) << ", +- " << fitEtaToPi0->GetParError(0) << endl;
        cout << "++++++++++++++++++++++++++++++++\n\n\n\n\n" << endl;

        fLog << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
        fLog << "Eta/Pi0 - Fit pol0: 3.5 < pT < 18.0" << endl;
        fLog << fitEtaToPi0->GetParameter(0) << ", +- " << fitEtaToPi0->GetParError(0) << endl;

        errorTot = fitEtaToPi0->GetParError(0) ;

        cout << "++++++++++++++++++++++++++++++++\n\n\n\n\n" << endl;
        cout << "Eta/pi0 ratio:  " << fitEtaToPi0->GetParameter(0) << "  +- " << errorStat << " (stat) +- " << TMath::Sqrt(pow(errorTot,2)-pow(errorStat,2))<<" (sys) "<<endl;
        cout << "++++++++++++++++++++++++++++++++\n\n\n\n\n" << endl;
    }

    // **********************************************************************************************************************
    // ******************************************* Calculate Ratio of Pythia to Comb fit*******************************
    // **********************************************************************************************************************

    histoOmegaXSecSim7TeV->Sumw2();
    TGraphAsymmErrors* graphRatioPythiaCombDataStat   = NULL;
    TGraphAsymmErrors* graphRatioPythiaCombDataSys    = NULL;

    TH1D* histoRatioPythia8ToFit                     = (TH1D*) histoOmegaXSecSim7TeV->Clone();
    histoRatioPythia8ToFit                           = CalculateHistoRatioToFit (histoRatioPythia8ToFit, fitInvXSectionOmega);
    TGraphErrors* graphRatioPythia8ToFit             = (TGraphErrors*) graphPythia8InvXSectionOmega->Clone();
    graphRatioPythia8ToFit                           = CalculateGraphErrRatioToFit (graphRatioPythia8ToFit, fitInvXSectionOmega);


    // tune 4C
    histoOmegaXSecSimTune4C7TeV->Sumw2();
    TGraphAsymmErrors* graphRatioPythiaTune4CCombDataStat   = NULL;
    TGraphAsymmErrors* graphRatioPythiaTune4CCombDataSys    = NULL;

    // NLO
    TGraphErrors* graphRatioNLOToFit             = (TGraphErrors*) graphNLOInvXSectionOmega->Clone();
    graphRatioNLOToFit->Print();
    fitInvXSectionOmega->Print();
    graphRatioNLOToFit                           = (TGraphErrors*) CalculateGraphRatioToFit(graphRatioNLOToFit, fitInvXSectionOmega);
    TH1D* histoRatioPythia8Tune4CToFit                     = (TH1D*) histoOmegaXSecSimTune4C7TeV->Clone();
    histoRatioPythia8Tune4CToFit                           = CalculateHistoRatioToFit (histoRatioPythia8Tune4CToFit, fitInvXSectionOmega);
    TGraphErrors* graphRatioPythia8Tune4CToFit             = (TGraphErrors*) graphPythia8Tune4CInvXSectionOmega->Clone();
    graphRatioPythia8Tune4CToFit                           = CalculateGraphErrRatioToFit (graphRatioPythia8Tune4CToFit, fitInvXSectionOmega);

    // **********************************************************************************************************************
    // ******************************************* Calculate Pythia to Eta Comb fit*******************************
    // **********************************************************************************************************************
    TGraphAsymmErrors* graphRatioPythiaEtaCombDataStat   = NULL;
    TGraphAsymmErrors* graphRatioPythiaEtaCombDataSys    = NULL;
    if(doEta){
        histoEtaXSecSim7TeV->Sumw2();
        graphRatioPythiaEtaCombDataStat                      = (TGraphAsymmErrors*)graphCombEtaInvXSectionStatUnShifted->Clone();
        graphRatioPythiaEtaCombDataStat                      = CalculateHistoRatioToGraphErr(histoEtaXSecSim7TeV,graphRatioPythiaEtaCombDataStat);
        graphRatioPythiaEtaCombDataSys                    = (TGraphAsymmErrors*)graphCombEtaInvXSectionSysUnShifted->Clone();
        graphRatioPythiaEtaCombDataSys                      = CalculateHistoRatioToGraphErr(histoEtaXSecSim7TeV,graphRatioPythiaEtaCombDataSys);
    }

    // **********************************************************************************************************************
    // ******************************************* Calculate Pythia to Pythia Ratios ******** *******************************
    // **********************************************************************************************************************

    TH1F* pythiaOmegaToPi0Ratio = (TH1F*) histoOmegaXSecSim7TeV->Clone();
    pythiaOmegaToPi0Ratio->Sumw2();
    pythiaOmegaToPi0Ratio->Divide(histoPi0XSecSim7TeV);
    TH1F* pythiaEtaToPi0Ratio = NULL;
    if(doEta){    
        pythiaEtaToPi0Ratio   = (TH1F*) histoEtaXSecSim7TeV->Clone();
        pythiaEtaToPi0Ratio->Sumw2();
        pythiaEtaToPi0Ratio->Divide(histoPi0XSecSim7TeVEtaBinning);
    }

    // tune 4C
    TH1F* pythiaTune4COmegaToPi0Ratio = (TH1F*) histoOmegaXSecSimTune4C7TeV->Clone();
    pythiaTune4COmegaToPi0Ratio->Sumw2();
    pythiaTune4COmegaToPi0Ratio->Divide(histoPi0XSecSimTune4C7TeV);


    // **********************************************************************************************************************
    // *******************************************Plot Ratio of Individual meas to Fit ******************************************
    // **********************************************************************************************************************

    textSizeLabelsPixel                 = 54;
    textSizeLabelsRel      = 54./1200;

    TCanvas* canvasRatioToCombFit       = new TCanvas("canvasRatioToCombFit","",200,10,1350,900);  // gives the page size
    DrawGammaCanvasSettings( canvasRatioToCombFit, 0.08, 0.01, 0.01, 0.125);
    canvasRatioToCombFit->SetLogx();
    canvasRatioToCombFit->cd();

    Double_t textsizeLabelsPP       = 0;
    Double_t textsizeFacPP          = 0;
    if (canvasRatioToCombFit->XtoPixel(canvasRatioToCombFit->GetX2()) <canvasRatioToCombFit->YtoPixel(canvasRatioToCombFit->GetY1()) ){
        textsizeLabelsPP            = (Double_t)textSizeLabelsPixel/canvasRatioToCombFit->XtoPixel(canvasRatioToCombFit->GetX2()) ;
        textsizeFacPP               = (Double_t)1./canvasRatioToCombFit->XtoPixel(canvasRatioToCombFit->GetX2()) ;
    } else {
        textsizeLabelsPP            = (Double_t)textSizeLabelsPixel/canvasRatioToCombFit->YtoPixel(canvasRatioToCombFit->GetY1());
        textsizeFacPP               = (Double_t)1./canvasRatioToCombFit->YtoPixel(canvasRatioToCombFit->GetY1());
    }
    cout << textsizeLabelsPP << endl;
    TH2F * histo2DOmegaRatioToCombFit;
    histo2DOmegaRatioToCombFit               = new TH2F("histo2DOmegaRatioToCombFit","histo2DOmegaRatioToCombFit",1000,minPtOmega, maxPtOmega,1000,0.02,10.);
    SetStyleHistoTH2ForGraphs(histo2DOmegaRatioToCombFit, "#it{p}_{T} (GeV/#it{c})","Data/Tsallis fit", 0.85*textsizeLabelsPP, textsizeLabelsPP,
                              0.85*textsizeLabelsPP,textsizeLabelsPP, 0.9, 0.65, 510, 505);
    histo2DOmegaRatioToCombFit->GetXaxis()->SetMoreLogLabels();
    histo2DOmegaRatioToCombFit->GetYaxis()->SetMoreLogLabels();
    histo2DOmegaRatioToCombFit->GetXaxis()->SetNoExponent(kTRUE);

    histo2DOmegaRatioToCombFit->GetYaxis()->SetRangeUser(0.,2.5);
    histo2DOmegaRatioToCombFit->GetXaxis()->SetRangeUser(minPtOmega,maxPtOmega);
    histo2DOmegaRatioToCombFit->Draw("copy");

     TLegend* legendCrossSectionRatioOmega           = GetAndSetLegend2(0.12, 0.66, 0.37, 0.66+(6.*textSizeLabelsRel),textSizeLabelsPixel);

    for (Int_t i = 0; i < 11; i++){
      if(i<numbersofmeas && availableMeas[i]){
        DrawGammaSetMarkerTGraphAsym(graphRatioCombFitSys[i], markerStyleDet[i] ,markerSizeDet[i]*0.5, colorDet[i], colorDet[i], widthLinesBoxes, kTRUE);
        DrawGammaSetMarkerTGraphAsym(graphRatioCombFitStat[i], markerStyleDet[i] ,markerSizeDet[i]*0.5, colorDet[i], colorDet[i]);

        graphRatioCombFitSys[i]->Draw("E2same");
        graphRatioCombFitStat[i]->Draw("p,same,e");

        legendCrossSectionRatioOmega->AddEntry(graphRatioCombFitSys[i],Form("%s",nameMeasGlobal[i].Data()),"pf");
      }
    }

    DrawGammaLines(minPtOmega,maxPtOmega , 1., 1.,3., kGray+2,7);



    drawLatexAdd("ALICE this thesis",0.93,0.91,textSizeLabelsRel*1.3,kFALSE,kFALSE,kTRUE);
    drawLatexAdd(collisionSystem7TeV,0.93,0.85,textSizeLabelsRel*1.3,kFALSE,kFALSE,kTRUE);
    drawLatexAdd("#omega #rightarrow #pi^{+}#pi^{-}#pi^{0}",0.93,0.80,textSizeLabelsRel*1.3,kFALSE,kFALSE,kTRUE);

    legendCrossSectionRatioOmega->Draw();


    canvasRatioToCombFit->SaveAs(Form("%s/Omega_RatioOfIndividualMeasToCombFit_PP7TeV.%s",outputDir.Data(),suffix.Data()));

    // **********************************************************************************************************************
    // *******************************************Plot Ratio of Individual meas to Fit for eta******************************
    // **********************************************************************************************************************
    if(doEta){
        textSizeLabelsPixel                 = 54;
        textSizeLabelsRel      = 54./1200;

        canvasRatioToCombFit       = new TCanvas("canvasRatioToCombFit","",200,10,1350,900);  // gives the page size
        DrawGammaCanvasSettings( canvasRatioToCombFit, 0.08, 0.01, 0.01, 0.125);
        canvasRatioToCombFit->SetLogx();
        canvasRatioToCombFit->cd();

        textsizeLabelsPP       = 0;
        textsizeFacPP          = 0;
        if (canvasRatioToCombFit->XtoPixel(canvasRatioToCombFit->GetX2()) <canvasRatioToCombFit->YtoPixel(canvasRatioToCombFit->GetY1()) ){
            textsizeLabelsPP            = (Double_t)textSizeLabelsPixel/canvasRatioToCombFit->XtoPixel(canvasRatioToCombFit->GetX2()) ;
            textsizeFacPP               = (Double_t)1./canvasRatioToCombFit->XtoPixel(canvasRatioToCombFit->GetX2()) ;
        } else {
            textsizeLabelsPP            = (Double_t)textSizeLabelsPixel/canvasRatioToCombFit->YtoPixel(canvasRatioToCombFit->GetY1());
            textsizeFacPP               = (Double_t)1./canvasRatioToCombFit->YtoPixel(canvasRatioToCombFit->GetY1());
        }
        cout << textsizeLabelsPP << endl;

        TH2F * histo2DEtaRatioToCombFit;
        histo2DEtaRatioToCombFit               = new TH2F("histo2DEtaRatioToCombFit","histo2DEtaRatioToCombFit",1000,minPtEta, maxPtEta,1000,0.02,10.);
        SetStyleHistoTH2ForGraphs(histo2DEtaRatioToCombFit, "#it{p}_{T} (GeV/#it{c})","Data/Tsallis fit", 0.85*textsizeLabelsPP, textsizeLabelsPP,
                                0.85*textsizeLabelsPP,textsizeLabelsPP, 0.9, 0.65, 510, 505);
        histo2DEtaRatioToCombFit->GetXaxis()->SetMoreLogLabels();
        histo2DEtaRatioToCombFit->GetYaxis()->SetMoreLogLabels();
        histo2DEtaRatioToCombFit->GetXaxis()->SetNoExponent(kTRUE);

        histo2DEtaRatioToCombFit->GetYaxis()->SetRangeUser(0.,5.7);
        histo2DEtaRatioToCombFit->GetXaxis()->SetRangeUser(minPtEta,maxPtEta);
        histo2DEtaRatioToCombFit->Draw("copy");

        TLegend* legendCrossSectionRatioEta           = GetAndSetLegend2(0.12, 0.66, 0.37, 0.66+(6.*textSizeLabelsRel),textSizeLabelsPixel);

        for (Int_t i = 0; i < 11; i++){
        if(i<numbersofmeas && availableMeas[i]){
            DrawGammaSetMarkerTGraphAsym(graphRatioCombEtaFitSys[i], markerStyleDet[i] ,markerSizeDet[i]*0.5, colorDet[i], colorDet[i], widthLinesBoxes, kTRUE);
            DrawGammaSetMarkerTGraphAsym(graphRatioCombEtaFitStat[i], markerStyleDet[i] ,markerSizeDet[i]*0.5, colorDet[i], colorDet[i]);

            graphRatioCombEtaFitSys[i]->Draw("E2same");
            graphRatioCombEtaFitStat[i]->Draw("p,same,e");

            legendCrossSectionRatioEta->AddEntry(graphRatioCombEtaFitSys[i],Form("%s",nameMeasGlobal[i].Data()),"pf");
        }
        }

        DrawGammaLines(minPtEta,maxPtEta , 1., 1.,3., kGray+2,7);



        drawLatexAdd("ALICE this thesis",0.93,0.91,textSizeLabelsRel*1.3,kFALSE,kFALSE,kTRUE);
        drawLatexAdd(collisionSystem7TeV,0.93,0.85,textSizeLabelsRel*1.3,kFALSE,kFALSE,kTRUE);
        drawLatexAdd("#eta #rightarrow #pi^{+}#pi^{-}#pi^{0}",0.93,0.80,textSizeLabelsRel*1.3,kFALSE,kFALSE,kTRUE);

        legendCrossSectionRatioEta->Draw();


        canvasRatioToCombFit->SaveAs(Form("%s/Eta_RatioOfIndividualMeasToCombFit_PP7TeV.%s",outputDir.Data(),suffix.Data()));
    }



    // ***************************************************************************************************************
    // ******************************* Plotting eta/pi0 ratio for single measurements ********************************
    // ***************************************************************************************************************
    if(doOmegaToPi0){
        textSizeLabelsPixel                 = 54;
        TCanvas* canvasOmegatoPi0combo       = new TCanvas("canvasOmegatoPi0combo","",200,10,1350,900);  // gives the page size
        DrawGammaCanvasSettings( canvasOmegatoPi0combo, 0.1, 0.01, 0.01, 0.125);
        canvasOmegatoPi0combo->SetLogx();

            Double_t textsizeLabelsOmegaToPi0 = 0;
            Double_t textsizeFacOmegaToPi0    = 0;
            if (canvasOmegatoPi0combo->XtoPixel(canvasOmegatoPi0combo->GetX2()) <canvasOmegatoPi0combo->YtoPixel(canvasOmegatoPi0combo->GetY1()) ){
                textsizeLabelsOmegaToPi0      = (Double_t)textSizeLabelsPixel/canvasOmegatoPi0combo->XtoPixel(canvasOmegatoPi0combo->GetX2()) ;
                textsizeFacOmegaToPi0         = (Double_t)1./canvasOmegatoPi0combo->XtoPixel(canvasOmegatoPi0combo->GetX2()) ;
            } else {
                textsizeLabelsOmegaToPi0      = (Double_t)textSizeLabelsPixel/canvasOmegatoPi0combo->YtoPixel(canvasOmegatoPi0combo->GetY1());
                textsizeFacOmegaToPi0         = (Double_t)1./canvasOmegatoPi0combo->YtoPixel(canvasOmegatoPi0combo->GetY1());
            }
            cout << textsizeLabelsOmegaToPi0 << endl;

            textsizeLabelsOmegaToPi0*=0.9;

        TH2F * histo2DOmegatoPi0combo;
        histo2DOmegatoPi0combo               = new TH2F("histo2DOmegatoPi0combo","histo2DOmegatoPi0combo",1000,minPtOmega,35.,1000,-0.05,1.05    );
        SetStyleHistoTH2ForGraphs(histo2DOmegatoPi0combo, "#it{p}_{T} (GeV/#it{c})","#eta/#pi^{0}", 0.85*textsizeLabelsOmegaToPi0, textsizeLabelsOmegaToPi0,
                                0.85*textsizeLabelsOmegaToPi0,1.1*textsizeLabelsOmegaToPi0, 0.9, 0.65, 510, 510);
        histo2DOmegatoPi0combo->GetXaxis()->SetMoreLogLabels();
        histo2DOmegatoPi0combo->GetXaxis()->SetNoExponent();
        histo2DOmegatoPi0combo->GetXaxis()->SetRangeUser(minPtOmega,maxPtOmega);
        histo2DOmegatoPi0combo->GetYaxis()->SetRangeUser(-0.05,1.05);
        histo2DOmegatoPi0combo->Draw("copy");
        cout << __LINE__ << endl;
            // plotting systematics graphs
            for (Int_t i = 0; i < 11; i++){
                cout << i << endl;
                if(graphOmegaToPi0Sys[i]){
                    //cout << graphOmegaToPi0Sys[i] << endl;
                    graphOmegaToPi0Sys[i]->Print();
                    DrawGammaSetMarkerTGraphAsym(graphOmegaToPi0Sys[i], markerStyleDet[i], markerSizeDet[i]*0.75, colorDet[i] , colorDet[i], widthLinesBoxes, kTRUE);
                    graphOmegaToPi0Sys[i]->Draw("E2same");
                }
            }
            cout << __LINE__ << endl;
            DrawGammaSetMarkerTGraphAsym(graphCombOmegaToPi0Stat, markerStyleDet[1], markerSizeDet[1]*0.75, kPink , kPink, widthLinesBoxes, kTRUE);
            // plotting statistics graphs
            for (Int_t i = 0; i < 11; i++){
                if(graphOmegaToPi0Stat[i]){
                    DrawGammaSetMarkerTGraphAsym(graphOmegaToPi0Stat[i], markerStyleDet[i], markerSizeDet[i]*0.75, colorDet[i] , colorDet[i]);
                    graphOmegaToPi0Stat[i]->Draw("p,same,e");
                }
            }
            DrawGammaSetMarkerTGraphAsym(graphCombOmegaToPi0Sys, markerStyleDet[1], markerSizeDet[1]*0.75, kPink , kPink);

            TLegend* legendOmegaToPi0 = GetAndSetLegend2(0.67, 0.15, 0.9, 0.15+(textsizeLabelsOmegaToPi0*4*0.9), textSizeLabelsPixel);
            for (Int_t i = 0; i < 11; i++){
                if(graphOmegaToPi0Sys[i]){
                    legendOmegaToPi0->AddEntry(graphOmegaToPi0Sys[i],nameMeasGlobal[i],"pf");
                }
            }
            legendOmegaToPi0->Draw();

            drawLatexAdd(collisionSystem7TeV.Data(),0.13, 0.92,0.85*textsizeLabelsOmegaToPi0,kFALSE);
            drawLatexAdd("ALICE",0.13, 0.92-(1*textsizeLabelsOmegaToPi0*0.85),0.85*textsizeLabelsOmegaToPi0,kFALSE);

        canvasOmegatoPi0combo->Update();
        canvasOmegatoPi0combo->SaveAs(Form("%s/OmegaToPi0_differentSystems.%s",outputDir.Data(), suffix.Data()));
    }
    //*************************************************************************************************************
    //*****************************************************************************************



    // **********************************************************************************************************************
    // *******************************************Plot Ratio of comb omega / pi0   ******************************************
    // **********************************************************************************************************************

    textSizeLabelsPixel                 = 54;
    textSizeLabelsRel      = 54./1200;

    TCanvas* canvasRatioOmegaToPi0       = new TCanvas("canvasRatioOmegaToPi0","",200,10,1350,900);  // gives the page size
    DrawGammaCanvasSettings( canvasRatioOmegaToPi0, 0.08, 0.01, 0.01, 0.125);
    canvasRatioOmegaToPi0->SetLogx();
    canvasRatioOmegaToPi0->cd();

    textsizeLabelsPP       = 0;
    textsizeFacPP          = 0;
    if (canvasRatioOmegaToPi0->XtoPixel(canvasRatioOmegaToPi0->GetX2()) <canvasRatioOmegaToPi0->YtoPixel(canvasRatioOmegaToPi0->GetY1()) ){
        textsizeLabelsPP            = (Double_t)textSizeLabelsPixel/canvasRatioOmegaToPi0->XtoPixel(canvasRatioOmegaToPi0->GetX2()) ;
        textsizeFacPP               = (Double_t)1./canvasRatioOmegaToPi0->XtoPixel(canvasRatioOmegaToPi0->GetX2()) ;
    } else {
        textsizeLabelsPP            = (Double_t)textSizeLabelsPixel/canvasRatioOmegaToPi0->YtoPixel(canvasRatioOmegaToPi0->GetY1());
        textsizeFacPP               = (Double_t)1./canvasRatioOmegaToPi0->YtoPixel(canvasRatioOmegaToPi0->GetY1());
    }
    textSizeLabelsPixel*=0.75;
    cout << textsizeLabelsPP << endl;

    TH2F * histo2DOmegaRatioToPi0;
    histo2DOmegaRatioToPi0               = new TH2F("histo2DOmegaRatioToPi0","histo2DOmegaRatioToPi0",1000,minPtOmega, maxPtOmega,1000,0.02,10.);
    SetStyleHistoTH2ForGraphs(histo2DOmegaRatioToPi0, "#it{p}_{T} (GeV/#it{c})","#omega / #pi^{0}", 0.85*textsizeLabelsPP, textsizeLabelsPP,
                              0.85*textsizeLabelsPP,textsizeLabelsPP, 0.85, 0.5, 510, 505);
    histo2DOmegaRatioToPi0->GetXaxis()->SetMoreLogLabels();
    histo2DOmegaRatioToPi0->GetYaxis()->SetMoreLogLabels(kTRUE);
    histo2DOmegaRatioToPi0->GetXaxis()->SetNoExponent(kTRUE);

    histo2DOmegaRatioToPi0->GetYaxis()->SetRangeUser(-0.1,1.9);
    histo2DOmegaRatioToPi0->GetXaxis()->SetRangeUser(minPtOmega,maxPtOmega);
    histo2DOmegaRatioToPi0->Draw("copy");

    TLegend* legendCrossSectionRatioOmegaPi0           = GetAndSetLegend2(0.12, 0.75, 0.37, 0.8+(3.*textSizeLabelsRel),textSizeLabelsPixel);

    DrawGammaSetMarker(pythiaOmegaToPi0Ratio, 24, 1.5, kRed+1 , kRed+1);
    pythiaOmegaToPi0Ratio->SetLineWidth(2.);
    //pythiaOmegaToPi0Ratio->GetXaxis()->SetRangeUser(1.8,16.);
    pythiaOmegaToPi0Ratio->Draw("p,same,z");


    DrawGammaSetMarkerTGraphAsym(graphOmegaToPi0PHOSPublic7TeVSys, 24 ,markerSizeDet[0]*0.5, kGray+2, kGray+2, widthLinesBoxes, kTRUE);
    DrawGammaSetMarkerTGraphErr(graphOmegaToPi0PHOSPublic7TeVStat, 24 ,markerSizeDet[0]*0.5, kGray+2, kGray+2);
    graphOmegaToPi0PHOSPublic7TeVSys->Draw("E2same");
    graphOmegaToPi0PHOSPublic7TeVStat->Draw("p,same,e");

    if(doOmegaToPi0){
        DrawGammaSetMarkerTGraphAsym(graphCombOmegaToPi0Sys, markerStyleDet[0] ,markerSizeDet[0]*0.5, colorDet[0], colorDet[0], widthLinesBoxes, kTRUE);
        DrawGammaSetMarkerTGraphAsym(graphCombOmegaToPi0Stat, markerStyleDet[0] ,markerSizeDet[0]*0.5, colorDet[0], colorDet[0]);

        graphCombOmegaToPi0Sys->Draw("E2same");
        graphCombOmegaToPi0Stat->Draw("p,same,e");
    }

    DrawGammaSetMarkerTGraphAsym(graphRatioOmegaCombPi0FitSys, markerStyleDet[0] ,markerSizeDet[0]*0.5, kGreen+3, kGreen+3, widthLinesBoxes, kTRUE);
    DrawGammaSetMarkerTGraphAsym(graphRatioOmegaCombPi0FitStat, markerStyleDet[0] ,markerSizeDet[0]*0.5, kGreen+3, kGreen+3);

    graphRatioOmegaCombPi0FitSys->Draw("E2same");
    graphRatioOmegaCombPi0FitStat->Draw("p,same,e");

    //funcRatioOmegaFitToPi0Fit->SetRange(1.5,19.);
    //funcRatioOmegaFitToPi0Fit->SetLineColor(kGray+2);
    //funcRatioOmegaFitToPi0Fit->SetLineStyle(2);
    //funcRatioOmegaFitToPi0Fit->Draw("same");


    if(doOmegaToPi0) legendCrossSectionRatioOmegaPi0->AddEntry(graphCombOmegaToPi0Sys,"#omega / #pi^{0}","pf");
    legendCrossSectionRatioOmegaPi0->AddEntry(graphRatioOmegaCombPi0FitSys,"#omega#rightarrow#pi^{+}#pi^{-}#pi^{0} / Tsallis(#pi^{0})","pf");
    legendCrossSectionRatioOmegaPi0->AddEntry(graphOmegaToPi0PHOSPublic7TeVSys,"PHOS PUB-787","pf");
    legendCrossSectionRatioOmegaPi0->AddEntry(pythiaOmegaToPi0Ratio,"PYTHIA 8.2 Monash 2013","lpz");


    //DrawGammaLines(minPtOmega,maxPtOmega , 1., 1.,3., kGray+2,7);



    drawLatexAdd("ALICE this thesis",0.93,0.91,textSizeLabelsRel,kFALSE,kFALSE,kTRUE);
    drawLatexAdd(collisionSystem7TeV,0.93,0.85,textSizeLabelsRel,kFALSE,kFALSE,kTRUE);
    //drawLatexAdd("#omega #rightarrow #pi^{+}#pi^{-}#pi^{0}",0.93,0.80,textSizeLabelsRel,kFALSE,kFALSE,kTRUE);

    legendCrossSectionRatioOmegaPi0->Draw();


    canvasRatioOmegaToPi0->SaveAs(Form("%s/OmegaCombToPi0CombFitRatio.%s",outputDir.Data(),suffix.Data()));

    // **********************************************************************************************************************
    // *******************************************Plot Ratio of comb omega / pi0  Comparison ******************************************
    // **********************************************************************************************************************

    textSizeLabelsPixel                 = 46;
    textSizeLabelsRel      = 46./1200;

    TCanvas* canvasRatioOmegaToPi0Compare       = new TCanvas("canvasRatioOmegaToPi0Compare","",200,10,1350,900);  // gives the page size
    DrawGammaCanvasSettings( canvasRatioOmegaToPi0Compare, 0.08, 0.01, 0.01, 0.125);
    //canvasRatioOmegaToPi0Compare->SetLogx();
    canvasRatioOmegaToPi0Compare->cd();

    textsizeLabelsPP       = 0;
    textsizeFacPP          = 0;
    if (canvasRatioOmegaToPi0Compare->XtoPixel(canvasRatioOmegaToPi0Compare->GetX2()) <canvasRatioOmegaToPi0Compare->YtoPixel(canvasRatioOmegaToPi0Compare->GetY1()) ){
        textsizeLabelsPP            = (Double_t)textSizeLabelsPixel/canvasRatioOmegaToPi0Compare->XtoPixel(canvasRatioOmegaToPi0Compare->GetX2()) ;
        textsizeFacPP               = (Double_t)1./canvasRatioOmegaToPi0Compare->XtoPixel(canvasRatioOmegaToPi0Compare->GetX2()) ;
    } else {
        textsizeLabelsPP            = (Double_t)textSizeLabelsPixel/canvasRatioOmegaToPi0Compare->YtoPixel(canvasRatioOmegaToPi0Compare->GetY1());
        textsizeFacPP               = (Double_t)1./canvasRatioOmegaToPi0Compare->YtoPixel(canvasRatioOmegaToPi0Compare->GetY1());
    }
    textSizeLabelsPixel*=0.75;
    cout << textsizeLabelsPP << endl;

    histo2DOmegaRatioToPi0               = new TH2F("histo2DOmegaRatioToPi0","histo2DOmegaRatioToPi0",1000,-0.01, maxPtOmega,1000,-0.01,10.);
    SetStyleHistoTH2ForGraphs(histo2DOmegaRatioToPi0, "#it{p}_{T} (GeV/#it{c})","#omega / #pi^{0}", 0.85*textsizeLabelsPP, textsizeLabelsPP,
                              0.85*textsizeLabelsPP,textsizeLabelsPP, 0.9, 0.7, 510, 705);
    histo2DOmegaRatioToPi0->GetXaxis()->SetMoreLogLabels();
    histo2DOmegaRatioToPi0->GetYaxis()->SetMoreLogLabels(kTRUE);
    histo2DOmegaRatioToPi0->GetXaxis()->SetNoExponent(kTRUE);
    histo2DOmegaRatioToPi0->GetYaxis()->SetLabelOffset(0.01);

    // uncomment if axis can't be shown
    // histo2DOmegaRatioToPi0->GetYaxis()->SetTickLength(0);
    // histo2DOmegaRatioToPi0->GetYaxis()->SetLabelSize(0);

    //histo2DOmegaRatioToPi0->GetXaxis()->SetTitleOffset(-0.01);

    histo2DOmegaRatioToPi0->GetYaxis()->SetRangeUser(-0.2,2.4);
    histo2DOmegaRatioToPi0->GetXaxis()->SetRangeUser(-0.2,28.5);
    histo2DOmegaRatioToPi0->Draw("copy");

    legendCrossSectionRatioOmegaPi0           = GetAndSetLegend2(0.12, 0.65, 0.37, 0.65+(7.*textSizeLabelsRel),textSizeLabelsPixel);


    DrawGammaSetMarkerTGraphAsym(graphOmegaeeToPi0200GeVSys, 4 ,markerSizeDet[0]*0.3, kRed+3, kRed+3, widthLinesBoxes, kTRUE);
    DrawGammaSetMarkerTGraphAsym(graphOmegaeeToPi0200GeVStat,4 ,markerSizeDet[0]*0.3,  kRed+3,  kRed+3);
    graphOmegaeeToPi0200GeVSys->Draw("E2same");
    graphOmegaeeToPi0200GeVStat->Draw("p,same,e");

    DrawGammaSetMarkerTGraphAsym(graphOmegapipipiToPi0200GeVSys, 5 ,markerSizeDet[0]*0.3, kGray+2, kGray+2, widthLinesBoxes, kTRUE);
    DrawGammaSetMarkerTGraphAsym(graphOmegapipipiToPi0200GeVStat, 5 ,markerSizeDet[0]*0.3,  kGray+2,  kGray+2);
    graphOmegapipipiToPi0200GeVSys->Draw("E2same");
    graphOmegapipipiToPi0200GeVStat->Draw("p,same,e");

    DrawGammaSetMarkerTGraphAsym(graphOmegapi0GammaToPi0200GeVSys, 25 ,markerSizeDet[0]*0.3, kBlue+2, kBlue+2, widthLinesBoxes, kTRUE);
    DrawGammaSetMarkerTGraphAsym(graphOmegapi0GammaToPi0200GeVStat, 25 ,markerSizeDet[0]*0.3,  kBlue+2,  kBlue+2);
    graphOmegapi0GammaToPi0200GeVSys->Draw("E2same");
    graphOmegapi0GammaToPi0200GeVStat->Draw("p,same,e");

    DrawGammaSetMarkerTGraphAsym(graphOmegapi0GammaToPi062GeVStat, 29 ,markerSizeDet[0]*0.5,  kGreen+3,  kGreen+3);
    graphOmegapi0GammaToPi062GeVStat->Draw("p,same,e");


    if(useFitForRatio){
        DrawGammaSetMarkerTGraphAsym(graphRatioOmegaCombPi0FitSys, markerStyleDet[0] ,markerSizeDet[0]*0.35, colorDet[0], colorDet[0], widthLinesBoxes, kTRUE);
        DrawGammaSetMarkerTGraphAsym(graphRatioOmegaCombPi0FitStat, markerStyleDet[0] ,markerSizeDet[0]*0.35, colorDet[0], colorDet[0]);
        graphRatioOmegaCombPi0FitSys->Draw("E2same");
        graphRatioOmegaCombPi0FitStat->Draw("p,same,e");
    } else{
        DrawGammaSetMarkerTGraphAsym(graphCombOmegaToPi0Sys, markerStyleDet[0] ,markerSizeDet[0]*0.35, colorDet[0], colorDet[0], widthLinesBoxes, kTRUE);
        DrawGammaSetMarkerTGraphAsym(graphCombOmegaToPi0Stat, markerStyleDet[0] ,markerSizeDet[0]*0.35, colorDet[0], colorDet[0]);
        graphCombOmegaToPi0Sys->Draw("E2same");
        graphCombOmegaToPi0Stat->Draw("p,same,e");
    }

    // DrawGammaSetMarkerTGraphAsym(graphRatioOmegaEMCPi0FitSys, markerStyleDet[0] ,markerSizeDet[0]*0.35, kRed+2, kRed+2, widthLinesBoxes, kTRUE);
    // DrawGammaSetMarkerTGraphAsym(graphRatioOmegaEMCPi0FitStat, markerStyleDet[0] ,markerSizeDet[0]*0.35, kRed+2, kRed+2);
    // graphRatioOmegaEMCPi0FitSys->Draw("E2same");
    // graphRatioOmegaEMCPi0FitStat->Draw("p,same,e");

    DrawGammaSetMarkerTGraphAsym(graphOmegaToPi0PHOSPublic7TeVSys, 24 ,markerSizeDet[0]*0.5, kGray+2, kGray+2, widthLinesBoxes, kTRUE);
    DrawGammaSetMarkerTGraphErr(graphOmegaToPi0PHOSPublic7TeVStat, 24 ,markerSizeDet[0]*0.5, kGray+2, kGray+2);
    graphOmegaToPi0PHOSPublic7TeVSys->Draw("E2same");
    graphOmegaToPi0PHOSPublic7TeVStat->Draw("p,same,e");

    //funcRatioOmegaFitToPi0Fit->SetRange(1.5,19.);
    //funcRatioOmegaFitToPi0Fit->SetLineColor(kGray+2);
    //funcRatioOmegaFitToPi0Fit->SetLineStyle(2);
    //funcRatioOmegaFitToPi0Fit->Draw("same");

    DrawGammaSetMarker(pythiaOmegaToPi0Ratio, 24, 1.5, kRed+2 , kRed+2);
    pythiaOmegaToPi0Ratio->SetLineWidth(widthCommonFit);
    pythiaOmegaToPi0Ratio->Draw("same,hist,l");

    DrawGammaSetMarker(pythiaTune4COmegaToPi0Ratio, 24, 1.5, kGreen+2 , kGreen+2);
    pythiaTune4COmegaToPi0Ratio->SetLineWidth(widthCommonFit);
    pythiaTune4COmegaToPi0Ratio->Draw("same,hist,l");


    legendCrossSectionRatioOmegaPi0->AddEntry(graphCombOmegaToPi0Sys,"#omega#rightarrow #pi^{+}#pi^{-}#pi^{0} ,pp #sqrt{s} = 7000 GeV, work in progress","pf");
    legendCrossSectionRatioOmegaPi0->AddEntry(graphOmegapipipiToPi0200GeVSys,"#omega#rightarrow #pi^{+}#pi^{-}#pi^{0} ,pp #sqrt{s} =   200 GeV, PHENIX","pf");
    legendCrossSectionRatioOmegaPi0->AddEntry(graphOmegapi0GammaToPi0200GeVSys,"#omega#rightarrow #pi^{0}#gamma     ,pp #sqrt{s} =   200 GeV, PHENIX","pf");
    legendCrossSectionRatioOmegaPi0->AddEntry(graphOmegaeeToPi0200GeVSys,"#omega#rightarrow e^{+}e^{-}    ,pp #sqrt{s} =   200 GeV, PHENIX","pf");
    legendCrossSectionRatioOmegaPi0->AddEntry(graphOmegapi0GammaToPi062GeVStat,"#omega#rightarrow #pi^{0}#gamma     ,pp #sqrt{s} =     62 GeV, ISR","pe");
    legendCrossSectionRatioOmegaPi0->AddEntry(pythiaOmegaToPi0Ratio,"Pythia 8 Monash 2013","l");
    legendCrossSectionRatioOmegaPi0->AddEntry(pythiaTune4COmegaToPi0Ratio,"Pythia 8 Tune 4C","l");


    //DrawGammaLines(minPtOmega,maxPtOmega , 1., 1.,3., kGray+2,7);



    legendCrossSectionRatioOmegaPi0->Draw();


    canvasRatioOmegaToPi0Compare->SaveAs(Form("%s/OmegaCombToPi0CombFitRatioComparison.%s",outputDir.Data(),suffix.Data()));

    // **********************************************************************************************************************
    // *******************************************Plot Ratio of comb eta / pi0   ******************************************
    // **********************************************************************************************************************
    if(doEta){
        textSizeLabelsPixel                 = 54;
        textSizeLabelsRel      = 54./1200;

        TCanvas* canvasRatioEtaToPi0       = new TCanvas("canvasRatioEtaToPi0","",200,10,1350,900);  // gives the page size
        DrawGammaCanvasSettings( canvasRatioEtaToPi0, 0.08, 0.01, 0.01, 0.125);
        canvasRatioEtaToPi0->SetLogx();
        canvasRatioEtaToPi0->cd();

        textsizeLabelsPP       = 0;
        textsizeFacPP          = 0;
        if (canvasRatioEtaToPi0->XtoPixel(canvasRatioEtaToPi0->GetX2()) <canvasRatioEtaToPi0->YtoPixel(canvasRatioEtaToPi0->GetY1()) ){
            textsizeLabelsPP            = (Double_t)textSizeLabelsPixel/canvasRatioEtaToPi0->XtoPixel(canvasRatioEtaToPi0->GetX2()) ;
            textsizeFacPP               = (Double_t)1./canvasRatioEtaToPi0->XtoPixel(canvasRatioEtaToPi0->GetX2()) ;
        } else {
            textsizeLabelsPP            = (Double_t)textSizeLabelsPixel/canvasRatioEtaToPi0->YtoPixel(canvasRatioEtaToPi0->GetY1());
            textsizeFacPP               = (Double_t)1./canvasRatioEtaToPi0->YtoPixel(canvasRatioEtaToPi0->GetY1());
        }
        textSizeLabelsPixel*=0.75;
        cout << textsizeLabelsPP << endl;

        TH2F * histo2DEtaRatioToPi0;
        histo2DEtaRatioToPi0               = new TH2F("histo2DEtaRatioToPi0","histo2DEtaRatioToPi0",1000,minPtEta, maxPtEta,1000,0.02,10.);
        SetStyleHistoTH2ForGraphs(histo2DEtaRatioToPi0, "#it{p}_{T} (GeV/#it{c})","#omega / #pi^{0}", 0.85*textsizeLabelsPP, textsizeLabelsPP,
                                0.85*textsizeLabelsPP,textsizeLabelsPP, 0.85, 0.5, 510, 505);
        histo2DEtaRatioToPi0->GetXaxis()->SetMoreLogLabels();
        histo2DEtaRatioToPi0->GetYaxis()->SetMoreLogLabels(kTRUE);
        histo2DEtaRatioToPi0->GetXaxis()->SetNoExponent(kTRUE);

        histo2DEtaRatioToPi0->GetYaxis()->SetRangeUser(-0.1,1.9);
        histo2DEtaRatioToPi0->GetXaxis()->SetRangeUser(minPtEta,maxPtEta);
        histo2DEtaRatioToPi0->Draw("copy");

        TLegend* legendCrossSectionRatioEtaPi0           = GetAndSetLegend2(0.12, 0.75, 0.37, 0.8+(3.*textSizeLabelsRel),textSizeLabelsPixel);

        DrawGammaSetMarkerTGraphAsym(graphEtaToPi0Comb7TeVSys,24 ,markerSizeDet[0]*0.5, kGray+2, kGray+2, widthLinesBoxes, kTRUE);
        DrawGammaSetMarkerTGraphAsym(graphEtaToPi0Comb7TeVStat, 24 ,markerSizeDet[0]*0.5, kGray+2, kGray+2);

        graphEtaToPi0Comb7TeVSys->Draw("E2same");
        graphEtaToPi0Comb7TeVStat->Draw("p,same,e");

        DrawGammaSetMarker(pythiaEtaToPi0Ratio, 24, 1.5, kRed+1 , kRed+1);
        pythiaEtaToPi0Ratio->SetLineWidth(2.);
        pythiaEtaToPi0Ratio->GetXaxis()->SetRangeUser(1.8,16.);
        pythiaEtaToPi0Ratio->Draw("p,same,z");

        DrawGammaSetMarkerTGraphAsym(graphRatioEtaCombPi0FitSys, markerStyleDet[0] ,markerSizeDet[0]*0.5, colorDet[0], colorDet[0], widthLinesBoxes, kTRUE);
        DrawGammaSetMarkerTGraphAsym(graphRatioEtaCombPi0FitStat, markerStyleDet[0] ,markerSizeDet[0]*0.5, colorDet[0], colorDet[0]);

        graphRatioEtaCombPi0FitSys->Draw("E2same");
        graphRatioEtaCombPi0FitStat->Draw("p,same,e");


    //    funcRatioEtaFitToPi0Fit->SetRange(1.5,19.);
    //    funcRatioEtaFitToPi0Fit->SetLineColor(kGray+2);
    //    funcRatioEtaFitToPi0Fit->SetLineStyle(2);
    //    funcRatioEtaFitToPi0Fit->Draw("same");

        legendCrossSectionRatioEtaPi0->AddEntry(graphRatioEtaCombPi0FitSys,"#eta#rightarrow#pi^{+}#pi^{-}#pi^{0} / Tsallis(#pi^{0})","pf");
        legendCrossSectionRatioEtaPi0->AddEntry(graphEtaToPi0Comb7TeVSys,"#eta#rightarrow#gamma#gamma / #pi^{0}","pf");
        legendCrossSectionRatioEtaPi0->AddEntry(pythiaEtaToPi0Ratio,"PYTHIA 8.2 Monash 2013","lpz");


        //DrawGammaLines(minPtEta,maxPtEta , 1., 1.,3., kGray+2,7);



        drawLatexAdd("ALICE this thesis",0.93,0.91,textSizeLabelsRel,kFALSE,kFALSE,kTRUE);
        drawLatexAdd(collisionSystem7TeV,0.93,0.85,textSizeLabelsRel,kFALSE,kFALSE,kTRUE);
        //drawLatexAdd("#eta #rightarrow #pi^{+}#pi^{-}#pi^{0}",0.93,0.80,textSizeLabelsRel,kFALSE,kFALSE,kTRUE);

        legendCrossSectionRatioEtaPi0->Draw();


        canvasRatioEtaToPi0->SaveAs(Form("%s/EtaCombToPi0CombFitRatio.%s",outputDir.Data(),suffix.Data()));
    }
    // **********************************************************************************************************************
    // ******************************************* Omega invariant cross section            *********************************
    // **********************************************************************************************************************
    TCanvas* canvasCrossSectionOmega       = new TCanvas("canvasCrossSectionOmega", "", 0,0,1250,1250);  // gives the page size
    DrawGammaCanvasSettings( canvasCrossSectionOmega,  0.14, 0.01, 0.01, 0.09);
    canvasCrossSectionOmega->SetLogy(1);
    canvasCrossSectionOmega->SetLogx(1);
        TH2F * histoDummyCrossSection;
            histoDummyCrossSection                = new TH2F("histoDummyCrossSection", "histoDummyCrossSection",1000, minPtOmega,  maxPtOmega, 1000, minXSectionOmega, maxXSectionOmega );
        SetStyleHistoTH2ForGraphs( histoDummyCrossSection, "#it{p}_{T} (GeV/#it{c})", "#it{E}#frac{d^{3}#sigma}{d#it{p}^{3}} (pb GeV^{-2}#it{c}^{3})",
                                0.85*textSizeLabelsRel, textSizeLabelsRel, 0.85*textSizeLabelsRel, textSizeLabelsRel, 0.9, 1.4);//(#times #epsilon_{pur})
        
        histoDummyCrossSection->GetYaxis()->SetLabelOffset(0.001);
        histoDummyCrossSection->GetXaxis()->SetLabelOffset(-0.01);

        histoDummyCrossSection->GetXaxis()->SetMoreLogLabels(kTRUE);
        histoDummyCrossSection->DrawCopy();
        Int_t exampleInteger = 0;
        Int_t totalMeasAvail = 0;
        for(Int_t i=0;i<numbersofmeas;i++)
          if(availableMeas[i]){
            exampleInteger = i;
            totalMeasAvail++;
          }
        TH1D * histoBlack               = (TH1D*)histoOmegaInvCrossSection[exampleInteger]->Clone("histoBlack") ;
        histoBlack->SetLineColor(kBlack);
        histoBlack->SetMarkerStyle(21) ;
        histoBlack->SetMarkerColor(kBlack) ;
        histoBlack->SetMarkerSize(1.) ;

        TGraphAsymmErrors * graphGrey   = (TGraphAsymmErrors*)graphOmegaInvCrossSectionSys[exampleInteger]->Clone("graphGrey") ;
        graphGrey   ->SetFillColor(kGray);
        graphGrey   ->SetLineColor(kGray);
        graphGrey   ->SetFillStyle(1001);
        
        TGraphAsymmErrors * graphOmegaInvCrossSectionSysClone[10];
        TGraphAsymmErrors * graphOmegaInvCrossSectionStatClone[10];
        
        TLegend* legendCrossSectionOmega           = GetAndSetLegend2(0.18, 0.13, 0.43, 0.14+(3.5*textSizeLabelsRel),textSizeLabelsPixel);
        
        Int_t scalingFactor = pow(10,totalMeasAvail-1);
        // Int_t legendScalingFactor = totalMeasAvail-1;
        Int_t legendScalingFactor = 0;
        for(Int_t i=numbersofmeas;i>-1;i--){
          if(availableMeas[i]){
            graphOmegaInvCrossSectionSysClone[i]= (TGraphAsymmErrors*)graphOmegaInvCrossSectionSys[i]->Clone(Form("csSys%d",i)) ;
            graphOmegaInvCrossSectionStatClone[i]= (TGraphAsymmErrors*)graphOmegaInvCrossSectionStat[i]->Clone(Form("csStat%d",i)) ;
            // for (int j=0;j<graphOmegaInvCrossSectionSysClone[i]->GetN();j++){
            //     graphOmegaInvCrossSectionSysClone[i]->GetY()[j] *= scalingFactor;
            //     graphOmegaInvCrossSectionSysClone[i]->GetEYhigh()[j] *= scalingFactor;
            //     graphOmegaInvCrossSectionSysClone[i]->GetEYlow()[j] *= scalingFactor;
            // }
            // DrawGammaSetMarkerTGraphAsym(graphOmegaInvCrossSectionSysInterpolation[i], markerStyleDet[i], markerSizeDet[i]*0.55, colorDetMC[i] , colorDetMC[i], widthLinesBoxes, kTRUE);
            // graphOmegaInvCrossSectionSysInterpolation[i]     ->Draw("E2same");

            DrawGammaSetMarkerTGraphAsym(graphOmegaInvCrossSectionSysClone[i], markerStyleDet[i], markerSizeDet[i]*0.55, colorDet[i] , colorDet[i], widthLinesBoxes, kTRUE);
            graphOmegaInvCrossSectionSysClone[i]     ->Draw("E2same");
            if(legendScalingFactor==0){
                legendCrossSectionOmega->AddEntry(graphOmegaInvCrossSectionSysClone[i],Form("%s",nameMeasGlobal[i].Data()),"pf");
            } else{
                legendCrossSectionOmega->AddEntry(graphOmegaInvCrossSectionSysClone[i],Form("%s x10^{%d}",nameMeasGlobal[i].Data(),legendScalingFactor),"pf");
            }
            // statistics
            // for (int j=0;j<graphOmegaInvCrossSectionStatClone[i]->GetN();j++){
            //     graphOmegaInvCrossSectionStatClone[i]->GetY()[j] *= scalingFactor;
            //     graphOmegaInvCrossSectionStatClone[i]->GetEYhigh()[j] *= scalingFactor;
            //     graphOmegaInvCrossSectionStatClone[i]->GetEYlow()[j] *= scalingFactor;
            // }
            // DrawGammaSetMarkerTGraph(graphOmegaInvCrossSectionStatInterpolation[i],  markerStyleDet[i], markerSizeDet[i]*0.55, colorDetMC[i] , colorDetMC[i]);
            // graphOmegaInvCrossSectionStatInterpolation[i]->Draw("p,same,z");
            DrawGammaSetMarkerTGraph(graphOmegaInvCrossSectionStatClone[i],  markerStyleDet[i], markerSizeDet[i]*0.55, colorDet[i] , colorDet[i]);
            graphOmegaInvCrossSectionStatClone[i]->Draw("p,same,z");
            scalingFactor/=10;
            // legendScalingFactor--;
          }
        }

        // Draw PHOS AnalysisNote
        //DrawGammaSetMarkerTGraphAsym(graphOmegaXSecPHOS7TeVSys, 24, markerSizeDet[0]*0.55,kRed+1, kRed+1, widthLinesBoxes, kTRUE);
        //graphOmegaXSecPHOS7TeVSys     ->Draw("E2same");
        //legendCrossSectionOmega->AddEntry(graphOmegaXSecPHOS7TeVSys,"PHOS Public Note PUB-787","pf");

        //DrawGammaSetMarkerTGraph(graphOmegaXSecPHOS7TeVStat,  24, markerSizeDet[0]*0.55,kRed+1 , kRed+1);
        //graphOmegaXSecPHOS7TeVStat->Draw("p,same,z");
        // --



        legendCrossSectionOmega->Draw();


        // TLegend* legendOmegaErr2 = GetAndSetLegend2(0.72, 0.72, 0.98, 0.72+(2*textSizeLabelsRel),0.85*textSizeLabelsPixel);
        // legendOmegaErr2->AddEntry(histoBlack, "stat. Err.","ple");
        // legendOmegaErr2->AddEntry(graphGrey,  "syst. Err.","f");
        // legendOmegaErr2->Draw();

        drawLatexAdd("ALICE work in progress",0.93,0.92,textSizeLabelsRel,kFALSE,kFALSE,kTRUE);
        drawLatexAdd(collisionSystem7TeV,0.93,0.87,textSizeLabelsRel,kFALSE,kFALSE,kTRUE);
        drawLatexAdd("#omega #rightarrow #pi^{+}#pi^{-}#pi^{0}",0.93,0.82,textSizeLabelsRel,kFALSE,kFALSE,kTRUE);
        histoDummyCrossSection->Draw("sameaxis");
    canvasCrossSectionOmega->Update();
    canvasCrossSectionOmega->Print(Form("%s/Omega_InvariantCrossSectionMeas.%s",outputDir.Data(),suffix.Data()));



    //********************************************************************************************************
    // Plotting combined inv cross section omega
    //********************************************************************************************************
    TCanvas* canvasCrossSectionOmegaCombined      = new TCanvas("canvasCrossSectionOmegaCombined", "", 0,0,1250,1250);  // gives the page size
    DrawGammaCanvasSettings( canvasCrossSectionOmegaCombined,  0.14, 0.01, 0.01, 0.09);
    canvasCrossSectionOmegaCombined->SetLogy(1);
    canvasCrossSectionOmegaCombined->SetLogx(1);
    TH2F* histoDummyCrossSectionCombined  = new TH2F("histoDummyCrossSectionCombined", "histoDummyCrossSectionCombined",1000, minPtOmega,  maxPtOmega, 1000, 1e3, 9e8 );
    SetStyleHistoTH2ForGraphs( histoDummyCrossSectionCombined, "#it{p}_{T} (GeV/#it{c})", "#it{E}#frac{d^{3}#sigma}{d#it{p}^{3}} (pb GeV^{-2}#it{c}^{3})",
                            0.85*textSizeLabelsRel, textSizeLabelsRel, 0.85*textSizeLabelsRel, textSizeLabelsRel, 0.9, 1.4);//(#times #epsilon_{pur})

    histoDummyCrossSectionCombined->GetYaxis()->SetLabelOffset(0.001);
    histoDummyCrossSectionCombined->GetXaxis()->SetLabelOffset(-0.01);
    histoDummyCrossSectionCombined->GetXaxis()->SetMoreLogLabels(kTRUE);
    histoDummyCrossSectionCombined->DrawCopy();


    DrawGammaSetMarkerTGraphAsym(graphCombOmegaInvXSectionSys, markerStyleDet[0], markerSizeDet[0]*0.55,kBlack, kBlack, widthLinesBoxes, kTRUE);
    graphCombOmegaInvXSectionSys     ->Draw("E2same");
    DrawGammaSetMarkerTF1( fitInvXSectionOmega, 3, 2, kGray+2);
    fitInvXSectionOmega->Draw("same");

    TLegend* legendCrossSectionOmegaCombined           = GetAndSetLegend2(0.18, 0.13, 0.43, 0.05+(3.5*textSizeLabelsRel),textSizeLabelsPixel);
     legendCrossSectionOmegaCombined->AddEntry(graphCombOmegaInvXSectionSys,"combined","fp");
     legendCrossSectionOmegaCombined->AddEntry(fitInvXSectionOmega,"Tsallis fit","l");
     legendCrossSectionOmegaCombined->Draw("");




    DrawGammaSetMarkerTGraph(graphCombOmegaInvXSectionStat,  markerStyleDet[0], markerSizeDet[0]*0.55,kBlack, kBlack);
    graphCombOmegaInvXSectionStat->Draw("p,same,z");
    drawLatexAdd("ALICE this thesis",0.93,0.92,textSizeLabelsRel,kFALSE,kFALSE,kTRUE);
    drawLatexAdd(collisionSystem7TeV,0.93,0.87,textSizeLabelsRel,kFALSE,kFALSE,kTRUE);
    drawLatexAdd("#omega #rightarrow #pi^{+}#pi^{-}#pi^{0}",0.93,0.82,textSizeLabelsRel,kFALSE,kFALSE,kTRUE);
            // --
    canvasCrossSectionOmegaCombined->Update();
    canvasCrossSectionOmegaCombined->Print(Form("%s/Omega_InvariantCrossSectionCombined.%s",outputDir.Data(),suffix.Data()));


    //********************************************************************************************************
    // Plotting combined inv cross section omega with Ratio
    //********************************************************************************************************

    Double_t arrayBoundariesX1_XSec[2];
    Double_t arrayBoundariesY1_XSec[6];
    Double_t relativeMarginsXXSec[3];
    Double_t relativeMarginsYXSec[3];
    textSizeLabelsPixel = 48;
    ReturnCorrectValuesForCanvasScaling(1200,2000, 1, 5,0.145, 0.006, 0.003,0.05,arrayBoundariesX1_XSec,arrayBoundariesY1_XSec,relativeMarginsXXSec,relativeMarginsYXSec);

    TCanvas* canvasInvSectionPaper      = new TCanvas("canvasInvSectionPaper","",0,0,1200,2000);  // gives the page size
    DrawGammaCanvasSettings( canvasInvSectionPaper,  0.18, 0.04, 0.03, 0.06);

    TPad* padInvSectionSpec             = new TPad("padInvSectionSpec", "", arrayBoundariesX1_XSec[0], arrayBoundariesY1_XSec[3], arrayBoundariesX1_XSec[1], arrayBoundariesY1_XSec[0],-1, -1, -2);
    DrawGammaPadSettings( padInvSectionSpec, relativeMarginsXXSec[0], relativeMarginsXXSec[2], relativeMarginsYXSec[0], relativeMarginsYXSec[1]);
    padInvSectionSpec->Draw();
    Double_t marginXSec                 = relativeMarginsXXSec[0]*1250;
    Double_t textsizeLabelsXSecUp       = 0;
    Double_t textsizeFacXSecUp          = 0;
    if (padInvSectionSpec->XtoPixel(padInvSectionSpec->GetX2()) < padInvSectionSpec->YtoPixel(padInvSectionSpec->GetY1())){
        textsizeLabelsXSecUp            = (Double_t)textSizeLabelsPixel/padInvSectionSpec->XtoPixel(padInvSectionSpec->GetX2()) ;
        textsizeFacXSecUp               = (Double_t)1./padInvSectionSpec->XtoPixel(padInvSectionSpec->GetX2()) ;
    } else {
        textsizeLabelsXSecUp            = (Double_t)textSizeLabelsPixel/padInvSectionSpec->YtoPixel(padInvSectionSpec->GetY1());
        textsizeFacXSecUp               = (Double_t)1./padInvSectionSpec->YtoPixel(padInvSectionSpec->GetY1());
    }

    TPad* padInvSectionPHOSRatio         = new TPad("padInvSectionPHOSRatio", "", arrayBoundariesX1_XSec[0], arrayBoundariesY1_XSec[4], arrayBoundariesX1_XSec[1], arrayBoundariesY1_XSec[3],-1, -1, -2);
    DrawGammaPadSettings( padInvSectionPHOSRatio, relativeMarginsXXSec[0], relativeMarginsXXSec[2], relativeMarginsYXSec[1], relativeMarginsYXSec[1]);
    padInvSectionPHOSRatio->Draw();
    Double_t textsizeLabelsXSecMiddle   = 0;
    Double_t textsizeFacXSecMiddle      = 0;
    if (padInvSectionPHOSRatio->XtoPixel(padInvSectionPHOSRatio->GetX2()) < padInvSectionPHOSRatio->YtoPixel(padInvSectionPHOSRatio->GetY1())){
        textsizeLabelsXSecMiddle        = (Double_t)textSizeLabelsPixel/padInvSectionPHOSRatio->XtoPixel(padInvSectionPHOSRatio->GetX2()) ;
        textsizeFacXSecMiddle           = (Double_t)1./padInvSectionPHOSRatio->XtoPixel(padInvSectionPHOSRatio->GetX2()) ;
    } else {
        textsizeLabelsXSecMiddle        = (Double_t)textSizeLabelsPixel/padInvSectionPHOSRatio->YtoPixel(padInvSectionPHOSRatio->GetY1());
        textsizeFacXSecMiddle           = (Double_t)1./padInvSectionPHOSRatio->YtoPixel(padInvSectionPHOSRatio->GetY1());
    }

    TPad* padInvSectionPythiaRatio      = new TPad("padInvSectionPythiaRatio", "", arrayBoundariesX1_XSec[0], arrayBoundariesY1_XSec[5], arrayBoundariesX1_XSec[1], arrayBoundariesY1_XSec[4],-1, -1, -2);
    DrawGammaPadSettings( padInvSectionPythiaRatio, relativeMarginsXXSec[0], relativeMarginsXXSec[2], relativeMarginsYXSec[1], relativeMarginsYXSec[2]);
    padInvSectionPythiaRatio->Draw();
    Double_t textsizeLabelsXSecDown     = 0;
    Double_t textsizeFacXSecDown        = 0;
    if (padInvSectionPythiaRatio->XtoPixel(padInvSectionPythiaRatio->GetX2()) < padInvSectionPythiaRatio->YtoPixel(padInvSectionPythiaRatio->GetY1())){
        textsizeLabelsXSecDown          = (Double_t)textSizeLabelsPixel/padInvSectionPythiaRatio->XtoPixel(padInvSectionPythiaRatio->GetX2()) ;
        textsizeFacXSecDown             = (Double_t)1./padInvSectionPythiaRatio->XtoPixel(padInvSectionPythiaRatio->GetX2()) ;
    } else {
        textsizeLabelsXSecDown          = (Double_t)textSizeLabelsPixel/padInvSectionPythiaRatio->YtoPixel(padInvSectionPythiaRatio->GetY1());
        textsizeFacXSecDown             = (Double_t)1./padInvSectionPythiaRatio->YtoPixel(padInvSectionPythiaRatio->GetY1());
    }

    padInvSectionSpec->cd();
    padInvSectionSpec->SetLogy(1);
    padInvSectionSpec->SetLogx(1);

    histoDummyCrossSectionCombined  = new TH2F("histoDummyCrossSectionCombined", "histoDummyCrossSectionCombined",1000, minPtOmega,  maxPtOmega, 1000, 1e1, 9e8 );
    SetStyleHistoTH2ForGraphs( histoDummyCrossSectionCombined, "#it{p}_{T} (GeV/#it{c})", "#it{E}#frac{d^{3}#sigma}{d#it{p}^{3}} (pb GeV^{-2}#it{c}^{3})",
                            0.85*textSizeLabelsRel, textSizeLabelsRel, 0.85*textSizeLabelsRel, textSizeLabelsRel, 0.9, 1.4);//(#times #epsilon_{pur})

    histoDummyCrossSectionCombined->GetYaxis()->SetLabelOffset(0.001);
    histoDummyCrossSectionCombined->GetXaxis()->SetLabelOffset(-0.01);
    histoDummyCrossSectionCombined->GetXaxis()->SetMoreLogLabels(kTRUE);

    // uncomment if axis can't be shown
    // histoDummyCrossSectionCombined->GetYaxis()->SetTickLength(0);
    // histoDummyCrossSectionCombined->GetYaxis()->SetLabelSize(0);

    histoDummyCrossSectionCombined->DrawCopy();

    TBox* boxErrorSigmaRatioOmega = CreateBoxConv(kGray+2, minPtOmega+0.1, 1.-(0.035 ), minPtOmega+0.3, 1.+(0.035));
    boxErrorSigmaRatioOmega->SetLineWidth(8);

    DrawGammaSetMarkerTF1( fitInvXSectionOmega, 3, 2, kGray+2);
    fitInvXSectionOmega->Draw("same");


    // plotting Pythia 8.2 Monash
    DrawGammaSetMarkerTGraphErr(graphPythia8InvXSectionOmega, 0, 0, kRed+2 , kRed+2, widthLinesBoxes, kTRUE, kRed+2);
    graphPythia8InvXSectionOmega->Draw("3,same");
    DrawGammaSetMarker(histoOmegaXSecSim7TeV, 24, 1.5, kRed+2 , kRed+2);
    histoOmegaXSecSim7TeV->SetLineWidth(widthCommonFit);
    histoOmegaXSecSim7TeV->Draw("same,hist,l");

    // plotting Pythia 8.2 Tune4c
    DrawGammaSetMarkerTGraphErr(graphPythia8Tune4CInvXSectionOmega, 0, 0, kRed+2 , kRed+2, widthLinesBoxes, kTRUE, kRed+2);
    graphPythia8Tune4CInvXSectionOmega->Draw("3,same");
    DrawGammaSetMarker(histoOmegaXSecSimTune4C7TeV, 24, 1.5, kGreen+2 , kGreen+2);
    histoOmegaXSecSimTune4C7TeV->SetLineWidth(widthCommonFit);
    histoOmegaXSecSimTune4C7TeV->Draw("same,hist,l");

    // Plot NLO
    //DrawGammaSetMarkerTGraphErr(graphNLOInvXSectionOmega, 0, 0, kBlue+2 , kBlue+2, widthLinesBoxes, kTRUE, kRed+2);
    graphNLOInvXSectionOmega->SetLineColor(kBlue);
    graphNLOInvXSectionOmega->Draw("same");

    if(drawPHOSPN){
        DrawGammaSetMarkerTGraph(graphOmegaXSecPHOS7TeVStat,  24,  markerSizeDet[0]*0.55,kGray+2 , kGray+2);
        graphOmegaXSecPHOS7TeVStat->Draw("p,same,z");
        DrawGammaSetMarkerTGraphAsym(graphOmegaXSecPHOS7TeVSys, 24,  markerSizeDet[0]*0.55,kGray+2, kGray+2, widthLinesBoxes, kTRUE);
        graphOmegaXSecPHOS7TeVSys     ->Draw("E2same");
    }

    DrawGammaSetMarkerTGraphAsym(graphCombOmegaInvXSectionSys, markerStyleDet[0], markerSizeDet[0]*0.55,kBlack, kBlack, widthLinesBoxes, kTRUE);
    graphCombOmegaInvXSectionSys     ->Draw("E2same");

    DrawGammaSetMarkerTGraph(graphCombOmegaInvXSectionStat,  markerStyleDet[0], markerSizeDet[0]*0.55,kBlack, kBlack);
    graphCombOmegaInvXSectionStat->Draw("p,same,z");

     legendCrossSectionOmegaCombined           = GetAndSetLegend2(0.18, 0.02, 0.43, 0.04+(5.*textSizeLabelsRel),textSizeLabelsPixel);
     legendCrossSectionOmegaCombined->AddEntry(graphCombOmegaInvXSectionSys,"data: #omega #rightarrow #pi^{+}#pi^{-}#pi^{0}","fp");
     legendCrossSectionOmegaCombined->AddEntry(boxErrorSigmaRatioOmega,"norm. unc. 3.5%","l");
     if(drawPHOSPN) legendCrossSectionOmegaCombined->AddEntry(graphOmegaXSecPHOS7TeVSys,"data: PHOS PUB-787","fp");
     legendCrossSectionOmegaCombined->AddEntry(histoOmegaXSecSim7TeV,"PYTHIA 8.2 Monash 2013","lpz");
     legendCrossSectionOmegaCombined->AddEntry(histoOmegaXSecSimTune4C7TeV,"PYTHIA 8.2 Tune4C","lpz");
     legendCrossSectionOmegaCombined->AddEntry(fitInvXSectionOmega,"Tsallis fit","l");
     legendCrossSectionOmegaCombined->Draw("");




    drawLatexAdd("ALICE work in progress",0.93,0.91,textSizeLabelsRel,kFALSE,kFALSE,kTRUE);
    drawLatexAdd(collisionSystem7TeV,0.93,0.86,textSizeLabelsRel,kFALSE,kFALSE,kTRUE);
            // --

    padInvSectionPHOSRatio->cd();
    padInvSectionPHOSRatio->SetLogx(1);
        TH2F * ratio2PHOSPN               = new TH2F("ratio2PHOSPN","ratio2PHOSPN",1000,minPtOmega,maxPtOmega,1000,0.15,2.3);
        SetStyleHistoTH2ForGraphs(ratio2PHOSPN, "#it{p}_{T} (GeV/#it{c})","#frac{data}{Tsallis}", 0.85*textsizeLabelsXSecMiddle, textsizeLabelsXSecMiddle,
                                  0.85*textsizeLabelsXSecMiddle,textsizeLabelsXSecMiddle, 1,0.2/(textsizeFacXSecMiddle*marginXSec), 510, 505);
        ratio2PHOSPN->GetYaxis()->SetMoreLogLabels(kTRUE);
        ratio2PHOSPN->GetYaxis()->SetNdivisions(505);
        ratio2PHOSPN->GetYaxis()->SetNoExponent(kTRUE);
        ratio2PHOSPN->GetXaxis()->SetMoreLogLabels(kTRUE);
        ratio2PHOSPN->GetXaxis()->SetNoExponent(kTRUE);
        ratio2PHOSPN->GetXaxis()->SetLabelFont(42);
        ratio2PHOSPN->GetYaxis()->SetLabelFont(42);
        ratio2PHOSPN->GetYaxis()->SetLabelOffset(0.);
        ratio2PHOSPN->GetXaxis()->SetTickLength(0.07);

        // uncomment if axis can't be shown
        // ratio2PHOSPN->GetYaxis()->SetTickLength(0);
        // ratio2PHOSPN->GetYaxis()->SetLabelSize(0);

        ratio2PHOSPN->DrawCopy();

        DrawGammaLines(minPtOmega,maxPtOmega , 1., 1.,3., kGray+2,7);


        if(drawPHOSPN){
            DrawGammaSetMarkerTGraphAsym(graphRatioPHOSPublicNoteCombFitSys, 24 ,markerSizeDet[0]*0.5, kGray+2, kGray+2, widthLinesBoxes, kTRUE);
            DrawGammaSetMarkerTGraphErr(graphRatioPHOSPublicNoteCombFitStat, 24 ,markerSizeDet[0]*0.5, kGray+2, kGray+2);
            graphRatioPHOSPublicNoteCombFitSys->Draw("E2same");
            graphRatioPHOSPublicNoteCombFitStat->Draw("p,same,e");
        }

        DrawGammaSetMarkerTGraphAsym(graphRatioCombCombFitSys, markerStyleDet[0] ,markerSizeDet[0]*0.5, kBlack, kBlack, widthLinesBoxes, kTRUE);
        DrawGammaSetMarkerTGraphAsym(graphRatioCombCombFitStat, markerStyleDet[0] ,markerSizeDet[0]*0.5, kBlack, kBlack);

        graphRatioCombCombFitSys->Draw("E2same");
        graphRatioCombCombFitStat->Draw("p,same,e");

        DrawGammaSetMarkerTGraphErr(graphRatioPythia8ToFit, 0, 0, kRed+2 , kRed+2, widthLinesBoxes, kTRUE, kRed+2);
        graphRatioPythia8ToFit->Draw("3,same");
        DrawGammaSetMarker(histoRatioPythia8ToFit, 24, 1.5, kRed+2 , kRed+2);
        histoRatioPythia8ToFit->SetLineWidth(widthCommonFit);
        histoRatioPythia8ToFit->Draw("same,hist,l");

        DrawGammaSetMarkerTGraphErr(graphRatioPythia8Tune4CToFit, 0, 0, kRed+2 , kRed+2, widthLinesBoxes, kTRUE, kGreen+2);
        graphRatioPythia8Tune4CToFit->Draw("3,same");
        DrawGammaSetMarker(histoRatioPythia8Tune4CToFit, 24, 1.5, kGreen+2 , kGreen+2);
        histoRatioPythia8Tune4CToFit->SetLineWidth(widthCommonFit);
        histoRatioPythia8Tune4CToFit->Draw("same,hist,l");
        
        // NLO
        graphRatioNLOToFit->SetLineColor(kBlue);
        graphRatioNLOToFit->SetLineWidth(3);
        graphRatioNLOToFit->Draw("same");

        boxErrorSigmaRatioOmega->Draw("");


    padInvSectionPythiaRatio->cd();
    padInvSectionPythiaRatio->SetLogx(1);
        TH2F * ratio2DPythiaOmega             = new TH2F("ratio2DPythiaOmega","ratio2DPythiaOmega",1000,minPtOmega,maxPtOmega,1000,0.2,2.1);
        SetStyleHistoTH2ForGraphs(ratio2DPythiaOmega, "#it{p}_{T} (GeV/#it{c})","#frac{PYTHIA}{data}", 0.85*textsizeLabelsXSecDown, textsizeLabelsXSecDown,
                              0.85*textsizeLabelsXSecDown,textsizeLabelsXSecDown, 0.9,0.2/(textsizeFacXSecDown*marginXSec), 510, 505);
        ratio2DPythiaOmega->GetYaxis()->SetMoreLogLabels(kTRUE);
        ratio2DPythiaOmega->GetYaxis()->SetNdivisions(505);
        ratio2DPythiaOmega->GetYaxis()->SetNoExponent(kTRUE);
        ratio2DPythiaOmega->GetXaxis()->SetMoreLogLabels(kTRUE);
        ratio2DPythiaOmega->GetXaxis()->SetNoExponent(kTRUE);
        ratio2DPythiaOmega->GetXaxis()->SetLabelFont(42);
        ratio2DPythiaOmega->GetYaxis()->SetLabelFont(42);
        ratio2DPythiaOmega->GetYaxis()->SetLabelOffset(0.);
        ratio2DPythiaOmega->GetXaxis()->SetTickLength(0.06);
        ratio2DPythiaOmega->GetYaxis()->SetTickLength(0.04);

        // uncomment if axis can't be shown
        // ratio2DPythiaOmega->GetYaxis()->SetTickLength(0);
        // ratio2DPythiaOmega->GetYaxis()->SetLabelSize(0);

        ratio2DPythiaOmega->DrawCopy();

        DrawGammaLines(minPtOmega,maxPtOmega , 1., 1.,3., kGray+2,7);
        boxErrorSigmaRatioOmega->Draw();

        // DrawGammaSetMarkerTGraphErr(graphRatioPythia8ToFit, 0, 0, kRed+2 , kRed+2, widthLinesBoxes, kTRUE, kRed+2);
        // graphRatioPythia8ToFit->Draw("3,same");
        // DrawGammaSetMarker(histoRatioPythia8ToFit, 24, 1.5, kRed+2 , kRed+2);
        // histoRatioPythia8ToFit->SetLineWidth(widthCommonFit);
        // histoRatioPythia8ToFit->Draw("same,hist,l");


    canvasInvSectionPaper->Update();
    canvasInvSectionPaper->Print(Form("%s/Omega_InvariantCrossSectionCombinedWithRatios.%s",outputDir.Data(),suffix.Data()));


    
    // **********************************************************************************************************************
    // ******************************************* Eta invariant cross section       ****************************************
    // **********************************************************************************************************************
       if(doEta){
            canvasCrossSectionOmega->Clear();
            canvasCrossSectionOmega->cd();
            TH2F * histoDummyCrossSectionEta;
                histoDummyCrossSectionEta                = new TH2F("histoDummyCrossSectionEta", "histoDummyCrossSectionEta",1000, minPtEta,  maxPtEta, 1000, minXSectionEta, maxXSectionEta );
            SetStyleHistoTH2ForGraphs( histoDummyCrossSectionEta, "#it{p}_{T} (GeV/#it{c})", "#it{E}#frac{d^{3}#sigma}{d#it{p}^{3}} (pb GeV^{-2}#it{c}^{3})",
                                    0.85*textSizeLabelsRel, textSizeLabelsRel, 0.85*textSizeLabelsRel, textSizeLabelsRel, 0.9, 1.4);//(#times #epsilon_{pur})
            
            histoDummyCrossSectionEta->GetYaxis()->SetLabelOffset(0.001);
            histoDummyCrossSectionEta->GetXaxis()->SetLabelOffset(-0.01);
            histoDummyCrossSectionEta->GetXaxis()->SetMoreLogLabels(kTRUE);
            histoDummyCrossSectionEta->DrawCopy();
            exampleInteger = 0;
            totalMeasAvail = 0;
            for(Int_t i=0;i<numbersofmeas;i++)
            if(availableMeas[i]){
                exampleInteger = i;
                totalMeasAvail++;
            }
            
            TGraphAsymmErrors * graphEtaInvCrossSectionSysClone[10];
            TGraphAsymmErrors * graphEtaInvCrossSectionStatClone[10];
            
            TLegend* legendCrossSectionEta           = GetAndSetLegend2(0.18, 0.13, 0.43, 0.13+(3.5*textSizeLabelsRel),textSizeLabelsPixel);
            
            scalingFactor = pow(10,totalMeasAvail-1);
            // Int_t legendScalingFactor = totalMeasAvail-1;
            legendScalingFactor = 0;
            for(Int_t i=numbersofmeas;i>-1;i--){
            if(availableMeas[i]){
                graphEtaInvCrossSectionSysClone[i]= (TGraphAsymmErrors*)graphEtaInvCrossSectionSys[i]->Clone(Form("csSys%d",i)) ;
                graphEtaInvCrossSectionStatClone[i]= (TGraphAsymmErrors*)graphEtaInvCrossSectionStat[i]->Clone(Form("csStat%d",i)) ;
                // for (int j=0;j<graphEtaInvCrossSectionSysClone[i]->GetN();j++){
                //     graphEtaInvCrossSectionSysClone[i]->GetY()[j] *= scalingFactor;
                //     graphEtaInvCrossSectionSysClone[i]->GetEYhigh()[j] *= scalingFactor;
                //     graphEtaInvCrossSectionSysClone[i]->GetEYlow()[j] *= scalingFactor;
                // }
                DrawGammaSetMarkerTGraphAsym(graphEtaInvCrossSectionSysClone[i], markerStyleDet[i], markerSizeDet[i]*0.55, colorDet[i] , colorDet[i], widthLinesBoxes, kTRUE);
                graphEtaInvCrossSectionSysClone[i]     ->Draw("E2same");
                legendCrossSectionEta->AddEntry(graphEtaInvCrossSectionSysClone[i],Form("%s",nameMeasGlobal[i].Data()),"pf");
                
                // statistics
                // for (int j=0;j<graphEtaInvCrossSectionStatClone[i]->GetN();j++){
                //     graphEtaInvCrossSectionStatClone[i]->GetY()[j] *= scalingFactor;
                //     graphEtaInvCrossSectionStatClone[i]->GetEYhigh()[j] *= scalingFactor;
                //     graphEtaInvCrossSectionStatClone[i]->GetEYlow()[j] *= scalingFactor;
                // }
                DrawGammaSetMarkerTGraph(graphEtaInvCrossSectionStatClone[i],  markerStyleDet[i], markerSizeDet[i]*0.55, colorDet[i] , colorDet[i]);
                graphEtaInvCrossSectionStatClone[i]->Draw("p,same,z");
                scalingFactor/=10;
                // legendScalingFactor--;
            }
            }
            // graphEtaInvCrossSectionSysInterpolation[9]     ->Draw("p,same,z");
            legendCrossSectionEta->Draw();


            // TLegend* legendEtaErr2 = GetAndSetLegend2(0.72, 0.72, 0.98, 0.72+(2*textSizeLabelsRel),0.85*textSizeLabelsPixel);
            // legendEtaErr2->AddEntry(histoBlack, "stat. Err.","ple");
            // legendEtaErr2->AddEntry(graphGrey,  "syst. Err.","f");
            // legendEtaErr2->Draw();

            drawLatexAdd("ALICE this thesis",0.93,0.91,textSizeLabelsRel,kFALSE,kFALSE,kTRUE);
            drawLatexAdd(collisionSystem7TeV,0.93,0.86,textSizeLabelsRel,kFALSE,kFALSE,kTRUE);
            drawLatexAdd("#eta #rightarrow #pi^{+}#pi^{-}#pi^{0}",0.93,0.82,textSizeLabelsRel,kFALSE,kFALSE,kTRUE);
            histoDummyCrossSectionEta->Draw("sameaxis");
        canvasCrossSectionOmega->Update();
        canvasCrossSectionOmega->Print(Form("%s/Eta_InvariantCrossSectionMeas.%s",outputDir.Data(),suffix.Data()));

        minX                               = 3.0;
        maxX                               = 16;
    }

    //********************************************************************************************************
    // Plotting combined inv cross section eta
    //********************************************************************************************************
    TLegend* legendCrossSectionEtaCombined = NULL;
    if(doEta){
        TCanvas* canvasCrossSectionEtaCombined      = new TCanvas("canvasCrossSectionEtaCombined", "", 0,0,1250,1250);  // gives the page size
        DrawGammaCanvasSettings( canvasCrossSectionEtaCombined,  0.14, 0.01, 0.01, 0.09);
        canvasCrossSectionEtaCombined->SetLogy(1);
        canvasCrossSectionEtaCombined->SetLogx(1);
        histoDummyCrossSectionCombined  = new TH2F("histoDummyCrossSectionCombined", "histoDummyCrossSectionCombined",1000, minPtEta,  maxPtEta, 1000, 1e3, 9e8 );
        SetStyleHistoTH2ForGraphs( histoDummyCrossSectionCombined, "#it{p}_{T} (GeV/#it{c})", "#it{E}#frac{d^{3}#sigma}{d#it{p}^{3}} (pb GeV^{-2}#it{c}^{3})",
                                0.85*textSizeLabelsRel, textSizeLabelsRel, 0.85*textSizeLabelsRel, textSizeLabelsRel, 0.9, 1.4);//(#times #epsilon_{pur})

        histoDummyCrossSectionCombined->GetYaxis()->SetLabelOffset(0.001);
        histoDummyCrossSectionCombined->GetXaxis()->SetLabelOffset(-0.01);
        histoDummyCrossSectionCombined->GetXaxis()->SetMoreLogLabels(kTRUE);
        histoDummyCrossSectionCombined->DrawCopy();


        DrawGammaSetMarkerTGraphAsym(graphCombEtaInvXSectionSys, markerStyleDet[0], markerSizeDet[0]*0.55,kBlack, kBlack, widthLinesBoxes, kTRUE);
        graphCombEtaInvXSectionSys     ->Draw("E2same");
        DrawGammaSetMarkerTF1( fitInvXSectionEta, 3, 2, kGray+2);
        fitInvXSectionEta->Draw("same");

        legendCrossSectionEtaCombined           = GetAndSetLegend2(0.18, 0.13, 0.43, 0.05+(3.5*textSizeLabelsRel),textSizeLabelsPixel);
        legendCrossSectionEtaCombined->AddEntry(graphCombEtaInvXSectionSys,"combined","fp");
        legendCrossSectionEtaCombined->AddEntry(fitInvXSectionEta,"Tsallis fit","l");
        legendCrossSectionEtaCombined->Draw("");




        DrawGammaSetMarkerTGraph(graphCombEtaInvXSectionStat,  markerStyleDet[0], markerSizeDet[0]*0.55,kBlack, kBlack);
        graphCombEtaInvXSectionStat->Draw("p,same,z");
        drawLatexAdd("ALICE this thesis",0.93,0.92,textSizeLabelsRel,kFALSE,kFALSE,kTRUE);
        drawLatexAdd(collisionSystem7TeV,0.93,0.87,textSizeLabelsRel,kFALSE,kFALSE,kTRUE);
        drawLatexAdd("#eta #rightarrow #pi^{+}#pi^{-}#pi^{0}",0.93,0.82,textSizeLabelsRel,kFALSE,kFALSE,kTRUE);
                // --
        canvasCrossSectionEtaCombined->Update();
        canvasCrossSectionEtaCombined->Print(Form("%s/Eta_InvariantCrossSectionCombined.%s",outputDir.Data(),suffix.Data()));
    }

    //********************************************************************************************************
    // Plotting combined inv cross section Eta with Ratio
    //********************************************************************************************************
    if(doEta){
        textSizeLabelsPixel = 48;
        ReturnCorrectValuesForCanvasScaling(1200,2000, 1, 5,0.145, 0.006, 0.003,0.05,arrayBoundariesX1_XSec,arrayBoundariesY1_XSec,relativeMarginsXXSec,relativeMarginsYXSec);

        canvasInvSectionPaper      = new TCanvas("canvasInvSectionPaper","",0,0,1200,2000);  // gives the page size
        DrawGammaCanvasSettings( canvasInvSectionPaper,  0.18, 0.03, 0.03, 0.06);

        padInvSectionSpec             = new TPad("padInvSectionSpec", "", arrayBoundariesX1_XSec[0], arrayBoundariesY1_XSec[3], arrayBoundariesX1_XSec[1], arrayBoundariesY1_XSec[0],-1, -1, -2);
        DrawGammaPadSettings( padInvSectionSpec, relativeMarginsXXSec[0], relativeMarginsXXSec[2], relativeMarginsYXSec[0], relativeMarginsYXSec[1]);
        padInvSectionSpec->Draw();

        marginXSec                 = relativeMarginsXXSec[0]*1250;
        textsizeLabelsXSecUp       = 0;
        textsizeFacXSecUp          = 0;
        if (padInvSectionSpec->XtoPixel(padInvSectionSpec->GetX2()) < padInvSectionSpec->YtoPixel(padInvSectionSpec->GetY1())){
            textsizeLabelsXSecUp            = (Double_t)textSizeLabelsPixel/padInvSectionSpec->XtoPixel(padInvSectionSpec->GetX2()) ;
            textsizeFacXSecUp               = (Double_t)1./padInvSectionSpec->XtoPixel(padInvSectionSpec->GetX2()) ;
        } else {
            textsizeLabelsXSecUp            = (Double_t)textSizeLabelsPixel/padInvSectionSpec->YtoPixel(padInvSectionSpec->GetY1());
            textsizeFacXSecUp               = (Double_t)1./padInvSectionSpec->YtoPixel(padInvSectionSpec->GetY1());
        }

        TPad* padInvSectionEtaGGRatio         = new TPad("padInvSectionEtaGGRatio", "", arrayBoundariesX1_XSec[0], arrayBoundariesY1_XSec[4], arrayBoundariesX1_XSec[1], arrayBoundariesY1_XSec[3],-1, -1, -2);
        DrawGammaPadSettings( padInvSectionEtaGGRatio, relativeMarginsXXSec[0], relativeMarginsXXSec[2], relativeMarginsYXSec[1], relativeMarginsYXSec[1]);
        padInvSectionEtaGGRatio->Draw();
        textsizeLabelsXSecMiddle   = 0;
        textsizeFacXSecMiddle      = 0;
        if (padInvSectionEtaGGRatio->XtoPixel(padInvSectionEtaGGRatio->GetX2()) < padInvSectionEtaGGRatio->YtoPixel(padInvSectionEtaGGRatio->GetY1())){
            textsizeLabelsXSecMiddle        = (Double_t)textSizeLabelsPixel/padInvSectionEtaGGRatio->XtoPixel(padInvSectionEtaGGRatio->GetX2()) ;
            textsizeFacXSecMiddle           = (Double_t)1./padInvSectionEtaGGRatio->XtoPixel(padInvSectionEtaGGRatio->GetX2()) ;
        } else {
            textsizeLabelsXSecMiddle        = (Double_t)textSizeLabelsPixel/padInvSectionEtaGGRatio->YtoPixel(padInvSectionEtaGGRatio->GetY1());
            textsizeFacXSecMiddle           = (Double_t)1./padInvSectionEtaGGRatio->YtoPixel(padInvSectionEtaGGRatio->GetY1());
        }

        padInvSectionPythiaRatio      = new TPad("padInvSectionPythiaRatio", "", arrayBoundariesX1_XSec[0], arrayBoundariesY1_XSec[5], arrayBoundariesX1_XSec[1], arrayBoundariesY1_XSec[4],-1, -1, -2);
        DrawGammaPadSettings( padInvSectionPythiaRatio, relativeMarginsXXSec[0], relativeMarginsXXSec[2], relativeMarginsYXSec[1], relativeMarginsYXSec[2]);
        padInvSectionPythiaRatio->Draw();
        textsizeLabelsXSecDown     = 0;
        textsizeFacXSecDown        = 0;
        if (padInvSectionPythiaRatio->XtoPixel(padInvSectionPythiaRatio->GetX2()) < padInvSectionPythiaRatio->YtoPixel(padInvSectionPythiaRatio->GetY1())){
            textsizeLabelsXSecDown          = (Double_t)textSizeLabelsPixel/padInvSectionPythiaRatio->XtoPixel(padInvSectionPythiaRatio->GetX2()) ;
            textsizeFacXSecDown             = (Double_t)1./padInvSectionPythiaRatio->XtoPixel(padInvSectionPythiaRatio->GetX2()) ;
        } else {
            textsizeLabelsXSecDown          = (Double_t)textSizeLabelsPixel/padInvSectionPythiaRatio->YtoPixel(padInvSectionPythiaRatio->GetY1());
            textsizeFacXSecDown             = (Double_t)1./padInvSectionPythiaRatio->YtoPixel(padInvSectionPythiaRatio->GetY1());
        }

        padInvSectionSpec->cd();
        padInvSectionSpec->SetLogy(1);
        padInvSectionSpec->SetLogx(1);

        histoDummyCrossSectionCombined  = new TH2F("histoDummyCrossSectionCombined", "histoDummyCrossSectionCombined",1000, minPtEta,  maxPtEta, 1000, minXSectionEta, maxXSectionEta );
        SetStyleHistoTH2ForGraphs( histoDummyCrossSectionCombined, "#it{p}_{T} (GeV/#it{c})", "#it{E}#frac{d^{3}#sigma}{d#it{p}^{3}} (pb GeV^{-2}#it{c}^{3})",
                                0.85*textSizeLabelsRel, textSizeLabelsRel, 0.85*textSizeLabelsRel, textSizeLabelsRel, 0.9, 1.4);//(#times #epsilon_{pur})

        histoDummyCrossSectionCombined->GetYaxis()->SetLabelOffset(0.001);
        histoDummyCrossSectionCombined->GetXaxis()->SetLabelOffset(-0.01);
        histoDummyCrossSectionCombined->GetXaxis()->SetMoreLogLabels(kTRUE);
        histoDummyCrossSectionCombined->DrawCopy();

        DrawGammaSetMarkerTF1( fitInvXSectionEta, 3, 2, kGray+2);
        fitInvXSectionEta->Draw("same");

        DrawGammaSetMarker(histoEtaXSecSim7TeV, 24, 1.5, kRed+1 , kRed+1);
        histoEtaXSecSim7TeV->SetLineWidth(2.);
        histoEtaXSecSim7TeV->GetXaxis()->SetRangeUser(1.5,12.);
        histoEtaXSecSim7TeV->Draw("p,same,z");

        DrawGammaSetMarkerTGraph(graphEtaXSecComb7TeVStat,  24,  markerSizeDet[0]*0.55,kGray+2 , kGray+2);
        graphEtaXSecComb7TeVStat->Draw("p,same,z");

        DrawGammaSetMarkerTGraphAsym(graphEtaXSecComb7TeVSys, 24,  markerSizeDet[0]*0.55,kGray+2, kGray+2, widthLinesBoxes, kTRUE);
        graphEtaXSecComb7TeVSys     ->Draw("E2same");

        DrawGammaSetMarkerTGraphAsym(graphCombEtaInvXSectionSys, markerStyleDet[0], markerSizeDet[0]*0.55,kBlack, kBlack, widthLinesBoxes, kTRUE);
        graphCombEtaInvXSectionSys     ->Draw("E2same");

        DrawGammaSetMarkerTGraph(graphCombEtaInvXSectionStat,  markerStyleDet[0], markerSizeDet[0]*0.55,kBlack, kBlack);
        graphCombEtaInvXSectionStat->Draw("p,same,z");

        TBox* boxErrorSigmaRatioEta = CreateBoxConv(kGray+2, minPtEta+0.1, 1.-(0.035 ), minPtEta+0.3, 1.+(0.035));
        boxErrorSigmaRatioEta->SetLineWidth(8);

        legendCrossSectionEtaCombined           = GetAndSetLegend2(0.18, 0.02, 0.43, 0.04+(5*textSizeLabelsRel),textSizeLabelsPixel);
        legendCrossSectionEtaCombined->AddEntry(graphCombEtaInvXSectionSys,"data: #eta #rightarrow #pi^{+}#pi^{-}#pi^{0}","fp");
        legendCrossSectionEtaCombined->AddEntry(boxErrorSigmaRatioEta,"norm. unc. 3.5%","l");
        legendCrossSectionEtaCombined->AddEntry(graphEtaXSecComb7TeVSys,"data: #eta #rightarrow #gamma #gamma","fp");
        legendCrossSectionEtaCombined->AddEntry(histoEtaXSecSim7TeV,"PYTHIA 8.2 Monash 2013","lpz");
        legendCrossSectionEtaCombined->AddEntry(fitInvXSectionEta,"Tsallis fit","l");
        legendCrossSectionEtaCombined->Draw("");




        drawLatexAdd("ALICE this thesis",0.93,0.91,textSizeLabelsRel,kFALSE,kFALSE,kTRUE);
        drawLatexAdd(collisionSystem7TeV,0.93,0.86,textSizeLabelsRel,kFALSE,kFALSE,kTRUE);
                // --

        padInvSectionEtaGGRatio->cd();
        padInvSectionEtaGGRatio->SetLogx(1);
            TH2F * ratio2EtaGG               = new TH2F("ratio2EtaGG","ratio2EtaGG",1000,minPtEta,maxPtEta,1000,0.15,2.3);
            SetStyleHistoTH2ForGraphs(ratio2EtaGG, "#it{p}_{T} (GeV/#it{c})","#frac{data}{Tsallis}", 0.85*textsizeLabelsXSecMiddle, textsizeLabelsXSecMiddle,
                                    0.85*textsizeLabelsXSecMiddle,textsizeLabelsXSecMiddle, 1,0.2/(textsizeFacXSecMiddle*marginXSec), 510, 505);
            ratio2EtaGG->GetYaxis()->SetMoreLogLabels(kTRUE);
            ratio2EtaGG->GetYaxis()->SetNdivisions(505);
            ratio2EtaGG->GetYaxis()->SetNoExponent(kTRUE);
            ratio2EtaGG->GetXaxis()->SetMoreLogLabels(kTRUE);
            ratio2EtaGG->GetXaxis()->SetNoExponent(kTRUE);
            ratio2EtaGG->GetXaxis()->SetLabelFont(42);
            ratio2EtaGG->GetYaxis()->SetLabelFont(42);
            ratio2EtaGG->GetYaxis()->SetLabelOffset(0.);
            ratio2EtaGG->GetXaxis()->SetTickLength(0.07);
            ratio2EtaGG->DrawCopy();

            DrawGammaLines(minPtEta,maxPtEta , 1., 1.,3., kGray+2,7);
            boxErrorSigmaRatioEta->Draw();

            DrawGammaSetMarkerTGraphAsym(graphRatioEtaGGCombFitSys, 24 ,markerSizeDet[0]*0.5, kGray+2, kGray+2, widthLinesBoxes, kTRUE);
            DrawGammaSetMarkerTGraphAsym(graphRatioEtaGGCombFitStat, 24 ,markerSizeDet[0]*0.5, kGray+2, kGray+2);

            graphRatioEtaGGCombFitSys->Draw("E2same");
            graphRatioEtaGGCombFitStat->Draw("p,same,e");

            DrawGammaSetMarkerTGraphAsym(graphRatioCombEtaCombFitSys, markerStyleDet[0] ,markerSizeDet[0]*0.5, colorDet[0], colorDet[0], widthLinesBoxes, kTRUE);
            DrawGammaSetMarkerTGraphAsym(graphRatioCombEtaCombFitStat, markerStyleDet[0] ,markerSizeDet[0]*0.5, colorDet[0], colorDet[0]);

            graphRatioCombEtaCombFitSys->Draw("E2same");
            graphRatioCombEtaCombFitStat->Draw("p,same,e");


        padInvSectionPythiaRatio->cd();
        padInvSectionPythiaRatio->SetLogx(1);
            TH2F * ratio2DPythiaEta             = new TH2F("ratio2DPythiaEta","ratio2DPythiaEta",1000,minPtEta,maxPtEta,1000,0.2,2.1);
            SetStyleHistoTH2ForGraphs(ratio2DPythiaEta, "#it{p}_{T} (GeV/#it{c})","#frac{PYTHIA}{data}", 0.85*textsizeLabelsXSecDown, textsizeLabelsXSecDown,
                                0.85*textsizeLabelsXSecDown,textsizeLabelsXSecDown, 0.9,0.2/(textsizeFacXSecDown*marginXSec), 510, 505);
            ratio2DPythiaEta->GetYaxis()->SetMoreLogLabels(kTRUE);
            ratio2DPythiaEta->GetYaxis()->SetNdivisions(505);
            ratio2DPythiaEta->GetYaxis()->SetNoExponent(kTRUE);
            ratio2DPythiaEta->GetXaxis()->SetMoreLogLabels(kTRUE);
            ratio2DPythiaEta->GetXaxis()->SetNoExponent(kTRUE);
            ratio2DPythiaEta->GetXaxis()->SetLabelFont(42);
            ratio2DPythiaEta->GetYaxis()->SetLabelFont(42);
            ratio2DPythiaEta->GetYaxis()->SetLabelOffset(0.);
            ratio2DPythiaEta->GetXaxis()->SetTickLength(0.06);
            ratio2DPythiaEta->GetYaxis()->SetTickLength(0.04);
            ratio2DPythiaEta->DrawCopy();

            DrawGammaLines(minPtEta,maxPtEta , 1., 1.,3., kGray+2,7);
            boxErrorSigmaRatioEta->Draw();

            DrawGammaSetMarkerTGraphAsym(graphRatioPythiaEtaCombDataSys, 24 ,markerSizeDet[0]*0.5, kRed+1, kRed+1, widthLinesBoxes, kTRUE);
            DrawGammaSetMarkerTGraphAsym(graphRatioPythiaEtaCombDataStat, 24 ,markerSizeDet[0]*0.5, kRed+1, kRed+1);

            graphRatioPythiaEtaCombDataSys->Draw("E2same");
            graphRatioPythiaEtaCombDataStat->Draw("p,same,e");


        canvasInvSectionPaper->Update();
        canvasInvSectionPaper->Print(Form("%s/Eta_InvariantCrossSectionCombinedWithRatios.%s",outputDir.Data(),suffix.Data()));
    }
    // **********************************************************************************************************************
    // ******************************************* Mass and width for Omega            ****************************************
    // **********************************************************************************************************************
    minX = 1.5;
    cout << "plotting Omega mass" << endl;
    Double_t arrayBoundariesX1_4[2];
    Double_t arrayBoundariesY1_4[3];
    Double_t relativeMarginsX[3];
    Double_t relativeMarginsY[3];
    textSizeLabelsPixel             = 35;
    ReturnCorrectValuesForCanvasScaling(1350,1250, 1, 2,0.09, 0.005, 0.005,0.085,arrayBoundariesX1_4,arrayBoundariesY1_4,relativeMarginsX,relativeMarginsY);

    TCanvas* canvasMassWidthOmega     = new TCanvas("canvasMassWidthOmega","",0,0,1350,1250);  // gives the page size
    DrawGammaCanvasSettings( canvasMassWidthOmega,  0.13, 0.02, 0.03, 0.0);

    TPad* padWidthOmega               = new TPad("padWidthOmega", "", arrayBoundariesX1_4[0], arrayBoundariesY1_4[1], arrayBoundariesX1_4[1], arrayBoundariesY1_4[0],-1, -1, -2);
    DrawGammaPadSettings( padWidthOmega, relativeMarginsX[0], relativeMarginsX[2], relativeMarginsY[0], relativeMarginsY[1]);
    padWidthOmega->Draw();

    TPad* padMassOmega                = new TPad("padMassOmega", "", arrayBoundariesX1_4[0], arrayBoundariesY1_4[2], arrayBoundariesX1_4[1], arrayBoundariesY1_4[1],-1, -1, -2);
    DrawGammaPadSettings( padMassOmega, relativeMarginsX[0], relativeMarginsX[2], relativeMarginsY[1], relativeMarginsY[2]);
    padMassOmega->Draw();

    TPad* padMassLegend1            = new TPad("padMassLegend1", "", 0.13, 0.32, 0.52, 0.52,-1, -1, -2);
    DrawGammaPadSettings( padMassLegend1, 0., 0., 0., 0.);
    padMassLegend1->SetFillStyle(0);
    padMassLegend1->Draw();

    drawLatexAdd("ALICE this thesis",0.93,0.92,textSizeLabelsRel*0.9,kFALSE,kFALSE,kTRUE);
    drawLatexAdd(collisionSystem7TeV,0.93,0.88,textSizeLabelsRel*0.9,kFALSE,kFALSE,kTRUE);
    drawLatexAdd("#omega #rightarrow #pi^{+}#pi^{-}#pi^{0}",0.93,0.84,textSizeLabelsRel*0.9,kFALSE,kFALSE,kTRUE);

    padWidthOmega->cd();
    padWidthOmega->SetLogx();
    Double_t margin                 = relativeMarginsX[0]*2.4*1350;
    Double_t textsizeLabelsWidth    = 0;
    Double_t textsizeFacWidth       = 0;
    if (padWidthOmega->XtoPixel(padWidthOmega->GetX2()) < padWidthOmega->YtoPixel(padWidthOmega->GetY1())){
        textsizeLabelsWidth         = (Double_t)textSizeLabelsPixel/padWidthOmega->XtoPixel(padWidthOmega->GetX2()) ;
        textsizeFacWidth            = (Double_t)1./padWidthOmega->XtoPixel(padWidthOmega->GetX2()) ;
    } else {
        textsizeLabelsWidth         = (Double_t)textSizeLabelsPixel/padWidthOmega->YtoPixel(padWidthOmega->GetY1());
        textsizeFacWidth            = (Double_t)1./padWidthOmega->YtoPixel(padWidthOmega->GetY1());
    }

    TH2F * histo2DAllOmegaFWHM    = new TH2F("histo2DAllOmegaFWHM","histo2DAllOmegaFWHM", 20, minX, maxX ,1000., -30, 60);
    SetStyleHistoTH2ForGraphs(histo2DAllOmegaFWHM, "#it{p}_{T} (GeV/#it{c})", "Peak width (MeV/#it{c}^{2})", 0.85*textsizeLabelsWidth, textsizeLabelsWidth,
                            0.85*textsizeLabelsWidth, textsizeLabelsWidth, 0.8,0.28/(textsizeFacWidth*margin), 512, 505);
    histo2DAllOmegaFWHM->GetYaxis()->SetRangeUser(-1.,39);
    histo2DAllOmegaFWHM->GetYaxis()->SetMoreLogLabels(kTRUE);
    histo2DAllOmegaFWHM->GetYaxis()->SetNdivisions(505);
    histo2DAllOmegaFWHM->GetYaxis()->SetNoExponent(kTRUE);
    histo2DAllOmegaFWHM->GetXaxis()->SetTickLength(0.05);
    histo2DAllOmegaFWHM->GetYaxis()->SetTickLength(0.026);
    histo2DAllOmegaFWHM->DrawCopy();

    for (Int_t i = 0; i < numbersofmeas; i++){
        if(histoOmegaFWHMMeV[i] && histoOmegaTrueFWHMMeV[i] && availableMeas[i]){
            DrawGammaSetMarker(histoOmegaFWHMMeV[i], markerStyleDet[i], markerSizeDet[i]*0.55, colorDet[i] , colorDet[i]);
            histoOmegaFWHMMeV[i]->Draw("p,same,e");
            DrawGammaSetMarker(histoOmegaTrueFWHMMeV[i], markerStyleDetMC[i], markerSizeDetMC[i]*0.55, colorDetMC[i] , colorDetMC[i]);
            histoOmegaTrueFWHMMeV[i]->Draw("p,same,e");
        }
    }

    TLatex *labelLegendAMass    = new TLatex(0.13,0.06,"a)");
    SetStyleTLatex( labelLegendAMass, textSizeLabelsPixel,4);
    labelLegendAMass->SetTextFont(43);
    labelLegendAMass->Draw();


    padMassOmega->cd();
    padMassOmega->SetLogx();

    Double_t textsizeLabelsMass         = 0;
    Double_t textsizeFacMass            = 0;
    if (padMassOmega->XtoPixel(padMassOmega->GetX2()) <padMassOmega->YtoPixel(padMassOmega->GetY1()) ){
        textsizeLabelsMass              = (Double_t)textSizeLabelsPixel/padMassOmega->XtoPixel(padMassOmega->GetX2()) ;
        textsizeFacMass                 = (Double_t)1./padMassOmega->XtoPixel(padMassOmega->GetX2()) ;
    } else {
        textsizeLabelsMass              = (Double_t)textSizeLabelsPixel/padMassOmega->YtoPixel(padMassOmega->GetY1());
        textsizeFacMass                 = (Double_t)1./padMassOmega->YtoPixel(padMassOmega->GetY1());
    }
    TH2F * histo2DAllOmegaMass            = new TH2F("histo2DAllOmegaMass","histo2DAllOmegaMass",20, minX, maxX, 1000., 765, 823);
    SetStyleHistoTH2ForGraphs(histo2DAllOmegaMass, "#it{p}_{T} (GeV/#it{c})", "Peak position (MeV/#it{c}^{2})", 0.85*textsizeLabelsMass, textsizeLabelsMass, 0.85*textsizeLabelsMass,
                            textsizeLabelsMass, 0.9, 0.28/(textsizeFacMass*margin), 512, 505);
    histo2DAllOmegaMass->GetXaxis()->SetMoreLogLabels(kTRUE);
    histo2DAllOmegaMass->GetYaxis()->SetNdivisions(505);
    histo2DAllOmegaMass->GetYaxis()->SetNoExponent(kTRUE);
    histo2DAllOmegaMass->GetXaxis()->SetTickLength(0.05);
    histo2DAllOmegaMass->GetXaxis()->SetLabelOffset(-0.015);
    histo2DAllOmegaMass->DrawCopy();

    for (Int_t i = 0; i < numbersofmeas; i++){
        if(histoOmegaMass[i] && histoOmegaTrueMass[i] && availableMeas[i]){
            DrawGammaSetMarker(histoOmegaMass[i], markerStyleDet[i], markerSizeDet[i]*0.55, colorDet[i] , colorDet[i]);
            histoOmegaMass[i]->Draw("p,same,e");
            DrawGammaSetMarker(histoOmegaTrueMass[i], markerStyleDetMC[i], markerSizeDetMC[i]*0.55, colorDetMC[i] , colorDetMC[i]);
            histoOmegaTrueMass[i]->Draw("p,same,e");
        }
    }

    DrawGammaLines(minX, maxX , mesonMassExpectOmega*1000., mesonMassExpectOmega*1000.,0.1, kGray);

    TLatex *labelLegendBMass            = new TLatex(0.13,0.22,"b)");
    SetStyleTLatex( labelLegendBMass, textSizeLabelsPixel,4);
    labelLegendBMass->SetTextFont(43);
    labelLegendBMass->Draw();

    //********************************** Defintion of the Legend **************************************************
    Double_t columnsLegendMass2[3]      = {0.,0.43,0.65};
    //   Double_t rowsLegendMass2[5] = {0.8,0.6,0.4,0.2,0.01};

    // Calculate Rows
    Double_t topEntryPos = 0.7;

    Double_t stepRow = topEntryPos/numbersofmeas;

    Double_t rowsLegendMass2[6] = {0.87,topEntryPos,topEntryPos-stepRow,topEntryPos-(2*stepRow),topEntryPos-(3*stepRow),topEntryPos-(4*stepRow)};
    //           Double_t  rowsLegendMass2[7]= {0.84,0.66,0.50,0.33,0.01,0.16};
    //Double_t  rowsLegendMass2[9]= {0.84,0.66,0.51,0.50,0.331,0.33,0.01,0.16}; //setting for use without PHOS and PCM-PHOS peak positions
    //******************* Offsets ***********************
    Double_t offsetMarkerXMass2         = 0.06;
    Double_t offsetMarkerYMass2         = 0.06;
    //****************** Scale factors ******************
    Double_t scaleMarkerMass2           = 1.2;

    padMassLegend1->cd();
    //****************** first Column **************************************************
    TLatex *textMassPCM[10];
    for (Int_t i = 0; i < numbersofmeas; i++){
        if(histoOmegaMass[i] && histoOmegaTrueMass[i] && histoOmegaFWHMMeV[i] && histoOmegaTrueFWHMMeV[i] && availableMeas[i]){
            textMassPCM[i]                  = new TLatex(columnsLegendMass2[0],rowsLegendMass2[i+1],nameMeasGlobal[i].Data());
            SetStyleTLatex( textMassPCM[i], textSizeLabelsPixel,4);
            textMassPCM[i]->SetTextFont(43);
            textMassPCM[i]->Draw();
        }
    }
    //****************** second Column *************************************************
    TLatex *textMassData                = new TLatex(columnsLegendMass2[1],rowsLegendMass2[0] ,"Data");
    SetStyleTLatex( textMassData, textSizeLabelsPixel,4);
    textMassData->SetTextFont(43);
    textMassData->Draw();
    TLatex *textMassMC                  = new TLatex(columnsLegendMass2[2]-0.02 ,rowsLegendMass2[0],"MC");
    SetStyleTLatex( textMassMC, textSizeLabelsPixel,4);
    textMassMC->SetTextFont(43);
    textMassMC->Draw();

    TMarker* markerPCMOmegaMass[10];
    TMarker* markerPCMOmegaMassMC[10];
    for (Int_t i = 0; i < numbersofmeas; i++){
        if(histoOmegaMass[i] && histoOmegaTrueMass[i]&& availableMeas[i]){
            markerPCMOmegaMass[i]             = CreateMarkerFromHisto(histoOmegaMass[i],columnsLegendMass2[1]+ offsetMarkerXMass2 ,rowsLegendMass2[i+1]+ offsetMarkerYMass2 ,scaleMarkerMass2);
            markerPCMOmegaMass[i]->DrawMarker(columnsLegendMass2[1]+ offsetMarkerXMass2 ,rowsLegendMass2[i+1]+ offsetMarkerYMass2);
            markerPCMOmegaMassMC[i]           = CreateMarkerFromHisto(histoOmegaTrueMass[i],columnsLegendMass2[2]+ offsetMarkerXMass2 ,rowsLegendMass2[i+1]+ offsetMarkerYMass2 ,scaleMarkerMass2);
            markerPCMOmegaMassMC[i]->DrawMarker(columnsLegendMass2[2]+ offsetMarkerXMass2-0.04 ,rowsLegendMass2[i+1]+ offsetMarkerYMass2);
        }
    }

    canvasMassWidthOmega->Update();
    canvasMassWidthOmega->Print(Form("%s/Omega_MassAndWidth.%s",outputDir.Data(),suffix.Data()));


    // **********************************************************************************************************************
    // ******************************************* Mass and width for Eta            ****************************************
    // **********************************************************************************************************************
    if(doEta){
        cout << "plotting eta mass" << endl;
        textSizeLabelsPixel             = 50;
        canvasMassWidthOmega->cd();
        padWidthOmega->Draw();
        padMassOmega->Draw();
        padMassLegend1->Draw();

        drawLatexAdd("ALICE this thesis",0.93,0.92,textSizeLabelsRel*0.9,kFALSE,kFALSE,kTRUE);
        drawLatexAdd(collisionSystem7TeV,0.93,0.88,textSizeLabelsRel*0.9,kFALSE,kFALSE,kTRUE);
        drawLatexAdd("#omega #rightarrow #pi^{+}#pi^{-}#pi^{0}",0.93,0.84,textSizeLabelsRel*0.9,kFALSE,kFALSE,kTRUE);

        padWidthOmega->cd();
        padWidthOmega->SetLogx();

        TH2F * histo2DAllEtaFWHM    = new TH2F("histo2DAllEtaFWHM","histo2DAllEtaFWHM", 20, minX, maxX ,1000., -3, 36);
        SetStyleHistoTH2ForGraphs(histo2DAllEtaFWHM, "#it{p}_{T} (GeV/#it{c})", "Peak width (MeV/#it{c}^{2})", 0.85*textsizeLabelsWidth, textsizeLabelsWidth,
                                0.85*textsizeLabelsWidth, textsizeLabelsWidth, 0.8,0.28/(textsizeFacWidth*margin), 512, 505);
        histo2DAllEtaFWHM->GetYaxis()->SetMoreLogLabels(kTRUE);
        histo2DAllEtaFWHM->GetYaxis()->SetNdivisions(505);
        histo2DAllEtaFWHM->GetYaxis()->SetNoExponent(kTRUE);
        histo2DAllEtaFWHM->GetXaxis()->SetTickLength(0.05);
        histo2DAllEtaFWHM->GetYaxis()->SetTickLength(0.026);
        histo2DAllEtaFWHM->DrawCopy();

        for (Int_t i = 0; i < numbersofmeas; i++){
            if(histoEtaFWHMMeV[i] && histoEtaTrueFWHMMeV[i] && availableMeas[i]){
                DrawGammaSetMarker(histoEtaFWHMMeV[i], markerStyleDet[i], markerSizeDet[i]*0.55, colorDet[i] , colorDet[i]);
                histoEtaFWHMMeV[i]->Draw("p,same,e");
                DrawGammaSetMarker(histoEtaTrueFWHMMeV[i], markerStyleDetMC[i], markerSizeDetMC[i]*0.55, colorDetMC[i] , colorDetMC[i]);
                histoEtaTrueFWHMMeV[i]->Draw("p,same,e");
            }
        }

        labelLegendAMass->Draw();



        padMassOmega->cd();
        padMassOmega->SetLogx();

        TH2F * histo2DAllEtaMass            = new TH2F("histo2DAllEtaMass","histo2DAllEtaMass",20, minX, maxX, 1000.,  539, 563.0);
        SetStyleHistoTH2ForGraphs(histo2DAllEtaMass, "#it{p}_{T} (GeV/#it{c})", "Peak position (MeV/#it{c}^{2})", 0.85*textsizeLabelsMass, textsizeLabelsMass, 0.85*textsizeLabelsMass,
                                textsizeLabelsMass, 0.9, 0.28/(textsizeFacMass*margin), 512, 505);
        histo2DAllEtaMass->GetXaxis()->SetMoreLogLabels(kTRUE);
        histo2DAllEtaMass->GetYaxis()->SetNdivisions(505);
        histo2DAllEtaMass->GetYaxis()->SetNoExponent(kTRUE);
        histo2DAllEtaMass->GetXaxis()->SetTickLength(0.05);
        histo2DAllEtaMass->GetXaxis()->SetLabelOffset(-0.015);
        histo2DAllEtaMass->DrawCopy();

        for (Int_t i = 0; i < numbersofmeas; i++){
            if(histoEtaMass[i] && histoEtaTrueMass[i] && availableMeas[i]){
                DrawGammaSetMarker(histoEtaMass[i], markerStyleDet[i], markerSizeDet[i]*0.55, colorDet[i] , colorDet[i]);
                histoEtaMass[i]->Draw("p,same,e");
                DrawGammaSetMarker(histoEtaTrueMass[i], markerStyleDetMC[i], markerSizeDetMC[i]*0.55, colorDetMC[i] , colorDetMC[i]);
                histoEtaTrueMass[i]->Draw("p,same,e");
            }
        }

        DrawGammaLines(minX, maxX , mesonMassExpectEta*1000., mesonMassExpectEta*1000.,0.1, kGray);

        labelLegendBMass->Draw();


        padMassLegend1->cd();
        //****************** first Column **************************************************
        for (Int_t i = 0; i < numbersofmeas; i++){
            if(histoEtaMass[i] && histoEtaTrueMass[i] && histoEtaFWHMMeV[i] && histoEtaTrueFWHMMeV[i] && availableMeas[i]){
                textMassPCM[i]->Draw();
            }
        }
        //****************** second Column *************************************************
        textMassData->Draw();
        textMassMC->Draw();

        TMarker* markerPCMEtaMass[10];
        TMarker* markerPCMEtaMassMC[10];
        for (Int_t i = 0; i < numbersofmeas; i++){
            if(histoEtaMass[i] && histoEtaTrueMass[i]&& availableMeas[i]){
                markerPCMEtaMass[i]             = CreateMarkerFromHisto(histoEtaMass[i],columnsLegendMass2[1]+ offsetMarkerXMass2 ,rowsLegendMass2[i+1]+ offsetMarkerYMass2 ,scaleMarkerMass2);
                markerPCMEtaMass[i]->DrawMarker(columnsLegendMass2[1]+ offsetMarkerXMass2 ,rowsLegendMass2[i+1]+ offsetMarkerYMass2);
                markerPCMEtaMassMC[i]           = CreateMarkerFromHisto(histoEtaTrueMass[i],columnsLegendMass2[2]+ offsetMarkerXMass2 ,rowsLegendMass2[i+1]+ offsetMarkerYMass2 ,scaleMarkerMass2);
                markerPCMEtaMassMC[i]->DrawMarker(columnsLegendMass2[2]+ offsetMarkerXMass2-0.04 ,rowsLegendMass2[i+1]+ offsetMarkerYMass2);
            }
        }

        canvasMassWidthOmega->Update();
        canvasMassWidthOmega->Print(Form("%s/Eta_MassAndWidth.%s",outputDir.Data(),suffix.Data()));
    }

    minX                               = 0.3;
    maxX                               = 27;


    // **********************************************************************************************************************
    // ******************************** Acceptance * Efficiency for Omega****************************************************
    Double_t minPt              = 1.3;
    Double_t maxPt              = 22.5;

    textSizeLabelsPixel             = 40;
    textSizeLabelsRel      = 40./1200;
    cout << textSizeLabelsRel << endl;

    TCanvas* canvasAcceptanceTimesEff       = new TCanvas("canvasAcceptanceTimesEff", "", 200, 10, 1200, 1100);  // gives the page size
    canvasAcceptanceTimesEff->cd();
    DrawGammaCanvasSettings( canvasAcceptanceTimesEff,  0.1, 0.01, 0.015, 0.095);
    canvasAcceptanceTimesEff->SetLogy(1);
    canvasAcceptanceTimesEff->SetLogx(1);

    TH2F* histo2DAccEff                = new TH2F("histo2DAccEff", "histo2DAccEff",1000, minPt,  maxPt, 1000, 4e-4, 1. );
    SetStyleHistoTH2ForGraphs( histo2DAccEff, "#it{p}_{T} (GeV/#it{c})", Form("%s%s","#it{#varepsilon} = 2#pi#upoint#Delta","#it{y}#upoint#it{A}#upoint#it{#varepsilon}_{rec}"),
                            0.85*textSizeLabelsRel, textSizeLabelsRel, 0.85*textSizeLabelsRel, textSizeLabelsRel, 0.9, 1);//(#times #epsilon_{pur})
                            histo2DAccEff->GetYaxis()->SetLabelOffset(0.001);
                            histo2DAccEff->GetXaxis()->SetLabelOffset(-0.01);
                            histo2DAccEff->GetXaxis()->SetMoreLogLabels(kTRUE);
                            histo2DAccEff->DrawCopy();
    histo2DAccEff->DrawCopy();

    TLegend* legendEffiAccTimesEffOmega           = GetAndSetLegend2(0.7, 0.13, 0.95, 0.13+(numbersofmeas*textSizeLabelsRel),textSizeLabelsPixel);
    for (Int_t i = 0; i < numbersofmeas; i++){
        if(histoOmegaAccTimesEff[i] && availableMeas[i]){
            DrawGammaSetMarker(histoOmegaAccTimesEff[i], markerStyleDet[i], markerSizeDet[i]*0.55, colorDet[i] , colorDet[i]);
            histoOmegaAccTimesEff[i]->Draw("p,same,e");
            legendEffiAccTimesEffOmega->AddEntry(histoOmegaAccTimesEff[i],nameMeasGlobal[i].Data(),"p");
        }

        if(histoOmegaAccTimesEff_EMC7[i] && availableMeas[i]){
            DrawGammaSetMarker(histoOmegaAccTimesEff_EMC7[i], markerStyleDetMC[i], markerSizeDet[i]*0.55, colorDet[i] , colorDet[i]);
            histoOmegaAccTimesEff_EMC7[i]->Draw("p,same,e");
            legendEffiAccTimesEffOmega->AddEntry(histoOmegaAccTimesEff_EMC7[i],Form("%s EMC7",nameMeasGlobal[i].Data()),"p");
        }
    }

    legendEffiAccTimesEffOmega->Draw();

    drawLatexAdd("ALICE this thesis",0.15,0.92,textSizeLabelsRel);
    drawLatexAdd(collisionSystem7TeV.Data(),0.15,0.88,textSizeLabelsRel);
    drawLatexAdd("#omega #rightarrow #pi^{+}#pi^{-}#pi^{0}",0.15,0.84,textSizeLabelsRel);


    canvasAcceptanceTimesEff->Update();
    canvasAcceptanceTimesEff->Print(Form("%s/Omega_AcceptanceTimesEff.%s",outputDir.Data(),suffix.Data()));



    // **********************************************************************************************************************
    // ******************************** Only Acceptance  for Omega***********************************************************
    // **********************************************************************************************************************
    minPt              = 0.;
    maxPt              = 22.5;

    textSizeLabelsPixel             = 40;
    textSizeLabelsRel      = 40./1200;
    cout << textSizeLabelsRel << endl;

    // reuse old canvas
    canvasAcceptanceTimesEff->Clear();
    canvasAcceptanceTimesEff->cd();
    DrawGammaCanvasSettings( canvasAcceptanceTimesEff,  0.1, 0.01, 0.015, 0.095);
    canvasAcceptanceTimesEff->SetLogy(1);
    canvasAcceptanceTimesEff->SetLogx(0);

    TH2F* histo2DAccOmega                = new TH2F("histo2DAccOmega", "histo2DAccOmega",100, minPt,  maxPt, 100, 2e-3, 1.1 );
    SetStyleHistoTH2ForGraphs( histo2DAccOmega, "#it{p}_{T} (GeV/#it{c})", Form("A_{#omega}"),
                            0.85*textSizeLabelsRel, textSizeLabelsRel, 0.85*textSizeLabelsRel, textSizeLabelsRel, 0.9, 1);
//    histo2DAccOmega->GetYaxis()->SetLabelOffset(0.001);
//    histo2DAccOmega->GetXaxis()->SetLabelOffset(-0.01);
//    histo2DAccOmega->GetXaxis()->SetMoreLogLabels(kTRUE);
    histo2DAccOmega->DrawCopy();

    TLegend* legendEffiAccOmega           = GetAndSetLegend2(0.7, 0.13, 0.95, 0.13+(numbersofmeas*textSizeLabelsRel),textSizeLabelsPixel);
    for (Int_t i = 0; i < numbersofmeas; i++){
        if(histoOmegaAcc[i] && availableMeas[i]){
            DrawGammaSetMarker(histoOmegaAcc[i], markerStyleDet[i], markerSizeDet[i]*0.55, colorDet[i] , colorDet[i]);
            histoOmegaAcc[i]->Draw("p,same,e");
            legendEffiAccOmega->AddEntry(histoOmegaAcc[i],nameMeasGlobal[i].Data(),"p");
        }

        if(histoOmegaAcc_EMC7[i] && availableMeas[i]){
            DrawGammaSetMarker(histoOmegaAcc_EMC7[i], markerStyleDetMC[i], markerSizeDet[i]*0.55, colorDet[i] , colorDet[i]);
            histoOmegaAcc_EMC7[i]->Draw("p,same,e");
            legendEffiAccOmega->AddEntry(histoOmegaAcc_EMC7[i],Form("%s EMC7",nameMeasGlobal[i].Data()),"p");
        }
    }

    legendEffiAccOmega->Draw();

    drawLatexAdd("ALICE this thesis",0.96,0.92,textSizeLabelsRel,kFALSE,kFALSE,kTRUE);
    drawLatexAdd(collisionSystem7TeV.Data(),0.96,0.88,textSizeLabelsRel,kFALSE,kFALSE,kTRUE);
    drawLatexAdd("#omega #rightarrow #pi^{+}#pi^{-}#pi^{0}",0.96,0.84,textSizeLabelsRel,kFALSE,kFALSE,kTRUE);


    canvasAcceptanceTimesEff->Update();
    canvasAcceptanceTimesEff->Print(Form("%s/Omega_Acceptance.%s",outputDir.Data(),suffix.Data()));

    // **********************************************************************************************************************
    // ******************************** Acceptance * Efficiency for Eta *****************************************************
    if(doEta){
        minPt              = 1.3;
        maxPt              = 21.;

        textSizeLabelsPixel             = 40;
        textSizeLabelsRel      = 40./1200;
        cout << textSizeLabelsRel << endl;

        canvasAcceptanceTimesEff       = new TCanvas("canvasAcceptanceTimesEff", "", 200, 10, 1200, 1100);  // gives the page size
        canvasAcceptanceTimesEff->cd();
        DrawGammaCanvasSettings( canvasAcceptanceTimesEff,  0.1, 0.01, 0.015, 0.095);
        canvasAcceptanceTimesEff->SetLogy(1);
        canvasAcceptanceTimesEff->SetLogx(1);

        histo2DAccEff                = new TH2F("histo2DAccEff", "histo2DAccEff",1000, minPt,  maxPt, 1000, 4e-4, 0.2 );
        SetStyleHistoTH2ForGraphs( histo2DAccEff, "#it{p}_{T} (GeV/#it{c})", Form("%s%s","#it{#varepsilon} = 2#pi#upoint#Delta","#it{y}#upoint#it{A}#upoint#it{#varepsilon}_{rec}"),
                                0.85*textSizeLabelsRel, textSizeLabelsRel, 0.85*textSizeLabelsRel, textSizeLabelsRel, 0.9, 1);//(#times #epsilon_{pur})
                                histo2DAccEff->GetYaxis()->SetLabelOffset(0.001);
                                histo2DAccEff->GetXaxis()->SetLabelOffset(-0.01);
                                histo2DAccEff->GetXaxis()->SetMoreLogLabels(kTRUE);
                                histo2DAccEff->DrawCopy();
        histo2DAccEff->DrawCopy();

        TLegend* legendEffiAccTimesEffEta           = GetAndSetLegend2(0.7, 0.13, 0.95, 0.13+(numbersofmeas*textSizeLabelsRel),textSizeLabelsPixel);
        for (Int_t i = 0; i < numbersofmeas; i++){
            if(histoEtaAccTimesEff[i] && availableMeas[i]){
                DrawGammaSetMarker(histoEtaAccTimesEff[i], markerStyleDet[i], markerSizeDet[i]*0.55, colorDet[i] , colorDet[i]);
                histoEtaAccTimesEff[i]->Draw("p,same,e");
                legendEffiAccTimesEffEta->AddEntry(histoEtaAccTimesEff[i],nameMeasGlobal[i].Data(),"p");
            }
        }

        legendEffiAccTimesEffEta->Draw();

        drawLatexAdd("ALICE this thesis",0.15,0.92,textSizeLabelsRel);
        drawLatexAdd(collisionSystem7TeV.Data(),0.15,0.88,textSizeLabelsRel);
        drawLatexAdd("#eta#rightarrow #pi^{+}#pi^{-}#pi^{0}",0.15,0.84,textSizeLabelsRel);


        canvasAcceptanceTimesEff->Update();
        canvasAcceptanceTimesEff->Print(Form("%s/Eta_AcceptanceTimesEff.%s",outputDir.Data(),suffix.Data()));
    }
    // **********************************************************************************************************************
    // ******************************** Only Acceptance  for Eta***********************************************************
    // **********************************************************************************************************************
    if(doEta){
        minPt              = 0.;
        maxPt              = 17.;

        textSizeLabelsPixel             = 40;
        textSizeLabelsRel      = 40./1200;
        cout << textSizeLabelsRel << endl;

        // reuse old canvas
        canvasAcceptanceTimesEff->Clear();
        canvasAcceptanceTimesEff->cd();
        DrawGammaCanvasSettings( canvasAcceptanceTimesEff,  0.1, 0.01, 0.015, 0.095);
        canvasAcceptanceTimesEff->SetLogy(1);
        canvasAcceptanceTimesEff->SetLogx(0);

        TH2F* histo2DAccEta                = new TH2F("histo2DAccEta", "histo2DAccEta",100, minPt,  maxPt, 100, 2e-3, 1.1 );
        SetStyleHistoTH2ForGraphs( histo2DAccEta, "#it{p}_{T} (GeV/#it{c})", Form("A_{#eta}"),
                                0.85*textSizeLabelsRel, textSizeLabelsRel, 0.85*textSizeLabelsRel, textSizeLabelsRel, 0.9, 1);
    //    histo2DAccEta->GetYaxis()->SetLabelOffset(0.001);
    //    histo2DAccEta->GetXaxis()->SetLabelOffset(-0.01);
    //    histo2DAccEta->GetXaxis()->SetMoreLogLabels(kTRUE);
        histo2DAccEta->DrawCopy();

        TLegend* legendEffiAccEta           = GetAndSetLegend2(0.7, 0.13, 0.95, 0.13+(numbersofmeas*textSizeLabelsRel),textSizeLabelsPixel);
        for (Int_t i = 0; i < numbersofmeas; i++){
            if(histoEtaAcc[i] && availableMeas[i]){
                DrawGammaSetMarker(histoEtaAcc[i], markerStyleDet[i], markerSizeDet[i]*0.55, colorDet[i] , colorDet[i]);
                histoEtaAcc[i]->Draw("p,same,e");
                legendEffiAccEta->AddEntry(histoEtaAcc[i],nameMeasGlobal[i].Data(),"p");
            }
        }

        legendEffiAccOmega->Draw();

        drawLatexAdd("ALICE this thesis",0.96,0.92,textSizeLabelsRel,kFALSE,kFALSE,kTRUE);
        drawLatexAdd(collisionSystem7TeV.Data(),0.96,0.88,textSizeLabelsRel,kFALSE,kFALSE,kTRUE);
        drawLatexAdd("#eta #rightarrow #pi^{+}#pi^{-}#pi^{0}",0.96,0.84,textSizeLabelsRel,kFALSE,kFALSE,kTRUE);


        canvasAcceptanceTimesEff->Update();
        canvasAcceptanceTimesEff->Print(Form("%s/Eta_Acceptance.%s",outputDir.Data(),suffix.Data()));

    }
    // **********************************************************************************************************************
    // ******************************** Only Efficiency  for Omega***********************************************************
    // **********************************************************************************************************************
    minPt              = 0.;
    maxPt              = 22.5;

    textSizeLabelsPixel             = 40;
    textSizeLabelsRel      = 40./1200;
    cout << textSizeLabelsRel << endl;

    // reuse old canvas
    canvasAcceptanceTimesEff->Clear();
    canvasAcceptanceTimesEff->cd();
    DrawGammaCanvasSettings( canvasAcceptanceTimesEff,  0.1, 0.01, 0.015, 0.095);
    canvasAcceptanceTimesEff->SetLogy(1);
    canvasAcceptanceTimesEff->SetLogx(0);

    TH2F* histo2DEffOmega                = new TH2F("histo2DEffOmega", "histo2DEffOmega",100, minPt,  maxPt, 100, 2e-5 , 1.1 );
    SetStyleHistoTH2ForGraphs( histo2DEffOmega, "#it{p}_{T} (GeV/#it{c})", Form("#varepsilon_{#omega}"),
                            0.85*textSizeLabelsRel, textSizeLabelsRel, 0.85*textSizeLabelsRel, textSizeLabelsRel, 0.9, 1);
//    histo2DEffOmega->GetYaxis()->SetLabelOffset(0.001);
//    histo2DEffOmega->GetXaxis()->SetLabelOffset(-0.01);
//    histo2DEffOmega->GetXaxis()->SetMoreLogLabels(kTRUE);
    histo2DEffOmega->DrawCopy();

    TLegend* legendEffiEffOmega           =  GetAndSetLegend2(0.7, 0.13, 0.95, 0.13+(numbersofmeas*textSizeLabelsRel),textSizeLabelsPixel);
    for (Int_t i = 0; i < numbersofmeas; i++){
        if(histoOmegaTrueEffPt[i] && availableMeas[i]){
            DrawGammaSetMarker(histoOmegaTrueEffPt[i], markerStyleDet[i], markerSizeDet[i]*0.55, colorDet[i] , colorDet[i]);
            histoOmegaTrueEffPt[i]->Draw("p,same,e");
            legendEffiEffOmega->AddEntry(histoOmegaTrueEffPt[i],nameMeasGlobal[i].Data(),"p");
        }

        if(histoOmegaTrueEffPt_EMC7[i] && availableMeas[i]){
            DrawGammaSetMarker(histoOmegaTrueEffPt_EMC7[i], markerStyleDetMC[i], markerSizeDet[i]*0.55, colorDet[i] , colorDet[i]);
            histoOmegaTrueEffPt_EMC7[i]->Draw("p,same,e");
            legendEffiEffOmega->AddEntry(histoOmegaTrueEffPt_EMC7[i],Form("%s EMC7",nameMeasGlobal[i].Data()),"p");
        }
    }

    legendEffiEffOmega->Draw();

    drawLatexAdd("ALICE this thesis",0.96,0.92,textSizeLabelsRel,kFALSE,kFALSE,kTRUE);
    drawLatexAdd(collisionSystem7TeV.Data(),0.96,0.88,textSizeLabelsRel,kFALSE,kFALSE,kTRUE);
    drawLatexAdd("#omega #rightarrow #pi^{+}#pi^{-}#pi^{0}",0.96,0.84,textSizeLabelsRel,kFALSE,kFALSE,kTRUE);


    canvasAcceptanceTimesEff->Update();
    canvasAcceptanceTimesEff->Print(Form("%s/Omega_Efficiency.%s",outputDir.Data(),suffix.Data()));


    // **********************************************************************************************************************
    // ******************************** Only Efficiency  for Eta  ***********************************************************
    // **********************************************************************************************************************
    if(doEta){
        minPt              = 0.;
        maxPt              = 17.;

        textSizeLabelsPixel             = 40;
        textSizeLabelsRel      = 40./1200;
        cout << textSizeLabelsRel << endl;

        // reuse old canvas
        canvasAcceptanceTimesEff->Clear();
        canvasAcceptanceTimesEff->cd();
        DrawGammaCanvasSettings( canvasAcceptanceTimesEff,  0.1, 0.01, 0.015, 0.095);
        canvasAcceptanceTimesEff->SetLogy(1);
        canvasAcceptanceTimesEff->SetLogx(0);

        TH2F* histo2DEffEta                = new TH2F("histo2DEffEta", "histo2DEffEta",100, minPt,  maxPt, 100, 2e-5 , 1.1 );
        SetStyleHistoTH2ForGraphs( histo2DEffEta, "#it{p}_{T} (GeV/#it{c})", Form("#varepsilon_{#eta}"),
                                0.85*textSizeLabelsRel, textSizeLabelsRel, 0.85*textSizeLabelsRel, textSizeLabelsRel, 0.9, 1);
    //    histo2DEffEta->GetYaxis()->SetLabelOffset(0.001);
    //    histo2DEffEta->GetXaxis()->SetLabelOffset(-0.01);
    //    histo2DEffEta->GetXaxis()->SetMoreLogLabels(kTRUE);
        histo2DEffEta->DrawCopy();

        TLegend* legendEffiEffEta           = GetAndSetLegend2(0.7, 0.13, 0.95, 0.13+(numbersofmeas*textSizeLabelsRel),textSizeLabelsPixel);
        for (Int_t i = 0; i < numbersofmeas; i++){
            if(histoEtaTrueEffPt[i] && availableMeas[i]){
                DrawGammaSetMarker(histoEtaTrueEffPt[i], markerStyleDet[i], markerSizeDet[i]*0.55, colorDet[i] , colorDet[i]);
                histoEtaTrueEffPt[i]->Draw("p,same,e");
                legendEffiEffEta->AddEntry(histoEtaTrueEffPt[i],nameMeasGlobal[i].Data(),"p");
            }
        }

        legendEffiEffEta->Draw();

        drawLatexAdd("ALICE this thesis",0.96,0.92,textSizeLabelsRel,kFALSE,kFALSE,kTRUE);
        drawLatexAdd(collisionSystem7TeV.Data(),0.96,0.88,textSizeLabelsRel,kFALSE,kFALSE,kTRUE);
        drawLatexAdd("#eta #rightarrow #pi^{+}#pi^{-}#pi^{0}",0.96,0.84,textSizeLabelsRel,kFALSE,kFALSE,kTRUE);


        canvasAcceptanceTimesEff->Update();
        canvasAcceptanceTimesEff->Print(Form("%s/Eta_Efficiency.%s",outputDir.Data(),suffix.Data()));
    }
    
    // **********************************************************************************************************************
    // ******************************** Acceptance * Efficiency for eta->pipipi and eta->gammagamma measurement *************
    // **********************************************************************************************************************
//    TH2F * histo2DAccEffEta;

//    // reuse old canvas
//    canvasAcceptanceTimesEff       = new TCanvas("canvasAcceptanceTimesEff", "", 200, 10, 1200, 1100);  // gives the page size
//    canvasAcceptanceTimesEff->cd();
//    DrawGammaCanvasSettings( canvasAcceptanceTimesEff,  0.1, 0.01, 0.015, 0.095);
//    canvasAcceptanceTimesEff->SetLogy(1);
//    canvasAcceptanceTimesEff->SetLogx(1);
//    minX=0.3;
//    histo2DAccEffEta                = new TH2F("histo2DAccEffEta", "histo2DAccEffEta",1000, minX,  maxX, 1000, 8e-6, 0.5 );
//    SetStyleHistoTH2ForGraphs( histo2DAccEffEta, "#it{p}_{T} (GeV/#it{c})", Form("%s%s","#it{#varepsilon} = 2#pi#upointBR#upoint#Delta","#it{y}#upoint#it{A}#upoint#it{#varepsilon}_{rec} / #it{P}"),
//                               0.85*textSizeLabelsRel, textSizeLabelsRel, 0.85*textSizeLabelsRel, textSizeLabelsRel, 0.9, 1);//(#times #epsilon_{pur})
//    histo2DAccEffEta->GetYaxis()->SetLabelOffset(0.001);
//    histo2DAccEffEta->GetXaxis()->SetLabelOffset(-0.01);
//    histo2DAccEffEta->GetXaxis()->SetMoreLogLabels(kTRUE);
//    histo2DAccEffEta->DrawCopy();

//    for (Int_t i = 0; i < numbersofmeas; i++){
//        if(histoEtaAccTimesEffBR[i] && availableMeas[i]){
//            DrawGammaSetMarker(histoEtaAccTimesEffBR[i], markerStyleDet[i], markerSizeDet[i]*0.55, colorDet[i] , colorDet[i]);
//            // Draw Eta->gamma gamma measurment for comparison. For now use MC markers
//            if(availableMeasEtaGG[i]) DrawGammaSetMarker(histoEtaGGAccTimesEff[i], markerStyleDetMC[i], markerSizeDet[i]*0.55, colorDet[i] , colorDet[i]);
//            histoEtaAccTimesEffBR[i]->Draw("p,same,e");
//            if(availableMeasEtaGG[i]) histoEtaGGAccTimesEff[i]->Draw("p,same,e");
//        }
//    }

//    drawLatexAdd("ALICE this thesis",0.15,0.92,textSizeLabelsRel,kFALSE);
//    drawLatexAdd(collisionSystem7TeV.Data(),0.15,0.87,textSizeLabelsRel,kFALSE);
//    drawLatexAdd("#eta",0.15,0.82,textSizeLabelsRel,kFALSE);

//    padAccEffLegend1            = new TPad("padAccEffLegend1", "", 0.53, 0.12, 0.93, 0.34,-1, -1, -2);
//    DrawGammaPadSettings( padAccEffLegend1, 0., 0., 0., 0.);
//    padAccEffLegend1->SetFillStyle(0);
//    padAccEffLegend1->Draw();
//    //********************************** Defintion of the Legend **************************************************
//    Double_t columnsLegendAccEff3[3]      = {0.,0.57,0.84};
//    //   Double_t rowsLegendMass2[5] = {0.8,0.6,0.4,0.2,0.01};
//    Double_t rowsLegendAccEff3[6] = {0.84,0.66,0.50,0.33,0.16,0.01};
//    //           Double_t  rowsLegendMass2[7]= {0.84,0.66,0.50,0.33,0.01,0.16};
//    //Double_t  rowsLegendMass2[9]= {0.84,0.66,0.51,0.50,0.331,0.33,0.01,0.16}; //setting for use without PHOS and PCM-PHOS peak positions
//    //******************* Offsets ***********************
//    offsetMarkerXMass2         = 0.1;
//    offsetMarkerYMass2         = 0.1;
//    //****************** Scale factors ******************
//    scaleMarkerMass2           = 1.2;

//    padAccEffLegend1->cd();
//    //****************** first Column **************************************************
//    for (Int_t i = 0; i < numbersofmeas; i++){
//         if(histoOmegaAccTimesEff[i] && availableMeas[i]){
//            textAccEffPCM[i]                  = new TLatex(columnsLegendAccEff3[0],rowsLegendAccEff3[i+1],nameMeasGlobal[i].Data());
//            SetStyleTLatex( textAccEffPCM[i], textSizeLabelsPixel,4);
//            textAccEffPCM[i]->SetTextFont(43);
//            textAccEffPCM[i]->Draw();
//        }
//    }
//    //****************** second Column *************************************************
//    TLatex *testAccEffEta                = new TLatex(columnsLegendAccEff3[1]-0.03,rowsLegendAccEff3[0] ,"#pi^{+}#pi^{-}#pi^{0}");
//    SetStyleTLatex( testAccEffEta, textSizeLabelsPixel,4);
//    testAccEffEta->SetTextFont(43);
//    testAccEffEta->Draw();
//    TLatex *textAccEffEtaGG                  = new TLatex(columnsLegendAccEff3[2]-0.03 ,rowsLegendAccEff3[0]," #gamma#gamma");
//    SetStyleTLatex( textAccEffEtaGG, textSizeLabelsPixel,4);
//    textAccEffEtaGG->SetTextFont(43);
//    textAccEffEtaGG->Draw();

//    TMarker* markerEtaAccEff[10];
//    TMarker* markerEtaGGAccEff[10];
//    for (Int_t i = 0; i < numbersofmeas; i++){
//        if(histoEtaAccTimesEff[i] && availableMeas[i]){
//            markerEtaAccEff[i]             = CreateMarkerFromHisto(histoEtaAccTimesEffBR[i],columnsLegendAccEff3[1]+ offsetMarkerXMass2 ,rowsLegendAccEff3[i+1]+ offsetMarkerYMass2 ,scaleMarkerMass2);
//            markerEtaAccEff[i]->DrawMarker(columnsLegendAccEff3[1]+ offsetMarkerXMass2 ,rowsLegendAccEff3[i+1]+ offsetMarkerYMass2);
//        }
//        if(availableMeasEtaGG[i]){
//            markerEtaGGAccEff[i]           = CreateMarkerFromHisto(histoEtaGGAccTimesEff[i],columnsLegendAccEff3[2]+ offsetMarkerXMass2 ,rowsLegendAccEff3[i+1]+ offsetMarkerYMass2 ,scaleMarkerMass2);
//            markerEtaGGAccEff[i]->DrawMarker(columnsLegendAccEff3[2]+ offsetMarkerXMass2-0.04 ,rowsLegendAccEff3[i+1]+ offsetMarkerYMass2);
//        }
//    }

//    canvasAcceptanceTimesEff->Update();
//    canvasAcceptanceTimesEff->Print(Form("%s/EtaEta_AcceptanceTimesEff.%s",outputDir.Data(),suffix.Data()));

    // **********************************************************************************************************************
    // ******************************** Acceptance * Efficiency for Omega and eta *******************************************
    // **********************************************************************************************************************
    if(doEta){
        textSizeLabelsPixel             = 55;
        textSizeLabelsRel      = 55./1200;
        cout << textSizeLabelsRel << endl;
        canvasAcceptanceTimesEff->Clear();
        canvasAcceptanceTimesEff->cd();
        DrawGammaCanvasSettings( canvasAcceptanceTimesEff,  0.1, 0.01, 0.015, 0.095);
        canvasAcceptanceTimesEff->SetLogy(1);
        canvasAcceptanceTimesEff->SetLogx(1);

        minX=1.5;

        histo2DAccEff                = new TH2F("histo2DAccEff", "histo2DAccEff",1000, minX,  maxX, 1000, 8e-5, 0.5 );
        SetStyleHistoTH2ForGraphs( histo2DAccEff, "#it{p}_{T} (GeV/#it{c})", Form("%s%s","#it{#varepsilon} = 2#pi#upoint#Delta","#it{y}#upoint#it{A}#upoint#it{#varepsilon}_{rec} / #it{P}"),
                                0.85*textSizeLabelsRel, textSizeLabelsRel, 0.85*textSizeLabelsRel, textSizeLabelsRel, 0.9, 1);//(#times #epsilon_{pur})
                                histo2DAccEff->GetYaxis()->SetLabelOffset(0.001);
                                histo2DAccEff->GetXaxis()->SetLabelOffset(-0.01);
                                histo2DAccEff->GetXaxis()->SetMoreLogLabels(kTRUE);
                                histo2DAccEff->DrawCopy();

        for (Int_t i = 0; i < numbersofmeas; i++){
            if(histoOmegaAccTimesEff[i] && availableMeas[i]){
                DrawGammaSetMarker(histoOmegaAccTimesEff[i], markerStyleDet[i], markerSizeDet[i]*0.55, colorDet[i] , colorDet[i]);
                DrawGammaSetMarker(histoEtaAccTimesEff[i], markerStyleDetMC[i], markerSizeDet[i]*0.55, colorDet[i] , colorDet[i]);
                histoOmegaAccTimesEff[i]->Draw("p,same,e");
                histoEtaAccTimesEff[i]->Draw("p,same,e");
            }
        }

        drawLatexAdd("ALICE this thesis",0.15,0.92,textSizeLabelsRel,kFALSE);
        drawLatexAdd(collisionSystem7TeV.Data(),0.15,0.87,textSizeLabelsRel,kFALSE);
        drawLatexAdd("X #rightarrow #pi^{+}#pi^{-}#pi^{0}",0.15,0.82,textSizeLabelsRel,kFALSE);

        TPad* padAccEffLegend1            = new TPad("padAccEffLegend1", "", 0.53, 0.05, 0.93, 0.40,-1, -1, -2);
        DrawGammaPadSettings( padAccEffLegend1, 0., 0., 0., 0.);
        padAccEffLegend1->SetFillStyle(0);
        padAccEffLegend1->Draw();

        //********************************** Defintion of the Legend **************************************************
        Double_t columnsLegendAccEff2[3]      = {0.,0.57,0.84};
        //   Double_t rowsLegendMass2[5] = {0.8,0.6,0.4,0.2,0.01};
        Double_t rowsLegendAccEff2[6] = {0.84,0.66,0.50,0.33,0.16,0.01};
        //           Double_t  rowsLegendMass2[7]= {0.84,0.66,0.50,0.33,0.01,0.16};
        //Double_t  rowsLegendMass2[9]= {0.84,0.66,0.51,0.50,0.331,0.33,0.01,0.16}; //setting for use without PHOS and PCM-PHOS peak positions
        //******************* Offsets ***********************
        offsetMarkerXMass2         = 0.1;
        offsetMarkerYMass2         = 0.1;
        //****************** Scale factors ******************
        scaleMarkerMass2           = 1.2;

        padAccEffLegend1->cd();
        //****************** first Column **************************************************
        TLatex* textAccEffPCM[11];
        for (Int_t i = 0; i < numbersofmeas; i++){
            if(histoOmegaAccTimesEff[i] && availableMeas[i]){
                textAccEffPCM[i]                  = new TLatex(columnsLegendAccEff2[0],rowsLegendAccEff2[i+1],nameMeasGlobal[i].Data());
                SetStyleTLatex( textAccEffPCM[i], textSizeLabelsPixel,4);
                textAccEffPCM[i]->SetTextFont(43);
                textAccEffPCM[i]->Draw();
            }
        }
        //****************** second Column *************************************************
        TLatex* testAccEffOmega                = new TLatex(columnsLegendAccEff2[1]+0.02,rowsLegendAccEff2[0] ," #omega");
        SetStyleTLatex( testAccEffOmega, textSizeLabelsPixel,4);
        testAccEffOmega->SetTextFont(43);
        testAccEffOmega->Draw();
        TLatex* textAccEffEta                  = new TLatex(columnsLegendAccEff2[2]-0.01,rowsLegendAccEff2[0]," #eta");
        SetStyleTLatex( textAccEffEta, textSizeLabelsPixel,4);
        textAccEffEta->SetTextFont(43);
        textAccEffEta->Draw();

        TMarker* markerOmegaAccEff[11];
        TMarker* markerEtaAccEff[11];
        for (Int_t i = 0; i < numbersofmeas; i++){
            if(histoOmegaAccTimesEff[i] && availableMeas[i]){
                markerOmegaAccEff[i]             = CreateMarkerFromHisto(histoOmegaAccTimesEff[i],columnsLegendAccEff2[1]+ offsetMarkerXMass2 ,rowsLegendAccEff2[i+1]+ offsetMarkerYMass2 ,scaleMarkerMass2);
                markerOmegaAccEff[i]->DrawMarker(columnsLegendAccEff2[1]+ offsetMarkerXMass2 ,rowsLegendAccEff2[i+1]+ offsetMarkerYMass2);

                markerEtaAccEff[i]           = CreateMarkerFromHisto(histoEtaAccTimesEff[i],columnsLegendAccEff2[2]+ offsetMarkerXMass2 ,rowsLegendAccEff2[i+1]+ offsetMarkerYMass2 ,scaleMarkerMass2);
                markerEtaAccEff[i]->DrawMarker(columnsLegendAccEff2[2]+ offsetMarkerXMass2-0.04 ,rowsLegendAccEff2[i+1]+ offsetMarkerYMass2);
            }
        }

        canvasAcceptanceTimesEff->Update();
        canvasAcceptanceTimesEff->Print(Form("%s/OmegaEta_AcceptanceTimesEff.%s",outputDir.Data(),suffix.Data()));
    }


    // **********************************************************************************************************************
    // ************************* Saving only fits to final results **********************************************************
    // **********************************************************************************************************************

       TString nameOutputCommonFileFitsOnly    = Form("%s/FitsPaperPP7TeV_%s.root",outputDir.Data(), dateForOutput.Data());
       TFile fFitsResults(nameOutputCommonFileFitsOnly.Data(), "RECREATE");

            // fits for pi0
           if(fitInvXSectionPi0) {
               fitInvXSectionPi0->Write("TsallisFitPi0");
           } else{  // if fit is not found, try fit from Daniels thesis
               fitTsallisPi0XSecComb7TeVThesis->Write("TsallisFitPi0");
           }


           // fits for omega
           if(fitInvXSectionOmega) fitInvXSectionOmega->Write("TsallisFitOmega");

       fFitsResults.Close();


}
