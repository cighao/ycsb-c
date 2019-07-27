/*
* rocksdb_recover.cc
* Hao Chen
* 2019-07-21
 */
#ifndef ROCKSDB_LITE

#include "rocksdb_db.h"

int main(const int argc, const char *argv[])  {

  if(argc != 4){
      fprintf(stderr, "usage: %s <data dir> <wal dir> <log number>\n", argv[0]);
      exit(0);
  }

  std::string data_dir = argv[1];
  std::string wal_dir = argv[2];
  int log_num = atoi(argv[3]);

  // open DB
  Options options;
  options.logs_num = log_num;
  options.wal_dir = wal_dir;
  options.create_if_missing = true;
  options.statistics = rocksdb::CreateDBStatistics();
  options.allow_concurrent_memtable_write = true;
  options.enable_pipelined_write = false;
  options.two_write_queues = false;

  options.max_total_wal_size = (1ul << 30)*3;
  options.write_buffer_size =  (1ul << 30)*3;

  std::vector<ColumnFamilyDescriptor> column_descriptor;
  column_descriptor.push_back(ColumnFamilyDescriptor(kDefaultColumnFamilyName, ColumnFamilyOptions()));
  std::vector<ColumnFamilyHandle*> column_handles;

  TransactionDB* txn_db;
  Status s = TransactionDB::Open(options, TransactionDBOptions(), kDBPath, column_descriptor, &column_handles, &txn_db);
  MY_ASSERT(s);

  printf("Recover complete\n");

  // Cleanup
  for(ColumnFamilyHandle *cf: column_handles){
    if(cf != nullptr)
      delete cf;
  }
  delete txn_db;
  return 0;
}

#endif  // ROCKSDB_LITE