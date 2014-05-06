#include <fstream>
#include <iostream>

#include "TH1F.h"
#include "TFile.h"
#include "TSystem.h"
#include "TTree.h"
#include "TNtuple.h"
#include "TBranch.h"

#include "../interface/hfcand_v0.hh"
#include "../interface/FindUsefulTrigger.hh"

const char* mesonName[nch] = {"Dstar2D0Pi", "D02KPi", "Ds2PhiPi", "Ds2KstarK", "Dpm2KPiPi"};

ClassImp(FindUsefulTrigger)

//
FindUsefulTrigger::FindUsefulTrigger()
{
    n_HLT_OR = 0;
    n_FinalPath = 0;
}

//
FindUsefulTrigger::~FindUsefulTrigger()
{
    delete filename;
    delete result;
    delete hfcandidate;
    delete nt_prscl;

    //.. all triggers 
    for(short ich = 0; ich<nch; ich++) {
        for(short i = 0; i<npt; i++) {
            delete hfg_tot[ich][i];
            delete hfgdiff_tot[ich][i];
            delete hbgdiff_tot[ich][i];

        }
    }

    //.. each triggers
    for(short ich = 0; ich<nch; ich++) {
        for(short it = 0; it<ntrg; it++) {
            for(short i = 0; i<npt; i++) {
                delete hfg[ich][it][i];
                delete hfgdiff[ich][it][i];
                delete hbgdiff[ich][it][i];

            }
        }
    }
}
//
void FindUsefulTrigger::Init(int startFile, int endFile, char *filelist)
{//.. called right after constructor 
    filename = new char[1000];
    sprintf(filename, "checkTrg_hhforest_%s_%dTo%d.root",  filelist, startFile, endFile);
    result = new TFile(filename, "recreate");

    //
    define_cuts();
    get_trg_name();
    book_hist();

    //
    hfcandidate = new hfcand_v0;

    HiTree = 0;
    HltTree = 0;
    hftree = 0;
}

//
void FindUsefulTrigger::book_hist()
{
    nt_prscl = new TNtuple("nt_prscl", "ntuple for prescale", "tid:prscl:lumi:run");
    char hname[100], pt_range[1000];
    //... all trigger 
    for(short ich = 0; ich<nch; ich++) {
        for(short i = 0; i<npt; i++) {
            float pt_low = i*delta_pt;
            float pt_high = pt_low +  delta_pt;

            sprintf(pt_range, "ch: %s,  pt: %f %f", mesonName[ich], pt_low, pt_high);

            sprintf(hname, "hfg_tot%d_%d", ich, i);
            hfg_tot[ich][i] = new TH1F(hname, pt_range, npt, cut_m_low[ich], cut_m_high[ich]);

            sprintf(hname, "hfgdiff_tot%d_%d", ich, i);
            hfgdiff_tot[ich][i] = new TH1F(hname, pt_range, npt, 0.135, 0.154);

            sprintf(hname, "hbgdiff_tot%d_%d", ich, i);
            hbgdiff_tot[ich][i] = new TH1F(hname, pt_range, npt, 0.135, 0.154);

            //.. track error ...
            hfg_tot[ich][i]->Sumw2();
            hfgdiff_tot[ich][i]->Sumw2();
            hbgdiff_tot[ich][i]->Sumw2();

            //.. specific interesting triggers
            sprintf(hname, "HLT_tot%d_%d", ich, i);
            HLT_tot[ich][i] = new TH1F(hname, pt_range, npt, cut_m_low[ich], cut_m_high[ich]);

            for(int j = 0; j<nhist; j++) {
                sprintf(hname, "h%d_%d_%d", j, ich, i);
                h[j][ich][i] = new TH1F(hname, pt_range, npt, cut_m_low[ich], cut_m_high[ich]);
            }
        }
    }

    //.. each trigger 
    for(short ich = 0; ich<nch; ich++) {
        for(short it = 0; it<ntrg; it++) {
            for(short i = 0; i<npt; i++) {
                float pt_low = i*delta_pt;
                float pt_high = pt_low +  delta_pt;

                sprintf(pt_range, "ch: %s, trg: %s,  pt: %f %f", mesonName[ich], trg_name[it], pt_low, pt_high);

                //
                sprintf(hname, "hfg%d_%d_%d", ich, it, i);
                hfg[ich][it][i] = new TH1F(hname, pt_range, npt, cut_m_low[ich], cut_m_high[ich]);

                sprintf(hname, "hfgdiff%d_%d_%d", ich, it, i);
                hfgdiff[ich][it][i] = new TH1F(hname, pt_range, npt, 0.135, 0.154);

                sprintf(hname, "hbgdiff%d_%d_%d", ich, it, i);
                hbgdiff[ich][it][i] = new TH1F(hname, pt_range, npt, 0.135, 0.154);

                //.. track error ...
                hfg[ich][it][i]->Sumw2();
                hfgdiff[ich][it][i]->Sumw2();
                hbgdiff[ich][it][i]->Sumw2();
            }
        }
    }
}

//
void FindUsefulTrigger::LoopOverFile(int startFile, int endFile, char *filelist) 
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
        HiTree  = (TTree*)f->Get("hiEvtAnalyzer/HiTree");
        HltTree  = (TTree*)f->Get("hltanalysis/HltTree");
        hftree = (TTree*)f->Get("OpenHfTree/hftree");

        if(!hftree || !HltTree) {
            cout<<"==> empty tree <=="<<endl;
            continue;
        }

        HltTree->AddFriend(hftree);
        HltTree->AddFriend(HiTree);

        HiTree->SetBranchAddress("run", &run);
        HiTree->SetBranchAddress("evt", &evt);
        HiTree->SetBranchAddress("lumi", &lumi);
        hftree->SetBranchAddress("hfcandidate", &hfcandidate);
        get_trg_info(HltTree);

        //
        LoopOverEvt(HltTree);
        cout<<"n_HLT_OR: "<<n_HLT_OR<<endl;
        cout<<"n_FinalPath: "<<n_FinalPath<<endl;

        //
        f->Close();
    }

    cout<<"--> Sum of n_HLT_OR: "<<n_HLT_OR<<endl;
    cout<<"--> Sum of n_FinalPath: "<<n_FinalPath<<endl;

    write();
}
//
void FindUsefulTrigger::write()
{//.. results 
    result->cd();

    //.. all triggers 
    for(short ich = 0; ich<nch; ich++) {
        for(short i = 0; i<npt; i++) {
            hfg_tot[ich][i]->Write();
            hfgdiff_tot[ich][i]->Write();
            hbgdiff_tot[ich][i]->Write();

            HLT_tot[ich][i]->Write();
            for(int j = 0; j<nhist; j++) {
                h[j][ich][i]->Write();
            }
        }
    }

    //.. each triggers
    for(short ich = 0; ich<nch; ich++) {
        for(short it = 0; it<ntrg; it++) {
            for(short i = 0; i<npt; i++) {
                hfg[ich][it][i]->Write();
                hfgdiff[ich][it][i]->Write();
                hbgdiff[ich][it][i]->Write();
            }
        }
    }

    nt_prscl->Write();

    result->Close();
}

//
void FindUsefulTrigger::get_trg_info(TTree* T) 
{
    reset_trg();

    TObjArray *branches = (TObjArray *)T->GetListOfBranches();
    TIter branch_iter(branches); 
    TBranch* branch = 0;
    while ((branch = (TBranch *)branch_iter.Next())) {  
        TString branch_name = branch->GetName();
        for(int it = 0; it<ntrg; it++) {
            if (branch_name.Contains(trg_name[it])){
                if(branch_name.Contains("Prescl")) {
                    T->SetBranchAddress(branch_name.Data(), &prscl[it]);
                } else if(branch_name.Contains("trigObject")) {
                    //.. do nothing ..
                } else {
                    T->SetBranchAddress(branch_name.Data(), &trg[it]);
                }

                break;
            } 
        }
    }
}

//
void FindUsefulTrigger::reset_trg()
{//.. reset trigger decision every event
    for(int i = 0; i<ntrg; i++) {
        prscl[i] = 0;
        trg[i] = 0;
    }
}

//
void FindUsefulTrigger::LoopOverEvt(TTree* T)
{//.. loop over each event. Add different analysis function here. 

    for(int i = 0; i<T->GetEntries(); i++) {
        reset_trg();
        T->GetEntry(i);

        fill_nt_prscl();
        LoopOverHFCandidate(T);
        CheckTriggerCounts();
    }
}
//
void FindUsefulTrigger::CheckTriggerCounts()
{//.. check if the sum of all HLT OR is equal to HLTriggerFinalPath

    if(HLT_OR()) 
        n_HLT_OR++;

    if(trg[167]) 
        n_FinalPath++;
}

//
void FindUsefulTrigger::LoopOverHFCandidate(TTree* T)
{//.. loop over Heavy Flavor candidate in a event 
    for(int i = 0; i<hfcandidate->get_nhfcand(); i++) {
        snglhfcand* cand = hfcandidate->get_hfcand(i);

        FillMassHisto(cand);
    }

    hfcandidate->Reset();
}

//
void FindUsefulTrigger::FillMassHisto(snglhfcand* cand)
{//.. fill the histogram of mass distribution ..
    for(int ich = 0; ich < nch; ich++) {

        if(cand->get_type() != ich+1) continue;

        float mass = cand->get_fm();
        float fpt = cand->get_fpt();
        float ffls3d = cand->get_ffls3d();
        float alpha = cand->get_falpha0();
        float fprob = cand->get_fprob();
        float fdr = cand->get_fdr();
        float fchi2 = cand->get_fchi2();
        float mass_dau = cand->get_fmdau1();

        //.. which pt bin
        int ipt = fpt/delta_pt;
        if(ipt >= npt)
            ipt = npt - 1;  //.. the last bin include all higher pT region...

        if(mass > cut_m_low[ich] && mass < cut_m_high[ich] && 
                ffls3d > cut_ffls3d[ich] && alpha < cut_falpha0[ich] && 
                fprob > cut_fprob[ich] && fdr < cut_fdr[ich] && fchi2 < cut_fchi2[ich]) {

            //.. forground ..
            if(ich==1) { //D0 has no daughter mass, treat separately. ...
                hfg_tot[ich][ipt]->Fill(mass);

                for(int itrg = 0; itrg<ntrg; itrg++) {
                    if(trg[itrg]) {
                        hfg[ich][itrg][ipt]->Fill(mass);
                    }
                }
                FillSpecialInterestTrigger(ich, ipt, mass);

            } else {
                if(mass_dau > cut_m_dau_low[ich] && mass_dau < cut_m_dau_high[ich]) {
                    hfg_tot[ich][ipt]->Fill(mass);
                    hfgdiff_tot[ich][ipt]->Fill(mass-mass_dau);

                    for(int itrg = 0; itrg<ntrg; itrg++) {
                        if(trg[itrg]) {
                            hfg[ich][itrg][ipt]->Fill(mass);
                            hfgdiff[ich][itrg][ipt]->Fill(mass-mass_dau);
                        }
                    }
                    FillSpecialInterestTrigger(ich, ipt, mass);
                } else { //.. background through side band ...
                    hbgdiff_tot[ich][ipt]->Fill(mass);
                    for(int itrg = 0; itrg<ntrg; itrg++) {
                        if(trg[itrg]) 
                            hbgdiff[ich][itrg][ipt]->Fill(mass-mass_dau);
                    }
                }
            }
        }
    }
}

//
void FindUsefulTrigger::FillSpecialInterestTrigger(int ich, int ipt, float mass)
{
    bool trg_OR[nhist] = {false};

    if(trg[55] ||  //"HLT_PAFullTrack12"
            trg[56] ||  //"HLT_PAFullTrack20"
            trg[57] ||  //"HLT_PAFullTrack30"
            trg[58])  //"HLT_PAFullTrack50"
        trg_OR[0]=true;

    if(trg[67] ||  //"HLT_PAJet100_NoJetID"
            trg[68] ||  //"HLT_PAJet120_NoJetID"
            trg[69] ||  //"HLT_PAJet20_NoJetID"
            trg[71] ||  //"HLT_PAJet40_NoJetID"
            trg[73] ||  //"HLT_PAJet60_NoJetID"
            trg[74]) //"HLT_PAJet80_NoJetID"
        trg_OR[1]=true;

    if(trg[108] ||    //"HLT_PAPhoton10_NoCaloIdVL"
            trg[114] ||    //"HLT_PAPhoton15_NoCaloIdVL"
            trg[119] ||    //"HLT_PAPhoton20_NoCaloIdVL"
            trg[125] ||    //"HLT_PAPhoton30_NoCaloIdVL"
            trg[129] ||    //"HLT_PAPhoton40_NoCaloIdVL"
            trg[131])    //"HLT_PAPhoton60_NoCaloIdVL"
        trg_OR[2]=true;



    if(trg[45] || // "HLT_PAForJet100Eta2"
            trg[46] || // "HLT_PAForJet100Eta3"
            trg[47] || // "HLT_PAForJet20Eta2"
            trg[48] || // "HLT_PAForJet20Eta3"
            trg[49] || // "HLT_PAForJet40Eta2"
            trg[50] || // "HLT_PAForJet40Eta3"
            trg[51] || // "HLT_PAForJet60Eta2"
            trg[52] || // "HLT_PAForJet60Eta3"
            trg[53] || // "HLT_PAForJet80Eta2"
            trg[54]) // "HLT_PAForJet80Eta3"
        trg_OR[3] = true;

    if(trg[8] || // "AlCa_PAEcalEtaEBonly"
            trg[10]|| // "AlCa_PAEcalPi0EBonly"
            trg[11]) // "AlCa_PAEcalPi0EEonly"
        trg_OR[4]=true;

    if(trg[153] || // "HLT_PATripleJet20_20_20"
            trg[154] || // "HLT_PATripleJet40_20_20"
            trg[155] || // "HLT_PATripleJet60_20_20"
            trg[156] || // "HLT_PATripleJet80_20_20"
            trg[152] ) // "HLT_PATripleJet100_20_20"
        trg_OR[5]=true;

    if(
            //trg[133] || // "HLT_PAPixelTrackMultiplicity100_L2DoubleMu3"
            //trg[135] || // "HLT_PAPixelTrackMultiplicity140_Jet80_NoJetID"
            trg[134] || // "HLT_PAPixelTrackMultiplicity130_FullTrack12"
            trg[132] || // "HLT_PAPixelTrackMultiplicity100_FullTrack12"
            trg[136] ) // "HLT_PAPixelTrackMultiplicity160_FullTrack12"
            //trg[141] || // "HLT_PAPixelTracks_Multiplicity220"
            //trg[138] || // "HLT_PAPixelTracks_Multiplicity130"
            //trg[139] || // "HLT_PAPixelTracks_Multiplicity160"
            //trg[140] || // "HLT_PAPixelTracks_Multiplicity190"
            //trg[137] ) // "HLT_PAPixelTracks_Multiplicity100"
        trg_OR[6]=true;

    if(
            trg[124] || // "HLT_PAPhoton20_TightCaloIdVL"
            //trg[120] || // "HLT_PAPhoton20_Photon15_NoCaloIdVL"
            trg[118] || // "HLT_PAPhoton15_TightCaloIdVL"
            //trg[115] || // "HLT_PAPhoton15_Photon10_NoCaloIdVL"
            trg[113]) // "HLT_PAPhoton10_TightCaloIdVL"
        //trg[109] ) // "HLT_PAPhoton10_Photon10_NoCaloIdVL"
        trg_OR[7] = true;

    if(trg[103] || // "HLT_PAMu3PFJet40"
            trg[102] || // "HLT_PAMu3PFJet20"
            trg[104] ) // "HLT_PAMu3"
        trg_OR[8] = true;

    if(trg[87] ) // "HLT_PAL1SingleJet36"
        trg_OR[9] = true;

    if(trg[72] || // "HLT_PAJet60ETM30"
            trg[70] ) //"HLT_PAJet40ETM30"
        trg_OR[10] = true;

    if(trg[18]) // "DST_Physics"
        trg_OR[11] = true;

    if(trg[60] || // "HLT_PAHFSumET100"
            trg[61] || // "HLT_PAHFSumET140"
            trg[62] ) // "HLT_PAHFSumET170"
        trg_OR[12] = true;

    //.. I plotted the mass distr for each channel per trigger and check which 
    //.. has some non-negligible counts. Then OR them to see if they acoount for all 
    //.. D signals. Then reduce those that overlap with others through trigger name and 
    //.. a few iterations of try and errors. The following are the useful triggers out 
    //.. of the 172 for this analysis. The some useful L1 base trigger is also included. 

    /*
       for(int j = 0; j<nhist; j++) {
       if(trg_OR[j])
       h[j][ich][ipt]->Fill(mass);
       }
     */

    if(trg_OR[0] || trg_OR[1])
        h[0][ich][ipt]->Fill(mass);
    if(trg_OR[0] || trg_OR[2])
        h[1][ich][ipt]->Fill(mass);
    if(trg_OR[1] || trg_OR[2])
        h[2][ich][ipt]->Fill(mass);
    if(trg_OR[0] || trg_OR[1] || trg_OR[2])
        h[3][ich][ipt]->Fill(mass);
    if(trg_OR[0] || trg_OR[1] || trg_OR[2] || trg_OR[3])
        h[4][ich][ipt]->Fill(mass);
    if(trg_OR[6])
        h[5][ich][ipt]->Fill(mass);
    if(trg_OR[0] || trg_OR[1] || trg_OR[2] || trg_OR[3] || trg_OR[6])
        h[6][ich][ipt]->Fill(mass);
    if(trg_OR[7])
        h[7][ich][ipt]->Fill(mass);
    if(trg_OR[0] || trg_OR[1] || trg_OR[2] || trg_OR[3] || trg_OR[6] || trg_OR[7])
        h[8][ich][ipt]->Fill(mass);
    if(trg_OR[12])
        h[9][ich][ipt]->Fill(mass);
    if(trg_OR[0] || trg_OR[1] || trg_OR[2] || trg_OR[3] || trg_OR[6] || trg_OR[7] || trg_OR[12])
        h[10][ich][ipt]->Fill(mass);
    if(trg_OR[0] || trg_OR[1] || trg_OR[2] || trg_OR[6] || trg_OR[7] || trg_OR[12])
        h[11][ich][ipt]->Fill(mass);
    if(trg_OR[0] || trg_OR[1] || trg_OR[2] || trg_OR[3] || trg_OR[6] || trg_OR[12])
        h[12][ich][ipt]->Fill(mass);
    if(trg_OR[0] || trg_OR[1] || trg_OR[2] || trg_OR[6] || trg_OR[12])
        h[13][ich][ipt]->Fill(mass);


    if(
            trg_OR[0] || 
            trg_OR[1] || 
            trg_OR[2] || 
            trg_OR[3] || 
            trg_OR[4] || 
            trg_OR[5] || 
            trg_OR[6] || 
            trg_OR[7] ||
            trg_OR[8] ||
            trg_OR[9] ||
            trg_OR[10] ||
            trg_OR[11] || 
            trg_OR[12])
        HLT_tot[ich][ipt]->Fill(mass);
}

//
bool FindUsefulTrigger::HLT_OR()
{
    for(int i = 0; i<ntrg; i++) {
        if(!strcmp(trg_name[i], "HLTriggerFinalPath")) {
            //..HLTriggerFinalPath fired everything...
            //cout<<" HLTriggerFinalPath rejected"<<endl;
            continue;
        }

        if(trg[i]) return true;
    }

    return false;
}

//
void FindUsefulTrigger::define_cuts()
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
void FindUsefulTrigger::get_trg_name()
{//.. this code is made by: ./make_code_to_read_hlt_name_pscl.txt.pl
 //.. all Run2013 pPb triggers. Most are useless. Here is to check which
 //.. one has most of the D mesons. 
 //.. trigger list is from, e.g. 
 //.. https://cmswbm.web.cern.ch/cmswbm/cmsdb/servlet/HLTSummary?RUN=211631&KEY=28422

    trg_name[0]= "ALCALUMIPIXELSOutput";
    trg_name[1]= "ALCAP0Output";
    trg_name[2]= "ALCAPHISYMOutput";
    trg_name[3]= "AOutput";
    trg_name[4]= "AlCa_EcalPhiSym";
    trg_name[5]= "AlCa_LumiPixels_Random";
    trg_name[6]= "AlCa_LumiPixels_ZeroBias";
    trg_name[7]= "AlCa_LumiPixels";
    trg_name[8]= "AlCa_PAEcalEtaEBonly";
    trg_name[9]= "AlCa_PAEcalEtaEEonly";
    trg_name[10]= "AlCa_PAEcalPi0EBonly";
    trg_name[11]= "AlCa_PAEcalPi0EEonly";
    trg_name[12]= "AlCa_RPCMuonNoHits";
    trg_name[13]= "AlCa_RPCMuonNoTriggers";
    trg_name[14]= "AlCa_RPCMuonNormalisation";
    trg_name[15]= "CalibrationOutput";
    trg_name[16]= "DQMOutput";
    trg_name[17]= "DQM_FEDIntegrity";
    trg_name[18]= "DST_Physics";
    trg_name[19]= "EcalCalibrationOutput";
    trg_name[20]= "ExpressOutput";
    trg_name[21]= "HLTDQMOutput";
    trg_name[22]= "HLT_Activity_Ecal_SC7";
    trg_name[23]= "HLT_BeamGas_HF_Beam1";
    trg_name[24]= "HLT_BeamGas_HF_Beam2";
    trg_name[25]= "HLT_BeamHalo";
    trg_name[26]= "HLT_DTCalibration";
    trg_name[27]= "HLT_EcalCalibration";
    trg_name[28]= "HLT_GlobalRunHPDNoise";
    trg_name[29]= "HLT_HcalCalibration";
    trg_name[30]= "HLT_L1SingleMuOpen_AntiBPTX";
    trg_name[31]= "HLT_L1TrackerCosmics";
    trg_name[32]= "HLT_LogMonitor";
    trg_name[33]= "HLT_PABTagMu_Jet20_Mu4";
    trg_name[34]= "HLT_PABptxMinusNotBptxPlus";
    trg_name[35]= "HLT_PABptxPlusNotBptxMinus";
    trg_name[36]= "HLT_PACastorEmNotHfCoincidencePm";
    trg_name[37]= "HLT_PACastorEmNotHfSingleChannel";
    trg_name[38]= "HLT_PACastorEmTotemLowMultiplicity";
    trg_name[39]= "HLT_PADimuon0_NoVertexing";
    trg_name[40]= "HLT_PADoubleEle6_CaloIdT_TrkIdVL";
    trg_name[41]= "HLT_PADoubleEle8_CaloIdT_TrkIdVL";
    trg_name[42]= "HLT_PADoubleJet20_ForwardBackward";
    trg_name[43]= "HLT_PADoubleMu4_Acoplanarity03";
    trg_name[44]= "HLT_PAExclDijet35_HFAND";
    trg_name[45]= "HLT_PAForJet100Eta2";
    trg_name[46]= "HLT_PAForJet100Eta3";
    trg_name[47]= "HLT_PAForJet20Eta2";
    trg_name[48]= "HLT_PAForJet20Eta3";
    trg_name[49]= "HLT_PAForJet40Eta2";
    trg_name[50]= "HLT_PAForJet40Eta3";
    trg_name[51]= "HLT_PAForJet60Eta2";
    trg_name[52]= "HLT_PAForJet60Eta3";
    trg_name[53]= "HLT_PAForJet80Eta2";
    trg_name[54]= "HLT_PAForJet80Eta3";
    trg_name[55]= "HLT_PAFullTrack12";
    trg_name[56]= "HLT_PAFullTrack20";
    trg_name[57]= "HLT_PAFullTrack30";
    trg_name[58]= "HLT_PAFullTrack50";
    trg_name[59]= "HLT_PAHFOR_SingleTrack";
    trg_name[60]= "HLT_PAHFSumET100";
    trg_name[61]= "HLT_PAHFSumET140";
    trg_name[62]= "HLT_PAHFSumET170";
    trg_name[63]= "HLT_PAHFSumET210";
    trg_name[64]= "HLT_PAHcalNZS";
    trg_name[65]= "HLT_PAHcalPhiSym";
    trg_name[66]= "HLT_PAHcalUTCA";
    trg_name[67]= "HLT_PAJet100_NoJetID";
    trg_name[68]= "HLT_PAJet120_NoJetID";
    trg_name[69]= "HLT_PAJet20_NoJetID";
    trg_name[70]= "HLT_PAJet40ETM30";
    trg_name[71]= "HLT_PAJet40_NoJetID";
    trg_name[72]= "HLT_PAJet60ETM30";
    trg_name[73]= "HLT_PAJet60_NoJetID";
    trg_name[74]= "HLT_PAJet80_NoJetID";
    trg_name[75]= "HLT_PAL1CastorTotalTotemLowMultiplicity";
    trg_name[76]= "HLT_PAL1DoubleEG3_FwdVeto";
    trg_name[77]= "HLT_PAL1DoubleEG5DoubleEle6_CaloIdT_TrkIdVL";
    trg_name[78]= "HLT_PAL1DoubleEG5_TotemDiffractive";
    trg_name[79]= "HLT_PAL1DoubleJet20_TotemDiffractive";
    trg_name[80]= "HLT_PAL1DoubleJetC36_TotemDiffractive";
    trg_name[81]= "HLT_PAL1DoubleMu0_HighQ";
    trg_name[82]= "HLT_PAL1DoubleMu0";
    trg_name[83]= "HLT_PAL1DoubleMu5_TotemDiffractive";
    trg_name[84]= "HLT_PAL1DoubleMuOpen";
    trg_name[85]= "HLT_PAL1SingleEG20_TotemDiffractive";
    trg_name[86]= "HLT_PAL1SingleJet16";
    trg_name[87]= "HLT_PAL1SingleJet36";
    trg_name[88]= "HLT_PAL1SingleJet52_TotemDiffractive";
    trg_name[89]= "HLT_PAL1SingleMu20_TotemDiffractive";
    trg_name[90]= "HLT_PAL1Tech53_MB_SingleTrack";
    trg_name[91]= "HLT_PAL1Tech53_MB";
    trg_name[92]= "HLT_PAL1Tech54_ZeroBias";
    trg_name[93]= "HLT_PAL1Tech63_CASTORHaloMuon";
    trg_name[94]= "HLT_PAL1Tech_HBHEHO_totalOR";
    trg_name[95]= "HLT_PAL2DoubleMu3";
    trg_name[96]= "HLT_PAMinBiasBHC_OR";
    trg_name[97]= "HLT_PAMinBiasBHC";
    trg_name[98]= "HLT_PAMinBiasHF_OR";
    trg_name[99]= "HLT_PAMinBiasHF";
    trg_name[100]= "HLT_PAMinBiasHfOrBHC";
    trg_name[101]= "HLT_PAMu12";
    trg_name[102]= "HLT_PAMu3PFJet20";
    trg_name[103]= "HLT_PAMu3PFJet40";
    trg_name[104]= "HLT_PAMu3";
    trg_name[105]= "HLT_PAMu7PFJet20";
    trg_name[106]= "HLT_PAMu7_Ele7_CaloIdT_CaloIsoVL";
    trg_name[107]= "HLT_PAMu7";
    trg_name[108]= "HLT_PAPhoton10_NoCaloIdVL";
    trg_name[109]= "HLT_PAPhoton10_Photon10_NoCaloIdVL";
    trg_name[110]= "HLT_PAPhoton10_Photon10_TightCaloIdVL_Iso50";
    trg_name[111]= "HLT_PAPhoton10_Photon10_TightCaloIdVL";
    trg_name[112]= "HLT_PAPhoton10_TightCaloIdVL_Iso50";
    trg_name[113]= "HLT_PAPhoton10_TightCaloIdVL";
    trg_name[114]= "HLT_PAPhoton15_NoCaloIdVL";
    trg_name[115]= "HLT_PAPhoton15_Photon10_NoCaloIdVL";
    trg_name[116]= "HLT_PAPhoton15_Photon10_TightCaloIdVL";
    trg_name[117]= "HLT_PAPhoton15_TightCaloIdVL_Iso50";
    trg_name[118]= "HLT_PAPhoton15_TightCaloIdVL";
    trg_name[119]= "HLT_PAPhoton20_NoCaloIdVL";
    trg_name[120]= "HLT_PAPhoton20_Photon15_NoCaloIdVL";
    trg_name[121]= "HLT_PAPhoton20_Photon15_TightCaloIdVL";
    trg_name[122]= "HLT_PAPhoton20_Photon20_NoCaloIdVL";
    trg_name[123]= "HLT_PAPhoton20_TightCaloIdVL_Iso50";
    trg_name[124]= "HLT_PAPhoton20_TightCaloIdVL";
    trg_name[125]= "HLT_PAPhoton30_NoCaloIdVL";
    trg_name[126]= "HLT_PAPhoton30_Photon30_NoCaloIdVL";
    trg_name[127]= "HLT_PAPhoton30_TightCaloIdVL_Iso50";
    trg_name[128]= "HLT_PAPhoton30_TightCaloIdVL";
    trg_name[129]= "HLT_PAPhoton40_NoCaloIdVL";
    trg_name[130]= "HLT_PAPhoton40_TightCaloIdVL";
    trg_name[131]= "HLT_PAPhoton60_NoCaloIdVL";
    trg_name[132]= "HLT_PAPixelTrackMultiplicity100_FullTrack12";
    trg_name[133]= "HLT_PAPixelTrackMultiplicity100_L2DoubleMu3";
    trg_name[134]= "HLT_PAPixelTrackMultiplicity130_FullTrack12";
    trg_name[135]= "HLT_PAPixelTrackMultiplicity140_Jet80_NoJetID";
    trg_name[136]= "HLT_PAPixelTrackMultiplicity160_FullTrack12";
    trg_name[137]= "HLT_PAPixelTracks_Multiplicity100";
    trg_name[138]= "HLT_PAPixelTracks_Multiplicity130";
    trg_name[139]= "HLT_PAPixelTracks_Multiplicity160";
    trg_name[140]= "HLT_PAPixelTracks_Multiplicity190";
    trg_name[141]= "HLT_PAPixelTracks_Multiplicity220";
    trg_name[142]= "HLT_PARandom";
    trg_name[143]= "HLT_PARomanPots_Tech52";
    trg_name[144]= "HLT_PASingleEle6_CaloIdNone_TrkIdVL";
    trg_name[145]= "HLT_PASingleEle6_CaloIdT_TrkIdVL";
    trg_name[146]= "HLT_PASingleEle8_CaloIdNone_TrkIdVL";
    trg_name[147]= "HLT_PASingleForJet15";
    trg_name[148]= "HLT_PASingleForJet25";
    trg_name[149]= "HLT_PAT1minbias_Tech55";
    trg_name[150]= "HLT_PATech35_HFSumET100";
    trg_name[151]= "HLT_PATech35";
    trg_name[152]= "HLT_PATripleJet100_20_20";
    trg_name[153]= "HLT_PATripleJet20_20_20";
    trg_name[154]= "HLT_PATripleJet40_20_20";
    trg_name[155]= "HLT_PATripleJet60_20_20";
    trg_name[156]= "HLT_PATripleJet80_20_20";
    trg_name[157]= "HLT_PAUpcSingleEG5Full_TrackVeto7";
    trg_name[158]= "HLT_PAUpcSingleEG5Pixel_TrackVeto";
    trg_name[159]= "HLT_PAUpcSingleMuOpenFull_TrackVeto7";
    trg_name[160]= "HLT_PAUpcSingleMuOpenPixel_TrackVeto";
    trg_name[161]= "HLT_PAUpcSingleMuOpenTkMu_Onia";
    trg_name[162]= "HLT_PAZeroBiasPixel_DoubleTrack";
    trg_name[163]= "HLT_PAZeroBiasPixel_SingleTrack";
    trg_name[164]= "HLT_PAZeroBias";
    trg_name[165]= "HLT_Physics";
    trg_name[166]= "HLT_TrackerCalibration";
    trg_name[167]= "HLTriggerFinalPath";
    trg_name[168]= "HLTriggerFirstPath";
    trg_name[169]= "NanoDSTOutput";
    trg_name[170]= "RPCMONOutput";
    trg_name[171]= "TrackerCalibrationOutput";
    trg_name[172]= "L1_SingleJet12_BptxAND";
    trg_name[173]= "L1_SingleJet16_BptxAND";
    trg_name[174]= "L1_SingleJet36";
    trg_name[175]= "L1_SingleEG5_BptxAND";
    trg_name[176]= "L1_SingleEG12";
    trg_name[177]= "L1_SingleEG20";
    trg_name[178]= "L1_ETT20_BptxAND";
    trg_name[179]= "L1_ETT40";
    trg_name[180]= "L1_ZeroBias";
}

//
void FindUsefulTrigger::get_hist(TFile* f, int ich)
{
    cout<<"--> getting histogram <---"<<endl;
    char hname[100];
    //... all trigger 
    for(short i = 0; i<npt; i++) {

        sprintf(hname, "hfg_tot%d_%d", ich, i);
        hfg_tot[ich][i] = (TH1F*)f->Get(hname);

        sprintf(hname, "hfgdiff_tot%d_%d", ich, i);
        hfgdiff_tot[ich][i] = (TH1F*)f->Get(hname);

        sprintf(hname, "hbgdiff_tot%d_%d", ich, i);
        hbgdiff_tot[ich][i] = (TH1F*)f->Get(hname);

        sprintf(hname, "HLT_tot%d_%d", ich, i);
        HLT_tot[ich][i] = (TH1F*)f->Get(hname);
        for(int j = 0; j<nhist; j++) {
            sprintf(hname, "h%d_%d_%d", j, ich, i);
            h[j][ich][i] = (TH1F*)f->Get(hname);
        }
    }

    //.. each trigger 
    for(short it = 0; it<ntrg; it++) {
        cout<<"     it:  "<<it<<" of "<<ntrg<<endl;
        for(short i = 0; i<npt; i++) {

            sprintf(hname, "hfg%d_%d_%d", ich, it, i);
            hfg[ich][it][i] = (TH1F*)f->Get(hname);

            sprintf(hname, "hfgdiff%d_%d_%d", ich, it, i);
            hfgdiff[ich][it][i] = (TH1F*)f->Get(hname);

            sprintf(hname, "hbgdiff%d_%d_%d", ich, it, i);
            hbgdiff[ich][it][i] = (TH1F*)f->Get(hname);
        }
    }
}

//
void FindUsefulTrigger::draw(char* filename, int ich, TCanvas* cfg, bool allTrg = true)
{//.. draw the hisogram and see which trigger is useful 

    book_hist();

    TFile* f = new TFile(filename);
    get_hist(f, ich);
    get_trg_name();

    //.. plot
    if(allTrg) {//.. one plot per trigger ..
        char outfile[1000];

        cfg->Divide(3, 3);

        for(int itrg = 0; itrg < ntrg; itrg++) {
            for(short i = 0; i<9; i++) {
                cfg->Update();
                cfg->cd(i+1);

                if(ich!=0) {
                    hfg_tot[ich][i]->SetLineColor(1);
                    hfg_tot[ich][i]->GetXaxis()->SetNdivisions(505);
                    hfg_tot[ich][i]->SetMinimum(0);

                    hfg_tot[ich][i]->Draw();

                    hfg[ich][itrg][i]->SetLineColor(2);
                    hfg[ich][itrg][i]->Draw("same");
                } else {//..for D*
                    hfgdiff_tot[ich][i]->SetLineColor(1);
                    hfgdiff_tot[ich][i]->GetXaxis()->SetNdivisions(505);
                    hfgdiff_tot[ich][i]->SetMinimum(0);

                    hfgdiff_tot[ich][i]->Draw();

                    hfgdiff[ich][itrg][i]->SetLineColor(2);
                    hfgdiff[ich][itrg][i]->Draw("same");
                }
            }

            sprintf(outfile, "%s_%s.gif", mesonName[ich], trg_name[itrg]);
            cfg->SaveAs(outfile);
        }
    } else {//.. drop specific groups of triggers ....

        cfg->Divide(3, 3);

        for(short i = 0; i<9; i++) {
            cfg->Update();
            cfg->cd(i+1);

            hfg_tot[ich][i]->SetLineColor(1);
            hfg_tot[ich][i]->GetXaxis()->SetNdivisions(505);
            hfg_tot[ich][i]->SetMinimum(0);

            hfg_tot[ich][i]->Draw();

            HLT_tot[ich][i]->SetLineColor(2);
            int color[nhist] = {1, 3, 4, 5, 6, 7, 9, 1, 3, 4, 5, 6, 7, 9};
            int style[nhist] = {2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1};

            HLT_tot[ich][i]->Draw("same");
            for(int j = 0; j<nhist; j++) {
                h[j][ich][i]->SetLineColor(color[j]);
                h[j][ich][i]->SetLineStyle(style[j]);

                //.. these are the trigger before the final round 
                if(j==6 || j==8 || j==10 || j==11 || j==12 ||j==13 || j==3 || j==2 || j==1 || j==0)
                    h[j][ich][i]->Draw("same");
            }
        }
        cfg->SaveAs("HLT_OR.gif");
    }
}

void FindUsefulTrigger::fill_nt_prscl()
{
    for(int i = 0; i<ntrg; i++) {
        nt_prscl->Fill(i, prscl[i], lumi, run);
    }
}
