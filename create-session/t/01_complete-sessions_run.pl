#!/usr/bin/env perl

use v5.010;
use strict;
use warnings;

use Cwd;
use DBI;
use File::Path qw(rmtree);

my $HOME = $ENV{HOME};

my $cd = getcwd;
my $exit = 0;

# test default session
chdir("../init");
system("./booker-init");
chdir($cd);
$exit++ if system("./booker-create-session", "-u", "user");
my $dbh = DBI->connect("dbi:SQLite:dbname=$HOME/.booker/db.sqlite3", "", "");
my $rows = $dbh->selectrow_arrayref("SELECT COUNT(*) FROM session_user " .
    "WHERE user = 'user'");
$exit++ if $rows->[0] != 1;
$dbh->disconnect;
rmtree("$HOME/.booker");

# test special session
chdir("../init");
system("./booker-init", "-d", "../create-session/test-path");
chdir($cd);
$exit++ if system("./booker-create-session", "-d", "test-path", "-u", "user");
$dbh = DBI->connect("dbi:SQLite:dbname=test-path/db.sqlite3", "", "");
$rows = $dbh->selectrow_arrayref("SELECT COUNT(*) FROM session_user " .
    "WHERE user = 'user'");
$exit++ if $rows->[0] != 1;
$dbh->disconnect;
rmtree("test-path");

# test no user
chdir("../init");
system("./booker-init");
chdir($cd);
$exit++ unless system("./booker-create-session");
rmtree("$HOME/.booker");

# test duplicate session
chdir("../init");
system("./booker-init");
chdir($cd);
$exit++ if system("./booker-create-session", "-u", "user");
$exit++ unless system("./booker-create-session", "-u", "user");
rmtree("$HOME/.booker");

# test duplicate user
chdir("../init");
system("./booker-init");
chdir($cd);
$exit++ if system("./booker-create-session", "-u", "user", "-u", "user");
$dbh = DBI->connect("dbi:SQLite:dbname=$HOME/.booker/db.sqlite3", "", "");
$rows = $dbh->selectrow_arrayref("SELECT COUNT(*) FROM session_user " .
    "WHERE user = 'user'");
$exit++ if $rows->[0] != 1;
$dbh->disconnect;
rmtree("$HOME/.booker");

# test max users
my @users;
for my $i (0..10) {
    push @users, "-u";
    push @users, "user$i";
}
chdir("../init");
system("./booker-init");
chdir($cd);
$exit++ unless system("./booker-create-session", @users);
rmtree("$HOME/.booker");

$exit++ unless system("./booker-create-session", "-d");
$exit++ unless system("./booker-create-session", "-d", "");
$exit++ if system("./booker-create-session", "-h");

exit $exit;
