#!/usr/local/bin/perl

sub produce_small_file_segment;

$njobs = $ARGV[0];
$meson = $ARGV[1];
$filelist = $ARGV[2];
$cutfile = $ARGV[3];

$HOME = "/home/wxie";
$BASE = "/usr/rmt_share/scratch96/w/wxie";
$workarea = "$BASE/CMSSW_5_3_8_HI/src/HeavyFlavorAnalysis/Bs2MuMu/macros2";

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

    #-- produce file segment 
    $file_seg = "$meson$ifile.lis";
    &produce_small_file_segment;

    #-- produce job exec file ...
    $job_execfile = "run_$file_seg";
    &produce_execfile;

    #-- produce condor configuration file ...
    $config_file = "condor_$file_seg.job";
    &produce_config_file;

    #system("condor_submit $config_file");
    system("./$job_execfile");

    if($is_last==1) {
        last;
    }

}

#-----------------------------
# produce small file segment
sub produce_small_file_segment {

    open(OUTPUTFILE,">$file_seg");
    for ($isub = $istart; $isub < $iend ; $isub ++) {
        chomp($lines[$isub]);
        print OUTPUTFILE ($lines[$isub],"\n");

        $ntot++;

        if($ntot >= $itotal) {
            $is_last = 1;
            return;
        }
    }
}

#-----------------------------------------------------
# produce condor configuation file for each file list  

sub produce_execfile {
    open(OUTPUTFILE,">$job_execfile");

    print OUTPUTFILE ("#!/bin/sh \n");
    print OUTPUTFILE ("source /cvmfs/cms.cern.ch/cmsset_default.sh \n");
    print OUTPUTFILE ("source /opt/osg/setup.sh \n");
    print OUTPUTFILE ("DIR=\"CMSSW_5_3_8_HI/src/HeavyFlavorAnalysis/Bs2MuMu/macros2\" \n");
    print OUTPUTFILE ("cd $BASE/CMSSW_5_3_8_HI \n");
    print OUTPUTFILE ("eval `scramv1 runtime -sh` \n");
    print OUTPUTFILE ("cd $BASE/\$DIR \n");
    print OUTPUTFILE ("bin/runHfReader -c $file_seg -C $cutfile \n");
    print OUTPUTFILE ("mv $file_seg Data/2GeV_cut/.\n");
    print OUTPUTFILE ("mv $file_seg.$cutfile.root Data/2GeV_cut/.\n");
    print OUTPUTFILE ("mv $job_execfile Data/2GeV_cut/.\n");
    print OUTPUTFILE ("mv $job_execfile.* Data/2GeV_cut/.\n");
    print OUTPUTFILE ("mv condor_$file_seg.job Data/2GeV_cut/.\n");

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
    print OUTPUTFILE ("\n");
    print OUTPUTFILE ("requirements = (Arch == \"X86_64\")||regexp(\"cms\",Name) \n");
    print OUTPUTFILE ("#======================================================================\n");
    print OUTPUTFILE ("+CMSJob = True\n");
    print OUTPUTFILE ("#+LENGTH=\"SHORT\"\n");
    print OUTPUTFILE ("\n");
    print OUTPUTFILE ("queue\n");
    print OUTPUTFILE ("\n");
}
