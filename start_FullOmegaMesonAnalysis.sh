#! /bin/bash
#
#
#
# This script gests as input a directory where the GammaConvV1 root file is stored,
# it also needs the desired output directory where the produced root files are put.
# If nothing is given it will use ./ for the input directory and ./Output for the output
#
#Input 1: Root file to analyze Default: AnalyisResults
#Input 2: Input directory  Default:$PWD
#Input 3: Output directory Default: $PWD/Results  (directory will be created if it does not exist)
PROGNAME=$0
AdvMesonQA=""
ONLYCORRECTION=0
ONLYETA=0
ONLYOMEGA=0
ONLYRESULTS=0
ONLYCUTS=0
Suffix=eps
PARTLY=0
DATAFILE=1
MCFILE=1
addedSig=0
ContaminationBackHistMode=0

NAMECUTSTUDIES="none"
PERIOD=""
NORMALCUTS=0
dataFileOK=0
altAccFileGiven=0

function GiveBinning7TeVOmega()
{
    echo "How many p_T bins do you want to use for Omega? 36(7gev), 37(8gev), 38(10gev), 39(12gev), 40 (16gev), 41 (20gev), 42 (25gev)";
    read answer
    BinsPtOmega=$answer
    correctOmega=1
    echo "You have chosen " $answer " pt bins for Omega";

}

function GiveBinning7TeVEta()
{
    echo "How many p_T bins do you want to use for Eta? 36(7gev), 37(8gev), 38(10gev), 39(12gev), 40 (16gev), 41 (20gev), 42 (25gev)";
    read answer
    BinsPtEta=$answer
    correctOmega=1
    echo "You have chosen " $answer " pt bins for Eta";

}

function GiveBinning13TeV()
{
    echo "How many p_T bins do you want to use for Omega? 36(7gev), 37(8gev), 38(10gev), 39(12gev), 40 (16gev), 41 (20gev), 42 (25gev)";
    read answer
    BinsPtOmega=$answer
    correctOmega=1
    echo "You have chosen " $answer " pt bins for Omega";

}

function GiveBinning5TeV()
{
    echo "How many p_T bins do you want to use for Omega? 36(7gev), 37(8gev), 38(10gev), 39(12gev), 40 (16gev), 41 (20gev), 42 (25gev)";
    read answer
    BinsPtOmega=$answer
    correctOmega=1
    echo "You have chosen " $answer " pt bins for Omega";

}

function GiveBinningpPb()
{
    echo "How many p_T bins do you want to use for Omega?  27 (7 GeV), 28 (8 GeV), 29 (10 GeV), 30 (12 GeV), 31 (14 GeV), for Calo measurements also 32 (16 GeV), 33 (18 GeV), 34 (20 GeV), 35 (22 GeV), 36 (26 GeV), 37 (30 GeV)";
    echo "for calo triggers bins reach to 46."
    read answer
    BinsPtOmega=$answer
    correctOmega=1
    echo "You have chosen " $answer " pt bins for Omega";

}


function ExtractSignal()
{
    root -l -b -q -x TaskV1/ExtractSignalPiPlPiMiNDM.C+\($1\)
    #root -x -q -l -b  TaskV1/ExtractSignalV2.C\+\($1\,$mode\,0\)
}

function CorrectSignal()
{
    root -x -l -b -q TaskV1/CorrectSignalPiPlPiMiPiZero.C+\($1\)
}


function CreateFinalResults()
{
    if [ $mode != 2 ] && [ $mode != 4 ]; then
        root -l -b -x -q TaskV1/ProduceFinalResultsPiPlPiMiPiZero.C+\($1\)
    fi
}

function Usage()
{
    echo -e "
This Script is provided by the Gamma Conversion Group of ALICE
the main developers are
    Friederike Bock \t friederike.bock@cern.ch
    Ana Marin \t\t marin@physi.uni-heidelberg.de
    Florian Jonas \t florian.jonas@cern.ch

If there are any complications with running this script do not hesitate to connect them.


How to use this script?
$PROGNAME -h  \t\t\t\t\t usage will be displayed
$PROGNAME --help \t\t\t\t usage will be displayed
$PROGNAME -c data.root suffix \t\t\t Will only correct and produce the final results for already existing RAWdata \n \t\t\t\t\t\t\t\t\t and produce the graphical output in the specified suffix-format.\n
$PROGNAME -d data.root suffix \t\t\t Will only fullfil cutstudies and produce the final results for already existing RAWdata \n \t\t\t\t\t\t\t\t\t and produce the graphical output in the specified suffix-format.\n
$PROGNAME -m DirectoryOfMergedOutputs suffix \t Automatic merging for LHC10bc and LHC10de for efficiencies will be done \n \t\t\t\t\t\t\t\t\t according to the fraction in Data, be careful files have to be arranged in a certain structure \n
    \t\t\t\t\t\t\t\t DataRootFile=DirectoryOfMergedOutputs/mergedALL/GammaConvV1Data.root
    \t\t\t\t\t\t\t\t MCRootFile=DirectoryOfMergedOutputs/mergedALL/GammaConvV1MC.root
    \t\t\t\t\t\t\t\t MCRootFileBC=DirectoryOfMergedOutputs/mergedBC/GammaConvV1MC.root
    \t\t\t\t\t\t\t\t MCRootFileD=DirectoryOfMergedOutputs/mergedDE/GammaConvV1MC.root\n
$PROGNAME -r data.root suffix \t\t\t Will only execute the production of the final results and \n \t\t\t\t\t\t\t\t\t produce the graphical output in the specified suffix-format.\n
$PROGNAME data.root MC.root suffix \t \t Will execute Gamma Conversion Analysis for data.root file and MC.root file \n \t\t\t\t\t\t\t\t\t and produce the graphical output in the specified suffix-format.\n
$PROGNAME  *-*gammaOff* \t\t\t gamma calculation switched off \n
$PROGNAME  *-*gammaOnly* \t\t\t gamma calculation only\n
$PROGNAME  *-*omegaOnly* \t\t\t\t Omega calculation only\n
$PROGNAME  *-*omegaetaOnly* \t\t\t Omega in eta binnin calculation only\n
$PROGNAME  *-*etaOnly* \t\t\t\t eta calculation only\n
$PROGNAME  *-*etaOff* \t\t\t\t eta calculation switched off\n

This script needs as a basis the output from the GammaConversion-Software which is provided with the Aliroot software, both the 'data.root' and the 'MC.root' have to be output files of this. The script then will check which cutnumbers are in these files and will ask you which exactly you want to analyse, furthermore you have to set a standard cut which has to be always the first in the CutLogFile. Because for this cut the systematic errors will be calculated. Not only one analysis will be fullfiled, you can additionally choose to do a Alpha studie or Chi2 of the meson as well which will give you the oportunity to calculate two indepent error due to cut variation. Additionally the CorrectSignal.C will correct the spectrum for all possible contributions and afterwards calculate the systematic error due to yield extraction. All these error will then enter the final results where you will have plots with only statistical errors as well as systematic + static errors. Several data output files are created, and
stored in the corresponding cut-directory or the working directory for the cutsstudies file.

    "
    exit
}

if [ "$#" == "0" ]; then
    echo -e "
$PROGNAME -h  \t\t\t\t\t usage will be displayed
$PROGNAME --help \t\t\t\t usage will be displayed
$PROGNAME -c data.root suffix \t\t\t Will only correct and produce the final results for already existing RAWdata \n \t\t\t\t\t\t\t\t\t and produce the graphical output in the specified suffix-format.\n
$PROGNAME -d data.root suffix \t\t\t Will only fullfil cutstudies and produce the final results for already existing RAWdata \n \t\t\t\t\t\t\t\t\t and produce the graphical output in the specified suffix-format.\n
$PROGNAME -m DirectoryOfMergedOutputs suffix \t Automatic merging for LHC10bc and LHC10de for efficiencies will be done \n \t\t\t\t\t\t\t\t\t according to the fraction in Data, be careful files have to be arranged in a certain structure \n
    \t\t\t\t\t\t\t\t DataRootFile=DirectoryOfMergedOutputs/mergedALL/GammaConvV1Data.root
    \t\t\t\t\t\t\t\t MCRootFile=DirectoryOfMergedOutputs/mergedALL/GammaConvV1MC.root
    \t\t\t\t\t\t\t\t MCRootFileBC=DirectoryOfMergedOutputs/mergedBC/GammaConvV1MC.root
    \t\t\t\t\t\t\t\t MCRootFileD=DirectoryOfMergedOutputs/mergedDE/GammaConvV1MC.root\n
$PROGNAME -r data.root suffix \t\t\t Will only execute the production of the final results and \n \t\t\t\t\t\t\t\t\t produce the graphical output in the specified suffix-format.\n
$PROGNAME data.root MC.root suffix \t \t Will execute Gamma Conversion Analysis for data.root file and MC.root file \n \t\t\t\t\t\t\t\t\t and produce the graphical output in the specified suffix-format.\n
$PROGNAME  *-*gammaOff* \t\t\t gamma calculation switched off \n
$PROGNAME  *-*gammaOnly* \t\t\t gamma calculation only\n
$PROGNAME  *-*omegaOnly* \t\t\t\t omega calculation only\n
$PROGNAME  *-*omegaetaOnly* \t\t\t omega in eta binnin calculation only\n
$PROGNAME  *-*etaOnly* \t\t\t\t eta calculation only\n
$PROGNAME  *-*etaOff* \t\t\t\t eta calculation switched off\n
"
    exit
fi
echo "------------------------------------------------------"
echo "Parameter 1: $1"
echo "Parameter 2: $2"
echo "Parameter 3: $3"
echo "Parameter 4: $4"
echo "Parameter 5: $5"
echo "------------------------------------------------------"

if [[ "$1" == *-h* ]] ; then
    Usage
elif [[ "$1" == *-c* ]] ; then
    ONLYCORRECTION=1
    DataRootFile=$2
    Suffix=$3;
elif [[ "$1" == "-omegaOnly" ]] ; then
    ONLYOMEGA=1
    DataRootFile=$2
    MCRootFile=$3
    Suffix=$4;
    AltCorrFile=$5
    if [ -f $DataRootFile ]; then
        dataFileOK=1
        echo "The data file specified is $DataRootFile"
        echo ""
    else
        echo "No data file specified, analysis can not be fullfiled. Will only do ExtractSIgnal for MC file "
        echo ""
#    exit
    fi
    if [ -f $MCRootFile ]; then
        echo "The MC file specified is $MCRootFile"
        echo ""
    else
        echo "No MC file specified, analysis will only made paritally, please be careful with the results."
        echo ""
        PARTLY=1
        MCFILE=0
    fi
    if [ -f $AltCorrFile ]; then
        altAccFileGiven=1
        echo "An additional file $AltCorrFile was given, which will be used to load the acceptance histogram."
        echo ""
    elif [ $AltCorrFile != "none" ]; then
        echo "An additional file was not found: $AltCorrFile"
        echo ""
    fi
elif [[ "$1" == "-etaOnly" ]] ; then
    ONLYETA=1
    DataRootFile=$2
    MCRootFile=$3
    Suffix=$4;
    AltCorrFile=$5
    if [ -f $DataRootFile ]; then
        dataFileOK=1
        echo "The data file specified is $DataRootFile"
    else
        echo "No data file specified, analysis can not be fullfiled."
#    exit
    fi
    if [ -f $AltCorrFile ] && ![ -z "$AltCorrFile" ]; then
        altAccFileGiven=1
        echo "An additional file $AltCorrFile was given, which will be used to load the acceptance histogram."
    fi
if [ -f $MCRootFile ]; then
    echo "The MC file specified is $MCRootFile"
else
    echo "No MC file specified, analysis will only made paritally, please be careful with the results."
    PARTLY=1
    MCFILE=0
fi
elif [[ "$1" == -d* ]] ; then
    #ONLYCORRECTION=1
    ONLYCUTS=1
    DataRootFile=$2
    MCRootFile=$3
    Suffix=$4;
    AltCorrFile=$5
    if [ -f $AltCorrFile ]; then
        altAccFileGiven=1
        echo "An additional file $AltCorrFile was given, which will be used to load the acceptance histogram."
    fi
elif [[ "$1" == *-r* ]] ; then
    ONLYCORRECTION=1
    ONLYRESULTS=1
    DataRootFile=$2
    echo $DataRootFile #DEBUG
    Suffix=$3;
elif [[ "$1" != -* ]] ; then
    DataRootFile=$1
    MCRootFile=$2
    Suffix=$3;
    if [ -f $DataRootFile ]; then
        dataFileOK=1
        echo "The data file specified is $DataRootFile"
    else
        echo "No data file specified, will run MC only"
        PARTLY=1
#    exit
    fi
    if [ -f $MCRootFile ]; then
        echo "The MC file specified is $MCRootFile"
    else
        echo "No MC file specified, analysis will only made paritally, please be careful with the results."
        PARTLY=1
        MCFILE=0
    fi
else
    DataRootFile=$1
    MCRootFile=$2
    Suffix=$3;
    AltCorrFile=$4
    if [ -f $DataRootFile ]; then
        dataFileOK=1
        echo "The data file specified is $DataRootFile"
    else
        echo "No data file specified, only ExtractSignal on MC file is run"
        PARTLY=1
    #    exit
    fi
    if [ -f $MCRootFile ]; then
        echo "The MC file specified is $MCRootFile"
    else
        echo "No MC file specified, analysis will only made paritally, please be careful with the results."
        PARTLY=1
        MCFILE=0
    fi
    if [ -f $AltCorrFile  ] && ![-z "$AltCorrFile" ]; then
        altAccFileGiven=1
        echo "An additional file $AltCorrFile was given, which will be used to load the acceptance histogram."
    fi
fi

PERIODNAME="No"

if [ $ONLYCUTS -eq 1 ]; then
    correct=0
    while [ $correct -eq 0 ]
    do
    echo "Which name should your CutStudies have? None/*name*"
    read answer
    if [ $answer = "None" ]; then
        correct=1
    else
        NAMECUTSTUDIES=$answer
        echo "You have selcted the name: " $NAMECUTSTUDIES;
        correct=1
    fi
    done

    correct=0
    while [ $correct -eq 0 ]
    do
    echo "Are these CutStudies for a specific data period? No/*name*"
    read answer
    if [ $answer = "No" ] || [ $answer = "N" ] || [ $answer = "no" ] || [ $answer = "n" ]; then
        correct=1
    else
        PERIODNAME=$answer
        echo "The period name was set to: " $PERIODNAME;
        correct=1
    fi
    done
fi

correct=0
while [ $correct -eq 0 ]
do
    echo "Which mode are you running? 40 (PCM-PCM *), 41 (PCM-EMCAL *), 42 (PCM-PHOS *), 43 (PCM-DCAL), 44 (EMCAL-EMCAL *), 45 (PHOS-PHOS*), 46 (DCAL-DCAL), 47 (PCM-DALITZ), 48 (EMCAL-DALITZ), 49 (PHOS-DALITZ), 50 (DCAL-DALITZ)"
    echo "For new NDM task choose:    60 (PCM-PCM *), 61 (PCM-EMCAL *), 62 (PCM-PHOS *), 63 (PCM-DCAL), 64 (EMCAL-EMCAL *), 65 (PHOS-PHOS*), 66 (DCAL-DCAL), 67 (PCM-DALITZ), 68 (EMCAL-DALITZ), 69 (PHOS-DALITZ), 70 (DCAL-DALITZ)"
    read answer
    if [ $answer = "40" ]; then
        echo "You are analysing PCM-PCM output";
        mode=40
        correct=1
    elif [ $answer = "41" ]; then
         echo "You are analysing PCM-EMCAL output";
         mode=41
         #AdvMesonQA="AdvancedMesonQA"
         correct=1
    elif [ $answer = "42" ]; then
         echo "You are analysing PCM-PHOS output";
         mode=42
         AdvMesonQA="AdvancedMesonQA"
         correct=1
    elif [ $answer = "43" ]; then
         echo "You are analysing PCM-DCAL output";
         mode=43
         #AdvMesonQA="AdvancedMesonQA"
         correct=1
    elif [ $answer = "44" ]; then
        echo "You are analysing EMCAL-EMCAL output";
        mode=44
        AdvMesonQA="AdvancedMesonQA"
        correct=1
    elif [ $answer = "45" ]; then
        echo "You are analysing PHOS-PHOS output";
        mode=45
        AdvMesonQA="AdvancedMesonQA"
        correct=1
    elif [ $answer = "46" ]; then
        echo "You are analysing DCAL-DCAL output";
        mode=46
        AdvMesonQA="AdvancedMesonQA"
        correct=1
    elif [ $answer = "47" ]; then
        echo "You are analysing PCM-DALITZ output";
        mode=47
        AdvMesonQA="AdvancedMesonQA"
        correct=1
    elif [ $answer = "48" ]; then
        echo "You are analysing EMCAL-DALITZ output";
        mode=48
        AdvMesonQA="AdvancedMesonQA"
        correct=1
    elif [ $answer = "49" ]; then
        echo "You are analysing PHOS-DALITZ output";
        mode=49
        AdvMesonQA="AdvancedMesonQA"
        correct=1
    elif [ $answer = "50" ]; then
        echo "You are analysing DCAL-DALITZ output";
        mode=50
        AdvMesonQA="AdvancedMesonQA"
        correct=1
    elif [ $answer = "60" ]; then
        echo "You are analysing PCM-PCM output";
        mode=60
        AdvMesonQA="AdvancedMesonQA"
        correct=1
    elif [ $answer = "61" ]; then
         echo "You are analysing PCM-EMCAL output";
         mode=61
        #AdvMesonQA="AdvancedMesonQA"
        correct=1
    elif [ $answer = "62" ]; then
         echo "You are analysing PCM-PHOS output";
         mode=62
         AdvMesonQA="AdvancedMesonQA"
         correct=1
    elif [ $answer = "63" ]; then
         echo "You are analysing PCM-DCAL output";
         mode=63
         #AdvMesonQA="AdvancedMesonQA"
         correct=1
    elif [ $answer = "64" ]; then
         echo "You are analysing EMCAL-EMCAL output";
         mode=64
         AdvMesonQA="AdvancedMesonQA"
         correct=1
    elif [ $answer = "65" ]; then
         echo "You are analysing PHOS-PHOS output";
         mode=65
         AdvMesonQA="AdvancedMesonQA"
         correct=1
    elif [ $answer = "66" ]; then
         echo "You are analysing DCAL-DCAL output";
         mode=66
         AdvMesonQA="AdvancedMesonQA"
         correct=1
    elif [ $answer = "67" ]; then
         echo "You are analysing PCM-DALITZ output";
         mode=67
         AdvMesonQA="AdvancedMesonQA"
         correct=1
    elif [ $answer = "68" ]; then
         echo "You are analysing EMCAL-DALITZ output";
         mode=68
         AdvMesonQA="AdvancedMesonQA"
         correct=1
    elif [ $answer = "69" ]; then
         echo "You are analysing PHOS-DALITZ output";
         mode=69
         AdvMesonQA="AdvancedMesonQA"
         correct=1
    elif [ $answer = "70" ]; then
         echo "You are analysing DCAL-DALITZ output";
         mode=70
         AdvMesonQA="AdvancedMesonQA"
         correct=1
    else
        echo "Command not found. Please try again.";
    fi
done


correct=0
while [ $correct -eq 0 ]
do
    echo "Do you want to take an already exitsting CutSelection.log-file. Yes/No"
    read answer
    if [ $answer = "Yes" ] || [ $answer = "Y" ] || [ $answer = "y" ] || [ $answer = "yes" ]; then
        echo "Chosen already existing logfile ...";
        cat CutSelection.log
        correct=1
    elif [ $answer = "No" ] || [ $answer = "N" ] || [ $answer = "no" ] || [ $answer = "n" ]; then
        if [ $dataFileOK -eq 1 ] ; then
            root -b -q -x -l TaskV1/MakeCutLog.C\(\"$DataRootFile\"\,\"CutSelection.log\"\,$mode\)
            correct=1
        else
            echo $DataRootFile #DEBUGGING
            echo "No data file found, will use MC root file for CutSelection"
            root -b -q -x -l TaskV1/MakeCutLog.C\(\"$MCRootFile\"\,\"CutSelection.log\"\,$mode\)
            correct=1
        fi
    else
        echo "Command not found. Please try again.";
    fi
done

correct=0
while [ $correct -eq 0 ]
do
    echo "Which collision system do you want to process? 8TeV (pp@8TeV), 7TeV (pp@7TeV), 13TeV (pp@13TeV), 900GeV (pp@900GeV), 2.76TeV (pp@2.76TeV), PbPb_2.76TeV (PbPb@2.76TeV), pPb_5.023TeV (pPb@5.023TeV)"
    read answer
    if [ $answer = "7TeV" ] || [ $answer = "7" ]; then
        energy="7TeV";
    elif [ $answer = "7TeVSys" ] || [ $answer = "7Sys" ]; then
        energy="7TeVSys";
    elif [ $answer = "5TeV" ] || [ $answer = "5" ]; then
        energy="5TeV";
    elif [ $answer = "8TeV" ] || [ $answer = "8" ]; then
        energy="8TeV";
    elif [ $answer = "13TeV" ] || [ $answer = "13" ]; then
        energy="13TeV";
    #elif [ $answer = "13TeVLowB" ]; then
    #    energy="13TeVLowB";
    elif [ $answer = "900GeV" ] || [ $answer = "900" ] || [ $answer = "9" ] || [ $answer = "0.9" ]; then
        energy="900GeV";
    elif [ $answer = "2.76TeV" ] || [ $answer = "2" ] || [ $answer = "2.76" ]; then
        energy="2.76TeV";
    elif [ $answer = "PbPb_2.76TeV" ] || [ $answer = "PbPb_2.76" ] || [ $answer = "PbPb2" ] || [ $answer = "Pb2" ]; then
        energy="PbPb_2.76TeV";
    #elif [ $answer = "5TeV" ] || [ $answer = "5.02TeV" ] || [ $answer = "5" ] || [ $answer = "5.02" ]; then
    #    energy="5TeV";
    #elif [ $answer = "PbPb_5.02TeV" ] || [ $answer = "PbPb_5.02" ] || [ $answer = "PbPb5" ] || [ $answer = "Pb5" ]; then
    #    energy="PbPb_5.02TeV";
    elif [ $answer = "pPb_5.023TeV" ] || [ $answer = "pPb_5.023" ] || [ $answer = "pPb5" ];  then
        energy="pPb_5.023TeV";
    fi
    echo "The collision system has been selected to be $energy."

    if [ $energy = "900GeV" ]; then
        if [ $ONLYCORRECTION -eq 0 ]; then
            GiveBinning900GeV
        else
            correctOmega=1
        fi
        if [ $correctOmega -eq 0 ]; then
            correct=0
        else
            correct=1
        fi
        if [ $mode = 2 ] || [ $mode = 3 ] || [ $mode = 4 ] || [ $mode = 5 ]; then
            AdvMesonQA="AdvancedMesonQA"
        fi
    elif [ $energy = "2.76TeV" ]; then
        if [ $ONLYRESULTS -eq 0 ]; then
            GiveBinning2760GeV
        else
            correctOmega=1
        fi
        if [ $correctOmega -eq 0 ]; then
            correct=0
        else
            correct=1
        fi
    #elif [ $energy = "5TeV" ] ; then
    #    if [ $ONLYCORRECTION -eq 0 ]; then
    #        GiveBinning5TeV
    #    else
    #        correctOmega=1
    #    fi
    #    if [ $correctOmega -eq 0 ]; then
    #        correct=0
    #    else
    #        correct=1
    #    fi

    elif [ $energy = "7TeV" ] ; then
        if [ $ONLYCORRECTION -eq 0 ]; then
            GiveBinning7TeVOmega
            GiveBinning7TeVEta
        else
            correctOmega=1
        fi
        if [ $correctOmega -eq 0 ]; then
            correct=0
        else
            correct=1
        fi

        if [ $mode = 2 ] || [ $mode = 3 ] || [ $mode = 4 ] || [ $mode = 5 ]; then
            AdvMesonQA="AdvancedMesonQA"
        fi
    elif [ $energy = "5TeV" ] ; then
        if [ $ONLYCORRECTION -eq 0 ]; then
            GiveBinning5TeV
        else
            correctOmega=1
        fi
        if [ $correctOmega -eq 0 ]; then
            correct=0
        else
            correct=1
        fi

        if [ $mode = 2 ] || [ $mode = 3 ] || [ $mode = 4 ] || [ $mode = 5 ]; then
            AdvMesonQA="AdvancedMesonQA"
        fi
    elif [ $energy = "7TeVSys" ] ; then
        if [ $ONLYCORRECTION -eq 0 ]; then
            GiveBinning7TeVOmega
            GiveBinning7TeVEta
        else
            correctOmega=1
        fi
        if [ $correctOmega -eq 0 ]; then
            correct=0
        else
            correct=1
        fi

        if [ $mode = 2 ] || [ $mode = 3 ] || [ $mode = 4 ] || [ $mode = 5 ]; then
            AdvMesonQA="AdvancedMesonQA"
        fi
    elif [ $energy = "8TeV" ]; then
        if [ $ONLYCORRECTION -eq 0 ]; then
            GiveBinning8TeV
        else
            correctOmega=1
        fi
        if [ $correctOmega -eq 0 ]; then
            correct=0
        else
            correct=1
        fi

        if [ $mode = 2 ] || [ $mode = 3 ] || [ $mode = 4 ] || [ $mode = 5 ]; then
            AdvMesonQA="AdvancedMesonQA"
        fi
    elif [ $energy = "13TeV" ]; then
        if [ $ONLYCORRECTION -eq 0 ]; then
            GiveBinning13TeV
        fi
        if [ $correctOmega -eq 0 ]; then
            correct=0
        else
            correct=1
        fi
    elif [ $energy = "pPb_5.023TeV" ]; then
        if [ $ONLYCORRECTION -eq 0 ]; then
            GiveBinningpPb
        else
            correctOmega=1
        fi
        if [ $correctOmega -eq 0 ]; then
            correct=0
        else
            correct=1
        fi

    elif [ $energy = "PbPb_2.76TeV" ]; then
        if [ $ONLYCORRECTION -eq 0 ]; then
            GiveBinningHI
        else
            correctOmega=1
        fi
        if [ $correctOmega -eq 0 ]; then
            correct=0
        else
            correct=1
        fi

   elif [ $energy = "PbPb_5.02TeV" ]; then
        if [ $ONLYCORRECTION -eq 0 ]; then
            GiveBinningHI5020GeV
        else
            correctOmega=1
        fi

        if [ $correctOmega -eq 0 ]; then
            correct=0
        else
            correct=1
        fi
     fi

done

echo "mode has been chosen: $mode "

echo "I went into standard modes";
if [ $ONLYRESULTS = 0 ] ; then
    if [ $ONLYCORRECTION -eq 0 ];  then
        correct=0
        while [ $correct -eq 0 ]
        do
            echo "Which fit do you want to do? CrystalBall or gaussian convoluted with an exponential function? CrystalBall/Gaussian?";
            ContaminationBackHistMode=0
            read answer
            if [ $answer = "CrystalBall" ] || [ $answer = "C" ] || [ $answer = "c" ]; then
                echo "CrystalBall chosen ...";
                correct=1
                crystal=CrystalBall
            elif [ $answer = "Gaussian" ] || [ $answer = "G" ] || [ $answer = "g" ]; then
                echo "Gaussian chosen ...";
                correct=1
                crystal=Gaussian
            elif [ $answer = "GaussianContamination0" ] || [ $answer = "GContamination0" ] || [ $answer = "gContamination0" ]||[ $answer = "GaussianCont" ] || [ $answer = "GCont0" ] || [ $answer = "gCont0" ]||[ $answer = "GaussianCon0" ] || [ $answer = "GCon0" ] || [ $answer = "gCon0" ]||[ $answer = "GaussianC0" ] || [ $answer = "GC0" ] || [ $answer = "gC0" ] || [ $answer = "gc0" ]; then
                echo "Gaussian chosen ...";
                correct=1
                crystal=Gaussian
                ContaminationBackHistMode=0
            elif [ $answer = "GaussianContamination1" ] || [ $answer = "GContamination1" ] || [ $answer = "gContamination1" ]||[ $answer = "GaussianCont" ] || [ $answer = "GCont1" ] || [ $answer = "gCont1" ]||[ $answer = "GaussianCon1" ] || [ $answer = "GCon1" ] || [ $answer = "gCon1" ]||[ $answer = "GaussianC1" ] || [ $answer = "GC1" ] || [ $answer = "gC1" ] || [ $answer = "gc1" ]; then
                ContaminationBackHistMode=1
                echo "Gaussian chosen with Contamination == $ContaminationBackHistMode...";
                correct=1
                crystal=Gaussian
            elif [ $answer = "GaussianContamination2" ] || [ $answer = "GContamination2" ] || [ $answer = "gContamination2" ]||[ $answer = "GaussianCont" ] || [ $answer = "GCont2" ] || [ $answer = "gCont2" ]||[ $answer = "GaussianCon2" ] || [ $answer = "GCon2" ] || [ $answer = "gCon2" ]||[ $answer = "GaussianC2" ] || [ $answer = "GC2" ] || [ $answer = "gC2" ] || [ $answer = "gc2" ]; then
                echo "Gaussian chosen ...";
                correct=1
                crystal=Gaussian
                ContaminationBackHistMode=2
            elif [ $answer = "GaussianContamination3" ] || [ $answer = "GContamination3" ] || [ $answer = "gContamination3" ]||[ $answer = "GaussianCont" ] || [ $answer = "GCont3" ] || [ $answer = "gCont3" ]||[ $answer = "GaussianCon3" ] || [ $answer = "GCon3" ] || [ $answer = "gCon3" ]||[ $answer = "GaussianC3" ] || [ $answer = "GC3" ] || [ $answer = "gC3" ] || [ $answer = "gc3" ]; then
                ContaminationBackHistMode=3
                echo "Gaussian chosen with Contamination == $ContaminationBackHistMode...";
                correct=1
                crystal=Gaussian
            elif [ $answer = "GaussianContamination4" ] || [ $answer = "GContamination4" ] || [ $answer = "gContamination4" ]||[ $answer = "GaussianCont" ] || [ $answer = "GCont4" ] || [ $answer = "gCont4" ]||[ $answer = "GaussianCon4" ] || [ $answer = "GCon4" ] || [ $answer = "gCon4" ]||[ $answer = "GaussianC4" ] || [ $answer = "GC4" ] || [ $answer = "gC4" ] || [ $answer = "gc4" ]; then
                ContaminationBackHistMode=4
                echo "Gaussian chosen with Contamination == $ContaminationBackHistMode...";
                correct=1
                crystal=Gaussian
            elif [ $answer = "GaussianContamination" ] || [ $answer = "GContamination" ] || [ $answer = "gContamination" ]||[ $answer = "GaussianCont" ] || [ $answer = "GCont" ] || [ $answer = "gCont" ]||[ $answer = "GaussianCon" ] || [ $answer = "GCon" ] || [ $answer = "gCon" ]||[ $answer = "GaussianC" ] || [ $answer = "GC" ] || [ $answer = "gC" ] || [ $answer = "gc" ]; then
                ContaminationBackHistMode=2
                echo "Gaussian chosen with Contamination == $ContaminationBackHistMode...";
                correct=1
                crystal=Gaussian
            elif [ $answer = "CrystalBallRatio" ] || [ $answer = "CRatio" ] || [ $answer = "cRatio" ]; then
                echo -e "--> CrystalBall with Signal To Background Fit Option chosen ...\n";
                correct=1
                crystal=CrystalBallRatio
            elif [ $answer = "GaussianRatio" ] || [ $answer = "GRatio" ] || [ $answer = "gRatio" ]; then
                echo -e "--> Gaussian with Signal To Background Fit Option  chosen ...\n";
                correct=1
                crystal=GaussianRatio
            else
                echo "Command not found. Please try again.";
            fi
        done
    fi
    correct=0
    while [ $correct -eq 0 ]
    do
        echo "Please check that you really want to process all cuts, otherwise change the CutSelection.log. Remember at first all gamma cutstudies will be carried out. Make sure that the standard cut is the first in the file. Continue? Yes/No?";
        read answer
        if [ $answer = "Yes" ] || [ $answer = "Y" ] || [ $answer = "y" ] || [ $answer = "yes" ]; then
            echo "Continuing ...";
            correct=1
        elif [ $answer = "No" ] || [ $answer = "N" ] || [ $answer = "no" ] || [ $answer = "n" ]; then
            echo "Aborting ...";
            exit
        else
            echo "Command not found. Please try again.";
        fi
    done

    #Read the different cuts form the Cut selection log file
    CutSelections=`cat CutSelection.log`
    for cutSelection in $CutSelections; do
        if [ -d $cutSelection ]; then
            echo "CutSelection $cutSelection directory already exists, all files will be overwritten ";
            mkdir $cutSelection/$energy
        else
            mkdir $cutSelection
            mkdir $cutSelection/$energy
        fi

        if [ $ONLYCUTS -eq 0 ]; then
            if [ -d $cutSelection/$energy/$Suffix ]; then
                echo "Graphical Output $Suffix directory already exists, all files will be overwritten ";
            else
                mkdir $cutSelection/$energy/$Suffix
            fi

            if [ $ONLYCORRECTION -eq 0 ]; then
                echo "CutSelection is $cutSelection";
                ContaminationBackFile="none"
                if [ $ContaminationBackHistMode -gt 0 ]&&[ $MCFILE -eq 1 ]; then
                    echo "Contamination Mode activated, ContaminationBackHistMode: $ContaminationBackHistMode "
                    ContaminationBackFile=$MCRootFile
                fi
                optionsOmegaData=\"Omega\"\,\"$DataRootFile\"\,\"$cutSelection\"\,\"$Suffix\"\,\"kFALSE\"\,\"$energy\"\,\"$crystal\"\,\"$OPTMINBIASEFF\"\,\"\"\,\"$AdvMesonQA\"\,$BinsPtOmega\,kFALSE\,$mode,\"$ContaminationBackFile\"\,$ContaminationBackHistMode

                optionsEtaData=\"Eta\"\,\"$DataRootFile\"\,\"$cutSelection\"\,\"$Suffix\"\,\"kFALSE\"\,\"$energy\"\,\"$crystal\"\,\"$OPTMINBIASEFF\"\,\"\"\,\"$AdvMesonQA\"\,$BinsPtEta\,kFALSE\,$mode
                if [ \( $ONLYOMEGA -eq 1 \) -a \( -f $DataRootFile \) ]; then
                        ExtractSignal $optionsOmegaData

                        OmegadataRAWFILE=`ls $cutSelection/$energy/Omega_data_GammaConvV1WithoutCorrection_*.root`
                elif [ \( $ONLYETA -eq 1 \) -a \( -f $DataRootFile \) ]; then
                        ExtractSignal $optionsEtaData

                        EtadataRAWFILE=`ls $cutSelection/$energy/Eta_data_GammaConvV1WithoutCorrection_*.root`
                else
                        if [ -f $DataRootFile ]; then
                        ExtractSignal $optionsEtaData
                        ExtractSignal $optionsOmegaData

                        OmegadataRAWFILE=`ls $cutSelection/$energy/Omega_data_GammaConvV1WithoutCorrection_*.root`
                        EtadataRAWFILE=`ls $cutSelection/$energy/Eta_data_GammaConvV1WithoutCorrection_*.root`
                        fi
                fi

                if [ $MCFILE -eq 1 ]; then
                    optionsOmegaMC=\"Omega\"\,\"$MCRootFile\"\,\"$cutSelection\"\,\"$Suffix\"\,\"kTRUE\"\,\"$energy\"\,\"$crystal\"\,\"$OPTMINBIASEFF\"\,\"\"\,\"$AdvMesonQA\"\,$BinsPtOmega\,kFALSE\,$mode,\"$ContaminationBackFile\"\,$ContaminationBackHistMode
                    optionsEtaMC=\"Eta\"\,\"$MCRootFile\"\,\"$cutSelection\"\,\"$Suffix\"\,\"kTRUE\"\,\"$energy\"\,\"$crystal\"\,\"$OPTMINBIASEFF\"\,\"\"\,\"$AdvMesonQA\"\,$BinsPtEta\,kFALSE\,$mode

                    if [ $ONLYOMEGA -eq 1 ]; then
                        ExtractSignal $optionsOmegaMC

                        OmegaMCRAWFILE=`ls $cutSelection/$energy/Omega_MC_GammaConvV1WithoutCorrection_$cutSelection.root`
                        OmegaMCcorrectionFILE=`ls $cutSelection/$energy/Omega_MC_GammaConvV1CorrectionHistos_$cutSelection.root`
                    elif [ $ONLYETA -eq 1 ]; then
                        ExtractSignal $optionsEtaMC

                        EtaMCRAWFILE=`ls $cutSelection/$energy/Eta_MC_GammaConvV1WithoutCorrection_$cutSelection.root`
                        EtaMCcorrectionFILE=`ls $cutSelection/$energy/Eta_MC_GammaConvV1CorrectionHistos_$cutSelection.root`
                    else
                        ExtractSignal $optionsEtaMC
                        ExtractSignal $optionsOmegaMC

                        OmegaMCRAWFILE=`ls $cutSelection/$energy/Omega_MC_GammaConvV1WithoutCorrection_$cutSelection.root`
                        OmegaMCcorrectionFILE=`ls $cutSelection/$energy/Omega_MC_GammaConvV1CorrectionHistos_$cutSelection.root`
                        EtaMCRAWFILE=`ls $cutSelection/$energy/Eta_MC_GammaConvV1WithoutCorrection_$cutSelection.root`
                        EtaMCcorrectionFILE=`ls $cutSelection/$energy/Eta_MC_GammaConvV1CorrectionHistos_$cutSelection.root`
                    fi
                    #root -x -l -b -q TaskV1/CompareMesonQuantities.C\+\(\"$OmegadataRAWFILE\"\,\"$OmegaMCRAWFILE\"\,\"$cutSelection\"\,\"Omega\"\,\"$Suffix\"\,\"$energy\"\,$BinsPtOmega\,$mode\)
                fi
                if [ $MCFILE -eq 1 ]&&[ -f $DataRootFile ]; then
                    echo -e "\n________________________"
                    echo -e "COMPARE MESON QUANTITIES\n"
                    root -x -l -b -q TaskV1/CompareMesonQuantitiesOmega.C\+\(\"$OmegadataRAWFILE\"\,\"$OmegaMCRAWFILE\"\,\"$cutSelection\"\,\"Omega\"\,\"$Suffix\"\,\"$energy\"\,\"\"\,$BinsPtOmega\,$mode\)
                fi
            fi

            OmegaMCcorrectionFullFILE=`ls $cutSelection/$energy/Omega_MC_GammaConvV1CorrectionHistos_$cutSelection.root`
            EtaMCcorrectionFullFILE=`ls $cutSelection/$energy/Eta_MC_GammaConvV1CorrectionHistos_$cutSelection.root`

            if [ $altAccFileGiven -eq 1 ]; then
                optionsOmegaCorrection=\"$OmegadataRAWFILE\"\,\"$OmegaMCcorrectionFILE\"\,\"$cutSelection\"\,\"$Suffix\"\,\"Omega\"\,kFALSE\,\"$energy\"\,\"\"\,0\,kFALSE\,$mode\,kTRUE\,\"$AltCorrFile\"
                optionsOmegaCorrectionMC=\"$OmegaMCRAWFILE\"\,\"$OmegaMCcorrectionFILE\"\,\"$cutSelection\"\,\"$Suffix\"\,\"Omega\"\,kTRUE\,\"$energy\"\,\"\"\,0\,kFALSE\,$mode\,kTRUE\,\"$AltCorrFile\"

                optionsEtaCorrection=\"$EtadataRAWFILE\"\,\"$EtaMCcorrectionFILE\"\,\"$cutSelection\"\,\"$Suffix\"\,\"Eta\"\,kFALSE\,\"$energy\"\,\"\"\,0\,kFALSE\,$mode\,kTRUE\,\"$AltCorrFile\"
                optionsEtaCorrectionMC=\"$EtaMCRAWFILE\"\,\"$EtaMCcorrectionFILE\"\,\"$cutSelection\"\,\"$Suffix\"\,\"Eta\"\,kTRUE\,\"$energy\"\,\"\"\,0\,kFALSE\,$mode\,kTRUE\,\"$AltCorrFile\"
            else 
                optionsOmegaCorrection=\"$OmegadataRAWFILE\"\,\"$OmegaMCcorrectionFILE\"\,\"$cutSelection\"\,\"$Suffix\"\,\"Omega\"\,kFALSE\,\"$energy\"\,\"\"\,0\,kFALSE\,$mode
                optionsOmegaCorrectionMC=\"$OmegaMCRAWFILE\"\,\"$OmegaMCcorrectionFILE\"\,\"$cutSelection\"\,\"$Suffix\"\,\"Omega\"\,kTRUE\,\"$energy\"\,\"\"\,0\,kFALSE\,$mode

                optionsEtaCorrection=\"$EtadataRAWFILE\"\,\"$EtaMCcorrectionFILE\"\,\"$cutSelection\"\,\"$Suffix\"\,\"Eta\"\,kFALSE\,\"$energy\"\,\"\"\,0\,kFALSE\,$mode
                optionsEtaCorrectionMC=\"$EtaMCRAWFILE\"\,\"$EtaMCcorrectionFILE\"\,\"$cutSelection\"\,\"$Suffix\"\,\"Eta\"\,kTRUE\,\"$energy\"\,\"\"\,0\,kFALSE\,$mode
            fi

            if [[ -f $OmegadataRAWFILE  &&  -f $OmegaMCcorrectionFILE ]]; then
            echo "Correct Signal Data omega"
                CorrectSignal $optionsOmegaCorrection
            else
                PARTLY=1
            fi

            if [[ -f $EtadataRAWFILE  &&  -f $EtaMCcorrectionFILE ]]; then
            echo "Correct Signal Data eta"
                CorrectSignal $optionsEtaCorrection
            else
                PARTLY=1
            fi

            if [[ -f $OmegaMCRAWFILE  &&   -f $OmegaMCcorrectionFILE ]]; then
            echo "Correct Signal MC omega"
                CorrectSignal $optionsOmegaCorrectionMC
            else
                PARTLY=1
            fi

            if [[ -f $EtaMCRAWFILE  &&  -f $EtaMCcorrectionFILE  ]]; then
            echo "Correct Signal MC eta"
                CorrectSignal $optionsEtaCorrectionMC
            else
                PARTLY=1
            fi
        fi
        NORMALCUTS=`expr $NORMALCUTS + 1`
    done

    if [ $ONLYOMEGA -eq 1 ]; then
        root -x -q -l -b TaskV1/CutStudiesOverviewOmega.C\(\"CutSelection.log\"\,\"$Suffix\"\,\"Omega\"\,kFALSE\,\"$OPTMINBIASEFF\"\,\"$energy\"\,\"$NAMECUTSTUDIES\"\,$NORMALCUTS\,kFALSE\,\"\"\,\"$PERIODNAME\"\,$mode\,kFALSE\)
        root -x -q -l -b TaskV1/CutStudiesOverviewOmega.C\(\"CutSelection.log\"\,\"$Suffix\"\,\"Omega\"\,kTRUE\,\"$OPTMINBIASEFF\"\,\"$energy\"\,\"$NAMECUTSTUDIES\"\,$NORMALCUTS\,kFALSE\,\"\"\,\"$PERIODNAME\"\,$mode\,kFALSE\)
    elif [ $ONLYETA -eq 1 ]; then
        root -x -q -l -b TaskV1/CutStudiesOverviewOmega.C\(\"CutSelection.log\"\,\"$Suffix\"\,\"Eta\"\,kFALSE\,\"$OPTMINBIASEFF\"\,\"$energy\"\,\"$NAMECUTSTUDIES\"\,$NORMALCUTS\,kFALSE\,\"\"\,\"$PERIODNAME\"\,$mode\,kFALSE\)
        root -x -q -l -b TaskV1/CutStudiesOverviewOmega.C\(\"CutSelection.log\"\,\"$Suffix\"\,\"Eta\"\,kTRUE\,\"$OPTMINBIASEFF\"\,\"$energy\"\,\"$NAMECUTSTUDIES\"\,$NORMALCUTS\,kFALSE\,\"\"\,\"$PERIODNAME\"\,$mode\,kFALSE\)
    else
        root -x -q -l -b TaskV1/CutStudiesOverviewOmega.C\(\"CutSelection.log\"\,\"$Suffix\"\,\"Omega\"\,kFALSE\,\"$OPTMINBIASEFF\"\,\"$energy\"\,\"$NAMECUTSTUDIES\"\,$NORMALCUTS\,kFALSE\,\"\"\,\"$PERIODNAME\"\,$mode\,kFALSE\)
        root -x -q -l -b TaskV1/CutStudiesOverviewOmega.C\(\"CutSelection.log\"\,\"$Suffix\"\,\"Omega\"\,kTRUE\,\"$OPTMINBIASEFF\"\,\"$energy\"\,\"$NAMECUTSTUDIES\"\,$NORMALCUTS\,kFALSE\,\"\"\,\"$PERIODNAME\"\,$mode\,kFALSE\)
        root -x -q -l -b TaskV1/CutStudiesOverviewOmega.C\(\"CutSelection.log\"\,\"$Suffix\"\,\"Eta\"\,kFALSE\,\"$OPTMINBIASEFF\"\,\"$energy\"\,\"$NAMECUTSTUDIES\"\,$NORMALCUTS\,kFALSE\,\"\"\,\"$PERIODNAME\"\,$mode\,kFALSE\)
        root -x -q -l -b TaskV1/CutStudiesOverviewOmega.C\(\"CutSelection.log\"\,\"$Suffix\"\,\"Eta\"\,kTRUE\,\"$OPTMINBIASEFF\"\,\"$energy\"\,\"$NAMECUTSTUDIES\"\,$NORMALCUTS\,kFALSE\,\"\"\,\"$PERIODNAME\"\,$mode\,kFALSE\)
    fi
fi
