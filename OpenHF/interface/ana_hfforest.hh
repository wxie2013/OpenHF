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
        const char* trg_name[NTRG]; //.. trigger name
        trigO *trg_obj[NTRG]; //.. object to store trigger primitive. 

        char* filename;

        TFile* result;  //.. result output file name
        hfcand_v0* hfcandidate; //.. heavy flavor candidate for each event

    private:  //.. forest tree used in the analyses
        //TTree *HiTree;  //.. HiEvent tree
        TTree *HltTree;  //.. HLT tree
        TTree *hltTreeReRun;  //.. HLT tree rerun adding Photon and Track trigger primitive
        TTree *hftree;   //.. heavy flavor tree

    private: //.. mass distributions for all triggers
        // histogram for each individual trigger for D
        TH1* hfg_raw[NTRG][NPT]; // raw spectram w/o correcting prescale
        TH1* hfg[NTRG][NPT];   //.. spectra after correcting prescale.

        // histogram for each individual trigger for D*
        TH1* hfgdiff_raw[NTRG][NPT];
        TH1* hfgdiff[NTRG][NPT];

        TH1* hbgdiff_raw[NTRG][NPT];
        TH1* hbgdiff[NTRG][NPT];

    private: //.. mass distributions for FullTrack trigger
        //histo for trg combination. for all D
        TH1* hfg_ZBiasSglTrkPt0_12_raw[NPT];  //.. 0-12GeV/c before correcting prescale
        TH1* hfg_ZBiasSglTrkPt0_12[NPT];  //.. 0-12GeV/c after correcting prescale

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
        //
    private: //.. mass distributions for Jet trigger
        //histo for trg combination. for all D
        TH1* hfg_ZBiasSglTrkPt0_20_raw[NPT];  
        TH1* hfg_ZBiasSglTrkPt0_20[NPT]; 

        TH1* hfg_JetTrgPt20_40_raw[NPT]; 
        TH1* hfg_JetTrgPt20_40[NPT]; 

        TH1* hfg_JetTrgPt40_60_raw[NPT];  
        TH1* hfg_JetTrgPt40_60[NPT];  

        TH1* hfg_JetTrgPt60_80_raw[NPT];  
        TH1* hfg_JetTrgPt60_80[NPT];  

        TH1* hfg_JetTrgPt80_100_raw[NPT];  
        TH1* hfg_JetTrgPt80_100[NPT];  

        TH1* hfg_JetTrgPt100above_raw[NPT];  
        TH1* hfg_JetTrgPt100above[NPT];  

        //.. histo for trg combination for D*
        TH1* hfgdiff_ZBiasSglTrkPt0_20_raw[NPT]; 
        TH1* hfgdiff_ZBiasSglTrkPt0_20[NPT];

        TH1* hfgdiff_JetTrgPt20_40_raw[NPT];
        TH1* hfgdiff_JetTrgPt20_40[NPT];

        TH1* hfgdiff_JetTrgPt40_60_raw[NPT];  
        TH1* hfgdiff_JetTrgPt40_60[NPT];  

        TH1* hfgdiff_JetTrgPt60_80_raw[NPT];  
        TH1* hfgdiff_JetTrgPt60_80[NPT];  

        TH1* hfgdiff_JetTrgPt80_100_raw[NPT];  
        TH1* hfgdiff_JetTrgPt80_100[NPT];  

        TH1* hfgdiff_JetTrgPt100above_raw[NPT];  
        TH1* hfgdiff_JetTrgPt100above[NPT];  

        TH1* hbgdiff_ZBiasSglTrkPt0_20_raw[NPT];
        TH1* hbgdiff_ZBiasSglTrkPt0_20[NPT];

        TH1* hbgdiff_JetTrgPt20_40_raw[NPT];
        TH1* hbgdiff_JetTrgPt20_40[NPT]; 

        TH1* hbgdiff_JetTrgPt40_60_raw[NPT];  
        TH1* hbgdiff_JetTrgPt40_60[NPT];  

        TH1* hbgdiff_JetTrgPt60_80_raw[NPT];  
        TH1* hbgdiff_JetTrgPt60_80[NPT];  

        TH1* hbgdiff_JetTrgPt80_100_raw[NPT];  
        TH1* hbgdiff_JetTrgPt80_100[NPT];  

        TH1* hbgdiff_JetTrgPt100above_raw[NPT];  
        TH1* hbgdiff_JetTrgPt100above[NPT];  
        //
    private: //.. mass distributions for Photon trigger
        //histo for trg combination. for all D
        TH1* hfg_ZBiasSglTrkPt0_10_raw[NPT];  
        TH1* hfg_ZBiasSglTrkPt0_10[NPT]; 

        TH1* hfg_PhoTrgPt10_15_raw[NPT]; 
        TH1* hfg_PhoTrgPt10_15[NPT]; 

        TH1* hfg_PhoTrgPt15_20_raw[NPT];  
        TH1* hfg_PhoTrgPt15_20[NPT];  

        TH1* hfg_PhoTrgPt20_30_raw[NPT];  
        TH1* hfg_PhoTrgPt20_30[NPT];  

        TH1* hfg_PhoTrgPt30above_raw[NPT];  
        TH1* hfg_PhoTrgPt30above[NPT];  

        //.. histo for trg combination for D*
        TH1* hfgdiff_ZBiasSglTrkPt0_10_raw[NPT]; 
        TH1* hfgdiff_ZBiasSglTrkPt0_10[NPT];

        TH1* hfgdiff_PhoTrgPt10_15_raw[NPT];
        TH1* hfgdiff_PhoTrgPt10_15[NPT];

        TH1* hfgdiff_PhoTrgPt15_20_raw[NPT];  
        TH1* hfgdiff_PhoTrgPt15_20[NPT];  

        TH1* hfgdiff_PhoTrgPt20_30_raw[NPT];  
        TH1* hfgdiff_PhoTrgPt20_30[NPT];  

        TH1* hfgdiff_PhoTrgPt30above_raw[NPT];  
        TH1* hfgdiff_PhoTrgPt30above[NPT];  

        TH1* hbgdiff_ZBiasSglTrkPt0_10_raw[NPT];
        TH1* hbgdiff_ZBiasSglTrkPt0_10[NPT];

        TH1* hbgdiff_PhoTrgPt10_15_raw[NPT];
        TH1* hbgdiff_PhoTrgPt10_15[NPT]; 

        TH1* hbgdiff_PhoTrgPt15_20_raw[NPT];  
        TH1* hbgdiff_PhoTrgPt15_20[NPT];  

        TH1* hbgdiff_PhoTrgPt20_30_raw[NPT];  
        TH1* hbgdiff_PhoTrgPt20_30[NPT];  

        TH1* hbgdiff_PhoTrgPt30above_raw[NPT];  
        TH1* hbgdiff_PhoTrgPt30above[NPT];  
        //

    private: //.. various cuts used to produce results. 
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
        void book_hist(int ich, int iy);
        bool checkRapidityBoundary(float y);
        void get_hist(TFile* f);
        void get_trg_name();
        void get_trg_info(TTree* T1, TTree* T2);
        int  get_pt_bin_num(float pt);
        void get_pt_range(int i, float& pt_low, float& pt_high);
        void reset_trg();
        void Init(int startFile, int endFile, char *filelist, int iy, int ich, char* outfile);
        void write();
        bool GetRapidity(float m, float pt, float eta, int iY);
        bool GetTreeInfo(TFile* f);
        void LoopOverEvt(TTree* T, int iy, int ich);
        void LoopOverHFCandidate(int iy, int ich);
        void FillMassHisto(snglhfcand* cand, int iy, int ich);
        void FillTrgCombineTrkTrg(int id, int ich, int ipt, int iy, float mass, float mass_dau);
        void FillTrgCombineJetTrg(int id, int ich, int ipt, int iy, float mass, float mass_dau);
        void FillTrgCombinePhoTrg(int id, int ich, int ipt, int iy, float mass, float mass_dau);
        void FindMaxTrgPt(trigO* tobj, float& max);

    public:
        ana_hfforest();
        virtual ~ana_hfforest();

        void LoopOverFile(int startFile, int endFile, char *filelist, int iy, int ich, char* outfile);

        ClassDef(ana_hfforest, 1)
};

#endif //ana_hfforest_H
