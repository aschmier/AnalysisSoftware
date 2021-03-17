//****************************************************************************
//********* provided by Gamma Conversion Group *******************************
//********* $ALICE_ROOT/PWGGA/GammaConv **************************************
//***** https://twiki.cern.ch/twiki/bin/view/ALICE/PWG4GammaConversion *******
//****************************************************************************

#ifndef GAMMACONV_ExtractSignalTemplates
#define GAMMACONV_ExtractSignalTemplates

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <TROOT.h>
#include <TApplication.h>
#include <TPaveLabel.h>
#include <TSystem.h>
#include <TFrame.h>
#include <TStyle.h>
#include <TString.h>
#include <TLatex.h>
// #include "TGaxis.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH1D.h"
#include "TH2F.h"
#include "TProfile2D.h"
#include "TF1.h"
#include "TVirtualFitter.h"
#include "TObject.h"
#include "TCanvas.h"
#include "TMultiGraph.h"
#include "TLegend.h"
#include "TDatabasePDG.h"
#include "TMinuit.h"
#include "../CommonHeaders/PlottingMeson.h"
#include "TASImage.h"
#include "TMath.h"
#include "TPostScript.h"
#include "TGraphErrors.h"
#include "TArrow.h"
#include "TGraphAsymmErrors.h"
#include "TGaxis.h"
#include "TMarker.h"
#include "TFitResultPtr.h"
#include "TFitResult.h"
#include "../CommonHeaders/PlottingGammaConversionHistos.h"
#include "../CommonHeaders/PlottingGammaConversionAdditional.h"
#include "../CommonHeaders/FittingGammaConversion.h"
#include "../CommonHeaders/ConversionFunctions.h"
#include "ExtractSignalV2.C"
#include "../CommonHeaders/ExtractSignalBinning.h"
#include "../CommonHeaders/ExtractSignalPlotting.h"
#include "THnSparse.h"
#include "THStack.h"








  void FillMassHistosArrayTemp();
  void MakeFit(TH1D* hGG, TH1D* hBack, TH1D* hTruePi0, TH1D* hTrueEta, bool fdoLeft, int iPt);
  void ScaleHistos(Int_t iPt, Int_t mode);
  TH1D* SubtractBackground(TH1D* hGG, TH1D* hBack, TH1D* hTruePi0, TH1D* hTrueEta);
  void Integrate(Int_t iPt, Int_t mode);
  void InitializeTemplates(TString setPi0, Int_t, Int_t);
  void CreatePtHistosTemp();

  int debugOutputLevel = true;
  int fdoLeft = 0;
  float fScaleFacEtaToPi0 = -1;
  int optionTemplateFit = 0;
  // int optionBackgroundMethod = 0;

  TH1D**      fHistoMappingTruePi0InvMassPtBins                               = new TH1D*[fNBinsPt];
  TH1D**      fHistoMappingTruePi0ScaledInvMassPtBins                         = new TH1D*[fNBinsPt];
  TH1D**      fHistoMappingTrueEtaInvMassPtBins                               = new TH1D*[fNBinsPt];
  TH1D**      fHistoMappingTrueEtaScaledInvMassPtBins                         = new TH1D*[fNBinsPt];
  TH1D**      fHistoMappingTruePi0InvMassLeftPtBins                           = new TH1D*[fNBinsPt];
  TH1D**      fHistoMappingTruePi0ScaledInvMassLeftPtBins                     = new TH1D*[fNBinsPt];
  TH1D**      fHistoMappingTrueEtaInvMassLeftPtBins                           = new TH1D*[fNBinsPt];
  TH1D**      fHistoMappingTrueEtaScaledInvMassLeftPtBins                     = new TH1D*[fNBinsPt];
  TH1D**      fHistoMappingTrueBackgroundInvMassPtBins                        = new TH1D*[fNBinsPt];
  TH2D*       fTruePi0InvMassVSPt                                             = nullptr;
  TH2D*       fTrueEtaInvMassVSPt                                             = nullptr;
  TH2D*       fTrueBckInvMassVSPt                                             = nullptr;

  // histos for Template fitting
  TH1D*       hDataBack1dFit                                                  = nullptr;
  TH1D*       hMCSignalPi01dFit                                               = nullptr;
  TH1D*       hMCSignalEta1dFit                                               = nullptr;

  // TF1 for template fitting without RooFit
  TF1 *fTempFit = nullptr;

  // Monitoring for template fits
  TH1F*       hScaleFacBack                                                   = nullptr;


  // // Monitoring Plots
  // TH1D*       fHistoScaleFacBack                                              = nullptr;
  // TH1D*       fHistoScaleFacPi0                                               = nullptr;
  // TH1D*       fHistoScaleFacEta                                               = nullptr;
  //
  // TH1D*       fHistoScaleFacLeftBack                                              = nullptr;
  // TH1D*       fHistoScaleFacLeftPi0                                               = nullptr;
  // TH1D*       fHistoScaleFacLeftEta                                               = nullptr;


double FitSimple3(double* x, double* pars){

  const double xx = x[0]; // use x[1] to get 2nd dimension, x[2] for 3rd ...
  const double w1 = pars[0];  // weights for different histograms
  const double w2 = pars[1];
  const double w3 = pars[2];

  const double y1 = hDataBack1dFit->GetBinContent(hDataBack1dFit->GetXaxis()->FindFixBin(xx));
  const double y2 = hMCSignalPi01dFit->GetBinContent(hMCSignalPi01dFit->GetXaxis()->FindFixBin(xx));
  const double y3 = hMCSignalEta1dFit->GetBinContent(hMCSignalEta1dFit->GetXaxis()->FindFixBin(xx));

  return w1*y1 + w2*y2 + w3*y3;
}

double FitSimple2(double* x, double* pars){

  const double xx = x[0]; // use x[1] to get 2nd dimension, x[2] for 3rd ...
  const double w1 = pars[0];  // weights for different histograms
  const double w2 = pars[1];

  const double y1 = hDataBack1dFit->GetBinContent(hDataBack1dFit->GetXaxis()->FindFixBin(xx));
  const double y2 = hMCSignalPi01dFit->GetBinContent(hMCSignalPi01dFit->GetXaxis()->FindFixBin(xx));

  return w1*y1 + w2*y2;
}

double FitSimple2Pi0Fixed(double* x, double* pars){

  const double xx = x[0]; // use x[1] to get 2nd dimension, x[2] for 3rd ...
  const double w1 = pars[0];  // weights for different histograms
  const double w2 = pars[1];
  const double w3 = pars[2];

  const double y1 = hDataBack1dFit->GetBinContent(hDataBack1dFit->GetXaxis()->FindFixBin(xx));
  const double y2 = hMCSignalPi01dFit->GetBinContent(hMCSignalPi01dFit->GetXaxis()->FindFixBin(xx)) + (w3 * hMCSignalEta1dFit->GetBinContent(hMCSignalEta1dFit->GetXaxis()->FindFixBin(xx)));

  return w1*y1 + w2*y2;
}


double FitLinearPi0Fixed(double* x, double* pars){

  const double xx = x[0]; // use x[1] to get 2nd dimension, x[2] for 3rd ...
  // const double w1 = pars[0];  // weights for different histograms
  const double w1 = pars[0];
  const double w2 = pars[1];
  const double w3 = pars[2];
  const double w4 = pars[3];

  const double y1 = hDataBack1dFit->GetBinContent(hDataBack1dFit->GetXaxis()->FindFixBin(xx));
  const double y2 = hMCSignalPi01dFit->GetBinContent(hMCSignalPi01dFit->GetXaxis()->FindFixBin(xx)) + (w2 * hMCSignalEta1dFit->GetBinContent(hMCSignalEta1dFit->GetXaxis()->FindFixBin(xx)));

  return y1*(w3*xx + w4) + w1*y2;
}

double FitPlusLinearPi0Fixed(double* x, double* pars){

  const double xx = x[0]; // use x[1] to get 2nd dimension, x[2] for 3rd ...
  // const double w1 = pars[0];  // weights for different histograms
  const double w1 = pars[0];
  const double w2 = pars[1];
  const double w3 = pars[2];
  const double w4 = pars[3];
  const double w5 = pars[4];

  const double y1 = w3*hDataBack1dFit->GetBinContent(hDataBack1dFit->GetXaxis()->FindFixBin(xx));
  const double y2 = hMCSignalPi01dFit->GetBinContent(hMCSignalPi01dFit->GetXaxis()->FindFixBin(xx)) + (w2 * hMCSignalEta1dFit->GetBinContent(hMCSignalEta1dFit->GetXaxis()->FindFixBin(xx)));

  return y1+ (w4*xx + w5) + w1*y2;
}

#endif
