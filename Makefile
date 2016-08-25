CC=g++
CFLAGS=-Wall -O3 -fPIC
RUNPATH=-Wl,-rpath=/usr/local/lib
LIBS=-L/usr/local/lib -lgsl -L/usr/local/lib -lgslcblas -L/usr/local/lib -lnlopt
INCLUDES=-I/usr/local/include -I/opt/jdk1.7.0_71/include -I/opt/jdk1.7.0_71/include/linux
SRCS=$(wildcard *.cpp)
OBJS=$(patsubst %.cpp,%.o,$(SRCS))
TARGET_LIB=libstockadjust.so
test: $(OBJS)
	$(CC)  $(CFLAGS) $^ $(RUNPATH) $(LIBS)  -shared -o $(TARGET_LIB)
%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<
.PHONY : 
	clean
clean :
	rm $(TARGET_LIB) $(OBJS)
