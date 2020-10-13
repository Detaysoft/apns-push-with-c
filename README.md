# iospush - apple iOS APNS Push With C++

## How to build

~~~~~
mkdir build
cd build
cmake ..
make
~~~~~

and the executable is in `build/iospush`.

## Third party software used:

  - `nghttp2` and `nghttp2_asio` for HTTP/2 support (client and server)
  - RocksDB as storage layer
  - `` for parsing command line options
  - OpenSSL for encryption (not included in source tree)
  - Boost for asio and some other stuff needed (not included in source tree)
