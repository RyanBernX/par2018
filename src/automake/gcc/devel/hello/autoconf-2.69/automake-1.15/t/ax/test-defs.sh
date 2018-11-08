# -*- shell-script -*-
# Generated from t/ax/test-defs.in; do not edit by hand.
#
# Copyright (C) 1996-2014 Free Software Foundation, Inc.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

# Defines and minimal setup for Automake testing environment.

# IMPORTANT NOTES AND REQUIREMENTS
#   - Multiple inclusions of this file should be idempotent.
#   - This code has to be 'set -e' clean.

# See whether the current test script is expected to use TAP or not.
# Use a sensible default, while allowing the scripts to override this
# check.
if test -z "$am_test_protocol"; then
  case $argv0 in
    *.tap) am_test_protocol=tap;;
        *) am_test_protocol=none;;
  esac
fi

am_rel_srcdir='.'
am_top_srcdir='/Users/alogon/test_parrell/gcc/devel/hello/autoconf-2.69/automake-1.15'
am_top_builddir='/Users/alogon/test_parrell/gcc/devel/hello/autoconf-2.69/automake-1.15'

# Where testsuite-related helper scripts, data files and shell libraries
# are placed.
am_testaux_srcdir=$am_top_srcdir/t/ax
am_testaux_builddir=$am_top_builddir/t/ax

# Support for the "installcheck" target.
case ${am_running_installcheck:=no} in
  yes)
    am_amdir='/usr/local/autotools-bin/share/automake-1.15/am'
    am_automake_acdir='/usr/local/autotools-bin/share/aclocal-1.15'
    am_bindir='/usr/local/autotools-bin/bin'
    am_datadir='/usr/local/autotools-bin/share'
    am_docdir='/usr/local/autotools-bin/share/doc/automake'
    am_pkgvdatadir='/usr/local/autotools-bin/share/automake-1.15'
    am_scriptdir='/usr/local/autotools-bin/share/automake-1.15'
    am_system_acdir='/usr/local/autotools-bin/share/aclocal'
    ;;
  no)
    am_amdir=$am_top_srcdir/lib/am
    am_automake_acdir=$am_top_srcdir/m4
    am_bindir=$am_top_builddir/t/wrap
    am_datadir=$am_top_srcdir
    am_docdir=$am_top_srcdir/doc
    am_pkgvdatadir=$am_top_srcdir/lib
    am_scriptdir=$am_top_srcdir/lib
    am_system_acdir=$am_top_srcdir/m4/acdir
    ;;
  *)
    echo "$me: variable 'am_running_installcheck' has invalid"
         "value '$am_running_installcheck'" >&2
    exit 99
    ;;
esac

APIVERSION='1.15'
PATH_SEPARATOR=':'

host_alias=${host_alias-''}; export host_alias
build_alias=${build_alias-''}; export build_alias

# Whether the testsuite is being run by faking the presence of a C
# compiler that doesn't grasp the '-c' and '-o' flags together.  By
# default, of course, it isn't.
: "${AM_TESTSUITE_SIMULATING_NO_CC_C_O:=no}"

# A concurrency-safe "mkdir -p" implementation.
MKDIR_P=${AM_TESTSUITE_MKDIR_P-'lib/install-sh -c -d'}

# The shell we use to run our own test scripts, determined at configure
# time.  It is required in the self tests, and most importantly for the
# automatic re-execution of test scripts.
AM_TEST_RUNNER_SHELL=${AM_TEST_RUNNER_SHELL-'/bin/sh'}

# Make sure we override the user shell.  And do not read the value of
# $SHELL from the environment (think to the non-uncommon situation where
# e.g., $SHELL=/bin/tcsh).
SHELL=${AM_TESTSUITE_SHELL-'/bin/sh'}; export SHELL

# User can override various tools used.  Prefer overriding specific for
# that automake testsuite, if they are available.
AWK=${AM_TESTSUITE_AWK-${AWK-'awk'}}
PERL=${AM_TESTSUITE_PERL-${PERL-'/usr/local/bin/perl'}}
MAKE=${AM_TESTSUITE_MAKE-${MAKE-'make'}}
YACC=${AM_TESTSUITE_YACC-${YACC-'yacc'}}
LEX=${AM_TESTSUITE_LEX-${LEX-'lex'}}
AUTOCONF=${AM_TESTSUITE_AUTOCONF-${AUTOCONF-'autoconf'}}
AUTOM4TE=${AM_TESTSUITE_AUTOM4TE-${AUTOM4TE-'autom4te'}}
AUTORECONF=${AM_TESTSUITE_AUTORECONF-${AUTORECONF-'autoreconf'}}
AUTOHEADER=${AM_TESTSUITE_AUTOHEADER-${AUTOHEADER-'autoheader'}}
AUTOUPDATE=${AM_TESTSUITE_AUTOUPDATE-${AUTOUPDATE-'autoupdate'}}

# Tests which want complete control over aclocal or automake command-line
# options should use $am_original_ACLOCAL or $am_original_AUTOMAKE.  The
# "test -z" tests take care not to re-initialize them if 'test-defs.sh'
# is re-sourced, as we want that file to remain really idempotent.
if test -z "$am_original_AUTOMAKE"; then
  am_original_AUTOMAKE=${AM_TESTSUITE_AUTOMAKE-${AUTOMAKE-"automake-$APIVERSION"}}
fi
if test -z "$am_original_ACLOCAL"; then
  am_original_ACLOCAL=${AM_TESTSUITE_ACLOCAL-${ACLOCAL-"aclocal-$APIVERSION"}}
fi

# Use -Werror by default.  Tests for which this is inappropriate should
# use -Wno-error.
# Tests which want complete control over aclocal command-line options
# should use $am_original_ACLOCAL instead.
ACLOCAL="$am_original_ACLOCAL -Werror"

# See how Automake should be run.  We put --foreign as the default
# strictness to avoid having to create lots and lots of files.  A test
# can override this by specifying a different strictness.  Use -Wall
# -Werror by default.  Tests for which this is inappropriate (e.g. when
# testing that a warning is enabled by a specific switch) should use
# -Wnone or/and -Wno-error.
# Tests who want complete control over automake command-line options
# should use $am_original_AUTOMAKE instead.
AUTOMAKE="$am_original_AUTOMAKE --foreign -Werror -Wall"

# POSIX no longer requires 'egrep' and 'fgrep',
# but some hosts lack 'grep -E' and 'grep -F'.
EGREP=${AM_TESTSUITE_EGREP-'/usr/bin/grep -E'}
FGREP=${AM_TESTSUITE_FGREP-'/usr/bin/grep -F'}

# Compilers and their flags.  These can point to non-GNU compilers (and
# on non-Linux and non-BSD systems, they probably will).
if test $AM_TESTSUITE_SIMULATING_NO_CC_C_O = no; then
  CC=${AM_TESTSUITE_CC-${CC-'cc'}}
else
  CC=$am_testaux_builddir/cc-no-c-o
fi
CXX=${AM_TESTSUITE_CXX-${CXX-'false'}}
F77=${AM_TESTSUITE_F77-${F77-'gfortran'}}
FC=${AM_TESTSUITE_FC-${FC-'gfortran'}}
CFLAGS=${AM_TESTSUITE_CFLAGS-${CFLAGS-'-g -O2'}}
CXXFLAGS=${AM_TESTSUITE_CXXFLAGS-${CXXFLAGS-''}}
FCFLAGS=${AM_TESTSUITE_FCFLAGS-${FCFLAGS-'-g -O2'}}
FFLAGS=${AM_TESTSUITE_FFLAGS-${FFLAGS-'-g -O2'}}
CPPFLAGS=${AM_TESTSUITE_CPPFLAGS-${CPPFLAGS-''}}

# GNU compilers and their flags.
if test $AM_TESTSUITE_SIMULATING_NO_CC_C_O = no; then
  GNU_CC=${AM_TESTSUITE_GNU_CC-${GNU_CC-'cc'}}
else
  GNU_CC=$am_testaux_builddir/cc-no-c-o
fi
GNU_CC=${AM_TESTSUITE_GNU_CC-${GNU_CC-'cc'}}
GNU_CXX=${AM_TESTSUITE_GNU_CXX-${GNU_CXX-'g++'}}
GNU_F77=${AM_TESTSUITE_GNU_F77-${GNU_F77-'gfortran'}}
GNU_FC=${AM_TESTSUITE_GNU_FC-${GNU_FC-'gfortran'}}
GNU_FFLAGS=${AM_TESTSUITE_GNU_FFLAGS-${GNU_FFLAGS-'-g -O2'}}
GNU_FCFLAGS=${AM_TESTSUITE_GNU_FCFLAGS-${GNU_FCFLAGS-'-g -O2'}}
GNU_CXXFLAGS=${AM_TESTSUITE_GNU_CXXFLAGS-${GNU_CXXFLAGS-''}}
GNU_CFLAGS=${AM_TESTSUITE_GNU_CFLAGS-${GNU_CFLAGS-'-g -O2'}}
GNU_GCJ=${AM_TESTSUITE_GNU_GCJ-${GNU_GCJ-'false'}}
GNU_GCJFLAGS=${AM_TESTSUITE_GNU_GCJFLAGS-${GNU_GCJFLAGS-''}}

# No all versions of Tex support '--version', so we use a configure
# check to decide if tex is available.  This decision is embodied in
# this variable.
TEX=${AM_TESTSUITE_TEX-'tex'}

# The amount we should wait after modifying files depends on the platform.
# For instance, Windows '95, '98 and ME have 2-second granularity
# and can be up to 3 seconds in the future w.r.t. the system clock.
# The creative quoting is to avoid spuriously triggering a failure in
# the maintainer checks,
sleep='sleep ''2'

# An old timestamp that can be given to a file, in "touch -t" format.
# The time stamp should be portable to all file systems of interest.
# Just for fun, choose the exact time of the announcement of the GNU project
# in UTC; see <http://www.gnu.org/gnu/initial-announcement.html>.
old_timestamp=198309271735.59

# Make our wrapper script (or installed scripts, if running under
# "installcheck") accessible by default.  And avoid to uselessly
# extend $PATH multiple times if this file is sourced multiple times.
case $PATH in
  $am_bindir$PATH_SEPARATOR*) ;;
  *) PATH=$am_bindir$PATH_SEPARATOR$PATH;;
esac

export PATH
