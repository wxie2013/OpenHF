#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>

#include "../interface/mapRunLumiHfTreeFile.hh"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"

using namespace std;

ClassImp(mapRunLumiHfTreeFile)

mapRunLumiHfTreeFile::mapRunLumiHfTreeFile()
{
}

//--------------------------
void mapRunLumiHfTreeFile::Init(int startFile, int endFile)
{
    char outname[200];
    sprintf(outname, "%s%d%s%d%s", "mapRunLumiHfTreeFile_", startFile, "To", endFile, ".root");
    outfile = new TFile(outname, "recreate");

    infor = new map_info;

    tmap = new TTree("tmap", "map among run_number, lumi region, and name of the file with HFtree to speed up hiforest production");

    tmap->Branch("map_info", &infor);

    //.. loop over file list ...
    fpEvt = new TAna01Event(0);

}
//--------------------------
void mapRunLumiHfTreeFile::LoopOverFile(int startFile, int endFile, char *filelist)
{
    Init(startFile, endFile);

    //..
    ifstream file_stream(filelist);


    int ifile = 0;
    while (file_stream >> filename) {

        ifile++;
        if(ifile-1 >= endFile) {
            break;
        } else if(ifile-1 < startFile) {
            continue;
        }

        //
        path_file = pathname+"/"+filename;
        cout<<ifile<<": "<<path_file<<endl;

        //...
        TFile* f = TFile::Open(path_file.c_str());
        if(!f) {
            cout<<"==> no T1 tree available <=="<<endl;
            continue;
        }

        TTree* T1 = (TTree*) f->Get("T1");
        fpEvt->Clear();

        TBranch* b_flumi = T1->GetBranch("fLumiSection");
        TBranch* b_run = T1->GetBranch("fRunNumber");
        T1->SetBranchAddress("TAna01Event", &fpEvt);

        float lumi_min = 1e10, lumi_max = -1;
        for(int i = 0; i<T1->GetEntries(); i++) {
            b_flumi->GetEntry(i);
            b_run->GetEntry(i);

            get_infor(lumi_min, lumi_max, fpEvt->fLumiSection);
        }


        write_infor(infor, fpEvt->fRunNumber, (int)lumi_min, (int)lumi_max, filename); 

        tmap->Fill();
    }

    outfile->cd();
    tmap->Write();
    outfile->Close();

}

//_______________________________________________________
void mapRunLumiHfTreeFile::get_infor(float& min, float& max, int lumi)
{
    if(lumi > max) 
        max = lumi;

    if(lumi < min) 
        min = lumi;
}

//_______________________________________________________
void mapRunLumiHfTreeFile::write_infor(map_info* infor, int run, int min, int max, string& fname)
{
    infor->set_run(run);
    infor->set_lumi_min(min);
    infor->set_lumi_max(max);
    infor->set_fname(fname);
}
