package BookerTest;

use strict;
use warnings;

use Cwd;
use DBI;
use Exporter;
use File::Path qw(rmtree);

our $VERSION     = 1.00;
our @ISA         = qw(Exporter);
our @EXPORT      = ();
our @EXPORT_OK   = qw(clean_dir_sub fail table_exists_sub run_tests
                      dir_test_sub file_test_sub test_sql_statement
                      booker_init);
our %EXPORT_TAGS = ( DEFAULT => [@EXPORT, @EXPORT_OK]
                   );
sub fail { !$_[0] }

sub booker_init {
    my $cd = getcwd;
    chdir("../init") || return 0;
    if (@_) {
        system("./booker-init", "-d", $_[0]) && return 0;
    } else {
        system("./booker-init") && return 0;
    }
    chdir($cd) || return 0;
    return 1;
}

sub dir_test_sub {
    my ($name, $dir) = @_;
    [$name, sub { -d $dir }]
}

sub file_test_sub {
    my ($name, $file) = @_;
    [$name, sub { -f $file }]
}

sub clean_dir_sub {
    my $tree = $_[0];
    sub { rmtree($tree) }
}

sub table_exists_sub {
    my ($test_name, $file, $name) = @_;
    [ $test_name,
      sub {
          my $dbh = DBI->connect("dbi:SQLite:dbname=$file/db.sqlite3", "", "");
          my $stmt = $dbh->prepare("SELECT COUNT(*) FROM $name");
          unless ($stmt) {
              return 0;
          }
          $stmt->execute();
          my $row = $stmt->fetchrow_arrayref();
          return $row->[0] == 0;
      }
    ]
}

sub test_sql_statement {
    my ($test_name, $file, $query, $test) = @_;
    [ $test_name,
      sub {
          my $dbh = DBI->connect("dbi:SQLite:dbname=$file/db.sqlite3", "", "");
          my $rows = $dbh->selectrow_arrayref($query);
          unless ($rows) {
              $dbh->disconnect;
              return 0;
          }
          $dbh->disconnect;
          return $test->($rows)
      }
    ]
}

sub run_tests {
    my @tests = @_;

    my $exit = 0;
    for my $test (@tests) {
        my $skip = 0;
        my @cmd = @{$test->{cmd}};
        printf("# running \"$test->{name}\"\n") if ($test->{name});
        if ($test->{pre}) {
            printf("# running precommands\n");
            for my $pre (@{$test->{pre}}) {
                if (! $pre->()) {
                    printf(STDERR "failed to run precommand\n");
                    $exit = 1;
                    $skip = 1;
                }
            }
        }
        printf("# running `@cmd'\n");
        my $result = system(@cmd);
        if ($test->{verify}) {
            if ($test->{verify}($result)) {
                printf(STDERR "failed to verify @cmd\n");
                $exit = 1;
                $skip = 1;
            }
        } elsif ($result) {
            printf(STDERR "failed to run @cmd\n");
            $exit = 1;
            $skip = 1;
        }
        unless ($skip) {
            printf("# testing output\n");
            for my $check (@{$test->{tests}}) {
                unless ($check->[1]()) {
                    printf(STDERR "failed to $check->[0]\n");
                    $exit = 1;
                }
            }
        }
        printf("# cleaning up\n");
        $test->{clean}() if $test->{clean};
        printf("\n");
    }

    exit $exit;
}

1;
