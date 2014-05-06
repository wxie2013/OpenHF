#ifndef __Link_Tana__
#include "../interface/TAna01Event.hh"
#include "../interface/TAnaCand.hh"
#include "../interface/TAnaTrack.hh"
#include "../interface/TAnaVertex.hh"
#include "../interface/map_info.hh"
#include "../interface/hfcand.hh"
#include "../interface/hfcand_v0.hh"
#include "../interface/snglhfcand.hh"
#include "../interface/snglhfcand_v0.hh"
#include "../interface/candAna.hh"
#include "../interface/mapRunLumiHfTreeFile.hh"
#include "../interface/FindUsefulTrigger.hh"
#include "../interface/ana_hfforest.hh"
#include "../interface/checkTrgMatch.hh"

#define __Link_Tana__
#endif


#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ nestedclass;
#pragma link C++ nestedtypedef;

#pragma link C++ class TAna01Event+;
#pragma link C++ class TAnaCand+;
#pragma link C++ class TAnaTrack+;
#pragma link C++ class TAnaVertex+;
#pragma link C++ class map_info+;
#pragma link C++ class hfcand+; 
#pragma link C++ class hfcand_v0+; 
#pragma link C++ class snglhfcand+; 
#pragma link C++ class snglhfcand_v0+; 
#pragma link C++ class candAna+; 
#pragma link C++ class mapRunLumiHfTreeFile+; 
#pragma link C++ class FindUsefulTrigger+; 
#pragma link C++ class ana_hfforest+; 
#pragma link C++ class checkTrgMatch+; 

#endif

