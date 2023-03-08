//This function is used to automatically adjust the y-axis range of an histogram
//It retrieves the maximum and minimum value(ignoring empty bins) of the histogram
//and adds a range on both sides which is defined by the variable factor

//used to calculate min and max values of a set of histograms
Bool_t AdjustHistRangeTH1F(TH1F* histogram, Double_t factorLow, Double_t factorHigh, Bool_t useBinError, Double_t* A_tmp, Double_t* B_tmp, Int_t xfirst = 1, Int_t xlast = 1){

    if(histogram->GetEntries()<=0) return kFALSE;

    Double_t max = histogram->GetBinContent(xfirst);
    Double_t min = histogram->GetBinContent(xfirst);

    if(histogram->GetBinContent(xfirst)==0){
        do{
            xfirst++;
            max = histogram->GetBinContent(xfirst);
            min = histogram->GetBinContent(xfirst);
        }
        while(histogram->GetBinContent(xfirst)==0 && xfirst < histogram->GetNbinsX());
    }
    if(xfirst==histogram->GetNbinsX()){
        cout << "Warning in AdjustHistRangeTH1F: Histogram with name: " << histogram->GetName() << " contains 0 in all bins, return..." << endl;
        return kFALSE;
    }

    Double_t valueMin = 0;
    Double_t valueMax = 0;

    if(xlast==1) xlast = histogram->GetXaxis()->GetNbins();

    for(Int_t binx = xfirst; binx <= xlast; binx++){
        valueMax = histogram->GetBinContent(binx);
        if(useBinError) valueMax += histogram->GetBinError(binx);
        valueMin = histogram->GetBinContent(binx);
        if(useBinError) valueMin -= histogram->GetBinError(binx);
        if ((valueMin < min) && (valueMin!=0)) min = valueMin;
        if ((valueMax > max) && (valueMax!=0)) max = valueMax;
    }
    if(max<0) max/=factorHigh;
    else max*=factorHigh;
    if(min<=0) min*=factorLow;
    else min/=factorLow;

    *A_tmp = min;
    *B_tmp = max;
    return kTRUE;
}

void AdjustHistRangeTH1F(TH1F* histogram, Double_t factorLow, Double_t factorHigh, Bool_t useBinError, Int_t fixRange = 0, Double_t range = 0, Int_t xfirst = 1, Int_t xlast = 1){

    if(histogram->GetEntries()<=0){
        cout << "Warning in AdjustHistRangeTH1F: No Entries in given histogram with name: " << histogram->GetName() << ", return..." << endl;
        return;
    }

    Double_t A, B;
    if(!AdjustHistRangeTH1F(histogram,factorLow,factorHigh,useBinError,&A,&B,xfirst,xlast)) {cout << "ERROR: AdjustHistRangeTH1F(TH1F* histogram, Double_t factorLow, Double_t factorHigh, Bool_t useBinError, Double_t* A_tmp, Double_t* B_tmp) returned kFALSE! Returning..." << endl; return;}

    if(fixRange==0) histogram->GetYaxis()->SetRangeUser(A, B);
    else if(fixRange==1) histogram->GetYaxis()->SetRangeUser(range, B);
    else if(fixRange==2) histogram->GetYaxis()->SetRangeUser(A, range);
    else cout << "ERROR*03: AdjustRange.h wrong value of parameter fixRange given (valid 0,1 or 2): " << fixRange << endl;

    return;
}

void AdjustHistRangeTH1F(TH2F* histogram, Double_t factorLow, Double_t factorHigh, Bool_t useBinError, Int_t fixRange = 0, Double_t range = 0){
    Double_t min = 0;
    Double_t max = 0;
    TH1F* hist = (TH1F*) histogram->ProjectionY("proj",1,histogram->GetNbinsX());
    AdjustHistRangeTH1F(hist,factorLow,factorHigh,useBinError,min,max);

    if(fixRange==0) histogram->GetYaxis()->SetRangeUser(min, max);
    else if(fixRange==1) histogram->GetYaxis()->SetRangeUser(range, max);
    else if(fixRange==2) histogram->GetYaxis()->SetRangeUser(min, range);
    else cout << "ERROR*01: AdjustRange.h wrong value of parameter fixRange given (valid 0,1 or 2): " << fixRange << endl;

    return;
}



//overloaded function for AdjustHistRangeTH1F
//This function calls AdjustHistRangeTH1F for all histograms in the same vector
void AdjustHistRangeTH1F(std::vector<TH1F*> vectorhist, Double_t factorLow, Double_t factorHigh, Bool_t useBinError, Int_t fixRange = 0, Double_t range = 0, Bool_t isYLog = kFALSE, Int_t xfirst = 1, Int_t xlast = 1){

    if(!vectorhist.empty()){
        Double_t Max_global,Min_global;
        Double_t A,B;

        Bool_t successRange = AdjustHistRangeTH1F(vectorhist.at(0),factorLow,factorHigh, useBinError, &A, &B, xfirst, xlast);
        if(isYLog){
            if(A<0){
            successRange = kFALSE;
            A = 0;
            }
        }

        Int_t iRange = 1;
        if(!successRange && ((Int_t) vectorhist.size() > 1) ){
            while(!successRange){
                successRange = AdjustHistRangeTH1F(vectorhist.at(iRange++),factorLow,factorHigh, useBinError, &A, &B, xfirst, xlast);
                if(isYLog){
                    if(A<0) successRange = kFALSE;
                }
                if(iRange>=(Int_t) vectorhist.size()){
                    cout << "ERROR in AdjustHistRangeTH1F, iRange>vectorhist.size(), hist: "<< ((TH1F*)vectorhist.at(0))->GetName() <<", returning..." << endl;
                    return;
                }
            }
        }

        Min_global = A;
        Max_global = B;

        for(Int_t j=iRange; j<(Int_t) vectorhist.size(); j++){
        if(!AdjustHistRangeTH1F(vectorhist.at(j),factorLow,factorHigh, useBinError, &A, &B, xfirst, xlast)) continue;
        if(A<Min_global){
            if(isYLog){
                if(A>0) Min_global=A;
            }else  Min_global=A;
        }
        if(B>Max_global) Max_global=B;
        }

        if(fixRange==1) Min_global=range;
        else if(fixRange==2) Max_global=range;
        else if(fixRange!=0) cout << "ERROR*02: AdjustRange.h wrong value of parameter fixRange given (valid 0,1 or 2): " << fixRange << endl;

        for(Int_t i=0; i<(Int_t) vectorhist.size(); i++) vectorhist.at(i)->GetYaxis()->SetRangeUser(Min_global, Max_global);
    }

    return;
}

//overloaded function for AdjustHistRangeTH1F
//This function calls AdjustHistRangeTH1F for all histograms in the same vector
void AdjustHistRangeTH1F(std::vector<TH1F*> vectorhist[], Double_t factorLow, Double_t factorHigh, Int_t h, Int_t nSets, Bool_t useBinError){

    Double_t Max_global,Min_global;
    Double_t A,B;

    Bool_t successRange = AdjustHistRangeTH1F(vectorhist[0].at(h),factorLow,factorHigh, useBinError, &A, &B);

    Int_t iRange = 1;
    if(!successRange){
        while(!successRange && iRange<nSets) successRange = AdjustHistRangeTH1F(vectorhist[iRange++].at(h),factorLow,factorHigh, useBinError, &A, &B);
    }

    if(iRange>=nSets){
        cout << "ERROR in AdjustHistRangeTH1F for " << vectorhist[0].at(h)->GetName() << ", iRange = " << iRange << " >= nSets = " << nSets << ", returning..." << endl;
        return;
    }

    Min_global = A;
    Max_global = B;

    for(Int_t i=iRange; i<nSets; i++){
        if(!AdjustHistRangeTH1F(vectorhist[i].at(h),factorLow,factorHigh, useBinError, &A, &B)) continue;
        if(A<Min_global) Min_global=A;
        if(B>Max_global) Max_global=B;
    }

    for(Int_t i=0; i<nSets; i++) vectorhist[i].at(h)->GetYaxis()->SetRangeUser(Min_global, Max_global);

    return;
}

//overloaded function for AdjustHistRangeTH1F
//This function calls AdjustHistRangeTH1F for all histograms in the same vector
void AdjustHistRangeTH1F(std::vector<TH1F*> vectorhist, Double_t factorLow, Double_t factorHigh, Int_t nSets, Bool_t useBinError){

    Double_t Max_global,Min_global;
    Double_t A,B;

    Bool_t successRange = AdjustHistRangeTH1F((TH1F*)vectorhist.at(0),factorLow,factorHigh, useBinError, &A, &B);

    Int_t iRange = 1;
    if(!successRange){
        while(!successRange && iRange<nSets) successRange = AdjustHistRangeTH1F(vectorhist.at(iRange++),factorLow,factorHigh, useBinError, &A, &B);
    }

    if(iRange>nSets){
        cout << "ERROR in AdjustHistRangeTH1F, iRange>nSets, returning..." << endl;
        return;
    }

    Min_global = A;
    Max_global = B;

    for(Int_t i=iRange; i<nSets; i++){
        if(!AdjustHistRangeTH1F(vectorhist.at(i),factorLow,factorHigh, useBinError, &A, &B)) continue;
        if(A<Min_global) Min_global=A;
        if(B>Max_global) Max_global=B;
    }

    for(Int_t i=0; i<nSets; i++) vectorhist.at(i)->GetYaxis()->SetRangeUser(Min_global, Max_global);

    return;
}

//overloaded function for AdjustHistRangeTH1F
//This function calls AdjustHistRangeTH1F for all histograms in the same vector
//and returns the global max and min
//The return value is true if the range was successfully adjusted
Bool_t AdjustHistRangeTH1F(std::vector<TH1F*> vectorhist[], Double_t factorLow, Double_t factorHigh, Int_t h, Int_t nSets, Bool_t useBinError, Double_t *min, Double_t *max){

    Double_t Max_global,Min_global;
    Double_t A,B;

    Bool_t successRange = AdjustHistRangeTH1F(vectorhist[0].at(h),factorLow,factorHigh, useBinError, &A, &B);

    Int_t iRange = 1;
    if(!successRange){
        while(!successRange && iRange<nSets) successRange = AdjustHistRangeTH1F(vectorhist[iRange++].at(h),factorLow,factorHigh, useBinError, &A, &B);
    }

    if(iRange>=nSets){
        cout << "ERROR in AdjustHistRangeTH1F for " << vectorhist[0].at(h)->GetName() << ", iRange = " << iRange << " >= nSets = " << nSets << ", returning..." << endl;
        return kFALSE;
    }

    Min_global = A;
    Max_global = B;

    for(Int_t i=iRange; i<nSets; i++){
        if(!AdjustHistRangeTH1F(vectorhist[i].at(h),factorLow,factorHigh, useBinError, &A, &B)) continue;
        if(A<Min_global) Min_global=A;
        if(B>Max_global) Max_global=B;
    }

    for(Int_t i=0; i<nSets; i++) vectorhist[i].at(h)->GetYaxis()->SetRangeUser(Min_global, Max_global);

    *min = Min_global;
    *max = Max_global;

    return kTRUE;
}

Bool_t AdjustHistRangeTH1F(Bool_t isJetQA, std::vector<TH1F*> vectorhist[], Double_t factorLow, Double_t factorHigh, Int_t h, Int_t nSets, Bool_t useBinError, Double_t *min, Double_t *max){

    Double_t Max_global,Min_global;
    Double_t A,B;

    Bool_t successRange = AdjustHistRangeTH1F(vectorhist[0].at(h),factorLow,factorHigh, useBinError, &A, &B);

    Int_t iRange = 1;
    if(!successRange){
        while(!successRange && iRange<nSets) successRange = AdjustHistRangeTH1F(vectorhist[iRange++].at(h),factorLow,factorHigh, useBinError, &A, &B);
    }

    if(iRange>=nSets){
        cout << "ERROR in AdjustHistRangeTH1F for " << vectorhist[0].at(h)->GetName() << ", iRange = " << iRange << " >= nSets = " << nSets << ", returning..." << endl;
        return kFALSE;
    }

    Min_global = A;
    Max_global = B;

    for(Int_t i=iRange; i<nSets; i++){
        if(!AdjustHistRangeTH1F(vectorhist[i].at(h),factorLow,factorHigh, useBinError, &A, &B)) continue;
        if(A<Min_global) Min_global=A;
        if(B>Max_global) Max_global=B;
    }

    for(Int_t i=0; i<nSets; i++) vectorhist[i].at(h)->GetYaxis()->SetRangeUser(Min_global, Max_global);

    *min = Min_global;
    *max = Max_global;

    return kTRUE;
}
