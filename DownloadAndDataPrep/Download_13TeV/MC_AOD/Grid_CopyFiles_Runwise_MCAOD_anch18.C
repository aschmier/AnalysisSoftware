/*******************************************************************************
 ******  provided by Gamma Conversion Group, PWGGA,                        *****
 ******     Daniel Muehlheim, d.muehlheim@cern.ch                          *****
 *******************************************************************************/

#include <iostream>
#include <vector>
#include <fstream>

#include <TFile.h>
#include <TGrid.h>
#include <TSystem.h>
#include <TString.h>
#include <TObject.h>

using namespace std;

Bool_t readin(TString fileTxt, std::vector<TString> &vec);
Bool_t copyAlien2Local(TString loc, TString rem);

void Grid_CopyFiles_Runwise_MCAOD_anch18(TString folder = "/home/jens/Programme_und_Skripte/alice-pcg/AnalysisSoftware", Int_t TrainNumberChoiceArgument=1, Int_t doUnzipArgument=1, Bool_t isJetJet=kFALSE)
{
    Int_t iClearScreen=0;
    if (iClearScreen >= 1){gSystem->Exec(Form("printf \"\033c\""));}
    Int_t doUnzip=doUnzipArgument;
    Int_t TrainNumberChoice=TrainNumberChoiceArgument;
    TString TrainNumber;
    TString TrainTag;
    TString TrainStart;
    TString AODString="/AOD209";
    if (TrainNumberChoice == 1){
        TrainNumber=Form("2278");
        TrainTag=Form("20200207");
        TrainStart=Form("20200208-2233");
    } else if (TrainNumberChoice == 2){
        TrainNumber=Form("2291");
        TrainTag=Form("20200209");
        TrainStart=Form("20200211-0947");
    } else if (TrainNumberChoice == 3){
        TrainNumber=Form("2330");
        TrainTag=Form("20200214");
        TrainStart=Form("20200217-1829");
    } else if (TrainNumberChoice == 4){
        TrainNumber=Form("2388");
        TrainTag=Form("20200305");
        TrainStart=Form("20200306-0810");
    } else if (TrainNumberChoice == 6){
        TrainNumber=Form("2472");
        TrainTag=Form("20200319");
        TrainStart=Form("20200320-1435");
    } else if (TrainNumberChoice == 7){
        TrainNumber=Form("2478");
        TrainTag=Form("20200319");
        TrainStart=Form("20200320-1643");
    } else if (TrainNumberChoice == 12){
        TrainNumber=Form("2596");
        TrainTag=Form("20200422");
        TrainStart=Form("20200424-1355");
    } else if (TrainNumberChoice == 13){
        TrainNumber=Form("2742");
        TrainTag=Form("20200512");
        TrainStart=Form("20200512-1321");
        AODString="/AOD235";
    } else if (TrainNumberChoice == 16){
        TrainNumber=Form("2787");
        TrainTag=Form("20200619");
        TrainStart=Form("20200619-1928");
    } else {
        cout<<"No Correct TrainNumberChoice("<<TrainNumberChoice<<"); Returning!"<<endl;
        return;
    }
    cout<<"Connecting to Alien..."<<endl;
    TGrid::Connect("alien://");
    cout<<"==============================="<<endl;
    cout<<"Successfully connected to Alien"<<endl;
    cout<<"==============================="<<endl;
    ofstream TerminalOutputTextFile;
    //TString folder1="/home/jens/Daten/GridDownload";
    TString folder1="/media/jens/Maxtor/Promotion/DatenFuerAnalyse/DanielsSkript";

    const Int_t nFiles = 1;//Number of Files to Download, for example GammaConvV1 and CammaCalo
    TString Tag = Form("%s_MCAOD_%s", TrainTag.Data(), TrainNumber.Data());
    TString DataSetsFile[nFiles] = {"root_archive.zip"};
    const Int_t nSets = 10;
    /*TString DataSets[nSets]={
        "LHC18g4_DPGTrack",
        "LHC18g5_DPGTrack",
        "LHC18g6_DPGTrack",
        "LHC18h2_DPGTrack",
        "LHC18h4_DPGTrack",
        "LHC18j1_DPGTrack",
        "LHC18j4_DPGTrack",
        "LHC18k1_DPGTrack",
        "LHC18k2_DPGTrack",
        "LHC18k3_DPGTrack"
    };*/
    TString DataSets[nSets]={
        "LHC18g4_DPGTrackIncAccTPC",
        "LHC18g5_DPGTrackIncAccTPC",
        "LHC18g6_DPGTrackIncAccTPC",
        "LHC18h2_DPGTrackIncAccTPC",
        "LHC18h4_DPGTrackIncAccTPC",
        "LHC18j1_DPGTrackIncAccTPC",
        "LHC18j4_DPGTrackIncAccTPC",
        "LHC18k1_DPGTrackIncAccTPC",
        "LHC18k2_DPGTrackIncAccTPC",
        "LHC18k3_DPGTrackIncAccTPC"
    };
    TString DataSetsFolder[nSets]={
        "LHC18g4",
        "LHC18g5",
        "LHC18g6",
        "LHC18h2",
        "LHC18h4",
        "LHC18j1",
        "LHC18j4",
        "LHC18k1",
        "LHC18k2",
        "LHC18k3"
    };
    TString PrefixDataSets[nSets]={
        "/alice/sim/2018/LHC18g4/",
        "/alice/sim/2018/LHC18g5/",
        "/alice/sim/2018/LHC18g6/",
        "/alice/sim/2018/LHC18h2/",
        "/alice/sim/2018/LHC18h4/",
        "/alice/sim/2018/LHC18j1/",
        "/alice/sim/2018/LHC18j4/",
        "/alice/sim/2018/LHC18k1/",
        "/alice/sim/2018/LHC18k2/",
        "/alice/sim/2018/LHC18k3/"
    };
    TString SuffixDataSets[nSets]={
        Form("%s/PWGGA/GA_pp_MC_AOD/%s_%s_child_%d/", AODString.Data(), TrainNumber.Data(), TrainStart.Data(), 1),
        Form("%s/PWGGA/GA_pp_MC_AOD/%s_%s_child_%d/", AODString.Data(), TrainNumber.Data(), TrainStart.Data(), 2),
        Form("%s/PWGGA/GA_pp_MC_AOD/%s_%s_child_%d/", AODString.Data(), TrainNumber.Data(), TrainStart.Data(), 3),
        Form("%s/PWGGA/GA_pp_MC_AOD/%s_%s_child_%d/", AODString.Data(), TrainNumber.Data(), TrainStart.Data(), 4),
        Form("%s/PWGGA/GA_pp_MC_AOD/%s_%s_child_%d/", AODString.Data(), TrainNumber.Data(), TrainStart.Data(), 5),
        Form("%s/PWGGA/GA_pp_MC_AOD/%s_%s_child_%d/", AODString.Data(), TrainNumber.Data(), TrainStart.Data(), 6),
        Form("%s/PWGGA/GA_pp_MC_AOD/%s_%s_child_%d/", AODString.Data(), TrainNumber.Data(), TrainStart.Data(), 7),
        Form("%s/PWGGA/GA_pp_MC_AOD/%s_%s_child_%d/", AODString.Data(), TrainNumber.Data(), TrainStart.Data(), 8),
        Form("%s/PWGGA/GA_pp_MC_AOD/%s_%s_child_%d/", AODString.Data(), TrainNumber.Data(), TrainStart.Data(), 9),
        Form("%s/PWGGA/GA_pp_MC_AOD/%s_%s_child_%d/", AODString.Data(), TrainNumber.Data(), TrainStart.Data(), 10)
    };

    std::vector<TString> vecRuns;
    std::vector<TString> vecBins;
    std::vector<TString> vecErrors[nSets];
    std::vector<TString> vecErrorsFileNames[nSets];
    TString fDataSet;
    TString fPathGrid;
    TString fPathLocal;
    TString fPathLocalwOFilename;
    TString fileTxt;

    Int_t nErr[nSets];
    Int_t nErrRuns[nSets];
    Int_t nRuns[nSets];

    Int_t nSetsStart=0;
    Int_t nSetsEnd=nSets;
    TerminalOutputTextFile.open(Form("%s/DataQA/%s/TerminalOutput.txt", folder1.Data(), Tag.Data()), ios::out);
    for(Int_t i=nSetsStart; i<nSetsEnd; i++)
    {
        nErr[i]=0;
        nErrRuns[i]=0;
        nRuns[i]=0;
        vecErrors[i].clear();
        vecErrorsFileNames[i].clear();
        vecRuns.clear();
        vecBins.clear();
        fDataSet = DataSets[i];
        if(fDataSet.CompareTo("")==0) continue;
        fileTxt = Form("%s/DownloadAndDataPrep/runlists/runNumbers%s.txt", folder.Data(), fDataSet.Data());
        cout << "\n------------------------------------------------------" << endl;
        if(!readin(fileTxt, vecRuns)) cout << "\n\n\n**********************ERROR, no Run Numbers could be found!**********************\n\n\n" << endl;
        cout << "------------------------------------------------------" << endl;
        if(isJetJet){
            fileTxt = Form("%s/DownloadAndDataPrep/binsJetJet%s.txt", folder.Data(), fDataSet.Data());
            cout << "\n------------------------------------------------------" << endl;
            if(!readin(fileTxt, vecBins)) cout << "\n\n\n**********************ERROR, no Jet Jet Bins could be found!**********************\n\n\n" << endl;
            cout << "------------------------------------------------------" << endl;
        }

        Int_t doNormalFolder = 2;
        if(DataSetsFolder[i].IsNull()) doNormalFolder = kTRUE;
        nRuns[i]=(Int_t)vecRuns.size();

        for(Int_t j=0; j<(Int_t)vecRuns.size(); j++)
        {
            // exclude files from unavailable SEs
            //if(fDataSet.CompareTo("LHC12g")==0 && vecRuns.at(j).CompareTo("188443")==0) continue;

            Int_t nBins = 1;
            if(isJetJet) nBins = (Int_t)vecBins.size();
            for(Int_t b=0; b<nBins; b++)
            {
                for(Int_t k=0; k<nFiles; k++)
                {
                    if(isJetJet){
                        fPathGrid = Form("%s%s/%s%s%s", PrefixDataSets[i].Data(), vecBins.at(b).Data(), vecRuns.at(j).Data(), SuffixDataSets[i].Data(), DataSetsFile[k].Data());
                        if(doNormalFolder) fPathLocal = Form("%s/DataQA/%s/%s/%s/%s", folder.Data(), Tag.Data(), fDataSet.Data(), vecBins.at(b).Data(), vecRuns.at(j).Data());
                        else fPathLocal = Form("%s/DataQA/%s/%s/%s/%s", folder.Data(), Tag.Data(), DataSetsFolder[i].Data(), vecBins.at(b).Data(), vecRuns.at(j).Data());
                        gSystem->Exec(Form("mkdir -p %s",fPathLocal.Data()));
                    }else{
                        fPathGrid = Form("%s%s%s%s", PrefixDataSets[i].Data(), vecRuns.at(j).Data(), SuffixDataSets[i].Data(), DataSetsFile[k].Data());
                        if(doNormalFolder==0) fPathLocal = Form("%s/DataQA/%s/%s/%s", folder.Data(), Tag.Data(), fDataSet.Data(), vecRuns.at(j).Data());
                        else if (doNormalFolder==1) fPathLocal = Form("%s/DataQA/%s/%s/%s", folder.Data(), Tag.Data(), DataSetsFolder[i].Data(), vecRuns.at(j).Data());
                        else if (doNormalFolder==2) fPathLocal = Form("%s/DataQA/%s/%s/%s", folder1.Data(), Tag.Data(), DataSetsFolder[i].Data(), vecRuns.at(j).Data());
                        gSystem->Exec(Form("mkdir -p %s",fPathLocal.Data()));
                    }

                    fPathLocalwOFilename=fPathLocal.Data();
                    fPathLocal+="/"; fPathLocal+=DataSetsFile[k];

                    cout << endl;
                    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
                    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
                    cout << "Copying from (grid): " << fPathGrid.Data() << endl;
                    cout << "Copying to (local): " << fPathLocal.Data() << endl;
                    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
                    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;

                    TFile fileCheck(fPathLocal.Data());
                    if(!fileCheck.IsZombie()) {
                        cout << "\n\t\t>>>>>>>>>>>>>>>>>>Info: ROOT-File |" << fPathLocal.Data() << "| does already exist! Continue...<<<<<<<<<<<<<<\n" << endl;
                        if ( doUnzip >= 2){
                            gSystem->Exec(Form("unzip -uoqq %s -d %s", fPathLocal.Data(), fPathLocalwOFilename.Data()));
                        }
                        continue;
                    }

                    if(copyAlien2Local(fPathGrid,fPathLocal)){
                        if ( doUnzip >= 1){
                            gSystem->Exec(Form("unzip -uoqq %s -d %s", fPathLocal.Data(), fPathLocalwOFilename.Data()));
                        }
                        continue;
                    }
                    else{
                        cout << "\n\n\t**********************************************************************************************" << endl;
                        cout << "\t**********************************************************************************************" << endl;
                        cout << "\t*******************Err: copyAlien2Local(), check runlist in photonconv rep!*******************" << endl;
                        cout << "\t**********************************************************************************************" << endl;
                        cout << "\t**********************************************************************************************\n" << endl;
                        nErr[i]++;
                        vecErrors[i].push_back(vecRuns.at(j));
                        vecErrorsFileNames[i].push_back(DataSetsFile[k]);
                    }
                }
            }
        }

        if(!doNormalFolder) gSystem->Exec(Form("ln -s %s %s",
                                               Form("%s/DataQA/%s/%s", folder.Data(), Tag.Data(), DataSetsFolder[i].Data()),
                                               Form("%s/DataQA/%s/%s", folder.Data(), Tag.Data(), fDataSet.Data())));
    }


    cout<<"========================================================================================================"<<endl;
    TerminalOutputTextFile<<"========================================================================================================"<<endl;
    for(Int_t i=nSetsStart; i<nSetsEnd; i++){
        if (nErr[i]!=0){
            for(Int_t iRuns=0; iRuns < (Int_t)vecErrors[i].size(); iRuns++) {
                if (iRuns==0){
                    nErrRuns[i]++;

                }
                else if ( (vecErrors[i].at(iRuns).CompareTo(vecErrors[i].at(iRuns-1).Data()))!=0 ){
                    nErrRuns[i]++;
                }
            }
            cout << endl;
            cout << endl;
         }
    }
    for(Int_t i=nSetsStart; i<nSetsEnd; i++){
        cout << "Without Files; DataSet: " << DataSets[i].Data() << ", number of errors: " << nErr[i] <<" in "<<nErrRuns[i]<<" runs out of "<<nRuns[i]<< endl;
        TerminalOutputTextFile<< "Without Files; DataSet: " << DataSets[i].Data() << ", number of errors: " << nErr[i] <<" in "<<nErrRuns[i]<<" runs out of "<<nRuns[i]<< endl;
        if (nErr[i]!=0){
            cout << "\t\tRuns:\t";
            TerminalOutputTextFile << "\t\tRuns:\t";
            for(Int_t iRuns=0; iRuns < (Int_t)vecErrors[i].size(); iRuns++) {
                if (iRuns==0){
                    cout << vecErrors[i].at(iRuns).Data() << ", ";
                    TerminalOutputTextFile << vecErrors[i].at(iRuns).Data() << ", ";

                }
                else if ( (vecErrors[i].at(iRuns).CompareTo(vecErrors[i].at(iRuns-1).Data()))!=0 ){
                    cout << vecErrors[i].at(iRuns).Data() << ", ";
                    TerminalOutputTextFile << vecErrors[i].at(iRuns).Data() << ", ";
                }
            }
            cout << endl <<"--------------------------------------------------------------------------------------------------------"<< endl;
            TerminalOutputTextFile << endl <<"--------------------------------------------------------------------------------------------------------"<< endl;
         }
    }
    cout<<"========================================================================================================"<<endl;
    cout<<"========================================================================================================"<<endl;
    TerminalOutputTextFile<<"========================================================================================================"<<endl;
    TerminalOutputTextFile<<"========================================================================================================"<<endl;
    cout<<"Errors != 0, With Files:"<<endl;
    TerminalOutputTextFile<<"Errors != 0, With Files:"<<endl;
    for(Int_t i=nSetsStart; i<nSetsEnd; i++){
        if (nErr[i]!=0){
            cout << "With Files; DataSet: " << DataSets[i].Data() << ", number of errors: " << nErr[i] <<" in "<<nErrRuns[i]<<" runs out of "<<nRuns[i]<< endl;
            cout << "\t\tRuns:\t";
            TerminalOutputTextFile << "With Files; DataSet: " << DataSets[i].Data() << ", number of errors: " << nErr[i] <<" in "<<nErrRuns[i]<<" runs out of "<<nRuns[i]<< endl;
            TerminalOutputTextFile << "\t\tRuns:\t";
            for(Int_t iRuns=0; iRuns < (Int_t)vecErrors[i].size(); iRuns++) {
                if (iRuns==0){
                    cout << vecErrors[i].at(iRuns).Data() << ": ";
                    cout << vecErrorsFileNames[i].at(iRuns).Data() << ", ";
                    TerminalOutputTextFile << vecErrors[i].at(iRuns).Data() << ": ";
                    TerminalOutputTextFile << vecErrorsFileNames[i].at(iRuns).Data() << ", ";

                }
                else if ( (vecErrors[i].at(iRuns).CompareTo(vecErrors[i].at(iRuns-1).Data()))!=0 ){
                    cout <<endl<<"\t\t\t"<<vecErrors[i].at(iRuns).Data() << ", ";
                    cout << vecErrorsFileNames[i].at(iRuns).Data() << ", ";
                    TerminalOutputTextFile <<endl<<"\t\t\t"<<vecErrors[i].at(iRuns).Data() << ", ";
                    TerminalOutputTextFile << vecErrorsFileNames[i].at(iRuns).Data() << ", ";
                }
                else {
                    cout << vecErrorsFileNames[i].at(iRuns).Data() << ", ";
                    TerminalOutputTextFile << vecErrorsFileNames[i].at(iRuns).Data() << ", ";
                }
            }
            cout << endl  <<"--------------------------------------------------------------------------------------------------------"<< endl;
            TerminalOutputTextFile << endl  <<"--------------------------------------------------------------------------------------------------------"<< endl;
        }
    }

    return;
}

Bool_t readin(TString fileTxt, std::vector<TString> &vec){
    cout << Form("Reading from %s...", fileTxt.Data()) << endl;
    fstream file;
    TString fVar;
    Int_t totalN=0;
    file.open(fileTxt.Data(), ios::in);
    if(file.good())
    {
        file.seekg(0L, ios::beg);
        if(fileTxt.Contains("binsJetJet")) cout << "Processing Bins: \"";
        else cout << "Processing Runs: \"";
        while(!file.eof())
        {
            file >> fVar;
            if(fVar.Sizeof()>1)
            {
                cout << fVar.Data() << ", ";
                vec.push_back(fVar);
                totalN++;
            }
        }
        cout << "\"" << endl;
    }
    file.close();
    if(fileTxt.Contains("binsJetJet"))  cout << "...done!\n\nIn total " << totalN << " Bins will be processed!" << endl;
    else cout << "...done!\n\nIn total " << totalN << " Runs will be processed!" << endl;
    if(totalN > 0) return kTRUE;
    else return kFALSE;
}

Bool_t copyAlien2Local(TString loc, TString rem){
    TString sl(Form("alien://%s", loc.Data()));
    TString sr(Form("file://%s", rem.Data()));
    Bool_t ret = TFile::Cp(sl,sr);
    if (!ret) cout << Form("ERROR: Failed to copy %s to %s", sl.Data(), sr.Data()) << endl;
    return ret;
}
