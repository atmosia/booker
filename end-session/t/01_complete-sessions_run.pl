#!/usr/bin/env perl

use v5.010;
use strict;
use warnings;

use lib '../perl';

use BookerTest qw(test_sql_statement clean_dir_sub run_tests fail booker_init
	          booker_create_session);
use Cwd;

my $HOME = $ENV{HOME};
my $cd = getcwd;

run_tests(
    { name  => "session in custom directory",
      pre   => [ sub { booker_init("../end-session/test-path") },
                 sub { booker_create_session("../end-session/test-path") }
	       ],
      cmd   => ["./booker-end-session", "-d", "test-path"],
      tests => [ test_sql_statement(
                    "counting special session users",
                    "test-path",
                    "SELECT COUNT(*) FROM session_user WHERE user = 'user'",
                    sub { $_[0]->[0] == 1 })
               ],
      clean => clean_dir_sub("test-path")
    },

    { name      => "double session end",
      pre       => [ \&booker_init, \&booker_create_session ],
      cmd       => ["./booker-end-session; " .
                    "./booker-end-session"],
      verify    => \&fail,
      clean     => clean_dir_sub("$HOME/.booker"),
    },

    { name      => "no directory",
      cmd       => ["./booker-end-session", "-d"],
      verify    => \&fail,
    },

    { name      => "empty directory directory",
      cmd       => ["./booker-end-session", "-d", ""],
      verify    => \&fail,
    },

    { name      => "help",
      cmd       => ["./booker-end-session", "-h"],
    },
   );
