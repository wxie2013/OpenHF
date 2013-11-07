#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>

#include "../interface/TAna01Event.hh"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "map_info.hh"

using namespace std;


template <typename T> string NumberToString(T pNumber);
void get_infor(float& min, float& max, int lumi);
void write_infor(map_info* infor, int run, int min, int max, string fname);

int main(int argc, char **argv)
{
    //.. input 
    string filelist;
    int endFile = 100000, startFile = 0; //.. default value ...

    bool opt = false;
    for (int i = 0; i < argc; i++){
        if (!strcmp(argv[i],"-h")) {
            cout << "-s startFile -e endFile -c filelist   list of files" << endl;
            return 0;
        }
        if (!strcmp(argv[i],"-c"))  {
            filelist   = string(argv[++i]); 
            opt = true;
        } else if (!strcmp(argv[i],"-s"))  {
            startFile   = atoi(argv[++i]); 
        } else if (!strcmp(argv[i],"-e"))  {
            endFile   = atoi(argv[++i]); 
        } 
    }

    if(!opt) {
        cout<<"!! need to specify a file list via -c file_list !!"<<endl;
        exit(0);
    }

    //.. output file ...
    string outname = "map_run_lumi_file";
    outname += "_";
    outname += NumberToString(startFile);
    outname += "_";
    outname += NumberToString(endFile);
    outname += ".root";

    TFile outfile(outname.c_str(), "recreate");
    map_info* infor = new map_info;

    TTree* tmap = new TTree("tmap", "map among run_number, lumi region, and name of the file with HFtree to speed up hiforest production");
    
    tmap->Branch("map_info", &infor);

    //.. loop over file list ...
    TAna01Event* fpEvt = new TAna01Event(0);

    ifstream file_stream(filelist.c_str());

    char* filename = new char[1000];
    int ifile = 0;
    while (file_stream >> filename) {

        ifile++;
        if(ifile-1 >= endFile) {
            break;
        } else if(ifile-1 < startFile) {
            continue;
        }

        cout<<filename<<endl;

        //...
        TFile f(filename);


        TTree* T1 = (TTree*) f.Get("T1");

        TBranch* b_flumi = T1->GetBranch("fLumiSection");
        TBranch* b_run = T1->GetBranch("fRunNumber");
        T1->SetBranchAddress("TAna01Event", &fpEvt);

        float lumi_min = 1e10, lumi_max = -1;
        for(int i = 0; i<T1->GetEntries(); i++) {
            b_flumi->GetEntry(i);
            b_run->GetEntry(i);

            get_infor(lumi_min, lumi_max, fpEvt->fLumiSection);
        }

        fpEvt->Clear();

        write_infor(infor, fpEvt->fRunNumber, (int)lumi_min, (int)lumi_max, (string) filename); 

        tmap->Fill();
    }

    outfile.cd();
    tmap->Write();
    outfile.Close();

    return 0;
}

//_______________________________________________________
void get_infor(float& min, float& max, int lumi)
{
    if(lumi > max) 
        max = lumi;

    if(lumi < min) 
        min = lumi;
}

//_______________________________________________________
void write_infor(map_info* infor, int run, int min, int max, string fname)
{
    infor->set_run(run);
    infor->set_lumi_min(min);
    infor->set_lumi_max(max);
    infor->set_fname(fname);
}

//_____________________________________
template <typename T> string NumberToString(T pNumber)
{
    ostringstream oOStrStream;
    oOStrStream << pNumber;
    return oOStrStream.str();
}
