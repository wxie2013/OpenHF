#ifndef __HFCAND_V0_H
#define __HFCAND_V0_H

#include "hfcand.hh"

#include "snglhfcand_v0.hh"

class TClonesArray;

class hfcand_v0 : public hfcand
{
    private:
        TClonesArray *GetCand() const {return HFcand;}
        unsigned int nhfcand;
        TClonesArray *HFcand;

        long int runNum;
        long int evtNum;

    public:
        hfcand_v0();
        ~hfcand_v0();

        void Reset();

        void set_nhfcand  (const unsigned int in) {nhfcand = in;}
        int  get_nhfcand () const {return nhfcand;}

        void set_runNum(const long int in) {runNum = in;}
        void set_evtNum(const long int in) {evtNum = in;}

        long int get_runNum() {return runNum;}
        long int get_evtNum() {return evtNum;}


        // Routines to manipulate the particle array...
        void AddCand          (const unsigned int itrk);
        snglhfcand* get_hfcand(const unsigned int itrk) const;

        ClassDef(hfcand_v0,1)
};

#endif 






