project(
    'swflib',
    'cpp',
    default_options: ['cpp_std=c++17'],
    version: '0.4.0',
)

zlib = dependency('zlib')
liblzma = dependency('liblzma')
incdir = include_directories('include')

sources = []
private_inc = []
subdir('lib')

swflib = library(
    'swflib',
    sources,
    include_directories: [incdir, private_inc],
    dependencies: [zlib, liblzma],
    install: true,
)
swflib_dep = declare_dependency(
    include_directories: incdir,
    link_with: swflib,
)