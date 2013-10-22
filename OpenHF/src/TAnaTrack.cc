#include "../interface/TAnaTrack.hh"
#include <iostream>

ClassImp(TAnaTrack)

    using namespace std;

    TAnaTrack::TAnaTrack(int index) { 
        fIndex = index;
    }

void TAnaTrack::clear() {
    fMCID     = -99999;
    fIndex    = -1;
    fAlgorithm= -1; 

    fQ = -99; 

    fDof = fValidHits = -99; 
    fChi2 = -99.;
    fTrackQuality = -99; 
    fAlgorithm = -99;

    fdz = fdzE = fd0 = fd0E = -99.;
    fLip = fLipE = fTip = fTipE = -99.; 

    fDeDx = -99.; 
}
