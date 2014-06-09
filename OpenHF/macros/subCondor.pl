#!/usr/local/bin/perl

sub produce_small_file_segment;

$macro = $ARGV[0];
$njobs = $ARGV[1];
$start_from = $ARGV[2];
$filelist = $ARGV[3];
$iy = $ARGV[4];
$ich = $ARGV[5];

#
$BASE = "/home/wxie";
$workarea = "CMSSW_6_2_9/src/UserCode/OpenHF/macros";
@MesonName = ("Dstar2D0Pi", "D02KPi", "Ds2PhiPi", "Ds2KstarK", "Dpm2KPiPi");
@yup = ("m2.5", "m2.0", "m1.5", "m1.0", "m0.5", "0.0", "p0.5", "p1.0", "p1.5", "p2.0", "p2.5", "p3.0");
@ydn = ("m3.0", "m2.5", "m2.0", "m1.5", "m1.0", "m0.5", "0.0", "p0.5", "p1.0", "p1.5", "p2.0", "p2.5");
#-- move files in to different directories
$dir = "$MesonName[$ich]\_ydn\_$ydn[$iy]\_yup\_$yup[$iy]";
system("mkdir $dir; cd $dir; ln -s ../$filelist .; cd ../.");

#
open(FILE, "$filelist");
@lines = <FILE>;
close(FILE);

#chain file...
$chain = "chain\_$dir";
open(OUTFILE,">$chain");
print OUTFILE ("hadd $chain\.root \\\n");

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

    #-- output root file name 
    $outfile = "ana_hfforest_$MesonName[$ich]\_ydn\_$ydn[$iy]\_yup\_$yup[$iy]\_from$istart\To$iend.root";
    #-- create chain file using hadd
    print OUTFILE ("$outfile \\\n");


    #-- produce job exec file ...
    $job_execfile = "tmp_condor\_$macro\_$filelist\_$istart\_$iend\_iy$iy\_ich$ich";
    &produce_execfile;

    #-- produce condor configuration file ...
    $config_file = "$job_execfile\.job";
    &produce_config_file;

    #-- move each rapidity and channel into one directory
    system("mv $job_execfile $config_file $dir");
    system("cd $dir; condor_submit $config_file; cd ../.");

    if($iend == $itotal) {
        last;
    }
}
system("chmod u+x $chain; mv $chain $dir");


#-----------------------------------------------------
# produce condor configuation file for each file list  

sub produce_execfile {
    open(EXECFILE,">$job_execfile");

    print EXECFILE ("#!/bin/sh \n");
    print EXECFILE ("source /cvmfs/cms.cern.ch/cmsset_default.sh \n");
    print EXECFILE ("export X509_USER_PROXY=/home/wxie/.myproxy  \n");
    print EXECFILE ("cd $BASE/CMSSW_6_2_9/src \n");
    print EXECFILE ("eval `scramv1 runtime -sh` \n");
    print EXECFILE ("cd $BASE/$workarea/$dir \n");
    print EXECFILE ("root -b<<EOF\n");
    print EXECFILE ("gSystem->Load\(\"$BASE\/CMSSW_6_2_9\/lib\/slc6_amd64_gcc472\/libUserCodeOpenHF.so\"\)\; \n");
    print EXECFILE (".x $BASE/$workarea/$macro($istart, $iend, \"$filelist\", $iy, $ich, \"$outfile\") \n");
    print EXECFILE ("EOF\n");

    system("chmod u+x $job_execfile");
}
#-----------------------------------------------------
# produce condor configuation file for each file list  

sub produce_config_file {
    open(CONFIGFILE,">$config_file");

    print CONFIGFILE ("#======================================================================\n");
    print CONFIGFILE ("#\n");
    print CONFIGFILE ("# condor_submit condor_job\n");
    print CONFIGFILE ("#\n");
    print CONFIGFILE ("#======================================================================\n");
    print CONFIGFILE ("universe             = vanilla\n");
    print CONFIGFILE ("executable           = $job_execfile\n");
    print CONFIGFILE ("initialdir           = $BASE/$workarea/$dir\n");
    print CONFIGFILE ("should_Transfer_Files = NO\n");
    print CONFIGFILE ("#======================================================================\n");
    print CONFIGFILE ("output               = $job_execfile.out\n");
    print CONFIGFILE ("error                = $job_execfile.err\n");
    print CONFIGFILE ("log                  = $job_execfile.log\n");
    print CONFIGFILE ("#======================================================================\n");
    print CONFIGFILE ("# get the environment (path, etc.)\n");
    print CONFIGFILE ("Getenv         = True \n");
    print CONFIGFILE ("# prefer to run on fast computers\n");
    print CONFIGFILE ("Rank           = kflops\n");
    print CONFIGFILE ("#======================================================================\n");
    print CONFIGFILE ("\n");
    print CONFIGFILE ("requirements = (Arch == \"X86_64\")||regexp(\"cms\",Name) \n");
    print CONFIGFILE ("#======================================================================\n");
    print CONFIGFILE ("+CMSJob = True\n");
    print CONFIGFILE ("#+LENGTH=\"SHORT\"\n");
    print CONFIGFILE ("\n");
    print CONFIGFILE ("queue\n");
    print CONFIGFILE ("\n");
}
