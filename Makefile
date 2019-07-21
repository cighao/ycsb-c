CC=g++
# need tbb lib
 = -I${TBB_INCLUDE} -Wl,-rpath,${TBB_LIBRARY_RELEASE} -L${TBB_LIBRARY_RELEASE} -ltbb
CFLAGS=-std=c++11 -g -Wall -pthread -I./ ${TBB_LIB}
# LDFLAGS= -lpthread -ltbb -lhiredis
LDFLAGS= -lpthread -ltbb   # don't need redis
# SUBDIRS=core db redis
SUBDIRS=core db
SUBSRCS=$(wildcard core/*.cc) $(wildcard db/*.cc)
SUBSRCS := $(filter-out db/redis_db.cc, $(SUBSRCS))   # don't compile redis

OBJECTS=$(SUBSRCS:.cc=.o)
EXEC=ycsbc

all: $(SUBDIRS) $(EXEC)

$(SUBDIRS):
	$(MAKE) -C $@

$(EXEC): $(wildcard *.cc) $(OBJECTS)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

clean:
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir $@; \
	done
	$(RM) $(EXEC)

.PHONY: $(SUBDIRS) $(EXEC)

