#ifndef ana_common_H
#define ana_common_H

typedef std::vector<trigger::TriggerObject> trigO;


//.. e.g. https://cmswbm.web.cern.ch/cmswbm/cmsdb/servlet/HLTSummary?RUN=211631&KEY=28422
const int NCH = 5; //.. 5 D meson decay channels
const int NTRG = 29; //.. including photon, Jet, Track and corresponding L1 base trigger and high multiplicity trigger

const int NPT = 100; //.. number of pT bin
const float dPt = 1.0; //.. this defines the minimum pT bin width. Can merge later

const int NBIN = 200; //.. number of bins in mass distribution
const int NY = 1; 
const float dY = 4;
const float Ymin = -2;
const float etaMax = 2.5; //.. maximum eta for single tracks

const int nMax = 1000;

// D* 
const float mdiffup = 0.135;
const float mdiffdn = 0.154;

//
const float MesonMass[NCH] = {0.145, 1.865, 1.968, 1.968, 1.870};
//.. fit ranges 
const float mlow[NCH] = {0.14, 1.70, 1.8, 1.8, 1.78};
const float mup[NCH] = {0.154, 2.0, 2.1, 2.1, 2.0};


//.. trigger combination andf merging 
// note: the bin width need to be the integer number of dPt
const int NptRebin = 26;
const float ptRebin[NptRebin+1] = {
    0, 2, 4, 6, 8, 10, 
    12, 14, 16, 18, 20, 
    22, 24, 26, 28, 30, 
    32, 34, 36, 38, 40, 
    42, 44, 50, 60, 80, 100
};

#endif
