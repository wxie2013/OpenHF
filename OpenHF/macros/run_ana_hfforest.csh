#! /bin/csh -f

foreach startFile ($1)
foreach endFile ($2)
foreach filelist ($3)
foreach iy ($4)
foreach ich ($5)
foreach outfile ($6)

root -b<<EOF
gSystem->Load("$CMSSW_BASE/lib/$SCRAM_ARCH/libUserCodeOpenHF.so");
.x run_ana_hfforest.C($startFile, $endFile, "$filelist", $iy, $ich, "$outfile")
.q
#EOF

