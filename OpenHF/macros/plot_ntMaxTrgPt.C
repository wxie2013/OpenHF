void plot_ntMaxTrgPt(char* infile, char* outfile)
{
    TFile fout(outfile, "recreate");
    TFile fin(infile);

    TNtuple* ntMaxTrgPt = fin.Get("ntMaxTrgPt");

    TH1F* h1 = new TH1F("h1", "", 1000, 0, 1000);
    TH1F* h2 = new TH1F("h2", "", 1000, 0, 1000);
    TH1F* h3 = new TH1F("h3", "", 1000, 0, 1000);
    TH1F* h4 = new TH1F("h4", "", 1000, 0, 1000);
    TH1F* h5 = new TH1F("h5", "", 1000, 0, 1000);
    TH1F* h6 = new TH1F("h6", "", 1000, 0, 1000);
    TH1F* h7 = new TH1F("h7", "", 1000, 0, 1000);
    TH1F* h8 = new TH1F("h8", "", 1000, 0, 1000);
    TH1F* h9 = new TH1F("h9", "", 1000, 0, 1000);
    TH1F* h10 = new TH1F("h10", "", 1000, 0, 1000);
    TH1F* h11 = new TH1F("h11", "", 1000, 0, 1000);
    TH1F* h12 = new TH1F("h12", "", 1000, 0, 1000);

    TH1F* h100 = new TH1F("h100", "", 1000, 0, 1000);;
    TH1F* h101 = new TH1F("h101", "", 1000, 0, 1000);;
    TH1F* h102 = new TH1F("h102", "", 1000, 0, 1000);;

    TH1F* h200 = new TH1F("h200", "", 1000, 0, 1000);;
    TH1F* h201 = new TH1F("h201", "", 1000, 0, 1000);;
    TH1F* h202 = new TH1F("h202", "", 1000, 0, 1000);;
    TH1F* h203 = new TH1F("h203", "", 1000, 0, 1000);;
    TH1F* h204 = new TH1F("h204", "", 1000, 0, 1000);;

    TH1F* h300 = new TH1F("h300", "", 1000, 0, 1000);;
    TH1F* h301 = new TH1F("h301", "", 1000, 0, 1000);;
    TH1F* h302 = new TH1F("h302", "", 1000, 0, 1000);;
    TH1F* h303 = new TH1F("h303", "", 1000, 0, 1000);;

    TH1F* hTrkTrg = new TH1F("hTrkTrg", "", 1000, 0, 1000);
    TH1F* hJetTrg = new TH1F("hJetTrg", "", 1000, 0, 1000);
    TH1F* hPhoTrg = new TH1F("hPhoTrg", "", 1000, 0, 1000);

    char name[100], cond[100];
    for(int i = 1; i<=12; i++)  {
        sprintf(name, "ptmax>>h%d", i);
        sprintf(cond, "(itrg==%d)*weight", i);
        ntMaxTrgPt->Draw(name, cond);
    }

    for(int i = 100; i<=102; i++)  {
        sprintf(name, "ptmax>>h%d", i);
        sprintf(cond, "(itrg==%d)*weight", i);
        ntMaxTrgPt->Draw(name, cond);
    }

    for(int i = 200; i<=204; i++)  {
        sprintf(name, "ptmax>>h%d", i);
        sprintf(cond, "(itrg==%d)*weight", i);
        ntMaxTrgPt->Draw(name, cond);
    }

    for(int i = 300; i<=303; i++)  {
        sprintf(name, "ptmax>>h%d", i);
        sprintf(cond, "(itrg==%d)*weight", i);
        ntMaxTrgPt->Draw(name, cond);
    }

    hTrkTrg->Add(h100);
    hTrkTrg->Add(h101);
    hTrkTrg->Add(h102);

    hJetTrg->Add(h200);
    hJetTrg->Add(h201);
    hJetTrg->Add(h202);
    hJetTrg->Add(h203);
    hJetTrg->Add(h204);

    hPhoTrg->Add(h300);
    hPhoTrg->Add(h301);
    hPhoTrg->Add(h302);
    hPhoTrg->Add(h303);

    fout->cd();
    h1->Write();
    h2->Write();
    h3->Write();
    h4->Write();
    h5->Write();
    h6->Write();
    h7->Write();
    h8->Write();
    h9->Write();
    h10->Write();
    h11->Write();
    h12->Write();

    h100->Write();
    h101->Write();
    h102->Write();

    h200->Write();
    h201->Write();
    h202->Write();
    h203->Write();
    h204->Write();

    h300->Write();
    h301->Write();
    h302->Write();
    h303->Write();

    hTrkTrg->Write();
    hJetTrg->Write();
    hPhoTrg->Write();
}
