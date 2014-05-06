import os
import FWCore.ParameterSet.Config as cms

process = cms.Process("HFA")

# ----------------------------------------------------------------------
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'
#process.MessageLogger.cerr.FwkReport.reportEvery = 1000
process.MessageLogger.cerr.FwkReport.reportEvery = 1
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )


# ----------------------------------------------------------------------
# -- Database configuration
process.load("CondCore.DBCommon.CondDBCommon_cfi")
process.load("CondCore.DBCommon.CondDBSetup_cfi")

# -- Conditions
#process.load("Configuration.StandardSequences.MagneticField_38T_cff")
#process.load("Configuration.Geometry.GeometryIdeal_cff")
#process.load("RecoVertex.BeamSpotProducer.BeamSpot_cfi")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")

process.load("HLTrigger.HLTcore.hltEventAnalyzerAOD_cfi")

process.GlobalTag.globaltag = "GR_P_V43D::All"

# ----------------------------------------------------------------------
# -- Input files

process.source = cms.Source("PoolSource",
        skipEvents = cms.untracked.uint32(0),
        fileNames  = cms.untracked.vstring(
                'file:/home/wxie/CMSSW_5_3_8_HI/src/HLTrigger/HLTanalyzers/test/outputA.root'
            )
        )

process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(-1)
        )

# ----------------------------------------------------------------------
process.TFileService = cms.Service("TFileService",
                   fileName = cms.string('dmeson.root')
                           )
# ----------------------------------------------------------------------
process.p = cms.Path(process.hltEventAnalyzerAOD)

