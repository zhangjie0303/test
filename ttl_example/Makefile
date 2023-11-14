include make_config.mk

ifndef DISABLE_JEMALLOC
	ifdef JEMALLOC
		PLATFORM_CXXFLAGS += -DROCKSDB_JEMALLOC -DJEMALLOC_NO_DEMANGLE
	endif
	EXEC_LDFLAGS := $(JEMALLOC_LIB) $(EXEC_LDFLAGS) -lpthread -lsnappy
	PLATFORM_CXXFLAGS += $(JEMALLOC_INCLUDE)
endif

ifneq ($(USE_RTTI), 1)
	CXXFLAGS += -fno-rtti
endif

INC_DIR = /opt/app/rocksdb/include
LIB_DIR = /opt/app/rocksdb/lib64

CFLAGS += -Wstrict-prototypes

.PHONY: clean librocksdb

all: sst_read_write generate_sst db_with_ttl sst_writer_reader

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@ -I$(INC_DIR)


simple_example: librocksdb simple_example.cc
	$(CXX) $(CXXFLAGS) $@.cc -o$@ ../librocksdb.a -I../include -O2 -std=c++17 $(PLATFORM_LDFLAGS) $(PLATFORM_CXXFLAGS) $(EXEC_LDFLAGS)

sst_read_write: librocksdb sst_read_write.cc
	$(CXX) $(CXXFLAGS) $@.cc -o$@ $(LIB_DIR)/librocksdb.a -I$(INC_DIR) -O2 -std=c++17 $(PLATFORM_LDFLAGS) $(PLATFORM_CXXFLAGS) $(EXEC_LDFLAGS)

generate_sst: librocksdb generate_sst.cc
	$(CXX) $(CXXFLAGS) $@.cc -o$@ $(LIB_DIR)/librocksdb.a -I$(INC_DIR) -O2 -std=c++17 $(PLATFORM_LDFLAGS) $(PLATFORM_CXXFLAGS) $(EXEC_LDFLAGS)

db_with_ttl: librocksdb db_with_ttl.cc
	$(CXX) $(CXXFLAGS) $@.cc -o$@ $(LIB_DIR)/librocksdb.a -I$(INC_DIR) -O2 -std=c++17 $(PLATFORM_LDFLAGS) $(PLATFORM_CXXFLAGS) $(EXEC_LDFLAGS)

sst_writer_reader: librocksdb sst_writer_reader.cc
	$(CXX) $(CXXFLAGS) $@.cc -o$@ $(LIB_DIR)/librocksdb.a -I$(INC_DIR) -O2 -std=c++17 $(PLATFORM_LDFLAGS) $(PLATFORM_CXXFLAGS) $(EXEC_LDFLAGS)

clean:
	rm -rf ./sst_read_write ./generate_sst ./db_with_ttl ./sst_writer_reader

#librocksdb:
#	cd .. && $(MAKE) static_lib
