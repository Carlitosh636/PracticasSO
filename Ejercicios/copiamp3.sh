#!/bin/bash

if test $# -gt 2
then
	echo "Error: Uso $0"
	exit 2
elif test $# -le 0
then
	echo "Error: Uso $0"
	exit 2
fi

if test $# -eq 1
then
	if test -d $1
	then
		DIRDESTINO=$1
		DIRFINAL="./"
	else
		echo "Error: Uso $0"
		exit 2
	fi 
	
elif test $# -eq 2
then
	DIRDESTINO=$1
	DIRFINAL=$2
fi

for FILE in `find $DIRDESTINO -name "*.a"`
do
	cp $FILE $DIRFINAL
done
