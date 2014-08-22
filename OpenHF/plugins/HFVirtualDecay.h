/*
 *  HFVirtualDecay.h
 *
 *  Created by Christoph Nägeli <christoph.naegeli@psi.ch> on 31.01.13.
 *
 */

#ifndef HFVIRTUALDECAY_H
#define HFVIRTUALDECAY_H

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "MagneticField/Engine/interface/MagneticField.h"

#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"

#include "UserCode/OpenHF/interface/HFTrackListBuilder.hh"
#include "UserCode/OpenHF/interface/HFSequentialVertexFit.hh"

struct HFSetupException {
	HFSetupException(const char* msg) : fMsg(msg) {}
	std::string fMsg;
};

class HFVirtualDecay : public edm::EDAnalyzer {
	
	public:
		explicit HFVirtualDecay(const edm::ParameterSet& iConfig);
	
	protected:
		virtual void analyze(const edm::Event &iEvent, const edm::EventSetup &iSetup);
		
		virtual void dumpConfiguration();
	protected:
		// configuration
		int fVerbose;
		
		edm::InputTag fTracksLabel;
		std::string fTrackQualityString;
		edm::InputTag fPrimaryVertexLabel;
		edm::InputTag fBeamSpotLabel;
		
		double fTrackPt;
		double fDeltaR;
		double fMaxDoca;
		double fMaxD0;
		double fMaxDz;
		double fPvWeight;
		
		int fType;

        bool isPbPb;
	
	protected:
		// data for subclasses
		const MagneticField *fMagneticField;
		reco::VertexCollection fVertexCollection;
		reco::BeamSpot fBeamSpot;
		
		edm::Handle<edm::View<reco::Track> > fTracksHandle;
		edm::ESHandle<TransientTrackBuilder> fTTB;
		
		// track list builder
		std::auto_ptr<HFTrackListBuilder> fListBuilder;
		std::auto_ptr<HFSequentialVertexFit> fSequentialFitter;
};

#endif
