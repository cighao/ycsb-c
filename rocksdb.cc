/*
* rocksdb.cc
* Hao Chen
* 2019-07-21
 */

#include <cstring>
#include <iostream>
#include <vector>
#include <future>
#include "core/utils.h"
#include "core/timer.h"
#include "core/rocksdb_client.h"
#include "core/core_workload.h"
#include "db/db_factory.h"
#include "db/rocksdb_db.h"

using namespace std;

void UsageMessage(const char *command);
bool StrStartWith(const char *str, const char *pre);
string ParseCommandLine(int argc, const char *argv[], utils::Properties &props);
void check_args(utils::Properties &props);

void DelegateClient(ycsbc::RocksDB &db, ycsbc::CoreWorkload &wl, const int num_ops, bool is_loading) {
    ycsbc::RocksDBClient client(db, wl);
    printf("hello\n");
    for (int i = 0; i < num_ops; i++) {
        if (is_loading) {
            client.DoInsert();
        } else {
            client.DoTransaction();
        }
    }
}

int main(const int argc, const char *argv[]) {
    utils::Properties props;
    string file_name = ParseCommandLine(argc, argv, props);

    ycsbc::CoreWorkload wl;
    wl.Init(props);

    ycsbc::RocksDB rocksdb(props.GetProperty("data_dir"), 
                           props.GetProperty("log_dir"), 
                           stoi(props.GetProperty("logs_num")));

    printf("We store data on %s, log on %s\n", 
                props.GetProperty("data_dir").c_str(), 
                props.GetProperty("log_dir").c_str());
    printf("We have %s threads, %s log files\n", 
                props.GetProperty("threadcount").c_str(), 
                props.GetProperty("logs_num").c_str());

    const int num_threads = stoi(props.GetProperty("threadcount", "1"));
    printf("hello1\n");
    int total_ops = stoi(props[ycsbc::CoreWorkload::RECORD_COUNT_PROPERTY]);
        printf("hello2\n");
    int ops_per_thread = total_ops / num_threads;
        printf("hello3\n");


    printf("%d %d %d\n", num_threads, total_ops, ops_per_thread);

    std::vector<std::thread> threads;
    for(int i=0; i<num_threads; i++){
        threads.push_back(std::thread(DelegateClient, &rocksdb, &wl, ops_per_thread, true));
    }
    for(int i=0; i<num_threads; i++){
        threads[i].join();
    }
    rocksdb.Reset();
    for(int i=0; i<num_threads; i++){
        threads.push_back(std::thread(DelegateClient, &rocksdb, &wl, ops_per_thread, false));
    }
    for(int i=0; i<num_threads; i++){
        threads[i].join();
    }
    return 0;
}


string ParseCommandLine(int argc, const char *argv[], utils::Properties &props) {
    int argindex = 1;
    string filename;
    while (argindex < argc && StrStartWith(argv[argindex], "-")) {
        if (strcmp(argv[argindex], "-threads") == 0) {
            argindex++;
            if (argindex >= argc) {
                UsageMessage(argv[0]);
                exit(0);
            }
            props.SetProperty("threadcount", argv[argindex]);
            argindex++;
        }else if (strcmp(argv[argindex], "-logs") == 0) {
            argindex++;
            if (argindex >= argc) {
              UsageMessage(argv[0]);
              exit(0);
            }
            props.SetProperty("logs_num", argv[argindex]);
            argindex++;
        }else if (strcmp(argv[argindex], "-datadir") == 0) {
        argindex++;
        if (argindex >= argc) {
            UsageMessage(argv[0]);
            exit(0);
        }
        props.SetProperty("data_dir", argv[argindex]);
        argindex++;
        }else if (strcmp(argv[argindex], "-logdir") == 0) {
        argindex++;
        if (argindex >= argc) {
            UsageMessage(argv[0]);
            exit(0);
        }
        props.SetProperty("log_dir", argv[argindex]);
        argindex++;
        } else if (strcmp(argv[argindex], "-slaves") == 0) {
        argindex++;
        if (argindex >= argc) {
            UsageMessage(argv[0]);
            exit(0);
        }
        props.SetProperty("slaves", argv[argindex]);
        argindex++;
        } else if (strcmp(argv[argindex], "-P") == 0) {
        argindex++;
        if (argindex >= argc) {
            UsageMessage(argv[0]);
            exit(0);
        }
        filename.assign(argv[argindex]);
        ifstream input(argv[argindex]);
        try {
            props.Load(input);
        } catch (const string &message) {
            printf("%s\n", message.c_str());
            exit(0);
        }
        input.close();
        argindex++;
        } else {
        printf("Unknown option  '%s'\n", argv[argindex]);
        exit(0);
        }
    }
    if (argindex == 1 || argindex != argc) {
        UsageMessage(argv[0]);
        exit(0);
    }
    check_args(props);
    return filename;
}

void UsageMessage(const char *command) {
    printf("Usage: %s [options]\n", command);
    printf("Options:\n");
    printf("   -threads n: execute using n threads (default: 1)\n");
    printf("   -P propertyfile: load properties from the given file. Multiple files can ");
    printf("be specified, and will be processed in the order specified\n");
}

inline bool StrStartWith(const char *str, const char *pre) {
  return strncmp(str, pre, strlen(pre)) == 0;
}

void check_args(utils::Properties &props){
    // TODO
}