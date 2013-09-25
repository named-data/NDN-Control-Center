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
        conf.find_program('ibtool', var='IBTOOL', mandatory=False)

        conf.check_cxx(framework_name='Foundation', uselib_store='FOUNDATION', compile_filename='test.mm')
        conf.check_cxx(framework_name='AppKit',     uselib_store='APPKIT',     compile_filename='test.mm')
        conf.check_cxx(framework_name='Cocoa',      uselib_store='COCOA',      compile_filename='test.mm')

        conf.env.ARCH_OSX = 'x86_64'
        conf.env.CXXFLAGS_OSX += ['-fobjc-arc', '-mmacosx-version-min=10.8']
        conf.env.LINKFLAGS_OSX += ['-mmacosx-version-min=10.8']
        
        conf.env.MACOSX_DEPLOYMENT_TARGET = '10.8'
        
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
            source = bld.path.ant_glob (['osx/**/*.mm', 'osx/MainMenu.xib']),
            
            mac_app = True,
            use = "OSX COCOA FOUNDATION APPKIT SPARKLE",

            mac_plist = 'osx/Info.plist',
            mac_resources = [i.path_from(bld.path) for i in bld.path.ant_glob ('osx/Resources/**/*')],
            mac_frameworks = "osx/Frameworks/Sparkle.framework",
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
    inst_to = getattr(self, 'install_path', '/Applications') + '/%s/Resources' % name
    self.bld.install_as (inst_to + '/%s' % real_out.name, real_out)
        
class xib(Task.Task):
    color='PINK'
    run_str = '${IBTOOL} --errors --warnings --notices --minimum-deployment-target 10.8 --output-format human-readable-text  --compile ${TGT} ${SRC}'
