# -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-
VERSION='0.2'
APPNAME='nfd-control-center'
APPCAST='https://irl.cs.ucla.edu/~cawka/ndn-control-center.xml'

from waflib import Logs, Utils, Task, TaskGen
import os

def options(opt):
    opt.load('compiler_c compiler_cxx qt5 gnu_dirs')
    opt.load('boost osx-frameworks default-compiler-flags', tooldir='.waf-tools')

def configure(conf):
    conf.load('compiler_c compiler_cxx default-compiler-flags boost osx-frameworks')

    if 'PKG_CONFIG_PATH' not in os.environ:
        conf.environ['PKG_CONFIG_PATH'] = Utils.subst_vars('${LIBDIR}/pkgconfig', conf.env)

    # add homebrew path, as qt5 is no longer linked
    conf.environ['PKG_CONFIG_PATH'] += ":/usr/local/opt/qt5/lib/pkgconfig:/opt/qt5/5.8/clang_64/lib/pkgconfig"
    conf.environ['PATH'] += ":/usr/local/opt/qt5/bin:/opt/qt5/5.8/clang_64/bin"

    conf.load('qt5')

    conf.check_cfg(package='libndn-cxx', args=['--cflags', '--libs'],
                   uselib_store='NDN_CXX', mandatory=True)

    conf.check_boost(lib="system thread")

    conf.define('RESOURCES_DIR', Utils.subst_vars("${DATAROOTDIR}/nfd-control-center", conf.env))

    if Utils.unversioned_sys_platform() == "darwin":
        conf.define('OSX_BUILD', 1)

    conf.define('NCC_VERSION', VERSION)
    conf.define('NCC_APPCAST', APPCAST)

    conf.write_config_header('config.hpp')

def build(bld):
    app = bld(
        features=['qt5', 'cxxprogram', 'cxx'],
        includes = ". src",
        use = "NDN_CXX BOOST QT5CORE QT5DBUS QT5QML QT5WIDGETS",
        moc = "src/tray-menu.hpp src/key-tree-model.hpp src/key-viewer-dialog.hpp src/cert-tree-model.hpp src/fib-status.hpp src/forwarder-status.hpp",
        source = bld.path.ant_glob(['src/*.cpp', 'src/**/*.qrc', 'src/**/*.ui', 'src/**/*.qrc'], excl=['src/osx-*']),
        )

    if Utils.unversioned_sys_platform() != "darwin":
        app.target = "nfd-control-center"

        bld(features = "subst",
             source = 'nfd-control-center.desktop.in',
             target = 'nfd-control-center.desktop',
             BINARY = "nfd-control-center",
             install_path = "${DATAROOTDIR}/nfd-control-center"
            )

        bld.install_files("${DATAROOTDIR}/nfd-control-center",
                          bld.path.ant_glob(['res/*']))
    else:
        bld(features="subst",
            source='src/Info.plist.in',
            target='src/Info.plist',
            install_path=None,
            VERSION=VERSION,
            APPCAST=APPCAST)
        bld.env['INCLUDES_NDN_TOOLS'] = 'build/ndn-tools'

        app.source += bld.path.ant_glob(['src/osx-*.mm', 'src/osx-*.cpp'])
        app.use += " OSX_FOUNDATION OSX_APPKIT OSX_SPARKLE OSX_COREWLAN NDN_TOOLS"
        app.target = "NFD Control Center"
        app.mac_app = True
        app.mac_plist = 'src/Info.plist'
        app.mac_files = [i.path_from(bld.path) for i in bld.path.ant_glob('res/**/*', excl='**/*.ai')]

from waflib import TaskGen
@TaskGen.extension('.mm')
def m_hook(self, node):
    """Alias .mm files to be compiled the same as .cpp files, gcc/clang will do the right thing."""
    return self.create_compiled_task('cxx', node)
