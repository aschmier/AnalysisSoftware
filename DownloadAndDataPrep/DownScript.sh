#! /bin/bash

source DownScript_header.sh

Init
ParseSettings $@

if [[ $DoDown = 1 ]] || [[ $DoSoftDown = 1 ]]
then
	for TrainNumber in `cat $TrainNumberFile`
	do
		InitilaizeTrain $TrainNumber
		# Loop over all childs
		for child in "${LIST_child[@]}"
		do
			InitilaizeChild $child
			Parsehtml
			if [[ $DoDown = 0 ]]; then continue;fi


			arrypos=0
			for RunlistName in "${LIST_RunlistName[@]}"
			do
				InitilaizeRunlist $arrypos
				if [[ $? -eq 1 ]]; then
					# echo "___________HERE________________"
					continue
				fi
				# check if merging was successful
				forcemerge=0
				if [[ ! $useSpecificRunlist = 1 ]]; then
					AvailibleFiles="$Dirout/AvailibleFiles.txt"
					touch $AvailibleFiles
					SeachFilesOnAlienAndAddToList "$AlienDir$child/merge_runlist_${RunlistID}/" $AvailibleFiles
					echo;
				else
					forcemerge=1
					printf "\e[33m|-> is specific Runlist (no download)"
				fi
				# Download all relevant files
				for filename in `cat $FilenamesinTrain`
				do
					MergeRuns=0

					tmp=`grep $filename $AvailibleFiles | wc -l`
					if [[ $tmp > 0 ]]; then
						printf "\e[33m|-> Found files:\e[0m $RunlistName $ChildName $filename"
						outFile="$Dirout/$filename"
						inFile="$Dirin/$filename"
						downlogFile="$Dirout/${filename%%.root}.downlog"

						GetFile $inFile $outFile $downlogFile
						if [[ $UseMerge = 1 ]]
						then
							doMergeFiles $filename $Dirout $Dirin $DirMerged
						else
							printf "\n"
						fi
					else
						MergeRuns=1
						echo  -e "\e[33mWARNING: File $filename not found\e[0m, trying to merge from runwise output: ChildName = $ChildName, childID = $childID, Runlist $RunlistID, Name = $RunlistName" | tee -a $WARNINGLog
					fi

					echo  -e "\e[36m------------------------------------\e[0m"


					if [[ $forcemerge = 1 ]]; then
						MergeRuns=1
					fi

					if [[ $UseOnlyrunwise = 1 ]]; then
						MergeRuns=0
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

							RunFileList="$runDir/.FileList.txt"
							RunPathList="$runDir/PathList.txt"
							SubRunFileList="$runDir/subFileList.txt"
							SubRunPathList="$runDir/subPathList.txt"

							mkdir -p $OUTPUTDIR/.$child
							mkdir -p $runDir

							if [[ -d $runDir ]] && [[ ! -L $OUTPUTDIR/$RunlistName/$ChildName/$runName ]] ; then
								cmd="ln -sf $runDir $OUTPUTDIR/$RunlistName/$ChildName/"
								# echo $cmd
								usecmd $cmd
								eval $cmd
							fi
							# echo "001 $cmd"

							if [ $newfiles = 1 ] || [ ! -f $RunPathList ]; then
								if [[ $Type = "sim" ]]; then
									FindPathsAndAddToList "/alice/$Type/$Year/$ChildName/$runName/" $RunPathList
								else
									FindPathsAndAddToList "/alice/$Type/$Year/$ChildName/000$runName/" $RunPathList
								fi
								# grep _$childID/AnalysisResults.root  > $RunPathList.tmp
							fi
							if [ ! -f $RunPathList ]; then
								echo -e "\t\e[31mError\e[0m Run $runName wasn't found!" | tee -a $ErrorLog
								continue
							fi
							arrypos2=0
							isfirsttmp=1
							Search=$filename
							UseMerge=$MergeRuns
							((arrypos2++))
							runoutFile=$runDir/$Search
							rundownlogFile=$runDir/.${Search%%.root}.downlog
							if [[ $isfirsttmp = 1 ]] ; then
								isfirsttmp=0
								printf "\tProcessing Run\t$tmpruncount/$NumberOfRuns\t$runName\t$Search "
							else
								printf "\t\t\t\t\t$Search "
							fi
							if [[ `grep "_$childID/$Search" $RunPathList | wc -c` -gt 0 ]]; then
								for runinFile in `grep "_$childID/$Search" $RunPathList`
								do
									# printf "\t$runoutFile"

									GetFile $runinFile $runoutFile $rundownlogFile
									if [[ $UseMerge = 1 ]]
									then
										doMergeFiles $Search $runDir ${runinFile/%%$Search} $Dirout
									else
										printf "\n"
									fi
								done
							else
								# printf "\n"
								printf "\t\t\e[33mWARNING:  No file found \e[0m, trying to merge from higher stages \n" #| tee -a $WARNINGLog
								maxcount=`grep "$Search" $RunPathList | wc -l`
								tmpsubruncount=0
								for subruninFile in `grep "$Search" $RunPathList`
								do
									subrunname=${subruninFile%%$Search}
									subrunname="${subrunname#*child_*/}"
									subrunname="${subrunname%/*}"
									if [[ $subrunname = "" ]]; then continue; fi
									subrunDir=$runDir/$subrunname/
									mkdir -p $subrunDir
									subrunoutFile=$subrunDir/$Search
									subrundownlogFile=$subrunDir/.${Search%%.root}.downlog
									((tmpsubruncount++))
									printf "\t\tProcessing SubRun\t$tmpsubruncount/$maxcount\t$runName|$subrunname\t$Search "
									GetFile $subruninFile $subrunoutFile $subrundownlogFile
									if [[ $? -eq 1 ]]; then
										continue
									fi
									if [[ $UseMerge = 1 ]]
									then
										doMergeFiles $Search $subrunDir ${subruninFile/%%$Search} $runDir
									else
										printf "\n"
									fi
								done
								echo -e "\t\t\e[33m|->\e[0m merge from higher stages.. done" #| tee -a $WARNINGLog
							fi
						done
					fi
				done
			done
		done
	done
fi


for RunlistName in `cat $OptRunlistNamefile`
do
	if [[ $RunlistName = "useSpecificRunlist_"* ]]
	then
		SpecificRunlist=${RunlistName#*useSpecificRunlist_}
		echo $SpecificRunlist  | tee -a $OptRunlistNamefile.tmp
	else
		echo $RunlistName  | tee -a $OptRunlistNamefile.tmp
	fi
done
new_rm $OptRunlistNamefile
mv $OptRunlistNamefile.tmp $OptRunlistNamefile

echo;echo;echo;
if [[ $MergeTrains = 1 ]]
then
	doMergeTrains
fi
echo;echo;echo;
if [[ $MergeTrainsPeriods = 1 ]]
then
	doMergeTrainsPeriods
fi
echo;echo;echo;
if [[ $MergeTrainsRuns = 1 ]] && [[ $Userunwise = 1 ]]
then
	doMergeTrainsRuns
fi

Finish
