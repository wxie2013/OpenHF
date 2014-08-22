#! /bin/csh -f

foreach ich ($1)
foreach cut ($2)
foreach finName ($3)
foreach foutName ($4)
foreach noRandFire ($5)

root -b<<EOF

gSystem->Load("$CMSSW_BASE/lib/$SCRAM_ARCH/libUserCodeOpenHF.so");

TFile* fin = new TFile("$finName");
TFile* fout = new TFile("$foutName", "recreate");

ana_hfforest *a = new ana_hfforest;

a->fill_ntMass_hist($ich, $cut, fin, fout, $noRandFire);

.q
#EOF

