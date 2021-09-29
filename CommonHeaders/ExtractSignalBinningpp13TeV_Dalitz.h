#include <vector>

#ifndef GAMMACONV_ExtractSignalBinningpp13TeV_Dalitz
#define GAMMACONV_ExtractSignalBinningpp13TeV_Dalitz

    #include <iostream>    
    using namespace std;

    //****************************************************************************************************
    //******************** Pt binning for pp, 13TeV *****************************************************
    //****************************************************************************************************



    // PCM-Dal
    std::vector<Double_t> fBinsPi013TeVDalitzPt {// size: 32;
        0.00, 0.30, 0.40, 0.60, 0.80, 1.00, 1.20, 1.40, 1.60, 1.80, 2.00,
        2.20, 2.40, 2.60, 2.80, 3.00, 3.20, 3.40, 3.60, 3.80, 4.00, 4.50,
        5.00, 5.50, 6.00, 7.00, 8.00, 9.00, 10.00, 12.00, 16.00, 20.00};
    std::vector<Int_t>    fBinsPi013TeVDalitzPtRebin {// size: 31
        3, 1, 2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        5, 5, 5, 5, 10, 10, 10, 10, 20,
        40, 40};
    // PCM-Dal-RBins
    std::vector<Double_t> fBinsPi013TeVRBinsDalitzPt {// size: 19
        0.0, 0.3, 0.4, 0.6, 0.8, 1., 1.4, 1.8,  2.2, 2.6, 3., 4.0,
        5.0, 6.0, 7.0, 8.0, 10., 12., 16. };
    std::vector<Int_t>    fBinsPi013TeVRBinsDalitzPtRebin {// size: 18
        3, 1, 2, 2, 2, 4, 4, 4, 4, 4,
        10, 10, 10, 10, 10, 20, 20, 40};


    // ---------------------------------------------------------
    // minimum bias trigger (INT7) - eta
    // ---------------------------------------------------------

    // PCM-Dal (mode 1)
    std::vector<Double_t> fBinsEta13TeVDalitzTrigINT7Pt {      // size: 10
        0., 0.6, 1.0, 1.4, 1.8, 2.2, 2.8, 4.4, 6., 10.};
    std::vector<Int_t>    fBinsEta13TeVDalitzTrigINT7PtRebin { // size: 9
        10, 10, 10, 10, 10, 10, 10, 10, 10 };

    // --------------------------------------------------
    // Rebin factors pi0 in eta binning
    // ---------------------------------------------------------
    // PCM-Dal
    std::vector<Int_t> fBinsPi0Eta13TeVDalitzPtRebin {      // size: 9
        8, 2, 2, 2, 2, 2, 4, 4, 4 };

    
#endif