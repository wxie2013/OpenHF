#include "HFD0.h"

#include "FWCore/Framework/interface/MakerMacros.h"

#include "UserCode/OpenHF/interface/TAna01Event.hh"

#include "UserCode/OpenHF/interface/HFMasses.hh"
#include "UserCode/OpenHF/interface/HFTwoParticleCombinatoricsNew.hh"

using std::cout;
using std::endl;

// -- Yikes!
extern TAna01Event *gHFEvent;

// ----------------------------------------------------------------------
HFD0::HFD0(const edm::ParameterSet& iConfig) :
    HFVirtualDecay(iConfig),
    fD0Window(iConfig.getUntrackedParameter<double>("D0Window", 0.1))
{
    dumpConfiguration();
} // HFD0()

void HFD0::dumpConfiguration()
{
    cout << "----------------------------------------------------------------------" << endl;
    cout << "--- HFD0 constructor" << endl;
    HFVirtualDecay::dumpConfiguration();
    cout << "---  D0Window:                 " << fD0Window << endl;
    cout << "----------------------------------------------------------------------" << endl;
} // dumpConfiguration()

// ----------------------------------------------------------------------
void HFD0::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    typedef HFTwoParticleCombinatoricsNew::HFTwoParticleCombinatoricsSet HFTwoParticleCombinatoricsSet;

    try {
        HFVirtualDecay::analyze(iEvent,iSetup);
    }
    catch (HFSetupException e) {
        cout << "==>HFD0> " << e.fMsg << endl;
        return; // problem with setup
    }

    std::vector<int> trkFastList = fListBuilder->getTrackList();

    HFTwoParticleCombinatoricsNew a(fTracksHandle,fVerbose);
    HFTwoParticleCombinatoricsSet kapiSet = a.combine(trkFastList, MKAON, trkFastList, MPION, MD0-fD0Window,MD0+fD0Window, 0);

    TLorentzVector ka,pi,d0;
    for (HFTwoParticleCombinatoricsNew::iterator d0It = kapiSet.begin(); d0It != kapiSet.end(); ++d0It) {

        unsigned int iKaon = d0It->first;
        unsigned int iPion = d0It->second;

        reco::TrackBaseRef kaonTrackView(fTracksHandle, iKaon);
        reco::Track tKaon(*kaonTrackView);
        ka.SetPtEtaPhiM(tKaon.pt(), tKaon.eta(), tKaon.phi(), MKAON);

        reco::TrackBaseRef pionTrackView(fTracksHandle, iPion);
        reco::Track tPion(*pionTrackView);
        pi.SetPtEtaPhiM(tPion.pt(), tPion.eta(), tPion.phi(), MPION);

        if( tKaon.charge()*tPion.charge() > 0 ) continue;

        d0 = ka + pi;

        HFDecayTree theTree(fType, true, MD0, false, -1.0, true);
        theTree.addTrack(iKaon,321);
        theTree.addTrack(iPion,211);
        theTree.setNodeCut(RefCountedHFNodeCut(new HFMaxDocaCut(fMaxDoca)));

        fSequentialFitter->doFit(&theTree);
    }
}

//define this as a plug-in
DEFINE_FWK_MODULE(HFD0);
