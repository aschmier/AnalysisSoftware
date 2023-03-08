//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++  adapted from Gamma Conversion Group, PWGGA,                       +++++
//+++++     Daniel Muehlheim, d.muehlheim@cern.ch                          +++++
//+++++     Friederike Bock, fbock@cern.ch                                 +++++
//+++++  provided by Austin Schmier, a.schmier@cern.ch                     +++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include "QA.h"

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++ Main routine +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void JetQA_Runwise(
        Int_t nSetsIn,                                      // number of sets to be analysed
        Int_t nDataIn,                                      // number of real data sets to be analysed
        TString fEnergyFlag,                                // energy flag
        TString filePath,                                   // path to the data
        TString filePathMC,                                 // path to the MC production
        TString fileName,                                   // file name of the data
        TString fileNameMC,                                 // file name of the MC
        TString* DataSets,                                  // technical names of data sets for output
        TString* plotDataSets,                              // labels of data sets in plots
        Int_t triggerNr                 = -1,               // 0 = EJE, 1 = EMC7, 2 = INT7
        TString jetType                 = "",               // run for full, charged, or neutral jets
        Int_t doExtQA                   = 2,                // 0: switched off, 1: normal extQA, 2: with Cell level plots, 3: with mean value calculations
        Bool_t doEquidistantXaxis       = kFALSE,           // kTRUE: each run in runlist corresponds to 1 bin in X in histogram,
        // kFALSE: histograms contain the complete specified run number range, where each run represents a bin - even if it is not specified
        Bool_t doTrigger                = kTRUE,            // enables trigger analysis
        Bool_t doHistsForEverySet       = kTRUE,            // kTRUE: output done for each set separately as well
        // kFALSE: only full run range output is produced
        Bool_t addSubFolder             = kFALSE,           // kTRUE: adds another subfolder for QA output fo reach DataSet[i]
        // kFALSE: stores the runwise output all together
        Bool_t useDataRunListForMC      = kFALSE,           // kTRUE: use the same run list for data and MC
        // kFALSE: use specified
        Size_t markerSize               = 1,                // how large should the markers be?
        TString suffix                  = "eps",            // output format of plots
        TString folderRunlists          = "",               // path to the runlists
        Int_t *nSigmasBadRun            = NULL,             // array of 8 integers
        TString addLabelRunList         = "",               // additional name for runlist
        TString fixedTopDir             = ""
        )
{
    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << "JetQA_Runwise" << endl;
    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;

    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //+++++++++++++++++++++++++++* Setting general plotting style ++++++++++++++++++++++++++++++++++++++++++++++++++
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    gROOT->Reset();
    TH1::AddDirectory(kFALSE);
    StyleSettingsThesis();
    SetPlotStyle();

    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //++++++++++++++++++++++++++++++ Setting common variables ++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    const Int_t nSets   = nSetsIn;
    const Int_t nData   = nDataIn;
    TString fileNameData= fileName;

    TString fDate               = ReturnDateString();
    TString fTextMeasurement    = Form("Jets");
    TString fCentrality[30];
    for(Int_t i=0; i<nSets; i++) {
        if(fEnergyFlag.Contains("PbPb")){
            if(plotDataSets[i].Contains("0-10%")) fCentrality[i] = "0-10%";
            else if(plotDataSets[i].Contains("10-20%")) fCentrality[i] = "10-20%";
            else if(plotDataSets[i].Contains("20-50%")) fCentrality[i] = "20-50%";
            else if(plotDataSets[i].Contains("50-90%")) fCentrality[i] = "50-90%";
            else if(plotDataSets[i].Contains("0-90%")) fCentrality[i] = "0-90%";
            else fCentrality[i] = "";
        } else {
            fCentrality[i] = "";
        }
    }
    TString fCollisionSystem    = ReturnFullCollisionsSystem(fEnergyFlag);
    if (fCollisionSystem.CompareTo("") == 0){
        cout << "No correct collision system specification, has been given" << endl;
        return;
    }

    // Trigger name handling
    TString fTrigger;
    if(triggerNr == 0) fTrigger    = "EJE";
    else if(triggerNr == 1) fTrigger    = "EMC7";
    else if(triggerNr == 2) fTrigger    = "INT7";
    else{cout << "Invalid (or no) trigger specified. Returning." << endl; return;}

    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //++++++++++++++++++++++++++++++ Define plotting settings ++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    std::vector<TString> vecDataSet;
    Style_t hMarkerStyle[nSets];
    Size_t hMarkerSize[nSets];
    Color_t hMarkerColor[nSets];
    Color_t hLineColor[nSets];

    for(Int_t i=0; i<nSets; i++){
        vecDataSet.push_back(DataSets[i].Data());
        if (i < nDataIn){
            hMarkerStyle[i]         = GetDefaultMarkerStyle(fEnergyFlag.Data(),DataSets[i].Data(),"");
            hMarkerColor[i]         = GetColorDefaultColor(fEnergyFlag.Data(),DataSets[i].Data(),"");
            hLineColor[i]           = GetColorDefaultColor(fEnergyFlag.Data(),DataSets[i].Data(),"");
            hMarkerSize[i]          = markerSize;
        } else {
            hMarkerStyle[i]         = GetDefaultMarkerStyle(fEnergyFlag.Data(),plotDataSets[i].Data(),"");
            hMarkerColor[i]         = GetColorDefaultColor(fEnergyFlag.Data(),plotDataSets[i].Data(),"");
            hLineColor[i]           = GetColorDefaultColor(fEnergyFlag.Data(),plotDataSets[i].Data(),"");
            hMarkerSize[i]          = markerSize;
        }
    }

    Float_t xPosLabel = 0.99;
    Bool_t drawVerticalLines = kFALSE;
    Int_t nLines;        // number of vertical lines
    Int_t runRanges[10]; // array of bin numbers where to draw vertical lines
    TLine* verticalLines[10] = {NULL};
    if (fEnergyFlag.CompareTo("8TeV_pp") == 0)
        xPosLabel = 0.9;
        nLines = 0;
    if (fEnergyFlag.CompareTo("pPb_5.023TeV") == 0)
        xPosLabel = 0.75;
    if(fEnergyFlag.Contains("PbPb")){
        xPosLabel = 0.75;
        if (fEnergyFlag.CompareTo("PbPb_5.02TeV") == 0){
            drawVerticalLines = kTRUE;
            nLines = 4;
            runRanges[0] = 5; runRanges[1] = 37; runRanges[2] = 70; runRanges[3] = 72;
        }
    }
    if (fEnergyFlag.CompareTo("5TeV") == 0){
        xPosLabel = 0.75;
        drawVerticalLines = kTRUE;
        nLines = 7;
        runRanges[0] = 6; runRanges[1] = 8; runRanges[2] = 11; runRanges[3] = 19; runRanges[4] = 28; runRanges[5] = 38; runRanges[6] = 42;
    }
    if (nLines > 10) cout << "ERROR: nLines cannot be larger than 10. Increase size of runRanges[10] and verticalLines[10]" << endl;

    // Bins for histogram creation loops
    const Int_t nPtBins     = 7;
    Int_t binsPt[8]        = {6, 10, 20, 30, 60, 100, 200, 350};
    Double_t binsPtdoub[8]        = {6., 10., 20., 30., 60., 100., 200., 350.};

    const Int_t nEtaBins    = 3;
    Double_t binsEta[4]    = {-0.9, -0.4, 0.4, 0.9};
    Int_t binsEtaInt[4]    = {6, 31, 71, 96};

    const Int_t nEBins      = 4;
    Int_t binsE[5]         = {1, 2, 5, 10, 30};
    Double_t binsEdoub[5]         = {1., 2., 5., 10., 30.};

    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // runNumbers
    std::vector<TString> vecRuns;
    TString fileRuns[nSets];
    // bad QA runs
    std::vector<TString> vecRunsBad;
    std::vector<TString> vecRunsBadVertexZ;
    std::vector<TString> vecRunsBadAllClusterEnergy;
    std::vector<TString> vecRunsBadNch;
    std::vector<TString> vecRunsBadPtCh;
    std::vector<TString> vecRunsBadCentrality;
    std::vector<TString> vecRunsBadJets;
    std::vector<TString> vecNoNeutral;
    std::vector<TString> vecNoCharged;
    std::vector<TString> vecNEFQA;
    TString fileRunsBad[nSets];
    TString fileRunsBadVertexZ[nSets];
    TString fileRunsBadAllClusterEnergy[nSets];
    TString fileRunsBadNch[nSets];
    TString fileRunsBadPtCh[nSets];
    TString fileRunsBadCentrality[nSets];
    TString fileRunsBadJets[nSets];
    TString fileNoNeutral;
    TString fileNoCharged;
    TString fileNEFQA;

    for(Int_t i=0; i<nSets; i++){
        if(i >= nData) fileRuns[i]                 = Form("%s/runNumbers%s%s.txt", folderRunlists.Data(), (vecDataSet.at(i)).Data(), addLabelRunList.Data());
        else fileRuns[i]                 = Form("%s/runNumbers%s_%s%s.txt", folderRunlists.Data(), (vecDataSet.at(i)).Data(), fTrigger.Data(), addLabelRunList.Data());
        cout << "================= Runlist file: " << fileRuns[i] << " ==================" << endl;
        fileRunsBad[i]              = Form("%s/runNumbers%s_%s%sBadQA.txt", folderRunlists.Data(), (vecDataSet.at(i)).Data(), fTrigger.Data(), addLabelRunList.Data());
        fileRunsBadVertexZ[i]       = Form("%s/runNumbers%s_%s%sBadQA_VTX.txt", folderRunlists.Data(), (vecDataSet.at(i)).Data(), fTrigger.Data(), addLabelRunList.Data());
        fileRunsBadAllClusterEnergy[i] = Form("%s/runNumbers%s%s_%sBadQA_Cluster.txt", folderRunlists.Data(), (vecDataSet.at(i)).Data(), fTrigger.Data(), addLabelRunList.Data());
        fileRunsBadNch[i]           = Form("%s/runNumbers%s_%s%sBadQA_Nch.txt", folderRunlists.Data(), (vecDataSet.at(i)).Data(), fTrigger.Data(), addLabelRunList.Data());
        fileRunsBadPtCh[i]          = Form("%s/runNumbers%s_%s%sBadQA_PtCh.txt", folderRunlists.Data(), (vecDataSet.at(i)).Data(), fTrigger.Data(), addLabelRunList.Data());
        fileRunsBadCentrality[i]    = Form("%s/runNumbers%s_%s%sBadQA_Centrality.txt", folderRunlists.Data(), (vecDataSet.at(i)).Data(), fTrigger.Data(), addLabelRunList.Data());
        fileRunsBadJets[i]          = Form("%s/runNumbers%s_%s%sBadQA_Jets.txt", folderRunlists.Data(), (vecDataSet.at(i)).Data(), fTrigger.Data(), addLabelRunList.Data());

        if(useDataRunListForMC) {
            if(nDataIn == 1){
                fileRuns[i] = Form("%s/runNumbers%s_%s%s.txt", folderRunlists.Data(), vecDataSet.at(0).Data(), fTrigger.Data(), addLabelRunList.Data());
                cout << "Switch useDataRunListForMC is true, reading runs from: " << fileRuns[i].Data() << endl;
            }else{
                cout << "Switch usdDataRunListForMC is true, but nDataIn != 1. Turning switch off." << endl;
                useDataRunListForMC = kFALSE;
            }
        }
        cout << endl << "trying to read: " << fileRuns[i].Data() << endl;
        if(!readin(fileRuns[i], vecRuns, kFALSE)) {cout << "ERROR, no Run Numbers could be found in "<<fileRuns[i]<<"! Returning..." << endl; return;}
        else cout << "Successfully read runlist file" << endl << endl;
    }

    fileNoNeutral            = Form("/home/austin/alice/runQA/runNumbers_%s%sBadQA_NoNeutral.txt", fTrigger.Data(), addLabelRunList.Data());
    fileNoCharged            = Form("/home/austin/alice/runQA/runNumbers_%s%sBadQA_NoCharged.txt", fTrigger.Data(), addLabelRunList.Data());
    fileNEFQA                = Form("/home/austin/alice/runQA/runNumbers_%s%sBadQA_NEFQA.txt", fTrigger.Data(), addLabelRunList.Data());

    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //++++++++++++++++++++++++++++++ Determine which Trigger to process +++++++++++++++++++++++++++++++++++++++++++++++
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    UInt_t ActualRunIndexInVector=0;
    TFile* fInputFile             = NULL;
    TKey *key = NULL;
    vector <TString> Triggers;

    while ( (fInputFile==NULL)&&(ActualRunIndexInVector<vecRuns.size()) ){
        fInputFile             = new TFile(Form("%s/%s/%s/%s", filePath.Data(), ((TString)vecDataSet.at(0)).Data(), ((TString)vecRuns.at(ActualRunIndexInVector)).Data(), fileName.Data()));
        cout << "Input file: " << fInputFile->GetName() << endl;
        if(fInputFile->IsZombie()) {
            cout << "ERROR: ROOT file '" << Form("%s/%s/%s/%s", filePath.Data(), ((TString)vecDataSet.at(0)).Data(), ((TString)vecRuns.at(ActualRunIndexInVector)).Data(), fileName.Data()) << "' could not be openend, return!" << endl;
            fInputFile->Close();
            delete fInputFile;
            fInputFile=NULL;
        }
        ActualRunIndexInVector++;
    }
    if(fInputFile==NULL) {
        cout << "ERROR: no ROOT file; last tried file: '" << Form("%s/%s/%s/%s", filePath.Data(), ((TString)vecDataSet.at(0)).Data(), ((TString)vecRuns.at(ActualRunIndexInVector-1)).Data(), fileName.Data()) << "'; return!" << endl;
        return;
    }

    TIter next(fInputFile->GetListOfKeys());
    while ((key=(TKey*)next())){
        cout << Form("Found Directory: '%s' ",key->GetName())<< endl;
        TString nameTrigger             = key->GetName();
        if(nameTrigger.BeginsWith("JetSpectrum_") && nameTrigger.Contains("R02")) Triggers.push_back(nameTrigger);
    }

    cout << endl;
    cout << "The following trigger directories can be used for QA:" << endl;
    for(Int_t i = 0; i < (Int_t) Triggers.size(); i++) {cout << Form("(%i) -- %s", i, Triggers.at(i).Data()) << endl;}
    cout << endl;
    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;

    fInputFile->Close();
    delete fInputFile;
    fInputFile = NULL;
    delete key;
    key = NULL;

    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << "Processing Directory Number: " << triggerNr << endl;
    cout << Triggers.at(triggerNr) << endl;
    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << "Pictures are saved as " << suffix.Data() << "!" << endl;
    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;

    TString fTriggerSelection            = Triggers.at(triggerNr);
    TString fEventTriggerSelection       = "";

    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //++++++++++++++++++++++++++ Define output directories+++++++++++++++++++++++++++++++++++++++++++++++++
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    TString outputDir           = Form("%s/%s/JetQA/%s/Runwise",fTriggerSelection.Data(),fEnergyFlag.Data(),suffix.Data());
    if (fEnergyFlag.Contains("13TeV")) {
        outputDir       = Form("%s/%s/JetQA/%s/%s/Runwise", fTriggerSelection.Data(),fEnergyFlag.Data(),suffix.Data(), DataSets[0].Data());
    }
    if(nDataIn == 1){
        if(nSetsIn == 2) outputDir   += "/MC_Comparison/Ratios";
        else outputDir               += "/MC_Comparison/MultiMC_Comparison";
    }
    if(addSubFolder){
        outputDir               += "/";
        outputDir               += DataSets[0];
    }
    gSystem->Exec("mkdir -p "+outputDir);


    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //+++++++++++++++++++++++++++* Determine global run list +++++++++++++++++++++++++++++++++++++++++++++*
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    std::vector<TString> globalRuns;
    Float_t rangesRuns[nSets][2];

    for(Int_t i=0; i<nSets; i++) {
        vecRuns.clear();
        if(!readin(fileRuns[i], vecRuns, kFALSE)) {cout << "ERROR, no Run Numbers could be found! Returning..." << endl; return;}

        for(Int_t j=0; j<(Int_t) vecRuns.size();j++){
            if( i==0 ) globalRuns.push_back(vecRuns.at(j));
            else {
                Bool_t bFound = kFALSE;
                for(Int_t k=0; k<(Int_t) globalRuns.size();k++){ if(globalRuns.at(k)==vecRuns.at(j)) bFound=kTRUE;}
                if(!bFound) globalRuns.push_back(vecRuns.at(j));
            }
        }

        if( !doEquidistantXaxis && ((Int_t) vecRuns.size())>0 ){
            if(nSets>2){
                rangesRuns[i][0]=((TString)vecRuns.front()).Atof() - 500.;
                rangesRuns[i][1]=((TString)vecRuns.back()).Atof() + 500.;
            }else{
                rangesRuns[i][0]=((TString)vecRuns.front()).Atof() - 25.;
                rangesRuns[i][1]=((TString)vecRuns.back()).Atof() + 25.;
            }
        }else{ rangesRuns[i][0]=0; rangesRuns[i][1]=0; }
    }

    selection_sort(globalRuns.begin(), globalRuns.end());

    map<TString,Int_t> mapBin;

    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //+++++++++++++++++++++ Create histograms for plotting ++++++++++++++++++++++++++++++++++++++++++++++++
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << "Processing following list of " << globalRuns.size() << " Runs:";
    for(Int_t i=0; i<(Int_t) globalRuns.size(); i++) {
        mapBin[globalRuns.at(i)]=i+1;
        if(i%10==0) cout << endl;
        cout << globalRuns.at(i) << ", ";
    }
    cout << endl;
    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;

    TH1D* hNEvents[nSets];
    TH1D* hVertexZMean[nSets];
    TH1D* hVertexZRMS[nSets];
    TH1D* hCentralityMean[nSets];
    TH1D* hCentralityMin[nSets];
    TH1D* hCentralityMax[nSets];
    TH1D* hEventPlaneAngleMean[nSets];
    TH1D* hJetPt_Mean[nSets];
    TH1D* hJetPt_RMS[nSets];
    TH1D* hJetPtMax_Mean[nSets];
    TH1D* hJetPtMax_RMS[nSets];
    TH1D* hNEF_Mean[nSets][nPtBins];
    TH1D* hNEF_RMS[nSets][nPtBins];
    TH1D* hNch_Mean[nSets][nPtBins];
    TH1D* hNch_RMS[nSets][nPtBins];
    TH1D* hMeanNch[nSets];
    TH1D* hNne_Mean[nSets][nPtBins];
    TH1D* hNne_RMS[nSets][nPtBins];
    TH1D* hPtCh_Mean[nSets][nPtBins];
    TH1D* hPtCh_RMS[nSets][nPtBins];
    TH1D* hMeanPtCh[nSets];
    TH1D* hPtNe_Mean[nSets][nPtBins];
    TH1D* hPtNe_RMS[nSets][nPtBins];
    TH1D* hPtChMax_Mean[nSets][nPtBins];
    TH1D* hPtChMax_RMS[nSets][nPtBins];
    TH1D* hPtNeMax_Mean[nSets][nPtBins];
    TH1D* hPtNeMax_RMS[nSets][nPtBins];
    TH1D* hEtaPhi_Mean[nSets][nEtaBins];
    TH1D* hEtaPhi_RMS[nSets][nEtaBins];
    TH1D* hEtaPhiCh_Mean[nSets][nEtaBins];
    TH1D* hEtaPhiCh_RMS[nSets][nEtaBins];
    TH1D* hEtaPhiNe_Mean[nSets][nEtaBins];
    TH1D* hEtaPhiNe_RMS[nSets][nEtaBins];
    TH1D* hEtaPhiChMax_Mean[nSets][nEtaBins];
    TH1D* hEtaPhiChMax_RMS[nSets][nEtaBins];
    TH1D* hEtaPhiNeMax_Mean[nSets][nEtaBins];
    TH1D* hEtaPhiNeMax_RMS[nSets][nEtaBins];
    TH1D* hJetClusterEnergy_Mean[nSets];
    TH1D* hJetClusterEnergy_RMS[nSets];
    TH1D* hClusterTime_Mean[nSets];
    TH1D* hClusterTime_RMS[nSets];
    TH1D* hClusterCells_Mean[nSets][nEBins];
    TH1D* hClusterCells_RMS[nSets][nEBins];
    TH1D* hAllClusterEnergy_Mean[nSets];
    TH1D* hAllClusterEnergy_RMS[nSets];

    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //++++++++++++++++++++++++++++++ Vectors for Histograms +++++++++++++++++++++++++++++++++++++++++++++++
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    std::vector<TH1D*>* vecVertexZ                = new std::vector<TH1D*>[nSets];
    std::vector<TH1D*>* vecCent                   = new std::vector<TH1D*>[nSets];
    std::vector<TH1D*>* vecJetPt                  = new std::vector<TH1D*>[nSets];
    std::vector<TH1D*>* vecJetPtMax               = new std::vector<TH1D*>[nSets];
    std::vector<TH1D*>* vecNchAll                 = new std::vector<TH1D*>[nSets];
    std::vector<TH1D*>* vecPtChAll                 = new std::vector<TH1D*>[nSets];
    std::vector<vector<TH1D*>>* vecNEF            = new std::vector<vector<TH1D*>>[nSets];
    std::vector<vector<TH1D*>>* vecNch            = new std::vector<vector<TH1D*>>[nSets];
    std::vector<vector<TH1D*>>* vecNne            = new std::vector<vector<TH1D*>>[nSets];
    std::vector<vector<TH1D*>>* vecPtCh           = new std::vector<vector<TH1D*>>[nSets];
    std::vector<vector<TH1D*>>* vecPtNe           = new std::vector<vector<TH1D*>>[nSets];
    std::vector<vector<TH1D*>>* vecPtChMax        = new std::vector<vector<TH1D*>>[nSets];
    std::vector<vector<TH1D*>>* vecPtNeMax        = new std::vector<vector<TH1D*>>[nSets];
    std::vector<vector<TH1D*>>* vecEtaPhi         = new std::vector<vector<TH1D*>>[nSets];
    std::vector<vector<TH1D*>>* vecEtaPhiCh       = new std::vector<vector<TH1D*>>[nSets];
    std::vector<vector<TH1D*>>* vecEtaPhiNe       = new std::vector<vector<TH1D*>>[nSets];
    std::vector<vector<TH1D*>>* vecEtaPhiChMax    = new std::vector<vector<TH1D*>>[nSets];
    std::vector<vector<TH1D*>>* vecEtaPhiNeMax    = new std::vector<vector<TH1D*>>[nSets];
    std::vector<TH1D*>* vecJetClusterEnergy       = new std::vector<TH1D*>[nSets];
    std::vector<TH1D*>* vecClusterTime            = new std::vector<TH1D*>[nSets];
    std::vector<vector<TH1D*>>* vecClusterCells   = new std::vector<vector<TH1D*>>[nSets];
    std::vector<TH1D*>* vecAllClusterEnergy       = new std::vector<TH1D*>[nSets];

    std::vector<TH1D*>* vecVertexZRatio              = new std::vector<TH1D*>[nSets-1];
    std::map<Int_t,Int_t> mapVertexRatio;
    std::vector<TH1D*>* vecCentRatio                 = new std::vector<TH1D*>[nSets-1];
    std::map<Int_t,Int_t> mapCentRatio;
    std::vector<TH1D*>* vecJetPtRatio                = new std::vector<TH1D*>[nSets-1];
    std::map<Int_t,Int_t>mapJetPtRatio;
    std::vector<TH1D*>* vecJetPtMaxRatio             = new std::vector<TH1D*>[nSets-1];
    std::map<Int_t,Int_t>mapJetPtMaxRatio;
    std::vector<TH1D*>* vecNchAllRatio               = new std::vector<TH1D*>[nSets-1];
    std::map<Int_t,Int_t>mapPtChAllRatio;
    std::vector<TH1D*>* vecPtChAllRatio               = new std::vector<TH1D*>[nSets-1];
    std::map<Int_t,Int_t>mapNchAllRatio;
    std::vector<vector<TH1D*>>* vecNEFRatio          = new std::vector<vector<TH1D*>>[nSets-1];
    std::map<Int_t,Int_t>mapNEFRatio;
    std::vector<vector<TH1D*>>* vecNchRatio          = new std::vector<vector<TH1D*>>[nSets-1];
    std::map<Int_t,Int_t>mapNchRatio;
    std::vector<vector<TH1D*>>* vecNneRatio          = new std::vector<vector<TH1D*>>[nSets-1];
    std::map<Int_t,Int_t>mapNneRatio;
    std::vector<vector<TH1D*>>* vecPtChRatio         = new std::vector<vector<TH1D*>>[nSets-1];
    std::map<Int_t,Int_t>mapPtChRatio;
    std::vector<vector<TH1D*>>* vecPtNeRatio         = new std::vector<vector<TH1D*>>[nSets-1];
    std::map<Int_t,Int_t>mapPtNeRatio;
    std::vector<vector<TH1D*>>* vecPtChMaxRatio      = new std::vector<vector<TH1D*>>[nSets-1];
    std::map<Int_t,Int_t>mapPtChMaxRatio;
    std::vector<vector<TH1D*>>* vecPtNeMaxRatio      = new std::vector<vector<TH1D*>>[nSets-1];
    std::map<Int_t,Int_t>mapPtNeMaxRatio;
    std::vector<vector<TH1D*>>* vecEtaPhiRatio       = new std::vector<vector<TH1D*>>[nSets-1];
    std::map<Int_t,Int_t>mapEtaPhiRatio;
    std::vector<vector<TH1D*>>* vecEtaPhiChRatio     = new std::vector<vector<TH1D*>>[nSets-1];
    std::map<Int_t,Int_t>mapEtaPhiChRatio;
    std::vector<vector<TH1D*>>* vecEtaPhiNeRatio     = new std::vector<vector<TH1D*>>[nSets-1];
    std::map<Int_t,Int_t>mapEtaPhiNeRatio;
    std::vector<vector<TH1D*>>* vecEtaPhiChMaxRatio  = new std::vector<vector<TH1D*>>[nSets-1];
    std::map<Int_t,Int_t>mapEtaPhiChMaxRatio;
    std::vector<vector<TH1D*>>* vecEtaPhiNeMaxRatio  = new std::vector<vector<TH1D*>>[nSets-1];
    std::map<Int_t,Int_t>mapEtaPhiNeMaxRatio;
    std::vector<TH1D*>* vecJetClusterEnergyRatio        = new std::vector<TH1D*>[nSets-1];
    std::map<Int_t,Int_t>mapJetClusterEnergyRatio;
    std::vector<TH1D*>* vecClusterTimeRatio          = new std::vector<TH1D*>[nSets-1];
    std::map<Int_t,Int_t>mapClusterTimeRatio;
    std::vector<vector<TH1D*>>* vecClusterCellsRatio = new std::vector<vector<TH1D*>>[nSets-1];
    std::map<Int_t,Int_t>mapClusterCellsRatio;
    std::vector<TH1D*>* vecAllClusterEnergyRatio      = new std::vector<TH1D*>[nSets-1];
    std::map<Int_t,Int_t>mapAllClusterEnergyRatio;

    std::vector<TH1D*>* vecHistosTH1D       = new std::vector<TH1D*>[nSets];
    std::vector<TH1F*>* vecHistosTH1F       = new std::vector<TH1F*>[nSets];
    std::vector<TString> vecHistosNameTH1D;
    std::vector<TString> vecHistosNameTH1F;
    std::vector<TString> vecBinRangeTH1D;
    std::vector<TString> vecBinRangeTH1F;
    TString histoName;
    TString binRange;


    Int_t hFBin;
    Int_t hLBin;
    Int_t hNBin;

    if(doEquidistantXaxis)    {
        hFBin       = 0;
        hLBin       = globalRuns.size();
        hNBin       = globalRuns.size();
    } else {
        if(nSets>2){
            hFBin   = ((TString) globalRuns.at(0)).Atoi() - 500;
            hLBin   = ((TString) globalRuns.at(globalRuns.size()-1)).Atoi()  + 500;
            hNBin   = hLBin - hFBin;
        }else{
            hFBin   = ((TString) globalRuns.at(0)).Atoi() - 25;
            hLBin   = ((TString) globalRuns.at(globalRuns.size()-1)).Atoi()  + 25;
            hNBin   = hLBin - hFBin;
        }
    }

    // Configure histograms
    for(Int_t i=0; i<nSets; i++){
        histoName               = "hNEvents";
        binRange                = "";
        if(i==0){
          vecBinRangeTH1D.push_back(binRange);
          vecHistosNameTH1D.push_back(histoName);
        }
        hNEvents[i]                 = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),"hNEvents; Run Number ; N^{Evt}",hNBin,hFBin,hLBin);
        EditTH1(globalRuns, doEquidistantXaxis, hNEvents[i], hMarkerStyle[i], hMarkerSize[i], hMarkerColor[i], hLineColor[i]);
        vecHistosTH1D[i].push_back(hNEvents[i]);

        histoName                   = "hVertexZ-Mean";
        binRange                = "";
        if(i==0){
          vecBinRangeTH1D.push_back(binRange);
          vecHistosNameTH1D.push_back(histoName);
        }
        hVertexZMean[i]             = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),"hVertexZ-Mean; Run Number ; <z_{Vertex}> (cm)",hNBin,hFBin,hLBin);
        EditTH1(globalRuns, doEquidistantXaxis, hVertexZMean[i], hMarkerStyle[i], hMarkerSize[i], hMarkerColor[i], hLineColor[i]);
        vecHistosTH1D[i].push_back(hVertexZMean[i]);

        histoName                   = "hVertexZ-RMS";
        binRange                = "";
        if(i==0){
          vecBinRangeTH1D.push_back(binRange);
          vecHistosNameTH1D.push_back(histoName);
        }
        hVertexZRMS[i]              = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),"hVertexZ-RMS; Run Number ; #sigma(z_{vertex}) (cm)",hNBin,hFBin,hLBin);
        EditTH1(globalRuns, doEquidistantXaxis, hVertexZRMS[i], hMarkerStyle[i], hMarkerSize[i], hMarkerColor[i], hLineColor[i]);
        vecHistosTH1D[i].push_back(hVertexZRMS[i]);

        histoName                   = "hJetPt-Mean";
        binRange                = "";
        if(i==0){
          vecBinRangeTH1D.push_back(binRange);
          vecHistosNameTH1D.push_back(histoName);
        }
        hJetPt_Mean[i]             = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),"hJetPt_Mean; Run Number ; <p_{T}^{jet}> (GeV)",hNBin,hFBin,hLBin);
        EditTH1(globalRuns, doEquidistantXaxis, hJetPt_Mean[i], hMarkerStyle[i], hMarkerSize[i], hMarkerColor[i], hLineColor[i]);
        vecHistosTH1D[i].push_back(hJetPt_Mean[i]);

        histoName                   = "hJetPt-RMS";
        binRange                = "";
        if(i==0){
          vecBinRangeTH1D.push_back(binRange);
          vecHistosNameTH1D.push_back(histoName);
        }
        hJetPt_RMS[i]             = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),"hJetPt_RMS; Run Number ; #sigma(p_{T}^{jet}) (GeV)",hNBin,hFBin,hLBin);
        EditTH1(globalRuns, doEquidistantXaxis, hJetPt_RMS[i], hMarkerStyle[i], hMarkerSize[i], hMarkerColor[i], hLineColor[i]);
        vecHistosTH1D[i].push_back(hJetPt_RMS[i]);

        histoName                   = "hJetPtMax-Mean";
        binRange                = "";
        if(i==0){
          vecBinRangeTH1D.push_back(binRange);
          vecHistosNameTH1D.push_back(histoName);
        }
        hJetPtMax_Mean[i]             = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),"hJetPtMax_Mean; Run Number ; <p_{T}^{jet,max}> (GeV)",hNBin,hFBin,hLBin);
        EditTH1(globalRuns, doEquidistantXaxis, hJetPtMax_Mean[i], hMarkerStyle[i], hMarkerSize[i], hMarkerColor[i], hLineColor[i]);
        vecHistosTH1D[i].push_back(hJetPtMax_Mean[i]);

        histoName                   = "hJetPtMax-RMS";
        binRange                = "";
        if(i==0){
          vecBinRangeTH1D.push_back(binRange);
          vecHistosNameTH1D.push_back(histoName);
        }
        hJetPtMax_RMS[i]             = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),"hJetPtMax_RMS; Run Number ; #sigma(p_{T}^{jet}) (GeV)",hNBin,hFBin,hLBin);
        EditTH1(globalRuns, doEquidistantXaxis, hJetPtMax_RMS[i], hMarkerStyle[i], hMarkerSize[i], hMarkerColor[i], hLineColor[i]);
        vecHistosTH1D[i].push_back(hJetPtMax_RMS[i]);

        histoName                   = "hClusterTime-Mean";
        binRange                = "";
        if(i==0){
          vecBinRangeTH1D.push_back(binRange);
          vecHistosNameTH1D.push_back(histoName);
        }
        hClusterTime_Mean[i]             = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),"hClusterTime_Mean; Run Number ; <t^{cluster}> (ns)",hNBin,hFBin,hLBin);
        EditTH1(globalRuns, doEquidistantXaxis, hClusterTime_Mean[i], hMarkerStyle[i], hMarkerSize[i], hMarkerColor[i], hLineColor[i]);
        vecHistosTH1D[i].push_back(hClusterTime_Mean[i]);

        histoName                   = "hClusterTime-RMS";
        binRange                = "";
        if(i==0){
          vecBinRangeTH1D.push_back(binRange);
          vecHistosNameTH1D.push_back(histoName);
        }
        hClusterTime_RMS[i]             = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),"hClusterTime_RMS; Run Number ; #sigma(t^{cluster}) (ns)",hNBin,hFBin,hLBin);
        EditTH1(globalRuns, doEquidistantXaxis, hClusterTime_RMS[i], hMarkerStyle[i], hMarkerSize[i], hMarkerColor[i], hLineColor[i]);
        vecHistosTH1D[i].push_back(hClusterTime_RMS[i]);

        histoName                   = "hJetClusterEnergy-Mean";
        binRange                = "";
        if(i==0){
          vecBinRangeTH1D.push_back(binRange);
          vecHistosNameTH1D.push_back(histoName);
        }
        hJetClusterEnergy_Mean[i]             = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),"hJetClusterEnergy_Mean; Run Number ; <E^{cluster}> (GeV)",hNBin,hFBin,hLBin);
        EditTH1(globalRuns, doEquidistantXaxis, hJetClusterEnergy_Mean[i], hMarkerStyle[i], hMarkerSize[i], hMarkerColor[i], hLineColor[i]);
        vecHistosTH1D[i].push_back(hJetClusterEnergy_Mean[i]);

        histoName                   = "hJetClusterEnergy-RMS";
        binRange                = "";
        if(i==0){
          vecBinRangeTH1D.push_back(binRange);
          vecHistosNameTH1D.push_back(histoName);
        }
        hJetClusterEnergy_RMS[i]             = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),"hJetClusterEnergy_RMS; Run Number ; #sigma(E^{cluster}) (GeV)",hNBin,hFBin,hLBin);
        EditTH1(globalRuns, doEquidistantXaxis, hJetClusterEnergy_RMS[i], hMarkerStyle[i], hMarkerSize[i], hMarkerColor[i], hLineColor[i]);
        vecHistosTH1D[i].push_back(hJetClusterEnergy_RMS[i]);

        histoName                   = "hAllClusterEnergy-Mean";
        binRange                = "";
        if(i==0){
          vecBinRangeTH1D.push_back(binRange);
          vecHistosNameTH1D.push_back(histoName);
        }
        hAllClusterEnergy_Mean[i]             = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),"hAllClusterEnergy_Mean; Run Number ; <E^{cluster}> (GeV)",hNBin,hFBin,hLBin);
        EditTH1(globalRuns, doEquidistantXaxis, hAllClusterEnergy_Mean[i], hMarkerStyle[i], hMarkerSize[i], hMarkerColor[i], hLineColor[i]);
        vecHistosTH1D[i].push_back(hAllClusterEnergy_Mean[i]);

        histoName                   = "hAllClusterEnergy-RMS";
        binRange                = "";
        if(i==0){
          vecBinRangeTH1D.push_back(binRange);
          vecHistosNameTH1D.push_back(histoName);
        }
        hAllClusterEnergy_RMS[i]             = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),"hAllClusterEnergy_RMS; Run Number ; #sigma(E^{cluster}) (GeV)",hNBin,hFBin,hLBin);
        EditTH1(globalRuns, doEquidistantXaxis, hAllClusterEnergy_RMS[i], hMarkerStyle[i], hMarkerSize[i], hMarkerColor[i], hLineColor[i]);
        vecHistosTH1D[i].push_back(hAllClusterEnergy_RMS[i]);


        if(jetType == "Full Jets"){
            histoName                   = "hMeanNch";
            binRange                = "";
            if(i==0){
              vecBinRangeTH1D.push_back(binRange);
              vecHistosNameTH1D.push_back(histoName);
            }
            hMeanNch[i]             = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),"hMeanNch; Run Number ; <N_{Ch}^{Const}>",hNBin,hFBin,hLBin);
            EditTH1(globalRuns, doEquidistantXaxis, hMeanNch[i], hMarkerStyle[i], hMarkerSize[i], hMarkerColor[i], hLineColor[i]);
            vecHistosTH1D[i].push_back(hMeanNch[i]);

            histoName                   = "hMeanPtCh";
            binRange                = "";
            if(i==0){
              vecBinRangeTH1D.push_back(binRange);
              vecHistosNameTH1D.push_back(histoName);
            }
            hMeanPtCh[i]            = new TH1D(Form("%s_%s", "hMeanPtCh", DataSets[i].Data()),"Mean p_{T} of Charged Tracks; Run Number ; <p_{T,Ch}^{Const}> (GeV)",hNBin,hFBin,hLBin);
            EditTH1(globalRuns, doEquidistantXaxis, hMeanPtCh[i], hMarkerStyle[i], hMarkerSize[i], hMarkerColor[i], hLineColor[i]);
            vecHistosTH1D[i].push_back(hMeanPtCh[i]);

            for (Int_t binPt = 0; binPt < nPtBins; binPt++){
                histoName                   = Form("hNEF_%ito%iGeV-Mean", binsPt[binPt], binsPt[binPt+1]);
                binRange                = Form("%iGeV < p_{T}^{jet} < %iGeV",binsPt[binPt],binsPt[binPt+1]);
                if(i==0){
                    vecBinRangeTH1D.push_back(binRange);
                    vecHistosNameTH1D.push_back(histoName);
                }
                hNEF_Mean[i][binPt]             = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),Form("hNEF_%ito%iGeVMean; Run Number ; <NEF>", binsPt[binPt], binsPt[binPt+1] ),hNBin,hFBin,hLBin);
                EditTH1(globalRuns, doEquidistantXaxis, hNEF_Mean[i][binPt], hMarkerStyle[i], hMarkerSize[i], hMarkerColor[i], hLineColor[i]);
                vecHistosTH1D[i].push_back(hNEF_Mean[i][binPt]);


                histoName                   = Form("hNEF_%ito%iGeV-RMS", binsPt[binPt], binsPt[binPt+1]);
                binRange                = Form("%iGeV < p_{T}^{jet} < %iGeV",binsPt[binPt],binsPt[binPt+1]);
                if(i==0){
                    vecBinRangeTH1D.push_back(binRange);
                    vecHistosNameTH1D.push_back(histoName);
                }
                hNEF_RMS[i][binPt]             = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),Form("hNEF_%ito%iGeVRMS; Run Number ; #sigma(NEF)", binsPt[binPt], binsPt[binPt+1]),hNBin,hFBin,hLBin);
                EditTH1(globalRuns, doEquidistantXaxis, hNEF_RMS[i][binPt], hMarkerStyle[i], hMarkerSize[i], hMarkerColor[i], hLineColor[i]);
                vecHistosTH1D[i].push_back(hNEF_RMS[i][binPt]);

                histoName                   = Form("hNch_%ito%iGeV-Mean", binsPt[binPt], binsPt[binPt+1]);
                binRange                = Form("%iGeV < p_{T}^{jet} < %iGeV",binsPt[binPt],binsPt[binPt+1]);
                if(i==0){
                    vecBinRangeTH1D.push_back(binRange);
                    vecHistosNameTH1D.push_back(histoName);
                }
                hNch_Mean[i][binPt]             = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),Form("hNch_%ito%iGeVMean; Run Number ; <N_{Ch}^{Const}>", binsPt[binPt], binsPt[binPt+1] ),hNBin,hFBin,hLBin);
                EditTH1(globalRuns, doEquidistantXaxis, hNch_Mean[i][binPt], hMarkerStyle[i], hMarkerSize[i], hMarkerColor[i], hLineColor[i]);
                vecHistosTH1D[i].push_back(hNch_Mean[i][binPt]);

                histoName                   = Form("hNch_%ito%iGeV-RMS", binsPt[binPt], binsPt[binPt+1]);
                binRange                = Form("%iGeV < p_{T}^{jet} < %iGeV",binsPt[binPt],binsPt[binPt+1]);
                if(i==0){
                    vecBinRangeTH1D.push_back(binRange);
                    vecHistosNameTH1D.push_back(histoName);
                }
                hNch_RMS[i][binPt]             = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),Form("hNch_%ito%iGeVRMS; Run Number ; #sigma(N_{Ch}^{Const})", binsPt[binPt], binsPt[binPt+1]),hNBin,hFBin,hLBin);
                EditTH1(globalRuns, doEquidistantXaxis, hNch_RMS[i][binPt], hMarkerStyle[i], hMarkerSize[i], hMarkerColor[i], hLineColor[i]);
                vecHistosTH1D[i].push_back(hNch_RMS[i][binPt]);

                histoName                   = Form("hNne_%ito%iGeV-Mean", binsPt[binPt], binsPt[binPt+1]);
                binRange                = Form("%iGeV < p_{T}^{jet} < %iGeV",binsPt[binPt],binsPt[binPt+1]);
                if(i==0){
                    vecBinRangeTH1D.push_back(binRange);
                    vecHistosNameTH1D.push_back(histoName);
                }
                hNne_Mean[i][binPt]             = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),Form("hNne_%ito%iGeVMean; Run Number ; <N_{Ne}^{Const}>", binsPt[binPt], binsPt[binPt+1] ),hNBin,hFBin,hLBin);
                EditTH1(globalRuns, doEquidistantXaxis, hNne_Mean[i][binPt], hMarkerStyle[i], hMarkerSize[i], hMarkerColor[i], hLineColor[i]);
                vecHistosTH1D[i].push_back(hNne_Mean[i][binPt]);

                histoName                   = Form("hNne_%ito%iGeV-RMS", binsPt[binPt], binsPt[binPt+1]);
                binRange                = Form("%iGeV < p_{T}^{jet} < %iGeV",binsPt[binPt],binsPt[binPt+1]);
                if(i==0){
                    vecBinRangeTH1D.push_back(binRange);
                    vecHistosNameTH1D.push_back(histoName);
                }
                hNne_RMS[i][binPt]             = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),Form("hNne_%ito%iGeVRMS; Run Number ; #sigma(N_{Ne}^{Const})", binsPt[binPt], binsPt[binPt+1]),hNBin,hFBin,hLBin);
                EditTH1(globalRuns, doEquidistantXaxis, hNne_RMS[i][binPt], hMarkerStyle[i], hMarkerSize[i], hMarkerColor[i], hLineColor[i]);
                vecHistosTH1D[i].push_back(hNne_RMS[i][binPt]);

                histoName                   = Form("hPtCh_%ito%iGeV-Mean", binsPt[binPt], binsPt[binPt+1]);
                binRange                = Form("%iGeV < p_{T}^{jet} < %iGeV",binsPt[binPt],binsPt[binPt+1]);
                if(i==0){
                    vecBinRangeTH1D.push_back(binRange);
                    vecHistosNameTH1D.push_back(histoName);
                }
                hPtCh_Mean[i][binPt]             = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),Form("hPtCh_%ito%iGeVMean; Run Number ; <p_{T,Ch}^{Const}> (GeV)", binsPt[binPt], binsPt[binPt+1] ),hNBin,hFBin,hLBin);
                EditTH1(globalRuns, doEquidistantXaxis, hPtCh_Mean[i][binPt], hMarkerStyle[i], hMarkerSize[i], hMarkerColor[i], hLineColor[i]);
                vecHistosTH1D[i].push_back(hPtCh_Mean[i][binPt]);

                histoName                   = Form("hPtCh_%ito%iGeV-RMS", binsPt[binPt], binsPt[binPt+1]);
                binRange                = Form("%iGeV < p_{T}^{jet} < %iGeV",binsPt[binPt],binsPt[binPt+1]);
                if(i==0){
                    vecBinRangeTH1D.push_back(binRange);
                    vecHistosNameTH1D.push_back(histoName);
                }
                hPtCh_RMS[i][binPt]             = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),Form("hPtCh_%ito%iGeVRMS; Run Number ; #sigma(p_{T,Ch}^{Const}) (GeV)", binsPt[binPt], binsPt[binPt+1]),hNBin,hFBin,hLBin);
                EditTH1(globalRuns, doEquidistantXaxis, hPtCh_RMS[i][binPt], hMarkerStyle[i], hMarkerSize[i], hMarkerColor[i], hLineColor[i]);
                vecHistosTH1D[i].push_back(hPtCh_RMS[i][binPt]);

                histoName                   = Form("hPtNe_%ito%iGeV-Mean", binsPt[binPt], binsPt[binPt+1]);
                binRange                = Form("%iGeV < p_{T}^{jet} < %iGeV",binsPt[binPt],binsPt[binPt+1]);
                if(i==0){
                    vecBinRangeTH1D.push_back(binRange);
                    vecHistosNameTH1D.push_back(histoName);
                }
                hPtNe_Mean[i][binPt]             = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),Form("hPtNe_%ito%iGeVMean; Run Number ; <p_{T,Ne}^{Const}> (GeV)", binsPt[binPt], binsPt[binPt+1] ),hNBin,hFBin,hLBin);
                EditTH1(globalRuns, doEquidistantXaxis, hPtNe_Mean[i][binPt], hMarkerStyle[i], hMarkerSize[i], hMarkerColor[i], hLineColor[i]);
                vecHistosTH1D[i].push_back(hPtNe_Mean[i][binPt]);

                histoName                   = Form("hPtNe_%ito%iGeV-RMS", binsPt[binPt], binsPt[binPt+1]);
                binRange                = Form("%iGeV < p_{T}^{jet} < %iGeV",binsPt[binPt],binsPt[binPt+1]);
                if(i==0){
                    vecBinRangeTH1D.push_back(binRange);
                    vecHistosNameTH1D.push_back(histoName);
                }
                hPtNe_RMS[i][binPt]             = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),Form("hPtNe_%ito%iGeVRMS; Run Number ; #sigma(p_{T,Ne}^{Const}) (GeV)", binsPt[binPt], binsPt[binPt+1]),hNBin,hFBin,hLBin);
                EditTH1(globalRuns, doEquidistantXaxis, hPtNe_RMS[i][binPt], hMarkerStyle[i], hMarkerSize[i], hMarkerColor[i], hLineColor[i]);
                vecHistosTH1D[i].push_back(hPtNe_RMS[i][binPt]);

                histoName                   = Form("hPtChMax_%ito%iGeV-Mean", binsPt[binPt], binsPt[binPt+1]);
                binRange                = Form("%iGeV < p_{T}^{jet} < %iGeV",binsPt[binPt],binsPt[binPt+1]);
                if(i==0){
                    vecBinRangeTH1D.push_back(binRange);
                    vecHistosNameTH1D.push_back(histoName);
                }
                hPtChMax_Mean[i][binPt]             = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),Form("hPtChMax_%ito%iGeVMean; Run Number ; <p_{T,Ch}^{Const,Max}> (GeV)", binsPt[binPt], binsPt[binPt+1] ),hNBin,hFBin,hLBin);
                EditTH1(globalRuns, doEquidistantXaxis, hPtChMax_Mean[i][binPt], hMarkerStyle[i], hMarkerSize[i], hMarkerColor[i], hLineColor[i]);
                vecHistosTH1D[i].push_back(hPtChMax_Mean[i][binPt]);

                histoName                   = Form("hPtChMax_%ito%iGeV-RMS", binsPt[binPt], binsPt[binPt+1]);
                binRange                = Form("%iGeV < p_{T}^{jet} < %iGeV",binsPt[binPt],binsPt[binPt+1]);
                if(i==0){
                    vecBinRangeTH1D.push_back(binRange);
                    vecHistosNameTH1D.push_back(histoName);
                }
                hPtChMax_RMS[i][binPt]             = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),Form("hPtChMax_%ito%iGeVRMS; Run Number ; #sigma(p_{T,Ch}^{Const,Max}) (GeV)", binsPt[binPt], binsPt[binPt+1]),hNBin,hFBin,hLBin);
                EditTH1(globalRuns, doEquidistantXaxis, hPtChMax_RMS[i][binPt], hMarkerStyle[i], hMarkerSize[i], hMarkerColor[i], hLineColor[i]);
                vecHistosTH1D[i].push_back(hPtChMax_RMS[i][binPt]);

                histoName                   = Form("hPtNeMax_%ito%iGeV-Mean", binsPt[binPt], binsPt[binPt+1]);
                binRange                = Form("%iGeV < p_{T}^{jet} < %iGeV",binsPt[binPt],binsPt[binPt+1]);
                if(i==0){
                    vecBinRangeTH1D.push_back(binRange);
                    vecHistosNameTH1D.push_back(histoName);
                }
                hPtNeMax_Mean[i][binPt]             = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),Form("hPtNeMax_%ito%iGeVMean; Run Number ; <p_{T,Ne}^{Const,Max}> (GeV)", binsPt[binPt], binsPt[binPt+1] ),hNBin,hFBin,hLBin);
                EditTH1(globalRuns, doEquidistantXaxis, hPtNeMax_Mean[i][binPt], hMarkerStyle[i], hMarkerSize[i], hMarkerColor[i], hLineColor[i]);
                vecHistosTH1D[i].push_back(hPtNeMax_Mean[i][binPt]);

                histoName                   = Form("hPtNeMax_%ito%iGeV-RMS", binsPt[binPt], binsPt[binPt+1]);
                binRange                = Form("%iGeV < p_{T}^{jet} < %iGeV",binsPt[binPt],binsPt[binPt+1]);
                if(i==0){
                    vecBinRangeTH1D.push_back(binRange);
                    vecHistosNameTH1D.push_back(histoName);
                }
                hPtNeMax_RMS[i][binPt]             = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),Form("hPtNeMax_%ito%iGeVRMS; Run Number ; #sigma(p_{T,Ne}^{Const,Max}) (GeV)", binsPt[binPt], binsPt[binPt+1]),hNBin,hFBin,hLBin);
                EditTH1(globalRuns, doEquidistantXaxis, hPtNeMax_RMS[i][binPt], hMarkerStyle[i], hMarkerSize[i], hMarkerColor[i], hLineColor[i]);
                vecHistosTH1D[i].push_back(hPtNeMax_RMS[i][binPt]);
            }
        }

        for (Int_t binEta = 0; binEta < nEtaBins; binEta++){
            histoName                   = Form("hEtaPhi_Eta%1.1fto%1.1f-Mean", binsEta[binEta], binsEta[binEta+1]);
            binRange                = Form("%1.1f < #eta < %1.1f",binsEta[binEta],binsEta[binEta+1]);
            if(i==0){
                vecBinRangeTH1D.push_back(binRange);
                vecHistosNameTH1D.push_back(histoName);
            }
            hEtaPhi_Mean[i][binEta]             = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),Form("hEtaPhi_Eta%1.1fto%1.1fMean; Run Number ; <#phi> (rad)", binsEta[binEta], binsEta[binEta+1]),hNBin,hFBin,hLBin);
            EditTH1(globalRuns, doEquidistantXaxis, hEtaPhi_Mean[i][binEta], hMarkerStyle[i], hMarkerSize[i], hMarkerColor[i], hLineColor[i]);
            vecHistosTH1D[i].push_back(hEtaPhi_Mean[i][binEta]);

            histoName                   = Form("hEtaPhi_Eta%1.1fto%1.1f-RMS", binsEta[binEta], binsEta[binEta+1]);
            binRange                = Form("%1.1f < #eta < %1.1f",binsEta[binEta],binsEta[binEta+1]);
            if(i==0){
                vecBinRangeTH1D.push_back(binRange);
                vecHistosNameTH1D.push_back(histoName);
            }
            hEtaPhi_RMS[i][binEta]             = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),Form("hEtaPhi_Eta%1.1fto%1.1fRMS; Run Number ; #sigma(#phi) (rad)", binsEta[binEta], binsEta[binEta+1]),hNBin,hFBin,hLBin);
            EditTH1(globalRuns, doEquidistantXaxis, hEtaPhi_RMS[i][binEta], hMarkerStyle[i], hMarkerSize[i], hMarkerColor[i], hLineColor[i]);
            vecHistosTH1D[i].push_back(hEtaPhi_RMS[i][binEta]);

            if(jetType == "Full Jets"){
                histoName                   = Form("hEtaPhiCh_Eta%1.1fto%1.1f-Mean", binsEta[binEta], binsEta[binEta+1]);
                binRange                = Form("%1.1f < #eta < %1.1f",binsEta[binEta],binsEta[binEta+1]);
                if(i==0){
                    vecBinRangeTH1D.push_back(binRange);
                    vecHistosNameTH1D.push_back(histoName);
                }
                hEtaPhiCh_Mean[i][binEta]             = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),Form("hEtaPhiCh_Eta%1.1fto%1.1fMean; Run Number ; <#phi_{Ch}> (rad)", binsEta[binEta], binsEta[binEta+1]),hNBin,hFBin,hLBin);
                EditTH1(globalRuns, doEquidistantXaxis, hEtaPhiCh_Mean[i][binEta], hMarkerStyle[i], hMarkerSize[i], hMarkerColor[i], hLineColor[i]);
                vecHistosTH1D[i].push_back(hEtaPhiCh_Mean[i][binEta]);

                histoName                   = Form("hEtaPhiCh_Eta%1.1fto%1.1f-RMS", binsEta[binEta], binsEta[binEta+1]);
                binRange                = Form("%1.1f < #eta < %1.1f",binsEta[binEta],binsEta[binEta+1]);
                if(i==0){
                    vecBinRangeTH1D.push_back(binRange);
                    vecHistosNameTH1D.push_back(histoName);
                }
                hEtaPhiCh_RMS[i][binEta]             = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),Form("hEtaPhiCh_Eta%1.1fto%1.1fRMS; Run Number ; #sigma(#phi_{Ch}) (rad)", binsEta[binEta], binsEta[binEta+1]),hNBin,hFBin,hLBin);
                EditTH1(globalRuns, doEquidistantXaxis, hEtaPhiCh_RMS[i][binEta], hMarkerStyle[i], hMarkerSize[i], hMarkerColor[i], hLineColor[i]);
                vecHistosTH1D[i].push_back(hEtaPhiCh_RMS[i][binEta]);

                histoName                   = Form("hEtaPhiNe_Eta%1.1fto%1.1f-Mean", binsEta[binEta], binsEta[binEta+1]);
                binRange                = Form("%1.1f < #eta < %1.1f",binsEta[binEta],binsEta[binEta+1]);
                if(i==0){
                    vecBinRangeTH1D.push_back(binRange);
                    vecHistosNameTH1D.push_back(histoName);
                }
                hEtaPhiNe_Mean[i][binEta]             = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),Form("hEtaPhiNe_Eta%1.1fto%1.1fMean; Run Number ; <#phi_Ne> (rad)", binsEta[binEta], binsEta[binEta+1]),hNBin,hFBin,hLBin);
                EditTH1(globalRuns, doEquidistantXaxis, hEtaPhiNe_Mean[i][binEta], hMarkerStyle[i], hMarkerSize[i], hMarkerColor[i], hLineColor[i]);
                vecHistosTH1D[i].push_back(hEtaPhiNe_Mean[i][binEta]);

                histoName                   = Form("hEtaPhiNe_Eta%1.1fto%1.1f-RMS", binsEta[binEta], binsEta[binEta+1]);
                binRange                = Form("%1.1f < #eta < %1.1f",binsEta[binEta],binsEta[binEta+1]);
                if(i==0){
                    vecBinRangeTH1D.push_back(binRange);
                    vecHistosNameTH1D.push_back(histoName);
                }
                hEtaPhiNe_RMS[i][binEta]             = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),Form("hEtaPhiNe_Eta%1.1fto%1.1fRMS; Run Number ; #sigma(#phi_{Ne}) (rad)", binsEta[binEta], binsEta[binEta+1]),hNBin,hFBin,hLBin);
                EditTH1(globalRuns, doEquidistantXaxis, hEtaPhiNe_RMS[i][binEta], hMarkerStyle[i], hMarkerSize[i], hMarkerColor[i], hLineColor[i]);
                vecHistosTH1D[i].push_back(hEtaPhiNe_RMS[i][binEta]);

                histoName                   = Form("hEtaPhiChMax_Eta%1.1fto%1.1f-Mean", binsEta[binEta], binsEta[binEta+1]);
                binRange                = Form("%1.1f < #eta < %1.1f",binsEta[binEta],binsEta[binEta+1]);
                if(i==0){
                    vecBinRangeTH1D.push_back(binRange);
                    vecHistosNameTH1D.push_back(histoName);
                }
                hEtaPhiChMax_Mean[i][binEta]             = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),Form("hEtaPhiChMax_Eta%1.1fto%1.1fMean; Run Number ; <#phi_{Ch}^{Max}>", binsEta[binEta], binsEta[binEta+1]),hNBin,hFBin,hLBin);
                EditTH1(globalRuns, doEquidistantXaxis, hEtaPhiChMax_Mean[i][binEta], hMarkerStyle[i], hMarkerSize[i], hMarkerColor[i], hLineColor[i]);
                vecHistosTH1D[i].push_back(hEtaPhiChMax_Mean[i][binEta]);

                histoName                   = Form("hEtaPhiChMax_Eta%1.1fto%1.1f-RMS", binsEta[binEta], binsEta[binEta+1]);
                binRange                = Form("%1.1f < #eta < %1.1f",binsEta[binEta],binsEta[binEta+1]);
                if(i==0){
                    vecBinRangeTH1D.push_back(binRange);
                    vecHistosNameTH1D.push_back(histoName);
                }
                hEtaPhiChMax_RMS[i][binEta]             = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),Form("hEtaPhiChMax_Eta%1.1fto%1.1fRMS; Run Number ; #sigma(#phi_{Ch}^{Max})", binsEta[binEta], binsEta[binEta+1]),hNBin,hFBin,hLBin);
                EditTH1(globalRuns, doEquidistantXaxis, hEtaPhiChMax_RMS[i][binEta], hMarkerStyle[i], hMarkerSize[i], hMarkerColor[i], hLineColor[i]);
                vecHistosTH1D[i].push_back(hEtaPhiChMax_RMS[i][binEta]);

                histoName                   = Form("hEtaPhiNeMax_Eta%1.1fto%1.1f-Mean", binsEta[binEta], binsEta[binEta+1]);
                binRange                = Form("%1.1f < #eta < %1.1f",binsEta[binEta],binsEta[binEta+1]);
                if(i==0){
                    vecBinRangeTH1D.push_back(binRange);
                    vecHistosNameTH1D.push_back(histoName);
                }
                hEtaPhiNeMax_Mean[i][binEta]             = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),Form("hEtaPhiNeMax_Eta%1.1fto%1.1fMean; Run Number ; <#phi_{Ne}^{Max}>", binsEta[binEta], binsEta[binEta+1]),hNBin,hFBin,hLBin);
                EditTH1(globalRuns, doEquidistantXaxis, hEtaPhiNeMax_Mean[i][binEta], hMarkerStyle[i], hMarkerSize[i], hMarkerColor[i], hLineColor[i]);
                vecHistosTH1D[i].push_back(hEtaPhiNeMax_Mean[i][binEta]);

                histoName                   = Form("hEtaPhiNeMax_Eta%1.1fto%1.1f-RMS", binsEta[binEta], binsEta[binEta+1]);
                binRange                = Form("%1.1f < #eta < %1.1f",binsEta[binEta],binsEta[binEta+1]);
                if(i==0){
                    vecBinRangeTH1D.push_back(binRange);
                    vecHistosNameTH1D.push_back(histoName);
                }
                hEtaPhiNeMax_RMS[i][binEta]             = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),Form("hEtaPhiNeMax_Eta%1.1fto%1.1fRMS; Run Number ; #sigma(#phi_{Ne}^{Max})", binsEta[binEta], binsEta[binEta+1]),hNBin,hFBin,hLBin);
                EditTH1(globalRuns, doEquidistantXaxis, hEtaPhiNeMax_RMS[i][binEta], hMarkerStyle[i], hMarkerSize[i], hMarkerColor[i], hLineColor[i]);
                vecHistosTH1D[i].push_back(hEtaPhiNeMax_RMS[i][binEta]);
            }
        }

        for (Int_t binE = 0; binE < nEBins; binE++){
            histoName                   = Form("hClusterCells_%ito%iGeV-Mean", binsE[binE], binsE[binE+1]);
            binRange                = Form("%iGeV < E^{cluster} < %iGeV",binsE[binE],binsE[binE+1]);
            if(i==0){
                vecBinRangeTH1D.push_back(binRange);
                vecHistosNameTH1D.push_back(histoName);
            }
            hClusterCells_Mean[i][binE]             = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),Form("hClusterCells_%ito%iGeVMean; Run Number ; #frac{<N^{Cell}>}{N^{Cluster}}", binsE[binE], binsE[binE+1]),hNBin,hFBin,hLBin);
            EditTH1(globalRuns, doEquidistantXaxis, hClusterCells_Mean[i][binE], hMarkerStyle[i], hMarkerSize[i], hMarkerColor[i], hLineColor[i]);
            vecHistosTH1D[i].push_back(hClusterCells_Mean[i][binE]);

            histoName                   = Form("hClusterCells_%ito%iGeV-RMS", binsE[binE], binsE[binE+1]);
            binRange                = Form("%iGeV < E^{cluster} < %iGeV",binsE[binE],binsE[binE+1]);
            if(i==0){
                vecBinRangeTH1D.push_back(binRange);
                vecHistosNameTH1D.push_back(histoName);
            }
            hClusterCells_RMS[i][binE]             = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),Form("hClusterCells_%ito%iGeVRMS; Run Number ; #sigma(#frac{N^{Cells}}{N^{Cluster}})", binsE[binE], binsE[binE+1]),hNBin,hFBin,hLBin);
            EditTH1(globalRuns, doEquidistantXaxis, hClusterCells_RMS[i][binE], hMarkerStyle[i], hMarkerSize[i], hMarkerColor[i], hLineColor[i]);
            vecHistosTH1D[i].push_back(hClusterCells_RMS[i][binE]);
        }

        if(fEnergyFlag.Contains("PbPb")){
            histoName                   = "hCentrality-Mean";
            binRange                = "";
            if(i==0){
              vecBinRangeTH1D.push_back(binRange);
              vecHistosNameTH1D.push_back(histoName);
            }
            hCentralityMean[i]             = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),"hCentrality-Mean; Run Number ; #bar{cent} (%)",hNBin,hFBin,hLBin);
            EditTH1(globalRuns, doEquidistantXaxis, hCentralityMean[i], hMarkerStyle[i], hMarkerSize[i], hMarkerColor[i], hLineColor[i]);
            vecHistosTH1D[i].push_back(hCentralityMean[i]);

            histoName                   = "hCentrality-Min";
            binRange                = "";
            if(i==0){
              vecBinRangeTH1D.push_back(binRange);
              vecHistosNameTH1D.push_back(histoName);
            }
            hCentralityMin[i]             = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),"hCentrality-Min; Run Number ; min cent (%)",hNBin,hFBin,hLBin);
            EditTH1(globalRuns, doEquidistantXaxis, hCentralityMin[i], hMarkerStyle[i], hMarkerSize[i], hMarkerColor[i], hLineColor[i]);
            vecHistosTH1D[i].push_back(hCentralityMin[i]);

            histoName                   = "hCentrality-Max";
            binRange                = "";
            if(i==0){
              vecBinRangeTH1D.push_back(binRange);
              vecHistosNameTH1D.push_back(histoName);
            }
            hCentralityMax[i]             = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),"hCentrality-Max; Run Number ; max cent (%)",hNBin,hFBin,hLBin);
            EditTH1(globalRuns, doEquidistantXaxis, hCentralityMax[i], hMarkerStyle[i], hMarkerSize[i], hMarkerColor[i], hLineColor[i]);
            vecHistosTH1D[i].push_back(hCentralityMax[i]);

            histoName                   = "hEventPlaneAngle-Mean";
            binRange                = "";
            if(i==0){
              vecBinRangeTH1D.push_back(binRange);
              vecHistosNameTH1D.push_back(histoName);
            }
            hEventPlaneAngleMean[i]     = new TH1D(Form("%s_%s", histoName.Data(), DataSets[i].Data()),"hEventPlaneAngle-Mean; Run Number ; #bar{e.p.a.} (rad)",hNBin,hFBin,hLBin);
            EditTH1(globalRuns, doEquidistantXaxis, hEventPlaneAngleMean[i], hMarkerStyle[i], hMarkerSize[i], hMarkerColor[i], hLineColor[i]);
            vecHistosTH1D[i].push_back(hEventPlaneAngleMean[i]);
        }

    } // end of loop over datasets

    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //+++++++++++++++++++++++++++++++ create log files +++++++++++++++++++++++++++++++++++++++++++++++++++++
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    fstream* fLog           = new fstream[nSets];
    fstream* fEventLog      = new fstream[nSets];
    for(Int_t iStr=0; iStr<nSets; iStr++){
        if(useDataRunListForMC && iStr>=nData) fLog[iStr].open(Form("%s/A-%s-%s.log",outputDir.Data(),DataSets[iStr].Data(),DataSets[0].Data()), ios::out);
        else fLog[iStr].open(Form("%s/A-%s.log",outputDir.Data(),DataSets[iStr].Data()), ios::out);
        fLog[iStr] << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
        fLog[iStr] << DataSets[iStr].Data() << endl;
        fLog[iStr] << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
        fLog[iStr] << fCollisionSystem.Data() << endl;
        fLog[iStr] << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
        fLog[iStr] << "processed Trigger: " << fTriggerSelection.Data() << endl;
        fLog[iStr] << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;

        if(useDataRunListForMC && iStr>=nData) fEventLog[iStr].open(Form("%s/A-NEvents-%s-%s.log",outputDir.Data(),DataSets[iStr].Data(),DataSets[0].Data()), ios::out);
        else fEventLog[iStr].open(Form("%s/A-NEvents-%s.log",outputDir.Data(),DataSets[iStr].Data()), ios::out);
        fEventLog[iStr] << Form("Run\t%s",DataSets[iStr].Data()) << endl;
    }

    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //+++++++++++++++++++++++++++++++++* Supporting variables +++++++++++++++++++++++++++++++++++++++++++++
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    TString fRootFile;
    TString fDataSet;
    TString fRunNumber;
    Int_t bin                               = -1;

    Double_t** fitValuesTH1D                = new Double_t*[nSets];
    Float_t** fitValuesTH1F                 = new Float_t*[nSets];
    Double_t* sumEvents                     = new Double_t[nSets];

    std::vector<TString>* vecMissingRuns    = new std::vector<TString>[nSets];
    std::vector<TString>* vecLowJetStats    = new std::vector<TString>[nSets];

    Bool_t isNullNSigmas = kFALSE;
    if(nSigmasBadRun == NULL){
        isNullNSigmas = kTRUE;
        const Int_t nQuantities = 5;
        nSigmasBadRun = (Int_t*)calloc(nQuantities,sizeof(Int_t));
        for(Int_t i=0; i<nQuantities; i++) nSigmasBadRun[i] = 5;
    }
    Int_t nSigmasBad[5];
    const Int_t nQuantities = 5;
    for(Int_t i=0; i<nQuantities; i++) nSigmasBad[i] = 5;

    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //++++++++++++++++++++++++++++++ Looping over DataSets ++++++++++++++++++++++++++++++++++++++++++++++++
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    for(Int_t i=0; i<nSets; i++) {
        fitValuesTH1D[i]        = new Double_t[(Int_t)vecHistosTH1D[i].size()];
        fitValuesTH1F[i]        = new Float_t[(Int_t)vecHistosTH1F[i].size()];
        vecRuns.clear();
        vecRunsBad.clear();
        vecRunsBadVertexZ.clear();
        vecRunsBadAllClusterEnergy.clear();
        vecRunsBadNch.clear();
        vecRunsBadPtCh.clear();
        vecRunsBadCentrality.clear();
        vecRunsBadJets.clear();
        badRunCalc sVertexZMean              = {nSigmasBad[0],0,0.,0.,0.,hVertexZMean[i]};        // calculate mean for every dataset separately
        badRunCalc sCentralityMean           = {nSigmasBad[1],0,0.,0.,0.,hCentralityMean[i]};
        badRunCalc sAllClusterEnergyMean     = {nSigmasBad[2],0,0.,0.,0.,hAllClusterEnergy_Mean[i]};
        badRunCalc sNchMean                  = {nSigmasBad[3],0,0.,0.,0.,hMeanNch[i]};
        badRunCalc sPtChMean                 = {nSigmasBad[4],0,0.,0.,0.,hMeanPtCh[i]};
        fDataSet                        = vecDataSet.at(i);
        if(!readin(fileRuns[i], vecRuns)) {cout << "ERROR, no Run Numbers could be found! Returning..." << endl; return;}

        TH1F *MeanVertexZ          = new TH1F("MeanVertexZ", "Mean z-Vertex", 1000, -10., 10.);
        TH1F *MeanCentrality       = new TH1F("MeanCentrality", "Mean Centrality", 1000, 0., 100.);
        TH1F *MeanAllClusterEnergy = new TH1F("MeanAllClusterEnergy", "Mean All Cluster Energy", 10000, 0, 100);
        TH1F *MeanNch              = new TH1F("MeanNch", "Mean Number of Charged Tracks", 1000, 0., 10.);
        TH1F *MeanPtCh             = new TH1F("MeanPtCh", "Mean Pt of Charged Tracks", 1000, 0., 20.);

        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        //++++++++++++++++++++++++++++++ Looping over Runs +++++++++++++++++++++++++++++++++++++++++++++++++++*
        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        cout << endl;
        cout << "\t----------------------------------------------------------------------------" << endl;
        cout << "\tLooping over Runs of DataSet |" << (vecDataSet.at(i)).Data() << "|" << endl;
        cout << "\t----------------------------------------------------------------------------" << endl;
        cout << endl;
        fLog[i] << "Looping over Runs:" << endl;
        sumEvents[i] = 0;

        vecMissingRuns[i].clear();
        vecLowJetStats[i].clear();
        for(Int_t j=0; j<(Int_t) vecRuns.size(); j++){
            //--------------------------------------------------------------------------------------------------------
            //------------------------- Read in individual files -----------------------------------------------------
            //--------------------------------------------------------------------------------------------------------
            fRunNumber                  = vecRuns.at(j);

            // MC reader
            if(i >= nData) fRootFile    = Form("%s/%s/%s", filePathMC.Data(), fRunNumber.Data(), fileName.Data());
            // Data reader
            else fRootFile              = Form("%s/%s/%s/%s", filePath.Data(), fDataSet.Data(), fRunNumber.Data(), fileName.Data());

            TFile* RootFile             = new TFile(fRootFile.Data(),"READ");
            if(RootFile->IsZombie()) {vecMissingRuns[i].push_back(fRunNumber); cout << "INFO: ROOT file '" << fRootFile.Data() << "' could not be openend, continue!" << endl; continue;}

            cout << endl;
            cout << "\t\t----------------------------------------------------------------------------" << endl;
            cout << Form("\t\tRun %s", fRunNumber.Data()) << endl;
            cout << "\t\tProcessing file: " << fRootFile.Data() << endl;
            cout << "\t\t----------------------------------------------------------------------------" << endl;
            cout << endl;

            //--------------------------------------------------------------------------------------------------------

            TDirectory *TopDir                   = NULL;
            TopDir                      = (TDirectory*)RootFile->Get(fTriggerSelection.Data());
            if(!TopDir){
                fTriggerSelection.ReplaceAll("nodownscalecorr","default");
                TopDir                      = (TDirectory*)RootFile->Get(fTriggerSelection.Data());
            }
            if(TopDir == NULL) {cout << "ERROR: TopDir not Found"<<endl; return;}
            TList* HistoList         = (TList*)TopDir->Get(fTriggerSelection.Data());
            if(HistoList == NULL) {cout << "ERROR: " << fTriggerSelection.Data() << " not found in File" << endl; return;}
            else HistoList->SetOwner(kTRUE);

            //--------------------------------------------------------------------------------------------------------
            if(doEquidistantXaxis) bin  = mapBin[fRunNumber];
            else bin = fRunNumber.Atoi() - hFBin;

            //--------------------------------------------------------------------------------------------------------
            //----------------------------- Event quality histograms processing --------------------------------------
            //--------------------------------------------------------------------------------------------------------
            TH1D *EVENTS    = NULL;
            TH2D *JETS      = NULL;
            EVENTS          = (TH1D*) HistoList->FindObject("hClusterCounter");
            JETS            = (TH2D*) HistoList->FindObject("hJetSpectrum");
            if(!EVENTS){
                cout << "ERROR: Object |NEvents| could not be found! Returning!" << endl;
                return;
            }
            if(!JETS){
                cout << "ERROR: Object hJetSpectrum could not be found! Returning!" << endl;
                return;
            }
            TH1D* Jets1D = JETS->ProjectionY("JetPtCheck",1,1);
            Double_t nJetEntries = Jets1D->GetEntries();

            Double_t nEvents = EVENTS->GetBinContent(1);

            if(nJetEntries < 1){
                vecLowJetStats[i].push_back(fRunNumber);
                vecRunsBadJets.push_back(fRunNumber);
                delete TopDir;
                TopDir = NULL;
                RootFile->Close();
                delete RootFile;
                RootFile = NULL;
                continue;
            }

            sumEvents[i]                += nEvents;
            //--------------------------------------------------------------------------------------------------------
            // write to log file
            fLog[i] << "----------------------------------------------------------------------------" << endl;
            fLog[i] << "Processing file: " << fRootFile.Data() << endl;
            fLog[i] << "Run: " << fRunNumber.Data() << endl;
            fLog[i] << "----------------------------------------------------------------------------" << endl;
            //--------------------------------------------------------------------------------------------------------
            if( nEvents == 0 ){cout << "Warning: number of accepted events in run: " << nEvents << "! Setting nEvents to 1..." << endl; nEvents = 1;}
            Bool_t notScaled = kTRUE;
            if( nEvents > 0 && nEvents < 1) notScaled = kFALSE; // Data already normalized to 1?

            //--------------------------------------------------------------------------------------------------------
            // fill trending histos for events
            hNEvents[i]->SetBinContent(bin, nEvents);

            //--------------------------------------------------------------------------------------------------------
            //------------------------- primary vertex distribution in Z ---------------------------------------------
            //--------------------------------------------------------------------------------------------------------
            TH1D* ZVertex               = (TH1D*) HistoList->FindObject("Vtz_selected");
            if(ZVertex){
                hVertexZMean[i]->SetBinContent(bin, ZVertex->GetMean());
                hVertexZMean[i]->SetBinError(bin, ZVertex->GetMeanError());
                hVertexZRMS[i]->SetBinContent(bin, ZVertex->GetRMS());
                hVertexZRMS[i]->SetBinError(bin, ZVertex->GetRMSError());
                sVertexZMean.mean +=  ZVertex->GetMean();
                MeanVertexZ->Fill(ZVertex->GetMean(),ZVertex->GetRMS());
                TH1D* tempVertexZ       = new TH1D(*ZVertex);
                tempVertexZ->GetXaxis()->SetTitle("z-Vertex (cm)");
                tempVertexZ->GetYaxis()->SetTitle("#frac{1}{N_{Events}} #frac{dN}{dz}");
                tempVertexZ->SetTitle(Form("%s",fRunNumber.Data()));
                tempVertexZ->Sumw2();
                if(notScaled) tempVertexZ->Scale(1 / tempVertexZ->GetEntries());
                tempVertexZ->SetDirectory(0);
                vecVertexZ[i].push_back(tempVertexZ);
                if(i==0) mapVertexRatio[j]=bin;
                else if( (mapVertexRatio.find(j) != mapVertexRatio.end()) && mapVertexRatio[j]>=0 && j<(Int_t)vecVertexZ[0].size() ){
                    TH1D* tempVertexRatio   = new TH1D(*tempVertexZ);
                    tempVertexRatio->Divide(tempVertexRatio,vecVertexZ[0].at(j),1,1,"B");
                    tempVertexRatio->SetName(Form("%s_z-VertexRatio_%s",fRunNumber.Data(),fDataSet.Data()));
                    tempVertexRatio->GetXaxis()->SetRangeUser(-10,10);
                    tempVertexRatio->GetYaxis()->SetTitle("ratio");
                    tempVertexRatio->GetYaxis()->SetRangeUser(0,2);
                    vecVertexZRatio[i-1].push_back(tempVertexRatio);
                }
            }else cout << "INFO: Object |VertexZ| could not be found! Skipping Fill..." << endl;

            //--------------------------------------------------------------------------------------------------------
            //------------------------- Jet pT -----------------------------------------------------------------------
            //--------------------------------------------------------------------------------------------------------
            TH2D* JetPt2D               = (TH2D*) HistoList->FindObject("hJetSpectrum");

            double nUsableJets;

            if(JetPt2D){
                Int_t tempNXbins;
                std::vector<Double_t> xbinsVec;

                if(fTrigger == "INT7"){
                  tempNXbins = 13;
                  xbinsVec = {1.,2.,3.,4.,5.,7.,
                              10.,15.,
                              20.,30.,50.,
                              80.,160.};
                  }

                else if(fTrigger == "EMC7" || fTrigger == "EJE"){
                  tempNXbins = 33;
                  xbinsVec = {1.,2.,3.,4.,5.,6.,7.,8.,9.,
                              10.,11.,12.,13.,14.,15.,16.,17.,18.,19.,
                              20.,22.,24.,26.,28.,30.,35.,40.,45.,50.,
                              60.,70.,100.,160.};
                }
                else{ cout << "nothing found" << endl; return; }

                const Int_t nXbins = tempNXbins;
                Double_t xbins[xbinsVec.size()];
                std::copy(xbinsVec.begin(), xbinsVec.end(), xbins);

                TH1D* JetPt = (TH1D*)JetPt2D->ProjectionY("JetPt",1,1);
                if(fTrigger == "INT7"){
                    JetPt->GetXaxis()->SetRangeUser(5,350);
                    nUsableJets = JetPt->Integral(10,60);
                }
                if(fTrigger == "EMC7"){
                    JetPt->GetXaxis()->SetRangeUser(5,350);
                    nUsableJets = JetPt->Integral(20,100);
                }
                if(fTrigger == "EJE"){
                    JetPt->GetXaxis()->SetRangeUser(16,350);
                    nUsableJets = JetPt->Integral(30,350);
                }
                hJetPt_Mean[i]->SetBinContent(bin, JetPt->GetMean());
                hJetPt_Mean[i]->SetBinError(bin, JetPt->GetMeanError());
                hJetPt_RMS[i]->SetBinContent(bin, JetPt->GetRMS());
                hJetPt_RMS[i]->SetBinError(bin, JetPt->GetRMSError());
                TH1D* tempJetPt       = new TH1D(*JetPt);
                tempJetPt->GetXaxis()->SetTitle("p_{T}^{Jet}");
                tempJetPt->GetYaxis()->SetTitle("#frac{1}{N_{Events}} N^{Jet}");
                tempJetPt->SetTitle(Form("%s",fRunNumber.Data()));
                tempJetPt->Sumw2();
                if(notScaled) tempJetPt->Scale(1 / tempJetPt->GetEntries());
                TH1 *rebinnedJetPt = tempJetPt->Rebin(nXbins-1,"rebinnedJetPt",xbins);
                rebinnedJetPt->Scale(1.,"width");
                rebinnedJetPt->SetDirectory(0);
                vecJetPt[i].push_back((TH1D*)rebinnedJetPt);
                //tempJetPt->SetDirectory(0);
                //vecJetPt[i].push_back(tempJetPt);
                if(i==0) mapJetPtRatio[j]=bin;
                else if( (mapJetPtRatio.find(j) != mapJetPtRatio.end()) && mapJetPtRatio[j]>=0 && j<(Int_t)vecJetPt[0].size() ){
                    TH1D* tempJetPtRatio   = new TH1D(*((TH1D*)rebinnedJetPt));
                    tempJetPtRatio->Divide(tempJetPtRatio,vecJetPt[0].at(j),1,1,"B");
                    tempJetPtRatio->SetName(Form("%s_z-JetPtRatio_%s",fRunNumber.Data(),fDataSet.Data()));
                    tempJetPtRatio->GetXaxis()->SetRangeUser(-10,10);
                    tempJetPtRatio->GetYaxis()->SetTitle("ratio");
                    tempJetPtRatio->GetYaxis()->SetRangeUser(0,2);
                    vecJetPtRatio[i-1].push_back(tempJetPtRatio);
                }
            }else cout << "INFO: Object |Jet pT| could not be found! Skipping Fill..." << endl;

            //--------------------------------------------------------------------------------------------------------
            //------------------------- Max Jet pT -------------------------------------------------------------------
            //--------------------------------------------------------------------------------------------------------
            TH2D* JetPtMax2D               = (TH2D*) HistoList->FindObject("hJetSpectrumMax");
            if(JetPtMax2D){
                Int_t tempNXbins;
                std::vector<Double_t> xbinsVec;

                if(fTrigger == "INT7"){
                  tempNXbins = 13;
                  xbinsVec = {1.,2.,3.,4.,5.,7.,
                              10.,15.,
                              20.,30.,50.,
                              80.,160.};
                  }

                else if(fTrigger == "EMC7" || fTrigger == "EJE"){
                  tempNXbins = 33;
                  xbinsVec = {1.,2.,3.,4.,5.,6.,7.,8.,9.,
                              10.,11.,12.,13.,14.,15.,16.,17.,18.,19.,
                              20.,22.,24.,26.,28.,30.,35.,40.,45.,50.,
                              60.,70.,100.,160.};
                }
                else{ cout << "nothing found" << endl; return; }

                const Int_t nXbins = tempNXbins;

                Double_t xbins[xbinsVec.size()];
                std::copy(xbinsVec.begin(), xbinsVec.end(), xbins);

                TH1D* JetPtMax = (TH1D*)JetPtMax2D->ProjectionY("JetPtMax",0,1);
                if(fTrigger == "INT7")  JetPtMax->GetXaxis()->SetRangeUser(5,350);
                if(fTrigger == "EMC7") JetPtMax->GetXaxis()->SetRangeUser(5,350);
                if(fTrigger == "EJE") JetPtMax->GetXaxis()->SetRangeUser(16,350);
                hJetPtMax_Mean[i]->SetBinContent(bin, JetPtMax->GetMean());
                hJetPtMax_Mean[i]->SetBinError(bin, JetPtMax->GetMeanError());
                hJetPtMax_RMS[i]->SetBinContent(bin, JetPtMax->GetRMS());
                hJetPtMax_RMS[i]->SetBinError(bin, JetPtMax->GetRMSError());
                TH1D* tempJetPtMax       = new TH1D(*JetPtMax);
                tempJetPtMax->GetXaxis()->SetTitle("p_{T}^{Jet,Max}");
                tempJetPtMax->GetYaxis()->SetTitle("#frac{1}{N_{Events}} N^{Jet,Max}");
                tempJetPtMax->SetTitle(Form("%s",fRunNumber.Data()));
                tempJetPtMax->Sumw2();
                if(notScaled) tempJetPtMax->Scale(1 / tempJetPtMax->GetEntries());
                TH1 *rebinnedJetPtMax = tempJetPtMax->Rebin(nXbins-1,"rebinnedJetPtMax",xbins);
                rebinnedJetPtMax->Scale(1.,"width");
                rebinnedJetPtMax->SetDirectory(0);
                vecJetPtMax[i].push_back((TH1D*)rebinnedJetPtMax);
                //tempJetPtMax->SetDirectory(0);
                //vecJetPtMax[i].push_back(tempJetPtMax);
                if(i==0) mapJetPtMaxRatio[j]=bin;
                else if( (mapJetPtMaxRatio.find(j) != mapJetPtMaxRatio.end()) && mapJetPtMaxRatio[j]>=0 && j<(Int_t)vecJetPtMax[0].size() ){
                    TH1D* tempJetPtMaxRatio   = new TH1D(*((TH1D*)rebinnedJetPtMax));
                    tempJetPtMaxRatio->Divide(tempJetPtMaxRatio,vecJetPtMax[0].at(j),1,1,"B");
                    tempJetPtMaxRatio->SetName(Form("%s_z-JetPtMaxRatio_%s",fRunNumber.Data(),fDataSet.Data()));
                    tempJetPtMaxRatio->GetXaxis()->SetRangeUser(-10,10);
                    tempJetPtMaxRatio->GetYaxis()->SetTitle("ratio");
                    tempJetPtMaxRatio->GetYaxis()->SetRangeUser(0,2);
                    vecJetPtMaxRatio[i-1].push_back(tempJetPtMaxRatio);
                }
            }else cout << "INFO: Object |Max Jet pT| could not be found! Skipping Fill..." << endl;

            if(jetType == "Full Jets"){
            //--------------------------------------------------------------------------------------------------------
            //------------------------- Neutral Energy Fraction ------------------------------------------------------
            //--------------------------------------------------------------------------------------------------------
            TH2D* NEFrac2D               = (TH2D*) HistoList->FindObject("hQANEFPt");
            std::vector<TH1D*> tempVecNEF;
            std::vector<TH1D*> tempVecNEFRatio;
            if(NEFrac2D){
                if(i < nData && nEvents > 0){
                    bool kBadNEF = true;
                    vecNEFQA.push_back("...");
                    for (Int_t binPt = 0; binPt < nPtBins; binPt++){
                        if(fTrigger == "INT7" && (binsPt[binPt] < 10 || binsPt[binPt+1] > 60 )) continue;
                        if(fTrigger == "EMC7" && (binsPt[binPt] < 20 || binsPt[binPt+1] > 100)) continue;
                        if(fTrigger == "EJE"  && (binsPt[binPt] < 30 || binsPt[binPt+1] > 350)) continue;

                        TH1D* NEFrac = (TH1D*)NEFrac2D->ProjectionY(Form("NEFraction_%d_%d", binPt, i),binsPt[binPt],binsPt[binPt+1]);
                        double meanNEFBin = NEFrac->GetMean();

                        if(meanNEFBin < 0.500000 && meanNEFBin > 0.100000) kBadNEF = false;
                        else vecNEFQA.push_back(Form("Period: %s -- Run Number: %s -- Events: %f -- pT Range: %i - %i GeV -- Mean NEF: %f", (vecDataSet.at(i)).Data(), fRunNumber.Data(), nEvents,binsPt[binPt],binsPt[binPt+1], meanNEFBin));

                    }
                    if(kBadNEF) vecNEFQA.push_back(Form("//////////////////// Period: %s -- Run Number: %s -- Events: %f -- NEF out of range for entire relevant pT range!!! ////////////////////", (vecDataSet.at(i)).Data(), fRunNumber.Data(), nEvents));
                }

                for (Int_t binPt = 0; binPt < nPtBins; binPt++){
                    TH1D* NEFrac = (TH1D*)NEFrac2D->ProjectionY(Form("NEFraction_%d_%d", binPt, i),binsPt[binPt],binsPt[binPt+1]);
                    hNEF_Mean[i][binPt]->SetBinContent(bin, NEFrac->GetMean());
                    hNEF_Mean[i][binPt]->SetBinError(bin, NEFrac->GetMeanError());
                    hNEF_RMS[i][binPt]->SetBinContent(bin, NEFrac->GetRMS());
                    hNEF_RMS[i][binPt]->SetBinError(bin, NEFrac->GetRMSError());
                    TH1D* tempNEF       = new TH1D(*NEFrac);
                    tempNEF->GetXaxis()->SetTitle("NEF");
                    tempNEF->GetYaxis()->SetTitle("#frac{1}{N_{Events}} #frac{dN_{Jet}}{dp_{t,jet}}");
                    tempNEF->SetTitle(Form("%s",fRunNumber.Data()));
                    tempNEF->Sumw2();
                    if(notScaled) tempNEF->Scale(1 / tempNEF->GetEntries());
                    tempNEF->SetDirectory(0);
                    tempVecNEF.push_back(tempNEF);
                }
                vecNEF[i].push_back(tempVecNEF);

                if(i==0) mapNEFRatio[j]=bin;
                else if( (mapNEFRatio.find(j) != mapNEFRatio.end()) && mapNEFRatio[j]>=0 && j<(Int_t)vecNEF[0].size() ){
                    for (Int_t binPt = 0; binPt < nPtBins; binPt++){
                        TH1D* tempNEFRatio   = new TH1D(*tempVecNEF.at(binPt));
                        tempNEFRatio->Divide(tempNEFRatio,vecNEF[0].at(j).at(binPt),1,1,"B");
                        tempNEFRatio->SetName(Form("%s_NeutralEnergyFraction_%s_%i",fRunNumber.Data(),fDataSet.Data(),binPt));
                        tempNEFRatio->GetXaxis()->SetRangeUser(-10,10);
                        tempNEFRatio->GetYaxis()->SetTitle("ratio");
                        tempNEFRatio->GetYaxis()->SetRangeUser(0,2);
                        tempVecNEFRatio.push_back(tempNEFRatio);
                    }
                }
                if( (i>0) && (mapNEFRatio.find(j) != mapNEFRatio.end()) && mapNEFRatio[j]>=0 && j<(Int_t)vecNEF[0].size() ){
                    vecNEFRatio[i-1].push_back(tempVecNEFRatio);
                }
                tempVecNEF.clear();
                tempVecNEFRatio.clear();

            }else cout << "INFO: Object |Neutral E-Frac| could not be found! Skipping Fill..." << endl;

            //--------------------------------------------------------------------------------------------------------
            //------------------------- Number of Charged Constituents -----------------------------------------------
            //--------------------------------------------------------------------------------------------------------
            TH2D* Nch2D               = (TH2D*) HistoList->FindObject("hQANChPt");
            std::vector<TH1D*> tempVecNch;
            std::vector<TH1D*> tempVecNchRatio;
            if(Nch2D){

                if(i < nData && nEvents > 0){
                    vecNoCharged.push_back("...");
                    double nCharged = (Nch2D->ProjectionY("NchQA_INT7",1,Nch2D->GetNbinsX()))->Integral();
                    if((nCharged/nEvents) < 0.01) vecNoCharged.push_back(Form("//////////////////// Period: %s -- Run Number: %s -- Events: %f -- Nch: %f -- Nch/nEvt < 0.01!!! ////////////////////", (vecDataSet.at(i)).Data(), fRunNumber.Data(), nEvents, nCharged));
                }

                TH1D *NchAll = (TH1D*)Nch2D->ProjectionY("NchAll",6,350);
                hMeanNch[i]->SetBinContent(bin, NchAll->GetMean());
                hMeanNch[i]->SetBinError(bin, NchAll->GetMeanError());
                if(NchAll->Integral() < 5){
                    cout << "//////////////// Nch = " << NchAll->Integral() << " for run " << fRunNumber.Data() << " of period " <<  (vecDataSet.at(i)).Data() << "! ////////////////" << endl;
                    cout << "NEvents = " << nEvents << endl;
                }
                sNchMean.mean += NchAll->GetMean();
                MeanNch->Fill(NchAll->GetMean(),NchAll->GetRMS());
                TH1D* tempNchAll       = new TH1D(*NchAll);
                tempNchAll->GetXaxis()->SetTitle("N_{ch}");
                tempNchAll->GetYaxis()->SetTitle("#frac{1}{N_{Events}} #frac{dN_{Jet}}{dp_{t,jet}}");
                tempNchAll->SetTitle(Form("%s",fRunNumber.Data()));
                tempNchAll->Sumw2();
                if(notScaled) tempNchAll->Scale(1 / tempNchAll->GetEntries());
                tempNchAll->SetDirectory(0);
                vecNchAll[i].push_back(tempNchAll);
                for (Int_t binPt = 0; binPt < nPtBins; binPt++){
                    TH1D* Nch = (TH1D*)Nch2D->ProjectionY(Form("Ncharged_%d_%d", binPt, i),binsPt[binPt],binsPt[binPt+1]);
                    hNch_Mean[i][binPt]->SetBinContent(bin, Nch->GetMean());
                    hNch_Mean[i][binPt]->SetBinError(bin, Nch->GetMeanError());
                    hNch_RMS[i][binPt]->SetBinContent(bin, Nch->GetRMS());
                    hNch_RMS[i][binPt]->SetBinError(bin, Nch->GetRMSError());
                    TH1D* tempNch       = new TH1D(*Nch);
                    tempNch->GetXaxis()->SetTitle("N_{ch}");
                    tempNch->GetYaxis()->SetTitle("#frac{1}{N_{Events}} #frac{dN_{Jet}}{dp_{t,jet}}");
                    tempNch->SetTitle(Form("%s",fRunNumber.Data()));
                    tempNch->Sumw2();
                    if(notScaled) tempNch->Scale(1 / tempNch->GetEntries());
                    tempNch->SetDirectory(0);
                    tempVecNch.push_back(tempNch);
                }
                vecNch[i].push_back(tempVecNch);

                if(i==0) mapNchAllRatio[j]=bin;
                else if( (mapNchAllRatio.find(j) != mapNchAllRatio.end()) && mapNchAllRatio[j]>=0 && j<(Int_t)vecJetPtMax[0].size() ){
                    TH1D* tempNchAllRatio   = new TH1D(*tempNchAll);
                    tempNchAllRatio->Divide(tempNchAllRatio,vecNchAll[0].at(j),1,1,"B");
                    tempNchAllRatio->SetName(Form("%s_NchAllRatio_%s",fRunNumber.Data(),fDataSet.Data()));
                    tempNchAllRatio->GetXaxis()->SetRangeUser(-10,10);
                    tempNchAllRatio->GetYaxis()->SetTitle("ratio");
                    tempNchAllRatio->GetYaxis()->SetRangeUser(0,2);
                    vecNchAllRatio[i-1].push_back(tempNchAllRatio);
                }

                if(i==0) mapNchRatio[j]=bin;
                else if( (mapNchRatio.find(j) != mapNchRatio.end()) && mapNchRatio[j]>=0 && j<(Int_t)vecNch[0].size() ){
                    for (Int_t binPt = 0; binPt < nPtBins; binPt++){
                        TH1D* tempNchRatio   = new TH1D(*tempVecNch.at(binPt));
                        tempNchRatio->Divide(tempNchRatio,vecNch[0].at(j).at(binPt),1,1,"B");
                        tempNchRatio->SetName(Form("%s_NChargedConstituents_%s_%i",fRunNumber.Data(),fDataSet.Data(),binPt));
                        tempNchRatio->GetXaxis()->SetRangeUser(-10,10);
                        tempNchRatio->GetYaxis()->SetTitle("ratio");
                        tempNchRatio->GetYaxis()->SetRangeUser(0,2);
                        tempVecNchRatio.push_back(tempNchRatio);
                    }
                }

                if( (i>0) && (mapNchRatio.find(j) != mapNchRatio.end()) && mapNchRatio[j]>=0 && j<(Int_t)vecNch[0].size() ){
                    vecNchRatio[i-1].push_back(tempVecNchRatio);
                }
                tempVecNch.clear();
                tempVecNchRatio.clear();

            }else cout << "INFO: Object |N Charged Constituents| could not be found! Skipping Fill..." << endl;

            //--------------------------------------------------------------------------------------------------------
            //------------------------- Number of Neutral Constituents -----------------------------------------------
            //--------------------------------------------------------------------------------------------------------
            TH2D* Nne2D               = (TH2D*) HistoList->FindObject("hQANnePt");
            std::vector<TH1D*> tempVecNne;
            std::vector<TH1D*> tempVecNneRatio;

            if(Nne2D){

                if(i < nData && nEvents > 0){
                    vecNoNeutral.push_back("...");
                    double nNeutral = (Nne2D->ProjectionY("NneQA_INT7",1,Nne2D->GetNbinsX()))->Integral();
                    if((nNeutral/nEvents) < 0.01) vecNoNeutral.push_back(Form("//////////////////// Period: %s -- Run Number: %s -- Events: %f -- Nne: %f -- Nne/nEvt < 0.01!!! ////////////////////", (vecDataSet.at(i)).Data(), fRunNumber.Data(), nEvents, nNeutral));
                }

                TH1D *NneAll = (TH1D*)Nne2D->ProjectionY("NneAll",6,350);
                if(NneAll->Integral() < 5){
                    cout << "//////////////// Nne = " << NneAll->Integral() << " for run " << fRunNumber.Data() << " of period " <<  (vecDataSet.at(i)).Data() << "! ////////////////" << endl;
                    cout << "NEvents = " << nEvents << endl;
                }
                for (Int_t binPt = 0; binPt < nPtBins; binPt++){
                    TH1D* Nne = (TH1D*)Nne2D->ProjectionY(Form("Nneutral_%d_%d", binPt, i),binsPt[binPt],binsPt[binPt+1]);
                    hNne_Mean[i][binPt]->SetBinContent(bin, Nne->GetMean());
                    hNne_Mean[i][binPt]->SetBinError(bin, Nne->GetMeanError());
                    hNne_RMS[i][binPt]->SetBinContent(bin, Nne->GetRMS());
                    hNne_RMS[i][binPt]->SetBinError(bin, Nne->GetRMSError());
                    TH1D* tempNne       = new TH1D(*Nne);
                    tempNne->GetXaxis()->SetTitle("N_{ne}");
                    tempNne->GetYaxis()->SetTitle("#frac{1}{N_{Events}} #frac{dN_{Jet}}{dp_{t,jet}}");
                    tempNne->SetTitle(Form("%s",fRunNumber.Data()));
                    tempNne->Sumw2();
                    if(notScaled) tempNne->Scale(1 / tempNne->GetEntries());
                    tempNne->SetDirectory(0);
                    tempVecNne.push_back(tempNne);
                }
                vecNne[i].push_back(tempVecNne);

                if(i==0) mapNneRatio[j]=bin;
                else if( (mapNneRatio.find(j) != mapNneRatio.end()) && mapNneRatio[j]>=0 && j<(Int_t)vecNne[0].size() ){
                    for (Int_t binPt = 0; binPt < nPtBins; binPt++){
                        TH1D* tempNneRatio   = new TH1D(*tempVecNne.at(binPt));
                        tempNneRatio->Divide(tempNneRatio,vecNne[0].at(j).at(binPt),1,1,"B");
                        tempNneRatio->SetName(Form("%s_NNeutralConstituents_%s_%i",fRunNumber.Data(),fDataSet.Data(),binPt));
                        tempNneRatio->GetXaxis()->SetRangeUser(-10,10);
                        tempNneRatio->GetYaxis()->SetTitle("ratio");
                        tempNneRatio->GetYaxis()->SetRangeUser(0,2);
                        tempVecNneRatio.push_back(tempNneRatio);
                    }
                }

                if( (i>0) && (mapNneRatio.find(j) != mapNneRatio.end()) && mapNneRatio[j]>=0 && j<(Int_t)vecNne[0].size() ){
                    vecNneRatio[i-1].push_back(tempVecNneRatio);
                }

                tempVecNne.clear();
                tempVecNneRatio.clear();

            }else cout << "INFO: Object |N Neutral Constituents| could not be found! Skipping Fill..." << endl;

            //--------------------------------------------------------------------------------------------------------
            //------------------------- Pt of Charged Constituents ---------------------------------------------------
            //--------------------------------------------------------------------------------------------------------
            TH2D* PtCh2D               = (TH2D*) HistoList->FindObject("hQAConstPtCh");
            std::vector<TH1D*> tempVecPtCh;
            std::vector<TH1D*> tempVecPtChRatio;
            if(PtCh2D){
                //TH1D *PtChAll = (TH1D*)PtCh2D->ProjectionY();
                TH1D *PtChAll = (TH1D*)PtCh2D->ProjectionY("PtChAll",6,350);
                hMeanPtCh[i]->SetBinContent(bin, PtChAll->GetMean());
                hMeanPtCh[i]->SetBinError(bin, PtChAll->GetMeanError());
                sPtChMean.mean +=  PtChAll->GetMean();
                MeanPtCh->Fill(PtChAll->GetMean(),PtChAll->GetRMS());
                TH1D* tempPtChAll       = new TH1D(*PtChAll);
                tempPtChAll->GetXaxis()->SetTitle("p_{T,ch}");
                tempPtChAll->GetYaxis()->SetTitle("#frac{1}{N_{Events}} #frac{dN_{Jet}}{dp_{T,jet}}");
                tempPtChAll->SetTitle(Form("%s",fRunNumber.Data()));
                tempPtChAll->Sumw2();
                if(notScaled) tempPtChAll->Scale(1 / tempPtChAll->GetEntries());
                tempPtChAll->SetDirectory(0);
                vecPtChAll[i].push_back(tempPtChAll);
                for (Int_t binPt = 0; binPt < nPtBins; binPt++){
                    TH1D* PtCh = (TH1D*)PtCh2D->ProjectionY(Form("PtCharged_%d_%d", binPt, i),binsPt[binPt],binsPt[binPt+1]);
                    hPtCh_Mean[i][binPt]->SetBinContent(bin, PtCh->GetMean());
                    hPtCh_Mean[i][binPt]->SetBinError(bin, PtCh->GetMeanError());
                    hPtCh_RMS[i][binPt]->SetBinContent(bin, PtCh->GetRMS());
                    hPtCh_RMS[i][binPt]->SetBinError(bin, PtCh->GetRMSError());
                    TH1D* tempPtCh       = new TH1D(*PtCh);
                    tempPtCh->GetXaxis()->SetTitle("p_{T,ch}");
                    tempPtCh->GetYaxis()->SetTitle("#frac{1}{N_{Events}} #frac{dN_{Jet}}{dp_{t,jet}}");
                    tempPtCh->SetTitle(Form("%s",fRunNumber.Data()));
                    tempPtCh->Sumw2();
                    if(notScaled) tempPtCh->Scale(1 / tempPtCh->GetEntries());
                    tempPtCh->SetDirectory(0);
                    tempVecPtCh.push_back(tempPtCh);
                }
                vecPtCh[i].push_back(tempVecPtCh);

                if(i==0) mapPtChAllRatio[j]=bin;
                else if( (mapPtChAllRatio.find(j) != mapPtChAllRatio.end()) && mapPtChAllRatio[j]>=0 && j<(Int_t)vecJetPtMax[0].size() ){
                    TH1D* tempPtChAllRatio   = new TH1D(*tempPtChAll);
                    tempPtChAllRatio->Divide(tempPtChAllRatio,vecPtChAll[0].at(j),1,1,"B");
                    tempPtChAllRatio->SetName(Form("%s_PtChAllRatio_%s",fRunNumber.Data(),fDataSet.Data()));
                    tempPtChAllRatio->GetXaxis()->SetRangeUser(-10,10);
                    tempPtChAllRatio->GetYaxis()->SetTitle("ratio");
                    tempPtChAllRatio->GetYaxis()->SetRangeUser(0,2);
                    vecPtChAllRatio[i-1].push_back(tempPtChAllRatio);
                }

                if(i==0) mapPtChRatio[j]=bin;
                else if( (mapPtChRatio.find(j) != mapPtChRatio.end()) && mapPtChRatio[j]>=0 && j<(Int_t)vecPtCh[0].size() ){
                    for (Int_t binPt = 0; binPt < nPtBins; binPt++){
                        TH1D* tempPtChRatio   = new TH1D(*tempVecPtCh.at(binPt));
                        tempPtChRatio->Divide(tempPtChRatio,vecPtCh[0].at(j).at(binPt),1,1,"B");
                        tempPtChRatio->SetName(Form("%s_PtChargedConstituents_%s_%i",fRunNumber.Data(),fDataSet.Data(),binPt));
                        tempPtChRatio->GetXaxis()->SetRangeUser(-10,10);
                        tempPtChRatio->GetYaxis()->SetTitle("ratio");
                        tempPtChRatio->GetYaxis()->SetRangeUser(0,2);
                        tempVecPtChRatio.push_back(tempPtChRatio);
                    }
                }
                if( (i>0) && (mapPtChRatio.find(j) != mapPtChRatio.end()) && mapPtChRatio[j]>=0 && j<(Int_t)vecPtCh[0].size() ){
                    vecPtChRatio[i-1].push_back(tempVecPtChRatio);
                }
                tempVecPtCh.clear();
                tempVecPtChRatio.clear();

            }else cout << "INFO: Object |Pt Charged Constituents| could not be found! Skipping Fill..." << endl;

            //--------------------------------------------------------------------------------------------------------
            //------------------------- Pt of Neutral Constituents ---------------------------------------------------
            //--------------------------------------------------------------------------------------------------------
            TH2D* PtNe2D               = (TH2D*) HistoList->FindObject("hQAConstPtNe");
            std::vector<TH1D*> tempVecPtNe;
            std::vector<TH1D*> tempVecPtNeRatio;
            if(PtNe2D){
                for (Int_t binPt = 0; binPt < nPtBins; binPt++){
                    TH1D* PtNe = (TH1D*)PtNe2D->ProjectionY(Form("PtNeutral_%d_%d", binPt, i),binsPt[binPt],binsPt[binPt+1]);
                    hPtNe_Mean[i][binPt]->SetBinContent(bin, PtNe->GetMean());
                    hPtNe_Mean[i][binPt]->SetBinError(bin, PtNe->GetMeanError());
                    hPtNe_RMS[i][binPt]->SetBinContent(bin, PtNe->GetRMS());
                    hPtNe_RMS[i][binPt]->SetBinError(bin, PtNe->GetRMSError());
                    TH1D* tempPtNe       = new TH1D(*PtNe);
                    tempPtNe->GetXaxis()->SetTitle("p_{T,ne}");
                    tempPtNe->GetYaxis()->SetTitle("#frac{1}{N_{Events}} #frac{dN_{Jet}}{dp_{t,jet}}");
                    tempPtNe->SetTitle(Form("%s",fRunNumber.Data()));
                    tempPtNe->Sumw2();
                    if(notScaled) tempPtNe->Scale(1 / tempPtNe->GetEntries());
                    tempPtNe->SetDirectory(0);
                    tempVecPtNe.push_back(tempPtNe);
                }
                vecPtNe[i].push_back(tempVecPtNe);

                if(i==0) mapPtNeRatio[j]=bin;
                else if( (mapPtNeRatio.find(j) != mapPtNeRatio.end()) && mapPtNeRatio[j]>=0 && j<(Int_t)vecPtNe[0].size() ){
                    for (Int_t binPt = 0; binPt < nPtBins; binPt++){
                        TH1D* tempPtNeRatio   = new TH1D(*tempVecPtNe.at(binPt));
                        tempPtNeRatio->Divide(tempPtNeRatio,vecPtNe[0].at(j).at(binPt),1,1,"B");
                        tempPtNeRatio->SetName(Form("%s_PtNeutralConstituents_%s_%i",fRunNumber.Data(),fDataSet.Data(),binPt));
                        tempPtNeRatio->GetXaxis()->SetRangeUser(-10,10);
                        tempPtNeRatio->GetYaxis()->SetTitle("ratio");
                        tempPtNeRatio->GetYaxis()->SetRangeUser(0,2);
                        tempVecPtNeRatio.push_back(tempPtNeRatio);
                    }
                }
                if( (i>0) && (mapPtNeRatio.find(j) != mapPtNeRatio.end()) && mapPtNeRatio[j]>=0 && j<(Int_t)vecPtNe[0].size() ){
                    vecPtNeRatio[i-1].push_back(tempVecPtNeRatio);
                }
                tempVecPtNe.clear();
                tempVecPtNeRatio.clear();

            }else cout << "INFO: Object |Pt Neutral Constituents| could not be found! Skipping Fill..." << endl;

            //--------------------------------------------------------------------------------------------------------
            //------------------------- Max Pt of Charged Constituents -----------------------------------------------
            //--------------------------------------------------------------------------------------------------------
            TH2D* PtChMax2D               = (TH2D*) HistoList->FindObject("hQAConstPtChMax");
            std::vector<TH1D*> tempVecPtChMax;
            std::vector<TH1D*> tempVecPtChMaxRatio;
            if(PtChMax2D){
                for (Int_t binPt = 0; binPt < nPtBins; binPt++){
                    TH1D* PtChMax = (TH1D*)PtChMax2D->ProjectionY(Form("MaxPtCharged_%d_%d", binPt, i),binsPt[binPt],binsPt[binPt+1]);
                    hPtChMax_Mean[i][binPt]->SetBinContent(bin, PtChMax->GetMean());
                    hPtChMax_Mean[i][binPt]->SetBinError(bin, PtChMax->GetMeanError());
                    hPtChMax_RMS[i][binPt]->SetBinContent(bin, PtChMax->GetRMS());
                    hPtChMax_RMS[i][binPt]->SetBinError(bin, PtChMax->GetRMSError());
                    TH1D* tempPtChMax       = new TH1D(*PtChMax);
                    tempPtChMax->GetXaxis()->SetTitle("p_{T,ch}");
                    tempPtChMax->GetYaxis()->SetTitle("#frac{1}{N_{Events}} #frac{dN_{Jet}}{dp_{t,jet}}");
                    tempPtChMax->SetTitle(Form("%s",fRunNumber.Data()));
                    tempPtChMax->Sumw2();
                    if(notScaled) tempPtChMax->Scale(1 / tempPtChMax->GetEntries());
                    tempPtChMax->SetDirectory(0);
                    tempVecPtChMax.push_back(tempPtChMax);
                }
                vecPtChMax[i].push_back(tempVecPtChMax);

                if(i==0) mapPtChMaxRatio[j]=bin;
                else if( (mapPtChMaxRatio.find(j) != mapPtChMaxRatio.end()) && mapPtChMaxRatio[j]>=0 && j<(Int_t)vecPtChMax[0].size() ){
                    for (Int_t binPt = 0; binPt < nPtBins; binPt++){
                        TH1D* tempPtChMaxRatio   = new TH1D(*tempVecPtChMax.at(binPt));
                        tempPtChMaxRatio->Divide(tempPtChMaxRatio,vecPtChMax[0].at(j).at(binPt),1,1,"B");
                        tempPtChMaxRatio->SetName(Form("%s_MaxPtChargedConstituents_%s_%i",fRunNumber.Data(),fDataSet.Data(),binPt));
                        tempPtChMaxRatio->GetXaxis()->SetRangeUser(-10,10);
                        tempPtChMaxRatio->GetYaxis()->SetTitle("ratio");
                        tempPtChMaxRatio->GetYaxis()->SetRangeUser(0,2);
                        tempVecPtChMaxRatio.push_back(tempPtChMaxRatio);
                    }
                }
                if( (i>0) && (mapPtChMaxRatio.find(j) != mapPtChMaxRatio.end()) && mapPtChMaxRatio[j]>=0 && j<(Int_t)vecPtChMax[0].size() ){
                    vecPtChMaxRatio[i-1].push_back(tempVecPtChMaxRatio);
                }
                tempVecPtChMax.clear();
                tempVecPtChMaxRatio.clear();

            }else cout << "INFO: Object |Max Pt Charged Constituents| could not be found! Skipping Fill..." << endl;

            //--------------------------------------------------------------------------------------------------------
            //------------------------- Max Pt of Neutral Constituents -----------------------------------------------
            //--------------------------------------------------------------------------------------------------------
            TH2D* PtNeMax2D               = (TH2D*) HistoList->FindObject("hQAConstPtNeMax");
            std::vector<TH1D*> tempVecPtNeMax;
            std::vector<TH1D*> tempVecPtNeMaxRatio;
            if(PtNeMax2D){
                for (Int_t binPt = 0; binPt < nPtBins; binPt++){
                    TH1D* PtNeMax = (TH1D*)PtNeMax2D->ProjectionY(Form("MaxPtNeutral_%d_%d", binPt, i),binsPt[binPt],binsPt[binPt+1]);
                    hPtNeMax_Mean[i][binPt]->SetBinContent(bin, PtNeMax->GetMean());
                    hPtNeMax_Mean[i][binPt]->SetBinError(bin, PtNeMax->GetMeanError());
                    hPtNeMax_RMS[i][binPt]->SetBinContent(bin, PtNeMax->GetRMS());
                    hPtNeMax_RMS[i][binPt]->SetBinError(bin, PtNeMax->GetRMSError());
                    TH1D* tempPtNeMax       = new TH1D(*PtNeMax);
                    tempPtNeMax->GetXaxis()->SetTitle("p_{T,ne}");
                    tempPtNeMax->GetYaxis()->SetTitle("#frac{1}{N_{Events}} #frac{dN_{Jet}}{dp_{t,jet}}");
                    tempPtNeMax->SetTitle(Form("%s",fRunNumber.Data()));
                    tempPtNeMax->Sumw2();
                    if(notScaled) tempPtNeMax->Scale(1 / tempPtNeMax->GetEntries());
                    tempPtNeMax->SetDirectory(0);
                    tempVecPtNeMax.push_back(tempPtNeMax);
                }
                vecPtNeMax[i].push_back(tempVecPtNeMax);

                if(i==0) mapPtNeMaxRatio[j]=bin;
                else if( (mapPtNeMaxRatio.find(j) != mapPtNeMaxRatio.end()) && mapPtNeMaxRatio[j]>=0 && j<(Int_t)vecPtNeMax[0].size() ){
                    for (Int_t binPt = 0; binPt < nPtBins; binPt++){
                        TH1D* tempPtNeMaxRatio   = new TH1D(*tempVecPtNeMax.at(binPt));
                        tempPtNeMaxRatio->Divide(tempPtNeMaxRatio,vecPtNeMax[0].at(j).at(binPt),1,1,"B");
                        tempPtNeMaxRatio->SetName(Form("%s_MaxPtNeutralConstituents_%s_%i",fRunNumber.Data(),fDataSet.Data(),binPt));
                        tempPtNeMaxRatio->GetXaxis()->SetRangeUser(-10,10);
                        tempPtNeMaxRatio->GetYaxis()->SetTitle("ratio");
                        tempPtNeMaxRatio->GetYaxis()->SetRangeUser(0,2);
                        tempVecPtNeMaxRatio.push_back(tempPtNeMaxRatio);
                    }
                }
                if( (i>0) && (mapPtNeMaxRatio.find(j) != mapPtNeMaxRatio.end()) && mapPtNeMaxRatio[j]>=0 && j<(Int_t)vecPtNeMax[0].size() ){
                    vecPtNeMaxRatio[i-1].push_back(tempVecPtNeMaxRatio);
                }
                tempVecPtNeMax.clear();
                tempVecPtNeMaxRatio.clear();

            }else cout << "INFO: Object |Max Pt Neutral Constituents| could not be found! Skipping Fill..." << endl;
            }
            //--------------------------------------------------------------------------------------------------------
            //------------------------- Eta-Phi Distribution ---------------------------------------------------------
            //--------------------------------------------------------------------------------------------------------
            TH2D* EtaPhi2D               = (TH2D*) HistoList->FindObject("hQAEtaPhi");
            std::vector<TH1D*> tempVecEtaPhi;
            std::vector<TH1D*> tempVecEtaPhiRatio;
            if(EtaPhi2D){
                for (Int_t binEta = 0; binEta < nEtaBins; binEta++){
                    TH1D* EtaPhi = (TH1D*)EtaPhi2D->ProjectionY(Form("EtaPhi_%i_%i", binEta, i),binsEtaInt[binEta],binsEtaInt[binEta+1]);
                    hEtaPhi_Mean[i][binEta]->SetBinContent(bin, EtaPhi->GetMean());
                    hEtaPhi_Mean[i][binEta]->SetBinError(bin, EtaPhi->GetMeanError());
                    hEtaPhi_RMS[i][binEta]->SetBinContent(bin, EtaPhi->GetRMS());
                    hEtaPhi_RMS[i][binEta]->SetBinError(bin, EtaPhi->GetRMSError());
                    TH1D* tempEtaPhi       = new TH1D(*EtaPhi);
                    tempEtaPhi->GetXaxis()->SetTitle("#phi");
                    tempEtaPhi->GetYaxis()->SetTitle("#frac{1}{N_{Events}} #frac{dN_{Jet}}{d#eta}");
                    tempEtaPhi->SetTitle(Form("%s",fRunNumber.Data()));
                    tempEtaPhi->Sumw2();
                    if(notScaled) tempEtaPhi->Scale(1 / tempEtaPhi->GetEntries());
                    tempEtaPhi->SetDirectory(0);
                    tempVecEtaPhi.push_back(tempEtaPhi);
                }
                vecEtaPhi[i].push_back(tempVecEtaPhi);

                if(i==0) mapEtaPhiRatio[j]=bin;
                else if( (mapEtaPhiRatio.find(j) != mapEtaPhiRatio.end()) && mapEtaPhiRatio[j]>=0 && j<(Int_t)vecEtaPhi[0].size() ){
                    for (Int_t binEta = 0; binEta < nEtaBins; binEta++){
                        TH1D* tempEtaPhiRatio   = new TH1D(*tempVecEtaPhi.at(binEta));
                        tempEtaPhiRatio->Divide(tempEtaPhiRatio,vecEtaPhi[0].at(j).at(binEta),1,1,"B");
                        tempEtaPhiRatio->SetName(Form("%s_EtaPhiDistribution_%s_%i",fRunNumber.Data(),fDataSet.Data(),binEta));
                        tempEtaPhiRatio->GetXaxis()->SetRangeUser(-10,10);
                        tempEtaPhiRatio->GetYaxis()->SetTitle("ratio");
                        tempEtaPhiRatio->GetYaxis()->SetRangeUser(0,2);
                        tempVecEtaPhiRatio.push_back(tempEtaPhiRatio);
                    }
                }
                if( (i>0) && (mapEtaPhiRatio.find(j) != mapEtaPhiRatio.end()) && mapEtaPhiRatio[j]>=0 && j<(Int_t)vecEtaPhi[0].size() ){
                    vecEtaPhiRatio[i-1].push_back(tempVecEtaPhiRatio);
                }
                tempVecEtaPhi.clear();
                tempVecEtaPhiRatio.clear();

            }else cout << "INFO: Object |Eta-Phi Distribution| could not be found! Skipping Fill..." << endl;

            if(jetType == "Full Jets"){
            //--------------------------------------------------------------------------------------------------------
            //------------------------- Eta-Phi Distribution for Charged Constituents---------------------------------
            //--------------------------------------------------------------------------------------------------------
            TH2D* EtaPhiCh2D               = (TH2D*) HistoList->FindObject("hQAEtaPhiConstCh");
            std::vector<TH1D*> tempVecEtaPhiCh;
            std::vector<TH1D*> tempVecEtaPhiChRatio;
            if(EtaPhiCh2D){
                for (Int_t binEta = 0; binEta < nEtaBins; binEta++){
                    TH1D* EtaPhiCh = (TH1D*)EtaPhiCh2D->ProjectionY(Form("EtaPhiCh_%i_%i", binEta, i),binsEtaInt[binEta],binsEtaInt[binEta+1]);
                    hEtaPhiCh_Mean[i][binEta]->SetBinContent(bin, EtaPhiCh->GetMean());
                    hEtaPhiCh_Mean[i][binEta]->SetBinError(bin, EtaPhiCh->GetMeanError());
                    hEtaPhiCh_RMS[i][binEta]->SetBinContent(bin, EtaPhiCh->GetRMS());
                    hEtaPhiCh_RMS[i][binEta]->SetBinError(bin, EtaPhiCh->GetRMSError());
                    TH1D* tempEtaPhiCh       = new TH1D(*EtaPhiCh);
                    tempEtaPhiCh->GetXaxis()->SetTitle("#phi");
                    tempEtaPhiCh->GetYaxis()->SetTitle("#frac{1}{N_{Events}} #frac{dN_{Jet}}{d#eta}");
                    tempEtaPhiCh->SetTitle(Form("%s",fRunNumber.Data()));
                    tempEtaPhiCh->Sumw2();
                    if(notScaled) tempEtaPhiCh->Scale(1 / tempEtaPhiCh->GetEntries());
                    tempEtaPhiCh->SetDirectory(0);
                    tempVecEtaPhiCh.push_back(tempEtaPhiCh);
                }
                vecEtaPhiCh[i].push_back(tempVecEtaPhiCh);

                if(i==0) mapEtaPhiChRatio[j]=bin;
                else if( (mapEtaPhiChRatio.find(j) != mapEtaPhiChRatio.end()) && mapEtaPhiChRatio[j]>=0 && j<(Int_t)vecEtaPhiCh[0].size() ){
                    for (Int_t binEta = 0; binEta < nEtaBins; binEta++){
                        TH1D* tempEtaPhiChRatio   = new TH1D(*tempVecEtaPhiCh.at(binEta));
                        tempEtaPhiChRatio->Divide(tempEtaPhiChRatio,vecEtaPhiCh[0].at(j).at(binEta),1,1,"B");
                        tempEtaPhiChRatio->SetName(Form("%s_EtaPhiChDistribution_%s_%i",fRunNumber.Data(),fDataSet.Data(),binEta));
                        tempEtaPhiChRatio->GetXaxis()->SetRangeUser(-10,10);
                        tempEtaPhiChRatio->GetYaxis()->SetTitle("ratio");
                        tempEtaPhiChRatio->GetYaxis()->SetRangeUser(0,2);
                        tempVecEtaPhiChRatio.push_back(tempEtaPhiChRatio);
                    }
                }
                if( (i>0) && (mapEtaPhiChRatio.find(j) != mapEtaPhiChRatio.end()) && mapEtaPhiChRatio[j]>=0 && j<(Int_t)vecEtaPhiCh[0].size() ){
                    vecEtaPhiChRatio[i-1].push_back(tempVecEtaPhiChRatio);
                }
                tempVecEtaPhiCh.clear();
                tempVecEtaPhiChRatio.clear();

            }else cout << "INFO: Object |Eta-Phi Charged Distribution| could not be found! Skipping Fill..." << endl;

            //--------------------------------------------------------------------------------------------------------
            //------------------------- Eta-Phi Distribution for Neutral Constituents --------------------------------
            //--------------------------------------------------------------------------------------------------------
            TH2D* EtaPhiNe2D               = (TH2D*) HistoList->FindObject("hQAEtaPhiConstNe");
            std::vector<TH1D*> tempVecEtaPhiNe;
            std::vector<TH1D*> tempVecEtaPhiNeRatio;
            if(EtaPhiNe2D){
                for (Int_t binEta = 0; binEta < nEtaBins; binEta++){
                    TH1D* EtaPhiNe = (TH1D*)EtaPhiNe2D->ProjectionY(Form("EtaPhiNe_%i_%i", binEta, i),binsEtaInt[binEta],binsEtaInt[binEta+1]);
                    hEtaPhiNe_Mean[i][binEta]->SetBinContent(bin, EtaPhiNe->GetMean());
                    hEtaPhiNe_Mean[i][binEta]->SetBinError(bin, EtaPhiNe->GetMeanError());
                    hEtaPhiNe_RMS[i][binEta]->SetBinContent(bin, EtaPhiNe->GetRMS());
                    hEtaPhiNe_RMS[i][binEta]->SetBinError(bin, EtaPhiNe->GetRMSError());
                    TH1D* tempEtaPhiNe       = new TH1D(*EtaPhiNe);
                    tempEtaPhiNe->GetXaxis()->SetTitle("#phi");
                    tempEtaPhiNe->GetYaxis()->SetTitle("#frac{1}{N_{Events}} #frac{dN_{Jet}}{d#eta}");
                    tempEtaPhiNe->SetTitle(Form("%s",fRunNumber.Data()));
                    tempEtaPhiNe->Sumw2();
                    if(notScaled) tempEtaPhiNe->Scale(1 / tempEtaPhiNe->GetEntries());
                    tempEtaPhiNe->SetDirectory(0);
                    tempVecEtaPhiNe.push_back(tempEtaPhiNe);
                }
                vecEtaPhiNe[i].push_back(tempVecEtaPhiNe);

                if(i==0) mapEtaPhiNeRatio[j]=bin;
                else if( (mapEtaPhiNeRatio.find(j) != mapEtaPhiNeRatio.end()) && mapEtaPhiNeRatio[j]>=0 && j<(Int_t)vecEtaPhiNe[0].size() ){
                    for (Int_t binEta = 0; binEta < nEtaBins; binEta++){
                        TH1D* tempEtaPhiNeRatio   = new TH1D(*tempVecEtaPhiNe.at(binEta));
                        tempEtaPhiNeRatio->Divide(tempEtaPhiNeRatio,vecEtaPhiNe[0].at(j).at(binEta),1,1,"B");
                        tempEtaPhiNeRatio->SetName(Form("%s_EtaPhiNeDistribution_%s_%i",fRunNumber.Data(),fDataSet.Data(),binEta));
                        tempEtaPhiNeRatio->GetXaxis()->SetRangeUser(-10,10);
                        tempEtaPhiNeRatio->GetYaxis()->SetTitle("ratio");
                        tempEtaPhiNeRatio->GetYaxis()->SetRangeUser(0,2);
                        tempVecEtaPhiNeRatio.push_back(tempEtaPhiNeRatio);
                    }
                }
                if( (i>0) && (mapEtaPhiNeRatio.find(j) != mapEtaPhiNeRatio.end()) && mapEtaPhiNeRatio[j]>=0 && j<(Int_t)vecEtaPhiNe[0].size() ){
                    vecEtaPhiNeRatio[i-1].push_back(tempVecEtaPhiNeRatio);
                }
                tempVecEtaPhiNe.clear();
                tempVecEtaPhiNeRatio.clear();

            }else cout << "INFO: Object |Eta-Phi Neutral Distribution| could not be found! Skipping Fill..." << endl;

            //--------------------------------------------------------------------------------------------------------
            //------------------------- Eta-Phi Distribution for Max Charged Constituents ----------------------------
            //--------------------------------------------------------------------------------------------------------
            TH2D* EtaPhiChMax2D               = (TH2D*) HistoList->FindObject("hQAEtaPhiMaxConstCh");
            std::vector<TH1D*> tempVecEtaPhiChMax;
            std::vector<TH1D*> tempVecEtaPhiChMaxRatio;
            if(EtaPhiChMax2D){
                for (Int_t binEta = 0; binEta < nEtaBins; binEta++){
                    TH1D* EtaPhiChMax = (TH1D*)EtaPhiChMax2D->ProjectionY(Form("EtaPhiChMax_%i_%i", binEta, i),binsEtaInt[binEta],binsEtaInt[binEta+1]);
                    hEtaPhiChMax_Mean[i][binEta]->SetBinContent(bin, EtaPhiChMax->GetMean());
                    hEtaPhiChMax_Mean[i][binEta]->SetBinError(bin, EtaPhiChMax->GetMeanError());
                    hEtaPhiChMax_RMS[i][binEta]->SetBinContent(bin, EtaPhiChMax->GetRMS());
                    hEtaPhiChMax_RMS[i][binEta]->SetBinError(bin, EtaPhiChMax->GetRMSError());
                    TH1D* tempEtaPhiChMax       = new TH1D(*EtaPhiChMax);
                    tempEtaPhiChMax->GetXaxis()->SetTitle("#phi");
                    tempEtaPhiChMax->GetYaxis()->SetTitle("#frac{1}{N_{Events}} #frac{dN_{Jet}}{d#eta}");
                    tempEtaPhiChMax->SetTitle(Form("%s",fRunNumber.Data()));
                    tempEtaPhiChMax->Sumw2();
                    if(notScaled) tempEtaPhiChMax->Scale(1 / tempEtaPhiChMax->GetEntries());
                    tempEtaPhiChMax->SetDirectory(0);
                    tempVecEtaPhiChMax.push_back(tempEtaPhiChMax);
                }
                vecEtaPhiChMax[i].push_back(tempVecEtaPhiChMax);

                if(i==0) mapEtaPhiChMaxRatio[j]=bin;
                else if( (mapEtaPhiChMaxRatio.find(j) != mapEtaPhiChMaxRatio.end()) && mapEtaPhiChMaxRatio[j]>=0 && j<(Int_t)vecEtaPhiChMax[0].size() ){
                        for (Int_t binEta = 0; binEta < nEtaBins; binEta++){
                        TH1D* tempEtaPhiChMaxRatio   = new TH1D(*tempVecEtaPhiChMax.at(binEta));
                        tempEtaPhiChMaxRatio->Divide(tempEtaPhiChMaxRatio,vecEtaPhiChMax[0].at(j).at(binEta),1,1,"B");
                        tempEtaPhiChMaxRatio->SetName(Form("%s_EtaPhiChMaxDistribution_%s_%i",fRunNumber.Data(),fDataSet.Data(),binEta));
                        tempEtaPhiChMaxRatio->GetXaxis()->SetRangeUser(-10,10);
                        tempEtaPhiChMaxRatio->GetYaxis()->SetTitle("ratio");
                        tempEtaPhiChMaxRatio->GetYaxis()->SetRangeUser(0,2);
                        tempVecEtaPhiChMaxRatio.push_back(tempEtaPhiChMaxRatio);
                    }
                }
                if( (i>0) && (mapEtaPhiChMaxRatio.find(j) != mapEtaPhiChMaxRatio.end()) && mapEtaPhiChMaxRatio[j]>=0 && j<(Int_t)vecEtaPhiChMax[0].size() ){
                    vecEtaPhiChMaxRatio[i-1].push_back(tempVecEtaPhiChMaxRatio);
                }
                tempVecEtaPhiChMax.clear();
                tempVecEtaPhiChMaxRatio.clear();

            }else cout << "INFO: Object |Eta-Phi Max Charged Distribution| could not be found! Skipping Fill..." << endl;

            //--------------------------------------------------------------------------------------------------------
            //------------------------- Eta-Phi Distribution for Max Neutral Constituents ----------------------------
            //--------------------------------------------------------------------------------------------------------
            TH2D* EtaPhiNeMax2D               = (TH2D*) HistoList->FindObject("hQAEtaPhiMaxConstNe");
            std::vector<TH1D*> tempVecEtaPhiNeMax;
            std::vector<TH1D*> tempVecEtaPhiNeMaxRatio;
            if(EtaPhiNeMax2D){
                for (Int_t binEta = 0; binEta < nEtaBins; binEta++){
                    TH1D* EtaPhiNeMax = (TH1D*)EtaPhiNeMax2D->ProjectionY(Form("EtaPhiNeMax_%i_%i", binEta, i),binsEtaInt[binEta],binsEtaInt[binEta+1]);
                    hEtaPhiNeMax_Mean[i][binEta]->SetBinContent(bin, EtaPhiNeMax->GetMean());
                    hEtaPhiNeMax_Mean[i][binEta]->SetBinError(bin, EtaPhiNeMax->GetMeanError());
                    hEtaPhiNeMax_RMS[i][binEta]->SetBinContent(bin, EtaPhiNeMax->GetRMS());
                    hEtaPhiNeMax_RMS[i][binEta]->SetBinError(bin, EtaPhiNeMax->GetRMSError());
                    TH1D* tempEtaPhiNeMax       = new TH1D(*EtaPhiNeMax);
                    tempEtaPhiNeMax->GetXaxis()->SetTitle("#phi");
                    tempEtaPhiNeMax->GetYaxis()->SetTitle("#frac{1}{N_{Events}} #frac{dN_{Jet}}{d#eta}");
                    tempEtaPhiNeMax->SetTitle(Form("%s",fRunNumber.Data()));
                    tempEtaPhiNeMax->Sumw2();
                    if(notScaled) tempEtaPhiNeMax->Scale(1 / tempEtaPhiNeMax->GetEntries());
                    tempEtaPhiNeMax->SetDirectory(0);
                    tempVecEtaPhiNeMax.push_back(tempEtaPhiNeMax);
                }
                vecEtaPhiNeMax[i].push_back(tempVecEtaPhiNeMax);

                if(i==0) mapEtaPhiNeMaxRatio[j]=bin;
                else if( (mapEtaPhiNeMaxRatio.find(j) != mapEtaPhiNeMaxRatio.end()) && mapEtaPhiNeMaxRatio[j]>=0 && j<(Int_t)vecEtaPhiNeMax[0].size() ){
                    for (Int_t binEta = 0; binEta < nEtaBins; binEta++){
                        TH1D* tempEtaPhiNeMaxRatio   = new TH1D(*tempVecEtaPhiNeMax.at(binEta));
                        tempEtaPhiNeMaxRatio->Divide(tempEtaPhiNeMaxRatio,vecEtaPhiNeMax[0].at(j).at(binEta),1,1,"B");
                        tempEtaPhiNeMaxRatio->SetName(Form("%s_EtaPhiNeMaxDistribution_%s_%i",fRunNumber.Data(),fDataSet.Data(),binEta));
                        tempEtaPhiNeMaxRatio->GetXaxis()->SetRangeUser(-10,10);
                        tempEtaPhiNeMaxRatio->GetYaxis()->SetTitle("ratio");
                        tempEtaPhiNeMaxRatio->GetYaxis()->SetRangeUser(0,2);
                        tempVecEtaPhiNeMaxRatio.push_back(tempEtaPhiNeMaxRatio);
                    }

                }
                if( (i>0) && (mapEtaPhiNeMaxRatio.find(j) != mapEtaPhiNeMaxRatio.end()) && mapEtaPhiNeMaxRatio[j]>=0 && j<(Int_t)vecEtaPhiNeMax[0].size() ){
                    vecEtaPhiNeMaxRatio[i-1].push_back(tempVecEtaPhiNeMaxRatio);
                }
                tempVecEtaPhiNeMax.clear();
                tempVecEtaPhiNeMaxRatio.clear();

            }else cout << "INFO: Object |Eta-Phi Max Neutral Distribution| could not be found! Skipping Fill..." << endl;
            }
            //--------------------------------------------------------------------------------------------------------
            //------------------------- Cluster Time and Energy ------------------------------------------------------
            //--------------------------------------------------------------------------------------------------------
            TH2D* ClusterTimeVsE2D               = (TH2D*) HistoList->FindObject("hQAClusterTimeVsEFine");
            if(ClusterTimeVsE2D){
                TH1D* ClusterTime = (TH1D*)ClusterTimeVsE2D->ProjectionX("ClusterTime",1,200);
                TH1D* JetClusterEnergy = (TH1D*)ClusterTimeVsE2D->ProjectionY("JetClusterEnergy",401,575);
                hClusterTime_Mean[i]->SetBinContent(bin, ClusterTime->GetMean());
                hClusterTime_Mean[i]->SetBinError(bin, ClusterTime->GetMeanError());
                hClusterTime_RMS[i]->SetBinContent(bin, ClusterTime->GetRMS());
                hClusterTime_RMS[i]->SetBinError(bin, ClusterTime->GetRMSError());
                hJetClusterEnergy_Mean[i]->SetBinContent(bin, JetClusterEnergy->GetMean());
                hJetClusterEnergy_Mean[i]->SetBinError(bin, JetClusterEnergy->GetMeanError());
                hJetClusterEnergy_RMS[i]->SetBinContent(bin, JetClusterEnergy->GetRMS());
                hJetClusterEnergy_RMS[i]->SetBinError(bin, JetClusterEnergy->GetRMSError());
                TH1D* tempClusterTime       = new TH1D(*ClusterTime);
                tempClusterTime->GetXaxis()->SetTitle("t^{Cluster} (ns)");
                tempClusterTime->GetYaxis()->SetTitle("#frac{1}{N_{Events}} #frac{dN^{Cluster}}{dt^{Cluster} (GeV)}");
                tempClusterTime->SetTitle(Form("%s",fRunNumber.Data()));
                tempClusterTime->Sumw2();
                if(notScaled) tempClusterTime->Scale(1 / tempClusterTime->GetEntries());
                tempClusterTime->SetDirectory(0);
                vecClusterTime[i].push_back(tempClusterTime);
                TH1D* tempJetClusterEnergy       = new TH1D(*JetClusterEnergy);
                tempJetClusterEnergy->GetXaxis()->SetTitle("E^{Cluster} (GeV)");
                tempJetClusterEnergy->GetYaxis()->SetTitle("#frac{1}{N_{Events}} #frac{dN^{Cluster}}{dE^{Cluster} (ns)}");
                tempJetClusterEnergy->SetTitle(Form("%s",fRunNumber.Data()));
                tempJetClusterEnergy->Sumw2();
                if(notScaled) tempJetClusterEnergy->Scale(1 / tempJetClusterEnergy->GetEntries());
                tempJetClusterEnergy->SetDirectory(0);
                vecJetClusterEnergy[i].push_back(tempJetClusterEnergy);
                if(i==0) mapClusterTimeRatio[j]=bin;
                else if( (mapClusterTimeRatio.find(j) != mapClusterTimeRatio.end()) && mapClusterTimeRatio[j]>=0 && j<(Int_t)vecClusterTime[0].size() ){
                    TH1D* tempClusterTimeRatio   = new TH1D(*tempClusterTime);
                    tempClusterTimeRatio->Divide(tempClusterTimeRatio,vecClusterTime[0].at(j),1,1,"B");
                    tempClusterTimeRatio->SetName(Form("%s_z-ClusterTimeRatio_%s",fRunNumber.Data(),fDataSet.Data()));
                    tempClusterTimeRatio->GetXaxis()->SetRangeUser(-10,10);
                    tempClusterTimeRatio->GetYaxis()->SetTitle("ratio");
                    tempClusterTimeRatio->GetYaxis()->SetRangeUser(0,2);
                    vecClusterTimeRatio[i-1].push_back(tempClusterTimeRatio);
                }
                if(i==0) mapJetClusterEnergyRatio[j]=bin;
                else if( (mapJetClusterEnergyRatio.find(j) != mapJetClusterEnergyRatio.end()) && mapJetClusterEnergyRatio[j]>=0 && j<(Int_t)vecJetClusterEnergy[0].size() ){
                    TH1D* tempJetClusterEnergyRatio   = new TH1D(*tempJetClusterEnergy);
                    tempJetClusterEnergyRatio->Divide(tempJetClusterEnergyRatio,vecJetClusterEnergy[0].at(j),1,1,"B");
                    tempJetClusterEnergyRatio->SetName(Form("%s_z-JetClusterEnergyRatio_%s",fRunNumber.Data(),fDataSet.Data()));
                    tempJetClusterEnergyRatio->GetXaxis()->SetRangeUser(-10,10);
                    tempJetClusterEnergyRatio->GetYaxis()->SetTitle("ratio");
                    tempJetClusterEnergyRatio->GetYaxis()->SetRangeUser(0,2);
                    vecJetClusterEnergyRatio[i-1].push_back(tempJetClusterEnergyRatio);
                }
            }else cout << "INFO: Object |Cluster Time vs Energy| could not be found! Skipping Fill..." << endl;

            //--------------------------------------------------------------------------------------------------------
            //------------------------- primary vertex distribution in Z ---------------------------------------------
            //--------------------------------------------------------------------------------------------------------
            TH1D* AllClusterEnergy               = (TH1D*) HistoList->FindObject("hClusterEnergy1D");
            if(AllClusterEnergy){
                hAllClusterEnergy_Mean[i]->SetBinContent(bin, AllClusterEnergy->GetMean());
                hAllClusterEnergy_Mean[i]->SetBinError(bin, AllClusterEnergy->GetMeanError());
                hAllClusterEnergy_RMS[i]->SetBinContent(bin, AllClusterEnergy->GetRMS());
                hAllClusterEnergy_RMS[i]->SetBinError(bin, AllClusterEnergy->GetRMSError());
                sAllClusterEnergyMean.mean += AllClusterEnergy->GetMean();
                MeanAllClusterEnergy->Fill(AllClusterEnergy->GetMean(),AllClusterEnergy->GetRMS());
                TH1D* tempAllClusterEnergy       = new TH1D(*AllClusterEnergy);
                tempAllClusterEnergy->GetXaxis()->SetTitle("z-Vertex (cm)");
                tempAllClusterEnergy->GetYaxis()->SetTitle("#frac{1}{N_{Events}} #frac{dN}{dz}");
                tempAllClusterEnergy->SetTitle(Form("%s",fRunNumber.Data()));
                tempAllClusterEnergy->Sumw2();
                if(notScaled) tempAllClusterEnergy->Scale(1 / tempAllClusterEnergy->GetEntries());
                tempAllClusterEnergy->SetDirectory(0);
                vecAllClusterEnergy[i].push_back(tempAllClusterEnergy);

                if(i==0) mapAllClusterEnergyRatio[j]=bin;
                else if( (mapAllClusterEnergyRatio.find(j) != mapAllClusterEnergyRatio.end()) && mapAllClusterEnergyRatio[j]>=0 && j<(Int_t)vecAllClusterEnergy[0].size() && i < nData){
                    TH1D* tempAllClusterEnergyRatio   = new TH1D(*tempAllClusterEnergy);
                    tempAllClusterEnergyRatio->Divide(tempAllClusterEnergyRatio,vecAllClusterEnergy[0].at(j),1,1,"B");
                    tempAllClusterEnergyRatio->SetName(Form("%s_z-VertexRatio_%s",fRunNumber.Data(),fDataSet.Data()));
                    //tempVertexRatio->GetXaxis()->SetRangeUser(-10,10);
                    tempAllClusterEnergyRatio->GetYaxis()->SetTitle("ratio");
                    //tempVertexRatio->GetYaxis()->SetRangeUser(0,2);
                    vecAllClusterEnergyRatio[i-1].push_back(tempAllClusterEnergyRatio);
                }
            }else cout << "INFO: Object |AllClusterEnergy| could not be found! Skipping Fill..." << endl;

            //--------------------------------------------------------------------------------------------------------
            //------------------------- Number of Cells Per Cluster --------------------------------------------------
            //--------------------------------------------------------------------------------------------------------
            TH2D* ClusterCells2D               = (TH2D*) HistoList->FindObject("hQAClusterNCellVsE");
            std::vector<TH1D*> tempVecClusterCells;
            std::vector<TH1D*> tempVecClusterCellsRatio;
            if(ClusterCells2D){
                for (Int_t binE = 0; binE < nEBins; binE++){
                    TH1D* ClusterCells = (TH1D*)ClusterCells2D->ProjectionX(Form("ClusterCells_%i_%i", binE, i),binsE[binE],binsE[binE+1]);
                    hClusterCells_Mean[i][binE]->SetBinContent(bin, ClusterCells->GetMean());
                    hClusterCells_Mean[i][binE]->SetBinError(bin, ClusterCells->GetMeanError());
                    hClusterCells_RMS[i][binE]->SetBinContent(bin, ClusterCells->GetRMS());
                    hClusterCells_RMS[i][binE]->SetBinError(bin, ClusterCells->GetRMSError());
                    TH1D* tempClusterCells       = new TH1D(*ClusterCells);
                    tempClusterCells->GetXaxis()->SetTitle("N^{Cells}");
                    tempClusterCells->GetYaxis()->SetTitle("#frac{1}{N_{Events}} #frac{dN_{Clusters}}{dE}");
                    tempClusterCells->SetTitle(Form("%s",fRunNumber.Data()));
                    tempClusterCells->Sumw2();
                    if(notScaled) tempClusterCells->Scale(1 / tempClusterCells->GetEntries());
                    tempClusterCells->SetDirectory(0);
                    tempVecClusterCells.push_back(tempClusterCells);
                }
                vecClusterCells[i].push_back(tempVecClusterCells);

                if(i==0) mapClusterCellsRatio[j]=bin;
                else if( (mapClusterCellsRatio.find(j) != mapClusterCellsRatio.end()) && mapClusterCellsRatio[j]>=0 && j<(Int_t)vecClusterCells[0].size() ){
                    for (Int_t binE = 0; binE < nEBins; binE++){
                        TH1D* tempClusterCellsRatio   = new TH1D(*tempVecClusterCells.at(binE));
                        tempClusterCellsRatio->Divide(tempClusterCellsRatio,vecClusterCells[0].at(j).at(binE),1,1,"B");
                        tempClusterCellsRatio->SetName(Form("%s_ClusterCellsDistribution_%s_%i",fRunNumber.Data(),fDataSet.Data(),binE));
                        tempClusterCellsRatio->GetXaxis()->SetRangeUser(-10,10);
                        tempClusterCellsRatio->GetYaxis()->SetTitle("ratio");
                        tempClusterCellsRatio->GetYaxis()->SetRangeUser(0,2);
                        tempVecClusterCellsRatio.push_back(tempClusterCellsRatio);
                    }

                }
                if( (i>0) && (mapClusterCellsRatio.find(j) != mapClusterCellsRatio.end()) && mapClusterCellsRatio[j]>=0 && j<(Int_t)vecClusterCells[0].size() ){
                    vecClusterCellsRatio[i-1].push_back(tempVecClusterCellsRatio);
                }
                tempVecClusterCells.clear();
                tempVecClusterCellsRatio.clear();

            }else cout << "INFO: Object |Cells Per Cluster| could not be found! Skipping Fill..." << endl;

            if(fEnergyFlag.Contains("PbPb")){
                //--------------------------------------------------------------------------------------------------------
                //--------------------------------------- centrality -----------------------------------------------------
                //--------------------------------------------------------------------------------------------------------
                TH1D* Centrality               = (TH1D*) HistoList->FindObject("hEventCentrality");
                if(Centrality){
                    Int_t binT  = 1;
                    while (!(Centrality->GetBinContent(binT) > 0) && binT < Centrality->GetNbinsX()) binT++;
                    hCentralityMin[i]->SetBinContent(bin, Centrality->GetBinCenter(binT)-0.5);
                    hCentralityMin[i]->SetBinError(bin, 0);
                    binT  = Centrality->GetNbinsX();
                    while (!(Centrality->GetBinContent(binT) > 0) && binT > 0 ) binT--;
                    hCentralityMax[i]->SetBinContent(bin, Centrality->GetBinCenter(binT)+0.5);
                    hCentralityMax[i]->SetBinError(bin, 0);
                    hCentralityMean[i]->SetBinContent(bin, Centrality->GetMean());
                    hCentralityMean[i]->SetBinError(bin, Centrality->GetMeanError());
                    sCentralityMean.mean += Centrality->GetMean();
                    MeanCentrality->Fill(Centrality->GetMean(),Centrality->GetRMS());
                    TH1D* tempCent       = new TH1D(*Centrality);
                    tempCent->GetXaxis()->SetTitle("Centrality (%)");
                    tempCent->GetYaxis()->SetTitle("#frac{1}{N_{Events}} #frac{dN}{dcent}");
                    tempCent->SetTitle(Form("%s",fRunNumber.Data()));
                    tempCent->Sumw2();
                    if(notScaled) tempCent->Scale(1 / tempCent->GetEntries());
                    tempCent->SetDirectory(0);
                    vecCent[i].push_back(tempCent);
                    if(i==0) mapCentRatio[j]=bin;
                    else if( (mapCentRatio.find(j) != mapCentRatio.end()) && mapCentRatio[j]>=0 && j<(Int_t)vecCent[0].size() ){
                        TH1D* tempCentRatio   = new TH1D(*tempCent);
                        tempCentRatio->Divide(tempCentRatio,vecCent[0].at(j),1,1,"B");
                        tempCentRatio->SetName(Form("%s_-CentRatio_%s",fRunNumber.Data(),fDataSet.Data()));
                        tempCentRatio->GetXaxis()->SetRangeUser(-10,10);
                        tempCentRatio->GetYaxis()->SetTitle("ratio");
                        tempCentRatio->GetYaxis()->SetRangeUser(0,2);
                        vecCentRatio[i-1].push_back(tempCentRatio);
                    }
                }else cout << "INFO: Object |Centrality| could not be found! Skipping Fill..." << endl;

                //--------------------------------------------------------------------------------------------------------
                //--------------------------------------- event plane angle ----------------------------------------------
                //--------------------------------------------------------------------------------------------------------
                //TH1D* EventPlaneAngle          = (TH1D*) HistoList->FindObject(fHistEventPlane);  // Need to deal with the fact that fHistEventPlane is a TH1F before this works
                //if(EventPlaneAngle){
                //    hEventPlaneAngleMean[i]->SetBinContent(bin, EventPlaneAngle->GetMean());
                //    hEventPlaneAngleMean[i]->SetBinError(bin, EventPlaneAngle->GetMeanError());
                //} else cout << "INFO: Object |EventPlaneAngle| could not be found! Skipping Fill..." << endl;
            }
            //--------------------------------------------------------------------------------------------------------
            //---------------------------- Cleanup -------------------------------------------------------------------
            //--------------------------------------------------------------------------------------------------------
            delete HistoList;
            HistoList = NULL;

            delete TopDir;
            TopDir = NULL;

            RootFile->Close();
            delete RootFile;
            RootFile = NULL;
    } // end of loop over runs

        //--------------------------------------------------------------------------------------------------------
        //--------------------------------------- Mean values ----------------------------------------------------
        //--------------------------------------------------------------------------------------------------------
        if(doExtQA==3){
            vecRuns.clear();
            if(!readin(fileRuns[i], vecRuns, kFALSE)) {cout << "ERROR, no Run Numbers could be found! Returning..." << endl; return;}

            for(Int_t iRun=0; iRun<(Int_t)vecLowJetStats[i].size(); iRun++){
                vecRuns.erase(std::remove(vecRuns.begin(), vecRuns.end(), vecLowJetStats[i].at(iRun)), vecRuns.end());
            }

            sVertexZMean.mean        /= vecRuns.size();  // divide the sum of bin contents by number of runs in the current dataset
            sNchMean.mean            /= vecRuns.size();
            sPtChMean.mean           /= vecRuns.size();
            sAllClusterEnergyMean.mean  /= vecRuns.size();
            if(fEnergyFlag.Contains("PbPb")){
                sCentralityMean.mean     /= vecRuns.size();
            }

            cout << endl << endl;
            cout << "Abs Mean z-vertex: "       << sVertexZMean.mean                 << endl;
            cout << "Abs Mean z-vertex: "       << MeanVertexZ->GetMean()            << endl;
            cout << "Std. Dev.: "               << MeanVertexZ->GetStdDev()          << endl;
            cout << "Abs Mean N-charged: "      << sNchMean.mean                     << endl;
            cout << "Abs Mean N-charged: "      << MeanNch->GetMean()                << endl;
            cout << "Std. Dev.: "               << MeanNch->GetStdDev()              << endl;
            cout << "Abs Mean pT-charged: "     << sPtChMean.mean                    << endl;
            cout << "Abs Mean pT-charged: "     << MeanPtCh->GetMean()               << endl;
            cout << "Std. Dev.: "               << MeanPtCh->GetStdDev()             << endl;
            cout << "Abs Mean cluster energy: " << sAllClusterEnergyMean.mean        << endl;
            cout << "Abs Mean cluster energy: " << MeanAllClusterEnergy->GetMean()   << endl;
            cout << "Std. Dev.: "               << MeanAllClusterEnergy->GetStdDev() << endl;

            for(Int_t j=0; j<(Int_t) vecRuns.size(); j++){  // loop over runs j of this dataset
              Bool_t isRunBadFlag             = kFALSE;
              Bool_t isRunBadVertexZ          = kFALSE;
              Bool_t isRunBadAllClusterEnergy = kFALSE;
              Bool_t isRunBadNch              = kFALSE;
              Bool_t isRunBadPtCh             = kFALSE;
              Bool_t isRunBadCentrality       = kFALSE;
                fRunNumber = vecRuns.at(j);
                if(doEquidistantXaxis) bin  = mapBin[fRunNumber]; else bin = fRunNumber.Atoi() - hFBin;     // bin: run number in global run list, starting from 1
                if(isBadRunJet(&sVertexZMean,bin,MeanVertexZ)){         isRunBadFlag = kTRUE; isRunBadVertexZ = kTRUE;}
                if(jetType == "Full Jets" || jetType == "Charged Jets"){
                    if(isBadRunJet(&sNchMean,bin,MeanNch)){              isRunBadFlag = kTRUE; isRunBadNch = kTRUE;}
                    if(isBadRunJet(&sPtChMean,bin,MeanPtCh)){            isRunBadFlag = kTRUE; isRunBadPtCh = kTRUE;}
                }
                if(isBadRunJet(&sAllClusterEnergyMean,bin,MeanAllClusterEnergy)){isRunBadFlag = kTRUE; isRunBadAllClusterEnergy = kTRUE;}
                if(fEnergyFlag.Contains("PbPb")){
                    if(isBadRunJet(&sCentralityMean,bin,MeanCentrality)){isRunBadFlag = kTRUE; isRunBadCentrality = kTRUE;}
                }
                if(isRunBadFlag){ vecRunsBad.push_back(fRunNumber); cout << "Run " << fRunNumber << " labeled bad. Quality issue with:" << endl; }
                if(isRunBadVertexZ){ vecRunsBadVertexZ.push_back(fRunNumber); cout << "Z Vertex position" << endl; }
                if(isRunBadAllClusterEnergy){ vecRunsBadAllClusterEnergy.push_back(fRunNumber); cout << "Cluster energy (all clusters)" << endl; }
                if(isRunBadNch){ vecRunsBadNch.push_back(fRunNumber); cout << "Average number of charged jet constituents" << endl; }
                if(isRunBadPtCh){ vecRunsBadPtCh.push_back(fRunNumber); cout << "Average pt of charged jet constituents" << endl; }
                if(isRunBadCentrality){ vecRunsBadCentrality.push_back(fRunNumber); cout << "Centrality" << endl; }
                if(isRunBadFlag) cout << endl;
            } // end of loop over runs
            cout << "+++++++++++++++++++++++++++++++++++++++++" << endl;
            cout << "INFO: hVertexZMean: "          << sVertexZMean.nRunsBad          << " runs of " <<  vecRuns.size() << " runs deviate by more than " << sVertexZMean.nSigmaBad          << " error bars from the mean over all runs" << endl;
            cout << "INFO: hNch_Mean: "             << sNchMean.nRunsBad              << " runs of " <<  vecRuns.size() << " runs deviate by more than " << sNchMean.nSigmaBad              << " error bars from the mean over all runs" << endl;
            cout << "INFO: hPtCh_Mean: "            << sPtChMean.nRunsBad             << " runs of " <<  vecRuns.size() << " runs deviate by more than " << sPtChMean.nSigmaBad             << " error bars from the mean over all runs" << endl;
            cout << "INFO: hAllClusterEnergy_Mean: "   << sAllClusterEnergyMean.nRunsBad    << " runs of " <<  vecRuns.size() << " runs deviate by more than " << sAllClusterEnergyMean.nSigmaBad    << " error bars from the mean over all runs" << endl;
            if(fEnergyFlag.Contains("PbPb")){
                cout << "INFO: hCentralityMean: "       << sCentralityMean.nRunsBad       << " runs of " <<  vecRuns.size() << " runs deviate by more than " << sCentralityMean.nSigmaBad       << " error bars from the mean over all runs" << endl;
            }
            if(!vecRunsBad.empty()) writeout(fileRunsBad[i], vecRunsBad, kTRUE);
            if(!vecRunsBadVertexZ.empty()) writeout(fileRunsBadVertexZ[i], vecRunsBadVertexZ, kTRUE);
            if(!vecRunsBadAllClusterEnergy.empty()) writeout(fileRunsBadAllClusterEnergy[i], vecRunsBadAllClusterEnergy, kTRUE);
            if(!vecRunsBadNch.empty()) writeout(fileRunsBadNch[i], vecRunsBadNch, kTRUE);
            if(!vecRunsBadPtCh.empty()) writeout(fileRunsBadPtCh[i], vecRunsBadPtCh, kTRUE);
            if(!vecRunsBadCentrality.empty()) writeout(fileRunsBadCentrality[i], vecRunsBadCentrality, kTRUE);
            if(!vecRunsBadJets.empty()) writeout(fileRunsBadJets[i], vecRunsBadJets, kTRUE);

            sVertexZMean.mean = 0;
            sNchMean.mean = 0;
            sPtChMean.mean = 0;
            sAllClusterEnergyMean.mean = 0;
            if(fEnergyFlag.Contains("PbPb")){
                sCentralityMean.mean = 0;
            }
        }

        cout << "Doing fitters..." << endl;
        //--------------------------------------------------------------------------------------------------------
        //---------------------------------------- Fitters -------------------------------------------------------
        //--------------------------------------------------------------------------------------------------------
        TF1 *tfFit;
        for(Int_t iHist=0; iHist<(Int_t)vecHistosTH1D[i].size(); iHist++){
            TH1D* temp          = vecHistosTH1D[i].at(iHist);
            if(doEquidistantXaxis)
                tfFit           = new TF1("tfFit","[0]",0,temp->GetNbinsX());
            else
                tfFit           = new TF1("tfFit","[0]",rangesRuns[i][0],rangesRuns[i][1]);
            temp->Fit(tfFit,"QRME0");
            fitValuesTH1D[i][iHist] = tfFit->GetParameter(0);
            delete tfFit;
            tfFit               = 0x0;
        }

        TF1 *tfFit2;
        for(Int_t iHist2=0; iHist2<(Int_t)vecHistosTH1F[i].size(); iHist2++){
            TH1F* temp2          = vecHistosTH1F[i].at(iHist2);
            if(doEquidistantXaxis)
                tfFit2           = new TF1("tfFit2","[0]",0,temp2->GetNbinsX());
            else
                tfFit2           = new TF1("tfFit2","[0]",rangesRuns[i][0],rangesRuns[i][1]);
            temp2->Fit(tfFit2,"QRME0");
            fitValuesTH1F[i][iHist2] = tfFit2->GetParameter(0);
            delete tfFit2;
            tfFit2               = 0x0;
        }
    } // end of loop over datasets

    writeout(fileNoNeutral, vecNoNeutral, kTRUE);
    writeout(fileNoCharged, vecNoCharged, kTRUE);
    writeout(fileNEFQA, vecNEFQA, kTRUE);

    if(isNullNSigmas){ free(nSigmasBadRun); free(nSigmasBad); }

    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //++++++++++++++++++++++++++++++ Drawing Histograms ++++++++++++++++++++++++++++++++++++++++++++++++
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << "Drawing Histograms" << endl;
    TCanvas* canvas         = new TCanvas("canvas","",10,10,750,500);  // gives the page size
    Double_t leftMar        = 0.09;
    Double_t rightMar       = 0.025;
    Double_t topMargin      = 0.04;
    Double_t bottomMargin   = 0.09;
    DrawGammaCanvasSettings(canvas, leftMar, rightMar, topMargin, bottomMargin);
    Double_t yMax;  // min of y range for vertical lines
    Double_t yMin;  // max of y range for vertical lines
    Bool_t adjustedRange = kFALSE;

    // Plot single periods as well
    if(doHistsForEverySet) {
        TString outputDirDataSet;

        for(Int_t i=0; i<nSets; i++) {
            cout << "DataSet: " << DataSets[i].Data() << endl;
            outputDirDataSet = Form("%s/%s",outputDir.Data(), DataSets[i].Data());
            if(useDataRunListForMC && i>=nData) {
                outputDirDataSet = Form("%s/%s-%s", outputDir.Data(), DataSets[i].Data(),DataSets[0].Data());
                cout << "Switch useDataRunListForMC is true, output to: " << outputDirDataSet.Data() << endl;
            }
            gSystem->Exec("mkdir -p "+outputDirDataSet);
            gSystem->Exec("mkdir -p "+outputDirDataSet+"/ExtQA");

            if(i>0){
                DrawVectorOverviewTH1D( canvas, vecVertexZRatio[i-1], "hRatioVertexZ", outputDirDataSet, suffix,
                        0.13, 0.15, 0.1, 0.14, 0.8, 0.9, 0.12, 0.93, 0x0, kFALSE, kFALSE);
                DrawVectorOverviewTH1D( canvas, vecJetPtRatio[i-1], "hRatioJetPt", outputDirDataSet, suffix,
                        0.13, 0.15, 0.1, 0.14, 0.8, 0.9, 0.12, 0.93, 0x0, kFALSE, kTRUE);
                DrawVectorOverviewTH1D( canvas, vecJetPtMaxRatio[i-1], "hRatioJetPtMax", outputDirDataSet, suffix,
                        0.13, 0.15, 0.1, 0.14, 0.8, 0.9, 0.12, 0.93, 0x0, kFALSE, kTRUE);
                DrawVectorOverviewTH1D( canvas, vecClusterTimeRatio[i-1], "hRatioClusterTime", outputDirDataSet, suffix,
                        0.13, 0.15, 0.1, 0.14, 0.8, 0.9, 0.12, 0.93, 0x0, kFALSE, kFALSE);
                DrawVectorOverviewTH1D( canvas, vecJetClusterEnergyRatio[i-1], "hRatioJetClusterEnergy", outputDirDataSet, suffix,
                        0.13, 0.15, 0.1, 0.14, 0.8, 0.9, 0.12, 0.93, 0x0, kFALSE, kFALSE);
                DrawVectorOverviewTH1D( canvas, vecAllClusterEnergyRatio[i-1], "hRatioAllClusterEnergy", outputDirDataSet, suffix,
                        0.13, 0.15, 0.1, 0.14, 0.8, 0.9, 0.12, 0.93, 0x0, kFALSE, kFALSE);
                DrawMatrixOverviewTH1D( canvas, vecClusterCellsRatio[i-1], "hRatioClusterCells", outputDirDataSet, suffix,
                        0.13, 0.15, 0.1, 0.14, 0.8, 0.9, 0.12, 0.93, 0x0, kFALSE, kFALSE);
                DrawMatrixOverviewTH1D( canvas, vecEtaPhiRatio[i-1], "hRatioEtaPhi", outputDirDataSet, suffix,
                        0.13, 0.15, 0.1, 0.14, 0.8, 0.9, 0.12, 0.93, 0x0, kFALSE, kFALSE);
                if(fEnergyFlag.Contains("PbPb")){
                    DrawVectorOverviewTH1D( canvas, vecCentRatio[i-1], "hRatioCentZ", outputDirDataSet, suffix,
                            0.13, 0.15, 0.1, 0.14, 0.8, 0.9, 0.12, 0.93, 0x0, kFALSE, kFALSE);
                }
                if(jetType == "Full Jets"){
                    DrawMatrixOverviewTH1D( canvas, vecNEFRatio[i-1], "hRatioNEF", outputDirDataSet, suffix,
                            0.13, 0.15, 0.1, 0.14, 0.8, 0.9, 0.12, 0.93, 0x0, kFALSE, kFALSE);
                    DrawMatrixOverviewTH1D( canvas, vecNchRatio[i-1], "hRatioNch", outputDirDataSet, suffix,
                            0.13, 0.15, 0.1, 0.14, 0.8, 0.9, 0.12, 0.93, 0x0, kFALSE, kFALSE);
                    DrawVectorOverviewTH1D( canvas, vecNchAllRatio[i-1], "hRatioNchAll", outputDirDataSet, suffix,
                            0.13, 0.15, 0.1, 0.14, 0.8, 0.9, 0.12, 0.93, 0x0, kFALSE, kFALSE);
                    DrawVectorOverviewTH1D( canvas, vecPtChAllRatio[i-1], "hRatioPtChAll", outputDirDataSet, suffix,
                            0.13, 0.15, 0.1, 0.14, 0.8, 0.9, 0.12, 0.93, 0x0, kFALSE, kFALSE);
                    DrawMatrixOverviewTH1D( canvas, vecNneRatio[i-1], "hRatioNne", outputDirDataSet, suffix,
                            0.13, 0.15, 0.1, 0.14, 0.8, 0.9, 0.12, 0.93, 0x0, kFALSE, kFALSE);
                    DrawMatrixOverviewTH1D( canvas, vecPtChRatio[i-1], "hRatioPtCh", outputDirDataSet, suffix,
                            0.13, 0.15, 0.1, 0.14, 0.8, 0.9, 0.12, 0.93, 0x0, kFALSE, kTRUE);
                    DrawMatrixOverviewTH1D( canvas, vecPtNeRatio[i-1], "hRatioPtNe", outputDirDataSet, suffix,
                            0.13, 0.15, 0.1, 0.14, 0.8, 0.9, 0.12, 0.93, 0x0, kFALSE, kTRUE);
                    DrawMatrixOverviewTH1D( canvas, vecPtChMaxRatio[i-1], "hRatioPtChMax", outputDirDataSet, suffix,
                            0.13, 0.15, 0.1, 0.14, 0.8, 0.9, 0.12, 0.93, 0x0, kFALSE, kTRUE);
                    DrawMatrixOverviewTH1D( canvas, vecPtNeMaxRatio[i-1], "hRatioPtNeMax", outputDirDataSet, suffix,
                            0.13, 0.15, 0.1, 0.14, 0.8, 0.9, 0.12, 0.93, 0x0, kFALSE, kTRUE);
                    DrawMatrixOverviewTH1D( canvas, vecEtaPhiChRatio[i-1], "hRatioEtaPhiCh", outputDirDataSet, suffix,
                            0.13, 0.15, 0.1, 0.14, 0.8, 0.9, 0.12, 0.93, 0x0, kFALSE, kFALSE);
                    DrawMatrixOverviewTH1D( canvas, vecEtaPhiNeRatio[i-1], "hRatioEtaPhiNe", outputDirDataSet, suffix,
                            0.13, 0.15, 0.1, 0.14, 0.8, 0.9, 0.12, 0.93, 0x0, kFALSE, kFALSE);
                    DrawMatrixOverviewTH1D( canvas, vecEtaPhiChMaxRatio[i-1], "hRatioEtaPhiChMax", outputDirDataSet, suffix,
                            0.13, 0.15, 0.1, 0.14, 0.8, 0.9, 0.12, 0.93, 0x0, kFALSE, kFALSE);
                    DrawMatrixOverviewTH1D( canvas, vecEtaPhiNeMaxRatio[i-1], "hRatioEtaPhiNeMax", outputDirDataSet, suffix,
                            0.13, 0.15, 0.1, 0.14, 0.8, 0.9, 0.12, 0.93, 0x0, kFALSE, kFALSE);
                }
            }

            //--------------------------------------------------------------------------------------------------------
            for(Int_t h=0; h<(Int_t) vecHistosTH1D[i].size(); h++) {
                ((TH1D*) vecHistosTH1D[i].at(h))->SetTitle("");
                cout << "Histo Name: " << vecHistosTH1D[i].at(h)->GetName() << endl;
                if( ((TString)vecHistosNameTH1D.at(h)).CompareTo("hNEvents")==0 ) {
                    if(vecHistosTH1D[i].at(h)->GetEntries() > 0) AdjustHistRange(((TH1D*) vecHistosTH1D[i].at(h)),10,10,kTRUE);
                    ((TH1D*) vecHistosTH1D[i].at(h))->Draw("p");
                } else {
                    if(vecHistosTH1D[i].at(h)->GetEntries() > 0) AdjustHistRange(((TH1D*) vecHistosTH1D[i].at(h)),1.1,1.1,kTRUE);
                    ((TH1D*) vecHistosTH1D[i].at(h))->Draw("px0e1");
                }
                if(i<nData) DrawFit(((TH1D*) vecHistosTH1D[i].at(h)),i,fitValuesTH1D[i][h],rangesRuns[i],DataSets[i],plotDataSets[i],0.15,0.9,0.03,1);

                if(doTrigger && i<nData){
                    PutProcessLabelAndEnergyOnPlot(xPosLabel, 0.92, 0.03, fCollisionSystem.Data(), plotDataSets[i].Data(), fTrigger.Data(), 62, 0.03, "", 1, 1.25, 31);
                } else{
                    TString temp="";
                    PutProcessLabelAndEnergyOnPlot(xPosLabel, 0.92, 0.03, fCollisionSystem.Data(), plotDataSets[i].Data(), temp.Data(), 62, 0.03, "", 1, 1.25, 31);
                }

                PutNameAndJetTypeOnPlot(xPosLabel, 0.84, 0.03, jetType.Data(), "", vecBinRangeTH1D.at(h), 62, 0.03, "", 1, 1.25, 31);

                if( ((TString)vecHistosNameTH1D.at(h)).CompareTo("hNEvents")==0 )
                    SaveCanvas(canvas, Form("%s/%s.%s", outputDirDataSet.Data(), vecHistosNameTH1D.at(h).Data(), suffix.Data()));
                else SaveCanvas(canvas, Form("%s/%s.%s", outputDirDataSet.Data(), vecHistosNameTH1D.at(h).Data(), suffix.Data()));
            }

            for(Int_t h=0; h<(Int_t) vecHistosTH1F[i].size(); h++) {
                ((TH1F*) vecHistosTH1F[i].at(h))->SetTitle("");
                if( ((TString)vecHistosNameTH1F.at(h)).CompareTo("hNEvents")==0 ) {
                    if(vecHistosTH1F[i].at(h)->GetEntries() > 0) AdjustHistRangeTH1F(((TH1F*) vecHistosTH1F[i].at(h)),10,10,kTRUE);
                    ((TH1F*) vecHistosTH1F[i].at(h))->Draw("p");
                } else {
                    if(vecHistosTH1F[i].at(h)->GetEntries() > 0) AdjustHistRangeTH1F(((TH1F*) vecHistosTH1F[i].at(h)),1.1,1.1,kTRUE);
                    ((TH1F*) vecHistosTH1F[i].at(h))->Draw("px0e1");
                }
                if(i<nData) DrawFit(((TH1D*) vecHistosTH1F[i].at(h)),i,fitValuesTH1F[i][h],rangesRuns[i],DataSets[i],plotDataSets[i],0.15,0.9,0.03,1);

                if(doTrigger && i<nData){
                    PutProcessLabelAndEnergyOnPlot(xPosLabel, 0.92, 0.03, fCollisionSystem.Data(), plotDataSets[i].Data(), fTrigger.Data(), 62, 0.03, "", 1, 1.25, 31);
                } else{
                    TString temp="";
                    PutProcessLabelAndEnergyOnPlot(xPosLabel, 0.92, 0.03, fCollisionSystem.Data(), plotDataSets[i].Data(), temp.Data(), 62, 0.03, "", 1, 1.25, 31);
                }

                PutNameAndJetTypeOnPlot(xPosLabel, 0.84, 0.03, jetType.Data(), "", vecBinRangeTH1F.at(h), 62, 0.03, "", 1, 1.25, 31);

                if( ((TString)vecHistosNameTH1F.at(h)).CompareTo("hNEvents")==0 )
                    SaveCanvas(canvas, Form("%s/%s.%s", outputDirDataSet.Data(), vecHistosNameTH1F.at(h).Data(), suffix.Data()), kFALSE, kTRUE);
                else SaveCanvas(canvas, Form("%s/%s.%s", outputDirDataSet.Data(), vecHistosNameTH1F.at(h).Data(), suffix.Data()));
            }
        }

        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*
        //++++++++++++++++++++++++++++++ Drawing Runwise Histograms ++++++++++++++++++++++++++++++++++++++++++++++++
        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*
        cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << "Drawing Runwise Histograms" << endl;

        TCanvas* canvasRunwise;
        TLegend *legendRuns;

        for(Int_t i=0; i<nData; i++) {
            vecRuns.clear();
            fDataSet                = vecDataSet.at(i);
            outputDirDataSet        = Form("%s/%s",outputDir.Data(), DataSets[i].Data());

            if(useDataRunListForMC && i>=nData) {
                outputDirDataSet    = Form("%s/%s-%s", outputDir.Data(), DataSets[i].Data(),DataSets[0].Data());
            }
            if(!readin(fileRuns[i], vecRuns, kFALSE)) {cout << "ERROR, no Run Numbers could be found! Returning..." << endl; return;}
            if(vecRuns.size() == 0) continue;

            for(Int_t iRun=0; iRun<(Int_t)vecMissingRuns[i].size(); iRun++){
                vecRuns.erase(std::remove(vecRuns.begin(), vecRuns.end(), vecMissingRuns[i].at(iRun)), vecRuns.end());
            }

            for(Int_t iRun=0; iRun<(Int_t)vecLowJetStats[i].size(); iRun++){
                vecRuns.erase(std::remove(vecRuns.begin(), vecRuns.end(), vecLowJetStats[i].at(iRun)), vecRuns.end());
            }

            Int_t NColumns          = 2; //((Int_t) vecRuns.size() / 31 ) + 1;

            canvasRunwise           = new TCanvas("canvasRunwise","",10,10,1350+(NColumns*108),900);  // gives the page size
            DrawGammaCanvasSettings(canvasRunwise, 130.5/(1350.+(NColumns*108.)), (40.5+(NColumns*108.))/(1350.+(NColumns*108.)), topMargin, bottomMargin);
            canvasRunwise->cd();

            Double_t addRight       = ((Double_t)NColumns*108)/(1350+((Double_t)NColumns*108));
            legendRuns              = new TLegend(0.98-addRight,0.09,0.98,0.94);
            legendRuns->SetNColumns(NColumns-1);
            legendRuns->SetFillColor(0);
            legendRuns->SetLineColor(0);
            legendRuns->SetTextSize(0.03);
            legendRuns->SetTextFont(42);

            cout << "DataSet: " << DataSets[i].Data() << endl;
            //--------------------------------------------------------------------------------------------------------

            DrawVectorRunwiseTH1D(	kTRUE, canvasRunwise, legendRuns, vecVertexZ[i], vecRuns, 2, 1.1, kTRUE, addRight, 0.8, 0.94, 0.03, 0.8, 0.83, 0.03,
                                    doTrigger, fTrigger, (Bool_t)(i<nData), outputDirDataSet, "VertexZ_Runwise", plotDataSets[i], kFALSE,
                                    fCollisionSystem, "", suffix, kFALSE, kFALSE, kFALSE);


            if(fEnergyFlag.Contains("PbPb")){
            DrawVectorRunwiseTH1D(	kTRUE, canvasRunwise, legendRuns, vecCent[i], vecRuns, 2, 1.1, kTRUE, addRight, 0.8, 0.94, 0.03, 0.8, 0.83, 0.03,
                                    doTrigger, fTrigger, (Bool_t)(i<nData), outputDirDataSet, "Cent_Runwise", plotDataSets[i], kFALSE,
                                    fCollisionSystem, "", suffix, kFALSE, kFALSE, kFALSE);
            }


//            DrawVectorRunwiseTH1D(	kTRUE, canvasRunwise, legendRuns, vecClusterCounter[i], vecRuns, 2, 1.1, kTRUE, addRight, 0.8, 0.94, 0.03, 0.8, 0.83, 0.03,
//                                    doTrigger, fTrigger, (Bool_t)(i<nData), outputDirDataSet, "ClusterCounter_Runwise", plotDataSets[i], kFALSE,
//                                    fCollisionSystem, "", suffix, kFALSE, kFALSE, kFALSE);


            DrawVectorRunwiseTH1D(	kTRUE, canvasRunwise, legendRuns, vecJetPt[i], vecRuns, 2, 1.1, kTRUE, addRight, 0.8, 0.94, 0.03, 0.8, 0.83, 0.03,
                                    doTrigger, fTrigger, (Bool_t)(i<nData), outputDirDataSet, "JetPt_Runwise", plotDataSets[i], kFALSE,
                                    fCollisionSystem, "", suffix, kFALSE, kTRUE, kFALSE);


            DrawVectorRunwiseTH1D(	kTRUE, canvasRunwise, legendRuns, vecJetPtMax[i], vecRuns, 2, 1.1, kTRUE, addRight, 0.8, 0.94, 0.03, 0.8, 0.83, 0.03,
                                    doTrigger, fTrigger, (Bool_t)(i<nData), outputDirDataSet, "JetPtMax_Runwise", plotDataSets[i], kFALSE,
                                    fCollisionSystem, "", suffix, kFALSE, kFALSE, kFALSE);


            if(jetType == "Full Jets"){
            DrawMatrixRunwiseTH1D(	kTRUE, canvasRunwise, legendRuns, vecNEF[i], vecRuns, binsPtdoub, 2, 1.1, kTRUE, addRight, 0.8, 0.94, 0.03, 0.8, 0.83, 0.03,
                                    doTrigger, fTrigger, (Bool_t)(i<nData), outputDirDataSet, "NEF_Runwise", plotDataSets[i], kFALSE,
                                    fCollisionSystem, "", suffix, kFALSE, kFALSE, kFALSE);

            DrawMatrixRunwiseTH1D(	kTRUE, canvasRunwise, legendRuns, vecNch[i], vecRuns, binsPtdoub, 2, 1.1, kTRUE, addRight, 0.8, 0.94, 0.03, 0.8, 0.83, 0.03,
                                    doTrigger, fTrigger, (Bool_t)(i<nData), outputDirDataSet, "Nch_Runwise", plotDataSets[i], kFALSE,
                                    fCollisionSystem, "", suffix, kFALSE, kFALSE, kFALSE);

            DrawVectorRunwiseTH1D(	kTRUE, canvasRunwise, legendRuns, vecNchAll[i], vecRuns, 2, 1.1, kTRUE, addRight, 0.8, 0.94, 0.03, 0.8, 0.83, 0.03,
                                    doTrigger, fTrigger, (Bool_t)(i<nData), outputDirDataSet, "NchAll_Runwise", plotDataSets[i], kFALSE,
                                    fCollisionSystem, "", suffix, kFALSE, kFALSE, kFALSE);

            DrawVectorRunwiseTH1D(	kTRUE, canvasRunwise, legendRuns, vecPtChAll[i], vecRuns, 2, 1.1, kTRUE, addRight, 0.8, 0.94, 0.03, 0.8, 0.83, 0.03,
                                    doTrigger, fTrigger, (Bool_t)(i<nData), outputDirDataSet, "PtChAll_Runwise", plotDataSets[i], kFALSE,
                                    fCollisionSystem, "", suffix, kFALSE, kFALSE, kFALSE);

            DrawMatrixRunwiseTH1D(	kTRUE, canvasRunwise, legendRuns, vecNne[i], vecRuns, binsPtdoub, 2, 1.1, kTRUE, addRight, 0.8, 0.94, 0.03, 0.8, 0.83, 0.03,
                                    doTrigger, fTrigger, (Bool_t)(i<nData), outputDirDataSet, "Nne_Runwise", plotDataSets[i], kFALSE,
                                    fCollisionSystem, "", suffix, kFALSE, kFALSE, kFALSE);

            DrawMatrixRunwiseTH1D(	kTRUE, canvasRunwise, legendRuns, vecPtCh[i], vecRuns, binsPtdoub, 2, 1.1, kTRUE, addRight, 0.8, 0.94, 0.03, 0.8, 0.83, 0.03,
                                    doTrigger, fTrigger, (Bool_t)(i<nData), outputDirDataSet, "PtCh_Runwise", plotDataSets[i], kFALSE,
                                    fCollisionSystem, "", suffix, kFALSE, kFALSE, kFALSE);

            DrawMatrixRunwiseTH1D(	kTRUE, canvasRunwise, legendRuns, vecPtNe[i], vecRuns, binsPtdoub, 2, 1.1, kTRUE, addRight, 0.8, 0.94, 0.03, 0.8, 0.83, 0.03,
                                    doTrigger, fTrigger, (Bool_t)(i<nData), outputDirDataSet, "PtNe_Runwise", plotDataSets[i], kFALSE,
                                    fCollisionSystem, "", suffix, kFALSE, kFALSE, kFALSE);

            DrawMatrixRunwiseTH1D(	kTRUE, canvasRunwise, legendRuns, vecPtChMax[i], vecRuns, binsPtdoub, 2, 1.1, kTRUE, addRight, 0.8, 0.94, 0.03, 0.8, 0.83, 0.03,
                                    doTrigger, fTrigger, (Bool_t)(i<nData), outputDirDataSet, "PtChMax_Runwise", plotDataSets[i], kFALSE,
                                    fCollisionSystem, "", suffix, kFALSE, kFALSE, kFALSE);

            DrawMatrixRunwiseTH1D(	kTRUE, canvasRunwise, legendRuns, vecPtNeMax[i], vecRuns, binsPtdoub, 2, 1.1, kTRUE, addRight, 0.8, 0.94, 0.03, 0.8, 0.83, 0.03,
                                    doTrigger, fTrigger, (Bool_t)(i<nData), outputDirDataSet, "PtNeMax_Runwise", plotDataSets[i], kFALSE,
                                    fCollisionSystem, "", suffix, kFALSE, kFALSE, kFALSE);
            }

            DrawMatrixRunwiseTH1D(	kTRUE, canvasRunwise, legendRuns, vecEtaPhi[i], vecRuns, binsEta, 2, 1.1, kTRUE, addRight, 0.8, 0.94, 0.03, 0.8, 0.83, 0.03,
                                    doTrigger, fTrigger, (Bool_t)(i<nData), outputDirDataSet, "EtaPhi_Runwise", plotDataSets[i], kFALSE,
                                    fCollisionSystem, "", suffix, kFALSE, kFALSE, kFALSE);

            if(jetType == "Full Jets"){
            DrawMatrixRunwiseTH1D(	kTRUE, canvasRunwise, legendRuns, vecEtaPhiCh[i], vecRuns, binsEta, 2, 1.1, kTRUE, addRight, 0.8, 0.94, 0.03, 0.8, 0.83, 0.03,
                                    doTrigger, fTrigger, (Bool_t)(i<nData), outputDirDataSet, "EtaPhiCh_Runwise", plotDataSets[i], kFALSE,
                                    fCollisionSystem, "", suffix, kFALSE, kFALSE, kFALSE);

            DrawMatrixRunwiseTH1D(	kTRUE, canvasRunwise, legendRuns, vecEtaPhiNe[i], vecRuns, binsEta, 2, 1.1, kTRUE, addRight, 0.8, 0.94, 0.03, 0.8, 0.83, 0.03,
                                    doTrigger, fTrigger, (Bool_t)(i<nData), outputDirDataSet, "EtaPhiNe_Runwise", plotDataSets[i], kFALSE,
                                    fCollisionSystem, "", suffix, kFALSE, kFALSE, kFALSE);

            DrawMatrixRunwiseTH1D(	kTRUE, canvasRunwise, legendRuns, vecEtaPhiChMax[i], vecRuns, binsEta, 2, 1.1, kTRUE, addRight, 0.8, 0.94, 0.03, 0.8, 0.83, 0.03,
                                    doTrigger, fTrigger, (Bool_t)(i<nData), outputDirDataSet, "EtaPhiChMax_Runwise", plotDataSets[i], kFALSE,
                                    fCollisionSystem, "", suffix, kFALSE, kFALSE, kFALSE);

            DrawMatrixRunwiseTH1D(	kTRUE, canvasRunwise, legendRuns, vecEtaPhiNeMax[i], vecRuns, binsEta, 2, 1.1, kTRUE, addRight, 0.8, 0.94, 0.03, 0.8, 0.83, 0.03,
                                    doTrigger, fTrigger, (Bool_t)(i<nData), outputDirDataSet, "EtaPhiNeMax_Runwise", plotDataSets[i], kFALSE,
                                    fCollisionSystem, "", suffix, kFALSE, kFALSE, kFALSE);
            }

            DrawVectorRunwiseTH1D(	kTRUE, canvasRunwise, legendRuns, vecClusterTime[i], vecRuns, 2, 1.1, kTRUE, addRight, 0.8, 0.94, 0.03, 0.8, 0.83, 0.03,
                                    doTrigger, fTrigger, (Bool_t)(i<nData), outputDirDataSet, "ClusterTime_Runwise", plotDataSets[i], kFALSE,
                                    fCollisionSystem, "", suffix, kFALSE, kFALSE, kFALSE);


            DrawVectorRunwiseTH1D(	kTRUE, canvasRunwise, legendRuns, vecJetClusterEnergy[i], vecRuns, 2, 1.1, kTRUE, addRight, 0.8, 0.94, 0.03, 0.8, 0.83, 0.03,
                                    doTrigger, fTrigger, (Bool_t)(i<nData), outputDirDataSet, "JetClusterEnergy_Runwise", plotDataSets[i], kFALSE,
                                    fCollisionSystem, "", suffix, kFALSE, kFALSE, kFALSE);

            DrawMatrixRunwiseTH1D(	kTRUE, canvasRunwise, legendRuns, vecClusterCells[i], vecRuns, binsEdoub, 2, 1.1, kTRUE, addRight, 0.8, 0.94, 0.03, 0.8, 0.83, 0.03,
                                    doTrigger, fTrigger, (Bool_t)(i<nData), outputDirDataSet, "ClusterCells_Runwise", plotDataSets[i], kFALSE,
                                    fCollisionSystem, "", suffix, kFALSE, kFALSE, kFALSE);

        }

        delete legendRuns;
        legendRuns = NULL;
        //delete canvasRunwise;
    }
    canvas->cd();

    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*
    //++++++++++++++++++++++++++++++ Combined Trending Histograms +++++++++++++++++++++++++++++++++++++++++++++*
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*
    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << "Drawing Trending Histograms" << endl;
    //--------------------------------------------------------------------------------------------------------
    if(useDataRunListForMC) cout << "WARNING: useDataRunListForMC is true, overwriting histograms for DataSet!" << endl;

    TLegend *legend = new TLegend(0.15,0.95,0.95,0.98);
    legend->SetNColumns(nSets);
    legend->SetFillColor(0);
    legend->SetLineColor(0);
    legend->SetTextSize(0.03);
    legend->SetTextFont(42);

    TLegend *legend2 = new TLegend(0.15,0.95,0.95,0.98);
    legend2->SetNColumns(nSets);
    legend2->SetFillColor(0);
    legend2->SetLineColor(0);
    legend2->SetTextSize(0.03);
    legend2->SetTextFont(42);
    //--------------------------------------------------------------------------------------------------------
    if(nSets > 1) {
        Int_t hExt;
        for(Int_t h=0; h<(Int_t) vecHistosTH1D[0].size(); h++) {
            hExt = h;
            for(Int_t i=0; i<nSets; i++) {
                legend->AddEntry(((TH1D*) vecHistosTH1D[i].at(h)),plotDataSets[i].Data(),"p");
            }
            if( ((TString)vecHistosNameTH1D.at(h)).CompareTo("hNEvents")==0 )
                adjustedRange = AdjustHistRange(kTRUE, vecHistosTH1D,10,10,h,nSets,kTRUE, &yMin, &yMax);
            else adjustedRange = AdjustHistRange(kTRUE, vecHistosTH1D,1.1,1.1,h,nSets,kTRUE, &yMin, &yMax);
            for(Int_t i=nSets-1; i>=0; i--)            {
                TString draw;
                if(h==0) draw = (i==nSets-1)?"p":"p, same";
                else draw = (i==nSets-1)?"px0e1":"px0e1, same";
                ((TH1D*) vecHistosTH1D[i].at(h))->SetTitle("");
                ((TH1D*) vecHistosTH1D[i].at(h))->Draw(draw.Data());
                if(i<nData) DrawFit(((TH1D*) vecHistosTH1D[i].at(h)),i,fitValuesTH1D[i][h],rangesRuns[i],DataSets[i],plotDataSets[i],0.15,0.9,0.03,1);
            }
            if (drawVerticalLines){
                if(!adjustedRange){
                    canvas->Update();
                    yMax = canvas->GetUymax();
                    yMin = canvas->GetUymin();
                }
                for(Int_t lineBin=0; lineBin<nLines; lineBin++){
                    verticalLines[lineBin] = new TLine(runRanges[lineBin],yMin,runRanges[lineBin],yMax);
                    verticalLines[lineBin]->SetLineWidth(1);
                    verticalLines[lineBin]->SetLineStyle(2);
                    verticalLines[lineBin]->Draw("same");
                }
            }
            legend->Draw();
            if(doTrigger){

                PutProcessLabelAndEnergyOnPlot(xPosLabel, 0.92, 0.03, fCollisionSystem.Data(), fTrigger.Data(), "", 62, 0.03, "", 1, 1.25, 31);
            }else{
                PutProcessLabelAndEnergyOnPlot(xPosLabel, 0.92, 0.03, fCollisionSystem.Data(), "", "", 62, 0.03, "", 1, 1.25, 31);
            }

            PutNameAndJetTypeOnPlot(xPosLabel, 0.84, 0.03, jetType.Data(), "", vecBinRangeTH1D.at(h), 62, 0.03, "", 1, 1.25, 31);

            if(canvas->GetTopMargin()!=0.06) canvas->SetTopMargin(0.06);
            if(useDataRunListForMC && !addSubFolder){
                SaveCanvas(canvas, Form("%s/%s/%s.%s", outputDir.Data(), DataSets[0].Data(),vecHistosNameTH1D.at(h).Data(),suffix.Data()));
            } else {
                SaveCanvas(canvas, Form("%s/%s.%s", outputDir.Data(), vecHistosNameTH1D.at(h).Data(),suffix.Data()));
            }
            legend->Clear();
            if(drawVerticalLines){
                for(Int_t lineBin=0; lineBin<nLines; lineBin++){
                    delete verticalLines[lineBin];
                    verticalLines[lineBin] = NULL;
                }
            }
        }

        for(Int_t g=0; g<(Int_t) vecHistosTH1F[0].size(); g++) {
            Int_t m = g + hExt + 1;
            for(Int_t i=0; i<nSets; i++) {
                legend2->AddEntry(((TH1F*) vecHistosTH1F[i].at(g)),plotDataSets[i].Data(),"p");
            }
            if( ((TString)vecHistosNameTH1F.at(g)).CompareTo("hNEvents")==0 )
                adjustedRange = AdjustHistRangeTH1F(kTRUE, vecHistosTH1F,10,10,g,nSets,kTRUE, &yMin, &yMax);
            else adjustedRange = AdjustHistRangeTH1F(kTRUE, vecHistosTH1F,1.1,1.1,g,nSets,kTRUE, &yMin, &yMax);
            for(Int_t i=nSets-1; i>=0; i--)            {
                TString draw;
                if(g==0) draw = (i==nSets-1)?"p":"p, same";
                else draw = (i==nSets-1)?"px0e1":"px0e1, same";
                ((TH1F*) vecHistosTH1F[i].at(g))->SetTitle("");
                ((TH1F*) vecHistosTH1F[i].at(g))->Draw(draw.Data());
                if(i<nData) DrawFit(((TH1D*) vecHistosTH1F[i].at(g)),i,fitValuesTH1F[i][g],rangesRuns[i],DataSets[i],plotDataSets[i],0.15,0.9,0.03,1);
            }
            if (drawVerticalLines){
                if(!adjustedRange){
                    canvas->Update();
                    yMax = canvas->GetUymax();
                    yMin = canvas->GetUymin();
                }
                for(Int_t lineBin=0; lineBin<nLines; lineBin++){
                    verticalLines[lineBin] = new TLine(runRanges[lineBin],yMin,runRanges[lineBin],yMax);
                    verticalLines[lineBin]->SetLineWidth(1);
                    verticalLines[lineBin]->SetLineStyle(2);
                    verticalLines[lineBin]->Draw("same");
                }
            }
            legend2->Draw();

            if(doTrigger){
                PutProcessLabelAndEnergyOnPlot(xPosLabel, 0.92, 0.03, fCollisionSystem.Data(), fTrigger.Data(), "", 62, 0.03, "", 1, 1.25, 31);
            }else{
                PutProcessLabelAndEnergyOnPlot(xPosLabel, 0.92, 0.03, fCollisionSystem.Data(), "", "", 62, 0.03, "", 1, 1.25, 31);
            }

            PutNameAndJetTypeOnPlot(xPosLabel, 0.84, 0.03, jetType.Data(), "", vecBinRangeTH1F.at(g), 62, 0.03, "", 1, 1.25, 31);

            if(canvas->GetTopMargin()!=0.06) canvas->SetTopMargin(0.06);
            if(useDataRunListForMC && !addSubFolder){
                SaveCanvas(canvas, Form("%s/%s/%s.%s", outputDir.Data(), DataSets[0].Data(),vecHistosNameTH1F.at(g).Data(),suffix.Data()));
            } else {
                SaveCanvas(canvas, Form("%s/%s.%s", outputDir.Data(), vecHistosNameTH1F.at(g).Data(),suffix.Data()));
            }
            legend2->Clear();
            if(drawVerticalLines){
                for(Int_t lineBin=0; lineBin<nLines; lineBin++){
                    delete verticalLines[lineBin];
                    verticalLines[lineBin] = NULL;
                }
            }
        }
    }

    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*
    //++++++++++++++++++++++++++++++ Combined Ratio Trending Histograms +++++++++++++++++++++++++++++++++++++++*
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*
    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << "Drawing Ratio Trending Histograms" << endl;

    if(doHistsForEverySet) {
        TString* ratioSets[nSets-nData];
        TH1D* ratio[nSets-nData];
        TH1F* ratio2[nSets-nData];
        TString outputDirDataSet;

        if(nSets>1 && nSets>nData) {
            legend->SetNColumns(nData*(nSets-nData));
            Int_t markerStyles[14]={2,4,5,20,21,22,23,24,25,26,27,28,29,30};

            for(Int_t h=0; h<(Int_t) vecHistosTH1D[0].size(); h++) {
                for(Int_t i=0; i<nData; i++) {
                    printf("Drawing histogram %s of period %s \n",vecHistosTH1D[i].at(h)->GetName(),plotDataSets[i].Data());
                    for(Int_t j=0; j<nSets-nData; j++){
                        ratioSets[j] = new TString(Form("%s / %s", plotDataSets[i].Data(), plotDataSets[j+nData].Data()));
                        ratio[j] = new TH1D(Form("%s%i%i",((TH1D*) vecHistosTH1D[i].at(h))->GetName(),i,j),
                                            Form("%s%i%i;%s;%s - Ratio: Data / MC",((TH1D*) vecHistosTH1D[i].at(h))->GetTitle(),i,j,((TH1D*) vecHistosTH1D[i].at(h))->GetXaxis()->GetTitle(),((TH1D*) vecHistosTH1D[i].at(h))->GetYaxis()->GetTitle()),
                                            hNBin,hFBin,hLBin);
                        EditTH1(globalRuns, doEquidistantXaxis, ratio[j], markerStyles[j % 14], hMarkerSize[i], hMarkerColor[i], hLineColor[i]);
                    }

                    for(Int_t b=1; b<hNBin+1; b++){
                        Double_t hData = ((TH1D*) vecHistosTH1D[i].at(h))->GetBinContent(b);
                        Double_t hErrData = ((TH1D*) vecHistosTH1D[i].at(h))->GetBinError(b);
                        for(Int_t j=0; j<nSets-nData; j++){
                            Double_t hMC = ((TH1D*) vecHistosTH1D[j+nData].at(h))->GetBinContent(b);
                            Double_t hErrMC = ((TH1D*) vecHistosTH1D[j+nData].at(h))->GetBinError(b);
                            if(hData!=0 && hMC!=0) {
                                //if(hData/hMC>1.98) ratio[j]->SetBinContent(b,1.98);
                                //else if(hData/hMC<0.02) ratio[j]->SetBinContent(b,0.02);
                                //else {
                                    ratio[j]->SetBinContent(b,hData/hMC);
                                    Double_t hErrRatio = hData*sqrt((hErrData/hData)*(hErrData/hData)+(hErrMC/hMC)*(hErrMC/hMC))/hMC;
                                    ratio[j]->SetBinError(b,hErrRatio);
                                //}
                            }//else ratio[j]->SetBinContent(b,1.98);
                        }
                    }

                    for(Int_t j=0; j<nSets-nData; j++) {
                        TString draw = (/*i==0&&*/j==0)?"p":"p, same";
                        ratio[j]->SetTitle("");
                        ratio[j]->GetYaxis()->SetRangeUser(0.8,1.2);
                        ratio[j]->Draw(draw.Data());
                        legend->AddEntry(ratio[j],ratioSets[j]->Data(),"p");
                    }
                    if (drawVerticalLines){
                        canvas->Update();
                        yMax = canvas->GetUymax();
                        yMin = canvas->GetUymin();
                        for(Int_t lineBin=0; lineBin<nLines; lineBin++){
                            verticalLines[lineBin] = new TLine(runRanges[lineBin],yMin,runRanges[lineBin],yMax);
                            verticalLines[lineBin]->SetLineWidth(1);
                            verticalLines[lineBin]->SetLineStyle(2);
                            verticalLines[lineBin]->Draw("same");
                        }
                    }
                    legend->Draw();
                    outputDirDataSet = Form("%s/%s",outputDir.Data(), DataSets[i].Data());
                    gSystem->Exec("mkdir -p "+outputDirDataSet+"/TrendingRatios");

                    if(doTrigger){
                        PutProcessLabelAndEnergyOnPlot(xPosLabel, 0.92, 0.03, fCollisionSystem.Data(), fTrigger.Data(), "", 62, 0.03, "", 1, 1.25, 31);
                    }else{
                        PutProcessLabelAndEnergyOnPlot(xPosLabel, 0.92, 0.03, fCollisionSystem.Data(), "", "", 62, 0.03, "", 1, 1.25, 31);
                    }

                    PutNameAndJetTypeOnPlot(xPosLabel, 0.84, 0.03, jetType.Data(), "", vecBinRangeTH1D.at(h), 62, 0.03, "", 1, 1.25, 31);

                    if(canvas->GetTopMargin()!=0.06) canvas->SetTopMargin(0.06);
                    SaveCanvas(canvas, Form("%s/TrendingRatios/%s.%s", outputDirDataSet.Data(),Form("%s",((TH1D*) vecHistosTH1D[i].at(h))->GetName()),suffix.Data()));
                    legend->Clear();
                    if(drawVerticalLines){
                        for(Int_t lineBin=0; lineBin<nLines; lineBin++){
                            delete verticalLines[lineBin];
                            verticalLines[lineBin] = NULL;
                        }
                    }
                    //for(Int_t j=0; j<nSets-nData; j++) {
                    //    delete ratio[j];
                    //    delete ratioSets[j];
                    //    ratio[j] = NULL;
                    //    ratioSets[j] = NULL;
                    //}
                }
            }

            for(Int_t j=0; j<nSets-nData; j++) {
                delete ratio[j];
                delete ratioSets[j];
                ratio[j] = NULL;
                ratioSets[j] = NULL;
            }

            for(Int_t h=0; h<(Int_t) vecHistosTH1F[0].size(); h++) {
                for(Int_t i=0; i<nData; i++) {
                    printf("Drawing histogram %s of period %s \n",vecHistosTH1F[i].at(h)->GetName(),plotDataSets[i].Data());
                    for(Int_t j=0; j<nSets-nData; j++){
                        ratioSets[j] = new TString(Form("%s / %s", plotDataSets[i].Data(), plotDataSets[j+nData].Data()));
                        ratio2[j] = new TH1F(Form("%s%i%i",((TH1F*) vecHistosTH1F[i].at(h))->GetName(),i,j),
                                            Form("%s%i%i;%s;%s - Ratio2: Data / MC",((TH1F*) vecHistosTH1F[i].at(h))->GetTitle(),i,j,((TH1F*) vecHistosTH1F[i].at(h))->GetXaxis()->GetTitle(),((TH1F*) vecHistosTH1F[i].at(h))->GetYaxis()->GetTitle()),
                                            hNBin,hFBin,hLBin);
                        EditTH1(globalRuns, doEquidistantXaxis, ratio2[j], markerStyles[j % 14], hMarkerSize[i], hMarkerColor[i], hLineColor[i]);
                    }

                    for(Int_t b=1; b<hNBin+1; b++){
                        Double_t hData = ((TH1F*) vecHistosTH1F[i].at(h))->GetBinContent(b);
                        Double_t hErrData = ((TH1F*) vecHistosTH1F[i].at(h))->GetBinError(b);
                        for(Int_t j=0; j<nSets-nData; j++){
                            Double_t hMC = ((TH1F*) vecHistosTH1F[j+nData].at(h))->GetBinContent(b);
                            Double_t hErrMC = ((TH1F*) vecHistosTH1F[j+nData].at(h))->GetBinError(b);
                            if(hMC!=0) {
                                if(hData/hMC>1.98) ratio2[j]->SetBinContent(b,1.98);
                                else if(hData/hMC<0.02) ratio2[j]->SetBinContent(b,0.02);
                                else {
                                    ratio2[j]->SetBinContent(b,hData/hMC);
                                    Double_t hErrRatio = hData*sqrt((hErrData/hData)*(hErrData/hData)+(hErrMC/hMC)*(hErrMC/hMC))/hMC;
                                    ratio2[j]->SetBinError(b,hErrRatio);
                                }
                            }else ratio2[j]->SetBinContent(b,1.98);
                        }
                    }

                    for(Int_t j=0; j<nSets-nData; j++) {
                        TString draw = (/*i==0&&*/j==0)?"p":"p, same";
                        ratio2[j]->SetTitle("");
                        ratio2[j]->GetYaxis()->SetRangeUser(0.8,1.2);
                        ratio2[j]->Draw(draw.Data());
                        legend->AddEntry(ratio2[j],ratioSets[j]->Data(),"p");
                    }
                    if (drawVerticalLines){
                        canvas->Update();
                        yMax = canvas->GetUymax();
                        yMin = canvas->GetUymin();
                        for(Int_t lineBin=0; lineBin<nLines; lineBin++){
                            verticalLines[lineBin] = new TLine(runRanges[lineBin],yMin,runRanges[lineBin],yMax);
                            verticalLines[lineBin]->SetLineWidth(1);
                            verticalLines[lineBin]->SetLineStyle(2);
                            verticalLines[lineBin]->Draw("same");
                        }
                    }
                    legend->Draw();
                    outputDirDataSet = Form("%s/%s",outputDir.Data(), DataSets[i].Data());
                    gSystem->Exec("mkdir -p "+outputDirDataSet+"/TrendingRatios");

                    if(doTrigger){
                        PutProcessLabelAndEnergyOnPlot(xPosLabel, 0.92, 0.03, fCollisionSystem.Data(), fTrigger.Data(), "", 62, 0.03, "", 1, 1.25, 31);
                    }else{
                        PutProcessLabelAndEnergyOnPlot(xPosLabel, 0.92, 0.03, fCollisionSystem.Data(), "", "", 62, 0.03, "", 1, 1.25, 31);
                    }

                    PutNameAndJetTypeOnPlot(xPosLabel, 0.84, 0.03, jetType.Data(), "", vecBinRangeTH1F.at(h), 62, 0.03, "", 1, 1.25, 31);

                    if(canvas->GetTopMargin()!=0.06) canvas->SetTopMargin(0.06);
                    SaveCanvas(canvas, Form("%s/TrendingRatios/%s.%s", outputDirDataSet.Data(),Form("%s",((TH1F*) vecHistosTH1F[i].at(h))->GetName()),suffix.Data()));
                    legend->Clear();
                    if(drawVerticalLines){
                        for(Int_t lineBin=0; lineBin<nLines; lineBin++){
                            delete verticalLines[lineBin];
                            verticalLines[lineBin] = NULL;
                        }
                    }
                    //for(Int_t j=0; j<nSets-nData; j++) {
                    //    delete ratio2[j];
                    //    ratio2[j] = NULL;
                    //    delete ratioSets[j];
                    //    ratioSets[j] = NULL;
                    //}
                }
            }

            for(Int_t j=0; j<nSets-nData; j++) {
                delete ratio2[j];
                ratio2[j] = NULL;
                delete ratioSets[j];
                ratioSets[j] = NULL;
            }

        }else cout << "...skipped due to nSets<=1 or nSets==nData!" << endl;

    }

    delete legend;
    legend = NULL;
    delete canvas;
    canvas = NULL;

    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*
    //++++++++++++++++++++++++++++++ Create Output ROOT-File +++++++++++++++++++++++++++++++++++++++++++++++++++
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*
    const char* nameOutput;

    for(Int_t i=0; i<nSets; i++){
        fDataSet = vecDataSet.at(i);

        fTriggerSelection.ReplaceAll("default","nodownscalecorr");

        if(useDataRunListForMC && i>=nData) nameOutput = Form("%s/%s/JetQA/%s-%s_JetQARunwise.root",fTriggerSelection.Data(),fEnergyFlag.Data(),fDataSet.Data(),vecDataSet.at(0).Data());
        else nameOutput = Form("%s/%s/JetQA/%s_JetQARunwise.root",fTriggerSelection.Data(),fEnergyFlag.Data(),fDataSet.Data());

        TFile* fOutput = new TFile(nameOutput,"RECREATE");
        cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << "Output file: " << nameOutput << endl;
        cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;

        cout << "Summed NEvents (for normalization) for period " << fDataSet << ": '" << sumEvents[i] << "'" << endl;
        cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
        fLog[i] << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
        fLog[i] << "Output file: " << nameOutput << endl;
        fLog[i] << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
        fLog[i] << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
        fLog[i] << "Summed NEvents (for normalization) for period " << fDataSet << ": '" << sumEvents[i] << "'" << endl;
        fLog[i] << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;

        for(Int_t h=0; h<(Int_t) vecHistosTH1D[i].size(); h++) WriteHistogram(((TH1D*) vecHistosTH1D[i].at(h)));
        for(Int_t h=0; h<(Int_t) vecHistosTH1F[i].size(); h++) WriteHistogram(((TH1F*) vecHistosTH1F[i].at(h)));

        WriteHistogramTH1DVec(fOutput,vecVertexZ[i],"VertexZ");
        if(fEnergyFlag.Contains("PbPb")){
          WriteHistogramTH1DVec(fOutput,vecCent[i],"Cent");
        }
        //WriteHistogramTH1DVec(fOutput,vecClusterCounter[i],"ClusterCounter");
        WriteHistogramTH1DVec(fOutput,vecJetPt[i],"JetPt");
        WriteHistogramTH1DVec(fOutput,vecJetPtMax[i],"JetPtMax");
        if(jetType == "Full Jets"){
          WriteHistogramTH1DMat(fOutput,vecNEF[i],"NEF");
          WriteHistogramTH1DMat(fOutput,vecNch[i],"Nch");
          WriteHistogramTH1DMat(fOutput,vecNne[i],"Nne");
          WriteHistogramTH1DMat(fOutput,vecPtCh[i],"PtCh");
          WriteHistogramTH1DMat(fOutput,vecPtNe[i],"PtNe");
          WriteHistogramTH1DMat(fOutput,vecPtChMax[i],"PtChMax");
          WriteHistogramTH1DMat(fOutput,vecPtNeMax[i],"PtNeMax");
        }
        WriteHistogramTH1DMat(fOutput,vecEtaPhi[i],"EtaPhi");
        if(jetType == "Full Jets"){
          WriteHistogramTH1DMat(fOutput,vecEtaPhiCh[i],"EtaPhiCh");
          WriteHistogramTH1DMat(fOutput,vecEtaPhiNe[i],"EtaPhiNe");
          WriteHistogramTH1DMat(fOutput,vecEtaPhiChMax[i],"EtaPhiChMax");
          WriteHistogramTH1DMat(fOutput,vecEtaPhiNeMax[i],"EtaPhiNeMax");
        }
        WriteHistogramTH1DVec(fOutput,vecClusterTime[i],"ClusterTime");
        WriteHistogramTH1DVec(fOutput,vecJetClusterEnergy[i],"JetClusterEnergy");
        WriteHistogramTH1DMat(fOutput,vecClusterCells[i],"ClusterCells");

        if(i>0) WriteHistogramTH1DVec(fOutput,vecVertexZRatio[i-1],"VertexZ-Ratios");
        if(fEnergyFlag.Contains("PbPb")){
        if(i>0) WriteHistogramTH1DVec(fOutput,vecCentRatio[i-1],"Cent-Ratios");
        }
        //if(i>0) WriteHistogramTH1DVec(fOutput,vecClusterCounterRatio[i-1],"ClusterCounter-Ratios");
        if(i>0) WriteHistogramTH1DVec(fOutput,vecJetPtRatio[i-1],"JetPt-Ratios");
        if(i>0) WriteHistogramTH1DVec(fOutput,vecJetPtMaxRatio[i-1],"JetPtMax-Ratios");
        if(jetType == "Full Jets"){
        if(i>0) WriteHistogramTH1DMat(fOutput,vecNEFRatio[i-1],"NEF-Ratios");
        if(i>0) WriteHistogramTH1DMat(fOutput,vecNchRatio[i-1],"Nch-Ratios");
        if(i>0) WriteHistogramTH1DMat(fOutput,vecNneRatio[i-1],"Nne-Ratios");
        if(i>0) WriteHistogramTH1DMat(fOutput,vecPtChRatio[i-1],"PtCh-Ratios");
        if(i>0) WriteHistogramTH1DMat(fOutput,vecPtNeRatio[i-1],"PtNe-Ratios");
        if(i>0) WriteHistogramTH1DMat(fOutput,vecPtChMaxRatio[i-1],"PtChMax-Ratios");
        if(i>0) WriteHistogramTH1DMat(fOutput,vecPtNeMaxRatio[i-1],"PtNeMax-Ratios");
        }
        if(i>0) WriteHistogramTH1DMat(fOutput,vecEtaPhiRatio[i-1],"EtaPhi-Ratios");
        if(jetType == "Full Jets"){
        if(i>0) WriteHistogramTH1DMat(fOutput,vecEtaPhiChRatio[i-1],"EtaPhiCh-Ratios");
        if(i>0) WriteHistogramTH1DMat(fOutput,vecEtaPhiNeRatio[i-1],"EtaPhiNe-Ratios");
        if(i>0) WriteHistogramTH1DMat(fOutput,vecEtaPhiChMaxRatio[i-1],"EtaPhiChMax-Ratios");
        if(i>0) WriteHistogramTH1DMat(fOutput,vecEtaPhiNeMaxRatio[i-1],"EtaPhiNeMax-Ratios");
        }
        if(i>0) WriteHistogramTH1DVec(fOutput,vecClusterTimeRatio[i-1],"ClusterTime-Ratios");
        if(i>0) WriteHistogramTH1DVec(fOutput,vecJetClusterEnergyRatio[i-1],"JetClusterEnergy-Ratios");
        if(i>0) WriteHistogramTH1DMat(fOutput,vecClusterCellsRatio[i-1],"ClusterCells-Ratios");

        delete[] fitValuesTH1D[i];
        delete[] fitValuesTH1F[i];

        fOutput->Write();
        fOutput->Close();
        delete fOutput;
        fOutput = NULL;
        fLog[i].close();
        fEventLog[i].close();
    }

    delete[] vecMissingRuns;
    delete[] vecLowJetStats;
//    delete[] sumEvents;
    delete[] fLog;
    delete[] fEventLog;
    delete[] vecHistosTH1D;
    delete[] vecHistosTH1F;
    delete[] vecVertexZ;
    delete[] vecVertexZRatio;
    delete[] vecCent;
    delete[] vecCentRatio;
    delete[] vecJetPt;
    delete[] vecJetPtRatio;
    delete[] vecJetPtMax;
    delete[] vecJetPtMaxRatio;
    delete[] vecNEF;
    delete[] vecNEFRatio;
    delete[] vecNch;
    delete[] vecNchRatio;
    delete[] vecNchAll;
    delete[] vecNchAllRatio;
    delete[] vecPtChAll;
    delete[] vecPtChAllRatio;
    delete[] vecNne;
    delete[] vecNneRatio;
    delete[] vecPtCh;
    delete[] vecPtChRatio;
    delete[] vecPtNe;
    delete[] vecPtNeRatio;
    delete[] vecPtChMax;
    delete[] vecPtChMaxRatio;
    delete[] vecPtNeMax;
    delete[] vecPtNeMaxRatio;
    delete[] vecEtaPhi;
    delete[] vecEtaPhiRatio;
    delete[] vecEtaPhiCh;
    delete[] vecEtaPhiChRatio;
    delete[] vecEtaPhiNe;
    delete[] vecEtaPhiNeRatio;
    delete[] vecEtaPhiChMax;
    delete[] vecEtaPhiChMaxRatio;
    delete[] vecEtaPhiNeMax;
    delete[] vecEtaPhiNeMaxRatio;
    delete[] vecClusterTime;
    delete[] vecClusterTimeRatio;
    delete[] vecJetClusterEnergy;
    delete[] vecJetClusterEnergyRatio;
    delete[] vecAllClusterEnergy;
    delete[] vecAllClusterEnergyRatio;
    delete[] vecClusterCells;
    delete[] vecClusterCellsRatio;
    delete[] fitValuesTH1D;
    delete[] fitValuesTH1F;

    TH1::AddDirectory(kTRUE);

    cout << "Done with JetQA_Runwise" << endl;
    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;

    return;

}//end
