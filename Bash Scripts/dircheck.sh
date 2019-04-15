#! /bin/bash

shopt -s globstar

dir="0"
fil="0"
full=""


for d in $*
do
	cd $d
	for i in **
	do
		full=$d"/"$i
		if [[ -d $full ]]
		then
			((dir=dir+1))
		elif [[ -f $full ]]
		then
			if [[ -s $full ]]
			then
				((fil=fil+1))
			else
				echo file $full is of zero length
			fi
		fi
	done
	
	echo Number of Sub-Directories: $dir
	echo Number of Files: $fil
	echo Total disk usage of $d: `du -sh | tr -d "."`
	echo

	dir="0"
	fil="0"
done	
	
