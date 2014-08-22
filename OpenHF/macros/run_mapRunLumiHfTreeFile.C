void run_mapRunLumiHfTreeFile(int startFile, int endFile, char *filelist, int trg)
{
    //gSystem->Load("../../../../lib/slc5_amd64_gcc462/libUserCodeOpenHF.so");

    mapRunLumiHfTreeFile *a = new mapRunLumiHfTreeFile;

    if(trg==-2) { //.. hlt path in MinbiasUPC
        a->set_pathname("/mnt/hadoop/store/user/wxie/PAMinBiasUPC/HLT_WXie/b8385945bc123ff6dcc68d83210a3d7d");
        a->LoopOverFileHlt(startFile, endFile, filelist, "minbiasUPC_pPb_HLT");

    } else if(trg==-1) { //.. hlt path in PAHighPt
        a->set_pathname("/mnt/hadoop/store/user/wxie/PAHighPt/HLT_WXie/b8385945bc123ff6dcc68d83210a3d7d");
        a->LoopOverFileHlt(startFile, endFile, filelist, "highPt_pPb_HLT");

    } else if(trg==0) {
        a->set_pathname("/mnt/hadoop/store/user/wxie/PAHighPt/HFTree/7fdf5ec06bc167d7c907e513d3ce2df4");
        a->LoopOverFile(startFile, endFile, filelist, "highPt_pPb");
    } else if(trg==1) {
        a->set_pathname("/mnt/hadoop/store/user/wxie/PAMinBiasUPC/HFTree/64eab6850fc00640501789253e9a7af2");
        a->LoopOverFile(startFile, endFile, filelist, "minbiasUPC_pPb");
    } else if(trg==2) {
        a->set_pathname("/mnt/hadoop/store/user/wxie/HeavyFlavor_minbias_pPb");
        a->LoopOverFile(startFile, endFile, filelist, "minbias_pPb");
    } else if(trg==3) {
        a->set_pathname("/mnt/hadoop/store/user/wxie/HeavyFlavor_highPt_pp");
        a->LoopOverFile(startFile, endFile, filelist, "highPt_pp");
    } else if(trg==4) {
        a->set_pathname("/mnt/hadoop/store/user/wxie/HeavyFlavor_minbias_pp");
        a->LoopOverFile(startFile, endFile, filelist, "minbias_pp");
    }

    delete a; // so we can dump some information in the destructor
}
