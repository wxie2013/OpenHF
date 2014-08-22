#ifndef SNGLHFCAND_H
#define SNGLHFCAND_H

#include "TNamed.h"
#include <iostream>
#include <vector>
using namespace std;

class snglhfcand : public TNamed
{
    private: 
        vector<int> zero;
    public:
        virtual int    get_pvn() const{WarningMSG(); return 0;}
        virtual int    get_pvw8() const{WarningMSG(); return 0;}
        virtual double get_pvlip() const{WarningMSG(); return 0;}
        virtual double get_pvlips() const{WarningMSG(); return 0;}
        virtual double get_pvlip2() const{WarningMSG(); return 0;}
        virtual double get_pvlips2() const{WarningMSG(); return 0;}
        virtual double get_pvip() const{WarningMSG(); return 0;}
        virtual double get_pvips() const{WarningMSG(); return 0;}
        virtual double get_pvip3d() const{WarningMSG(); return 0;}
        virtual double get_pvips3d() const{WarningMSG(); return 0;}
        virtual double get_pvdchi2() const{WarningMSG(); return 0;}

        //.. topology cut
        virtual float get_fprob() const{WarningMSG(); return 0;}
        virtual float get_ff3d() const{WarningMSG(); return 0;}
        virtual float get_ff3de() const{WarningMSG(); return 0;}
        virtual float get_ffls3d() const{WarningMSG(); return 0;}
        virtual float get_fchi2() const{WarningMSG(); return 0;}
        virtual float get_falpha0() const{WarningMSG(); return 0;}
        virtual float get_fdr() const{WarningMSG(); return 0;} 

        //... parent meson infor
        virtual int   get_type() const{WarningMSG(); return 0;} //..particle ID
        virtual float get_fpt() const{WarningMSG(); return 0;}
        virtual float get_fm() const{WarningMSG(); return 0;} 
        virtual float get_feta() const{WarningMSG(); return 0;}
        virtual float get_fphi() const{WarningMSG(); return 0;}

        //.. daughter meson info ..
        virtual float get_fmdau1() const{WarningMSG(); return 0;} //.. mass of daughter meson 
        virtual float get_fptdau1() const{WarningMSG(); return 0;} 
        virtual float get_fetadau1() const{WarningMSG(); return 0;}
        virtual float get_fphidau1() const{WarningMSG(); return 0;}

        //.. daughter single particle info
        virtual float get_fptdau2() const{WarningMSG(); return 0;}
        virtual float get_fetadau2() const{WarningMSG(); return 0;}
        virtual float get_fphidau2() const{WarningMSG(); return 0;}
        virtual float get_fqdau2() const{WarningMSG(); return 0;} //.. daughter of parent meson
        virtual int   get_Index_dau2() const {WarningMSG(); return 0;}
        virtual vector<int>  get_gIndex_dau2() const {WarningMSG(); return zero;}

        //.. daughter of daughter meson info
        virtual float get_fq1() const{WarningMSG(); return 0;} 
        virtual float get_fpt1() const{WarningMSG(); return 0;}
        virtual float get_feta1() const{WarningMSG(); return 0;}
        virtual float get_fphi1() const{WarningMSG(); return 0;}
        virtual int   get_Index1() const {WarningMSG(); return 0;}
        virtual vector<int>  get_gIndex1() const {WarningMSG(); return zero;}

        virtual float get_fpt2() const{WarningMSG(); return 0;}
        virtual float get_fq2() const{WarningMSG(); return 0;} 
        virtual float get_feta2() const{WarningMSG(); return 0;}
        virtual float get_fphi2() const{WarningMSG(); return 0;}
        virtual int   get_Index2() const {WarningMSG(); return 0;}
        virtual vector<int>  get_gIndex2() const {WarningMSG(); return zero;}

        virtual int get_nBPixLayer_dau2() const{WarningMSG(); return 0;}
        virtual int get_nPixLayer_dau2() const{WarningMSG(); return 0;}
        virtual int get_nBPixLayer1Hits_dau2() const{WarningMSG(); return 0;}
        virtual int get_nTrackerLayer_dau2() const{WarningMSG(); return 0;}

        virtual int get_nBPixLayer_1() const{WarningMSG(); return 0;}
        virtual int get_nPixLayer_1() const{WarningMSG(); return 0;}
        virtual int get_nBPixLayer1Hits_1() const{WarningMSG(); return 0;}
        virtual int get_nTrackerLayer_1() const{WarningMSG(); return 0;}

        virtual int get_nBPixLayer_2() const{WarningMSG(); return 0;}
        virtual int get_nPixLayer_2() const{WarningMSG(); return 0;}
        virtual int get_nBPixLayer1Hits_2() const{WarningMSG(); return 0;}
        virtual int get_nTrackerLayer_2() const{WarningMSG(); return 0;}


        //_________________________________________
        virtual void set_vtz(const float in) {WarningMSG();}
        virtual void set_pvn(const int in) {WarningMSG(); }
        virtual void set_pvw8(const int in) {WarningMSG(); }
        virtual void set_pvlip(const double in) {WarningMSG(); }
        virtual void set_pvlips(const double in) {WarningMSG(); }
        virtual void set_pvlip2(const double in) {WarningMSG(); }
        virtual void set_pvlips2(const double in) {WarningMSG(); }
        virtual void set_pvip(const double in) {WarningMSG(); }
        virtual void set_pvips(const double in) {WarningMSG(); }
        virtual void set_pvip3d(const double in) {WarningMSG(); }
        virtual void set_pvips3d(const double in) {WarningMSG(); }
        virtual void set_pvdchi2(const double in) {WarningMSG(); }

        //.. topology cut
        virtual void set_fprob(const float in) {WarningMSG();}
        virtual void set_ff3d(const float in) {WarningMSG();}
        virtual void set_ff3de(const float in) {WarningMSG();}
        virtual void set_ffls3d(const float in) {WarningMSG();}
        virtual void set_fchi2(const float in) {WarningMSG();}
        virtual void set_falpha0(const float in) {WarningMSG();}
        virtual void set_fdr(const float in) {WarningMSG();} 

        //... parent meson infor
        virtual void set_type(const int in) {WarningMSG();} //..particle ID
        virtual void set_fpt(const float in) {WarningMSG();}
        virtual void set_fm(const float in) {WarningMSG();} 
        virtual void set_feta(const float in) {WarningMSG();}
        virtual void set_fphi(const float in) {WarningMSG();}

        //.. dauvoid seson info ..
        virtual void set_fmdau1(const float in) {WarningMSG();} //.. mass of daughter meson 
        virtual void set_fptdau1(const float in) {WarningMSG();} 
        virtual void set_fetadau1(const float in) {WarningMSG();}
        virtual void set_fphidau1(const float in) {WarningMSG();}

        //.. daughter single particle info
        virtual void set_fptdau2(const float in) {WarningMSG();}
        virtual void set_fetadau2(const float in) {WarningMSG();}
        virtual void set_fphidau2(const float in) {WarningMSG();}
        virtual void set_fqdau2(const float in) {WarningMSG();} //.. daughter of parent meson
        virtual void set_Index_dau2(int in) {WarningMSG();} 
        virtual void set_gIndex_dau2(vector<int>& in) {WarningMSG();} 

        //.. daughter of daughter meson info
        virtual void set_fq1(const float in) {WarningMSG();} 
        virtual void set_fpt1(const float in) {WarningMSG();}
        virtual void set_feta1(const float in) {WarningMSG();}
        virtual void set_fphi1(const float in) {WarningMSG();}
        virtual void set_Index1(int in) {WarningMSG();} 
        virtual void set_gIndex1(vector<int>& in) {WarningMSG();} 

        virtual void set_fpt2(const float in) {WarningMSG();}
        virtual void set_fq2(const float in) {WarningMSG();} 
        virtual void set_feta2(const float in) {WarningMSG();}
        virtual void set_fphi2(const float in) {WarningMSG();}
        virtual void set_Index2(int in) {WarningMSG();} 
        virtual void set_gIndex2(vector<int>& in) {WarningMSG();} 

        virtual void set_nBPixLayer_dau2(const int in) {WarningMSG();}
        virtual void set_nPixLayer_dau2(const int in) {WarningMSG();}
        virtual void set_nBPixLayer1Hits_dau2(const int in) {WarningMSG();}
        virtual void set_nTrackerLayer_dau2(const int in) {WarningMSG();}

        virtual void set_nBPixLayer_1(const int in) {WarningMSG();}
        virtual void set_nPixLayer_1(const int in) {WarningMSG();}
        virtual void set_nBPixLayer1Hits_1(const int in) {WarningMSG();}
        virtual void set_nTrackerLayer_1(const int in) {WarningMSG();}

        virtual void set_nBPixLayer_2(const int in) {WarningMSG();}
        virtual void set_nPixLayer_2(const int in) {WarningMSG();}
        virtual void set_nBPixLayer1Hits_2(const int in) {WarningMSG();}
        virtual void set_nTrackerLayer_2(const int in) {WarningMSG();}

    private:
        void WarningMSG() const {cout<<"!!!!snglhfcand: the function not implemented in daughter class"<<endl;}

        ClassDef(snglhfcand, 2)
};

#endif
