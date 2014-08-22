#! /bin/csh -f

foreach hp0_in ($1)
foreach hp_in ($2)
foreach cut ($3)
foreach finName ($4)
foreach foutName ($5)

root -b<<EOF

gSystem->Load("$CMSSW_BASE/lib/$SCRAM_ARCH/libUserCodeOpenHF.so");

TFile* fin = new TFile("$finName");
TFile* fout = new TFile("$foutName", "recreate");

ana_hfforest *a = new ana_hfforest;

a->fillPPTrkHist($hp0_in, $hp_in, $cut, fin, fout);

.q
#EOF

