#ifndef _HFDs2PhiPi_h_
#define _HFDs2PhiPi_h_

#include "UserCode/OpenHF/plugins/HFVirtualDecay.h"

class HFDs2PhiPi : public HFVirtualDecay {
	public:
		explicit HFDs2PhiPi(const edm::ParameterSet&);

	protected:
		virtual void analyze(const edm::Event&, const edm::EventSetup&);
		virtual void dumpConfiguration();

		double fSlowPionPt; 
		double fPhiWindow;
};

#endif
