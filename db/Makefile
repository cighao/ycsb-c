CC=g++
# CFLAGS=-std=c++11 -c -g -Wall -fgnu-tm
CFLAGS=-std=c++11 -c -g -Wall -I${TBB_INCLUDE} -Wl,-rpath,${TBB_LIBRARY_RELEASE} -L${TBB_LIBRARY_RELEASE} -ltbb

INCLUDES=-I../
SOURCES=$(wildcard *.cc)
SOURCES := $(filter-out redis_db.cc, $(SOURCES))
OBJECTS=$(SOURCES:.cc=.o)

all: $(SOURCES) $(OBJECTS)

.cc.o:
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@

clean:
	$(RM) $(OBJECTS)

