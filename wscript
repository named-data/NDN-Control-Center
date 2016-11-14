# -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-
VERSION='0.1.1'
APPNAME='nfd-control-center'

from waflib import Logs, Utils, Task, TaskGen
import os

def options(opt):
    opt.load('compiler_c compiler_cxx qt5 gnu_dirs')
    opt.load('msvs boost sparkle xcode default-compiler-flags', tooldir='.waf-tools')

    # grp = opt.add_option_group('NFD Control Center options')

def configure(conf):
    conf.load('compiler_c compiler_cxx default-compiler-flags boost')

    if 'PKG_CONFIG_PATH' not in os.environ:
        os.environ['PKG_CONFIG_PATH'] = Utils.subst_vars('${LIBDIR}/pkgconfig', conf.env)

    # add homebrew path, as qt5 is no longer linked
    os.environ['PKG_CONFIG_PATH'] += ":/usr/local/opt/qt5/lib/pkgconfig:/opt/qt5/5.8/clang_64/lib/pkgconfig"
    os.environ['PATH'] += ":/usr/local/opt/qt5/bin:/opt/qt5/5.8/clang_64/bin"

    conf.load('qt5')

    conf.check_cfg(package='libndn-cxx', args=['--cflags', '--libs'],
                   uselib_store='NDN_CXX', mandatory=True)

    conf.check_boost(lib="system thread")

    conf.define('RESOURCES_DIR', Utils.subst_vars("${DATAROOTDIR}/nfd-control-center", conf.env))

    if Utils.unversioned_sys_platform() == "darwin":
        conf.define('OSX_BUILD', 1)

    conf.write_config_header('config.hpp')

def build(bld):
    app = bld(
        features=['qt5', 'cxxprogram', 'cxx'],
        includes = ". src",

        use = "NDN_CXX BOOST QT5CORE QT5DBUS QT5QML QT5WIDGETS",

        defines = "WAF",
        source = bld.path.ant_glob(['src/*.cpp', 'src/**/*.qrc', 'src/**/*.ui', 'src/**/*.qrc']),
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
        app.target = "NFD Control Center"
        app.mac_app = True
        app.mac_plist = 'src/Info.plist'
        app.mac_resources = [i.path_from(bld.path) for i in bld.path.ant_glob('res/**/*', excl='**/*.ai')]
