#ifndef ana_common_H
#define ana_common_H

typedef std::vector<trigger::TriggerObject> trigO;


//.. e.g. https://cmswbm.web.cern.ch/cmswbm/cmsdb/servlet/HLTSummary?RUN=211631&KEY=28422
const int NCH = 5; //.. 5 D meson decay channels
const int NTRG = 14; //.. for QM14. There are more to be handled later

const int NPT = 100; //.. number of pT bin
const float dPt = 1.0;

const int NBIN = 200;
const int NY = 3; //.. [-3,-1], [-1, 1], [1, 3];
const float dY = 2.0;
const float Ymin = -3;
const float matchWindow = 0.2;

const int nMax = 1000;

#endif
