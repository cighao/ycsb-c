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
    printf("read \n");
    return true;
}

bool RocksDB::Insert(const std::string &key, std::string &value){
    printf("insert \n");
    return true;
}

bool RocksDB::Delete(const std::string &key){
    printf("delete \n");
    return true;
}

bool RocksDB::Update(const std::string &key, std::string &value){
    printf("delete \n");
    return true;
}

void RocksDB::Reset(){
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
