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
// #include "../CommonHeaders/ConversionFunctionsBasicsAndLabeling.h"
#include "../CommonHeaders/ConversionFunctions.h"

void ChangeCutnumberInFile( TString fileNameConfig   = "",
                            TString outputFileName   = "",
                            Int_t mode               = 0
                          ){

    
    // Read cuts from CutSelection file
    ifstream in(fileNameConfig.Data());
    cout<<"Available Cuts:"<<endl;
    TString fileName1[40];
    TString cutNumber1[40];
    TString cutNumberOut[40];
    Int_t Number = 0;
    while(!in.eof() ){
        in >> fileName1[Number] >> cutNumber1[Number]  >> cutNumberOut[Number];
        cout<< fileName1[Number] << "\t" << cutNumber1[Number]<<  "\t" << cutNumberOut[Number] <<endl;
        Number++;
    }
    cout<<"=========================="<<endl;
    cout << " " << endl;

    // read trees
    TFile* filesIn1[40] = {NULL};
    TTree *treeCopyPhotonInter1[40]   = {NULL};        
    TTree *treeCopyMesonInter1[40]   = {NULL};        
    for (Int_t i = 0; i < Number-1; i++){
        filesIn1[i] = new TFile(fileName1[i].Data());
        treeCopyPhotonInter1[i] = (TTree*)filesIn1[i]->Get(Form("%s Photon DCA tree",cutNumber1[i].Data() ));
        if (treeCopyPhotonInter1[i]){
            cout << "found tree" << endl;
            treeCopyPhotonInter1[i]->SetName(Form("%s Photon DCA tree",cutNumberOut[i].Data() ));
        }
        treeCopyMesonInter1[i] = (TTree*)filesIn1[i]->Get(Form("%s Meson DCA tree",cutNumber1[i].Data() ));
        if (treeCopyMesonInter1[i]){
            cout << "found tree" << endl;
            treeCopyMesonInter1[i]->SetName(Form("%s Meson DCA tree",cutNumberOut[i].Data() ));
        }
    }
    
    TString listNameOutMain = GetDefaultMainTListName(mode);
    TFile *fileInter1        = new TFile("buffer1.root","RECREATE");
    TList *listInter1        = new TList();
    listInter1->SetName(listNameOutMain.Data());
    TList *listToCopyInter1[40]   = {NULL};        
    
    for (Int_t i = 0; i < Number-1; i++){
        cout << "reading config " << i << endl;
        TString autoDetectedMainDir1     = AutoDetectMainTList(mode , filesIn1[i]);
        if (autoDetectedMainDir1.CompareTo("") == 0){
            cout << "ERROR: trying to read file, which is incompatible with mode selected" << endl;;
            return;
        }
        
        TList *list1 = (TList*) filesIn1[i]->Get(autoDetectedMainDir1.Data());
        Int_t refNr     = -1;
        for(Int_t j = 0; j<list1->GetEntries(); j++){
            TList *l2 = (TList*) list1->At(j);
            TString dirname = l2->GetName();
            if(dirname.Contains(cutNumber1[i].Data()) ){
                cout<<"found " << cutNumber1[i].Data() << " in " << fileName1[i] <<endl;
                dirname.ReplaceAll(cutNumber1[i].Data(), cutNumberOut[i].Data());
                listToCopyInter1[i] = new TList();
                listToCopyInter1[i]->SetName(dirname.Data());
                refNr         = j;
            } 
                
        }
        if (listToCopyInter1[i]){
            for (Int_t k = 0; k < ((TList*) list1->At(refNr))->GetEntries();k++){
                TList *l3 = (TList*)((TList*) list1->At(refNr))->At(k);
                TString listName = l3->GetName();
                if (listName.BeginsWith(cutNumber1[i].Data())){
                    listName.ReplaceAll(cutNumber1[i].Data(), cutNumberOut[i].Data());
                    l3->SetName(listName.Data());
                    listToCopyInter1[i]->Add(l3);
                }
            }
        } else {
            cout<<"couldn't find " << cutNumber1[i].Data() << " in " << fileName1[i] <<endl;
            return;
        }
        listInter1->Add(listToCopyInter1[i]);
    }
    fileInter1->cd();
    listInter1->Write("",TObject::kSingleKey);
    for (Int_t i = 0;  i < Number-1; i++){
        if (treeCopyPhotonInter1[i]) treeCopyPhotonInter1[i]->CloneTree(-1,"fast");
        if (treeCopyMesonInter1[i]) treeCopyMesonInter1[i]->CloneTree(-1,"fast");
    }
    fileInter1->Write();
    fileInter1->Close();

    for (Int_t i = 0; i < Number-1; i++){
        if (filesIn1[i]) delete filesIn1[i];
    }

    gSystem->Exec("mv buffer1.root "+outputFileName);
}
