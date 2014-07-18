#include <TTree.h>
#include <iostream>
#include <fstream>

#include "../interface/candAna.hh"
#include "../interface/HFMasses.hh"
#include "../interface/HFPid.hh"

using namespace std;

ClassImp(candAna)

// ----------------------------------------------------------------------
candAna::candAna() 
{
    //.. default cut is no cut
    cut_ffls3d = 0;
    cut_falpha0 = 999;

    ncand = 0;
    hfcandidate = 0;
    fpEvt = 0;

    fTree = 0;
}

// ----------------------------------------------------------------------
candAna::~candAna() 
{
}

//_________________________________________________________________________
void candAna::Init(TTree* treeIn)
{
    if(!treeIn)
        fTree = new TTree("hftree", "heavy flavor reduced tree ");
    else 
        fTree = treeIn;

    hfcandidate = new hfcand_v0;
    fpEvt = new TAna01Event(0);

    fTree->Branch("hfcandidate", &hfcandidate);
}

//___________________________________________________________
bool candAna::pass_cut(float ffls3d, float falpha0) 
{
    return (ffls3d > cut_ffls3d) && (falpha0 < cut_falpha0);
}
// ----------------------------------------------------------------------
void candAna::evtAnalysis(TAna01Event *evt) 
{
    fpEvt = evt; 

    ncand = 0;

    hfcandidate->set_runNum(static_cast<long int>(fpEvt->fRunNumber));
    hfcandidate->set_evtNum(static_cast<long int>(fpEvt->fEventNumber));

    for (int iC = 0; iC < fpEvt->nCands(); ++iC) {
        fpCand = fpEvt->getCand(iC);

        if (0 == fpCand || fpCand->fMom !=-1) continue; //..fMom!=-1 means decay meson


        if(fpCand->fType == Dstar2D0Pi) {//.. Dstar->D0+pi ...

            DstarAnalysis();


        } else if(fpCand->fType == D02KPi) {//.. D0->K+pi ..

            D0Analysis();

        } else if(fpCand->fType == Ds2PhiPi) {//.. Ds->phi+pi

            Ds2PhiPiAnalysis();

        } else if(fpCand->fType == Ds2KstarK) {//.. Ds->K*+K

            Ds2KstarKaonAnalysis();

        } else if(fpCand->fType == Dpm2KPiPi) {//.. D+/-->K+pi+pi

            Dpm2KaPiPiAnalysis();

        } else {
            cout<<" !! no such particle ID: "<<fpCand->fType<<" !!"<<endl;
            continue;
        }
    }

    fTree->Fill(); 
}

// ----------------------------------------------------------------------
void candAna::D0Analysis() 
{
    TAnaTrack *pPi, *pK; 
    double fls3d(0.), prob(0.), chi2(0.), alpha0(0.); 

    pPi = 0;
    pK =0; 

    for (int id = fpCand->fSig1; id <= fpCand->fSig2; ++id) {

        int index = id;

        if (211 == fpEvt->getSigTrack(id)->fMCID) {  // pion 
            pPi = fpEvt->getSigTrack(index);
        } else {  // kaon
            pK = fpEvt->getSigTrack(index);
        }
    }


    if(pPi == 0 || pK==0) {
        return;  // skip if no daughters 
    }

    // Pt, eta
    double pt    = fpCand->fPlab.Perp(); // cand pt
    double ptPi = pPi->fPlab.Perp();
    double ptK  = pK->fPlab.Perp();

    double eta   = fpCand->fPlab.Eta();
    double etaPi = pPi->fPlab.Eta();
    double etaK  = pK->fPlab.Eta();

    double phi   = fpCand->fPlab.Phi();
    double phiPi = pPi->fPlab.Phi();
    double phiK  = pK->fPlab.Phi();


    // charge 
    int qPi   = pPi->fQ;
    int qK  = pK->fQ;

    // masses
    double mdz = fpCand->fMass; 

    // D0 vertex
    TAnaVertex sv = fpCand->fVtx;  // Ds vertex
    TVector3 sv1 = sv.fPoint;  // Ds decay vertex position 
    int pvidx = (fpCand->fPvIdx > -1? fpCand->fPvIdx : 0);  // Ds PV index
    TVector3 pv =  fpEvt->getPV(pvidx)->fPoint;  // Ds pv vertex 


    // Calculate angles
    TVector3 t1(sv1-pv);

    fls3d = sv.fD3d/sv.fD3dE; 
    //flsxy = sv.fDxy/sv.fDxyE; 
    prob  = sv.fProb;
    chi2  = sv.fChi2;

    alpha0 = t1.Angle(fpCand->fPlab);  // Ds pointing angle

    //..
    int nBPixLayer_dau2 = 0;
    int nPixLayer_dau2 = 0;
    int nBPixLayer1Hits_dau2 = 0;
    int nTrackerLayer_dau2 = 0;

    int nBPixLayer_1 = numberOfBPixLayers(pPi);
    int nPixLayer_1  = numberOfPixLayers(pPi);
    int nBPixLayer1Hits_1  = numberOfBPixLayer1Hits(pPi);
    int nTrackerLayer_1 = numberOfTrackerLayers(pPi);

    int nBPixLayer_2 = numberOfBPixLayers(pK);
    int nPixLayer_2 = numberOfPixLayers(pK);
    int nBPixLayer1Hits_2  = numberOfBPixLayer1Hits(pK);
    int nTrackerLayer_2 = numberOfTrackerLayers(pK);


    //.. fill the hfcand ...
    addCandidate(prob, sv.fD3d, sv.fD3dE, fls3d, chi2, alpha0, -1, fpCand->fType, pt, mdz, eta, phi, -999, -999, -999, -999, -999, -999, -999, -999, qPi, ptPi, etaPi, phiPi, qK, ptK, etaK, phiK, nBPixLayer_dau2, nPixLayer_dau2, nBPixLayer1Hits_dau2, nTrackerLayer_dau2, nBPixLayer_1, nPixLayer_1, nBPixLayer1Hits_1, nTrackerLayer_1, nBPixLayer_2, nPixLayer_2, nBPixLayer1Hits_2, nTrackerLayer_2); 
}

//-----------------------------------------------------------------
void candAna::DstarAnalysis() 
{
    TAnaCand *pC(0);
    TAnaTrack *pK, *pPi, *pPis; 
    double fls3d(0.), prob(0.), chi2(0.), alpha0(0.), dr(0.); 
    //int indexPi=-1, indexK=-1;
    // -- D0 
    if (fpCand->fDau1 < 0  || fpCand->fDau2 < 0) {
        return;  // skip if no daughters 
    }


    pC = fpEvt->getCand(fpCand->fDau1);  // D0 candidate 
    pK = 0;
    pPi =0; 

    int pPisId = fpCand->fSig1; // slow pion index
    pPis = fpEvt->getSigTrack(pPisId); // slow pi from D*
    TVector3 piSlowMom = pPis->fPlab; // slow pi momentum vector 

    // loop over D0 tracks 
    for (int id = pC->fSig1; id <= pC->fSig2; ++id) {
        int index = id;

        if (211 == fpEvt->getSigTrack(id)->fMCID) {  // pion 
            pPi = fpEvt->getSigTrack(index);
        } else {  // kaon
            pK = fpEvt->getSigTrack(index);
        }
    }

    if(pPi == 0 || pK==0) {
        return;  // skip if no daughters 
    }

    // Pt, eta
    double pt    = fpCand->fPlab.Perp(); // cand pt
    double ptdz  = pC->fPlab.Perp();
    double ptPis = pPis->fPlab.Perp();
    double ptPi  = pPi->fPlab.Perp();
    double ptK   = pK->fPlab.Perp();

    double eta   = fpCand->fPlab.Eta();
    double etadz  = pC->fPlab.Eta();
    double etaPis = pPis->fPlab.Eta();
    double etaPi = pPi->fPlab.Eta();
    double etaK  = pK->fPlab.Eta();

    double phi   = fpCand->fPlab.Phi();
    double phidz  = pC->fPlab.Phi();
    double phiPis = pPis->fPlab.Phi();
    double phiPi = pPi->fPlab.Phi();
    double phiK  = pK->fPlab.Phi();

    // charge 
    int qK   = pK->fQ;
    int qPi  = pPi->fQ;
    int qPis = pPis->fQ;

    // masses
    double mdstar = fpCand->fMass; 
    double mdz = pC->fMass;
    //double dm = mdstar - mdz;

    // D* vertex
    TAnaVertex sv = fpCand->fVtx;  // D* vertex
    TVector3 sv1 = sv.fPoint;  // D* decay vertex position 
    int pvidx = (fpCand->fPvIdx > -1? fpCand->fPvIdx : 0);  // D* PV index
    TVector3 pv =  fpEvt->getPV(pvidx)->fPoint;  // Dstar vertex 

    // D0 vertex
    TAnaVertex svD0 = pC->fVtx;  // D0 vertex
    TVector3 sv2 = svD0.fPoint; // D0 decay vertex position

    // Calculate angles
    TVector3 t2(sv2-pv);

    fls3d = svD0.fD3d/svD0.fD3dE; //  use D0
    //flsxy = svD0.fDxy/svD0.fDxyE; 
    prob  = svD0.fProb;
    chi2  = svD0.fChi2;

    alpha0  = t2.Angle(pC->fPlab); // D0 angle
    dr = piSlowMom.Angle(fpCand->fPlab); // pislow openinig

    // Now the selection cuts cut 
    if( (qPi+qPis)==0 ) return; // skip wrong sign decys 


    //..
    int nBPixLayer_dau2 = numberOfBPixLayers(pPis);
    int nPixLayer_dau2 = numberOfPixLayers(pPis);
    int nBPixLayer1Hits_dau2 = numberOfBPixLayer1Hits(pPis);
    int nTrackerLayer_dau2 = numberOfTrackerLayers(pPis);

    int nBPixLayer_1 = numberOfBPixLayers(pPi);
    int nPixLayer_1  = numberOfPixLayers(pPi);
    int nBPixLayer1Hits_1  = numberOfBPixLayer1Hits(pPi);
    int nTrackerLayer_1 = numberOfTrackerLayers(pPi);

    int nBPixLayer_2 = numberOfBPixLayers(pK);
    int nPixLayer_2 = numberOfPixLayers(pK);
    int nBPixLayer1Hits_2  = numberOfBPixLayer1Hits(pK);
    int nTrackerLayer_2 = numberOfTrackerLayers(pK);


    //.. fill the hfcand ...
    addCandidate(prob, svD0.fD3d, svD0.fD3dE, fls3d, chi2, alpha0, dr, fpCand->fType, pt, mdstar, eta, phi, mdz, ptdz, etadz, phidz, qPis, ptPis, etaPis, phiPis, qPi, ptPi, etaPi, phiPi, qK, ptK, etaK, phiK, nBPixLayer_dau2, nPixLayer_dau2, nBPixLayer1Hits_dau2, nTrackerLayer_dau2, nBPixLayer_1, nPixLayer_1, nBPixLayer1Hits_1, nTrackerLayer_1, nBPixLayer_2, nPixLayer_2, nBPixLayer1Hits_2, nTrackerLayer_2); 
}

//_________________________________________________________________________
void candAna::Dpm2KaPiPiAnalysis() 
{
    TAnaCand *pC(0);
    TAnaTrack *pK, *pPi1, *pPi2; 
    double fls3d(0.), prob(0.), chi2(0.), alpha0(0.), dr(0.); 

    // -- 
    if (fpCand->fDau1 < 0  || fpCand->fDau2 < 0) {
        return;  // skip if no daughters 
    }

    pC = fpEvt->getCand(fpCand->fDau1);  // kpi candidate 
    pK = 0;
    pPi1 =0; 

    int pPi2Id = fpCand->fSig1; // slow pion index
    pPi2 = fpEvt->getSigTrack(pPi2Id); // slow pi from D*
    TVector3 piSlowMom = pPi2->fPlab; // slow pi momentum vector 

    // loop over kpi tracks 
    for (int id = pC->fSig1; id <= pC->fSig2; ++id) {
        int index = id;

        if (211 == fpEvt->getSigTrack(id)->fMCID) {  // pion 
            pPi1 = fpEvt->getSigTrack(index);
        } else {  // kaon
            pK = fpEvt->getSigTrack(index);
        }
    }

    if(pPi1 == 0 || pK==0) {
        return;  // skip if no daughters 
    }

    // Pt, eta
    double pt    = fpCand->fPlab.Perp(); // cand pt
    double ptkpi  = pC->fPlab.Perp();
    double ptPi1  = pPi1->fPlab.Perp();
    double ptPi2 = pPi2->fPlab.Perp();
    double ptK   = pK->fPlab.Perp();

    double eta   = fpCand->fPlab.Eta();
    double etakpi  = pC->fPlab.Eta();
    double etaPi1 = pPi1->fPlab.Eta();
    double etaPi2 = pPi2->fPlab.Eta();
    double etaK  = pK->fPlab.Eta();

    double phi   = fpCand->fPlab.Phi();
    double phikpi  = pC->fPlab.Phi();
    double phiPi1 = pPi1->fPlab.Phi();
    double phiPi2 = pPi2->fPlab.Phi();
    double phiK  = pK->fPlab.Phi();

    // charge 
    int qK   = pK->fQ;
    int qPi1  = pPi1->fQ;
    int qPi2 = pPi2->fQ;

    // masses
    double mdpm = fpCand->fMass; 
    double mkpi = pC->fMass;
    //double dm = mdpm - mkpi;

    // Dpm vertex
    TAnaVertex sv = fpCand->fVtx;  // Dpm vertex
    TVector3 sv1 = sv.fPoint;  // Dpm decay vertex position 
    int pvidx = (fpCand->fPvIdx > -1? fpCand->fPvIdx : 0);  // D* PV index
    TVector3 pv =  fpEvt->getPV(pvidx)->fPoint;  // Dpm vertex 

    // kpi vertex
    TAnaVertex svkpi = pC->fVtx;  // Phi vertex
    TVector3 sv2 = svkpi.fPoint; // Phi decay vertex position

    // Calculate angles
    TVector3 t1(sv1-pv);

    fls3d = sv.fD3d/sv.fD3dE; //  use Phi
    //flsxy = sv.fDxy/sv.fDxyE; 
    prob  = sv.fProb;
    chi2  = sv.fChi2;

    alpha0 = t1.Angle(fpCand->fPlab);  // Dpm pointing angle
    dr = piSlowMom.Angle(fpCand->fPlab); // pislow openinig

    //..
    int nBPixLayer_dau2 = numberOfBPixLayers(pPi2);
    int nPixLayer_dau2 = numberOfPixLayers(pPi2);
    int nBPixLayer1Hits_dau2 = numberOfBPixLayer1Hits(pPi2);
    int nTrackerLayer_dau2 = numberOfTrackerLayers(pPi2);

    int nBPixLayer_1 = numberOfBPixLayers(pPi1);
    int nPixLayer_1  = numberOfPixLayers(pPi1);
    int nBPixLayer1Hits_1  = numberOfBPixLayer1Hits(pPi1);
    int nTrackerLayer_1 = numberOfTrackerLayers(pPi1);

    int nBPixLayer_2 = numberOfBPixLayers(pK);
    int nPixLayer_2 = numberOfPixLayers(pK);
    int nBPixLayer1Hits_2  = numberOfBPixLayer1Hits(pK);
    int nTrackerLayer_2 = numberOfTrackerLayers(pK);

    //.. fill the hfcand ...
    addCandidate(prob, sv.fD3d, sv.fD3dE, fls3d, chi2, alpha0, dr, fpCand->fType, pt, mdpm, eta, phi, mkpi, ptkpi, etakpi, phikpi, qPi2, ptPi2, etaPi2, phiPi2, qPi1, ptPi1, etaPi1, phiPi1, qK, ptK, etaK, phiK, nBPixLayer_dau2, nPixLayer_dau2, nBPixLayer1Hits_dau2, nTrackerLayer_dau2, nBPixLayer_1, nPixLayer_1, nBPixLayer1Hits_1, nTrackerLayer_1, nBPixLayer_2, nPixLayer_2, nBPixLayer1Hits_2, nTrackerLayer_2); 

}

//_________________________________________________________________
void candAna::Ds2KstarKaonAnalysis() 
{
    TAnaCand *pC(0);
    TAnaTrack *pK1, *pPi, *pK2; 
    double fls3d(0.), prob(0.), chi2(0.), alpha0(0.), dr(0.); 
    //int indexPi=-1, indexK=-1;
    // -- kpi 
    if (fpCand->fDau1 < 0  || fpCand->fDau2 < 0) {
        return;  // skip if no daughters 
    }

    pC = fpEvt->getCand(fpCand->fDau1);  // kpi candidate 
    pK1 = 0;
    pPi =0; 

    int pK2Id = fpCand->fSig1; // slow pion index
    pK2 = fpEvt->getSigTrack(pK2Id); // slow pi from D*
    TVector3 piSlowMom = pK2->fPlab; // slow pi momentum vector 

    // loop over kpi tracks 

    for (int id = pC->fSig1; id <= pC->fSig2; ++id) {

        int index = id;

        if (211 == fpEvt->getSigTrack(id)->fMCID) { //..pion 
            pPi = fpEvt->getSigTrack(index);
        } else {  // k-
            pK1 = fpEvt->getSigTrack(index);
        }
    }


    if(pPi == 0 || pK1==0) {
        return;  // skip if no daughters 
    }

    // Pt, eta
    double pt    = fpCand->fPlab.Perp(); // cand pt
    double ptkpi  = pC->fPlab.Perp();
    double ptK2 = pK2->fPlab.Perp();
    double ptPi  = pPi->fPlab.Perp();
    double ptK1   = pK1->fPlab.Perp();

    double eta   = fpCand->fPlab.Eta();
    double etakpi  = pC->fPlab.Eta();
    double etaPi = pPi->fPlab.Eta();
    double etaK1  = pK1->fPlab.Eta();
    double etaK2  = pK2->fPlab.Eta();

    double phi   = fpCand->fPlab.Phi();
    double phikpi  = pC->fPlab.Phi();
    double phiPi = pPi->fPlab.Phi();
    double phiK1  = pK1->fPlab.Phi();
    double phiK2  = pK2->fPlab.Phi();

    // charge 
    int qK1   = pK1->fQ;
    int qPi  = pPi->fQ;
    int qK2 = pK2->fQ;

    // masses
    double mds = fpCand->fMass; 
    double mkpi = pC->fMass;

    // Ds vertex
    TAnaVertex sv = fpCand->fVtx;  // Ds vertex
    TVector3 sv1 = sv.fPoint;  // Ds decay vertex position 
    int pvidx = (fpCand->fPvIdx > -1? fpCand->fPvIdx : 0);  // Ds PV index
    TVector3 pv =  fpEvt->getPV(pvidx)->fPoint;  // Ds vertex 

    // kpi vertex
    TAnaVertex svkpi = pC->fVtx;  // kpi vertex
    TVector3 sv2 = svkpi.fPoint; // kpi decay vertex position

    // Calculate angles
    TVector3 t1(sv1-pv);

    fls3d = sv.fD3d/sv.fD3dE; 
    //flsxy = sv.fDxy/sv.fDxyE; 
    prob  = sv.fProb;
    chi2  = sv.fChi2;

    //ffls3dkpi = svkpi.fD3d/svkpi.fD3dE; 
    //fflsxykpi = svkpi.fDxy/svkpi.fDxyE; 
    //fprobkpi  = svkpi.fProb;
    //fchi2kpi  = svkpi.fChi2;

    alpha0 = t1.Angle(fpCand->fPlab);  // Ds pointing angle
    dr = piSlowMom.Angle(fpCand->fPlab); // pislow openinig

    //..
    int nBPixLayer_dau2 = numberOfBPixLayers(pK2);
    int nPixLayer_dau2 = numberOfPixLayers(pK2);
    int nBPixLayer1Hits_dau2 = numberOfBPixLayer1Hits(pK2);
    int nTrackerLayer_dau2 = numberOfTrackerLayers(pK2);

    int nBPixLayer_1 = numberOfBPixLayers(pPi);
    int nPixLayer_1  = numberOfPixLayers(pPi);
    int nBPixLayer1Hits_1  = numberOfBPixLayer1Hits(pPi);
    int nTrackerLayer_1 = numberOfTrackerLayers(pPi);

    int nBPixLayer_2 = numberOfBPixLayers(pK1);
    int nPixLayer_2 = numberOfPixLayers(pK1);
    int nBPixLayer1Hits_2  = numberOfBPixLayer1Hits(pK1);
    int nTrackerLayer_2 = numberOfTrackerLayers(pK1);


    //.. fill the hfcand ...
    addCandidate(prob, sv.fD3d, sv.fD3dE, fls3d, chi2, alpha0, dr, fpCand->fType, pt, mds, eta, phi, mkpi, ptkpi, etakpi, phikpi, qK2, ptK2, etaK2, phiK2, qPi, ptPi, etaPi, phiPi, qK1, ptK1, etaK1, phiK1, nBPixLayer_dau2, nPixLayer_dau2, nBPixLayer1Hits_dau2, nTrackerLayer_dau2, nBPixLayer_1, nPixLayer_1, nBPixLayer1Hits_1, nTrackerLayer_1, nBPixLayer_2, nPixLayer_2, nBPixLayer1Hits_2, nTrackerLayer_2); 

}

//_________________________________________________________________________
void candAna::Ds2PhiPiAnalysis() 
{
    TAnaCand *pC(0);
    TAnaTrack *pK1, *pK2, *pPi; 
    double fls3d(0.), prob(0.), chi2(0.), alpha0(0.), dr(0.); 

    // -- Phi 
    if (fpCand->fDau1 < 0  || fpCand->fDau2 < 0) {
        return;  // skip if no daughters 
    }

    pC = fpEvt->getCand(fpCand->fDau1);  // Phi candidate 
    pK1 = 0;
    pK2 =0; 

    int pPiId = fpCand->fSig1; // slow pion index
    pPi = fpEvt->getSigTrack(pPiId); // slow pi from D*
    TVector3 piSlowMom = pPi->fPlab; // slow pi momentum vector 

    // loop over Phi tracks 

    int idau = 0;
    for (int id = pC->fSig1; id <= pC->fSig2; ++id) {
        int index = id;

        if (idau==0) { //.. k+
            pK1 = fpEvt->getSigTrack(index);
        } else {  // k-
            pK2 = fpEvt->getSigTrack(index);
        }

        idau++;
    }

    if(idau >2) {
        cout<<" !!! phi can not have more than 2 daughters, exit !!! "<<endl;
        exit(0);
    }

    if(pK2 == 0 || pK1==0) {
        return;  // skip if no daughters 
    }

    // Pt, eta
    double pt    = fpCand->fPlab.Perp(); // cand pt
    double ptPhi  = pC->fPlab.Perp();
    double ptPi = pPi->fPlab.Perp();
    double ptK2  = pK2->fPlab.Perp();
    double ptK1   = pK1->fPlab.Perp();

    double eta   = fpCand->fPlab.Eta();
    double etaPhi  = pC->fPlab.Eta();
    double etaPi= pPi->fPlab.Eta();
    double etaK2 = pK2->fPlab.Eta();
    double etaK1  = pK1->fPlab.Eta();

    double phi   = fpCand->fPlab.Phi();
    double phiPhi  = pC->fPlab.Phi();
    double phiPi= pPi->fPlab.Phi();
    double phiK2 = pK2->fPlab.Phi();
    double phiK1  = pK1->fPlab.Phi();

    // charge 
    int qK1   = pK1->fQ;
    int qK2  = pK2->fQ;
    int qPi = pPi->fQ;

    // masses
    double mds = fpCand->fMass; 
    double mPhi = pC->fMass;
    //double dm = mds - mPhi;

    // Ds vertex
    TAnaVertex sv = fpCand->fVtx;  // Ds vertex
    TVector3 sv1 = sv.fPoint;  // Ds decay vertex position 
    int pvidx = (fpCand->fPvIdx > -1? fpCand->fPvIdx : 0);  // Ds PV index
    TVector3 pv =  fpEvt->getPV(pvidx)->fPoint;  // Ds vertex 

    // Phi vertex
    TAnaVertex svPhi = pC->fVtx;  // Phi vertex
    TVector3 sv2 = svPhi.fPoint; // Phi decay vertex position

    // Calculate angles
    TVector3 t1(sv1-pv), t2(sv2-pv), t3(sv2-sv1);

    fls3d = sv.fD3d/sv.fD3dE; 
    //flsxy = sv.fDxy/sv.fDxyE; 
    prob  = sv.fProb;
    chi2  = sv.fChi2;

    alpha0 = t1.Angle(fpCand->fPlab);  // Ds pointing angle
    dr = piSlowMom.Angle(fpCand->fPlab); // pislow openinig

    //..
    int nBPixLayer_dau2 = numberOfBPixLayers(pPi);
    int nPixLayer_dau2 = numberOfPixLayers(pPi);
    int nBPixLayer1Hits_dau2 = numberOfBPixLayer1Hits(pPi);
    int nTrackerLayer_dau2 = numberOfTrackerLayers(pPi);

    int nBPixLayer_1 = numberOfBPixLayers(pK1);
    int nPixLayer_1  = numberOfPixLayers(pK1);
    int nBPixLayer1Hits_1  = numberOfBPixLayer1Hits(pK1);
    int nTrackerLayer_1 = numberOfTrackerLayers(pK1);

    int nBPixLayer_2 = numberOfBPixLayers(pK2);
    int nPixLayer_2 = numberOfPixLayers(pK2);
    int nBPixLayer1Hits_2  = numberOfBPixLayer1Hits(pK2);
    int nTrackerLayer_2 = numberOfTrackerLayers(pK2);


    //.. fill the hfcand ...
    addCandidate(prob, sv.fD3d, sv.fD3dE, fls3d, chi2, alpha0, dr, fpCand->fType, pt, mds, eta, phi, mPhi, ptPhi, etaPhi, phiPhi, qPi, ptPi, etaPi, phiPi, qK1, ptK1, etaK1, phiK1, qK2, ptK2, etaK2, phiK2, nBPixLayer_dau2, nPixLayer_dau2, nBPixLayer1Hits_dau2, nTrackerLayer_dau2, nBPixLayer_1, nPixLayer_1, nBPixLayer1Hits_1, nTrackerLayer_1, nBPixLayer_2, nPixLayer_2, nBPixLayer1Hits_2, nTrackerLayer_2); 
}

//__________________________________________
void candAna::addCandidate(
        float fprob,
        float ff3d,
        float ff3de,
        float ffls3d,
        float fchi2,
        float falpha0,
        float fdr,
        int   type,
        float fpt,
        float fm,
        float feta,
        float fphi,
        float fmdau1,
        float fptdau1,
        float fetadau1,
        float fphidau1,
        float fqdau2,
        float fptdau2,
        float fetadau2,
        float fphidau2,
        float fq1,
        float fpt1,
        float feta1,
        float fphi1,
        float fq2,
        float fpt2,
        float feta2,
        float fphi2, 
        int   nBPixLayer_dau2, 
        int   nPixLayer_dau2,
        int   nBPixLayer1Hits_dau2,
        int   nTrackerLayer_dau2,
        int   nBPixLayer_1,
        int   nPixLayer_1,
        int   nBPixLayer1Hits_1,
        int   nTrackerLayer_1,
        int   nBPixLayer_2,
        int   nPixLayer_2,
        int   nBPixLayer1Hits_2,
        int   nTrackerLayer_2)
{

   if(!pass_cut(ffls3d, falpha0)) 
      return; 

    hfcandidate->AddCand(ncand);
    hfcandidate->set_nhfcand(ncand+1);

    hfcandidate->get_hfcand(ncand)->set_fprob(fprob);
    hfcandidate->get_hfcand(ncand)->set_ff3d(ff3d);
    hfcandidate->get_hfcand(ncand)->set_ff3de(ff3de);
    hfcandidate->get_hfcand(ncand)->set_ffls3d(ffls3d);
    hfcandidate->get_hfcand(ncand)->set_fchi2(fchi2);
    hfcandidate->get_hfcand(ncand)->set_falpha0(falpha0);
    hfcandidate->get_hfcand(ncand)->set_fdr(fdr);
    hfcandidate->get_hfcand(ncand)->set_type(type);
    hfcandidate->get_hfcand(ncand)->set_fpt(fpt);
    hfcandidate->get_hfcand(ncand)->set_fm(fm);
    hfcandidate->get_hfcand(ncand)->set_feta(feta);
    hfcandidate->get_hfcand(ncand)->set_fphi(fphi);
    hfcandidate->get_hfcand(ncand)->set_fmdau1(fmdau1);
    hfcandidate->get_hfcand(ncand)->set_fptdau1(fptdau1);
    hfcandidate->get_hfcand(ncand)->set_fetadau1(fetadau1);
    hfcandidate->get_hfcand(ncand)->set_fphidau1(fphidau1);
    hfcandidate->get_hfcand(ncand)->set_fptdau2(fptdau2);
    hfcandidate->get_hfcand(ncand)->set_fetadau2(fetadau2);
    hfcandidate->get_hfcand(ncand)->set_fphidau2(fphidau2);
    hfcandidate->get_hfcand(ncand)->set_fqdau2(fqdau2);
    hfcandidate->get_hfcand(ncand)->set_fq1(fq1);
    hfcandidate->get_hfcand(ncand)->set_fpt1(fpt1);
    hfcandidate->get_hfcand(ncand)->set_feta1(feta1);
    hfcandidate->get_hfcand(ncand)->set_fphi1(fphi1);
    hfcandidate->get_hfcand(ncand)->set_fpt2(fpt2);
    hfcandidate->get_hfcand(ncand)->set_fq2(fq2);
    hfcandidate->get_hfcand(ncand)->set_feta2(feta2);
    hfcandidate->get_hfcand(ncand)->set_fphi2(fphi2);
    hfcandidate->get_hfcand(ncand)->set_nBPixLayer_dau2(nBPixLayer_dau2);
    hfcandidate->get_hfcand(ncand)->set_nPixLayer_dau2(nPixLayer_dau2);
    hfcandidate->get_hfcand(ncand)->set_nBPixLayer1Hits_dau2(nBPixLayer1Hits_dau2);
    hfcandidate->get_hfcand(ncand)->set_nTrackerLayer_dau2(nTrackerLayer_dau2);
    hfcandidate->get_hfcand(ncand)->set_nBPixLayer_1(nBPixLayer_1);
    hfcandidate->get_hfcand(ncand)->set_nPixLayer_1(nPixLayer_1);
    hfcandidate->get_hfcand(ncand)->set_nBPixLayer1Hits_1(nBPixLayer1Hits_1);
    hfcandidate->get_hfcand(ncand)->set_nTrackerLayer_1(nTrackerLayer_1);
    hfcandidate->get_hfcand(ncand)->set_nBPixLayer_2(nBPixLayer_2);
    hfcandidate->get_hfcand(ncand)->set_nPixLayer_2(nPixLayer_2);
    hfcandidate->get_hfcand(ncand)->set_nBPixLayer1Hits_2(nBPixLayer1Hits_2);
    hfcandidate->get_hfcand(ncand)->set_nTrackerLayer_2(nTrackerLayer_2);

    ncand++;
}

// ----------------------------------------------------------------------
void candAna::LoopOverFile(int startFile, int endFile, char *filelist) 
{
    char* resultfile = new char[100];
    sprintf(resultfile, "%s%s%d%s%d%s", "candAna_", filelist, startFile, "To", endFile, ".root");
    result = new TFile(resultfile, "RECREATE");  

    //...
    Init(fTree);

    //..
    ifstream file_stream(filelist);

    char* filename = new char[1000];

    int ifile = 0;
    while (file_stream >> filename) {

        ifile++;
        if(ifile-1 >= endFile) 
            break;
        else if(ifile-1 < startFile) 
            continue;

        cout<<filename<<endl;

        TFile* f = TFile::Open(filename);
        if(!f || f->IsZombie()) {
            cout<<" corrupted files "<<endl;
            continue;
        }

        TTree* T1 = 0;
        if(!(T1 = (TTree*) f->Get("T1")))
            T1 = (TTree*) f->Get("tree/fTree");



        LoopOverEvt(T1);

        f->Close();
    }

    result->cd();
    fTree->Write();
    result->Close ();
}

// ----------------------------------------------------------------------
void candAna::LoopOverEvt(TTree* T1, Long64_t& run_hifst, Long64_t& evt_hifst) 
{
    hfcandidate->Reset();
    if(!T1) {
        fTree->Fill();  //.. no D candidate. Fill in an empty object for that event.
        return;
    }

    int maxEvents = (int)T1->GetEntries();

    T1->SetBranchAddress("TAna01Event", &fpEvt);
    TBranch* b_run = T1->GetBranch("fRunNumber");
    TBranch* b_evt = T1->GetBranch("fEventNumber");

    bool match = false;
    for (int jEvent = 0; jEvent < maxEvents; ++jEvent) {
        //.. readout only event and run number for speed. Note: no need to call Clear()
        //.. since it does nothing to the fEventNumber and fRunNumber(Check the code). 
        b_run->GetEntry(jEvent);
        b_evt->GetEntry(jEvent);
        fEvt = static_cast<long int>(fpEvt->fEventNumber);		
        fRun = static_cast<long int>(fpEvt->fRunNumber);

        if(fEvt == evt_hifst && fRun == run_hifst) {//.. sync with hiforest 
            match = true;
            fpEvt->Clear(); //note: clear is slow. Do it only when there is a candidate match
            //.. readout the whole events only when there's match
            T1->GetEntry(jEvent);          
            evtAnalysis(fpEvt); 

            return;
        } 
    }
    if(!match) {
        cout<<" --> no matching hftree for this event <--"<<endl;
        fTree->Fill();  //.. no D candidate. Fill in an empty object for that event.
    }
}
// ----------------------------------------------------------------------
void candAna::LoopOverEvt(TTree* T1) 
{
    int maxEvents = (int)T1->GetEntries();

    int step(50000); 
    if (maxEvents < 1000000) step = 10000; 
    if (maxEvents < 100000)  step = 1000; 
    if (maxEvents < 10000)   step = 500; 
    if (maxEvents < 1000)    step = 100; 


    T1->SetBranchAddress("TAna01Event", &fpEvt);

    for (int jEvent = 0; jEvent < maxEvents; ++jEvent) {

        if (jEvent%step == 0) cout << Form(" .. Event %8d", jEvent)<<endl;
        fpEvt->Clear();                        
        T1->GetEntry(jEvent);          

        hfcandidate->Reset();
        evtAnalysis(fpEvt); 
    }
}

// ----------------------------------------------------------------------
int  candAna::numberOfBPixLayers(TAnaTrack *pTrack) 
{
    bool layer1 = false, layer2 = false, layer3 = false;
    int hits = pTrack->fValidHits;
    //cout<<" muon1 "<<algo1<<" "<<qual1<<" "<<hits1<<hex<<" ";
    if(hits>20) hits=20; // pattern has only 20 locations
    for(int i =0; i<hits; ++i){
        int pat = pTrack->fHitPattern[i];
        //cout<<pat<<" ";
        if( pat == 0x488 ) layer1 = true;
        else if( pat == 0x490 ) layer2 = true;
        else if( pat == 0x498 ) layer3 = true;
    }
    //cout<<dec<<endl;

    int pixHits=0;
    if(layer1) {pixHits++;}
    if(layer2) {pixHits++;}
    if(layer3) {pixHits++;}

    return pixHits;
}

// ----------------------------------------------------------------------
int  candAna::numberOfPixLayers(TAnaTrack *pTrack) 
{
    bool layer1 = false, layer2 = false, layer3 = false, disk1=false, disk2=false;
    int hits = pTrack->fValidHits;
    //cout<<" muon1 "<<algo1<<" "<<qual1<<" "<<hits1<<hex<<" ";
    if(hits>20) hits=20; // pattern has only 20 locations
    for(int i =0; i<hits; ++i){
        unsigned int pat = pTrack->fHitPattern[i];
        //cout<<pat<<" ";
        if( pat == 0x488 ) layer1 = true;
        else if( pat == 0x490 ) layer2 = true;
        else if( pat == 0x498 ) layer3 = true;
        else if( pat == 0x508 ) disk1 = true;
        else if( pat == 0x510 ) disk2 = true;
    }
    //cout<<dec<<endl;

    int pixHits=0;
    if(layer1) {pixHits++;}
    if(layer2) {pixHits++;}
    if(layer3) {pixHits++;}
    if(disk1) {pixHits++;}
    if(disk2) {pixHits++;}

    return pixHits;
}

// ----------------------------------------------------------------------
int  candAna::numberOfBPixLayer1Hits(TAnaTrack *pTrack) 
{
    int pixHits=0;
    int hits = pTrack->fValidHits;
    //cout<<" muon1 "<<algo1<<" "<<qual1<<" "<<hits1<<hex<<" ";
    if(hits>20) hits=20; // pattern has only 20 locations
    for(int i =0; i<hits; ++i){
        unsigned int pat = pTrack->fHitPattern[i];
        //cout<<pat<<" ";
        if( pat == 0x488 ) pixHits++;
        //else if( pat == 0x490 ) layer2 = true;
        //else if( pat == 0x498 ) layer3 = true;
    }
    //cout<<dec<<endl;

    return pixHits;
}


// ----------------------------------------------------------------------
// Hit pattern is the summary information of the hits associated to track in
// AOD.  When RecHits are no longer available, the compact hit pattern should
// allow basic track selection based on the hits in various subdetectors.  The
// hits of a track are saved in unit32_t hitPattern_[28], initialized as
// 0x00000000, ..., 0x00000000.  Set one hit with 10 bits
//
//      +-----+-----+-----+-----+-----+-----+-----+-----+----------------+-----+-----+
//      |tk/mu|  sub-structure  |   sub-sub-structure   |     stereo     |  hit type |
//      +-----+-----+-----+-----+-----+-----+-----+-----+----------------+-----+-----+
//  ... | 10  |   9    8     7  |   6    5     4     3  |        2       |  1     0  | bit
//  ... |  2  |   1    0     3  |   2    1     0     3  |        2       |  1     0  | bit
//
//      |tk = 1      PXB = 1            layer = 1-3                       hit type = 0-3
//      |tk = 1      PXF = 2            disk  = 1-2                       hit type = 0-3
//      |tk = 1      TIB = 3            layer = 1-4      0=rphi,1=stereo  hit type = 0-3
//      |tk = 1      TID = 4            wheel = 1-3      0=rphi,1=stereo  hit type = 0-3
//      |tk = 1      TOB = 5            layer = 1-6      0=rphi,1=stereo  hit type = 0-3
//      |tk = 1      TEC = 6            wheel = 1-9      0=rphi,1=stereo  hit type = 0-3
//      |mu = 0      DT  = 1            4*(stat-1)+superlayer             hit type = 0-3
//      |mu = 0      CSC = 2            4*(stat-1)+(ring-1)               hit type = 0-3
//      |mu = 0      RPC = 3            4*(stat-1)+2*layer+region         hit type = 0-3
//
//      hit type, see DataFormats/TrackingRecHit/interface/TrackingRecHit.h
//      valid    = valid hit                                     = 0
//      missing  = detector is good, but no rec hit found        = 1
//      inactive = detector is off, so there was no hope         = 2
//      bad      = there were many bad strips within the ellipse = 3
// ----------------------------------------------------------------------
int  candAna::numberOfTrackerLayers(TAnaTrack *pTrack) 
{
    bool pixl[3], tibl[4], tobl[6]; 
    bool pixd[2], tidw[3], tecw[9];

    for (int i = 0; i < 3; ++i) pixl[i] = false; 
    for (int i = 0; i < 4; ++i) tibl[i] = false; 
    for (int i = 0; i < 6; ++i) tobl[i] = false; 

    for (int i = 0; i < 2; ++i) pixd[i] = false; 
    for (int i = 0; i < 3; ++i) tidw[i] = false; 
    for (int i = 0; i < 9; ++i) tecw[i] = false; 

    int hits = pTrack->fValidHits;
    if(hits>20) hits=20; // pattern has only 20 locations
    //  cout << "----------------------------------------------------------------------" << endl;
    int hit(0), hitmask(3); 
    int det(0), detpos(7), detmask(0); 
    int lay(0), layerpos(3), layermask(0); 
    detmask = 0x7 << detpos;
    layermask = 0xf << layerpos;
    //  cout << "detmask = " << std::hex << detmask << " laymask = " << layermask << std::dec << endl;
    for(int i =0; i<hits; ++i){
        unsigned int pat = pTrack->fHitPattern[i];

        hit = (pat & hitmask);
        det = 0; 
        det = (pat & detmask)>>detpos; 
        lay = 0; 
        lay = (pat & layermask)>>layerpos; 

        if ((1 == det) && (0 == hit)) pixl[lay] = true; 
        if ((2 == det) && (0 == hit)) pixd[lay] = true; 

        if ((3 == det) && (0 == hit)) tibl[lay] = true; 
        if ((4 == det) && (0 == hit)) tidw[lay] = true; 

        if ((5 == det) && (0 == hit)) tobl[lay] = true; 
        if ((6 == det) && (0 == hit)) tecw[lay] = true; 

    }

    int trkHits(0);
    for (int i = 0; i < 3; ++i) if (pixl[i]) ++trkHits;
    for (int i = 0; i < 4; ++i) if (tibl[i]) ++trkHits; 
    for (int i = 0; i < 6; ++i) if (tobl[i]) ++trkHits; 

    for (int i = 0; i < 2; ++i) if (pixd[i]) ++trkHits; 
    for (int i = 0; i < 3; ++i) if (tidw[i]) ++trkHits; 
    for (int i = 0; i < 9; ++i) if (tecw[i]) ++trkHits; 

    return trkHits;
}
