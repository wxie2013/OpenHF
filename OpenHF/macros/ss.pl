#!/usr/local/bin/perl

#e.g. sub.pl 10 dstar charm.lis cut_file.lis

$njobs = $ARGV[0];
$filelist = $ARGV[1];

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
    if($istart >= $itotal) {
        last;
    }

    print $istart, " ", $iend, "\n";
    system("./run_candAna.csh $istart $iend $filelist");
}
