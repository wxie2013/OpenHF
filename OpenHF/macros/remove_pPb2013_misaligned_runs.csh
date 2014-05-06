#! /bin/csh -f

foreach startFile ($1)
foreach endFile ($2)
foreach filelist ($3)

root -b<<EOF
gSystem->Load("~/CMSSW_5_3_8_HI/lib/slc5_amd64_gcc462/libUserCodeOpenHF.so");
.x remove_pPb2013_misaligned_runs.C+($startFile, $endFile, "$filelist")
.q
#EOF

