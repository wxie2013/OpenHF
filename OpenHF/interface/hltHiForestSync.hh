#ifndef hltHiForestSync_H
#define hltHiForestSync_H
 

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
#include "DataFormats/HLTReco/interface/TriggerObject.h"

#include "../interface/map_info.hh"

typedef std::vector<trigger::TriggerObject> trigO;

const int ntrg = 14;
using namespace std;
//..
class hltHiForestSync : public edm::EDAnalyzer {

    private:
        TTree* hltTree;
        TTree* T1;

        multimap<int, map_info> listOfHFTreeFile;

        TFile* map_run_lumi_file;

        map_info* run_lumi_file;

        string nameOfMapRunLumiFile;
        string pathOfhftree; //.. directory path to the hftree file
        void read_map_run_lumi_file();

        string filename;
        string path_file; //.. directory path to the data file

        int trigflag[ntrg];
        const char* trg_name[ntrg];
        trigO *trigObject[ntrg];

        int fRun;
        int fEvt;
    private:
        edm::Service<TFileService> fs;

    private: 
        void reset();
        void LoopOverEvt(TTree* T1, Long64_t& run_hifst, Long64_t& evt_hifst);
        void get_trg_name();
        void get_trg_info(TTree* T);
        bool findLumi(int lumi, vector<int>& list);

    private:
        virtual void beginJob() ;
        virtual void analyze(const edm::Event&, const edm::EventSetup&);
        virtual void endJob() ;

    public:
        explicit hltHiForestSync(const edm::ParameterSet&);
        ~hltHiForestSync();

};
#endif

