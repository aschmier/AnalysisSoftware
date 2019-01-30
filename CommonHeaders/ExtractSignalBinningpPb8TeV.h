#ifndef GAMMACONV_ExtractSignalBinningpPb8TeV
#define GAMMACONV_ExtractSignalBinningpPb8TeV

    //****************************************************************************************************
    //****************** Pt binning for pPb, 8 TeV *******************************************************
    //****************************************************************************************************
    Double_t fBinsPi0pPb8TeVPt[41]                  = { 0.0, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 1.0, 1.2, 1.4,
                                                        1.6, 1.8, 2.0, 2.2, 2.4, 2.6, 2.8, 3.0, 3.2, 3.4,
                                                        3.6, 3.8, 4.0, 4.5, 5.0, 5.5, 6.0, 7.0, 8.0, 9.0, 10.0,
                                                        12.0, 16.0, 18.0, 20.0, 22.0, 24.0, 26.0, 30.0, 35.0, 40.0};
    Double_t fBinsPi0pPb8TeVPtmEMC[60]              = { 0.0, 0.4, 0.6, 0.8, 1.0, 1.2, 1.4, 1.6, 1.8, 2.0,
                                                        2.2, 2.4, 2.6, 3.0, 3.5, 4.0, 4.5, 5.0, 5.5, 6.0,
                                                        6.5, 7.0, 7.5, 8.0, 8.5, 9.0, 9.5, 10.0, 11.0, 12.0,
                                                        13.0, 14.0, 15.0, 16.0, 17.0, 18.0, 19.0, 20.0, 22.0, 24.0,
                                                        26.0, 28.0, 30.0, 32.0, 34.0, 36.0, 38.0, 40.0, 45.0, 50.0,
                                                        55.0, 60.0, 65.0, 70.0, 80.0, 100.0, 125.0, 150.0, 175.0, 200.0};
    Double_t fBinsPi0pPb8TeVEMCPt[33]               = { 0.0, 1.2, 1.4,
                                                        1.6, 1.8, 2.0, 2.2, 2.4, 2.6, 2.8, 3.0, 3.2, 3.4,
                                                        3.6, 3.8, 4.0, 4.5, 5.0, 5.5, 6.0, 7.0, 8.0, 9.0, 10.0,
                                                        11.0, 12.0, 14.0, 16.0, 20.0, 25.0, 30.0, 35.0, 40.0};
    Double_t fBinsPi0pPb8TeVEMCTrigEG1Pt[39]        = { 0.0, 1.2, 1.4,
                                                        1.6, 1.8, 2.0, 2.2, 2.4, 2.6, 2.8, 3.0, 3.2, 3.4,
                                                        3.6, 3.8, 4.0, 4.5, 5.0, 5.5, 6.0, 7.0, 8.0, 9.0, 10.0,
                                                        11.0, 12.0,
                                                        13.0, 14.0, 15.0, 16.0, 17.0, 18.0, 19.0, 20.0, 22.0, 24.0,
                                                        26.0, 28.0, 30.0};
    Double_t fBinsPi0pPb8TeVEMCTrigEG2Pt[38]        = { 0.0, 1.2, 1.4,
                                                        1.6, 1.8, 2.0, 2.2, 2.4, 2.6, 2.8, 3.0, 3.2, 3.4,
                                                        3.6, 3.8, 4.0, 4.5, 5.0, 5.5, 6.0, 7.0, 8.0, 9.0, 10.0,
                                                        11.0, 12.0,
                                                        13.0, 14.0, 15.0, 16.0, 17.0, 18.0, 20.0, 22.0, 24.0,
                                                        26.0, 28.0, 30.0};
    Double_t fBinsPi0pPb8TeVEMCTrigCombinePt[39]     = { 0.0, 1.2, 1.4,
                                                        1.6, 1.8, 2.0, 2.2, 2.4, 2.6, 2.8, 3.0, 3.2, 3.4,
                                                        3.6, 3.8, 4.0, 4.5, 5.0, 5.5, 6.0, 7.0, 8.0, 9.0, 10.0,
                                                        11.0, 12.0,
                                                        13.0, 14.0, 15.0, 16.0, 17.0, 18.0, 19.0, 20.0, 22.0, 24.0,
                                                        26.0, 28.0, 30.0};
    Double_t fBinsPi0pPb8TeVPCMEMCPt[33]            = { 0.0, 0.8, 1.0, 1.2, 1.4,
                                                        1.6, 1.8, 2.0, 2.2, 2.4, 2.6, 2.8, 3.0, 3.2, 3.4,
                                                        3.6, 3.8, 4.0, 4.5, 5.0, 5.5, 6.0, 7.0, 8.0, 9.0, 10.0,
                                                        12.0, 16.0, 20.0, 25.0, 30.0, 35.0, 40.0};
    Double_t fBinsPi0pPb8TeVPCMEMCTrigEG2Pt[39]     = { 0.0, 0.8, 1.0, 1.2, 1.4,
                                                        1.6, 1.8, 2.0, 2.2, 2.4, 2.6, 2.8, 3.0, 3.2, 3.4,
                                                        3.6, 3.8, 4.0, 4.5, 5.0, 5.5, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0,
                                                        12.0, 16.0, 20.0, 25.0, 30.0, 40.0,
                                                        45.0, 50.0, 60.0, 70.0, 90.0, 100.0};
    Double_t fBinsPi0pPb8TeVPCMEMCTrigEG1Pt[47]     = { 0.0, 0.8, 1.0, 1.2, 1.4,
                                                        1.6, 1.8, 2.0, 2.2, 2.4, 2.6, 2.8, 3.0, 3.2, 3.4,
                                                        3.6, 3.8, 4.0, 4.5, 5.0, 5.5, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0,
                                                        12.0, 13.0, 14.0, 15., 16.0, 17., 18.,19., 20.0, 22.0, 24.0,
                                                        26.0, 30.0, 35.0,
                                                        40.0, 50.0, 60.0, 70.0, 90.0, 100.0};
    Double_t fBinsPi0pPb8TeVPCMEMCTrigCombinePt[47]     = { 0.0, 0.8, 1.0, 1.2, 1.4,
                                                        1.6, 1.8, 2.0, 2.2, 2.4, 2.6, 2.8, 3.0, 3.2, 3.4,
                                                        3.6, 3.8, 4.0, 4.5, 5.0, 5.5, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0,
                                                        12.0, 13.0, 14.0, 15., 16.0, 17., 18.,19., 20.0, 22.0, 24.0,
                                                        26.0, 30.0, 35.0,
                                                        40.0, 50.0, 60.0, 70.0, 90.0, 100.0};
    Double_t fBinsPi0pPb8TeVPHOSPt[37]              = { 0.0, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 1.0, 1.2, 1.4,
                                                        1.6, 1.8, 2.0, 2.2, 2.4, 2.6, 2.8, 3.0, 3.2, 3.4,
                                                        3.6, 3.8, 4.0, 4.5, 5.0, 5.5, 6.0, 7.0, 8.0, 10.0,
                                                        12.0, 16.0, 20.0, 25.0, 30.0, 35.0, 40.0};
    Double_t fBinsPi0pPb8TeVPtDCA[17]               = { 0.0, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 1.0, 1.2, 1.4,
                                                        1.8, 2.4, 3.5, 5.0, 7.0, 10.0, 14.0};
    Double_t fBinsPi0pPb8TeVCentPt[25]              = { 0.0, 0.4, 0.5, 0.6, 0.7, 0.8, 1.0, 1.2, 1.4, 1.6,
                                                        1.8, 2.0, 2.2, 2.4, 2.6, 2.8, 3.0, 3.5, 4.0, 5.0,
                                                        6.0, 8.0, 10.0, 12.0, 14.0};
    Int_t fBinsPi0pPb8TeVPtRebin[40]                = { 10, 8, 4, 2, 2, 1, 1, 1, 1, 1,
                                                        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                                                        1, 1, 1, 2, 2, 2, 4, 5, 8, 8, 10,
                                                        10,10, 10, 10,10, 10,10,10,10};
    Int_t fBinsPi0pPb8TeVPCMEMCPtRebin[34]          = { 4, 4, 4, 2, 2, 1, 1, 1, 1, 1, 1,
                                                        1, 1, 2, 2, 2, 2, 2, 2, 2, 4, 4, 5, 5,
                                                        8, 10, 10, 10, 10, 10, 10, 10};
    Int_t fBinsPi0pPb8TeVPCMEMCTrigEG2PtRebin[38]      = { 10, 8, 8, 8, 8, 4, 2, 4, 4, 4,
                                                        2, 2, 1, 1, 1, 1, 1, 1, 1, 1,
                                                        1, 1, 2, 2, 2, 2, 2, 2, 4, 4, 5, 10,
                                                        8, 10, 10, 10, 10, 10};
    Int_t fBinsPi0pPb8TeVPCMEMCTrigEG1PtRebin[46]      = { 10, 8, 8, 8, 8, 4, 2, 4, 4, 4,
                                                        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                                                        2, 2, 2, 2, 2, 4, 4, 4, 4, 4, 4, 4, 5, 10,
                                                        8, 10, 10, 10, 10, 10};
    Int_t fBinsPi0pPb8TeVmEMCPtRebin[59]         = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                                                        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                                                        1, 1, 2, 2, 2, 2, 2, 2, 2, 2,
                                                        2, 2, 2, 2, 4, 4, 4, 4, 4, 4,
                                                        4, 4, 4, 4, 4, 4, 4, 4, 5, 5,
                                                        5, 5, 5, 5, 5, 10,10,10,10};
    Int_t fBinsPi0pPb8TeVPCMPHOSPtRebin[36]         = { 7 , 7 , 7 , 7 , 4, 3, 3, 3, 3, 3,
                                                        3 , 3 , 3 , 3 , 3, 3, 3, 3, 4, 4,
                                                        4 , 4 , 4 , 4 , 4, 5, 7, 7, 7, 7,
                                                        7, 7, 7, 7, 10, 10};
    Int_t fBinsPi0pPb8TeVEMCPtRebin[32]             = { 5, 4, 4, 4, 4, 4, 4, 4, 4, 4,
                                                        4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
                                                        4, 4, 4, 4, 4, 4, 4, 4, 4};
    Int_t fBinsPi0pPb8TeVEMCTrigEG1PtRebin[38]             = { 5, 4, 4, 4, 4, 4, 4, 4, 4, 4,
                                                        4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
                                                        4, 4, 4, 4, 4, 4, 4, 4, 4,
                                                        4, 4, 4, 4, 4, 4, 4, 4};
    Int_t fBinsPi0pPb8TeVEMCTrigEG2PtRebin[37]             = { 5, 4, 4, 4, 4, 4, 4, 4, 4, 4,
                                                        4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
                                                        4, 4, 4, 4, 4, 4, 4, 4, 4, 5,
                                                        8, 8, 8, 8, 8, 8, 8};
    Int_t fBinsPi0pPb8TeVPHOSPtRebin[36]            = { 5, 4 , 3 , 3 , 3, 3, 3, 3, 3, 3,
                                                        3, 3 , 3 , 3 , 3, 3, 3, 3, 3, 3,
                                                        3, 3 , 3 , 3 , 3, 3, 3, 3, 3, 3,
                                                        7, 11, 11, 11, 10, 10};
    Int_t fBinsPi0pPb8TeVCentPtRebin[24]            = { 10, 4, 4, 4, 4, 4, 4, 4, 4, 4,
                                                        4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
                                                        4, 5, 10, 10};
    Double_t fBinsPi0pPb8TeVDalitzPt[23]            = { 0, 0.6, 0.7, 0.8, 1.0, 1.2, 1.4, 1.6, 1.8, 2.0,
                                                        2.2, 2.4, 2.6, 2.8, 3.0, 3.2, 3.6, 4.0, 5.0, 6.0,
                                                        8.0, 10., 15.};
    Int_t fBinsPi0pPb8TeVDalitzPtRebin[22]          = { 5, 5, 5, 5, 4, 4, 4, 4, 4, 4,
                                                        4, 4, 5, 5, 5, 5, 5, 5, 5, 5,
                                                        8, 8};
    Double_t fBinsPi0pPb8TeVEMCDalitzPt[23]         = { 0, 0.6, 0.7, 0.8, 1.0, 1.2, 1.4, 1.6, 1.8, 2.0,
                                                        2.2, 2.4, 2.6, 2.8, 3.0, 3.2, 3.6, 4.0, 5.0, 6.0,
                                                        8.0, 10., 15.};
    Int_t fBinsPi0pPb8TeVEMCDalitzPtRebin[22]       = { 5, 5, 8, 5, 4, 4, 4, 4, 4, 4,
                                                        4, 4, 5, 5, 5, 5, 5, 5, 5, 5,
                                                        8, 8};
    // ETA MESON
    Double_t fBinsEtapPb8TeVPt[23]                  = { 0., 0.3, 0.5, 0.7, 0.9, 1.1, 1.4, 1.8, 2.2, 2.6,
                                                        3.0, 3.5, 4.,  5.,  6., 8.,  10,  12., 14., 16.,
                                                        20., 25., 30.};
    Double_t fBinsEtapPb8TeVEMCPt[21]               = { 0., 1.4, 1.8, 2.2, 2.6,
                                                        3.0, 3.5, 4.,  5.,  6., 8.,  10,  12., 16., 20.,
                                                        25., 30., 35., 40., 45., 50.};
    Double_t fBinsEtapPb8TeVEMCTrigPt[21]               = { 0., 1.4, 1.8, 2.2, 2.6,
                                                        3.0, 3.5, 4.,  5.,  6., 8.,  10,  12., 16., 20.,
                                                        25., 30., 35., 40., 45., 50.};
    Double_t fBinsEtapPb8TeVPCMEMCPt[22]            = { 0., 0.3, 0.5, 0.7, 0.9, 1.1, 1.4, 1.8, 2.2, 2.6,
                                                        3.0, 3.5, 4.,  5.,  6., 8.,  10,  12., 16., 20.,
                                                        25., 30.};
    Double_t fBinsEtapPb8TeVPCMEMCTrigPt[24]            = { 0., 0.3, 0.5, 0.7, 0.9, 1.1, 1.4, 1.8, 2.2, 2.6,
                                                        3.0, 3.5, 4.,  5.,  6., 8.,  10,  12., 16., 20.,
                                                        25., 30.,40.,50.};
    Double_t fBinsEtapPb8TeVDMCPt[13]               = { 0., 1.4, 1.8, 2.5,
                                                        3.0, 4.,  6., 8., 12., 16., 20.,
                                                        25., 30.};
    Double_t fBinsEtapPb8TeVPCMDMCPt[19]            = { 0., 0.3, 0.5, 0.7, 0.9, 1.5, 2.0, 2.5,
                                                        3.0, 4.,  5.,  6., 8.,  10,  12., 16., 20.,
                                                        25., 30.};
    Double_t fBinsEtapPb8TeVPHOSPt[20]              = { 0., 0.3, 0.5, 0.7, 0.9, 1.1, 1.4, 1.8, 2.2, 3.0,
                                                        4.,  5.,  6., 8.,  10,  12., 16., 20., 25., 30.};
    Double_t fBinsEtapPb8TeVPCMPHOSPt[21]           = { 0., 0.3, 0.5, 0.7, 1.1, 1.4, 1.8, 2.2, 2.6, 3.0,
                                                        3.5, 4.,  5.,  6., 8.,  10,  12., 16., 20., 25.,
                                                        30.};
    Double_t fBinsEtapPb8TeVPtEMCTrig[27]           = { 0., 0.3, 0.5, 0.7, 0.9, 1.1, 1.4, 1.8, 2.2, 2.6,
                                                        3.0, 3.5, 4.,  5.,  6., 7.,  8., 9., 10, 11.,
                                                        12., 13., 14., 18., 20., 24., 30.};

    Double_t fBinsEtapPb8TeVCentPt[15]              = { 0.,  0.4,  0.6,  0.8,  1.0, 1.2, 1.4,  1.6,  2.0,  2.5,
                                                        3.0, 4.,   6.,   8.,   10};
    Double_t fBinsEtapPb8TeVPtDCA[17]               = { 0.0, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 1.0, 1.2, 1.4,
                                                        1.8, 2.4, 3.5, 5.0, 7.0, 10.0, 14.0};
    Int_t fBinsEtapPb8TeVPtRebin[22]                = { 10, 8,  8,  8,  5, 5,  5,  5,  4,  4,
                                                        4,   5,  8,  8,  8, 8,   10, 10, 10, 10,
                                                        10,  10};
    Int_t fBinsEtapPb8TeVPCMDMCPtRebin[18]          = { 10, 10,  10,  10,  16, 16,  16,
                                                        16,   10,  16,  16,  20, 20,  20, 20, 20, 20,
                                                        20};
    Int_t fBinsEtapPb8TeVPCMEMCPtRebin[21]          = { 10, 10,  10,  10,  16, 16,   16,  16,  10,  10,
                                                        10,   10,  10,  10,  16, 20,  20, 20, 20, 20,
                                                        20};
    Int_t fBinsEtapPb8TeVPCMEMCTrigPtRebin[23]      = { 10, 10,  10,  10,  8, 8,   8,  8,  8,  8,
                                                        8,   8,  8, 8,  8,  16,  8,
                                                        8,   8,  10, 16, 16, 20};
    Int_t fBinsEtapPb8TeVPCMEMCTrig85PtRebin[23]      = { 10, 10,  10,  10,  8, 8,   8,  8,  8,  8,
                                                        10,   10,  16, 16,  16,  16,  8,
                                                        8,   8,  10, 16, 16, 20};
    Int_t fBinsEtapPb8TeVPCMPHOSPtRebin[20]         = { 16,  16,  16,  16,  16,  16,  16,  16,  16, 16,
                                                        16,  16,  16,  16,  16,  21,  21,  21,  21, 21};
    Int_t fBinsEtapPb8TeVEMCPtRebin[20]             = { 8,   16,  10,  10,  8,
                                                        8,   5,  5,  5,  8, 10,   16,  20,  20,  20,
                                                        20,  20,  20,  20,
                                                        20};
    Int_t fBinsEtapPb8TeVEMCTrigPtRebin[20]             = { 8,   16,  10,  10,  8,
                                                        8,   5,  5,  5,  8, 10,   16,  20,  20,  20,
                                                        20,  20,  20,  20,
                                                        20};
    Int_t fBinsEtapPb8TeVDMCPtRebin[12]             = { 8,   16,  16,  16,  16,
                                                        16,   16,   16,  20,  20,  20,
                                                        20};
    Int_t fBinsEtapPb8TeVPHOSPtRebin[19]            = { 19,  17,  17,  17,  15,  15,  15,  15,  15,   15,
                                                        15,  15,  20,  25,  25,  25,  24,  19,  19};
    Int_t fBinsEtapPb8TeVCentPtRebin[14]            = { 10, 10, 8, 8, 8, 5,  5,  4,  4, 4,
                                                        4,  5,  8,  10};
    Double_t fBinsEtapPb8TeVDalitzPt[10]            = { 0., 0.6, 1.0, 1.4, 1.8, 2.2, 2.8, 4.4, 6., 10.};
    Int_t fBinsEtapPb8TeVDalitzPtRebin[9]           = { 10, 10, 10, 10, 10, 10, 10, 10, 10};

    Int_t fBinsPi0EtapPb8TeVPtRebin[22]             = { 8, 2, 1, 1, 1, 1, 1, 1, 1, 1,
                                                        1, 1, 2, 2, 2, 4, 4, 4, 4, 4,
                                                        4, 4};
    Int_t fBinsPi0EtapPb8TeVPCMEMCPtRebin[22]       = { 8, 2, 1, 1, 1, 1, 1, 1, 1, 1,
                                                        1, 1, 2, 2, 2, 4, 4, 4, 8, 10,
                                                        10, 10};
    Int_t fBinsPi0EtapPb8TeVPCMEMCTrigPtRebin[24]   = { 8, 2, 1, 1, 1, 1, 1, 1, 1, 1,
                                                        1, 1, 2, 2, 2, 2, 2, 4, 4, 4, 8, 10,
                                                        10, 10};
    Int_t fBinsPi0EtapPb8TeVPCMDMCPtRebin[22]       = { 8, 2, 1, 1, 1, 1, 1, 1, 1, 1,
                                                        1, 1, 2, 2, 2, 4, 4, 4, 8, 10,
                                                        10, 10};
    Int_t fBinsPi0EtapPb8TeVPCMPHOSPtRebin[22]      = { 8, 2, 1, 1, 1, 1, 1, 1, 1, 1,
                                                        1, 1, 2, 2, 2, 4, 4, 4, 4, 4,
                                                        4, 4};
    Int_t fBinsPi0EtapPb8TeVEMCPtRebin[16]          = { 4, 4, 4, 4,
                                                        4, 4, 4, 4, 4, 4, 5, 5, 8, 10,
                                                        10, 10};
    Int_t fBinsPi0EtapPb8TeVDMCPtRebin[12]          = { 4, 4, 2, 2,
                                                        2, 2, 2, 2, 4, 4,
                                                        4, 10};
    Int_t fBinsPi0EtapPb8TeVPHOSPtRebin[22]         = { 8, 2, 1, 1, 1, 1, 1, 1, 1, 1,
                                                        1, 1, 1, 1, 1, 2, 2, 2, 4, 4,
                                                        4, 4};
    Int_t fBinsPi0EtapPb8TeVDalitzPtRebin[9]        = { 8, 2, 2, 2, 2, 2, 4, 4, 4};

    Double_t fBinsEtapPb8TeVPt3Body[15]             = { 0., 0.4, 0.6, 0.8, 1.0, 1.2, 1.4, 1.6, 2.0, 2.5,
                                                        3.0, 4.0, 6.0, 8.0, 10};
    Int_t fBinsEtapPb8TeVPt3BodyRebin[14]           = { 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
                                                        5, 5, 5, 5};
    // DIR GAMMA
    Double_t fBinsDirGammapPb8TeVPt[26]             = { 0.0, 0.4, 0.5, 0.6, 0.7, 0.8, 1.0, 1.2, 1.4, 1.6,
                                                        1.8, 2.0, 2.2, 2.4, 2.6, 2.8, 3.2, 3.6, 4.0, 4.8,
                                                        5.6, 6.4, 7.2, 8.0, 10.0, 14.0};
    Int_t fBinsDirGammapPb8TeVPtRebin[25]           = { 4, 2, 1, 1, 1, 1, 1, 1, 1, 1,
                                                        1, 1, 1, 1, 1, 1, 1, 1, 2, 2,
                                                        2, 2, 4, 4, 4};
    Double_t fBinsDirGammapPb8TeVPCMEMCPt[29]       = { 0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 1.0,
                                                        1.2, 1.4, 1.6, 1.8, 2.0, 2.2, 2.4, 2.6, 2.8, 3.2,
                                                        3.6, 4.0, 4.8, 5.6, 6.4, 7.2, 8.0, 10.0, 14.0};
    Int_t fBinsDirGammapPb8TeVPCMEMCPtRebin[28]     = { 10, 4, 4, 2, 1, 1, 1, 1, 1, 1,
                                                        1, 1, 1, 1, 1, 1, 2, 2, 2, 2,
                                                        2, 4, 4, 4, 4, 5, 5, 5 };

#endif