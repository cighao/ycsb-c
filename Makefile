CC=g++

# need tbb lib
TBB_LIB = -I${TBB_INCLUDE} -Wl,-rpath,${TBB_LIBRARY_RELEASE} -L${TBB_LIBRARY_RELEASE} -ltbb
ROCKSDB_INCLUDE = /home/chenhao/test-rocksdb/linkbench-cpp/build/include
ROCKSDB_LIB = /home/chenhao/test-rocksdb/linkbench-cpp/build/lib64
ROCKSDB = -I${ROCKSDB_INCLUDE} -Wl,-rpath,${ROCKSDB_LIB} -L${ROCKSDB_LIB} -lrocksdb
HIREDIS_LIB = /home/chenhao/test-rocksdb/linkbench-cpp/ycsb-c/redis
HIREDIS = -I${HIREDIS_LIB} -Wl,-rpath,${HIREDIS_LIB}/hiredis -L${HIREDIS_LIB}/hiredis -lhiredis

CFLAGS=-std=c++11 -g -Wall -pthread -I./ ${TBB_LIB} ${ROCKSDB} ${HIREDIS}
DFLAGS= -lpthread -ltbb -lhiredis
#LDFLAGS= -lpthread -ltbb   -lhiredis # don't need redis

# SUBDIRS=core db redis
SUBDIRS=core db
SUBSRCS=$(wildcard core/*.cc) $(wildcard db/*.cc)
#SUBSRCS := $(filter-out db/redis_db.cc, $(SUBSRCS))   # don't compile redis

OBJECTS=$(SUBSRCS:.cc=.o)

EXEC=ycsbc rocksdb rocksdb_recover rocksdb_test

all: $(SUBDIRS) $(EXEC)

$(SUBDIRS):
	$(MAKE) -C $@

ycsbc: ycsbc.cc $(OBJECTS)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o ycsbc

rocksdb: rocksdb.cc $(OBJECTS)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o rocksdb

rocksdb_recover: rocksdb_recover.cc $(OBJECTS)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o rocksdb_recover

rocksdb_test: rocksdb_test.cc $(OBJECTS)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o rocksdb_test

clean:
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir $@; \
	done
	$(RM) $(EXEC)

.PHONY: $(SUBDIRS) $(EXEC)

