#!/bin/bash

#REFERENCE: https://stackoverflow.com/questions/26465496/rounding-up-float-point-numbers-bash
#REFERENCE: https://stackoverflow.com/questions/12722095/how-do-i-use-floating-point-arithmetic-in-bash

arg="$@"

num=$2
die=$3
eat=$4
sleep=$5
even=$(echo "$(($num % 2))")
lunches=$6

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

#check how to check for all meals finished
#check ho


echo $death
exit 0
