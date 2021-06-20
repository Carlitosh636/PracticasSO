#!/bin/bash

if test $# -lt 1
then
	echo "Error: Uso $0"
	exit 1
fi

for FILE in `find $1 -name '*.sh'`
do
	chmod u+x $FILE
done
