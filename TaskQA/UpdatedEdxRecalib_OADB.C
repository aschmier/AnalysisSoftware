//!  EMCal bad channel map creation macro
/*!
    With this macro, the 2D histogram bad channel maps can be added to OADB/EMCAL/TPCdEdxRecalib.root.
*/
void updateFile(const char *fileNameOADB,TString arrName, TString fileNameInput,Int_t lowRun, Int_t highRun);
void sortOutput(const char *fileNameOADB="");
TObjArray *CreatePeriodContainer(TObjArray *inputcont);
void rebuildContainer(const char *fileNameOADB="");



/*******************************************************************
 *  NOTE: Main function which needs to be adjusted for new dEdx maps *
 *******************************************************************/
 void UpdatedEdxRecalib_OADB(const char *fileNameOADBAli="$ALICE_DATA/OADB/PWGGA/TPCdEdxRecalibOADB.root", Bool_t addOldMaps = kFALSE)
{
    gSystem->Load("libOADB");

    // create working copy of OADB file from EOS
    const char *fileNameOADB                ="TPCdEdxRecalib_temp.root";
    gSystem->Exec(Form("cp %s %s",fileNameOADBAli,fileNameOADB));

    // update OADB file with dead, bad and warm cells
    // last parameter:
    //      0: write new map for given run range
    //      1: update existing map for given run range
    //      2: same as 1 but also save over/underhead parts (special setting, not for normal use!)

    if(addOldMaps){
    }

    updateFile(fileNameOADB,"TPCRecalib_LHC13b","TaskQA/OADB_inputs/LHC13b/DeDxMaps_80010113_00200009327000000000000000_000481b1202.root", 195344, 195484);
    updateFile(fileNameOADB,"TPCRecalib_LHC13c","TaskQA/OADB_inputs/LHC13c/DeDxMaps_80010113_00200009327000000000000000_000481b1202.root", 195529, 195678);
    updateFile(fileNameOADB,"TPCRecalib_LHC13d","TaskQA/OADB_inputs/LHC13d/DeDxMaps_80010113_00200009327000000000000000_000481b1202.root", 195681, 195874);
    updateFile(fileNameOADB,"TPCRecalib_LHC13e","TaskQA/OADB_inputs/LHC13e/DeDxMaps_80010113_00200009327000000000000000_000481b1202.root", 195935, 196311);
    updateFile(fileNameOADB,"TPCRecalib_LHC13f","TaskQA/OADB_inputs/LHC13f/DeDxMaps_80010113_00200009327000000000000000_000481b1202.root", 196433, 197388);
    updateFile(fileNameOADB,"TPCRecalib_LHC15n","TaskQA/OADB_inputs/LHC15n/DeDxMaps_00010113_00200009227300000000000000_000481b1202.root", 244340, 244628); //
    updateFile(fileNameOADB,"TPCRecalib_LHC15o_1","TaskQA/OADB_inputs/LHC15o/DeDxMaps_16810013_00200009127600000500004000_000690b0402_list2.root", 244918, 245555);
    updateFile(fileNameOADB,"TPCRecalib_LHC15o_2","TaskQA/OADB_inputs/LHC15o/DeDxMaps_16810013_00200009127600000500004000_000690b0402_list1.root", 245683, 246277);
    updateFile(fileNameOADB,"TPCRecalib_LHC15o_3","TaskQA/OADB_inputs/LHC15o/DeDxMaps_16810013_00200009127600000500004000_000690b0402_list2.root", 246390, 246393);
    updateFile(fileNameOADB,"TPCRecalib_LHC15o_4","TaskQA/OADB_inputs/LHC15o/DeDxMaps_16810013_00200009127600000500004000_000690b0402_list1.root", 246424, 246995);
    updateFile(fileNameOADB,"TPCRecalib_LHC16f","TaskQA/OADB_inputs/LHC16f/DeDxMaps_00010113_00200089227300000000000000_0053b1f1202.root", 253659, 253834);
    updateFile(fileNameOADB,"TPCRecalib_LHC16q_1","TaskQA/OADB_inputs/LHC16q/DeDxMaps_80010113_00200009327000000000000000_000481b1202_range1.root", 265305, 265345); //
    updateFile(fileNameOADB,"TPCRecalib_LHC16q_2","TaskQA/OADB_inputs/LHC16q/DeDxMaps_80010113_00200009327000000000000000_000481b1202_range2.root", 265377, 265389); //
    updateFile(fileNameOADB,"TPCRecalib_LHC16q_3","TaskQA/OADB_inputs/LHC16q/DeDxMaps_80010113_00200009327000000000000000_000481b1202_range3.root", 265419, 265526); //
    updateFile(fileNameOADB,"TPCRecalib_LHC16r","TaskQA/OADB_inputs/LHC16r/DeDxMaps_80010113_00200009327000000000000000_000481b1202.root", 265588, 266319); //
    updateFile(fileNameOADB,"TPCRecalib_LHC16s","TaskQA/OADB_inputs/LHC16s/DeDxMaps_80010113_00200009327000000000000000_000481b1202.root", 266404, 267111); //
    updateFile(fileNameOADB,"TPCRecalib_LHC16t","TaskQA/OADB_inputs/LHC16t/DeDxMaps_80010113_00200009327000000000000000_000481b1202.root", 267161, 267167); //
    updateFile(fileNameOADB,"TPCRecalib_LHC17g_1","TaskQA/OADB_inputs/LHC17g/DeDxMaps_00010113_00200089227300000000000000_0053b1f1202_1.root", 270882, 270940);
    updateFile(fileNameOADB,"TPCRecalib_LHC17g_2","TaskQA/OADB_inputs/LHC17g/DeDxMaps_00010113_00200089227300000000000000_0053b1f1202_2.root", 270941, 271777);
    updateFile(fileNameOADB,"TPCRecalib_LHC17n","TaskQA/OADB_inputs/LHC17n/DeDxMaps_14810013_00200009327000000000000000_0053b1f1202.root", 280234, 280235); //
    updateFile(fileNameOADB,"TPCRecalib_LHC17pq","TaskQA/OADB_inputs/LHC17pq/DeDxMaps_00010113_00200009327000000000000000_000481b1202.root",282008, 282441); //
    updateFile(fileNameOADB,"TPCRecalib_LHC18c","TaskQA/OADB_inputs/LHC18c/DeDxMaps_00010113_00200089227300000000000000_0053b1f1202.root", 285466, 285958);
    updateFile(fileNameOADB,"TPCRecalib_LHC18q","TaskQA/OADB_inputs/LHC18q/DeDxMaps_16810013_00200009247600008250404000_000481b1202.root", 295581, 296624);
    updateFile(fileNameOADB,"TPCRecalib_LHC18r","TaskQA/OADB_inputs/LHC18r/DeDxMaps_16810013_00200009247600008250404000_000481b1202.root", 296690, 297625);
    
    
    /*******************************************************************
    *                       pp 13TeV, Normal B                         *
    *                       LHC16  - Jens Luehder                      *
    *******************************************************************/
    //------------------------------------
    //LHC16d 252235-252375 | whole range: 252235-252480 => ![252235-252480]!
    updateFile(fileNameOADB,"TPCRecalib_LHC16d","TaskQA/OADB_inputs/LHC16d/DeDxMaps_00010113_00200009327000000000000000_000481b1202.root", 252235, 252480);
    //------------------------------------
    //LHC16e Part1 252858-252867, 253517-253591 | whole range: 252603-253603 => ![252603-252867]!, ![253517-253603]!
    updateFile(fileNameOADB,"TPCRecalib_LHC16e_1","TaskQA/OADB_inputs/LHC16e/DeDxMaps_00010113_00200009327000000000000000_000481b1202_part1.root", 252603, 252867);
    //-
    updateFile(fileNameOADB,"TPCRecalib_LHC16e_1","TaskQA/OADB_inputs/LHC16e/DeDxMaps_00010113_00200009327000000000000000_000481b1202_part1.root", 253517, 253603);
    //------------------------------------
    //LHC16e Part2 253437-253488 | whole range: 252603-253603 => ![252868-253516]!
    updateFile(fileNameOADB,"TPCRecalib_LHC16e_2","TaskQA/OADB_inputs/LHC16e/DeDxMaps_00010113_00200009327000000000000000_000481b1202_part2.root", 252868, 253516);
    //------------------------------------
    //LHC16f 253951-253978 | whole range: 253614-253979 => ![253614,253979]!
    updateFile(fileNameOADB,"TPCRecalib_LHC16f","TaskQA/OADB_inputs/LHC16f/DeDxMaps_00010113_00200009327000000000000000_000481b1202_fullB.root", 253614, 253979);
    //------------------------------------
    //LHC16g 254128-254332 | whole range: 254124-254332 => ![254124,254332]!
    updateFile(fileNameOADB,"TPCRecalib_LHC16g","TaskQA/OADB_inputs/LHC16g/DeDxMaps_00010113_00200009327000000000000000_000481b1202.root", 254124, 254332);
    //------------------------------------
    //LHC16h 254418-255469 | whole range: 254378-255469 => ![254378,255469]!
    updateFile(fileNameOADB,"TPCRecalib_LHC16h","TaskQA/OADB_inputs/LHC16h/DeDxMaps_00010113_00200009327000000000000000_000481b1202.root", 254378, 255469);
    //------------------------------------
    //LHC16i 255539-255618 | whole range: 255515-255650 => ![255515,255650]!
    updateFile(fileNameOADB,"TPCRecalib_LHC16i","TaskQA/OADB_inputs/LHC16i/DeDxMaps_00010113_00200009327000000000000000_000481b1202.root", 255515, 255650);
    //------------------------------------
    //LHC16j 256204-256420 | whole range: 256146-256420 => ![256146,256420]!
    updateFile(fileNameOADB,"TPCRecalib_LHC16j","TaskQA/OADB_inputs/LHC16j/DeDxMaps_00010113_00200009327000000000000000_000481b1202.root", 256146, 256420);
    //------------------------------------
    //LHC16k Part1 256504-257492 | whole range: 256504-258574 => ![256504,257492]!
    updateFile(fileNameOADB,"TPCRecalib_LHC16k_1","TaskQA/OADB_inputs/LHC16k/DeDxMaps_00010113_00200009327000000000000000_000481b1202_part1.root", 256504, 257492);
    //------------------------------------
    //LHC16k Part2 257530-258008 | whole range: 256504-258574 => ![257493,258008]!
    updateFile(fileNameOADB,"TPCRecalib_LHC16k_2","TaskQA/OADB_inputs/LHC16k/DeDxMaps_00010113_00200009327000000000000000_000481b1202_part2.root", 257493, 258008);
    //------------------------------------
    //LHC16k Part3 258012-258537 | whole range: 256504-258574 => ![258009,258574]!
    updateFile(fileNameOADB,"TPCRecalib_LHC16k_3","TaskQA/OADB_inputs/LHC16k/DeDxMaps_00010113_00200009327000000000000000_000481b1202_part3.root", 258009, 258574);
    //------------------------------------
    //LHC16l Part1 258962-259477, 259748-259888 | whole range: 258883-260187 => ![258883,259477]!, ![259748,260187]!
    updateFile(fileNameOADB,"TPCRecalib_LHC16l_1","TaskQA/OADB_inputs/LHC16l/DeDxMaps_00010113_00200009327000000000000000_000481b1202_part1.root", 258883, 259477);
    //-
    updateFile(fileNameOADB,"TPCRecalib_LHC16l_1","TaskQA/OADB_inputs/LHC16l/DeDxMaps_00010113_00200009327000000000000000_000481b1202_part1.root", 259748, 260187);
    //------------------------------------
    //LHC16l Part2 259649-259747 | whole range: 258883-260187 => ![259478,259747]!
    updateFile(fileNameOADB,"TPCRecalib_LHC16l_2","TaskQA/OADB_inputs/LHC16l/DeDxMaps_00010113_00200009327000000000000000_000481b1202_part2.root", 259478, 259747);
    //------------------------------------
    //LHC16o 262450-264035 | whole range: 262395-264035 => ![262395,264035]!
    updateFile(fileNameOADB,"TPCRecalib_LHC16o","TaskQA/OADB_inputs/LHC16o/DeDxMaps_00010113_00200009327000000000000000_000481b1202.root", 262395, 264035);
    //------------------------------------
    //LHC16p 264078-264347 | whole range: 264076-264347 => ![264076,264347]!
    updateFile(fileNameOADB,"TPCRecalib_LHC16p","TaskQA/OADB_inputs/LHC16p/DeDxMaps_00010113_00200009327000000000000000_000481b1202.root", 264076, 264347);
    /*******************************************************************
     *                       LHC17  - Jens Luehder                     *
     ********************************************************************/
    //LHC17cef 270581-270865 | whole range: c:270531-f:270865 => ![270531,270865]!
    updateFile(fileNameOADB,"TPCRecalib_LHC17cef","TaskQA/OADB_inputs/LHC17cef/DeDxMaps_00010113_00200009327000000000000000_000481b1202.root", 270531, 270865);
    //------------------------------------
    //LHC17h Part1 271870-272834 | whole range: 271839-273103 => ![271839,272834]!
    updateFile(fileNameOADB,"TPCRecalib_LHC17h_1","TaskQA/OADB_inputs/LHC17h/DeDxMaps_00010113_00200009327000000000000000_000481b1202_part1.root", 271839, 272834);
    //------------------------------------
    //LHC17h Part2 272836-273103 | whole range: 271839-273103 => ![272835,273103]!
    updateFile(fileNameOADB,"TPCRecalib_LHC17h_2","TaskQA/OADB_inputs/LHC17h/DeDxMaps_00010113_00200009327000000000000000_000481b1202_part2.root", 272835, 273103);
    //------------------------------------
    //LHC17i 273591-274442 | whole range: 273486-274442 => ![273486,274442]!
    updateFile(fileNameOADB,"TPCRecalib_LHC17i","TaskQA/OADB_inputs/LHC17i/DeDxMaps_00010113_00200009327000000000000000_000481b1202.root", 273486, 274442);
    //------------------------------------
    //LHC17j 274593-274671 | whole range: 274591-274671 => ![274591,274671]!
    updateFile(fileNameOADB,"TPCRecalib_LHC17j","TaskQA/OADB_inputs/LHC17j/DeDxMaps_00010113_00200009327000000000000000_000481b1202.root", 274591, 274671);
    //------------------------------------
    //LHC17k 274690-276508 | whole range: 274690-276508=> ![274690,276508]!
    updateFile(fileNameOADB,"TPCRecalib_LHC17k","TaskQA/OADB_inputs/LHC17k/DeDxMaps_00010113_00200009327000000000000000_000481b1202.root", 274690, 276508);
    //------------------------------------
    //LHC17l 276551-278216 | whole range: 276551-278729 => ![276551,278729]!
    updateFile(fileNameOADB,"TPCRecalib_LHC17l","TaskQA/OADB_inputs/LHC17l/DeDxMaps_00010113_00200009327000000000000000_000481b1202.root", 276551, 278729);
    //------------------------------------
    //LHC17m Part1 278914-279559 | whole range: 278818-280140 => ![278818,279559]!
    updateFile(fileNameOADB,"TPCRecalib_LHC17m_1","TaskQA/OADB_inputs/LHC17m/DeDxMaps_00010113_00200009327000000000000000_000481b1202_part1.root", 278818, 279559);
    //------------------------------------
    //LHC17m Part2 279630-280140 | whole range: 278818-280140 => ![279560,280140]!
    updateFile(fileNameOADB,"TPCRecalib_LHC17m_2","TaskQA/OADB_inputs/LHC17m/DeDxMaps_00010113_00200009327000000000000000_000481b1202_part2.root", 279560, 280140);
    //------------------------------------
    //LHC17o Part1 280282-280634 | whole range: 280282-281961 => ![280282,280634]!
    updateFile(fileNameOADB,"TPCRecalib_LHC17o_1","TaskQA/OADB_inputs/LHC17o/DeDxMaps_00010113_00200009327000000000000000_000481b1202_part1.root", 280282, 280634);
    //------------------------------------
    //LHC17o Part2 280636-281633 | whole range: 280282-281961 => ![280635,281633]!
    updateFile(fileNameOADB,"TPCRecalib_LHC17o_2","TaskQA/OADB_inputs/LHC17o/DeDxMaps_00010113_00200009327000000000000000_000481b1202_part2.root", 280635, 281633);
    //------------------------------------
    //LHC17o Part3 281892-281961 | whole range: 280282-281961 => ![281634,281961]!
    updateFile(fileNameOADB,"TPCRecalib_LHC17o_3","TaskQA/OADB_inputs/LHC17o/DeDxMaps_00010113_00200009327000000000000000_000481b1202_part3.root", 281634, 281961);
    //------------------------------------
    //LHC17r 282528-282704 | whole range: 282504-282704 => ![282504,282704]!
    updateFile(fileNameOADB,"TPCRecalib_LHC17r","TaskQA/OADB_inputs/LHC17r/DeDxMaps_00010113_00200009327000000000000000_000481b1202.root", 282504, 282704);
    /*******************************************************************
     *                       LHC18  - Jens Luehder                     *
     ********************************************************************/
    //LHC18b 285009-285396 | whole range: 284706-285447 => ![284706,285447]!
    updateFile(fileNameOADB,"TPCRecalib_LHC18b","TaskQA/OADB_inputs/LHC18b/DeDxMaps_00010113_00200009327000000000000000_000481b1202.root", 284706, 285447);
    //------------------------------------
    //LHC18d 285978-286350 | whole range: 285978-286350 => ![285978,286350]!
    updateFile(fileNameOADB,"TPCRecalib_LHC18d","TaskQA/OADB_inputs/LHC18d/DeDxMaps_00010113_00200009327000000000000000_000481b1202.root", 285978, 286350);
    //------------------------------------
    //LHC18e 286380-286937 | whole range: 286380-286958 => ![286380,286958]!
    updateFile(fileNameOADB,"TPCRecalib_LHC18e","TaskQA/OADB_inputs/LHC18e/DeDxMaps_00010113_00200009327000000000000000_000481b1202.root", 286380, 286958);
    //------------------------------------
    //LHC18f 287000-287658 | whole range: 286982-287977 => ![286982,287977]!
    updateFile(fileNameOADB,"TPCRecalib_LHC18f","TaskQA/OADB_inputs/LHC18f/DeDxMaps_00010113_00200009327000000000000000_000481b1202.root", 286982, 287977);
    //------------------------------------
    //LHC18g 288619-288750 | whole range: 288619-288750 => ![288619,288750]!
    updateFile(fileNameOADB,"TPCRecalib_LHC18g","TaskQA/OADB_inputs/LHC18g/DeDxMaps_00010113_00200009327000000000000000_000481b1202.root", 288619, 288750);
    //------------------------------------
    //LHC18hij 288804-288806, 288861-288909, 288943 | whole range: h:288804-j:288943 => ![288804,288943]!
    updateFile(fileNameOADB,"TPCRecalib_LHC18hij","TaskQA/OADB_inputs/LHC18hij/DeDxMaps_00010113_00200009327000000000000000_000481b1202.root", 288804, 288943);
    //------------------------------------
    //LHC18k 289165-289201 | whole range: 289165-289201 => ![289165,289201]!
    updateFile(fileNameOADB,"TPCRecalib_LHC18k","TaskQA/OADB_inputs/LHC18k/DeDxMaps_00010113_00200009327000000000000000_000481b1202.root", 289165, 289201);
    //------------------------------------
    //LHC18l Part1 289240-289494 | whole range: 289240-289971 => ![289240,289494]!
    updateFile(fileNameOADB,"TPCRecalib_LHC18l_1","TaskQA/OADB_inputs/LHC18l/DeDxMaps_00010113_00200009327000000000000000_000481b1202.root", 289240, 289494);
    //------------------------------------
    //LHC18l Part2 289521-289971 | whole range: 289240-289971 => ![289495,289971]!
    updateFile(fileNameOADB,"TPCRecalib_LHC18l_2","TaskQA/OADB_inputs/LHC18l/DeDxMaps_00010113_00200009327000000000000000_000481b1202.root", 289495, 289971);
    //------------------------------------
    //LHC18mn 290323-292839, 293357-293359 | whole range: m:290167-n:293362 => ![290167,293362]!
    updateFile(fileNameOADB,"TPCRecalib_LHC18m","TaskQA/OADB_inputs/LHC18m/DeDxMaps_00010113_00200009327000000000000000_000481b1202.root", 290323, 293359);
    //------------------------------------
    //LHC18o 293475-293898 | whole range: 293368-293898 => ![293368,293898]!
    updateFile(fileNameOADB,"TPCRecalib_LHC18o","TaskQA/OADB_inputs/LHC18o/DeDxMaps_00010113_00200009327000000000000000_000481b1202.root", 293368, 293898);
    //------------------------------------
    //LHC18p 294009-294925 | whole range: 294009-295232 => ![294009,295232]!
    updateFile(fileNameOADB,"TPCRecalib_LHC18p","TaskQA/OADB_inputs/LHC18p/DeDxMaps_00010113_00200009327000000000000000_000481b1202.root", 294009, 295232);

    
    // the final output will be sorted by runnumber
    sortOutput(fileNameOADB);
    // the OADB container is additionally checked for ownership
    rebuildContainer("TPCdEdxRecalibNEW.root");
}


/*******************************************************************
 *  NOTE: Update function that removes existing BC maps from old   *
 *          file and adds new BC maps at the end of the file       *
 *  NOTE: The final file is saved and directly renamed as new      *
 *      input file for the next loop (this reduces total file size)*
 *******************************************************************/
void updateFile(const char *fileNameOADB,TString arrName, TString fileNameInput,Int_t lowRun, Int_t highRun){
    printf("\n\nAdding %s maps to OADB file:\n",arrName.Data());

    const int nRBins=4;

    // load new input file
    TFile *fInput                                    = TFile::Open(fileNameInput);
    if(fInput->IsZombie()){
        cout << "new input file " << fileNameInput.Data() << " not found! returning..." << endl;
        return;
    }
    // load OADB file
    AliOADBContainer *con = NULL;
    TFile *f                                    = new TFile(fileNameOADB);
    if(!fInput->IsZombie()){
        f->ls();
        con                       =(AliOADBContainer*)f->Get("AliTPCdEdxRecalib");
        if(con) con->SetName("Old");
    } else {
        cout << "previous OADB file not found... this is fine if it is the first time running...." << endl;
    }

    // make the output container
    AliOADBContainer *con2                      = new AliOADBContainer("AliTPCdEdxRecalib");

    // List of brand new array
    TObjArray arrayAdd(16);
    arrayAdd.SetName(arrName.Data());

    TH2S *hdEdxRecalibElecRecalib[nRBins];
    TH2S *hdEdxRecalibPosiRecalib[nRBins];

    // initialize histograms in array - either load existing map or create new, empty map
    for(int i=0;i<nRBins;i++){
        hdEdxRecalibElecRecalib[i] = NULL;
        hdEdxRecalibPosiRecalib[i] = NULL;

        hdEdxRecalibElecRecalib[i] = (TH2S*)fInput->Get(Form("Ele_Cl%d_recalib",i));
        hdEdxRecalibElecRecalib[i]->SetName(Form("Ele_Cl%d_recalib",i));
        hdEdxRecalibPosiRecalib[i] = (TH2S*)fInput->Get(Form("Pos_Cl%d_recalib",i));
        hdEdxRecalibPosiRecalib[i]->SetName(Form("Pos_Cl%d_recalib",i));
        if(!hdEdxRecalibElecRecalib[i] || !hdEdxRecalibPosiRecalib[i] ){
            cout << "Input histogram not found for bin " << i << "! returning...." <<  endl;
            return;
        }
    }

    // add histograms to a new array
    for (Int_t i=0;i<nRBins;i++){
        arrayAdd.Add(hdEdxRecalibElecRecalib[i]);
        arrayAdd.Add(hdEdxRecalibPosiRecalib[i]);
    }

    // check old OADB file for existing BC maps in given run range and remove that old BC map
    Int_t replacedContainerLowLimit             = -1;
    Int_t replacedContainerHighLimit            = -1;
    if(con){
        for(int i=0;i<con->GetNumberOfEntries();i++){
            if (lowRun >= con->LowerLimit(i) && lowRun <= con->UpperLimit(i)){
                printf("\n!!! Not adding index %d for runrange %d--%d as low run limit %d is contained\n", i, con->LowerLimit(i),con->UpperLimit(i),lowRun);
                replacedContainerLowLimit           = con->LowerLimit(i);
                replacedContainerHighLimit          = con->UpperLimit(i);
            } else if (highRun >= con->LowerLimit(i) && highRun <= con->UpperLimit(i)){
                printf("\n!!! Not adding index %d for runrange %d--%d as high run limit %d is contained\n", i, con->LowerLimit(i),con->UpperLimit(i),highRun);
                replacedContainerLowLimit           = con->LowerLimit(i);
                replacedContainerHighLimit          = con->UpperLimit(i);
            } else if ((con->LowerLimit(i)==con->UpperLimit(i))&& con->LowerLimit(i) >= lowRun && con->UpperLimit(i)<=highRun){
                printf("\n!!! Not adding index %d for runrange %d--%d as high run limit %d is contained\n", i, con->LowerLimit(i),con->UpperLimit(i),highRun);
            }else{
                con2->AddDefaultObject(con->GetObjectByIndex(i));
                con2->AppendObject(con->GetObjectByIndex(i),con->LowerLimit(i),con->UpperLimit(i));
            }
        }
    } else {
        cout << "old container not found... is this the first time running???" << endl;
    }
    // add new BC maps at the end of the new OADB file (will be sorted later)
    con2->AddDefaultObject(&arrayAdd);
    con2->AppendObject(&arrayAdd, lowRun,highRun);


    // temporarilty save BC map file and rename as new input file
    con2->WriteToFile("tempCalib.root");
    gSystem->Exec(Form("mv tempCalib.root %s",fileNameOADB));

    printf("\n%s Maps have been successfully added!\n\n",arrName.Data());

//     plotBadChannelMapUpdate(hdEdxRecalib,arrName);

//     printf("...and also plotted!!\n");

}

/*******************************************************************
 *  NOTE: Sorting function to sort the final OADB file             *
 *                  by ascending runnumber                         *
 *******************************************************************/
void sortOutput(const char *fileNameOADB){

    TFile *f                                    = TFile::Open(fileNameOADB);
    f->ls();
    AliOADBContainer *con                       =(AliOADBContainer*)f->Get("AliTPCdEdxRecalib");
    con->SetName("Old");

    Int_t indexAdd                              = 0;
    Int_t largerthan                            = 0;
    Int_t currentvalue                          = 0;

    AliOADBContainer *con2                      = new AliOADBContainer("AliTPCdEdxRecalib");
    con2->SetOwner(1);
    // First entry needs to be added before sorting loop
    con2->AppendObject(con->GetObjectByIndex(0),con->LowerLimit(0),con->UpperLimit(0));
    TString strTemp                             = "";

    // sorting magic happens here
    for(int i=1;i<con->GetNumberOfEntries();i++){
        largerthan                              = con2->UpperLimit(con2->GetNumberOfEntries()-1);
        currentvalue                            = -1;
        indexAdd                                = 0;
        for(int j=0;j<con->GetNumberOfEntries();j++){
            if(con->UpperLimit(j)<=largerthan)
                continue;
            if(currentvalue < 0){
                currentvalue                    = con->UpperLimit(j);
                indexAdd                        = j;
            }
            if(con->UpperLimit(j)<currentvalue){
                currentvalue                    = con->UpperLimit(j);
                indexAdd                        = j;
            }
        }
        con2->AppendObject(con->GetObjectByIndex(indexAdd),con->LowerLimit(indexAdd),con->UpperLimit(indexAdd));
    }

    printf("\n\n");
    Int_t nentries2                             = con2->GetNumberOfEntries();
    for(int i=0;i<nentries2;i++){
        printf("\n Entry2 --> %d/%d -->",i,nentries2);
        printf("%d -- %d --> obj = %p , %s", con2->LowerLimit(i),con2->UpperLimit(i),con2->GetObjectByIndex(i),con2->GetObjectByIndex(i)->GetName());
    }
    printf("\n\n");

    con2->WriteToFile("TPCdEdxRecalibNEW.root");
}


TObjArray *CreatePeriodContainer(TObjArray *inputcont){
  TObjArray *newcont = new TObjArray(inputcont->GetEntries());
  newcont->SetName(inputcont->GetName());
  for(int i = 0; i < inputcont->GetEntries(); i++){
    newcont->AddAt(inputcont->At(i)->Clone(), i);
  }
  return newcont;
}

/*******************************************************************
 *                                                                 *
 *        NOTE: Function required to fix OADB ownership            *
 *                                                                 *
 *******************************************************************/
void rebuildContainer(const char *fileNameOADB){
  TFile *reader = TFile::Open(fileNameOADB);
  AliOADBContainer *cont = static_cast<AliOADBContainer *>(reader->Get("AliTPCdEdxRecalib"));
  delete reader;

  AliOADBContainer *newcont = new AliOADBContainer("AliTPCdEdxRecalib");
  newcont->SetOwner(1);
  for(int irun = 0; irun < cont->GetNumberOfEntries(); irun++){
    newcont->AppendObject(CreatePeriodContainer(static_cast<TObjArray *>(cont->GetObjArray()->At(irun))), cont->LowerLimit(irun), cont->UpperLimit(irun));
  }

  newcont->WriteToFile("TPCdEdxRecalibOADBfix.root");

  TFile *reReader = TFile::Open("TPCdEdxRecalibOADBfix.root", "READ");
    AliOADBContainer *reCont = static_cast<AliOADBContainer *>(reReader->Get("AliTPCdEdxRecalib"));
    delete reReader;
    delete reCont;

  gSystem->Exec("mv TPCdEdxRecalibOADBfix.root TPCdEdxRecalibOADB.root");
  gSystem->Exec("rm TPCdEdxRecalibNEW.root");
  gSystem->Exec("rm TPCdEdxRecalib_temp.root");

}

