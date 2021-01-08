/****************************************************************************************************************************
 ******     provided by Gamma Conversion Group, PWGGA,                                                       *****
 ******     Friederike Bock, friederike.bock@cern.ch                                                    *****
 *****************************************************************************************************************************/

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

void MergeSignalExtraction(  TString fCutSelection,
                         TString mesonType,
                         TString fSuffix,
                         TString fEnergyFlag,
                         TString nameFileFileFull,
                         TString nameFileMBMC,
                         TString nameFileJetJetMC,
                         Double_t minPt = 6.,
                         Double_t maxPt = 30.
                      ){

    gSystem->Exec("cp "+nameFileFileFull+" "+nameFileMBMC);
    TString outputDir                       = Form("%s/%s/%s/ExtractGammaSignal", fCutSelection.Data(), fEnergyFlag.Data(), fSuffix.Data());
    gSystem->Exec("mkdir "+outputDir);

    TFile* fileMBMC                             = new TFile (nameFileMBMC);
    TFile* fileJetJetMC                             = new TFile (nameFileJetJetMC);

    TFile *fOut = new TFile (nameFileFileFull, "UPDATE");

    TList* list = fileMBMC->GetListOfKeys() ;
    if (!list) { printf("<E> No keys found in file\n") ; exit(1) ; }
    TIter next(list) ;
    TKey* key ;
    TObject* obj ;

    while ( key = (TKey*)next() ) {
      obj = key->ReadObj();
      // Dont merge Objects that are not of type TH1
      if ( (!obj->InheritsFrom("TH1"))
         ) {
        TString objName = obj->GetName();
        TH1F* histoJJ = (TH1F*) fileJetJetMC->Get(objName);
        if(!histoJJ) continue;
        cout<<"Writing: "<<objName<<endl;
        histoJJ->Write(objName, TObject::kOverwrite);
        continue;
      }
      // Dont merge the inv. mass peaks. Just take the JJ ones
      if(((TString)obj->GetName()).Contains("fBckNorm") || ((TString)obj->GetName()).Contains("_Bin") || ((TString)obj->GetName()).Contains("InvMass") || obj->InheritsFrom("TH2")) {
        TString objName = obj->GetName();
        TH1F* histoJJ = (TH1F*) fileJetJetMC->Get(objName);
        if(!histoJJ) continue;
        cout<<"Writing: "<<objName<<endl;
        histoJJ->Write(objName, TObject::kOverwrite);
        continue;
      }
      TString histName = obj->GetName();
      // printf("Histo name:%s title:%s\n",obj->GetName(),obj->GetTitle());
      TString objName = obj->GetName();
      TH1F* histoMinBias = (TH1F*) fileMBMC->Get(objName);
      TH1F* histoJetJetMC = (TH1F*) fileJetJetMC->Get(objName);
      TH1F* histoMerged = (TH1F*) histoMinBias->Clone(Form("%s",((TString)objName).Data()));


      for(int i = 1; i <= histoMinBias->GetNbinsX(); ++i){
        if(!histoJetJetMC) continue;
        if(histoMinBias->GetBinCenter(i) < minPt) continue;
        else if(histoMinBias->GetBinCenter(i) > maxPt){ // Set a maximum pT to reject "outliers" in the min. bias MC
          histoMerged->SetBinContent(i, histoJetJetMC->GetBinContent(i));
          histoMerged->SetBinError(i, histoJetJetMC->GetBinError(i));
        } else { // merge the two histograms
          Double_t relErrMinBias      = histoMinBias->GetBinError(i)/histoMinBias->GetBinContent(i)*100;
          Double_t relErrJetJetMC     = histoJetJetMC->GetBinError(i)/histoJetJetMC->GetBinContent(i)*100;

          Double_t weightMinBias      = 1./TMath::Power(histoMinBias->GetBinError(i),2);
          Double_t weightJetJetMC     = 1./TMath::Power(histoJetJetMC->GetBinError(i),2);
          Double_t weightSum          = weightMinBias + weightJetJetMC;
          Double_t weightedEffi       = (weightMinBias*histoMinBias->GetBinContent(i) + weightJetJetMC * histoJetJetMC->GetBinContent(i))/weightSum;
          Double_t weightedEffiErr    = TMath::Power((weightMinBias +  weightJetJetMC),-0.5);

          if (isfinite(weightedEffi) && isfinite(weightedEffiErr)){
              histoMerged->SetBinContent(i, weightedEffi);
              histoMerged->SetBinError(i, weightedEffiErr);
          }
        }
      }

      fOut->cd();
      TString OutName = histName;
      if(histName.Contains("_merge")) OutName = histName.Remove(histName.Sizeof() - 7, 7);
      cout<<"Writing: "<<OutName<<endl;
      histoMerged->Write(OutName, TObject::kOverwrite);

    }

    fOut->Close();
}
