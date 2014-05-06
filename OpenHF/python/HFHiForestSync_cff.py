import FWCore.ParameterSet.Config as cms

OpenHfTree = cms.EDAnalyzer(
        "HFHiForestSync", 
        nameOfMapRunLumiFile = cms.untracked.string("mapRunLumiHfTreeFile_highPt_pPb.root"),
        pathOfhftree = cms.untracked.string("root://xrootd.rcac.purdue.edu//store/user/wxie/PAHighPt/HFTree/7fdf5ec06bc167d7c907e513d3ce2df4"), 
        #nameOfMapRunLumiFile = cms.untracked.string("mapRunLumiHfTreeFile_minbiasUPC_pPb.root"),
        #pathOfhftree = cms.untracked.string("root://xrootd.rcac.purdue.edu//store/user/wxie/PAMinBiasUPC/HFTree/d32c37ab268e1492812aecf1e46c856e"), 
	#
        #nameOfMapRunLumiFile = cms.untracked.string("mapRunLumiHfTreeFile_minbias_pPb.root"),
        #pathOfhftree = cms.untracked.string("root://xrootd.rcac.purdue.edu//store/user/wxie/HeavyFlavor_minbias_pPb"), 
	#
        #nameOfMapRunLumiFile = cms.untracked.string("mapRunLumiHfTreeFile_minbias_pp.root"),
        #pathOfhftree = cms.untracked.string("root://xrootd.rcac.purdue.edu//store/user/wxie/HeavyFlavor_minbias_pp"), 
        cut_ffls3d = cms.untracked.double(2), 
        cut_falpha0 = cms.untracked.double(0.5)
        )
