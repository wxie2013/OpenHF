#!/usr/local/bin/perl

$infile1 = $ARGV[0];
$base = "/home/wxie/CMSSW_6_2_9/src/UserCode/OpenHF/macros";
open(INPUTFILE1,$infile1);
@infile1_lines = <INPUTFILE1>;
close(INPUTFILE1);

foreach $s1 (@infile1_lines) {
    chomp($s1);

    if($s1 =~ m/\/chain/g) {
        $end1 = pos($s1)-6;
    }
    if($s1 =~ m/\*/g) {
        $end2 = pos($s1)-1;
    }

    $s2 = substr($s1, 0, $end1);
    $s3 = substr($s1, $end1+1, $end2-$end1-1);

    system("cd $base/$s2; $s3");

}

#e.g. the following is one file in the list. I manually checked its size>0, i.e. some error 
#...............
#Dpm2KPiPi_ydn_p1.0_yup_p1.5/chain_Dpm2KPiPi_ydn_p1.0_yup_p1.5*
#...............
