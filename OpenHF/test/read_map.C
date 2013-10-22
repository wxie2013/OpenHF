#include <iostream>
#include <map>
#include <TFile.h>
#include <TSystem.h>
#include <TTree.h>
#include "../interface/map_info.hh"

using namespace std;
struct compRun
{
    bool operator()(const int r1, const int r2) const
    {
        return r1 < r2;
    }
};

void read_map()
{
    //gSystem->Load("../../../../lib/slc5_amd64_gcc462/libUserCodeOpenHF.so");

    map_info* a = new map_info;

    TFile f("mapRunLumiHfTreeFile.root");

    TTree* tmap = (TTree*)f.Get("tmap");
    
    tmap->SetBranchAddress("map_info", &a);

    multimap<int, map_info*, compRun> listOfHFTreeFile;

    //for (int i = 0; i<tmap->GetEntries(); i++) {
    for (int i = 0; i<5; i++) {
        tmap->GetEntry(i);

        int run = a->get_run();

        listOfHFTreeFile.insert(pair<int, map_info*>(run, a));
        for(multimap<int, map_info*>::iterator tt = listOfHFTreeFile.begin(); tt!=listOfHFTreeFile.end(); tt++) {
        cout<<"1__"<<(*tt).first<<" "<<(*tt).second->get_run()<<" "<<(*tt).second->get_fname()<<endl;
        }

        cout<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
    }
    cout<<"==>"<<endl;

    for(multimap<int, map_info*>::iterator tt = listOfHFTreeFile.begin(); tt!=listOfHFTreeFile.end(); tt++) {
        cout<<"1__"<<(*tt).first<<" "<<(*tt).second->get_run()<<" "<<(*tt).second->get_fname()<<endl;
    }

}

