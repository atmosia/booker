#!/usr/bin/env perl

use strict;

my @cmds = (
    ["./booker-init"],
    ["./booker-init", "-d", "test"],
);

my $exit = 0;
for my $cmd (@cmds) {
    printf("running `@$cmd'\n");
    if (system(@$cmd)) {
        printf(STDERR "failed to run $cmd\n");
        $exit = 1;
    }
}

exit $exit;
