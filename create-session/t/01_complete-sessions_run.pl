#!/usr/bin/env perl

use v5.010;
use strict;
use warnings;

use lib '../perl';

use BookerTest qw(test_sql_statement clean_dir_sub run_tests fail booker_init);

my $HOME = $ENV{HOME};

my @users;
for my $i (0..10) {
    push @users, "-u";
    push @users, "user$i";
}

run_tests(
    { name  => "default session",
      pre   => [ \&booker_init ],
      cmd   => ["./booker-create-session", "-u", "user"],
      tests => [ test_sql_statement(
                    "counting session users",
                    "$HOME/.booker",
                    "SELECT COUNT(*) FROM session_user WHERE user = 'user'",
                    sub { $_[0]->[0] == 1 })
               ],
      clean => clean_dir_sub("$HOME/.booker"),
    },

    { name  => "session in custom directory",
      pre   => [ sub { booker_init("../create-session/test-path") } ],
      cmd   => ["./booker-create-session", "-d", "test-path", "-u", "user"],
      tests => [ test_sql_statement(
                    "counting special session users",
                    "test-path",
                    "SELECT COUNT(*) FROM session_user WHERE user = 'user'",
                    sub { $_[0]->[0] == 1 })
               ],
      clean => clean_dir_sub("test-path")
    },

    { name      => "no user",
      pre       => [ \&booker_init ],
      cmd       => ["./booker-create-session"],
      verify    => \&fail,
      clean     => clean_dir_sub("$HOME/.booker")
    },

    { name      => "duplicate session",
      pre       => [ \&booker_init ],
      cmd       => ["./booker-create-session -u user; " .
                    "./booker-create-session -u user"],
      verify    => \&fail,
      clean     => clean_dir_sub("$HOME/.booker"),
    },

    { name  => "duplicate user",
      pre   => [ \&booker_init ],
      cmd   => ["./booker-create-session", "-u", "user", "-u", "user" ],
      tests => [ test_sql_statement(
                    "counting duplicate users",
                    "$HOME/.booker",
                    "SELECT COUNT(*) FROM session_user WHERE user = 'user'",
                    sub { $_[0]->[0] == 1 })
               ],
      clean => clean_dir_sub("$HOME/.booker"),
    },

    { name      => "max users",
      pre       => [ \&booker_init ],
      cmd       => ["./booker-create-session", @users],
      verify    => \&fail,
      clean     => clean_dir_sub("$HOME/.booker"),
    },

    { name      => "no directory",
      cmd       => ["./booker-create-session", "-d"],
      verify    => \&fail,
    },

    { name      => "empty directory directory",
      cmd       => ["./booker-create-session", "-d", ""],
      verify    => \&fail,
    },

    { name      => "help",
      cmd       => ["./booker-create-session", "-h"],
    },
   );
