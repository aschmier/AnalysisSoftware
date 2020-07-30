#include "Grid_CopyFiles_DataAOD_13TeV.h"

using namespace std;

//void ChangeStrucToStd(TString nameInputFile, TString namefileOutput, TString nameInputList);
Bool_t copyAlien2Local(TString loc, TString rem);
Bool_t readin(TString fileRuns, std::vector<TString> &vec){
    cout << Form("Reading from %s...", fileRuns.Data()) << endl;
    std::fstream file;
    TString fVar;
    Int_t totalN=0;
    file.open(fileRuns.Data(), ios::in);
    if(file.good())
    {
        file.seekg(0L, ios::beg);
        while(!file.eof())
        {
            file >> fVar;
            if(fVar.Sizeof()>1)
            {
                vec.push_back(fVar);
                totalN++;
            }
        }
    }
    file.close();
    if(totalN > 0) return kTRUE;
    else return kFALSE;
}

void Grid_CopyFiles_DataAOD_13TeV(TString system, TString type, TString folder, Int_t TrainNumberChoiceArgument, Int_t doUnzipArgument)
{
    cout<<"Connecting to Alien..."<<endl;
    TGrid::Connect("alien://");
    cout<<"==============================="<<endl;
    cout<<"Successfully connected to Alien"<<endl;
    cout<<"==============================="<<endl;

    TrainNumberChoice           = TrainNumberChoiceArgument;


    //*********************************************************************************************************************************
    //*********************************************************************************************************************************
    //*********************************************************************************************************************************
    if ( TrainNumberChoice == 0 ){ //1479 20200723
        cout<<"TrainNumberChoice == "<<TrainNumberChoice<< " has been chosen" <<endl;
        //Files
        nFilesVariation                     = 2;
        nFilesVariation_max                 = 4;
        if (nFilesVariation > nFilesVariation_max){
            nFilesVariation                 = nFilesVariation_max;
        }
        FilesVariation                      = new TString[nFilesVariation_max];
        FilesVariation[0]                   = "GammaCalo_800";
        FilesVariation[1]                   = "GammaCalo_801";
        FilesVariation[2]                   = "GammaConvCalo_800";
        FilesVariation[3]                   = "GammaConvCalo_801";
        //Data
        do16Data        = 1;                TrainNumber16Data       ="1479";
        do17Data        = 1;                TrainNumber17Data       ="1480";
        do18Data        = 1;                TrainNumber18Data       ="1481";
        TrainNumberSave_Data                = TrainNumber16Data;
        TrainTag_Data                       = "20200723";
        //MC
        do16MC          = 1;                TrainNumber16MC         ="2849";
        do16MCextra     = 1;                TrainNumber16MCextra    ="2850";
        do16MCextra2    = 1;                TrainNumber16MCextra2   ="2851";
        do17MC          = 1;                TrainNumber17MC         ="2852";
        do17MCextra     = 1;                TrainNumber17MCextra    ="2853";
        do18MC          = 1;                TrainNumber18MC         ="2854";
        do18MCextra     = 1;                TrainNumber18MCextra    ="2855";
        TrainNumberSave_MC                  = TrainNumber16MC;
        TrainTag_MC                         = TrainTag_Data;
        useRunlist                          = 0;
    } else if ( TrainNumberChoice == 1 ) {//1482 20200726
        cout<<"TrainNumberChoice == "<<TrainNumberChoice<< " has been chosen" <<endl;
        //Files
        nFilesVariation                     = 2;
        nFilesVariation_max                 = 4;
        if (nFilesVariation > nFilesVariation_max){
            nFilesVariation                 = nFilesVariation_max;
        }
        FilesVariation                      = new TString[nFilesVariation_max];
        FilesVariation[0]                   = "GammaCalo_800";
        FilesVariation[1]                   = "GammaCalo_801";
        FilesVariation[2]                   = "GammaConvCalo_800";
        FilesVariation[3]                   = "GammaConvCalo_801";
        //Data
        do16Data        = 1;                TrainNumber16Data       ="1482";
        do17Data        = 1;                TrainNumber17Data       ="1483";
        do18Data        = 1;                TrainNumber18Data       ="1484";
        TrainNumberSave_Data                = TrainNumber16Data;
        TrainTag_Data                       = "20200726";
        //MC
        do16MC          = 1;                TrainNumber16MC         ="2866";
        do16MCextra     = 1;                TrainNumber16MCextra    ="2867";
        do16MCextra2    = 1;                TrainNumber16MCextra2   ="2868";
        do17MC          = 1;                TrainNumber17MC         ="2869";
        do17MCextra     = 1;                TrainNumber17MCextra    ="2870";
        do18MC          = 1;                TrainNumber18MC         ="2871";
        do18MCextra     = 1;                TrainNumber18MCextra    ="2872";
        TrainNumberSave_MC                  = TrainNumber16MC;
        TrainTag_MC                         = "20200724";
        useRunlist                          = 0;
    } else { //for Copies
        cout<<"TrainNumberChoice == "<<TrainNumberChoice<< " has been chosen" <<endl;
        //Files
        nFilesVariation                     = 1;
        nFilesVariation_max                 = 4;
        if (nFilesVariation > nFilesVariation_max){
            nFilesVariation                 = nFilesVariation_max;
        }
        FilesVariation                      = new TString[nFilesVariation_max];
        FilesVariation[0]                   = "GammaCalo_800";
        FilesVariation[1]                   = "GammaCalo_801";
        FilesVariation[2]                   = "GammaConvCalo_800";
        FilesVariation[3]                   = "GammaConvCalo_801";
        //Data
        do16Data        = 0;                TrainNumber16Data       ="";
        do17Data        = 0;                TrainNumber17Data       ="";
        do18Data        = 0;                TrainNumber18Data       ="";
        TrainNumberSave_Data                = TrainNumber16Data;
        TrainTag_Data                       = "";
        //MC
        do16MC          = 0;                TrainNumber16MC         ="";
        do16MCextra     = 0;                TrainNumber16MCextra    ="";
        do16MCextra2    = 0;                TrainNumber16MCextra2   ="";
        do17MC          = 0;                TrainNumber17MC         ="";
        do17MCextra     = 0;                TrainNumber17MCextra    ="";
        do18MC          = 0;                TrainNumber18MC         ="";
        do18MCextra     = 0;                TrainNumber18MCextra    ="";
        TrainNumberSave_MC                  = TrainNumber16MC;
        TrainTag_MC                         = TrainTag_Data;
        useRunlist                          = 0;

        cout<<"No Correct Train Config Defined; RETURNING!"<<endl;
        return;
    }
    //==============================================================================================================================
    const Int_t nFiles                      = nFilesVariation;
    TString Files[nFiles];
    for (Int_t iFilesVariationLoop=0; iFilesVariationLoop<nFilesVariation; iFilesVariationLoop++){
        Files[iFilesVariationLoop]          = FilesVariation[iFilesVariationLoop];
    }
    //------------------------------------------------------------------------------------------------------------------------------
    for (Int_t iSet=0; iSet<nSets_16Data; iSet++){
        DataSets_16Data[iSet]               = DataSets_16Data_PHOS[iSet];
        runlist_16Data[iSet]                = runlist_16Data_PHOS[iSet];
        if (useRunlist == 0){
            runlist_16Data[iSet]            = runlist_16Data_PHOS[iSet];
        }
    }
    for (Int_t iSet=0; iSet<nSets_17Data; iSet++){
        DataSets_17Data[iSet]               = DataSets_17Data_PHOS[iSet];
        runlist_17Data[iSet]                = runlist_17Data_PHOS[iSet];
        if (useRunlist == 0){
            runlist_17Data[iSet]            = runlist_17Data_PHOS[iSet];
        }
    }
    for (Int_t iSet=0; iSet<nSets_18Data; iSet++){
        DataSets_18Data[iSet]               = DataSets_18Data_PHOS[iSet];
        runlist_18Data[iSet]                = runlist_18Data_PHOS[iSet];
        if (useRunlist == 0){
            runlist_18Data[iSet]            = runlist_18Data_PHOS[iSet];
        }
    }
    //------------------------------------------------------------------------------------------------------------------------------
    for (Int_t iSet=0; iSet<nSets_16MC; iSet++){
        DataSets_16MC[iSet]                 = DataSets_16MC_PHOS[iSet];
        runlist_16MC[iSet]                  = runlist_16MC_PHOS[iSet];
        if (useRunlist == 0){
            runlist_16MC[iSet]              = runlist_16MC_PHOS[iSet];
        }
    }
    for (Int_t iSet=0; iSet<nSets_16MCextra; iSet++){
        DataSets_16MCextra[iSet]            = DataSets_16MCextra_PHOS[iSet];
        runlist_16MCextra[iSet]             = runlist_16MCextra_PHOS[iSet];
        if (useRunlist == 0){
            runlist_16MCextra[iSet]         = runlist_16MCextra_PHOS[iSet];
        }
    }
    for (Int_t iSet=0; iSet<nSets_16MCextra2; iSet++){
        DataSets_16MCextra2[iSet]           = DataSets_16MCextra2_PHOS[iSet];
        runlist_16MCextra2[iSet]            = runlist_16MCextra2_PHOS[iSet];
        if (useRunlist == 0){
            runlist_16MCextra2[iSet]        = runlist_16MCextra2_PHOS[iSet];
        }
    }
    //---------------------------------------------------------------
    for (Int_t iSet=0; iSet<nSets_17MC; iSet++){
        DataSets_17MC[iSet]                 = DataSets_17MC_PHOS[iSet];
        runlist_17MC[iSet]                  = runlist_17MC_PHOS[iSet];
        if (useRunlist == 0){
            runlist_17MC[iSet]              = runlist_17MC_PHOS[iSet];
        }
    }
    for (Int_t iSet=0; iSet<nSets_17MCextra; iSet++){
        DataSets_17MCextra[iSet]            = DataSets_17MCextra_PHOS[iSet];
        runlist_17MCextra[iSet]             = runlist_17MCextra_PHOS[iSet];
        if (useRunlist == 0){
            runlist_17MCextra[iSet]         = runlist_17MCextra_PHOS[iSet];
        }
    }
    //---------------------------------------------------------------
    for (Int_t iSet=0; iSet<nSets_18MC; iSet++){
        DataSets_18MC[iSet]                 = DataSets_18MC_PHOS[iSet];
        runlist_18MC[iSet]                  = runlist_18MC_PHOS[iSet];
        if (useRunlist == 0){
            runlist_18MC[iSet]              = runlist_18MC_PHOS[iSet];
        }
    }
    for (Int_t iSet=0; iSet<nSets_18MCextra; iSet++){
        DataSets_18MCextra[iSet]            = DataSets_18MCextra_PHOS[iSet];
        runlist_18MCextra[iSet]             = runlist_18MCextra_PHOS[iSet];
        if (useRunlist == 0){
            runlist_18MCextra[iSet]         = runlist_18MCextra_PHOS[iSet];
        }
    }
    //==============================================================================================================================
    nSets_nonConst                          = 0;
    nData_nonconst                          = 0;
    nMerge_nonconst                         = 0;
    //------------------------------------------------------------------------------------------------------------------------------
    if (do16Data>=1){nSets_nonConst+=nSets_16Data; nData_nonconst+=nSets_16Data; nMerge_nonconst++;}
    if (do17Data>=1){nSets_nonConst+=nSets_17Data; nData_nonconst+=nSets_17Data; nMerge_nonconst++;}
    if (do18Data>=1){nSets_nonConst+=nSets_18Data; nData_nonconst+=nSets_18Data; nMerge_nonconst++;}
    //---------------------------------------------------------------
    if (do16MC>=1){nSets_nonConst+=nSets_16MC; nMerge_nonconst++;}
    if (do16MCextra>=1){nSets_nonConst+=nSets_16MCextra; nMerge_nonconst++;}
    if (do16MCextra2>=1){nSets_nonConst+=nSets_16MCextra2; nMerge_nonconst++;}
    //---------------------------------------------------------------
    if (do17MC>=1){nSets_nonConst+=nSets_17MC; nMerge_nonconst++;}
    if (do17MCextra>=1){nSets_nonConst+=nSets_17MCextra; nMerge_nonconst++;}
    //---------------------------------------------------------------
    if (do18MC>=1){nSets_nonConst+=nSets_18MC; nMerge_nonconst++;}
    if (do18MCextra>=1){nSets_nonConst+=nSets_18MCextra; nMerge_nonconst++;}
    //---------------------------------------------------------------
    if ((do16Data>=1)||(do17Data>=1)||(do18Data>=1)){nMerge_nonconst++;}
    if ((do16MC>=1)||(do16MCextra>=1)||(do16MCextra2>=1)){nMerge_nonconst++;}
    if ((do17MC>=1)||(do17MCextra>=1)){nMerge_nonconst++;}
    if ((do18MC>=1)||(do18MCextra>=1)){nMerge_nonconst++;}
    if ((do16MC>=1)||(do16MCextra>=1)||(do17MC>=1)||(do17MCextra>=1)||(do18MC>=1)||(do18MCextra>=1)){nMerge_nonconst++;}
    //------------------------------------------------------------------------------------------------------------------------------
    const Int_t nSets_const                 = nSets_nonConst;
    const Int_t nData_const                 = nData_nonconst;
    const Int_t nMerge_const                = nMerge_nonconst;
    //---------------------------------------------------------------
    TString DataSets[nSets_const];
    TString runlist[nSets_const];
    Int_t trainRuns[nSets_const];
    TString strMerge[nMerge_const];
    Int_t mergeStart[nMerge_const];
    Int_t mergeEnd[nMerge_const];
    nMerge_Current                          = 0;
    nSets_Current                           = 0;
    //==============================================================================================================================
    for (Int_t inSetsLoop=0; inSetsLoop<nSets_const; inSetsLoop++){
        DataSets[inSetsLoop]               = "";
        runlist[inSetsLoop]                = "";
        trainRuns[inSetsLoop]              = 0;
    }
    for (Int_t iMergeVectors=0; iMergeVectors<nMerge_const; iMergeVectors++){
        strMerge[iMergeVectors]              = "";
        mergeStart[iMergeVectors]            = 0;
        mergeEnd[iMergeVectors]              = 0;
    }
    //==============================================================================================================================
    if (do16Data>=1){
        if (TrainNumber_Data.CompareTo("") == 0){if (TrainNumberSave_Data.CompareTo("") == 0){TrainNumber_Data=TrainNumber16Data;} else {TrainNumber_Data=TrainNumberSave_Data;}}
        mergeStart[nMerge_Current]          = nSets_Current;
        for (Int_t currentSet=0; currentSet<nSets_16Data; currentSet++){
            DataSets[nSets_Current]         = DataSets_16Data[currentSet];
            runlist[nSets_Current]          = runlist_16Data[currentSet];
            trainRuns[nSets_Current]        = TrainNumber16Data.Atoi();
            nSets_Current++;
        }
        mergeEnd[nMerge_Current]            = nSets_Current-1;
        strMerge[nMerge_Current]            = "LHC16_pass1_PHOS_merged";
        nMerge_Current++;
    }
    if (do17Data>=1){
        if (TrainNumber_Data.CompareTo("") == 0){if (TrainNumberSave_Data.CompareTo("") == 0){TrainNumber_Data=TrainNumber17Data;} else {TrainNumber_Data=TrainNumberSave_Data;}}
        mergeStart[nMerge_Current]          = nSets_Current;
        for (Int_t currentSet=0; currentSet<nSets_17Data; currentSet++){
            DataSets[nSets_Current]         = DataSets_17Data[currentSet];
            runlist[nSets_Current]          = runlist_17Data[currentSet];
            trainRuns[nSets_Current]        = TrainNumber17Data.Atoi();
            nSets_Current++;
        }
        mergeEnd[nMerge_Current]            = nSets_Current-1;
        strMerge[nMerge_Current]            = "LHC17_PHOS_merged";
        nMerge_Current++;
    }
    if (do18Data>=1){
        if (TrainNumber_Data.CompareTo("") == 0){if (TrainNumberSave_Data.CompareTo("") == 0){TrainNumber_Data=TrainNumber18Data;} else {TrainNumber_Data=TrainNumberSave_Data;}}
        mergeStart[nMerge_Current]          = nSets_Current;
        for (Int_t currentSet=0; currentSet<nSets_18Data; currentSet++){
            DataSets[nSets_Current]         = DataSets_18Data[currentSet];
            runlist[nSets_Current]          = runlist_18Data[currentSet];
            trainRuns[nSets_Current]        = TrainNumber18Data.Atoi();
            nSets_Current++;
        }
        mergeEnd[nMerge_Current]            = nSets_Current-1;
        strMerge[nMerge_Current]            = "LHC18_PHOS_merged";
        nMerge_Current++;
    }
    if ((do16Data>=1)||(do17Data>=1)||(do18Data>=1)){
        Current_Merge_DataAll_Start         = 0;
        Current_Merge_DataAll_End           = nData_const-1;
        mergeStart[nMerge_Current]          = Current_Merge_DataAll_Start;
        mergeEnd[nMerge_Current]            = Current_Merge_DataAll_End;
        strMerge[nMerge_Current]            = "CombinedYears";
        if (do16Data){strMerge[nMerge_Current] += "LHC16";}
        if (do17Data){strMerge[nMerge_Current] += "LHC17";}
        if (do18Data){strMerge[nMerge_Current] += "LHC18";}
        strMerge[nMerge_Current]            += "_pass1_PHOS_merged";
        nMerge_Current++;
    }
    nData_Merge_nonconst                    = nMerge_Current;
    //------------------------------------------------------------------------------------------------------------------------------
    Current_Merge_MCAll_Start               = nSets_Current-1;
    if (do16MC>=1){
        if (TrainNumber_MC.CompareTo("") == 0){if (TrainNumberSave_MC.CompareTo("") == 0){TrainNumber_MC=TrainNumber16MC;} else {TrainNumber_MC=TrainNumberSave_MC;}}
        mergeStart[nMerge_Current]          = nSets_Current;
        for (Int_t currentSet=0; currentSet<nSets_16MC; currentSet++){
            DataSets[nSets_Current]         = DataSets_16MC[currentSet];
            runlist[nSets_Current]          = runlist_16MC[currentSet];
            trainRuns[nSets_Current]        = TrainNumber16MC.Atoi();
            nSets_Current++;
        }
        Current_Merge_16MC_End              = nSets_Current-1;
        mergeEnd[nMerge_Current]            = nSets_Current-1;
        strMerge[nMerge_Current]            = "LHC16anch_PHOS_merged";
        nMerge_Current++;
    }
    if (do16MCextra>=1){
        if (TrainNumber_MC.CompareTo("") == 0){if (TrainNumberSave_MC.CompareTo("") == 0){TrainNumber_MC=TrainNumber16MCextra;} else {TrainNumber_MC=TrainNumberSave_MC;}}
        mergeStart[nMerge_Current]          = nSets_Current;
        for (Int_t currentSet=0; currentSet<nSets_16MCextra; currentSet++){
            DataSets[nSets_Current]         = DataSets_16MCextra[currentSet];
            runlist[nSets_Current]          = runlist_16MCextra[currentSet];
            trainRuns[nSets_Current]        = TrainNumber16MCextra.Atoi();
            nSets_Current++;
        }
        Current_Merge_16MC_End              = nSets_Current-1;
        mergeEnd[nMerge_Current]            = nSets_Current-1;
        strMerge[nMerge_Current]            = "LHC16anchExtra_PHOS_merged";
        nMerge_Current++;
    }
    if (do16MCextra2>=1){
        if (TrainNumber_MC.CompareTo("") == 0){if (TrainNumberSave_MC.CompareTo("") == 0){TrainNumber_MC=TrainNumber16MCextra2;} else {TrainNumber_MC=TrainNumberSave_MC;}}
        mergeStart[nMerge_Current]          = nSets_Current;
        for (Int_t currentSet=0; currentSet<nSets_16MCextra2; currentSet++){
            DataSets[nSets_Current]         = DataSets_16MCextra2[currentSet];
            runlist[nSets_Current]          = runlist_16MCextra2[currentSet];
            trainRuns[nSets_Current]        = TrainNumber16MCextra2.Atoi();
            nSets_Current++;
        }
        Current_Merge_16MC_End              = nSets_Current-1;
        mergeEnd[nMerge_Current]            = nSets_Current-1;
        strMerge[nMerge_Current]            = "LHC16anchExtra2_PHOS_merged";
        nMerge_Current++;
    }
    if ((do16MC>=1)||(do16MCextra>=1)||(do16MCextra2>=1)){
        Current_Merge_16MC_Start            = nData_const;
        mergeStart[nMerge_Current]          = Current_Merge_16MC_Start;
        mergeEnd[nMerge_Current]            = Current_Merge_16MC_End;
        strMerge[nMerge_Current]            = "LHC16anchWExtras_PHOS_merged";
        nMerge_Current++;
    }
    //---------------------------------------------------------------
    if (do17MC>=1){
        if (TrainNumber_MC.CompareTo("") == 0){if (TrainNumberSave_MC.CompareTo("") == 0){TrainNumber_MC=TrainNumber17MC;} else {TrainNumber_MC=TrainNumberSave_MC;}}
        mergeStart[nMerge_Current]          = nSets_Current;
        for (Int_t currentSet=0; currentSet<nSets_17MC; currentSet++){
            DataSets[nSets_Current]         =DataSets_17MC[currentSet];
            runlist[nSets_Current]          =runlist_17MC[currentSet];
            trainRuns[nSets_Current]        =TrainNumber17MC.Atoi();
            nSets_Current++;
        }
        Current_Merge_17MC_End              = nSets_Current-1;
        mergeEnd[nMerge_Current]            = nSets_Current-1;
        strMerge[nMerge_Current]            = "LHC17anch_PHOS_merged";
        nMerge_Current++;
    }
    if (do17MCextra>=1){
        if (TrainNumber_MC.CompareTo("") == 0){if (TrainNumberSave_MC.CompareTo("") == 0){TrainNumber_MC=TrainNumber17MCextra;} else {TrainNumber_MC=TrainNumberSave_MC;}}
        mergeStart[nMerge_Current]          = nSets_Current;
        for (Int_t currentSet=0; currentSet<nSets_17MCextra; currentSet++){
            DataSets[nSets_Current]         = DataSets_17MCextra[currentSet];
            runlist[nSets_Current]          = runlist_17MCextra[currentSet];
            trainRuns[nSets_Current]        = TrainNumber17MCextra.Atoi();
            nSets_Current++;
        }
        Current_Merge_17MC_End              = nSets_Current-1;
        mergeEnd[nMerge_Current]            = nSets_Current-1;
        strMerge[nMerge_Current]            = "LHC17anchExtra_PHOS_merged";
        nMerge_Current++;
    }
    if ((do17MC>=1)||(do17MCextra>=1)){
        if ((do16MC>=1)||(do16MCextra>=1)||(do16MCextra2>=1)){
            Current_Merge_17MC_Start            = Current_Merge_16MC_End+1;
        } else {
            Current_Merge_17MC_Start            = nData_const;
        }
        mergeStart[nMerge_Current]          = Current_Merge_17MC_Start;
        mergeEnd[nMerge_Current]            = Current_Merge_17MC_End;
        strMerge[nMerge_Current]            = "LHC17anchWExtras_PHOS_merged";
        nMerge_Current++;
    }
    //---------------------------------------------------------------
    if (do18MC>=1){
        if (TrainNumber_MC.CompareTo("") == 0){if (TrainNumberSave_MC.CompareTo("") == 0){TrainNumber_MC=TrainNumber18MC;} else {TrainNumber_MC=TrainNumberSave_MC;}}
        mergeStart[nMerge_Current]          = nSets_Current;
        for (Int_t currentSet=0; currentSet<nSets_18MC; currentSet++){
            DataSets[nSets_Current]         = DataSets_18MC[currentSet];
            runlist[nSets_Current]          = runlist_18MC[currentSet];
            trainRuns[nSets_Current]        = TrainNumber18MC.Atoi();
            nSets_Current++;
        }
        Current_Merge_18MC_End              = nSets_Current-1;
        mergeEnd[nMerge_Current]            = nSets_Current-1;
        strMerge[nMerge_Current]            = "LHC18anch_PHOS_merged";
        nMerge_Current++;
    }
    if (do18MCextra>=1){
        if (TrainNumber_MC.CompareTo("") == 0){if (TrainNumberSave_MC.CompareTo("") == 0){TrainNumber_MC=TrainNumber18MCextra;} else {TrainNumber_MC=TrainNumberSave_MC;}}
        mergeStart[nMerge_Current]          = nSets_Current;
        for (Int_t currentSet=0; currentSet<nSets_18MCextra; currentSet++){
            DataSets[nSets_Current]         = DataSets_18MCextra[currentSet];
            runlist[nSets_Current]          = runlist_18MCextra[currentSet];
            trainRuns[nSets_Current]        = TrainNumber18MCextra.Atoi();
            nSets_Current++;
        }
        Current_Merge_18MC_End              = nSets_Current-1;
        mergeEnd[nMerge_Current]            = nSets_Current-1;
        strMerge[nMerge_Current]            = "LHC18anchExtra_PHOS_merged";
        nMerge_Current++;
    }
    if ((do18MC>=1)||(do18MCextra>=1)){
        if ((do17MC>=1)||(do17MCextra>=1)){
                    Current_Merge_18MC_Start        = Current_Merge_17MC_End+1;
        }
        else if ((do16MC>=1)||(do16MCextra>=1)||(do16MCextra2>=1)){
            Current_Merge_18MC_Start        = Current_Merge_16MC_End+1;
        } else {
            Current_Merge_18MC_Start        = nData_const;
        }
        mergeStart[nMerge_Current]          = Current_Merge_18MC_Start;
        mergeEnd[nMerge_Current]            = Current_Merge_18MC_End;
        strMerge[nMerge_Current]            = "LHC18anchWExtras_PHOS_merged";
        nMerge_Current++;
    }
    //---------------------------------------------------------------
    if ((do16MC>=1)||(do16MCextra>=1)||(do16MCextra2>=1)||(do17MC>=1)||(do17MCextra>=1)||(do18MC>=1)||(do18MCextra>=1)){
        Current_Merge_MCAll_End             = mergeEnd[nMerge_Current-1];
        mergeStart[nMerge_Current]          = Current_Merge_MCAll_Start;
        mergeEnd[nMerge_Current]            = Current_Merge_MCAll_End;
        strMerge[nMerge_Current]            = "CombinedYears";
        if ((do16MC>=1)||(do16MCextra>=1)||(do16MCextra2>=1)){
            strMerge[nMerge_Current]        += "LHC16anch";
        }
        if ((do17MC>=1)||(do17MCextra>=1)){
            strMerge[nMerge_Current]        += "LHC17anch";
        }
        if ((do18MC>=1)||(do18MCextra>=1)){
            strMerge[nMerge_Current]        += "LHC18anch";
        }
        strMerge[nMerge_Current]            += "_PHOS_merged";
        nMerge_Current++;
    }
    //==============================================================================================================================
    train_Data                              = Form("%s%s_%s", TrainTag_Data.Data(), train_FolderAddition_Data.Data(), TrainNumber_Data.Data());
    train_MC                                = Form("%s%s_%s", TrainTag_MC.Data(), train_FolderAddition_MC.Data(), TrainNumber_MC.Data());
    std::vector<Int_t> mergeVec[nMerge_const];
    std::vector<Int_t>::iterator it;
    for(Int_t i=0; i<nSets_const; i++){
        for (Int_t iMergeVectors=0; iMergeVectors<nMerge_const; iMergeVectors++){
            if(mergeStart[iMergeVectors]<=i && i<=mergeEnd[iMergeVectors]) mergeVec[iMergeVectors].push_back(i);
        }
    }
    //---------------------------------------------------------------------------------------------------

    TString alienFolder;
    TString alienFolder_MC;

    if(type.CompareTo("ESD")==0){
      alienFolder                           = Form("/alice/cern.ch/user/a/alitrain/PWGGA/GA_%s/",system.Data());
      alienFolder_MC                        = Form("/alice/cern.ch/user/a/alitrain/PWGGA/GA_%s_MC/",system.Data());
    }else{
      alienFolder                           = Form("/alice/cern.ch/user/a/alitrain/PWGGA/GA_%s_AOD/",system.Data());
      alienFolder_MC                        = Form("/alice/cern.ch/user/a/alitrain/PWGGA/GA_%s_MC_AOD/",system.Data());
    }
    gSystem->Exec(Form("alien_ls %s > tempData.log", alienFolder.Data()));
    gSystem->Exec(Form("alien_ls %s > tempMC.log", alienFolder_MC.Data()));

    TString strTrain[nSets_const];
    std::vector<TString> vecStrTrain;
    std::vector<TString> vecStrTrainMC;
    if(!readin("tempData.log", vecStrTrain)) cout << "\n\n\n**********************ERROR!**********************\n\n\n" << endl;
    if(!readin("tempMC.log", vecStrTrainMC)) cout << "\n\n\n**********************ERROR!**********************\n\n\n" << endl;

    for(Int_t i=0; i<nSets_const; i++){
      TString temp;
      TString tempRuns;
      if(i<nData_const){
        for(Int_t j=0; j<(Int_t)vecStrTrain.size(); j++){
          tempRuns = Form("%i",trainRuns[i]);
          temp = vecStrTrain.at(j);
          if(temp.BeginsWith(tempRuns)){
            if(temp.Contains("_child_")) temp.Remove(temp.Length()-8,8);
            if(temp.EndsWith("/")){temp.Remove(temp.Length()-1,1);}  //Added this; maybe a bug anywhere?
            if(temp.EndsWith("_")){temp.Remove(temp.Length()-1,1);}  //Added this; maybe a bug anywhere?
            strTrain[i] = temp;
            break;
          }
        }
      }else{
        for(Int_t j=0; j<(Int_t)vecStrTrainMC.size(); j++){
          tempRuns = Form("%i",trainRuns[i]);
          temp = vecStrTrainMC.at(j);
          if(temp.BeginsWith(tempRuns)){
            if(temp.Contains("_child_")) temp.Remove(temp.Length()-8,8);
            if(temp.EndsWith("/")){temp.Remove(temp.Length()-1,1);}  //Added this; maybe a bug anywhere?
            if(temp.EndsWith("_")){temp.Remove(temp.Length()-1,1);}  //Added this; maybe a bug anywhere?
            strTrain[i] = temp;
            break;
          }
        }
      }
    }

    Int_t nErr[nSets_const];
    std::vector<TString> vecErrorsFileNames[nSets_const];
    TString fPathGrid;
    TString fPathLocal_Data;
    TString fPathLocal_MC;
    TString fPathTemp;

    fPathLocal_Data                         = Form("%s/%s", folder.Data(), train_Data.Data());
    fPathLocal_MC                           = Form("%s/%s", folder.Data(), train_MC.Data());
    if ((do16Data>=1)||(do17Data>=1)||(do18Data>=1)){
        gSystem->Exec(Form("mkdir -p %s",fPathLocal_Data.Data()));}
    if ((do16MC>=1)||(do16MCextra>=1)||(do16MCextra2>=1)||(do17MC>=1)||(do17MCextra>=1)||(do18MC>=1)||(do18MCextra>=1)){
        gSystem->Exec(Form("mkdir -p %s",fPathLocal_MC.Data()));
    }

    TString mergePeriod[nMerge_const][nFiles];
    TString mergeP;
    for(Int_t iFiles=0; iFiles<nFiles; iFiles++){
      for(Int_t iM=0; iM<nMerge_const; iM++){
          if (iM<nData_Merge_nonconst){
              mergeP                        = Form("%s/%s_%s.root", fPathLocal_Data.Data(), strMerge[iM].Data(), Files[iFiles].Data());
          } else {
              mergeP                        = Form("%s/%s_%s.root", fPathLocal_MC.Data(), strMerge[iM].Data(), Files[iFiles].Data());
          }

        mergePeriod[iM][iFiles]             = Form("hadd -f -k %s",mergeP.Data());
      }
    }

    for(Int_t i=0; i<nSets_const; i++)
    {
        nErr[i]=0;
        vecErrorsFileNames[i].clear();
        for(Int_t k=0; k<nFiles; k++)
        {
          if(Files[k].BeginsWith("OmegaToPiZeroGamma_")){
            if(i<nData_const) Files[k].Append("_0");
            else Files[k].Append("_1");
          }
          //cout<<"Test Output!"<<endl<<"    alienFolder: "<<alienFolder.Data()<<endl<<"    strTrain[i]: "<<strTrain[i].Data()<<endl<<"    runlist[i]: "<<runlist[i].Data()<<endl;
          if(runlist[i].Contains("/")){
            if(i<nData_const) fPathGrid     = Form("%s%s%s/%s.root", alienFolder.Data(), strTrain[i].Data(), runlist[i].Data(), Files[k].Data());
            else fPathGrid                  = Form("%s%s%s/%s.root", alienFolder_MC.Data(), strTrain[i].Data(), runlist[i].Data(), Files[k].Data());
          }else{
            if(i<nData_const) fPathGrid     = Form("%s%s/%s/%s.root", alienFolder.Data(), strTrain[i].Data(), runlist[i].Data(), Files[k].Data());
            else fPathGrid                  = Form("%s%s/%s/%s.root", alienFolder_MC.Data(), strTrain[i].Data(), runlist[i].Data(), Files[k].Data());
          }

          if(i<nData_const){
              fPathTemp                     = Form("%s/%s", fPathLocal_Data.Data(), strTrain[i].Data());
          } else {
              fPathTemp                     = Form("%s/%s", fPathLocal_MC.Data(), strTrain[i].Data());
          }
          gSystem->Exec(Form("mkdir -p %s",fPathTemp.Data()));

          fPathTemp                         +=Form("/%s_",DataSets[i].Data());
          fPathTemp                         +=Files[k].Data();
          fPathTemp                         +=".root";

          cout << endl;
          cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
          cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
          cout << "Copying from (grid): " << fPathGrid.Data() << endl;
          cout << "Copying to (local): " << fPathTemp.Data() << endl;
          cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
          cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;

          TFile fileCheck(fPathTemp.Data());
          if(!fileCheck.IsZombie()) {
            cout << "\n\t\t>>>>>>>>>>>>>>>>>>Info: ROOT-File |" << fPathTemp.Data() << "| does already exist! Continue...<<<<<<<<<<<<<<\n" << endl;
            for(Int_t iM=0; iM<nMerge_const; iM++){
              it = find( mergeVec[iM].begin(), mergeVec[iM].end(), i);
              if( it!=mergeVec[iM].end() ) mergePeriod[iM][k] += Form(" %s",fPathTemp.Data());
            }
            if(i<nData_const){
                gSystem->Exec(Form("ln -s %s %s/%s_%s.root",fPathTemp.Data(), fPathLocal_Data.Data(), DataSets[i].Data(), Files[k].Data()));
            } else {
                gSystem->Exec(Form("ln -s %s %s/%s_%s.root",fPathTemp.Data(), fPathLocal_MC.Data(), DataSets[i].Data(), Files[k].Data()));
            }
            if(Files[k].BeginsWith("OmegaToPiZeroGamma_")){Files[k].Resize(Files[k].Length()-2);}
            continue;
          }

          if(copyAlien2Local(fPathGrid,fPathTemp)){
            //ChangeStrucToStd(fPathTemp.Data(),fPathTemp.Data(),Files[k].Data());
            for(Int_t iM=0; iM<nMerge_const; iM++){
              it = find( mergeVec[iM].begin(), mergeVec[iM].end(), i);
              if( it!=mergeVec[iM].end() ) mergePeriod[iM][k] += Form(" %s",fPathTemp.Data());
            }
            if(i<nData_const){
                gSystem->Exec(Form("ln -s %s %s/%s_%s.root",fPathTemp.Data(), fPathLocal_Data.Data(), DataSets[i].Data(), Files[k].Data()));
            } else {
                gSystem->Exec(Form("ln -s %s %s/%s_%s.root",fPathTemp.Data(), fPathLocal_MC.Data(), DataSets[i].Data(), Files[k].Data()));
            }
            if(Files[k].BeginsWith("OmegaToPiZeroGamma_")){Files[k].Resize(Files[k].Length()-2);}
            continue;
          }
          else{
            cout << "\n\n\t**********************************************************************************************" << endl;
            cout << "\t**********************************************************************************************" << endl;
            cout << "\t*******************Err: copyAlien2Local()!****************************************************" << endl;
            cout << "\t**********************************************************************************************" << endl;
            cout << "\t**********************************************************************************************\n" << endl;
            nErr[i]++;
            vecErrorsFileNames[i].push_back(Files[k]);
          }
        }
    }

    if (doMerging>=1){
        cout << "\n------------------------------------------------------" << endl;
        cout << "Merging: " << endl;
        cout << "------------------------------------------------------\n" << endl;
    } else if (doShowMergeCommand) {
        cout << "\n------------------------------------------------------" << endl;
        cout << "Merging (not enabled): " << endl;
        cout << "------------------------------------------------------\n" << endl;
    }
    if ((doMerging>=1)||(doShowMergeCommand)) {
        for(Int_t iFiles=0; iFiles<nFiles; iFiles++){
            for(Int_t iM=0; iM<nMerge_const; iM++){
                if (doMerging>=1){cout << "Merging " << strMerge[iM].Data() << " ..." << endl;}
                if (doShowMergeCommand) {
                    if (doMerging>=1){cout << "------------------------------------------------------" << endl;}
                    cout << "Command: "<<mergePeriod[iM][iFiles].Data() << endl;
                    cout << "------------------------------------------------------" << endl;
                }
                if (doMerging>=1){
                    gSystem->Exec(mergePeriod[iM][iFiles].Data());
                    cout << "done!" << endl;
                }
            }
        }
    }
    cout<<"Errors Without Files:"<<endl;
    for(Int_t iSets=0; iSets<nSets_const; iSets++){
        if ((doShowAllErrors>=1)||(nErr[iSets]>=1)){cout << "DataSet: " << DataSets[iSets].Data() << "; runlist: "<< runlist[iSets].Data() << ", number of errors: " << nErr[iSets] << endl;}
    }
    cout<<"========================================================================================================"<<endl;
    cout<<"Errors With Files:"<<endl;
    for(Int_t iSets=0; iSets<nSets_const; iSets++){
        if ((doShowAllErrors>=1)||(nErr[iSets]>=1)){
            cout << "DataSet: " << DataSets[iSets].Data() << "; runlist: "<< runlist[iSets].Data() << ", number of errors: " << nErr[iSets] << endl;
            cout << "          Files: ";
            for(UInt_t iFiles=0; iFiles<vecErrorsFileNames[iSets].size(); iFiles++){
                cout << vecErrorsFileNames[iSets].at(iFiles).Data();
                if (iFiles != vecErrorsFileNames[iSets].size()-1) {cout<< ", ";}
            }
            cout<<endl;
        }
    }

    gSystem->Exec("rm tempData.log");
    gSystem->Exec("rm tempMC.log");

    return;
}

Bool_t copyAlien2Local(TString loc, TString rem){
   TString sl(Form("alien://%s", loc.Data()));
   TString sr(Form("file://%s", rem.Data()));
   Bool_t ret = TFile::Cp(sl,sr);
   if (!ret) cout << Form("ERROR: Failed to copy %s to %s", sl.Data(), sr.Data()) << endl;
   return ret;
}
