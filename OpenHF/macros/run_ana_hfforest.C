void run_ana_hfforest(int startFile, int endFile, char *filelist, int iy, int ich, char* outfile, bool ppTrk)
{
    //gSystem->Load("/home/wxie/CMSSW_6_2_9/lib/slc6_amd64_gcc472/libUserCodeOpenHF.so")

    ana_hfforest *a = new ana_hfforest;
    a->set_run_ppTrack(ppTrk);
    a->LoopOverFile(startFile, endFile, filelist, iy, ich, outfile);

    //delete a; 
}
