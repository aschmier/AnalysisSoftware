#include <vector>

#ifndef GAMMACONV_ExtractSignalBinningpp13TeV
#define GAMMACONV_ExtractSignalBinningpp13TeV

    #include <iostream>
    #include "ExtractSignalBinningpp13TeV_PCM.h"
    #include "ExtractSignalBinningpp13TeV_EDC.h"
    #include "ExtractSignalBinningpp13TeV_PHOS.h"
    #include "ExtractSignalBinningpp13TeV_PCMEDC.h"
    #include "ExtractSignalBinningpp13TeV_PCMPHOS.h"
    #include "ExtractSignalBinningpp13TeV_Dalitz.h"
    #include "ExtractSignalBinningpp13TeV_EMCMerged.h"
    #include "ExtractSignalBinningpp13TeV_omega.h"
    
    using namespace std;

    //****************************************************************************************************
    //******************** Pt binning for pp, 13TeV *****************************************************
    //****************************************************************************************************



    // ---------------------------------------------------------
    // CombinedBinning- pi0
    // ---------------------------------------------------------


    std::vector<Double_t> fBinsPi013TeV_CombinedBinning {   // size: 109
        0.00,  0.30,  0.40,  0.50,  0.60,        0.70,  0.80,  0.90,  1.00,  1.10,
        1.20,  1.30,  1.40,  1.50,  1.60,        1.70,  1.80,  1.90,  2.00,  2.10,
        2.20,  2.30,  2.40,  2.50,  2.60,        2.70,  2.80,  2.90,  3.00,  3.10,
        3.20,  3.30,  3.40,  3.50,  3.60,        3.70,  3.80,  3.90,  4.00,  4.20,
        4.40,  4.60,  4.80,  5.00,  5.20,        5.40,  5.60,  5.80,  6.00,  6.20,
        6.40,  6.60,  6.80,  7.00,  7.20,        7.40,  7.60,  7.80,  8.00,  8.40,
        8.80,  9.20,  9.60,  10.0,  10.5,        11.0,  11.5,  12.0,  12.5,  13.0,
        13.5,  14.0,  15.0,  16.0,  17.0,        18.0,  19.0,  20.0,  21.0,  22.0,
        23.0,  24.0,  25.0,  26.0,  27.0,        28.0,  29.0,  30.0,  32.0,  34.0,
        36.0,  38.0,  40.0,  45.0,  50.0,        55.0,  60.0,  65.0,  70.0,  80.0,
        90.0,  100.,  110.,  120.,  130.,        140.,  150.,  175.,  200.
        };



    // ---------------------------------------------------------
    // CombinedBinning- eta
    // ---------------------------------------------------------


    // std::vector<Double_t> fBinsEta13TeV_CombinedBinning { // size: 38
    //   0.00, 0.40, 0.60, 0.80, 1.00,  1.20, 1.40, 1.60, 1.80, 2.00,
    //   2.20, 2.40, 2.60, 2.80, 3.00,  3.40, 3.80, 4.20, 4.60, 5.00,
    //   6.00, 7.00, 8.00, 10.0, 12.0,  14.0, 16.0, 18.0, 20.0, 22.0,
    //   24.0, 26.0, 30.0, 35.0, 40.0,  50.0, 60.0, 80.0};

    std::vector<Double_t> fBinsEta13TeV_CombinedBinning { // size: 27
        0.00, 0.40, 0.70, 1.00, 1.30,   1.60, 2.00, 2.50, 3.00, 3.50, 
        4.00, 5.00, 6.00, 8.00, 10.0,   12.0, 14.0, 16.0, 20.0, 25.0, 
        30.0, 35.0, 40.0, 50.0, 60.0,   80.0};




    // CaloMix
        std::vector<Double_t> fBinsPi013TeVCaloMixTrigINT7Pt {      // size: 90
                0.00,  1.00,  1.10,  1.20,  1.30,          1.40,  1.50,  1.60,  1.70,  1.80,
                1.90,  2.00,  2.10,  2.20,  2.30,          2.40,  2.50,  2.60,  2.70,  2.80,
                2.90,  3.00,  3.10,  3.20,  3.30,          3.40,  3.50,  3.60,  3.70,  3.80,
                3.90,  4.00,  4.20,  4.40,  4.60,          4.80,  5.00,  5.20,  5.40,  5.60,
                5.80,  6.00,  6.20,  6.40,  6.60,          6.80,  7.00,  7.20,  7.40,  7.60,
                7.80,  8.00,  8.40,  8.80,  9.20,           9.60,  10.0,  10.5,  11.0,  11.5,
                12.0,  12.5,  13.0,  13.5,  14.0,          15.0,  17.0,  20.0,  21.0,  22.0,
                23.0,  24.0,  25.0,  26.0,  27.0,           28.0,  29.0,  30.0,  32.0,  34.0,
                36.0,  38.0,  40.0,  45.0,  50.0,          55.0,  65.0,  80.0,  90.0, 100.
        };
            std::vector<Int_t>    fBinsPi013TeVCaloMixTrigINT7PtRebin { // size: 91
            4,  4,  4,  4,  4,          4,  4,  4,  4,  4,
            4,  4,  4,  4,  4,          4,  4,  4,  4,  4,
            4,  4,  4,  4,  4,          4,  4,  4,  4,  4,
            4,  4,  4,  4,  4,          4,  4,  4,  4,  4,
            4,  4,  4,  4,  4,          4,  4,  4,  4,  4,
            4,  4,  4,  4,  4,          4,  4,  4,  4,  4,
            4,  4,  4,  4,  4,          4,  4,  4,  4,  4,
            4,  4,  4,  4,  4,          4,  4,  4,  4,  4,
            4,  4,  4,  4,  4,          4,  4,  4,  4,  4,
            4};




    // ---------------------------------------------------------
    // minimum bias trigger (INT7) - eta
    // ---------------------------------------------------------

    
    // EDC-PHOS (mode 14)
    std::vector<Double_t> fBinsEta13TeVCaloMixTrigINT7Pt {      // size: 33
        0.00, 1.4, 1.60,         1.80, 2.00, 2.20, 2.40, 2.60,
        2.80, 3.00, 3.40, 3.80, 4.20,       4.60, 5.00, 6.00, 8.00,
        10.0, 14.0, 20.0, 22.0, 24.0,        26.0, 30.0, 40.0, 50.0, 60.0,
        70.0, 80.0, 100.0
    };
    std::vector<Int_t>    fBinsEta13TeVCaloMixTrigINT7PtRebin { // size: 47
        16,  16,  16,  16,  16,          16,  16,  16,  16,  16,
        16,  16,  16,  16,  16,          16,  16,  16,  16,  16,
        16,  16,  16,  16,  16,          16,  16,  16,  16,  16,
        16,  16,  16,  16,  16,          16,  16,  16,  16,  16,
        16,  16,  16,  16,  16,          16, 16};


    // ---------------------------------------------------------
    // minimum bias trigger (INT7) - eta
    // ---------------------------------------------------------

    // Eta->pi+pi-pi0 PCM
    std::vector<Double_t> fBinsEtaPiPlPiMiPiZero13TevPtPCM {      // size: 20
        0, 1, 1.4, 1.6, 1.8, 2, 2.2, 2.4, 2.6, 3,
        3.5, 4, 4.5, 5, 5.5, 6., 7, 8., 10., 15. };
    std::vector<Int_t>    fBinsEtaPiPlPiMiPiZero13TevPtRebinPCM { // size: 19
        8, 8, 6, 6, 6, 6, 4, 4, 4, 4,
        4, 4, 4, 4, 4, 4, 6, 8, 8 };



    // ---------------------------------------------------------
    // Direct photon binnings
    // ---------------------------------------------------------
    Double_t fBinsDirGamma13TeVPt[25]               = { 0.0, 0.1, 0.4, 0.6, 0.8, 1.0, 1.2, 1.4, 1.6, 1.8, 2.0,               2.2, 2.4, 2.6, 2.8, 3.0, 3.4, 3.8, 4.2, 4.8, 5.8,
                                                        7.0, 8.5, 10, 12.0};
    Int_t fBinsDirGamma13TeVPtRebin[29]             = { 4, 4, 4, 2, 2, 2, 2, 2, 2, 2,           2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                                                        2, 4, 4, 4, 8, 8, 8, 8, 8};
    Double_t fBinsDirGamma13TeVPtDCAzDist[16]       = { 0.0, 0.1, 0.4, 0.6, 0.8, 1.0, 1.2, 1.4, 1.6, 1.8, 2.0,               2.5, 4.0, 6.0, 9.0, 12.0};


    // ---------------------------------------------------------
    // Cluster binning
    // ---------------------------------------------------------
    Int_t fNBinsClusterOmega13TeVPt                 =  301;
    Int_t fNBinsCluster13TeVPt                      =  307;
    Double_t fBinsCluster13TeVPt[1000];
    Int_t fNBinsCluster13TeVPCMPt                   =  301;
    Double_t fBinsCluster13TeVPCMPt[302];


    Int_t fNBinsCluster13TeVmEMCPt                   =  93;
    Double_t fBinsCluster13TeVmEMCPt_from7TeV[94]    = { 0.0,  0.1,  0.2,  0.3,  0.4,  0.5,  0.6,  0.7,  0.8,  0.9,     //10
                                                         1.0,  1.1,  1.2,  1.3,  1.4,  1.5,  1.6,  1.7,  1.8,  1.9,     //20
                                                         2.0,  2.2,  2.4,  2.6,  2.8,  3.0,  3.2,  3.4,  3.6,  3.8,     //30
                                                         4.0,  4.2,  4.4,  4.6,  4.8,  5.0,  5.2,  5.4,  5.6,  5.8,     //40
                                                         6.0,  6.2,  6.4,  6.6,  6.8,  7.0,  7.4,  7.8,  8.2,  8.6,     //50
                                                         9.0,  9.5,  10,   10.5, 11,   11.5, 12.,  13.,  14.,  15.,     //60
                                                         16.,  17.,  18.,  19.,  20.,  22.,  24.,  26.,  28.,  30.,     //70
                                                         35,   40,   45,   50,   55,   60,   65,   70,   75,   80,      //80
                                                         85,   90,   95,   100,  110,  120,  130,  140,  150,  160,     //90
                                                         170,  180,  190,  200};                                        //100 -> 94

    Double_t fBinsCluster13TeVmEMCPt[94]             = { 0.0,  0.1,  0.2,  0.3,  0.4,  0.5,  0.6,  0.7,  0.8,  0.9,     //10
                                                         1.0,  1.1,  1.2,  1.3,  1.4,  1.5,  1.6,  1.7,  1.8,  1.9,     //20
                                                         2.0,  2.2,  2.4,  2.6,  2.8,  3.0,  3.2,  3.4,  3.6,  3.8,     //30
                                                         4.0,  4.2,  4.4,  4.6,  4.8,  5.0,  5.2,  5.4,  5.6,  5.8,     //40
                                                         6.0,  6.2,  6.4,  6.6,  6.8,  7.0,  7.4,  7.8,  8.2,  8.6,     //50
                                                         9.0,  9.5,  10,   10.5, 11,   11.5, 12.,  13.,  14.,  15.,     //60
                                                         16.,  17.,  18.,  19.,  20.,  22.,  24.,  26.,  28.,  30.,     //70
                                                         35,   40,   45,   50,   55,   60,   65,   70,   75,   80,      //80
                                                         85,   90,   95,   100,  110,  120,  130,  140,  150,  160,     //90
                                                         170,  180,  190,  200};                     //60 -> 68

    //****************************************************************************************************
    //******************** Pt binning for pp, 13TeV low B ( 0.2T ) **************************************
    //****************************************************************************************************
    std::vector<Double_t> fBinsPi013TeVLowBPt { // 47
        0.00, 0.20, 0.30, 0.40, 0.50, 0.60, 0.70, 0.80, 0.90, 1.00,
        1.10, 1.20, 1.30, 1.40, 1.50, 1.60, 1.70, 1.80, 1.90, 2.00,
        2.10, 2.20, 2.30, 2.40, 2.50, 2.60, 2.70, 2.80, 3.00, 3.20,
        3.40, 3.60, 3.80, 4.00, 4.20, 4.40, 4.60, 4.80, 5.00, 5.50,
        6.00, 6.50, 7.00, 8.00, 10.0, 12.5, 15.0};
    std::vector<Int_t> fBinsPi013TeVLowBPtRebin { // 46
        4, 8, 4, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        2, 4, 4, 4, 4, 8};
    //****************************************************************************************************
    //******************** Pt binning for pp, 13TeV low B ( 0.2T ) **************************************
    //****************************************************************************************************

    std::vector<Int_t> fBinsPi0Eta13TeVLowBPtRebin{ // size 29
        4, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        2, 2, 2, 2, 4, 4, 4, 4};

    std::vector<Double_t> fBinsDirGamma13TeVLowBPt { // size: 25
        0.0, 0.4, 0.6, 0.8, 1.0, 1.2, 1.4, 1.6, 1.8, 2.0,
        2.2, 2.4, 2.6, 2.8, 3.0, 3.4, 3.8, 4.2, 4.8, 5.8,
        7.0, 8.5, 10, 12.0, 16.0};
    std::vector<Int_t> fBinsDirGamma13TeVLowBPtRebin{ //size: 24
        4, 4, 4, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 4, 4, 4,
        8, 8, 8, 8};
    std::vector<Double_t> fBinsDirGamma13TeVLowBPtDCAzDist { // size: 16
        0.0, 0.4, 0.6, 0.8, 1.0, 1.2, 1.4, 1.6, 1.8, 2.0,
        2.5, 4.0, 6.0, 9.0, 12.0, 16.0};

    //****************************************************************************************************
    //******************** EtaPrime Pt binning for pp, 13TeV  ********************************************
    //****************************************************************************************************

    // * PCM-PCM   (mode 100) *
    std::vector<Double_t> fBinsEtaPrime13TeV_PCM_INT7_Pt {      // size: 8
        0.0, 0.4, 0.8, 1.5, 2.5,    5.0, 10.0, 20.0 };
    std::vector<Int_t>    fBinsEtaPrime13TeV_PCM_INT7_PtRebin { // bins: 7
        10, 4, 8, 12, 12,     16, 20 };
    std::vector<Double_t> fBinsEtaPrime13TeV_PCM_L0_Pt {        // size: 6
        0.0, 4.0, 7.0, 10.0, 20.0,     30. };
    std::vector<Int_t>    fBinsEtaPrime13TeV_PCM_L0_PtRebin {   // bins: 5
        10, 12, 12, 16, 16 };
    std::vector<Double_t> fBinsEtaPrime13TeV_PCM_EG1_Pt {       // size: 7
        0.0, 7.0, 8.0, 10.0, 15.0,       20.0, 30. };
    std::vector<Int_t>    fBinsEtaPrime13TeV_PCM_EG1_PtRebin {  // bins: 6
        10, 10, 10, 12, 20,      20 };
    std::vector<Double_t> fBinsEtaPrime13TeV_PCM_EG2_Pt {       // size: 7
        0.0, 4.0, 6.0, 8.0, 10.0,       15.0, 30. };
    std::vector<Int_t>    fBinsEtaPrime13TeV_PCM_EG2_PtRebin {  // bins: 6
        10, 8, 10, 10, 12,      20 };

    // * PCM-EMC   (mode 102) *
    std::vector<Double_t> fBinsEtaPrime13TeV_PCMEMC_INT7_Pt {      // size: 10
        0.0, 0.8, 1.2, 1.6, 2.0,       2.5, 3.0, 5.0, 10.0, 20.0 };
    std::vector<Int_t>    fBinsEtaPrime13TeV_PCMEMC_INT7_PtRebin { // bins: 9
        10, 8, 12, 12, 12,      12, 12, 16, 20 };
    std::vector<Double_t> fBinsEtaPrime13TeV_PCMEMC_L0_Pt {        // size: 10
        0.0, 0.8, 1.2, 1.6, 2.0,        2.5, 3.0, 5.0, 10.0, 20.0 };
    std::vector<Int_t>    fBinsEtaPrime13TeV_PCMEMC_L0_PtRebin {   // bins: 9
        10, 16, 16, 16, 12,       12, 12, 16, 20 };
    std::vector<Double_t> fBinsEtaPrime13TeV_PCMEMC_EG1_Pt {       // size: 9
        0.0, 7.0, 8.0, 9.0, 10.0,       12.0, 15.0, 20.0, 30.0 };
    std::vector<Int_t>    fBinsEtaPrime13TeV_PCMEMC_EG1_PtRebin {  // bins: 8
        10, 20, 16, 12, 16,            20, 25, 25 };
    std::vector<Double_t> fBinsEtaPrime13TeV_PCMEMC_EG2_Pt {       // size: 11
        0.0, 4.0, 5.0, 6.0, 7.0,        8.0, 10.0, 12.0, 15.0, 20.0,
        30.0 };
    std::vector<Int_t>    fBinsEtaPrime13TeV_PCMEMC_EG2_PtRebin {  // bins: 10
        10, 10, 10, 12, 12,            12, 16, 16, 25, 25, 25 };

    // * PCM-PHOS  (mode 103) *
    std::vector<Double_t> fBinsEtaPrime13TeV_PCMPHOS_INT7_Pt {      // size: 7
        0.0, 0.8, 1.5, 2.5, 5.0,        15.0, 30. };
    std::vector<Int_t>    fBinsEtaPrime13TeV_PCMPHOS_INT7_PtRebin { // bins: 6
        10, 20, 20, 16, 20,         25 };
    std::vector<Double_t> fBinsEtaPrime13TeV_PCMPHOS_VZERO_Pt {        // size: 8
        0.0, 5.5, 6.0, 6.5, 7.0,        8.0, 15.0, 30. };
    std::vector<Int_t>    fBinsEtaPrime13TeV_PCMPHOS_VZERO_PtRebin {   // bins: 7
        10, 20, 20, 20, 20,         20, 25 };

    // * EMC-EMC   (mode 104) *
    std::vector<Double_t> fBinsEtaPrime13TeV_EMC_INT7_Pt {      // size: 10
        0.0, 1.4, 2.2, 3.0, 3.5,        4.0, 5.0, 8.0, 15.0, 20.0 };
    std::vector<Int_t>    fBinsEtaPrime13TeV_EMC_INT7_PtRebin { // bins: 9
        10, 12, 10, 8, 8,       8, 12, 16, 25 };
    std::vector<Double_t> fBinsEtaPrime13TeV_EMC_L0_Pt {        // size: 7
        0.0, 3.0, 4.0, 5.0, 8.0,        15.0, 30.0 };
    std::vector<Int_t>    fBinsEtaPrime13TeV_EMC_L0_PtRebin {   // bins: 6
        10, 16, 16, 16, 20,         25 };
    std::vector<Double_t> fBinsEtaPrime13TeV_EMC_EG1_Pt {       // size: 16
        0.0, 7.5, 8.0, 8.5, 9.0,        9.5, 10.0, 10.5, 11.0, 11.5,
        12.0, 13.0, 14.0, 16.0, 20.0,   30.0 };
    std::vector<Int_t>    fBinsEtaPrime13TeV_EMC_EG1_PtRebin {  // bins: 15
        10, 4, 4, 4, 4,         4, 8, 8, 8, 8,
        8, 10, 10, 10, 10 };
    std::vector<Double_t> fBinsEtaPrime13TeV_EMC_EG2_Pt {       // size: 13
        0.0, 4.0, 4.5, 5.0, 5.5,        6.0, 6.5, 7.0, 8.0, 10.0,
        15.0, 20.0, 30. };
    std::vector<Int_t>    fBinsEtaPrime13TeV_EMC_EG2_PtRebin {  // bins: 12
        10, 8, 8, 8, 8,     8, 8, 10, 10, 10,
        12, 16 };

    // * PHOS-PHOS (mode 105) *
    std::vector<Double_t> fBinsEtaPrime13TeV_PHOS_INT7_Pt {      // size: 6
        0.0, 1.4, 4.0, 6.0, 10.0, 20.0 };
    std::vector<Int_t>    fBinsEtaPrime13TeV_PHOS_INT7_PtRebin { // bins: 5
        10, 16, 8, 8, 8 };
    std::vector<Double_t> fBinsEtaPrime13TeV_PHOS_VZERO_Pt {        // size: 9
        0.0, 6.0, 7.0, 8.0, 10.0,       12.5, 15.0, 20.0, 30. };
    std::vector<Int_t>    fBinsEtaPrime13TeV_PHOS_VZERO_PtRebin {   // bins: 8
        10, 4, 4, 8, 8,      8, 8, 12 };


#endif
