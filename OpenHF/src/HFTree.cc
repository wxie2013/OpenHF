#include "FWCore/Framework/interface/MakerMacros.h"
#include "UserCode/OpenHF/interface/HFTree.hh"

#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>

#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"

//.. used for MC matching 
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticleFwd.h"
#include "SimTracker/TrackAssociation/interface/TrackAssociatorBase.h"
#include "SimTracker/TrackAssociation/interface/TrackAssociatorByChi2.h"
#include "SimTracker/Records/interface/TrackAssociatorRecord.h"
#include "SimTracker/TrackAssociation/interface/TrackAssociatorByHits.h"
//

#include <TROOT.h>
#include <TTree.h>
#include <TH1.h>
#include <TCanvas.h>

#include "UserCode/OpenHF/interface/TAna01Event.hh"
#include "UserCode/OpenHF/interface/TAnaTrack.hh"
#include "UserCode/OpenHF/interface/TAnaCand.hh"
#include "UserCode/OpenHF/interface/TAnaVertex.hh"
#include "UserCode/OpenHF/interface/candAna.hh"

//reco -> sim matching
#include "HeavyIonsAnalysis/TrackAnalysis/interface/TrkAnalyzerUtils.h"

// -- Yikes!
TAna01Event  *gHFEvent;

using namespace::std;
using namespace::edm;

// ----------------------------------------------------------------------
HFTree::HFTree(const edm::ParameterSet& iConfig) :
    fReducedTree(iConfig.getUntrackedParameter<bool>("reducedTree", true)), 
    fRequireCand(iConfig.getUntrackedParameter<bool>("requireCand", true)), 
    fFullGenBlock(iConfig.getUntrackedParameter<bool>("fullGenBlock", false)), 
    fTreeName(iConfig.getUntrackedParameter<string>("treeName", string("T1"))), 
    fVerbose(iConfig.getUntrackedParameter<int>("verbose", 1)),
    fPrintFrequency(iConfig.getUntrackedParameter<int>("printFrequency", 1000)),
    isMC(iConfig.getUntrackedParameter<bool>("isMC", false)), 
    cut_ffls3d_in(iConfig.getUntrackedParameter<double>("cut_ffls3d_in", 2.0)), 
    cut_alpha0_in(iConfig.getUntrackedParameter<double>("cut_alpha0_in", 0.5)), 
    associateChi2(iConfig.getUntrackedParameter<bool>("associateChi2", false)), 
    trackSrc_(iConfig.getUntrackedParameter<InputTag>("tracksLabel", InputTag("generalTracks"))), 
    tpFakeSrc_(iConfig.getUntrackedParameter<InputTag>("tpFakeLabel", InputTag("mergedtruth", "MergedTrackTruth"))) 
{
        using namespace std;
        cout << "----------------------------------------------------------------------" << endl;
        cout << "--- HFTree constructor" << endl;
        cout << "---  verbose:                         " << fVerbose << endl;
        cout << "---  printFrequency:                  " << fPrintFrequency << endl;
        cout << "---  treeName:                        " << fTreeName << endl; 
        cout << "---  requireCand:                     " << (fRequireCand?"true":"false") << endl; 
        cout << "---  fullGenBlock:                    " << (fFullGenBlock?"true":"false") << endl; 
        cout << "---  reducedTree:                    " << (fReducedTree?"true":"false") << endl; 
        cout << "----------------------------------------------------------------------" << endl;


        fEvent = new TAna01Event(0);
        gHFEvent = fEvent;

        fEventCounter = -1; 
    }


// ----------------------------------------------------------------------
HFTree::~HFTree() {

}


// ----------------------------------------------------------------------
void HFTree::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {

    ++fEventCounter; 
    if (fVerbose > 0) {
        if (fEventCounter%fPrintFrequency == 0) {
            pid_t pid = getpid();
            char line[100]; 
            sprintf(line, "ps -F %i", pid); 
            cout << "==>HFTree: analyze() in event #" << fEventCounter 
                << "  run: " << gHFEvent->fRunNumber 
                << " event: "  << gHFEvent->fEventNumber
                << endl;
            system(line); 
        }
    }

    if(!fReducedTree) { //.. fill original tree
        if (fRequireCand){
            if (gHFEvent->nCands() > 0) {
                if (fVerbose > 1) {
                    cout << "HFTree> filling tree for run: " << gHFEvent->fRunNumber
                        << " event: "  << gHFEvent->fEventNumber ;

                    if (fVerbose > 2) {
                        cout << " Cand: ";

                        for (int i = 0; i < gHFEvent->nCands(); ++i) {
                            cout << gHFEvent->getCand(i)->fType << " ";
                        }
                    }
                    cout << endl;
                }
                fTree->Fill();
            } 
        } else {
            fTree->Fill();
        }
    } else {//.. fill reduced tree
        ana->hfcandidate->Reset();
        ana->evtAnalysis(gHFEvent);
        if(isMC) 
            FillCandMCInfo(ana->hfcandidate, iEvent, iSetup);
        ana->fillTree();
    }

    gHFEvent->Clear();
}

// ------------ method called once each job just before starting event loop  ------------
void  HFTree::beginJob() {
    edm::Service<TFileService> fs;
    if(!fReducedTree) {
        cout<<" creating HF original tree "<<endl; 
        fTree = fs->make<TTree>("fTree" , "heavy flavor original tree");
        fTree->Branch("TAna01Event", "TAna01Event", &fEvent, 256000/8, 1);
    } else {
        cout<<" creating HF reduced tree "<<endl; 
        fTree = fs->make<TTree>("hftree" , "heavy flavor reduced tree");
        ana = new candAna;
        ana->Init(fTree);

        cout<<" !! cut_ffls3d_in: "<<cut_ffls3d_in<<" cut_alpha0_in: "<<cut_alpha0_in<<endl;
        ana->set_cut(cut_ffls3d_in, cut_alpha0_in);
    }
}

// ------------ method called once each job just after ending the event loop  ------------
void  HFTree::endJob() {

    pid_t pid = getpid();
    char line[100]; 
    sprintf(line, "ps -F %i", pid); 
    cout << "==>HFTree: endJob():" << endl;
    system(line); 

}

//... fill MC information in hfcandidate
void HFTree::FillCandMCInfo(hfcand* hfcandidate, const edm::Event& iEvent, const edm::EventSetup& iSetup) 
{
    //.. need to call for each event
    edm::Handle<TrackingParticleCollection>  TPCollectionHfake;
    Handle<edm::View<reco::Track> >  trackCollection;
    iEvent.getByLabel(trackSrc_, trackCollection);
    ESHandle<TrackAssociatorBase> theAssociator;
    reco::RecoToSimCollection recSimColl;

    //annoyingly, you have to load in the tracks into two separate containers...
    Handle<vector<reco::Track> > etracks;
    iEvent.getByLabel(trackSrc_, etracks);

    iEvent.getByLabel(tpFakeSrc_,TPCollectionHfake);
    if(associateChi2){
        iSetup.get<TrackAssociatorRecord>().get("TrackAssociatorByChi2",theAssociator);
        const TrackAssociatorByChi2 *theAssociatorByChi2 = (const TrackAssociatorByChi2*) theAssociator.product();
        recSimColl= theAssociatorByChi2->associateRecoToSim(trackCollection,TPCollectionHfake,&iEvent);
    }else{
        iSetup.get<TrackAssociatorRecord>().get("TrackAssociatorByHits",theAssociator);
        const TrackAssociatorByHits *theAssociatorByHits = (const TrackAssociatorByHits*) theAssociator.product();
        recSimColl= theAssociatorByHits->associateRecoToSim(trackCollection,TPCollectionHfake,&iEvent);
    }
    //.. now do the matching to MC for each daughter
    const TrackingParticle* tparticle;
    reco::TrackRef trackRef;
    for(int i = 0; i<hfcandidate->get_nhfcand(); i++) {
        snglhfcand* cand = hfcandidate->get_hfcand(i);

        unsigned index_dau2 = (unsigned)cand->get_Index_dau2();
        trackRef=reco::TrackRef(etracks,index_dau2);
        tparticle = doRecoToTpMatch(recSimColl, trackRef);
        vector<int> tempD2 = matchTpToGen(iEvent, tparticle);
        for(unsigned int imatch=0; imatch<tempD2.size(); imatch++){
            cand->set_gIndex_dau2(tempD2);
        }

        unsigned index1 = (unsigned)cand->get_Index1();
        trackRef=reco::TrackRef(etracks,index1);
        tparticle = doRecoToTpMatch(recSimColl, trackRef);
        vector<int> tempI1 = matchTpToGen(iEvent, tparticle);
        for(unsigned int imatch=0; imatch<tempI1.size(); imatch++){
            cand->set_gIndex1(tempI1);
        }

        unsigned index2 = (unsigned)cand->get_Index2();
        trackRef=reco::TrackRef(etracks,index2);
        tparticle = doRecoToTpMatch(recSimColl, trackRef);
        vector<int> tempI2 = matchTpToGen(iEvent, tparticle);
        for(unsigned int imatch=0; imatch<tempI2.size(); imatch++){
            cand->set_gIndex2(tempI2);
        } 
    }
}
//define this as a plug-in
DEFINE_FWK_MODULE(HFTree);
