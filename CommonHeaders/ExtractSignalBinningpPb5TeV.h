#ifndef GAMMACONV_ExtractSignalBinningpPb5TeV
#define GAMMACONV_ExtractSignalBinningpPb5TeV

    //****************************************************************************************************
    //****************** Pt binning for pPb, 5.023 TeV ***************************************************
    //****************************************************************************************************

    //----------------------------------------------------------------------------------------------------
    // Pi0 
    //----------------------------------------------------------------------------------------------------
    // pt binnings 
    //____________________________________________________________________________________________________
    // combined:
//     std::vector<Double_t> fBinsPi0pPb5TeVCombPt { // size 36
//         0.0, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0,   1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0, 
//         2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 2.9, 3.0,   3.2, 3.4, 3.6, 3.8, 4.0, 4.2, 4.4, 4.6, 4.8, 5.0,
//         5.4, 5.8, 6.2, 6.6, 7.0, 7.5, 8.0, 9.0, 10.0, 11.0,  12.0, 14.0, 16.0, 18.0, 20.0, 25.0, 30.0};
    std::vector<Double_t> fBinsPi0pPb5TeVCombPt { // size 79
        0.0, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0,   1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0, 
        2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 2.9, 3.0,   3.2, 3.4, 3.6, 3.8, 4.0, 4.2, 4.4, 4.6, 4.8, 5.0,
        5.4, 5.8, 6.2, 6.6, 7.0, 7.5, 8.0, 8.5, 9.0, 9.5,   10.0, 10.5, 11.0, 11.5, 12.0, 12.5, 13.0, 13.5, 14.0, 15.0, 
        16.0, 17.0, 18.0, 19.0, 20.0, 22.0, 24.0, 26.0, 28.0, 30.0,     35.0, 40.0, 50.0, 60.0, 80.0, 100., 130.0, 160.0, 200.0
        };
        
//     std::vector<Double_t> fBinsPi0pPb5TeVCombCentPt { // size 28
//         0.0, 0.4, 0.5, 0.6, 0.7, 0.8, 1.0, 1.2, 1.4, 1.6,   1.8, 2.0, 2.2, 2.4, 2.6, 2.8, 3.0, 3.4, 4.0, 5.0,
//         6.0, 8.0, 10.0, 12.0, 16.0, 24.0, 32.0, 40.0};
    std::vector<Double_t> fBinsPi0pPb5TeVCombCentPt { // size 51
        0.0, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2,   1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0, 2.2, 2.4,
        2.6, 2.8, 3.0, 3.2, 3.4, 3.6, 3.8, 4.0, 4.5, 5.0,   5.5, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 
        15.0, 16.0, 17.0, 18.0, 19.0, 20.0, 22.0, 24.0, 26.0, 30.0,     40.0 
    };
        
    // PCM:
    std::vector<Double_t> fBinsPi0pPb5TeVPCMPt { // size 56
        0.0, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0,   1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0, 
        2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 2.9, 3.0,   3.2, 3.4, 3.6, 3.8, 4.0, 4.2, 4.4, 4.6, 4.8, 5.0,
        5.4, 5.8, 6.2, 6.6, 7.0, 7.5, 8.0, 9.0, 10.0, 11.0,  12.0, 14.0, 16.0, 20.0, 25.0, 30.0};
    std::vector<Double_t> fBinsPi0pPb5TeVPCMDCAPt { // size 17
        0.0, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 1.0, 1.2, 1.4,   1.8, 2.4, 3.5, 5.0, 7.0, 10.0, 14.0};
    std::vector<Double_t> fBinsPi0pPb5TeVPCMCentPt { // size 39
        0.0, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2,   1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0, 2.2, 2.4,
        2.6, 2.8, 3.0, 3.2, 3.4, 3.6, 3.8, 4.0, 4.5, 5.0,   5.5, 6.0, 7.0, 8.0, 9.0, 10.0, 12.0, 16.0, 20.0};
    std::vector<Double_t> fBinsPi0pPb5TeVPCMDCACentPt { // size 17
        0.0, 0.5, 0.6, 0.7, 0.8, 1.0, 1.2, 1.4, 1.6,  1.8, 2.0, 2.4, 3.5, 5.0, 7.0, 10.0, 14.0};

    // PCM-EMC    :
    std::vector<Double_t> fBinsPi0pPb5TeVPCMEMCPt { // size 58
        0.0, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3, 1.4, 1.5,   1.6, 1.7, 1.8, 1.9, 2.0, 2.1, 2.2, 2.3, 2.4, 2.5, 
        2.6, 2.7, 2.8, 2.9, 3.0, 3.2, 3.4, 3.6, 3.8, 4.0,   4.2, 4.4, 4.6, 4.8, 5.0, 5.4, 5.8, 6.2, 6.6, 7.0, 
        7.5, 8.0, 8.5, 9.0, 9.5, 10.0, 10.5, 11.0, 11.5, 12.0, 14.0, 16.0, 18.0, 20.0,   25.0, 30.0, 35.0, 40.0};
    std::vector<Double_t> fBinsPi0pPb5TeVPCMEMCTrigEMC7Pt{ // size 18
        0.0, 3.0, 3.5, 4.0, 4.5, 5.0, 5.5, 6.0, 6.5, 7.0,               8.0, 9.0, 10.0, 12.0, 14.0, 16.0, 18.0, 20.0};
    std::vector<Double_t> fBinsPi0pPb5TeVPCMEMCTrigEG2Pt { // size 36
        0.0, 3.0, 3.5, 4.0, 4.5, 5.0, 5.5, 6.0, 6.5, 7.0,               7.5, 8.0, 8.5, 9.0, 9.5, 10.0, 10.5, 11.0, 11.5, 12.0,
        12.5, 13.0, 13.5, 14.0, 15.0, 16.0, 17.0, 18.0, 19.0, 20.0,     22.0, 24.0, 26.0, 30.0, 35.0, 40.0};
    std::vector<Double_t> fBinsPi0pPb5TeVPCMEMCTrigEG1Pt { // size 31
        0.0, 7.0, 7.5, 8.0, 8.5, 9.0, 9.5, 10.0, 10.5, 11.0,            11.5, 12.0, 12.5, 13.0, 13.5, 14.0, 15.0, 16.0, 17.0, 18.0, 
        19.0, 20.0, 22.0, 24.0, 26.0, 28.0, 30.0, 35.0, 40.0, 50.0,     60.0};
    std::vector<Double_t> fBinsPi0pPb5TeVPCMEMCCombTrigPt { // size 69
        0.0, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3, 1.4, 1.5,   1.6, 1.7, 1.8, 1.9, 2.0, 2.1, 2.2, 2.3, 2.4, 2.5, 
        2.6, 2.7, 2.8, 2.9, 3.0, 3.2, 3.4, 3.6, 3.8, 4.0,   4.2, 4.4, 4.6, 4.8, 5.0, 5.4, 5.8, 6.2, 6.6, 7.0, 
        7.5, 8.0, 8.5, 9.0, 9.5, 10.0, 10.5, 11.0, 11.5, 12.0,      12.5, 13.0, 13.5, 14.0, 15.0, 16.0, 17.0, 18.0, 19.0, 20.0, 
        22.0, 24.0, 26.0, 28.0, 30.0, 35.0, 40.0, 50.0, 60.0};
    
    std::vector<Double_t> fBinsPi0pPb5TeVPCMEMCCentPt { // size 39
        0.0, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2,   1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0, 2.2, 2.4,
        2.6, 2.8, 3.0, 3.2, 3.4, 3.6, 3.8, 4.0, 4.5, 5.0,   5.5, 6.0, 7.0, 8.0, 9.0, 10.0, 12.0, 16.0, 20.0};
    std::vector<Double_t> fBinsPi0pPb5TeVPCMEMCTrigEG2CentPt { // size 15
        0.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0,             12.0, 14.0, 16.0, 20.0, 30.0};
    std::vector<Double_t> fBinsPi0pPb5TeVPCMEMCTrigEG1CentPt { // size 20
        0.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0,         16.0, 17.0, 18.0, 19.0, 20.0, 22.0, 24.0, 26.0, 30.0, 40.0};
    std::vector<Double_t> fBinsPi0pPb5TeVPCMEMCCombTrigCentPt { // size 51
        0.0, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2,   1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0, 2.2, 2.4,
        2.6, 2.8, 3.0, 3.2, 3.4, 3.6, 3.8, 4.0, 4.5, 5.0,   5.5, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 
        15.0, 16.0, 17.0, 18.0, 19.0, 20.0, 22.0, 24.0, 26.0, 30.0,     40.0 };


    // EMC:
    std::vector<Double_t> fBinsPi0pPb5TeVEMCPt { // size 48
        0.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9,       2.0, 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 2.9, 
        3.0, 3.2, 3.4, 3.6, 3.8, 4.0, 4.2, 4.4, 4.6, 4.8,       5.0, 5.4, 5.8, 6.2, 6.6, 7.0, 7.5, 8.0, 9.0, 10.0,
        11.0, 12.0, 14.0, 16.0, 18.0, 20.0, 24.0, 30.0};
    std::vector<Double_t> fBinsPi0pPb5TeVEMCTrigEMC7Pt{ // size 18
        0.0, 3.0, 3.5, 4.0, 4.5, 5.0, 5.5, 6.0, 6.5, 7.0,               8.0, 9.0, 10.0, 12.0, 14.0, 16.0, 18.0, 20.0};
    std::vector<Double_t> fBinsPi0pPb5TeVEMCTrigEG2Pt { // size 30
        0.0, 5.8, 6.2, 6.6, 7.0, 7.5, 8.0, 8.5, 9.0, 9.5,               10.0, 10.5, 11.0, 11.5, 12.0, 12.5, 13.0, 13.5, 14.0, 15.0, 
        16.0, 17.0, 18.0, 19.0, 20.0, 22.0, 24.0, 26.0, 28., 30.0 };
    std::vector<Double_t> fBinsPi0pPb5TeVEMCTrigEG1Pt { // size 27
        0.0, 7.0, 7.5, 8.0, 8.5, 9.0, 9.5, 10.0, 10.5, 11.0,            11.5, 12.0, 12.5, 13.0, 13.5, 14.0, 15.0, 16.0, 17.0, 18.0, 
        19.0, 20.0, 22.0, 24.0, 26.0, 28., 30.0 };

    std::vector<Double_t> fBinsPi0pPb5TeVEMCCombTrigPt { // size 61
        0.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9,       2.0, 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 2.9, 
        3.0, 3.2, 3.4, 3.6, 3.8, 4.0, 4.2, 4.4, 4.6, 4.8,       5.0, 5.4, 5.8, 6.2, 6.6, 7.0, 7.5, 8.0, 8.5, 9.0, 
        9.5, 10.0, 10.5, 11.0, 11.5, 12.0, 12.5, 13.0, 13.5, 14.0,  15.0, 16.0, 17.0, 18.0, 19.0, 20.0, 22.0, 24.0, 26.0, 28., 
        30.0 };
    
    std::vector<Double_t> fBinsPi0pPb5TeVEMCCentPt { // size 31
        0.0, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0,    2.2, 2.4, 2.6, 2.8, 3.0, 3.2, 3.4, 3.6, 3.8, 4.0,
        4.5, 5.0, 5.5, 6.0, 7.0, 8.0, 9.0, 10.0, 12.0, 16.0, 20.0};
    std::vector<Double_t> fBinsPi0pPb5TeVEMCTrigEG2CentPt { // size 16
        0.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0,  15.0, 16.0, 17.0, 18.0, 19.0, 20.0};
    std::vector<Double_t> fBinsPi0pPb5TeVEMCTrigEG1CentPt { // size 14
        0.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0,        17.0, 18.0, 19.0, 20.0};
    std::vector<Double_t> fBinsPi0pPb5TeVEMCCombTrigCentPt { // size 38
        0.0, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0,    2.2, 2.4, 2.6, 2.8, 3.0, 3.2, 3.4, 3.6, 3.8, 4.0,
        4.5, 5.0, 5.5, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0, 17.0, 18.0, 19.0, 20.0};
    
    // PHOS:
    std::vector<Double_t> fBinsPi0pPb5TeVPHOSPt { // size 53
        0.0, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3,   1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0, 2.1, 2.2, 2.3, 
        2.4, 2.5, 2.6, 2.7, 2.8, 2.9, 3.0, 3.2, 3.4, 3.6,   3.8, 4.0, 4.2, 4.4, 4.6, 4.8, 5.0, 5.4, 5.8, 6.2, 
        6.6, 7.0, 7.5, 8.0, 9.0, 10.0, 11.0, 12.0, 14.0, 16.0,  20.0, 25.0, 30.0};
        
//     std::vector<Double_t> fBinsPi0pPb5TeVPHOSPt { // size 37
//         0.0, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 1.0, 1.2, 1.4,   1.6, 1.8, 2.0, 2.2, 2.4, 2.6, 2.8, 3.0, 3.2, 3.4,
//         3.6, 3.8, 4.0, 4.5, 5.0, 5.5, 6.0, 7.0, 8.0, 10.0,  12.0, 16.0, 20.0, 25.0, 30.0, 35.0, 40.0};
    std::vector<Double_t> fBinsPi0pPb5TeVPHOSPHI7Pt { // size 35
        0.0, 5.0, 5.4, 5.8, 6.2, 6.6, 7.0, 7.5, 8.0, 8.5,   9.0, 9.5, 10.0, 10.5, 11.0, 11.5, 12.0, 12.5, 13.0, 13.5, 
        14.0, 15.0, 16.0, 17.0, 18.0, 19.0, 20.0, 22.0, 24.0, 26.0,     28.0, 30.0, 35.0, 40.0, 50.0 };
    std::vector<Double_t> fBinsPi0pPb5TeVPHOSDmitriPt { // size 31
        0.0, 0.8, 1.0, 1.2, 1.4, 1.6, 1.8, 2.0, 2.2, 2.4,   2.6, 2.8, 3.0, 3.2, 3.4, 3.6, 3.8, 4.0, 4.5, 5.0,
        5.5, 6.0, 7.0, 8.0, 10.0, 12.0, 14.0, 18.0, 24.0, 32.0, 40.0};
    std::vector<Double_t> fBinsPi0pPb5TeVPHOSCentPt { // size 24
        0.0, 0.8, 1.0, 1.2, 1.4, 1.6, 1.8, 2.0, 2.2, 2.4,   2.6, 2.8, 3.0, 3.4, 4.0, 5.0, 6.0, 8.0, 10.0, 12.0,
        16.0, 24.0, 32.0, 40.0};
        
    // mEMC:
    // std::vector<Double_t> fBinsPi0pPb5TeVmEMCPt { // size 22 binning for pPb8 comparison
    //     0.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16., 18.,
    //     20., 25., 30., 35., 40., 50., 60., 80., 100, 130,       160, 200, 250};
    std::vector<Double_t> fBinsPi0pPb5TeVmEMCPt { // size 26 binning for Fredi combination
        0.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0, 17.0, 18.0,    19.0, 20.0, 22.0, 24.0, 26.0, 28.0, 30.0,
        35.0, 40.0, 50.0, 60.0, 80.0, 100., 130.0, 160.0,200.0};

    // PCM-Dalitz:
    std::vector<Double_t> fBinsPi0pPb5TeVDalitzPt { // size 23
        0, 0.6, 0.7, 0.8, 1.0, 1.2, 1.4, 1.6, 1.8, 2.0,       2.2, 2.4, 2.6, 2.8, 3.0, 3.2, 3.6, 4.0, 5.0, 6.0,
        8.0, 10., 15.};
    // EMC-Dalitz
    std::vector<Double_t> fBinsPi0pPb5TeVEMCDalitzPt { // size 23
        0, 0.6, 0.7, 0.8, 1.0, 1.2, 1.4, 1.6, 1.8, 2.0,     2.2, 2.4, 2.6, 2.8, 3.0, 3.2, 3.6, 4.0, 5.0, 6.0,
        8.0, 10., 15.};

    //______________________________________________________________________________________
    // pt rebins
    //______________________________________________________________________________________
    // PCM
    std::vector<Int_t> fBinsPi0pPb5TeVPCMPtRebin { // size 55
        10, 8, 4, 2, 2, 1, 1, 1, 1, 1,      1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,       1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 2, 2, 4, 5, 8, 8, 10,      10,10, 10, 10,10};
    std::vector<Int_t> fBinsPi0pPb5TeVPCMCentPtRebin { // size 38
        10, 4, 2, 2, 2, 2, 2, 2, 2, 2,      2, 2, 2, 2, 2, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 4, 4, 4, 4, 4, 4,       8, 8, 8, 8, 10, 10, 10, 10};
    // PCM-EMC
    std::vector<Int_t> fBinsPi0pPb5TeVPCMEMCPtRebin { // size 57
        10, 4, 2, 2, 2, 2, 2, 2, 2, 2,      2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2,       2, 2, 2, 2, 2, 2, 2, 2, 2, 4,
        4, 4, 4, 5, 5, 5, 5, 8, 8, 8,       8, 8, 8, 10, 10, 10, 10};
    std::vector<Int_t> fBinsPi0pPb5TeVPCMEMCTrigEMC7PtRebin { // size 17
        10, 4, 4, 4, 4, 4, 4, 4, 4, 4,      5, 8, 8, 8, 8, 10, 10};
    std::vector<Int_t> fBinsPi0pPb5TeVPCMEMCTrigEG2PtRebin { // size 35
        10, 8, 8, 8, 8, 4, 4, 4, 4, 4,      4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
        4, 4, 4, 5, 5, 5, 8, 8, 8, 8,       8, 8, 10, 10, 10 };
    std::vector<Int_t> fBinsPi0pPb5TeVPCMEMCTrigEG1PtRebin { // size 30
        10, 8, 8, 8, 8, 4, 4, 4, 4, 4,      4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 4,  8, 10, 10, 10, 10    };
    std::vector<Int_t> fBinsPi0pPb5TeVCentPCMEMCPtRebin { // size 38
        8, 8, 8, 8, 8, 8, 4, 4, 4, 2,       2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2,       4, 4, 4, 4, 8, 8, 10, 10};
    std::vector<Int_t> fBinsPi0pPb5TeVPCMEMCTrigEG2CentPtRebin { // size 14
        10, 8, 8, 8, 8, 8, 8, 8, 8, 8,      8, 8, 8, 10 };
    std::vector<Int_t> fBinsPi0pPb5TeVPCMEMCTrigEG1CentPtRebin { // size 19
        10, 8, 8, 4, 4, 4, 4, 4, 4, 4,      4, 4, 4, 4, 8, 8, 8, 10, 10};
    std::vector<Int_t> fBinsPi0pPb5TeVPCMEMCTrigEG1CentCentPtRebin { // size 19
        10, 8, 8, 8, 8, 8, 8, 8, 8, 8,      8, 8, 8, 8, 8, 10, 10, 15, 20};
    std::vector<Int_t> fBinsPi0pPb5TeVPCMEMCTrigEG1CentPerPtRebin { // size 19
        10, 8, 8, 8, 8, 8, 8, 8, 8, 8,      8, 8, 8, 8, 8, 10, 10, 15, 20};
    // EMC
    std::vector<Int_t> fBinsPi0pPb5TeVEMCPtRebin { // size 47
        5, 4, 4, 4, 2, 2, 2, 2, 2, 2,       2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2,       4, 4, 4, 4, 4, 4, 4, 4, 4, 5,
        10, 10, 10, 10, 10, 10, 10};
    std::vector<Int_t> fBinsPi0pPb5TeVEMCTrigEMC7PtRebin { // size 17
        10, 8, 4, 4, 4, 4, 4, 4, 4, 4,      4, 5, 5, 8, 8, 10, 10};
    std::vector<Int_t> fBinsPi0pPb5TeVEMCTrigEG2PtRebin { // size 29
        5, 4, 4, 4, 4, 4, 4, 2, 2, 2,       2, 2, 2, 2, 2, 4, 4, 4, 5, 5, 
        5, 5, 5, 8, 8, 8, 8, 8, 8, 8};
    std::vector<Int_t> fBinsPi0pPb5TeVEMCTrigEG1PtRebin { // size 26
        5, 4, 2, 2, 2, 2, 2, 2, 2, 2,       2, 2, 2, 2, 2, 2, 2, 4, 4, 4,
        5, 5, 5, 5, 5, 5};
    std::vector<Int_t> fBinsPi0pPb5TeVCentEMCPtRebin { // size 30
        5, 4, 4, 4, 4, 4, 4, 2, 2, 2,       2, 2, 2, 2, 2, 2, 2, 2, 4, 4,
        4, 4, 4, 4, 4, 8, 8, 8, 10, 10};
    std::vector<Int_t> fBinsPi0pPb5TeVEMCTrigEG2CentPtRebin { // size 18
        5, 8, 4, 4, 4, 4, 4, 4, 4, 4,       4, 4, 4, 4, 4, 8, 10, 10};
    std::vector<Int_t> fBinsPi0pPb5TeVEMCTrigEG2CentPerPtRebin { // size 18
        5, 8, 8, 8, 8, 8, 8, 8, 8, 8,       8, 10, 10, 10, 10, 15, 20, 20};
    std::vector<Int_t> fBinsPi0pPb5TeVEMCTrigEG1CentPtRebin { // size 13
        5, 8, 4, 4, 2, 2, 2, 2, 4, 4,       4, 8, 10};
    std::vector<Int_t> fBinsPi0pPb5TeVEMCTrigEG1CentPerPtRebin { // size 13
        5, 8, 8, 8, 4, 4, 4, 4, 8, 8,       8, 10, 20};
    // PHOS
    std::vector<Int_t> fBinsPi0pPb5TeVPHOSPtRebin { // size 52
        5, 4, 4, 4, 4, 4, 4, 4, 4, 4,       4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 4, 4, 4, 4, 4, 4,       4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 8, 8, 8, 12, 12, 12,    12, 12};
    std::vector<Int_t> fBinsPi0pPb5TeVPHOSPHI7PtRebin { // size 34
        10, 5, 4, 4, 4, 2, 2, 2, 2, 2,      4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 
        5, 5, 5, 5, 5, 5, 5, 5, 8, 8,       8, 10, 10, 10 };
    std::vector<Int_t> fBinsPi0pPb5TeVPHOSCentPtRebin { // size 24
        8, 4, 4, 4, 2, 2, 2, 2, 2, 2,       2, 2, 2, 2, 2, 2, 2, 4, 4, 4,
        10, 10, 10, 10};
    // PCM-PHOS
    std::vector<Int_t> fBinsPi0pPb5TeVPCMPHOSPtRebin { // size 36
        7 , 7 , 7 , 7 , 4, 3, 3, 3, 3, 3,   3 , 3 , 3 , 3 , 3, 3, 3, 3, 4, 4,
        4 , 4 , 4 , 4 , 4, 5, 7, 7, 7, 7,   7, 7, 7, 7, 10, 10};
    std::vector<Int_t> fBinsPi0pPb5TeVPCMPHOSCentPtRebin { // size 24
        8, 4, 4, 4, 4, 4, 4, 4, 4, 4,       4, 4, 4, 4, 4, 4, 4, 4, 8, 8,
        10, 20, 20, 20};
    // mEMC
    std::vector<Int_t> fBinsPi0pPb5TeVmEMCPtRebin { // size 31
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,       2, 2, 2, 2, 4, 4, 4, 4, 4, 4,
        4, 4, 4, 5, 5, 5, 5, 5, 5, 5,       10 };
    // PCM-Dalitz
    std::vector<Int_t> fBinsPi0pPb5TeVDalitzPtRebin { // size 22
        5, 5, 5, 5, 4, 4, 4, 4, 4, 4,       4, 4, 5, 5, 5, 5, 5, 5, 5, 5,
        8, 8};
    // EMC-Dalitz
    std::vector<Int_t> fBinsPi0pPb5TeVEMCDalitzPtRebin { // size 22
        5, 5, 8, 5, 4, 4, 4, 4, 4, 4,       4, 4, 5, 5, 5, 5, 5, 5, 5, 5,
        8, 8};

 
    //----------------------------------------------------------------------------------------------------
    // Eta 
    //----------------------------------------------------------------------------------------------------
    // pt binnings 
    //____________________________________________________________________________________________________
    // Comb:
//     std::vector<Double_t> fBinsEtapPb5TeVCombPt { // size 23
//         0., 0.3, 0.5, 0.7, 0.9, 1.1, 1.4, 1.8, 2.2, 2.6,    3.0, 3.5, 4.,  5.,  6., 8.,  10,  12., 16., 20.,
//         26., 32., 40};        
    std::vector<Double_t> fBinsEtapPb5TeVCombPt { // size 45
        0.0, 0.3, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.2, 1.4,    1.6, 1.8, 2.0, 2.2, 2.4, 2.6, 3.0, 3.4, 3.8, 4.2, 
        4.6, 5.0, 5.5, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0, 17.0, 18.0, 20.0, 22.0, 24.0, 26.0,
        30.0, 35.0, 40.0, 50.0,  60.0};        

//     std::vector<Double_t> fBinsEtapPb5TeVCombCentPt { // size 23
//         0., 0.3, 0.5, 0.7, 0.9, 1.1, 1.4, 1.8, 2.2, 2.6,    3.0, 3.5, 4.,  5.,  6., 8.,  10,  12., 16., 20.,
//         26., 32., 40};        
    std::vector<Double_t> fBinsEtapPb5TeVCombCentPt { // size 29
        0.0, 0.3, 0.4, 0.5, 0.6, 0.8, 1.0, 1.2, 1.4, 1.6, 1.8,  2.2, 2.6, 3.0, 3.5, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0,
        10.0, 12.0, 14.0, 16.0, 18.0, 20.0, 24.0, 30.0, 40.0};
        
    std::vector<Double_t> fBinsEtapPb5TeVPCMPt { // size 32
        0., 0.3, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.2, 1.4,    1.6, 1.8, 2.0, 2.2, 2.4, 2.6, 3.0, 3.4, 3.8, 4.2, 
        4.6, 5.0, 5.5, 6.0, 7.0, 8.0, 10, 12., 16., 20.,    25., 30.};
    std::vector<Double_t> fBinsEtapPb5TeVPCMDCAPt { // size 17
        0.0, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 1.0, 1.2, 1.4,   1.8, 2.4, 3.5, 5.0, 7.0, 10.0, 14.0};
    std::vector<Double_t> fBinsEtapPb5TeVPCMCentPt { // size 18
        0., 0.3, 0.4, 0.6, 0.8, 1.0, 1.2, 1.4, 1.6, 1.8,    2.2, 2.6, 3.0, 3.5, 4.0, 5.0, 6.0, 8.0, 10.0};
    // PCM-EMC
    std::vector<Double_t> fBinsEtapPb5TeVPCMEMCPt { // size 26
        0., 1.0, 1.2, 1.4, 1.6, 1.8, 2.0, 2.2, 2.4, 2.6,    3.0, 3.4, 3.8, 4.2, 4.6, 5., 5.5, 6., 7.0, 8., 
        10,  12., 16., 20., 25., 30.};
    std::vector<Double_t> fBinsEtapPb5TeVPCMEMCTrigEMC7Pt { // size 12
        0., 3.0, 3.5, 4.0, 4.5, 5.0, 6.0, 8.0, 10, 12.,     16.0, 20.0};
    std::vector<Double_t> fBinsEtapPb5TeVPCMEMCTrigEG2Pt { // size 16
        0., 6.0, 7.0, 8.0, 9., 10, 11., 12., 13., 14.,      15.0, 16.0, 18., 20.,  24., 30.};
    std::vector<Double_t> fBinsEtapPb5TeVPCMEMCTrigEG1Pt { // size 16
        0., 11., 12., 13., 14., 15.0, 16.0, 17.0, 18.,  20., 22., 24., 26.0, 30., 35.0, 40.0};
    std::vector<Double_t> fBinsEtapPb5TeVPCMEMCCombTrigPt { // size 37
        0., 1.0, 1.2, 1.4, 1.6, 1.8, 2.0, 2.2, 2.4, 2.6,    3.0, 3.4, 3.8, 4.2, 4.6, 5., 5.5, 6., 7.0, 8.0, 
        9., 10, 11., 12., 13., 14., 15.0, 16.0, 17.0, 18.,  20., 22., 24., 26.0, 30., 35.0, 40.0};
    std::vector<Double_t> fBinsEtapPb5TeVPCMEMCCentPt { // size 24
        0., 0.3, 0.4, 0.6, 0.8, 1.0, 1.2, 1.4, 1.6, 1.8,    2.2, 2.6, 3.0, 3.5, 4.0, 5.0, 6.0, 8.0, 10.0, 14.0,
        20.0, 26.0, 32.0, 40.};
    std::vector<Double_t> fBinsEtapPb5TeVPCMEMCTrigEG2CentPt { // size 9
        0., 4.0, 5.0, 6.0, 8.0, 10, 14., 20.,  30.};
    std::vector<Double_t> fBinsEtapPb5TeVPCMEMCTrigEG1CentPt { // size 9
        0., 9., 10, 12., 14., 16.0, 20., 24.,  30.};
    std::vector<Double_t> fBinsEtapPb5TeVPCMEMCCombTrigCentPt { // size 25
        0., 0.3, 0.4, 0.6, 0.8, 1.0, 1.2, 1.4, 1.6, 1.8,    2.2, 2.6, 3.0, 3.5, 4.0, 5.0, 6.0, 8.0, 10, 12.,
        14., 16.0, 20., 24.,  30.};
    // EMC: 
    std::vector<Double_t> fBinsEtapPb5TeVEMCPt { // size 23
        0., 1.4, 1.6, 1.8, 2.0, 2.2, 2.4, 2.6, 3.0, 3.4,    3.8, 4.2, 4.6, 5.0, 5.5, 6., 7.0, 8.0, 10,  12., 
        16., 20., 25.0};
    std::vector<Double_t> fBinsEtapPb5TeVEMCTrigEMC7Pt { // size 12
        0., 3.0, 3.5, 4.0, 4.5, 5.0, 6.0, 8.0, 10, 12.,     16.0, 20.0};
    std::vector<Double_t> fBinsEtapPb5TeVEMCTrigEG2Pt { // size 20
        0., 5.5, 6.0, 7.0, 8.0, 9., 10, 11., 12., 13.,      14., 15.0, 16.0, 17.0, 18., 20., 22., 26., 30., 40};
    std::vector<Double_t> fBinsEtapPb5TeVEMCTrigEG1Pt { // size 20
        0., 9., 10, 11., 12., 13., 14., 15.0, 16.0, 17.0,   18., 20., 22., 24., 26.0, 30., 35.0, 40.0, 50.0, 60.0};
    std::vector<Double_t> fBinsEtapPb5TeVEMCCombTrigPt { // size 37
        0.0, 1.4, 1.6, 1.8, 2.0, 2.2, 2.4, 2.6, 3.0, 3.4,    3.8, 4.2, 4.6, 5.0, 5.5, 6.0, 7.0, 8.0, 9., 10, 
        11., 12., 13., 14., 15.0, 16.0, 17.0, 18., 20., 22., 24., 26.0, 30., 35.0, 40.0, 50.0, 60.0};
    std::vector<Double_t> fBinsEtapPb5TeVEMCCentPt { // size 18
        0., 1.4, 1.6, 1.8, 2.2, 2.6, 3.0, 3.5, 4.0, 5.0,    6.0, 8.0, 10.0, 14.0, 20.0, 26.0, 32.0, 40.};
    std::vector<Double_t> fBinsEtapPb5TeVEMCTrigEG2CentPt { // size 12
        0., 4.0, 5.0, 6.0, 8.0, 10, 12., 14., 16.0, 18.,    20., 26.};
    std::vector<Double_t> fBinsEtapPb5TeVEMCTrigEG1CentPt { // size 10
        0., 11., 12., 14., 16.0, 18., 20., 24., 30., 40.0};
    std::vector<Double_t> fBinsEtapPb5TeVEMCCombTrigCentPt { // size 21
        0., 1.4, 1.6, 1.8, 2.2, 2.6, 3.0, 3.5, 4.0, 5.0,    6.0, 8.0, 10, 12., 14., 16.0, 18., 20., 24., 30., 
        40.0};
        
        
    // PHOS:
    std::vector<Double_t> fBinsEtapPb5TeVPHOSPt { // size 20
        0., 0.3, 0.5, 0.7, 0.9, 1.1, 1.4, 1.8, 2.2, 3.0,    4.,  5.,  6., 8.,  10,  12., 16., 20., 25., 30.};
    std::vector<Double_t> fBinsEtapPb5TeVPHOSPHI7Pt { // size 18
        0., 5.0, 5.5, 6.0, 7.0, 8.0, 9., 10, 11., 12.,      13., 14., 16.0, 18., 22., 26.0, 30., 40.0};
    std::vector<Double_t> fBinsEtapPb5TeVPHOSDmitriPt { // size 13
        0., 2.6, 3.0, 4.0, 6.0, 8.0, 10.0, 12.0, 16.0, 20.0,    26.0, 32.0, 40.0};
    std::vector<Double_t> fBinsEtapPb5TeVPHOSCentPt { // size 11
        0., 2.6, 3.0, 4.0, 6.0, 10.0, 14.0, 20.0, 26.0, 32.0, 40.0};
    // PCM-PHOS:
    std::vector<Double_t> fBinsEtapPb5TeVPCMPHOSPt { // size 21
        0., 0.3, 0.5, 0.7, 1.1, 1.4, 1.8, 2.2, 2.6, 3.0,    3.5, 4.,  5.,  6., 8.,  10,  12., 16., 20., 25.,
        30.};
    // Dalitz:
    std::vector<Double_t> fBinsEtapPb5TeVDalitzPt { // size 10
        0., 0.6, 1.0, 1.4, 1.8, 2.2, 2.8, 4.4, 6., 10.};

    //______________________________________________________________________________________
    // pt rebins
    //______________________________________________________________________________________
    // PCM:
    std::vector<Int_t> fBinsEtapPb5TeVPCMPtRebin { // size 31
        10, 8, 8, 8, 8, 5, 5, 4, 4, 4,           4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 
        8, 8, 8, 8, 8, 10, 10, 10, 10, 10,       10};
    std::vector<Int_t> fBinsEtapPb5TeVPCMCentPtRebin { // size 18
        10, 10, 10, 10, 8, 5, 5, 5, 5, 5,        5, 5, 5, 5, 8, 8, 8, 10};
    // PCM-EMC:
    std::vector<Int_t> fBinsEtapPb5TeVPCMEMCPtRebin { // size 25
        10, 8, 5, 5, 4, 4, 4, 4, 4, 4,          4, 4, 4, 4, 4, 5, 5, 8, 8, 8, 
        10, 12, 12, 12, 12 };
    std::vector<Int_t> fBinsEtapPb5TeVPCMEMCTrigEMC7PtRebin { // size 11
        10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 20};
    std::vector<Int_t> fBinsEtapPb5TeVPCMEMCTrigEG2PtRebin { // size 15
        10, 10, 10, 10, 10, 10, 10, 10, 12, 12,     12, 20, 20, 20, 20};
    std::vector<Int_t> fBinsEtapPb5TeVPCMEMCTrigEG1PtRebin { // size 15
        10, 10, 8, 8, 8, 8, 8, 8, 8, 10,            10, 12, 12, 16, 20};
    std::vector<Int_t> fBinsEtapPb5TeVPCMEMCCentPtRebin { // size 23
        10, 10, 10, 10, 10, 10, 10, 10, 10, 8,         8, 8, 8, 8, 8, 8, 10, 12, 15, 15,
        15,  15,  15};
    std::vector<Int_t> fBinsEtapPb5TeVPCMEMCTrigEG2CentPtRebin { // size 8
        10, 12, 12, 12, 12, 12, 20, 20};
    std::vector<Int_t> fBinsEtapPb5TeVPCMEMCTrigEG1CentPtRebin { // size 8
        10, 12, 12, 12, 12, 12, 12, 20};
    // EMC
    std::vector<Int_t> fBinsEtapPb5TeVEMCPtRebin { // size 22
        10, 8, 8, 8, 8, 8, 8, 8, 5, 5,          5, 5, 5, 5, 8, 10, 10, 15, 15, 15,
        20, 20};
    std::vector<Int_t> fBinsEtapPb5TeVEMCTrigEMC7PtRebin { // size 11
        10, 8, 8, 8, 8, 8, 8, 10, 10, 10,        20 };
    std::vector<Int_t> fBinsEtapPb5TeVEMCTrigEG2PtRebin { // size 19
        10, 8, 8, 8, 5, 5, 5, 8, 8, 8,          8, 8, 8, 10, 10, 10, 10, 20, 20};
    std::vector<Int_t> fBinsEtapPb5TeVEMCTrigEG1PtRebin { // size 19
        10, 8, 8, 8, 5, 5, 5, 5, 5, 5,          5, 5, 10, 10, 10, 10, 10, 10, 20 };
    std::vector<Int_t> fBinsEtapPb5TeVEMCCentPtRebin { // size 17
        10, 10, 8, 8, 8, 8, 8, 8, 8, 8,         8, 8, 8, 10, 20,  20, 20};
    std::vector<Int_t> fBinsEtapPb5TeVEMCCentPerPtRebin { // size 17
        10, 10, 10, 10, 10, 10, 10, 10, 10, 10,         16, 16, 16, 20, 20,  20, 20};
    std::vector<Int_t> fBinsEtapPb5TeVEMCTrigEG2CentPtRebin { // size 13
        10, 10, 8, 8, 8, 8, 8, 8, 8, 8, 10, 10, 20};
    std::vector<Int_t> fBinsEtapPb5TeVEMCTrigEG2CentPerPtRebin { // size 13
        10, 10, 12, 12, 12, 12, 12, 12, 12, 16, 20, 20, 40};
    std::vector<Int_t> fBinsEtapPb5TeVEMCTrigEG1CentPtRebin { // size 9
        10, 10, 8, 8, 8, 10, 15, 20, 20};
    std::vector<Int_t> fBinsEtapPb5TeVEMCTrigEG1CentPerPtRebin { // size 9
        10, 12, 12, 12, 12, 15, 20, 25, 25};
        
    // PHOS
    std::vector<Int_t> fBinsEtapPb5TeVPHOSPtRebin { // size 19
        19, 17, 17, 17, 15, 15, 15, 15, 15, 15, 15, 15, 20, 25, 25, 25, 24, 19, 19};
    std::vector<Int_t> fBinsEtapPb5TeVPHOSPHI7PtRebin { // size 17
        10, 8, 8, 8, 8, 8, 8, 8, 8, 8,      8, 10, 10, 15, 15, 15, 20};
    std::vector<Int_t> fBinsEtapPb5TeVPHOSCentPtRebin { // size 14
        10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10};
    // PCM-PHOS
    std::vector<Int_t> fBinsEtapPb5TeVPCMPHOSPtRebin { // size 20
        16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 21, 21, 21, 21, 21};
    std::vector<Int_t> fBinsEtapPb5TeVPCMPHOSCentPtRebin { // size 14
        20, 20, 15, 12, 12, 12, 12, 12, 12, 15, 15, 20, 20, 20};
    // Dalitz
    std::vector<Int_t> fBinsEtapPb5TeVDalitzPtRebin { // size 9
        10, 10, 10, 10, 10, 10, 10, 10, 10};

    //______________________________________________________________________________________
    // pt rebins pi0Eta 
    //______________________________________________________________________________________
    // PCM
    std::vector<Int_t> fBinsPi0EtapPb5TeVPCMPtRebin{ // size 32
        8, 2, 1, 1, 1, 1, 1, 1, 1, 1,           1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
        1, 2, 2, 2, 4, 4, 4, 8, 8, 8,           8};
    std::vector<Int_t> fBinsPi0EtapPb5TeVPCMCentPtRebin { // size 18
        5, 5, 5, 5, 4, 2, 2, 2, 2, 2,           2, 2, 2, 2, 4, 4, 8, 10};
        
    // PCM-EMC:
    std::vector<Int_t> fBinsPi0EtapPb5TeVPCMEMCPtRebin { // size 25
        10, 2, 1, 1, 1, 1, 1, 1, 1, 1,          1, 1, 1, 1, 1, 2, 2, 4, 4, 4, 
        4, 4, 10, 10, 10 };
    std::vector<Int_t> fBinsPi0EtapPb5TeVPCMEMCTrigEMC7PtRebin{ // size 11
        8, 4, 2, 2, 2, 2, 4, 4, 8, 8, 10};
    std::vector<Int_t> fBinsPi0EtapPb5TeVPCMEMCTrigEG2PtRebin { // size 15
        10, 4, 4, 4, 4, 4, 4, 4, 4, 4,      4, 4, 4, 4, 8, 10, 10};
    std::vector<Int_t> fBinsPi0EtapPb5TeVPCMEMCTrigEG1PtRebin { // size 15
        10, 5, 4, 4, 4, 4, 4, 4, 4, 4,       4, 4, 5, 8, 10};
    std::vector<Int_t> fBinsPi0EtapPb5TeVPCMEMCCentPtRebin { // size 23
        10, 2, 2, 2, 2, 2, 2, 2, 2, 2,         4, 4, 4, 4, 4, 4, 5, 5, 10, 10,
        10,  10,  10};
    std::vector<Int_t> fBinsPi0EtapPb5TeVPCMEMCTrigEG2CentPtRebin { // size 8
        10, 4, 4, 4, 4, 4, 4, 8, 10};
    std::vector<Int_t> fBinsPi0EtapPb5TeVPCMEMCTrigEG1CentPtRebin { // size 8
        10, 4, 4, 4, 4, 6, 6, 8, 10};
    // EMC
    std::vector<Int_t> fBinsPi0EtapPb5TeVEMCPtRebin { // size 22
        5, 4, 4, 4, 4, 4, 4, 4, 2, 2,          2, 2, 2, 2, 4, 5, 5, 6, 6, 6,
        10, 10};
    std::vector<Int_t> fBinsPi0EtapPb5TeVEMCTrigEMC7PtRebin { // size 11
        10, 4, 4, 4, 4, 4, 4, 5, 5, 5,         10 };
    std::vector<Int_t> fBinsPi0EtapPb5TeVEMCTrigEG2PtRebin { // size 19
        10, 4, 4, 4, 2, 2, 2, 2, 2, 2,         4, 4, 4, 5, 5, 8, 10, 10, 10 };
    std::vector<Int_t> fBinsPi0EtapPb5TeVEMCTrigEG1PtRebin { // size 19
        10, 4, 4, 4, 4, 4, 4, 2, 2, 2,         2, 2, 2, 5, 5, 5, 5, 10, 10 };
    std::vector<Int_t> fBinsPi0EtapPb5TeVEMCCentPtRebin { // size 17
        10, 5, 4, 4, 4, 4, 4, 4, 4, 4,         4, 4, 4, 5, 8,  10, 10};
    std::vector<Int_t> fBinsPi0EtapPb5TeVEMCTrigEG2CentPtRebin { // size 13
        10, 5, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 8};
    std::vector<Int_t> fBinsPi0EtapPb5TeVEMCTrigEG2CentPerPtRebin { // size 13
        10, 5, 5, 5, 5, 5, 5, 5, 8, 8, 8, 8, 16};
    std::vector<Int_t> fBinsPi0EtapPb5TeVEMCTrigEG1CentPtRebin { // size 9
        10, 5, 4, 4, 4, 5, 5, 10, 10};
    std::vector<Int_t> fBinsPi0EtapPb5TeVEMCTrigEG1CentPerPtRebin { // size 9
        10, 5, 4, 4, 8, 10, 10, 15, 15};

    //----------------------------------------------------------------------------------------------------
    // Eta Run 2
    //----------------------------------------------------------------------------------------------------
    std::vector<Double_t> fBinsEtapPb5TeVPHOSR2Pt { // size 31
        0., 1.0, 1.1, 1.2, 1.4, 1.6, 1.8, 2.0, 2.2, 2.4,    2.6, 2.8, 3.0, 3.2, 3.4, 3.6, 3.8, 4.0, 4.5, 5.0,
        5.5, 6.0, 7.0, 8.0, 9.0, 10.0, 12.0, 14.0, 16.0, 20.0,  25.0};
    std::vector<Double_t> fBinsEtapPb5TeVPHOSR2CentPt { // size 22
        0., 1.0, 1.1, 1.2, 1.4, 2.0, 2.6, 3.0, 3.4, 3.6,    4.0, 4.5, 5.0, 5.5, 6.0, 7.0, 8.0, 10.0, 12.0, 16.0,
        20.0,  25.0};
    std::vector<Double_t> fBinsEtapPb5TeVPCMPHOSR2Pt { // size 29
        0., 0.8, 0.9, 1.0, 1.1, 1.2, 1.4, 1.6, 1.8, 2.0,    2.2, 2.4, 2.6, 2.8, 3.0, 3.2, 3.4, 3.6, 3.8, 4.0,
        4.5, 5.0, 5.5, 6.0, 7.0, 8.0, 10.0, 12.0, 16.0};

    std::vector<Double_t> fBinsEtapPb5TeVPCMPHOSR2SCentPt{ // size 25
        0., 0.8, 0.9, 1.0, 1.1, 1.2, 1.4, 1.6, 1.8, 2.0,    2.2, 2.4, 2.6, 2.8, 3.0, 3.2, 3.4, 3.6, 3.8, 4.0,
        4.5, 5.0, 6.0, 8.0, 12.0};
    // Eta pt binning per run 2
    std::vector<Double_t> fBinsEtapPb5TeVPCMPHOSR2PerPt{ // size 27
        0., 0.8, 0.9, 1.0, 1.1, 1.2, 1.4, 1.6, 1.8, 2.0,    2.2, 2.4, 2.6, 2.8, 3.0, 3.2, 3.4, 3.6, 3.8, 4.0,
        4.5, 5.0, 5.5, 6.0, 8.0, 10.0, 12.0};

    std::vector<Int_t> fBinsEtapPb5TeVPHOSR2PtRebin{ // size 30
        10, 10, 8, 8, 8, 8, 5, 5, 5, 5,         5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
        8, 8, 8, 8, 8, 10, 10, 16, 16, 20 };
    std::vector<Int_t> fBinsEtapPb5TeVPHOSR2CentPtRebin{ // size 21
        10, 10, 8, 8, 5, 5, 5, 5, 5, 5,         5, 5, 5, 5, 5, 5, 5, 5, 8, 8,
        8 };
    std::vector<Int_t> fBinsEtapPb5TeVPCMPHOSR2PtRebin{ // size 28
        20, 10, 10, 10, 10, 8, 8, 8, 8, 8,      5, 5, 5, 5, 5, 5, 5, 5, 8, 8,
        8, 10, 10, 10, 16, 20, 20, 20};

    std::vector<Int_t> fBinsEtapPb5TeVCentPCMPHOSR2PtRebin{ // size 32
        20, 10, 10, 10, 5, 5, 5, 5, 5, 5,       4, 4, 2, 2, 2, 2, 2, 2, 2, 4,
        4, 5, 5, 8, 8, 10, 10, 16, 16, 20,      20, 20};
    // Eta cent rebin run 2
    std::vector<Int_t> fBinsEtapPb5TeVPCMPHOSR2CentPtRebin{ // size 28
        20, 10, 10, 10, 10, 10, 10, 10, 8, 8,   8, 8, 8, 8, 8, 8, 8, 8, 10, 10,
        10, 15, 15, 20, 20, 20, 20, 20};
    // Eta cent (small) rebin run 2
    std::vector<Int_t> fBinsEtapPb5TeVPCMPHOSR2SCentPtRebin{ // size 24
        20, 10, 10, 10, 10, 10, 10, 10, 8, 8,   8, 8, 8, 8, 8, 8, 8, 8, 10, 10,
        15, 15, 20, 20};

    // Eta peri rebin run 2
    std::vector<Int_t> fBinsEtapPb5TeVPCMPHOSR2PerPtRebin{ // size 26
        20, 10, 10, 10, 10, 10, 10, 10, 8, 8,      8, 8, 8, 8, 8, 8, 10, 10, 10, 15,
        15, 20, 20, 20, 20, 20};


    // Pi0 Eta binning rebin factors Run 1
    std::vector<Int_t> fBinsPi0EtapPb5TeVPCMPHOSPtRebin{ // size 22
        8, 2, 1, 1, 1, 1, 1, 1, 1, 1,           1, 1, 2, 2, 2, 4, 4, 4, 4, 4,
        4, 4};
    std::vector<Int_t> fBinsPi0EtapPb5TeVPHOSPtRebin{ // size 22
        8, 2, 1, 1, 1, 1, 1, 1, 1, 1,           1, 1, 1, 1, 1, 2, 2, 2, 4, 4,
        4, 4};
    std::vector<Int_t> fBinsPi0EtapPb5TeVDalitzPtRebin{ // size 9
        8, 2, 2, 2, 2, 2, 4, 4, 4};
    std::vector<Int_t> fBinsPi0EtapPb5TeVPCMPHOSCentPtRebin{ // size 14
        10, 4, 4, 2, 2, 2, 2, 2, 2, 2,          4, 4, 8, 10};
    std::vector<Int_t> fBinsPi0EtapPb5TeVPHOSCentPtRebin{ // size 14
        10, 8, 8, 4, 2, 2, 2, 2, 2, 2,          4, 4, 8, 10};


    // Pi0 Eta binning rebin factors Run 2
    std::vector<Int_t> fBinsPi0EtapPb5TeVPHOSR2PtRebin{ // size 32
        4, 2, 2, 1, 1, 1, 1, 1, 1, 1,           1, 1, 1, 1, 1, 1, 1, 2, 2, 2,
        2, 2, 2, 4, 4, 4, 8, 8, 10, 16,         20, 20};
    std::vector<Int_t> fBinsPi0EtapPb5TeVPCMPHOSR2PtRebin{ // size 37
        10, 10, 5, 5, 2, 2, 2, 2, 1, 1,         1, 1, 1, 1, 1, 1, 1, 1, 1, 2,
        2, 2, 2, 4, 4, 5, 5, 5, 5, 8,           8, 8, 10, 10, 10, 10, 10};

    //----------------------------------------------------------------------------------------------------
    // Eta Run 1 pi0, pi+, pi- body
    //----------------------------------------------------------------------------------------------------
    std::vector<Double_t> fBinsEtapPb5TeVPt3Body{ // size 15
        0., 0.4, 0.6, 0.8, 1.0, 1.2, 1.4, 1.6, 2.0, 2.5,    3.0, 4.0, 6.0, 8.0, 10};
    std::vector<Int_t> fBinsEtapPb5TeVPt3BodyRebin{ // size 14
        5, 5, 5, 5, 5, 5, 5, 5, 5, 5,           5, 5, 5, 5};

    //----------------------------------------------------------------------------------------------------
    // DirGamma binning Run 1
    //----------------------------------------------------------------------------------------------------
    Double_t fBinsDirGammapPb5TeVPCMPt[31]          = { 0.0, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 1.0, 1.2, 1.4,   1.6, 1.8, 2.0, 2.2, 2.4, 2.6, 2.8, 3.0, 3.2, 3.4,
                                                        3.6, 3.8, 4.0, 4.5, 5.0, 5.5, 6.0, 7.0, 8.0, 10.0,  14.0};
    Double_t fBinsDirGammapPb5TeVCentPCMPt[26]      = { 0.0, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 1.0, 1.2, 1.4,   1.6, 1.8, 2.0, 2.2, 2.4, 2.6, 2.8, 3.0, 3.4,
                                                        4.0, 5.0, 6.0, 7.0, 8.0, 10.0,  14.0};
    Double_t fBinsDirGammapPb5TeVEMCPt[34]          = { 0.0, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 1.0, 1.2, 1.4,   1.6, 1.8, 2.0, 2.2, 2.4, 2.6, 2.8, 3.0, 3.2, 3.4,
                                                        3.6, 3.8, 4.0, 4.5, 5.0, 5.5, 6.0, 7.0, 8.0, 10.0,  12.0, 14.0, 16.0, 20.};
    Double_t fBinsDirGammapPb5TeVPCMEMCPt[33]       = { 0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 1.0,   1.2, 1.4, 1.6, 1.8, 2.0, 2.2, 2.4, 2.6, 2.8, 3.0,
                                                        3.2, 3.4, 3.6, 3.8, 4.0, 4.5, 5.0, 5.5, 6.0, 7.0,   8.0, 10.0, 14.0};
    Double_t fBinsDirGammapPb5TeVPt[42]             = { 0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 1.0,   1.2, 1.4, 1.6, 1.8, 2.0, 2.2, 2.4, 2.6, 2.8, 3.0,
                                                        3.2, 3.4, 3.6, 3.8, 4.0, 4.5, 5.0, 5.5, 6.0, 7.0,   8.0, 10.0, 12.0, 14.0, 16.0, 18.0, 20.0, 22.0, 26.0, 30.0,
                                                        35.0, 40.0  };
    Double_t fBinsDirGammapPb5TeVAlterPt[37]        = { 0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 1.0,   1.2, 1.4, 1.6, 1.8, 2.0, 2.2, 2.4, 2.6, 2.8, 3.0,
                                                        3.2, 3.4, 3.6, 3.8, 4.0, 4.5, 5.0, 5.5, 6.0, 7.0,   8.0, 10.0, 14.0, 18.0, 24.0, 32.0, 40.0};
    Double_t fBinsDirGammapPb5TeVAlter2Pt[38]       = { 0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 1.0,   1.2, 1.4, 1.6, 1.8, 2.0, 2.2, 2.4, 2.6, 2.8, 3.0,
                                                        3.2, 3.4, 3.6, 3.8, 4.0, 4.5, 5.0, 5.5, 6.0, 7.0,   8.0, 10.0, 12.0, 14.0, 18.0, 24.0, 32.0, 40.0};
    Double_t fBinsDirGammapPb5TeVCombinationPt[30]  = { 0.0, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 1.0, 1.2, 1.4,   1.6, 1.8, 2.0, 2.2, 2.4, 2.6, 2.8, 3.0, 3.4,
                                                        4.0, 5.0, 6.0, 7.0, 8.0, 10.0, 12.0, 16.0, 24.0, 32.0, 40.0};
    Int_t fBinsDirGammapPb5TeVPtRebin[30]           = { 4, 2, 1, 1, 1, 1, 1, 1, 1, 1,           1, 1, 1, 1, 1, 1, 1, 1, 2, 2,
                                                        2, 4, 4, 4, 4, 4, 4, 4, 8, 8};
    Int_t fBinsDirGammapPb5TeVPCMPtRebin[30]        = { 4, 2, 1, 1, 1, 1, 1, 1, 1, 1,           1, 1, 1, 1, 1, 1, 1, 1, 2, 2,
                                                        2, 4, 4, 4, 4, 4, 4, 4, 8, 8};
    Int_t fBinsDirGammapPb5TeVCentPCMPtRebin[25]    = { 4, 2, 1, 1, 1, 1, 1, 1, 1, 1,           1, 1, 1, 2, 2,
                                                        2, 4, 4, 4, 4, 4, 4, 4, 8, 8};
    Int_t fBinsDirGammapPb5TeVEMCPtRebin[33]        = { 4, 2, 2, 2, 2, 2, 2, 2, 2, 2,           1, 1, 1, 1, 1, 1, 1, 1, 2, 2,
                                                        2, 4, 4, 4, 4, 4, 4, 4, 8, 8,           8, 10, 12};
    Int_t fBinsDirGammapPb5TeVPCMEMCPtRebin[32]     = { 10, 4, 4, 2, 2, 2, 2, 2, 2, 1,          1, 1, 1, 1, 1, 1, 2, 2, 2, 2,
                                                        2, 4, 4, 4, 4, 4, 4, 4, 5, 5,           8, 8 };
    Int_t fBinsDirGammapPb5TeVCentPCMEMCPtRebin[25] = { 4, 4, 4, 4, 4, 4, 4, 2, 2, 2,           2, 2, 2, 2, 2,
                                                        2, 4, 4, 4, 4, 4, 4, 4, 8, 8};

    //----------------------------------------------------------------------------------------------------
    // DirGamma binning Run 2
    //----------------------------------------------------------------------------------------------------
    Double_t fBinsDirGammapPb5TeVR2PCMPt[87]             = { 0.0, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7,   0.75, 0.8, 0.85, 0.9, 0.95, 1.0, 1.05, 1.1, 1.15, 1.2,
                                                        1.25, 1.3, 1.35, 1.4, 1.45, 1.5, 1.55, 1.6, 1.65, 1.7,  1.75, 1.8, 1.85, 1.9, 1.95, 2.0, 2.05, 2.1, 2.15, 2.2,
                                                        2.25, 2.3, 2.35, 2.4, 2.45, 2.5, 2.6, 2.7, 2.8, 2.9,    3.0, 3.1, 3.2, 3.3, 3.4, 3.5, 3.6, 3.7, 3.8, 3.9,
                                                        4.0, 4.2, 4.4, 4.6, 4.8, 5.0, 5.2, 5.4, 5.6, 5.8,       6.0, 6.2, 6.4, 6.6, 6.8, 7.0, 7.5, 8.0, 8.5, 9.0,
                                                        10.0, 11.0, 12.0, 14.0, 16.0, 20.0, 25.0};
    Double_t fBinsDirGammapPb5TeVR2CentPCMPt[81]         = { 0.0, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7,   0.75, 0.8, 0.85, 0.9, 0.95, 1.0, 1.05, 1.1, 1.15, 1.2,
                                                        1.25, 1.3, 1.35, 1.4, 1.45, 1.5, 1.55, 1.6, 1.65, 1.7,  1.75, 1.8, 1.85, 1.9, 1.95, 2.0, 2.05, 2.1, 2.15, 2.2,
                                                        2.25, 2.3, 2.35, 2.4, 2.45, 2.5, 2.6, 2.7, 2.8, 2.9,    3.0, 3.1, 3.2, 3.3, 3.4, 3.5, 3.6, 3.7, 3.8, 3.9,
                                                        4.0, 4.2, 4.4, 4.6, 4.8, 5.0, 5.2, 5.4, 5.6, 5.8,       6.0, 6.2, 6.6, 7.0, 7.5, 8.0, 9.0,
                                                        12.0, 16.0, 20.0, 25.0};
    Double_t fBinsDirGammapPb5TeVR2Cent2PCMPt[46]        = { 0.0, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2,
                                                        1.3, 1.4, 1.5,1.6, 1.7, 1.8,  1.9,  2.0,  2.1, 2.2,
                                                        2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 2.9,    3.0,  3.2,  3.4, 3.6,  3.8, 4.0,  4.5,  5.0, 5.5,
                                                        6.0,  7.0, 9.0, 12.0, 16.0, 20.0, 25.0};
    Double_t fBinsDirGammapPb5TeVR2Cent3PCMPt[29]        = { 0.0, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2,
                                                        1.4,1.6, 1.8,   2.0,
                                                         2.4, 2.8,  3.2, 3.6, 4.0,  4.5,  5.0,
                                                        6.0,  8.0, 12.0, 16.0, 20.0, 25.0};
    Int_t fBinsDirGammapPb5TeVR2PCMPtRebin[86]           = { 5, 4, 4, 4, 4, 2, 2, 2, 2, 2,       2, 2, 2, 2, 2, 2, 2, 1, 1, 1,
                                                        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,       1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                                                        1, 1, 1, 1, 1, 1, 2, 2, 2, 2,       2, 2, 2, 2, 2, 2, 2, 2, 4, 4,
                                                        4, 4, 4, 4, 4, 4, 4, 4, 4, 4,       4, 4, 4, 4, 4, 5, 5, 5, 5, 5,
                                                        5, 5, 10, 15, 20, 20 };
    Int_t fBinsDirGammapPb5TeVR2CentPCMPtRebin[80]       = { 5, 4, 4, 4, 4, 2, 2, 2, 2, 2,       2, 2, 2, 2, 2, 2, 2, 1, 1, 1,
                                                        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,       1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                                                        1, 1, 1, 1, 1, 1, 2, 2, 2, 2,       2, 2, 2, 2, 2, 2, 2, 2, 4, 4,
                                                        4, 4, 4, 4, 4, 4, 4, 4, 4, 4,       4, 4, 4, 4, 4, 5,
                                                        5, 10, 15, 20 };
    Int_t fBinsDirGammapPb5TeVR2Cent2PCMPtRebin[45]      = {10,10, 4, 4, 2, 2, 1, 1, 1, 1,       1, 1, 1, 1, 1, 1, 1, 1,
                                                        2, 2, 2, 2, 2, 4, 4, 4,       4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
                                                        4, 4, 4, 4, 5, 5,10,15,20 };
    Int_t fBinsDirGammapPb5TeVR2Cent3PCMPtRebin[28]      = {10,10, 4, 4, 2, 2, 1, 1, 1, 1,
                                                        2, 2, 2, 2, 2, 4, 4, 4, 4,
                                                        4, 4, 4, 4, 5, 5,10,15,20 };

    //----------------------------------------------------------------------------------------------------
    // Cluster binning
    //----------------------------------------------------------------------------------------------------
    Int_t fNBinsClusterpPb5TeVPt                    =  73;
    Double_t fBinsClusterpPb5TeVPt[74]              = { 0.0, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1,
                                                        1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0, 2.2,
                                                        2.4, 2.6, 2.8, 3.0, 3.2, 3.4, 3.6, 3.8, 4.0, 4.2,
                                                        4.4, 4.6, 4.8, 5.0, 5.2, 5.4, 5.6, 5.8, 6.0, 6.2,
                                                        6.4, 6.6, 6.8, 7.0, 7.4, 7.8, 8.2, 8.6, 9.0, 9.5,
                                                        10,  11,  12,  13., 14,  15.0, 16, 17.0, 18, 19.0,
                                                        20,  22.5, 25, 27.5, 30, 35, 40,  45, 50,  60,
                                                        70,  80,  90,  100};

    //****************************************************************************************************
    //******************** EtaPrime Pt binning for pPb, 5.023TeV  ****************************************
    //****************************************************************************************************

    // * PCM-PCM   (mode 100) *
    std::vector<Double_t> fBinsEtaPrimepPb5TeV_PCM_INT7_Pt {      // size: 8
        0.0, 0.4, 0.8, 1.5, 2.5,    5.0, 10.0, 20.0 };
    std::vector<Int_t>    fBinsEtaPrimepPb5TeV_PCM_INT7_PtRebin { // bins: 7
        10, 4, 8, 12, 12,     16, 20 };
    std::vector<Double_t> fBinsEtaPrimepPb5TeV_PCM_L0_Pt {        // size: 6
        0.0, 4.0, 7.0, 10.0, 20.0,     30. };
    std::vector<Int_t>    fBinsEtaPrimepPb5TeV_PCM_L0_PtRebin {   // bins: 5
        10, 12, 12, 16, 16 };
    std::vector<Double_t> fBinsEtaPrimepPb5TeV_PCM_EG1_Pt {       // size: 7
        0.0, 7.0, 8.0, 10.0, 15.0,       20.0, 30. };
    std::vector<Int_t>    fBinsEtaPrimepPb5TeV_PCM_EG1_PtRebin {  // bins: 6
        10, 10, 10, 12, 20,      20 };
    std::vector<Double_t> fBinsEtaPrimepPb5TeV_PCM_EG2_Pt {       // size: 7
        0.0, 4.0, 6.0, 8.0, 10.0,       15.0, 30. };
    std::vector<Int_t>    fBinsEtaPrimepPb5TeV_PCM_EG2_PtRebin {  // bins: 6
        10, 8, 10, 10, 12,      20 };

    // * PCM-EMC   (mode 102) *
    std::vector<Double_t> fBinsEtaPrimepPb5TeV_PCMEMC_INT7_Pt {      // size: 10
        0.0, 0.8, 1.2, 1.6, 2.0,       2.5, 3.0, 5.0, 10.0, 20.0 };
    std::vector<Int_t>    fBinsEtaPrimepPb5TeV_PCMEMC_INT7_PtRebin { // bins: 9
        10, 8, 12, 12, 12,      12, 12, 16, 20 };
    std::vector<Double_t> fBinsEtaPrimepPb5TeV_PCMEMC_L0_Pt {        // size: 10
        0.0, 0.8, 1.2, 1.6, 2.0,        2.5, 3.0, 5.0, 10.0, 20.0 };
    std::vector<Int_t>    fBinsEtaPrimepPb5TeV_PCMEMC_L0_PtRebin {   // bins: 9
        10, 16, 16, 16, 12,       12, 12, 16, 20 };
    std::vector<Double_t> fBinsEtaPrimepPb5TeV_PCMEMC_EG1_Pt {       // size: 9
        0.0, 7.0, 8.0, 9.0, 10.0,       12.0, 15.0, 20.0, 30.0 };
    std::vector<Int_t>    fBinsEtaPrimepPb5TeV_PCMEMC_EG1_PtRebin {  // bins: 8
        10, 20, 16, 12, 16,            20, 25, 25 };
    std::vector<Double_t> fBinsEtaPrimepPb5TeV_PCMEMC_EG2_Pt {       // size: 11
        0.0, 4.0, 5.0, 6.0, 7.0,        8.0, 10.0, 12.0, 15.0, 20.0,
        30.0 };
    std::vector<Int_t>    fBinsEtaPrimepPb5TeV_PCMEMC_EG2_PtRebin {  // bins: 10
        10, 10, 10, 12, 12,            12, 16, 16, 25, 25, 25 };

    // * PCM-PHOS  (mode 103) *
    std::vector<Double_t> fBinsEtaPrimepPb5TeV_PCMPHOS_INT7_Pt {      // size: 7
        0.0, 0.8, 1.5, 2.5, 5.0,        15.0, 30. };
    std::vector<Int_t>    fBinsEtaPrimepPb5TeV_PCMPHOS_INT7_PtRebin { // bins: 6
        10, 20, 20, 16, 20,         25 };
    std::vector<Double_t> fBinsEtaPrimepPb5TeV_PCMPHOS_VZERO_Pt {        // size: 8
        0.0, 5.5, 6.0, 6.5, 7.0,        8.0, 15.0, 30. };
    std::vector<Int_t>    fBinsEtaPrimepPb5TeV_PCMPHOS_VZERO_PtRebin {   // bins: 7
        10, 20, 20, 20, 20,         20, 25 };

    // * EMC-EMC   (mode 104) *
    std::vector<Double_t> fBinsEtaPrimepPb5TeV_EMC_INT7_Pt {      // size: 10
        0.0, 1.4, 2.2, 3.0, 3.5,        4.0, 5.0, 8.0, 15.0, 20.0 };
    std::vector<Int_t>    fBinsEtaPrimepPb5TeV_EMC_INT7_PtRebin { // bins: 9
        10, 12, 10, 8, 8,       8, 12, 16, 25 };
    std::vector<Double_t> fBinsEtaPrimepPb5TeV_EMC_L0_Pt {        // size: 7
        0.0, 3.0, 4.0, 5.0, 8.0,        15.0, 30.0 };
    std::vector<Int_t>    fBinsEtaPrimepPb5TeV_EMC_L0_PtRebin {   // bins: 6
        10, 16, 16, 16, 20,         25 };
    std::vector<Double_t> fBinsEtaPrimepPb5TeV_EMC_EG1_Pt {       // size: 16
        0.0, 7.5, 8.0, 8.5, 9.0,        9.5, 10.0, 10.5, 11.0, 11.5,
        12.0, 13.0, 14.0, 16.0, 20.0,   30.0 };
    std::vector<Int_t>    fBinsEtaPrimepPb5TeV_EMC_EG1_PtRebin {  // bins: 15
        10, 4, 4, 4, 4,         4, 8, 8, 8, 8,
        8, 10, 10, 10, 10 };
    std::vector<Double_t> fBinsEtaPrimepPb5TeV_EMC_EG2_Pt {       // size: 13
        0.0, 4.0, 4.5, 5.0, 5.5,        6.0, 6.5, 7.0, 8.0, 10.0,
        15.0, 20.0, 30. };
    std::vector<Int_t>    fBinsEtaPrimepPb5TeV_EMC_EG2_PtRebin {  // bins: 12
        10, 8, 8, 8, 8,     8, 8, 10, 10, 10,
        12, 16 };

    // * PHOS-PHOS (mode 105) *
    std::vector<Double_t> fBinsEtaPrimepPb5TeV_PHOS_INT7_Pt {      // size: 6
        0.0, 1.4, 4.0, 6.0, 10.0, 20.0 };
    std::vector<Int_t>    fBinsEtaPrimepPb5TeV_PHOS_INT7_PtRebin { // bins: 5
        10, 16, 8, 8, 8 };
    std::vector<Double_t> fBinsEtaPrimepPb5TeV_PHOS_VZERO_Pt {        // size: 9
        0.0, 6.0, 7.0, 8.0, 10.0,       12.5, 15.0, 20.0, 30. };
    std::vector<Int_t>    fBinsEtaPrimepPb5TeV_PHOS_VZERO_PtRebin {   // bins: 8
        10, 4, 4, 8, 8,      8, 8, 12 };


#endif
