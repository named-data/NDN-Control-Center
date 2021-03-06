#!/usr/bin/env bash

# Avoid dependency on non-standard tools
export PATH=/usr/bin:/bin:/usr/sbin:/sbin
export PKGCONFIG=/usr/local/bin/pkg-config

NDN_CXX_COMMIT=${NDN_CXX_COMMIT:-1709aa70999bd8cbe0e7c680aaab37f2d24323a5}
NFD_COMMIT=${NFD_COMMIT:-d396b61ba14a84b34d3ae65db4530062f7d43301}
NDN_TOOLS_COMMIT=${NDN_TOOLS_COMMIT:-ndn-tools-0.4}

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
