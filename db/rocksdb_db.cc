/*
* rocksdb_db.cc
* Hao Chen
* 2019-07-21
 */

#include "rocksdb_db.h"

#include <cstring>

using namespace std;

namespace ycsbc {

bool RocksDB::Read(const std::string &key){
    __sync_fetch_and_add(&read_num_, 1);
    std::string value;
    ERR(db->Get(read_options, key, &value));
    return true;
}

bool RocksDB::Insert(const std::string &key, std::string &value){
    __sync_fetch_and_add(&update_num_, 1);
    __sync_fetch_and_add(&total_key_size, key.size());
    __sync_fetch_and_add(&total_value_size, value.size());
    ERR(db->Put(write_options, key, value));
    return true;
}

bool RocksDB::Delete(const std::string &key){
    __sync_fetch_and_add(&update_num_, 1);
    db->Delete(write_options, key);
    return true;
}

bool RocksDB::Update(const std::string &key, std::string &value){
    Insert(key, value);
    return true;
}

void RocksDB::Reset(){
    db->reset_my_state();
    total_value_size = total_key_size = 0;
    update_num_ = read_num_ = 0;
    write_wal = write_thread_wait = write_memtable = 0;
    flush_wal_time = complete_parallel_memtable = sync_time = 0;
}

void RocksDB::AddState(){
    __sync_fetch_and_add(&write_wal, rocksdb::get_perf_context()->write_wal_time);
    __sync_fetch_and_add(&write_memtable, rocksdb::get_perf_context()->write_memtable_time);
    __sync_fetch_and_add(&sync_time, rocksdb::get_iostats_context()->fsync_nanos);
    __sync_fetch_and_add(&flush_wal_time, rocksdb::get_perf_context()->flush_wal_time);
    __sync_fetch_and_add(&complete_parallel_memtable, rocksdb::get_perf_context()->complete_parallel_memtable);
    __sync_fetch_and_add(&write_thread_wait, rocksdb::get_perf_context()->write_thread_wait_nanos);
}

void RocksDB::PrintState(){
    printf("total request: %lu\n", read_num_ + update_num_);
    printf("update request: %lu\n", update_num_);
    printf("read request: %lu\n", read_num_);
    printf("total insert key size: %lf GB\n", total_key_size /(1024 * 1024 * 1024.0));
    printf("total insert value size: %lf GB\n", total_value_size /(1024 * 1024 * 1024.0));
    printf("total wait time: %lf\n", write_thread_wait / 1000000.0);
    printf("write wal time: %lf\n", write_wal / 1000000.0);
    printf("flush wal time: %lf\n", flush_wal_time / 1000000.0);
    printf("sync time: %lf\n", sync_time / 1000000.0);
    printf("write memtable time: %lf\n", write_memtable / 1000000.0);
    printf("complete parallel time: %lf\n", complete_parallel_memtable / 1000000.0);
}

} // namespace ycsbc
