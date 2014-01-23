# -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-
VERSION='1.0'
APPNAME='ndnx-control-center'

from waflib import Logs, Utils, Task, TaskGen

def options(opt):
    opt.load('compiler_c compiler_cxx qt4 gnu_dirs')
    opt.load('sparkle xcode', tooldir='waf-tools')

    grp = opt.add_option_group ('NDNx Control Center options')
    grp.add_option ('--ndnx', help='''Root path to NDNx installation (default: /usr/local/ndn)''',
                    dest='ndnx_root', type=str, default='/usr/local/ndn')

    if Utils.unversioned_sys_platform () == "darwin":
        grp.add_option ('--qt4', help='''Build QT4 app, instead of native one''',
                        action='store_true', dest='build_qt', default=False)

    
def configure(conf):
    conf.load('compiler_c compiler_cxx')

    conf.start_msg('Checking for ndnd-tlv in %s' % conf.options.ndnx_root)
    if not conf.find_file('ndnd-tlv ndnd-tlv-status ndnd-tlv-start ndnd-tlv-stop', path_list='%s/bin' % conf.options.ndnx_root, mandatory=False):
        conf.fatal ('not found', 'RED')
    else:
        conf.end_msg ('ok')

    conf.define('NDNX_ROOT', conf.options.ndnx_root)
    conf.define('NDND_STATUS_COMMAND', '%s/bin/ndndsmoketest' % conf.options.ndnx_root)
    conf.define('NDND_FIB_COMMAND',  '%s/bin/ndndc' % conf.options.ndnx_root)
    conf.define('NDND_AUTOCONFIG_COMMAND', '%s/bin/ndnd-autoconfig' % conf.options.ndnx_root)
    
    if Utils.unversioned_sys_platform () == "darwin" and not conf.options.build_qt:
        conf.define('NDND_START_COMMAND', '%s/bin/ndnd-tlv-start' % conf.options.ndnx_root)
        conf.define('NDND_STOP_COMMAND', '%s/bin/ndnd-tlv-stop' % conf.options.ndnx_root)

        conf.env.BUILD_OSX_NATIVE = 1
        
        conf.find_program('ibtool', var='IBTOOL', mandatory=False)

        conf.check_cxx(framework_name='Foundation', uselib_store='FOUNDATION', compile_filename='test.mm')
        conf.check_cxx(framework_name='AppKit',     uselib_store='APPKIT',     compile_filename='test.mm')
        conf.check_cxx(framework_name='Cocoa',      uselib_store='COCOA',      compile_filename='test.mm')

        conf.env.ARCH_OSX = 'x86_64'
        conf.env.CXXFLAGS_OSX += ['-fobjc-arc', '-mmacosx-version-min=10.7']
        conf.env.LINKFLAGS_OSX += ['-mmacosx-version-min=10.7']        
        conf.env.MACOSX_DEPLOYMENT_TARGET = '10.7'
        
        conf.load('sparkle')
    else:
        conf.load('qt4')
        conf.load('gnu_dirs')
        conf.find_program('xsltproc', mandatory=True)
        conf.define('XSLTPROC', conf.env['XSLTPROC'])

        conf.define('RESOURCES_DIR', Utils.subst_vars("${DATAROOTDIR}/ndnx-control-center", conf.env))
        
        if Utils.unversioned_sys_platform () == "darwin":
            conf.define('OSX_BUILD', 1)
        
    conf.write_config_header('config.h')

def build (bld):
    if bld.env.BUILD_OSX_NATIVE:
        bld (
            target = "NDNx Control Center",
            features=['cxxprogram', 'cxx'],
            includes = ". osx",
            source = bld.path.ant_glob (['osx/**/*.mm', 'osx/MainMenu.xib']),
            
            mac_app = True,
            use = "OSX COCOA FOUNDATION APPKIT SPARKLE",

            mac_plist = 'osx/Info.plist',
            mac_resources = [i.path_from(bld.path) for i in bld.path.ant_glob ('osx/Resources/**/*')],
            mac_frameworks = "osx/Frameworks/Sparkle.framework",
            )
    else:
        bld (features = "subst",
             source = 'linux/ndnxcontrolcenter.desktop.in',
             target = 'linux/ndnxcontrolcenter.desktop',
             BINARY = "ndnx-control-center",
             install_path = "${DATAROOTDIR}/ndnx-control-center"
            )
        bld (features = "subst",
             source = 'linux/ndnx-control-center.desktop.in',
             target = 'linux/ndnx-control-center.desktop',
             BINARY = "ndnx-control-center",
             install_path = "${DATAROOTDIR}/applications"
            )
        bld.install_files("${DATAROOTDIR}/ndnx-control-center",
                          bld.path.ant_glob(['linux/Resources/*']))

        bld (
            target = "ndnx-control-center",
            features=['qt4', 'cxxprogram', 'cxx'],
            includes = ". linux",

            use = "QTCORE QTGUI QTXML QTXMLPATTERNS QTDBUS",
            
            defines = "WAF",
            source = bld.path.ant_glob (['linux/**/*.cpp', 'linux/**/*.ui', 'linux/**/*.qrc']),
            )

        
from waflib import TaskGen
@TaskGen.extension('.mm')
def m_hook(self, node):
    """Alias .mm files to be compiled the same as .cc files, gcc/clang will do the right thing."""
    return self.create_compiled_task('cxx', node)

@TaskGen.extension('.m')
def m_hook(self, node):
    """Alias .m files to be compiled the same as .c files, gcc/clang will do the right thing."""
    return self.create_compiled_task('c', node)


def bundle_name_for_output(name):
	k = name.rfind('.')
	if k >= 0:
		name = name[:k] + '.app'
	else:
		name = name + '.app'
	return name

@TaskGen.extension('.xib')
def xib(self,node):
    out = node.change_ext ('.nib')

    name = self.path.get_bld ().find_or_declare (bundle_name_for_output(self.target))
    resources = name.find_or_declare(['Contents', 'Resources'])
    resources.mkdir()
    real_out = resources.make_node (out.name)

    self.create_task('xib', node, real_out)
    inst_to = getattr(self, 'install_path', '/Applications') + '/%s/Contents/Resources' % name
    self.bld.install_as (inst_to + '/%s' % real_out.name, real_out)
        
class xib(Task.Task):
    color='PINK'
    run_str = '${IBTOOL} --errors --warnings --notices --output-format human-readable-text  --compile ${TGT} ${SRC}'
