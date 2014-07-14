#include "FWCore/Framework/interface/MakerMacros.h"
#include "../interface/HFHiForestSync.hh"

#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"

HFHiForestSync::HFHiForestSync(const edm::ParameterSet& iConfig)
{
    nameOfMapRunLumiFile = iConfig.getUntrackedParameter<string>("nameOfMapRunLumiFile");
    pathOfhftree = iConfig.getUntrackedParameter<string>("pathOfhftree");
    cut_ffls3d = iConfig.getUntrackedParameter<double>("cut_ffls3d");
    cut_falpha0 = iConfig.getUntrackedParameter<double>("cut_falpha0");

    hftree = fs->make<TTree>("hftree" , "heavy flavor reduced tree");
    ana = new candAna;
    ana->Init(hftree);
    ana->set_cut(cut_ffls3d, cut_falpha0);

    hftree = 0;
    T1 = 0;

    read_map_run_lumi_file();
}

//_________________________________________
HFHiForestSync::~HFHiForestSync()
{
}


//_____________________________
void HFHiForestSync::read_map_run_lumi_file()
{
    //.. read the file that correlated run:lumiBlock:fileName ....
    try {
        map_run_lumi_file = new TFile(nameOfMapRunLumiFile.c_str());
        cout<<" -> reading map file: "<<nameOfMapRunLumiFile.c_str()<<endl;
    } catch (cms::Exception &ex) {
        cout<<" ==> the "<<nameOfMapRunLumiFile.c_str()<<" does not exist. exit <=== "<<endl;
        exit(0);
    }

    //
    run_lumi_file = new map_info;

    TTree* tmap = (TTree*) map_run_lumi_file->Get("tmap");
    tmap->SetBranchAddress("map_info", &run_lumi_file);

    for(int i = 0; i<tmap->GetEntries(); i++) {
        tmap->GetEntry(i);

        int run = run_lumi_file->get_run();
        listOfHFTreeFile.insert(pair<int, map_info>(run, *run_lumi_file));
    }

    /*
    for(multimap<int, map_info>::iterator tt = listOfHFTreeFile.begin(); tt!=listOfHFTreeFile.end(); tt++) {
        cout<<(*tt).first<<" "<<(*tt).second.get_run()<<" "<<(*tt).second.get_fname()<<endl;
    }
    */

    cout<<"--> done <--"<<endl;
}
// ------------ method called for each event  ------------
    void
HFHiForestSync::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    static int lumi_min_last = -1;
    static int lumi_max_last = -1;

    int lumi = iEvent.luminosityBlock();
    Long64_t evtNum = iEvent.id().event(); 
    Long64_t runNum = iEvent.id().run(); 

    pair<multimap<int, map_info>::iterator, multimap<int, map_info>::iterator> iter;
    iter = listOfHFTreeFile.equal_range(runNum);

    if(!(lumi>=lumi_min_last && lumi <=lumi_max_last)) {

        bool match = false;

        for (multimap<int, map_info>::iterator it = iter.first; it != iter.second; ++it) {
            int lumi_min = (*it).second.get_lumi_min(); 
            int lumi_max = (*it).second.get_lumi_max(); 
            if(lumi>=lumi_min && lumi <=lumi_max) {
                vector<int> listOfLumis = (*it).second.get_listOfLumis();
                if(findLumi(lumi, listOfLumis)) {
                    match = true;
                    lumi_min_last = lumi_min;
                    lumi_max_last = lumi_max;

                    filename = (*it).second.get_fname();

                    path_file = pathOfhftree+"/"+filename;

                    TFile* f = TFile::Open(path_file.c_str());
                    if(!f) {
                        cout<<"!! The file: "<<filename<<" does not exist. Stop and make sure it is there"<<endl;
                        exit(0);
                    }
                    if(f->IsZombie()) {
                        cout<<"!! The file: "<<filename<<" is a Zombie"<<endl;
                        match = false;
                        break;
                    }

                    T1 = 0;
                    if(!(T1 = (TTree*) f->Get("T1")))
                        T1 = (TTree*) f->Get("tree/fTree");

                    cout<<" --> found file: "<<path_file<<endl;
                    break;
                }
            }
        }

        if(!match) 
            T1 = 0; //..don't have a match
    }

    if(!T1) {
        cout<<" !! did not find HFTree file for run: "<<runNum<<" evt: "<<evtNum<<endl;
    }

    ana->LoopOverEvt(T1, runNum, evtNum);
}
//
bool HFHiForestSync::findLumi(int lumi, vector<int>& list)
{
    int nlumi = list.size();
    for(int i = 0; i<nlumi; i++) {
        if(lumi == list.at(i))
            return true;
    }

    return false;
}

// ------------ method called once each job just before starting event loop  ------------
    void 
HFHiForestSync::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
    void 
HFHiForestSync::endJob() 
{
}

//define this as a plug-in
DEFINE_FWK_MODULE(HFHiForestSync);
