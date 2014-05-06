#include "../interface/checkTrgMatch.hh"

void run_checkTrgMatch(int startFile, int endFile, char *filelist)
{
    checkTrgMatch *a = new checkTrgMatch;

    a->LoopOverFile(startFile, endFile, filelist);

    //delete a; 
}
