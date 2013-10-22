/*
 *  HFTrackListBuilder.cpp
 *
 *  Created by Christoph Nägeli <christoph.naegeli@psi.ch> on 10.11.12.
 *
 */

#include <algorithm>

#include "UserCode/OpenHF/interface/HFTrackListBuilder.hh"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/TrackReco/interface/TrackBase.h"

#include "TrackingTools/PatternTools/interface/TwoTrackMinimumDistance.h"

using namespace std;

HFTrackListBuilder::HFTrackListBuilder(edm::Handle<edm::View<reco::Track> > &hTracks, const TransientTrackBuilder *ttb, int verbose) :
	fhTracks(hTracks),
	fTTB(ttb),
	fVerbose(verbose),
	fCallerName("HFTrackListBuilder"),
	fMaxD0(999.),
	fMaxDz(999.),
	fMinPt(-1.),
	fMaxDocaToTrks(999.),
	fTrackQuality("highPurity"),
	fCloseTracks(NULL)
{} // HFTrackListBuilder()


std::vector<int> HFTrackListBuilder::getTrackList()
{
	vector<int> trackList; // allocate capacity
	int ix;
	
	trackList.reserve(300);
	for (ix = 0; (unsigned)ix < fhTracks->size(); ix++) {
	  reco::TrackBaseRef rTrackView(fhTracks, ix);
	  const reco::Track trackView(*rTrackView);
	  if (!trackView.quality(reco::TrackBase::qualityByName(fTrackQuality))) continue;
	  
	  if ( !(*this)(ix)) 
	    trackList.push_back(ix);
	}
	
	return trackList;
} // getTrackList()

bool HFTrackListBuilder::operator()(int ix)
{
	reco::TrackBaseRef rTrackView(fhTracks,ix);
	reco::Track tTrack(*rTrackView);
	bool result = tTrack.d0() > fMaxD0 || tTrack.dz() > fMaxDz || tTrack.pt() < fMinPt;
	
	if (!result && fCloseTracks) {
		// check wether this track is nearby anyone in the fCloseTracks vector
		reco::TransientTrack tTrkCur = fTTB->build(tTrack);
		TwoTrackMinimumDistance md;
		double minDoca = DBL_MAX;
		size_t j;
		
		for (j = 0; j < fCloseTracks->size(); j++) {
			if ((*fCloseTracks)[j] == ix) // compare to itself
				continue;
			
			reco::TransientTrack tTrkCompare = fTTB->build((*fhTracks)[(*fCloseTracks)[j]]);
			md.calculate(tTrkCur.initialFreeState(),tTrkCompare.initialFreeState());
			if (md.distance() < minDoca)
				minDoca = md.distance();
		}
		
		result = minDoca > fMaxDocaToTrks;
	}
	
	return result;
} // operator(int ix);
