#! /bin/csh -f

foreach startFile ($1)
foreach endFile ($2)
foreach filelist ($3)

root -b<<EOF
gSystem->Load("../../../../lib/slc5_amd64_gcc462/libUserCodeOpenHF.so");
.x CompareHLT.C($startFile, $endFile, "$filelist")
.q
#EOF

