void run_drawOneTrg(int ich, int nrebin, char* finName, bool snglTrg, int it )
{
    gSystem->Load("$CMSSW_BASE/lib/$SCRAM_ARCH/libUserCodeOpenHF.so");

    TFile* fin = new TFile(finName);
    TCanvas* cc = new TCanvas("cc", "", 1000, 800);
    ana_hfforest *a = new ana_hfforest;
    a->get_comb_hist(ich, fin);
    a->drawOneTrg(ich, nrebin, snglTrg, it, cc);

    fin->Close();
}

