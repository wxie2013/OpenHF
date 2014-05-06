#include <iostream>

//#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticle.h"
#include "SimTracker/TrackAssociation/interface/TrackAssociatorBase.h" 
#include "SimTracker/Records/interface/TrackAssociatorRecord.h"
#include "SimTracker/TrackAssociation/interface/TrackAssociatorByChi2.h"
#include "SimTracker/TrackAssociation/interface/TrackAssociatorByHits.h"
#include "SimDataFormats/Track/interface/SimTrack.h"
#include "SimDataFormats/Track/interface/SimTrackContainer.h"

#include "TrackingTools/GeomPropagators/interface/AnalyticalImpactPointExtrapolator.h"

#include "DataFormats/TrackerRecHit2D/interface/SiTrackerGSRecHit2D.h" 

#include "DataFormats/TrackReco/interface/TrackExtraFwd.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"

#include "DataFormats/TrackReco/interface/DeDxData.h"
#include "DataFormats/TrackReco/interface/HitPattern.h"
#include "DataFormats/TrackingRecHit/interface/TrackingRecHit.h"


#include "UserCode/OpenHF/interface/HFDumpUtilities.hh"

#include "UserCode/OpenHF/interface/TAna01Event.hh"

using namespace std;
using namespace edm;
using namespace reco;

// -- Yikes!
extern TAna01Event *gHFEvent;

// ----------------------------------------------------------------------
void fillAnaTrack(TAnaTrack *pTrack, const reco::Track &trackView, int tidx, 
        const reco::VertexCollection *vc, const reco::BeamSpot *bs) {

    math::XYZPoint refPt; 
    double xE(0.), yE(0), zE(0.);
    int pvIdx = getPv(tidx, vc);
    //  cout << "aT " << tidx << " setting pv index: " << pvIdx << endl;
    if (pvIdx > -1) {
        refPt = vc->at(pvIdx).position();
        xE = vc->at(pvIdx).xError();
        yE = vc->at(pvIdx).yError();
        zE = vc->at(pvIdx).zError();
    } else {
        if (bs) {
            refPt = bs->position(); 
            xE = bs->BeamWidthX();
            yE = bs->BeamWidthY();
            zE = bs->sigmaZ();
        } else {
            xE = 99.;
            yE = 99.;
            zE = 99.;
        } 
    }

    pTrack->fIndex = tidx;
    pTrack->fPlab.SetPtEtaPhi(trackView.pt(),
            trackView.eta(),
            trackView.phi()
            );
    pTrack->fPtE   = trackView.ptError();
    pTrack->fPhiE  = trackView.phiError();
    pTrack->fEtaE  = trackView.etaError();

    pTrack->fTip   = trackView.dxy(refPt);
    pTrack->fTipE  = sqrt(trackView.d0Error()*trackView.d0Error() + 0.5*xE*xE + 0.5*yE*yE); 

    pTrack->fLip   = trackView.dz(refPt); 
    pTrack->fLipE  = sqrt(trackView.dzError()*trackView.dzError() + zE*zE); 
    pTrack->fPvIdx = pvIdx;

    pTrack->fdxy  = trackView.dxy();
    pTrack->fdxyE = trackView.dxyError();
    pTrack->fd0   = trackView.d0();
    pTrack->fd0E  = trackView.d0Error();
    pTrack->fdz   = trackView.dz();
    pTrack->fdzE  = trackView.dzError();
    pTrack->fdsz  = trackView.dsz();
    pTrack->fdszE = trackView.dszError();

    pTrack->fQ = trackView.charge();
    pTrack->fChi2 = trackView.chi2();
    pTrack->fDof = int(trackView.ndof());
    pTrack->fValidHits = trackView.numberOfValidHits();  
    pTrack->fAlgorithm = trackView.algo(); 

    // -- see https://indico.cern.ch/getFile.py/access?contribId=2&resId=1&materialId=slides&confId=123067
    pTrack->fValidHitFraction = trackView.validFraction();
    pTrack->fLayersWithHits   = trackView.hitPattern().trackerLayersWithMeasurement();


    // -- from: RecoBTag/TrackProbability/src/TrackClassFilter.cc
    reco::TrackBase::TrackQuality trackQualityUndef               =  reco::TrackBase::qualityByName("undefQuality");
    reco::TrackBase::TrackQuality trackQualityLoose               =  reco::TrackBase::qualityByName("loose");
    reco::TrackBase::TrackQuality trackQualityTight               =  reco::TrackBase::qualityByName("tight");
    reco::TrackBase::TrackQuality trackQualityhighPur             =  reco::TrackBase::qualityByName("highPurity");
    reco::TrackBase::TrackQuality trackQualityConfirmed           =  reco::TrackBase::qualityByName("confirmed");
    reco::TrackBase::TrackQuality trackQualityGoodIterative       =  reco::TrackBase::qualityByName("goodIterative");
    reco::TrackBase::TrackQuality trackQualityLooseSetWithPV      =  reco::TrackBase::qualityByName("looseSetWithPV");
    reco::TrackBase::TrackQuality trackQualityHighPuritySetWithPV =  reco::TrackBase::qualityByName("highPuritySetWithPV");

    int trakQuality  = 0;
    if (trackView.quality(trackQualityUndef))               trakQuality |= 0x1<<10;
    if (trackView.quality(trackQualityLoose))               trakQuality |= 0x1<<0;
    if (trackView.quality(trackQualityTight))               trakQuality |= 0x1<<1;
    if (trackView.quality(trackQualityhighPur))             trakQuality |= 0x1<<2;
    if (trackView.quality(trackQualityConfirmed))           trakQuality |= 0x1<<3;
    if (trackView.quality(trackQualityGoodIterative))       trakQuality |= 0x1<<4;
    if (trackView.quality(trackQualityLooseSetWithPV))      trakQuality |= 0x1<<5;
    if (trackView.quality(trackQualityHighPuritySetWithPV)) trakQuality |= 0x1<<6;
    pTrack->fTrackQuality = trakQuality; 


    // -- hits of the track
    const reco::HitPattern& p = trackView.hitPattern();
    for (int i=0; i<p.numberOfHits(); i++) {
        uint32_t hit = p.getHitPattern(i);
        if (i < 20) pTrack->fHitPattern[i] = hit; 
    }
}


// ----------------------------------------------------------------------
int getPv(int tidx, const reco::VertexCollection *vc) {
    if (vc) {
        for (unsigned int i = 0; i < vc->size(); ++i) {
            Vertex::trackRef_iterator v1TrackIter;
            Vertex::trackRef_iterator v1TrackBegin = vc->at(i).tracks_begin();
            Vertex::trackRef_iterator v1TrackEnd   = vc->at(i).tracks_end();
            for (v1TrackIter = v1TrackBegin; v1TrackIter != v1TrackEnd; v1TrackIter++) {
                if (static_cast<unsigned int>(tidx) == v1TrackIter->key()) return i;
            }
        }
    }
    return -1; 
}

