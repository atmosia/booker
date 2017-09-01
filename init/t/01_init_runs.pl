#!/usr/bin/env perl

use v5.010;
use strict;
use warnings;

use lib '../perl';

use BookerTest qw(table_exists_sub fail clean_dir_sub run_tests
                  dir_test_sub file_test_sub);

my $HOME = $ENV{HOME};

sub test_files {
    [dir_test_sub("create directory", $_[0]),
     file_test_sub("create config", "$_[0]/config.ini"),
     file_test_sub("create db", "$_[0]/db.sqlite3"),
    ]
}

sub test_db {
   [table_exists_sub("open session table", $_[0], "session"),
    table_exists_sub("open session_user table", $_[0], "session_user"),
    table_exists_sub("open type table", $_[0], "type"),
    table_exists_sub("open product table", $_[0], "product"),
    ]
}

run_tests(
    { name      => "default init",
      cmd       => ["./booker-init"],
      tests     => [@{test_files("$HOME/.booker")},
                    @{test_db("$HOME/.booker")},
                   ],
      clean     => clean_dir_sub("$HOME/.booker"),
    },
    { name      => "custom init",
      cmd       => ["./booker-init", "-d", "test-path"],
      tests     => [@{test_files("test-path")},
                    @{test_db("test-path")},
                   ],
      clean     => clean_dir_sub("test-path"),
    },
    { name      => "no directory",
      cmd       => ["./booker-init", "-d"],
      verify    => \&fail,
      tests     => [["not create directory", sub { ! -d "$HOME/.booker" }]],
    },
    { name      => "empty directory",
      cmd       => ["./booker-init", "-d", ""],
      verify    => \&fail,
      tests     => [["not create directory", sub { ! -d "$HOME/.booker" }]],
    },
    { name      => "double initialization",
      cmd       => ["./booker-init; ./booker-init"],
      verify    => \&fail,
      clean     => clean_dir_sub("$HOME/.booker"),
    },
    { name      => "help",
      cmd       => ["./booker-init", "-h"]
    },
);
