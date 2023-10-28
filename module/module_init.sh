#!/bin/bash
module_path=/home/yangcuilang/C_Code/module
module_name=MODULE
readme=${module_path}/Readme

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

create_readme()
{
	#here document
	cat > ${readme}  <<- END
	README
	module name:    ${module_name}
	END
	chmod 777 ${readme}
}

create_readme
compile
stop
print $*
start $*
