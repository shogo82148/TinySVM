#!/usr/bin/perl

#
# svm_learn.pl - svm_learn implementation using perl module
#

# $Id: svm_learn.pl,v 1.7 2001/08/29 15:12:29 taku-ku Exp $;
use TinySVM;

$model = $ARGV[$#ARGV];
$train = $ARGV[$#ARGV-1];
$param = join(" ",@ARGV[0..$#ARGV-2]);
die "not enough parameters\n" if (! $model || ! $train);

$e = new TinySVM::Example();
$e->read($train);
$m = $e->learn($param);

print "VC = ", $m->estimateVC(), "\n";
print "Margin = ", $m->estimateMargin(), "\n";
print "VCnum = ", $m->getSVnum(), "\n";
$m->write($model);
