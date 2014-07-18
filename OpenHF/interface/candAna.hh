#ifndef CANDANA_H
#define CANDANA_H

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <string>
#include <TFile.h>
#include <TNamed.h>
#include <TTree.h>

#include "hfcand_v0.hh"
#include "TAna01Event.hh"
#include "TAnaCand.hh"
#include "TAnaVertex.hh"
#include "TAnaTrack.hh"


using namespace std;

class candAna : public TNamed 
{
    private:
        TTree *fTree;
        TAna01Event *fpEvt;
        TAnaCand *fpCand;

        Long64_t fRun, fEvt;
        int fLS;

        int ncand; 

        float cut_ffls3d; 
        float cut_falpha0;

        TFile* result;

    private:
        void addCandidate(
                float fprob,
                float ff3d,
                float ff3de,
                float ffls3d,
                float fchi2,
                float falpha0,
                float fdr,
                int   type,
                float fpt,
                float fm,
                float feta,
                float fphi,
                float fmdau1,
                float fptdau1,
                float fetadau1,
                float fphidau1,
                float fptdau2,
                float fetadau2,
                float fphidau2,
                float fqdau2,
                float fq1,
                float fpt1,
                float feta1,
                float fphi1,
                float fpt2,
                float fq2,
                float feta2,
                float fphi2,
                int   nBPixLayer_dau2,
                int   nPixLayer_dau2,
                int   nBPixLayer1Hits_dau2,
                int   nTrackerLayer_dau2,
                int   nBPixLayer_1,
                int   nPixLayer_1,
                int   nBPixLayer1Hits_1,
                int   nTrackerLayer_1,
                int   nBPixLayer_2,
                int   nPixLayer_2,
                int   nBPixLayer1Hits_2,
                int   nTrackerLayer_2);


        void D0Analysis();
        void DstarAnalysis();
        void Dpm2KaPiPiAnalysis();
        void Ds2KstarKaonAnalysis();
        void Ds2PhiPiAnalysis();


        int numberOfBPixLayers(TAnaTrack *pTrack);
        int numberOfPixLayers(TAnaTrack *pTrack);
        int numberOfBPixLayer1Hits(TAnaTrack *pTrack);
        int numberOfTrackerLayers(TAnaTrack *pTrack);

        bool pass_cut(float ffls3d, float falpha0);

    public:
        candAna();
        virtual ~candAna();

        //.. reduce the file size ..
        void set_cut(float cut_ffls3d_in, float cut_falpha0_in) {
            cut_ffls3d = cut_ffls3d_in;
            cut_falpha0 = cut_falpha0_in;
        } 

        //..
        void Init(TTree* in);
        void LoopOverFile(int startFile, int endFile, char *filelist);
        void LoopOverEvt(TTree* T1);
        //.. sync production with hiforest ....
        void LoopOverEvt(TTree* T1, Long64_t& run_hifst, Long64_t& evt_hifst);

        //.. produce reduced tree directly
        hfcand* hfcandidate;
        void evtAnalysis(TAna01Event *evt);

        ClassDef(candAna, 1)
};

#endif //  CANDANA_H
