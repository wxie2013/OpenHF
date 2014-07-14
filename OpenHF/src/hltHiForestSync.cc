//.. this is to sync the re-run hlt trigObj with HIForest production ...
#include "FWCore/Framework/interface/MakerMacros.h"
#include "../interface/hltHiForestSync.hh"

#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"

hltHiForestSync::hltHiForestSync(const edm::ParameterSet& iConfig)
{
    nameOfMapRunLumiFile = iConfig.getUntrackedParameter<string>("nameOfMapRunLumiFile");
    pathOfhftree = iConfig.getUntrackedParameter<string>("pathOfhftree");

    for(int i = 0; i<NTRG; i++) {
        trigObject[i] = new trigO;
    }


    get_trg_name();

    hltTree = fs->make<TTree>("hltTree" , "rerun hlt tree to add trigObj");
    hltTree->Branch("Run", &fRun, "Run/I");
    hltTree->Branch("Event", &fEvt, "Event/I");
    for (int itrig = 0; itrig != NTRG; ++itrig) {
        TString trigName(trg_name[itrig]);
        hltTree->Branch(trigName,trigflag+itrig,trigName+"/I");
        hltTree->Branch(trigName+"_trigObject",&trigObject[itrig]);
    }

    read_map_run_lumi_file();

    T1 = 0;
}

//_________________________________________
hltHiForestSync::~hltHiForestSync()
{
    for(int i = 0; i<NTRG; i++)
        delete trigObject[i];
}


//_____________________________
void hltHiForestSync::read_map_run_lumi_file()
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

    cout<<"--> done <--"<<endl;
}
// ------------ method called for each event  ------------
    void
hltHiForestSync::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
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
                    T1 = (TTree*) f->Get("hltanalysis/HltTree");

                    get_trg_info(T1); //.. SetBranchAddress for trigger obj

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

    LoopOverEvt(T1, runNum, evtNum);
}
//
bool hltHiForestSync::findLumi(int lumi, vector<int>& list)
{
    int nlumi = list.size();
    for(int i = 0; i<nlumi; i++) {
        if(lumi == list.at(i))
            return true;
    }

    return false;
}
//
void hltHiForestSync::get_trg_name()
{
    trg_name[0]= "HLT_PAZeroBiasPixel_SingleTrack"; //.. dominate in minbiasUPC sample.
    trg_name[1]= "HLT_PAFullTrack12";   //.. base: L1_SingleJet12_BptxAND w/ prescl=1
    trg_name[2]= "HLT_PAFullTrack20";   //.. base: L1_SingleJet16_BptxAND w/ prescl=1
    trg_name[3]= "HLT_PAFullTrack30";   //.. base: L1_SingleJet16_BptxAND w/ prescl=1
    trg_name[4]= "L1_ZeroBias";  //.. base of HLT_PAZeroBiasPixel_SingleTrack

    trg_name[5]= "HLT_PAJet20_NoJetID";   //.. base: L1_SingleJet16_BptxAND
    trg_name[6]= "HLT_PAJet40_NoJetID";   //.. base: L1_SingleJet16_BptxAND
    trg_name[7]= "HLT_PAJet60_NoJetID";   //.. base: L1_SingleJet36
    trg_name[8]= "HLT_PAJet80_NoJetID";   //.. base: L1_SingleJet36
    trg_name[9]= "HLT_PAJet100_NoJetID";   //.. base: L1_SingleJet36

    trg_name[10]= "HLT_PAPhoton10_NoCaloIdVL";   //.. base: L1_SingleEG5_BptxAND
    trg_name[11]= "HLT_PAPhoton15_NoCaloIdVL";   //.. base: L1_SingleEG5_BptxAND
    trg_name[12]= "HLT_PAPhoton20_NoCaloIdVL";   //.. base: L1_SingleEG5_BptxAND
    trg_name[13]= "HLT_PAPhoton30_NoCaloIdVL";   //.. base: L1_SingleEG12

}

// ----------------------------------------------------------------------
void hltHiForestSync::LoopOverEvt(TTree* T1, Long64_t& run_hifst, Long64_t& evt_hifst) 
{
    if(!T1) {
        hltTree->Fill();  //.. no re-run HLT 
        return;
    }

    int maxEvents = (int)T1->GetEntries();

    //.. run and event number 
    fRun = -1, fEvt = -1;
    T1->SetBranchAddress("Run", &fRun);
    T1->SetBranchAddress("Event", &fEvt);
    TBranch* b_evt = T1->GetBranch("Event");
    TBranch* b_run = T1->GetBranch("Run");

    bool match = false;
    for (int jEvent = 0; jEvent < maxEvents; ++jEvent) {
        b_run->GetEntry(jEvent);
        b_evt->GetEntry(jEvent);

        if(fEvt == evt_hifst && fRun == run_hifst) {//.. sync with hiforest 
            reset();

            T1->GetEntry(jEvent);          
            match = true;

            hltTree->Fill();

            return;
        } 
    }
    if(!match) {
        cout<<" --> no matching hltTree for this event <--"<<endl;
        hltTree->Fill();  //.. no re-run HLT. Fill in an empty object for that event.
    }
}

//
void hltHiForestSync::get_trg_info(TTree* T)
{
    reset();

    TObjArray *branches = (TObjArray *)T->GetListOfBranches();
    TIter branch_iter(branches);
    TBranch* branch = 0;
    while ((branch = (TBranch *)branch_iter.Next())) {
        TString branch_name = branch->GetName();
        for(int it = 0; it<NTRG; it++) {
            if (branch_name.Contains(trg_name[it])){
                if(branch_name.Contains("Prescl")) {
                    //.. do nothing here
                } else if(branch_name.Contains("trigObject")) {
                    T->SetBranchAddress(branch_name.Data(), &trigObject[it]);
                } else {
                    T->SetBranchAddress(branch_name.Data(), &trigflag[it]);
                }

                break;
            }
        }
    }
}

//
void hltHiForestSync::reset()
{//.. reset trigger decision every event
    for(int i = 0; i<NTRG; i++) {
        trigflag[i] = 0;
        trigObject[i]->clear();
    }
}

// ------------ method called once each job just before starting event loop  ------------
    void 
hltHiForestSync::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
    void 
hltHiForestSync::endJob() 
{
}

//define this as a plug-in
DEFINE_FWK_MODULE(hltHiForestSync);
