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
        const char* trg_name[NTRG]; //.. trigger name
        trigO *trg_obj[NTRG]; //.. object to store trigger primitive. 

        char* filename;

        TFile* result;  //.. result output file name
        hfcand_v0* hfcandidate; //.. heavy flavor candidate for each event

    private:  //.. forest tree used in the analyses
        //TTree *HiTree;  //.. HiEvent tree
        TTree *HltTree;  //.. HLT tree
        TTree *hftree;   //.. heavy flavor tree

    private: //.. mass distributions 
        //.. w/o trigger match
        TH1* hfg[NY][NCH][NTRG][NPT];  //.. same as above for also for each trigger 

        TH1* hfgdiff[NY][NTRG][NPT]; //.. only for D*
        TH1* hbgdiff[NY][NTRG][NPT];  //.. only for D*

        //histo for trg combination. for all D
        TH1* hfg_ZBiasSglTrkPt0_12_raw[NY][NCH][NPT];  //.. 0-12GeV/c before correcting prescale
        TH1* hfg_ZBiasSglTrkPt0_12[NY][NCH][NPT];  //.. 0-12GeV/c after correcting prescale

        TH1* hfg_TrkTrgPt12_20_raw[NY][NCH][NPT]; //.. 12-20 GeV/c before correcting prescale 
        TH1* hfg_TrkTrgPt12_20[NY][NCH][NPT];  //.. 12-20 GeV/c after correcting prescale

        TH1* hfg_TrkTrgPt20_30_raw[NY][NCH][NPT];  
        TH1* hfg_TrkTrgPt20_30[NY][NCH][NPT];  

        TH1* hfg_TrkTrgPt30above_raw[NY][NCH][NPT];  
        TH1* hfg_TrkTrgPt30above[NY][NCH][NPT];  

        //.. histo for trg combination for D*
        TH1* hfgdiff_ZBiasSglTrkPt0_12_raw[NY][NPT];  //.. 0-12GeV/c before correcting prescale
        TH1* hfgdiff_ZBiasSglTrkPt0_12[NY][NPT];  //.. 0-12GeV/c after correcting prescale

        TH1* hfgdiff_TrkTrgPt12_20_raw[NY][NPT]; //.. 12-20 GeV/c before correcting prescale 
        TH1* hfgdiff_TrkTrgPt12_20[NY][NPT];  //.. 12-20 GeV/c after correcting prescale

        TH1* hfgdiff_TrkTrgPt20_30_raw[NY][NPT];  
        TH1* hfgdiff_TrkTrgPt20_30[NY][NPT];  

        TH1* hfgdiff_TrkTrgPt30above_raw[NY][NPT];  
        TH1* hfgdiff_TrkTrgPt30above[NY][NPT];  

        //
        TH1* hbgdiff_ZBiasSglTrkPt0_12_raw[NY][NPT];  //.. 0-12GeV/c before correcting prescale
        TH1* hbgdiff_ZBiasSglTrkPt0_12[NY][NPT];  //.. 0-12GeV/c after correcting prescale

        TH1* hbgdiff_TrkTrgPt12_20_raw[NY][NPT]; //.. 12-20 GeV/c before correcting prescale 
        TH1* hbgdiff_TrkTrgPt12_20[NY][NPT];  //.. 12-20 GeV/c after correcting prescale

        TH1* hbgdiff_TrkTrgPt20_30_raw[NY][NPT];  
        TH1* hbgdiff_TrkTrgPt20_30[NY][NPT];  

        TH1* hbgdiff_TrkTrgPt30above_raw[NY][NPT];  
        TH1* hbgdiff_TrkTrgPt30above[NY][NPT];  
        //

        TH1* hMatchWin[NPT]; //.. decide trigger matching window
        TH1* hRecoTrgPtDiff[NPT]; //.. (recoPt - trgPt)/recoPt

    private: //.. different cuts used to produce results. 
        float cut_m_low[NCH]; //.. low end of the mass hiso.
        float cut_m_high[NCH]; //.. high end of the mass hiso
        float cut_m_dau_low[NCH]; //.. low end of the daughter mass cut
        float cut_m_dau_high[NCH]; //.. high end of the daughter mass cut
        float cut_ffls3d[NCH]; 
        float cut_falpha0[NCH]; 
        float cut_fprob[NCH];
        float cut_fdr[NCH]; 
        float cut_fchi2[NCH];

    private:
        void define_cuts();
        void book_hist();
        void get_hist(TFile* f, int ich);
        void get_trg_name();
        void get_trg_info(TTree* t);
        int  get_pt_bin_num(float pt);
        void get_pt_range(int i, float& pt_low, float& pt_high);
        void reset_trg();
        void Init(int startFile, int endFile, char *filelist);
        void write();
        bool GetRapidity(float m, float pt, float eta, int& iY);
        bool GetTreeInfo(TFile* f);
        void LoopOverEvt(TTree* T);
        void LoopOverHFCandidate();
        void FillMassHisto(snglhfcand* cand);
        bool matchTrgObj(int ipt, float pt, int tid, float eta, float phi, float& trgPt);
        void FillTrgCombine(int id, float eta0, float phi0, float eta1,
                float phi1, float eta2, float phi2, int ich, int ipt, 
                int iy, float mass, float mass_dau, float pt);

    public:
        ana_hfforest();
        virtual ~ana_hfforest();

        void LoopOverFile(int startFile, int endFile, char *filelist);

        ClassDef(ana_hfforest, 1)
};

#endif //  ana_hfforest_H
