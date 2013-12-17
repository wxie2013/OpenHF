#include <fstream>
#include <iostream>

#include "TH1F.h"
#include "TFile.h"
#include "TSystem.h"
#include "TTree.h"
#include "TNtuple.h"

#include "../interface/hfcand_v0.hh"

void CompareTrees(int startFile, int endFile, char *filelist) 
{
    char* filename = new char[1000];
    sprintf(filename, "ana_hiforest_%s_%dTo%d.root",  filelist, startFile, endFile);
    TFile result(filename, "recreate");

    //
    TNtuple* nt = new TNtuple("nt", "", "runhrst:evthrst:run:evt:pt:m");

    Int_t run, evt;
    hfcand_v0* hfcandidate = new hfcand_v0;

    ifstream file_stream(filelist);
    int ifile = 0;
    while (file_stream >> filename) {

        if(ifile >=endFile) 
            break;

        if(ifile<startFile || !filename) {
            ifile++;
            continue;
        }

        ifile++;

        cout << "file  = " << ifile<<" name = "<<filename <<endl;


        TFile *f = TFile::Open(filename);
        if(!f) {
            cout<<"==> empty file <=="<<endl;
            continue;
        }
        TTree *HiTree  = (TTree*)f->Get("hiEvtAnalyzer/HiTree");
        TTree *hftree = (TTree*)f->Get("OpenHfTree/hftree");

        if(!HiTree || !hftree) {
            cout<<"==> empty tree <=="<<endl;
            continue;
        }

        long int runNum, evtNum;
        HiTree->SetBranchAddress("run",&run);
        HiTree->SetBranchAddress("evt",&evt);
        hftree->SetBranchAddress("hfcandidate", &hfcandidate);
        HiTree->AddFriend(hftree);

        Long64_t nentries = HiTree->GetEntries();
        for (Long64_t i=0;i<nentries;i++) {
            HiTree->GetEntry(i);

            //if(!hfcandidate->get_nhfcand()) {
            //    continue;
            //} 

            runNum = hfcandidate->get_runNum();
            evtNum = hfcandidate->get_evtNum();

            if (runNum != run || evtNum != evt) {
                cout<<" run: "<<run<<" runNum: "<<runNum<<" evt: "<<evt<<" evtNum: "<<evtNum<<endl;
            } 

            for(int ic = 0; ic<hfcandidate->get_nhfcand(); ic++) {
                nt->Fill(run, evt, runNum, evtNum, 
                         hfcandidate->get_hfcand(ic)->get_fpt(), 
                         hfcandidate->get_hfcand(ic)->get_fm());
            }
        }

        f->Close();
    }

    result.cd();
    nt->Write();
    result.Close();
}
