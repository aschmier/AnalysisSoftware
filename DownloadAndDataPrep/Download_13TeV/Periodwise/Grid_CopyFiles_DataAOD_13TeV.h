/*******************************************************************************
 ******  provided by Gamma Conversion Group, PWGGA,                        *****
 ******     Daniel Muehlheim, d.muehlheim@cern.ch                          *****
 ******     adjusted by Jens Luehder, jens.luhder@cern.ch                  *****
 *******************************************************************************/

#include <algorithm>
#include <vector>
#include <iterator>
#include <iostream>
#include <fstream>
#include <TObject.h>
#include <TString.h>
#include <TFile.h>
#include <TGrid.h>
#include <TSystem.h>

using namespace std;

Int_t       TrainNumberChoice           = 0;
Int_t       nSets_nonConst              = 0;
Int_t       nData_nonconst              = 0;
Int_t       nMerge_nonconst             = 0;
Int_t       nData_Merge_nonconst        = 0;
Int_t       nSets_Current               = 0;
Int_t       nMerge_Current              = 0;
Int_t       nFilesVariation             = 0;
Int_t       nFilesVariation_max         = 0;
Int_t       useRunlist                  = 0;
//-
Int_t       Current_Merge_DataAll_Start = 0;
Int_t       Current_Merge_DataAll_End   = 0;
//-
Int_t       Current_Merge_16MC_Start    = 0;
Int_t       Current_Merge_16MC_End      = 0;
Int_t       Current_Merge_17MC_Start    = 0;
Int_t       Current_Merge_17MC_End      = 0;
Int_t       Current_Merge_18MC_Start    = 0;
Int_t       Current_Merge_18MC_End      = 0;
//-
Int_t       Current_Merge_MCAll_Start   = 0;
Int_t       Current_Merge_MCAll_End     = 0;
//---------------------------------------------------------------
Int_t       doMerging                   = 0;
Int_t       doShowMergeCommand          = 0;
Int_t       doShowAllErrors             = 0;
//---------------------------------------------------------------
Int_t       do16Data                    = 0;
Int_t       do17Data                    = 0;
Int_t       do18Data                    = 0;
Int_t       do16MC                      = 0;
Int_t       do16MCextra                 = 0;
Int_t       do16MCextra2                = 0;
Int_t       do17MC                      = 0;
Int_t       do17MCextra                 = 0;
Int_t       do18MC                      = 0;
Int_t       do18MCextra                 = 0;
//---------------------------------------------------------------
const Int_t nSets_16Data                = 11;
const Int_t nSets_17Data                = 11;
const Int_t nSets_18Data                = 14;
const Int_t nSets_16MC                  = 11;
const Int_t nSets_16MCextra             = 10;
const Int_t nSets_16MCextra2            = 8;
const Int_t nSets_17MC                  = 10;
const Int_t nSets_17MCextra             = 10;
const Int_t nSets_18MC                  = 10;
const Int_t nSets_18MCextra             = 10;
//---------------------------------------------------------------
TString     TrainNumberSave_Data        = "";
TString     TrainNumberSave_MC          = "";
TString     TrainNumber16Data           = "";
TString     TrainNumber17Data           = "";
TString     TrainNumber18Data           = "";
TString     TrainNumber16MC             = "";
TString     TrainNumber16MCextra        = "";
TString     TrainNumber16MCextra2       = "";
TString     TrainNumber17MC             = "";
TString     TrainNumber17MCextra        = "";
TString     TrainNumber18MC             = "";
TString     TrainNumber18MCextra        = "";
TString     TrainNumber_Data            = "";
TString     TrainNumber_MC              = "";
TString     TrainTag_Data               = "";
TString     TrainTag_MC                 = "";
TString     train_FolderAddition_Data   = "_DataAOD";
TString     train_FolderAddition_MC     = "_MCAOD";
TString     train_Data                  = "";
TString     train_MC                    = "";
TString     *FilesVariation             = NULL;
//******************************************************************************************************************************
// LHC 16 Data
TString DataSets_16Data[nSets_16Data];
TString DataSets_16Data_PHOS[nSets_16Data]={
   //01 16d                         02 16e                          03 16f                          04 16g                          05 16h
    "LHC16d",                       "LHC16e",                       "LHC16f",                       "LHC16g",                       "LHC16h",
   //06 16i                         07 16j                          08 16k                          09 16l                          10 16o
    "LHC16i",                       "LHC16j",                       "LHC16k",                       "LHC16l",                       "LHC16o",
   //11 16p
    "LHC16p"
};
//---------------------------------------------------------------
TString runlist_16Data[nSets_16Data];
TString runlist_16Data_PHOS[nSets_16Data]{
   //01 16d                         02 16e                          03 16f                          04 16g                          05 16h
    "_child_1/merge_runlist_5",     "_child_2/merge_runlist_5",     "_child_3/merge",               "_child_4/merge_runlist_5",     "_child_5/merge_runlist_5",
   //06 16i                         07 16j                          08 16k                          09 16l                          10 16o
    "_child_6/merge_runlist_5",     "_child_7/merge_runlist_5",     "_child_8/merge_runlist_5",     "_child_9/merge_runlist_5",     "_child_10/merge_runlist_5",
   //11 16p
    "_child_11/merge_runlist_5"
};
//------------------------------------------------------------------------------------------------------------------------------
// LHC 17 Data
TString DataSets_17Data[nSets_17Data];
TString DataSets_17Data_PHOS[nSets_17Data]={
   //01 17c                         02 17e                          03 17f                          04 17h                          05 17i
    "LHC17c",                       "LHC17e",                       "LHC17f",                       "LHC17h",                       "LHC17i",
   //06 17j                         07 17k                          08 17l                          09 17m                          10 17o
    "LHC17j",                       "LHC17k",                       "LHC17l",                       "LHC17m",                       "LHC17o",
   //11 17p
    "LHC17p"
};
//---------------------------------------------------------------
TString runlist_17Data[nSets_17Data];
TString runlist_17Data_PHOS[nSets_17Data]{
   //01 17c                         02 17e                          03 17f                          04 17h                          05 17i
    "_child_1/merge_runlist_5",     "_child_2/merge_runlist_5",     "_child_3/merge_runlist_5",     "_child_4/merge_runlist_5",     "_child_5/merge_runlist_5",
   //06 17i                         07 17j                          08 17k                          09 17l                           10 17o
    "_child_6/merge_runlist_5",     "_child_7/merge_runlist_5",     "_child_8/merge_runlist_5",     "_child_9/merge_runlist_5",     "_child_10/merge_runlist_5",
   //11 17p
    "_child_11/merge_runlist_5"
};
//------------------------------------------------------------------------------------------------------------------------------
// LHC 18 Data
TString DataSets_18Data[nSets_18Data];
TString DataSets_18Data_PHOS[nSets_18Data]={
   //01 18b                         02 18d                          03 18e                          04 18f                          05 18g
    "LHC18b",                       "LHC18d",                       "LHC18e",                       "LHC18f",                       "LHC18g",
   //06 18h                         07 18i                          08 18j                          09 18k                          10 18l
    "LHC18h",                       "LHC18i",                       "LHC18j",                       "LHC18k",                       "LHC18l",
   //11 18m                         12 18n                          13 18o                          14 18p
    "LHC18m",                       "LHC18n",                       "LHC18o",                       "LHC18p"
};
//---------------------------------------------------------------
TString runlist_18Data[nSets_18Data];
TString runlist_18Data_PHOS[nSets_18Data]{
   //01 18b                         02 18d                          03 18e                          04 18f                          05 18g
    "_child_1/merge_runlist_5",     "_child_2/merge_runlist_5",     "_child_3/merge_runlist_5",     "_child_4/merge_runlist_5",     "_child_5/merge_runlist_5",
   //06 18h                         07 18i                          08 18j                          09 18k                          10 18l
    "_child_6/merge",               "_child_7/merge_runlist_5",     "_child_8/merge",               "_child_9/merge_runlist_5",     "_child_10/merge_runlist_5",
   //11 18m                         12 18n                          13 18o                          14 18p
    "_child_11/merge_runlist_5",    "_child_12/merge",              "_child_13/merge_runlist_5",    "_child_14/merge_runlist_5"
};
//==============================================================================================================================
//MC
// LHC 16 MC
TString DataSets_16MC[nSets_16MC];
TString DataSets_16MC_PHOS[nSets_16MC]={
   //01 16d 17f6                    02 16e 17f9                     03 16g 17d17                    04 16h 17f5                     05 16i 17d3
    "LHC17f6",                      "LHC17f9",                      "LHC17d17",                     "LHC17f5",                      "LHC17d3",
   //06 16j 17e5                    07 16o 17d16                    08 16p 17d18                    09 16k 18f1                     10 16l 18d8
    "LHC17e5",                      "LHC17d16",                     "LHC17d18",                     "LHC18f1",                      "LHC18d8",
   //11 16f 17d1
    "LHC17d1"
};
//---------------------------------------------------------------
TString runlist_16MC[nSets_16MC];
TString runlist_16MC_PHOS[nSets_16MC]{
   //01 16d 17f6                    02 16e 17f9                     03 16g 17d17                    04 16h 17f5                     05 16i 17d3
    "_child_1/merge_runlist_7",     "_child_2/merge_runlist_7",     "_child_3/merge_runlist_8",     "_child_4/merge_runlist_8",     "_child_5/merge_runlist_9",
   //06 16j 17e5                    07 16o 17d16                    08 16p 17d18                    09 16k 18f1                     10 16l 18d8
    "_child_6/merge_runlist_9",     "_child_7/merge_runlist_9",     "_child_8/merge_runlist_9",     "_child_9/merge_runlist_9",     "_child_10/merge_runlist_8",
   //11 16f 17d1
    "_child_11"
    "/merge_runlist_8"
};
//==============================================================================================================================
// LHC 16 MC extra
TString DataSets_16MCextra[nSets_16MCextra];
TString DataSets_16MCextra_PHOS[nSets_16MCextra]={
   //01 16d 17f6                    02 16e 17f9                     03 16g 17d17                    04 16h 17f5                     05 16i 17d3
    "LHC17f6_extra",                "LHC17f9_extra",                "LHC17d17_extra",               "LHC17h_extra",                 "LHC17d3_extra",
   //06 16j 17e5                    07 16o 17d16                    08 16p 17d18                    09 16k 18f1                     10 16l 18d8
    "LHC17e5_extra",                "LHC17d16_extra",               "LHC17d18_extra",               "LHC18f1_extra",                "LHC18d8_extra"
};
//---------------------------------------------------------------
TString runlist_16MCextra[nSets_16MCextra];
TString runlist_16MCextra_PHOS[nSets_16MCextra]{
   //01 16d 17f6                    02 16e 17f9                     03 16g 17d17                    04 16h 17f5                     05 16i 17d3
    "_child_1/merge_runlist_8",     "_child_2/merge_runlist_8",     "_child_3/merge_runlist_8",     "_child_4/merge_runlist_8",     "_child_5/merge_runlist_9",
   //06 16j 17e5                    07 16o 17d16                    08 16p 17d18                    09 16k 18f1                     10 16l 18d8
    "_child_6/merge_runlist_9",     "_child_7/merge_runlist_9",     "_child_8/merge_runlist_9",     "_child_9/merge_runlist_9",     "_child_10/merge_runlist_8"
};
//------------------------------------------------------------------------------------------------------------------------------
// LHC 16 MC extra2
TString DataSets_16MCextra2[nSets_16MCextra2];
TString DataSets_16MCextra2_PHOS[nSets_16MCextra2]={
   //01 16d 17f6                    02 16e 17f9                     03 16g 17d17                    04 16h 17f5                     05 16i 17d3
    "LHC17f6_extra2",               "LHC17f9_extra2",               "LHC17d17_extra2",              "LHC17h_extra2",                "LHC17d3_extra2",
   //06 16j 17e5                    07 16o 17d16                    08 16p 17d18
    "LHC17e5_extra2",               "LHC17d16_extra2",              "LHC17d18_extra2"
};
//---------------------------------------------------------------
TString runlist_16MCextra2[nSets_16MCextra2];
TString runlist_16MCextra2_PHOS[nSets_16MCextra2]{
   //01 16d 17f6                    02 16e 17f9                     03 16g 17d17                    04 16h 17f5                     05 16i 17d3
    "_child_1/merge_runlist_8",     "_child_2/merge_runlist_8",     "_child_3/merge_runlist_8",     "_child_4/merge_runlist_8",     "_child_5/merge_runlist_9",
   //06 16j 17e5                    07 16o 17d16                    08 16p 17d18
    "_child_6/merge_runlist_9",     "_child_7/merge_runlist_9",     "_child_8/merge_runlist_9"
};
//==============================================================================================================================
// LHC 17 MC
TString DataSets_17MC[nSets_17MC];
TString DataSets_17MC_PHOS[nSets_17MC]={
   //01 17e 17h1                    02 17c,f 18d3                   03 17h 18c12                    04 16h 17k4                     05 16i 17h11
    "LHC17h1",                      "LHC18d3",                      "LHC18c12",                     "LHC17k4",                      "LHC17h11",
   //06 16j 18c13                   07 16o 18a8                     08 16p 17l5                     09 16k 18a9                     10 16l 18a1
    "LHC18c13",                     "LHC18a8",                      "LHC17l5",                      "LHC18a9",                      "LHC18a1"
};
//---------------------------------------------------------------
TString runlist_17MC[nSets_17MC];
TString runlist_17MC_PHOS[nSets_17MC]={
   //01 17e 17h1                    02 17c,f 18d3                   03 17h 18c12                    04 16h 17k4                     05 16i 17h11
    "_child_1/merge_runlist_9",     "_child_2/merge_runlist_9",     "_child_3/merge_runlist_10",    "_child_4/merge_runlist_10",    "_child_5/merge_runlist_10",
   //06 16j 18c13                   07 16o 18a8                     08 16p 17l5                     09 16k 18a9                     10 16l 18a1
    "_child_6/merge_runlist_10",    "_child_7/merge_runlist_10",    "_child_8/merge_runlist_10",    "_child_9/merge_runlist_10",    "_child_10/merge_runlist_10"
};
//------------------------------------------------------------------------------------------------------------------------------
TString DataSets_17MCextra[nSets_17MCextra];
TString DataSets_17MCextra_PHOS[nSets_17MCextra]={
   //01 17e 17h1                    02 17c,f 18d3                   03 17h 18c12                    04 16h 17k4                     05 16i 17h11
    "LHC17h1_extra",                "LHC18d3_extra",                "LHC18c12_extra",               "LHC17k4_extra",                "LHC17h11_extra",
   //06 16j 18c13                   07 16o 18a8                     08 16p 17l5                     09 16k 18a9                     10 16l 18a1
    "LHC18c13_extra",               "LHC18a8_extra",                "LHC17l5_extra",                "LHC18a9_extra",                "LHC18a1_extra"
};
//---------------------------------------------------------------
TString runlist_17MCextra[nSets_17MCextra];
TString runlist_17MCextra_PHOS[nSets_17MCextra]={
   //01 17e 17h1                    02 17c,f 18d3                   03 17h 18c12                    04 16h 17k4                     05 16i 17h11
    "_child_1/merge_runlist_7",     "_child_2/merge_runlist_9",     "_child_3/merge_runlist_10",    "_child_4/merge_runlist_10",    "_child_5/merge_runlist_10",
   //06 16j 18c13                   07 16o 18a8                     08 16p 17l5                     09 16k 18a9                     10 16l 18a1
   "_child_6/merge_runlist_10",     "_child_7/merge_runlist_10",    "_child_8/merge_runlist_10",    "_child_9/merge_runlist_10",    "_child_10/merge_runlist_10"
};
//==============================================================================================================================
// LHC 18 MC
TString DataSets_18MC[nSets_18MC];
TString DataSets_18MC_PHOS[nSets_18MC]={
   //01 18b 18g4                    02 18d 18g5                     03 18e 18g6                     04 18f 18h2                     05 18g,h,i,j,k 18h4
    "LHC18g4",                      "LHC18g5",                      "LHC18g6",                      "LHC18h2",                      "LHC18h4",
   //06 18l 18j1                    07 18m 18j4                     08 18n 18k1                     09 18o 18k2                     10 18p 18k3
    "LHC18j1",                      "LHC18j4",                      "LHC18k1",                      "LHC18k2",                      "LHC18k3"
};
//---------------------------------------------------------------
TString runlist_18MC[nSets_18MC];
TString runlist_18MC_PHOS[nSets_18MC]{
   //01 18b 18g4                    02 18d 18g5                     03 18e 18g6                     04 18f 18h2                     05 18g,h,i,j,k 18h4
    "_child_1/merge_runlist_6",     "_child_2/merge_runlist_6",     "_child_3/merge_runlist_6",     "_child_4/merge_runlist_6",     "_child_5/merge_runlist_6",
   //06 18l 18j1                    07 18m 18j4                     08 18n 18k1                     09 18o 18k2                     10 18p 18k3
    "_child_6/merge_runlist_6",     "_child_7/merge_runlist_6",     "_child_8/merge_runlist_6",     "_child_9/merge_runlist_6",     "_child_10/merge_runlist_6"
};
//------------------------------------------------------------------------------------------------------------------------------
// LHC 18 MC extra
TString DataSets_18MCextra[nSets_18MCextra];
TString DataSets_18MCextra_PHOS[nSets_18MCextra]={
   //01 18b 18g4                    02 18d 18g5                     03 18e 18g6                     04 18f 18h2                     05 18g,h,i,j,k 18h4
    "LHC18g4_extra",                "LHC18g5_extra",                "LHC18g6_extra",                "LHC18h2_extra",                "LHC18h4_extra",
   //06 18l 18j1                    07 18m 18j4                     08 18n 18k1                     09 18o 18k2                     10 18p 18k3
    "LHC18j1_extra",                "LHC18j4_extra",                "LHC18k1_extra",                "LHC18k2_extra",                "LHC18k3_extra"
};
//---------------------------------------------------------------
TString runlist_18MCextra[nSets_18MCextra];
TString runlist_18MCextra_PHOS[nSets_18MCextra]{
   //01 18b 18g4                    02 18d 18g5                     03 18e 18g6                     04 18f 18h2                     05 18g,h,i,j,k 18h4
    "_child_1/merge_runlist_6",     "_child_2/merge_runlist_6",     "_child_3/merge_runlist_6",     "_child_4/merge_runlist_6",     "_child_5/merge_runlist_6",
   //06 18l 18j1                    07 18m 18j4                     08 18n 18k1                     09 18o 18k2                     10 18p 18k3
    "_child_6/merge_runlist_6",     "_child_7/merge_runlist_6",     "_child_8/merge_runlist_6",     "_child_9/merge_runlist_6",     "_child_10/merge_runlist_6"
};
//******************************************************************************************************************************

Bool_t copyAlien2Local(TString loc, TString rem);
Bool_t readin(TString fileRuns, std::vector<TString> &vec);
void Grid_CopyFiles_DataAOD_13TeV(TString system = "pp", TString type = "AOD", TString folder = "/media/jens/Maxtor/Promotion/DatenFuerAnalyse/DanielsSkript/DataQA", Int_t TrainNumberChoiceArgument=1, Int_t doUnzipArgument=1);
Bool_t copyAlien2Local(TString loc, TString rem);
