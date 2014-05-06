//.. to check if the rerun HLT is synced with the orignal HLT in the hiforest
#include <fstream>
#include <iostream>
#include <vector>

#include "TH1F.h"
#include "TFile.h"
#include "TSystem.h"
#include "TTree.h"
#include "TNtuple.h"

#include "DataFormats/HLTReco/interface/TriggerObject.h"

typedef std::vector<trigger::TriggerObject> trigO;

using namespace std;

void CompareHLT(int startFile, int endFile, char *filelist) 
{
    char* filename = new char[1000];
    sprintf(filename, "ana_hiforest_%s_%dTo%d.root",  filelist, startFile, endFile);
    TFile result(filename, "recreate");

    //
    TNtuple* nt = new TNtuple("nt", "", "runhrst:evthrst:run:evt");
    TNtuple* ntObj = new TNtuple("ntObj", "", "pt0:eta0:phi0:pt1:eta1:phi1");

    Int_t run, evt;

    trigO* trgObjOrg = new trigO;
    trigO* trgObjNew = new trigO;

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


        TFile *f = TFile::Open(filename);
        if(!f) {
            cout<<"==> empty file <=="<<endl;
            continue;
        }
        TTree *HltTree  = (TTree*)f->Get("hltanalysis/HltTree");
        TTree *hltTree = (TTree*)f->Get("hltReRun/hltTree");

        if(!HltTree || !hltTree) {
            cout<<"==> empty tree <=="<<endl;
            continue;
        }

        int runNum, evtNum;
        HltTree->SetBranchAddress("Run",&run);
        HltTree->SetBranchAddress("Event",&evt);
        hltTree->SetBranchAddress("Run",&runNum);
        hltTree->SetBranchAddress("Event",&evtNum);
        if(!get_trg_info(HltTree, trgObjOrg)) {
            cout<<" Org trgObj error !"<<endl;
            exit(0);
        }
        if(!get_trg_info(hltTree, trgObjNew)) {
            cout<<" New trgObj error !"<<endl;
            exit(0);
        }

        HltTree->AddFriend(hltTree);

        Long64_t nentries = HltTree->GetEntries();
        for (Long64_t i=0;i<nentries;i++) {
            HltTree->GetEntry(i);

            if (runNum != run || evtNum != evt) {
                cout<<" run: "<<run<<" runNum: "<<runNum<<" evt: "<<evt<<" evtNum: "<<evtNum<<endl;
            } 

            nt->Fill(run, evt, runNum, evtNum); 

            cout<<"2__"<<endl;
            int nOrg = trgObjOrg->size();
            cout<<"3__"<<endl;
            int nNew = trgObjNew->size();
            cout<<"4__"<<endl;

            if(nOrg!=nNew) {
                cout<<"!! number of trg primitive between Org and New are different.Skip !!"<<endl;
                continue;
            }

            for(int it = 0; it<nOrg; it++) {
                ntObj->Fill(trgObjOrg.at(i).pt(), trgObjOrg.at(i).eta(), trgObjOrg.at(i).phi(), trgObjNew.at(i).pt(), trgObjNew.at(i).eta(), trgObjNew.at(i).phi());
            }

        }

        f->Close();
    }

    result.cd();
    nt->Write();
    ntObj->Write();
    result.Close();
}

bool get_trg_info(TTree* T, trigO* trg_obj)
{
    const char* trg_name = "HLT_PAJet100_NoJetID";   //.. base: L1_SingleJet36

    TObjArray *branches = (TObjArray *)T->GetListOfBranches();
    TIter branch_iter(branches);
    TBranch* branch = 0;
    while ((branch = (TBranch *)branch_iter.Next())) {
        TString branch_name = branch->GetName();
        if (branch_name.Contains(trg_name)){
            if(branch_name.Contains("trigObject")) {
                T->SetBranchAddress(branch_name.Data(), &trg_obj);
                cout<<branch_name.Data()<<endl;
                return true;
            } 
        }
    }

    return false;
}

