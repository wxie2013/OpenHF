#ifndef ana_common_H
#define ana_common_H

typedef std::vector<trigger::TriggerObject> trigO;


//.. e.g. https://cmswbm.web.cern.ch/cmswbm/cmsdb/servlet/HLTSummary?RUN=211631&KEY=28422
const int NCH = 5; //.. 5 D meson decay channels

const int NPT = 100; //.. number of pT bin
const float dPt = 1.0; //.. this defines the minimum pT bin width. Can merge later

const int NBIN = 200; //.. number of bins in mass distribution
const int NY = 1; 
const float dY = 4;
const float Ymin = -2;
const float etaMax = 2.5; //.. maximum eta for single tracks

// D* 
const float mdiffup = 0.135;
const float mdiffdn = 0.154;

// note on "const char* const" instead of "const char*" 
const char* const MesonName[NCH] = {"Dstar2D0Pi", "D02KPi", "Ds2PhiPi", "Ds2KstarK", "Dpm2KPiPi"};
const float MesonMass[NCH] = {0.145, 1.865, 1.968, 1.968, 1.870};
// cut
//.. low and higher end of the mass hiso
const float cut_m_low[NCH] =  {1.92, 1.70, 1.80, 1.80, 1.70}; 
const float cut_m_high[NCH] = {2.10, 2.05, 2.15, 2.15, 2.05}; 
//low and high end of the daughter mass cut
const float cut_m_dau_low[NCH] =  {1.80, -1, 1.00, 0.0, 0.50}; 
const float cut_m_dau_high[NCH] = {1.92, -1, 1.04, 2.0, 2.05}; 
// topology cut 
const float cut_ffls3d[NCH] = {2.0, 2.0, 2.0, 2.0, 2.0}; 
const float cut_falpha0[NCH] = {0.2, 0.2, 0.2, 0.2, 0.2}; 
const float cut_fprob[NCH] = {0.05, 0.05, 0.05, 0.05, 0.05};
const float cut_fdr[NCH] = {0.25, 0.25, 0.25, 0.25, 0.25}; 
const float cut_fchi2[NCH] = {3.0, 3.0, 3.0, 3.0, 3.0};

//.. fit ranges 
const float mlow[NCH] = {0.14, 1.70, 1.8, 1.8, 1.78};
const float mup[NCH] = {0.154, 2.1, 2.1, 2.1, 2.1};

//
const int nTrkMax = 10000;
const int NFullTrkTrg = 4; //.. number of Full Trk trigger, including minbias in UPC
const float TrkTrgOffThres[NFullTrkTrg] = {0, 14, 22, 32}; //.. offline fully efficient

const float maxDiff = 1e-3; //.. for identical quantity comparison

const float dRjet = 0.4; //.. window for jet D meson association 

const float pTMinbiasMax = 24; //.. no clear signal after this and Jet20 eff is 100%. Do not contribute to trigger combination

//.. trigger combination andf merging 
// note: the bin width need to be the integer number of dPt
const int NptRebin = 31;
const float ptRebin[NptRebin+1] = {
    0,  2,  4,  6,  8, 
    10, 12, 14, 16, 18, 
    20, 22, 24, 26, 28, 
    30, 32, 34, 36, 38, 
    40, 42, 44, 46, 48, 
    50, 55, 60, 65, 70, 
    80, 100
};
/*
//.. this one is for minbiasUPC
const int NptRebin= 8;
const float ptRebin[NptRebin+1] = {
    0,  2,  4,  6,  8, 
    10, 12, 15, 25
};
*/

/*
const int NptRebin = 27;
const float ptRebin[NptRebin+1] = {
    0,  2,  4,  6,  8, 
    10, 12, 14, 16, 18, 
    20, 22, 24, 26, 28, 
    30, 32, 34, 36, 38, 
    40, 42, 44, 50, 60, 
    70, 80, 100
};
*/
/*
// for debugging at high pT
const int NptRebin = 21;
const float ptRebin[NptRebin+1] = {
    0,  2,  4,  6,  8, 
    10, 12, 14, 16, 18, 
    20, 22, 24, 26, 28, 
    30, 32, 34, 36, 38, 
    40, 100
};
*/

const int NTRG = 13; //.. including Track and minbias trigger
//produced by make_code_to_read_hlt_name_pscl.txt.pl
//.. all Run2013 pPb triggers. Most are useless. Here is to check which
//.. one has most of the D mesons. 
//.. trigger list is from, e.g. 
//.. https://cmswbm.web.cern.ch/cmswbm/cmsdb/servlet/HLTSummary?RUN=211631&KEY=28422
const char* const trg_name[NTRG] = {
    "HLT_PAZeroBiasPixel_SingleTrack", //..base: L1_ZeroBias
    "HLT_PAFullTrack12",   //.. base: L1_SingleJet12_BptxAND w/ prescl=1
    "HLT_PAFullTrack20",   //.. base: L1_SingleJet16_BptxAND w/ prescl=1
    "HLT_PAFullTrack30",   //.. base: L1_SingleJet16_BptxAND w/ prescl=1
    "HLT_PAJet20_NoJetID",   //.. base: L1_SingleJet16_BptxAND
    "HLT_PAJet40_NoJetID",   //.. base: L1_SingleJet16_BptxAND
    "HLT_PAJet60_NoJetID",   //.. base: L1_SingleJet36
    "HLT_PAJet80_NoJetID",   //.. base: L1_SingleJet36
    "HLT_PAJet100_NoJetID",   //.. base: L1_SingleJet36
    "L1_ZeroBias",
    "L1_SingleJet12_BptxAND",
    "L1_SingleJet16_BptxAND",
    "L1_SingleJet36"
};

#endif
