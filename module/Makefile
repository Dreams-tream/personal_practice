# Author: Cuilang Yang 2023.07

CONFIG_FILE = module.config
include $(CONFIG_FILE)

TARGET = MODULE
PWD = $(shell pwd)
AUTHOR = $(shell ls /home)

CC = gcc
CFLAGS = -Wall #-Werror # -fdiagnostics-color=auto, gcc >= 4.9.0 supports this option
CFLAGS += $(shell pkg-config --cflags json-c)
CFLAGS += -DMODULE_CODE_DIR=\"$(PWD)/\"
CFLAGS += -DDEFAULT_AUTHOR_NAME=\"$(AUTHOR)\"

ifeq ($(CONFIG_AUTHOR_NAME_SUPPORT),y)
CFLAGS += -DCONFIG_AUTHOR_NAME_SUPPORT
endif

ifeq ($(CONFIG_USE_IOCTL),y)
CFLAGS += -DUSE_IOCTL
endif

LDFLAGS = $(shell pkg-config --libs json-c)

FILES = $(shell find . -type f)
SRCS = $(filter %.c, $(FILES))
# replace .c with .o
OBJS = $(SRCS:.c=.o)

CFLAGS += -I$(PWD) -I./shell -I./ioctl -I./platform_src

.PHONY: all clean

all: $(TARGET)

$(TARGET) : $(OBJS)
	@echo "CFLAGS = $(CFLAGS)"
	@echo "LDFLAGS = $(LDFLAGS)"
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LDFLAGS)

%.o : %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

clean :
	rm -f $(PWD)/$(TARGET)* $(OBJS) $(TARGET).cfg Readme
