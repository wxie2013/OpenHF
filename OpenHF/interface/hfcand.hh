#ifndef __HFCAND_H
#define __HFCAND_H

#include "TNamed.h"
#include "snglhfcand.hh"
#include <iostream>
using namespace std;

class TClonesArray;

class hfcand : public TNamed
{
  public:
    virtual void Reset() {WarningMSG();}

    virtual void set_nhfcand(const unsigned int in) {WarningMSG();}
    virtual int  get_nhfcand() const {WarningMSG(); return 0;}

    virtual void set_runNum(const long int in) {WarningMSG();}
    virtual void set_evtNum(const long int in) {WarningMSG();}

    virtual long int get_runNum() {WarningMSG(); return 0;}
    virtual long int get_evtNum() {WarningMSG(); return 0;}

    // Routines to manipulate the particle array...
    virtual void AddCand(const unsigned int itrk) {WarningMSG();}
    virtual snglhfcand* get_hfcand(const unsigned int itrk) const {WarningMSG(); return 0;}

  private:
    void WarningMSG() const {cout<<"!!!!hfcand: not implemented in daughter class"<<endl;}

  ClassDef(hfcand,1)
};

#endif 
