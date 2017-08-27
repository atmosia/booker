#!/usr/bin/env perl

use strict;

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
      tests     => [["create directory", sub { -d "$HOME/.booker" }]],
      clean     => sub { rmdir("$HOME/.booker") },
    },
    { cmd       => ["./booker-init", "-d", "test-path"],
      verify    => \&pass,
      tests     => [["create directory", sub { -d "test-path" }]],
      clean     => sub { rmdir("test-path") }
    },
    { cmd       => ["./booker-init", "-d"],
      verify    => \&fail,
      tests     => [["not create directory", sub { ! -d "$HOME/.booker" }]],
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
