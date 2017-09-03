#!/usr/bin/env perl

use v5.010;
use strict;
use warnings;

use lib '../perl';

use BookerTest qw(run_tests booker_init booker_create_session
                  clean_dir_sub test_sql_statement fail);

my $HOME = $ENV{HOME};

run_tests(
    { name      => "add type",
      pre       => [ \&booker_init, \&booker_create_session ],
      cmd       => [ "./booker-add-type", "book" ],
      tests     => [ test_sql_statement(
                        "checking new type added",
                        "$HOME/.booker",
                        "SELECT COUNT(*) FROM type WHERE name = 'book'",
                        sub { $_[0]->[0] == 1 })
                   ],
      clean     => clean_dir_sub("$HOME/.booker"),
    },
    { name      => "add type custorm dir",
      pre       => [ sub { booker_init("../add-type/test-path") },
                     sub { booker_create_session("../add-type/test-path") },
                   ],
      cmd       => [ "./booker-add-type", "-d", "../add-type/test-path",
                     "book" ],
      tests     => [ test_sql_statement(
                        "checking new type added",
                        "test-path",
                        "SELECT COUNT(*) FROM type WHERE name = 'book'",
                        sub { $_[0]->[0] == 1 })
                   ],
      clean     => clean_dir_sub("test-path"),
    },
    { name      => "add double add type",
      pre       => [ \&booker_init, \&booker_create_session ],
      cmd       => [ "./booker-add-type book; ./booker-add-type book" ],
      verify    => \&fail,
      clean     => clean_dir_sub("$HOME/.booker"),
    },
    { name      => "add type outside session",
      pre       => [ \&booker_init ],
      cmd       => [ "./booker-add-type", "book" ],
      verify    => \&fail,
      clean     => clean_dir_sub("$HOME/.booker"),
    },
    { name      => "no dir provided",
      cmd       => [ "./booker-add-type", "-d" ],
      verify    => \&fail,
    },
    { name      => "empty dir provided",
      cmd       => [ "./booker-add-type", "-d", "" ],
      verify    => \&fail,
    },
    { name      => "help",
      cmd       => ["./booker-add-type", "-h"],
    },
   );
