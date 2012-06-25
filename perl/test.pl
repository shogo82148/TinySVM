# Before `make install' is performed this script should be runnable with
# `make test'. After `make install' it should work as `perl test.pl'

######################### We start with some black magic to print on failure.

# Change 1..1 below to 1..last_test_to_print .
# (It may become useful if the test is moved to ./t subdirectory.)

# $Id: test.pl,v 1.12 2001/08/24 13:07:49 taku-ku Exp $;

use lib "../src/.libs";
use lib $ENV{PWD} . "/blib/lib";
use lib $ENV{PWD} . "/blib/arch";
BEGIN { $| = 1; print "1..1\n"; }
END {print "not ok 1\n" unless $loaded;}

use TinySVM;
$loaded = 1;

my $e = new TinySVM::Example;
$e->read("../tests/train.svmdata");

print "Example Size = ", $e->size(),"\n";
$m = $e->learn("-t 3 -d 2 -c 100");
$m->write("model");

for $i (0..5) {
   my $a = $m->getX($i);
   print $m->getY($i) , " $a\n";
}

print "\n";
$m->remove(0);
for $i (0..5) {
   my $a = $m->getX($i);
   print $m->getY($i) , " $a\n";
}

$m->writeSVindex("svindex");

$m->clear();

$m = new TinySVM::Model();
$m->read("model");

print "classify=", $m->classify("1:1 2:1"), "\n";
print "classify=", $m->classify("4:1 6:1"), "\n";
print "SV=" ,      $m->getSVnum() , "\n";
print "Margin=" ,  $m->estimateMargin() , "\n";
print "VC=" ,      $m->estimateVC() , "\n";

unlink "svindex";
unlink "model";

my $m2 = $e->learn ();
$m2->compress();
$m2->write("model2");
unlink "model2";

print "ok 1\n";
