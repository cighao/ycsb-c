/*
* rocksdb_db.h
* Hao Chen
* 2019-07-21
 */

#ifndef YCSB_C_ROCKSDB_DB_H_
#define YCSB_C_ROCKSDB_DB_H_

#include "core/db.h"
#include "unistd.h"
#include "sys/mman.h"

#include <iostream>
#include <string>
#include "core/properties.h"
#include "rocksdb/db.h"
#include "rocksdb/table.h"
#include "rocksdb/options.h"
#include "rocksdb/utilities/options_util.h"
#include "rocksdb/utilities/transaction_db.h"
#include "rocksdb/iostats_context.h"
#include "rocksdb/perf_context.h"

#define ERR(expr) do{rocksdb::Status ec = (expr); if(!ec.ok()) fprintf(stderr,  \
    "%s:%d: %s: %s\n", __FILE__, __LINE__, #expr, ec.ToString().c_str()), assert(false);}while(0)

namespace ycsbc {

class RocksDB{
 public:
    RocksDB(){

    }
    RocksDB(std::string path, std::string wal_dir, int logs_num):
        write_options(), read_options(), update_num_(0), read_num_(0), 
        write_wal(0), write_thread_wait(0), write_memtable(0),
        flush_wal_time(0), complete_parallel_memtable(0), sync_time(0),
        total_key_size(0), total_value_size(0){
        
        rocksdb::BlockBasedTableOptions table_options;
        table_options.block_cache = rocksdb::NewLRUCache((1ul << 30) * 16, 10);
        rocksdb::Options options;
        options.table_factory.reset(rocksdb::NewBlockBasedTableFactory(table_options));
        options.max_total_wal_size = (1ul << 30)*5;
        options.write_buffer_size = (1ul << 30)*5;
        options.logs_num = logs_num;
        options.wal_dir = wal_dir;
        options.allow_concurrent_memtable_write = true;
        options.enable_pipelined_write = false;
        options.create_if_missing = true;
        options.error_if_exists = true;
        options.two_write_queues = false;
        options.IncreaseParallelism(128);
        options.table_cache_numshardbits = 10;
        options.compression = rocksdb::kSnappyCompression;
        options.bottommost_compression = rocksdb::kSnappyCompression;
        write_options.sync = true;
        write_options.disableWAL = false;
        ERR(rocksdb::TransactionDB::Open(options, rocksdb::TransactionDBOptions(), path.c_str(), &db));
        Reset();
    }

    bool Read(const std::string &key);

    bool Scan(const std::string &key) {
        throw "Scan: function not implemented!";
    }

    bool Insert(const std::string &key, std::string &value);

    bool Update(const std::string &key, std::string &value);

    bool Delete(const std::string &key);

    void Reset();

    void AddState();

    void PrintState();

    ~RocksDB(){
         delete db;
    }

 private:
    rocksdb::TransactionDB *db;
    rocksdb::WriteOptions write_options;
    rocksdb::ReadOptions read_options;
    
    std::string data_dir;
    std::string wal_dir;
    uint64_t update_num_, read_num_;
    uint64_t write_wal, write_thread_wait, write_memtable;
    uint64_t flush_wal_time, complete_parallel_memtable, sync_time;
    uint64_t total_key_size, total_value_size;


    struct MySet{
      private:
        uint64_t size_;
        double *data_;
        const uint64_t capacity = 1ul << 30;
      public:
        MySet(){
            size_ = 0;
            data_ = (double *) mmap(nullptr, capacity, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS | MAP_NORESERVE, -1, 0);
            assert(data_ != nullptr);
        }
        void insert(double value){
            uint64_t index = __sync_fetch_and_add(&size_, 1);
            data_[index] = value;
        }
        double sum(){
            return std::accumulate(data_, data_ + size_, 0.0);
        }
        double avg(){
            return std::accumulate(data_, data_ + size_, 0.0)/size_;
        }
        double& operator[](uint64_t index){
            assert(index < size_);
            return data_[index];
         }
        uint64_t size(){
            return size_;
        }
        void reset(){
            memset(data_, 0, sizeof(double)*size_);
            size_ = 0;
        }
        void deallocate(){
            assert(munmap(data_, capacity) == 0);
        }
    };
    MySet update_time_;
    MySet read_time_;
};

} // ycsbc

#endif // YCSB_C_ROCKSDB_DB_H_

