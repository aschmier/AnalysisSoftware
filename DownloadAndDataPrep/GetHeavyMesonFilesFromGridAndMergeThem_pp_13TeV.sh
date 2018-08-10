# WARNING: this script uses regular expressions (e.g. ^...$). Checkout if your terminal uses the same regex version!

# Run as:
# bash GetHeavyMesonFilesFromGridAndMergeThem_pp_13TeV.sh <your_username>

#! /bin/bash

# copies files from grid
# creates directory
# changes internal structure
# merges files according to the pPb needs

#! /bin/bash
source basicFunction.sh

DOWNLOADON=1
MERGEON=1
SEPARATEON=0
CLEANUP=1
CLEANUPMAYOR=0
number=""
AliTrainDirDt="/alice/cern.ch/user/a/alitrain/PWGGA/GA_pp" # data train path on the GRID
AliTrainDirMC="/alice/cern.ch/user/a/alitrain/PWGGA/GA_pp_MC" # MC train path on the GRID

# Print input arguments
    echo "User name: $1"
    # echo "Path: $PATH"
#

# Set number of slashes
    if [ "$1" = "fbock" ]; then
        BASEDIR=/mnt/additionalStorage/OutputLegoTrains/pp13TeV
    elif [ "$1" = "hannahbossi" ]; then
        BASEDIR=/Volumes/external_memory/CERN_data/QA
    elif [ "$1" = "dmuhlhei" ]; then
        BASEDIR=~/data/work/Grid
    elif [ "$1" = "jlueh" ]; then
        BASEDIR=~/Daten/GridDownload
    elif [ "$1" = "redeboer" ]; then
        BASEDIR=~/alice/GridOutput
    else
        echo "ERROR: User name \"$1\" not valid"
        exit 1
    fi
    NSlashesBASE=`tr -dc '/' <<<"$BASEDIR" | wc -c`
    NSlashes=`expr $NSlashesBASE + 4`
    NSlashes2=`expr $NSlashes - 1`
    NSlashes3=`expr $NSlashes + 1`
    NSlashes4=`expr $NSlashes + 2`
#

# Train configurations
    passNr="1"
    # DEFAULT LHC data files (for if you comment below input)
        LHC16Data=""
        LHC16dData=""
        LHC16gData=""
        LHC16hData=""
        LHC16iData=""
        LHC16jData=""
        LHC16kData=""
        LHC16lData=""
        LHC16oData=""
        LHC16pData=""
        LHC16eData=""
    # DEFAULT Monte Carlo data files (for if you comment below input)
        LHC17MC=""
        LHC17f6MC=""
        LHC17f9MC=""
        LHC17d17MC=""
        LHC17f5MC=""
        LHC17d3MC=""
        LHC17e5MC=""
        LHC17d20a1MC=""
        LHC17d20a2MC=""
        LHC17d16MC=""
        LHC17d18MC=""
    # DEFAULT Monte Carlo EXTRA data files (for if you comment below input)
        LHC17MCextra=""
        LHC17f6MCextra=""
        LHC17f9MCextra=""
        LHC17d17MCextra=""
        LHC17f5MCextra=""
        LHC17d3MCextra=""
        LHC17e5MCextra=""
        LHC17d20a1MCextra=""
        LHC17d20a2MCextra=""
        LHC17d16MCextra=""
        LHC17d18MCextra=""
    # LHC data files
        # LHC16Data="2374" # comment: "omega + eta'"                    --> PCM+EMC (test!)
        # LHC16Data="2422" # comment: "Remco PHOS request"              --> PCM+PHOS
        LHC16Data="2423" # comment: "Remco & Hannah, Joshua request"  --> PCM+EMC
        # LHC16dData="child_1"
        # LHC16gData="child_2"
        LHC16hData="child_3"
        LHC16iData="child_4"
        LHC16jData="child_5"
        LHC16kData="child_6"
        LHC16lData="child_7"
        LHC16oData="child_8"
        LHC16pData="child_9"
        # LHC16eData="child_10"
    # Monte Carlo data files
        # LHC17MC="3341" # comment: "eta' test"                --> PCM+EMC (test!)
        # LHC17MC="3398" # comment: "Remco request PHOS"       --> PCM+PHOS
        LHC17MC="3409" # comment: "Remco request - PCM+EMC"  --> PCM+EMC
        # LHC17f6MC="child_1"    # anchored to LHC16d (child 1)
        # LHC17f9MC="child_2"    # anchored to LHC16e (child 10)
        # LHC17d17MC="child_3"   # anchored to LHC16g (child 2)
        LHC17f5MC="child_4"    # anchored to LHC16h (child 3)
        LHC17d3MC="child_5"    # anchored to LHC16i (child 4)
        LHC17e5MC="child_6"    # anchored to LHC16j (child 5)
        LHC17d20a1MC="child_7" # anchored to LHC16k (child 6)
        LHC17d20a2MC="child_8" # anchored to LHC16l (child 7)
        LHC17d16MC="child_9"   # anchored to LHC16o (child 8)
        LHC17d18MC="child_10"  # anchored to LHC16p (child 9)
    # Monte Carlo data files EXTRA
        # LHC17MCextra="3399" # comment: "Remco request PHOS"       --> PCM+PHOS
        # LHC17MCextra="3410" # comment: "Remco request - PCM+EMC"  --> PCM+EMC
        # LHC17f6MCextra=$LHC17f6MC    # anchored to LHC16d (child 1)
        # LHC17f9MCextra=$LHC17f9MC    # anchored to LHC16e (child 10)
        # LHC17d17MCextra=$LHC17d17MC   # anchored to LHC16g (child 2)
        # LHC17f5MCextra=$LHC17f5MC    # anchored to LHC16h (child 3)
        # LHC17d3MCextra=$LHC17d3MC    # anchored to LHC16i (child 4)
        # LHC17e5MCextra=$LHC17e5MC    # anchored to LHC16j (child 5)
        # LHC17d20a1MCextra=$LHC17d20a1MC # anchored to LHC16k (child 6)
        # LHC17d20a2MCextra=$LHC17d20a2MC # anchored to LHC16l (child 7)
        # LHC17d16MCextra=$LHC17d16MC   # anchored to LHC16o (child 8)
        # LHC17d18MCextra=$LHC17d18MC  # anchored to LHC16p (child 9)
#

# Set output directory (offline and remote)
    # TRAINDIR="vAN-20180617-1-jkl" # comment: "eta' test"
    TRAINDIR="vAN-20180727-1-hijklop" # comment: "Remco PHOS request" or "Remco & Hannah, Joshua request"
    OUTPUTDIR=$BASEDIR/$TRAINDIR
#

# Set local and GRID directory names
    echo -e "\n\n--== SETTING REMOTE AND LOCAL OUTPUT DIRECTORIES ==--\n"
    mkdir -p $OUTPUTDIR/CutSelections # this folder will contain txt files with cutnumbers
    # LHC data files
        if [ -n "$LHC16dData" ]; then # child 1
            LHC16dData=`alien_ls $AliTrainDirDt/ | grep "^$LHC16Data.*$LHC16dData$"`
            OUTPUTDIR_LHC16d=$BASEDIR/$TRAINDIR/$(basename $AliTrainDirDt)-$LHC16dData
            echo "Directories for LHC16d:"
            echo "  remote \"$LHC16dData\""
            echo "  local  \"$OUTPUTDIR_LHC16d\""
        fi
        if [ -n "$LHC16gData" ]; then # child 2
            LHC16gData=`alien_ls $AliTrainDirDt/ | grep "^$LHC16Data.*$LHC16gData$"`
            OUTPUTDIR_LHC16g=$BASEDIR/$TRAINDIR/$(basename $AliTrainDirDt)-$LHC16gData
            echo "Directories for LHC16g:"
            echo "  remote \"$LHC16gData\""
            echo "  local  \"$OUTPUTDIR_LHC16g\""
        fi
        if [ -n "$LHC16hData" ]; then # child 3
            LHC16hData=`alien_ls $AliTrainDirDt/ | grep "^$LHC16Data.*$LHC16hData$"`
            OUTPUTDIR_LHC16h=$BASEDIR/$TRAINDIR/$(basename $AliTrainDirDt)-$LHC16hData
            echo "Directories for LHC16h:"
            echo "  remote \"$LHC16hData\""
            echo "  local  \"$OUTPUTDIR_LHC16h\""
        fi
        if [ -n "$LHC16iData" ]; then # child 4
            LHC16iData=`alien_ls $AliTrainDirDt/ | grep "^$LHC16Data.*$LHC16iData$"`
            OUTPUTDIR_LHC16i=$BASEDIR/$TRAINDIR/$(basename $AliTrainDirDt)-$LHC16iData
            echo "Directories for LHC16i:"
            echo "  remote \"$LHC16iData\""
            echo "  local  \"$OUTPUTDIR_LHC16i\""
        fi
        if [ -n "$LHC16jData" ]; then # child 5
            LHC16jData=`alien_ls $AliTrainDirDt/ | grep "^$LHC16Data.*$LHC16jData$"`
            OUTPUTDIR_LHC16j=$BASEDIR/$TRAINDIR/$(basename $AliTrainDirDt)-$LHC16jData
            echo "Directories for LHC16j:"
            echo "  remote \"$LHC16jData\""
            echo "  local  \"$OUTPUTDIR_LHC16j\""
        fi
        if [ -n "$LHC16kData" ]; then # child 6
            LHC16kData=`alien_ls $AliTrainDirDt/ | grep "^$LHC16Data.*$LHC16kData$"`
            OUTPUTDIR_LHC16k=$BASEDIR/$TRAINDIR/$(basename $AliTrainDirDt)-$LHC16kData
            echo "Directories for LHC16k:"
            echo "  remote \"$LHC16kData\""
            echo "  local  \"$OUTPUTDIR_LHC16k\""
        fi
        if [ -n "$LHC16lData" ]; then # child 7
            LHC16lData=`alien_ls $AliTrainDirDt/ | grep "^$LHC16Data.*$LHC16lData$"`
            OUTPUTDIR_LHC16l=$BASEDIR/$TRAINDIR/$(basename $AliTrainDirDt)-$LHC16lData
            echo "Directories for LHC16l:"
            echo "  remote \"$LHC16lData\""
            echo "  local  \"$OUTPUTDIR_LHC16l\""
        fi
        if [ -n "$LHC16oData" ]; then # child 8
            LHC16oData=`alien_ls $AliTrainDirDt/ | grep "^$LHC16Data.*$LHC16oData$"`
            OUTPUTDIR_LHC16o=$BASEDIR/$TRAINDIR/$(basename $AliTrainDirDt)-$LHC16oData
            echo "Directories for LHC16o:"
            echo "  remote \"$LHC16oData\""
            echo "  local  \"$OUTPUTDIR_LHC16o\""
        fi
        if [ -n "$LHC16pData" ]; then # child 9
            LHC16pData=`alien_ls $AliTrainDirDt/ | grep "^$LHC16Data.*$LHC16pData$"`
            OUTPUTDIR_LHC16p=$BASEDIR/$TRAINDIR/$(basename $AliTrainDirDt)-$LHC16pData
            echo "Directories for LHC16p:"
            echo "  remote \"$LHC16pData\""
            echo "  local  \"$OUTPUTDIR_LHC16p\""
        fi
        if [ -n "$LHC16eData" ]; then # child 10
            LHC16eData=`alien_ls $AliTrainDirDt/ | grep "^$LHC16Data.*$LHC16eData$"`
            OUTPUTDIR_LHC16e=$BASEDIR/$TRAINDIR/$(basename $AliTrainDirDt)-$LHC16eData
            echo "Directories for LHC16e:"
            echo "  remote \"$LHC16eData\""
            echo "  local  \"$OUTPUTDIR_LHC16e\""
        fi
    # Monte Carlo files
        if [ -n "$LHC17MC" ]; then
            echo ""
        fi
        if [ -n "$LHC17f6MC" ]; then # child 1
            LHC17f6MC=`alien_ls $AliTrainDirMC/ | grep "^$LHC17MC.*$LHC17f6MC$"`
            OUTPUTDIR_LHC17f6=$BASEDIR/$TRAINDIR/$(basename $AliTrainDirMC)-$LHC17f6MC
            echo "Directories for LHC17f6 (MC):"
            echo "  remote \"$LHC17f6MC\""
            echo "  local  \"$OUTPUTDIR_LHC17f6\""
        fi
        if [ -n "$LHC17f9MC" ]; then # child 2
            LHC17f9MC=`alien_ls $AliTrainDirMC/ | grep "^$LHC17MC.*$LHC17f9MC$"`
            OUTPUTDIR_LHC17f9=$BASEDIR/$TRAINDIR/$(basename $AliTrainDirMC)-$LHC17f9MC
            echo "Directories for LHC17f9 (MC):"
            echo "  remote \"$LHC17f9MC\""
            echo "  local  \"$OUTPUTDIR_LHC17f9\""
        fi
        if [ -n "$LHC17d17MC" ]; then # child 3
            LHC17d17MC=`alien_ls $AliTrainDirMC/ | grep "^$LHC17MC.*$LHC17d17MC$"`
            OUTPUTDIR_LHC17d17=$BASEDIR/$TRAINDIR/$(basename $AliTrainDirMC)-$LHC17d17MC
            echo "Directories for LHC17d17 (MC):"
            echo "  remote \"$LHC17d17MC\""
            echo "  local  \"$OUTPUTDIR_LHC17d17\""
        fi
        if [ -n "$LHC17f5MC" ]; then # child 4
            LHC17f5MC=`alien_ls $AliTrainDirMC/ | grep "^$LHC17MC.*$LHC17f5MC$"`
            OUTPUTDIR_LHC17f5=$BASEDIR/$TRAINDIR/$(basename $AliTrainDirMC)-$LHC17f5MC
            echo "Directories for LHC17f5 (MC):"
            echo "  remote \"$LHC17f5MC\""
            echo "  local  \"$OUTPUTDIR_LHC17f5\""
        fi
        if [ -n "$LHC17d3MC" ]; then # child 5
            LHC17d3MC=`alien_ls $AliTrainDirMC/ | grep "^$LHC17MC.*$LHC17d3MC$"`
            OUTPUTDIR_LHC17d3=$BASEDIR/$TRAINDIR/$(basename $AliTrainDirMC)-$LHC17d3MC
            echo "Directories for LHC17d3 (MC):"
            echo "  remote \"$LHC17d3MC\""
            echo "  local  \"$OUTPUTDIR_LHC17d3\""
        fi
        if [ -n "$LHC17e5MC" ]; then # child 6
            LHC17e5MC=`alien_ls $AliTrainDirMC/ | grep "^$LHC17MC.*$LHC17e5MC$"`
            OUTPUTDIR_LHC17e5=$BASEDIR/$TRAINDIR/$(basename $AliTrainDirMC)-$LHC17e5MC
            echo "Directories for LHC17e5 (MC):"
            echo "  remote \"$LHC17e5MC\""
            echo "  local  \"$OUTPUTDIR_LHC17e5\""
        fi
        if [ -n "$LHC17d20a1MC" ]; then # child 7
            LHC17d20a1MC=`alien_ls $AliTrainDirMC/ | grep "^$LHC17MC.*$LHC17d20a1MC$"`
            OUTPUTDIR_LHC17d20a1=$BASEDIR/$TRAINDIR/$(basename $AliTrainDirMC)-$LHC17d20a1MC
            echo "Directories for LHC17d20a1 (MC):"
            echo "  remote \"$LHC17d20a1MC\""
            echo "  local  \"$OUTPUTDIR_LHC17d20a1\""
        fi
        if [ -n "$LHC17d20a2MC" ]; then # child 8
            LHC17d20a2MC=`alien_ls $AliTrainDirMC/ | grep "^$LHC17MC.*$LHC17d20a2MC$"`
            OUTPUTDIR_LHC17d20a2=$BASEDIR/$TRAINDIR/$(basename $AliTrainDirMC)-$LHC17d20a2MC
            echo "Directories for LHC17d20a2 (MC):"
            echo "  remote \"$LHC17d20a2MC\""
            echo "  local  \"$OUTPUTDIR_LHC17d20a2\""
        fi
        if [ -n "$LHC17d16MC" ]; then # child 9
            LHC17d16MC=`alien_ls $AliTrainDirMC/ | grep "^$LHC17MC.*$LHC17d16MC$"`
            OUTPUTDIR_LHC17d16=$BASEDIR/$TRAINDIR/$(basename $AliTrainDirMC)-$LHC17d16MC
            echo "Directories for LHC17d16 (MC):"
            echo "  remote \"$LHC17d16MC\""
            echo "  local  \"$OUTPUTDIR_LHC17d16\""
        fi
        if [ -n "$LHC17d18MC" ]; then # child 10
            LHC17d18MC=`alien_ls $AliTrainDirMC/ | grep "^$LHC17MC.*$LHC17d18MC$"`
            OUTPUTDIR_LHC17d18=$BASEDIR/$TRAINDIR/$(basename $AliTrainDirMC)-$LHC17d18MC
            echo "Directories for LHC17d18 (MC):"
            echo "  remote \"$LHC17d18MC\""
            echo "  local  \"$OUTPUTDIR_LHC17d18\""
        fi
    # Monte Carlo files EXTRA
        if [ -n "$LHC17MCextra" ]; then
            echo ""
        fi
        if [ -n "$LHC17f6MCextra" ]; then # child 1
            LHC17f6MCextra=`alien_ls $AliTrainDirMC/ | grep "^$LHC17MCextra.*$LHC17f6MCextra$"`
            OUTPUTDIR_LHC17f6extra=$BASEDIR/$TRAINDIR/$(basename $AliTrainDirMC)-$LHC17f6MCextra
            echo "Directories for LHC17f6 (MC extra):"
            echo "  remote \"$LHC17f6MCextra\""
            echo "  local  \"$OUTPUTDIR_LHC17f6extra\""
        fi
        if [ -n "$LHC17f9MCextra" ]; then # child 2
            LHC17f9MCextra=`alien_ls $AliTrainDirMC/ | grep "^$LHC17MCextra.*$LHC17f9MCextra$"`
            OUTPUTDIR_LHC17f9extra=$BASEDIR/$TRAINDIR/$(basename $AliTrainDirMC)-$LHC17f9MCextra
            echo "Directories for LHC17f9 (MC extra):"
            echo "  remote \"$LHC17f9MCextra\""
            echo "  local  \"$OUTPUTDIR_LHC17f9extra\""
        fi
        if [ -n "$LHC17d17MCextra" ]; then # child 3
            LHC17d17MCextra=`alien_ls $AliTrainDirMC/ | grep "^$LHC17MCextra.*$LHC17d17MCextra$"`
            OUTPUTDIR_LHC17d17extra=$BASEDIR/$TRAINDIR/$(basename $AliTrainDirMC)-$LHC17d17MCextra
            echo "Directories for LHC17d17 (MC extra):"
            echo "  remote \"$LHC17d17MCextra\""
            echo "  local  \"$OUTPUTDIR_LHC17d17extra\""
        fi
        if [ -n "$LHC17f5MCextra" ]; then # child 4
            LHC17f5MCextra=`alien_ls $AliTrainDirMC/ | grep "^$LHC17MCextra.*$LHC17f5MCextra$"`
            OUTPUTDIR_LHC17f5extra=$BASEDIR/$TRAINDIR/$(basename $AliTrainDirMC)-$LHC17f5MCextra
            echo "Directories for LHC17f5 (MC extra):"
            echo "  remote \"$LHC17f5MCextra\""
            echo "  local  \"$OUTPUTDIR_LHC17f5extra\""
        fi
        if [ -n "$LHC17d3MCextra" ]; then # child 5
            LHC17d3MCextra=`alien_ls $AliTrainDirMC/ | grep "^$LHC17MCextra.*$LHC17d3MCextra$"`
            OUTPUTDIR_LHC17d3extra=$BASEDIR/$TRAINDIR/$(basename $AliTrainDirMC)-$LHC17d3MCextra
            echo "Directories for LHC17d3 (MC extra):"
            echo "  remote \"$LHC17d3MCextra\""
            echo "  local  \"$OUTPUTDIR_LHC17d3extra\""
        fi
        if [ -n "$LHC17e5MCextra" ]; then # child 6
            LHC17e5MCextra=`alien_ls $AliTrainDirMC/ | grep "^$LHC17MCextra.*$LHC17e5MCextra$"`
            OUTPUTDIR_LHC17e5extra=$BASEDIR/$TRAINDIR/$(basename $AliTrainDirMC)-$LHC17e5MCextra
            echo "Directories for LHC17e5 (MC extra):"
            echo "  remote \"$LHC17e5MCextra\""
            echo "  local  \"$OUTPUTDIR_LHC17e5extra\""
        fi
        if [ -n "$LHC17d20a1MCextra" ]; then # child 7
            LHC17d20a1MCextra=`alien_ls $AliTrainDirMC/ | grep "^$LHC17MCextra.*$LHC17d20a1MCextra$"`
            OUTPUTDIR_LHC17d20a1extra=$BASEDIR/$TRAINDIR/$(basename $AliTrainDirMC)-$LHC17d20a1MCextra
            echo "Directories for LHC17d20a1 (MC extra):"
            echo "  remote \"$LHC17d20a1MCextra\""
            echo "  local  \"$OUTPUTDIR_LHC17d20a1extra\""
        fi
        if [ -n "$LHC17d20a2MCextra" ]; then # child 8
            LHC17d20a2MCextra=`alien_ls $AliTrainDirMC/ | grep "^$LHC17MCextra.*$LHC17d20a2MCextra$"`
            OUTPUTDIR_LHC17d20a2extra=$BASEDIR/$TRAINDIR/$(basename $AliTrainDirMC)-$LHC17d20a2MCextra
            echo "Directories for LHC17d20a2 (MC extra):"
            echo "  remote \"$LHC17d20a2MCextra\""
            echo "  local  \"$OUTPUTDIR_LHC17d20a2extra\""
        fi
        if [ -n "$LHC17d16MCextra" ]; then # child 9
            LHC17d16MCextra=`alien_ls $AliTrainDirMC/ | grep "^$LHC17MCextra.*$LHC17d16MCextra$"`
            OUTPUTDIR_LHC17d16extra=$BASEDIR/$TRAINDIR/$(basename $AliTrainDirMC)-$LHC17d16MCextra
            echo "Directories for LHC17d16 (MC extra):"
            echo "  remote \"$LHC17d16MCextra\""
            echo "  local  \"$OUTPUTDIR_LHC17d16extra\""
        fi
        if [ -n "$LHC17d18MCextra" ]; then # child 10
            LHC17d18MCextra=`alien_ls $AliTrainDirMC/ | grep "^$LHC17MCextra.*$LHC17d18MCextra$"`
            OUTPUTDIR_LHC17d18extra=$BASEDIR/$TRAINDIR/$(basename $AliTrainDirMC)-$LHC17d18MCextra
            echo "Directories for LHC17d18 (MC extra):"
            echo "  remote \"$LHC17d18MCextra\""
            echo "  local  \"$OUTPUTDIR_LHC17d18extra\""
        fi
#

# Download files from the GRID!
    if [ $DOWNLOADON == 1 ]; then
        echo -e "\n\n--== DOWNLOADING FILES FROM THE GRID ==--\n"
    # LHC data files
        if [ -n "$LHC16dData" ]; then # child 1
            echo -e "\n--> Downloading LHC16d..."
            CopyFileIfNonExisitentDiffList $OUTPUTDIR_LHC16d "$AliTrainDirDt/$LHC16dData/merge" DPGTracks $NSlashes3 "" kFALSE
            CopyFileIfNonExisitentDiffList $OUTPUTDIR_LHC16d "$AliTrainDirDt/$LHC16dData/merge" DPGTracksAndCalo $NSlashes3 "" kFALSE
        fi
        if [ -n "$LHC16gData" ]; then # child 2
            echo -e "\n--> Downloading LHC16g..."
            CopyFileIfNonExisitentDiffList $OUTPUTDIR_LHC16g "$AliTrainDirDt/$LHC16gData/merge" DPGTracks $NSlashes3 "" kFALSE
            CopyFileIfNonExisitentDiffList $OUTPUTDIR_LHC16g "$AliTrainDirDt/$LHC16gData/merge" DPGTracksAndCalo $NSlashes3 "" kFALSE
        fi
        if [ -n "$LHC16hData" ]; then # child 3
            echo -e "\n--> Downloading LHC16h..."
            CopyFileIfNonExisitentDiffList $OUTPUTDIR_LHC16h "$AliTrainDirDt/$LHC16hData/merge_runlist_2" DPGTracks $NSlashes3 "" kFALSE
            CopyFileIfNonExisitentDiffList $OUTPUTDIR_LHC16h "$AliTrainDirDt/$LHC16hData/merge_runlist_3" DPGTracksAndCalo $NSlashes3 "" kFALSE
        fi
        if [ -n "$LHC16iData" ]; then # child 4
            echo -e "\n--> Downloading LHC16i..."
            CopyFileIfNonExisitentDiffList $OUTPUTDIR_LHC16i "$AliTrainDirDt/$LHC16iData/merge_runlist_2" DPGTracks $NSlashes3 "" kFALSE
            CopyFileIfNonExisitentDiffList $OUTPUTDIR_LHC16i "$AliTrainDirDt/$LHC16iData/merge_runlist_3" DPGTracksAndCalo $NSlashes3 "" kFALSE
        fi
        if [ -n "$LHC16jData" ]; then # child 5
            echo -e "\n--> Downloading LHC16j..."
            CopyFileIfNonExisitentDiffList $OUTPUTDIR_LHC16j "$AliTrainDirDt/$LHC16jData/merge_runlist_2" DPGTracks $NSlashes3 "" kFALSE
            CopyFileIfNonExisitentDiffList $OUTPUTDIR_LHC16j "$AliTrainDirDt/$LHC16jData/merge_runlist_3" DPGTracksAndCalo $NSlashes3 "" kFALSE
        fi
        if [ -n "$LHC16kData" ]; then # child 6
            echo -e "\n--> Downloading LHC16k..."
            CopyFileIfNonExisitentDiffList $OUTPUTDIR_LHC16k "$AliTrainDirDt/$LHC16kData/merge_runlist_2" DPGTracks $NSlashes3 "" kFALSE
            CopyFileIfNonExisitentDiffList $OUTPUTDIR_LHC16k "$AliTrainDirDt/$LHC16kData/merge_runlist_4" DPGTracksAndCalo $NSlashes3 "" kFALSE
        fi
        if [ -n "$LHC16lData" ]; then # child 7
            echo -e "\n--> Downloading LHC16l..."
            CopyFileIfNonExisitentDiffList $OUTPUTDIR_LHC16l "$AliTrainDirDt/$LHC16lData/merge_runlist_2" DPGTracks $NSlashes3 "" kFALSE
            CopyFileIfNonExisitentDiffList $OUTPUTDIR_LHC16l "$AliTrainDirDt/$LHC16lData/merge_runlist_3" DPGTracksAndCalo $NSlashes3 "" kFALSE
        fi
        if [ -n "$LHC16oData" ]; then # child 8
            echo -e "\n--> Downloading LHC16o..."
            CopyFileIfNonExisitentDiffList $OUTPUTDIR_LHC16o "$AliTrainDirDt/$LHC16oData/merge" DPGTracks $NSlashes3 "" kFALSE
            CopyFileIfNonExisitentDiffList $OUTPUTDIR_LHC16o "$AliTrainDirDt/$LHC16oData/merge" DPGTracksAndCalo $NSlashes3 "" kFALSE
        fi
        if [ -n "$LHC16pData" ]; then # child 9
            echo -e "\n--> Downloading LHC16p..."
            CopyFileIfNonExisitentDiffList $OUTPUTDIR_LHC16p "$AliTrainDirDt/$LHC16pData/merge" DPGTracks $NSlashes3 "" kFALSE
            CopyFileIfNonExisitentDiffList $OUTPUTDIR_LHC16p "$AliTrainDirDt/$LHC16pData/merge" DPGTracksAndCalo $NSlashes3 "" kFALSE
        fi
        if [ -n "$LHC16eData" ]; then # child 10
            echo -e "\n--> Downloading LHC16e..."
            CopyFileIfNonExisitentDiffList $OUTPUTDIR_LHC16e "$AliTrainDirDt/$LHC16eData/merge" DPGTracks $NSlashes3 "" kFALSE
            CopyFileIfNonExisitentDiffList $OUTPUTDIR_LHC16e "$AliTrainDirDt/$LHC16eData/merge" DPGTracksAndCalo $NSlashes3 "" kFALSE
        fi
    # Monte Carlo files
        if [ -n "$LHC17f6MC" ]; then # child 1
            echo -e "\n--> Downloading LHC17f6..."
            CopyFileIfNonExisitentDiffList $OUTPUTDIR_LHC17f6 "$AliTrainDirMC/$LHC17f6MC/merge_runlist_2" DPGTracks $NSlashes3 "" kFALSE
            CopyFileIfNonExisitentDiffList $OUTPUTDIR_LHC17f6 "$AliTrainDirMC/$LHC17f6MC/merge_runlist_4" DPGTracksAndCalo $NSlashes3 "" kFALSE
        fi
        if [ -n "$LHC17f9MC" ]; then # child 2
            echo -e "\n--> Downloading LHC17f9..."
            CopyFileIfNonExisitentDiffList $OUTPUTDIR_LHC17f9 "$AliTrainDirMC/$LHC17f9MC/merge_runlist_2" DPGTracks $NSlashes3 "" kFALSE
            CopyFileIfNonExisitentDiffList $OUTPUTDIR_LHC17f9 "$AliTrainDirMC/$LHC17f9MC/merge_runlist_4" DPGTracksAndCalo $NSlashes3 "" kFALSE
        fi
        if [ -n "$LHC17d17MC" ]; then # child 3
            echo -e "\n--> Downloading LHC17d17..."
            CopyFileIfNonExisitentDiffList $OUTPUTDIR_LHC17d17 "$AliTrainDirMC/$LHC17d17MC/merge_runlist_2" DPGTracks $NSlashes3 "" kFALSE
            CopyFileIfNonExisitentDiffList $OUTPUTDIR_LHC17d17 "$AliTrainDirMC/$LHC17d17MC/merge_runlist_4" DPGTracksAndCalo $NSlashes3 "" kFALSE
        fi
        if [ -n "$LHC17f5MC" ]; then # child 4
            echo -e "\n--> Downloading LHC17f5..."
            CopyFileIfNonExisitentDiffList $OUTPUTDIR_LHC17f5 "$AliTrainDirMC/$LHC17f5MC/merge_runlist_2" DPGTracks $NSlashes3 "" kFALSE
            CopyFileIfNonExisitentDiffList $OUTPUTDIR_LHC17f5 "$AliTrainDirMC/$LHC17f5MC/merge_runlist_4" DPGTracksAndCalo $NSlashes3 "" kFALSE
        fi
        if [ -n "$LHC17d3MC" ]; then # child 5
            echo -e "\n--> Downloading LHC17d3..."
            CopyFileIfNonExisitentDiffList $OUTPUTDIR_LHC17d3 "$AliTrainDirMC/$LHC17d3MC/merge_runlist_2" DPGTracks $NSlashes3 "" kFALSE
            CopyFileIfNonExisitentDiffList $OUTPUTDIR_LHC17d3 "$AliTrainDirMC/$LHC17d3MC/merge_runlist_4" DPGTracksAndCalo $NSlashes3 "" kFALSE
        fi
        if [ -n "$LHC17e5MC" ]; then # child 6
            echo -e "\n--> Downloading LHC17e5..."
            CopyFileIfNonExisitentDiffList $OUTPUTDIR_LHC17e5 "$AliTrainDirMC/$LHC17e5MC/merge_runlist_2" DPGTracks $NSlashes3 "" kFALSE
            CopyFileIfNonExisitentDiffList $OUTPUTDIR_LHC17e5 "$AliTrainDirMC/$LHC17e5MC/merge_runlist_4" DPGTracksAndCalo $NSlashes3 "" kFALSE
        fi
        if [ -n "$LHC17d20a1MC" ]; then # child 7
            echo -e "\n--> Downloading LHC17d20a1..."
            CopyFileIfNonExisitentDiffList $OUTPUTDIR_LHC17d20a1 "$AliTrainDirMC/$LHC17d20a1MC/merge_runlist_2" DPGTracks $NSlashes3 "" kFALSE
            CopyFileIfNonExisitentDiffList $OUTPUTDIR_LHC17d20a1 "$AliTrainDirMC/$LHC17d20a1MC/merge_runlist_4" DPGTracksAndCalo $NSlashes3 "" kFALSE
        fi
        if [ -n "$LHC17d20a2MC" ]; then # child 8
            echo -e "\n--> Downloading LHC17d20a2..."
            CopyFileIfNonExisitentDiffList $OUTPUTDIR_LHC17d20a2 "$AliTrainDirMC/$LHC17d20a2MC/merge_runlist_2" DPGTracks $NSlashes3 "" kFALSE
            CopyFileIfNonExisitentDiffList $OUTPUTDIR_LHC17d20a2 "$AliTrainDirMC/$LHC17d20a2MC/merge_runlist_4" DPGTracksAndCalo $NSlashes3 "" kFALSE
        fi
        if [ -n "$LHC17d16MC" ]; then # child 9
            echo -e "\n--> Downloading LHC17d16..."
            CopyFileIfNonExisitentDiffList $OUTPUTDIR_LHC17d16 "$AliTrainDirMC/$LHC17d16MC/merge_runlist_2" DPGTracks $NSlashes3 "" kFALSE
            CopyFileIfNonExisitentDiffList $OUTPUTDIR_LHC17d16 "$AliTrainDirMC/$LHC17d16MC/merge_runlist_4" DPGTracksAndCalo $NSlashes3 "" kFALSE
        fi
        if [ -n "$LHC17d18MC" ]; then # child 10
            echo -e "\n--> Downloading LHC17d18..."
            CopyFileIfNonExisitentDiffList $OUTPUTDIR_LHC17d18 "$AliTrainDirMC/$LHC17d18MC/merge_runlist_2" DPGTracks $NSlashes3 "" kFALSE
            CopyFileIfNonExisitentDiffList $OUTPUTDIR_LHC17d18 "$AliTrainDirMC/$LHC17d18MC/merge_runlist_4" DPGTracksAndCalo $NSlashes3 "" kFALSE
        fi
    # Monte Carlo files EXTRA
        if [ -n "$LHC17f6MCextra" ]; then # child 1 EXTRA
            echo -e "\n--> Downloading LHC17f6 extra..."
            CopyFileIfNonExisitentDiffList $OUTPUTDIR_LHC17f6extra "$AliTrainDirMC/$LHC17f6MCextra/merge_runlist_2" DPGTracks $NSlashes3 "" kFALSE
            CopyFileIfNonExisitentDiffList $OUTPUTDIR_LHC17f6extra "$AliTrainDirMC/$LHC17f6MCextra/merge_runlist_4" DPGTracksAndCalo $NSlashes3 "" kFALSE
        fi
        if [ -n "$LHC17f9MCextra" ]; then # child 2 EXTRA
            echo -e "\n--> Downloading LHC17f9 extra..."
            CopyFileIfNonExisitentDiffList $OUTPUTDIR_LHC17f9extra "$AliTrainDirMC/$LHC17f9MCextra/merge_runlist_2" DPGTracks $NSlashes3 "" kFALSE
            CopyFileIfNonExisitentDiffList $OUTPUTDIR_LHC17f9extra "$AliTrainDirMC/$LHC17f9MCextra/merge_runlist_4" DPGTracksAndCalo $NSlashes3 "" kFALSE
        fi
        if [ -n "$LHC17d17MCextra" ]; then # child 3 EXTRA
            echo -e "\n--> Downloading LHC17d17 extra..."
            CopyFileIfNonExisitentDiffList $OUTPUTDIR_LHC17d17extra "$AliTrainDirMC/$LHC17d17MCextra/merge_runlist_2" DPGTracks $NSlashes3 "" kFALSE
            CopyFileIfNonExisitentDiffList $OUTPUTDIR_LHC17d17extra "$AliTrainDirMC/$LHC17d17MCextra/merge_runlist_4" DPGTracksAndCalo $NSlashes3 "" kFALSE
        fi
        if [ -n "$LHC17f5MCextra" ]; then # child 4 EXTRA
            echo -e "\n--> Downloading LHC17f5 extra..."
            CopyFileIfNonExisitentDiffList $OUTPUTDIR_LHC17f5extra "$AliTrainDirMC/$LHC17f5MCextra/merge_runlist_2" DPGTracks $NSlashes3 "" kFALSE
            CopyFileIfNonExisitentDiffList $OUTPUTDIR_LHC17f5extra "$AliTrainDirMC/$LHC17f5MCextra/merge_runlist_4" DPGTracksAndCalo $NSlashes3 "" kFALSE
        fi
        if [ -n "$LHC17d3MCextra" ]; then # child 5 EXTRA
            echo -e "\n--> Downloading LHC17d3 extra..."
            CopyFileIfNonExisitentDiffList $OUTPUTDIR_LHC17d3extra "$AliTrainDirMC/$LHC17d3MCextra/merge_runlist_2" DPGTracks $NSlashes3 "" kFALSE
            CopyFileIfNonExisitentDiffList $OUTPUTDIR_LHC17d3extra "$AliTrainDirMC/$LHC17d3MCextra/merge_runlist_4" DPGTracksAndCalo $NSlashes3 "" kFALSE
        fi
        if [ -n "$LHC17e5MCextra" ]; then # child 6 EXTRA
            echo -e "\n--> Downloading LHC17e5 extra..."
            CopyFileIfNonExisitentDiffList $OUTPUTDIR_LHC17e5extra "$AliTrainDirMC/$LHC17e5MCextra/merge_runlist_2" DPGTracks $NSlashes3 "" kFALSE
            CopyFileIfNonExisitentDiffList $OUTPUTDIR_LHC17e5extra "$AliTrainDirMC/$LHC17e5MCextra/merge_runlist_4" DPGTracksAndCalo $NSlashes3 "" kFALSE
        fi
        if [ -n "$LHC17d20a1MCextra" ]; then # child 7 EXTRA
            echo -e "\n--> Downloading LHC17d20a1 extra..."
            CopyFileIfNonExisitentDiffList $OUTPUTDIR_LHC17d20a1extra "$AliTrainDirMC/$LHC17d20a1MCextra/merge_runlist_2" DPGTracks $NSlashes3 "" kFALSE
            CopyFileIfNonExisitentDiffList $OUTPUTDIR_LHC17d20a1extra "$AliTrainDirMC/$LHC17d20a1MCextra/merge_runlist_4" DPGTracksAndCalo $NSlashes3 "" kFALSE
        fi
        if [ -n "$LHC17d20a2MCextra" ]; then # child 8 EXTRA
            echo -e "\n--> Downloading LHC17d20a2 extra..."
            CopyFileIfNonExisitentDiffList $OUTPUTDIR_LHC17d20a2extra "$AliTrainDirMC/$LHC17d20a2MCextra/merge_runlist_2" DPGTracks $NSlashes3 "" kFALSE
            CopyFileIfNonExisitentDiffList $OUTPUTDIR_LHC17d20a2extra "$AliTrainDirMC/$LHC17d20a2MCextra/merge_runlist_4" DPGTracksAndCalo $NSlashes3 "" kFALSE
        fi
        if [ -n "$LHC17d16MCextra" ]; then # child 9 EXTRA
            echo -e "\n--> Downloading LHC17d16 extra..."
            CopyFileIfNonExisitentDiffList $OUTPUTDIR_LHC17d16extra "$AliTrainDirMC/$LHC17d16MCextra/merge_runlist_2" DPGTracks $NSlashes3 "" kFALSE
            CopyFileIfNonExisitentDiffList $OUTPUTDIR_LHC17d16extra "$AliTrainDirMC/$LHC17d16MCextra/merge_runlist_4" DPGTracksAndCalo $NSlashes3 "" kFALSE
        fi
        if [ -n "$LHC17d18MCextra" ]; then # child 10 EXTRA
            echo -e "\n--> Downloading LHC17d18 extra..."
            CopyFileIfNonExisitentDiffList $OUTPUTDIR_LHC17d18extra "$AliTrainDirMC/$LHC17d18MCextra/merge_runlist_2" DPGTracks $NSlashes3 "" kFALSE
            CopyFileIfNonExisitentDiffList $OUTPUTDIR_LHC17d18extra "$AliTrainDirMC/$LHC17d18MCextra/merge_runlist_4" DPGTracksAndCalo $NSlashes3 "" kFALSE
        fi
#

# Copy files with proper name
        echo -e "\n\n--== CHANGING STRUCTURE IF NEEDED ==--\n"
    # LHC data files
        if [ -n "$LHC16dData" ]; then # child 1
            for fileName in `ls $OUTPUTDIR_LHC16d/HeavyNeutralMesonToGG-DPGTracks_*.root`; do
                ChangeStructureIfNeededHeavy $fileName $OUTPUTDIR_LHC16d $NSlashes "LHC16d-pass$passNr-DPGTracks" "-DPGTracks"
            done
            for fileName in `ls $OUTPUTDIR_LHC16d/HeavyNeutralMesonToGG-DPGTracksAndCalo_*.root`; do
                ChangeStructureIfNeededHeavy $fileName $OUTPUTDIR_LHC16d $NSlashes "LHC16d-pass$passNr-DPGTracksAndCalo" "-DPGTracksAndCalo"
            done
        fi
        if [ -n "$LHC16gData" ]; then # child 2
            for fileName in `ls $OUTPUTDIR_LHC16g/HeavyNeutralMesonToGG-DPGTracks_*.root`; do
                ChangeStructureIfNeededHeavy $fileName $OUTPUTDIR_LHC16g $NSlashes "LHC16g-pass$passNr-DPGTracks" "-DPGTracks"
            done
            for fileName in `ls $OUTPUTDIR_LHC16g/HeavyNeutralMesonToGG-DPGTracksAndCalo_*.root`; do
                ChangeStructureIfNeededHeavy $fileName $OUTPUTDIR_LHC16g $NSlashes "LHC16g-pass$passNr-DPGTracksAndCalo" "-DPGTracksAndCalo"
            done
        fi
        if [ -n "$LHC16hData" ]; then # child 3
            for fileName in `ls $OUTPUTDIR_LHC16h/HeavyNeutralMesonToGG-DPGTracks_*.root`; do
                ChangeStructureIfNeededHeavy $fileName $OUTPUTDIR_LHC16h $NSlashes "LHC16h-pass$passNr-DPGTracks" "-DPGTracks"
            done
            for fileName in `ls $OUTPUTDIR_LHC16h/HeavyNeutralMesonToGG-DPGTracksAndCalo_*.root`; do
                ChangeStructureIfNeededHeavy $fileName $OUTPUTDIR_LHC16h $NSlashes "LHC16h-pass$passNr-DPGTracksAndCalo" "-DPGTracksAndCalo"
            done
        fi
        if [ -n "$LHC16iData" ]; then # child 4
            for fileName in `ls $OUTPUTDIR_LHC16i/HeavyNeutralMesonToGG-DPGTracks_*.root`; do
                ChangeStructureIfNeededHeavy $fileName $OUTPUTDIR_LHC16i $NSlashes "LHC16i-pass$passNr-DPGTracks" "-DPGTracks"
            done
            for fileName in `ls $OUTPUTDIR_LHC16i/HeavyNeutralMesonToGG-DPGTracksAndCalo_*.root`; do
                ChangeStructureIfNeededHeavy $fileName $OUTPUTDIR_LHC16i $NSlashes "LHC16i-pass$passNr-DPGTracksAndCalo" "-DPGTracksAndCalo"
            done
        fi
        if [ -n "$LHC16jData" ]; then # child 5
            for fileName in `ls $OUTPUTDIR_LHC16j/HeavyNeutralMesonToGG-DPGTracks_*.root`; do
                ChangeStructureIfNeededHeavy $fileName $OUTPUTDIR_LHC16j $NSlashes "LHC16j-pass$passNr-DPGTracks" "-DPGTracks"
            done
            for fileName in `ls $OUTPUTDIR_LHC16j/HeavyNeutralMesonToGG-DPGTracksAndCalo_*.root`; do
                ChangeStructureIfNeededHeavy $fileName $OUTPUTDIR_LHC16j $NSlashes "LHC16j-pass$passNr-DPGTracksAndCalo" "-DPGTracksAndCalo"
            done
        fi
        if [ -n "$LHC16kData" ]; then # child 6
            for fileName in `ls $OUTPUTDIR_LHC16k/HeavyNeutralMesonToGG-DPGTracks_*.root`; do
                ChangeStructureIfNeededHeavy $fileName $OUTPUTDIR_LHC16k $NSlashes "LHC16k-pass$passNr-DPGTracks" "-DPGTracks"
            done
            for fileName in `ls $OUTPUTDIR_LHC16k/HeavyNeutralMesonToGG-DPGTracksAndCalo_*.root`; do
                ChangeStructureIfNeededHeavy $fileName $OUTPUTDIR_LHC16k $NSlashes "LHC16k-pass$passNr-DPGTracksAndCalo" "-DPGTracksAndCalo"
            done
        fi
        if [ -n "$LHC16lData" ]; then # child 7
            for fileName in `ls $OUTPUTDIR_LHC16l/HeavyNeutralMesonToGG-DPGTracks_*.root`; do
                ChangeStructureIfNeededHeavy $fileName $OUTPUTDIR_LHC16l $NSlashes "LHC16l-pass$passNr-DPGTracks" "-DPGTracks"
            done
            for fileName in `ls $OUTPUTDIR_LHC16l/HeavyNeutralMesonToGG-DPGTracksAndCalo_*.root`; do
                ChangeStructureIfNeededHeavy $fileName $OUTPUTDIR_LHC16l $NSlashes "LHC16l-pass$passNr-DPGTracksAndCalo" "-DPGTracksAndCalo"
            done
        fi
        if [ -n "$LHC16oData" ]; then # child 8
            for fileName in `ls $OUTPUTDIR_LHC16o/HeavyNeutralMesonToGG-DPGTracks_*.root`; do
                ChangeStructureIfNeededHeavy $fileName $OUTPUTDIR_LHC16o $NSlashes "LHC16o-pass$passNr-DPGTracks" "-DPGTracks"
            done
            for fileName in `ls $OUTPUTDIR_LHC16o/HeavyNeutralMesonToGG-DPGTracksAndCalo_*.root`; do
                ChangeStructureIfNeededHeavy $fileName $OUTPUTDIR_LHC16o $NSlashes "LHC16o-pass$passNr-DPGTracksAndCalo" "-DPGTracksAndCalo"
            done
        fi
        if [ -n "$LHC16pData" ]; then # child 9
            for fileName in `ls $OUTPUTDIR_LHC16p/HeavyNeutralMesonToGG-DPGTracks_*.root`; do
                ChangeStructureIfNeededHeavy $fileName $OUTPUTDIR_LHC16p $NSlashes "LHC16p-pass$passNr-DPGTracks" "-DPGTracks"
            done
            for fileName in `ls $OUTPUTDIR_LHC16p/HeavyNeutralMesonToGG-DPGTracksAndCalo_*.root`; do
                ChangeStructureIfNeededHeavy $fileName $OUTPUTDIR_LHC16p $NSlashes "LHC16p-pass$passNr-DPGTracksAndCalo" "-DPGTracksAndCalo"
            done
        fi
        if [ -n "$LHC16eData" ]; then # child 10
            for fileName in `ls $OUTPUTDIR_LHC16e/HeavyNeutralMesonToGG-DPGTracks_*.root`; do
                ChangeStructureIfNeededHeavy $fileName $OUTPUTDIR_LHC16e $NSlashes "LHC16e-pass$passNr-DPGTracks" "-DPGTracks"
            done
            for fileName in `ls $OUTPUTDIR_LHC16e/HeavyNeutralMesonToGG-DPGTracksAndCalo_*.root`; do
                ChangeStructureIfNeededHeavy $fileName $OUTPUTDIR_LHC16e $NSlashes "LHC16e-pass$passNr-DPGTracksAndCalo" "-DPGTracksAndCalo"
            done
        fi
    # Monte Carlo files
        if [ -n "$LHC17f6MC" ]; then    # child 1
            for fileName in `ls $OUTPUTDIR_LHC17f6/HeavyNeutralMesonToGG-DPGTracks_*.root`; do
                ChangeStructureIfNeededHeavy $fileName $OUTPUTDIR_LHC17f6 $NSlashes "MC_LHC17f6-anchor16d-DPGTracks" "-DPGTracks"
            done
            for fileName in `ls $OUTPUTDIR_LHC17f6/HeavyNeutralMesonToGG-DPGTracksAndCalo_*.root`; do
                ChangeStructureIfNeededHeavy $fileName $OUTPUTDIR_LHC17f6 $NSlashes "MC_LHC17f6-anchor16d-DPGTracksAndCalo" "-DPGTracksAndCalo"
            done
        fi
        if [ -n "$LHC17f9MC" ]; then    # child 2
            for fileName in `ls $OUTPUTDIR_LHC17f9/HeavyNeutralMesonToGG-DPGTracks_*.root`; do
                ChangeStructureIfNeededHeavy $fileName $OUTPUTDIR_LHC17f9 $NSlashes "MC_LHC17f9-anchor16e-DPGTracks" "-DPGTracks"
            done
            for fileName in `ls $OUTPUTDIR_LHC17f9/HeavyNeutralMesonToGG-DPGTracksAndCalo_*.root`; do
                ChangeStructureIfNeededHeavy $fileName $OUTPUTDIR_LHC17f9 $NSlashes "MC_LHC17f9-anchor16e-DPGTracksAndCalo" "-DPGTracksAndCalo"
            done
        fi
        if [ -n "$LHC17d17MC" ]; then   # child 3
            for fileName in `ls $OUTPUTDIR_LHC17d17/HeavyNeutralMesonToGG-DPGTracks_*.root`; do
                ChangeStructureIfNeededHeavy $fileName $OUTPUTDIR_LHC17d17 $NSlashes "MC_LHC17d17-anchor16g-DPGTracks" "-DPGTracks"
            done
            for fileName in `ls $OUTPUTDIR_LHC17d17/HeavyNeutralMesonToGG-DPGTracksAndCalo_*.root`; do
                ChangeStructureIfNeededHeavy $fileName $OUTPUTDIR_LHC17d17 $NSlashes "MC_LHC17d17-anchor16g-DPGTracksAndCalo" "-DPGTracksAndCalo"
            done
        fi
        if [ -n "$LHC17f5MC" ]; then    # child 4
            for fileName in `ls $OUTPUTDIR_LHC17f5/HeavyNeutralMesonToGG-DPGTracks_*.root`; do
                ChangeStructureIfNeededHeavy $fileName $OUTPUTDIR_LHC17f5 $NSlashes "MC_LHC17f5-anchor16h-DPGTracks" "-DPGTracks"
            done
            for fileName in `ls $OUTPUTDIR_LHC17f5/HeavyNeutralMesonToGG-DPGTracksAndCalo_*.root`; do
                ChangeStructureIfNeededHeavy $fileName $OUTPUTDIR_LHC17f5 $NSlashes "MC_LHC17f5-anchor16h-DPGTracksAndCalo" "-DPGTracksAndCalo"
            done
        fi
        if [ -n "$LHC17d3MC" ]; then    # child 5
            for fileName in `ls $OUTPUTDIR_LHC17d3/HeavyNeutralMesonToGG-DPGTracks_*.root`; do
                ChangeStructureIfNeededHeavy $fileName $OUTPUTDIR_LHC17d3 $NSlashes "MC_LHC17d3-anchor16i-DPGTracks" "-DPGTracks"
            done
            for fileName in `ls $OUTPUTDIR_LHC17d3/HeavyNeutralMesonToGG-DPGTracksAndCalo_*.root`; do
                ChangeStructureIfNeededHeavy $fileName $OUTPUTDIR_LHC17d3 $NSlashes "MC_LHC17d3-anchor16i-DPGTracksAndCalo" "-DPGTracksAndCalo"
            done
        fi
        if [ -n "$LHC17e5MC" ]; then    # child 6
            for fileName in `ls $OUTPUTDIR_LHC17e5/HeavyNeutralMesonToGG-DPGTracks_*.root`; do
                ChangeStructureIfNeededHeavy $fileName $OUTPUTDIR_LHC17e5 $NSlashes "MC_LHC17e5-anchor16j-DPGTracks" "-DPGTracks"
            done
            for fileName in `ls $OUTPUTDIR_LHC17e5/HeavyNeutralMesonToGG-DPGTracksAndCalo_*.root`; do
                ChangeStructureIfNeededHeavy $fileName $OUTPUTDIR_LHC17e5 $NSlashes "MC_LHC17e5-anchor16j-DPGTracksAndCalo" "-DPGTracksAndCalo"
            done
        fi
        if [ -n "$LHC17d20a1MC" ]; then # child 7
            for fileName in `ls $OUTPUTDIR_LHC17d20a1/HeavyNeutralMesonToGG-DPGTracks_*.root`; do
                ChangeStructureIfNeededHeavy $fileName $OUTPUTDIR_LHC17d20a1 $NSlashes "MC_LHC17d20a1-anchor16k-DPGTracks" "-DPGTracks"
            done
            for fileName in `ls $OUTPUTDIR_LHC17d20a1/HeavyNeutralMesonToGG-DPGTracksAndCalo_*.root`; do
                ChangeStructureIfNeededHeavy $fileName $OUTPUTDIR_LHC17d20a1 $NSlashes "MC_LHC17d20a1-anchor16k-DPGTracksAndCalo" "-DPGTracksAndCalo"
            done
        fi
        if [ -n "$LHC17d20a2MC" ]; then # child 8
            for fileName in `ls $OUTPUTDIR_LHC17d20a2/HeavyNeutralMesonToGG-DPGTracks_*.root`; do
                ChangeStructureIfNeededHeavy $fileName $OUTPUTDIR_LHC17d20a2 $NSlashes "MC_LHC17d20a2-anchor16l-DPGTracks" "-DPGTracks"
            done
            for fileName in `ls $OUTPUTDIR_LHC17d20a2/HeavyNeutralMesonToGG-DPGTracksAndCalo_*.root`; do
                ChangeStructureIfNeededHeavy $fileName $OUTPUTDIR_LHC17d20a2 $NSlashes "MC_LHC17d20a2-anchor16l-DPGTracksAndCalo" "-DPGTracksAndCalo"
            done
        fi
        if [ -n "$LHC17d16MC" ]; then   # child 9
            for fileName in `ls $OUTPUTDIR_LHC17d16/HeavyNeutralMesonToGG-DPGTracks_*.root`; do
                ChangeStructureIfNeededHeavy $fileName $OUTPUTDIR_LHC17d16 $NSlashes "MC_LHC17d16-anchor16o-DPGTracks" "-DPGTracks"
            done
            for fileName in `ls $OUTPUTDIR_LHC17d16/HeavyNeutralMesonToGG-DPGTracksAndCalo_*.root`; do
                ChangeStructureIfNeededHeavy $fileName $OUTPUTDIR_LHC17d16 $NSlashes "MC_LHC17d16-anchor16o-DPGTracksAndCalo" "-DPGTracksAndCalo"
            done
        fi
        if [ -n "$LHC17d18MC" ]; then   # child 10
            for fileName in `ls $OUTPUTDIR_LHC17d18/HeavyNeutralMesonToGG-DPGTracks_*.root`; do
                ChangeStructureIfNeededHeavy $fileName $OUTPUTDIR_LHC17d18 $NSlashes "MC_LHC17d18-anchor16p-DPGTracks" "-DPGTracks"
            done
            for fileName in `ls $OUTPUTDIR_LHC17d18/HeavyNeutralMesonToGG-DPGTracksAndCalo_*.root`; do
                ChangeStructureIfNeededHeavy $fileName $OUTPUTDIR_LHC17d18 $NSlashes "MC_LHC17d18-anchor16p-DPGTracksAndCalo" "-DPGTracksAndCalo"
            done
        fi
    # Monte Carlo files EXTRA
        if [ -n "$LHC17f6MCextra" ]; then    # child 1 EXTRA
            for fileName in `ls $OUTPUTDIR_LHC17f6extra/HeavyNeutralMesonToGG-DPGTracks_*.root`; do
                ChangeStructureIfNeededHeavy $fileName $OUTPUTDIR_LHC17f6extra $NSlashes "MCextra_LHC17f6-anchor16d-DPGTracks" "-DPGTracks"
            done
            for fileName in `ls $OUTPUTDIR_LHC17f6extra/HeavyNeutralMesonToGG-DPGTracksAndCalo_*.root`; do
                ChangeStructureIfNeededHeavy $fileName $OUTPUTDIR_LHC17f6extra $NSlashes "MCextra_LHC17f6-anchor16d-DPGTracksAndCalo" "-DPGTracksAndCalo"
            done
        fi
        if [ -n "$LHC17f9MCextra" ]; then    # child 2 EXTRA
            for fileName in `ls $OUTPUTDIR_LHC17f9extra/HeavyNeutralMesonToGG-DPGTracks_*.root`; do
                ChangeStructureIfNeededHeavy $fileName $OUTPUTDIR_LHC17f9extra $NSlashes "MCextra_LHC17f9-anchor16e-DPGTracks" "-DPGTracks"
            done
            for fileName in `ls $OUTPUTDIR_LHC17f9extra/HeavyNeutralMesonToGG-DPGTracksAndCalo_*.root`; do
                ChangeStructureIfNeededHeavy $fileName $OUTPUTDIR_LHC17f9extra $NSlashes "MCextra_LHC17f9-anchor16e-DPGTracksAndCalo" "-DPGTracksAndCalo"
            done
        fi
        if [ -n "$LHC17d17MCextra" ]; then   # child 3 EXTRA
            for fileName in `ls $OUTPUTDIR_LHC17d17extra/HeavyNeutralMesonToGG-DPGTracks_*.root`; do
                ChangeStructureIfNeededHeavy $fileName $OUTPUTDIR_LHC17d17extra $NSlashes "MCextra_LHC17d17-anchor16g-DPGTracks" "-DPGTracks"
            done
            for fileName in `ls $OUTPUTDIR_LHC17d17extra/HeavyNeutralMesonToGG-DPGTracksAndCalo_*.root`; do
                ChangeStructureIfNeededHeavy $fileName $OUTPUTDIR_LHC17d17extra $NSlashes "MCextra_LHC17d17-anchor16g-DPGTracksAndCalo" "-DPGTracksAndCalo"
            done
        fi
        if [ -n "$LHC17f5MCextra" ]; then    # child 4 EXTRA
            for fileName in `ls $OUTPUTDIR_LHC17f5extra/HeavyNeutralMesonToGG-DPGTracks_*.root`; do
                ChangeStructureIfNeededHeavy $fileName $OUTPUTDIR_LHC17f5extra $NSlashes "MCextra_LHC17f5-anchor16h-DPGTracks" "-DPGTracks"
            done
            for fileName in `ls $OUTPUTDIR_LHC17f5extra/HeavyNeutralMesonToGG-DPGTracksAndCalo_*.root`; do
                ChangeStructureIfNeededHeavy $fileName $OUTPUTDIR_LHC17f5extra $NSlashes "MCextra_LHC17f5-anchor16h-DPGTracksAndCalo" "-DPGTracksAndCalo"
            done
        fi
        if [ -n "$LHC17d3MCextra" ]; then    # child 5 EXTRA
            for fileName in `ls $OUTPUTDIR_LHC17d3extra/HeavyNeutralMesonToGG-DPGTracks_*.root`; do
                ChangeStructureIfNeededHeavy $fileName $OUTPUTDIR_LHC17d3extra $NSlashes "MCextra_LHC17d3-anchor16i-DPGTracks" "-DPGTracks"
            done
            for fileName in `ls $OUTPUTDIR_LHC17d3extra/HeavyNeutralMesonToGG-DPGTracksAndCalo_*.root`; do
                ChangeStructureIfNeededHeavy $fileName $OUTPUTDIR_LHC17d3extra $NSlashes "MCextra_LHC17d3-anchor16i-DPGTracksAndCalo" "-DPGTracksAndCalo"
            done
        fi
        if [ -n "$LHC17e5MCextra" ]; then    # child 6 EXTRA
            for fileName in `ls $OUTPUTDIR_LHC17e5extra/HeavyNeutralMesonToGG-DPGTracks_*.root`; do
                ChangeStructureIfNeededHeavy $fileName $OUTPUTDIR_LHC17e5extra $NSlashes "MCextra_LHC17e5-anchor16j-DPGTracks" "-DPGTracks"
            done
            for fileName in `ls $OUTPUTDIR_LHC17e5extra/HeavyNeutralMesonToGG-DPGTracksAndCalo_*.root`; do
                ChangeStructureIfNeededHeavy $fileName $OUTPUTDIR_LHC17e5extra $NSlashes "MCextra_LHC17e5-anchor16j-DPGTracksAndCalo" "-DPGTracksAndCalo"
            done
        fi
        if [ -n "$LHC17d20a1MCextra" ]; then # child 7 EXTRA
            for fileName in `ls $OUTPUTDIR_LHC17d20a1extra/HeavyNeutralMesonToGG-DPGTracks_*.root`; do
                ChangeStructureIfNeededHeavy $fileName $OUTPUTDIR_LHC17d20a1extra $NSlashes "MCextra_LHC17d20a1-anchor16k-DPGTracks" "-DPGTracks"
            done
            for fileName in `ls $OUTPUTDIR_LHC17d20a1extra/HeavyNeutralMesonToGG-DPGTracksAndCalo_*.root`; do
                ChangeStructureIfNeededHeavy $fileName $OUTPUTDIR_LHC17d20a1extra $NSlashes "MCextra_LHC17d20a1-anchor16k-DPGTracksAndCalo" "-DPGTracksAndCalo"
            done
        fi
        if [ -n "$LHC17d20a2MCextra" ]; then # child 8 EXTRA
            for fileName in `ls $OUTPUTDIR_LHC17d20a2extra/HeavyNeutralMesonToGG-DPGTracks_*.root`; do
                ChangeStructureIfNeededHeavy $fileName $OUTPUTDIR_LHC17d20a2extra $NSlashes "MCextra_LHC17d20a2-anchor16l-DPGTracks" "-DPGTracks"
            done
            for fileName in `ls $OUTPUTDIR_LHC17d20a2extra/HeavyNeutralMesonToGG-DPGTracksAndCalo_*.root`; do
                ChangeStructureIfNeededHeavy $fileName $OUTPUTDIR_LHC17d20a2extra $NSlashes "MCextra_LHC17d20a2-anchor16l-DPGTracksAndCalo" "-DPGTracksAndCalo"
            done
        fi
        if [ -n "$LHC17d16MCextra" ]; then   # child 9 EXTRA
            for fileName in `ls $OUTPUTDIR_LHC17d16extra/HeavyNeutralMesonToGG-DPGTracks_*.root`; do
                ChangeStructureIfNeededHeavy $fileName $OUTPUTDIR_LHC17d16extra $NSlashes "MCextra_LHC17d16-anchor16o-DPGTracks" "-DPGTracks"
            done
            for fileName in `ls $OUTPUTDIR_LHC17d16extra/HeavyNeutralMesonToGG-DPGTracksAndCalo_*.root`; do
                ChangeStructureIfNeededHeavy $fileName $OUTPUTDIR_LHC17d16extra $NSlashes "MCextra_LHC17d16-anchor16o-DPGTracksAndCalo" "-DPGTracksAndCalo"
            done
        fi
        if [ -n "$LHC17d18MCextra" ]; then   # child 10 EXTRA
            for fileName in `ls $OUTPUTDIR_LHC17d18extra/HeavyNeutralMesonToGG-DPGTracks_*.root`; do
                ChangeStructureIfNeededHeavy $fileName $OUTPUTDIR_LHC17d18extra $NSlashes "MCextra_LHC17d18-anchor16p-DPGTracks" "-DPGTracks"
            done
            for fileName in `ls $OUTPUTDIR_LHC17d18extra/HeavyNeutralMesonToGG-DPGTracksAndCalo_*.root`; do
                ChangeStructureIfNeededHeavy $fileName $OUTPUTDIR_LHC17d18extra $NSlashes "MCextra_LHC17d18-anchor16p-DPGTracksAndCalo" "-DPGTracksAndCalo"
            done
        fi
    fi # end of [ $DOWNLOADON == 1 ]
#

# Merge ROOT files
    if [ $MERGEON == 1 ]; then
        echo -e "\n\n--== MERGING ROOT FILES ==--\n"
    # LHC data files
        echo -e "\n--> Merging LHC data files"
        periodList=(
            d # child 1
            e # child 10
            g # child 2
            h # child 3
            i # child 4
            j # child 5
            k # child 6
            l # child 7
            o # child 8
            p # child 9
        )
        rm -f runlistsToMerge.txt
        echo -e "DPGTracks" >> runlistsToMerge.txt
        echo -e "DPGTracksAndCalo" >> runlistsToMerge.txt
        listsToMerge=`cat runlistsToMerge.txt`
        filesForMerging=`ls $OUTPUTDIR/HeavyNeutralMesonToGG_LHC16h-pass$passNr-DPGTracks\_*.root`
        echo "!! \"filesForMerging\" contains ${#filesForMerging[@]} files"
        for fileName in $filesForMerging; do
            GetFileNumberMerging $fileName $((NSlashes-1)) 3
            echo "$(basename $fileName) --> number: $number"
            if [ -n "$LHC16Data" ]; then
                for runListName in $listsToMerge; do
                    echo -e "\nMerging runlist \"$runListName\""
                    rm -f listCurrMerge.txt
                    nameOut=""
                    for periodID in ${periodList[@]}; do
                        currFile=$OUTPUTDIR/HeavyNeutralMesonToGG_LHC16$periodID-pass$passNr-$runListName\_$number.root
                        if [ -f $currFile ]; then
                            echo "Period ID: $periodID"
                            nameOut+=$periodID
                            echo -e "$currFile" >> listCurrMerge.txt
                        fi
                    done
                    MergeAccordingToList listCurrMerge.txt $OUTPUTDIR/HeavyNeutralMesonToGG_LHC16$nameOut\-pass$passNr-$runListName\_$number.root
                done
            fi
        done
    # Monte Carlo files
        echo -e "\n--> Merging Monte Carlo files"
        periodListMC=(
            f6    # child 1  --> anchored to LHC16d
            f9    # child 2  --> anchored to LHC16e
            d17   # child 3  --> anchored to LHC16g
            f5    # child 4  --> anchored to LHC16h
            d3    # child 5  --> anchored to LHC16i
            e5    # child 6  --> anchored to LHC16j
            d20a1 # child 7  --> anchored to LHC16k
            d20a2 # child 8  --> anchored to LHC16l
            d16   # child 9  --> anchored to LHC16o
            d18   # child 10 --> anchored to LHC16p
        )
        rm -f runlistsToMerge.txt
        echo -e "DPGTracks" >> runlistsToMerge.txt
        echo -e "DPGTracksAndCalo" >> runlistsToMerge.txt
        listsToMerge=`cat runlistsToMerge.txt`
        filesForMerging=`ls $OUTPUTDIR/HeavyNeutralMesonToGG_MC_LHC17d20a2-anchor16*-DPGTracks\_*.root`
        echo "!! \"filesForMerging\" contains ${#filesForMerging[@]} files"
        for fileName in $filesForMerging; do
            GetFileNumberMerging $fileName $((NSlashes-1)) 4
            echo -e "\n--> Merging $number"
            for runListName in $listsToMerge; do
                rm -f listCurrMerge.txt
                nameOut=""
                for ((i = 0; i < ${#periodListMC[@]}; i++)); do
                    # 'Normal' Monte Carlo
                    if [ -n "$LHC17MC" ]; then
                        currFile=$OUTPUTDIR/HeavyNeutralMesonToGG_MC_LHC17${periodListMC[i]}-anchor16${periodList[i]}-$runListName\_$number.root
                        if [ -f $currFile ]; then
                            echo "Period ID: 17${periodListMC[i]}-anchor16${periodList[i]}"
                            nameOut+=${periodListMC[i]}
                            echo -e "$currFile" >> listCurrMerge.txt
                        fi
                    fi
                    # Extra Monte Carlo files
                    if [ -n "$LHC17MCextra" ]; then
                        currFile=$OUTPUTDIR/HeavyNeutralMesonToGG_MCextra_LHC17${periodListMC[i]}-anchor16${periodList[i]}-$runListName\_$number.root
                        if [ -f $currFile ]; then
                            echo "Period ID: 17${periodListMC[i]}-anchor16${periodList[i]}"
                            # nameOut+=${periodListMC[i]}
                            echo -e "$currFile" >> listCurrMerge.txt
                        fi
                    fi
                done
                MergeAccordingToList listCurrMerge.txt $OUTPUTDIR/HeavyNeutralMesonToGG_MC_LHC17$nameOut\-$runListName\_$number.root
            done
        done
        echo "--> Merging done"
    fi # end of [ $MERGEON == 1 ]
#

# Clean ROOT files
    if [ $CLEANUPMAYOR == 1 ]; then
        echo -e "\n--> Cleaning up ROOT files"
    # LHC data files
        if [ -n "$LHC16dData" ]; then # child 1
            echo "removing all HeavyNeutralMesonToGG files in runFolders for LHC16d"
            rm $OUTPUTDIR_LHC16d/*/HeavyNeutralMesonToGG_*.root
            rm $OUTPUTDIR_LHC16d/*/*/*HeavyNeutralMesonToGG_*.root
        fi
        if [ -n "$LHC16gData" ]; then # child 2
            echo "removing all HeavyNeutralMesonToGG files in runFolders for LHC16g"
            rm $OUTPUTDIR_LHC16g/*/HeavyNeutralMesonToGG_*.root
            rm $OUTPUTDIR_LHC16g/*/*/*HeavyNeutralMesonToGG_*.root
        fi
        if [ -n "$LHC16hData" ]; then # child 3
            echo "removing all HeavyNeutralMesonToGG files in runFolders for LHC16h"
            rm $OUTPUTDIR_LHC16h/*/HeavyNeutralMesonToGG_*.root
            rm $OUTPUTDIR_LHC16h/*/*/*HeavyNeutralMesonToGG_*.root
        fi
        if [ -n "$LHC16iData" ]; then # child 4
            echo "removing all HeavyNeutralMesonToGG files in runFolders for LHC16i"
            rm $OUTPUTDIR_LHC16i/*/HeavyNeutralMesonToGG_*.root
            rm $OUTPUTDIR_LHC16i/*/*/*HeavyNeutralMesonToGG_*.root
        fi
        if [ -n "$LHC16jData" ]; then # child 5
            echo "removing all HeavyNeutralMesonToGG files in runFolders for LHC16j"
            rm $OUTPUTDIR_LHC16j/*/HeavyNeutralMesonToGG_*.root
            rm $OUTPUTDIR_LHC16j/*/*/*HeavyNeutralMesonToGG_*.root
        fi
        if [ -n "$LHC16kData" ]; then # child 6
            echo "removing all HeavyNeutralMesonToGG files in runFolders for LHC16k"
            rm $OUTPUTDIR_LHC16k/*/HeavyNeutralMesonToGG_*.root
            rm $OUTPUTDIR_LHC16k/*/*/*HeavyNeutralMesonToGG_*.root
        fi
        if [ -n "$LHC16lData" ]; then # child 7
            echo "removing all HeavyNeutralMesonToGG files in runFolders for LHC16l"
            rm $OUTPUTDIR_LHC16l/*/HeavyNeutralMesonToGG_*.root
            rm $OUTPUTDIR_LHC16l/*/*/*HeavyNeutralMesonToGG_*.root
        fi
        if [ -n "$LHC16oData" ]; then # child 8
            echo "removing all HeavyNeutralMesonToGG files in runFolders for LHC16o"
            rm $OUTPUTDIR_LHC16o/*/HeavyNeutralMesonToGG_*.root
            rm $OUTPUTDIR_LHC16o/*/*/*HeavyNeutralMesonToGG_*.root
        fi
        if [ -n "$LHC16pData" ]; then # child 9
            echo "removing all HeavyNeutralMesonToGG files in runFolders for LHC16p"
            rm $OUTPUTDIR_LHC16p/*/HeavyNeutralMesonToGG_*.root
            rm $OUTPUTDIR_LHC16p/*/*/*HeavyNeutralMesonToGG_*.root
        fi
        if [ -n "$LHC16eData" ]; then # child 10
            echo "removing all HeavyNeutralMesonToGG files in runFolders for LHC16e"
            rm $OUTPUTDIR_LHC16e/*/HeavyNeutralMesonToGG_*.root
            rm $OUTPUTDIR_LHC16e/*/*/*HeavyNeutralMesonToGG_*.root
        fi
    # Monte Carlo files
        if [ -n "$LHC17f6MC" ]; then    # child 1
            echo "removing all HeavyNeutralMesonToGG files in runFolders for LHC17f6MC"
            rm $OUTPUTDIR_LHC17f6/*/HeavyNeutralMesonToGG_*.root
            rm $OUTPUTDIR_LHC17f6/*/*/*HeavyNeutralMesonToGG_*.root
        fi
        if [ -n "$LHC17f9MC" ]; then    # child 2
            echo "removing all HeavyNeutralMesonToGG files in runFolders for LHC17f9MC"
            rm $OUTPUTDIR_LHC17f9/*/HeavyNeutralMesonToGG_*.root
            rm $OUTPUTDIR_LHC17f9/*/*/*HeavyNeutralMesonToGG_*.root
        fi
        if [ -n "$LHC17d17MC" ]; then   # child 3
            echo "removing all HeavyNeutralMesonToGG files in runFolders for LHC17d17MC"
            rm $OUTPUTDIR_LHC17d17/*/HeavyNeutralMesonToGG_*.root
            rm $OUTPUTDIR_LHC17d17/*/*/*HeavyNeutralMesonToGG_*.root
        fi
        if [ -n "$LHC17f5MC" ]; then    # child 4
            echo "removing all HeavyNeutralMesonToGG files in runFolders for LHC17f5MC"
            rm $OUTPUTDIR_LHC17f5/*/HeavyNeutralMesonToGG_*.root
            rm $OUTPUTDIR_LHC17f5/*/*/*HeavyNeutralMesonToGG_*.root
        fi
        if [ -n "$LHC17d3MC" ]; then    # child 5
            echo "removing all HeavyNeutralMesonToGG files in runFolders for LHC17d3MC"
            rm $OUTPUTDIR_LHC17d3/*/HeavyNeutralMesonToGG_*.root
            rm $OUTPUTDIR_LHC17d3/*/*/*HeavyNeutralMesonToGG_*.root
        fi
        if [ -n "$LHC17e5MC" ]; then    # child 6
            echo "removing all HeavyNeutralMesonToGG files in runFolders for LHC17e5MC"
            rm $OUTPUTDIR_LHC17e5/*/HeavyNeutralMesonToGG_*.root
            rm $OUTPUTDIR_LHC17e5/*/*/*HeavyNeutralMesonToGG_*.root
        fi
        if [ -n "$LHC17d20a1MC" ]; then # child 7
            echo "removing all HeavyNeutralMesonToGG files in runFolders for LHC17d20a1MC"
            rm $OUTPUTDIR_LHC17d20a1/*/HeavyNeutralMesonToGG_*.root
            rm $OUTPUTDIR_LHC17d20a1/*/*/*HeavyNeutralMesonToGG_*.root
        fi
        if [ -n "$LHC17d20a2MC" ]; then # child 8
            echo "removing all HeavyNeutralMesonToGG files in runFolders for LHC17d20a2MC"
            rm $OUTPUTDIR_LHC17d20a2/*/HeavyNeutralMesonToGG_*.root
            rm $OUTPUTDIR_LHC17d20a2/*/*/*HeavyNeutralMesonToGG_*.root
        fi
        if [ -n "$LHC17d16MC" ]; then   # child 9
            echo "removing all HeavyNeutralMesonToGG files in runFolders for LHC17d16MC"
            rm $OUTPUTDIR_LHC17d16/*/HeavyNeutralMesonToGG_*.root
            rm $OUTPUTDIR_LHC17d16/*/*/*HeavyNeutralMesonToGG_*.root
        fi
        if [ -n "$LHC17d18MC" ]; then   # child 10
            echo "removing all HeavyNeutralMesonToGG files in runFolders for LHC17d18MC"
            rm $OUTPUTDIR_LHC17d18/*/HeavyNeutralMesonToGG_*.root
            rm $OUTPUTDIR_LHC17d18/*/*/*HeavyNeutralMesonToGG_*.root
        fi
    # Monte Carlo files EXTRA
        if [ -n "$LHC17f6MCextra" ]; then    # child 1 EXTRA
            echo "removing all HeavyNeutralMesonToGG files in runFolders for LHC17f6MCextra"
            rm $OUTPUTDIR_LHC17f6extra/*/HeavyNeutralMesonToGG_*.root
            rm $OUTPUTDIR_LHC17f6extra/*/*/*HeavyNeutralMesonToGG_*.root
        fi
        if [ -n "$LHC17f9MCextra" ]; then    # child 2 EXTRA
            echo "removing all HeavyNeutralMesonToGG files in runFolders for LHC17f9MCextra"
            rm $OUTPUTDIR_LHC17f9extra/*/HeavyNeutralMesonToGG_*.root
            rm $OUTPUTDIR_LHC17f9extra/*/*/*HeavyNeutralMesonToGG_*.root
        fi
        if [ -n "$LHC17d17MCextra" ]; then   # child 3 EXTRA
            echo "removing all HeavyNeutralMesonToGG files in runFolders for LHC17d17MCextra"
            rm $OUTPUTDIR_LHC17d17extra/*/HeavyNeutralMesonToGG_*.root
            rm $OUTPUTDIR_LHC17d17extra/*/*/*HeavyNeutralMesonToGG_*.root
        fi
        if [ -n "$LHC17f5MCextra" ]; then    # child 4 EXTRA
            echo "removing all HeavyNeutralMesonToGG files in runFolders for LHC17f5MCextra"
            rm $OUTPUTDIR_LHC17f5extra/*/HeavyNeutralMesonToGG_*.root
            rm $OUTPUTDIR_LHC17f5extra/*/*/*HeavyNeutralMesonToGG_*.root
        fi
        if [ -n "$LHC17d3MCextra" ]; then    # child 5 EXTRA
            echo "removing all HeavyNeutralMesonToGG files in runFolders for LHC17d3MCextra"
            rm $OUTPUTDIR_LHC17d3extra/*/HeavyNeutralMesonToGG_*.root
            rm $OUTPUTDIR_LHC17d3extra/*/*/*HeavyNeutralMesonToGG_*.root
        fi
        if [ -n "$LHC17e5MCextra" ]; then    # child 6 EXTRA
            echo "removing all HeavyNeutralMesonToGG files in runFolders for LHC17e5MCextra"
            rm $OUTPUTDIR_LHC17e5extra/*/HeavyNeutralMesonToGG_*.root
            rm $OUTPUTDIR_LHC17e5extra/*/*/*HeavyNeutralMesonToGG_*.root
        fi
        if [ -n "$LHC17d20a1MCextra" ]; then # child 7 EXTRA
            echo "removing all HeavyNeutralMesonToGG files in runFolders for LHC17d20a1MCextra"
            rm $OUTPUTDIR_LHC17d20a1extra/*/HeavyNeutralMesonToGG_*.root
            rm $OUTPUTDIR_LHC17d20a1extra/*/*/*HeavyNeutralMesonToGG_*.root
        fi
        if [ -n "$LHC17d20a2MCextra" ]; then # child 8 EXTRA
            echo "removing all HeavyNeutralMesonToGG files in runFolders for LHC17d20a2MCextra"
            rm $OUTPUTDIR_LHC17d20a2extra/*/HeavyNeutralMesonToGG_*.root
            rm $OUTPUTDIR_LHC17d20a2extra/*/*/*HeavyNeutralMesonToGG_*.root
        fi
        if [ -n "$LHC17d16MCextra" ]; then   # child 9 EXTRA
            echo "removing all HeavyNeutralMesonToGG files in runFolders for LHC17d16MCextra"
            rm $OUTPUTDIR_LHC17d16extra/*/HeavyNeutralMesonToGG_*.root
            rm $OUTPUTDIR_LHC17d16extra/*/*/*HeavyNeutralMesonToGG_*.root
        fi
        if [ -n "$LHC17d18MCextra" ]; then   # child 10 EXTRA
            echo "removing all HeavyNeutralMesonToGG files in runFolders for LHC17d18MCextra"
            rm $OUTPUTDIR_LHC17d18extra/*/HeavyNeutralMesonToGG_*.root
            rm $OUTPUTDIR_LHC17d18extra/*/*/*HeavyNeutralMesonToGG_*.root
        fi
    fi # end of [ $CLEANUPMAYOR == 1 ]
*/*/*HeavyNeutralMesonToGG_*.root
        fi
        if [ -n "$LHC17d20a2MCextra" ]; then # child 8 EXTRA
            echo "removing all HeavyNeutralMesonToGG files in runFolders for LHC17d20a2MCextra"
            rm $OUTPUTDIR_LHC17d20a2extra/*/HeavyNeutralMesonToGG_*.root
            rm $OUTPUTDIR_LHC17d20a2extra/*/*/*HeavyNeutralMesonToGG_*.root
        fi
        if [ -n "$LHC17d16MCextra" ]; then   # child 9 EXTRA
            echo "removing all HeavyNeutralMesonToGG files in runFolders for LHC17d16MCextra"
            rm $OUTPUTDIR_LHC17d16extra/*/HeavyNeutralMesonToGG_*.root
            rm $OUTPUTDIR_LHC17d16extra/*/*/*HeavyNeutralMesonToGG_*.root
        fi
        if [ -n "$LHC17d18MCextra" ]; then   # child 10 EXTRA
            echo "removing all HeavyNeutralMesonToGG files in runFolders for LHC17d18MCextra"
            rm $OUTPUTDIR_LHC17d18extra/*/HeavyNeutralMesonToGG_*.root
            rm $OUTPUTDIR_LHC17d18extr