#!/usr/local/bin/perl

$infile1 = $ARGV[0];
$base = "/home/wxie/CMSSW_6_2_9/src/UserCode/OpenHF/macros";
open(INPUTFILE1,$infile1);
@infile1_lines = <INPUTFILE1>;
close(INPUTFILE1);

foreach $s1 (@infile1_lines) {
    chomp($s1);

    if($s1 =~ m/\/tmp/g) {
        $end1 = pos($s1)-4;
    }
    if($s1 =~ m/\.err/g) {
        $end2 = pos($s1)-4;
    }

    $s2 = substr($s1, 0, $end1);
    $s3 = substr($s1, $end1+1, $end2-$end1-1);
    $s3 = "$s3\.job";

    system("cd $base/$s2; condor_submit $s3");

}

#e.g. the following is one file in the list. I manually checked its size>0, i.e. some error 
#...............
#Dstar2D0Pi_ydn_p2.0_yup_p2.5/tmp_condor_run_ana_hfforest.C_pPb.lis_9270_10294_iy10_ich0.err
#...............
