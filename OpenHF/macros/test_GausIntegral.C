//par[0]/binWidth gives the total counts of the fit
// the default chi2 fit does not give correction integral
// Likelyhood fit give the correct integral. 
// details see Manual's past presentation in Purdue STAR analysis meeting
void test_GausIntegral(int N, float scale)
{
    TF1* fun = new TF1("fun", "[0]*(1/[2]/sqrt(6.28)*exp(-0.5*pow((x-[1])/[2], 2)))", -10, 10);

    fun->SetParameter(0, 100);
    fun->SetParameter(1, 0);
    fun->SetParameter(2, 2);

    h = new TH1F("h", "", 100, 0, 10);
    h->Sumw2();
    for(int i = 0; i<N; i++) {
            h->Fill(gRandom->Gaus(5, 0.5));
    }
    if(scale) 
        h->Scale(scale);
    h->Fit(fun) ;
    cout<<" total counts from the chi2 fit is: "<<fun->GetParameter(0)/h->GetBinWidth(1)<<"+/-"<<fun->GetParError(0)/h->GetBinWidth(1)<<" input: "<<N*scale<<"+/-"<<sqrt(N)*scale<<endl;

    h->Fit(fun, "WLQ") ;
    cout<<" total counts from the Likelyhood fit is: "<<fun->GetParameter(0)/h->GetBinWidth(1)<<"+/-"<<fun->GetParError(0)/h->GetBinWidth(1)<<" input: "<<N*scale<<"+/-"<<sqrt(N)*scale<<endl;
}

