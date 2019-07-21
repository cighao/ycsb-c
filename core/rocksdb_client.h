/*
* rocksdb_client.h
* Hao Chen
* 2019-07-21
 */

#ifndef YCSB_C_ROCKSDB_CLIENT_H_
#define YCSB_C_ROCKSDB_CLIENT_H_

#include "../db/rocksdb_db.h"
#include "../db/db.h"
#include "core_workload.h"
#include "utils.h"

namespace ycsbc {

class RocksDBClient {
 public:
  RocksDBClient(RocksDB &db, CoreWorkload &wl) : db_(db), workload_(wl) { }
  
  virtual bool DoInsert();
  virtual bool DoTransaction();
  
  virtual ~RocksDBClient() { }
  
 protected:
  
  virtual int TransactionRead();
  virtual int TransactionReadModifyWrite();
  virtual int TransactionScan();
  virtual int TransactionUpdate();
  virtual int TransactionInsert();
  
  RocksDB &db_;
  CoreWorkload &workload_;
};

inline bool RocksDBClient::DoInsert() {
    std::string key = workload_.NextSequenceKey();
    std::vector<DB::KVPair> values;
    workload_.BuildValues(values);
    for (DB::KVPair &field_pair : values) {
        std::string value = field_pair.second;
        return db_.Insert(key, value);
    }
}

inline bool RocksDBClient::DoTransaction() {
    int status = -1;
    switch (workload_.NextOperation()) {
        case READ:
        status = TransactionRead();
        break;
        case UPDATE:
        status = TransactionUpdate();
        break;
        case INSERT:
        status = TransactionInsert();
        break;
        case SCAN:
        status = TransactionScan();
        break;
        case READMODIFYWRITE:
        status = TransactionReadModifyWrite();
        break;
        default:
        throw utils::Exception("Operation request is not recognized!");
    }
    assert(status >= 0);
    return (status == DB::kOK);
}

inline int RocksDBClient::TransactionRead() {
    const std::string &key = workload_.NextTransactionKey();
    return db_.Read(key);
}

inline int RocksDBClient::TransactionReadModifyWrite() {
    throw utils::Exception("TransactionReadModifyWrite() is not implemented!");
}

inline int RocksDBClient::TransactionScan() {
    throw utils::Exception("TransactionScan() is not implemented!");
}

inline int RocksDBClient::TransactionUpdate() {
    const std::string &table = workload_.NextTable();
    const std::string &key = workload_.NextTransactionKey();
    std::vector<DB::KVPair> values;
    if (workload_.write_all_fields()) {
        workload_.BuildValues(values);
    } else {
        workload_.BuildUpdate(values);
    }
    return db_.Update(table, key, values);
}

inline int RocksDBClient::TransactionInsert() {
    const std::string &table = workload_.NextTable();
    const std::string &key = workload_.NextSequenceKey();
    std::vector<DB::KVPair> values;
    workload_.BuildValues(values);
    assert(values.size() == 1);
    for (KVPair &field_pair : values) {
        std::string value = field_pair.second;
        return db_.Insert(key, value);
    }
}

} // ycsbc

#endif // YCSB_C_ROCKSDB_CLIENT_H_
