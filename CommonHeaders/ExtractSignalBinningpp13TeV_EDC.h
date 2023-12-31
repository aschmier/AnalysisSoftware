#include <vector>

#ifndef GAMMACONV_ExtractSignalBinningpp13TeV_EDC
#define GAMMACONV_ExtractSignalBinningpp13TeV_EDC

    #include <iostream>    
    using namespace std;

    //****************************************************************************************************
    //******************** Pt binning for pp, 13TeV *****************************************************
    //****************************************************************************************************



    // EMC
    std::vector<Double_t> fBinsPi013TeVEMCTrigINT7Pt {  // size: 69
         0.00,  1.30,  1.40,  1.50,  1.60,        1.70,  1.80,  1.90,  2.00,  2.10,
       2.20,  2.30,  2.40,  2.50,  2.60,        2.70,  2.80,  2.90,  3.00,  3.10,
       3.20,  3.30,  3.40,  3.50,  3.60,        3.70,  3.80,  3.90,  4.00,  4.20,
       4.40,  4.60,  4.80,  5.00,  5.20,        5.40,  5.60,  5.80,  6.00,  6.20,
       6.40,  6.60,  6.80,  7.00,  7.20,        7.40,  7.60,  7.80,  8.00,  8.40,
	     8.80,  9.20,  9.60,  10.0,  10.5,        11.0,  11.5,  12.0,  12.5,  13.0,
	     13.5,  14.0,  15.0,  16.0,  17.0,  	    18.0,  19.0,  20.0,  21.0,  22.0,
       24.0,  26.0,  28.0,  30.0,  35.0,        40.0};
    std::vector<Int_t>    fBinsPi013TeVEMCTrigINT7PtRebin { // size: 68 or 75 if you want to go beyond 20 GeV (which is a bit experimental for min bias)
       4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
       4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
       4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
       4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
       4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
       4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
       4, 4, 4, 4, 4, 4, 8, 8, 8, 8,
       8, 8, 8, 8, 8};

    // EMC HighMultiplicity
    std::vector<Double_t> fBinsPi013TeVEMCTrigHMPt {  // size: 15
	     0.00,  1.30,  1.50,  2.00, 3.00,      4.00, 5.00, 6.00, 7.00, 8.00,
       10.0,  12.0,  14.0,  16.0, 20.0};
    std::vector<Int_t>    fBinsPi013TeVEMCTrigHMPtRebin { // size = 14
       4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
       4, 4, 4, 4};

    // EMC Multiplicity
    std::vector<Double_t> fBinsPi013TeVEMCTrigINT7MultPt {  // size: 37
      0.00,  1.30,  1.40,  1.60,  1.80,        2.00,  2.20,  2.40,  2.60,  2.80,
      3.00,  3.20,  3.40,  3.60,  3.80,        4.00,  4.40,  4.80,  5.20,  5.60,
      6.00,  6.50,  7.00,  7.50,  8.00,        9.00,  10.0,  11.0,  12.0,  13.0,
      14.0,  16.0,  18.0,  20.0,  22.0,        24.0,  30.0};

    std::vector<Int_t>    fBinsPi013TeVEMCTrigINT7MultPtRebin { // size = 36
      4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
      4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
      4, 4, 4, 4, 4, 5, 5, 5, 5, 5,
      8, 8, 8, 8, 8, 8};


    std::vector<Double_t> fBinsGamma13TeVEMCTrigINT7Pt {  // size: 75
       0.00,  0.70,  0.80,  0.90,  1.00,        1.10,  1.20,  1.30,  1.40,  1.50,
       1.60,  1.70,  1.80,  1.90,  2.00,        2.10,  2.20,  2.30,  2.40,  2.50,
       2.60,  2.70,  2.80,  2.90,  3.00,        3.10,  3.20,  3.30,  3.40,  3.50,
       3.60,  3.70,  3.80,  3.90,  4.00,        4.20,  4.40,  4.60,  4.80,  5.00,
       5.20,  5.40,  5.60,  5.80,  6.00,        6.20,  6.40,  6.60,  6.80,  7.00,
       7.20,  7.40,  7.60,  7.80,  8.00,        8.40,  8.80,  9.20,  9.60,  10.0,
       10.5,  11.0,  12.0,  13.0,  14.0,        16.0,  18.0,  20.0,  25.0,  30.0,
       35.0,  40.0,  50.0,  75.0,  100.};
    std::vector<Int_t>    fBinsGamma13TeVEMCTrigINT7PtRebin { // size: 68 or 75 if you want to go beyond 20 GeV (which is a bit experimental for min bias)
       4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
       4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
       4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
       4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
       4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
       4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
       4, 4, 4, 4, 4, 4, 8, 8, 8, 8,
       8, 8, 8, 8, 8};


    // ---------------------------------------------------------
    // L0 EMC trigger (EMC7, ~2 GeV) - pi0
    // ---------------------------------------------------------

    std::vector<Double_t> fBinsPi013TeVEMCTrigEMC7Pt {      // size: 128
        0.0, 3.0, 3.1, 3.2, 3.3, 3.4, 3.5, 3.6, 3.7, 3.8,
        3.9, 4.0, 4.1, 4.2, 4.3, 4.4, 4.5, 4.6, 4.7, 4.8,
        4.9, 5.0, 5.1, 5.2, 5.3, 5.4, 5.5, 5.6, 5.7, 5.8,
        5.9, 6.0, 6.1, 6.2, 6.3, 6.4, 6.5, 6.6, 6.7, 6.8,
        6.9, 7.0, 7.1, 7.2, 7.3, 7.4, 7.5, 7.6, 7.7, 7.8,
        7.9, 8.0, 8.1, 8.2, 8.3, 8.4, 8.5, 8.6, 8.7, 8.8,
        8.9, 9.0, 9.1, 9.2, 9.3, 9.4, 9.5, 9.6, 9.7, 9.8,
        9.9, 10.0, 10.1, 10.2, 10.3, 10.4, 10.5, 10.6, 10.7, 10.8,
        10.9, 11.0, 11.1, 11.2, 11.3, 11.4, 11.5, 11.6, 11.7, 11.8,
        11.9, 12.0, 12.1, 12.2, 12.3, 12.4, 12.5, 12.6, 12.7, 12.8,
        12.9, 13.0, 13.1, 13.2, 13.3, 13.4, 13.5, 13.6, 13.7, 13.8,
        13.9, 14.0, 14.2, 14.4, 14.6, 14.8, 15.0, 15.2, 15.4, 15.6,
        15.8, 16.0, 16.5, 17.0, 17.5, 18.0, 19.0, 20.0 };
    std::vector<Int_t>    fBinsPi013TeVEMCTrigEMC7PtRebin { // size: 127
        4, 4, 4, 4, 4, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
        5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 10, 10, 10, 10, 10 };



    // ---------------------------------------------------------
    // L1 low threshold EMC trigger (EG2, ~3.5 GeV) - pi0
    // ---------------------------------------------------------

 // EMC
    std::vector<Double_t> fBinsPi013TeVEMCTrigEG2Pt {      // size: 44
        0.00,  4.60,  4.80,  5.00,  5.20,    5.40,  5.60,  5.80,  6.00,  6.20,
        6.40,  6.60,  6.80,  7.00,  7.20,    7.40,  7.60,  7.80,  8.00,  8.40,
        8.80,  9.20,  9.60,  10.0,  10.5,    11.0,  11.5,  12.0,  12.5,  13.0,
        13.5,  14.0,  15.0,  16.0,  17.0,    18.0,  19.0,  20.0,  21.0,  22.0,
        23.0,  24.0,  25.0,  26.0,  28.0,    30.0,  35.0,  40.0,  50.0};
    std::vector<Int_t>    fBinsPi013TeVEMCTrigEG2PtRebin { // size: 43
        2, 2, 2, 2, 2,    2, 2, 2, 2, 2,
        2, 2, 2, 2, 2,    2, 2, 2, 2, 2,
        2, 2, 4, 4, 4,    4, 4, 4, 4, 4,
        4, 4, 4, 4, 4,    4, 4, 4, 4, 4,
        8, 8, 8, 8, 8,    8, 8, 8};

      std::vector<Double_t> fBinsPi013TeVEMCTrigEG2MultPt {      // size: 21
        0.00,  4.80,  5.20, 5.60,  6.00,      6.50,  7.00,  7.50,  8.00,  9.00,
        10.0,  11.0,  12.0, 13.0,  14.0,      16.0,  18.0,  20.0,  22.0,  24.0,
        30.0};

      std::vector<Int_t>    fBinsPi013TeVEMCTrigEG2MultPtRebin { // size: 20
        2, 2, 4, 4, 4,    4, 4, 4, 4, 4,
        4, 4, 4, 4, 4,    4, 4, 4, 8, 8};

    // ---------------------------------------------------------
    // L1 high threshold EMC trigger (EG1, ~8 GeV) - pi0
    // ---------------------------------------------------------
    
    // EMC
  std::vector<Double_t> fBinsPi013TeVEMCTrigEG1Pt { // size: 29
       0.00,  9.20,  9.60,  10.0,  10.5,        11.0,  11.5,  12.0,  12.5,  13.0,
       13.5,  14.0,  15.0,  16.0,  17.0,        18.0,  19.0,  20.0,  21.0,  22.0,
       23.0,  24.0,  25.0,  26.0,  28.0,        30.0,  35.0,  40.0,  50.0};
  std::vector<Int_t>    fBinsPi013TeVEMCTrigEG1PtRebin { // size: 28
       2, 2, 2, 2, 2,    2, 2, 2, 2, 2,
       2, 2, 2, 2, 4,    4, 4, 4, 4, 4,
       4, 4, 4, 4, 4,    4, 4, 4};

   std::vector<Double_t> fBinsPi013TeVEMCTrigEG1MultPt { // size: 11
       10.0,  11.0,  12.0, 13.0,  14.0,      16.0,  18.0,  20.0,  22.0,  24.0,
       30.0};

   std::vector<Int_t>    fBinsPi013TeVEMCTrigEG1MultPtRebin { // size: 10
       2, 2, 2, 2, 4,    4, 4, 4, 4, 4};





    // ---------------------------------------------------------
    // Combined all triggers - pi0
    // ---------------------------------------------------------

    // EMC
    std::vector<Double_t> fBinsPi013TeVEMCTrigCombPt { // size: 74
      0.00,  1.30,  1.40,  1.50,  1.60,        1.70,  1.80,  1.90,  2.00,  2.10,
      2.20,  2.30,  2.40,  2.50,  2.60,        2.70,  2.80,  2.90,  3.00,  3.10,
      3.20,  3.30,  3.40,  3.50,  3.60,        3.70,  3.80,  3.90,  4.00,  4.20,
      4.40,  4.60,  4.80,  5.00,  5.20,        5.40,  5.60,  5.80,  6.00,  6.20,
      6.40,  6.60,  6.80,  7.00,  7.20,        7.40,  7.60,  7.80,  8.00,  8.40,
      8.80,  9.20,  9.60,  10.0,  10.5,        11.0,  11.5,  12.0,  12.5,  13.0,
      13.5,  14.0,  15.0,  16.0,  17.0,        18.0,  19.0,  20.0,  21.0,  22.0,
      23.0,  24.0,  25.0,  26.0};



    // ---------------------------------------------------------
    // minimum bias trigger (INT7) - eta
    // ---------------------------------------------------------

    // EMC (mode 4)
    std::vector<Double_t> fBinsEta13TeVEMCTrigINT7Pt {      // size: 19
      0.00, 2.40, 2.60, 2.80, 3.00,  3.40, 3.80, 4.20, 4.60, 5.00,
      6.00, 7.00, 8.00, 10.0, 12.0,  14.0, 16.0, 20.0, 24.0
      };
    std::vector<Int_t>    fBinsEta13TeVEMCTrigINT7PtRebin { // size: 18
      8, 8, 8, 8, 8,    8, 8, 8, 8, 8,
      8, 8, 8, 8, 8,    12,12, 20};

    std::vector<Double_t> fBinsEta13TeVEMCTrigINT7MultPt {  // size: 15
      0.00, 2.40, 2.70, 3.00, 3.50,     4.00, 5.00, 6.00, 8.00, 10.0,
      12.0, 14.0, 16.0, 20.0, 25.0
    };
    std::vector<Int_t>    fBinsEta13TeVEMCTrigINT7MultPtRebin { // size = 14
      8, 8, 8, 8, 8,    8, 8, 8, 8, 8,
      8, 12,12, 20};


    // ---------------------------------------------------------
    // L1 low threshold EMC trigger (EG2, ~3.5 GeV) - eta
    // ---------------------------------------------------------
    
    // EMC
    std::vector<Double_t> fBinsEta13TeVEMCTrigEG2Pt {   // size: 19
        0.00, 5.00, 6.00, 7.00, 8.00, 10.0, 12.0, 14.0, 16.0, 18.0,
        20.0, 22.0, 24.0, 26.0, 30.0, 35.0, 40.0, 50.0, 60.0};
    std::vector<Int_t>    fBinsEta13TeVEMCTrigEG2PtRebin { // size: 18
        2, 2, 2, 4, 4, 4, 4, 4, 4, 4,
        4, 4, 8, 8, 8, 10, 10, 10 };


    std::vector<Double_t> fBinsEta13TeVEMCTrigEG2MultPt {   // size: 15
        0.00, 5.00, 6.00, 8.00, 10.0,     12.0, 14.0, 16.0, 20.0, 25.0,
        30.0, 35.0, 40.0, 50.0, 60.0};
    std::vector<Int_t>    fBinsEta13TeVEMCTrigEG2MultPtRebin { // size: 14
        2, 2, 2, 4, 4,      4, 4, 8, 8, 8,
        8, 10, 10, 10 };


    // ---------------------------------------------------------
    // L1 high threshold EMC trigger (EG1, ~8 GeV) - eta
    // ---------------------------------------------------------

    // EMC
    std::vector<Double_t> fBinsEta13TeVEMCTrigEG1Pt { // size: 14
      0.00, 12.0, 14.0, 16.0, 18.0, 20.0, 22.0, 24.0, 26.0, 30.0,
      35.0, 40.0, 50.0, 60.0};
    std::vector<Int_t>    fBinsEta13TeVEMCTrigEG1PtRebin { // size: 13
      4, 4, 4, 4, 4, 4, 8, 8, 8, 10,
      10, 10, 10 };

    std::vector<Double_t> fBinsEta13TeVEMCTrigEG1MultPt {   // size: 15
      0.00, 5.00, 6.00, 8.00, 10.0,     12.0, 14.0, 16.0, 20.0, 25.0,
      30.0, 35.0, 40.0, 50.0, 60.0};
    std::vector<Int_t>    fBinsEta13TeVEMCTrigEG1MultPtRebin { // size: 14
      2, 2, 2, 4, 4,      4, 4, 8, 8, 8,
      8, 10, 10, 10 };


    // ---------------------------------------------------------
    // Combined all triggers - eta
    // ---------------------------------------------------------

    // EMC
    std::vector<Double_t> fBinsEta13TeVEMCTrigCombPt { // size: 38
        0.00, 2.20, 2.40, 2.60, 2.80, 3.00,  3.40, 3.80, 4.20, 4.60,
        5.00, 6.00, 7.00, 8.00, 10.0, 12.0,  14.0, 16.0, 18.0, 20.0,
        22.0, 24.0, 26.0, 30.0,  35.0, 40.0, 50.0, 60.0};


    // --------------------------------------------------
    // Rebin factors pi0 in eta binning


    // EMC
    std::vector<Int_t> fBinsPi0Eta13TeVPtEMCTrigINT7Rebin { // size: 26
      8, 8, 8, 4, 4, 4, 4, 4, 4, 4,
      4, 4, 4, 4, 4, 5, 5, 5, 5, 5,
      8, 8 };
    std::vector<Int_t> fBinsPi0Eta13TeVEMCTrigEG2PtRebin { // size: 29
      8, 8, 8, 4, 4, 4, 4, 4, 4, 4,
      4, 4, 4, 4, 4, 5, 5, 5, 5, 5,
      8, 8 };
    std::vector<Int_t> fBinsPi0Eta13TeVEMCTrigEG1PtRebin { // size: 29
      8, 8, 8, 4, 4, 4, 4, 4, 4, 4,
      4, 4, 4, 4, 4, 5, 5, 5, 5, 5,
      8, 8 };

          //****************************************************************************************************
    //******************** Pt binning for pp, 13TeV low B ( 0.2T ) **************************************
    //****************************************************************************************************
    std::vector<Double_t> fBinsPi013TeVLowBEMCPt { // 35
        0.00, 1.40, 1.50, 1.60, 1.70, 1.80, 1.90, 2.00, 2.10, 2.20,
        2.30, 2.40, 2.50, 2.60, 2.70, 2.80, 3.00, 3.20, 3.40, 3.60,
        3.80, 4.00, 4.20, 4.40, 4.60, 4.80, 5.00, 5.50, 6.00, 6.50,
        7.00, 8.00, 10.0, 12.5, 15.0};
    std::vector<Int_t> fBinsPi013TeVLowBEMCPtRebin{ // 34
        4, 4, 4, 4, 4, 4, 4, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 4,
        4, 4, 4, 8};
#endif