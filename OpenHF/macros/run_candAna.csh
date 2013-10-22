#! /bin/csh -f

foreach startFile ($1)
foreach endFile ($2)
foreach filelist ($3)

root -b<<EOF
.x run_candAna.C($startFile, $endFile, "$filelist")
.q
#EOF

