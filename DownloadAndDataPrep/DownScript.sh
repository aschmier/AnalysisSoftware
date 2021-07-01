#! /bin/bash

source DownScript_header.sh

Init
ParseSettings $@

if [[ $DoDown = 1 ]] || [[ $DoSoftDown = 1 ]]
then
	for TrainNumber in `cat $TrainNumberFile`
	do
		InitilaizeTrain $TrainNumber
		# safty=
		# if [[ !$safty ]]; then
		# 	return
		# fi

		# Loop over all childs
		NChilds=0
		for child in "${LIST_child[@]}"
		do
			((NChilds++))
			InitilaizeChild $child
			if [[ $OptSpecificChild = 1 ]]; then
			# cat $Childsfile
				if [[ `grep  "Child $childID|" $Childsfile | wc -l` < 1 ]]; then
					printf "\e[33m|-> skipping child $childID" | tee -a $LogFile
					continue
				fi
			fi
			Parsehtml
			if [[ $DoDown = 0 ]]; then continue;fi
			# echo "___________HERE________________" | tee -a $LogFile



			arrypos=0
			for RunlistName in "${LIST_RunlistName[@]}"
			do
				# echo "___________HERE________________" | tee -a $LogFile
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
					if [[ $OptNoRunlistChild = 0 ]]; then
						SeachFilesOnAlienAndAddToList "$AlienDir$child/merge_runlist_${RunlistID}/" $AvailibleFiles
					else
						SeachFilesOnAlienAndAddToList "$AlienDir$child/merge/" $AvailibleFiles
					fi
					# merge
					echo;
					if [[ $OptZip = 1 ]]; then
						filename="root_archive.zip"
						printf "\e[33m|-> Found zip file:\e[0m $RunlistName $ChildName $filename" | tee -a $LogFile
						outFile="$Dirout/$filename"
						inFile="$Dirin/$filename"
						downlogFile="$Dirout/${filename%%.root}.downlog"
						GetFileZip $inFile $outFile $downlogFile $Dirout
						echo;
					fi
				else
					forcemerge=1
					printf "\e[33m|-> is specific Runlist (no download)\n" | tee -a $LogFile
				fi

				# Download all relevant files
				for filename in `cat $FilenamesinTrain`
				do
					echo  -e "\e[36m------------------------------------\e[0m" | tee -a $LogFile
					MergeRuns=0

					if [[ ! $useSpecificRunlist = 1 ]]; then

						tmp=`grep $filename $AvailibleFiles | wc -l`
						# completlymerged="$Dirout/${filename%%.root}.completlymerged"
						if [[ $tmp > 0 ]]; then
							printf "\e[33m|-> Found files:\e[0m $RunlistName $ChildName $filename" | tee -a $LogFile
							outFile="$Dirout/$filename"
							inFile="$Dirin/$filename"
							downlogFile="$Dirout/${filename%%.root}.downlog"

							if [[ $UseOnlyrunwise = 0 ]]; then
								GetFile $inFile $outFile $downlogFile
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
								if [[ $OptNorunwise = 0 ]]; then

								# if [[ ! -f $completlymerged ]] || [[ $newfiles = 1 ]]; then
									MergeRuns=1
									echo  -e "\e[33mWARNING: File $filename not found\e[0m, trying to merge from runwise output: ChildName = $ChildName, childID = $childID, Runlist $RunlistID, Name = $RunlistName" | tee -a $WARNINGLog | tee -a $LogFile
								# else
									# echo  -e "\e[33mWARNING:\e[0m File was merged from runwise output previously: ChildName = $ChildName, childID = $childID, Runlist $RunlistID, Name = $RunlistName" | tee -a $WARNINGLog | tee -a $LogFile
								# fi
								else
									echo  -e "\e[33mWARNING: File $filename not found\e[0m, trying to merge from runwise disabled" | tee -a $WARNINGLog | tee -a $LogFile
									MergeRuns=0
								fi
							fi
						fi

						if [[ $DoSoftDown = 1 ]]; then
							printf "\e[33m|-> Merging file:\e[0m $RunlistName $ChildName $filename " | tee -a $LogFile
							doMergeFiles $filename $Dirout $Dirin $DirMerged
							continue
						fi
					else
						printf "\e[33m|-> Processing files:\e[0m $filename \n" | tee -a $LogFile
					fi


					if [[ $forcemerge = 1 ]]; then
						MergeRuns=1
					fi

					if [[ $UseOnlyrunwise = 1 ]]; then
						MergeRuns=0
					fi
					if [[ $Userunwise = 1 ]] || [[ $MergeRuns = 1 ]]; then
						# Download all runs
						AllPathList="$OUTPUTDIR/.$child/.AllPathList.txt"
						NorunwiseinTrainFile="$OUTPUTDIR/.$child/.NorunwiseinTrainFile.txt"
						# printf "\e[36m--  FindPathsAndAddToList \e[0m" | tee -a $LogFile
						if [[ ! -f $AllPathList ]] || [[ `cat $AllPathList | wc -l ` < 1 ]]; then
							printf "\e[36m--  FindPathsAndAddToList --  create new \e[0m \n" | tee -a $LogFile
						elif [[  `grep "$filename" $AllPathList | wc -l` < 1 ]]; then
							printf "\e[36m--  FindPathsAndAddToList --  add $filename \e[0m \n" | tee -a $LogFile
							# new_rm $AllPathList.tmp
							# for dirtomodi in `cat $AllPathList`; do
							# 	echo "${dirtomodi%/*}/$filename" >> $AllPathList.tmp
							# done
							# AddToList $AllPathList.tmp $AllPathList
							# new_rm $AllPathList.tmp
						else
							printf "\e[36m--  FindPathsAndAddToList --  exists \e[0m \n" | tee -a $LogFile
						fi
						if [[ $NorunwiseinTrain = 1 ]]; then
							echo -e "\t no runwise output availible" | tee -a $ErrorLog  >> $rundownlogFile.tmp
							RLextension="/$RunlistName"
							NorunwiseinTrainfirstrundone=0
							NorunwiseinTrain=1
							if [[ ! -f $AllPathList ]]  || [[  `grep "$filename" $AllPathList 2> /dev/null | wc -l` < 1 ]]; then
								FindPathsAndAddToListnew $Dirin $AllPathList $filename
							fi
						else
							if [[ ! -f $AllPathList ]]  || [[  `grep "$filename" $AllPathList 2> /dev/null | wc -l` < 2 ]]; then
								if [[ $Type = "sim" ]]; then
									FindPathsAndAddToList "/alice/$Type/$Year/$ChildNameonline/" $AllPathList $filename $Runlist ""
								else
									FindPathsAndAddToList "/alice/$Type/$Year/$ChildNameonline/" $AllPathList $filename $Runlist "000"
								fi
							fi
						fi
						if [[ -f $AllPathList ]] && [[  `grep "$filename" $AllPathList 2> /dev/null | wc -l` < 1 ]]; then
							new_rm $AllPathList.tmp
							for dirtomodi in `cat $AllPathList`; do
								echo "${dirtomodi%/*}/$filename" >> $AllPathList.tmp
							done
							AddToList $AllPathList.tmp $AllPathList
							new_rm $AllPathList.tmp
						fi
						# echo; cat $AllPathList; echo; echo
						if [[ $OptIsJJ = 1 ]]; then
							DiroutTmp=$Dirout
						fi

						if [ ! -f $AllPathList ]; then
							echo -e "\t\e[31mError\e[0m Meging failed, no paths found!" | tee -a $ErrorLog  >> $rundownlogFile.tmp
							continue
						fi
						tmpruncount=0
						tmpruncount2=0
						for (( pthardbin = $minpthardbins; pthardbin <= $maxpthardbins; pthardbin++ )); do
							if [[ $OptIsJJ = 1 ]]; then
								printf "\e[33m|->\e[0m Child: $childID/$NChildsAll, pThard: $pthardbin/$maxpthardbins \n"  #| tee -a $LogFile
							fi
							tmpruncount=0
							tmpruncount2=0
							NorunwiseinTrainfirstrundone=0
							for runName in `cat $Runlist`; do
								if [[ $tmpruncount2 = $ParalellDownloads ]]; then
									tmptmpnrunsbegin=$(expr $tmpruncount - $tmpruncount2)
									printf "\t\e[33m|->\e[0m waiting for runs $tmptmpnrunsbegin - $tmpruncount of $NumberOfRuns runs\n" #| tee -a $WARNINGLog | tee -a $LogFile
									wait
									tmpruncount2=0
								fi
								if [[ $NorunwiseinTrain = 1 ]] && [[ $NorunwiseinTrainfirstrundone = 1 ]]; then
									continue
								fi
								((tmpruncount++))
								((tmpruncount2++))
								merginghigher=0
								if [[ $NorunwiseinTrain = 1 ]]; then
									runDir=$OUTPUTDIR/.$child
								else
									# look for availible Files
									if [[ $OptIsJJ = 1 ]]; then
										runDir=$OUTPUTDIR/.$child/$pthardbin/$runName
										Dirout=$OUTPUTDIR/.$child/$pthardbin
									else
										runDir=$OUTPUTDIR/.$child/$runName
									fi
								fi


								RunFileList="$runDir/.FileList.txt"
								RunPathList="$runDir/PathList.txt"
								SubRunFileList="$runDir/subFileList.txt"
								SubRunPathList="$runDir/subPathList.txt"

								mkdir -p $OUTPUTDIR/.$child
								mkdir -p $runDir


								if [[ $NorunwiseinTrain = 0 ]]; then
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
								fi

								# echo "001 $cmd" | tee -a $LogFile
								newfilestmp2=0
								if [[ ! -f $RunPathList ]]; then
									newfilestmp2=1
								elif [[  `grep "$Search" $RunPathList | wc -l` < 1 ]]; then
									newfilestmp2=1
								fi

								if [[ $newfilestmp2 = 1 ]]; then
									if [[ $NorunwiseinTrain = 1 ]] ; then
										GrapPathsAndAddToList $Dirin $RunPathList $AllPathList
									else
										if [[ $Type = "sim" ]]; then
											if [[ $OptIsJJ = 1 ]]; then
												GrapPathsAndAddToList "/alice/$Type/$Year/$ChildNameonline/$pthardbin/$runName/" $RunPathList $AllPathList
											else
												GrapPathsAndAddToList "/alice/$Type/$Year/$ChildNameonline/$runName/" $RunPathList $AllPathList
											fi
										else
											GrapPathsAndAddToList "/alice/$Type/$Year/$ChildNameonline/000$runName/" $RunPathList $AllPathList
										fi
									fi
									# grep _$childID/AnalysisResults.root  > $RunPathList.tmp
								fi
								if [ ! -f $RunPathList ]; then
									if [[ $OptIsJJ = 1 ]]; then
										# printf "\tProcessing Run\t$tmpruncount/$NumberOfRuns\t$runName\t$filename \n " | tee -a $LogFile
										echo -e "\t\e[31mError\e[0m pThardbin $pthardbin, Run\t$tmpruncount/$NumberOfRuns\t$runName  wasn't found!" | tee -a $ErrorLog  >> $rundownlogFile.tmp
									else
										echo -e "\t\e[31mError\e[0m Run $runName wasn't found!" | tee -a $ErrorLog  >> $rundownlogFile.tmp
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
								new_rm $rundownlogFile.tmp
								searchChildtmp="_$childID/$Search"
								if [[ $OptIsJJ = 1 ]]; then
									printf "\t Child: $childID/$NChildsAll, pThard: $pthardbin/$maxpthardbins " >> $rundownlogFile.tmp  #| tee -a $LogFile
								fi
								if [[ `echo $childID | wc -c` -gt 4 ]]; then
									searchChildtmp="$childID/$Search"
								fi

								if [[ $NorunwiseinTrain = 0 ]]; then
									printf "\tProcessing Run\t$tmpruncount/$NumberOfRuns\t$runName\t$Search "  >> $rundownlogFile.tmp
								fi
								# printf "\n ************************\n"
								# echo "$searchChildtmp"
								# cat $RunPathList
								# printf "\n ************************\n"
								if [[ `grep "$searchChildtmp" $RunPathList | wc -c` -gt 0 ]] && [[ $NorunwiseinTrain = 0 ]]; then
									for runinFile in `grep "$searchChildtmp" $RunPathList`; do
										if [[ $OptZipRun = 1 ]] && [[ ! -f $runoutFile ]]; then
											filenameZip="root_archive.zip"
											# outdirZiptmp=${outFile/%%$Search}
											# outdirZip=${outdirZiptmp/%%$filenameZip}
											outFileZip="$runDir/$filenameZip"
											inFileZip="${runinFile%%$filename}/$filenameZip"
											# printf "\tProcessing Run\t$tmpruncount/$NumberOfRuns\t$runName\t$Search " >> $rundownlogFile.tmp #  | tee -a $LogFile
											printf "\e[33m|-> download $filenameZip:\e[0m  " >> $rundownlogFile.tmp #  | tee -a $LogFile
											GetFileZip $inFileZip $outFileZip $rundownlogFile  ${runDir} >> $rundownlogFile.tmp && printf "\e[33m|->\e[0m $filename:" >> $rundownlogFile.tmp && GetFile $runinFile $runoutFile $rundownlogFile &>> $rundownlogFile.tmp &
										else
											GetFile $runinFile $runoutFile $rundownlogFile &>> $rundownlogFile.tmp &
										fi
									done
								else
									if [[ $OptZipRun = 1 ]]; then
										# printf "\t\e[33m|->\e[0m finish download zip files \n " #| tee -a $WARNINGLog | tee -a $LogFile
										wait
									fi
									if [[ -f $rundownlogFile.tmp ]]; then
										cat $rundownlogFile.tmp | tee -a $LogFile
										new_rm $rundownlogFile.tmp
									fi
									# printf "\n" | tee -a $LogFile
									if [[ $Optfast = 0 ]] || [[ $NorunwiseinTrain = 1 ]]; then
										if [[ $NorunwiseinTrain = 0 ]]; then
											if [[ $OptIsJJ = 0 ]]; then
												printf "\t\e[33mWARNING:  No file found \e[0m, trying to merge from higher stages" #| tee -a $rundownlogFile.tmp #| tee -a $WARNINGLog | tee -a $LogFile
											else
												printf " \e[33mWARNING:  No file found \e[0m, trying to merge from higher stages" #| tee -a $rundownlogFile.tmp #| tee -a $WARNINGLog | tee -a $LogFile
											fi
											printf "\t\e[33m|->\e[0m finish download runs " #| tee -a $WARNINGLog | tee -a $LogFile
											# printf "\t\e[33m|->\e[0m waiting for downloads of runs to finish first" #| tee -a $WARNINGLog | tee -a $LogFile
											wait
										fi
										if [[  `grep "$Search" $RunPathList | wc -l` < 1 ]]; then
											new_rm $RunPathList.tmp
											for dirtomodi in `cat $RunPathList`; do
												echo "${dirtomodi%/*}/$filename" >> $RunPathList.tmp
											done
											AddToList $RunPathList.tmp $RunPathList
											new_rm $RunPathList.tmp
										fi
										maxcount=`grep "$Search" $RunPathList | wc -l`
										tmpsubruncount=0
										if [[ $NorunwiseinTrain = 0 ]]; then
											if [[ $maxcount -eq 0 ]]; then
												echo -e "\t\t\e[33m|->\e[0m merge from higher stages failed ($runName)" | tee -a $WARNINGLog | tee -a $LogFile | tee -a $rundownlogFile.tmp
												if [[ $debug = 1 ]] || [[ $debug = 2 ]]
												then
													echo $AllPathList | tee -a $rundownlogFile.tmp
													cat $AllPathList | tee -a $LogFile | tee -a $rundownlogFile.tmp
													echo  -e "\e[36m------------------------------------\e[0m" | tee -a $LogFile | tee -a $rundownlogFile.tmp
													echo $RunPathList | tee -a $rundownlogFile.tmp
													cat $RunPathList | tee -a $LogFile | tee -a $rundownlogFile.tmp
												fi
												echo  -e "\e[36m------------------------------------\e[0m" | tee -a $LogFile | tee -a $rundownlogFile.tmp
												continue
											fi
										else
											if [[ $maxcount -eq 0 ]]; then
												echo -e "\t\t\e[33m|->\e[0m merge failed" | tee -a $WARNINGLog | tee -a $LogFile | tee -a $rundownlogFile.tmp
												if [[ $debug = 1 ]] || [[ $debug = 2 ]]
												then
													echo $AllPathList | tee -a $rundownlogFile.tmp
													cat $AllPathList | tee -a $LogFile | tee -a $rundownlogFile.tmp
													echo  -e "\e[36m------------------------------------\e[0m" | tee -a $LogFile | tee -a $rundownlogFile.tmp
													echo $RunPathList | tee -a $rundownlogFile.tmp
													cat $RunPathList | tee -a $LogFile | tee -a $rundownlogFile.tmp
												fi
												echo  -e "\e[36m------------------------------------\e[0m" | tee -a $LogFile | tee -a $rundownlogFile.tmp
												continue
											fi
										fi
										printf "\n" #| tee -a $WARNINGLog | tee -a $LogFile

										if [[ $OptZipSubRun = 1 ]]; then
											for subruninFile in `grep "$Search" $RunPathList`
											do
												subrunname=${subruninFile%%$Search}
												subrunDir=$runDir/$subrunname/
												mkdir -p $subrunDir
												filenameZip="root_archive.zip"
												downlogFileZip="$subrunDir/${filenameZip}.downlog"
												inFileZip="${subruninFile%%$Search}/$filenameZip"
												outFileZip="$subrunDir/$filenameZip"
												outFileOriginal="$subrunDir/$Search"
												if [[ ! -f $outFileOriginal ]]; then
													GetFileZip $inFileZip $outFileZip $downlogFileZip ${subrunDir} >> $downlogFileZip.tmp &
												fi
											done
											# printf "\t\e[33m|->\e[0m download zip files \n " #| tee -a $WARNINGLog | tee -a $LogFile
											wait
										fi

										tmpsubruncount2=0
										for subruninFile in `grep "$Search" $RunPathList`
										do
											if [[ $tmpsubruncount2 = $ParalellDownloads ]]; then
												tmptmpnrunsbegin=$(expr $tmpsubruncount - $ParalellDownloads)
												printf "\t\t\e[33m|->\e[0m waiting for subruns $tmptmpnrunsbegin - $tmpsubruncount of $maxcount runs\n" #| tee -a $WARNINGLog | tee -a $LogFile
												wait
												tmpsubruncount2=0
											fi
											subrunname=${subruninFile%%$Search}
											if [[ $NorunwiseinTrain = 1 ]]; then
												subrunname="${subrunname%/*}"
												subrunname="${subrunname##*/}"
											else
												if [[ $OptIsJJ = 1 ]]; then
													subrunname="${subrunname#*child_*/}"
													# subrunname="${subrunname#*$childID/}"
													# echo "$subrunname $runDir $subrunDir $subrundownlogFile"
												else
													subrunname="${subrunname#*child_*/}"
												fi
												subrunname="${subrunname%/*}"
												subrunname="${subrunname##*/}"
											fi
											if [[ $subrunname = "" ]]; then continue; fi
											subrunDir=$runDir/$subrunname/
											mkdir -p $subrunDir
											subrunoutFile=$subrunDir/$Search
											subrundownlogFile=$subrunDir/.${Search%%.root}.downlog
											new_rm $subrundownlogFile
											new_rm $subrundownlogFile.tmp
											((tmpsubruncount++))
											((tmpsubruncount2++))
											if [[ $OptZipSubRun = 1 ]] && [[ ! -f $subrunoutFile ]]; then
												filenameZip="root_archive.zip"
												outFileZip="$subrunDir/$filenameZip"
												inFileZip="${subruninFile%%$Search}/$filenameZip"
												if [[ $NorunwiseinTrain = 0 ]]; then
													printf "\t\tProcessing SubRun\t$tmpsubruncount/$maxcount\t$runName|$subrunname\t$filenameZip " | tee -a $LogFile >> $subrundownlogFile.tmp
												else
													printf "\t\tProcessing SubRun\t$tmpsubruncount/$maxcount\t$subrunname\t$filenameZip " | tee -a $LogFile >> $subrundownlogFile.tmp
												fi
												# printf "\e[33m|-> download $filenameZip:\e[0m  " | tee -a $LogFile
												GetFileZip $inFileZip $outFileZip $subrundownlogFile ${subrunDir} &>> $subrundownlogFile.tmp &
											else
												if [[ $NorunwiseinTrain = 0 ]]; then
													printf "\t\tProcessing SubRun\t$tmpsubruncount/$maxcount\t$runName|$subrunname\t$Search " >> $subrundownlogFile.tmp
												else
													printf "\t\tProcessing SubRun\t$tmpsubruncount/$maxcount\t$subrunname\t$Search " >> $subrundownlogFile.tmp
												fi
												GetFile $subruninFile $subrunoutFile $subrundownlogFile &>> $subrundownlogFile.tmp &
											fi
											# if [[ $? -eq 1 ]]; then
											# 	continue
											# fi
										done

										# printf "\t\e[33m|->\e[0m download subruns ($maxcount Subruns)\n" #| tee -a $WARNINGLog | tee -a $LogFile
										wait
										for subruninFile in `grep "$Search" $RunPathList`
										do
											subrunname=${subruninFile%%$Search}
											if [[ $NorunwiseinTrain = 1 ]]; then
												subrunname="${subrunname%/*}"
												subrunname="${subrunname##*/}"
												# printf "$subrunname, "
											else
												if [[ $OptIsJJ = 1 ]]; then
													# subrunname="${subrunname#*$childID/}"
													subrunname="${subrunname#*child_*/}"
												else
													subrunname="${subrunname#*child_*/}"
												fi
												subrunname="${subrunname%/*}"
												subrunname="${subrunname##*/}"
											fi
											if [[ $subrunname = "" ]]; then continue; fi
											subrunDir=$runDir/$subrunname/
											subrunoutFile=$subrunDir/$Search
											subrundownlogFile=$subrunDir/.${Search%%.root}.downlog
											cat $subrundownlogFile.tmp
											new_rm $subrundownlogFile.tmp
											runinFileTMP=${subruninFile#$subrunname*}
											if [[ $debug = 1 ]] || [[ $debug = 2 ]]
											then
												echo "doMergeFiles $Search | $subrunDir | ${subruninFile%%$Search} | $runDir$RLextension" #| tee -a $LogFile
											fi
											PrepMerge $Search $subrunDir ${subruninFile%%$Search} "$runDir$RLextension"
										done
										# printf "\t\t\t\e[33m|->\e[0m StartMergeProcess" #| tee -a $WARNINGLog | tee -a $LogFile
										printf "\t\t\t"
										StartMergeProcess "$runDir$RLextension" #&> /dev/null
										if [[ $NorunwiseinTrain = 0 ]]; then
											# echo -e "\t\t\e[33m|->\e[0m merge from higher stages.. done"  | tee -a $rundownlogFile.tmp #| tee -a $WARNINGLog | tee -a $LogFile
											if [[ $MergeRuns = 1 ]]
											then
												printf "\t\t\e[33m|->\e[0m merging run " #| tee -a $WARNINGLog | tee -a $LogFile
												runinFileTMP=${subruninFile#$subrunname*}
												if [[ $debug = 1 ]] || [[ $debug = 2 ]]
												then
													echo "doMergeFiles $Search, $runDir, ${runinFileTMP%%$Search}, $Dirout" | tee -a $LogFile
												fi
												PrepMerge $Search $runDir ${runinFileTMP/%%$Search} $Dirout
											else
												echo
											fi
										else
											cmd="ln -sf "$runDir$RLextension/*" $Dirout"
											usecmd $cmd
											eval $cmd
										fi
									else
										printf "\t\e[33mWARNING:  No file found \e[0m, merging from higher stages disabled \n" | tee -a $WARNINGLog | tee -a $LogFile #| tee -a $rundownlogFile.tmp
										new_rm $rundownlogFile.tmp
									fi
								fi
								if [[ $NorunwiseinTrain = 1 ]]; then
									NorunwiseinTrainfirstrundone=1
								fi
							done
							# printf "\t\t\e[33m|->\e[0m waiting: merge from higher stages..." #| tee -a $WARNINGLog | tee -a $LogFile
							# wait
							# printf "\t done \n\t\t" #| tee -a $WARNINGLog | tee -a $LogFile

							if [[ $tmpruncount > 0 ]]; then
								tmptmpnrunsbegin=$(expr $tmpruncount - $tmpruncount2)
								printf "\t\e[33m|->\e[0m waiting for runs $tmptmpnrunsbegin - $tmpruncount of $NumberOfRuns runs\n" #| tee -a $WARNINGLog | tee -a $LogFile
								wait
							fi
						# done
						# if [[ $NorunwiseinTrain = 0 ]]; then
						# 	printf "\t\e[33m|->\e[0m waiting for downloads ($NumberOfRuns runs)\n" #| tee -a $WARNINGLog | tee -a $LogFile
						# fi
						# wait
						#
						# for (( pthardbin = $minpthardbins; pthardbin <= $maxpthardbins; pthardbin++ )); do
							NorunwiseinTrainfirstrundone=0
							for runName in `cat $Runlist`; do
								if [[ $NorunwiseinTrain = 1 ]] && [[ $NorunwiseinTrainfirstrundone = 1 ]]; then
									continue
								fi
								searchChildtmp="_$childID/$Search"
								if [[ `echo $childID | wc -c` -gt 4 ]]; then
									searchChildtmp="$childID/$Search"
								fi

								if [[ $NorunwiseinTrain = 1 ]]; then
									runDir=$OUTPUTDIR/.$child
								else
									# look for availible Files
									if [[ $OptIsJJ = 1 ]]; then
										runDir=$OUTPUTDIR/.$child/$pthardbin/$runName
										Dirout=$OUTPUTDIR/.$child/$pthardbin
									else
										runDir=$OUTPUTDIR/.$child/$runName
									fi
								fi


								RunPathList="$runDir/PathList.txt"
								if [[ $OptIsJJ = 1 ]]; then
									rundownlogFile="$runDir/.${Search%%.root}.downlog"
								else
									rundownlogFile=$runDir/.${Search%%.root}.downlog
								fi
								if [[ -f $rundownlogFile.tmp ]]; then
									cat $rundownlogFile.tmp | tee -a $LogFile
								fi
								new_rm $rundownlogFile.tmp
								if [[ `grep "$searchChildtmp" $RunPathList | wc -c` -gt 0 ]] && [[ $MergeRuns = 1 ]]; then
									for runinFile in `grep "$searchChildtmp" $RunPathList`; do
											PrepMerge $Search $runDir ${runinFile/%%$Search} $Dirout
									done
								else
									echo;
								fi
								if [[ $NorunwiseinTrain = 1 ]]; then
									NorunwiseinTrainfirstrundone=1
								fi
							done
							if [[ $NorunwiseinTrain = 0 ]]; then
								printf "\t\t" #| tee -a $WARNINGLog | tee -a $LogFile
								StartMergeProcess $Dirout
							fi
							# if [[ $OptIsJJ = 1 ]] ; then
							# 	Dirout=$DiroutTmp
							# fi
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
					if [[ $MergeRuns = 1 ]] ; then
						# touch $completlymerged
						printf "\e[33m|-> Merging file:\e[0m $RunlistName $ChildName $filename " | tee -a $LogFile
						doMergeFiles $filename $Dirout $Dirin $DirMerged
					fi
					if [[ $UseMerge = 0 ]] && [[ $NChilds = 1 ]] ; then
						printf "\e[33m|-> Link file:\e[0m $RunlistName $ChildName $filename \n" | tee -a $LogFile
						cmd="ln -sf $BASEDIR/$OutName/$RunlistName/$ChildName/$filename $BASEDIR/$OutName/$RunlistName/$filename"
						eval $cmd
					fi
				done
			done
		done
	done
fi


if [[ $useSpecificRunlist = 1 ]]; then
	for RunlistName in `cat $OptRunlistNamefile`
	do
		SpecificRunlist=${RunlistName#*useSpecificRunlist_}
		echo $SpecificRunlist  | tee -a $OptRunlistNamefile.tmp | tee -a $LogFile
	done
	new_rm $OptRunlistNamefile
	mv $OptRunlistNamefile.tmp $OptRunlistNamefile
fi

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
