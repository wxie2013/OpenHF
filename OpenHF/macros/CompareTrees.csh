#! /bin/csh -f

foreach startFile ($1)
foreach endFile ($2)
foreach filelist ($3)

root -b<<EOF
gSystem->Load("/afs/cern.ch/user/w/wxie/CMSSW_5_3_8_HI/lib/slc5_amd64_gcc462/libUserCodeOpenHF.so");
.x CompareTrees.C+($startFile, $endFile, "$filelist")
.q
#EOF

