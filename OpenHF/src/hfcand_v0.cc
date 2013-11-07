#include "TClonesArray.h"
#include "../interface/hfcand_v0.hh"

ClassImp(hfcand_v0)

    using namespace std;

    static const unsigned int NCAND = 0;

    //_________________
hfcand_v0::hfcand_v0()
{
    HFcand  = new TClonesArray("snglhfcand_v0",NCAND);
    Reset();
}

//_________________
hfcand_v0::~hfcand_v0()
{
    HFcand->Clear("c");
    return;
}

//_________________
snglhfcand* hfcand_v0::get_hfcand (const unsigned int itrk) const 
{
    snglhfcand_v0 *Particle = (snglhfcand_v0 *) GetCand()->UncheckedAt(itrk);
    return Particle;
}

//_________________
void hfcand_v0::Reset()
{
    HFcand->Clear("c");
    if (nhfcand>NCAND)
    {
        HFcand->Expand(NCAND); //.. reset to the orignal value
    }
    nhfcand = 0;
    runNum = -1;
    evtNum = -1;

    return;
}

//_____________________________________
void  hfcand_v0::AddCand(const unsigned int itrk)
{
    TClonesArray &Particle = *HFcand;
    new(Particle[itrk]) snglhfcand_v0();
    return;
}
