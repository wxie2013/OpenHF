#ifndef checkTrgMatch_H
#define checkTrgMatch_H

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <TFile.h>
#include <TNtuple.h>
#include <TNamed.h>
#include <TTree.h>
#include <TCanvas.h>

#include "hfcand_v0.hh"

#include "DataFormats/HLTReco/interface/TriggerObject.h"


#include "ana_common.hh"

using namespace std;

class checkTrgMatch : public TNamed 
{
    private:
        int prscl[NTRG]; //.. trigger prescale factors
        int trg[NTRG];  //.. trigger decisions. better bool but the hlttree used int
        const char* trg_name[NTRG]; //.. trigger name
        trigO *trg_obj[NTRG]; //.. object to store trigger primitive. 

        char* filename;

        TFile* result;  //.. result output file name

        float trkEta[nMax];
        float trkPhi[nMax];
        float trkPt[nMax];

        int nTrk;

    private:  //.. forest tree used in the analyses
        TTree *trackTree;  //.. single track tree
        TTree *HltTree;  //.. HLT tree

    private: //.. mass distributions 

        TNtuple* nt;

    private:
        void get_trg_name();
        void get_trg_info(TTree* t);
        void reset();
        void Init(int startFile, int endFile, char *filelist);
        void write();
        bool GetTreeInfo(TFile* f);
        void LoopOverEvt(TTree* T);
        void LoopOverTracks();

    public:
        checkTrgMatch();
        virtual ~checkTrgMatch();

        void LoopOverFile(int startFile, int endFile, char *filelist);

        ClassDef(checkTrgMatch, 1)
};

#endif //  checkTrgMatch_H
