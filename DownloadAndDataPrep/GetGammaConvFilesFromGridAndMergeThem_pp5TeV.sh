#! /bin/bash

# variable: $1 = username, $2 = AOD/ESD, $3 = filename

# usernames: fbock, leardini, passfeld, amarin, mwilde, pgonzales
# specific location is added the same username will have a different output directory
# i.e fbockGSI or leardiniALICESERV1, passfeldMAF

if [ $1 = "fbock" ]; then 
   BASEDIR=/home/fbock/Photon/Grid/OutputLegoTrains/pp
elif [ $1 = "fbockGSI" ]; then 
   BASEDIR=/hera/alice/fbock/Grid/OutputLegoTrains/pp
elif [ $1 = "leardini" ]; then 
   BASEDIR=/Users/lucy/
elif [ $1 = "leardiniALICESERV1" ]; then 
   BASEDIR=/alidata50/alice_u/leardini/GridOutput/PbPb/
elif [ $1 = "leardiniGSI" ]; then 
   BASEDIR=/hera/alice/leardini/Grid/OutputLegoTrains/pp
elif [ $1 = "passfeld" ]; then 
   BASEDIR=~/work/Gridoutput/pp
elif [ $1 = "passfeldMAF" ]; then 
   BASEDIR=/data9/a_pass02/gamma_test/AnalysisSoftware/LegoTrain/
elif [ $1 = "passfeldGSI" ]; then  
   BASEDIR=/hera/alice/passfeld/Grid/OutputLegoTrains/pp
elif [ $1 = "amarin" ]; then     
   BASEDIR=/Users/marin/
elif [ $1 = "amarinGSI" ]; then     
   BASEDIR=/hera/alice/marin/Grid/OutputLegoTrains/pp 
elif [ $1 = "amarinALICESERV1" ]; then     
   BASEDIR=/alidata50/alice_u/amarin/GridOutput/PbPb/   
elif [ $1 = "mwilde" ]; then        
   BASEDIR=~/work/GridOutput 
elif [ $1 = "mwildeGSI" ]; then           
   BASEDIR=/hera/alice/mwilde/Grid/OutputLegoTrains/pp 
elif [ $1 = "pgonzalez" ]; then     
   BASEDIR=/home/pgonzale/ALICE/GridOutput
elif [ $1 = "pgonzalezGSI" ]; then        
   BASEDIR=/hera/alice/pgonzales/Grid/OutputLegoTrains/pp
elif [ $1 = "dmuhlheim" ]; then 
   BASEDIR=/home/daniel/Desktop/Grid
fi

echo "Test test entro";

echo $BASEDIR

mkdir -p $BASEDIR
fileName=$3 
 
TRAINDIR=Legotrain-vAN-20170515-1-ConvCalo
LHC15np2Data="";
LHC15np3Data="";

# LHC16h8a="2905_20170426-0936";
# LHC16h8b="2901_20170426-0928";
# LHC16k5a="2904_20170426-0935";
# LHC16k5b="2903_20170426-0928";

LHC16h8a="2916_20170515-2055";
LHC16h8b="2920_20170515-2129";
LHC16k5a="2924_20170515-2133";
LHC16k5b="2926_20170515-2146";




OUTPUTDIR=$BASEDIR/$TRAINDIR
if [ $2 = "AOD" ]; then
   TRAINPATHData=GA_pp_AOD
else    
   TRAINPATHData=GA_pp
fi

if [ $2 = "AOD" ]; then
   TRAINPATHMC=GA_pp_MC_AOD
else    
   TRAINPATHMC=GA_pp_MC
fi


if [ $LHC15np2Data != "" ]; then
   OUTPUTDIR_LHC15np2=$BASEDIR/$TRAINDIR/$TRAINPATHData-$LHC15np2Data   
   echo $TRAINPATHData-$LHC15np2Data;
   mkdir -p $OUTPUTDIR_LHC15np2
   echo $OUTPUTDIR_LHC15np2
   rm runNumbersLHC15np2FailedRunMerge.txt
   echo "copying LHC15n pass2 MC" 
   runNumbers=`cat runlists/runNumbersLHC15n.txt`
   for runNumber in $runNumbers; do
      echo $runNumber
      mkdir -p $OUTPUTDIR_LHC15np2/$runNumber
      if [ -f $OUTPUTDIR_LHC15np2/$runNumber/$fileName ]; then
         echo "file " $fileName  " has already been copied for run " $runNumber
      else 
         if [ $2 = "AOD" ]; then 
            echo "copying /alice/data/2015/LHC15n/000$runNumber/pass2/PWGGA/$TRAINPATHData/$LHC15np2Data/$fileName"
            alien_cp alien:/alice/data/2015/LHC15n/000$runNumber/pass2/PWGGA/$TRAINPATHData/$LHC15np2Data/$fileName file:$OUTPUTDIR_LHC15np2/$runNumber
         else
            echo "copying /alice/data/2015/LHC15n/000$runNumber/pass2/PWGGA/$TRAINPATHData/$LHC15np2Data/$fileName"
            alien_cp alien:/alice/data/2015/LHC15n/000$runNumber/pass2/PWGGA/$TRAINPATHData/$LHC15np2Data/$fileName file:$OUTPUTDIR_LHC15np2/$runNumber
         fi
      fi
      if [ $? = 0 ] && [ -f $OUTPUTDIR_LHC15np2/$runNumber/$fileName ]; then
         echo "file " $fileName  " been copied for sucessfully for run " $runNumber
      else 
         echo $runNumber >> runNumbersLHC15np2FailedRunMerge.txt
      fi     
   done;

   runNumbersBroken=`cat runNumbersLHC15np2FailedRunMerge.txt`
   for runNumber in $runNumbersBroken; do
      echo "copying stage_1 output for " $runNumber
      mkdir -p $OUTPUTDIR_LHC15np2/$runNumber
      if [ $2 = "AOD" ]; then
         stageOutputs=`alien_ls /alice/data/2011/LHC11a/000$runNumber/ESDs/pass4_with_SDD/AOD139/PWGGA/$TRAINPATHData/Stage_1/`
      else 
         stageOutputs=`alien_ls /alice/data/2015/LHC15n/000$runNumber/pass2/PWGGA/$TRAINPATHData/$LHC15np2Data/Stage_1/`
      fi
      for stageOutput in $stageOutputs; do
		 testOut=`alien_ls /alice/data/2015/LHC15n/000$runNumber/pass2/PWGGA/GA_pp/$TRAINPATHData/$LHC15np2Data/Stage_1/$stageOutput/$fileName`
		 if [ $testOut = $fileName ]; then 
			mkdir -p $OUTPUTDIR_LHC15np2/$runNumber/Stage_1/$stageOutput
			if [ -f $OUTPUTDIR_LHC15np2/$runNumber/Stage_1/$stageOutput/$fileName ]; then
				echo "file " $fileName  " has already been copied for run " $runNumber"/"$stageOutput
			else 
				if [ $2 = "AOD" ]; then
				echo "copying /alice/data/2015/LHC15n/000$runNumber/ESDs/pass4_with_SDD/AOD139/PWGGA/$TRAINPATHData/$LHC11aData/Stage_2/$stageOutput/$fileName"
				alien_cp alien:/alice/data/2011/LHC11a/000$runNumber/ESDs/pass4_with_SDD/AOD139/PWGGA/$TRAINPATHData/$LHC11aData/Stage_2/$stageOutput/$fileName file:$OUTPUTDIR_LHC11a/$runNumber/Stage_2/$stageOutput/
				else
				echo "copying /alice/data/2015/LHC15n/000$runNumber/pass2/PWGGA/GA_pp/$TRAINPATHData/Stage_1/$stageOutput/$fileName"
				alien_cp alien:/alice/data/2015/LHC15n/000$runNumber/pass2/PWGGA/GA_pp/$TRAINPATHData/$LHC15np2Data/$stageOutput/$fileName file:$OUTPUTDIR_LHC15np2/$runNumber/Stage_1/$stageOutput/
				fi
			fi
         fi
      done;
   done;      
fi



if [ $LHC15np3Data != "" ]; then
   OUTPUTDIR_LHC15np3=$BASEDIR/$TRAINDIR/$TRAINPATHData-$LHC15np3Data   
   echo $TRAINPATHData-$LHC15np3Data;
   mkdir -p $OUTPUTDIR_LHC15np3
   echo $OUTPUTDIR_LHC15np3
   rm runNumbersLHC15np3FailedRunMerge.txt
   echo "copying LHC15n pass3 MC" 
   runNumbers=`cat runlists/runNumbersLHC15n.txt`
   for runNumber in $runNumbers; do
      echo $runNumber
      mkdir -p $OUTPUTDIR_LHC15np3/$runNumber
      if [ -f $OUTPUTDIR_LHC15np3/$runNumber/$fileName ]; then
         echo "file " $fileName  " has already been copied for run " $runNumber
      else 
         if [ $2 = "AOD" ]; then 
            echo "copying /alice/data/2015/LHC15n/000$runNumber/pass3/PWGGA/$TRAINPATHData/$LHC15np3Data/$fileName"
            alien_cp alien:/alice/data/2015/LHC15n/000$runNumber/pass3/PWGGA/$TRAINPATHData/$LHC15np3Data/$fileName file:$OUTPUTDIR_LHC15np3/$runNumber
         else
            echo "copying /alice/data/2015/LHC15n/000$runNumber/pass3/PWGGA/$TRAINPATHData/$LHC15np3Data/$fileName"
            alien_cp alien:/alice/data/2015/LHC15n/000$runNumber/pass3/PWGGA/$TRAINPATHData/$LHC15np3Data/$fileName file:$OUTPUTDIR_LHC15np3/$runNumber
         fi
      fi
      if [ $? = 0 ] && [ -f $OUTPUTDIR_LHC15np3/$runNumber/$fileName ]; then
         echo "file " $fileName  " been copied for sucessfully for run " $runNumber
      else 
         echo $runNumber >> runNumbersLHC15np3FailedRunMerge.txt
      fi     
   done;

   runNumbersBroken=`cat runNumbersLHC15np3FailedRunMerge.txt`
   for runNumber in $runNumbersBroken; do
      echo "copying stage_1 output for " $runNumber
      mkdir -p $OUTPUTDIR_LHC15np2/$runNumber
      if [ $2 = "AOD" ]; then
         stageOutputs=`alien_ls /alice/data/2015/LHC15n/000$runNumber/ESDs/pass4_with_SDD/AOD139/PWGGA/$TRAINPATHData/Stage_1/`
      else 
         stageOutputs=`alien_ls /alice/data/2015/LHC15n/000$runNumber/pass3/PWGGA/$TRAINPATHData/$LHC15np2Data/Stage_1/`
      fi
      for stageOutput in $stageOutputs; do
		 testOut=`alien_ls /alice/data/2015/LHC15n/000$runNumber/pass3/PWGGA/GA_pp/$TRAINPATHData/$LHC15np3Data/Stage_1/$stageOutput/$fileName`
		 if [ $testOut = $fileName ]; then 
			mkdir -p $OUTPUTDIR_LHC15np3/$runNumber/Stage_1/$stageOutput
			if [ -f $OUTPUTDIR_LHC15np3/$runNumber/Stage_1/$stageOutput/$fileName ]; then
				echo "file " $fileName  " has already been copied for run " $runNumber"/"$stageOutput
			else 
				if [ $2 = "AOD" ]; then
				echo "copying /alice/data/2015/LHC15n/000$runNumber/pass3/PWGGA/$TRAINPATHData/$LHC15p3Data/Stage_1/$stageOutput/$fileName"
				alien_cp alien:/alice/data/2015/LHC15n/000$runNumber/pass3/PWGGA/$TRAINPATHData/$LHC15p3Data/Stage_1/$stageOutput/$fileName file:$OUTPUTDIR_LHC15np3/$runNumber/Stage_1/$stageOutput/
				else
				echo "copying /alice/data/2015/LHC15n/000$runNumber/pass3/PWGGA/$TRAINPATHData/Stage_1/$stageOutput/$fileName"
				alien_cp alien:/alice/data/2015/LHC15n/000$runNumber/pass2/PWGGA/$TRAINPATHData/$LHC15np3Data/$stageOutput/$fileName file:$OUTPUTDIR_LHC15np3/$runNumber/Stage_1/$stageOutput/
				fi
			fi
         fi
      done;
   done;      
fi



if [ $LHC16k5a != "" ]; then
   OUTPUTDIR_LHC16k5a=$BASEDIR/$TRAINDIR/$TRAINPATHMC-$LHC16k5a
   echo $TRAINPATHMC-$LHC16k5a;
   mkdir -p $OUTPUTDIR_LHC16k5a
   rm runNumbersLHC16k5aFailedRunMerge.txt
   echo "copying LHC16k5a MC" 
   runNumbers=`cat runlists/runNumbersLHC15n.txt`
   for runNumber in $runNumbers; do
      echo $runNumber
      runNumberlocal=$runNumber
      if [ $2 = "AOD" ]; then
        runNumber=$runNumber/AOD158
        
      fi
      mkdir -p $OUTPUTDIR_LHC16k5a/$runNumberlocal
      if [ -f $OUTPUTDIR_LHC16k5a/$runNumberlocal/$fileName ]; then
         echo "file " $fileName  " has already been copied for run " $runNumberlocal
      else 
         echo "copying /alice/sim/2016/LHC16k5a/$runNumber/PWGGA/$TRAINPATHMC/$LHC16k5a/$fileName"
         alien_cp alien:/alice/sim/2016/LHC16k5a/$runNumber/PWGGA/$TRAINPATHMC/$LHC16k5a/$fileName file:$OUTPUTDIR_LHC16k5a/$runNumberlocal
      fi
      if [ $? -ne 0 ] && [ -f $OUTPUTDIR_LHC16k5a/$runNumberlocal/$fileName ]; then
         echo "file " $fileName  " been copied for sucessfully for run " $runNumberlocal
      else 
         echo $runNumberlocal >> runNumbersLHC16k5aFailedRunMerge.txt
      fi     
   done;
fi




if [ $LHC16k5a != "" ]; then
   OUTPUTDIR_LHC16k5a=$BASEDIR/$TRAINDIR/$TRAINPATHMC-$LHC16k5a
   echo $TRAINPATHMC-$LHC16k5a;
   mkdir -p $OUTPUTDIR_LHC16k5a
   rm runNumbersLHC16k5aFailedRunMerge.txt
   echo "copying LHC16k5a MC" 
   runNumbers=`cat runlists/runNumbersLHC15n.txt`
   for runNumber in $runNumbers; do
      echo $runNumber
      runNumberlocal=$runNumber
      if [ $2 = "AOD" ]; then
        runNumber=$runNumber/AOD158
        
      fi
      mkdir -p $OUTPUTDIR_LHC16k5a/$runNumberlocal
      if [ -f $OUTPUTDIR_LHC16k5a/$runNumberlocal/$fileName ]; then
         echo "file " $fileName  " has already been copied for run " $runNumberlocal
      else 5
         echo "copying /alice/sim/2016/LHC16k5a/$runNumber/PWGGA/$TRAINPATHMC/$LHC16k5a/$fileName"
         alien_cp alien:/alice/sim/2016/LHC16k5a/$runNumber/PWGGA/$TRAINPATHMC/$LHC16k5a/$fileName file:$OUTPUTDIR_LHC16k5a/$runNumberlocal
      fi
      if [ $? -ne 0 ] && [ -f $OUTPUTDIR_LHC16k5a/$runNumberlocal/$fileName ]; then
         echo "file " $fileName  " been copied for sucessfully for run " $runNumberlocal
      else 
         echo $runNumberlocal >> runNumbersLHC16k5aFailedRunMerge.txt
      fi     
   done;
fi



if [ $LHC16k5b != "" ]; then
   OUTPUTDIR_LHC16k5b=$BASEDIR/$TRAINDIR/$TRAINPATHMC-$LHC16k5b
   echo $TRAINPATHMC-$LHC16k5b;
   mkdir -p $OUTPUTDIR_LHC16k5b
   rm runNumbersLHC16k5bFailedRunMerge.txt
   echo "copying LHC16k5b MC" 
   runNumbers=`cat runlists/runNumbersLHC15n.txt`
   for runNumber in $runNumbers; do
      echo $runNumber
      runNumberlocal=$runNumber
      if [ $2 = "AOD" ]; then
        runNumber=$runNumber/AOD158
        
      fi
      mkdir -p $OUTPUTDIR_LHC16k5b/$runNumberlocal
      if [ -f $OUTPUTDIR_LHC16k5b/$runNumberlocal/$fileName ]; then
         echo "file " $fileName  " has already been copied for run " $runNumberlocal
      else 5
         echo "copying /alice/sim/2016/LHC16k5b/$runNumber/PWGGA/$TRAINPATHMC/$LHC16k5b/$fileName"
         alien_cp alien:/alice/sim/2016/LHC16k5b/$runNumber/PWGGA/$TRAINPATHMC/$LHC16k5b/$fileName file:$OUTPUTDIR_LHC16k5b/$runNumberlocal
      fi
      if [ $? -ne 0 ] && [ -f $OUTPUTDIR_LHC16k5b/$runNumberlocal/$fileName ]; then
         echo "file " $fileName  " been copied for sucessfully for run " $runNumberlocal
      else 
         echo $runNumberlocal >> runNumbersLHC16k5bFailedRunMerge.txt
      fi     
   done;
fi



if [ $LHC16h8a != "" ]; then
   OUTPUTDIR_LHC16h8a=$BASEDIR/$TRAINDIR/$TRAINPATHMC-$LHC16h8a
   echo $TRAINPATHMC-$LHC16h8a;
   mkdir -p $OUTPUTDIR_LHC16h8a
   rm runNumbersLHC16h8aFailedRunMerge.txt
   echo "copying LHC16h8a MC" 
   runNumbers=`cat runlists/runNumbersLHC15n.txt`
   for runNumber in $runNumbers; do
      echo $runNumber
      runNumberlocal=$runNumber
      if [ $2 = "AOD" ]; then
        runNumber=$runNumber/AOD158
        
      fi
      mkdir -p $OUTPUTDIR_LHC16h8a/$runNumberlocal
      if [ -f $OUTPUTDIR_LHC16h8a/$runNumberlocal/$fileName ]; then
         echo "file " $fileName  " has already been copied for run " $runNumberlocal
      else 5
         echo "copying /alice/sim/2016/LHC16h8a/$runNumber/PWGGA/$TRAINPATHMC/$LHC16h8a/$fileName"
         alien_cp alien:/alice/sim/2016/LHC16h8a/$runNumber/PWGGA/$TRAINPATHMC/$LHC16h8a/$fileName file:$OUTPUTDIR_LHC16h8a/$runNumberlocal
      fi
      if [ $? -ne 0 ] && [ -f $OUTPUTDIR_LHC16h8a/$runNumberlocal/$fileName ]; then
         echo "file " $fileName  " been copied for sucessfully for run " $runNumberlocal
      else 
         echo $runNumberlocal >> runNumbersLHC16h8aFailedRunMerge.txt
      fi     
   done;
fi



if [ $LHC16h8a != "" ]; then
   OUTPUTDIR_LHC16h8a=$BASEDIR/$TRAINDIR/$TRAINPATHMC-$LHC16h8a
   echo $TRAINPATHMC-$LHC16h8a;
   mkdir -p $OUTPUTDIR_LHC16h8a
   rm runNumbersLHC16h8aFailedRunMerge.txt
   echo "copying LHC16h8a MC" 
   runNumbers=`cat runlists/runNumbersLHC15n.txt`
   for runNumber in $runNumbers; do
      echo $runNumber
      runNumberlocal=$runNumber
      if [ $2 = "AOD" ]; then
        runNumber=$runNumber/AOD158
        
      fi
      mkdir -p $OUTPUTDIR_LHC16h8a/$runNumberlocal
      if [ -f $OUTPUTDIR_LHC16h8a/$runNumberlocal/$fileName ]; then
         echo "file " $fileName  " has already been copied for run " $runNumberlocal
      else 5
         echo "copying /alice/sim/2016/LHC16h8a/$runNumber/PWGGA/$TRAINPATHMC/$LHC16h8a/$fileName"
         alien_cp alien:/alice/sim/2016/LHC16h8a/$runNumber/PWGGA/$TRAINPATHMC/$LHC16h8a/$fileName file:$OUTPUTDIR_LHC16h8a/$runNumberlocal
      fi
      if [ $? -ne 0 ] && [ -f $OUTPUTDIR_LHC16h8a/$runNumberlocal/$fileName ]; then
         echo "file " $fileName  " been copied for sucessfully for run " $runNumberlocal
      else 
         echo $runNumberlocal >> runNumbersLHC16h8aFailedRunMerge.txt
      fi     
   done;
fi


if [ $LHC16h8b != "" ]; then
   OUTPUTDIR_LHC16h8b=$BASEDIR/$TRAINDIR/$TRAINPATHMC-$LHC16h8b
   echo $TRAINPATHMC-$LHC16h8b;
   mkdir -p $OUTPUTDIR_LHC16h8b
   rm runNumbersLHC16h8bFailedRunMerge.txt
   echo "copying_LHC16h8b MC" 
   runNumbers=`cat runlists/runNumbersLHC15n.txt`
   for runNumber in $runNumbers; do
      echo $runNumber
      runNumberlocal=$runNumber
      if [ $2 = "AOD" ]; then
        runNumber=$runNumber/AOD158
      fi
      mkdir -p $OUTPUTDIR_LHC16h8b/$runNumberlocal
      if [ -f $OUTPUTDIR_LHC16h8b/$runNumberlocal/$fileName ]; then
         echo "file " $fileName  " has already been copied for run " $runNumberlocal
      else 
         echo "copying /alice/sim/2016/LHC16h8b/$runNumber/PWGGA/$TRAINPATHMC/$LHC16h8b/$fileName"
         alien_cp alien:/alice/sim/2016/LHC16h8b/$runNumber/PWGGA/$TRAINPATHMC/$LHC16h8b/$fileName file:$OUTPUTDIR_LHC16h8b/$runNumberlocal
      fi
      if [ $? -ne 0 ] && [ -f $OUTPUTDIR_LHC16h8b/$runNumberlocal/$fileName ]; then
         echo "file " $fileName  " been copied for sucessfully for run " $runNumberlocal
      else 
         echo $runNumberlocal >> runNumbersLHC16h8bFailedRunMerge.txt
      fi     
   done;
fi







