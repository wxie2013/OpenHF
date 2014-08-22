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

ana_hfforest::ana_hfforest()
{
    run_ppTrack = false;
}

//
ana_hfforest::~ana_hfforest()
{
    delete filename;
    delete result;
    delete hfcandidate;

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
    }

    delete ntMaxTrgPt;
    delete ntMaxTrgPtAlgo2;
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

    ntMaxTrgPt = new TNtuple("ntMaxTrgPt", "maximum trigger pT spectra for debugging", "ptmax:ptmax_org:itrg:weight");
    ntMaxTrgPtAlgo2 = new TNtuple("ntMaxTrgPtAlgo2", "maximum trigger pT spectra for debugging for algo#2", "ptmax:hp:tDz1:tDz1Err:tDxy1:tDxyErr:tPtErr:itrg:weight");
    ntMass = new TNtuple("ntMass", "mass spectra", "mass:ptmax:ipt:itrg:isbg:weight:hp0:hp1:hp2:pt0:pt1:pt2:ptErr0:ptErr1:ptErr2:hp:tPtErr");
    ntTrk = new TNtuple("ntTrk", "trk spectra", "pt:ptErr:ptmax:ptmaxErr:hp0:hp:itrg:weight");

    //
    hfcandidate = new hfcand_v0;

    //HiTree = 0;
    HltTree = 0;
    hltTreeReRun = 0;
    hftree = 0;
    trackTree = 0;

    //.. trigger object ...
    for(int i = 0; i<NTRG; i++) {
        trg_obj[i] = new trigO;
        org_trg_obj[i] = new trigO;
    }

}

//
void ana_hfforest::book_hist(int ich)
{
    float pt_low = 0, pt_high = 0;
    char hname[100], pt_range[1000];

    if(ich!=0) {
        // all D mesons
        for(short it = 0; it<NTRG; it++) {
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
        }
        // components for trigger combination for all D..
        for(short i = 0; i<NPT; i++) {

            get_pt_range(i, pt_low, pt_high);

            //FullTrack trigger 
            sprintf(pt_range, "ch: %s, pt: %f %f", MesonName[ich], pt_low, pt_high);

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
        }
    } else {
        // D*
        for(short it = 0; it<NTRG; it++) {
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

        }
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
    trackTree  = (TTree*)f->Get("ppTrack/trackTree");

    //if(!hftree || !HltTree || !HiTree) {
    if(!hftree || !HltTree || !hltTreeReRun || !trackTree) {
        cout<<"==> missing hftree or HltTree or hltTreeReRun or trackTree <=="<<endl;
        return false;
    }

    HltTree->AddFriend(hftree);
    HltTree->AddFriend(hltTreeReRun);
    HltTree->AddFriend(trackTree);
    //HltTree->AddFriend(HiTree);

    hftree->SetBranchAddress("hfcandidate", &hfcandidate);
    trackTree->SetBranchAddress("nTrk", &nTrk);
    trackTree->SetBranchAddress("trkCharge", trkCharge);
    trackTree->SetBranchAddress("trkPhi", trkPhi);
    trackTree->SetBranchAddress("trkEta", trkEta);
    trackTree->SetBranchAddress("highPurity", highPurity);
    trackTree->SetBranchAddress("trkPt", trkPt);
    trackTree->SetBranchAddress("trkPtError", trkPtError);
    trackTree->SetBranchAddress("trkDz1", trkDz1);
    trackTree->SetBranchAddress("trkDzError1", trkDzError1);
    trackTree->SetBranchAddress("trkDxy1", trkDxy1);
    trackTree->SetBranchAddress("trkDxyError1", trkDxyError1);


    get_trg_info(HltTree, hltTreeReRun);

    return true;
}

//
void ana_hfforest::write()
{//.. results 
    result->cd();

    ntMaxTrgPt->Write();
    ntMaxTrgPtAlgo2->Write();
    ntMass->Write();
    if(run_ppTrack)
        ntTrk->Write();

}
//
void ana_hfforest::write_hist(int ich)
{
    for(int itrg = 0; itrg<NTRG; itrg++) {
        for(short i = 0; i<NPT; i++) {
            if(ich!=0) {
                hfg_raw[itrg][i]->Write();
                hfg[itrg][i]->Write();
            } else {
                hfgdiff_raw[itrg][i]->Write();
                hfgdiff[itrg][i]->Write();
                hbgdiff_raw[itrg][i]->Write();
                hbgdiff[itrg][i]->Write();
            }
        }
    }

    for(short i = 0; i<NPT; i++) {
        if(ich!=0) {
            hfg_ZBiasSglTrkPt0_12_raw[i]->Write();
            hfg_ZBiasSglTrkPt0_12[i]->Write();
            hfg_TrkTrgPt12_20_raw[i]->Write();
            hfg_TrkTrgPt12_20[i]->Write();
            hfg_TrkTrgPt20_30_raw[i]->Write();
            hfg_TrkTrgPt20_30[i]->Write();
            hfg_TrkTrgPt30above_raw[i]->Write();
            hfg_TrkTrgPt30above[i]->Write();
        } else {
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
        }
    }
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
        const char*  branch_nameT1 = branchT1->GetName();
        for(int it = 0; it<NTRG; it++) {
            if (strstr(branch_nameT1, trg_name[it])){
                if(!strstr(branch_nameT1, "Prescl") && 
                        !strstr(branch_nameT1, "trigObject")) {//.. use original trigger decision
                    T1->SetBranchAddress(branch_nameT1, &trg[it]);
                    tid = it;
                } else  if(strstr(branch_nameT1, "Prescl")) {//.. use original prescale
                    T1->SetBranchAddress(branch_nameT1, &prscl[it]);
                } else  if(strstr(branch_nameT1, "trigObject")) {
                    T1->SetBranchAddress(branch_nameT1, &org_trg_obj[it]);
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
                const char* branch_nameT2 = branchT2->GetName();
                if (strstr(branch_nameT2, trg_name[tid]) && strstr(branch_nameT2, "trigObject")){
                    T2->SetBranchAddress(branch_nameT2, &trg_obj[tid]);
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
        org_trg_obj[i]->clear();
    }

    maxTrkTrgPt = 0;

    nTrk = 0;
    is_hp = false;
    tDz1=-9999;
    tDz1Err=-9999;
    tDxy1=-9999;
    tDxyErr=-9999;
    tPtErr=-9999;
}

//
void ana_hfforest::LoopOverEvt(TTree* T, int iy, int ich)
{//.. loop over each event. Add different analysis function here. 

    for(int i = 0; i<T->GetEntries(); i++) {
        reset_trg();
        T->GetEntry(i);

        GetMaxTrgPt();
        FillTrgMaxPt();
        LoopOverHFCandidate(iy, ich);

        if(run_ppTrack) 
            LoopOverTrk();
    }
}

//
void ana_hfforest::GetMaxTrgPt()
{
    for(int itrk = 0; itrk<nTrk; itrk++) {
        //.. the cuts are from Eric 
        //if(highPurity[itrk] && fabs(trkDz1[itrk]/trkDzError1[itrk])<3 &&
        //   fabs(trkDxy1[itrk]/trkDxyError1[itrk])<3 && trkPtError[itrk]/trkPt[itrk]<0.1) 
        //    maxTrkTrgPt = TMath::Max(maxTrkTrgPt, trkPt[itrk]);
        if(trkPt[itrk] > maxTrkTrgPt) {
            maxTrkTrgPt = trkPt[itrk];
            tPtErr = trkPtError[itrk];
            tDz1 = trkDz1[itrk];
            tDz1Err = trkDzError1[itrk];
            tDxy1 = trkDxy1[itrk];
            tDxyErr = trkDxyError1[itrk];
            is_hp = highPurity[itrk];
        }
    }

    //
    float weight = 0;
    for(int it = 0; it<=NTRG; it++) {
        if(trg[it]) {
            if(it==0) 
                weight = prscl[0]*prscl[4];
            else 
                weight = prscl[it];

            ntMaxTrgPtAlgo2->Fill(maxTrkTrgPt, is_hp, tDz1, tDz1Err, tDxy1, tDxyErr, tPtErr, it, weight);
        }
    }

    int it = -1;
    if(trg[0] && maxTrkTrgPt <TrkTrgOffThres[1]) {
        weight = prscl[0]*prscl[4]; //.. include L1 base
        it = 100;
    } else if(trg[1] && maxTrkTrgPt >=TrkTrgOffThres[1] && maxTrkTrgPt < TrkTrgOffThres[2]) {
        weight = prscl[1];
        it = 101;
    } else if(trg[2] && maxTrkTrgPt >=TrkTrgOffThres[2] && maxTrkTrgPt < TrkTrgOffThres[3]) {
        weight = prscl[2];
        it = 102;
    } else if(trg[3] && maxTrkTrgPt >= TrkTrgOffThres[3]) {
        weight = prscl[3];
        it = 103;
    }
    if(it!=-1)
        ntMaxTrgPtAlgo2->Fill(maxTrkTrgPt, is_hp, tDz1, tDz1Err, tDxy1, tDxyErr, tPtErr, it, weight);
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
    float fq2 = cand->get_fpt2(); //.. fpt2 is filled by fq2
    float feta1 = cand->get_feta1();
    float feta2 = cand->get_feta2();
    float fpt1 = cand->get_fpt1();
    float fpt2 = cand->get_fq2();
    float fphi1 = cand->get_fphi1();
    float fphi2 = cand->get_fphi2();
    float fq0 = cand->get_fqdau2();
    float fpt0 = cand->get_fptdau2();
    float feta0 = cand->get_fetadau2();
    float fphi0 = cand->get_fphidau2();

    if(!GetRapidity(mass, fpt, feta, iy) || fq1==fq2 || fabs(feta1)>etaMax || fabs(feta2)>etaMax) 
        return;
    //
    float ffls3d = cand->get_ffls3d();
    float alpha = cand->get_falpha0();
    float fprob = cand->get_fprob();
    float fdr = cand->get_fdr();
    float fchi2 = cand->get_fchi2();
    float mass_dau = cand->get_fmdau1();

    // compare with ppTrack and get high purity bit and trkPtError for algo#2
    tPt0 = fpt0;
    tPt1 = fpt1;
    tPt2 = fpt2;
    is_hp_0 =false;
    is_hp_1 =false;
    is_hp_2 =false;
    tPtErr0 = -9999;
    tPtErr1 = -9999;
    tPtErr2 = -9999;

    if(ich==1) {//D0 does not have 3rd particle
        is_hp_0 = true;
        tPtErr0=0;
    } else {
        find_hp_trkPtErr(fq0, fpt0, feta0, fphi0, is_hp_0, tPtErr0);
    }
    find_hp_trkPtErr(fq1, fpt1, feta1, fphi1, is_hp_1, tPtErr1);
    find_hp_trkPtErr(fq2, fpt2, feta2, fphi2, is_hp_2, tPtErr2);
    //
    int ipt = get_pt_bin_num(fpt);

    float weight = 0;
    if(mass > cut_m_low[ich] && mass < cut_m_high[ich] && 
            ffls3d > cut_ffls3d[ich] && alpha < cut_falpha0[ich] && 
            fprob > cut_fprob[ich] && fdr < cut_fdr[ich] && fchi2 < cut_fchi2[ich]) {

        //.. forground ..
        if(ich==1) { //D0 has no daughter mass, treat separately. ...
            for(int itrg = 0; itrg<NTRG; itrg++) {
                if(trg[itrg]) {
                    if(itrg==0) 
                        weight = prscl[0]*prscl[4];
                    else 
                        weight = prscl[itrg];

                    fill_ntMass(mass, maxTrkTrgPt, ipt, itrg, false, weight);
                }
            }

            FillTrgCombineTrkTrg_HIN12017(0, ich, ipt, mass, mass_dau);

        } else {
            if(mass_dau > cut_m_dau_low[ich] && mass_dau < cut_m_dau_high[ich]) {
                for(int itrg = 0; itrg<NTRG; itrg++) {
                    if(trg[itrg]) {
                        if(itrg==0) 
                            weight = prscl[0]*prscl[4];
                        else 
                            weight = prscl[itrg];

                        if(ich==0)
                            fill_ntMass(mass-mass_dau, maxTrkTrgPt, ipt, itrg, false, weight);
                        else 
                            fill_ntMass(mass, maxTrkTrgPt, ipt, itrg, false, weight);
                    }
                }

                //
                if(ich==0) {
                    FillTrgCombineTrkTrg_HIN12017(1, ich, ipt, mass, mass_dau);
                } else {
                    FillTrgCombineTrkTrg_HIN12017(0, ich, ipt, mass, mass_dau);
                }
            } else { //.. background through side band ...
                if(ich==0) {// for D* only
                    for(int itrg = 0; itrg<NTRG; itrg++) {
                        if(trg[itrg]) {
                            if(itrg==0) 
                                weight = prscl[0]*prscl[4];
                            else 
                                weight = prscl[itrg];

                            fill_ntMass(mass-mass_dau, maxTrkTrgPt, ipt, itrg, true, weight);
                        }
                    }

                    FillTrgCombineTrkTrg_HIN12017(2, ich, ipt, mass, mass_dau);
                }
            }
        }
    }
}

//
void ana_hfforest::FillTrgCombineTrkTrg_HIN12017(int id, int ich, int ipt, float mass, float mass_dau)
{//.. id:  0-->hfg,  1-->hfgdiff, 2-->hbgdiff

    float weight = 0;
    if(trg[0] && maxTrkTrgPt <TrkTrgOffThres[1]) {

        weight = prscl[0]*prscl[4];

        if(id==0) {
            fill_ntMass(mass, maxTrkTrgPt, ipt, 100, false, weight);
        } else if(id==1) {
            fill_ntMass(mass-mass_dau, maxTrkTrgPt, ipt, 100, false, weight);
        } else if(id==2) {
            fill_ntMass(mass-mass_dau, maxTrkTrgPt, ipt, 100, true, weight);
        }

    } else if(trg[1] && maxTrkTrgPt >=TrkTrgOffThres[1] && maxTrkTrgPt < TrkTrgOffThres[2]) {

        weight = prscl[1];

        if(id==0) {
            fill_ntMass(mass, maxTrkTrgPt, ipt, 101, false, weight);
        } else if(id==1) {
            fill_ntMass(mass-mass_dau, maxTrkTrgPt, ipt, 101, false, weight);
        } else if(id==2) {
            fill_ntMass(mass-mass_dau, maxTrkTrgPt, ipt, 101, true, weight);
        }


    } else if(trg[2] && maxTrkTrgPt >=TrkTrgOffThres[2] && maxTrkTrgPt < TrkTrgOffThres[3]) {

        weight = prscl[2];

        if(id==0) {
            fill_ntMass(mass, maxTrkTrgPt, ipt, 102, false, weight);
        } else if(id==1) {
            fill_ntMass(mass-mass_dau, maxTrkTrgPt, ipt, 102, false, weight);
        } else if(id==2) {
            fill_ntMass(mass-mass_dau, maxTrkTrgPt, ipt, 102, true, weight);
        }
    } else if(trg[3] && maxTrkTrgPt >= TrkTrgOffThres[3]) {

        weight = prscl[3];

        if(id==0) {
            fill_ntMass(mass, maxTrkTrgPt, ipt, 103, false, weight);
        } else if(id==1) {
            fill_ntMass(mass-mass_dau, maxTrkTrgPt, ipt, 103, false, weight);
        } else if(id==2) {
            fill_ntMass(mass-mass_dau, maxTrkTrgPt, ipt, 103, true, weight);
        }
    }
}

//
void ana_hfforest::get_comb_hist(int ich, TFile* f)
{
    if(!f) {
        cout<<" !!! file does not exist,  exit"<<endl;
        exit(0);
    }

    cout<<" .... fetching combination file  ..."<<endl;
    char hname[100];
    for(short i = 0; i<NptRebin; i++) {
        if(ich!=0) {
            sprintf(hname, "hfg_raw_combTrkTrg%d", i);
            hfg_raw_combTrkTrg[i] = (TH1F*)f->Get(hname);
            sprintf(hname, "hfg_combTrkTrg%d", i);
            hfg_combTrkTrg[i] = (TH1F*)f->Get(hname);
        } else {
            sprintf(hname, "hfgdiff_raw_combTrkTrg%d", i);
            hfgdiff_raw_combTrkTrg[i] = (TH1F*)f->Get(hname);
            sprintf(hname, "hfgdiff_combTrkTrg%d", i);
            hfgdiff_combTrkTrg[i] = (TH1F*)f->Get(hname);
            sprintf(hname, "hbgdiff_raw_combTrkTrg%d", i);
            hbgdiff_raw_combTrkTrg[i] = (TH1F*)f->Get(hname);
            sprintf(hname, "hbgdiff_combTrkTrg%d", i);
            hbgdiff_combTrkTrg[i] = (TH1F*)f->Get(hname);
        }
    }
    // reference hist for checking trigger combination ...
    for(short i = 0; i<NptRebin; i++) {
        for(short it = 0; it<NTRG; it++) {
            if(ich!=0) {
                sprintf(hname, "hfg_rebin%d_%d", it, i);
                hfg_rebin[it][i] = (TH1F*)f->Get(hname);
            } else {
                sprintf(hname, "hfgdiff_rebin%d_%d", it, i);
                hfgdiff_rebin[it][i] = (TH1F*)f->Get(hname);

                sprintf(hname, "hbgdiff_rebin%d_%d", it, i);
                hbgdiff_rebin[it][i] = (TH1F*)f->Get(hname);
            }
        }

        //
        sprintf(hname, "hfg_TrkTrgPt12_20_rebin%d", i);
        hfg_TrkTrgPt12_20_rebin[i] = (TH1F*)f->Get(hname);
        sprintf(hname, "hfg_TrkTrgPt20_30_rebin%d", i);
        hfg_TrkTrgPt20_30_rebin[i] = (TH1F*)f->Get(hname);
        sprintf(hname, "hfg_TrkTrgPt30above_rebin%d", i);
        hfg_TrkTrgPt30above_rebin[i] = (TH1F*)f->Get(hname);

    }
    cout<<" .... done ..."<<endl;
}
//
void ana_hfforest::get_hist(int ich, TFile* f)
{
    if(!f) {
        cout<<" !!! file does not exist,  exit"<<endl;
        exit(0);
    }
    cout<<" .... fetching histogram ...."<<endl;
    char hname[100];
    if(ich!=0) {
        for(short it = 0; it<NTRG; it++) {
            for(short i = 0; i<NPT; i++) {
                sprintf(hname, "hfg%d_%d", it, i);
                hfg[it][i] = (TH1F*)f->Get(hname);
                sprintf(hname, "hfg_raw%d_%d", it, i);
                hfg_raw[it][i] =  (TH1F*)f->Get(hname);
            }
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
        }

    } else {
        // D*
        for(short it = 0; it<NTRG; it++) {
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
        }
    }
    cout<<" .... done ...."<<endl;
}
//
//
void ana_hfforest::define_rebinSpec(int ich)
{
    //clone any mass spectrum to define the combined one then reset them to 0 contents
    char hname[200];
    for(short i = 0; i<NptRebin; i++) {
        for(short it = 0; it<NTRG; it++) {
            if(ich!=0) {
                sprintf(hname, "hfg_rebin%d_%d", it, i);
                hfg_rebin[it][i] = (TH1F*)hfg[it][0]->Clone(hname);
                hfg_rebin[it][i]->Reset();

            } else {
                sprintf(hname, "hfgdiff_rebin%d_%d", it, i);
                hfgdiff_rebin[it][i] = (TH1F*)hfgdiff[it][0]->Clone(hname);

                sprintf(hname, "hbgdiff_rebin%d_%d", it, i);
                hbgdiff_rebin[it][i] = (TH1F*)hbgdiff[it][0]->Clone(hname);

                hfgdiff_rebin[it][i]->Reset();
                hbgdiff_rebin[it][i]->Reset();
            }
        }
        //
        sprintf(hname, "hfg_TrkTrgPt12_20_rebin%d", i);
        hfg_TrkTrgPt12_20_rebin[i] = (TH1F*)hfg_TrkTrgPt12_20[0]->Clone(hname);
        sprintf(hname, "hfg_TrkTrgPt20_30_rebin%d", i);
        hfg_TrkTrgPt20_30_rebin[i] = (TH1F*)hfg_TrkTrgPt20_30[0]->Clone(hname);
        sprintf(hname, "hfg_TrkTrgPt30above_rebin%d", i);
        hfg_TrkTrgPt30above_rebin[i] = (TH1F*)hfg_TrkTrgPt30above[0]->Clone(hname);

        hfg_TrkTrgPt12_20_rebin[i]->Reset();
        hfg_TrkTrgPt20_30_rebin[i]->Reset();
        hfg_TrkTrgPt30above_rebin[i]->Reset();
    }
}

//
void ana_hfforest::define_combSpec(int ich)
{
    //clone any mass spectrum to define the combined one then reset them to 0 contents
    char hname[200];
    for(short i = 0; i<NptRebin; i++) {
        if(ich!=0) {
            sprintf(hname, "hfg_raw_combTrkTrg%d", i);
            hfg_raw_combTrkTrg[i] = (TH1F*)hfg_TrkTrgPt12_20[i]->Clone(hname);
            sprintf(hname, "hfg_combTrkTrg%d", i);
            hfg_combTrkTrg[i] = (TH1F*)hfg_TrkTrgPt12_20[i]->Clone(hname);
            //.. now reset all bin to zero 
            hfg_raw_combTrkTrg[i]->Reset();
            hfg_combTrkTrg[i]->Reset();
        } else {
            sprintf(hname, "hfgdiff_raw_combTrkTrg%d", i);
            hfgdiff_raw_combTrkTrg[i] = (TH1F*)hfgdiff_TrkTrgPt12_20[i]->Clone(hname);
            sprintf(hname, "hfgdiff_combTrkTrg%d", i);
            hfgdiff_combTrkTrg[i] = (TH1F*)hfgdiff_TrkTrgPt12_20[i]->Clone(hname);

            sprintf(hname, "hbgdiff_raw_combTrkTrg%d", i);
            hbgdiff_raw_combTrkTrg[i] = (TH1F*)hfgdiff_TrkTrgPt12_20[i]->Clone(hname);
            sprintf(hname, "hbgdiff_combTrkTrg%d", i);
            hbgdiff_combTrkTrg[i] = (TH1F*)hfgdiff_TrkTrgPt12_20[i]->Clone(hname);

            //.. now reset all bin to zero 
            hfgdiff_raw_combTrkTrg[i]->Reset();
            hfgdiff_combTrkTrg[i]->Reset();
            hbgdiff_raw_combTrkTrg[i]->Reset();
            hbgdiff_combTrkTrg[i]->Reset();
        }
    }
}
//
void ana_hfforest::rebinSpec(int ich, TFile* fout)
{
    cout<<".... rebining the spectra ...."<<endl;
    define_rebinSpec(ich);

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

            if(ich!=0) {
                hfg_rebin[it][im]->Add(hfg[it][i]);

            } else {
                hfgdiff_rebin[it][im]->Add(hfgdiff[it][i]);
                hbgdiff_rebin[it][im]->Add(hbgdiff[it][i]);
            }
        }
    }

    //
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
        hfg_TrkTrgPt12_20_rebin[im]->Add(hfg_TrkTrgPt12_20[i]);
        hfg_TrkTrgPt20_30_rebin[im]->Add(hfg_TrkTrgPt20_30[i]);
        hfg_TrkTrgPt30above_rebin[im]->Add(hfg_TrkTrgPt30above[i]);

    }

    cout<<" ....Writing the spectra ...."<<endl;
    fout->cd();
    //.. save 
    for(short i = 0; i<NptRebin; i++) {
        for(short it = 0; it<NTRG; it++) {
            if(ich!=0) {
                hfg_rebin[it][i]->Write(); 
            } else {
                hfgdiff_rebin[it][i]->Write(); 
                hbgdiff_rebin[it][i]->Write(); 
            }
        }

        //
        hfg_TrkTrgPt12_20_rebin[i]->Write();
        hfg_TrkTrgPt20_30_rebin[i]->Write();
        hfg_TrkTrgPt30above_rebin[i]->Write();

    }
    cout<<"... done ..."<<endl;
}

//
void ana_hfforest::CombineSpec(int ich, TFile* fout)
{
    cout<<".... combining the spectra ...."<<endl;
    define_combSpec(ich);

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

        if(ich!=0) {
            hfg_combTrkTrg[im]->Add(hfg_TrkTrgPt12_20[i]);
            hfg_combTrkTrg[im]->Add(hfg_TrkTrgPt20_30[i]);
            hfg_combTrkTrg[im]->Add(hfg_TrkTrgPt30above[i]);
        } else {
            hfgdiff_combTrkTrg[im]->Add( hfgdiff_TrkTrgPt12_20[i]);
            hfgdiff_combTrkTrg[im]->Add(hfgdiff_TrkTrgPt20_30[i]);
            hfgdiff_combTrkTrg[im]->Add(hfgdiff_TrkTrgPt30above[i]);

            //
            hbgdiff_combTrkTrg[im]->Add( hbgdiff_TrkTrgPt12_20[i]);
            hbgdiff_combTrkTrg[im]->Add(hbgdiff_TrkTrgPt20_30[i]);
            hbgdiff_combTrkTrg[im]->Add(hbgdiff_TrkTrgPt30above[i]);
        }
    }

    cout<<" ....Writing the spectra ...."<<endl;
    fout->cd();
    //.. save 
    for(short i = 0; i<NptRebin; i++) {
        if(ich!=0) {
            hfg_combTrkTrg[i]->Write();   
        } else {
            hfgdiff_combTrkTrg[i]->Write();
            hbgdiff_combTrkTrg[i]->Write();
        }
    }
    cout<<" ... done ...."<<endl;
}

//
void ana_hfforest::drawOneHist(int ich, TH1* h, int nrb)
{
    float sig= 0, err= 0;
    draw_fit(ich, h, nrb, mlow[ich], mup[ich], sig, err);
}
//
void ana_hfforest::drawOneTrg(int ich, int Nrebin, bool chk, int it, TCanvas* cc)
{//..ich: 0=Dstar2D0Pi, 1=D02KPi, 2=Ds2PhiPi, 3=Ds2KstarK, 4=Dpm2KPiPi;
 //.. it: which single trigger 
    char plotName[200];
    char outName[200];
    //
    if(!chk) 
        sprintf(outName, "%s_comb.root", MesonName[ich]);
    else 
        sprintf(outName, "%s_check_it%d.root", MesonName[ich], it);

    TFile outFile(outName, "recreate");
    TH1F* hpt = new TH1F("hpt", "", NptRebin, ptRebin);
    hpt->SetMinimum(1);

    //
    int nrow = 4, ncol = 4;
    cc->Divide(nrow, ncol);

    int ic = 0, iplt= 0;
    float sig= 0, err= 0;
    for(int i = 0; i<NptRebin; i++) {
        cc->cd(ic+1);

        if(ich==0) {
            if(!chk) 
                drawDstar(hfgdiff_combTrkTrg[i], hbgdiff_combTrkTrg[i], Nrebin, sig, err);
            //draw_fit(ich, hfgdiff_combTrkTrg[i], Nrebin, mlow[ich], mup[ich], sig, err);
            else
                drawDstar(hfgdiff_rebin[it][i], hbgdiff_rebin[it][i], Nrebin, sig, err);
            //draw_fit(ich, hfgdiff_rebin[it][i], Nrebin, mlow[ich], mup[ich], sig, err);
        } else if(ich <NCH) {
            if(!chk)
                draw_fit(ich, hfg_combTrkTrg[i], Nrebin, mlow[ich], mup[ich], sig, err);
            else
                draw_fit(ich, hfg_rebin[it][i], Nrebin, mlow[ich], mup[ich], sig, err);
        } else {
            cout<<" !!! no such D meson  !!!"<<endl;
        }

        if(ic == nrow*ncol-1 || i==NptRebin-1) {
            if(!chk)
                sprintf(plotName, "%s_plot%d_comb.gif", MesonName[ich], iplt);
            else
                sprintf(plotName, "%s_plot%d_check_it%d.gif", MesonName[ich], iplt, it);

            cc->SaveAs(plotName);
            cc->Clear();
            cc->Divide(nrow, ncol);
            iplt++;
            ic = 0;
        } else 
            ic++;

        // correct pT bin width ..
        sig /= hpt->GetBinWidth(i+1);
        err /= hpt->GetBinWidth(i+1);
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
void ana_hfforest::drawHighMult(int ich, int whichTrg, int Nrebin, TFile* f, TCanvas* cc)
{//..ich: 0=Dstar2D0Pi, 1=D02KPi, 2=Ds2PhiPi, 3=Ds2KstarK, 4=Dpm2KPiPi;
 //.. whichTrg: see trg_name ..
    char plotName[200];

    get_hist(ich, f);

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
    //.. fit with a Gaussian and pol or expo
    //TF1* fit_fun = new TF1("fit_fun", "[0]*(1/[2]/sqrt(6.28)*exp(-0.5*pow((x-[1])/[2], 2))) + pol2(3) + [6]*(1/[8]/sqrt(6.28)*exp(-0.5*pow((x-[7])/[8], 2)))", rlow, rhigh);
    TF1* fit_fun = new TF1("fit_fun", "[0]*(1/[2]/sqrt(6.28)*exp(-0.5*pow((x-[1])/[2], 2))) + expo(3) + [6]*(1/[8]/sqrt(6.28)*exp(-0.5*pow((x-[7])/[8], 2)))", rlow, rhigh);
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
        //fit_fun->SetParLimits(0, 0, 1e20);
        fit_fun->SetParLimits(1,TMath::Max(zero,MesonMass[ich]-var_mean), MesonMass[ich]+var_mean);
        fit_fun->SetParLimits(2, TMath::Max(zero, p2-var_width),  p2+var_width);

        if(ich==2) {
            //fit_fun->SetParLimits(6, 0, 1e20);
            fit_fun->SetParLimits(7,TMath::Max(zero, MesonMass[4]-var_mean), MesonMass[4]+var_mean);
            fit_fun->SetParLimits(8, TMath::Max(zero, p8-var_width),  p8+var_width);
        } else {//.. all D except Ds->phi+pi have only one peak. remove the 2nd peak
            fit_fun->FixParameter(6, 0);
            fit_fun->FixParameter(7, 0);
            fit_fun->FixParameter(8, 0);
        }

        //.. using likelyhood. Chi2 bias total counts
        for(int ii = 0; ii<10; ii++)
            h->Fit(fit_fun,  "", "", rlow, rhigh);
            //!!!-- I can't get the fit error from likelihood fit right 
            //!!!-- the mean is usable for systematic check
            //h->Fit(fit_fun,  "WL", "", rlow, rhigh);

        //.. draw foreground and background ..
        cout<<" hist: "<<h->GetName()<<endl;
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

        if(pass==1) {//.. change the par range only
            cout<<" var(mean), var(width) ?"<<endl;
            scanf("%f%f", &var_mean, &var_width);
        } else if(pass==2) {//.. change the fitting range only
            cout<<" rlow, rhigh? "<<endl;
            scanf("%f%f", &rlow, &rhigh);
        } else if(pass==3) {// change both par and fitting range
            cout<<" var(mean), var(width) ?"<<endl;
            scanf("%f%f", &var_mean, &var_width);
            cout<<" rlow, rhigh? "<<endl;
            scanf("%f%f", &rlow, &rhigh);
        }
    }

    // correct mass bin width
    sig = fit_fun->GetParameter(0)/h->GetBinWidth(1);
    err = fit_fun->GetParError(0)/h->GetBinWidth(1);

    cout<<"total number of D: "<<sig<<"+/-"<<err<<endl;
}

//
void ana_hfforest::FillTrgMaxPt()
{
    //.. max pT for trktrg, jetTrg and photrg
    for(int it = 0; it<=3; it++) {
        if(trg[it]) 
            ntMaxTrgPt->Fill(maxTrkTrgPt, 0, it, prscl[it]);
    }
    //
    float weight = 0;
    if(trg[0] && maxTrkTrgPt <TrkTrgOffThres[1]) {
        weight = prscl[0]*prscl[4];
        ntMaxTrgPt->Fill(maxTrkTrgPt,0,  100, weight);
    } else if(trg[1] && maxTrkTrgPt >=TrkTrgOffThres[1] && maxTrkTrgPt < TrkTrgOffThres[2]) {
        weight = prscl[1];
        ntMaxTrgPt->Fill(maxTrkTrgPt,0,  101, weight);
    } else if(trg[2] && maxTrkTrgPt >=TrkTrgOffThres[2] && maxTrkTrgPt <TrkTrgOffThres[3]) {
        weight = prscl[2];
        ntMaxTrgPt->Fill(maxTrkTrgPt,0,  102, weight);
    } else if(trg[3] && maxTrkTrgPt >=TrkTrgOffThres[3]) {
        weight = prscl[3];
        ntMaxTrgPt->Fill(maxTrkTrgPt,0,  103, weight);
    }
}

//
void ana_hfforest::find_hp_trkPtErr(float q_in, float pt_in, float eta_in, float phi_in, bool& hp, float& pterr)
{
    for(int itrk = 0; itrk<nTrk; itrk++) {
        if(q_in == trkCharge[itrk] && 
           fabs(pt_in/trkPt[itrk] -1) < maxDiff && 
           fabs(eta_in/trkEta[itrk] -1) < maxDiff && 
           fabs(phi_in/trkPhi[itrk] -1) < maxDiff) {  

            hp = highPurity[itrk];
            pterr = trkPtError[itrk];

            return;
        }
    }
}

//..
void ana_hfforest::fill_ntMass(float mass, float ptmax, int ipt, int itrg, bool isbg, float weight)
{
    float nt_nt[200] = {};
    int i = 0;
    nt_nt[i++] = mass; 
    nt_nt[i++] = ptmax;
    nt_nt[i++] = ipt;
    nt_nt[i++] = itrg;
    nt_nt[i++] = isbg;
    nt_nt[i++] = weight;
    nt_nt[i++] = is_hp_0;
    nt_nt[i++] = is_hp_1;
    nt_nt[i++] = is_hp_2;
    nt_nt[i++] = tPt0;
    nt_nt[i++] = tPt1;
    nt_nt[i++] = tPt2;
    nt_nt[i++] = tPtErr0;
    nt_nt[i++] = tPtErr1;
    nt_nt[i++] = tPtErr2;
    nt_nt[i++] = is_hp;
    nt_nt[i++] = tPtErr;

    ntMass->Fill(nt_nt);
}
//
void ana_hfforest::LoopOverTrk()
{
    float weight = 0;
    for(int itrg = 0; itrg<=3; itrg++) {
        if(trg[itrg]) {
            if(itrg==0) 
                weight = prscl[0]*prscl[4];
            else 
                weight = prscl[itrg];

            for(int itrk = 0; itrk<nTrk; itrk++) {
                fill_ntTrk(trkPt[itrk], trkPtError[itrk], maxTrkTrgPt, tPtErr, highPurity[itrk], is_hp, itrg, weight);
            }
        }
    }

    //
    int itrg = -1;
    if(trg[0] && maxTrkTrgPt <TrkTrgOffThres[1]) {
        weight = prscl[0]*prscl[4];
        itrg = 100;
    } else if(trg[1] && maxTrkTrgPt >=TrkTrgOffThres[1] && maxTrkTrgPt < TrkTrgOffThres[2]) {
        weight = prscl[1];
        itrg = 101;
    } else if(trg[2] && maxTrkTrgPt >=TrkTrgOffThres[2] && maxTrkTrgPt < TrkTrgOffThres[3]) {
        weight = prscl[2];
        itrg = 102;
    } else if(trg[3] && maxTrkTrgPt >= TrkTrgOffThres[3]) {
        weight = prscl[3];
        itrg = 103;
    }

    if(itrg!=-1) {
        for(int itrk = 0; itrk<nTrk; itrk++) 
            fill_ntTrk(trkPt[itrk], trkPtError[itrk], maxTrkTrgPt, tPtErr, highPurity[itrk], is_hp, itrg, weight);
    }
}

//
void ana_hfforest::fill_ntTrk(float pt, float ptErr, float ptmax, float ptmaxErr, bool hp0, bool hp, int itrg, float weight)
{
    float nt_nt[200] = {};
    int i = 0;
    nt_nt[i++] = pt; 
    nt_nt[i++] = ptErr; 
    nt_nt[i++] = ptmax;
    nt_nt[i++] = ptmaxErr;
    nt_nt[i++] = hp0;
    nt_nt[i++] = hp;
    nt_nt[i++] = itrg;
    nt_nt[i++] = weight;

    ntTrk->Fill(nt_nt);
}

//
void ana_hfforest::book_ppTrk_hist()
{
    char hname[100];
    for(short it = 0; it<NFullTrkTrg; it++) {
        sprintf(hname, "hPtTrk%d", it);
        hPtTrk[it] = new TH1F(hname, "", 1000, 0, 1000);
        hPtTrk[it]->Sumw2();
    }

    hPtTrk0_12 = new TH1F("hPtTrk0_12", "", 1000, 0, 1000);
    hPtTrk12_20 = new TH1F("hPtTrk12_20", "", 1000, 0, 1000);
    hPtTrk20_30 = new TH1F("hPtTrk20_30", "", 1000, 0, 1000);
    hPtTrk30Above = new TH1F("hPtTrk30Above", "", 1000, 0, 1000);
    hPtTrkComb = new TH1F("hPtTrkComb", "", 1000, 0, 1000);
    hPtTrk0_12->Sumw2();
    hPtTrk12_20->Sumw2();
    hPtTrk20_30->Sumw2();
    hPtTrk30Above->Sumw2();
    hPtTrkComb->Sumw2();
}

//
void ana_hfforest::fillPPTrkHist(bool hp0_in, bool hp_in, float cut,  TFile* fin, TFile*fout)
{
    book_ppTrk_hist();

    float pt=-1, ptErr=-1, ptmax=-1, ptmaxErr = -1, hp0=-1, hp=-1, itrg=-1, weight=-1;
    ntTrk = (TNtuple*) fin->Get("ntTrk");
    ntTrk->SetBranchAddress("pt", &pt);
    ntTrk->SetBranchAddress("ptErr", &ptErr);
    ntTrk->SetBranchAddress("ptmax", &ptmax);
    ntTrk->SetBranchAddress("ptmaxErr", &ptmaxErr);
    ntTrk->SetBranchAddress("hp0", &hp0);
    ntTrk->SetBranchAddress("hp", &hp);
    ntTrk->SetBranchAddress("itrg", &itrg);
    ntTrk->SetBranchAddress("weight", &weight);

    long int nentries = ntTrk->GetEntries();
    for(long int i = 0; i<nentries; i++) {
        ntTrk->GetEntry(i);

        if(!(i%1000000)) 
            cout<<i<<" of "<<nentries<<endl;

        if(hp0 == (float)hp0_in && hp == (float) hp_in  && ptErr/pt < cut && ptmaxErr/ptmax<cut) {

            if(itrg<=3)
                hPtTrk[(int)itrg]->Fill(pt, weight);
            else if(itrg==100)
                hPtTrk0_12->Fill(pt, weight);
            else if(itrg==101)
                hPtTrk12_20->Fill(pt, weight);
            else if(itrg==102)
                hPtTrk20_30->Fill(pt, weight);
            else if(itrg==103)
                hPtTrk30Above->Fill(pt, weight);

        }
    }

    //
    hPtTrkComb->Add(hPtTrk0_12);
    hPtTrkComb->Add(hPtTrk12_20);
    hPtTrkComb->Add(hPtTrk20_30);
    hPtTrkComb->Add(hPtTrk30Above);

    //
    fout->cd();
    for(short it = 0; it<NFullTrkTrg; it++) {
        hPtTrk[it]->Write();
    }

    hPtTrk0_12->Write();
    hPtTrk12_20->Write();
    hPtTrk20_30->Write();
    hPtTrk30Above->Write();
    hPtTrkComb->Write();
}
//
void ana_hfforest::fillntMaxTrgPtAlgo2Hist(bool hp_in, float cut,  TFile* fin, TFile*fout, bool dcaCut)
{
    book_ppTrk_hist();
    //ntMaxTrgPtAlgo2 = new TNtuple("ntMaxTrgPtAlgo2", "maximum trigger pT spectra for debugging for algo#2", "ptmax:hp:tDz1:tDz1Err:tDxy1:tDxyErr:tPtErr:itrg:weight");

    float PtErr=-1, ptmax=-1, hp=-1, itrg=-1, weight=-1, tDz1, tDz1Err, tDxy1, tDxyErr;
    ntMaxTrgPtAlgo2 = (TNtuple*) fin->Get("ntMaxTrgPtAlgo2");

    ntMaxTrgPtAlgo2->SetBranchAddress("ptmax", &ptmax);
    ntMaxTrgPtAlgo2->SetBranchAddress("hp", &hp);
    ntMaxTrgPtAlgo2->SetBranchAddress("tPtErr", &PtErr);
    ntMaxTrgPtAlgo2->SetBranchAddress("itrg", &itrg);
    ntMaxTrgPtAlgo2->SetBranchAddress("weight", &weight);
    ntMaxTrgPtAlgo2->SetBranchAddress("tDz1", &tDz1);
    ntMaxTrgPtAlgo2->SetBranchAddress("tDz1Err", &tDz1Err);
    ntMaxTrgPtAlgo2->SetBranchAddress("tDxy1", &tDxy1);
    ntMaxTrgPtAlgo2->SetBranchAddress("tDxyErr", &tDxyErr);

    int nentries = ntMaxTrgPtAlgo2->GetEntries();
    for(int i = 0; i<nentries; i++) {
        ntMaxTrgPtAlgo2->GetEntry(i);

        if(!(i%1000000)) 
            cout<<i<<" of "<<nentries<<endl;

        if(hp == (float) hp_in  && PtErr/ptmax < cut) {
            if(dcaCut) {
                if(fabs(tDz1/tDz1Err) >3 || fabs(tDxy1/tDxyErr)>3) 
                    continue;
            }

            if(itrg<=3 && itrg>=0)
                hPtTrk[(int)itrg]->Fill(ptmax, weight);
            else if(itrg==100)
                hPtTrk0_12->Fill(ptmax, weight);
            else if(itrg==101)
                hPtTrk12_20->Fill(ptmax, weight);
            else if(itrg==102)
                hPtTrk20_30->Fill(ptmax, weight);
            else if(itrg==103)
                hPtTrk30Above->Fill(ptmax, weight);

        }
    }

    //
    hPtTrkComb->Add(hPtTrk0_12);
    hPtTrkComb->Add(hPtTrk12_20);
    hPtTrkComb->Add(hPtTrk20_30);
    hPtTrkComb->Add(hPtTrk30Above);

    //
    fout->cd();
    for(short it = 0; it<NFullTrkTrg; it++) {
        hPtTrk[it]->Write();
    }

    hPtTrk0_12->Write();
    hPtTrk12_20->Write();
    hPtTrk20_30->Write();
    hPtTrk30Above->Write();
    hPtTrkComb->Write();
}
//
//
void ana_hfforest::fill_ntMass_hist(int ich, float cut,  TFile* fin, TFile*fout, bool noRandFire)
{
    if(noRandFire) 
        cout<<" --- remove all random trigger benefit ----"<<endl;
    else {
        cout<<" --- keep all random trigger benefit ----"<<endl;
        cout<<" Note: combined spectrum will be inconsistent with the FullTrk12"<<endl;
        cout<<"since the online threshold is 12 but low comb region is 14.  "<<endl;
    }

    //
    book_hist(ich);

    float mass = -1;
    float ptmax = -1;
    float ipt = -1;
    float itrg = -1;
    float isbg = -1;
    float weight = -1;
    float hp0 = -1;
    float hp1 = -1;
    float hp2 = -1;
    float pt0 = -1;
    float pt1 = -1;
    float pt2 = -1;
    float ptErr0 = -1;
    float ptErr1 = -1;
    float ptErr2 = -1;
    float hp = -1;
    float PtErr = -1;

    //
    ntMass = (TNtuple*) fin->Get("ntMass");

    ntMass->SetBranchAddress("mass", &mass);
    ntMass->SetBranchAddress("ptmax", &ptmax);
    ntMass->SetBranchAddress("ipt", &ipt);
    ntMass->SetBranchAddress("itrg", &itrg);
    ntMass->SetBranchAddress("isbg", &isbg);
    ntMass->SetBranchAddress("weight", &weight);
    ntMass->SetBranchAddress("hp0", &hp0);
    ntMass->SetBranchAddress("hp1", &hp1);
    ntMass->SetBranchAddress("hp2", &hp2);
    ntMass->SetBranchAddress("pt0", &pt0);
    ntMass->SetBranchAddress("pt1", &pt1);
    ntMass->SetBranchAddress("pt2", &pt2);
    ntMass->SetBranchAddress("ptErr0", &ptErr0);
    ntMass->SetBranchAddress("ptErr1", &ptErr1);
    ntMass->SetBranchAddress("ptErr2", &ptErr2);
    ntMass->SetBranchAddress("hp", &hp);
    ntMass->SetBranchAddress("tPtErr", &PtErr);


    long int nentries = ntMass->GetEntries();
    for(long int i = 0; i<nentries; i++) {
        ntMass->GetEntry(i);

        if(!(i%1000000)) 
            cout<<i<<" of "<<nentries<<endl;

        if(hp ==0 || tPtErr/ptmax > cut || 
                ptErr0/pt0 > cut || 
                ptErr1/pt1 > cut || 
                ptErr2/pt2 > cut)
            continue; 

        if(itrg<=3 && itrg>=0) {

            if(noRandFire && pt0<TrkTrgOffThres[(int)itrg] && 
               pt1<TrkTrgOffThres[(int)itrg] && pt2<TrkTrgOffThres[(int)itrg]) 
                continue;

            if(ich!=0) {
                hfg[(int)itrg][(int)ipt]->Fill(mass, weight);
                hfg_raw[(int)itrg][(int)ipt]->Fill(mass, weight);
            } else {
                if(isbg==0)  {
                    hfgdiff[(int)itrg][(int)ipt]->Fill(mass, weight);
                    hfgdiff_raw[(int)itrg][(int)ipt]->Fill(mass, weight);
                } else {
                    hbgdiff[(int)itrg][(int)ipt]->Fill(mass, weight);
                    hbgdiff_raw[(int)itrg][(int)ipt]->Fill(mass, weight);
                }
            }
        } else if(itrg==100) {
            if(noRandFire && 
               (pt0>TrkTrgOffThres[1] || pt1>TrkTrgOffThres[1] || pt2>TrkTrgOffThres[1])) 
                continue;

            if(ich!=0) {
                hfg_ZBiasSglTrkPt0_12_raw[(int)ipt]->Fill(mass, weight);
                hfg_ZBiasSglTrkPt0_12[(int)ipt]->Fill(mass, weight); 
            } else {
                if(isbg==0) {
                    hfgdiff_ZBiasSglTrkPt0_12_raw[(int)ipt]->Fill(mass, weight);  
                    hfgdiff_ZBiasSglTrkPt0_12[(int)ipt]->Fill(mass, weight);  
                } else {
                    hbgdiff_ZBiasSglTrkPt0_12_raw[(int)ipt]->Fill(mass, weight);  
                    hbgdiff_ZBiasSglTrkPt0_12[(int)ipt]->Fill(mass, weight);  
                }
            }

        } else if(itrg==101) {
            if(noRandFire && 
               (pt0<TrkTrgOffThres[1] || pt0>TrkTrgOffThres[2]) &&
               (pt1<TrkTrgOffThres[1] || pt1>TrkTrgOffThres[2]) && 
               (pt2<TrkTrgOffThres[1] || pt2>TrkTrgOffThres[2])) 
                continue;

            if(ich!=0) {
                hfg_TrkTrgPt12_20_raw[(int)ipt]->Fill(mass, weight); 
                hfg_TrkTrgPt12_20[(int)ipt]->Fill(mass, weight);  
            } else {
                if(isbg==0) {
                    hfgdiff_TrkTrgPt12_20_raw[(int)ipt]->Fill(mass, weight); 
                    hfgdiff_TrkTrgPt12_20[(int)ipt]->Fill(mass, weight);  
                } else {
                    hbgdiff_TrkTrgPt12_20_raw[(int)ipt]->Fill(mass, weight); 
                    hbgdiff_TrkTrgPt12_20[(int)ipt]->Fill(mass, weight);  
                }
            }
        } else if(itrg==102) {
            if(noRandFire && 
               (pt0<TrkTrgOffThres[2] || pt0>TrkTrgOffThres[3]) &&
               (pt1<TrkTrgOffThres[2] || pt1>TrkTrgOffThres[3]) && 
               (pt2<TrkTrgOffThres[2] || pt2>TrkTrgOffThres[3])) 
                continue;

            if(ich!=0) {
                hfg_TrkTrgPt20_30_raw[(int)ipt]->Fill(mass, weight);  
                hfg_TrkTrgPt20_30[(int)ipt]->Fill(mass, weight);  
            } else {
                if(isbg==0) {
                    hfgdiff_TrkTrgPt20_30_raw[(int)ipt]->Fill(mass, weight);  
                    hfgdiff_TrkTrgPt20_30[(int)ipt]->Fill(mass, weight);  
                } else {
                    hfgdiff_TrkTrgPt30above_raw[(int)ipt]->Fill(mass, weight);  
                    hfgdiff_TrkTrgPt30above[(int)ipt]->Fill(mass, weight);  
                }
            }

        } else if(itrg==103) {
            if(noRandFire && 
               (pt0<TrkTrgOffThres[3]) && 
               (pt1<TrkTrgOffThres[3]) && 
               (pt2<TrkTrgOffThres[3])) 
                continue;

            if(ich!=0) {
                hfg_TrkTrgPt30above_raw[(int)ipt]->Fill(mass, weight);  
                hfg_TrkTrgPt30above[(int)ipt]->Fill(mass, weight);  
            } else {
                if(isbg==0) {
                    hfgdiff_TrkTrgPt30above_raw[(int)ipt]->Fill(mass, weight);  
                    hfgdiff_TrkTrgPt30above[(int)ipt]->Fill(mass, weight);  
                } else {
                    hbgdiff_TrkTrgPt30above_raw[(int)ipt]->Fill(mass, weight);
                    hbgdiff_TrkTrgPt30above[(int)ipt]->Fill(mass, weight);
                }
            }
        }
    }

    //
    fout->cd();
    write_hist(ich);
}
