#! /usr/bin/env python
# encoding: utf-8

from waflib import Logs
from waflib.Configure import conf

@conf
def check_sparkle_base (self, *k, **kw):
  self.check_cxx(framework_name="Sparkle", header_name=["Foundation/Foundation.h", "AppKit/AppKit.h"],
                 uselib_store='SPARKLE', define_name='HAVE_SPARKLE',
                 compile_filename='test.mm', use="FOUNDATION APPKIT",
                 *k,
                 **kw
                 )

@conf
def check_sparkle(self, *k, **kw):
    try:
        self.check_sparkle_base(*k, **kw)
    except:
        try:
            # Try local path
            # Logs.info("Check local version of Sparkle framework")
            self.check_sparkle_base(cxxflags="-F%s/Frameworks/" % self.path.abspath(),
                                    linkflags="-F%s/Frameworks/" % self.path.abspath())
        except:
            import urllib, subprocess, os, shutil
            if not os.path.exists('osx/Frameworks/Sparkle.framework'):
                # Download to local path and retry
                Logs.info("Sparkle framework not found, trying to download it to 'build/'")

                urllib.urlretrieve("https://github.com/sparkle-project/Sparkle/releases/download/1.7.1/Sparkle-1.7.1.zip", "build/Sparkle.zip")
                if os.path.exists('build/Sparkle.zip'):
                    # try:
                        subprocess.check_call(['unzip', '-qq', 'build/Sparkle.zip', '-d', 'build/Sparkle'])
                        os.remove("build/Sparkle.zip")
                        if not os.path.exists("osx/Frameworks"):
                            os.mkdir("osx/Frameworks")
                        os.rename("build/Sparkle/Sparkle.framework", "osx/Frameworks/Sparkle.framework")
                        shutil.rmtree("build/Sparkle", ignore_errors=True)

                        self.check_sparkle_base(cxxflags="-F%s/Frameworks/" % self.path.abspath(),
                                                 linkflags="-F%s/Frameworks/" % self.path.abspath())
                    # except subprocess.CalledProcessError as e:
                    #     self.fatal("Cannot find Sparkle framework. Auto download failed: '%s' returned %s" %(' '.join(e.cmd), e.returncode))
                    # except:
                    #     self.fatal("Unknown Error happened when auto downloading Sparkle framework")
def configure(conf):
    conf.check_sparkle()
