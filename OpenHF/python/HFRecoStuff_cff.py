import FWCore.ParameterSet.Config as cms

# ----------------------------------------------------------------------
stuffDump = cms.EDAnalyzer(
    "HFDumpStuff",
    verbose                  = cms.untracked.int32(0),
    PrimaryVertexLabel       = cms.untracked.InputTag("offlinePrimaryVertices"),
    PrimaryVertexTracksLabel = cms.untracked.InputTag("generalTracks")
    )


# ----------------------------------------------------------------------
from TrackingTools.TransientTrack.TransientTrackBuilder_cfi import *
from SimTracker.TrackAssociation.TrackAssociatorByHits_cfi import *


# ######################################################################
# Sequences
# ######################################################################
recoStuffSequence     = cms.Sequence(stuffDump)
