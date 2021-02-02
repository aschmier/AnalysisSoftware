#! /bin/bash
echo "Starting DownloadAndMerge for pp 13TeV..."
echo;

thisuser=`echo ${USER}`
d=`date +%Y_%m_%d`
if [[ $thisuser = "adrian" || $thisuser = "amechler" ]]
then

    if [[ -f Error.log ]]; then
        rm Error.log
    fi
    gridpath="/media/adrian/Data/grid_data"

# # 1692 	vAN-20210118_ROOT6-1 	LHC18c_13TeV_LowB_pass1_AOD236 	2021 Jan 20, Train run finished 	13TeV AOD validation 	
# # 1691 	vAN-20210118_ROOT6-1 	LHC17g_13TeV_LowB_pass1_AOD236 	2021 Jan 20, Train run finished 	13TeV AOD validation 	
# # 1690 	vAN-20210118_ROOT6-1 	LHC16f_13TeV_LowB_pass1_AOD236 	2021 Jan 20, Train run finished 	13TeV AOD validation 	
# bash DownScript.sh  1690 -Name_Data_lowB_AOD236_16 GA_pp_AOD ?_GammaConvCalo -RL_listDPGTrackIncAccTPCandEMC  $1
# bash DownScript.sh  1690 -Name_Data_lowB_AOD236_16 GA_pp_AOD ?_GammaCalo_800 -RL_listDPGTrackIncAccTPCandEMC  $1
# bash DownScript.sh  1691 -Name_Data_lowB_AOD236_17 GA_pp_AOD ?_GammaConvCalo -RL_listDPGTrackIncAccTPCandEMC  $1
# bash DownScript.sh  1691 -Name_Data_lowB_AOD236_17 GA_pp_AOD ?_GammaCalo_800 -RL_listDPGTrackIncAccTPCandEMC  $1
# bash DownScript.sh  1692 -Name_Data_lowB_AOD236_18 GA_pp_AOD ?_GammaConvCalo Child_1 Child_2 -RL_listDPGTrackIncAccTPCandEMC  $1
# bash DownScript.sh  1692 -Name_Data_lowB_AOD236_18 GA_pp_AOD ?_GammaCalo_800 Child_1 Child_2 -RL_listDPGTrackIncAccTPCandEMC  $1
# bash DownScript.sh  1692 1691 1690 -Name_Data_lowB_AOD236 GA_pp_AOD ?_GammaConvCalo -mergetrains -RL_listDPGTrackIncAccTPCandEMC -noDown $1

# # 1689 	vAN-20210118_ROOT6-1 	LHC18c_13TeV_LowB_pass1_AOD212 	2021 Jan 20, Train run finished 	13TeV AOD validation 	
# # 1688 	vAN-20210118_ROOT6-1 	LHC17g_13TeV_LowB_pass1_AOD212 	2021 Jan 20, Train run finished 	13TeV AOD validation 	
# # 1687 	vAN-20210118_ROOT6-1 	LHC16f_13TeV_LowB_pass1_AOD212 	2021 Jan 20, Train run finished 	13TeV AOD validation 
# bash DownScript.sh  1687 -Name_Data_lowB_AOD237_16 GA_pp_AOD ?_GammaConvCalo -RL_listDPGTrackIncAccTPCandEMC  $1
# bash DownScript.sh  1687 -Name_Data_lowB_AOD237_16 GA_pp_AOD ?_GammaCalo_800 -RL_listDPGTrackIncAccTPCandEMC  $1
# bash DownScript.sh  1688 -Name_Data_lowB_AOD237_17 GA_pp_AOD ?_GammaConvCalo -RL_listDPGTrackIncAccTPCandEMC  $1
# bash DownScript.sh  1688 -Name_Data_lowB_AOD237_17 GA_pp_AOD ?_GammaCalo_800 -RL_listDPGTrackIncAccTPCandEMC  $1
bash DownScript.sh  1689 -Name_Data_lowB_AOD237_18 GA_pp_AOD ?_GammaConvCalo Child_1 Child_2 -RL_listDPGTrackIncAccTPCandEMC  $1
bash DownScript.sh  1689 -Name_Data_lowB_AOD237_18 GA_pp_AOD ?_GammaCalo_800 Child_1 Child_2 -mergechilds -RL_listDPGTrackIncAccTPCandEMC  $1
bash DownScript.sh  1689 1688 1687 -Name_Data_lowB_AOD237 GA_pp_AOD ?_GammaConvCalo -mergetrains -RL_listDPGTrackIncAccTPCandEMC -noDown $1
bash DownScript.sh  1689 1688 1687 -Name_Data_lowB_AOD237 GA_pp_AOD ?_GammaCalo_800 -mergetrains -RL_listDPGTrackIncAccTPCandEMC -noDown $1

# # # 3426 	vAN-20210118_ROOT6-1 	LHC18h1_PYT8_13TeV_ancLHC18c_LowB_AOD237 	2021 Jan 21, Train run finished 	13TeV AOD validation 	
# # # 3425 	vAN-20210118_ROOT6-1 	LHC17h3_PYT8_13TeV_anchLHC17g_LowB_AOD237 	2021 Jan 21, Train run finished 	13TeV AOD validation 	
# # # 3424 	vAN-20210118_ROOT6-1 	LHC17d1_PYT8_13TeV_ancLHC16f_LowB_AOD237 	2021 Jan 21, Train run finished 	13TeV AOD validation
# # bash DownScript.sh  3424 -Name_MC_lowB_AOD237_16 GA_pp_MC_AOD ?_GammaConvCalo -RL_listDPGTrackIncAccTPCandEMC  $1
# bash DownScript.sh  3424 -Name_MC_lowB_AOD237_16 GA_pp_MC_AOD ?_GammaCalo_800 -RL_listDPGTrackIncAccTPCandEMC  $1
# # bash DownScript.sh  3425 -Name_MC_lowB_AOD237_17 GA_pp_MC_AOD ?_GammaConvCalo -RL_listDPGTrackIncAccTPCandEMC  $1
# bash DownScript.sh  3425 -Name_MC_lowB_AOD237_17 GA_pp_MC_AOD ?_GammaCalo_800 -RL_listDPGTrackIncAccTPCandEMC  $1
# # bash DownScript.sh  3426 -Name_MC_lowB_AOD237_18 GA_pp_MC_AOD ?_GammaConvCalo Child_1 Child_2 -RL_listDPGTrackIncAccTPCandEMC  $1
# bash DownScript.sh  3426 -Name_MC_lowB_AOD237_18 GA_pp_MC_AOD ?_GammaCalo_800 Child_1 Child_2 -RL_listDPGTrackIncAccTPCandEMC  $1
# bash DownScript.sh  3426 3425 3424 -Name_MC_lowB_AOD237 GA_pp_MC_AOD ?_GammaConvCalo -mergetrains -RL_listDPGTrackIncAccTPCandEMC -noDown $1
# bash DownScript.sh  3426 3425 3424 -Name_MC_lowB_AOD237 GA_pp_MC_AOD ?_GammaCalo_800 -mergetrains -RL_listDPGTrackIncAccTPCandEMC -noDown $1

# # # 3423 	vAN-20210118_ROOT6-1 	LHC18h1_PYT8_13TeV_ancLHC18c_LowB_AOD213 	2021 Jan 21, Train run finished 	13TeV AOD validation 	
# # # 3422 	vAN-20210118_ROOT6-1 	LHC17h3_PYT8_13TeV_anchLHC17g_LowB_AOD213 	2021 Jan 21, Train run finished 	13TeV AOD validation 	
# # # 3421 	vAN-20210118_ROOT6-1 	LHC17d1_PYT8_13TeV_ancLHC16f_LowB_AOD213 	2021 Jan 21, Train run finished 	13TeV AOD validation 
# # bash DownScript.sh  3421 -Name_MC_lowB_AOD213_16 GA_pp_MC_AOD ?_GammaConvCalo -RL_listDPGTrackIncAccTPCandEMC  $1
# bash DownScript.sh  3421 -Name_MC_lowB_AOD213_16 GA_pp_MC_AOD ?_GammaCalo_800 -RL_listDPGTrackIncAccTPCandEMC  $1
# # bash DownScript.sh  3422 -Name_MC_lowB_AOD213_17 GA_pp_MC_AOD ?_GammaConvCalo -RL_listDPGTrackIncAccTPCandEMC  $1
# bash DownScript.sh  3422 -Name_MC_lowB_AOD213_17 GA_pp_MC_AOD ?_GammaCalo_800 -RL_listDPGTrackIncAccTPCandEMC  $1
# # bash DownScript.sh  3423 -Name_MC_lowB_AOD213_18 GA_pp_MC_AOD ?_GammaConvCalo Child_1 Child_2 Child_4 Child_5 -RL_listDPGTrackIncAccTPCandEMC  $1
# bash DownScript.sh  3423 -Name_MC_lowB_AOD213_18 GA_pp_MC_AOD ?_GammaCalo_800 Child_1 Child_2 Child_4 Child_5 -RL_listDPGTrackIncAccTPCandEMC  $1
# bash DownScript.sh  3423 3422 3421 -Name_MC_lowB_AOD213 GA_pp_MC_AOD ?_GammaConvCalo -mergetrains -RL_listDPGTrackIncAccTPCandEMC -noDown $1
# bash DownScript.sh  3423 3422 3421 -Name_MC_lowB_AOD213 GA_pp_MC_AOD ?_GammaCalo_800 -mergetrains -RL_listDPGTrackIncAccTPCandEMC -noDown $1

# # # 1686 	vAN-20210118_ROOT6-1 	LHC18_13TeV_pass1_AOD234 	2021 Jan 20, Train run: All jobs submitted 	13TeV AOD validation 	
# # # 1685 	vAN-20210118_ROOT6-1 	LHC17_13TeV_pass1_AOD234 	2021 Jan 20, Train run: All jobs submitted 	13TeV AOD validation 	
# # # 1684 	vAN-20210118_ROOT6-1 	LHC16_13TeV_pass1_2_AOD234 	2021 Jan 20, Train run finished 	13TeV AOD validation 	
# # bash DownScript.sh 1684 -Name_Data_AOD234_16 GA_pp_AOD ?_GammaConvCalo -RL_listPCMEDC  -RL_listPHOS  $1
# # bash DownScript.sh 1685 -Name_Data_AOD234_17 GA_pp_AOD ?_GammaConvCalo -RL_listPCMEDC  -RL_listPHOS  $1
# # bash DownScript.sh 1686 -Name_Data_AOD234_18 GA_pp_AOD ?_GammaConvCalo -RL_listPCMEDC  -RL_listPHOS  $1
# bash DownScript.sh 1686 1685 1684 -Name_Data_AOD234 GA_pp_AOD ?_GammaConvCalo -mergetrains -RL_listPCMEDC  -RL_listPHOS -noDown $1

# # # 1683 	vAN-20210118_ROOT6-1 	LHC18_13TeV_pass1_AOD208 	2021 Jan 20, Train run: All jobs submitted 	13TeV AOD validation 	
# # # 1682 	vAN-20210118_ROOT6-1 	LHC17_13TeV_pass1_AOD208 	2021 Jan 20, Train run: All jobs submitted 	13TeV AOD validation 	
# # # 1681 	vAN-20210118_ROOT6-1 	LHC16_13TeV_pass1_2_AOD208 	2021 Jan 20, Train run: All jobs submitted 	13TeV AOD validation 	
# # bash DownScript.sh 1681 -Name_Data_AOD208_16 GA_pp_AOD ?_GammaConvCalo -RL_listPCMEDC  -RL_listPHOS  $1
# # bash DownScript.sh 1682 -Name_Data_AOD208_17 GA_pp_AOD ?_GammaConvCalo -RL_listPCMEDC  -RL_listPHOS  $1
# # bash DownScript.sh 1683 -Name_Data_AOD208_18 GA_pp_AOD ?_GammaConvCalo -RL_listPCMEDC  -RL_listPHOS  $1
# bash DownScript.sh 1683 1682 1681 -Name_Data_AOD208 GA_pp_AOD ?_GammaConvCalo -mergetrains -RL_listPCMEDC  -RL_listPHOS -noDown $1

# # # 3416 	vAN-20210118_ROOT6-1 	PYT8_13TeV_anchLHC18_AOD_extra 	2021 Jan 21, Train run finished 	13TeV AOD validation 	
# # # 3420 	vAN-20210118_ROOT6-1 	PYT8_13TeV_anchLHC18_AOD235 	2021 Jan 21, Train run finished 	13TeV AOD validation 	
# # # 3419 	vAN-20210118_ROOT6-1 	PYT8_13TeV_anchLHC17_AOD235 	2021 Jan 21, Train run finished 	13TeV AOD validation 	
# # # 3418 	vAN-20210118_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD235_extra 	2021 Jan 21, Train run finished 	13TeV AOD validation 	
# # # 3417 	vAN-20210118_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD235 	2021 Jan 21, Train run finished 	13TeV AOD validation 	
# # bash DownScript.sh  3418 3417 -Name_MC_AOD235_16 GA_pp_MC_AOD ?_GammaConvCalo -RL_listPHOS -RL_listPCMEDC  $1
# # bash DownScript.sh  3419 -Name_MC_AOD235_17 GA_pp_MC_AOD ?_GammaConvCalo -RL_listPHOS -RL_listPCMEDC  $1
# # bash DownScript.sh  3420 -Name_MC_AOD235_18 GA_pp_MC_AOD ?_GammaConvCalo -RL_listPHOS -RL_listPCMEDC  $1
# # bash DownScript.sh  3416 3420 -Name_MC_AOD235_18_extra GA_pp_MC_AOD ?_GammaConvCalo -RL_listPHOS -RL_listPCMEDC  $1
# bash DownScript.sh  3416 3420 3419 3418 3417 -Name_MC_AOD235 GA_pp_MC_AOD ?_GammaConvCalo -mergetrains -RL_listPHOS -RL_listPCMEDC -noDown $1

# # # 3416 	vAN-20210118_ROOT6-1 	PYT8_13TeV_anchLHC18_AOD_extra 	2021 Jan 21, Train run finished 	13TeV AOD validation 	
# # # 3415 	vAN-20210118_ROOT6-1 	PYT8_13TeV_anchLHC18_AOD209 	2021 Jan 21, Train run finished 	13TeV AOD validation 	
# # # 3414 	vAN-20210118_ROOT6-1 	PYT8_13TeV_anchLHC17_AOD209_extra 	2021 Jan 21, Train run finished 	13TeV AOD validation 	
# # # 3413 	vAN-20210118_ROOT6-1 	PYT8_13TeV_anchLHC17_AOD209 	2021 Jan 21, Train run finished 	13TeV AOD validation 	
# # # 3412 	vAN-20210118_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD209_extra2 	2021 Jan 21, Train run finished 	13TeV AOD validation 	
# # # 3411 	vAN-20210118_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD209_extra 	2021 Jan 21, Train run finished 	13TeV AOD validation 	
# # # 3410 	vAN-20210118_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD209 	2021 Jan 21, Train run finished 	13TeV AOD validation 
# # bash DownScript.sh  3412 3411 3410 -Name_MC_AOD209_16 GA_pp_MC_AOD ?_GammaConvCalo -RL_listPHOS -RL_listPCMEDC $1
# # bash DownScript.sh  3414 3413 -Name_MC_AOD209_17 GA_pp_MC_AOD ?_GammaConvCalo -RL_listPHOS -RL_listPCMEDC $1
# # bash DownScript.sh  3415 -Name_MC_AOD209_18 GA_pp_MC_AOD ?_GammaConvCalo -RL_listPHOS -RL_listPCMEDC  $1
# # bash DownScript.sh  3416 3415 -Name_MC_AOD209_18_extra GA_pp_MC_AOD ?_GammaConvCalo -RL_listPHOS -RL_listPCMEDC  $1
# bash DownScript.sh  3416 3415 3414 3413 3412 3411 3410 -Name_MC_AOD209 GA_pp_MC_AOD ?_GammaConvCalo -mergetrains -RL_listPHOS -RL_listPCMEDC -noDown $1

###### redownload lowB

# #     #  ### LHC16 MC
# # rm -r /media/adrian/Data/grid_data/.GA_pp_MC_AOD-3348
#  bash DownScript.sh 3348 GA_pp_MC_AOD  ?_zip ?_GammaConvCalo -RL_DPGTrackIncAccTPCandEMC $1
#   ln -sf "$gridpath/.GA_pp_MC_AOD-3348/DPGTrackIncAccTPCandEMC" "$gridpath/.GA_pp_MC_AOD-3348/listDPGTrackIncAccTPCandEMC"
# # #  ### LHC17 MC
# # # rm -r /media/adrian/Data/grid_data/.GA_pp_MC_AOD-3349
#  bash DownScript.sh 3349 GA_pp_MC_AOD  ?_zip ?_GammaConvCalo -RL_listDPGTrackIncAccTPCandEMC $1
# # #  ### LHC18 MC
# # # rm -r /media/adrian/Data/grid_data/.GA_pp_MC_AOD-3350
#  bash DownScript.sh 3350 GA_pp_MC_AOD  ?_zip ?_GammaConvCalo Child_1 Child_2 Child_4 Child_5  -RL_listDPGTrackIncAccTPCandEMC $1

#     ### LHC16 Data
#     # rm -r /media/adrian/Data/grid_data/.GA_pp_AOD-1611 
#     bash DownScript.sh 1611 GA_pp_AOD  ?_zip ?_GammaConvCalo  -RL_DPGTrackIncAccTPCandEMC $1
#      ln -sf "$gridpath/.GA_pp_AOD-1611/DPGTrackIncAccTPCandEMC" "$gridpath/.GA_pp_AOD-1611/listDPGTrackIncAccTPCandEMC"
#     ### LHC17 Data
#     # rm -r /media/adrian/Data/grid_data/.GA_pp_AOD-1612 
#     bash DownScript.sh 1612 GA_pp_AOD  ?_zip ?_GammaConvCalo  -RL_listDPGTrackIncAccTPCandEMC $1
#     ### LHC18 Data
#     # rm -r /media/adrian/Data/grid_data/.GA_pp_AOD-1613 
#     bash DownScript.sh 1613 GA_pp_AOD  ?_zip ?_GammaConvCalo  Child_1 Child_2  -RL_listDPGTrackIncAccTPCandEMC $1


#     ### LHC16 MC
#     # rm -r /media/adrian/Data/grid_data/.GA_pp_MC_AOD-3216
#     bash DownScript.sh 3216 GA_pp_MC_AOD  ?_zip ?_GammaConvCalo -RL_DPGTrackIncAccTPCandEMC $1
#      ln -sf "$gridpath/.GA_pp_MC_AOD-3216/DPGTrackIncAccTPCandEMC" "$gridpath/.GA_pp_MC_AOD-3216/listDPGTrackIncAccTPCandEMC"
#     ### LHC17 MC
#     # rm -r /media/adrian/Data/grid_data/.GA_pp_MC_AOD-3217
#     bash DownScript.sh 3217 GA_pp_MC_AOD  ?_zip ?_GammaConvCalo -RL_listDPGTrackIncAccTPCandEMC $1
#     ### LHC18 MC
#     # rm -r /media/adrian/Data/grid_data/.GA_pp_MC_AOD-3218
#     bash DownScript.sh 3218 GA_pp_MC_AOD  ?_zip ?_GammaConvCalo  Child_1 Child_2 Child_4 Child_5  -RL_listDPGTrackIncAccTPCandEMC $1

#      ##################   Data   #############################
#     ### LHC16 Data
#     # rm -r /media/adrian/Data/grid_data/.GA_pp_AOD-1564
    # bash DownScript.sh 1564 GA_pp_AOD  ?_GammaConvCalo  -RL_DPGTrackIncAccTPCandEMC $1
    #  ln -sf "$gridpath/.GA_pp_AOD-1564/DPGTrackIncAccTPCandEMC" "$gridpath/.GA_pp_AOD-1564/listDPGTrackIncAccTPCandEMC"
#     ### LHC17 Data
#     # rm -r /media/adrian/Data/grid_data/.GA_pp_AOD-1565
    # bash DownScript.sh 1565 GA_pp_AOD  ?_GammaConvCalo  -RL_listDPGTrackIncAccTPCandEMC $1
#     ## LHC18 Data
#     # rm -r /media/adrian/Data/grid_data/.GA_pp_AOD-1566
    # bash DownScript.sh 1566 GA_pp_AOD  ?_GammaConvCalo  Child_1 Child_2  -RL_listDPGTrackIncAccTPCandEMC $1
# #     ##################   MC   #############################
# #     ##### low B
# #     ### LHC16 MC
#     # rm -r /media/adrian/Data/grid_data/.GA_pp_MC_AOD-3114
    # bash DownScript.sh 3114 GA_pp_MC_AOD  ?_GammaConvCalo -RL_DPGTrackIncAccTPCandEMC $1
    #  ln -sf "$gridpath/.GA_pp_MC_AOD-3114/DPGTrackIncAccTPCandEMC" "$gridpath/.GA_pp_MC_AOD-3114/listDPGTrackIncAccTPCandEMC"
#     ### LHC17 MC
#     # rm -r /media/adrian/Data/grid_data/.GA_pp_MC_AOD-3115
    # bash DownScript.sh 3115 GA_pp_MC_AOD  ?_GammaConvCalo -RL_listDPGTrackIncAccTPCandEMC $1
#     ### LHC18 MC
#     # rm -r /media/adrian/Data/grid_data/.GA_pp_MC_AOD-3116
    # bash DownScript.sh 3116 GA_pp_MC_AOD  ?_GammaConvCalo  Child_1 Child_2 Child_4 Child_5  -RL_listDPGTrackIncAccTPCandEMC $1

# #  #################   merging   #############################
# # rm -r /media/adrian/Data/grid_data/MC_lowB_v17
# # bash DownScript.sh  3350 3349 3348 GA_pp_MC_AOD ?_GammaConvCalo -RL_listDPGTrackIncAccTPCandEMC $1
# bash DownScript.sh  3350 3349 3348 -Name_MC_lowB_v17 GA_pp_MC_AOD ?_GammaConvCalo -mergetrains -RL_listDPGTrackIncAccTPCandEMC -noDown $1
# # ################   
# # rm -r /media/adrian/Data/grid_data/Data_lowB_v16
# # bash DownScript.sh  1613 1612 1611 GA_pp_AOD ?_GammaConvCalo -RL_listDPGTrackIncAccTPCandEMC $1
# bash DownScript.sh  1613 1612 1611 -Name_Data_lowB_v16 GA_pp_AOD ?_GammaConvCalo -mergetrains -RL_listDPGTrackIncAccTPCandEMC -noDown $1
# # ################   
# # rm -r /media/adrian/Data/grid_data/MC_lowB_v16
# # bash DownScript.sh  3218 3217 3216 GA_pp_MC_AOD ?_GammaConvCalo -RL_listDPGTrackIncAccTPCandEMC $1
# bash DownScript.sh  3218 3217 3216 -Name_MC_lowB_v16 GA_pp_MC_AOD ?_GammaConvCalo -mergetrains -RL_listDPGTrackIncAccTPCandEMC -noDown $1
# # #################   
# # rm -r /media/adrian/Data/grid_data/Data_lowB_v15
# # bash DownScript.sh  1564 1565 1566 GA_pp_AOD ?_GammaConvCalo -RL_listDPGTrackIncAccTPCandEMC $1
# bash DownScript.sh  1564 1565 1566 -Name_Data_lowB_v15 GA_pp_AOD ?_GammaConvCalo -mergetrains -RL_listDPGTrackIncAccTPCandEMC -noDown $1
# # #################   
# # rm -r /media/adrian/Data/grid_data/MC_lowB_v15
# # bash DownScript.sh  3116 3115 3114 GA_pp_MC_AOD ?_GammaConvCalo -RL_listDPGTrackIncAccTPCandEMC $1
# bash DownScript.sh  3116 3115 3114 -Name_MC_lowB_v15 GA_pp_MC_AOD ?_GammaConvCalo -mergetrains -RL_listDPGTrackIncAccTPCandEMC -noDown $1




    # Variations
# 1664 	vAN-20201221_ROOT6-1 	LHC18_13TeV_pass1_AOD208 	2020 Dec 23, Train run finished 	request Adrian & Jens 	
# 1663 	vAN-20201221_ROOT6-1 	LHC17_13TeV_pass1_AOD208 	2020 Dec 23, Train run: All jobs submitted 	request Adrian & Jens 	
# 1662 	vAN-20201221_ROOT6-1 	LHC16_13TeV_pass1_2_AOD208 	2020 Dec 23, Train run: All jobs submitted 	request Adrian & Jens 	

# rm -r /media/adrian/Data/grid_data/.GA_pp_AOD-1664
# mkdir /media/adrian/Data/grid_data/.GA_pp_AOD-1664
# mkdir /media/adrian/Data/grid_data/.GA_pp_AOD-1664/listPCMEDC
# scp  amechler@pcikf2:/data/amechler/Downloads/data/.GA_pp_AOD-1664/listPCMEDC/*.root /media/adrian/Data/grid_data/.GA_pp_AOD-1664/listPCMEDC/.
# rm -r /media/adrian/Data/grid_data/.GA_pp_AOD-1663
# mkdir /media/adrian/Data/grid_data/.GA_pp_AOD-1663
# mkdir /media/adrian/Data/grid_data/.GA_pp_AOD-1663/listPCMEDC
# scp  amechler@pcikf2:/data/amechler/Downloads/data/.GA_pp_AOD-1663/listPCMEDC/*.root /media/adrian/Data/grid_data/.GA_pp_AOD-1663/listPCMEDC/.
# rm -r /media/adrian/Data/grid_data/.GA_pp_AOD-1662
# mkdir /media/adrian/Data/grid_data/.GA_pp_AOD-1662
# mkdir /media/adrian/Data/grid_data/.GA_pp_AOD-1662/listPCMEDC
# scp  amechler@pcikf2:/data/amechler/Downloads/data/.GA_pp_AOD-1662/listPCMEDC/*.root /media/adrian/Data/grid_data/.GA_pp_AOD-1662/listPCMEDC/.

# bash DownScript.sh 1664 1663 1662 -Name_Data_pp13TeV_v18 GA_pp_AOD ?_GammaConvCalo -mergetrains -RL_listPCMEDC -noDown $1




# 3367 	vAN-20201221_ROOT6-1 	PYT8_13TeV_anchLHC18_AOD_extra 	2020 Dec 23, Train run finished 	request Adrian 	
# 3366 	vAN-20201221_ROOT6-1 	PYT8_13TeV_anchLHC18_AOD209 	2020 Dec 23, Train run finished 	request Adrian 	
# 3365 	vAN-20201221_ROOT6-1 	PYT8_13TeV_anchLHC17_AOD209_extra 	2020 Dec 23, Train run: All jobs submitted 	request Adrian 	
# 3364 	vAN-20201221_ROOT6-1 	PYT8_13TeV_anchLHC17_AOD209 	2020 Dec 23, Train run: All jobs submitted 	request Adrian 	
# 3363 	vAN-20201221_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD209_extra2 	2020 Dec 23, Train run: All jobs submitted 	request Adrian 	
# 3362 	vAN-20201221_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD209_extra 	2020 Dec 23, Train run: All jobs submitted 	request Adrian 	
# 3361 	vAN-20201221_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD209 	2020 Dec 23, Train run: All jobs submitted 	request Adrian 

# rm -r /media/adrian/Data/grid_data/.GA_pp_MC_AOD-3367
# mkdir /media/adrian/Data/grid_data/.GA_pp_MC_AOD-3367
# mkdir /media/adrian/Data/grid_data/.GA_pp_MC_AOD-3367/listPCMEDC
# scp  amechler@pcikf2:/data/amechler/Downloads/data/.GA_pp_MC_AOD-3367/listPCMEDC/*.root /media/adrian/Data/grid_data/.GA_pp_MC_AOD-3367/listPCMEDC/.
# rm -r /media/adrian/Data/grid_data/.GA_pp_MC_AOD-3366
# mkdir /media/adrian/Data/grid_data/.GA_pp_MC_AOD-3366
# mkdir /media/adrian/Data/grid_data/.GA_pp_MC_AOD-3366/listPCMEDC
# scp  amechler@pcikf2:/data/amechler/Downloads/data/.GA_pp_MC_AOD-3366/listPCMEDC/*.root /media/adrian/Data/grid_data/.GA_pp_MC_AOD-3366/listPCMEDC/.
# rm -r /media/adrian/Data/grid_data/.GA_pp_MC_AOD-3365
# mkdir /media/adrian/Data/grid_data/.GA_pp_MC_AOD-3365
# mkdir /media/adrian/Data/grid_data/.GA_pp_MC_AOD-3365/listPCMEDC
# scp  amechler@pcikf2:/data/amechler/Downloads/data/.GA_pp_MC_AOD-3365/listPCMEDC/*.root /media/adrian/Data/grid_data/.GA_pp_MC_AOD-3365/listPCMEDC/.
# rm -r /media/adrian/Data/grid_data/.GA_pp_MC_AOD-3364
# mkdir /media/adrian/Data/grid_data/.GA_pp_MC_AOD-3364
# mkdir /media/adrian/Data/grid_data/.GA_pp_MC_AOD-3364/listPCMEDC
# scp  amechler@pcikf2:/data/amechler/Downloads/data/.GA_pp_MC_AOD-3364/listPCMEDC/*.root /media/adrian/Data/grid_data/.GA_pp_MC_AOD-3364/listPCMEDC/.
# rm -r /media/adrian/Data/grid_data/.GA_pp_MC_AOD-3363
# mkdir /media/adrian/Data/grid_data/.GA_pp_MC_AOD-3363
# mkdir /media/adrian/Data/grid_data/.GA_pp_MC_AOD-3363/listPCMEDC
# scp  amechler@pcikf2:/data/amechler/Downloads/data/.GA_pp_MC_AOD-3363/listPCMEDC/*.root /media/adrian/Data/grid_data/.GA_pp_MC_AOD-3363/listPCMEDC/.
# rm -r /media/adrian/Data/grid_data/.GA_pp_MC_AOD-3362
# mkdir /media/adrian/Data/grid_data/.GA_pp_MC_AOD-3362
# mkdir /media/adrian/Data/grid_data/.GA_pp_MC_AOD-3362/listPCMEDC
# scp  amechler@pcikf2:/data/amechler/Downloads/data/.GA_pp_MC_AOD-3362/listPCMEDC/*.root /media/adrian/Data/grid_data/.GA_pp_MC_AOD-3362/listPCMEDC/.
# rm -r /media/adrian/Data/grid_data/.GA_pp_MC_AOD-3361
# mkdir /media/adrian/Data/grid_data/.GA_pp_MC_AOD-3361
# mkdir /media/adrian/Data/grid_data/.GA_pp_MC_AOD-3361/listPCMEDC
# scp  amechler@pcikf2:/data/amechler/Downloads/data/.GA_pp_MC_AOD-3361/listPCMEDC/*.root /media/adrian/Data/grid_data/.GA_pp_MC_AOD-3361/listPCMEDC/.

# bash DownScript.sh  3367 3366 3365 3364 3363 3362 3361 -Name_MC_pp13TeV_v18 GA_pp_MC_AOD ?_GammaConvCalo -mergetrains  -RL_listPCMEDC -noDown

# # 1657 	vAN-20201217_ROOT6-1 	LHC18_13TeV_pass1_AOD208 	2020 Dec 18, Train run finished 	request Adrian, Joshua
# # 1656 	vAN-20201217_ROOT6-1 	LHC17_13TeV_pass1_AOD208 	2020 Dec 18, Train run finished 	request Adrian, Joshua
# # 1655 	vAN-20201217_ROOT6-1 	LHC16_13TeV_pass1_2_AOD208 	2020 Dec 18, Train run finished 	request Adrian, Joshua
# ### LHC16 Data
#  bash DownScript.sh 1655 GA_pp_AOD  ?_zip ?_GammaConvCalo  -RL_listPCMEDC -RL_listDPGEDCtrigger -RL_listPHOS $1
#  ### LHC17 Data
#  bash DownScript.sh 1656 GA_pp_AOD  ?_zip ?_GammaConvCalo  -RL_listPCMEDC -RL_listDPGEDCtrigger -RL_listPHOS $1
#  ### LHC18 Data
#  bash DownScript.sh 1657 GA_pp_AOD  ?_zip ?_GammaConvCalo  -RL_listPCMEDC -RL_listEDC_trigger -RL_listDPGEDCtrigger -RL_listPHOS $1
#      ln -sf "$gridpath/.GA_pp_AOD-1657/listEDC_trigger" "$gridpath/.GA_pp_AOD-1657/listDPGEDCtrigger"
#      #################   merging   #############################
# #  ### Data 16 + 17 + 18
# bash DownScript.sh 1657 1656 1655 -Name_Data_pp13TeV_v17 GA_pp_AOD ?_GammaConvCalo -mergetrains -RL_listPCMEDC  -RL_listPHOS -RL_listDPGEDCtrigger -noDown $1

# # 3350 	vAN-20201217_ROOT6-1 	LHC18h1_PYT8_13TeV_ancLHC18c_LowB_AOD213 	2020 Dec 18, Train run finished 	request Adrian, Joshua
# # 3349 	vAN-20201217_ROOT6-1 	LHC17h3_PYT8_13TeV_anchLHC17g_LowB_AOD213 	2020 Dec 18, Train run finished 	request Adrian, Joshua
# # 3348 	vAN-20201217_ROOT6-1 	LHC17d1_PYT8_13TeV_ancLHC16f_LowB_AOD213 	2020 Dec 18, Train run finished 	request Adrian, Joshua
# #  ### LHC16 MC
# rm -r /media/adrian/Data/grid_data/.GA_pp_MC_AOD-3348
#  bash DownScript.sh 3348 GA_pp_MC_AOD  ?_zip ?_GammaConvCalo -RL_lowB_DPGTrackIncAccTPCandEMC  -RL_listDPGTrackIncAccTPCandEMC $1
#  ln -sf "$gridpath/.GA_pp_MC_AOD-3348/lowB_DPGTrackIncAccTPCandEMC" "$gridpath/.GA_pp_MC_AOD-3348/listDPGTrackIncAccTPCandEMC"
# #  ### LHC17 MC
# rm -r /media/adrian/Data/grid_data/.GA_pp_MC_AOD-3349
#  bash DownScript.sh 3349 GA_pp_MC_AOD  ?_zip ?_GammaConvCalo -RL_listDPGTrackIncAccTPCandEMC $1
# #  ### LHC18 MC
# rm -r /media/adrian/Data/grid_data/.GA_pp_MC_AOD-3350
#  bash DownScript.sh 3350 GA_pp_MC_AOD  ?_zip ?_GammaConvCalo -mergetrains  -RL_listDPGTrackIncAccTPCandEMC $1
# #  #################   merging   #############################
# rm -r /media/adrian/Data/grid_data/MC_pp13TeV_lowB_v17
# bash DownScript.sh  3350 3349 3348 -Name_MC_pp13TeV_lowB_v17 GA_pp_MC_AOD ?_GammaConvCalo -mergetrains -RL_listDPGTrackIncAccTPCandEMC -noDown $1


# # 3347 	vAN-20201217_ROOT6-1 	PYT8_13TeV_anchLHC18_AOD_extra 	2020 Dec 18, Train run finished 	request Adrian, Joshua
# # 3346 	vAN-20201217_ROOT6-1 	PYT8_13TeV_anchLHC18_AOD209 	2020 Dec 18, Train run finished 	request Adrian, Joshua
# # 3345 	vAN-20201217_ROOT6-1 	PYT8_13TeV_anchLHC17_AOD209_extra 	2020 Dec 18, Train run finished 	request Adrian, Joshua
# # 3344 	vAN-20201217_ROOT6-1 	PYT8_13TeV_anchLHC17_AOD209 	2020 Dec 18, Train run finished 	request Adrian, Joshua
# # 3343 	vAN-20201217_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD209_extra2 	2020 Dec 18, Train run finished 	request Adrian, Joshua
# # 3342 	vAN-20201217_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD209_extra 	2020 Dec 18, Train run finished 	request Adrian, Joshua
# # 3341 	vAN-20201217_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD209 	2020 Dec 18, Train run finished 	request Adrian, Joshua
# ### LHC16 MC
#  bash DownScript.sh 3343 3342 3341 GA_pp_MC_AOD  ?_zip ?_GammaConvCalo -mergetrains  -RL_listPCMEDC -RL_listPHOS $1
#  ### LHC17 MC
#  bash DownScript.sh 3345 3344 GA_pp_MC_AOD  ?_zip ?_GammaConvCalo -mergetrains  -RL_listPCMEDC -RL_listPHOS $1
#  ### LHC18 MC
#  bash DownScript.sh 3347 3346 GA_pp_MC_AOD  ?_zip ?_GammaConvCalo -mergetrains  -RL_listPCMEDC -RL_listPHOS $1
# # ################   merging   #############################
#  bash DownScript.sh  3347 3346 3345 3344 3343 3342 3341 -Name_MC_pp13TeV_v17 GA_pp_MC_AOD ?_GammaConvCalo -mergetrains  -RL_listPCMEDC -RL_listPHOS -noDown $1


    # 3262 	vAN-20201126_ROOT6-1 	PYT8JJ_13TeV_anchLHC18_AOD 	2020 Nov 27, Test started 58s ago 	request Adrian
    # 3261 	vAN-20201126_ROOT6-1 	PYT8JJ_13TeV_anchLHC17_AOD 	2020 Nov 27, Test started 1m 22s ago 	request Adrian
    # 3260 	vAN-20201126_ROOT6-1 	PYT8JJ_13TeV_anchLHC16_AOD209 	2020 Nov 27, Test started 1m 55s ago 	request Adrian
    # bash getTraintests.sh ?_GammaConvCalo GA_pp_MC_AOD 3262 3261 3260
       ##############   JJ MC   #############################
    ### LHC16 MC
    # bash DownScript.sh 3260 -Name_JJLHC16MC_v16 GA_pp_MC_AOD  ?_zip ?_Gamma -mergetrains -IsJJ $1
    ### LHC17 MC
    # bash DownScript.sh 3261 -Name_JJLHC17MC_v16 GA_pp_MC_AOD   ?_zip ?_Gamma -mergetrains -IsJJ $1
    # ### LHC18 MC
    # bash DownScript.sh 3262 -Name_JJLHC18MC_v16 GA_pp_MC_AOD  ?_zip ?_Gamma -mergetrains -IsJJ $1
    # ################   merging   #############################
    # # bash DownScript.sh 3262 3261 3260 GA_pp_MC_AOD ?_zip ?_Gamma -mergetrains  -IsJJ -norunwise $1
    # bash DownScript.sh 3262 3261 3260 -Name_JJMC_pp13TeV_v16 GA_pp_MC_AOD ?_Gamma -mergetrains  -RL_default -noDown -norunwise $1
    #

    # rm -r /media/adrian/Data/grid_data/.GA_pp_MC_AOD-3262
    # mkdir /media/adrian/Data/grid_data/.GA_pp_MC_AOD-3262
    # mkdir /media/adrian/Data/grid_data/.GA_pp_MC_AOD-3262/default
    # scp  amechler@pcikf2:/data/amechler/Downloads/data/.GA_pp_MC_AOD-3262/default/*.root /media/adrian/Data/grid_data/.GA_pp_MC_AOD-3262/default/.
    # rm -r /media/adrian/Data/grid_data/.GA_pp_MC_AOD-3261
    # mkdir /media/adrian/Data/grid_data/.GA_pp_MC_AOD-3261
    # mkdir /media/adrian/Data/grid_data/.GA_pp_MC_AOD-3261/default
    # scp  amechler@pcikf2:/data/amechler/Downloads/data/.GA_pp_MC_AOD-3261/default/*.root /media/adrian/Data/grid_data/.GA_pp_MC_AOD-3261/default/.
    # rm -r /media/adrian/Data/grid_data/.GA_pp_MC_AOD-3260
    # mkdir /media/adrian/Data/grid_data/.GA_pp_MC_AOD-3260
    # mkdir /media/adrian/Data/grid_data/.GA_pp_MC_AOD-3260/default
    # scp  amechler@pcikf2:/data/amechler/Downloads/data/.GA_pp_MC_AOD-3260/default/*.root /media/adrian/Data/grid_data/.GA_pp_MC_AOD-3260/default/.
    # bash DownScript.sh 3262 3261 3260 -Name_JJMC_pp13TeV_v16 GA_pp_MC_AOD ?_Gamma -mergetrains  -RL_default -noDown $1
    #
    # ### JJ Photon QA
    # bash DownScript.sh 3260 -Name_LHC16JJMC_PhotonQA GA_pp_AOD ?_AnalysisResults -onlyrunwise -IsJJ -runwise $1
    # bash DownScript.sh 3261 -Name_LHC17JJMC_PhotonQA GA_pp_AOD ?_AnalysisResults -onlyrunwise -IsJJ -runwise $1
    # bash DownScript.sh 3262 -Name_LHC18JJMC_PhotonQA GA_pp_AOD ?_AnalysisResults -onlyrunwise -IsJJ -runwise $1




    # bash getTraintests.sh ?_GammaConvCalo GA_pp_AOD 1613 1612 1611
    # 1613 	vAN-20201119_ROOT6-1 	LHC18c_13TeV_LowB_pass1_AOD212 	2020 Nov 20, Test finished (29m 39s total time) 	request Adrian
    # 1612 	vAN-20201119_ROOT6-1 	LHC17g_13TeV_LowB_pass1_AOD212 	2020 Nov 20, Test finished (26m 10s total time) 	request Adrian
    # 1611 	vAN-20201119_ROOT6-1 	LHC16f_13TeV_LowB_pass1_AOD212 	2020 Nov 20, Test finished (17m 32s total time) 	request Adrian
    # ### LHC16 Data
    # rm -r /media/adrian/Data/grid_data/.GA_pp_AOD-1611 
    # bash DownScript.sh 1611 GA_pp_AOD  ?_zip ?_GammaConvCalo  -RL_lowB_DPGTrackIncAccTPCandEMC $1
    # ln -sf "$gridpath/.GA_pp_AOD-1611/lowB_DPGTrackIncAccTPCandEMC" "$gridpath/.GA_pp_AOD-1611/listDPGTrackIncAccTPCandEMC"
    # ### LHC17 Data
    # rm -r /media/adrian/Data/grid_data/.GA_pp_AOD-1612 
    # bash DownScript.sh 1612 GA_pp_AOD  ?_zip ?_GammaConvCalo  -RL_listDPGTrackIncAccTPCandEMC $1
    # ### LHC18 Data
    # rm -r /media/adrian/Data/grid_data/.GA_pp_AOD-1613 
    # bash DownScript.sh 1613 GA_pp_AOD  ?_zip ?_GammaConvCalo  -RL_listDPGTrackIncAccTPCandEMC $1
   
    # # bash getTraintests.sh ?_GammaConvCalo GA_pp_MC_AOD 3216 3217 3218
    # # 3218 	vAN-20201119_ROOT6-1 	LHC18h1_PYT8_13TeV_ancLHC18c_LowB_AOD213 	2020 Nov 20, Test finished (1:07 total time) 	request Adrian
    # # 3217 	vAN-20201119_ROOT6-1 	LHC17h3_PYT8_13TeV_anchLHC17g_LowB_AOD213 	2020 Nov 20, Test finished (23m 56s total time) 	request Adrian
    # # 3216 	vAN-20201119_ROOT6-1 	LHC17d1_PYT8_13TeV_ancLHC16f_LowB_AOD213 	2020 Nov 20, Test finished (59m 11s total time) 	request Adrian
    # ### LHC16 MC
    # rm -r /media/adrian/Data/grid_data/.GA_pp_MC_AOD-3216
    # bash DownScript.sh 3216 GA_pp_MC_AOD  ?_zip ?_GammaConvCalo -RL_lowB_DPGTrackIncAccTPCandEMC $1
    # ln -sf "$gridpath/.GA_pp_MC_AOD-3216/lowB_DPGTrackIncAccTPCandEMC" "$gridpath/.GA_pp_MC_AOD-3216/listDPGTrackIncAccTPCandEMC"
    # ### LHC17 MC
    # rm -r /media/adrian/Data/grid_data/.GA_pp_MC_AOD-3217
    # bash DownScript.sh 3217 GA_pp_MC_AOD  ?_zip ?_GammaConvCalo -RL_listDPGTrackIncAccTPCandEMC $1
    # ### LHC18 MC
    # rm -r /media/adrian/Data/grid_data/.GA_pp_MC_AOD-3218
    # bash DownScript.sh 3218 GA_pp_MC_AOD  ?_zip ?_GammaConvCalo -mergetrains  -RL_listDPGTrackIncAccTPCandEMC $1
   #  #################   merging   #############################
   #  ### Data lowB
   # rm -r /media/adrian/Data/grid_data/.GA_pp_AOD-1613
   # mkdir /media/adrian/Data/grid_data/.GA_pp_AOD-1613
   # mkdir /media/adrian/Data/grid_data/.GA_pp_AOD-1613/listDPGTrackIncAccTPCandEMC
   # scp  amechler@pcikf2:/data/amechler/Downloads/data/.GA_pp_AOD-1613/listDPGTrackIncAccTPCandEMC/*.root /media/adrian/Data/grid_data/.GA_pp_AOD-1613/listDPGTrackIncAccTPCandEMC/.
   # rm -r /media/adrian/Data/grid_data/.GA_pp_AOD-1612
   # mkdir /media/adrian/Data/grid_data/.GA_pp_AOD-1612
   # mkdir /media/adrian/Data/grid_data/.GA_pp_AOD-1612/listDPGTrackIncAccTPCandEMC
   # scp  amechler@pcikf2:/data/amechler/Downloads/data/.GA_pp_AOD-1612/listDPGTrackIncAccTPCandEMC/*.root /media/adrian/Data/grid_data/.GA_pp_AOD-1612/listDPGTrackIncAccTPCandEMC/.
   # rm -r /media/adrian/Data/grid_data/.GA_pp_AOD-1611
   # mkdir /media/adrian/Data/grid_data/.GA_pp_AOD-1611
   # mkdir /media/adrian/Data/grid_data/.GA_pp_AOD-1611/listDPGTrackIncAccTPCandEMC
   # scp  amechler@pcikf2:/data/amechler/Downloads/data/.GA_pp_AOD-1611/listDPGTrackIncAccTPCandEMC/*.root /media/adrian/Data/grid_data/.GA_pp_AOD-1611/listDPGTrackIncAccTPCandEMC/.
#    rm -r /media/adrian/Data/grid_data/Data_pp13TeV_lowB_v16
    # bash DownScript.sh  1613 1612 1611 -Name_Data_pp13TeV_lowB_v16 GA_pp_AOD ?_GammaConvCalo -mergetrains -RL_listDPGTrackIncAccTPCandEMC -noDown $1
   #  ### MC lowB
   # rm -r /media/adrian/Data/grid_data/.GA_pp_MC_AOD-3218
   # mkdir /media/adrian/Data/grid_data/.GA_pp_MC_AOD-3218
   # mkdir /media/adrian/Data/grid_data/.GA_pp_MC_AOD-3218/listDPGTrackIncAccTPCandEMC
   # scp  amechler@pcikf2:/data/amechler/Downloads/data/.GA_pp_MC_AOD-3218/listDPGTrackIncAccTPCandEMC/*.root /media/adrian/Data/grid_data/.GA_pp_MC_AOD-3218/listDPGTrackIncAccTPCandEMC/.
   # rm -r /media/adrian/Data/grid_data/.GA_pp_MC_AOD-3217
   # mkdir /media/adrian/Data/grid_data/.GA_pp_MC_AOD-3217
   # mkdir /media/adrian/Data/grid_data/.GA_pp_MC_AOD-3217/listDPGTrackIncAccTPCandEMC
   # scp  amechler@pcikf2:/data/amechler/Downloads/data/.GA_pp_MC_AOD-3217/listDPGTrackIncAccTPCandEMC/*.root /media/adrian/Data/grid_data/.GA_pp_MC_AOD-3217/listDPGTrackIncAccTPCandEMC/.
   # rm -r /media/adrian/Data/grid_data/.GA_pp_MC_AOD-3216
   # mkdir /media/adrian/Data/grid_data/.GA_pp_MC_AOD-3216
   # mkdir /media/adrian/Data/grid_data/.GA_pp_MC_AOD-3216/lowB_DPGTrackIncAccTPCandEMC
   # scp  amechler@pcikf2:/data/amechler/Downloads/data/.GA_pp_MC_AOD-3216/lowB_DPGTrackIncAccTPCandEMC/*.root /media/adrian/Data/grid_data/.GA_pp_MC_AOD-3216/lowB_DPGTrackIncAccTPCandEMC/.
#    ln -sf "$gridpath/.GA_pp_MC_AOD-3216/lowB_DPGTrackIncAccTPCandEMC" "$gridpath/.GA_pp_MC_AOD-3216/listDPGTrackIncAccTPCandEMC"
#     rm -r /media/adrian/Data/grid_data/MC_pp13TeV_lowB_v16
#    bash DownScript.sh  3218 3217 3216 -Name_MC_pp13TeV_lowB_v16 GA_pp_MC_AOD ?_GammaConvCalo -mergetrains -RL_listDPGTrackIncAccTPCandEMC -noDown $1
   # #
   #
   #
   #
   #
   #  # bash getTraintests.sh ?_GammaConvCalo GA_pp_AOD 1616 1615 1614
   #  # 1616 	vAN-20201119_ROOT6-1 	LHC18_13TeV_pass1_AOD208 	2020 Nov 20, Test started 1:45 ago 	reuest Adrian
   #  # 1615 	vAN-20201119_ROOT6-1 	LHC17_13TeV_pass1_AOD208 	2020 Nov 20, Test started 1:48 ago 	reuest Adrian
   #  # 1614 	vAN-20201119_ROOT6-1 	LHC16_13TeV_pass1_2_AOD208 	2020 Nov 20, Test finished (1:14 total time) 	reuest Adrian
   #  ### LHC16 Data
   #  bash DownScript.sh 1614 -Name_LHC16Data_v16 GA_pp_AOD  ?_zip ?_GammaCalo  -RL_listPHOS $1
   #  bash DownScript.sh 1614 -Name_LHC16Data_v16 GA_pp_AOD  ?_zip ?_GammaConvCalo  -RL_listPCMEDC -RL_listDPGEDCtrigger $1
   #  ### LHC17 Data
   #  bash DownScript.sh 1615 -Name_LHC17Data_v16 GA_pp_AOD  ?_zip ?_GammaConvCalo  -RL_listPCMEDC -RL_listDPGEDCtrigger $1
   #  bash DownScript.sh 1615 -Name_LHC17Data_v16 GA_pp_AOD  ?_zip ?_GammaCalo  -RL_listPHOS $1
   #  ### LHC18 Data
   #  bash DownScript.sh 1616 -Name_LHC18Data_v16 GA_pp_AOD  ?_zip ?_GammaConvCalo  -RL_listPCMEDC -RL_listEDC_trigger $1
   #  bash DownScript.sh 1616 -Name_LHC18Data_v16 GA_pp_AOD  ?_zip ?_GammaCalo  -RL_listPHOS $1
   #      ln -sf "$gridpath/LHC18Data_v16/listEDC_trigger" "$gridpath/LHC18Data_v16/listDPGEDCtrigger"
   #      ln -sf "$gridpath/.GA_pp_AOD-1616/listEDC_trigger" "$gridpath/.GA_pp_AOD-1616/listDPGEDCtrigger"
   #
   #  # bash getTraintests.sh ?_GammaConvCalo GA_pp_MC_AOD 3225 3224 3223 3222 3221 3220 3219
   #  # 3225 	vAN-20201119_ROOT6-1 	PYT8_13TeV_anchLHC18_AOD_extra 	2020 Nov 20, Test finished (17m 9s total time) 	request Adrian
   #  # 3224 	vAN-20201119_ROOT6-1 	PYT8_13TeV_anchLHC18_AOD209 	2020 Nov 20, Test finished (8m 36s total time) 	request Adrian
   #  # 3223 	vAN-20201119_ROOT6-1 	PYT8_13TeV_anchLHC17_AOD209_extra 	2020 Nov 20, Test finished (13m 46s total time) 	request Adrian
   #  # 3222 	vAN-20201119_ROOT6-1 	PYT8_13TeV_anchLHC17_AOD209 	2020 Nov 20, Test finished (11m 28s total time) 	request Adrian
   #  # 3221 	vAN-20201119_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD209_extra2 	2020 Nov 20, Test finished (8m 30s total time) 	request Adrian
   #  # 3220 	vAN-20201119_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD209_extra 	2020 Nov 20, Test finished (20m 36s total time) 	request Adrian
   #  # 3219 	vAN-20201119_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD209 	2020 Nov 20, Test finished (7m 27s total time) 	request Adrian, Marvin
   # ### LHC16 MC
   #  bash DownScript.sh 3221 3220 3219 -Name_LHC16MC_v16 GA_pp_MC_AOD  ?_zip ?_GammaConvCalo -mergetrains  -RL_listPCMEDC $1
   #  bash DownScript.sh 3221 3220 3219 -Name_LHC16MC_v16 GA_pp_MC_AOD  ?_zip ?_GammaCalo -mergetrains  -RL_listPHOS $1
   #  ### LHC17 MC
   #  bash DownScript.sh 3223 3222 -Name_LHC17MC_v16 GA_pp_MC_AOD  ?_zip ?_GammaConvCalo -mergetrains  -RL_listPCMEDC $1
   #  bash DownScript.sh 3223 3222 -Name_LHC17MC_v16 GA_pp_MC_AOD  ?_zip ?_GammaCalo -mergetrains  -RL_listPHOS $1
   #  ### LHC18 MC
   #  bash DownScript.sh 3225 3224 -Name_LHC18MC_v16 GA_pp_MC_AOD  ?_zip ?_GammaConvCalo -mergetrains  -RL_listPCMEDC $1
   #  bash DownScript.sh 3225 3224 -Name_LHC18MC_v16 GA_pp_MC_AOD  ?_zip ?_GammaCalo -mergetrains  -RL_listPHOS $1
   #
   #  # #################   merging   #############################
#    #  ### Data 16 + 17 + 18
#     bash DownScript.sh 1616 1615 1614 -Name_Data_pp13TeV_v16 GA_pp_AOD ?_GammaConvCalo -mergetrains -RL_listPCMEDC -RL_listDPGEDCtrigger -noDown $1
#     bash DownScript.sh 1616 1615 1614 -Name_Data_pp13TeV_v16 GA_pp_AOD ?_GammaCalo -mergetrains -RL_listPHOS -noDown $1
#    #  ### MC 16 + 17 + 18
#     bash DownScript.sh  3225 3224 3223 3222 3221 3220 3219 -Name_MC_pp13TeV_v16 GA_pp_MC_AOD ?_GammaConvCalo -mergetrains  -RL_listPCMEDC -RL_listDPGEDCtrigger -noDown $1
#     bash DownScript.sh  3225 3224 3223 3222 3221 3220 3219 -Name_MC_pp13TeV_v16 GA_pp_MC_AOD ?_GammaCalo -mergetrains  -RL_listPHOS -noDown $1
#    #
   #  # 3228 	vAN-20201119_ROOT6-1 	PYT8JJ_13TeV_anchLHC18_AOD 	2020 Nov 26, Train run: All jobs submitted 	request Adrian
   #  # 3227 	vAN-20201119_ROOT6-1 	PYT8JJ_13TeV_anchLHC17_AOD 	2020 Nov 26, Train run: All jobs submitted 	request Adrian
   #  # 3226 	vAN-20201119_ROOT6-1 	PYT8JJ_13TeV_anchLHC16_AOD209 	2020 Nov 26, Train run: All jobs submitted 	request Adrian, Marvin
   #  #    ##############   JJ MC   #############################
    # ### LHC16 MC
    # bash DownScript.sh 3226 -Name_JJLHC16MC_v16 GA_pp_MC_AOD  ?_zip ?_Gamma -mergetrains -IsJJ $1
    # ### LHC17 MC
    # bash DownScript.sh 3227 -Name_JJLHC17MC_v16 GA_pp_MC_AOD   ?_zip ?_Gamma -mergetrains -IsJJ $1
    # ### LHC18 MC
    # bash DownScript.sh 3228 -Name_JJLHC18MC_v16 GA_pp_MC_AOD  ?_zip ?_Gamma -mergetrains -IsJJ $1
    # #################   merging   #############################
    # bash DownScript.sh 3228 3227 3226 -Name_JJMC_pp13TeV_v16 GA_pp_MC_AOD ?_Gamma -mergetrains  -IsJJ $1
    # bash DownScript.sh 3228 3227 3226 -Name_JJMC_pp13TeV_v16 GA_pp_MC_AOD ?_Gamma -mergetrains  -RL_default -noDown $1




#     ############ NL, NCell, Rbins   v15  ##############
#    ###############   Data   #########################
#     # 1566 	vAN-20201004_ROOT6-1 	LHC18c_13TeV_LowB_pass1_AOD212 	2020 Oct 06, Train run: All jobs submitted 	request Adrian, Ana
#     # 1565 	vAN-20201004_ROOT6-1 	LHC17g_13TeV_LowB_pass1_AOD212 	2020 Oct 06, Train run: All jobs submitted 	request Adrian, Ana
#     # 1564 	vAN-20201004_ROOT6-1 	LHC16f_13TeV_LowB_pass1_AOD212 	2020 Oct 06, Train run: All jobs submitted 	request Adrian, Ana
#     # ##### low B
# #     ##################   Data   #############################
#     ### LHC16 Data
#     rm -r /media/adrian/Data/grid_data/.GA_pp_AOD-1564
#     bash DownScript.sh 1564 GA_pp_AOD  ?_GammaConvCalo  -RL_lowB_DPGTrackIncAccTPCandEMC $1
#     ln -sf "$gridpath/.GA_pp_AOD-1564/lowB_DPGTrackIncAccTPCandEMC" "$gridpath/.GA_pp_AOD-1564/listDPGTrackIncAccTPCandEMC"
#     ### LHC17 Data
#     rm -r /media/adrian/Data/grid_data/.GA_pp_AOD-1565
#     bash DownScript.sh 1565 GA_pp_AOD  ?_GammaConvCalo  -RL_listDPGTrackIncAccTPCandEMC $1
#     ## LHC18 Data
#     rm -r /media/adrian/Data/grid_data/.GA_pp_AOD-1566
#     bash DownScript.sh 1566 GA_pp_AOD  ?_GammaConvCalo  -RL_listDPGTrackIncAccTPCandEMC $1
# #     ##################   MC   #############################
# #     ##### low B
# #    # 3116 	vAN-20201004_ROOT6-1 	LHC18h1_PYT8_13TeV_ancLHC18c_LowB_AOD213 	2020 Oct 17, Train run: All jobs submitted 	request Adrian, Ana
# #    # 3115 	vAN-20201004_ROOT6-1 	LHC17h3_PYT8_13TeV_anchLHC17g_LowB_AOD213 	2020 Oct 17, Train run finished 	request Adrian, Ana
# #    # 3114 	vAN-20201004_ROOT6-1 	LHC17d1_PYT8_13TeV_ancLHC16f_LowB_AOD213 	2020 Oct 17, Train run finished 	request Adrian, Ana
# #     ### LHC16 MC
#     rm -r /media/adrian/Data/grid_data/.GA_pp_MC_AOD-3114
#     bash DownScript.sh 3114 GA_pp_MC_AOD  ?_GammaConvCalo -RL_lowB_DPGTrackIncAccTPCandEMC $1
#     ln -sf "$gridpath/.GA_pp_MC_AOD-3114/lowB_DPGTrackIncAccTPCandEMC" "$gridpath/.GA_pp_MC_AOD-3114/listDPGTrackIncAccTPCandEMC"
#     ### LHC17 MC
#     rm -r /media/adrian/Data/grid_data/.GA_pp_MC_AOD-3115
#     bash DownScript.sh 3115 GA_pp_MC_AOD  ?_GammaConvCalo -RL_listDPGTrackIncAccTPCandEMC $1
#     ### LHC18 MC
#     rm -r /media/adrian/Data/grid_data/.GA_pp_MC_AOD-3116
#     bash DownScript.sh 3116 GA_pp_MC_AOD  ?_GammaConvCalo -mergetrains  -RL_listDPGTrackIncAccTPCandEMC $1
#     ##################   merging   #############################
# #     ### Data 16 + 17 + 18
# #     rm -r /media/adrian/Data/grid_data/Data_pp13TeV_lowB_v15
#     bash DownScript.sh  1564 1565 1566 -Name_Data_pp13TeV_lowB_v15 GA_pp_AOD ?_GammaConvCalo -mergetrains -RL_listDPGTrackIncAccTPCandEMC -noDown $1
# #     ### MC 16 + 17 + 18
# #     rm -r /media/adrian/Data/grid_data/MC_pp13TeV_lowB_v15
#     bash DownScript.sh  3116 3115 3114 -Name_MC_pp13TeV_lowB_v15 GA_pp_MC_AOD ?_GammaConvCalo -mergetrains -RL_listDPGTrackIncAccTPCandEMC -noDown $1
# #
# #
#
    # ##### norm B
    # 1563 	vAN-20201005_ROOT6-1 	LHC18_13TeV_pass1_AOD208 	2020 Oct 06, Train run: All jobs submitted 	request Ana, Adrian, Jens
    # 1562 	vAN-20201005_ROOT6-1 	LHC17_13TeV_pass1_AOD208 	2020 Oct 06, Train run: All jobs submitted 	request Ana, Adrian, Jens
    # 1561 	vAN-20201005_ROOT6-1 	LHC16_13TeV_pass1_2_AOD208 	2020 Oct 06, Train run: All jobs submitted 	request Ana, Adrian, Jens
    # ### LHC16 Data
    # bash DownScript.sh 1561 -Name_LHC16Data_v15 GA_pp_AOD  ?_GammaConvCalo  -RL_listPCMEDC -RL_listDPGEDCtrigger $1
    # ### LHC17 Data
    # bash DownScript.sh 1562 -Name_LHC17Data_v15 GA_pp_AOD  ?_GammaConvCalo  -RL_listPCMEDC -RL_listDPGEDCtrigger $1
    # ### LHC18 Data
    # bash DownScript.sh 1563 -Name_LHC18Data_v15 GA_pp_AOD  ?_GammaConvCalo  -RL_listPCMEDC -RL_listEDC_trigger $1
    #     ln -sf "$gridpath/LHC18Data_v15/listEDC_trigger" "$gridpath/LHC18Data_v15/listDPGEDCtrigger"
    #     ln -sf "$gridpath/.GA_pp_AOD-1563/listEDC_trigger" "$gridpath/.GA_pp_AOD-1563/listDPGEDCtrigger"
    # ##### norm B
    # 3113 	vAN-20201015_ROOT6-1 	PYT8_13TeV_anchLHC18_AOD_extra 	2020 Oct 16, Train run finished 	request Adrian, Steven
    # 3112 	vAN-20201015_ROOT6-1 	PYT8_13TeV_anchLHC18_AOD209 	2020 Oct 16, Train run finished 	request Adrian, Steven
    # 3111 	vAN-20201015_ROOT6-1 	PYT8_13TeV_anchLHC17_AOD209_extra 	2020 Oct 16, Train run finished 	request Adrian, Steven
    # 3110 	vAN-20201015_ROOT6-1 	PYT8_13TeV_anchLHC17_AOD209 	2020 Oct 16, Train run finished 	request Adrian, Steven
    # 3109 	vAN-20201015_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD209_extra2 	2020 Oct 16, Train run finished 	request Adrian, Steven
    # 3108 	vAN-20201015_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD209_extra 	2020 Oct 16, Train run finished 	request Adrian, Steven
    # 3107 	vAN-20201015_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD209 	2020 Oct 16, Train run finished 	request Adrian, Steven
    # ### LHC16 MC
    # bash DownScript.sh 3109 3108 3107 -Name_LHC16MC_v15 GA_pp_MC_AOD  ?_GammaConvCalo -mergetrains  -RL_listPCMEDC $1
    # ### LHC17 MC
    # bash DownScript.sh 3110 3111 -Name_LHC17MC_v15 GA_pp_MC_AOD  ?_GammaConvCalo -mergetrains  -RL_listPCMEDC $1
    # ### LHC18 MC
#     # bash DownScript.sh 3112 3113 -Name_LHC18MC_v15 GA_pp_MC_AOD  ?_GammaConvCalo -mergetrains  -RL_listPCMEDC $1
#     #################   merging   #############################
#     ### Data 16 + 17 + 18
#     bash DownScript.sh 1563 1562 1561 -Name_Data_pp13TeV_v15 GA_pp_AOD ?_GammaConvCalo -mergetrains -RL_listPCMEDC -RL_listDPGEDCtrigger -noDown $1
#     ### MC 16 + 17 + 18
#     bash DownScript.sh  3113 3112 3111 3110 3109 3108 3107 -Name_MC_pp13TeV_v15 GA_pp_MC_AOD ?_GammaConvCalo -mergetrains  -RL_listPCMEDC -RL_listDPGEDCtrigger -noDown $1
# #
# #     # 3119 	vAN-20201006_ROOT6-1 	PYT8JJ_13TeV_anchLHC18_AOD 	2020 Oct 17, Train run finished 	request Adrian
#     # 3118 	vAN-20201006_ROOT6-1 	PYT8JJ_13TeV_anchLHC17_AOD 	2020 Oct 17, Train run finished 	request Adrian
#     # 3117 	vAN-20201006_ROOT6-1 	PYT8JJ_13TeV_anchLHC16_AOD209 	2020 Oct 17, Train run finished 	request Adrian
#    ##############   JJ MC   #############################
#    ### LHC16 MC
#    bash DownScript.sh 3117 -Name_JJLHC16MC_v15 GA_pp_MC_AOD  ?_GammaConvCalo -mergetrains -IsJJ $1
#    ### LHC17 MC
#    bash DownScript.sh 3118 -Name_JJLHC17MC_v15 GA_pp_MC_AOD   ?_GammaConvCalo -mergetrains -IsJJ $1
#    ### LHC18 MC
# #    bash DownScript.sh 3119 -Name_JJLHC18MC_v15 GA_pp_MC_AOD  ?_GammaConvCalo -mergetrains -IsJJ $1
# #    #################   merging   #############################
#    bash DownScript.sh 3119 3118 3117 -Name_JJMC_pp13TeV_v15 GA_pp_MC_AOD ?_GammaConvCalo -mergetrains  -RL_default -noDown $1
# #
#
#
#
#
#
#
#
# #
# #
# #
#  ########### NL, NCell, Rbins   v14  + QA ##############
# ##############   Data   #########################
#
# 1551 	vAN-20200924_ROOT6-1 	LHC18_13TeV_pass1_AOD208 	2020 Sep 25, Train run: All jobs submitted 	request Adrian
# 1550 	vAN-20200924_ROOT6-1 	LHC17_13TeV_pass1_AOD208 	2020 Sep 25, Train run: All jobs submitted 	request Adrian
# 1549 	vAN-20200924_ROOT6-1 	LHC16_13TeV_pass1_2_AOD208 	2020 Sep 25, Train run: All jobs submitted 	request Adrian
# ### LHC16 Data
# bash DownScript.sh 1549 -Name_LHC16Data_v14 GA_pp_AOD  ?_GammaConvCalo_20  -RL_listPCMEDC -RL_listDPGEDCtrigger $1
# ### LHC17 Data
# bash DownScript.sh 1550 -Name_LHC17Data_v14 GA_pp_AOD  ?_GammaConvCalo_20  -RL_listPCMEDC -RL_listDPGEDCtrigger $1
# ### LHC18 Data
# bash DownScript.sh 1551 -Name_LHC18Data_v14 GA_pp_AOD  ?_GammaConvCalo_20  -RL_listPCMEDC -RL_listEDC_trigger $1
# ln -sf "$gridpath/LHC18Data_v14/listEDC_trigger" "$gridpath/LHC18Data_v14/listDPGEDCtrigger"
# ln -sf "$gridpath/.GA_pp_AOD-1551/listEDC_trigger" "$gridpath/.GA_pp_AOD-1551/listDPGEDCtrigger"
# ##################   MC   #############################
# # 3034 	vAN-20200924_ROOT6-1 	PYT8_13TeV_anchLHC18_AOD_extra 	2020 Sep 25, Train run finished 	request Adrian
# # 3033 	vAN-20200924_ROOT6-1 	PYT8_13TeV_anchLHC18_AOD209 	2020 Sep 25, Train run finished 	request Adrian
# # 3032 	vAN-20200924_ROOT6-1 	PYT8_13TeV_anchLHC17_AOD209_extra 	2020 Sep 25, Train run: All jobs submitted 	request Adrian
# # 3031 	vAN-20200924_ROOT6-1 	PYT8_13TeV_anchLHC17_AOD209 	2020 Sep 25, Train run finished 	request Adrian
# # 3030 	vAN-20200924_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD209_extra2 	2020 Sep 25, Train run finished 	request Adrian
# # 3029 	vAN-20200924_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD209_extra 	2020 Sep 25, Train run finished 	request Adrian
# # 3028 	vAN-20200924_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD209 	2020 Sep 25, Train run finished 	request Adrian
# ### LHC16 MC
# bash DownScript.sh 3030 3029 3028 -Name_LHC16MC_v14 GA_pp_MC_AOD  ?_GammaConvCalo_20 -mergetrains  -RL_listPCMEDC $1
# ### LHC17 MC
# bash DownScript.sh 3032 3031 -Name_LHC17MC_v14 GA_pp_MC_AOD  ?_GammaConvCalo_20 -mergetrains  -RL_listPCMEDC $1
# ### LHC18 MC
# bash DownScript.sh 3034 3033 -Name_LHC18MC_v14 GA_pp_MC_AOD  ?_GammaConvCalo_20 -mergetrains  -RL_listPCMEDC $1
# #################   merging   #############################
# # ### Data 16 + 17 + 18
# bash DownScript.sh 1551 1550 1549 -Name_Data_pp13TeV_v14 GA_pp_AOD  ?_GammaConvCalo ?_GammaCaloMix -mergetrains -RL_listPCMEDC -RL_listDPGEDCtrigger -noDown $1
# # ### MC 16 + 17 + 18
# bash DownScript.sh  3034 3033 3032 3031 3030 3029 3028 -Name_MC_pp13TeV_v14 GA_pp_MC_AOD  ?_GammaConvCalo ?_GammaCaloMix -mergetrains  -RL_listPCMEDC -noDown $1
# #
# #
# # ####################    QA   #############################
# # ### LHC16 Data
# bash DownScript.sh 1549 -Name_LHC16Data_v14 GA_pp_AOD ?_GammaConvCalo_2001 -onlyrunwise  -RL_listDPGEDC -RL_listDPG -RL_listDPGIncAcc -RL_listPCMEDC $1
# ln -sf "$gridpath/LHC16Data_v14/listDPGIncAcc" "$gridpath/LHC16Data_v14/listDPGIncTPC"
# ### LHC17 Data
# bash DownScript.sh 1550 -Name_LHC17Data_v14 GA_pp_AOD ?_GammaConvCalo_2001 -onlyrunwise  -RL_listDPGEDC -RL_listDPG -RL_listDPGIncTPC -RL_listPCMEDC $1
# ### LHC18 Datas
# bash DownScript.sh 1551 -Name_LHC18Data_v14 GA_pp_AOD ?_GammaConvCalo_2001 -onlyrunwise -RL_listDPGEMC -RL_listDPG -RL_listDPGIncTPC $1
# # rm -r "$gridpath/LHC18Data_v14/listDPGEDC"
# ln -sf "$gridpath/LHC18Data_v14/listDPGEMC" "$gridpath/LHC18Data_v14/listDPGEDC"
# # rm -r "$gridpath/.GA_pp_AOD-1551/listDPGEDC"
# ln -sf "$gridpath/.GA_pp_AOD-1551/listDPGEMC" "$gridpath/.GA_pp_AOD-1551/listDPGEDC"
# ### LHC16 MC
# bash DownScript.sh 3030 3029 3028 -Name_LHC16MC_v14 GA_pp_MC_AOD ?_GammaConvCalo_2001 -onlyrunwise -mergeruns  -RL_listDPGEDC -RL_listDPG -RL_listDPGIncAccTPC -RL_listPCMEDC $1
# ln -sf "$gridpath/LHC16MC_v14/listDPGIncAccTPC" "$gridpath/LHC16MC_v14/listDPGIncTPC"
# ### LHC17 MC
# bash DownScript.sh 3032 3031 -Name_LHC17MC_v14 GA_pp_MC_AOD ?_GammaConvCalo_2001 -onlyrunwise -mergeruns  -RL_listDPGEDC -RL_listDPG -RL_listDPGIncTPC -RL_listPCMEDC $1
# ### LHC18 MC
# bash DownScript.sh 3034 3033 -Name_LHC18MC_v14 GA_pp_MC_AOD ?_GammaConvCalo_2001 -onlyrunwise -mergeruns  -RL_listDPGEMC -RL_listDPG -RL_listPCMEDC -RL_listDPCIncAccTPC $1
# ln -sf "$gridpath/LHC18MC_v14/listDPCIncAccTPC" "$gridpath/LHC18MC_v14/listDPGIncTPC"
# # rm -r "$gridpath/LHC18MC_v14/listDPGEDC"
# ln -sf "$gridpath/LHC18MC_v14/listDPGEMC" "$gridpath/LHC18MC_v14/listDPGEDC"
# # rm -r "$gridpath/.GA_pp_MC_AOD-3033/listDPGEDC"
# ln -sf "$gridpath/.GA_pp_MC_AOD-3033/listDPGEMC" "$gridpath/.GA_pp_MC_AOD-3033/listDPGEDC"
# # rm -r "$gridpath/.GA_pp_MC_AOD-3034/listDPGEDC"
# ln -sf "$gridpath/.GA_pp_MC_AOD-3034/listDPGEMC" "$gridpath/.GA_pp_MC_AOD-3034/listDPGEDC"
#
# ### Data 16 + 17 + 18
# bash DownScript.sh 1551 1550 1549 -Name_Data_pp13TeV_v14 GA_pp_AOD ?_GammaConvCalo_2001 -mergetrains -RL_listDPGEDC -RL_listDPGEMC -RL_listDPG -RL_listDPGIncTPC $1
# ### MC 16 + 17 + 18
# bash DownScript.sh  3034 3033 3032 3031 3030 3029 3028 -Name_MC_pp13TeV_v14 GA_pp_MC_AOD ?_GammaConvCalo_2001 -mergetrains  -RL_listDPGEDC -RL_listDPGEMC -RL_listDPG -RL_listPCMEDC -RL_listDPCIncAccTPC $1

########################    Photon QA   #############################
# ### LHC16 Data
# bash DownScript.sh 1549 -Name_LHC16Data_PhotonQA_new GA_pp_AOD ?_AnalysisResults -onlyrunwise -RL_listDPGIncAcc -runwise $1 # -RL_listDPG -RL_listPCM
# ln -sf "$gridpath/LHC16Data_v14/listDPGIncAcc" "$gridpath/LHC16Data_v14/listDPGIncTPC"
# ### LHC17 Data
# bash DownScript.sh 1550 -Name_LHC17Data_PhotonQA_new GA_pp_AOD ?_AnalysisResults -onlyrunwise -RL_listDPGIncTPC -runwise $1 # -RL_listDPG -RL_listPCM
# ### LHC18 Data
# bash DownScript.sh 1551 -Name_LHC18Data_PhotonQA_new GA_pp_AOD ?_AnalysisResults -onlyrunwise -RL_listDPGIncTPC -runwise $1 # -RL_listDPG -RL_listPCM
# ### LHC16 MC
# bash DownScript.sh 3030 3029 3028 -Name_LHC16MC_PhotonQA_new GA_pp_MC_AOD ?_AnalysisResults -onlyrunwise -mergeruns -RL_listDPGIncAccTPC -runwise $1 # -RL_listDPG -RL_listPCM
# ln -sf "$gridpath/LHC16MC_v14/listDPGIncAccTPC" "$gridpath/LHC16MC_v14/listDPGIncTPC"
# ### LHC17 MC
# bash DownScript.sh 3032 3031 -Name_LHC17MC_PhotonQA_new GA_pp_MC_AOD ?_AnalysisResults -onlyrunwise -mergeruns -RL_listDPGIncTPC -runwise $1 # -RL_listDPG -RL_listPCM
# ### LHC18 MC
# bash DownScript.sh 3034 3033 -Name_LHC18MC_PhotonQA_new GA_pp_MC_AOD ?_AnalysisResults -onlyrunwise -mergeruns -RL_listDPCIncAccTPC -runwise $1 # -RL_listDPG -RL_listPCM
# ln -sf "$gridpath/LHC18MC_v14/listDPCIncAccTPC" "$gridpath/LHC18MC_v14/listDPGIncTPC"





    #     # 2839-2841 (LowB D5TeVNchMBW)
    # # 2841 	vAN-20200718_ROOT6-1 	LHC18h1_PYT8_13TeV_ancLHC18c_LowB_AOD213 	2020 Jul 20, Train run: All jobs submitted 	request Ana, Joshua MBW
    # # 2840 	vAN-20200718_ROOT6-1 	LHC17h3_PYT8_13TeV_anchLHC17g_LowB_AOD213 	2020 Jul 20, Train run: All jobs submitted 	request Ana, Joshua MBW
    # # 2839 	vAN-20200718_ROOT6-1 	LHC17d1_PYT8_13TeV_ancLHC16f_LowB_AOD213 	2020 Jul 20, Train run: All jobs submitted 	request Ana, Joshua MBW
    # ##### low B
    # ##################   MC   #############################
    # ### LHC16 MC
    # bash DownScript.sh 2839 -Name_LHC16MC_lowB_v4_2D5TeVNchMBW GA_pp_MC_AOD  ?_GammaConvCalo -RL_lowB_DPGTrackIncAccTPCandEMC $1
    # ln -sf "$gridpath/LHC16MC_lowB_v4_2D5TeVNchMBW/lowB_DPGTrackIncAccTPCandEMC" "$gridpath/LHC16MC_lowB_v4_2D5TeVNchMBW/listDPGTrackIncAccTPCandEMC"
    # ln -sf "$gridpath/.GA_pp_MC_AOD-2839/lowB_DPGTrackIncAccTPCandEMC" "$gridpath/.GA_pp_MC_AOD-2839/listDPGTrackIncAccTPCandEMC"
    # ### LHC17 MC
    # bash DownScript.sh 2840 -Name_LHC17MC_lowB_v4_2D5TeVNchMBW GA_pp_MC_AOD  ?_GammaConvCalo -RL_listDPGTrackIncAccTPCandEMC $1
    # # ### LHC18 MC
    # # bash DownScript.sh 2841 -Name_LHC18MC_lowB_v4_2D5TeVNchMBW GA_pp_MC_AOD  ?_GammaConvCalo -mergetrains  -RL_listDPGTrackIncAccTPCandEMC $1
    # bash DownScript.sh  2839 2840 2841 -Name_MC_pp13TeV_lowB_v4_2D5TeVNchMBW GA_pp_MC_AOD ?_GammaConvCalo -mergetrains -RL_listDPGTrackIncAccTPCandEMC -noDown $1
    # # #

    # ################   NCell variation and New M02, E/p, timing  ##############
    # ################   Data   #########################
    # # 1534 	vAN-20200910_ROOT6-1 	LHC18_13TeV_pass1_AOD208 	2020 Sep 12, Train run: All jobs submitted 	request Steven, Adrian
    # # 1533 	vAN-20200910_ROOT6-1 	LHC17_13TeV_pass1_AOD208 	2020 Sep 12, Train run: All jobs submitted 	request Steven, Adrian
    # # 1532 	vAN-20200910_ROOT6-1 	LHC16_13TeV_pass1_2_AOD208 	2020 Sep 12, Train run: All jobs submitted 	request Steven, Adrian
    # ### LHC16 Data
    # bash DownScript.sh 1532 -Name_LHC16Data_v13 GA_pp_AOD  ?_GammaConvCalo_20  -RL_listPCMEDC $1
    # # ### LHC17 Data
    # bash DownScript.sh 1533 -Name_LHC17Data_v13 GA_pp_AOD  ?_GammaConvCalo_20  -RL_listPCMEDC $1
    # # ### LHC18 Data
    # bash DownScript.sh 1534 -Name_LHC18Data_v13 GA_pp_AOD  ?_GammaConvCalo_20  -RL_listDPGEMC $1
    # ln -sf "$gridpath/LHC18Data_v13/listDPGEMC" "$gridpath/LHC18Data_v13/listPCMEDC"
    # ln -sf "$gridpath/.GA_pp_AOD-1534/listDPGEMC" "$gridpath/.GA_pp_AOD-1534/listPCMEDC"
    # ##################   MC   #############################
    # # 2978 	vAN-20200910_ROOT6-1 	PYT8_13TeV_anchLHC18_AOD_extra 	2020 Sep 12, Train run: All jobs submitted 	request Steven, Adrian
    # # 2977 	vAN-20200910_ROOT6-1 	PYT8_13TeV_anchLHC18_AOD209 	2020 Sep 12, Train run finished 	request Steven, Adrian
    # # 2976 	vAN-20200910_ROOT6-1 	PYT8_13TeV_anchLHC17_AOD209_extra 	2020 Sep 12, Train run finished 	request Steven, Adrian
    # # 2975 	vAN-20200910_ROOT6-1 	PYT8_13TeV_anchLHC17_AOD209 	2020 Sep 12, Train run finished 	request Steven, Adrian
    # # 2974 	vAN-20200910_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD209_extra2 	2020 Sep 12, Train run: All jobs submitted 	request Steven, Adrian
    # # 2973 	vAN-20200910_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD209_extra 	2020 Sep 12, Train run finished 	request Steven, Adrian
    # # 2972 	vAN-20200910_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD209 	2020 Sep 12, Train run finished 	request Steven, Adrian
    # ### LHC16 MC
    # bash DownScript.sh 2974 2973 2972 -Name_LHC16MC_v13 GA_pp_MC_AOD  ?_GammaConvCalo_20 -mergetrains  -RL_listPCMEDC $1
    # ### LHC17 MC
    # bash DownScript.sh 2976 2975 -Name_LHC17MC_v13 GA_pp_MC_AOD  ?_GammaConvCalo_20 -mergetrains  -RL_listPCMEDC $1
    # ### LHC18 MC
    # bash DownScript.sh 2978 2977 -Name_LHC18MC_v13 GA_pp_MC_AOD  ?_GammaConvCalo_20 -mergetrains  -RL_listDPGEMC $1
    # ln -sf "$gridpath/LHC18MC_v13/listDPGEMC" "$gridpath/LHC18MC_v14/listPCMEDC"
    # ln -sf "$gridpath/.GA_pp_MC_AOD-2977/listDPGEMC" "$gridpath/.GA_pp_MC_AOD-2977/listPCMEDC"
    # ln -sf "$gridpath/.GA_pp_MC_AOD-2978/listDPGEMC" "$gridpath/.GA_pp_MC_AOD-2978/listPCMEDC"
    # # #################   merging   #############################
    # # ### Data 16 + 17 + 18
    # bash DownScript.sh 1532 1533 1534 -Name_Data_pp13TeV_v13 GA_pp_AOD ?_GammaConvCalo ?_GammaCaloMix -mergetrains -RL_listPCMEDC -noDown $1
    # # ### MC 16 + 17 + 18
    # bash DownScript.sh  2972 2973 2974 2975 2976 2977 2978 -Name_MC_pp13TeV_v13 GA_pp_MC_AOD ?_GammaConvCalo ?_GammaCaloMix -mergetrains  -RL_listPCMEDC -noDown $1


    #
    # ################   Data   #############################
    # ### LHC16 Data
    # bash DownScript.sh 1495 -Name_LHC16Data_SigmaPlus GA_pp_AOD ?_SigmaPlus  -RL_listPHOS $1
    # ### LHC17 Data
    # bash DownScript.sh 1496 -Name_LHC17Data_SigmaPlus GA_pp_AOD ?_SigmaPlus  -RL_listPHOS $1
    # ### LHC18 Data
    # bash DownScript.sh 1497 -Name_LHC18Data_SigmaPlus GA_pp_AOD ?_SigmaPlus  -RL_listPHOS $1
    # # ##################   MC   #############################
    # ### LHC16 MC
    # bash DownScript.sh 2899 2898 2897 -Name_LHC16MC_SigmaPlus GA_pp_MC_AOD ?_SigmaPlus -mergetrains  -RL_listPHOS $1
    # ### LHC17 MC
    # bash DownScript.sh 2900 2901 -Name_LHC17MC_SigmaPlus GA_pp_MC_AOD ?_SigmaPlus -mergetrains  -RL_listPHOS $1
    # ### LHC18 MC
    # bash DownScript.sh 2902 2903 -Name_LHC18MC_SigmaPlus GA_pp_MC_AOD ?_SigmaPlus -mergetrains  -RL_listPHOS $1
    # # #################   merging   #############################
    # # ### Data 16 + 17 + 18
    # bash DownScript.sh 1495 1496 1497 -Name_Data_pp13TeV_SigmaPlus GA_pp_AOD ?_SigmaPlus -mergetrains -RL_listPHOS -noDown $1
    # # ### MC 16 + 17 + 18
    # bash DownScript.sh  2903 2902 2901 2900 2899 2898 2897 -Name_MC_pp13TeV_SigmaPlus GA_pp_MC_AOD ?_SigmaPlus -mergetrains  -RL_listPHOS -noDown $1
    # #
    #
    #






################   Data   #############################
# 1497 	vAN-20200811_ROOT6-1 	LHC18_13TeV_pass1_AOD208 	2020 Aug 13, Train run: All jobs submitted 	request Florian, Adrian, ...
# 1496 	vAN-20200811_ROOT6-1 	LHC17_13TeV_pass1_AOD208 	2020 Aug 13, Train run: All jobs submitted 	request Florian, Adrian, ...
# 1495 	vAN-20200811_ROOT6-1 	LHC16_13TeV_pass1_2_AOD208 	2020 Aug 13, Train run: All jobs submitted 	request Florian, Adrian &...
# ### LHC16 Data
# bash DownScript.sh 1495 -Name_LHC16Data_v12 GA_pp_AOD  ?_GammaConvCalo_20  -RL_listPCMEDC $1
# ### LHC17 Databck
# bash DownScript.sh 1496 -Name_LHC17Data_v12 GA_pp_AOD  ?_GammaConvCalo_20  -RL_listPCMEDC $1
# ### LHC18 Data
# bash DownScript.sh 1497 -Name_LHC18Data_v12 GA_pp_AOD  ?_GammaConvCalo_20  -uSR_DPGEDC $1
# ln -sf "$gridpath/LHC18Data_v12/DPGEDC" "$gridpath/LHC18Data_v12/listPCMEDC"
# ln -sf "$gridpath/.GA_pp_AOD-1497/DPGEDC" "$gridpath/.GA_pp_AOD-1497/listPCMEDC"# ln -sf "$gridpath/LHC18Data_v10/listDPGIncTPC" "$gridpath/LHC18Data_v10/listPCMEDC"
# ln -sf "$gridpath/.GA_pp_AOD-1497/DPGEDC" "$gridpath/.GA_pp_AOD-1497/listPCMEDC"
# ##################   MC   #############################
# 2903 	vAN-20200811_ROOT6-1 	PYT8_13TeV_anchLHC18_AOD_extra 	2020 Aug 13, Train run: All jobs submitted 	request Florian, Steven, ...
# 2902 	vAN-20200811_ROOT6-1 	PYT8_13TeV_anchLHC18_AOD209 	2020 Aug 13, Train run: All jobs submitted 	request Florian Steven, A...
# 2901 	vAN-20200811_ROOT6-1 	PYT8_13TeV_anchLHC17_AOD209_extra 	2020 Aug 13, Train run: All jobs submitted 	request Florian, Steven,
# 2900 	vAN-20200811_ROOT6-1 	PYT8_13TeV_anchLHC17_AOD209 	2020 Aug 13, Train run: All jobs submitted 	request Florian, Steven, ...
# 2899 	vAN-20200811_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD209_extra2 	2020 Aug 13, Train run: All jobs submitted 	request Florian, Adrian,
# 2898 	vAN-20200811_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD209_extra 	2020 Aug 13, Train run: All jobs submitted 	request Florian, Adrian,
# 2897 	vAN-20200811_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD209 	2020 Aug 13, Train run: All jobs submitted 	request Florian, Adrian &...
# ### LHC16 MC
# bash DownScript.sh 2899 2898 2897 -Name_LHC16MC_v12 GA_pp_MC_AOD  ?_GammaConvCalo_20 -mergetrains  -RL_listPCMEDC $1
# ### LHC17 MC
# bash DownScript.sh 2900 2901 -Name_LHC17MC_v12 GA_pp_MC_AOD  ?_GammaConvCalo_20 -mergetrains  -RL_listPCMEDC $1
# ### LHC18 MCbck
# bash DownScript.sh 2902 2903 -Name_LHC18MC_v12 GA_pp_MC_AOD  ?_GammaConvCalo_20 -mergetrains  -uSR_DPGEDC $1
# ln -sf "$gridpath/LHC18MC_v12/DPGEDC" "$gridpath/LHC18MC_v12/listPCMEDC"
# ln -sf "$gridpath/.GA_pp_MC_AOD-2902/DPGEDC" "$gridpath/.GA_pp_MC_AOD-2902/listPCMEDC"
# ln -sf "$gridpath/.GA_pp_MC_AOD-2903/DPGEDC" "$gridpath/.GA_pp_MC_AOD-2903/listPCMEDC"
# #################   merging   #############################
# # ## Data 16 + 17 + 18
# bash DownScript.sh 1495 1496 1497 -Name_Data_pp13TeV_v12 GA_pp_AOD ?_GammaConvCalo ?_GammaCaloMix -mergetrains -RL_listPCMEDC -noDown $1
# # ## MC 16 + 17 + 18
# bash DownScript.sh  2903 2902 2901 2900 2899 2898 2897 -Name_MC_pp13TeV_v12 GA_pp_MC_AOD ?_GammaConvCalo ?_GammaCaloMix -mergetrains  -RL_listPCMEDC -noDown $1
# #
#########################################################################################################################################
#########################################################################################################################################

    # 1494 	vAN-20200805_ROOT6-1 	LHC18_13TeV_pass1_AOD208 	2020 Aug 07, Train run finished 	Jens (PCM-)PHOS, Adrian Calo
    # 1493 	vAN-20200805_ROOT6-1 	LHC17_13TeV_pass1_AOD208 	2020 Aug 07, Train run finished 	Jens (PCM-)PHOS, Adrian Calo
    # 1492 	vAN-20200805_ROOT6-1 	LHC16_13TeV_pass1_2_AOD208 	2020 Aug 07, Train run finished 	Jens (PCM-)PHOS, Adrian Calo
    # #################   Data   #############################
    # ### LHC16 Data
    # bash DownScript.sh 1492 -Name_LHC16Data_TimeEffi GA_pp_AOD ?_GammaCalo_2039  -RL_listPCMEDC $1
    # ### LHC17 Data
    # bash DownScript.sh 1493 -Name_LHC17Data_TimeEffi GA_pp_AOD ?_GammaCalo_2039  -RL_listPCMEDC $1
    # ### LHC18 Data
    # bash DownScript.sh 1494 -Name_LHC18Data_TimeEffi GA_pp_AOD ?_GammaCalo_2039  -uSR_DPGEDC $1
    # ln -sf "$gridpath/LHC18Data_TimeEffi/DPGEDC" "$gridpath/LHC18Data_TimeEffi/listPCMEDC"
    # ln -sf "$gridpath/.GA_pp_AOD-1494/DPGEDC" "$gridpath/.GA_pp_AOD-1494/listPCMEDC"
    # ### Data 16 + 17 + 18
    # bash DownScript.sh 1492 1493 1494 -Name_Data_pp13TeV_TimeEffi GA_pp_AOD ?_GammaCalo_2039 -mergetrains -RL_listPCMEDC -noDown $1


#
    # 2831-2833 (LowB D5TeVNchMBW)
    # 2833 	vAN-20200713_ROOT6-1 	LHC18h1_PYT8_13TeV_ancLHC18c_LowB_AOD213 	2020 Jul 14, Train run finished 	request Ana MBW
    # 2832 	vAN-20200713_ROOT6-1 	LHC17h3_PYT8_13TeV_anchLHC17g_LowB_AOD213 	2020 Jul 14, Train run finished 	request Ana MBW
    # 2831 	vAN-20200713_ROOT6-1 	LHC17d1_PYT8_13TeV_ancLHC16f_LowB_AOD213 	2020 Jul 14, Train run finished 	request Ana MBW
    #
    # ##### low B
    # ##################   MC   #############################
    # ### LHC16 MC
    # bash DownScript.sh 2831 -Name_LHC16MC_lowB_v3_2D5TeVNchMBW GA_pp_MC_AOD  ?_GammaConvCalo -RL_lowB_DPGTrackIncAccTPCandEMC $1
    # ln -sf "$gridpath/LHC16MC_lowB_v3_2D5TeVNchMBW/lowB_DPGTrackIncAccTPCandEMC" "$gridpath/LHC16MC_lowB_v3_2D5TeVNchMBW/listDPGTrackIncAccTPCandEMC"
    # ln -sf "$gridpath/.GA_pp_MC_AOD-2831/lowB_DPGTrackIncAccTPCandEMC" "$gridpath/.GA_pp_MC_AOD-2831/listDPGTrackIncAccTPCandEMC"
    # ### LHC17 MC
    # bash DownScript.sh 2832 -Name_LHC17MC_lowB_v3_2D5TeVNchMBW GA_pp_MC_AOD  ?_GammaConvCalo -RL_listDPGTrackIncAccTPCandEMC $1
    # ### LHC18 MC
    # bash DownScript.sh 2833 -Name_LHC18MC_lowB_v3_2D5TeVNchMBW GA_pp_MC_AOD  ?_GammaConvCalo -mergetrains  -RL_listDPGTrackIncAccTPCandEMC $1
    # bash DownScript.sh  2831 2832 2833 -Name_MC_pp13TeV_lowB_v3_2D5TeVNchMBW GA_pp_MC_AOD ?_GammaConvCalo -mergetrains -RL_listDPGTrackIncAccTPCandEMC -noDown $1
    #
#
#

    # 2856-2858 LowB 2D13TeVNChMBW
    # 2858 	vAN-20200722_ROOT6-1 	LHC18h1_PYT8_13TeV_ancLHC18c_LowB_AOD213 	2020 Jul 24, Train run finished 	request Ana MBW
    # 2857 	vAN-20200722_ROOT6-1 	LHC17h3_PYT8_13TeV_anchLHC17g_LowB_AOD213 	2020 Jul 24, Train run finished 	request Ana MBW
    # 2856 	vAN-20200722_ROOT6-1 	LHC17d1_PYT8_13TeV_ancLHC16f_LowB_AOD213 	2020 Jul 24, Train run finished 	request Ana MBW
    # ##### low B
#     ##################   MC   #############################
#     ### LHC16 MC
#     bash DownScript.sh 2856 -Name_LHC16MC_lowB_v3_2D13TeVNChMBW GA_pp_MC_AOD  ?_GammaConvCalo -RL_lowB_DPGTrackIncAccTPCandEMC $1
#     ln -sf "$gridpath/LHC16MC_lowB_v3_2D13TeVNChMBW/lowB_DPGTrackIncAccTPCandEMC" "$gridpath/LHC16MC_lowB_v3_2D13TeVNChMBW/listDPGTrackIncAccTPCandEMC"
#     ln -sf "$gridpath/.GA_pp_MC_AOD-2856/lowB_DPGTrackIncAccTPCandEMC" "$gridpath/.GA_pp_MC_AOD-2856/listDPGTrackIncAccTPCandEMC"
#     ### LHC17 MC
#     bash DownScript.sh 2857 -Name_LHC17MC_lowB_v3_2D13TeVNChMBW GA_pp_MC_AOD  ?_GammaConvCalo -RL_listDPGTrackIncAccTPCandEMC $1
#     ### LHC18 MC
#     bash DownScript.sh 2858 -Name_LHC18MC_lowB_v3_2D13TeVNChMBW GA_pp_MC_AOD  ?_GammaConvCalo -mergetrains  -RL_listDPGTrackIncAccTPCandEMC $1
    # bash DownScript.sh  2856 2857 2858 -Name_MC_pp13TeV_lowB_v3_2D13TeVNChMBW GA_pp_MC_AOD ?_GammaConvCalo -mergetrains -RL_listDPGTrackIncAccTPCandEMC -noDown $1
#     #
# #
#
#
# # ######################################################################################################
# # 2849-2855 (NomB 2D13TeVNchMBW)
# # 2855 	vAN-20200722_ROOT6-1 	PYT8_13TeV_anchLHC18_AOD_extra 	2020 Jul 24, Train run finished 	request Ana MBW
# # 2854 	vAN-20200722_ROOT6-1 	PYT8_13TeV_anchLHC18_AOD209 	2020 Jul 24, Train run finished 	request Ana MBW
# # 2853 	vAN-20200722_ROOT6-1 	PYT8_13TeV_anchLHC17_AOD209_extra 	2020 Jul 24, Train run finished 	request Ana MBW
# # 2852 	vAN-20200722_ROOT6-1 	PYT8_13TeV_anchLHC17_AOD209 	2020 Jul 24, Train run finished 	request Ana MBW
# # 2851 	vAN-20200722_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD209_extra2 	2020 Jul 24, Train run finished 	request Ana MBW
# # 2850 	vAN-20200722_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD209_extra 	2020 Jul 24, Train run finished 	request Ana MBW
# # 2849 	vAN-20200722_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD209 	2020 Jul 24, Train run finished 	request Ana MBW
#         ##################   MC   #############################
#         ### LHC16 MC
#         bash DownScript.sh 2849 2850 2851 -Name_LHC16MC_v11_2D13TeVNchMBW GA_pp_MC_AOD  ?_GammaConvCalo -mergetrains  -RL_listPCMEDC $1
#         ### LHC17 MC
#         bash DownScript.sh 2852 2853 -Name_LHC17MC_v11_2D13TeVNchMBW GA_pp_MC_AOD  ?_GammaConvCalo -mergetrains  -RL_listPCMEDC $1
#         ### LHC18 MC
#         # bash DownScript.sh 2705 2707 -Name_LHC18MC_v10 GA_pp_MC_AOD  ?_GammaConvCalo  -mergetrains  -RL_listDPCIncAccTPC $1
#         bash DownScript.sh 2854 2854 -Name_LHC18MC_v11_2D13TeVNchMBW GA_pp_MC_AOD  ?_GammaConvCalo -mergetrains  -uSR_DPGEDC $1
#         # ln -sf "$gridpath/LHC18MC_v10/listDPCIncAccTPC" "$gridpath/LHC18MC_v10/listPCMEDC"
#         # ln -sf "$gridpath/.GA_pp_MC_AOD-2705/listDPCIncAccTPC" "$gridpath/.GA_pp_MC_AOD-2705/listPCMEDC"
#         # ln -sf "$gridpath/.GA_pp_MC_AOD-2707/listDPCIncAccTPC" "$gridpath/.GA_pp_MC_AOD-2707/listPCMEDC"
#         ln -sf "$gridpath/LHC18MC_v11_2D13TeVNchMBW/DPGEDC" "$gridpath/LHC18MC_v11_2D13TeVNchMBW/listPCMEDC"
#         ln -sf "$gridpath/.GA_pp_MC_AOD-2854/DPGEDC" "$gridpath/.GA_pp_MC_AOD-2854/listPCMEDC"
#         ln -sf "$gridpath/.GA_pp_MC_AOD-2855/DPGEDC" "$gridpath/.GA_pp_MC_AOD-2855/listPCMEDC"
# #         #         #################   merging   #############################
# #         #         # ### Data 16 + 17 + 18
#                 bash DownScript.sh 1378 1379 1380 -Name_Data_pp13TeV_v10 GA_pp_AOD ?_GammaConvCalo ?_GammaCaloMix -mergetrains -RL_listPCMEDC -noDown $1
# #         #         # ### MC 16 + 17 + 18
#                 bash DownScript.sh  2849 2850 2851 2852 2853 2854 2855 -Name_MC_pp13TeV_v11_2D13TeVNchMBW GA_pp_MC_AOD ?_GammaConvCalo -mergetrains  -RL_listPCMEDC -noDown $1
#
#
# # ######################################################################################################
# # 2824-2830 (Nom B 2D5TeVNchMBW)
# # 2830 	vAN-20200710_ROOT6-1 	PYT8_13TeV_anchLHC18_AOD_extra 	2020 Jul 13, Train run finished 	request Ana MBW
# # 2829 	vAN-20200710_ROOT6-1 	PYT8_13TeV_anchLHC18_AOD209 	2020 Jul 13, Train run finished 	request Ana MBW
# # 2828 	vAN-20200710_ROOT6-1 	PYT8_13TeV_anchLHC17_AOD209_extra 	2020 Jul 13, Train run finished 	request Ana MBW
# # 2827 	vAN-20200710_ROOT6-1 	PYT8_13TeV_anchLHC17_AOD209 	2020 Jul 13, Train run finished 	request Ana MBW
# # 2826 	vAN-20200710_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD209_extra2 	2020 Jul 13, Train run finished 	request Ana MBW
# # 2825 	vAN-20200710_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD209_extra 	2020 Jul 13, Train run finished 	request Ana MBW
# # 2824 	vAN-20200710_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD209 	2020 Jul 13, Train run finished 	request Ana MBW
# #         ##################   MC   #############################
#         ### LHC16 MC
#         bash DownScript.sh 2824 2825 2826 -Name_LHC16MC_v11_2D5TeVNchMBW GA_pp_MC_AOD  ?_GammaConvCalo -mergetrains  -RL_listPCMEDC $1
#         ### LHC17 MC
#         bash DownScript.sh 2827 2828 -Name_LHC17MC_v11_2D5TeVNchMBW GA_pp_MC_AOD  ?_GammaConvCalo -mergetrains  -RL_listPCMEDC $1
#         ### LHC18 MC
#         bash DownScript.sh 2829 2830 -Name_LHC18MC_v11_2D5TeVNchMBW GA_pp_MC_AOD  ?_GammaConvCalo -mergetrains  -uSR_DPGEDC $1
#         # bash DownScript.sh 2829 2830 -Name_LHC18MC_v11_2D5TeVNchMBW GA_pp_MC_AOD  ?_GammaConvCalo -mergetrains  -RL_listDPCIncAccTPC $1
#         # ln -sf "$gridpath/LHC18MC_v11_2D5TeVNchMBW/listDPCIncAccTPC" "$gridpath/LHC18MC_v11_2D5TeVNchMBW/listPCMEDC"
#         # ln -sf "$gridpath/.GA_pp_MC_AOD-2829/listDPCIncAccTPC" "$gridpath/.GA_pp_MC_AOD-2829/listPCMEDC"
#         # ln -sf "$gridpath/.GA_pp_MC_AOD-2830/listDPCIncAccTPC" "$gridpath/.GA_pp_MC_AOD-2830/listPCMEDC"
#         ln -sf "$gridpath/LHC18MC_v11_2D5TeVNchMBW/DPGEDC" "$gridpath/LHC18MC_v11_2D5TeVNchMBW/listPCMEDC"
#         ln -sf "$gridpath/.GA_pp_MC_AOD-2829/DPGEDC" "$gridpath/.GA_pp_MC_AOD-2829/listPCMEDC"
#         ln -sf "$gridpath/.GA_pp_MC_AOD-2830/DPGEDC" "$gridpath/.GA_pp_MC_AOD-2830/listPCMEDC"
# #         # ### MC 16 + 17 + 18
#         bash DownScript.sh  2824 2825 2826 2827 2828 2829 2830 -Name_MC_pp13TeV_v11_2D5TeVNchMBW GA_pp_MC_AOD ?_GammaConvCalo -mergetrains  -RL_listPCMEDC -noDown $1

######################################################################################################
######################################################################################################





# ##### low B
# ##################   MC   #############################
# ### LHC16 MC
# bash DownScript.sh 2744 -Name_LHC16MC_lowB_NL_v2 GA_pp_MC_AOD ?_GammaConvCalo_33 -RL_lowB_DPGTrackIncAccTPCandEMC $1
# ln -sf "$gridpath/LHC16MC_lowB_NL_v2/lowB_DPGTrackIncAccTPCandEMC" "$gridpath/LHC16MC_lowB_NL_v2/listDPGTrackIncAccTPCandEMC"
# ln -sf "$gridpath/.GA_pp_MC_AOD-2744/lowB_DPGTrackIncAccTPCandEMC" "$gridpath/.GA_pp_MC_AOD-2744/listDPGTrackIncAccTPCandEMC"
# ### LHC17 MC
# bash DownScript.sh 2745 -Name_LHC17MC_lowB_NL_v2 GA_pp_MC_AOD ?_GammaConvCalo_33 -RL_listDPGTrackIncAccTPCandEMC $1
# ### LHC18 MC
# bash DownScript.sh 2746 -Name_LHC18MC_lowB_NL_v2 GA_pp_MC_AOD ?_GammaConvCalo_33 -mergetrains  -RL_listDPGTrackIncAccTPCandEMC $1
# ##################   Data   #############################
# ### LHC16 Data
# bash DownScript.sh 1405 -Name_LHC16Data_lowB_NL_v2 GA_pp_AOD ?_GammaConvCalo_33  -RL_lowB_DPGTrackIncAccTPCandEMC $1
# ln -sf "$gridpath/LHC16Data_lowB_NL_v2/lowB_DPGTrackIncAccTPCandEMC" "$gridpath/LHC16MC_lowB_NL_v2/listDPGTrackIncAccTPCandEMC"
# ln -sf "$gridpath/.GA_pp_AOD-1405/lowB_DPGTrackIncAccTPCandEMC" "$gridpath/.GA_pp_AOD-1405/listDPGTrackIncAccTPCandEMC"
# ### LHC17 Data
# bash DownScript.sh 1406 -Name_LHC17Data_lowB_NL_v2 GA_pp_AOD ?_GammaConvCalo_33  -RL_listDPGTrackIncAccTPCandEMC $1
# ### LHC18 Data
# bash DownScript.sh 1407 -Name_LHC18Data_lowB_NL_v2 GA_pp_AOD ?_GammaConvCalo_33  -RL_listDPGTrackIncAccTPCandEMC $1
# # ##################   merging   #############################
# # ### Data 16 + 17 + 18
# bash DownScript.sh  1407 1406 1405 -Name_Data_pp13TeV_lowB GA_pp_AOD ?_GammaConvCalo -mergetrains -RL_listDPGTrackIncAccTPCandEMC -noDown $1
# # ### MC 16 + 17 + 18
# bash DownScript.sh  2746 2745 2744 -Name_MC_pp13TeV_lowB GA_pp_MC_AOD ?_GammaConvCalo -mergetrains -RL_listDPGTrackIncAccTPCandEMC -noDown $1



#
# ##################   AOD Validation   #############################
# ##################   Data   #############################
# bash DownScript.sh 1404 -Name_LHC18Data_lowB_AOD_validation GA_pp_AOD ?_GammaConvCalo_33  -RL_listDPGTrackIncAccTPCandEMC $1
# bash DownScript.sh 1403 -Name_LHC18Data_AOD_validation GA_pp_AOD ?_GammaConvCalo_2 ?_GammaCalo_2039  -RL_listDPGIncTPC $1
# ##################   MC   #############################
# bash DownScript.sh 2743 -Name_LHC18MC_lowB_AOD_validation GA_pp_MC_AOD ?_GammaConvCalo_33 -mergetrains  -RL_listDPGTrackIncAccTPCandEMC $1
# bash DownScript.sh 2742 -Name_LHC18MC_AOD_validation GA_pp_MC_AOD ?_GammaConvCalo_2 -mergetrains  -RL_listDPCIncAccTPC $1
#

    #
    # #################   Data   #############################
    # ### LHC16 Data
    # bash DownScript.sh 1378 -Name_LHC16Data_v10 GA_pp_AOD  ?_GammaConvCalo_20 ?_GammaCaloMix  -RL_listPCMEDC $1
    # ### LHC17 Data
    # bash DownScript.sh 1379 -Name_LHC17Data_v10 GA_pp_AOD  ?_GammaConvCalo_20 ?_GammaCaloMix  -RL_listPCMEDC $1
    # ### LHC18 Data
    # bash DownScript.sh 1380 -Name_LHC18Data_v10 GA_pp_AOD  ?_GammaConvCalo_20 ?_GammaCaloMix  -uSR_DPGEDC $1
    # bash DownScript.sh 1380 -Name_LHC18Data_v10 GA_pp_AOD  ?_GammaConvCalo_20 ?_GammaCaloMix  -RL_listDPGIncTPC $1
    # ln -sf "$gridpath/LHC18Data_v10/listDPGIncTPC" "$gridpath/LHC18Data_v10/listPCMEDC"
    # ln -sf "$gridpath/.GA_pp_AOD-1380/listDPGIncTPC" "$gridpath/.GA_pp_AOD-1380/listPCMEDC"# ln -sf "$gridpath/LHC18Data_v10/listDPGIncTPC" "$gridpath/LHC18Data_v10/listPCMEDC"
    # ln -sf "$gridpath/.GA_pp_AOD-1380/listDPGIncTPC" "$gridpath/.GA_pp_AOD-1380/listPCMEDC"
    # ln -sf "$gridpath/LHC18Data_v10/DPGEDC" "$gridpath/LHC18Data_v10/listPCMEDC"
    # ln -sf "$gridpath/.GA_pp_AOD-1380/DPGEDC" "$gridpath/.GA_pp_AOD-1380/listPCMEDC"# ln -sf "$gridpath/LHC18Data_v10/listDPGIncTPC" "$gridpath/LHC18Data_v10/listPCMEDC"
    # ln -sf "$gridpath/.GA_pp_AOD-1380/DPGEDC" "$gridpath/.GA_pp_AOD-1380/listPCMEDC"
    # ##################   MC   #############################
    # ### LHC16 MC
    # bash DownScript.sh 2700 2701 2702 -Name_LHC16MC_v10 GA_pp_MC_AOD  ?_GammaConvCalo_20 ?_GammaCaloMix -mergetrains  -RL_listPCMEDC $1
    # ### LHC17 MC
    # bash DownScript.sh 2703 2704 -Name_LHC17MC_v10 GA_pp_MC_AOD  ?_GammaConvCalo_20 ?_GammaCaloMix -mergetrains  -RL_listPCMEDC $1
    # ### LHC18 MC
    # bash DownScript.sh 2705 2707 -Name_LHC18MC_v10 GA_pp_MC_AOD  ?_GammaConvCalo_20 ?_GammaCaloMix -mergetrains  -uSR_DPGEDC $1
    # bash DownScript.sh 2705 2707 -Name_LHC18MC_v10 GA_pp_MC_AOD  ?_GammaConvCalo_20 ?_GammaCaloMix -mergetrains  -RL_listDPCIncAccTPC $1
    # ln -sf "$gridpath/LHC18MC_v10/listDPCIncAccTPC" "$gridpath/LHC18MC_v10/listPCMEDC"
    # ln -sf "$gridpath/.GA_pp_MC_AOD-2705/listDPCIncAccTPC" "$gridpath/.GA_pp_MC_AOD-2705/listPCMEDC"
    # ln -sf "$gridpath/.GA_pp_MC_AOD-2707/listDPCIncAccTPC" "$gridpath/.GA_pp_MC_AOD-2707/listPCMEDC"
    # ln -sf "$gridpath/LHC18MC_v10/DPGEDC" "$gridpath/LHC18MC_v10/listPCMEDC"
    # ln -sf "$gridpath/.GA_pp_MC_AOD-2705/DPGEDC" "$gridpath/.GA_pp_MC_AOD-2705/listPCMEDC"
    # ln -sf "$gridpath/.GA_pp_MC_AOD-2707/DPGEDC" "$gridpath/.GA_pp_MC_AOD-2707/listPCMEDC"
    # # #################   merging   #############################
    # # # ### Data 16 + 17 + 18
    # bash DownScript.sh 1378 1379 1380 -Name_Data_pp13TeV_v10 GA_pp_AOD ?_GammaConvCalo ?_GammaCaloMix -mergetrains -RL_listPCMEDC -noDown $1
    # # # ### MC 16 + 17 + 18
    # bash DownScript.sh  2700 2701 2702 2703 2704 2705 2707 -Name_MC_pp13TeV_v10 GA_pp_MC_AOD ?_GammaConvCalo ?_GammaCaloMix -mergetrains  -RL_listPCMEDC -noDown $1
    # #


    #################  Skin Data   #############################
    # ### LHC16 Data
    # bash DownScript.sh 1388 1391 -Name_SkimLHC16Data_v10 GA_pp_AOD   ?_GammaConvCalo_20 ?_GammaConvCalo_3 -mergetrains  -RL_listDPGEDCtrigger $1
    # ### LHC17 Data
    # bash DownScript.sh 1389 1392 -Name_SkimLHC17Data_v10 GA_pp_AOD   ?_GammaConvCalo_20 ?_GammaConvCalo_3 -mergetrains  -RL_listDPGEDCtrigger $1
    # ### LHC18 Data
    # bash DownScript.sh 1390 1393 -Name_SkimLHC18Data_v10 GA_pp_AOD   ?_GammaConvCalo_20 ?_GammaConvCalo_3 -mergetrains  -RL_listEDC_trigger $1
    # ln -sf "$gridpath/SkimLHC18Data_v10/listEDC_trigger" "$gridpath/SkimLHC18Data_v10/listDPGEDCtrigger"
    # ln -sf "$gridpath/.GA_pp_AOD-1390/listEDC_trigger" "$gridpath/.GA_pp_AOD-1390/listDPGEDCtrigger"
    # ln -sf "$gridpath/.GA_pp_AOD-1393/listEDC_trigger" "$gridpath/.GA_pp_AOD-1393/listDPGEDCtrigger"
    # ##############   JJ MC   #############################
    # ### LHC16 MC
    # bash DownScript.sh 2697 GA_pp_MC_AOD  ?_GammaConvCalo_20 ?_GammaConvCalo_3 -mergetrains -IsJJ $1
    # bash DownScript.sh 2710 GA_pp_MC_AOD  ?_GammaConvCalo_20 ?_GammaConvCalo_3 -mergetrains -IsJJ $1
    # bash DownScript.sh 2673 GA_pp_MC_AOD  ?_GammaConvCalo_20 ?_GammaConvCalo_3 -mergetrains -IsJJ $1
    # bash DownScript.sh 2697 2710 2673 -Name_JJLHC16MC_v10 GA_pp_MC_AOD  ?_GammaConvCalo_20 ?_GammaConvCalo_3 -mergetrains -RL_default -noDown $1
    # ### LHC17 MC
    # bash DownScript.sh 2698 GA_pp_MC_AOD   ?_GammaConvCalo_20 ?_GammaConvCalo_3 -mergetrains -IsJJ $1
    # bash DownScript.sh 2709 GA_pp_MC_AOD   ?_GammaConvCalo_20 ?_GammaConvCalo_3 -mergetrains -IsJJ $1
    # bash DownScript.sh 2674 GA_pp_MC_AOD   ?_GammaConvCalo_20 ?_GammaConvCalo_3 -mergetrains -IsJJ $1
    # bash DownScript.sh 2698 2709 2674 -Name_JJLHC17MC_v10 GA_pp_MC_AOD   ?_GammaConvCalo_20 ?_GammaConvCalo_3 -mergetrains -RL_default -noDown $1
    # ### LHC18 MC
    # bash DownScript.sh 2699 GA_pp_MC_AOD  ?_GammaConvCalo_20 ?_GammaConvCalo_3 -mergetrains -IsJJ $1
    # bash DownScript.sh 2708 GA_pp_MC_AOD  ?_GammaConvCalo_20 ?_GammaConvCalo_3 -mergetrains -IsJJ $1
    # bash DownScript.sh 2675 GA_pp_MC_AOD  ?_GammaConvCalo_20 ?_GammaConvCalo_3 -mergetrains -IsJJ $1
    # bash DownScript.sh 2699 2708 2675 -Name_JJLHC18MC_v10 GA_pp_MC_AOD  ?_GammaConvCalo_20 ?_GammaConvCalo_3 -mergetrains -RL_default -noDown $1
    # #################   merging   #############################
    # # ### Data 16 + 17 + 18s
    # bash DownScript.sh 1388 1391 1389 1392 1390 1393 -Name_SkimData_pp13TeV_v10 GA_pp_AOD ?_GammaConvCalo -mergetrains  -RL_listDPGEDCtrigger -noDown $1
    # ### MC 16 + 17 + 18
    # bash DownScript.sh 2697 2710 2673 2698 2709 2674 2699 2708 2675 -Name_JJMC_pp13TeV_v10 GA_pp_MC_AOD ?_GammaConvCalo -mergetrains  -RL_default -noDown $1



    # bash DownScript.sh 1260 1261 1348 -Name_Data_pp13TeV_v7 GA_pp_AOD ?_GammaConvCalo_2020 -mergetrains  -RL_listPCMEDC -noDown $1
    #
    # ##################   Data   #############################
    # ### LHC16 Data
    # bash DownScript.sh 1367 -Name_LHC16Data_v9 GA_pp_AOD ?_GammaConvCalo_20  -RL_listPCMEDC $1
    # ### LHC17 Data
    # bash DownScript.sh 1368 -Name_LHC17Data_v9 GA_pp_AOD ?_GammaConvCalo_20  -RL_listPCMEDC $1
    # ### LHC18 Data
    # bash DownScript.sh 1369 -Name_LHC18Data_v9 GA_pp_AOD ?_GammaConvCalo_20  -RL_listDPGIncTPC $1
    # ##################   MC   #############################
    # # ### LHC16 MC
    # bash DownScript.sh 2580 2581 2582 -Name_LHC16MC_v9 GA_pp_MC_AOD ?_GammaConvCalo_20 -mergetrains  -RL_listPCMEDC $1
    # # ### LHC17 MC
    # bash DownScript.sh 2583 2584 -Name_LHC17MC_v9 GA_pp_MC_AOD ?_GammaConvCalo_20 -mergetrains  -RL_listPCMEDC $1
    # # ### LHC18 MC
    # bash DownScript.sh 2585 2586 -Name_LHC18MC_v9 GA_pp_MC_AOD ?_GammaConvCalo_20 -mergetrains  -RL_listDPCIncAccTPC $1
    # # #################   merging   #############################
    # # ### Data 16 + 17 + 18
    # bash DownScript.sh 1367 1368 1369 -Name_Data_pp13TeV_v9 GA_pp_AOD ?_GammaConvCalo -mergetrains -RL_listPCMEDC -noDown $1
    # # ### MC 16 + 17 + 18
    # bash DownScript.sh  2580 2581 2582 2583 2584 2585 2586 -Name_MC_pp13TeV_v9 GA_pp_MC_AOD ?_GammaConvCalo -mergetrains  -RL_listPCMEDC -noDown $1
    # #
    #
    #
    # ##############   JJ MC   #############################
    # ## LHC16 MC
    # bash DownScript.sh 2558 -Name_JJLHC16MC_v9 GA_pp_MC_AOD ?_GammaConvCalo_20 -IsJJ $1
    # ## LHC17 MC
    # bash DownScript.sh 2559 -Name_JJLHC17MC_v9 GA_pp_MC_AOD  ?_GammaConvCalo_20 -IsJJ $1
    # ## LHC18 MC
    # bash DownScript.sh 2560 -Name_JJLHC18MC_v9 GA_pp_MC_AOD ?_GammaConvCalo_20 -IsJJ $1
    # #################  Skin Data   #############################
    # # ## LHC16 Data
    # # bash DownScript.sh 1263 -Name_SkimLHC16Data_v9 GA_pp_AOD  ?_GammaConvCalo_20  -RL_listDPGEDCtrigger $1
    # # ## LHC17 Data
    # # bash DownScript.sh 1264 -Name_SkimLHC17Data_v9 GA_pp_AOD  ?_GammaConvCalo_20  -RL_listDPGEDCtrigger $1
    # # ## LHC18 Data
    # # bash DownScript.sh 1265 -Name_SkimLHC18Data_v6 GA_pp_AOD  ?_GammaConvCalo_20  -RL_listEDC_trigger $1
    # #################   merging   #############################
    # # ### Data 16 + 17 + 18s
    # bash DownScript.sh 1263 1264 1265 -Name_SkimData_pp13TeV_v9 GA_pp_AOD ?_GammaConvCalo_20 -mergetrains  -RL_listDPGEDCtrigger -noDown $1
    # ### MC 16 + 17 + 18
    # bash DownScript.sh 2558 2559 2560 -Name_JJMC_pp13TeV_v9 GA_pp_MC_AOD ?_GammaConvCalo_20 -mergetrains  -RL_default -noDown $1
    #
    #
    #


    # 2545, 2543, 2535, 2530
    # 2729, 2728, 2727, 2726
# bash DownScript.sh 2484 -Name_DecayJJLHC16MC_JJhigh_DCal GA_pp_MC_AOD ?_Gamma -IsJJ_8 $1
#############################################   need Rerun    #########################################################
    # ##################   DecaA_pp_MC_AOD  ?_GammaConvCalo -IsJJ_8 $1
    # bash DownScript.sh 2726 2729 -Name_DecayJJhigh_13TeV_EMCal_v10_var GA_pp_MC_AOD ?_GammaConvCalo -mergetrains -RL_default -noDown $1
    # bash DownScript.sh 2529 -Name_DecayJJlow_13TeV_EMCal_v10_var GA_pp_MC_AOD  ?_GammaConvCalo -IsJJ_8 $1
    #
# y MCs   #############################
#     # # bash DownScript.sh 2539 -Name_DecayJJhigh_13TeV_DCAL_LHC17_v10 GA_pp_MC_AOD ?_GammaConvCalo_2003 -IsJJ_8 $1
#     # # bash DownScript.sh 2537 -Name_DecayJJlow_13TeV_DCal_LHC17_v10 GA_pp_MC_AOD ?_GammaConvCalo_2003 -IsJJ_8 $1
#     # # bash DownScript.sh 2727 GA_pp_MC_AOD ?_GammaConvCalo -IsJJ_8 $1
#     # # bash DownScript.sh 2728 GA_pp_MC_AOD ?_GammaConvCalo -IsJJ_8 $1
#     # bash DownScript.sh 2727 2728 -Name_DecayJJhigh_13TeV_DCAL_LHC18_v10 GA_pp_MC_AOD ?_GammaConvCalo -mergetrains -RL_default -noDown $1
#     # # bash DownScript.sh 2534 GA_pp_MC_AOD ?_GammaConvCalo_2003 -IsJJ_8 $1
#     # # bash DownScript.sh 2544 GA_pp_MC_AOD ?_GammaConvCalo_2003 -IsJJ_8 $1
#     # bash DownScript.sh 2534 2544 -Name_DecayJJlow_13TeV_DCAL_LHC18_v10 GA_pp_MC_AOD ?_GammaConvCalo -mergetrains -RL_default -noDown $1
#     # # bash DownScript.sh 2523 -Name_DecayJJhigh_13TeV_DCal_LHC16_v10 GA_pp_MC_AOD ?_GammaConvCalo_2003 -IsJJ_8 $1
#     # # bash DownScript.sh 2522 -Name_DecayJJlow_13TeV_DCal_LHC16_v10 GA_pp_MC_AOD ?_GammaConvCalo_2003 -IsJJ_8 $1
#     # # bash DownScript.sh 2726 GA_pp_MC_AOD ?_GammaConvCalo_2003 -IsJJ_8 $1
#     # # bash DownScript.sh 2729 GA_pp_MC_AOD ?_GammaConvCalo_2003 -IsJJ_8 $1
#     # bash DownScript.sh 2726 2729 -Name_DecayJJhigh_13TeV_EMCal_LHC18_v10 GA_pp_MC_AOD ?_GammaConvCalo -mergetrains -RL_default -noDown $1
#     # # bash DownScript.sh 2529 GA_pp_MC_AOD ?_GammaConvCalo_2003 -IsJJ_8 $1
#     # # bash DownScript.sh 2546 GA_pp_MC_AOD ?_GammaConvCalo_2003 -IsJJ_8 $1
#     # bash DownScript.sh 2529 2546 -Name_DecayJJlow_13TeV_EMCal_LHC18_v10 GA_pp_MC_AOD ?_GammaConvCalo -mergetrains -RL_default -noDown $1
#     # # bash DownScript.sh 2528 -Name_DecayJJhigh_13TeV_EMCal_LHC17_v10 GA_pp_MC_AOD ?_GammaConvCalo_2003 -IsJJ_8 $1
#     # # bash DownScript.sh 2527 -Name_DecayJJlow_13TeV_EMCal_LHC17_v10 GA_pp_MC_AOD ?_GammaConvCalo_2003 -IsJJ_8 $1
#     # # bash DownScript.sh 2525 -Name_DecayJJhigh_13TeV_EMCal_LHC16_v10 GA_pp_MC_AOD ?_GammaConvCalo_2003 -IsJJ_8 $1
#     # # bash DownScript.sh 2524 -Name_DecayJJlow_13TeV_EMCal_LHC16_v10 GA_pp_MC_AOD ?_GammaConvCalo_2003 -IsJJ_8 $1

#     # bash DownScript.sh 2523 2525 -Name_DecayJJhigh_13TeV_LHC16_v10 GA_pp_MC_AOD ?_GammaConvCalo -mergetrains -RL_default -noDown $1
#     # bash DownScript.sh 2522 2524 -Name_DecayJJlow_13TeV_LHC16_v10 GA_pp_MC_AOD ?_GammaConvCalo -mergetrains -RL_default -noDown $1

#     # # ##################   Decay MCs Sys Var  #############################
#     # # bash DownScript.sh 2727 GA_pp_MC_AOD  ?_GammaConvCalo -IsJJ_8 $1
#     # # bash DownScript.sh 2728 GA_pp_MC_AOD  ?_GammaConvCalo -IsJJ_8 $1
#     # bash DownScript.sh 2727 2728 -Name_DecayJJhigh_13TeV_DCAL_v10_var GA_pp_MC_AOD ?_GammaConvCalo -mergetrains -RL_default -noDown $1
#     # # bash DownScript.sh 2534 -Name_DecayJJlow_13TeV_DCAL_v10_var GA_pp_MC_AOD  ?_GammaConvCalo -IsJJ_8 $1
#     # # bash DownScript.sh 2726 GA_pp_MC_AOD  ?_GammaConvCalo -IsJJ_8 $1
#     # # bash DownScript.sh 2729 G
    # # ##################   MC Sys Var  #############################
    # # ##################   Data   #############################
    # ### LHC18 Data
    # bash DownScript.sh 1348 -Name_LHC18Data_var GA_pp_AOD  ?_GammaConvCalo_20  -RL_listDPGIncTPC $1
    # # ### LHC18 MC
    # # bash DownScript.sh 2512 -Name_LHC18MC_var GA_pp_MC_AOD  ?_GammaConvCalo_20 -mergetrains  -RL_listDPCIncAccTPC $1
    # bash DownScript.sh 2513 2512 -Name_LHC18MC_var GA_pp_MC_AOD  ?_GammaConvCalo_20 -mergetrains  -RL_listDPCIncAccTPC $1
    #
    # ##############   JJ MC Sys var  #############################
    # # ### LHC18 MC
    # # bash DownScript.sh 2547 2515 -Name_JJLHC18MC_var GA_pp_MC_AOD  ?_GammaConvCalo_3 -IsJJ $1
    # bash DownScript.sh 2547 2515 -Name_JJLHC18MC_var GA_pp_MC_AOD  ?_GammaConvCalo_3 -RL_default -mergetrains -noDown $1
    # # #################  Skin Data Sys Var   #############################
    # # ### LHC18 Data
    # # bash DownScript.sh 1350 1360 -Name_SkimLHC18Data_var GA_pp_AOD  ?_GammaConvCalo_3 -mergetrains  -RL_listEDC_trigger $1
    # #


#######################################################################################################
#######################################################################################################

    ##################   Sigma V2   #############################
    ### LHC18 Data
    # bash DownScript.sh 2617 -Name_LHC18Data_Sigma_V2 GA_pp ?_zip -RL_DPGTrackCalo -RL_listDPGCalo -RL_listDPGTrackAndCalo $1
    ### LHC18 MC
    # bash DownScript.sh 3832 -Name_LHC18MC_Sigma_V2 GA_pp_MC ?_Sigma -RL_listDPGAndCalo -RL_listDPGTrackAndCalo $1



    ##################   Sigma V1   #############################
    ### LHC18 Data
    # bash DownScript.sh 2616 -Name_LHC18Data_Sigma GA_pp ?_Sigma -RL_DPGTrackCalo -RL_listDPGCalo -RL_listDPGTrackAndCalo $1
    ### LHC18 MC
    # bash DownScript.sh 3829 -Name_LHC18MC_Sigma GA_pp_MC ?_Sigma -RL_listDPGAndCalo -RL_listDPGTrackAndCalo $1




    ####                 new Trains v8
    #
    ##### low B
    # ##################   MC   #############################
    # # ### LHC16 MC
    # bash DownScript.sh 2381 -Name_LHC16MC_lowB GA_pp_MC_AOD ?_GammaConvCalo -RL_lowB_DPGTrackIncAccTPCandEMC $1
    # ln -sf "$gridpath/LHC16MC_lowB/lowB_DPGTrackIncAccTPCandEMC" "$gridpath/LHC16MC_lowB/listDPGTrackIncAccTPCandEMC"
    # ln -sf "$gridpath/.GA_pp_MC_AOD-2381/lowB_DPGTrackIncAccTPCandEMC" "$gridpath/.GA_pp_MC_AOD-2381/listDPGTrackIncAccTPCandEMC"
    # # ### LHC17 MC
    # bash DownScript.sh 2341 -Name_LHC17MC_lowB GA_pp_MC_AOD ?_GammaConvCalo -RL_listDPGTrackIncAccTPCandEMC $1
    # # ### LHC18 MC
    # bash DownScript.sh 2342 -Name_LHC18MC_lowB GA_pp_MC_AOD ?_GammaConvCalo -mergetrains  -RL_listDPGTrackIncAccTPCandEMC $1
    # ##################   Data   #############################
    # ### LHC16 Data
    # bash DownScript.sh 1289 -Name_LHC16Data_lowB GA_pp_AOD ?_GammaConvCalo  -RL_lowB_DPGTrackIncAccTPCandEMC $1
    # ln -sf "$gridpath/LHC16Data_lowB/lowB_DPGTrackIncAccTPCandEMC" "$gridpath/LHC16Data_lowB/listDPGTrackIncAccTPCandEMC"
    # ln -sf "$gridpath/.GA_pp_AOD-1289/lowB_DPGTrackIncAccTPCandEMC" "$gridpath/.GA_pp_AOD-1289/listDPGTrackIncAccTPCandEMC"
    # ### LHC17 Data
    # bash DownScript.sh 1278 -Name_LHC17Data_lowB GA_pp_AOD ?_GammaConvCalo  -RL_listDPGTrackIncAccTPCandEMC $1
    # ### LHC18 Data
    # bash DownScript.sh 1279 -Name_LHC18Data_lowB GA_pp_AOD ?_GammaConvCalo  -RL_listDPGTrackIncAccTPCandEMC $1
    # ##################   merging   #############################
    # ### Data 16 + 17 + 18
    # bash DownScript.sh  1289 1278 1279 -Name_Data_pp13TeV_lowB_MBW GA_pp_AOD ?_GammaConvCalo -mergetrains  -RL_lowB_DPGTrackIncAccTPCandEMC -RL_listDPGTrackIncAccTPCandEMC -noDown $1
    # ### MC 16 + 17 + 18
    # bash DownScript.sh  2381 2341 2342 -Name_MC_pp13TeV_lowB GA_pp_MC_AOD ?_GammaConvCalo -mergetrains -RL_lowB_DPGTrackIncAccTPCandEMC -RL_listDPGTrackIncAccTPCandEMC -noDown $1

    # # Norm b
    # ##################   MC   #############################
    # # ### LHC16 MC
    # bash DownScript.sh 2378 2338 2377 -Name_LHC16MC_v8 GA_pp_MC_AOD ?_GammaConvCalo_20 -mergetrains  -RL_listPCMEDC $1
    # # ### LHC17 MC
    # bash DownScript.sh 2339 2379 -Name_LHC17MC_v8 GA_pp_MC_AOD ?_GammaConvCalo_20 -mergetrains  -RL_listPCMEDC $1
    # # ### LHC18 MC
    # bash DownScript.sh 2380 2340 -Name_LHC18MC_v8 GA_pp_MC_AOD ?_GammaConvCalo_20 -mergetrains  -RL_listDPCIncAccTPC $1
    # ##################   Data   #############################
    # ### LHC16 Data
    # bash DownScript.sh 1275 -Name_LHC16Data_v8 GA_pp_AOD ?_GammaConvCalo_20  -RL_listPCMEDC $1
    # ### LHC17 Data
    # bash DownScript.sh 1276 -Name_LHC17Data_v8 GA_pp_AOD ?_GammaConvCalo_20  -RL_listPCMEDC $1
    # ### LHC18 Data
    # bash DownScript.sh 1277 -Name_LHC18Data_v8 GA_pp_AOD ?_GammaConvCalo_20  -RL_listDPGIncTPC $1
    # ##################   merging   #############################
    # # ### Data 16 + 17 + 18
    # bash DownScript.sh 1275 1276 1277 -Name_Data_pp13TeV_v8 GA_pp_AOD ?_GammaConvCalo -mergetrains -RL_listPCMEDC -noDown $1
    # # ### MC 16 + 17 + 18
    # bash DownScript.sh  2378 2338 2377 2339 2379 2380 2340 -Name_MC_pp13TeV_v8 GA_pp_MC_AOD ?_GammaConvCalo -mergetrains  -RL_listPCMEDC -noDown $1


    # ##################   MC   #############################
    # ### LHC16 MC
    # bash DownScript.sh 2315 2316 2317 -Name_LHC16MC_v7 GA_pp_MC_AOD ?_GammaCaloMix ?_GammaConvCalo_20 -mergetrains  -RL_listPCMEDC $1
    # ### LHC17 MC
    # bash DownScript.sh 2318 2319 -Name_LHC17MC_v7 GA_pp_MC_AOD ?_GammaCaloMix ?_GammaConvCalo_20 -mergetrains  -RL_listPCMEDC $1
    # ### LHC18 MC
    # bash DownScript.sh 2320 2321 -Name_LHC18MC_v7 GA_pp_MC_AOD ?_GammaCaloMix ?_GammaConvCalo_20 -mergetrains  -RL_listDPCIncAccTPC $1
    # ##################   Data   #############################
    # ### LHC16 Data
    # bash DownScript.sh 1260 -Name_LHC16Data_v7 GA_pp_AOD ?_GammaCaloMix ?_GammaConvCalo_20  -RL_listPCMEDC $1
    # ### LHC17 Data
    # bash DownScript.sh 1261 -Name_LHC17Data_v7 GA_pp_AOD ?_GammaCaloMix ?_GammaConvCalo_20  -RL_listPCMEDC $1
    # ### LHC18 Data
    # # bash DownScript.sh 1262 -Name_LHC18Data_v7 GA_pp_AOD ?_GammaCaloMix ?_GammaConvCalo_20  -RL_listDPGIncTPC $1
    # # ##################   merging   #############################
    # # ### Data 16 + 17 + 18
    # bash DownScript.sh 1260 1261 1262 -Name_Data_pp13TeV_16_17_18_v7 GA_pp_AOD ?_Gamma -mergetrains  -RL_listPCMEDC -noDown $1
    # # ### MC 16 + 17 + 18
    # bash DownScript.sh 2315 2316 2317 2318 2319 2320 2321 -Name_MC_pp13TeV_16_17_18_v7 GA_pp_MC_AOD ?_GammaCaloMix ?_GammaConvCalo -mergetrains -RL_listPCMEDC -noDown $1

    ###############   JJ MC   #############################
    ### LHC16 MC
    # bash DownScript.sh 2322 -Name_JJLHC16MC_v7 GA_pp_MC_AOD ?_GammaCaloMix ?_GammaConvCalo_20 -IsJJ $1
    ### LHC17 MC
    # bash DownScript.sh 2323 -Name_JJLHC17MC_v7 GA_pp_MC_AOD ?_GammaCaloMix ?_GammaConvCalo_20 -IsJJ $1
    ### LHC18 MC
    # bash DownScript.sh 2324 -Name_JJLHC18MC_v7 GA_pp_MC_AOD ?_Gamma -IsJJ $1
    # #################  Skin Data   #############################
    ### LHC16 Data
    # bash DownScript.sh 1263 -Name_SkimLHC16Data_v7 GA_pp_AOD ?_GammaCaloMix ?_GammaConvCalo_20  -RL_listDPGEDCtrigger $1
    ### LHC17 Data
    # bash DownScript.sh 1264 -Name_SkimLHC17Data_v7 GA_pp_AOD ?_GammaCaloMix ?_GammaConvCalo_20  -RL_listDPGEDCtrigger $1
    ### LHC18 Data
    # bash DownScript.sh 1265 -Name_SkimLHC18Data_v6 GA_pp_AOD ?_GammaCaloMix ?_GammaConvCalo_20  -RL_listEDC_trigger $1
#     # #################   merging   #############################
#     # ### Data 16 + 17 + 18s
#     bash DownScript.sh 1263 1264 1265 -Name_SkimData_pp13TeV_v7 GA_pp_AOD ?_GammaCaloMix ?_GammaConvCalo -mergetrains  -RL_listDPGEDCtrigger -noDown $1
#     # ### MC 16 + 17 + 18
#     bash DownScript.sh 2322 2323 2324 -Name_JJMC_pp13TeV_v7 GA_pp_MC_AOD ?_GammaCaloMix ?_GammaConvCalo -mergetrains  -RL_default -noDown $1
#     #
#     #

# #####################################################################################################################################################################
# #####################################################################################################################################################################

#     # ### LHC18 Data
#     # bash DownScript.sh 1203 -Name_LHC18Data_CaloMix GA_pp_AOD ?_GammaCaloMix_1  -RL_listDPGCalo $1

# #####                 new Trains v6
# # #####                 with recalib and MBW
# # #####                 including QA
# # ##################   Data   #############################
# # ### LHC16 Data
# # bash DownScript.sh 1181 -Name_LHC16Data_v6 GA_pp_AOD ?_GammaConvCalo_20  -RL_listPCMEDC $1
# # ### LHC17 Data
# # bash DownScript.sh 1182 -Name_LHC17Data_v6 GA_pp_AOD ?_GammaConvCalo_20  -RL_listPCMEDC $1
# # ### LHC18 Data
# # bash DownScript.sh 1183 -Name_LHC18Data_v6 GA_pp_AOD ?_GammaConvCalo_20  -RL_listDPGIncTPC $1
# # ##################   MC   #############################
# # ### LHC16 MC
# # bash DownScript.sh 2139 2144 2145 -Name_LHC16MC_v6 GA_pp_MC_AOD ?_GammaConvCalo_20 -mergetrains  -RL_listPCMEDC $1
# # ### LHC17 MC
# # bash DownScript.sh 2146 2147 -Name_LHC17MC_v6 GA_pp_MC_AOD ?_GammaConvCalo_20 -mergetrains  -RL_listPCMEDC $1
# # ### LHC18 MC
# # bash DownScript.sh 2148 2149 -Name_LHC18MC_v6 GA_pp_MC_AOD ?_GammaConvCalo_20 -mergetrains  -RL_listDPCIncAccTPC $1
# # # ##################   merging   #############################
# # # ### Data 16 + 17 + 18
# bash DownScript.sh 1181 1182 1183 -Name_Data_pp13TeV_16_17_18_v6 GA_pp_AOD ?_GammaConvCalo -mergetrains  -RL_listPCMEDC -noDown $1
# # # ### MC 16 + 17 + 18
# bash DownScript.sh 2139 2144 2145 2146 2147 2148 2149 -Name_MC_pp13TeV_16_17_18_v6 GA_pp_MC_AOD ?_GammaConvCalo -mergetrains -RL_listPCMEDC -noDown $1
# # ################    QA   #############################
# # ### LHC16 Data
# # bash DownScript.sh 1181 -Name_LHC16Data_v6 GA_pp_AOD ?_GammaConvCalo_2002 -onlyrunwise  -RL_listPCMEDC $1
# # ### LHC17 Data
# # bash DownScript.sh 1182 -Name_LHC17Data_v6 GA_pp_AOD ?_GammaConvCalo_2002 -onlyrunwise  -RL_listPCMEDC $1
# # ### LHC18 Datas
# # bash DownScript.sh 1183 -Name_LHC18Data_v6 GA_pp_AOD ?_GammaConvCalo_2002 -onlyrunwise  -RL_listDPGIncTPC $1
# # # ### LHC16 MC
# # bash DownScript.sh 2139 2144 2145 -Name_LHC16MC_v6 GA_pp_MC_AOD ?_GammaConvCalo_2002 -onlyrunwise -mergeruns -mergetrains  -RL_listPCMEDC $1
# # ### LHC17 MC
# # bash DownScript.sh 2146 2147 -Name_LHC17MC_v6 GA_pp_MC_AOD ?_GammaConvCalo_2002 -onlyrunwise -mergeruns -mergetrains  -RL_listPCMEDC $1
# # ### LHC18 MC
# # bash DownScript.sh 2148 2149 -Name_LHC18MC_v6 GA_pp_MC_AOD ?_GammaConvCalo_2002 -onlyrunwise -mergeruns -mergetrains  -RL_listDPCIncAccTPC $1
# # ##################    Photon QA   #############################
# # ## LHC16 Data 938
# # bash DownScript.sh 1181 -Name_LHC16Data_PhotonQA3 GA_pp_AOD ?_AnalysisResults -onlyrunwise  -RL_listDPGIncAcc -runwise $1
# # ## LHC17 Data 939
# # bash DownScript.sh 1182 -Name_LHC17Data_PhotonQA3 GA_pp_AOD ?_AnalysisResults -onlyrunwise  -RL_listDPGIncTPC -runwise $1
# # ## LHC18 Data 940
# # bash DownScript.sh 1183 -Name_LHC18Data_PhotonQA3 GA_pp_AOD ?_AnalysisResults -onlyrunwise  -RL_listDPGIncTPC -runwise $1
# # ### LHC16 MC
# # bash DownScript.sh 2139 2144 2145 -Name_LHC16MC_PhotonQA3 GA_pp_MC_AOD ?_AnalysisResults -onlyrunwise -mergeruns  -RL_listDPGIncAccTPC -runwise $1
# # ### LHC17 MC
# # bash DownScript.sh 2146 2147 -Name_LHC17MC_PhotonQA3 GA_pp_MC_AOD ?_AnalysisResults -onlyrunwise -mergeruns  -RL_listDPGIncTPC -runwise $1
# # ### LHC18 MC
# # bash DownScript.sh 2148 2149 -Name_LHC18MC_PhotonQA3 GA_pp_MC_AOD ?_AnalysisResults -onlyrunwise -mergeruns  -RL_listDPCIncAccTPC -runwise $1

# ###############   MC   #############################
# # ### LHC16 MC
# # bash DownScript.sh 2187 -Name_JJLHC16MC_v6 GA_pp_MC_AOD ?_GammaConvCalo_2003 -IsJJ -runwise $1
# # bash DownScript.sh 2187 -Name_JJLHC16MC_v6 GA_pp_MC_AOD ?_GammaConvCalo_20 -IsJJ $1
# # ### LHC17 MC
# # bash DownScript.sh 2188 -Name_JJLHC17MC_v6 GA_pp_MC_AOD ?_GammaConvCalo_2003 -IsJJ -runwise $1
# # bash DownScript.sh 2188 -Name_JJLHC17MC_v6 GA_pp_MC_AOD ?_GammaConvCalo_20 -IsJJ $1
# # # LHC18 MC
# # bash DownScript.sh 2189 -Name_JJLHC18MC_v6 GA_pp_MC_AOD ?_GammaConvCalo_2003 -IsJJ -runwise $1
# # bash DownScript.sh 2189 -Name_JJLHC18MC_v6 GA_pp_MC_AOD ?_GammaConvCalo_20 -IsJJ $1
# # #################   Data   #############################
# # # ### LHC16 Data
# # # bash DownScript.sh 1193 -Name_SkimLHC16Data_v6 GA_pp_AOD ?_GammaConvCalo_2003  -RL_listDPGEDCtrigger $1
# # bash DownScript.sh 1193 -Name_SkimLHC16Data_v6 GA_pp_AOD ?_GammaConvCalo_20  -RL_listPCMEDC $1
# # # ### LHC17 Data
# # # bash DownScript.sh 1194 -Name_SkimLHC17Data_v6 GA_pp_AOD ?_GammaConvCalo_2003  -RL_listDPGEDCtrigger $1
# # bash DownScript.sh 1194 -Name_SkimLHC17Data_v6 GA_pp_AOD ?_GammaConvCalo_20  -RL_listPCMEDC $1
# # # ### LHC18 Data
# # # bash DownScript.sh 1195 -Name_SkimLHC18Data_v6 GA_pp_AOD ?_GammaConvCalo_2003  -RL_listEDC_trigger $1
# # bash DownScript.sh 1195 -Name_SkimLHC18Data_v6 GA_pp_AOD ?_GammaConvCalo_20  -RL_listDPGIncTPC $1
# ##################   merging   #############################
# ### Data 16 + 17 + 18s
# bash DownScript.sh 1193 1194 1195 -Name_SkimData_pp13TeV_16_17_18_v6 GA_pp_AOD ?_GammaConvCalo -mergetrains  -RL_listDPGEDCtrigger -noDown $1
# ### MC 16 + 17 + 18
# bash DownScript.sh 2187 2188 2189 -Name_JJMC_pp13TeV_16_17_18_v6 GA_pp_MC_AOD ?_GammaConvCalo -mergetrains  -RL_default -noDown $1


#     #######################################################
#     ######################   Daten   ######################
#     #######################################################

#     # ### LHC16Data="679"; #pass 1 SECOND TRAIN RUN
#     # bash DownScript.sh 679 -Name_LHC16Data GA_pp_AOD ?_GammaConvCalo -mergetrains  -RL_listDPGEDC

#     # ### LHC17Data="635"; #pass 1 SECOND TRAIN RUN
#     # bash DownScript.sh 635 -Name_LHC17Data GA_pp_AOD ?_GammaConvCalo -mergetrains  -RL_listDPGEDC

#     # ### LHC18Data="705"; #pass 1 SECOND TRAIN RUN
#     # bash DownScript.sh 705 -Name_LHC18Data_old GA_pp_AOD ?_GammaConvCalo -mergetrains  -RL_listDPGEDC

#     # ### LHC18Data="753"; #pass 1 SECOND TRAIN RUN
#     # bash DownScript.sh 753 -Name_LHC18Data GA_pp_AOD ?_GammaConvCalo -mergetrains  -RL_listDPGEDC

#     # ### Daten LHC16Data="773"
#     # bash DownScript.sh 773 -Name_Data_pp13TeV_16 GA_pp_AOD ?_GammaConvCalo -mergetrains  -RL_listDPGEDC -RL_listDPGCalo -RL_listDPGalo -RL_listDPCIncAccTPC -RL_listDPGIncAccTPC -RL_listDPGIncTPC -RL_listDPGEMC

#     # ### Daten LHC17Data="774"
#     # bash DownScript.sh 774 -Name_Data_pp13TeV_17 GA_pp_AOD ?_GammaConvCalo -mergetrains  -RL_listDPGEDC -RL_listDPGCalo -RL_listDPGalo -RL_listDPCIncAccTPC -RL_listDPGIncAccTPC -RL_listDPGIncTPC -RL_listDPGEMC

#     # ### Daten LHC18Data="753"
#     # bash DownScript.sh 753 -Name_Data_pp13TeV_18 GA_pp_AOD ?_GammaConvCalo -mergetrains  -RL_listDPGCalo -RL_listDPGalo -RL_listDPCIncAccTPC -RL_listDPGIncAccTPC -RL_listDPGIncTPC -RL_listDPGEMC -RL_listDPGEDC -runwise

#     ### merging 16 + 17
#     bash DownScript.sh 773 774 -Name_Data_pp13TeV_16_17_v0 GA_pp_AOD ?_GammaConvCalo -mergetrains  -RL_listDPGEDC -RL_listDPGCalo -RL_listDPGalo -RL_listDPCIncAccTPC -RL_listDPGIncAccTPC -RL_listDPGIncTPC -RL_listDPGEMC -noDown

#     # ### merging 16 + 17 + 18
#     bash DownScript.sh 773 774 753 -Name_Data_pp13TeV_16_17_18 GA_pp_AOD ?_GammaConvCalo_  -RL_listDPGCalo -RL_listDPGalo -RL_listDPCIncAccTPC -RL_listDPGIncAccTPC -RL_listDPGIncTPC -RL_listDPGEMC -RL_listDPGEDC -noDown

#     # ### Daten LHC16Data_triggered="775"
#     # bash DownScript.sh 775 -Name_Skim_Data_pp13TeV_16 GA_pp_AOD ?_GammaConvCalo -mergetrains  -RL_listDPGEDCtrigger -RL_listDPGCalo -RL_listDPGalo -RL_listDPCIncAccTPC -RL_listDPGIncAccTPC -RL_listDPGIncTPC -RL_listDPGEMC

#     # ### Daten LHC17Data_triggered="776"
#     # bash DownScript.sh 776 -Name_Skim_Data_pp13TeV_17 GA_pp_AOD ?_GammaConvCalo -mergetrains  -RL_listDPGEDCtrigger -RL_listDPGCalo -RL_listDPGalo -RL_listDPCIncAccTPC -RL_listDPGIncAccTPC -RL_listDPGIncTPC -RL_listDPGEMC

#     # ### merging 16 + 17 triggered
#     bash DownScript.sh 775 776 -Name_Skim_Data_pp13TeV_16_17 GA_pp_AOD ?_GammaConvCalo -mergetrains  -RL_listDPGEDCtrigger -RL_listDPGCalo -RL_listDPGalo -RL_listDPCIncAccTPC -RL_listDPGIncAccTPC -RL_listDPGIncTPC -RL_listDPGEMC -noDown

#     ### LHC16 Data 829
#     # bash DownScript.sh 829 -Name_LHC16Data_v1 GA_pp_AOD ?_GammaConvCalo  -uSR_PCMEDC -runwise $1

#     ### LHC17 Data 830
#     # bash DownScript.sh 830 -Name_LHC17Data_v1 GA_pp_AOD ?_GammaConvCalo  -uSR_PCMEDC -runwise $1

#     ### LHC18 Data 734
#     # bash DownScript.sh 834 -Name_LHC18Data_v1 GA_pp_AOD ?_GammaConvCalo -mergetrains  -uSR_PCMEDC -runwise $1
#     # bash DownScript.sh 834 -Name_LHC18Data_v1 GA_pp_AOD ?_GammaConvCalo -mergetrains  -RL_listDPGIncAccTPC -RL_listDPGIncTPC -runwise $1

#     ### merging 16 + 17
#     bash DownScript.sh 829 830 -Name_Data_pp13TeV_16_17_v1 GA_pp_AOD ?_GammaConvCalo -mergetrains  -uSR_PCMEDC -noDown $1
#     ### merging 16 + 17 + 18
#     bash DownScript.sh 829 830 834 -Name_Data_pp13TeV_16_17_18_v1 GA_pp_AOD ?_GammaConvCalo -mergetrains  -uSR_PCMEDC -noDown -runwise $1

#     # ### LHC16 Data 900
#     # bash DownScript.sh 900 -Name_LHC16Data_v2 GA_pp_AOD ?_GammaConvCalo_20  -RL_listPCMEDC $1
#     #
#     # ### LHC17 Data 901
#     # bash DownScript.sh 901 -Name_LHC17Data_v2 GA_pp_AOD ?_GammaConvCalo_20  -RL_listPCMEDC $1
#     #
#     # ### merging 16 + 17
#     bash DownScript.sh 900 901 -Name_Data_pp13TeV_16_17_v2 GA_pp_AOD ?_GammaConvCalo -mergetrains  -RL_listPCMEDC -noDown $1
#     #
#     # ##### Photon QA
#     # bash DownScript.sh 885 -Name_LHC18Data_PhotonQA2 GA_pp_AOD ?_AnalysisResults -mergeruns -RL_listDPCIncAccTPC -RL_listDPGIncAccTPC -RL_listDPGIncTPC $1 -onlyrunwise
#     # bash DownScript.sh 903 -Name_LHC18Data_PhotonQA GA_pp_AOD ?_AnalysisResults -RL_listDPCIncAccTPC -RL_listDPGIncAccTPC -RL_listDPGIncTPC $1 -onlyrunwise

#     # ### LHC16 Data 938
#     # bash DownScript.sh 938 -Name_LHC16Data_v3 GA_pp_AOD ?_GammaConvCalo_20  -RL_listPCMEDC $1
#     #
#     # ### LHC17 Data 939
#     # bash DownScript.sh 939 -Name_LHC17Data_v3 GA_pp_AOD ?_GammaConvCalo_20  -RL_listPCMEDC $1
#     #
#     # ### LHC18 Data 940
#     # bash DownScript.sh 940 -Name_LHC18Data_v3 GA_pp_AOD ?_GammaConvCalo_20  -RL_listPCMEDC -RL_listDPCIncAccTPC -RL_listDPGIncAccTPC -RL_listDPGIncTPC $1
#     # # bash DownScript.sh 940 -Name_LHC18Data_v3 GA_pp_AOD ?_GammaConvCalo_20  -RL_listDPG $1

#     # ### merging 16 + 17
#     bash DownScript.sh 938 939 -Name_Data_pp13TeV_16_17_v3 GA_pp_AOD ?_GammaConvCalo -mergetrains  -RL_listPCMEDC -noDown $1
#     #
#     # ### merging 16 + 17 + 18
#     bash DownScript.sh 938 939 940 -Name_Data_pp13TeV_16_17_18_v3 GA_pp_AOD ?_GammaConvCalo -mergetrains  -RL_listPCMEDC -noDown $1
#     bash DownScript.sh 938 939 940 -Name_Data_pp13TeV_16_17_18_v3 GA_pp_AOD ?_GammaConvCalo -mergetrains -noDown $1

#     # ### LHC16 Data 938
#     # bash DownScript.sh 1001 -Name_LHC16Data_v4 GA_pp_AOD ?_GammaConvCalo_20  -RL_listPCMEDC $1
#     #
#     # ### LHC17 Data 939
#     # bash DownScript.sh 1010 -Name_LHC17Data_v4 GA_pp_AOD ?_GammaConvCalo_20  -RL_listPCMEDC $1
#     #
#     # ### LHC18 Data 940
#     # bash DownScript.sh 1011 -Name_LHC18Data_v4 GA_pp_AOD ?_GammaConvCalo_20  -RL_listPCMEDC -RL_listDPCIncAccTPC -RL_listDPGIncAccTPC -RL_listDPGIncTPC $1
#     #
#     # ### merging 16 + 17
#     bash DownScript.sh 1001 1010 -Name_Data_pp13TeV_16_17_v4 GA_pp_AOD ?_GammaConvCalo -mergetrains  -RL_listPCMEDC -noDown $1
#     #
#     # ### merging 16 + 17 + 18
#     bash DownScript.sh 1001 1010 1011 -Name_Data_pp13TeV_16_17_18_v4 GA_pp_AOD ?_GammaConvCalo -mergetrains  -RL_listPCMEDC -noDown $1
#     #
#     # ### LHC16 Data 938
#     # bash DownScript.sh 1083 -Name_LHC16Data_v5 GA_pp_AOD ?_GammaConvCalo_20  -RL_listPCMEDC $1
#     #
#     # ### LHC17 Data 939
#     # bash DownScript.sh 1084 -Name_LHC17Data_v5 GA_pp_AOD ?_GammaConvCalo_20  -RL_listPCMEDC $1
#     #
#     # ### LHC18 Data 940
#     # bash DownScript.sh 1085 -Name_LHC18Data_v5 GA_pp_AOD ?_GammaConvCalo_20  -RL_listPCMEDC -RL_listDPCIncAccTPC -RL_listDPGIncAccTPC -RL_listDPGIncTPC $1
#     #
#     # ### merging 16 + 17
#     bash DownScript.sh 1083 1084 -Name_Data_pp13TeV_16_17_v5 GA_pp_AOD ?_GammaConvCalo -mergetrains  -RL_listPCMEDC -noDown $1
#     #
#     # ### merging 16 + 17 + 18
#     bash DownScript.sh 1083 1084 1085 -Name_Data_pp13TeV_16_17_18_v5 GA_pp_AOD ?_GammaConvCalo -mergetrains  -RL_listPCMEDC -noDown $1

#     #######################################################
#     ######################   MC   ######################
#     #######################################################


#     # ### PYT8_13TeV_anchLHC16_AOD209   LHC16xMCPHY="1276"; #pass 1
#     # bash DownScript.sh 1276 -Name_LHC16xMCPHY GA_pp_MC_AOD ?_GammaConvCalo -mergetrains  -RL_listDPGEDC

#     # ### PYT8_13TeV_anchLHC16_AOD209_extra   LHC16xMCPHY_extra="1273"; #pass 1
#     # bash DownScript.sh 1273 -Name_LHC16xMCPHY_extra GA_pp_MC_AOD ?_GammaConvCalo -mergetrains  -RL_listDPGEDC

#     # ### PYT8_13TeV_anchLHC17_AOD209_extra   LHC17xMCPHY_extra="1408";
#     # bash DownScript.sh 1408 -Name_LHC17xMCPHY_extra GA_pp_MC_AOD ?_GammaConvCalo -mergetrains  -RL_listDPGEDC

#     # ### PYT8_13TeV_anchLHC17_AOD209  LHC17xJJMC="1393";
#     # bash DownScript.sh 1393 -Name_LHC17xJJMC GA_pp_MC_AOD ?_GammaConvCalo -mergetrains  -RL_listDPGEDC

#     # ### PYT8_13TeV_anchLHC17_AOD209  LHC18xMCPHY="1325"; #pass 1 FIRST TRAIN RUN
#     # bash DownScript.sh 1325 -Name_LHC18xMCPHY GA_pp_MC_AOD ?_GammaConvCalo -mergetrains  -RL_listDPGEDC

#     # ### PYT8_13TeV_anchLHC17_AOD209  LHC18xMCPHY="1415"; #pass 1 FIRST TRAIN RUN
#     # bash DownScript.sh 1415 -Name_LHC18xMCPHY GA_pp_MC_AOD ?_GammaConvCalo -mergetrains  -RL_listDPGEDC

#     # ### PYT8_13TeV_anchLHC16_AOD209
#     # bash DownScript.sh 1470 -Name_MC_pp13TeV_16_17 GA_pp_MC_AOD ?_GammaConvCalo -mergetrains  -RL_listDPGEDC -RL_listDPGCalo -RL_listDPGalo -RL_listDPCIncAccTPC -RL_listDPGIncAccTPC -RL_listDPGIncTPC -RL_listDPGEMC

#     # ### PYT8_13TeV_anchLHC16_AOD209_extra
#     # bash DownScript.sh 1471 -Name_MC_pp13TeV_16_17 GA_pp_MC_AOD ?_GammaConvCalo -mergetrains  -RL_listDPGEDC -RL_listDPGCalo -RL_listDPGalo -RL_listDPCIncAccTPC -RL_listDPGIncAccTPC -RL_listDPGIncTPC -RL_listDPGEMC

#     # ### PYT8_13TeV_anchLHC16_AOD209_extra2
#     # bash DownScript.sh 1472 -Name_MC_pp13TeV_16_17 GA_pp_MC_AOD ?_GammaConvCalo -mergetrains  -RL_listDPGEDC -RL_listDPGCalo -RL_listDPGalo -RL_listDPCIncAccTPC -RL_listDPGIncAccTPC -RL_listDPGIncTPC -RL_listDPGEMC

#     # ### PYT8_13TeV_anchLHC17_AOD209_extra
#     # bash DownScript.sh 1473 -Name_MC_pp13TeV_16_17 GA_pp_MC_AOD ?_GammaConvCalo -mergetrains  -RL_listDPGEDC -RL_listDPGCalo -RL_listDPGalo -RL_listDPCIncAccTPC -RL_listDPGIncAccTPC -RL_listDPGIncTPC -RL_listDPGEMC

#     # ### PYT8_13TeV_anchLHC17_AOD209
#     # bash DownScript.sh 1474 -Name_MC_pp13TeV_16_17 GA_pp_MC_AOD ?_GammaConvCalo -mergetrains  -RL_listDPGEDC -RL_listDPGCalo -RL_listDPGalo -RL_listDPCIncAccTPC -RL_listDPGIncAccTPC -RL_listDPGIncTPC -RL_listDPGEMC

#     # ### merging 16 MCs
#     # bash DownScript.sh 1470 1471 1472 -Name_MC_pp13TeV_16_v0 GA_pp_MC_AOD ?_GammaConvCalo -mergetrains  -RL_listDPGEDC -RL_listDPGCalo -RL_listDPGalo -RL_listDPCIncAccTPC -RL_listDPGIncAccTPC -RL_listDPGIncTPC -RL_listDPGEMC
#     #
#     # ### merging 17 MC
#     # bash DownScript.sh 1473 1474 -Name_MC_pp13TeV_17_v0 GA_pp_MC_AOD ?_GammaConvCalo -mergetrains  -RL_listDPGEDC -RL_listDPGCalo -RL_listDPGalo -RL_listDPCIncAccTPC -RL_listDPGIncAccTPC -RL_listDPGIncTPC -RL_listDPGEMC
#     #
#     # ### merging 16 + 17 MCs
#     # bash DownScript.sh 1470 1471 1472 1473 1474 -Name_MC_pp13TeV_16_17_v0 GA_pp_MC_AOD ?_GammaConvCalo -mergetrains  -RL_listDPGEDC -RL_listDPGCalo -RL_listDPGalo -RL_listDPCIncAccTPC -RL_listDPGIncAccTPC -RL_listDPGIncTPC -RL_listDPGEMC

#     ### LHC16 MC 1546 + 1547 + 1548
#     # bash DownScript.sh 1546 1547 1548 -Name_LHC16MC_v1 GA_pp_MC_AOD ?_GammaConvCalo -mergeall  -uSR_PCMEDC -runwise $1

#     ### LHC17 MC 1549 + 1550
#     # bash DownScript.sh 1549 1550 -Name_LHC17MC_v1 GA_pp_MC_AOD ?_GammaConvCalo -mergeall  -uSR_PCMEDC -runwise $1

#     ### LHC18 MC 1556 1557
#     # bash DownScript.sh 1556 1557 -Name_LHC18MC_v1 GA_pp_MC_AOD ?_GammaConvCalo -mergeall -uSR_PCMEDC -runwise $1
#     # bash DownScript.sh 1556 1557 -Name_LHC18MC_v1 GA_pp_MC_AOD ?_GammaConvCalo -mergeall -RL_listDPGIncAccTPC -RL_listDPGIncTPC  -runwise $1

#     ### merging 16 + 17
#     bash DownScript.sh 1546 1547 1548 1549 1550 -Name_MC_pp13TeV_16_17_v1 GA_pp_MC_AOD ?_GammaConvCalo -mergeall -uSR_PCMEDC -noDown $1 -runwise
#     ### merging 16 + 17 + 18
#     bash DownScript.sh 1546 1547 1548 1549 1550 1556 1557 -Name_MC_pp13TeV_16_17_18_v1 GA_pp_MC_AOD ?_GammaConvCalo -mergeall  -uSR_PCMEDC -noDown $1 -runwise

#     # ### LHC16 MC 1651 + 1705 + 1706
#     # # bash DownScript.sh 1651 -Name_LHC16MC_v2 GA_pp_MC_AOD ?_GammaConvCalo_20 -mergetrains  -RL_listPCMEDC $1
#     # bash DownScript.sh 1651 1705 1706 -Name_LHC16MC_v2 GA_pp_MC_AOD ?_GammaConvCalo_20 -mergetrains  -RL_listPCMEDC $1
#     #
#     # # ### LHC17 MC 1707 + 1708
#     # bash DownScript.sh 1707 1708 -Name_LHC17MC_v2 GA_pp_MC_AOD ?_GammaConvCalo_20 -mergetrains  -RL_listPCMEDC $1
#     #
#     # ### LHC18 MC 1709 1710
#     # bash DownScript.sh 1709 1710 -Name_LHC18MC_v2 GA_pp_MC_AOD ?_GammaConvCalo_20 -mergetrains  -RL_listDPCIncAccTPC -RL_listDPGIncAccTPC -RL_listDPGIncTPC -RL_listPCMEDC $1
#     #
#     # ### merging 16 + 17
#     bash DownScript.sh 1651 1705 1706 1707 1708 -Name_MC_pp13TeV_16_17_v2 GA_pp_MC_AOD ?_GammaConvCalo -mergetrains -RL_listPCMEDC -noDown $1

#     # ### merging 16 + 17 + 18
#     bash DownScript.sh 1651 1705 1706 1707 1708 1709 1710 -Name_MC_pp13TeV_16_17_18_v2 GA_pp_MC_AOD ?_GammaConvCalo -mergetrains -RL_listPCMEDC -noDown $1

#     ##### Photon QA
#     # bash DownScript.sh 1624 1625 -Name_LHC18MC_PhotonQA GA_pp_MC_AOD ?_AnalysisResults -mergeruns -RL_listDPCIncAccTPC -RL_listDPGIncAccTPC -RL_listDPGIncTPC $1 -onlyrunwise

#     # ### LHC16 MC 1756 + 1757 + 1758
#     # bash DownScript.sh 1756 1757 1758 -Name_LHC16MC_v3 GA_pp_MC_AOD ?_GammaConvCalo_20 -mergetrains  -RL_listPCMEDC $1
#     #
#     # ### LHC17 MC 1759 + 1760
#     # bash DownScript.sh 1759 1760 -Name_LHC17MC_v3 GA_pp_MC_AOD ?_GammaConvCalo_20 -mergetrains  -RL_listPCMEDC $1
#     #
#     # ### LHC18 MC 1761 1762
#     # bash DownScript.sh 1761 1762 -Name_LHC18MC_v3 GA_pp_MC_AOD ?_GammaConvCalo_20 -mergetrains  -RL_listPCMEDC -RL_listDPCIncAccTPC -RL_listDPGIncAccTPC -RL_listDPGIncTPC $1
#     # bash DownScript.sh 1761 1762 -Name_LHC18MC_v3 GA_pp_MC_AOD ?_GammaConvCalo_20 -mergetrains  -RL_listDPG $1
#     #
#     # ### merging 16 + 17
#     bash DownScript.sh 1756 1757 1758 1759 1760 -Name_MC_pp13TeV_16_17_v3 GA_pp_MC_AOD ?_GammaConvCalo -mergetrains -RL_listPCMEDC -noDown $1
#     #
#     # ### merging 16 + 17 + 18
#     bash DownScript.sh 1756 1757 1758 1759 1760 1761 1762 -Name_MC_pp13TeV_16_17_18_v3 GA_pp_MC_AOD ?_GammaConvCalo -mergetrains -RL_listPCMEDC -noDown $1

#     # ### With weights
#     # ### LHC16 MC 1789 + 1790 + 1791
#     # bash DownScript.sh 1789 1790 1791 -Name_LHC16MC_v3_ww GA_pp_MC_AOD ?_GammaConvCalo_20 -mergetrains  -RL_listPCMEDC $1
#     #
#     # ### LHC17 MC 1792 + 1793
#     # bash DownScript.sh 1792 1793 -Name_LHC17MC_v3_ww GA_pp_MC_AOD ?_GammaConvCalo_20 -mergetrains  -RL_listPCMEDC $1
#     #
#     # ### LHC18 MC 1794 1795
#     # bash DownScript.sh 1794 1795 -Name_LHC18MC_v3_ww GA_pp_MC_AOD ?_GammaConvCalo_20 -mergetrains  -RL_listDPG $1
#     #
#     # ### merging 16 + 17
#     bash DownScript.sh 1789 1790 1791 1792 1793 -Name_MC_pp13TeV_16_17_v3_ww GA_pp_MC_AOD ?_GammaConvCalo -mergetrains -RL_listPCMEDC -noDown $1
#     #
#     # ### merging 16 + 17 + 18
#     bash DownScript.sh 1789 1790 1791 1792 1793 1794 1795 -Name_MC_pp13TeV_16_17_18_v3_ww GA_pp_MC_AOD ?_GammaConvCalo -mergetrains -RL_listPCMEDC -noDown $1
#     bash DownScript.sh 1789 1790 1791 1792 1793 1794 1795 -Name_MC_pp13TeV_16_17_18_v3_ww GA_pp_MC_AOD ?_GammaConvCalo -mergetrains -noDown $1

#     # ### LHC16 MC
#     # bash DownScript.sh 1860 1861 -Name_LHC16MC_v4 GA_pp_MC_AOD ?_GammaConvCalo_20 -mergetrains  -RL_listPCMEDC $1
#     #
#     # ### LHC17 MC
#     # bash DownScript.sh 1862 1863 -Name_LHC17MC_v4 GA_pp_MC_AOD ?_GammaConvCalo_20 -mergetrains  -RL_listPCMEDC $1
#     #
#     # ### LHC18 MC
#     # bash DownScript.sh 1864 1865 -Name_LHC18MC_v4 GA_pp_MC_AOD ?_GammaConvCalo_20 -mergetrains  -RL_listPCMEDC -RL_listDPCIncAccTPC -RL_listDPGIncAccTPC -RL_listDPGIncTPC $1
#     #
#     # ### merging 16 + 17
#     bash DownScript.sh 1860 1861 1862 1863 -Name_MC_pp13TeV_16_17_v4 GA_pp_MC_AOD ?_GammaConvCalo -mergetrains -RL_listPCMEDC -noDown $1
#     #
#     # ### merging 16 + 17 + 18
#     bash DownScript.sh 1860 1861 1862 1863 1864 1865 -Name_MC_pp13TeV_16_17_18_v4 GA_pp_MC_AOD ?_GammaConvCalo -mergetrains -RL_listPCMEDC -noDown $1
#     bash DownScript.sh 1860 1861 1862 1863 1864 1865 -Name_MC_pp13TeV_16_17_18_v4 GA_pp_MC_AOD ?_GammaConvCalo -mergetrains -noDown $1

#     # ### LHC16 MC
#     # bash DownScript.sh 1971 1982 1983 -Name_LHC16MC_v5 GA_pp_MC_AOD ?_GammaConvCalo_204 -mergetrains  -RL_listPCMEDC $1
#     #
#     # ### LHC17 MC
#     # bash DownScript.sh 1984 1985 -Name_LHC17MC_v5 GA_pp_MC_AOD ?_GammaConvCalo_20 -mergetrains  -RL_listPCMEDC $1
#     #
#     # ### LHC18 MC
#     # bash DownScript.sh 1986 1987 -Name_LHC18MC_v5 GA_pp_MC_AOD ?_GammaConvCalo_20 -mergetrains  -RL_listPCMEDC -RL_listDPCIncAccTPC $1
#     #
#     # ### merging 16 + 17
#     bash DownScript.sh 1971 1982 1983 1984 1985 -Name_MC_pp13TeV_16_17_v5 GA_pp_MC_AOD ?_GammaConvCalo -mergetrains -RL_listPCMEDC -noDown $1
#     #
#     # ### merging 16 + 17 + 18
#     bash DownScript.sh 1971 1982 1983 1984 1985 1986 1987 -Name_MC_pp13TeV_16_17_18_v5 GA_pp_MC_AOD ?_GammaConvCalo -mergetrains -RL_listPCMEDC -noDown $1

    cat Error.log
    exit
fi
