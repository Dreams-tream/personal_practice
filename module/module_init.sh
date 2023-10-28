#!/bin/bash
module_path=/home/yangcuilang/C_Code/module
module_name=MODULE

function start()
{
	${module_path}/${module_name} $*
}

stop()
{
	ps -e |grep ${module_name} && killall ${module_name}
}

function print
{
	#print all argument
	echo "all argument is $*, the number of argument is $#"
}

compile()
{
	make
	chmod -x *.c *.h
	chmod +x *.sh
}

compile
stop
print $*
start $*
