/*
* rocksdb.cc
* Hao Chen
* 2019-07-21
 */

#include "db/rocksdb_db.h"

int main(const int argc, const char *argv[]) {
    ycsbc::RocksDB rocksdb("tmp", "tmp", 1);
    printf("hello world\n");
    return 0;
}