#ifndef FindUsefulTrigger_H
#define FindUsefulTrigger_H

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <TFile.h>
#include <TH1F.h>
#include <TNamed.h>
#include <TNtuple.h>
#include <TTree.h>
#include <TCanvas.h>

#include "hfcand_v0.hh"

//.. e.g. https://cmswbm.web.cern.ch/cmswbm/cmsdb/servlet/HLTSummary?RUN=211631&KEY=28422
const int ntrg = 181; //.. total number of pPb2013 HLT triggers and some useful L1 base trigger
const int npt = 20; //.. number of pT bin
const float delta_pt = 10; //.. 10 GeV per bin
const int nch = 5; //.. 5 D meson decay channels
const int nhist = 14;

using namespace std;

class FindUsefulTrigger : public TNamed 
{
    private:
        int prscl[ntrg]; //.. trigger prescale factors
        int trg[ntrg];  //.. trigger decisions. better bool but the hlttree used int
        const char* trg_name[ntrg]; //.. trigger name

        char* filename;

        TFile* result;  //.. result output file name
        hfcand_v0* hfcandidate; //.. heavy flavor candidate for each event

        int n_HLT_OR;  //..number of events with n_HLT_OR fired..
        int n_FinalPath; //.. number of events with HLTriggerFinalPath fired

        int run;
        int evt; 
        int lumi;

    private:  //.. forest tree used in the analyses
        TTree *HiTree;  //.. HLT tree
        TTree *HltTree;  //.. HLT tree
        TTree *hftree;   //.. heavy flavor tree

    private: //.. mass distributions 
        TNtuple* nt_prscl;

        TH1* hfg_tot[nch][npt];  //.. foreground per channel per pT bin
        TH1* hfgdiff_tot[nch][npt];  //.. foreground mass diff per channel per pT bin
        TH1* hbgdiff_tot[nch][npt];  //.. background mass diff per channel per pT bin

        TH1* hfg[nch][ntrg][npt];  //.. same as above for also for each trigger 
        TH1* hfgdiff[nch][ntrg][npt];  //.. same as above for also for each trigger 
        TH1* hbgdiff[nch][ntrg][npt];  //.. same as above for also for each trigger 

        //.. special interesting triggers for D analysis
        TH1* HLT_tot[nch][npt];
        TH1* h[nhist][nch][npt];

    private: //.. different cuts used to produce results. 
        float cut_m_low[nch]; //.. low end of the mass hiso.
        float cut_m_high[nch]; //.. high end of the mass hiso
        float cut_m_dau_low[nch]; //.. low end of the daughter mass cut
        float cut_m_dau_high[nch]; //.. high end of the daughter mass cut
        float cut_ffls3d[nch]; 
        float cut_falpha0[nch]; 
        float cut_fprob[nch];
        float cut_fdr[nch]; 
        float cut_fchi2[nch];

    private:
        void define_cuts();
        void book_hist();
        void get_hist(TFile* f, int ich);
        void get_trg_name();
        void get_trg_info(TTree* t);
        void reset_trg();
        void Init(int startFile, int endFile, char *filelist);
        void write();
        void LoopOverEvt(TTree* T);
        void LoopOverHFCandidate(TTree* T);
        void FillMassHisto(snglhfcand* cand);
        void FillSpecialInterestTrigger(int ich, int ipt, float mass);

        void CheckTriggerCounts(); //.. check number of event fired for certain triggers
        bool HLT_OR();  //..OR all HLT trigger except HLTriggerFinalPath

        void fill_nt_prscl();

    public:
        FindUsefulTrigger();
        virtual ~FindUsefulTrigger();

        void LoopOverFile(int startFile, int endFile, char *filelist);
        void draw(char* filename, int ich, TCanvas* cfg, bool allTrg);

        ClassDef(FindUsefulTrigger, 1)
};

#endif //  FindUsefulTrigger_H
