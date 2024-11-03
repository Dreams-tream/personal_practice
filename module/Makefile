# Author: Cuilang Yang 2023.07

CONFIG_FILE = module.config
include $(CONFIG_FILE)

CC = gcc
CFLAGS += -Wall
CFLAGS += $(shell pkg-config --cflags json-c)
LDFLAGS += $(shell pkg-config --libs json-c)

CFLAGS += -DMODULE_CODE_DIR=\"$(PWD)/\"
CFLAGS += -DDEFAULT_AUTHOR_NAME=\"$(AUTHOR)\"

ifeq ($(CONFIG_AUTHOR_NAME_SUPPORT),y)
CFLAGS += -DCONFIG_AUTHOR_NAME_SUPPORT
endif

TARGET = MODULE
PWD = $(shell pwd)
AUTHOR = $(shell ls /home)
SRC_FILE = $(shell ls $(PWD))
#SRC_CODE = $(wildcard $(PWD)/*.c)
SRC_CODE = $(filter %.c %.h,$(SRC_FILE))
COMPILE_SRC = module.c module_callback.c module_platform.c module_common.c log.c

PLATFORM_TARGET = platform
PLATFORM_OBJECT = $(PLATFORM_TARGET).o
PLATFORM_DIR = $(PWD)/platform_src
PLATFORM_SRC = $(PLATFORM_DIR)/platform.c
PLATFORM_CFLAGS = -Wall -I$(PLATFORM_DIR) -I$(PWD)



.PHONY: all clean

all: $(TARGET)

#$(PLATFORM_TARGET) : $(PLATFORM_DIR)/*
#	$(CC) -c $(PLATFORM_SRC) $(PLATFORM_CFLAGS)


$(TARGET) : $(SRC_CODE) $(PLATFORM_DIR)/* $(CONFIG_FILE)
	@clear
ifneq ($(AUTHOR_NAME),)
	echo $(CONFIG_AUTHOR_NAME) > $(TARGET).cfg
endif
	@echo "SRC_FILE = $(SRC_FILE)"
	@echo "SRC_CODE = $(SRC_CODE)"
	@echo "CFLAGS = $(CFLAGS)"
	@echo "LDFLAGS = $(LDFLAGS)"
	$(CC) -c $(PLATFORM_SRC) $(PLATFORM_CFLAGS)
	$(CC) $(CFLAGS) $(COMPILE_SRC) $(PLATFORM_OBJECT) -o $@ $(LDFLAGS)

$(CONFIG_FILE):

clean :
	rm -f $(PWD)/$(TARGET)* *.o $(TARGET).cfg Readme
