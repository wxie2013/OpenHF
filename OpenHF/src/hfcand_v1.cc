#include "TClonesArray.h"
#include "../interface/hfcand_v1.hh"

ClassImp(hfcand_v1)

using namespace std;

static const unsigned int NCAND = 0;

    //_________________
hfcand_v1::hfcand_v1()
{
    HFcand  = new TClonesArray("snglhfcand_v1",NCAND);
    Reset();
}

//_________________
hfcand_v1::~hfcand_v1()
{
    HFcand->Clear("c");
    return;
}

//_________________
snglhfcand* hfcand_v1::get_hfcand (const unsigned int itrk) const 
{
    snglhfcand_v1 *Particle = (snglhfcand_v1 *) GetCand()->UncheckedAt(itrk);
    return Particle;
}

//_________________
void hfcand_v1::Reset()
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
void  hfcand_v1::AddCand(const unsigned int itrk)
{
    TClonesArray &Particle = *HFcand;
    new(Particle[itrk]) snglhfcand_v1();
    return;
}
