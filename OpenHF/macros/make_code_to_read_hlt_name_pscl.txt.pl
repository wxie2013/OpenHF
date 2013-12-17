#!/usr/local/bin/perl

# e.g. ./make_code_to_read_hlt_name_pscl.txt.pl hlt_name_pscl.txt oo.C

$filelist = $ARGV[0];
$job_execfile = $ARGV[1];

open(OUTPUTFILE,">$job_execfile");
open(FILE, "$filelist");
@all_lines = <FILE>;
close(FILE);

$it = 0;
$ipscle = 0;
foreach $line(@all_lines) {
    chomp($line);
    if($line !~ m/_Prescl/) {
        $trg_name[$it] = $line;
        $trg_name[$it] =~ s/_v.*//;
        $trg_name[$it] =~ s/\s+$//;  # remove the space at the end of the string which cause trouble when doing string comparison
        $it++;
    }
}


for($i =0; $i<$it; $i++) {
    print OUTPUTFILE ("trg_name[$i]= \"$trg_name[$i]\";\n");
}
