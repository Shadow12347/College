#! /bin/bash

search() {
	count="$#"
	dir=""
	name=""
	typ=""
	prin=""
	exec_args=""
	
	while [ $count -gt 0 ]
	do
		if [[ -d $1 ]]
		then
			dir=$1
			shift 1
		else
			case $1 in
				(-name) shift 1; name=(`echo $1 | tr -d "*"`); shift 1;;
				(-type) shift 1; typ=$1; shift 1;;
				(-print) shift 1; prin=1;;
				(-exec) shift 1; exec_args=$*; break;; 
			esac	
		fi
		((count=count-1))
	done

	shopt -s globstar

	cd $dir

	for i in **
	do
		full=$dir"/"$i
		if [[ -z $typ && -z $name ]]
		then
			echo $full
		else
			if [[ $full ==  *"$name" ]]
			then
				if [[ -z $typ ]]
				then
					echo $full
				else
					case $typ in
						(d)
							if [[ -d $full ]]
							then
								if [[ -z exec_args ]]
								then
									echo $full
								else
									exec_args=`echo $exec_args | tr -d ";"`
									exec_args="${exec_args/'{}'/${full}}"
									$exec_args
									echo $full
								fi
							fi;;
						(f)
							if [[ -f $full ]]
							then
								if [[ -z exec_args ]]
								then
									echo $full
								else
									exec_args=`echo $exec_args | tr -d ";"`
									exec_args="${exec_args/'{}'/${full}}"
									$exec_args
									echo $full
								fi
							fi;;
					esac
				fi
			fi
		fi
	done
	
}

search $*


