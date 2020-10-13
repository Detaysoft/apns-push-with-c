cmake_minimum_required(VERSION 3.10)
project(iospush VERSION 0.0.1)

find_package(OpenSSL 1.1.0)
find_package(Boost 1.65.0 REQUIRED system thread)

add_subdirectory(3rdParty)

add_executable(iospush iospush.cpp)

target_include_directories(iospush PRIVATE
  ${OPENSSL_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIRS}
  ${nghttp2_SOURCE_DIR}/src/includes
  3rdParty/docopt.cpp
  3rdParty/velocypack/include
  ${ROCKSDB_INCLUDE_DIRS}
  3rdParty/spdlog/include
)

target_link_libraries(iospush
  docopt
  nghttp2
  nghttp2_asio
  velocypack
  rocksdb
  ${OPENSSL_LIBRARIES}
  ${Boost_LIBRARIES}
)