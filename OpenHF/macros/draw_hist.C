//.. note: the following are initial value and constraint on parameters for each histogram. 
//.. ------------------------- For Ds->Phi+pi ------------------------------
//     root> .x draw_hist.C+ ("ana_candAnaDs2PhiPi_ana_ds2PhiPi.lis_0To11.root", false, "ana_ds2Phi.gif", 4, 1.8, 2.1);
//     fg0: pol2 can not describe background 
//     initial value for all histograms: 
//     577 1.97 0.01 400 1.86 0.005
//     constraints for each histogram: 
//      hfg0
//      1.94 2.0 0.01 0.015 0 0 0 0 0 0
//      
//      hfg1
//      1.94 2.0 0.01 0.015 0 1e6 1.84 1.88 0 0.02
//      
//      hfg2
//      1.94 2.0 0.01 0.015 0 1e6 1.84 1.88 0 0.02
//      
//      hfg3
//      1.94 2.0 0.01 0.015 0 1e6 1.84 1.88 0 0.02
//      
//      hfg4
//      1.94 2.0 0.01 0.015 0 100 1.85 1.9  0.001 0.01
//      
//      
//      hfg5
//      1.94 2.0 0.01 0.03 0 100 1.85  1.9  0.000 0.01
//      
//      
//      hfg6
//      1.94 2.0 0.01 0.05 0 100 1.85 1.9  0.002 0.01
//.. ------------------------- For D0->kaon+pi ------------------------------
//     initial value for all histograms: 
//     root>  .x draw_hist.C+ ("ana_D0_tot.root", false, "ana_d0.gif", 5, 1.78, 1.95);
//      1000 1.87 0.02 0 0 0
//     constraints for all histograms: 
//      1.84 1.9 0 0.05 0 0 0 0 0 0
//.. ------------------------- For Dpm->kaon+pi+pi ------------------------------
//     initial value for all histograms: 
//     root>  .x draw_hist.C+ ("ana_Dpm_tot.root", false, "ana_dpm.gif", 5, 1.78, 2.0);
//      1000 1.87 0.005 0 0 0
//     constraints for all histograms: 
//      1.84 1.9 0 0.03 0 0 0 0 0 0
//
//      hfg6
//      1.8 1.9 0 0.05 0 0 0 0 0 0


#include <iostream>

#include <TFile.h>
#include <TH1F.h>
#include <TF1.h>
#include <TCanvas.h>

const int nhist = 20;
using namespace std;

void draw_hist(char* filename, bool draw_diff = false, char* outfile="oo.root", int n_rebin = 5, float rlow = 1.8, float rhigh = 2.1)
{//..note:  draw_diff=true only for D* ..

    TFile* f = new TFile(filename);

    //. get histogram
    TH1* hfg[nhist] = {0};
    TH1* hbg[nhist] = {0};
    TH1* hdau[nhist] = {0};
    TH1* hfgdiff[nhist] = {0};
    TH1* hbgdiff[nhist] = {0};
    char hname[200];
    for(short i = 0; i<nhist; i++) {
        sprintf(hname, "hfg%d", i);
        hfg[i] = (TH1F*)f->Get(hname)->Clone();

        sprintf(hname, "hbg%d", i);
        hbg[i] = (TH1F*)f->Get(hname)->Clone(); 

        sprintf(hname, "hdau%d", i);
        hdau[i] =  (TH1F*)f->Get(hname)->Clone();

        sprintf(hname, "hfgdiff%d", i);
        hfgdiff[i] =  (TH1F*)f->Get(hname)->Clone();

        sprintf(hname, "hbgdiff%d", i);
        hbgdiff[i] =  (TH1F*)f->Get(hname)->Clone();
    }

    //... draw ...
    TCanvas* cfg = new TCanvas("cfg", "", 1000, 1000);
    cfg->Divide(3, 3);

    for(short i = 0; i<9; i++) {
        cfg->cd(i+1);

        if(!draw_diff) {
            hfg[i]->SetMarkerSize(0.8);
            hfg[i]->Rebin(n_rebin);
            hbg[i]->Rebin(n_rebin);

            hfg[i]->SetLineColor(2);
            hfg[i]->SetMarkerColor(2);
            hfg[i]->SetMarkerStyle(20);
            hfg[i]->GetXaxis()->SetNdivisions(505);

            hfg[i]->GetXaxis()->SetRangeUser(rlow, rhigh);
            //.. fit with a Gaussian and pol
            TF1* fit_fun = new TF1("fit_fun", "gaus(0) + gaus(3) + pol2(6)", rlow, rhigh);
            float max = hfg[i]->GetMaximum();

            int pass = 0;
            float p0 = max, p1 = 1.97, p2 = 0.02;
            float p3 = max, p4 = 1.88, p5 = 0.02;
            float p0_L = 0, p1_L = 1.80, p2_L = 0.001;
            float p0_H = 2*max, p1_H = 2.1, p2_H = 0.5;
            float p3_L = 0, p4_L = 1.80, p5_L = 0.001;
            float p3_H = 2*max, p4_H = 2.1, p5_H = 0.5;
            while(!pass) {
                //.. initial value 
                fit_fun->SetParameter(0, p0);
                fit_fun->SetParameter(1, p1);
                fit_fun->SetParameter(2, p2);
                fit_fun->SetParameter(3, p3);
                fit_fun->SetParameter(4, p4);
                fit_fun->SetParameter(5, p5);

                //.. fit constraint ..
                fit_fun->SetParLimits(0, p0_L, p0_H);
                fit_fun->SetParLimits(1, p1_L, p1_H);
                fit_fun->SetParLimits(2, p2_L, p2_H);

                if(p3_L==0 && p3_H==0) {
                    //.. asked to fix all parameters of 2nd gaussian to zero
                    fit_fun->FixParameter(3, 0);
                    fit_fun->FixParameter(4, 0);
                    fit_fun->FixParameter(5, 0);
                } else {
                    fit_fun->SetParLimits(3, p3_L, p3_H);
                    fit_fun->SetParLimits(4, p4_L, p4_H);
                    fit_fun->SetParLimits(5, p5_L, p5_H);
                }

                //.. fit
                hfg[i]->Fit(fit_fun,  "", "", rlow, rhigh);

                //.. draw foreground and background ..
                hfg[i]->Draw();

                TF1* fit_fun_2nd = (TF1*)fit_fun->Clone("fit_fun_2nd");
                fit_fun_2nd->SetParameter(0, 0);
                fit_fun_2nd->SetParameter(1, 0);
                fit_fun_2nd->SetParameter(2, 0);

                fit_fun_2nd->SetLineColor(4);
                fit_fun_2nd->Draw("same");

                TF1* fit_fun_bg = (TF1*)fit_fun->Clone("fit_fun_bg");
                fit_fun_bg->SetParameter(0, 0);
                fit_fun_bg->SetParameter(1, 0);
                fit_fun_bg->SetParameter(2, 0);
                fit_fun_bg->SetParameter(3, 0);
                fit_fun_bg->SetParameter(4, 0);
                fit_fun_bg->SetParameter(5, 0);

                fit_fun_bg->SetLineColor(8);
                fit_fun_bg->Draw("same");
                gPad->Update();

                //.. check if need to fit again ...
                cout<<" good fit?"<<endl;
                scanf("%d", &pass);

                if(!pass) {
                    cout<<" p0, p1, p2, p3, p4, p5 ?"<<endl;
                    scanf("%f%f%f%f%f%f", &p0, &p1, &p2, &p3, &p4, &p5);

                    cout<<" p1_L, p1_H, p2_L, p2_H, p3_L, p3_H, p4_L, p4_H, p5_L, p5_H ?"<<endl;
                    scanf("%f%f%f%f%f%f%f%f%f%f", &p1_L, &p1_H, &p2_L, &p2_H, &p3_L, &p3_H, &p4_L, &p4_H, &p5_L, &p5_H);

                    //cout<<" rlow, rhigh? "<<endl;
                    //scanf("%f%f", &rlow, &rhigh);
                }
            }


            /* side band only works for dstar since it calculate the difference ...
            int ib_low = hbg[i]->FindBin(2.05);
            int ib_high = hbg[i]->FindBin(2.12);
            float scale = hfg[i]->Integral(ib_low, ib_high)/hbg[i]->Integral(ib_low, ib_high);
            hbg[i]->Scale(scale);
            hbg[i]->Draw("histsame");
            */

        } else {
            hfgdiff[i]->SetMarkerSize(0.8);
            hfgdiff[i]->Rebin(n_rebin);
            hbgdiff[i]->Rebin(n_rebin);
            hbgdiff[i]->SetLineWidth(2);

            hfgdiff[i]->SetLineColor(2);
            hfgdiff[i]->SetMarkerColor(2);
            hfgdiff[i]->SetMarkerStyle(20);
            hfgdiff[i]->GetXaxis()->SetNdivisions(505);

            hfgdiff[i]->Draw();

            int ib_low = hbgdiff[i]->FindBin(0.149);
            int ib_high = hbgdiff[i]->FindBin(0.153);
            float scale = hfgdiff[i]->Integral(ib_low, ib_high)/hbgdiff[i]->Integral(ib_low, ib_high);
            hbgdiff[i]->Scale(scale);
            hbgdiff[i]->Draw("histsame");
        }
    }

    cfg->SaveAs(outfile);
}
