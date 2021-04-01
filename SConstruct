import fnmatch
import sys
import os
from subprocess import call



# User variables
VST2_SDK_HEADERS        = 'E:/dev/.____Libraries/VST3 SDK'
VST2_SDK_CPP            = 'E:/dev/.____Libraries/VST3 SDK/public.sdk/source/vst2.x'
OPENGL_AND_GLEW_HEADERS = 'E:/dev/.____Libraries/GL/include'
GLEW_LIBRARY            = 'E:/dev/.____Libraries/GL/lib/x64'
WEBP_INC                = 'E:/dev/.____Libraries/libwebp_x64/include'
WEBP_LIB                = 'E:/dev/.____Libraries/libwebp_x64/lib'
# VANANAGUI               = 'E:/dev/.____Libraries/VananaGUI'


env = Environment(
    CPPPATH=['./src', VST2_SDK_HEADERS, OPENGL_AND_GLEW_HEADERS, WEBP_INC],
    CPPDEFINES=['WIN32'],
    CCFLAGS='/nologo /EHsc ',
    LIBS=['user32', 'gdi32', 'shell32', 'opengl32', 'glew32s', 'libwebp'],
    LIBPATH=[GLEW_LIBRARY, WEBP_LIB],

    SHCXXCOMSTR = "Compiling $TARGET",
    SHLINKCOMSTR = "Linking $TARGET"
)
env.Append(LINKFLAGS='/DEF:src/exports.def')
env.Append(LINKFLAGS='/SUBSYSTEM:CONSOLE')
env.Append(LINKFLAGS='build/assets.res')

# Debug and release target
release = ARGUMENTS.get('release', 0)
if int(release):
    env.Append(CCFLAGS = '/O2')
    env.Append(LINKFLAGS = '')
else:
    env.Append(CCFLAGS = '/Od')
    env.Append(LINKFLAGS = '/DEBUG:FULL')
    env.Append(CPPDEFINES = 'DEBUG')


# if 'x86' == arch:
#     env.Append(LINKFLAGS = '/MACHINE:X86')
# elif 'x64' == architecture:
#     # nothing



# Gather list of sources
sources = []
for root, dirnames, filenames in os.walk('src'):
    for filename in filenames:
        if fnmatch.fnmatch(filename, '*.cpp'):
            sources.append(str(os.path.join(root, filename)))

for root, dirnames, filenames in os.walk(VST2_SDK_CPP):
    for filename in filenames:
        if fnmatch.fnmatch(filename, '*.cpp'):
            sources.append(str(os.path.join(root, filename)))

# for root, dirnames, filenames in os.walk(VANANAGUI):
#     for filename in filenames:
#         if fnmatch.fnmatch(filename, '*.cpp'):
#             sources.append(str(os.path.join(root, filename)))


# Build DLL
call('windres src/assets.rc build/assets.res')
VariantDir('build', 'src', duplicate=0)
sources = [s.replace('src', 'build', 1) for s in sources]
env.SharedLibrary('build/I3DL2Refurb.dll', source=sources)
