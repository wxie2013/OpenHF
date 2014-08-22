#! /bin/csh -f

foreach pid ($1)
foreach tid ($2)
foreach Nrebin ($3)
foreach finName ($4)

root -b<<EOF
gSystem->Load("$CMSSW_BASE/lib/$SCRAM_ARCH/libUserCodeOpenHF.so");

TFile* fin = new TFile("$finName");

ana_hfforest *a = new ana_hfforest;

a->drawHighMult($pid, $tid, $Nrebin, fin);

fin->Close();

.q
#EOF

