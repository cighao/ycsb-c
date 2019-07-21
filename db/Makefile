CC=g++
# CFLAGS=-std=c++11 -c -g -Wall -fgnu-tm
# need tbb lib
TBB_LIB = -I${TBB_INCLUDE} -Wl,-rpath,${TBB_LIBRARY_RELEASE} -L${TBB_LIBRARY_RELEASE} -ltbb
CFLAGS=-std=c++11 -c -g -Wall ${TBB_LIB}

INCLUDES=-I../
SOURCES=$(wildcard *.cc)
SOURCES := $(filter-out redis_db.cc, $(SOURCES))    # don't compile redis
OBJECTS=$(SOURCES:.cc=.o)

all: $(SOURCES) $(OBJECTS)

.cc.o:
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@

clean:
	$(RM) $(OBJECTS)

