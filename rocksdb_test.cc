/*
* rocksdb_test.cc
* Hao Chen
* 2019-07-21
 */
#ifndef ROCKSDB_LITE

#include "db/rocksdb_db.h"
#include "thread"

using namespace rocksdb;

int main(const int argc, const char *argv[])  {

  if(argc != 2){
      fprintf(stderr, "usage: %s <request number>\n", argv[0]);
      exit(0);
  }

  std::string data_dir = "/home/chenhao/optane/rocksdb/data/";
  std::string wal_dir =  "/home/chenhao/optane/rocksdb/log/";
  int log_num = 1;
  int req_num = atoi(argv[1]);

  system(("rm " + data_dir + "*").c_str());
  system(("rm " + wal_dir + "*").c_str());

  // open DB
  rocksdb::Options options;
  options.logs_num = log_num;
  options.wal_dir = wal_dir;
  options.create_if_missing = true;
  options.statistics = rocksdb::CreateDBStatistics();
  options.allow_concurrent_memtable_write = true;
  options.enable_pipelined_write = false;
  options.two_write_queues = false;

  options.max_total_wal_size = (1ul << 30)*3;
  options.write_buffer_size =  (1ul << 30)*3;
  
  WriteOptions write_options;
  write_options.sync = true;
  
  rocksdb::TransactionDB* txn_db;
  ERR(rocksdb::TransactionDB::Open(options, rocksdb::TransactionDBOptions(), 
    data_dir, &txn_db));

  rocksdb::SetPerfLevel(rocksdb::PerfLevel::kEnableTimeExceptForMutex);
  rocksdb::get_perf_context()->Reset();
  rocksdb::get_iostats_context()->Reset();

  auto start = std::chrono::high_resolution_clock::now();
  for(int i=0;i<req_num;i++){
    int k = rand() % 100000 + 100000;
    ERR(txn_db->Put(write_options,"abc" + std::to_string(k), "defffffffff"));
    std::this_thread::sleep_for(std::chrono::microseconds(1));
  }
  auto end = std::chrono::high_resolution_clock::now();
  double time = std::chrono::duration<double>(end - start).count()*1000;
  double wait_time = get_perf_context()->write_thread_wait_nanos/1000000.0;
  double wal_time = get_perf_context()->write_wal_time/1000000.0;
  double wal_flush_time = get_perf_context()->flush_wal_time/1000000.0;
  double fsync_time = get_iostats_context()->fsync_nanos/1000000.0;
  double write_memtable = get_perf_context()->write_memtable_time/1000000.0;
  printf("-----------\n");
  printf("total average latency: %lf ms\n",time/req_num);
  printf("wait time: %.3lf\n",wait_time/req_num);
  printf("wal time: %.3lf\n",wal_time/req_num);
  printf("wal flush time:%lf ms\n", wal_flush_time/req_num);
  printf("fsync time: %lf\n", fsync_time/req_num);
  printf("write_memtable: %lf\n", write_memtable/req_num);
  printf("-----------\n");

  delete txn_db;
  return 0;
}

#endif  // ROCKSDB_LITE