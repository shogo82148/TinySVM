package TinySVM;

# $Id: TinySVM.pm,v 1.5 2000/12/07 10:14:00 taku-ku Exp $;

use strict;
use Carp;
require Exporter;
require DynaLoader;
use vars qw($VERSION @ISA @EXPORT @EXPORT_OK);

@ISA = qw(Exporter DynaLoader);
$VERSION = '0.01';

bootstrap TinySVM $VERSION;

1;
__END__

=head1 NAME

Perl extension for TinySVM (tiny Support Vector Machine Package)

=head1 SYNOPSIS

    # training examples stored in some file
    $e = new Example();
    $e->read("training");
    $m = $e->learn("-t 1 -d 2 -c 10"); # return instance of Model object

    # classify
    print $m->classify("1:1 2:1 3:1") ,"\n";
    print $m->classify_list(qw(1 2 3)) ,"\n";

    # store model instance to some file
    $m->write("model");

    # example information
    print $e->size() , "\n";        

    # model information
    print $m->getSVnum()       ,"\n";
    print $m->estimateMargin() ,"\n";
    print $m->estimateVC()     ,"\n";

    # re-learning using distinct parameter
    $m2 = $e->learn("-t 1 -d 5 -c 1");
    $m2->write("model2");

    # append to some file
    $m2->append("model");

    # add some exmples 
    $e->add(1,"1:1 3:1 4:1 10:1");
    $e->add(-1,"3:1 5:1 4:1 9:1");
    $e->add("1 1:1 3:1 4:1 10:1 11:1"); # single argument

    # clear
    $e->clear();
    $m->clear();

    $m3 = $e->learn("-t 1 -d 5 -c 1");
    $m2->write("model3");

    # get specific example and model
    my @t = $e->getX(10);
    my $y = $e->getY(10);

    my @t = $m->getX(2);
    my $y = $m->getY(2);

    # classsify using stored model file
    $m4 = new Model();
    $m4->read("model3");
    print $m4->classify("1:1 2:1 3:1") ,"\n";

=head1 DESCRIPTION

Stub documentation for TinySVM, created by h2xs. It looks like the
author of the extension was negligent enough to leave the stub
unedited.

=head2 EXPORT

None by default.

=head1 AUTHOR

Taku Kudoh  <taku-ku@is.aist-nara.ac.jp>

=head1 SEE ALSO

svm_learn(1). svm_classify(1). svm_model(1). perl(1). 

=cut
