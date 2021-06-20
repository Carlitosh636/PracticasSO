#!/bin/bash


if test $# -gt 1
then
	echo "Error: uso $0"
	exit 1
fi
FILE=$1
for DIR in `echo $PATH | tr ":" "\n"` #como hacer un ls de directorios
do
	STRING=`find DIR -name FILE`
	if test -z STRING #testear que no es vacia
	then
		echo "ENCONTRADO EN EL DIR $DIR"
	fi
done


