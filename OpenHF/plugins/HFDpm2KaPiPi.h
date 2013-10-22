#ifndef _HFDpm2KaPiPi_h_
#define _HFDpm2KaPiPi_h_

#include "UserCode/OpenHF/plugins/HFVirtualDecay.h"

class HFDpm2KaPiPi : public HFVirtualDecay {
	public:
		explicit HFDpm2KaPiPi(const edm::ParameterSet&);

	protected:
		virtual void analyze(const edm::Event&, const edm::EventSetup&);
		virtual void dumpConfiguration();

		double fSlowPionPt; 
		double fDeltaM;
};

#endif
