#ifndef map_info_H
#define map_info_H

#include <TNamed.h>
#include <vector>
#include <string>

using namespace std;

class map_info: public TNamed
{
    private:
        int run;  //.. run number 
        int lumi_min;  //.. max lumi in this file 
        int lumi_max;  //.. min lumi in this file 
        string fname;  //.. file name 
        vector<int> listOfLumis;

        void reset();

    public:
        map_info();

        int get_run() {return run;}
        int get_lumi_min() {return lumi_min;}
        int get_lumi_max() {return lumi_max;}
        string get_fname() {return fname;}
        vector<int> get_listOfLumis() {return listOfLumis;}

        void set_run(int in) {run = in;}
        void set_lumi_min(int in) {lumi_min = in;}
        void set_lumi_max(int in) {lumi_max = in;}
        void set_fname(string& in) {fname = in;}
        void set_listOfLumis(vector<int>& in) {listOfLumis = in;}

        ClassDef(map_info,1)  

};
#endif
