#ifndef _HFD0_h_
#define _HFD0_h_

#include "UserCode/OpenHF/plugins/HFVirtualDecay.h"

class HFD0 : public HFVirtualDecay {
	public:
		explicit HFD0(const edm::ParameterSet&);

	protected:
		virtual void analyze(const edm::Event&, const edm::EventSetup&);
		virtual void dumpConfiguration();

		double fD0Window;
};

#endif
