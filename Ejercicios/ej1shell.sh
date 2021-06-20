#!/bin/bash

if test $# -ne 0
then
	echo "Error: Uso $0"
	exit 1
fi
#para los for, los arrays donde queramos iterar deben ir entre las comillas torcidas
for FILE in `ls /bin`
do
	if ! test -e '/usr/share/man/man1/$FILE.1.gz'
	then
		echo "$FILE tiene entrada en la parte 1 del manual"
	fi
done


