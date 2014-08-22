void run_drawOneHist(int ich, TH1* h, int nrb)
{
    gSystem->Load("$CMSSW_BASE/lib/$SCRAM_ARCH/libUserCodeOpenHF.so");

    ana_hfforest *a = new ana_hfforest;
    a->drawOneHist(ich, h, nrb);
}

