#!/bin/bash
#small script to run simuls between 0.1 and 2.9 w a step of 0.1

#checks that argc is 3 
if [[ ! $# -eq 3 ]]; then 
    echo "usage:  $0 [path of graph] [path of attack] [iteration number]"
    exit 1
fi 

#launches in parallel cause the scheduler has a job to do ffs 
for i in {0..2} ;do 
    for j in {0..9}; do  
        if [[ ! $i -eq 0 || ! $j -eq 0 ]] ; then        
            ./tog "$1" "$2" "res$i.$j" "$3" "$i.$j" &   
            sleep 1 
        fi
    done
done

wait 
echo "done" 