/*******************************************************************************
 ******  provided by Gamma Conversion Group, PWGGA,                        *****
 ******     Daniel Muehlheim, d.muehlheim@cern.ch                          *****
 ******     Nicolas Schmidt, nicolas.schmidt@cern.ch                       *****
 ******     Friederike Bock, fbock@cern.ch                                 *****
 *******************************************************************************/

#include "QA.h"
#include "../TaskQA/BuildHistogramsForGammaQAAdvV3.C"

void PhotonQA_Runwise_V2(
                        Int_t nSetsIn,
                        Int_t nDataIn,
                        TString fEnergyFlag,
                        TString filePath,
                        TString fileName,
                        TString* DataSets,
                        TString* plotDataSets,
                        Int_t mode                      = 2,
                        Int_t cutNr                     = 0,               // if -1: you have to choose number at runtime, if 0 the first or only number is chosen
                        Int_t doExtQA                   = 2,                // 0: switched off, 1: normal extQA, 2: with Cell level plots
                        Bool_t doEquidistantXaxis       = kFALSE,
                        Bool_t doTrigger                = kTRUE,
                        Bool_t doHistsForEverySet       = kTRUE,
                        Bool_t addSubFolder             = kFALSE,
                        Bool_t useDataRunListForMC      = kFALSE,
                        Size_t markerSize               = 1,
                        TString suffix                  = "eps",
                        TString folderRunlists          = "",
                        TString addLabelRunList         = "",
                        TString cutTreeProjection       = "",
                        Bool_t useConsistentCut         = kFALSE
                    ){
    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << "PhotonQA_Runwise" << endl;
    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;

    gROOT->Reset();
    TH1::AddDirectory(kFALSE);

    const Int_t nSets = nSetsIn;
    const Int_t nData = nDataIn;
    TString nameOutput;
    Double_t leftMar = 0.09; Double_t rightMar = 0.025; Double_t topMargin = 0.04; Double_t bottomMargin = 0.09;

    StyleSettingsThesis();
    SetPlotStyle();
//**************************************************************************************************************
    TString fDate = ReturnDateString();
    TString fTextMeasurement = Form("#pi^{0} #rightarrow #gamma#gamma");

    TString DataSetName;
    Style_t hMarkerStyle;
    Size_t hMarkerSize;
    hMarkerSize=markerSize;
    Color_t hMarkerColor;
    Color_t hLineColor;
    TString fileRuns;
    TString fTrigger = "";
    Float_t xPosLabel = 0.8;
    std::vector<TString>* vecMissingRuns = new std::vector<TString>[nSets];

    Int_t fMode = mode;
    // mode:    0 // new output PCM-PCM
    //          1 // new output PCM dalitz
    //          2 // new output PCM-EMCal
    //          3 // new output PCM-PHOS
    //          4 // new output EMCal-EMCal
    //          5 // new output PHOS-PHOS
    //          9 // old output PCM-PCM
    //          10 // merged EMCal
    //          11 // merged PHOS

    if(fMode == 4 || fMode == 5 || fMode == 10 || fMode == 11){ cout << "Returning, given mode contains no PCM information: " << fMode << endl; return;}

    std::vector<TString> vecRuns;

    DataSetName=DataSets[0].Data();
    fileRuns             = Form("%s/runNumbers%s%s.txt", folderRunlists.Data(), DataSetName.Data(),addLabelRunList.Data());
        // if(useDataRunListForMC && i>=nData) {
        //     fileRuns         = Form("%s/runNumbers%s%s-%s.txt", folderRunlists.Data(), DataSetName.Data(), addLabelRunList.Data(),DataSets[0].Data());
        //     cout << "Switch useDataRunListForMC is true, reading runs from: " << fileRuns.Data() << endl;
        // }
    cout << "trying to read: " << fileRuns.Data() << endl;
    if(!readin(fileRuns, vecRuns, kFALSE)) {cout << "ERROR, no Run Numbers could be found! Returning..." << endl; return;}

    //******************************************************************************

    UInt_t ActualRunIndexInVector=0;
    TFile* fPhotonQAFile=NULL;
    while ( (fPhotonQAFile==NULL)&&(ActualRunIndexInVector<vecRuns.size()) ){
        cout << "creating: " << Form("%s/%s/%s/%s", filePath.Data(), ((TString)DataSetName).Data(), ((TString)vecRuns.at(ActualRunIndexInVector)).Data(),fileName.Data()) << endl;
        fPhotonQAFile = new TFile(Form("%s/%s/%s/%s", filePath.Data(), ((TString)DataSetName).Data(), ((TString)vecRuns.at(ActualRunIndexInVector)).Data(),fileName.Data()));
        if(fPhotonQAFile->IsZombie()) {
            cout << "ERROR: ROOT file '" << Form("%s/%s/%s/%s", filePath.Data(), ((TString)DataSetName).Data(), ((TString)vecRuns.at(ActualRunIndexInVector)).Data(), fileName.Data()) << "' could not be openend, trying next output file!" << endl;
            fPhotonQAFile->Close();
            delete fPhotonQAFile;
            fPhotonQAFile=NULL;
        }
        ActualRunIndexInVector++;
    }
    if (fPhotonQAFile==NULL){
        // check wether there are subfiles
        TString listForTesting  = Form("%s/%s/%s/Stage*/*/%s", filePath.Data(), ((TString)DataSetName).Data(), ((TString)DataSetName).Data(), fileName.Data());
        TString listName        = Form("fileList%s.txt", ((TString)vecRuns.at(0)).Data());
        gSystem->Exec("ls "+listForTesting+ " > " + listName);

        ifstream fileListName;
        fileListName.open(listName,ios_base::in);
        if (!fileListName) {
            cout << "ERROR: settings " << listName.Data() << " not found!" << endl;
            return;
        }

        Bool_t hasStagedFile    = kFALSE ;
        for( TString tempLine; tempLine.ReadLine(fileListName, kTRUE) && !hasStagedFile ; ) {
            cout << tempLine.Data() << endl;
            fPhotonQAFile = new TFile(tempLine.Data(),"READ");
            if(!fPhotonQAFile->IsZombie()) {
                hasStagedFile = kTRUE;
                cout << "found corresponding file in Stages output" << endl;
            }
        }
        if (!hasStagedFile){
            cout << "could not find any merges of Stages either, return!" << endl;
            return;
        }
    }



    TString nameCutsPQA;        // full cut number
    TString nameCutsPQAshort;   // event cut number
    vector <TString> cutsPQA;   // in case there are several different event cut selections (eg centralities) saved in one AnalysisResults.root file
    vector <TString> cutsPQAshort;
    TString nameMainDir;
    TKey *keyPQA;
    TIter nextPQA(fPhotonQAFile->GetListOfKeys());
    while ((keyPQA=(TKey*)nextPQA())){
        cout << Form("Found TopDir: '%s' ",keyPQA->GetName()) << endl;
        nameMainDir = keyPQA->GetName();
        if(nameMainDir.Contains("GammaConvV1_QA_")){
            nameCutsPQA = Form("%s",nameMainDir.Data());
            nameCutsPQAshort = Form("%s",nameMainDir.Data());
            nameCutsPQA.Replace(0,15,"");
            nameCutsPQAshort.Replace(0,15,"");
            nameCutsPQAshort.Replace(8,35,"");
            cutsPQA.push_back(nameCutsPQA);
            cutsPQAshort.push_back(nameCutsPQAshort);
            // break;
        }
    }
    cout << "The following cuts are available:" << endl;
    for(Int_t icuts = 0; icuts < (Int_t) cutsPQA.size(); icuts++) {
      cout << Form("(%i) -- %s", icuts, cutsPQA[icuts].Data()) << endl;
    }
    if(cutNr == -1){
        do{ cin >> cutNr;}
        while( (cutNr < 0) || (cutNr > (Int_t) cutsPQA.size()) );
    }
    cout << "Processing Cut Number: " << cutNr << endl;
    nameCutsPQA      = cutsPQA.at(cutNr);
    nameCutsPQAshort = cutsPQAshort.at(cutNr);
    nameMainDir = Form("GammaConvV1_QA_%s",cutsPQA.at(cutNr).Data());

    TString fCentralityFromCut = GetCentralityString(nameCutsPQAshort);
    cout << "Corresponding to centrality: " << fCentralityFromCut << endl;
    if (!fCentralityFromCut.CompareTo("pp") || !fCentralityFromCut.CompareTo("0-100%"))
      fCentralityFromCut = "";

    Bool_t cutTreeStdCut = kFALSE;
    if (cutTreeProjection.CompareTo("") == 0 ){
        cutTreeProjection   = "0004314141";
        cutTreeStdCut       = kTRUE;
    }
    // choosing cut to project from tree
    if(fEnergyFlag.CompareTo("8TeV")==0){
        cutTreeProjection   = "0005314140";
        cutTreeStdCut       = kTRUE;
    } else if(fEnergyFlag.CompareTo("PbPb_2.76TeV")==0){
        cutTreeProjection   = "0005310040";
        cutTreeStdCut       = kTRUE;
    }
    // if(fEnergyFlag.CompareTo("pPb_5.023TeV")==0){cutTreeProjection = "0000004140"; cutTreeStdCut = kFALSE;} //without chi2 and psipair

    cout << endl;
    cout << "long cutnumber for PhotonQA: '" << nameCutsPQA << "'\nevent cutnumber for PhotonQA: '"<< nameCutsPQAshort << "'" << endl;
    cout << "****************************************************************************" << endl;
    cout << "****************************************************************************" << endl;
    cout << "****************************************************************************\n" << endl;
    if (cutTreeStdCut) cout << "WARNING: PhotonQA hist will be build with a preset cut, make sure its what you wanted!!" << endl;
    cout << "using cutnumber: '" << cutTreeProjection << "' to project out of tree, double check with BuildHistogramsForGammaQAAdvV3.C!!!" << endl;
    cout << "\n****************************************************************************" << endl;
    cout << "****************************************************************************" << endl;
    cout << "****************************************************************************" << endl;

    fPhotonQAFile->Close();
    delete fPhotonQAFile;
    if(nameMainDir.IsNull() || !nameMainDir.BeginsWith("Gamma")){cout << "ERROR, Unable to obtain valid name of MainDir:|" << nameMainDir.Data() << "|, running in mode: " << fMode << endl; return;}

    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << "Processing Cut Number: " << nameCutsPQA << endl;
    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << "Pictures are saved as " << suffix.Data() << "!" << endl;
    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;

    TString fCollisionSystem = ReturnFullCollisionsSystem(fEnergyFlag);
    if (fCollisionSystem.CompareTo("") == 0){
        cout << "No correct collision system specification, has been given" << endl;
        return;
    }
    if (fCentralityFromCut.CompareTo("pp")!=0 && !fCentralityFromCut.Contains("0-100%") ){
      fCollisionSystem    = Form("%s %s", fCentralityFromCut.Data(), fCollisionSystem.Data());
    }

    TString fDetectionProcess = ReturnFullTextReconstructionProcess(fMode);

    TString outputDir = Form("%s_%s/%s/PhotonQA/%s/Runwise",nameCutsPQA.Data(),cutTreeProjection.Data(), fEnergyFlag.Data(),suffix.Data());
    if(addSubFolder){
        outputDir += "/";
        outputDir += DataSets[0];
    }
    gSystem->Exec("mkdir -p "+outputDir);

//****************************** Histograms ************************************************

    std::vector<TString> globalRuns;

    for(Int_t i=0; i<nSets; i++) {
        vecRuns.clear();
        fileRuns             = Form("%s/runNumbers%s%s.txt", folderRunlists.Data(), DataSets[i].Data(),addLabelRunList.Data());
        cout << "trying to read: " << fileRuns.Data() << endl;
        if(!readin(fileRuns, vecRuns, kFALSE)) {cout << "ERROR, no Run Numbers could be found! Returning..." << endl; return;}

        for(Int_t j=0; j<(Int_t) vecRuns.size();j++)
        {
            if( i==0 ) globalRuns.push_back(vecRuns.at(j));
            else
            {
                Bool_t bFound = kFALSE;
                for(Int_t k=0; k<(Int_t) globalRuns.size();k++){ if(globalRuns.at(k)==vecRuns.at(j)) bFound=kTRUE;}
                if(!bFound) globalRuns.push_back(vecRuns.at(j));
            }
        }
    }

    selection_sort(globalRuns.begin(), globalRuns.end());

    map<TString,Int_t> mapBin;

    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << "Processing following list of " << globalRuns.size() << " Runs:";
    for(Int_t iglobalRuns=0; iglobalRuns<(Int_t) globalRuns.size(); iglobalRuns++) {
        mapBin[globalRuns.at(iglobalRuns)]=iglobalRuns+1;
        if(iglobalRuns%10==0) cout << endl;
        cout << globalRuns.at(iglobalRuns) << ", ";
    }
    cout << endl;
    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;

    Int_t hFBin;
    Int_t hLBin;
    Int_t hNBin;

    if(doEquidistantXaxis) {
        hFBin = 0;
        hLBin = globalRuns.size();
        hNBin = globalRuns.size();
    } else {
        if(nSets>2){
            hFBin = ((TString) globalRuns.at(0)).Atoi() - 500;
            hLBin = ((TString) globalRuns.at(globalRuns.size()-1)).Atoi()  + 500;
            hNBin = hLBin - hFBin;
        }else{
            hFBin = ((TString) globalRuns.at(0)).Atoi() - 25;
            hLBin = ((TString) globalRuns.at(globalRuns.size()-1)).Atoi()  + 25;
            hNBin = hLBin - hFBin;
        }
    }

    TString lepton[2] = {"Electron","Positron"};
    TString charge[2] = {"-","+"};


    std::vector<TString> vecHistosName;

    //**************************************************************************************************************
    //**************************************************************************************************************
    //**************************************************************************************************************

    for(Int_t i=0; i<nSets; i++){

        TH1D* hGammaN;
        TH1D* hGammaPt;
        TH1D* hGammaAlpha;
        TH1D* hGammaEta;
        TH1D* hGammaEtaNeg;
        TH1D* hGammaEtaPos;
        TH1D* hGammaPhi;
        TH1D* hGammaPhiLow;
        TH1D* hGammaPhiHigh;
        TH1D* hGammaPhiEtaNeg;
        TH1D* hGammaPhiEtaPos;
        TH1D* hGammaChi2;
        TH1D* hGammaPsiPair;
        TH1D* hGammaCosPoint;
        TH1D* hGammaInvMass;
        TH1D* hGammaQt;
        TH1D* hGammaAsym;
        TH1D* hGammaR;
        TH1D* hNSdEdx[2];
        TH1D* hEta[2];
        TH1D* hPt[2];
        TH1D* hTPCclusters[2];
        TH1D* hfTPCclusters[2];

        TH1D* hGammaPtRMS;
        TH1D* hGammaAlphaRMS;
        TH1D* hGammaEtaRMS;
        TH1D* hGammaPhiRMS;
        TH1D* hGammaChi2RMS;
        TH1D* hGammaPsiPairRMS;
        TH1D* hGammaCosPointRMS;
        TH1D* hGammaInvMassRMS;
        TH1D* hGammaQtRMS;
        TH1D* hGammaAsymRMS;
        TH1D* hGammaRRMS;
        TH1D* hNSdEdxRMS[2];
        TH1D* hEtaRMS[2];
        TH1D* hPtRMS[2];
        TH1D* hTPCclustersRMS[2];
        TH1D* hfTPCclustersRMS[2];

        std::vector<TH1D*> vecGammaPt;
        std::vector<TH1D*> vecGammaAlpha;
        std::vector<TH1D*> vecGammaEta;
        std::vector<TH1D*> vecGammaPhi;
        std::vector<TH1D*> vecGammaChi2;
        std::vector<TH1D*> vecGammaPsiPair;
        std::vector<TH1D*> vecGammaCosPoint;
        std::vector<TH1D*> vecGammaInvMass;

        std::vector<TH2D*> vecInvMassR;
        std::vector<TH2D*> vecEtaR;
        std::vector<TH2D*> vecPhiR;
        std::vector<TH2D*> vecAlphaR;
        std::vector<TH2D*> vecPsiPairR;
        std::vector<TH2D*> vecAsymR;

        std::vector<TH2D*> vecNSdEdx[2];
        std::vector<TH2D*> vecTPCClusR[2];

        std::vector<TH1D*> vecHistos;

        TString histoName;
        DataSetName=DataSets[i].Data();
        vecRuns.clear();
        fileRuns             = Form("%s/runNumbers%s%s.txt", folderRunlists.Data(), DataSetName.Data(),addLabelRunList.Data());
            // if(useDataRunListForMC && i>=nData) {
            //     fileRuns         = Form("%s/runNumbers%s%s-%s.txt", folderRunlists.Data(), DataSetName.Data(), addLabelRunList.Data(),DataSets[0].Data());
            //     cout << "Switch useDataRunListForMC is true, reading runs from: " << fileRuns.Data() << endl;
            // }
        cout << "trying to read: " << fileRuns.Data() << endl;
        if(!readin(fileRuns, vecRuns, kFALSE)) {cout << "ERROR, no Run Numbers could be found! Returning..." << endl; return;}

        //******************************************************************************

        UInt_t ActualRunIndexInVector=0;
        TFile* fPhotonQAFile=NULL;
        while ( (fPhotonQAFile==NULL)&&(ActualRunIndexInVector<vecRuns.size()) ){
            cout << "creating: " << Form("%s/%s/%s/%s", filePath.Data(), ((TString)DataSetName).Data(), ((TString)vecRuns.at(ActualRunIndexInVector)).Data(),fileName.Data()) << endl;
            fPhotonQAFile = new TFile(Form("%s/%s/%s/%s", filePath.Data(), ((TString)DataSetName).Data(), ((TString)vecRuns.at(ActualRunIndexInVector)).Data(),fileName.Data()));
            if(fPhotonQAFile->IsZombie()) {
                cout << "ERROR: ROOT file '" << Form("%s/%s/%s/%s", filePath.Data(), ((TString)DataSetName).Data(), ((TString)vecRuns.at(ActualRunIndexInVector)).Data(), fileName.Data()) << "' could not be openend, trying next output file!" << endl;
                fPhotonQAFile->Close();
                delete fPhotonQAFile;
                fPhotonQAFile=NULL;
            }
            ActualRunIndexInVector++;
        }
        if (fPhotonQAFile==NULL){
            // check wether there are subfiles
            TString listForTesting  = Form("%s/%s/%s/Stage*/*/%s", filePath.Data(), ((TString)DataSetName).Data(), ((TString)DataSetName).Data(), fileName.Data());
            TString listName        = Form("fileList%s.txt", ((TString)vecRuns.at(0)).Data());
            gSystem->Exec("ls "+listForTesting+ " > " + listName);

            ifstream fileListName;
            fileListName.open(listName,ios_base::in);
            if (!fileListName) {
                cout << "ERROR: settings " << listName.Data() << " not found!" << endl;
                return;
            }

            Bool_t hasStagedFile    = kFALSE ;
            for( TString tempLine; tempLine.ReadLine(fileListName, kTRUE) && !hasStagedFile ; ) {
                cout << tempLine.Data() << endl;
                fPhotonQAFile = new TFile(tempLine.Data(),"READ");
                if(!fPhotonQAFile->IsZombie()) {
                    hasStagedFile = kTRUE;
                    cout << "found corresponding file in Stages output" << endl;
                }
            }
            if (!hasStagedFile){
                cout << "could not find any merges of Stages either, return!" << endl;
                return;
            }
        }



        // plotting settings according to default for energy, dataset and centrality
        if (i < nDataIn){
            hMarkerStyle         = GetDefaultMarkerStyle(fEnergyFlag.Data(),DataSets[i].Data(),fCentralityFromCut);
            hMarkerColor         = GetColorDefaultColor(fEnergyFlag.Data(),DataSets[i].Data(),fCentralityFromCut);
            hLineColor           = GetColorDefaultColor(fEnergyFlag.Data(),DataSets[i].Data(),fCentralityFromCut);
            cout  << i << "\t"<< hMarkerStyle << "\t"<< hMarkerColor << "\t"<< hLineColor << endl;
        } else {
            hMarkerStyle         = GetDefaultMarkerStyle(fEnergyFlag.Data(),plotDataSets[i].Data(),fCentralityFromCut);
            hMarkerColor         = GetColorDefaultColor(fEnergyFlag.Data(),plotDataSets[i].Data(),fCentralityFromCut);
            hLineColor           = GetColorDefaultColor(fEnergyFlag.Data(),plotDataSets[i].Data(),fCentralityFromCut);
            cout  << i << "\t"<< hMarkerStyle << "\t"<< hMarkerColor << "\t"<< hLineColor << endl;
        }


    // for(Int_t i=0; i<nSets; i++) {
        histoName = "nGamma";
        if(i==0) vecHistosName.push_back(histoName);
        hGammaN = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),"nGamma; Run Number ; Number of Photons",hNBin,hFBin,hLBin);
        EditTH1(globalRuns, doEquidistantXaxis, hGammaN, hMarkerStyle, hMarkerSize, hMarkerColor, hLineColor);
        vecHistos.push_back(hGammaN);

        histoName = "hGammaPt";
        if(i==0) vecHistosName.push_back(histoName);
        hGammaPt = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),"hGammaPt; Run Number ; Mean Photon #it{p}_{T}",hNBin,hFBin,hLBin);
        EditTH1(globalRuns, doEquidistantXaxis, hGammaPt, hMarkerStyle, hMarkerSize, hMarkerColor, hLineColor);
        vecHistos.push_back(hGammaPt);

        histoName = "hGammaPtRMS";
        if(i==0) vecHistosName.push_back(histoName);
        hGammaPtRMS = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),"hGammaPtRMS; Run Number ; #sigma_{Photon #it{p}_{T}}",hNBin,hFBin,hLBin);
        EditTH1(globalRuns, doEquidistantXaxis, hGammaPtRMS, hMarkerStyle, hMarkerSize, hMarkerColor, hLineColor);
        vecHistos.push_back(hGammaPtRMS);

        histoName = "hGammaAlpha";
        if(i==0) vecHistosName.push_back(histoName);
        hGammaAlpha = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),"hGammaAlpha; Run Number ; Mean Photon #alpha",hNBin,hFBin,hLBin);
        EditTH1(globalRuns, doEquidistantXaxis, hGammaAlpha, hMarkerStyle, hMarkerSize, hMarkerColor, hLineColor);
        vecHistos.push_back(hGammaAlpha);

        histoName = "hGammaAlphaRMS";
        if(i==0) vecHistosName.push_back(histoName);
        hGammaAlphaRMS = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),"hGammaAlphaRMS; Run Number ; #sigma_{Photon #alpha}",hNBin,hFBin,hLBin);
        EditTH1(globalRuns, doEquidistantXaxis, hGammaAlphaRMS, hMarkerStyle, hMarkerSize, hMarkerColor, hLineColor);
        vecHistos.push_back(hGammaAlphaRMS);

        histoName = "hGammaEta";
        if(i==0) vecHistosName.push_back(histoName);
        hGammaEta = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),"hGammaEta; Run Number ; Mean Photon #eta",hNBin,hFBin,hLBin);
        EditTH1(globalRuns, doEquidistantXaxis, hGammaEta, hMarkerStyle, hMarkerSize, hMarkerColor, hLineColor);
        vecHistos.push_back(hGammaEta);

        histoName = "hGammaEtaRMS";
        if(i==0) vecHistosName.push_back(histoName);
        hGammaEtaRMS = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),"hGammaEtaRMS; Run Number ; #sigma_{Photon #eta}",hNBin,hFBin,hLBin);
        EditTH1(globalRuns, doEquidistantXaxis, hGammaEtaRMS, hMarkerStyle, hMarkerSize, hMarkerColor, hLineColor);
        vecHistos.push_back(hGammaEtaRMS);

        histoName = "hGammaEtaNeg";
        if(i==0) vecHistosName.push_back(histoName);
        hGammaEtaNeg = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),"hGammaEtaNeg; Run Number ; #gamma_{#eta < 0}/#gamma_{total}",hNBin,hFBin,hLBin);
        EditTH1(globalRuns, doEquidistantXaxis, hGammaEtaNeg, hMarkerStyle, hMarkerSize, hMarkerColor, hLineColor);
        vecHistos.push_back(hGammaEtaNeg);

        histoName = "hGammaEtaPos";
        if(i==0) vecHistosName.push_back(histoName);
        hGammaEtaPos = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),"hGammaEtaPos; Run Number ; #gamma_{#eta > 0}/#gamma_{total}",hNBin,hFBin,hLBin);
        EditTH1(globalRuns, doEquidistantXaxis, hGammaEtaPos, hMarkerStyle, hMarkerSize, hMarkerColor, hLineColor);
        vecHistos.push_back(hGammaEtaPos);

        histoName = "hGammaPhi";
        if(i==0) vecHistosName.push_back(histoName);
        hGammaPhi = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),"hGammaPhi; Run Number ; Mean Photon #phi",hNBin,hFBin,hLBin);
        EditTH1(globalRuns, doEquidistantXaxis, hGammaPhi, hMarkerStyle, hMarkerSize, hMarkerColor, hLineColor);
        vecHistos.push_back(hGammaPhi);

        histoName = "hGammaPhiRMS";
        if(i==0) vecHistosName.push_back(histoName);
        hGammaPhiRMS = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),"hGammaPhiRMS; Run Number ; #sigma_{Photon #phi}",hNBin,hFBin,hLBin);
        EditTH1(globalRuns, doEquidistantXaxis, hGammaPhiRMS, hMarkerStyle, hMarkerSize, hMarkerColor, hLineColor);
        vecHistos.push_back(hGammaPhiRMS);

        histoName = "hGammaPhiLow";
        if(i==0) vecHistosName.push_back(histoName);
        hGammaPhiLow = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),"hGammaPhiLow; Run Number ; #gamma_{#phi < #pi}/#gamma_{total}",hNBin,hFBin,hLBin);
        EditTH1(globalRuns, doEquidistantXaxis, hGammaPhiLow, hMarkerStyle, hMarkerSize, hMarkerColor, hLineColor);
        vecHistos.push_back(hGammaPhiLow);

        histoName = "hGammaPhiHigh";
        if(i==0) vecHistosName.push_back(histoName);
        hGammaPhiHigh = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),"hGammaPhiHigh; Run Number ; #gamma_{#phi > #pi}/#gamma_{total}",hNBin,hFBin,hLBin);
        EditTH1(globalRuns, doEquidistantXaxis, hGammaPhiHigh, hMarkerStyle, hMarkerSize, hMarkerColor, hLineColor);
        vecHistos.push_back(hGammaPhiHigh);

        histoName = "hGammaPhiEtaNeg";
        if(i==0) vecHistosName.push_back(histoName);
        hGammaPhiEtaNeg = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),"hGammaPhiEtaNeg; Run Number ; Mean Photon #phi C side",hNBin,hFBin,hLBin);
        EditTH1(globalRuns, doEquidistantXaxis, hGammaPhiEtaNeg, hMarkerStyle, hMarkerSize, hMarkerColor, hLineColor);
        vecHistos.push_back(hGammaPhiEtaNeg);

        histoName = "hGammaPhiEtaPos";
        if(i==0) vecHistosName.push_back(histoName);
        hGammaPhiEtaPos = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),"hGammaPhiEtaPos; Run Number ; Mean Photon #phi A side",hNBin,hFBin,hLBin);
        EditTH1(globalRuns, doEquidistantXaxis, hGammaPhiEtaPos, hMarkerStyle, hMarkerSize, hMarkerColor, hLineColor);
        vecHistos.push_back(hGammaPhiEtaPos);

        histoName = "hGammaChi2";
        if(i==0) vecHistosName.push_back(histoName);
        hGammaChi2 = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),"hGammaChi2; Run Number ; Mean #chi^{2}_{#gamma}/ndf",hNBin,hFBin,hLBin);
        EditTH1(globalRuns, doEquidistantXaxis, hGammaChi2, hMarkerStyle, hMarkerSize, hMarkerColor, hLineColor);
        vecHistos.push_back(hGammaChi2);

        histoName = "hGammaChi2RMS";
        if(i==0) vecHistosName.push_back(histoName);
        hGammaChi2RMS = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),"hGammaChi2RMS; Run Number ; #sigma_{#chi^{2}_{#gamma}/ndf}",hNBin,hFBin,hLBin);
        EditTH1(globalRuns, doEquidistantXaxis, hGammaChi2RMS, hMarkerStyle, hMarkerSize, hMarkerColor, hLineColor);
        vecHistos.push_back(hGammaChi2RMS);

        histoName = "hGammaPsiPair";
        if(i==0) vecHistosName.push_back(histoName);
        hGammaPsiPair = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),"hGammaPsiPair; Run Number ; Mean #psi_{pair}",hNBin,hFBin,hLBin);
        EditTH1(globalRuns, doEquidistantXaxis, hGammaPsiPair, hMarkerStyle, hMarkerSize, hMarkerColor, hLineColor);
        vecHistos.push_back(hGammaPsiPair);

        histoName = "hGammaPsiPairRMS";
        if(i==0) vecHistosName.push_back(histoName);
        hGammaPsiPairRMS = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),"hGammaPsiPairRMS; Run Number ; #sigma_{#psi_{pair}}",hNBin,hFBin,hLBin);
        EditTH1(globalRuns, doEquidistantXaxis, hGammaPsiPairRMS, hMarkerStyle, hMarkerSize, hMarkerColor, hLineColor);
        vecHistos.push_back(hGammaPsiPairRMS);

        histoName = "hGammaCosPoint";
        if(i==0) vecHistosName.push_back(histoName);
        hGammaCosPoint = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),"hGammaCosPoint; Run Number ; Mean cos(#theta_{point})",hNBin,hFBin,hLBin);
        EditTH1(globalRuns, doEquidistantXaxis, hGammaCosPoint, hMarkerStyle, hMarkerSize, hMarkerColor, hLineColor);
        vecHistos.push_back(hGammaCosPoint);

        histoName = "hGammaCosPointRMS";
        if(i==0) vecHistosName.push_back(histoName);
        hGammaCosPointRMS = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),"hGammaCosPointRMS; Run Number ; #sigma_{cos(#theta_{point})}",hNBin,hFBin,hLBin);
        EditTH1(globalRuns, doEquidistantXaxis, hGammaCosPointRMS, hMarkerStyle, hMarkerSize, hMarkerColor, hLineColor);
        vecHistos.push_back(hGammaCosPointRMS);

        histoName = "hGammaInvMass";
        if(i==0) vecHistosName.push_back(histoName);
        hGammaInvMass = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),"hGammaInvMass; Run Number ; Mean M_{e^{+}e^{-}} (GeV/#it{c}^{2})",hNBin,hFBin,hLBin);
        EditTH1(globalRuns, doEquidistantXaxis, hGammaInvMass, hMarkerStyle, hMarkerSize, hMarkerColor, hLineColor);
        vecHistos.push_back(hGammaInvMass);

        histoName = "hGammaInvMassRMS";
        if(i==0) vecHistosName.push_back(histoName);
        hGammaInvMassRMS = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),"hGammaInvMassRMS; Run Number ; #sigma_{M_{e^{+}e^{-}}} (GeV/#it{c}^{2})",hNBin,hFBin,hLBin);
        EditTH1(globalRuns, doEquidistantXaxis, hGammaInvMassRMS, hMarkerStyle, hMarkerSize, hMarkerColor, hLineColor);
        vecHistos.push_back(hGammaInvMassRMS);

        histoName = "hGammaQt";
        if(i==0) vecHistosName.push_back(histoName);
        hGammaQt = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),"hGammaQt; Run Number ; Mean Photon #it{q}_{T}",hNBin,hFBin,hLBin);
        EditTH1(globalRuns, doEquidistantXaxis, hGammaQt, hMarkerStyle, hMarkerSize, hMarkerColor, hLineColor);
        vecHistos.push_back(hGammaQt);

        histoName = "hGammaQtRMS";
        if(i==0) vecHistosName.push_back(histoName);
        hGammaQtRMS = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),"hGammaQtRMS; Run Number ; #sigma_{Photon #it{q}_{T}}",hNBin,hFBin,hLBin);
        EditTH1(globalRuns, doEquidistantXaxis, hGammaQtRMS, hMarkerStyle, hMarkerSize, hMarkerColor, hLineColor);
        vecHistos.push_back(hGammaQtRMS);

        histoName = "hGammaAsym";
        if(i==0) vecHistosName.push_back(histoName);
        hGammaAsym = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),"hGammaAsym; Run Number ; Mean Asym",hNBin,hFBin,hLBin);
        EditTH1(globalRuns, doEquidistantXaxis, hGammaAsym, hMarkerStyle, hMarkerSize, hMarkerColor, hLineColor);
        vecHistos.push_back(hGammaAsym);

        histoName = "hGammaAsymRMS";
        if(i==0) vecHistosName.push_back(histoName);
        hGammaAsymRMS = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),"hGammaAsymRMS; Run Number ; #sigma_{Asym}",hNBin,hFBin,hLBin);
        EditTH1(globalRuns, doEquidistantXaxis, hGammaAsymRMS, hMarkerStyle, hMarkerSize, hMarkerColor, hLineColor);
        vecHistos.push_back(hGammaAsymRMS);

        histoName = "hGammaR";
        if(i==0) vecHistosName.push_back(histoName);
        hGammaR = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),"hGammaR; Run Number ; Mean Photon R",hNBin,hFBin,hLBin);
        EditTH1(globalRuns, doEquidistantXaxis, hGammaR, hMarkerStyle, hMarkerSize, hMarkerColor, hLineColor);
        vecHistos.push_back(hGammaR);

        histoName = "hGammaRRMS";
        if(i==0) vecHistosName.push_back(histoName);
        hGammaRRMS = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),"hGammaRRMS; Run Number ; #sigma_{Photon R}",hNBin,hFBin,hLBin);
        EditTH1(globalRuns, doEquidistantXaxis, hGammaRRMS, hMarkerStyle, hMarkerSize, hMarkerColor, hLineColor);
        vecHistos.push_back(hGammaRRMS);

        //-----------------------------------
        //-----------------------------------
        //-- Leptons
        //-----------------------------------
        //-----------------------------------

        for(Int_t iL=0; iL<2; iL++){
        //iL==0, Electron - iL==1, Positron
        histoName = Form("h%sEta",lepton[iL].Data());
        if(i==0) vecHistosName.push_back(histoName);
        hEta[iL] = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),Form("h%sEta; Run Number ; Mean %s #eta",lepton[iL].Data(),lepton[iL].Data()),hNBin,hFBin,hLBin);
        EditTH1(globalRuns, doEquidistantXaxis, hEta[iL], hMarkerStyle, hMarkerSize, hMarkerColor, hLineColor);
        vecHistos.push_back(hEta[iL]);

        histoName = Form("h%sEtaRMS",lepton[iL].Data());
        if(i==0) vecHistosName.push_back(histoName);
        hEtaRMS[iL] = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),Form("h%sEtaRMS; Run Number ; #sigma_{%s #eta}",lepton[iL].Data(),lepton[iL].Data()),hNBin,hFBin,hLBin);
        EditTH1(globalRuns, doEquidistantXaxis, hEtaRMS[iL], hMarkerStyle, hMarkerSize, hMarkerColor, hLineColor);
        vecHistos.push_back(hEtaRMS[iL]);

        histoName = Form("h%sPt",lepton[iL].Data());
        if(i==0) vecHistosName.push_back(histoName);
        hPt[iL] = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),Form("h%sPt; Run Number ; Mean %s #it{p}_{T}",lepton[iL].Data(),lepton[iL].Data()),hNBin,hFBin,hLBin);
        EditTH1(globalRuns, doEquidistantXaxis, hPt[iL], hMarkerStyle, hMarkerSize, hMarkerColor, hLineColor);
        vecHistos.push_back(hPt[iL]);

        histoName = Form("h%sPtRMS",lepton[iL].Data());
        if(i==0) vecHistosName.push_back(histoName);
        hPtRMS[iL] = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),Form("h%sPtRMS; Run Number ; #sigma_{%s #it{p}_{T}}",lepton[iL].Data(),lepton[iL].Data()),hNBin,hFBin,hLBin);
        EditTH1(globalRuns, doEquidistantXaxis, hPtRMS[iL], hMarkerStyle, hMarkerSize, hMarkerColor, hLineColor);
        vecHistos.push_back(hPtRMS[iL]);

        histoName = Form("hNSdEdx%s",lepton[iL].Data());
        if(i==0) vecHistosName.push_back(histoName);
        hNSdEdx[iL] = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),Form("hNSdEdx%s; Run Number ; Mean #it{n}#sigma_{e^{%s}} d#it{E}/d#it{x} TPC",lepton[iL].Data(),charge[iL].Data()),hNBin,hFBin,hLBin);
        EditTH1(globalRuns, doEquidistantXaxis, hNSdEdx[iL], hMarkerStyle, hMarkerSize, hMarkerColor, hLineColor);
        vecHistos.push_back(hNSdEdx[iL]);

        histoName = Form("hNSdEdxRMS%s",lepton[iL].Data());
        if(i==0) vecHistosName.push_back(histoName);
        hNSdEdxRMS[iL] = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),Form("hNSdEdxRMS%s; Run Number ; #sigma_{#it{n}#sigma_{e^{%s}} d#it{E}/d#it{x} TPC}",lepton[iL].Data(),charge[iL].Data()),hNBin,hFBin,hLBin);
        EditTH1(globalRuns, doEquidistantXaxis, hNSdEdxRMS[iL], hMarkerStyle, hMarkerSize, hMarkerColor, hLineColor);
        vecHistos.push_back(hNSdEdxRMS[iL]);

        histoName = Form("hTPCclusters%s",lepton[iL].Data());
        if(i==0) vecHistosName.push_back(histoName);
        hTPCclusters[iL] = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),Form("hTPCclusters%s; Run Number ; Mean number of TPC clusters in e^{%s} track",lepton[iL].Data(),charge[iL].Data()),hNBin,hFBin,hLBin);
        EditTH1(globalRuns, doEquidistantXaxis, hTPCclusters[iL], hMarkerStyle, hMarkerSize, hMarkerColor, hLineColor);
        vecHistos.push_back(hTPCclusters[iL]);

        histoName = Form("hTPCclustersRMS%s",lepton[iL].Data());
        if(i==0) vecHistosName.push_back(histoName);
        hTPCclustersRMS[iL] = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),Form("hTPCclustersRMS%s; Run Number ; #sigma_{number of TPC clusters in e^{%s} track}",lepton[iL].Data(),charge[iL].Data()),hNBin,hFBin,hLBin);
        EditTH1(globalRuns, doEquidistantXaxis, hTPCclustersRMS[iL], hMarkerStyle, hMarkerSize, hMarkerColor, hLineColor);
        vecHistos.push_back(hTPCclustersRMS[iL]);

        histoName = Form("hfTPCclusters%s",lepton[iL].Data());
        if(i==0) vecHistosName.push_back(histoName);
        hfTPCclusters[iL] = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),Form("hfTPCclusters%s; Run Number ; Mean number of TPC clusters in e^{%s} track / findable",lepton[iL].Data(),charge[iL].Data()),hNBin,hFBin,hLBin);
        EditTH1(globalRuns, doEquidistantXaxis, hfTPCclusters[iL], hMarkerStyle, hMarkerSize, hMarkerColor, hLineColor);
        vecHistos.push_back(hfTPCclusters[iL]);

        histoName = Form("hfTPCclustersRMS%s",lepton[iL].Data());
        if(i==0) vecHistosName.push_back(histoName);
        hfTPCclustersRMS[iL] = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),Form("hfTPCclustersRMS%s; Run Number ; #sigma_{number of TPC clusters in e^{%s} track / findable}",lepton[iL].Data(),charge[iL].Data()),hNBin,hFBin,hLBin);
        EditTH1(globalRuns, doEquidistantXaxis, hfTPCclustersRMS[iL], hMarkerStyle, hMarkerSize, hMarkerColor, hLineColor);
        vecHistos.push_back(hfTPCclustersRMS[iL]);
        }


    //****************************** Looping over DataSets ************************************************

        fstream fLog;
        fLog.open(Form("%s/A-%s.log",outputDir.Data(),DataSets[i].Data()), ios::out);
        fLog << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
        fLog << DataSets[i].Data() << endl;
        fLog << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
        fLog << fCollisionSystem.Data() << endl;
        fLog << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
        fLog << "processed cut: " << nameCutsPQA.Data() << endl;
        fLog << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;


        TString fRootFile;
        // TString DataSetName;
        TString fRunNumber;
        Int_t bin = -1;



        vecRuns.clear();
        // cout << "trying to read: " << fileRuns.Data() << endl;
        if(!readin(fileRuns, vecRuns)) {cout << "ERROR, no Run Numbers could be found! Returning..." << endl; return;}

        Bool_t doMergeOutput = kFALSE;
        TString mergeCommand = Form("hadd -f -k %s/%s/PhotonQA_%s.root",filePath.Data(),DataSetName.Data(),DataSetName.Data());
        //check if file exists, otherwise merge
        TFile* mergedFile = new TFile(Form("%s/%s/PhotonQA_%s.root",filePath.Data(),DataSetName.Data(),DataSetName.Data()),"READ");
        if(mergedFile->IsZombie()){doMergeOutput = kTRUE; cout << "--- Switched on merging of Runs! ---" << endl;}
        mergedFile->Close();
        delete mergedFile;

        //****************************** Looping over Runs ************************************************
        cout << endl;
        cout << "\t----------------------------------------------------------------------------" << endl;
        cout << "\tLooping over Runs of DataSet |" << (DataSets[i]).Data() << "|" << endl;
        cout << "\t----------------------------------------------------------------------------" << endl;
        cout << endl;
        fLog << "Looping over Runs:" << endl;

        vecMissingRuns[i].clear();
        for(Int_t j=0; j<(Int_t) vecRuns.size(); j++) {
            fRunNumber = vecRuns.at(j);
            if(i>=nData) fRootFile = Form("%s/%s/%s/PhotonQA_MC.root", filePath.Data(), DataSetName.Data(), fRunNumber.Data());
            else fRootFile = Form("%s/%s/%s/PhotonQA_Data.root", filePath.Data(), DataSetName.Data(), fRunNumber.Data());

            cout << endl;
            cout << "\t\t----------------------------------------------------------------------------" << endl;
            cout << Form("\t\tRun %s", fRunNumber.Data()) << "\t"<<  j+1 << "/"<< (Int_t) vecRuns.size() << endl;
            cout << "\t\tProcessing file: " << fRootFile.Data() << endl;
            cout << "\t\t----------------------------------------------------------------------------" << endl;
            cout << endl;

            Bool_t isPhotonQAFileOpen   = kTRUE;
            Bool_t needToBuildQAFile    = kFALSE;
            TFile* fPhotonQAFile = new TFile(fRootFile.Data(),"READ");
            if(fPhotonQAFile->IsZombie()){
                cout << endl;
                cout << "\t\t----------------------------------------------------------------------------" << endl;
                cout << "\t\t" << fRootFile << ", could not be opened." << endl;
                cout << endl;
                isPhotonQAFileOpen      = kFALSE;
                fPhotonQAFile->Close();
                delete fPhotonQAFile;

                needToBuildQAFile       = kTRUE;

            } else {
                TDirectory* TopDir = (TDirectory*) fPhotonQAFile->Get(Form("GammaConvV1_QA_%s_%s",nameCutsPQA.Data(),cutTreeProjection.Data()));
                if(TopDir == NULL) {
                    cout << "ERROR: TopDir not Found"<<endl;
                    cout << endl;
                    cout << "\t\t----------------------------------------------------------------------------" << endl;
                    cout << endl;
                    isPhotonQAFileOpen      = kFALSE;
                    fPhotonQAFile->Close();
                    delete fPhotonQAFile;
                    needToBuildQAFile       = kTRUE;
                }
            }

            if (needToBuildQAFile){
                TString fAnalysisResultsFile = Form("%s/%s/%s/%s", filePath.Data(), DataSetName.Data(), fRunNumber.Data(), fileName.Data());
                cout << "\t\tTrying to open " << fAnalysisResultsFile << "..." << endl;
                TFile* RootFilePQA = new TFile(fAnalysisResultsFile.Data(),"READ");
                if(RootFilePQA->IsZombie()) {
                    vecMissingRuns[i].push_back(fRunNumber);
                    cout << "INFO: ROOT file '" << fAnalysisResultsFile.Data() << "' could not be openend, continue!" << endl;
                    cout << "\t\t----------------------------------------------------------------------------" << endl;
                    RootFilePQA->Close();
                    delete RootFilePQA;

                    // check wether there are subfiles
                    TString listForTesting  = Form("%s/%s/%s/Stage*/*/%s", filePath.Data(), DataSetName.Data(), fRunNumber.Data(), fileName.Data());
                    TString listName        = Form("fileList%s.txt", fRunNumber.Data());
                    gSystem->Exec("ls "+listForTesting+ " > " + listName);

                    TString fileNamesInList[100];
                    Int_t nFilesInList                  = 0;

                    ifstream fileListName;
                    fileListName.open(listName,ios_base::in);
                    if (!fileListName) {
                        cout << "ERROR: settings " << listName.Data() << " not found!" << endl;
                        return;
                    }

                    for( TString tempLine; tempLine.ReadLine(fileListName, kTRUE); ) {
                        cout << tempLine.Data() << endl;
                        TFile* RootFilePQA = new TFile(tempLine.Data(),"READ");
                        if(!RootFilePQA->IsZombie()) {
                            fileNamesInList[nFilesInList]   = tempLine;
                            nFilesInList++;
                        }
                        RootFilePQA->Close();
                        delete RootFilePQA;
                    }

                    for (Int_t readFile = 0; readFile < nFilesInList; readFile++ ){
                        doMergeOutput = kTRUE;
                        if (readFile == 0){
                            cout << "\t\tCalling BuildHistogramsForGammaQAAdvV3 for " << fileNamesInList[readFile].Data() << endl;
                            BuildHistogramsForGammaQAAdvV3(fileNamesInList[readFile].Data(),nameCutsPQA,cutTreeProjection,(i>=nData),0,Form("%s/%s/%s/PhotonQA",filePath.Data(), DataSetName.Data(), fRunNumber.Data()),kTRUE);
                            cout << "\t\tdone!" << endl;
                            cout << "\t\t----------------------------------------------------------------------------" << endl;
                        } else {
                            cout << "\t\tCalling BuildHistogramsForGammaQAAdvV3... to add " << fileNamesInList[readFile].Data() << endl;
                            BuildHistogramsForGammaQAAdvV3(fileNamesInList[readFile].Data(),nameCutsPQA,cutTreeProjection,(i>=nData),1,Form("%s/%s/%s/PhotonQA",filePath.Data(), DataSetName.Data(), fRunNumber.Data()),kTRUE);
                            cout << "\t\tdone!" << endl;
                            cout << "\t\t----------------------------------------------------------------------------" << endl;
                        }
                    }
                } else {
                    RootFilePQA->Close();
                    delete RootFilePQA;
                    cout << "\t\tCalling BuildHistogramsForGammaQAAdvV3..." << endl;
                    BuildHistogramsForGammaQAAdvV3(fAnalysisResultsFile.Data(),nameCutsPQA,cutTreeProjection,(i>=nData),0,Form("%s/%s/%s/PhotonQA",filePath.Data(), DataSetName.Data(), fRunNumber.Data()),kTRUE);
                    cout << "\t\tdone!" << endl;
                    cout << "\t\t----------------------------------------------------------------------------" << endl;
                    doMergeOutput = kTRUE;
                    cout << "--- Switched on merging of Runs! ---" << endl;
                }
            }

            mergeCommand += Form(" %s",fRootFile.Data());
            if(!isPhotonQAFileOpen){
                fPhotonQAFile = new TFile(fRootFile.Data(),"READ");
                if(fPhotonQAFile->IsZombie()) {vecMissingRuns[i].push_back(fRunNumber); cout << "INFO: ROOT file '" << fRootFile.Data() << "' could not be openend, continue!" << endl; fPhotonQAFile->Close(); delete fPhotonQAFile; continue;}
            }

            TDirectory* TopDir = (TDirectory*) fPhotonQAFile->Get(Form("GammaConvV1_QA_%s_%s",nameCutsPQA.Data(), cutTreeProjection.Data()));
                if(TopDir == NULL) {cout << "ERROR: TopDir not Found"<<endl; return;}

            //---------
            if(doEquidistantXaxis) bin = mapBin[fRunNumber];
            else bin = fRunNumber.Atoi() - hFBin;

            Double_t nEvents = 0;
            TH1D* GOODESD = (TH1D*) TopDir->Get("histoGoodESDTracks");
            if(GOODESD){
                nEvents = (Double_t) GOODESD->Integral();
                delete GOODESD;
            }

            TH2D* GammaEtaPt = (TH2D*) TopDir->Get("histoGammaEtaPt");
            Double_t nGammas = 1.;
            if(GammaEtaPt){
                nGammas = GammaEtaPt->Integral();
                hGammaN->SetBinContent(bin, nGammas);
            }
            //---------
            fLog << "----------------------------------------------------------------------------" << endl;
            fLog << "Processing file: " << fRootFile.Data() << endl;
            fLog << "Run: " << fRunNumber.Data() << ", with NEvents: " << nEvents << endl;
            fLog << "----------------------------------------------------------------------------" << endl;
            //---------
            if( nEvents <= 1. ){cout << "Warning: number of accepted events in run: " << nEvents << "! Setting nEvents to 1..." << endl; nEvents = 1.;}
            if( nGammas <= 1. ){cout << "Warning: number of gammas: " << nGammas << "! Setting nGammas to 1..." << endl; nGammas = 1.;}
            //---------
            if(GammaEtaPt){
                TH1D * GammaEta = (TH1D*) GammaEtaPt->ProjectionX("GammaEta");
                hGammaEta->SetBinContent(bin, GammaEta->GetMean());
                hGammaEta->SetBinError(bin, GammaEta->GetMeanError());
                hGammaEtaRMS->SetBinContent(bin, GammaEta->GetRMS());
                hGammaEtaRMS->SetBinError(bin, GammaEta->GetRMSError());
                hGammaEtaNeg->SetBinContent(bin, GetHistogramIntegral(GammaEta,-1,0)/GammaEta->Integral());
                hGammaEtaNeg->SetBinError(bin, GetHistogramIntegralError(GammaEta,-1,0)/GammaEta->Integral());
                hGammaEtaPos->SetBinContent(bin, GetHistogramIntegral(GammaEta,0,1)/GammaEta->Integral());
                hGammaEtaPos->SetBinError(bin, GetHistogramIntegralError(GammaEta,0,1)/GammaEta->Integral());

                TH1D * GammaPt = (TH1D*) GammaEtaPt->ProjectionY("GammaPt");
                hGammaPt->SetBinContent(bin, GammaPt->GetMean());
                hGammaPt->SetBinError(bin, GammaPt->GetMeanError());
                hGammaPtRMS->SetBinContent(bin, GammaPt->GetRMS());
                hGammaPtRMS->SetBinError(bin, GammaPt->GetRMSError());
                GammaPt->GetXaxis()->SetTitle("Photon #it{p}_{T}");
                GammaPt->GetYaxis()->SetTitle("#frac{1}{N_{#gamma}} #frac{dN}{d#it{p}_{T}}");
                GammaPt->Sumw2();
                GammaPt->Scale(1 / nGammas);
                vecGammaPt.push_back(GammaPt);

                GammaEta->GetXaxis()->SetTitle("Photon #eta");
                GammaEta->GetXaxis()->SetRangeUser(-1,1);
                GammaEta->GetYaxis()->SetTitle("#frac{1}{N_{#gamma}} #frac{dN}{d#eta}");
                GammaEta->Sumw2();
                GammaEta->Scale(1 / nGammas);
                vecGammaEta.push_back(GammaEta);
                delete GammaEtaPt;
            }else cout << "INFO: Object |histoGammaEtaPt| could not be found! Skipping Fill..." << endl;
    //---------
            TH1D* GammaPhi = (TH1D*) TopDir->Get("histoGammaPhi");
            if(GammaPhi){
                hGammaPhi->SetBinContent(bin, GammaPhi->GetMean());
                hGammaPhi->SetBinError(bin, GammaPhi->GetMeanError());
                hGammaPhiRMS->SetBinContent(bin, GammaPhi->GetRMS());
                hGammaPhiRMS->SetBinError(bin, GammaPhi->GetRMSError());
                hGammaPhiLow->SetBinContent(bin, GetHistogramIntegral(GammaPhi,0,TMath::Pi())/GammaPhi->Integral());
                hGammaPhiLow->SetBinError(bin, GetHistogramIntegralError(GammaPhi,0,TMath::Pi())/GammaPhi->Integral());
                hGammaPhiHigh->SetBinContent(bin, GetHistogramIntegral(GammaPhi,TMath::Pi(),2*TMath::Pi())/GammaPhi->Integral());
                hGammaPhiHigh->SetBinError(bin, GetHistogramIntegralError(GammaPhi,TMath::Pi(),2*TMath::Pi())/GammaPhi->Integral());
                GammaPhi->GetXaxis()->SetTitle("Photon #phi");
                GammaPhi->GetYaxis()->SetTitle("#frac{1}{N_{#gamma}} #frac{dN}{d#phi}");
                GammaPhi->Sumw2();
                GammaPhi->Scale(1 / nGammas);
                vecGammaPhi.push_back(GammaPhi);
            }else cout << "INFO: Object |histoGammaPhi| could not be found! Skipping Fill..." << endl;

            TH1D* GammaPhiEtaNeg = (TH1D*) TopDir->Get("histoGammaPhiEtaNeg");
            if(GammaPhiEtaNeg){
                hGammaPhiEtaNeg->SetBinContent(bin, GammaPhiEtaNeg->GetMean());
                hGammaPhiEtaNeg->SetBinError(bin, GammaPhiEtaNeg->GetMeanError());
            }else cout << "INFO: Object |histoGammaPhiEtaNeg| could not be found! Skipping Fill..." << endl;

            TH1D* GammaPhiEtaPos = (TH1D*) TopDir->Get("histoGammaPhiEtaPos");
            if(GammaPhiEtaPos){
                hGammaPhiEtaPos->SetBinContent(bin, GammaPhiEtaPos->GetMean());
                hGammaPhiEtaPos->SetBinError(bin, GammaPhiEtaPos->GetMeanError());
            }else cout << "INFO: Object |histoGammaPhiEtaPos| could not be found! Skipping Fill..." << endl;

            //---------
            TH3F* GammaAlphaQtPt = (TH3F*) TopDir->Get("histoGammaAlphaQtPt");
            if(GammaAlphaQtPt){
                TH1D * GammaQt = (TH1D*) GammaAlphaQtPt->ProjectionY("GammaQt");
                hGammaQt->SetBinContent(bin, GammaQt->GetMean());
                hGammaQt->SetBinError(bin, GammaQt->GetMeanError());
                hGammaQtRMS->SetBinContent(bin, GammaQt->GetRMS());
                hGammaQtRMS->SetBinError(bin, GammaQt->GetRMSError());
                delete GammaQt;

                TH1D * GammaAlpha = (TH1D*) GammaAlphaQtPt->ProjectionX("GammaAlpha");
                hGammaAlpha->SetBinContent(bin, GammaAlpha->GetMean());
                hGammaAlpha->SetBinError(bin, GammaAlpha->GetMeanError());
                hGammaAlphaRMS->SetBinContent(bin, GammaAlpha->GetRMS());
                hGammaAlphaRMS->SetBinError(bin, GammaAlpha->GetRMSError());
                GammaAlpha->GetXaxis()->SetTitle("Photon #alpha");
                GammaAlpha->GetYaxis()->SetTitle("#frac{1}{N_{#gamma}} #frac{dN}{d#alpha}");
                GammaAlpha->Sumw2();
                GammaAlpha->Scale(1 / nGammas);
                vecGammaAlpha.push_back(GammaAlpha);
//                 delete GammaAlpha;
                delete GammaAlphaQtPt;
            }else cout << "INFO: Object |GammaAlphaQtPt| could not be found! Skipping Fill..." << endl;

            TH3F* GammaChi2PsiPairPt = (TH3F*) TopDir->Get("histoGammaChi2PsiPairPt");
            if(GammaChi2PsiPairPt){
                TH1D * GammaChi2 = (TH1D*) GammaChi2PsiPairPt->ProjectionX("GammaChi2");
                hGammaChi2->SetBinContent(bin, GammaChi2->GetMean());
                hGammaChi2->SetBinError(bin, GammaChi2->GetMeanError());
                hGammaChi2RMS->SetBinContent(bin, GammaChi2->GetRMS());
                hGammaChi2RMS->SetBinError(bin, GammaChi2->GetRMSError());
                GammaChi2->GetXaxis()->SetTitle("Photon #chi^{2}_{#gamma}/ndf");
                GammaChi2->GetYaxis()->SetTitle("#frac{1}{N_{#gamma}} #frac{dN}{d#chi^{2}}");
                GammaChi2->Sumw2();
                GammaChi2->Scale(1 / nGammas);
                vecGammaChi2.push_back(GammaChi2);
//                 delete GammaChi2;

                TH1D * GammaPsiPair = (TH1D*) GammaChi2PsiPairPt->ProjectionY("GammaPsiPair");
                hGammaPsiPair->SetBinContent(bin, GammaPsiPair->GetMean());
                hGammaPsiPair->SetBinError(bin, GammaPsiPair->GetMeanError());
                hGammaPsiPairRMS->SetBinContent(bin, GammaPsiPair->GetRMS());
                hGammaPsiPairRMS->SetBinError(bin, GammaPsiPair->GetRMSError());
                GammaPsiPair->GetXaxis()->SetTitle("Photon #psi_{pair}");
                GammaPsiPair->GetYaxis()->SetTitle("#frac{1}{N_{#gamma}} #frac{dN}{d#psi_{pair}}");
                GammaPsiPair->Sumw2();
                GammaPsiPair->Scale(1 / nGammas);
                vecGammaPsiPair.push_back(GammaPsiPair);
//                 delete GammaPsiPair;
                delete GammaChi2PsiPairPt;
            }else cout << "INFO: Object |histoGammaChi2PsiPairPt| could not be found! Skipping Fill..." << endl;

            TH2D* GammaCosPointPt = (TH2D*) TopDir->Get("histoGammaCosPointPt");
            if(GammaCosPointPt){
                TH1D * GammaCosPoint = (TH1D*) GammaCosPointPt->ProjectionX("GammaCosPoint");
                hGammaCosPoint->SetBinContent(bin, GammaCosPoint->GetMean());
                hGammaCosPoint->SetBinError(bin, GammaCosPoint->GetMeanError());
                hGammaCosPointRMS->SetBinContent(bin, GammaCosPoint->GetRMS());
                hGammaCosPointRMS->SetBinError(bin, GammaCosPoint->GetRMSError());
                GammaCosPoint->GetXaxis()->SetTitle("Photon cos(#theta_{point})");
                GammaCosPoint->GetYaxis()->SetTitle("#frac{1}{N_{#gamma}} #frac{dN}{d cos(#theta_{point}})");
                GammaCosPoint->Sumw2();
                GammaCosPoint->Scale(1 / nGammas);
                vecGammaCosPoint.push_back(GammaCosPoint);
//                 delete GammaCosPoint;
                delete GammaCosPointPt;
            }else cout << "INFO: Object |histoGammaCosPointPt| could not be found! Skipping Fill..." << endl;

            TH2D* GammaInvMassPt = (TH2D*) TopDir->Get("histoGammaInvMassPt");
            if(GammaInvMassPt){
                TH1D * GammaInvMass = (TH1D*) GammaInvMassPt->ProjectionX("GammaInvMass");
                hGammaInvMass->SetBinContent(bin, GammaInvMass->GetMean());
                hGammaInvMass->SetBinError(bin, GammaInvMass->GetMeanError());
                hGammaInvMassRMS->SetBinContent(bin, GammaInvMass->GetRMS());
                hGammaInvMassRMS->SetBinError(bin, GammaInvMass->GetRMSError());
                GammaInvMass->GetXaxis()->SetTitle("M_{e^{+}e^{-}}");
                GammaInvMass->GetYaxis()->SetTitle("#frac{1}{N_{#gamma}} #frac{dN}{dM_{e^{+}e^{-}}}");
                GammaInvMass->Sumw2();
                GammaInvMass->Scale(1 / nGammas);
                vecGammaInvMass.push_back(GammaInvMass);
//                 delete GammaInvMass;
                delete GammaInvMassPt;
            }else cout << "INFO: Object |histoGammaInvMassPt| could not be found! Skipping Fill..." << endl;

            TH2D* GammaInvMassR = (TH2D*) TopDir->Get("histoGammaInvMassR");
            if(GammaInvMassR){
                GammaInvMassR->SetTitle(fRunNumber);
                vecInvMassR.push_back(GammaInvMassR);
//                 delete GammaInvMassR;
            }else cout << "INFO: Object |histoGammaInvMassR| could not be found! Skipping Fill..." << endl;

            TH2D* GammaEtaR = (TH2D*) TopDir->Get("histoGammaEtaR");
            if(GammaEtaR){
                GammaEtaR->SetTitle(fRunNumber);
                vecEtaR.push_back(GammaEtaR);
//                 delete GammaEtaR;
            }else cout << "INFO: Object |histoGammaEtaR| could not be found! Skipping Fill..." << endl;

            TH2D* GammaPhiR = (TH2D*) TopDir->Get("histoGammaPhiR");
            if(GammaPhiR){
                GammaPhiR->SetTitle(fRunNumber);
                vecPhiR.push_back(GammaPhiR);
//                 delete GammaPhiR;
            }else cout << "INFO: Object |histoGammaPhiR| could not be found! Skipping Fill..." << endl;

            TH2D* GammaAlphaR = (TH2D*) TopDir->Get("histoGammaAlphaR");
            if(GammaAlphaR){
                GammaAlphaR->SetTitle(fRunNumber);
                vecAlphaR.push_back(GammaAlphaR);
//                 delete GammaAlphaR;
            }else cout << "INFO: Object |histoGammaAlphaR| could not be found! Skipping Fill..." << endl;

            TH2D* GammaPsiPairR = (TH2D*) TopDir->Get("histoGammaPsiPairR");
            if(GammaPsiPairR){
                GammaPsiPairR->SetTitle(fRunNumber);
                vecPsiPairR.push_back(GammaPsiPairR);
//                 delete GammaPsiPairR;
            }else cout << "INFO: Object |histoGammaPsiPairR| could not be found! Skipping Fill..." << endl;

            TH2D* GammaAsymR = (TH2D*) TopDir->Get("histoGammaAsymR");
            if(GammaAsymR){
                TH1D * GammaAsym = (TH1D*) GammaAsymR->ProjectionX("GammaAsym");
                hGammaAsym->SetBinContent(bin, GammaAsym->GetMean());
                hGammaAsym->SetBinError(bin, GammaAsym->GetMeanError());
                hGammaAsymRMS->SetBinContent(bin, GammaAsym->GetRMS());
                hGammaAsymRMS->SetBinError(bin, GammaAsym->GetRMSError());

                delete GammaAsym;

                TH1D * GammaR = (TH1D*) GammaAsymR->ProjectionY("GammaR");
                hGammaR->SetBinContent(bin, GammaR->GetMean());
                hGammaR->SetBinError(bin, GammaR->GetMeanError());
                hGammaRRMS->SetBinContent(bin, GammaR->GetRMS());
                hGammaRRMS->SetBinError(bin, GammaR->GetRMSError());
                delete GammaR;

                GammaAsymR->SetTitle(fRunNumber);
                vecAsymR.push_back(GammaAsymR);
//                 delete GammaAsymR;
            }else cout << "INFO: Object |histoGammaAsymR| could not be found! Skipping Fill..." << endl;

            //-----------------------------------
            //-----------------------------------
            //-- Leptons
            //-----------------------------------
            //-----------------------------------

            for(Int_t iL=0; iL<2; iL++){
                //iL==0, Electron - iL==1, Positron

                TH3D* NSigmadEdxEtaP = (TH3D*) TopDir->Get(Form("histo%sNSigmadEdxEtaP",lepton[iL].Data()));
                if(NSigmadEdxEtaP){
                    TH1D * NSigmadEdx = (TH1D*) NSigmadEdxEtaP->Project3D("x");
                    hNSdEdx[iL]->SetBinContent(bin, NSigmadEdx->GetMean(1));
                    hNSdEdx[iL]->SetBinError(bin, NSigmadEdx->GetMeanError(1));
                    hNSdEdxRMS[iL]->SetBinContent(bin, NSigmadEdx->GetRMS(1));
                    hNSdEdxRMS[iL]->SetBinError(bin, NSigmadEdx->GetRMSError(1));
                    delete NSigmadEdx;
                    TH1D * Eta = (TH1D*) NSigmadEdxEtaP->Project3D("y");
                    hEta[iL]->SetBinContent(bin, Eta->GetMean(1));
                    hEta[iL]->SetBinError(bin, Eta->GetMeanError(1));
                    hEtaRMS[iL]->SetBinContent(bin, Eta->GetRMS(1));
                    hEtaRMS[iL]->SetBinError(bin, Eta->GetRMSError(1));
                    delete Eta;
                    TH1D * Pt = (TH1D*) NSigmadEdxEtaP->Project3D("z");
                    hPt[iL]->SetBinContent(bin, Pt->GetMean(1));
                    hPt[iL]->SetBinError(bin, Pt->GetMeanError(1));
                    hPtRMS[iL]->SetBinContent(bin, Pt->GetRMS(1));
                    hPtRMS[iL]->SetBinError(bin, Pt->GetRMSError(1));
                    delete Pt;
                    delete NSigmadEdxEtaP;
                }else cout << Form("INFO: Object |histo%sNSigmadEdxEtaP| could not be found! Skipping Fill...",lepton[iL].Data()) << endl;

                TH2D* TPCclustersPt= (TH2D*) TopDir->Get(Form("histo%sClPt",lepton[iL].Data()));
                if(TPCclustersPt){
                    TH1D * TPCclusters = (TH1D*) TPCclustersPt->ProjectionX("TPCclusters");
                    hTPCclusters[iL]->SetBinContent(bin, TPCclusters->GetMean());
                    hTPCclusters[iL]->SetBinError(bin, TPCclusters->GetMeanError());
                    hTPCclustersRMS[iL]->SetBinContent(bin, TPCclusters->GetRMS());
                    hTPCclustersRMS[iL]->SetBinError(bin, TPCclusters->GetRMSError());
                    delete TPCclusters;
                    delete TPCclustersPt;
                }else cout << Form("INFO: Object |histo%sClPt| could not be found! Skipping Fill...", lepton[iL].Data()) << endl;

                TH2D* fTPCclustersPt= (TH2D*) TopDir->Get(Form("histo%sFClPt",lepton[iL].Data()));
                if(fTPCclustersPt){
                    TH1D * fTPCclusters = (TH1D*) fTPCclustersPt->ProjectionX("fTPCclusters");
                    hfTPCclusters[iL]->SetBinContent(bin, fTPCclusters->GetMean());
                    hfTPCclusters[iL]->SetBinError(bin, fTPCclusters->GetMeanError());
                    hfTPCclustersRMS[iL]->SetBinContent(bin, fTPCclusters->GetRMS());
                    hfTPCclustersRMS[iL]->SetBinError(bin, fTPCclusters->GetRMSError());
                    delete fTPCclusters;
                    delete fTPCclustersPt;
                }else cout << Form("INFO: Object |histo%sFClPt| could not be found! Skipping Fill...", lepton[iL].Data()) << endl;
                //---------
                TH2D* NSdEdx = (TH2D*) TopDir->Get(Form("histo%sNSigmadEdxPhi",lepton[iL].Data()));
                if(NSdEdx){
                    vecNSdEdx[iL].push_back(NSdEdx);
//                     delete NSdEdx;
                }else cout << Form("INFO: Object |histo%sNSigmadEdxPhi| could not be found! Skipping Fill...",lepton[iL].Data()) << endl;

                TH2D* TPCclustersR = (TH2D*) TopDir->Get(Form("histo%sClR",lepton[iL].Data()));
                if(TPCclustersR){
                    vecTPCClusR[iL].push_back(TPCclustersR);
//                     delete TPCclustersR;
                }else cout << Form("INFO: Object |histo%sClR| could not be found! Skipping Fill...",lepton[iL].Data()) << endl;

            }

            TopDir->Clear();
            delete TopDir;

            fPhotonQAFile->Close();
            delete fPhotonQAFile;
        }

        if(doMergeOutput) {
            cout << "Flag doMergeOutput = kTRUE, merging PhotonQA output..." << endl;
            gSystem->Exec(mergeCommand.Data());
            cout << "done!" << endl;
        }

        //****************************** Drawing Histograms ************************************************
        cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << "Drawing Histograms" << endl;

        TCanvas* canvas2D = new TCanvas("canvas2D","",10,10,750*6,500*6);  // gives the page size

        DrawGammaCanvasSettings(canvas2D, leftMar, rightMar, topMargin, bottomMargin);
        // TCanvas* canvas = new TCanvas("canvas","",10,10,750,500);  // gives the page size
        // DrawGammaCanvasSettings(canvas, leftMar, rightMar, topMargin, bottomMargin);
        TCanvas* canvas;
        Int_t width = (Int_t)((Double_t)vecRuns.size()*1.5);
        canvas = new TCanvas("canvas","",10,10,750,500);  // gives the page size
        // canvas = new TCanvas("canvas","",10,10,width,500);  // gives the page size
        DrawGammaCanvasSettings(canvas, leftMar, rightMar, topMargin, bottomMargin);

        if(fEnergyFlag.Contains("PbPb")) xPosLabel = 0.75;

        if(doHistsForEverySet) {
            TBox *boxLabel = new TBox(1.37,0.7,1.78,0.83);
            boxLabel->SetFillStyle(0);boxLabel->SetFillColor(0);boxLabel->SetLineColor(1);boxLabel->SetLineWidth(1);
            TBox *boxLabel2 = new TBox(-0.4,51,6.5,56.5);
            boxLabel2->SetFillStyle(0);boxLabel2->SetFillColor(0);boxLabel2->SetLineColor(1);boxLabel2->SetLineWidth(1);

            TString outputDirDataSet;

            cout << "DataSet: " << DataSets[i].Data() << endl;
            outputDirDataSet = Form("%s/%s",outputDir.Data(), DataSets[i].Data());
            if(useDataRunListForMC && i>=nData) {
                outputDirDataSet = Form("%s/%s-%s", outputDir.Data(), DataSets[i].Data(),DataSets[0].Data());
                cout << "Switch useDataRunListForMC is true, output to: " << outputDirDataSet.Data() << endl;
            }
            gSystem->Exec("mkdir -p "+outputDirDataSet);

            fTrigger = "";
            if(doTrigger && i<nData){
                cout << "Obtaining trigger - ";
                TString fTriggerCut = nameCutsPQAshort(3,2);
                fTrigger = AnalyseSpecialTriggerCut(fTriggerCut.Atoi(), plotDataSets[i]);
                cout << "'" << fTrigger.Data() << "' - was found!" << endl;
                if(fTrigger.Contains("not defined")){
                    fTrigger = "";
                    cout << "INFO: Trigger cut not defined!" << endl;
                }
            }

            DrawVectorOverviewTH2D(canvas2D, vecInvMassR, "oInvMassR", outputDirDataSet, "jpg",
                                        0.13, 0.15, 0.1, 0.1, 0.6, 0.8, 0.12, 0.93, 0x0, kFALSE, kFALSE);
            DrawVectorOverviewTH2D(canvas2D, vecEtaR, "oEtaR", outputDirDataSet, "jpg",
                                        0.13, 0.15, 0.1, 0.1, 0.6, 0.8, 0.12, 0.93, 0x0, kFALSE, kFALSE);
            DrawVectorOverviewTH2D(canvas2D, vecPhiR, "oPhiR", outputDirDataSet, "jpg",
                                        0.13, 0.15, 0.1, 0.1, 0.6, 0.8, 0.12, 0.93, 0x0, kFALSE, kFALSE);
            DrawVectorOverviewTH2D(canvas2D, vecAlphaR, "oAlphaR", outputDirDataSet, "jpg",
                                        0.13, 0.15, 0.1, 0.1, 0.6, 0.8, 0.12, 0.93, 0x0, kFALSE, kFALSE);
            DrawVectorOverviewTH2D(canvas2D, vecPsiPairR, "oPsiPairR", outputDirDataSet, "jpg",
                                        0.13, 0.15, 0.1, 0.1, 0.6, 0.8, 0.12, 0.93, 0x0, kFALSE, kFALSE);
            DrawVectorOverviewTH2D(canvas2D, vecAsymR, "oAsymR", outputDirDataSet, "jpg",
                                        0.13, 0.15, 0.1, 0.1, 0.6, 0.8, 0.12, 0.93, 0x0, kFALSE, kFALSE);

            DrawVectorOverviewTH2D(canvas2D, vecNSdEdx[0], "oNSdEdx_Electron", outputDirDataSet, "jpg",
                                        0.13, 0.15, 0.1, 0.1, 0.6, 0.8, 0.12, 0.93, 0x0, kFALSE, kFALSE);
            DrawVectorOverviewTH2D(canvas2D, vecNSdEdx[1], "oNSdEdx_Positron", outputDirDataSet, "jpg",
                                        0.13, 0.15, 0.1, 0.1, 0.6, 0.8, 0.12, 0.93, 0x0, kFALSE, kFALSE);
            DrawVectorOverviewTH2D(canvas2D, vecTPCClusR[0], "oTPCClusR_Electron", outputDirDataSet, "jpg",
                                        0.13, 0.15, 0.1, 0.1, 0.6, 0.8, 0.12, 0.93, 0x0, kFALSE, kFALSE);
            DrawVectorOverviewTH2D(canvas2D, vecTPCClusR[1], "oTPCClusR_Positron", outputDirDataSet, "jpg",
                                        0.13, 0.15, 0.1, 0.1, 0.6, 0.8, 0.12, 0.93, 0x0, kFALSE, kFALSE);

        //--------
            for(Int_t h=0; h<(Int_t) vecHistos.size(); h++)
            {
                (((TH1D*) vecHistos.at(h)))->SetTitle("");
                AdjustHistRange(((TH1D*) vecHistos.at(h)),1.1,1.1,kTRUE);
                ((TH1D*) vecHistos.at(h))->GetYaxis()->SetTitleOffset(1.*750/width);
                if(((TString)vecHistosName.at(h)).CompareTo("hGammaCosPoint")==0) AdjustHistRange(((TH1D*) vecHistos.at(h)),1.001,1.001,kTRUE);
                if(((TString)vecHistosName.at(h)).CompareTo("nGamma")==0) AdjustHistRange(((TH1D*) vecHistos.at(h)),10,10,kTRUE);
                ((TH1D*) vecHistos.at(h))->Draw("px0e1");

                if(doTrigger) PutProcessLabelAndEnergyOnPlot(xPosLabel, 0.92, 0.03, fCollisionSystem.Data(), plotDataSets[i].Data(), fTrigger.Data());
                else PutProcessLabelAndEnergyOnPlot(xPosLabel, 0.92, 0.03, fCollisionSystem.Data(), plotDataSets[i].Data(), "");

                if(((TString)vecHistosName.at(h)).CompareTo("nGamma")==0) SaveCanvas(canvas, Form("%s/%s.%s", outputDirDataSet.Data(), vecHistosName.at(h).Data(), suffix.Data()), kFALSE, kTRUE);
                else SaveCanvas(canvas, Form("%s/%s.%s", outputDirDataSet.Data(), vecHistosName.at(h).Data(), suffix.Data()));
            }
            delete boxLabel;
            delete boxLabel2;
            //****************************** Drawing Runwise Histograms ************************************************
            cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
            cout << "Drawing Runwise Histograms" << endl;

            TCanvas* canvasRunwise;
            TLegend *legendRuns;

            vecRuns.clear();
            DataSetName = DataSets[i];
            outputDirDataSet = Form("%s/%s",outputDir.Data(), DataSets[i].Data());
            if(useDataRunListForMC && i>=nData) {
                outputDirDataSet = Form("%s/%s-%s", outputDir.Data(), DataSets[i].Data(),DataSets[0].Data());
            }
            if(!readin(fileRuns, vecRuns, kFALSE)) {cout << "ERROR, no Run Numbers could be found! Returning..." << endl; return;}

            for(Int_t iRun=0; iRun<(Int_t)vecMissingRuns[i].size(); iRun++){
                vecRuns.erase(std::remove(vecRuns.begin(), vecRuns.end(), vecMissingRuns[i].at(iRun)), vecRuns.end());
            }

            Int_t NColumns = ((Int_t) vecRuns.size() / 31 ) + 1;

            canvasRunwise = new TCanvas("canvasRunwise","",10,10,1350+(NColumns*108),900);  // gives the page size
            DrawGammaCanvasSettings(canvasRunwise, 130.5/(1350.+(NColumns*108.)), (40.5+(NColumns*108.))/(1350.+(NColumns*108.)), topMargin, bottomMargin);
            canvasRunwise->cd();

            Double_t addRight = ((Double_t)NColumns*108)/(1350+((Double_t)NColumns*108));
            legendRuns = new TLegend(0.98-addRight,0.09,0.98,0.94);
            legendRuns->SetNColumns(NColumns);
            legendRuns->SetFillColor(0);
            legendRuns->SetLineColor(0);
            legendRuns->SetTextSize(0.03);
            legendRuns->SetTextFont(42);

            cout << "DataSet: " << plotDataSets[i].Data() << endl;
            fTrigger = "";
            if(i<nData){
                TString fTriggerCut = nameCutsPQAshort(3,2);
                fTrigger = AnalyseSpecialTriggerCut(fTriggerCut.Atoi(), plotDataSets[i]);
                if(fTrigger.Contains("not defined")) fTrigger = "";
            }

            //---------

            DrawVectorRunwiseTH1D(	canvasRunwise, legendRuns, vecGammaPt, vecRuns, 5, 5, kTRUE, addRight, xPosLabel, 0.94, 0.03, xPosLabel, 0.8, 0.03,
                                    doTrigger, fTrigger, (Bool_t)(i<nData), outputDirDataSet, "GammaPt", plotDataSets[i], kFALSE,
                                    fCollisionSystem, "", suffix, kTRUE, kTRUE, kFALSE);

            DrawVectorRunwiseTH1D(	canvasRunwise, legendRuns, vecGammaAlpha, vecRuns, 2, 1.1, kTRUE, addRight, xPosLabel, 0.94, 0.03, xPosLabel, 0.8, 0.03,
                                    doTrigger, fTrigger, (Bool_t)(i<nData), outputDirDataSet, "GammaAlpha", plotDataSets[i], kFALSE,
                                    fCollisionSystem, "", suffix, kFALSE, kFALSE, kFALSE);

            DrawVectorRunwiseTH1D(	canvasRunwise, legendRuns, vecGammaEta, vecRuns, 2, 1.1, kTRUE, addRight, xPosLabel, 0.94, 0.03, xPosLabel, 0.8, 0.03,
                                    doTrigger, fTrigger, (Bool_t)(i<nData), outputDirDataSet, "GammaEta", plotDataSets[i], kFALSE,
                                    fCollisionSystem, "", suffix, kFALSE, kFALSE, kFALSE);

            DrawVectorRunwiseTH1D(	canvasRunwise, legendRuns, vecGammaPhi, vecRuns, 2, 1.1, kTRUE, addRight, xPosLabel, 0.94, 0.03, xPosLabel, 0.8, 0.03,
                                    doTrigger, fTrigger, (Bool_t)(i<nData), outputDirDataSet, "GammaPhi", plotDataSets[i], kFALSE,
                                    fCollisionSystem, "", suffix, kFALSE, kFALSE, kFALSE);

            DrawVectorRunwiseTH1D(	canvasRunwise, legendRuns, vecGammaChi2, vecRuns, 5, 5, kTRUE, addRight, xPosLabel, 0.94, 0.03, xPosLabel, 0.8, 0.03,
                                    doTrigger, fTrigger, (Bool_t)(i<nData), outputDirDataSet, "GammaChi2", plotDataSets[i], kFALSE,
                                    fCollisionSystem, "", suffix, kFALSE, kTRUE, kFALSE);

            DrawVectorRunwiseTH1D(	canvasRunwise, legendRuns, vecGammaPsiPair, vecRuns, 2, 1.1, kTRUE, addRight, xPosLabel, 0.94, 0.03, xPosLabel, 0.8, 0.03,
                                    doTrigger, fTrigger, (Bool_t)(i<nData), outputDirDataSet, "GammaPsiPair", plotDataSets[i], kFALSE,
                                    fCollisionSystem, "", suffix, kFALSE, kFALSE, kFALSE);

            DrawVectorRunwiseTH1D(	canvasRunwise, legendRuns, vecGammaCosPoint, vecRuns, 5, 5, kTRUE, addRight, xPosLabel, 0.94, 0.03, xPosLabel, 0.8, 0.03,
                                    doTrigger, fTrigger, (Bool_t)(i<nData), outputDirDataSet, "GammaCosPoint", plotDataSets[i], kFALSE,
                                    fCollisionSystem, "", suffix, kTRUE, kTRUE, kFALSE);

            DrawVectorRunwiseTH1D(	canvasRunwise, legendRuns, vecGammaInvMass, vecRuns, 5, 5, kTRUE, addRight, xPosLabel, 0.94, 0.03, xPosLabel, 0.8, 0.03,
                                    doTrigger, fTrigger, (Bool_t)(i<nData), outputDirDataSet, "GammaInvMass", plotDataSets[i], kFALSE,
                                    fCollisionSystem, "", suffix, kFALSE, kTRUE, kFALSE);

        //--------
            delete legendRuns;
            delete canvasRunwise;

            canvas->cd();
        }


//****************************** Create Output ROOT-File ************************************************

        // const char* nameOutput;

    // for(Int_t i=0; i<nSets; i++) {
        DataSetName = DataSets[i];

        if(useDataRunListForMC && i>=nData) nameOutput = Form("%s_%s/%s/PhotonQA/%s-%s_PhotonQARunwise.root",nameCutsPQA.Data(), cutTreeProjection.Data() ,fEnergyFlag.Data(),DataSetName.Data(),DataSets[0].Data());
        else nameOutput = Form("%s_%s/%s/PhotonQA/%s_PhotonQARunwise.root",nameCutsPQA.Data(),cutTreeProjection.Data(),fEnergyFlag.Data(),DataSetName.Data());

        TFile* fOutput = new TFile(nameOutput,"RECREATE");
        cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << "Output file: " << nameOutput << endl;
        cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
        fLog << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
        fLog << "Output file: " << nameOutput << endl;
        fLog << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;

        for(Int_t h=0; h<(Int_t) vecHistos.size(); h++) WriteHistogram(((TH1D*) vecHistos.at(h)));

        DeleteVecTH1D(vecGammaPt);
        DeleteVecTH1D(vecGammaAlpha);
        DeleteVecTH1D(vecGammaEta);
        DeleteVecTH1D(vecGammaPhi);
        DeleteVecTH1D(vecGammaChi2);
        DeleteVecTH1D(vecGammaPsiPair);
        DeleteVecTH1D(vecGammaCosPoint);
        DeleteVecTH1D(vecGammaInvMass);

        vecHistos.clear();
        DeleteVecTH1D(vecHistos);

        WriteHistogramTH2DVec(fOutput,vecAlphaR,"GammaAlphaR");
        WriteHistogramTH2DVec(fOutput,vecEtaR,"GammaEtaR");
        WriteHistogramTH2DVec(fOutput,vecPhiR,"GammaPhiR");
        WriteHistogramTH2DVec(fOutput,vecInvMassR,"GammaInvMassR");
        WriteHistogramTH2DVec(fOutput,vecPsiPairR,"GammaPsiPairR");
        WriteHistogramTH2DVec(fOutput,vecAsymR,"GammaAsymR");

        // vecAlphaR.clear();
        // vecEtaR.clear();
        // vecPhiR.clear();
        // vecInvMassR.clear();
        // vecPsiPairR.clear();
        // vecAsymR.clear();

        WriteHistogramTH2DVec(fOutput,vecNSdEdx[0],"ElectronNSdEdx");
        WriteHistogramTH2DVec(fOutput,vecNSdEdx[1],"PositronNSdEdx");
        WriteHistogramTH2DVec(fOutput,vecTPCClusR[0],"ElectronTPCClusR");
        WriteHistogramTH2DVec(fOutput,vecTPCClusR[1],"PositronTPCClusR");

        // vecNSdEdx[0].clear();
        // vecNSdEdx[1].clear();
        // vecTPCClusR[0].clear();
        // vecTPCClusR[1].clear();

        fOutput->Write();
        fOutput->Close();
        delete fOutput;
        fLog.close();

}
    //****************************** Combined Trending Histograms ************************************************
    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << "Drawing Trending Histograms" << endl;
    //---------
    if(useDataRunListForMC) cout << "WARNING: useDataRunListForMC is true, overwriting histograms for DataSet!" << endl;

    TCanvas* canvas;
    canvas = new TCanvas("canvas","",10,10,750,500);  // gives the page size
    // canvas = new TCanvas("canvas","",10,10,width,500);  // gives the page size
    DrawGammaCanvasSettings(canvas, leftMar, rightMar, topMargin, bottomMargin);
    TLegend *legend = new TLegend(0.15,0.95,0.95,0.98);
    legend->SetNColumns(nSets);
    legend->SetFillColor(0);
    legend->SetLineColor(0);
    legend->SetTextSize(0.03);
    legend->SetTextFont(42);
    //--------------------------------------------------------------------------------------------------------
    std::vector< TH1D* > vecHTMP;
    if(nSets > 1) {
        TH1D* hTMP;
        // for(Int_t h=0; h<(Int_t) vecHistos[0].size(); h++) {
        for(Int_t h=0; h<(Int_t) vecHistosName.size(); h++){
            fTrigger = "";
            printf("%s\n", vecHistosName.at(h).Data());
            for(Int_t i=0; i<nSets; i++) {
                if(useDataRunListForMC && i>=nData) nameOutput = Form("%s_%s/%s/PhotonQA/%s-%s_PhotonQARunwise.root",nameCutsPQA.Data(), cutTreeProjection.Data() ,fEnergyFlag.Data(),DataSetName.Data(),DataSets[0].Data());
                else nameOutput = Form("%s_%s/%s/PhotonQA/%s_PhotonQARunwise.root",nameCutsPQA.Data(),cutTreeProjection.Data(),fEnergyFlag.Data(),DataSetName.Data());
                if(doTrigger && i<nData && i==0){
                    TString fTriggerCut = nameCutsPQAshort(3,2);
                    fTrigger = AnalyseSpecialTriggerCut(fTriggerCut.Atoi(), plotDataSets[i]);
                    if(fTrigger.Contains("not defined")) fTrigger = "";
                }
                // printf("%s\n", nameOutput);
                TFile* fOutput = new TFile(nameOutput,"READ");
                fOutput->cd();
                hTMP = (TH1D*)(fOutput->Get(Form("%s_%s",vecHistosName.at(h).Data(), DataSets[i].Data())))->Clone(Form("%s_%s_%i",vecHistosName.at(h).Data(), DataSets[i].Data(),i));
                if(!hTMP) {
                    printf("\n%s-%s\t\033[0;31mERROR\033[0m not found \n",  DataSets[i].Data(), vecHistosName.at(h).Data());
                    continue;
                }

                hTMP->SetDirectory(0);
                vecHTMP.push_back(hTMP);
                printf("%s, ",  DataSets[i].Data());

                fOutput->Close();
                delete fOutput;
            }

            legend->AddEntry(((TH1D*) vecHTMP.at(0)),plotDataSets[0].Data(),"p");
            AdjustHistRange(vecHTMP,1.1,1.1,h,nSets,kTRUE);
            if(((TString)vecHistosName.at(h)).CompareTo("hGammaCosPoint")==0) AdjustHistRange(vecHTMP,1.001,1.001,h,nSets,kTRUE);
            if(((TString)vecHistosName.at(h)).CompareTo("hGammaN")==0) AdjustHistRange(vecHTMP,10,10,h,nSets,kTRUE);
            for(Int_t i=nSets-1; i>=0; i--) {
                TString draw = (i==nSets-1)?"px0e1":"px0e1, same";
                ((TH1D*) vecHTMP.at(h))->SetTitle("");
                ((TH1D*) vecHTMP.at(h))->Draw(draw.Data());
            }
            legend->Draw();

            if(doTrigger) PutProcessLabelAndEnergyOnPlot(xPosLabel, 0.92, 0.03, fCollisionSystem.Data(), fTrigger.Data(), "");
            else PutProcessLabelAndEnergyOnPlot(xPosLabel, 0.92, 0.03, fCollisionSystem.Data(), "", "");

            if(canvas->GetTopMargin()!=0.06) canvas->SetTopMargin(0.06);
            if(useDataRunListForMC && !addSubFolder){
                if( ((TString)vecHistosName.at(h)).CompareTo("nGamma")==0 ) SaveCanvas(canvas, Form("%s/%s/%s.%s", outputDir.Data(),
                DataSets[0].Data(),vecHistosName.at(h).Data(),suffix.Data()), kFALSE, kTRUE);
                else SaveCanvas(canvas, Form("%s/%s/%s.%s", outputDir.Data(), DataSets[0].Data(),vecHistosName.at(h).Data(),suffix.Data()));
            }else{
                if(((TString)vecHistosName.at(h)).CompareTo("nGamma")==0) SaveCanvas(canvas, Form("%s/%s.%s", outputDir.Data(), vecHistosName.at(h).Data(), suffix.Data()), kFALSE, kTRUE);
                else SaveCanvas(canvas, Form("%s/%s.%s", outputDir.Data(), vecHistosName.at(h).Data(), suffix.Data()));
            }
            legend->Clear();

        }
    }

//****************************** Combined Ratio Trending Histograms ************************************************
    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << "Drawing Ratio Trending Histograms" << endl;

    if(doHistsForEverySet) {
        TString* ratioSets[nSets-nData];
        TH1D* ratio[nSets-nData];
        TString outputDirDataSet;
        std::vector< TH1D* > vecHTMP;

        if(nSets>1 && nSets>nData) {
            TH1D* hTMP;
            legend->SetNColumns(nData*(nSets-nData));
            Int_t markerStyles[14]={2,4,5,20,21,22,23,24,25,26,27,28,29,30};

            // for(Int_t h=0; h<(Int_t) vecHistos[0].size(); h++) {
            for(Int_t h=0; h<(Int_t) vecHistosName.size(); h++){
                printf("%s\n", vecHistosName.at(h).Data());
                for(Int_t i=0; i<nSets; i++) {
                    if(useDataRunListForMC && i>=nData) nameOutput = Form("%s_%s/%s/PhotonQA/%s-%s_PhotonQARunwise.root",nameCutsPQA.Data(), cutTreeProjection.Data() ,fEnergyFlag.Data(),DataSetName.Data(),DataSets[0].Data());
                    else nameOutput = Form("%s_%s/%s/PhotonQA/%s_PhotonQARunwise.root",nameCutsPQA.Data(),cutTreeProjection.Data(),fEnergyFlag.Data(),DataSetName.Data());
                    if(doTrigger && i<nData && i==0){
                        TString fTriggerCut = nameCutsPQAshort(3,2);
                        fTrigger = AnalyseSpecialTriggerCut(fTriggerCut.Atoi(), plotDataSets[i]);
                        if(fTrigger.Contains("not defined")) fTrigger = "";
                    }
                    // printf("%s\n", nameOutput);
                    TFile* fOutput = new TFile(nameOutput,"READ");
                    fOutput->cd();
                    hTMP = (TH1D*)(fOutput->Get(Form("%s_%s",vecHistosName.at(h).Data(), DataSets[i].Data())))->Clone(Form("%s_%s_%i",vecHistosName.at(h).Data(), DataSets[i].Data(),i));
                    if(!hTMP) {
                        printf("\n%s-%s\t\033[0;31mERROR\033[0m not found \n",  DataSets[i].Data(), vecHistosName.at(h).Data());
                        continue;
                    }

                    hTMP->SetDirectory(0);
                    vecHTMP.push_back(hTMP);
                    printf("%s, ",  DataSets[i].Data());

                    fOutput->Close();
                    delete fOutput;
                }


                for(Int_t i=0; i<nData; i++) {
                    for(Int_t j=0; j<nSets-nData; j++)  {
                        ratioSets[j] = new TString(Form("%s / %s", plotDataSets[i].Data(), plotDataSets[j+nData].Data()));
                        ratio[j] = new TH1D(Form("%s%i%i",((TH1D*) vecHTMP.at(i))->GetName(),i,j),
                                            Form("%s%i%i;%s;%s - Ratio: Data / MC",((TH1D*) vecHTMP.at(i))->GetTitle(),i,j,((TH1D*) vecHTMP.at(i))->GetXaxis()->GetTitle(),((TH1D*) vecHTMP.at(i))->GetYaxis()->GetTitle()),
                                            hNBin,hFBin,hLBin);
                        hMarkerStyle         = GetDefaultMarkerStyle(fEnergyFlag.Data(),DataSets[i].Data(),fCentralityFromCut);
                        hMarkerColor         = GetColorDefaultColor(fEnergyFlag.Data(),DataSets[i].Data(),fCentralityFromCut);
                        hLineColor           = GetColorDefaultColor(fEnergyFlag.Data(),DataSets[i].Data(),fCentralityFromCut);
                        EditTH1(globalRuns, doEquidistantXaxis, ratio[j], markerStyles[j % 14], hMarkerSize, hMarkerColor, hLineColor);
                    }

                    for(Int_t b=1; b<hNBin+1; b++) {
                        Double_t hData = ((TH1D*) vecHTMP.at(i))->GetBinContent(b);
                        for(Int_t j=0; j<nSets-nData; j++) {
                            Double_t hMC = ((TH1D*) vecHTMP.at(j+nData))->GetBinContent(b);
                            if(hMC!=0) {
                                if(hData/hMC>1.98) ratio[j]->SetBinContent(b,1.98);
                                else if(hData/hMC<0.02) ratio[j]->SetBinContent(b,0.02);
                                else ratio[j]->SetBinContent(b,hData/hMC);
                            }else ratio[j]->SetBinContent(b,1.98);
                        }
                    }

                    for(Int_t j=0; j<nSets-nData; j++) {
                        TString draw = (i==0&&j==0)?"p":"p, same";
                        ratio[j]->SetTitle("");
                        ratio[j]->GetYaxis()->SetRangeUser(0,2);
                        ratio[j]->Draw(draw.Data());
                        legend->AddEntry(ratio[j],ratioSets[j]->Data(),"p");
                    }

                    legend->Draw();
                    outputDirDataSet = Form("%s/TrendingRatios",outputDir.Data());
                    gSystem->Exec("mkdir -p "+outputDirDataSet);

                    if(doTrigger){
                        fTrigger = "";
                        TString fTriggerCut = nameCutsPQAshort(3,2);
                        fTrigger = AnalyseSpecialTriggerCut(fTriggerCut.Atoi(), plotDataSets[i]);
                        if(fTrigger.Contains("not defined")) fTrigger = "";

                        PutProcessLabelAndEnergyOnPlot(xPosLabel, 0.92, 0.03, fCollisionSystem.Data(), fTrigger.Data(), "");
                    }else PutProcessLabelAndEnergyOnPlot(xPosLabel, 0.92, 0.03, fCollisionSystem.Data(), "", "");

                    SaveCanvas(canvas, Form("%s/%s.%s", outputDirDataSet.Data(),Form("%s",((TH1D*) vecHTMP.at(i))->GetName()),suffix.Data()));
                    legend->Clear();
                    for(Int_t j=0; j<nSets-nData; j++) {
                        delete ratio[j];
                        delete ratioSets[j];
                    }
                }
            }
        }else cout << "...skipped due to nSets<=1 or nSets==nData!" << endl;
    }
    delete legend;
    delete canvas;

    delete[] vecMissingRuns;

    TH1::AddDirectory(kTRUE);

    cout << "Done with PhotonQA_Runwise" << endl;
    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;

    return;

}
//end
