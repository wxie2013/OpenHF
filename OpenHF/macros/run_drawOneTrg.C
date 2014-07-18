void run_drawOneTrg(int ich, int tid, int nrebin, char* finName, bool snglTrg )
{
    gSystem->Load("$CMSSW_BASE/lib/$SCRAM_ARCH/libUserCodeOpenHF.so");

    TFile* fin = new TFile(finName);

    ana_hfforest *a = new ana_hfforest;
    a->get_comb_hist(fin);
    a->drawOneTrg(ich, tid, nrebin, snglTrg);

    fin->Close();
}

