#include "../interface/FindUsefulTrigger.hh"

void run_FindUsefulTrigger(int startFile, int endFile, char *filelist)
{
    FindUsefulTrigger *a = new FindUsefulTrigger;

    a->LoopOverFile(startFile, endFile, filelist);

    //delete a; 
}
