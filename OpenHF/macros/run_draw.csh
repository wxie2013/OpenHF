#! /bin/csh -f

foreach pid ($1)
foreach tid ($2)
foreach nrebin ($3)
foreach finName ($4)
foreach snglTrg ($5)

root -b<<EOF
gSystem->Load("$CMSSW_BASE/lib/$SCRAM_ARCH/libUserCodeOpenHF.so");

.x run_draw($pid, $tid, $rebin, "$finName", $snglTrg);
.q
#EOF

