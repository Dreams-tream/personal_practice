#!/bin/bash
make clean
make
cnt=0
loop=4
while [ ${cnt} -lt ${loop} ]
do
	echo cnt=${cnt}, sleep $((loop-cnt)) second to execute program
	cnt=$((cnt+1))
	sleep 1
done
./software
