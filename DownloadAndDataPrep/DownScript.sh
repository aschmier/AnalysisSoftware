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
					# echo "___________HERE________________" | tee -a $LogFile
					continue
				fi
				# check if merging was successful
				forcemerge=0
				if [[ ! $useSpecificRunlist = 1 ]]; then
					AvailibleFiles="$Dirout/AvailibleFiles.txt"
					touch $AvailibleFiles
					if [[ $OptNoRunlist = 0 ]]; then
						SeachFilesOnAlienAndAddToList "$AlienDir$child/merge_runlist_${RunlistID}/" $AvailibleFiles
					else
						SeachFilesOnAlienAndAddToList "$AlienDir$child/merge/" $AvailibleFiles
					fi
					# merge
					echo;
				else
					forcemerge=1
					printf "\e[33m|-> is specific Runlist (no download)" | tee -a $LogFile
				fi
				if [[ $OptZip = 1 ]]; then
					filename="root_archive.zip"
					printf "\e[33m|-> Found files:\e[0m $RunlistName $ChildName $filename" | tee -a $LogFile
					outFile="$Dirout/$filename"
					inFile="$Dirin/$filename"
					downlogFile="$Dirout/${filename%%.root}.downlog"
					if [[ $isjalien = 1 ]]; then
						GetFile_jalien $inFile $outFile $downlogFile
					else
						GetFile $inFile $outFile $downlogFile
					fi
					if [[ -f $outFile ]]; then
						if [[ ! -f "$outFile.zipped" ]]; then
							if [[ $debug = 1 ]] || [[ $debug = 2 ]]
							then
								unzip -o $outFile -d $Dirout/ | tee -a $LogFile
							else
								unzip -o $outFile -d $Dirout/ >> $LogFile
							fi
							touch "$outFile.zipped"
							echo;
						else
							printf "\e[33m|-> Files was unzipped already :\e[0m $RunlistName $ChildName $filename\n" | tee -a $LogFile
						fi
					else
						echo;
					fi
				fi

				# Download all relevant files
				for filename in `cat $FilenamesinTrain`
				do
					echo  -e "\e[36m------------------------------------\e[0m" | tee -a $LogFile
					MergeRuns=0

					tmp=`grep $filename $AvailibleFiles | wc -l`
					# completlymerged="$Dirout/${filename%%.root}.completlymerged"
					if [[ $tmp > 0 ]]; then
						printf "\e[33m|-> Found files:\e[0m $RunlistName $ChildName $filename" | tee -a $LogFile
						outFile="$Dirout/$filename"
						inFile="$Dirin/$filename"
						downlogFile="$Dirout/${filename%%.root}.downlog"

						if [[ $UseOnlyrunwise = 0 ]]; then
							if [[ $isjalien = 1 ]]; then
								GetFile_jalien $inFile $outFile $downlogFile
							else
								GetFile $inFile $outFile $downlogFile
							fi
							if [[ $UseMerge = 1 ]] && [[ ! $filename = *".zip" ]]
							then
								doMergeFiles $filename $Dirout $Dirin $DirMerged
							else
								printf "\n" | tee -a $LogFile
							fi
						else
							printf "\n" | tee -a $LogFile
						fi
					else
						if [[ $UseOnlyrunwise = 0 ]]; then
							# if [[ ! -f $completlymerged ]] || [[ $newfiles = 1 ]]; then
								MergeRuns=1
								echo  -e "\e[33mWARNING: File $filename not found\e[0m, trying to merge from runwise output: ChildName = $ChildName, childID = $childID, Runlist $RunlistID, Name = $RunlistName" | tee -a $WARNINGLog | tee -a $LogFile
							# else
								# echo  -e "\e[33mWARNING:\e[0m File was merged from runwise output previously: ChildName = $ChildName, childID = $childID, Runlist $RunlistID, Name = $RunlistName" | tee -a $WARNINGLog | tee -a $LogFile
							# fi
						fi
					fi

					if [[ $DoSoftDown = 1 ]]; then
						printf "\e[33m|-> Merging file:\e[0m $RunlistName $ChildName $filename " | tee -a $LogFile
						doMergeFiles $filename $Dirout $Dirin $DirMerged
						continue
					fi


					if [[ $forcemerge = 1 ]]; then
						MergeRuns=1
					fi

					if [[ $UseOnlyrunwise = 1 ]]; then
						MergeRuns=0
					fi
					if [[ $Userunwise = 1 ]] || [[ $MergeRuns = 1 ]]; then
						# Download all runs
						AllPathList="$OUTPUTDIR/.$child/.AllPathList_$filename.txt"
						if [[ ! -f $AllPathList ]] || [[  `grep "$Search" $AllPathList | wc -l` < 1 ]]; then
							newfiles=1
							FindPathsAndAddToList "/alice/$Type/$Year/$ChildName/" $AllPathList $filename
						fi
						if [[ $OptIsJJ = 1 ]]; then
							DiroutTmp=$Dirout
						fi
						for (( pthardbin = $minpthardbins; pthardbin <= $maxpthardbins; pthardbin++ )); do
							tmpruncount=0
							for runName in `cat $Runlist`; do
								((tmpruncount++))
								merginghigher=0

								# look for availible Files
								if [[ $OptIsJJ = 1 ]]; then
									runDir=$OUTPUTDIR/.$child/$pthardbin/$runName
									Dirout=$OUTPUTDIR/.$child/$pthardbin
								else
									runDir=$OUTPUTDIR/.$child/$runName
								fi

								RunFileList="$runDir/.FileList.txt"
								RunPathList="$runDir/PathList.txt"
								SubRunFileList="$runDir/subFileList.txt"
								SubRunPathList="$runDir/subPathList.txt"

								mkdir -p $OUTPUTDIR/.$child
								mkdir -p $runDir

								if [[ $OptIsJJ = 1 ]]; then
									if [[ -d $Dirout ]] && [[ ! -L $OUTPUTDIR/$RunlistName/$ChildName/$runName ]] ; then
										cmd="ln -sf $Dirout $OUTPUTDIR/$RunlistName/$ChildName/."
										# echo $cmd | tee -a $LogFile
										usecmd $cmd
										eval $cmd
									fi
								else
									if [[ -d $runDir ]] && [[ ! -L $OUTPUTDIR/$RunlistName/$ChildName/$runName ]] ; then
										cmd="ln -sf $runDir $OUTPUTDIR/$RunlistName/$ChildName/."
										# echo $cmd | tee -a $LogFile
										usecmd $cmd
										eval $cmd
									fi
								fi

								# echo "001 $cmd" | tee -a $LogFile

								if [[ ! -f $RunPathList ]]; then
									newfiles=1
								elif [[  `grep "$Search" $RunPathList | wc -l` < 1 ]]; then
									newfiles=1
								fi

								if [[ $newfiles = 1 ]]; then
									if [[ $Type = "sim" ]]; then
										if [[ $OptIsJJ = 1 ]]; then
											GrapPathsAndAddToList "/alice/$Type/$Year/$ChildName/$pthardbin/$runName/" $RunPathList $AllPathList
										else
											GrapPathsAndAddToList "/alice/$Type/$Year/$ChildName/$runName/" $RunPathList $AllPathList
										fi
									else
										GrapPathsAndAddToList "/alice/$Type/$Year/$ChildName/000$runName/" $RunPathList $AllPathList
									fi
									# grep _$childID/AnalysisResults.root  > $RunPathList.tmp
								fi
								if [ ! -f $RunPathList ]; then
									if [[ $OptIsJJ = 1 ]]; then
										# printf "\tProcessing Run\t$tmpruncount/$NumberOfRuns\t$runName\t$filename \n " | tee -a $LogFile
										echo -e "\t\e[31mError\e[0m pThardbin $pthardbin, Run\t$tmpruncount/$NumberOfRuns\t$runName  wasn't found!" | tee -a $ErrorLog | tee -a $LogFile
									else
										echo -e "\t\e[31mError\e[0m Run $runName wasn't found!" | tee -a $ErrorLog | tee -a $LogFile
									fi
									continue
								fi
								arrypos2=0
								isfirsttmp=1
								Search=$filename
								# UseMerge=$MergeRuns
								((arrypos2++))
								if [[ $OptIsJJ = 1 ]]; then
									runoutFile="$runDir/$Search"
									rundownlogFile="$runDir/.${Search%%.root}.downlog"
								else
									runoutFile=$runDir/$Search
									rundownlogFile=$runDir/.${Search%%.root}.downlog
								fi
								if [[ $OptIsJJ = 1 ]]; then
									printf "\t $pthardbin/$maxpthardbins "| tee -a $LogFile
								fi
								printf "\tProcessing Run\t$tmpruncount/$NumberOfRuns\t$runName\t$Search " | tee -a $LogFile
								if [[ `grep "_$childID/$Search" $RunPathList | wc -c` -gt 0 ]]; then
									for runinFile in `grep "_$childID/$Search" $RunPathList`
									do
										if [[ $OptZip = 1 ]]; then
											filenameZip="root_archive.zip"
											# outdirZiptmp=${outFile/%%$Search}
											# outdirZip=${outdirZiptmp/%%$filenameZip}
											outFileZip="$runDir/$filenameZip"
											inFileZip="${runinFile%%$filename}/$filenameZip"
											downlogFileZip="$runDir/${filenameZip}.downlog"
											if [[ ! -f $outFileZip ]]; then
											printf "\e[33m|-> download $filenameZip:\e[0m  " | tee -a $LogFile
												if [[ $isjalien = 1 ]]; then
													GetFile_jalien $inFileZip $outFileZip $downlogFileZip
												else
													GetFile $inFileZip $outFileZip $downlogFileZip
												fi
												if [[ -f $outFileZip ]]; then
													if [[ ! -f "$outFileZip.zipped" ]]; then
														if [[ $debug = 1 ]] || [[ $debug = 2 ]]
														then
															unzip -o $outFileZip -d ${runDir}/ | tee -a $LogFile
														else
															unzip -o $outFileZip -d ${runDir}/ >> $LogFile
														fi
														touch "$outFileZip.zipped"
														echo;
													# else
														# printf "\e[33m|-> Files was unzipped already :\e[0m " | tee -a $LogFile
													fi
												fi
												printf "\t\t\t\t\t\t  " | tee -a $LogFile
											fi
										fi
										# printf "\t$runoutFile" | tee -a $LogFile
										if [[ $isjalien = 1 ]]; then
											GetFile_jalien $runinFile $outFile $rundownlogFile
										else
											GetFile $runinFile $runoutFile $rundownlogFile
										fi
										if [[ $MergeRuns = 1 ]]
										then
											PrepMerge $Search $runDir ${runinFile/%%$Search} $Dirout
										else
											printf "\n" | tee -a $LogFile
										fi
									done
								else
									# printf "\n" | tee -a $LogFile
									if [[ $OptIsJJ = 0 ]]; then
										printf "\t\t\e[33mWARNING:  No file found \e[0m, trying to merge from higher stages \n" #| tee -a $WARNINGLog | tee -a $LogFile
									else
										printf " \e[33mWARNING:  No file found \e[0m, trying to merge from higher stages \n" #| tee -a $WARNINGLog | tee -a $LogFile
									fi
									maxcount=`grep "$Search" $RunPathList | wc -l`
									tmpsubruncount=0
									if [[ $maxcount -eq 0 ]]; then
										echo -e "\t\t\e[33m|->\e[0m merge from higher stages failed ($runName)" | tee -a $WARNINGLog | tee -a $LogFile
										cat $RunPathList | tee -a $LogFile
										echo  -e "\e[36m------------------------------------\e[0m" | tee -a $LogFile
										continue
									fi
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
										printf "\t\tProcessing SubRun\t$tmpsubruncount/$maxcount\t$runName|$subrunname\t$Search " | tee -a $LogFile
										if [[ $OptZip = 1 ]]; then
											filenameZip="root_archive.zip"
											# outdirZiptmp=${outFile/%%$Search}
											# outdirZip=${outdirZiptmp/%%$filenameZip}
											outFileZip="$subrunDir/$filenameZip"
											inFileZip="${subruninFile%%$Search}/$filenameZip"
											downlogFileZip="$subrunDir/${filenameZip}.downlog"
											if [[ ! -f $outFileZip ]]; then
												printf "\e[33m|-> download $filenameZip:\e[0m  " | tee -a $LogFile
												if [[ $isjalien = 1 ]]; then
													GetFile_jalien $inFileZip $outFileZip $downlogFileZip
												else
													GetFile $inFileZip $outFileZip $downlogFileZip
												fi
												if [[ -f $outFileZip ]]; then
													if [[ ! -f "$outFileZip.zipped" ]]; then
														if [[ $debug = 1 ]] || [[ $debug = 2 ]]
														then
															unzip -o $outFileZip -d ${subrunDir}/ | tee -a $LogFile
														else
															unzip -o $outFileZip -d ${subrunDir}/ >> $LogFile
														fi
														touch "$outFileZip.zipped"
														echo;
													# else
														# printf "\e[33m|-> Files was unzipped already :\e[0m " | tee -a $LogFile
													fi
												fi
												printf "\t\t\t\t\t\t  " | tee -a $LogFile
											fi
										fi
										if [[ $isjalien = 1 ]]; then
											GetFile_jalien $subruninFile $subrunoutFile $subrundownlogFile
										else
											GetFile $subruninFile $subrunoutFile $subrundownlogFile
										fi
										# if [[ $? -eq 1 ]]; then
										# 	continue
										# fi
										PrepMerge $Search $subrunDir ${subruninFile/%%$Search} $runDir
									done
									printf "\t\t\t\e[33m|->\e[0m StartMergeProcess" #| tee -a $WARNINGLog | tee -a $LogFile
									StartMergeProcess $runDir &> /dev/null
									echo -e "\t\t\e[33m|->\e[0m merge from higher stages.. done" #| tee -a $WARNINGLog | tee -a $LogFile
									if [[ $MergeRuns = 1 ]]
									then
										printf "\t\t\e[33m|->\e[0m merging run " #| tee -a $WARNINGLog | tee -a $LogFile
										if [[ $debug = 1 ]] || [[ $debug = 2 ]]
										then
											echo "doMergeFiles $Search $runDir ${runinFile/%%$Search} $Dirout" | tee -a $LogFile
										fi
										PrepMerge $Search $runDir ${runinFile/%%$Search} $Dirout
									fi
								fi
							done
							# printf "\t\t\e[33m|->\e[0m waiting: merge from higher stages..." #| tee -a $WARNINGLog | tee -a $LogFile
							# wait
							# printf "\t done \n\t\t" #| tee -a $WARNINGLog | tee -a $LogFile
							printf "\t\t" #| tee -a $WARNINGLog | tee -a $LogFile
							StartMergeProcess $Dirout
							if [[ $OptIsJJ = 1 ]] ; then
								if [[ $MergeRuns = 1 ]]; then
									printf "\t\t\e[33m|->\e[0m merging pT hardbin " #| tee -a $WARNINGLog | tee -a $LogFile
									if [[ $debug = 1 ]] || [[ $debug = 2 ]]
									then
										echo "doMergeFiles $Search $Dirout 0 $DiroutTmp" | tee -a $LogFile
									fi
									PrepMerge $Search $Dirout "0" $DiroutTmp
								fi
								Dirout=$DiroutTmp
							fi
						done
						printf "\t" #| tee -a $WARNINGLog | tee -a $LogFile
						StartMergeProcess $DiroutTmp
					fi
					if [[ $UseMerge = 1 ]] ; then
						# touch $completlymerged
						printf "\e[33m|-> Merging file:\e[0m $RunlistName $ChildName $filename " | tee -a $LogFile
						doMergeFiles $filename $Dirout $Dirin $DirMerged
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
		echo $SpecificRunlist  | tee -a $OptRunlistNamefile.tmp | tee -a $LogFile
	else
		echo $RunlistName  | tee -a $OptRunlistNamefile.tmp | tee -a $LogFile
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
