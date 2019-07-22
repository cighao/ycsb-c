/*
* rocksdb_db.h
* Hao Chen
* 2019-07-21
 */

#ifndef YCSB_C_ROCKSDB_DB_H_
#define YCSB_C_ROCKSDB_DB_H_

#include "core/db.h"

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
    RocksDB(std::string path, std::string wal_dir, int logs_num):update_num_(0),
        read_num_(0), write_wal(0), write_thread(0), write_memtable(0),
        flush_wal_time(0), complete_parallel_memtable(0), sync_time(0),
        total_key_size(0), total_value_size(0){
        
        rocksdb::Options options;
        rocksdb::BlockBasedTableOptions table_options;
        table_options.block_cache = rocksdb::NewLRUCache((1ul << 30) * 16, 10);
        options.logs_num = logs_num;
        options.wal_dir = wal_dir;
        options.allow_concurrent_memtable_write = true;
        options.enable_pipelined_write = false;
        options.max_total_wal_size = (1ul << 30)*5;
        options.table_factory.reset(rocksdb::NewBlockBasedTableFactory(table_options));
        options.table_cache_numshardbits = 10;
        options.IncreaseParallelism(128);
        options.create_if_missing = true;
        options.error_if_exists = true;
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

    bool Update(const std::string &key, std::string &value){
        return Insert(&key, &value);
    }

    bool Insert(const std::string &key, std::string &value);

    bool Delete(const std::string &key);

    void Reset();

 private:
    rocksdb::TransactionDB *db;
    rocksdb::WriteOptions write_options;
    rocksdb::ReadOptions read_options;
    rocksdb::Options options;
    
    std::string data_dir;
    std::string wal_dir;
    uint64_t update_num_, read_num_;
    uint64_t write_wal, write_thread, write_memtable;
    uint64_t flush_wal_time, complete_parallel_memtable, sync_time;
    uint64_t total_key_size, total_value_size;
};

} // ycsbc

#endif // YCSB_C_ROCKSDB_DB_H_

