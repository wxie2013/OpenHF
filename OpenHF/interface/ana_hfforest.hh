////////////////////////////////////////////////////////////////////////////////////////
// To avoid memory issues and coding complications, this code analyze D meson for only 
//  - one species 
//  - one Y bin
// The code thus need these two inputs
/////////////////////////////////////////////////////////////////////////////////////////  
//
#ifndef ana_hfforest_H
#define ana_hfforest_H

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <TFile.h>
#include <TH1F.h>
#include <TNamed.h>
#include <TTree.h>
#include <TCanvas.h>

#include "hfcand_v0.hh"

#include "DataFormats/HLTReco/interface/TriggerObject.h"

#include "ana_common.hh"

using namespace std;

class ana_hfforest : public TNamed 
{
    private:
        int prscl[NTRG]; //.. trigger prescale factors
        int trg[NTRG];  //.. trigger decisions. better bool but the hlttree used int
        trigO *trg_obj[NTRG]; //.. object to store trigger primitive from rerun trigger. 
        trigO *org_trg_obj[NTRG]; //.. object to store trigger primitive from the original tree. 

        char* filename;

        TFile* result;  //.. result output file name
        hfcand_v0* hfcandidate; //.. heavy flavor candidate for each event

    private:  //.. forest tree used in the analyses
        //TTree *HiTree;  //.. HiEvent tree
        TTree *HltTree;  //.. HLT tree
        TTree *hltTreeReRun;  //.. HLT tree rerun adding Photon and Track trigger primitive
        TTree *hftree;   //.. heavy flavor tree
        TTree *trackTree;

    private:
        int   trkCharge[nTrkMax];
        float trkPhi[nTrkMax];
        float trkEta[nTrkMax];
        float trkPt[nTrkMax];
        float trkPtError[nTrkMax];
        float trkDz1[nTrkMax];               // dZ to the first vertex
        float trkDzError1[nTrkMax];
        float trkDxy1[nTrkMax];              // d0 to the first vertex
        float trkDxyError1[nTrkMax];
        Bool_t highPurity[nTrkMax];

        int nTrk;

    private: //.. mass distributions for all triggers
        // histogram for each individual trigger for D
        TH1* hfg_raw[NTRG][NPT]; // raw spectram w/o correcting prescale
        TH1* hfg[NTRG][NPT];   //.. spectra after correcting prescale.

        // histogram for each individual trigger for D*
        TH1* hfgdiff_raw[NTRG][NPT];
        TH1* hfgdiff[NTRG][NPT];

        TH1* hbgdiff_raw[NTRG][NPT];
        TH1* hbgdiff[NTRG][NPT];

    private: //.. mass distributions for combined spec 
        // track trigger 
        TH1* hfg_raw_combTrkTrg[NptRebin]; // raw spectra for the comb to avoid large error from prescale
        TH1* hfg_combTrkTrg[NptRebin];   //.. regular method

        TH1* hfgdiff_raw_combTrkTrg[NptRebin];
        TH1* hfgdiff_combTrkTrg[NptRebin];

        TH1* hbgdiff_raw_combTrkTrg[NptRebin];
        TH1* hbgdiff_combTrkTrg[NptRebin];

        //.. all individual triggers (for checking trigger combination)
        TH1* hfg_rebin[NTRG][NptRebin]; // raw spectram w/o correcting prescale
        TH1* hfgdiff_rebin[NTRG][NptRebin];
        TH1* hbgdiff_rebin[NTRG][NptRebin];
        
        TH1* hfg_TrkTrgPt12_20_rebin[NptRebin]; 
        TH1* hfg_TrkTrgPt20_30_rebin[NptRebin];  
        TH1* hfg_TrkTrgPt30above_rebin[NptRebin];  

    private: //.. mass distributions for FullTrack trigger
        //histo for trg combination. for all D
        TH1* hfg_ZBiasSglTrkPt0_12_raw[NPT];  //.. 0-12GeV/c before correcting prescale
        TH1* hfg_ZBiasSglTrkPt0_12[NPT];  //.. 0-12GeV/c after correcting prescale

        //
        TH1* hfg_TrkTrgPt12_20_raw[NPT]; //.. 12-20 GeV/c before correcting prescale 
        TH1* hfg_TrkTrgPt12_20[NPT];  //.. 12-20 GeV/c after correcting prescale

        TH1* hfg_TrkTrgPt20_30_raw[NPT];  
        TH1* hfg_TrkTrgPt20_30[NPT];  

        TH1* hfg_TrkTrgPt30above_raw[NPT];  
        TH1* hfg_TrkTrgPt30above[NPT];  

        //.. histo for trg combination for D*
        TH1* hfgdiff_ZBiasSglTrkPt0_12_raw[NPT];  //.. 0-12GeV/c before correcting prescale
        TH1* hfgdiff_ZBiasSglTrkPt0_12[NPT];  //.. 0-12GeV/c after correcting prescale

        TH1* hfgdiff_TrkTrgPt12_20_raw[NPT]; //.. 12-20 GeV/c before correcting prescale 
        TH1* hfgdiff_TrkTrgPt12_20[NPT];  //.. 12-20 GeV/c after correcting prescale

        TH1* hfgdiff_TrkTrgPt20_30_raw[NPT];  
        TH1* hfgdiff_TrkTrgPt20_30[NPT];  

        TH1* hfgdiff_TrkTrgPt30above_raw[NPT];  
        TH1* hfgdiff_TrkTrgPt30above[NPT];  

        TH1* hbgdiff_ZBiasSglTrkPt0_12_raw[NPT];  //.. 0-12GeV/c before correcting prescale
        TH1* hbgdiff_ZBiasSglTrkPt0_12[NPT];  //.. 0-12GeV/c after correcting prescale

        TH1* hbgdiff_TrkTrgPt12_20_raw[NPT]; //.. 12-20 GeV/c before correcting prescale 
        TH1* hbgdiff_TrkTrgPt12_20[NPT];  //.. 12-20 GeV/c after correcting prescale

        TH1* hbgdiff_TrkTrgPt20_30_raw[NPT];  
        TH1* hbgdiff_TrkTrgPt20_30[NPT];  

        TH1* hbgdiff_TrkTrgPt30above_raw[NPT];  
        TH1* hbgdiff_TrkTrgPt30above[NPT];  

    private: 
        //.. ppTrack for checking trigger combination
        //also for filling ntMaxTrgPtAlgo2 histogram to save space.
        TH1* hPtTrk[NFullTrkTrg];
        TH1* hPtTrk0_12;
        TH1* hPtTrk12_20;
        TH1* hPtTrk20_30;
        TH1* hPtTrk30Above;
        TH1* hPtTrkComb;

    private:
        //.. need for cuts to determine maxTrkTrgPt 
        bool is_hp;
        float tDz1;
        float tDz1Err;
        float tDxy1;
        float tDxyErr;
        float tPtErr;

        //.. for each HF candidate daughter
        bool is_hp_0;
        bool is_hp_1;
        bool is_hp_2;

        float tPt0;
        float tPt1;
        float tPt2;

        float tPtErr0;
        float tPtErr1;
        float tPtErr2;


        // trigger prescale weight for each events 
        float maxTrkTrgPt;

        //.. maxPt spectra for triggers and components for debugging
        TNtuple* ntMaxTrgPt;
        TNtuple* ntMaxTrgPtAlgo2; //.. for trgCombAlgo = 2

        //.. mass ntuple to speed up the analysis
        TNtuple* ntMass;
       //.. ppTrack for cross checking with HIN-12-017
        bool run_ppTrack;
        TNtuple* ntTrk;

    private:
        void book_hist(int ich);
        bool checkRapidityBoundary(float y);
        void get_trg_info(TTree* T1, TTree* T2);
        int  get_pt_bin_num(float pt);
        void get_pt_range(int i, float& pt_low, float& pt_high);
        void reset_trg();
        void Init(int startFile, int endFile, char *filelist, int iy, int ich, char* outfile);
        void write();
        void write_hist(int ich);
        bool GetRapidity(float m, float pt, float eta, int iY);
        bool GetTreeInfo(TFile* f);
        void LoopOverEvt(TTree* T, int iy, int ich);
        void LoopOverTrk();
        void LoopOverHFCandidate(int iy, int ich);
        void FillMassHisto(snglhfcand* cand, int iy, int ich);
        void FillTrgMaxPt();
        void FillTrgCombineTrkTrg_HIN12017(int id, int ich, int ipt, float mass, float mass_dau);
        void define_combSpec(int ich);
        void define_rebinSpec(int ich);
        void drawDstar(TH1* h_fg, TH1* h_bg, int nrb, float& sig, float& err);
        void draw_fit(int ich, TH1* h, int nrb, float rlow, float rhigh, float& sig, float& err);
        void GetMaxTrgPt(); //for trigger combination 
        void find_hp_trkPtErr(float q_in, float pt_in, float eta_in, float phi_in, bool& is_hp, float& pterr);
        void fill_ntMass(float mass, float ptmax, int ipt, int itrg, bool isbg, float weight);
        void fill_ntTrk(float pt, float ptErr, float ptmax, float ptmaxErr, bool hp0, bool hp, int itrg, float weight);
        void book_ppTrk_hist();
    public:
        ana_hfforest();
        virtual ~ana_hfforest();

        void set_run_ppTrack(bool in) {run_ppTrack = in;}
        void LoopOverFile(int startFile, int endFile, char *filelist, int iy, int ich, char* outfile);
        void fillPPTrkHist(bool hp0_in, bool hp_in, float cut,  TFile* fin, TFile*fout);
        void fillntMaxTrgPtAlgo2Hist(bool hp_in, float cut,  TFile* fin, TFile*fout, bool dcaCut);
        void fill_ntMass_hist(int ich, float cut,  TFile* fin, TFile*fout, bool noRandFire);
        //
        void get_hist(int ich, TFile* fin);
        void get_comb_hist(int ich, TFile* fin);
        void rebinSpec(int ich, TFile* fout);
        void CombineSpec(int ich, TFile* fout);
        void drawOneHist(int ich, TH1* h, int nrb);
        void drawOneTrg(int ich, int Nrebin, bool chk, int it, TCanvas* cc);
        void drawHighMult(int ich, int whichTrg, int Nrebin, TFile* fin, TCanvas* cc);

        ClassDef(ana_hfforest, 1)
};

#endif //ana_hfforest_H
