#! /bin/bash
echo "Starting GetGammaConvCaloFilesFromGridAndMergeThem_pp_13TeV..."
echo;

thisuser=`echo ${USER}`
d=`date +%Y_%m_%d`
if [[ $thisuser = "adrian" || $thisuser = "amechler" ]]
then

    if [[ -f Error.log ]]; then
        rm Error.log
    fi

    #################   MC   #############################
    ### LHC16 MC
    bash DownScript.sh 2187 -Name_JJLHC16MC_v6 GA_pp_MC_AOD ?_GammaConvCalo_20 -mergechilds -childsareperiods -IsJJ -totalLog $1
    ### LHC17 MC
    bash DownScript.sh 2188 -Name_JJLHC17MC_v6 GA_pp_MC_AOD ?_GammaConvCalo_20 -mergechilds -childsareperiods -IsJJ -totalLog $1
    ### LHC18 MC
    bash DownScript.sh 2189 -Name_JJLHC18MC_v6 GA_pp_MC_AOD ?_GammaConvCalo_20 -mergechilds -childsareperiods -IsJJ -totalLog $1
    #################   Data   #############################
    # ### LHC16 Data
    bash DownScript.sh 1193 -Name_SkimLHC16Data_v6 GA_pp_AOD ?_GammaConvCalo_20 -mergechilds -childsareperiods  -RL_listPCMEDC -totalLog $1
    # ### LHC17 Data
    bash DownScript.sh 1194 -Name_SkimLHC17Data_v6 GA_pp_AOD ?_GammaConvCalo_20 -mergechilds -childsareperiods  -RL_listPCMEDC -totalLog $1
    # ### LHC18 Data
    bash DownScript.sh 1195 -Name_SkimLHC18Data_v6 GA_pp_AOD ?_GammaConvCalo_20 -mergechilds -childsareperiods  -RL_listDPGIncTPC -totalLog $1
    ##################   merging   #############################
    ### Data 16 + 17 + 18s
    bash DownScript.sh 1193 1194 1195 -Name_SkimData_pp13TeV_16_17_18_v6 GA_pp_AOD ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods  -RL_listPCMEDC -noDown -totalLog $1
    ### MC 16 + 17 + 18
    bash DownScript.sh 2187 2188 2189 -Name_JJMC_pp13TeV_16_17_18_v6 GA_pp_MC_AOD ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods -RL_listPCMEDC -noDown -totalLog $1

#####                 new Trains v6
#####                 with recalib and MBW
#####                 including QA
##################   Data   #############################
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
# ##################   merging   #############################
# ### Data 16 + 17 + 18
# bash DownScript.sh 1181 1182 1183 -Name_Data_pp13TeV_16_17_18_v6 GA_pp_AOD ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods  -RL_listPCMEDC -noDown -totalLog $1
# ### MC 16 + 17 + 18
# bash DownScript.sh 2139 2144 2145 2146 2147 2148 2149 -Name_MC_pp13TeV_16_17_18_v6 GA_pp_MC_AOD ?_GammaConvCalo -mergechilds -mergetrains -childsareperiods -RL_listPCMEDC -noDown -totalLog $1
# ################    QA   #############################
# ## LHC16 Data
# bash DownScript.sh 1181 -Name_LHC16Data_v6 GA_pp_AOD ?_GammaConvCalo_2002 -onlyrunwise -mergechilds -childsareperiods  -RL_listPCMEDC -totalLog $1
# ### LHC17 Data
# bash DownScript.sh 1182 -Name_LHC17Data_v6 GA_pp_AOD ?_GammaConvCalo_2002 -onlyrunwise -mergechilds -childsareperiods  -RL_listPCMEDC -totalLog $1
# ### LHC18 Data
# bash DownScript.sh 1183 -Name_LHC18Data_v6 GA_pp_AOD ?_GammaConvCalo_2002 -onlyrunwise -mergechilds -childsareperiods  -RL_listDPGIncTPC -totalLog $1
# ### LHC16 MC
# bash DownScript.sh 2139 2144 2145 -Name_LHC16MC_v6 GA_pp_MC_AOD ?_GammaConvCalo_2002 -onlyrunwise -mergeruns -mergechilds -mergetrains -childsareperiods  -RL_listPCMEDC -totalLog $1
# ### LHC17 MC
# bash DownScript.sh 2146 2147 -Name_LHC17MC_v6 GA_pp_MC_AOD ?_GammaConvCalo_2002 -onlyrunwise -mergeruns -mergechilds -mergetrains -childsareperiods  -RL_listPCMEDC -totalLog $1
# ### LHC18 MC
# bash DownScript.sh 2148 2149 -Name_LHC18MC_v6 GA_pp_MC_AOD ?_GammaConvCalo_2002 -onlyrunwise -mergeruns -mergechilds -mergetrains -childsareperiods  -RL_listDPCIncAccTPC -totalLog $1
##################    Photon QA   #############################
# ### LHC16 Data 938
# bash DownScript.sh 1181 -Name_LHC16Data_PhotonQA3 GA_pp_AOD ?_AnalysisResults -onlyrunwise -childsareperiods  -RL_listDPGIncTPC -RL_listDPGIncAcc -totalLog $1
# ### LHC17 Data 939
# bash DownScript.sh 1182 -Name_LHC17Data_PhotonQA3 GA_pp_AOD ?_AnalysisResults -onlyrunwise -childsareperiods  -RL_listDPGIncTPC -totalLog $1
# ### LHC18 Data 940
# bash DownScript.sh 1183 -Name_LHC18Data_PhotonQA3 GA_pp_AOD ?_AnalysisResults -onlyrunwise -childsareperiods  -RL_listDPGIncTPC -totalLog $1
# ### LHC16 MC
# bash DownScript.sh 2139 2144 2145 -Name_LHC16MC_PhotonQA3 GA_pp_MC_AOD ?_AnalysisResults -onlyrunwise -mergeruns -childsareperiods  -RL_listDPGIncAccTPC -totalLog $1
# ### LHC17 MC
# bash DownScript.sh 2146 2147 -Name_LHC17MC_PhotonQA3 GA_pp_MC_AOD ?_AnalysisResults -onlyrunwise -mergeruns -childsareperiods  -RL_listDPGIncTPC -totalLog $1
# ### LHC18 MC
# bash DownScript.sh 2148 2149 -Name_LHC18MC_PhotonQA3 GA_pp_MC_AOD ?_AnalysisResults -onlyrunwise -mergeruns -childsareperiods  -RL_listDPCIncAccTPC -totalLog $1


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
