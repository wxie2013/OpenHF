{
    TFile f1("D02KPi_check_it1_without_randFire.root");
    TFile f2("D02KPi_check_it1_with_randFire.root");

    f1.cd();
    hpt.SetLineWidth(2);
    hpt.SetLineColor(2);
    hpt.Draw();
    f2.cd();
    hpt.SetLineWidth(2);
    hpt.SetLineColor(1);
    hpt.Draw("same");
}

