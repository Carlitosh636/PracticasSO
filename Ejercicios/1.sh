#!/bin/bash

if test $# -lt 2
then
	echo "Error: Uso $0"
	exit 2
fi


for VAR in "$@"
do
	if ! test -d $VAR	
	then
		echo "Error: Uso $0"
		exit 2
	fi
	
done

FIRST=2
LAST=$#

for FILE in `ls $1`
do
	for DIR in "${@:2}"
	do
		cp $FILE $DIR
	done 
done

