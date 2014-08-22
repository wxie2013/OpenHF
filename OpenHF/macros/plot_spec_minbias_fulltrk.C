{
    TFile f1("D02KPi_comb.root");
    TFile f2("D02KPi_check_it0.root");

    f1.cd();
    hpt.SetLineWidth(2);
    hpt.SetLineColor(4);
    hpt.SetMaximum(1e8);
    hpt.Draw();
    f2.cd();
    hpt.SetLineWidth(2);
    hpt.SetLineColor(2);
    hpt.Draw("same");
}

