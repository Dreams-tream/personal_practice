#compile this daemon process wifid and iterable small program wifi
CC = gcc
CFLAGS += -w
CFLAGS += $(shell pkg-config --cflags json-c)
LDFLAGS += $(shell pkg-config --libs json-c)
TARGET = YCL
PWD = $(shell pwd)
SRC_FILE = $(shell ls $(PWD))
#SRC_CODE = $(wildcard $(PWD)/*.c)
SRC_CODE = $(filter %.c %.h,$(SRC_FILE))
COMPILE_SRC =  module.c   module_callback.c module_platform.c module_common.c log.c

.PHONY: all clean

all: $(TARGET)

$(TARGET) : $(SRC_CODE)
	@clear
	@echo "SRC_FILE = $(SRC_FILE)"
	@echo "SRC_CODE = $(SRC_CODE)"
	@echo "CFLAGS = $(CFLAGS)"
	@echo "LDFLAGS = $(LDFLAGS)"
	$(CC) $(CFLAGS) $(COMPILE_SRC)  -o $@ $(LDFLAGS)

clean :
	rm -f $(PWD)/$(TARGET)* $(PWD)/.pid $(PWD)/*.pid




