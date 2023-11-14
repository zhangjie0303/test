// Copyright (c) 2011-present, Facebook, Inc.  All rights reserved.
//  This source code is licensed under both the GPLv2 (found in the
//  COPYING file in the root directory) and Apache 2.0 License
//  (found in the LICENSE.Apache file in the root directory).

#include <cstdio>
#include <string>
#include <iostream>
#include <unistd.h>

#include "rocksdb/db.h"
#include "rocksdb/slice.h"
#include "rocksdb/options.h"

using ROCKSDB_NAMESPACE::DB;
using ROCKSDB_NAMESPACE::Options;
using ROCKSDB_NAMESPACE::PinnableSlice;
using ROCKSDB_NAMESPACE::ReadOptions;
using ROCKSDB_NAMESPACE::Status;
using ROCKSDB_NAMESPACE::WriteBatch;
using ROCKSDB_NAMESPACE::WriteOptions;

using namespace std;

const std::string kDBPath = "/tmp/generate_sst";

int main() {
	DB* db;
	Options options;
	// Optimize RocksDB. This is the easiest way to get RocksDB to perform well
	options.IncreaseParallelism();
	options.OptimizeLevelStyleCompaction();
	// create the DB if it's not already present
	options.create_if_missing = true;
    options.write_buffer_size = 67108864;
	
	// open DB
	Status s = DB::Open(options, kDBPath, &db);
	assert(s.ok());
    string identity;
    //db->db_id_ = "d6e9eeb8-d9dc-4e30-8dac-0e68f1082aaa";
    s = db->GetDbIdentity(identity);
	assert(s.ok());
    cout << "identity: " << identity << endl;
	
    for (int i = 0; i < 6000000; i++) {
        char key[32] = {0};
        char value[64] = {0};
        sprintf(key, "%09d", i);
        sprintf(value, "%09d_v", i);

        if(i % 1000 == 0) {
		    cout << key << " | " << value << endl;
        }
#if 1
		// Put key-value
		s = db->Put(WriteOptions(), key, value);
		if (!s.ok()) {
        	cout << "Error while adding Key: " << key << ", Error: " << s.ToString() << endl;
        	return 1;
        }
#endif
    }
	
    cout << "before sleep." << endl;
    ::sleep(10);
    cout << "after sleep." << endl;
	delete db;
	return 0;
}
