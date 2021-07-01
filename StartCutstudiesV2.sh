#!/bin/bash

# energyDir="13TeVSys"
energyDir="13TeV"
# energyDir=$1
# mode="3"
mode=$1
# stdCut="00010113_0dm00009f9730000dge0404000_24466190sa01cc00000_0163103100b00010"
# stdCut="00010113_24466190sa01cc00000_0163103100000010"
stdCut=$2


##########################################################


function CutStudiesOverview() {
    energy="13TeV"
    if [[ $# = 4 ]]; then energy=$4; fi
    root -l -q -b 'TaskV1/CutStudiesOverview.C+("CutSelection.log", "eps", "Pi0", "kFALSE",     "", "'$energy'", "'$1'", '$2', 0, "", "LHC161718", '$3', 0, 1, 1)'
    root -l -q -b 'TaskV1/CutStudiesOverview.C+("CutSelection.log", "eps", "Pi0", "kTRUE",      "", "'$energy'", "'$1'", '$2', 0, "", "LHC161718", '$3', 0, 1, 1)'
    root -l -q -b 'TaskV1/CutStudiesOverview.C+("CutSelection.log", "eps", "Eta", "kFALSE",     "", "'$energy'", "'$1'", '$2', 0, "", "LHC161718", '$3', 0, 1, 1)'
    root -l -q -b 'TaskV1/CutStudiesOverview.C+("CutSelection.log", "eps", "Eta", "kTRUE",      "", "'$energy'", "'$1'", '$2', 0, "", "LHC161718", '$3', 0, 1, 1)'
    root -l -q -b 'TaskV1/CutStudiesOverview.C+("CutSelection.log", "eps", "Pi0EtaBinning", "kFALSE",     "", "'$energy'", "'$1'", '$2', 0, "", "LHC161718", '$3', 0, 1, 0)'
    root -l -q -b 'TaskV1/CutStudiesOverview.C+("CutSelection.log", "eps", "Pi0EtaBinning", "kTRUE",      "", "'$energy'", "'$1'", '$2', 0, "", "LHC161718", '$3', 0, 1, 0)'
}
function CompareDifferentDirectories() {
    root -l -b -q 'TaskV1/CompareDifferentDirectories.C+("CutSelection.log", "eps", "Pi0", 0, "13TeV", '$2', "No", '$3',"'$1'")'
    root -l -b -q 'TaskV1/CompareDifferentDirectories.C+("CutSelection.log", "eps", "Pi0", 1, "13TeV", '$2', "No", '$3',"'$1'")'
    root -l -b -q 'TaskV1/CompareDifferentDirectories.C+("CutSelection.log", "eps", "Eta", 0, "13TeV", '$2', "No", '$3',"'$1'")'
    root -l -b -q 'TaskV1/CompareDifferentDirectories.C+("CutSelection.log", "eps", "Eta", 1, "13TeV", '$2', "No", '$3',"'$1'")'
    root -l -b -q 'TaskV1/CompareDifferentDirectories.C+("CutSelection.log", "eps", "Pi0EtaBinning", 0, "13TeV", '$2', "No", '$3',"'$1'")'
    root -l -b -q 'TaskV1/CompareDifferentDirectories.C+("CutSelection.log", "eps", "Pi0EtaBinning", 1, "13TeV", '$2', "No", '$3',"'$1'")'
}

function AddToList(){
	if [ -f $1 ]; then
		for tmp in `cat $1`
		do
			if [[ -f $2 ]]; then
				if [[ `grep $tmp $2 | wc -l` < 1 ]]; then
					echo "$tmp" >> $2
				fi
			else
				echo "$tmp" >> $2
			fi
		done
		rm $1
	fi
}


# gridpath="/media/adrian/Data/grid_data"
# dir="$gridpath/MergedDecayMCs/variations/"
# if [[ ! -f "CutSelectionAll.log" ]]; then
#     for file in `ls $dir`; do
#         root -l -b -q 'TaskV1/MakeCutLog.C("'$dir/$file'","CutSelection.log",'$mode')' &> /dev/null
#         echo $file
#         cat "CutSelection.log"
#         AddToList "CutSelection.log" "CutSelectionAll.log"
#     done
#     echo
#     echo "-----------------------"
#     echo
#     cat "CutSelectionAll.log"
# fi

if [[  -f "CutSelectionAll.log" ]]; then rm "CutSelectionAll.log"; fi
for Cut in `ls | grep 000`; do
    # root -l -b -q 'TaskV1/MakeCutLog.C("'$dir/$file'","CutSelection.log",'$mode')' &> /dev/null
    echo $Cut
    if [[ ! -f "CutSelectionAll.log" ]] || [[ `grep $Cut "CutSelectionAll.log" | wc -l` < 1 ]]; then
        echo "$Cut" >> "CutSelectionAll.log"
    fi
done
echo
echo "-----------------------"
echo
# cat "CutSelectionAll.log"

declare -A cutstringConvCalo
declare -A cutstringCalo

cutstringConvCalo[0]="HeavyIon"
cutstringConvCalo[1]="CentralityMin"
cutstringConvCalo[2]="CentralityMax"
cutstringConvCalo[3]="Trigger"
cutstringConvCalo[4]="Trigger"
cutstringConvCalo[5]="RemovePileUp"
cutstringConvCalo[6]="RejectExtraSignals"
cutstringConvCalo[7]="VertexCut"
cutstringConvCalo[8]="_"
cutstringConvCalo[9]="V0FinderType"
cutstringConvCalo[10]="EtaCut"
cutstringConvCalo[11]="MinRCut"
cutstringConvCalo[12]="EtaForPhiCut"
cutstringConvCalo[13]="MinPhiCut"
cutstringConvCalo[14]="MaxPhiCut"
cutstringConvCalo[15]="SinglePt"
cutstringConvCalo[16]="TPCCluster"
cutstringConvCalo[17]="dEdxE"
cutstringConvCalo[18]="dEdxPi"
cutstringConvCalo[19]="dEdxPivsp"
cutstringConvCalo[20]="dEdxPivsp"
cutstringConvCalo[21]="dEdxPivsp"
cutstringConvCalo[22]="TOFelectronPID"
cutstringConvCalo[23]="ITSelectronPID"
cutstringConvCalo[24]="TRDelectronPID"
cutstringConvCalo[25]="Qt"
cutstringConvCalo[26]="Chi2"
cutstringConvCalo[27]="PsiPair"
cutstringConvCalo[28]="DoPhotonAsymmetryCut"
cutstringConvCalo[29]="cosPA"
cutstringConvCalo[30]="SharedElectronCuts"
cutstringConvCalo[31]="RejectToCloseV0s"
cutstringConvCalo[32]="DcaRPrimVtx"
cutstringConvCalo[33]="DcaZPrimVtx"
cutstringConvCalo[34]="EventPlane"
cutstringConvCalo[35]="_"
cutstringConvCalo[36]="ClusterType"
cutstringConvCalo[37]="EtaMin"
cutstringConvCalo[38]="EtaMax"
cutstringConvCalo[39]="PhiMin"
cutstringConvCalo[40]="PhiMax"
cutstringConvCalo[41]="ClusterNonLinearity"
cutstringConvCalo[42]="ClusterNonLinearity"
cutstringConvCalo[43]="DistanceToBadChannel"
cutstringConvCalo[44]="CellTiming"
cutstringConvCalo[45]="ClusterTrackMatching"
cutstringConvCalo[46]="ExoticCluster"
cutstringConvCalo[47]="ClusterMinEnergy"
cutstringConvCalo[48]="ClusterNCells"
cutstringConvCalo[49]="ClusterM02"
cutstringConvCalo[50]="ClusterM02"
cutstringConvCalo[51]="ClusterM20"
cutstringConvCalo[52]="RecConv"
cutstringConvCalo[53]="MaximumDispersion"
cutstringConvCalo[54]="NLM"
cutstringConvCalo[55]="_"
cutstringConvCalo[56]="MesonKind"
cutstringConvCalo[57]="BackgroundScheme"
cutstringConvCalo[58]="NumberOfBGEvents"
cutstringConvCalo[59]="DegreesForRotationMethod"
cutstringConvCalo[60]="RapidityMesonCut"
cutstringConvCalo[61]="PtCut"
cutstringConvCalo[62]="Alpha"
cutstringConvCalo[63]="SelectionWindow"
cutstringConvCalo[64]="SharedElectronCuts"
cutstringConvCalo[65]="RejectToCloseV0s"
cutstringConvCalo[66]="MCSmearing"
cutstringConvCalo[67]="DcaGammaGamma"
cutstringConvCalo[68]="DcaRPrimVtx"
cutstringConvCalo[69]="DcaZPrimVtx"
cutstringConvCalo[70]="OpeningAngle"
cutstringConvCalo[71]="OpeningAngle"

cutstringCalo[0]="HeavyIon"
cutstringCalo[1]="CentralityMin"
cutstringCalo[2]="CentralityMax"
cutstringCalo[3]="Trigger"
cutstringCalo[4]="Trigger"
cutstringCalo[5]="RemovePileUp"
cutstringCalo[6]="RejectExtraSignals"
cutstringCalo[7]="VertexCut"
cutstringCalo[8]="_"
cutstringCalo[9]="ClusterType"
cutstringCalo[10]="EtaMin"
cutstringCalo[11]="EtaMax"
cutstringCalo[12]="PhiMin"
cutstringCalo[13]="PhiMax"
cutstringCalo[14]="ClusterNonLinearity"
cutstringCalo[15]="ClusterNonLinearity"
cutstringCalo[16]="DistanceToBadChannel"
cutstringCalo[17]="CellTiming"
cutstringCalo[18]="ClusterTrackMatching"
cutstringCalo[19]="ExoticCluster"
cutstringCalo[20]="ClusterMinEnergy"
cutstringCalo[21]="ClusterNCells"
cutstringCalo[22]="ClusterM02"
cutstringCalo[23]="ClusterM02"
cutstringCalo[24]="ClusterM20"
cutstringCalo[25]="RecConv"
cutstringCalo[26]="MaximumDispersion"
cutstringCalo[27]="NLM"
cutstringCalo[28]="_"
cutstringCalo[29]="MesonKind"
cutstringCalo[30]="BackgroundScheme"
cutstringCalo[31]="NumberOfBGEvents"
cutstringCalo[32]="DegreesForRotationMethod"
cutstringCalo[33]="RapidityMesonCut"
cutstringCalo[34]="PtCut"
cutstringCalo[35]="Alpha"
cutstringCalo[36]="SelectionWindow"
cutstringCalo[37]="SharedElectronCuts"
cutstringCalo[38]="RejectToCloseV0s"
cutstringCalo[39]="MCSmearing"
cutstringCalo[40]="DcaGammaGamma"
cutstringCalo[41]="DcaRPrimVtx"
cutstringCalo[42]="DcaZPrimVtx"
cutstringCalo[43]="OpeningAngle"
cutstringCalo[44]="OpeningAngle"

for file in  `ls CutsForVariation*`; do 
    rm $file
done
echo
for Cut in `cat "CutSelectionAll.log"`; do
    NVar=0
    NameStudy=""
    for StringPos in  {0..71}; do 
        if [[ ! ${Cut:$StringPos:1} = ${stdCut:$StringPos:1} ]]; then 
            ((NVar++))
            if [[ $mode = "2" ]] || [[ $mode = "3" ]]; then
                NameStudyTmp="${cutstringConvCalo[$StringPos]}"
            else
                NameStudyTmp="${cutstringCalo[$StringPos]}"
            fi
            if [[ $NameStudy == "" ]]; then
                NameStudy="$NameStudyTmp"
            else
                if [[ ! "$NameStudyTmp" == *"$NameStudy"* ]]; then
                    NameStudy="${NameStudy}${NameStudyTmp}"
                fi
            fi
            # printf "[$StringPos:${Cut:$StringPos:1}-${stdCut:$StringPos:1}] "
        fi
    done
    # echo 
    if [[ $NVar > 0 ]]; then
        if [[ $NVar > 3 ]]; then 
            # printf "$Cut : too many cuts varried \n"
            NameStudy="Many"
        # else
            # printf "$Cut : $NameStudy \n"
        fi
        if [[ ! -f "CutsForVariation$NameStudy.log" ]] || [[ `grep $Cut "CutsForVariation$NameStudy.log" | wc -l` < 1 ]]; then
            echo "$Cut" >> "CutsForVariation$NameStudy.log"
        fi
    fi
done



if [[ $# < 3 ]]; then
    for FileCuts in  `ls CutsForVariation*`; do 
        FileCuts2=${FileCuts#*CutsForVariation}
        NameStudy=${FileCuts2%.log}
        printf "$NameStudy \n"
        if  [[ $NameStudy == "Many" ]] || [[ $NameStudy == "Trigger" ]]; then echo; continue; fi
        cat $FileCuts
        echo
    done
    echo
    echo "-----------------------"
    echo
    for FileCuts in  `ls CutsForVariation*`; do 
        FileCuts2=${FileCuts#*CutsForVariation}
        NameStudy=${FileCuts2%.log}
        if  [[ $NameStudy == "Many" ]] || [[ $NameStudy == "Trigger" ]]; then continue; fi
        printf "\n\n$NameStudy \n------------------\n"
        if [[ -f $FileCuts ]]; then 
            echo -e "$stdCut" >   CutSelection.log
            for Cut in `cat $FileCuts`; do
                echo -e "$Cut" >>   CutSelection.log
            done
            NCuts=`cat CutSelection.log | wc  -l`
            CutStudiesOverview $NameStudy $NCuts $mode $energyDir
        fi
    done
else
    if [[ $mode = "2" ]]  || [[ $mode = "3" ]]; then
        NameStudy="${cutstringConvCalo[$3]}"
    else
        NameStudy="${cutstringCalo[$3]}"
    fi
    FileCuts="CutsForVariation$NameStudy.log"
    printf "$NameStudy \n"
    cat $FileCuts
    echo -e "$stdCut" >   CutSelection.log
    for Cut in `cat $FileCuts`; do
        echo -e "$Cut" >>   CutSelection.log
    done
    NCuts=`cat CutSelection.log | wc  -l`
    CutStudiesOverview $NameStudy $NCuts $mode $energyDir
fi



