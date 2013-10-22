#include "HFDs2PhiPi.h"

#include "FWCore/Framework/interface/MakerMacros.h"

#include "UserCode/OpenHF/interface/TAna01Event.hh"

#include "UserCode/OpenHF/interface/HFMasses.hh"
#include "UserCode/OpenHF/interface/HFTwoParticleCombinatoricsNew.hh"

using std::cout;
using std::endl;

// -- Yikes!
extern TAna01Event *gHFEvent;

// ----------------------------------------------------------------------
HFDs2PhiPi::HFDs2PhiPi(const edm::ParameterSet& iConfig) :
    HFVirtualDecay(iConfig),
    fSlowPionPt(iConfig.getUntrackedParameter<double>("slowPionPt", 0.4)),
    fPhiWindow(iConfig.getUntrackedParameter<double>("PhiWindow", 0.1))
{
    dumpConfiguration();
} // HFDs2PhiPi()

void HFDs2PhiPi::dumpConfiguration()
{
    cout << "----------------------------------------------------------------------" << endl;
    cout << "--- HFDs2PhiPi constructor" << endl;
    HFVirtualDecay::dumpConfiguration();
    cout << "---  slowPionPt:               " << fSlowPionPt << endl;
    cout << "---  PhiWindow:                 " << fPhiWindow << endl;
    cout << "----------------------------------------------------------------------" << endl;
} // dumpConfiguration()

// ----------------------------------------------------------------------
void HFDs2PhiPi::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    typedef HFTwoParticleCombinatoricsNew::HFTwoParticleCombinatoricsSet HFTwoParticleCombinatoricsSet;

    try {
        HFVirtualDecay::analyze(iEvent,iSetup);
    }
    catch (HFSetupException e) {
        cout << "==>HFDs2PhiPi> " << e.fMsg << endl;
        return; // problem with setup
    }

    std::vector<int> trkFastList = fListBuilder->getTrackList();
    fListBuilder->setMinPt(fSlowPionPt);
    fListBuilder->setMaxDocaToTracks(fMaxDoca);
    fListBuilder->setCloseTracks(&trkFastList);
    std::vector<int> trkSlowList = fListBuilder->getTrackList();

    HFTwoParticleCombinatoricsNew a(fTracksHandle,fVerbose);
    HFTwoParticleCombinatoricsSet kakaSet = a.combine(trkFastList, MKAON, trkFastList, MKAON, MPHI-fPhiWindow,MPHI+fPhiWindow, 0);

    TLorentzVector ka1st,ka2nd,phi,pis,ds;
    for (HFTwoParticleCombinatoricsNew::iterator phiIt = kakaSet.begin(); phiIt != kakaSet.end(); ++phiIt) {


        unsigned int iKaon1st = phiIt->first;
        unsigned int iKaon2nd = phiIt->second;

        reco::TrackBaseRef kaon1stTrackView(fTracksHandle, iKaon1st);
        reco::Track tKaon1st(*kaon1stTrackView);
        ka1st.SetPtEtaPhiM(tKaon1st.pt(), tKaon1st.eta(), tKaon1st.phi(), MKAON);

        reco::TrackBaseRef Kaon2ndTrackView(fTracksHandle, iKaon2nd);
        reco::Track tKaon2nd(*Kaon2ndTrackView);
        ka2nd.SetPtEtaPhiM(tKaon2nd.pt(), tKaon2nd.eta(), tKaon2nd.phi(), MKAON);

        //.. k+k-: need to be different sign
        if (tKaon1st.charge()*tKaon2nd.charge() > 0) continue;

        phi = ka1st + ka2nd;

        for (unsigned iTrack = 0; iTrack < trkSlowList.size(); iTrack++) {

            if (iTrack == iKaon1st || iTrack == iKaon2nd) continue;
            reco::TrackBaseRef rTrackView(fTracksHandle, iTrack);
            reco::Track tSlowPion(*rTrackView);

            pis.SetXYZM(tSlowPion.px(), tSlowPion.py(), tSlowPion.pz(), MPION);
            if (phi.DeltaR(pis) > fDeltaR) continue;

            ds = phi + pis;

            if (TMath::Abs(ds.M() - MDS) > 0.2) continue;


            HFDecayTree theTree(fType, true, MDS, false, -1.0, true);
            HFDecayTreeIterator iterator = theTree.addDecayTree(102, true, MPHI, false);
            iterator->addTrack(iKaon1st,321);
            iterator->addTrack(iKaon2nd,321);
            iterator->setNodeCut(RefCountedHFNodeCut(new HFMaxDocaCut(fMaxDoca)));
            theTree.addTrack(iTrack,211);
            theTree.setNodeCut(RefCountedHFNodeCut(new HFMaxDocaCut(fMaxDoca)));

            fSequentialFitter->doFit(&theTree);
        }
    }
}

//define this as a plug-in
DEFINE_FWK_MODULE(HFDs2PhiPi);
