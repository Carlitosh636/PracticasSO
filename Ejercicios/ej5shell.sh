#!/bin/bash


if test $# -gt 1
then
	echo "Error: uso $0"
	exit 1
	
elif test $# -eq 1
then
	if test -d $1
	then
		WDIR=$1
	
	else
		echo "Error: uso $0"
		exit 2
	fi
	
else
	WDIR="./"
fi
cd $WDIR

FILES=`ls -lS | head -3`
echo $FILES

