#!/usr/local/bin/perl

sub produce_small_file_segment;

$njobs = $ARGV[0];
$start_from = $ARGV[1];
$filelist = $ARGV[2];

$BASE = "/home/wxie";
$workarea = "$BASE/CMSSW_5_3_8_HI/src/UserCode/OpenHF/macros";

open(FILE, "$filelist");
@lines = <FILE>;
close(FILE);

$itotal = @lines;

print "number of files: $itotal \n";


$ntot = 0;
$nfiles = int($itotal/$njobs)+1;
for ($ifile = 0; $ifile < $njobs ; $ifile ++) {

    $istart = $ifile*$nfiles;
    $iend = $nfiles + $istart;

    if($iend >$itotal) {
        $iend = $itotal;
    }

    if($istart < $start_from) {
        next;
    }

    #-- produce job exec file ...
    $job_execfile = "tmp_run\_$type\_$istart\_$iend";
    &produce_execfile;

    #-- produce condor configuration file ...
    $config_file = "tmp_condor\_$type\_$istart\_$iend.job";
    &produce_config_file;

    system("condor_submit $config_file");
    #system("./$job_execfile");

    if($iend == $itotal) {
        last;
    }
}


#-----------------------------------------------------
# produce condor configuation file for each file list  

sub produce_execfile {
    open(OUTPUTFILE,">$job_execfile");

    print OUTPUTFILE ("#!/bin/sh \n");
    print OUTPUTFILE ("source /cvmfs/cms.cern.ch/cmsset_default.sh \n");
    print OUTPUTFILE ("DIR=\"CMSSW_5_3_8_HI/src/UserCode/OpenHF/macros\" \n");
    print OUTPUTFILE ("cd $BASE/CMSSW_5_3_8_HI \n");
    print OUTPUTFILE ("eval `scramv1 runtime -sh` \n");
    print OUTPUTFILE ("cd $BASE/\$DIR \n");
    print OUTPUTFILE ("root -b<<EOF\n");
    print OUTPUTFILE ("gSystem->Load\(\"\.\.\/\.\.\/\.\.\/\.\.\/lib\/slc5_amd64_gcc462\/libUserCodeOpenHF.so\"\)\; \n");
    print OUTPUTFILE (".x run_ana_hfforest.C($istart, $iend, \"$filelist\") \n");
    print OUTPUTFILE ("EOF\n");

    system("chmod u+x $job_execfile");
}
#-----------------------------------------------------
# produce condor configuation file for each file list  

sub produce_config_file {
    open(OUTPUTFILE,">$config_file");

    print OUTPUTFILE ("#======================================================================\n");
    print OUTPUTFILE ("#\n");
    print OUTPUTFILE ("# condor_submit condor_job\n");
    print OUTPUTFILE ("#\n");
    print OUTPUTFILE ("#======================================================================\n");
    print OUTPUTFILE ("universe             = vanilla\n");
    print OUTPUTFILE ("executable           = $job_execfile\n");
    print OUTPUTFILE ("initialdir           = $workarea\n");
    print OUTPUTFILE ("should_Transfer_Files = NO\n");
    print OUTPUTFILE ("#======================================================================\n");
    print OUTPUTFILE ("output               = $job_execfile.out\n");
    print OUTPUTFILE ("error                = $job_execfile.err\n");
    print OUTPUTFILE ("log                  = $job_execfile.log\n");
    print OUTPUTFILE ("#======================================================================\n");
    print OUTPUTFILE ("# get the environment (path, etc.)\n");
    print OUTPUTFILE ("Getenv         = True \n");
    print OUTPUTFILE ("# prefer to run on fast computers\n");
    print OUTPUTFILE ("Rank           = kflops\n");
    print OUTPUTFILE ("#======================================================================\n");
    print OUTPUTFILE ("\n");
    print OUTPUTFILE ("requirements = (Arch == \"X86_64\")||regexp(\"cms\",Name) \n");
    print OUTPUTFILE ("#======================================================================\n");
    print OUTPUTFILE ("+CMSJob = True\n");
    print OUTPUTFILE ("#+LENGTH=\"SHORT\"\n");
    print OUTPUTFILE ("\n");
    print OUTPUTFILE ("queue\n");
    print OUTPUTFILE ("\n");
}
