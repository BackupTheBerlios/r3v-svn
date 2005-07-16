#!/bin/sh

automakeVersion=`automake --version`
result=`expr match "$automakeVersion" ".* 1\.4"`
if [ $result != 0 ]; then
	echo "r3v does not work with automake 1.4"
	echo "Either install a newer automake version"
	echo "or if you already have one modify this file"
	echo "and change"
	echo "automake --add-missing"
	echo "to"
	echo "automake-YOURVERSION --add-missing"
	exit
fi
	

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
