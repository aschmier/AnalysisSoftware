#! /bin/bash
source basicFunction.sh

FILENAMEBASE=$4
MODE=$5
echo -e "DPGTrackIncAccAndEMC" > runlistsToMerge.txt
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
# check if train configuration has actually been given
HAVELHC13b=1
HAVELHC13c=1
HAVELHC13d=1
HAVELHC13e=1
HAVELHC13f=1
HAVELHC18j51=1
HAVELHC18j52=1
HAVELHC18j53=1
HAVELHC16q=1
HAVELHC16t=1
HAVELHC16qF=1
HAVELHC16tF=1
HAVETOBUILDData=0
HAVELHC18f31=1
HAVELHC18f32=1
HAVELHC18f3F1=1
HAVELHC18f3F2=1
HAVETOBUILDLHC18f3=0
HAVELHC17f2afix=1
HAVELHC17f2afixF=1
HAVETOBUILDLHC17f2afix=0
HAVELHC17g8a=1
HAVELHC17g8aF=1
HAVETOBUILDLHC17g8a=0
HAVELHC19a41=1
HAVELHC19a42=1

# default trainconfigurations
LHC13bData="";
LHC13cData="";
LHC13dData="";
LHC13eData="";
LHC13fData="";
LHC13beData="";
LHC18j5MC="";
LHC18j5_1MC="";
LHC18j5_2MC="";
LHC18j5_3MC="";
LHC16qData="";
LHC16tData="";
LHC16qDataFast="";
LHC16tDataFast="";
LHC16qtData="";
LHC18f3MCMoth="";
LHC18f3MC1="";
LHC18f3MC2="";
LHC18f3MCFast1="";
LHC18f3MCFast2="";
LHC17f2a_fixMCMoth="";
LHC17f2a_fixMC="";
LHC17f2a_fixMCFast="";
LHC17g8aMCMoth=""
LHC17g8aMC=""
LHC17g8aMCFast=""
LHC19a4MCMother=""
LHC19a4MC1=""
LHC19a4MC2=""
LHC16c3aMC1=""
LHC16c3aMC2=""
LHC16c3bMC1=""
LHC16c3bMC2=""
LHC17g6a2MC=""
LHC17g6a3MC=""

passNr="1";

if [ $1 = "fbock" ]; then
    BASEDIR=/mnt/additionalStorageExternal/OutputLegoTrains/pPb
elif [ $1 = "fbockExt" ]; then
#     BASEDIR=/media/fbock/BackupSeagate/OutputLegoTrains/pPb
    BASEDIR=/media/fbock/externalpPbStora/OutputLegoTrains/pPb
elif [ $1 = "dmuhlhei" ]; then
    BASEDIR=~/data/work/Grid
fi

if [ $3 = "AOD" ]; then
    baseLegoData=GA_pPb_AOD
    baseLegoMC=GA_pPb_MC_AOD
    pathDataR1b=pass4/AOD210/PWGGA/GA_pPb_AOD
    pathDataR1c=pass4/AOD210/PWGGA/GA_pPb_AOD
    pathDataR1d=pass4/AOD210/PWGGA/GA_pPb_AOD
    pathDataR1e=pass4/AOD210/PWGGA/GA_pPb_AOD
    pathDataR1f=pass4/AOD210/PWGGA/GA_pPb_AOD
    pathMCR1=AOD214/PWGGA/GA_pPb_MC_AOD
    pathDataR2WOSDD=pass1_CENT_woSDD/AOD190/PWGGA/GA_pPb_AOD
    pathDataR2FAST=pass1_FAST/AOD190/PWGGA/GA_pPb_AOD
    pathMCR2=AOD202/PWGGA/GA_pPb_MC_AOD
    FAST="_FAST"
    WOSDD="_CENT_woSDD"
elif [ $3 = "AODSKIMMB" ]; then
    baseLegoData=GA_pPb_AOD
    baseLegoMC=GA_pPb_MC_AOD
    pathDataR1b=pass4/AOD210/PWGGA/GA_pPb_AOD
    pathDataR1c=pass4/AOD210/PWGGA/GA_pPb_AOD
    pathDataR1d=pass4/AOD210/PWGGA/GA_pPb_AOD/547_20190603-1141/PWGGA/GA_pPb_AOD
    pathDataR1e=pass4/AOD210/PWGGA/GA_pPb_AOD/548_20190603-1142/PWGGA/GA_pPb_AOD
    pathDataR1f=pass4/AOD210/PWGGA/GA_pPb_AOD/540_20190524-1221/PWGGA/GA_pPb_AOD
    pathMCR1=AOD214/PWGGA/GA_pPb_MC_AOD
    pathDataR2WOSDD=pass1_CENT_woSDD/AOD190/PWGGA/GA_pPb_AOD
    pathDataR2FAST=pass1_FAST/AOD190/PWGGA/GA_pPb_AOD
    pathMCR2=AOD214/PWGGA/GA_pPb_MC_AOD
    FAST="_FAST"
    WOSDD="_CENT_woSDD"
elif [ $3 = "AODSKIMEMC7" ]; then
    baseLegoData=GA_pPb_AOD
    baseLegoMC=GA_pPb_MC_AOD
    pathDataR1b=pass4/AOD210/PWGGA/GA_pPb_AOD/549_20190603-1142/PWGGA/GA_pPb_AOD
    pathDataR1c=pass4/AOD210/PWGGA/GA_pPb_AOD/550_20190603-1143/PWGGA/GA_pPb_AOD
    pathDataR1d=pass4/AOD210/PWGGA/GA_pPb_AOD/551_20190603-1143/PWGGA/GA_pPb_AOD
    pathDataR1e=pass4/AOD210/PWGGA/GA_pPb_AOD/552_20190603-1143/PWGGA/GA_pPb_AOD
    pathDataR1f=pass4/AOD210/PWGGA/GA_pPb_AOD/539_20190524-1221/PWGGA/GA_pPb_AOD
    pathMCR1=AOD214/PWGGA/GA_pPb_MC_AOD
    pathDataR2WOSDD=pass1_CENT_woSDD/AOD190/PWGGA/GA_pPb_AOD
    pathDataR2FAST=pass1_FAST/AOD190/PWGGA/GA_pPb_AOD
    pathMCR2=AOD214/PWGGA/GA_pPb_MC_AOD
    FAST="_FAST"
    WOSDD="_CENT_woSDD"
elif [ $3 = "AODSKIMEGAJE" ]; then
    baseLegoData=GA_pPb_AOD
    baseLegoMC=GA_pPb_MC_AOD
    pathDataR1b=pass4/AOD210/PWGGA/GA_pPb_AOD/544_20190524-1413/PWGGA/GA_pPb_AOD
    pathDataR1c=pass4/AOD210/PWGGA/GA_pPb_AOD/543_20190524-1412/PWGGA/GA_pPb_AOD
    pathDataR1d=pass4/AOD210/PWGGA/GA_pPb_AOD/542_20190524-1412/PWGGA/GA_pPb_AOD
    pathDataR1e=pass4/AOD210/PWGGA/GA_pPb_AOD/541_20190524-1411/PWGGA/GA_pPb_AOD
    pathDataR1f=pass4/AOD210/PWGGA/GA_pPb_AOD/537_20190524-1218/PWGGA/GA_pPb_AOD
    pathMCR1=AOD214/PWGGA/GA_pPb_MC_AOD
    pathDataR2WOSDD=pass1_CENT_woSDD/AOD190/PWGGA/GA_pPb_AOD
    pathDataR2FAST=pass1_FAST/AOD190/PWGGA/GA_pPb_AOD
    pathMCR2=AOD214/PWGGA/GA_pPb_MC_AOD
    FAST="_FAST"
    WOSDD="_CENT_woSDD"
elif [ $3 = "AODSKIMPHI7" ]; then
    baseLegoData=GA_pPb_AOD
    baseLegoMC=GA_pPb_MC_AOD
    pathDataR1b=pass4/AOD210/PWGGA/GA_pPb_AOD/702_20191115-2027/PWGGA/GA_pPb_AOD
    pathDataR1c=pass4/AOD210/PWGGA/GA_pPb_AOD/703_20191115-2027/PWGGA/GA_pPb_AOD
    pathDataR1d=pass4/AOD210/PWGGA/GA_pPb_AOD/704_20191115-2028/PWGGA/GA_pPb_AOD
    pathDataR1e=pass4/AOD210/PWGGA/GA_pPb_AOD/705_20191115-2028/PWGGA/GA_pPb_AOD
    pathDataR1f=pass4/AOD210/PWGGA/GA_pPb_AOD/706_20191115-2028/PWGGA/GA_pPb_AOD
    pathMCR1=AOD214/PWGGA/GA_pPb_MC_AOD
    pathDataR2WOSDD=pass1_CENT_woSDD/AOD190/PWGGA/GA_pPb_AOD
    pathDataR2FAST=pass1_FAST/AOD190/PWGGA/GA_pPb_AOD
    pathMCR2=AOD214/PWGGA/GA_pPb_MC_AOD
    FAST="_FAST"
    WOSDD="_CENT_woSDD"
else
    baseLegoData=GA_pPb
    baseLegoMC=GA_pPb_MC
    pathDataR1b=pass4/PWGGA/GA_pPb
    pathDataR1c=pass4/PWGGA/GA_pPb
    pathDataR1d=pass4/PWGGA/GA_pPb
    pathDataR1e=pass4/PWGGA/GA_pPb
    pathDataR1f=pass4/PWGGA/GA_pPb
    pathMCR1=PWGGA/GA_pPb_MC
    pathDataR2WOSDD=pass1/PWGGA/GA_pPb
    pathDataR2FAST=pass1/PWGGA/GA_pPb
    pathMCR2=PWGGA/GA_pPb_MC
fi


# Definitition of number of slashes in your path to different depths
NSlashesBASE=`tr -dc '/' <<<"$BASEDIR" | wc -c`
NSlashes=`expr $NSlashesBASE + 4`
NSlashes2=`expr $NSlashes - 1`
NSlashes3=`expr $NSlashes + 1`
NSlashes4=`expr $NSlashes + 2`
echo "$NSlashesBASE $NSlashes $NSlashes2 $NSlashes3 $NSlashes4"

if [ $3 = "AODSKIMMB" ]; then
#     TRAINDIR=20190831-EMCNonLin
#     LHC13beData="";
#     LHC13bcData="589"
#     LHC13bData="child_1"
#     LHC13cData="child_2"
#     LHC13deData="590" #skim MB
#     LHC13dData="child_1"
#     LHC13eData="child_2"
#     LHC13fData="591" #skim MB
#     TRAINDIR=20190903-EMCtriggerStat
#     LHC13beData="";
#     LHC13bcData="602"
#     LHC13bData="child_1"
#     LHC13cData="child_2"
#     LHC13deData="603" #skim MB
#     LHC13dData="child_1"
#     LHC13eData="child_2"
#     LHC13fData="604" #skim MB
#     TRAINDIR=20190916-PCMEMCSys
#     LHC13beData="";
#     LHC13bcData="619"
#     LHC13bcData="620"
#     LHC13bcData="641"
#     LHC13bcData="621"
#     LHC13bData="child_1"
#     LHC13cData="child_2"
#     LHC13deData="612" #skim MB
#     LHC13deData="642" #skim MB
#     LHC13dData="child_1"
#     LHC13eData="child_2"
#     LHC13fData="613" #skim MB
#     LHC13fData="617" #skim MB
#     LHC13fData="643" #skim MB
#     TRAINDIR=20191010-FullStatAllMethods
#     LHC13beData="";
#     LHC13bcData="672"
#     LHC13bcData="673"
#     LHC13bData="child_1"
#     LHC13cData="child_2"
#     LHC13deData="668" #skim MB
#     LHC13deData="674" #skim MB
#     LHC13dData="child_1"
#     LHC13eData="child_2"
#     LHC13fData="667" #skim MB
#     LHC13fData="675" #skim MB

#     TRAINDIR=20191104-FullStatAllMethods
#     LHC13beData="";
#     LHC13bcData="685"
#     LHC13bcData="689"
#     LHC13bcData="698"
#     LHC13bData="child_1"
#     LHC13cData="child_2"
#     LHC13deData="686" #skim MB
#     LHC13deData="690" #skim MB
#     LHC13deData="699" #skim MB
#     LHC13dData="child_1"
#     LHC13eData="child_2"
#     LHC13fData="687" #skim MB
#     LHC13fData="691" #skim MB
#     LHC13fData="700" #skim MB

#     TRAINDIR=20191122-PCMCentDepAddPCM
#     LHC13beData="";
#     LHC13bcData="713"
#     LHC13bData="child_1"
#     LHC13cData="child_2"
#     LHC13deData="714" #skim MB
#     LHC13dData="child_1"
#     LHC13eData="child_2"
#     LHC13fData="715" #skim MB
#     TRAINDIR=20200106-EMCSysJJGT
#     LHC13beData="";
# #     LHC13bcData="729"
#     LHC13bcData="756"
#     LHC13bData="child_1"
#     LHC13cData="child_2"
# #     LHC13deData="730" #skim MB
#     LHC13deData="750" #skim MB
#     LHC13dData="child_1"
#     LHC13eData="child_2"
# #     LHC13fData="731" #skim MB
#     LHC13fData="755" #skim MB

#     TRAINDIR=20200130-RerunDataWOldTempCalib
#     LHC13beData="";
#     LHC13bcData="777"
#     LHC13bcData="778"
#     LHC13bcData="783"
#     LHC13bData="child_1"
#     LHC13cData="child_2"
#     LHC13deData="779" #skim MB
#     LHC13deData="780" #skim MB
#     LHC13deData="784" #skim MB
#     LHC13dData="child_1"
#     LHC13eData="child_2"
#     LHC13fData="781" #skim MB
#     LHC13fData="782" #skim MB
#     LHC13fData="785" #skim MB
#     LHC13fData="802" #skim MB
#     TRAINDIR=20200206-MixedEventUpdate
#     LHC13beData="";
#     LHC13bcData="805"
#     LHC13bData="child_1"
#     LHC13cData="child_2"
#     LHC13deData="806" #skim MB
#     LHC13dData="child_1"
#     LHC13eData="child_2"
#     LHC13fData="807" #skim MB
    
#     TRAINDIR=20200214-NLUpdate
#     LHC13beData="";
#     LHC13bcData="818"
# #     LHC13bcData="833"
#     LHC13bData="child_1"
#     LHC13cData="child_2"
#     LHC13deData="821" #skim MB
# #     LHC13deData="834" #skim MB
#     LHC13dData="child_1"
#     LHC13eData="child_2"
#     LHC13fData="823" #skim MB
# #     LHC13fData="835" #skim MB
    
    TRAINDIR=20200314-RerunAll
    LHC13beData="";
#     LHC13bcData="855"
#     LHC13bcData="863"
    LHC13bcData="875"
    LHC13bData="child_1"
    LHC13cData="child_2"
#     LHC13deData="857" #skim MB
#     LHC13deData="864" #skim MB
    LHC13deData="876" #skim MB
    LHC13dData="child_1"
    LHC13eData="child_2"
#     LHC13fData="858" #skim MB
#     LHC13fData="865" #skim MB
    LHC13fData="877" #skim MB
elif [ $3 = "AODSKIMEMC7" ]; then
    TRAINDIR=20190831-EMCNonLin2
    # LHC13beData="600" #skim EMC7
    # LHC13bData="child_1"
    # LHC13cData="child_2"
    # LHC13dData="child_3"
    # LHC13eData="child_4"
    # LHC13fData="596" #skim EMC7
elif [ $3 = "AODSKIMEGAJE" ]; then
    # TRAINDIR=20190831-EMCNonLin
    # LHC13beData="593" #skim EGA
    # LHC13bData="child_1"
    # LHC13cData="child_2"
    # LHC13dData="child_3"
    # LHC13eData="child_4"
    # LHC13fData="594" #skim EGA

#     TRAINDIR=20190903-EMCtriggerStat
#     LHC13beData="605" #skim EGA
#     LHC13bData="child_1"
#     LHC13cData="child_2"
#     LHC13dData="child_3"
#     LHC13eData="child_4"
#     LHC13fData="606" #skim EGA

#     TRAINDIR=20190916-PCMEMCSys
#     LHC13beData="627" #skim EGA
#     LHC13beData="634" #skim EGA
#     LHC13beData="645" #skim EGA
#     LHC13bData="child_1"
#     LHC13cData="child_2"
#     LHC13dData="child_3"
#     LHC13eData="child_4"
#     LHC13fData="628" #skim EGA
#     LHC13fData="635" #skim EGA
#     LHC13fData="646" #skim EGA
    
#     TRAINDIR=20191010-FullStatAllMethods
#     LHC13beData="670" #skim EGA
#     LHC13bData="child_1"
#     LHC13cData="child_2"
#     LHC13dData="child_3"
#     LHC13eData="child_4"
#     LHC13fData="671" #skim EGA
#     TRAINDIR=20191104-FullStatAllMethods
#     LHC13beData="693" #skim EGA
# #     LHC13beData="683" #skim EGA
#     LHC13bData="child_1"
#     LHC13cData="child_2"
#     LHC13dData="child_3"
#     LHC13eData="child_4"
#     LHC13fData="694" #skim EGA
#     LHC13fData="684" #skim EGA
    
#     TRAINDIR=20200106-EMCSysJJGT
#     LHC13beData="738" #skim EGA
#     LHC13bData="child_1"
#     LHC13cData="child_2"
#     LHC13dData="child_3"
#     LHC13eData="child_4"
#     LHC13fData="740" #skim EGA
#     LHC13beData="753" #skim EGA
#     LHC13bData="child_1"
#     LHC13cData="child_2"
#     LHC13dData="child_3"
#     LHC13eData="child_4"
#     LHC13fData="754" #skim EGA
    
#     TRAINDIR=20200130-RerunDataWOldTempCalib
#     LHC13beData="786" #skim EGA
#     LHC13beData="788" #skim EGA
#     LHC13beData="789" #skim EGA
#     LHC13beData="789" #skim EGA
#     LHC13bData="child_1"
#     LHC13cData="child_2"
#     LHC13dData="child_3"
#     LHC13eData="child_4"
#     LHC13fData="787" #skim EGA
#     LHC13fData="791" #skim EGA
#     LHC13fData="790" #skim EGA

#     TRAINDIR=20200206-MixedEventUpdate
#     LHC13beData="809" #skim EGA
#     LHC13bData="child_1"
#     LHC13cData="child_2"
#     LHC13dData="child_3"
#     LHC13eData="child_4"
#     LHC13fData="810" #skim EGA
    
#     TRAINDIR=20200214-NLUpdate
#     LHC13beData="820" #skim EGA
#     LHC13beData="831" #skim EGA
#     LHC13bData="child_1"
#     LHC13cData="child_2"
#     LHC13dData="child_3"
#     LHC13eData="child_4"
# #     LHC13fData="822" #skim EGA
#     LHC13fData="832" #skim EGA
    
    TRAINDIR=20200314-RerunAll
#     LHC13beData="859" #skim EGA
    LHC13beData="867" #skim EGA
    LHC13bData="child_1"
    LHC13cData="child_2"
    LHC13dData="child_3"
    LHC13eData="child_4"
#     LHC13fData="860" #skim EGA
    LHC13fData="868" #skim EGA
elif [ $3 = "AODSKIMPHI7" ]; then
#     TRAINDIR=20190831-EMCNonLin
#     LHC13beData="597" #skim PHI7
#     LHC13bData="child_1"
#     LHC13cData="child_2"
#     LHC13dData="child_3"
#     LHC13eData="child_4"
#     LHC13fData="598" #skim PHI7
#     TRAINDIR=20190903-PHOStriggerStat
#     LHC13beData="609" #skim PHI7
#     LHC13bData="child_1"
#     LHC13cData="child_2"
#     LHC13dData="child_3"
#     LHC13eData="child_4"
#     LHC13fData="610" #skim PHI7
    TRAINDIR=20200316-PHOS
    LHC13beData="861" #skim PHI7
    LHC13bData="child_1"
    LHC13cData="child_2"
    LHC13dData="child_3"
    LHC13eData="child_4"
    LHC13fData="862" #skim PHI7

else
#     TRAINDIR=20190831-EMCNonLin
    # LHC18j5MC="730"
    # LHC18j5_1MC="child_1"
    # LHC18j5_2MC="child_2"
    # LHC18j5_3MC="child_3"
#     TRAINDIR=20190903-EMCtriggerStat
#     LHC16qtData="601";
#     LHC16qDataFast="child_1";
#     LHC16tDataFast="child_2";
#     LHC16qData="child_3";
#     LHC16tData="child_4";
#     TRAINDIR=20190916-PCMEMCSys
#     LHC16qtData="622";
#     LHC16qtData="623";
#     LHC16qtData="637";
#     LHC16qtData="638";
#     LHC16qtData="644";
#     LHC16qDataFast="child_1";
#     LHC16tDataFast="child_2";
#     LHC16qData="child_3";
#     LHC16tData="child_4";
#     LHC18j5MC="756"
#     LHC18j5MC="768"
#     LHC18j5MC="774"
#     LHC18j5_1MC="child_1"
#     LHC18j5_2MC="child_2"
#     LHC18j5_3MC="child_3"
#     LHC18f3MCMoth="757";
#     LHC18f3MCMoth="761";
#     LHC18f3MCMoth="775";
#     LHC18f3MC1="child_2";
#     LHC18f3MC2="child_4";
#     LHC18f3MCFast1="child_1";
#     LHC18f3MCFast2="child_3";
    # LHC17f2a_fixMCMoth="1217";
    # LHC17f2a_fixMC="child_2";
    # LHC17f2a_fixMCFast="child_1";

#     LHC19a4MCMother="765";
#     LHC19a4MC1="child_1";
#     LHC19a4MC2="child_2";
#     LHC17g8aMCMoth="764"
#     LHC17g8aMC="child_2"
#     LHC17g8aMCFast="child_1"

#     TRAINDIR=20191010-FullStatAllMethods
#     LHC16qtData="669";
#     LHC16qtData="676";
#     LHC16qDataFast="child_1";
#     LHC16tDataFast="child_2";
#     LHC16qData="child_3";
#     LHC16tData="child_4";
    
#     TRAINDIR=20191104-FullStatAllMethods
#     LHC16qtData="688";
#     LHC16qtData="692";
#     LHC16qtData="695";
#     LHC16qDataFast="child_1";
#     LHC16tDataFast="child_2";
#     LHC16qData="child_3";
#     LHC16tData="child_4";

#     LHC18j5MC="790"
#     LHC18j5MC="791"
#     LHC18j5MC="803"
#     LHC18j5MC="810"
#     LHC18j5_1MC="child_1"
#     LHC18j5_2MC="child_2"
#     LHC18j5_3MC="child_3"
#     LHC18f3MCMoth="789";
#     LHC18f3MCMoth="792";
#     LHC18f3MCMoth="802";
#     LHC18f3MCMoth="794";
#     LHC18f3MC1="child_2";
#     LHC18f3MC2="child_4";
#     LHC18f3MCFast1="child_1";
#     LHC18f3MCFast2="child_3";
#     LHC19a4MCMother="793";
#     LHC19a4MCMother="798";
#     LHC19a4MC1="child_1";
# #     LHC19a4MC2="child_2";

#     TRAINDIR=20191120-FullStatAllMethods
#     LHC19a4MCMother="808";
#     LHC19a4MC1="child_1";
#     LHC19a4MC2="child_2";
#     TRAINDIR=20191123-OutlierRemovalJetFinder
#     LHC19a4MCMother="820";
#     LHC19a4MC1="child_1";
#     LHC19a4MC2="child_2";

#     TRAINDIR=20191121-PHOSTestAndPCMEMCSys
#     LHC19a4MCMother="816";
#     LHC19a4MC1="child_1";
#     LHC19a4MC2="child_2";
    
#     TRAINDIR=20191122-PCMCentDepAddPCM
#     LHC16qtData="716";
#     LHC16qDataFast="child_1";
#     LHC16tDataFast="child_2";
#     LHC16qData="child_3";
#     LHC16tData="child_4";
    
#     TRAINDIR=20191205-TestEMCJJTriggered
#     LHC16c3aMC1="840";
#     LHC16c3aMC2="839";
#     LHC16c3bMC1="841";
#     LHC16c3bMC2="842";
#     LHC17g6a2MC="835";
#     LHC17g6a3MC="828";
#     LHC19a4MCMother="834";
#     LHC19a4MC1="child_1";
#     LHC19a4MC2="child_2";

#     TRAINDIR=20191212-TestEMCJJTriggered
#     LHC16c3aMC1="863";
#     LHC16c3aMC2="862";
#     LHC16c3bMC1="864";
#     LHC16c3bMC2="865";
#     LHC17g6a2MC="877";
#     LHC17g6a3MC="878";

#     TRAINDIR=20200106-EMCSysJJGT
#     LHC16qtData="737";
#     LHC16qtData="752";
#     LHC16qDataFast="child_1";
#     LHC16tDataFast="child_2";
#     LHC16qData="child_3";
#     LHC16tData="child_4";
#     LHC16c3aMC1="892";
#     LHC16c3aMC2="891";
#     LHC16c3bMC1="894";
#     LHC16c3bMC2="893";
#     LHC17g6a2MC="895";
#     LHC17g6a3MC="896";
#     LHC16c3aMC1="898";
#     LHC16c3aMC2="897";
#     LHC16c3bMC1="900";
#     LHC16c3bMC2="899";
#     LHC17g6a2MC="901";
#     LHC17g6a3MC="902";
#     LHC18j5MC="952"
#     LHC18j5MC="1009"
#     LHC18j5MC="1010"
#     LHC18j5_1MC="child_1"
#     LHC18j5_2MC="child_2"
#     LHC18j5_3MC="child_3"
#     LHC18f3MCMoth="951";
#     LHC18f3MCMoth="1008";
#     LHC18f3MCMoth="1000";
#     LHC18f3MC1="child_2";
#     LHC18f3MC2="child_4";
#     LHC18f3MCFast1="child_1";
#     LHC18f3MCFast2="child_3";
#     LHC16c3aMC1="1012";
#     LHC16c3aMC2="1011";
#     LHC16c3bMC1="1014";
#     LHC16c3bMC2="1013";
#     LHC17g6a2MC="1015";
#     LHC17g6a3MC="1016";
    
#     TRAINDIR=20200106-PCMEMCSysJJGT
#     #EG1 - 1
#     LHC16c3aMC1="907";
#     LHC16c3aMC2="903";
#     LHC16c3bMC1="915";
#     LHC16c3bMC2="911";
#     LHC17g6a2MC="919";
#     LHC17g6a3MC="923";
#     #EG1 - 2
#     LHC16c3aMC1="908";
#     LHC16c3aMC2="904";
#     LHC16c3bMC1="916";
#     LHC16c3bMC2="912";
#     LHC17g6a2MC="920";
#     LHC17g6a3MC="924";
#     #EG1 - 3
#     LHC16c3aMC1="909";
#     LHC16c3aMC2="905";
#     LHC16c3bMC1="917";
#     LHC16c3bMC2="913";
#     LHC17g6a2MC="921";
#     LHC17g6a3MC="925";
# #     #EG1 - 4
#     LHC16c3aMC1="910";
#     LHC16c3aMC2="906";
#     LHC16c3bMC1="918";
#     LHC16c3bMC2="914";
#     LHC17g6a2MC="922";
#     LHC17g6a3MC="926";
#     #EG2 - 1
#     LHC16c3aMC1="931";
#     LHC16c3aMC2="927";
#     LHC16c3bMC1="939";
#     LHC16c3bMC2="935";
#     LHC17g6a2MC="943";
#     LHC17g6a3MC="947";
#     #EG2 - 2
#     LHC16c3aMC1="932";
#     LHC16c3aMC2="928";
#     LHC16c3bMC1="940";
#     LHC16c3bMC2="936";
#     LHC17g6a2MC="944";
#     LHC17g6a3MC="948";
#     #EG2 - 3
#     LHC16c3aMC1="933";
#     LHC16c3aMC2="929";
#     LHC16c3bMC1="941";
#     LHC16c3bMC2="937";
#     LHC17g6a2MC="945";
#     LHC17g6a3MC="949";
#     #EG2 - 4 + INT7 - 4 
#     LHC16c3aMC1="934";
#     LHC16c3aMC2="930";
#     LHC16c3bMC1="942";
#     LHC16c3bMC2="938";
#     LHC17g6a2MC="946";
#     LHC17g6a3MC="950";
#     #INT - 1
#     LHC16c3aMC1="977";
#     LHC16c3aMC2="974";
#     LHC16c3bMC1="983";
#     LHC16c3bMC2="980";
#     LHC17g6a2MC="986";
#     LHC17g6a3MC="989";

#     LHC18j5MC="996"
#     LHC18j5MC="997"
#     LHC18j5MC="998"
#     LHC18j5_1MC="child_1"
#     LHC18j5_2MC="child_2"
#     LHC18j5_3MC="child_3"
#     LHC18f3MCMoth="992";
#     LHC18f3MCMoth="993";
#     LHC18f3MCMoth="994";
#     LHC18f3MC1="child_2";
#     LHC18f3MC2="child_4";
#     LHC18f3MCFast1="child_1";
#     LHC18f3MCFast2="child_3";

#     TRAINDIR=20200118-EMCSingleRunUnskimmed
#     LHC13dData="762"
#     LHC13eData="763"
#     LHC13fData="759"

#     TRAINDIR=20200130-RerunDataWOldTempCalib
#     LHC16qtData="772";
#     LHC16qtData="773";
#     LHC16qtData="792";
#     LHC16qDataFast="child_1";
#     LHC16tDataFast="child_2";
#     LHC16qData="child_3";
#     LHC16tData="child_4";

#     TRAINDIR=20200206-MixedEventUpdate
#     LHC16c3aMC1="1035";
#     LHC16c3aMC2="1034";
#     LHC16c3bMC1="1037";
#     LHC16c3bMC2="1036";
#     LHC17g6a2MC="1038";
#     LHC17g6a3MC="1039";
#     LHC18j5MC="1041"
#     LHC18j5_1MC="child_1"
#     LHC18j5_2MC="child_2"
#     LHC18j5_3MC="child_3"
#     LHC18f3MCMoth="1040";
#     LHC18f3MC1="child_2";
#     LHC18f3MC2="child_4";
#     LHC18f3MCFast1="child_1";
#     LHC18f3MCFast2="child_3";
#     LHC16qtData="808";
#     LHC16qDataFast="child_1";
#     LHC16tDataFast="child_2";
#     LHC16qData="child_3";
#     LHC16tData="child_4";

#     TRAINDIR=20200214-NLUpdate
#     LHC16c3aMC1="1087";
#     LHC16c3aMC2="1086";
#     LHC16c3aMC1="1093";
#     LHC16c3aMC2="1092";
#     LHC16c3bMC1="1089";
#     LHC16c3bMC2="1088";
#     LHC17g6a2MC="1090";
#     LHC17g6a2MC="1094";
#     LHC17g6a3MC="1091";
#     LHC18j5MC="1097"
#     LHC18j5MC="1098"
#     LHC18j5_1MC="child_1"
#     LHC18j5_2MC="child_2"
#     LHC18j5_3MC="child_3"
#     LHC18f3MCMoth="1060";
#     LHC18f3MCMoth="1095";
#     LHC18f3MCMoth="1096";
#     LHC18f3MC1="child_2";
#     LHC18f3MC2="child_4";
#     LHC18f3MCFast1="child_1";
#     LHC18f3MCFast2="child_3";
#     LHC16qtData="819";
#     LHC16qtData="836";
#     LHC16qDataFast="child_1";
#     LHC16tDataFast="child_2";
#     LHC16qData="child_3";
#     LHC16tData="child_4";
    
    TRAINDIR=20200314-RerunAll
#     LHC16c3aMC1="1125";
#     LHC16c3aMC2="1124";
#     LHC16c3aMC1="1134";
#     LHC16c3aMC2="1133";
#     LHC16c3aMC1="1231";
#     LHC16c3aMC2="1230";
#     LHC16c3aMC1="1224";
#     LHC16c3aMC2="1223";
#     LHC16c3aMC1="1217";
#     LHC16c3aMC2="1216";

#     LHC16c3bMC1="1127";
#     LHC16c3bMC2="1126";
#     LHC16c3bMC1="1233";
#     LHC16c3bMC2="1232";
#     LHC16c3bMC1="1226";
#     LHC16c3bMC2="1225";
#     LHC16c3bMC1="1219";
#     LHC16c3bMC2="1218";

#     LHC17g6a2MC="1128";
#     LHC17g6a2MC="1135";
#     LHC17g6a2MC="1234";
#     LHC17g6a2MC="1227";
#     LHC17g6a2MC="1220";
#     LHC17g6a3MC="1129";
#     LHC17g6a3MC="1235";
#     LHC17g6a3MC="1228";
#     LHC17g6a3MC="1221";
#     LHC18j5MC="1138"
#     LHC18j5MC="1196"
#     LHC18j5MC="1199"
#     LHC18j5_1MC="child_1"
#     LHC18j5_2MC="child_2"
#     LHC18j5_3MC="child_3"
#     LHC18f3MCMoth="1131";
#     LHC18f3MCMoth="1137";
#     LHC18f3MCMoth="1195";
#     LHC18f3MCMoth="1200";
#     LHC18f3MCMoth="1201";
#     LHC18f3MC1="child_2";
#     LHC18f3MC2="child_4";
#     LHC18f3MCFast1="child_1";
#     LHC18f3MCFast2="child_3";
#     LHC19a4MCMother="1130";
#     LHC19a4MCMother="1136";
#     LHC19a4MCMother="1236";
#     LHC19a4MCMother="1229";
#     LHC19a4MCMother="1222";
#     LHC19a4MCMother="1237";
#     LHC19a4MC1="child_1";
#     LHC19a4MC2="child_2";
#     LHC16qtData="856";
    LHC16qtData="866";
#     LHC16qDataFast="child_1";
    LHC16tDataFast="child_2";
#     LHC16qData="child_3";
#     LHC16tData="child_4";
    
#     TRAINDIR=20200316-PHOS
#     LHC16c3aMC1="1150";
#     LHC16c3aMC2="1158";
#     LHC16c3aMC1="1141";
#     LHC16c3aMC2="1156";
#     LHC16c3aMC1="1161";
#     LHC16c3aMC2="1160";
#     LHC16c3aMC1="1168";
#     LHC16c3aMC2="1167";
#     LHC16c3bMC1="1152";
#     LHC16c3bMC2="1151";
#     LHC16c3bMC1="1143";
#     LHC16c3bMC2="1157";
#     LHC16c3bMC1="1163";
#     LHC16c3bMC2="1162";
#     LHC16c3bMC1="1170";
#     LHC16c3bMC2="1169";
#     LHC17g6a2MC="1153";
#     LHC17g6a2MC="1144";
#     LHC17g6a2MC="1164";
#     LHC17g6a2MC="1171";
#     LHC17g6a3MC="1154";
#     LHC17g6a3MC="1145";
#     LHC17g6a3MC="1165";
#     LHC17g6a3MC="1172";
#     LHC19a4MCMother="1149";
#     LHC19a4MCMother="1155";
#     LHC19a4MCMother="1166";
#     LHC19a4MCMother="1173";
#     LHC19a4MC1="child_1";
#     LHC19a4MC2="child_2";

#     LHC18j5MC="1147"
#     LHC18j5_1MC="child_1"
#     LHC18j5_2MC="child_2"
#     LHC18j5_3MC="child_3"
#     LHC18f3MCMoth="1146";
#     LHC18f3MC1="child_2";
#     LHC18f3MC2="child_4";
#     LHC18f3MCFast1="child_1";
#     LHC18f3MCFast2="child_3";

fi


OUTPUTDIR=$BASEDIR/$TRAINDIR
ALIENDIRData="/alice/cern.ch/user/a/alitrain/PWGGA/$baseLegoData/"
OUTPUTDIRData=$BASEDIR/$TRAINDIR/$baseLegoData
ALIENDIRMC="/alice/cern.ch/user/a/alitrain/PWGGA/$baseLegoMC/"
OUTPUTDIRMC=$BASEDIR/$TRAINDIR/$baseLegoMC
mkdir -p $OUTPUTDIR/CutSelections
mkdir -p $OUTPUTDIR/SinglePeriods

# finding run1 data paths
if [ "$LHC13beData" == "" ] && [ $3 = "AOD" ]; then
  FindCorrectTrainDirectory $LHC13bData $OUTPUTDIRData $ALIENDIRData 
  HAVELHC13b=$tempBool
  LHC13bData=$tempDir
  OUTPUTDIR_LHC13b=$tempPath
  echo "13b: $HAVELHC13b $LHC13bData $OUTPUTDIR_LHC13b"
  FindCorrectTrainDirectory $LHC13cData $OUTPUTDIRData $ALIENDIRData 
  HAVELHC13c=$tempBool
  LHC13cData=$tempDir
  OUTPUTDIR_LHC13c=$tempPath
  echo "13c: $HAVELHC13c $LHC13cData $OUTPUTDIR_LHC13c"
  FindCorrectTrainDirectory $LHC13dData $OUTPUTDIRData $ALIENDIRData 
  HAVELHC13d=$tempBool
  LHC13dData=$tempDir
  OUTPUTDIR_LHC13d=$tempPath
  echo "13d: $HAVELHC13d $LHC13dData $OUTPUTDIR_LHC13d"
  FindCorrectTrainDirectory $LHC13eData $OUTPUTDIRData $ALIENDIRData 
  HAVELHC13e=$tempBool
  LHC13eData=$tempDir
  OUTPUTDIR_LHC13e=$tempPath
  echo "13e: $HAVELHC13e $LHC13eData $OUTPUTDIR_LHC13e"
elif [ "$LHC13beData" == "" ]; then
  FindCorrectTrainDirectory $LHC13bData $OUTPUTDIRData $ALIENDIRData $LHC13bcData
  HAVELHC13b=$tempBool
  LHC13bData=$tempDir
  OUTPUTDIR_LHC13b=$tempPath
  echo "13b: $HAVELHC13b $LHC13bData $OUTPUTDIR_LHC13b"
  FindCorrectTrainDirectory $LHC13cData $OUTPUTDIRData $ALIENDIRData $LHC13bcData
  HAVELHC13c=$tempBool
  LHC13cData=$tempDir
  OUTPUTDIR_LHC13c=$tempPath
  echo "13c: $HAVELHC13c $LHC13cData $OUTPUTDIR_LHC13c"
  FindCorrectTrainDirectory $LHC13dData $OUTPUTDIRData $ALIENDIRData $LHC13deData
  HAVELHC13d=$tempBool
  LHC13dData=$tempDir
  OUTPUTDIR_LHC13d=$tempPath
  echo "13d: $HAVELHC13d $LHC13dData $OUTPUTDIR_LHC13d"
  FindCorrectTrainDirectory $LHC13eData $OUTPUTDIRData $ALIENDIRData $LHC13deData
  HAVELHC13e=$tempBool
  LHC13eData=$tempDir
  OUTPUTDIR_LHC13e=$tempPath
  echo "13e: $HAVELHC13e $LHC13eData $OUTPUTDIR_LHC13e"
else
  FindCorrectTrainDirectory $LHC13bData $OUTPUTDIRData $ALIENDIRData $LHC13beData
  HAVELHC13b=$tempBool
  LHC13bData=$tempDir
  OUTPUTDIR_LHC13b=$tempPath
  echo "13b: $HAVELHC13b $LHC13bData $OUTPUTDIR_LHC13b"
  FindCorrectTrainDirectory $LHC13cData $OUTPUTDIRData $ALIENDIRData $LHC13beData
  HAVELHC13c=$tempBool
  LHC13cData=$tempDir
  OUTPUTDIR_LHC13c=$tempPath
  echo "13c: $HAVELHC13c $LHC13cData $OUTPUTDIR_LHC13c"
  FindCorrectTrainDirectory $LHC13dData $OUTPUTDIRData $ALIENDIRData $LHC13beData
  HAVELHC13d=$tempBool
  LHC13dData=$tempDir
  OUTPUTDIR_LHC13d=$tempPath
  echo "13d: $HAVELHC13d $LHC13dData $OUTPUTDIR_LHC13d"
  FindCorrectTrainDirectory $LHC13eData $OUTPUTDIRData $ALIENDIRData $LHC13beData
  HAVELHC13e=$tempBool
  LHC13eData=$tempDir
  OUTPUTDIR_LHC13e=$tempPath
  echo "13e: $HAVELHC13e $LHC13eData $OUTPUTDIR_LHC13e"
fi

FindCorrectTrainDirectory $LHC13fData $OUTPUTDIRData $ALIENDIRData
HAVELHC13f=$tempBool
LHC13fData=$tempDir
OUTPUTDIR_LHC13f=$tempPath
echo "13f: $HAVELHC13f $LHC13fData $OUTPUTDIR_LHC13f"

# finding run2 data paths
FindCorrectTrainDirectory $LHC16qData $OUTPUTDIRData $ALIENDIRData $LHC16qtData
HAVELHC16q=$tempBool
LHC16qData=$tempDir
OUTPUTDIR_LHC16q=$tempPath
echo "16q: $HAVELHC16q $LHC16qData $OUTPUTDIR_LHC16q"
FindCorrectTrainDirectory $LHC16tData $OUTPUTDIRData $ALIENDIRData $LHC16qtData
HAVELHC16t=$tempBool
LHC16tData=$tempDir
OUTPUTDIR_LHC16t=$tempPath
echo "16t: $HAVELHC16t $LHC16tData $OUTPUTDIR_LHC16t"
FindCorrectTrainDirectory $LHC16qDataFast $OUTPUTDIRData $ALIENDIRData $LHC16qtData
HAVELHC16qF=$tempBool
LHC16qDataFast=$tempDir
OUTPUTDIR_LHC16qF=$tempPath
echo "16q: $HAVELHC16qF $LHC16qDataFast $OUTPUTDIR_LHC16qF"
FindCorrectTrainDirectory $LHC16tDataFast $OUTPUTDIRData $ALIENDIRData $LHC16qtData
HAVELHC16tF=$tempBool
LHC16tDataFast=$tempDir
OUTPUTDIR_LHC16tF=$tempPath
echo "16t: $HAVELHC16tF $LHC16tDataFast $OUTPUTDIR_LHC16tF"

# finding run1 MC path
FindCorrectTrainDirectory $LHC18j5_1MC $OUTPUTDIRMC $ALIENDIRMC $LHC18j5MC
HAVELHC18j51=$tempBool
LHC18j5_1MC=$tempDir
OUTPUTDIR_LHC18j51=$tempPath
echo "18j5_1 anchored to 13bf: $HAVELHC18j51 $LHC18j5_1MC $OUTPUTDIR_LHC18j51"
FindCorrectTrainDirectory $LHC18j5_2MC $OUTPUTDIRMC $ALIENDIRMC $LHC18j5MC
HAVELHC18j52=$tempBool
LHC18j5_2MC=$tempDir
OUTPUTDIR_LHC18j52=$tempPath
echo "18j5_2 anchored to 13bf: $HAVELHC18j52 $LHC18j5_2MC $OUTPUTDIR_LHC18j52"
FindCorrectTrainDirectory $LHC18j5_3MC $OUTPUTDIRMC $ALIENDIRMC $LHC18j5MC
HAVELHC18j53=$tempBool
LHC18j5_3MC=$tempDir
OUTPUTDIR_LHC18j53=$tempPath
echo "18j5_3 anchored to 13bf: $HAVELHC18j53 $LHC18j5_3MC $OUTPUTDIR_LHC18j53"

FindCorrectTrainDirectory $LHC19a4MC1 $OUTPUTDIRMC $ALIENDIRMC $LHC19a4MCMother
HAVELHC19a41=$tempBool
LHC19a4MC1=$tempDir
OUTPUTDIR_LHC19a41=$tempPath
echo "19a4_1 JJ anchored to 13bf: $HAVELHC19a41 $LHC19a4MC1 $OUTPUTDIR_LHC19a41"
FindCorrectTrainDirectory $LHC19a4MC2 $OUTPUTDIRMC $ALIENDIRMC $LHC19a4MCMother
HAVELHC19a42=$tempBool
LHC19a4MC2=$tempDir
OUTPUTDIR_LHC19a42=$tempPath
echo "19a4_2 JJ anchored to 13bf: $HAVELHC19a42 $LHC19a4MC2 $OUTPUTDIR_LHC19a42"

FindCorrectTrainDirectory $LHC16c3aMC1 $OUTPUTDIRMC $ALIENDIRMC 
HAVELHC16c3a1=$tempBool
LHC16c3aMC1=$tempDir
OUTPUTDIR_LHC16c3a1=$tempPath
echo "16c3a JJ trigg EMC low anchored to 13cd: $HAVELHC16c3a1 $LHC16c3aMC1 $OUTPUTDIR_LHC16c3a1"
FindCorrectTrainDirectory $LHC16c3aMC2 $OUTPUTDIRMC $ALIENDIRMC 
HAVELHC16c3a2=$tempBool
LHC16c3aMC2=$tempDir
OUTPUTDIR_LHC16c3a2=$tempPath
echo "16c3a2 JJ trigg EMC low anchored to 13f: $HAVELHC16c3a2 $LHC16c3aMC2 $OUTPUTDIR_LHC16c3a2"
FindCorrectTrainDirectory $LHC16c3bMC1 $OUTPUTDIRMC $ALIENDIRMC 
HAVELHC16c3b1=$tempBool
LHC16c3bMC1=$tempDir
OUTPUTDIR_LHC16c3b1=$tempPath
echo "16c3b JJ trigg EMC high anchored to 13cd: $HAVELHC16c3b1 $LHC16c3bMC1 $OUTPUTDIR_LHC16c3b1"
FindCorrectTrainDirectory $LHC16c3bMC2 $OUTPUTDIRMC $ALIENDIRMC 
HAVELHC16c3b2=$tempBool
LHC16c3bMC2=$tempDir
OUTPUTDIR_LHC16c3b2=$tempPath
echo "16c3b2 JJ trigg EMC high anchored to 13f: $HAVELHC16c3b2 $LHC16c3bMC2 $OUTPUTDIR_LHC16c3b2"

FindCorrectTrainDirectory $LHC17g6a2MC $OUTPUTDIRMC $ALIENDIRMC 
HAVELHC17g6a2=$tempBool
LHC17g6a2MC=$tempDir
OUTPUTDIR_LHC17g6a2=$tempPath
echo "17g6a2 JJ trigg EMC low anchored to 13def: $HAVELHC17g6a2 $LHC17g6a2MC $OUTPUTDIR_LHC17g6a2"
FindCorrectTrainDirectory $LHC17g6a3MC $OUTPUTDIRMC $ALIENDIRMC 
HAVELHC17g6a3=$tempBool
LHC17g6a3MC=$tempDir
OUTPUTDIR_LHC17g6a3=$tempPath
echo "17g6a3 JJ trigg EMC high anchored to 13def: $HAVELHC17g6a3 $LHC17g6a3MC $OUTPUTDIR_LHC17g6a3"


# finding run2 MC path
FindCorrectTrainDirectory $LHC18f3MC1 $OUTPUTDIRMC $ALIENDIRMC $LHC18f3MCMoth
HAVELHC18f31=$tempBool
LHC18f3MC1=$tempDir
OUTPUTDIR_LHC18f31=$tempPath
echo "18f3_1 anchored to 16qt: $HAVELHC18f31 $LHC18f3MC1 $OUTPUTDIR_LHC18f31"
FindCorrectTrainDirectory $LHC18f3MC2 $OUTPUTDIRMC $ALIENDIRMC $LHC18f3MCMoth
HAVELHC18f32=$tempBool
LHC18f3MC2=$tempDir
OUTPUTDIR_LHC18f32=$tempPath
echo "18f3_2 anchored to 16qt: $HAVELHC18f32 $LHC18f3MC2 $OUTPUTDIR_LHC18f32"
FindCorrectTrainDirectory $LHC18f3MCFast1 $OUTPUTDIRMC $ALIENDIRMC $LHC18f3MCMoth
HAVELHC18f3F1=$tempBool
LHC18f3MCFast1=$tempDir
OUTPUTDIR_LHC18f3F1=$tempPath
echo "18f3_fast_1 anchored to 16qt: $HAVELHC18f3F1 $LHC18f3MCFast1 $OUTPUTDIR_LHC18f3F1"
FindCorrectTrainDirectory $LHC18f3MCFast2 $OUTPUTDIRMC $ALIENDIRMC $LHC18f3MCMoth
HAVELHC18f3F2=$tempBool
LHC18f3MCFast2=$tempDir
OUTPUTDIR_LHC18f3F2=$tempPath
echo "18f3_fast_2 anchored to 16qt: $HAVELHC18f3F2 $LHC18f3MCFast2 $OUTPUTDIR_LHC18f3F2"
FindCorrectTrainDirectory $LHC17f2a_fixMC $OUTPUTDIRMC $ALIENDIRMC $LHC17f2a_fixMCMoth
HAVELHC17f2afix=$tempBool
LHC17f2a_fixMC=$tempDir
OUTPUTDIR_LHC17f2a_fix=$tempPath
echo "17f2a_fix anchored to 16qt: $HAVELHC17f2afix $LHC17f2a_fixMC $OUTPUTDIR_LHC17f2a_fix"
FindCorrectTrainDirectory $LHC17f2a_fixMCFast $OUTPUTDIRMC $ALIENDIRMC $LHC17f2a_fixMCMoth
HAVELHC17f2afixF=$tempBool
LHC17f2a_fixMCFast=$tempDir
OUTPUTDIR_LHC17f2a_fixF=$tempPath
echo "17f2a_fix_fast anchored to 16qt: $HAVELHC17f2afixF $LHC17f2a_fixMCFast $OUTPUTDIR_LHC17f2a_fixF"
FindCorrectTrainDirectory $LHC17g8aMC $OUTPUTDIRMC $ALIENDIRMC $LHC17g8aMCMoth
HAVELHC17g8a=$tempBool
LHC17g8aMC=$tempDir
OUTPUTDIR_LHC17g8a=$tempPath
echo "17g8a anchored to 16qt: $HAVELHC17g8a $LHC17g8aMC $OUTPUTDIR_LHC17g8a"
FindCorrectTrainDirectory $LHC17g8aMCFast $OUTPUTDIRMC $ALIENDIRMC $LHC17g8aMCMoth
HAVELHC17g8aF=$tempBool
LHC17g8aMCFast=$tempDir
OUTPUTDIR_LHC17g8aF=$tempPath
echo "17g8a_fast anchored to 16qt: $HAVELHC17g8aF $LHC17g8aMCFast $OUTPUTDIR_LHC17g8aF"


currentDir=$PWD
if [ $CLEANUPMAYOR == 0 ]; then
    cd $currentDir
    CopyRunwiseAndMergeAccordingToRunlistData "LHC13b" $HAVELHC13b $OUTPUTDIR_LHC13b $LHC13bData $pathDataR1b $baseLegoData "/alice/data/2013" $NSlashes3 runlistsToMerge.txt "pass4" $FILENAMEBASE
    cd $currentDir
    CopyRunwiseAndMergeAccordingToRunlistData "LHC13c" $HAVELHC13c $OUTPUTDIR_LHC13c $LHC13cData $pathDataR1c $baseLegoData "/alice/data/2013" $NSlashes3 runlistsToMerge.txt "pass4" $FILENAMEBASE
    cd $currentDir
    CopyRunwiseAndMergeAccordingToRunlistData "LHC13d" $HAVELHC13d $OUTPUTDIR_LHC13d $LHC13dData $pathDataR1d $baseLegoData "/alice/data/2013" $NSlashes3 runlistsToMerge.txt "pass4" $FILENAMEBASE
    cd $currentDir
    CopyRunwiseAndMergeAccordingToRunlistData "LHC13e" $HAVELHC13e $OUTPUTDIR_LHC13e $LHC13eData $pathDataR1e $baseLegoData "/alice/data/2013" $NSlashes3 runlistsToMerge.txt "pass4" $FILENAMEBASE
    cd $currentDir
    CopyRunwiseAndMergeAccordingToRunlistData "LHC13f" $HAVELHC13f $OUTPUTDIR_LHC13f $LHC13fData $pathDataR1f $baseLegoData "/alice/data/2013" $NSlashes3 runlistsToMerge.txt "pass4" $FILENAMEBASE

    cd $currentDir
    CopyRunwiseAndMergeAccordingToRunlistMC "LHC18j5_1" $HAVELHC18j51 $OUTPUTDIR_LHC18j51 $LHC18j5_1MC $pathMCR1 $baseLegoMC "/alice/sim/2018" $NSlashes3 runlistsToMerge.txt $FILENAMEBASE
    cd $currentDir
    CopyRunwiseAndMergeAccordingToRunlistMC "LHC18j5_2" $HAVELHC18j52 $OUTPUTDIR_LHC18j52 $LHC18j5_2MC $pathMCR1 $baseLegoMC "/alice/sim/2018" $NSlashes3 runlistsToMerge.txt $FILENAMEBASE
    cd $currentDir
    CopyRunwiseAndMergeAccordingToRunlistMC "LHC18j5_3" $HAVELHC18j53 $OUTPUTDIR_LHC18j53 $LHC18j5_3MC $pathMCR1 $baseLegoMC "/alice/sim/2018" $NSlashes3 runlistsToMerge.txt $FILENAMEBASE
    cd $currentDir
    CopyRunwiseAndMergeAccordingToRunlistJJMC "LHC19a4_1" $HAVELHC19a41 $OUTPUTDIR_LHC19a41 $LHC19a4MC1 $pathMCR1 $baseLegoMC "/alice/sim/2019" $NSlashes3 runlistsToMerge.txt $FILENAMEBASE runlists/binsJetJetLHC19a4_1.txt
    cd $currentDir
    CopyRunwiseAndMergeAccordingToRunlistJJMC "LHC19a4_2" $HAVELHC19a42 $OUTPUTDIR_LHC19a42 $LHC19a4MC2 $pathMCR1 $baseLegoMC "/alice/sim/2019" $NSlashes3 runlistsToMerge.txt $FILENAMEBASE runlists/binsJetJetLHC19a4_2.txt

    cd $currentDir
    CopyRunwiseAndMergeAccordingToRunlistJJMC "LHC16c3a" $HAVELHC16c3a1 $OUTPUTDIR_LHC16c3a1 $LHC16c3aMC1 $pathMCR1 $baseLegoMC "/alice/sim/2016" $NSlashes3 runlistsToMerge.txt $FILENAMEBASE runlists/binsJetJetLHC16c3a.txt
    cd $currentDir
    CopyRunwiseAndMergeAccordingToRunlistJJMC "LHC16c3a2" $HAVELHC16c3a2 $OUTPUTDIR_LHC16c3a2 $LHC16c3aMC2 $pathMCR1 $baseLegoMC "/alice/sim/2016" $NSlashes3 runlistsToMerge.txt $FILENAMEBASE runlists/binsJetJetLHC16c3a2.txt
    cd $currentDir
    CopyRunwiseAndMergeAccordingToRunlistJJMC "LHC16c3b" $HAVELHC16c3b1 $OUTPUTDIR_LHC16c3b1 $LHC16c3bMC1 $pathMCR1 $baseLegoMC "/alice/sim/2016" $NSlashes3 runlistsToMerge.txt $FILENAMEBASE runlists/binsJetJetLHC16c3b.txt
    cd $currentDir
    CopyRunwiseAndMergeAccordingToRunlistJJMC "LHC16c3b2" $HAVELHC16c3b2 $OUTPUTDIR_LHC16c3b2 $LHC16c3bMC2 $pathMCR1 $baseLegoMC "/alice/sim/2016" $NSlashes3 runlistsToMerge.txt $FILENAMEBASE runlists/binsJetJetLHC16c3b2.txt
    cd $currentDir
    CopyRunwiseAndMergeAccordingToRunlistJJMC "LHC17g6a2" $HAVELHC17g6a2 $OUTPUTDIR_LHC17g6a2 $LHC17g6a2MC $pathMCR1 $baseLegoMC "/alice/sim/2017" $NSlashes3 runlistsToMerge.txt $FILENAMEBASE runlists/binsJetJetLHC17g6a2.txt
    cd $currentDir
    CopyRunwiseAndMergeAccordingToRunlistJJMC "LHC17g6a3" $HAVELHC17g6a3 $OUTPUTDIR_LHC17g6a3 $LHC17g6a3MC $pathMCR1 $baseLegoMC "/alice/sim/2017" $NSlashes3 runlistsToMerge.txt $FILENAMEBASE runlists/binsJetJetLHC17g6a3.txt

    
    
    cd $currentDir
    echo "LHC16q" $HAVELHC16q $OUTPUTDIR_LHC16q $LHC16qData $pathDataR2WOSDD $baseLegoData "/alice/data/2016" $NSlashes3 runlistsToMerge.txt "pass1$WOSDD" $FILENAMEBASE
    CopyRunwiseAndMergeAccordingToRunlistData "LHC16q" $HAVELHC16q $OUTPUTDIR_LHC16q $LHC16qData $pathDataR2WOSDD $baseLegoData "/alice/data/2016" $NSlashes3 runlistsToMerge.txt "pass1$WOSDD" $FILENAMEBASE
    cd $currentDir
    CopyRunwiseAndMergeAccordingToRunlistData "LHC16q" $HAVELHC16qF $OUTPUTDIR_LHC16qF $LHC16qDataFast $pathDataR2FAST $baseLegoData "/alice/data/2016" $NSlashes3 runlistsToMerge.txt "pass1$FAST" $FILENAMEBASE
    cd $currentDir
    CopyRunwiseAndMergeAccordingToRunlistData "LHC16t" $HAVELHC16t $OUTPUTDIR_LHC16t $LHC16tData $pathDataR2WOSDD $baseLegoData "/alice/data/2016" $NSlashes3 runlistsToMerge.txt "pass1$WOSDD" $FILENAMEBASE
    cd $currentDir
    echo "LHC16t" $HAVELHC16tF $OUTPUTDIR_LHC16tF $LHC16tDataFast $pathDataR2FAST $baseLegoData "/alice/data/2016" $NSlashes3 runlistsToMerge.txt "pass1$FAST" $FILENAMEBASE
    CopyRunwiseAndMergeAccordingToRunlistData "LHC16t" $HAVELHC16tF $OUTPUTDIR_LHC16tF $LHC16tDataFast $pathDataR2FAST $baseLegoData "/alice/data/2016" $NSlashes3 runlistsToMerge.txt "pass1$FAST" $FILENAMEBASE


    cd $currentDir
    CopyRunwiseAndMergeAccordingToRunlistMC "LHC18f3_cent_woSDD_1" $HAVELHC18f31 $OUTPUTDIR_LHC18f31 $LHC18f3MC1 $pathMCR2 $baseLegoMC "/alice/sim/2018" $NSlashes3 runlistsToMerge.txt $FILENAMEBASE
    CopyRunwiseAndMergeAccordingToRunlistMC "LHC18f3_cent_woSDD_2" $HAVELHC18f32 $OUTPUTDIR_LHC18f32 $LHC18f3MC2 $pathMCR2 $baseLegoMC "/alice/sim/2018" $NSlashes3 runlistsToMerge.txt $FILENAMEBASE
    CopyRunwiseAndMergeAccordingToRunlistMC "LHC18f3_fast_1" $HAVELHC18f3F1 $OUTPUTDIR_LHC18f3F1 $LHC18f3MCFast1 $pathMCR2 $baseLegoMC "/alice/sim/2018" $NSlashes3 runlistsToMerge.txt $FILENAMEBASE
    CopyRunwiseAndMergeAccordingToRunlistMC "LHC18f3_fast_2" $HAVELHC18f3F2 $OUTPUTDIR_LHC18f3F2 $LHC18f3MCFast2 $pathMCR2 $baseLegoMC "/alice/sim/2018" $NSlashes3 runlistsToMerge.txt $FILENAMEBASE
    CopyRunwiseAndMergeAccordingToRunlistMC "LHC17f2a_fix" $HAVELHC17f2afix $OUTPUTDIR_LHC17f2a_fix $LHC17f2a_fixMC $pathMCR2 $baseLegoMC "/alice/sim/2017" $NSlashes3 runlistsToMerge.txt $FILENAMEBASE
    CopyRunwiseAndMergeAccordingToRunlistMC "LHC17f2a_fix_fast" $HAVELHC17f2afixF $OUTPUTDIR_LHC17f2a_fixF $LHC17f2a_fixMCFast $pathMCR2 $baseLegoMC "/alice/sim/2017" $NSlashes3 runlistsToMerge.txt $FILENAMEBASE
    CopyRunwiseAndMergeAccordingToRunlistJJMC "LHC17g8a" $HAVELHC17g8a $OUTPUTDIR_LHC17g8a $LHC17g8aMC $pathMCR2 $baseLegoMC "/alice/sim/2017" $NSlashes3 runlistsToMerge.txt $FILENAMEBASE
    CopyRunwiseAndMergeAccordingToRunlistJJMC "LHC17g8a_fast" $HAVELHC17g8aF $OUTPUTDIR_LHC17g8aF $LHC17g8aMCFast $pathMCR2 $baseLegoMC "/alice/sim/2017" $NSlashes3 runlistsToMerge.txt $FILENAMEBASE

    for runListName in $listsToMerge; do
        if [ $HAVELHC13b == 1 ]; then
            ls $OUTPUTDIR_LHC13b/$FILENAMEBASE-$runListName\_*.root > fileLHC13b.txt
            fileNumbers=`cat fileLHC13b.txt`
            for fileName in $fileNumbers; do
                echo $fileName
                ChangeStructureIfNeededGeneral $fileName $OUTPUTDIR_LHC13b $NSlashes "LHC13b-pass4-$runListName" "-$runListName"
            done;
        fi
        if [ $HAVELHC13c == 1 ]; then
            ls $OUTPUTDIR_LHC13c/$FILENAMEBASE-$runListName\_*.root > fileLHC13c.txt
            fileNumbers=`cat fileLHC13c.txt`
            for fileName in $fileNumbers; do
                echo $fileName
                ChangeStructureIfNeededGeneral $fileName $OUTPUTDIR_LHC13c $NSlashes "LHC13c-pass4-$runListName" "-$runListName"
            done;
        fi
        if [ $HAVELHC13d == 1 ]; then
            ls $OUTPUTDIR_LHC13d/$FILENAMEBASE-$runListName\_*.root > fileLHC13d.txt
            fileNumbers=`cat fileLHC13d.txt`
            for fileName in $fileNumbers; do
                echo $fileName
                ChangeStructureIfNeededGeneral $fileName $OUTPUTDIR_LHC13d $NSlashes "LHC13d-pass4-$runListName" "-$runListName"
            done;
        fi
        if [ $HAVELHC13e == 1 ]; then
            ls $OUTPUTDIR_LHC13e/$FILENAMEBASE-$runListName\_*.root > fileLHC13e.txt
            fileNumbers=`cat fileLHC13e.txt`
            for fileName in $fileNumbers; do
                echo $fileName
                ChangeStructureIfNeededGeneral $fileName $OUTPUTDIR_LHC13e $NSlashes "LHC13e-pass4-$runListName" "-$runListName"
            done;
        fi
        if [ $HAVELHC13f == 1 ]; then
            ls $OUTPUTDIR_LHC13f/$FILENAMEBASE-$runListName\_*.root > fileLHC13f.txt
            fileNumbers=`cat fileLHC13f.txt`
            for fileName in $fileNumbers; do
                echo $fileName
                ChangeStructureIfNeededGeneral $fileName $OUTPUTDIR_LHC13f $NSlashes "LHC13f-pass4-$runListName" "-$runListName"
            done;
        fi

        if [ $HAVELHC18j51 == 1 ]; then
            ls $OUTPUTDIR_LHC18j51/$FILENAMEBASE-$runListName\_*.root > fileLHC18j5.txt
            fileNumbers=`cat fileLHC18j5.txt`
            for fileName in $fileNumbers; do
                echo $fileName
                ChangeStructureIfNeededGeneral $fileName $OUTPUTDIR_LHC18j51 $NSlashes "MC_LHC18j5_1-$runListName" "-$runListName"
            done;
        fi
        if [ $HAVELHC18j52 == 1 ]; then
            ls $OUTPUTDIR_LHC18j52/$FILENAMEBASE-$runListName\_*.root > fileLHC18j5.txt
            fileNumbers=`cat fileLHC18j5.txt`
            for fileName in $fileNumbers; do
                echo $fileName
                ChangeStructureIfNeededGeneral $fileName $OUTPUTDIR_LHC18j52 $NSlashes "MC_LHC18j5_2-$runListName" "-$runListName"
            done;
        fi
        if [ $HAVELHC18j53 == 1 ]; then
            ls $OUTPUTDIR_LHC18j53/$FILENAMEBASE-$runListName\_*.root > fileLHC18j5.txt
            fileNumbers=`cat fileLHC18j5.txt`
            for fileName in $fileNumbers; do
                echo $fileName
                ChangeStructureIfNeededGeneral $fileName $OUTPUTDIR_LHC18j53 $NSlashes "MC_LHC18j5_3-$runListName" "-$runListName"
            done;
        fi

        if [ $HAVELHC16q == 1 ]; then
            ls $OUTPUTDIR_LHC16q/$FILENAMEBASE-$runListName\_*.root > fileLHC16q.txt
            fileNumbers=`cat fileLHC16q.txt`
            for fileName in $fileNumbers; do
                echo $fileName
                ChangeStructureIfNeededGeneral $fileName $OUTPUTDIR_LHC16q $NSlashes "LHC16q_woSDD-pass1-$runListName" "-$runListName"
            done;
        fi
        if [ $HAVELHC16qF == 1 ]; then
            ls $OUTPUTDIR_LHC16qF/$FILENAMEBASE-$runListName\_*.root > fileLHC16qF.txt
            fileNumbers=`cat fileLHC16qF.txt`
            for fileName in $fileNumbers; do
                echo $fileName
                ChangeStructureIfNeededGeneral $fileName $OUTPUTDIR_LHC16qF $NSlashes "LHC16q_fast-pass1-$runListName" "-$runListName"
            done;
        fi
        if [ $HAVELHC16t == 1 ]; then
            ls $OUTPUTDIR_LHC16t/$FILENAMEBASE-$runListName\_*.root > fileLHC16t.txt
            fileNumbers=`cat fileLHC16t.txt`
            for fileName in $fileNumbers; do
                echo $fileName
                ChangeStructureIfNeededGeneral $fileName $OUTPUTDIR_LHC16t $NSlashes "LHC16t_woSDD-pass1-$runListName" "-$runListName"
            done;
        fi
        if [ $HAVELHC16tF == 1 ]; then
            ls $OUTPUTDIR_LHC16tF/$FILENAMEBASE-$runListName\_*.root > fileLHC16tF.txt
            fileNumbers=`cat fileLHC16tF.txt`
            for fileName in $fileNumbers; do
                echo $fileName
                ChangeStructureIfNeededGeneral $fileName $OUTPUTDIR_LHC16tF $NSlashes "LHC16t_fast-pass1-$runListName" "-$runListName"
            done;
        fi

        if [ $HAVELHC18f31 == 1 ]; then
            ls $OUTPUTDIR_LHC18f31/$FILENAMEBASE-$runListName\_*.root > fileLHC18f3.txt
            fileNumbers=`cat fileLHC18f3.txt`
            for fileName in $fileNumbers; do
                echo $fileName
                ChangeStructureIfNeededGeneral $fileName $OUTPUTDIR_LHC18f31 $NSlashes "MC_LHC18f3_1-$runListName" "-$runListName"
            done;
        fi
        if [ $HAVELHC18f32 == 1 ]; then
            ls $OUTPUTDIR_LHC18f32/$FILENAMEBASE-$runListName\_*.root > fileLHC18f3.txt
            fileNumbers=`cat fileLHC18f3.txt`
            for fileName in $fileNumbers; do
                echo $fileName
                ChangeStructureIfNeededGeneral $fileName $OUTPUTDIR_LHC18f32 $NSlashes "MC_LHC18f3_2-$runListName" "-$runListName"
            done;
        fi
        if [ $HAVELHC18f3F1 == 1 ]; then
            ls $OUTPUTDIR_LHC18f3F1/$FILENAMEBASE-$runListName\_*.root > fileLHC18f3F1.txt
            fileNumbers=`cat fileLHC18f3F1.txt`
            for fileName in $fileNumbers; do
                echo $fileName
                ChangeStructureIfNeededGeneral $fileName $OUTPUTDIR_LHC18f3F1 $NSlashes "MC_LHC18f3_fast_1-$runListName" "-$runListName"
            done;
        fi
        if [ $HAVELHC18f3F2 == 1 ]; then
            ls $OUTPUTDIR_LHC18f3F2/$FILENAMEBASE-$runListName\_*.root > fileLHC18f3F2.txt
            fileNumbers=`cat fileLHC18f3F2.txt`
            for fileName in $fileNumbers; do
                echo $fileName
                ChangeStructureIfNeededGeneral $fileName $OUTPUTDIR_LHC18f3F2 $NSlashes "MC_LHC18f3_fast_2-$runListName" "-$runListName"
            done;
        fi
        if [ $HAVELHC17f2afix == 1 ]; then
            ls $OUTPUTDIR_LHC17f2a_fix/$FILENAMEBASE-$runListName\_*.root > fileLHC17f2a_fix.txt
            fileNumbers=`cat fileLHC17f2a_fix.txt`
            for fileName in $fileNumbers; do
                echo $fileName
                ChangeStructureIfNeededGeneral $fileName $OUTPUTDIR_LHC17f2a_fix $NSlashes "MC_LHC17f2a_fix-$runListName" "-$runListName"
            done;
        fi
        if [ $HAVELHC17f2afixF == 1 ]; then
            ls $OUTPUTDIR_LHC17f2a_fixF/$FILENAMEBASE-$runListName\_*.root > fileLHC17f2a_fixF.txt
            fileNumbers=`cat fileLHC17f2a_fixF.txt`
            for fileName in $fileNumbers; do
                echo $fileName
                ChangeStructureIfNeededGeneral $fileName $OUTPUTDIR_LHC17f2a_fixF $NSlashes "MC_LHC17f2a_fix_fast-$runListName" "-$runListName"
            done;
        fi
        if [ $HAVELHC19a41 == 1 ]; then
            ls $OUTPUTDIR_LHC19a41/$FILENAMEBASE-$runListName\_*.root > fileLHC19a41.txt
            fileNumbers=`cat fileLHC19a41.txt`
            for fileName in $fileNumbers; do
                echo $fileName
                ChangeStructureIfNeededGeneral $fileName $OUTPUTDIR_LHC19a41 $NSlashes "MC_LHC19a4_1-$runListName" "-$runListName"
            done;
        fi
        if [ $HAVELHC19a42 == 1 ]; then
            ls $OUTPUTDIR_LHC19a42/$FILENAMEBASE-$runListName\_*.root > fileLHC19a42.txt
            fileNumbers=`cat fileLHC19a42.txt`
            for fileName in $fileNumbers; do
                echo $fileName
                ChangeStructureIfNeededGeneral $fileName $OUTPUTDIR_LHC19a42 $NSlashes "MC_LHC19a4_2-$runListName" "-$runListName"
            done;
        fi
#         if [ $HAVELHC16c3a1 == 1 ]; then
#             ls $OUTPUTDIR_LHC16c3a1/$FILENAMEBASE-$runListName\_*.root > fileLHC16c3a1.txt
#             fileNumbers=`cat fileLHC16c3a1.txt`
#             for fileName in $fileNumbers; do
#                 echo $fileName
#                 ChangeStructureIfNeededGeneral $fileName $OUTPUTDIR_LHC16c3a1 $NSlashes "MC_LHC16c3a_1-$runListName" "-$runListName"
#             done;
#         fi
#         if [ $HAVELHC16c3a2 == 1 ]; then
#             ls $OUTPUTDIR_LHC16c3a2/$FILENAMEBASE-$runListName\_*.root > fileLHC16c3a2.txt
#             fileNumbers=`cat fileLHC16c3a2.txt`
#             for fileName in $fileNumbers; do
#                 echo $fileName
#                 ChangeStructureIfNeededGeneral $fileName $OUTPUTDIR_LHC16c3a2 $NSlashes "MC_LHC16c3a_2-$runListName" "-$runListName"
#             done;
#         fi
#         if [ $HAVELHC16c3b1 == 1 ]; then
#             ls $OUTPUTDIR_LHC16c3b1/$FILENAMEBASE-$runListName\_*.root > fileLHC16c3b1.txt
#             fileNumbers=`cat fileLHC16c3b1.txt`
#             for fileName in $fileNumbers; do
#                 echo $fileName
#                 ChangeStructureIfNeededGeneral $fileName $OUTPUTDIR_LHC16c3b1 $NSlashes "MC_LHC16c3b_1-$runListName" "-$runListName"
#             done;
#         fi
#         if [ $HAVELHC16c3b2 == 1 ]; then
#             ls $OUTPUTDIR_LHC16c3b2/$FILENAMEBASE-$runListName\_*.root > fileLHC16c3b2.txt
#             fileNumbers=`cat fileLHC16c3b2.txt`
#             for fileName in $fileNumbers; do
#                 echo $fileName
#                 ChangeStructureIfNeededGeneral $fileName $OUTPUTDIR_LHC16c3b2 $NSlashes "MC_LHC16c3b_2-$runListName" "-$runListName"
#             done;
#         fi
        if [ $HAVELHC17g8a == 1 ]; then
            ls $OUTPUTDIR_LHC17g8a/$FILENAMEBASE-$runListName\_*.root > fileLHC17g8a.txt
            fileNumbers=`cat fileLHC17g8a.txt`
            for fileName in $fileNumbers; do
                echo $fileName
                ChangeStructureIfNeededGeneral $fileName $OUTPUTDIR_LHC17g8a $NSlashes "MC_LHC17g8a-$runListName" "-$runListName"
            done;
#             for binNumber in $binNumbersJJ; do
#                 echo $binNumber
#                 ls $OUTPUTDIR_LHC17g8a/$FILENAMEBASE-$runListName\_*.root > fileLHC17g8a.txt
#                 fileNumbers=`cat fileLHC17g8a.txt`
#                 for fileName in $fileNumbers; do
#                     echo $fileName
#                     GetFileNumberMerging $fileName $((NSlashes)) 2
#                     cp $OUTPUTDIR_LHC17g8a/$binNumber/$FILENAMEBASE-$runListName\_$number.root  $OUTPUTDIR/JJMCSingleBins/$FILENAMEBASE\_MC_LHC17g8a-$binNumber\_$3-$runListName\_$number.root
#                 done
#             done;
        fi
        if [ $HAVELHC17g8aF == 1 ]; then
            ls $OUTPUTDIR_LHC17g8aF/$FILENAMEBASE-$runListName\_*.root > fileLHC17g8a.txt
            fileNumbers=`cat fileLHC17g8a.txt`
            for fileName in $fileNumbers; do
                echo $fileName
                ChangeStructureIfNeededGeneral $fileName $OUTPUTDIR_LHC17g8aF $NSlashes "MC_LHC17g8a_fast-$runListName" "-$runListName"
            done;
#             for binNumber in $binNumbersJJ; do
#                 echo $binNumber
#                 ls $OUTPUTDIR_LHC17g8aF/$FILENAMEBASE-$runListName\_*.root > fileLHC17g8a.txt
#                 fileNumbers=`cat fileLHC17g8a.txt`
#                 for fileName in $fileNumbers; do
#                     echo $fileName
#                     GetFileNumberMerging $fileName $((NSlashes)) 2
#                     cp $OUTPUTDIR_LHC17g8aF/$binNumber/$FILENAMEBASE-$runListName\_$number.root  $OUTPUTDIR/JJMCSingleBins/$FILENAMEBASE\_MC_LHC17g8a_fast-$binNumber\_$3-$runListName\_$number.root
#                 done
#             done;
        fi
    done

    if [ $MERGEON == 1 ]; then
        echo "Starting Merging"

        #######################################################################################################################
        ###### Start merging of the data 
        #######################################################################################################################
        if [ $MERGEONData == 1 ]; then
            for runListName in $listsToMerge; do
                ls $OUTPUTDIR/$FILENAMEBASE\_LHC13f-pass4-$runListName\_*.root > filesForMerging.txt
                filesForMerging=`cat filesForMerging.txt`
                #13er daten
                periodList=`echo -e "b-pass4\nc-pass4\nd-pass4\ne-pass4\nf-pass4"`
                for fileName in $filesForMerging; do
                    echo $fileName
                    GetFileNumberMerging $fileName $((NSlashes-1)) 3 "bla" 1
                    echo $number
                    nameOut=""
                    rm listCurrMerge.txt
                    echo $fileName
                    for periodID in $periodList; do
                        echo $periodID
                        currFile=$OUTPUTDIR/$FILENAMEBASE\_LHC13$periodID-$runListName\_$number.root
                        if [ -f $currFile ]; then
                            outAdd=`echo $periodID  | cut -d "-" -f 1 `
                            nameOut+=$outAdd
                            echo -e "$currFile\n" >> listCurrMerge.txt
                        else
                            echo $currFile " does not exist"
                        fi
                    done
                    MergeAccordingToList listCurrMerge.txt $OUTPUTDIR/$FILENAMEBASE\_LHC13$nameOut-pass4-$runListName\_$number.root
                    for periodID in $periodList; do
                        mv $OUTPUTDIR/$FILENAMEBASE\_LHC13$periodID-$runListName\_$number.root $OUTPUTDIR/SinglePeriods/
                    done
                done
            done
    #             ls $OUTPUTDIR/$FILENAMEBASE\_LHC13e-pass4-$runListName\_*.root > filesForMerging.txt
    #             periodList=`echo -e "b-pass4\nc-pass4\nd-pass4\ne-pass4"`
    #             for fileName in $filesForMerging; do
    #                 echo $fileName
    #                 GetFileNumberMerging $fileName $((NSlashes-1)) 3 "bla" 1
    #                 echo $number
    #                 nameOut=""
    #                 rm listCurrMerge.txt
    #                 echo $fileName
    #                 for periodID in $periodList; do
    #                     echo $periodID
    #                     currFile=$OUTPUTDIR/$FILENAMEBASE\_LHC13$periodID-$runListName\_$number.root
    #                     if [ -f $currFile ]; then
    #                         outAdd=`echo $periodID  | cut -d "-" -f 1 `
    #                         nameOut+=$outAdd
    #                         echo -e "$currFile\n" >> listCurrMerge.txt
    #                     else
    #                         echo $currFile " does not exist"
    #                     fi
    #                 done
    #                 MergeAccordingToList listCurrMerge.txt $OUTPUTDIR/$FILENAMEBASE\_LHC13$nameOut-pass4-$runListName\_$number.root
    #             done
            echo -e "_woSDD\n_fast" > listReconstruction.txt
            listReconstruction=`cat listReconstruction.txt`
            periodList=`echo -e "q\nt"`
            for reco in $listReconstruction; do
                ls $OUTPUTDIR/$FILENAMEBASE\_LHC16q$reco-pass$passNr-DPGTrackIncAccAndEMC\_*.root > filesForMerging.txt
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
                            currFile=$OUTPUTDIR/$FILENAMEBASE\_LHC16$periodID$reco-pass$passNr-$runListName\_$number.root
                            if [ -f $currFile ]; then
                                outAdd=`echo $periodID  | cut -d "-" -f 1 `
                                nameOut+=$outAdd
                                echo -e "$currFile\n" >> listCurrMerge.txt
                            else
                                echo $currFile " does not exist"
                            fi
                        done
                        MergeAccordingToList listCurrMerge.txt $OUTPUTDIR/$FILENAMEBASE\_LHC16$nameOut$reco-pass$passNr-$runListName\_$number.root
                        for periodID in $periodList; do
                            mv $OUTPUTDIR/$FILENAMEBASE\_LHC16$periodID$reco-pass$passNr-$runListName\_$number.root $OUTPUTDIR/SinglePeriods/
                        done  
                    done
                done
            done
        
            ls $OUTPUTDIR/$FILENAMEBASE\_LHC16qt_fast-pass$passNr-DPGTrackIncAccAndEMC\_*.root | grep -v "WTree" > filesForMerging.txt
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
                        currFile=$OUTPUTDIR/$FILENAMEBASE\_LHC16qt$periodID-pass$passNr-$runListName\_$number.root
                        if [ -f $currFile ]; then
                            outAdd=`echo $periodID  | cut -d "-" -f 1 `
                            nameOut+=$outAdd
                            echo -e "$currFile\n" >> listCurrMerge.txt
                        else
                            echo $currFile " does not exist"
                        fi
                    done
                    MergeAccordingToList listCurrMerge.txt $OUTPUTDIR/$FILENAMEBASE\_LHC16qt$nameOut-pass$passNr-$runListName\_$number.root
                    for periodID in $periodList; do
                        mv $OUTPUTDIR/$FILENAMEBASE\_LHC16qt$periodID-pass$passNr-$runListName\_$number.root $OUTPUTDIR/SinglePeriods/
                    done  
                done
            done
            ls $OUTPUTDIR/$FILENAMEBASE\_LHC16qt_fast_woSDD-pass$passNr-DPGTrackIncAccAndEMC\_*.root | grep -v "WTree" > filesForMerging.txt
            filesForMerging=`cat filesForMerging.txt`
            periodList=`echo -e "LHC13bcdef-pass4\nLHC16qt_fast_woSDD-pass$passNr"`
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
                ls $OUTPUTDIR/$FILENAMEBASE\_MC_LHC18j5_1-$runListName\_*.root > filesForMerging.txt
                filesForMerging=`cat filesForMerging.txt`
                periodList=`echo -e "j5_1\nj5_2\nj5_3"`
                for fileName in $filesForMerging; do
                    echo $fileName
                    GetFileNumberMerging $fileName $((NSlashes-1)) 5 "bla" 1
                    echo $number
                    nameOut=""
                    rm listCurrMerge.txt
                    echo $fileName
                    for periodID in $periodList; do
                        echo $periodID
                        currFile=$OUTPUTDIR/$FILENAMEBASE\_MC_LHC18$periodID-$runListName\_$number.root
                        if [ -f $currFile ]; then
                            outAdd=`echo $periodID  | cut -d "-" -f 1 `
                            nameOut+=$outAdd
                            echo -e "$currFile\n" >> listCurrMerge.txt
                        else
                            echo $currFile " does not exist"
                        fi
                    done
                    MergeAccordingToList listCurrMerge.txt $OUTPUTDIR/$FILENAMEBASE\_MC_LHC18j5x-$runListName\_$number.root
                    for periodID in $periodList; do
                        mv $OUTPUTDIR/$FILENAMEBASE\_MC_LHC18$periodID-$runListName\_$number.root $OUTPUTDIR/SinglePeriods/
                    done
                done
            done
            ls $OUTPUTDIR/$FILENAMEBASE\_MC_LHC17f2a_fix_fast-DPGTrackIncAccAndEMC\_*.root | grep -v "WTree" > filesForMerging.txt
            filesForMerging=`cat filesForMerging.txt`
            periodList=`echo -e "_fast\n_woSDD"`
            for fileName in $filesForMerging; do
                echo $fileName
                GetFileNumberMerging $fileName $((NSlashes-1)) 5
                echo $number
                for runListName in $listsToMerge; do
                    rm listCurrMerge.txt
                    nameOut=""
                    for periodID in $periodList; do
                        echo $periodID
                        currFile=$OUTPUTDIR/$FILENAMEBASE\_MC_LHC17f2a_fix$periodID-$runListName\_$number.root
                        if [ -f $currFile ]; then
                            outAdd=`echo $periodID  | cut -d "-" -f 1 `
                            nameOut+=$outAdd
                            echo -e "$currFile\n" >> listCurrMerge.txt
                        else
                            echo $currFile " does not exist"
                        fi
                    done
                    if [ $nameOut = "_fast_woSDD" ]; then
                        nameOut="";
                    fi
                    MergeAccordingToList listCurrMerge.txt $OUTPUTDIR/$FILENAMEBASE\_MC_LHC17f2a_fix$nameOut-$runListName\_$number.root
                    for periodID in $periodList; do
                        mv $OUTPUTDIR/$FILENAMEBASE\_MC_LHC17f2a_fix$periodID-$runListName\_$number.root $OUTPUTDIR/SinglePeriods/
                    done  
                done
            done

            ls $OUTPUTDIR/$FILENAMEBASE\_MC_LHC18f3_fast_1-DPGTrackIncAccAndEMC\_*.root | grep -v "WTree" > filesForMerging.txt
            filesForMerging=`cat filesForMerging.txt`
            periodList=`echo -e "_fast_1\n_fast_2\n_1\n_2"`
            for fileName in $filesForMerging; do
                echo $fileName
                GetFileNumberMerging $fileName $((NSlashes-1)) 6
                echo $number
                for runListName in $listsToMerge; do
                    rm listCurrMerge.txt
                    nameOut=""
                    for periodID in $periodList; do
                        echo $periodID
                        currFile=$OUTPUTDIR/$FILENAMEBASE\_MC_LHC18f3$periodID-$runListName\_$number.root
                        if [ -f $currFile ]; then
                            outAdd=`echo $periodID  | cut -d "-" -f 1 `
                            nameOut+=$outAdd
                            echo -e "$currFile\n" >> listCurrMerge.txt
                        else
                            echo $currFile " does not exist"
                        fi
                    done
                    if [ $nameOut = "_fast_1_fast_2_1_2" ]; then
                        nameOut="x";
                    fi
                    MergeAccordingToList listCurrMerge.txt $OUTPUTDIR/$FILENAMEBASE\_MC_LHC18f3$nameOut-$runListName\_$number.root
                    for periodID in $periodList; do
                        mv $OUTPUTDIR/$FILENAMEBASE\_MC_LHC18f3$periodID-$runListName\_$number.root $OUTPUTDIR/SinglePeriods/
                    done  
                done
            done


            ls $OUTPUTDIR/$FILENAMEBASE\_MC_LHC18f3x-DPGTrackIncAccAndEMC\_*.root | grep -v "WTree" > filesForMerging.txt
            filesForMerging=`cat filesForMerging.txt`
            periodList=`echo -e "LHC18j5x\nLHC18f3x"`
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

            ls $OUTPUTDIR/$FILENAMEBASE\_MC_LHC18f3x-DPGTrackIncAccAndEMC\_*.root | grep -v "WTree" > filesForMerging.txt
            filesForMerging=`cat filesForMerging.txt`
            periodList=`echo -e "LHC17f2a_fix\nLHC18f3x"`
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

            ls $OUTPUTDIR/$FILENAMEBASE\_MC_LHC17g8a_fast-DPGTrackIncAccAndEMC\_*.root > filesForMerging.txt
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
                        currFile=$OUTPUTDIR/$FILENAMEBASE\_MC_LHC17g8a$periodID-$runListName\_$number.root
                        if [ -f $currFile ]; then
                            outAdd=`echo $periodID  | cut -d "-" -f 1 `
                            nameOut+=$outAdd
                            echo -e "$currFile\n" >> listCurrMerge.txt
                        else
                            echo $currFile " does not exist"
                        fi
                    done
                    if [ $nameOut = "_fast_woSDD" ]; then
                        nameOut="x";
                    fi
                    MergeAccordingToList listCurrMerge.txt $OUTPUTDIR/$FILENAMEBASE\_MC_LHC17g8a$nameOut-$runListName\_$number.root
                    for periodID in $periodList; do
                        mv $OUTPUTDIR/$FILENAMEBASE\_MC_LHC17g8a$periodID-$runListName\_$number.root $OUTPUTDIR/SinglePeriods/
                    done  
    #                 for binNumber in $binNumbersJJ; do
    #                     echo $binNumber
    #                     rm listCurrMerge.txt
    #                     fileF="$OUTPUTDIR/JJMCSingleBins/$FILENAMEBASE\_MC_LHC17g8a-$binNumber""_fast-$runListName""_$number.root"
    #                     fileW="$OUTPUTDIR/JJMCSingleBins/$FILENAMEBASE\_MC_LHC17g8a-$binNumber""_woSDD-$runListName""_$number.root"
    #                     echo -e "$fileF\n$fileW" > listCurrMerge.txt
    #                     MergeAccordingToList listCurrMerge.txt $OUTPUTDIR/JJMCSingleBins/$FILENAMEBASE\_MC_LHC17g8a-$binNumber\_fast-woSDD-$runListName\_$number.root
    #                 done

                done
            done
            
            ls $OUTPUTDIR/$FILENAMEBASE\_MC_LHC19a4_1-DPGTrackIncAccAndEMC\_*.root > filesForMerging.txt
            filesForMerging=`cat filesForMerging.txt`
            periodList=`echo -e "_1\n_2"`
            for fileName in $filesForMerging; do
                echo $fileName
                GetFileNumberMerging $fileName $((NSlashes-1)) 5
                echo $number
                for runListName in $listsToMerge; do
                    rm listCurrMerge.txt
                    nameOut=""
                    for periodID in $periodList; do
                        echo $periodID
                        currFile=$OUTPUTDIR/$FILENAMEBASE\_MC_LHC19a4$periodID-$runListName\_$number.root
                        if [ -f $currFile ]; then
                            outAdd=`echo $periodID  | cut -d "-" -f 1 `
                            nameOut+=$outAdd
                            echo -e "$currFile\n" >> listCurrMerge.txt
                        else
                            echo $currFile " does not exist"
                        fi
                    done
                    if [ $nameOut = "_1_2" ]; then
                        nameOut="x";
                    fi
                    MergeAccordingToList listCurrMerge.txt $OUTPUTDIR/$FILENAMEBASE\_MC_LHC19a4$nameOut-$runListName\_$number.root
                    for periodID in $periodList; do
                        mv $OUTPUTDIR/$FILENAMEBASE\_MC_LHC19a4$periodID-$runListName\_$number.root $OUTPUTDIR/SinglePeriods/
                    done  
    #                 for binNumber in $binNumbersJJ; do
    #                     echo $binNumber
    #                     rm listCurrMerge.txt
    #                     fileF="$OUTPUTDIR/JJMCSingleBins/$FILENAMEBASE\_MC_LHC17g8a-$binNumber""_fast-$runListName""_$number.root"
    #                     fileW="$OUTPUTDIR/JJMCSingleBins/$FILENAMEBASE\_MC_LHC17g8a-$binNumber""_woSDD-$runListName""_$number.root"
    #                     echo -e "$fileF\n$fileW" > listCurrMerge.txt
    #                     MergeAccordingToList listCurrMerge.txt $OUTPUTDIR/JJMCSingleBins/$FILENAMEBASE\_MC_LHC17g8a-$binNumber\_fast-woSDD-$runListName\_$number.root
    #                 done

                done
            done

            ls $OUTPUTDIR/$FILENAMEBASE\_MC_LHC16c3a_1-DPGTrackIncAccAndEMC\_*.root > filesForMerging.txt
            filesForMerging=`cat filesForMerging.txt`
            periodList=`echo -e "_1\n_2"`
            for fileName in $filesForMerging; do
                echo $fileName
                GetFileNumberMerging $fileName $((NSlashes-1)) 5
                echo $number
                for runListName in $listsToMerge; do
                    rm listCurrMerge.txt
                    nameOut=""
                    for periodID in $periodList; do
                        echo $periodID
                        currFile=$OUTPUTDIR/$FILENAMEBASE\_MC_LHC16c3a$periodID-$runListName\_$number.root
                        if [ -f $currFile ]; then
                            outAdd=`echo $periodID  | cut -d "-" -f 1 `
                            nameOut+=$outAdd
                            echo -e "$currFile\n" >> listCurrMerge.txt
                        else
                            echo $currFile " does not exist"
                        fi
                    done
                    if [ $nameOut = "_1_2" ]; then
                        nameOut="x";
                    fi
                    MergeAccordingToList listCurrMerge.txt $OUTPUTDIR/$FILENAMEBASE\_MC_LHC16c3a$nameOut-$runListName\_$number.root
                    for periodID in $periodList; do
                        mv $OUTPUTDIR/$FILENAMEBASE\_MC_LHC16c3a$periodID-$runListName\_$number.root $OUTPUTDIR/SinglePeriods/
                    done  
    #                 for binNumber in $binNumbersJJ; do
    #                     echo $binNumber
    #                     rm listCurrMerge.txt
    #                     fileF="$OUTPUTDIR/JJMCSingleBins/$FILENAMEBASE\_MC_LHC17g8a-$binNumber""_fast-$runListName""_$number.root"
    #                     fileW="$OUTPUTDIR/JJMCSingleBins/$FILENAMEBASE\_MC_LHC17g8a-$binNumber""_woSDD-$runListName""_$number.root"
    #                     echo -e "$fileF\n$fileW" > listCurrMerge.txt
    #                     MergeAccordingToList listCurrMerge.txt $OUTPUTDIR/JJMCSingleBins/$FILENAMEBASE\_MC_LHC17g8a-$binNumber\_fast-woSDD-$runListName\_$number.root
    #                 done

                done
            done

            ls $OUTPUTDIR/$FILENAMEBASE\_MC_LHC16c3b_1-DPGTrackIncAccAndEMC\_*.root > filesForMerging.txt
            filesForMerging=`cat filesForMerging.txt`
            periodList=`echo -e "_1\n_2"`
            for fileName in $filesForMerging; do
                echo $fileName
                GetFileNumberMerging $fileName $((NSlashes-1)) 5
                echo $number
                for runListName in $listsToMerge; do
                    rm listCurrMerge.txt
                    nameOut=""
                    for periodID in $periodList; do
                        echo $periodID
                        currFile=$OUTPUTDIR/$FILENAMEBASE\_MC_LHC16c3b$periodID-$runListName\_$number.root
                        if [ -f $currFile ]; then
                            outAdd=`echo $periodID  | cut -d "-" -f 1 `
                            nameOut+=$outAdd
                            echo -e "$currFile\n" >> listCurrMerge.txt
                        else
                            echo $currFile " does not exist"
                        fi
                    done
                    if [ $nameOut = "_1_2" ]; then
                        nameOut="x";
                    fi
                    MergeAccordingToList listCurrMerge.txt $OUTPUTDIR/$FILENAMEBASE\_MC_LHC16c3b$nameOut-$runListName\_$number.root
                    for periodID in $periodList; do
                        mv $OUTPUTDIR/$FILENAMEBASE\_MC_LHC16c3b$periodID-$runListName\_$number.root $OUTPUTDIR/SinglePeriods/
                    done  
    #                 for binNumber in $binNumbersJJ; do
    #                     echo $binNumber
    #                     rm listCurrMerge.txt
    #                     fileF="$OUTPUTDIR/JJMCSingleBins/$FILENAMEBASE\_MC_LHC17g8a-$binNumber""_fast-$runListName""_$number.root"
    #                     fileW="$OUTPUTDIR/JJMCSingleBins/$FILENAMEBASE\_MC_LHC17g8a-$binNumber""_woSDD-$runListName""_$number.root"
    #                     echo -e "$fileF\n$fileW" > listCurrMerge.txt
    #                     MergeAccordingToList listCurrMerge.txt $OUTPUTDIR/JJMCSingleBins/$FILENAMEBASE\_MC_LHC17g8a-$binNumber\_fast-woSDD-$runListName\_$number.root
    #                 done

                done
            done
            
        fi
    fi

else
    if [ $HAVELHC16q == 1 ]; then
        echo "removing all $FILENAMEBASE files in runFolders for LHC16q";
#         rm $OUTPUTDIR_LHC16q/*/$FILENAMEBASE\_*.root
        rm $OUTPUTDIR_LHC16q/*/*/*$FILENAMEBASE\_*.root
    fi
    if [ $HAVELHC16t == 1 ]; then
        echo "removing all $FILENAMEBASE files in runFolders for LHC16t";
#         rm $OUTPUTDIR_LHC16t/*/$FILENAMEBASE\_*.root
        rm $OUTPUTDIR_LHC16t/*/*/*$FILENAMEBASE\_*.root
    fi
    if [ $HAVELHC16qF == 1 ]; then
        echo "removing all $FILENAMEBASE files in runFolders for LHC16q";
#         rm $OUTPUTDIR_LHC16q/*/$FILENAMEBASE\_*.root
        rm $OUTPUTDIR_LHC16qF/*/*/*$FILENAMEBASE\_*.root
    fi
    if [ $HAVELHC16tF == 1 ]; then
        echo "removing all $FILENAMEBASE files in runFolders for LHC16t";
#         rm $OUTPUTDIR_LHC16t/*/$FILENAMEBASE\_*.root
        rm $OUTPUTDIR_LHC16tF/*/*/*$FILENAMEBASE\_*.root
    fi

    if [ $HAVELHC18f3 == 1 ]; then
        echo "removing all $FILENAMEBASE files in runFolders for LHC18f3";
#         rm $OUTPUTDIR_LHC18f3/*/$FILENAMEBASE\_*.root
        rm -rf $OUTPUTDIR_LHC18f3/*/Stage*
    fi
    if [ $HAVELHC17f2afix == 1 ]; then
        echo "removing all $FILENAMEBASE files in runFolders for LHC17f2a_fix";
#         rm $OUTPUTDIR_LHC17f2a_fix/*/$FILENAMEBASE\_*.root
        rm -rf $OUTPUTDIR_LHC17f2a_fix/*/Stage*
    fi
    if [ $HAVELHC18f3F1 == 1 ]; then
        echo "removing all $FILENAMEBASE files in runFolders for LHC18f3";
#         rm $OUTPUTDIR_LHC18f3/*/$FILENAMEBASE\_*.root
        rm -rf $OUTPUTDIR_LHC18f3F1/*/Stage*
    fi
    if [ $HAVELHC18f3F2 == 1 ]; then
        echo "removing all $FILENAMEBASE files in runFolders for LHC18f3";
#         rm $OUTPUTDIR_LHC18f3/*/$FILENAMEBASE\_*.root
        rm -rf $OUTPUTDIR_LHC18f3F2/*/Stage*
    fi
    if [ $HAVELHC17f2afixF == 1 ]; then
        echo "removing all $FILENAMEBASE files in runFolders for LHC17f2a_fix";
#         rm $OUTPUTDIR_LHC17f2a_fix/*/$FILENAMEBASE\_*.root
        rm -rf $OUTPUTDIR_LHC17f2a_fixF/*/Stage*
    fi
    if [ $HAVELHC17g8a == 1 ]; then
        echo "removing all $FILENAMEBASE files in runFolders for LHC17g8a";
#         rm $OUTPUTDIR_LHC17g8a/*/$FILENAMEBASE\_*.root
        rm -rf $OUTPUTDIR_LHC17g8a/*/*/Stage*
    fi
    if [ $HAVELHC17g8aF == 1 ]; then
        echo "removing all $FILENAMEBASE files in runFolders for LHC17g8a";
#         rm $OUTPUTDIR_LHC17g8a/*/$FILENAMEBASE\_*.root
        rm -rf $OUTPUTDIR_LHC17g8aF/*/*/Stage*
    fi
fi
