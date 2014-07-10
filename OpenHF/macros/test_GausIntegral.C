//par[0]/binWidth gives the total counts of the fit
// the default chi2 fit does not give correction integral
// Likelyhood fit give the correct integral. 
// details see Manual's past presentation in Purdue STAR analysis meeting
void test_GausIntegral(int N)
{
    TF1* fun = new TF1("fun", "[0]*(1/[2]/sqrt(6.28)*exp(-0.5*pow((x-[1])/[2], 2)))", -10, 10);

    fun->SetParameter(1, 0);
    fun->SetParameter(2, 2);
    fun->SetParameter(3, 100);
    h = new TH1F("h", "", 100, -10, 10);
    for(int i = 0; i<N; i++) {
        h->Fill(gRandom->Gaus(0, 2));
    }

    h->Fit(fun) ;
    cout<<" total counts from the chi2 fit is: "<<fun->GetParameter(0)/0.2<<"+/-"<<fun->GetParError(0)/0.2<<" input: "<<N<<"+/-"<<sqrt(N)<<endl;

    h->Fit(fun, "L") ;
    cout<<" total counts from the Likelyhood fit is: "<<fun->GetParameter(0)/0.2<<"+/-"<<fun->GetParError(0)/0.2<<" input: "<<N<<"+/-"<<sqrt(N)<<endl;


}

