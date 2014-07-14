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
void mapRunLumiHfTreeFile::Init(int startFile, int endFile, const char* trgName)
{
    char outname[200];
    sprintf(outname, "%s%s%s%d%s%d%s", "mapRunLumiHfTreeFile_", trgName, "_", startFile, "To", endFile, ".root");
    outfile = new TFile(outname, "recreate");

    infor = new map_info;

    tmap = new TTree("tmap", "map among run_number, lumi region, and name of the file with HFtree to speed up hiforest production");

    tmap->Branch("map_info", &infor);

    //.. loop over file list ...
    fpEvt = new TAna01Event(0);

}
//--------------------------
void mapRunLumiHfTreeFile::LoopOverFile(int startFile, int endFile, char *filelist, const char* trgName)
{
    Init(startFile, endFile, trgName);

    //..
    ifstream file_stream(filelist);


    int ifile = 0;
    vector<int> listOfLumis;
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

        TTree* T1 = 0; 
        if(!(T1 = (TTree*) f->Get("T1")))
                T1 = (TTree*) f->Get("tree/fTree");

        if(!T1) {
            cout<<" no T1 tree "<<endl;
            continue;
        }
        fpEvt->Clear();

        TBranch* b_flumi = T1->GetBranch("fLumiSection");
        TBranch* b_run = T1->GetBranch("fRunNumber");
        T1->SetBranchAddress("TAna01Event", &fpEvt);

        int run = -1;
        int last_run=-1;
        float lumi_min = 1e10, lumi_max = -1;
        listOfLumis.clear();
        for(int i = 0; i<T1->GetEntries(); i++) {
            b_flumi->GetEntry(i);
            b_run->GetEntry(i);

            run = fpEvt->fRunNumber;

            if(run!=last_run && i!=0) {

                write_infor(infor, last_run, (int)lumi_min, (int)lumi_max, filename, listOfLumis); 
                tmap->Fill();

                lumi_min = 1e10;
                lumi_max = -1;
                last_run=run;
                listOfLumis.clear();
                get_infor(lumi_min, lumi_max, fpEvt->fLumiSection);
                fill_listOfLumis(fpEvt->fLumiSection, listOfLumis);
            } else {
                last_run=run;
                get_infor(lumi_min, lumi_max, fpEvt->fLumiSection);
                fill_listOfLumis(fpEvt->fLumiSection, listOfLumis);
            }
        }

        // for the last run in the file
        write_infor(infor, run, (int)lumi_min, (int)lumi_max, filename, listOfLumis); 
        tmap->Fill();

        f->Close();
    }

    outfile->cd();
    tmap->Write();
    outfile->Close();

}
//--------------------------
void mapRunLumiHfTreeFile::LoopOverFileHlt(int startFile, int endFile, char *filelist, const char* trgName)
{
    Init(startFile, endFile, trgName);

    //..
    ifstream file_stream(filelist);


    int ifile = 0;
    vector<int> listOfLumis;
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

        TTree* T1 = 0; 
        T1 = (TTree*) f->Get("hltanalysis/HltTree");

        if(!T1) {
            cout<<" no T1 tree "<<endl;
            continue;
        }

        int run = -1, last_run=-1, lumi = -1;
        T1->SetBranchAddress("Run", &run);
        T1->SetBranchAddress("LumiBlock", &lumi);
        TBranch* b_flumi = T1->GetBranch("LumiBlock");
        TBranch* b_run = T1->GetBranch("Run");

        float lumi_min = 1e10, lumi_max = -1;
        listOfLumis.clear();
        for(int i = 0; i<T1->GetEntries(); i++) {
            b_flumi->GetEntry(i);
            b_run->GetEntry(i);

            if(run!=last_run && i!=0) {
                write_infor(infor, last_run, (int)lumi_min, (int)lumi_max, filename, listOfLumis); 
                tmap->Fill();

                lumi_min = 1e10;
                lumi_max = -1;
                last_run=run;
                listOfLumis.clear();
                get_infor(lumi_min, lumi_max, lumi);
                fill_listOfLumis(lumi, listOfLumis);
            } else {
                last_run=run;
                get_infor(lumi_min, lumi_max, lumi);
                fill_listOfLumis(lumi, listOfLumis);
            }
        }
        // for the last run in the file..
        write_infor(infor, last_run, (int)lumi_min, (int)lumi_max, filename, listOfLumis); 
        tmap->Fill();

        f->Close();
    }

    outfile->cd();
    tmap->Write();
    outfile->Close();

}
//_______________________________________________________
void mapRunLumiHfTreeFile::fill_listOfLumis(int lumi, vector<int>& list)
{
    for(unsigned int i = 0; i<list.size(); i++) {
        if(lumi == list.at(i)) 
            return;
    }
    list.push_back(lumi);
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
void mapRunLumiHfTreeFile::write_infor(map_info* infor, int run, int min, int max, string& fname, vector<int>& list)
{
    infor->set_run(run);
    infor->set_lumi_min(min);
    infor->set_lumi_max(max);
    infor->set_fname(fname);
    infor->set_listOfLumis(list);
}
