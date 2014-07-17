#include "../interface/map_info.hh"

ClassImp(map_info)

map_info::map_info()
{
    reset();
}
//
//
void map_info::reset()
{
    run = -1;
    lumi_min = -1;
    lumi_max = -1;
    fname = "";
    listOfLumis.clear();
}
