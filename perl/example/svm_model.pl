#!/usr/bin/perl

#
# svm_view_model.pl - svm_model implementation using perl module
#

# $Id: svm_model.pl,v 1.2 2000/11/15 13:22:28 taku-ku Exp $;
use TinySVM;

my $model = $ARGV[$#ARGV];
die "FATAL: not enough parameters\n" if (! $model);

my $m = new Model();
$m->read($model);

my $margin = $m->estimateMargin();
printf("File Name:\t\t\t%s\n",$model);
printf("Margin:\t\t\t\t%g\n",$margin);
printf("Number of SVs:\t\t\t%d\n",$m->getSVnum());
printf("Estimated VC dimension:\t\t%g\n",$m->estimateVC());
