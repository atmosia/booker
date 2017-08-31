#!/usr/bin/env perl

use v5.010;
use strict;
use warnings;

use Cwd;
use File::Path qw(rmtree);

my $HOME = $ENV{HOME};

my $cd = getcwd;
my $exit = 0;
chdir("../init");
system("./booker-init");
chdir($cd);
$exit++ if system("./booker-create-session");
rmtree("$HOME/.booker");

chdir("../init");
system("./booker-init", "-d", "../create-session/test-path");
chdir($cd);
$exit++ if system("./booker-create-session", "-d", "test-path");
rmtree("test-path");

$exit++ unless system("./booker-create-session", "-d");
rmtree("test-path");

$exit++ unless system("./booker-create-session", "-d", "");
rmtree("test-path");

exit $exit;
