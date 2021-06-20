#!/bin/bash


if test $# -gt 1
then
	echo "Error: uso $0"
	exit 1
fi
if test $# -eq 1
then
	if test -d $1
	then
		WDIR=$1
	fi
	else
		echo "Error: uso $0"
		exit 2
	fi
	
fi
else
	WDIR="./"
fi

find $WDIR -name "[ab]*" | grep -v '-'
