void run_ana_hfforest(int startFile, int endFile, char *filelist)
{
    gSystem->Load("../../../../lib/slc5_amd64_gcc462/libUserCodeOpenHF.so");

    ana_hfforest *a = new ana_hfforest;

    a->LoopOverFile(startFile, endFile, filelist);

    delete a; 
}
