#! /bin/bash

# copies files from grid
# creates directory
# changes internal structure
# merges files according to the pPb needs

#! /bin/bash
source basicFunction.sh

DOWNLOADON=1
MERGEON=1
SINGLERUN=1
SEPARATEON=0
MERGEONSINGLEData=1
MERGEONSINGLEMC=1
CLEANUP=1
CLEANUPMAYOR=$2
number=""

# check if train configuration has actually been given
HAVELHC16k=1
HAVELHC16l=1
HAVETOBUILDData=0
HAVELHC17d20a1=1
HAVELHC17d20a1Ex=1
HAVELHC17d20a2=1
HAVELHC17d20a2Ex=1
HAVETOBUILDMC=0

# default trainconfigurations

LHC16kData="";
LHC16lData="";
LHC16Data="";
LHC17MCPythia="";
LHC17MCEPOS="";
LHC17d20a1MC="";
LHC17d20a1ExMC="";
LHC17d20a2MC="";
LHC17d20a2ExMC="";

passNr="1";
NSlashes=10;

if [ $1 = "fbock" ]; then
    BASEDIR=/mnt/additionalStorageExternal/OutputLegoTrains/pp13TeV
    NSlashes=8
    NSlashes2=7
    NSlashes3=9
    NSlashes4=10
elif [ $1 = "hannahbossi" ]; then
    BASEDIR=/Users/hannahbossi/data/OutputLegoTrains
    NSlashes=8
    NSlashes2=7
    NSlashes3=9
    NSlashes4=10
elif [ $1 = "dmuhlhei" ]; then
    BASEDIR=~/data/work/Grid
    NSlashes=9;
fi

TRAINDIR=Legotrain-vAN20171105-QA

LHC16Data="2254"; #pass 1
LHC16dData = "child_1"; #pass 1
LHC16gData = "child_2"; #pass 1
LHC16hData = "child_3"; #pass 1
LHC16iData = "child_4"; #pass 1
LHC16jData = "child_5"; #pass 1
LHC16kData = "child_6"; #pass 1
LHC16lData = "child_7"; #pass 1
LHC16oData = "child_8"; #pass 1
LHC16pData = "child_9"; #pass 1
LHC16eData = "child_10"; #pass 1

LHC17MCPythia="3176"; #pass 1
LHC17d20a1MC="child_7";
LHC17d20a1ExMC="child_8";
LHC17d20a2MC="child_9";
LHC17d20a2ExMC="child_10";

OUTPUTDIR=$BASEDIR/$TRAINDIR
echo $BASEDIR
echo $OUTPUTDIR

if [ "$LHC16dData" == "" ]; then
    HAVELHC16d=0;
fi
if [ "$LHC16gData" == "" ]; then
    HAVELHC16g=0;
fi
if [ "$LHC16hData" == "" ]; then
    HAVELHC16h=0;
fi
if [ "$LHC16iData" == "" ]; then
    HAVELHC16i=0;
fi
if [ "$LHC16jData" == "" ]; then
    HAVELHC16j=0;
fi
if [ "$LHC16kData" == "" ]; then
    HAVELHC16k=0;
fi
if [ "$LHC16lData" = "" ]; then
    HAVELHC16l=0;
fi
if [ "$LHC16oData" = "" ]; then
    HAVELHC16o=0;
fi
if [ "$LHC16pData" = "" ]; then
    HAVELHC16p=0;
fi
if [ "$LHC16eData" = "" ]; then
    HAVELHC16e=0;
fi
if [ "$LHC16Data" != "" ]; then
    HAVETOBUILDData=1;
fi

if [ "$LHC17d20a1MC" == "" ]; then
    HAVELHC17d20a1=0;
    echo $LHC17d20a1MC
fi
if [ "$LHC17d20a1ExMC" == "" ]; then
    HAVELHC17d20a1Ex=0;
fi
if [ "$LHC17d20a2MC" == "" ]; then
    HAVELHC17d20a2=0;
    echo $LHC17d20a2MC
fi
if [ "$LHC17d20a2ExMC" == "" ]; then
    HAVELHC17d20a2Ex=0;
fi
if [ "$LHC17MCPythia" != "" ]; then
    HAVETOBUILDMC=1;
fi
if [ "$LHC17MCEPOS" != "" ]; then
    HAVETOBUILDMC=1;
fi


mkdir -p $OUTPUTDIR/CutSelections
# Get data directory for 16d period
if [ $HAVELHC16d == 1 ]; then
    if [ $HAVETOBUILDData == 1 ]; then
        LHC16dData=`alien_ls /alice/cern.ch/user/a/alitrain/PWGGA/GA_pp/ | grep $LHC16Data\_ | grep $LHC16dData`
    else
        LHC16dData=`alien_ls /alice/cern.ch/user/a/alitrain/PWGGA/GA_pp/ | grep $LHC16dData\_`
    fi
    if [ "$LHC16dData" == "" ]; then
        HAVELHC16d=0;
    else
        OUTPUTDIR_LHC16d=$BASEDIR/$TRAINDIR/GA_pp-$LHC16dData
    fi
    echo $OUTPUTDIR_LHC16d
fi
# Get data directory for 16g period
if [ $HAVELHC16g == 1 ]; then
    if [ $HAVETOBUILDData == 1 ]; then
        LHC16gData=`alien_ls /alice/cern.ch/user/a/alitrain/PWGGA/GA_pp/ | grep $LHC16Data\_ | grep $LHC16gData`
    else
        LHC16gData=`alien_ls /alice/cern.ch/user/a/alitrain/PWGGA/GA_pp/ | grep $LHC16gData\_`
    fi
    if [ "$LHC16gData" == "" ]; then
        HAVELHC16g=0;
    else
        OUTPUTDIR_LHC16g=$BASEDIR/$TRAINDIR/GA_pp-$LHC16gData
    fi
    echo $OUTPUTDIR_LHC16g
fi
# Get data directory for 16h period
if [ $HAVELHC16h == 1 ]; then
    if [ $HAVETOBUILDData == 1 ]; then
        LHC16hData=`alien_ls /alice/cern.ch/user/a/alitrain/PWGGA/GA_pp/ | grep $LHC16Data\_ | grep $LHC16hData`
    else
        LHC16hData=`alien_ls /alice/cern.ch/user/a/alitrain/PWGGA/GA_pp/ | grep $LHC16hData\_`
    fi
    if [ "$LHC16hData" == "" ]; then
        HAVELHC16h=0;
    else
        OUTPUTDIR_LHC16h=$BASEDIR/$TRAINDIR/GA_pp-$LHC16hData
    fi
    echo $OUTPUTDIR_LHC16h
fi
# Get data directory for 16i period
if [ $HAVELHC16i == 1 ]; then
    if [ $HAVETOBUILDData == 1 ]; then
        LHC16iData=`alien_ls /alice/cern.ch/user/a/alitrain/PWGGA/GA_pp/ | grep $LHC16Data\_ | grep $LHC16iData`
    else
        LHC16iData=`alien_ls /alice/cern.ch/user/a/alitrain/PWGGA/GA_pp/ | grep $LHC16iData\_`
    fi
    if [ "$LHC16iData" == "" ]; then
        HAVELHC16i=0;
    else
        OUTPUTDIR_LHC16i=$BASEDIR/$TRAINDIR/GA_pp-$LHC16iData
    fi
    echo $OUTPUTDIR_LHC16i
fi
# Get data directory for 16j period
if [ $HAVELHC16j == 1 ]; then
    if [ $HAVETOBUILDData == 1 ]; then
        LHC16jData=`alien_ls /alice/cern.ch/user/a/alitrain/PWGGA/GA_pp/ | grep $LHC16Data\_ | grep $LHC16jData`
    else
        LHC16jData=`alien_ls /alice/cern.ch/user/a/alitrain/PWGGA/GA_pp/ | grep $LHC16jData\_`
    fi
    if [ "$LHC16jData" == "" ]; then
        HAVELHC16j=0;
    else
        OUTPUTDIR_LHC16j=$BASEDIR/$TRAINDIR/GA_pp-$LHC16jData
    fi
    echo $OUTPUTDIR_LHC16j
fi
# Get data directory for 16k period
if [ $HAVELHC16k == 1 ]; then
    if [ $HAVETOBUILDData == 1 ]; then
        LHC16kData=`alien_ls /alice/cern.ch/user/a/alitrain/PWGGA/GA_pp/ | grep $LHC16Data\_ | grep $LHC16kData`
    else
        LHC16kData=`alien_ls /alice/cern.ch/user/a/alitrain/PWGGA/GA_pp/ | grep $LHC16kData\_`
    fi
    if [ "$LHC16kData" == "" ]; then
        HAVELHC16k=0;
    else
        OUTPUTDIR_LHC16k=$BASEDIR/$TRAINDIR/GA_pp-$LHC16kData
    fi
    echo $OUTPUTDIR_LHC16k
fi
# Get data directory for 16l period
if [ $HAVELHC16l == 1 ]; then
    if [ $HAVETOBUILDData == 1 ]; then
        LHC16lData=`alien_ls /alice/cern.ch/user/a/alitrain/PWGGA/GA_pp/ | grep $LHC16Data\_ | grep $LHC16lData`
    else
        LHC16lData=`alien_ls /alice/cern.ch/user/a/alitrain/PWGGA/GA_pp/ | grep $LHC16lData\_`
    fi
    if [ "$LHC16lData" == "" ]; then
        HAVELHC16l=0;
    else
        OUTPUTDIR_LHC16l=$BASEDIR/$TRAINDIR/GA_pp-$LHC16lData
    fi
    echo $OUTPUTDIR_LHC16l
fi
# Get data directory for 16o period
if [ $HAVELHC16o == 1 ]; then
    if [ $HAVETOBUILDData == 1 ]; then
        LHC16oData=`alien_ls /alice/cern.ch/user/a/alitrain/PWGGA/GA_pp/ | grep $LHC16Data\_ | grep $LHC16oData`
    else
        LHC16oData=`alien_ls /alice/cern.ch/user/a/alitrain/PWGGA/GA_pp/ | grep $LHC16oData\_`
    fi
    if [ "$LHC16oData" == "" ]; then
        HAVELHC16o=0;
    else
        OUTPUTDIR_LHC16o=$BASEDIR/$TRAINDIR/GA_pp-$LHC16oData
    fi
    echo $OUTPUTDIR_LHC16o
fi
# Get data directory for 16p period
if [ $HAVELHC16p == 1 ]; then
    if [ $HAVETOBUILDData == 1 ]; then
        LHC16pData=`alien_ls /alice/cern.ch/user/a/alitrain/PWGGA/GA_pp/ | grep $LHC16Data\_ | grep $LHC16pData`
    else
        LHC16pData=`alien_ls /alice/cern.ch/user/a/alitrain/PWGGA/GA_pp/ | grep $LHC16pData\_`
    fi
    if [ "$LHC16pData" == "" ]; then
        HAVELHC16p=0;
    else
        OUTPUTDIR_LHC16p=$BASEDIR/$TRAINDIR/GA_pp-$LHC16pData
    fi
    echo $OUTPUTDIR_LHC16p
fi
# Get data directory for 16e period
if [ $HAVELHC16e == 1 ]; then
    if [ $HAVETOBUILDData == 1 ]; then
        LHC16eData=`alien_ls /alice/cern.ch/user/a/alitrain/PWGGA/GA_pp/ | grep $LHC16Data\_ | grep $LHC16eData`
    else
        LHC16eData=`alien_ls /alice/cern.ch/user/a/alitrain/PWGGA/GA_pp/ | grep $LHC16eData\_`
    fi
    if [ "$LHC16eData" == "" ]; then
        HAVELHC16e=0;
    else
        OUTPUTDIR_LHC16e=$BASEDIR/$TRAINDIR/GA_pp-$LHC16eData
    fi
    echo $OUTPUTDIR_LHC16e
fi


# Get MC directory for LHC17d20a1 MC anchored to LHC16k
if [ $HAVELHC17d20a1 == 1 ]; then
    if [ $HAVETOBUILDMC == 1 ]; then
        LHC17d20a1MC=`alien_ls /alice/cern.ch/user/a/alitrain/PWGGA/GA_pp_MC/ | grep $LHC17MCPythia\_ | grep $LHC17d20a1MC`
    else
        LHC17d20a1MC=`alien_ls /alice/cern.ch/user/a/alitrain/PWGGA/GA_pp_MC/ | grep $LHC17d20a1MC\_`
    fi
    if [ "$LHC17d20a1MC" == "" ]; then
        HAVELHC17d20a1=0;
    else
        OUTPUTDIR_LHC17d20a1=$BASEDIR/$TRAINDIR/GA_pp_MC-$LHC17d20a1MC
    fi
    echo $OUTPUTDIR_LHC17d20a1
fi

# Get MC directory for LHC17d20a1_extra MC anchored to LHC16k
if [ $HAVELHC17d20a1Ex == 1 ]; then
    if [ $HAVETOBUILDMC == 1 ]; then
        LHC17d20a1ExMC=`alien_ls /alice/cern.ch/user/a/alitrain/PWGGA/GA_pp_MC/ | grep $LHC17MCPythia\_ | grep $LHC17d20a1ExMC`
    else
        LHC17d20a1ExMC=`alien_ls /alice/cern.ch/user/a/alitrain/PWGGA/GA_pp_MC/ | grep $LHC17d20a1ExMC\_`
    fi
    if [ "$LHC17d20a1ExMC" == "" ]; then
        HAVELHC17d20a1Ex=0;
    else
        OUTPUTDIR_LHC17d20a1Ex=$BASEDIR/$TRAINDIR/GA_pp_MC-$LHC17d20a1ExMC
    fi
    echo $OUTPUTDIR_LHC17d20a1Ex
fi

# Get MC directory for LHC17d20a2 MC anchored to LHC16k
if [ $HAVELHC17d20a2 == 1 ]; then
    if [ $HAVETOBUILDMC == 1 ]; then
        LHC17d20a2MC=`alien_ls /alice/cern.ch/user/a/alitrain/PWGGA/GA_pp_MC/ | grep $LHC17MCPythia\_ | grep $LHC17d20a2MC`
    else
        LHC17d20a2MC=`alien_ls /alice/cern.ch/user/a/alitrain/PWGGA/GA_pp_MC/ | grep $LHC17d20a2MC\_`
    fi
    if [ "$LHC17d20a2MC" == "" ]; then
        HAVELHC17d20a2=0;
    else
        OUTPUTDIR_LHC17d20a2=$BASEDIR/$TRAINDIR/GA_pp_MC-$LHC17d20a2MC
    fi
    echo $OUTPUTDIR_LHC17d20a2
fi

# Get MC directory for LHC17d20a2_extra MC anchored to LHC16k
if [ $HAVELHC17d20a2Ex == 1 ]; then
    if [ $HAVETOBUILDMC == 1 ]; then
        LHC17d20a2ExMC=`alien_ls /alice/cern.ch/user/a/alitrain/PWGGA/GA_pp_MC/ | grep $LHC17MCPythia\_ | grep $LHC17d20a2ExMC`
    else
        LHC17d20a2ExMC=`alien_ls /alice/cern.ch/user/a/alitrain/PWGGA/GA_pp_MC/ | grep $LHC17d20a2ExMC\_`
    fi
    if [ "$LHC17d20a2ExMC" == "" ]; then
        HAVELHC17d20a2Ex=0;
    else
        OUTPUTDIR_LHC17d20a2Ex=$BASEDIR/$TRAINDIR/GA_pp_MC-$LHC17d20a2ExMC
    fi
    echo $OUTPUTDIR_LHC17d20a2Ex
fi



if [ $CLEANUPMAYOR == 0 ]; then
  if [ $HAVELHC16d == 1 ]; then
      echo "downloading LHC16d"
      if [ $SINGLERUN == 1 ]; then
          runNumbers=`cat runlists/runNumbersLHC16d.txt`
          echo $runNumbers
          for runNumber in $runNumbers; do
              CopyFileIfNonExisitent $OUTPUTDIR_LHC16d/$runNumber "/alice/data/2016/LHC16d/000$runNumber/pass$passNr/PWGGA/GA_pp/$LHC16dData" $NSlashes3 "/alice/data/2016/LHC16d/000$runNumber/pass$passNr/PWGGA/GA_pp/$LHC16dData/" kTRUE
          done;
          if [ $MERGEONSINGLEData == 1 ] && [ ! -f $OUTPUTDIR_LHC16d/mergedAllCalo.txt ]; then
              rm $OUTPUTDIR_LHC16d/GammaCalo*.root*
              firstrunNumber=`head -n1 runlists/runNumbersLHC16d_pass1.txt`
              ls $OUTPUTDIR_LHC16d/$firstrunNumber/GammaCalo_*.root > fileLHC16d.txt

              MergeAccordingToSpecificRunlist fileLHC16d.txt $OUTPUTDIR_LHC16d $NSlashes3 GammaCalo All runlists/runNumbersLHC16d_pass1.txt
              MergeAccordingToSpecificRunlist fileLHC16d.txt $OUTPUTDIR_LHC16d $NSlashes3 GammaCalo DPGTrack runlists/runNumbersLHC16d_pass1_DPG.txt
              MergeAccordingToSpecificRunlist fileLHC16d.txt $OUTPUTDIR_LHC16d $NSlashes3 GammaCalo DPGTrackAndCalo runlists/runNumbersLHC16d_pass1_DPGTrackAndCalo.txt
          fi
      else
          CopyFileIfNonExisitent $OUTPUTDIR_LHC16d "/alice/cern.ch/user/a/alitrain/PWGGA/GA_pp/$LHC16dData/merge_runlist_1" $NSlashes "" kTRUE
      fi
  fi
  if [ $HAVELHC16g == 1 ]; then
      echo "downloading LHC16g"
      if [ $SINGLERUN == 1 ]; then
          runNumbers=`cat runlists/runNumbersLHC16g.txt`
          echo $runNumbers
          for runNumber in $runNumbers; do
              CopyFileIfNonExisitent $OUTPUTDIR_LHC16g/$runNumber "/alice/data/2016/LHC16g/000$runNumber/pass$passNr/PWGGA/GA_pp/$LHC16gData" $NSlashes3 "/alice/data/2016/LHC16g/000$runNumber/pass$passNr/PWGGA/GA_pp/$LHC16gData/" kTRUE
          done;
          if [ $MERGEONSINGLEData == 1 ] && [ ! -f $OUTPUTDIR_LHC16g/mergedAllCalo.txt ]; then
              rm $OUTPUTDIR_LHC16g/GammaCalo*.root*
              firstrunNumber=`head -n1 runlists/runNumbersLHC16g_pass1.txt`
              ls $OUTPUTDIR_LHC16g/$firstrunNumber/GammaCalo_*.root > fileLHC16g.txt

              MergeAccordingToSpecificRunlist fileLHC16g.txt $OUTPUTDIR_LHC16g $NSlashes3 GammaCalo All runlists/runNumbersLHC16g_pass1.txt
              MergeAccordingToSpecificRunlist fileLHC16g.txt $OUTPUTDIR_LHC16g $NSlashes3 GammaCalo DPGTrack runlists/runNumbersLHC16g_pass1_DPG.txt
              MergeAccordingToSpecificRunlist fileLHC16g.txt $OUTPUTDIR_LHC16g $NSlashes3 GammaCalo DPGTrackAndCalo runlists/runNumbersLHC16g_pass1_DPGTrackAndCalo.txt
          fi
      else
          CopyFileIfNonExisitent $OUTPUTDIR_LHC16g "/alice/cern.ch/user/a/alitrain/PWGGA/GA_pp/$LHC16gData/merge_runlist_1" $NSlashes "" kTRUE
      fi
  fi
  if [ $HAVELHC16h == 1 ]; then
      echo "downloading LHC16h"
      if [ $SINGLERUN == 1 ]; then
          runNumbers=`cat runlists/runNumbersLHC16h.txt`
          echo $runNumbers
          for runNumber in $runNumbers; do
              CopyFileIfNonExisitent $OUTPUTDIR_LHC16h/$runNumber "/alice/data/2016/LHC16h/000$runNumber/pass$passNr/PWGGA/GA_pp/$LHC16hData" $NSlashes3 "/alice/data/2016/LHC16h/000$runNumber/pass$passNr/PWGGA/GA_pp/$LHC16hData/" kTRUE
          done;
          if [ $MERGEONSINGLEData == 1 ] && [ ! -f $OUTPUTDIR_LHC16h/mergedAllCalo.txt ]; then
              rm $OUTPUTDIR_LHC16h/GammaCalo*.root*
              firstrunNumber=`head -n1 runlists/runNumbersLHC16h_pass1.txt`
              ls $OUTPUTDIR_LHC16h/$firstrunNumber/GammaConvCalo_*.root > fileLHC16h.txt

              MergeAccordingToSpecificRunlist fileLHC16h.txt $OUTPUTDIR_LHC16h $NSlashes3 GammaCalo All runlists/runNumbersLHC16h_pass1.txt
              MergeAccordingToSpecificRunlist fileLHC16h.txt $OUTPUTDIR_LHC16h $NSlashes3 GammaCalo DPGTrack runlists/runNumbersLHC16h_pass1_DPG.txt
              MergeAccordingToSpecificRunlist fileLHC16h.txt $OUTPUTDIR_LHC16h $NSlashes3 GammaCalo DPGTrackAndCalo runlists/runNumbersLHC16h_pass1_DPGTrackAndCalo.txt
          fi
      else
          CopyFileIfNonExisitent $OUTPUTDIR_LHC16h "/alice/cern.ch/user/a/alitrain/PWGGA/GA_pp/$LHC16hData/merge_runlist_1" $NSlashes "" kTRUE
      fi
  fi
  if [ $HAVELHC16i == 1 ]; then
      echo "downloading LHC16i"
      if [ $SINGLERUN == 1 ]; then
          runNumbers=`cat runlists/runNumbersLHC16i.txt`
          echo $runNumbers
          for runNumber in $runNumbers; do
              CopyFileIfNonExisitent $OUTPUTDIR_LHC16i/$runNumber "/alice/data/2016/LHC16i/000$runNumber/pass$passNr/PWGGA/GA_pp/$LHC16iData" $NSlashes3 "/alice/data/2016/LHC16i/000$runNumber/pass$passNr/PWGGA/GA_pp/$LHC16iData/" kTRUE
          done;
          if [ $MERGEONSINGLEData == 1 ] && [ ! -f $OUTPUTDIR_LHC16i/mergedAllConv.txt ]; then
              rm $OUTPUTDIR_LHC16i/GammaCalo*.root*
              firstrunNumber=`head -n1 runlists/runNumbersLHC16i_pass1.txt`
              ls $OUTPUTDIR_LHC16i/$firstrunNumber/GammaCalo_*.root > fileLHC16i.txt

              MergeAccordingToSpecificRunlist fileLHC16i.txt $OUTPUTDIR_LHC16i $NSlashes3 GammaCalo All runlists/runNumbersLHC16i_pass1.txt
              MergeAccordingToSpecificRunlist fileLHC16i.txt $OUTPUTDIR_LHC16i $NSlashes3 GammaCalo DPGTrack runlists/runNumbersLHC16i_pass1_DPG.txt
              MergeAccordingToSpecificRunlist fileLHC16i.txt $OUTPUTDIR_LHC16i $NSlashes3 GammaCalo DPGTrackAndCalo runlists/runNumbersLHC16i_pass1_DPGTrackAndCalo.txt
          fi
      else
          CopyFileIfNonExisitent $OUTPUTDIR_LHC16i "/alice/cern.ch/user/a/alitrain/PWGGA/GA_pp/$LHC16iData/merge_runlist_1" $NSlashes "" kTRUE
      fi
  fi
  if [ $HAVELHC16j == 1 ]; then
      echo "downloading LHC16j"
      if [ $SINGLERUN == 1 ]; then
          runNumbers=`cat runlists/runNumbersLHC16j.txt`
          echo $runNumbers
          for runNumber in $runNumbers; do
              CopyFileIfNonExisitent $OUTPUTDIR_LHC16j/$runNumber "/alice/data/2016/LHC16j/000$runNumber/pass$passNr/PWGGA/GA_pp/$LHC16jData" $NSlashes3 "/alice/data/2016/LHC16j/000$runNumber/pass$passNr/PWGGA/GA_pp/$LHC16jData/" kTRUE
          done;
          if [ $MERGEONSINGLEData == 1 ] && [ ! -f $OUTPUTDIR_LHC16j/mergedAllCalo.txt ]; then
              rm $OUTPUTDIR_LHC16j/GammaCalo*.root*
              firstrunNumber=`head -n1 runlists/runNumbersLHC16j_pass1.txt`
              ls $OUTPUTDIR_LHC16j/$firstrunNumber/GammaCalo_*.root > fileLHC16j.txt

              MergeAccordingToSpecificRunlist fileLHC16j.txt $OUTPUTDIR_LHC16i $NSlashes3 GammaCalo All runlists/runNumbersLHC16j_pass1.txt
              MergeAccordingToSpecificRunlist fileLHC16j.txt $OUTPUTDIR_LHC16i $NSlashes3 GammaCalo DPGTrack runlists/runNumbersLHC16j_pass1_DPG.txt
              MergeAccordingToSpecificRunlist fileLHC16j.txt $OUTPUTDIR_LHC16i $NSlashes3 GammaCalo DPGTrackAndCalo runlists/runNumbersLHC16j_pass1_DPGTrackAndCalo.txt
          fi
      else
          CopyFileIfNonExisitent $OUTPUTDIR_LHC16j "/alice/cern.ch/user/a/alitrain/PWGGA/GA_pp/$LHC16jData/merge_runlist_1" $NSlashes "" kTRUE
      fi
  fi
    if [ $HAVELHC16k == 1 ]; then
        echo "downloading LHC16k"
        if [ $SINGLERUN == 1 ]; then
            runNumbers=`cat runlists/runNumbersLHC16k.txt`
            echo $runNumbers
            for runNumber in $runNumbers; do
                CopyFileIfNonExisitent $OUTPUTDIR_LHC16k/$runNumber "/alice/data/2016/LHC16k/000$runNumber/pass$passNr/PWGGA/GA_pp/$LHC16kData" $NSlashes3 "/alice/data/2016/LHC16k/000$runNumber/pass$passNr/PWGGA/GA_pp/$LHC16kData/" kTRUE
            done;
            if [ $MERGEONSINGLEData == 1 ] && [ ! -f $OUTPUTDIR_LHC16k/mergedAllCalo.txt ]; then
                rm $OUTPUTDIR_LHC16k/GammaCalo*.root*
                firstrunNumber=`head -n1 runlists/runNumbersLHC16k_pass1.txt`
                ls $OUTPUTDIR_LHC16k/$firstrunNumber/GammaCalo_*.root > fileLHC16k.txt

                MergeAccordingToSpecificRunlist fileLHC16k.txt $OUTPUTDIR_LHC16k $NSlashes3 GammaCalo All runlists/runNumbersLHC16k_pass1.txt
                MergeAccordingToSpecificRunlist fileLHC16k.txt $OUTPUTDIR_LHC16k $NSlashes3 GammaCalo DPGTrack runlists/runNumbersLHC16k_pass1_DPG.txt
                MergeAccordingToSpecificRunlist fileLHC16k.txt $OUTPUTDIR_LHC16k $NSlashes3 GammaCalo DPGTrackAndCalo runlists/runNumbersLHC16k_pass1_DPGTrackAndCalo.txt
            fi
        else
            CopyFileIfNonExisitent $OUTPUTDIR_LHC16k "/alice/cern.ch/user/a/alitrain/PWGGA/GA_pp/$LHC16kData/merge_runlist_1" $NSlashes "" kTRUE
        fi
    fi
    if [ $HAVELHC16l == 1 ]; then
        echo "downloading LHC16l"
        if [ $SINGLERUN == 1 ]; then
            runNumbers=`cat runlists/runNumbersLHC16l_pass1.txt`
            echo $runNumbers
            for runNumber in $runNumbers; do
                CopyFileIfNonExisitent $OUTPUTDIR_LHC16l/$runNumber "/alice/data/2016/LHC16l/000$runNumber/pass$passNr/PWGGA/GA_pp/$LHC16lData" $NSlashes3 "/alice/data/2016/LHC16l/000$runNumber/pass$passNr/PWGGA/GA_pp/$LHC16lData/" kTRUE
            done;
            if [ $MERGEONSINGLEData == 1 ] && [ ! -f $OUTPUTDIR_LHC16l/mergedAllCalo.txt ]; then
                rm $OUTPUTDIR_LHC16l/GammaCalo*.root*
                firstrunNumber=`head -n1 runlists/runNumbersLHC16l_pass1.txt`
                ls $OUTPUTDIR_LHC16l/$firstrunNumber/GammaCalo_*.root > fileLHC16l.txt
                fileNumbers=`cat fileLHC16l.txt`
                MergeAccordingToSpecificRunlist fileLHC16l.txt $OUTPUTDIR_LHC16l $NSlashes3 GammaCalo All runlists/runNumbersLHC16l_pass1.txt
                MergeAccordingToSpecificRunlist fileLHC16l.txt $OUTPUTDIR_LHC16l $NSlashes3 GammaCalo DPGTrack runlists/runNumbersLHC16l_pass1_DPG.txt
                MergeAccordingToSpecificRunlist fileLHC16l.txt $OUTPUTDIR_LHC16l $NSlashes3 GammaCalo DPGTrackAndCalo runlists/runNumbersLHC16l_pass1_DPGTrackAndCalo.txt
            fi
        else
            CopyFileIfNonExisitent $OUTPUTDIR_LHC16l "/alice/cern.ch/user/a/alitrain/PWGGA/GA_pp/$LHC16lData/merge_runlist_1" $NSlashes "" kTRUE
        fi
    fi
    if [ $HAVELHC16o == 1 ]; then
        echo "downloading LHC16o"
        if [ $SINGLERUN == 1 ]; then
            runNumbers=`cat runlists/runNumbersLHC16o_pass1.txt`
            echo $runNumbers
            for runNumber in $runNumbers; do
                CopyFileIfNonExisitent $OUTPUTDIR_LHC16o/$runNumber "/alice/data/2016/LHC16o/000$runNumber/pass$passNr/PWGGA/GA_pp/$LHC16oData" $NSlashes3 "/alice/data/2016/LHC16o/000$runNumber/pass$passNr/PWGGA/GA_pp/$LHC16oData/" kTRUE
            done;
            if [ $MERGEONSINGLEData == 1 ] && [ ! -f $OUTPUTDIR_LHC16o/mergedAllCalo.txt ]; then
                rm $OUTPUTDIR_LHC16o/GammaCalo*.root*
                firstrunNumber=`head -n1 runlists/runNumbersLHC16o_pass1.txt`
                ls $OUTPUTDIR_LHC16l/$firstrunNumber/GammaCalo_*.root > fileLHC16o.txt
                fileNumbers=`cat fileLHC16o.txt`
                MergeAccordingToSpecificRunlist fileLHC16o.txt $OUTPUTDIR_LHC16o $NSlashes3 GammaCalo All runlists/runNumbersLHC16o_pass1.txt
                MergeAccordingToSpecificRunlist fileLHC16o.txt $OUTPUTDIR_LHC16o $NSlashes3 GammaCalo DPGTrack runlists/runNumbersLHC16o_pass1_DPG.txt
                MergeAccordingToSpecificRunlist fileLHC16o.txt $OUTPUTDIR_LHC16o $NSlashes3 GammaCalo DPGTrackAndCalo runlists/runNumbersLHC16o_pass1_DPGTrackAndCalo.txt
            fi
        else
            CopyFileIfNonExisitent $OUTPUTDIR_LHC16o "/alice/cern.ch/user/a/alitrain/PWGGA/GA_pp/$LHC16oData/merge_runlist_1" $NSlashes "" kTRUE
        fi
    fi
    if [ $HAVELHC16p == 1 ]; then
        echo "downloading LHC16p"
        if [ $SINGLERUN == 1 ]; then
            runNumbers=`cat runlists/runNumbersLHC16p_pass1.txt`
            echo $runNumbers
            for runNumber in $runNumbers; do
                CopyFileIfNonExisitent $OUTPUTDIR_LHC16p/$runNumber "/alice/data/2016/LHC16p/000$runNumber/pass$passNr/PWGGA/GA_pp/$LHC16pData" $NSlashes3 "/alice/data/2016/LHC16p/000$runNumber/pass$passNr/PWGGA/GA_pp/$LHC16pData/" kTRUE
            done;
            if [ $MERGEONSINGLEData == 1 ] && [ ! -f $OUTPUTDIR_LHC16p/mergedAllCalo.txt ]; then
                rm $OUTPUTDIR_LHC16p/GammaCalo*.root*
                firstrunNumber=`head -n1 runlists/runNumbersLHC16p_pass1.txt`
                ls $OUTPUTDIR_LHC16p/$firstrunNumber/GammaCalo_*.root > fileLHC16p.txt
                fileNumbers=`cat fileLHC16p.txt`
                MergeAccordingToSpecificRunlist fileLHC16p.txt $OUTPUTDIR_LHC16p $NSlashes3 GammaCalo All runlists/runNumbersLHC16p_pass1.txt
                MergeAccordingToSpecificRunlist fileLHC16p.txt $OUTPUTDIR_LHC16p $NSlashes3 GammaCalo DPGTrack runlists/runNumbersLHC16p_pass1_DPG.txt
                MergeAccordingToSpecificRunlist fileLHC16p.txt $OUTPUTDIR_LHC16p $NSlashes3 GammaCalo DPGTrackAndCalo runlists/runNumbersLHC16p_pass1_DPGTrackAndCalo.txt
            fi
        else
            CopyFileIfNonExisitent $OUTPUTDIR_LHC16p "/alice/cern.ch/user/a/alitrain/PWGGA/GA_pp/$LHC16pData/merge_runlist_1" $NSlashes "" kTRUE
        fi
    fi
    if [ $HAVELHC16e == 1 ]; then
        echo "downloading LHC16e"
        if [ $SINGLERUN == 1 ]; then
            runNumbers=`cat runlists/runNumbersLHC16e_pass1.txt`
            echo $runNumbers
            for runNumber in $runNumbers; do
                CopyFileIfNonExisitent $OUTPUTDIR_LHC16e/$runNumber "/alice/data/2016/LHC16e/000$runNumber/pass$passNr/PWGGA/GA_pp/$LHC16eData" $NSlashes3 "/alice/data/2016/LHC16e/000$runNumber/pass$passNr/PWGGA/GA_pp/$LHC16eData/" kTRUE
            done;
            if [ $MERGEONSINGLEData == 1 ] && [ ! -f $OUTPUTDIR_LHC16e/mergedAllCalo.txt ]; then
                rm $OUTPUTDIR_LHC16e/GammaCalo*.root*
                firstrunNumber=`head -n1 runlists/runNumbersLHC16e_pass1.txt`
                ls $OUTPUTDIR_LHC16e/$firstrunNumber/GammaCalo_*.root > fileLHC16e.txt
                fileNumbers=`cat fileLHC16p.txt`
                MergeAccordingToSpecificRunlist fileLHC16e.txt $OUTPUTDIR_LHC16e $NSlashes3 GammaCalo All runlists/runNumbersLHC16e_pass1.txt
                MergeAccordingToSpecificRunlist fileLHC16e.txt $OUTPUTDIR_LHC16e $NSlashes3 GammaCalo DPGTrack runlists/runNumbersLHC16e_pass1_DPG.txt
                MergeAccordingToSpecificRunlist fileLHC16e.txt $OUTPUTDIR_LHC16e $NSlashes3 GammaCalo DPGTrackAndCalo runlists/runNumbersLHC16e_pass1_DPGTrackAndCalo.txt
            fi
        else
            CopyFileIfNonExisitent $OUTPUTDIR_LHC16p "/alice/cern.ch/user/a/alitrain/PWGGA/GA_pp/$LHC16pData/merge_runlist_1" $NSlashes "" kTRUE
        fi
    fi

    currentDir=$PWD
    if [ $HAVELHC17d20a1 == 1 ]; then
        echo "downloading LHC17d20a1"
        if [ $SINGLERUN == 1 ]; then
            runNumbers=`cat runlists/runNumbersLHC17d20a1.txt`
            echo $runNumbers
            for runNumber in $runNumbers; do
                CopyFileIfNonExisitent $OUTPUTDIR_LHC17d20a1/$runNumber "/alice/sim/2017/LHC17d20a1/$runNumber/PWGGA/GA_pp_MC/$LHC17d20a1MC" $NSlashes3 "/alice/sim/2017/LHC17d20a1/$runNumber/PWGGA/GA_pp_MC/$LHC17d20a1MC/Stage_1/" kTRUE
            done;
            if [ $MERGEONSINGLEMC == 1 ] && [ ! -f $OUTPUTDIR_LHC17d20a1/mergedAllConv.txt ]; then
                cd $currentDir
                rm $OUTPUTDIR_LHC17d20a1/GammaCalo*.root*
                echo runlists/runNumbersLHC17d20a1.txt
                firstrunNumber=`head -n1 runlists/runNumbersLHC17d20a1.txt`
                echo $firstrunNumber
                ls $OUTPUTDIR_LHC17d20a1/$firstrunNumber/GammaCalo_*.root > fileLHC17d20a1.txt
                MergeAccordingToSpecificRunlist fileLHC17d20a1.txt $OUTPUTDIR_LHC17d20a1 $NSlashes3 GammaCalo All runlists/runNumbersLHC17d20a1.txt
                MergeAccordingToSpecificRunlist fileLHC17d20a1.txt $OUTPUTDIR_LHC17d20a1 $NSlashes3 GammaCalo DPGTrack runlists/runNumbersLHC17d20a1_DPG.txt
                MergeAccordingToSpecificRunlist fileLHC17d20a1.txt $OUTPUTDIR_LHC17d20a1 $NSlashes3 GammaCalo DPGTrackAndCalo runlists/runNumbersLHC17d20a1_DPGTrackAndCalo.txt
            fi
        else
            CopyFileIfNonExisitent $OUTPUTDIR_LHC17d20a1 "/alice/cern.ch/user/a/alitrain/PWGGA/GA_pp_MC/$LHC17d20a1MC/merge" $NSlashes "" kTRUE
        fi
    fi
    if [ $HAVELHC17d20a1Ex == 1 ]; then
        echo "downloading LHC17d20a1_extra"
        if [ $SINGLERUN == 1 ]; then
            runNumbers=`cat runlists/runNumbersLHC17d20a1_extra.txt`
            echo $runNumbers
            for runNumber in $runNumbers; do
                CopyFileIfNonExisitent $OUTPUTDIR_LHC17d20a1Ex/$runNumber "/alice/sim/2017/LHC17d20a1_extra/$runNumber/PWGGA/GA_pp_MC/$LHC17d20a1ExMC" $NSlashes3 "/alice/sim/2017/LHC17d20a1_extra/$runNumber/PWGGA/GA_pp_MC/$LHC17d20a1ExMC/Stage_1/" kTRUE
            done;
            if [ $MERGEONSINGLEMC == 1 ]  && [ ! -f $OUTPUTDIR_LHC17d20a1Ex/mergedAllCalo.txt ]; then
                cd $currentDir
                rm $OUTPUTDIR_LHC17d20a1Ex/GammaCalo*.root*
                firstrunNumber=`head -n1 runlists/runNumbersLHC17d20a1_extra.txt`
                ls $OUTPUTDIR_LHC17d20a1Ex/$firstrunNumber/GammaCalo_*.root > fileLHC17d20a1_extra.txt
                fileNumbers=`cat fileLHC17d20a1_extra.txt`
                MergeAccordingToSpecificRunlist fileLHC17d20a1_extra.txt $OUTPUTDIR_LHC17d20a1Ex $NSlashes3 GammaCalo All runlists/runNumbersLHC17d20a1_extra.txt
                MergeAccordingToSpecificRunlist fileLHC17d20a1_extra.txt $OUTPUTDIR_LHC17d20a1Ex $NSlashes3 GammaCalo DPGTrack runlists/runNumbersLHC17d20a1_extra_DPG.txt
                MergeAccordingToSpecificRunlist fileLHC17d20a1_extra.txt $OUTPUTDIR_LHC17d20a1Ex $NSlashes3 GammaCalo DPGTrackAndCalo runlists/runNumbersLHC17d20a1_extra_DPGTrackAndCalo.txt
            fi
        else
            CopyFileIfNonExisitent $OUTPUTDIR_LHC17d20a1Ex "/alice/cern.ch/user/a/alitrain/PWGGA/GA_pp_MC/$LHC17d20a1ExMC/merge" $NSlashes "" kTRUE
        fi
    fi
    if [ $HAVELHC17d20a2 == 1 ]; then
        echo "downloading LHC17d20a2"
        if [ $SINGLERUN == 1 ]; then
            runNumbers=`cat runlists/runNumbersLHC17d20a2.txt`
            echo $runNumbers
            for runNumber in $runNumbers; do
                CopyFileIfNonExisitent $OUTPUTDIR_LHC17d20a2/$runNumber "/alice/sim/2017/LHC17d20a2/$runNumber/PWGGA/GA_pp_MC/$LHC17d20a2MC" $NSlashes3 "/alice/sim/2017/LHC17d20a2/$runNumber/PWGGA/GA_pp_MC/$LHC17d20a2MC/Stage_1/" kTRUE
            done;
            if [ $MERGEONSINGLEMC == 1 ] && [ ! -f $OUTPUTDIR_LHC17d20a2/mergedAllConv.txt ]; then
                cd $currentDir
                rm $OUTPUTDIR_LHC17d20a2/GammaCalo*.root*
                echo runlists/runNumbersLHC17d20a2.txt
                firstrunNumber=`head -n1 runlists/runNumbersLHC17d20a2.txt`
                echo $firstrunNumber
                ls $OUTPUTDIR_LHC17d20a2/$firstrunNumber/GammaCalo_*.root > fileLHC17d20a2.txt
                MergeAccordingToSpecificRunlist fileLHC17d20a2.txt $OUTPUTDIR_LHC17d20a2 $NSlashes3 GammaCalo All runlists/runNumbersLHC17d20a2.txt
                MergeAccordingToSpecificRunlist fileLHC17d20a2.txt $OUTPUTDIR_LHC17d20a2 $NSlashes3 GammaCalo DPGTrack runlists/runNumbersLHC17d20a2_DPG.txt
                MergeAccordingToSpecificRunlist fileLHC17d20a2.txt $OUTPUTDIR_LHC17d20a2 $NSlashes3 GammaCalo DPGTrackAndCalo runlists/runNumbersLHC17d20a2_DPG.txt
            fi
        else
            CopyFileIfNonExisitent $OUTPUTDIR_LHC17d20a2 "/alice/cern.ch/user/a/alitrain/PWGGA/GA_pp_MC/$LHC17d20a2MC/merge" $NSlashes "" kTRUE
        fi
    fi
    if [ $HAVELHC17d20a2Ex == 1 ]; then
        echo "downloading LHC17d20a2_extra"
        if [ $SINGLERUN == 1 ]; then
            runNumbers=`cat runlists/runNumbersLHC17d20a2_extra.txt`
            echo $runNumbers
            for runNumber in $runNumbers; do
                CopyFileIfNonExisitent $OUTPUTDIR_LHC17d20a2Ex/$runNumber "/alice/sim/2017/LHC17d20a2_extra/$runNumber/PWGGA/GA_pp_MC/$LHC17d20a2ExMC" $NSlashes3 "/alice/sim/2017/LHC17d20a2_extra/$runNumber/PWGGA/GA_pp_MC/$LHC17d20a2ExMC/Stage_1/" kTRUE
            done;
            if [ $MERGEONSINGLEMC == 1 ]  && [ ! -f $OUTPUTDIR_LHC17d20a2Ex/mergedAllCalo.txt ]; then
                cd $currentDir
                rm $OUTPUTDIR_LHC17d20a2Ex/GammaCalo*.root*
                firstrunNumber=`head -n1 runlists/runNumbersLHC17d20a2_extra.txt`
                ls $OUTPUTDIR_LHC17d20a2Ex/$firstrunNumber/GammaCalo_*.root > fileLHC17d20a2_extra.txt
                fileNumbers=`cat fileLHC17d20a2_extra.txt`
                MergeAccordingToSpecificRunlist fileLHC17d20a2_extra.txt $OUTPUTDIR_LHC17d20a2Ex $NSlashes3 GammaCalo All runlists/runNumbersLHC17d20a2_extra.txt
                MergeAccordingToSpecificRunlist fileLHC17d20a2_extra.txt $OUTPUTDIR_LHC17d20a2Ex $NSlashes3 GammaCalo DPGTrack runlists/runNumbersLHC17d20a2_extra_DPG.txt
                MergeAccordingToSpecificRunlist fileLHC17d20a2_extra.txt $OUTPUTDIR_LHC17d20a2Ex $NSlashes3 GammaCalo DPGTrackAndCalo runlists/runNumbersLHC17d20a2_extra_DPGTrackAndCalo.txt
            fi
        else
            CopyFileIfNonExisitent $OUTPUTDIR_LHC17d20a2Ex "/alice/cern.ch/user/a/alitrain/PWGGA/GA_pp_MC/$LHC17d20a2ExMC/merge" $NSlashes "" kTRUE
        fi
    fi

    if [ $HAVELHC16d == 1 ]; then
        ls $OUTPUTDIR_LHC16d/GammaCalo-All_*.root > fileLHC16d.txt
        fileNumbers=`cat fileLHC16d.txt`
        for fileName in $fileNumbers; do
            echo $fileName
            ChangeStructureIfNeededCalo $fileName $OUTPUTDIR_LHC16d $NSlashes "LHC16d-pass$passNr-All" "-All"
        done;
        ls $OUTPUTDIR_LHC16d/GammaCalo-DPGTrack_*.root > fileLHC16d.txt
        fileNumbers=`cat fileLHC16d.txt`
        for fileName in $fileNumbers; do
            echo $fileName
            ChangeStructureIfNeededCalo $fileName $OUTPUTDIR_LHC16d $NSlashes "LHC16d-pass$passNr-DPGTrack" "-DPGTrack"
        done;
        ls $OUTPUTDIR_LHC16d/GammaCalo-DPGTrackAndCalo_*.root > fileLHC16d.txt
        fileNumbers=`cat fileLHC16d.txt`
        for fileName in $fileNumbers; do
            echo $fileName
            ChangeStructureIfNeededCalo $fileName $OUTPUTDIR_LHC16d $NSlashes "LHC16d-pass$passNr-DPGTrackAndCalo" "-DPGTrackAndCalo"
        done;
    fi

    if [ $HAVELHC16g == 1 ]; then
        ls $OUTPUTDIR_LHC16g/GammaCalo-All_*.root > fileLHC16g.txt
        fileNumbers=`cat fileLHC16g.txt`
        for fileName in $fileNumbers; do
            echo $fileName
            ChangeStructureIfNeededCalo $fileName $OUTPUTDIR_LHC16g $NSlashes "LHC16g-pass$passNr-All" "-All"
        done;
        ls $OUTPUTDIR_LHC16g/GammaCalo-DPGTrack_*.root > fileLHC16g.txt
        fileNumbers=`cat fileLHC16g.txt`
        for fileName in $fileNumbers; do
            echo $fileName
            ChangeStructureIfNeededCalo $fileName $OUTPUTDIR_LHC16g $NSlashes "LHC16g-pass$passNr-DPGTrack" "-DPGTrack"
        done;
        ls $OUTPUTDIR_LHC16g/GammaCalo-DPGTrackAndCalo_*.root > fileLHC16g.txt
        fileNumbers=`cat fileLHC16g.txt`
        for fileName in $fileNumbers; do
            echo $fileName
            ChangeStructureIfNeededCalo $fileName $OUTPUTDIR_LHC16g $NSlashes "LHC16g-pass$passNr-DPGTrackAndCalo" "-DPGTrackAndCalo"
        done;
    fi

    if [ $HAVELHC16h == 1 ]; then
        ls $OUTPUTDIR_LHC16h/GammaCalo-All_*.root > fileLHC16h.txt
        fileNumbers=`cat fileLHC16h.txt`
        for fileName in $fileNumbers; do
            echo $fileName
            ChangeStructureIfNeededCalo $fileName $OUTPUTDIR_LHC16h $NSlashes "LHC16h-pass$passNr-All" "-All"
        done;
        ls $OUTPUTDIR_LHC16h/GammaCalo-DPGTrack_*.root > fileLHC16h.txt
        fileNumbers=`cat fileLHC16h.txt`
        for fileName in $fileNumbers; do
            echo $fileName
            ChangeStructureIfNeededCalo $fileName $OUTPUTDIR_LHC16h $NSlashes "LHC16h-pass$passNr-DPGTrack" "-DPGTrack"
        done;
        ls $OUTPUTDIR_LHC16h/GammaCalo-DPGTrackAndCalo_*.root > fileLHC16h.txt
        fileNumbers=`cat fileLHC16h.txt`
        for fileName in $fileNumbers; do
            echo $fileName
            ChangeStructureIfNeededCalo $fileName $OUTPUTDIR_LHC16h $NSlashes "LHC16h-pass$passNr-DPGTrackAndCalo" "-DPGTrackAndCalo"
        done;
    fi

    if [ $HAVELHC16i == 1 ]; then
        ls $OUTPUTDIR_LHC16i/GammaCalo-All_*.root > fileLHC16i.txt
        fileNumbers=`cat fileLHC16i.txt`
        for fileName in $fileNumbers; do
            echo $fileName
            ChangeStructureIfNeededCalo $fileName $OUTPUTDIR_LHC16i $NSlashes "LHC16i-pass$passNr-All" "-All"
        done;
        ls $OUTPUTDIR_LHC16i/GammaCalo-DPGTrack_*.root > fileLHC16i.txt
        fileNumbers=`cat fileLHC16i.txt`
        for fileName in $fileNumbers; do
            echo $fileName
            ChangeStructureIfNeededCalo $fileName $OUTPUTDIR_LHC16i $NSlashes "LHC16i-pass$passNr-DPGTrack" "-DPGTrack"
        done;
        ls $OUTPUTDIR_LHC16i/GammaCalo-DPGTrackAndCalo_*.root > fileLHC16i.txt
        fileNumbers=`cat fileLHC16i.txt`
        for fileName in $fileNumbers; do
            echo $fileName
            ChangeStructureIfNeededCalo $fileName $OUTPUTDIR_LHC16i $NSlashes "LHC16i-pass$passNr-DPGTrackAndCalo" "-DPGTrackAndCalo"
        done;
    fi

    if [ $HAVELHC16j == 1 ]; then
        ls $OUTPUTDIR_LHC16j/GammaCalo-All_*.root > fileLHC16j.txt
        fileNumbers=`cat fileLHC16j.txt`
        for fileName in $fileNumbers; do
            echo $fileName
            ChangeStructureIfNeededCalo $fileName $OUTPUTDIR_LHC16j $NSlashes "LHC16j-pass$passNr-All" "-All"
        done;
        ls $OUTPUTDIR_LHC16j/GammaCalo-DPGTrack_*.root > fileLHC16j.txt
        fileNumbers=`cat fileLHC16j.txt`
        for fileName in $fileNumbers; do
            echo $fileName
            ChangeStructureIfNeededCalo $fileName $OUTPUTDIR_LHC16j $NSlashes "LHC16j-pass$passNr-DPGTrack" "-DPGTrack"
        done;
        ls $OUTPUTDIR_LHC16j/GammaCalo-DPGTrackAndCalo_*.root > fileLHC16j.txt
        fileNumbers=`cat fileLHC16j.txt`
        for fileName in $fileNumbers; do
            echo $fileName
            ChangeStructureIfNeededCalo $fileName $OUTPUTDIR_LHC16j $NSlashes "LHC16j-pass$passNr-DPGTrackAndCalo" "-DPGTrackAndCalo"
        done;
    fi
    if [ $HAVELHC16k == 1 ]; then
        ls $OUTPUTDIR_LHC16k/GammaCalo-All_*.root > fileLHC16k.txt
        fileNumbers=`cat fileLHC16k.txt`
        for fileName in $fileNumbers; do
            echo $fileName
            ChangeStructureIfNeededCalo $fileName $OUTPUTDIR_LHC16k $NSlashes "LHC16k-pass$passNr-All" "-All"
        done;
        ls $OUTPUTDIR_LHC16k/GammaCalo-DPGTrack_*.root > fileLHC16k.txt
        fileNumbers=`cat fileLHC16k.txt`
        for fileName in $fileNumbers; do
            echo $fileName
            ChangeStructureIfNeededCalo $fileName $OUTPUTDIR_LHC16k $NSlashes "LHC16k-pass$passNr-DPGTrack" "-DPGTrack"
        done;
        ls $OUTPUTDIR_LHC16k/GammaCalo-DPGTrackAndCalo_*.root > fileLHC16k.txt
        fileNumbers=`cat fileLHC16k.txt`
        for fileName in $fileNumbers; do
            echo $fileName
            ChangeStructureIfNeededCalo $fileName $OUTPUTDIR_LHC16k $NSlashes "LHC16k-pass$passNr-DPGTrackAndCalo" "-DPGTrackAndCalo"
        done;
    fi

    if [ $HAVELHC16l == 1 ]; then
        ls $OUTPUTDIR_LHC16l/GammaCalo-All_*.root > fileLHC16l.txt
        fileNumbers=`cat fileLHC16l.txt`
        for fileName in $fileNumbers; do
            echo $fileName
            ChangeStructureIfNeededCalo $fileName $OUTPUTDIR_LHC16l $NSlashes "LHC16l-pass$passNr-All" "-All"
        done;
        ls $OUTPUTDIR_LHC16l/GammaCalo-DPGTrack_*.root > fileLHC16l.txt
        fileNumbers=`cat fileLHC16l.txt`
        for fileName in $fileNumbers; do
            echo $fileName
            ChangeStructureIfNeededCalo $fileName $OUTPUTDIR_LHC16l $NSlashes "LHC16l-pass$passNr-DPGTrack" "-DPGTrack"
        done;
        ls $OUTPUTDIR_LHC16l/GammaCalo-DPGTrackAndCalo_*.root > fileLHC16l.txt
        fileNumbers=`cat fileLHC16l.txt`
        for fileName in $fileNumbers; do
            echo $fileName
            ChangeStructureIfNeededCalo $fileName $OUTPUTDIR_LHC16l $NSlashes "LHC16l-pass$passNr-DPGTrackAndCalo" "-DPGTrackAndCalo"
        done;
    fi
    if [ $HAVELHC16o == 1 ]; then
        ls $OUTPUTDIR_LHC16o/GammaCalo-All_*.root > fileLHC16o.txt
        fileNumbers=`cat fileLHC16o.txt`
        for fileName in $fileNumbers; do
            echo $fileName
            ChangeStructureIfNeededCalo $fileName $OUTPUTDIR_LHC16o $NSlashes "LHC16o-pass$passNr-All" "-All"
        done;
        ls $OUTPUTDIR_LHC16o/GammaCalo-DPGTrack_*.root > fileLHC16o.txt
        fileNumbers=`cat fileLHC16o.txt`
        for fileName in $fileNumbers; do
            echo $fileName
            ChangeStructureIfNeededCalo $fileName $OUTPUTDIR_LHC16o $NSlashes "LHC16o-pass$passNr-DPGTrack" "-DPGTrack"
        done;
        ls $OUTPUTDIR_LHC16o/GammaCalo-DPGTrackAndCalo_*.root > fileLHC16o.txt
        fileNumbers=`cat fileLHC16o.txt`
        for fileName in $fileNumbers; do
            echo $fileName
            ChangeStructureIfNeededCalo $fileName $OUTPUTDIR_LHC16o $NSlashes "LHC16o-pass$passNr-DPGTrackAndCalo" "-DPGTrackAndCalo"
        done;
    fi

    if [ $HAVELHC16p == 1 ]; then
        ls $OUTPUTDIR_LHC16p/GammaCalo-All_*.root > fileLHC16p.txt
        fileNumbers=`cat fileLHC16p.txt`
        for fileName in $fileNumbers; do
            echo $fileName
            ChangeStructureIfNeededCalo $fileName $OUTPUTDIR_LHC16p $NSlashes "LHC16p-pass$passNr-All" "-All"
        done;
        ls $OUTPUTDIR_LHC16p/GammaCalo-DPGTrack_*.root > fileLHC16p.txt
        fileNumbers=`cat fileLHC16p.txt`
        for fileName in $fileNumbers; do
            echo $fileName
            ChangeStructureIfNeededCalo $fileName $OUTPUTDIR_LHC16p $NSlashes "LHC16p-pass$passNr-DPGTrack" "-DPGTrack"
        done;
        ls $OUTPUTDIR_LHC16p/GammaCalo-DPGTrackAndCalo_*.root > fileLHC16p.txt
        fileNumbers=`cat fileLHC16p.txt`
        for fileName in $fileNumbers; do
            echo $fileName
            ChangeStructureIfNeededCalo $fileName $OUTPUTDIR_LHC16p $NSlashes "LHC16p-pass$passNr-DPGTrackAndCalo" "-DPGTrackAndCalo"
        done;
    fi

    if [ $HAVELHC16e == 1 ]; then
        ls $OUTPUTDIR_LHC16e/GammaCalo-All_*.root > fileLHC16e.txt
        fileNumbers=`cat fileLHC16e.txt`
        for fileName in $fileNumbers; do
            echo $fileName
            ChangeStructureIfNeededCalo $fileName $OUTPUTDIR_LHC16e $NSlashes "LHC16e-pass$passNr-All" "-All"
        done;
        ls $OUTPUTDIR_LHC16e/GammaCalo-DPGTrack_*.root > fileLHC16e.txt
        fileNumbers=`cat fileLHC16e.txt`
        for fileName in $fileNumbers; do
            echo $fileName
            ChangeStructureIfNeededCalo $fileName $OUTPUTDIR_LHC16e $NSlashes "LHC16e-pass$passNr-DPGTrack" "-DPGTrack"
        done;
        ls $OUTPUTDIR_LHC16e/GammaCalo-DPGTrackAndCalo_*.root > fileLHC16e.txt
        fileNumbers=`cat fileLHC16e.txt`
        for fileName in $fileNumbers; do
            echo $fileName
            ChangeStructureIfNeededCalo $fileName $OUTPUTDIR_LHC16e $NSlashes "LHC16e-pass$passNr-DPGTrackAndCalo" "-DPGTrackAndCalo"
        done;
    fi

    if [ $HAVELHC17d20a1 == 1 ]; then
        ls $OUTPUTDIR_LHC17d20a1/GammaCalo-All_*.root > fileLHC17d20a1.txt
        fileNumbers=`cat fileLHC17d20a1.txt`
        for fileName in $fileNumbers; do
            echo $fileName
            ChangeStructureIfNeededCalo $fileName $OUTPUTDIR_LHC17d20a1 $NSlashes "MC_LHC17d20a1-anchor16k-All" "-All"
        done;
        ls $OUTPUTDIR_LHC17d20a1/GammaCalo-DPGTrack_*.root > fileLHC17d20a1.txt
        fileNumbers=`cat fileLHC17d20a1.txt`
        for fileName in $fileNumbers; do
            echo $fileName
            ChangeStructureIfNeededCalo $fileName $OUTPUTDIR_LHC17d20a1 $NSlashes "MC_LHC17d20a1-anchor16k-DPGTrack" "-DPGTrack"
        done;
        ls $OUTPUTDIR_LHC17d20a1/GammaCalo-DPGTrackAndCalo_*.root > fileLHC17d20a1.txt
        fileNumbers=`cat fileLHC17d20a1.txt`
        for fileName in $fileNumbers; do
            echo $fileName
            ChangeStructureIfNeededCalo $fileName $OUTPUTDIR_LHC17d20a1 $NSlashes "MC_LHC17d20a1-anchor16k-DPGTrackAndCalo" "-DPGTrackAndCalo"
        done;
    fi

    if [ $HAVELHC17d20a1Ex == 1 ]; then
        ls $OUTPUTDIR_LHC17d20a1Ex/GammaCalo-All_*.root > fileLHC17d20a1_extra.txt
        fileNumbers=`cat fileLHC17d20a1_extra.txt`
        for fileName in $fileNumbers; do
            echo $fileName
            ChangeStructureIfNeededCalo $fileName $OUTPUTDIR_LHC17d20a1Ex $NSlashes "MC_LHC17d20a1_extra-anchor16k-All" "-All"
        done;
        ls $OUTPUTDIR_LHC17d20a1Ex/GammaCalo-DPGTrack_*.root > fileLHC17d20a1_extra.txt
        fileNumbers=`cat fileLHC17d20a1_extra.txt`
        for fileName in $fileNumbers; do
            echo $fileName
            ChangeStructureIfNeededCalo $fileName $OUTPUTDIR_LHC17d20a1Ex $NSlashes "MC_LHC17d20a1_extra-anchor16k-DPGTrack" "-DPGTrack"
        done;
        ls $OUTPUTDIR_LHC17d20a1Ex/GammaCalo-DPGTrackAndCalo*.root > fileLHC17d20a1_extra.txt
        fileNumbers=`cat fileLHC17d20a1_extra.txt`
        for fileName in $fileNumbers; do
            echo $fileName
            ChangeStructureIfNeededCalo $fileName $OUTPUTDIR_LHC17d20a1Ex $NSlashes "MC_LHC17d20a1_extra-anchor16k-DPGTrackAndCalo" "-DPGTrackAndCalo"
        done;
    fi
    if [ $HAVELHC17d20a2 == 1 ]; then
        ls $OUTPUTDIR_LHC17d20a2/GammaCalo-All_*.root > fileLHC17d20a2.txt
        fileNumbers=`cat fileLHC17d20a2.txt`
        for fileName in $fileNumbers; do
            echo $fileName
            ChangeStructureIfNeededCalo $fileName $OUTPUTDIR_LHC17d20a2 $NSlashes "MC_LHC17d20a2-anchor16l-All" "-All"
        done;
        ls $OUTPUTDIR_LHC17d20a2/GammaCalo-DPGTrack_*.root > fileLHC17d20a2.txt
        fileNumbers=`cat fileLHC17d20a2.txt`
        for fileName in $fileNumbers; do
            echo $fileName
            ChangeStructureIfNeededCalo $fileName $OUTPUTDIR_LHC17d20a2 $NSlashes "MC_LHC17d20a2-anchor16l-DPGTrack" "-DPGTrack"
        done;
        ls $OUTPUTDIR_LHC17d20a2/GammaCalo-DPGTrackAndCalo_*.root > fileLHC17d20a2.txt
        fileNumbers=`cat fileLHC17d20a2.txt`
        for fileName in $fileNumbers; do
            echo $fileName
            ChangeStructureIfNeededCalo $fileName $OUTPUTDIR_LHC17d20a2 $NSlashes "MC_LHC17d20a2-anchor16l-DPGTrackAndCalo" "-DPGTrackAndCalo"
        done;
    fi

    if [ $HAVELHC17d20a2Ex == 1 ]; then
        ls $OUTPUTDIR_LHC17d20a2Ex/GammaCalo-All_*.root > fileLHC17d20a2_extra.txt
        fileNumbers=`cat fileLHC17d20a2_extra.txt`
        for fileName in $fileNumbers; do
            echo $fileName
            ChangeStructureIfNeededCalo $fileName $OUTPUTDIR_LHC17d20a2Ex $NSlashes "MC_LHC17d20a2_extra-anchor16l-All" "-All"
        done;
        ls $OUTPUTDIR_LHC17d20a2Ex/GammaCalo-DPGTrack_*.root > fileLHC17d20a2_extra.txt
        fileNumbers=`cat fileLHC17d20a2_extra.txt`
        for fileName in $fileNumbers; do
            echo $fileName
            ChangeStructureIfNeededCalo $fileName $OUTPUTDIR_LHC17d20a2Ex $NSlashes "MC_LHC17d20a2_extra-anchor16l-DPGTrack" "-DPGTrack"
        done;
        ls $OUTPUTDIR_LHC17d20a2Ex/GammaCalo-DPGTrackAndCalo*.root > fileLHC17d20a2_extra.txt
        fileNumbers=`cat fileLHC17d20a2_extra.txt`
        for fileName in $fileNumbers; do
            echo $fileName
            ChangeStructureIfNeededCalo $fileName $OUTPUTDIR_LHC17d20a2Ex $NSlashes "MC_LHC17d20a2_extra-anchor16l-DPGTrackAndCalo" "-DPGTrackAndCalo"
        done;
    fi

    if [ $MERGEON == 1 ]; then
        ls $OUTPUTDIR/GammaCalo_LHC16o-pass$passNr-DPGTrackAndCalo\_*.root > filesForMerging.txt
        filesForMerging=`cat filesForMerging.txt`
        for fileName in $filesForMerging; do
            echo $fileName
            GetFileNumberMerging $fileName $((NSlashes-1)) 4
            echo $number
            ls $OUTPUTDIR/GammaCalo_LHC16o-pass$passNr-DPGTrackAndCalo\_$number.root
            ls $OUTPUTDIR/GammaCalo_LHC16k-pass$passNr-DPGTrackAndCalo\_$number.root
            ls $OUTPUTDIR/GammaCalo_LHC16l-pass$passNr-DPGTrackAndCalo\_$number.root
            if [ -f $OUTPUTDIR/GammaCalo_LHC16k-pass$passNr-DPGTrackAndCalo\_$number.root ] && [ -f $OUTPUTDIR/GammaCalo_LHC16l-pass$passNr-DPGTrackAndCalo\_$number.root ]  && [ -f $OUTPUTDIR/GammaCalo_LHC16o-pass$passNr-DPGTrackAndCalo\_$number.root ] ; then
                hadd -f $OUTPUTDIR/GammaCalo_LHC16klo-pass$passNr-DPGTrackAndCalo\_$number.root $OUTPUTDIR/GammaCalo_LHC16k-pass$passNr-DPGTrackAndCalo\_$number.root $OUTPUTDIR/GammaCalo_LHC16l-pass$passNr-DPGTrackAndCalo\_$number.root $OUTPUTDIR/GammaCalo_LHC16o-pass$passNr-DPGTrackAndCalo\_$number.root
            fi
        done
        ls $OUTPUTDIR/GammaCalo_LHC16o-pass$passNr-DPGTrack\_*.root > filesForMerging.txt
        filesForMerging=`cat filesForMerging.txt`
        for fileName in $filesForMerging; do
            echo $fileName
            GetFileNumberMerging $fileName $((NSlashes-1)) 4
            echo $number
            ls $OUTPUTDIR/GammaCalo_LHC16o-pass$passNr-DPGTrack\_$number.root
            ls $OUTPUTDIR/GammaCalo_LHC16k-pass$passNr-DPGTrack\_$number.root
            ls $OUTPUTDIR/GammaCalo_LHC16l-pass$passNr-DPGTrack\_$number.root
            if [ -f $OUTPUTDIR/GammaCalo_LHC16k-pass$passNr-DPGTrack\_$number.root ] && [ -f $OUTPUTDIR/GammaCalo_LHC16l-pass$passNr-DPGTrack\_$number.root ] && [ -f $OUTPUTDIR/GammaCalo_LHC16o-pass$passNr-DPGTrack\_$number.root ] ; then
                hadd -f $OUTPUTDIR/GammaCalo_LHC16klo-pass$passNr-DPGTrack\_$number.root $OUTPUTDIR/GammaCalo_LHC16k-pass$passNr-DPGTrack\_$number.root $OUTPUTDIR/GammaCalo_LHC16l-pass$passNr-DPGTrack\_$number.root $OUTPUTDIR/GammaCalo_LHC16o-pass$passNr-DPGTrack\_$number.root
            fi
        done
        ls $OUTPUTDIR/GammaCalo_LHC16o-pass$passNr-All\_*.root > filesForMerging.txt
        filesForMerging=`cat filesForMerging.txt`
        for fileName in $filesForMerging; do
            echo $fileName
            GetFileNumberMerging $fileName $((NSlashes-1)) 4
            echo $number
            ls $OUTPUTDIR/GammaCalo_LHC16o-pass$passNr-All\_$number.root
            ls $OUTPUTDIR/GammaCalo_LHC16k-pass$passNr-All\_$number.root
            ls $OUTPUTDIR/GammaCalo_LHC16l-pass$passNr-All\_$number.root
            if [ -f $OUTPUTDIR/GammaCalo_LHC16k-pass$passNr-All\_$number.root ] && [ -f $OUTPUTDIR/GammaCalo_LHC16l-pass$passNr-All\_$number.root ] && [ -f $OUTPUTDIR/GammaCalo_LHC16o-pass$passNr-All\_$number.root ]; then
                hadd -f $OUTPUTDIR/GammaCalo_LHC16klo-pass$passNr-All\_$number.root $OUTPUTDIR/GammaCalo_LHC16k-pass$passNr-All\_$number.root $OUTPUTDIR/GammaCalo_LHC16l-pass$passNr-All\_$number.root $OUTPUTDIR/GammaCalo_LHC16o-pass$passNr-All\_$number.root
            fi
        done



    fi
else
  if [ $HAVELHC16d == 1 ]; then
      echo "removing all GammaCalo files in runFolders for LHC16l";
      rm $OUTPUTDIR_LHC16d/*/GammaCalo_*.root
      rm $OUTPUTDIR_LHC16d/*/*/*GammaCalo_*.root
  fi
  if [ $HAVELHC16g == 1 ]; then
      echo "removing all GammaConv files in runFolders for LHC16g";
      rm $OUTPUTDIR_LHC16g/*/GammaCalo_*.root
      rm $OUTPUTDIR_LHC16g/*/*/*GammaCalo_*.root
  fi
  if [ $HAVELHC16h == 1 ]; then
      echo "removing all GammaCalo files in runFolders for LHC16h";
      rm $OUTPUTDIR_LHC16h/*/GammaCalo_*.root
      rm $OUTPUTDIR_LHC16h/*/*/*GammaCalo_*.root
  fi
  if [ $HAVELHC16i == 1 ]; then
      echo "removing all GammaCalo files in runFolders for LHC16i";
      rm $OUTPUTDIR_LHC16i/*/GammaCalo_*.root
      rm $OUTPUTDIR_LHC16i/*/*/*GammaCalo_*.root
  fi
  if [ $HAVELHC16j == 1 ]; then
      echo "removing all GammaCalo files in runFolders for LHC16j";
      rm $OUTPUTDIR_LHC16j/*/GammaCalo_*.root
      rm $OUTPUTDIR_LHC16j/*/*/*GammaCalo_*.root
  fi
    if [ $HAVELHC16k == 1 ]; then
        echo "removing all GammaCalo files in runFolders for LHC16k";
        rm $OUTPUTDIR_LHC16k/*/GammaCalo_*.root
        rm $OUTPUTDIR_LHC16k/*/*/*GammaCalo_*.root
    fi
    if [ $HAVELHC16l == 1 ]; then
        echo "removing all GammaCalo files in runFolders for LHC16l";
        rm $OUTPUTDIR_LHC16l/*/GammaCalo_*.root
        rm $OUTPUTDIR_LHC16l/*/*/*GammaCalo_*.root
    fi

    if [ $HAVELHC17d20a1 == 1 ]; then
        echo "removing all GammaCalo files in runFolders for LHC17d20a1";
        rm $OUTPUTDIR_LHC17d20a1/*/GammaCalo_*.root
        rm $OUTPUTDIR_LHC17d20a1/*/*/*GammaCalo_*.root
    fi
    if [ $HAVELHC17d20a1Ex == 1 ]; then
        echo "removing all GammaCalo files in runFolders for LHC17d20a1Ex";
        rm $OUTPUTDIR_LHC17d20a1Ex/*/GammaCalo_*.root
        rm $OUTPUTDIR_LHC17d20a1Ex/*/*/*GammaCalo_*.root
    fi
    if [ $HAVELHC17d20a2 == 1 ]; then
        echo "removing all GammaCalo files in runFolders for LHC17d20a2";
        rm $OUTPUTDIR_LHC17d20a2/*/GammaCalo_*.root
        rm $OUTPUTDIR_LHC17d20a2/*/*/*GammaCalo_*.root
    fi
    if [ $HAVELHC17d20a2Ex == 1 ]; then
        echo "removing all GammaCalo files in runFolders for LHC17d20a2Ex";
        rm $OUTPUTDIR_LHC17d20a2Ex/*/GammaCalo_*.root
        rm $OUTPUTDIR_LHC17d20a2Ex/*/*/*GammaCalo_*.root
    fi
fi