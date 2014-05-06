#include <stdio.h>
#include <fstream>
#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "../interface/TAna01Event.hh"

using namespace std;

void remove_pPb2013_misaligned_runs(int startFile, int endFile, char *filelist)
{
    char* filename = new char[1000];
    sprintf(filename, "remove_%s_%dTo%d.lis",  filelist, startFile, endFile);
    FILE* fp = fopen(filename, "w");


    ifstream file_stream(filelist);
    int ifile = 0;
    Long64_t run = 0;
    Int_t lumi = 0;

    TAna01Event* fpEvt = new TAna01Event(0);

    while (file_stream >> filename) {

        if(ifile >=endFile)
            break;

        if(ifile<startFile || !filename) {
            ifile++;
            continue;
        }

        ifile++;

        cout<<ifile<<" "<<filename<<endl;

        TFile *f = TFile::Open(filename);
        if(!f) {
            cout<<"==> empty file <=="<<endl;
            continue;
        }

        TTree* T1 = 0;
        if(!(T1 = (TTree*) f->Get("T1")))
            T1 = (TTree*) f->Get("tree/fTree");

        if(!T1) continue;

        T1->SetBranchAddress("TAna01Event", &fpEvt);

        TBranch* b_run = T1->GetBranch("fRunNumber");
        TBranch* b_lum = T1->GetBranch("fLumiSection");


        Long64_t nentries = T1->GetEntries();
        for (Long64_t i=0;i<nentries;i++) {
            b_run->GetEntry(i);
            b_lum->GetEntry(i);

            run = static_cast<long int>(fpEvt->fRunNumber);
            lumi = static_cast<long int>(fpEvt->fLumiSection);

            if(run==210498 && lumi>=121 && lumi<=351 ||
                    run==210534 && lumi>=24 && lumi<=347 ||
                    run==210614 && (lumi>=101 && lumi<=505 || lumi>=507 && lumi<=1652) ||
                    run==210634 && lumi>=97 && lumi<=793||
                    run==210635 && lumi>=1 && lumi<=850 ||
                    run==210638 && lumi>=3 && lumi<=187 ||
                    run==210658 && lumi>=100 && lumi<=1550) {

                fprintf(fp, "%s\n", filename); 

                break;
            }
        }

        f->Close();
    }
}
