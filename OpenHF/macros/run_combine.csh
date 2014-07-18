#! /bin/csh -f

foreach finName ($1)
foreach foutName ($2)

root -b<<EOF

gSystem->Load("$CMSSW_BASE/lib/$SCRAM_ARCH/libUserCodeOpenHF.so");

TFile* fin = new TFile("$finName");
TFile* fout = new TFile("$foutName", "recreate");

ana_hfforest *a = new ana_hfforest;

a->get_hist(fin);
a->rebinSpec(fout);
a->CombineSpec(fout);

.q
#EOF

