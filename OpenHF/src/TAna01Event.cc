#include "../interface/TAna01Event.hh"
#include <iostream>
#include <stdexcept>

ClassImp(TAna01Event)

using namespace std;

#define NGEN 1000
#define NGENCAND 100000
#define NRECTRACK 50000
#define NSIMPLETRACK 50000
#define NMUON 1000
#define NTRGOBJ 1000
#define NSIGTRACK 500000
#define NCALOJET 1000
#define NGENJET 1000
#define NTRACKJET 1000
#define NCANDIDATES 200000
#define NVERTEX 1000

// ----------------------------------------------------------------------
TAna01Event::TAna01Event(Int_t Option) {

    fSigTracks       = new TClonesArray("TAnaTrack", NSIGTRACK);
    fnSigTracks      = 0;

    fCandidates      = new TClonesArray("TAnaCand", NCANDIDATES);
    fnCandidates     = 0;

    fPV              = new TClonesArray("TAnaVertex", NVERTEX);
    fnPV             = 0;

    Clear();

}

// ----------------------------------------------------------------------
void TAna01Event::Clear(Option_t *option) {

    fLumi = fLumiInt = fLumiSection = fBx = fOrbit = -9999; 
    fTimeLo = fTimeHi = 0; 

    TAnaTrack *pSigTrack;
    for (int i = 0; i < fnSigTracks; i++) {
        pSigTrack = getSigTrack(i);
        pSigTrack->clear();
    }
    fSigTracks->Clear(option);
    fnSigTracks = 0;

    TAnaCand *pCand;
    for (int i = 0; i < fnCandidates; i++) {
        pCand = getCand(i);
        pCand->~TAnaCand(); // call destructor, not only clear(). Prevents memleak due to std::vector
    }
    fCandidates->Clear(option);
    fnCandidates = 0;

    TAnaVertex *pPV;
    for (int i = 0; i < fnPV; i++) {
        pPV = getPV(i);
        pPV->clear();
    }
    fPV->Clear(option);
    fnPV = 0;

}

// ----------------------------------------------------------------------
TAnaTrack* TAna01Event::getSigTrack(Int_t n) { 
    return (TAnaTrack*)fSigTracks->UncheckedAt(n); 
}

// ----------------------------------------------------------------------
TAnaTrack* TAna01Event::addSigTrack() {
    TClonesArray& d = *fSigTracks; 
    new(d[d.GetLast()+1]) TAnaTrack(fnSigTracks);
    ++fnSigTracks;
    return (TAnaTrack*)d[d.GetLast()];
}


// ----------------------------------------------------------------------
TAnaCand* TAna01Event::getCand(Int_t n) { 
    return (TAnaCand*)fCandidates->UncheckedAt(n); 
}

// ----------------------------------------------------------------------
TAnaCand* TAna01Event::addCand() {
    TClonesArray& d = *fCandidates; 
    new(d[d.GetLast()+1]) TAnaCand(fnCandidates);
    ++fnCandidates;
    return (TAnaCand*)d[d.GetLast()];
}

// ----------------------------------------------------------------------
TAnaVertex* TAna01Event::getPV(Int_t n) { 
    return (TAnaVertex*)fPV->UncheckedAt(n); 
}

// ----------------------------------------------------------------------
TAnaVertex* TAna01Event::addPV() {
    TClonesArray& d = *fPV; 
    new(d[d.GetLast()+1]) TAnaVertex();
    ++fnPV;
    return (TAnaVertex*)d[d.GetLast()];
}

