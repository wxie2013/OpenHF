#include "HFDs2KstarKaon.h"

#include "FWCore/Framework/interface/MakerMacros.h"

#include "UserCode/OpenHF/interface/TAna01Event.hh"

#include "UserCode/OpenHF/interface/HFMasses.hh"
#include "UserCode/OpenHF/interface/HFTwoParticleCombinatoricsNew.hh"

using std::cout;
using std::endl;

// -- Yikes!
extern TAna01Event *gHFEvent;

// ----------------------------------------------------------------------
HFDs2KstarKaon::HFDs2KstarKaon(const edm::ParameterSet& iConfig) :
    HFVirtualDecay(iConfig),
    fSlowKaonPt(iConfig.getUntrackedParameter<double>("slowPionPt", 0.4)),
    fKstarWindow(iConfig.getUntrackedParameter<double>("KstarWindow", 0.1)),
    fDeltaM(iConfig.getUntrackedParameter<double>("deltaM", 0.03))
{
    dumpConfiguration();
} // HFDs2KstarKaon()

void HFDs2KstarKaon::dumpConfiguration()
{
    cout << "----------------------------------------------------------------------" << endl;
    cout << "--- HFDs2KstarKaon constructor" << endl;
    HFVirtualDecay::dumpConfiguration();
    cout << "---  slowPionPt:               " << fSlowKaonPt << endl;
    cout << "---  KstarWindow:                 " << fKstarWindow << endl;
    cout << "---  deltaM:                   " << fDeltaM << endl;
    cout << "----------------------------------------------------------------------" << endl;
} // dumpConfiguration()

// ----------------------------------------------------------------------
void HFDs2KstarKaon::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    typedef HFTwoParticleCombinatoricsNew::HFTwoParticleCombinatoricsSet HFTwoParticleCombinatoricsSet;

    try {
        HFVirtualDecay::analyze(iEvent,iSetup);
    }
    catch (HFSetupException e) {
        cout << "==>HFDs2KstarKaon> " << e.fMsg << endl;
        return; // problem with setup
    }

    std::vector<int> trkFastList = fListBuilder->getTrackList();
    fListBuilder->setMinPt(fSlowKaonPt);
    fListBuilder->setMaxDocaToTracks(fMaxDoca);
    fListBuilder->setCloseTracks(&trkFastList);
    std::vector<int> trkSlowList = fListBuilder->getTrackList();

    HFTwoParticleCombinatoricsNew a(fTracksHandle,fVerbose);
    HFTwoParticleCombinatoricsSet kapiSet = a.combine(trkFastList, MKAON, trkFastList, MPION, MKSTAR-fKstarWindow,MKSTAR+fKstarWindow, 0);

    TLorentzVector ka,pi,kstar,kas,ds;
    for (HFTwoParticleCombinatoricsNew::iterator kstarIt = kapiSet.begin(); kstarIt != kapiSet.end(); ++kstarIt) {

        unsigned int iKaon = kstarIt->first;
        unsigned int iPion = kstarIt->second;

        reco::TrackBaseRef kaonTrackView(fTracksHandle, iKaon);
        reco::Track tKaon(*kaonTrackView);
        ka.SetPtEtaPhiM(tKaon.pt(), tKaon.eta(), tKaon.phi(), MKAON);

        reco::TrackBaseRef pionTrackView(fTracksHandle, iPion);
        reco::Track tPion(*pionTrackView);
        pi.SetPtEtaPhiM(tPion.pt(), tPion.eta(), tPion.phi(), MPION);

        //.. K and pi have different charge signs
        if (tKaon.charge()*tPion.charge() > 0) continue;


        kstar = ka + pi;

        for (unsigned iTrack = 0; iTrack < trkSlowList.size(); iTrack++) {

            if (iTrack == iKaon || iTrack == iPion) continue;
            reco::TrackBaseRef rTrackView(fTracksHandle, iTrack);
            reco::Track tSlowKaon(*rTrackView);

            // -- the pion has the same charge like the pion
            if (tSlowKaon.charge()*tPion.charge() < 0) continue;

            kas.SetXYZM(tSlowKaon.px(), tSlowKaon.py(), tSlowKaon.pz(), MKAON);
            if (kstar.DeltaR(kas) > fDeltaR) continue;

            ds = kstar + kas;

            if (TMath::Abs(ds.M() - MDS) > 0.2) continue;


            HFDecayTree theTree(fType, true, MDS, false, -1.0, true);
            HFDecayTreeIterator iterator = theTree.addDecayTree(101, true, MKSTAR, false);
            iterator->addTrack(iKaon,321);
            iterator->addTrack(iPion,211);
            iterator->setNodeCut(RefCountedHFNodeCut(new HFMaxDocaCut(fMaxDoca)));
            theTree.addTrack(iTrack,321);
            theTree.setNodeCut(RefCountedHFNodeCut(new HFMaxDocaCut(fMaxDoca)));

            fSequentialFitter->doFit(&theTree);
        }
    }
}

//define this as a plug-in
DEFINE_FWK_MODULE(HFDs2KstarKaon);
