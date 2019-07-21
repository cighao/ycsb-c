CC=g++
# CFLAGS=-std=c++11 -c -g -Wall -fgnu-tm
# add tbb lib and rocskdb lib
TBB_LIB = -I${TBB_INCLUDE} -Wl,-rpath,${TBB_LIBRARY_RELEASE} -L${TBB_LIBRARY_RELEASE} -ltbb
ROCKSDB_INCLUDE = /home/chenhao/test-rocksdb/linkbench-cpp/build/include
ROCKSDB_LIB = /home/chenhao/test-rocksdb/linkbench-cpp/build/lib64
ROCKSDB = -I${ROCKSDB_INCLUDE} -Wl,-rpath,${ROCKSDB_LIB} -L${ROCKSDB_LIB} -lrocksdb
CFLAGS=-std=c++11 -c -g -Wall ${TBB_LIB} ${ROCKSDB}

INCLUDES=-I../
SOURCES=$(wildcard *.cc)
SOURCES := $(filter-out redis_db.cc, $(SOURCES))    # don't compile redis
OBJECTS=$(SOURCES:.cc=.o)

all: $(SOURCES) $(OBJECTS)

.cc.o:
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@

clean:
	$(RM) $(OBJECTS)

