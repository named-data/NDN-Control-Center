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

patch -p1 <<EOF
diff --git a/src/transport/unix-transport.cpp b/src/transport/unix-transport.cpp
index 59c00ae..aa67b24 100644
--- a/src/transport/unix-transport.cpp
+++ b/src/transport/unix-transport.cpp
@@ -74,7 +74,7 @@ UnixTransport::getDefaultSocketName(const ConfigFile& config)
     }

   // Assume the default nfd.sock location.
-  return "/var/run/nfd.sock";
+  return "/tmp/nfd.sock";
 }

 shared_ptr<UnixTransport>
@@ -135,4 +135,4 @@ UnixTransport::resume()

 }

-#endif // _WIN32
\ No newline at end of file
+#endif // _WIN32
EOF

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

PKG_CONFIG_PATH="${path}/build/deps/lib/pkgconfig:${PKG_CONFIG_PATH}" \
               ./waf configure
