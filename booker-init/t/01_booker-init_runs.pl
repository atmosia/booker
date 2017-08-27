#!/usr/bin/env perl

use strict;

sub pass {
    $_[0]
}

sub fail {
    !$_[0]
}

my @cmds = (
    [["./booker-init"], \&pass],
    [["./booker-init", "-d", "test"], \&pass],
    [["./booker-init", "-d"], \&fail],
);

my $exit = 0;
for my $cmd (@cmds) {
    printf("running `@{$cmd->[0]}'\n");
    if ($cmd->[1](system(@{$cmd->[0]}))) {
        printf(STDERR "failed to run @{$cmd->[0]}\n");
        $exit = 1;
    }
}

exit $exit;
