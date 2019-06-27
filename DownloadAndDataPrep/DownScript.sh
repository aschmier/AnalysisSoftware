#! /bin/bash
debug=0
ErrorLog=""
WARNINGLog=""
# This Script is intended to automize the download of train outputs

# Version: V3.3
echo  -e "\e[36m+++++++++++++++++++++++++++++++++++++\e[0m"
echo "DownScript.sh Version: V4.3"

# Author: Adrian Mechler (mechler@ikf.uni-frankfurt.de)

# to use this you have to download 2 packages html-xml-utils and w3m
# additional you need an alien token
# and you have to enter the path to your certificat at line ~120

# Use: bash DownScript.sh [TrainPage] [Trainnumber] -[OPTIONS]

# actually all inputs are also asked interactive in case you forget something

# 	|-> TrainPage (e.g. GA_pp_AOD)
# 	|-> TrainNumber (e.g 768)
#                            YOU CAN SELECT multiple trains at once e.g. "768 775"

#####################################

# OPTIONS:
# 	|-> OutName (is optional)
# 	|-> Search (search for string in filename to be downloaded e.g GammaCalo, default is .root)
#							 YOU CAN SELECT multiple at once e.g. "?_GammaCalo ?_GammaConv"
# 	|-> OptRunlistName (select Runlist to be downloaded e.g. listDPGCalo, default is all availible)
#                            YOU CAN SELECT multiple at once e.g. "-RL_listDPGCalo -RL_listDPGEDC"
# 	|-> mergechilds (merges all childs)
# 	|-> runwise (download all runs)
# 	|-> childsareperiods
# 	|-> nodown
# 	|-> debug
# 	|-> newfiles

# Examlple: bash DownScript_V2.sh 776 GA_pp_AOD -GammaConvCalo -RL_listDPGCalo -RL_listDPGEDC -mergechilds -childsareperiods -runwise

#####################################
#####################################

# we need a certificat to get info from alimonitor
pathtocert=""
UserName=""
alienUserName=""
BASEDIR=${PWD}
thisuser=`echo ${USER}`
if [[ $thisuser = "adrian" || $thisuser = "amechler" ]]
then
	BASEDIR="/media/adrian/Elements/grid_data"
	FrameworkDir="/home/adrian/git/AnalysisSoftware"
	UserName="Adrian Mechler";
	pathtocert="~/.globus"
	alienUserName="amechler"
	key="key.pem"
	cacert="ca.pem"
	clientca="client.pem"
else
	echo
	echo -e "This feature is not supported for user:$thisuser."
	echo -e "You have to add your certificat path to folder."
	echo
	exit
fi
# printf "++++++++++++++\n  $UserName\n++++++++++++++\n"

# cd $BASEDIR


#####################################
#####################################

function usecmd()
{
	if [[ $debug = 1 ]] || [[ $debug = 2 ]]
	then
		echo $1
	fi
}


# Fuction to check if file is up to date and download it if not
function GetFile_check()
{
	if [[ $debug = 1 ]] || [[ $debug = 2 ]]
	then
	echo -e "\e[33mGetFile(): \e[0m alien:/$1  file:/$2"
	fi
	if [ -f $2 ]
	then
		# Check if older than one day (or not)
		if [ "$(( $(date +"%s") - $(stat -c "%Y" $2) ))" -gt "86400" ]
		then
			echo -e "\e[33m|-> \e[0mFile $2 exists. \e[33m|-> \e[0mbut is older then 1 day! download new one from alien:/$1"
			alien_cp alien:$1 file:$2 | tee -a $3
			touch "$2";
		else
			echo -e "\e[33m|-> \e[0mFile $2 exists. \e[33m|-> \e[0mand is up-to-date"
		fi
	else
		echo -e "\e[33m|-> \e[0mDownloading file alien:$1"
		alien_cp alien:$1 file:$2 | tee -a $3
		touch "$2";
	fi
}


function GetWebpage()
{
	if [[ $debug = 1 ]] || [[ $debug = 2 ]]
	then
		echo -e "\e[33mGetWebpage(): \e[0m\t$1\t$2\t$3"
	fi
	certpath="$3"
	Webpage="$2"

	# we download the html to grep needed information from there
	if [ -f $1 ]
	then # Check if older than one day (or not)
		if [[ `wc -l $1` < 100 ]]
		then
			rm $1
		fi
		if [ "$(( $(date +"%s") - $(stat -c "%Y" $1) ))" -gt "86400" ]
		then
			echo -e "\e[33m|-> \e[0mFile $1 exists. \e[33m|-> \e[0mbut is older then 1 Day! download new one from alien:/$1"
			# cmd="curl -s '${Webpage}' --key $certpath/$key --cacert $certpath/$cacert --cert $certpath/$clientca &> $1"
			cmd="curl '${Webpage}' --key $certpath/$key -k --cert $certpath/$clientca &> $1"
			eval $cmd
			usecmd $cmd
			touch "$1";
		else
			echo -e "\e[33m|-> \e[0mFile $1 exists. \e[33m|-> \e[0mand is up-to-date"
		fi
	else
		echo -e "\e[33m|-> \e[0mDownloading ${Webpage}"
		# cmd="curl '${Webpage}' --key $certpath/$key --cacert $certpath/$cacert --cert $certpath/$clientca &> $1"
		cmd="curl '${Webpage}' --key $certpath/$key -k --cert $certpath/$clientca &> $1"
		eval $cmd
		usecmd $cmd
		touch "$1";
	fi
}
# # if this is not working try
# openssl pkcs12 -in myCertificate.p12 -out newcert.pem
# openssl pkcs12 -in myCertificate.p12 -out $cacert -cacerts -nokeys
# openssl pkcs12 -in myCertificate.p12 -out $clientca -clcerts -nokeys
# openssl pkcs12 -in myCertificate.p12 -out $key -nocerts



# Fuction to check if file is there and download it if not
function GetFile()
{
	if [[ $debug = 1 ]] || [[ $debug = 2 ]]; then
		echo -e "\e[33mGetFile(): \e[0m alien:/$1  file:/$2"
	fi

	if [[ -f $2 ]] # Check if file there
	then
		if [[ $debug = 1 ]] || [[ $debug = 2 ]]; then
			echo -e "\e[33m|-> \e[0mFile $2 exists."
		else
			printf "\e[33m|-> \e[0mFile exists."
		fi
	else
		if [[ `alien_ls $1 2> /dev/null | grep "no such file or directory" | wc -c` -eq 0 ]]; then
			if [[ $debug = 1 ]] || [[ $debug = 2 ]]; then
				echo -e "\e[33m|-> \e[0mDownloading file alien:/$1"
				alien_cp -o  alien:/$1 file:/$2 | tee -a $3
			else
				printf "\e[33m|-> \e[0mDownloading file"
				alien_cp -o  alien:/$1 file:/$2 &> $3
			fi
			tmp=1
			if [[ ! -f $2 ]]; then
				printf "  \e[33m|->\e[0m Retry "
			fi
			while [[ ! -f $2 ]]; do
				if [[ $tmp = 11 ]]; then
					echo "."
					break
				fi
				printf "${tmp} "
				alien_cp -o  alien:/$1 file:/$2 &> $3
				((tmp++))
			done
			if [[ -f $2 ]]; then
				printf "  \e[33m|->\e[0m successful"
			else
				printf "  \e[33m|->\e[0m Download failed"  | tee -a $3  | tee -a $ErrorLog
			fi
		else
			if [[ $debug = 1 ]] || [[ $debug = 2 ]]
			then
				echo -e "\e[31m|->\e[0m missing $1 on alien"  | tee -a $3  | tee -a $ErrorLog
			else
				echo -e "\e[31m|->\e[0m missing on alien"  | tee -a $3  | tee -a $ErrorLog
			fi
		fi
	fi
}


function AddToList()
{
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
	if [[ $debug = 1 ]] || [[ $debug = 2 ]]
	then
		echo "cat $2"
		cat $2
	fi
}



#####################################
#####################################

echo;echo;

echo  -e "\e[36m------------------------------------\e[0m"
echo -e "Using settings for: $UserName"
echo  -e "\e[36m------------------------------------\e[0m"

# chack if valid token is active
if [[ `alien-token-info` =~ "No Token found!" ]]
then
	echo -e "\e[31mWARNING:\e[0m No alien token"
	alien-token-init $alienUserName
	echo  -e "\e[36m------------------------------------\e[0m"
	echo;echo;
fi


################
#There are Settings to be used beforehand
TrainPage=""
TrainPageNum=""
TrainNumber=""
MergeTrains=0
DoDown=1
MergeTrainsOutname=0
OutName=""
Search=".root"
OptRunlistName=list
OptRunlistNameSet=0
OptAllRunlists=0
UseMerge=0
MergeTrains=0
MultiTrains=0
Userunwise=0
SetOutName=""
Usechildsareperiods=0
newfiles=0
re='^[0-9]+$'

RunningScripts=0
while [[ -f OptRunlistNames_$RunningScripts.txt ]]; do
	((RunningScripts++))
done
OptRunlistNamefile=OptRunlistNames_$RunningScripts.txt
TrainNumberFile=TrainNumberFile_$RunningScripts.txt
Searchfile=Searchfile_$RunningScripts.txt

job=$$
lockfile=$job.lock
for Process in `ls *.lock | grep -v $job` ; do
	tmpjob=${Process%.lock}
	status=`ps -efww | grep -w "DownScript.sh" | grep -v grep | grep $tmpjob | awk '{ print $2 }'`
	if [ -z "$status" ]; then
		for tmp in `cat $Process` ; do
			if [[ -f OptRunlistNames_$tmp.txt ]]; then rm OptRunlistNames_$tmp.txt; fi
			if [[ -f TrainNumberFile_$tmp.txt ]]; then rm TrainNumberFile_$tmp.txt; fi
			if [[ -f Searchfile_$tmp.txt ]]; then rm Searchfile_$tmp.txt; fi
		done
		rm $Process
	fi
done
echo "$RunningScripts" > $lockfile



if [[ -f $TrainNumberFile ]]
then
	rm $TrainNumberFile
fi
if [[ -f $Searchfile ]]
then
	rm $Searchfile
fi
if [[ -f $OptRunlistNamefile ]]
then
	rm $OptRunlistNamefile
fi

for setting in "$@"
do
	# echo $setting
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
	elif [[ $setting = "-mergetrains" ]]
	then
		MergeTrains=1
	elif [[ $setting = "-runwise" ]]
	then
		Userunwise=1
	elif [[ $setting = "-childsareperiods" ]]
	then
		Usechildsareperiods=1
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

if [[ ! -f $OptRunlistNamefile ]]
then
	OptAllRunlists=1
fi
################
# Required Settings can also be Set interactive
if [[ $TrainPage = "" ]]
then
	echo -e "\e[33m|-> \e[0m TrainPage not set! What shall be used? GA_pp, GA_pp_MC, GA_pp_AOD, GA_pp_MC_AOD"
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
else
	echo -e "\e[31m|-> $TrainPage\e[0m not supported"
fi
if [[ $TrainNumber = "" ]]
then
	echo -e "\e[33m|-> \e[0m TrainNumber not set! What shall be used? "
	read TrainNumber
fi

##################
# Print Settings
OutName=.$TrainPage-$TrainNumber
if [[ $SetOutName = "" ]]
then
	# SetOutName=$OutName
	# echo -e "\e[33m|-> \e[0m OutName not set! using TrainNumber: OutName = $OutName"
	echo -e "\e[33m|-> \e[0m OutName = $OutName"
else
	echo -e "\e[33m|-> \e[0m OutName = $SetOutName"
fi
if [[ $MultiTrains = 0 ]]
then
	echo -e "\e[33m|-> \e[0m TrainNumber = $TrainNumber"
elif [[ $MultiTrains = 1 ]]
then
	echo -e "\e[33m|-> \e[0m TrainNumber set: $TrainNumber"
	MergeTrainsOutname=$BASEDIR/$SetOutName
fi
echo -e "\e[33m|-> \e[0m TrainPage = $TrainPage"
echo -e "\e[33m|-> \e[0m Search = $Search"
echo -e "\e[33m|-> \e[0m RunlistName = $OptRunlistName"
# echo -e "\e[33m|-> \e[0m Usechildsareperiods = $Usechildsareperiods"
if [[ $DoDown = 0 ]]
then
	echo -e "\e[33m|-> \e[0m Downloading disabled"
else
	if [[ $UseMerge = 1 ]]
	then
		echo -e "\e[33m|-> \e[0m Merge files"
	fi
	if [[ $Usechildsareperiods = 1 ]]
	then
		echo -e "\e[33m|-> \e[0m Childs are periods"
	fi
fi
echo  -e "\e[36m------------------------------------\e[0m"
# echo -e "\e[33mIs this Setup correct?\e[0m (y/n)"
# printf "Answer: "
# read -e setupcorrect
# if [ ! "$setupcorrect" = "y" ]
# then
#   echo ""
#   echo "please try again   :-("
#   echo ""
#   exit
# fi
# echo  -e "\e[36m------------------------------------\e[0m"
echo ""
echo ""
echo ""

echo ""

if [[ $DoDown = 1 ]]
then
	for TrainNumber in `cat $TrainNumberFile`
	do

		OutName=.$TrainPage-$TrainNumber
		AlienDir="/alice/cern.ch/user/a/alitrain/PWGGA/$TrainPage/"

		ErrorLog="$BASEDIR/$OutName/Errors.log"
		WARNINGLog="$BASEDIR/$OutName/Warnings.log"
		if [[ -f $ErrorLog ]]; then rm $ErrorLog; fi
		if [[ -f $WARNINGLog ]]; then rm $WARNINGLog; fi

		OUTPUTDIR=$BASEDIR/$OutName
		List="ListGrid.txt"
		GlobalVariablesFile="globalvariables.C"
		envFile="$OUTPUTDIR/.env.sh"

		TrainPageHTML="$BASEDIR/.$TrainPage.html"

		trainwebpage="https://alimonitor.cern.ch/trains/train.jsp?train_id=${TrainPageNum}"
		PeriodName=""

		echo;echo;echo;echo;
		echo  -e "\e[36m~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\e[0m"
		echo -e "\e[36m |-> \e[0m OutName = $OutName\e[36m | \e[0mTrainNumber = $TrainNumber\e[36m | \e[0mTrainPage = $TrainPage\e[36m | \e[0mSearch = $Search\e[36m | \e[0mRunlistName = $OptRunlistName\e[36m | \e[0mUsechildsareperiods = $Usechildsareperiods\e[36m | \e[0mUseMerge = $UseMerge"
		echo  -e "\e[36m~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\e[0m"
		echo;echo;

		#####################################
		#####################################





		# prepare directory
		mkdir -p $OUTPUTDIR

		#####################################
		#####################################

		echo
		GetWebpage $TrainPageHTML $trainwebpage $pathtocert
		echo
		echo  -e "\e[36m------------------------------------\e[0m"
		echo

		# get all childs involved in train run
		alien_ls $AlienDir 2> /dev/null | grep $TrainNumber\_2 &> $List


		# get one env.sh to get basic information about the train
		Childeone=`sed '2q;d' $List`
		echo -e "\e[33m|->\e[0m $Childeone"
		if [[ $debug = 1 ]] || [[ $debug = 2 ]]
		then
			GetFile $AlienDir$Childeone/env.sh $envFile /dev/null
		else
			GetFile $AlienDir$Childeone/env.sh $envFile /dev/null &> /dev/null
		fi
		PERIODNAME=`grep "PERIOD_NAME=" $envFile | awk -F "=" '{print $2}' | awk -F ";" '{print $1}'`
		echo "PERIODNAME = $PERIODNAME"
		NCHilds=`grep "CHILD_DATASETS=" $envFile | awk -F "=" '{print $2}' | awk -F ";" '{print $1}'`
		echo "NCHilds = $NCHilds"
		echo



		#####################################
		#####################################


		# Loop over all childs
		for child in `sort ${List}`;
		do
			echo
			echo  -e "\e[35m=====================================\e[0m"
			echo
			echo -e "\e[35m $child \e[0m"

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

			# prepare directory
			mkdir -p "$OUTPUTDIR/.$child"

			# get one globalvariables.C to get information about the child
			GlobalVariablesPath="$OUTPUTDIR/.$child/$GlobalVariablesFile"
			if [[ $debug = 1 ]] || [[ $debug = 2 ]]
			then
				GetFile "$AlienDir$child/$GlobalVariablesFile" "$GlobalVariablesPath" /dev/null
			else
				GetFile "$AlienDir$child/$GlobalVariablesFile" "$GlobalVariablesPath" /dev/null &> /dev/null
			fi

			ChildName=`grep "periodName =" "$GlobalVariablesPath" | awk -F "= " '{print $2}' | awk -F ";" '{print $1}' | awk -F "\"" '{print $2}'`
			if [[ $ChildName = "" ]]; then ChildName=`grep "periodName =" "$GlobalVariablesPath" | awk -F "=" '{print $2}' | awk -F ";" '{print $1}' | awk -F "\"" '{print $2}'`; fi
			echo -e " periodName = $ChildName"


			#####################################
			#####################################


			if [[ `wc -l $TrainPageHTML` < 1 ]]
			then
				echo -e "\e[31mError\e[0m $TrainPageHTML not found! aborting" | tee -a $ErrorLog
				continue
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
						found=1
						if [[ $debug = 2 ]]
						then
							echo -e "\e[33m|-> \e[0m found period"
						fi
					fi
				fi
				if [[ $found = 1 ]]
				then
					if [[ $lookchildID = 1 ]]
					then
						if [[ $debug = 2 ]]
						then
							echo -e "\e[33m|-> \e[0m lookchildID $line"
						fi
						foundchild=0
						if [[ $line = "$childID" ]]
						then
							if [[ $Usechildsareperiods = 0 ]]
							then
								echo -e "\e[33m|->\e[0m ChildName = $ChildName"
							else
								echo -e "\e[33m|->\e[0m will load child/period name later"
							fi
							echo -e "\e[33m|->\e[0m childID = $childID"
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
						echo -e "\e[33m|-> \e[0m found child: $line"
					fi
					if [[ $readRuns = 1 ]]
					then
						if [[ $debug = 2 ]]; then
							printf "($line)"
						fi
						if [[ $line = *"<"* ]]
						then
							readRuns=0
							if [[ $line = *"<br>"* ]] && [[ ! $line = "<br>" ]]; then
								readrun=${line%<*}
								printf "$readrun"
								printf "$readrun\n" >> $Runlist
								((NumberOfRuns++))
							fi
							printf "\n $NumberOfRuns Runs found \n"
							lastRun=1

						else
							readrun=${line%,}
							printf "$readrun, "
							printf "$readrun\n" >> $Runlist
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
						Runlist="$FrameworkDir/DownloadAndDataPrep/runlists/runNumbers$ChildName-$RunlistName.txt"
						echo;
						echo  -e "\e[36m------------------------------------\e[0m"
						echo -e "\e[36m|-> Runlist $RunlistID\tName = $RunlistName \e[0m"

						if [[ -f $Runlist ]]; then rm $Runlist; fi
					fi

					if [[ $lastRun = 1 ]]
					then
						lastRun=0
						if [[ $OptAllRunlists = 1 ]]
						then
							echo "$RunlistName" > $OptRunlistNamefile
						fi
						foundRunlistNamefile=$OUTPUTDIR/.${child}/FoundRunlistNames.txt
						AllRunlistNames=$BASEDIR/AllRunlistNames.txt
						if [[ -f $foundRunlistNamefile ]]; then
							if [[ `grep "|$ChildName|$childID|$RunlistName|" $foundRunlistNamefile | wc -l` < 1 ]]; then
								echo "|$ChildName|$childID|$RunlistName|$RunlistID|" >> $foundRunlistNamefile
							else
								if [[ ! `grep "|$ChildName|$childID|$RunlistName|" $foundRunlistNamefile | cut -d "|" -f 5` =  "$RunlistID" ]]; then
									printf "\e[33mWARNING: RunList found twice\e[0m \t" | tee -a $WARNINGLog
									printf "|$ChildName|$childID|$RunlistName|$RunlistID| = " | tee -a $WARNINGLog
									grep "|$ChildName|$childID|$RunlistName|" $foundRunlistNamefile | tee -a $WARNINGLog
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

						for OptRunlistName in `cat $OptRunlistNamefile`
						do
							if [[ ! "$RunlistName" = "$OptRunlistName" ]]
							then
								continue
							fi
							if [[ $NumberOfRuns = 0 ]]; then
								echo  -e "\e[33mWARNING:  No runs in Runlist\e[0m: ChildName = $ChildName, childID = $childID, Runlist $RunlistID, Name = $RunlistName" | tee -a $WARNINGLog
								continue
							fi
							if [[ $Usechildsareperiods = 1 ]]; then
								ChildName=`grep "export ALIEN_JDL_child_${childID}_LPMPRODUCTIONTAG" $envFile | cut -d "'" -f 2`
								# ChildName=`echo $(head -n 1 $PathtoRuns) | awk -F "/" '{print $7}' `
								echo -e "\e[33m|->\e[0m Changed periodName = $ChildName"
							fi
							Type='data'
							if (( `echo -n ${ChildName#LHC*} | wc -c` > 3 )); then
								Type='sim'
							fi
							Year="20${ChildName:3:2}"

							# prepare directory
							mkdir -p "$OUTPUTDIR/.$child/$RunlistName"

							FileList="$OUTPUTDIR/.$child/$RunlistName/FileList.txt"

							# look for availible Files
							if [ $newfiles = 1 ] || [ ! -f $FileList ]; then
								for Search in `cat $Searchfile`
								do
									cmd="alien_ls $AlienDir$child/merge_runlist_$RunlistID/ 2> /dev/null | grep "$Search" > $FileList.tmp"
									eval $cmd
									usecmd $cmd
									AddToList $FileList.tmp $FileList
								done
							fi

							mkdir -p $OUTPUTDIR/$RunlistName
							mkdir -p $OUTPUTDIR/$RunlistName/$ChildName

							# check if merging was successful
							MergeRuns=0
							test=".err.log"
							if [[ -f $test ]]; then rm $test; fi
							tmp=`alien_ls $AlienDir$child/merge_runlist_${RunlistID}/ 2> $test | grep "AnalysisResults.root" | wc -l`
							if [[ `grep "FATAL" $test | wc -l` > 0 ]]; then
								printf "alien connection lost Retry  "
							fi
							count=1
							while [[ `grep "FATAL" $test | wc -l` > 0 ]]; do
								printf "$count.."
								((count++))
								sleep 1
								tmp=`alien_ls $AlienDir$child/merge_runlist_${RunlistID}/ 2> $test | grep "AnalysisResults.root" | wc -l`
							done
							printf "\n"

							rm $test
							if [[ $tmp < 1 ]]; then
								MergeRuns=1
								echo  -e "\e[33mWARNING:  No files found\e[0m, trying to merge from runwise output: ChildName = $ChildName, childID = $childID, Runlist $RunlistID, Name = $RunlistName" | tee -a $WARNINGLog
								echo  -e "\e[36m------------------------------------\e[0m"
							fi


							if [[ $Userunwise = 1 ]] || [[ $MergeRuns = 1 ]]; then
								# Download all runs
								tmpruncount=0
								for runName in `cat $Runlist`
								do
									((tmpruncount++))
									merginghigher=0

									# look for availible Files
									runDir=$OUTPUTDIR/.$child/$runName

									RunFileList="$runDir/FileList.txt"
									RunPathList="$runDir/PathList.txt"
									SubRunFileList="$runDir/subFileList.txt"

									mkdir -p $runDir &> /dev/null
									if [[ -d $runDir ]]; then
										ln -sf $runDir $OUTPUTDIR/$RunlistName/$ChildName/$runName
										# cmd="ln -sf $runDir $OUTPUTDIR/$RunlistName/$ChildName/$runName"
										# eval $cmd
										# usecmd $cmd
									fi

									if [ $newfiles = 1 ] || [ ! -f $RunPathList ]; then
										cmd=""
										if [[ $Type = "sim" ]]; then
											cmd="alien_find /alice/$Type/$Year/$ChildName/$runName/ AnalysisResults.root 2> /dev/null | grep $TrainPage/$TrainNumber | grep _$childID/AnalysisResults.root > $RunPathList.tmp"
										else
											cmd="alien_find /alice/$Type/$Year/$ChildName/000$runName/ AnalysisResults.root 2> /dev/null | grep $TrainPage/$TrainNumber | grep _$childID/AnalysisResults.root  > $RunPathList.tmp"
										fi
										eval $cmd
										usecmd $cmd
										AddToList $RunPathList.tmp $RunPathList
									fi

									maxcount=0
									path=""
									if [[ ! -f $RunPathList ]]; then
										## need merging higher stages
										merginghigher=1

										if [ $newfiles = 1 ] || [ ! -f $RunPathList ]; then
											cmd=""
											if [[ $Type = "sim" ]]; then
												cmd="alien_find /alice/$Type/$Year/$ChildName/$runName/ AnalysisResults.root 2> /dev/null | grep $TrainPage/$TrainNumber > $RunPathList.tmp"
											else
												cmd="alien_find /alice/$Type/$Year/$ChildName/000$runName/ AnalysisResults.root 2> /dev/null | grep $TrainPage/$TrainNumber  > $RunPathList.tmp"
											fi
											eval $cmd
											usecmd $cmd
											AddToList $RunPathList.tmp $RunPathList
										fi

										tmp=1
										tmp2=0
										if [ ! -f $RunPathList ]; then
											tmp2=1
											printf "  \e[33m|->\e[0m Run $runName wasn't found! Retry "
										fi
										while [[ ! -f $RunPathList ]]; do
											if [[ $tmp = 11 ]]; then
												break
											fi
											printf "${tmp} "
											alien_cp -o  alien:/$1 file:/$2 &> $3
											((tmp++))
										done


										if [ ! -f $RunPathList ]; then
											echo -e "\t\e[31mError\e[0m Run $runName wasn't found!" | tee -a $ErrorLog
											continue
										fi
										if [[ $tmp2 = 1 ]]; then
											printf "\n"
										fi

										for path2 in `cat $RunPathList `
										do
											subpath=${path2%%AnalysisResults.root}
											path=${path2%%/*/AnalysisResults.root}
											subrunname2="${subpath#*child_*/}"
											subrunname="${subrunname2%/*}"
											if [[ $subrunname = "" ]]; then continue; fi
											((maxcount++))
											subrunDir=$OUTPUTDIR/.$child/$runName/$subrunname/
											for Search in `cat $Searchfile`
											do
												cmd="alien_ls $subpath 2> /dev/null | grep "$Search" > $SubRunFileList.tmp"
												eval $cmd
												usecmd $cmd
												if [[ -f $SubRunFileList.tmp ]]; then
													for tmp in `cat $SubRunFileList.tmp`
													do
														if [ ! -f $runDir/$tmp ]; then
															if [[ -f $SubRunFileList ]]; then
																if [[ `grep $tmp $SubRunFileList | wc -l` < 1 ]]; then
																	echo $tmp >> $SubRunFileList
																fi
															else
																echo $tmp >> $SubRunFileList
															fi
														fi
													done
													rm $SubRunFileList.tmp
												fi
											done
										done

										tmpsubruncount=0
										if [[ -f $SubRunFileList ]]; then
											echo  -e "\e[33mWARNING:  No files found \e[0m, trying to merge from higher stages" | tee -a $WARNINGLog
											for path2 in `cat $RunPathList `
											do
												subpath=${path2%%AnalysisResults.root}
												path=${path2%%/*/AnalysisResults.root}
												subrunname2="${subpath#*child_*/}"
												subrunname="${subrunname2%/*}"
												if [[ $subrunname = "" ]] ||  [[ $subrunname = *"/"* ]]; then continue; fi
												((tmpsubruncount++))
												subrunDir=$OUTPUTDIR/.$child/$runName/$subrunname/
												SubRunFileList="$runDir/subFileList.txt"

												# Download all relevant files
												isfirsttmp=1
												for subrunfilename in `cat $SubRunFileList`
												do
													if [[ $isfirsttmp = 1 ]] ; then
														isfirsttmp=0
														printf "\tDownload SubRun\t$tmpsubruncount/$maxcount\t$runName|$subrunname\t$subrunfilename "
													else
														printf "\t\t\t\t\t\t$subrunfilename "
													fi
													if [[ -f $RunFileList ]]; then
														if [[ `grep $subrunfilename $RunFileList | wc -c` -eq 0 ]]; then
															echo "$subrunfilename" >> $RunFileList
														fi
													else
														echo "$subrunfilename" >> $RunFileList
														subruninFile=$subpath/$subrunfilename
													fi

													# subrunDir=$OUTPUTDIR/$RunlistName/$ChildName/$runName/$subrunname/
													mkdir -p $subrunDir &> /dev/null

													subrunoutFile=$subrunDir/$subrunfilename
													subrundownlogFile=$subrunDir/.${subrunfilename%%.root}.downlog

													# if [[ `alien_ls $subruninFile | grep "no such file or directory" | wc -c` -eq 0 ]]; then
													# 	GetFile $subruninFile $subrunoutFile $subrundownlogFile
													# else
													# 	echo -e "\e[31m|->\e[0m missing $subruninFile  "  | tee -a $ErrorLog
													# fi
													GetFile $subruninFile $subrunoutFile $subrundownlogFile


													subalreadyMerged=$subrunDir/.${subrunfilename%%.root}.merged
													sublogFile=$subrunDir/.${subrunfilename%%.root}.log
													submergedFile=$runDir/$subrunfilename
													if [[ -f $subalreadyMerged ]]
													then
														printf "\t\e[33m|-> \e[0m already merged\n"
														# echo -e "\e[33m|-> \e[0m $RunlistName $ChildName $runName $subrunname already merged"
													else
														if [[ -f $sublogFile ]]
														then
															rm $sublogFile
														fi
														if [[ -f $subrunoutFile ]]
														then
															if [[ -f $submergedFile ]]
															then
																printf "\t\e[33m|->\e[0m merging subrun to run"  #(log: $logFile)"
																# echo -e "\e[33m|->\e[0m merging subrun to run $RunlistName $ChildName $runName $subrunname"  #(log: $logFile)"
																hadd -k $submergedFile.tmp $submergedFile $subrunoutFile  &> $sublogFile
																wait
																if [[ $debug = 1 ]] || [[ $debug = 2 ]]
																then
																	echo " Log:"
																	cat $sublogFile
																	echo;echo;
																fi
																if [[ `wc -l $sublogFile` > 9 ]]
																then
																	echo -e "\t\e[31mError\e[0m $RunlistName $ChildName $runName $subrunname not merged correctly" | tee -a $ErrorLog
																	cat $sublogFile >> $ErrorLog
																	rm $submergedFile.tmp
																else
																	printf "\t\e[33m|->\e[0m successful\n"  #(log: $logFile)"
																	rm $submergedFile
																	mv $submergedFile.tmp $submergedFile
																	touch $subalreadyMerged
																fi
															else
																printf "\t\e[33m|->\e[0m Copy: is first\n"
																# echo -e "\e[33m|->\e[0m Copy: $RunlistName $ChildName $runName $subrunname is first"
																echo "Copyed to $submergedFile" > $sublogFile
																cp $subrunoutFile $submergedFile
																touch $subalreadyMerged
															fi
														else
															if [[ $debug = 1 ]] || [[ $debug = 2 ]]
															then
																echo -e "\t\e[31m|->\e[0m subrun missing $subrunoutFile  "  | tee -a $ErrorLog
															else
																echo -e "\t\e[31m|->\e[0m subrun missing "  | tee -a $ErrorLog
															fi
														fi
													fi
												done
											done

											echo -e "merge from higher stages.. done, removing downloads" | tee -a $WARNINGLog
										else
											echo -e "was merged from higher stages" | tee -a $WARNINGLog
										fi
										rm -r $runDir/[0-9][0-9]*
									else
										for path2 in `cat $RunPathList`
										do
											path=${path2%%AnalysisResults.root}
											if [[ $debug = 1 ]] || [[ $debug = 2 ]]
											then
												echo "path = $path"
											fi
											if [ $newfiles = 1 ] || [ ! -f $RunFileList ]; then
												for Search in `cat $Searchfile`
												do
													cmd="alien_ls $path 2> /dev/null | grep "$Search" > $RunFileList.tmp "
													eval $cmd
													usecmd $cmd
													AddToList $RunFileList.tmp $RunFileList
												done
											fi
										done
									fi


									# Download all relevant files
									isfirsttmp=1
									for runfilename in `cat $RunFileList`
									do
										if [[ -f $FileList ]]; then
											if [[ `grep $runfilename $FileList | wc -l` < 1 ]]; then
												echo "$runfilename" >> $FileList
											fi
										else
											echo "$runfilename" >> $FileList
										fi
										runinFile=$path/$runfilename
										runoutFile=$runDir/$runfilename
										rundownlogFile=$runDir/.${runfilename%%.root}.downlog

										if [[ $merginghigher = 0 ]] || [[ $MergeRuns = 1 ]]; then
											if [[ $isfirsttmp = 1 ]] ; then
												isfirsttmp=0
												printf "Download Run\t$tmpruncount/$NumberOfRuns\t$runName\t$runfilename "
											else
												printf "\t\t\t\t$runfilename "
											fi
										fi

										if [[ $merginghigher = 0 ]]; then
											GetFile $runinFile $runoutFile $rundownlogFile
										fi

										if [[ $MergeRuns = 1 ]]
										then
											# runoutFile=$runDir/$filename
											alreadyMerged=$runDir/.${runfilename%%.root}-${RunlistName}.merged
											logFile=$runDir/.${runfilename%%.root}-${RunlistName}.log
											# mergedFile=$OUTPUTDIR/.$child/$runfilename
											mergedFile=$OUTPUTDIR/$RunlistName/$ChildName/$runfilename
											if [[ -f $alreadyMerged ]]
											then
												printf "\t\e[33m|-> \e[0m already merged\n"
												# echo -e "\e[33m|-> \e[0m $RunlistName $ChildName $runName already merged"
											else
												if [[ -f $logFile ]]
												then
													rm $logFile
												fi
												if [[ -f $runoutFile ]]
												then
													if [[ -f $mergedFile ]]
													then
														if [[ $debug = 1 ]] || [[ $debug = 2 ]]
														then
															echo -e "\e[33m|->\e[0m merging run to period $RunlistName $ChildName $runName (log: $logFile)"
														else
															printf "\t\e[33m|->\e[0m merging run to period"
														fi
														hadd -k $mergedFile.tmp $mergedFile $runoutFile  &> $logFile
														wait
														if [[ $debug = 1 ]] || [[ $debug = 2 ]]
														then
															echo " Log:"
															cat $logFile
															echo;echo;
														fi
														if [[ $debug = 1 ]] || [[ $debug = 2 ]]
														then
															echo "rm $mergedFile"
														fi
														if [[ `wc -l $logFile` > 9 ]]
														then
															echo -e "\e[31mError\e[0m $RunlistName $ChildName $runName not merged correctly" | tee -a $ErrorLog
															cat $logFile >> $ErrorLog
															rm $mergedFile.tmp
															rm $runoutFile
														else
															rm $mergedFile
															if [[ $debug = 1 ]] || [[ $debug = 2 ]]
															then
																echo "mv $mergedFile.tmp $mergedFile "
															fi
															mv $mergedFile.tmp $mergedFile
															touch $alreadyMerged
															printf "\t\e[33m|->\e[0m successful\n"
														fi
													else
														printf "\t\e[33m|->\e[0m Copy: is first\n"
														# echo -e "\e[33m|->\e[0m Copy: $RunlistName $ChildName $runName is first"
														echo "Copyed to $mergedFile" > $logFile
														cp $runoutFile $mergedFile
														touch $alreadyMerged
													fi
												else
													if [[ $debug = 1 ]] || [[ $debug = 2 ]]
													then
														echo -e "\t\e[31m|->\e[0m run missing $runinFile  "  | tee -a $ErrorLog
													else
														echo -e "\t\e[31m|->\e[0m run missing "  | tee -a $ErrorLog
													fi
												fi
											fi
										else
											printf "\n"
										fi
									done
									# ###################################
								done
								if [[ $MergeRuns = 1 ]]; then
									echo -e "merge from runwise.. done" | tee -a $ErrorLog
								fi
							fi



							echo;
							# Download all relevant files
							for filename in `cat $FileList`
							do
								printf "\e[33m|-> Found files:\e[0m $RunlistName $ChildName $filename"
								outFile="$OUTPUTDIR/$RunlistName/$ChildName/$filename"
								inFile="$AlienDir$child/merge_runlist_$RunlistID/$filename"
								downlogFile="$OUTPUTDIR/$RunlistName/$ChildName/.${filename%%.root}.downlog"

								GetFile $inFile $outFile $downlogFile

								if [[ $UseMerge = 1 ]]
								then
									alreadyMerged=$OUTPUTDIR/$RunlistName/$ChildName/.${filename%%.root}.merged
									logFile=$OUTPUTDIR/$RunlistName/$ChildName/.${filename%%.root}.log
									mergedFile=$OUTPUTDIR/$RunlistName/$filename
									if [[ -f $alreadyMerged ]]
									then
										printf "\e[33m|-> \e[0m already merged\n"
									else
										if [[ -f $logFile ]]
										then
											rm $logFile
										fi
										if [[ -f $outFile ]]
										then
											if [[ -f $mergedFile ]]
											then
												printf "\e[33m|->\e[0m merging childs \n"  #(log: $logFile)"
												hadd -k $mergedFile.tmp $mergedFile $outFile  &> $logFile
												wait
												if [[ $debug = 1 ]] || [[ $debug = 2 ]]
												then
													echo " Log:"
													cat $logFile
													echo;echo;
												fi
												if [[ $debug = 1 ]] || [[ $debug = 2 ]]
												then
													echo "rm $mergedFile"
												fi
												if [[ `wc -l $logFile` > 9 ]]
												then
													echo -e "\e[31mError\e[0m $outFile not merged correctly" | tee -a $ErrorLog
													cat $logFile >> $ErrorLog
													rm $mergedFile.tmp
													# rm $outFile
												else
													rm $mergedFile
													if [[ $debug = 1 ]] || [[ $debug = 2 ]]
													then
														echo "mv $mergedFile.tmp $mergedFile "
													fi
													mv $mergedFile.tmp $mergedFile
													touch $alreadyMerged
												fi
											else
												echo -e "\e[33m|->\e[0m Copy: $outFile is first"
												echo "Copyed to $mergedFile" > $logFile
												cp $outFile $mergedFile
												touch $alreadyMerged
											fi
										else
											if [[ $debug = 1 ]] || [[ $debug = 2 ]]
											then
												echo -e "\t\e[31m|->\e[0m child missing $inFile  "  | tee -a $ErrorLog
											else
												echo -e "\t\e[31m|->\e[0m child missing "  | tee -a $ErrorLog
											fi
										fi
									fi
								fi
							done

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
		done

		echo;echo;echo;echo;

	done

fi


if [[ $NTrainFile = 1 ]] #&& [[ ! $TrainNumber = *"+"* ]]
then
	ln -sf $BASEDIR/$OutName $BASEDIR/$SetOutName
	# cmd="ln -sf $BASEDIR/$OutName $BASEDIR/$SetOutName"
	# eval $cmd
	# usecmd $cmd
fi

if [[ $MultiTrains = 1 ]] && [[ $MergeTrains = 0 ]]
then
	echo  -e "\e[36m------------------------------------\e[0m"
	echo "Start merging trains: $MergeTrainsOutname"
	echo  -e "\e[36m------------------------------------\e[0m"
	for TrainNumber in `cat $TrainNumberFile`
	do
		echo;echo;
		echo  -e "\e[36m|-> \e[0m $TrainNumber"
		singletrainDir=.$TrainPage-$TrainNumber
		for RunlistName in `cat $OptRunlistNamefile`
		do
			echo  -e "\t\e[36m|-> \e[0m $RunlistName"
			if [[ ! -e $BASEDIR/$singletrainDir/$RunlistName ]]; then
				echo  -e "\t\e[33mWARNING: $singletrainDir $RunlistName Not found \e[0m " | tee -a $WARNINGLog
				continue
			fi
			mkdir -p $MergeTrainsOutname/$RunlistName
			ln -sf $BASEDIR/$singletrainDir/$RunlistName/*.* $MergeTrainsOutname/$RunlistName/.

			# look for availible periods
			periodList="$MergeTrainsOutname/$RunlistName/.periodList.txt"
			if [[ -f $periodList ]]; then rm $periodList; fi
			cmd="ls $BASEDIR/$singletrainDir/$RunlistName/ | grep 'LHC' >>  $periodList"
			eval $cmd
			usecmd $cmd

			for periodnametmp in `cat $periodList`
			do
				Periodname2=${periodnametmp#*$RunlistName/}
				Periodname=${Periodname2%/*}
				printf  "\t\t\e[36m|-> \e[0m $Periodname \n\n"
				mkdir -p $MergeTrainsOutname/$RunlistName/$Periodname
				ln -sf $BASEDIR/$singletrainDir/$RunlistName/$Periodname/*.* $MergeTrainsOutname/$RunlistName/$Periodname/.

				if [[ $Userunwise = 1 ]]; then
					# look for availible periods
					Runlist="$MergeTrainsOutname/$RunlistName/.$Periodname-Runlist.txt"
					if [[ ! -f $Runlist ]]; then
						cmd="ls -d $BASEDIR/$singletrainDir/$RunlistName/$Periodname/[0-9]* >>  $Runlist"
						eval $cmd
						usecmd $cmd
					fi

					for runnametmp in `cat $Runlist`
					do
						runname2=${runnametmp#*$Periodname/}
						runname=${runname2%/*}
						printf "\e[36m$runname, \e[0m"
						mkdir -p $MergeTrainsOutname/$RunlistName/$Periodname/$runname
						ln -sf $BASEDIR/$singletrainDir/$RunlistName/$Periodname/$runname/*.* $MergeTrainsOutname/$RunlistName/$Periodname/$runname/.
					done
					printf "\n\n\n"
				fi
			done
		done
	done
fi


if [[ $MergeTrains = 1 ]]
then
	echo  -e "\e[36m------------------------------------\e[0m"
	echo "Start merging trains: $MergeTrainsOutname"
	echo  -e "\e[36m------------------------------------\e[0m"
	for TrainNumber in `cat $TrainNumberFile`
	do
		echo;echo;
		echo  -e "\e[36m|-> \e[0m $TrainNumber"
		singletrainDir=.$TrainPage-$TrainNumber
		for RunlistName in `cat $OptRunlistNamefile`
		do
			echo  -e "\t\e[36m|-> \e[0m $RunlistName"
			mkdir -p $MergeTrainsOutname/$RunlistName

			# look for availible periods
			periodList="$MergeTrainsOutname/$RunlistName/.periodList.txt"
			if [[ -f $periodList ]]; then rm $periodList; fi
			cmd="ls $BASEDIR/$singletrainDir/$RunlistName/ | grep 'LHC' >>  $periodList"
			eval $cmd
			usecmd $cmd

			for periodnametmp in `cat $periodList`
			do
				Periodname2=${periodnametmp#*$RunlistName/}
				Periodname=${Periodname2%/*}
				echo  -e "\t\t\e[36m|-> \e[0m $Periodname"
				mkdir -p $MergeTrainsOutname/$RunlistName/$Periodname

				# look for availible Files
				FileList="$MergeTrainsOutname/$RunlistName/.FileList.txt"
				if [ -f $FileList ]; then rm $FileList; fi
				for Search in `cat $Searchfile`
				do
					cmd="ls $BASEDIR/$singletrainDir/$RunlistName/$Periodname/ | grep "$Search" | grep ".root" >> $FileList"
					eval $cmd
					usecmd $cmd
				done
				for filenametmp in `cat $FileList`
				do
					filename=${filenametmp#*/$RunlistName/*/}
					printf "\t\t\t\e[36m|-> \e[0m $filename"
					outFile=$BASEDIR/$singletrainDir/$RunlistName/$Periodname/$filename
					alreadyMerged=$MergeTrainsOutname/$RunlistName/$Periodname/.$TrainNumber-${filename%%.root}.merged
					logFile=$MergeTrainsOutname/$RunlistName/$Periodname/.$TrainNumber-${filename%%.root}.log
					mergedFile=$MergeTrainsOutname/$RunlistName/$Periodname/$filename
					if [[ -f $alreadyMerged ]]
					then
						printf "\t\e[33m|-> \e[0malready merged (${outFile}) \n"
					else
						if [[ -f $logFile ]]
						then
							rm $logFile
						fi
						if [[ -f $outFile ]]
						then
							if [[ -f $mergedFile ]]
							then
								printf "\t\e[33m|->\e[0m merging trains $RunlistName $Periodname $runname $filename"  #(log: $logFile)"
								hadd -k $mergedFile.tmp $mergedFile $outFile  &> $logFile
								wait
								if [[ $debug = 1 ]] || [[ $debug = 2 ]]
								then
									echo " Log:"
									cat $logFile
									echo;echo;
								fi
								if [[ $debug = 1 ]] || [[ $debug = 2 ]]
								then
									echo "rm $mergedFile"
								fi
								if [[ `wc -l $logFile` > 9 ]]
								then
									echo -e "\t\e[31mError\e[0m $outFile not merged correctly" | tee -a $ErrorLog
									cat $logFile >> $ErrorLog
									rm $mergedFile.tmp
									# rm $outFile
								else
									rm $mergedFile
									if [[ $debug = 1 ]] || [[ $debug = 2 ]]
									then
										echo "mv $mergedFile.tmp $mergedFile "
									fi
									mv $mergedFile.tmp $mergedFile
									touch $alreadyMerged
									printf "\n"
								fi
							else
								printf "\t\e[33m|->\e[0m Copy first ($outFile) \n"
								echo "Copyed to $mergedFile" > $logFile
								cp $outFile $mergedFile
								touch $alreadyMerged
							fi
						else
							if [[ $debug = 1 ]] || [[ $debug = 2 ]]
							then
								echo -e "\t\e[31m|->\e[0m missing ($outFile)"  | tee -a $ErrorLog
							else
								echo -e "\t\e[31m|->\e[0m missing "  | tee -a $ErrorLog
							fi
						fi
					fi
				done
			done
			# fi
		done
	done
fi
if [[ $MergeTrains = 1 ]] && [[ $Userunwise = 1 ]]
then
	echo  -e "\e[36m------------------------------------\e[0m"
	echo "Start merging runs from trains to $MergeTrainsOutname"
	echo  -e "\e[36m------------------------------------\e[0m"
	for TrainNumber in `cat $TrainNumberFile`
	do
		echo;echo;
		echo  -e "\e[36m|-> \e[0m $TrainNumber"
		singletrainDir=.$TrainPage-$TrainNumber
		for RunlistName in `cat $OptRunlistNamefile`
		do
			echo  -e "\t\e[36m|-> \e[0m $RunlistName"

			# look for availible periods
			periodList="$MergeTrainsOutname/$RunlistName/.periodList.txt"
			if [[ -f $periodList ]]; then rm $periodList; fi
			cmd="ls $BASEDIR/$singletrainDir/$RunlistName/ | grep 'LHC' >>  $periodList"
			eval $cmd
			usecmd $cmd

			for periodnametmp in `cat $periodList`
			do
				Periodname2=${periodnametmp#*$RunlistName/}
				Periodname=${Periodname2%/*}
				echo  -e "\t\t\e[36m|-> \e[0m $Periodname \n"


				# look for availible periods
				Runlist="$MergeTrainsOutname/$RunlistName/.$Periodname-Runlist.txt"
				if [[ -f $Runlist ]]; then rm $Runlist; fi
				cmd="ls -d $BASEDIR/$singletrainDir/$RunlistName/$Periodname/[0-9]* >>  $Runlist"
				eval $cmd
				usecmd $cmd

				for runnametmp in `cat $Runlist`
				do
					runname2=${runnametmp#*$Periodname/}
					runname=${runname2%/*}
					printf "\t\t\t\e[36m|-> \e[0m $runname"
					mkdir -p $MergeTrainsOutname/$RunlistName/$Periodname/$runname
					# look for availible Files
					FileList="$MergeTrainsOutname/$RunlistName/.FileList.txt"
					if [ -f $FileList ]; then rm $FileList; fi
					for Search in `cat $Searchfile`
					do
						cmd="ls $BASEDIR/$singletrainDir/$RunlistName/$Periodname/$runname/ | grep "$Search" | grep ".root" >> $FileList"
						eval $cmd
						usecmd $cmd
					done
					for filenametmp in `cat $FileList`
					do
						filename=${filenametmp#*/$runname/}
						printf "\t\e[36m|-> \e[0m $filename"
						outFile=$BASEDIR/$singletrainDir/$RunlistName/$Periodname/$runname/$filename
						alreadyMerged=$MergeTrainsOutname/$RunlistName/$Periodname/$runname/.$TrainNumber-${filename%%.root}.merged
						logFile=$MergeTrainsOutname/$RunlistName/$Periodname/$runname/.$TrainNumber-${filename%%.root}.log
						mergedFile=$MergeTrainsOutname/$RunlistName/$Periodname/$runname/$filename
						if [[ -f $alreadyMerged ]]
						then
							printf "\t\e[33m|-> \e[0malready merged\n"
						else
							if [[ -f $logFile ]]
							then
								rm $logFile
							fi
							if [[ -f $outFile ]]
							then
								if [[ -f $mergedFile ]]
								then
									printf "\t\e[33m|->\e[0m merging trains"  #(log: $logFile)"
									hadd -k $mergedFile.tmp $mergedFile $outFile  &> $logFile
									wait
									if [[ $debug = 1 ]] || [[ $debug = 2 ]]
									then
										echo " Log:"
										cat $logFile
										echo;echo;
									fi
									if [[ $debug = 1 ]] || [[ $debug = 2 ]]
									then
										echo "rm $mergedFile"
									fi
									if [[ `wc -l $logFile` > 9 ]]
									then
										echo -e "\t\e[31mError\e[0m not merged correctly" | tee -a $ErrorLog
										cat $logFile >> $ErrorLog
										rm $mergedFile.tmp
									else
										rm $mergedFile
										if [[ $debug = 1 ]] || [[ $debug = 2 ]]
										then
											echo "mv $mergedFile.tmp $mergedFile "
										fi
										mv $mergedFile.tmp $mergedFile
										touch $alreadyMerged
										printf "\n"
									fi
								else
									printf "\t\e[33m|->\e[0m Copy first\n"
									echo "Copyed to $mergedFile" > $logFile
									cp $outFile $mergedFile
									touch $alreadyMerged
								fi
							else
								if [[ $debug = 1 ]] || [[ $debug = 2 ]]
								then
									echo -e "\t\e[31m|->\e[0m missing ($outFile)" | tee -a $ErrorLog
								else
									echo -e "\t\e[31m|->\e[0m missing "  | tee -a $ErrorLog
								fi
							fi
						fi
					done
				done
				echo;
			done
		done
	done
fi


rm $TrainNumberFile
rm $Searchfile
rm $OptRunlistNamefile


echo;echo;echo;echo;
echo  -e "\e[36m------------------------------------\e[0m"
for setting in "$@"
do
	printf "${setting#-} "
done
echo;
if [[ -f $ErrorLog ]]; then
	cat $ErrorLog
else
	echo  -e "\tfinished without errors"
fi
if [[ -f $WARNINGLog ]]; then
	cat $WARNINGLog
else
	echo  -e "\tfinished without warnings"
fi
echo  -e "\e[36m------------------------------------\e[0m"

echo;echo;echo;

for setting in "$@"
do
	printf "${setting#-} \n" >> ${PWD}/Error.log
	# echo $setting
	if [[ $setting = "-totalLog" ]]
	then
		echo " " >> ${PWD}/Error.log
		echo  -e "\e[36m------------------------------------\e[0m" >> ${PWD}/Error.log
		# echo "$@" >> ${PWD}/Error.log
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
rm $lockfile
