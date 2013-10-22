#include "HFDpm2KaPiPi.h"

#include "FWCore/Framework/interface/MakerMacros.h"

#include "UserCode/OpenHF/interface/TAna01Event.hh"

#include "UserCode/OpenHF/interface/HFMasses.hh"
#include "UserCode/OpenHF/interface/HFTwoParticleCombinatoricsNew.hh"

using std::cout;
using std::endl;

// -- Yikes!
extern TAna01Event *gHFEvent;

// ----------------------------------------------------------------------
HFDpm2KaPiPi::HFDpm2KaPiPi(const edm::ParameterSet& iConfig) :
    HFVirtualDecay(iConfig),
    fSlowPionPt(iConfig.getUntrackedParameter<double>("slowPionPt", 0.4)),
    fDeltaM(iConfig.getUntrackedParameter<double>("deltaM", 0.03))
{
    dumpConfiguration();
} // HFDpm2KaPiPi()

void HFDpm2KaPiPi::dumpConfiguration()
{
    cout << "----------------------------------------------------------------------" << endl;
    cout << "--- HFDpm2KaPiPi constructor" << endl;
    HFVirtualDecay::dumpConfiguration();
    cout << "---  slowPionPt:               " << fSlowPionPt << endl;
    cout << "---  deltaM:                   " << fDeltaM << endl;
    cout << "----------------------------------------------------------------------" << endl;
} // dumpConfiguration()

// ----------------------------------------------------------------------
void HFDpm2KaPiPi::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    typedef HFTwoParticleCombinatoricsNew::HFTwoParticleCombinatoricsSet HFTwoParticleCombinatoricsSet;

    try {
        HFVirtualDecay::analyze(iEvent,iSetup);
    }
    catch (HFSetupException e) {
        cout << "==>HFDpm2KaPiPi> " << e.fMsg << endl;
        return; // problem with setup
    }

    std::vector<int> trkFastList = fListBuilder->getTrackList();
    fListBuilder->setMinPt(fSlowPionPt);
    fListBuilder->setMaxDocaToTracks(fMaxDoca);
    fListBuilder->setCloseTracks(&trkFastList);
    std::vector<int> trkSlowList = fListBuilder->getTrackList();

    HFTwoParticleCombinatoricsNew a(fTracksHandle,fVerbose);
    //.. now sure the mass range of (kpi)s-wave
    HFTwoParticleCombinatoricsSet kapiSet = a.combine(trkFastList, MKAON, trkFastList, MPION, 0, 5, 0);

    TLorentzVector ka,pi,kp,pis,dpm;
    for (HFTwoParticleCombinatoricsNew::iterator kpIt = kapiSet.begin(); kpIt != kapiSet.end(); ++kpIt) {

        unsigned int iKaon = kpIt->first;
        unsigned int iPion = kpIt->second;

        reco::TrackBaseRef kaonTrackView(fTracksHandle, iKaon);
        reco::Track tKaon(*kaonTrackView);
        ka.SetPtEtaPhiM(tKaon.pt(), tKaon.eta(), tKaon.phi(), MKAON);

        reco::TrackBaseRef pionTrackView(fTracksHandle, iPion);
        reco::Track tPion(*pionTrackView);
        pi.SetPtEtaPhiM(tPion.pt(), tPion.eta(), tPion.phi(), MPION);

        kp = ka + pi;

        //if (kp.M() < 0.5) continue; //.. speed up calculation. m(k+pi) >= m(k)+m(pi)

        for (unsigned iTrack = 0; iTrack < trkSlowList.size(); iTrack++) {

            if (iTrack == iKaon || iTrack == iPion) continue;
            reco::TrackBaseRef rTrackView(fTracksHandle, iTrack);
            reco::Track tSlowPion(*rTrackView);

            // -- the slow pion has the same charge like the fast pion
            if (tSlowPion.charge()*tPion.charge() < 0) continue;

            pis.SetXYZM(tSlowPion.px(), tSlowPion.py(), tSlowPion.pz(), MPION);
            if (kp.DeltaR(pis) > fDeltaR) continue;

            dpm = kp + pis;

            if (TMath::Abs(dpm.M() - MDPLUS) > 0.2) continue;


            HFDecayTree theTree(fType, true, MDPLUS, false, -1.0, true);

            //.. now sure the mass range of (kpi)s-wave: set it to zero
            HFDecayTreeIterator iterator = theTree.addDecayTree(100, true, 0, false);
            iterator->addTrack(iKaon,321);
            iterator->addTrack(iPion,211);
            iterator->setNodeCut(RefCountedHFNodeCut(new HFMaxDocaCut(fMaxDoca)));
            theTree.addTrack(iTrack,211);
            theTree.setNodeCut(RefCountedHFNodeCut(new HFMaxDocaCut(fMaxDoca)));

            fSequentialFitter->doFit(&theTree);
        }
    }
}

//define this as a plug-in
DEFINE_FWK_MODULE(HFDpm2KaPiPi);
