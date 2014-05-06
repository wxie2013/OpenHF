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

    for(short iy = 0; iy<NY; iy++) 
        for(short ich = 0; ich<NCH; ich++) 
            for(short it = 0; it<NTRG; it++) 
                for(short i = 0; i<NPT; i++) 
                    delete hfg[iy][ich][it][i];

    for(short iy = 0; iy<NY; iy++) 
        for(short it = 0; it<NTRG; it++) 
            for(short i = 0; i<NPT; i++) {
                delete hfgdiff[iy][it][i];
                delete hbgdiff[iy][it][i];

            }

    for(int i = 0; i<NTRG; i++) 
        delete trg_obj[i];

    for(short iy = 0; iy<NY; iy++) 
        for(short ich = 0; ich<NCH; ich++) 
            for(short i = 0; i<NPT; i++) {
                delete hfg_ZBiasSglTrkPt0_12_raw[iy][ich][i];
                delete hfg_ZBiasSglTrkPt0_12[iy][ich][i];

                delete hfg_TrkTrgPt12_20_raw[iy][ich][i];
                delete hfg_TrkTrgPt12_20[iy][ich][i];

                delete hfg_TrkTrgPt20_30_raw[iy][ich][i];
                delete hfg_TrkTrgPt20_30[iy][ich][i];

                delete hfg_TrkTrgPt30above_raw[iy][ich][i];
                delete hfg_TrkTrgPt30above[iy][ich][i];
            }

    //
    for(short iy = 0; iy<NY; iy++) 
        for(short i = 0; i<NPT; i++) {
            delete hfgdiff_ZBiasSglTrkPt0_12_raw[iy][i];
            delete hfgdiff_ZBiasSglTrkPt0_12[iy][i];

            delete hfgdiff_TrkTrgPt12_20_raw[iy][i];
            delete hfgdiff_TrkTrgPt12_20[iy][i];

            delete hfgdiff_TrkTrgPt20_30_raw[iy][i];
            delete hfgdiff_TrkTrgPt20_30[iy][i];

            delete hfgdiff_TrkTrgPt30above_raw[iy][i];
            delete hfgdiff_TrkTrgPt30above[iy][i];

            delete hbgdiff_ZBiasSglTrkPt0_12_raw[iy][i];
            delete hbgdiff_ZBiasSglTrkPt0_12[iy][i];

            delete hbgdiff_TrkTrgPt12_20_raw[iy][i];
            delete hbgdiff_TrkTrgPt12_20[iy][i];

            delete hbgdiff_TrkTrgPt20_30_raw[iy][i];
            delete hbgdiff_TrkTrgPt20_30[iy][i];

            delete hbgdiff_TrkTrgPt30above_raw[iy][i];
            delete hbgdiff_TrkTrgPt30above[iy][i];
        }

        for(short i = 0; i<NPT; i++) {
            delete hMatchWin[i];
            delete hRecoTrgPtDiff[i];
        }
}

//
void ana_hfforest::Init(int startFile, int endFile, char *filelist)
{//.. called right after constructor 
    filename = new char[1000];
    sprintf(filename, "ana_hhforest_%s_%dTo%d.root",  filelist, startFile, endFile);
    result = new TFile(filename, "recreate");

    //
    define_cuts();
    get_trg_name();
    book_hist();

    //
    hfcandidate = new hfcand_v0;

    //HiTree = 0;
    HltTree = 0;
    hftree = 0;

    //.. trigger object ...
    for(int i = 0; i<NTRG; i++) {
        trg_obj[i] = new trigO;
    }
}

//
void ana_hfforest::book_hist()
{
    float pt_low = 0, pt_high = 0;
    char hname[100], pt_range[1000];

    //
    for(short i = 0; i<NPT; i++) {
        get_pt_range(i, pt_low, pt_high);
        sprintf(pt_range, "pt: %f %f", pt_low, pt_high);

        sprintf(hname, "hMatchWin%d", i);
        hMatchWin[i] = new TH1F(hname, pt_range, NBIN, -1, 1);

        sprintf(hname, "hRecoTrgPtDiff%d", i);
        hRecoTrgPtDiff[i] = new TH1F(hname, pt_range, NBIN, -1, 1);

        hMatchWin[i]->Sumw2();
        hRecoTrgPtDiff[i]->Sumw2();
    }

    //
    for(short iy = 0; iy<NY; iy++) 
        for(short ich = 0; ich<NCH; ich++) 
            for(short it = 0; it<NTRG; it++) 
                for(short i = 0; i<NPT; i++) {

                    get_pt_range(i, pt_low, pt_high);

                    sprintf(pt_range, "ch: %s, rapidity:%d, trg: %s,  pt: %f %f", MesonName[ich], iy, trg_name[it], pt_low, pt_high);

                    //
                    sprintf(hname, "hfg%d_%d_%d_%d", iy, ich, it, i);
                    hfg[iy][ich][it][i] = new TH1F(hname, pt_range, NBIN, cut_m_low[ich], cut_m_high[ich]);

                    //.. track error ...
                    hfg[iy][ich][it][i]->Sumw2();
                }

    //
    for(short iy = 0; iy<NY; iy++) 
        for(short it = 0; it<NTRG; it++) 
            for(short i = 0; i<NPT; i++) {

                get_pt_range(i, pt_low, pt_high);

                sprintf(pt_range, "D*, rapidity: %d, trg: %s,  pt: %f %f", iy, trg_name[it], pt_low, pt_high);

                sprintf(hname, "hfgdiff%d_%d_%d", iy, it, i);
                hfgdiff[iy][it][i] = new TH1F(hname, pt_range, NBIN, 0.135, 0.154);

                sprintf(hname, "hbgdiff%d_%d_%d", iy, it, i);
                hbgdiff[iy][it][i] = new TH1F(hname, pt_range, NBIN, 0.135, 0.154);

                //.. track error ...
                hfgdiff[iy][it][i]->Sumw2();
                hbgdiff[iy][it][i]->Sumw2();
            }

    // components for trigger combination ..
    for(short iy = 0; iy<NY; iy++) 
        for(short ich = 0; ich<NCH; ich++) 
            for(short i = 0; i<NPT; i++) {

                get_pt_range(i, pt_low, pt_high);

                sprintf(pt_range, "ch: %s, rapidity:%d, pt: %f %f", MesonName[ich], iy, pt_low, pt_high);

                sprintf(hname, "hfg_ZBiasSglTrkPt0_12_raw%d_%d_%d", iy, ich, i);
                hfg_ZBiasSglTrkPt0_12_raw[iy][ich][i] = new TH1F(hname, pt_range, NBIN, cut_m_low[ich], cut_m_high[ich]);
                sprintf(hname, "hfg_ZBiasSglTrkPt0_12%d_%d_%d", iy, ich, i);
                hfg_ZBiasSglTrkPt0_12[iy][ich][i] = new TH1F(hname, pt_range, NBIN, cut_m_low[ich], cut_m_high[ich]);

                sprintf(hname, "hfg_TrkTrgPt12_20_raw%d_%d_%d", iy, ich, i);
                hfg_TrkTrgPt12_20_raw[iy][ich][i] = new TH1F(hname, pt_range, NBIN, cut_m_low[ich], cut_m_high[ich]);
                sprintf(hname, "hfg_TrkTrgPt12_20%d_%d_%d", iy, ich, i);
                hfg_TrkTrgPt12_20[iy][ich][i] = new TH1F(hname, pt_range, NBIN, cut_m_low[ich], cut_m_high[ich]);

                sprintf(hname, "hfg_TrkTrgPt20_30_raw%d_%d_%d", iy, ich, i);
                hfg_TrkTrgPt20_30_raw[iy][ich][i] = new TH1F(hname, pt_range, NBIN, cut_m_low[ich], cut_m_high[ich]);
                sprintf(hname, "hfg_TrkTrgPt20_30%d_%d_%d", iy, ich, i);
                hfg_TrkTrgPt20_30[iy][ich][i] = new TH1F(hname, pt_range, NBIN, cut_m_low[ich], cut_m_high[ich]);

                sprintf(hname, "hfg_TrkTrgPt30above_raw%d_%d_%d", iy, ich, i);
                hfg_TrkTrgPt30above_raw[iy][ich][i] = new TH1F(hname, pt_range, NBIN, cut_m_low[ich], cut_m_high[ich]);
                sprintf(hname, "hfg_TrkTrgPt30above%d_%d_%d", iy, ich, i);
                hfg_TrkTrgPt30above[iy][ich][i] = new TH1F(hname, pt_range, NBIN, cut_m_low[ich], cut_m_high[ich]);

                //.. track error ...
                hfg_ZBiasSglTrkPt0_12_raw[iy][ich][i]->Sumw2();
                hfg_ZBiasSglTrkPt0_12[iy][ich][i]->Sumw2();

                hfg_TrkTrgPt12_20_raw[iy][ich][i]->Sumw2();
                hfg_TrkTrgPt12_20[iy][ich][i]->Sumw2();

                hfg_TrkTrgPt20_30_raw[iy][ich][i]->Sumw2();
                hfg_TrkTrgPt20_30[iy][ich][i]->Sumw2();

                hfg_TrkTrgPt30above_raw[iy][ich][i]->Sumw2();
                hfg_TrkTrgPt30above[iy][ich][i]->Sumw2();
            }

    //
    for(short iy = 0; iy<NY; iy++) 
        for(short i = 0; i<NPT; i++) {

            get_pt_range(i, pt_low, pt_high);

            sprintf(pt_range, "D*, rapidity: %d, pt: %f %f", iy, pt_low, pt_high);

            sprintf(hname, "hfgdiff_ZBiasSglTrkPt0_12_raw%d_%d", iy, i);
            hfgdiff_ZBiasSglTrkPt0_12_raw[iy][i] = new TH1F(hname, pt_range, NBIN, 0.135, 0.154);
            sprintf(hname, "hfgdiff_ZBiasSglTrkPt0_12%d_%d", iy, i);
            hfgdiff_ZBiasSglTrkPt0_12[iy][i] = new TH1F(hname, pt_range, NBIN, 0.135, 0.154);

            sprintf(hname, "hfgdiff_TrkTrgPt12_20_raw%d_%d", iy, i);
            hfgdiff_TrkTrgPt12_20_raw[iy][i] = new TH1F(hname, pt_range, NBIN, 0.135, 0.154);
            sprintf(hname, "hfgdiff_TrkTrgPt12_20%d_%d", iy, i);
            hfgdiff_TrkTrgPt12_20[iy][i] = new TH1F(hname, pt_range, NBIN, 0.135, 0.154);

            sprintf(hname, "hfgdiff_TrkTrgPt20_30_raw%d_%d", iy, i);
            hfgdiff_TrkTrgPt20_30_raw[iy][i] = new TH1F(hname, pt_range, NBIN, 0.135, 0.154);
            sprintf(hname, "hfgdiff_TrkTrgPt20_30%d_%d", iy, i);
            hfgdiff_TrkTrgPt20_30[iy][i] = new TH1F(hname, pt_range, NBIN, 0.135, 0.154);

            sprintf(hname, "hfgdiff_TrkTrgPt30above_raw%d_%d", iy, i);
            hfgdiff_TrkTrgPt30above_raw[iy][i] = new TH1F(hname, pt_range, NBIN, 0.135, 0.154);
            sprintf(hname, "hfgdiff_TrkTrgPt30above%d_%d", iy, i);
            hfgdiff_TrkTrgPt30above[iy][i] = new TH1F(hname, pt_range, NBIN, 0.135, 0.154);


            sprintf(hname, "hbgdiff_ZBiasSglTrkPt0_12_raw%d_%d", iy, i);
            hbgdiff_ZBiasSglTrkPt0_12_raw[iy][i] = new TH1F(hname, pt_range, NBIN, 0.135, 0.154);
            sprintf(hname, "hbgdiff_ZBiasSglTrkPt0_12%d_%d", iy, i);
            hbgdiff_ZBiasSglTrkPt0_12[iy][i] = new TH1F(hname, pt_range, NBIN, 0.135, 0.154);

            sprintf(hname, "hbgdiff_TrkTrgPt12_20_raw%d_%d", iy, i);
            hbgdiff_TrkTrgPt12_20_raw[iy][i] = new TH1F(hname, pt_range, NBIN, 0.135, 0.154);
            sprintf(hname, "hbgdiff_TrkTrgPt12_20%d_%d", iy, i);
            hbgdiff_TrkTrgPt12_20[iy][i] = new TH1F(hname, pt_range, NBIN, 0.135, 0.154);

            sprintf(hname, "hbgdiff_TrkTrgPt20_30_raw%d_%d", iy, i);
            hbgdiff_TrkTrgPt20_30_raw[iy][i] = new TH1F(hname, pt_range, NBIN, 0.135, 0.154);
            sprintf(hname, "hbgdiff_TrkTrgPt20_30%d_%d", iy, i);
            hbgdiff_TrkTrgPt20_30[iy][i] = new TH1F(hname, pt_range, NBIN, 0.135, 0.154);

            sprintf(hname, "hbgdiff_TrkTrgPt30above_raw%d_%d", iy, i);
            hbgdiff_TrkTrgPt30above_raw[iy][i] = new TH1F(hname, pt_range, NBIN, 0.135, 0.154);
            sprintf(hname, "hbgdiff_TrkTrgPt30above%d_%d", iy, i);
            hbgdiff_TrkTrgPt30above[iy][i] = new TH1F(hname, pt_range, NBIN, 0.135, 0.154);

            //.. track error ...
            hfgdiff_ZBiasSglTrkPt0_12_raw[iy][i]->Sumw2();
            hfgdiff_ZBiasSglTrkPt0_12[iy][i]->Sumw2();

            hfgdiff_TrkTrgPt12_20_raw[iy][i]->Sumw2();
            hfgdiff_TrkTrgPt12_20[iy][i]->Sumw2();

            hfgdiff_TrkTrgPt20_30_raw[iy][i]->Sumw2();
            hfgdiff_TrkTrgPt20_30[iy][i]->Sumw2();

            hfgdiff_TrkTrgPt30above_raw[iy][i]->Sumw2();
            hfgdiff_TrkTrgPt30above[iy][i]->Sumw2();

            //
            hbgdiff_ZBiasSglTrkPt0_12_raw[iy][i]->Sumw2();
            hbgdiff_ZBiasSglTrkPt0_12[iy][i]->Sumw2();

            hbgdiff_TrkTrgPt12_20_raw[iy][i]->Sumw2();
            hbgdiff_TrkTrgPt12_20[iy][i]->Sumw2();

            hbgdiff_TrkTrgPt20_30_raw[iy][i]->Sumw2();
            hbgdiff_TrkTrgPt20_30[iy][i]->Sumw2();

            hbgdiff_TrkTrgPt30above_raw[iy][i]->Sumw2();
            hbgdiff_TrkTrgPt30above[iy][i]->Sumw2();
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
void ana_hfforest::LoopOverFile(int startFile, int endFile, char *filelist) 
{//.. loop over files in the "*filelist" from startFile to endFile in sequence
    //
    Init(startFile, endFile, filelist);

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
        LoopOverEvt(HltTree);

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
    hftree = (TTree*)f->Get("OpenHfTree/hftree");

    //if(!hftree || !HltTree || !HiTree) {
    if(!hftree || !HltTree) {
        cout<<"==> missing hftree or HltTree <=="<<endl;
        return false;
    }

    HltTree->AddFriend(hftree);
    //HltTree->AddFriend(HiTree);

    hftree->SetBranchAddress("hfcandidate", &hfcandidate);

    get_trg_info(HltTree);

    return true;
}

//
void ana_hfforest::write()
{//.. results 
    result->cd();

    //.. each triggers
    for(short iy = 0; iy<NY; iy++) 
        for(short ich = 0; ich<NCH; ich++) 
            for(short it = 0; it<NTRG; it++) 
                for(short i = 0; i<NPT; i++) 
                    hfg[iy][ich][it][i]->Write();

    for(short iy = 0; iy<NY; iy++) 
        for(short it = 0; it<NTRG; it++) 
            for(short i = 0; i<NPT; i++) {
                hfgdiff[iy][it][i]->Write();
                hbgdiff[iy][it][i]->Write();
            }

    for(short iy = 0; iy<NY; iy++) 
        for(short ich = 0; ich<NCH; ich++) 
            for(short i = 0; i<NPT; i++) {

                hfg_ZBiasSglTrkPt0_12_raw[iy][ich][i]->Write();
                hfg_ZBiasSglTrkPt0_12[iy][ich][i]->Write();

                hfg_TrkTrgPt12_20_raw[iy][ich][i]->Write();
                hfg_TrkTrgPt12_20[iy][ich][i]->Write();

                hfg_TrkTrgPt20_30_raw[iy][ich][i]->Write();
                hfg_TrkTrgPt20_30[iy][ich][i]->Write();

                hfg_TrkTrgPt30above_raw[iy][ich][i]->Write();
                hfg_TrkTrgPt30above[iy][ich][i]->Write();
            }

    //
    for(short iy = 0; iy<NY; iy++) 
        for(short i = 0; i<NPT; i++) {

            hfgdiff_ZBiasSglTrkPt0_12_raw[iy][i]->Write();
            hfgdiff_ZBiasSglTrkPt0_12[iy][i]->Write();

            hfgdiff_TrkTrgPt12_20_raw[iy][i]->Write();
            hfgdiff_TrkTrgPt12_20[iy][i]->Write();

            hfgdiff_TrkTrgPt20_30_raw[iy][i]->Write();
            hfgdiff_TrkTrgPt20_30[iy][i]->Write();

            hfgdiff_TrkTrgPt30above_raw[iy][i]->Write();
            hfgdiff_TrkTrgPt30above[iy][i]->Write();

            //
            hbgdiff_ZBiasSglTrkPt0_12_raw[iy][i]->Write();
            hbgdiff_ZBiasSglTrkPt0_12[iy][i]->Write();

            hbgdiff_TrkTrgPt12_20_raw[iy][i]->Write();
            hbgdiff_TrkTrgPt12_20[iy][i]->Write();

            hbgdiff_TrkTrgPt20_30_raw[iy][i]->Write();
            hbgdiff_TrkTrgPt20_30[iy][i]->Write();

            hbgdiff_TrkTrgPt30above_raw[iy][i]->Write();
            hbgdiff_TrkTrgPt30above[iy][i]->Write();
        }
    //
    for(short i = 0; i<NPT; i++) {
        hMatchWin[i]->Write();
        hRecoTrgPtDiff[i]->Write();
    }

    result->Close();
}

//
void ana_hfforest::get_trg_info(TTree* T) 
{
    reset_trg();

    TObjArray *branches = (TObjArray *)T->GetListOfBranches();
    TIter branch_iter(branches); 
    TBranch* branch = 0;
    while ((branch = (TBranch *)branch_iter.Next())) {  
        TString branch_name = branch->GetName();
        for(int it = 0; it<NTRG; it++) {
            if (branch_name.Contains(trg_name[it])){
                if(branch_name.Contains("Prescl")) {
                    T->SetBranchAddress(branch_name.Data(), &prscl[it]);
                } else if(branch_name.Contains("trigObject")) {
                    T->SetBranchAddress(branch_name.Data(), &trg_obj[it]);
                } else {
                    T->SetBranchAddress(branch_name.Data(), &trg[it]);
                } 

                break;
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
void ana_hfforest::LoopOverEvt(TTree* T)
{//.. loop over each event. Add different analysis function here. 

    for(int i = 0; i<T->GetEntries(); i++) {
        reset_trg();
        T->GetEntry(i);

        LoopOverHFCandidate();
    }
}

//
bool ana_hfforest::GetRapidity(float m, float pt, float eta, int& iY)
{
    float theta = 2*atan(exp(-eta));
    float p = pt/sin(theta);
    float E = sqrt(m*m + p*p);
    float pz = pt/tan(theta);
    float Y = 0.5*log((E+pz)/(E-pz));


    if(Y<=Ymin || Y>=Ymin+NY*dY) 
        return false;

    iY = (Y-Ymin)/dY; 

    return true;
}

//
void ana_hfforest::LoopOverHFCandidate()
{//.. loop over Heavy Flavor candidate in a event 
    for(int i = 0; i<hfcandidate->get_nhfcand(); i++) {
        snglhfcand* cand = hfcandidate->get_hfcand(i);

        FillMassHisto(cand);
    }

    hfcandidate->Reset();
}

//
void ana_hfforest::FillMassHisto(snglhfcand* cand)
{//.. fill the histogram of mass distribution ..
    for(int ich = 0; ich < NCH; ich++) {

        if(cand->get_type() != ich+1) continue;

        float mass = cand->get_fm();
        float fpt = cand->get_fpt();
        float feta = cand->get_feta();
        float ffls3d = cand->get_ffls3d();
        float alpha = cand->get_falpha0();
        float fprob = cand->get_fprob();
        float fdr = cand->get_fdr();
        float fchi2 = cand->get_fchi2();
        float mass_dau = cand->get_fmdau1();

        //.. input for trigger matching ...
        float fetadau2 = cand->get_fetadau2(); 
        float fphidau2 = cand->get_fphidau2(); 
        float feta1 = cand->get_feta1(); 
        float fphi1 = cand->get_fphi1(); 
        float feta2 = cand->get_feta2(); 
        float fphi2 = cand->get_fphi2(); 
        //

        int ipt = get_pt_bin_num(fpt);

        int iy = -1;
        if(!GetRapidity(mass, fpt, feta, iy)) continue;

        if(mass > cut_m_low[ich] && mass < cut_m_high[ich] && 
                ffls3d > cut_ffls3d[ich] && alpha < cut_falpha0[ich] && 
                fprob > cut_fprob[ich] && fdr < cut_fdr[ich] && fchi2 < cut_fchi2[ich]) {

            //.. forground ..
            if(ich==1) { //D0 has no daughter mass, treat separately. ...
                for(int itrg = 0; itrg<NTRG; itrg++) {
                    if(trg[itrg]) 
                        hfg[iy][ich][itrg][ipt]->Fill(mass);
                }

                FillTrgCombine(0, fetadau2, fphidau2, feta1, fphi1, feta2, fphi2, 
                        ich, ipt, iy, mass, mass_dau, fpt);

            } else {
                if(mass_dau > cut_m_dau_low[ich] && mass_dau < cut_m_dau_high[ich]) {
                    for(int itrg = 0; itrg<NTRG; itrg++) {
                        if(trg[itrg]) {
                            hfg[iy][ich][itrg][ipt]->Fill(mass);

                            if(ich==0) 
                                hfgdiff[iy][itrg][ipt]->Fill(mass-mass_dau);
                        }
                    }

                    FillTrgCombine(0, fetadau2, fphidau2, feta1, fphi1, feta2, fphi2, 
                            ich, ipt, iy, mass, mass_dau, fpt);
                    if(ich==0)
                        FillTrgCombine(1, fetadau2, fphidau2, feta1, fphi1, feta2, fphi2, 
                                ich, ipt, iy, mass, mass_dau, fpt);

                } else { //.. background through side band ...
                    for(int itrg = 0; itrg<NTRG; itrg++) {
                        if(trg[itrg] && ich==0) 
                            hbgdiff[iy][itrg][ipt]->Fill(mass-mass_dau);
                    }

                    if(ich==0)
                        FillTrgCombine(2, fetadau2, fphidau2, feta1, fphi1, feta2, fphi2, 
                                ich, ipt, iy, mass, mass_dau, fpt);
                }
            }
        }
    }
}

//
void ana_hfforest::FillTrgCombine(int id, float eta0, float phi0, float eta1, 
        float phi1, float eta2, float phi2, int ich, int ipt, int iy, float mass, float mass_dau, float pt)
{//.. id:  0-->hfg,  1-->hfgdiff, 2-->hbgdiff

    float trgPt0 = -1, trgPt1 = -1,trgPt2 = -1;

    if(trg[0]) {

        if(matchTrgObj(ipt, pt, 0, eta0, phi0, trgPt0) ||
           matchTrgObj(ipt, pt, 0, eta1, phi1, trgPt1) ||
           matchTrgObj(ipt, pt, 0, eta2, phi2, trgPt2)) {

            float maxTrgPt = TMath::Max(trgPt0, TMath::Max(trgPt1, trgPt2));

            if(maxTrgPt < 12) {
                if(id==0) {
                    hfg_ZBiasSglTrkPt0_12_raw[iy][ich][ipt]->Fill(mass);
                    hfg_ZBiasSglTrkPt0_12[iy][ich][ipt]->Fill(mass, prscl[0]*prscl[4]);
                } else if(id==1) {
                    hfgdiff_ZBiasSglTrkPt0_12_raw[iy][ipt]->Fill(mass-mass_dau);
                    hfgdiff_ZBiasSglTrkPt0_12[iy][ipt]->Fill(mass-mass_dau, prscl[0]*prscl[4]);
                } else if(id==2) {
                    hbgdiff_ZBiasSglTrkPt0_12_raw[iy][ipt]->Fill(mass-mass_dau);
                    hbgdiff_ZBiasSglTrkPt0_12[iy][ipt]->Fill(mass-mass_dau, prscl[0]*prscl[4]);
                }
            }
        }
    }

    if(trg[1]) {
        if(matchTrgObj(ipt, pt, 1, eta0, phi0, trgPt0) ||
           matchTrgObj(ipt, pt, 1, eta1, phi1, trgPt1) ||
           matchTrgObj(ipt, pt, 1, eta2, phi2, trgPt2)) {

            float maxTrgPt = TMath::Max(trgPt0, TMath::Max(trgPt1, trgPt2));

            if(maxTrgPt >=12 && maxTrgPt < 20) {
                if(id==0) {
                    hfg_TrkTrgPt12_20_raw[iy][ich][ipt]->Fill(mass);
                    hfg_TrkTrgPt12_20[iy][ich][ipt]->Fill(mass, prscl[1]); //.. L1 base prescale 1
                } else if(id==1) {
                    hfgdiff_TrkTrgPt12_20_raw[iy][ipt]->Fill(mass-mass_dau);
                    hfgdiff_TrkTrgPt12_20[iy][ipt]->Fill(mass-mass_dau, prscl[1]); //.. L1 base prescale 1
                } else if(id==2) {
                    hbgdiff_TrkTrgPt12_20_raw[iy][ipt]->Fill(mass-mass_dau);
                    hbgdiff_TrkTrgPt12_20[iy][ipt]->Fill(mass-mass_dau, prscl[1]); //.. L1 base prescale 1
                }
            }
        }
    }

    if(trg[2])  {
        if(matchTrgObj(ipt, pt, 2, eta0, phi0, trgPt0) ||
           matchTrgObj(ipt, pt, 2, eta1, phi1, trgPt1) ||
           matchTrgObj(ipt, pt, 2, eta2, phi2, trgPt2)) {

            float maxTrgPt = TMath::Max(trgPt0, TMath::Max(trgPt1, trgPt2));

            if(maxTrgPt >=20 && maxTrgPt < 30) {
                if(id==0) {
                    hfg_TrkTrgPt20_30_raw[iy][ich][ipt]->Fill(mass);
                    hfg_TrkTrgPt20_30[iy][ich][ipt]->Fill(mass, prscl[2]); //.. L1 base prescale 1
                } else if(id==1) {
                    hfgdiff_TrkTrgPt20_30_raw[iy][ipt]->Fill(mass-mass_dau);
                    hfgdiff_TrkTrgPt20_30[iy][ipt]->Fill(mass-mass_dau, prscl[2]); //.. L1 base prescale 1
                } else if(id==2) {
                    hbgdiff_TrkTrgPt20_30_raw[iy][ipt]->Fill(mass-mass_dau);
                    hbgdiff_TrkTrgPt20_30[iy][ipt]->Fill(mass-mass_dau, prscl[2]); //.. L1 base prescale 1
                }
            }
        }
    }

    if(trg[3]) {
        if(matchTrgObj(ipt, pt, 3, eta0, phi0, trgPt0) ||
           matchTrgObj(ipt, pt, 3, eta1, phi1, trgPt1) ||
           matchTrgObj(ipt, pt, 3, eta2, phi2, trgPt2)) {

            float maxTrgPt = TMath::Max(trgPt0, TMath::Max(trgPt1, trgPt2));

            if(maxTrgPt >=20 && maxTrgPt < 30) {
                if(id==0) {
                    hfg_TrkTrgPt30above_raw[iy][ich][ipt]->Fill(mass);
                    hfg_TrkTrgPt30above[iy][ich][ipt]->Fill(mass, prscl[3]);// L1 base prescale 1
                } else if(id==1) {
                    hfgdiff_TrkTrgPt30above_raw[iy][ipt]->Fill(mass-mass_dau);
                    hfgdiff_TrkTrgPt30above[iy][ipt]->Fill(mass-mass_dau, prscl[3]);// L1 base prescale 1
                } else if(id==2) {
                    hfgdiff_TrkTrgPt30above_raw[iy][ipt]->Fill(mass-mass_dau);
                    hfgdiff_TrkTrgPt30above[iy][ipt]->Fill(mass-mass_dau, prscl[3]);// L1 base prescale 1
                }
            }
        }
    }
}

//
bool ana_hfforest::matchTrgObj(int ipt, float pt, int tid, float eta, float phi, float& trgPt)
{
    trgPt = -1;
    float min = 999;
    int io = -1;

    //.. find the min distance 
    for(unsigned int i = 0; i<trg_obj[tid]->size(); i++) {
        float dist = sqrt(pow((eta - trg_obj[tid]->at(i).eta()),2) + pow((phi - trg_obj[tid]->at(i).phi()), 2)); 

        if(dist < min) {
            min = dist;
            io = i;
        }
    }     

    hMatchWin[ipt]->Fill(min);

    if(min < matchWindow) {
        trgPt = trg_obj[tid]->at(io).pt();

        hRecoTrgPtDiff[ipt]->Fill((pt-trgPt)/pt);

        return true;
    }

    return false;
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
    trg_name[4]= "L1_ZeroBias";  //.. base of HLT_PAZeroBiasPixel_SingleTrack

    /*
    trg_name[4]= "HLT_PAJet20_NoJetID";   //.. base: L1_SingleJet16_BptxAND
    trg_name[5]= "HLT_PAJet40_NoJetID";   //.. base: L1_SingleJet16_BptxAND
    trg_name[6]= "HLT_PAJet60_NoJetID";   //.. base: L1_SingleJet36
    trg_name[7]= "HLT_PAJet80_NoJetID";   //.. base: L1_SingleJet36
    trg_name[8]= "HLT_PAJet100_NoJetID";   //.. base: L1_SingleJet36

    trg_name[9]= "HLT_PAPhoton10_NoCaloIdVL";   //.. base: L1_SingleEG5_BptxAND
    trg_name[10]= "HLT_PAPhoton15_NoCaloIdVL";   //.. base: L1_SingleEG5_BptxAND
    trg_name[11]= "HLT_PAPhoton20_NoCaloIdVL";   //.. base: L1_SingleEG5_BptxAND
    trg_name[12]= "HLT_PAPhoton30_NoCaloIdVL";   //.. base: L1_SingleEG12

    trg_name[13]= "HLT_PAPixelTrackMultiplicity100_FullTrack12";   //.. base: L1_ETT20_BptxAND
    trg_name[14]= "HLT_PAPixelTrackMultiplicity130_FullTrack12";   //.. base: L1_ETT20_BptxAND
    trg_name[15]= "HLT_PAPixelTrackMultiplicity160_FullTrack12";   //.. base: L1_ETT40
    //.. 30% of pT = 0-10GeV/c is recorded in the following triggers
    trg_name[16]= "HLT_PAPixelTracks_Multiplicity100";   //.. base: L1_ETT20_BptxAND
    trg_name[17]= "HLT_PAPixelTracks_Multiplicity130";   //.. base: L1_ETT20_BptxAND
    trg_name[18]= "HLT_PAPixelTracks_Multiplicity160";   //.. base: L1_ETT40
    trg_name[19]= "HLT_PAPixelTracks_Multiplicity190";   //.. base: L1_ETT40

    trg_name[20]= "HLT_PAHFSumET100";   //.. base: L1_ETT20_BptxAND
    trg_name[21]= "HLT_PAHFSumET140";   //.. base: L1_ETT20_BptxAND
    trg_name[22]= "HLT_PAHFSumET170";   //.. base: L1_ETT20_BptxAND

    //.. base: L1_ZeroBias
    //.. L1 base trigger 
    trg_name[23]= "L1_SingleJet12_BptxAND";
    trg_name[24]= "L1_SingleJet16_BptxAND";
    trg_name[25]= "L1_SingleJet36";
    trg_name[26]= "L1_SingleEG5_BptxAND";
    trg_name[27]= "L1_SingleEG12";
    trg_name[28]= "L1_SingleEG20";
    trg_name[29]= "L1_ETT20_BptxAND";
    trg_name[30]= "L1_ETT40";
    trg_name[31]= "L1_ZeroBias";
    */
}

//
void ana_hfforest::get_hist(TFile* f, int ich)
{
    cout<<"--> getting histogram <---"<<endl;
    char hname[100];

    for(short iy = 0; iy<NY; iy++) 
        for(short ich = 0; ich<NCH; ich++) 
            for(short it = 0; it<NTRG; it++) 
                for(short i = 0; i<NPT; i++) {

                    sprintf(hname, "hfg%d_%d_%d_%d", iy, ich, it, i);
                    hfg[iy][ich][it][i] = (TH1F*)f->Get(hname);
                }

    //
    for(short iy = 0; iy<NY; iy++) 
        for(short it = 0; it<NTRG; it++) 
            for(short i = 0; i<NPT; i++) {

                sprintf(hname, "hfgdiff%d_%d_%d", iy, it, i);
                hfgdiff[iy][it][i] = (TH1F*)f->Get(hname);

                sprintf(hname, "hbgdiff%d_%d_%d", iy, it, i);
                hbgdiff[iy][it][i] = (TH1F*)f->Get(hname);

            }

    // components for trigger combination ..
    for(short iy = 0; iy<NY; iy++) 
        for(short ich = 0; ich<NCH; ich++) 
            for(short i = 0; i<NPT; i++) {

                sprintf(hname, "hfg_ZBiasSglTrkPt0_12_raw%d_%d_%d", iy, ich, i);
                hfg_ZBiasSglTrkPt0_12_raw[iy][ich][i] = (TH1F*)f->Get(hname);
                sprintf(hname, "hfg_ZBiasSglTrkPt0_12%d_%d_%d", iy, ich, i);
                hfg_ZBiasSglTrkPt0_12[iy][ich][i] = (TH1F*)f->Get(hname);

                sprintf(hname, "hfg_TrkTrgPt12_20_raw%d_%d_%d", iy, ich, i);
                hfg_TrkTrgPt12_20_raw[iy][ich][i] = (TH1F*)f->Get(hname);
                sprintf(hname, "hfg_TrkTrgPt12_20%d_%d_%d", iy, ich, i);
                hfg_TrkTrgPt12_20[iy][ich][i] = (TH1F*)f->Get(hname);

                sprintf(hname, "hfg_TrkTrgPt20_30_raw%d_%d_%d", iy, ich, i);
                hfg_TrkTrgPt20_30_raw[iy][ich][i] = (TH1F*)f->Get(hname);
                sprintf(hname, "hfg_TrkTrgPt20_30%d_%d_%d", iy, ich, i);
                hfg_TrkTrgPt20_30[iy][ich][i] = (TH1F*)f->Get(hname);

                sprintf(hname, "hfg_TrkTrgPt30above_raw%d_%d_%d", iy, ich, i);
                hfg_TrkTrgPt30above_raw[iy][ich][i] = (TH1F*)f->Get(hname);
                sprintf(hname, "hfg_TrkTrgPt30above%d_%d_%d", iy, ich, i);
                hfg_TrkTrgPt30above[iy][ich][i] = (TH1F*)f->Get(hname);

            }

    //
    for(short iy = 0; iy<NY; iy++) 
        for(short i = 0; i<NPT; i++) {

            sprintf(hname, "hfgdiff_ZBiasSglTrkPt0_12_raw%d_%d", iy, i);
            hfgdiff_ZBiasSglTrkPt0_12_raw[iy][i] = (TH1F*)f->Get(hname);
            sprintf(hname, "hfgdiff_ZBiasSglTrkPt0_12%d_%d", iy, i);
            hfgdiff_ZBiasSglTrkPt0_12[iy][i] = (TH1F*)f->Get(hname);

            sprintf(hname, "hfgdiff_TrkTrgPt12_20_raw%d_%d", iy, i);
            hfgdiff_TrkTrgPt12_20_raw[iy][i] = (TH1F*)f->Get(hname);
            sprintf(hname, "hfgdiff_TrkTrgPt12_20%d_%d", iy, i);
            hfgdiff_TrkTrgPt12_20[iy][i] = (TH1F*)f->Get(hname);

            sprintf(hname, "hfgdiff_TrkTrgPt20_30_raw%d_%d", iy, i);
            hfgdiff_TrkTrgPt20_30_raw[iy][i] = (TH1F*)f->Get(hname);
            sprintf(hname, "hfgdiff_TrkTrgPt20_30%d_%d", iy, i);
            hfgdiff_TrkTrgPt20_30[iy][i] = (TH1F*)f->Get(hname);

            sprintf(hname, "hfgdiff_TrkTrgPt30above_raw%d_%d", iy, i);
            hfgdiff_TrkTrgPt30above_raw[iy][i] = (TH1F*)f->Get(hname);
            sprintf(hname, "hfgdiff_TrkTrgPt30above%d_%d", iy, i);
            hfgdiff_TrkTrgPt30above[iy][i] = (TH1F*)f->Get(hname);


            sprintf(hname, "hbgdiff_ZBiasSglTrkPt0_12_raw%d_%d", iy, i);
            hbgdiff_ZBiasSglTrkPt0_12_raw[iy][i] = (TH1F*)f->Get(hname);
            sprintf(hname, "hbgdiff_ZBiasSglTrkPt0_12%d_%d", iy, i);
            hbgdiff_ZBiasSglTrkPt0_12[iy][i] = (TH1F*)f->Get(hname);

            sprintf(hname, "hbgdiff_TrkTrgPt12_20_raw%d_%d", iy, i);
            hbgdiff_TrkTrgPt12_20_raw[iy][i] = (TH1F*)f->Get(hname);
            sprintf(hname, "hbgdiff_TrkTrgPt12_20%d_%d", iy, i);
            hbgdiff_TrkTrgPt12_20[iy][i] = (TH1F*)f->Get(hname);

            sprintf(hname, "hbgdiff_TrkTrgPt20_30_raw%d_%d", iy, i);
            hbgdiff_TrkTrgPt20_30_raw[iy][i] = (TH1F*)f->Get(hname);
            sprintf(hname, "hbgdiff_TrkTrgPt20_30%d_%d", iy, i);
            hbgdiff_TrkTrgPt20_30[iy][i] = (TH1F*)f->Get(hname);

            sprintf(hname, "hbgdiff_TrkTrgPt30above_raw%d_%d", iy, i);
            hbgdiff_TrkTrgPt30above_raw[iy][i] = (TH1F*)f->Get(hname);
            sprintf(hname, "hbgdiff_TrkTrgPt30above%d_%d", iy, i);
            hbgdiff_TrkTrgPt30above[iy][i] = (TH1F*)f->Get(hname);
        }
}
