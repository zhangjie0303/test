// refer: https://wanghenshui.github.io/rocksdb-doc-cn/doc/Creating-and-Ingesting-SST-files.html

#include <cstdio>
#include <string>
#include <iostream>

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
using ROCKSDB_NAMESPACE::SstFileWriter;
using ROCKSDB_NAMESPACE::EnvOptions;
using ROCKSDB_NAMESPACE::IngestExternalFileOptions;
using ROCKSDB_NAMESPACE::ExternalSstFileInfo;

using namespace std;

const std::string kDBPath = "/tmp/rocksdb_simple";

int main() {
	Options options;
	SstFileWriter sst_file_writer(EnvOptions(), options);
	// Path to where we will write the SST file
	std::string file_path = "/tmp/sst/file1.sst";
	//std::string file_path = "/tmp/sst/001111.sst";

    Status s;
#if 0
	// Open the file for writing
	s = sst_file_writer.Open(file_path);
	if (!s.ok()) {
		cout << "Error while opening file " << file_path << ", Error: " << s.ToString() << endl;
		return 1;
	}
#endif

#if 1
	IngestExternalFileOptions ifo;
    // ifo.write_global_seqno = false;
    DB* db;
	// Optimize RocksDB. This is the easiest way to get RocksDB to perform well
	options.IncreaseParallelism();
	options.OptimizeLevelStyleCompaction();
	// create the DB if it's not already present
	options.create_if_missing = true;

	// open DB
	s = DB::Open(options, kDBPath, &db);
	assert(s.ok());
	// Ingest the 2 passed SST files into the DB
	s = db->IngestExternalFile({file_path}, ifo);
	if (!s.ok()) {
        cout << "Error while adding file " << file_path << ", Error " << s.ToString() << endl;
	  return 1;
	}
    cout << "load sst file sucess, status: " << s.ToString() << endl;
    auto iter = db->NewIterator(ReadOptions());
    //iter->SeekToFirst();
    for (iter->SeekToFirst(); iter->Valid(); iter->Next()) {
        cout << iter->key().ToString() << " | " << iter->value().ToString() << endl;
    }
#endif

#if 0
	// Insert rows into the SST file, note that inserted keys must be 
	// strictly increasing (based on options.comparator)
	for (int i = 0; i < 10; i++) {
        char key[32] = {0};
        char value[64] = {0};
        sprintf(key, "%d", i);
        sprintf(value, "%d_v", i);
		s = sst_file_writer.Put(key, value);
		if (!s.ok()) {
		  cout << "Error while adding Key: " << key << ", Error: " << s.ToString() << endl;
		  return 1;
		}
	}

    ExternalSstFileInfo ei;
    ei.sequence_number = 1;
    ei.version = 6;
    cout << "seq: " << ei.sequence_number << ", ver: " << ei.version << endl;
	// Close the file
	s = sst_file_writer.Finish(&ei);
	if (!s.ok()) {
		cout << "Error while finishing file " << file_path << ", Error: " << s.ToString() << endl;
		return 1;
	}
    //cout << "seq: " << ei.sequence_number << ", ver: " << ei.version << ", session: "<< sst_file_writer.rep_.get()->db_session_id << endl;
    cout << "seq: " << ei.sequence_number << ", ver: " << ei.version << ", db session id: "<< sst_file_writer.DbSessionId() << endl;
#endif
    return 0;
}
