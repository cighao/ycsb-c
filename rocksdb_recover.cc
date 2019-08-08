/*
* rocksdb_recover.cc
* Hao Chen
* 2019-07-21
 */
#ifndef ROCKSDB_LITE

#include "db/rocksdb_db.h"

int main(const int argc, const char *argv[])  {

  if(argc != 5){
      fprintf(stderr, "usage: %s <data dir> <wal dir> <log number> <is_concurrent_recovery>\n", argv[0]);
      exit(0);
  }

  std::string data_dir = argv[1];
  std::string wal_dir = argv[2];
  int log_num = atoi(argv[3]);
  int is_concurrent_recovery = atoi(argv[4]);

  // open DB
  rocksdb::Options options;
  options.logs_num = log_num;
  options.wal_dir = wal_dir;
  options.create_if_missing = true;
  options.statistics = rocksdb::CreateDBStatistics();
  options.allow_concurrent_memtable_write = true;
  options.enable_pipelined_write = false;
  options.two_write_queues = false;
  if(is_concurrent_recovery == 1 ){
    options.concurrent_recovery = true;
  }else{
    options.concurrent_recovery = false;
  }

  options.max_total_wal_size = (1ul << 30)*10;
  options.write_buffer_size =  (1ul << 30)*10;
  rocksdb::ColumnFamilyOptions def_cfo;
  def_cfo.write_buffer_size =  (1ul << 30)*10;  

  std::vector<rocksdb::ColumnFamilyDescriptor> column_descriptor;
  column_descriptor.push_back(rocksdb::ColumnFamilyDescriptor(rocksdb::kDefaultColumnFamilyName, def_cfo));
  std::vector<rocksdb::ColumnFamilyHandle*> column_handles;

  rocksdb::TransactionDB* txn_db;
  ERR(rocksdb::TransactionDB::Open(options, rocksdb::TransactionDBOptions(), 
    data_dir, column_descriptor, &column_handles, &txn_db));

  // printf("Recover complete\n");

  // Cleanup
  for(rocksdb::ColumnFamilyHandle *cf: column_handles){
    if(cf != nullptr)
      delete cf;
  }
  delete txn_db;
  return 0;
}

#endif  // ROCKSDB_LITE