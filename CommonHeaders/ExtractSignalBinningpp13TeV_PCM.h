#include <vector>

#ifndef GAMMACONV_ExtractSignalBinningpp13TeV_PCM
#define GAMMACONV_ExtractSignalBinningpp13TeV_PCM

    #include <iostream>    
    using namespace std;

    //****************************************************************************************************
    //******************** Pt binning for pp, 13TeV *****************************************************
    //****************************************************************************************************


    // ---------------------------------------------------------
    // CombinedBinning- pi0
    // ---------------------------------------------------------

    std::vector<Double_t> fBinsPi013TeV_PCM_CombinedBinning {   // size: 68
        0.00,  0.20, 0.30,  0.40,  0.50,  0.60,        0.70,  0.80,  0.90,  1.00,  1.10,
        1.20,  1.30,  1.40,  1.50,  1.60,        1.70,  1.80,  1.90,  2.00,  2.10,
        2.20,  2.30,  2.40,  2.50,  2.60,        2.70,  2.80,  2.90,  3.00,  3.10,
        3.20,  3.30,  3.40,  3.50,  3.60,        3.70,  3.80,  3.90,  4.00,  4.20,
        4.40,  4.60,  4.80,  5.00,  5.20,        5.40,  5.60,  5.80,  6.00,  6.20,
        6.40,  6.60,  6.80,  7.00,  7.20,        7.40,  7.60,  7.80,  8.00,  8.40,
        9.20,  10.0,  11.0,  12.0,  16.0,        20.0 };

        

   std::vector<Int_t>    fBinsPi013TeV_PCM_Rebin_CombinedBinning { // size: 108
     4, 4, 4, 1, 1, 1,      1, 1, 1, 1, 1,
	1, 1, 1, 1, 1,      1, 1, 1, 1, 1,
	1, 1, 1, 1, 1,      1, 1, 1, 1, 1,
	1, 1, 1, 1, 1,      1, 1, 1, 1, 1,
        1, 1, 1, 1, 1,      1, 1, 1, 1, 1,
	1, 1, 1, 1, 1,      2, 2, 2, 2, 2,
	4, 4, 4, 4, 4};



    // ---------------------------------------------------------
    // CombinedBinning- eta
    // ---------------------------------------------------------



    std::vector<Double_t> fBinsEta13TeV_PCM_CombinedBinning {  //size 29
        0.00, 0.20, 0.40, 0.60, 0.80,   1.00, 1.20, 1.40, 1.60, 1.80, 
        2.00, 2.20, 2.40, 2.60, 2.80,   3.00, 3.40, 3.80, 4.20, 4.60, 
        5.00, 6.00, 7.00, 8.00, 10.0,   12.0, 16.0, 20.0};



    std::vector<Int_t>    fBinsEta13TeV_PCM_Rebin_CombinedBinning { // size: 28
       8,  8,  8,  8, 4,          4,  4,  4,  4,  4,
       4,  4,  4,  4, 4,          4,  4,  4,  4,  4,
       4,  4,  4,  4, 8,          8};




    // ---------------------------------------------------------
    // minimum bias trigger (INT7) - pi0
    // ---------------------------------------------------------



    // PCM
    std::vector<Double_t> fBinsPi013TeVPCMTrigINT7Pt_DPG2019 {      // size: 29, alternative Binning  PCM-PCM 13TeV, DPG2019
        0.00,  0.25,  0.50,  0.75,  1.00,        1.25,  1.50,  1.75,  2.00,  2.25,
        2.50,  2.75,  3.00,  3.50,  4.00,        4.50,  5.00,  5.50,  6.00,  6.50,
        7.00,  8.00, 10.00, 12.00, 16.00,       20.00, 25.00, 30.00, 35.00};
    std::vector<Int_t>    fBinsPi013TeVPCMTrigINT7PtRebin_DPG2019 { // size: 28
        8, 8, 4, 4, 2,      2, 1, 1, 1, 1,
        1, 1, 1, 1, 1,      1, 1, 2, 2, 2,
        4, 4, 4, 8, 8,      8, 8, 8};

    std::vector<Double_t> fBinsEta13TeVPCMMultBinsPt {      // size: 16, Binning PCM 13TeV in multiplicity bins
            0.00,  0.60,  1.00,  1.40,  1.80,        2.20,  2.60,  3.00,  3.40,  3.80,
            4.20,  5.00,  6.00,  8.00,  12.00,       20, 30};

    std::vector<Double_t> fBinsPi013TeVPCMMultBinsPt {      // size: 55, Binning PCM 13TeV in multiplicity bins
            0.00,  0.80,  0.90,  1.00,  1.10,        1.20,  1.30,  1.40,  1.50,  1.60,
            1.70,  1.80,  1.90,  2.00,  2.10,        2.20,  2.30,  2.40,  2.50,  2.60,
            2.70,  2.80,  2.90,  3.00,  3.10,        3.20,  3.30,  3.40,  3.50,  3.60,
            3.70,  3.80,  3.90,  4.00,  4.20,        4.40,  4.60,  4.80,  5.00,  5.20,
            5.40,  5.60,  5.80,  6.00,  6.40,        6.60,  6.80,  7.00,  8.00,  9.00,
            10.00,  12.00,  16.00, 20.00, 30};

    std::vector<Double_t> fBinsPi013TeVPCMTrigINT7Pt {      // size: 85, Standard Binning PCM 13TeV
        0.00,  0.30,  0.40,  0.50,  0.60,        0.70,  0.80,  0.90,  1.00,  1.10,
        1.20,  1.30,  1.40,  1.50,  1.60,        1.70,  1.80,  1.90,  2.00,  2.10,
        2.20,  2.30,  2.40,  2.50,  2.60,        2.70,  2.80,  2.90,  3.00,  3.10,
        3.20,  3.30,  3.40,  3.50,  3.60,        3.70,  3.80,  3.90,  4.00,  4.10,
        4.20,  4.30,  4.40,  4.50,  4.60,        4.70,  4.80,  4.90,  5.00,  5.10,
        5.20,  5.30,  5.40,  5.50,  5.60,        5.70,  5.80,  5.90,  6.00,  6.20,
        6.40,  6.60,  6.80,  7.00,  7.50,        8.00,  8.50,  9.00, 10.00, 12.00,
       16.00, 20.00, 25.00, 30.00, 35.00,       40.00, 45.00, 50.00, 55.00, 60.00,
           65.00, 70.00, 75.00, 80.00, 85.00};

    std::vector<Int_t>    fBinsPi013TeVPCMTrigINT7PtRebin { // size: 84
        4, 4, 1, 1, 1,      1, 1, 1, 1, 1,
        1, 1, 1, 1, 1,      1, 1, 1, 1, 1,
        1, 1, 1, 1, 1,      1, 1, 1, 1, 1,
        1, 1, 1, 1, 1,      1, 1, 1, 1, 1,
        1, 1, 1, 1, 1,      1, 1, 1, 1, 1,
        1, 1, 1, 1, 1,      1, 2, 2, 2, 2,
        2, 2, 2, 2, 2,      4, 4, 4, 4, 4,
        4, 8, 8, 8, 8,      8, 8,16,16,32,
        32,32,32,32};

    // PCM binning fir Pi0 in RBins
    std::vector<Double_t> fBinsPi013TeVPCMTrigINT7RBinsPt {      // size: 20
      0.0, 0.2, 0.3, 0.4, 0.6,   0.8, 1., 1.4, 1.8,  2.2,
      2.6, 3., 4.0,  5.0, 6.0,   7.0, 8.0, 10., 12., 16. };

    std::vector<Int_t>    fBinsPi013TeVPCMTrigINT7RBinsPtRebin { // size: 19
      5, 5, 5 ,4, 4,    4, 4, 4, 4, 4,
      4, 4, 4, 4, 5,    5, 5, 5, 5  };

    // PCM dca binning for pi0
    std::vector<Double_t> fBinsPi013TeVPCMTrigINT7PtDCA { // size: 28
        0.0, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1,
        1.2, 1.3, 1.4, 1.6, 1.8, 2.0, 2.4, 2.8, 3.2, 3.6,
        4.0, 5.0, 6.0, 8.0,12.0,16.0,20.0, 25.0 };



    // ---------------------------------------------------------
    // L0 EMC trigger (EMC7, ~2 GeV) - pi0
    // ---------------------------------------------------------
    // PCM
    std::vector<Double_t> fBinsPi013TeVPCMTrigEMC7Pt {      // size: 65 (Pi0 binning LHC16X EMC7)
        0.0, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1,
        1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0, 2.1,
        2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 2.9, 3.0, 3.2,
        3.4, 3.6, 3.8, 4.0, 4.2, 4.4, 4.6, 4.8, 5.0, 5.4,
        5.8, 6.2, 6.6, 7.0, 7.5, 8.0, 8.5, 9.0, 10., 12.,
        16., 20., 25., 30., 35., 40., 45., 50., 55., 60.,
        65.0, 70.0, 75.0, 80.0, 85.0 };
    std::vector<Int_t>    fBinsPi013TeVPCMTrigEMC7PtRebin { // size: 64
        4, 4, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 4, 4, 4, 4, 4, 4,
        8, 5, 5, 5, 5, 5, 5, 5, 5, 5,
        5, 5, 5, 5 };




    // ---------------------------------------------------------
    // L1 low threshold EMC trigger (EG2, ~3.5 GeV) - pi0
    // ---------------------------------------------------------
    // PCM
    std::vector<Double_t> fBinsPi013TeVPCMTrigEG2Pt {      // size: 96
        0.0, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1,
        1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0, 2.1,
        2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 2.9, 3.0, 3.1,
        3.2, 3.3, 3.4, 3.5, 3.6, 3.7, 3.8, 3.9, 4.0, 4.2,
        4.4, 4.6, 4.8, 5.0, 5.2, 5.4, 5.6, 5.8, 6.0, 6.2,
        6.4, 6.6, 6.8, 7.0, 7.2, 7.4, 7.6, 7.8, 8.0, 8.2,
        8.4, 8.6, 8.8, 9.0, 9.4, 9.8, 10.2, 10.6, 11.0, 11.4,
        11.8, 12.2, 12.6, 13.0, 13.4, 13.8, 14.2, 14.6, 15.0, 16.0,
        17.0, 18.0, 21.0, 25.0, 30.0, 35.0, 40.0, 45.0, 50.0, 55.0,
        60.0, 65.0, 70.0, 75.0, 80.0, 85.0 };
    std::vector<Int_t>    fBinsPi013TeVPCMTrigEG2PtRebin { // size: 95
        4, 8, 4, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 4, 4, 4, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 4 };

    
    // ---------------------------------------------------------
    // L1 high threshold EMC trigger (EG1, ~8 GeV) - pi0
    // ---------------------------------------------------------
    // PCM
    std::vector<Double_t> fBinsPi013TeVPCMTrigEG1Pt { // size: 104 (Pi0 binning LHC16X EG1 8GeV)
        0.0, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1,
        1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0, 2.1,
        2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 2.9, 3.0, 3.1,
        3.2, 3.3, 3.4, 3.5, 3.6, 3.7, 3.8, 3.9, 4.0, 4.2,
        4.4, 4.6, 4.8, 5.0, 5.2, 5.4, 5.6, 5.8, 6.0, 6.2,
        6.4, 6.6, 6.8, 7.0, 7.2, 7.4, 7.6, 7.8, 8.0, 8.2,
        8.4, 8.6, 8.8, 9.0, 9.4, 9.8, 10.2, 10.6, 11.0, 11.4,
        11.8, 12.2, 12.6, 13.0, 13.4, 13.8, 14.2, 14.6, 15.0, 15.4,
        15.8, 16.2, 16.6, 17.0, 17.5, 18.0, 18.5, 19.0, 20.0, 21.0,
        23.0, 25.0, 30.0, 35.0, 40.0, 45.0, 50.0, 55.0, 60.0, 65.0,
        70.0, 75.0, 80.0, 85.0 };
    std::vector<Int_t>    fBinsPi013TeVPCMTrigEG1PtRebin { // size: 103
        4, 8, 4, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 4, 4, 4, 4, 4, 4, 4, 4,
        8, 4, 4, 4, 4, 4, 4, 4, 4, 4,
        4, 4, 4 };
    

    // ---------------------------------------------------------
    // Combined all triggers - pi0
    // ---------------------------------------------------------
    // PCM
    std::vector<Double_t> fBinsPi013TeVPCMTrigCombPt { // size: 104
        0.0, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1,
        1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0, 2.1,
        2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 2.9, 3.0, 3.1,
        3.2, 3.3, 3.4, 3.5, 3.6, 3.7, 3.8, 3.9, 4.0, 4.2,
        4.4, 4.6, 4.8, 5.0, 5.2, 5.4, 5.6, 5.8, 6.0, 6.2,
        6.4, 6.6, 6.8, 7.0, 7.2, 7.4, 7.6, 7.8, 8.0, 8.2,
        8.4, 8.6, 8.8, 9.0, 9.4, 9.8, 10.2, 10.6, 11.0, 11.4,
        11.8, 12.2, 12.6, 13.0, 13.4, 13.8, 14.2, 14.6, 15.0, 15.4,
        15.8, 16.2, 16.6, 17.0, 17.5, 18.0, 18.5, 19.0, 20.0, 21.0,
        23.0, 25.0, 30.0, 35.0, 40.0, 45.0, 50.0, 55.0, 60.0, 65.0,
        70.0, 75.0, 80.0, 85.0 };




    // ---------------------------------------------------------
    // minimum bias trigger (INT7) - eta
    // ---------------------------------------------------------
    // PCM (mode 0)
    std::vector<Double_t> fBinsEta13TeVPCMTrigINT7Pt_DPG2019 {      // size: 11
         0.0,  1.0,  2.5,  4.0,     5.5,  7.0,  9.0,  13.0, 16.0,
        25.0};
     std::vector<Int_t>    fBinsEta13TeVPCMTrigINT7PtRebin_DPG2019 { // size: 10
         16,  8,  4,  4,  4,      8, 16, 16, 16, 16};

    std::vector<Double_t> fBinsEta13TeVPCMTrigINT7Pt {      // size: 50
      0.0, 0.4, 0.6, 0.8,  1.0, 1.2, 1.3, 1.4, 1.5, 1.6,
        1.7, 1.8, 1.9, 2.0, 2.1, 2.2, 2.3, 2.4, 2.5, 2.6,
        2.7, 2.8, 2.9, 3.0, 3.2, 3.4, 3.6, 3.8, 4.0, 4.2,
        4.4, 4.6, 4.8, 5.0, 5.5, 6.0, 7.0, 8.0, 12.0, 16.0,
       25.0,30.0,35.0,40.0,50.0,60.0,70.0,80.0,90.0,100.0};
    std::vector<Int_t>    fBinsEta13TeVPCMTrigINT7PtRebin { // size: 49
      16, 16,16, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
        5, 5, 5, 5, 5, 5, 5, 6, 8, 8,
       16,16,16,16,16,16,16,32,32};

    // PCM binning  in RBins
    std::vector<Double_t> fBinsEta13TeVPCMTrigINT7RBinsPt {      // size: 20
      0.0, 0.2, 0.3, 0.4, 0.6,  0.8, 1., 1.4, 1.8,  2.2,
      2.6, 3., 4.0, 5.0, 6.0,   7.0, 8.0, 10., 12., 16. };

    std::vector<Int_t>    fBinsEta13TeVPCMTrigINT7RBinsPtRebin { // size: 19
      8, 8, 8, 8, 8,   6, 4,  4,  4, 4,
      4, 4, 4, 4, 4,   5, 6, 10, 10 };


    // PCM dca binning
    std::vector<Double_t> fBinsEta13TeVPCMTrigINT7PtDCA { // size: 10
        0.0, 0.4, 0.7, 1.0, 1.5, 2.0, 3.0, 5.0, 7.0, 12.0 };


    // ---------------------------------------------------------
    // L0 EMC trigger (EMC7, ~2 GeV) - eta
    // ---------------------------------------------------------
    // PCM
    std::vector<Double_t> fBinsEta13TeVPCMTrigEMC7Pt {      // size: 23
        0.0, 0.4, 0.8, 1.2, 1.6, 2.0, 2.2, 2.4, 2.6, 2.8,
        3.0, 3.4, 3.8, 4.2, 4.6, 5.0, 5.5, 6.0, 7.0, 8.0,
        12.0, 16.0, 25.0 };
    std::vector<Int_t>    fBinsEta13TeVPCMTrigEMC7PtRebin { // size: 22
        8, 8, 8, 4, 4, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 4, 5, 5, 5, 5, 5,
        8, 8 };


    // ---------------------------------------------------------
    // L1 low threshold EMC trigger (EG2, ~3.5 GeV) - eta
    // ---------------------------------------------------------
    // PCM
    std::vector<Double_t> fBinsEta13TeVPCMTrigEG2Pt {      // size: 23
        0.0, 0.4, 0.8, 1.2, 1.6, 2.0, 2.2, 2.4, 2.6, 2.8,
        3.0, 3.4, 3.8, 4.2, 4.6, 5.0, 5.5, 6.0, 7.0, 8.0,
        12.0, 16.0, 25.0 };
    std::vector<Int_t>    fBinsEta13TeVPCMTrigEG2PtRebin { // size: 22
        16, 8, 8, 4, 4, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 4, 5, 5, 5, 5, 5,
        8, 8 };


    // ---------------------------------------------------------
    // L1 high threshold EMC trigger (EG1, ~8 GeV) - eta
    // ---------------------------------------------------------
    // PCM
    std::vector<Double_t> fBinsEta13TeVPCMTrigEG1Pt {      // size: 23
        0.0, 0.4, 0.8, 1.2, 1.6, 2.0, 2.2, 2.4, 2.6, 2.8,
        3.0, 3.4, 3.8, 4.2, 4.6, 5.0, 5.5, 6.0, 7.0, 8.0,
        12.0, 16.0, 25.0 };
    std::vector<Int_t>    fBinsEta13TeVPCMTrigEG1PtRebin { // size: 22
        8, 8, 8, 4, 4, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 4, 5, 5, 5, 5, 5,
        8, 8 };



    // ---------------------------------------------------------
    // Combined all triggers - eta
    // ---------------------------------------------------------
    // PCM
    std::vector<Double_t> fBinsEta13TeVPCMTrigCombPt { // size: 41
        0.0, 0.4, 0.6, 0.8, 1.0, 1.2, 1.3, 1.4, 1.5, 1.6,
        1.7, 1.8, 1.9, 2.0, 2.1, 2.2, 2.3, 2.4, 2.5, 2.6,
        2.7, 2.8, 2.9, 3.0, 3.2, 3.4, 3.6, 3.8, 4.0, 4.2,
        4.4, 4.6, 4.8, 5.0, 5.5, 6.0, 7.0, 8.0, 12.0, 16.0,
        25.0 };


    // --------------------------------------------------
    // Rebin factors pi0 in eta binning
    // ---------------------------------------------------------
    // PCM
    std::vector<Int_t> fBinsPi0Eta13TeVPtPCMTrigINT7Rebin { // size: 40
        8, 8, 8, 4, 4, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
        5, 5, 5, 5, 5, 5, 5, 6, 8, 8 };


//****************************************************************************************************
    //******************** Pt binning for pp, 13TeV low B ( 0.2T ) **************************************
    //****************************************************************************************************
    std::vector<Double_t> fBinsPi013TeVLowBPtDCA { // 22
        0.0, 0.2, 0.3, 0.4, 0.5, 0.6, 0.8, 1.0, 1.2, 1.4, 1.6, 2.0,
        2.4, 2.8, 3.2, 3.6, 4.0, 5.0, 6.0, 8.0, 10.0, 12.0 };

    std::vector<Double_t> fBinsEta13TeVLowBPCMPt {      // size: 30
        0.0, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0,
        1.1, 1.2, 1.3, 1.4, 1.6, 1.8, 2.0, 2.2, 2.4, 2.6,
        2.8, 3.0, 3.2, 3.6, 4.0, 4.5, 5.0, 6.0, 8.0, 10 };
    std::vector<Int_t> fBinsEta13TeVLowBPCMPtRebin { // size: 29
        16, 8, 8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
        10, 10, 10, 10, 10, 10, 10, 10};

    std::vector<Double_t> fBinsEta13TeVLowBPtDCA{ // size: 5
        0., 0.4, 2., 3., 6.};

#endif
