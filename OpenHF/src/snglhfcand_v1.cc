#include "../interface/snglhfcand_v1.hh"

ClassImp(snglhfcand_v1)

void snglhfcand_v1::Reset()
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

    Index_dau2 = -1;
    Index1 = -1;
    Index2 = -1;

    gIndex_dau2.clear();
    gIndex1.clear();
    gIndex2.clear();
}
