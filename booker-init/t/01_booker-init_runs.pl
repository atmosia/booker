#!/usr/bin/env perl

use v5.010;
use strict;

use File::Path qw(rmtree);

sub pass {
    $_[0]
}

sub fail {
    !$_[0]
}

sub clean_dir {
    my $tree = $_[0];
    sub { rmtree($tree) }
}

sub test_files {
    my $path = $_[0];
    [["create directory", sub { -d "$path" }],
     ["create config", sub { -f "$path/config.ini" }],
     ["create db", sub { -f "$path/db.sqlite3" }],
    ]
}

my $HOME = $ENV{HOME};

my @cmds = (
    { cmd       => ["./booker-init"],
      verify    => \&pass,
      tests     => test_files("$HOME/.booker"),
      clean     => clean_dir("$HOME/.booker"),
    },
    { cmd       => ["./booker-init", "-d", "test-path"],
      verify    => \&pass,
      tests     => test_files("test-path"),
      clean     => clean_dir("test-path"),
    },
    { cmd       => ["./booker-init", "-d"],
      verify    => \&fail,
      tests     => [["not create directory", sub { ! -d "$HOME/.booker" }]],
    },
    { cmd       => ["./booker-init; ./booker-init"],
      verify    => \&fail,
      clean     => clean_dir("$HOME/.booker"),
    },
);

my $exit = 0;
for my $cmd (@cmds) {
    my @cmd = @{$cmd->{cmd}};
    printf("# running `@cmd'\n");
    if ($cmd->{verify}(system(@cmd))) {
        printf(STDERR "failed to run @cmd\n");
        $exit = 1;
    }
    printf("# testing output\n");
    for my $test (@{$cmd->{tests}}) {
        unless ($test->[1]()) {
            printf(STDERR "failed to $test->[0]\n");
            $exit = 1;
        }
    }
    printf("# cleaning up\n");
    $cmd->{clean}() if $cmd->{clean};
}

exit $exit;
