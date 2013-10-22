#ifndef SNGLHFCAND_V0_H
#define SNGLHFCAND_V0_H

#include "snglhfcand.hh"

class snglhfcand_v0 : public snglhfcand
{
    private:
        //.. topology cut
        float fprob;
        float ff3d;
        float ff3de;
        float ffls3d;
        float fchi2;
        float falpha0;
        float fdr;

        //... parent meson infor
        int type;
        float fpt;
        float fm;
        float feta;
        float fphi;

        //.. daughter meson info ..
        float fmdau1;
        float fptdau1;
        float fetadau1;
        float fphidau1;

        //.. daughter single particle info
        float fptdau2;
        float fetadau2;
        float fphidau2;
        float fqdau2;

        //.. daughter of daughter meson info
        float fq1;
        float fpt1;
        float feta1;
        float fphi1;

        float fpt2;
        float fq2;
        float feta2;
        float fphi2;

        //..
        int nBPixLayer_dau2; //number Of BPixLayers
        int nPixLayer_dau2; //number Of PixLayers
        int nBPixLayer1Hits_dau2; //number Of BPixLayer1 Hits
        int nTrackerLayer_dau2; //number Of Tracker Layers

        int nBPixLayer_1; 
        int nPixLayer_1; 
        int nBPixLayer1Hits_1; 
        int nTrackerLayer_1; 

        int nBPixLayer_2; 
        int nPixLayer_2; 
        int nBPixLayer1Hits_2; 
        int nTrackerLayer_2; 


    private:
        void Reset();

    public:
        snglhfcand_v0() {Reset();}   
        ~snglhfcand_v0( ) {;}  

        //.. topology cut
        float get_fprob() const{return fprob;}
        float get_ff3d() const{return ff3d;}
        float get_ff3de() const{return ff3de;}
        float get_ffls3d() const{return ffls3d;}
        float get_fchi2() const{return fchi2;}
        float get_falpha0() const{return falpha0;}
        float get_fdr() const{return fdr;}

        //... parent meson infor
        int   get_type() const{return type;}
        float get_fpt() const{return fpt;}
        float get_fm() const{return fm;}
        float get_feta() const{return feta;}
        float get_fphi() const{return fphi;}

        //.. daughter meson info ..
        float get_fmdau1() const{return fmdau1;}
        float get_fptdau1() const{return fptdau1;}
        float get_fetadau1() const{return fetadau1;}
        float get_fphidau1() const{return fphidau1;}

        //.. daughter single particle info
        float get_fptdau2() const{return fptdau2;}
        float get_fetadau2() const{return fetadau2;}
        float get_fphidau2() const{return fphidau2;}
        float get_fqdau2() const{return fqdau2;}

        //.. daughter of daughter meson info
        float get_fq1() const{return fq1;}
        float get_fpt1() const{return fpt1;}
        float get_feta1() const{return feta1;}
        float get_fphi1() const{return fphi1;}

        float get_fpt2() const{return fpt2;}
        float get_fq2() const{return fq2;}
        float get_feta2() const{return feta2;}
        float get_fphi2() const{return fphi2;}

        int get_nBPixLayer_dau2() const{return nBPixLayer_dau2;} 
        int get_nPixLayer_dau2() const{return nPixLayer_dau2;} 
        int get_nBPixLayer1Hits_dau2() const{return nBPixLayer1Hits_dau2;} 
        int get_nTrackerLayer_dau2() const{return nTrackerLayer_dau2;} 

        int get_nBPixLayer_1() const{return nBPixLayer_1;} 
        int get_nPixLayer_1() const{return nPixLayer_1;} 
        int get_nBPixLayer1Hits_1() const{return nBPixLayer1Hits_1;} 
        int get_nTrackerLayer_1() const{return nTrackerLayer_1;} 

        int get_nBPixLayer_2() const{return nBPixLayer_2;} 
        int get_nPixLayer_2() const{return nPixLayer_2;} 
        int get_nBPixLayer1Hits_2() const{return nBPixLayer1Hits_2;} 
        int get_nTrackerLayer_2() const{return nTrackerLayer_2;} 

        //_________________________________________
        //.. topology cut                       
        void set_fprob(const float in) {   fprob = in;}
        void set_ff3d(const float in) {    ff3d = in;}
        void set_ff3de(const float in) {   ff3de = in;}
        void set_ffls3d(const float in) {  ffls3d = in;}
        void set_fchi2(const float in) {   fchi2 = in;}
        void set_falpha0(const float in) { falpha0 = in;}
        void set_fdr(const float in) {     fdr = in;}
                                                            
        //... parent meson infor                
        void set_type(const int in) {      type = in;}
        void set_fpt(const float in) {     fpt = in;}
        void set_fm(const float in) {      fm = in;}
        void set_feta(const float in) {    feta = in;}
        void set_fphi(const float in) {    fphi = in;}
                                                            
        //.. dauvoid seson info ..              
        void set_fmdau1(const float in) {  fmdau1 = in;}
        void set_fptdau1(const float in) { fptdau1 = in;}
        void set_fetadau1(const float in) {fetadau1 = in;}
        void set_fphidau1(const float in) {fphidau1 = in;}
                                                            
        //.. daughter single particle info      
        void set_fptdau2(const float in) { fptdau2 = in;}
        void set_fetadau2(const float in) {fetadau2 = in;}
        void set_fphidau2(const float in) {fphidau2 = in;}
        void set_fqdau2(const float in) {  fqdau2 = in;}
                                                            
        //.. daughter of daughter meson info    
        void set_fq1(const float in) {     fq1 = in;}
        void set_fpt1(const float in) {    fpt1 = in;}
        void set_feta1(const float in) {   feta1 = in;}
        void set_fphi1(const float in) {   fphi1 = in;}
                                                            
        void set_fpt2(const float in) {    fpt2 = in;}
        void set_fq2(const float in) {     fq2 = in;}
        void set_feta2(const float in) {   feta2 = in;}
        void set_fphi2(const float in) {   fphi2 = in;}

        void set_nBPixLayer_dau2(const int in) {nBPixLayer_dau2 = in;} 
        void set_nPixLayer_dau2(const int in) {nPixLayer_dau2 = in;} 
        void set_nBPixLayer1Hits_dau2(const int in) {nBPixLayer1Hits_dau2 = in;} 
        void set_nTrackerLayer_dau2(const int in) {nTrackerLayer_dau2 = in;} 

        void set_nBPixLayer_1(const int in) {nBPixLayer_1 = in;} 
        void set_nPixLayer_1(const int in) {nPixLayer_1 = in;} 
        void set_nBPixLayer1Hits_1(const int in) {nBPixLayer1Hits_1 = in;} 
        void set_nTrackerLayer_1(const int in) {nTrackerLayer_1 = in;} 

        void set_nBPixLayer_2(const int in) {nBPixLayer_2 = in;} 
        void set_nPixLayer_2(const int in) {nPixLayer_2 = in;} 
        void set_nBPixLayer1Hits_2(const int in) {nBPixLayer1Hits_2 = in;} 
        void set_nTrackerLayer_2(const int in) {nTrackerLayer_2 = in;} 

        ClassDef(snglhfcand_v0, 1)

};

#endif
