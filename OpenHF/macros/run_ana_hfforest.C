void run_ana_hfforest(int startFile, int endFile, char *filelist, int iy, int ich, char* outfile)
{
    ana_hfforest *a = new ana_hfforest;

    a->LoopOverFile(startFile, endFile, filelist, iy, ich, outfile);

    //delete a; 
}
