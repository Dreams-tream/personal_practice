#!/bin/bash
module_path=/home/yangcuilang/C_Code/module
module_name=MODULE

function start()
{
	${module_path}/${module_name} $*
}

stop()
{
	killall ${module_name}
}

function print
{
	#print all argument
	echo "all argument is $*, the number of argument is $#"
}

stop
print $*
start $*
