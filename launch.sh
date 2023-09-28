#!/bin/bash
#small script to run simuls between 0.1 and 2.9 w a step of 0.1

#checks that argc is 4 
if [[ ! $# -eq 4 ]]; then 
    echo "usage:  $0 [path of graph] [path of attack] [iteration number] [target directory]"
    exit 1
fi 

#creates a result directory if it doesn't already exist 
if [[ ! -d $4 ]]; then 
	mkdir $4
	touch "$4/index.csv"
fi 

#creates the index file if it doesn't exist 
if [[ ! -f "$4/index.csv" ]]; then 
	touch "$4/index.csv"
fi 

#launches the simulation in parallel cause the scheduler has a job to do ffs 
for i in {0..1} ;do 
    for j in {0..9}; do 
	for k in {0..9}; do 
        	if [[ ! $i -eq 0 || ! $j -eq 0 ]] ; then  
                echo  "$1" "$2" "$(pwd)/$4/res$i.$j$k" "$3" "$i.$j$k" -i "$4/index.csv" 
                ./tog "$1" "$2" "$(pwd)/$4/res$i.$j$k" "$3" "$i.$j$k" -i "$4/index.csv" 
            	sleep 1 
        	fi
	done
    done
done

wait 
echo "done" 
