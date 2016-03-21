#!/usr/bin/env bash

NDN_CXX_COMMIT=${NDN_CXX_COMMIT:-master}
NFD_COMMIT=${NFD_COMMIT:-master}

GIT=${GIT:-https://github.com/named-data}

mkdir build 2>/dev/null || true
path="$(pwd)"

#######################################

rm -Rf build/ndn-cxx
git clone ${GIT}/ndn-cxx build/ndn-cxx
pushd build/ndn-cxx
git checkout ${NDN_CXX_COMMIT}
./waf configure --prefix="${path}/build/deps"
./waf build
./waf install
popd

####################################

rm -Rf build/NFD
git clone ${GIT}/NFD build/NFD
pushd build/NFD
git checkout ${NFD_COMMIT}
git submodule update --init
PKG_CONFIG_PATH="${path}/build/deps/lib/pkgconfig:${PKG_CONFIG_PATH}" \
               ./waf configure --prefix="${path}/build/deps"
./waf build
./waf install
popd
