import FWCore.ParameterSet.Config as cms

hltReRun = cms.EDAnalyzer(
        "hltHiForestSync", 
        nameOfMapRunLumiFile = cms.untracked.string("mapRunLumiHfTreeFile_highPt_pPb_HLT.root"),
        pathOfhftree = cms.untracked.string("root://xrootd.rcac.purdue.edu//store/user/wxie/PAHighPt/HLT_WXie/b8385945bc123ff6dcc68d83210a3d7d"), 
        )
