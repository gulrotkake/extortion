APPNAME='extortion'
VERSION='0.1.1'
out='build'
top="."

def options(opt):
    opt.load('compiler_cxx waf_unit_test')

def configure(conf):
    conf.load('compiler_cxx waf_unit_test')
    conf.env['CXXFLAGS'] += ['-g', '-Wall', '-O2', '-Wno-strict-aliasing']
    conf.env['INCLUDES'] += ['..']

    conf.check(header_name='expat.h', mandatory=True)
    conf.check(header_name='tr1/functional', mandatory=True)

    conf.check(lib='expat', mandatory=True)

def build(ctx):
    ctx.recurse('src')
    ctx.recurse('tests')
