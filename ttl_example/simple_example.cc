// Copyright (c) 2011-present, Facebook, Inc.  All rights reserved.
//  This source code is licensed under both the GPLv2 (found in the
//  COPYING file in the root directory) and Apache 2.0 License
//  (found in the LICENSE.Apache file in the root directory).

#include <cstdio>
#include <string>
#include <iostream>

#include "rocksdb/db.h"
#include "rocksdb/slice.h"
#include "rocksdb/options.h"
#include "rocksdb/utilities/options_util.h"

using ROCKSDB_NAMESPACE::DB;
using ROCKSDB_NAMESPACE::Options;
using ROCKSDB_NAMESPACE::PinnableSlice;
using ROCKSDB_NAMESPACE::ReadOptions;
using ROCKSDB_NAMESPACE::Status;
using ROCKSDB_NAMESPACE::WriteBatch;
using ROCKSDB_NAMESPACE::WriteOptions;

using ROCKSDB_NAMESPACE::DBOptions; // zhangjie
using ROCKSDB_NAMESPACE::ColumnFamilyDescriptor;
using ROCKSDB_NAMESPACE::ConfigOptions;
using ROCKSDB_NAMESPACE::ColumnFamilyHandle;
using ROCKSDB_NAMESPACE::kDefaultColumnFamilyName;
using ROCKSDB_NAMESPACE::ColumnFamilyOptions;

#if defined(OS_WIN)
std::string kDBPath = "C:\\Windows\\TEMP\\rocksdb_simple_example";
#else
std::string kDBPath = "/tmp/rocksdb_simple_example";
#endif

int main() {
  DB* db;
#if 0
  Options options;
  // Optimize RocksDB. This is the easiest way to get RocksDB to perform well
  options.IncreaseParallelism();
  options.OptimizeLevelStyleCompaction();
  // create the DB if it's not already present
  options.create_if_missing = true;

  // open DB
  Status s = DB::Open(options, kDBPath, &db);
  assert(s.ok());
#else
  Status s;
  DBOptions loaded_db_opt;
  //std::vector<ColumnFamilyDescriptor> loaded_cf_descs;
  ConfigOptions config_options;
  std::string options_file_name;
  //s = GetLatestOptionsFileName(g_config_path, config_options.env, &options_file_name);
  //s = GetLatestOptionsFileName(kDBPath, config_options.env, &options_file_name);
  //assert(s.ok());
  //std::cout << "latest options file name: " << options_file_name << std::endl;

  //s = LoadOptionsFromFile(config_options, "/root/zhangjie/open_source/rocksdb/examples/options/options.ini", &loaded_db_opt, &loaded_cf_descs);
  s = LoadDBOptionsSimplyFromFile(config_options, "/root/zhangjie/open_source/rocksdb/examples/options/options.ini", &loaded_db_opt);
  if (!s.ok()) {
    std::cout << "load, status: " << s.ToString() << std::endl;
    return -1;
  }
  assert(s.ok());
  loaded_db_opt.IncreaseParallelism();
  //loaded_db_opt.OptimizeLevelStyleCompaction();
  loaded_db_opt.create_if_missing = true;

  // open the db using the loaded options.
  std::vector<ColumnFamilyHandle*> handles;
  // Open DB only with default column family
  std::vector<ColumnFamilyDescriptor> cf_descs;
  ColumnFamilyOptions cf_options;
  cf_descs.emplace_back(kDefaultColumnFamilyName, cf_options);
  s = DB::Open(loaded_db_opt, kDBPath, cf_descs, &handles, &db);
  if (!s.ok()) {
      std::cout << "open failed, status: " << s.ToString() << std::endl;
      return -1;
  }
  assert(s.ok());
#endif

  // Put key-value
  s = db->Put(WriteOptions(), "key1", "value");
  assert(s.ok());
  std::string value;
  // get value
  s = db->Get(ReadOptions(), "key1", &value);
  assert(s.ok());
  assert(value == "value");
  std::cout << "key1: " << "key1" << ", value: " << value << std::endl;

  // atomically apply a set of updates
  {
    WriteBatch batch;
    batch.Delete("key1");
    batch.Put("key2", value);
    s = db->Write(WriteOptions(), &batch);
  }

  s = db->Get(ReadOptions(), "key1", &value);
  assert(s.IsNotFound());

  db->Get(ReadOptions(), "key2", &value);
  assert(value == "value");

  {
    PinnableSlice pinnable_val;
    db->Get(ReadOptions(), db->DefaultColumnFamily(), "key2", &pinnable_val);
    assert(pinnable_val == "value");
  }

  {
    std::string string_val;
    // If it cannot pin the value, it copies the value to its internal buffer.
    // The intenral buffer could be set during construction.
    PinnableSlice pinnable_val(&string_val);
    db->Get(ReadOptions(), db->DefaultColumnFamily(), "key2", &pinnable_val);
    assert(pinnable_val == "value");
    // If the value is not pinned, the internal buffer must have the value.
    assert(pinnable_val.IsPinned() || string_val == "value");
  }

  PinnableSlice pinnable_val;
  s = db->Get(ReadOptions(), db->DefaultColumnFamily(), "key1", &pinnable_val);
  assert(s.IsNotFound());
  // Reset PinnableSlice after each use and before each reuse
  pinnable_val.Reset();
  db->Get(ReadOptions(), db->DefaultColumnFamily(), "key2", &pinnable_val);
  assert(pinnable_val == "value");
  pinnable_val.Reset();
  // The Slice pointed by pinnable_val is not valid after this point

  delete db;

  return 0;
}
