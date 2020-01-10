#! /bin/bash

#! /bin/bash
# This Script is intended to automize the download of train outputs

# Version: V5.5
echo  -e "\e[36m+++++++++++++++++++++++++++++++++++++\e[0m"
echo "DownScript.sh Version: V5.5"
sleep 2

# Author: Adrian Mechler (mechler@ikf.uni-frankfurt.de)

# to use this you have to download 2 packages html-xml-utils and w3m
# additional you need an alien token
# and you have to enter the path to your certificat at line ~120

# Use: bash DownScript.sh [TrainPage] [Trainnumber] -[OPTIONS]

# actually all inputs are also asked interactive in case you forget something

# 	|-> TrainPage (e.g. GA_pp_AOD)
# 	|-> TrainNumber (e.g 768)
#                            YOU CAN SELECT multiple trains at once e.g. "768 775"



############### global variables
debug=0
ErrorLog=""
WARNINGLog=""
pathtocert=""
UserName=""
alienUserName=""
BASEDIR=${PWD}
thisuser=`echo ${USER}`

#There are Settings to be used beforehand
TrainPage=""
TrainPageNum=""
TrainNumber=""
MergeTrains=0
MergeTrainsRuns=0
MergeTrainsPeriods=0
DoDown=1
DoSoftDown=0
MergeTrainsOutname=""
OutName=""
Search=".root"
OptRunlistName=list
OptRunlistNameSet=0
OptAllRunlists=0
UseMerge=0
MergeTrains=0
MultiTrains=0
Userunwise=0
UseOnlyrunwise=0
SetOutName=""
Usechildsareperiods=0
newfiles=0
re='^[0-9]+$'

RunningScripts=0
OptRunlistNamefile=OptRunlistNames_$RunningScripts.txt

job=$$
lockfile=$job.lock

OutName=""
MergeTrainsOutname=""
AlienDir=""

OUTPUTDIR=""
List=""
GlobalVariablesFile=""
envFile=""
TrainPageHTML=""
trainwebpage=""
PeriodName=""

childID=""
foundperiod=0
foundchild=0
NumberOfRuns=0
lookchildID=0
foundRunlists=0
readRuns=0
lastRun=0
RunlistID=0
ChildName=""
Runlist=""
RunlistOnTrainpage=""

LIST_child=()
LIST_RunlistName=()
LIST_RunlistID=()
LIST_foundRunlists=()
LIST_RunlistOnTrainpage=()

Type=""
Year=""
useSpecificRunlist=0
SearchSpecificRunlist=""
RunlistName=""
FileList=""

FilenamesinTrain=""
AvailibleFiles=""
MissingFiles=""

Dirout=""
Dirin="0"
DirMerged=""


######################### functions
function new_rm(){
	if [[ -f $1 ]]
	then
		rm $1
	fi
}
function usecmd(){
	if [[ $debug = 1 ]] || [[ $debug = 2 ]]
	then
		echo $@ | tee -a $LogFile
	fi
}
function GetWebpage(){
	if [[ $debug = 1 ]] || [[ $debug = 2 ]]
	then
		echo -e "\e[33mGetWebpage(): \e[0m\t$1\t$2\t$3" | tee -a $LogFile
	fi
	certpath="$3"
	Webpage="$2"

	# we download the html to grep needed information from there
	if [ -f $1 ]
	then # Check if older than one day (or not)
		# cat $1 | wc -l
		if [[ `cat $1 | wc -l` -lt 100 ]]
		then
			new_rm $1
			echo -e "\e[33m|-> \e[0mDownloading new one: ${Webpage}" | tee -a $LogFile
			cmd="curl '${Webpage}' --key $certpath/$key -k --cert $certpath/$clientca &> $1"
			eval $cmd
			usecmd $cmd
			touch "$1";
		else
			# echo -e "\e[33m|-> \e[0mFile $1 exists." | tee -a $LogFile
		# fi
			if [ "$(( $(date +"%s") - $(stat -c "%Y" $1) ))" -gt "604800" ]  # 86400 = 1 Day
			then
				echo -e "\e[33m|-> \e[0mFile $1 exists. \e[33m|-> \e[0mbut is older then 7 Days! download new one from alien:/$1" | tee -a $LogFile
				# cmd="curl -s '${Webpage}' --key $certpath/$key --cacert $certpath/$cacert --cert $certpath/$clientca &> $1"
				cmd="curl '${Webpage}' --key $certpath/$key -k --cert $certpath/$clientca &> $1"
				eval $cmd
				usecmd $cmd
				touch "$1";
			else
				echo -e "\e[33m|-> \e[0mFile $1 exists. \e[33m|-> \e[0mand is up-to-date" | tee -a $LogFile
			fi
		fi
		# echo -e "\e[33m|-> \e[0mFile $1 exists." | tee -a $LogFile
	else
		echo -e "\e[33m|-> \e[0mDownloading ${Webpage}" | tee -a $LogFile
		# cmd="curl '${Webpage}' --key $certpath/$key --cacert $certpath/$cacert --cert $certpath/$clientca &> $1"
		cmd="curl '${Webpage}' --key $certpath/$key -k --cert $certpath/$clientca &> $1"
		usecmd $cmd
		eval $cmd
		touch "$1";
	fi
}
# # if this is not working try
# openssl pkcs12 -in myCertificate.p12 -out newcert.pem
# openssl pkcs12 -in myCertificate.p12 -out $cacert -cacerts -nokeys
# openssl pkcs12 -in myCertificate.p12 -out $clientca -clcerts -nokeys
# openssl pkcs12 -in myCertificate.p12 -out $key -nocerts

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
		new_rm $1
	fi
	if [[ $debug = 1 ]] || [[ $debug = 2 ]]
	then
		echo "cat $2" | tee -a $LogFile
		cat $2 | tee -a $LogFile
	fi
}

function Init(){

	#####################################


	if [[ $thisuser = "adrian" || $thisuser = "amechler" ]]
	then
		BASEDIR="/media/adrian/Adrian/grid_data"
		FrameworkDir="/home/adrian/git/AnalysisSoftware"
		UserName="Adrian Mechler";
		pathtocert="~/.globus"
		alienUserName="amechler"
		key="key.pem"
		cacert="ca.pem"
		clientca="client.pem"
	else
		echo | tee -a $LogFile
		echo -e "This feature is not supported for user:$thisuser." | tee -a $LogFile
		echo -e "You have to add your certificat path to folder." | tee -a $LogFile
		echo | tee -a $LogFile
		exit
	fi
	#####################################
	#####################################
	echo;echo;
	echo  -e "\e[36m------------------------------------\e[0m" | tee -a $LogFile
	echo -e "Using settings for: $UserName" | tee -a $LogFile
	echo  -e "\e[36m------------------------------------\e[0m" | tee -a $LogFile


	# chack if valid token is active
	if [[ `alien-token-info` =~ "No Token found!" ]]
	then
		echo -e "\e[31mWARNING:\e[0m No alien token" | tee -a $LogFile
		# alien-token-init $alienUserName
		echo;echo;
		alien-token-init $alienUserName
		echo  -e "\e[36m------------------------------------\e[0m" | tee -a $LogFile
		echo;echo;
	fi

	while [[ -f OptRunlistNames_$RunningScripts.txt ]]; do
		((RunningScripts++))
	done
	OptRunlistNamefile=OptRunlistNames_$RunningScripts.txt
	# useSpecificRunlistfile=useSpecificRunlist_$RunningScripts.txt
	TrainNumberFile=TrainNumberFile_$RunningScripts.txt
	Searchfile=Searchfile_$RunningScripts.txt

	for Process in `ls *.lock | grep -v $job` ; do
		tmpjob=${Process%.lock}
		status=`ps -efww | grep -w "DownScript.sh" | grep -v grep | grep $tmpjob | awk '{ print $2 }'`
		if [ -z "$status" ]; then
			for tmp in `cat $Process` ; do
				if [[ -f OptRunlistNames_$tmp.txt ]]; then new_rm OptRunlistNames_$tmp.txt; fi
				if [[ -f TrainNumberFile_$tmp.txt ]]; then new_rm TrainNumberFile_$tmp.txt; fi
				if [[ -f Searchfile_$tmp.txt ]]; then new_rm Searchfile_$tmp.txt; fi
			done
			new_rm $Process
		fi
	done
	echo "$RunningScripts" > $lockfile | tee -a $LogFile

	new_rm $TrainNumberFile
	new_rm $Searchfile
	new_rm $OptRunlistNamefile

	LogFile="$BASEDIR/Log_$tmp.txt"
	new_rm $LogFile
}
function Finish(){
	if [[ $MultiTrains = 0 ]]; then
		if [[ ! $OutName = $SetOutName ]]; then
			mkdir -p $BASEDIR/$SetOutName
			cmd="ln -sf $BASEDIR/$OutName/* $BASEDIR/$SetOutName/."
			eval $cmd
			# echo "002 $cmd" | tee -a $LogFile
			usecmd $cmd
		fi
	fi

	mv $LogFile $BASEDIR/$SetOutName/Log.txt


	new_rm $TrainNumberFile
	new_rm $Searchfile
	new_rm $OptRunlistNamefile


	echo;echo;echo;echo;
	echo  -e "\e[36m------------------------------------\e[0m" | tee -a $LogFile
	for setting in "$@"
	do
		printf "${setting#-} " | tee -a $LogFile
	done
	echo;
	if [[ -f $ErrorLog ]]; then
		cat $ErrorLog | tee -a $LogFile
	else
		echo  -e "\tfinished without errors" | tee -a $LogFile
	fi
	if [[ -f $WARNINGLog ]]; then
		cat $WARNINGLog | tee -a $LogFile
	else
		echo  -e "\tfinished without warnings" | tee -a $LogFile
	fi
	echo  -e "\e[36m------------------------------------\e[0m" | tee -a $LogFile

	echo;echo;echo;

	for setting in "$@"
	do
		printf "${setting#-} \n" >> ${PWD}/Error.log
		# echo $setting | tee -a $LogFile
		if [[ $setting = "-totalLog" ]]
		then
			echo " " >> ${PWD}/Error.log
			echo  -e "\e[36m------------------------------------\e[0m" >> ${PWD}/Error.log
			# echo "$@" >> ${PWD}/Error.log | tee -a $LogFile
			echo  >> ${PWD}/Error.log
			if [[ -f $ErrorLog ]]; then cat $ErrorLog >> ${PWD}/Error.log; fi
			if [[ -f $WARNINGLog ]]; then cat $WARNINGLog >> ${PWD}/Error.log; fi
			echo  -e "\e[36m------------------------------------\e[0m" >> ${PWD}/Error.log
			echo  >> ${PWD}/Error.log
			echo  >> ${PWD}/Error.log
		fi
	done
	echo;
	echo;
	new_rm $lockfile
}
function InitilaizeChild(){
	child=$1
	echo
	echo  -e "\e[35m=====================================\e[0m" | tee -a $LogFile
	echo
	echo -e "\e[35m $child \e[0m" | tee -a $LogFile

	childID=${child##*child_}

	foundperiod=0
	foundchild=0
	NumberOfRuns=0
	lookchildID=0
	foundRunlists=0
	readRuns=0
	lastRun=0
	RunlistID=0
	ChildName=""
	Runlist=""
	RunlistOnTrainpage=""
	unset LIST_RunlistName
	unset LIST_RunlistID
	unset LIST_foundRunlists
	unset LIST_RunlistOnTrainpage

	# prepare directory
	mkdir -p "$OUTPUTDIR/.$child"

	# get one globalvariables.C to get information about the child
	GlobalVariablesPath="$OUTPUTDIR/.$child/$GlobalVariablesFile"
	if [[ $debug = 1 ]] || [[ $debug = 2 ]]
	then
		GetFile "$AlienDir$child/$GlobalVariablesFile" "$GlobalVariablesPath" $GlobalVariablesPath.downlog
	else
		GetFile "$AlienDir$child/$GlobalVariablesFile" "$GlobalVariablesPath" $GlobalVariablesPath.downlog #&> /dev/null
	fi

	ChildName=`grep "periodName =" "$GlobalVariablesPath" | awk -F "= " '{print $2}' | awk -F ";" '{print $1}' | awk -F "\"" '{print $2}'`
	if [[ $ChildName = "" ]]; then ChildName=`grep "periodName =" "$GlobalVariablesPath" | awk -F "=" '{print $2}' | awk -F ";" '{print $1}' | awk -F "\"" '{print $2}'`; fi
	if [[ $ChildName = "" ]]; then ChildName=`grep "periodName=" "$GlobalVariablesPath" | awk -F "=" '{print $2}' | awk -F ";" '{print $1}' | awk -F "\"" '{print $2}'`; fi
	echo -e " periodName = $ChildName" | tee -a $LogFile

	if [[ $Usechildsareperiods = 1 ]]; then
		ChildName=`grep "export ALIEN_JDL_child_${childID}_LPMPRODUCTIONTAG" $envFile | cut -d "'" -f 2`
		# ChildName=`echo $(head -n 1 $PathtoRuns) | awk -F "/" '{print $7}' ` | tee -a $LogFile
		echo -e "\e[33m|->\e[0m Changed periodName = $ChildName" | tee -a $LogFile
	fi


}
function InitilaizeTrain(){
	TrainNumber=$1
	OutName=.$TrainPage-$TrainNumber
	ErrorLog="$BASEDIR/$OutName/Errors.log"
	WARNINGLog="$BASEDIR/$OutName/Warnings.log"

	new_rm $ErrorLog
	new_rm $WARNINGLog


	OUTPUTDIR=$BASEDIR/$OutName
	List="ListGrid.txt"
	GlobalVariablesFile="globalvariables.C"
	envFile="$OUTPUTDIR/.env.sh"
	FilenamesinTrain=$OUTPUTDIR/.FilenamesinTrain.txt

	TrainPageHTML="$BASEDIR/.$TrainPage.html"

	trainwebpage="https://alimonitor.cern.ch/trains/train.jsp?train_id=${TrainPageNum}"
	PeriodName=""

	echo;echo;echo;echo;
	echo  -e "\e[36m~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\e[0m" | tee -a $LogFile
	echo -e "\e[36m |-> \e[0m OutName = $OutName\e[36m | \e[0mTrainNumber = $TrainNumber\e[36m | \e[0mTrainPage = $TrainPage\e[36m | \e[0mSearch = $Search\e[36m | \e[0mRunlistName = $OptRunlistName\e[36m | \e[0mUsechildsareperiods = $Usechildsareperiods\e[36m | \e[0mUseMerge = $UseMerge" | tee -a $LogFile
	echo  -e "\e[36m~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\e[0m" | tee -a $LogFile
	echo;echo;

	# prepare directory
	mkdir -p $OUTPUTDIR
	echo
	GetWebpage $TrainPageHTML $trainwebpage $pathtocert
	echo
	echo  -e "\e[36m------------------------------------\e[0m" | tee -a $LogFile
	echo

	if [[ `wc -l $TrainPageHTML` < 1 ]]
	then
		echo -e "\e[31mError\e[0m $TrainPageHTML not found! aborting" | tee -a $ErrorLog | tee -a $LogFile
		return 1
	fi

	Childeone=""
	while [[ $Childeone = "" ]]; do
		# get all childs involved in train run
		GetChilds $List
		if [[ $Usechildsareperiods = 1 ]]
		then
			# get one env.sh to get basic information about the train
			Childeone=`sed '2q;d' $List`
		else
			# get one env.sh to get basic information about the train
			Childeone=`sed '1q;d' $List`
		fi
		printf "."
	done
	# cat $List
	echo -e "\e[33m|->\e[0m $Childeone" | tee -a $LogFile
	if [[ $debug = 1 ]] || [[ $debug = 2 ]]
	then
		GetFile $AlienDir$Childeone/env.sh $envFile $envFile.downlog
	else
		GetFile $AlienDir$Childeone/env.sh $envFile $envFile.downlog &> /dev/null
	fi
	PERIODNAME=`grep "PERIOD_NAME=" $envFile | awk -F "=" '{print $2}' | awk -F ";" '{print $1}'`
	echo "PERIODNAME = $PERIODNAME" | tee -a $LogFile
	NCHilds=`grep "CHILD_DATASETS=" $envFile | awk -F "=" '{print $2}' | awk -F ";" '{print $1}'`
	echo "NCHilds = $NCHilds" | tee -a $LogFile
	echo

	lego_train_mergePath="$OUTPUTDIR/.lego_train_merge.C"
	if [[ $debug = 1 ]] || [[ $debug = 2 ]]
	then
		GetFile $AlienDir$Childeone/lego_train_merge.C "$lego_train_mergePath" $lego_train_mergePath.downlog
	else
		GetFile $AlienDir$Childeone/lego_train_merge.C "$lego_train_mergePath" $lego_train_mergePath.downlog &> /dev/null
	fi
	grep "TString outputFiles" "$lego_train_mergePath"  | awk -F "\"" '{print $2}' > $FilenamesinTrain.tmp
	sed -i 's/,/\n/g' $FilenamesinTrain.tmp
	sed -i 's/ //g' $FilenamesinTrain.tmp
	new_rm $FilenamesinTrain.tmp2

	for Search in `cat $Searchfile`
	do
		grep $Search $FilenamesinTrain.tmp >> $FilenamesinTrain.tmp2
	done
	# AddToList $FilenamesinTrain.tmp2 $FilenamesinTrain
	mv $FilenamesinTrain.tmp2 $FilenamesinTrain
	new_rm $FilenamesinTrain.tmp
	new_rm $FilenamesinTrain.tmp2

	# Loop over all childs
	for child in `sort ${List}`;
	do
		LIST_child+=($child)
	done

}
function InitilaizeRunlist(){
	arrypos=$1
	Type=""
	Year=""
	useSpecificRunlist=0
	SearchSpecificRunlist=""
	# RunlistName=""
	FileList=""
	NumberOfRuns=0
	# RunlistName=${LIST_RunlistName[$arrypos]}
	RunlistID=${LIST_RunlistID[$arrypos]}
	foundRunlists=${LIST_foundRunlists[$arrypos]}
	RunlistOnTrainpage=${LIST_RunlistOnTrainpage[$arrypos]}
	((arrypos++))
	echo;
	echo  -e "\e[36m------------------------------------\e[0m" | tee -a $LogFile
	echo -e "\e[36m|-> Runlist $RunlistID\tName = $RunlistName \e[0m" | tee -a $LogFile
	if [[ ! "$OptRunlistName" = "useSpecificRunlist_"* ]]; then
		Runlist="$RunlistOnTrainpage"
	fi
	for runName in `cat $Runlist`
	do
		printf "$runName, " | tee -a $LogFile
		((NumberOfRuns++))
	done
	printf "\n $NumberOfRuns Runs found \n" | tee -a $LogFile
	Dirout=$OUTPUTDIR/$RunlistName/$ChildName
	Dirin=$AlienDir$child/merge_runlist_$RunlistID
	DirMerged=$OUTPUTDIR/$RunlistName
	mkdir -p $DirMerged
	mkdir -p $Dirout
	if [[ $NumberOfRuns = 0 ]]; then
		echo  -e "\e[33mWARNING:  No runs in Runlist\e[0m: ChildName = $ChildName, childID = $childID, Runlist $RunlistID, Name = $RunlistName" | tee -a $WARNINGLog | tee -a $LogFile
		return 1
	fi
	Type='data'
	if (( `echo -n ${ChildName#LHC*} | wc -c` > 3 )); then
		Type='sim'
	fi
	Year="20${ChildName:3:2}"
	if [[ "$OptRunlistName" = "useSpecificRunlist_"* ]]; then
		if [[ ! $RunlistID = "1" ]]; then return 1; fi
		useSpecificRunlist=1
		SearchSpecificRunlist=${OptRunlistName#useSpecificRunlist_}
		Runlist="${FrameworkDir}/DownloadAndDataPrep/runlists/runNumbers${ChildName}${SearchSpecificRunlist}.txt"
		if [[ ! -f $Runlist ]] && [[ ! $Type = "data" ]]; then
				AnchorChildName=`grep "export ALIEN_JDL_child_${childID}_LPMANCHORPRODUCTION" $envFile | cut -d "'" -f 2`
				justtocopythis="${FrameworkDir}/DownloadAndDataPrep/runlists/runNumbers${AnchorChildName}${SearchSpecificRunlist}.txt"
				if [[ -f $justtocopythis ]]; then
					cp $justtocopythis $Runlist
				fi
				Runlist=$justtocopythis
		fi

		mkdir -p "${BASEDIR}/Runlists"
		mkdir -p "${BASEDIR}/Runlists/${TrainPage}/"
		mkdir -p "${BASEDIR}/Runlists/${TrainPage}/${PERIODNAME}"
		if [[ $Type = "data" ]]; then
			cp $Runlist $BASEDIR/Runlists/${TrainPage}/${PERIODNAME}/child${childID}_${ChildName}-list${SearchSpecificRunlist}
		else
			cp $Runlist $BASEDIR/Runlists/${TrainPage}/${PERIODNAME}/child${childID}_${ChildName}-anch${AnchorChildName}-list${SearchSpecificRunlist}
		fi

		RunlistName="$SearchSpecificRunlist"

		echo -e "\e[36m|-> useing specific runlist ${Runlist#*runlists/} \e[0m" | tee -a $LogFile
		if [[ ! -f $Runlist ]]; then
			echo  -e "\e[33mWARNING: ${Runlist} not found \e[0m" | tee -a $WARNINGLog | tee -a $LogFile
			return 1
		fi
		if [[ -f $Runlist ]]; then
			for run in `cat $Runlist`; do
				printf "$run, " | tee -a $LogFile
			done
		fi
		echo;
	fi


	# prepare directory
	mkdir -p "$OUTPUTDIR/.$child"
	mkdir -p "$OUTPUTDIR/.$child/$RunlistName"

	FileList="$OUTPUTDIR/.$child/$RunlistName/.FileList.txt"

	# look for availible Files
	if [[ ! $useSpecificRunlist = 1 ]]; then
		if [[ $newfiles = 0 ]];then
			if [[ ! -f $FileList ]]; then
				newfiles=1
			elif [[  `grep "$Search" $FileList | wc -l` < 1 ]]; then
				newfiles=1
			fi
		fi
		if [[ $newfiles = 1 ]] || [ ! -f $FileList ]; then
			for Search in `cat $FilenamesinTrain`
			do
				cmd="alien_ls $AlienDir$child/merge_runlist_$RunlistID/ 2> /dev/null | grep "$Search" > $FileList.tmp"
				eval $cmd
				usecmd $cmd
				AddToList $FileList.tmp $FileList
			done
		fi
	fi

	mkdir -p $OUTPUTDIR/$RunlistName
	mkdir -p $OUTPUTDIR/$RunlistName/$ChildName
	return 0
}
function ParseSettings(){

	for setting in "$@"
	do
		# echo $setting | tee -a $LogFile
		if [[ $setting = "GA_"* ]]
		then
			TrainPage="$setting"
		elif [[ $setting =~ $re ]]
		then
			TrainNumbertmp=${setting}
			echo $TrainNumbertmp >> $TrainNumberFile
			if [[ $TrainNumber = "" ]]
			then
				TrainNumber=$TrainNumbertmp
				MultiTrains=0
			else
				TrainNumber=$TrainNumber'+'$TrainNumbertmp
				MultiTrains=1
			fi
		elif [[ $setting = "-Name_"* ]]
		then
			SetOutName=${setting#*-Name_}
		elif [[ $setting = "?_"* ]]
		then
			Searchtmp=${setting#*\?_}
			echo $Searchtmp >> $Searchfile
			if [[ $Search = ".root" ]]
			then
				Search=*$Searchtmp*
			else
				Search=$Search'|'*$Searchtmp*
			fi
		elif [[ $setting = "-RL_"* ]]
		then
			OptRunlistNametmp=${setting#*-RL_}
			echo $OptRunlistNametmp >> $OptRunlistNamefile
			if [[ $OptRunlistName = "list" ]]
			then
				OptRunlistName=*$OptRunlistNametmp*
			else
				OptRunlistName=$OptRunlistName'|'*$OptRunlistNametmp*
			fi
		elif [[ $setting = "-mergechilds" ]]
		then
			UseMerge=1
		elif [[ $setting = "-mergeall" ]]
		then
			MergeTrains=1
			MergeTrainsRuns=1
			MergeTrainsPeriods=1
		elif [[ $setting = "-mergetrains" ]]
		then
			MergeTrains=1
		elif [[ $setting = "-mergeruns" ]]
		then
			MergeTrainsRuns=1
		elif [[ $setting = "-mergeperiods" ]]
		then
			MergeTrainsPeriods=1
		elif [[ $setting = "-runwise" ]]
		then
			Userunwise=1
		elif [[ $setting = "-onlyrunwise" ]]
		then
			Userunwise=1
			UseOnlyrunwise=1
		elif [[ $setting = "-childsareperiods" ]]
		then
			Usechildsareperiods=1
		elif [[ $setting = "-uSR_"* ]]
		then
			useSpecificRunlist=1
			SearchSpecificRunlist=${setting#*-uSR_}
			OptRunlistNametmp="useSpecificRunlist_${SearchSpecificRunlist}"
			echo "$OptRunlistNametmp" | tee -a $OptRunlistNamefile #$useSpecificRunlistfile | tee -a $LogFile
			if [[ $OptRunlistName = "list" ]]
			then
				OptRunlistName=*$OptRunlistNametmp*
			else
				OptRunlistName=$OptRunlistName'|'*$OptRunlistNametmp*
			fi
		elif [[ $setting = "-useSpecificRunlist_"* ]]
		then

			SearchSpecificRunlist=${setting#*-useSpecificRunlist_}
			OptRunlistNametmp="useSpecificRunlist_${SearchSpecificRunlist}"
			echo "$OptRunlistNametmp" | tee -a $OptRunlistNamefile #$useSpecificRunlistfile | tee -a $LogFile
			if [[ $OptRunlistName = "list" ]]
			then
				OptRunlistName=*$OptRunlistNametmp*
			else
				OptRunlistName=$OptRunlistName'|'*$OptRunlistNametmp*
			fi
		elif [[ $setting = "-noDown" ]]
		then
			DoDown=0
		elif [[ $setting = "-debug" ]]
		then
			debug=1
		elif [[ $setting = "-debugmore" ]]
		then
			debug=2
		elif [[ $setting = "-newfiles" ]]
		then
			newfiles=1
		fi
	done

	# cat $useSpecificRunlistfile >> $OptRunlistNamefile

	if [[ ! -e $OptRunlistNamefile ]]
	then
		OptAllRunlists=1
		# echo "USING all runlists $OptAllRunlists" | tee -a $LogFile
	fi
	################
	# Required Settings can also be Set interactive
	if [[ $TrainPage = "" ]]
	then
		echo -e "\e[33m|-> \e[0m TrainPage not set! What shall be used? GA_pp, GA_pp_MC, GA_pp_AOD, GA_pp_MC_AOD" | tee -a $LogFile
		read TrainPage
	fi
	if [[ $TrainPage = "GA_pp" ]]
	then
		TrainPageNum="23"
	elif [[ $TrainPage = "GA_pp_MC" ]]
	then
		TrainPageNum="43"
	elif [[ $TrainPage = "GA_pp_AOD" ]]
	then
		TrainPageNum="62"
	elif [[ $TrainPage = "GA_pp_MC_AOD" ]]
	then
		TrainPageNum="66"
	elif [[ $TrainPage = "GA_PbPb_MC_AOD" ]]
	then
		TrainPageNum="65"
	else
		echo -e "\e[31m|-> $TrainPage\e[0m not supported" | tee -a $LogFile
	fi
	if [[ $TrainNumber = "" ]]
	then
		echo -e "\e[33m|-> \e[0m TrainNumber not set! What shall be used? " | tee -a $LogFile
		read TrainNumber
	fi

	##################
	# Print Settings
	OutName=.$TrainPage-$TrainNumber
	AlienDir="/alice/cern.ch/user/a/alitrain/PWGGA/$TrainPage/"
	MergeTrainsOutname="${OutName}"
	if [[ $SetOutName = "" ]]
	then
		# SetOutName=$OutName
		# echo -e "\e[33m|-> \e[0m OutName not set! using TrainNumber: OutName = $OutName" | tee -a $LogFile
		echo -e "\e[33m|-> \e[0m OutName = $OutName" | tee -a $LogFile
	else
		echo -e "\e[33m|-> \e[0m OutName = $SetOutName" | tee -a $LogFile
	fi
	if [[ $MultiTrains = 0 ]]
	then
		echo -e "\e[33m|-> \e[0m TrainNumber = $TrainNumber" | tee -a $LogFile
	elif [[ $MultiTrains = 1 ]]
	then
		echo -e "\e[33m|-> \e[0m TrainNumber set: $TrainNumber" | tee -a $LogFile
		MergeTrainsOutname="${SetOutName}"
	fi
	echo -e "\e[33m|-> \e[0m TrainPage = $TrainPage" | tee -a $LogFile
	echo -e "\e[33m|-> \e[0m Search = $Search" | tee -a $LogFile
	echo -e "\e[33m|-> \e[0m RunlistName = $OptRunlistName" | tee -a $LogFile
	# echo -e "\e[33m|-> \e[0m Usechildsareperiods = $Usechildsareperiods" | tee -a $LogFile
	if [[ $DoDown = 0 ]]
	then
		echo -e "\e[33m|-> \e[0m Downloading disabled" | tee -a $LogFile
	else
		if [[ $UseMerge = 1 ]]
		then
			echo -e "\e[33m|-> \e[0m Merge files" | tee -a $LogFile
		fi
		if [[ $Usechildsareperiods = 1 ]]
		then
			echo -e "\e[33m|-> \e[0m Childs are periods" | tee -a $LogFile
		fi
	fi
	echo  -e "\e[36m------------------------------------\e[0m" | tee -a $LogFile
	# echo -e "\e[33mIs this Setup correct?\e[0m (y/n)" | tee -a $LogFile
	# printf "Answer: " | tee -a $LogFile
	# read -e setupcorrect
	# if [ ! "$setupcorrect" = "y" ]
	# then
	#   echo "" | tee -a $LogFile
	#   echo "please try again   :-(" | tee -a $LogFile
	#   echo "" | tee -a $LogFile
	#   exit
	# fi
	# echo  -e "\e[36m------------------------------------\e[0m" | tee -a $LogFile
	echo "" | tee -a $LogFile
	echo "" | tee -a $LogFile
	echo "" | tee -a $LogFile


	if [ $newfiles = 1 ]; then
		printf "removing existing fileslists" | tee -a $LogFile
		for TrainNumber in `cat $TrainNumberFile`
		do
			for file in `find  $BASEDIR/.$TrainPage-$TrainNumber/ -name FileList.txt`; do new_rm $file; done
		done
		printf " done \n" | tee -a $LogFile
	fi

}
function Parsehtml(){
	printf "\e[33m|-> \e[0m start Parsehtml...\n" | tee -a $LogFile
	# parse html to get infos
	for line in `cat $TrainPageHTML`
	do
		if [[ $line = *"editPeriod"* ]]
		then
			if [[ $found = 1 ]]
			then
				found=0
			fi
			if [[ $line = *"$PERIODNAME"* ]]
			then
				found=1
				if [[ $debug = 2 ]]
				then
					echo -e "\e[33m|-> \e[0m found period" | tee -a $LogFile
				fi
			fi
		fi
		if [[ $found = 1 ]]
		then
			if [[ $lookchildID = 1 ]]
			then
				if [[ $debug = 2 ]]
				then
					echo -e "\e[33m|-> \e[0m lookchildID $line" | tee -a $LogFile
				fi
				foundchild=0
				if [[ $line = "$childID" ]]
				then
					if [[ $debug = 2 ]]
					then
						if [[ $Usechildsareperiods = 0 ]]
						then
							echo -e "\e[33m|->\e[0m ChildName = $ChildName" | tee -a $LogFile
						else
							echo -e "\e[33m|->\e[0m will load child/period name later" | tee -a $LogFile
						fi
						echo -e "\e[33m|->\e[0m childID = $childID" | tee -a $LogFile
					fi
					foundchild=1
				fi
				lookchildID=0
			fi
			if [[ $line = *"child"* ]]
			then
				lookchildID=1
			fi
			if [[ $line = *"</tr>"* ]]
			then
				foundchild=0
			fi
		fi
		if [[ $foundchild = 1 ]]
		then
			if [[ $debug = 2 ]]
			then
				echo -e "\e[33m|-> \e[0m found child: $line" | tee -a $LogFile
			fi
			if [[ $readRuns = 1 ]]
			then
				if [[ $debug = 2 ]]; then
					printf "($line)" | tee -a $LogFile
				fi
				if [[ $line = *"<"* ]]
				then
					readRuns=0
					if [[ $line = *"<br>"* ]] && [[ ! $line = "<br>" ]]; then
						readrun=${line%<*}

						if [[ $readrun = *","* ]]; then
							readrun2=`echo $readrun | cut -d "," -f 1` | tee -a $LogFile
							readrun=`echo $readrun | cut -d "," -f 2` | tee -a $LogFile
							# printf "$readrun2, " | tee -a $LogFile
							printf "$readrun2\n" >> $RunlistOnTrainpage
							((NumberOfRuns++))
						fi
						# printf "$readrun" | tee -a $LogFile
						printf "$readrun\n" >> $RunlistOnTrainpage
						((NumberOfRuns++))
					fi
					# printf "\n $NumberOfRuns Runs found \n" | tee -a $LogFile
					lastRun=1

				else
					readrun=${line%,}
					if [[ $readrun = *","* ]]; then
						readrun2=`echo $readrun | cut -d "," -f 1` | tee -a $LogFile
						readrun=`echo $readrun | cut -d "," -f 2` | tee -a $LogFile
						# printf "$readrun2, " | tee -a $LogFile
						printf "$readrun2\n" >> $RunlistOnTrainpage
						((NumberOfRuns++))
					fi
					# printf "$readrun, " | tee -a $LogFile
					printf "$readrun\n" >> $RunlistOnTrainpage
					((NumberOfRuns++))
				fi
			fi

			if [[ $foundRunlists = 1 ]]
			then
				RunlistName=${line%%:}
				RunlistID=$(( $RunlistID +1))
				foundRunlists=0
				readRuns=1
				NumberOfRuns=0

				mkdir -p "$FrameworkDir/DownloadAndDataPrep/runlistsOnTrainpage"
				RunlistOnTrainpage="$FrameworkDir/DownloadAndDataPrep/runlistsOnTrainpage/runNumbers$ChildName-$RunlistName.txt"

				# echo;
				# echo  -e "\e[36m------------------------------------\e[0m" | tee -a $LogFile
				# echo -e "\e[36m|-> Runlist $RunlistID\tName = $RunlistName \e[0m" | tee -a $LogFile

				new_rm $RunlistOnTrainpage
				touch $RunlistOnTrainpage
			fi

			if [[ $lastRun = 1 ]]
			then
				lastRun=0
				if [[ $OptAllRunlists -eq 1 ]]
				then
					# echo "########" | tee -a $LogFile
					echo "$RunlistName" > $OptRunlistNamefile | tee -a $LogFile
				fi

				foundRunlistNamefile=$OUTPUTDIR/.${child}/FoundRunlistNames.txt
				AllRunlistNames=$BASEDIR/AllRunlistNames.txt
				if [[ -f $foundRunlistNamefile ]]; then
					if [[ `grep "|$ChildName|$childID|$RunlistName|" $foundRunlistNamefile | wc -l` < 1 ]]; then
						echo "|$ChildName|$childID|$RunlistName|$RunlistID|" >> $foundRunlistNamefile
					else
						if [[ ! `grep "|$ChildName|$childID|$RunlistName|" $foundRunlistNamefile | cut -d "|" -f 5` =  "$RunlistID" ]]; then
							printf "\e[33mWARNING: RunList found twice\e[0m \t" >> $WARNINGLog
							printf "|$ChildName|$childID|$RunlistName|$RunlistID| = " >> $WARNINGLog
							grep "|$ChildName|$childID|$RunlistName|" $foundRunlistNamefile >> $WARNINGLog
						fi
					fi
				else
					echo "|$RunlistName|" >> $foundRunlistNamefile
				fi
				if [[ -f $AllRunlistNames ]]; then
					if [[ `grep $RunlistName $AllRunlistNames | wc -l` < 1 ]]; then
						echo "$RunlistName" >> $AllRunlistNames
					fi
				else
					echo "$RunlistName" >> $AllRunlistNames
				fi

				AddToList $foundRunlistNamefile.tmp $foundRunlistNamefile
				useSpecificRunlist=0
				# printf "$RunlistName: " | tee -a $LogFile
				for Searchedrunlist in `cat $OptRunlistNamefile`
				do
					if [[ "$RunlistName" = "$Searchedrunlist" ]] || [[ "$OptRunlistName" = "useSpecificRunlist_"* ]]
					then
						LIST_RunlistName+=($RunlistName)
						LIST_RunlistID+=($RunlistID)
						LIST_foundRunlists+=($foundRunlists)
						LIST_RunlistOnTrainpage+=($RunlistOnTrainpage)
						printf "$Searchedrunlist found \n" | tee -a $LogFile
						# break
					fi
				done
			fi
			if [[ $line = *"Runlist"* ]]
			then
				foundRunlists=1
			fi
			if [[ $line = *"table_row"* ]]
			then
				RunlistID=0
			fi
		fi
	done
	printf "\t\t\t done \n" | tee -a $LogFile
}
function doMergeTrainsPeriods(){
	echo  -e "\e[36m------------------------------------\e[0m" | tee -a $LogFile
	echo "Start merging periods from trains: ${BASEDIR}/${MergeTrainsOutname}" | tee -a $LogFile
	echo  -e "\e[36m------------------------------------\e[0m" | tee -a $LogFile
	for TrainNumber in `cat $TrainNumberFile`
	do
		echo;echo;
		echo  -e "\e[36m|-> \e[0m $TrainNumber" | tee -a $LogFile
		singletrainDir=.$TrainPage-$TrainNumber
		for RunlistName in `cat $OptRunlistNamefile`
		do
			echo  -e "\t\e[36m|-> \e[0m $RunlistName" | tee -a $LogFile
			mkdir -p ${BASEDIR}/${MergeTrainsOutname}
			mkdir -p ${BASEDIR}/${MergeTrainsOutname}/$RunlistName

			# look for availible periods
			periodList="${BASEDIR}/${MergeTrainsOutname}/$RunlistName/.periodList.txt"
			if [[ -f $periodList ]]; then new_rm $periodList; fi
			cmd="ls $BASEDIR/$singletrainDir/$RunlistName/ | grep 'LHC' >>  $periodList"
			eval $cmd
			usecmd $cmd

			for periodnametmp in `cat $periodList`
			do
				Periodname2=${periodnametmp#*$RunlistName/}
				Periodname=${Periodname2%/*}
				PeriodnameWOextra=${Periodname2%_extra*}
				echo  -e "\t\t\e[36m|-> \e[0m $Periodname" | tee -a $LogFile
				mkdir -p ${BASEDIR}/${MergeTrainsOutname}/$RunlistName/$PeriodnameWOextra

				# look for availible Files
				FileList="${BASEDIR}/${MergeTrainsOutname}/$RunlistName/.FileList.txt"
				if [ -f $FileList ]; then new_rm $FileList; fi
				for Search in `cat $Searchfile`
				do
					cmd="ls $BASEDIR/$singletrainDir/$RunlistName/$Periodname/ | grep "$Search" | grep ".root" >> $FileList"
					eval $cmd
					usecmd $cmd
				done
				for filenametmp in `cat $FileList`
				do
					filename=${filenametmp#*/$RunlistName/*/}
					printf "\t\t\t\e[33m|-> \e[0m $filename" | tee -a $LogFile
					Dirout=$BASEDIR/$singletrainDir/$RunlistName/$Periodname/
					Dirin="0"
					DirMerged=${BASEDIR}/${MergeTrainsOutname}/$RunlistName/$PeriodnameWOextra/
					doMergeFiles $filename $Dirout $Dirin $DirMerged "Periods"
				done
			done
			# fi
		done
	done
}
function doMergeTrainsRuns(){
	echo  -e "\e[36m------------------------------------\e[0m" | tee -a $LogFile
	echo "Start merging runs from trains to ${BASEDIR}/${MergeTrainsOutname}" | tee -a $LogFile
	echo  -e "\e[36m------------------------------------\e[0m" | tee -a $LogFile
	for TrainNumber in `cat $TrainNumberFile`
	do
		echo;echo;
		echo  -e "\e[36m|-> \e[0m $TrainNumber" | tee -a $LogFile
		singletrainDir=.$TrainPage-$TrainNumber
		for RunlistName in `cat $OptRunlistNamefile`
		do
			echo  -e "\t\e[36m|-> \e[0m $RunlistName" | tee -a $LogFile

			# look for availible periods
			periodList="${BASEDIR}/${MergeTrainsOutname}/$RunlistName/.periodList.txt"
			if [[ -f $periodList ]]; then new_rm $periodList; fi
			cmd="ls $BASEDIR/$singletrainDir/$RunlistName/ | grep 'LHC' >>  $periodList"
			eval $cmd
			usecmd $cmd

			for periodnametmp in `cat $periodList`
			do
				Periodname2=${periodnametmp#*$RunlistName/}
				Periodname=${Periodname2%/*}
				PeriodnameWOextra=${Periodname2%_extra*}
				echo  -e "\t\t\e[36m|-> \e[0m $Periodname \n" | tee -a $LogFile


				# look for availible periods
				Runlist="${BASEDIR}/${MergeTrainsOutname}/$RunlistName/.$Periodname-Runlist.txt"
				if [[ -f $Runlist ]]; then new_rm $Runlist; fi
				cmd="ls -d $BASEDIR/$singletrainDir/$RunlistName/$Periodname/[0-9]* >>  $Runlist"
				eval $cmd
				usecmd $cmd

				for runnametmp in `cat $Runlist`
				do
					runname2=${runnametmp#*$Periodname/}
					runname=${runname2%/*}
					printf "\t\t\t\e[36m|-> \e[0m $runname \n" | tee -a $LogFile
					mkdir -p ${BASEDIR}/${MergeTrainsOutname}
					mkdir -p ${BASEDIR}/${MergeTrainsOutname}/$RunlistName
					mkdir -p ${BASEDIR}/${MergeTrainsOutname}/$RunlistName/$PeriodnameWOextra
					mkdir -p ${BASEDIR}/${MergeTrainsOutname}/$RunlistName/$PeriodnameWOextra/$runname
					# look for availible Files
					FileList="${BASEDIR}/${MergeTrainsOutname}/$RunlistName/.FileList.txt"
					if [ -f $FileList ]; then new_rm $FileList; fi
					for Search in `cat $Searchfile`
					do
						cmd="ls $BASEDIR/$singletrainDir/$RunlistName/$Periodname/$runname/ | grep "$Search" | grep ".root" >> $FileList"
						eval $cmd
						usecmd $cmd
					done
					for filenametmp in `cat $FileList`
					do
						filename=${filenametmp#*/$runname/}
						printf "\t\t\t\t\e[33m|-> \e[0m $filename" | tee -a $LogFile
						Dirout=$BASEDIR/$singletrainDir/$RunlistName/$Periodname/$runname/
						Dirin="0"
						DirMerged=${BASEDIR}/${MergeTrainsOutname}/$RunlistName/$PeriodnameWOextra/$runname/
						doMergeFiles $filename $Dirout $Dirin $DirMerged "Runs"
					done
				done
				echo;
			done
		done
	done
}
function doMergeTrains(){
	redomerging=0
	echo  -e "\e[36m------------------------------------\e[0m" | tee -a $LogFile
	echo "Start merging trains: ${BASEDIR}/${MergeTrainsOutname}" | tee -a $LogFile
	echo  -e "\e[36m------------------------------------\e[0m" | tee -a $LogFile
	for TrainNumber in `cat $TrainNumberFile`
	do
		echo;echo;
		echo  -e "\e[36m|-> \e[0m $TrainNumber" | tee -a $LogFile
		singletrainDir=.$TrainPage-$TrainNumber
		for RunlistName in `cat $OptRunlistNamefile`
		do
			echo  -e "\t\e[36m|-> \e[0m $RunlistName " | tee -a $LogFile
			mkdir -p ${BASEDIR}/${MergeTrainsOutname}/$RunlistName

			# look for availible Files
			FileList="${BASEDIR}/${MergeTrainsOutname}/$RunlistName/.FileList.txt"
			if [ -f $FileList ]; then new_rm $FileList; fi
			for Search in `cat $Searchfile`
			do
				cmd="ls $BASEDIR/$singletrainDir/$RunlistName/ | grep "$Search" | grep ".root" >> $FileList"
				eval $cmd
				usecmd $cmd
			done
			for filenametmp in `cat $FileList`
			do
				if [[ $filenametmp = *".root.tmp" ]]; then
					new_rm $filenametmp
					continue
				fi
				filename=${filenametmp#*/$RunlistName/*/}
				printf "\t\t\e[33m|-> \e[0m $filename" | tee -a $LogFile
				Dirout=$BASEDIR/$singletrainDir/$RunlistName
				Dirin="0"
				mkdir -p ${BASEDIR}/${MergeTrainsOutname}
				mkdir -p ${BASEDIR}/${MergeTrainsOutname}/$RunlistName
				DirMerged=${BASEDIR}/${MergeTrainsOutname}/$RunlistName
				if [[ ! -f $DirMerged/$filename ]]; then
					redomerging=1
				fi
				if [[ $redomerging = 1 ]]; then
					new_rm $Dirout/.${filename%%.root}.merged
				fi
				doMergeFiles $filename $Dirout $Dirin $DirMerged $MergeTrainsOutname
			done
		done
	done
}
function doMergeFiles() {
	filename=$1
	Dirout=$2
	Dirin=$3
	DirMerged=$4
	outFile="$Dirout/$filename"
	inFile="$Dirin/$filename"
	downlogFile="$Dirout/${filename%%.root}.downlog"
	mergedFile=$DirMerged/$filename
	if [ "$#" == "5" ]; then
		alreadyMerged="$Dirout/.${filename%%.root}_$5.merged"
	else
		alreadyMerged="$Dirout/.${filename%%.root}.merged"
	fi
	logFile=$Dirout/.${filename%%.root}.log
	new_rm $logFile
	new_rm $mergedFile.tmp
	if [[ -f $alreadyMerged ]]
	then
		printf "\e[33m|-> \e[0m already merged\n" | tee -a $LogFile
		return
	fi
	if [[ ! -f $outFile ]]
	then
		if [[ $debug = 1 ]] || [[ $debug = 2 ]]
		then
			echo -e "\t\e[31m|->\e[0m File missing $outFile  "  | tee -a $ErrorLog | tee -a $LogFile
		else
			echo -e "\t\e[31m|->\e[0m File missing "  | tee -a $ErrorLog | tee -a $LogFile
		fi
		return
	fi
	if [[ ! -f $mergedFile ]]
	then
		echo -e "\e[33m|->\e[0m Copy: is first" | tee -a $LogFile
		echo "Copyed to $mergedFile" > $logFile | tee -a $LogFile
		cp $outFile $mergedFile
		touch $alreadyMerged
		return
	fi
	printf "\e[33m|->\e[0m merging Files \n"  #(log: $logFile)" | tee -a $LogFile
	hadd -k $mergedFile.tmp $mergedFile $outFile  &> $logFile
	exitstatus=$?
	if [[ ! "$exitstatus" = "0" ]]; then
		printf "  \e[33m|->\e[0m Retry with download" | tee -a $LogFile
	fi
	tmp=1
	while [[ ! "$exitstatus" = "0" ]]; do
		if [[ $tmp = 6 ]]; then
			echo "." | tee -a $LogFile
			break
		fi
		printf "${tmp} " | tee -a $LogFile
		new_rm $mergedFile.tmp
		if [[ ! $Dirin -eq "0"  ]]; then
			new_rm $outFile
			GetFile $inFile $outFile $downlogFile
		fi
		printf "  \e[33m|->\e[0m mergeing " | tee -a $LogFile
		hadd -k $mergedFile.tmp $mergedFile $outFile  &> $logFile
		exitstatus=$?
		((tmp++))
	done
	if [[ ! "$exitstatus" = "0" ]]
	then
		echo -e "\e[31mError\e[0m $outFile not merged correctly" | tee -a $ErrorLog | tee -a $LogFile
		cat $logFile >> $ErrorLog
		new_rm $mergedFile.tmp
	else
		new_rm $mergedFile
		mv $mergedFile.tmp $mergedFile
		touch $alreadyMerged
	fi
}
function SeachFilesOnAlienAndAddToList(){
	if [[ $debug = 1 ]] || [[ $debug = 2 ]]
	then
		echo "SeachFilesOnAlienAndAddToList:" | tee -a $LogFile
	fi
	for Search in `cat $FilenamesinTrain`
	do
		cmd='alien_ls $1 2> /dev/null | grep "$Search" > $2.tmp'
		if [[ $debug = 1 ]] || [[ $debug = 2 ]]
		then
			echo $cmd | tee -a $LogFile
		fi
		eval $cmd
		tmpcouuntnew=0
		if [[ ! "$?" = "0" ]]; then
			printf "  \e[33m|->\e[0m Retry " | tee -a $LogFile
		fi
		while [[ ! $? = "0" ]]; do
			if [[ $tmpcouuntnew > 10 ]]; then break; fi
			printf "${tmpcouuntnew} " | tee -a $LogFile
			# printf "." | tee -a $LogFile
			((tmpcouuntnew++))
			eval $cmd
		done
		usecmd $cmd
		AddToList $2.tmp $2
		new_rm $2.tmp
	done
}
function FindPathsAndAddToList(){
	if [[ $debug = 1 ]] || [[ $debug = 2 ]]
	then
		echo "FindPathsAndAddToList:" | tee -a $LogFile
	fi
	for Search in `cat $FilenamesinTrain`
	do
		cmd="alien_find $1 $Search  2> /dev/null | grep $TrainPage/$TrainNumber | grep child_$childID/ > $2.tmp"
		eval $cmd
		tmpcouuntnew=0
		if [[ ! "$?" = "0" ]]; then
			printf "  \e[33m|->\e[0m Retry " | tee -a $LogFile
		fi
		while [[ ! $? = "0" ]]; do
			if [[ $tmpcouuntnew > 10 ]]; then break; fi
			printf "${tmpcouuntnew} " | tee -a $LogFile
			# printf "." | tee -a $LogFile
			((tmpcouuntnew++))
			eval $cmd
		done
		usecmd $cmd
		AddToList $2.tmp $2
		new_rm $2.tmp
	done
}
function GetChilds(){
	cmd="alien_ls $AlienDir 2> /dev/null | grep $TrainNumber\_2 &> $1"
	if [[ $debug = 1 ]] || [[ $debug = 2 ]]
	then
		echo $cmd | tee -a $LogFile
	fi
	eval $cmd
	tmpcouuntnew=0
	while [[ ! $? = "0" ]] ; do
		if [[ $tmpcouuntnew > 10 ]]; then break; fi
		((tmpcouuntnew++))
		eval $cmd
		printf "."
		sleep 1
	done
}
# Fuction to check if file is there and download it if not
function GetFile(){
	if [[ $debug = 1 ]] || [[ $debug = 2 ]]; then
		echo -e "\e[33mGetFile(): \e[0m alien:/$1  file:/$2" | tee -a $LogFile
	fi

	if [[ -f $3 ]]; then
		if [[ $debug = 1 ]] || [[ $debug = 2 ]]; then
			cat $3 | tee -a $LogFile
		fi
		if [[ ! `grep "100.00 %" $3 | wc -l` > 0 ]]; then
			if [[ -f $2 ]]; then
				new_rm $2
				if [[ $debug = 1 ]] || [[ $debug = 2 ]]; then
					echo -e "\t deleting $2" | tee -a $LogFile
				fi
			fi
		fi
	fi

	if [[ -f $2 ]] # Check if file there
	then
		if [[ $debug = 1 ]] || [[ $debug = 2 ]]; then
			echo -e "\e[33m|-> \e[0mFile $2 exists." | tee -a $LogFile
		else
			printf "\e[33m|-> \e[0mFile exists." | tee -a $LogFile
		fi
	else
		tmp=`alien_ls $1 2> /dev/null | grep "no such file or directory" | wc -c`
		while [[ ! $? = "0" ]]; do
			print "."
			tmp=`alien_ls $1 2> /dev/null | grep "no such file or directory" | wc -c`
		done
		if [[ $tmp -eq 0 ]]; then
			if [[ $debug = 1 ]] || [[ $debug = 2 ]]; then
				echo -e "\e[33m|-> \e[0mDownloading file alien:/$1" | tee -a $LogFile
				alien_cp -o  alien:/$1 file:/$2 &> $3
			else
				printf "\e[33m|-> \e[0mDownloading file" | tee -a $LogFile
				alien_cp -o  alien:/$1 file:/$2 &> $3
			fi
			downexitstatus=$?
			tmpdowncount=1
			if [[ ! -f $2 ]] || [[ ! "$downexitstatus" = "0" ]]; then
				printf "  \e[33m|->\e[0m Retry " | tee -a $LogFile
			fi
			while [[ ! -f $2 ]] || [[ ! "$downexitstatus" = "0" ]]; do
				if [[ $tmpdowncount = 10 ]]; then
					echo "." | tee -a $LogFile
					break
				fi
				printf "${tmpdowncount} " | tee -a $LogFile
				alien_cp -o  alien:/$1 file:/$2 &> $3
				downexitstatus=$?
				((tmpdowncount++))
			done
			if [[ -f $2 ]] && [[ `grep "100.00 %" $3 | wc -l` > 0 ]] ; then
				printf "  \e[33m|->\e[0m successful  " | tee -a $LogFile
			else
				printf "  \e[33m|->\e[0m Download failed "   | tee -a $ErrorLog | tee -a $LogFile
			fi
		else
			if [[ $debug = 1 ]] || [[ $debug = 2 ]]
			then
				printf "\e[31m|->\e[0m missing $1 on alien"    | tee -a $ErrorLog | tee -a $LogFile
			else
				printf "\e[31m|->\e[0m missing on alien"  | tee -a $ErrorLog | tee -a $LogFile
			fi
		fi
	fi

	if [[ $debug = 1 ]] || [[ $debug = 2 ]]; then
		cat $3 | tee -a $LogFile
	fi
}
