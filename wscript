# -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-
VERSION='0.1'
APPNAME='nfd-control-center'

from waflib import Logs, Utils, Task, TaskGen

def options(opt):
    opt.load('compiler_c compiler_cxx qt4 gnu_dirs')
    opt.load('sparkle xcode', tooldir='waf-tools')

    grp = opt.add_option_group('NFD Control Center options')
    grp.add_option('--with-nfd', dest='with_nfd', type=str, default='/usr/local',
                   help='''Root path to NFD installation (default: /usr/local)''')

    if Utils.unversioned_sys_platform () == "darwin":
        grp.add_option('--with-qt', help='''Build QT4 app, instead of native one''',
                       action='store_true', dest='with_qt', default=False)

def configure(conf):
    conf.load('compiler_c compiler_cxx')

    conf.start_msg('Checking for NFD tools in %s' % conf.options.with_nfd)
    if not conf.find_file(['nfd-start', 'nfd-stop'],
                          path_list='%s/bin' % conf.options.with_nfd, mandatory=False):
        conf.fatal('not found', 'RED')
    else:
        conf.end_msg('ok')

    conf.check_cfg(package='libndn-cxx', args=['--cflags', '--libs'],
                   uselib_store='NDN_CXX', mandatory=True)

    conf.define('NFD_ROOT', conf.options.with_nfd)
    conf.define('NFD_START_COMMAND', '%s/bin/nfd-start' % conf.options.with_nfd)
    conf.define('NFD_STOP_COMMAND', '%s/bin/nfd-stop' % conf.options.with_nfd)
    conf.define('NFD_AUTOCONFIG_COMMAND', '%s/bin/ndn-autoconfig' % conf.options.with_nfd)

    if not conf.options.with_qt and Utils.unversioned_sys_platform() == "darwin":
        # conf.fatal("Native implementation of NFD Control Center is not yet available")
        conf.env.BUILD_OSX_NATIVE = 1
        conf.recurse('osx')
    else:
        conf.recurse('qt')

    conf.write_config_header('config.hpp')

def build(bld):
    if bld.env.BUILD_OSX_NATIVE:
        bld.recurse('osx')
    else:
        bld.recurse('qt')
