#include <fstream>
#include <iostream>

#include "TH1F.h"
#include "TFile.h"
#include "TSystem.h"
#include "TTree.h"
#include "TNtuple.h"
#include "TBranch.h"

#include "../interface/hfcand_v0.hh"
#include "../interface/ana_hfforest.hh"

const char* MesonName[NCH] = {"Dstar2D0Pi", "D02KPi", "Ds2PhiPi", "Ds2KstarK", "Dpm2KPiPi"};

ClassImp(ana_hfforest)

    //
ana_hfforest::ana_hfforest()
{
}

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
    if(y<=Ymin || y>=Ymin+NY*dY) {
        cout<<"!! rapidity: "<<y<<" beyond "<<Ymin<<"--"<<Ymin+NY*dY<<".  exit !!!"<<endl;
        return false;
    }

    return true;
}

//
void ana_hfforest::Init(int startFile, int endFile, char *filelist, int iy, int ich)
{//.. called right after constructor 
    filename = new char[1000];
    float ydn = iy*dY + Ymin; 
    float yup = ydn+dY;
    if(!checkRapidityBoundary(ydn) || !checkRapidityBoundary(yup)) 
        exit(0);

    sprintf(filename, "ana_hhforest_%s_%dTo%d_rapidity_%3.1f_%3.1f_%s.root",  filelist, startFile, endFile, ydn, yup, MesonName[ich]);
    result = new TFile(filename, "recreate");

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
void ana_hfforest::LoopOverFile(int startFile, int endFile, char *filelist, int iy, int ich) 
{//.. loop over files in the "*filelist" from startFile to endFile in sequence
    //
    Init(startFile, endFile, filelist, iy, ich);

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
        }

    for(short i = 0; i<NPT; i++) {
        //FullTrack trigger ...
        hfg_ZBiasSglTrkPt0_12_raw[i]->Write();
        hfg_ZBiasSglTrkPt0_12[i]->Write();

        hfg_TrkTrgPt12_20_raw[i]->Write();
        hfg_TrkTrgPt12_20[i]->Write();
        hfg_TrkTrgPt20_30_raw[i]->Write();
        hfg_TrkTrgPt20_30[i]->Write();
        hfg_TrkTrgPt30above_raw[i]->Write();
        hfg_TrkTrgPt30above[i]->Write();

        // Jet trigger ..
        hfg_ZBiasSglTrkPt0_20_raw[i]->Write();
        hfg_ZBiasSglTrkPt0_20[i]->Write();

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
}

//
void ana_hfforest::LoopOverEvt(TTree* T, int iy, int ich)
{//.. loop over each event. Add different analysis function here. 

    for(int i = 0; i<T->GetEntries(); i++) {
        reset_trg();
        T->GetEntry(i);

        LoopOverHFCandidate(iy, ich);
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

    if(!GetRapidity(mass, fpt, feta, iy) || fq1==fq2) 
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

            FillTrgCombineTrkTrg(0, ich, ipt, iy, mass, mass_dau);
            FillTrgCombineJetTrg(0, ich, ipt, iy, mass, mass_dau);
            FillTrgCombinePhoTrg(0, ich, ipt, iy, mass, mass_dau);

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

                FillTrgCombineTrkTrg(0, ich, ipt, iy, mass, mass_dau);
                FillTrgCombineJetTrg(0, ich, ipt, iy, mass, mass_dau);
                FillTrgCombinePhoTrg(0, ich, ipt, iy, mass, mass_dau);

                if(ich==0) {
                    FillTrgCombineTrkTrg(1, ich, ipt, iy, mass, mass_dau);
                    FillTrgCombineJetTrg(1, ich, ipt, iy, mass, mass_dau);
                    FillTrgCombinePhoTrg(1, ich, ipt, iy, mass, mass_dau);
                }

            } else { //.. background through side band ...
                for(int itrg = 0; itrg<NTRG; itrg++) {
                    if(trg[itrg] && ich==0) {
                        hbgdiff_raw[itrg][ipt]->Fill(mass-mass_dau);
                        hbgdiff[itrg][ipt]->Fill(mass-mass_dau, prscl[itrg]);
                    }
                }

                if(ich==0) {
                    FillTrgCombineTrkTrg(2, ich, ipt, iy, mass, mass_dau);
                    FillTrgCombineJetTrg(2, ich, ipt, iy, mass, mass_dau);
                    FillTrgCombinePhoTrg(2, ich, ipt, iy, mass, mass_dau);
                }
            }
        }
    }
}

//
void ana_hfforest::FillTrgCombineTrkTrg(int id, int ich, int ipt, int iy, float mass, float mass_dau)
{//.. id:  0-->hfg,  1-->hfgdiff, 2-->hbgdiff

    float maxTrgPt = 0;
    if(trg[1]) {
        FindMaxTrgPt(trg_obj[1], maxTrgPt);

        if(maxTrgPt >=12 && maxTrgPt < 20) {
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
        }
    }

    if(trg[2])  {
        FindMaxTrgPt(trg_obj[2], maxTrgPt);

        if(maxTrgPt >=20 && maxTrgPt < 30) {
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
        }
    }

    if(trg[3]) {
        FindMaxTrgPt(trg_obj[3], maxTrgPt);
        if(maxTrgPt >= 30) {
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
}

//
void ana_hfforest::FillTrgCombineJetTrg(int id, int ich, int ipt, int iy, float mass, float mass_dau)
{//.. id:  0-->hfg,  1-->hfgdiff, 2-->hbgdiff

    float maxTrgPt = 0;
    if(trg[4]) {
        FindMaxTrgPt(trg_obj[4], maxTrgPt);

        if(maxTrgPt >=20 && maxTrgPt < 40) {
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
        }
    }

    if(trg[5])  {
        FindMaxTrgPt(trg_obj[5], maxTrgPt);

        if(maxTrgPt >=40 && maxTrgPt < 60) {
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
        }
    }

    if(trg[6])  {
        FindMaxTrgPt(trg_obj[6], maxTrgPt);

        if(maxTrgPt >=60 && maxTrgPt < 80) {
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
        }
    }

    if(trg[7])  {
        FindMaxTrgPt(trg_obj[7], maxTrgPt);

        if(maxTrgPt >=80 && maxTrgPt < 100) {
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
        }
    }

    if(trg[8]) {
        FindMaxTrgPt(trg_obj[8], maxTrgPt);
        if(maxTrgPt >= 100) {
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
}

//
void ana_hfforest::FillTrgCombinePhoTrg(int id, int ich, int ipt, int iy, float mass, float mass_dau)
{//.. id:  0-->hfg,  1-->hfgdiff, 2-->hbgdiff

    float maxTrgPt = 0;
    if(trg[9]) {
        FindMaxTrgPt(trg_obj[9], maxTrgPt);

        if(maxTrgPt >=10 && maxTrgPt < 15) {
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
        }
    }

    if(trg[10])  {
        FindMaxTrgPt(trg_obj[10], maxTrgPt);

        if(maxTrgPt >=15 && maxTrgPt < 20) {
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
        }
    }

    if(trg[11])  {
        FindMaxTrgPt(trg_obj[11], maxTrgPt);

        if(maxTrgPt >=20 && maxTrgPt < 30) {
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
        }
    }

    if(trg[12]) {
        FindMaxTrgPt(trg_obj[12], maxTrgPt);
        if(maxTrgPt >= 30) {
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
}

//
void ana_hfforest::get_hist(TFile* f)
{
    char hname[100];

    // all D mesons
    for(short it = 0; it<NTRG; it++) 
        for(short i = 0; i<NPT; i++) {

            sprintf(hname, "hfg%d_%d", it, i);
            hfg[it][i] = (TH1F*)f->Get(hname)->Clone();
            sprintf(hname, "hfg_raw%d_%d", it, i);
            hfg_raw[it][i] =  (TH1F*)f->Get(hname)->Clone();
        }

    // D*
    for(short it = 0; it<NTRG; it++) 
        for(short i = 0; i<NPT; i++) {
            sprintf(hname, "hfgdiff%d_%d", it, i);
            hfgdiff[it][i] = (TH1F*)f->Get(hname)->Clone();
            sprintf(hname, "hfgdiff_raw%d_%d", it, i);
            hfgdiff_raw[it][i] = (TH1F*)f->Get(hname)->Clone();

            sprintf(hname, "hbgdiff%d_%d", it, i);
            hbgdiff[it][i] = (TH1F*)f->Get(hname)->Clone();
            sprintf(hname, "hbgdiff_raw%d_%d", it, i);
            hbgdiff_raw[it][i] = (TH1F*)f->Get(hname)->Clone();
        }

    // components for trigger combination for all D..
    for(short i = 0; i<NPT; i++) {

        sprintf(hname, "hfg_ZBiasSglTrkPt0_12_raw%d", i);
        hfg_ZBiasSglTrkPt0_12_raw[i] = (TH1F*)f->Get(hname)->Clone();
        sprintf(hname, "hfg_ZBiasSglTrkPt0_12%d", i);
        hfg_ZBiasSglTrkPt0_12[i] = (TH1F*)f->Get(hname)->Clone();

        sprintf(hname, "hfg_TrkTrgPt12_20_raw%d", i);
        hfg_TrkTrgPt12_20_raw[i] = (TH1F*)f->Get(hname)->Clone();
        sprintf(hname, "hfg_TrkTrgPt12_20%d", i);
        hfg_TrkTrgPt12_20[i] = (TH1F*)f->Get(hname)->Clone();

        sprintf(hname, "hfg_TrkTrgPt20_30_raw%d", i);
        hfg_TrkTrgPt20_30_raw[i] = (TH1F*)f->Get(hname)->Clone();
        sprintf(hname, "hfg_TrkTrgPt20_30%d", i);
        hfg_TrkTrgPt20_30[i] = (TH1F*)f->Get(hname)->Clone();

        sprintf(hname, "hfg_TrkTrgPt30above_raw%d", i);
        hfg_TrkTrgPt30above_raw[i] = (TH1F*)f->Get(hname)->Clone();
        sprintf(hname, "hfg_TrkTrgPt30above%d", i);
        hfg_TrkTrgPt30above[i] = (TH1F*)f->Get(hname)->Clone();

        //Jet trigger 
        sprintf(hname, "hfg_ZBiasSglTrkPt0_20_raw%d", i);
        hfg_ZBiasSglTrkPt0_20_raw[i] = (TH1F*)f->Get(hname)->Clone();
        sprintf(hname, "hfg_ZBiasSglTrkPt0_20%d", i);
        hfg_ZBiasSglTrkPt0_20[i] = (TH1F*)f->Get(hname)->Clone();

        sprintf(hname, "hfg_JetTrgPt20_40_raw%d", i);
        hfg_JetTrgPt20_40_raw[i] = (TH1F*)f->Get(hname)->Clone();
        sprintf(hname, "hfg_JetTrgPt20_40%d", i);
        hfg_JetTrgPt20_40[i] = (TH1F*)f->Get(hname)->Clone();

        sprintf(hname, "hfg_JetTrgPt40_60_raw%d", i);
        hfg_JetTrgPt40_60_raw[i] = (TH1F*)f->Get(hname)->Clone();
        sprintf(hname, "hfg_JetTrgPt40_60%d", i);
        hfg_JetTrgPt40_60[i] = (TH1F*)f->Get(hname)->Clone();

        sprintf(hname, "hfg_JetTrgPt60_80_raw%d", i);
        hfg_JetTrgPt60_80_raw[i] = (TH1F*)f->Get(hname)->Clone();
        sprintf(hname, "hfg_JetTrgPt60_80%d", i);
        hfg_JetTrgPt60_80[i] = (TH1F*)f->Get(hname)->Clone();

        sprintf(hname, "hfg_JetTrgPt80_100_raw%d", i);
        hfg_JetTrgPt80_100_raw[i] = (TH1F*)f->Get(hname)->Clone();
        sprintf(hname, "hfg_JetTrgPt80_100%d", i);
        hfg_JetTrgPt80_100[i] = (TH1F*)f->Get(hname)->Clone();

        sprintf(hname, "hfg_JetTrgPt100above_raw%d", i);
        hfg_JetTrgPt100above_raw[i] = (TH1F*)f->Get(hname)->Clone();
        sprintf(hname, "hfg_JetTrgPt100above%d", i);
        hfg_JetTrgPt100above[i] = (TH1F*)f->Get(hname)->Clone();

        //Photon trigger 
        sprintf(hname, "hfg_ZBiasSglTrkPt0_10_raw%d", i);
        hfg_ZBiasSglTrkPt0_10_raw[i] = (TH1F*)f->Get(hname)->Clone();
        sprintf(hname, "hfg_ZBiasSglTrkPt0_10%d", i);
        hfg_ZBiasSglTrkPt0_10[i] = (TH1F*)f->Get(hname)->Clone();

        sprintf(hname, "hfg_PhoTrgPt10_15_raw%d", i);
        hfg_PhoTrgPt10_15_raw[i] = (TH1F*)f->Get(hname)->Clone();
        sprintf(hname, "hfg_PhoTrgPt10_15%d", i);
        hfg_PhoTrgPt10_15[i] = (TH1F*)f->Get(hname)->Clone();

        sprintf(hname, "hfg_PhoTrgPt15_20_raw%d", i);
        hfg_PhoTrgPt15_20_raw[i] = (TH1F*)f->Get(hname)->Clone();
        sprintf(hname, "hfg_PhoTrgPt15_20%d", i);
        hfg_PhoTrgPt15_20[i] = (TH1F*)f->Get(hname)->Clone();

        sprintf(hname, "hfg_PhoTrgPt20_30_raw%d", i);
        hfg_PhoTrgPt20_30_raw[i] = (TH1F*)f->Get(hname)->Clone();
        sprintf(hname, "hfg_PhoTrgPt20_30%d", i);
        hfg_PhoTrgPt20_30[i] = (TH1F*)f->Get(hname)->Clone();

        sprintf(hname, "hfg_PhoTrgPt30above_raw%d", i);
        hfg_PhoTrgPt30above_raw[i] = (TH1F*)f->Get(hname)->Clone();
        sprintf(hname, "hfg_PhoTrgPt30above%d", i);
        hfg_PhoTrgPt30above[i] = (TH1F*)f->Get(hname)->Clone();
    }

    // components for trigger combination for D*
    for(short i = 0; i<NPT; i++) {

        // FullTrack trigger
        sprintf(hname, "hfgdiff_ZBiasSglTrkPt0_12_raw%d", i);
        hfgdiff_ZBiasSglTrkPt0_12_raw[i] = (TH1F*)f->Get(hname)->Clone();
        sprintf(hname, "hfgdiff_ZBiasSglTrkPt0_12%d", i);
        hfgdiff_ZBiasSglTrkPt0_12[i] = (TH1F*)f->Get(hname)->Clone();

        sprintf(hname, "hfgdiff_TrkTrgPt12_20_raw%d", i);
        hfgdiff_TrkTrgPt12_20_raw[i] = (TH1F*)f->Get(hname)->Clone();
        sprintf(hname, "hfgdiff_TrkTrgPt12_20%d", i);
        hfgdiff_TrkTrgPt12_20[i] = (TH1F*)f->Get(hname)->Clone();

        sprintf(hname, "hfgdiff_TrkTrgPt20_30_raw%d", i);
        hfgdiff_TrkTrgPt20_30_raw[i] = (TH1F*)f->Get(hname)->Clone();
        sprintf(hname, "hfgdiff_TrkTrgPt20_30%d", i);
        hfgdiff_TrkTrgPt20_30[i] = (TH1F*)f->Get(hname)->Clone();

        sprintf(hname, "hfgdiff_TrkTrgPt30above_raw%d", i);
        hfgdiff_TrkTrgPt30above_raw[i] = (TH1F*)f->Get(hname)->Clone();
        sprintf(hname, "hfgdiff_TrkTrgPt30above%d", i);
        hfgdiff_TrkTrgPt30above[i] = (TH1F*)f->Get(hname)->Clone();


        sprintf(hname, "hbgdiff_ZBiasSglTrkPt0_12_raw%d", i);
        hbgdiff_ZBiasSglTrkPt0_12_raw[i] = (TH1F*)f->Get(hname)->Clone();
        sprintf(hname, "hbgdiff_ZBiasSglTrkPt0_12%d", i);
        hbgdiff_ZBiasSglTrkPt0_12[i] = (TH1F*)f->Get(hname)->Clone();

        sprintf(hname, "hbgdiff_TrkTrgPt12_20_raw%d", i);
        hbgdiff_TrkTrgPt12_20_raw[i] = (TH1F*)f->Get(hname)->Clone();
        sprintf(hname, "hbgdiff_TrkTrgPt12_20%d", i);
        hbgdiff_TrkTrgPt12_20[i] = (TH1F*)f->Get(hname)->Clone();

        sprintf(hname, "hbgdiff_TrkTrgPt20_30_raw%d", i);
        hbgdiff_TrkTrgPt20_30_raw[i] = (TH1F*)f->Get(hname)->Clone();
        sprintf(hname, "hbgdiff_TrkTrgPt20_30%d", i);
        hbgdiff_TrkTrgPt20_30[i] = (TH1F*)f->Get(hname)->Clone();

        sprintf(hname, "hbgdiff_TrkTrgPt30above_raw%d", i);
        hbgdiff_TrkTrgPt30above_raw[i] = (TH1F*)f->Get(hname)->Clone();
        sprintf(hname, "hbgdiff_TrkTrgPt30above%d", i);
        hbgdiff_TrkTrgPt30above[i] = (TH1F*)f->Get(hname)->Clone();

        // Jet trigger
        sprintf(hname, "hfgdiff_ZBiasSglTrkPt0_20_raw%d", i);
        hfgdiff_ZBiasSglTrkPt0_20_raw[i] = (TH1F*)f->Get(hname)->Clone();
        sprintf(hname, "hfgdiff_ZBiasSglTrkPt0_20%d", i);
        hfgdiff_ZBiasSglTrkPt0_20[i] = (TH1F*)f->Get(hname)->Clone();

        sprintf(hname, "hfgdiff_JetTrgPt20_40_raw%d", i);
        hfgdiff_JetTrgPt20_40_raw[i] = (TH1F*)f->Get(hname)->Clone();
        sprintf(hname, "hfgdiff_JetTrgPt20_40%d", i);
        hfgdiff_JetTrgPt20_40[i] = (TH1F*)f->Get(hname)->Clone();

        sprintf(hname, "hfgdiff_JetTrgPt40_60_raw%d", i);
        hfgdiff_JetTrgPt40_60_raw[i] = (TH1F*)f->Get(hname)->Clone();
        sprintf(hname, "hfgdiff_JetTrgPt40_60%d", i);
        hfgdiff_JetTrgPt40_60[i] = (TH1F*)f->Get(hname)->Clone();

        sprintf(hname, "hfgdiff_JetTrgPt60_80_raw%d", i);
        hfgdiff_JetTrgPt60_80_raw[i] = (TH1F*)f->Get(hname)->Clone();
        sprintf(hname, "hfgdiff_JetTrgPt60_80%d", i);
        hfgdiff_JetTrgPt60_80[i] = (TH1F*)f->Get(hname)->Clone();

        sprintf(hname, "hfgdiff_JetTrgPt80_100_raw%d", i);
        hfgdiff_JetTrgPt80_100_raw[i] = (TH1F*)f->Get(hname)->Clone();
        sprintf(hname, "hfgdiff_JetTrgPt80_100%d", i);
        hfgdiff_JetTrgPt80_100[i] = (TH1F*)f->Get(hname)->Clone();

        sprintf(hname, "hfgdiff_JetTrgPt100above_raw%d", i);
        hfgdiff_JetTrgPt100above_raw[i] = (TH1F*)f->Get(hname)->Clone();
        sprintf(hname, "hfgdiff_JetTrgPt100above%d", i);
        hfgdiff_JetTrgPt100above[i] = (TH1F*)f->Get(hname)->Clone();


        sprintf(hname, "hbgdiff_ZBiasSglTrkPt0_20_raw%d", i);
        hbgdiff_ZBiasSglTrkPt0_20_raw[i] = (TH1F*)f->Get(hname)->Clone();
        sprintf(hname, "hbgdiff_ZBiasSglTrkPt0_20%d", i);
        hbgdiff_ZBiasSglTrkPt0_20[i] = (TH1F*)f->Get(hname)->Clone();

        sprintf(hname, "hbgdiff_JetTrgPt20_40_raw%d", i);
        hbgdiff_JetTrgPt20_40_raw[i] = (TH1F*)f->Get(hname)->Clone();
        sprintf(hname, "hbgdiff_JetTrgPt20_40%d", i);
        hbgdiff_JetTrgPt20_40[i] = (TH1F*)f->Get(hname)->Clone();

        sprintf(hname, "hbgdiff_JetTrgPt40_60_raw%d", i);
        hbgdiff_JetTrgPt40_60_raw[i] = (TH1F*)f->Get(hname)->Clone();
        sprintf(hname, "hbgdiff_JetTrgPt40_60%d", i);
        hbgdiff_JetTrgPt40_60[i] = (TH1F*)f->Get(hname)->Clone();

        sprintf(hname, "hbgdiff_JetTrgPt60_80_raw%d", i);
        hbgdiff_JetTrgPt60_80_raw[i] = (TH1F*)f->Get(hname)->Clone();
        sprintf(hname, "hbgdiff_JetTrgPt60_80%d", i);
        hbgdiff_JetTrgPt60_80[i] = (TH1F*)f->Get(hname)->Clone();

        sprintf(hname, "hbgdiff_JetTrgPt80_100_raw%d", i);
        hbgdiff_JetTrgPt80_100_raw[i] = (TH1F*)f->Get(hname)->Clone();
        sprintf(hname, "hbgdiff_JetTrgPt80_100%d", i);
        hbgdiff_JetTrgPt80_100[i] = (TH1F*)f->Get(hname)->Clone();

        sprintf(hname, "hbgdiff_JetTrgPt100above_raw%d", i);
        hbgdiff_JetTrgPt100above_raw[i] = (TH1F*)f->Get(hname)->Clone();
        sprintf(hname, "hbgdiff_JetTrgPt100above%d", i);
        hbgdiff_JetTrgPt100above[i] = (TH1F*)f->Get(hname)->Clone();

        // Photon trigger
        sprintf(hname, "hfgdiff_ZBiasSglTrkPt0_10_raw%d", i);
        hfgdiff_ZBiasSglTrkPt0_10_raw[i] = (TH1F*)f->Get(hname)->Clone();
        sprintf(hname, "hfgdiff_ZBiasSglTrkPt0_10%d", i);
        hfgdiff_ZBiasSglTrkPt0_10[i] = (TH1F*)f->Get(hname)->Clone();

        sprintf(hname, "hfgdiff_PhoTrgPt10_15_raw%d", i);
        hfgdiff_PhoTrgPt10_15_raw[i] = (TH1F*)f->Get(hname)->Clone();
        sprintf(hname, "hfgdiff_PhoTrgPt10_15%d", i);
        hfgdiff_PhoTrgPt10_15[i] = (TH1F*)f->Get(hname)->Clone();

        sprintf(hname, "hfgdiff_PhoTrgPt15_20_raw%d", i);
        hfgdiff_PhoTrgPt15_20_raw[i] = (TH1F*)f->Get(hname)->Clone();
        sprintf(hname, "hfgdiff_PhoTrgPt15_20%d", i);
        hfgdiff_PhoTrgPt15_20[i] = (TH1F*)f->Get(hname)->Clone();

        sprintf(hname, "hfgdiff_PhoTrgPt20_30_raw%d", i);
        hfgdiff_PhoTrgPt20_30_raw[i] = (TH1F*)f->Get(hname)->Clone();
        sprintf(hname, "hfgdiff_PhoTrgPt20_30%d", i);
        hfgdiff_PhoTrgPt20_30[i] = (TH1F*)f->Get(hname)->Clone();

        sprintf(hname, "hfgdiff_PhoTrgPt30above_raw%d", i);
        hfgdiff_PhoTrgPt30above_raw[i] = (TH1F*)f->Get(hname)->Clone();
        sprintf(hname, "hfgdiff_PhoTrgPt30above%d", i);
        hfgdiff_PhoTrgPt30above[i] = (TH1F*)f->Get(hname)->Clone();

        sprintf(hname, "hbgdiff_ZBiasSglTrkPt0_10_raw%d", i);
        hbgdiff_ZBiasSglTrkPt0_10_raw[i] = (TH1F*)f->Get(hname)->Clone();
        sprintf(hname, "hbgdiff_ZBiasSglTrkPt0_10%d", i);
        hbgdiff_ZBiasSglTrkPt0_10[i] = (TH1F*)f->Get(hname)->Clone();

        sprintf(hname, "hbgdiff_PhoTrgPt10_15_raw%d", i);
        hbgdiff_PhoTrgPt10_15_raw[i] = (TH1F*)f->Get(hname)->Clone();
        sprintf(hname, "hbgdiff_PhoTrgPt10_15%d", i);
        hbgdiff_PhoTrgPt10_15[i] = (TH1F*)f->Get(hname)->Clone();

        sprintf(hname, "hbgdiff_PhoTrgPt15_20_raw%d", i);
        hbgdiff_PhoTrgPt15_20_raw[i] = (TH1F*)f->Get(hname)->Clone();
        sprintf(hname, "hbgdiff_PhoTrgPt15_20%d", i);
        hbgdiff_PhoTrgPt15_20[i] = (TH1F*)f->Get(hname)->Clone();

        sprintf(hname, "hbgdiff_PhoTrgPt20_30_raw%d", i);
        hbgdiff_PhoTrgPt20_30_raw[i] = (TH1F*)f->Get(hname)->Clone();
        sprintf(hname, "hbgdiff_PhoTrgPt20_30%d", i);
        hbgdiff_PhoTrgPt20_30[i] = (TH1F*)f->Get(hname)->Clone();

        sprintf(hname, "hbgdiff_PhoTrgPt30above_raw%d", i);
        hbgdiff_PhoTrgPt30above_raw[i] = (TH1F*)f->Get(hname)->Clone();
        sprintf(hname, "hbgdiff_PhoTrgPt30above%d", i);
        hbgdiff_PhoTrgPt30above[i] = (TH1F*)f->Get(hname)->Clone();
    }
}
