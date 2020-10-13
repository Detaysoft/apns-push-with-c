# nghttp2:
set(ENABLE_LIB_ONLY ON CACHE type BOOL)
set(ENABLE_ASIO_LIB ON CACHE type BOOL)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/nghttp2)

# docopt
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/docopt.cpp)

# velocypack
set(BuildVelocyPackExamples OFF CACHE type BOOL)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/velocypack)

# RocksDB

set(WITH_JEMALLOC OFF CACHE type BOOL)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/rocksdb)
