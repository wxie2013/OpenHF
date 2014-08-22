#! /bin/csh -f

foreach startFile ($1)
foreach endFile ($2)
foreach filelist ($3)
foreach trg ($4)

root -b<<EOF
gSystem->Load("$CMSSW_BASE/lib/$SCRAM_ARCH/libUserCodeOpenHF.so");
.x run_mapRunLumiHfTreeFile.C($startFile, $endFile, "$filelist", $trg)
.q
#EOF

