import os
import FWCore.ParameterSet.Config as cms

process = cms.Process("HFA")

# breakdown time cost
#process.Timing = cms.Service("Timing");

# ----------------------------------------------------------------------
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )


# ----------------------------------------------------------------------
# -- Database configuration
process.load("CondCore.DBCommon.CondDBCommon_cfi")
process.load("CondCore.DBCommon.CondDBSetup_cfi")

# -- Conditions
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load("Configuration.Geometry.GeometryIdeal_cff")
process.load("RecoVertex.BeamSpotProducer.BeamSpot_cfi")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = "GR_P_V43D::All"

# ----------------------------------------------------------------------
# -- Input files

process.source = cms.Source("PoolSource",
        skipEvents = cms.untracked.uint32(0),
	fileNames = cms.untracked.vstring("file:/home/wxie/data/1485AF8A-E16B-E211-85AD-001D09F248F8.root")
        )

process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(100)
        )

#rootFileName = "HeavyFlavor.root"

process.tree = cms.EDAnalyzer(
    "HFTree",
    verbose      = cms.untracked.int32(1),
    printFrequency = cms.untracked.int32(1),
    requireCand  =  cms.untracked.bool(True),
    fReducedTree  =  cms.untracked.bool(True)
    )

# ----------------------------------------------------------------------
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load("UserCode.OpenHF.HFRecoStuff_cff")
process.load("UserCode.OpenHF.HFCharm_cff")

# ----------------------------------------------------------------------
process.TFileService = cms.Service("TFileService",
           fileName = cms.string('HeavyFlavor.root')
        )

# ----------------------------------------------------------------------
process.p = cms.Path(
    process.recoStuffSequence*
    process.charmSequence*
    process.tree
)
