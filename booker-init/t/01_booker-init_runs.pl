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

my $HOME = $ENV{HOME};

my @cmds = (
    { cmd       => ["./booker-init"],
      verify    => \&pass,
      tests     => [["create directory", sub { -d "$HOME/.booker" }],
                    ["create config", sub { -f "$HOME/.booker/config.ini" }],
                   ],
      clean     => sub { rmtree("$HOME/.booker") },
    },
    { cmd       => ["./booker-init", "-d", "test-path"],
      verify    => \&pass,
      tests     => [["create directory", sub { -d "test-path" }],
                    ["create config", sub { -f "test-path/config.ini" }],
                   ],
      clean     => sub { rmtree("test-path") }
    },
    { cmd       => ["./booker-init", "-d"],
      verify    => \&fail,
      tests     => [["not create directory", sub { ! -d "$HOME/.booker" }]],
    },
    { cmd       => ["./booker-init; ./booker-init"],
      verify    => \&fail,
      clean     => sub { rmtree("$HOME/.booker") },
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
