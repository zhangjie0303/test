#include <iostream>
#include <stdio.h>

#include "rocksdb/sst_file_reader.h"
#include "rocksdb/sst_file_writer.h"

const std::string file_name = "/tmp/sst_file.sst";

using namespace ROCKSDB_NAMESPACE;

int main(int argc, char** argv) {
	rocksdb::Options options;
	options.env = Env::Default();
	
	if (options.env == nullptr) {
		std::cout << "env is nullptr." << std::endl;
		return -1;
	}

	SstFileWriter writer(EnvOptions(), options);
	Status status = writer.Open(file_name);
	if (!status.ok()) {
		std::cout << "writer open error: " << status.ToString() << std::endl;
		return -1;
	}
	
	// write
	for (int i = 0; i < 100; i++) {
		char buf[32] = {0};
		sprintf(buf, "%03d", i);

		std::string key = "key_" + std::string(buf);
		std::string value = "value_" + std::string(buf);
		status = writer.Put(Slice(key), Slice(value));
		if (!status.ok()) {
			std::cout << "writer put error: " << status.ToString() << std::endl;
			return -1;
		}

		std::cout << "Put OK: " << key << " | " << value << std::endl;
	}

	status = writer.Finish();
	if (!status.ok()) {
		std::cout << "writer finish error: " << status.ToString() << std::endl;
		return -1;
	}

	std::cout << "--------------------------------------------------" << std::endl;

	// read
	rocksdb::SstFileReader reader(options);
	status = reader.Open(file_name);
	if (!status.ok()) {
		std::cout << "reader open error: " << status.ToString() << std::endl;
		return -1;
	}

    std::unique_ptr<rocksdb::Iterator> iter(reader.NewIterator(ReadOptions()));
    for (iter->SeekToFirst(); iter->Valid(); iter->Next()) {
      std::cout << "zhangjie-> " << iter->key().ToStringView() << " | " << iter->value().ToStringView() << std::endl;
    }

	return 0;
}

