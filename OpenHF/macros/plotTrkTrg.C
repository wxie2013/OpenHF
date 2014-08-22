{
    TFile f0("D02KPi_comb.root");
    TFile f1("D02KPi_check_it1.root");
    TFile f2("D02KPi_check_it2.root");
    TFile f3("D02KPi_check_it3.root");

    TH1F* hpt0 = f0.Get("hpt")->Clone("h0");
    TH1F* hpt1 = f1.Get("hpt")->Clone("h1");
    TH1F* hpt2 = f2.Get("hpt")->Clone("h2");
    TH1F* hpt3 = f3.Get("hpt")->Clone("h3");

    TH1F* rhpt1 = f1.Get("hpt")->Clone("rh1");
    TH1F* rhpt2 = f2.Get("hpt")->Clone("rh2");
    TH1F* rhpt3 = f3.Get("hpt")->Clone("rh3");

    hpt0->SetLineColor(2);
    hpt1->SetLineColor(1);
    hpt2->SetLineColor(4);
    hpt3->SetLineColor(8);

    rhpt1->SetLineColor(1);
    rhpt2->SetLineColor(4);
    rhpt3->SetLineColor(8);

    hpt0->SetLineWidth(2);
    hpt1->SetLineWidth(2);
    hpt2->SetLineWidth(2);
    hpt3->SetLineWidth(2);

    rhpt1->Divide(hpt0);
    rhpt2->Divide(hpt0);
    rhpt3->Divide(hpt0);

    cc = new TCanvas("cc", "", 1000, 600);
    cc->Divide(2, 1);
    cc->cd(1);
    gPad->SetLogy(1);
    gPad->SetGridx(1);
    gPad->SetGridy(1);
    hpt0->Draw();
    hpt1->Draw("same");
    hpt2->Draw("same");
    hpt3->Draw("same");
    hpt0->Draw("same");
    cc->cd(2);
    gPad->SetGridx(1);
    gPad->SetGridy(1);
    rhpt1->SetLineWidth(2);
    rhpt2->SetLineWidth(2);
    rhpt3->SetLineWidth(2);

    rhpt1->SetMinimum(0);
    rhpt1->SetMaximum(2);
    rhpt1->Draw();
    rhpt2->Draw("same");
    rhpt3->Draw("same");
}
