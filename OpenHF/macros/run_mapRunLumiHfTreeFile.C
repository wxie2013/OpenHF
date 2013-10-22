void run_mapRunLumiHfTreeFile(int startFile, int endFile, char *filelist)
{
    gSystem->Load("../../../../lib/slc5_amd64_gcc462/libUserCodeOpenHF.so");

    mapRunLumiHfTreeFile *a = new mapRunLumiHfTreeFile;

    a->set_pathname("/mnt/hadoop/store/user/wxie/PAHighPt/HFTree/7fdf5ec06bc167d7c907e513d3ce2df4");
    a->LoopOverFile(startFile, endFile, filelist);

    delete a; // so we can dump some information in the destructor
}
