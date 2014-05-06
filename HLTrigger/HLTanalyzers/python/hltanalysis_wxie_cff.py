import FWCore.ParameterSet.Config as cms

hltbitanalysis = cms.EDAnalyzer("HLTBitAnalyzer",
    ### Trigger objects
    l1GctHFBitCounts                = cms.InputTag("hltGctDigis"),
    l1GctHFRingSums                 = cms.InputTag("hltGctDigis"),
    l1GtObjectMapRecord             = cms.InputTag("hltL1GtObjectMap::HLTwxie"),
    l1GtReadoutRecord               = cms.InputTag("hltGtDigis::HLTwxie"),

    l1extramc                       = cms.string('hltL1extraParticles'),
    l1extramu                       = cms.string('hltL1extraParticles'),
    hltresults                      = cms.InputTag("TriggerResults::HLTwxie"),
    HLTProcessName                  = cms.string("HLTwxie"),

    ### Run parameters
    RunParameters = cms.PSet(
    HistogramFile = cms.untracked.string('hltbitanalysis.root')
    )

)

hltbitanalysis.UseTFileService = cms.untracked.bool(True)

hltanalysis = hltbitanalysis.clone(
    dummyBranches = cms.untracked.vstring(
    ),
    
    l1GtReadoutRecord            = cms.InputTag("gtDigis"),
    l1GctHFBitCounts     = cms.InputTag("gctDigis"),
    l1GctHFRingSums      = cms.InputTag("gctDigis"),
    l1extramu            = cms.string('l1extraParticles'),
    l1extramc            = cms.string('l1extraParticles'),
    hltresults           = cms.InputTag("TriggerResults","","HLTwxie"),
    HLTProcessName       = cms.string("HLTwxie")
    )
