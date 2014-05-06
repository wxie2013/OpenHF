#define events_cxx

#include <fstream>
#include <iostream>

#include "TH1F.h"
#include "TFile.h"

#include "events.h"

const short nhist = 20; //.. number of histograms
const float delta_pt = 10; //.. 10 GeV per bin
const int nbin = 200;

using namespace std;

void plot(int startFile, int endFile, char *filelist, char* folderName, 
        float cut_m_low, float cut_m_high, 
        float cut_m_dau_tight_low, float cut_m_dau_tight_high,
        float cut_m_dau_loose_low, float cut_m_dau_loose_high,
        float cut_ffls3d = 2, float cut_falpha0 = 0.5, float cut_fprob = 0.05, 
        float cut_fdr = 0.25, float cut_fchi2 = 2.0, 
        int nseg = 1000000)
{
    //.. output file ...
    char* treeLocal = new char[1000];//.. tree location and name ...
    char* filename = new char[1000];
    char* hname = new char[1000];
    char* pt_range = new char[1000];

    sprintf(filename, "ana_%s_%s_%dTo%d.root", folderName, filelist, startFile, endFile);

    TFile result(filename, "recreate");

    //.histogram
    TH1* hfg[nhist] = {0};
    TH1* hbg[nhist] = {0};
    TH1* hdau[nhist] = {0};
    TH1* hfgdiff[nhist] = {0};
    TH1* hbgdiff[nhist] = {0};
    for(short i = 0; i<nhist; i++) {
        float pt_low = i*delta_pt;
        float pt_high = pt_low +  delta_pt;

        sprintf(hname, "hfg%d", i);
        sprintf(pt_range, "pt: %f %f", pt_low, pt_high);
        hfg[i] = new TH1F(hname, pt_range, nbin, cut_m_low, cut_m_high);

        sprintf(hname, "hbg%d", i);
        hbg[i] = new TH1F(hname, pt_range, nbin, cut_m_low, cut_m_high);

        sprintf(hname, "hdau%d", i);
        hdau[i] = new TH1F(hname, pt_range, nbin, cut_m_dau_loose_low, cut_m_dau_loose_high);

        sprintf(hname, "hfgdiff%d", i);
        hfgdiff[i] = new TH1F(hname, pt_range, nbin, 0.135, 0.154);

        sprintf(hname, "hbgdiff%d", i);
        hbgdiff[i] = new TH1F(hname, pt_range, nbin, 0.135, 0.154);

        //.. track error ...
        hfg[i]->Sumw2();
        hbg[i]->Sumw2();
        hdau[i]->Sumw2();
        hfgdiff[i]->Sumw2();
        hbgdiff[i]->Sumw2();
    }

    //..loop over files ...
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


        //..the files are in dcache ..
        TFile *f = TFile::Open(filename);
        //

        sprintf(treeLocal, "%s/events", folderName);
        TTree *tr = (TTree*)f->Get(treeLocal);

        events t(tr);

        for(int i = 0; i<tr->GetEntries(); i++) {
            t.GetEntry(i);

            if(!(i%nseg)) 
                cout<<i<<"  of "<<tr->GetEntries()<<endl;

            int ih = t.fpt/delta_pt;
            if(ih >= nhist)
                ih = nhist - 1;  //.. the last bin include all higher pT region...

            //.. assign mass from different particles...
            float mass = 0, mass_dau = 0, alpha = 9999;
            if(!strcmp(folderName, "candAnaD0")) {
                mass = t.fmdz;
                mass_dau = 0;
                alpha = t.falpha0;
            } else if(!strcmp(folderName, "candAnaDstar")) {
                mass = t.fmds;
                mass_dau = t.fmdz;
                alpha = t.falpha;
            } else if(!strcmp(folderName, "candAnaDpm2KaPiPi")) {
                mass = t.fmdpm;
                mass_dau = t.fmkpi;
                alpha = t.falpha0;
            } else if(!strcmp(folderName, "candAnaDs2PhiPi")) {
                mass = t.fmds;
                mass_dau = t.fmPhi;
                alpha = t.falpha0;
            } else if(!strcmp(folderName, "candAnaDs2KstarKaon")) {
                mass = t.fmds;
                mass_dau = t.fmkpi;
                alpha = t.falpha0;
            } else {
                cout<<"!!!  no such particles. Exit !!!"<<endl;
                exit(0);
            }

            if(mass > cut_m_low && mass < cut_m_high && 
               t.ffls3d > cut_ffls3d && alpha < cut_falpha0 && 
               t.fprob > cut_fprob && t.fdr < cut_fdr && t.fchi2 < cut_fchi2) {

                //.. forground ..
                if(mass_dau > cut_m_dau_loose_low && mass_dau < cut_m_dau_loose_high) {
                    hdau[ih]->Fill(mass_dau);

                    if(mass_dau > cut_m_dau_tight_low && mass_dau < cut_m_dau_tight_high) {
                        hfg[ih]->Fill(mass);
                        hfgdiff[ih]->Fill(mass-mass_dau);
                    }
                }

                //.. background through side band ...
                if(mass_dau < cut_m_dau_tight_low || mass_dau > cut_m_dau_tight_high) {
                    hbg[ih]->Fill(mass);
                    hbgdiff[ih]->Fill(mass-mass_dau);
                }
            }
        }

        delete f;
    }

    //.. normalization of through side bands...


    //.. save files ...
    result.cd();
    for(short i = 0; i<nhist; i++) {
        hfg[i]->Write();
        hbg[i]->Write();
        hdau[i]->Write();
        hfgdiff[i]->Write();
        hbgdiff[i]->Write();
    }
    result.Close();
}
