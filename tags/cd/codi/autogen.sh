#!/bin/sh

run ()
{
    echo "running: $*"
    eval $*

    if test $? != 0 ; then
	echo "error: while running '$*'"
	exit 1
    fi
}

run aclocal
run autoheader
run libtoolize --automake
run automake --add-missing
run autoconf
