void run_ana_hfforest(int startFile, int endFile, char *filelist)
{
    ana_hfforest *a = new ana_hfforest;

    a->LoopOverFile(startFile, endFile, filelist);

    //delete a; 
}
