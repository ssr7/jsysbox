CC=clang++-11
# CC=cc
CFLAGS=-Wall -fPIC -I$(JAVA_HOME)/include -I$(JAVA_HOME)/include/linux -lstdc++
BUILD_DIR=build

.PHONY: all clean 

all: jsystem jnetwork jdatetime

init:
	mkdir -p $(BUILD_DIR)

jsystem: init
	$(CC) $(CFLAGS) -shared -o $(BUILD_DIR)/jsystem.so jsystem.cpp
	strip --strip-all $(BUILD_DIR)/jsystem.so

jnetwork: init
	$(CC) $(CFLAGS) -shared -o $(BUILD_DIR)/jnetwork.so jnetwork.cpp
	strip --strip-all $(BUILD_DIR)/jsystem.so

jdatetime: init
	$(CC) $(CFLAGS) -shared -o $(BUILD_DIR)/jdatetime.so jdatetime.cpp
	strip --strip-all $(BUILD_DIR)/jdatetime.so

clean: 
	rm -rfv *.so $(BUILD_DIR)
