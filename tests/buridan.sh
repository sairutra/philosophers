#!/bin/bash

#REFERENCE: https://stackoverflow.com/questions/26465496/rounding-up-float-point-numbers-bash
#REFERENCE: https://stackoverflow.com/questions/12722095/how-do-i-use-floating-point-arithmetic-in-bash
#REFERENCE: https://stackoverflow.com/questions/16436000/bash-run-command-for-certain-time
#REFERENCE: https://stackoverflow.com/questions/10823635/how-to-include-file-in-a-bash-shell-script

. config.sh

arg="$@"
num=$2
die=$3
eat=$4
sleep=$5
even=$(echo "$(($num % 2))")
lunches=$6

if [ ! -f $1 ]; then
    echo "executable not found!"
	exit 1
fi


echo $arg
echo $num
echo $die
echo $eat
echo $sleep

round() {
  printf "%.${2}f" "${1}"
}

if [[ $even == "0" ]]; then
echo even
multi=$(bc -l <<< "($num - 2) / $num")
else
multi=$(bc -l <<< "($num - 1) / $num")
fi

sum_multi=$(bc -l <<< "$multi * $eat")
sum_total=$(bc -l <<< "$sum_multi + $eat + $sleep")
sum_total_rounded=$(printf "%.0f\n" "$sum_total" 2> /dev/null)

if [[ $die > $sum_total_rounded ]]; then
death=false
else
death=true
fi

if [[ $num == 1 ]]; then
death=true
fi


timeout 10 $arg > $outfile
exit_code=$(echo $?)
timeout 10 $valgrind $arg > $mem_outfile
mem_exit_code=$(echo $?)

#check how to check for all meals finished

echo $exit_code
echo $mem_exit_code
echo $death
exit 0
