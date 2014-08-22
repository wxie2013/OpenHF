#!/usr/local/bin/perl

sub produce_small_file_segment;

$macro = $ARGV[0];
$njobs = $ARGV[1];
$start_from = $ARGV[2];
$filelist = $ARGV[3];
$iy = $ARGV[4];
$ich = $ARGV[5];
$ppTrk = $ARGV[6];
$CMSSW_BASE = $ARGV[7];
$SCRAM_ARCH = $ARGV[8];
#
$workarea = "$CMSSW_BASE/src/UserCode/OpenHF/macros";
@MesonName = ("Dstar2D0Pi", "D02KPi", "Ds2PhiPi", "Ds2KstarK", "Dpm2KPiPi");
#@yup = ("m2.5", "m2.0", "m1.5", "m1.0", "m0.5", "0.0", "p0.5", "p1.0", "p1.5", "p2.0", "p2.5", "p3.0");
#@ydn = ("m3.0", "m2.5", "m2.0", "m1.5", "m1.0", "m0.5", "0.0", "p0.5", "p1.0", "p1.5", "p2.0", "p2.5");
@yup = ("p2.0");
@ydn = ("m2.0");
#-- move files in to different directories
$dir = "$MesonName[$ich]\_ydn\_$ydn[$iy]\_yup\_$yup[$iy]\_$filelist";
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

#print EXECFILE ("#!/bin/sh \n");
#print EXECFILE ("source /cvmfs/cms.cern.ch/cmsset_default.sh \n");
#print EXECFILE ("export X509_USER_PROXY=/home/wxie/.myproxy  \n");
    print EXECFILE ("cd $CMSSW_BASE/src \n");
#print EXECFILE ("eval `scramv1 runtime -sh` \n");
    print EXECFILE ("cd $workarea/$dir \n");
    print EXECFILE ("root -b<<EOF\n");
    print EXECFILE ("gSystem->Load\(\"$CMSSW_BASE\/lib\/$SCRAM_ARCH\/libUserCodeOpenHF.so\"\)\; \n");
    print EXECFILE (".x $workarea/$macro($istart, $iend, \"$filelist\", $iy, $ich, \"$outfile\", $ppTrk\) \n");
    print EXECFILE ("EOF\n");

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
    print OUTPUTFILE ("initialdir           = $workarea/$dir\n");
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
    print OUTPUTFILE ("Requirements   = Arch == \"X86_64\" && regexp(\"cms\",Name) && TARGET.UidDomain == \"rcac.purdue.edu\" && TARGET.Machine != \"airplay-render1.rcac.purdue.edu\" && TARGET.Machine != \"airplay-render2.rcac.purdue.edu\"\n");
    print OUTPUTFILE ("#======================================================================\n");
    print OUTPUTFILE ("+CMSJob = True\n");
    print OUTPUTFILE ("#+LENGTH=\"SHORT\"\n");
    print OUTPUTFILE ("\n");
    print OUTPUTFILE ("queue\n");
    print OUTPUTFILE ("\n");
}
