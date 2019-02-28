#ifndef GAMMACONV_ExtractSignalBinningpp5TeV
#define GAMMACONV_ExtractSignalBinningpp5TeV

    //****************************************************************************************************
    //******************** Pt binning for pp, 5 TeV ******************************************************
    //****************************************************************************************************
    Double_t fBinsPi05TeVPt[27]                     = { 0.0, 0.4, 0.6, 0.8, 1.0, 1.2, 1.4, 1.6, 1.8, 2.0,
                                                        2.2, 2.4, 2.6, 2.8, 3.0, 3.2, 3.4, 3.6, 3.8, 4.0,
                                                        4.5, 5.0, 5.5, 6.0, 7.0, 8.0, 10.0};

    //__________________________________Pi0 PCM binning for 2017 data____________________________________
    Double_t fBinsPi05TeV2017Pt[45]                 = { 0.0, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1,
                                                        1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0, 2.1,
                                                        2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 2.9, 3.0, 3.1,
                                                        3.2, 3.4, 3.6, 3.8, 4.0, 4.5, 5.0, 5.5, 6.0, 7.0,
                                                        8.0, 10., 12., 14., 16.};
    Int_t fBinsPi05TeV2017PtRebin[44]               = {   5,   4,   2,   2,   2,   2,   2,   2,   2,   2,
                                                          2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
                                                          2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
                                                          4,   4,   4,   4,   4,   4,   4,   4,   4,   4,
                                                          5,   5,   5,   5};
    Double_t fBinsPi05TeV2017ExtraFinePt[68]        = { 0.0, 0.3, .35, 0.4, .45, 0.5, .55, 0.6, .65, 0.7,
                                                        .75, 0.8, .85, 0.9, .95, 1., 1.05, 1.1, 1.15, 1.2,
                                                        1.25, 1.3, 1.35, 1.4, 1.45, 1.5, 1.55, 1.6, 1.65, 1.7,
                                                        1.75, 1.8, 1.85, 1.9, 1.95, 2.0, 2.05, 2.1, 2.15, 2.2,
                                                        2.25, 2.3, 2.35, 2.4, 2.45, 2.5, 2.55, 2.6, 2.7, 2.8,
                                                        2.9,  3.0, 3.1,  3.2, 3.4,  3.6, 3.8,  4.0, 4.5, 5.0,
                                                        5.5, 6.0, 7.0, 8.0, 10., 12., 14., 16.};
    Int_t fBinsPi05TeV2017ExtraFinePtRebin[67]      = {   8,   8,   5,   5,   4,   4,   2,   2,   2,   2,
                                                          2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
                                                          2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
                                                          2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
                                                          2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
                                                          2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
                                                          2,   2,   2,   4,   4,   4,   8};
    Double_t fBinsPi05TeV2017PCMCombinationPt[35]   = { 0.0, 0.3, 0.4, 0.6, 0.8, 1.0, 1.2, 1.4, 1.6, 1.8,
                                                        2.0, 2.2, 2.4, 2.6, 2.8, 3.0, 3.2, 3.4, 3.6, 3.8,
                                                        4.0, 4.5, 5.0, 5.5, 6.0, 7.0, 8.0, 9.0, 10., 12.,
                                                        14., 16., 20., 25., 30.};
    Int_t fBinsPi05TeV2017PCMCombinationPtRebin[34] = {   8,   5,   5,   4,   4,   2,   2,   2,   2,   2,
                                                          2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
                                                          2,   2,   2,   2,   2,   2,   2,   4,   4,   5,
                                                          8,   8,   8,   8};
    Double_t fBinsPi05TeV2017PCMforPbPbPt[24]       = { 0.0, 0.4, 0.6, 0.8, 1.0, 1.2, 1.4, 1.6, 1.8, 2.0,
                                                        2.2, 2.4, 2.6, 3.0, 3.5, 4.0, 5.0, 6.0, 8.0, 10.,
                                                        12., 14., 16., 20.};
    Int_t fBinsPi05TeV2017PCMforPbPbPtRebin[23]     = {   8,   8,   5,   5,   4,   4,   2,   2,   2,   2,
                                                          2,   2,   2,   2,   2,   2,   2,   2,   4,   4,
                                                          5,   8,   8};
    Double_t fBinsPi05TeV2017PtDCA[19]              = { 0.0, 0.4, 0.6, 0.8, 1.0, 1.2, 1.4, 1.6, 1.8, 2.0,
                                                        2.2, 2.6, 3.0, 3.5, 4.0, 5.0, 6.0, 8.0, 10.};
    Double_t fBinsDirGamma5TeV2017PCMPt[45]         = { 0.0, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1,
                                                        1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0, 2.1,
                                                        2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 2.9, 3.0, 3.1,
                                                        3.2, 3.4, 3.6, 3.8, 4.0, 4.5, 5.0, 5.5, 6.0, 7.0,
                                                        8.0, 10., 12., 14., 16.};
    Int_t fBinsDirGamma5TeV2017PCMPtRebin[44]       = {   5,   4,   2,   2,   2,   2,   2,   2,   2,   2,
                                                          2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
                                                          2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
                                                          4,   4,   4,   4,   4,   4,   4,   4,   4,   4,
                                                          5,   5,   5,   5};
    //___________________________________________________________________________________________________



    Double_t fBinsPi05TeV2017PtCombination[34]      = { 0.0, 0.4, 0.6, 0.8, 1.0, 1.2, 1.4, 1.6, 1.8, 2.0,
                                                        2.2, 2.4, 2.6, 2.8, 3.0, 3.2, 3.4, 3.6, 3.8, 4.0,
                                                        4.5, 5.0, 5.5, 6.0, 7.0, 8.0, 9.0, 10., 12., 14.,
                                                        16.0, 20., 25., 30.};

    Double_t fBinsPi05TeV2017PtJets[26]             = { 0.0, 0.4, 0.6, 0.8, 1.0, 1.2, 1.4, 1.6, 2.0, 2.4,
                                                        2.8, 3.2, 3.6, 4.0 ,5.0, 6.0, 7.0, 8.0, 9.0, 10.,
                                                        12., 14., 16.0, 20., 25., 30.};

    Double_t fBinsPi05TeVPtDCal[15]                 = { 0.0, 1.0, 1.4, 1.8, 2.2,
                                                        2.6, 3.0, 3.4, 3.8, 4.5,
                                                        6.0, 8., 12., 20.0, 30.0};

    Double_t fBinsPi05TeV2017PtDMC[33]              = { 0.4, 0.6, 0.8, 1.0, 1.2, 1.4, 1.6, 1.8, 2.0, 2.2,
                                                         2.4, 2.6, 2.8, 3.0, 3.2, 3.4, 3.6, 3.8, 4.0, 4.5,
                                                         5.0, 5.5, 6.0, 7.0, 8.0, 9.0, 10.0, 12.0, 14.0, 16.0,
                                                         20.0, 25.0, 30.0};

    Double_t fBinsPi05TeVPtPCMDCal[25]              = { 0.0, 1., 1.2, 1.4, 1.6,
                                                        1.7, 1.8, 1.9, 2.0, 2.1,
                                                        2.2, 2.4, 2.6, 2.8, 3.0,
                                                        3.4, 3.8, 4.1, 4.4, 5.0,
                                                        6.0, 8.0, 12.0, 20.0, 30.0};
    Double_t fBinsPi05TeV2017PtPCMDCal[81]          = { 0.0, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1.0, 1.05, 1.1,
                                                        1.15, 1.2, 1.25, 1.3, 1.35, 1.4, 1.45, 1.5, 1.55, 1.6,
                                                        1.65, 1.7, 1.75, 1.8, 1.85, 1.9, 1.95, 2.0, 2.05, 2.1,  2.15, 2.2, 2.25, 2.3, 2.35, 2.4, 2.45, 2.5, 2.6, 2.7,
                                                        2.8, 2.9, 3.0, 3.1, 3.2, 3.3, 3.4, 3.5, 3.6, 3.7,       3.8, 3.9, 4.0, 4.2, 4.4, 4.6, 4.8, 5.0, 5.2, 5.4,
                                                        5.6, 5.8, 6.0, 6.2, 6.4, 6.6, 6.8, 7.0, 7.5, 8.0,       9.0, 10.0, 12.0, 16.0,20.0, 25.0, 30.0, 40.0,
                                                        45.0, 50.0, 75.0};

    Double_t fBinsPi05TeVPCMEMCPt[35]               = { 0.0, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 1.0, 1.2, 1.4,
                                                        1.6, 1.8, 2.0, 2.2, 2.4, 2.6, 2.8, 3.0, 3.2, 3.4,
                                                        3.6, 3.8, 4.0, 4.5, 5.0, 5.5, 6.0, 7.0, 8.0, 10.0,
                                                        12.0, 16.0, 20.0, 24.0, 30.0};
    Double_t fBinsPi05TeV2017PCMEMCPt[85]          = { 0.0, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1.0, 1.05, 1.1,   1.15, 1.2, 1.25, 1.3, 1.35, 1.4, 1.45, 1.5, 1.55, 1.6,
                                                        1.65, 1.7, 1.75, 1.8, 1.85, 1.9, 1.95, 2.0, 2.05, 2.1,  2.15, 2.2, 2.25, 2.3, 2.35, 2.4, 2.45, 2.5, 2.6, 2.7,
                                                        2.8, 2.9, 3.0, 3.1, 3.2, 3.3, 3.4, 3.5, 3.6, 3.7,       3.8, 3.9, 4.0, 4.2, 4.4, 4.6, 4.8, 5.0, 5.2, 5.4,
                                                        5.6, 5.8, 6.0, 6.2, 6.4, 6.6, 6.8, 7.0, 7.5, 8.0,       8.5, 9.0, 9.5, 10.0, 11.0, 12.0, 14.0, 16.0,
                                                        20.0, 25.0, 30.0, 40.0, 45.0, 50.0, 75.0};
    Double_t fBinsPi05TeV2017PCMPHOSPt[91]          = { 0.0, 0.40, 0.45, 0.50, 0.55, 0.60, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1.0, 1.05, 1.1,   1.15, 1.2, 1.25, 1.3, 1.35, 1.4, 1.45, 1.5, 1.55, 1.6,
                                                        1.65, 1.7, 1.75, 1.8, 1.85, 1.9, 1.95, 2.0, 2.05, 2.1,  2.15, 2.2, 2.25, 2.3, 2.35, 2.4, 2.45, 2.5, 2.6, 2.7,
                                                        2.8, 2.9, 3.0, 3.1, 3.2, 3.3, 3.4, 3.5, 3.6, 3.7,       3.8, 3.9, 4.0, 4.2, 4.4, 4.6, 4.8, 5.0, 5.2, 5.4,
                                                        5.6, 5.8, 6.0, 6.2, 6.4, 6.6, 6.8, 7.0, 7.5, 8.0,       8.5, 9.0, 9.5, 10.0, 11.0, 12.0, 14.0, 16.0,
                                                        20.0, 25.0, 30.0, 40.0, 45.0, 50.0, 75.0};
    Double_t fBinsPi05TeVPtEMC[35]                  = { 0.0, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 1.0, 1.2, 1.4,
                                                        1.6, 1.8, 2.0, 2.2, 2.4, 2.6, 2.8, 3.0, 3.2, 3.4,
                                                        3.6, 3.8, 4.0, 4.5, 5.0, 5.5, 6.0, 7.0, 8.0, 10.0,
                                                        12.0, 16.0, 20.0, 24.0, 30.0};
    Double_t fBinsPi05TeV2017PtEMC[34]             = { 0.0, 0.4, 0.6, 0.8, 1.0, 1.2,    1.4, 1.6, 1.8, 2.0, 2.2,
                                                       2.4, 2.6, 2.8, 3.0, 3.2,    3.4, 3.6, 3.8, 4.0, 4.5,
                                                       5.0, 5.5, 6.0, 7.0, 8.0,    9.0, 10.0, 12.0, 14.0, 16.0,
                                                       20.0, 25.0, 30.0};
    Double_t fBinsPi05TeVPtEMCTrigger1[51]          = { 0.0, 0.3, 0.4, 0.5, 0.6, 0.8, 1.0, 1.2, 1.4, 1.6,
                                                        1.8, 2.0, 2.2, 2.4, 2.6, 2.8, 3.0, 3.2, 3.4, 3.6,
                                                        3.8, 4.0, 4.5, 5.0, 5.5, 6.0, 6.5, 7.0, 7.5, 8.0,
                                                        8.5, 9.0, 9.5, 10., 11., 12., 13., 14., 15., 16.,
                                                        17., 18., 19., 20., 22., 25., 30., 35, 40.};
    Double_t fBinsPi05TeV2017PtmEMC[56]             = { 0.0, 0.4, 0.6, 0.8, 1.0, 1.2, 1.4, 1.6, 1.8, 2.0,
                                                        2.2, 2.4, 2.6, 3.0, 3.5, 4.0, 4.5, 5.0, 5.5, 6.0,
                                                        6.5, 7.0, 7.5, 8.0, 8.5, 9.0, 9.5, 10.0, 11.0, 12.0,
                                                        13.0, 14.0, 15.0, 16.0, 17.0, 18.0, 19.0, 20.0, 22.0, 24.0,
                                                        26.0, 28.0, 30.0, 35.0, 40.0, 45.0, 50.0,
                                                        55.0, 60.0, 70.0, 80.0, 100.0, 125.0, 150.0, 175.0, 200.0};
    Double_t fBinsPi05TeVPtDCA[16]                  = { 0.0, 0.3, 0.4, 0.6, 0.8, 1.0, 1.2, 1.4, 1.6, 1.8,
                                                        2.0, 2.4, 3.0, 5.0, 10., 12.0};
    Double_t fBinsPi05TeV2017DalitzPt[30]           = { 0.0, 0.4, 0.6, 0.8, 1.0, 1.2, 1.4, 1.6, 1.8, 2.0, 2.2, 2.4,
                                                        2.6, 2.8, 3.0, 3.2, 3.4, 3.6, 3.8, 4.0, 4.5, 5.0, 5.5, 6.0,
                                                        8.0, 10.0, 15.0, 20.0, 25.0, 30.0};
    Int_t fBinsPi05TeVPtRebin[26]                   = { 4, 4, 2, 2, 2, 2, 2, 2, 2, 2,
                                                        2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                                                        4, 4, 4, 4 ,4, 4};
    Int_t fBinsPi05TeV2017PtCombinationRebin[33]               = { 5, 5, 4, 2, 2, 2, 2, 2, 2, 2,
                                                        2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                                                        2, 4, 4, 4, 4 ,4, 4, 4, 4, 4,
                                                        4, 5, 5};

    Int_t fBinsPi05TeV2017PtJetsRebin[25]           = { 10, 10, 8, 8, 8, 8, 8, 8, 8, 8,
                                                        8,  8,  8, 8, 8 ,8, 8, 8, 8, 8,
                                                        10, 16, 22, 22, 22};

    Int_t fBinsPi05TeVPtRebinDMC[24]              = { 1, 1, 8, 4, 4,
                                                        4, 4, 4, 4, 4,
                                                        8, 8, 8, 8, 8,
                                                        8, 4, 4, 4, 4,
                                                        4, 4, 8, 16};

    Int_t fBinsPi05TeV2017DMCPtRebin[32]             = { 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
                                                        8, 8, 4, 4, 4, 4, 4, 4, 4, 4,
                                                        8, 8, 8, 8, 8, 8, 8, 16, 16, 16,
                                                        16, 16 };

    Double_t fBinsPi05TeVPtRebinPCMDCal[24]              = { 1, 1, 8, 4, 4,
                                                        4, 4, 4, 4, 4,
                                                        4, 4, 4, 4, 4,
                                                        4, 4, 4, 4, 4,
                                                        4, 4, 8, 16};

    Double_t fBinsPi05TeV2017PtRebinPCMDCal[80]        = { 10, 10, 8, 10, 8, 4, 4, 2, 2, 2,       1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                                                        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,       1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                                                        1, 1, 1, 1, 1, 1, 2, 2, 2, 2,       2, 2, 2, 2, 2, 2, 4, 4,
                                                        4, 4, 4, 4, 4, 4, 4, 4,       5, 5, 5, 5, 10, 10, 10, 10, 10, 10,
                                                        15, 15, 20, 20 };

    Int_t fBinsPi05TeVPCMEMCPtRebin[84]           = { 10, 10, 8, 5, 4, 4, 2, 1, 1, 1,       1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                                                        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,       1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                                                        1, 1, 1, 1, 1, 1, 2, 2, 2, 2,       2, 2, 2, 2, 2, 2, 2, 2, 4, 4,
                                                        4, 4, 4, 4, 4, 4, 4, 4, 4, 4,       5, 5, 5, 5, 10, 10, 10, 10, 10, 10,
                                                        15, 15, 20, 20 };
    Int_t fBinsPi05TeV2017PCMEMCPtRebin[84]        = { 10, 10, 8, 10, 8, 4, 4, 2, 2, 2,       1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                                                        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,       1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                                                        1, 1, 1, 1, 1, 1, 2, 2, 2, 2,       2, 2, 2, 2, 2, 2, 2, 2, 4, 4,
                                                        4, 4, 4, 4, 4, 4, 4, 4, 4, 4,       5, 5, 5, 5, 10, 10, 10, 10, 10, 10,
                                                        15, 15, 20, 20 };
    Int_t fBinsPi05TeV2017PCMEMCCombPtRebin[33]    = {  4, 4, 4, 4, 4, 2, 2, 1, 1, 1,       1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                                                        1, 1, 1, 1, 1, 2, 2, 4, 4, 4,       8, 8, 8};
    Int_t fBinsPi05TeV2017PCMPHOSPtRebin[90]        = { 10, 10, 8, 8, 8, 8, 8, 8, 8, 8, 8, 4, 4, 2, 2, 2,       1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                                                        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,       1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                                                        1, 1, 1, 1, 1, 1, 2, 2, 2, 2,       2, 2, 2, 2, 2, 2, 2, 2, 4, 4,
                                                        4, 4, 4, 4, 4, 4, 4, 4, 4, 4,       5, 5, 5, 5, 10, 10, 10, 10, 10, 10,
                                                        15, 15, 20, 20 };
    Int_t fBinsPi05TeVEMCPtRebin[34]                = { 2, 2, 2, 2, 2, 2, 2, 2, 4, 4,
                                                        2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                                                        2, 2, 2, 2, 4, 4, 4, 4, 4, 8,
                                                        8, 8, 8, 8};
    Int_t fBinsPi05TeV2017EMCPtRebin[74]           = { 5, 4, 4, 4, 4, 4, 2, 2, 2, 2,       2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                                                        2, 2, 2, 2, 2, 2, 2, 2, 2, 2,       2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                                                        2, 2, 2, 4, 4, 4, 4, 4, 4, 4,       4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
                                                        4, 4, 5, 5, 5, 5, 5, 10, 10, 10,    10, 10, 10, 20 };
    Int_t fBinsPi05TeVEMCPtRebinTrigger1[50]        = { 2, 2, 2, 2, 2, 2, 2, 4, 2, 2,
                                                        2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                                                        2, 2, 2, 2, 4, 4, 4, 4, 4, 4,
                                                        4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
                                                        4, 4, 4, 4, 4, 4, 4, 4, 4, 4};
    Int_t fBinsPi05TeV2017mEMCPtRebin[56]           = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                                                        1, 1, 1, 1, 1, 1, 1,
                                                        1, 1, 2, 2, 2, 2, 2, 2, 2, 2,
                                                        2, 2, 2, 2, 4, 4, 4, 4, 4, 4,
                                                        4, 4, 4, 4, 4, 4, 4, 5, 5,
                                                        5, 5, 5, 5, 5, 10,10,10,10};
    Double_t fBinsDirGamma5TeVPt[24]                = { 0.0, 0.3, 0.6, 0.8, 1.0, 1.2, 1.4, 1.6, 1.8, 2.0,
                                                        2.4, 2.8, 3.2, 3.6, 4.2, 4.8, 5.8, 7.0, 8.5, 10.0,
                                                        12.0, 16.0, 20.0, 25.0};
    Int_t fBinsDirGamma5TeVPtRebin[23]              = { 2, 2, 1, 1, 1, 1, 1, 1, 1, 1,
                                                        1, 1, 1, 2, 2, 2, 2, 4, 4, 4,
                                                        5, 5, 5};
    Double_t fBinsDirGamma5TeV2017PCMPtDCA[19]      = { 0.0, 0.4, 0.6, 0.8, 1.0, 1.2, 1.4, 1.6, 1.8, 2.0,
                                                        2.2, 2.6, 3.0, 3.5, 4.0, 5.0, 6.0, 8.0, 10.};

    Int_t fBinsPi05TeV2017DalitzPtRebin[29]         ={ 5, 5, 4, 2, 2, 2, 2, 2, 2, 2,
                                                        2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                                                        2, 4, 4, 4, 4 ,4, 4, 4, 4};
    Double_t fBinsEta5TeVPt[14]                     = { 0.0, 0.5, 1.0, 1.5, 2.0, 3.0, 4.0, 6.0, 8.0, 12.0,
                                                        13.0, 14.0, 20.0, 30.0};
    Int_t fBinsEta5TeVPtRebin[13]                   = { 8, 8, 5, 5, 5, 8, 8, 8, 8, 8,
                                                        10, 10, 10};

    //__________________________________Eta PCM binning for 2017 data____________________________________
    Double_t fBinsEta5TeV2017Pt[25]                 = { 0.0, 0.4, 0.6, 0.8, 1.0, 1.2, 1.4, 1.6, 1.8, 2.0,
                                                        2.2, 2.4, 2.6, 2.8, 3.0, 3.5, 4.0, 5.0, 6.0, 8.0,
                                                        10., 14., 16., 18., 20.};
    Int_t fBinsEta5TeV2017PtRebin[24]               = {   4,   8,   5,   5,   4,   4,   4,   4,   4,   4,
                                                          4,   4,   4,   4,   4,   4,   4,   4,   4,   5,
                                                         10,   8,   8,   8};
    Double_t fBinsEta5TeV2017PCMCombinationPt[15]   = { 0.0, 0.5, 1.0, 1.5, 2.0, 3.0, 4.0, 6.0, 8.0, 10.0,
                                                        12., 14., 16., 18., 20.};
    Int_t fBinsEta5TeV2017PCMCombinationPtRebin[14] = {   4,   4,   2,   2,   2,   2,   2,   2,   2,   4,
                                                          5,   5,   5,   5};
    Double_t fBinsEta5TeV2017PCMforPbPbPt[6]        = { 0.0, 1.0, 2.4, 3.6, 6.0, 10.0};
    Int_t fBinsEta5TeV2017PCMforPbPbPtRebin[5]      = {   8,   8,   8,   8,   8,};
    Double_t fBinsEta5TeV2017PtDCA[19]              = { 0.0, 0.4, 0.6, 0.8, 1.0, 1.2, 1.4, 1.6, 1.8, 2.0,
                                                        2.2, 2.6, 3.0, 3.5, 4.0, 5.0, 6.0, 8.0, 10.};
    //___________________________________________________________________________________________________


    Double_t fBinsEta5TeV2017PtCombination[25]      = { 0.0, 0.4, 0.6, 0.8, 1.2, 1.6, 2.0, 2.4, 2.8, 3.2,
                                                        3.6, 4.0, 4.5, 5.0, 6.0, 7.0, 8.0, 10.0, 12.0, 16.0,
                                                        20.0, 25.0, 30.0, 35.0, 40.0};
    Int_t fBinsEta5TeV2017PtCombinationRebin[24]    = { 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
                                                        8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
                                                        8, 8, 8, 8};
    Double_t fBinsEta5TeV2017PtJets[14]             = { 0.5, 2.0, 3.0, 4.0, 6.0, 8.0, 10.0, 12.0,
                                                        16.0, 20.0, 25.0, 30.0, 35.0, 40.0};
    Int_t fBinsEta5TeV2017PtJetsRebin[13]           = {12, 12, 12, 12, 20, 20, 20, 25, 22, 22,
                                                        22, 22, 22};
    Double_t fBinsEta5TeVPCMEMCPt[19]               = { 0., 0.3, 0.5, 0.7, 0.9, 1.0, 1.5, 2.0,
                                                        3.0, 4.,  6., 8.,  10,  14., 16., 20.,
                                                        25., 30., 35.};
    Int_t fBinsEta5TeVPCMEMCPtRebin[22]             = { 5, 8, 10, 10, 8, 8, 8, 8, 8, 8,
                                                        8, 8, 8, 8, 10, 10, 16, 5, 5, 5, 5, 5};
    Double_t fBinsEta5TeV2017PCMEMCPt[32]           = { 0., 1.0, 1.1, 1.2, 1.4, 1.6, 1.8, 2.0, 2.2, 2.4,
                                                       2.6, 2.8, 3.0, 3.2, 3.4, 3.6, 3.8, 4.0, 4.5, 5.0,
                                                       5.5, 6.0, 7.0, 8.0, 9.0,10.0,12.0,16.0,20.0,25.0,
                                                       30.0, 40.0};
    Int_t fBinsEta5TeV2017PCMEMCPtRebin[31]         = { 10,10, 8, 8, 5, 4, 4, 4, 4, 4,
                                                         4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
                                                         8, 8, 8, 8, 8,10,10,16,16,20,
                                                         20};
    Int_t fBinsEta5TeV2017PCMEMCCombPtRebin[24]     = { 10,10, 8, 8, 5, 4, 4, 4, 4, 4,
                                                         4, 4, 4, 4, 4, 8, 8, 8, 16, 16,
                                                         8, 8, 8, 8};
    Double_t fBinsEta5TeV2017PCMDCalPt[30]           = { 0., 1.0, 1.2, 1.4, 1.6, 1.8, 2.0, 2.2, 2.4,
                                                       2.6, 2.8, 3.0, 3.2, 3.4, 3.6, 3.8, 4.0, 4.5, 5.0,
                                                       5.5, 6.0, 7.0, 8.0, 10.0,12.0,16.0,20.0,25.0,
                                                       30.0, 40.0};
    Double_t fBinsEta5TeV2017DMCPt[12]                   = { 0.5, 1.0, 1.5, 2.0, 3.0, 4.0, 6.0, 8.0, 10.0, 12.0,
                                                         16.0, 20.0};
    Int_t fBinsEta5TeV2017PCMDCalPtRebin[29]         = { 10,10, 8, 8, 8, 8, 8, 5, 5, 5,
                                                         5, 5, 5, 5, 5, 5, 5, 5, 5, 8,
                                                         8, 10, 10, 16,10,10,16,16,20 };
    Double_t fBinsEta5TeV2017DalitzPt[11]           = { 0., 0.6, 1.0, 1.4, 1.8, 2.2, 2.8, 4., 6., 10., 14.};
    Int_t fBinsEta5TeV2017DalitzPtRebin[10]          = { 20, 20, 20, 20, 20, 20, 20, 20, 20, 20};
    Double_t fBinsEta5TeVEMCPt[23]                  = { 0., 0.3, 0.5, 0.7, 0.9, 1.1, 1.4, 1.8, 2.2, 2.6,
                                                        3.0, 3.5, 4.,  5.,  6., 8.,  10,  12., 16., 20.,
                                                        25., 30., 35.};
    Int_t fBinsEta5TeVEMCPtRebin[22]                = { 8, 8, 8, 8, 8, 8, 8, 16, 10, 10,
                                                        8, 8, 8, 8, 8, 8, 10, 10, 8, 8, 8, 8};
    Double_t fBinsEta5TeV2017EMCPt[30]             = { 0., 1.4, 1.6, 1.8, 2.0, 2.2, 2.4, 2.6, 2.8, 3.0,    3.2, 3.4, 3.6, 3.8, 4.0, 4.5, 5.0, 5.5, 6.0, 7.0,
                                                        8.0, 9.0, 10.0, 12.0, 14.0, 16.0, 20.0, 25.0, 30.0, 40.0};
    Int_t fBinsEta5TeV2017EMCPtRebin[29]           = { 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,        10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
                                                        10, 10, 10, 10, 16, 16, 16, 20, 20};
    Int_t fBinsEta5TeV2017DMCPtRebin[12]           = { 40, 40, 40, 40, 20, 20, 20, 40, 40, 40,
                                                       20, 20};
    Double_t fBinsEta5TeVEMCPtTrigger1[30]          = { 0.0, 0.4, 0.8, 1.2, 1.6, 2.0, 2.4, 2.8, 3.2, 3.6,
                                                        4.0, 5.0, 6.0, 7.0, 8., 9., 10., 11., 12., 13.,
                                                        14., 15., 16., 17., 18., 20., 25., 30., 35.,40.};
    Int_t fBinsEta5TeVEMCPtRebinTrigger1[29]        = { 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
                                                        8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
                                                        8, 8, 8, 8, 8, 8, 8, 8, 8};
    Double_t fBinsEta5TeVPtDCA[9]                   = { 0.0, 0.4, 1.0, 1.5, 2.0, 3.0, 5.0, 7.0, 12.0};
    Double_t fBinsEtaPrim5TeVPt[8]                  = { 0.0, 0.5, 1.0, 2.0, 3.0, 4.0, 6.0, 10.0};
    Int_t fBinsEtaPrim5TeVPtRebin[7]                = { 8, 2, 2, 2, 2, 2, 2};
    Int_t fBinsPi0EtaBinning5TeVPtRebin[13]         = { 8, 2, 2, 2, 2, 2, 2, 4, 4, 4,
                                                        4, 4, 4 };
    Int_t fBinsPi0EtaBinning5TeVPtRebinEMCTrigger1[29]  = { 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
                                                            5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
                                                            5, 5, 5, 5, 5, 5, 5, 5, 5};
    Int_t fBinsPi0EtaBinning5TeVPCMEMCPtRebin[24]   = { 8, 2, 2, 2, 2, 2, 2, 4, 4, 4,
                                                        8, 2, 2, 2, 2, 2, 2, 4, 4, 4,
                                                        4, 4, 4 };
    Int_t fBinsPi0EtaBinning5TeV2017DalitzPtRebin[9]    = { 8, 2, 2, 2, 2, 2, 4, 4, 4};

#endif