#ifndef mapRunLumiHfTreeFile_H
#define mapRunLumiHfTreeFile_H

#include <vector>
#include <map>
#include <iostream>
#include <TFile.h>
#include <TNamed.h>
#include <TTree.h>

#include "map_info.hh"
#include "TAna01Event.hh"

using namespace std;

class mapRunLumiHfTreeFile : public TNamed 
{
    private:
        TFile *outfile;
        TFile *result;
        map_info* infor;
        TTree* tmap;
        TAna01Event* fpEvt;

        string filename;
        string pathname; //.. directory path to the data file
        string path_file; //.. directory path to the data file
    private:
        void Init(int startFile, int endFile);
        void get_infor(float& min, float& max, int lumi);
        void write_infor(map_info* infor, int run, int min, int max, string& fname);

    public:
        mapRunLumiHfTreeFile();
        virtual ~mapRunLumiHfTreeFile() {;}

        //..
        void set_pathname(string& in) {pathname = in;}
        void LoopOverFile(int startFile, int endFile, char *filelist);

        ClassDef(mapRunLumiHfTreeFile, 1)
};

#endif 
