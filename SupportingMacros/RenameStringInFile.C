#include <iostream>
#include <fstream>
#include "TSystem.h"
#include "TROOT.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TMath.h"
#include "TStyle.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TColor.h"
#include "TTree.h"
#include "TEllipse.h"
#include "TLine.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TMultiGraph.h"
#include "TAxis.h"
#include <cmath>
#include <vector>
using std::vector;

using namespace std;

void RenameStringInFile(TString file = "", TString NameFileOut = "", TString replace = "0008e113", TString with = "0008d113"){

    TFile *File = new TFile(file.Data());
    TList* TopDir                   = (TList*) File->GetListOfKeys();

    // vector<TList*> topcontainers_out;
    // vector<TList*> containers_out;
    // vector<TObject*> vec_obj_out;

    TFile* f_out = new TFile(Form("%s",NameFileOut.Data()),"RECREATE");
    TList *toplist_out = new TList();
    toplist_out->SetName(Form("%s",NameFileOut.Data()));

    TList* TopDir2;
    for(const auto&& obj: *TopDir){
        TopDir2                   = (TList*) File->Get(obj->GetName());
        cout << "Toplist:"  <<  " Classname:  " << obj->ClassName() <<  " Name:  " << obj->GetName()  <<  " Newname:  " << NameFileOut  << endl;
    }


    Int_t nhist=0;
    for(const auto&& obj: *TopDir2){
        TClass *cl1 = gROOT->GetClass(obj->ClassName());
        TList* list_out = new TList();
        TString list_out_Name = (TString)obj->GetName();
        list_out_Name.ReplaceAll(replace, with);
        list_out->SetName(list_out_Name);
        cout << "   Key:" << nhist <<  " Classname:  " << obj->ClassName() <<  " Name:  " << obj->GetName()   << endl;
        nhist++;
        if (((TString)obj->ClassName()).CompareTo("BrokenFiles")==0) break;
        // topcontainers_out.push_back(list_out);

        // if (cl1->InheritsFrom("TKey")) {
            Int_t nhist2=0;
            for(const auto&& obj2: *((TList*)obj)){
                cout << "       Key:" << nhist2 <<  " Classname:  " << obj2->ClassName() <<  " Name:  " << obj2->GetName()  << endl ;
                nhist2++;

                TClass *cl2 = gROOT->GetClass(obj2->ClassName());
                if (cl2->InheritsFrom("TList")) {
                    TList* sub_list_out = new TList();
                    TString sublist_out_Name = (TString)obj2->GetName();
                    sublist_out_Name.ReplaceAll(replace, with);
                    sub_list_out->SetName(sublist_out_Name);

                    Int_t nhist3=0;
                    for(const auto&& obj3: *((TList*)obj2)){
                        cout << "           Key:" << nhist3 <<  " Classname:  " << obj3->ClassName() <<  " Name:  " << obj3->GetName()  << endl ;
                        nhist3++;
                        TString obj_out_Name2 = (TString)obj3->GetName();
                        obj_out_Name2.ReplaceAll(replace, with);
                        TObject* tmp2 = obj3->Clone(obj_out_Name2.Data());

                        sub_list_out->Add(tmp2);
                    }
                    list_out->Add(sub_list_out);
                } else {
                    TString obj_out_Name = (TString)obj2->GetName();
                    obj_out_Name.ReplaceAll(replace, with);
                    TObject* tmp = obj2->Clone(obj_out_Name.Data());

                    list_out->Add(tmp);
                }
            }
        // }
        toplist_out->Add(list_out);
    }
    f_out->cd();
    gFile->WriteObject(toplist_out,Form("%s",NameFileOut.Data()));
    f_out->Close();
}

////////////////////////////////////

//
//
//     Int_t nhist=0;
//     for(const auto&& obj: *TopDir_EMC){
//         nhist++;
//         TClass *cl = gROOT->GetClass(obj->ClassName());
//         // cout << "Key:" << nhist <<  " Classname:  " << obj->ClassName() <<  " Name:  " << obj->GetName()  << endl << endl;
//         if (cl->InheritsFrom("TList")) {
//             topcontainers_emc.push_back((TList*)obj);
//         }
//     }
//     // cout  << endl<< " -------------------------- " << endl << " -------------------------- " << endl << endl;
//
//     for (size_t i = 0; i < topcontainers_emc.size(); i++) {
//         for (size_t j = 0; j < topcontainers_dmc.size(); j++) {
//             if(((TString)topcontainers_emc.at(i)->GetName()).EqualTo((topcontainers_dmc.at(j)->GetName()))){
//                 cout << "topcontainers " << topcontainers_emc.at(i)->GetName()   << "  found"  << endl ;
//                 TList* list_out = new TList();
//                 list_out->SetName(topcontainers_emc.at(i)->GetName());
//                 topcontainers_out.push_back(list_out);
//                 nhist=0;
//                 containers_emc.clear();
//                 containers_dmc.clear();
//                 containers_out.clear();
//                 vec_obj_emc.clear();
//                 vec_obj_dmc.clear();
//                 vec_obj_out.clear();
//                 for(const auto&& obj: *topcontainers_emc.at(i)){
//                     nhist++;
//                     TClass *cl = gROOT->GetClass(obj->ClassName());
//                     // cout << "Key:" << nhist  << " Classname:  " << obj->ClassName()  << " Name:  " << obj->GetName()  << endl ;
//                     if (cl->InheritsFrom("TList")) {
//                         containers_emc.push_back((TList*)obj);
//                     } else vec_obj_emc.push_back((TList*)obj);
//                 }
//                 // cout  << endl<< " -------------------------- " << endl << endl;
//                 nhist=0;
//                 for(const auto&& obj: *topcontainers_dmc.at(j)){
//                     nhist++;
//                     TClass *cl = gROOT->GetClass(obj->ClassName());
//                     // cout << "Key:" << nhist  << " Classname:  " << obj->ClassName()  << " Name:  " << obj->GetName()  << endl ;
//                     if (cl->InheritsFrom("TList")) {
//                         containers_dmc.push_back((TList*)obj);
//                     } else vec_obj_dmc.push_back((TList*)obj);
//                 }
//                 nhist=0;
//                 for (size_t m = 0; m < vec_obj_emc.size(); m++) {
//                     for (size_t n = 0; n < vec_obj_dmc.size(); n++) {
//                         if(((TString)vec_obj_emc.at(m)->GetName()).EqualTo((vec_obj_dmc.at(n)->GetName()))) {
//                             TClass *cl = gROOT->GetClass(vec_obj_emc.at(m)->ClassName());
//                             TObject* tmp;
//                             if (cl->InheritsFrom("TH1")) {
//                                 if(dopTCut) pTCut = ((TString)vec_obj_emc.at(m)->GetName()).Contains("Pt");
//                                 ((TH1F*) vec_obj_emc.at(m))->SetDirectory(0);
//                                 ((TH1F*) vec_obj_dmc.at(n))->SetDirectory(0);
//                                 tmp = MergeIt((TH1F*)vec_obj_emc.at(m), (TH1F*)vec_obj_dmc.at(n), pTCut, weight_EMC, weight_DMC);
//                             } else if (cl->InheritsFrom("TH2")) {
//                                 if(((TString)vec_obj_emc.at(m)->GetName()).CompareTo("TrueDoubleCountClusterGamma_Pt")){
//                                   x_pT = kTRUE;
//                                 } else{
//                                   x_pT = kFALSE;
//                                 }
//                                 if(dopTCut) pTCut = ((TString)vec_obj_emc.at(m)->GetName()).Contains("Pt");
//                                 ((TH2F*) vec_obj_emc.at(m))->SetDirectory(0);
//                                 ((TH2F*) vec_obj_dmc.at(n))->SetDirectory(0);
//                                 tmp = MergeIt2D((TH2F*)vec_obj_emc.at(m), (TH2F*)vec_obj_dmc.at(n), pTCut, x_pT, weight_EMC, weight_DMC);
//                             } else continue;
//                             nhist++;
//                             cout << "                     Key:" << nhist  << " Classname:  " << vec_obj_emc.at(m)->ClassName()  << " Name:  " << vec_obj_emc.at(m)->GetName()  << endl ;
//                             vec_obj_out.push_back(tmp);
//                             list_out->Add(tmp);
//                         }
//                     }
//                 }
//                 // cout  << endl<< " -------------------------- " << endl << endl;
//                 for (size_t k = 0; k < containers_emc.size(); k++) {
//                     for (size_t l = 0; l < containers_dmc.size(); l++) {
//                         if(((TString)containers_emc.at(k)->GetName()).EqualTo((containers_dmc.at(l)->GetName()))) {
//                             cout << "          container  " << containers_emc.at(k)->GetName()  << "  found" << endl ;
//                             TList* sublist_out = new TList();
//                             sublist_out->SetName(containers_emc.at(k)->GetName());
//                             containers_out.push_back(sublist_out);
//                             list_out->Add(sublist_out);
//                             nhist=0;
//                             vec_obj_emc.clear();
//                             vec_obj_dmc.clear();
//                             vec_obj_out.clear();
//                             for(const auto&& obj: *containers_emc.at(k)){
//                                 nhist++;
//                                 TClass *cl = gROOT->GetClass(obj->ClassName());
//                                 // cout << "EMC: Key:" << nhist  << " Classname:  " << obj->ClassName()  << " Name:  " << obj->GetName()  << endl ;
//                                 vec_obj_emc.push_back((TList*)obj);
//                             }
//                             nhist=0;
//                             for(const auto&& obj: *containers_dmc.at(l)){
//                                 nhist++;
//                                 TClass *cl = gROOT->GetClass(obj->ClassName());
//                                 // cout << "DMC: Key:" << nhist  << " Classname:  " << obj->ClassName()  << " Name:  " << obj->GetName()  << endl ;
//                                 vec_obj_dmc.push_back((TList*)obj);
//                             }
//                             // cout  << endl<< " -------------------------- " << endl << endl;
//                             nhist=0;
//                             for (size_t m = 0; m < vec_obj_emc.size(); m++) {
//                                 for (size_t n = 0; n < vec_obj_dmc.size(); n++) {
//                                     if(((TString)vec_obj_emc.at(m)->GetName()).EqualTo((vec_obj_dmc.at(n)->GetName()))) {
//                                         TClass *cl = gROOT->GetClass(vec_obj_emc.at(m)->ClassName());
//                                         TObject* tmp;
//                                         if (cl->InheritsFrom("TH1")) {
//                                             if(dopTCut) pTCut = ((TString)vec_obj_emc.at(m)->GetName()).Contains("Pt");
//                                             ((TH1F*) vec_obj_emc.at(m))->SetDirectory(0);
//                                             ((TH1F*) vec_obj_dmc.at(n))->SetDirectory(0);
//                                             tmp = MergeIt((TH1F*)vec_obj_emc.at(m), (TH1F*)vec_obj_dmc.at(n), pTCut, weight_EMC, weight_DMC);
//                                         } else if (cl->InheritsFrom("TH2")) {
//                                             if(((TString)vec_obj_emc.at(m)->GetName()).CompareTo("TrueDoubleCountClusterGamma_Pt")){
//                                               x_pT = kTRUE;
//                                             } else{
//                                               x_pT = kFALSE;
//                                             }
//                                             if(dopTCut) pTCut = ((TString)vec_obj_emc.at(m)->GetName()).Contains("Pt");
//                                             ((TH2F*) vec_obj_emc.at(m))->SetDirectory(0);
//                                             ((TH2F*) vec_obj_dmc.at(n))->SetDirectory(0);
//                                             tmp = MergeIt2D((TH2F*)vec_obj_emc.at(m), (TH2F*)vec_obj_dmc.at(n), pTCut, x_pT, weight_EMC, weight_DMC);
//                                         } else continue;
//                                         nhist++;
//                                         cout << "                     Key:" << nhist  << " Classname:  " << vec_obj_emc.at(m)->ClassName()  << " Name:  " << vec_obj_emc.at(m)->GetName()  << endl ;
//                                         vec_obj_out.push_back(tmp);
//                                         sublist_out->Add(tmp);
//                                     }
//                                 }
//                             }
//                         }
//                     }
//                 }
//                 toplist_out->Add(list_out);
//             }
//         }
//     }
//
//     f_out->cd();
//     gFile->WriteObject(toplist_out,Form("%s",NameTopList.Data()));
//     f_out->Close();
//
//
// }
