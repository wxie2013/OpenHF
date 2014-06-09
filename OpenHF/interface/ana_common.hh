#ifndef ana_common_H
#define ana_common_H

typedef std::vector<trigger::TriggerObject> trigO;


//.. e.g. https://cmswbm.web.cern.ch/cmswbm/cmsdb/servlet/HLTSummary?RUN=211631&KEY=28422
const int NCH = 5; //.. 5 D meson decay channels
const int NTRG = 19; //.. including photon, Jet, Track and corresponding L1 base trigger

const int NPT = 100; //.. number of pT bin
const float dPt = 1.0;

const int NBIN = 200; //.. number of bins in mass distribution
const int NY = 12; 
const float dY = 0.5;
const float Ymin = -3;

const int nMax = 1000;

// D* 
const float mdiffup = 0.135;
const float mdiffdn = 0.154;

#endif
