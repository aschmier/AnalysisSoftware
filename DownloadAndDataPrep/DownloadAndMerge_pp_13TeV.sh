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
    gridpath="/media/adrian/Adrian/grid_data"


    ################   Data   #############################
    ### LHC16 Data
    bash DownScript.sh 1495 -Name_LHC16Data_SigmaPlus GA_pp_AOD ?_SigmaPlus -mergechilds -childsareperiods  -RL_listPHOS -totalLog $1
    ### LHC17 Data
    bash DownScript.sh 1496 -Name_LHC17Data_SigmaPlus GA_pp_AOD ?_SigmaPlus -mergechilds -childsareperiods  -RL_listPHOS -totalLog $1
    ### LHC18 Data
    bash DownScript.sh 1497 -Name_LHC18Data_SigmaPlus GA_pp_AOD ?_SigmaPlus -mergechilds -childsareperiods  -RL_listPHOS -totalLog $1
    # ##################   MC   #############################
    ### LHC16 MC
    bash DownScript.sh 2899 2898 2897 -Name_LHC16MC_SigmaPlus GA_pp_MC_AOD ?_SigmaPlus -mergechilds -mergetrains -childsareperiods  -RL_listPHOS -totalLog $1
    ### LHC17 MC
    bash DownScript.sh 2900 2901 -Name_LHC17MC_SigmaPlus GA_pp_MC_AOD ?_SigmaPlus -mergechilds -mergetrains -childsareperiods  -RL_listPHOS -totalLog $1
    ### LHC18 MC
    bash DownScript.sh 2902 2903 -Name_LHC18MC_SigmaPlus GA_pp_MC_AOD ?_SigmaPlus -mergechilds -mergetrains -childsareperiods  -RL_listPHOS -totalLog $1
    #################   merging   #############################
    ### Data 16 + 17 + 18
    bash DownScript.sh 1495 1496 1497 -Name_Data_pp13TeV_SigmaPlus GA_pp_AOD ?_SigmaPlus -mergechilds -mergetrains -childsareperiods -RL_listPHOS -noDown -totalLog $1
    ### MC 16 + 17 + 18
    bash DownScript.sh  2903 2902 2901 2900 2899 2898 2897 -Name_MC_pp13TeV_SigmaPlus GA_pp_MC_AOD ?_SigmaPlus -mergechilds -mergetrains -childsareperiods  -RL_listPHOS -noDown -totalLog $1
    #








# ################   Data   #############################
# # 1497 	vAN-20200811_ROOT6-1 	LHC18_13TeV_pass1_AOD208 	2020 Aug 13, Train run: All jobs submitted 	request Florian, Adrian, ...
# # 1496 	vAN-20200811_ROOT6-1 	LHC17_13TeV_pass1_AOD208 	2020 Aug 13, Train run: All jobs submitted 	request Florian, Adrian, ...
# # 1495 	vAN-20200811_ROOT6-1 	LHC16_13TeV_pass1_2_AOD208 	2020 Aug 13, Train run: All jobs submitted 	request Florian, Adrian &...
# ### LHC16 Data
# bash DownScript.sh 1495 -Name_LHC16Data_v12 GA_pp_AOD ?_GammaConvCalo_20 -mergechilds -childsareperiods  -RL_listPCMEDC -totalLog $1
# ### LHC17 Data
# bash DownScript.sh 1496 -Name_LHC17Data_v12 GA_pp_AOD ?_GammaConvCalo_20 -mergechilds -childsareperiods  -RL_listPCMEDC -totalLog $1
# ### LHC18 Data
# bash DownScript.sh 1497 -Name_LHC18Data_v12 GA_pp_AOD ?_GammaConvCalo_20 -mergechilds -childsareperiods  -uSR_DPGEDC -totalLog $1
# ln -sf "$gridpath/LHC18Data_v12/DPGEDC" "$gridpath/LHC18Data_v12/listPCMEDC"
# ln -sf "$gridpath/.GA_pp_AOD-1497/DPGEDC" "$gridpath/.GA_pp_AOD-1497/listPCMEDC"# ln -sf "$gridpath/LHC18Data_v10/listDPGIncTPC" "$gridpath/LHC18Data_v10/listPCMEDC"
# ln -sf "$gridpath/.GA_pp_AOD-1497/DPGEDC" "$gridpath/.GA_pp_AOD-1497/listPCMEDC"
# # ##################   MC   #############################
# # 2903 	vAN-20200811_ROOT6-1 	PYT8_13TeV_anchLHC18_AOD_extra 	2020 Aug 13, Train run: All jobs submitted 	request Florian, Steven, ...
# # 2902 	vAN-20200811_ROOT6-1 	PYT8_13TeV_anchLHC18_AOD209 	2020 Aug 13, Train run: All jobs submitted 	request Florian Steven, A...
# # 2901 	vAN-20200811_ROOT6-1 	PYT8_13TeV_anchLHC17_AOD209_extra 	2020 Aug 13, Train run: All jobs submitted 	request Florian, Steven,
# # 2900 	vAN-20200811_ROOT6-1 	PYT8_13TeV_anchLHC17_AOD209 	2020 Aug 13, Train run: All jobs submitted 	request Florian, Steven, ...
# # 2899 	vAN-20200811_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD209_extra2 	2020 Aug 13, Train run: All jobs submitted 	request Florian, Adrian,
# # 2898 	vAN-20200811_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD209_extra 	2020 Aug 13, Train run: All jobs submitted 	request Florian, Adrian,
# # 2897 	vAN-20200811_ROOT6-1 	PYT8_13TeV_anchLHC16_AOD209 	2020 Aug 13, Train run: All jobs submitted 	request Florian, Adrian &...
# ### LHC16 MC
# bash DownScript.sh 2899 2898 2897 -Name_LHC16MC_v12 GA_pp_MC_AOD ?_GammaConvCalo_20 -mergechilds -mergetrains -childsareperiods  -RL_listPCMEDC -totalLog $1
# ### LHC17 MC
# bash DownScript.sh 2900 2901 -Name_LHC17MC_v12 GA_pp_MC_AOD ?_GammaConvCalo_20 -mergechilds -mergetrains -childsareperiods  -RL_listPCMEDC -totalLog $1
# ### LHC18 MC
# bash DownScript.sh 2902 2903 -Name_LHC18MC_v12 GA_pp_MC_AOD ?_GammaConvCalo_20 -mergechilds -mergetrains -childsareperiods  -uSR_DPGEDC -totalLog $1
# ln -sf "$gridpath/LHC18MC_v12/DPGEDC" "$gridpath/LHC18MC_v12/listPCMEDC"
# ln -sf "$gridpath/.GA_pp_MC_AOD-2902/DPGEDC" "$gridpath/.GA_pp_MC_AOD-2902/listPCMEDC"
# ln -sf "$gridpath/.GA_pp_MC_AOD-2903/DPGEDC" "$gridpath/.GA_pp_MC_AOD-2903/listPCMEDC"
# #################   merging   #############################
### Data 16 + 17 + 18
bash DownScript.sh 1495 1496 1497 -Name_Data_pp13TeV_v12 GA_pp_AOD ?_GammaConvCalo ?_GammaCaloMix -mergechilds -mergetrains -childsareperiods -RL_listPCMEDC -noDown -totalLog $1
### MC 16 + 17 + 18
bash DownScript.sh  2903 2902 2901 2900 2899 2898 2897 -Name_MC_pp13TeV_v12 GA_pp_MC_AOD ?_GammaConvCalo ?_GammaCaloMix -mergechilds -mergetrains -childsareperiods  -RL_listPCMEDC -noDown -totalLog $1
# #
#########################################################################################################################################
#########################################################################################################################################
#
#     # 1494 	vAN-20200805_ROOT6-1 	LHC18_13TeV_pass1_AOD208 	2020 Aug 07, Train run finished 	Jens (PCM-)PHOS, Adrian Calo
#     # 1493 	vAN-20200805_ROOT6-1 	LHC17_13TeV_pass1_AOD208 	2020 Aug 07, Train run finished 	Jens (PCM-)PHOS, Adrian Calo
#     # 1492 	vAN-20200805_ROOT6-1 	LHC16_13TeV_pass1_2_AOD208 	2020 Aug 07, Train run finished 	Jens (PCM-)PHOS, Adrian Calo
#     #################   Data   #############################
#     ### LHC16 Data
#     bash DownScript.sh 1492 -Name_LHC16Data_TimeEffi GA_pp_AOD ?_GammaCalo_2039 -mergechilds -childsareperiods  -RL_listPCMEDC -totalLog $1
#     ### LHC17 Data
#     bash DownScript.sh 1493 -Name_LHC17Data_TimeEffi GA_pp_AOD ?_GammaCalo_2039 -mergechilds -childsareperiods  -RL_listPCMEDC -totalLog $1
#     ### LHC18 Data
#     bash DownScript.sh 1494 -Name_LHC18Data_TimeEffi GA_pp_AOD ?_GammaCalo_2039 -mergechilds -childsareperiods  -uSR_DPGEDC -totalLog $1
#     ln -sf "$gridpath/LHC18Data_TimeEffi/DPGEDC" "$gridpath/LHC18Data_TimeEffi/listPCMEDC"
#     ln -sf "$gridpath/.GA_pp_AOD-1494/DPGEDC" "$gridpath/.GA_pp_AOD-1494/listPCMEDC"
#     ### Data 16 + 17 + 18
#     bash DownScript.sh 1492 1493 1494 -Name_Data_pp13TeV_TimeEffi GA_pp_AOD ?_GammaCalo_2039 -mergechilds -mergetrains -childsareperiods -RL_listPCMEDC -noDown -totalLog $1
#
#
# #
#     # 2831-2833 (LowB D5TeVNchMBW)
#     # 2833 	vAN-20200713_ROOT6-1 	LHC18h1_PYT8_13TeV_ancLHC18c_LowB_AOD213 	2020 Jul 14, Train run finished 	request Ana MBW
#     # 2832 	vAN-20200713_ROOT6-1 	LHC17h3_PYT8_13TeV_anchLHC17g_LowB_AOD213 	2020 Jul 14, Train run finished 	request Ana MBW
#     # 2831 	vAN-20200713_ROOT6-1 	LHC17d1_PYT8_13TeV_ancLHC16f_LowB_AOD213 	2020 Jul 14, Train run finished 	request Ana MBW
#     #
#     ##### low B
#     ##################   MC   #############################
#     ### LHC16 MC
#     bash DownScript.sh 2831 -Name_LHC16MC_lowB_v3_2D5TeVNchMBW GA_pp_MC_AOD ?_GammaConvCalo -mergechilds -RL_lowB_DPGTrackIncAccTPCandEMC -totalLog $1
#     ln -sf "$gridpath/LHC16MC_lowB_v3_2D5TeVNchMBW/lowB_DPGTrackIncAccTPCandEMC" "$gridpath/LHC16MC_lowB_v3_2D5TeVNchMBW/listDPGTrackIncAccTPCandEMC"
#     ln -sf "$gridpath/.GA_pp_MC_AOD-2831/lowB_DPGTrackIncAccTPCandEMC" "$gridpath/.GA_pp_MC_AOD-2831/listDPGTrackIncAccTPCandEMC"
#     ### LHC17 MC
#     bash DownScript.sh 2832 -Name_LHC17MC_lowB_v3_2D5TeVNchMBW GA_pp_MC_AOD ?_GammaConvCalo -mergechilds -RL_listDPGTrackIncAccTPCandEMC -totalLog $1
#     ### LHC18 MC
#     bash DownScript.sh 2833 -Name_LHC18MC_lowB_v3_2D5TeVNchMBW GA_pp_MC_AOD ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods  -RL_listDPGTrackIncAccTPCandEMC -totalLog $1
#     bash DownScript.sh  2831 2832 2833 -Name_MC_pp13TeV_lowB_v3_2D5TeVNchMBW GA_pp_MC_AOD ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods -RL_listDPGTrackIncAccTPCandEMC -noDown -totalLog $1
#     #
#
#
#
#     # 2856-2858 LowB 2D13TeVNChMBW
#     # 2858 	vAN-20200722_ROOT6-1 	LHC18h1_PYT8_13TeV_ancLHC18c_LowB_AOD213 	2020 Jul 24, Train run finished 	request Ana MBW
#     # 2857 	vAN-20200722_ROOT6-1 	LHC17h3_PYT8_13TeV_anchLHC17g_LowB_AOD213 	2020 Jul 24, Train run finished 	request Ana MBW
#     # 2856 	vAN-20200722_ROOT6-1 	LHC17d1_PYT8_13TeV_ancLHC16f_LowB_AOD213 	2020 Jul 24, Train run finished 	request Ana MBW
#     # ##### low B
#     ##################   MC   #############################
#     ### LHC16 MC
#     bash DownScript.sh 2856 -Name_LHC16MC_lowB_v3_2D13TeVNChMBW GA_pp_MC_AOD ?_GammaConvCalo -mergechilds -RL_lowB_DPGTrackIncAccTPCandEMC -totalLog $1
#     ln -sf "$gridpath/LHC16MC_lowB_v3_2D13TeVNChMBW/lowB_DPGTrackIncAccTPCandEMC" "$gridpath/LHC16MC_lowB_v3_2D13TeVNChMBW/listDPGTrackIncAccTPCandEMC"
#     ln -sf "$gridpath/.GA_pp_MC_AOD-2856/lowB_DPGTrackIncAccTPCandEMC" "$gridpath/.GA_pp_MC_AOD-2856/listDPGTrackIncAccTPCandEMC"
#     ### LHC17 MC
#     bash DownScript.sh 2857 -Name_LHC17MC_lowB_v3_2D13TeVNChMBW GA_pp_MC_AOD ?_GammaConvCalo -mergechilds -RL_listDPGTrackIncAccTPCandEMC -totalLog $1
#     ### LHC18 MC
#     bash DownScript.sh 2858 -Name_LHC18MC_lowB_v3_2D13TeVNChMBW GA_pp_MC_AOD ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods  -RL_listDPGTrackIncAccTPCandEMC -totalLog $1
#     bash DownScript.sh  2856 2857 2858 -Name_MC_pp13TeV_lowB_v3_2D13TeVNChMBW GA_pp_MC_AOD ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods -RL_listDPGTrackIncAccTPCandEMC -noDown -totalLog $1
#     #
#
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
#         bash DownScript.sh 2849 2850 2851 -Name_LHC16MC_v11_2D13TeVNchMBW GA_pp_MC_AOD ?_.zip ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods  -RL_listPCMEDC -totalLog $1
#         ### LHC17 MC
#         bash DownScript.sh 2852 2853 -Name_LHC17MC_v11_2D13TeVNchMBW GA_pp_MC_AOD ?_.zip ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods  -RL_listPCMEDC -totalLog $1
#         ### LHC18 MC
#         # bash DownScript.sh 2705 2707 -Name_LHC18MC_v10 GA_pp_MC_AOD ?_.zip ?_GammaConvCalo  -mergechilds -mergetrains -childsareperiods  -RL_listDPCIncAccTPC -totalLog $1
#         bash DownScript.sh 2854 2854 -Name_LHC18MC_v11_2D13TeVNchMBW GA_pp_MC_AOD ?_.zip ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods  -uSR_DPGEDC -totalLog $1
#         # ln -sf "$gridpath/LHC18MC_v10/listDPCIncAccTPC" "$gridpath/LHC18MC_v10/listPCMEDC"
#         # ln -sf "$gridpath/.GA_pp_MC_AOD-2705/listDPCIncAccTPC" "$gridpath/.GA_pp_MC_AOD-2705/listPCMEDC"
#         # ln -sf "$gridpath/.GA_pp_MC_AOD-2707/listDPCIncAccTPC" "$gridpath/.GA_pp_MC_AOD-2707/listPCMEDC"
#         ln -sf "$gridpath/LHC18MC_v11_2D13TeVNchMBW/DPGEDC" "$gridpath/LHC18MC_v11_2D13TeVNchMBW/listPCMEDC"
#         ln -sf "$gridpath/.GA_pp_MC_AOD-2854/DPGEDC" "$gridpath/.GA_pp_MC_AOD-2854/listPCMEDC"
#         ln -sf "$gridpath/.GA_pp_MC_AOD-2855/DPGEDC" "$gridpath/.GA_pp_MC_AOD-2855/listPCMEDC"
#         #         #################   merging   #############################
#         #         # ### Data 16 + 17 + 18
                bash DownScript.sh 1378 1379 1380 -Name_Data_pp13TeV_v10 GA_pp_AOD ?_GammaConvCalo ?_GammaCaloMix -mergechilds -mergetrains -childsareperiods -RL_listPCMEDC -noDown -totalLog $1
#         #         # ### MC 16 + 17 + 18
                # bash DownScript.sh  2849 2850 2851 2852 2853 2854 2855 -Name_MC_pp13TeV_v11_2D13TeVNchMBW GA_pp_MC_AOD ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods  -RL_listPCMEDC -noDown -totalLog $1
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
#         bash DownScript.sh 2824 2825 2826 -Name_LHC16MC_v11_2D5TeVNchMBW GA_pp_MC_AOD ?_.zip ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods  -RL_listPCMEDC -totalLog $1
#         ### LHC17 MC
#         bash DownScript.sh 2827 2828 -Name_LHC17MC_v11_2D5TeVNchMBW GA_pp_MC_AOD ?_.zip ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods  -RL_listPCMEDC -totalLog $1
#         ### LHC18 MC
#         bash DownScript.sh 2829 2830 -Name_LHC18MC_v11_2D5TeVNchMBW GA_pp_MC_AOD ?_.zip ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods  -uSR_DPGEDC -totalLog $1
#         # bash DownScript.sh 2829 2830 -Name_LHC18MC_v11_2D5TeVNchMBW GA_pp_MC_AOD ?_.zip ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods  -RL_listDPCIncAccTPC -totalLog $1
#         # ln -sf "$gridpath/LHC18MC_v11_2D5TeVNchMBW/listDPCIncAccTPC" "$gridpath/LHC18MC_v11_2D5TeVNchMBW/listPCMEDC"
#         # ln -sf "$gridpath/.GA_pp_MC_AOD-2829/listDPCIncAccTPC" "$gridpath/.GA_pp_MC_AOD-2829/listPCMEDC"
#         # ln -sf "$gridpath/.GA_pp_MC_AOD-2830/listDPCIncAccTPC" "$gridpath/.GA_pp_MC_AOD-2830/listPCMEDC"
#         ln -sf "$gridpath/LHC18MC_v11_2D5TeVNchMBW/DPGEDC" "$gridpath/LHC18MC_v11_2D5TeVNchMBW/listPCMEDC"
#         ln -sf "$gridpath/.GA_pp_MC_AOD-2829/DPGEDC" "$gridpath/.GA_pp_MC_AOD-2829/listPCMEDC"
#         ln -sf "$gridpath/.GA_pp_MC_AOD-2830/DPGEDC" "$gridpath/.GA_pp_MC_AOD-2830/listPCMEDC"
#         # ### MC 16 + 17 + 18
#         bash DownScript.sh  2824 2825 2826 2827 2828 2829 2830 -Name_MC_pp13TeV_v11_2D5TeVNchMBW GA_pp_MC_AOD ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods  -RL_listPCMEDC -noDown -totalLog $1

######################################################################################################
######################################################################################################





# ##### low B
# ##################   MC   #############################
# ### LHC16 MC
# bash DownScript.sh 2744 -Name_LHC16MC_lowB_NL_v2 GA_pp_MC_AOD ?_GammaConvCalo_33 -mergechilds -RL_lowB_DPGTrackIncAccTPCandEMC -totalLog $1
# ln -sf "$gridpath/LHC16MC_lowB_NL_v2/lowB_DPGTrackIncAccTPCandEMC" "$gridpath/LHC16MC_lowB_NL_v2/listDPGTrackIncAccTPCandEMC"
# ln -sf "$gridpath/.GA_pp_MC_AOD-2744/lowB_DPGTrackIncAccTPCandEMC" "$gridpath/.GA_pp_MC_AOD-2744/listDPGTrackIncAccTPCandEMC"
# ### LHC17 MC
# bash DownScript.sh 2745 -Name_LHC17MC_lowB_NL_v2 GA_pp_MC_AOD ?_GammaConvCalo_33 -mergechilds -RL_listDPGTrackIncAccTPCandEMC -totalLog $1
# ### LHC18 MC
# bash DownScript.sh 2746 -Name_LHC18MC_lowB_NL_v2 GA_pp_MC_AOD ?_GammaConvCalo_33 -mergechilds -mergetrains -childsareperiods  -RL_listDPGTrackIncAccTPCandEMC -totalLog $1
# ##################   Data   #############################
# ### LHC16 Data
# bash DownScript.sh 1405 -Name_LHC16Data_lowB_NL_v2 GA_pp_AOD ?_GammaConvCalo_33 -mergechilds  -RL_lowB_DPGTrackIncAccTPCandEMC -totalLog $1
# ln -sf "$gridpath/LHC16Data_lowB_NL_v2/lowB_DPGTrackIncAccTPCandEMC" "$gridpath/LHC16MC_lowB_NL_v2/listDPGTrackIncAccTPCandEMC"
# ln -sf "$gridpath/.GA_pp_AOD-1405/lowB_DPGTrackIncAccTPCandEMC" "$gridpath/.GA_pp_AOD-1405/listDPGTrackIncAccTPCandEMC"
# ### LHC17 Data
# bash DownScript.sh 1406 -Name_LHC17Data_lowB_NL_v2 GA_pp_AOD ?_GammaConvCalo_33 -mergechilds  -RL_listDPGTrackIncAccTPCandEMC -totalLog $1
# ### LHC18 Data
# bash DownScript.sh 1407 -Name_LHC18Data_lowB_NL_v2 GA_pp_AOD ?_GammaConvCalo_33 -mergechilds -childsareperiods  -RL_listDPGTrackIncAccTPCandEMC -totalLog $1
# ##################   merging   #############################
# ### Data 16 + 17 + 18
# bash DownScript.sh  1407 1406 1405 -Name_Data_pp13TeV_lowB GA_pp_AOD ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods -RL_listDPGTrackIncAccTPCandEMC -noDown -totalLog $1
# ### MC 16 + 17 + 18
# bash DownScript.sh  2746 2745 2744 -Name_MC_pp13TeV_lowB GA_pp_MC_AOD ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods -RL_listDPGTrackIncAccTPCandEMC -noDown -totalLog $1



#
# ##################   AOD Validation   #############################
# ##################   Data   #############################
# bash DownScript.sh 1404 -Name_LHC18Data_lowB_AOD_validation GA_pp_AOD ?_GammaConvCalo_33 -mergechilds -childsareperiods  -RL_listDPGTrackIncAccTPCandEMC -totalLog $1
# bash DownScript.sh 1403 -Name_LHC18Data_AOD_validation GA_pp_AOD ?_GammaConvCalo_2 ?_GammaCalo_2039 -mergechilds -childsareperiods  -RL_listDPGIncTPC -totalLog $1
# ##################   MC   #############################
# bash DownScript.sh 2743 -Name_LHC18MC_lowB_AOD_validation GA_pp_MC_AOD ?_GammaConvCalo_33 -mergechilds -mergetrains -childsareperiods  -RL_listDPGTrackIncAccTPCandEMC -totalLog $1
# bash DownScript.sh 2742 -Name_LHC18MC_AOD_validation GA_pp_MC_AOD ?_GammaConvCalo_2 -mergechilds -mergetrains -childsareperiods  -RL_listDPCIncAccTPC -totalLog $1
#

    #
    # #################   Data   #############################
    # ### LHC16 Data
    # bash DownScript.sh 1378 -Name_LHC16Data_v10 GA_pp_AOD ?_.zip ?_GammaConvCalo_20 ?_GammaCaloMix -mergechilds -childsareperiods  -RL_listPCMEDC -totalLog $1
    # ### LHC17 Data
    # bash DownScript.sh 1379 -Name_LHC17Data_v10 GA_pp_AOD ?_.zip ?_GammaConvCalo_20 ?_GammaCaloMix -mergechilds -childsareperiods  -RL_listPCMEDC -totalLog $1
    # ### LHC18 Data
    # bash DownScript.sh 1380 -Name_LHC18Data_v10 GA_pp_AOD ?_.zip ?_GammaConvCalo_20 ?_GammaCaloMix -mergechilds -childsareperiods  -uSR_DPGEDC -totalLog $1
    # bash DownScript.sh 1380 -Name_LHC18Data_v10 GA_pp_AOD ?_.zip ?_GammaConvCalo_20 ?_GammaCaloMix -mergechilds -childsareperiods  -RL_listDPGIncTPC -totalLog $1
    # ln -sf "$gridpath/LHC18Data_v10/listDPGIncTPC" "$gridpath/LHC18Data_v10/listPCMEDC"
    # ln -sf "$gridpath/.GA_pp_AOD-1380/listDPGIncTPC" "$gridpath/.GA_pp_AOD-1380/listPCMEDC"# ln -sf "$gridpath/LHC18Data_v10/listDPGIncTPC" "$gridpath/LHC18Data_v10/listPCMEDC"
    # ln -sf "$gridpath/.GA_pp_AOD-1380/listDPGIncTPC" "$gridpath/.GA_pp_AOD-1380/listPCMEDC"
    # ln -sf "$gridpath/LHC18Data_v10/DPGEDC" "$gridpath/LHC18Data_v10/listPCMEDC"
    # ln -sf "$gridpath/.GA_pp_AOD-1380/DPGEDC" "$gridpath/.GA_pp_AOD-1380/listPCMEDC"# ln -sf "$gridpath/LHC18Data_v10/listDPGIncTPC" "$gridpath/LHC18Data_v10/listPCMEDC"
    # ln -sf "$gridpath/.GA_pp_AOD-1380/DPGEDC" "$gridpath/.GA_pp_AOD-1380/listPCMEDC"
    # ##################   MC   #############################
    # ### LHC16 MC
    # bash DownScript.sh 2700 2701 2702 -Name_LHC16MC_v10 GA_pp_MC_AOD ?_.zip ?_GammaConvCalo_20 ?_GammaCaloMix -mergechilds -mergetrains -childsareperiods  -RL_listPCMEDC -totalLog $1
    # ### LHC17 MC
    # bash DownScript.sh 2703 2704 -Name_LHC17MC_v10 GA_pp_MC_AOD ?_.zip ?_GammaConvCalo_20 ?_GammaCaloMix -mergechilds -mergetrains -childsareperiods  -RL_listPCMEDC -totalLog $1
    # ### LHC18 MC
    # bash DownScript.sh 2705 2707 -Name_LHC18MC_v10 GA_pp_MC_AOD ?_.zip ?_GammaConvCalo_20 ?_GammaCaloMix -mergechilds -mergetrains -childsareperiods  -uSR_DPGEDC -totalLog $1
    # bash DownScript.sh 2705 2707 -Name_LHC18MC_v10 GA_pp_MC_AOD ?_.zip ?_GammaConvCalo_20 ?_GammaCaloMix -mergechilds -mergetrains -childsareperiods  -RL_listDPCIncAccTPC -totalLog $1
    # ln -sf "$gridpath/LHC18MC_v10/listDPCIncAccTPC" "$gridpath/LHC18MC_v10/listPCMEDC"
    # ln -sf "$gridpath/.GA_pp_MC_AOD-2705/listDPCIncAccTPC" "$gridpath/.GA_pp_MC_AOD-2705/listPCMEDC"
    # ln -sf "$gridpath/.GA_pp_MC_AOD-2707/listDPCIncAccTPC" "$gridpath/.GA_pp_MC_AOD-2707/listPCMEDC"
    # ln -sf "$gridpath/LHC18MC_v10/DPGEDC" "$gridpath/LHC18MC_v10/listPCMEDC"
    # ln -sf "$gridpath/.GA_pp_MC_AOD-2705/DPGEDC" "$gridpath/.GA_pp_MC_AOD-2705/listPCMEDC"
    # ln -sf "$gridpath/.GA_pp_MC_AOD-2707/DPGEDC" "$gridpath/.GA_pp_MC_AOD-2707/listPCMEDC"
    # #################   merging   #############################
    # # ### Data 16 + 17 + 18
    # bash DownScript.sh 1378 1379 1380 -Name_Data_pp13TeV_v10 GA_pp_AOD ?_GammaConvCalo ?_GammaCaloMix -mergechilds -mergetrains -childsareperiods -RL_listPCMEDC -noDown -totalLog $1
    # # ### MC 16 + 17 + 18
    # bash DownScript.sh  2700 2701 2702 2703 2704 2705 2707 -Name_MC_pp13TeV_v10 GA_pp_MC_AOD ?_GammaConvCalo ?_GammaCaloMix -mergechilds -mergetrains -childsareperiods  -RL_listPCMEDC -noDown -totalLog $1
    #


    #################  Skin Data   #############################
    # ### LHC16 Data
    # bash DownScript.sh 1388 1391 -Name_SkimLHC16Data_v10 GA_pp_AOD  ?_.zip ?_GammaConvCalo_20 ?_GammaConvCalo_3 -mergechilds -mergetrains -childsareperiods  -RL_listDPGEDCtrigger -totalLog $1
    # ### LHC17 Data
    # bash DownScript.sh 1389 1392 -Name_SkimLHC17Data_v10 GA_pp_AOD  ?_.zip ?_GammaConvCalo_20 ?_GammaConvCalo_3 -mergechilds -mergetrains -childsareperiods  -RL_listDPGEDCtrigger -totalLog $1
    # ### LHC18 Data
    # bash DownScript.sh 1390 1393 -Name_SkimLHC18Data_v10 GA_pp_AOD  ?_.zip ?_GammaConvCalo_20 ?_GammaConvCalo_3 -mergechilds -mergetrains -childsareperiods  -RL_listEDC_trigger -totalLog $1
    # ln -sf "$gridpath/SkimLHC18Data_v10/listEDC_trigger" "$gridpath/SkimLHC18Data_v10/listDPGEDCtrigger"
    # ln -sf "$gridpath/.GA_pp_AOD-1390/listEDC_trigger" "$gridpath/.GA_pp_AOD-1390/listDPGEDCtrigger"
    # ln -sf "$gridpath/.GA_pp_AOD-1393/listEDC_trigger" "$gridpath/.GA_pp_AOD-1393/listDPGEDCtrigger"
    # ##############   JJ MC   #############################
    # ### LHC16 MC
    # bash DownScript.sh 2697 GA_pp_MC_AOD ?_.zip ?_GammaConvCalo_20 ?_GammaConvCalo_3 -mergechilds -mergetrains -childsareperiods -IsJJ -totalLog $1
    # bash DownScript.sh 2710 GA_pp_MC_AOD ?_.zip ?_GammaConvCalo_20 ?_GammaConvCalo_3 -mergechilds -mergetrains -childsareperiods -IsJJ -totalLog $1
    # bash DownScript.sh 2673 GA_pp_MC_AOD ?_.zip ?_GammaConvCalo_20 ?_GammaConvCalo_3 -mergechilds -mergetrains -childsareperiods -IsJJ -totalLog $1
    # bash DownScript.sh 2697 2710 2673 -Name_JJLHC16MC_v10 GA_pp_MC_AOD ?_.zip ?_GammaConvCalo_20 ?_GammaConvCalo_3 -mergechilds -mergetrains -childsareperiods -RL_default -noDown -totalLog $1
    # ### LHC17 MC
    # bash DownScript.sh 2698 GA_pp_MC_AOD  ?_.zip ?_GammaConvCalo_20 ?_GammaConvCalo_3 -mergechilds -mergetrains -childsareperiods -IsJJ -totalLog $1
    # bash DownScript.sh 2709 GA_pp_MC_AOD  ?_.zip ?_GammaConvCalo_20 ?_GammaConvCalo_3 -mergechilds -mergetrains -childsareperiods -IsJJ -totalLog $1
    # bash DownScript.sh 2674 GA_pp_MC_AOD  ?_.zip ?_GammaConvCalo_20 ?_GammaConvCalo_3 -mergechilds -mergetrains -childsareperiods -IsJJ -totalLog $1
    # bash DownScript.sh 2698 2709 2674 -Name_JJLHC17MC_v10 GA_pp_MC_AOD  ?_.zip ?_GammaConvCalo_20 ?_GammaConvCalo_3 -mergechilds -mergetrains -childsareperiods -RL_default -noDown -totalLog $1
    # ### LHC18 MC
    # bash DownScript.sh 2699 GA_pp_MC_AOD ?_.zip ?_GammaConvCalo_20 ?_GammaConvCalo_3 -mergechilds -mergetrains -childsareperiods -IsJJ -totalLog $1
    # bash DownScript.sh 2708 GA_pp_MC_AOD ?_.zip ?_GammaConvCalo_20 ?_GammaConvCalo_3 -mergechilds -mergetrains -childsareperiods -IsJJ -totalLog $1
    # bash DownScript.sh 2675 GA_pp_MC_AOD ?_.zip ?_GammaConvCalo_20 ?_GammaConvCalo_3 -mergechilds -mergetrains -childsareperiods -IsJJ -totalLog $1
    # bash DownScript.sh 2699 2708 2675 -Name_JJLHC18MC_v10 GA_pp_MC_AOD ?_.zip ?_GammaConvCalo_20 ?_GammaConvCalo_3 -mergechilds -mergetrains -childsareperiods -RL_default -noDown -totalLog $1
    # #################   merging   #############################
    # # ### Data 16 + 17 + 18s
    # # bash DownScript.sh 1388 1391 1389 1392 1390 1393 -Name_SkimData_pp13TeV_v10 GA_pp_AOD ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods  -RL_listDPGEDCtrigger -noDown -totalLog $1
    # ### MC 16 + 17 + 18
    # bash DownScript.sh 2697 2710 2673 2698 2709 2674 2699 2708 2675 -Name_JJMC_pp13TeV_v10 GA_pp_MC_AOD ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods  -RL_default -noDown -totalLog $1



    # bash DownScript.sh 1260 1261 1348 -Name_Data_pp13TeV_v7 GA_pp_AOD ?_GammaConvCalo_2020 -mergechilds -mergetrains -childsareperiods  -RL_listPCMEDC -noDown -totalLog $1
    #
    # ##################   Data   #############################
    # ### LHC16 Data
    # bash DownScript.sh 1367 -Name_LHC16Data_v9 GA_pp_AOD ?_GammaConvCalo_20 -mergechilds -childsareperiods  -RL_listPCMEDC -totalLog $1
    # ### LHC17 Data
    # bash DownScript.sh 1368 -Name_LHC17Data_v9 GA_pp_AOD ?_GammaConvCalo_20 -mergechilds -childsareperiods  -RL_listPCMEDC -totalLog $1
    # ### LHC18 Data
    # bash DownScript.sh 1369 -Name_LHC18Data_v9 GA_pp_AOD ?_GammaConvCalo_20 -mergechilds -childsareperiods  -RL_listDPGIncTPC -totalLog $1
    # ##################   MC   #############################
    # # ### LHC16 MC
    # bash DownScript.sh 2580 2581 2582 -Name_LHC16MC_v9 GA_pp_MC_AOD ?_GammaConvCalo_20 -mergechilds -mergetrains -childsareperiods  -RL_listPCMEDC -totalLog $1
    # # ### LHC17 MC
    # bash DownScript.sh 2583 2584 -Name_LHC17MC_v9 GA_pp_MC_AOD ?_GammaConvCalo_20 -mergechilds -mergetrains -childsareperiods  -RL_listPCMEDC -totalLog $1
    # # ### LHC18 MC
    # bash DownScript.sh 2585 2586 -Name_LHC18MC_v9 GA_pp_MC_AOD ?_GammaConvCalo_20 -mergechilds -mergetrains -childsareperiods  -RL_listDPCIncAccTPC -totalLog $1
    # #################   merging   #############################
    # ### Data 16 + 17 + 18
    # bash DownScript.sh 1367 1368 1369 -Name_Data_pp13TeV_v9 GA_pp_AOD ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods -RL_listPCMEDC -noDown -totalLog $1
    # ### MC 16 + 17 + 18
    # bash DownScript.sh  2580 2581 2582 2583 2584 2585 2586 -Name_MC_pp13TeV_v9 GA_pp_MC_AOD ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods  -RL_listPCMEDC -noDown -totalLog $1
    #
    #
    #
    # ##############   JJ MC   #############################
    # ## LHC16 MC
    # bash DownScript.sh 2558 -Name_JJLHC16MC_v9 GA_pp_MC_AOD ?_GammaConvCalo_20 -mergechilds -childsareperiods -IsJJ -totalLog $1
    # ## LHC17 MC
    # bash DownScript.sh 2559 -Name_JJLHC17MC_v9 GA_pp_MC_AOD  ?_GammaConvCalo_20 -mergechilds -childsareperiods -IsJJ -totalLog $1
    # ## LHC18 MC
    # bash DownScript.sh 2560 -Name_JJLHC18MC_v9 GA_pp_MC_AOD ?_GammaConvCalo_20 -mergechilds -childsareperiods -IsJJ -totalLog $1
    # #################  Skin Data   #############################
    # # ## LHC16 Data
    # # bash DownScript.sh 1263 -Name_SkimLHC16Data_v9 GA_pp_AOD  ?_GammaConvCalo_20 -mergechilds -childsareperiods  -RL_listDPGEDCtrigger -totalLog $1
    # # ## LHC17 Data
    # # bash DownScript.sh 1264 -Name_SkimLHC17Data_v9 GA_pp_AOD  ?_GammaConvCalo_20 -mergechilds -childsareperiods  -RL_listDPGEDCtrigger -totalLog $1
    # # ## LHC18 Data
    # # bash DownScript.sh 1265 -Name_SkimLHC18Data_v6 GA_pp_AOD  ?_GammaConvCalo_20 -mergechilds -childsareperiods  -RL_listEDC_trigger -totalLog $1
    # #################   merging   #############################
    # # ### Data 16 + 17 + 18s
    # # bash DownScript.sh 1263 1264 1265 -Name_SkimData_pp13TeV_v9 GA_pp_AOD ?_GammaConvCalo_20 -mergechilds -mergetrains -childsareperiods  -RL_listDPGEDCtrigger -noDown -totalLog $1
    # ### MC 16 + 17 + 18
    # bash DownScript.sh 2558 2559 2560 -Name_JJMC_pp13TeV_v9 GA_pp_MC_AOD ?_GammaConvCalo_20 -mergechilds -mergetrains -childsareperiods  -RL_default -noDown -totalLog $1
    #
    #
    #


    # 2545, 2543, 2535, 2530
    # 2729, 2728, 2727, 2726
# bash DownScript.sh 2484 -Name_DecayJJLHC16MC_JJhigh_DCal GA_pp_MC_AOD ?_Gamma -IsJJ_8 -totalLog $1
#############################################   need Rerun    #########################################################
    ##################   Decay MCs   #############################
    # bash DownScript.sh 2539 -Name_DecayJJhigh_13TeV_DCAL_LHC17_v10 GA_pp_MC_AOD ?_GammaConvCalo_2003 -IsJJ_8 -totalLog $1
    # bash DownScript.sh 2537 -Name_DecayJJlow_13TeV_DCal_LHC17_v10 GA_pp_MC_AOD ?_GammaConvCalo_2003 -IsJJ_8 -totalLog $1
    # bash DownScript.sh 2727 GA_pp_MC_AOD ?_GammaConvCalo -IsJJ_8 -totalLog $1
    # bash DownScript.sh 2728 GA_pp_MC_AOD ?_GammaConvCalo -IsJJ_8 -totalLog $1
    # bash DownScript.sh 2727 2728 -Name_DecayJJhigh_13TeV_DCAL_LHC18_v10 GA_pp_MC_AOD ?_GammaConvCalo -mergetrains -RL_default -noDown -totalLog $1
    # bash DownScript.sh 2534 GA_pp_MC_AOD ?_GammaConvCalo_2003 -IsJJ_8 -totalLog $1
    # bash DownScript.sh 2544 GA_pp_MC_AOD ?_GammaConvCalo_2003 -IsJJ_8 -totalLog $1
    # bash DownScript.sh 2534 2544 -Name_DecayJJlow_13TeV_DCAL_LHC18_v10 GA_pp_MC_AOD ?_GammaConvCalo -mergetrains -RL_default -noDown -totalLog $1
    # bash DownScript.sh 2523 -Name_DecayJJhigh_13TeV_DCal_LHC16_v10 GA_pp_MC_AOD ?_GammaConvCalo_2003 -IsJJ_8 -totalLog $1
    # bash DownScript.sh 2522 -Name_DecayJJlow_13TeV_DCal_LHC16_v10 GA_pp_MC_AOD ?_GammaConvCalo_2003 -IsJJ_8 -totalLog $1
    # bash DownScript.sh 2726 GA_pp_MC_AOD ?_GammaConvCalo_2003 -IsJJ_8 -totalLog $1
    # bash DownScript.sh 2729 GA_pp_MC_AOD ?_GammaConvCalo_2003 -IsJJ_8 -totalLog $1
    # bash DownScript.sh 2726 2729 -Name_DecayJJhigh_13TeV_EMCal_LHC18_v10 GA_pp_MC_AOD ?_GammaConvCalo -mergetrains -RL_default -noDown -totalLog $1
    # bash DownScript.sh 2529 GA_pp_MC_AOD ?_GammaConvCalo_2003 -IsJJ_8 -totalLog $1
    # bash DownScript.sh 2546 GA_pp_MC_AOD ?_GammaConvCalo_2003 -IsJJ_8 -totalLog $1
    # bash DownScript.sh 2529 2546 -Name_DecayJJlow_13TeV_EMCal_LHC18_v10 GA_pp_MC_AOD ?_GammaConvCalo -mergetrains -RL_default -noDown -totalLog $1
    # bash DownScript.sh 2528 -Name_DecayJJhigh_13TeV_EMCal_LHC17_v10 GA_pp_MC_AOD ?_GammaConvCalo_2003 -IsJJ_8 -totalLog $1
    # bash DownScript.sh 2527 -Name_DecayJJlow_13TeV_EMCal_LHC17_v10 GA_pp_MC_AOD ?_GammaConvCalo_2003 -IsJJ_8 -totalLog $1
    # bash DownScript.sh 2525 -Name_DecayJJhigh_13TeV_EMCal_LHC16_v10 GA_pp_MC_AOD ?_GammaConvCalo_2003 -IsJJ_8 -totalLog $1
    # bash DownScript.sh 2524 -Name_DecayJJlow_13TeV_EMCal_LHC16_v10 GA_pp_MC_AOD ?_GammaConvCalo_2003 -IsJJ_8 -totalLog $1

    # bash DownScript.sh 2523 2525 -Name_DecayJJhigh_13TeV_LHC16_v10 GA_pp_MC_AOD ?_GammaConvCalo -mergetrains -RL_default -noDown -totalLog $1
    # bash DownScript.sh 2522 2524 -Name_DecayJJlow_13TeV_LHC16_v10 GA_pp_MC_AOD ?_GammaConvCalo -mergetrains -RL_default -noDown -totalLog $1

    # ##################   Decay MCs Sys Var  #############################
    # bash DownScript.sh 2727 GA_pp_MC_AOD ?_.zip ?_GammaConvCalo -IsJJ_8 -totalLog $1
    # bash DownScript.sh 2728 GA_pp_MC_AOD ?_.zip ?_GammaConvCalo -IsJJ_8 -totalLog $1
    # bash DownScript.sh 2727 2728 -Name_DecayJJhigh_13TeV_DCAL_v10_var GA_pp_MC_AOD ?_GammaConvCalo -mergetrains -RL_default -noDown -totalLog $1
    # bash DownScript.sh 2534 -Name_DecayJJlow_13TeV_DCAL_v10_var GA_pp_MC_AOD ?_.zip ?_GammaConvCalo -IsJJ_8 -totalLog $1
    # bash DownScript.sh 2726 GA_pp_MC_AOD ?_.zip ?_GammaConvCalo -IsJJ_8 -totalLog $1
    # bash DownScript.sh 2729 GA_pp_MC_AOD ?_.zip ?_GammaConvCalo -IsJJ_8 -totalLog $1
    # bash DownScript.sh 2726 2729 -Name_DecayJJhigh_13TeV_EMCal_v10_var GA_pp_MC_AOD ?_GammaConvCalo -mergetrains -RL_default -noDown -totalLog $1
    # bash DownScript.sh 2529 -Name_DecayJJlow_13TeV_EMCal_v10_var GA_pp_MC_AOD ?_.zip ?_GammaConvCalo -IsJJ_8 -totalLog $1
    #

    # # ##################   MC Sys Var  #############################
    # # ##################   Data   #############################
    # ### LHC18 Data
    # bash DownScript.sh 1348 -Name_LHC18Data_var GA_pp_AOD ?_.zip ?_GammaConvCalo_20 -mergechilds -childsareperiods  -RL_listDPGIncTPC -totalLog $1
    # # ### LHC18 MC
    # # bash DownScript.sh 2512 -Name_LHC18MC_var GA_pp_MC_AOD ?_.zip ?_GammaConvCalo_20 -mergechilds -mergetrains -childsareperiods  -RL_listDPCIncAccTPC -totalLog $1
    # bash DownScript.sh 2513 2512 -Name_LHC18MC_var GA_pp_MC_AOD ?_.zip ?_GammaConvCalo_20 -mergechilds -mergetrains -childsareperiods  -RL_listDPCIncAccTPC -totalLog $1
    #
    # ##############   JJ MC Sys var  #############################
    # ### LHC18 MC
    # bash DownScript.sh 2547 2515 -Name_JJLHC18MC_var GA_pp_MC_AOD ?_.zip ?_GammaConvCalo_3 -mergechilds -childsareperiods -IsJJ -totalLog $1
    # bash DownScript.sh 2547 2515 -Name_JJLHC18MC_var GA_pp_MC_AOD ?_.zip ?_GammaConvCalo_3 -RL_default -mergetrains -mergechilds -childsareperiods -noDown -totalLog $1
    # #################  Skin Data Sys Var   #############################
    # ### LHC18 Data
    # bash DownScript.sh 1350 1360 -Name_SkimLHC18Data_var GA_pp_AOD ?_.zip ?_GammaConvCalo_3 -mergetrains -mergechilds -childsareperiods  -RL_listEDC_trigger -totalLog $1
    #


#######################################################################################################
#######################################################################################################

    ##################   Sigma V2   #############################
    ### LHC18 Data
    # bash DownScript.sh 2617 -Name_LHC18Data_Sigma_V2 GA_pp ?_zip -mergechilds -childsareperiods -RL_DPGTrackCalo -RL_listDPGCalo -RL_listDPGTrackAndCalo -totalLog $1
    ### LHC18 MC
    # bash DownScript.sh 3832 -Name_LHC18MC_Sigma_V2 GA_pp_MC ?_Sigma -mergechilds -childsareperiods -RL_listDPGAndCalo -RL_listDPGTrackAndCalo -totalLog $1



    ##################   Sigma V1   #############################
    ### LHC18 Data
    # bash DownScript.sh 2616 -Name_LHC18Data_Sigma GA_pp ?_Sigma -mergechilds -childsareperiods -RL_DPGTrackCalo -RL_listDPGCalo -RL_listDPGTrackAndCalo -totalLog $1
    ### LHC18 MC
    # bash DownScript.sh 3829 -Name_LHC18MC_Sigma GA_pp_MC ?_Sigma -mergechilds -childsareperiods -RL_listDPGAndCalo -RL_listDPGTrackAndCalo -totalLog $1




    ####                 new Trains v8
    #
    ##### low B
    # ##################   MC   #############################
    # # ### LHC16 MC
    # bash DownScript.sh 2381 -Name_LHC16MC_lowB GA_pp_MC_AOD ?_GammaConvCalo -mergechilds -RL_lowB_DPGTrackIncAccTPCandEMC -totalLog $1
    # ln -sf "$gridpath/LHC16MC_lowB/lowB_DPGTrackIncAccTPCandEMC" "$gridpath/LHC16MC_lowB/listDPGTrackIncAccTPCandEMC"
    # ln -sf "$gridpath/.GA_pp_MC_AOD-2381/lowB_DPGTrackIncAccTPCandEMC" "$gridpath/.GA_pp_MC_AOD-2381/listDPGTrackIncAccTPCandEMC"
    # # ### LHC17 MC
    # bash DownScript.sh 2341 -Name_LHC17MC_lowB GA_pp_MC_AOD ?_GammaConvCalo -mergechilds -RL_listDPGTrackIncAccTPCandEMC -totalLog $1
    # # ### LHC18 MC
    # bash DownScript.sh 2342 -Name_LHC18MC_lowB GA_pp_MC_AOD ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods  -RL_listDPGTrackIncAccTPCandEMC -totalLog $1
    # ##################   Data   #############################
    # ### LHC16 Data
    # bash DownScript.sh 1289 -Name_LHC16Data_lowB GA_pp_AOD ?_GammaConvCalo -mergechilds  -RL_lowB_DPGTrackIncAccTPCandEMC -totalLog $1
    # ln -sf "$gridpath/LHC16Data_lowB/lowB_DPGTrackIncAccTPCandEMC" "$gridpath/LHC16Data_lowB/listDPGTrackIncAccTPCandEMC"
    # ln -sf "$gridpath/.GA_pp_AOD-1289/lowB_DPGTrackIncAccTPCandEMC" "$gridpath/.GA_pp_AOD-1289/listDPGTrackIncAccTPCandEMC"
    # ### LHC17 Data
    # bash DownScript.sh 1278 -Name_LHC17Data_lowB GA_pp_AOD ?_GammaConvCalo -mergechilds  -RL_listDPGTrackIncAccTPCandEMC -totalLog $1
    # ### LHC18 Data
    # bash DownScript.sh 1279 -Name_LHC18Data_lowB GA_pp_AOD ?_GammaConvCalo -mergechilds -childsareperiods  -RL_listDPGTrackIncAccTPCandEMC -totalLog $1
    ##################   merging   #############################
    ### Data 16 + 17 + 18
    # bash DownScript.sh  1289 1278 1279 -Name_Data_pp13TeV_lowB_MBW GA_pp_AOD ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods  -RL_lowB_DPGTrackIncAccTPCandEMC -RL_listDPGTrackIncAccTPCandEMC -noDown -totalLog $1
    ### MC 16 + 17 + 18
    # bash DownScript.sh  2381 2341 2342 -Name_MC_pp13TeV_lowB GA_pp_MC_AOD ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods -RL_lowB_DPGTrackIncAccTPCandEMC -RL_listDPGTrackIncAccTPCandEMC -noDown -totalLog $1

    # Norm b
    # ##################   MC   #############################
    # # ### LHC16 MC
    # bash DownScript.sh 2378 2338 2377 -Name_LHC16MC_v8 GA_pp_MC_AOD ?_GammaConvCalo_20 -mergechilds -mergetrains -childsareperiods  -RL_listPCMEDC -totalLog $1
    # # ### LHC17 MC
    # bash DownScript.sh 2339 2379 -Name_LHC17MC_v8 GA_pp_MC_AOD ?_GammaConvCalo_20 -mergechilds -mergetrains -childsareperiods  -RL_listPCMEDC -totalLog $1
    # # ### LHC18 MC
    # bash DownScript.sh 2380 2340 -Name_LHC18MC_v8 GA_pp_MC_AOD ?_GammaConvCalo_20 -mergechilds -mergetrains -childsareperiods  -RL_listDPCIncAccTPC -totalLog $1
    # ##################   Data   #############################
    # ### LHC16 Data
    # bash DownScript.sh 1275 -Name_LHC16Data_v8 GA_pp_AOD ?_GammaConvCalo_20 -mergechilds -childsareperiods  -RL_listPCMEDC -totalLog $1
    # ### LHC17 Data
    # bash DownScript.sh 1276 -Name_LHC17Data_v8 GA_pp_AOD ?_GammaConvCalo_20 -mergechilds -childsareperiods  -RL_listPCMEDC -totalLog $1
    # ### LHC18 Data
    # bash DownScript.sh 1277 -Name_LHC18Data_v8 GA_pp_AOD ?_GammaConvCalo_20 -mergechilds -childsareperiods  -RL_listDPGIncTPC -totalLog $1
    ##################   merging   #############################
    # ### Data 16 + 17 + 18
    # bash DownScript.sh 1275 1276 1277 -Name_Data_pp13TeV_v8 GA_pp_AOD ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods -RL_listPCMEDC -noDown -totalLog $1
    # ### MC 16 + 17 + 18
    # bash DownScript.sh  2378 2338 2377 2339 2379 2380 2340 -Name_MC_pp13TeV_v8 GA_pp_MC_AOD ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods  -RL_listPCMEDC -noDown -totalLog $1


    # ##################   MC   #############################
    # ### LHC16 MC
    # bash DownScript.sh 2315 2316 2317 -Name_LHC16MC_v7 GA_pp_MC_AOD ?_GammaCaloMix ?_GammaConvCalo_20 -mergechilds -mergetrains -childsareperiods  -RL_listPCMEDC -totalLog $1
    # ### LHC17 MC
    # bash DownScript.sh 2318 2319 -Name_LHC17MC_v7 GA_pp_MC_AOD ?_GammaCaloMix ?_GammaConvCalo_20 -mergechilds -mergetrains -childsareperiods  -RL_listPCMEDC -totalLog $1
    # ### LHC18 MC
    # bash DownScript.sh 2320 2321 -Name_LHC18MC_v7 GA_pp_MC_AOD ?_GammaCaloMix ?_GammaConvCalo_20 -mergechilds -mergetrains -childsareperiods  -RL_listDPCIncAccTPC -totalLog $1
    # ##################   Data   #############################
    # ### LHC16 Data
    # bash DownScript.sh 1260 -Name_LHC16Data_v7 GA_pp_AOD ?_GammaCaloMix ?_GammaConvCalo_20 -mergechilds -childsareperiods  -RL_listPCMEDC -totalLog $1
    # ### LHC17 Data
    # bash DownScript.sh 1261 -Name_LHC17Data_v7 GA_pp_AOD ?_GammaCaloMix ?_GammaConvCalo_20 -mergechilds -childsareperiods  -RL_listPCMEDC -totalLog $1
    # ### LHC18 Data
    # bash DownScript.sh 1262 -Name_LHC18Data_v7 GA_pp_AOD ?_GammaCaloMix ?_GammaConvCalo_20 -mergechilds -childsareperiods  -RL_listDPGIncTPC -totalLog $1
    # ##################   merging   #############################
    # ### Data 16 + 17 + 18
    # bash DownScript.sh 1260 1261 1262 -Name_Data_pp13TeV_16_17_18_v7 GA_pp_AOD ?_Gamma -mergechilds -mergetrains -childsareperiods  -RL_listPCMEDC -noDown -totalLog $1
    # ### MC 16 + 17 + 18
    # bash DownScript.sh 2315 2316 2317 2318 2319 2320 2321 -Name_MC_pp13TeV_16_17_18_v7 GA_pp_MC_AOD ?_GammaCaloMix ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods -RL_listPCMEDC -noDown -totalLog $1

    ###############   JJ MC   #############################
    ### LHC16 MC
    # bash DownScript.sh 2322 -Name_JJLHC16MC_v7 GA_pp_MC_AOD ?_GammaCaloMix ?_GammaConvCalo_20 -mergechilds -childsareperiods -IsJJ -totalLog $1
    ### LHC17 MC
    # bash DownScript.sh 2323 -Name_JJLHC17MC_v7 GA_pp_MC_AOD ?_GammaCaloMix ?_GammaConvCalo_20 -mergechilds -childsareperiods -IsJJ -totalLog $1
    ### LHC18 MC
    # bash DownScript.sh 2324 -Name_JJLHC18MC_v7 GA_pp_MC_AOD ?_Gamma -mergechilds -childsareperiods -IsJJ -totalLog $1
    # #################  Skin Data   #############################
    ### LHC16 Data
    # bash DownScript.sh 1263 -Name_SkimLHC16Data_v7 GA_pp_AOD ?_GammaCaloMix ?_GammaConvCalo_20 -mergechilds -childsareperiods  -RL_listDPGEDCtrigger -totalLog $1
    ### LHC17 Data
    # bash DownScript.sh 1264 -Name_SkimLHC17Data_v7 GA_pp_AOD ?_GammaCaloMix ?_GammaConvCalo_20 -mergechilds -childsareperiods  -RL_listDPGEDCtrigger -totalLog $1
    ### LHC18 Data
    # bash DownScript.sh 1265 -Name_SkimLHC18Data_v6 GA_pp_AOD ?_GammaCaloMix ?_GammaConvCalo_20 -mergechilds -childsareperiods  -RL_listEDC_trigger -totalLog $1
    # #################   merging   #############################
    # ### Data 16 + 17 + 18s
    # bash DownScript.sh 1263 1264 1265 -Name_SkimData_pp13TeV_v7 GA_pp_AOD ?_GammaCaloMix ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods  -RL_listDPGEDCtrigger -noDown -totalLog $1
    # ### MC 16 + 17 + 18
    # bash DownScript.sh 2322 2323 2324 -Name_JJMC_pp13TeV_v7 GA_pp_MC_AOD ?_GammaCaloMix ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods  -RL_default -noDown -totalLog $1
    #
    #

#####################################################################################################################################################################
#####################################################################################################################################################################

    # ### LHC18 Data
    # bash DownScript.sh 1203 -Name_LHC18Data_CaloMix GA_pp_AOD ?_GammaCaloMix_1 -mergechilds -childsareperiods  -RL_listDPGCalo -totalLog $1

#####                 new Trains v6
# #####                 with recalib and MBW
# #####                 including QA
# ##################   Data   #############################
# ### LHC16 Data
# bash DownScript.sh 1181 -Name_LHC16Data_v6 GA_pp_AOD ?_GammaConvCalo_20 -mergechilds -childsareperiods  -RL_listPCMEDC -totalLog $1
# ### LHC17 Data
# bash DownScript.sh 1182 -Name_LHC17Data_v6 GA_pp_AOD ?_GammaConvCalo_20 -mergechilds -childsareperiods  -RL_listPCMEDC -totalLog $1
# ### LHC18 Data
# bash DownScript.sh 1183 -Name_LHC18Data_v6 GA_pp_AOD ?_GammaConvCalo_20 -mergechilds -childsareperiods  -RL_listDPGIncTPC -totalLog $1
# ##################   MC   #############################
# ### LHC16 MC
# bash DownScript.sh 2139 2144 2145 -Name_LHC16MC_v6 GA_pp_MC_AOD ?_GammaConvCalo_20 -mergechilds -mergetrains -childsareperiods  -RL_listPCMEDC -totalLog $1
# ### LHC17 MC
# bash DownScript.sh 2146 2147 -Name_LHC17MC_v6 GA_pp_MC_AOD ?_GammaConvCalo_20 -mergechilds -mergetrains -childsareperiods  -RL_listPCMEDC -totalLog $1
# ### LHC18 MC
# bash DownScript.sh 2148 2149 -Name_LHC18MC_v6 GA_pp_MC_AOD ?_GammaConvCalo_20 -mergechilds -mergetrains -childsareperiods  -RL_listDPCIncAccTPC -totalLog $1
# # ##################   merging   #############################
# # ### Data 16 + 17 + 18
# bash DownScript.sh 1181 1182 1183 -Name_Data_pp13TeV_16_17_18_v6 GA_pp_AOD ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods  -RL_listPCMEDC -noDown -totalLog $1
# # ### MC 16 + 17 + 18
# bash DownScript.sh 2139 2144 2145 2146 2147 2148 2149 -Name_MC_pp13TeV_16_17_18_v6 GA_pp_MC_AOD ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods -RL_listPCMEDC -noDown -totalLog $1
# ################    QA   #############################
# ### LHC16 Data
# bash DownScript.sh 1181 -Name_LHC16Data_v6 GA_pp_AOD ?_GammaConvCalo_2002 -onlyrunwise -mergechilds -childsareperiods  -RL_listPCMEDC -totalLog $1
# ### LHC17 Data
# bash DownScript.sh 1182 -Name_LHC17Data_v6 GA_pp_AOD ?_GammaConvCalo_2002 -onlyrunwise -mergechilds -childsareperiods  -RL_listPCMEDC -totalLog $1
# ### LHC18 Datas
# bash DownScript.sh 1183 -Name_LHC18Data_v6 GA_pp_AOD ?_GammaConvCalo_2002 -onlyrunwise -mergechilds -childsareperiods  -RL_listDPGIncTPC -totalLog $1
# # ### LHC16 MC
# bash DownScript.sh 2139 2144 2145 -Name_LHC16MC_v6 GA_pp_MC_AOD ?_GammaConvCalo_2002 -onlyrunwise -mergeruns -mergechilds -mergetrains -childsareperiods  -RL_listPCMEDC -totalLog $1
# ### LHC17 MC
# bash DownScript.sh 2146 2147 -Name_LHC17MC_v6 GA_pp_MC_AOD ?_GammaConvCalo_2002 -onlyrunwise -mergeruns -mergechilds -mergetrains -childsareperiods  -RL_listPCMEDC -totalLog $1
# ### LHC18 MC
# bash DownScript.sh 2148 2149 -Name_LHC18MC_v6 GA_pp_MC_AOD ?_GammaConvCalo_2002 -onlyrunwise -mergeruns -mergechilds -mergetrains -childsareperiods  -RL_listDPCIncAccTPC -totalLog $1
# ##################    Photon QA   #############################
# ## LHC16 Data 938
# bash DownScript.sh 1181 -Name_LHC16Data_PhotonQA3 GA_pp_AOD ?_AnalysisResults -onlyrunwise -childsareperiods  -RL_listDPGIncAcc -totalLog -runwise $1
# ## LHC17 Data 939
# bash DownScript.sh 1182 -Name_LHC17Data_PhotonQA3 GA_pp_AOD ?_AnalysisResults -onlyrunwise -childsareperiods  -RL_listDPGIncTPC -totalLog -runwise $1
# ## LHC18 Data 940
# bash DownScript.sh 1183 -Name_LHC18Data_PhotonQA3 GA_pp_AOD ?_AnalysisResults -onlyrunwise -childsareperiods  -RL_listDPGIncTPC -totalLog -runwise $1
# ### LHC16 MC
# bash DownScript.sh 2139 2144 2145 -Name_LHC16MC_PhotonQA3 GA_pp_MC_AOD ?_AnalysisResults -onlyrunwise -mergeruns -childsareperiods  -RL_listDPGIncAccTPC -totalLog -runwise $1
# ### LHC17 MC
# bash DownScript.sh 2146 2147 -Name_LHC17MC_PhotonQA3 GA_pp_MC_AOD ?_AnalysisResults -onlyrunwise -mergeruns -childsareperiods  -RL_listDPGIncTPC -totalLog -runwise $1
# ### LHC18 MC
# bash DownScript.sh 2148 2149 -Name_LHC18MC_PhotonQA3 GA_pp_MC_AOD ?_AnalysisResults -onlyrunwise -mergeruns -childsareperiods  -RL_listDPCIncAccTPC -totalLog -runwise $1

###############   MC   #############################
# ### LHC16 MC
# bash DownScript.sh 2187 -Name_JJLHC16MC_v6 GA_pp_MC_AOD ?_GammaConvCalo_2003 -mergechilds -childsareperiods -IsJJ -runwise -totalLog $1
# bash DownScript.sh 2187 -Name_JJLHC16MC_v6 GA_pp_MC_AOD ?_GammaConvCalo_20 -mergechilds -childsareperiods -IsJJ -totalLog $1
# ### LHC17 MC
# bash DownScript.sh 2188 -Name_JJLHC17MC_v6 GA_pp_MC_AOD ?_GammaConvCalo_2003 -mergechilds -childsareperiods -IsJJ -runwise -totalLog $1
# bash DownScript.sh 2188 -Name_JJLHC17MC_v6 GA_pp_MC_AOD ?_GammaConvCalo_20 -mergechilds -childsareperiods -IsJJ -totalLog $1
# # LHC18 MC
# bash DownScript.sh 2189 -Name_JJLHC18MC_v6 GA_pp_MC_AOD ?_GammaConvCalo_2003 -mergechilds -childsareperiods -IsJJ -runwise -totalLog $1
# bash DownScript.sh 2189 -Name_JJLHC18MC_v6 GA_pp_MC_AOD ?_GammaConvCalo_20 -mergechilds -childsareperiods -IsJJ -totalLog $1
# #################   Data   #############################
# # ### LHC16 Data
# # bash DownScript.sh 1193 -Name_SkimLHC16Data_v6 GA_pp_AOD ?_GammaConvCalo_2003 -mergechilds -childsareperiods  -RL_listDPGEDCtrigger -totalLog $1
# bash DownScript.sh 1193 -Name_SkimLHC16Data_v6 GA_pp_AOD ?_GammaConvCalo_20 -mergechilds -childsareperiods  -RL_listPCMEDC -totalLog $1
# # ### LHC17 Data
# # bash DownScript.sh 1194 -Name_SkimLHC17Data_v6 GA_pp_AOD ?_GammaConvCalo_2003 -mergechilds -childsareperiods  -RL_listDPGEDCtrigger -totalLog $1
# bash DownScript.sh 1194 -Name_SkimLHC17Data_v6 GA_pp_AOD ?_GammaConvCalo_20 -mergechilds -childsareperiods  -RL_listPCMEDC -totalLog $1
# # ### LHC18 Data
# # bash DownScript.sh 1195 -Name_SkimLHC18Data_v6 GA_pp_AOD ?_GammaConvCalo_2003 -mergechilds -childsareperiods  -RL_listEDC_trigger -totalLog $1
# bash DownScript.sh 1195 -Name_SkimLHC18Data_v6 GA_pp_AOD ?_GammaConvCalo_20 -mergechilds -childsareperiods  -RL_listDPGIncTPC -totalLog $1
##################   merging   #############################
### Data 16 + 17 + 18s
# bash DownScript.sh 1193 1194 1195 -Name_SkimData_pp13TeV_16_17_18_v6 GA_pp_AOD ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods  -RL_listDPGEDCtrigger -noDown -totalLog $1
### MC 16 + 17 + 18
# bash DownScript.sh 2187 2188 2189 -Name_JJMC_pp13TeV_16_17_18_v6 GA_pp_MC_AOD ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods  -RL_default -noDown -totalLog $1


    #######################################################
    ######################   Daten   ######################
    #######################################################

    # ### LHC16Data="679"; #pass 1 SECOND TRAIN RUN
    # bash DownScript.sh 679 -Name_LHC16Data GA_pp_AOD ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods  -RL_listDPGEDC

    # ### LHC17Data="635"; #pass 1 SECOND TRAIN RUN
    # bash DownScript.sh 635 -Name_LHC17Data GA_pp_AOD ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods  -RL_listDPGEDC

    # ### LHC18Data="705"; #pass 1 SECOND TRAIN RUN
    # bash DownScript.sh 705 -Name_LHC18Data_old GA_pp_AOD ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods  -RL_listDPGEDC

    # ### LHC18Data="753"; #pass 1 SECOND TRAIN RUN
    # bash DownScript.sh 753 -Name_LHC18Data GA_pp_AOD ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods  -RL_listDPGEDC

    # ### Daten LHC16Data="773"
    # bash DownScript.sh 773 -Name_Data_pp13TeV_16 GA_pp_AOD ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods  -RL_listDPGEDC -RL_listDPGCalo -RL_listDPGalo -RL_listDPCIncAccTPC -RL_listDPGIncAccTPC -RL_listDPGIncTPC -RL_listDPGEMC

    # ### Daten LHC17Data="774"
    # bash DownScript.sh 774 -Name_Data_pp13TeV_17 GA_pp_AOD ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods  -RL_listDPGEDC -RL_listDPGCalo -RL_listDPGalo -RL_listDPCIncAccTPC -RL_listDPGIncAccTPC -RL_listDPGIncTPC -RL_listDPGEMC

    # ### Daten LHC18Data="753"
    # bash DownScript.sh 753 -Name_Data_pp13TeV_18 GA_pp_AOD ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods  -RL_listDPGCalo -RL_listDPGalo -RL_listDPCIncAccTPC -RL_listDPGIncAccTPC -RL_listDPGIncTPC -RL_listDPGEMC -RL_listDPGEDC -runwise

    ### merging 16 + 17
    # bash DownScript.sh 773 774 -Name_Data_pp13TeV_16_17_v0 GA_pp_AOD ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods  -RL_listDPGEDC -RL_listDPGCalo -RL_listDPGalo -RL_listDPCIncAccTPC -RL_listDPGIncAccTPC -RL_listDPGIncTPC -RL_listDPGEMC -noDown

    # ### merging 16 + 17 + 18
    # bash DownScript.sh 773 774 753 -Name_Data_pp13TeV_16_17_18 GA_pp_AOD ?_GammaConvCalo_ -mergechilds -childsareperiods  -RL_listDPGCalo -RL_listDPGalo -RL_listDPCIncAccTPC -RL_listDPGIncAccTPC -RL_listDPGIncTPC -RL_listDPGEMC -RL_listDPGEDC -noDown

    # ### Daten LHC16Data_triggered="775"
    # bash DownScript.sh 775 -Name_Skim_Data_pp13TeV_16 GA_pp_AOD ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods  -RL_listDPGEDCtrigger -RL_listDPGCalo -RL_listDPGalo -RL_listDPCIncAccTPC -RL_listDPGIncAccTPC -RL_listDPGIncTPC -RL_listDPGEMC

    # ### Daten LHC17Data_triggered="776"
    # bash DownScript.sh 776 -Name_Skim_Data_pp13TeV_17 GA_pp_AOD ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods  -RL_listDPGEDCtrigger -RL_listDPGCalo -RL_listDPGalo -RL_listDPCIncAccTPC -RL_listDPGIncAccTPC -RL_listDPGIncTPC -RL_listDPGEMC

    # ### merging 16 + 17 triggered
    # bash DownScript.sh 775 776 -Name_Skim_Data_pp13TeV_16_17 GA_pp_AOD ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods  -RL_listDPGEDCtrigger -RL_listDPGCalo -RL_listDPGalo -RL_listDPCIncAccTPC -RL_listDPGIncAccTPC -RL_listDPGIncTPC -RL_listDPGEMC -noDown

    ### LHC16 Data 829
    # bash DownScript.sh 829 -Name_LHC16Data_v1 GA_pp_AOD ?_GammaConvCalo -mergechilds -childsareperiods  -uSR_PCMEDC -runwise -totalLog $1

    ### LHC17 Data 830
    # bash DownScript.sh 830 -Name_LHC17Data_v1 GA_pp_AOD ?_GammaConvCalo -mergechilds -childsareperiods  -uSR_PCMEDC -runwise -totalLog $1

    ### LHC18 Data 734
    # bash DownScript.sh 834 -Name_LHC18Data_v1 GA_pp_AOD ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods  -uSR_PCMEDC -runwise -totalLog $1
    # bash DownScript.sh 834 -Name_LHC18Data_v1 GA_pp_AOD ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods  -RL_listDPGIncAccTPC -RL_listDPGIncTPC -runwise -totalLog $1

    ### merging 16 + 17
    # bash DownScript.sh 829 830 -Name_Data_pp13TeV_16_17_v1 GA_pp_AOD ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods  -uSR_PCMEDC -noDown -totalLog $1
    ### merging 16 + 17 + 18
    # bash DownScript.sh 829 830 834 -Name_Data_pp13TeV_16_17_18_v1 GA_pp_AOD ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods  -uSR_PCMEDC -noDown -runwise -totalLog $1

    # ### LHC16 Data 900
    # bash DownScript.sh 900 -Name_LHC16Data_v2 GA_pp_AOD ?_GammaConvCalo_20 -mergechilds -childsareperiods  -RL_listPCMEDC -totalLog $1
    #
    # ### LHC17 Data 901
    # bash DownScript.sh 901 -Name_LHC17Data_v2 GA_pp_AOD ?_GammaConvCalo_20 -mergechilds -childsareperiods  -RL_listPCMEDC -totalLog $1
    #
    # ### merging 16 + 17
    # bash DownScript.sh 900 901 -Name_Data_pp13TeV_16_17_v2 GA_pp_AOD ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods  -RL_listPCMEDC -noDown -totalLog $1
    #
    # ##### Photon QA
    # bash DownScript.sh 885 -Name_LHC18Data_PhotonQA2 GA_pp_AOD ?_AnalysisResults -mergeruns -childsareperiods -RL_listDPCIncAccTPC -RL_listDPGIncAccTPC -RL_listDPGIncTPC -totalLog $1 -onlyrunwise
    # bash DownScript.sh 903 -Name_LHC18Data_PhotonQA GA_pp_AOD ?_AnalysisResults -childsareperiods -RL_listDPCIncAccTPC -RL_listDPGIncAccTPC -RL_listDPGIncTPC -totalLog $1 -onlyrunwise

    # ### LHC16 Data 938
    # bash DownScript.sh 938 -Name_LHC16Data_v3 GA_pp_AOD ?_GammaConvCalo_20 -mergechilds -childsareperiods  -RL_listPCMEDC -totalLog $1
    #
    # ### LHC17 Data 939
    # bash DownScript.sh 939 -Name_LHC17Data_v3 GA_pp_AOD ?_GammaConvCalo_20 -mergechilds -childsareperiods  -RL_listPCMEDC -totalLog $1
    #
    # ### LHC18 Data 940
    # bash DownScript.sh 940 -Name_LHC18Data_v3 GA_pp_AOD ?_GammaConvCalo_20 -mergechilds -childsareperiods  -RL_listPCMEDC -RL_listDPCIncAccTPC -RL_listDPGIncAccTPC -RL_listDPGIncTPC -totalLog $1
    # # bash DownScript.sh 940 -Name_LHC18Data_v3 GA_pp_AOD ?_GammaConvCalo_20 -mergechilds -childsareperiods  -RL_listDPG -totalLog $1

    # ### merging 16 + 17
    # bash DownScript.sh 938 939 -Name_Data_pp13TeV_16_17_v3 GA_pp_AOD ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods  -RL_listPCMEDC -noDown -totalLog $1
    #
    # ### merging 16 + 17 + 18
    # bash DownScript.sh 938 939 940 -Name_Data_pp13TeV_16_17_18_v3 GA_pp_AOD ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods  -RL_listPCMEDC -noDown -totalLog $1
    # bash DownScript.sh 938 939 940 -Name_Data_pp13TeV_16_17_18_v3 GA_pp_AOD ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods -noDown -totalLog $1

    # ### LHC16 Data 938
    # bash DownScript.sh 1001 -Name_LHC16Data_v4 GA_pp_AOD ?_GammaConvCalo_20 -mergechilds -childsareperiods  -RL_listPCMEDC -totalLog $1
    #
    # ### LHC17 Data 939
    # bash DownScript.sh 1010 -Name_LHC17Data_v4 GA_pp_AOD ?_GammaConvCalo_20 -mergechilds -childsareperiods  -RL_listPCMEDC -totalLog $1
    #
    # ### LHC18 Data 940
    # bash DownScript.sh 1011 -Name_LHC18Data_v4 GA_pp_AOD ?_GammaConvCalo_20 -mergechilds -childsareperiods  -RL_listPCMEDC -RL_listDPCIncAccTPC -RL_listDPGIncAccTPC -RL_listDPGIncTPC -totalLog $1
    #
    # ### merging 16 + 17
    # bash DownScript.sh 1001 1010 -Name_Data_pp13TeV_16_17_v4 GA_pp_AOD ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods  -RL_listPCMEDC -noDown -totalLog $1
    #
    # ### merging 16 + 17 + 18
    # bash DownScript.sh 1001 1010 1011 -Name_Data_pp13TeV_16_17_18_v4 GA_pp_AOD ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods  -RL_listPCMEDC -noDown -totalLog $1
    #
    # ### LHC16 Data 938
    # bash DownScript.sh 1083 -Name_LHC16Data_v5 GA_pp_AOD ?_GammaConvCalo_20 -mergechilds -childsareperiods  -RL_listPCMEDC -totalLog $1
    #
    # ### LHC17 Data 939
    # bash DownScript.sh 1084 -Name_LHC17Data_v5 GA_pp_AOD ?_GammaConvCalo_20 -mergechilds -childsareperiods  -RL_listPCMEDC -totalLog $1
    #
    # ### LHC18 Data 940
    # bash DownScript.sh 1085 -Name_LHC18Data_v5 GA_pp_AOD ?_GammaConvCalo_20 -mergechilds -childsareperiods  -RL_listPCMEDC -RL_listDPCIncAccTPC -RL_listDPGIncAccTPC -RL_listDPGIncTPC -totalLog $1
    #
    # ### merging 16 + 17
    # bash DownScript.sh 1083 1084 -Name_Data_pp13TeV_16_17_v5 GA_pp_AOD ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods  -RL_listPCMEDC -noDown -totalLog $1
    #
    # ### merging 16 + 17 + 18
    # bash DownScript.sh 1083 1084 1085 -Name_Data_pp13TeV_16_17_18_v5 GA_pp_AOD ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods  -RL_listPCMEDC -noDown -totalLog $1

    #######################################################
    ######################   MC   ######################
    #######################################################


    # ### PYT8_13TeV_anchLHC16_AOD209   LHC16xMCPHY="1276"; #pass 1
    # bash DownScript.sh 1276 -Name_LHC16xMCPHY GA_pp_MC_AOD ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods  -RL_listDPGEDC

    # ### PYT8_13TeV_anchLHC16_AOD209_extra   LHC16xMCPHY_extra="1273"; #pass 1
    # bash DownScript.sh 1273 -Name_LHC16xMCPHY_extra GA_pp_MC_AOD ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods  -RL_listDPGEDC

    # ### PYT8_13TeV_anchLHC17_AOD209_extra   LHC17xMCPHY_extra="1408";
    # bash DownScript.sh 1408 -Name_LHC17xMCPHY_extra GA_pp_MC_AOD ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods  -RL_listDPGEDC

    # ### PYT8_13TeV_anchLHC17_AOD209  LHC17xJJMC="1393";
    # bash DownScript.sh 1393 -Name_LHC17xJJMC GA_pp_MC_AOD ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods  -RL_listDPGEDC

    # ### PYT8_13TeV_anchLHC17_AOD209  LHC18xMCPHY="1325"; #pass 1 FIRST TRAIN RUN
    # bash DownScript.sh 1325 -Name_LHC18xMCPHY GA_pp_MC_AOD ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods  -RL_listDPGEDC

    # ### PYT8_13TeV_anchLHC17_AOD209  LHC18xMCPHY="1415"; #pass 1 FIRST TRAIN RUN
    # bash DownScript.sh 1415 -Name_LHC18xMCPHY GA_pp_MC_AOD ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods  -RL_listDPGEDC

    # ### PYT8_13TeV_anchLHC16_AOD209
    # bash DownScript.sh 1470 -Name_MC_pp13TeV_16_17 GA_pp_MC_AOD ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods  -RL_listDPGEDC -RL_listDPGCalo -RL_listDPGalo -RL_listDPCIncAccTPC -RL_listDPGIncAccTPC -RL_listDPGIncTPC -RL_listDPGEMC

    # ### PYT8_13TeV_anchLHC16_AOD209_extra
    # bash DownScript.sh 1471 -Name_MC_pp13TeV_16_17 GA_pp_MC_AOD ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods  -RL_listDPGEDC -RL_listDPGCalo -RL_listDPGalo -RL_listDPCIncAccTPC -RL_listDPGIncAccTPC -RL_listDPGIncTPC -RL_listDPGEMC

    # ### PYT8_13TeV_anchLHC16_AOD209_extra2
    # bash DownScript.sh 1472 -Name_MC_pp13TeV_16_17 GA_pp_MC_AOD ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods  -RL_listDPGEDC -RL_listDPGCalo -RL_listDPGalo -RL_listDPCIncAccTPC -RL_listDPGIncAccTPC -RL_listDPGIncTPC -RL_listDPGEMC

    # ### PYT8_13TeV_anchLHC17_AOD209_extra
    # bash DownScript.sh 1473 -Name_MC_pp13TeV_16_17 GA_pp_MC_AOD ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods  -RL_listDPGEDC -RL_listDPGCalo -RL_listDPGalo -RL_listDPCIncAccTPC -RL_listDPGIncAccTPC -RL_listDPGIncTPC -RL_listDPGEMC

    # ### PYT8_13TeV_anchLHC17_AOD209
    # bash DownScript.sh 1474 -Name_MC_pp13TeV_16_17 GA_pp_MC_AOD ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods  -RL_listDPGEDC -RL_listDPGCalo -RL_listDPGalo -RL_listDPCIncAccTPC -RL_listDPGIncAccTPC -RL_listDPGIncTPC -RL_listDPGEMC

    # ### merging 16 MCs
    # bash DownScript.sh 1470 1471 1472 -Name_MC_pp13TeV_16_v0 GA_pp_MC_AOD ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods  -RL_listDPGEDC -RL_listDPGCalo -RL_listDPGalo -RL_listDPCIncAccTPC -RL_listDPGIncAccTPC -RL_listDPGIncTPC -RL_listDPGEMC
    #
    # ### merging 17 MC
    # bash DownScript.sh 1473 1474 -Name_MC_pp13TeV_17_v0 GA_pp_MC_AOD ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods  -RL_listDPGEDC -RL_listDPGCalo -RL_listDPGalo -RL_listDPCIncAccTPC -RL_listDPGIncAccTPC -RL_listDPGIncTPC -RL_listDPGEMC
    #
    # ### merging 16 + 17 MCs
    # bash DownScript.sh 1470 1471 1472 1473 1474 -Name_MC_pp13TeV_16_17_v0 GA_pp_MC_AOD ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods  -RL_listDPGEDC -RL_listDPGCalo -RL_listDPGalo -RL_listDPCIncAccTPC -RL_listDPGIncAccTPC -RL_listDPGIncTPC -RL_listDPGEMC

    ### LHC16 MC 1546 + 1547 + 1548
    # bash DownScript.sh 1546 1547 1548 -Name_LHC16MC_v1 GA_pp_MC_AOD ?_GammaConvCalo -mergechilds -mergeall -childsareperiods  -uSR_PCMEDC -runwise -totalLog $1

    ### LHC17 MC 1549 + 1550
    # bash DownScript.sh 1549 1550 -Name_LHC17MC_v1 GA_pp_MC_AOD ?_GammaConvCalo -mergechilds -mergeall -childsareperiods  -uSR_PCMEDC -runwise -totalLog $1

    ### LHC18 MC 1556 1557
    # bash DownScript.sh 1556 1557 -Name_LHC18MC_v1 GA_pp_MC_AOD ?_GammaConvCalo -mergechilds -mergeall -childsareperiods -uSR_PCMEDC -runwise -totalLog $1
    # bash DownScript.sh 1556 1557 -Name_LHC18MC_v1 GA_pp_MC_AOD ?_GammaConvCalo -mergechilds -mergeall -childsareperiods -RL_listDPGIncAccTPC -RL_listDPGIncTPC  -runwise -totalLog $1

    ### merging 16 + 17
    # bash DownScript.sh 1546 1547 1548 1549 1550 -Name_MC_pp13TeV_16_17_v1 GA_pp_MC_AOD ?_GammaConvCalo -mergechilds -mergeall -childsareperiods -uSR_PCMEDC -noDown -totalLog $1 -runwise
    ### merging 16 + 17 + 18
    # bash DownScript.sh 1546 1547 1548 1549 1550 1556 1557 -Name_MC_pp13TeV_16_17_18_v1 GA_pp_MC_AOD ?_GammaConvCalo -mergechilds -mergeall -childsareperiods  -uSR_PCMEDC -noDown -totalLog $1 -runwise

    # ### LHC16 MC 1651 + 1705 + 1706
    # # bash DownScript.sh 1651 -Name_LHC16MC_v2 GA_pp_MC_AOD ?_GammaConvCalo_20 -mergechilds -mergetrains -childsareperiods  -RL_listPCMEDC -totalLog $1
    # bash DownScript.sh 1651 1705 1706 -Name_LHC16MC_v2 GA_pp_MC_AOD ?_GammaConvCalo_20 -mergechilds -mergetrains -childsareperiods  -RL_listPCMEDC -totalLog $1
    #
    # # ### LHC17 MC 1707 + 1708
    # bash DownScript.sh 1707 1708 -Name_LHC17MC_v2 GA_pp_MC_AOD ?_GammaConvCalo_20 -mergechilds -mergetrains -childsareperiods  -RL_listPCMEDC -totalLog $1
    #
    # ### LHC18 MC 1709 1710
    # bash DownScript.sh 1709 1710 -Name_LHC18MC_v2 GA_pp_MC_AOD ?_GammaConvCalo_20 -mergechilds -mergetrains -childsareperiods  -RL_listDPCIncAccTPC -RL_listDPGIncAccTPC -RL_listDPGIncTPC -RL_listPCMEDC -totalLog $1
    #
    # ### merging 16 + 17
    # bash DownScript.sh 1651 1705 1706 1707 1708 -Name_MC_pp13TeV_16_17_v2 GA_pp_MC_AOD ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods -RL_listPCMEDC -noDown -totalLog $1

    # ### merging 16 + 17 + 18
    # bash DownScript.sh 1651 1705 1706 1707 1708 1709 1710 -Name_MC_pp13TeV_16_17_18_v2 GA_pp_MC_AOD ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods -RL_listPCMEDC -noDown -totalLog $1

    ##### Photon QA
    # bash DownScript.sh 1624 1625 -Name_LHC18MC_PhotonQA GA_pp_MC_AOD ?_AnalysisResults -mergeruns -childsareperiods -RL_listDPCIncAccTPC -RL_listDPGIncAccTPC -RL_listDPGIncTPC -totalLog $1 -onlyrunwise

    # ### LHC16 MC 1756 + 1757 + 1758
    # bash DownScript.sh 1756 1757 1758 -Name_LHC16MC_v3 GA_pp_MC_AOD ?_GammaConvCalo_20 -mergechilds -mergetrains -childsareperiods  -RL_listPCMEDC -totalLog $1
    #
    # ### LHC17 MC 1759 + 1760
    # bash DownScript.sh 1759 1760 -Name_LHC17MC_v3 GA_pp_MC_AOD ?_GammaConvCalo_20 -mergechilds -mergetrains -childsareperiods  -RL_listPCMEDC -totalLog $1
    #
    # ### LHC18 MC 1761 1762
    # bash DownScript.sh 1761 1762 -Name_LHC18MC_v3 GA_pp_MC_AOD ?_GammaConvCalo_20 -mergechilds -mergetrains -childsareperiods  -RL_listPCMEDC -RL_listDPCIncAccTPC -RL_listDPGIncAccTPC -RL_listDPGIncTPC -totalLog $1
    # bash DownScript.sh 1761 1762 -Name_LHC18MC_v3 GA_pp_MC_AOD ?_GammaConvCalo_20 -mergechilds -mergetrains -childsareperiods  -RL_listDPG -totalLog $1
    #
    # ### merging 16 + 17
    # bash DownScript.sh 1756 1757 1758 1759 1760 -Name_MC_pp13TeV_16_17_v3 GA_pp_MC_AOD ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods -RL_listPCMEDC -noDown -totalLog $1
    #
    # ### merging 16 + 17 + 18
    # bash DownScript.sh 1756 1757 1758 1759 1760 1761 1762 -Name_MC_pp13TeV_16_17_18_v3 GA_pp_MC_AOD ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods -RL_listPCMEDC -noDown -totalLog $1

    # ### With weights
    # ### LHC16 MC 1789 + 1790 + 1791
    # bash DownScript.sh 1789 1790 1791 -Name_LHC16MC_v3_ww GA_pp_MC_AOD ?_GammaConvCalo_20 -mergechilds -mergetrains -childsareperiods  -RL_listPCMEDC -totalLog $1
    #
    # ### LHC17 MC 1792 + 1793
    # bash DownScript.sh 1792 1793 -Name_LHC17MC_v3_ww GA_pp_MC_AOD ?_GammaConvCalo_20 -mergechilds -mergetrains -childsareperiods  -RL_listPCMEDC -totalLog $1
    #
    # ### LHC18 MC 1794 1795
    # bash DownScript.sh 1794 1795 -Name_LHC18MC_v3_ww GA_pp_MC_AOD ?_GammaConvCalo_20 -mergechilds -mergetrains -childsareperiods  -RL_listDPG -totalLog $1
    #
    # ### merging 16 + 17
    # bash DownScript.sh 1789 1790 1791 1792 1793 -Name_MC_pp13TeV_16_17_v3_ww GA_pp_MC_AOD ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods -RL_listPCMEDC -noDown -totalLog $1
    #
    # ### merging 16 + 17 + 18
    # bash DownScript.sh 1789 1790 1791 1792 1793 1794 1795 -Name_MC_pp13TeV_16_17_18_v3_ww GA_pp_MC_AOD ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods -RL_listPCMEDC -noDown -totalLog $1
    # bash DownScript.sh 1789 1790 1791 1792 1793 1794 1795 -Name_MC_pp13TeV_16_17_18_v3_ww GA_pp_MC_AOD ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods -noDown -totalLog $1

    # ### LHC16 MC
    # bash DownScript.sh 1860 1861 -Name_LHC16MC_v4 GA_pp_MC_AOD ?_GammaConvCalo_20 -mergechilds -mergetrains -childsareperiods  -RL_listPCMEDC -totalLog $1
    #
    # ### LHC17 MC
    # bash DownScript.sh 1862 1863 -Name_LHC17MC_v4 GA_pp_MC_AOD ?_GammaConvCalo_20 -mergechilds -mergetrains -childsareperiods  -RL_listPCMEDC -totalLog $1
    #
    # ### LHC18 MC
    # bash DownScript.sh 1864 1865 -Name_LHC18MC_v4 GA_pp_MC_AOD ?_GammaConvCalo_20 -mergechilds -mergetrains -childsareperiods  -RL_listPCMEDC -RL_listDPCIncAccTPC -RL_listDPGIncAccTPC -RL_listDPGIncTPC -totalLog $1
    #
    # ### merging 16 + 17
    # bash DownScript.sh 1860 1861 1862 1863 -Name_MC_pp13TeV_16_17_v4 GA_pp_MC_AOD ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods -RL_listPCMEDC -noDown -totalLog $1
    #
    # ### merging 16 + 17 + 18
    # bash DownScript.sh 1860 1861 1862 1863 1864 1865 -Name_MC_pp13TeV_16_17_18_v4 GA_pp_MC_AOD ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods -RL_listPCMEDC -noDown -totalLog $1
    # bash DownScript.sh 1860 1861 1862 1863 1864 1865 -Name_MC_pp13TeV_16_17_18_v4 GA_pp_MC_AOD ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods -noDown -totalLog $1

    # ### LHC16 MC
    # bash DownScript.sh 1971 1982 1983 -Name_LHC16MC_v5 GA_pp_MC_AOD ?_GammaConvCalo_204 -mergechilds -mergetrains -childsareperiods  -RL_listPCMEDC -totalLog $1
    #
    # ### LHC17 MC
    # bash DownScript.sh 1984 1985 -Name_LHC17MC_v5 GA_pp_MC_AOD ?_GammaConvCalo_20 -mergechilds -mergetrains -childsareperiods  -RL_listPCMEDC -totalLog $1
    #
    # ### LHC18 MC
    # bash DownScript.sh 1986 1987 -Name_LHC18MC_v5 GA_pp_MC_AOD ?_GammaConvCalo_20 -mergechilds -mergetrains -childsareperiods  -RL_listPCMEDC -RL_listDPCIncAccTPC -totalLog $1
    #
    # ### merging 16 + 17
    # bash DownScript.sh 1971 1982 1983 1984 1985 -Name_MC_pp13TeV_16_17_v5 GA_pp_MC_AOD ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods -RL_listPCMEDC -noDown -totalLog $1
    #
    # ### merging 16 + 17 + 18
    # bash DownScript.sh 1971 1982 1983 1984 1985 1986 1987 -Name_MC_pp13TeV_16_17_18_v5 GA_pp_MC_AOD ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods -RL_listPCMEDC -noDown -totalLog $1

    cat Error.log
    exit
fi
