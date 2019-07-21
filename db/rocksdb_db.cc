/*
* rocks_db.cc
* Hao Chen
* 2019-07-21
 */

#include "rocksdb_db.h"

#include <cstring>

using namespace std;

namespace ycsbc {

int RocksDB::Read(const string &table, const string &key,
         const vector<string> *fields,
         vector<KVPair> &result) {
             
    return 1;
}

int RocksDB::Update(const string &table, const string &key,
           vector<KVPair> &values) {
    return 1;
}

int RocksDB::Delete(const std::string &table, const std::string &key){
    return 1;
}

int RocksDB::Reset(){
    db->reset_my_state();
    rocksdb::SetPerfLevel(rocksdb::PerfLevel::kEnableTimeExceptForMutex);
    rocksdb::get_perf_context()->Reset();
    rocksdb::get_iostats_context()->Reset();
    total_value_size = total_key_size = 0;
    update_num_ = read_num_ = 0;
    write_wal = write_thread = write_memtable = 0;
    flush_wal_time = complete_parallel_memtable = sync_time = 0;
}

} // namespace ycsbc
