#include <vector>

#ifndef GAMMACONV_ExtractSignalBinningpp13TeV_omega
#define GAMMACONV_ExtractSignalBinningpp13TeV_omega

    #include <iostream>    
    using namespace std;


    // omega->pi+pi-pi0
    //****************************************************************************************************
    //*********************** Omega Pt binning for pp, 13TeV  ********************************************
    //****************************************************************************************************

    //---------------------------------------------------------------------------------------------------------
    //-------------------------------------------Combined-Binning----------------------------------------------
    Int_t fBinsOmegaPiPlPiMiPiZero13TevPtCombined_maxBins                           = 29;
    std::vector<Double_t> fBinsOmegaPiPlPiMiPiZero13TevPtCombined                   = { //Combined
                                                                                         0.00,  1.60,  2.00,  2.50,  3.00,          3.50,  4.00,  5.00,  6.00,  8.00,
                                                                                        10.00, 12.00, 14.00, 16.00, 18.00,         20.00, 22.00, 24.00, 26.00, 28.00,
                                                                                        30.00, 35.00, 40.00, 45.00, 50.00,         60.00, 70.00, 80.00, 90.00, 100.0
                                                                                    }; //30
    std::vector<Int_t> fBinsOmegaPiPlPiMiPiZero13TevPtRebinCombined                 = { //PCM-PCM Rebin
                                                                                        20, 10, 10, 10, 10,      10,  4,  4,  4,  4,
                                                                                         4,  4,  4,  4,  4,      10, 10, 10, 10, 10,
                                                                                        20, 20, 20, 20, 20,      20, 20, 20, 20
                                                                                    };//29


    //---------------------------------------------------------------------------------------------------------
    //-----------------------------------------------PCM-------------------------------------------------------
    Int_t fBinsOmegaPiPlPiMiPiZero13TevPtPCM_maxBins                                = 29;
    std::vector<Double_t> fBinsOmegaPiPlPiMiPiZero13TevPtPCM                        = { //PCM-PCM
                                                                                        0.00,  1.60,  2.00,  2.50,  3.00,          3.50,  4.00,  5.00,  6.00,  8.00,
                                                                                       10.00, 12.00, 14.00, 16.00, 18.00,         20.00, 22.00, 24.00, 26.00, 28.00,
                                                                                       30.00, 35.00, 40.00, 45.00, 50.00,         60.00, 70.00, 80.00, 90.00, 100.0
                                                                                    }; //30

    std::vector<Int_t> fBinsOmegaPiPlPiMiPiZero13TevPtRebinPCM                      = { //PCM-PCM Rebin
                                                                                        20, 10,  5,  5,  4,       4,  4,  4,  4, 10,
                                                                                        10, 10, 10, 10, 10,      10, 10, 10, 10, 10,
                                                                                        20, 20, 20, 20, 20,      20, 20, 20, 20
                                                                                    };//29
    //--------------------------------------------PCM-PCM: INT7
    Int_t fBinsOmegaPiPlPiMiPiZero13TevPtPCMINT7_maxBins                            = 29;
    std::vector<Double_t> fBinsOmegaPiPlPiMiPiZero13TevPtPCMINT7                    = { //PCM-PCM
                                                                                        0.00,  1.60,  2.00,  2.50,  3.00,          3.50,  4.00,  5.00,  6.00,  8.00,
                                                                                       10.00, 12.00, 14.00, 16.00, 18.00,         20.00, 22.00, 24.00, 26.00, 28.00,
                                                                                       30.00, 35.00, 40.00, 45.00, 50.00,         60.00, 70.00, 80.00, 90.00, 100.0
                                                                                    }; //30

    std::vector<Int_t> fBinsOmegaPiPlPiMiPiZero13TevPtRebinPCMINT7                  = { //PCM-PCM Rebin
                                                                                        20, 10,  5,  5,  4,       4,  4,  4,  4, 10,
                                                                                        10, 10, 10, 10, 10,      10, 10, 10, 10, 10,
                                                                                        20, 20, 20, 20, 20,      20, 20, 20, 20
                                                                                    };//29


    //---------------------------------------------------------------------------------------------------------
    //-----------------------------------------------PCM-PHOS--------------------------------------------------
    Int_t fBinsOmegaPiPlPiMiPiZero13TevPtPCMPHOS_maxBins                            = 29;
    std::vector<Double_t> fBinsOmegaPiPlPiMiPiZero13TevPtPCMPHOS                    = { //PCM-PHOS
                                                                                        0.00,  1.60,  2.00,  2.50,  3.00,          3.50,  4.00,  5.00,  6.00,  8.00,
                                                                                       10.00, 12.00, 14.00, 16.00, 18.00,         20.00, 22.00, 24.00, 26.00, 28.00,
                                                                                       30.00, 35.00, 40.00, 45.00, 50.00,         60.00, 70.00, 80.00, 90.00, 100.0
                                                                                    }; //30
    std::vector<Int_t> fBinsOmegaPiPlPiMiPiZero13TevPtRebinPCMPHOS                  = { //PCM-PHOS Rebin
                                                                                        20, 10, 10, 10, 10,      10,  4,  4,  4,  4,
                                                                                         4,  4,  4,  4,  4,      10, 10, 10, 10, 10,
                                                                                        20, 20, 20, 20, 20,      20, 20, 20, 20
                                                                                    };//29
    //--------------------------------------------PCM-PHOS: INT7
    Int_t fBinsOmegaPiPlPiMiPiZero13TevPtPCMPHOSINT7_maxBins                        = 29;
    std::vector<Double_t> fBinsOmegaPiPlPiMiPiZero13TevPtPCMPHOSINT7                = { //PCM-PHOS
                                                                                        0.00,  1.60,  2.00,  2.50,  3.00,          3.50,  4.00,  5.00,  6.00,  8.00,
                                                                                       10.00, 12.00, 14.00, 16.00, 18.00,         20.00, 22.00, 24.00, 26.00, 28.00,
                                                                                       30.00, 35.00, 40.00, 45.00, 50.00,         60.00, 70.00, 80.00, 90.00, 100.0
                                                                                    }; //30
    std::vector<Int_t> fBinsOmegaPiPlPiMiPiZero13TevPtRebinPCMPHOSINT7              = { //PCM-PHOS Rebin
                                                                                        20, 10, 10, 10,  4,       4,  4,  4, 10, 10,
                                                                                        10, 10, 10, 10, 10,      10, 10, 10, 10, 10,
                                                                                        20, 20, 20, 20, 20,      20, 20, 20, 20
                                                                                    };//29
    //--------------------------------------------PCM-PHOS: PHI7
    Int_t fBinsOmegaPiPlPiMiPiZero13TevPtPCMPHOSPHI7_maxBins                        = 29;
    std::vector<Double_t> fBinsOmegaPiPlPiMiPiZero13TevPtPCMPHOSPHI7                = { //PCM-PHOS
                                                                                        0.00,  1.60,  2.00,  2.50,  3.00,          3.50,  4.00,  5.00,  6.00,  8.00,
                                                                                       10.00, 12.00, 14.00, 16.00, 18.00,         20.00, 22.00, 24.00, 26.00, 28.00,
                                                                                       30.00, 35.00, 40.00, 45.00, 50.00,         60.00, 70.00, 80.00, 90.00, 100.0
                                                                                    }; //30
    std::vector<Int_t> fBinsOmegaPiPlPiMiPiZero13TevPtRebinPCMPHOSPHI7              = { //PCM-PHOS Rebin
                                                                                        20, 10, 10, 10, 10,      10,  4,  4,  4,  4,
                                                                                         4,  4,  4,  4,  4,      10, 10, 10, 10, 10,
                                                                                        20, 20, 20, 20, 20,      20, 20, 20, 20
                                                                                    };//29

    //---------------------------------------------------------------------------------------------------------
    //----------------------------------------------PHOS-PHOS--------------------------------------------------
    Int_t fBinsOmegaPiPlPiMiPiZero13TevPtPHOS_maxBins                               = 29;
    std::vector<Double_t> fBinsOmegaPiPlPiMiPiZero13TevPtPHOS                       = { //PHOS-PHOS
                                                                                        0.00,  1.60,  2.00,  2.50,  3.00,          3.50,  4.00,  5.00,  6.00,  8.00,
                                                                                       10.00, 12.00, 14.00, 16.00, 18.00,         20.00, 22.00, 24.00, 26.00, 28.00,
                                                                                       30.00, 35.00, 40.00, 45.00, 50.00,         60.00, 70.00, 80.00, 90.00, 100.0
                                                                                    }; //30
    std::vector<Int_t> fBinsOmegaPiPlPiMiPiZero13TevPtRebinPHOS                     = { //PHOS-PHOS Rebin
                                                                                        20, 10, 10, 10, 10,      10,  4,  4,  4,  4,
                                                                                         4,  4,  4,  4,  4,      10, 10, 10, 10, 10,
                                                                                        20, 20, 20, 20, 20,      20, 20, 20, 20
                                                                                    };//29
    //--------------------------------------------PHOS-PHOS: INT7
    Int_t fBinsOmegaPiPlPiMiPiZero13TevPtPHOSINT7_maxBins                           = 29;
    std::vector<Double_t> fBinsOmegaPiPlPiMiPiZero13TevPtPHOSINT7                   = { //PHOS-PHOS
                                                                                        0.00,  1.60,  2.00,  2.50,  3.00,          3.50,  4.00,  5.00,  6.00,  8.00,
                                                                                       10.00, 12.00, 14.00, 16.00, 18.00,         20.00, 22.00, 24.00, 26.00, 28.00,
                                                                                       30.00, 35.00, 40.00, 45.00, 50.00,         60.00, 70.00, 80.00, 90.00, 100.0
                                                                                    }; //30
    std::vector<Int_t> fBinsOmegaPiPlPiMiPiZero13TevPtRebinPHOSINT7                     = { //PHOS-PHOS Rebin
                                                                                        20, 10, 10, 10, 10,      10,  4,  4,  4, 10,
                                                                                        10, 10, 10, 10, 10,      10, 10, 10, 10, 10,
                                                                                        20, 20, 20, 20, 20,      20, 20, 20, 20
                                                                                    };//29
    //--------------------------------------------PHOS-PHOS: PHI7
    Int_t fBinsOmegaPiPlPiMiPiZero13TevPtPHOSPHI7_maxBins                           = 29;
    std::vector<Double_t> fBinsOmegaPiPlPiMiPiZero13TevPtPHOSPHI7                   = { //PHOS-PHOS
                                                                                        0.00,  1.60,  2.00,  2.50,  3.00,          3.50,  4.00,  5.00,  6.00,  8.00,
                                                                                       10.00, 12.00, 14.00, 16.00, 18.00,         20.00, 22.00, 24.00, 26.00, 28.00,
                                                                                       30.00, 35.00, 40.00, 45.00, 50.00,         60.00, 70.00, 80.00, 90.00, 100.0
                                                                                    }; //30
    std::vector<Int_t> fBinsOmegaPiPlPiMiPiZero13TevPtRebinPHOSPHI7                 = { //PHOS-PHOS Rebin
                                                                                        20, 10, 10, 10, 10,      10,  4,  4,  4,  4,
                                                                                         4,  4,  4,  4,  4,      10, 10, 10, 10, 10,
                                                                                        20, 20, 20, 20, 20,      20, 20, 20, 20
                                                                                    };//29

    //---------------------------------------------------------------------------------------------------------
    //-----------------------------------------------PCM-EMCal-------------------------------------------------
    Int_t fBinsOmegaPiPlPiMiPiZero13TevPtPCMEMC_maxBins                             = 29;
    std::vector<Double_t> fBinsOmegaPiPlPiMiPiZero13TevPtPCMEMC                     = { //PCM-EMCal
                                                                                        0.00,  1.60,  2.00,  2.50,  3.00,          3.50,  4.00,  5.00,  6.00,  8.00,
                                                                                       10.00, 12.00, 14.00, 16.00, 18.00,         20.00, 22.00, 24.00, 26.00, 28.00,
                                                                                       30.00, 35.00, 40.00, 45.00, 50.00,         60.00, 70.00, 80.00, 90.00, 100.0
                                                                                    }; //30
    std::vector<Int_t> fBinsOmegaPiPlPiMiPiZero13TevPtRebinPCMEMC                   = {
                                                                                        20, 10, 10, 10, 10,      10,  4,  4,  4,  4,
                                                                                       10, 10, 10, 10, 10,      10, 10, 10, 10, 10,
                                                                                        20, 20, 20, 20, 20,      20, 20, 20, 20
                                                                                    };//29
    //--------------------------------------------PCM-EMCal: EMC7
    Int_t fBinsOmegaPiPlPiMiPiZero13TevPtPCMEMCEMC7_maxBins                         = 29;
    std::vector<Double_t> fBinsOmegaPiPlPiMiPiZero13TevPtPCMEMCEMC7                 = { //PCM-EMCal
                                                                                        0.00,  1.60,  2.00,  2.50,  3.00,          3.50,  4.00,  5.00,  6.00,  8.00,
                                                                                       10.00, 12.00, 14.00, 16.00, 18.00,         20.00, 22.00, 24.00, 26.00, 28.00,
                                                                                       30.00, 35.00, 40.00, 45.00, 50.00,         60.00, 70.00, 80.00, 90.00, 100.0
                                                                                    }; //30
    std::vector<Int_t> fBinsOmegaPiPlPiMiPiZero13TevPtRebinPCMEMCEMC7               = {
                                                                                        20, 10, 10, 10, 10,      10,  4,  4,  4,  4,
                                                                                         4, 10, 10, 10, 10,      10, 10, 10, 10, 10,
                                                                                        20, 20, 20, 20, 20,      20, 20, 20, 20
                                                                                    };//29
    //--------------------------------------------PCM-EMCal: EG1, 8d 8GeV
    Int_t fBinsOmegaPiPlPiMiPiZero13TevPtPCMEMCEG1_maxBins                          = 29;
    std::vector<Double_t> fBinsOmegaPiPlPiMiPiZero13TevPtPCMEMCEG1                  = { //PCM-EMCal EG1
                                                                                        0.00,  1.60,  2.00,  2.50,  3.00,          3.50,  4.00,  5.00,  6.00,  8.00,
                                                                                       10.00, 12.00, 14.00, 16.00, 18.00,         20.00, 22.00, 24.00, 26.00, 28.00,
                                                                                       30.00, 35.00, 40.00, 45.00, 50.00,         60.00, 70.00, 80.00, 90.00, 100.0
                                                                                    }; //30
    std::vector<Int_t> fBinsOmegaPiPlPiMiPiZero13TevPtRebinPCMEMCEG1                = { //PCM-EMCal EG1 (8d)
                                                                                        20, 10, 10, 10, 10,      10, 10, 10, 10, 10,
                                                                                        10, 10, 10, 10, 10,      10, 10, 10, 10, 10,
                                                                                        10, 10, 20, 20, 20,      20, 20, 20, 20
                                                                                    };//29
    //--------------------------------------------PCM-EMCal: EG2, 8e, 4GeV
    Int_t fBinsOmegaPiPlPiMiPiZero13TevPtPCMEMCEG2_maxBins                          = 29;
    std::vector<Double_t> fBinsOmegaPiPlPiMiPiZero13TevPtPCMEMCEG2                  = { //PCM-EMCal
                                                                                        0.00,  1.60,  2.00,  2.50,  3.00,          3.50,  4.00,  5.00,  6.00,  8.00,
                                                                                       10.00, 12.00, 14.00, 16.00, 18.00,         20.00, 22.00, 24.00, 26.00, 28.00,
                                                                                       30.00, 35.00, 40.00, 45.00, 50.00,         60.00, 70.00, 80.00, 90.00, 100.0
                                                                                    }; //30
    std::vector<Int_t> fBinsOmegaPiPlPiMiPiZero13TevPtRebinPCMEMCEG2                = { //PCM-EMCal EG2 (8e)
                                                                                        20, 10, 10, 10, 10,      10, 10, 10, 10, 10,
                                                                                         4,  4,  4,  4, 10,      10, 10, 10, 10, 10,
                                                                                        20, 20, 20, 20, 20,      20, 20, 20, 20
                                                                                    };//29
    //---------------------------------------------------------------------------------------------------------
    //--------------------------------------------EMCal-EMCal--------------------------------------------------
    Int_t fBinsOmegaPiPlPiMiPiZero13TevPtEMC_maxBins                                = 29;//25;
    std::vector<Double_t> fBinsOmegaPiPlPiMiPiZero13TevPtEMC                        = { //EMCal-EMCal Combined
                                                                                        0.00,  1.60,  2.00,  2.50,
                                                                                        3.00,  3.50,  4.00,  5.00,  6.00,          8.00, 10.00, 12.00, 14.00, 16.00,
                                                                                       18.00, 20.00, 22.00, 24.00, 26.00,         28.00, 30.00, 35.00, 40.00, 45.00, 
                                                                                       50.00, 60.00, 70.00, 80.00, 90.00,         100.0
                                                                                    }; //26
    std::vector<Int_t> fBinsOmegaPiPlPiMiPiZero13TevPtRebinEMC                      = { //Rebin EMCal-EMCal EG1 (8d)
                                                                                       10, 10, 10, 10,
                                                                                       10, 10, 10, 10, 10,     10,  4,  4,  4,  4,
                                                                                        4, 10, 10, 10, 10,     10, 20, 20, 20, 20,
                                                                                       20, 20, 20, 20, 20
                                                                                    };//25
    //--------------------------------------------EMCal-EMCal: EMC7, MB
    Int_t fBinsOmegaPiPlPiMiPiZero13TevPtEMCEMC7_maxBins                            = 15;//11;
    std::vector<Double_t> fBinsOmegaPiPlPiMiPiZero13TevPtEMCEMC7                    = { //EMCal-EMCal EMC7
                                                                                        0.00,  1.60,  2.00,  2.50,
                                                                                        3.00,  3.50,  4.00,  5.00,  6.00,          8.00, 10.00, 12.00, 14.00, 16.00,
                                                                                       20.00, 24.00
                                                                                    }; //12
    std::vector<Int_t> fBinsOmegaPiPlPiMiPiZero13TevPtRebinEMCEMC7                  = { //Rebin EMCal-EMCal, EMC7 Binning
                                                                                        10, 10, 10, 10,
                                                                                        10, 10, 10, 10, 10,     10, 10, 10, 10, 10,
                                                                                        10
                                                                                    };//11

    //--------------------------------------------EMCal-EMCal: EG1, 8d 8GeV
    Int_t fBinsOmegaPiPlPiMiPiZero13TevPtEMCEG1_maxBins                             = 29;//25;
    std::vector<Double_t> fBinsOmegaPiPlPiMiPiZero13TevPtEMCEG1                     = { //EMCal-EMCal EG1 (8d)
                                                                                        0.00,  1.60,  2.00,  2.50,
                                                                                        3.00,  3.50,  4.00,  5.00,  6.00,          8.00, 10.00, 12.00, 14.00, 16.00,
                                                                                       18.00, 20.00, 22.00, 24.00, 26.00,         28.00, 30.00, 35.00, 40.00, 45.00,
                                                                                       50.00, 60.00, 70.00, 80.00, 90.00,         100.0

                                                                                       //18.00, 20.00, 22.00, 24.00, 26.00,         28.00, 30.00, 35.00, 40.00, 50.00,
                                                                                       //60.00, 70.00, 80.00, 90.00,         100.0
                                                                                    }; //26
    std::vector<Int_t> fBinsOmegaPiPlPiMiPiZero13TevPtRebinEMCEG1                   = { //Rebin EMCal-EMCal EG1 (8d)
                                                                                        5,  5,  5,  5,
                                                                                        5,  5,  5,  5,  5,      5,  5,  5,  5,  5,
                                                                                        5,  5,  5,  5,  5,     10, 10, 10, 10, 20,
                                                                                       20, 20, 20, 20, 20
                                                                                    };//25

    //--------------------------------------------EMCal-EMCal: EG2, 8e, 4GeV
    Int_t fBinsOmegaPiPlPiMiPiZero13TevPtEMCEG2_maxBins                             = 27;//23;
    std::vector<Double_t> fBinsOmegaPiPlPiMiPiZero13TevPtEMCEG2                     = { //EMCal-EMCal EG2 (8e)
                                                                                        0.00,  1.60,  2.00,  2.50,
                                                                                        3.00,  3.50,  4.00,  5.00,  6.00,          8.00, 10.00, 12.00, 14.00, 16.00,
                                                                                       18.00, 20.00, 22.00, 24.00, 26.00,         30.00, 35.00, 40.00, 50.00, 60.00,
                                                                                       70.00, 80.00, 90.00, 100.0
                                                                                    }; //24
    std::vector<Int_t> fBinsOmegaPiPlPiMiPiZero13TevPtRebinEMCEG2                   = { //Rebin EMCal-EMCal EG2 (8e)
                                                                                       10, 10, 10, 10,
                                                                                       10, 10, 10, 10, 10,     10, 10, 10, 10, 10,
                                                                                       10, 10, 10, 10, 10,     10, 20, 20, 20, 20,
                                                                                       20, 20, 20
                                                                                    };//23


#endif