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



} // namespace ycsbc
