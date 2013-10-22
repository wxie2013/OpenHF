/*
 *  HFTrackListBuilder.hh
 *
 *  Created by Christoph Nägeli <christoph.naegeli@psi.ch> on 10.11.12.
 *
 */

#ifndef HFTRACKLISTBUILDER_H
#define HFTRACKLISTBUILDER_H

#include <vector>
#include <string>

#include <TLorentzVector.h>

#include "FWCore/Framework/interface/Event.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/Common/interface/View.h"

#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"

class HFTrackListBuilder {
	
	public:
		HFTrackListBuilder(edm::Handle<edm::View<reco::Track> > &hTracks, const TransientTrackBuilder *ttb, int verbose);
		
		std::vector<int> getTrackList();
		
		// accessor functions
		void setMaxD0(double maxD0) { fMaxD0 = maxD0; }
		void setMaxDz(double maxDz) { fMaxDz = maxDz; }
		void setMinPt(double minPt) { fMinPt = minPt; }
        void setTrackQuality(std::string t) { fTrackQuality = t; }
		void setMaxDocaToTracks(double docaToTrks) { fMaxDocaToTrks = docaToTrks; }
		void setCloseTracks(std::vector<int> *closeTracks) { fCloseTracks = closeTracks; }
		void setCallerName(const char *callerName) { fCallerName = std::string(callerName); }
		
	public:
		// for STL
		bool operator()(int ix);
		
	private:
		edm::Handle<edm::View<reco::Track> > &fhTracks;
		const TransientTrackBuilder *fTTB;
		
		int fVerbose;
	
	private:
		// cuts applied to the tracks
		std::string fCallerName;
		double fMaxD0;
		double fMaxDz;
		double fMinPt;
		double fMaxDocaToTrks;
                std::string fTrackQuality; 
		std::vector<int> *fCloseTracks;
};

#endif
