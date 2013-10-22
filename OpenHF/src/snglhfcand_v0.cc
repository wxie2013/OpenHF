#include "../interface/snglhfcand_v0.hh"

ClassImp(snglhfcand_v0)

void snglhfcand_v0::Reset()
{
        //.. topology cut
        fprob = -1;
        ff3d = -1;
        ff3de = -1;
        ffls3d = -1;
        fchi2 = -1;
        falpha0 = -1;
        fdr = -1;

        //... parent meson infor
        type = -1;
        fpt = -1;
        fm = -1;
        feta = -999;
        fphi = -999;

        //.. daughter meson info ..
        fmdau1 = -1;
        fptdau1 = -1;
        fetadau1 = -999;
        fphidau1 = -999;

        //.. daughter single particle info
        fptdau2 = -1;
        fetadau2 = -999;
        fphidau2 = -999;
        fqdau2 = 0;

        //.. daughter of daughter meson info
        fq1 = 0;
        fpt1 = -1;
        feta1 = -999;
        fphi1 = -999;

        fpt2 = -1;
        fq2 = 0;
        feta2 = -999;
        fphi2 = -999;

        nBPixLayer_dau2 = 0; 
        nPixLayer_dau2 = 0; 
        nBPixLayer1Hits_dau2 = 0; 
        nTrackerLayer_dau2 = 0; 

        nBPixLayer_1 = 0;
        nPixLayer_1 = 0;
        nBPixLayer1Hits_1 = 0;
        nTrackerLayer_1 = 0; 

        nBPixLayer_2 = 0;
        nPixLayer_2 = 0;
        nBPixLayer1Hits_2 = 0;
        nTrackerLayer_2 = 0;
}
