{
    c1 = new TCanvas("c1", "", 700, 500);
    TFile f1("D02KPi_check_it0.root");
    TFile f2("D02KPi_comb.root");
    hpt.SetMaximum(1e8);
    c1->SetLogy(1);
    hpt.Draw();
    f1.cd();
    hpt.Draw("Same");

}

