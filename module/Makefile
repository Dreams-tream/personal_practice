#compile this program MODULE and iterable small program module
.PHONY: all clean
CC = gcc
CFLAGS += -Wall
CFLAGS += $(shell pkg-config --cflags json-c)
LDFLAGS += $(shell pkg-config --libs json-c)

TARGET = MODULE
PWD = $(shell pwd)
SRC_FILE = $(shell ls $(PWD))
#SRC_CODE = $(wildcard $(PWD)/*.c)
SRC_CODE = $(filter %.c %.h,$(SRC_FILE))
COMPILE_SRC = module.c module_callback.c module_platform.c module_common.c log.c

PLATFORM_TARGET = platform
PLATFORM_OBJECT = $(PLATFORM_TARGET).o
PLATFORM_DIR = $(PWD)/platform_src
PLATFORM_SRC = $(PLATFORM_DIR)/platform.c
PLATFORM_CFLAGS = -Wall -I$(PLATFORM_DIR)

all: $(TARGET)

#$(PLATFORM_TARGET) : $(PLATFORM_DIR)/*
#	$(CC) -c $(PLATFORM_SRC) $(PLATFORM_CFLAGS)


$(TARGET) : $(SRC_CODE) $(PLATFORM_DIR)/*
	@clear
	@echo "SRC_FILE = $(SRC_FILE)"
	@echo "SRC_CODE = $(SRC_CODE)"
	@echo "CFLAGS = $(CFLAGS)"
	@echo "LDFLAGS = $(LDFLAGS)"
	$(CC) -c $(PLATFORM_SRC) $(PLATFORM_CFLAGS)
	$(CC) $(CFLAGS) $(COMPILE_SRC) $(PLATFORM_OBJECT) -o $@ $(LDFLAGS)

clean :
	rm -f $(PWD)/$(TARGET)* *.o




