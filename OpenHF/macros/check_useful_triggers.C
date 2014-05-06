#define class_HltTree_cxx
#define class_HiTree_cxx

#include <fstream>
#include <iostream>

#include "TH1F.h"
#include "TFile.h"
#include "TSystem.h"
#include "TTree.h"
#include "TNtuple.h"
#include "TBranch.h"

#include "../interface/hfcand_v0.hh"

const int ntrg = 334;
int prscl[ntrg] = {};
int trg[ntrg] = {};
const char* trg_name[ntrg];

void get_trg_name();
void get_trg_info(TTree* t);
void reset();

void check_useful_triggers(int startFile, int endFile, char *filelist) 
{
    char* filename = new char[1000];
    sprintf(filename, "check_useful_triggers_%s_%dTo%d.root",  filelist, startFile, endFile);
    TFile result(filename, "recreate");

    //
    get_trg_name();

    TH1F* h = new TH1F("h", "", ntrg, 0, ntrg);
    for (int i=1;i<=ntrg;i++) {
        h->GetXaxis()->SetBinLabel(i,trg_name[i-1]);
    }

    //
    hfcand_v0* hfcandidate = new hfcand_v0;

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
        TTree *hftree = (TTree*)f->Get("OpenHfTree/hftree");

        if(!hftree || !HltTree) {
            cout<<"==> empty tree <=="<<endl;
            continue;
        }

        //.. add friend ...
        HltTree->AddFriend(hftree);

        //.. fetch data ..
        hftree->SetBranchAddress("hfcandidate", &hfcandidate);
        get_trg_info(HltTree);


        Long64_t nentries = HltTree->GetEntries();
        for (Long64_t i=0;i<nentries;i++) {
            HltTree->GetEntry(i);

            if(hfcandidate->get_nhfcand()) {
                for(int it = 0; it<ntrg; it++) {
                    if(trg[it]) 
                        h->Fill(it);
                }
            }

        }

        f->Close();
    }

    result.cd();
    h->Write();
    result.Close();
}

//
void get_trg_info(TTree* T) 
{
    reset();

    TObjArray *branches = (TObjArray *)T->GetListOfBranches();
    TIter branch_iter(branches); 
    TBranch* branch = 0;
    while ((branch = (TBranch *)branch_iter.Next())) {  
        TString branch_name = branch->GetName();
        for(int it = 0; it<ntrg; it++) {
            if (branch_name.Contains(trg_name[it])){
                if(branch_name.Contains("Prescl")) {
                    T->SetBranchAddress(branch_name.Data(), &prscl[it]);
                    //cout<<" prescale: "<<branch_name.Data()<<endl;
                } else {
                    T->SetBranchAddress(branch_name.Data(), &trg[it]);
                    //cout<<" trg: "<<branch_name.Data()<<endl;
                }

                break;
            } 
        }
    }
}

//
void reset()
{
    for(int i = 0; i<ntrg; i++) {
        prscl[i] = 0;
        trg[i] = 0;
    }
}

//
void get_trg_name()
{//.. this code is made by: ./make_code_to_read_hlt_name_pscl.txt.pl
    trg_name[0]= "HLTriggerFirstPath";
    trg_name[1]= "HLT_Activity_Ecal_SC7";
    trg_name[2]= "HLT_BeamGas_HF_Beam1";
    trg_name[3]= "HLT_BeamGas_HF_Beam2";
    trg_name[4]= "HLT_BeamHalo";
    trg_name[5]= "HLT_PAHcalUTCA";
    trg_name[6]= "HLT_PAHcalPhiSym";
    trg_name[7]= "HLT_PAHcalNZS";
    trg_name[8]= "HLT_GlobalRunHPDNoise";
    trg_name[9]= "HLT_Physics";
    trg_name[10]= "DST_Physics";
    trg_name[11]= "HLT_DTCalibration";
    trg_name[12]= "HLT_EcalCalibration";
    trg_name[13]= "HLT_HcalCalibration";
    trg_name[14]= "HLT_TrackerCalibration";
    trg_name[15]= "HLT_L1SingleMuOpen_AntiBPTX";
    trg_name[16]= "HLT_L1TrackerCosmics";
    trg_name[17]= "AlCa_PAEcalPi0EBonly";
    trg_name[18]= "AlCa_PAEcalPi0EEonly";
    trg_name[19]= "AlCa_PAEcalEtaEBonly";
    trg_name[20]= "AlCa_PAEcalEtaEEonly";
    trg_name[21]= "AlCa_EcalPhiSym";
    trg_name[22]= "AlCa_RPCMuonNoTriggers";
    trg_name[23]= "AlCa_RPCMuonNoHits";
    trg_name[24]= "AlCa_RPCMuonNormalisation";
    trg_name[25]= "AlCa_LumiPixels";
    trg_name[26]= "AlCa_LumiPixels_ZeroBias";
    trg_name[27]= "AlCa_LumiPixels_Random";
    trg_name[28]= "HLT_PAL1SingleJet16";
    trg_name[29]= "HLT_PAL1SingleJet36";
    trg_name[30]= "HLT_PASingleForJet15";
    trg_name[31]= "HLT_PASingleForJet25";
    trg_name[32]= "HLT_PAJet20_NoJetID";
    trg_name[33]= "HLT_PAJet40_NoJetID";
    trg_name[34]= "HLT_PAJet60_NoJetID";
    trg_name[35]= "HLT_PAJet80_NoJetID";
    trg_name[36]= "HLT_PAJet100_NoJetID";
    trg_name[37]= "HLT_PAJet120_NoJetID";
    trg_name[38]= "HLT_PAForJet20Eta2";
    trg_name[39]= "HLT_PAForJet40Eta2";
    trg_name[40]= "HLT_PAForJet60Eta2";
    trg_name[41]= "HLT_PAForJet80Eta2";
    trg_name[42]= "HLT_PAForJet100Eta2";
    trg_name[43]= "HLT_PAForJet20Eta3";
    trg_name[44]= "HLT_PAForJet40Eta3";
    trg_name[45]= "HLT_PAForJet60Eta3";
    trg_name[46]= "HLT_PAForJet80Eta3";
    trg_name[47]= "HLT_PAForJet100Eta3";
    trg_name[48]= "HLT_PATripleJet20_20_20";
    trg_name[49]= "HLT_PATripleJet40_20_20";
    trg_name[50]= "HLT_PATripleJet60_20_20";
    trg_name[51]= "HLT_PATripleJet80_20_20";
    trg_name[52]= "HLT_PATripleJet100_20_20";
    trg_name[53]= "HLT_PAJet40ETM30";
    trg_name[54]= "HLT_PAJet60ETM30";
    trg_name[55]= "HLT_PAL1DoubleMu0";
    trg_name[56]= "HLT_PADimuon0_NoVertexing";
    trg_name[57]= "HLT_PAL1DoubleMu0_HighQ";
    trg_name[58]= "HLT_PAL1DoubleMuOpen";
    trg_name[59]= "HLT_PAL2DoubleMu3";
    trg_name[60]= "HLT_PAMu3";
    trg_name[61]= "HLT_PAMu7";
    trg_name[62]= "HLT_PAMu12";
    trg_name[63]= "HLT_PABTagMu_Jet20_Mu4";
    trg_name[64]= "HLT_PAMu3PFJet20";
    trg_name[65]= "HLT_PAMu3PFJet40";
    trg_name[66]= "HLT_PAMu7PFJet20";
    trg_name[67]= "HLT_PAPhoton10_NoCaloIdVL";
    trg_name[68]= "HLT_PAPhoton15_NoCaloIdVL";
    trg_name[69]= "HLT_PAPhoton20_NoCaloIdVL";
    trg_name[70]= "HLT_PAPhoton30_NoCaloIdVL";
    trg_name[71]= "HLT_PAPhoton40_NoCaloIdVL";
    trg_name[72]= "HLT_PAPhoton60_NoCaloIdVL";
    trg_name[73]= "HLT_PAPhoton10_TightCaloIdVL";
    trg_name[74]= "HLT_PAPhoton15_TightCaloIdVL";
    trg_name[75]= "HLT_PAPhoton20_TightCaloIdVL";
    trg_name[76]= "HLT_PAPhoton30_TightCaloIdVL";
    trg_name[77]= "HLT_PAPhoton40_TightCaloIdVL";
    trg_name[78]= "HLT_PAPhoton10_TightCaloIdVL_Iso50";
    trg_name[79]= "HLT_PAPhoton15_TightCaloIdVL_Iso50";
    trg_name[80]= "HLT_PAPhoton20_TightCaloIdVL_Iso50";
    trg_name[81]= "HLT_PAPhoton30_TightCaloIdVL_Iso50";
    trg_name[82]= "HLT_PAPhoton10_Photon10_NoCaloIdVL";
    trg_name[83]= "HLT_PAPhoton15_Photon10_NoCaloIdVL";
    trg_name[84]= "HLT_PAPhoton20_Photon15_NoCaloIdVL";
    trg_name[85]= "HLT_PAPhoton20_Photon20_NoCaloIdVL";
    trg_name[86]= "HLT_PAPhoton30_Photon30_NoCaloIdVL";
    trg_name[87]= "HLT_PAPhoton10_Photon10_TightCaloIdVL";
    trg_name[88]= "HLT_PAPhoton10_Photon10_TightCaloIdVL_Iso50";
    trg_name[89]= "HLT_PAPhoton15_Photon10_TightCaloIdVL";
    trg_name[90]= "HLT_PAPhoton20_Photon15_TightCaloIdVL";
    trg_name[91]= "HLT_PASingleEle6_CaloIdT_TrkIdVL";
    trg_name[92]= "HLT_PASingleEle6_CaloIdNone_TrkIdVL";
    trg_name[93]= "HLT_PASingleEle8_CaloIdNone_TrkIdVL";
    trg_name[94]= "HLT_PAL1DoubleEG5DoubleEle6_CaloIdT_TrkIdVL";
    trg_name[95]= "HLT_PADoubleEle6_CaloIdT_TrkIdVL";
    trg_name[96]= "HLT_PADoubleEle8_CaloIdT_TrkIdVL";
    trg_name[97]= "HLT_PAPixelTracks_Multiplicity100";
    trg_name[98]= "HLT_PAPixelTracks_Multiplicity130";
    trg_name[99]= "HLT_PAPixelTracks_Multiplicity160";
    trg_name[100]= "HLT_PAPixelTracks_Multiplicity190";
    trg_name[101]= "HLT_PAPixelTracks_Multiplicity220";
    trg_name[102]= "HLT_PAPixelTrackMultiplicity100_FullTrack12";
    trg_name[103]= "HLT_PAPixelTrackMultiplicity130_FullTrack12";
    trg_name[104]= "HLT_PAPixelTrackMultiplicity160_FullTrack12";
    trg_name[105]= "HLT_PAPixelTrackMultiplicity100_L2DoubleMu3";
    trg_name[106]= "HLT_PAPixelTrackMultiplicity140_Jet80_NoJetID";
    trg_name[107]= "HLT_PATech35";
    trg_name[108]= "HLT_PATech35_HFSumET100";
    trg_name[109]= "HLT_PAFullTrack12";
    trg_name[110]= "HLT_PAFullTrack20";
    trg_name[111]= "HLT_PAFullTrack30";
    trg_name[112]= "HLT_PAFullTrack50";
    trg_name[113]= "HLT_PAHFSumET100";
    trg_name[114]= "HLT_PAHFSumET140";
    trg_name[115]= "HLT_PAHFSumET170";
    trg_name[116]= "HLT_PAHFSumET210";
    trg_name[117]= "HLT_PARomanPots_Tech52";
    trg_name[118]= "HLT_PAL1Tech53_MB";
    trg_name[119]= "HLT_PAL1Tech53_MB_SingleTrack";
    trg_name[120]= "HLT_PAL1Tech54_ZeroBias";
    trg_name[121]= "HLT_PAT1minbias_Tech55";
    trg_name[122]= "HLT_PAL1Tech_HBHEHO_totalOR";
    trg_name[123]= "HLT_PAL1Tech63_CASTORHaloMuon";
    trg_name[124]= "HLT_PACastorEmTotemLowMultiplicity";
    trg_name[125]= "HLT_PACastorEmNotHfCoincidencePm";
    trg_name[126]= "HLT_PACastorEmNotHfSingleChannel";
    trg_name[127]= "HLT_PAL1CastorTotalTotemLowMultiplicity";
    trg_name[128]= "HLT_PAMinBiasHF";
    trg_name[129]= "HLT_PAMinBiasHF_OR";
    trg_name[130]= "HLT_PAMinBiasBHC";
    trg_name[131]= "HLT_PAMinBiasBHC_OR";
    trg_name[132]= "HLT_PAMinBiasHfOrBHC";
    trg_name[133]= "HLT_PABptxPlusNotBptxMinus";
    trg_name[134]= "HLT_PABptxMinusNotBptxPlus";
    trg_name[135]= "HLT_PAZeroBias";
    trg_name[136]= "HLT_PAZeroBiasPixel_SingleTrack";
    trg_name[137]= "HLT_PAHFOR_SingleTrack";
    trg_name[138]= "HLT_PAZeroBiasPixel_DoubleTrack";
    trg_name[139]= "HLT_PADoubleMu4_Acoplanarity03";
    trg_name[140]= "HLT_PAExclDijet35_HFAND";
    trg_name[141]= "HLT_PAL1DoubleEG3_FwdVeto";
    trg_name[142]= "HLT_PAL1DoubleJet20_TotemDiffractive";
    trg_name[143]= "HLT_PADoubleJet20_ForwardBackward";
    trg_name[144]= "HLT_PAMu7_Ele7_CaloIdT_CaloIsoVL";
    trg_name[145]= "HLT_PAUpcSingleEG5Pixel_TrackVeto";
    trg_name[146]= "HLT_PAUpcSingleEG5Full_TrackVeto7";
    trg_name[147]= "HLT_PAUpcSingleMuOpenPixel_TrackVeto";
    trg_name[148]= "HLT_PAUpcSingleMuOpenFull_TrackVeto7";
    trg_name[149]= "HLT_PAUpcSingleMuOpenTkMu_Onia";
    trg_name[150]= "HLT_PARandom";
    trg_name[151]= "DQM_FEDIntegrity";
    trg_name[152]= "HLT_LogMonitor";
    trg_name[153]= "HLTriggerFinalPath";
    trg_name[154]= "L1_AlwaysTrue";
    trg_name[155]= "L1_BeamGas_Hf_BptxMinusPostQuiet";
    trg_name[156]= "L1_BeamGas_Hf_BptxPlusPostQuiet";
    trg_name[157]= "L1_BeamHalo";
    trg_name[158]= "L1_BptxMinus_NotBptxPlus";
    trg_name[159]= "L1_BptxPlus_NotBptxMinus";
    trg_name[160]= "L1_BscHighMultiplicity_BptxAND";
    trg_name[161]= "L1_BscMinBiasOR_BptxAND";
    trg_name[162]= "L1_BscMinBiasThreshold1_BptxAND";
    trg_name[163]= "L1_BscMinBiasThreshold2_BptxAND";
    trg_name[164]= "L1_CastorEm_NotHcalHfCoincidencePm";
    trg_name[165]= "L1_CastorEm_NotHcalHfSingleChannel";
    trg_name[166]= "L1_CastorEm_TotemLowMultiplicity";
    trg_name[167]= "L1_CastorTotalEnergy_TotemLowMultiplicity";
    trg_name[168]= "L1_DoubleEG3_FwdVeto";
    trg_name[169]= "L1_DoubleEG5";
    trg_name[170]= "L1_DoubleEG5_TotemDiffractive";
    trg_name[171]= "L1_DoubleEG6_HTT100";
    trg_name[172]= "L1_DoubleEG6_HTT125";
    trg_name[173]= "L1_DoubleEG_13_7";
    trg_name[174]= "L1_DoubleForJet16_EtaOpp";
    trg_name[175]= "L1_DoubleJet20";
    trg_name[176]= "L1_DoubleJet20_TotemDiffractive";
    trg_name[177]= "L1_DoubleJet24";
    trg_name[178]= "L1_DoubleJet36_Central";
    trg_name[179]= "L1_DoubleJet52_Central";
    trg_name[180]= "L1_DoubleJetC36_TotemDiffractive";
    trg_name[181]= "L1_DoubleJetC44_ETM30";
    trg_name[182]= "L1_DoubleJetC56";
    trg_name[183]= "L1_DoubleJetC56_Eta1p74_WdEta4";
    trg_name[184]= "L1_DoubleMu0";
    trg_name[185]= "L1_DoubleMu0_HighQ_EtaCuts";
    trg_name[186]= "L1_DoubleMu3p5_EG5";
    trg_name[187]= "L1_DoubleMu5_EG5";
    trg_name[188]= "L1_DoubleMu5_TotemDiffractive";
    trg_name[189]= "L1_DoubleMu5";
    trg_name[190]= "L1_DoubleMuOpen_BptxAND";
    trg_name[191]= "L1_DoubleMu_10_3p5";
    trg_name[192]= "L1_DoubleMu_10_Open";
    trg_name[193]= "L1_DoubleMu_12_5";
    trg_name[194]= "L1_DoubleMu_3er_0er_HighQ_WdEta22";
    trg_name[195]= "L1_DoubleMu_5er_0er_HighQ_WdEta22";
    trg_name[196]= "L1_EG8_DoubleJetC20";
    trg_name[197]= "L1_ETM100";
    trg_name[198]= "L1_ETM30";
    trg_name[199]= "L1_ETM36";
    trg_name[200]= "L1_ETM40";
    trg_name[201]= "L1_ETM50";
    trg_name[202]= "L1_ETM70";
    trg_name[203]= "L1_ETT140";
    trg_name[204]= "L1_ETT20_BptxAND";
    trg_name[205]= "L1_ETT300";
    trg_name[206]= "L1_ETT40";
    trg_name[207]= "L1_ETT60";
    trg_name[208]= "L1_ETT80";
    trg_name[209]= "L1_HTT100";
    trg_name[210]= "L1_HTT125";
    trg_name[211]= "L1_HTT150";
    trg_name[212]= "L1_HTT175";
    trg_name[213]= "L1_HTT200";
    trg_name[214]= "L1_HTT50";
    trg_name[215]= "L1_HTT75";
    trg_name[216]= "L1_HcalHfCoincidencePm_BptxAND";
    trg_name[217]= "L1_HcalHfSingleChannel_BptxAND";
    trg_name[218]= "L1_Mu10er_JetC32";
    trg_name[219]= "L1_Mu12_EG7";
    trg_name[220]= "L1_Mu3_Jet16";
    trg_name[221]= "L1_Mu3_Jet36";
    trg_name[222]= "L1_Mu3_JetC16_WdEtaPhi2";
    trg_name[223]= "L1_Mu3_JetC52_WdEtaPhi2";
    trg_name[224]= "L1_Mu5_DoubleEG5";
    trg_name[225]= "L1_Mu5_DoubleEG6";
    trg_name[226]= "L1_Mu7_Jet16";
    trg_name[227]= "L1_Mu8_DoubleJetC20";
    trg_name[228]= "L1_MuOpen_EG12";
    trg_name[229]= "L1_MuOpen_EG5";
    trg_name[230]= "L1_QuadJetC32";
    trg_name[231]= "L1_QuadJetC36";
    trg_name[232]= "L1_QuadJetC40";
    trg_name[233]= "L1_SingleEG12";
    trg_name[234]= "L1_SingleEG18er";
    trg_name[235]= "L1_SingleEG20";
    trg_name[236]= "L1_SingleEG20_TotemDiffractive";
    trg_name[237]= "L1_SingleEG22";
    trg_name[238]= "L1_SingleEG24";
    trg_name[239]= "L1_SingleEG30";
    trg_name[240]= "L1_SingleEG5_BptxAND";
    trg_name[241]= "L1_SingleEG7";
    trg_name[242]= "L1_SingleForJet16";
    trg_name[243]= "L1_SingleIsoEG18er";
    trg_name[244]= "L1_SingleIsoEG20er";
    trg_name[245]= "L1_SingleJet128";
    trg_name[246]= "L1_SingleJet12_BptxAND";
    trg_name[247]= "L1_SingleJet16_BptxAND";
    trg_name[248]= "L1_SingleJet16_FwdVeto5";
    trg_name[249]= "L1_SingleJet20_Central_NotBptxOR";
    trg_name[250]= "L1_SingleJet36";
    trg_name[251]= "L1_SingleJet36_FwdVeto5";
    trg_name[252]= "L1_SingleJet52";
    trg_name[253]= "L1_SingleJet52_TotemDiffractive";
    trg_name[254]= "L1_SingleJet68";
    trg_name[255]= "L1_SingleJet92";
    trg_name[256]= "L1_SingleJetC32_NotBptxOR";
    trg_name[257]= "L1_SingleMu12";
    trg_name[258]= "L1_SingleMu12er";
    trg_name[259]= "L1_SingleMu14_Eta2p1";
    trg_name[260]= "L1_SingleMu16";
    trg_name[261]= "L1_SingleMu16_Eta2p1";
    trg_name[262]= "L1_SingleMu18er";
    trg_name[263]= "L1_SingleMu20";
    trg_name[264]= "L1_SingleMu20_TotemDiffractive";
    trg_name[265]= "L1_SingleMu20er";
    trg_name[266]= "L1_SingleMu25er";
    trg_name[267]= "L1_SingleMu3";
    trg_name[268]= "L1_SingleMu6_NotBptxOR";
    trg_name[269]= "L1_SingleMu7";
    trg_name[270]= "L1_SingleMuBeamHalo";
    trg_name[271]= "L1_SingleMuOpen";
    trg_name[272]= "L1_TripleEG7";
    trg_name[273]= "L1_TripleEG_12_7_5";
    trg_name[274]= "L1_TripleJetC_52_28_28";
    trg_name[275]= "L1_TripleJet_64_44_24_VBF";
    trg_name[276]= "L1_TripleJet_64_48_28_VBF";
    trg_name[277]= "L1_ZdcCaloPlus_TotemDiffractive_QElastic";
    trg_name[278]= "L1_ZeroBias";
    trg_name[279]= "L1Tech_BPTX_PreBPTX";
    trg_name[280]= "L1Tech_BPTX_minus";
    trg_name[281]= "L1Tech_BPTX_minus_AND_not_plus";
    trg_name[282]= "L1Tech_BPTX_plus";
    trg_name[283]= "L1Tech_BPTX_plus_AND_NOT_minus";
    trg_name[284]= "L1Tech_BPTX_plus_AND_minus";
    trg_name[285]= "L1Tech_BPTX_plus_AND_minus_instance1";
    trg_name[286]= "L1Tech_BPTX_plus_OR_minus";
    trg_name[287]= "L1Tech_BPTX_quiet";
    trg_name[288]= "L1Tech_BSC_HighMultiplicity";
    trg_name[289]= "L1Tech_BSC_halo_beam1_inner";
    trg_name[290]= "L1Tech_BSC_halo_beam1_outer";
    trg_name[291]= "L1Tech_BSC_halo_beam2_inner";
    trg_name[292]= "L1Tech_BSC_halo_beam2_outer";
    trg_name[293]= "L1Tech_BSC_minBias_OR";
    trg_name[294]= "L1Tech_BSC_minBias_inner_threshold1";
    trg_name[295]= "L1Tech_BSC_minBias_inner_threshold2";
    trg_name[296]= "L1Tech_BSC_minBias_threshold1";
    trg_name[297]= "L1Tech_BSC_minBias_threshold2";
    trg_name[298]= "L1Tech_BSC_splash_beam1";
    trg_name[299]= "L1Tech_BSC_splash_beam2";
    trg_name[300]= "L1Tech_CASTOR_0";
    trg_name[301]= "L1Tech_CASTOR_EM";
    trg_name[302]= "L1Tech_CASTOR_HaloMuon";
    trg_name[303]= "L1Tech_CASTOR_TotalEnergy";
    trg_name[304]= "L1Tech_DT_GlobalOR";
    trg_name[305]= "L1Tech_FSC_St3Sect45_downLeft";
    trg_name[306]= "L1Tech_FSC_St3Sect45_downRight";
    trg_name[307]= "L1Tech_FSC_St3Sect45_uppLeft";
    trg_name[308]= "L1Tech_FSC_St3Sect45_uppRight";
    trg_name[309]= "L1Tech_FSC_St3Sect56_downLeft";
    trg_name[310]= "L1Tech_FSC_St3Sect56_downRight";
    trg_name[311]= "L1Tech_FSC_St3Sect56_uppLeft";
    trg_name[312]= "L1Tech_FSC_St3Sect56_uppRight";
    trg_name[313]= "L1Tech_HCAL_HBHE_totalOR";
    trg_name[314]= "L1Tech_HCAL_HF_MMP_or_MPP";
    trg_name[315]= "L1Tech_HCAL_HF_coincidence_PM";
    trg_name[316]= "L1Tech_HCAL_HF_single_channel";
    trg_name[317]= "L1Tech_HCAL_HO_totalOR";
    trg_name[318]= "L1Tech_RPC_TTU_RB0_Cosmics";
    trg_name[319]= "L1Tech_RPC_TTU_RBminus1_Cosmics";
    trg_name[320]= "L1Tech_RPC_TTU_RBminus2_Cosmics";
    trg_name[321]= "L1Tech_RPC_TTU_RBplus1_Cosmics";
    trg_name[322]= "L1Tech_RPC_TTU_RBplus2_Cosmics";
    trg_name[323]= "L1Tech_RPC_TTU_RBst1_collisions";
    trg_name[324]= "L1Tech_RPC_TTU_barrel_Cosmics";
    trg_name[325]= "L1Tech_RPC_TTU_pointing_Cosmics";
    trg_name[326]= "L1Tech_TOTEM_Diffractive";
    trg_name[327]= "L1Tech_TOTEM_LowMultiplicity";
    trg_name[328]= "L1Tech_TOTEM_MinBias";
    trg_name[329]= "L1Tech_TOTEM_ZeroBias";
    trg_name[330]= "L1Tech_ZDC_Scint_loose";
    trg_name[331]= "L1Tech_ZDC_Scint_minus";
    trg_name[332]= "L1Tech_ZDC_Scint_plus";
    trg_name[333]= "L1Tech_ZDC_Scint_tight";
}
