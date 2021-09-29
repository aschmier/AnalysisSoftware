#! /bin/bash


wkdir=$PWD
# BASEDIR="/media/adrian/Data/Test"
BASEDIR="/media/adrian/Data/grid_data"
FrameworkDir="/home/adrian/git/AnalysisSoftware"
UserName="Adrian Mechler";
pathtocert="~/.globus"
alienUserName="amechler"
key="key.pem"
cacert="ca.pem"
clientca="client.pem"
Type='data'
debug=0
trainnum=""
file="root_archive.zip"
Runlist=""
TrainPagetmp=""
link=""
mergedtrainname=""
LIST_TrainID=()
unset LIST_TrainID
LIST_Runlists=()
unset LIST_Runlists
# LIST_Lists=()
# unset LIST_Lists
LIST_Files=()
unset LIST_Files
LIST_FilesGrid=()
unset LIST_FilesGrid
norunlist=1
dorunwise=0
domergetrains=0
dopergeruns=0
nodown=0
onlyrunwise=0
redomerging=0
nomerging=0
OLDIFS=$IFS
re='^[0-9]+$'

DownloadWebpage(){
	debug "\e[33m|-> \e[0mDownloading ${2}\n" 
	cmd="curl '${2}' --key $pathtocert/$key -k --cert $pathtocert/$clientca &> $1"
	debug "$cmd\n"
	eval $cmd
	touch "$1";
}

debug(){
	if [[ $debug = 1 ]]
	then
		printf "$1"
	fi
}

checkFiletooold(){
	if [ -f $1 ]
	then 
		if [[ `cat $1 | wc -l` -lt 100 ]]
		then
			return 0
		else
			if [ "$(( $(date +"%s") - $(stat -c "%Y" $1) ))" -gt "$2" ]  # 86400 = 1 Day
			then
				debug "\e[33m|-> \e[0mFile $1 exists. \e[33m|-> \e[0mbut is too old! download new one \n" 
				return 0
			else
				debug "\e[33m|-> \e[0mFile $1 exists. \e[33m|-> \e[0mand is up-to-date\n"
				return 1
			fi
		fi
	else
		return 0
	fi
	return 1
}

GetWebpage(){
	debug "\e[33mDownloadWebpage(): \e[0m\t$1\t$2\n"
	# we download the html to grep needed information from there
	# if checkFiletooold $1 "86400" 
	if [ ! -e $1 ]
	then
		DownloadWebpage $1 $2
	fi
}

GetChildName(){
	# envFile=$1
	# child=$2
	# childID=$3
	ChildName=`grep "PERIOD_NAME=" $envFile | awk -F "=" '{print $2}' | awk -F ";" '{print $1}'`
	ChildName=${ChildName%\'}
	ChildName=${ChildName#\'}
	# debug "\e[33m|->\e[0m child = $child ($ChildName)\n" 

	childID=${child##*child_}
	childID=${childID%/*}

	ChildName2=`grep "export ALIEN_JDL_child_${childID}_LPMPRODUCTIONTAG" $envFile | cut -d "'" -f 2`
	if [[ $ChildName2 = "" ]]; then
		ChildName2=`grep "export TEST_DIR_child_${childID}=" $envFile | cut -d "'" -f 2 | cut -d "/" -f 5`
	fi
	if [[ $ChildName2 = "" ]]; then
		ChildName2=`grep "export ALIEN_JDL_LPMPRODUCTIONTAG=" $envFile | cut -d "'" -f 2 | cut -d "/" -f 5`
	fi
	if [[ ! $ChildName == $ChildName2 ]]; then
		ChildName2Sub=`grep "export SUBSELECTION_child_${childID}" $envFile | cut -d "'" -f 2`
		ChildNameonline=$ChildName2
		if [[ ! $ChildName2Sub = "" ]]; then
			ChildName2="${ChildName2}${ChildName2Sub%/*}"
		fi
		debug "\e[33m|->\e[0m Changed periodName = $ChildName2\n" 
		if [[ ! $ChildName2 = "" ]]; then
			ChildName=$ChildName2
		fi
	fi
	# return $ChildName
}

GetTrainPage(){
	TrainPage=$1
	if [[ $TrainPage = "GA_pp" ]]
	then
		TrainPageNum="23"
	elif [[ $TrainPage = "GA_pp_MC" ]]
	then
		TrainPageNum="43"
		Type='sim'
	elif [[ $TrainPage = "GA_pp_AOD" ]]
	then
		TrainPageNum="62"
	elif [[ $TrainPage = "GA_pp_MC_AOD" ]]
	then
		TrainPageNum="66"
		Type='sim'
	elif [[ $TrainPage = "GA_PbPb_MC_AOD" ]]
	then
		TrainPageNum="65"
		Type='sim'
	else
		printf "\e[31m|-> $TrainPage\e[0m not supported"  \n
	fi
	return $TrainPageNum
}

GetFile(){
	debug "\e[33mGetFile(): \e[0m \n"
	debug "$1 $2 $3 \n"
	cmd="alien_cp -select $1 $2 file:$3"
	debug "$cmd\n"
	eval $cmd 
	printf "\e[33mReturned:\e[0m $?\n"
}

ParseSettings(){
	Type='data'
	debug=0
	file="root_archive.zip"
	Runlist=""
	mergedtrainname=""
	unset LIST_TrainID
	unset LIST_Runlists
	unset LIST_Files
	unset LIST_FilesGrid
	norunlist=1
	dorunwise=0
	nodown=0
	onlyrunwise=0
	redomerging=0
	nomerging=0
    
    printf "\e[36m"
    for opt in "$@"
	do
		if [[ $opt = "+"* ]]; then 
            file=${opt#+}
			LIST_Files+=($file)			
        fi
		if [[ $opt = "-"* ]]; then 
            Runlist=${opt#-}
			LIST_Runlists+=($Runlist)
			norunlist=0
        fi
		if [[ $opt = "_"* ]]; then 
            trainnum=${opt#_}
			LIST_TrainID+=($trainnum)
        fi
		if [[ $opt = "runwise" ]]; then 
            dorunwise=1
        fi
		if [[ $opt = "onlyrunwise" ]]; then 
            dorunwise=1
            onlyrunwise=1
        fi
		if [[ $opt = "nodown" ]]; then 
			printf "Will skip download \n"
            nodown=1
        fi
		if [[ $opt = "redomerging" ]]; then 
			printf "Will redo merging \n"
            redomerging=1
        fi
		if [[ $opt = "nomerging" ]]; then 
			printf "Will skip merging \n"
            nomerging=1
        fi
		if [[ $opt = "pergeruns" ]]; then 
            dopergeruns=1
        fi
		if [[ $opt = "debug" ]]; then 
            debug=1
        fi
		if [[ $opt = "/"* ]]; then 
            domergetrains=1
            mergedtrainname=${opt#/}
			printf "Will merge trains in $mergedtrainname\n"
        fi
	done
    printf "\n\e[0m"

}
    


Down(){
	printf "\n\n"
	debug "$@\n"


	TrainPagetmp="$1"
	GetTrainPage $TrainPagetmp
	link='/alice/cern.ch/user/a/alitrain/PWGGA/'$TrainPagetmp'/'

	ParseSettings ${@:2}


	for trainnum in "${LIST_TrainID[@]}"
	do
		printf "\e[33m|-> Train: \e[0m $trainnum\n"
		DownDir="${BASEDIR}/.$TrainPagetmp-$trainnum"
		tosearch=''$trainnum'_.*/env.sh'
		if [ $nodown = 0 ] ; then
			printf "\e[33mDownload: env.sh\e[0m \n"
			if [[ $debug = 1 ]]
			then
				GetFile "$tosearch" "$link" "$DownDir"
			else
				GetFile "$tosearch" "$link" "$DownDir" > /dev/null
			fi
		fi
		mkdir -p $DownDir
		
		Trainpage="$DownDir/.$TrainPagetmp.html"
		offset=0
		ListString=""
		trainwebpage="https://alimonitor.cern.ch/trains/train.jsp?train_id=${TrainPageNum}&offset=$offset#runs"
		GetWebpage $Trainpage $trainwebpage
		ListString=`grep -C 6 "$trainnum</a>" $Trainpage | grep "list"`
		while [[ $ListString = "" ]]; do
			offset=$((offset + 50))
			debug "offset: $offset\n"
			rm $Trainpage
			trainwebpage="https://alimonitor.cern.ch/trains/train.jsp?train_id=${TrainPageNum}&offset=$offset#runs"
			GetWebpage $Trainpage $trainwebpage
			ListString=`grep -C 6 "$trainnum</a>" $Trainpage | grep "list"`
		done
		# debug "$ListString\n"
		ListString=${ListString#*overlib(\'}
		ListString=${ListString%%\')\;*}
		unset LIST_AllLists
		IFS='>' LIST_AllLists=( $ListString )
		IFS=$OLDIFS
		RunlistID=0
		
		# if no runlist given add all availible to the list
		if [[ $norunlist = 1 ]]; then
			unset LIST_Runlists
			for list in "${LIST_AllLists[@]}"
			do
				RunlistName=${list%%:*}
				foundlist=0
				for Runlist in "${LIST_Runlists[@]}"
				do
					if [[ $RunlistName = *"$Runlist"* ]] ; then
						foundlist=1
					fi
				done
				if [[ $foundlist = 0 ]]; then
					LIST_Runlists+=($RunlistName)
				fi
			done
		fi
		# Loop over all lists in the web
		childID=0
		childsearch=""
		for list in "${LIST_AllLists[@]}"
		do
			if [[ $list = *"child"* ]]; then 
				((childID++))
				printf "\n\e[35m------------------------------------\e[0m \n"
				printf "\e[35m|-> child: $childID \e[0m \n"
				childsearch="child_$childID"
				RunlistID=0
				continue
			fi
			RunlistName=${list%%:*}
			((RunlistID++))
			for Runlist in "${LIST_Runlists[@]}"
			do
				if [[ $RunlistName = *"$Runlist"* ]] 
				then
					mkdir -p "$DownDir/$Runlist"
					printf "\e[33m------------------------------------\e[0m \n" 
					printf "\e[33m|-> Runlist $RunlistID\tName = $RunlistName \e[0m \n"
					for file in "${LIST_Files[@]}"; do
						tosearch=''$trainnum'_.*'$childsearch'/merge_runlist_'$RunlistID/'.*'$file'.*'
						if [ $onlyrunwise = 0 ]; then
							if [ $nodown = 0 ] ; then
								printf "\e[33mDownload: $file ($tosearch)\e[0m \n"
								GetFile "$tosearch" "$link" "$DownDir"
								if [ $? = 42 ]; then 
									tosearch=''$trainnum'_.*'$childsearch'/merge/.*'$file'.*'
									printf "\e[33mDownload: $file ($tosearch)\e[0m \n"
									GetFile "$tosearch" "$link" "$DownDir"
								fi
							fi
						fi
					done
					debug "\e[33m------------------------------------\e[0m \n" 
				else
					debug "\e[36m|-> Runlist $RunlistID\tName = $RunlistName \e[0m \n"
				fi
			done
		done

		printf "\n\n\e[36m|-> Add files to List \n\e[0m"
		for file in ` find $DownDir/ -regex ".*.root"`
		do
			for filetmp in "${LIST_Files[@]}"
			do
				if [[ $file = *"$filetmp"* ]] ; then
					foundlist=0
					filenametmp=${file##*/}
					for filetmp2 in "${LIST_FilesGrid[@]}"
					do
						if [[ $filetmp2 = $filenametmp ]] ; then
							foundlist=1
						fi
					done
					if [[ $foundlist = 0 ]]; then
						LIST_FilesGrid+=($filenametmp)
						printf "$filenametmp, "
					fi
				fi
			done
		done
		printf "\n"

		for child in `ls $DownDir | grep $trainnum`
		do
			envFile="$DownDir/$child/env.sh"
			# NCHilds=`grep "CHILD_DATASETS=" $envFile  | awk -F "=" '{print $2}' | awk -F ";" '{print $1}'`
			# NCHilds=${NCHilds%\'}
			# NCHilds=${NCHilds#\'}
			
			GetChildName
				
			debug "\t\e[33m|-> link \e[0m $child -> $ChildName\n"
			ln -sf $DownDir/$child $DownDir/$ChildName

							
			childID=0
			# Loop over all lists in the web
			for list in "${LIST_AllLists[@]}"
			do
				childsearch=""
				if [[ $list = *"child"* ]]; then 
					((childID++))
					RunlistID=0
					continue
				fi
				if [[ ! $child = *"child_$childID" ]]; then
					continue
				fi
				RunlistName=${list%%:*}
				((RunlistID++))
				for Runlist in "${LIST_Runlists[@]}"
				do
					if [[ $RunlistName = *"$Runlist"* ]] 
					then
						# printf "\e[35m|-> child: $childID\t$child\e[0m \n"
						if [ -e "$DownDir/$child/merge" ]; then 
							debug "\t\t\e[33m|-> link \e[0m merge -> merge_runlist_$merge_runlist_$RunlistID\n"
							ln -sf "$DownDir/$child/merge" "$DownDir/$child/merge_runlist_$RunlistID"
							
							debug "\t\t\e[33m|-> link \e[0m merge -> $RunlistName\n"
							ln -sf "$DownDir/$child/merge" "$DownDir/$child/$RunlistName"
										
							debug "\t\t\e[33m|-> link \e[0m $child/merge -> $Runlist/$ChildName\n"
							ln -sf "$DownDir/$child/merge" "$DownDir/$Runlist/$ChildName"
						else
							debug "\t\t\e[33m|-> link \e[0m merge_runlist_$RunlistID -> $RunlistName\n"
							ln -sf "$DownDir/$child/merge_runlist_$RunlistID" "$DownDir/$child/$RunlistName"			
							debug "\t\t\e[33m|-> link \e[0m $child/merge_runlist_$RunlistID -> $Runlist/$ChildName\n"
							ln -sf "$DownDir/$child/merge_runlist_$RunlistID" "$DownDir/$Runlist/$ChildName"
						fi
					fi
				done
			done
		done
		printf "\n\n"
	done


	if [ $onlyrunwise = 0 ] && [ $nomerging = 0 ]; then
		Mergeperiods $TrainPagetmp
	fi


	if [ $dorunwise = 1 ]; then
		Downloadruns $TrainPagetmp
	fi

	if [ $domergetrains = 1 ]; then
		MergeTrains
	fi

	if [ $dopergeruns = 1 ]; then
		PergeRuns
	fi
}

Mergeperiods() {
	for trainnum in "${LIST_TrainID[@]}"
	do
		printf "\n\n\e[36m------------------------------------\nMerging $trainnum:\n\e[0m"
		DownDir="${BASEDIR}/.$1-$trainnum"
		for Runlist in "${LIST_Runlists[@]}"
		do
			for file in "${LIST_FilesGrid[@]}"
			do
				# for mergefile in ` find $DownDir/*child_1/$Runlist/ -name $file`
				# for mergefile in ` ls $DownDir/*child_1/$Runlist/ | grep $file`
				mergefile=$file
				printf "\e[33m------------------------------------\n$mergefile\n\e[0m" 
				if [ -e $DownDir/$Runlist/$mergefile ] && [ $redomerging = 0 ]; then
					printf "$DownDir/$Runlist/$mergefile exists, skip merging..\n"
				else
					haddCommand="hadd -f -j 4 $DownDir/$Runlist/$mergefile "
					for child in ` ls $DownDir | grep $trainnum | grep child `
					do
						if [ -e $DownDir/$child/$Runlist/$mergefile ]; then
							haddCommand="${haddCommand}$DownDir/$child/$Runlist/$mergefile "
						fi
					done
					debug "$haddCommand\n"
					eval $haddCommand
					printf "\e[36mReturned:\e[0m $?\n"
				fi
			done
		done
		printf "\n\n"
	done
}
	

Downloadruns(){
	printf "\n\n\e[36m------------------------------------\nDownloadruns:\n\e[0m"
	if [ -e RunstoDownload.dat ] ; then rm RunstoDownload.dat; fi
	for trainnum in "${LIST_TrainID[@]}"
	do
		DownDir="${BASEDIR}/.$1-$trainnum"
		RunlistID=0
		tosearch=''$trainnum'_.*/merge.*/Stage_1.xml'
		printf "\e[33mDownload: Stage_1.xml ($tosearch)\e[0m \n"
		if [[ $debug = 1 ]]
		then
			GetFile "$tosearch" "$link" "$DownDir"
		else
			GetFile "$tosearch" "$link" "$DownDir" > /dev/null
		fi
		# Loop over all lists in the web
		childID=0
		childsearch=""
		for list in "${LIST_AllLists[@]}"
		do
			if [[ $list = *"child"* ]]; then 
				((childID++))
				printf "\n\e[35m------------------------------------\e[0m \n"
				printf "\e[35m|-> child: $childID \e[0m \n"
				childsearch="child_$childID"
				RunlistID=0
				continue
			fi
			RunlistName=${list%%:*}
			((RunlistID++))
			for Runlist in "${LIST_Runlists[@]}"
			do
				if [[ $RunlistName = *"$Runlist"* ]]
				then
					# printf "\e[33m------------------------------------\e[0m \n" 
					printf "\e[33m|-> Train: $trainnum\tChild: $childID\tRunlist: $RunlistID\tName: $RunlistName \e[0m \n"
					debug "\e[33mGet run files: ($DownDir/*$childsearch/merge_runlist_$RunlistID/) \e[0m \n"
					foundtmp=0
					for xmlfile in ` find $DownDir/*$childsearch/merge_runlist_$RunlistID/ -name Stage_1.xml`
					do
						foundtmp=1
						debug "$xmlfile\n"
						dir=${xmlfile%%/Stage*}
						if [[ ! $dir = *"/${trainnum}_"* ]]; then
							continue
						fi
						debug "$dir\n"
						for runline in `cat $xmlfile | grep 'lfn=' `
						do
							if [[ $runline = 'lfn'* ]]; then
								rundir=${runline#*lfn=\"}
								rundir=${rundir%%/root_archive.zip\"}
								debug "$rundir\n"
								run=${rundir#*LHC}
								run=${run#*/}
								run=${run%%/*}
								debug "$run, "
								mkdir -p $dir/$run
								for file in "${LIST_FilesGrid[@]}"; do
									if [ -e $dir/$run/$file ]; then
										debug "$dir/$run/$file  exists, skipping..\n" 
									else
										debug "Download: $rundir/$file -> $dir/$run/$file\n" 
										printf "alien:$rundir/$file file:$dir/$run/$file\n" >> RunstoDownload.dat
										# NFILESTMP=0
										# if [ -e RunstoDownload.dat ]; then
										# 	NFILESTMP=`cat RunstoDownload.dat | wc -l `
										# fi
										# if [ $NFILESTMP = 50 ]; then
										# 	printf "\e[36mStart download $NFILESTMP files:\e[0m \n"
										# 	cmd="alien_cp -input RunstoDownload.dat"
										# 	debug "$cmd\n"
										# 	eval $cmd 
										# 	debug "\e[36mReturned:\e[0m $?\n"
										# 	printf "" > RunstoDownload.dat
										# fi
									fi
								done
							fi
						done
						debug "\n"
						NFILESTMP=0
						if [ -e RunstoDownload.dat ]; then
							NFILESTMP=`cat RunstoDownload.dat | wc -l `
						fi
						if [ $NFILESTMP = 0 ]; then
							printf "\e[32mAll runs exist\e[0m \n"
						else
							# cat RunstoDownload.dat
							printf "\e[36mStart download $NFILESTMP files:\e[0m \n"
							cmd="alien_cp -input RunstoDownload.dat"
							debug "$cmd\n"
							eval $cmd 
							debug "\e[36mReturned:\e[0m $?\n"
							if [ -e RunstoDownload.dat ] ; then rm RunstoDownload.dat; fi
							# printf "" > RunstoDownload.dat
						fi
					done
					if [ $foundtmp = 0 ]; then 
						envFile=`find "$DownDir/" -regex  ".*$childsearch/env.sh"`
						# debug "$envFile\n"
						trainstring=${envFile#*$trainnum/}
						trainstring=${trainstring%%/env.sh*}
						debug "$trainstring\n"

						# tosearch=$trainstring'/lego_train.jdl'
						# printf "\e[33mDownload: lego_train.jdl ($tosearch)\e[0m \n"
						# if [[ $debug = 1 ]]
						# then
						# 	GetFile "$tosearch" "$link" "$DownDir"
						# else
						# 	GetFile "$tosearch" "$link" "$DownDir" > /dev/null
						# fi

						# debug "$DownDir/${trainstring}/lego_train.jdl\n"
						# runsarray=`grep "RunChunks" $DownDir/${trainstring}/lego_train.jdl | awk -F "\"" '{print $2}'`
						# printf "$runsarray\n"

						# unset LIST_runsfromarray
						# IFS=',' LIST_runsfromarray=( $runsarray )
						# IFS=$OLDIFS

						child=$trainstring
						GetChildName 

						Type='data'
						if (( `echo -n ${ChildName#LHC*} | wc -c` > 3 )); then
							Type='sim'
						fi
						Year="20${ChildName:3:2}"

						# /alice/data/2018/LHC18m/000291702/pass1_withTRDtracking/AOD234/PWGGA/GA_pp_AOD/1916_20210812-1232_child_11/GammaCalo_800.root
						# for run in "${LIST_runsfromarray[@]}"
						# do
							debug "\e[33m|->\e[0m Type: $Type; Year: $Year; periodName: $ChildName;  \n"
							for file in "${LIST_FilesGrid[@]}"; do
								# tosearch=".*$run/.*/.*/PWGGA/$TrainPagetmp/$trainstring/$file"
								tosearch=".*/.*/.*/PWGGA/$TrainPagetmp/$trainstring/$file"
								printf "\e[33mDownload: $file ($tosearch)\e[0m \n"
								GetFile "$tosearch" "/alice/$Type/$Year/$ChildName" "$DownDir/$child"
							done
						# done
					fi
				fi
			done
		done
	done
}

MergeTrains(){
	echo  -e "\e[36m------------------------------------\e[0m" 
	echo "Start merging trains: ${BASEDIR}/${mergedtrainname}" 
	echo  -e "\e[36m------------------------------------\e[0m" 
	for RunlistName in "${LIST_Runlists[@]}"
	do
		echo  -e "\t\e[36m|-> \e[0m $RunlistName " 
		for filename in "${LIST_FilesGrid[@]}"
		do
			printf "\e[33m------------------------------------\n$filename\n\e[0m" 
			Outfile=${BASEDIR}/${mergedtrainname}/$RunlistName/$filename
			mkdir -p ${BASEDIR}/${mergedtrainname}/$RunlistName
			if [ -e $Outfile ] && [ $redomerging = 0 ]; then
				printf "$Outfile exists, skip merging..\n"
			else
				haddCommand="hadd -f -j 4 $Outfile "
				printf "\e[36m|-> \e[0m" 
				Ntrains=0
				for TrainNumber in "${LIST_TrainID[@]}"
				do
					((Ntrains++))
					printf "$Ntrains:$TrainNumber, " 
					singletrainDir=".$TrainPage-$TrainNumber"
					filedir="${BASEDIR}/$singletrainDir/$RunlistName/$filename"
					haddCommand="${haddCommand}$filedir "
				done
				printf "\n"
				if [ $Ntrains -gt 1 ]; then
					debug "$haddCommand\n"
					eval $haddCommand
					printf "\e[36mReturned:\e[0m $?\n"
				else 
					printf "\t\e[33m|-> link \e[0m $filedir -> $Outfile\n"
					ln -sf $filedir $Outfile
				fi			
			fi				
		done
	done
}


PergeRuns() {
	echo -e "\e[33m|-> \e[0m perge runs" | tee -a $LogFile
	for TrainNumber in "${LIST_TrainID[@]}"
	do
		for RunlistName in "${LIST_Runlists[@]}"
		do
			printf "\e[33m|->\e[0m $BASEDIR/.$TrainPage-$TrainNumber/$TrainNumber*/$RunlistName\n"
			printf "\t\e[33m|->  $RunlistName \e[0m\n" 
			for TMPfolder in `ls $BASEDIR/.$TrainPage-$TrainNumber/ | grep $TrainNumber`; do
				printf "\t\t\e[32m|-> $TMPfolder\e[0m\n"
				for TMPfolder2 in `ls $BASEDIR/.$TrainPage-$TrainNumber/$TMPfolder/$RunlistName`; do
					if [[ $TMPfolder2 =~ $re ]] #&& [[ `echo $TMPfolder | wc -c` = 7 ]]
					then
						printf "$TMPfolder2, "
						rm -r $BASEDIR/.$TrainPage-$TrainNumber/$TMPfolder/$RunlistName/$TMPfolder2
					fi
				done
				printf "\n\n"
			done
			printf "\n\n"
		done
	done
}