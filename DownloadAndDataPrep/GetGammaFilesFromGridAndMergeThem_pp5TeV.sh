#! /bin/bash
source basicFunction.sh

FILENAMEBASE=$4
MODE=$5

if [ $MODE == 0 ]; then
    echo -e "DPGTrack" > runlistsToMerge.txt
elif [ $MODE == 2 ]; then
    echo -e "DPGTrackAndCalo" > runlistsToMerge.txt
elif [ $MODE == 4 ] && [ $3 = "AODmcp" ]; then
    echo -e "EMC" > runlistsToMerge.txt
elif [ $MODE == 4 ] && [ $3 = "AODmcpSKIMEMCFull" ]; then
    echo -e "EMC" > runlistsToMerge.txt
elif [ $MODE == 4 ]; then
    echo -e "DPGTrackAndCalo\nEMC" > runlistsToMerge.txt
fi

# echo -e "DPGTrackIncAccAndEMC" > runlistsToMerge.txt
listsToMerge=`cat runlistsToMerge.txt`

DOWNLOADON=1
MERGEON=1
MERGEONData=1
MERGEONMC=1
MERGEONFASTAndWOSDD=1
SINGLERUN=1
SEPARATEON=0
MERGEONSINGLEData=1
MERGEONSINGLEMC=1
MERGEONSINGLEMCJJ=0
CLEANUP=1
CLEANUPMAYOR=$2
jALIEN=1
number=""
FAST="_FAST"
SEPARATEONLYConv=1
DoStageDownload=0
# check if train configuration has actually been given
HAVELHC15n=1
HAVELHC17e2=1
HAVELHC18j3=1
HAVELHC17q=1
HAVELHC17p=1
HAVELHC17qF=1
HAVELHC17pF=1
HAVELHC17l3b=1
HAVELHC18j2=1
HAVELHC17l3bF=1
HAVELHC18j2F=1
HAVELHC18b8F=1
HAVELHC18b8=1

# default trainconfigurations
LHC15nData="";
LHC15nMC="";
LHC17e2MC="";
LHC18j3MC="";
LHC17qData="";
LHC17pData="";
LHC17qDataFast="";
LHC17pDataFast="";
LHC17pqData="";
LHC17pqMC="";
LHC17l3bMC="";
LHC18j2MC="";
LHC17l3bMCFast="";
LHC18j2MCFast="";
LHC18b8MCMother=""
LHC18b8MCF=""
LHC18b8MC=""
LHC16h3MC=""
LHC18l2MCMother=""
LHC18l2MCE=""
LHC18l2MCD=""

passNr="1";

if [ $1 = "fbock" ]; then
    BASEDIR=/mnt/additionalStorageExternal/OutputLegoTrains/pp5TeV
elif [ $1 = "fbockExt" ]; then
    BASEDIR=/media/fbock/2fba62ae-79f7-469c-bca3-385d6f6261da/OutputLegoTrains/pp5TeV
fi

if [ $3 = "AOD" ]; then
    baseLegoData=GA_pp_AOD
    baseLegoMC=GA_pp_MC_AOD
    pathData15n=pass4/AOD208/PWGGA/GA_pp_AOD
    pathData17pwoSDD=pass1_CENT_woSDD/AOD208/PWGGA/GA_pp_AOD
    pathData17pfast=pass1_FAST/AOD208/PWGGA/GA_pp_AOD
    pathData17qwoSDD=pass1_CENT_woSDD/AOD208/PWGGA/GA_pp_AOD
    pathData17qfast=pass1_FAST/AOD208/PWGGA/GA_pp_AOD
    pathMCR1=AOD209/PWGGA/GA_pp_MC_AOD
    pathMCR2=AOD209/PWGGA/GA_pp_MC_AOD
    pathMCJJ=PWGGA/GA_pp_MC_AOD
    FAST="_FAST"
    WOSDD="_CENT_woSDD"
elif [ $3 = "AODmcp" ]; then
    baseLegoData=GA_pp_AOD
    baseLegoMC=GA_pp_MC_AOD
    pathData15n=muon_calo_pass2/PWGGA/GA_pp_AOD
    pathData17pfast=muon_calo_pass1/PWGGA/GA_pp_AOD
    pathData17qfast=muon_calo_pass1/PWGGA/GA_pp_AOD
    FAST="_FAST"
elif [ $3 = "AODmcpSKIMEMCFull" ]; then
    baseLegoData=GA_pp_AOD
    baseLegoMC=GA_pp_MC_AOD
    pathData15n=muon_calo_pass2/PWGGA/GA_pp_AOD/1229_20200207-1613/PWGGA/GA_pp_AOD
    pathData17pfast=muon_calo_pass1/PWGGA/GA_pp_AOD/1240_20200207-1620/PWGGA/GA_pp_AOD
    pathData17qfast=muon_calo_pass1/PWGGA/GA_pp_AOD/1248_20200210-1801/PWGGA/GA_pp_AOD
    FAST="_FAST"
elif [ $3 = "AODSKIMEMCFull" ]; then
    baseLegoData=GA_pp_AOD
    pathData17pwoSDD=pass1_CENT_woSDD/AOD208/PWGGA/GA_pp_AOD/1232_20200207-1602/PWGGA/GA_pp_AOD
    pathData17pfast=pass1_FAST/AOD208/PWGGA/GA_pp_AOD/1231_20200207-1602/PWGGA/GA_pp_AOD
    pathData17qwoSDD=pass1_CENT_woSDD/AOD208/PWGGA/GA_pp_AOD/1234_20200207-1604/PWGGA/GA_pp_AOD
    pathData17qfast=pass1_FAST/AOD208/PWGGA/GA_pp_AOD/1233_20200207-1603/PWGGA/GA_pp_AOD
    FAST="_FAST"
    WOSDD="_CENT_woSDD"
elif [ $3 = "AODSKIMPHI7" ]; then
    baseLegoData=GA_pp_AOD
    pathData15n=pass4/AOD208/PWGGA/GA_pp_AOD/558_20190604-1320/PWGGA/GA_pp_AOD
    pathData17pwoSDD=pass1_CENT_woSDD/AOD208/PWGGA/GA_pp_AOD
    pathData17pfast=pass1_FAST/AOD208/PWGGA/GA_pp_AOD
    pathData17qwoSDD=pass1_CENT_woSDD/AOD208/PWGGA/GA_pp_AOD
    pathData17qfast=pass1_FAST/AOD208/PWGGA/GA_pp_AOD
    FAST="_FAST"
    WOSDD="_CENT_woSDD"
else
    baseLegoData=GA_pp
    baseLegoMC=GA_pp_MC
    pathData15n=pass4/PWGGA/GA_pp
    pathMCR1=PWGGA/GA_pp_MC
    pathData17pwoSDD=pass1/PWGGA/GA_pp
    pathData17pfast=pass1/PWGGA/GA_pp
    pathMCR2=PWGGA/GA_pp_MC
fi


# Definitition of number of slashes in your path to different depths
NSlashesBASE=`tr -dc '/' <<<"$BASEDIR" | wc -c`
NSlashes=`expr $NSlashesBASE + 4`
NSlashes2=`expr $NSlashes - 1`
NSlashes3=`expr $NSlashes + 1`
NSlashes4=`expr $NSlashes + 2`
echo "$NSlashesBASE $NSlashes $NSlashes2 $NSlashes3 $NSlashes4"

if [ $3 = "AODSKIMMB" ]; then
    TRAINDIR=20200206-MixedEventUpdate
    LHC15nData="child_1"
    
elif [ $3 = "AODmcp" ]; then
#     TRAINDIR=20200206-RerunSysPCMEMC
# #     LHC15nData="1251"
#     LHC15nData="1269"
# #     LHC17pqData="1252";
#     LHC17pqData="1271"; #(PHOS)
#     LHC17pDataFast="child_1";
#     LHC17qDataFast="child_2";
    TRAINDIR=20200224-RerunSysEMC
    LHC15nData="1284"
    LHC17pqData="1285";
    LHC17pDataFast="child_1";
    LHC17qDataFast="child_2";
elif [ $3 = "AODmcpSKIMEMCFull" ]; then
#     TRAINDIR=20200206-RerunSysPCMEMC
# #     LHC15nData="1253"
    LHC17pqData="1255";
#     LHC17pDataFast="child_1";
#     LHC17qDataFast="child_2";
    TRAINDIR=20200224-RerunSysEMC
    LHC15nData="1284"
    LHC17pqData="1285";
    LHC17pDataFast="child_1";
    LHC17qDataFast="child_2";
    
elif [ $3 = "AODSKIMEMCFull" ]; then
    TRAINDIR=20200206-RerunSysPCMEMC
    LHC17pqData="1256";
    LHC17pDataFast="child_1";
    LHC17pData="child_2";
    LHC17qDataFast="child_3";
    LHC17qData="child_4";
elif [ $3 = "AODSKIMPHI7" ]; then
    TRAINDIR=20190903-PHOStriggerStat
    LHC15nData="child_1"
else

    TRAINDIR=20200206-RerunSysPCMEMC
#     LHC15nData="1227"
#     LHC15nData="1249"
#     LHC15nData="1270" #PHOS
#     LHC17pqData="1228";
#     LHC17pqData="1250";
#     LHC17pqData="1272";
#     LHC17pDataFast="child_1";
#     LHC17pData="child_2";
#     LHC17qDataFast="child_3";
#     LHC17qData="child_4";
    
#     LHC15nMC="2280"
#     LHC15nMC="2282"
#     LHC15nMC="2283"
#     LHC15nMC="2302"
#     LHC15nMC="2304" #(EMC)
#     LHC15nMC="2310" #(EMC)
#     LHC15nMC="2332" #(PHOS)
#     LHC17e2MC="child_1"
#     LHC18j3MC="child_2"
#     LHC17pqMC="2281";
#     LHC17pqMC="2284";
#     LHC17pqMC="2285";
#     LHC17pqMC="2303";
#     LHC17pqMC="2305"; #(EMC)
#     LHC17pqMC="2309"; #(EMC)
#     LHC17pqMC="2333"; #(PHOS)
#     LHC17l3bMC="child_2";
#     LHC18j2MC="child_4";
#     LHC17l3bMCFast="child_1";
#     LHC18j2MCFast="child_3";
#     LHC16h3MC="2306";
    LHC16h3MC="2374"; #(EMC rerun proper Thresholds)
#     LHC18b8MCMother="2307";
#     LHC18b8MCMother="2311";
#     LHC18b8MCMother="2337"; #PHOS
    LHC18b8MCMother="2375"; #(EMC rerun proper Thresholds)
    LHC18b8MCF="child_1";
    LHC18b8MC="child_3";
#     LHC18l2MCMother="2369"; #(EMC rerun proper Thresholds)
#     LHC18l2MCE="child_1";
#     LHC18l2MCD="child_2";
fi


OUTPUTDIR=$BASEDIR/$TRAINDIR
ALIENDIRData="/alice/cern.ch/user/a/alitrain/PWGGA/$baseLegoData/"
OUTPUTDIRData=$BASEDIR/$TRAINDIR/$baseLegoData
ALIENDIRMC="/alice/cern.ch/user/a/alitrain/PWGGA/$baseLegoMC/"
OUTPUTDIRMC=$BASEDIR/$TRAINDIR/$baseLegoMC
mkdir -p $OUTPUTDIR/CutSelections
mkdir -p $OUTPUTDIR/SinglePeriods

FindCorrectTrainDirectory $LHC15nData $OUTPUTDIRData $ALIENDIRData 
HAVELHC15n=$tempBool
LHC15nData=$tempDir
OUTPUTDIR_LHC15n=$tempPath
echo "15n: $HAVELHC15n $LHC15nData $OUTPUTDIR_LHC15n"
FindCorrectTrainDirectory $LHC17pData $OUTPUTDIRData $ALIENDIRData $LHC17pqData
HAVELHC17p=$tempBool
LHC17pData=$tempDir
OUTPUTDIR_LHC17p=$tempPath
echo "17p: $HAVELHC17p $LHC17pData $OUTPUTDIR_LHC17p"
FindCorrectTrainDirectory $LHC17pDataFast $OUTPUTDIRData $ALIENDIRData $LHC17pqData
HAVELHC17pF=$tempBool
LHC17pDataFast=$tempDir
OUTPUTDIR_LHC17pF=$tempPath
echo "17p: $HAVELHC17pF $LHC17pDataFast $OUTPUTDIR_LHC17pF"
FindCorrectTrainDirectory $LHC17qData $OUTPUTDIRData $ALIENDIRData $LHC17pqData
HAVELHC17q=$tempBool
LHC17qData=$tempDir
OUTPUTDIR_LHC17q=$tempPath
echo "17q: $HAVELHC17q $LHC17qData $OUTPUTDIR_LHC17q"
FindCorrectTrainDirectory $LHC17qDataFast $OUTPUTDIRData $ALIENDIRData $LHC17pqData
HAVELHC17qF=$tempBool
LHC17qDataFast=$tempDir
OUTPUTDIR_LHC17qF=$tempPath
echo "17q: $HAVELHC17qF $LHC17qDataFast $OUTPUTDIR_LHC17qF"

FindCorrectTrainDirectory $LHC17e2MC $OUTPUTDIRMC $ALIENDIRMC $LHC15nMC
HAVELHC17e2=$tempBool
LHC17e2MC=$tempDir
OUTPUTDIR_LHC17e2=$tempPath
echo "17e2 anchored to 15n: $HAVELHC17e2 $LHC17e2MC $OUTPUTDIR_LHC17e2"
FindCorrectTrainDirectory $LHC18j3MC $OUTPUTDIRMC $ALIENDIRMC $LHC15nMC
HAVELHC18j3=$tempBool
LHC18j3MC=$tempDir
OUTPUTDIR_LHC18j3=$tempPath
echo "18j3 anchored to 15n: $HAVELHC18j3 $LHC18j3MC $OUTPUTDIR_LHC18j3"

FindCorrectTrainDirectory $LHC16h3MC $OUTPUTDIRMC $ALIENDIRMC 
HAVELHC16h3=$tempBool
LHC16h3MC=$tempDir
OUTPUTDIR_LHC16h3=$tempPath
echo "16h3 JJ anchored to 15n: $HAVELHC16h3 $LHC16h3MC $OUTPUTDIR_LHC16h3"

FindCorrectTrainDirectory $LHC17l3bMC $OUTPUTDIRMC $ALIENDIRMC $LHC17pqMC
HAVELHC17l3b=$tempBool
LHC17l3bMC=$tempDir
OUTPUTDIR_LHC17l3b=$tempPath
echo "17l3b anchored to 17pq: $HAVELHC17l3b $LHC17l3bMC $OUTPUTDIR_LHC17l3b"
FindCorrectTrainDirectory $LHC18j2MC $OUTPUTDIRMC $ALIENDIRMC $LHC17pqMC
HAVELHC18j2=$tempBool
LHC18j2MC=$tempDir
OUTPUTDIR_LHC18j2=$tempPath
echo "18j2 anchored to 17pq: $HAVELHC18j2 $LHC18j2MC $OUTPUTDIR_LHC18j2"
FindCorrectTrainDirectory $LHC17l3bMCFast $OUTPUTDIRMC $ALIENDIRMC $LHC17pqMC
HAVELHC17l3bF=$tempBool
LHC17l3bMCFast=$tempDir
OUTPUTDIR_LHC17l3bF=$tempPath
echo "17l3b_fast anchored to 17pq: $HAVELHC17l3bF $LHC17l3bMCFast $OUTPUTDIR_LHC17l3bF"
FindCorrectTrainDirectory $LHC18j2MCFast $OUTPUTDIRMC $ALIENDIRMC $LHC17pqMC
HAVELHC18j2F=$tempBool
LHC18j2MCFast=$tempDir
OUTPUTDIR_LHC18j2F=$tempPath
echo "18j2_fast anchored to 17pq: $HAVELHC18j2F $LHC18j2MCFast $OUTPUTDIR_LHC18j2F"
FindCorrectTrainDirectory $LHC18b8MCF $OUTPUTDIRMC $ALIENDIRMC $LHC18b8MCMother
HAVELHC18b8F=$tempBool
LHC18b8MCF=$tempDir
OUTPUTDIR_LHC18b8F=$tempPath
echo "18b8_fast JJ anchored to 17pq: $HAVELHC18b8F $LHC18b8MCF $OUTPUTDIR_LHC18b8F"
FindCorrectTrainDirectory $LHC18b8MC $OUTPUTDIRMC $ALIENDIRMC $LHC18b8MCMother
HAVELHC18b8=$tempBool
LHC18b8MC=$tempDir
OUTPUTDIR_LHC18b8=$tempPath
echo "18b8_cent_woSDD JJ anchored to 17pq: $HAVELHC18b8 $LHC18b8MC $OUTPUTDIR_LHC18b8"

FindCorrectTrainDirectory $LHC18l2MCE $OUTPUTDIRMC $ALIENDIRMC $LHC18l2MCMother
HAVELHC18l2E=$tempBool
LHC18l2MCE=$tempDir
OUTPUTDIR_LHC18l2E=$tempPath
echo "18l2_EMCal JJ anchored to 17pq: $HAVELHC18l2E $LHC18l2MCE $OUTPUTDIR_LHC18l2E"
FindCorrectTrainDirectory $LHC18l2MCD $OUTPUTDIRMC $ALIENDIRMC $LHC18l2MCMother
HAVELHC18l2D=$tempBool
LHC18l2MCD=$tempDir
OUTPUTDIR_LHC18l2D=$tempPath
echo "18l2_DCal JJ anchored to 17pq: $HAVELHC18l2D $LHC18l2MCD $OUTPUTDIR_LHC18l2D"


currentDir=$PWD
if [ $CLEANUPMAYOR == 0 ]; then
    cd $currentDir
    CopyRunwiseAndMergeAccordingToRunlistData "LHC15n" $HAVELHC15n $OUTPUTDIR_LHC15n $LHC15nData $pathData15n $baseLegoData "/alice/data/2015" $NSlashes3 runlistsToMerge.txt "pass4" $FILENAMEBASE

    cd $currentDir
    CopyRunwiseAndMergeAccordingToRunlistMC "LHC17e2" $HAVELHC17e2 $OUTPUTDIR_LHC17e2 $LHC17e2MC $pathMCR1 $baseLegoMC "/alice/sim/2017" $NSlashes3 runlistsToMerge.txt $FILENAMEBASE
    cd $currentDir
    CopyRunwiseAndMergeAccordingToRunlistMC "LHC18j3" $HAVELHC18j3 $OUTPUTDIR_LHC18j3 $LHC18j3MC $pathMCR1 $baseLegoMC "/alice/sim/2018" $NSlashes3 runlistsToMerge.txt $FILENAMEBASE
    cd $currentDir
    CopyRunwiseAndMergeAccordingToRunlistJJMC "LHC16h3" $HAVELHC16h3 $OUTPUTDIR_LHC16h3 $LHC16h3MC $pathMCJJ $baseLegoMC "/alice/sim/2016" $NSlashes3 runlistsToMerge.txt $FILENAMEBASE runlists/binsJetJetLHC16h3.txt
    
    cd $currentDir
    echo "LHC17q" $HAVELHC17q $OUTPUTDIR_LHC17q $LHC17qData $pathData17qwoSDD $baseLegoData "/alice/data/2017" $NSlashes3 runlistsToMerge.txt "pass1$WOSDD" $FILENAMEBASE
    CopyRunwiseAndMergeAccordingToRunlistData "LHC17q" $HAVELHC17q $OUTPUTDIR_LHC17q $LHC17qData $pathData17qwoSDD $baseLegoData "/alice/data/2017" $NSlashes3 runlistsToMerge.txt "pass1$WOSDD" $FILENAMEBASE
    cd $currentDir
    CopyRunwiseAndMergeAccordingToRunlistData "LHC17q" $HAVELHC17qF $OUTPUTDIR_LHC17qF $LHC17qDataFast $pathData17qfast $baseLegoData "/alice/data/2017" $NSlashes3 runlistsToMerge.txt "pass1$FAST" $FILENAMEBASE
    cd $currentDir
    CopyRunwiseAndMergeAccordingToRunlistData "LHC17p" $HAVELHC17p $OUTPUTDIR_LHC17p $LHC17pData $pathData17pwoSDD $baseLegoData "/alice/data/2017" $NSlashes3 runlistsToMerge.txt "pass1$WOSDD" $FILENAMEBASE
    cd $currentDir
    echo "LHC17p" $HAVELHC17pF $OUTPUTDIR_LHC17pF $LHC17pDataFast $pathData17pfast $baseLegoData "/alice/data/2017" $NSlashes3 runlistsToMerge.txt "pass1$FAST" $FILENAMEBASE
    CopyRunwiseAndMergeAccordingToRunlistData "LHC17p" $HAVELHC17pF $OUTPUTDIR_LHC17pF $LHC17pDataFast $pathData17pfast $baseLegoData "/alice/data/2017" $NSlashes3 runlistsToMerge.txt "pass1$FAST" $FILENAMEBASE


    cd $currentDir
    CopyRunwiseAndMergeAccordingToRunlistMC "LHC17l3b_cent_woSDD" $HAVELHC17l3b $OUTPUTDIR_LHC17l3b $LHC17l3bMC $pathMCR2 $baseLegoMC "/alice/sim/2017" $NSlashes3 runlistsToMerge.txt $FILENAMEBASE
    cd $currentDir
    CopyRunwiseAndMergeAccordingToRunlistMC "LHC18j2_cent_woSDD" $HAVELHC18j2 $OUTPUTDIR_LHC18j2 $LHC18j2MC $pathMCR2 $baseLegoMC "/alice/sim/2018" $NSlashes3 runlistsToMerge.txt $FILENAMEBASE
    cd $currentDir
    CopyRunwiseAndMergeAccordingToRunlistMC "LHC17l3b_fast" $HAVELHC17l3bF $OUTPUTDIR_LHC17l3bF $LHC17l3bMCFast $pathMCR2 $baseLegoMC "/alice/sim/2017" $NSlashes3 runlistsToMerge.txt $FILENAMEBASE
    cd $currentDir
    CopyRunwiseAndMergeAccordingToRunlistMC "LHC18j2_fast" $HAVELHC18j2F $OUTPUTDIR_LHC18j2F $LHC18j2MCFast $pathMCR2 $baseLegoMC "/alice/sim/2018" $NSlashes3 runlistsToMerge.txt $FILENAMEBASE
    cd $currentDir
    CopyRunwiseAndMergeAccordingToRunlistJJMC "LHC18b8_fast" $HAVELHC18b8F $OUTPUTDIR_LHC18b8F $LHC18b8MCF $pathMCJJ $baseLegoMC "/alice/sim/2018" $NSlashes3 runlistsToMerge.txt $FILENAMEBASE runlists/binsJetJetLHC18b8_fast.txt
    cd $currentDir
    CopyRunwiseAndMergeAccordingToRunlistJJMC "LHC18b8_cent_woSDD" $HAVELHC18b8 $OUTPUTDIR_LHC18b8 $LHC18b8MC $pathMCJJ $baseLegoMC "/alice/sim/2018" $NSlashes3 runlistsToMerge.txt $FILENAMEBASE runlists/binsJetJetLHC18b8_cent_woSDD.txt

    CopyRunwiseAndMergeAccordingToRunlistJJMC "LHC18l2a" $HAVELHC18l2E $OUTPUTDIR_LHC18l2E $LHC18l2MCE $pathMCJJ $baseLegoMC "/alice/sim/2018" $NSlashes3 runlistsToMerge.txt $FILENAMEBASE runlists/binsJetJetLHC18l2a.txt
    cd $currentDir
    CopyRunwiseAndMergeAccordingToRunlistJJMC "LHC18l2b" $HAVELHC18l2D $OUTPUTDIR_LHC18l2D $LHC18l2MCD $pathMCJJ $baseLegoMC "/alice/sim/2018" $NSlashes3 runlistsToMerge.txt $FILENAMEBASE runlists/binsJetJetLHC18l2b.txt

    
    for runListName in $listsToMerge; do
        if [ $HAVELHC15n == 1 ]; then
            ls $OUTPUTDIR_LHC15n/$FILENAMEBASE-$runListName\_*.root > fileLHC15n.txt
            fileNumbers=`cat fileLHC15n.txt`
            for fileName in $fileNumbers; do
                echo $fileName
                ChangeStructureIfNeededGeneral $fileName $OUTPUTDIR_LHC15n $NSlashes "LHC15n-pass4-$runListName" "-$runListName"
            done;
        fi
        if [ $HAVELHC17e2 == 1 ]; then
            ls $OUTPUTDIR_LHC17e2/$FILENAMEBASE-$runListName\_*.root > fileLHC18j5.txt
            fileNumbers=`cat fileLHC18j5.txt`
            for fileName in $fileNumbers; do
                echo $fileName
                ChangeStructureIfNeededGeneral $fileName $OUTPUTDIR_LHC17e2 $NSlashes "MC_LHC17e2-$runListName" "-$runListName"
            done;
        fi
        if [ $HAVELHC18j3 == 1 ]; then
            ls $OUTPUTDIR_LHC18j3/$FILENAMEBASE-$runListName\_*.root > fileLHC18j5.txt
            fileNumbers=`cat fileLHC18j5.txt`
            for fileName in $fileNumbers; do
                echo $fileName
                ChangeStructureIfNeededGeneral $fileName $OUTPUTDIR_LHC18j3 $NSlashes "MC_LHC18j3-$runListName" "-$runListName"
            done;
        fi
        if [ $HAVELHC17p == 1 ]; then
            ls $OUTPUTDIR_LHC17p/$FILENAMEBASE-$runListName\_*.root > fileLHC17p.txt
            fileNumbers=`cat fileLHC17p.txt`
            for fileName in $fileNumbers; do
                echo $fileName
                ChangeStructureIfNeededGeneral $fileName $OUTPUTDIR_LHC17p $NSlashes "LHC17p_woSDD-pass1-$runListName" "-$runListName"
            done;
        fi
        if [ $HAVELHC17pF == 1 ]; then
            ls $OUTPUTDIR_LHC17pF/$FILENAMEBASE-$runListName\_*.root > fileLHC17pF.txt
            fileNumbers=`cat fileLHC17pF.txt`
            for fileName in $fileNumbers; do
                echo $fileName
                ChangeStructureIfNeededGeneral $fileName $OUTPUTDIR_LHC17pF $NSlashes "LHC17p_fast-pass1-$runListName" "-$runListName"
            done;
        fi
        if [ $HAVELHC17q == 1 ]; then
            ls $OUTPUTDIR_LHC17q/$FILENAMEBASE-$runListName\_*.root > fileLHC17q.txt
            fileNumbers=`cat fileLHC17q.txt`
            for fileName in $fileNumbers; do
                echo $fileName
                ChangeStructureIfNeededGeneral $fileName $OUTPUTDIR_LHC17q $NSlashes "LHC17q_woSDD-pass1-$runListName" "-$runListName"
            done;
        fi
        if [ $HAVELHC17qF == 1 ]; then
            ls $OUTPUTDIR_LHC17qF/$FILENAMEBASE-$runListName\_*.root > fileLHC17qF.txt
            fileNumbers=`cat fileLHC17qF.txt`
            for fileName in $fileNumbers; do
                echo $fileName
                ChangeStructureIfNeededGeneral $fileName $OUTPUTDIR_LHC17qF $NSlashes "LHC17q_fast-pass1-$runListName" "-$runListName"
            done;
        fi

        if [ $HAVELHC17l3b == 1 ]; then
            ls $OUTPUTDIR_LHC17l3b/$FILENAMEBASE-$runListName\_*.root > fileLHC18f3.txt
            fileNumbers=`cat fileLHC18f3.txt`
            for fileName in $fileNumbers; do
                echo $fileName
                ChangeStructureIfNeededGeneral $fileName $OUTPUTDIR_LHC17l3b $NSlashes "MC_LHC17l3b-$runListName" "-$runListName"
            done;
        fi
        if [ $HAVELHC18j2 == 1 ]; then
            ls $OUTPUTDIR_LHC18j2/$FILENAMEBASE-$runListName\_*.root > fileLHC18f3.txt
            fileNumbers=`cat fileLHC18f3.txt`
            for fileName in $fileNumbers; do
                echo $fileName
                ChangeStructureIfNeededGeneral $fileName $OUTPUTDIR_LHC18j2 $NSlashes "MC_LHC18j2-$runListName" "-$runListName"
            done;
        fi
        if [ $HAVELHC17l3bF == 1 ]; then
            ls $OUTPUTDIR_LHC17l3bF/$FILENAMEBASE-$runListName\_*.root > fileLHC17l3bF.txt
            fileNumbers=`cat fileLHC17l3bF.txt`
            for fileName in $fileNumbers; do
                echo $fileName
                ChangeStructureIfNeededGeneral $fileName $OUTPUTDIR_LHC17l3bF $NSlashes "MC_LHC17l3b_fast-$runListName" "-$runListName"
            done;
        fi
        if [ $HAVELHC18j2F == 1 ]; then
            ls $OUTPUTDIR_LHC18j2F/$FILENAMEBASE-$runListName\_*.root > fileLHC18j2F.txt
            fileNumbers=`cat fileLHC18j2F.txt`
            for fileName in $fileNumbers; do
                echo $fileName
                ChangeStructureIfNeededGeneral $fileName $OUTPUTDIR_LHC18j2F $NSlashes "MC_LHC18j2_fast-$runListName" "-$runListName"
            done;
        fi
        if [ $HAVELHC16h3 == 1 ]; then
            ls $OUTPUTDIR_LHC16h3/$FILENAMEBASE-$runListName\_*.root > fileLHC16h3.txt
            fileNumbers=`cat fileLHC16h3.txt`
            for fileName in $fileNumbers; do
                echo $fileName
                ChangeStructureIfNeededGeneral $fileName $OUTPUTDIR_LHC16h3 $NSlashes "MC_LHC16h3-$runListName" "-$runListName"
            done;
        fi
        if [ $HAVELHC18b8F == 1 ]; then
            ls $OUTPUTDIR_LHC18b8F/$FILENAMEBASE-$runListName\_*.root > fileLHC18b8F.txt
            fileNumbers=`cat fileLHC18b8F.txt`
            for fileName in $fileNumbers; do
                echo $fileName
                ChangeStructureIfNeededGeneral $fileName $OUTPUTDIR_LHC18b8F $NSlashes "MC_LHC18b8_fast-$runListName" "-$runListName"
            done;
        fi
        if [ $HAVELHC18b8 == 1 ]; then
            ls $OUTPUTDIR_LHC18b8/$FILENAMEBASE-$runListName\_*.root > fileLHC18b8.txt
            fileNumbers=`cat fileLHC18b8.txt`
            for fileName in $fileNumbers; do
                echo $fileName
                ChangeStructureIfNeededGeneral $fileName $OUTPUTDIR_LHC18b8 $NSlashes "MC_LHC18b8_cent_woSDD-$runListName" "-$runListName"
            done;
        fi
        if [ $HAVELHC18l2E == 1 ]; then
            ls $OUTPUTDIR_LHC18l2E/$FILENAMEBASE-$runListName\_*.root > fileLHC18l2E.txt
            fileNumbers=`cat fileLHC18l2E.txt`
            for fileName in $fileNumbers; do
                echo $fileName
                ChangeStructureIfNeededGeneral $fileName $OUTPUTDIR_LHC18l2E $NSlashes "MC_LHC18l2a-$runListName" "-$runListName"
            done;
        fi
        if [ $HAVELHC18l2D == 1 ]; then
            ls $OUTPUTDIR_LHC18l2D/$FILENAMEBASE-$runListName\_*.root > fileLHC18l2D.txt
            fileNumbers=`cat fileLHC18l2D.txt`
            for fileName in $fileNumbers; do
                echo $fileName
                ChangeStructureIfNeededGeneral $fileName $OUTPUTDIR_LHC18l2D $NSlashes "MC_LHC18l2b-$runListName" "-$runListName"
            done;
        fi
    done

    if [ $MERGEON == 1 ]; then
        echo "Starting Merging"

        #######################################################################################################################
        ###### Start merging of the data 
        #######################################################################################################################
        if [ $MERGEONData == 1 ]; then
            echo -e "_woSDD\n_fast" > listReconstruction.txt
            listReconstruction=`cat listReconstruction.txt`
            periodList=`echo -e "p\nq"`
            for reco in $listReconstruction; do
                ls $OUTPUTDIR/$FILENAMEBASE\_LHC17q$reco-pass$passNr-$runListName\_*.root > filesForMerging.txt
                filesForMerging=`cat filesForMerging.txt`
                for fileName in $filesForMerging; do
                    echo $fileName
                    GetFileNumberMerging $fileName $((NSlashes-1)) 4
                    echo $number
                    for runListName in $listsToMerge; do
                        rm listCurrMerge.txt
                        nameOut=""
                        for periodID in $periodList; do
                            echo $periodID
                            currFile=$OUTPUTDIR/$FILENAMEBASE\_LHC17$periodID$reco-pass$passNr-$runListName\_$number.root
                            if [ -f $currFile ]; then
                                outAdd=`echo $periodID  | cut -d "-" -f 1 `
                                nameOut+=$outAdd
                                echo -e "$currFile\n" >> listCurrMerge.txt
                            else
                                echo $currFile " does not exist"
                            fi
                        done
                        MergeAccordingToList listCurrMerge.txt $OUTPUTDIR/$FILENAMEBASE\_LHC17$nameOut$reco-pass$passNr-$runListName\_$number.root
                        for periodID in $periodList; do
                            mv $OUTPUTDIR/$FILENAMEBASE\_LHC17$periodID$reco-pass$passNr-$runListName\_$number.root $OUTPUTDIR/SinglePeriods/
                        done  
                    done
                done
            done
        
            ls $OUTPUTDIR/$FILENAMEBASE\_LHC17pq_fast-pass$passNr-$runListName\_*.root | grep -v "WTree" > filesForMerging.txt
            filesForMerging=`cat filesForMerging.txt`
            periodList=`echo -e "_fast\n_woSDD"`
            for fileName in $filesForMerging; do
                echo $fileName
                GetFileNumberMerging $fileName $((NSlashes-1)) 4
                echo $number
                for runListName in $listsToMerge; do
                    rm listCurrMerge.txt
                    nameOut=""
                    for periodID in $periodList; do
                        echo $periodID
                        currFile=$OUTPUTDIR/$FILENAMEBASE\_LHC17pq$periodID-pass$passNr-$runListName\_$number.root
                        if [ -f $currFile ]; then
                            outAdd=`echo $periodID  | cut -d "-" -f 1 `
                            nameOut+=$outAdd
                            echo -e "$currFile\n" >> listCurrMerge.txt
                        else
                            echo $currFile " does not exist"
                        fi
                    done
                    MergeAccordingToList listCurrMerge.txt $OUTPUTDIR/$FILENAMEBASE\_LHC17pq$nameOut-pass$passNr-$runListName\_$number.root
                    for periodID in $periodList; do
                        mv $OUTPUTDIR/$FILENAMEBASE\_LHC17pq$periodID-pass$passNr-$runListName\_$number.root $OUTPUTDIR/SinglePeriods/
                    done  
                done
            done
            ls $OUTPUTDIR/$FILENAMEBASE\_LHC17pq_fast_woSDD-pass$passNr-$runListName\_*.root | grep -v "WTree" > filesForMerging.txt
            filesForMerging=`cat filesForMerging.txt`
            periodList=`echo -e "LHC15n-pass4\nLHC17pq_fast_woSDD-pass$passNr"`
            for fileName in $filesForMerging; do
                echo $fileName
                GetFileNumberMerging $fileName $((NSlashes-1)) 5
                echo $number
                for runListName in $listsToMerge; do
                    rm listCurrMerge.txt
                    nameOut=""
                    for periodID in $periodList; do
                        echo $periodID
                        currFile=$OUTPUTDIR/$FILENAMEBASE\_$periodID-$runListName\_$number.root
                        if [ -f $currFile ]; then
                            outAdd=`echo $periodID  | cut -d "-" -f 1 `
                            nameOut+=_$outAdd
                            echo -e "$currFile\n" >> listCurrMerge.txt
                        else
                            echo $currFile " does not exist"
                        fi
                    done
                    MergeAccordingToList listCurrMerge.txt $OUTPUTDIR/$FILENAMEBASE$nameOut-$runListName\_$number.root
                done
            done

        fi

        #######################################################################################################################
        ###### Start merging of the MC
        #######################################################################################################################

        if [ $MERGEONMC == 1 ]; then
            for runListName in $listsToMerge; do
                ls $OUTPUTDIR/$FILENAMEBASE\_MC_LHC17e2-$runListName\_*.root > filesForMerging.txt
                filesForMerging=`cat filesForMerging.txt`
                periodList=`echo -e "17e2\n18j3"`
                for fileName in $filesForMerging; do
                    echo $fileName
                    GetFileNumberMerging $fileName $((NSlashes-1)) 4 "bla" 1
                    echo $number
                    nameOut=""
                    rm listCurrMerge.txt
                    echo $fileName
                    for periodID in $periodList; do
                        echo $periodID
                        currFile=$OUTPUTDIR/$FILENAMEBASE\_MC_LHC$periodID-$runListName\_$number.root
                        if [ -f $currFile ]; then
                            outAdd=`echo $periodID  | cut -d "-" -f 1 `
                            nameOut+=$outAdd
                            echo -e "$currFile\n" >> listCurrMerge.txt
                        else
                            echo $currFile " does not exist"
                        fi
                    done
                    MergeAccordingToList listCurrMerge.txt $OUTPUTDIR/$FILENAMEBASE\_MC_LHC15nanc-$runListName\_$number.root
                    for periodID in $periodList; do
                        mv $OUTPUTDIR/$FILENAMEBASE\_MC_LHC$periodID-$runListName\_$number.root $OUTPUTDIR/SinglePeriods/
                    done
                done
            done
            
            ls $OUTPUTDIR/$FILENAMEBASE\_MC_LHC17l3b_fast-$runListName\_*.root | grep -v "WTree" > filesForMerging.txt
            filesForMerging=`cat filesForMerging.txt`
            periodList=`echo -e "17l3b_fast\n18j2_fast\n17l3b\n18j2"`
            for fileName in $filesForMerging; do
                echo $fileName
                GetFileNumberMerging $fileName $((NSlashes-1)) 5
                echo $number
                for runListName in $listsToMerge; do
                    rm listCurrMerge.txt
                    nameOut=""
                    for periodID in $periodList; do
                        echo $periodID
                        currFile=$OUTPUTDIR/$FILENAMEBASE\_MC_LHC$periodID-$runListName\_$number.root
                        if [ -f $currFile ]; then
                            outAdd=`echo $periodID  | cut -d "-" -f 1 `
                            nameOut+=$outAdd
                            echo -e "$currFile\n" >> listCurrMerge.txt
                        else
                            echo $currFile " does not exist"
                        fi
                    done
                    if [ $nameOut = "17l3b_fast18j2_fast17l3b18j2" ]; then
                        nameOut="17pqanc";
                    fi
                    MergeAccordingToList listCurrMerge.txt $OUTPUTDIR/$FILENAMEBASE\_MC_LHC$nameOut-$runListName\_$number.root
                    for periodID in $periodList; do
                        mv $OUTPUTDIR/$FILENAMEBASE\_MC_LHC$periodID-$runListName\_$number.root $OUTPUTDIR/SinglePeriods/
                    done  
                done
            done


            ls $OUTPUTDIR/$FILENAMEBASE\_MC_LHC17pqanc-$runListName\_*.root | grep -v "WTree" > filesForMerging.txt
            filesForMerging=`cat filesForMerging.txt`
            periodList=`echo -e "LHC15nanc\nLHC17pqanc"`
            for fileName in $filesForMerging; do
                echo $fileName
                GetFileNumberMerging $fileName $((NSlashes-1)) 4
                echo $number
                for runListName in $listsToMerge; do
                    rm listCurrMerge.txt
                    nameOut=""
                    for periodID in $periodList; do
                        echo $periodID
                        currFile=$OUTPUTDIR/$FILENAMEBASE\_MC_$periodID-$runListName\_$number.root
                        if [ -f $currFile ]; then
                            outAdd=`echo $periodID  | cut -d "-" -f 1 `
                            nameOut+=_$outAdd
                            echo -e "$currFile\n" >> listCurrMerge.txt
                        else
                            echo $currFile " does not exist"
                        fi
                    done
                    MergeAccordingToList listCurrMerge.txt $OUTPUTDIR/$FILENAMEBASE\_MC$nameOut-$runListName\_$number.root
                done
            done
        fi
    fi
    rm runlistsToMerge.txt
else
    if [ $HAVELHC17q == 1 ]; then
        echo "removing all $FILENAMEBASE files in runFolders for LHC17q";
        rm $OUTPUTDIR_LHC17q/*/*/*$FILENAMEBASE\_*.root
    fi
    if [ $HAVELHC17p == 1 ]; then
        echo "removing all $FILENAMEBASE files in runFolders for LHC17p";
        rm $OUTPUTDIR_LHC17p/*/*/*$FILENAMEBASE\_*.root
    fi
    if [ $HAVELHC17qF == 1 ]; then
        echo "removing all $FILENAMEBASE files in runFolders for LHC17q";
        rm $OUTPUTDIR_LHC17qF/*/*/*$FILENAMEBASE\_*.root
    fi
    if [ $HAVELHC17pF == 1 ]; then
        echo "removing all $FILENAMEBASE files in runFolders for LHC17p";
        rm $OUTPUTDIR_LHC17pF/*/*/*$FILENAMEBASE\_*.root
    fi
    if [ $HAVELHC17e2 == 1 ]; then
        echo "removing all $FILENAMEBASE files in runFolders for LHC17e2";
        rm -rf $OUTPUTDIR_LHC17e2/*/Stage*
    fi
    if [ $HAVELHC18j3 == 1 ]; then
        echo "removing all $FILENAMEBASE files in runFolders for LHC18j3";
        rm -rf $OUTPUTDIR_LHC18j3/*/Stage*
    fi
    if [ $HAVELHC17l3bF == 1 ]; then
        echo "removing all $FILENAMEBASE files in runFolders for LHC17l3b_fast";
        rm -rf $OUTPUTDIR_LHC17l3bF/*/Stage*
    fi
    if [ $HAVELHC17l3b == 1 ]; then
        echo "removing all $FILENAMEBASE files in runFolders for LHC17l3b_cent_woSDD";
        rm -rf $OUTPUTDIR_LHC17l3b/*/Stage*
    fi
    if [ $HAVELHC18j2F == 1 ]; then
        echo "removing all $FILENAMEBASE files in runFolders for LHC18j2_fast";
        rm -rf $OUTPUTDIR_LHC18j2F/*/Stage*
    fi
    if [ $HAVELHC18j2 == 1 ]; then
        echo "removing all $FILENAMEBASE files in runFolders for LHC18j2_cent_woSDD";
        rm -rf $OUTPUTDIR_LHC18j2/*/Stage*
    fi
fi
