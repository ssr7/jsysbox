CC=gcc 
CFLAGS=-W -fPIC -I$(JAVA_HOME)/include -I$(JAVA_HOME)/include/linux -shared
BUILD_DIR=build

.PHONY: all clean 

all: jsystem  

init:
	mkdir -p $(BUILD_DIR)

jsystem: init
	$(CC) $(CFLAGS) -shared -o $(BUILD_DIR)/jsystem.so jsystem.c
	strip --strip-all $(BUILD_DIR)/jsystem.so


clean: 
	rm -rfv *.so $(BUILD_DIR)
