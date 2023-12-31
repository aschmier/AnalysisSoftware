#ifndef GAMMACONV_ExtractSignalBinningpp2760GeV
#define GAMMACONV_ExtractSignalBinningpp2760GeV

    //****************************************************************************************************
    //******************** Pt binning for pp, 2.76 TeV ***************************************************
    //****************************************************************************************************
    Double_t fBinsPi02760GeVPt[25]                  = { 0.0, 0.4, 0.6, 0.8, 1.0, 1.2, 1.4, 1.6, 1.8, 2.0,
                                                        2.2, 2.4, 2.6, 3.0, 3.5, 4.0, 5.0, 6.0, 8.0, 10.0,
                                                        12.0, 15.0, 20., 25., 30.};
    Double_t fBinsPi02760GeVPtTrig13g[30]           = { 0.0, 0.4, 0.6, 0.8, 1.0, 1.2, 1.4, 1.6, 1.8, 2.0,
                                                        2.2, 2.4, 2.6, 3.0, 3.5, 4.0, 5.0, 6.0, 7.0, 8.0,
                                                        9.0, 10.0, 11.0, 12.0, 14.0, 16.0, 20., 24., 28., 30.};
    Double_t fBinsPi02760GeVPtTrig13gPCMEMC[29]     = { 0.0, 0.4, 0.6, 0.8, 1.0, 1.2, 1.4, 1.6, 1.8, 2.0,
                                                        2.2, 2.4, 2.6, 3.0, 3.5, 4.0, 5.0, 6.0, 7.0, 8.0,
                                                        9.0, 10.0, 12.0, 14.0, 16.0, 20., 24., 28., 30.};
    Double_t fBinsPi02760GeVPtTrig11a[26]           = { 0.0, 0.4, 0.6, 0.8, 1.0, 1.2, 1.4, 1.6, 1.8, 2.0,
                                                        2.2, 2.4, 2.6, 3.0, 3.5, 4.0, 5.0, 6.0, 7.0, 8.0,
                                                        9.0, 10.0, 12.0, 16.0, 20.0, 25};
    Double_t fBinsPi02760GeVPtTrigFullPCMEMC[26]    = { 0.0, 0.4, 0.6, 0.8, 1.0, 1.2, 1.4, 1.6, 1.8, 2.0,
                                                        2.2, 2.4, 2.6, 3.0, 3.5, 4.0, 5.0, 6.0, 7.0, 8.0,
                                                        9.0, 10.0, 12.0, 14.0, 16.0, 20.0 };
    Double_t fBinsPi02760GeVPtmEMC[34]              = { 0.0, 0.4, 0.6, 0.8, 1.0, 1.2, 1.4, 1.6, 1.8, 2.0,
                                                        2.2, 2.4, 2.6, 3.0, 3.5, 4.0, 5.0, 6.0, 7.0, 8.0,
                                                        9.0, 10.0, 11.0, 12.0, 14.0, 16.0, 18., 22., 26., 30.,
                                                        35.0, 40., 50., 60.};
    Double_t fBinsPi02760GeVPtDCA[15]               = { 0.0, 0.4, 0.6, 0.8, 1.0, 1.2, 1.4, 1.6, 2.0, 2.4,
                                                        3.0, 4.0, 6.0, 8.0, 10.0};
    Double_t fBinsPi02760GeVDalitzPt[8]             = { 0.0, 0.6, 1.0, 1.4, 2.0, 3.0, 5.0, 10.0};
    Int_t fBinsPi02760GeVDalitzPtRebin[7]           = { 5, 5, 4, 4, 4, 5, 5};
    Int_t fBinsPi02760GeVPtRebin[24]                = { 4, 4, 2, 2, 2, 2, 2, 2, 2, 2,
                                                        2, 2, 2, 2, 2, 2, 4, 4, 4, 8,
                                                        8, 8, 8, 8};
    Int_t fBinsPi02760GeVPCMEMCPtRebin[24]          = { 4, 4, 2, 2, 2, 2, 2, 2, 2, 2,
                                                        2, 2, 2, 2, 2, 2, 2, 4, 4, 4,
                                                        4, 4, 4, 4};
    Int_t fBinsPi02760GeVPCMEMCPtTrig13gRebin[28]   = { 4, 4, 2, 2, 2, 2, 2, 2, 2, 2,
                                                        2, 2, 2, 2, 2, 2, 2, 4, 4, 4,
                                                        4, 4, 4, 8, 8, 8, 8, 8};
    Int_t fBinsPi02760GeVEMCPtTrig13gRebin[29]      = { 4, 4, 2, 2, 2, 2, 2, 2, 2, 2,
                                                        2, 2, 2, 2, 2, 2, 2, 4, 4, 4,
                                                        4, 4, 4, 4, 8, 12, 12, 12, 12};
    Int_t fBinsPi02760GeVPCMEMCPtTrig11aRebin[25]   = { 4, 4, 2, 2, 2, 2, 2, 2, 2, 2,
                                                        2, 2, 2, 2, 2, 2, 2, 4, 4, 5,
                                                        5, 5, 10, 10, 10};
    Int_t fBinsPi02760GeVPtmEMCRebin[32]            = { 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
                                                        4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
                                                        4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
                                                        4, 4 };
    Double_t fBinsPi02760GeVFullHaitaomEMC[34]      = { 0.0,    0.4,    0.6,    0.8,    1.0,    1.2,    1.4,    1.6,    1.8,    2.0,
                                                        2.2,    2.4,    2.6,    3.0,    3.5,    4.0,    5.0,    6.0,    7.0,    8.0,
                                                        9.0,    10.0,   11.0,   12.0,   13.0,   14.0,   16.0,   18.,    22.0,   26.0,
                                                        30.0,   35.0,   40.0,   50.0    };

    Double_t fBinsEta2760GeVDalitzPt[8]             = { 0., 0.5, 1.0, 1.5, 2.0, 2.5, 4., 6.};
    Int_t fBinsEta2760GeVDalitzPtRebin[7]           = { 8, 8, 5, 5, 5, 5, 8};
    Double_t fBinsEta2760GeVPtAsPbPb[10]            = { 0., 0.5, 1.0, 1.5, 2.0, 2.5, 4.0, 6.0, 8.0, 10.};
    Double_t fBinsEta2760GeVPt[14]                  = { 0., 0.5, 1.0, 1.5, 2.0, 3.0, 4.0, 6.0, 8.0, 10.0,
                                                        12.0, 14.0, 20.0, 30.0};
    Double_t fBinsEta2760GeVPtTrig11a[14]           = { 0., 0.5, 1.0, 1.5, 2.0, 3.0, 4.0, 6.0, 8.0, 10.0,
                                                        12.0, 16.0, 20.0, 30.0};
    Double_t fBinsEta2760GeVPtDCA[16]               = { 0., 0.4, 0.6, 0.8, 1.0, 1.125, 1.5, 1.75, 2.0, 2.5,
                                                        3.0, 4.0, 6.0, 10.0, 15.0, 20.0 };
    Int_t fBinsEta2760GeVPtRebinAsPbPb[9]           = { 8, 8, 5, 5, 5, 5, 8, 10, 10};
    Int_t fBinsEta2760GeVPtRebin[13]                = { 10, 10, 10, 10, 10, 10, 10, 16, 16, 16,
                                                        16, 16, 16};
    Int_t fBinsEta2760GeVPCMEMCPtRebin[13]          = { 15, 15, 12, 12, 10, 10, 10, 10, 10, 10,
                                                        10, 10, 10 };
    Int_t fBinsEta2760GeVPCMEMCPtEG2Rebin[13]       = { 15, 15, 12, 12, 10, 10, 10, 10, 12, 20,
                                                        15, 15, 15 };
    Int_t fBinsEta2760GeVPCMEMCPtTrigINT7Rebin[13]  = { 8, 10, 10, 10, 10, 10, 10, 12, 12, 12,
                                                        12, 12, 12 };
    Int_t fBinsEta2760GeVPCMEMCPtTrig11aRebin[13]   = { 8, 8, 8, 8, 8, 8, 8, 8, 10, 16,
                                                        16, 16, 16 };

    Int_t fBinsPi0EtaBinning2760GeVDalitzPtRebin[7] = { 8, 2, 2, 2, 2, 4, 4};
    Int_t fBinsPi0EtaBinning2760GeVPtRebin[13]      = { 8, 2, 2, 2, 2, 2, 2, 4, 4, 4,
                                                        4, 4, 4 };
    Int_t fBinsPi0EtaBinning2760GeVPCMEMCPtRebin[13] = {8, 2, 2, 2, 2, 2, 2, 2, 2, 4,
                                                        4, 5, 5 };
    Int_t fBinsPi0EtaBinning2760GeVPCMEMCPtTrig11aRebin[13] = {4, 2, 2, 2, 2, 2, 2, 2, 2, 4,
                                                        4, 5, 5 };

    Double_t fBinsDirGamma2760GeVPt[25]             = { 0.0, 0.4, 0.6, 0.8, 1.0, 1.2, 1.4, 1.6, 1.8, 2.0,
                                                        2.2, 2.4, 2.6, 3.0, 3.5, 4.0, 5.0, 6.0, 8.0, 10.0,
                                                        12.0, 15.0, 20., 25., 30.};
    Int_t fBinsDirGamma2760GeVPtRebin[24]           = { 4, 4, 2, 2, 2, 2, 2, 2, 2, 2,
                                                        2, 2, 2, 2, 2, 2, 4, 4, 4, 8,
                                                        8, 8, 8, 8};
    Int_t fNBinsCluster2760GeVPt                    =  64;
    Double_t fBinsCluster2760GeVPt[65]              = { 0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9,
                                                        1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9,
                                                        2.0, 2.2, 2.4, 2.6, 2.8, 3.0, 3.2, 3.4, 3.6, 3.8,
                                                        4.0, 4.2, 4.4, 4.6, 4.8, 5.0, 5.2, 5.4, 5.6, 5.8,
                                                        6.0, 6.2, 6.4, 6.6, 6.8, 7.0, 7.4, 7.8, 8.2, 8.6,
                                                        9.0, 9.5, 10,  11,  12,  14,  16,  18,  20,  25,
                                                        30 , 35, 40, 45, 50 };


#endif
