#! /bin/bash

path=(`echo $PATH | tr ":" "\n" | sort | uniq`)
for f in ${path[*]}
do
	for a in $*
	do
		list=$(find $f -name $a)
		for l in ${list[*]}
		do
			echo $l
		done
	done
done
