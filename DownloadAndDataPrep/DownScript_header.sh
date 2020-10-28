#! /bin/bash
# This Script is intended to automize the download of train outputs

# Version: V6.2
echo  -e "\e[36m+++++++++++++++++++++++++++++++++++++\e[0m"
echo "DownScript.sh Version: V6.2"

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
isjalien=0
ErrorLog=""
WARNINGLog=""
pathtocert=""
UserName=""
alienUserName=""
BASEDIR=${PWD}
thisuser=`echo ${USER}`
NCHilds=0
NorunwiseinTrain=0
NFiles=0

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
OptIsJJ=0
Optfast=0
OptRunlistNameSet=0
OptNoRunlist=0
OptAllRunlists=0
UseMerge=0
MergeTrains=0
MultiTrains=0
Userunwise=0
UseOnlyrunwise=0
NorunwiseinTrain=0
SetOutName=""
OptZip=1
OptZipRun=0
OptZipSubRun=0
Usechildsareperiods=0
newfiles=0
RedoMerging=0
re='^[0-9]+$'

RunningScripts=0
OptRunlistNamefile=OptRunlistNames_$RunningScripts.txt
OptDownloadAll=0

job=$$
# job=1
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

maxpthardbins=1
minpthardbins=1

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
	# we download the html to grep needed information from there
	if [ -f $1 ]
	then # Check if older than one day (or not)
		# cat $1 | wc -l
		if [[ `cat $1 | wc -l` -lt 100 ]]
		then
			new_rm $1
			echo -e "\e[33m|-> \e[0mDownloading new one: ${2}" | tee -a $LogFile
			cmd="curl '${2}' --key $3/$key -k --cert $3/$clientca &> $1"
			eval $cmd
			usecmd $cmd
			touch "$1";
		else
			# echo -e "\e[33m|-> \e[0mFile $1 exists." | tee -a $LogFile
		# fi
			if [ "$(( $(date +"%s") - $(stat -c "%Y" $1) ))" -gt "604800" ]  # 86400 = 1 Day
			then
				echo -e "\e[33m|-> \e[0mFile $1 exists. \e[33m|-> \e[0mbut is older then 7 Days! download new one from alien:/$1" | tee -a $LogFile
				# cmd="curl -s '${2}' --key $3/$key --cacert $3/$cacert --cert $3/$clientca &> $1"
				cmd="curl '${2}' --key $3/$key -k --cert $3/$clientca &> $1"
				eval $cmd
				usecmd $cmd
				touch "$1";
			else
				echo -e "\e[33m|-> \e[0mFile $1 exists. \e[33m|-> \e[0mand is up-to-date" | tee -a $LogFile
			fi
		fi
		# echo -e "\e[33m|-> \e[0mFile $1 exists." | tee -a $LogFile
	else
		echo -e "\e[33m|-> \e[0mDownloading ${2}" | tee -a $LogFile
		# cmd="curl '${Webpage}' --key $3/$key --cacert $3/$cacert --cert $3/$clientca &> $1"
		cmd="curl '${2}' --key $3/$key -k --cert $3/$clientca &> $1"
		usecmd $cmd
		eval $cmd
		touch "$1";
	fi
}
# # if this is not working try
# openssl pkcs12 -in myCertificate.p12 -nodes -out newcert.pem
# openssl pkcs12 -in myCertificate.p12 -nodes -out ca.pem -cacerts -nokeys
# openssl pkcs12 -in myCertificate.p12 -nodes -out client.pem -clcerts -nokeys
# openssl pkcs12 -in myCertificate.p12 -nodes -out key.pem -nocerts



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
	# if [[ $debug = 1 ]] || [[ $debug = 2 ]]
	# then
	# 	echo "cat $2" | tee -a $LogFile
	# 	cat $2 | tee -a $LogFile
	# fi
}

function Init(){

	#####################################

	if [[ $thisuser = "adrian"  ]]
	then
		BASEDIR="/media/adrian/Adrian/grid_data"
		# BASEDIR="/home/adrian/grid_data"
		FrameworkDir="/home/adrian/git/AnalysisSoftware"
		UserName="Adrian Mechler";
		pathtocert="~/.globus"
		alienUserName="amechler"
		key="key.pem"
		cacert="ca.pem"
		clientca="client.pem"
	elif [[ $thisuser = "amechler" ]]
	then
		BASEDIR="/afs/cern.ch/user/a/amechler/Download"
		FrameworkDir="/afs/cern.ch/user/a/amechler/Download"
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
	if [[ $isjalien = 0 ]]; then
		if [[ `alien-token-info` =~ "No Token found!" ]]
		then
			echo -e "\e[31mWARNING:\e[0m No alien token" | tee -a $LogFile
			# alien-token-init $alienUserName
			echo;echo;
			alien-token-init $alienUserName
			echo  -e "\e[36m------------------------------------\e[0m" | tee -a $LogFile
			echo;echo;
		fi
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

# Fuction to check if file is there and download it if not
function GetFile_jalien(){

	debugtmp=0
	silent=0
	if [ "$#" == "4" ]; then
		if [[ $4 = 0 ]]; then
			silent=1
		fi
		# debugtmp=$4
	fi
	if [[ $debug = 1 ]] || [[ $debug = 2 ]] ; then
		debugtmp=1
	fi
	if [[ $debugtmp = 1 ]] || [[ $debug = 3 ]]; then
		echo -e "\e[33mGetFile(): \e[0m alien_cp $1 file:///$2" | tee -a $LogFile
		# echo -e "\e[33mGetFile(): \e[0m alien:/$1  file:/$2" | tee -a $LogFile
	fi

	if [[ -f $3 ]]; then
		if [[ $debug = 1 ]] || [[ $debug = 2 ]]; then
			cat $3 | tee -a $LogFile
		fi
		if [[ ! `grep "SUCCESS" $3 | wc -l` > 0 ]] && [[ ! `grep "100.00 %" $3 | wc -l` > 0 ]]; then
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
		if [[ $debugtmp = 1 ]] || [[ $debugtmp = 2 ]]; then
			echo -e "\e[33m|-> \e[0mFile $2 exists." | tee -a $LogFile
		else
			if [[ $silent = 1 ]]; then
				printf "\e[33m|-> \e[0mFile exists." >> $LogFile
			else
				printf "\e[33m|-> \e[0mFile exists." | tee -a $LogFile
			fi
		fi
	else
		# if [[ $isjalien = 1 ]]; then
		# 	tmp=`alien.py ls $1 2> /dev/null | grep "no such file or directory" | wc -c`
		# else
			tmp=`alien_ls $1 2> /dev/null | grep "no such file or directory" | wc -c`
		# fi
		while [[ ! $? = "0" ]]; do
			print "."
			# if [[ $isjalien = 1 ]]; then
			# 	tmp=`alien.py ls $1 2> /dev/null | grep "no such file or directory" | wc -c`
			# else
				tmp=`alien_ls $1 2> /dev/null | grep "no such file or directory" | wc -c`
			# fi
		done
		if [[ $tmp -eq 0 ]]; then
			if [[ $debug = 1 ]] || [[ $debug = 2 ]]; then
				echo -e "\e[33m|-> \e[0mDownloading file alien:/$1" | tee -a $LogFile
				alien_cp $1 file:///$2 &> $3
				# alien_cp -o  alien:/$1 file:/$2 &> $3
			else
				if [[ $silent = 1 ]]; then
					printf "\e[33m|-> \e[0mDownloading file" >> $LogFile
				else
					printf "\e[33m|-> \e[0mDownloading file" | tee -a $LogFile
				fi
				alien_cp $1 file:///$2 &> $3
				# alien_cp -o  alien:/$1 file:/$2 &> $3
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
				alien_cp $1 file:///$2 &> $3
				# alien_cp -o  alien:/$1 file:/$2 &> $3
				downexitstatus=$?
				((tmpdowncount++))
			done
			# if [[ -f $2 ]] && [[ `grep "SUCCESS" $3 | wc -l` > 0 ]]  && [[ ! `grep "STATUS OK" $3 | wc -l` > 0 ]]; then
			if [[ -f $2 ]] && [[ `grep "jobID: 1/1 >>> Start" $3 | wc -l` > 0 ]]  && [[ ! `cat $3 | wc -l` > 1 ]]; then
				if [[ $silent = 1 ]]; then
					printf "  \e[33m|->\e[0m successful  " >> $LogFile
				else
					printf "  \e[33m|->\e[0m successful  " | tee -a $LogFile
				fi
			else
				printf "  \e[33m|->\e[0m Download failed "   | tee -a $ErrorLog | tee -a $LogFile
				cat $3
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

function GetFile_alienrootLegacy(){

	debugtmp=0
	silent=0
	if [ "$#" == "4" ]; then
		if [[ $4 = 0 ]]; then
			silent=1
		fi
		# debugtmp=$4
	fi
	if [[ $debug = 1 ]] || [[ $debug = 2 ]] ; then
		debugtmp=1
	fi
	if [[ $debugtmp = 1 ]] || [[ $debug = 3 ]]; then
		echo -e "\e[33mGetFile(): \e[0m alien_cp $1 file:///$2" | tee -a $LogFile
		# echo -e "\e[33mGetFile(): \e[0m alien:/$1  file:/$2" | tee -a $LogFile
	fi

	if [[ -f $3 ]]; then
		if [[ $debug = 1 ]] || [[ $debug = 2 ]]; then
			cat $3 | tee -a $LogFile
		fi
		if [[ ! `grep "100.00 %" $3 | wc -l` > 0 ]] && [[ ! `grep "SUCCESS" $3 | wc -l` > 0 ]]; then
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
		if [[ $debugtmp = 1 ]] || [[ $debugtmp = 2 ]]; then
			echo -e "\e[33m|-> \e[0mFile $2 exists." | tee -a $LogFile
		else
			# echo -e "\e[33m|-> \e[0mFile $2 exists." >> $LogFile
			if [[ $silent = 1 ]]; then
				printf "\e[33m|-> \e[0mFile exists." >> $LogFile
			else
				printf "\e[33m|-> \e[0mFile exists." | tee -a $LogFile
			fi
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
				if [[ $silent = 1 ]]; then
					printf "\e[33m|-> \e[0mDownloading file" >> $LogFile
				else
					printf "\e[33m|-> \e[0mDownloading file"  | tee -a $LogFile
				fi
				alien_cp -o  alien:/$1 file:/$2 &> $3
			fi
			downexitstatus=$?
			tmpdowncount=1
			if [[ ! -f $2 ]] || [[ ! "$downexitstatus" = "0" ]]; then
				if [[ $debug = 1 ]] || [[ $debug = 2 ]]; then
					printf "  \e[33m|->\e[0m Retry " | tee -a $LogFile
				else
					if [[ $silent = 1 ]]; then
						printf "  \e[33m|->\e[0m Retry " >> $LogFile
					else
						printf "  \e[33m|->\e[0m Retry " | tee -a $LogFile
					fi
				fi
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
			if [[ -f $2 ]] && [[ `grep "100.00 %" $3 | wc -l` > 0 ]] && [[ ! `grep "SUCCESS" $3 | wc -l` > 0 ]]; then
				if [[ $debug = 1 ]] || [[ $debug = 2 ]]; then
					printf "  \e[33m|->\e[0m successful  " | tee -a $LogFile
				else
					if [[ $silent = 1 ]]; then
						printf "  \e[33m|->\e[0m successful  " >> $LogFile
					else
						printf "  \e[33m|->\e[0m successful  " | tee -a $LogFile
					fi
				fi
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

function GetFile(){
	if [[ $isjalien = 1 ]]; then
		GetFile_jalien $1 $2 $3 $4
	else
		GetFile_alienrootLegacy $1 $2 $3 $4
	fi
}
function GetFileZip(){
	GetFile $1 $2 $3
	if [[ -f $2 ]]; then
		if [[ ! -f "$2.zipped" ]]; then
			if [[ $debug = 1 ]] || [[ $debug = 2 ]]
			then
				unzip -o $2 -d $4/ | tee -a $LogFile
			else
				unzip -o $2 -d $4/ >> $LogFile
			fi
			touch "$2.zipped"
		# else
			# printf "\e[33m|-> Files was unzipped already :\e[0m " | tee -a $LogFile
		fi
	fi
	echo;
}
function InitilaizeChild(){
	echo
	echo  -e "\e[35m=====================================\e[0m" | tee -a $LogFile
	echo
	echo -e "\e[35m $1 \e[0m" | tee -a $LogFile

	childID=${1##*child_}
	childID=${childID%/*}

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
	mkdir -p "$OUTPUTDIR/.$1"

	# get one globalvariables.C to get information about the child
	GlobalVariablesPath="$OUTPUTDIR/.$1/$GlobalVariablesFile"
	GetFile "$AlienDir$1/$GlobalVariablesFile" "$GlobalVariablesPath" $GlobalVariablesPath.downlog 0
	# echo;

	ChildName=`grep "periodName =" "$GlobalVariablesPath" | awk -F "= " '{print $2}' | awk -F ";" '{print $1}' | awk -F "\"" '{print $2}'`
	if [[ $ChildName = "" ]]; then ChildName=`grep "periodName =" "$GlobalVariablesPath" | awk -F "=" '{print $2}' | awk -F ";" '{print $1}' | awk -F "\"" '{print $2}'`; fi
	if [[ $ChildName = "" ]]; then ChildName=`grep "periodName=" "$GlobalVariablesPath" | awk -F "=" '{print $2}' | awk -F ";" '{print $1}' | awk -F "\"" '{print $2}'`; fi
	if [[ $debug = 1 ]] || [[ $debug = 2 ]]
	then
		echo -e " periodName = $ChildName" | tee -a $LogFile
	else
		echo -e " periodName = $ChildName" >> $LogFile
	fi

	ChildName2=`grep "export ALIEN_JDL_child_${childID}_LPMPRODUCTIONTAG" $envFile | cut -d "'" -f 2`
	# ChildName=`echo $(head -n 1 $PathtoRuns) | awk -F "/" '{print $7}' ` | tee -a $LogFile
	if [[ $ChildName2 = "" ]]; then
		ChildName2=`grep "export TEST_DIR_child_${childID}=" $envFile | cut -d "'" -f 2 | cut -d "/" -f 5`
	fi
	if [[ $ChildName2 = "" ]]; then
		ChildName2=`grep "export ALIEN_JDL_LPMPRODUCTIONTAG=" $envFile | cut -d "'" -f 2 | cut -d "/" -f 5`
	fi
	if [[ $Usechildsareperiods = 1 ]] || [[ ! $ChildName == $ChildName2 ]]; then
		if [[ $debug = 1 ]] || [[ $debug = 2 ]]; then
			echo -e "\e[33m|->\e[0m Changed periodName = $ChildName2" | tee -a $LogFile
		fi
		ChildName=$ChildName2
	fi

	Type='data'
	if (( `echo -n ${ChildName#LHC*} | wc -c` > 3 )); then
		Type='sim'
	fi
	Year="20${ChildName:3:2}"
	echo -e "\e[33m|->\e[0m Type: $Type; Year: $Year; periodName: $ChildName" | tee -a $LogFile


}
function GetChilds(){
	# if [[ $isjalien = 1 ]]; then
	# 	cmd="alien.py ls $AlienDir 2> /dev/null | grep $TrainNumber\_2 &> $1"
	# else
		cmd="alien_ls $AlienDir 2> /dev/null | grep $TrainNumber\_2 &> $1"
	# fi
	if [[ $debug = 1 ]] || [[ $debug = 2 ]]
	then
		echo $cmd | tee -a $LogFile
	fi
	tmpcouuntnew=0
	eval $cmd
	while [[ ! $? = "0" ]] ; do
		if [[ $tmpcouuntnew > 5 ]]; then break; fi
		((tmpcouuntnew++))
		eval $cmd
		# alien_ls $AlienDir 2> /dev/null | grep $TrainNumber\_2  | tee $1
		# printf "${tmpcouuntnew} " | tee -a $LogFile
		# printf "."<
		sleep 1
	done
}
function InitilaizeTrain(){
	TrainNumber=$1
	OutName=.$TrainPage-$TrainNumber
	ErrorLog="$BASEDIR/$OutName/Errors.log"
	WARNINGLog="$BASEDIR/$OutName/Warnings.log"

	new_rm $ErrorLog
	new_rm $WARNINGLog

	for file in `find  $BASEDIR/$OutName/ -name .FileForAllMergeProcesses.txt`; do new_rm $file; done
	for file in `find  $BASEDIR/$OutName/ -name .FileForAllMergeProcessesSavedDetails.txt`; do new_rm $file; done


	OUTPUTDIR=$BASEDIR/$OutName
	List="$OUTPUTDIR/ListGrid.txt"
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
	tmphere=0
	while [[ $Childeone = "" ]]; do
		GetChilds $List
		if [[ -f $List ]]; then
			if [[ $Childeone = "" ]]
			then
				Childeone=`sed '2q;d' $List`
			fi
			if [[ $Childeone = "" ]]
			then
				Childeone=`sed '1q;d' $List`
			fi
			if [[ $Childeone = "" ]]
			then
				Childeone=`sed '0q;d' $List`
			fi
		fi

		if [[ $tmphere > 9 ]]; then
			echo -e "\e[33m|->\e[0m Childeone ($Childeone) not found" | tee -a $LogFile
			return 0
		fi
		if [[ $tmphere > 0 ]]; then
			printf "."
		fi
		((tmphere++))
		sleep 1
	done
	# cat $List
	GetFile "$AlienDir$Childeone/env.sh" "$envFile" "$envFile.downlog" 0
	# echo
	PERIODNAME=`grep "PERIOD_NAME=" $envFile | awk -F "=" '{print $2}' | awk -F ";" '{print $1}'`
	NCHilds=`grep "CHILD_DATASETS=" $envFile | awk -F "=" '{print $2}' | awk -F ";" '{print $1}'`
	SLOWTRAINRUN=`grep "SLOW_TRAIN_RUN=" $envFile | awk -F "=" '{print $2}' | awk -F ";" '{print $1}'`
	if [[ $SLOWTRAINRUN = '0' ]]; then
		NorunwiseinTrain=1
	fi
	if [[ $debug = 2 ]] || [[ $debug = 1 ]]
	then
		echo -e "\e[33m|->\e[0m $Childeone" | tee -a $LogFile
		echo "PERIODNAME = $PERIODNAME" | tee -a $LogFile
		echo "NCHilds = $NCHilds" | tee -a $LogFile
		echo
	fi

	lego_train_mergePath="$OUTPUTDIR/.lego_train_merge.C"
	GetFile $AlienDir$Childeone/lego_train_merge.C "$lego_train_mergePath" $lego_train_mergePath.downlog 0
	# echo
	grep "TString outputFiles" "$lego_train_mergePath"  | awk -F "\"" '{print $2}' > $FilenamesinTrain.tmp
	sed -i 's/,/\n/g' $FilenamesinTrain.tmp
	sed -i 's/ //g' $FilenamesinTrain.tmp
	new_rm $FilenamesinTrain.tmp2
	new_rm $FilenamesinTrain

	if [[ $OptDownloadAll = 1 ]]; then
		for SearchTMP in `cat $FilenamesinTrain.tmp`; do
			if [[ ! $SearchTMP = "AnalysisResults.root" ]] && [[ ! $SearchTMP = "EventStat_temp.root" ]]; then
				echo $SearchTMP >> $FilenamesinTrain
			fi
		done
	else
		for SearchTMP in `cat $Searchfile`
		do
			grep $SearchTMP $FilenamesinTrain.tmp >> $FilenamesinTrain.tmp2
		done
		# AddToList $FilenamesinTrain.tmp2 $FilenamesinTrain
		mv $FilenamesinTrain.tmp2 $FilenamesinTrain
		new_rm $FilenamesinTrain.tmp
		new_rm $FilenamesinTrain.tmp2
	fi


	# Loop over all childs
	unset LIST_child
	for child in `sort ${List}`;
	do
		LIST_child+=($child)
	done

}
function InitilaizeRunlist(){
	arrypos=$1
	useSpecificRunlist=0
	SearchSpecificRunlist=""
	RLextension=""
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
	if [[ ! "$OptRunlistName" = *"useSpecificRunlist_"* ]]; then
		Runlist="$RunlistOnTrainpage"
	fi

	if [[ "$OptRunlistName" = *"useSpecificRunlist_"* ]]; then
		if [[ ! $RunlistID = "1" ]]; then return 1; fi
		useSpecificRunlist=1
		SearchSpecificRunlist=${RunlistName#*useSpecificRunlist_}
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
	for runName in `cat $Runlist`
	do
		printf "$runName, " | tee -a $LogFile
		((NumberOfRuns++))
	done
	printf "\n $NumberOfRuns Runs found \n" | tee -a $LogFile
	Dirout=$OUTPUTDIR/$RunlistName/$ChildName
	if [[ $OptNoRunlist = 0 ]]; then
		Dirin=$AlienDir$child/merge_runlist_$RunlistID
	else
		Dirin=$AlienDir$child/merge
	fi
	DirMerged=$OUTPUTDIR/$RunlistName
	mkdir -p $DirMerged
	mkdir -p $Dirout
	if [[ $NumberOfRuns = 0 ]] ; then
		echo  -e "\e[33mWARNING:  No runs in Runlist\e[0m: ChildName = $ChildName, childID = $childID, Runlist $RunlistID, Name = $RunlistName" | tee -a $WARNINGLog | tee -a $LogFile
		return 1
	fi



	# prepare directory
	mkdir -p "$OUTPUTDIR/.$child"
	mkdir -p "$OUTPUTDIR/.$child/$RunlistName"

	FileList="$OUTPUTDIR/.$child/$RunlistName/.FileList.txt"

	# look for availible Files
	if [[ ! $useSpecificRunlist = 1 ]]; then
		newfilestmp=0
		if [[ ! -f $FileList ]]; then
			newfilestmp=1
		elif [[  `grep "$Search" $FileList | wc -l` < 1 ]]; then
			newfilestmp=1
		fi
		if [[ $newfilestmp = 1 ]] || [ ! -f $FileList ]; then
			cmd="alien_ls $AlienDir$child/merge_runlist_$RunlistID/ 2> /dev/null > tmp.txt"
			eval $cmd
			usecmd $cmd
			for Search in `cat $FilenamesinTrain`
			do
				if [[ $OptNoRunlist = 0 ]]; then
					# if [[ $isjalien = 1 ]]; then
					# 	cmd="alien.py ls $AlienDir$child/merge_runlist_$RunlistID/ 2> /dev/null | grep "$Search" > $FileList.tmp"
					# else
						cmd="cat tmp.txt | grep "$Search" > $FileList.tmp"
					# fi
				else
					# if [[ $isjalien = 1 ]]; then
					# 	cmd="alien.py ls $AlienDir$child/merge/ 2> /dev/null | grep "$Search" > $FileList.tmp"
					# else
						cmd="cat tmp.txt | grep "$Search" > $FileList.tmp"
					# fi
				fi
				eval $cmd
				usecmd $cmd
				AddToList $FileList.tmp $FileList
			done
			rm tmp.txt
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
			((NFiles++))
			if [[ $OptDownloadAll = 1 ]]; then
				OptDownloadAll=0
			fi
			Searchtmp=${setting#*\?_}
			if [[ $Searchtmp = ".root" ]]; then
				OptDownloadAll=1
				OptZip=1
				OptZipRun=1
				OptZipSubRun=1
				Search=*$Searchtmp*
			fi
			if [[ $Searchtmp = *".zip" ]]; then
				OptZip=1
				OptZipRun=1
				OptZipSubRun=1
				Search=*$Searchtmp*
			else
				echo $Searchtmp >> $Searchfile
				if [[ $Search = ".root" ]]
				then
					Search=*$Searchtmp*
				else
					Search=$Search'|'*$Searchtmp*
				fi
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
		elif [[ $setting = "-isjalien" ]] || [[ $setting = "-jalien" ]]
		then
			isjalien=1
			echo "selecting isjalien"
		elif [[ $setting = "-onlyrunwise" ]]
		then
			Userunwise=1
			UseOnlyrunwise=1
		elif [[ $setting = "-childsareperiods" ]]
		then
			Usechildsareperiods=1
		elif [[ $setting = "-fast" ]]
		then
			Optfast=1
		elif [[ $setting = "-IsJJ" ]]
		then
			OptIsJJ=1
			OptNoRunlist=1
			maxpthardbins=20
		elif [[ $setting = "-IsJJ_"* ]]
		then
			OptIsJJ=1
			OptNoRunlist=1
			minpthardbins=1
			maxpthardbins=${setting#*-IsJJ_}
			echo "selecting pthardbins: 1 - $maxpthardbins"
		elif [[ $setting = "-pThard_"* ]]
		then
			pthardbintmp=${setting#*-pThard_}
			maxpthardbins=${pthardbintmp#*_}
			minpthardbins=${pthardbintmp%_*}
			echo "selecting pthardbins: $minpthardbins - $maxpthardbins"
		elif [[ $setting = "-NoRL" ]]
		then
			OptNoRunlist=1
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
		elif [[ $setting = "-debug1" ]]
		then
			debug=1
		elif [[ $setting = "-debugmore" ]] ||  [[ $setting = "-debug2" ]]
		then
			debug=2
		elif [[ $setting = "-debugsoft" ]] ||  [[ $setting = "-debug3" ]]
		then
			debug=3
		elif [[ $setting = "-debug4" ]]
		then
			debug=4
		elif [[ $setting = "-newfiles" ]] || [[ $setting = "-newFiles" ]]
		then
			newfiles=1
		elif [[ $setting = "-RedoMerging" ]] || [[ $setting = "-redomerging" ]]
		then
			RedoMerging=1
		fi
	done

	if [[ $NFiles > 3 ]]; then
		OptZip=1
		OptZipRun=1
		OptZipSubRun=1
		echo -e "\e[33m|-> \e[0m enable downloading zip files" | tee -a $LogFile
	fi

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
	if [[ $OptIsJJ = 1 ]]; then
		echo -e "\e[33m|-> \e[0m Processing JJMC " | tee -a $LogFile
	fi
	if [[ $Optfast = 1 ]]; then
		echo -e "\e[33m|-> \e[0m not using sub run merging " | tee -a $LogFile
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
			for file in `find  $BASEDIR/.$TrainPage-$TrainNumber/ -name .AllPathList.txt`; do new_rm $file; done
			for file in `find  $BASEDIR/.$TrainPage-$TrainNumber/ -name PathList.txt`; do new_rm $file; done
			for file in `find  $BASEDIR/.$TrainPage-$TrainNumber/ -name .NorunwiseinTrainFile.txt`; do new_rm $file; done
		done
		printf " done \n" | tee -a $LogFile
	fi
	if [ $RedoMerging = 1 ]; then
		echo -e "\e[33m|-> \e[0m RedoMerging files" | tee -a $LogFile
		for TrainNumber in `cat $TrainNumberFile`
		do
			for TMPfile in `find $BASEDIR/.$TrainPage-$TrainNumber/ -name "*.merged"`; do
				for TMPfile2 in `cat $TMPfile`; do
					new_rm $TMPfile2
				done
				rm $TMPfile
			done
		done
		exit
	fi

}
function Parsehtml(){
	if [[ $debug = 2 ]] || [[ $debug = 1 ]]
	then
		printf "\e[33m|-> \e[0m start Parsehtml...\n" | tee -a $LogFile
	fi
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
				if [[ $debug = 2 ]]
				then
					echo -e "\e[33m|-> \e[0m found period" | tee -a $LogFile
				fi
				if [[ $NCHilds = "'1'" ]] ; then
					if [[ $debug = 2 ]] || [[ $debug = 1 ]]
					then
						echo -e "\e[33m|-> \e[0m skipping searching childs" | tee -a $LogFile
					fi
					foundchild=1

					if [[ $OptNoRunlist = 1 ]]; then
						echo -e "\e[33m|->\e[0m RunlistName = default" | tee -a $LogFile
						RunlistName="default"
						RunlistID=1
						foundRunlists=1
						RunlistOnTrainpage="$FrameworkDir/DownloadAndDataPrep/runlistsOnTrainpage/runNumbers$ChildName-$RunlistName.txt"
						LIST_RunlistName+=($RunlistName)
						LIST_RunlistID+=($RunlistID)
						LIST_foundRunlists+=($foundRunlists)
						LIST_RunlistOnTrainpage+=($RunlistOnTrainpage)
						FindAllRunsAndAddToList $RunlistOnTrainpage
					fi
				else
					found=1
				fi
			fi
		fi
		if [[ $found = 1 ]]
		then
			if [[ $debug = 2 ]]
			then
				printf "\e[33m|->\e[0m $line \t " | tee -a $LogFile
			fi
			if [[ $lookchildID = 1 ]]
			then
				if [[ $debug = 2 ]] || [[ $debug = 1 ]]
				then
					echo -e "\e[33m|-> \e[0m lookchildID (childID): $line" | tee -a $LogFile
				fi
				foundchild=0
				if [[ $line = "$childID" ]]
				then
					if [[ $debug = 2 ]] || [[ $debug = 1 ]]
					then
						if [[ $Usechildsareperiods = 0 ]]
						then
							echo -e "\e[33m|->\e[0m ChildName = $ChildName" | tee -a $LogFile
						else
							echo -e "\e[33m|->\e[0m will load child/period name later" | tee -a $LogFile
						fi
						echo -e "\e[33m|->\e[0m childID = $childID" | tee -a $LogFile
					fi
					if [[ $OptNoRunlist = 1 ]]; then
						echo -e "\e[33m|->\e[0m RunlistName = default" | tee -a $LogFile
						RunlistName="default"
						RunlistID=1
						foundRunlists=1
						RunlistOnTrainpage="$FrameworkDir/DownloadAndDataPrep/runlistsOnTrainpage/runNumbers$ChildName-$RunlistName.txt"
						LIST_RunlistName+=($RunlistName)
						LIST_RunlistID+=($RunlistID)
						LIST_foundRunlists+=($foundRunlists)
						LIST_RunlistOnTrainpage+=($RunlistOnTrainpage)
						FindAllRunsAndAddToList $RunlistOnTrainpage
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
			if [[ $line = *"</tr>"* ]] && [[ $NCHilds = "'1'" ]]; then
				foundchild=0
			fi
			# printf " |$line| " | tee -a $LogFile
			if [[ $readRuns = 1 ]]
			then
				if [[ $debug = 2 ]] || [[ $debug = 1 ]]; then
					printf "($line)" | tee -a $LogFile
				fi
				# echo $RunlistOnTrainpage
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
				# useSpecificRunlist=0
				# printf "$RunlistName: " | tee -a $LogFile
				for Searchedrunlist in `cat $OptRunlistNamefile`
				do
					if [[ "$RunlistName" = "$Searchedrunlist" ]] || [[ "$Searchedrunlist" = *"useSpecificRunlist_"* ]]
					then
						if [[ $OptNoRunlist = 0 ]]; then
							LIST_RunlistName+=($Searchedrunlist)
							LIST_RunlistID+=($RunlistID)
							LIST_foundRunlists+=($foundRunlists)
							LIST_RunlistOnTrainpage+=($RunlistOnTrainpage)
							if [[ $debug = 1 ]] || [[ $debug = 2 ]] || [[ $debug = 3 ]]; then
								printf "$Searchedrunlist found \n" | tee -a $LogFile
							fi
							# return
						fi
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
			if [[ $debug = 2 ]] &&  [[ ! $NCHilds = "'1'" ]]; then
				echo -e "\e[33m|-> \e[0m found child: $line" | tee -a $LogFile
			fi
		fi
	done
	if [[ $debug = 2 ]] || [[ $debug = 1 ]]
	then
		printf "\t\t\t done \n" | tee -a $LogFile
	fi
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
				for SearchTMP in `cat $Searchfile`
				do
					cmd="ls $BASEDIR/$singletrainDir/$RunlistName/$Periodname/ | grep "$SearchTMP" | grep ".root" >> $FileList"
					eval $cmd
					usecmd $cmd
				done
				for filenametmp in `cat $FileList`
				do
					filename=${filenametmp#*/$RunlistName/*/}
					printf "\t\t\t\e[33m|-> \e[0m $filename" | tee -a $LogFile
					Dirout2=$BASEDIR/$singletrainDir/$RunlistName/$Periodname/
					Dirin="0"
					DirMerged=${BASEDIR}/${MergeTrainsOutname}/$RunlistName/$PeriodnameWOextra/
					doMergeFiles $filename $Dirout2 $Dirin $DirMerged "Periods"
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
			periodList="${BASEDIR}/${singletrainDir}/$RunlistName/.periodList.txt"
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

				mkdir -p ${BASEDIR}/${MergeTrainsOutname}
				mkdir -p "${BASEDIR}/${MergeTrainsOutname}/listall"
				mkdir -p ${BASEDIR}/${MergeTrainsOutname}/$RunlistName
				mkdir -p ${BASEDIR}/${MergeTrainsOutname}/$RunlistName/$PeriodnameWOextra

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
					mkdir -p "${BASEDIR}/${MergeTrainsOutname}/listall/$PeriodnameWOextra/$runname"
					# mkdir -p ${BASEDIR}/${MergeTrainsOutname}/$RunlistName/$PeriodnameWOextra/$runname
					# look for availible Files
					FileList="${BASEDIR}/${MergeTrainsOutname}/$RunlistName/.FileList.txt"
					if [ -f $FileList ]; then new_rm $FileList; fi
					for SearchTMP in `cat $Searchfile`
					do
						cmd="ls $BASEDIR/$singletrainDir/$RunlistName/$Periodname/$runname/ | grep "$SearchTMP" | grep ".root" >> $FileList"
						eval $cmd
						usecmd $cmd
					done
					for filenametmp in `cat $FileList`
					do
						filename=${filenametmp#*/$runname/}
						printf "\t\t\t\t\e[33m|-> \e[0m $filename" | tee -a $LogFile
						Dirout2=$BASEDIR/$singletrainDir/$RunlistName/$Periodname/$runname/
						Dirin="0"
						DirMerged="${BASEDIR}/${MergeTrainsOutname}/listall/$PeriodnameWOextra/$runname/"
						doMergeFiles $filename $Dirout2 $Dirin $DirMerged "Runs"
					done
					if [[ -d "${BASEDIR}/${MergeTrainsOutname}/listall/$PeriodnameWOextra/$runname" ]] && [[ ! -L "${BASEDIR}/${MergeTrainsOutname}/$RunlistName/$PeriodnameWOextra/$runname" ]] ; then
					   cmd="ln -sf ${BASEDIR}/${MergeTrainsOutname}/listall/$PeriodnameWOextra/$runname ${BASEDIR}/${MergeTrainsOutname}/$RunlistName/$PeriodnameWOextra/."
					   # echo $cmd | tee -a $LogFile
					   usecmd $cmd
					   eval $cmd
				   	fi
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
			for SearchTMP in `cat $Searchfile`
			do
				cmd="ls $BASEDIR/$singletrainDir/$RunlistName/ | grep "$SearchTMP" | grep ".root" >> $FileList"
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
				Dirout2=$BASEDIR/$singletrainDir/$RunlistName
				Dirin="0"
				mkdir -p ${BASEDIR}/${MergeTrainsOutname}
				mkdir -p ${BASEDIR}/${MergeTrainsOutname}/$RunlistName
				DirMerged=${BASEDIR}/${MergeTrainsOutname}/$RunlistName
				if [[ ! -f $DirMerged/$filename ]]; then
					redomerging=1
				fi
				if [[ $redomerging = 1 ]]; then
					new_rm $Dirout2/.${filename%%.root}.merged
				fi
				doMergeFiles $filename $Dirout2 $Dirin $DirMerged $MergeTrainsOutname
			done
		done
	done
}
function doMergeFiles() {
	if [[ $debug = 1 ]] || [[ $debug = 2 ]] || [[ $debug = 3 ]]; then
		echo -e "\e[33mdoMergeFiles(): \e[0m from:$2/$1  to:$4/$1" | tee -a $LogFile
	fi
	downlogFile="$2/${1%%.root}.downlog"
	mergedFile=$4/$1
	if [ "$#" == "5" ]; then
		alreadyMerged="$2/.${1%%.root}_$5.merged"
	else
		alreadyMerged="$2/.${1%%.root}.merged"
	fi
	logFile=$2/.${1%%.root}.log
	new_rm $logFile
	new_rm $mergedFile.tmp
	if [[ -f $alreadyMerged ]]
	then
		for filesalreadymerged in `cat $alreadyMerged`; do
			if [[ $filesalreadymerged = $mergedFile ]]; then
				printf "\e[33m|-> \e[0m already merged\n" | tee -a $LogFile
				return
			fi
		done
	fi
	if [[ ! -f $2/$1 ]]
	then
		if [[ $debug = 1 ]] || [[ $debug = 2 ]]
		then
			echo -e "\t\e[31m|->\e[0m File missing $2/$1  "  | tee -a $ErrorLog | tee -a $LogFile
		else
			echo -e "\t\e[31m|->\e[0m File missing "  | tee -a $ErrorLog | tee -a $LogFile
		fi
		return
	fi
	if [[ ! -f $mergedFile ]]
	then
		echo -e "\e[33m|->\e[0m Copy: is first" | tee -a $LogFile
		echo "Copyed to $mergedFile" > $logFile | tee -a $LogFile
		cp $2/$1 $mergedFile
		# touch $alreadyMerged
		echo $mergedFile >> $alreadyMerged
		return
	fi
	printf "\e[33m|->\e[0m merging Files \n"  #(log: $logFile)" | tee -a $LogFile
	if [[ $debug = 2 ]] || [[ $debug = 3 ]]; then
		echo "hadd -k $mergedFile.tmp $mergedFile $2/$1  &> $logFile"
		hadd -k $mergedFile.tmp $mergedFile $2/$1  | tee $logFile
	else
		hadd -k $mergedFile.tmp $mergedFile $2/$1  &> $logFile
	fi
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
		if [[ ! $3 -eq "0"  ]]; then
			new_rm $2/$1
			GetFile $3/$1 $2/$1 $downlogFile
		fi
		printf "  \e[33m|->\e[0m merging " | tee -a $LogFile
		hadd -k $mergedFile.tmp $mergedFile $2/$1  &> $logFile
		exitstatus=$?
		((tmp++))
	done
	if [[ ! "$exitstatus" = "0" ]]
	then
		echo -e "\e[31mError\e[0m $2/$1 not merged correctly" | tee -a $ErrorLog | tee -a $LogFile
		cat $logFile >> $ErrorLog
		new_rm $mergedFile.tmp
	else
		new_rm $mergedFile
		mv $mergedFile.tmp $mergedFile
		# touch $alreadyMerged
		echo $mergedFile >> $alreadyMerged
	fi
}
function SeachFilesOnAlienAndAddToList(){
	if [[ $debug = 1 ]] || [[ $debug = 2 ]]
	then
		echo;
		echo "SeachFilesOnAlienAndAddToList($1,$2):" | tee -a $LogFile
	fi
	cmd='alien_ls $1 2> /dev/null > tmp.txt'
	if [[ $debug = 1 ]] || [[ $debug = 2 ]]
	then
		echo "alien_ls $1 2> /dev/null > tmp.txt" | tee -a $LogFile
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
	for SearchTMP in `cat $FilenamesinTrain`
	do
		cmd='cat tmp.txt | grep "$SearchTMP" > $2.tmp'
		if [[ $debug = 1 ]] || [[ $debug = 2 ]]
		then
			echo "cat tmp.txt | grep "$SearchTMP" > $2.tmp" | tee -a $LogFile
		fi
		eval $cmd
		# usecmd $cmd
		AddToList $2.tmp $2
		new_rm $2.tmp
	done
	new_rm  tmp.txt
}
function FindPathsAndAddToListnew(){
	# printf "\e[36m--  FindPathsAndAddToList \e[0m" | tee -a $LogFile
	# if [[ ! -f $2 ]]  || [ "$(( $(date +"%s") - $(stat -c "%Y" $2) ))" -gt "604800" ]; then  # 86400 = 1 Day
		cmd="alien_find $1 $3  2> /dev/null  > $2.tmp"

		if [[ $debug = 1 ]] || [[ $debug = 2 ]]
		then
			echo;
			echo "$cmd" | tee -a $LogFile
		fi
		tmpcouuntnew=1
		# echo "$cmd" | tee -a $LogFile
		eval $cmd
		tmpreturn=$?
		if [[ ! -f $2.tmp ]]; then
			tmpreturn="0"
		fi
		if [[ -f "$2.tmp" ]]; then
			if [[ `cat "$2.tmp" | wc -l`  < 1 ]]; then
				tmpreturn="0"
			fi
		fi
		if [[ ! "$tmpreturn" = "0" ]]; then
			printf "  \e[33m|->\e[0m Retry " | tee -a $LogFile
			eval $cmd
			tmpreturn=$?
		fi
		if [[ $debug = 1 ]] || [[ $debug = 2 ]]
		then
			if [[ -f "$2.tmp" ]]; then
				cat "$2.tmp"
			fi
		fi
		if [[ ! -f $2.tmp ]]; then
			tmpreturn="0"
		fi
		if [[ -f "$2.tmp" ]]; then
			if [[ `cat "$2.tmp" | wc -l`  < 1 ]]; then
				tmpreturn="0"
			fi
		fi
		if [[ ! "$tmpreturn" = "0" ]]; then
			# sed -n '/files/!p' $2.tmp >  $2.tmp
			AddToList $2.tmp $2
			new_rm $2.tmp
		fi
	# fi
}
function FindPathsAndAddToList(){
	# printf "\e[36m--  FindPathsAndAddToList \e[0m" | tee -a $LogFile
	new_rm $2.tmp
	# if [[ ! -f $2 ]]  || [ "$(( $(date +"%s") - $(stat -c "%Y" $2) ))" -gt "604800" ]; then  # 86400 = 1 Day
		# if [[ $isjalien = 1 ]]; then
		# 	cmd="alien.py find $1 $3  2> /dev/null | grep $TrainPage/$TrainNumber | grep child_$childID/ > $2.tmp"
		# else
		# echo $childID
		if (( `echo -n $childID | wc -c` > 3 )); then
			cmd="alien_find $1 $3  2> /dev/null | grep $TrainPage/$TrainNumber | grep $childID/ > $2.tmp"
		else
			cmd="alien_find $1 $3  2> /dev/null | grep $TrainPage/$TrainNumber | grep child_$childID/ > $2.tmp"
		fi
		if [[ $debug = 1 ]] || [[ $debug = 2 ]]
		then
			echo;
			echo "$cmd" | tee -a $LogFile
		fi
		tmpcouuntnew=1
		eval $cmd
		tmpreturn=$?
		if [[ ! -f $2.tmp ]]; then
			tmpreturn="0"
		fi
		if [[ -f $2.tmp ]]; then
			if [[ `cat $2.tmp | wc -l`  < 1 ]]; then
				tmpreturn="0"
			fi
		fi
		if [[ ! "$tmpreturn" = "0" ]]; then
			printf "  \e[33m|->\e[0m Retry " | tee -a $LogFile
			eval $cmd
			tmpreturn=$?
		fi
		# else
		if [[ ! -f $2.tmp ]]; then
			tmpreturn="0"
		fi
		if [[ -f $2.tmp ]]; then
			if [[ `cat $2.tmp | wc -l`  < 1 ]]; then
				tmpreturn="0"
			fi
		fi
		if [[ "$tmpreturn" = "0" ]]; then
			if [[ $debug = 1 ]] || [[ $debug = 2 ]]
			then
				cat $2.tmp
			fi
			# sed -n '/files/!p' $2.tmp >  $2.tmp
			AddToList $2.tmp $2
			new_rm $2.tmp
		# fi
		else
			# /alice/data/2016/LHC16d/000252375/pass1/AOD208/PWGGA/GA_pp_AOD/1549_20200925-1736_child_1/0016/GammaConvCalo_2001.root
		# if [[ ! "$tmpreturn" = "0" ]] || [[ ! -f $2.tmp ]] || [[ `cat $2.tmp | wc -l`  < 1 ]]; then
			printf "  -- alien_find didn't work trying runwise -- \n" | tee -a $LogFile
			for runName in `cat $4`; do

				tmprunfilename=${2%AllPathList*}$5$runName
				new_rm $tmprunfilename.tmp
				touch $tmprunfilename.tmp
				# echo $tmprunfilename
				if [[ ! -f "$tmprunfilename.txt" ]]; then
					tmpdirusing="$1$5$runName"
					if (( `echo -n $childID | wc -c` > 3 )); then
						cmd="alien_find $tmpdirusing $3  2> /dev/null | grep $TrainPage/$TrainNumber | grep $childID/ > $tmprunfilename.tmp"
					else
						cmd="alien_find $tmpdirusing $3  2> /dev/null | grep $TrainPage/$TrainNumber | grep child_$childID/ > $tmprunfilename.tmp"
					fi
					if [[ $debug = 1 ]] || [[ $debug = 2 ]]
					then
						echo "$runName: $cmd" | tee -a $LogFile
					else
						printf "$runName, " | tee -a $LogFile
					fi
					tmpcouuntnew=1
					eval $cmd
					if [[ $debug = 1 ]] || [[ $debug = 2 ]]
					then
						cat $tmprunfilename.tmp
					fi
					tmpreturn=$?
					retried=0
					if [[ ! -f $2.tmp ]]; then
						tmpreturn="0"
					fi
					if [[ -f $2.tmp ]]; then
						if [[ `cat $2.tmp | wc -l`  < 1 ]]; then
							tmpreturn="0"
						fi
					fi
					if [[ ! "$tmpreturn" = "0" ]]; then
						retried=1
						printf "\n $runName  \e[33m|->\e[0m Retry " | tee -a $LogFile
						eval $cmd
						tmpreturn=$?
					fi
					while [[ ! $tmpreturn = "0" ]] || [[ `cat $tmprunfilename.tmp | wc -l`  < 1 ]]; do
						if [[ $tmpcouuntnew > 4 ]]; then break; fi
						sleep 1
						printf "${tmpcouuntnew} " | tee -a $LogFile
						# printf "." | tee -a $LogFile
						((tmpcouuntnew++))
						eval $cmd
						tmpreturn=$?
					done
					if [[ $retried = 1 ]]; then
						echo
					fi
					if [[ ! "$tmpreturn" = "0" ]] || [[ `cat $2.tmp | wc -l`  < 1 ]]; then
						continue
					fi
					# cat "$tmprunfilename.tmp"
					# sed -n '/files/!p' $2.tmp >  $2.tmp
					AddToList "$tmprunfilename.tmp" "$tmprunfilename.txt"
					new_rm "$tmprunfilename.tmp"
				fi
				AddToList "$tmprunfilename.txt" "$2"
			done
		fi
		echo;
	# else
	# 	printf "  \e[33m|->\e[0m is up to date \n" | tee -a $LogFile
	# fi
}
function GrapPathsAndAddToList(){
	grep "$1" "$3" > "$2.tmp"
	AddToList "$2.tmp" "$2"
	new_rm "$2.tmp"
}
function FindAllRunsAndAddToList(){
	maxtmp=1
	if [[ $OptIsJJ = 1 ]]; then
		maxtmp=21
	fi

	for (( i = 1; i < $maxtmp; i++ )); do
		# if [[ $isjalien = 1 ]]; then
		# 	cmd="alien.py ls /alice/$Type/$Year/$ChildName 1> tmprunfolder.txt"
		# else
			cmd="alien_ls /alice/$Type/$Year/$ChildName 1> tmprunfolder.txt"
		# fi
		if [[ $OptIsJJ = 1 ]]; then
			# if [[ $isjalien = 1 ]]; then
			# 	cmd="alien.py ls /alice/$Type/$Year/$ChildName/$i 1> tmprunfolder.txt"
			# else
				cmd="alien_ls /alice/$Type/$Year/$ChildName/$i 1> tmprunfolder.txt"
			# fi
		fi
		# echo "/alice/$Type/$Year/$ChildName/$i"
		if [[ $debug = 1 ]] || [[ $debug = 2 ]]
		then
			echo $cmd | tee -a $LogFile
		fi
		eval $cmd
		tmpcouuntnew=0
		while [[ ! $? = "0" ]] ; do
			if [[ $tmpcouuntnew > 10 ]]; then return; fi
			((tmpcouuntnew++))
			eval $cmd
			printf "."
			sleep 1
		done


		new_rm tmprunfolder.txt.tmp
		for TMPrun in `cat tmprunfolder.txt`; do
			echo ${TMPrun%/} >> tmprunfolder.txt.tmp
			# printf "${TMPrun%/}, "
		done
		new_rm tmprunfolder.txt
		mv tmprunfolder.txt.tmp tmprunfolder.txt


		if [[ `grep "does not exist" tmprunfolder.txt | wc -l` < 1 ]] ; then
			# cat tmprunfolder.txt
			AddToList tmprunfolder.txt $1
			new_rm tmprunfolder.txt
			break
		fi
	done
}


function PrepMerge() {
	# printf "\e[33m|->\e[0m PrepMerge() " | tee -a $LogFile
	if [ "$#" == "5" ]; then
		additionalargument=$5
	else
		additionalargument=""
	fi
	FileForAllMergeProcesses="$4/.FileForAllMergeProcesses.txt"
	FileForAllMergeProcessesSavedDetails="$4/.FileForAllMergeProcessesSavedDetails.txt"
	if [[ -f $FileForAllMergeProcessesSavedDetails ]]; then
		if [[ `grep "$1 $2 $3 $4 $additionalargument" $FileForAllMergeProcessesSavedDetails | wc -l` < 1 ]]; then
			echo "$1 $2 $3 $4 $additionalargument" >> $FileForAllMergeProcessesSavedDetails
		else
			printf "\e[33m|-> \e[0m already added\n" | tee -a $LogFile
			return
		fi
	else
		echo "$1 $2 $3 $4 $additionalargument" >> $FileForAllMergeProcessesSavedDetails
	fi
	downlogFile="$2/${1%%.root}.downlog"
	mergedFile=$4/$1
	if [ "$#" == "5" ]; then
		alreadyMerged="$2/.${1%%.root}_$5.merged"
	else
		alreadyMerged="$2/.${1%%.root}.merged"
	fi
	logFile=$2/.${1%%.root}.log
	new_rm $logFile
	new_rm $mergedFile.tmp
	if [[ -f $alreadyMerged ]]
	then
		for filesalreadymerged in `cat $alreadyMerged`; do
			if [[ $filesalreadymerged = $mergedFile ]]; then
				printf "\e[33m|-> \e[0m already merged\n" | tee -a $LogFile
				return
			fi
		done
	fi
	if [[ ! -f $2/$1 ]]
	then
		if [[ $debug = 1 ]] || [[ $debug = 2 ]]
		then
			echo -e "\t\e[31m|->\e[0m File missing $2/$1  "  | tee -a $ErrorLog | tee -a $LogFile
		else
			echo -e "\t\e[31m|->\e[0m File missing "  | tee -a $ErrorLog | tee -a $LogFile
		fi
		return
	fi
	if [[ ! -f $mergedFile ]]
	then
		echo -e "\e[33m|->\e[0m Copy: is first" | tee -a $LogFile
		echo "Copyed to $mergedFile" > $logFile | tee -a $LogFile
		cp $2/$1 $mergedFile
		# touch $alreadyMerged
		echo $mergedFile >> $alreadyMerged
		return
	fi
	if [[ -f $FileForAllMergeProcesses ]]; then
		if [[ `grep "hadd -k $mergedFile.tmp" $FileForAllMergeProcesses | wc -l` < 1 ]] ; then
			printf "\e[33m|-> \e[0m Add file for merging\n" | tee -a $LogFile
			if [[ $debug = 1 ]] || [[ $debug = 2 ]] || [[ $debug = 3 ]] || [[ $debug = 4 ]]; then
				printf "\t\e[33m|-> \e[0m from:$2/$1  to:$4/$1" | tee -a $LogFile
			fi
			echo "hadd -k $mergedFile.tmp $mergedFile $2/$1  &> $logFile" >> $FileForAllMergeProcesses
			# cat $FileForAllMergeProcesses
		else
			new_rm $FileForAllMergeProcesses.tmp
			# cat $FileForAllMergeProcesses
			# for lineTmp in `cat $FileForAllMergeProcesses`; do
			while IFS= read -r lineTmp; do
				# if [[ $debug = 1 ]] || [[ $debug = 2 ]] || [[ $debug = 3 ]]; then
					# printf "\e[33m|-> \e[0m $lineTmp" | tee -a $LogFile
				# fi
				# echo "|$lineTmp|"
				if [[ `echo "$lineTmp" | grep "hadd -k $mergedFile.tmp" | wc -l` < 1 ]]; then
					echo "$lineTmp" >> $FileForAllMergeProcesses.tmp
				else
					printf "\e[33m|-> \e[0m Add file to list for merging " | tee -a $LogFile
					if [[ $debug = 1 ]] || [[ $debug = 2 ]] || [[ $debug = 3 ]] || [[ $debug = 4 ]]; then
						printf "\t\e[33m|-> \e[0m from:$2/$1  to:$4/$1" | tee -a $LogFile
					fi
					printf "\n" | tee -a $LogFile
					logFile=$2/.${1%%.root}.log
					echo "${lineTmp%\&\>*} $2/$1 &> ${lineTmp#*\&\>}" >> $FileForAllMergeProcesses.tmp
					# cat $FileForAllMergeProcesses.tmp
				fi
			done < "$FileForAllMergeProcesses"
			# done
			mv $FileForAllMergeProcesses.tmp $FileForAllMergeProcesses
			# cat $FileForAllMergeProcesses
		fi
	else
		printf "\e[33m|-> \e[0m Add first file for merging\n" | tee -a $LogFile
		if [[ $debug = 1 ]] || [[ $debug = 2 ]] || [[ $debug = 3 ]] || [[ $debug = 4 ]]; then
			printf "\t\e[33m|-> \e[0m from:$2/$1  to:$4/$1" | tee -a $LogFile
		fi
		echo "hadd -k $mergedFile.tmp $mergedFile $2/$1  &> $logFile" > $FileForAllMergeProcesses
		# cat $FileForAllMergeProcesses
	fi
	# echo;
}

function StartMergeProcess() {
	if [[ ! -e $FileForAllMergeProcesses ]]; then
		# if [[ $debug = 1 ]] || [[ $debug = 2 ]] || [[ $debug = 3 ]]; then
			printf " \e[33m|->\e[0m no merging needed\n" | tee -a $LogFile
		# else
			# echo;
		# fi
		return
	fi
	printf "\e[33m|->\e[0m StartMergeProcess  " | tee -a $LogFile
	if [[ $debug = 1 ]] || [[ $debug = 2 ]] || [[ $debug = 3 ]]; then
		printf -e "\e[33m|->\e[0m from:$4" | tee -a $LogFile
	fi
	FileForAllMergeProcesses="$1/.FileForAllMergeProcesses.txt"
	FileForAllMergeProcessesSavedDetails="$1/.FileForAllMergeProcessesSavedDetails.txt"
	# cat $FileForAllMergeProcesses
	IFS=$'\n'       # make newlines the only separator
	for processinline in $(cat $FileForAllMergeProcesses); do
		if [[ $debug = 1 ]] || [[ $debug = 2 ]] || [[ $debug = 3 ]]; then
			printf "\e[33m|->\e[0m $processinline" | tee -a $LogFile
		fi
		# echo;
		# printf "\e[33m|->\e[0m $processinline \n" | tee -a $LogFile
		eval "$processinline"  | tee -a $LogFile

		tmp=1
		exitstatus=$?
		if [[ ! "$exitstatus" = "0" ]]; then
			printf "  \e[33m|->\e[0m Retry " | tee -a $LogFile
		fi
		while [[ ! "$exitstatus" = "0" ]]; do
			printf "." | tee -a $LogFile
			if [[ $tmp = 6 ]]; then
				printf "  \e[33m|->\e[0m Retry with original processes\n" | tee -a $LogFile
				while IFS=" " read -r value1 value2 value3 value4 value5 remainder
				do
					if [[ `echo $processinline | grep "$value2/$value1" | grep "$value4/$value1" | wc -l` = 1 ]]; then
						echo -e "\e[33mdoMergeFiles(): \e[0m from:$value2/$value1  to:$value4/$value1" | tee -a $LogFile
						doMergeFiles $value1 $value2 $value3 $value4 $value5
					fi
				done < "$FileForAllMergeProcessesSavedDetails"
				break
			fi
			eval $processinline >> $LogFile
			exitstatus=$?
			((tmp++))
		done
		if [[ "$exitstatus" = "0" ]]; then
			printf "  \e[33m|->\e[0m Done " | tee -a $LogFile
			while IFS=" " read -r value1 value2 value3 value4 value5 remainder
			do
				if [[ `echo $processinline | grep "$value2/$value1" | grep "$value4/$value1" | wc -l` = 1 ]]; then
					mergedFile=$value4/$value1
					alreadyMerged="$value2/.${value1%%.root}_$value5.merged"
					if [[ $value5 = "" ]]; then
						alreadyMerged="$value2/.${value1%%.root}.merged"
					fi
					if [[ $debug = 1 ]] || [[ $debug = 2 ]] || [[ $debug = 3 ]]; then
						printf "\n\t\t  \e[33m|->\e[0m merged $mergedFile " | tee -a $LogFile
					fi
					if [[ $debug = 2 ]] || [[ $debug = 3 ]]; then
						printf " (->$alreadyMerged)" | tee -a $LogFile
					fi
					echo $mergedFile >> $alreadyMerged
				fi
			done < "$FileForAllMergeProcessesSavedDetails"
		fi
		echo;
	done
	unset IFS

	new_rm $FileForAllMergeProcesses
	new_rm $FileForAllMergeProcessesSavedDetails
}
