#include <fstream>
#include <iostream>

#include "TH1F.h"
#include "TFile.h"
#include "TF1.h"
#include "TSystem.h"
#include "TTree.h"
#include "TNtuple.h"
#include "TBranch.h"

#include "../interface/hfcand_v0.hh"
#include "../interface/ana_hfforest.hh"

ClassImp(ana_hfforest)

// for unknown reason this can not be placed in ana_common.hh
const char* MesonName[NCH] = {"Dstar2D0Pi", "D02KPi", "Ds2PhiPi", "Ds2KstarK", "Dpm2KPiPi"};

ana_hfforest::ana_hfforest()
{;}

//
ana_hfforest::~ana_hfforest()
{
    delete filename;
    delete result;
    delete hfcandidate;

    for(int i = 0; i<NTRG; i++) 
        delete trg_obj[i];

    for(int itrg = 0; itrg<NTRG; itrg++) 
        for(short i = 0; i<NPT; i++) {
            delete hfg_raw[itrg][i];
            delete hfg[itrg][i];
            delete hfgdiff_raw[itrg][i];
            delete hfgdiff[itrg][i];
        }

    for(short i = 0; i<NPT; i++) {
        //FullTrack trigger ...
        delete hfg_ZBiasSglTrkPt0_12_raw[i];
        delete hfg_ZBiasSglTrkPt0_12[i];

        delete hfg_TrkTrgPt12_20_raw[i];
        delete hfg_TrkTrgPt12_20[i];
        delete hfg_TrkTrgPt20_30_raw[i];
        delete hfg_TrkTrgPt20_30[i];
        delete hfg_TrkTrgPt30above_raw[i];
        delete hfg_TrkTrgPt30above[i];

        // Jet trigger ..
        delete hfg_ZBiasSglTrkPt0_20_raw[i];
        delete hfg_ZBiasSglTrkPt0_20[i];

        delete hfg_JetTrgPt20_40_raw[i];
        delete hfg_JetTrgPt20_40[i];
        delete hfg_JetTrgPt40_60_raw[i];
        delete hfg_JetTrgPt40_60[i];
        delete hfg_JetTrgPt60_80_raw[i];
        delete hfg_JetTrgPt60_80[i];
        delete hfg_JetTrgPt80_100_raw[i];
        delete hfg_JetTrgPt80_100[i];
        delete hfg_JetTrgPt100above_raw[i];
        delete hfg_JetTrgPt100above[i];

        //.. photon trigger 
        delete hfg_ZBiasSglTrkPt0_10_raw[i];
        delete hfg_ZBiasSglTrkPt0_10[i];

        delete hfg_PhoTrgPt10_15_raw[i];
        delete hfg_PhoTrgPt10_15[i];
        delete hfg_PhoTrgPt15_20_raw[i];
        delete hfg_PhoTrgPt15_20[i];
        delete hfg_PhoTrgPt20_30_raw[i];
        delete hfg_PhoTrgPt20_30[i];
        delete hfg_PhoTrgPt30above_raw[i];
        delete hfg_PhoTrgPt30above[i];

        //.. track trigger 
        delete hfgdiff_ZBiasSglTrkPt0_12_raw[i];
        delete hfgdiff_ZBiasSglTrkPt0_12[i];

        delete hfgdiff_TrkTrgPt12_20_raw[i];
        delete hfgdiff_TrkTrgPt12_20[i];
        delete hfgdiff_TrkTrgPt20_30_raw[i];
        delete hfgdiff_TrkTrgPt20_30[i];
        delete hfgdiff_TrkTrgPt30above_raw[i];
        delete hfgdiff_TrkTrgPt30above[i];

        delete hbgdiff_ZBiasSglTrkPt0_12_raw[i];
        delete hbgdiff_ZBiasSglTrkPt0_12[i];

        delete hbgdiff_TrkTrgPt12_20_raw[i];
        delete hbgdiff_TrkTrgPt12_20[i];
        delete hbgdiff_TrkTrgPt20_30_raw[i];
        delete hbgdiff_TrkTrgPt20_30[i];
        delete hbgdiff_TrkTrgPt30above_raw[i];
        delete hbgdiff_TrkTrgPt30above[i];

        //. Jet trigger 
        delete hfgdiff_ZBiasSglTrkPt0_20_raw[i];
        delete hfgdiff_ZBiasSglTrkPt0_20[i];

        delete hfgdiff_JetTrgPt20_40_raw[i];
        delete hfgdiff_JetTrgPt20_40[i];
        delete hfgdiff_JetTrgPt40_60_raw[i];
        delete hfgdiff_JetTrgPt40_60[i];
        delete hfgdiff_JetTrgPt60_80_raw[i];
        delete hfgdiff_JetTrgPt60_80[i];
        delete hfgdiff_JetTrgPt80_100_raw[i];
        delete hfgdiff_JetTrgPt80_100[i];
        delete hfgdiff_JetTrgPt100above_raw[i];
        delete hfgdiff_JetTrgPt100above[i];

        delete hbgdiff_ZBiasSglTrkPt0_20_raw[i];
        delete hbgdiff_ZBiasSglTrkPt0_20[i];

        delete hbgdiff_JetTrgPt20_40_raw[i];
        delete hbgdiff_JetTrgPt20_40[i];
        delete hbgdiff_JetTrgPt40_60_raw[i];
        delete hbgdiff_JetTrgPt40_60[i];
        delete hbgdiff_JetTrgPt60_80_raw[i];
        delete hbgdiff_JetTrgPt60_80[i];
        delete hbgdiff_JetTrgPt80_100_raw[i];
        delete hbgdiff_JetTrgPt80_100[i];
        delete hbgdiff_JetTrgPt100above_raw[i];
        delete hbgdiff_JetTrgPt100above[i];

        //. photon trigger 
        delete hfgdiff_ZBiasSglTrkPt0_10_raw[i];
        delete hfgdiff_ZBiasSglTrkPt0_10[i];

        delete hfgdiff_PhoTrgPt10_15_raw[i];
        delete hfgdiff_PhoTrgPt10_15[i];
        delete hfgdiff_PhoTrgPt15_20_raw[i];
        delete hfgdiff_PhoTrgPt15_20[i];
        delete hfgdiff_PhoTrgPt20_30_raw[i];
        delete hfgdiff_PhoTrgPt20_30[i];
        delete hfgdiff_PhoTrgPt30above_raw[i];
        delete hfgdiff_PhoTrgPt30above[i];

        delete hbgdiff_ZBiasSglTrkPt0_10_raw[i];
        delete hbgdiff_ZBiasSglTrkPt0_10[i];

        delete hbgdiff_PhoTrgPt10_15_raw[i];
        delete hbgdiff_PhoTrgPt10_15[i];
        delete hbgdiff_PhoTrgPt15_20_raw[i];
        delete hbgdiff_PhoTrgPt15_20[i];
        delete hbgdiff_PhoTrgPt20_30_raw[i];
        delete hbgdiff_PhoTrgPt20_30[i];
        delete hbgdiff_PhoTrgPt30above_raw[i];
        delete hbgdiff_PhoTrgPt30above[i];
    }
}

//
bool ana_hfforest::checkRapidityBoundary(float y) 
{
    if(y<Ymin || y>Ymin+NY*dY) {
        //cout<<"!! rapidity: "<<y<<" beyond "<<Ymin<<"--"<<Ymin+NY*dY<<". !!!"<<endl;
        return false;
    }

    return true;
}

//
void ana_hfforest::Init(int startFile, int endFile, char *filelist, int iy, int ich, char* outfile)
{//.. called right after constructor 
    filename = new char[1000];
    float ydn = iy*dY + Ymin; 
    float yup = ydn+dY;
    if(!checkRapidityBoundary(ydn) || !checkRapidityBoundary(yup)) 
        exit(0);

    //sprintf(filename, "ana_hhforest_%s_%dTo%d_rapidity_%3.1f_%3.1f_%s.root",  filelist, startFile, endFile, ydn, yup, MesonName[ich]);
    result = new TFile(outfile, "recreate");

    //
    define_cuts();
    get_trg_name();
    book_hist(ich, iy);

    //
    hfcandidate = new hfcand_v0;

    //HiTree = 0;
    HltTree = 0;
    hltTreeReRun = 0;
    hftree = 0;

    //.. trigger object ...
    for(int i = 0; i<NTRG; i++) {
        trg_obj[i] = new trigO;
    }

}

//
void ana_hfforest::book_hist(int ich, int iy)
{
    float pt_low = 0, pt_high = 0;
    char hname[100], pt_range[1000];

    // all D mesons
    for(short it = 0; it<NTRG; it++) 
        for(short i = 0; i<NPT; i++) {

            get_pt_range(i, pt_low, pt_high);

            sprintf(pt_range, "trg: %s,  pt: %f %f", trg_name[it], pt_low, pt_high);

            //
            sprintf(hname, "hfg%d_%d", it, i);
            hfg[it][i] = new TH1F(hname, pt_range, NBIN, cut_m_low[ich], cut_m_high[ich]);
            sprintf(hname, "hfg_raw%d_%d", it, i);
            hfg_raw[it][i] = new TH1F(hname, pt_range, NBIN, cut_m_low[ich], cut_m_high[ich]);

            //.. track error ...
            hfg[it][i]->Sumw2();
            hfg_raw[it][i]->Sumw2();
        }

    // D*
    for(short it = 0; it<NTRG; it++) 
        for(short i = 0; i<NPT; i++) {

            get_pt_range(i, pt_low, pt_high);

            sprintf(pt_range, "trg: %s,  pt: %f %f", trg_name[it], pt_low, pt_high);

            sprintf(hname, "hfgdiff%d_%d", it, i);
            hfgdiff[it][i] = new TH1F(hname, pt_range, NBIN, mdiffup, mdiffdn);
            sprintf(hname, "hfgdiff_raw%d_%d", it, i);
            hfgdiff_raw[it][i] = new TH1F(hname, pt_range, NBIN, mdiffup, mdiffdn);

            sprintf(hname, "hbgdiff%d_%d", it, i);
            hbgdiff[it][i] = new TH1F(hname, pt_range, NBIN, mdiffup, mdiffdn);
            sprintf(hname, "hbgdiff_raw%d_%d", it, i);
            hbgdiff_raw[it][i] = new TH1F(hname, pt_range, NBIN, mdiffup, mdiffdn);

            //.. track error ...
            hfgdiff[it][i]->Sumw2();
            hbgdiff[it][i]->Sumw2();
            hfgdiff_raw[it][i]->Sumw2();
            hbgdiff_raw[it][i]->Sumw2();
        }

    // components for trigger combination for all D..
    for(short i = 0; i<NPT; i++) {

        get_pt_range(i, pt_low, pt_high);

        //FullTrack trigger 
        sprintf(pt_range, "pt: %f %f", pt_low, pt_high);

        sprintf(hname, "hfg_ZBiasSglTrkPt0_12_raw%d", i);
        hfg_ZBiasSglTrkPt0_12_raw[i] = new TH1F(hname, pt_range, NBIN, cut_m_low[ich], cut_m_high[ich]);
        sprintf(hname, "hfg_ZBiasSglTrkPt0_12%d", i);
        hfg_ZBiasSglTrkPt0_12[i] = new TH1F(hname, pt_range, NBIN, cut_m_low[ich], cut_m_high[ich]);

        sprintf(hname, "hfg_TrkTrgPt12_raw%d", i);
        hfg_TrkTrgPt12_raw[i] = new TH1F(hname, pt_range, NBIN, cut_m_low[ich], cut_m_high[ich]);

        sprintf(hname, "hfg_TrkTrgPt12_20_raw%d", i);
        hfg_TrkTrgPt12_20_raw[i] = new TH1F(hname, pt_range, NBIN, cut_m_low[ich], cut_m_high[ich]);
        sprintf(hname, "hfg_TrkTrgPt12_20%d", i);
        hfg_TrkTrgPt12_20[i] = new TH1F(hname, pt_range, NBIN, cut_m_low[ich], cut_m_high[ich]);

        sprintf(hname, "hfg_TrkTrgPt20_30_raw%d", i);
        hfg_TrkTrgPt20_30_raw[i] = new TH1F(hname, pt_range, NBIN, cut_m_low[ich], cut_m_high[ich]);
        sprintf(hname, "hfg_TrkTrgPt20_30%d", i);
        hfg_TrkTrgPt20_30[i] = new TH1F(hname, pt_range, NBIN, cut_m_low[ich], cut_m_high[ich]);

        sprintf(hname, "hfg_TrkTrgPt30above_raw%d", i);
        hfg_TrkTrgPt30above_raw[i] = new TH1F(hname, pt_range, NBIN, cut_m_low[ich], cut_m_high[ich]);
        sprintf(hname, "hfg_TrkTrgPt30above%d", i);
        hfg_TrkTrgPt30above[i] = new TH1F(hname, pt_range, NBIN, cut_m_low[ich], cut_m_high[ich]);

        hfg_ZBiasSglTrkPt0_12_raw[i]->Sumw2();
        hfg_ZBiasSglTrkPt0_12[i]->Sumw2();

        hfg_TrkTrgPt12_20_raw[i]->Sumw2();
        hfg_TrkTrgPt12_20[i]->Sumw2();

        hfg_TrkTrgPt20_30_raw[i]->Sumw2();
        hfg_TrkTrgPt20_30[i]->Sumw2();

        hfg_TrkTrgPt30above_raw[i]->Sumw2();
        hfg_TrkTrgPt30above[i]->Sumw2();

        //Jet trigger 
        sprintf(pt_range, "ch: %s, rapidity:%d, pt: %f %f", MesonName[ich], iy, pt_low, pt_high);

        sprintf(hname, "hfg_ZBiasSglTrkPt0_20_raw%d", i);
        hfg_ZBiasSglTrkPt0_20_raw[i] = new TH1F(hname, pt_range, NBIN, cut_m_low[ich], cut_m_high[ich]);
        sprintf(hname, "hfg_ZBiasSglTrkPt0_20%d", i);
        hfg_ZBiasSglTrkPt0_20[i] = new TH1F(hname, pt_range, NBIN, cut_m_low[ich], cut_m_high[ich]);

        sprintf(hname, "hfg_JetTrgPt20_raw%d", i);
        hfg_JetTrgPt20_raw[i] = new TH1F(hname, pt_range, NBIN, cut_m_low[ich], cut_m_high[ich]);

        sprintf(hname, "hfg_JetTrgPt20_40_raw%d", i);
        hfg_JetTrgPt20_40_raw[i] = new TH1F(hname, pt_range, NBIN, cut_m_low[ich], cut_m_high[ich]);
        sprintf(hname, "hfg_JetTrgPt20_40%d", i);
        hfg_JetTrgPt20_40[i] = new TH1F(hname, pt_range, NBIN, cut_m_low[ich], cut_m_high[ich]);

        sprintf(hname, "hfg_JetTrgPt40_60_raw%d", i);
        hfg_JetTrgPt40_60_raw[i] = new TH1F(hname, pt_range, NBIN, cut_m_low[ich], cut_m_high[ich]);
        sprintf(hname, "hfg_JetTrgPt40_60%d", i);
        hfg_JetTrgPt40_60[i] = new TH1F(hname, pt_range, NBIN, cut_m_low[ich], cut_m_high[ich]);

        sprintf(hname, "hfg_JetTrgPt60_80_raw%d", i);
        hfg_JetTrgPt60_80_raw[i] = new TH1F(hname, pt_range, NBIN, cut_m_low[ich], cut_m_high[ich]);
        sprintf(hname, "hfg_JetTrgPt60_80%d", i);
        hfg_JetTrgPt60_80[i] = new TH1F(hname, pt_range, NBIN, cut_m_low[ich], cut_m_high[ich]);

        sprintf(hname, "hfg_JetTrgPt80_100_raw%d", i);
        hfg_JetTrgPt80_100_raw[i] = new TH1F(hname, pt_range, NBIN, cut_m_low[ich], cut_m_high[ich]);
        sprintf(hname, "hfg_JetTrgPt80_100%d", i);
        hfg_JetTrgPt80_100[i] = new TH1F(hname, pt_range, NBIN, cut_m_low[ich], cut_m_high[ich]);

        sprintf(hname, "hfg_JetTrgPt100above_raw%d", i);
        hfg_JetTrgPt100above_raw[i] = new TH1F(hname, pt_range, NBIN, cut_m_low[ich], cut_m_high[ich]);
        sprintf(hname, "hfg_JetTrgPt100above%d", i);
        hfg_JetTrgPt100above[i] = new TH1F(hname, pt_range, NBIN, cut_m_low[ich], cut_m_high[ich]);

        hfg_ZBiasSglTrkPt0_20_raw[i]->Sumw2();
        hfg_ZBiasSglTrkPt0_20[i]->Sumw2();

        hfg_JetTrgPt20_40_raw[i]->Sumw2();
        hfg_JetTrgPt20_40[i]->Sumw2();

        hfg_JetTrgPt40_60_raw[i]->Sumw2();
        hfg_JetTrgPt40_60[i]->Sumw2();

        hfg_JetTrgPt60_80_raw[i]->Sumw2();
        hfg_JetTrgPt60_80[i]->Sumw2();

        hfg_JetTrgPt80_100_raw[i]->Sumw2();
        hfg_JetTrgPt80_100[i]->Sumw2();

        hfg_JetTrgPt100above_raw[i]->Sumw2();
        hfg_JetTrgPt100above[i]->Sumw2();

        //Photon trigger 
        sprintf(pt_range, "ch: %s, rapidity:%d, pt: %f %f", MesonName[ich], iy, pt_low, pt_high);

        sprintf(hname, "hfg_ZBiasSglTrkPt0_10_raw%d", i);
        hfg_ZBiasSglTrkPt0_10_raw[i] = new TH1F(hname, pt_range, NBIN, cut_m_low[ich], cut_m_high[ich]);
        sprintf(hname, "hfg_ZBiasSglTrkPt0_10%d", i);
        hfg_ZBiasSglTrkPt0_10[i] = new TH1F(hname, pt_range, NBIN, cut_m_low[ich], cut_m_high[ich]);

        sprintf(hname, "hfg_PhoTrgPt10_raw%d", i);
        hfg_PhoTrgPt10_raw[i] = new TH1F(hname, pt_range, NBIN, cut_m_low[ich], cut_m_high[ich]);

        sprintf(hname, "hfg_PhoTrgPt10_15_raw%d", i);
        hfg_PhoTrgPt10_15_raw[i] = new TH1F(hname, pt_range, NBIN, cut_m_low[ich], cut_m_high[ich]);
        sprintf(hname, "hfg_PhoTrgPt10_15%d", i);
        hfg_PhoTrgPt10_15[i] = new TH1F(hname, pt_range, NBIN, cut_m_low[ich], cut_m_high[ich]);

        sprintf(hname, "hfg_PhoTrgPt15_20_raw%d", i);
        hfg_PhoTrgPt15_20_raw[i] = new TH1F(hname, pt_range, NBIN, cut_m_low[ich], cut_m_high[ich]);
        sprintf(hname, "hfg_PhoTrgPt15_20%d", i);
        hfg_PhoTrgPt15_20[i] = new TH1F(hname, pt_range, NBIN, cut_m_low[ich], cut_m_high[ich]);

        sprintf(hname, "hfg_PhoTrgPt20_30_raw%d", i);
        hfg_PhoTrgPt20_30_raw[i] = new TH1F(hname, pt_range, NBIN, cut_m_low[ich], cut_m_high[ich]);
        sprintf(hname, "hfg_PhoTrgPt20_30%d", i);
        hfg_PhoTrgPt20_30[i] = new TH1F(hname, pt_range, NBIN, cut_m_low[ich], cut_m_high[ich]);

        sprintf(hname, "hfg_PhoTrgPt30above_raw%d", i);
        hfg_PhoTrgPt30above_raw[i] = new TH1F(hname, pt_range, NBIN, cut_m_low[ich], cut_m_high[ich]);
        sprintf(hname, "hfg_PhoTrgPt30above%d", i);
        hfg_PhoTrgPt30above[i] = new TH1F(hname, pt_range, NBIN, cut_m_low[ich], cut_m_high[ich]);

        hfg_ZBiasSglTrkPt0_10_raw[i]->Sumw2();
        hfg_ZBiasSglTrkPt0_10[i]->Sumw2();

        hfg_PhoTrgPt10_15_raw[i]->Sumw2();
        hfg_PhoTrgPt10_15[i]->Sumw2();

        hfg_PhoTrgPt15_20_raw[i]->Sumw2();
        hfg_PhoTrgPt15_20[i]->Sumw2();

        hfg_PhoTrgPt20_30_raw[i]->Sumw2();
        hfg_PhoTrgPt20_30[i]->Sumw2();

        hfg_PhoTrgPt30above_raw[i]->Sumw2();
        hfg_PhoTrgPt30above[i]->Sumw2();
    }

    // components for trigger combination for D*
    for(short i = 0; i<NPT; i++) {

        get_pt_range(i, pt_low, pt_high);

        sprintf(pt_range, "pt: %f %f", pt_low, pt_high);

        // FullTrack trigger
        sprintf(hname, "hfgdiff_ZBiasSglTrkPt0_12_raw%d", i);
        hfgdiff_ZBiasSglTrkPt0_12_raw[i] = new TH1F(hname, pt_range, NBIN, mdiffup, mdiffdn);
        sprintf(hname, "hfgdiff_ZBiasSglTrkPt0_12%d", i);
        hfgdiff_ZBiasSglTrkPt0_12[i] = new TH1F(hname, pt_range, NBIN, mdiffup, mdiffdn);

        sprintf(hname, "hfgdiff_TrkTrgPt12_20_raw%d", i);
        hfgdiff_TrkTrgPt12_20_raw[i] = new TH1F(hname, pt_range, NBIN, mdiffup, mdiffdn);
        sprintf(hname, "hfgdiff_TrkTrgPt12_20%d", i);
        hfgdiff_TrkTrgPt12_20[i] = new TH1F(hname, pt_range, NBIN, mdiffup, mdiffdn);

        sprintf(hname, "hfgdiff_TrkTrgPt20_30_raw%d", i);
        hfgdiff_TrkTrgPt20_30_raw[i] = new TH1F(hname, pt_range, NBIN, mdiffup, mdiffdn);
        sprintf(hname, "hfgdiff_TrkTrgPt20_30%d", i);
        hfgdiff_TrkTrgPt20_30[i] = new TH1F(hname, pt_range, NBIN, mdiffup, mdiffdn);

        sprintf(hname, "hfgdiff_TrkTrgPt30above_raw%d", i);
        hfgdiff_TrkTrgPt30above_raw[i] = new TH1F(hname, pt_range, NBIN, mdiffup, mdiffdn);
        sprintf(hname, "hfgdiff_TrkTrgPt30above%d", i);
        hfgdiff_TrkTrgPt30above[i] = new TH1F(hname, pt_range, NBIN, mdiffup, mdiffdn);


        sprintf(hname, "hbgdiff_ZBiasSglTrkPt0_12_raw%d", i);
        hbgdiff_ZBiasSglTrkPt0_12_raw[i] = new TH1F(hname, pt_range, NBIN, mdiffup, mdiffdn);
        sprintf(hname, "hbgdiff_ZBiasSglTrkPt0_12%d", i);
        hbgdiff_ZBiasSglTrkPt0_12[i] = new TH1F(hname, pt_range, NBIN, mdiffup, mdiffdn);

        sprintf(hname, "hbgdiff_TrkTrgPt12_20_raw%d", i);
        hbgdiff_TrkTrgPt12_20_raw[i] = new TH1F(hname, pt_range, NBIN, mdiffup, mdiffdn);
        sprintf(hname, "hbgdiff_TrkTrgPt12_20%d", i);
        hbgdiff_TrkTrgPt12_20[i] = new TH1F(hname, pt_range, NBIN, mdiffup, mdiffdn);

        sprintf(hname, "hbgdiff_TrkTrgPt20_30_raw%d", i);
        hbgdiff_TrkTrgPt20_30_raw[i] = new TH1F(hname, pt_range, NBIN, mdiffup, mdiffdn);
        sprintf(hname, "hbgdiff_TrkTrgPt20_30%d", i);
        hbgdiff_TrkTrgPt20_30[i] = new TH1F(hname, pt_range, NBIN, mdiffup, mdiffdn);

        sprintf(hname, "hbgdiff_TrkTrgPt30above_raw%d", i);
        hbgdiff_TrkTrgPt30above_raw[i] = new TH1F(hname, pt_range, NBIN, mdiffup, mdiffdn);
        sprintf(hname, "hbgdiff_TrkTrgPt30above%d", i);
        hbgdiff_TrkTrgPt30above[i] = new TH1F(hname, pt_range, NBIN, mdiffup, mdiffdn);

        hfgdiff_ZBiasSglTrkPt0_12_raw[i]->Sumw2();
        hfgdiff_ZBiasSglTrkPt0_12[i]->Sumw2();

        hfgdiff_TrkTrgPt12_20_raw[i]->Sumw2();
        hfgdiff_TrkTrgPt12_20[i]->Sumw2();

        hfgdiff_TrkTrgPt20_30_raw[i]->Sumw2();
        hfgdiff_TrkTrgPt20_30[i]->Sumw2();

        hfgdiff_TrkTrgPt30above_raw[i]->Sumw2();
        hfgdiff_TrkTrgPt30above[i]->Sumw2();

        hbgdiff_ZBiasSglTrkPt0_12_raw[i]->Sumw2();
        hbgdiff_ZBiasSglTrkPt0_12[i]->Sumw2();

        hbgdiff_TrkTrgPt12_20_raw[i]->Sumw2();
        hbgdiff_TrkTrgPt12_20[i]->Sumw2();

        hbgdiff_TrkTrgPt20_30_raw[i]->Sumw2();
        hbgdiff_TrkTrgPt20_30[i]->Sumw2();

        hbgdiff_TrkTrgPt30above_raw[i]->Sumw2();
        hbgdiff_TrkTrgPt30above[i]->Sumw2();

        // Jet trigger
        sprintf(hname, "hfgdiff_ZBiasSglTrkPt0_20_raw%d", i);
        hfgdiff_ZBiasSglTrkPt0_20_raw[i] = new TH1F(hname, pt_range, NBIN, mdiffup, mdiffdn);
        sprintf(hname, "hfgdiff_ZBiasSglTrkPt0_20%d", i);
        hfgdiff_ZBiasSglTrkPt0_20[i] = new TH1F(hname, pt_range, NBIN, mdiffup, mdiffdn);

        sprintf(hname, "hfgdiff_JetTrgPt20_40_raw%d", i);
        hfgdiff_JetTrgPt20_40_raw[i] = new TH1F(hname, pt_range, NBIN, mdiffup, mdiffdn);
        sprintf(hname, "hfgdiff_JetTrgPt20_40%d", i);
        hfgdiff_JetTrgPt20_40[i] = new TH1F(hname, pt_range, NBIN, mdiffup, mdiffdn);

        sprintf(hname, "hfgdiff_JetTrgPt40_60_raw%d", i);
        hfgdiff_JetTrgPt40_60_raw[i] = new TH1F(hname, pt_range, NBIN, mdiffup, mdiffdn);
        sprintf(hname, "hfgdiff_JetTrgPt40_60%d", i);
        hfgdiff_JetTrgPt40_60[i] = new TH1F(hname, pt_range, NBIN, mdiffup, mdiffdn);

        sprintf(hname, "hfgdiff_JetTrgPt60_80_raw%d", i);
        hfgdiff_JetTrgPt60_80_raw[i] = new TH1F(hname, pt_range, NBIN, mdiffup, mdiffdn);
        sprintf(hname, "hfgdiff_JetTrgPt60_80%d", i);
        hfgdiff_JetTrgPt60_80[i] = new TH1F(hname, pt_range, NBIN, mdiffup, mdiffdn);

        sprintf(hname, "hfgdiff_JetTrgPt80_100_raw%d", i);
        hfgdiff_JetTrgPt80_100_raw[i] = new TH1F(hname, pt_range, NBIN, mdiffup, mdiffdn);
        sprintf(hname, "hfgdiff_JetTrgPt80_100%d", i);
        hfgdiff_JetTrgPt80_100[i] = new TH1F(hname, pt_range, NBIN, mdiffup, mdiffdn);

        sprintf(hname, "hfgdiff_JetTrgPt100above_raw%d", i);
        hfgdiff_JetTrgPt100above_raw[i] = new TH1F(hname, pt_range, NBIN, mdiffup, mdiffdn);
        sprintf(hname, "hfgdiff_JetTrgPt100above%d", i);
        hfgdiff_JetTrgPt100above[i] = new TH1F(hname, pt_range, NBIN, mdiffup, mdiffdn);


        sprintf(hname, "hbgdiff_ZBiasSglTrkPt0_20_raw%d", i);
        hbgdiff_ZBiasSglTrkPt0_20_raw[i] = new TH1F(hname, pt_range, NBIN, mdiffup, mdiffdn);
        sprintf(hname, "hbgdiff_ZBiasSglTrkPt0_20%d", i);
        hbgdiff_ZBiasSglTrkPt0_20[i] = new TH1F(hname, pt_range, NBIN, mdiffup, mdiffdn);

        sprintf(hname, "hbgdiff_JetTrgPt20_40_raw%d", i);
        hbgdiff_JetTrgPt20_40_raw[i] = new TH1F(hname, pt_range, NBIN, mdiffup, mdiffdn);
        sprintf(hname, "hbgdiff_JetTrgPt20_40%d", i);
        hbgdiff_JetTrgPt20_40[i] = new TH1F(hname, pt_range, NBIN, mdiffup, mdiffdn);

        sprintf(hname, "hbgdiff_JetTrgPt40_60_raw%d", i);
        hbgdiff_JetTrgPt40_60_raw[i] = new TH1F(hname, pt_range, NBIN, mdiffup, mdiffdn);
        sprintf(hname, "hbgdiff_JetTrgPt40_60%d", i);
        hbgdiff_JetTrgPt40_60[i] = new TH1F(hname, pt_range, NBIN, mdiffup, mdiffdn);

        sprintf(hname, "hbgdiff_JetTrgPt60_80_raw%d", i);
        hbgdiff_JetTrgPt60_80_raw[i] = new TH1F(hname, pt_range, NBIN, mdiffup, mdiffdn);
        sprintf(hname, "hbgdiff_JetTrgPt60_80%d", i);
        hbgdiff_JetTrgPt60_80[i] = new TH1F(hname, pt_range, NBIN, mdiffup, mdiffdn);

        sprintf(hname, "hbgdiff_JetTrgPt80_100_raw%d", i);
        hbgdiff_JetTrgPt80_100_raw[i] = new TH1F(hname, pt_range, NBIN, mdiffup, mdiffdn);
        sprintf(hname, "hbgdiff_JetTrgPt80_100%d", i);
        hbgdiff_JetTrgPt80_100[i] = new TH1F(hname, pt_range, NBIN, mdiffup, mdiffdn);

        sprintf(hname, "hbgdiff_JetTrgPt100above_raw%d", i);
        hbgdiff_JetTrgPt100above_raw[i] = new TH1F(hname, pt_range, NBIN, mdiffup, mdiffdn);
        sprintf(hname, "hbgdiff_JetTrgPt100above%d", i);
        hbgdiff_JetTrgPt100above[i] = new TH1F(hname, pt_range, NBIN, mdiffup, mdiffdn);

        hfgdiff_ZBiasSglTrkPt0_20_raw[i]->Sumw2();
        hfgdiff_ZBiasSglTrkPt0_20[i]->Sumw2();

        hfgdiff_JetTrgPt20_40_raw[i]->Sumw2();
        hfgdiff_JetTrgPt20_40[i]->Sumw2();

        hfgdiff_JetTrgPt40_60_raw[i]->Sumw2();
        hfgdiff_JetTrgPt40_60[i]->Sumw2();

        hfgdiff_JetTrgPt60_80_raw[i]->Sumw2();
        hfgdiff_JetTrgPt60_80[i]->Sumw2();

        hfgdiff_JetTrgPt80_100_raw[i]->Sumw2();
        hfgdiff_JetTrgPt80_100[i]->Sumw2();

        hfgdiff_JetTrgPt100above_raw[i]->Sumw2();
        hfgdiff_JetTrgPt100above[i]->Sumw2();

        hbgdiff_ZBiasSglTrkPt0_20_raw[i]->Sumw2();
        hbgdiff_ZBiasSglTrkPt0_20[i]->Sumw2();

        hbgdiff_JetTrgPt20_40_raw[i]->Sumw2();
        hbgdiff_JetTrgPt20_40[i]->Sumw2();

        hbgdiff_JetTrgPt40_60_raw[i]->Sumw2();
        hbgdiff_JetTrgPt40_60[i]->Sumw2();

        hbgdiff_JetTrgPt60_80_raw[i]->Sumw2();
        hbgdiff_JetTrgPt60_80[i]->Sumw2();

        hbgdiff_JetTrgPt80_100_raw[i]->Sumw2();
        hbgdiff_JetTrgPt80_100[i]->Sumw2();

        hbgdiff_JetTrgPt100above_raw[i]->Sumw2();
        hbgdiff_JetTrgPt100above[i]->Sumw2();

        // Photon trigger
        sprintf(hname, "hfgdiff_ZBiasSglTrkPt0_10_raw%d", i);
        hfgdiff_ZBiasSglTrkPt0_10_raw[i] = new TH1F(hname, pt_range, NBIN, mdiffup, mdiffdn);
        sprintf(hname, "hfgdiff_ZBiasSglTrkPt0_10%d", i);
        hfgdiff_ZBiasSglTrkPt0_10[i] = new TH1F(hname, pt_range, NBIN, mdiffup, mdiffdn);

        sprintf(hname, "hfgdiff_PhoTrgPt10_15_raw%d", i);
        hfgdiff_PhoTrgPt10_15_raw[i] = new TH1F(hname, pt_range, NBIN, mdiffup, mdiffdn);
        sprintf(hname, "hfgdiff_PhoTrgPt10_15%d", i);
        hfgdiff_PhoTrgPt10_15[i] = new TH1F(hname, pt_range, NBIN, mdiffup, mdiffdn);

        sprintf(hname, "hfgdiff_PhoTrgPt15_20_raw%d", i);
        hfgdiff_PhoTrgPt15_20_raw[i] = new TH1F(hname, pt_range, NBIN, mdiffup, mdiffdn);
        sprintf(hname, "hfgdiff_PhoTrgPt15_20%d", i);
        hfgdiff_PhoTrgPt15_20[i] = new TH1F(hname, pt_range, NBIN, mdiffup, mdiffdn);

        sprintf(hname, "hfgdiff_PhoTrgPt20_30_raw%d", i);
        hfgdiff_PhoTrgPt20_30_raw[i] = new TH1F(hname, pt_range, NBIN, mdiffup, mdiffdn);
        sprintf(hname, "hfgdiff_PhoTrgPt20_30%d", i);
        hfgdiff_PhoTrgPt20_30[i] = new TH1F(hname, pt_range, NBIN, mdiffup, mdiffdn);

        sprintf(hname, "hfgdiff_PhoTrgPt30above_raw%d", i);
        hfgdiff_PhoTrgPt30above_raw[i] = new TH1F(hname, pt_range, NBIN, mdiffup, mdiffdn);
        sprintf(hname, "hfgdiff_PhoTrgPt30above%d", i);
        hfgdiff_PhoTrgPt30above[i] = new TH1F(hname, pt_range, NBIN, mdiffup, mdiffdn);

        sprintf(hname, "hbgdiff_ZBiasSglTrkPt0_10_raw%d", i);
        hbgdiff_ZBiasSglTrkPt0_10_raw[i] = new TH1F(hname, pt_range, NBIN, mdiffup, mdiffdn);
        sprintf(hname, "hbgdiff_ZBiasSglTrkPt0_10%d", i);
        hbgdiff_ZBiasSglTrkPt0_10[i] = new TH1F(hname, pt_range, NBIN, mdiffup, mdiffdn);

        sprintf(hname, "hbgdiff_PhoTrgPt10_15_raw%d", i);
        hbgdiff_PhoTrgPt10_15_raw[i] = new TH1F(hname, pt_range, NBIN, mdiffup, mdiffdn);
        sprintf(hname, "hbgdiff_PhoTrgPt10_15%d", i);
        hbgdiff_PhoTrgPt10_15[i] = new TH1F(hname, pt_range, NBIN, mdiffup, mdiffdn);

        sprintf(hname, "hbgdiff_PhoTrgPt15_20_raw%d", i);
        hbgdiff_PhoTrgPt15_20_raw[i] = new TH1F(hname, pt_range, NBIN, mdiffup, mdiffdn);
        sprintf(hname, "hbgdiff_PhoTrgPt15_20%d", i);
        hbgdiff_PhoTrgPt15_20[i] = new TH1F(hname, pt_range, NBIN, mdiffup, mdiffdn);

        sprintf(hname, "hbgdiff_PhoTrgPt20_30_raw%d", i);
        hbgdiff_PhoTrgPt20_30_raw[i] = new TH1F(hname, pt_range, NBIN, mdiffup, mdiffdn);
        sprintf(hname, "hbgdiff_PhoTrgPt20_30%d", i);
        hbgdiff_PhoTrgPt20_30[i] = new TH1F(hname, pt_range, NBIN, mdiffup, mdiffdn);

        sprintf(hname, "hbgdiff_PhoTrgPt30above_raw%d", i);
        hbgdiff_PhoTrgPt30above_raw[i] = new TH1F(hname, pt_range, NBIN, mdiffup, mdiffdn);
        sprintf(hname, "hbgdiff_PhoTrgPt30above%d", i);
        hbgdiff_PhoTrgPt30above[i] = new TH1F(hname, pt_range, NBIN, mdiffup, mdiffdn);

        hfgdiff_ZBiasSglTrkPt0_10_raw[i]->Sumw2();
        hfgdiff_ZBiasSglTrkPt0_10[i]->Sumw2();

        hfgdiff_PhoTrgPt10_15_raw[i]->Sumw2();
        hfgdiff_PhoTrgPt10_15[i]->Sumw2();

        hfgdiff_PhoTrgPt15_20_raw[i]->Sumw2();
        hfgdiff_PhoTrgPt15_20[i]->Sumw2();

        hfgdiff_PhoTrgPt20_30_raw[i]->Sumw2();
        hfgdiff_PhoTrgPt20_30[i]->Sumw2();

        hfgdiff_PhoTrgPt30above_raw[i]->Sumw2();
        hfgdiff_PhoTrgPt30above[i]->Sumw2();

        hbgdiff_ZBiasSglTrkPt0_10_raw[i]->Sumw2();
        hbgdiff_ZBiasSglTrkPt0_10[i]->Sumw2();

        hbgdiff_PhoTrgPt10_15_raw[i]->Sumw2();
        hbgdiff_PhoTrgPt10_15[i]->Sumw2();

        hbgdiff_PhoTrgPt15_20_raw[i]->Sumw2();
        hbgdiff_PhoTrgPt15_20[i]->Sumw2();

        hbgdiff_PhoTrgPt20_30_raw[i]->Sumw2();
        hbgdiff_PhoTrgPt20_30[i]->Sumw2();

        hbgdiff_PhoTrgPt30above_raw[i]->Sumw2();
        hbgdiff_PhoTrgPt30above[i]->Sumw2();
    }

}

//
int ana_hfforest::get_pt_bin_num(float pt)
{
    int ipt = pt/dPt;
    if(ipt >= NPT)
        ipt = NPT - 1;  //.. the last bin include all higher pT region...

    return ipt;
}

//
void ana_hfforest::get_pt_range(int i, float& pt_low, float& pt_high)
{
    pt_low = i*dPt;
    pt_high = pt_low +  dPt;
}

//
void ana_hfforest::LoopOverFile(int startFile, int endFile, char *filelist, int iy, int ich, char* outfile) 
{//.. loop over files in the "*filelist" from startFile to endFile in sequence
    //
    Init(startFile, endFile, filelist, iy, ich, outfile);

    //
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

        //
        TFile *f = TFile::Open(filename);
        if(!f) {
            cout<<"==> empty file <=="<<endl;
            continue;
        }

        if(!GetTreeInfo(f)) continue;

        //
        LoopOverEvt(HltTree, iy, ich);

        //
        f->Close();
    }

    write();
}

//
bool ana_hfforest::GetTreeInfo(TFile* f)
{
    //HiTree  = (TTree*)f->Get("hiEvtAnalyzer/HiTree");
    HltTree  = (TTree*)f->Get("hltanalysis/HltTree");
    hltTreeReRun  = (TTree*)f->Get("hltReRun/hltTree");
    hftree = (TTree*)f->Get("OpenHfTree/hftree");

    //if(!hftree || !HltTree || !HiTree) {
    if(!hftree || !HltTree || !hltTreeReRun) {
        cout<<"==> missing hftree or HltTree or hltTreeReRun <=="<<endl;
        return false;
    }

    HltTree->AddFriend(hftree);
    HltTree->AddFriend(hltTreeReRun);
    //HltTree->AddFriend(HiTree);

    hftree->SetBranchAddress("hfcandidate", &hfcandidate);

    get_trg_info(HltTree, hltTreeReRun);

    return true;
}

//
void ana_hfforest::write()
{//.. results 
    result->cd();

    for(int itrg = 0; itrg<NTRG; itrg++) 
        for(short i = 0; i<NPT; i++) {
            hfg_raw[itrg][i]->Write();
            hfg[itrg][i]->Write();
            hfgdiff_raw[itrg][i]->Write();
            hfgdiff[itrg][i]->Write();
            hbgdiff_raw[itrg][i]->Write();
            hbgdiff[itrg][i]->Write();
        }

    for(short i = 0; i<NPT; i++) {
        //FullTrack trigger ...
        hfg_ZBiasSglTrkPt0_12_raw[i]->Write();
        hfg_ZBiasSglTrkPt0_12[i]->Write();

	hfg_TrkTrgPt12_raw[i]->Write();
        hfg_TrkTrgPt12_20_raw[i]->Write();
        hfg_TrkTrgPt12_20[i]->Write();
        hfg_TrkTrgPt20_30_raw[i]->Write();
        hfg_TrkTrgPt20_30[i]->Write();
        hfg_TrkTrgPt30above_raw[i]->Write();
        hfg_TrkTrgPt30above[i]->Write();

        // Jet trigger ..
        hfg_ZBiasSglTrkPt0_20_raw[i]->Write();
        hfg_ZBiasSglTrkPt0_20[i]->Write();

        hfg_JetTrgPt20_raw[i]->Write();
        hfg_JetTrgPt20_40_raw[i]->Write();
        hfg_JetTrgPt20_40[i]->Write();
        hfg_JetTrgPt40_60_raw[i]->Write();
        hfg_JetTrgPt40_60[i]->Write();
        hfg_JetTrgPt60_80_raw[i]->Write();
        hfg_JetTrgPt60_80[i]->Write();
        hfg_JetTrgPt80_100_raw[i]->Write();
        hfg_JetTrgPt80_100[i]->Write();
        hfg_JetTrgPt100above_raw[i]->Write();
        hfg_JetTrgPt100above[i]->Write();

        //.. photon trigger 
        hfg_ZBiasSglTrkPt0_10_raw[i]->Write();
        hfg_ZBiasSglTrkPt0_10[i]->Write();

        hfg_PhoTrgPt10_raw[i]->Write();
        hfg_PhoTrgPt10_15_raw[i]->Write();
        hfg_PhoTrgPt10_15[i]->Write();
        hfg_PhoTrgPt15_20_raw[i]->Write();
        hfg_PhoTrgPt15_20[i]->Write();
        hfg_PhoTrgPt20_30_raw[i]->Write();
        hfg_PhoTrgPt20_30[i]->Write();
        hfg_PhoTrgPt30above_raw[i]->Write();
        hfg_PhoTrgPt30above[i]->Write();

        //.. track trigger 
        hfgdiff_ZBiasSglTrkPt0_12_raw[i]->Write();
        hfgdiff_ZBiasSglTrkPt0_12[i]->Write();

        hfgdiff_TrkTrgPt12_20_raw[i]->Write();
        hfgdiff_TrkTrgPt12_20[i]->Write();
        hfgdiff_TrkTrgPt20_30_raw[i]->Write();
        hfgdiff_TrkTrgPt20_30[i]->Write();
        hfgdiff_TrkTrgPt30above_raw[i]->Write();
        hfgdiff_TrkTrgPt30above[i]->Write();

        hbgdiff_ZBiasSglTrkPt0_12_raw[i]->Write();
        hbgdiff_ZBiasSglTrkPt0_12[i]->Write();

        hbgdiff_TrkTrgPt12_20_raw[i]->Write();
        hbgdiff_TrkTrgPt12_20[i]->Write();
        hbgdiff_TrkTrgPt20_30_raw[i]->Write();
        hbgdiff_TrkTrgPt20_30[i]->Write();
        hbgdiff_TrkTrgPt30above_raw[i]->Write();
        hbgdiff_TrkTrgPt30above[i]->Write();

        //. Jet trigger 
        hfgdiff_ZBiasSglTrkPt0_20_raw[i]->Write();
        hfgdiff_ZBiasSglTrkPt0_20[i]->Write();

        hfgdiff_JetTrgPt20_40_raw[i]->Write();
        hfgdiff_JetTrgPt20_40[i]->Write();
        hfgdiff_JetTrgPt40_60_raw[i]->Write();
        hfgdiff_JetTrgPt40_60[i]->Write();
        hfgdiff_JetTrgPt60_80_raw[i]->Write();
        hfgdiff_JetTrgPt60_80[i]->Write();
        hfgdiff_JetTrgPt80_100_raw[i]->Write();
        hfgdiff_JetTrgPt80_100[i]->Write();
        hfgdiff_JetTrgPt100above_raw[i]->Write();
        hfgdiff_JetTrgPt100above[i]->Write();

        hbgdiff_ZBiasSglTrkPt0_20_raw[i]->Write();
        hbgdiff_ZBiasSglTrkPt0_20[i]->Write();

        hbgdiff_JetTrgPt20_40_raw[i]->Write();
        hbgdiff_JetTrgPt20_40[i]->Write();
        hbgdiff_JetTrgPt40_60_raw[i]->Write();
        hbgdiff_JetTrgPt40_60[i]->Write();
        hbgdiff_JetTrgPt60_80_raw[i]->Write();
        hbgdiff_JetTrgPt60_80[i]->Write();
        hbgdiff_JetTrgPt80_100_raw[i]->Write();
        hbgdiff_JetTrgPt80_100[i]->Write();
        hbgdiff_JetTrgPt100above_raw[i]->Write();
        hbgdiff_JetTrgPt100above[i]->Write();

        //. photon trigger 
        hfgdiff_ZBiasSglTrkPt0_10_raw[i]->Write();
        hfgdiff_ZBiasSglTrkPt0_10[i]->Write();

        hfgdiff_PhoTrgPt10_15_raw[i]->Write();
        hfgdiff_PhoTrgPt10_15[i]->Write();
        hfgdiff_PhoTrgPt15_20_raw[i]->Write();
        hfgdiff_PhoTrgPt15_20[i]->Write();
        hfgdiff_PhoTrgPt20_30_raw[i]->Write();
        hfgdiff_PhoTrgPt20_30[i]->Write();
        hfgdiff_PhoTrgPt30above_raw[i]->Write();
        hfgdiff_PhoTrgPt30above[i]->Write();

        hbgdiff_ZBiasSglTrkPt0_10_raw[i]->Write();
        hbgdiff_ZBiasSglTrkPt0_10[i]->Write();

        hbgdiff_PhoTrgPt10_15_raw[i]->Write();
        hbgdiff_PhoTrgPt10_15[i]->Write();
        hbgdiff_PhoTrgPt15_20_raw[i]->Write();
        hbgdiff_PhoTrgPt15_20[i]->Write();
        hbgdiff_PhoTrgPt20_30_raw[i]->Write();
        hbgdiff_PhoTrgPt20_30[i]->Write();
        hbgdiff_PhoTrgPt30above_raw[i]->Write();
        hbgdiff_PhoTrgPt30above[i]->Write();
    }

    result->Close();
}

//
void ana_hfforest::get_trg_info(TTree* T1, TTree* T2) 
{
    reset_trg();

    TObjArray *branchesT1 = (TObjArray *)T1->GetListOfBranches();
    TIter branch_iterT1(branchesT1); 
    TBranch* branchT1 = 0;
    while ((branchT1 = (TBranch *)branch_iterT1.Next())) {  
        int tid = -1;
        TString branch_nameT1 = branchT1->GetName();
        for(int it = 0; it<NTRG; it++) {
            if (branch_nameT1.Contains(trg_name[it])){
                if(!branch_nameT1.Contains("Prescl") && 
                        !branch_nameT1.Contains("trigObject")) {//.. use original trigger decision
                    T1->SetBranchAddress(branch_nameT1.Data(), &trg[it]);
                    tid = it;
                } else  if(branch_nameT1.Contains("Prescl")) {//.. use original prescale
                    T1->SetBranchAddress(branch_nameT1.Data(), &prscl[it]);
                }

                break;
            } 
        }
        //
        //
        if(tid !=-1) {//.. Loop only once since there're multiple objects for the same trigger  
            TObjArray *branchesT2 = (TObjArray *)T2->GetListOfBranches();
            TIter branch_iterT2(branchesT2); 
            TBranch* branchT2 = 0;
            while ((branchT2 = (TBranch *)branch_iterT2.Next())) {  
                TString branch_nameT2 = branchT2->GetName();
                if (branch_nameT2.Contains(trg_name[tid]) && branch_nameT2.Contains("trigObject")){
                    T2->SetBranchAddress(branch_nameT2.Data(), &trg_obj[tid]);
                    break;
                } 
            }
        }
    }
}


//
void ana_hfforest::reset_trg()
{//.. reset trigger decision every event
    for(int i = 0; i<NTRG; i++) {
        prscl[i] = 0;
        trg[i] = 0;
        trg_obj[i]->clear();
    }

    maxTrkTrgPt = 0;
    maxJetTrgPt = 0;
    maxPhoTrgPt = 0;
}

//
void ana_hfforest::LoopOverEvt(TTree* T, int iy, int ich)
{//.. loop over each event. Add different analysis function here. 

    for(int i = 0; i<T->GetEntries(); i++) {
        reset_trg();
        T->GetEntry(i);

        GetMaxTrgPt();
        LoopOverHFCandidate(iy, ich);
    }
}

//
void ana_hfforest::GetMaxTrgPt()
{
    //.. track trigger ...
    for(int it = 1; it<=3; it++) {
        if(trg[it]) {
            FindMaxTrgPt(trg_obj[it], maxTrkTrgPt);
            break;
        }
    }

    // Jet trigger
    for(int it = 4; it<=8; it++) {
        if(trg[it]) {
            FindMaxTrgPt(trg_obj[it], maxJetTrgPt);
            break;
        }
    }

    // photon trigger 
    for(int it = 9; it<=12; it++) {
        if(trg[it]) {
            FindMaxTrgPt(trg_obj[it], maxPhoTrgPt);
            break;
        }
    }
}

//
bool ana_hfforest::GetRapidity(float m, float pt, float eta, int iY)
{
    float theta = 2*atan(exp(-eta));
    float p = pt/sin(theta);
    float E = sqrt(m*m + p*p);
    float pz = pt/tan(theta);
    float Y = 0.5*log((E+pz)/(E-pz));


    if(!checkRapidityBoundary(Y))
        return false;

    if(iY != (int)((Y-Ymin)/dY))
        return false; 

    return true;
}

//
void ana_hfforest::LoopOverHFCandidate(int iy, int ich)
{//.. loop over Heavy Flavor candidate in a event 
    for(int i = 0; i<hfcandidate->get_nhfcand(); i++) {
        snglhfcand* cand = hfcandidate->get_hfcand(i);

        FillMassHisto(cand, iy, ich);
    }

    hfcandidate->Reset();
}

//
void ana_hfforest::FillMassHisto(snglhfcand* cand, int iy, int ich)
{//.. fill the histogram of mass distribution ..

    if(cand->get_type() != ich+1) 
        return;
    //
    float mass = cand->get_fm();
    float fpt = cand->get_fpt();
    float feta = cand->get_feta();
    //.. the two daughters of the resonance and they should be unlike-sign. 
    float fq1 = cand->get_fq1();  
    //float fq2 = cand->get_fq2();
    float fq2 = cand->get_fpt2(); //.. fpt2 is filled by fq2
    float feta1 = cand->get_feta1();
    float feta2 = cand->get_feta2();


    if(!GetRapidity(mass, fpt, feta, iy) || fq1==fq2 || fabs(feta1)>etaMax || fabs(feta2)>etaMax) 
        return;
    //
    float ffls3d = cand->get_ffls3d();
    float alpha = cand->get_falpha0();
    float fprob = cand->get_fprob();
    float fdr = cand->get_fdr();
    float fchi2 = cand->get_fchi2();
    float mass_dau = cand->get_fmdau1();


    int ipt = get_pt_bin_num(fpt);


    if(mass > cut_m_low[ich] && mass < cut_m_high[ich] && 
            ffls3d > cut_ffls3d[ich] && alpha < cut_falpha0[ich] && 
            fprob > cut_fprob[ich] && fdr < cut_fdr[ich] && fchi2 < cut_fchi2[ich]) {

        //.. forground ..
        if(ich==1) { //D0 has no daughter mass, treat separately. ...
            for(int itrg = 0; itrg<NTRG; itrg++) {
                if(trg[itrg]) {
                    hfg_raw[itrg][ipt]->Fill(mass);
                    hfg[itrg][ipt]->Fill(mass, prscl[itrg]);
                }
            }

            FillTrgCombineTrkTrg(0, ich, ipt, mass, mass_dau);
            FillTrgCombineJetTrg(0, ich, ipt, mass, mass_dau);
            FillTrgCombinePhoTrg(0, ich, ipt, mass, mass_dau);

        } else {
            if(mass_dau > cut_m_dau_low[ich] && mass_dau < cut_m_dau_high[ich]) {
                for(int itrg = 0; itrg<NTRG; itrg++) {
                    if(trg[itrg]) {
                        hfg_raw[itrg][ipt]->Fill(mass);
                        hfg[itrg][ipt]->Fill(mass, prscl[itrg]);

                        if(ich==0) {
                            hfgdiff_raw[itrg][ipt]->Fill(mass-mass_dau);
                            hfgdiff[itrg][ipt]->Fill(mass-mass_dau, prscl[itrg]);
                        }
                    }
                }

                FillTrgCombineTrkTrg(0, ich, ipt, mass, mass_dau);
                FillTrgCombineJetTrg(0, ich, ipt, mass, mass_dau);
                FillTrgCombinePhoTrg(0, ich, ipt, mass, mass_dau);

                if(ich==0) {
                    FillTrgCombineTrkTrg(1, ich, ipt, mass, mass_dau);
                    FillTrgCombineJetTrg(1, ich, ipt, mass, mass_dau);
                    FillTrgCombinePhoTrg(1, ich, ipt, mass, mass_dau);
                }

            } else { //.. background through side band ...
                for(int itrg = 0; itrg<NTRG; itrg++) {
                    if(trg[itrg] && ich==0) {
                        hbgdiff_raw[itrg][ipt]->Fill(mass-mass_dau);
                        hbgdiff[itrg][ipt]->Fill(mass-mass_dau, prscl[itrg]);
                    }
                }

                if(ich==0) {
                    FillTrgCombineTrkTrg(2, ich, ipt, mass, mass_dau);
                    FillTrgCombineJetTrg(2, ich, ipt, mass, mass_dau);
                    FillTrgCombinePhoTrg(2, ich, ipt, mass, mass_dau);
                }
            }
        }
    }
}

//
void ana_hfforest::FillTrgCombineTrkTrg(int id, int ich, int ipt, float mass, float mass_dau)
{//.. id:  0-->hfg,  1-->hfgdiff, 2-->hbgdiff

    if(trg[1] && maxTrkTrgPt >=12) 
        hfg_TrkTrgPt12_raw[ipt]->Fill(mass);

    if(trg[1] && maxTrkTrgPt >=12 && maxTrkTrgPt < 20) {
        if(id==0) {
            hfg_TrkTrgPt12_20_raw[ipt]->Fill(mass);
            hfg_TrkTrgPt12_20[ipt]->Fill(mass, prscl[1]); //.. L1 base prescale 1
        } else if(id==1) {
            hfgdiff_TrkTrgPt12_20_raw[ipt]->Fill(mass-mass_dau);
            hfgdiff_TrkTrgPt12_20[ipt]->Fill(mass-mass_dau, prscl[1]); //.. L1 base prescale 1
        } else if(id==2) {
            hbgdiff_TrkTrgPt12_20_raw[ipt]->Fill(mass-mass_dau);
            hbgdiff_TrkTrgPt12_20[ipt]->Fill(mass-mass_dau, prscl[1]); //.. L1 base prescale 1
        }
    } else if(trg[2] && maxTrkTrgPt >=20 && maxTrkTrgPt < 30) {
        if(id==0) {
            hfg_TrkTrgPt20_30_raw[ipt]->Fill(mass);
            hfg_TrkTrgPt20_30[ipt]->Fill(mass, prscl[2]); //.. L1 base prescale 1
        } else if(id==1) {
            hfgdiff_TrkTrgPt20_30_raw[ipt]->Fill(mass-mass_dau);
            hfgdiff_TrkTrgPt20_30[ipt]->Fill(mass-mass_dau, prscl[2]); //.. L1 base prescale 1
        } else if(id==2) {
            hbgdiff_TrkTrgPt20_30_raw[ipt]->Fill(mass-mass_dau);
            hbgdiff_TrkTrgPt20_30[ipt]->Fill(mass-mass_dau, prscl[2]); //.. L1 base prescale 1
        }
    } else if(trg[3] && maxTrkTrgPt >= 30) {
        if(id==0) {
            hfg_TrkTrgPt30above_raw[ipt]->Fill(mass);
            hfg_TrkTrgPt30above[ipt]->Fill(mass, prscl[3]);// L1 base prescale 1
        } else if(id==1) {
            hfgdiff_TrkTrgPt30above_raw[ipt]->Fill(mass-mass_dau);
            hfgdiff_TrkTrgPt30above[ipt]->Fill(mass-mass_dau, prscl[3]);// L1 base prescale 1
        } else if(id==2) {
            hfgdiff_TrkTrgPt30above_raw[ipt]->Fill(mass-mass_dau);
            hfgdiff_TrkTrgPt30above[ipt]->Fill(mass-mass_dau, prscl[3]);// L1 base prescale 1
        }
    }
}

//
void ana_hfforest::FillTrgCombineJetTrg(int id, int ich, int ipt, float mass, float mass_dau)
{//.. id:  0-->hfg,  1-->hfgdiff, 2-->hbgdiff

    if(trg[4] && maxJetTrgPt >=20) 
        hfg_JetTrgPt20_raw[ipt]->Fill(mass);

    if(trg[4] && maxJetTrgPt >=20 && maxJetTrgPt < 40) {
        if(id==0) {
            hfg_JetTrgPt20_40_raw[ipt]->Fill(mass);
            hfg_JetTrgPt20_40[ipt]->Fill(mass, prscl[4]); //.. L1 base prescale 1
        } else if(id==1) {
            hfgdiff_JetTrgPt20_40_raw[ipt]->Fill(mass-mass_dau);
            hfgdiff_JetTrgPt20_40[ipt]->Fill(mass-mass_dau, prscl[4]); //.. L1 base prescale 1
        } else if(id==2) {
            hbgdiff_JetTrgPt20_40_raw[ipt]->Fill(mass-mass_dau);
            hbgdiff_JetTrgPt20_40[ipt]->Fill(mass-mass_dau, prscl[4]); //.. L1 base prescale 1
        }
    } else if(trg[5] && maxJetTrgPt >=40 && maxJetTrgPt < 60) {
        if(id==0) {
            hfg_JetTrgPt40_60_raw[ipt]->Fill(mass);
            hfg_JetTrgPt40_60[ipt]->Fill(mass, prscl[5]); //.. L1 base prescale 1
        } else if(id==1) {
            hfgdiff_JetTrgPt40_60_raw[ipt]->Fill(mass-mass_dau);
            hfgdiff_JetTrgPt40_60[ipt]->Fill(mass-mass_dau, prscl[5]); //.. L1 base prescale 1
        } else if(id==2) {
            hbgdiff_JetTrgPt40_60_raw[ipt]->Fill(mass-mass_dau);
            hbgdiff_JetTrgPt40_60[ipt]->Fill(mass-mass_dau, prscl[5]); //.. L1 base prescale 1
        }
    } else if(trg[6] && maxJetTrgPt >=60 && maxJetTrgPt < 80) {
        if(id==0) {
            hfg_JetTrgPt60_80_raw[ipt]->Fill(mass);
            hfg_JetTrgPt60_80[ipt]->Fill(mass, prscl[6]); //.. L1 base prescale 1
        } else if(id==1) {
            hfgdiff_JetTrgPt60_80_raw[ipt]->Fill(mass-mass_dau);
            hfgdiff_JetTrgPt60_80[ipt]->Fill(mass-mass_dau, prscl[6]); //.. L1 base prescale 1
        } else if(id==2) {
            hbgdiff_JetTrgPt60_80_raw[ipt]->Fill(mass-mass_dau);
            hbgdiff_JetTrgPt60_80[ipt]->Fill(mass-mass_dau, prscl[6]); //.. L1 base prescale 1
        }
    } else if(trg[7] && maxJetTrgPt >=80 && maxJetTrgPt < 100) {
        if(id==0) {
            hfg_JetTrgPt80_100_raw[ipt]->Fill(mass);
            hfg_JetTrgPt80_100[ipt]->Fill(mass, prscl[7]); //.. L1 base prescale 1
        } else if(id==1) {
            hfgdiff_JetTrgPt80_100_raw[ipt]->Fill(mass-mass_dau);
            hfgdiff_JetTrgPt80_100[ipt]->Fill(mass-mass_dau, prscl[7]); //.. L1 base prescale 1
        } else if(id==2) {
            hbgdiff_JetTrgPt80_100_raw[ipt]->Fill(mass-mass_dau);
            hbgdiff_JetTrgPt80_100[ipt]->Fill(mass-mass_dau, prscl[7]); //.. L1 base prescale 1
        }
    } else if(trg[8] && maxJetTrgPt >= 100) {
        if(id==0) {
            hfg_JetTrgPt100above_raw[ipt]->Fill(mass);
            hfg_JetTrgPt100above[ipt]->Fill(mass, prscl[8]);// L1 base prescale 1
        } else if(id==1) {
            hfgdiff_JetTrgPt100above_raw[ipt]->Fill(mass-mass_dau);
            hfgdiff_JetTrgPt100above[ipt]->Fill(mass-mass_dau, prscl[8]);// L1 base prescale 1
        } else if(id==2) {
            hfgdiff_JetTrgPt100above_raw[ipt]->Fill(mass-mass_dau);
            hfgdiff_JetTrgPt100above[ipt]->Fill(mass-mass_dau, prscl[8]);// L1 base prescale 1
        }
    }
}

//
void ana_hfforest::FillTrgCombinePhoTrg(int id, int ich, int ipt, float mass, float mass_dau)
{//.. id:  0-->hfg,  1-->hfgdiff, 2-->hbgdiff

    if(trg[9] && maxPhoTrgPt >=10) 
        hfg_PhoTrgPt10_raw[ipt]->Fill(mass);

    if(trg[9] && maxPhoTrgPt >=10 && maxPhoTrgPt < 15) {
        if(id==0) {
            hfg_PhoTrgPt10_15_raw[ipt]->Fill(mass);
            hfg_PhoTrgPt10_15[ipt]->Fill(mass, prscl[9]); //.. L1 base prescale 1
        } else if(id==1) {
            hfgdiff_PhoTrgPt10_15_raw[ipt]->Fill(mass-mass_dau);
            hfgdiff_PhoTrgPt10_15[ipt]->Fill(mass-mass_dau, prscl[9]); //.. L1 base prescale 1
        } else if(id==2) {
            hbgdiff_PhoTrgPt10_15_raw[ipt]->Fill(mass-mass_dau);
            hbgdiff_PhoTrgPt10_15[ipt]->Fill(mass-mass_dau, prscl[9]); //.. L1 base prescale 1
        }
    } else if(trg[10] && maxPhoTrgPt >=15 && maxPhoTrgPt < 20) {
        if(id==0) {
            hfg_PhoTrgPt15_20_raw[ipt]->Fill(mass);
            hfg_PhoTrgPt15_20[ipt]->Fill(mass, prscl[10]); //.. L1 base prescale 1
        } else if(id==1) {
            hfgdiff_PhoTrgPt15_20_raw[ipt]->Fill(mass-mass_dau);
            hfgdiff_PhoTrgPt15_20[ipt]->Fill(mass-mass_dau, prscl[10]); //.. L1 base prescale 1
        } else if(id==2) {
            hbgdiff_PhoTrgPt15_20_raw[ipt]->Fill(mass-mass_dau);
            hbgdiff_PhoTrgPt15_20[ipt]->Fill(mass-mass_dau, prscl[10]); //.. L1 base prescale 1
        }
    } else if(trg[11] && maxPhoTrgPt >=20 && maxPhoTrgPt < 30) {
        if(id==0) {
            hfg_PhoTrgPt20_30_raw[ipt]->Fill(mass);
            hfg_PhoTrgPt20_30[ipt]->Fill(mass, prscl[11]); //.. L1 base prescale 1
        } else if(id==1) {
            hfgdiff_PhoTrgPt20_30_raw[ipt]->Fill(mass-mass_dau);
            hfgdiff_PhoTrgPt20_30[ipt]->Fill(mass-mass_dau, prscl[11]); //.. L1 base prescale 1
        } else if(id==2) {
            hbgdiff_PhoTrgPt20_30_raw[ipt]->Fill(mass-mass_dau);
            hbgdiff_PhoTrgPt20_30[ipt]->Fill(mass-mass_dau, prscl[11]); //.. L1 base prescale 1
        }
    } else if(trg[12] && maxPhoTrgPt >= 30) {
        if(id==0) {
            hfg_PhoTrgPt30above_raw[ipt]->Fill(mass);
            hfg_PhoTrgPt30above[ipt]->Fill(mass, prscl[12]);// L1 base prescale 1
        } else if(id==1) {
            hfgdiff_PhoTrgPt30above_raw[ipt]->Fill(mass-mass_dau);
            hfgdiff_PhoTrgPt30above[ipt]->Fill(mass-mass_dau, prscl[12]);// L1 base prescale 1
        } else if(id==2) {
            hfgdiff_PhoTrgPt30above_raw[ipt]->Fill(mass-mass_dau);
            hfgdiff_PhoTrgPt30above[ipt]->Fill(mass-mass_dau, prscl[12]);// L1 base prescale 1
        }
    }
}

//
void ana_hfforest::FindMaxTrgPt(trigO* tobj, float& max)
{
    max = 0;
    for(unsigned int i = 0; i<tobj->size(); i++) {
        if(tobj->at(i).pt() > max)
            max = tobj->at(i).pt();
    }
}

//
void ana_hfforest::define_cuts()
{//.. define different cuts 

    //D*->D0+pi
    cut_m_low[0] = 1.92; 
    cut_m_high[0] = 2.10;
    cut_m_dau_low[0] = 1.8;
    cut_m_dau_high[0] = 1.92;
    cut_ffls3d[0] = 2.0;
    cut_falpha0[0] = 0.2;
    cut_fprob[0] = 0.05;
    cut_fdr[0] = 0.25;
    cut_fchi2[0] = 3;

    //D0->K+pi
    cut_m_low[1] = 1.70; 
    cut_m_high[1] = 2.05;
    cut_m_dau_low[1] = -1;
    cut_m_dau_high[1] = 1;
    cut_ffls3d[1] = 2.0;
    cut_falpha0[1] = 0.2;
    cut_fprob[1] = 0.05;
    cut_fdr[1] = 0.25;
    cut_fchi2[1] = 3;

    //Ds->phi+pi
    cut_m_low[2] = 1.80; 
    cut_m_high[2] = 2.15;
    cut_m_dau_low[2] = 1.0;
    cut_m_dau_high[2] = 1.04;
    cut_ffls3d[2] = 2.0;
    cut_falpha0[2] = 0.2;
    cut_fprob[2] = 0.05;
    cut_fdr[2] = 0.25;
    cut_fchi2[2] = 3;

    //Ds->K*+K
    cut_m_low[3] = 1.80; 
    cut_m_high[3] = 2.15;
    cut_m_dau_low[3] = 0.0;
    cut_m_dau_high[3] = 2.0;
    cut_ffls3d[3] = 2.0;
    cut_falpha0[3] = 0.2;
    cut_fprob[3] = 0.05;
    cut_fdr[3] = 0.25;
    cut_fchi2[3] = 3;

    //D+/- = K+pi+pi
    cut_m_low[4] = 1.70; 
    cut_m_high[4] = 2.05;
    cut_m_dau_low[4] = 0.5;
    cut_m_dau_high[4] = 2.05;
    cut_ffls3d[4] = 2.0;
    cut_falpha0[4] = 0.2;
    cut_fprob[4] = 0.05;
    cut_fdr[4] = 0.25;
    cut_fchi2[4] = 3;
}

//
void ana_hfforest::get_trg_name()
{//.. this code is made by: ./make_code_to_read_hlt_name_pscl.txt.pl
    //.. all Run2013 pPb triggers. Most are useless. Here is to check which
    //.. one has most of the D mesons. 
    //.. trigger list is from, e.g. 
    //.. https://cmswbm.web.cern.ch/cmswbm/cmsdb/servlet/HLTSummary?RUN=211631&KEY=28422

    trg_name[0]= "HLT_PAZeroBiasPixel_SingleTrack"; //.. dominate in minbiasUPC sample.
    trg_name[1]= "HLT_PAFullTrack12";   //.. base: L1_SingleJet12_BptxAND w/ prescl=1
    trg_name[2]= "HLT_PAFullTrack20";   //.. base: L1_SingleJet16_BptxAND w/ prescl=1
    trg_name[3]= "HLT_PAFullTrack30";   //.. base: L1_SingleJet16_BptxAND w/ prescl=1

    trg_name[4]= "HLT_PAJet20_NoJetID";   //.. base: L1_SingleJet16_BptxAND
    trg_name[5]= "HLT_PAJet40_NoJetID";   //.. base: L1_SingleJet16_BptxAND
    trg_name[6]= "HLT_PAJet60_NoJetID";   //.. base: L1_SingleJet36
    trg_name[7]= "HLT_PAJet80_NoJetID";   //.. base: L1_SingleJet36
    trg_name[8]= "HLT_PAJet100_NoJetID";   //.. base: L1_SingleJet36

    trg_name[9]= "HLT_PAPhoton10_NoCaloIdVL";   //.. base: L1_SingleEG5_BptxAND
    trg_name[10]= "HLT_PAPhoton15_NoCaloIdVL";   //.. base: L1_SingleEG5_BptxAND
    trg_name[11]= "HLT_PAPhoton20_NoCaloIdVL";   //.. base: L1_SingleEG5_BptxAND
    trg_name[12]= "HLT_PAPhoton30_NoCaloIdVL";   //.. base: L1_SingleEG12

    trg_name[13]= "L1_ZeroBias";  //.. base of HLT_PAZeroBiasPixel_SingleTrack
    trg_name[14]= "L1_SingleJet12_BptxAND";
    trg_name[15]= "L1_SingleJet16_BptxAND";
    trg_name[16]= "L1_SingleJet36";
    trg_name[17]= "L1_SingleEG5_BptxAND";
    trg_name[18]= "L1_SingleEG12";

    //.. high multiplicity triggers 
    trg_name[19]= "HLT_PAPixelTrackMultiplicity100_FullTrack12";
    trg_name[20]= "HLT_PAPixelTrackMultiplicity100_L2DoubleMu3";
    trg_name[21]= "HLT_PAPixelTrackMultiplicity130_FullTrack12";
    trg_name[22]= "HLT_PAPixelTrackMultiplicity140_Jet80_NoJetID";
    trg_name[23]= "HLT_PAPixelTrackMultiplicity160_FullTrack12";
    trg_name[24]= "HLT_PAPixelTracks_Multiplicity100";
    trg_name[25]= "HLT_PAPixelTracks_Multiplicity130";
    trg_name[26]= "HLT_PAPixelTracks_Multiplicity160";
    trg_name[27]= "HLT_PAPixelTracks_Multiplicity190";
    trg_name[28]= "HLT_PAPixelTracks_Multiplicity220";

}

//
void ana_hfforest::get_comb_hist(TFile* f)
{
    if(!f) {
        cout<<" !!! file does not exist,  exit"<<endl;
        exit(0);
    }

    cout<<" .... fetching combination file  ..."<<endl;
    char hname[100];
    for(short i = 0; i<NptRebin; i++) {
        sprintf(hname, "hfg_raw_combTrkTrg%d", i);
        hfg_raw_combTrkTrg[i] = (TH1F*)f->Get(hname);
        sprintf(hname, "hfg_combTrkTrg%d", i);
        hfg_combTrkTrg[i] = (TH1F*)f->Get(hname);
        sprintf(hname, "hfgdiff_raw_combTrkTrg%d", i);
        hfgdiff_raw_combTrkTrg[i] = (TH1F*)f->Get(hname);
        sprintf(hname, "hfgdiff_combTrkTrg%d", i);
        hfgdiff_combTrkTrg[i] = (TH1F*)f->Get(hname);
        sprintf(hname, "hbgdiff_raw_combTrkTrg%d", i);
        hbgdiff_raw_combTrkTrg[i] = (TH1F*)f->Get(hname);
        sprintf(hname, "hbgdiff_combTrkTrg%d", i);
        hbgdiff_combTrkTrg[i] = (TH1F*)f->Get(hname);

        sprintf(hname, "hfg_raw_combJetTrg%d", i);
        hfg_raw_combJetTrg[i] = (TH1F*)f->Get(hname);
        sprintf(hname, "hfg_combJetTrg%d", i);
        hfg_combJetTrg[i] = (TH1F*)f->Get(hname);
        sprintf(hname, "hfgdiff_raw_combJetTrg%d", i);
        hfgdiff_raw_combJetTrg[i] = (TH1F*)f->Get(hname);
        sprintf(hname, "hfgdiff_combJetTrg%d", i);
        hfgdiff_combJetTrg[i] = (TH1F*)f->Get(hname);
        sprintf(hname, "hbgdiff_raw_combJetTrg%d", i);
        hbgdiff_raw_combJetTrg[i] = (TH1F*)f->Get(hname);
        sprintf(hname, "hbgdiff_combJetTrg%d", i);
        hbgdiff_combJetTrg[i] = (TH1F*)f->Get(hname);

        sprintf(hname, "hfg_raw_combPhoTrg%d", i);
        hfg_raw_combPhoTrg[i] = (TH1F*)f->Get(hname);
        sprintf(hname, "hfg_combPhoTrg%d", i);
        hfg_combPhoTrg[i] = (TH1F*)f->Get(hname);
        sprintf(hname, "hfgdiff_raw_combPhoTrg%d", i);
        hfgdiff_raw_combPhoTrg[i] = (TH1F*)f->Get(hname);
        sprintf(hname, "hfgdiff_combPhoTrg%d", i);
        hfgdiff_combPhoTrg[i] = (TH1F*)f->Get(hname);
        sprintf(hname, "hbgdiff_raw_combPhoTrg%d", i);
        hbgdiff_raw_combPhoTrg[i] = (TH1F*)f->Get(hname);
        sprintf(hname, "hbgdiff_combPhoTrg%d", i);
        hbgdiff_combPhoTrg[i] = (TH1F*)f->Get(hname);
    }
    // reference hist for checking trigger combination ...
    for(short it = 0; it<NTRG; it++) {
        for(short i = 0; i<NptRebin; i++) {
            sprintf(hname, "hfg_raw_rebin%d_%d", it, i);
            hfg_raw_rebin[it][i] = (TH1F*)f->Get(hname);

            sprintf(hname, "hfgdiff_raw_rebin%d_%d", it, i);
            hfgdiff_raw_rebin[it][i] = (TH1F*)f->Get(hname);

            sprintf(hname, "hbgdiff_raw_rebin%d_%d", it, i);
            hbgdiff_raw_rebin[it][i] = (TH1F*)f->Get(hname);
        }
    }
    cout<<" .... done ..."<<endl;
}
//
void ana_hfforest::get_hist(TFile* f)
{
    if(!f) {
        cout<<" !!! file does not exist,  exit"<<endl;
        exit(0);
    }
    cout<<" .... fetching histogram ...."<<endl;
    char hname[100];

    // all D mesons
    for(short it = 0; it<NTRG; it++) 
        for(short i = 0; i<NPT; i++) {

            sprintf(hname, "hfg%d_%d", it, i);
            hfg[it][i] = (TH1F*)f->Get(hname);
            sprintf(hname, "hfg_raw%d_%d", it, i);
            hfg_raw[it][i] =  (TH1F*)f->Get(hname);
        }

    // D*
    for(short it = 0; it<NTRG; it++) 
        for(short i = 0; i<NPT; i++) {
            sprintf(hname, "hfgdiff%d_%d", it, i);
            hfgdiff[it][i] = (TH1F*)f->Get(hname);
            sprintf(hname, "hfgdiff_raw%d_%d", it, i);
            hfgdiff_raw[it][i] = (TH1F*)f->Get(hname);

              sprintf(hname, "hbgdiff%d_%d", it, i);
              hbgdiff[it][i] = (TH1F*)f->Get(hname);
              sprintf(hname, "hbgdiff_raw%d_%d", it, i);
              hbgdiff_raw[it][i] = (TH1F*)f->Get(hname);
        }


    // components for trigger combination for all D..
    for(short i = 0; i<NPT; i++) {

        sprintf(hname, "hfg_ZBiasSglTrkPt0_12_raw%d", i);
        hfg_ZBiasSglTrkPt0_12_raw[i] = (TH1F*)f->Get(hname);
        sprintf(hname, "hfg_ZBiasSglTrkPt0_12%d", i);
        hfg_ZBiasSglTrkPt0_12[i] = (TH1F*)f->Get(hname);

        sprintf(hname, "hfg_TrkTrgPt12_20_raw%d", i);
        hfg_TrkTrgPt12_20_raw[i] = (TH1F*)f->Get(hname);
        sprintf(hname, "hfg_TrkTrgPt12_20%d", i);
        hfg_TrkTrgPt12_20[i] = (TH1F*)f->Get(hname);

        sprintf(hname, "hfg_TrkTrgPt20_30_raw%d", i);
        hfg_TrkTrgPt20_30_raw[i] = (TH1F*)f->Get(hname);
        sprintf(hname, "hfg_TrkTrgPt20_30%d", i);
        hfg_TrkTrgPt20_30[i] = (TH1F*)f->Get(hname);

        sprintf(hname, "hfg_TrkTrgPt30above_raw%d", i);
        hfg_TrkTrgPt30above_raw[i] = (TH1F*)f->Get(hname);
        sprintf(hname, "hfg_TrkTrgPt30above%d", i);
        hfg_TrkTrgPt30above[i] = (TH1F*)f->Get(hname);

        //Jet trigger 
        sprintf(hname, "hfg_ZBiasSglTrkPt0_20_raw%d", i);
        hfg_ZBiasSglTrkPt0_20_raw[i] = (TH1F*)f->Get(hname);
        sprintf(hname, "hfg_ZBiasSglTrkPt0_20%d", i);
        hfg_ZBiasSglTrkPt0_20[i] = (TH1F*)f->Get(hname);

        sprintf(hname, "hfg_JetTrgPt20_40_raw%d", i);
        hfg_JetTrgPt20_40_raw[i] = (TH1F*)f->Get(hname);
        sprintf(hname, "hfg_JetTrgPt20_40%d", i);
        hfg_JetTrgPt20_40[i] = (TH1F*)f->Get(hname);

        sprintf(hname, "hfg_JetTrgPt40_60_raw%d", i);
        hfg_JetTrgPt40_60_raw[i] = (TH1F*)f->Get(hname);
        sprintf(hname, "hfg_JetTrgPt40_60%d", i);
        hfg_JetTrgPt40_60[i] = (TH1F*)f->Get(hname);

        sprintf(hname, "hfg_JetTrgPt60_80_raw%d", i);
        hfg_JetTrgPt60_80_raw[i] = (TH1F*)f->Get(hname);
        sprintf(hname, "hfg_JetTrgPt60_80%d", i);
        hfg_JetTrgPt60_80[i] = (TH1F*)f->Get(hname);

        sprintf(hname, "hfg_JetTrgPt80_100_raw%d", i);
        hfg_JetTrgPt80_100_raw[i] = (TH1F*)f->Get(hname);
        sprintf(hname, "hfg_JetTrgPt80_100%d", i);
        hfg_JetTrgPt80_100[i] = (TH1F*)f->Get(hname);

        sprintf(hname, "hfg_JetTrgPt100above_raw%d", i);
        hfg_JetTrgPt100above_raw[i] = (TH1F*)f->Get(hname);
        sprintf(hname, "hfg_JetTrgPt100above%d", i);
        hfg_JetTrgPt100above[i] = (TH1F*)f->Get(hname);

        //Photon trigger 
        sprintf(hname, "hfg_ZBiasSglTrkPt0_10_raw%d", i);
        hfg_ZBiasSglTrkPt0_10_raw[i] = (TH1F*)f->Get(hname);
        sprintf(hname, "hfg_ZBiasSglTrkPt0_10%d", i);
        hfg_ZBiasSglTrkPt0_10[i] = (TH1F*)f->Get(hname);

        sprintf(hname, "hfg_PhoTrgPt10_15_raw%d", i);
        hfg_PhoTrgPt10_15_raw[i] = (TH1F*)f->Get(hname);
        sprintf(hname, "hfg_PhoTrgPt10_15%d", i);
        hfg_PhoTrgPt10_15[i] = (TH1F*)f->Get(hname);

        sprintf(hname, "hfg_PhoTrgPt15_20_raw%d", i);
        hfg_PhoTrgPt15_20_raw[i] = (TH1F*)f->Get(hname);
        sprintf(hname, "hfg_PhoTrgPt15_20%d", i);
        hfg_PhoTrgPt15_20[i] = (TH1F*)f->Get(hname);

        sprintf(hname, "hfg_PhoTrgPt20_30_raw%d", i);
        hfg_PhoTrgPt20_30_raw[i] = (TH1F*)f->Get(hname);
        sprintf(hname, "hfg_PhoTrgPt20_30%d", i);
        hfg_PhoTrgPt20_30[i] = (TH1F*)f->Get(hname);

        sprintf(hname, "hfg_PhoTrgPt30above_raw%d", i);
        hfg_PhoTrgPt30above_raw[i] = (TH1F*)f->Get(hname);
        sprintf(hname, "hfg_PhoTrgPt30above%d", i);
        hfg_PhoTrgPt30above[i] = (TH1F*)f->Get(hname);
    }


    // components for trigger combination for D*
    for(short i = 0; i<NPT; i++) {

        // FullTrack trigger
        sprintf(hname, "hfgdiff_ZBiasSglTrkPt0_12_raw%d", i);
        hfgdiff_ZBiasSglTrkPt0_12_raw[i] = (TH1F*)f->Get(hname);
        sprintf(hname, "hfgdiff_ZBiasSglTrkPt0_12%d", i);
        hfgdiff_ZBiasSglTrkPt0_12[i] = (TH1F*)f->Get(hname);

        sprintf(hname, "hfgdiff_TrkTrgPt12_20_raw%d", i);
        hfgdiff_TrkTrgPt12_20_raw[i] = (TH1F*)f->Get(hname);
        sprintf(hname, "hfgdiff_TrkTrgPt12_20%d", i);
        hfgdiff_TrkTrgPt12_20[i] = (TH1F*)f->Get(hname);

        sprintf(hname, "hfgdiff_TrkTrgPt20_30_raw%d", i);
        hfgdiff_TrkTrgPt20_30_raw[i] = (TH1F*)f->Get(hname);
        sprintf(hname, "hfgdiff_TrkTrgPt20_30%d", i);
        hfgdiff_TrkTrgPt20_30[i] = (TH1F*)f->Get(hname);

        sprintf(hname, "hfgdiff_TrkTrgPt30above_raw%d", i);
        hfgdiff_TrkTrgPt30above_raw[i] = (TH1F*)f->Get(hname);
        sprintf(hname, "hfgdiff_TrkTrgPt30above%d", i);
        hfgdiff_TrkTrgPt30above[i] = (TH1F*)f->Get(hname);


        sprintf(hname, "hbgdiff_ZBiasSglTrkPt0_12_raw%d", i);
        hbgdiff_ZBiasSglTrkPt0_12_raw[i] = (TH1F*)f->Get(hname);
        sprintf(hname, "hbgdiff_ZBiasSglTrkPt0_12%d", i);
        hbgdiff_ZBiasSglTrkPt0_12[i] = (TH1F*)f->Get(hname);

        sprintf(hname, "hbgdiff_TrkTrgPt12_20_raw%d", i);
        hbgdiff_TrkTrgPt12_20_raw[i] = (TH1F*)f->Get(hname);
        sprintf(hname, "hbgdiff_TrkTrgPt12_20%d", i);
        hbgdiff_TrkTrgPt12_20[i] = (TH1F*)f->Get(hname);

        sprintf(hname, "hbgdiff_TrkTrgPt20_30_raw%d", i);
        hbgdiff_TrkTrgPt20_30_raw[i] = (TH1F*)f->Get(hname);
        sprintf(hname, "hbgdiff_TrkTrgPt20_30%d", i);
        hbgdiff_TrkTrgPt20_30[i] = (TH1F*)f->Get(hname);

        sprintf(hname, "hbgdiff_TrkTrgPt30above_raw%d", i);
        hbgdiff_TrkTrgPt30above_raw[i] = (TH1F*)f->Get(hname);
        sprintf(hname, "hbgdiff_TrkTrgPt30above%d", i);
        hbgdiff_TrkTrgPt30above[i] = (TH1F*)f->Get(hname);

        // Jet trigger
        sprintf(hname, "hfgdiff_ZBiasSglTrkPt0_20_raw%d", i);
        hfgdiff_ZBiasSglTrkPt0_20_raw[i] = (TH1F*)f->Get(hname);
        sprintf(hname, "hfgdiff_ZBiasSglTrkPt0_20%d", i);
        hfgdiff_ZBiasSglTrkPt0_20[i] = (TH1F*)f->Get(hname);

        sprintf(hname, "hfgdiff_JetTrgPt20_40_raw%d", i);
        hfgdiff_JetTrgPt20_40_raw[i] = (TH1F*)f->Get(hname);
        sprintf(hname, "hfgdiff_JetTrgPt20_40%d", i);
        hfgdiff_JetTrgPt20_40[i] = (TH1F*)f->Get(hname);

        sprintf(hname, "hfgdiff_JetTrgPt40_60_raw%d", i);
        hfgdiff_JetTrgPt40_60_raw[i] = (TH1F*)f->Get(hname);
        sprintf(hname, "hfgdiff_JetTrgPt40_60%d", i);
        hfgdiff_JetTrgPt40_60[i] = (TH1F*)f->Get(hname);

        sprintf(hname, "hfgdiff_JetTrgPt60_80_raw%d", i);
        hfgdiff_JetTrgPt60_80_raw[i] = (TH1F*)f->Get(hname);
        sprintf(hname, "hfgdiff_JetTrgPt60_80%d", i);
        hfgdiff_JetTrgPt60_80[i] = (TH1F*)f->Get(hname);

        sprintf(hname, "hfgdiff_JetTrgPt80_100_raw%d", i);
        hfgdiff_JetTrgPt80_100_raw[i] = (TH1F*)f->Get(hname);
        sprintf(hname, "hfgdiff_JetTrgPt80_100%d", i);
        hfgdiff_JetTrgPt80_100[i] = (TH1F*)f->Get(hname);

        sprintf(hname, "hfgdiff_JetTrgPt100above_raw%d", i);
        hfgdiff_JetTrgPt100above_raw[i] = (TH1F*)f->Get(hname);
        sprintf(hname, "hfgdiff_JetTrgPt100above%d", i);
        hfgdiff_JetTrgPt100above[i] = (TH1F*)f->Get(hname);


        sprintf(hname, "hbgdiff_ZBiasSglTrkPt0_20_raw%d", i);
        hbgdiff_ZBiasSglTrkPt0_20_raw[i] = (TH1F*)f->Get(hname);
        sprintf(hname, "hbgdiff_ZBiasSglTrkPt0_20%d", i);
        hbgdiff_ZBiasSglTrkPt0_20[i] = (TH1F*)f->Get(hname);

        sprintf(hname, "hbgdiff_JetTrgPt20_40_raw%d", i);
        hbgdiff_JetTrgPt20_40_raw[i] = (TH1F*)f->Get(hname);
        sprintf(hname, "hbgdiff_JetTrgPt20_40%d", i);
        hbgdiff_JetTrgPt20_40[i] = (TH1F*)f->Get(hname);

        sprintf(hname, "hbgdiff_JetTrgPt40_60_raw%d", i);
        hbgdiff_JetTrgPt40_60_raw[i] = (TH1F*)f->Get(hname);
        sprintf(hname, "hbgdiff_JetTrgPt40_60%d", i);
        hbgdiff_JetTrgPt40_60[i] = (TH1F*)f->Get(hname);

        sprintf(hname, "hbgdiff_JetTrgPt60_80_raw%d", i);
        hbgdiff_JetTrgPt60_80_raw[i] = (TH1F*)f->Get(hname);
        sprintf(hname, "hbgdiff_JetTrgPt60_80%d", i);
        hbgdiff_JetTrgPt60_80[i] = (TH1F*)f->Get(hname);

        sprintf(hname, "hbgdiff_JetTrgPt80_100_raw%d", i);
        hbgdiff_JetTrgPt80_100_raw[i] = (TH1F*)f->Get(hname);
        sprintf(hname, "hbgdiff_JetTrgPt80_100%d", i);
        hbgdiff_JetTrgPt80_100[i] = (TH1F*)f->Get(hname);

        sprintf(hname, "hbgdiff_JetTrgPt100above_raw%d", i);
        hbgdiff_JetTrgPt100above_raw[i] = (TH1F*)f->Get(hname);
        sprintf(hname, "hbgdiff_JetTrgPt100above%d", i);
        hbgdiff_JetTrgPt100above[i] = (TH1F*)f->Get(hname);

        // Photon trigger
        sprintf(hname, "hfgdiff_ZBiasSglTrkPt0_10_raw%d", i);
        hfgdiff_ZBiasSglTrkPt0_10_raw[i] = (TH1F*)f->Get(hname);
        sprintf(hname, "hfgdiff_ZBiasSglTrkPt0_10%d", i);
        hfgdiff_ZBiasSglTrkPt0_10[i] = (TH1F*)f->Get(hname);

        sprintf(hname, "hfgdiff_PhoTrgPt10_15_raw%d", i);
        hfgdiff_PhoTrgPt10_15_raw[i] = (TH1F*)f->Get(hname);
        sprintf(hname, "hfgdiff_PhoTrgPt10_15%d", i);
        hfgdiff_PhoTrgPt10_15[i] = (TH1F*)f->Get(hname);

        sprintf(hname, "hfgdiff_PhoTrgPt15_20_raw%d", i);
        hfgdiff_PhoTrgPt15_20_raw[i] = (TH1F*)f->Get(hname);
        sprintf(hname, "hfgdiff_PhoTrgPt15_20%d", i);
        hfgdiff_PhoTrgPt15_20[i] = (TH1F*)f->Get(hname);

        sprintf(hname, "hfgdiff_PhoTrgPt20_30_raw%d", i);
        hfgdiff_PhoTrgPt20_30_raw[i] = (TH1F*)f->Get(hname);
        sprintf(hname, "hfgdiff_PhoTrgPt20_30%d", i);
        hfgdiff_PhoTrgPt20_30[i] = (TH1F*)f->Get(hname);

        sprintf(hname, "hfgdiff_PhoTrgPt30above_raw%d", i);
        hfgdiff_PhoTrgPt30above_raw[i] = (TH1F*)f->Get(hname);
        sprintf(hname, "hfgdiff_PhoTrgPt30above%d", i);
        hfgdiff_PhoTrgPt30above[i] = (TH1F*)f->Get(hname);

        sprintf(hname, "hbgdiff_ZBiasSglTrkPt0_10_raw%d", i);
        hbgdiff_ZBiasSglTrkPt0_10_raw[i] = (TH1F*)f->Get(hname);
        sprintf(hname, "hbgdiff_ZBiasSglTrkPt0_10%d", i);
        hbgdiff_ZBiasSglTrkPt0_10[i] = (TH1F*)f->Get(hname);

        sprintf(hname, "hbgdiff_PhoTrgPt10_15_raw%d", i);
        hbgdiff_PhoTrgPt10_15_raw[i] = (TH1F*)f->Get(hname);
        sprintf(hname, "hbgdiff_PhoTrgPt10_15%d", i);
        hbgdiff_PhoTrgPt10_15[i] = (TH1F*)f->Get(hname);

        sprintf(hname, "hbgdiff_PhoTrgPt15_20_raw%d", i);
        hbgdiff_PhoTrgPt15_20_raw[i] = (TH1F*)f->Get(hname);
        sprintf(hname, "hbgdiff_PhoTrgPt15_20%d", i);
        hbgdiff_PhoTrgPt15_20[i] = (TH1F*)f->Get(hname);

        sprintf(hname, "hbgdiff_PhoTrgPt20_30_raw%d", i);
        hbgdiff_PhoTrgPt20_30_raw[i] = (TH1F*)f->Get(hname);
        sprintf(hname, "hbgdiff_PhoTrgPt20_30%d", i);
        hbgdiff_PhoTrgPt20_30[i] = (TH1F*)f->Get(hname);

        sprintf(hname, "hbgdiff_PhoTrgPt30above_raw%d", i);
        hbgdiff_PhoTrgPt30above_raw[i] = (TH1F*)f->Get(hname);
        sprintf(hname, "hbgdiff_PhoTrgPt30above%d", i);
        hbgdiff_PhoTrgPt30above[i] = (TH1F*)f->Get(hname);
    }
    cout<<" .... done ...."<<endl;
}
//
//
void ana_hfforest::define_rebinSpec()
{
    //clone any mass spectrum to define the combined one then reset them to 0 contents
    char hname[200];
    for(short it = 0; it<NTRG; it++) 
        for(short i = 0; i<NptRebin; i++) {
            sprintf(hname, "hfg_raw_rebin%d_%d", it, i);
            hfg_raw_rebin[it][i] = (TH1F*)hfg_raw[it][i]->Clone(hname);

            sprintf(hname, "hfgdiff_raw_rebin%d_%d", it, i);
            hfgdiff_raw_rebin[it][i] = (TH1F*)hfgdiff_raw[it][i]->Clone(hname);

            sprintf(hname, "hbgdiff_raw_rebin%d_%d", it, i);
            hbgdiff_raw_rebin[it][i] = (TH1F*)hbgdiff_raw[it][i]->Clone(hname);

            hfg_raw_rebin[it][i]->Reset();
            hfgdiff_raw_rebin[it][i]->Reset();
            hbgdiff_raw_rebin[it][i]->Reset();
        }
}

//
void ana_hfforest::define_combSpec()
{
    //clone any mass spectrum to define the combined one then reset them to 0 contents
    char hname[200];
    for(short i = 0; i<NptRebin; i++) {
        //FullTrack trigger 
        sprintf(hname, "hfg_raw_combTrkTrg%d", i);
        hfg_raw_combTrkTrg[i] = (TH1F*)hfg_TrkTrgPt12_20_raw[i]->Clone(hname);
        sprintf(hname, "hfg_combTrkTrg%d", i);
        hfg_combTrkTrg[i] = (TH1F*)hfg_TrkTrgPt12_20_raw[i]->Clone(hname);

        sprintf(hname, "hfgdiff_raw_combTrkTrg%d", i);
        hfgdiff_raw_combTrkTrg[i] = (TH1F*)hfgdiff_TrkTrgPt12_20_raw[i]->Clone(hname);
        sprintf(hname, "hfgdiff_combTrkTrg%d", i);
        hfgdiff_combTrkTrg[i] = (TH1F*)hfgdiff_TrkTrgPt12_20_raw[i]->Clone(hname);

        sprintf(hname, "hbgdiff_raw_combTrkTrg%d", i);
        hbgdiff_raw_combTrkTrg[i] = (TH1F*)hfgdiff_TrkTrgPt12_20_raw[i]->Clone(hname);
        sprintf(hname, "hbgdiff_combTrkTrg%d", i);
        hbgdiff_combTrkTrg[i] = (TH1F*)hfgdiff_TrkTrgPt12_20_raw[i]->Clone(hname);

        //Jet trigger 
        sprintf(hname, "hfg_raw_combJetTrg%d", i);
        hfg_raw_combJetTrg[i] = (TH1F*)hfg_TrkTrgPt12_20_raw[i]->Clone(hname);
        sprintf(hname, "hfg_combJetTrg%d", i);
        hfg_combJetTrg[i] = (TH1F*)hfg_TrkTrgPt12_20_raw[i]->Clone(hname);

        sprintf(hname, "hfgdiff_raw_combJetTrg%d", i);
        hfgdiff_raw_combJetTrg[i] = (TH1F*)hfgdiff_TrkTrgPt12_20_raw[i]->Clone(hname);
        sprintf(hname, "hfgdiff_combJetTrg%d", i);
        hfgdiff_combJetTrg[i] = (TH1F*)hfgdiff_TrkTrgPt12_20_raw[i]->Clone(hname);

        sprintf(hname, "hbgdiff_raw_combJetTrg%d", i);
        hbgdiff_raw_combJetTrg[i] = (TH1F*)hfgdiff_TrkTrgPt12_20_raw[i]->Clone(hname);
        sprintf(hname, "hbgdiff_combJetTrg%d", i);
        hbgdiff_combJetTrg[i] = (TH1F*)hfgdiff_TrkTrgPt12_20_raw[i]->Clone(hname);

        //Photon trigger 
        sprintf(hname, "hfg_raw_combPhoTrg%d", i);
        hfg_raw_combPhoTrg[i] = (TH1F*)hfg_TrkTrgPt12_20_raw[i]->Clone(hname);
        sprintf(hname, "hfg_combPhoTrg%d", i);
        hfg_combPhoTrg[i] = (TH1F*)hfg_TrkTrgPt12_20_raw[i]->Clone(hname);

        sprintf(hname, "hfgdiff_raw_combPhoTrg%d", i);
        hfgdiff_raw_combPhoTrg[i] = (TH1F*)hfgdiff_TrkTrgPt12_20_raw[i]->Clone(hname);
        sprintf(hname, "hfgdiff_combPhoTrg%d", i);
        hfgdiff_combPhoTrg[i] = (TH1F*)hfgdiff_TrkTrgPt12_20_raw[i]->Clone(hname);

        sprintf(hname, "hbgdiff_raw_combPhoTrg%d", i);
        hbgdiff_raw_combPhoTrg[i] = (TH1F*)hfgdiff_TrkTrgPt12_20_raw[i]->Clone(hname);
        sprintf(hname, "hbgdiff_combPhoTrg%d", i);
        hbgdiff_combPhoTrg[i] = (TH1F*)hfgdiff_TrkTrgPt12_20_raw[i]->Clone(hname);

        //.. now reset all bin to zero 
        hfg_raw_combTrkTrg[i]->Reset();
        hfg_combTrkTrg[i]->Reset();
        hfgdiff_raw_combTrkTrg[i]->Reset();
        hfgdiff_combTrkTrg[i]->Reset();
        hbgdiff_raw_combTrkTrg[i]->Reset();
        hbgdiff_combTrkTrg[i]->Reset();

        //Jet trigger 
        hfg_raw_combJetTrg[i]->Reset();
        hfg_combJetTrg[i]->Reset();
        hfgdiff_raw_combJetTrg[i]->Reset();
        hfgdiff_combJetTrg[i]->Reset();
        hbgdiff_raw_combJetTrg[i]->Reset();
        hbgdiff_combJetTrg[i]->Reset();

        //Photon trigger 
        hfg_raw_combPhoTrg[i]->Reset();
        hfg_combPhoTrg[i]->Reset();
        hfgdiff_raw_combPhoTrg[i]->Reset();
        hfgdiff_combPhoTrg[i]->Reset();
        hbgdiff_raw_combPhoTrg[i]->Reset();
        hbgdiff_combPhoTrg[i]->Reset();
    }
}
//
void ana_hfforest::rebinSpec(TFile* fout)
{
    cout<<".... rebining the spectra ...."<<endl;
    define_rebinSpec();

    for(short it = 0; it<NTRG; it++) {
        int im = 0; //.. bin id for merged histogram
        for(short i = 0; i<NPT; i++) {

            float pt_low = 0, pt_high = 0;
            get_pt_range(i, pt_low, pt_high);
            //
            //.. check if the merged bin range is incorrectly assigned...
            if(pt_low < ptRebin[im+1] && pt_high > ptRebin[im+1]) {
                cout<<"!!  merged bin range: "<<ptRebin[im]<<" - "<<ptRebin[im+1]<<" is not a integer number of the basic bin range: "<<pt_low<<" - "<<pt_high<<". exit !!"<<endl;
                exit(0);
            }
            //.. check if merging of one bin is done
            if(pt_low >= ptRebin[im+1] && pt_high <= ptRebin[im+2])
                im++;

            //Track trigger
            if(hfg[it][i]->Integral())
                hfg_raw[it][i]->Scale(hfg[it][i]->Integral()/hfg_raw[it][i]->Integral());
            if(hfgdiff[it][i]->Integral())
                hfgdiff_raw[it][i]->Scale(hfgdiff[it][i]->Integral()/hfgdiff_raw[it][i]->Integral());
            if(hbgdiff[it][i]->Integral())
                hbgdiff_raw[it][i]->Scale(hbgdiff[it][i]->Integral()/hbgdiff_raw[it][i]->Integral());

            hfg_raw_rebin[it][im]->Add(hfg_raw[it][i]);
            hfgdiff_raw_rebin[it][im]->Add(hfgdiff_raw[it][i]);
            hbgdiff_raw_rebin[it][im]->Add(hbgdiff_raw[it][i]);
        }
    }

    cout<<" ....Writing the spectra ...."<<endl;
    fout->cd();
    //.. save 
    for(short it = 0; it<NTRG; it++) {
        for(short i = 0; i<NptRebin; i++) {
            // track trigger 
            hfg_raw_rebin[it][i]->Write(); 
            hfgdiff_raw_rebin[it][i]->Write(); 
            hbgdiff_raw_rebin[it][i]->Write(); 
        }
    }
    cout<<"... done ..."<<endl;
}

//
void ana_hfforest::CombineSpec(TFile* fout)
{
    cout<<".... combining the spectra ...."<<endl;
    define_combSpec();

    int im = 0; //.. bin id for merged histogram
    for(short i = 0; i<NPT; i++) {

        float pt_low = 0, pt_high = 0;
        get_pt_range(i, pt_low, pt_high);
        //
        //.. check if the merged bin range is incorrectly assigned...
        if(pt_low < ptRebin[im+1] && pt_high > ptRebin[im+1]) {
            cout<<"!!  merged bin range: "<<ptRebin[im]<<" - "<<ptRebin[im+1]<<" is not a integer number of the basic bin range: "<<pt_low<<" - "<<pt_high<<". exit !!"<<endl;
            exit(0);
        }
        //.. check if merging of one bin is done
        if(pt_low >= ptRebin[im+1] && pt_high <= ptRebin[im+2])
            im++;

        //Track trigger
        if(hfg_TrkTrgPt12_20_raw[i]->Integral())
            hfg_TrkTrgPt12_20_raw[i]->Scale(hfg_TrkTrgPt12_20[i]->Integral()/hfg_TrkTrgPt12_20_raw[i]->Integral());
        if(hfg_TrkTrgPt20_30_raw[i]->Integral())
            hfg_TrkTrgPt20_30_raw[i]->Scale(hfg_TrkTrgPt20_30[i]->Integral()/hfg_TrkTrgPt20_30_raw[i]->Integral());
        if(hfg_TrkTrgPt30above_raw[i]->Integral())
            hfg_TrkTrgPt30above_raw[i]->Scale(hfg_TrkTrgPt30above[i]->Integral()/hfg_TrkTrgPt30above_raw[i]->Integral());

        hfg_raw_combTrkTrg[im]->Add(hfg_TrkTrgPt12_20_raw[i]);
        hfg_raw_combTrkTrg[im]->Add(hfg_TrkTrgPt20_30_raw[i]);
        hfg_raw_combTrkTrg[im]->Add(hfg_TrkTrgPt30above_raw[i]);

        hfg_combTrkTrg[im]->Add(hfg_TrkTrgPt12_20[i]);
        hfg_combTrkTrg[im]->Add(hfg_TrkTrgPt20_30[i]);
        hfg_combTrkTrg[im]->Add(hfg_TrkTrgPt30above[i]);
        //
        //
        //Jet trigger 
        if(hfg_JetTrgPt20_40_raw[i]->Integral())
            hfg_JetTrgPt20_40_raw[i]->Scale(hfg_JetTrgPt20_40[i]->Integral()/hfg_JetTrgPt20_40_raw[i]->Integral());
        if(hfg_JetTrgPt40_60_raw[i]->Integral())
            hfg_JetTrgPt40_60_raw[i]->Scale(hfg_JetTrgPt40_60[i]->Integral()/hfg_JetTrgPt40_60_raw[i]->Integral());
        if(hfg_JetTrgPt60_80_raw[i]->Integral())
            hfg_JetTrgPt60_80_raw[i]->Scale(hfg_JetTrgPt60_80[i]->Integral()/hfg_JetTrgPt60_80_raw[i]->Integral());
        if(hfg_JetTrgPt80_100_raw[i]->Integral())
            hfg_JetTrgPt80_100_raw[i]->Scale(hfg_JetTrgPt80_100[i]->Integral()/hfg_JetTrgPt80_100_raw[i]->Integral());
        if(hfg_JetTrgPt100above_raw[i]->Integral())
            hfg_JetTrgPt100above_raw[i]->Scale(hfg_JetTrgPt100above[i]->Integral()/hfg_JetTrgPt100above_raw[i]->Integral());

        hfg_raw_combJetTrg[im]->Add( hfg_JetTrgPt20_40_raw[i]);
        hfg_raw_combJetTrg[im]->Add(hfg_JetTrgPt40_60_raw[i]);
        hfg_raw_combJetTrg[im]->Add(hfg_JetTrgPt60_80_raw[i]);
        hfg_raw_combJetTrg[im]->Add(hfg_JetTrgPt80_100_raw[i]);
        hfg_raw_combJetTrg[im]->Add(hfg_JetTrgPt100above_raw[i]);

        hfg_combJetTrg[im]->Add( hfg_JetTrgPt20_40[i]);
        hfg_combJetTrg[im]->Add(hfg_JetTrgPt40_60[i]);
        hfg_combJetTrg[im]->Add(hfg_JetTrgPt60_80[i]);
        hfg_combJetTrg[im]->Add(hfg_JetTrgPt80_100[i]);
        hfg_combJetTrg[im]->Add(hfg_JetTrgPt100above[i]);



        //Photon trigger 
        if(hfg_PhoTrgPt10_15_raw[i]->Integral())
            hfg_PhoTrgPt10_15_raw[i]->Scale(hfg_PhoTrgPt10_15[i]->Integral()/hfg_PhoTrgPt10_15_raw[i]->Integral());
        if(hfg_PhoTrgPt15_20_raw[i]->Integral())
            hfg_PhoTrgPt15_20_raw[i]->Scale(hfg_PhoTrgPt15_20[i]->Integral()/hfg_PhoTrgPt15_20_raw[i]->Integral());
        if(hfg_PhoTrgPt20_30_raw[i]->Integral())
            hfg_PhoTrgPt20_30_raw[i]->Scale(hfg_PhoTrgPt20_30[i]->Integral()/hfg_PhoTrgPt20_30_raw[i]->Integral());
        if(hfg_PhoTrgPt30above_raw[i]->Integral())
            hfg_PhoTrgPt30above_raw[i]->Scale(hfg_PhoTrgPt30above[i]->Integral()/hfg_PhoTrgPt30above_raw[i]->Integral());

        hfg_raw_combPhoTrg[im]->Add(hfg_PhoTrgPt10_15_raw[i]);
        hfg_raw_combPhoTrg[im]->Add(hfg_PhoTrgPt15_20_raw[i]);
        hfg_raw_combPhoTrg[im]->Add(hfg_PhoTrgPt20_30_raw[i]);
        hfg_raw_combPhoTrg[im]->Add(hfg_PhoTrgPt30above_raw[i]);

        hfg_combPhoTrg[im]->Add( hfg_PhoTrgPt10_15[i]);
        hfg_combPhoTrg[im]->Add(hfg_PhoTrgPt15_20[i]);
        hfg_combPhoTrg[im]->Add(hfg_PhoTrgPt20_30[i]);
        hfg_combPhoTrg[im]->Add(hfg_PhoTrgPt30above[i]);

        // components for trigger combination for D*
        // fulltrack trigger
        if(hfgdiff_TrkTrgPt12_20_raw[i]->Integral())
            hfgdiff_TrkTrgPt12_20_raw[i]->Scale(hfgdiff_TrkTrgPt12_20[i]->Integral()/hfgdiff_TrkTrgPt12_20_raw[i]->Integral());
        if(hfgdiff_TrkTrgPt20_30_raw[i]->Integral())
            hfgdiff_TrkTrgPt20_30_raw[i]->Scale(hfgdiff_TrkTrgPt20_30[i]->Integral()/hfgdiff_TrkTrgPt20_30_raw[i]->Integral());
        if(hfgdiff_TrkTrgPt30above_raw[i]->Integral())
            hfgdiff_TrkTrgPt30above_raw[i]->Scale(hfgdiff_TrkTrgPt30above[i]->Integral()/hfgdiff_TrkTrgPt30above_raw[i]->Integral());

        hfgdiff_raw_combTrkTrg[im]->Add( hfgdiff_TrkTrgPt12_20_raw[i]);
        hfgdiff_raw_combTrkTrg[im]->Add(hfgdiff_TrkTrgPt20_30_raw[i]);
        hfgdiff_raw_combTrkTrg[im]->Add(hfgdiff_TrkTrgPt30above_raw[i]);

        hfgdiff_combTrkTrg[im]->Add( hfgdiff_TrkTrgPt12_20[i]);
        hfgdiff_combTrkTrg[im]->Add(hfgdiff_TrkTrgPt20_30[i]);
        hfgdiff_combTrkTrg[im]->Add(hfgdiff_TrkTrgPt30above[i]);

        //
        if(hbgdiff_TrkTrgPt12_20_raw[i]->Integral())
            hbgdiff_TrkTrgPt12_20_raw[i]->Scale(hbgdiff_TrkTrgPt12_20[i]->Integral()/hbgdiff_TrkTrgPt12_20_raw[i]->Integral());
        if(hbgdiff_TrkTrgPt20_30_raw[i]->Integral())
            hbgdiff_TrkTrgPt20_30_raw[i]->Scale(hbgdiff_TrkTrgPt20_30[i]->Integral()/hbgdiff_TrkTrgPt20_30_raw[i]->Integral());
        if(hbgdiff_TrkTrgPt30above_raw[i]->Integral())
            hbgdiff_TrkTrgPt30above_raw[i]->Scale(hbgdiff_TrkTrgPt30above[i]->Integral()/hbgdiff_TrkTrgPt30above_raw[i]->Integral());

        hbgdiff_raw_combTrkTrg[im]->Add( hbgdiff_TrkTrgPt12_20_raw[i]);
        hbgdiff_raw_combTrkTrg[im]->Add(hbgdiff_TrkTrgPt20_30_raw[i]);
        hbgdiff_raw_combTrkTrg[im]->Add(hbgdiff_TrkTrgPt30above_raw[i]);

        hbgdiff_combTrkTrg[im]->Add( hbgdiff_TrkTrgPt12_20[i]);
        hbgdiff_combTrkTrg[im]->Add(hbgdiff_TrkTrgPt20_30[i]);
        hbgdiff_combTrkTrg[im]->Add(hbgdiff_TrkTrgPt30above[i]);


        // Jet trigger
        if(hfgdiff_JetTrgPt20_40_raw[i]->Integral())
            hfgdiff_JetTrgPt20_40_raw[i]->Scale(hfgdiff_JetTrgPt20_40[i]->Integral()/hfgdiff_JetTrgPt20_40_raw[i]->Integral());
        if(hfgdiff_JetTrgPt40_60_raw[i]->Integral())
            hfgdiff_JetTrgPt40_60_raw[i]->Scale(hfgdiff_JetTrgPt40_60[i]->Integral()/hfgdiff_JetTrgPt40_60_raw[i]->Integral());
        if(hfgdiff_JetTrgPt60_80_raw[i]->Integral())
            hfgdiff_JetTrgPt60_80_raw[i]->Scale(hfgdiff_JetTrgPt60_80[i]->Integral()/hfgdiff_JetTrgPt60_80_raw[i]->Integral());
        if(hfgdiff_JetTrgPt80_100_raw[i]->Integral())
            hfgdiff_JetTrgPt80_100_raw[i]->Scale(hfgdiff_JetTrgPt80_100[i]->Integral()/hfgdiff_JetTrgPt80_100_raw[i]->Integral());
        if(hfgdiff_JetTrgPt100above_raw[i]->Integral())
            hfgdiff_JetTrgPt100above_raw[i]->Scale(hfgdiff_JetTrgPt100above[i]->Integral()/hfgdiff_JetTrgPt100above_raw[i]->Integral());

        hfgdiff_raw_combJetTrg[im]->Add(hfgdiff_JetTrgPt20_40_raw[i]);
        hfgdiff_raw_combJetTrg[im]->Add(hfgdiff_JetTrgPt40_60_raw[i]);
        hfgdiff_raw_combJetTrg[im]->Add(hfgdiff_JetTrgPt60_80_raw[i]);
        hfgdiff_raw_combJetTrg[im]->Add(hfgdiff_JetTrgPt80_100_raw[i]);
        hfgdiff_raw_combJetTrg[im]->Add(hfgdiff_JetTrgPt100above_raw[i]);

        hfgdiff_combJetTrg[im]->Add(hfgdiff_JetTrgPt20_40[i]);
        hfgdiff_combJetTrg[im]->Add(hfgdiff_JetTrgPt40_60[i]);
        hfgdiff_combJetTrg[im]->Add(hfgdiff_JetTrgPt60_80[i]);
        hfgdiff_combJetTrg[im]->Add(hfgdiff_JetTrgPt80_100[i]);
        hfgdiff_combJetTrg[im]->Add(hfgdiff_JetTrgPt100above[i]);


        //
        if(hbgdiff_JetTrgPt20_40_raw[i]->Integral())
            hbgdiff_JetTrgPt20_40_raw[i]->Scale(hbgdiff_JetTrgPt20_40[i]->Integral()/hbgdiff_JetTrgPt20_40_raw[i]->Integral());
        if(hbgdiff_JetTrgPt40_60_raw[i]->Integral())
            hbgdiff_JetTrgPt40_60_raw[i]->Scale(hbgdiff_JetTrgPt40_60[i]->Integral()/hbgdiff_JetTrgPt40_60_raw[i]->Integral());
        if(hbgdiff_JetTrgPt60_80_raw[i]->Integral())
            hbgdiff_JetTrgPt60_80_raw[i]->Scale(hbgdiff_JetTrgPt60_80[i]->Integral()/hbgdiff_JetTrgPt60_80_raw[i]->Integral());
        if(hbgdiff_JetTrgPt80_100_raw[i]->Integral())
            hbgdiff_JetTrgPt80_100_raw[i]->Scale(hbgdiff_JetTrgPt80_100[i]->Integral()/hbgdiff_JetTrgPt80_100_raw[i]->Integral());
        if(hbgdiff_JetTrgPt100above_raw[i]->Integral())
            hbgdiff_JetTrgPt100above_raw[i]->Scale(hbgdiff_JetTrgPt100above[i]->Integral()/hbgdiff_JetTrgPt100above_raw[i]->Integral());

        hbgdiff_raw_combJetTrg[im]->Add(hbgdiff_JetTrgPt20_40_raw[i]);
        hbgdiff_raw_combJetTrg[im]->Add(hbgdiff_JetTrgPt40_60_raw[i]);
        hbgdiff_raw_combJetTrg[im]->Add(hbgdiff_JetTrgPt60_80_raw[i]);
        hbgdiff_raw_combJetTrg[im]->Add(hbgdiff_JetTrgPt80_100_raw[i]);
        hbgdiff_raw_combJetTrg[im]->Add(hbgdiff_JetTrgPt100above_raw[i]);

        hbgdiff_combJetTrg[im]->Add(hbgdiff_JetTrgPt20_40[i]);
        hbgdiff_combJetTrg[im]->Add(hbgdiff_JetTrgPt40_60[i]);
        hbgdiff_combJetTrg[im]->Add(hbgdiff_JetTrgPt60_80[i]);
        hbgdiff_combJetTrg[im]->Add(hbgdiff_JetTrgPt80_100[i]);
        hbgdiff_combJetTrg[im]->Add(hbgdiff_JetTrgPt100above[i]);


        // Photon trigger
        if(hfgdiff_PhoTrgPt10_15_raw[i]->Integral())
            hfgdiff_PhoTrgPt10_15_raw[i]->Scale(hfgdiff_PhoTrgPt10_15[i]->Integral()/hfgdiff_PhoTrgPt10_15_raw[i]->Integral());
        if(hfgdiff_PhoTrgPt15_20_raw[i]->Integral())
            hfgdiff_PhoTrgPt15_20_raw[i]->Scale(hfgdiff_PhoTrgPt15_20[i]->Integral()/hfgdiff_PhoTrgPt15_20_raw[i]->Integral());
        if(hfgdiff_PhoTrgPt20_30_raw[i]->Integral())
            hfgdiff_PhoTrgPt20_30_raw[i]->Scale(hfgdiff_PhoTrgPt20_30[i]->Integral()/hfgdiff_PhoTrgPt20_30_raw[i]->Integral());
        if(hfgdiff_PhoTrgPt30above_raw[i]->Integral())
            hfgdiff_PhoTrgPt30above_raw[i]->Scale(hfgdiff_PhoTrgPt30above[i]->Integral()/hfgdiff_PhoTrgPt30above_raw[i]->Integral());

        hfgdiff_raw_combPhoTrg[im]->Add(hfgdiff_PhoTrgPt10_15_raw[i]);
        hfgdiff_raw_combPhoTrg[im]->Add(hfgdiff_PhoTrgPt15_20_raw[i]);
        hfgdiff_raw_combPhoTrg[im]->Add(hfgdiff_PhoTrgPt20_30_raw[i]);
        hfgdiff_raw_combPhoTrg[im]->Add(hfgdiff_PhoTrgPt30above_raw[i]);

        hfgdiff_combPhoTrg[im]->Add(hfgdiff_PhoTrgPt10_15[i]);
        hfgdiff_combPhoTrg[im]->Add(hfgdiff_PhoTrgPt15_20[i]);
        hfgdiff_combPhoTrg[im]->Add(hfgdiff_PhoTrgPt20_30[i]);
        hfgdiff_combPhoTrg[im]->Add(hfgdiff_PhoTrgPt30above[i]);

        //
        if(hbgdiff_PhoTrgPt10_15_raw[i]->Integral())
            hbgdiff_PhoTrgPt10_15_raw[i]->Scale(hbgdiff_PhoTrgPt10_15[i]->Integral()/hbgdiff_PhoTrgPt10_15_raw[i]->Integral());
        if(hbgdiff_PhoTrgPt15_20_raw[i]->Integral())
            hbgdiff_PhoTrgPt15_20_raw[i]->Scale(hbgdiff_PhoTrgPt15_20[i]->Integral()/hbgdiff_PhoTrgPt15_20_raw[i]->Integral());
        if(hbgdiff_PhoTrgPt20_30_raw[i]->Integral())
            hbgdiff_PhoTrgPt20_30_raw[i]->Scale(hbgdiff_PhoTrgPt20_30[i]->Integral()/hbgdiff_PhoTrgPt20_30_raw[i]->Integral());
        if(hbgdiff_PhoTrgPt30above_raw[i]->Integral())
            hbgdiff_PhoTrgPt30above_raw[i]->Scale(hbgdiff_PhoTrgPt30above[i]->Integral()/hbgdiff_PhoTrgPt30above_raw[i]->Integral());

        hbgdiff_raw_combPhoTrg[im]->Add(hbgdiff_PhoTrgPt10_15_raw[i]);
        hbgdiff_raw_combPhoTrg[im]->Add(hbgdiff_PhoTrgPt15_20_raw[i]);
        hbgdiff_raw_combPhoTrg[im]->Add(hbgdiff_PhoTrgPt20_30_raw[i]);
        hbgdiff_raw_combPhoTrg[im]->Add(hbgdiff_PhoTrgPt30above_raw[i]);

        hbgdiff_combPhoTrg[im]->Add(hbgdiff_PhoTrgPt10_15[i]);
        hbgdiff_combPhoTrg[im]->Add(hbgdiff_PhoTrgPt15_20[i]);
        hbgdiff_combPhoTrg[im]->Add(hbgdiff_PhoTrgPt20_30[i]);
        hbgdiff_combPhoTrg[im]->Add(hbgdiff_PhoTrgPt30above[i]);

    }
    cout<<" ....Writing the spectra ...."<<endl;
    fout->cd();
    //.. save 
    for(short i = 0; i<NptRebin; i++) {
        // track trigger 
        hfg_raw_combTrkTrg[i]->Write(); 
        hfg_combTrkTrg[i]->Write();   

        hfgdiff_raw_combTrkTrg[i]->Write();
        hfgdiff_combTrkTrg[i]->Write();

        hbgdiff_raw_combTrkTrg[i]->Write();
        hbgdiff_combTrkTrg[i]->Write();

        // Jet trigger 
        hfg_raw_combJetTrg[i]->Write(); 
        hfg_combJetTrg[i]->Write();   

        hfgdiff_raw_combJetTrg[i]->Write();
        hfgdiff_combJetTrg[i]->Write();

        hbgdiff_raw_combJetTrg[i]->Write();
        hbgdiff_combJetTrg[i]->Write();

        // Photon trigger 
        hfg_raw_combPhoTrg[i]->Write(); 
        hfg_combPhoTrg[i]->Write();   

        hfgdiff_raw_combPhoTrg[i]->Write();
        hfgdiff_combPhoTrg[i]->Write();

        hbgdiff_raw_combPhoTrg[i]->Write();
        hbgdiff_combPhoTrg[i]->Write();
    }
    cout<<" ... done ...."<<endl;
}

//
void ana_hfforest::drawOneTrg(int ich, int whichTrg, int Nrebin, bool chk)
{//..ich: 0=Dstar2D0Pi, 1=D02KPi, 2=Ds2PhiPi, 3=Ds2KstarK, 4=Dpm2KPiPi;
 //.. whichTrg: 0=TrkTrg, 1=JetTrg, 2=PhoTrg
    char plotName[200];
    char outName[200];
    //
    if(!chk) 
        sprintf(outName, "%s_trg%d_comb.root", MesonName[ich], whichTrg);
    else 
        sprintf(outName, "%s_trg%d_check.root", MesonName[ich], whichTrg);

    TFile outFile(outName, "recreate");
    TH1F* hpt = new TH1F("hpt", "", NptRebin, ptRebin);
    hpt->SetMinimum(1);

    //
    TCanvas* cc = new TCanvas("cc", "", 1000, 800);
    int nrow = 4, ncol = 4;
    cc->Divide(nrow, ncol);

    int ic = 0, iplt= 0;
    float sig= 0, err= 0;
    for(int i = 0; i<NptRebin; i++) {
        cc->cd(ic+1);

        if(ich==0) {
            if(whichTrg==0) {
                if(!chk) 
                    drawDstar(hfgdiff_raw_combTrkTrg[i], hbgdiff_raw_combTrkTrg[i], Nrebin, sig, err);
                    //draw_fit(ich, hfgdiff_raw_combTrkTrg[i], Nrebin, mlow[ich], mup[ich], sig, err);
                else
                    drawDstar(hfgdiff_raw_rebin[1][i], hbgdiff_raw_rebin[1][i], Nrebin, sig, err);
                    //draw_fit(ich, hfgdiff_raw_rebin[1][i], Nrebin, mlow[ich], mup[ich], sig, err);
            } else if(whichTrg==1) {
                if(!chk) 
                    drawDstar(hfgdiff_raw_combJetTrg[i], hbgdiff_raw_combJetTrg[i], Nrebin, sig, err);
                    //draw_fit(ich, hfgdiff_raw_combJetTrg[i], Nrebin, mlow[ich], mup[ich], sig, err);
                else
                    drawDstar(hfgdiff_raw_rebin[4][i], hbgdiff_raw_rebin[1][i], Nrebin, sig, err);
                    //draw_fit(ich, hfgdiff_raw_rebin[4][i], Nrebin, mlow[ich], mup[ich], sig, err);
            } else if(whichTrg==2) {
                if(!chk) 
                    drawDstar(hfgdiff_raw_combPhoTrg[i], hbgdiff_combPhoTrg[i], Nrebin, sig, err);
                    //draw_fit(ich, hfgdiff_raw_combPhoTrg[i], Nrebin, mlow[ich], mup[ich], sig, err);
                else
                    drawDstar(hfgdiff_raw_rebin[9][i], hbgdiff_raw_rebin[1][i], Nrebin, sig, err);
                    //draw_fit(ich, hfgdiff_raw_rebin[9][i], Nrebin, mlow[ich], mup[ich], sig, err);
            }
        } else if(ich <NCH) {
            if(whichTrg==0) {
                if(!chk)
                    draw_fit(ich, hfg_raw_combTrkTrg[i], Nrebin, mlow[ich], mup[ich], sig, err);
                else
                    draw_fit(ich, hfg_raw_rebin[1][i], Nrebin, mlow[ich], mup[ich], sig, err);
            } else if(whichTrg==1) {
                if(!chk) 
                    draw_fit(ich, hfg_raw_combJetTrg[i], Nrebin, mlow[ich], mup[ich], sig, err);
                else
                    draw_fit(ich, hfg_raw_rebin[4][i], Nrebin, mlow[ich], mup[ich], sig, err);
            } else if(whichTrg==2) {
                if(!chk) 
                    draw_fit(ich, hfg_raw_combPhoTrg[i], Nrebin, mlow[ich], mup[ich], sig, err);
                else
                    draw_fit(ich, hfg_raw_rebin[9][i], Nrebin, mlow[ich], mup[ich], sig, err);
            }
        } else {
            cout<<" !!! no such D meson  !!!"<<endl;
        }

        if(ic == nrow*ncol-1 || i==NptRebin-1) {
            if(!chk)
                sprintf(plotName, "%s_Trg%d_plot%d_comb.gif", MesonName[ich], whichTrg, iplt);
            else
                sprintf(plotName, "%s_Trg%d_plot%d_check.gif", MesonName[ich], whichTrg, iplt);

            cc->SaveAs(plotName);
            cc->Clear();
            cc->Divide(nrow, ncol);
            iplt++;
            ic = 0;
        } else 
            ic++;

        //
        hpt->SetBinContent(i+1, sig);
        hpt->SetBinError(i+1, err);
    }

    hpt->Write();
    outFile.Close();
}

//
void ana_hfforest::drawDstar(TH1* h_fg, TH1* h_bg, int nrb, float& sig, float& err)
{
    int ib1 = h_fg->FindBin(0.142);
    int ib2 = h_fg->FindBin(0.15);
    int ibmax = h_fg->GetNbinsX();

    float scale = 1;
    float bgInt = h_bg->Integral(1, ib1) + h_bg->Integral(ib2, ibmax);
    if(bgInt)
        scale = (h_fg->Integral(1, ib1) + h_fg->Integral(ib2, ibmax))/bgInt;

    h_bg->Scale(scale);

    h_fg->Rebin(nrb);
    h_bg->Rebin(nrb);

    h_fg->SetLineColor(2);
    h_fg->SetMarkerStyle(20);
    h_fg->SetMarkerColor(2);
    h_fg->SetMarkerSize(0.5);

    h_bg->SetLineColor(1);
    h_bg->SetMarkerStyle(25);
    h_bg->SetMarkerColor(1);
    h_bg->SetMarkerSize(0.5);

    h_fg->Draw();
    h_bg->Draw("same");

    sig = h_fg->Integral(ib1, ib2) - h_bg->Integral(ib1, ib2);
}

//
void ana_hfforest::drawHighMult(int ich, int whichTrg, int Nrebin, TFile* f)
{//..ich: 0=Dstar2D0Pi, 1=D02KPi, 2=Ds2PhiPi, 3=Ds2KstarK, 4=Dpm2KPiPi;
 //.. whichTrg: see trg_name ..
    char plotName[200];

    get_hist(f);

    TCanvas* cc = new TCanvas("cc", "", 1000, 800);
    int nrow = 4, ncol = 4;
    cc->Divide(nrow, ncol);
    int ic = 0, iplt= 0;
    for(int i = 0; i<NPT; i++) {
        cc->cd(ic+1);

        if(ich==0) {
            // normalize the background counts outside the signal region, i.e. mdiff<0.143||>0.149 
            int ib1 = hfgdiff_raw[whichTrg][i]->FindBin(0.143);
            int ib2 = hfgdiff_raw[whichTrg][i]->FindBin(0.149);
            int ibmax = hfgdiff_raw[whichTrg][i]->GetNbinsX();

            float scale = (hfgdiff_raw[whichTrg][i]->Integral(1, ib1) + 
                          hfgdiff_raw[whichTrg][i]->Integral(ib2, ibmax))/
                          (hbgdiff_raw[whichTrg][i]->Integral(1, ib1) + 
                           hbgdiff_raw[whichTrg][i]->Integral(ib2, ibmax));
            hbgdiff_raw[whichTrg][i]->Scale(scale);

            hfgdiff_raw[whichTrg][i]->Rebin(Nrebin);
            hbgdiff_raw[whichTrg][i]->Rebin(Nrebin);
            hfgdiff_raw[whichTrg][i]->SetLineColor(2);
            hbgdiff_raw[whichTrg][i]->SetLineColor(4);
            hfgdiff_raw[whichTrg][i]->Draw();
            hbgdiff_raw[whichTrg][i]->Draw("same");
        } else if(ich < NCH) {
            hfg_raw[whichTrg][i]->Rebin(Nrebin);
            hfg_raw[whichTrg][i]->SetLineColor(2);
            hfg_raw[whichTrg][i]->Draw();
        } else {
            cout<<" !!! no such D meson  !!!"<<endl;
        }

        if(ic == nrow*ncol-1 || i==NPT-1) {
            sprintf(plotName, "%s_Trg%d_plot%d.gif", MesonName[ich], whichTrg, iplt);
            cc->SaveAs(plotName);
            cc->Clear();
            cc->Divide(nrow, ncol);
            iplt++;
            ic = 0;
        } else 
            ic++;
    }
}
//
void ana_hfforest::draw_fit(int ich, TH1* h, int nrb, float rlow, float rhigh, float& sig, float& err)
{
    h->Rebin(nrb);
    h->SetMarkerSize(0.8);
    h->SetLineColor(2);
    h->SetMarkerColor(2);
    h->SetMarkerStyle(20);
    h->GetXaxis()->SetNdivisions(505);

    h->GetXaxis()->SetRangeUser(rlow, rhigh);
    //.. fit with a Gaussian and pol
    TF1* fit_fun = new TF1("fit_fun", "[0]*(1/[2]/sqrt(6.28)*exp(-0.5*pow((x-[1])/[2], 2))) + pol2(3) + [6]*(1/[8]/sqrt(6.28)*exp(-0.5*pow((x-[7])/[8], 2)))", rlow, rhigh);
    float tot = h->Integral();

    float var_mean = 0.01, var_width = 0.01;
    float p0 = tot, p1 = MesonMass[ich], p2 = 0.015;
    if(ich==0) {
        var_mean = 0.005;
        var_width = 5e-4;
        p2 = 1e-3;
    }
    int pass = 1;
    float p6 = tot, p7 = MesonMass[4], p8 = 0.015; //..2nd peak is D+/-
    float zero = 0;
    while(pass) {
        //.. initial value 
        fit_fun->SetParameter(0, p0);
        fit_fun->SetParameter(1, p1);
        fit_fun->SetParameter(2, p2);
        if(ich==2) {//.. 2nd gaussian for Ds->phi+pi
            fit_fun->SetParameter(6, p6);
            fit_fun->SetParameter(7, p7);
            fit_fun->SetParameter(8, p8);
        }

        //.. fit constraint ..
        fit_fun->SetParLimits(0, 0, 1e10);
        fit_fun->SetParLimits(1, TMath::Max(zero, MesonMass[ich]-var_mean), MesonMass[ich]+var_mean);
        fit_fun->SetParLimits(2, TMath::Max(zero, p2-var_width),  p2+var_width);

        if(ich==2) {
            fit_fun->SetParLimits(6, 0, 1e10);
            fit_fun->SetParLimits(7, TMath::Max(zero, MesonMass[4]-var_mean), MesonMass[4]+var_mean);
            fit_fun->SetParLimits(8, TMath::Max(zero, p8-var_width),  p8+var_width);
        } else {//.. all D except Ds->phi+pi have only one peak. remove the 2nd peak
            fit_fun->FixParameter(6, 0);
            fit_fun->FixParameter(7, 0);
            fit_fun->FixParameter(8, 0);
        }

        //.. using likelyhood. Chi2 bias total counts
        for(int ii = 0; ii<10; ii++)
            h->Fit(fit_fun,  "LQ", "", rlow, rhigh);

        //.. draw foreground and background ..
        h->Draw();

        TF1* fit_fun_bg = (TF1*)fit_fun->Clone("fit_fun_bg");
        fit_fun_bg->SetParameter(0, 0);
        fit_fun_bg->SetParameter(1, 0);
        fit_fun_bg->SetParameter(2, 0);
        fit_fun_bg->SetParameter(6, 0);
        fit_fun_bg->SetParameter(7, 0);
        fit_fun_bg->SetParameter(8, 0);

        fit_fun_bg->SetLineColor(8);
        fit_fun_bg->Draw("same");
        gPad->Update();

        //.. check if need to fit again ...
        cout<<" good fit? (0: no refit, 1: refit w/o range change. 2: refit w/ range change"<<endl;
        scanf("%d", &pass);

        if(pass) {
            cout<<" var(mean), var(width) ?"<<endl;
            scanf("%f%f", &var_mean, &var_width);

            if(pass==2) {
                cout<<" rlow, rhigh? "<<endl;
                scanf("%f%f", &rlow, &rhigh);
            }
        }
    }

    //
    sig = fit_fun->GetParameter(0)/h->GetBinWidth(1);
    err = fit_fun->GetParError(0)/h->GetBinWidth(1);

    cout<<"total number of D: "<<sig<<"+/-"<<err<<endl;
}
