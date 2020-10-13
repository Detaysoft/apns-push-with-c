#!/bin/sh
if test ! -d nghttp2 ; then
    git clone https://github.com/nghttp2/nghttp2
fi
if test ! -d docopt.cpp ; then
    git clone https://github.com/docopt/docopt.cpp
fi
if test ! -d velocypack ; then
    git clone https://github.com/arangodb/velocypack
fi
if test ! -d rocksdb ; then
    git clone https://github.com/facebook/rocksdb
fi
if test ! -d spdlog ; then
    git clone https://github.com/gabime/spdlog
fi
