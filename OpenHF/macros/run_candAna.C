void run_candAna(int startFile, int endFile, char *filelist)
{
    gSystem->Load("../../../../lib/slc5_amd64_gcc462/libUserCodeOpenHF.so");

    candAna *a = new candAna;

    //a->set_cut(2, 0.5);
    a->LoopOverFile(startFile, endFile, filelist);

    delete a; // so we can dump some information in the destructor
}
