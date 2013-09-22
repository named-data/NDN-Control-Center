# -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-
VERSION='1.0'
APPNAME='ndnx-control-center'

from waflib import Logs, Utils, Task, TaskGen

def options(opt):
    opt.load('compiler_c compiler_cxx')
    opt.load('sparkle', tooldir='waf-tools')

def configure(conf):
    conf.load('compiler_c compiler_cxx')

    if Utils.unversioned_sys_platform () != "darwin":
        Logs.error ("Only OSX is supported so far")
        return -1

    if Utils.unversioned_sys_platform () == "darwin":
        conf.check_cxx(framework_name='Foundation', uselib_store='OSX_FOUNDATION', compile_filename='test.mm')
        conf.check_cxx(framework_name='AppKit',     uselib_store='OSX_APPKIT',     compile_filename='test.mm')
        conf.check_cxx(framework_name='Cocoa',      uselib_store='OSX_COCOA',     compile_filename='test.mm')

        conf.load('sparkle')

def build (bld):
    if Utils.unversioned_sys_platform () != "darwin":
        Logs.error ("Only OSX is supported so far")
        return -1

    if Utils.unversioned_sys_platform () == "darwin":
        bld (
            target = "NDNx Control Center",
            features=['cxxprogram', 'cxx'],
            includes = "osx",
            mac_app = "NDNx Control Center.app",
            source = bld.path.ant_glob ('osx/**/*.mm'),
            use = "OSX_FOUNDATION OSX_APPKIT OSX_COCOA OSX_SPARKLE",

            mac_plist = bld.path.find_resource('osx/Info.plist').read (),
            mac_resources = 'osx/ndnx-main.icns osx/ndnx-tray.icns',
            mac_frameworks = "osx/Frameworks/Sparkle.framework",
            )

from waflib import TaskGen
@TaskGen.extension('.mm')
def m_hook(self, node):
    """Alias .mm files to be compiled the same as .cc files, gcc/clang will do the right thing."""
    return self.create_compiled_task('cxx', node)
