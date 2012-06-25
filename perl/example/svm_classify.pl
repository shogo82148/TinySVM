#!/usr/bin/perl

#
# svm_classify.pl - svm_classify implementation using perl module
#

# $Id: svm_classify.pl,v 1.5 2001/08/29 15:00:15 taku-ku Exp $;

use TinySVM;
use Getopt::Std;
getopts("v");

my $model = $ARGV[$#ARGV];
my $test  = $ARGV[$#ARGV-1];
die "not enough parameters\n" if (! $model || ! $test);

my $m = new TinySVM::Model();
$m->read($model);

my($all,$ok);
open(F,$test) || die "$! $test\n";
while(<F>) {
    chomp;
    if  (/^\s*([-\+]?1\.?0*)\s+/) {
	$all++;
	my($l,$v) = split(/\s+/,$_,2);
	my($r) = $m->classify($v);
	printf("%g %g\n",$l,$r) if ($opt_v);
	$ok++ if ($r * $l > 0);
    } else {
	printf("%g\n",$m->classify($_));
    }
}
close(F);

printf("Accuracy = %10.10f%% (%d/%d)\n",100.0*$ok/$all,$ok,$all) if ($all);
