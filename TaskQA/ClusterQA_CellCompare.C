/*******************************************************************************
 ******  provided by Gamma Conversion Group, PWGGA,                        *****
 ******     Daniel Muehlheim, d.muehlheim@cern.ch                          *****
 *******************************************************************************/

#include "QA.h"

void FillVector(TH1* hist, std::vector<Int_t> &vec);
void FillGlobalCellsVector(std::vector<Int_t> &vec,std::vector<Int_t> &vec2);
void ClusterQA_CellCompare(TString suffix = "eps", Int_t mode = 2)
{
	cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
	cout << "ClusterQA_CellCompare" << endl;
	cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;

	gROOT->Reset();

	StyleSettingsThesis();
	SetPlotStyle();

	TString outputDir ="CellCompare";

		// configuration for LHC16 13 TeV pp data for EMC
		const Int_t nSets = 1;
		TString fEnergyFlag = "13TeV";
		TString cut = "00010113_3885500081041220000_01631031000000d0";
		TString DataSets[nSets]={ "LHC16p_pass1"};
		// Data Path for Hot Cell compare log files. Refer to V2 for how this will be integrated into steering macro.
		TString DataPath			 = "ClusterQA_HotCellCompare/LHC16_EMC";
		TString DataPath_cold  = "ClusterQA_DeadCellCompare/LHC16_EMC";
		// parameters determining whether a log file for the given range will be written
		Int_t runStart         	= 264078;
		Int_t runEnd					  = 264088;
		Bool_t runRange				  = kTRUE;

    // const Int_t nSets = 8;
    // TString fEnergyFlag = "8TeV";
    // TString cut = "00000113_00200009327000008250400000_1111111063032230000_0163103100000010";
    // TString DataSets[nSets]={"LHC12a","LHC12b","LHC12c","LHC12d","LHC12f","LHC12g","LHC12h","LHC12i"};

//    const Int_t nSets = 5;
//    TString fEnergyFlag = "7TeV";
//    TString cut = "00000113_00200009327000008250400000_1111100013032230000_0163103100000010";
//    TString DataSets[nSets]={"LHC10b_pass4","LHC10c_pass4","LHC10d_pass4","LHC10e_pass4","LHC10f_pass4"};

	TString outputPath = Form("%s/%s/ClusterQA/%s/%s",cut.Data(),fEnergyFlag.Data(),suffix.Data(),outputDir.Data());
	gSystem->Exec("mkdir -p "+outputPath);
	fstream out[nSets];

	std::vector<Int_t> cellIDsEnergy[nSets];
	std::vector<Int_t> cellIDsTime[nSets];
	std::vector<Int_t> cellIDsHotCells1D[nSets];
	std::vector<Int_t> cellIDsHotCellsTime1D[nSets];
	std::vector<Int_t> cellIDsHotCells2D[nSets];
	std::vector<Int_t> cellIDsMissing[nSets];
	std::vector<Int_t> allCells[nSets];

	TString vecStr[7]={"Energy","Time","HotCells1D","HotCellsTime1D","HotCells2D","Missing","allCells"};

	std::vector<Int_t> globalCells;

	TH1D* temp[7];
	for(Int_t i=0; i<nSets; i++)
	{
		TString fFile = Form("%s/%s/ClusterQA/ClusterQA_%s.root", cut.Data(),fEnergyFlag.Data(), DataSets[i].Data());
		TFile* File = new TFile(fFile.Data(),"READ");
		if(File->IsZombie()) {cout << "Warning: ROOT file '" << fFile.Data() << "' could not be openend!" << endl;}
		out[i].open(Form("%s/BadCells_%s.log",outputPath.Data(),DataSets[i].Data()), ios::out);

		cout << endl;
		cout << "\t\t----------------------------------------------------------------------------" << endl;
		cout << "\t\tDataSet: " << DataSets[i].Data() << ":" << endl;
		cout << "\t\tProcessing file: " << fFile.Data() << endl;
		cout << "\t\t----------------------------------------------------------------------------" << endl;
		cout << endl;

		temp[0] = (TH1D*) File->Get(vecStr[0]);
		temp[1] = (TH1D*) File->Get(vecStr[1]);
		temp[2] = (TH1D*) File->Get(vecStr[2]);
		temp[3] = (TH1D*) File->Get(vecStr[3]);
		temp[4] = (TH1D*) File->Get(vecStr[4]);
		temp[5] = (TH1D*) File->Get(vecStr[5]);
		temp[6] = (TH1D*) File->Get(vecStr[6]);
		for(Int_t j=0; j<7; j++) cout << " - "<< temp[j] << ", ";
		FillVector(temp[0],cellIDsEnergy[i]);
		FillVector(temp[1],cellIDsTime[i]);
		FillVector(temp[2],cellIDsHotCells1D[i]);
		FillVector(temp[3],cellIDsHotCellsTime1D[i]);
		FillVector(temp[4],cellIDsHotCells2D[i]);
		FillVector(temp[5],cellIDsMissing[i]);
		FillVector(temp[6],allCells[i]);
		FillGlobalCellsVector(allCells[i],globalCells);


		// Make Cell ID vs. Run Number Histo for Bad Cells
		// Takes as input Runwise Log File from Hot Cell Compare
		//---------------------------------------------------------------------------------------------------------------
		fstream fLogInput_Runwise;
		TString logFileName                     = Form("%s/%s-Runwise-Cleaned.log", DataPath.Data(), DataSets[i].Data());
		fLogInput_Runwise.open(logFileName.Data(), ios::in);

		fstream fLogInput_RunwiseCold;
		TString logFileNameCold                 = Form("%s/%s-Runwise-Cleaned.log", DataPath_cold.Data(), DataSets[i].Data());
		fLogInput_RunwiseCold.open(logFileName.Data(), ios::in);

		//If the user has not cleaned the log file
		if (!fLogInput_Runwise.good()){
			cout << "\n\t\t############################################################################" << endl;
			cout << "\t\tThere is no cleaned log file with the name: " <<  logFileName << endl;
			logFileName                     = Form("%s/%s-Runwise.log", DataPath.Data(), DataSets[i].Data());
			fLogInput_Runwise.open(logFileName.Data(), ios::in);
			cout << "\t\tInstead using the uncleaned version with the name: "<< logFileName << endl;
			cout << "\t\t############################################################################" << endl;
		}
		//If the user has not cleaned the log file
		if (!fLogInput_RunwiseCold.good()){
			cout << "\n\t\t############################################################################" << endl;
			cout << "\t\tThere is no cleaned log file with the name: " <<  logFileNameCold << endl;
			logFileNameCold                     = Form("%s/%s-Runwise.log", DataPath_cold.Data(), DataSets[i].Data());
			fLogInput_RunwiseCold.open(logFileNameCold.Data(), ios::in);
			cout << "\t\tInstead using the uncleaned version with the name: "<< logFileNameCold << endl;
			cout << "\t\t############################################################################" << endl;
		}


		// read in the runlist
		std::vector<TString> vecRuns;
    TString fileRuns[nSets];
    fileRuns[i]             = Form("DownloadAndDataPrep/runlists/runNumbers%s.txt", DataSets[i].Data());
    cout << "trying to read: " << fileRuns[i].Data() << endl;
    if(!readin(fileRuns[i], vecRuns, kFALSE)) {cout << "ERROR, no Run Numbers could be found! Returning..." << endl; return;}


		Double_t xbins[vecRuns.size()];
		std::vector<Double_t> vecRunsInd;
		std::vector<Double_t> vecCellIDs;
		std::vector<Double_t> uniqueCells;
		std::vector<Double_t> vecSigma;


		// Read in from the Input File
		if (fLogInput_Runwise.good()){
			fLogInput_Runwise.seekg(0L, ios::beg);
			//loop over hot cells
			while(!fLogInput_Runwise.eof()){
				TString fCurrentLine;
				fLogInput_Runwise >> fCurrentLine;
				if(fCurrentLine.Sizeof()>1) {
					TObjArray *rNumber          = fCurrentLine.Tokenize("-");
					TObjString* rCellID         = (TObjString*)rNumber->At(0);
					TObjString* rRunNumber	    = (TObjString*)rNumber->At(1);
					TObjString* rSigma          = (TObjString*)rNumber->At(2);
					TString fCellID	           	= rCellID->GetString();
					TString fRunNumber				 	= rRunNumber->GetString();
					TString fSigma							= rSigma->GetString();
					//cout << "Cell ID: " << fCellID << " runNumber: " << fRunNumber << endl;
					vecRunsInd.push_back(fRunNumber.Atoi());
					vecCellIDs.push_back(fCellID.Atoi());
					vecSigma.push_back(fSigma.Atof());
					// check to see if it is a unique cell
					if (std::find(uniqueCells.begin(), uniqueCells.end(), fCellID.Atof()) != uniqueCells.end()){
					}
					else{
						uniqueCells.push_back(fCellID.Atof());
						//cout << "The cell is unique " << fCellID << endl;
					}
				}
			}
			// loop over cold cells
			if (fLogInput_RunwiseCold.good()){
				while(!fLogInput_RunwiseCold.eof()){
					TString fCurrentLine;
					fLogInput_RunwiseCold >> fCurrentLine;
					if(fCurrentLine.Sizeof()>1) {
						TObjArray *rNumber          = fCurrentLine.Tokenize("-");
						TObjString* rCellID         = (TObjString*)rNumber->At(0);
						TObjString* rRunNumber	    = (TObjString*)rNumber->At(1);
						TObjString* rSigma          = (TObjString*)rNumber->At(2);
						TString fCellID	           	= rCellID->GetString();
						TString fRunNumber				 	= rRunNumber->GetString();
						TString fSigma							= rSigma->GetString();
						cout << "Cell ID: " << fCellID << " runNumber: " << fRunNumber << endl;
						vecRunsInd.push_back(fRunNumber.Atoi());
						vecCellIDs.push_back(fCellID.Atoi());
						vecSigma.push_back(fSigma.Atof());
						// check to see if it is a unique cell
						if (std::find(uniqueCells.begin(), uniqueCells.end(), fCellID.Atof()) != uniqueCells.end()){
						}
						else{
							uniqueCells.push_back(fCellID.Atof());
							//cout << "The cell is unique " << fCellID << endl;
						}
					}
				}
			}
			else{
				cout << "\nThere is an error opening the file: " << logFileNameCold << endl;
			}
		
			// Create the binning for the histogram.
			std::sort(uniqueCells.begin(), uniqueCells.end());
			Double_t ybins[uniqueCells.size()];
			for(UInt_t j = 0; j< vecRuns.size(); j++){
					xbins[j] = j;
			}
			for(UInt_t h = 0; h< uniqueCells.size(); h++){
					ybins[h] = h;
			}

			// Create the canvas as a function of # of Cells and # of Runs
			TCanvas* canvas2 = new TCanvas("canvas2","",200,10,vecRuns.size()*50,uniqueCells.size()*20);  // gives the page size
			Double_t leftMargin = 0.1; Double_t rightMargin = 0.15; Double_t topMargin = 0.1; Double_t bottomMargin = 0.1;
			DrawGammaCanvasSettings(canvas2, leftMargin, rightMargin, topMargin, bottomMargin);
			TH2D* RunwiseHist;
			RunwiseHist = new TH2D(Form("Bad Cells Runwise %s", DataSets[i].Data()), Form("Bad Cells Runwise %s", DataSets[i].Data()),vecRuns.size()-1, xbins, uniqueCells.size()-1, ybins );
			for(UInt_t z= 1; z< vecRuns.size();z++){
				RunwiseHist->GetXaxis()->SetBinLabel(z, vecRuns.at(z));
			}

			fstream fLogRunRange;
			if(runRange == kTRUE){
				cout << "Creating Bad Cell log file for Run Range for Run " << runStart << " to Run " << runEnd << endl;
				TString name = outputPath + "/" +"BadCells_"+ std::to_string(runStart) + "_" + std::to_string(runEnd) +".log";
				cout << "Log File Path: " << name << endl;
				fLogRunRange.open(name, ios::out);
			}
			else{
				cout << "Creating Bad Cell log file" << endl;
				TString name = outputPath + "/" +"BadCells" +".log";
				cout << "Log File Path: " << name << endl;
				fLogRunRange.open(name, ios::out);
			}

			for(UInt_t z= 1; z< uniqueCells.size();z++){
				TString s;
				s.Form("%.0f", uniqueCells.at(z));
				RunwiseHist->GetYaxis()->SetBinLabel(z, s);
				RunwiseHist->GetYaxis()->SetLabelSize(0.01);
				if (runRange == kFALSE){
					fLogRunRange << uniqueCells.at(z) << endl;
				}
			}

			// Loop over runs to populate hist
			std::vector<Double_t> vecOutput;
			for(UInt_t r= 0; r< vecRunsInd.size();r++){
				if(runRange == kTRUE){
					if( runStart <= vecRunsInd.at(r) <= runEnd){
						if (std::find(vecOutput.begin(), vecOutput.end(), vecCellIDs.at(r)) != vecOutput.end()){
							//cout << "repeat" << endl;
						}
						else{
							vecOutput.push_back(vecCellIDs.at(r));
							fLogRunRange << vecCellIDs.at(r) << endl;
						}
					}
				}
				Int_t binx;
				Int_t biny;
				// Get proper bin
				for(UInt_t d = 0; d < vecRuns.size(); d++){
					if(vecRuns.at(d).Atof() == vecRunsInd.at(r)){
						binx = d;
						break;
					}
				}
				for(UInt_t d = 0; d < uniqueCells.size(); d++){
					if( vecCellIDs.at(r) == uniqueCells.at(d)){
						biny = d;
						break;
					}
				}
				RunwiseHist->SetBinContent(binx,biny, vecSigma.at(r));
				//cout << "binx: " << binx << " biny: " << biny << "with value: " << vecSigma.at(r) << endl;
			}
			fLogRunRange.close();
			RunwiseHist->LabelsOption("v", "X");
			RunwiseHist->GetXaxis()->SetTitle("Run Number");
			RunwiseHist->GetXaxis()->SetTitleOffset(1.4);
			RunwiseHist->GetYaxis()->SetTitle("CellID");
			RunwiseHist->Draw("COLZ");
			canvas2->Update();
			canvas2->SaveAs(Form("%s/BadCellCandidates_Runwise_%s.pdf", outputPath.Data(),DataSets[i].Data()));
			canvas2->Clear();
			delete RunwiseHist;

		}
		else{
			cout << "\nThere is an error opening the file: " << logFileName << endl;
		}
		fLogInput_Runwise.close();
	} // end loop over nsets
	cout.flush();
	cout << endl;
	cout << "vector.size before sort and unique: " << globalCells.size() << "." << endl;
	cout << "sorting vector...";
	selection_sort(globalCells.begin(),globalCells.end());
	cout << "done" << endl;
	vector<Int_t>::iterator it;
	cout << "unique vector...";
	it = unique(globalCells.begin(),globalCells.end());
	cout << "done" << endl;
	cout << "resize vector...";
	globalCells.resize( distance(globalCells.begin(),it) );
	cout << "done" << endl;
	cout << "vector.size after sort and unique: " << globalCells.size() << "." << endl;

	TCanvas* canvas = new TCanvas("canvas","",200,10,1350,1800);  // gives the page size
	Double_t leftMargin = 0.1; Double_t rightMargin = 0.1; Double_t topMargin = 0.06; Double_t bottomMargin = 0.1;
	DrawGammaCanvasSettings(canvas, leftMargin, rightMargin, topMargin, bottomMargin);

	TH2D* hist;

	Int_t atPlotting = 0;
	Int_t iCount = 0;
	Int_t stepSize = 150;
	do{
		atPlotting = (iCount+1)*stepSize;
		if((Int_t)globalCells.size()<=atPlotting) atPlotting = (Int_t)globalCells.size();
		hist = new TH2D("BadCellCandidates","BadCellCandidates",nSets,0,nSets,atPlotting-iCount*stepSize,0+iCount*stepSize,atPlotting);
		for(Int_t iX=0; iX<nSets; iX++) hist->GetXaxis()->SetBinLabel(iX+1,DataSets[iX]);
		for(Int_t iC=iCount*stepSize; iC<atPlotting; iC++){
			hist->GetYaxis()->SetBinLabel(iC+1-iCount*stepSize,Form("%i",globalCells.at(iC)));
			for(Int_t iX=0; iX<nSets; iX++){
				std::vector<Int_t>::iterator it;
				it = find (allCells[iX].begin(), allCells[iX].end(), globalCells.at(iC));
				if (it != allCells[iX].end()){
					hist->SetBinContent(iX+1,iC+1-iCount*stepSize,1);
					out[iX] << globalCells.at(iC) << endl;
				}
			}

		}

		hist->GetZaxis()->SetRangeUser(0,2);
		DrawAutoGammaHisto2D(hist,
							 "Bad Cell Candidates",
							 "Period",
							 "CellID",
							 "",
							 0,0,0,
							 0,0,0,
							 1,1,0.035,0.035,0.015,0.035);
		//PutProcessLabelAndEnergyOnPlot(0.73, 0.93, 0.03, fCollisionSystem.Data(), fPlot.Data(), Form("%s clusters", calo.Data()));
		canvas->SetLogx(0); canvas->SetLogy(0); canvas->SetLogz(0); canvas->Update();
		canvas->SaveAs(Form("%s/BadCellCandidates_%i.%s", outputPath.Data(),iCount++,suffix.Data()));
		canvas->Clear();
		delete hist;
	}while(atPlotting != (Int_t)globalCells.size());

	for(Int_t i=0; i<nSets; i++) out[i].close();

	cout << "Done with ClusterQA_CellCompare" << endl;
	cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
	return;
}

void FillVector(TH1* hist, std::vector<Int_t> &vec){
	if(hist){
		for(Int_t i=1; i<hist->GetXaxis()->GetNbins()+1;i++){
			TString st = hist->GetXaxis()->GetBinLabel(i);
			vec.push_back((Int_t)st.Atoi());
		}
	}

	return;
}

void FillGlobalCellsVector(std::vector<Int_t> &vec,std::vector<Int_t> &vec2){
	for(Int_t i=0; i<(Int_t)vec.size();i++){
		vec2.push_back((Int_t)vec.at(i));
	}
	return;
}

