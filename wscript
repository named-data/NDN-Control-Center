# -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-
VERSION='0.1'
APPNAME='nfd-control-center'

from waflib import Logs, Utils, Task, TaskGen

def options(opt):
    opt.load('compiler_c compiler_cxx qt5 gnu_dirs')
    opt.load('msvs boost sparkle xcode default-compiler-flags', tooldir='.waf-tools')

    # grp = opt.add_option_group('NFD Control Center options')

def configure(conf):
    conf.load('compiler_c compiler_cxx qt5 default-compiler-flags boost')

    conf.check_cfg(package='libndn-cxx', args=['--cflags', '--libs'],
                   uselib_store='NDN_CXX', mandatory=True)

    conf.check_boost(lib="system thread")

    # conf.define('RESOURCES_DIR', Utils.subst_vars("${DATAROOTDIR}/nfd-control-center", conf.env))

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
             source = 'src/nfd-control-center.desktop.in',
             target = 'src/nfd-control-center.desktop',
             BINARY = "nfd-control-center",
             install_path = "${DATAROOTDIR}/nfd-control-center"
            )

        bld.install_files("${DATAROOTDIR}/nfd-control-center",
                      bld.path.ant_glob(['Resources/*']))
    else:
        app.target = "NFD Control Center"
        app.mac_app = True
        # app.mac_plist = 'src/Info.plist'
        # app.mac_resources = [i.path_from(bld.path)
        #                      for i in bld.path.parent.ant_glob('src/Resources/*')]
