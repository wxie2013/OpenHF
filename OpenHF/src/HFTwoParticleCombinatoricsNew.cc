/*
 *  HFTwoParticleCombinatoricsNew.cc
 *
 *  Created by Christoph Nägeli <christoph.naegeli@psi.ch> on 10.11.12.
 *
 */

#include "UserCode/OpenHF/interface/HFTwoParticleCombinatoricsNew.hh"

#include <iostream>
#include <TLorentzVector.h>

using std::cout;
using std::endl;

inline bool dupl(std::pair<int,int> p1, std::pair<int,int> p2)
{ return (p1.first == p2.first && p1.second == p2.second) || (p1.first == p2.second && p1.second == p2.first); }

HFTwoParticleCombinatoricsNew::HFTwoParticleCombinatoricsSet HFTwoParticleCombinatoricsNew::combine(std::vector<int> &tlist1, double mass1, std::vector<int> &tlist2, double mass2, double loMass, double hiMass, int rmDuplicate)
{
	HFTwoParticleCombinatoricsCompare comp(rmDuplicate);
	HFTwoParticleCombinatoricsNew::HFTwoParticleCombinatoricsSet result(comp);
	HFTwoParticleCombinatoricsNew::iterator it;
	std::vector<int>::const_iterator it1;
	std::vector<int>::const_iterator it2;
	TLorentzVector lv1,lv2;
	double momMass;
	
	for (it1 = tlist1.begin(); it1 != tlist1.end(); ++it1) {
	
		lv1.SetXYZM( (*fhTracks)[*it1].px(), (*fhTracks)[*it1].py(), (*fhTracks)[*it1].pz(), mass1 );
		
		for (it2 = tlist2.begin(); it2 != tlist2.end(); ++it2) {
			if (*it1 == *it2) continue; // same object
			
			lv2.SetXYZM( (*fhTracks)[*it2].px(), (*fhTracks)[*it2].py(), (*fhTracks)[*it2].pz(), mass2 );
			
			momMass = (lv1 + lv2).M();
			if (loMass < momMass && momMass < hiMass)
				result.insert(std::make_pair(*it1, *it2));
		}
	}
	
	if (fVerbose > 2) {
		for (it = result.begin(); it != result.end(); ++it)
			cout << "combList. 1: " << it->first << " 2: " << it->second << endl;
	}
	
	return result;
} // combine()
