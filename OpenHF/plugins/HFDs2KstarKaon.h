#ifndef _HFDs2KstarKaon_h_
#define _HFDs2KstarKaon_h_

#include "UserCode/OpenHF/plugins/HFVirtualDecay.h"

class HFDs2KstarKaon : public HFVirtualDecay {
	public:
		explicit HFDs2KstarKaon(const edm::ParameterSet&);

	protected:
		virtual void analyze(const edm::Event&, const edm::EventSetup&);
		virtual void dumpConfiguration();

		double fSlowKaonPt; 
		double fKstarWindow;
		double fDeltaM;
};

#endif
