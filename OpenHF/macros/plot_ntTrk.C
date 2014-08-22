void plot_ntTrk(int nrb) 
{
    hPtTrk0->SetLineColor(5);
    hPtTrk1->SetLineColor(3);
    hPtTrk2->SetLineColor(4);
    hPtTrk3->SetLineColor(1);
    hPtTrkComb->SetLineColor(2);
    //hPtTrk0.Draw();
    hPtTrk0->Rebin(nrb);
    hPtTrk1->Rebin(nrb);
    hPtTrk2->Rebin(nrb);
    hPtTrk3->Rebin(nrb);
    hPtTrkComb->Rebin(nrb);
    hPtTrk1->SetAxisRange(0, 200);

    TH1F* r_hPtTrk0 = hPtTrk0->Clone("r_hPtTrk0");
    TH1F* r_hPtTrk1 = hPtTrk1->Clone("r_hPtTrk1");
    TH1F* r_hPtTrk2 = hPtTrk2->Clone("r_hPtTrk2");
    TH1F* r_hPtTrk3 = hPtTrk3->Clone("r_hPtTrk3");

    r_hPtTrk0->Divide(hPtTrkComb);
    r_hPtTrk1->Divide(hPtTrkComb);
    r_hPtTrk2->Divide(hPtTrkComb);
    r_hPtTrk3->Divide(hPtTrkComb);
    r_hPtTrk1->SetMinimum(0);
    r_hPtTrk1->SetMaximum(2);

    r_hPtTrk1->SetLineWidth(2);
    r_hPtTrk2->SetLineWidth(2);
    r_hPtTrk3->SetLineWidth(2);

    cc = new TCanvas("cc", "", 800, 500);
    cc->Divide(2, 1);
    cc->cd(1);
    gPad->SetLogy(1);
    gPad->SetGridx(1);
    gPad->SetGridy(1);
    hPtTrk1->Draw("ehist");
    hPtTrk2->Draw("ehistsame");
    hPtTrk3->Draw("ehistsame");
    hPtTrkComb->Draw("histsame");
    cc->cd(2);
    gPad->SetGridx(1);
    gPad->SetGridy(1);
    r_hPtTrk1->Draw("ehist");
    r_hPtTrk2->Draw("ehistsame");
    r_hPtTrk3->Draw("ehistsame");
}
