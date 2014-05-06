#include <fstream>
#include <iostream>

#include "TH1F.h"
#include "TFile.h"
#include "TSystem.h"
#include "TTree.h"
#include "TNtuple.h"
#include "TBranch.h"

#include "../interface/hfcand_v0.hh"
#include "../interface/checkTrgMatch.hh"


ClassImp(checkTrgMatch)

    //
checkTrgMatch::checkTrgMatch()
{
}

//
checkTrgMatch::~checkTrgMatch()
{
    delete filename;
    delete result;

    delete nt;
}

//
void checkTrgMatch::Init(int startFile, int endFile, char *filelist)
{//.. called right after constructor 
    filename = new char[1000];
    sprintf(filename, "checkTrgMatch_%s_%dTo%d.root",  filelist, startFile, endFile);
    result = new TFile(filename, "recreate");

    //
    get_trg_name();

    nt = new TNtuple("nt", "", "trkEta:trkPhi:trkPt:trgEta:trgPhi:trgPt:min_dist:tid");

    trackTree = 0;
    HltTree = 0;

    //.. trigger object ...
    for(int i = 0; i<NTRG; i++) {
        trg_obj[i] = new trigO;
    }
}

//
void checkTrgMatch::LoopOverFile(int startFile, int endFile, char *filelist) 
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
bool checkTrgMatch::GetTreeInfo(TFile* f)
{
    HltTree  = (TTree*)f->Get("hltanalysis/HltTree");
    trackTree  = (TTree*)f->Get("ppTrack/trackTree");

    if(!trackTree || !HltTree) {
        cout<<"==> missing trackTree || HltTree <=="<<endl;
        return false;
    }

    HltTree->AddFriend(trackTree);

    trackTree->SetBranchAddress("nTrk", &nTrk);
    trackTree->SetBranchAddress("trkEta", trkEta);
    trackTree->SetBranchAddress("trkPhi", trkPhi);
    trackTree->SetBranchAddress("trkPt", trkPt);

    get_trg_info(HltTree);

    return true;
}

//
void checkTrgMatch::write()
{//.. results 
    result->cd();

    nt->Write();

    result->Close();
}

//
void checkTrgMatch::get_trg_info(TTree* T) 
{
    reset();

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
void checkTrgMatch::reset()
{//.. reset trigger decision every event
    for(int i = 0; i<NTRG; i++) {
        prscl[i] = 0;
        trg[i] = 0;
        trg_obj[i]->clear();
    }

    nTrk = 0;
}

//
void checkTrgMatch::LoopOverEvt(TTree* T)
{//.. loop over each event. Add different analysis function here. 

    for(int i = 0; i<T->GetEntries(); i++) {
        reset();
        T->GetEntry(i);

        LoopOverTracks();
    }
}


//
void checkTrgMatch::LoopOverTracks()
{//.. loop over Heavy Flavor candidate in an event 

    for(int tid = 0; tid<NTRG; tid++) {
        if(trg[tid]) {
            for(unsigned int i = 0; i<trg_obj[tid]->size(); i++) {
                float min = 999;
                int io = -1;

                //.. find the min distance 
                for(int itrk = 0; itrk<nTrk; itrk++) {
                    float dist = sqrt(pow((trkEta[itrk] - trg_obj[tid]->at(i).eta()),2) + pow((trkPhi[itrk] - trg_obj[tid]->at(i).phi()), 2));

                    if(dist < min) {
                        min = dist;
                        io = itrk;
                    }
                }

                nt->Fill(trkEta[io], trkPhi[io], trkPt[io], 
                        trg_obj[tid]->at(i).eta(),
                        trg_obj[tid]->at(i).phi(),
                        trg_obj[tid]->at(i).pt(),
                        min, tid);

            }

            break;
        }
    }
}


//
void checkTrgMatch::get_trg_name()
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

    trg_name[5]= "HLT_PAJet20_NoJetID";   //.. base: L1_SingleJet16_BptxAND
    trg_name[6]= "HLT_PAJet40_NoJetID";   //.. base: L1_SingleJet16_BptxAND
    trg_name[7]= "HLT_PAJet60_NoJetID";   //.. base: L1_SingleJet36
    trg_name[8]= "HLT_PAJet80_NoJetID";   //.. base: L1_SingleJet36
    trg_name[9]= "HLT_PAJet100_NoJetID";   //.. base: L1_SingleJet36

    trg_name[10]= "HLT_PAPhoton10_NoCaloIdVL";   //.. base: L1_SingleEG5_BptxAND
    trg_name[11]= "HLT_PAPhoton15_NoCaloIdVL";   //.. base: L1_SingleEG5_BptxAND
    trg_name[12]= "HLT_PAPhoton20_NoCaloIdVL";   //.. base: L1_SingleEG5_BptxAND
    trg_name[13]= "HLT_PAPhoton30_NoCaloIdVL";   //.. base: L1_SingleEG12

    /*
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
