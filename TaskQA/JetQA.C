/*******************************************************************************
 ******  provided by Gamma Conversion Group, PWGGA,                        *****
 ******     Daniel Muehlheim, d.muehlheim@cern.ch                          *****
 ******     Friederike Bock, fbock@cern.ch                                 *****
 *******************************************************************************/

#include "QA.h"

//**************************************************************************************************************
//***************************** Main routine *******************************************************************
//**************************************************************************************************************
void JetQA(
                Int_t nSetsIn,                          // number of data sets to be analysed
                TString fEnergyFlag,                    // energy flag
                TString* DataSets,                      // technical names of data sets for output
                TString* plotDataSets,                  // labels of data sets in plots
                TString* pathDataSets,                  // path for data sets
                Int_t triggerNr             = -1,       // if -1: you have to choose number at runtime
                TString jetType         = "",
                TString suffix          = "eps",        // output format of plots
                TString labelData       = "Data",       // Label for data
                Bool_t addSubfolder     = kFALSE        // flag to enable subdirectory creation for primary cut
            )
{
    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << "JetQA" << endl;
    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;

    //**************************************************************************************************************
    //**************************** Setting general plotting style **************************************************
    //**************************************************************************************************************
    gROOT->Reset();
    TH1::AddDirectory(kFALSE);
    StyleSettingsThesis();
    SetPlotStyle();


    //**************************************************************************************************************
    //****************************** Setting common variables ******************************************************
    //**************************************************************************************************************

    const Int_t nSets                   = nSetsIn;
    Int_t radius = 2;
    // enable respective flags

    TString fCollisionSystem            = ReturnFullCollisionsSystem(fEnergyFlag);
    if (fCollisionSystem.CompareTo("") == 0){
        cout << "No correct collision system specification, has been given" << endl;
        return;
    }

    TString fDate                       = ReturnDateString();
    TString fTextMeasurement            = Form("#pi^{0} #rightarrow #gamma#gamma");

    const Int_t maxSets                 = 12;
    //nSets == 0 is always data!

    if(nSets>maxSets){
        cout << "Maximum hardcoded number of Data Sets in EventQA: " << maxSets << endl;
        cout << "You have chosen: " << nSets << ", returning!" << endl;
        return;
    }

    Color_t colorCompare[maxSets]       = {kBlack, kRed+1, kMagenta+2, 807, 800, kGreen+2, kCyan+2, kBlue+1, kOrange+2, kAzure, kViolet, kGray+1};
    Double_t processLabelOffsetX1       = 0.85;
/*
    const Int_t nPtBins     = 13;
    Int_t binsPt[14]        = {0, 10, 20, 30, 40, 50, 60, 70, 80, 90,
                               100, 120, 220, 350};

    const Int_t nEtaBins    = 10;
    Double_t binsEta[11]    = {-1.0, -0.8, -0.6, -0.4, -0.2, 0.0, 0.2, 0.4, 0.6, 0.8,
                               1.0};
    Int_t binsEtaInt[11]    = {0, 10, 20, 30, 40, 50, 60, 70, 80, 90,
                               100};

    const Int_t nEBins      = 12;
    Int_t binsE[13]         = {0,2,4,6,8,10,15,20,30,40,
                               50,100,200};
*/
    const Int_t nAreaBins    = 17;
    Double_t binsArea[18]    = {0.0, 0.05, 0.07, 0.09, 0.11, 0.13, 0.15, 0.17, 0.19, 0.21,
                               0.25, 0.3, 0.35, 0.4, 0.5, 1.0, 1.5, 1.98};
    Int_t binsAreaInt[18]    = {0, 6, 8, 10, 12, 14, 16, 18, 20, 22,
                               26, 31, 36, 41, 51, 101, 151, 200};

    const Int_t nPtBins     = 7;
    Int_t binsPt[8]        = {6, 10, 20, 30, 60, 100, 200, 350};
    Double_t binsPtdoub[8]        = {6., 10., 20., 30., 60., 100., 200., 350.};

    const Int_t nEtaBins    = 3;
    Double_t binsEta[4]    = {-0.9, -0.4, 0.4, 0.9};
    Int_t binsEtaInt[4]    = {6, 31, 71, 96};

    const Int_t nEBins      = 4;
    Int_t binsE[5]         = {1, 2, 5, 10, 30};
    Double_t binsEdoub[5]         = {1., 2., 5., 10., 30.};

    // Trigger name for plots
    TString trigName;
    if (triggerNr == 0) trigName = "EJE";
    else if (triggerNr == 1) trigName = "EMC7";
    else if (triggerNr == 2) trigName = "INT7";
    Float_t xPosLabel = 0.9;

    //*****************************************************************************************************
    //*****************************************************************************************************
    //****************************** Determine which cut to process ***************************************
    //*****************************************************************************************************
    //*****************************************************************************************************

    TString nameTrigger;
    std::vector<TString> triggersTemp[maxSets];
    map<TString,Int_t> mapCuts;

    for(Int_t i=0; i<nSets; i++){
        TFile *fFile = new TFile(pathDataSets[i].Data(),"READ");
        if(fFile->IsZombie()){cout << "ERROR: File " << pathDataSets[i].Data() << " could not be openend! Returning..." << endl; return;}
        else{
            cout << "Processing file: " << pathDataSets[i].Data();
            TKey *key;
            TIter next(fFile->GetListOfKeys());
            while ((key=(TKey*)next())){
                cout << Form(" - Found Directory: %s",key->GetName()) << endl;
                nameTrigger = key->GetName();
                if(nameTrigger.BeginsWith("JetSpectrum_") && nameTrigger.Contains("R02")){
                    triggersTemp[i].push_back(nameTrigger);
                }
                if(nameTrigger.IsNull()){cout << "ERROR, Unable to obtain valid name of Trigger:|" << nameTrigger.Data() << endl; return;}
            }
        cout << endl;
        }
        fFile->Close();
        delete fFile;
    }

    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << "The following triggers are available:" << endl;
    Int_t nullTriggerCounter = 0;
    for(Int_t i = 0; i < nSets; i++) {
        cout << "Data set " << DataSets[i].Data() << endl;
        for(Int_t j = 0; j < (Int_t) triggersTemp[i].size(); j++) cout << Form("(%i) -- %s", j, triggersTemp[i].at(j).Data()) << endl;
        if(triggersTemp[i].size() == 0){
            cout << "No available triggers in this dataset" << endl;
            nullTriggerCounter++;
        }
    }
    if(nullTriggerCounter > nSets-1){cout << "No available triggers in any dataset. Returning." << endl; return;}

    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << "Processing Trigger Number: " << triggerNr << endl;
    cout << triggersTemp[0].at(triggerNr) << endl;
    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << "Pictures are saved as " << suffix.Data() << "!" << endl;
    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;

    Bool_t fIsPbPb = kFALSE;
    Bool_t fIspPb  = kFALSE;
    Bool_t fIspp   = kFALSE;
    Bool_t fIsHeavyIon = kFALSE;
    if(fEnergyFlag.Contains("PbPb")){
      fIsPbPb = kTRUE;
      fIsHeavyIon = kTRUE;
      processLabelOffsetX1 = 0.85;
    } else if(fEnergyFlag.Contains("pPb")){
      fIspPb = kTRUE;
      fIsHeavyIon = kTRUE;
    } else if (fEnergyFlag.Contains("pp")){
      fIspp = kTRUE;
    } else {cout << "ERROR detecting collision system" << endl; return;}

    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << "Obtaining histograms from selected trigger" << endl;
    TString* fTrigger       = new TString[nSets];
    for(Int_t iT=0;iT<nSets;iT++){
        fTrigger[iT] = "";
    }
    fTrigger[0]             = "";

    // *****************************************************************************************************
    // ************************** Define output directories*************************************************
    // *****************************************************************************************************
    TString outputDir                   = "";
    TString outputDirRootFile           = "";

    outputDir                       = Form("%s/%s/JetQA/%s",triggersTemp[0].at(triggerNr).Data(),fEnergyFlag.Data(),suffix.Data());
    outputDirRootFile               = Form("%s/%s/JetQA",triggersTemp[0].at(triggerNr).Data(),fEnergyFlag.Data());

    if(addSubfolder) outputDir          +=Form("/%s",DataSets[0].Data());
    gSystem->Exec("mkdir -p "+outputDir);
    gSystem->Exec("mkdir -p "+outputDir+"/Comparison");
    gSystem->Exec("mkdir -p "+outputDir+"/Comparison/Ratios");


    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;

    // *****************************************************************************************************
    // ******************************* create log file *****************************************************
    // *****************************************************************************************************

    fstream fLog;
    fLog.open(Form("%s/A-%s.log",outputDirRootFile.Data(),DataSets[0].Data()), ios::out);
    fLog << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    for(Int_t i=0; i<nSets; i++) fLog << "Using file: " << pathDataSets[i].Data() << endl;
    fLog << fCollisionSystem.Data() << endl;
    fLog << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    fLog << "processed cut: " << triggersTemp[0].at(triggerNr).Data() << endl;

    // *****************************************************************************************************
    // *****************************************************************************************************
    // ****************************** Vectors for Histograms ***********************************************
    // *****************************************************************************************************
    // *****************************************************************************************************

    std::vector<TH1D*> vecNEvents;
    std::vector<TH1D*> vecVertexZ;
    std::vector<TH1D*> vecCentrality;
    std::vector<TH1D*> vecCentrality2;
    std::vector<TH1D*> vecEventPlaneAngle;
    std::vector<TH1D*> vecJetPt;
    std::vector<TH1D*> vecJetPtMax;
    std::vector<TH1D*> vecClusterEnergy;
    std::vector<TH1D*> vecClusterTime;

    std::vector<TH1D*> vecNEF[nPtBins];
    std::vector<TH1D*> vecZch[nPtBins];
    std::vector<TH1D*> vecZne[nPtBins];
    std::vector<TH1D*> vecZchMax[nPtBins];
    std::vector<TH1D*> vecZneMax[nPtBins];
    std::vector<TH1D*> vecNch[nPtBins];
    std::vector<TH1D*> vecNne[nPtBins];
    std::vector<TH1D*> vecPtCh[nPtBins];
    std::vector<TH1D*> vecPtNe[nPtBins];
    std::vector<TH1D*> vecPtChMax[nPtBins];
    std::vector<TH1D*> vecPtNeMax[nPtBins];
    std::vector<TH1D*> vecJetArea[nPtBins];

    std::vector<TH1D*> vecEtaPhi[nEtaBins];
    std::vector<TH1D*> vecEtaPhiCh[nEtaBins];
    std::vector<TH1D*> vecEtaPhiNe[nEtaBins];
    std::vector<TH1D*> vecEtaPhiChMax[nEtaBins];
    std::vector<TH1D*> vecEtaPhiNeMax[nEtaBins];

    std::vector<TH1D*> vecNConst[nAreaBins];

    std::vector<TH1D*> vecM02[nEBins];
    std::vector<TH1D*> vecClusterCells[nEBins];

    Bool_t hasCentHist = kFALSE;
    Double_t* nEventsAll    = new Double_t[nSets];
    Double_t* nEvents       = new Double_t[nSets];

    Int_t minB          = 0;    Int_t maxB          = 0;
    Int_t minYB         = 0;    Int_t maxYB         = 0;

    Int_t minB_SPD      = 0;    Int_t maxB_SPD      = 0;
    Int_t minYB_SPD     = 0;    Int_t maxYB_SPD     = 0;
    Bool_t isMinMaxSPD  = kTRUE;

    // *****************************************************************************************************
    // *****************************************************************************************************
    // ****************************** Looping over DataSets ************************************************
    // *****************************************************************************************************
    // *****************************************************************************************************

    // canvas definition
    TCanvas* canvas             = new TCanvas("canvas","",10,10,750,500);  // gives the page size
    TCanvas* canvas2            = new TCanvas("canvas2","",10,10,750,500);  // gives the page size
    TCanvas* cvsQuadratic       = new TCanvas("cvsQuadratic","",10,10,500,500);  // gives the page size
    Double_t leftMargin         = 0.075;
    Double_t leftMargin2        = 0.09;
    Double_t rightMargin        = 0.02;
    Double_t topMargin          = 0.04;
    Double_t bottomMargin       = 0.09;
    Double_t topMarginQuad      = 0.014;
    Double_t bottomMarginQuad   = 0.092;
    Double_t leftMarginQuad     = 0.117;
    Double_t rightMarginQuad    = 0.117;
    DrawGammaCanvasSettings(canvas,leftMargin,rightMargin,topMargin,bottomMargin);
    DrawGammaCanvasSettings(canvas2,leftMargin2,rightMargin,topMargin,bottomMargin);
    DrawGammaCanvasSettings(cvsQuadratic,leftMarginQuad,rightMarginQuad,topMarginQuad,bottomMarginQuad);

    for(Int_t i=0; i<nSets; i++) {
        TFile *fFile = new TFile(pathDataSets[i].Data(),"READ");
        if(fFile->IsZombie()) {
            cout << "ERROR: ROOT file '" << pathDataSets[i].Data() << "' could not be openend, returning!" << endl;
            fLog << "ERROR: ROOT file '" << pathDataSets[i].Data() << "' could not be openend, returning!" << endl;
            return;
        }
        //-------------------------------------------------------------------------------------------------------------------------------
        // reading respective containers
        TDirectory *TopDir                      = (TDirectory*)fFile->Get(triggersTemp[i].at(triggerNr).Data());
        if(TopDir == NULL) {cout << "ERROR: TopDir not Found"<<endl; return;}
        TList* HistoList                   = (TList*)TopDir->Get(triggersTemp[i].at(triggerNr).Data());
        if(HistoList == NULL) {cout << "ERROR: " << triggersTemp[0].at(triggerNr).Data() << " not found in File" << endl; return;}
        else HistoList->SetOwner(kTRUE);
        if(i==0){
            cout << "Histogram list from trigger output directory" << endl;
            for(Int_t j = 0; j<HistoList->GetSize(); j++){
                TObject *histoObject = (TObject*)HistoList->At(j);
                cout << histoObject->GetName() << endl;
            }
        }

        //-------------------------------------------------------------------------------------------------------------------------------
        nEvents[i]          = 0;
        nEventsAll[i]       = 0;
        TH1D* fHistNEvents  = NULL;
        fHistNEvents        = (TH1D*)HistoList->FindObject("hClusterCounter");
        if(fHistNEvents){
            nEvents[i]     = (Double_t)fHistNEvents->GetBinContent(1);
        }else{
            cout << "ERROR: Object |fHistNEvents| could not be found in File '" << pathDataSets[i].Data() << "'! Returning..." << endl;
            return;
        }
        //-------------------------------------------------------------------------------------------------------------------------------
        cout << endl;
        cout << "----------------------------------------------------------------------------" << endl;
        cout << "Processing file: " << pathDataSets[i].Data() << endl;
        cout << "Set: " << plotDataSets[i].Data() << endl;
        cout << "NEvents (for normalization): '" << nEvents[i] << "'" << endl;
        cout << "----------------------------------------------------------------------------" << endl;
        cout << endl;
        fLog << "----------------------------------------------------------------------------" << endl;
        fLog << "Processing file: " << pathDataSets[i].Data() << endl;
        fLog << "Set: " << plotDataSets[i].Data() << endl;
        fLog << "NEvents (for normalization): '" << nEvents[i] << "'" << endl;
        fLog << "----------------------------------------------------------------------------" << endl;
        //-------------------------------------------------------------------------------------------------------------------------------
        if( nEvents[i] < 1. ){cout << "ERROR: number of accepted events in data set is " << nEvents[i] << "! Did you set the correct trigger in the configuration file? Returning..." << endl; return;}
        //-------------------------------------------------------------------------------------------------------------------------------
        const char* nameOutput = Form("%s/JetQA_%s.root",outputDirRootFile.Data(),DataSets[i].Data());
        TFile* fOutput = new TFile(nameOutput,"RECREATE");
        cout << endl;
        cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << "Output file: " << nameOutput << endl;
        cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << endl;
        fLog << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
        fLog << "Output file: " << nameOutput << endl;
        fLog << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;

        cout << "Drawing event property histograms" << endl;
        //-------------------------------------------------------------------------------------------------------------------------------
        //----------------------------------------------- Event properties --------------------------------------------------------------
        //-------------------------------------------------------------------------------------------------------------------------------
        // Event counting histos
        if(fHistNEvents){
            AdjustHistRange(fHistNEvents,1,10,kTRUE,1,0.1);
            DrawPeriodQAHistoTH1(canvas,leftMargin,rightMargin,topMargin,bottomMargin,kFALSE,kTRUE,kFALSE,
                                fHistNEvents,"","","# of Entries",1,1,
                                processLabelOffsetX1,0.94,0.03,fCollisionSystem,plotDataSets[i],fTrigger[i]);
            SaveCanvasAndWriteHistogram(canvas, fHistNEvents, Form("%s/NEvents_%s.%s", outputDir.Data(), DataSets[i].Data(), suffix.Data()));
            vecNEvents.push_back(new TH1D(*fHistNEvents));
        }
        //-------------------------------------------------------------------------------------------------------------------------------
        // vertex Z distribution
        TH1D* fHistVertexZ = (TH1D*)HistoList->FindObject("Vtz_selected");
        if(fHistVertexZ){
            GetMinMaxBin(fHistVertexZ,minB,maxB);
            SetXRange(fHistVertexZ,minB,maxB);
            DrawPeriodQAHistoTH1(canvas,leftMargin,rightMargin,topMargin,bottomMargin,kFALSE,kFALSE,kFALSE,
                                fHistVertexZ,"","z-Vertex (cm)","#frac{dN}{dz}",1,1,
                                processLabelOffsetX1,0.94,0.03,fCollisionSystem,plotDataSets[i],fTrigger[i]);
            WriteHistogram(fHistVertexZ);
            vecVertexZ.push_back(new TH1D(*fHistVertexZ));
        } else cout << "INFO: Object |fHistVertexZ| could not be found! Skipping Draw..." << endl;
        //-------------------------------------------------------------------------------------------------------------------------------
        // centrality distribution stored in main task
/*        TH1D* fHistCentMain = (TH1D*)HistoList->FindObject("hEventCentrality");
        if(fHistCentMain){
            GetMinMaxBin(fHistCentMain,minB,maxB);
            SetXRange(fHistCentMain,0,100);
            DrawPeriodQAHistoTH1(canvas,leftMargin,rightMargin,topMargin,bottomMargin,kFALSE,kFALSE,kFALSE,
                                 fHistCentMain,"","Centrality (%)","#events",1,1,
                                 0.95,0.24,0.03,fCollisionSystem,plotDataSets[i],fTrigger[i]);
            SaveCanvasAndWriteHistogram(canvas, fHistCentMain, Form("%s/CentralityMain_%s.%s", outputDir.Data(), DataSets[i].Data(), suffix.Data()));
            WriteHistogram(fHistCentMain);
            vecCentrality.push_back(new TH1D(*fHistCentMain));
        } else cout << "INFO: Object |Centrality| could not be found! Skipping Draw..." << endl;
        //-------------------------------------------------------------------------------------------------------------------------------
        // centrality
        if(fIsPbPb){
            TH1D* fHistCentrality = (TH1D*)ESDContainer->FindObject("Centrality");
            if(fHistCentrality){
                fHistCentrality->Rebin(4);
                GetMinMaxBin(fHistCentrality,minB,maxB);
                SetXRange(fHistCentrality,minB,maxB);
                DrawPeriodQAHistoTH1(   canvas,leftMargin,rightMargin,topMargin,bottomMargin,kFALSE,kFALSE,kFALSE,
                                        fHistCentrality,"","Centrality (%)","",1,1,
                                        0.95,0.24,0.03,fCollisionSystem,plotDataSets[i],fTrigger[i]);
                WriteHistogram(fHistCentrality);
                SaveCanvasAndWriteHistogram(canvas, fHistCentMain, Form("%s/CentralityMain2_%s.%s", outputDir.Data(), DataSets[i].Data(), suffix.Data()));
                vecCentrality.push_back(new TH1D(*fHistCentrality));
            } else cout << "INFO: Object |fHistCentrality| could not be found! Skipping Draw..." << endl;
            // event plane angle
            TH1D* fHistEventPlaneAngle = (TH1D*)ConvEventCutsContainer->FindObject(Form("EventPlaneAngle %s",fEventCutSelection[i].Data()));
            if(fHistEventPlaneAngle){
                GetMinMaxBin(fHistEventPlaneAngle,minB,maxB);
                    SetXRange(fHistEventPlaneAngle,minB,maxB);
                    DrawPeriodQAHistoTH1(   canvas,leftMargin,rightMargin,topMargin,bottomMargin,kFALSE,kFALSE,kFALSE,
                                            fHistEventPlaneAngle,"","event plane angle (rad)","",1,1,
                                            processLabelOffsetX1,0.94,0.03,fCollisionSystem,plotDataSets[i],fTrigger[i]);
                    WriteHistogram(fHistEventPlaneAngle);
                    vecEventPlaneAngle.push_back(new TH1D(*fHistEventPlaneAngle));
            } else cout << "INFO: Object |fHistEventPlaneAngle| could not be found! Skipping Draw..." << endl;
        }
        TH1D* fHistCentrality2 = (TH1D*)ConvEventCutsContainer->FindObject(Form("Centrality %s",fEventCutSelection[i].Data()));
        if(fHistCentrality2){
            GetMinMaxBin(fHistCentrality2,minB,maxB);
            fHistCentrality2->Rebin(4);
            SetXRange(fHistCentrality2,0,100);
            DrawPeriodQAHistoTH1(   canvas,leftMargin,rightMargin,topMargin,bottomMargin,kFALSE,kFALSE,kFALSE,
                                    fHistCentrality2,"","Centrality (%)","",1,1,
                                    0.95,0.24,0.03,fCollisionSystem,plotDataSets[i],fTrigger[i]);
            WriteHistogram(fHistCentrality2);
            SaveCanvasAndWriteHistogram(canvas, fHistCentrality2, Form("%s/CentralityEventCuts_%s.%s", outputDir.Data(), DataSets[i].Data(), suffix.Data()));
            vecCentrality2.push_back(new TH1D(*fHistCentrality2));
            hasCentHist             = kTRUE;
        } else cout << "INFO: Object |fHistCentrality2| could not be found! Skipping Draw..." << endl;
*/
        //-------------------------------------------------------------------------------------------------------------------------------
        // Jet Spectrum
        TH2D* hJetSpectrum2D = (TH2D*)HistoList->FindObject("hJetSpectrum");
        if(hJetSpectrum2D){
            TH1D* hJetSpectrum = (TH1D*)hJetSpectrum2D->ProjectionY("JetPt",0,1);
            GetMinMaxBin(hJetSpectrum,minB,maxB);
            SetXRange(hJetSpectrum,minB,maxB);
            DrawPeriodQAHistoTH1(canvas,leftMargin,rightMargin,topMargin,bottomMargin,kFALSE,kFALSE,kFALSE,
                                 hJetSpectrum,"","p_{T}^{jet}","#frac{dN}{dp_{T}^{jet}}",1,1,
                                 processLabelOffsetX1,0.94,0.03,fCollisionSystem,plotDataSets[i],fTrigger[i]);
            WriteHistogram(hJetSpectrum);
            vecJetPt.push_back(new TH1D(*hJetSpectrum));
        } else cout << "INFO: Object |hJetSpectrum| could not be found! Skipping Draw..." << endl;
        //-------------------------------------------------------------------------------------------------------------------------------
        // Max Jet Spectrum
        TH2D* hJetSpectrumMax2D = (TH2D*)HistoList->FindObject("hJetSpectrumMax");
        if(hJetSpectrumMax2D){
            TH1D* hJetSpectrumMax = (TH1D*)hJetSpectrumMax2D->ProjectionY("JetPtMax",0,1);
            GetMinMaxBin(hJetSpectrumMax,minB,maxB);
            SetXRange(hJetSpectrumMax,minB,maxB);
            DrawPeriodQAHistoTH1(canvas,leftMargin,rightMargin,topMargin,bottomMargin,kFALSE,kFALSE,kFALSE,
                                 hJetSpectrumMax,"","p_{T}^{jet,max}","#frac{dN}{dp_{T}^{jet,max}}",1,1,
                                 processLabelOffsetX1,0.94,0.03,fCollisionSystem,plotDataSets[i],fTrigger[i]);
            WriteHistogram(hJetSpectrumMax);
            vecJetPtMax.push_back(new TH1D(*hJetSpectrumMax));
        } else cout << "INFO: Object |hJetSpectrumMax| could not be found! Skipping Draw..." << endl;
        //-------------------------------------------------------------------------------------------------------------------------------
        // Neutral Energy Fraction
        TH2D* hNEF2D = (TH2D*)HistoList->FindObject("hQANEFPt");
        if(hNEF2D){
            for (Int_t binPt = 0; binPt < nPtBins; binPt++){
                TH1D* hNEF = (TH1D*)hNEF2D->ProjectionY(Form("hNEF_%i-%iGeV", binsPt[binPt], binsPt[binPt+1]),binsPt[binPt],binsPt[binPt+1]);
                GetMinMaxBin(hNEF,minB,maxB);
                SetXRange(hNEF,minB,maxB);
                DrawPeriodQAHistoTH1(canvas,leftMargin,rightMargin,topMargin,bottomMargin,kFALSE,kFALSE,kFALSE,
                                     hNEF,"","NEF","#frac{dN^{jet}}{dNEF}",1,1,
                                     processLabelOffsetX1,0.94,0.03,fCollisionSystem,plotDataSets[i],fTrigger[i]);
                WriteHistogram(hNEF);
                vecNEF[binPt].push_back(new TH1D(*hNEF));
            }
        } else cout << "INFO: Object |hNEF| could not be found! Skipping Draw..." << endl;
        //-------------------------------------------------------------------------------------------------------------------------------
        // Z Charged
        TH2D* hZch2D = (TH2D*)HistoList->FindObject("hQAZchPt");
        if(hZch2D){
            for (Int_t binPt = 0; binPt < nPtBins; binPt++){
                TH1D* hZch = (TH1D*)hZch2D->ProjectionY(Form("hZch_%i-%iGeV", binsPt[binPt], binsPt[binPt+1]),binsPt[binPt],binsPt[binPt+1]);
                GetMinMaxBin(hZch,minB,maxB);
                SetXRange(hZch,minB,maxB);
                DrawPeriodQAHistoTH1(canvas,leftMargin,rightMargin,topMargin,bottomMargin,kFALSE,kFALSE,kFALSE,
                                     hZch,"","z_{ch}","#frac{dN^{jet}}{dz_{ch}",1,1,
                                     processLabelOffsetX1,0.94,0.03,fCollisionSystem,plotDataSets[i],fTrigger[i]);
                WriteHistogram(hZch);
                vecZch[binPt].push_back(new TH1D(*hZch));
            }
        } else cout << "INFO: Object |hZch| could not be found! Skipping Draw..." << endl;
        //-------------------------------------------------------------------------------------------------------------------------------
        // Z Neutral
        TH2D* hZne2D = (TH2D*)HistoList->FindObject("hQAZnePt");
        if(hZne2D){
            for (Int_t binPt = 0; binPt < nPtBins; binPt++){
                TH1D* hZne = (TH1D*)hZne2D->ProjectionY(Form("hZne_%i-%iGeV", binsPt[binPt], binsPt[binPt+1]),binsPt[binPt],binsPt[binPt+1]);
                GetMinMaxBin(hZne,minB,maxB);
                SetXRange(hZne,minB,maxB);
                DrawPeriodQAHistoTH1(canvas,leftMargin,rightMargin,topMargin,bottomMargin,kFALSE,kFALSE,kFALSE,
                                     hZne,"","z_{ne}","#frac{dN^{jet}}{dz_{ne}",1,1,
                                     processLabelOffsetX1,0.94,0.03,fCollisionSystem,plotDataSets[i],fTrigger[i]);
                WriteHistogram(hZne);
                vecZne[binPt].push_back(new TH1D(*hZne));
            }
        } else cout << "INFO: Object |hZne| could not be found! Skipping Draw..." << endl;
        //-------------------------------------------------------------------------------------------------------------------------------
        // Z Charged Max
        TH2D* hZchMax2D = (TH2D*)HistoList->FindObject("hQAZchPtMax");
        if(hZchMax2D){
            for (Int_t binPt = 0; binPt < nPtBins; binPt++){
                TH1D* hZchMax = (TH1D*)hZchMax2D->ProjectionY(Form("hZchMax_%i-%iGeV", binsPt[binPt], binsPt[binPt+1]),binsPt[binPt],binsPt[binPt+1]);
                GetMinMaxBin(hZchMax,minB,maxB);
                SetXRange(hZchMax,minB,maxB);
                DrawPeriodQAHistoTH1(canvas,leftMargin,rightMargin,topMargin,bottomMargin,kFALSE,kFALSE,kFALSE,
                                     hZchMax,"","z_{ch}^{max}","#frac{dN^{jet}}{dz_{ch}^{max}}",1,1,
                                     processLabelOffsetX1,0.94,0.03,fCollisionSystem,plotDataSets[i],fTrigger[i]);
                WriteHistogram(hZchMax);
                vecZchMax[binPt].push_back(new TH1D(*hZchMax));
            }
        } else cout << "INFO: Object |hZchMax| could not be found! Skipping Draw..." << endl;
        //-------------------------------------------------------------------------------------------------------------------------------
        // Z Neutral Max
        TH2D* hZneMax2D = (TH2D*)HistoList->FindObject("hQAZnePtMax");
        if(hZneMax2D){
            for (Int_t binPt = 0; binPt < nPtBins; binPt++){
                TH1D* hZneMax = (TH1D*)hZneMax2D->ProjectionY(Form("hZneMax_%i-%iGeV", binsPt[binPt], binsPt[binPt+1]),binsPt[binPt],binsPt[binPt+1]);
                GetMinMaxBin(hZneMax,minB,maxB);
                SetXRange(hZneMax,minB,maxB);
                DrawPeriodQAHistoTH1(canvas,leftMargin,rightMargin,topMargin,bottomMargin,kFALSE,kFALSE,kFALSE,
                                     hZneMax,"","z_{ne}^{max}","#frac{dN^{jet}}{dz_{ne}^{max}}",1,1,
                                     processLabelOffsetX1,0.94,0.03,fCollisionSystem,plotDataSets[i],fTrigger[i]);
                WriteHistogram(hZneMax);
                vecZneMax[binPt].push_back(new TH1D(*hZneMax));
            }
        } else cout << "INFO: Object |hZneMax| could not be found! Skipping Draw..." << endl;
        //-------------------------------------------------------------------------------------------------------------------------------
        // Number of Charged Constituents
        TH2D* hNch2D = (TH2D*)HistoList->FindObject("hQANChPt");
        if(hNch2D){
            for (Int_t binPt = 0; binPt < nPtBins; binPt++){
                TH1D* hNch = (TH1D*)hNch2D->ProjectionY(Form("hNch_%i-%iGeV", binsPt[binPt], binsPt[binPt+1]),binsPt[binPt],binsPt[binPt+1]);
                GetMinMaxBin(hNch,minB,maxB);
                SetXRange(hNch,minB,maxB);
                DrawPeriodQAHistoTH1(canvas,leftMargin,rightMargin,topMargin,bottomMargin,kFALSE,kFALSE,kFALSE,
                                     hNch,"","N_{ch}^{const}","#frac{dN^{jet}}{dN_{ch}^{const}}",1,1,
                                     processLabelOffsetX1,0.94,0.03,fCollisionSystem,plotDataSets[i],fTrigger[i]);
                WriteHistogram(hNch);
                vecNch[binPt].push_back(new TH1D(*hNch));
            }
        } else cout << "INFO: Object |hNch| could not be found! Skipping Draw..." << endl;
        //-------------------------------------------------------------------------------------------------------------------------------
        // Number of Neutral Constituents
        TH2D* hNne2D = (TH2D*)HistoList->FindObject("hQANnePt");
        if(hNch2D){
            for (Int_t binPt = 0; binPt < nPtBins; binPt++){
                TH1D* hNne = (TH1D*)hNne2D->ProjectionY(Form("hNne_%i-%iGeV", binsPt[binPt], binsPt[binPt+1]),binsPt[binPt],binsPt[binPt+1]);
                GetMinMaxBin(hNne,minB,maxB);
                SetXRange(hNne,minB,maxB);
                DrawPeriodQAHistoTH1(canvas,leftMargin,rightMargin,topMargin,bottomMargin,kFALSE,kFALSE,kFALSE,
                                     hNne,"","N_{ne}^{const}","#frac{dN^{jet}}{dN_{ne}^{const}}",1,1,
                                     processLabelOffsetX1,0.94,0.03,fCollisionSystem,plotDataSets[i],fTrigger[i]);
                WriteHistogram(hNne);
                vecNne[binPt].push_back(new TH1D(*hNne));
            }
        } else cout << "INFO: Object |hNch| could not be found! Skipping Draw..." << endl;
        //-------------------------------------------------------------------------------------------------------------------------------
        // Pt of Charged Constituents
        TH2D* hPtCh2D = (TH2D*)HistoList->FindObject("hQAConstPtCh");
        if(hPtCh2D){
            for (Int_t binPt = 0; binPt < nPtBins; binPt++){
                TH1D* hPtCh = (TH1D*)hPtCh2D->ProjectionY(Form("hPtCh_%i-%iGeV", binsPt[binPt], binsPt[binPt+1]),binsPt[binPt],binsPt[binPt+1]);
                GetMinMaxBin(hPtCh,minB,maxB);
                SetXRange(hPtCh,minB,maxB);
                DrawPeriodQAHistoTH1(canvas,leftMargin,rightMargin,topMargin,bottomMargin,kFALSE,kFALSE,kFALSE,
                                     hPtCh,"","p_{T,ch}^{const}","#frac{dN^{jet}}{dp_{T,ch}^{const}}",1,1,
                                     processLabelOffsetX1,0.94,0.03,fCollisionSystem,plotDataSets[i],fTrigger[i]);
                WriteHistogram(hPtCh);
                vecPtCh[binPt].push_back(new TH1D(*hPtCh));
            }
        } else cout << "INFO: Object |hPtCh| could not be found! Skipping Draw..." << endl;
        //-------------------------------------------------------------------------------------------------------------------------------
        // Pt of Neutral Constituents
        TH2D* hPtNe2D = (TH2D*)HistoList->FindObject("hQAConstPtNe");
        if(hPtNe2D){
            for (Int_t binPt = 0; binPt < nPtBins; binPt++){
                TH1D* hPtNe = (TH1D*)hPtNe2D->ProjectionY(Form("hPtNe_%i-%iGeV", binsPt[binPt], binsPt[binPt+1]),binsPt[binPt],binsPt[binPt+1]);
                GetMinMaxBin(hPtNe,minB,maxB);
                SetXRange(hPtNe,minB,maxB);
                DrawPeriodQAHistoTH1(canvas,leftMargin,rightMargin,topMargin,bottomMargin,kFALSE,kFALSE,kFALSE,
                                     hPtNe,"","p_{T,ne}^{const}","#frac{dN^{jet}}{dp_{T,ne}^{const}}",1,1,
                                     processLabelOffsetX1,0.94,0.03,fCollisionSystem,plotDataSets[i],fTrigger[i]);
                WriteHistogram(hPtNe);
                vecPtNe[binPt].push_back(new TH1D(*hPtNe));
            }
        } else cout << "INFO: Object |hPtNe| could not be found! Skipping Draw..." << endl;
        //-------------------------------------------------------------------------------------------------------------------------------
        // Pt of Max Charged Constituents
        TH2D* hPtChMax2D = (TH2D*)HistoList->FindObject("hQAConstPtChMax");
        if(hPtChMax2D){
            for (Int_t binPt = 0; binPt < nPtBins; binPt++){
                TH1D* hPtChMax = (TH1D*)hPtChMax2D->ProjectionY(Form("hPtChMax_%i-%iGeV", binsPt[binPt], binsPt[binPt+1]),binsPt[binPt],binsPt[binPt+1]);
                GetMinMaxBin(hPtChMax,minB,maxB);
                SetXRange(hPtChMax,minB,maxB);
                DrawPeriodQAHistoTH1(canvas,leftMargin,rightMargin,topMargin,bottomMargin,kFALSE,kFALSE,kFALSE,
                                     hPtChMax,"","p_{T,ch}^{const,max}","#frac{dN^{jet}}{dp_{T,ch}^{const,max}}",1,1,
                                     processLabelOffsetX1,0.94,0.03,fCollisionSystem,plotDataSets[i],fTrigger[i]);
                WriteHistogram(hPtChMax);
                vecPtChMax[binPt].push_back(new TH1D(*hPtChMax));
            }
        } else cout << "INFO: Object |hPtChMax| could not be found! Skipping Draw..." << endl;
        //-------------------------------------------------------------------------------------------------------------------------------
        // Pt of Max Neutral Constituents
        TH2D* hPtNeMax2D = (TH2D*)HistoList->FindObject("hQAConstPtNeMax");
        if(hPtNeMax2D){
            for (Int_t binPt = 0; binPt < nPtBins; binPt++){
                TH1D* hPtNeMax = (TH1D*)hPtNeMax2D->ProjectionY(Form("hPtNeMax_%i-%iGeV", binsPt[binPt], binsPt[binPt+1]),binsPt[binPt],binsPt[binPt+1]);
                GetMinMaxBin(hPtNeMax,minB,maxB);
                SetXRange(hPtNeMax,minB,maxB);
                DrawPeriodQAHistoTH1(canvas,leftMargin,rightMargin,topMargin,bottomMargin,kFALSE,kFALSE,kFALSE,
                                     hPtNeMax,"","p_{T,ne}^{const,max}","#frac{dN^{jet}}{dp_{T,ne}^{const,max}}",1,1,
                                     processLabelOffsetX1,0.94,0.03,fCollisionSystem,plotDataSets[i],fTrigger[i]);
                WriteHistogram(hPtNeMax);
                vecPtNeMax[binPt].push_back(new TH1D(*hPtNeMax));
            }
        } else cout << "INFO: Object |hPtNeMax| could not be found! Skipping Draw..." << endl;
        //-------------------------------------------------------------------------------------------------------------------------------
        // Eta-Phi Distribution
        TH2D* hEtaPhi2D = (TH2D*)HistoList->FindObject("hQAEtaPhi");
        if(hEtaPhi2D){
            for (Int_t binEta = 0; binEta < nEtaBins; binEta++){
                TH1D* hEtaPhi = (TH1D*)hEtaPhi2D->ProjectionY(Form("hEtaPhi_%1.1f_to_%1.1f", binsEta[binEta],binsEta[binEta+1]),binsEta[binEta],binsEta[binEta+1]);
                GetMinMaxBin(hEtaPhi,minB,maxB);
                SetXRange(hEtaPhi,minB,maxB);
                DrawPeriodQAHistoTH1(canvas,leftMargin,rightMargin,topMargin,bottomMargin,kFALSE,kFALSE,kFALSE,
                                     hEtaPhi,"","#phi","#frac{dN^{jet}}{d#phi}",1,1,
                                     processLabelOffsetX1,0.94,0.03,fCollisionSystem,plotDataSets[i],fTrigger[i]);
                WriteHistogram(hEtaPhi);
                vecEtaPhi[binEta].push_back(new TH1D(*hEtaPhi));
            }
        } else cout << "INFO: Object |hEtaPhi| could not be found! Skipping Draw..." << endl;
        //-------------------------------------------------------------------------------------------------------------------------------
        // Eta-Phi Distribution of Charged Constituents
        TH2D* hEtaPhiCh2D = (TH2D*)HistoList->FindObject("hQAEtaPhiConstCh");
        if(hEtaPhiCh2D){
            for (Int_t binEta = 0; binEta < nEtaBins; binEta++){
                TH1D* hEtaPhiCh = (TH1D*)hEtaPhiCh2D->ProjectionY(Form("hEtaPhiCh_%1.1f_to_%1.1f", binsEta[binEta],binsEta[binEta+1]),binsEtaInt[binEta],binsEtaInt[binEta+1]);
                GetMinMaxBin(hEtaPhiCh,minB,maxB);
                SetXRange(hEtaPhiCh,minB,maxB);
                DrawPeriodQAHistoTH1(canvas,leftMargin,rightMargin,topMargin,bottomMargin,kFALSE,kFALSE,kFALSE,
                                     hEtaPhiCh,"","#phi","#frac{dN^{jet}}{d#phi}",1,1,
                                     processLabelOffsetX1,0.94,0.03,fCollisionSystem,plotDataSets[i],fTrigger[i]);
                WriteHistogram(hEtaPhiCh);
                vecEtaPhiCh[binEta].push_back(new TH1D(*hEtaPhiCh));
            }
        } else cout << "INFO: Object |hEtaPhiCh| could not be found! Skipping Draw..." << endl;
        //-------------------------------------------------------------------------------------------------------------------------------
        // Eta-Phi Distribution of Neutral Constituents
        TH2D* hEtaPhiNe2D = (TH2D*)HistoList->FindObject("hQAEtaPhiConstNe");
        if(hEtaPhiNe2D){
            for (Int_t binEta = 0; binEta < nEtaBins; binEta++){
                TH1D* hEtaPhiNe = (TH1D*)hEtaPhiNe2D->ProjectionY(Form("hEtaPhiNe_%1.1f_to_%1.1f", binsEta[binEta],binsEta[binEta+1]),binsEtaInt[binEta],binsEtaInt[binEta+1]);
                GetMinMaxBin(hEtaPhiNe,minB,maxB);
                SetXRange(hEtaPhiNe,minB,maxB);
                DrawPeriodQAHistoTH1(canvas,leftMargin,rightMargin,topMargin,bottomMargin,kFALSE,kFALSE,kFALSE,
                                     hEtaPhiNe,"","#phi","#frac{dN^{jet}}{d#phi}",1,1,
                                     processLabelOffsetX1,0.94,0.03,fCollisionSystem,plotDataSets[i],fTrigger[i]);
                WriteHistogram(hEtaPhiNe);
                vecEtaPhiNe[binEta].push_back(new TH1D(*hEtaPhiNe));
            }
        } else cout << "INFO: Object |hEtaPhiNe| could not be found! Skipping Draw..." << endl;
        //-------------------------------------------------------------------------------------------------------------------------------
        // Eta-Phi Distribution of Max Charged Constituents
        TH2D* hEtaPhiChMax2D = (TH2D*)HistoList->FindObject("hQAEtaPhiMaxConstCh");
        if(hEtaPhiChMax2D){
            for (Int_t binEta = 0; binEta < nEtaBins; binEta++){
                TH1D* hEtaPhiChMax = (TH1D*)hEtaPhiChMax2D->ProjectionY(Form("hEtaPhiChMax_%1.1f_to_%1.1f", binsEta[binEta],binsEta[binEta+1]),binsEtaInt[binEta],binsEtaInt[binEta+1]);
                GetMinMaxBin(hEtaPhiChMax,minB,maxB);
                SetXRange(hEtaPhiChMax,minB,maxB);
                DrawPeriodQAHistoTH1(canvas,leftMargin,rightMargin,topMargin,bottomMargin,kFALSE,kFALSE,kFALSE,
                                     hEtaPhiChMax,"","#phi","#frac{dN^{jet}}{d#phi}",1,1,
                                     processLabelOffsetX1,0.94,0.03,fCollisionSystem,plotDataSets[i],fTrigger[i]);
                WriteHistogram(hEtaPhiChMax);
                vecEtaPhiChMax[binEta].push_back(new TH1D(*hEtaPhiChMax));
            }
        } else cout << "INFO: Object |hEtaPhi| could not be found! Skipping Draw..." << endl;
        //-------------------------------------------------------------------------------------------------------------------------------
        // Eta-Phi Distribution of Max Neutral Constituents
        TH2D* hEtaPhiNeMax2D = (TH2D*)HistoList->FindObject("hQAEtaPhiMaxConstNe");
        if(hEtaPhiNeMax2D){
            for (Int_t binEta = 0; binEta < nEtaBins; binEta++){
                TH1D* hEtaPhiNeMax = (TH1D*)hEtaPhiNeMax2D->ProjectionY(Form("hEtaPhiNeMax_%1.1f_to_%1.1f", binsEta[binEta],binsEta[binEta+1]),binsEtaInt[binEta],binsEtaInt[binEta+1]);
                GetMinMaxBin(hEtaPhiNeMax,minB,maxB);
                SetXRange(hEtaPhiNeMax,minB,maxB);
                DrawPeriodQAHistoTH1(canvas,leftMargin,rightMargin,topMargin,bottomMargin,kFALSE,kFALSE,kFALSE,
                                     hEtaPhiNeMax,"","#phi","#frac{dN^{jet}}{d#phi}",1,1,
                                     processLabelOffsetX1,0.94,0.03,fCollisionSystem,plotDataSets[i],fTrigger[i]);
                WriteHistogram(hEtaPhiNeMax);
                vecEtaPhiNeMax[binEta].push_back(new TH1D(*hEtaPhiNeMax));
            }
        } else cout << "INFO: Object |hEtaPhiNeMax| could not be found! Skipping Draw..." << endl;
        //-------------------------------------------------------------------------------------------------------------------------------
        // Jet Area
        TH2D* hJetArea2D = (TH2D*)HistoList->FindObject("hQAJetAreaVsJetPt");
        if(hJetArea2D){
            for (Int_t binPt = 0; binPt < nPtBins; binPt++){
                TH1D* hJetArea = (TH1D*)hJetArea2D->ProjectionY(Form("hJetArea_%i-%iGeV", binsPt[binPt], binsPt[binPt+1]),binsPt[binPt],binsPt[binPt+1]);
                GetMinMaxBin(hJetArea,minB,maxB);
                SetXRange(hJetArea,minB,maxB);
                DrawPeriodQAHistoTH1(canvas,leftMargin,rightMargin,topMargin,bottomMargin,kFALSE,kFALSE,kFALSE,
                                     hJetArea,"","Area","#frac{dN^{jet}}{dArea}",1,1,
                                     processLabelOffsetX1,0.94,0.03,fCollisionSystem,plotDataSets[i],fTrigger[i]);
                WriteHistogram(hJetArea);
                vecJetArea[binPt].push_back(new TH1D(*hJetArea));
            }
        } else cout << "INFO: Object |hJetArea| could not be found! Skipping Draw..." << endl;
        //-------------------------------------------------------------------------------------------------------------------------------
        // Number of Constituents
        TH2D* hNConst2D = (TH2D*)HistoList->FindObject("hQAJetAreaVsNConst");
        if(hNConst2D){
            for (Int_t binArea = 0; binArea < nAreaBins; binArea++){
                TH1D* hNConst = (TH1D*)hNConst2D->ProjectionX(Form("hNConst_%1.2f_%1.2f", binsArea[binArea],binsArea[binArea+1]),binsAreaInt[binArea],binsAreaInt[binArea+1]);
                GetMinMaxBin(hNConst,minB,maxB);
                SetXRange(hNConst,minB,maxB);
                DrawPeriodQAHistoTH1(canvas,leftMargin,rightMargin,topMargin,bottomMargin,kFALSE,kFALSE,kFALSE,
                                     hNConst,"","N^{const}","#frac{dN^{jet}}{dN^const}",1,1,
                                     processLabelOffsetX1,0.94,0.03,fCollisionSystem,plotDataSets[i],fTrigger[i]);
                WriteHistogram(hNConst);
                vecNConst[binArea].push_back(new TH1D(*hNConst));
            }
        } else cout << "INFO: Object |hNConst| could not be found! Skipping Draw..." << endl;
        //-------------------------------------------------------------------------------------------------------------------------------
        // Cluster Time and Energy
        TH2D* hClusterTimeVsE2D = (TH2D*)HistoList->FindObject("hQAClusterTimeVsEFine");
        if(fHistVertexZ){
            TH1D* hClusterTime = (TH1D*)hClusterTimeVsE2D->ProjectionX("hClusterTime",0,200);
            GetMinMaxBin(hClusterTime,minB,maxB);
            SetXRange(hClusterTime,minB,maxB);
            DrawPeriodQAHistoTH1(canvas,leftMargin,rightMargin,topMargin,bottomMargin,kFALSE,kFALSE,kFALSE,
                                hClusterTime,"","t^{cluster} (ns)","#frac{dN}{dt^{cluster}}",1,1,
                                processLabelOffsetX1,0.94,0.03,fCollisionSystem,plotDataSets[i],fTrigger[i]);
            WriteHistogram(hClusterTime);
            vecClusterTime.push_back(new TH1D(*hClusterTime));

            TH1D* hClusterEnergy = (TH1D*)hClusterTimeVsE2D->ProjectionY("hClusterEnergy",-100,100);
            GetMinMaxBin(hClusterEnergy,minB,maxB);
            SetXRange(hClusterEnergy,minB,maxB);
            DrawPeriodQAHistoTH1(canvas,leftMargin,rightMargin,topMargin,bottomMargin,kFALSE,kFALSE,kFALSE,
                                hClusterEnergy,"","E^{cluster} (GeV)","#frac{dN}{dE^{cluster}}",1,1,
                                processLabelOffsetX1,0.94,0.03,fCollisionSystem,plotDataSets[i],fTrigger[i]);
            WriteHistogram(hClusterEnergy);
            vecClusterEnergy.push_back(new TH1D(*hClusterEnergy));
        } else cout << "INFO: Object |hQAClusterTimeVsEFine| could not be found! Skipping Draw..." << endl;
        //-------------------------------------------------------------------------------------------------------------------------------
        // Cluster Cells
        TH2D* hClusterCells2D = (TH2D*)HistoList->FindObject("hQAClusterNCellVsE");
        if(hClusterCells2D){
            for (Int_t binE = 0; binE < nEBins; binE++){
                TH1D* hClusterCells = (TH1D*)hClusterCells2D->ProjectionX(Form("hClusterCells_%i-%iGeV", binsE[binE],binsE[binE+1]),binsE[binE],binsE[binE+1]);
                GetMinMaxBin(hClusterCells,minB,maxB);
                SetXRange(hClusterCells,minB,maxB);
                DrawPeriodQAHistoTH1(canvas,leftMargin,rightMargin,topMargin,bottomMargin,kFALSE,kFALSE,kFALSE,
                                     hClusterCells,"","N^{cell}","#frac{dN^{jet}}{dN^{cell}}",1,1,
                                     processLabelOffsetX1,0.94,0.03,fCollisionSystem,plotDataSets[i],fTrigger[i]);
                WriteHistogram(hClusterCells);
                vecClusterCells[binE].push_back(new TH1D(*hClusterCells));
            }
        } else cout << "INFO: Object |hClusterCells| could not be found! Skipping Draw..." << endl;
        //-------------------------------------------------------------------------------------------------------------------------------
        // Cluster Cells
        TH2D* hM022D = (TH2D*)HistoList->FindObject("hQAClusterM02VsE");
        if(hM022D){
            for (Int_t binE = 0; binE < nEBins; binE++){
                TH1D* hM02 = (TH1D*)hM022D->ProjectionX(Form("hM02_%i-%iGeV", binsE[binE],binsE[binE+1]),binsE[binE],binsE[binE+1]);
                GetMinMaxBin(hM02,minB,maxB);
                SetXRange(hM02,minB,maxB);
                DrawPeriodQAHistoTH1(canvas,leftMargin,rightMargin,topMargin,bottomMargin,kFALSE,kFALSE,kFALSE,
                                     hM02,"","M02","#frac{dN^{jet}}{dM02}",1,1,
                                     processLabelOffsetX1,0.94,0.03,fCollisionSystem,plotDataSets[i],fTrigger[i]);
                WriteHistogram(hM02);
                vecM02[binE].push_back(new TH1D(*hM02));
            }
        } else cout << "INFO: Object |hM02| could not be found! Skipping Draw..." << endl;

        //-------------------------------------------------------------------------------------------------------------------------------

        Float_t nEventsAllEvt = fHistNEvents->GetBinContent(1);
        Float_t nEventsAllEvtErr = sqrt(nEventsAllEvt);

        cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << "nEventsAllEvt: \t\t\t(" << nEventsAllEvt << " +- " << nEventsAllEvtErr << ")" << endl;
        fLog << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
        fLog << "nEventsAllEvt: \t\t\t(" << nEventsAllEvt << " +- " << nEventsAllEvtErr << ")" << endl;

        //-------------------------------------------------------------------------------------------------------------------------------
        //----------------------------------- clean up ----------------------------------------------------------------------------------
        //-------------------------------------------------------------------------------------------------------------------------------
        delete TopDir;
        fFile->Close();
        delete fFile;
        fOutput->Close();
        delete fOutput;
    }

    // *****************************************************************************************************
    // *****************************************************************************************************
    // ****************************** Comparison Histograms ************************************************
    // *****************************************************************************************************
    // *****************************************************************************************************

    std::vector<TH1D*> temp;

    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << "Drawing Comparison Histograms" << endl;
    //-------------------------------------------------------------------------------------------------------------------------------
    //---------------------------------------- Event histogram comparisons ----------------------------------------------------------
    //-------------------------------------------------------------------------------------------------------------------------------
    // event statistics overview
    for(Int_t iVec=0; iVec<(Int_t)vecNEvents.size(); iVec++){
        TH1D* temp = vecNEvents.at(iVec);
        temp->Sumw2();
    }
    DrawPeriodQACompareHistoTH1(canvas,0.11, 0.02, 0.05, 0.11,kFALSE,kTRUE,kFALSE,
                                vecNEvents,"","","N_{Events}",1,1.1,
                                labelData, colorCompare, kFALSE, 1.1, 1.1, kTRUE,
                                0.95,0.92,0.03,fCollisionSystem,plotDataSets,fTrigger[0],31);
    drawLatexAdd(Form("%s Jets, R=0.%i", jetType.Data(), radius),0.95,0.84, 0.03,kFALSE, kFALSE, kTRUE);
    SaveCanvas(canvas, Form("%s/Comparison/NEvents_%s.%s", outputDir.Data(), trigName.Data(), suffix.Data()), kFALSE, kTRUE);

    DrawPeriodQACompareHistoRatioTH1(canvas,0.11, 0.02, 0.05, 0.11,kFALSE,kFALSE,kFALSE,
                                    vecNEvents,"","","N_{Events}",1,1.1,
                                    labelData, colorCompare, kTRUE, 1.1, 1.1, kTRUE,
                                    0.95,0.92,0.03,fCollisionSystem,plotDataSets,fTrigger[0],31);
    drawLatexAdd(Form("%s Jets, R=0.%i", jetType.Data(), radius),0.95,0.84, 0.03,kFALSE, kFALSE, kTRUE);
    SaveCanvas(canvas, Form("%s/Comparison/Ratios/ratio_NEvents_%s.%s", outputDir.Data(), trigName.Data(), suffix.Data()));
    //-------------------------------------------------------------------------------------------------------------------------------
    // z-vertex distribution
    GetMinMaxBin(vecVertexZ,minB,maxB);
    for(Int_t iVec=0; iVec<(Int_t)vecVertexZ.size(); iVec++){
        TH1D* temp = vecVertexZ.at(iVec);
        temp->Sumw2();
        temp->Scale(1./temp->Integral());
        //temp->Scale(1./nEvents[iVec]);
        SetXRange(temp,minB,maxB);
    }
    DrawPeriodQACompareHistoTH1(canvas,0.11, 0.02, 0.05, 0.11,kFALSE,kFALSE,kFALSE,
                        vecVertexZ,"","Vertex z (cm)","#frac{1}{N} #frac{dN^{jet}}{dz}",1,1.1,
                        labelData, colorCompare, kTRUE, 1.1, 1.1, kTRUE,
                        0.95,0.92,0.03,fCollisionSystem,plotDataSets,fTrigger[0],31);
    drawLatexAdd(Form("%s Jets, R=0.%i", jetType.Data(), radius),0.95,0.84, 0.03,kFALSE, kFALSE, kTRUE);
    SaveCanvas(canvas, Form("%s/Comparison/Vertex_Z_%s.%s", outputDir.Data(), trigName.Data(), suffix.Data()));

    DrawPeriodQACompareHistoRatioTH1(canvas,0.11, 0.02, 0.05, 0.11,kFALSE,kFALSE,kFALSE,
                        vecVertexZ,"","Vertex z (cm)","#frac{1}{N} #frac{dN^{jet}}{dz}",1,1.1,
                        labelData, colorCompare, kTRUE, 1.1, 1.1, kTRUE,
                        0.95,0.92,0.03,fCollisionSystem,plotDataSets,fTrigger[0],31);
    drawLatexAdd(Form("%s Jets, R=0.%i", jetType.Data(), radius),0.95,0.84, 0.03,kFALSE, kFALSE, kTRUE);
    SaveCanvas(canvas, Form("%s/Comparison/Ratios/ratio_Vertex_Z_%s.%s", outputDir.Data(), trigName.Data(), suffix.Data()));

    //-------------------------------------------------------------------------------------------------------------------------------
    // centrality
/*    if(fIsPbPb){
      GetMinMaxBin(vecCentrality,minB,maxB);
      for(Int_t iVec=0; iVec<(Int_t)vecCentrality.size(); iVec++){
        TH1D* temp = vecCentrality.at(iVec);
        temp->Sumw2();
        temp->Scale(1./temp->Integral());
        SetXRange(temp,minB,maxB);
      }
      DrawPeriodQACompareHistoTH1(  canvas,0.11, 0.02, 0.05, 0.11,kFALSE,kFALSE,kFALSE,
                                    vecCentrality,"","Centrality (%)","",1,1.1,
                                    labelData, colorCompare, kTRUE, 1.1, 1.1, kTRUE,
                                    0.95,0.24,0.03,fCollisionSystem,plotDataSets,fTrigger[0],31);
      SaveCanvas(canvas, Form("%s/Comparison/Centrality.%s", outputDir.Data(), suffix.Data()));

      DrawPeriodQACompareHistoRatioTH1( canvas,0.11, 0.02, 0.05, 0.11,kFALSE,kFALSE,kFALSE,
                                        vecCentrality,"","Centrality (%)","",1,1.1,
                                        labelData, colorCompare, kTRUE, 1.1, 1.1, kTRUE,
                                        0.95,0.24,0.03,fCollisionSystem,plotDataSets,fTrigger[0],31);
      SaveCanvas(canvas, Form("%s/Comparison/Ratios/ratio_Centrality.%s", outputDir.Data(), suffix.Data()));
      //Event Plane Angle
      GetMinMaxBin(vecEventPlaneAngle,minB,maxB);
      for(Int_t iVec=0; iVec<(Int_t)vecEventPlaneAngle.size(); iVec++){
        TH1D* temp = vecEventPlaneAngle.at(iVec);
        temp->Sumw2();
        temp->Scale(1./temp->Integral());
        SetXRange(temp,minB,maxB);
      }
      DrawPeriodQACompareHistoTH1(  canvas,0.11, 0.02, 0.05, 0.11,kFALSE,kFALSE,kFALSE,
                                    vecEventPlaneAngle,"","Event Plane Angle (rad)","",1,1.1,
                                    labelData, colorCompare, kTRUE, 1.1, 1.1, kTRUE,
                                    0.95,0.92,0.03,fCollisionSystem,plotDataSets,fTrigger[0],31);
      SaveCanvas(canvas, Form("%s/Comparison/EventPlaneAngle.%s", outputDir.Data(), suffix.Data()));

      DrawPeriodQACompareHistoRatioTH1( canvas,0.11, 0.02, 0.05, 0.11,kFALSE,kFALSE,kFALSE,
                                        vecEventPlaneAngle,"","Event Plane Angle (rad)","",1,1.1,
                                        labelData, colorCompare, kTRUE, 1.1, 1.1, kTRUE,
                                        0.95,0.92,0.03,fCollisionSystem,plotDataSets,fTrigger[0],31);
      SaveCanvas(canvas, Form("%s/Comparison/Ratios/ratio_EventPlaneAngle.%s", outputDir.Data(), suffix.Data()));
    }

    if (hasCentHist){
        for(Int_t iVec=0; iVec<(Int_t)vecCentrality2.size(); iVec++){
            TH1D* temp = vecCentrality2.at(iVec);
            temp->Sumw2();
            temp->Scale(1./temp->Integral());
            SetXRange(temp,0,100);
        }
        DrawPeriodQACompareHistoTH1(  canvas,0.11, 0.02, 0.05, 0.11,kFALSE,kFALSE,kFALSE,
                                      vecCentrality2,"","Centrality (%)","",1,1.1,
                                      labelData, colorCompare, kTRUE, 1.1, 1.1, kTRUE,
                                      0.95,0.24,0.03,fCollisionSystem,plotDataSets,fTrigger[0],31);
        SaveCanvas(canvas, Form("%s/Comparison/CentralityEventCuts.%s", outputDir.Data(), suffix.Data()));

        DrawPeriodQACompareHistoRatioTH1( canvas,0.11, 0.02, 0.05, 0.11,kFALSE,kFALSE,kFALSE,
                                          vecCentrality2,"","Centrality (%)","",1,1.1,
                                          labelData, colorCompare, kTRUE, 1.1, 1.1, kTRUE,
                                          0.95,0.24,0.03,fCollisionSystem,plotDataSets,fTrigger[0],31);
        SaveCanvas(canvas, Form("%s/Comparison/Ratios/ratio_CentralityEventCuts.%s", outputDir.Data(), suffix.Data()));
    }
*/
    //-------------------------------------------------------------------------------------------------------------------------------
    // Jet Spectrum
    GetMinMaxBin(vecJetPt,minB,maxB);
    for(Int_t iVec=0; iVec<(Int_t)vecJetPt.size(); iVec++){
        TH1D* temp = vecJetPt.at(iVec);
        temp->Sumw2();
        temp->Scale(1./temp->Integral());
        //temp->Scale(1./nEvents[iVec]);
        SetXRange(temp,minB,maxB);
    }

    DrawPeriodQACompareHistoTH1(canvas,0.11, 0.02, 0.05, 0.11,kFALSE,kFALSE,kFALSE,
                        vecJetPt,"","p_{T}^{jet}","#frac{1}{N^{Evt}} #frac{dN^{jet}}{dp_{T}^{jet}}",1,1.1,
                        labelData, colorCompare, kTRUE, 1.1, 1.1, kTRUE,
                        0.95,0.92,0.03,fCollisionSystem,plotDataSets,fTrigger[0],31);
    drawLatexAdd(Form("%s Jets, R=0.%i", jetType.Data(), radius),0.95,0.84, 0.03,kFALSE, kFALSE, kTRUE);
    SaveCanvas(canvas, Form("%s/Comparison/JetPt_%s.%s", outputDir.Data(), trigName.Data(), suffix.Data()), kFALSE, kTRUE);

    DrawPeriodQACompareHistoRatioTH1(canvas,0.11, 0.02, 0.05, 0.11,kFALSE,kFALSE,kFALSE,
                        vecJetPt,"","p_{T}^{jet}","#frac{1}{N^Evt} #frac{dN^{jet}}{dp_{T}^{jet}}",1,1.1,
                        labelData, colorCompare, kTRUE, 1.1, 1.1, kTRUE,
                        0.95,0.92,0.03,fCollisionSystem,plotDataSets,fTrigger[0],31);
    drawLatexAdd(Form("%s Jets, R=0.%i", jetType.Data(), radius),0.95,0.84, 0.03,kFALSE, kFALSE, kTRUE);
    SaveCanvas(canvas, Form("%s/Comparison/Ratios/ratio_JetPt_%s.%s", outputDir.Data(), trigName.Data(), suffix.Data()));

    //-------------------------------------------------------------------------------------------------------------------------------
    // Jet Spectrum Max
    GetMinMaxBin(vecJetPtMax,minB,maxB);
    for(Int_t iVec=0; iVec<(Int_t)vecJetPtMax.size(); iVec++){
        TH1D* temp = vecJetPtMax.at(iVec);
        temp->Sumw2();
        temp->Scale(1./temp->Integral());
        //temp->Scale(1./nEvents[iVec]);
        SetXRange(temp,minB,maxB);
    }
    DrawPeriodQACompareHistoTH1(canvas,0.11, 0.02, 0.05, 0.11,kFALSE,kFALSE,kFALSE,
                        vecJetPtMax,"","p_{T}^{jet,max}","#frac{1}{N^{Evt}} #frac{dN^{jet}}{dp_{T}^{jet,max}}",1,1.1,
                        labelData, colorCompare, kTRUE, 1.1, 1.1, kTRUE,
                        0.95,0.92,0.03,fCollisionSystem,plotDataSets,fTrigger[0],31);
    drawLatexAdd(Form("%s Jets, R=0.%i", jetType.Data(), radius),0.95,0.84, 0.03,kFALSE, kFALSE, kTRUE);
    SaveCanvas(canvas, Form("%s/Comparison/JetPtMax_%s.%s", outputDir.Data(), trigName.Data(), suffix.Data()), false, true);

    DrawPeriodQACompareHistoRatioTH1(canvas,0.11, 0.02, 0.05, 0.11,kFALSE,kFALSE,kFALSE,
                        vecJetPtMax,"","p_{T}^{jet,max}","#frac{1}{N^Evt} #frac{dN^{jet}}{dp_{T}^{jet,max}}",1,1.1,
                        labelData, colorCompare, kTRUE, 1.1, 1.1, kTRUE,
                        0.95,0.92,0.03,fCollisionSystem,plotDataSets,fTrigger[0],31);
    drawLatexAdd(Form("%s Jets, R=0.%i", jetType.Data(), radius),0.95,0.84, 0.03,kFALSE, kFALSE, kTRUE);
    SaveCanvas(canvas, Form("%s/Comparison/Ratios/ratio_JetPtMax_%s.%s", outputDir.Data(), trigName.Data(), suffix.Data()));

    //-------------------------------------------------------------------------------------------------------------------------------
    // Neutral Energy Fraction
    for (Int_t binPt = 0; binPt < nPtBins; binPt++){
        GetMinMaxBin(vecNEF[binPt],minB,maxB);
        for(Int_t iVec=0; iVec<(Int_t)vecNEF[binPt].size(); iVec++){
            TH1D* temp = vecNEF[binPt].at(iVec);
            temp->Sumw2();
            temp->Scale(1./temp->Integral());
            //temp->Scale(1./nEvents[iVec]);
            SetXRange(temp,minB,maxB);
        }

        DrawPeriodQACompareHistoTH1(canvas,0.11, 0.02, 0.05, 0.11,kFALSE,kFALSE,kFALSE,
                            vecNEF[binPt],"","NEF","#frac{1}{N^{Evt}} #frac{dN^{jet}}{dNEF}",1,1.1,
                            labelData, colorCompare, kTRUE, 1.1, 1.1, kTRUE,
                            0.95,0.92,0.03,fCollisionSystem,plotDataSets,fTrigger[0],31);
        drawLatexAdd(Form("p_{T}^{jet} = %i - %i GeV", binsPt[binPt], binsPt[binPt+1]),0.95,0.8, 0.03,kFALSE, kFALSE, kTRUE);
        drawLatexAdd(Form("%s Jets, R=0.%i", jetType.Data(), radius),0.95,0.84, 0.03,kFALSE, kFALSE, kTRUE);
        SaveCanvas(canvas, Form("%s/Comparison/NEF_%s_%i-%iGeV.%s", outputDir.Data(), trigName.Data(), binsPt[binPt], binsPt[binPt+1], suffix.Data()), false, false);

        DrawPeriodQACompareHistoRatioTH1(canvas,0.11, 0.02, 0.05, 0.11,kFALSE,kFALSE,kFALSE,
                            vecNEF[binPt],"","NEF","#frac{1}{N^Evt} #frac{dN^{jet}}{dNEF}",1,1.1,
                            labelData, colorCompare, kTRUE, 1.1, 1.1, kTRUE,
                            0.95,0.92,0.03,fCollisionSystem,plotDataSets,fTrigger[0],31);
        drawLatexAdd(Form("p_{T}^{jet} = %i - %i GeV", binsPt[binPt], binsPt[binPt+1]),0.95,0.8, 0.03,kFALSE, kFALSE, kTRUE);
        drawLatexAdd(Form("%s Jets, R=0.%i", jetType.Data(), radius),0.95,0.84, 0.03,kFALSE, kFALSE, kTRUE);
        SaveCanvas(canvas, Form("%s/Comparison/Ratios/ratio_NEF_%s_%i-%iGeV.%s", outputDir.Data(), trigName.Data(), binsPt[binPt], binsPt[binPt+1], suffix.Data()));
    }

    //-------------------------------------------------------------------------------------------------------------------------------
    // z Charged
    for (Int_t binPt = 0; binPt < nPtBins; binPt++){
        GetMinMaxBin(vecZch[binPt],minB,maxB);
        for(Int_t iVec=0; iVec<(Int_t)vecZch[binPt].size(); iVec++){
            TH1D* temp = vecZch[binPt].at(iVec);
            temp->Sumw2();
            temp->Scale(1./temp->Integral());
            //temp->Scale(1./nEvents[iVec]);
            SetXRange(temp,minB,maxB);
        }

        DrawPeriodQACompareHistoTH1(canvas,0.11, 0.02, 0.05, 0.11,kFALSE,kFALSE,kFALSE,
                            vecZch[binPt],"","z_{ch}","#frac{1}{N^{Evt}} #frac{dN^{jet}}{dz_{ch}}",1,1.1,
                            labelData, colorCompare, kTRUE, 1.1, 1.1, kTRUE,
                            0.95,0.92,0.03,fCollisionSystem,plotDataSets,fTrigger[0],31);
        drawLatexAdd(Form("p_{T}^{jet} = %i - %i GeV", binsPt[binPt], binsPt[binPt+1]),0.95,0.8, 0.03,kFALSE, kFALSE, kTRUE);
        drawLatexAdd(Form("%s Jets, R=0.%i", jetType.Data(), radius),0.95,0.84, 0.03,kFALSE, kFALSE, kTRUE);
        SaveCanvas(canvas, Form("%s/Comparison/Zch_%s_%i-%iGeV.%s", outputDir.Data(), trigName.Data(), binsPt[binPt], binsPt[binPt+1], suffix.Data()), false, true);

        DrawPeriodQACompareHistoRatioTH1(canvas,0.11, 0.02, 0.05, 0.11,kFALSE,kFALSE,kFALSE,
                            vecZch[binPt],"","z_{ch}","#frac{1}{N^Evt} #frac{dN^{jet}}{dz_{ch}}",1,1.1,
                            labelData, colorCompare, kTRUE, 1.1, 1.1, kTRUE,
                            0.95,0.92,0.03,fCollisionSystem,plotDataSets,fTrigger[0],31);
        drawLatexAdd(Form("p_{T}^{jet} = %i - %i GeV", binsPt[binPt], binsPt[binPt+1]),0.95,0.8, 0.03,kFALSE, kFALSE, kTRUE);
        drawLatexAdd(Form("%s Jets, R=0.%i", jetType.Data(), radius),0.95,0.84, 0.03,kFALSE, kFALSE, kTRUE);
        SaveCanvas(canvas, Form("%s/Comparison/Ratios/ratio_Zch_%s_%i-%iGeV.%s", outputDir.Data(), trigName.Data(), binsPt[binPt], binsPt[binPt+1], suffix.Data()));
    }

    //-------------------------------------------------------------------------------------------------------------------------------
    // z Neutral
    for (Int_t binPt = 0; binPt < nPtBins; binPt++){
        GetMinMaxBin(vecZne[binPt],minB,maxB);
        for(Int_t iVec=0; iVec<(Int_t)vecZne[binPt].size(); iVec++){
            TH1D* temp = vecZne[binPt].at(iVec);
            temp->Sumw2();
            temp->Scale(1./temp->Integral());
            //temp->Scale(1./nEvents[iVec]);
            SetXRange(temp,minB,maxB);
        }

        DrawPeriodQACompareHistoTH1(canvas,0.11, 0.02, 0.05, 0.11,kFALSE,kFALSE,kFALSE,
                            vecZne[binPt],"","z_{ne}","#frac{1}{N^{Evt}} #frac{dN^{jet}}{dz_{ne}}",1,1.1,
                            labelData, colorCompare, kTRUE, 1.1, 1.1, kTRUE,
                            0.95,0.92,0.03,fCollisionSystem,plotDataSets,fTrigger[0],31);
        drawLatexAdd(Form("p_{T}^{jet} = %i - %i GeV", binsPt[binPt], binsPt[binPt+1]),0.95,0.8, 0.03,kFALSE, kFALSE, kTRUE);
        drawLatexAdd(Form("%s Jets, R=0.%i", jetType.Data(), radius),0.95,0.84, 0.03,kFALSE, kFALSE, kTRUE);
        SaveCanvas(canvas, Form("%s/Comparison/Zne_%s_%i-%iGeV.%s", outputDir.Data(), trigName.Data(), binsPt[binPt], binsPt[binPt+1], suffix.Data()), false, true);

        DrawPeriodQACompareHistoRatioTH1(canvas,0.11, 0.02, 0.05, 0.11,kFALSE,kFALSE,kFALSE,
                            vecZne[binPt],"","z_{ne}","#frac{1}{N^Evt} #frac{dN^{jet}}{dz_{ne}}",1,1.1,
                            labelData, colorCompare, kTRUE, 1.1, 1.1, kTRUE,
                            0.95,0.92,0.03,fCollisionSystem,plotDataSets,fTrigger[0],31);
        drawLatexAdd(Form("p_{T}^{jet} = %i - %i GeV", binsPt[binPt], binsPt[binPt+1]),0.95,0.8, 0.03,kFALSE, kFALSE, kTRUE);
        drawLatexAdd(Form("%s Jets, R=0.%i", jetType.Data(), radius),0.95,0.84, 0.03,kFALSE, kFALSE, kTRUE);
        SaveCanvas(canvas, Form("%s/Comparison/Ratios/ratio_Zne_%s_%i-%iGeV.%s", outputDir.Data(), trigName.Data(), binsPt[binPt], binsPt[binPt+1], suffix.Data()));
    }

    //-------------------------------------------------------------------------------------------------------------------------------
    // z Charged Max
    for (Int_t binPt = 0; binPt < nPtBins; binPt++){
        GetMinMaxBin(vecZchMax[binPt],minB,maxB);
        for(Int_t iVec=0; iVec<(Int_t)vecZchMax[binPt].size(); iVec++){
            TH1D* temp = vecZchMax[binPt].at(iVec);
            temp->Sumw2();
            temp->Scale(1./temp->Integral());
            //temp->Scale(1./nEvents[iVec]);
            SetXRange(temp,minB,maxB);
        }

        DrawPeriodQACompareHistoTH1(canvas,0.11, 0.02, 0.05, 0.11,kFALSE,kFALSE,kFALSE,
                            vecZchMax[binPt],"","z_{ch}^{max}","#frac{1}{N^{Evt}} #frac{dN^{jet}}{dz_{ch}^{max}}",1,1.1,
                            labelData, colorCompare, kTRUE, 1.1, 1.1, kTRUE,
                            0.95,0.92,0.03,fCollisionSystem,plotDataSets,fTrigger[0],31);
        drawLatexAdd(Form("p_{T}^{jet} = %i - %i GeV", binsPt[binPt], binsPt[binPt+1]),0.95,0.8, 0.03,kFALSE, kFALSE, kTRUE);
        drawLatexAdd(Form("%s Jets, R=0.%i", jetType.Data(), radius),0.95,0.84, 0.03,kFALSE, kFALSE, kTRUE);
        SaveCanvas(canvas, Form("%s/Comparison/ZchMax_%s_%i-%iGeV.%s", outputDir.Data(), trigName.Data(), binsPt[binPt], binsPt[binPt+1], suffix.Data()), false, true);

        DrawPeriodQACompareHistoRatioTH1(canvas,0.11, 0.02, 0.05, 0.11,kFALSE,kFALSE,kFALSE,
                            vecZchMax[binPt],"","z_{ch}^{max}","#frac{1}{N^Evt} #frac{dN^{jet}}{dz_{ch}^{max}}",1,1.1,
                            labelData, colorCompare, kTRUE, 1.1, 1.1, kTRUE,
                            0.95,0.92,0.03,fCollisionSystem,plotDataSets,fTrigger[0],31);
        drawLatexAdd(Form("p_{T}^{jet} = %i - %i GeV", binsPt[binPt], binsPt[binPt+1]),0.95,0.8, 0.03,kFALSE, kFALSE, kTRUE);
        drawLatexAdd(Form("%s Jets, R=0.%i", jetType.Data(), radius),0.95,0.84, 0.03,kFALSE, kFALSE, kTRUE);
        SaveCanvas(canvas, Form("%s/Comparison/Ratios/ratio_ZchMax_%s_%i-%iGeV.%s", outputDir.Data(), trigName.Data(), binsPt[binPt], binsPt[binPt+1], suffix.Data()));
    }

    //-------------------------------------------------------------------------------------------------------------------------------
    // z Neutral Max
    for (Int_t binPt = 0; binPt < nPtBins; binPt++){
        GetMinMaxBin(vecZneMax[binPt],minB,maxB);
        for(Int_t iVec=0; iVec<(Int_t)vecZneMax[binPt].size(); iVec++){
            TH1D* temp = vecZneMax[binPt].at(iVec);
            temp->Sumw2();
            temp->Scale(1./temp->Integral());
            //temp->Scale(1./nEvents[iVec]);
            SetXRange(temp,minB,maxB);
        }

        DrawPeriodQACompareHistoTH1(canvas,0.11, 0.02, 0.05, 0.11,kFALSE,kFALSE,kFALSE,
                            vecZneMax[binPt],"","z_{ne}^{max}","#frac{1}{N^{Evt}} #frac{dN^{jet}}{dz_{ne}^{max}}",1,1.1,
                            labelData, colorCompare, kTRUE, 1.1, 1.1, kTRUE,
                            0.95,0.92,0.03,fCollisionSystem,plotDataSets,fTrigger[0],31);
        //
        drawLatexAdd(Form("p_{T}^{jet} = %i - %i GeV", binsPt[binPt], binsPt[binPt+1]),0.95,0.8, 0.03,kFALSE, kFALSE, kTRUE);
        drawLatexAdd(Form("%s Jets, R=0.%i", jetType.Data(), radius),0.95,0.84, 0.03,kFALSE, kFALSE, kTRUE);
        SaveCanvas(canvas, Form("%s/Comparison/ZneMax_%s_%i-%iGeV.%s", outputDir.Data(), trigName.Data(), binsPt[binPt], binsPt[binPt+1], suffix.Data()), false, true);

        DrawPeriodQACompareHistoRatioTH1(canvas,0.11, 0.02, 0.05, 0.11,kFALSE,kFALSE,kFALSE,
                            vecZneMax[binPt],"","z_{ne}^{max}","#frac{1}{N^Evt} #frac{dN^{jet}}{dz_{ne}^{max}}",1,1.1,
                            labelData, colorCompare, kTRUE, 1.1, 1.1, kTRUE,
                            0.95,0.92,0.03,fCollisionSystem,plotDataSets,fTrigger[0],31);
        drawLatexAdd(Form("p_{T}^{jet} = %i - %i GeV", binsPt[binPt], binsPt[binPt+1]),0.95,0.8, 0.03,kFALSE, kFALSE, kTRUE);
        drawLatexAdd(Form("%s Jets, R=0.%i", jetType.Data(), radius),0.95,0.84, 0.03,kFALSE, kFALSE, kTRUE);
        SaveCanvas(canvas, Form("%s/Comparison/Ratios/ratio_ZneMax_%s_%i-%iGeV.%s", outputDir.Data(), trigName.Data(), binsPt[binPt], binsPt[binPt+1], suffix.Data()));
    }

    //-------------------------------------------------------------------------------------------------------------------------------
    // Number of Charged Constituents
    for (Int_t binPt = 0; binPt < nPtBins; binPt++){
        GetMinMaxBin(vecNch[binPt],minB,maxB);
        for(Int_t iVec=0; iVec<(Int_t)vecNch[binPt].size(); iVec++){
            TH1D* temp = vecNch[binPt].at(iVec);
            temp->Sumw2();
            temp->Scale(1./temp->Integral());
            //temp->Scale(1./nEvents[iVec]);
            SetXRange(temp,minB,maxB);
        }

        DrawPeriodQACompareHistoTH1(canvas,0.11, 0.02, 0.05, 0.11,kFALSE,kFALSE,kFALSE,
                            vecNch[binPt],"","N_{ch}^{const}","#frac{1}{N^{Evt}} #frac{dN^{jet}}{dN_{ch}^{const}}",1,1.1,
                            labelData, colorCompare, kTRUE, 1.1, 1.1, kTRUE,
                            0.95,0.92,0.03,fCollisionSystem,plotDataSets,fTrigger[0],31);
        //
        drawLatexAdd(Form("p_{T}^{jet} = %i - %i GeV", binsPt[binPt], binsPt[binPt+1]),0.95,0.8, 0.03,kFALSE, kFALSE, kTRUE);
        drawLatexAdd(Form("%s Jets, R=0.%i", jetType.Data(), radius),0.95,0.84, 0.03,kFALSE, kFALSE, kTRUE);
        SaveCanvas(canvas, Form("%s/Comparison/NCh_%s_%i-%iGeV.%s", outputDir.Data(), trigName.Data(), binsPt[binPt], binsPt[binPt+1], suffix.Data()), false, true);

        DrawPeriodQACompareHistoRatioTH1(canvas,0.11, 0.02, 0.05, 0.11,kFALSE,kFALSE,kFALSE,
                            vecNch[binPt],"","N_{ch}^{const}","#frac{1}{N^Evt} #frac{dN^{jet}}{dN_{ch}^{const}}",1,1.1,
                            labelData, colorCompare, kTRUE, 1.1, 1.1, kTRUE,
                            0.95,0.92,0.03,fCollisionSystem,plotDataSets,fTrigger[0],31);
        drawLatexAdd(Form("p_{T}^{jet} = %i - %i GeV", binsPt[binPt], binsPt[binPt+1]),0.95,0.8, 0.03,kFALSE, kFALSE, kTRUE);
        drawLatexAdd(Form("%s Jets, R=0.%i", jetType.Data(), radius),0.95,0.84, 0.03,kFALSE, kFALSE, kTRUE);
        SaveCanvas(canvas, Form("%s/Comparison/Ratios/ratio_NCh_%s_%i-%iGeV.%s", outputDir.Data(), trigName.Data(), binsPt[binPt], binsPt[binPt+1], suffix.Data()));
    }

    //-------------------------------------------------------------------------------------------------------------------------------
    // Number of Neutral Constituents
    for (Int_t binPt = 0; binPt < nPtBins; binPt++){
        GetMinMaxBin(vecNne[binPt],minB,maxB);
        for(Int_t iVec=0; iVec<(Int_t)vecNne[binPt].size(); iVec++){
            TH1D* temp = vecNne[binPt].at(iVec);
            temp->Sumw2();
            temp->Scale(1./temp->Integral());
            //temp->Scale(1./nEvents[iVec]);
            SetXRange(temp,minB,maxB);
        }

        DrawPeriodQACompareHistoTH1(canvas,0.11, 0.02, 0.05, 0.11,kFALSE,kFALSE,kFALSE,
                            vecNne[binPt],"","N_{ne}^{const}","#frac{1}{N^{Evt}} #frac{dN^{jet}}{dN_{ne}^{const}}",1,1.1,
                            labelData, colorCompare, kTRUE, 1.1, 1.1, kTRUE,
                            0.95,0.92,0.03,fCollisionSystem,plotDataSets,fTrigger[0],31);
        drawLatexAdd(Form("p_{T}^{jet} = %i - %i GeV", binsPt[binPt], binsPt[binPt+1]),0.95,0.8, 0.03,kFALSE, kFALSE, kTRUE);
        drawLatexAdd(Form("%s Jets, R=0.%i", jetType.Data(), radius),0.95,0.84, 0.03,kFALSE, kFALSE, kTRUE);
        SaveCanvas(canvas, Form("%s/Comparison/Nne_%s_%i-%iGeV.%s", outputDir.Data(), trigName.Data(), binsPt[binPt], binsPt[binPt+1], suffix.Data()), false, true);

        DrawPeriodQACompareHistoRatioTH1(canvas,0.11, 0.02, 0.05, 0.11,kFALSE,kFALSE,kFALSE,
                            vecNne[binPt],"","N_{ne}^{const}","#frac{1}{N^Evt} #frac{dN^{jet}}{dN_{ne}^{const}}",1,1.1,
                            labelData, colorCompare, kTRUE, 1.1, 1.1, kTRUE,
                            0.95,0.92,0.03,fCollisionSystem,plotDataSets,fTrigger[0],31);
        drawLatexAdd(Form("p_{T}^{jet} = %i - %i GeV", binsPt[binPt], binsPt[binPt+1]),0.95,0.8, 0.03,kFALSE, kFALSE, kTRUE);
        drawLatexAdd(Form("%s Jets, R=0.%i", jetType.Data(), radius),0.95,0.84, 0.03,kFALSE, kFALSE, kTRUE);
        SaveCanvas(canvas, Form("%s/Comparison/Ratios/ratio_Nne_%s_%i-%iGeV.%s", outputDir.Data(), trigName.Data(), binsPt[binPt], binsPt[binPt+1], suffix.Data()));
    }

    //-------------------------------------------------------------------------------------------------------------------------------
    // Pt of Charged Constituents
    for (Int_t binPt = 0; binPt < nPtBins; binPt++){
        GetMinMaxBin(vecPtCh[binPt],minB,maxB);
        for(Int_t iVec=0; iVec<(Int_t)vecPtCh[binPt].size(); iVec++){
            TH1D* temp = vecPtCh[binPt].at(iVec);
            temp->Sumw2();
            temp->Scale(1./temp->Integral());
            //temp->Scale(1./nEvents[iVec]);
            SetXRange(temp,minB,maxB);
        }

        DrawPeriodQACompareHistoTH1(canvas,0.11, 0.02, 0.05, 0.11,kFALSE,kFALSE,kFALSE,
                            vecPtCh[binPt],"","p_{T,ch}^{const}","#frac{1}{N^{Evt}} #frac{dN^{jet}}{dp_{T,ch}^{const}}",1,1.1,
                            labelData, colorCompare, kTRUE, 1.1, 1.1, kTRUE,
                            0.95,0.92,0.03,fCollisionSystem,plotDataSets,fTrigger[0],31);
        drawLatexAdd(Form("p_{T}^{jet} = %i - %i GeV", binsPt[binPt], binsPt[binPt+1]),0.95,0.8, 0.03,kFALSE, kFALSE, kTRUE);
        drawLatexAdd(Form("%s Jets, R=0.%i", jetType.Data(), radius),0.95,0.84, 0.03,kFALSE, kFALSE, kTRUE);
        SaveCanvas(canvas, Form("%s/Comparison/PtCh_%s_%i-%iGeV.%s", outputDir.Data(), trigName.Data(), binsPt[binPt], binsPt[binPt+1], suffix.Data()), false, true);

        DrawPeriodQACompareHistoRatioTH1(canvas,0.11, 0.02, 0.05, 0.11,kFALSE,kFALSE,kFALSE,
                            vecPtCh[binPt],"","p_{T,ch}^{const}","#frac{1}{N^Evt} #frac{dN^{jet}}{dp_{T,ch}^{const}}",1,1.1,
                            labelData, colorCompare, kTRUE, 1.1, 1.1, kTRUE,
                            0.95,0.92,0.03,fCollisionSystem,plotDataSets,fTrigger[0],31);
        drawLatexAdd(Form("p_{T}^{jet} = %i - %i GeV", binsPt[binPt], binsPt[binPt+1]),0.95,0.8, 0.03,kFALSE, kFALSE, kTRUE);
        drawLatexAdd(Form("%s Jets, R=0.%i", jetType.Data(), radius),0.95,0.84, 0.03,kFALSE, kFALSE, kTRUE);
        SaveCanvas(canvas, Form("%s/Comparison/Ratios/ratio_PtCh_%s_%i-%iGeV.%s", outputDir.Data(), trigName.Data(), binsPt[binPt], binsPt[binPt+1], suffix.Data()));
    }

    //-------------------------------------------------------------------------------------------------------------------------------
    // Pt of Neutral Constituents
    for (Int_t binPt = 0; binPt < nPtBins; binPt++){
        GetMinMaxBin(vecPtNe[binPt],minB,maxB);
        for(Int_t iVec=0; iVec<(Int_t)vecPtNe[binPt].size(); iVec++){
            TH1D* temp = vecPtNe[binPt].at(iVec);
            temp->Sumw2();
            temp->Scale(1./temp->Integral());
            //temp->Scale(1./nEvents[iVec]);
            SetXRange(temp,minB,maxB);
        }

        DrawPeriodQACompareHistoTH1(canvas,0.11, 0.02, 0.05, 0.11,kFALSE,kFALSE,kFALSE,
                            vecPtNe[binPt],"","p_{T,ne}^{const}","#frac{1}{N^{Evt}} #frac{dN^{jet}}{dp_{T,ne}^{const}}",1,1.1,
                            labelData, colorCompare, kTRUE, 1.1, 1.1, kTRUE,
                            0.95,0.92,0.03,fCollisionSystem,plotDataSets,fTrigger[0],31);
        drawLatexAdd(Form("p_{T}^{jet} = %i - %i GeV", binsPt[binPt], binsPt[binPt+1]),0.95,0.8, 0.03,kFALSE, kFALSE, kTRUE);
        drawLatexAdd(Form("%s Jets, R=0.%i", jetType.Data(), radius),0.95,0.84, 0.03,kFALSE, kFALSE, kTRUE);
        SaveCanvas(canvas, Form("%s/Comparison/PtNe_%s_%i-%iGeV.%s", outputDir.Data(), trigName.Data(), binsPt[binPt], binsPt[binPt+1], suffix.Data()), false, true);

        DrawPeriodQACompareHistoRatioTH1(canvas,0.11, 0.02, 0.05, 0.11,kFALSE,kFALSE,kFALSE,
                            vecPtNe[binPt],"","p_{T,ne}^{const}","#frac{1}{N^Evt} #frac{dN^{jet}}{dp_{T,ne}^{const}}",1,1.1,
                            labelData, colorCompare, kTRUE, 1.1, 1.1, kTRUE,
                            0.95,0.92,0.03,fCollisionSystem,plotDataSets,fTrigger[0],31);
        drawLatexAdd(Form("p_{T}^{jet} = %i - %i GeV", binsPt[binPt], binsPt[binPt+1]),0.95,0.8, 0.03,kFALSE, kFALSE, kTRUE);
        drawLatexAdd(Form("%s Jets, R=0.%i", jetType.Data(), radius),0.95,0.84, 0.03,kFALSE, kFALSE, kTRUE);
        SaveCanvas(canvas, Form("%s/Comparison/Ratios/ratio_PtNe_%s_%i-%iGeV.%s", outputDir.Data(), trigName.Data(), binsPt[binPt], binsPt[binPt+1], suffix.Data()));
    }

    //-------------------------------------------------------------------------------------------------------------------------------
    // Pt of Max Charged Constituents
    for (Int_t binPt = 0; binPt < nPtBins; binPt++){
        GetMinMaxBin(vecPtChMax[binPt],minB,maxB);
        for(Int_t iVec=0; iVec<(Int_t)vecPtChMax[binPt].size(); iVec++){
            TH1D* temp = vecPtChMax[binPt].at(iVec);
            temp->Sumw2();
            temp->Scale(1./temp->Integral());
            //temp->Scale(1./nEvents[iVec]);
            SetXRange(temp,minB,maxB);
        }

        DrawPeriodQACompareHistoTH1(canvas,0.11, 0.02, 0.05, 0.11,kFALSE,kFALSE,kFALSE,
                            vecPtChMax[binPt],"","p_{T,ch}^{const,max}","#frac{1}{N^{Evt}} #frac{dN^{jet}}{dp_{T,ch}^{const,max}}",1,1.1,
                            labelData, colorCompare, kTRUE, 1.1, 1.1, kTRUE,
                            0.95,0.92,0.03,fCollisionSystem,plotDataSets,fTrigger[0],31);
        drawLatexAdd(Form("p_{T}^{jet} = %i - %i GeV", binsPt[binPt], binsPt[binPt+1]),0.95,0.8, 0.03,kFALSE, kFALSE, kTRUE);
        drawLatexAdd(Form("%s Jets, R=0.%i", jetType.Data(), radius),0.95,0.84, 0.03,kFALSE, kFALSE, kTRUE);
        SaveCanvas(canvas, Form("%s/Comparison/PtChMax_%s_%i-%iGeV.%s", outputDir.Data(), trigName.Data(), binsPt[binPt], binsPt[binPt+1], suffix.Data()), false, true);

        DrawPeriodQACompareHistoRatioTH1(canvas,0.11, 0.02, 0.05, 0.11,kFALSE,kFALSE,kFALSE,
                            vecPtChMax[binPt],"","p_{T,ch}^{const,max}","#frac{1}{N^Evt} #frac{dN^{jet}}{dp_{T,ch}^{const,max}}",1,1.1,
                            labelData, colorCompare, kTRUE, 1.1, 1.1, kTRUE,
                            0.95,0.92,0.03,fCollisionSystem,plotDataSets,fTrigger[0],31);
        drawLatexAdd(Form("p_{T}^{jet} = %i - %i GeV", binsPt[binPt], binsPt[binPt+1]),0.95,0.8, 0.03,kFALSE, kFALSE, kTRUE);
        drawLatexAdd(Form("%s Jets, R=0.%i", jetType.Data(), radius),0.95,0.84, 0.03,kFALSE, kFALSE, kTRUE);
        SaveCanvas(canvas, Form("%s/Comparison/Ratios/ratio_PtChMax_%s_%i-%iGeV.%s", outputDir.Data(), trigName.Data(), binsPt[binPt], binsPt[binPt+1], suffix.Data()));
    }

    //-------------------------------------------------------------------------------------------------------------------------------
    // Pt of Max Neutral Constituents
    for (Int_t binPt = 0; binPt < nPtBins; binPt++){
        GetMinMaxBin(vecPtNeMax[binPt],minB,maxB);
        for(Int_t iVec=0; iVec<(Int_t)vecPtNeMax[binPt].size(); iVec++){
            TH1D* temp = vecPtNeMax[binPt].at(iVec);
            temp->Sumw2();
            temp->Scale(1./temp->Integral());
            //temp->Scale(1./NeMaxvents[iVec]);
            SetXRange(temp,minB,maxB);
        }

        DrawPeriodQACompareHistoTH1(canvas,0.11, 0.02, 0.05, 0.11,kFALSE,kFALSE,kFALSE,
                            vecPtNeMax[binPt],"","p_{T,ne}^{const,max}","#frac{1}{N^{Evt}} #frac{dN^{jet}}{dp_{T,ne}^{const,max}}",1,1.1,
                            labelData, colorCompare, kTRUE, 1.1, 1.1, kTRUE,
                            0.95,0.92,0.03,fCollisionSystem,plotDataSets,fTrigger[0],31);
        drawLatexAdd(Form("p_{T}^{jet} = %i - %i GeV", binsPt[binPt], binsPt[binPt+1]),0.95,0.8, 0.03,kFALSE, kFALSE, kTRUE);
        drawLatexAdd(Form("%s Jets, R=0.%i", jetType.Data(), radius),0.95,0.84, 0.03,kFALSE, kFALSE, kTRUE);
        SaveCanvas(canvas, Form("%s/Comparison/PtNeMax_%s_%i-%iGeV.%s", outputDir.Data(), trigName.Data(), binsPt[binPt], binsPt[binPt+1], suffix.Data()), false, true);

        DrawPeriodQACompareHistoRatioTH1(canvas,0.11, 0.02, 0.05, 0.11,kFALSE,kFALSE,kFALSE,
                            vecPtNeMax[binPt],"","p_{T,ne}^{const,max}","#frac{1}{N^Evt} #frac{dN^{jet}}{dp_{T,ne}^{const,max}}",1,1.1,
                            labelData, colorCompare, kTRUE, 1.1, 1.1, kTRUE,
                            0.95,0.92,0.03,fCollisionSystem,plotDataSets,fTrigger[0],31);
        drawLatexAdd(Form("p_{T}^{jet} = %i - %i GeV", binsPt[binPt], binsPt[binPt+1]),0.95,0.8, 0.03,kFALSE, kFALSE, kTRUE);
        drawLatexAdd(Form("%s Jets, R=0.%i", jetType.Data(), radius),0.95,0.84, 0.03,kFALSE, kFALSE, kTRUE);
        SaveCanvas(canvas, Form("%s/Comparison/Ratios/ratio_PtNeMax_%s_%i-%iGeV.%s", outputDir.Data(), trigName.Data(), binsPt[binPt], binsPt[binPt+1], suffix.Data()));
    }

    //-------------------------------------------------------------------------------------------------------------------------------
    // Eta-Phi Distribution
    for (Int_t binEta = 0; binEta < nEtaBins; binEta++){
        GetMinMaxBin(vecEtaPhi[binEta],minB,maxB);
        for(Int_t iVec=0; iVec<(Int_t)vecEtaPhi[binEta].size(); iVec++){
            TH1D* temp = vecEtaPhi[binEta].at(iVec);
            temp->Sumw2();
            temp->Scale(1./temp->Integral());
            //temp->Scale(1./nEvents[iVec]);
            SetXRange(temp,minB,maxB);
        }

        DrawPeriodQACompareHistoTH1(canvas,0.11, 0.02, 0.05, 0.11,kFALSE,kFALSE,kFALSE,
                            vecEtaPhi[binEta],"","#phi","#frac{1}{N^{Evt}} #frac{dN^{jet}}{d#phi}",1,1.1,
                            labelData, colorCompare, kTRUE, 1.1, 1.1, kTRUE,
                            0.95,0.92,0.03,fCollisionSystem,plotDataSets,fTrigger[0],31);
        drawLatexAdd(Form("%s Jets, R=0.%i", jetType.Data(), radius),0.95,0.84, 0.03,kFALSE, kFALSE, kTRUE);
        drawLatexAdd(Form("#eta = %1.1f - %1.1f", binsEta[binEta], binsEta[binEta+1]),0.95,0.8, 0.03,kFALSE, kFALSE, kTRUE);
        SaveCanvas(canvas, Form("%s/Comparison/EtaPhi_%s_%1.1f_to_%1.1f.%s", outputDir.Data(), trigName.Data(), binsEta[binEta],binsEta[binEta+1], suffix.Data()));

        DrawPeriodQACompareHistoRatioTH1(canvas,0.11, 0.02, 0.05, 0.11,kFALSE,kFALSE,kFALSE,
                            vecEtaPhi[binEta],"","#phi","#frac{1}{N^Evt} #frac{dN^{jet}}{d#phi}",1,1.1,
                            labelData, colorCompare, kTRUE, 1.1, 1.1, kTRUE,
                            0.95,0.92,0.03,fCollisionSystem,plotDataSets,fTrigger[0],31);
        drawLatexAdd(Form("%s Jets, R=0.%i", jetType.Data(), radius),0.95,0.84, 0.03,kFALSE, kFALSE, kTRUE);
        drawLatexAdd(Form("#eta = %1.1f - %1.1f", binsEta[binEta], binsEta[binEta+1]),0.95,0.8, 0.03,kFALSE, kFALSE, kTRUE);
        SaveCanvas(canvas, Form("%s/Comparison/Ratios/ratio_EtaPhi_%s_%1.1f_to_%1.1f.%s", outputDir.Data(), trigName.Data(), binsEta[binEta],binsEta[binEta+1], suffix.Data()));
    }

    //-------------------------------------------------------------------------------------------------------------------------------
    // Eta-Phi Distribution of Charged Constituents
    for (Int_t binEta = 0; binEta < nEtaBins; binEta++){
        GetMinMaxBin(vecEtaPhiCh[binEta],minB,maxB);
        for(Int_t iVec=0; iVec<(Int_t)vecEtaPhiCh[binEta].size(); iVec++){
            TH1D* temp = vecEtaPhiCh[binEta].at(iVec);
            temp->Sumw2();
            temp->Scale(1./temp->Integral());
            //temp->Scale(1./nEvents[iVec]);
            SetXRange(temp,minB,maxB);
        }

        DrawPeriodQACompareHistoTH1(canvas,0.11, 0.02, 0.05, 0.11,kFALSE,kFALSE,kFALSE,
                            vecEtaPhiCh[binEta],"","#phi_{ch}","#frac{1}{N^{Evt}} #frac{dN^{jet}}{d#phi_{ch}}",1,1.1,
                            labelData, colorCompare, kTRUE, 1.1, 1.1, kTRUE,
                            0.95,0.92,0.03,fCollisionSystem,plotDataSets,fTrigger[0],31);
        drawLatexAdd(Form("%s Jets, R=0.%i", jetType.Data(), radius),0.95,0.84, 0.03,kFALSE, kFALSE, kTRUE);
        drawLatexAdd(Form("#eta = %1.1f - %1.1f", binsEta[binEta], binsEta[binEta+1]),0.95,0.8, 0.03,kFALSE, kFALSE, kTRUE);
        SaveCanvas(canvas, Form("%s/Comparison/EtaPhiCh_%s_%1.1f_to_%1.1f.%s", outputDir.Data(), trigName.Data(), binsEta[binEta],binsEta[binEta+1], suffix.Data()));

        DrawPeriodQACompareHistoRatioTH1(canvas,0.11, 0.02, 0.05, 0.11,kFALSE,kFALSE,kFALSE,
                            vecEtaPhiCh[binEta],"","#phi_{ch}","#frac{1}{N^Evt} #frac{dN^{jet}}{d#phi_{ch}}",1,1.1,
                            labelData, colorCompare, kTRUE, 1.1, 1.1, kTRUE,
                            0.95,0.92,0.03,fCollisionSystem,plotDataSets,fTrigger[0],31);
        drawLatexAdd(Form("%s Jets, R=0.%i", jetType.Data(), radius),0.95,0.84, 0.03,kFALSE, kFALSE, kTRUE);
        drawLatexAdd(Form("#eta = %1.1f - %1.1f", binsEta[binEta], binsEta[binEta+1]),0.95,0.8, 0.03,kFALSE, kFALSE, kTRUE);
        SaveCanvas(canvas, Form("%s/Comparison/Ratios/ratio_EtaPhiCh_%s_%1.1f_to_%1.1f.%s", outputDir.Data(), trigName.Data(), binsEta[binEta],binsEta[binEta+1], suffix.Data()));
    }

    //-------------------------------------------------------------------------------------------------------------------------------
    // Eta-Phi Distribution of Neutral Constituents
    for (Int_t binEta = 0; binEta < nEtaBins; binEta++){
        GetMinMaxBin(vecEtaPhiNe[binEta],minB,maxB);
        for(Int_t iVec=0; iVec<(Int_t)vecEtaPhiNe[binEta].size(); iVec++){
            TH1D* temp = vecEtaPhiNe[binEta].at(iVec);
            temp->Sumw2();
            temp->Scale(1./temp->Integral());
            //temp->Scale(1./nEvents[iVec]);
            SetXRange(temp,minB,maxB);
        }

        DrawPeriodQACompareHistoTH1(canvas,0.11, 0.02, 0.05, 0.11,kFALSE,kFALSE,kFALSE,
                            vecEtaPhiNe[binEta],"","#phi_{ne}","#frac{1}{N^{Evt}} #frac{dN^{jet}}{d#phi_{ne}}",1,1.1,
                            labelData, colorCompare, kTRUE, 1.1, 1.1, kTRUE,
                            0.95,0.92,0.03,fCollisionSystem,plotDataSets,fTrigger[0],31);
        drawLatexAdd(Form("%s Jets, R=0.%i", jetType.Data(), radius),0.95,0.84, 0.03,kFALSE, kFALSE, kTRUE);
        drawLatexAdd(Form("#eta = %1.1f - %1.1f", binsEta[binEta], binsEta[binEta+1]),0.95,0.8, 0.03,kFALSE, kFALSE, kTRUE);
        SaveCanvas(canvas, Form("%s/Comparison/EtaPhiNe_%s_%1.1f_to_%1.1f.%s", outputDir.Data(), trigName.Data(), binsEta[binEta],binsEta[binEta+1], suffix.Data()));

        DrawPeriodQACompareHistoRatioTH1(canvas,0.11, 0.02, 0.05, 0.11,kFALSE,kFALSE,kFALSE,
                            vecEtaPhiNe[binEta],"","#phi_{ne}","#frac{1}{N^Evt} #frac{dN^{jet}}{d#phi_{ne}}",1,1.1,
                            labelData, colorCompare, kTRUE, 1.1, 1.1, kTRUE,
                            0.95,0.92,0.03,fCollisionSystem,plotDataSets,fTrigger[0],31);
        drawLatexAdd(Form("%s Jets, R=0.%i", jetType.Data(), radius),0.95,0.84, 0.03,kFALSE, kFALSE, kTRUE);
        drawLatexAdd(Form("#eta = %1.1f - %1.1f", binsEta[binEta], binsEta[binEta+1]),0.95,0.8, 0.03,kFALSE, kFALSE, kTRUE);
        SaveCanvas(canvas, Form("%s/Comparison/Ratios/ratio_EtaPhiNe_%s_%1.1f_to_%1.1f.%s", outputDir.Data(), trigName.Data(), binsEta[binEta],binsEta[binEta+1], suffix.Data()));
    }

    //-------------------------------------------------------------------------------------------------------------------------------
    // Eta-Phi Distribution of Max Charged Constituents
    for (Int_t binEta = 0; binEta < nEtaBins; binEta++){
        GetMinMaxBin(vecEtaPhiChMax[binEta],minB,maxB);
        for(Int_t iVec=0; iVec<(Int_t)vecEtaPhiChMax[binEta].size(); iVec++){
            TH1D* temp = vecEtaPhiChMax[binEta].at(iVec);
            temp->Sumw2();
            temp->Scale(1./temp->Integral());
            //temp->Scale(1./nEvents[iVec]);
            SetXRange(temp,minB,maxB);
        }

        DrawPeriodQACompareHistoTH1(canvas,0.11, 0.02, 0.05, 0.11,kFALSE,kFALSE,kFALSE,
                            vecEtaPhiChMax[binEta],"","#phi_{ch}^{max}","#frac{1}{N^{Evt}} #frac{dN^{jet}}{d#phi_{ch}^{max}}",1,1.1,
                            labelData, colorCompare, kTRUE, 1.1, 1.1, kTRUE,
                            0.95,0.92,0.03,fCollisionSystem,plotDataSets,fTrigger[0],31);
        drawLatexAdd(Form("%s Jets, R=0.%i", jetType.Data(), radius),0.95,0.84, 0.03,kFALSE, kFALSE, kTRUE);
        drawLatexAdd(Form("#eta = %1.1f - %1.1f", binsEta[binEta], binsEta[binEta+1]),0.95,0.8, 0.03,kFALSE, kFALSE, kTRUE);
        SaveCanvas(canvas, Form("%s/Comparison/EtaPhiChMax_%s_%1.1f_to_%1.1f.%s", outputDir.Data(), trigName.Data(), binsEta[binEta],binsEta[binEta+1], suffix.Data()));

        DrawPeriodQACompareHistoRatioTH1(canvas,0.11, 0.02, 0.05, 0.11,kFALSE,kFALSE,kFALSE,
                            vecEtaPhiChMax[binEta],"","#phi_{ch}^{max}","#frac{1}{N^Evt} #frac{dN^{jet}}{d#phi_{ch}^{max}}",1,1.1,
                            labelData, colorCompare, kTRUE, 1.1, 1.1, kTRUE,
                            0.95,0.92,0.03,fCollisionSystem,plotDataSets,fTrigger[0],31);
        drawLatexAdd(Form("%s Jets, R=0.%i", jetType.Data(), radius),0.95,0.84, 0.03,kFALSE, kFALSE, kTRUE);
        drawLatexAdd(Form("#eta = %1.1f - %1.1f", binsEta[binEta], binsEta[binEta+1]),0.95,0.8, 0.03,kFALSE, kFALSE, kTRUE);
        SaveCanvas(canvas, Form("%s/Comparison/Ratios/ratio_EtaPhiChMax_%s_%1.1f_to_%1.1f.%s", outputDir.Data(), trigName.Data(), binsEta[binEta],binsEta[binEta+1], suffix.Data()));
    }

    //-------------------------------------------------------------------------------------------------------------------------------
    // Eta-Phi Distribution of Max Neutral Constituents
    for (Int_t binEta = 0; binEta < nEtaBins; binEta++){
        GetMinMaxBin(vecEtaPhiNeMax[binEta],minB,maxB);
        for(Int_t iVec=0; iVec<(Int_t)vecEtaPhiNeMax[binEta].size(); iVec++){
            TH1D* temp = vecEtaPhiNeMax[binEta].at(iVec);
            temp->Sumw2();
            temp->Scale(1./temp->Integral());
            //temp->Scale(1./nEvents[iVec]);
            SetXRange(temp,minB,maxB);
        }

        DrawPeriodQACompareHistoTH1(canvas,0.11, 0.02, 0.05, 0.11,kFALSE,kFALSE,kFALSE,
                            vecEtaPhiNeMax[binEta],"","#phi_{ne}^{max}","#frac{1}{N^{Evt}} #frac{dN^{jet}}{d#phi_{ne}^{max}}",1,1.1,
                            labelData, colorCompare, kTRUE, 1.1, 1.1, kTRUE,
                            0.95,0.92,0.03,fCollisionSystem,plotDataSets,fTrigger[0],31);
        drawLatexAdd(Form("%s Jets, R=0.%i", jetType.Data(), radius),0.95,0.84, 0.03,kFALSE, kFALSE, kTRUE);
        drawLatexAdd(Form("#eta = %1.1f - %1.1f", binsEta[binEta], binsEta[binEta+1]),0.95,0.8, 0.03,kFALSE, kFALSE, kTRUE);
        SaveCanvas(canvas, Form("%s/Comparison/EtaPhiNeMax_%s_%1.1f_to_%1.1f.%s", outputDir.Data(), trigName.Data(), binsEta[binEta],binsEta[binEta+1], suffix.Data()));

        DrawPeriodQACompareHistoRatioTH1(canvas,0.11, 0.02, 0.05, 0.11,kFALSE,kFALSE,kFALSE,
                            vecEtaPhiNeMax[binEta],"","#phi_{ne}^{max}","#frac{1}{N^Evt} #frac{dN^{jet}}{d#phi_{ne}^{max}}",1,1.1,
                            labelData, colorCompare, kTRUE, 1.1, 1.1, kTRUE,
                            0.95,0.92,0.03,fCollisionSystem,plotDataSets,fTrigger[0],31);
        drawLatexAdd(Form("%s Jets, R=0.%i", jetType.Data(), radius),0.95,0.84, 0.03,kFALSE, kFALSE, kTRUE);
        drawLatexAdd(Form("#eta = %1.1f - %1.1f", binsEta[binEta], binsEta[binEta+1]),0.95,0.8, 0.03,kFALSE, kFALSE, kTRUE);
        SaveCanvas(canvas, Form("%s/Comparison/Ratios/ratio_EtaPhiNeMax_%s_%1.1f_to_%1.1f.%s", outputDir.Data(), trigName.Data(), binsEta[binEta],binsEta[binEta+1], suffix.Data()));
    }

    //-------------------------------------------------------------------------------------------------------------------------------
    // Jet Area
    for (Int_t binPt = 0; binPt < nPtBins; binPt++){
        GetMinMaxBin(vecJetArea[binPt],minB,maxB);
        for(Int_t iVec=0; iVec<(Int_t)vecJetArea[binPt].size(); iVec++){
            TH1D* temp = vecJetArea[binPt].at(iVec);
            temp->Sumw2();
            temp->Scale(1./temp->Integral());
            //temp->Scale(1./nEvents[iVec]);
            SetXRange(temp,minB,maxB);
        }

        DrawPeriodQACompareHistoTH1(canvas,0.11, 0.02, 0.05, 0.11,kFALSE,kFALSE,kFALSE,
                            vecJetArea[binPt],"","Area","#frac{1}{N^{Evt}} #frac{dN^{jet}}{dArea}",1,1.1,
                            labelData, colorCompare, kTRUE, 1.1, 1.1, kTRUE,
                            0.95,0.92,0.03,fCollisionSystem,plotDataSets,fTrigger[0],31);
        drawLatexAdd(Form("%s Jets, R=0.%i", jetType.Data(), radius),0.95,0.84, 0.03,kFALSE, kFALSE, kTRUE);
        drawLatexAdd(Form("%i - %i GeV", binsPt[binPt], binsPt[binPt+1]),0.95,0.8, 0.03,kFALSE, kFALSE, kTRUE);
        SaveCanvas(canvas, Form("%s/Comparison/JetArea_%s_%i-%iGeV.%s", outputDir.Data(), trigName.Data(), binsPt[binPt], binsPt[binPt+1], suffix.Data()));

        DrawPeriodQACompareHistoRatioTH1(canvas,0.11, 0.02, 0.05, 0.11,kFALSE,kFALSE,kFALSE,
                            vecJetArea[binPt],"","Area","#frac{1}{N^Evt} #frac{dN^{jet}}{dArea}",1,1.1,
                            labelData, colorCompare, kTRUE, 1.1, 1.1, kTRUE,
                            0.95,0.92,0.03,fCollisionSystem,plotDataSets,fTrigger[0],31);
        drawLatexAdd(Form("%s Jets, R=0.%i", jetType.Data(), radius),0.95,0.84, 0.03,kFALSE, kFALSE, kTRUE);
        drawLatexAdd(Form("%i - %i GeV", binsPt[binPt], binsPt[binPt+1]),0.95,0.8, 0.03,kFALSE, kFALSE, kTRUE);
        SaveCanvas(canvas, Form("%s/Comparison/Ratios/ratio_JetArea_%s_%i-%iGeV.%s", outputDir.Data(), trigName.Data(), binsPt[binPt], binsPt[binPt+1], suffix.Data()));
    }

    //-------------------------------------------------------------------------------------------------------------------------------
    // Number of Constituents
    for (Int_t binArea = 0; binArea < nAreaBins; binArea++){
        GetMinMaxBin(vecNConst[binArea],minB,maxB);
        for(Int_t iVec=0; iVec<(Int_t)vecNConst[binArea].size(); iVec++){
            TH1D* temp = vecNConst[binArea].at(iVec);
            temp->Sumw2();
            temp->Scale(1./temp->Integral());
            //temp->Scale(1./nEvents[iVec]);
            SetXRange(temp,minB,maxB);
        }

        DrawPeriodQACompareHistoTH1(canvas,0.11, 0.02, 0.05, 0.11,kFALSE,kFALSE,kFALSE,
                            vecNConst[binArea],"","N_{constituents}","#frac{1}{N^{Evt}} #frac{dN^{jet}}{dN_{constituents}}",1,1.1,
                            labelData, colorCompare, kTRUE, 1.1, 1.1, kTRUE,
                            0.95,0.92,0.03,fCollisionSystem,plotDataSets,fTrigger[0],31);
        drawLatexAdd(Form("%s Jets, R=0.%i", jetType.Data(), radius),0.95,0.84, 0.03,kFALSE, kFALSE, kTRUE);
        drawLatexAdd(Form("Area = %1.2f - %1.2f", binsArea[binArea], binsArea[binArea+1]),0.95,0.8, 0.03,kFALSE, kFALSE, kTRUE);
        SaveCanvas(canvas, Form("%s/Comparison/NConstituents_%s_%1.2f-%1.2f.%s", outputDir.Data(), trigName.Data(), binsArea[binArea], binsArea[binArea+1], suffix.Data()));

        DrawPeriodQACompareHistoRatioTH1(canvas,0.11, 0.02, 0.05, 0.11,kFALSE,kFALSE,kFALSE,
                            vecNConst[binArea],"","N_{constituents}","#frac{1}{N^{Evt}} #frac{dN^{jet}}{dN_{constituents}}",1,1.1,
                            labelData, colorCompare, kTRUE, 1.1, 1.1, kTRUE,
                            0.95,0.92,0.03,fCollisionSystem,plotDataSets,fTrigger[0],31);
        drawLatexAdd(Form("%s Jets, R=0.%i", jetType.Data(), radius),0.95,0.84, 0.03,kFALSE, kFALSE, kTRUE);
        drawLatexAdd(Form("Area = %1.2f - %1.2f", binsArea[binArea], binsArea[binArea+1]),0.95,0.8, 0.03,kFALSE, kFALSE, kTRUE);
        SaveCanvas(canvas, Form("%s/Comparison/Ratios/ratio_NConstituents_%s_%1.2f-%1.2f.%s", outputDir.Data(), trigName.Data(), binsArea[binArea], binsArea[binArea+1], suffix.Data()));
    }

    //-------------------------------------------------------------------------------------------------------------------------------
    // Cluster Time
    GetMinMaxBin(vecClusterTime,minB,maxB);
    for(Int_t iVec=0; iVec<(Int_t)vecClusterTime.size(); iVec++){
        TH1D* temp = vecClusterTime.at(iVec);
        temp->Sumw2();
        temp->Scale(1./temp->Integral());
        //temp->Scale(1./nEvents[iVec]);
        SetXRange(temp,minB,maxB);
    }
    DrawPeriodQACompareHistoTH1(canvas,0.11, 0.02, 0.05, 0.11,kFALSE,kFALSE,kFALSE,
                        vecClusterTime,"","t^{cluster} (ns)","#frac{1}{N} #frac{dN^{jet}}{dt^{cluster}}",1,1.1,
                        labelData, colorCompare, kTRUE, 1.1, 1.1, kTRUE,
                        0.95,0.92,0.03,fCollisionSystem,plotDataSets,fTrigger[0],31);
    drawLatexAdd(Form("%s Jets, R=0.%i", jetType.Data(), radius),0.95,0.84, 0.03,kFALSE, kFALSE, kTRUE);
    SaveCanvas(canvas, Form("%s/Comparison/ClusterTime_%s.%s", outputDir.Data(), trigName.Data(), suffix.Data()));

    DrawPeriodQACompareHistoRatioTH1(canvas,0.11, 0.02, 0.05, 0.11,kFALSE,kFALSE,kFALSE,
                        vecClusterTime,"","t^{cluster} (ns)","#frac{1}{N} #frac{dN^{jet}}{dt^{cluster}}",1,1.1,
                        labelData, colorCompare, kTRUE, 1.1, 1.1, kTRUE,
                        0.95,0.92,0.03,fCollisionSystem,plotDataSets,fTrigger[0],31);
    drawLatexAdd(Form("%s Jets, R=0.%i", jetType.Data(), radius),0.95,0.84, 0.03,kFALSE, kFALSE, kTRUE);
    SaveCanvas(canvas, Form("%s/Comparison/Ratios/ratio_ClusterTime_%s.%s", outputDir.Data(), trigName.Data(), suffix.Data()));

    //-------------------------------------------------------------------------------------------------------------------------------
    // Cluster Energy
    GetMinMaxBin(vecClusterEnergy,minB,maxB);
    for(Int_t iVec=0; iVec<(Int_t)vecClusterEnergy.size(); iVec++){
        TH1D* temp = vecClusterEnergy.at(iVec);
        temp->Sumw2();
        temp->Scale(1./temp->Integral());
        //temp->Scale(1./nEvents[iVec]);
        SetXRange(temp,minB,maxB);
    }
    DrawPeriodQACompareHistoTH1(canvas,0.11, 0.02, 0.05, 0.11,kFALSE,kFALSE,kFALSE,
                        vecClusterEnergy,"","E^{cluster} (GeV)","#frac{1}{N} #frac{dN^{jet}}{dE^{cluster}}",1,1.1,
                        labelData, colorCompare, kTRUE, 1.1, 1.1, kTRUE,
                        0.95,0.92,0.03,fCollisionSystem,plotDataSets,fTrigger[0],31);
    drawLatexAdd(Form("%s Jets, R=0.%i", jetType.Data(), radius),0.95,0.84, 0.03,kFALSE, kFALSE, kTRUE);
    SaveCanvas(canvas, Form("%s/Comparison/ClusterEnergy_%s.%s", outputDir.Data(), trigName.Data(), suffix.Data()), false, true);

    DrawPeriodQACompareHistoRatioTH1(canvas,0.11, 0.02, 0.05, 0.11,kFALSE,kFALSE,kFALSE,
                        vecClusterEnergy,"","E^{cluster} (GeV)","#frac{1}{N} #frac{dN^{jet}}{dE^{cluster}}",1,1.1,
                        labelData, colorCompare, kTRUE, 1.1, 1.1, kTRUE,
                        0.95,0.92,0.03,fCollisionSystem,plotDataSets,fTrigger[0],31);
    drawLatexAdd(Form("%s Jets, R=0.%i", jetType.Data(), radius),0.95,0.84, 0.03,kFALSE, kFALSE, kTRUE);
    SaveCanvas(canvas, Form("%s/Comparison/Ratios/ratio_ClusterEnergy_%s.%s", outputDir.Data(), trigName.Data(), suffix.Data()));

    //-------------------------------------------------------------------------------------------------------------------------------
    // Cluster Cells
    for (Int_t binE = 0; binE < nEBins; binE++){
        GetMinMaxBin(vecClusterCells[binE],minB,maxB);
        for(Int_t iVec=0; iVec<(Int_t)vecClusterCells[binE].size(); iVec++){
            TH1D* temp = vecClusterCells[binE].at(iVec);
            temp->Sumw2();
            temp->Scale(1./temp->Integral());
            //temp->Scale(1./nEvents[iVec]);
            SetXRange(temp,minB,maxB);
        }

        DrawPeriodQACompareHistoTH1(canvas,0.11, 0.02, 0.05, 0.11,kFALSE,kFALSE,kFALSE,
                            vecClusterCells[binE],"","M02","#frac{1}{N^{Evt}} #frac{dN^{jet}}{dM02}",1,1.1,
                            labelData, colorCompare, kTRUE, 1.1, 1.1, kTRUE,
                            0.95,0.92,0.03,fCollisionSystem,plotDataSets,fTrigger[0],31);
        drawLatexAdd(Form("%i - %i GeV", binsE[binE], binsE[binE+1]),0.95,0.8, 0.03,kFALSE, kFALSE, kTRUE);
        drawLatexAdd(Form("%s Jets, R=0.%i", jetType.Data(), radius),0.95,0.84, 0.03,kFALSE, kFALSE, kTRUE);
        SaveCanvas(canvas, Form("%s/Comparison/ClusterCells_%s_%i-%iGeV.%s", outputDir.Data(), trigName.Data(), binsE[binE], binsE[binE+1], suffix.Data()), false, true);

        DrawPeriodQACompareHistoRatioTH1(canvas,0.11, 0.02, 0.05, 0.11,kFALSE,kFALSE,kFALSE,
                            vecClusterCells[binE],"","M02","#frac{1}{N^Evt} #frac{dN^{jet}}{dM02}",1,1.1,
                            labelData, colorCompare, kTRUE, 1.1, 1.1, kTRUE,
                            0.95,0.92,0.03,fCollisionSystem,plotDataSets,fTrigger[0],31);
        drawLatexAdd(Form("%i - %i GeV", binsE[binE], binsE[binE+1]),0.95,0.8, 0.03,kFALSE, kFALSE, kTRUE);
        drawLatexAdd(Form("%s Jets, R=0.%i", jetType.Data(), radius),0.95,0.84, 0.03,kFALSE, kFALSE, kTRUE);
        SaveCanvas(canvas, Form("%s/Comparison/Ratios/ratio_ClusterCells_%s_%i-%iGeV.%s", outputDir.Data(), trigName.Data(), binsE[binE], binsE[binE+1], suffix.Data()));
    }

    //-------------------------------------------------------------------------------------------------------------------------------
    // Jet M02
    for (Int_t binE = 0; binE < nEBins; binE++){
        GetMinMaxBin(vecM02[binE],minB,maxB);
        for(Int_t iVec=0; iVec<(Int_t)vecM02[binE].size(); iVec++){
            TH1D* temp = vecM02[binE].at(iVec);
            temp->Sumw2();
            temp->Scale(1./temp->Integral());
            //temp->Scale(1./nEvents[iVec]);
            SetXRange(temp,minB,maxB);
        }

        DrawPeriodQACompareHistoTH1(canvas,0.11, 0.02, 0.05, 0.11,kFALSE,kFALSE,kFALSE,
                            vecM02[binE],"","M02","#frac{1}{N^{Evt}} #frac{dN^{jet}}{dM02}",1,1.1,
                            labelData, colorCompare, kTRUE, 1.1, 1.1, kTRUE,
                            0.95,0.92,0.03,fCollisionSystem,plotDataSets,fTrigger[0],31);
        drawLatexAdd(Form("%i - %i GeV", binsE[binE], binsE[binE+1]),0.95,0.8, 0.03,kFALSE, kFALSE, kTRUE);
        drawLatexAdd(Form("%s Jets, R=0.%i", jetType.Data(), radius),0.95,0.84, 0.03,kFALSE, kFALSE, kTRUE);
        SaveCanvas(canvas, Form("%s/Comparison/M02_%s_%i-%iGeV.%s", outputDir.Data(), trigName.Data(), binsE[binE], binsE[binE+1], suffix.Data()), false, true);

        DrawPeriodQACompareHistoRatioTH1(canvas,0.11, 0.02, 0.05, 0.11,kFALSE,kFALSE,kFALSE,
                            vecM02[binE],"","M02","#frac{1}{N^Evt} #frac{dN^{jet}}{dM02}",1,1.1,
                            labelData, colorCompare, kTRUE, 1.1, 1.1, kTRUE,
                            0.95,0.92,0.03,fCollisionSystem,plotDataSets,fTrigger[0],31);
        drawLatexAdd(Form("%i - %i GeV", binsE[binE], binsE[binE+1]),0.95,0.8, 0.03,kFALSE, kFALSE, kTRUE);
        drawLatexAdd(Form("%s Jets, R=0.%i", jetType.Data(), radius),0.95,0.84, 0.03,kFALSE, kFALSE, kTRUE);
        SaveCanvas(canvas, Form("%s/Comparison/Ratios/ratio_M02_%s_%i-%iGeV.%s", outputDir.Data(), trigName.Data(), binsE[binE], binsE[binE+1], suffix.Data()));
    }

    // *****************************************************************************************************
    // ***************************** Cleanup vectors *******************************************************
    // *****************************************************************************************************

    fLog.close();

    delete cvsQuadratic;
    delete canvas;

    DeleteVecTH1D(vecNEvents);
    DeleteVecTH1D(vecVertexZ);
    DeleteVecTH1D(vecCentrality);
    DeleteVecTH1D(vecCentrality2);
    DeleteVecTH1D(vecEventPlaneAngle);
    DeleteVecTH1D(vecJetPt);
    DeleteVecTH1D(vecJetPtMax);
    for (Int_t binPt = 0; binPt < nPtBins; binPt++){
        DeleteVecTH1D(vecNEF[binPt]);
        DeleteVecTH1D(vecZch[binPt]);
        DeleteVecTH1D(vecZne[binPt]);
        DeleteVecTH1D(vecZchMax[binPt]);
        DeleteVecTH1D(vecZneMax[binPt]);
        DeleteVecTH1D(vecNch[binPt]);
        DeleteVecTH1D(vecNne[binPt]);
        DeleteVecTH1D(vecPtCh[binPt]);
        DeleteVecTH1D(vecPtNe[binPt]);
        DeleteVecTH1D(vecPtChMax[binPt]);
        DeleteVecTH1D(vecPtNeMax[binPt]);
        DeleteVecTH1D(vecJetArea[binPt]);
    }
    for (Int_t binEta = 0; binEta < nEtaBins; binEta++){
        DeleteVecTH1D(vecEtaPhi[binEta]);
        DeleteVecTH1D(vecEtaPhiCh[binEta]);
        DeleteVecTH1D(vecEtaPhiNe[binEta]);
        DeleteVecTH1D(vecEtaPhiChMax[binEta]);
        DeleteVecTH1D(vecEtaPhiNeMax[binEta]);
    }
    DeleteVecTH1D(vecClusterEnergy);
    DeleteVecTH1D(vecClusterTime);
    for (Int_t binArea = 0; binArea < nAreaBins; binArea++){
        DeleteVecTH1D(vecNConst[binArea]);
    }
    for (Int_t binE = 0; binE < nEBins; binE++){
        DeleteVecTH1D(vecM02[binE]);
        DeleteVecTH1D(vecClusterCells[binE]);
    }

    delete[] nEvents;
    delete[] nEventsAll;

    delete[] fTrigger;

    TH1::AddDirectory(kTRUE);

    cout << "Done with JetQA" << endl;
    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;

    return;

}//end
