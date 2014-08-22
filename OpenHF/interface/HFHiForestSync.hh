#ifndef HFHiForestSync_H
#define HFHiForestSync_H

// system include files
#include <memory>
#include <iostream>
#include <vector>
#include <string>
#include <map>

#include <TTree.h>
#include <TFile.h>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"


#include "../interface/map_info.hh"
#include "../interface/candAna.hh"

using namespace std;
//..
class HFHiForestSync : public edm::EDAnalyzer {

    private:
        TTree* hftree;
        TTree* T1;

        multimap<int, map_info> listOfHFTreeFile;

        TFile* map_run_lumi_file;

        map_info* run_lumi_file;

        TFile* hfTreeFile;

        candAna* ana;

        string nameOfMapRunLumiFile;
        string pathOfhftree; //.. directory path to the hftree file
        void read_map_run_lumi_file();

        string filename;
        string path_file; //.. directory path to the data file

        float cut_ffls3d;
        float cut_falpha0;

        bool findLumi(int lumi, vector<int>& list);

    private:
        edm::Service<TFileService> fs;

    private:
        virtual void beginJob() ;
        virtual void analyze(const edm::Event&, const edm::EventSetup&);
        virtual void endJob() ;

    public:
        explicit HFHiForestSync(const edm::ParameterSet&);
        ~HFHiForestSync();

};

#endif
