NFD Control Center
==================

NFD Control Center is a helper application to manager local instance of NFD.  In addition to that, on macOS NFD Control Center also bundles an internal version of NFD.

## Notes

As of November 14, 2016, to successfully build the macOS app bundle with HomeBrew version of QT libraries, it is necessary to apply the following patch to HomeBrew formula and build qt5 from source (`brew install qt5 --build-from-source`):

```patch
diff --git a/Formula/qt5.rb b/Formula/qt5.rb
index 8f2a408..302f4fa 100644
--- a/Formula/qt5.rb
+++ b/Formula/qt5.rb
@@ -115,6 +115,11 @@ class Qt5 < Formula
     sha256 "48ff18be2f4050de7288bddbae7f47e949512ac4bcd126c2f504be2ac701158b"
   end

+  patch do
+    url "https://gist.githubusercontent.com/cawka/3025baecf3b05e14311b82210a15320f/raw/7ac6a2696f19630df5d02ccc0e48aec733da1364/qt5-patch"
+    sha256 "b18e4715fcef2992f051790d3784a54900508c93350c25b0f2228cb058567142"
+  end
+
   def install
     args = %W[
       -verbose
```

For more information, see https://github.com/Homebrew/homebrew-core/issues/3219 and https://bugreports.qt.io/browse/QTBUG-56814

* * *

The code cannot be compiled with the official version of QT libraries, as they are currently missing .pc file for pkg-config detection of libraries.  This relates to the upstream [QT commit](https://codereview.qt-project.org/#/c/140954/) that disabled generation of .pc file. HomeBrew partially reverts this commit in a [patch](https://raw.githubusercontent.com/Homebrew/formula-patches/e8fe6567/qt5/restore-pc-files.patch).
