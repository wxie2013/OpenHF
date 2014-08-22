#! /bin/csh -f

foreach ich($1)
foreach finName ($2)
foreach foutName ($3)

root -b<<EOF

gSystem->Load("$CMSSW_BASE/lib/$SCRAM_ARCH/libUserCodeOpenHF.so");

TFile* fin = new TFile("$finName");
TFile* fout = new TFile("$foutName", "recreate");

ana_hfforest *a = new ana_hfforest;

a->get_hist($ich, fin);
a->rebinSpec($ich, fout);
a->CombineSpec($ich, fout);

.q
#EOF

