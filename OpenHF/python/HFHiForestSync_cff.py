import FWCore.ParameterSet.Config as cms

OpenHfTree = cms.EDAnalyzer(
     "HFHiForestSync", 
     nameOfMapRunLumiFile = cms.untracked.string("mapRunLumiHfTreeFile.root"),
     pathOfhftree = cms.untracked.string("root://xrootd.rcac.purdue.edu//store/user/wxie/PAHighPt/HFTree/7fdf5ec06bc167d7c907e513d3ce2df4")
     )
