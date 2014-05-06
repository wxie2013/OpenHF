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
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load("Configuration.Geometry.GeometryIdeal_cff")
process.load("RecoVertex.BeamSpotProducer.BeamSpot_cfi")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = "GR_P_V43D::All"

# ----------------------------------------------------------------------
# -- Input files

process.source = cms.Source("PoolSource",
        skipEvents = cms.untracked.uint32(0),
        fileNames  = cms.untracked.vstring(
                #'file:/afs/cern.ch/work/w/wxie/public/2A54599E-DB74-E211-9D29-5404A63886AD.root'
                '/store/hidata/HIRun2013/PAHighPt/RECO/PromptReco-v1/000/211/032/00000/3CEE6CB0-626C-E211-9E31-003048D3C982.root'
            )
        )

process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(-1)
        )

rootFileName = "test.root"

process.load("UserCode.OpenHF.HFHiForestSync_cff")

# ----------------------------------------------------------------------
process.TFileService = cms.Service("TFileService",
                   fileName = cms.string('dmeson.root')
                           )
# ----------------------------------------------------------------------
process.p = cms.Path(process.OpenHfTree)

