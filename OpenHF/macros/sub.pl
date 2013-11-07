#!/usr/local/bin/perl

$njobs = $ARGV[0];
$start_from = $ARGV[1];
$filelist = $ARGV[2];
$trg = $ARGV[3];

open(FILE, "$filelist");
@lines = <FILE>;
close(FILE);

$itotal = @lines;

print "number of files: $itotal \n";

$ntot = 0;
$is_last = 0;
for ($ifile = 0; $ifile < $njobs ; $ifile ++) {

    $nfiles = int($itotal/$njobs)+1;
    $istart = $ifile*$nfiles;
    $iend = $nfiles + $istart;
    
    if($iend > $itotal) {
        $iend = $itotal
    }

    if($istart < $start_from) {
        next;
    }

    if($istart >= $itotal) {
        last;
    }

    $to = "_";
    print $istart, " ", $iend, "\n";
    system("./run_mapRunLumiHfTreeFile.csh $istart $iend $filelist $trg > out_$istart$to$iend$to$trg\&");
}
