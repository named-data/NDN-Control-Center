#!/usr/bin/env bash

NDN_CXX_COMMIT=${NDN_CXX_COMMIT:-master}
NFD_COMMIT=${NFD_COMMIT:-master}
NDN_TOOLS_COMMIT=${NDN_TOOLS_COMMIT:-master}

GIT=${GIT:-https://github.com/named-data}

mkdir build 2>/dev/null || true
path="$(pwd)"

#######################################

rm -Rf build/ndn-cxx
git clone ${GIT}/ndn-cxx build/ndn-cxx
pushd build/ndn-cxx
git checkout ${NDN_CXX_COMMIT}

patch -p1 <<EOF
diff --git a/src/transport/unix-transport.cpp b/src/transport/unix-transport.cpp
index f2e44aa..268f967 100644
--- a/src/transport/unix-transport.cpp
+++ b/src/transport/unix-transport.cpp
@@ -40,7 +40,7 @@ std::string
 UnixTransport::getSocketNameFromUri(const std::string& uriString)
 {
   // Assume the default nfd.sock location.
-  std::string path = "/var/run/nfd.sock";
+  std::string path = "/tmp/nfd.sock";
 
   if (uriString.empty()) {
     return path;
EOF

./waf configure --prefix="${path}/build/deps" \
                --sysconfdir="/Applications/NDN.app/Contents/etc"
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
               ./waf configure --prefix="${path}/build/deps" \
                               --sysconfdir="/Applications/NDN.app/Contents/etc"
./waf build
./waf install
popd

####################################

rm -Rf build/ndn-tools
git clone ${GIT}/ndn-tools build/ndn-tools
pushd build/ndn-tools
git checkout ${NDN_TOOLS_COMMIT}
PKG_CONFIG_PATH="${path}/build/deps/lib/pkgconfig:${PKG_CONFIG_PATH}" \
               ./waf configure --prefix="${path}/build/deps" \
                               --sysconfdir="/Applications/NDN.app/Contents/etc"
./waf build
./waf install
popd

####################################

PKG_CONFIG_PATH="${path}/build/deps/lib/pkgconfig:${PKG_CONFIG_PATH}" \
               ./waf configure
