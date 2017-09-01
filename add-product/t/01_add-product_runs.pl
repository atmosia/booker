#!/usr/bin/env perl

use v5.010;
use strict;
use warnings;

use lib '../perl';

use BookerTest qw(run_tests booker_init clean_dir_sub);

my $HOME = $ENV{HOME};


run_tests(
    { name      => "default add-product",
      pre       => [ \&booker_init ],
      cmd       => ["./booker-add-product"],
      clean     => clean_dir_sub("$HOME/.booker"),
    },
);
