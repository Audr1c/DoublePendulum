CC = gcc
# CFLAGS = -Iinclude
# LDFLAGS = -lpthread 
CFLAGS = -Iinclude -DCL_TARGET_OPENCL_VERSION=300 -Wno-deprecated -fopenmp
OPT = -O3
DEBUG = -g -O0
LDFLAGS = -lpthread C:/Windows/System32/OpenCL.dll -fopenmp
SRC = src/*.c
TEST = test/*.c
MAIN = main/main.c
TARGET = main.exe

.PHONY: all test clean


debug:
	$(CC) $(CFLAGS) $(DEBUG) $(SRC) $(MAIN) -o $(TARGET) $(LDFLAGS)

test_debug:
	$(CC) $(CFLAGS) $(DEBUG) $(TEST) $(SRC) -o test/testline.exe $(LDFLAGS)

all:
	$(CC) $(CFLAGS) $(OPT) $(SRC) $(MAIN) -o $(TARGET) $(LDFLAGS)

test: 
	$(CC) $(CFLAGS) $(OPT) $(TEST) $(SRC) -o test/testline.exe $(LDFLAGS)
	./test/testline.exe
# 	 execute the test script "testline.c"
# 	./$(TARGET) -f 100 -h 1 -p 3000 -s 0 -o resources\Image -y 2 -w 1000 -b 12 -u 0 -i 1.57 -j 1.58 -z 1 -k 0.5 -t 0.5 -a 10 -c "viridis"

clean:
	rm -f test/testline.exe
	rm -f $(TARGET)