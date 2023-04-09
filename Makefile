CC = gcc
VPATH = src/
target = software
OBJ = log.o loop.o main.o
all:$(target)
	@#date +%G'year '%m'month '%d'day '%k'hour '%M'minute '%S'second '
	@echo -n "install $(target) done on "
	@date +%G-%m-%d' at '%k:%M:%S

$(target):$(OBJ)
	$(CC) $^ -o $@

log.o:log.h

loop.o:log.h loop.h

main.o:loop.h

clean:
	rm -f $(OBJ) $(target)
