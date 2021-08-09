#! /bin/bash



echo "Starting DownloadAndMerge for pp 13TeV..."
echo;

source DownScript_header.sh

thisuser=`echo ${USER}`
d=`date +%Y_%m_%d`
if [[ $thisuser = "adrian" || $thisuser = "amechler" ]]
then

 if [[ -f Error.log ]]; then
 rm Error.log
 fi
 gridpath="/media/adrian/Data/grid_data"


# # 1897 	vAN-20210728_ROOT6-1 	LHC18_13TeV_pass1_AOD234 	2021 Jul 30, Waiting for files to be copied 	request Edgar 	
# # 1896 	vAN-20210728_ROOT6-1 	LHC17_13TeV_pass1_AOD234 	2021 Jul 30, Train run: Submitting enabled 	request Edgar 	
# # 1895 	vAN-20210728_ROOT6-1 	LHC16_13TeV_pass1_2_AOD234 	2021 Jul 30, Train run: Submitting enabled 	request Edgar 	
bash DownScript.sh 1897 1896 1895 GA_pp_AOD ?_GammaConvCalo -RL_listPCMEDC  $1
bash DownScript.sh 1897 1896 1895 -Name_Data_CellCalib GA_pp_AOD ?_Gamma -mergetrains -RL_listPCMEDC -noDown $1


# # 3949 	vAN-20210728_ROOT6-1 	PYT8_13TeV_anchLHC18_AOD_extra 	2021 Jul 30, Train run: Submitting enabled 	request Edgar 	
# # 3948 	vAN-20210728_ROOT6-1 	PYT8_13TeV_anchLHC18_AOD235 	2021 Jul 30, Train run: Submitting enabled 	request Edgar 	
# # 3947 	vAN-20210728_ROOT6-1 	PYT8_13TeV_anchLHC17_AOD235 	2021 Jul 30, Train run: All jobs submitted 	request Edgar 	
# # 3946 	vAN-20210728_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD235_extra 	2021 Jul 30, Train run: All jobs submitted 	request Edgar 	
# # 3945 	vAN-20210728_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD235 	2021 Jul 30, Train run: All jobs submitted 	request Edgar 	
bash DownScript.sh 3949 3948 3947 3946 3945 GA_pp_MC_AOD ?_GammaConvCalo -RL_listPCMEDC  $1
bash DownScript.sh 3949 3948 3947 3946 3945 -Name_MC_CellCalib GA_pp_MC_AOD ?_Gamma -mergetrains -RL_listPCMEDC  -noDown $1

# ############################################

# # 1894 	vAN-20210728_ROOT6-1 	LHC18c_13TeV_LowB_pass1_AOD236 	2021 Jul 30, Train run: Submitting enabled 	request Joshua, Edgar 	
# # 1893 	vAN-20210728_ROOT6-1 	LHC17g_13TeV_LowB_pass1_AOD236 	2021 Jul 30, Train run: Submitting enabled 	request Joshua, Edgar 	
# # 1892 	vAN-20210728_ROOT6-1 	LHC16f_13TeV_LowB_pass1_AOD236 	2021 Jul 30, Train run: Submitting enabled 	request Joshua, Edgar 
bash DownScript.sh 1894 GA_pp_AOD  ?_GammaConvCalo Child_1 Child_2  -RL_listDPGTrackIncAccTPCandEMC $1
bash DownScript.sh 1893 GA_pp_AOD  ?_GammaConvCalo  -RL_listDPGTrackIncAccTPCandEMC $1
bash DownScript.sh 1892 GA_pp_AOD  ?_GammaConvCalo -RL_listDPGTrackIncAccTPCandEMC $1
bash DownScript.sh 1894 1893 1892 -Name_Data_CellCalibLowB GA_pp_AOD ?_zip ?_GammaConvCalo ?_GammaCalo -mergetrains -RL_listDPGTrackIncAccTPCandEMC -noDown $1


# # 3944 	vAN-20210728_ROOT6-1 	LHC18h1_PYT8_13TeV_ancLHC18c_LowB_AOD237 	2021 Jul 30, Train run: Submitting enabled 	request Joshua, Edgar 	
# # 3943 	vAN-20210727_ROOT6-1 	LHC17h3_PYT8_13TeV_anchLHC17g_LowB_AOD237 	2021 Jul 30, Train run: All jobs submitted 	request Joshua, Edgar 	
# # 3942 	vAN-20210728_ROOT6-1 	LHC17d1_PYT8_13TeV_ancLHC16f_LowB_AOD237 	2021 Jul 30, Train run: All jobs submitted 	request Joshua, Edgar 
bash DownScript.sh 3944 GA_pp_MC_AOD ?_GammaConvCalo Child_1 Child_2 Child_4 Child_5 -RL_listDPGTrackIncAccTPCandEMC $1
bash DownScript.sh 3943 GA_pp_MC_AOD ?_GammaConvCalo -RL_listDPGTrackIncAccTPCandEMC $1
bash DownScript.sh 3942 GA_pp_MC_AOD ?_GammaConvCalo -RL_listDPGTrackIncAccTPCandEMC $1
bash DownScript.sh 3944 3943 3942 -Name_MC_CellCalibLowB GA_pp_MC_AOD ?_Gamma -mergetrains -RL_listDPGTrackIncAccTPCandEMC -noDown $1


#################################################

### LowB QA
# bash DownScript.sh 1894 1893 1892 GA_pp_AOD ?_GammaCalo_2062 -RL_listDPGTrackIncAccTPCandEMC -runwise $1
bash DownScript.sh 1894 1893 1892 -Name_Data_QALowB GA_pp_AOD ?_GammaCalo_2062 -RL_listDPGTrackIncAccTPCandEMC -runwise -mergeall -noDown $1
bash DownScript.sh 1892 -Name_Data_QALowB_16 GA_pp_AOD ?_GammaCalo_2062 -RL_listDPGTrackIncAccTPCandEMC -runwise -mergeall -noDown $1
bash DownScript.sh 1893 -Name_Data_QALowB_17 GA_pp_AOD ?_GammaCalo_2062 -RL_listDPGTrackIncAccTPCandEMC -runwise -mergeall -noDown $1
bash DownScript.sh 1894 -Name_Data_QALowB_18 GA_pp_AOD ?_GammaCalo_2062 -RL_listDPGTrackIncAccTPCandEMC -runwise -mergeall -noDown $1

# bash DownScript.sh 3944 3943 3942 GA_pp_MC_AOD ?_GammaCalo_2062  -RL_listDPGTrackIncAccTPCandEMC -runwise $1
bash DownScript.sh 3944 3943 3942 -Name_MC_QALowB GA_pp_MC_AOD ?_GammaCalo_2062  -RL_listDPGTrackIncAccTPCandEMC -runwise -mergeall -noDown $1
bash DownScript.sh 3942 -Name_MC_QALowB_16 GA_pp_MC_AOD ?_GammaCalo_2062  -RL_listDPGTrackIncAccTPCandEMC -runwise -mergeall -noDown $1
bash DownScript.sh 3943 -Name_MC_QALowB_17 GA_pp_MC_AOD ?_GammaCalo_2062  -RL_listDPGTrackIncAccTPCandEMC -runwise -mergeall -noDown $1
bash DownScript.sh 3944 -Name_MC_QALowB_18 GA_pp_MC_AOD ?_GammaCalo_2062  -RL_listDPGTrackIncAccTPCandEMC -runwise -mergeall -noDown $1

###      QA
# bash DownScript.sh 1897 1896 1895 GA_pp_AOD  ?_GammaCalo_2062 ?_GammaCalo_2063 ?_GammaCalo_2064 -RL_listDPGEDC_OR_listDPGEMC -RL_listDPG -runwise $1
bash DownScript.sh 1897 1896 1895 -Name_Data_QA GA_pp_AOD  ?_GammaCalo_2062 ?_GammaCalo_2063 ?_GammaCalo_2064 -RL_listDPGEDC_OR_listDPGEMC -RL_listDPG -runwise -mergeall -noDown $1
bash DownScript.sh 1895 -Name_Data_QA_16 GA_pp_AOD  ?_GammaCalo_2062 ?_GammaCalo_2063 ?_GammaCalo_2064 -RL_listDPGEDC_OR_listDPGEMC -RL_listDPG -runwise -mergeall -noDown $1
bash DownScript.sh 1896 -Name_Data_QA_17 GA_pp_AOD  ?_GammaCalo_2062 ?_GammaCalo_2063 ?_GammaCalo_2064 -RL_listDPGEDC_OR_listDPGEMC -RL_listDPG -runwise -mergeall -noDown $1
bash DownScript.sh 1897 -Name_Data_QA_18 GA_pp_AOD  ?_GammaCalo_2062 ?_GammaCalo_2063 ?_GammaCalo_2064 -RL_listDPGEDC_OR_listDPGEMC -RL_listDPG -runwise -mergeall -noDown $1

# bash DownScript.sh 3949 3948 3947 3946 3945 GA_pp_MC_AOD  ?_GammaCalo_2062 ?_GammaCalo_2063 ?_GammaCalo_2064  -RL_listDPGEDC_OR_listDPGEMC -RL_listDPG -runwise $1
bash DownScript.sh 3949 3948 3947 3946 3945 -Name_MC_QA GA_pp_MC_AOD  ?_GammaCalo_2062 ?_GammaCalo_2063 ?_GammaCalo_2064  -RL_listDPGEDC_OR_listDPGEMC -RL_listDPG -runwise -mergeall -noDown $1
bash DownScript.sh 3946 3945 -Name_MC_QA_16 GA_pp_MC_AOD  ?_GammaCalo_2062 ?_GammaCalo_2063 ?_GammaCalo_2064  -RL_listDPGEDC_OR_listDPGEMC -RL_listDPG -runwise -mergeall -noDown $1
bash DownScript.sh 3947 -Name_MC_QA_17 GA_pp_MC_AOD  ?_GammaCalo_2062 ?_GammaCalo_2063 ?_GammaCalo_2064  -RL_listDPGEDC_OR_listDPGEMC -RL_listDPG -runwise -mergeall -noDown $1
bash DownScript.sh 3949 3948 -Name_MC_QA_18 GA_pp_MC_AOD  ?_GammaCalo_2062 ?_GammaCalo_2063 ?_GammaCalo_2064  -RL_listDPGEDC_OR_listDPGEMC -RL_listDPG -runwise -mergeall -noDown $1




# bash DownScript.sh 1897 1896 1895 -Name_Data_QA GA_pp_AOD  ?_GammaCalo_2062 ?_GammaCalo_2063 ?_GammaCalo_2064 ?_GammaCalo_2046 ?_GammaCalo_2047 ?_GammaCalo_2048 -RL_listDPGEDC_OR_listDPGEMC -RL_listDPG -runwise -mergeall $1
# bash DownScript.sh 3949 3948 3947 3946 3945 -Name_MC_QA GA_pp_MC_AOD  ?_GammaCalo_2062 ?_GammaCalo_2063 ?_GammaCalo_2064 ?_GammaCalo_2046 ?_GammaCalo_2047 ?_GammaCalo_2048 -RL_listDPGEDC_OR_listDPGEMC -RL_listDPG -runwise -mergeall $1


#################################################
#################################################

#  ####### AOD Validation

# 1692 	vAN-20210118_ROOT6-1 	LHC18c_13TeV_LowB_pass1_AOD236 	2021 Jan 20, Train run finished 	13TeV AOD validation 	
# 1691 	vAN-20210118_ROOT6-1 	LHC17g_13TeV_LowB_pass1_AOD236 	2021 Jan 20, Train run finished 	13TeV AOD validation 	
# 1690 	vAN-20210118_ROOT6-1 	LHC16f_13TeV_LowB_pass1_AOD236 	2021 Jan 20, Train run finished 	13TeV AOD validation 	

# 3426 	vAN-20210118_ROOT6-1 	LHC18h1_PYT8_13TeV_ancLHC18c_LowB_AOD237 	2021 Jan 21, Train run finished 	13TeV AOD validation 	
# 3425 	vAN-20210118_ROOT6-1 	LHC17h3_PYT8_13TeV_anchLHC17g_LowB_AOD237 	2021 Jan 21, Train run finished 	13TeV AOD validation 	
# 3424 	vAN-20210118_ROOT6-1 	LHC17d1_PYT8_13TeV_ancLHC16f_LowB_AOD237 	2021 Jan 21, Train run finished 	13TeV AOD validation



# 1686 	vAN-20210118_ROOT6-1 	LHC18_13TeV_pass1_AOD234 	2021 Jan 20, Train run: All jobs submitted 	13TeV AOD validation 	
# 1685 	vAN-20210118_ROOT6-1 	LHC17_13TeV_pass1_AOD234 	2021 Jan 20, Train run: All jobs submitted 	13TeV AOD validation 	
# 1684 	vAN-20210118_ROOT6-1 	LHC16_13TeV_pass1_2_AOD234 	2021 Jan 20, Train run finished 	13TeV AOD validation 	
#  bash DownScript.sh 1686 1685 1684  GA_pp_AOD  ?_GammaConvCalo_2001  -RL_listDPG -runwise $1
#  bash DownScript.sh 1684 -Name_Data_AOD234_16 GA_pp_AOD  ?_GammaConvCalo_2001 -mergeall -RL_listDPG -noDown $1
#  bash DownScript.sh 1685 -Name_Data_AOD234_17 GA_pp_AOD  ?_GammaConvCalo_2001 -mergeall -RL_listDPG -noDown $1
#  bash DownScript.sh 1686 -Name_Data_AOD234_18 GA_pp_AOD  ?_GammaConvCalo_2001 -mergeall -RL_listDPG -noDown $1
#  bash DownScript.sh 1686 1685 1684 -Name_Data_AOD234 GA_pp_AOD  ?_GammaConvCalo_2001 -mergeall -RL_listDPG -noDown $1

# 3416 	vAN-20210118_ROOT6-1 	PYT8_13TeV_anchLHC18_AOD_extra 	2021 Jan 21, Train run finished 	13TeV AOD validation 	
# 3420 	vAN-20210118_ROOT6-1 	PYT8_13TeV_anchLHC18_AOD235 	2021 Jan 21, Train run finished 	13TeV AOD validation 	
# 3419 	vAN-20210118_ROOT6-1 	PYT8_13TeV_anchLHC17_AOD235 	2021 Jan 21, Train run finished 	13TeV AOD validation 	
# 3418 	vAN-20210118_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD235_extra 	2021 Jan 21, Train run finished 	13TeV AOD validation 	
# 3417 	vAN-20210118_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD235 	2021 Jan 21, Train run finished 	13TeV AOD validation 	
#  bash DownScript.sh 3416 3420 3419 3418 3417  GA_pp_MC_AOD  ?_GammaConvCalo_2001 -RL_listDPG -runwise  $1
#  bash DownScript.sh 3418 3417 -Name_MC_AOD235_16 GA_pp_MC_AOD  ?_GammaConvCalo_2001 -mergeall -RL_listDPG -noDown $1
#  bash DownScript.sh 3419 -Name_MC_AOD235_17 GA_pp_MC_AOD  ?_GammaConvCalo_2001 -mergeall -RL_listDPG -noDown $1
#  bash DownScript.sh 3416 3420 -Name_MC_AOD235_18 GA_pp_MC_AOD  ?_GammaConvCalo_2001 -mergeall -RL_listDPG -noDown $1
#  bash DownScript.sh 3416 3420 3419 3418 3417 -Name_MC_AOD235 GA_pp_MC_AOD  ?_GammaConvCalo_2001 -mergeall -RL_listDPG -noDown $1




###############################################

# # 1877 	vAN-20210721_ROOT6-1 	LHC18c_13TeV_LowB_pass1_AOD236 	2021 Jul 22, Train run finished 	request Adrian 	
# # 1876 	vAN-20210721_ROOT6-1 	LHC17g_13TeV_LowB_pass1_AOD236 	2021 Jul 22, Train run finished 	request Adrian 	
# # 1875 	vAN-20210721_ROOT6-1 	LHC16f_13TeV_LowB_pass1_AOD236 	2021 Jul 22, Train run finished 	request Adrian 	
# bash DownScript.sh 1877 GA_pp_AOD  ?_GammaConvCalo ?_GammaCalo ?_zip -RL_listDPGTrackIncAccTPC $1
# bash DownScript.sh 1876 GA_pp_AOD  ?_GammaConvCalo ?_GammaCalo ?_zip -RL_listDPGTrackIncAccTPC $1
# bash DownScript.sh 1875 GA_pp_AOD  ?_GammaConvCalo ?_GammaCalo ?_zip Child_1 Child_2 -RL_listDPGTrackIncAccTPC $1
# bash DownScript.sh 1877 1876 1875 -Name_Data_NewMBW3 GA_pp_AOD ?_zip ?_GammaConvCalo ?_GammaCalo -mergetrains -RL_listDPGTrackIncAccTPC -noDown $1


# # 1874 	vAN-20210721_ROOT6-1 	LHC18_13TeV_pass1_AOD234 	2021 Jul 22, Train run finished 	request Adrian 	
# # 1873 	vAN-20210721_ROOT6-1 	LHC17_13TeV_pass1_AOD234 	2021 Jul 22, Train run: All jobs submitted 	request Adrian 	
# # 1872 	vAN-20210721_ROOT6-1 	LHC16_13TeV_pass1_2_AOD234 	2021 Jul 22, Train run finished 	request Adrian 
# bash DownScript.sh 1874 1873 1872 GA_pp_AOD ?_zip ?_GammaConvCalo ?_GammaCalo -RL_listPHOS  $1
# bash DownScript.sh 1874 1873 1872 -Name_Data_NewMBW3 GA_pp_AOD ?_GammaConvCalo ?_GammaCalo -mergetrains -RL_listPCMEDC -RL_listPHOS -noDown $1

# # 3894 	vAN-20210721_ROOT6-1 	LHC18h1_PYT8_13TeV_ancLHC18c_LowB_AOD237 	2021 Jul 22, Train run finished 	request Adrian 	
# # 3893 	vAN-20210721_ROOT6-1 	LHC17h3_PYT8_13TeV_anchLHC17g_LowB_AOD237 	2021 Jul 22, Train run finished 	request Adrian 	
# # 3892 	vAN-20210721_ROOT6-1 	LHC17d1_PYT8_13TeV_ancLHC16f_LowB_AOD237 	2021 Jul 22, Train run finished 	request Adrian 	
# bash DownScript.sh 3894 GA_pp_MC_AOD ?_zip ?_GammaConvCalo ?_GammaCalo Child_1 Child_2 Child_4 Child_5 -RL_listDPGTrackIncAccTPC $1
# bash DownScript.sh 3893 GA_pp_MC_AOD ?_zip ?_GammaConvCalo ?_GammaCalo -RL_listDPGTrackIncAccTPC $1
# bash DownScript.sh 3892 GA_pp_MC_AOD ?_zip ?_GammaConvCalo ?_GammaCalo -RL_listDPGTrackIncAccTPC $1
# bash DownScript.sh 3894 3893 3892 -Name_MC_NewMBW3 GA_pp_MC_AOD ?_zip ?_GammaConvCalo ?_GammaCalo -mergetrains -RL_listDPGTrackIncAccTPC -noDown $1

# # 3891 	vAN-20210721_ROOT6-1 	PYT8_13TeV_anchLHC18_AOD_extra 	2021 Jul 22, Train run finished 	request Adrian 	
# # 3890 	vAN-20210721_ROOT6-1 	PYT8_13TeV_anchLHC18_AOD235 	2021 Jul 22, Train run finished 	request Adrian 	
# # 3889 	vAN-20210721_ROOT6-1 	PYT8_13TeV_anchLHC17_AOD235 	2021 Jul 22, Train run finished 	request Adrian 	
# # 3888 	vAN-20210721_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD235_extra 	2021 Jul 22, Train run finished 	request Adrian 	
# # 3887 	vAN-20210721_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD235 	2021 Jul 22, Train run finished 	request Adrian 
# bash DownScript.sh 3891 3890 3889 3888 3887 GA_pp_MC_AOD ?_GammaCalo ?_GammaConvCalo -RL_listPHOS  $1
# bash DownScript.sh 3891 3890 3889 3888 3887 -Name_MC_NewMBW3 GA_pp_MC_AOD ?_Gamma -mergetrains -RL_listPHOS  -noDown $1


##########################

# # 3860 	vAN-20210709_ROOT6-1 	LHC18h1_PYT8_13TeV_ancLHC18c_LowB_AOD237 	2021 Jul 10, Train run finished 	request Adrian, MBW 	
# # 3859 	vAN-20210709_ROOT6-1 	LHC17h3_PYT8_13TeV_anchLHC17g_LowB_AOD237 	2021 Jul 10, Train run finished 	request Adrian, MBW 	
# # 3858 	vAN-20210709_ROOT6-1 	LHC17d1_PYT8_13TeV_ancLHC16f_LowB_AOD237 	2021 Jul 10, Train run: All jobs submitted 	request Adrian, MBW 
# bash DownScript.sh 3860 GA_pp_MC_AOD ?_zip ?_GammaConvCalo Child_1 Child_2 Child_4 Child_5 -RL_listDPGTrackIncAccTPCandEMC -RL_listDPGTrackIncAccTPC $1
# bash DownScript.sh 3859 GA_pp_MC_AOD ?_zip ?_GammaConvCalo -RL_listDPGTrackIncAccTPCandEMC -RL_listDPGTrackIncAccTPC $1
# bash DownScript.sh 3858 GA_pp_MC_AOD ?_zip ?_GammaConvCalo -RL_listDPGTrackIncAccTPCandEMC -RL_listDPGTrackIncAccTPC $1
# bash DownScript.sh 3860 3859 3858 -Name_MC_NewMBW2 GA_pp_MC_AOD ?_zip ?_GammaConvCalo -mergetrains -RL_listDPGTrackIncAccTPCandEMC -RL_listDPGTrackIncAccTPC -noDown $1

# 3811 	vAN-20210629_ROOT6-1 	LHC18h1_PYT8_13TeV_ancLHC18c_LowB_AOD237 	2021 Jun 30, Train run: All jobs submitted 	request Adrian 	
# 3810 	vAN-20210629_ROOT6-1 	LHC17h3_PYT8_13TeV_anchLHC17g_LowB_AOD237 	2021 Jun 30, Train run finished 	request Adrian 	
# 3809 	vAN-20210629_ROOT6-1 	LHC17d1_PYT8_13TeV_ancLHC16f_LowB_AOD237 	2021 Jun 30, Train run: All jobs submitted 	request Adrian 
# bash DownScript.sh 3809 GA_pp_MC_AOD ?_zip ?_GammaConvCalo -RL_listDPGTrackIncAccTPCandEMC -RL_listDPGTrackIncAccTPC $1
# bash DownScript.sh 3810 GA_pp_MC_AOD ?_zip ?_GammaConvCalo -RL_listDPGTrackIncAccTPCandEMC -RL_listDPGTrackIncAccTPC $1
# bash DownScript.sh 3811 GA_pp_MC_AOD ?_zip ?_GammaConvCalo Child_1 Child_2 Child_4 Child_5 -RL_listDPGTrackIncAccTPCandEMC -RL_listDPGTrackIncAccTPC $1
# bash DownScript.sh 3809 3810 3811 -Name_MC_NewMBW GA_pp_MC_AOD ?_zip ?_GammaConvCalo -mergetrains -RL_listDPGTrackIncAccTPCandEMC -RL_listDPGTrackIncAccTPC -noDown $1

# 1840 	vAN-20210625_ROOT6-1 	LHC18c_13TeV_LowB_pass1_AOD236 	2021 Jun 28, Train run finished 	request Edgar, Adrian 	
# 1839 	vAN-20210625_ROOT6-1 	LHC17g_13TeV_LowB_pass1_AOD236 	2021 Jun 28, Train run finished 	request Edgar, Adrian 	
# 1838 	vAN-20210625_ROOT6-1 	LHC16f_13TeV_LowB_pass1_AOD236 	2021 Jun 28, Train run finished 	request Edgar, Adrian 	
# bash DownScript.sh 1840 GA_pp_AOD  ?_GammaConvCalo ?_zip -RL_listDPGTrackIncAccTPCandEMC $1
# bash DownScript.sh 1839 GA_pp_AOD  ?_GammaConvCalo ?_zip -RL_listDPGTrackIncAccTPCandEMC $1
# bash DownScript.sh 1838 GA_pp_AOD  ?_GammaConvCalo ?_zip Child_1 Child_2 -RL_listDPGTrackIncAccTPCandEMC $1
# bash DownScript.sh 1840 1839 1838 -Name_Data_NewMBW GA_pp_AOD ?_zip ?_GammaConvCalo -mergetrains -RL_listDPGTrackIncAccTPCandEMC -noDown $1

# 1837 	vAN-20210625_ROOT6-1 	LHC18_13TeV_pass1_AOD234 	2021 Jun 29, Train run finished 	request Edgar, Adrian 	
# 1836 	vAN-20210625_ROOT6-1 	LHC17_13TeV_pass1_AOD234 	2021 Jun 29, Train run: All jobs submitted 	request Edgar, Adrian 	
# 1835 	vAN-20210625_ROOT6-1 	LHC16_13TeV_pass1_2_AOD234 	2021 Jun 29, Train run finished 	request Adrian, Edgar 	
# bash DownScript.sh 1837 1836 1835 GA_pp_AOD ?_zip ?_GammaConvCalo_3 ?_GammaConvCalo_9 -RL_listPHOS  $1
# bash DownScript.sh 1837 1836 1835 GA_pp_AOD ?_zip ?_GammaConvCalo_2 -RL_listPCMEDC $1
# bash DownScript.sh 1837 1836 1835 -Name_Data_NewMBW GA_pp_AOD ?_GammaConvCalo -mergetrains -RL_listPCMEDC -RL_listPHOS -noDown $1



# 3800 	vAN-20210628_ROOT6-1 	PYT8_13TeV_anchLHC18_AOD_extra 	2021 Jun 29, Train run: All jobs submitted 	request Adrian 	
# 3799 	vAN-20210628_ROOT6-1 	PYT8_13TeV_anchLHC18_AOD235 	2021 Jun 29, Train run finished 	request Adrian 	
# 3798 	vAN-20210628_ROOT6-1 	PYT8_13TeV_anchLHC17_AOD235 	2021 Jun 29, Train run: All jobs submitted 	request Adrian 	
# 3797 	vAN-20210628_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD235_extra 	2021 Jun 29, Train run finished 	request Adrian 	
# 3796 	vAN-20210628_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD235 	2021 Jun 29, Train run finished 	request Adrian 
# bash DownScript.sh 3800 3799 3798 3797 3796 GA_pp_MC_AOD ?_GammaConvCalo_3 ?_GammaConvCalo_9 -RL_listPHOS  $1
# bash DownScript.sh 3800 3799 3798 3797 3796 GA_pp_MC_AOD ?_GammaConvCalo_2 -RL_listPCMEDC $1
# bash DownScript.sh 3800 3799 3798 3797 3796 -Name_MC_NewMBW GA_pp_MC_AOD ?_Gamma -mergetrains -RL_listPHOS -RL_listPCMEDC -RL_listDPGEDC -RL_listDPGEMC -noDown $1





# 3774 	vAN-20210610_ROOT6-1 	PYT8_13TeV_anchLHC18_AOD_extra 	2021 Jun 11, Train run: All jobs submitted 	request Adrian, Joshua 	
# 3773 	vAN-20210610_ROOT6-1 	PYT8_13TeV_anchLHC18_AOD235 	2021 Jun 11, Train run finished 	request Adrian, Joshua 	
# 3772 	vAN-20210610_ROOT6-1 	PYT8_13TeV_anchLHC17_AOD235 	2021 Jun 11, Train run finished 	request Adrian, Joshua 	
# 3771 	vAN-20210610_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD235_extra 	2021 Jun 11, Train run finished 	request Adrian, Joshua 	
# 3770 	vAN-20210610_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD235 	2021 Jun 11, Train run finished 	request Adrian, Joshua 
# bash DownScript.sh 3774 3773 3772 3771 3770 GA_pp_MC_AOD ?_GammaCalo_8 ?_GammaConvCalo_9 -RL_listPHOS  $1
# bash DownScript.sh 3774 3773 3772 3771 3770 GA_pp_MC_AOD ?_GammaCalo_2062 -RL_listDPGEDC_OR_listDPGEMC $1
# bash DownScript.sh 3774 3773 3772 3771 3770 GA_pp_MC_AOD ?_GammaConvCalo_2 -RL_listPCMEDC $1
# bash DownScript.sh 3774 3773 3772 3771 3770 -Name_MC_AdditionTrue GA_pp_MC_AOD ?_Gamma -mergetrains -RL_listPHOS -RL_listPCMEDC -RL_listDPGEDC -RL_listDPGEMC -mergetrains  -noDown $1

# # 3763 	vAN-20210608_ROOT6-1 	PYT8_13TeV_anchLHC18_AOD_extra 	2021 Jun 09, Train run: All jobs submitted 	request Adrian 	
# # 3762 	vAN-20210608_ROOT6-1 	PYT8_13TeV_anchLHC18_AOD235 	2021 Jun 09, Train run: All jobs submitted 	request Adrian 	
# # 3761 	vAN-20210608_ROOT6-1 	PYT8_13TeV_anchLHC17_AOD235 	2021 Jun 09, Train run: All jobs submitted 	request Adrian 	
# # 3760 	vAN-20210608_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD235_extra 	2021 Jun 09, Train run finished 	request Adrian 	
# # 3759 	vAN-20210608_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD235 	2021 Jun 09, Train run finished 	request Adrian 
# bash DownScript.sh 3763 GA_pp_MC_AOD  ?_zip ?_Gamma -RAll checks have passed_PHOS_var GA_pp_MC_AOD ?_Gamma -mergetrains -RL_listPHOS -mergetrains  -noDown $1

# 1804 	vAN-20210527_ROOT6-1 	LHC18_13TeV_pass1_AOD234 	2021 May 28, Train run finished 	request Adrian, Jens 	
# 1803 	vAN-20210527_ROOT6-1 	LHC17_13TeV_pass1_AOD234 	2021 May 28, Train run finished 	request Adrian, Jens 	
# 1802 	vAN-20210527_ROOT6-1 	LHC16_13TeV_pass1_2_AOD234 	2021 May 28, Train run finished 	request Adrian, Jens 
# bash DownScript.sh 1804 1803 1802 GA_pp_AOD ?_zip ?_Gamma -RL_listPHOS $1
# bash DownScript.sh 1804 1803 1802 GA_pp_AOD -Name_Data_PHOS_var -mergetrains ?_Gamma -RL_listPHOS -noDown $1


# 3693 	vAN-20210527_ROOT6-1 	PYT8_13TeV_anchLHC18_AOD_extra 	2021 May 28, Train run finished 	request Adrian, Jens 	
# 3692 	vAN-20210527_ROOT6-1 	PYT8_13TeV_anchLHC18_AOD235 	2021 May 28, Train run finished 	request Adrian, Jens 	
# 3691 	vAN-20210527_ROOT6-1 	PYT8_13TeV_anchLHC17_AOD235 	2021 May 28, Train run finished 	request Adrian, Jens 	
# 3690 	vAN-20210527_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD235_extra 	2021 May 28, Train run finished 	request Adrian, Jens 	
# 3689 	vAN-20210527_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD235 	2021 May 28, Train run finished 	request Adrian 
# bash DownScript.sh 3693 3692 3691 3690 3689 GA_pp_MC_AOD  ?_zip ?_Gamma -RL_listPHOS $1
# bash DownScript.sh 3693 3692 3691 3690 3689 -Name_MC_PHOS_var GA_pp_MC_AOD ?_Gamma -mergetrains -RL_listPHOS -mergetrains  -noDown $1

# 3677 	vAN-20210505_ROOT6-1 	LHC19i3c2_JJhigh_13TeV_DCAL_anchLHC18 	2021 May 14, Train run finished 	request Adrian 	
# 3676 	vAN-20210505_ROOT6-1 	LHC19i3c1_JJhigh_13TeV_EMCal_anchLHC18 	2021 May 14, Train run finished 	request Adrian 	
# 3675 	vAN-20210505_ROOT6-1 	LHC19i3b2_JJlow_13TeV_DCAL_anchLHC18 	2021 May 14, Train run finished 	request Adrian 	
# 3674 	vAN-20210505_ROOT6-1 	LHC19i3b1_JJlow_13TeV_EMCal_anchLHC18 	2021 May 14, Train run finished 	request Adrian 
# bash DownScript.sh 3677 GA_pp_MC_AOD ?_zip ?_GammaConvCalo -IsJJ -RL_list_Trigger -norunwise $1
# bash DownScript.sh 3676 GA_pp_MC_AOD ?_zip ?_GammaConvCalo -IsJJ -RL_list_Trigger -norunwise $1
# bash DownScript.sh 3675 GA_pp_MC_AOD ?_zip ?_GammaConvCalo -IsJJ -RL_list_Trigger -norunwise $1
# bash DownScript.sh 3674 GA_pp_MC_AOD ?_zip ?_GammaConvCalo -IsJJ -RL_list_Trigger -norunwise $1

# 1789 	vAN-20210505_ROOT6-1 	LHC18_13TeV_pass1_AOD234 	2021 May 08, Train run finished 	request Adrian, Hanseo 	
# 1788 	vAN-20210505_ROOT6-1 	LHC17_13TeV_pass1_AOD234 	2021 May 08, Train run: All jobs submitted 	request Adrian, Hanseo 	
# 1787 	vAN-20210505_ROOT6-1 	LHC16_13TeV_pass1_2_AOD234 	2021 May 07, Train run finished 	request Adrian, Hanseo 	
# bash DownScript.sh 1789 1788 1787 GA_pp_AOD ?_zip ?_GammaConvCalo -RL_listPCMEDC $1
# bash DownScript.sh 1789 1788 1787 -Name_Data_v22 GA_pp_AOD ?_GammaConvCalo -mergetrains -RL_listPCMEDC -noDown $1


# 3652 	vAN-20210505_ROOT6-1 	LHC19i3c2_JJhigh_13TeV_DCAL_anchLHC18 	2021 May 13, Train run: All jobs submitted 	request Adrian 	
# 3651 	vAN-20210505_ROOT6-1 	LHC19i3c1_JJhigh_13TeV_EMCal_anchLHC18 	2021 May 13, Train run: All jobs submitted 	request Adrian 	
# 3650 	vAN-20210505_ROOT6-1 	LHC19i3b2_JJlow_13TeV_DCAL_anchLHC18 	2021 May 13, Train run finished 	request Adrian 	
# 3649 	vAN-20210505_ROOT6-1 	LHC19i3b1_JJlow_13TeV_EMCal_anchLHC18 	2021 May 12, Train run finished 	request Adrian 
# bash DownScript.sh 3652 GA_pp_MC_AOD ?_zip ?_GammaConvCalo -IsJJ -RL_list_Trigger -norunwise $1
# bash DownScript.sh 3651 GA_pp_MC_AOD ?_zip ?_GammaConvCalo -IsJJ -RL_list_Trigger -norunwise $1
# bash DownScript.sh 3650 GA_pp_MC_AOD ?_zip ?_GammaConvCalo -IsJJ -RL_list_Trigger -norunwise $1
# bash DownScript.sh 3649 GA_pp_MC_AOD ?_zip ?_GammaConvCalo -IsJJ -RL_list_Trigger -norunwise $1

#  3610 	vAN-20210406_ROOT6-1 	PYT8_13TeV_anchLHC17_AOD235 	2021 Apr 07, Train run finished 	request Adrian 
# bash DownScript.sh 3610 -Name_MC_PCM_calib_v2 GA_pp_MC_AOD ?_GammaConvV1 -RL_listPCM $1
# bash DownScript.sh 3610 -Name_MC_PCM_calib_v2 GA_pp_MC_AOD ?_GammaConvCalo -RL_listPCMEDC $1

#  1764 	vAN-20210402_ROOT6-1 	LHC18_13TeV_pass1_AOD234 	2021 Apr 04, Train run finished 	request Steven, Marvin 	
# 1763 	vAN-20210402_ROOT6-1 	LHC17_13TeV_pass1_AOD234 	2021 Apr 03, Train run finished 	request Steven, Marvin 	
# 1762 	vAN-20210402_ROOT6-1 	LHC16_13TeV_pass1_2_AOD234 	2021 Apr 03, Train run finished 	request Steven, Marvin 	
# bash DownScript.sh 1764 1763 1762 GA_pp_AOD ?_zip ?_Sigma -RL_listPHOS $1
# bash DownScript.sh 1764 1763 1762 GA_pp_AOD -Name_SimgaData -mergetrains ?_Sigma -RL_listPHOS -noDown $1

# 3605 	vAN-20210402_ROOT6-1 	PYT8_13TeV_anchLHC18_AOD_extra 	2021 Apr 03, Train run finished 	request Steven 	
# 3604 	vAN-20210402_ROOT6-1 	PYT8_13TeV_anchLHC18_AOD235 	2021 Apr 03, Train run finished 	request Steven 	
# 3603 	vAN-20210402_ROOT6-1 	PYT8_13TeV_anchLHC17_AOD235 	2021 Apr 03, Train run finished 	request Steven 	
# 3602 	vAN-20210402_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD235_extra 	2021 Apr 03, Train run finished 	request Steven 	
# 3601 	vAN-20210402_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD235 	2021 Apr 03, Train run finished 	request Steven 
# bash DownScript.sh 3605 3604 3603 3602 3601 GA_pp_MC_AOD ?_zip ?_Sigma -RL_listPHOS $1
# bash DownScript.sh 3605 3604 3603 3602 3601 GA_pp_MC_AOD -Name_SigmaMC -mergetrains ?_Sigma -RL_listPHOS -noDown $1

# 3600 	vAN-20210329_ROOT6-1 	LHC19i3b2_JJlow_13TeV_DCAL_anchLHC18 	2021 Apr 01, Train run finished 	request Jens, Adrian 	
# 3599 	vAN-20210329_ROOT6-1 	LHC18l6b2_JJlow_13TeV_DCal_anchLHC17 	2021 Apr 01, Train run finished 	request Jens, Adrian 	
# 3598 	vAN-20210329_ROOT6-1 	LHC20b1b2_JJlow_13TeV_DCal_anchLHC16 	2021 Apr 01, Train run finished 	request Jens, Adrian 	
# bash DownScript.sh 3600 GA_pp_MC_AOD ?_zip ?_Gamma -IsJJ $1
# bash DownScript.sh 3599 GA_pp_MC_AOD ?_zip ?_Gamma -IsJJ $1
# bash DownScript.sh 3598 GA_pp_MC_AOD ?_zip ?_Gamma -IsJJ $1


# 3597 	vAN-20210329_ROOT6-1 	PYT8JJ_13TeV_anchLHC18_AOD 	2021 Apr 01, Train run: All jobs submitted 	request Jens, Adrian 	
# 3596 	vAN-20210329_ROOT6-1 	PYT8JJ_13TeV_anchLHC17_AOD 	2021 Apr 01, Train run: All jobs submitted 	request Jens, Adrian 	
# 3595 	vAN-20210329_ROOT6-1 	PYT8JJ_13TeV_anchLHC16_AOD209 	2021 Apr 01, Train run: All jobs submitted 	request Jens, Adrian 
# bash DownScript.sh 3597 GA_pp_MC_AOD ?_zip ?_Gamma -IsJJ -RL_list_Joshua_OR_default $1
# bash DownScript.sh 3596 GA_pp_MC_AOD ?_zip ?_Gamma -IsJJ $1
# bash DownScript.sh 3595 GA_pp_MC_AOD ?_zip ?_Gamma -IsJJ $1
# bash DownScript.sh 3597 3596 3595 -Name_JJMC_PHOS_v1 GA_pp_MC_AOD ?_zip ?_Gamma -mergetrains -RL_default -noDown $1

# GA_pp_MC_AOD: 3585-389
# GA_pp_AOD: 1752-1754
# bash DownScript.sh 1754 1753 1752 GA_pp_AOD ?_Sigma -RL_listPHOS $1
# bash DownScript.sh 1754 1753 1752 GA_pp_AOD -Name_SimgaData -mergetrains ?_Sigma -RL_listPHOS -noDown $1
# bash DownScript.sh 3585 3586 3587 3588 3589 GA_pp_MC_AOD ?_Sigma -RL_listPHOS $1
# bash DownScript.sh 3585 3586 3587 3588 3589 GA_pp_MC_AOD -Name_SigmaMC -mergetrains ?_Sigma -RL_listPHOS -noDown $1

# 1754 	vAN-20210326_ROOT6-1 	LHC18_13TeV_pass1_AOD234 	2021 Mar 26, Train run: All jobs submitted 	request Steven & Adrian 	
# 1753 	vAN-20210326_ROOT6-1 	LHC17_13TeV_pass1_AOD234 	2021 Mar 26, Train run finished 	request Steven ,Adrian 	
# 1752 	vAN-20210326_ROOT6-1 	LHC16_13TeV_pass1_2_AOD234 	2021 Mar 26, Train run finished 	request Steven, Adrian, E... 	
# bash DownScript.sh 1754 1753 1752 GA_pp_AOD ?_GammaConvCalo -RL_listEDC_trigger_OR_listDPGEDCtrigger $1
# bash DownScript.sh 1754 1753 1752 GA_pp_AOD -Name_Data_v21 -mergetrains ?_GammaConvCalo -RL_listDPGEDCtrigger -noDown $1

# 1750 	vAN-20210316_ROOT6-1 	LHC18_13TeV_pass1_AOD234 	2021 Mar 19, Train run: All jobs submitted 	request Hanseo, Adrian 	
# 1749 	vAN-20210316_ROOT6-1 	LHC17_13TeV_pass1_AOD234 	2021 Mar 19, Train run finished 	request Hanseo, Adrian 	
# 1748 	vAN-20210316_ROOT6-1 	LHC16_13TeV_pass1_2_AOD234 	2021 Mar 19, Train run finished 	request Hanseo, Adrian 	
# bash DownScript.sh 1750 1749 1748 GA_pp_AOD ?_GammaConvCalo_9 -RL_listPHOS $1
# bash DownScript.sh 1750 1749 1748 GA_pp_AOD ?_GammaConvV1 -RL_listPCM $1
# bash DownScript.sh 1749 GA_pp_AOD ?_GammaConvCalo_2 -RL_listPCMEDC $1
# bash DownScript.sh 1750 1749 1748 -Name_Data_v21 GA_pp_AOD ?_Gamma -mergetrains -RL_listPCMEDC -RL_listPCM -RL_listPHOS  -noDown $1

# 3565 	vAN-20210316_ROOT6-1 	PYT8_13TeV_anchLHC18_AOD_extra 	2021 Mar 18, Train run finished 	request Hanseo, Adrian 	
# 3564 	vAN-20210316_ROOT6-1 	PYT8_13TeV_anchLHC18_AOD235 	2021 Mar 18, Train run finished 	request Hanseo, Adrian 	
# 3563 	vAN-20210316_ROOT6-1 	PYT8_13TeV_anchLHC17_AOD235 	2021 Mar 18, Train run finished 	request Hanseo, Adrian 	
# 3562 	vAN-20210316_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD235_extra 	2021 Mar 18, Train run finished 	request Hanseo, Adrian 	
# 3561 	vAN-20210316_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD235 	2021 Mar 18, Train run finished 	request Hanseo, Adrian 
# bash DownScript.sh 3565 3564 3563 3562 3561 GA_pp_MC_AOD   ?_GammaConvCalo_9 -RL_listPHOS $1
# bash DownScript.sh 3565 3564 3563 3562 3561 GA_pp_MC_AOD   ?_GammaConvV1 -RL_listPCM $1
# bash DownScript.sh 3563 GA_pp_MC_AOD   ?_GammaConvCalo_2 -RL_listPCMEDC $1
# bash DownScript.sh 3565 3564 3563 3562 3561 -Name_MC_v21 GA_pp_MC_AOD ?_Gamma -mergetrains -RL_listPCMEDC -RL_listPCM -RL_listPHOS -mergetrains  -noDown $1

# 3569 	vAN-20210318_ROOT6-1 	LHC19i3c2_JJhigh_13TeV_DCAL_anchLHC18 	2021 Mar 20, Train run: All jobs submitted 	request Joshua, Adrian 	
# 3568 	vAN-20210318_ROOT6-1 	LHC19i3c1_JJhigh_13TeV_EMCal_anchLHC18 	2021 Mar 20, Train run: All jobs submitted 	request Joshua, Adrian 	
# 3567 	vAN-20210318_ROOT6-1 	LHC19i3b2_JJlow_13TeV_DCAL_anchLHC18 	2021 Mar 20, Train run: All jobs submitted 	request Joshua, Adrian 	
# 3566 	vAN-20210318_ROOT6-1 	LHC19i3b1_JJlow_13TeV_EMCal_anchLHC18 	2021 Mar 20, Train run: All jobs submitted 	request Joshua, Adrian 	
# 3584 	vAN-20210318_ROOT6-1 	LHC20b1c2_JJhigh_13TeV_DCal_anchLHC16 	2021 Mar 21, Train run: All jobs submitted 	request Joshua, Adrian 	
# 3583 	vAN-20210318_ROOT6-1 	LHC20b1c1_JJhigh_13TeV_EMCal_anchLHC16 	2021 Mar 21, Train run: All jobs submitted 	request Joshua, Adrian 	
# 3582 	vAN-20210318_ROOT6-1 	LHC20b1b2_JJlow_13TeV_DCal_anchLHC16 	2021 Mar 21, Train run: All jobs submitted 	request Joshua, Adrian 	
# 3581 	vAN-20210318_ROOT6-1 	LHC20b1b1_JJlow_13TeV_EMCal_anchLHC16 	2021 Mar 21, Train run: All jobs submitted 	request Joshua, Adrian 	
# 3580 	vAN-20210318_ROOT6-1 	LHC18l6c2_JJhigh_13TeV_DCAL_anchLHC17 	2021 Mar 21, Train run: All jobs submitted 	request Joshua, Adrian 	
# 3579 	vAN-20210318_ROOT6-1 	LHC18l6c1_JJhigh_13TeV_EMCal_anchLHC17 	2021 Mar 21, Train run: All jobs submitted 	request Joshua, Adrian 	
# 3578 	vAN-20210318_ROOT6-1 	LHC18l6b2_JJlow_13TeV_DCal_anchLHC17 	2021 Mar 21, Train run finished 	request Joshua, Adrian 	
# 3577 	vAN-20210318_ROOT6-1 	LHC18l6b1_JJlow_13TeV_EMCal_anchLHC17 	2021 Mar 21, Train run: All jobs submitted 	request Joshua, Adrian 
# bash DownScript.sh 3569 GA_pp_MC_AOD ?_GammaConvCalo -IsJJ $1
# bash DownScript.sh 3568 GA_pp_MC_AOD ?_GammaConvCalo -IsJJ $1
# bash DownScript.sh 3567 GA_pp_MC_AOD ?_GammaConvCalo -IsJJ $1
# bash DownScript.sh 3566 GA_pp_MC_AOD ?_GammaConvCalo -IsJJ $1
# bash DownScript.sh 3584 GA_pp_MC_AOD ?_GammaConvCalo -IsJJ $1
# bash DownScript.sh 3583 GA_pp_MC_AOD ?_GammaConvCalo -IsJJ $1
# bash DownScript.sh 3582 GA_pp_MC_AOD ?_GammaConvCalo -IsJJ $1
# bash DownScript.sh 3581 GA_pp_MC_AOD ?_zip ?_GammaConvCalo -IsJJ $1
# bash DownScript.sh 3581 GA_pp_MC_AOD ?_zip ?_Gamma -IsJJ $1
# bash DownScript.sh 3580 GA_pp_MC_AOD ?_GammaConvCalo -IsJJ $1
# bash DownScript.sh 3579 GA_pp_MC_AOD ?_GammaConvCalo -IsJJ $1
# bash DownScript.sh 3578 GA_pp_MC_AOD ?_GammaConvCalo -IsJJ $1
# bash DownScript.sh 3577 GA_pp_MC_AOD ?_GammaConvCalo -IsJJ $1

# bash DownScript.sh 3462 3461 -Name_JJhigh GA_pp_MC_AOD -mergetrains ?_GammaConvCalo -RL_default -noDown $1
# bash DownScript.sh 3459 3460 -Name_JJlow GA_pp_MC_AOD -mergetrains ?_GammaConvCalo -RL_default -noDown $1

###############################

# 1732 	vAN-20210301_ROOT6-1 	LHC17_13TeV_pass1_AOD234 	2021 Mar 05, Train run finished 	request Adrian, smearing ... 	
# bash DownScript.sh 1732 -Name_Data_PCM_calib GA_pp_AOD ?_GammaConvV1 -RL_listPCM $1

# 3521 	vAN-20210301_ROOT6-1 	PYT8_13TeV_anchLHC17_AOD235 	2021 Mar 05, Train run finished 	request Adrian, smearing ...
# bash DownScript.sh 3521 -Name_MC_PCM_calib GA_pp_MC_AOD ?_GammaConvV1 -RL_listPCM $1


# 1726 	vAN-20210224_ROOT6-1 	LHC18_13TeV_pass1_AOD234 	2021 Feb 25, Train run finished 	request Adrian, Jens 	
# 1725 	vAN-20210224_ROOT6-1 	LHC17_13TeV_pass1_AOD234 	2021 Feb 25, Train run finished 	request Adrian, Jens 	
# 1724 	vAN-20210224_ROOT6-1 	LHC16_13TeV_pass1_2_AOD234 	2021 Feb 25, Train run finished 	request Adrian, Jens 
# bash DownScript.sh 1726 1725 1724 GA_pp_AOD ?_GammaConvCalo_2 -mergetrains -RL_listPCMEDC $1
# bash DownScript.sh 1726 1725 1724 GA_pp_AOD ?_zip ?_GammaCalo_8 ?_GammaConvCalo_9 -mergetrains -RL_listPHOS $1
# bash DownScript.sh 1724 -Name_Data_16_v20 GA_pp_AOD ?_GammaConvCalo  ?_GammaCalo_8 -mergetrains -RL_listPCMEDC -RL_listPHOS -noDown $1
# bash DownScript.sh 1725 -Name_Data_17_v20 GA_pp_AOD ?_GammaConvCalo  ?_GammaCalo_8 -mergetrains -RL_listPCMEDC -RL_listPHOS -noDown $1
# bash DownScript.sh 1726 -Name_Data_18_v20 GA_pp_AOD ?_GammaConvCalo  ?_GammaCalo_8 -mergetrains -RL_listPCMEDC -RL_listPHOS -noDown $1
# bash DownScript.sh 1726 1725 1724 -Name_Data_v20 GA_pp_AOD ?_GammaConvCalo  ?_GammaCalo_8 -mergetrains -RL_listPCMEDC -RL_listPHOS -noDown $1



# 3504 	vAN-20210224_ROOT6-1 	PYT8_13TeV_anchLHC18_AOD_extra 	2021 Feb 25, Train run finished 	request Adrian, Jens 	
# 3503 	vAN-20210224_ROOT6-1 	PYT8_13TeV_anchLHC18_AOD235 	2021 Feb 25, Train run finished 	request Adrian, Jens 	
# 3502 	vAN-20210224_ROOT6-1 	PYT8_13TeV_anchLHC17_AOD235 	2021 Feb 25, Train run finished 	request Adrian, Jens 	
# 3501 	vAN-20210224_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD235_extra 	2021 Feb 25, Train run finished 	request Adrian, Jens 	
# 3500 	vAN-20210224_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD235 	2021 Feb 25, Train run finished 	request Adrian, Jens 	
# bash DownScript.sh 3504 3503 3502 3501 3500 GA_pp_MC_AOD ?_GammaConvCalo_2  -mergetrains -RL_listPCMEDC $1
# bash DownScript.sh 3504 3503 3502 3501 3500 GA_pp_MC_AOD ?_zip ?_GammaCalo_8 ?_GammaConvCalo_9  -mergetrains -RL_listPHOS $1
# bash DownScript.sh 3501 3500 -Name_MC_16_v20 GA_pp_MC_AOD ?_zip ?_GammaCalo_8 ?_GammaConvCalo_9  -mergetrains -RL_listPHOS  -noDown $1
# bash DownScript.sh 3502 -Name_MC_17_v20 GA_pp_MC_AOD ?_zip ?_GammaCalo_8 ?_GammaConvCalo_9  -mergetrains -RL_listPHOS  -noDown $1
# bash DownScript.sh 3504 3503 -Name_MC_18_v20 GA_pp_MC_AOD ?_zip ?_GammaCalo_8 ?_GammaConvCalo_9  -mergetrains -RL_listPHOS  -noDown $1
# bash DownScript.sh 3504 3503 3502 3501 3500 -Name_MC_v20 GA_pp_MC_AOD ?_zip ?_GammaConvCalo  ?_GammaCalo_8 -mergetrains -mergeperiods -RL_listPHOS -RL_listPCMEDC -noDown $1





# # 3484 	vAN-20210210_ROOT6-1 	LHC18h1_PYT8_13TeV_ancLHC18c_LowB_AOD237 	2021 Feb 13, Train run: All jobs submitted 	request Adrian, Ana 	
# # 3483 	vAN-20210210_ROOT6-1 	LHC17h3_PYT8_13TeV_anchLHC17g_LowB_AOD237 	2021 Feb 13, Train run finished 	request Adrian, Ana 	
# # 3482 	vAN-20210210_ROOT6-1 	LHC17d1_PYT8_13TeV_ancLHC16f_LowB_AOD237 	2021 Feb 12, Train run finished 	request Adrian, Ana 	
# bash DownScript.sh 3482 -Name_MC_lowB_v19_16 GA_pp_MC_AOD ?_zip ?_GammaConvCalo -RL__OR_listDPGTrackIncAccTPCandEMC -RL_listDPGTrackIncAccTPC $1
# bash DownScript.sh 3483 -Name_MC_lowB_v19_17 GA_pp_MC_AOD ?_zip ?_GammaConvCalo -RL_listDPGTrackIncAccTPCandEMC -RL_listDPGTrackIncAccTPC $1
# bash DownScript.sh 3484 -Name_MC_lowB_v19_18 GA_pp_MC_AOD ?_zip ?_GammaConvCalo Child_1 Child_2 -RL_listDPGTrackIncAccTPCandEMC -RL_listDPGTrackIncAccTPC $1
# bash DownScript.sh 3484 3483 3482 -Name_MC_lowB_v19 GA_pp_MC_AOD ?_zip ?_GammaConvCalo -mergetrains -RL_listDPGTrackIncAccTPCandEMC -RL_listDPGTrackIncAccTPC -noDown $1


# # 1697 	vAN-20210127_ROOT6-1 	LHC18c_13TeV_LowB_pass1_AOD236 	2021 Jan 29, Train run finished 	request Adrian 	
# # 1696 	vAN-20210127_ROOT6-1 	LHC17g_13TeV_LowB_pass1_AOD236 	2021 Jan 29, Train run finished 	request Adrian 	
# # 1695 	vAN-20210127_ROOT6-1 	LHC16f_13TeV_LowB_pass1_AOD236 	2021 Jan 29, Train run finished 	request Adrian 
# bash DownScript.sh 1695 -Name_Data_lowB_v19_16 GA_pp_AOD  ?_GammaConvCalo ?_zip -RL_listDPGTrackIncAccTPCandEMC $1
# bash DownScript.sh 1696 -Name_Data_lowB_v19_17 GA_pp_AOD  ?_GammaConvCalo ?_zip -RL_listDPGTrackIncAccTPCandEMC $1
# bash DownScript.sh 1697 -Name_Data_lowB_v19_18 GA_pp_AOD  ?_GammaConvCalo ?_zip Child_1 Child_2 -RL_listDPGTrackIncAccTPCandEMC $1
# bash DownScript.sh 1697 1696 1695 -Name_Data_lowB_v19 GA_pp_AOD ?_zip ?_GammaConvCalo -mergetrains -RL_listDPGTrackIncAccTPCandEMC -noDown $1
# bash DownScript.sh 1697 1612 1611 -Name_Data_lowB_v19_MBW GA_pp_AOD ?_zip ?_GammaConvCalo_5 -mergetrains -RL_listDPGTrackIncAccTPCandEMC -noDown $1



# # 1718 	vAN-20210210_ROOT6-1 	LHC18_13TeV_pass1_AOD234 	2021 Feb 12, Train run: All jobs submitted 	request Adrian 	
# # 1717 	vAN-20210210_ROOT6-1 	LHC17_13TeV_pass1_AOD234 	2021 Feb 12, Train run: All jobs submitted 	request Adrian 	
# # 1716 	vAN-20210210_ROOT6-1 	LHC16_13TeV_pass1_2_AOD234 	2021 Feb 12, Train run: All jobs submitted 	request Adrian 
# bash DownScript.sh 1716 -Name_Data_v19_16 GA_pp_AOD ?_zip ?_GammaConvCalo -RL_listDPGEDC_OR_listDPGEMC -RL_listPHOS $1
# bash DownScript.sh 1717 -Name_Data_v19_17 GA_pp_AOD ?_zip ?_GammaConvCalo -RL_listDPGEDC_OR_listDPGEMC -RL_listPHOS $1
# bash DownScript.sh 1718 -Name_Data_v19_18 GA_pp_AOD ?_zip ?_GammaConvCalo -RL_listDPGEDC_OR_listDPGEMC -RL_listPHOS $1
# bash DownScript.sh 1716 1717 1718 -Name_Data_v19 GA_pp_AOD ?_zip ?_GammaConvCalo -mergetrains -RL_listDPGEDC_OR_listDPGEMC -RL_listPHOS -noDown $1


# # 3481 	vAN-20210210_ROOT6-1 	PYT8_13TeV_anchLHC18_AOD_extra 	2021 Feb 12, Train run finished 	request Adrian 	
# # 3480 	vAN-20210210_ROOT6-1 	PYT8_13TeV_anchLHC18_AOD235 	2021 Feb 12, Train run: All jobs submitted 	request Adrian 	
# # 3479 	vAN-20210210_ROOT6-1 	PYT8_13TeV_anchLHC17_AOD235 	2021 Feb 12, Train run: All jobs submitted 	request Adrian 	
# # 3478 	vAN-20210210_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD235_extra 	2021 Feb 12, Train run finished 	request Adrian 	
# # 3477 	vAN-20210210_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD235 	2021 Feb 12, Train run finished 	request Adrian 
# bash DownScript.sh 3478 3477 -Name_MC_AOD235_16 GA_pp_MC_AOD ?_zip ?_GammaConvCalo -mergetrains -RL_listPHOS -RL_listDPGEDC_OR_listDPGEMC $1
# bash DownScript.sh 3479 -Name_MC_AOD235_17 GA_pp_MC_AOD ?_zip ?_GammaConvCalo -RL_listPHOS -RL_listDPGEDC_OR_listDPGEMC $1
# bash DownScript.sh 3480 -Name_MC_AOD235_18 GA_pp_MC_AOD ?_zip ?_GammaConvCalo -RL_listPHOS -RL_listDPGEDC_OR_listDPGEMC $1
# bash DownScript.sh 3481 3480 -Name_MC_AOD235_18_extra GA_pp_MC_AOD -mergetrains ?_zip ?_GammaConvCalo -RL_listPHOS -RL_listDPGEDC_OR_listDPGEMC $1
# bash DownScript.sh 3481 3480 3479 3478 3477 -Name_MC_AOD235 GA_pp_MC_AOD ?_zip ?_GammaConvCalo -mergetrains -RL_listPHOS -RL_listDPGEDC_OR_listDPGEMC -noDown $1
# bash DownScript.sh 3481 3480 3479 3478 3477 -Name_MC_v19 GA_pp_MC_AOD ?_zip ?_GammaConvCalo -mergetrains -RL_listPHOS -RL_listDPGEDC_OR_listDPGEMC -noDown $1


# ####################### Photon QA #############################
# ### LHC16 Data
# bash DownScript.sh 1716 -Name_LHC16Data_PhotonQA GA_pp_AOD ?_AnalysisResults -onlyrunwise -RL_listDPGIncAcc_OR_listDPGIncTPC -RL_listDPCIncAccTPC_OR_listDPGIncTPC -runwise $1 # -RL_listDPG -RL_listPCM
# ### LHC17 Data
# bash DownScript.sh 1717 -Name_LHC17Data_PhotonQA GA_pp_AOD ?_AnalysisResults -onlyrunwise -RL_listDPGIncAcc_OR_listDPGIncTPC -RL_listDPCIncAccTPC_OR_listDPGIncTPC -runwise $1 # -RL_listDPG -RL_listPCM
# ### LHC18 Data
# bash DownScript.sh 1718 -Name_LHC18Data_PhotonQA GA_pp_AOD ?_AnalysisResults -onlyrunwise -RL_listDPGIncAcc_OR_listDPGIncTPC -RL_listDPCIncAccTPC_OR_listDPGIncTPC -runwise $1 # -RL_listDPG -RL_listPCM
# ### LHC16 MC
# bash DownScript.sh 3478 3477 -Name_LHC16MC_PhotonQA GA_pp_MC_AOD ?_AnalysisResults -onlyrunwise -mergeruns -RL_listDPGIncAcc_OR_listDPGIncTPC -RL_listDPCIncAccTPC_OR_listDPGIncTPC -runwise $1 # -RL_listDPG -RL_listPCM
# ### LHC17 MC
# bash DownScript.sh 3479 -Name_LHC17MC_PhotonQA GA_pp_MC_AOD ?_AnalysisResults -onlyrunwise -RL_listDPGIncAcc_OR_listDPGIncTPC -RL_listDPCIncAccTPC_OR_listDPGIncTPC -runwise $1 # -RL_listDPG -RL_listPCM
# ### LHC18 MC
# bash DownScript.sh 3481 3480 -Name_LHC18MC_PhotonQA GA_pp_MC_AOD ?_AnalysisResults -onlyrunwise -mergeruns -RL_listDPGIncAcc_OR_listDPGIncTPC -RL_listDPCIncAccTPC_OR_listDPGIncTPC -runwise $1 # -RL_listDPG -RL_listPCM
                                                                                                                

# 3462 	vAN-20210205_ROOT6-1 	LHC19i3c2_JJhigh_13TeV_DCAL_anchLHC18 	2021 Feb 07, Train run: All jobs submitted 	request Joshua, Adrian 	
# 3461 	vAN-20210205_ROOT6-1 	LHC19i3c1_JJhigh_13TeV_EMCal_anchLHC18 	2021 Feb 07, Train run: All jobs submitted 	request Joshua, Adrian 	
# 3460 	vAN-20210205_ROOT6-1 	LHC19i3b2_JJlow_13TeV_DCAL_anchLHC18 	2021 Feb 07, Train run: All jobs submitted 	request Joshua, Adrian 	
# 3459 	vAN-20210205_ROOT6-1 	LHC19i3b1_JJlow_13TeV_EMCal_anchLHC18 	2021 Feb 07, Train run: All jobs submitted 	request Joshua, Adrian 
# bash DownScript.sh 3462 -Name_JJhigh_DCAL GA_pp_MC_AOD  ?_GammaConvCalo -IsJJ $1
# bash DownScript.sh 3461 -Name_JJhigh_EMCal GA_pp_MC_AOD  ?_GammaConvCalo -IsJJ $1
# bash DownScript.sh 3460 -Name_JJlow_DCAL GA_pp_MC_AOD  ?_GammaConvCalo -IsJJ $1
# bash DownScript.sh 3459 -Name_JJlow_EMCal GA_pp_MC_AOD  ?_GammaConvCalo -IsJJ $1
# bash DownScript.sh 3462 3461 -Name_JJhigh GA_pp_MC_AOD -mergetrains ?_GammaConvCalo -RL_default -noDown $1
# bash DownScript.sh 3459 3460 -Name_JJlow GA_pp_MC_AOD -mergetrains ?_GammaConvCalo -RL_default -noDown $1


#  ####### AOD Validation

#  # 1692 	vAN-20210118_ROOT6-1 	LHC18c_13TeV_LowB_pass1_AOD236 	2021 Jan 20, Train run finished 	13TeV AOD validation 	
#  # 1691 	vAN-20210118_ROOT6-1 	LHC17g_13TeV_LowB_pass1_AOD236 	2021 Jan 20, Train run finished 	13TeV AOD validation 	
#  # 1690 	vAN-20210118_ROOT6-1 	LHC16f_13TeV_LowB_pass1_AOD236 	2021 Jan 20, Train run finished 	13TeV AOD validation 	
#  bash DownScript.sh 1690 -Name_Data_lowB_AOD236_16 GA_pp_AOD ?_zip ?_GammaCalo_8  ?_GammaConvCalo -runwise -RL_listDPGTrackIncAccTPCandEMC -RL_listDPGTrackIncAccTPC $1
#  bash DownScript.sh 1691 -Name_Data_lowB_AOD236_17 GA_pp_AOD ?_zip ?_GammaCalo_8  ?_GammaConvCalo -runwise -RL_listDPGTrackIncAccTPCandEMC -RL_listDPGTrackIncAccTPC $1
#  bash DownScript.sh 1692 -Name_Data_lowB_AOD236_18 GA_pp_AOD ?_zip ?_GammaCalo_8  ?_GammaConvCalo -runwise Child_1 Child_2 -RL_listDPGTrackIncAccTPCandEMC -RL_listDPGTrackIncAccTPC $1
#  bash DownScript.sh 1692 1691 1690 -Name_Data_lowB_AOD236 GA_pp_AOD ?_zip ?_GammaConvCalo -mergetrains -RL_listDPGTrackIncAccTPCandEMC -noDown $1

#  # 1689 	vAN-20210118_ROOT6-1 	LHC18c_13TeV_LowB_pass1_AOD212 	2021 Jan 20, Train run finished 	13TeV AOD validation 	
#  # 1688 	vAN-20210118_ROOT6-1 	LHC17g_13TeV_LowB_pass1_AOD212 	2021 Jan 20, Train run finished 	13TeV AOD validation 	
#  # 1687 	vAN-20210118_ROOT6-1 	LHC16f_13TeV_LowB_pass1_AOD212 	2021 Jan 20, Train run finished 	13TeV AOD validation 
#  bash DownScript.sh 1687 -Name_Data_lowB_AOD212_16 GA_pp_AOD  ?_GammaCalo_8 ?_zip ?_GammaConvCalo -runwise -RL_listDPGTrackIncAccTPCandEMC $1
#  bash DownScript.sh 1688 -Name_Data_lowB_AOD212_17 GA_pp_AOD  ?_GammaCalo_8 ?_zip ?_GammaConvCalo -runwise -RL_listDPGTrackIncAccTPCandEMC $1
#  bash DownScript.sh 1689 -Name_Data_lowB_AOD212_18 GA_pp_AOD  ?_GammaCalo_8 ?_zip ?_GammaConvCalo -runwise Child_1 Child_2 -RL_listDPGTrackIncAccTPCandEMC $1
#  bash DownScript.sh 1689 1688 1687 -Name_Data_lowB_AOD212 GA_pp_AOD  ?_GammaCalo_8 ?_zip ?_GammaConvCalo -mergetrains -RL_listDPGTrackIncAccTPCandEMC -noDown $1

#  # 3426 	vAN-20210118_ROOT6-1 	LHC18h1_PYT8_13TeV_ancLHC18c_LowB_AOD237 	2021 Jan 21, Train run finished 	13TeV AOD validation 	
#  # 3425 	vAN-20210118_ROOT6-1 	LHC17h3_PYT8_13TeV_anchLHC17g_LowB_AOD237 	2021 Jan 21, Train run finished 	13TeV AOD validation 	
#  # 3424 	vAN-20210118_ROOT6-1 	LHC17d1_PYT8_13TeV_ancLHC16f_LowB_AOD237 	2021 Jan 21, Train run finished 	13TeV AOD validation
#  bash DownScript.sh 3424 -Name_MC_lowB_AOD237_16 GA_pp_MC_AOD  ?_GammaCalo_8 ?_zip ?_GammaConvCalo -runwise -RL_listDPGTrackIncAccTPCandEMC -RL_listDPGTrackIncAccTPC $1
#  bash DownScript.sh 3425 -Name_MC_lowB_AOD237_17 GA_pp_MC_AOD  ?_GammaCalo_8 ?_zip ?_GammaConvCalo -runwise -RL_listDPGTrackIncAccTPCandEMC -RL_listDPGTrackIncAccTPC $1
#  bash DownScript.sh 3426 -Name_MC_lowB_AOD237_18 GA_pp_MC_AOD  ?_GammaCalo_8 ?_zip ?_GammaConvCalo -runwise Child_1 Child_2 -RL_listDPGTrackIncAccTPCandEMC -RL_listDPGTrackIncAccTPC $1
#  bash DownScript.sh 3426 3425 3424 -Name_MC_lowB_AOD237 GA_pp_MC_AOD  ?_GammaCalo_8 ?_zip ?_GammaConvCalo -mergetrains -RL_listDPGTrackIncAccTPCandEMC -noDown $1

#  # 3423 	vAN-20210118_ROOT6-1 	LHC18h1_PYT8_13TeV_ancLHC18c_LowB_AOD213 	2021 Jan 21, Train run finished 	13TeV AOD validation 	
#  # 3422 	vAN-20210118_ROOT6-1 	LHC17h3_PYT8_13TeV_anchLHC17g_LowB_AOD213 	2021 Jan 21, Train run finished 	13TeV AOD validation 	
#  # 3421 	vAN-20210118_ROOT6-1 	LHC17d1_PYT8_13TeV_ancLHC16f_LowB_AOD213 	2021 Jan 21, Train run finished 	13TeV AOD validation 
#  bash DownScript.sh 3421 -Name_MC_lowB_AOD213_16 GA_pp_MC_AOD  ?_GammaCalo_8 ?_zip ?_GammaConvCalo -runwise -RL_listDPGTrackIncAccTPCandEMC $1
#  bash DownScript.sh 3422 -Name_MC_lowB_AOD213_17 GA_pp_MC_AOD  ?_GammaCalo_8 ?_zip ?_GammaConvCalo -runwise -RL_listDPGTrackIncAccTPCandEMC $1
#  bash DownScript.sh 3423 -Name_MC_lowB_AOD213_18 GA_pp_MC_AOD  ?_GammaCalo_8 ?_zip ?_GammaConvCalo -runwise Child_1 Child_2 Child_4 Child_5 -RL_listDPGTrackIncAccTPCandEMC $1
#  bash DownScript.sh 3423 3422 3421 -Name_MC_lowB_AOD213 GA_pp_MC_AOD  ?_GammaCalo_8 ?_zip ?_GammaConvCalo -mergetrains -RL_listDPGTrackIncAccTPCandEMC -noDown $1

#  # 1686 	vAN-20210118_ROOT6-1 	LHC18_13TeV_pass1_AOD234 	2021 Jan 20, Train run: All jobs submitted 	13TeV AOD validation 	
#  # 1685 	vAN-20210118_ROOT6-1 	LHC17_13TeV_pass1_AOD234 	2021 Jan 20, Train run: All jobs submitted 	13TeV AOD validation 	
#  # 1684 	vAN-20210118_ROOT6-1 	LHC16_13TeV_pass1_2_AOD234 	2021 Jan 20, Train run finished 	13TeV AOD validation 	
#  bash DownScript.sh 1684 -Name_Data_AOD234_16 GA_pp_AOD ?_zip ?_GammaCalo_8 ?_GammaConvCalo -runwise -RL_listDPGEDC_OR_listDPGEMC -RL_listPHOS $1
#  bash DownScript.sh 1685 -Name_Data_AOD234_17 GA_pp_AOD ?_zip ?_GammaCalo_8 ?_GammaConvCalo -runwise -RL_listDPGEDC_OR_listDPGEMC -RL_listPHOS $1
#  bash DownScript.sh 1686 -Name_Data_AOD234_18 GA_pp_AOD ?_zip ?_GammaCalo_8 ?_GammaConvCalo -runwise -RL_listDPGEDC_OR_listDPGEMC -RL_listPHOS $1
#  bash DownScript.sh 1686 1685 1684 -Name_Data_AOD234 GA_pp_AOD ?_zip ?_GammaCalo_8 ?_GammaConvCalo -mergetrains -RL_listDPGEDC_OR_listDPGEMC -RL_listPHOS -noDown $1

#  # 1683 	vAN-20210118_ROOT6-1 	LHC18_13TeV_pass1_AOD208 	2021 Jan 20, Train run: All jobs submitted 	13TeV AOD validation 	
#  # 1682 	vAN-20210118_ROOT6-1 	LHC17_13TeV_pass1_AOD208 	2021 Jan 20, Train run: All jobs submitted 	13TeV AOD validation 	
#  # 1681 	vAN-20210118_ROOT6-1 	LHC16_13TeV_pass1_2_AOD208 	2021 Jan 20, Train run: All jobs submitted 	13TeV AOD validation 	
#  bash DownScript.sh 1683 1682 1681 GA_pp_AOD -pergeruns
#  bash DownScript.sh 1681 -Name_Data_AOD208_16 GA_pp_AOD ?_zip ?_GammaCalo_8 ?_GammaConvCalo -RL_listDPGEDC_OR_listDPGEMC -RL_listPHOS $1
#  bash DownScript.sh 1682 -Name_Data_AOD208_17 GA_pp_AOD ?_zip ?_GammaCalo_8 ?_GammaConvCalo -RL_listDPGEDC_OR_listDPGEMC -RL_listPHOS $1
#  bash DownScript.sh 1683 -Name_Data_AOD208_18 GA_pp_AOD ?_zip ?_GammaCalo_8 ?_GammaConvCalo -RL_listDPGEDC_OR_listDPGEMC -RL_listPHOS $1
#  bash DownScript.sh 1683 1682 1681 -Name_Data_AOD208 GA_pp_AOD ?_zip ?_GammaCalo_8 ?_GammaConvCalo -mergetrains -RL_listDPGEDC_OR_listDPGEMC -RL_listPHOS -noDown $1

#  # 3416 	vAN-20210118_ROOT6-1 	PYT8_13TeV_anchLHC18_AOD_extra 	2021 Jan 21, Train run finished 	13TeV AOD validation 	
#  # 3420 	vAN-20210118_ROOT6-1 	PYT8_13TeV_anchLHC18_AOD235 	2021 Jan 21, Train run finished 	13TeV AOD validation 	
#  # 3419 	vAN-20210118_ROOT6-1 	PYT8_13TeV_anchLHC17_AOD235 	2021 Jan 21, Train run finished 	13TeV AOD validation 	
#  # 3418 	vAN-20210118_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD235_extra 	2021 Jan 21, Train run finished 	13TeV AOD validation 	
# #  # 3417 	vAN-20210118_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD235 	2021 Jan 21, Train run finished 	13TeV AOD validation 	
#  bash DownScript.sh 3418 3417 -Name_MC_AOD235_16 GA_pp_MC_AOD ?_zip ?_GammaCalo_8 ?_GammaConvCalo -runwise -mergeruns -mergetrains -RL_listPHOS -RL_listDPGEDC_OR_listDPGEMC $1
#  bash DownScript.sh 3419 -Name_MC_AOD235_17 GA_pp_MC_AOD ?_zip ?_GammaCalo_8 ?_GammaConvCalo -runwise -RL_listPHOS -RL_listDPGEDC_OR_listDPGEMC $1
#  bash DownScript.sh 3420 -Name_MC_AOD235_18 GA_pp_MC_AOD ?_zip ?_GammaCalo_8 ?_GammaConvCalo -runwise -RL_listPHOS -RL_listDPGEDC_OR_listDPGEMC $1
#  bash DownScript.sh 3416 3420 -Name_MC_AOD235_18_extra GA_pp_MC_AOD -mergetrains ?_zip ?_GammaCalo_8 ?_GammaConvCalo -runwise -mergeruns -RL_listPHOS -RL_listDPGEDC_OR_listDPGEMC $1
#  bash DownScript.sh 3416 3420 3419 3418 3417 -Name_MC_AOD235 GA_pp_MC_AOD ?_zip ?_GammaCalo_8 ?_GammaConvCalo -mergetrains -RL_listPHOS -RL_listDPGEDC_OR_listDPGEMC -noDown $1

#  # 3416 	vAN-20210118_ROOT6-1 	PYT8_13TeV_anchLHC18_AOD_extra 	2021 Jan 21, Train run finished 	13TeV AOD validation 	
#  # 3415 	vAN-20210118_ROOT6-1 	PYT8_13TeV_anchLHC18_AOD209 	2021 Jan 21, Train run finished 	13TeV AOD validation 	
#  # 3414 	vAN-20210118_ROOT6-1 	PYT8_13TeV_anchLHC17_AOD209_extra 	2021 Jan 21, Train run finished 	13TeV AOD validation 	
#  # 3413 	vAN-20210118_ROOT6-1 	PYT8_13TeV_anchLHC17_AOD209 	2021 Jan 21, Train run finished 	13TeV AOD validation 	
#  # 3412 	vAN-20210118_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD209_extra2 	2021 Jan 21, Train run finished 	13TeV AOD validation 	
#  # 3411 	vAN-20210118_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD209_extra 	2021 Jan 21, Train run finished 	13TeV AOD validation 	
#  # 3410 	vAN-20210118_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD209 	2021 Jan 21, Train run finished 	13TeV AOD validation 
#  bash DownScript.sh 3416 3415 3414 3413 3412 3411 3410 GA_pp_MC_AOD -pergeruns
#  bash DownScript.sh 3412 3411 3410 -Name_MC_AOD209_16 GA_pp_MC_AOD -mergeperiods -mergetrains ?_zip ?_GammaCalo_8 ?_GammaConvCalo  -RL_listPHOS -RL_listDPGEDC_OR_listDPGEMC $1
#  bash DownScript.sh 3414 3413 -Name_MC_AOD209_17 GA_pp_MC_AOD -mergeperiods -mergetrains ?_zip ?_GammaCalo_8 ?_GammaConvCalo -RL_listPHOS  -RL_listDPGEDC_OR_listDPGEMC $1
#  bash DownScript.sh 3415 -Name_MC_AOD209_18 GA_pp_MC_AOD ?_zip ?_GammaCalo_8 ?_GammaConvCalo -RL_listPHOS  -RL_listDPGEDC_OR_listDPGEMC $1
#  bash DownScript.sh 3416 3415 -Name_MC_AOD209_18_extra GA_pp_MC_AOD -mergeperiods -mergetrains ?_zip ?_GammaCalo_8 ?_GammaConvCalo -RL_listPHOS  -RL_listDPGEDC_OR_listDPGEMC $1
#  bash DownScript.sh 3416 3415 3414 3413 3412 3411 3410 -Name_MC_AOD209 GA_pp_MC_AOD ?_zip ?_GammaCalo_8 ?_GammaConvCalo -mergetrains -RL_listPHOS -RL_listDPGEDC_OR_listDPGEMC -noDown $1

#  ###### redownload lowB

#  ### LHC16 MC
#  bash DownScript.sh 3348 GA_pp_MC_AOD ?_zip ?_GammaConvCalo -RL_DPGTrackIncAccTPCandEMC $1
#  ### LHC17 MC
#  bash DownScript.sh 3349 GA_pp_MC_AOD ?_zip ?_GammaConvCalo -RL_listDPGTrackIncAccTPCandEMC $1
#  ### LHC18 MC
#  bash DownScript.sh 3350 GA_pp_MC_AOD ?_zip ?_GammaConvCalo Child_1 Child_2 Child_4 Child_5 -RL_listDPGTrackIncAccTPCandEMC $1

#  ### LHC16 Data
#  bash DownScript.sh 1611 GA_pp_AOD ?_zip ?_GammaConvCalo -RL_listDPGTrackIncAccTPCandEMC $1
#  ### LHC17 Data
#  bash DownScript.sh 1612 GA_pp_AOD ?_zip ?_GammaConvCalo -RL_listDPGTrackIncAccTPCandEMC $1
#  ### LHC18 Data
#  bash DownScript.sh 1613 GA_pp_AOD ?_zip ?_GammaConvCalo Child_1 Child_2 -RL_listDPGTrackIncAccTPCandEMC $1


#  ### LHC16 MC
#  bash DownScript.sh 3216 GA_pp_MC_AOD ?_zip ?_GammaConvCalo -RL_listDPGTrackIncAccTPCandEMC $1
#  ### LHC17 MC
#  bash DownScript.sh 3217 GA_pp_MC_AOD ?_zip ?_GammaConvCalo -RL_listDPGTrackIncAccTPCandEMC $1
#  ### LHC18 MC
#  bash DownScript.sh 3218 GA_pp_MC_AOD ?_zip ?_GammaConvCalo Child_1 Child_2 Child_4 Child_5 -RL_listDPGTrackIncAccTPCandEMC $1

#  ################## Data #############################
#  ### LHC16 Data
#  bash DownScript.sh 1564 GA_pp_AOD ?_zip ?_GammaConvCalo -RL_listDPGTrackIncAccTPCandEMC $1
#  ### LHC17 Data
#  bash DownScript.sh 1565 GA_pp_AOD ?_zip ?_GammaConvCalo -RL_listDPGTrackIncAccTPCandEMC $1
#  ### LHC18 Data
#  bash DownScript.sh 1566 GA_pp_AOD ?_zip ?_GammaConvCalo Child_1 Child_2 -RL_listDPGTrackIncAccTPCandEMC $1
#  ################## MC #############################
#  ##### low B
#  ### LHC16 MC
#  bash DownScript.sh 3114 GA_pp_MC_AOD ?_zip ?_GammaConvCalo -RL_listDPGTrackIncAccTPCandEMC $1
#  ### LHC17 MC
#  bash DownScript.sh 3115 GA_pp_MC_AOD ?_zip ?_GammaConvCalo -RL_listDPGTrackIncAccTPCandEMC $1
#  ### LHC18 MC
#  bash DownScript.sh 3116 GA_pp_MC_AOD ?_zip ?_GammaConvCalo Child_1 Child_2 Child_4 Child_5 -RL_listDPGTrackIncAccTPCandEMC $1

#  ################# merging #############################
#  bash DownScript.sh 3350 3349 3348 GA_pp_MC_AOD ?_zip ?_GammaConvCalo -RL_listDPGTrackIncAccTPCandEMC $1
#  bash DownScript.sh 3350 3349 3348 -Name_MC_lowB_v17 GA_pp_MC_AOD ?_zip ?_GammaConvCalo -mergetrains -RL_listDPGTrackIncAccTPCandEMC -noDown $1
#  ################ 
#  bash DownScript.sh 1613 1612 1611 GA_pp_AOD ?_zip ?_GammaConvCalo -RL_listDPGTrackIncAccTPCandEMC $1
#  bash DownScript.sh 1613 1612 1611 -Name_Data_lowB_v16 GA_pp_AOD ?_zip ?_GammaConvCalo -mergetrains -RL_listDPGTrackIncAccTPCandEMC -noDown $1
#  ################ 
#  bash DownScript.sh 3218 3217 3216 GA_pp_MC_AOD ?_zip ?_GammaConvCalo -RL_listDPGTrackIncAccTPCandEMC $1
#  bash DownScript.sh 3218 3217 3216 -Name_MC_lowB_v16 GA_pp_MC_AOD ?_zip ?_GammaConvCalo -mergetrains -RL_listDPGTrackIncAccTPCandEMC -noDown $1
#  ################# 
#  bash DownScript.sh 1564 1565 1566 GA_pp_AOD ?_zip ?_GammaConvCalo -RL_listDPGTrackIncAccTPCandEMC $1
#  bash DownScript.sh 1564 1565 1566 -Name_Data_lowB_v15 GA_pp_AOD ?_zip ?_GammaConvCalo -mergetrains -RL_listDPGTrackIncAccTPCandEMC -noDown $1
#  ################# 
#  bash DownScript.sh 3116 3115 3114 GA_pp_MC_AOD ?_zip ?_GammaConvCalo -RL_listDPGTrackIncAccTPCandEMC $1
#  bash DownScript.sh 3116 3115 3114 -Name_MC_lowB_v15 GA_pp_MC_AOD ?_zip ?_GammaConvCalo -mergetrains -RL_listDPGTrackIncAccTPCandEMC -noDown $1

#  bash DownScript.sh 1697 1612 1611 -Name_Data_lowB_v19_MBW GA_pp_AOD ?_zip ?_GammaConvCalo_5 -mergetrains -RL_listDPGTrackIncAccTPCandEMC -noDown $1


#  # Variations
#  # 1664 	vAN-20201221_ROOT6-1 	LHC18_13TeV_pass1_AOD208 	2020 Dec 23, Train run finished 	request Adrian & Jens 	
#  # 1663 	vAN-20201221_ROOT6-1 	LHC17_13TeV_pass1_AOD208 	2020 Dec 23, Train run: All jobs submitted 	request Adrian & Jens 	
#  # 1662 	vAN-20201221_ROOT6-1 	LHC16_13TeV_pass1_2_AOD208 	2020 Dec 23, Train run: All jobs submitted 	request Adrian & Jens 	
# bash DownScript.sh 1664 1663 1662 GA_pp_AOD ?_zip ?_GammaConvCalo -RL_listPCMEDC -norunwise $1
# bash DownScript.sh 1664 1663 1662 GA_pp_AOD ?_GammaConvCalo_2003 -RL_listEDC_trigger_OR_listDPGEDCtrigger $1
# bash DownScript.sh 1664 1663 1662 -Name_Data_pp13TeV_v18 GA_pp_AOD ?_GammaConvCalo -mergetrains -RL_listPCMEDC -RL_listDPGEDCtrigger -noDown $1



#  # 3367 	vAN-20201221_ROOT6-1 	PYT8_13TeV_anchLHC18_AOD_extra 	2020 Dec 23, Train run finished 	request Adrian 	
#  # 3366 	vAN-20201221_ROOT6-1 	PYT8_13TeV_anchLHC18_AOD209 	2020 Dec 23, Train run finished 	request Adrian 	
#  # 3365 	vAN-20201221_ROOT6-1 	PYT8_13TeV_anchLHC17_AOD209_extra 	2020 Dec 23, Train run: All jobs submitted 	request Adrian 	
#  # 3364 	vAN-20201221_ROOT6-1 	PYT8_13TeV_anchLHC17_AOD209 	2020 Dec 23, Train run: All jobs submitted 	request Adrian 	
#  # 3363 	vAN-20201221_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD209_extra2 	2020 Dec 23, Train run: All jobs submitted 	request Adrian 	
#  # 3362 	vAN-20201221_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD209_extra 	2020 Dec 23, Train run: All jobs submitted 	request Adrian 	
#  # 3361 	vAN-20201221_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD209 	2020 Dec 23, Train run: All jobs submitted 	request Adrian 


#  bash DownScript.sh 3367 3366 3365 3364 3363 3362 3361 GA_pp_MC_AOD ?_zip ?_GammaConvCalo -RL_listPCMEDC 
#  bash DownScript.sh 3367 3366 3365 3364 3363 3362 3361 -Name_MC_pp13TeV_v18 GA_pp_MC_AOD ?_zip ?_GammaConvCalo -mergetrains -RL_listPCMEDC -noDown

#  # 1657 	vAN-20201217_ROOT6-1 	LHC18_13TeV_pass1_AOD208 	2020 Dec 18, Train run finished 	request Adrian, Joshua
#  # 1656 	vAN-20201217_ROOT6-1 	LHC17_13TeV_pass1_AOD208 	2020 Dec 18, Train run finished 	request Adrian, Joshua
#  # 1655 	vAN-20201217_ROOT6-1 	LHC16_13TeV_pass1_2_AOD208 	2020 Dec 18, Train run finished 	request Adrian, Joshua
#  ### LHC16 Data
#  bash DownScript.sh 1655 GA_pp_AOD ?_zip ?_GammaConvCalo -RL_listPCMEDC -RL_listDPGEDCtrigger -RL_listPHOS $1
#  ### LHC17 Data
#  bash DownScript.sh 1656 GA_pp_AOD ?_zip ?_GammaConvCalo -RL_listPCMEDC -RL_listDPGEDCtrigger -RL_listPHOS $1
#  ### LHC18 Data
#  bash DownScript.sh 1657 GA_pp_AOD ?_zip ?_GammaConvCalo -RL_listPCMEDC -RL_listEDC_trigger -RL_listDPGEDCtrigger -RL_listPHOS $1
#  ln -sf "$gridpath/.GA_pp_AOD-1657/listEDC_trigger" "$gridpath/.GA_pp_AOD-1657/listDPGEDCtrigger"
#  ################# merging #############################
#  ### Data 16 + 17 + 18
#  bash DownScript.sh 1657 1656 1655 -Name_Data_pp13TeV_v17 GA_pp_AOD ?_zip ?_GammaConvCalo -mergetrains -RL_listPCMEDC -RL_listPHOS -RL_listDPGEDCtrigger -noDown $1


#  # 3347 	vAN-20201217_ROOT6-1 	PYT8_13TeV_anchLHC18_AOD_extra 	2020 Dec 18, Train run finished 	request Adrian, Joshua
#  # 3346 	vAN-20201217_ROOT6-1 	PYT8_13TeV_anchLHC18_AOD209 	2020 Dec 18, Train run finished 	request Adrian, Joshua
#  # 3345 	vAN-20201217_ROOT6-1 	PYT8_13TeV_anchLHC17_AOD209_extra 	2020 Dec 18, Train run finished 	request Adrian, Joshua
#  # 3344 	vAN-20201217_ROOT6-1 	PYT8_13TeV_anchLHC17_AOD209 	2020 Dec 18, Train run finished 	request Adrian, Joshua
#  # 3343 	vAN-20201217_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD209_extra2 	2020 Dec 18, Train run finished 	request Adrian, Joshua
#  # 3342 	vAN-20201217_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD209_extra 	2020 Dec 18, Train run finished 	request Adrian, Joshua
#  # 3341 	vAN-20201217_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD209 	2020 Dec 18, Train run finished 	request Adrian, Joshua
#  ### LHC16 MC
#  bash DownScript.sh 3343 3342 3341 GA_pp_MC_AOD ?_zip ?_GammaConvCalo -mergetrains -RL_listPCMEDC -RL_listPHOS $1
#  ### LHC17 MC
#  bash DownScript.sh 3345 3344 GA_pp_MC_AOD ?_zip ?_GammaConvCalo -mergetrains -RL_listPCMEDC -RL_listPHOS $1
#  ### LHC18 MC
#  bash DownScript.sh 3347 3346 GA_pp_MC_AOD ?_zip ?_GammaConvCalo -mergetrains -RL_listPCMEDC -RL_listPHOS $1
#  ################ merging #############################
#  bash DownScript.sh 3347 3346 3345 3344 3343 3342 3341 -Name_MC_pp13TeV_v17 GA_pp_MC_AOD ?_zip ?_GammaConvCalo -mergetrains -RL_listPCMEDC -RL_listPHOS -noDown $1



#  # bash getTraintests.sh ?_zip ?_GammaConvCalo GA_pp_AOD 1613 1612 1611
#  # 1613 	vAN-20201119_ROOT6-1 	LHC18c_13TeV_LowB_pass1_AOD212 	2020 Nov 20, Test finished (29m 39s total time) 	request Adrian
#  # 1612 	vAN-20201119_ROOT6-1 	LHC17g_13TeV_LowB_pass1_AOD212 	2020 Nov 20, Test finished (26m 10s total time) 	request Adrian
#  # 1611 	vAN-20201119_ROOT6-1 	LHC16f_13TeV_LowB_pass1_AOD212 	2020 Nov 20, Test finished (17m 32s total time) 	request Adrian
#  ### LHC16 Data
#  bash DownScript.sh 1611 GA_pp_AOD ?_zip ?_GammaConvCalo -RL_listDPGTrackIncAccTPCandEMC $1
#  ### LHC17 Data
#  bash DownScript.sh 1612 GA_pp_AOD ?_zip ?_GammaConvCalo -RL_listDPGTrackIncAccTPCandEMC $1
#  ### LHC18 Data
#  bash DownScript.sh 1613 GA_pp_AOD ?_zip ?_GammaConvCalo Child_1 Child_2 -RL_listDPGTrackIncAccTPCandEMC $1

#  # bash getTraintests.sh ?_zip ?_GammaConvCalo GA_pp_MC_AOD 3216 3217 3218
#  # 3218 	vAN-20201119_ROOT6-1 	LHC18h1_PYT8_13TeV_ancLHC18c_LowB_AOD213 	2020 Nov 20, Test finished (1:07 total time) 	request Adrian
#  # 3217 	vAN-20201119_ROOT6-1 	LHC17h3_PYT8_13TeV_anchLHC17g_LowB_AOD213 	2020 Nov 20, Test finished (23m 56s total time) 	request Adrian
#  # 3216 	vAN-20201119_ROOT6-1 	LHC17d1_PYT8_13TeV_ancLHC16f_LowB_AOD213 	2020 Nov 20, Test finished (59m 11s total time) 	request Adrian
#  ### LHC16 MC
#  bash DownScript.sh 3216 GA_pp_MC_AOD ?_zip ?_GammaConvCalo -RL_listDPGTrackIncAccTPCandEMC $1
#  ### LHC17 MC
#  bash DownScript.sh 3217 GA_pp_MC_AOD ?_zip ?_GammaConvCalo -RL_listDPGTrackIncAccTPCandEMC $1
#  ### LHC18 MC
#  bash DownScript.sh 3218 GA_pp_MC_AOD ?_zip ?_GammaConvCalo -mergetrains Child_1 Child_2 -RL_listDPGTrackIncAccTPCandEMC $1
#  ################# merging #############################
#  ### Data lowB
#  bash DownScript.sh 1613 1612 1611 -Name_Data_pp13TeV_lowB_v16 GA_pp_AOD ?_zip ?_GammaConvCalo -mergetrains -RL_listDPGTrackIncAccTPCandEMC -noDown $1
#  ### MC lowB
#  bash DownScript.sh 3218 3217 3216 -Name_MC_pp13TeV_lowB_v16 GA_pp_MC_AOD ?_zip ?_GammaConvCalo -mergetrains -RL_listDPGTrackIncAccTPCandEMC -noDown $1





#  # bash getTraintests.sh ?_zip ?_GammaConvCalo GA_pp_AOD 1616 1615 1614
#  # 1616 	vAN-20201119_ROOT6-1 	LHC18_13TeV_pass1_AOD208 	2020 Nov 20, Test started 1:45 ago 	reuest Adrian
#  # 1615 	vAN-20201119_ROOT6-1 	LHC17_13TeV_pass1_AOD208 	2020 Nov 20, Test started 1:48 ago 	reuest Adrian
#  # 1614 	vAN-20201119_ROOT6-1 	LHC16_13TeV_pass1_2_AOD208 	2020 Nov 20, Test finished (1:14 total time) 	reuest Adrian
#  ### LHC16 Data
#  bash DownScript.sh 1614 -Name_LHC16Data_v16 GA_pp_AOD ?_zip ?_GammaCalo -RL_listPHOS $1
#  bash DownScript.sh 1614 -Name_LHC16Data_v16 GA_pp_AOD ?_zip ?_GammaConvCalo -RL_listPCMEDC -RL_listDPGEDCtrigger $1
#  ### LHC17 Data
#  bash DownScript.sh 1615 -Name_LHC17Data_v16 GA_pp_AOD ?_zip ?_GammaConvCalo -RL_listPCMEDC -RL_listDPGEDCtrigger $1
#  bash DownScript.sh 1615 -Name_LHC17Data_v16 GA_pp_AOD ?_zip ?_GammaCalo -RL_listPHOS $1
#  ### LHC18 Data
#  bash DownScript.sh 1616 -Name_LHC18Data_v16 GA_pp_AOD ?_zip ?_GammaConvCalo -RL_listPCMEDC -RL_listEDC_trigger $1
#  bash DownScript.sh 1616 -Name_LHC18Data_v16 GA_pp_AOD ?_zip ?_GammaCalo -RL_listPHOS $1
#  ln -sf "$gridpath/LHC18Data_v16/listEDC_trigger" "$gridpath/LHC18Data_v16/listDPGEDCtrigger"
#  ln -sf "$gridpath/.GA_pp_AOD-1616/listEDC_trigger" "$gridpath/.GA_pp_AOD-1616/listDPGEDCtrigger"

#  # bash getTraintests.sh ?_zip ?_GammaConvCalo GA_pp_MC_AOD 3225 3224 3223 3222 3221 3220 3219
#  # 3225 	vAN-20201119_ROOT6-1 	PYT8_13TeV_anchLHC18_AOD_extra 	2020 Nov 20, Test finished (17m 9s total time) 	request Adrian
#  # 3224 	vAN-20201119_ROOT6-1 	PYT8_13TeV_anchLHC18_AOD209 	2020 Nov 20, Test finished (8m 36s total time) 	request Adrian
#  # 3223 	vAN-20201119_ROOT6-1 	PYT8_13TeV_anchLHC17_AOD209_extra 	2020 Nov 20, Test finished (13m 46s total time) 	request Adrian
#  # 3222 	vAN-20201119_ROOT6-1 	PYT8_13TeV_anchLHC17_AOD209 	2020 Nov 20, Test finished (11m 28s total time) 	request Adrian
#  # 3221 	vAN-20201119_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD209_extra2 	2020 Nov 20, Test finished (8m 30s total time) 	request Adrian
#  # 3220 	vAN-20201119_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD209_extra 	2020 Nov 20, Test finished (20m 36s total time) 	request Adrian
#  # 3219 	vAN-20201119_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD209 	2020 Nov 20, Test finished (7m 27s total time) 	request Adrian, Marvin
#  ### LHC16 MC
#  bash DownScript.sh 3221 3220 3219 -Name_LHC16MC_v16 GA_pp_MC_AOD ?_zip ?_GammaConvCalo -mergetrains -RL_listPCMEDC $1
#  bash DownScript.sh 3221 3220 3219 -Name_LHC16MC_v16 GA_pp_MC_AOD ?_zip ?_GammaCalo -mergetrains -RL_listPHOS $1
#  ### LHC17 MC
#  bash DownScript.sh 3223 3222 -Name_LHC17MC_v16 GA_pp_MC_AOD ?_zip ?_GammaConvCalo -mergetrains -RL_listPCMEDC $1
#  bash DownScript.sh 3223 3222 -Name_LHC17MC_v16 GA_pp_MC_AOD ?_zip ?_GammaCalo -mergetrains -RL_listPHOS $1
#  ### LHC18 MC
#  bash DownScript.sh 3225 3224 -Name_LHC18MC_v16 GA_pp_MC_AOD ?_zip ?_GammaConvCalo -mergetrains -RL_listPCMEDC $1
#  bash DownScript.sh 3225 3224 -Name_LHC18MC_v16 GA_pp_MC_AOD ?_zip ?_GammaCalo -mergetrains -RL_listPHOS $1

#  ################# merging #############################
#  ### Data 16 + 17 + 18
#  bash DownScript.sh 1616 1615 1614 -Name_Data_pp13TeV_v16 GA_pp_AOD ?_zip ?_GammaConvCalo -mergetrains -RL_listPCMEDC -RL_listDPGEDCtrigger -noDown $1
#  bash DownScript.sh 1616 1615 1614 -Name_Data_pp13TeV_v16 GA_pp_AOD ?_zip ?_GammaCalo -mergetrains -RL_listPHOS -noDown $1
#  ### MC 16 + 17 + 18
#  bash DownScript.sh 3225 3224 3223 3222 3221 3220 3219 -Name_MC_pp13TeV_v16 GA_pp_MC_AOD ?_zip ?_GammaConvCalo -mergetrains -RL_listPCMEDC -noDown $1
#  bash DownScript.sh 3225 3224 3223 3222 3221 3220 3219 -Name_MC_pp13TeV_v16 GA_pp_MC_AOD ?_zip ?_GammaCalo -mergetrains -RL_listPHOS -noDown $1



#  ############ NL, NCell, Rbins v15 ##############
#  ############### Data #########################
#  # 1566 	vAN-20201004_ROOT6-1 	LHC18c_13TeV_LowB_pass1_AOD212 	2020 Oct 06, Train run: All jobs submitted 	request Adrian, Ana
#  # 1565 	vAN-20201004_ROOT6-1 	LHC17g_13TeV_LowB_pass1_AOD212 	2020 Oct 06, Train run: All jobs submitted 	request Adrian, Ana
#  # 1564 	vAN-20201004_ROOT6-1 	LHC16f_13TeV_LowB_pass1_AOD212 	2020 Oct 06, Train run: All jobs submitted 	request Adrian, Ana
#  ##### low B
#  ################## Data #############################
#  ### LHC16 Data
#  bash DownScript.sh 1564 GA_pp_AOD ?_zip ?_GammaConvCalo -RL_listDPGTrackIncAccTPCandEMC $1
#  ### LHC17 Data
#  bash DownScript.sh 1565 GA_pp_AOD ?_zip ?_GammaConvCalo -RL_listDPGTrackIncAccTPCandEMC $1
#  ### LHC18 Data
#  bash DownScript.sh 1566 GA_pp_AOD ?_zip ?_GammaConvCalo Child_1 Child_2 -RL_listDPGTrackIncAccTPCandEMC $1
#  ################## MC #############################
#  ##### low B
#  # 3116 	vAN-20201004_ROOT6-1 	LHC18h1_PYT8_13TeV_ancLHC18c_LowB_AOD213 	2020 Oct 17, Train run: All jobs submitted 	request Adrian, Ana
#  # 3115 	vAN-20201004_ROOT6-1 	LHC17h3_PYT8_13TeV_anchLHC17g_LowB_AOD213 	2020 Oct 17, Train run finished 	request Adrian, Ana
#  # 3114 	vAN-20201004_ROOT6-1 	LHC17d1_PYT8_13TeV_ancLHC16f_LowB_AOD213 	2020 Oct 17, Train run finished 	request Adrian, Ana
#  ### LHC16 MC
#  bash DownScript.sh 3114 GA_pp_MC_AOD ?_zip ?_GammaConvCalo -RL_listDPGTrackIncAccTPCandEMC $1
#  ### LHC17 MC
#  bash DownScript.sh 3115 GA_pp_MC_AOD ?_zip ?_GammaConvCalo -RL_listDPGTrackIncAccTPCandEMC $1
#  ### LHC18 MC
#  bash DownScript.sh 3116 GA_pp_MC_AOD ?_zip ?_GammaConvCalo -mergetrains Child_1 Child_2 Child_4 Child_5 -RL_listDPGTrackIncAccTPCandEMC $1
#  ################## merging #############################
#  ### Data 16 + 17 + 18
#  bash DownScript.sh 1564 1565 1566 -Name_Data_pp13TeV_lowB_v15 GA_pp_AOD ?_zip ?_GammaConvCalo -mergetrains -RL_listDPGTrackIncAccTPCandEMC -noDown $1
#  ### MC 16 + 17 + 18
#  bash DownScript.sh 3116 3115 3114 -Name_MC_pp13TeV_lowB_v15 GA_pp_MC_AOD ?_zip ?_GammaConvCalo -mergetrains -RL_listDPGTrackIncAccTPCandEMC -noDown $1



#  ##### norm B
#  # 1563 	vAN-20201005_ROOT6-1 	LHC18_13TeV_pass1_AOD208 	2020 Oct 06, Train run: All jobs submitted 	request Ana, Adrian, Jens
#  # 1562 	vAN-20201005_ROOT6-1 	LHC17_13TeV_pass1_AOD208 	2020 Oct 06, Train run: All jobs submitted 	request Ana, Adrian, Jens
#  # 1561 	vAN-20201005_ROOT6-1 	LHC16_13TeV_pass1_2_AOD208 	2020 Oct 06, Train run: All jobs submitted 	request Ana, Adrian, Jens
#  ### LHC16 Data
#  bash DownScript.sh 1561 -Name_LHC16Data_v15 GA_pp_AOD ?_zip ?_GammaConvCalo -RL_listPCMEDC -RL_listDPGEDCtrigger $1
#  ### LHC17 Data
#  bash DownScript.sh 1562 -Name_LHC17Data_v15 GA_pp_AOD ?_zip ?_GammaConvCalo -RL_listPCMEDC -RL_listDPGEDCtrigger $1
#  ### LHC18 Data
#  bash DownScript.sh 1563 -Name_LHC18Data_v15 GA_pp_AOD ?_zip ?_GammaConvCalo -RL_listPCMEDC -RL_listEDC_trigger $1
#  ln -sf "$gridpath/LHC18Data_v15/listEDC_trigger" "$gridpath/LHC18Data_v15/listDPGEDCtrigger"
#  ln -sf "$gridpath/.GA_pp_AOD-1563/listEDC_trigger" "$gridpath/.GA_pp_AOD-1563/listDPGEDCtrigger"
#  ##### norm B
#  # 3113 	vAN-20201015_ROOT6-1 	PYT8_13TeV_anchLHC18_AOD_extra 	2020 Oct 16, Train run finished 	request Adrian, Steven
#  # 3112 	vAN-20201015_ROOT6-1 	PYT8_13TeV_anchLHC18_AOD209 	2020 Oct 16, Train run finished 	request Adrian, Steven
#  # 3111 	vAN-20201015_ROOT6-1 	PYT8_13TeV_anchLHC17_AOD209_extra 	2020 Oct 16, Train run finished 	request Adrian, Steven
#  # 3110 	vAN-20201015_ROOT6-1 	PYT8_13TeV_anchLHC17_AOD209 	2020 Oct 16, Train run finished 	request Adrian, Steven
#  # 3109 	vAN-20201015_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD209_extra2 	2020 Oct 16, Train run finished 	request Adrian, Steven
#  # 3108 	vAN-20201015_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD209_extra 	2020 Oct 16, Train run finished 	request Adrian, Steven
#  # 3107 	vAN-20201015_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD209 	2020 Oct 16, Train run finished 	request Adrian, Steven
#  ### LHC16 MC
#  bash DownScript.sh 3109 3108 3107 -Name_LHC16MC_v15 GA_pp_MC_AOD ?_zip ?_GammaConvCalo -mergetrains -RL_listPCMEDC $1
#  ### LHC17 MC
#  bash DownScript.sh 3110 3111 -Name_LHC17MC_v15 GA_pp_MC_AOD ?_zip ?_GammaConvCalo -mergetrains -RL_listPCMEDC $1
#  ### LHC18 MC
#  bash DownScript.sh 3112 3113 -Name_LHC18MC_v15 GA_pp_MC_AOD ?_zip ?_GammaConvCalo -mergetrains -RL_listPCMEDC $1
#  ################# merging #############################
#  ### Data 16 + 17 + 18
#  bash DownScript.sh 1563 1562 1561 -Name_Data_pp13TeV_v15 GA_pp_AOD ?_zip ?_GammaConvCalo -mergetrains -RL_listPCMEDC -RL_listDPGEDCtrigger -noDown $1
#  ### MC 16 + 17 + 18
#  bash DownScript.sh 3113 3112 3111 3110 3109 3108 3107 -Name_MC_pp13TeV_v15 GA_pp_MC_AOD ?_zip ?_GammaConvCalo -mergetrains -RL_listPCMEDC -RL_listDPGEDCtrigger -noDown $1




#  ########### NL, NCell, Rbins v14 + QA ##############
#  ############## Data #########################

#  # 1551 	vAN-20200924_ROOT6-1 	LHC18_13TeV_pass1_AOD208 	2020 Sep 25, Train run: All jobs submitted 	request Adrian
#  # 1550 	vAN-20200924_ROOT6-1 	LHC17_13TeV_pass1_AOD208 	2020 Sep 25, Train run: All jobs submitted 	request Adrian
#  # 1549 	vAN-20200924_ROOT6-1 	LHC16_13TeV_pass1_2_AOD208 	2020 Sep 25, Train run: All jobs submitted 	request Adrian
#  ### LHC16 Data
#  bash DownScript.sh 1549 -Name_LHC16Data_v14 GA_pp_AOD ?_zip ?_GammaConvCalo_20 -RL_listPCMEDC -RL_listDPGEDCtrigger $1
#  ### LHC17 Data
#  bash DownScript.sh 1550 -Name_LHC17Data_v14 GA_pp_AOD ?_zip ?_GammaConvCalo_20 -RL_listPCMEDC -RL_listDPGEDCtrigger $1
#  ### LHC18 Data
#  bash DownScript.sh 1551 -Name_LHC18Data_v14 GA_pp_AOD ?_zip ?_GammaConvCalo_20 -RL_listPCMEDC -RL_listEDC_trigger $1
#  ln -sf "$gridpath/LHC18Data_v14/listEDC_trigger" "$gridpath/LHC18Data_v14/listDPGEDCtrigger"
#  ln -sf "$gridpath/.GA_pp_AOD-1551/listEDC_trigger" "$gridpath/.GA_pp_AOD-1551/listDPGEDCtrigger"
#  ################## MC #############################
#  # 3034 	vAN-20200924_ROOT6-1 	PYT8_13TeV_anchLHC18_AOD_extra 	2020 Sep 25, Train run finished 	request Adrian
#  # 3033 	vAN-20200924_ROOT6-1 	PYT8_13TeV_anchLHC18_AOD209 	2020 Sep 25, Train run finished 	request Adrian
#  # 3032 	vAN-20200924_ROOT6-1 	PYT8_13TeV_anchLHC17_AOD209_extra 	2020 Sep 25, Train run: All jobs submitted 	request Adrian
#  # 3031 	vAN-20200924_ROOT6-1 	PYT8_13TeV_anchLHC17_AOD209 	2020 Sep 25, Train run finished 	request Adrian
#  # 3030 	vAN-20200924_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD209_extra2 	2020 Sep 25, Train run finished 	request Adrian
#  # 3029 	vAN-20200924_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD209_extra 	2020 Sep 25, Train run finished 	request Adrian
#  # 3028 	vAN-20200924_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD209 	2020 Sep 25, Train run finished 	request Adrian
#  ### LHC16 MC
#  bash DownScript.sh 3030 3029 3028 -Name_LHC16MC_v14 GA_pp_MC_AOD ?_zip ?_GammaConvCalo_20 -mergetrains -RL_listPCMEDC $1
#  ### LHC17 MC
#  bash DownScript.sh 3032 3031 -Name_LHC17MC_v14 GA_pp_MC_AOD ?_zip ?_GammaConvCalo_20 -mergetrains -RL_listPCMEDC $1
#  ### LHC18 MC
#  bash DownScript.sh 3034 3033 -Name_LHC18MC_v14 GA_pp_MC_AOD ?_zip ?_GammaConvCalo_20 -mergetrains -RL_listPCMEDC $1
#  ################# merging #############################
#  ### Data 16 + 17 + 18
#  bash DownScript.sh 1551 1550 1549 -Name_Data_pp13TeV_v14 GA_pp_AOD ?_zip ?_GammaConvCalo ?_zip ?_GammaCaloMix -mergetrains -RL_listPCMEDC -RL_listDPGEDCtrigger -noDown $1
#  ### MC 16 + 17 + 18
#  bash DownScript.sh 3034 3033 3032 3031 3030 3029 3028 -Name_MC_pp13TeV_v14 GA_pp_MC_AOD ?_zip ?_GammaConvCalo ?_zip ?_GammaCaloMix -mergetrains -RL_listPCMEDC -noDown $1



#  # 3262 	vAN-20201126_ROOT6-1 	PYT8JJ_13TeV_anchLHC18_AOD 	2020 Nov 27, Test started 58s ago 	request Adrian
#  # 3261 	vAN-20201126_ROOT6-1 	PYT8JJ_13TeV_anchLHC17_AOD 	2020 Nov 27, Test started 1m 22s ago 	request Adrian
#  # 3260 	vAN-20201126_ROOT6-1 	PYT8JJ_13TeV_anchLHC16_AOD209 	2020 Nov 27, Test started 1m 55s ago 	request Adrian
#  # bash getTraintests.sh ?_zip ?_GammaConvCalo GA_pp_MC_AOD 3262 3261 3260
 ############## JJ MC #####################  PHOS
#  ### LHC18 MC
#  bash DownScript.sh 3262 -Name_JJLHC18MC_v16 GA_pp_MC_AOD ?_zip ?_Gamma -mergetrains -IsJJ -RL_list_Joshua $1
#  ################ merging #############################
#  bash DownScript.sh 3262 3261 3260 GA_pp_MC_AOD ?_zip ?_Gamma -mergetrains -IsJJ -norunwise -RL_list_Joshua $1
#  bash DownScript.sh 3262 3261 3260 -Name_JJMC_pp13TeV_v17 GA_pp_MC_AOD ?_zip ?_Gamma -mergetrains -RL_list_Joshua -noDown $1


#  # 3228 	vAN-20201119_ROOT6-1 	PYT8JJ_13TeV_anchLHC18_AOD 	2020 Nov 26, Train run: All jobs submitted 	request Adrian
#  # 3227 	vAN-20201119_ROOT6-1 	PYT8JJ_13TeV_anchLHC17_AOD 	2020 Nov 26, Train run: All jobs submitted 	request Adrian
#  # 3226 	vAN-20201119_ROOT6-1 	PYT8JJ_13TeV_anchLHC16_AOD209 	2020 Nov 26, Train run: All jobs submitted 	request Adrian, Marvin
#  # ############## JJ MC #############################
#  ### LHC18 MC
#  bash DownScript.sh 3228 -Name_JJLHC18MC_v16 GA_pp_MC_AOD ?_zip ?_Gamma -mergetrains -IsJJ -RL_list_Joshua $1
#  ################# merging #############################
#  bash DownScript.sh 3228 3227 3226 GA_pp_MC_AOD ?_zip ?_Gamma -mergetrains -IsJJ -norunwise -RL_list_Joshua $1
#  bash DownScript.sh 3228 3227 3226 -Name_JJMC_pp13TeV_v16 GA_pp_MC_AOD ?_zip ?_Gamma -mergetrains -RL_list_Joshua -noDown $1


#  bash DownScript.sh 3262 3261 3260 -Name_JJMC_pp13TeV_v16 GA_pp_MC_AOD ?_zip ?_Gamma -mergetrains -RL_default -noDown $1

#  ### JJ Photon QA
 # bash DownScript.sh 3260 -Name_LHC16JJMC_PhotonQA GA_pp_AOD ?_zip ?_AnalysisResults -onlyrunwise -IsJJ -runwise $1
 # bash DownScript.sh 3261 -Name_LHC17JJMC_PhotonQA GA_pp_AOD ?_zip ?_AnalysisResults -onlyrunwise -IsJJ -runwise $1
 # bash DownScript.sh 3262 -Name_LHC18JJMC_PhotonQA GA_pp_AOD ?_zip ?_AnalysisResults -onlyrunwise -IsJJ -runwise $1

#  # 3119 	vAN-20201006_ROOT6-1 	PYT8JJ_13TeV_anchLHC18_AOD 	2020 Oct 17, Train run finished 	request Adrian
#  # 3118 	vAN-20201006_ROOT6-1 	PYT8JJ_13TeV_anchLHC17_AOD 	2020 Oct 17, Train run finished 	request Adrian
#  # 3117 	vAN-20201006_ROOT6-1 	PYT8JJ_13TeV_anchLHC16_AOD209 	2020 Oct 17, Train run finished 	request Adrian
#  ############## JJ MC #############################
#  ### LHC16 MC
#  bash DownScript.sh 3117 -Name_JJLHC16MC_v15 GA_pp_MC_AOD ?_zip ?_GammaConvCalo -mergetrains -IsJJ $1
# #  ### LHC17 MC
#  bash DownScript.sh 3118 -Name_JJLHC17MC_v15 GA_pp_MC_AOD ?_zip ?_GammaConvCalo -mergetrains -IsJJ $1
# #  ### LHC18 MC
#  bash DownScript.sh 3119 -Name_JJLHC18MC_v15 GA_pp_MC_AOD ?_zip ?_GammaConvCalo -mergetrains -IsJJ $1
#  ################# merging #############################
#  bash DownScript.sh 3119 3118 3117 -Name_JJMC_pp13TeV_v15 GA_pp_MC_AOD ?_zip ?_GammaConvCalo -mergetrains -RL_default -noDown $1





#  #################### QA #############################
#  ### LHC16 Data
#  bash DownScript.sh 1549 -Name_LHC16Data_v14 GA_pp_AOD ?_zip ?_GammaConvCalo_2001 -onlyrunwise -RL_listDPGEDC -RL_listDPG -RL_listDPGIncAcc -RL_listPCMEDC $1
#  # ln -sf "$gridpath/LHC16Data_v14/listDPGIncAcc" "$gridpath/LHC16Data_v14/listDPGIncTPC"
#  ### LHC17 Data
#  bash DownScript.sh 1550 -Name_LHC17Data_v14 GA_pp_AOD ?_zip ?_GammaConvCalo_2001 -onlyrunwise -RL_listDPGEDC -RL_listDPG -RL_listDPGIncTPC -RL_listPCMEDC $1
#  ### LHC18 Datas
#  bash DownScript.sh 1551 -Name_LHC18Data_v14 GA_pp_AOD ?_zip ?_GammaConvCalo_2001 -onlyrunwise -RL_listDPGEMC -RL_listDPG -RL_listDPGIncTPC $1
#  # ln -sf "$gridpath/LHC18Data_v14/listDPGEMC" "$gridpath/LHC18Data_v14/listDPGEDC"
#  # ln -sf "$gridpath/.GA_pp_AOD-1551/listDPGEMC" "$gridpath/.GA_pp_AOD-1551/listDPGEDC"
#  ### LHC16 MC
#  bash DownScript.sh 3030 3029 3028 -Name_LHC16MC_v14 GA_pp_MC_AOD ?_zip ?_GammaConvCalo_2001 -onlyrunwise -mergeruns -RL_listDPGEDC -RL_listDPG -RL_listDPGIncAccTPC -RL_listPCMEDC $1
#  # ln -sf "$gridpath/LHC16MC_v14/listDPGIncAccTPC" "$gridpath/LHC16MC_v14/listDPGIncTPC"
#  ### LHC17 MC
#  bash DownScript.sh 3032 3031 -Name_LHC17MC_v14 GA_pp_MC_AOD ?_zip ?_GammaConvCalo_2001 -onlyrunwise -mergeruns -RL_listDPGEDC -RL_listDPG -RL_listDPGIncTPC -RL_listPCMEDC $1
#  ### LHC18 MC
#  bash DownScript.sh 3034 3033 -Name_LHC18MC_v14 GA_pp_MC_AOD ?_zip ?_GammaConvCalo_2001 -onlyrunwise -mergeruns -RL_listDPGEMC -RL_listDPG -RL_listPCMEDC -RL_listDPCIncAccTPC $1
#  # ln -sf "$gridpath/LHC18MC_v14/listDPCIncAccTPC" "$gridpath/LHC18MC_v14/listDPGIncTPC"
#  # ln -sf "$gridpath/LHC18MC_v14/listDPGEMC" "$gridpath/LHC18MC_v14/listDPGEDC"
#  # ln -sf "$gridpath/.GA_pp_MC_AOD-3033/listDPGEMC" "$gridpath/.GA_pp_MC_AOD-3033/listDPGEDC"
#  # ln -sf "$gridpath/.GA_pp_MC_AOD-3034/listDPGEMC" "$gridpath/.GA_pp_MC_AOD-3034/listDPGEDC"

#  ### Data 16 + 17 + 18
#  bash DownScript.sh 1551 1550 1549 -Name_Data_pp13TeV_v14 GA_pp_AOD ?_zip ?_GammaConvCalo_2001 -mergetrains -RL_listDPGEDC_OR_listDPGEMC -RL_listDPG -RL_listDPGIncTPC $1
#  ### MC 16 + 17 + 18
#  bash DownScript.sh 3034 3033 3032 3031 3030 3029 3028 -Name_MC_pp13TeV_v14 GA_pp_MC_AOD ?_zip ?_GammaConvCalo_2001 -mergetrains -RL_listDPGEDC_OR_listDPGEMC -RL_listDPG -RL_listPCMEDC -RL_listDPCIncAccTPC $1

#  ######################## Photon QA #############################
#  ### LHC16 Data
#  bash DownScript.sh 1549 -Name_LHC16Data_PhotonQA_new GA_pp_AOD ?_zip ?_AnalysisResults -onlyrunwise -RL_listDPGIncAcc -runwise $1 # -RL_listDPG -RL_listPCM
#  # ln -sf "$gridpath/LHC16Data_v14/listDPGIncAcc" "$gridpath/LHC16Data_v14/listDPGIncTPC"
#  ### LHC17 Data
#  bash DownScript.sh 1550 -Name_LHC17Data_PhotonQA_new GA_pp_AOD ?_zip ?_AnalysisResults -onlyrunwise -RL_listDPGIncTPC -runwise $1 # -RL_listDPG -RL_listPCM
#  ### LHC18 Data
#  bash DownScript.sh 1551 -Name_LHC18Data_PhotonQA_new GA_pp_AOD ?_zip ?_AnalysisResults -onlyrunwise -RL_listDPGIncTPC -runwise $1 # -RL_listDPG -RL_listPCM
#  ### LHC16 MC
#  bash DownScript.sh 3030 3029 3028 -Name_LHC16MC_PhotonQA_new GA_pp_MC_AOD ?_zip ?_AnalysisResults -onlyrunwise -mergeruns -RL_listDPGIncAccTPC -runwise $1 # -RL_listDPG -RL_listPCM
#  # ln -sf "$gridpath/LHC16MC_v14/listDPGIncAccTPC" "$gridpath/LHC16MC_v14/listDPGIncTPC"
#  ### LHC17 MC
#  bash DownScript.sh 3032 3031 -Name_LHC17MC_PhotonQA_new GA_pp_MC_AOD ?_zip ?_AnalysisResults -onlyrunwise -mergeruns -RL_listDPGIncTPC -runwise $1 # -RL_listDPG -RL_listPCM
#  ### LHC18 MC
#  bash DownScript.sh 3034 3033 -Name_LHC18MC_PhotonQA_new GA_pp_MC_AOD ?_zip ?_AnalysisResults -onlyrunwise -mergeruns -RL_listDPCIncAccTPC -runwise $1 # -RL_listDPG -RL_listPCM
#  # ln -sf "$gridpath/LHC18MC_v14/listDPCIncAccTPC" "$gridpath/LHC18MC_v14/listDPGIncTPC"



cat Error.log
exit
fi
