#ifndef TANA01EVENT
#define TANA01EVENT


#include "TObject.h"
#include "TClonesArray.h"
#include "TVector3.h"
#include "TLorentzVector.h"

#include "TAnaTrack.hh"
#include "TAnaCand.hh"
#include "TAnaVertex.hh"

class TAna01Event : public TObject {

public:

  TAna01Event() { };
  TAna01Event(Int_t Option);
  virtual ~TAna01Event() { };
  virtual void  Clear(Option_t *option ="");

  // ----------------------------------------------------------------------

  // -- Signal Tracks (e.g. refitted tracks forming cands in the next block)
  int                 nSigTracks() {return fnSigTracks;}
  TAnaTrack*          getSigTrack(int n);
  virtual TAnaTrack*  addSigTrack();

  // -- Signal Candidates (e.g. B_s)
  int                 nCands() {return fnCandidates;}
  TAnaCand*           getCand(int n);
  virtual TAnaCand*   addCand();

  // -- Primary vertices
  int                 nPV()    {return fnPV;}
  TAnaVertex*         bestPV() {return getPV(fBestPV); }
  TAnaVertex*         getPV(int n);
  virtual TAnaVertex* addPV();

  // -- Beamspot
  TAnaVertex          fBeamSpot; 

  // ----------------------------------------------------------------------
  // -- Basic event and detector information
  Long64_t			fRunNumber, fEventNumber;
  int               fEventBits;
  int               fDetectorStatus; 
  int               fEventTag;
  int               fBestPV;   // be careful with this...

  // -- MC event/generation information
  int               fProcessID;
  double            fXsec;
  double            fFilterEff;
  double            fEventWeight;
  double            fPtHat;

  // -- Lumi
  double            fLumi, fLumiInt; 
  int               fLumiSection; 
  int               fOrbit; 
  int               fBx; 
  
  unsigned int      fTimeLo, fTimeHi; 


  // -- Reserve variables
  int               fIntRes[10]; 
  double            fDoubleRes[10]; 

private:

  int               fnSigTracks;
  TClonesArray      *fSigTracks;

  int               fnCandidates;
  TClonesArray      *fCandidates;

  int               fnPV;
  TClonesArray      *fPV;

  ClassDef(TAna01Event,1)

};

#endif
