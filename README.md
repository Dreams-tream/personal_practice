# personal_practice
This is a simple personal practice of C language, which can check whether your virtual machine has ipv4 address. If not, the program will create one by dhcp. Otherwise, it will check next time.

Tips:
1.The program set 4 print level, imitating other projects. And the default print level is 0(ERR). You can adjust the print level by kill command. Such as:
	1)kill -USR1 pid
	2)kill -USR2 pid
And the pid is a number represents the program getting from command 'ps -e|grep software'.
2.In most cases, ipv4 address exists. So you can shutdown the interface mannually by command 'sudo ifconfig eth0 down'. Then execute the program. You will watch how the program gets ip address.
