#
# Spearmint Makefile
#
# GNU Make required
#

# ioquake3 svn version that this is based on
IOQ3_REVISION = 2275

COMPILE_PLATFORM=$(shell uname|sed -e s/_.*//|tr '[:upper:]' '[:lower:]'|sed -e 's/\//_/g')

COMPILE_ARCH=$(shell uname -m | sed -e s/i.86/i386/)

ifeq ($(COMPILE_PLATFORM),sunos)
  # Solaris uname and GNU uname differ
  COMPILE_ARCH=$(shell uname -p | sed -e s/i.86/i386/)
endif
ifeq ($(COMPILE_PLATFORM),darwin)
  # Apple does some things a little differently...
  COMPILE_ARCH=$(shell uname -p | sed -e s/i.86/i386/)
endif

ifeq ($(COMPILE_PLATFORM),mingw32)
  ifeq ($(COMPILE_ARCH),i386)
    COMPILE_ARCH=x86
  endif
  ifeq ($(COMPILE_ARCH),x86_64)
    COMPILE_ARCH=x64
  endif
endif

ifndef BUILD_CLIENT
  BUILD_CLIENT     =
endif
ifndef BUILD_CLIENT_SMP
  BUILD_CLIENT_SMP =
endif
ifndef BUILD_SERVER
  BUILD_SERVER     =
endif
ifndef BUILD_GAME_SO
  BUILD_GAME_SO    =
endif
ifndef BUILD_GAME_QVM
  BUILD_GAME_QVM   =
endif
ifndef BUILD_GAME_LLVM
  BUILD_GAME_LLVM  =
endif
ifndef BUILD_BASEGAME
  BUILD_BASEGAME =
endif
ifndef BUILD_MISSIONPACK
  BUILD_MISSIONPACK=
endif
ifndef BUILD_FINAL
  BUILD_FINAL      =0
endif

# Default to supporting both .qvm and llvm.bc
ifndef USE_QVM
  USE_QVM=1
endif
ifndef USE_LLVM
  USE_LLVM=1
endif

ifneq ($(PLATFORM),darwin)
  BUILD_CLIENT_SMP = 0
endif

ifneq ($(USE_QVM),1)
   BUILD_GAME_QVM = 0
endif

ifneq ($(USE_LLVM),1)
   BUILD_GAME_LLVM = 0
endif

#############################################################################
#
# If you require a different configuration from the defaults below, create a
# new file named "Makefile.local" in the same directory as this file and define
# your parameters there. This allows you to change configuration without
# causing problems with keeping up to date with the repository.
#
#############################################################################
-include Makefile.local

ifndef PLATFORM
PLATFORM=$(COMPILE_PLATFORM)
endif
export PLATFORM

ifeq ($(COMPILE_ARCH),powerpc)
  COMPILE_ARCH=ppc
endif
ifeq ($(COMPILE_ARCH),powerpc64)
  COMPILE_ARCH=ppc64
endif

ifndef ARCH
ARCH=$(COMPILE_ARCH)
endif
export ARCH

ifneq ($(PLATFORM),$(COMPILE_PLATFORM))
  CROSS_COMPILING=1
else
  CROSS_COMPILING=0

  ifneq ($(ARCH),$(COMPILE_ARCH))
    CROSS_COMPILING=1
  endif
endif
export CROSS_COMPILING

ifndef VERSION
VERSION=1.36
endif

ifndef CLIENTBIN
CLIENTBIN=spearmint
endif

ifndef SERVERBIN
SERVERBIN=spearmint-server
endif

ifndef BASEGAME
BASEGAME=baseq3
endif

ifndef BASEGAME_CFLAGS
BASEGAME_CFLAGS=
endif

ifndef MISSIONPACK
MISSIONPACK=missionpack
endif

ifndef MISSIONPACK_CFLAGS
MISSIONPACK_CFLAGS=-DMISSIONPACK
endif

# Add "-DEXAMPLE" to define EXAMPLE in engine and game/cgame/ui.
ifndef BUILD_DEFINES
BUILD_DEFINES =
endif

ifndef COPYDIR
COPYDIR="/usr/local/games/quake3"
endif

ifndef COPYBINDIR
COPYBINDIR=$(COPYDIR)
endif

ifndef MOUNT_DIR
MOUNT_DIR=code
endif

ifndef BUILD_DIR
BUILD_DIR=build
endif

ifndef TEMPDIR
TEMPDIR=/tmp
endif

ifndef GENERATE_DEPENDENCIES
GENERATE_DEPENDENCIES=1
endif

ifndef USE_OPENAL
USE_OPENAL=1
endif

ifndef USE_OPENAL_DLOPEN
USE_OPENAL_DLOPEN=1
endif

ifndef USE_CURL
USE_CURL=1
endif

ifndef USE_CURL_DLOPEN
  ifeq ($(PLATFORM),mingw32)
    USE_CURL_DLOPEN=0
  else
    USE_CURL_DLOPEN=1
  endif
endif

ifndef USE_CODEC_VORBIS
USE_CODEC_VORBIS=1
endif

ifndef USE_MUMBLE
USE_MUMBLE=1
endif

ifndef USE_VOIP
USE_VOIP=1
endif

ifndef USE_FREETYPE
USE_FREETYPE=1
endif

ifndef USE_INTERNAL_SPEEX
USE_INTERNAL_SPEEX=1
endif

ifndef USE_INTERNAL_ZLIB
USE_INTERNAL_ZLIB=1
endif

ifndef USE_INTERNAL_JPEG
USE_INTERNAL_JPEG=1
endif

ifndef USE_INTERNAL_FREETYPE
USE_INTERNAL_FREETYPE=1
endif

ifndef USE_INTERNAL_LLVM
USE_INTERNAL_LLVM=1
endif

ifndef USE_LOCAL_HEADERS
USE_LOCAL_HEADERS=1
endif

ifndef USE_RENDERER_DLOPEN
USE_RENDERER_DLOPEN=1
endif

ifndef DEBUG_CFLAGS
DEBUG_CFLAGS=-g -O0
endif

ifndef USE_OLD_VM64
USE_OLD_VM64=0
endif

#############################################################################

BD=$(BUILD_DIR)/debug-$(PLATFORM)-$(ARCH)
BR=$(BUILD_DIR)/release-$(PLATFORM)-$(ARCH)
CDIR=$(MOUNT_DIR)/client
SDIR=$(MOUNT_DIR)/server
RDIR=$(MOUNT_DIR)/renderer
CMDIR=$(MOUNT_DIR)/qcommon
SDLDIR=$(MOUNT_DIR)/sdl
ASMDIR=$(MOUNT_DIR)/asm
SYSDIR=$(MOUNT_DIR)/sys
GDIR=$(MOUNT_DIR)/game
CGDIR=$(MOUNT_DIR)/cgame
BLIBDIR=$(MOUNT_DIR)/botlib
NDIR=$(MOUNT_DIR)/null
UIDIR=$(MOUNT_DIR)/ui
Q3UIDIR=$(MOUNT_DIR)/q3_ui
JPDIR=$(MOUNT_DIR)/jpeg-8c
SPEEXDIR=$(MOUNT_DIR)/libspeex
ZDIR=$(MOUNT_DIR)/zlib
FTDIR=$(MOUNT_DIR)/freetype-2.4.9
Q3ASMDIR=$(MOUNT_DIR)/tools/asm
LBURGDIR=$(MOUNT_DIR)/tools/lcc/lburg
Q3CPPDIR=$(MOUNT_DIR)/tools/lcc/cpp
Q3LCCETCDIR=$(MOUNT_DIR)/tools/lcc/etc
Q3LCCSRCDIR=$(MOUNT_DIR)/tools/lcc/src
CLANGDIR=$(MOUNT_DIR)/tools/clang
LLVMDIR=$(MOUNT_DIR)/tools/llvm
LOKISETUPDIR=misc/setup
NSISDIR=misc/nsis
SDLHDIR=$(MOUNT_DIR)/SDL12
LIBSDIR=$(MOUNT_DIR)/libs
OGGDIR=$(MOUNT_DIR)/libogg
VORBISDIR=$(MOUNT_DIR)/libvorbis

bin_path=$(shell which $(1) 2> /dev/null)

# We won't need this if we only build the server
ifneq ($(BUILD_CLIENT),0)
  # set PKG_CONFIG_PATH to influence this, e.g.
  # PKG_CONFIG_PATH=/opt/cross/i386-mingw32msvc/lib/pkgconfig
  ifneq ($(call bin_path, pkg-config),)
    CURL_CFLAGS=$(shell pkg-config --silence-errors --cflags libcurl)
    CURL_LIBS=$(shell pkg-config --silence-errors --libs libcurl)
    OPENAL_CFLAGS=$(shell pkg-config --silence-errors --cflags openal)
    OPENAL_LIBS=$(shell pkg-config --silence-errors --libs openal)
    SDL_CFLAGS=$(shell pkg-config --silence-errors --cflags sdl|sed 's/-Dmain=SDL_main//')
    SDL_LIBS=$(shell pkg-config --silence-errors --libs sdl)
    FREETYPE_CFLAGS=$(shell pkg-config --silence-errors --cflags freetype2)
  endif
  # Use sdl-config if all else fails
  ifeq ($(SDL_CFLAGS),)
    ifneq ($(call bin_path, sdl-config),)
      SDL_CFLAGS=$(shell sdl-config --cflags)
      SDL_LIBS=$(shell sdl-config --libs)
    endif
  endif
endif

ifeq ($(USE_INTERNAL_LLVM),1)
  LLVM_CXXFLAGS=-D__STDC_LIMIT_MACROS -D__STDC_CONSTANT_MACROS
  LLVM_LIBS=
else
  ifneq ($(call bin_path, llvm-config),)
    ifeq ($(LLVM_CXXFLAGS),)
      LLVM_CXXFLAGS=$(shell llvm-config --cxxflags)
    endif
    ifeq ($(LLVM_LIBS),)
      LLVM_LIBS=$(shell llvm-config --libs)
    endif
  endif
endif

ifneq ($(BUILD_FINAL),1)
	# Add svn version info
	USE_SVN=
	ifeq ($(wildcard .svn),.svn)
	  SVN_REV=$(shell LANG=C svnversion .)
	  ifneq ($(SVN_REV),)
		VERSION:=$(VERSION)_SVN$(SVN_REV)
		USE_SVN=1
	  endif
	else
	ifeq ($(wildcard .git/svn/.metadata),.git/svn/.metadata)
	  SVN_REV=$(shell LANG=C git svn info | awk '$$1 == "Revision:" {print $$2; exit 0}')
	  ifneq ($(SVN_REV),)
		VERSION:=$(VERSION)_SVN$(SVN_REV)
	  endif
	endif
	endif

	ifdef IOQ3_REVISION
	  VERSION:=$(VERSION)_IOQ3r$(IOQ3_REVISION)
	endif
endif


#############################################################################
# SETUP AND BUILD -- LINUX
#############################################################################

## Defaults
LIB=lib

INSTALL=install
MKDIR=mkdir

ifeq ($(USE_LLVM),1)
  LD=$(CXX)
else
  LD=$(CC)
endif

ifneq (,$(findstring "$(PLATFORM)", "linux" "gnu_kfreebsd" "kfreebsd-gnu"))

  ifeq ($(ARCH),axp)
    ARCH=alpha
  else
  ifeq ($(ARCH),x86_64)
    LIB=lib64
  else
  ifeq ($(ARCH),ppc64)
    LIB=lib64
  else
  ifeq ($(ARCH),s390x)
    LIB=lib64
  endif
  endif
  endif
  endif

  BASE_CFLAGS = -Wall -fno-strict-aliasing -Wimplicit -Wstrict-prototypes \
    -pipe -DUSE_ICON
  CLIENT_CFLAGS += $(SDL_CFLAGS)

  OPTIMIZEVM = -O3 -funroll-loops -fomit-frame-pointer
  OPTIMIZE = $(OPTIMIZEVM) -ffast-math

  ifeq ($(ARCH),x86_64)
    OPTIMIZEVM = -O3 -fomit-frame-pointer -funroll-loops \
      -falign-loops=2 -falign-jumps=2 -falign-functions=2 \
      -fstrength-reduce
    OPTIMIZE = $(OPTIMIZEVM) -ffast-math
    HAVE_VM_COMPILED = true
  else
  ifeq ($(ARCH),i386)
    OPTIMIZEVM = -O3 -march=i586 -fomit-frame-pointer \
      -funroll-loops -falign-loops=2 -falign-jumps=2 \
      -falign-functions=2 -fstrength-reduce
    OPTIMIZE = $(OPTIMIZEVM) -ffast-math
    HAVE_VM_COMPILED=true
  else
  ifeq ($(ARCH),ppc)
    BASE_CFLAGS += -maltivec
    HAVE_VM_COMPILED=true
  endif
  ifeq ($(ARCH),ppc64)
    BASE_CFLAGS += -maltivec
    HAVE_VM_COMPILED=true
  endif
  ifeq ($(ARCH),sparc)
    OPTIMIZE += -mtune=ultrasparc3 -mv8plus
    OPTIMIZEVM += -mtune=ultrasparc3 -mv8plus
    HAVE_VM_COMPILED=true
  endif
  ifeq ($(ARCH),alpha)
    # According to http://bugs.debian.org/cgi-bin/bugreport.cgi?bug=410555
    # -ffast-math will cause the client to die with SIGFPE on Alpha
    OPTIMIZE = $(OPTIMIZEVM)
  endif
  endif
  endif

  SHLIBEXT=so
  SHLIBCFLAGS=-fPIC -fvisibility=hidden
  SHLIBLDFLAGS=-shared $(LDFLAGS)

  THREAD_LIBS=-lpthread
  ifeq ($(USE_LLVM),1)
    LIBS=$(LLVM_LIBS) $(THREAD_LIBS) -ldl -lm
  else
    LIBS=-ldl -lm
  endif

  CLIENT_LIBS=$(SDL_LIBS)
  RENDERER_LIBS = $(SDL_LIBS) -lGL

  ifeq ($(USE_OPENAL),1)
    ifneq ($(USE_OPENAL_DLOPEN),1)
      CLIENT_LIBS += -lopenal
    endif
  endif

  ifeq ($(USE_CURL),1)
    ifneq ($(USE_CURL_DLOPEN),1)
      CLIENT_LIBS += -lcurl
    endif
  endif

  ifeq ($(USE_CODEC_VORBIS),1)
    CLIENT_LIBS += -lvorbisfile -lvorbis -logg
  endif

  ifeq ($(USE_MUMBLE),1)
    CLIENT_LIBS += -lrt
  endif

  ifeq ($(USE_FREETYPE),1)
    ifneq ($(USE_INTERNAL_FREETYPE), 1)
      BASE_CFLAGS += $(FREETYPE_CFLAGS)
    endif
  endif

  ifeq ($(ARCH),i386)
    # linux32 make ...
    BASE_CFLAGS += -m32
  else
  ifeq ($(ARCH),ppc64)
    BASE_CFLAGS += -m64
  endif
  endif
else # ifeq Linux

#############################################################################
# SETUP AND BUILD -- MAC OS X
#############################################################################

ifeq ($(PLATFORM),darwin)
  HAVE_VM_COMPILED=true
  LIBS = -framework Cocoa
  CLIENT_LIBS=
  RENDERER_LIBS=
  OPTIMIZEVM=
  
  BASE_CFLAGS = -Wall -Wimplicit -Wstrict-prototypes

  ifeq ($(ARCH),ppc)
    BASE_CFLAGS += -arch ppc -faltivec -mmacosx-version-min=10.2
    OPTIMIZEVM += -O3
  endif
  ifeq ($(ARCH),ppc64)
    BASE_CFLAGS += -arch ppc64 -faltivec -mmacosx-version-min=10.2
  endif
  ifeq ($(ARCH),i386)
    OPTIMIZEVM += -march=prescott -mfpmath=sse
    # x86 vm will crash without -mstackrealign since MMX instructions will be
    # used no matter what and they corrupt the frame pointer in VM calls
    BASE_CFLAGS += -arch i386 -m32 -mstackrealign
  endif
  ifeq ($(ARCH),x86_64)
    OPTIMIZEVM += -arch x86_64 -mfpmath=sse
  endif

  BASE_CFLAGS += -fno-strict-aliasing -DMACOS_X -fno-common -pipe

  ifeq ($(USE_OPENAL),1)
    ifneq ($(USE_OPENAL_DLOPEN),1)
      CLIENT_LIBS += -framework OpenAL
    endif
  endif

  ifeq ($(USE_CURL),1)
    ifneq ($(USE_CURL_DLOPEN),1)
      CLIENT_LIBS += -lcurl
    endif
  endif

  ifeq ($(USE_FREETYPE),1)
    ifneq ($(USE_INTERNAL_FREETYPE), 1)
      BASE_CFLAGS += $(FREETYPE_CFLAGS)
    endif
  endif

  ifeq ($(USE_CODEC_VORBIS),1)
    CLIENT_LIBS += -lvorbisfile -lvorbis -logg
  endif

  BASE_CFLAGS += -D_THREAD_SAFE=1

  ifeq ($(USE_LOCAL_HEADERS),1)
    BASE_CFLAGS += -I$(SDLHDIR)/include
  endif

  # We copy sdlmain before ranlib'ing it so that subversion doesn't think
  #  the file has been modified by each build.
  LIBSDLMAIN=$(B)/libSDLmain.a
  LIBSDLMAINSRC=$(LIBSDIR)/macosx/libSDLmain.a
  CLIENT_LIBS += -framework IOKit \
    $(LIBSDIR)/macosx/libSDL-1.2.0.dylib
  RENDERER_LIBS += -framework OpenGL $(LIBSDIR)/macosx/libSDL-1.2.0.dylib

  OPTIMIZEVM += -falign-loops=16
  OPTIMIZE = $(OPTIMIZEVM) -ffast-math

  SHLIBEXT=dylib
  SHLIBCFLAGS=-fPIC -fno-common
  SHLIBLDFLAGS=-dynamiclib $(LDFLAGS) -Wl,-U,_com_altivec

  NOTSHLIBCFLAGS=-mdynamic-no-pic

  TOOLS_CFLAGS += -DMACOS_X

else # ifeq darwin


#############################################################################
# SETUP AND BUILD -- MINGW32
#############################################################################

ifeq ($(PLATFORM),mingw32)

  # Some MinGW installations define CC to cc, but don't actually provide cc,
  # so explicitly use gcc instead (which is the only option anyway)
  ifeq ($(call bin_path, $(CC)),)
    CC=gcc
  endif
  ifeq ($(call bin_path, $(CXX)),)
    CXX=g++
  endif

  ifndef WINDRES
    WINDRES=windres
  endif

  BASE_CFLAGS = -Wall -fno-strict-aliasing -Wimplicit -Wstrict-prototypes \
    -DUSE_ICON

  # In the absence of wspiapi.h, require Windows XP or later
  ifeq ($(shell test -e $(CMDIR)/wspiapi.h; echo $$?),1)
    BASE_CFLAGS += -DWINVER=0x501
  endif

  ifeq ($(USE_OPENAL),1)
    CLIENT_CFLAGS += $(OPENAL_CFLAGS)
    ifneq ($(USE_OPENAL_DLOPEN),1)
      CLIENT_LDFLAGS += $(OPENAL_LDFLAGS)
    endif
  endif
  
  ifeq ($(ARCH),x64)
    OPTIMIZEVM = -O3 -fno-omit-frame-pointer \
      -falign-loops=2 -funroll-loops -falign-jumps=2 -falign-functions=2 \
      -fstrength-reduce
    OPTIMIZE = $(OPTIMIZEVM) --fast-math
    HAVE_VM_COMPILED = true
  endif
  ifeq ($(ARCH),x86)
    OPTIMIZEVM = -O3 -march=i586 -fno-omit-frame-pointer \
      -falign-loops=2 -funroll-loops -falign-jumps=2 -falign-functions=2 \
      -fstrength-reduce
    OPTIMIZE = $(OPTIMIZEVM) -ffast-math
    HAVE_VM_COMPILED = true
  endif

  SHLIBEXT=dll
  SHLIBCFLAGS=
  SHLIBLDFLAGS=-shared $(LDFLAGS)

  BINEXT=.exe

  LIBS= -lws2_32 -lwinmm -lpsapi
  CLIENT_LDFLAGS += -mwindows
  CLIENT_LIBS = -lgdi32 -lole32
  RENDERER_LIBS = -lgdi32 -lole32 -lopengl32

  ifeq ($(USE_FREETYPE),1)
    ifneq ($(USE_INTERNAL_FREETYPE),1)
      BASE_CFLAGS += -Ifreetype2
    endif
  endif

  ifeq ($(ARCH),x64)
    WINLIBDIR=$(LIBSDIR)/win64
  else
    WINLIBDIR=$(LIBSDIR)/win32
  endif

  ifeq ($(USE_CURL),1)
    CLIENT_CFLAGS += $(CURL_CFLAGS)
    ifneq ($(USE_CURL_DLOPEN),1)
      ifeq ($(USE_LOCAL_HEADERS),1)
        CLIENT_CFLAGS += -DCURL_STATICLIB
        CLIENT_LIBS += $(WINLIBDIR)/libcurl.a
      else
        CLIENT_LIBS += $(CURL_LIBS)
      endif
    endif
  endif

  ifeq ($(ARCH),x86)
    # build 32bit
    BASE_CFLAGS += -m32
  else
    BASE_CFLAGS += -m64
  endif

  # libmingw32 must be linked before libSDLmain
  CLIENT_LIBS += -lmingw32
  RENDERER_LIBS += -lmingw32
  
  ifeq ($(USE_LOCAL_HEADERS),1)
    CLIENT_CFLAGS += -I$(SDLHDIR)/include
    CLIENT_LIBS += $(WINLIBDIR)/libSDLmain.a
    ifeq ($(ARCH),x64)
      CLIENT_LIBS += $(WINLIBDIR)/libSDL64.dll.a
    else
      CLIENT_LIBS += $(WINLIBDIR)/libSDL.dll.a
    endif

    RENDERER_LIBS += $(WINLIBDIR)/libSDLmain.a
    ifeq ($(ARCH),x64)
      RENDERER_LIBS += $(WINLIBDIR)/libSDL64.dll.a
      SDLDLL=SDL64.dll
    else
      RENDERER_LIBS += $(WINLIBDIR)/libSDL.dll.a
      SDLDLL=SDL.dll
    endif

    ifeq ($(USE_CODEC_VORBIS),1)
      CLIENT_CFLAGS += -I$(OGGDIR)/include \
                      -I$(VORBISDIR)/include
      CLIENT_LIBS += $(WINLIBDIR)/libvorbisfile.a \
                      $(WINLIBDIR)/libvorbis.a \
                      $(WINLIBDIR)/libogg.a
    endif
  else
    CLIENT_CFLAGS += $(SDL_CFLAGS)
    CLIENT_LIBS += $(SDL_LIBS)
    RENDERER_LIBS += $(SDL_LIBS)
    SDLDLL=SDL.dll

    ifeq ($(USE_CODEC_VORBIS),1)
      CLIENT_LIBS += -lvorbisfile -lvorbis -logg
    endif
  endif

  BUILD_CLIENT_SMP = 0

else # ifeq mingw32

#############################################################################
# SETUP AND BUILD -- FREEBSD
#############################################################################

ifeq ($(PLATFORM),freebsd)

  # flags
  BASE_CFLAGS = $(shell env MACHINE_ARCH=$(ARCH) make -f /dev/null -VCFLAGS) \
    -Wall -fno-strict-aliasing -Wimplicit -Wstrict-prototypes \
    -DUSE_ICON -DMAP_ANONYMOUS=MAP_ANON
  CLIENT_CFLAGS += $(SDL_CFLAGS)
  HAVE_VM_COMPILED = true

  OPTIMIZEVM = -O3 -funroll-loops -fomit-frame-pointer
  OPTIMIZE = $(OPTIMIZEVM) -ffast-math

  SHLIBEXT=so
  SHLIBCFLAGS=-fPIC
  SHLIBLDFLAGS=-shared $(LDFLAGS)

  THREAD_LIBS=-lpthread
  # don't need -ldl (FreeBSD)
  LIBS=-lm

  CLIENT_LIBS =

  CLIENT_LIBS += $(SDL_LIBS)
  RENDERER_LIBS = $(SDL_LIBS) -lGL

  # optional features/libraries
  ifeq ($(USE_OPENAL),1)
    ifeq ($(USE_OPENAL_DLOPEN),1)
      CLIENT_LIBS += $(THREAD_LIBS) -lopenal
    endif
  endif

  ifeq ($(USE_CURL),1)
    ifeq ($(USE_CURL_DLOPEN),1)
      CLIENT_LIBS += -lcurl
    endif
  endif

  ifeq ($(USE_CODEC_VORBIS),1)
    CLIENT_LIBS += -lvorbisfile -lvorbis -logg
  endif

  # cross-compiling tweaks
  ifeq ($(ARCH),i386)
    ifeq ($(CROSS_COMPILING),1)
      BASE_CFLAGS += -m32
    endif
  endif
  ifeq ($(ARCH),amd64)
    ifeq ($(CROSS_COMPILING),1)
      BASE_CFLAGS += -m64
    endif
  endif

else # ifeq freebsd

#############################################################################
# SETUP AND BUILD -- OPENBSD
#############################################################################

ifeq ($(PLATFORM),openbsd)

  ARCH=$(shell uname -m)

  BASE_CFLAGS = -Wall -fno-strict-aliasing -Wimplicit -Wstrict-prototypes \
    -DUSE_ICON -DMAP_ANONYMOUS=MAP_ANON
  CLIENT_CFLAGS += $(SDL_CFLAGS)

  ifeq ($(USE_CURL),1)
    CLIENT_CFLAGS += $(CURL_CFLAGS)
    USE_CURL_DLOPEN=0
  endif

  SHLIBEXT=so
  SHLIBNAME=.$(SHLIBEXT)
  SHLIBCFLAGS=-fPIC
  SHLIBLDFLAGS=-shared $(LDFLAGS)

  THREAD_LIBS=-pthread
  LIBS=-lm

  CLIENT_LIBS =

  CLIENT_LIBS += $(SDL_LIBS)
  RENDERER_LIBS = $(SDL_LIBS) -lGL

  ifeq ($(USE_OPENAL),1)
    ifneq ($(USE_OPENAL_DLOPEN),1)
      CLIENT_LIBS += $(THREAD_LIBS) -lossaudio -lopenal
    endif
  endif

  ifeq ($(USE_CODEC_VORBIS),1)
    CLIENT_LIBS += -lvorbisfile -lvorbis -logg
  endif

  ifeq ($(USE_CURL),1) 
    ifneq ($(USE_CURL_DLOPEN),1)
      CLIENT_LIBS += -lcurl
    endif
  endif

else # ifeq openbsd

#############################################################################
# SETUP AND BUILD -- NETBSD
#############################################################################

ifeq ($(PLATFORM),netbsd)

  ifeq ($(shell uname -m),i386)
    ARCH=i386
  endif

  LIBS=-lm
  SHLIBEXT=so
  SHLIBCFLAGS=-fPIC
  SHLIBLDFLAGS=-shared $(LDFLAGS)
  THREAD_LIBS=-lpthread

  BASE_CFLAGS = -Wall -fno-strict-aliasing -Wimplicit -Wstrict-prototypes

  ifeq ($(ARCH),i386)
    HAVE_VM_COMPILED=true
  endif

  BUILD_CLIENT = 0

else # ifeq netbsd

#############################################################################
# SETUP AND BUILD -- IRIX
#############################################################################

ifeq ($(PLATFORM),irix64)

  ARCH=mips

  CC = c99
  MKDIR = mkdir -p

  BASE_CFLAGS=-Dstricmp=strcasecmp -Xcpluscomm -woff 1185 \
    -I. -I$(ROOT)/usr/include
  CLIENT_CFLAGS += $(SDL_CFLAGS)
  OPTIMIZE = -O3
  
  SHLIBEXT=so
  SHLIBCFLAGS=
  SHLIBLDFLAGS=-shared

  LIBS=-ldl -lm -lgen
  # FIXME: The X libraries probably aren't necessary?
  CLIENT_LIBS=-L/usr/X11/$(LIB) $(SDL_LIBS) \
    -lX11 -lXext -lm
  RENDERER_LIBS = $(SDL_LIBS) -lGL

else # ifeq IRIX

#############################################################################
# SETUP AND BUILD -- SunOS
#############################################################################

ifeq ($(PLATFORM),sunos)

  CC=gcc
  INSTALL=ginstall
  MKDIR=gmkdir
  COPYDIR="/usr/local/share/games/quake3"

  ifneq (,$(findstring i86pc,$(shell uname -m)))
    ARCH=i386
  else #default to sparc
    ARCH=sparc
  endif

  ifneq ($(ARCH),i386)
    ifneq ($(ARCH),sparc)
      $(error arch $(ARCH) is currently not supported)
    endif
  endif

  BASE_CFLAGS = -Wall -fno-strict-aliasing -Wimplicit -Wstrict-prototypes \
    -pipe -DUSE_ICON
  CLIENT_CFLAGS += $(SDL_CFLAGS)

  OPTIMIZEVM = -O3 -funroll-loops

  ifeq ($(ARCH),sparc)
    OPTIMIZEVM += -O3 \
      -fstrength-reduce -falign-functions=2 \
      -mtune=ultrasparc3 -mv8plus -mno-faster-structs
    HAVE_VM_COMPILED=true
  else
  ifeq ($(ARCH),i386)
    OPTIMIZEVM += -march=i586 -fomit-frame-pointer \
      -falign-loops=2 -falign-jumps=2 \
      -falign-functions=2 -fstrength-reduce
    HAVE_VM_COMPILED=true
    BASE_CFLAGS += -m32
    CLIENT_CFLAGS += -I/usr/X11/include/NVIDIA
    CLIENT_LDFLAGS += -L/usr/X11/lib/NVIDIA -R/usr/X11/lib/NVIDIA
  endif
  endif
  
  OPTIMIZE = $(OPTIMIZEVM) -ffast-math

  SHLIBEXT=so
  SHLIBCFLAGS=-fPIC
  SHLIBLDFLAGS=-shared $(LDFLAGS)

  THREAD_LIBS=-lpthread
  LIBS=-lsocket -lnsl -ldl -lm

  BOTCFLAGS=-O0

  CLIENT_LIBS +=$(SDL_LIBS) -lX11 -lXext -liconv -lm
  RENDERER_LIBS = $(SDL_LIBS) -lGL

else # ifeq sunos

#############################################################################
# SETUP AND BUILD -- GENERIC
#############################################################################
  BASE_CFLAGS=
  OPTIMIZE = -O3

  SHLIBEXT=so
  SHLIBCFLAGS=-fPIC
  SHLIBLDFLAGS=-shared

endif #Linux
endif #darwin
endif #mingw32
endif #FreeBSD
endif #OpenBSD
endif #NetBSD
endif #IRIX
endif #SunOS

ifneq ($(HAVE_VM_COMPILED),true)
  BASE_CFLAGS += -DNO_VM_COMPILED
  BUILD_GAME_QVM=0
endif

TARGETS =

ifeq ($(USE_FREETYPE),1)
  BASE_CFLAGS += -DBUILD_FREETYPE
endif

ifndef FULLBINEXT
  FULLBINEXT=.$(ARCH)$(BINEXT)
endif

ifndef SHLIBNAME
  SHLIBNAME=$(ARCH).$(SHLIBEXT)
endif

ifneq ($(BUILD_SERVER),0)
  TARGETS += $(B)/$(SERVERBIN)$(FULLBINEXT)
endif

ifneq ($(BUILD_CLIENT),0)
  ifneq ($(USE_RENDERER_DLOPEN),0)
    TARGETS += $(B)/$(CLIENTBIN)$(FULLBINEXT) $(B)/renderer_opengl1_$(SHLIBNAME)
    ifneq ($(BUILD_CLIENT_SMP),0)
      TARGETS += $(B)/renderer_opengl1_smp_$(SHLIBNAME)
    endif
  else
    TARGETS += $(B)/$(CLIENTBIN)$(FULLBINEXT)
    ifneq ($(BUILD_CLIENT_SMP),0)
      TARGETS += $(B)/$(CLIENTBIN)-smp$(FULLBINEXT)
    endif
  endif
endif

ifneq ($(BUILD_GAME_SO),0)
  ifneq ($(BUILD_BASEGAME),0)
    TARGETS += \
	$(B)/$(BASEGAME)/cgame$(SHLIBNAME) \
	$(B)/$(BASEGAME)/qagame$(SHLIBNAME) \
	$(B)/$(BASEGAME)/ui$(SHLIBNAME)
  endif
  ifneq ($(BUILD_MISSIONPACK),0)
    TARGETS += \
    $(B)/$(MISSIONPACK)/cgame$(SHLIBNAME) \
    $(B)/$(MISSIONPACK)/qagame$(SHLIBNAME) \
    $(B)/$(MISSIONPACK)/ui$(SHLIBNAME)
  endif
endif

ifneq ($(BUILD_GAME_QVM),0)
  ifneq ($(CROSS_COMPILING),1)
    ifneq ($(BUILD_BASEGAME),0)
      TARGETS += \
      $(B)/$(BASEGAME)/vm/cgame.qvm \
      $(B)/$(BASEGAME)/vm/qagame.qvm \
      $(B)/$(BASEGAME)/vm/ui.qvm
	endif
    ifneq ($(BUILD_MISSIONPACK),0)
      TARGETS += \
      $(B)/$(MISSIONPACK)/vm/qagame.qvm \
      $(B)/$(MISSIONPACK)/vm/cgame.qvm \
      $(B)/$(MISSIONPACK)/vm/ui.qvm
    endif
  endif
endif

ifneq ($(BUILD_GAME_LLVM),0)
  ifneq ($(CROSS_COMPILING),1)
    ifneq ($(BUILD_BASEGAME),0)
      TARGETS += \
      $(B)/$(BASEGAME)/cgamellvm.bc \
      $(B)/$(BASEGAME)/qagamellvm.bc \
      $(B)/$(BASEGAME)/uillvm.bc
    endif
    ifneq ($(BUILD_MISSIONPACK),0)
      TARGETS += \
      $(B)/$(MISSIONPACK)/cgamellvm.bc \
      $(B)/$(MISSIONPACK)/qagamellvm.bc \
      $(B)/$(MISSIONPACK)/uillvm.bc
    endif
  endif
endif

ifneq ($(USE_QVM),1)
  HAVE_VM_COMPILED = false
endif

ifneq ($(HAVE_VM_COMPILED),true)
  BASE_CFLAGS += -DNO_VM_COMPILED
endif

ifeq ($(USE_LLVM),1)
  BASE_CFLAGS += -DUSE_LLVM
endif

ifeq ($(USE_OPENAL),1)
  CLIENT_CFLAGS += -DUSE_OPENAL
  ifeq ($(USE_OPENAL_DLOPEN),1)
    CLIENT_CFLAGS += -DUSE_OPENAL_DLOPEN
  endif
endif

ifeq ($(USE_CURL),1)
  CLIENT_CFLAGS += -DUSE_CURL
  ifeq ($(USE_CURL_DLOPEN),1)
    CLIENT_CFLAGS += -DUSE_CURL_DLOPEN
  endif
endif

ifeq ($(USE_CODEC_VORBIS),1)
  CLIENT_CFLAGS += -DUSE_CODEC_VORBIS
endif

ifeq ($(USE_RENDERER_DLOPEN),1)
  CLIENT_CFLAGS += -DUSE_RENDERER_DLOPEN
endif

ifeq ($(USE_MUMBLE),1)
  CLIENT_CFLAGS += -DUSE_MUMBLE
endif

ifeq ($(USE_VOIP),1)
  CLIENT_CFLAGS += -DUSE_VOIP
  SERVER_CFLAGS += -DUSE_VOIP
  ifeq ($(USE_INTERNAL_SPEEX),1)
    CLIENT_CFLAGS += -DFLOATING_POINT -DUSE_ALLOCA -I$(SPEEXDIR)/include
  else
    CLIENT_LIBS += -lspeex -lspeexdsp
  endif
endif

ifeq ($(USE_INTERNAL_ZLIB),1)
  BASE_CFLAGS += -DNO_GZIP
  BASE_CFLAGS += -I$(ZDIR)
else
  LIBS += -lz
endif

ifeq ($(USE_INTERNAL_JPEG),1)
  BASE_CFLAGS += -DUSE_INTERNAL_JPEG
  BASE_CFLAGS += -I$(JPDIR)
else
  RENDERER_LIBS += -ljpeg
endif

ifeq ($(USE_FREETYPE),1)
ifeq ($(USE_INTERNAL_FREETYPE),1)
  BASE_CFLAGS += -I$(FTDIR)/include \
					-DFT2_BUILD_LIBRARY
else
  RENDERER_LIBS += -lfreetype
endif
endif

ifeq ($(USE_LLVM),1)
ifeq ($(USE_INTERNAL_LLVM),1)
  #LIBS += -lLLVMsomethings
  BASE_CFLAGS += -Icode/tools/llvm/include
endif
endif

ifeq ("$(CC)", $(findstring "$(CC)", "clang" "clang++"))
  BASE_CFLAGS += -Qunused-arguments
endif

ifdef DEFAULT_BASEDIR
  BASE_CFLAGS += -DDEFAULT_BASEDIR=\\\"$(DEFAULT_BASEDIR)\\\"
endif

ifeq ($(USE_LOCAL_HEADERS),1)
  BASE_CFLAGS += -DUSE_LOCAL_HEADERS
endif

ifeq ($(GENERATE_DEPENDENCIES),1)
  DEPEND_CFLAGS = -MMD
else
  DEPEND_CFLAGS =
endif

ifeq ($(NO_STRIP),1)
  STRIP_FLAG =
else
  STRIP_FLAG = -s
endif

BASE_CFLAGS += -DPRODUCT_VERSION=\\\"$(VERSION)\\\"
BASE_CFLAGS += -Wformat=2 -Wno-format-zero-length -Wformat-security -Wno-format-nonliteral
BASE_CFLAGS += -Wstrict-aliasing=2 -Wmissing-format-attribute
BASE_CFLAGS += -Wdisabled-optimization
BASE_CFLAGS += -Werror-implicit-function-declaration

BASE_CFLAGS += $(BUILD_DEFINES)

ifeq ($(V),1)
echo_cmd=@:
Q=
else
echo_cmd=@echo
Q=@
endif

define DO_CC
$(echo_cmd) "CC $<"
$(Q)$(CC) $(NOTSHLIBCFLAGS) $(CFLAGS) $(CLIENT_CFLAGS) $(OPTIMIZE) -o $@ -c $<
endef

define DO_CXX
$(echo_cmd) "CXX $<"
$(Q)$(CXX) $(NOTSHLIBCFLAGS) $(CFLAGS) $(CLIENT_CFLAGS) $(OPTIMIZE) $(LLVM_CXXFLAGS) -o $@ -c $<
endef

define DO_REF_CC
$(echo_cmd) "REF_CC $<"
$(Q)$(CC) $(SHLIBCFLAGS) $(CFLAGS) $(CLIENT_CFLAGS) $(OPTIMIZE) -o $@ -c $<
endef

define DO_SMP_CC
$(echo_cmd) "SMP_CC $<"
$(Q)$(CC) $(SHLIBCFLAGS) $(CFLAGS) $(CLIENT_CFLAGS) $(OPTIMIZE) -DSMP -o $@ -c $<
endef

define DO_BOT_CC
$(echo_cmd) "BOT_CC $<"
$(Q)$(CC) $(NOTSHLIBCFLAGS) $(CFLAGS) $(BOTCFLAGS) $(OPTIMIZE) -DBOTLIB -o $@ -c $<
endef

ifeq ($(GENERATE_DEPENDENCIES),1)
  DO_QVM_DEP=cat $(@:%.o=%.d) | sed -e 's/\.o/\.asm/g' >> $(@:%.o=%.d)
endif

define DO_SHLIB_CC
$(echo_cmd) "SHLIB_CC $<"
$(Q)$(CC) $(BASEGAME_CFLAGS) $(SHLIBCFLAGS) $(CFLAGS) $(OPTIMIZEVM) -o $@ -c $<
$(Q)$(DO_QVM_DEP)
endef

define DO_GAME_CC
$(echo_cmd) "GAME_CC $<"
$(Q)$(CC) $(BASEGAME_CFLAGS) -DQAGAME $(SHLIBCFLAGS) $(CFLAGS) $(OPTIMIZEVM) -o $@ -c $<
$(Q)$(DO_QVM_DEP)
endef

define DO_CGAME_CC
$(echo_cmd) "CGAME_CC $<"
$(Q)$(CC) $(BASEGAME_CFLAGS) -DCGAME $(SHLIBCFLAGS) $(CFLAGS) $(OPTIMIZEVM) -o $@ -c $<
$(Q)$(DO_QVM_DEP)
endef

define DO_UI_CC
$(echo_cmd) "UI_CC $<"
$(Q)$(CC) $(BASEGAME_CFLAGS) -DUI $(SHLIBCFLAGS) $(CFLAGS) $(OPTIMIZEVM) -o $@ -c $<
$(Q)$(DO_QVM_DEP)
endef

define DO_SHLIB_CC_MISSIONPACK
$(echo_cmd) "SHLIB_CC_MISSIONPACK $<"
$(Q)$(CC) $(MISSIONPACK_CFLAGS) $(SHLIBCFLAGS) $(CFLAGS) $(OPTIMIZEVM) -o $@ -c $<
$(Q)$(DO_QVM_DEP)
endef

define DO_GAME_CC_MISSIONPACK
$(echo_cmd) "GAME_CC_MISSIONPACK $<"
$(Q)$(CC) $(MISSIONPACK_CFLAGS) -DQAGAME $(SHLIBCFLAGS) $(CFLAGS) $(OPTIMIZEVM) -o $@ -c $<
$(Q)$(DO_QVM_DEP)
endef

define DO_CGAME_CC_MISSIONPACK
$(echo_cmd) "CGAME_CC_MISSIONPACK $<"
$(Q)$(CC) $(MISSIONPACK_CFLAGS) -DCGAME $(SHLIBCFLAGS) $(CFLAGS) $(OPTIMIZEVM) -o $@ -c $<
$(Q)$(DO_QVM_DEP)
endef

define DO_UI_CC_MISSIONPACK
$(echo_cmd) "UI_CC_MISSIONPACK $<"
$(Q)$(CC) $(MISSIONPACK_CFLAGS) -DUI $(SHLIBCFLAGS) $(CFLAGS) $(OPTIMIZEVM) -o $@ -c $<
$(Q)$(DO_QVM_DEP)
endef

define DO_AS
$(echo_cmd) "AS $<"
$(Q)$(CC) $(CFLAGS) $(OPTIMIZE) -x assembler-with-cpp -o $@ -c $<
endef

define DO_DED_CC
$(echo_cmd) "DED_CC $<"
$(Q)$(CC) $(NOTSHLIBCFLAGS) -DDEDICATED $(CFLAGS) $(SERVER_CFLAGS) $(OPTIMIZE) -o $@ -c $<
endef

define DO_DED_CXX
$(echo_cmd) "DED_CXX $<"
$(Q)$(CXX) $(NOTSHLIBCFLAGS) -DDEDICATED $(CFLAGS) $(SERVER_CFLAGS) $(OPTIMIZE) $(LLVM_CXXFLAGS) -o $@ -c $<
endef

define DO_WINDRES
$(echo_cmd) "WINDRES $<"
$(Q)$(WINDRES) -i $< -o $@
endef


#############################################################################
# MAIN TARGETS
#############################################################################

default: release
all: debug release

debug:
	@$(MAKE) targets B=$(BD) CFLAGS="$(CFLAGS) $(BASE_CFLAGS) $(DEPEND_CFLAGS)" \
	  OPTIMIZE="$(DEBUG_CFLAGS)" OPTIMIZEVM="$(DEBUG_CFLAGS)" \
	  CLIENT_CFLAGS="$(CLIENT_CFLAGS)" SERVER_CFLAGS="$(SERVER_CFLAGS)" V=$(V)

release:
	@$(MAKE) targets B=$(BR) CFLAGS="$(CFLAGS) $(BASE_CFLAGS) $(DEPEND_CFLAGS)" \
	  OPTIMIZE="-DNDEBUG $(OPTIMIZE)" OPTIMIZEVM="-DNDEBUG $(OPTIMIZEVM)" \
	  CLIENT_CFLAGS="$(CLIENT_CFLAGS)" SERVER_CFLAGS="$(SERVER_CFLAGS)" V=$(V)

# Create the build directories, check libraries and print out
# an informational message, then start building
targets: makedirs
	@echo ""
	@echo "Building $(CLIENTBIN) in $(B):"
	@echo "  IOQ3_REVISION: $(IOQ3_REVISION)"
	@echo "  PLATFORM: $(PLATFORM)"
	@echo "  ARCH: $(ARCH)"
	@echo "  VERSION: $(VERSION)"
	@echo "  COMPILE_PLATFORM: $(COMPILE_PLATFORM)"
	@echo "  COMPILE_ARCH: $(COMPILE_ARCH)"
	@echo "  CC: $(CC)"
	@echo "  CXX: $(CXX)"
	@echo "  LD: $(LD)"
	@echo ""
	@echo "  CFLAGS:"
	-@for i in $(CFLAGS); \
	do \
		echo "    $$i"; \
	done
	-@for i in $(OPTIMIZE); \
	do \
		echo "    $$i"; \
	done
	@echo ""
	@echo "  CLIENT_CFLAGS:"
	-@for i in $(CLIENT_CFLAGS); \
	do \
		echo "    $$i"; \
	done
	@echo ""
	@echo "  SERVER_CFLAGS:"
	-@for i in $(SERVER_CFLAGS); \
	do \
		echo "    $$i"; \
	done
	@echo ""
	@echo "  LDFLAGS:"
	-@for i in $(LDFLAGS); \
	do \
		echo "    $$i"; \
	done
	@echo ""
	@echo "  LIBS:"
	-@for i in $(LIBS); \
	do \
		echo "    $$i"; \
	done
	@echo ""
	@echo "  CLIENT_LIBS:"
	-@for i in $(CLIENT_LIBS); \
	do \
		echo "    $$i"; \
	done
	@echo ""
	@echo "  Output:"
	-@for i in $(TARGETS); \
	do \
		echo "    $$i"; \
	done
	@echo ""
ifneq ($(TARGETS),)
	@$(MAKE) $(TARGETS) V=$(V)
endif

makedirs:
	@if [ ! -d $(BUILD_DIR) ];then $(MKDIR) $(BUILD_DIR);fi
	@if [ ! -d $(B) ];then $(MKDIR) $(B);fi
	@if [ ! -d $(B)/client ];then $(MKDIR) $(B)/client;fi
	@if [ ! -d $(B)/renderer ];then $(MKDIR) $(B)/renderer;fi
	@if [ ! -d $(B)/renderersmp ];then $(MKDIR) $(B)/renderersmp;fi
	@if [ ! -d $(B)/ded ];then $(MKDIR) $(B)/ded;fi
	@if [ ! -d $(B)/$(BASEGAME) ];then $(MKDIR) $(B)/$(BASEGAME);fi
	@if [ ! -d $(B)/$(BASEGAME)/cgame ];then $(MKDIR) $(B)/$(BASEGAME)/cgame;fi
	@if [ ! -d $(B)/$(BASEGAME)/game ];then $(MKDIR) $(B)/$(BASEGAME)/game;fi
	@if [ ! -d $(B)/$(BASEGAME)/ui ];then $(MKDIR) $(B)/$(BASEGAME)/ui;fi
	@if [ ! -d $(B)/$(BASEGAME)/qcommon ];then $(MKDIR) $(B)/$(BASEGAME)/qcommon;fi
	@if [ ! -d $(B)/$(BASEGAME)/vm ];then $(MKDIR) $(B)/$(BASEGAME)/vm;fi
	@if [ ! -d $(B)/$(MISSIONPACK) ];then $(MKDIR) $(B)/$(MISSIONPACK);fi
	@if [ ! -d $(B)/$(MISSIONPACK)/cgame ];then $(MKDIR) $(B)/$(MISSIONPACK)/cgame;fi
	@if [ ! -d $(B)/$(MISSIONPACK)/game ];then $(MKDIR) $(B)/$(MISSIONPACK)/game;fi
	@if [ ! -d $(B)/$(MISSIONPACK)/ui ];then $(MKDIR) $(B)/$(MISSIONPACK)/ui;fi
	@if [ ! -d $(B)/$(MISSIONPACK)/qcommon ];then $(MKDIR) $(B)/$(MISSIONPACK)/qcommon;fi
	@if [ ! -d $(B)/$(MISSIONPACK)/vm ];then $(MKDIR) $(B)/$(MISSIONPACK)/vm;fi
	@if [ ! -d $(B)/tools ];then $(MKDIR) $(B)/tools;fi
ifeq ($(USE_QVM),1)
	@if [ ! -d $(B)/tools/asm ];then $(MKDIR) $(B)/tools/asm;fi
	@if [ ! -d $(B)/tools/etc ];then $(MKDIR) $(B)/tools/etc;fi
	@if [ ! -d $(B)/tools/rcc ];then $(MKDIR) $(B)/tools/rcc;fi
	@if [ ! -d $(B)/tools/cpp ];then $(MKDIR) $(B)/tools/cpp;fi
	@if [ ! -d $(B)/tools/lburg ];then $(MKDIR) $(B)/tools/lburg;fi
endif
ifeq ($(USE_LLVM),1)
	@if [ ! -d $(B)/tools/llvmclang ];then $(MKDIR) $(B)/tools/llvmclang;fi
	@if [ ! -d $(B)/tools/llvmlinker ];then $(MKDIR) $(B)/tools/llvmlinker;fi
	@if [ ! -d $(B)/tools/llvmlibs ];then $(MKDIR) $(B)/tools/llvmlibs;fi
endif


#############################################################################
# QVM BUILD TOOLS
#############################################################################

TOOLS_OPTIMIZE = -g -Wall -fno-strict-aliasing
TOOLS_CFLAGS += $(TOOLS_OPTIMIZE) \
                -DTEMPDIR=\"$(TEMPDIR)\" -DSYSTEM=\"\" \
                -I$(Q3LCCSRCDIR) \
                -I$(LBURGDIR)
TOOLS_LIBS =
TOOLS_LDFLAGS =

ifeq ($(GENERATE_DEPENDENCIES),1)
	TOOLS_CFLAGS += -MMD
endif

define DO_TOOLS_CC
$(echo_cmd) "TOOLS_CC $<"
$(Q)$(CC) $(TOOLS_CFLAGS) -o $@ -c $<
endef

define DO_TOOLS_CC_DAGCHECK
$(echo_cmd) "TOOLS_CC_DAGCHECK $<"
$(Q)$(CC) $(TOOLS_CFLAGS) -Wno-unused -o $@ -c $<
endef

LBURG       = $(B)/tools/lburg/lburg$(BINEXT)
DAGCHECK_C  = $(B)/tools/rcc/dagcheck.c
Q3RCC       = $(B)/tools/q3rcc$(BINEXT)
Q3CPP       = $(B)/tools/q3cpp$(BINEXT)
Q3LCC       = $(B)/tools/q3lcc$(BINEXT)
Q3ASM       = $(B)/tools/q3asm$(BINEXT)

LBURGOBJ= \
	$(B)/tools/lburg/lburg.o \
	$(B)/tools/lburg/gram.o

$(B)/tools/lburg/%.o: $(LBURGDIR)/%.c
	$(DO_TOOLS_CC)

$(LBURG): $(LBURGOBJ)
	$(echo_cmd) "LD $@"
	$(Q)$(CC) $(TOOLS_CFLAGS) $(TOOLS_LDFLAGS) -o $@ $^ $(TOOLS_LIBS)

Q3RCCOBJ = \
  $(B)/tools/rcc/alloc.o \
  $(B)/tools/rcc/bind.o \
  $(B)/tools/rcc/bytecode.o \
  $(B)/tools/rcc/dag.o \
  $(B)/tools/rcc/dagcheck.o \
  $(B)/tools/rcc/decl.o \
  $(B)/tools/rcc/enode.o \
  $(B)/tools/rcc/error.o \
  $(B)/tools/rcc/event.o \
  $(B)/tools/rcc/expr.o \
  $(B)/tools/rcc/gen.o \
  $(B)/tools/rcc/init.o \
  $(B)/tools/rcc/inits.o \
  $(B)/tools/rcc/input.o \
  $(B)/tools/rcc/lex.o \
  $(B)/tools/rcc/list.o \
  $(B)/tools/rcc/main.o \
  $(B)/tools/rcc/null.o \
  $(B)/tools/rcc/output.o \
  $(B)/tools/rcc/prof.o \
  $(B)/tools/rcc/profio.o \
  $(B)/tools/rcc/simp.o \
  $(B)/tools/rcc/stmt.o \
  $(B)/tools/rcc/string.o \
  $(B)/tools/rcc/sym.o \
  $(B)/tools/rcc/symbolic.o \
  $(B)/tools/rcc/trace.o \
  $(B)/tools/rcc/tree.o \
  $(B)/tools/rcc/types.o

$(DAGCHECK_C): $(LBURG) $(Q3LCCSRCDIR)/dagcheck.md
	$(echo_cmd) "LBURG $(Q3LCCSRCDIR)/dagcheck.md"
	$(Q)$(LBURG) $(Q3LCCSRCDIR)/dagcheck.md $@

$(B)/tools/rcc/dagcheck.o: $(DAGCHECK_C)
	$(DO_TOOLS_CC_DAGCHECK)

$(B)/tools/rcc/%.o: $(Q3LCCSRCDIR)/%.c
	$(DO_TOOLS_CC)

$(Q3RCC): $(Q3RCCOBJ)
	$(echo_cmd) "LD $@"
	$(Q)$(CC) $(TOOLS_CFLAGS) $(TOOLS_LDFLAGS) -o $@ $^ $(TOOLS_LIBS)

Q3CPPOBJ = \
	$(B)/tools/cpp/cpp.o \
	$(B)/tools/cpp/lex.o \
	$(B)/tools/cpp/nlist.o \
	$(B)/tools/cpp/tokens.o \
	$(B)/tools/cpp/macro.o \
	$(B)/tools/cpp/eval.o \
	$(B)/tools/cpp/include.o \
	$(B)/tools/cpp/hideset.o \
	$(B)/tools/cpp/getopt.o \
	$(B)/tools/cpp/unix.o

$(B)/tools/cpp/%.o: $(Q3CPPDIR)/%.c
	$(DO_TOOLS_CC)

$(Q3CPP): $(Q3CPPOBJ)
	$(echo_cmd) "LD $@"
	$(Q)$(CC) $(TOOLS_CFLAGS) $(TOOLS_LDFLAGS) -o $@ $^ $(TOOLS_LIBS)

Q3LCCOBJ = \
	$(B)/tools/etc/lcc.o \
	$(B)/tools/etc/bytecode.o

$(B)/tools/etc/%.o: $(Q3LCCETCDIR)/%.c
	$(DO_TOOLS_CC)

$(Q3LCC): $(Q3LCCOBJ) $(Q3RCC) $(Q3CPP)
	$(echo_cmd) "LD $@"
	$(Q)$(CC) $(TOOLS_CFLAGS) $(TOOLS_LDFLAGS) -o $@ $(Q3LCCOBJ) $(TOOLS_LIBS)

define DO_Q3LCC
$(echo_cmd) "Q3LCC $<"
$(Q)$(Q3LCC) $(BASEGAME_CFLAGS) $(BUILD_DEFINES) -o $@ $<
endef

define DO_CGAME_Q3LCC
$(echo_cmd) "CGAME_Q3LCC $<"
$(Q)$(Q3LCC) $(BASEGAME_CFLAGS) -DCGAME $(BUILD_DEFINES) -o $@ $<
endef

define DO_GAME_Q3LCC
$(echo_cmd) "GAME_Q3LCC $<"
$(Q)$(Q3LCC) $(BASEGAME_CFLAGS) -DQAGAME $(BUILD_DEFINES) -o $@ $<
endef

define DO_UI_Q3LCC
$(echo_cmd) "UI_Q3LCC $<"
$(Q)$(Q3LCC) $(BASEGAME_CFLAGS) -DUI $(BUILD_DEFINES) -o $@ $<
endef

define DO_Q3LCC_MISSIONPACK
$(echo_cmd) "Q3LCC_MISSIONPACK $<"
$(Q)$(Q3LCC) $(MISSIONPACK_CFLAGS) $(BUILD_DEFINES) -o $@ $<
endef

define DO_CGAME_Q3LCC_MISSIONPACK
$(echo_cmd) "CGAME_Q3LCC_MISSIONPACK $<"
$(Q)$(Q3LCC) $(MISSIONPACK_CFLAGS) -DCGAME $(BUILD_DEFINES) -o $@ $<
endef

define DO_GAME_Q3LCC_MISSIONPACK
$(echo_cmd) "GAME_Q3LCC_MISSIONPACK $<"
$(Q)$(Q3LCC) $(MISSIONPACK_CFLAGS) -DQAGAME $(BUILD_DEFINES) -o $@ $<
endef

define DO_UI_Q3LCC_MISSIONPACK
$(echo_cmd) "UI_Q3LCC_MISSIONPACK $<"
$(Q)$(Q3LCC) $(MISSIONPACK_CFLAGS) -DUI $(BUILD_DEFINES) -o $@ $<
endef


Q3ASMOBJ = \
  $(B)/tools/asm/q3asm.o \
  $(B)/tools/asm/cmdlib.o

$(B)/tools/asm/%.o: $(Q3ASMDIR)/%.c
	$(DO_TOOLS_CC)

$(Q3ASM): $(Q3ASMOBJ)
	$(echo_cmd) "LD $@"
	$(Q)$(CC) $(TOOLS_CFLAGS) $(TOOLS_LDFLAGS) -o $@ $^ $(TOOLS_LIBS)

#############################################################################
# LLVM BUILD TOOLS
#############################################################################

LLVM_TOOLS_CFLAGS=-I$(LLVMDIR)/include -I$(CLANGDIR)/include -D_GNU_SOURCE -D__STDC_LIMIT_MACROS -D__STDC_CONSTANT_MACROS

define DO_LLVM_TOOLS_CXX
$(echo_cmd) "LLVM_TOOLS_CXX $<"
$(Q)$(CXX) $(LLVM_TOOLS_CFLAGS) -o $@ -c $<
endef

define DO_LLVM_TOOLS_CC
$(echo_cmd) "LLVM_TOOLS_CC $<"
$(Q)$(CC) $(LLVM_TOOLS_CFLAGS) -o $@ -c $<
endef

CLANG=$(B)/tools/clang$(BINEXT)
LLVMLINK=$(B)/tools/llvm-link$(BINEXT)
LLVMCC:=$(CLANG)
LLVMCCFLAGS:= -emit-llvm -c -O3
LLVMLD:=$(LLVMLINK)
LLVMLDFLAGS:=

LLVMLINKOBJ_= \
	$(B)/tools/llvmlibs/Archive.o \
	$(B)/tools/llvmlibs/ArchiveReader.o \
	$(B)/tools/llvmlibs/ArchiveWriter.o \
	\
	$(B)/tools/llvmlibs/LLLexer.o \
	$(B)/tools/llvmlibs/LLParser.o \
	$(B)/tools/llvmlibs/Parser.o \
	\
	$(B)/tools/llvmlibs/BitcodeReader.o \
	$(B)/tools/llvmlibs/BitReader.o \
	\
	$(B)/tools/llvmlibs/BitcodeWriter.o \
	$(B)/tools/llvmlibs/BitcodeWriterPass.o \
	$(B)/tools/llvmlibs/BitWriter.o \
	$(B)/tools/llvmlibs/ValueEnumerator.o \
	\
	$(B)/tools/llvmlibs/LinkArchives.o \
	$(B)/tools/llvmlibs/Linker.o \
	$(B)/tools/llvmlibs/LinkItems.o \
	$(B)/tools/llvmlibs/LinkModules.o \
	\
	$(B)/tools/llvmlibs/Allocator.o \
	$(B)/tools/llvmlibs/APFloat.o \
	$(B)/tools/llvmlibs/APInt.o \
	$(B)/tools/llvmlibs/APSInt.o \
	$(B)/tools/llvmlibs/Atomic.o \
	$(B)/tools/llvmlibs/circular_raw_ostream.o \
	$(B)/tools/llvmlibs/CommandLine.o \
	$(B)/tools/llvmlibs/ConstantRange.o \
	$(B)/tools/llvmlibs/CrashRecoveryContext.o \
	$(B)/tools/llvmlibs/DAGDeltaAlgorithm.o \
	$(B)/tools/llvmlibs/Debug.o \
	$(B)/tools/llvmlibs/DeltaAlgorithm.o \
	$(B)/tools/llvmlibs/Disassembler.o \
	$(B)/tools/llvmlibs/Dwarf.o \
	$(B)/tools/llvmlibs/DynamicLibrary.o \
	$(B)/tools/llvmlibs/Errno.o \
	$(B)/tools/llvmlibs/ErrorHandling.o \
	$(B)/tools/llvmlibs/FileUtilities.o \
	$(B)/tools/llvmlibs/FoldingSet.o \
	$(B)/tools/llvmlibs/FormattedStream.o \
	$(B)/tools/llvmlibs/GraphWriter.o \
	$(B)/tools/llvmlibs/Host.o \
	$(B)/tools/llvmlibs/IncludeFile.o \
	$(B)/tools/llvmlibs/IntEqClasses.o \
	$(B)/tools/llvmlibs/IntervalMap.o \
	$(B)/tools/llvmlibs/IsInf.o \
	$(B)/tools/llvmlibs/IsNAN.o \
	$(B)/tools/llvmlibs/ManagedStatic.o \
	$(B)/tools/llvmlibs/Memory.o \
	$(B)/tools/llvmlibs/MemoryBuffer.o \
	$(B)/tools/llvmlibs/MemoryObject.o \
	$(B)/tools/llvmlibs/Mutex.o \
	$(B)/tools/llvmlibs/Path.o \
	$(B)/tools/llvmlibs/PathV2.o \
	$(B)/tools/llvmlibs/PluginLoader.o \
	$(B)/tools/llvmlibs/PrettyStackTrace.o \
	$(B)/tools/llvmlibs/Process.o \
	$(B)/tools/llvmlibs/Program.o \
	$(B)/tools/llvmlibs/raw_os_ostream.o \
	$(B)/tools/llvmlibs/raw_ostream.o \
	$(B)/tools/llvmlibs/regcomp.o \
	$(B)/tools/llvmlibs/regerror.o \
	$(B)/tools/llvmlibs/regexec.o \
	$(B)/tools/llvmlibs/regfree.o \
	$(B)/tools/llvmlibs/regstrlcpy.o \
	$(B)/tools/llvmlibs/Regex.o \
	$(B)/tools/llvmlibs/RWMutex.o \
	$(B)/tools/llvmlibs/SearchForAddressOfSpecialSymbol.o \
	$(B)/tools/llvmlibs/Signals.o \
	$(B)/tools/llvmlibs/SmallPtrSet.o \
	$(B)/tools/llvmlibs/SmallVector.o \
	$(B)/tools/llvmlibs/SourceMgr.o \
	$(B)/tools/llvmlibs/Statistic.o \
	$(B)/tools/llvmlibs/StringExtras.o \
	$(B)/tools/llvmlibs/StringMap.o \
	$(B)/tools/llvmlibs/StringPool.o \
	$(B)/tools/llvmlibs/StringRef.o \
	$(B)/tools/llvmlibs/system_error.o \
	$(B)/tools/llvmlibs/SystemUtils.o \
	$(B)/tools/llvmlibs/TargetRegistry.o \
	$(B)/tools/llvmlibs/Threading.o \
	$(B)/tools/llvmlibs/ThreadLocal.o \
	$(B)/tools/llvmlibs/Timer.o \
	$(B)/tools/llvmlibs/TimeValue.o \
	$(B)/tools/llvmlibs/ToolOutputFile.o \
	$(B)/tools/llvmlibs/Triple.o \
	$(B)/tools/llvmlibs/Twine.o \
	$(B)/tools/llvmlibs/Valgrind.o \
	\
	$(B)/tools/llvmlibs/ValueMapper.o \
	\
	$(B)/tools/llvmlibs/AsmWriter.o \
	$(B)/tools/llvmlibs/Attributes.o \
	$(B)/tools/llvmlibs/AutoUpgrade.o \
	$(B)/tools/llvmlibs/BasicBlock.o \
	$(B)/tools/llvmlibs/ConstantFold.o \
	$(B)/tools/llvmlibs/Constants.o \
	$(B)/tools/llvmlibs/Core.o \
	$(B)/tools/llvmlibs/DebugLoc.o \
	$(B)/tools/llvmlibs/Dominators.o \
	$(B)/tools/llvmlibs/Function.o \
	$(B)/tools/llvmlibs/Globals.o \
	$(B)/tools/llvmlibs/GVMaterializer.o \
	$(B)/tools/llvmlibs/InlineAsm.o \
	$(B)/tools/llvmlibs/Instruction.o \
	$(B)/tools/llvmlibs/Instructions.o \
	$(B)/tools/llvmlibs/IntrinsicInst.o \
	$(B)/tools/llvmlibs/IRBuilder.o \
	$(B)/tools/llvmlibs/LeakDetector.o \
	$(B)/tools/llvmlibs/LLVMContext.o \
	$(B)/tools/llvmlibs/LLVMContextImpl.o \
	$(B)/tools/llvmlibs/Metadata.o \
	$(B)/tools/llvmlibs/Module.o \
	$(B)/tools/llvmlibs/Pass.o \
	$(B)/tools/llvmlibs/PassManager.o \
	$(B)/tools/llvmlibs/PassRegistry.o \
	$(B)/tools/llvmlibs/PrintModulePass.o \
	$(B)/tools/llvmlibs/Type.o \
	$(B)/tools/llvmlibs/TypeSymbolTable.o \
	$(B)/tools/llvmlibs/Use.o \
	$(B)/tools/llvmlibs/User.o \
	$(B)/tools/llvmlibs/Value.o \
	$(B)/tools/llvmlibs/ValueSymbolTable.o \
	$(B)/tools/llvmlibs/ValueTypes.o \
	$(B)/tools/llvmlibs/Verifier.o

LLVMLINKOBJ = $(B)/tools/llvmlinker/llvm-link.o $(LLVMLINKOBJ_)
LLVMLINK_LDFLAGS=-pthread
LLVMLINK_LIBS=-ldl

$(B)/tools/llvmlinker/%.o: $(LLVMDIR)/tools/llvm-link/%.cpp
	$(DO_LLVM_TOOLS_CXX)

$(B)/tools/llvmlibs/%.o: $(LLVMDIR)/lib/Archive/%.cpp
	$(DO_LLVM_TOOLS_CXX)

$(B)/tools/llvmlibs/%.o: $(LLVMDIR)/lib/AsmParser/%.cpp
	$(DO_LLVM_TOOLS_CXX)

$(B)/tools/llvmlibs/%.o: $(LLVMDIR)/lib/Bitcode/Reader/%.cpp
	$(DO_LLVM_TOOLS_CXX)

$(B)/tools/llvmlibs/%.o: $(LLVMDIR)/lib/Bitcode/Writer/%.cpp
	$(DO_LLVM_TOOLS_CXX)

$(B)/tools/llvmlibs/%.o: $(LLVMDIR)/lib/Linker/%.cpp
	$(DO_LLVM_TOOLS_CXX)

$(B)/tools/llvmlibs/%.o: $(LLVMDIR)/lib/Support/%.cpp
	$(DO_LLVM_TOOLS_CXX)

$(B)/tools/llvmlibs/%.o: $(LLVMDIR)/lib/Support/%.c
	$(DO_LLVM_TOOLS_CC)

$(B)/tools/llvmlibs/%.o: $(LLVMDIR)/lib/Target/%.cpp
	$(DO_LLVM_TOOLS_CXX)

$(B)/tools/llvmlibs/%.o: $(LLVMDIR)/lib/Transforms/Utils/%.cpp
	$(DO_LLVM_TOOLS_CXX)

$(B)/tools/llvmlibs/%.o: $(LLVMDIR)/lib/VMCore/%.cpp
	$(DO_LLVM_TOOLS_CXX)

$(LLVMLINK): $(LLVMLINKOBJ)
	$(echo_cmd) "LD $@"
	$(Q)$(CXX) $(LLVMLINK_CFLAGS) $(LLVMLINK_LDFLAGS) -o $@ $^ $(LLVMLINK_LIBS)

llvmlink: $(LLVMLINK)

CLANGLLVMLIBOBJS= \
	$(B)/tools/llvmlibs/Mangler.o \
	$(B)/tools/llvmlibs/SubtargetFeature.o \
	$(B)/tools/llvmlibs/Target.o \
	$(B)/tools/llvmlibs/TargetAsmInfo.o \
	$(B)/tools/llvmlibs/TargetAsmLexer.o \
	$(B)/tools/llvmlibs/TargetData.o \
	$(B)/tools/llvmlibs/TargetELFWriterInfo.o \
	$(B)/tools/llvmlibs/TargetFrameLowering.o \
	$(B)/tools/llvmlibs/TargetInstrInfo.o \
	$(B)/tools/llvmlibs/TargetIntrinsicInfo.o \
	$(B)/tools/llvmlibs/TargetLibraryInfo.o \
	$(B)/tools/llvmlibs/TargetLoweringObjectFile.o \
	$(B)/tools/llvmlibs/TargetMachine.o \
	$(B)/tools/llvmlibs/TargetRegisterInfo.o \
	$(B)/tools/llvmlibs/TargetSubtarget.o

CLANGOBJ_= \
	$(B)/tools/llvmclang/cc1as_main.o \
	$(B)/tools/llvmclang/cc1_main.o \
	$(B)/tools/llvmclang/driver.o \
	\
	$(B)/tools/llvmclang/AnalysisContext.o \
	$(B)/tools/llvmclang/CFG.o \
	$(B)/tools/llvmclang/CFGReachabilityAnalysis.o \
	$(B)/tools/llvmclang/CFGStmtMap.o \
	$(B)/tools/llvmclang/CocoaConventions.o \
	$(B)/tools/llvmclang/FormatString.o \
	$(B)/tools/llvmclang/LiveVariables.o \
	$(B)/tools/llvmclang/PrintfFormatString.o \
	$(B)/tools/llvmclang/PseudoConstantAnalysis.o \
	$(B)/tools/llvmclang/ReachableCode.o \
	$(B)/tools/llvmclang/ScanfFormatString.o \
	$(B)/tools/llvmclang/UninitializedValues.o \
	$(B)/tools/llvmclang/UninitializedValuesV2.o \
	\
	$(B)/tools/llvmclang/APValue.o \
	$(B)/tools/llvmclang/ASTConsumer.o \
	$(B)/tools/llvmclang/ASTContext.o \
	$(B)/tools/llvmclang/ASTDiagnostic.o \
	$(B)/tools/llvmclang/ASTImporter.o \
	$(B)/tools/llvmclang/AttrImpl.o \
	$(B)/tools/llvmclang/CXXInheritance.o \
	$(B)/tools/llvmclang/Decl.o \
	$(B)/tools/llvmclang/DeclarationName.o \
	$(B)/tools/llvmclang/DeclBase.o \
	$(B)/tools/llvmclang/DeclCXX.o \
	$(B)/tools/llvmclang/DeclFriend.o \
	$(B)/tools/llvmclang/DeclGroup.o \
	$(B)/tools/llvmclang/DeclObjC.o \
	$(B)/tools/llvmclang/DeclPrinter.o \
	$(B)/tools/llvmclang/DeclTemplate.o \
	$(B)/tools/llvmclang/DumpXML.o \
	$(B)/tools/llvmclang/Expr.o \
	$(B)/tools/llvmclang/ExprClassification.o \
	$(B)/tools/llvmclang/ExprConstant.o \
	$(B)/tools/llvmclang/ExprCXX.o \
	$(B)/tools/llvmclang/ExternalASTSource.o \
	$(B)/tools/llvmclang/InheritViz.o \
	$(B)/tools/llvmclang/ItaniumCXXABI.o \
	$(B)/tools/llvmclang/ItaniumMangle.o \
	$(B)/tools/llvmclang/Mangle.o \
	$(B)/tools/llvmclang/MicrosoftCXXABI.o \
	$(B)/tools/llvmclang/MicrosoftMangle.o \
	$(B)/tools/llvmclang/NestedNameSpecifier.o \
	$(B)/tools/llvmclang/ParentMap.o \
	$(B)/tools/llvmclang/RecordLayout.o \
	$(B)/tools/llvmclang/RecordLayoutBuilder.o \
	$(B)/tools/llvmclang/Stmt.o \
	$(B)/tools/llvmclang/StmtDumper.o \
	$(B)/tools/llvmclang/StmtIterator.o \
	$(B)/tools/llvmclang/StmtPrinter.o \
	$(B)/tools/llvmclang/StmtProfile.o \
	$(B)/tools/llvmclang/StmtViz.o \
	$(B)/tools/llvmclang/TemplateBase.o \
	$(B)/tools/llvmclang/TemplateName.o \
	$(B)/tools/llvmclang/Type.o \
	$(B)/tools/llvmclang/TypeLoc.o \
	$(B)/tools/llvmclang/TypePrinter.o \
	\
	$(B)/tools/llvmclang/ConvertUTF.o \
	$(B)/tools/llvmclang/Builtins.o \
	$(B)/tools/llvmclang/Diagnostic.o \
	$(B)/tools/llvmclang/DiagnosticIDs.o \
	$(B)/tools/llvmclang/FileManager.o \
	$(B)/tools/llvmclang/FileSystemStatCache.o \
	$(B)/tools/llvmclang/IdentifierTable.o \
	$(B)/tools/llvmclang/SourceLocation.o \
	$(B)/tools/llvmclang/SourceManager.o \
	$(B)/tools/llvmclang/TargetInfo.o \
	$(B)/tools/llvmclang/Targets.o \
	$(B)/tools/llvmclang/TokenKinds.o \
	$(B)/tools/llvmclang/Version.o \
	\
	$(B)/tools/llvmclang/BackendUtil.o \
	$(B)/tools/llvmclang/CGBlocks.o \
	$(B)/tools/llvmclang/CGBuiltin.o \
	$(B)/tools/llvmclang/CGCall.o \
	$(B)/tools/llvmclang/CGClass.o \
	$(B)/tools/llvmclang/CGCleanup.o \
	$(B)/tools/llvmclang/CGCXX.o \
	$(B)/tools/llvmclang/CGCXXABI.o \
	$(B)/tools/llvmclang/CGDebugInfo.o \
	$(B)/tools/llvmclang/CGDecl.o \
	$(B)/tools/llvmclang/CGDeclCXX.o \
	$(B)/tools/llvmclang/CGException.o \
	$(B)/tools/llvmclang/CGExpr.o \
	$(B)/tools/llvmclang/CGExprAgg.o \
	$(B)/tools/llvmclang/CGExprComplex.o \
	$(B)/tools/llvmclang/CGExprConstant.o \
	$(B)/tools/llvmclang/CGExprCXX.o \
	$(B)/tools/llvmclang/CGExprScalar.o \
	$(B)/tools/llvmclang/CGObjC.o \
	$(B)/tools/llvmclang/CGObjCGNU.o \
	$(B)/tools/llvmclang/CGObjCMac.o \
	$(B)/tools/llvmclang/CGRecordLayoutBuilder.o \
	$(B)/tools/llvmclang/CGRTTI.o \
	$(B)/tools/llvmclang/CGStmt.o \
	$(B)/tools/llvmclang/CGTemporaries.o \
	$(B)/tools/llvmclang/CGVTables.o \
	$(B)/tools/llvmclang/CGVTT.o \
	$(B)/tools/llvmclang/CodeGenAction.o \
	$(B)/tools/llvmclang/CodeGenFunction.o \
	$(B)/tools/llvmclang/CodeGenModule.o \
	$(B)/tools/llvmclang/CodeGenTBAA.o \
	$(B)/tools/llvmclang/CodeGenTypes.o \
	$(B)/tools/llvmclang/ItaniumCXXABI.o \
	$(B)/tools/llvmclang/MicrosoftCXXABI.o \
	$(B)/tools/llvmclang/ModuleBuilder.o \
	$(B)/tools/llvmclang/TargetInfo.o \
	\
	$(B)/tools/llvmclang/Action.o \
	$(B)/tools/llvmclang/Arg.o \
	$(B)/tools/llvmclang/ArgList.o \
	$(B)/tools/llvmclang/CC1AsOptions.o \
	$(B)/tools/llvmclang/CC1Options.o \
	$(B)/tools/llvmclang/Compilation.o \
	$(B)/tools/llvmclang/Driver.o \
	$(B)/tools/llvmclang/DriverOptions.o \
	$(B)/tools/llvmclang/HostInfo.o \
	$(B)/tools/llvmclang/Job.o \
	$(B)/tools/llvmclang/Option.o \
	$(B)/tools/llvmclang/OptTable.o \
	$(B)/tools/llvmclang/Phases.o \
	$(B)/tools/llvmclang/Tool.o \
	$(B)/tools/llvmclang/ToolChain.o \
	$(B)/tools/llvmclang/ToolChains.o \
	$(B)/tools/llvmclang/Tools.o \
	$(B)/tools/llvmclang/Types.o \
	\
	$(B)/tools/llvmclang/ASTConsumers.o \
	$(B)/tools/llvmclang/ASTMerge.o \
	$(B)/tools/llvmclang/ASTUnit.o \
	$(B)/tools/llvmclang/BoostConAction.o \
	$(B)/tools/llvmclang/CacheTokens.o \
	$(B)/tools/llvmclang/CompilerInstance.o \
	$(B)/tools/llvmclang/CompilerInvocation.o \
	$(B)/tools/llvmclang/DependencyFile.o \
	$(B)/tools/llvmclang/DiagChecker.o \
	$(B)/tools/llvmclang/FrontendAction.o \
	$(B)/tools/llvmclang/FrontendActions.o \
	$(B)/tools/llvmclang/FrontendOptions.o \
	$(B)/tools/llvmclang/HeaderIncludeGen.o \
	$(B)/tools/llvmclang/InitHeaderSearch.o \
	$(B)/tools/llvmclang/InitPreprocessor.o \
	$(B)/tools/llvmclang/LangStandards.o \
	$(B)/tools/llvmclang/MultiplexConsumer.o \
	$(B)/tools/llvmclang/PrintPreprocessedOutput.o \
	$(B)/tools/llvmclang/TextDiagnosticBuffer.o \
	$(B)/tools/llvmclang/TextDiagnosticPrinter.o \
	$(B)/tools/llvmclang/VerifyDiagnosticsClient.o \
	$(B)/tools/llvmclang/Warnings.o \
	\
	$(B)/tools/llvmclang/ExecuteCompilerInvocation.o \
	\
	$(B)/tools/llvmclang/Analyzer.o \
	$(B)/tools/llvmclang/ASTLocation.o \
	$(B)/tools/llvmclang/CallGraph.o \
	$(B)/tools/llvmclang/DeclReferenceMap.o \
	$(B)/tools/llvmclang/Entity.o \
	$(B)/tools/llvmclang/GlobalSelector.o \
	$(B)/tools/llvmclang/Handlers.o \
	$(B)/tools/llvmclang/Indexer.o \
	$(B)/tools/llvmclang/IndexProvider.o \
	$(B)/tools/llvmclang/Program.o \
	$(B)/tools/llvmclang/SelectorMap.o \
	\
	$(B)/tools/llvmclang/HeaderMap.o \
	$(B)/tools/llvmclang/HeaderSearch.o \
	$(B)/tools/llvmclang/Lexer.o \
	$(B)/tools/llvmclang/LiteralSupport.o \
	$(B)/tools/llvmclang/MacroArgs.o \
	$(B)/tools/llvmclang/MacroInfo.o \
	$(B)/tools/llvmclang/PPCaching.o \
	$(B)/tools/llvmclang/PPDirectives.o \
	$(B)/tools/llvmclang/PPExpressions.o \
	$(B)/tools/llvmclang/PPLexerChange.o \
	$(B)/tools/llvmclang/PPMacroExpansion.o \
	$(B)/tools/llvmclang/Pragma.o \
	$(B)/tools/llvmclang/PreprocessingRecord.o \
	$(B)/tools/llvmclang/Preprocessor.o \
	$(B)/tools/llvmclang/PreprocessorLexer.o \
	$(B)/tools/llvmclang/PTHLexer.o \
	$(B)/tools/llvmclang/ScratchBuffer.o \
	$(B)/tools/llvmclang/TokenConcatenation.o \
	$(B)/tools/llvmclang/TokenLexer.o \
	\
	$(B)/tools/llvmclang/ParseAST.o \
	$(B)/tools/llvmclang/ParseCXXInlineMethods.o \
	$(B)/tools/llvmclang/ParseDecl.o \
	$(B)/tools/llvmclang/ParseDeclCXX.o \
	$(B)/tools/llvmclang/ParseExpr.o \
	$(B)/tools/llvmclang/ParseExprCXX.o \
	$(B)/tools/llvmclang/ParseInit.o \
	$(B)/tools/llvmclang/ParseObjc.o \
	$(B)/tools/llvmclang/ParsePragma.o \
	$(B)/tools/llvmclang/Parser.o \
	$(B)/tools/llvmclang/ParseStmt.o \
	$(B)/tools/llvmclang/ParseTemplate.o \
	$(B)/tools/llvmclang/ParseTentative.o \
	\
	$(B)/tools/llvmclang/DeltaTree.o \
	$(B)/tools/llvmclang/FixItRewriter.o \
	$(B)/tools/llvmclang/FrontendActions.o \
	$(B)/tools/llvmclang/HTMLPrint.o \
	$(B)/tools/llvmclang/HTMLRewrite.o \
	$(B)/tools/llvmclang/RewriteMacros.o \
	$(B)/tools/llvmclang/RewriteObjC.o \
	$(B)/tools/llvmclang/Rewriter.o \
	$(B)/tools/llvmclang/RewriteRope.o \
	$(B)/tools/llvmclang/RewriteTest.o \
	$(B)/tools/llvmclang/TokenRewriter.o \
	\
	$(B)/tools/llvmclang/AnalysisBasedWarnings.o \
	$(B)/tools/llvmclang/AttributeList.o \
	$(B)/tools/llvmclang/CodeCompleteConsumer.o \
	$(B)/tools/llvmclang/DeclSpec.o \
	$(B)/tools/llvmclang/IdentifierResolver.o \
	$(B)/tools/llvmclang/JumpDiagnostics.o \
	$(B)/tools/llvmclang/Sema.o \
	$(B)/tools/llvmclang/SemaAccess.o \
	$(B)/tools/llvmclang/SemaAttr.o \
	$(B)/tools/llvmclang/SemaChecking.o \
	$(B)/tools/llvmclang/SemaCodeComplete.o \
	$(B)/tools/llvmclang/SemaCXXCast.o \
	$(B)/tools/llvmclang/SemaCXXScopeSpec.o \
	$(B)/tools/llvmclang/SemaDecl.o \
	$(B)/tools/llvmclang/SemaDeclAttr.o \
	$(B)/tools/llvmclang/SemaDeclCXX.o \
	$(B)/tools/llvmclang/SemaDeclObjC.o \
	$(B)/tools/llvmclang/SemaExceptionSpec.o \
	$(B)/tools/llvmclang/SemaExpr.o \
	$(B)/tools/llvmclang/SemaExprCXX.o \
	$(B)/tools/llvmclang/SemaExprObjC.o \
	$(B)/tools/llvmclang/SemaInit.o \
	$(B)/tools/llvmclang/SemaLookup.o \
	$(B)/tools/llvmclang/SemaObjCProperty.o \
	$(B)/tools/llvmclang/SemaOverload.o \
	$(B)/tools/llvmclang/SemaStmt.o \
	$(B)/tools/llvmclang/SemaTemplate.o \
	$(B)/tools/llvmclang/SemaTemplateDeduction.o \
	$(B)/tools/llvmclang/SemaTemplateInstantiate.o \
	$(B)/tools/llvmclang/SemaTemplateInstantiateDecl.o \
	$(B)/tools/llvmclang/SemaTemplateVariadic.o \
	$(B)/tools/llvmclang/SemaType.o \
	$(B)/tools/llvmclang/TargetAttributesSema.o \
	\
	$(B)/tools/llvmclang/ASTCommon.o \
	$(B)/tools/llvmclang/ASTReader.o \
	$(B)/tools/llvmclang/ASTReaderDecl.o \
	$(B)/tools/llvmclang/ASTReaderStmt.o \
	$(B)/tools/llvmclang/ASTWriter.o \
	$(B)/tools/llvmclang/ASTWriterDecl.o \
	$(B)/tools/llvmclang/ASTWriterStmt.o \
	$(B)/tools/llvmclang/GeneratePCH.o \
	\
	$(B)/tools/llvmclang/AdjustedReturnValueChecker.o \
	$(B)/tools/llvmclang/AnalyzerStatsChecker.o \
	$(B)/tools/llvmclang/ArrayBoundChecker.o \
	$(B)/tools/llvmclang/ArrayBoundCheckerV2.o \
	$(B)/tools/llvmclang/AttrNonNullChecker.o \
	$(B)/tools/llvmclang/BasicObjCFoundationChecks.o \
	$(B)/tools/llvmclang/BuiltinFunctionChecker.o \
	$(B)/tools/llvmclang/CallAndMessageChecker.o \
	$(B)/tools/llvmclang/CastSizeChecker.o \
	$(B)/tools/llvmclang/CastToStructChecker.o \
	$(B)/tools/llvmclang/CheckObjCDealloc.o \
	$(B)/tools/llvmclang/CheckObjCInstMethSignature.o \
	$(B)/tools/llvmclang/CheckSecuritySyntaxOnly.o \
	$(B)/tools/llvmclang/CheckSizeofPointer.o \
	$(B)/tools/llvmclang/ChrootChecker.o \
	$(B)/tools/llvmclang/ClangSACheckerProvider.o \
	$(B)/tools/llvmclang/CStringChecker.o \
	$(B)/tools/llvmclang/DeadStoresChecker.o \
	$(B)/tools/llvmclang/DebugCheckers.o \
	$(B)/tools/llvmclang/DereferenceChecker.o \
	$(B)/tools/llvmclang/DivZeroChecker.o \
	$(B)/tools/llvmclang/FixedAddressChecker.o \
	$(B)/tools/llvmclang/IdempotentOperationChecker.o \
	$(B)/tools/llvmclang/LLVMConventionsChecker.o \
	$(B)/tools/llvmclang/MacOSXAPIChecker.o \
	$(B)/tools/llvmclang/MallocChecker.o \
	$(B)/tools/llvmclang/NoReturnFunctionChecker.o \
	$(B)/tools/llvmclang/NSAutoreleasePoolChecker.o \
	$(B)/tools/llvmclang/NSErrorChecker.o \
	$(B)/tools/llvmclang/ObjCAtSyncChecker.o \
	$(B)/tools/llvmclang/ObjCSelfInitChecker.o \
	$(B)/tools/llvmclang/ObjCUnusedIVarsChecker.o \
	$(B)/tools/llvmclang/OSAtomicChecker.o \
	$(B)/tools/llvmclang/PointerArithChecker.o \
	$(B)/tools/llvmclang/PointerSubChecker.o \
	$(B)/tools/llvmclang/PthreadLockChecker.o \
	$(B)/tools/llvmclang/ReturnPointerRangeChecker.o \
	$(B)/tools/llvmclang/ReturnUndefChecker.o \
	$(B)/tools/llvmclang/StackAddrEscapeChecker.o \
	$(B)/tools/llvmclang/StreamChecker.o \
	$(B)/tools/llvmclang/UndefBranchChecker.o \
	$(B)/tools/llvmclang/UndefCapturedBlockVarChecker.o \
	$(B)/tools/llvmclang/UndefinedArraySubscriptChecker.o \
	$(B)/tools/llvmclang/UndefinedAssignmentChecker.o \
	$(B)/tools/llvmclang/UndefResultChecker.o \
	$(B)/tools/llvmclang/UnixAPIChecker.o \
	$(B)/tools/llvmclang/UnreachableCodeChecker.o \
	$(B)/tools/llvmclang/VLASizeChecker.o \
	\
	$(B)/tools/llvmclang/AggExprVisitor.o \
	$(B)/tools/llvmclang/AnalysisManager.o \
	$(B)/tools/llvmclang/BasicConstraintManager.o \
	$(B)/tools/llvmclang/BasicStore.o \
	$(B)/tools/llvmclang/BasicValueFactory.o \
	$(B)/tools/llvmclang/BlockCounter.o \
	$(B)/tools/llvmclang/BugReporter.o \
	$(B)/tools/llvmclang/BugReporterVisitors.o \
	$(B)/tools/llvmclang/CFRefCount.o \
	$(B)/tools/llvmclang/CheckerContext.o \
	$(B)/tools/llvmclang/CheckerHelpers.o \
	$(B)/tools/llvmclang/CheckerManager.o \
	$(B)/tools/llvmclang/CoreEngine.o \
	$(B)/tools/llvmclang/CXXExprEngine.o \
	$(B)/tools/llvmclang/Environment.o \
	$(B)/tools/llvmclang/ExplodedGraph.o \
	$(B)/tools/llvmclang/ExprEngine.o \
	$(B)/tools/llvmclang/FlatStore.o \
	$(B)/tools/llvmclang/GRState.o \
	$(B)/tools/llvmclang/HTMLDiagnostics.o \
	$(B)/tools/llvmclang/MemRegion.o \
	$(B)/tools/llvmclang/ObjCMessage.o \
	$(B)/tools/llvmclang/PathDiagnostic.o \
	$(B)/tools/llvmclang/PlistDiagnostics.o \
	$(B)/tools/llvmclang/RangeConstraintManager.o \
	$(B)/tools/llvmclang/RegionStore.o \
	$(B)/tools/llvmclang/SimpleConstraintManager.o \
	$(B)/tools/llvmclang/SimpleSValBuilder.o \
	$(B)/tools/llvmclang/Store.o \
	$(B)/tools/llvmclang/SValBuilder.o \
	$(B)/tools/llvmclang/SVals.o \
	$(B)/tools/llvmclang/SymbolManager.o \
	$(B)/tools/llvmclang/TextPathDiagnostics.o \
	\
	$(B)/tools/llvmclang/AnalysisConsumer.o \
	$(B)/tools/llvmclang/CheckerRegistration.o \
	$(B)/tools/llvmclang/FrontendActions.o \

CLANGOBJ=$(LLVMLINKOBJ_) $(CLANGLLVMLIBOBJS) $(CLANGOBJ_)
CLANGOBJ_LDFLAGS=-pthread
CLANGOBJ_LIBS=-ldl

$(B)/tools/llvmclang/%.o: $(CLANGDIR)/tools/driver/%.cpp
	$(DO_LLVM_TOOLS_CXX)

$(B)/tools/llvmclang/%.o: $(CLANGDIR)/lib/Analysis/%.cpp
	$(DO_LLVM_TOOLS_CXX)

$(B)/tools/llvmclang/%.o: $(CLANGDIR)/lib/AST/%.cpp
	$(DO_LLVM_TOOLS_CXX)

$(B)/tools/llvmclang/%.o: $(CLANGDIR)/lib/Basic/%.cpp
	$(DO_LLVM_TOOLS_CXX)

$(B)/tools/llvmclang/%.o: $(CLANGDIR)/lib/Basic/%.c
	$(DO_LLVM_TOOLS_CC)

$(B)/tools/llvmclang/%.o: $(CLANGDIR)/lib/CodeGen/%.cpp
	$(DO_LLVM_TOOLS_CXX)

$(B)/tools/llvmclang/%.o: $(CLANGDIR)/lib/Driver/%.cpp
	$(DO_LLVM_TOOLS_CXX)

$(B)/tools/llvmclang/%.o: $(CLANGDIR)/lib/Frontend/%.cpp
	$(DO_LLVM_TOOLS_CXX)

$(B)/tools/llvmclang/%.o: $(CLANGDIR)/lib/FrontendTool/%.cpp
	$(DO_LLVM_TOOLS_CXX)

$(B)/tools/llvmclang/%.o: $(CLANGDIR)/lib/Index/%.cpp
	$(DO_LLVM_TOOLS_CXX)

$(B)/tools/llvmclang/%.o: $(CLANGDIR)/lib/Lex/%.cpp
	$(DO_LLVM_TOOLS_CXX)

$(B)/tools/llvmclang/%.o: $(CLANGDIR)/lib/Parse/%.cpp
	$(DO_LLVM_TOOLS_CXX)

$(B)/tools/llvmclang/%.o: $(CLANGDIR)/lib/Rewrite/%.cpp
	$(DO_LLVM_TOOLS_CXX)

$(B)/tools/llvmclang/%.o: $(CLANGDIR)/lib/Sema/%.cpp
	$(DO_LLVM_TOOLS_CXX)

$(B)/tools/llvmclang/%.o: $(CLANGDIR)/lib/Serialization/%.cpp
	$(DO_LLVM_TOOLS_CXX)

$(B)/tools/llvmclang/%.o: $(CLANGDIR)/lib/StaticAnalyzer/Checkers/%.cpp
	$(DO_LLVM_TOOLS_CXX)

$(B)/tools/llvmclang/%.o: $(CLANGDIR)/lib/StaticAnalyzer/Core/%.cpp
	$(DO_LLVM_TOOLS_CXX)

$(B)/tools/llvmclang/%.o: $(CLANGDIR)/lib/StaticAnalyzer/Frontend/%.cpp
	$(DO_LLVM_TOOLS_CXX)

$(CLANG): $(CLANGOBJ)
	$(echo_cmd) "LD $@"
	$(Q)$(CXX) $(CLANG_CFLAGS) $(CLANG_LDFLAGS) -o $@ $^ $(CLANG_LIBS)

ifneq ($(USE_INTERNAL_LLVM),1)
  CLANG=
  LLVMLINK=
  LLVMCC:=clang$(BINEXT)
  LLVMLD:=llvm-link$(BINEXT)
endif

define DO_LLVMCC
$(echo_cmd) "LLVMCC $<"
$(Q)$(LLVMCC) $(LLVMCCFLAGS) $(CFLAGS) -o $@ $<
endef

define DO_CGAME_LLVMCC
$(echo_cmd) "CGAME_LLVMCC $<"
$(Q)$(LLVMCC) $(LLVMCCFLAGS) $(CFLAGS) -DCGAME -o $@ $<
endef

define DO_GAME_LLVMCC
$(echo_cmd) "GAME_LLVMCC $<"
$(Q)$(LLVMCC) $(LLVMCCFLAGS) $(CFLAGS) -DQAGAME -o $@ $<
endef

define DO_UI_LLVMCC
$(echo_cmd) "UI_LLVMCC $<"
$(Q)$(LLVMCC) $(LLVMCCFLAGS) $(CFLAGS) -DUI -o $@ $<
endef

define DO_LLVMCC_MISSIONPACK
$(echo_cmd) "LLVMCC_MISSIONPACK $<"
$(Q)$(LLVMCC) $(LLVMCCFLAGS) $(CFLAGS) -DMISSIONPACK -o $@ $<
endef

define DO_CGAME_LLVMCC_MISSIONPACK
$(echo_cmd) "CGAME_LLVMCC_MISSIONPACK $<"
$(Q)$(LLVMCC) $(LLVMCCFLAGS) $(CFLAGS) -DMISSIONPACK -DCGAME -o $@ $<
endef

define DO_GAME_LLVMCC_MISSIONPACK
$(echo_cmd) "GAME_LLVMCC_MISSIONPACK $<"
$(Q)$(LLVMCC) $(LLVMCCFLAGS) $(CFLAGS) -DMISSIONPACK -DQAGAME -o $@ $<
endef

define DO_UI_LLVMCC_MISSIONPACK
$(echo_cmd) "UI_LLVMCC_MISSIONPACK $<"
$(Q)$(LLVMCC) $(LLVMCCFLAGS) $(CFLAGS) -DMISSIONPACK -DUI -o $@ $<
endef

#############################################################################
# CLIENT/SERVER
#############################################################################

Q3OBJ = \
  $(B)/client/cl_cgame.o \
  $(B)/client/cl_cin.o \
  $(B)/client/cl_console.o \
  $(B)/client/cl_input.o \
  $(B)/client/cl_keys.o \
  $(B)/client/cl_main.o \
  $(B)/client/cl_net_chan.o \
  $(B)/client/cl_parse.o \
  $(B)/client/cl_scrn.o \
  $(B)/client/cl_ui.o \
  $(B)/client/cl_avi.o \
  \
  $(B)/client/cm_load.o \
  $(B)/client/cm_patch.o \
  $(B)/client/cm_polylib.o \
  $(B)/client/cm_test.o \
  $(B)/client/cm_trace.o \
  \
  $(B)/client/cmd.o \
  $(B)/client/common.o \
  $(B)/client/cvar.o \
  $(B)/client/files.o \
  $(B)/client/md4.o \
  $(B)/client/md5.o \
  $(B)/client/msg.o \
  $(B)/client/net_chan.o \
  $(B)/client/net_ip.o \
  $(B)/client/huffman.o \
  \
  $(B)/client/snd_adpcm.o \
  $(B)/client/snd_dma.o \
  $(B)/client/snd_mem.o \
  $(B)/client/snd_mix.o \
  $(B)/client/snd_wavelet.o \
  \
  $(B)/client/snd_main.o \
  $(B)/client/snd_codec.o \
  $(B)/client/snd_codec_wav.o \
  $(B)/client/snd_codec_ogg.o \
  \
  $(B)/client/qal.o \
  $(B)/client/snd_openal.o \
  \
  $(B)/client/cl_curl.o \
  \
  $(B)/client/sv_bot.o \
  $(B)/client/sv_ccmds.o \
  $(B)/client/sv_client.o \
  $(B)/client/sv_game.o \
  $(B)/client/sv_init.o \
  $(B)/client/sv_main.o \
  $(B)/client/sv_net_chan.o \
  $(B)/client/sv_snapshot.o \
  $(B)/client/sv_world.o \
  \
  $(B)/client/q_math.o \
  $(B)/client/q_shared.o \
  \
  $(B)/client/unzip.o \
  $(B)/client/ioapi.o \
  $(B)/client/puff.o \
  $(B)/client/vm.o \
  $(B)/client/vm_interpreted.o \
  \
  $(B)/client/be_aas_bspq3.o \
  $(B)/client/be_aas_cluster.o \
  $(B)/client/be_aas_debug.o \
  $(B)/client/be_aas_entity.o \
  $(B)/client/be_aas_file.o \
  $(B)/client/be_aas_main.o \
  $(B)/client/be_aas_move.o \
  $(B)/client/be_aas_optimize.o \
  $(B)/client/be_aas_reach.o \
  $(B)/client/be_aas_route.o \
  $(B)/client/be_aas_routealt.o \
  $(B)/client/be_aas_sample.o \
  $(B)/client/be_ai_char.o \
  $(B)/client/be_ai_chat.o \
  $(B)/client/be_ai_gen.o \
  $(B)/client/be_ai_goal.o \
  $(B)/client/be_ai_move.o \
  $(B)/client/be_ai_weap.o \
  $(B)/client/be_ai_weight.o \
  $(B)/client/be_ea.o \
  $(B)/client/be_interface.o \
  $(B)/client/l_crc.o \
  $(B)/client/l_libvar.o \
  $(B)/client/l_log.o \
  $(B)/client/l_memory.o \
  $(B)/client/l_precomp.o \
  $(B)/client/l_script.o \
  $(B)/client/l_struct.o \
  \
  $(B)/client/sdl_input.o \
  $(B)/client/sdl_snd.o \
  \
  $(B)/client/con_log.o \
  $(B)/client/sys_main.o

ifeq ($(PLATFORM),mingw32)
  Q3OBJ += \
	$(B)/client/con_passive.o
else
  Q3OBJ += \
	$(B)/client/con_tty.o
endif

Q3ROBJ = \
  $(B)/renderer/tr_animation.o \
  $(B)/renderer/tr_backend.o \
  $(B)/renderer/tr_bsp.o \
  $(B)/renderer/tr_cmds.o \
  $(B)/renderer/tr_curve.o \
  $(B)/renderer/tr_flares.o \
  $(B)/renderer/tr_font.o \
  $(B)/renderer/tr_image.o \
  $(B)/renderer/tr_image_png.o \
  $(B)/renderer/tr_image_jpg.o \
  $(B)/renderer/tr_image_bmp.o \
  $(B)/renderer/tr_image_tga.o \
  $(B)/renderer/tr_image_pcx.o \
  $(B)/renderer/tr_init.o \
  $(B)/renderer/tr_light.o \
  $(B)/renderer/tr_main.o \
  $(B)/renderer/tr_marks.o \
  $(B)/renderer/tr_mesh.o \
  $(B)/renderer/tr_model.o \
  $(B)/renderer/tr_model_iqm.o \
  $(B)/renderer/tr_noise.o \
  $(B)/renderer/tr_scene.o \
  $(B)/renderer/tr_shade.o \
  $(B)/renderer/tr_shade_calc.o \
  $(B)/renderer/tr_shader.o \
  $(B)/renderer/tr_shadows.o \
  $(B)/renderer/tr_sky.o \
  $(B)/renderer/tr_surface.o \
  $(B)/renderer/tr_world.o \
  \
  $(B)/renderer/sdl_gamma.o
  
ifneq ($(USE_RENDERER_DLOPEN), 0)
  Q3ROBJ += \
    $(B)/renderer/q_shared.o \
    $(B)/renderer/puff.o \
    $(B)/renderer/q_math.o \
    $(B)/renderer/tr_subs.o
endif

ifneq ($(USE_INTERNAL_JPEG),0)
  Q3ROBJ += \
    $(B)/renderer/jaricom.o \
    $(B)/renderer/jcapimin.o \
    $(B)/renderer/jcapistd.o \
    $(B)/renderer/jcarith.o \
    $(B)/renderer/jccoefct.o  \
    $(B)/renderer/jccolor.o \
    $(B)/renderer/jcdctmgr.o \
    $(B)/renderer/jchuff.o   \
    $(B)/renderer/jcinit.o \
    $(B)/renderer/jcmainct.o \
    $(B)/renderer/jcmarker.o \
    $(B)/renderer/jcmaster.o \
    $(B)/renderer/jcomapi.o \
    $(B)/renderer/jcparam.o \
    $(B)/renderer/jcprepct.o \
    $(B)/renderer/jcsample.o \
    $(B)/renderer/jctrans.o \
    $(B)/renderer/jdapimin.o \
    $(B)/renderer/jdapistd.o \
    $(B)/renderer/jdarith.o \
    $(B)/renderer/jdatadst.o \
    $(B)/renderer/jdatasrc.o \
    $(B)/renderer/jdcoefct.o \
    $(B)/renderer/jdcolor.o \
    $(B)/renderer/jddctmgr.o \
    $(B)/renderer/jdhuff.o \
    $(B)/renderer/jdinput.o \
    $(B)/renderer/jdmainct.o \
    $(B)/renderer/jdmarker.o \
    $(B)/renderer/jdmaster.o \
    $(B)/renderer/jdmerge.o \
    $(B)/renderer/jdpostct.o \
    $(B)/renderer/jdsample.o \
    $(B)/renderer/jdtrans.o \
    $(B)/renderer/jerror.o \
    $(B)/renderer/jfdctflt.o \
    $(B)/renderer/jfdctfst.o \
    $(B)/renderer/jfdctint.o \
    $(B)/renderer/jidctflt.o \
    $(B)/renderer/jidctfst.o \
    $(B)/renderer/jidctint.o \
    $(B)/renderer/jmemmgr.o \
    $(B)/renderer/jmemnobs.o \
    $(B)/renderer/jquant1.o \
    $(B)/renderer/jquant2.o \
    $(B)/renderer/jutils.o
endif

ifeq ($(USE_FREETYPE),1)
ifneq ($(USE_INTERNAL_FREETYPE),0)
  Q3ROBJ += \
    $(B)/renderer/ftinit.o \
    $(B)/renderer/ftsystem.o \
    $(B)/renderer/ftbase.o \
	\
    $(B)/renderer/ftbzip2.o \
    $(B)/renderer/ftgzip.o \
    $(B)/renderer/ftlzw.o \
	\
    $(B)/renderer/autofit.o \
    $(B)/renderer/bdf.o \
    $(B)/renderer/cff.o \
    $(B)/renderer/pcf.o \
    $(B)/renderer/pfr.o \
    $(B)/renderer/psaux.o \
    $(B)/renderer/pshinter.o \
    $(B)/renderer/psnames.o \
    $(B)/renderer/raster.o \
    $(B)/renderer/sfnt.o \
    $(B)/renderer/smooth.o \
    $(B)/renderer/truetype.o \
    $(B)/renderer/type1.o \
    $(B)/renderer/type1cid.o \
    $(B)/renderer/type42.o \
    $(B)/renderer/winfnt.o
endif
endif

ifeq ($(ARCH),i386)
  Q3OBJ += \
    $(B)/client/snd_mixa.o \
    $(B)/client/matha.o \
    $(B)/client/snapvector.o \
    $(B)/client/ftola.o
endif
ifeq ($(ARCH),x86)
  Q3OBJ += \
    $(B)/client/snd_mixa.o \
    $(B)/client/matha.o \
    $(B)/client/snapvector.o \
    $(B)/client/ftola.o
endif
ifeq ($(ARCH),x86_64)
  Q3OBJ += \
    $(B)/client/snapvector.o \
    $(B)/client/ftola.o
endif
ifeq ($(ARCH),amd64)
  Q3OBJ += \
    $(B)/client/snapvector.o \
    $(B)/client/ftola.o
endif
ifeq ($(ARCH),x64)
  Q3OBJ += \
    $(B)/client/snapvector.o \
    $(B)/client/ftola.o
endif

ifeq ($(USE_VOIP),1)
ifeq ($(USE_INTERNAL_SPEEX),1)
Q3OBJ += \
  $(B)/client/bits.o \
  $(B)/client/buffer.o \
  $(B)/client/cb_search.o \
  $(B)/client/exc_10_16_table.o \
  $(B)/client/exc_10_32_table.o \
  $(B)/client/exc_20_32_table.o \
  $(B)/client/exc_5_256_table.o \
  $(B)/client/exc_5_64_table.o \
  $(B)/client/exc_8_128_table.o \
  $(B)/client/fftwrap.o \
  $(B)/client/filterbank.o \
  $(B)/client/filters.o \
  $(B)/client/gain_table.o \
  $(B)/client/gain_table_lbr.o \
  $(B)/client/hexc_10_32_table.o \
  $(B)/client/hexc_table.o \
  $(B)/client/high_lsp_tables.o \
  $(B)/client/jitter.o \
  $(B)/client/kiss_fft.o \
  $(B)/client/kiss_fftr.o \
  $(B)/client/lpc.o \
  $(B)/client/lsp.o \
  $(B)/client/lsp_tables_nb.o \
  $(B)/client/ltp.o \
  $(B)/client/mdf.o \
  $(B)/client/modes.o \
  $(B)/client/modes_wb.o \
  $(B)/client/nb_celp.o \
  $(B)/client/preprocess.o \
  $(B)/client/quant_lsp.o \
  $(B)/client/resample.o \
  $(B)/client/sb_celp.o \
  $(B)/client/smallft.o \
  $(B)/client/speex.o \
  $(B)/client/speex_callbacks.o \
  $(B)/client/speex_header.o \
  $(B)/client/stereo.o \
  $(B)/client/vbr.o \
  $(B)/client/vq.o \
  $(B)/client/window.o
endif
endif

ifeq ($(USE_INTERNAL_ZLIB),1)
Q3OBJ += \
  $(B)/client/adler32.o \
  $(B)/client/crc32.o \
  $(B)/client/inffast.o \
  $(B)/client/inflate.o \
  $(B)/client/inftrees.o \
  $(B)/client/zutil.o
endif

ifeq ($(HAVE_VM_COMPILED),true)
  ifeq ($(ARCH),i386)
    Q3OBJ += \
      $(B)/client/vm_x86.o
  endif
  ifeq ($(ARCH),x86)
    Q3OBJ += \
      $(B)/client/vm_x86.o
  endif
  ifeq ($(ARCH),x86_64)
    ifeq ($(USE_OLD_VM64),1)
      Q3OBJ += \
        $(B)/client/vm_x86_64.o \
        $(B)/client/vm_x86_64_assembler.o
    else
      Q3OBJ += \
        $(B)/client/vm_x86.o
    endif
  endif
  ifeq ($(ARCH),amd64)
    ifeq ($(USE_OLD_VM64),1)
      Q3OBJ += \
        $(B)/client/vm_x86_64.o \
        $(B)/client/vm_x86_64_assembler.o
    else
      Q3OBJ += \
        $(B)/client/vm_x86.o
    endif
  endif
  ifeq ($(ARCH),x64)
    ifeq ($(USE_OLD_VM64),1)
      Q3OBJ += \
        $(B)/client/vm_x86_64.o \
        $(B)/client/vm_x86_64_assembler.o
    else
      Q3OBJ += \
        $(B)/client/vm_x86.o
    endif
  endif
  ifeq ($(ARCH),ppc)
    Q3OBJ += $(B)/client/vm_powerpc.o $(B)/client/vm_powerpc_asm.o
  endif
  ifeq ($(ARCH),ppc64)
    Q3OBJ += $(B)/client/vm_powerpc.o $(B)/client/vm_powerpc_asm.o
  endif
  ifeq ($(ARCH),sparc)
    Q3OBJ += $(B)/client/vm_sparc.o
  endif
endif

ifeq ($(USE_LLVM),1)
  Q3OBJ += $(B)/client/vm_llvm.o
endif

ifeq ($(PLATFORM),mingw32)
  Q3OBJ += \
    $(B)/client/win_resource.o \
    $(B)/client/sys_win32.o
else
  Q3OBJ += \
    $(B)/client/sys_unix.o
endif

ifeq ($(PLATFORM),darwin)
  Q3OBJ += \
    $(B)/client/sys_osx.o
endif

ifeq ($(USE_MUMBLE),1)
  Q3OBJ += \
    $(B)/client/libmumblelink.o
endif

Q3POBJ += \
  $(B)/renderer/sdl_glimp.o

Q3POBJ_SMP += \
  $(B)/renderersmp/sdl_glimp.o

ifneq ($(USE_RENDERER_DLOPEN),0)
$(B)/$(CLIENTBIN)$(FULLBINEXT): $(Q3OBJ) $(LIBSDLMAIN)
	$(echo_cmd) "LD $@"
	$(Q)$(LD) $(CLIENT_CFLAGS) $(CFLAGS) $(CLIENT_LDFLAGS) $(LDFLAGS) \
		-o $@ $(Q3OBJ) \
		$(LIBSDLMAIN) $(CLIENT_LIBS) $(LIBS)

$(B)/renderer_opengl1_$(SHLIBNAME): $(Q3ROBJ) $(Q3POBJ)
	$(echo_cmd) "LD $@"
	$(Q)$(LD) $(CFLAGS) $(SHLIBLDFLAGS) -o $@ $(Q3ROBJ) $(Q3POBJ) \
		$(THREAD_LIBS) $(LIBSDLMAIN) $(RENDERER_LIBS) $(LIBS)

$(B)/renderer_opengl1_smp_$(SHLIBNAME): $(Q3ROBJ) $(Q3POBJ_SMP)
	$(echo_cmd) "LD $@"
	$(Q)$(LD) $(CFLAGS) $(SHLIBLDFLAGS) -o $@ $(Q3ROBJ) $(Q3POBJ_SMP) \
		$(THREAD_LIBS) $(LIBSDLMAIN) $(RENDERER_LIBS) $(LIBS)
else
$(B)/$(CLIENTBIN)$(FULLBINEXT): $(Q3OBJ) $(Q3ROBJ) $(Q3POBJ) $(LIBSDLMAIN)
	$(echo_cmd) "LD $@"
	$(Q)$(LD) $(CLIENT_CFLAGS) $(CFLAGS) $(CLIENT_LDFLAGS) $(LDFLAGS) \
		-o $@ $(Q3OBJ) $(Q3ROBJ) $(Q3POBJ) \
		$(LIBSDLMAIN) $(CLIENT_LIBS) $(RENDERER_LIBS) $(LIBS)

$(B)/$(CLIENTBIN)-smp$(FULLBINEXT): $(Q3OBJ) $(Q3ROBJ) $(Q3POBJ_SMP) $(LIBSDLMAIN)
	$(echo_cmd) "LD $@"
	$(Q)$(LD) $(CLIENT_CFLAGS) $(CFLAGS) $(CLIENT_LDFLAGS) $(LDFLAGS) $(THREAD_LDFLAGS) \
		-o $@ $(Q3OBJ) $(Q3ROBJ) $(Q3POBJ_SMP) \
		$(THREAD_LIBS) $(LIBSDLMAIN) $(CLIENT_LIBS) $(RENDERER_LIBS) $(LIBS)
endif

ifneq ($(strip $(LIBSDLMAIN)),)
ifneq ($(strip $(LIBSDLMAINSRC)),)
$(LIBSDLMAIN) : $(LIBSDLMAINSRC)
	cp $< $@
	ranlib $@
endif
endif



#############################################################################
# DEDICATED SERVER
#############################################################################

Q3DOBJ = \
  $(B)/ded/sv_bot.o \
  $(B)/ded/sv_client.o \
  $(B)/ded/sv_ccmds.o \
  $(B)/ded/sv_game.o \
  $(B)/ded/sv_init.o \
  $(B)/ded/sv_main.o \
  $(B)/ded/sv_net_chan.o \
  $(B)/ded/sv_snapshot.o \
  $(B)/ded/sv_world.o \
  \
  $(B)/ded/cm_load.o \
  $(B)/ded/cm_patch.o \
  $(B)/ded/cm_polylib.o \
  $(B)/ded/cm_test.o \
  $(B)/ded/cm_trace.o \
  $(B)/ded/cmd.o \
  $(B)/ded/common.o \
  $(B)/ded/cvar.o \
  $(B)/ded/files.o \
  $(B)/ded/md4.o \
  $(B)/ded/msg.o \
  $(B)/ded/net_chan.o \
  $(B)/ded/net_ip.o \
  $(B)/ded/huffman.o \
  \
  $(B)/ded/q_math.o \
  $(B)/ded/q_shared.o \
  \
  $(B)/ded/unzip.o \
  $(B)/ded/ioapi.o \
  $(B)/ded/vm.o \
  $(B)/ded/vm_interpreted.o \
  \
  $(B)/ded/be_aas_bspq3.o \
  $(B)/ded/be_aas_cluster.o \
  $(B)/ded/be_aas_debug.o \
  $(B)/ded/be_aas_entity.o \
  $(B)/ded/be_aas_file.o \
  $(B)/ded/be_aas_main.o \
  $(B)/ded/be_aas_move.o \
  $(B)/ded/be_aas_optimize.o \
  $(B)/ded/be_aas_reach.o \
  $(B)/ded/be_aas_route.o \
  $(B)/ded/be_aas_routealt.o \
  $(B)/ded/be_aas_sample.o \
  $(B)/ded/be_ai_char.o \
  $(B)/ded/be_ai_chat.o \
  $(B)/ded/be_ai_gen.o \
  $(B)/ded/be_ai_goal.o \
  $(B)/ded/be_ai_move.o \
  $(B)/ded/be_ai_weap.o \
  $(B)/ded/be_ai_weight.o \
  $(B)/ded/be_ea.o \
  $(B)/ded/be_interface.o \
  $(B)/ded/l_crc.o \
  $(B)/ded/l_libvar.o \
  $(B)/ded/l_log.o \
  $(B)/ded/l_memory.o \
  $(B)/ded/l_precomp.o \
  $(B)/ded/l_script.o \
  $(B)/ded/l_struct.o \
  \
  $(B)/ded/null_client.o \
  $(B)/ded/null_input.o \
  $(B)/ded/null_snddma.o \
  \
  $(B)/ded/con_log.o \
  $(B)/ded/sys_main.o

ifeq ($(ARCH),i386)
  Q3DOBJ += \
      $(B)/ded/matha.o \
      $(B)/ded/snapvector.o \
      $(B)/ded/ftola.o
endif
ifeq ($(ARCH),x86)
  Q3DOBJ += \
      $(B)/ded/matha.o \
      $(B)/ded/snapvector.o \
      $(B)/ded/ftola.o 
endif
ifeq ($(ARCH),x86_64)
  Q3DOBJ += \
      $(B)/ded/snapvector.o \
      $(B)/ded/ftola.o 
endif
ifeq ($(ARCH),amd64)
  Q3DOBJ += \
      $(B)/ded/snapvector.o \
      $(B)/ded/ftola.o 
endif
ifeq ($(ARCH),x64)
  Q3DOBJ += \
      $(B)/ded/snapvector.o \
      $(B)/ded/ftola.o 
endif

ifeq ($(USE_INTERNAL_ZLIB),1)
Q3DOBJ += \
  $(B)/ded/adler32.o \
  $(B)/ded/crc32.o \
  $(B)/ded/inffast.o \
  $(B)/ded/inflate.o \
  $(B)/ded/inftrees.o \
  $(B)/ded/zutil.o
endif

ifeq ($(HAVE_VM_COMPILED),true)
  ifeq ($(ARCH),i386)
    Q3DOBJ += \
      $(B)/ded/vm_x86.o
  endif
  ifeq ($(ARCH),x86)
    Q3DOBJ += \
      $(B)/ded/vm_x86.o
  endif
  ifeq ($(ARCH),x86_64)
    ifeq ($(USE_OLD_VM64),1)
      Q3DOBJ += \
        $(B)/ded/vm_x86_64.o \
        $(B)/ded/vm_x86_64_assembler.o
    else
      Q3DOBJ += \
        $(B)/ded/vm_x86.o
    endif
  endif
  ifeq ($(ARCH),amd64)
    ifeq ($(USE_OLD_VM64),1)
      Q3DOBJ += \
        $(B)/ded/vm_x86_64.o \
        $(B)/ded/vm_x86_64_assembler.o
    else
      Q3DOBJ += \
        $(B)/ded/vm_x86.o
    endif
  endif
  ifeq ($(ARCH),x64)
    ifeq ($(USE_OLD_VM64),1)
      Q3DOBJ += \
        $(B)/ded/vm_x86_64.o \
        $(B)/ded/vm_x86_64_assembler.o
    else
      Q3DOBJ += \
        $(B)/ded/vm_x86.o
    endif
  endif
  ifeq ($(ARCH),ppc)
    Q3DOBJ += $(B)/ded/vm_powerpc.o $(B)/ded/vm_powerpc_asm.o
  endif
  ifeq ($(ARCH),ppc64)
    Q3DOBJ += $(B)/ded/vm_powerpc.o $(B)/ded/vm_powerpc_asm.o
  endif
  ifeq ($(ARCH),sparc)
    Q3DOBJ += $(B)/ded/vm_sparc.o
  endif
endif

ifeq ($(USE_LLVM),1)
  Q3DOBJ += $(B)/ded/vm_llvm.o
endif

ifeq ($(PLATFORM),mingw32)
  Q3DOBJ += \
    $(B)/ded/win_resource.o \
    $(B)/ded/sys_win32.o \
    $(B)/ded/con_win32.o
else
  Q3DOBJ += \
    $(B)/ded/sys_unix.o \
    $(B)/ded/con_tty.o
endif

ifeq ($(PLATFORM),darwin)
  Q3DOBJ += \
    $(B)/ded/sys_osx.o
endif

$(B)/$(SERVERBIN)$(FULLBINEXT): $(Q3DOBJ)
	$(echo_cmd) "LD $@"
	$(Q)$(LD) $(CFLAGS) $(LDFLAGS) -o $@ $(Q3DOBJ) $(LIBS)



#############################################################################
## BASEGAME CGAME
#############################################################################

Q3CGOBJ_ = \
  $(B)/$(BASEGAME)/cgame/cg_main.o \
  $(B)/$(BASEGAME)/cgame/bg_misc.o \
  $(B)/$(BASEGAME)/cgame/bg_pmove.o \
  $(B)/$(BASEGAME)/cgame/bg_slidemove.o \
  $(B)/$(BASEGAME)/cgame/bg_lib.o \
  $(B)/$(BASEGAME)/cgame/cg_consolecmds.o \
  $(B)/$(BASEGAME)/cgame/cg_newdraw.o \
  $(B)/$(BASEGAME)/cgame/cg_draw.o \
  $(B)/$(BASEGAME)/cgame/cg_drawtools.o \
  $(B)/$(BASEGAME)/cgame/cg_effects.o \
  $(B)/$(BASEGAME)/cgame/cg_ents.o \
  $(B)/$(BASEGAME)/cgame/cg_event.o \
  $(B)/$(BASEGAME)/cgame/cg_info.o \
  $(B)/$(BASEGAME)/cgame/cg_localents.o \
  $(B)/$(BASEGAME)/cgame/cg_marks.o \
  $(B)/$(BASEGAME)/cgame/cg_particles.o \
  $(B)/$(BASEGAME)/cgame/cg_players.o \
  $(B)/$(BASEGAME)/cgame/cg_playerstate.o \
  $(B)/$(BASEGAME)/cgame/cg_predict.o \
  $(B)/$(BASEGAME)/cgame/cg_scoreboard.o \
  $(B)/$(BASEGAME)/cgame/cg_servercmds.o \
  $(B)/$(BASEGAME)/cgame/cg_snapshot.o \
  $(B)/$(BASEGAME)/cgame/cg_view.o \
  $(B)/$(BASEGAME)/cgame/cg_weapons.o \
  \
  $(B)/$(BASEGAME)/qcommon/q_math.o \
  $(B)/$(BASEGAME)/qcommon/q_shared.o

Q3CGOBJ = $(Q3CGOBJ_) $(B)/$(BASEGAME)/cgame/cg_syscalls.o
Q3CGVMOBJ = $(Q3CGOBJ_:%.o=%.asm)
Q3CGLLVMOBJ = $(Q3CGOBJ:%.o=%.llvmo)

$(B)/$(BASEGAME)/cgame$(SHLIBNAME): $(Q3CGOBJ)
	$(echo_cmd) "LD $@"
	$(Q)$(LD) $(CFLAGS) $(SHLIBLDFLAGS) -o $@ $(Q3CGOBJ)

$(B)/$(BASEGAME)/vm/cgame.qvm: $(Q3CGVMOBJ) $(CGDIR)/cg_syscalls.asm $(Q3ASM)
	$(echo_cmd) "Q3ASM $@"
	$(Q)$(Q3ASM) -o $@ $(Q3CGVMOBJ) $(CGDIR)/cg_syscalls.asm

$(B)/$(BASEGAME)/cgamellvm.bc: $(Q3CGLLVMOBJ) $(LLVMLINK)
	$(echo_cmd) "LLVMLD $@"
	$(Q)$(LLVMLD) $(LLVMLDFLAGS) -o $@ $(Q3CGLLVMOBJ)

#############################################################################
## MISSIONPACK CGAME
#############################################################################

MPCGOBJ_ = \
  $(B)/$(MISSIONPACK)/cgame/cg_main.o \
  $(B)/$(MISSIONPACK)/cgame/bg_misc.o \
  $(B)/$(MISSIONPACK)/cgame/bg_pmove.o \
  $(B)/$(MISSIONPACK)/cgame/bg_slidemove.o \
  $(B)/$(MISSIONPACK)/cgame/bg_lib.o \
  $(B)/$(MISSIONPACK)/cgame/cg_consolecmds.o \
  $(B)/$(MISSIONPACK)/cgame/cg_newdraw.o \
  $(B)/$(MISSIONPACK)/cgame/cg_draw.o \
  $(B)/$(MISSIONPACK)/cgame/cg_drawtools.o \
  $(B)/$(MISSIONPACK)/cgame/cg_effects.o \
  $(B)/$(MISSIONPACK)/cgame/cg_ents.o \
  $(B)/$(MISSIONPACK)/cgame/cg_event.o \
  $(B)/$(MISSIONPACK)/cgame/cg_info.o \
  $(B)/$(MISSIONPACK)/cgame/cg_localents.o \
  $(B)/$(MISSIONPACK)/cgame/cg_marks.o \
  $(B)/$(MISSIONPACK)/cgame/cg_particles.o \
  $(B)/$(MISSIONPACK)/cgame/cg_players.o \
  $(B)/$(MISSIONPACK)/cgame/cg_playerstate.o \
  $(B)/$(MISSIONPACK)/cgame/cg_predict.o \
  $(B)/$(MISSIONPACK)/cgame/cg_scoreboard.o \
  $(B)/$(MISSIONPACK)/cgame/cg_servercmds.o \
  $(B)/$(MISSIONPACK)/cgame/cg_snapshot.o \
  $(B)/$(MISSIONPACK)/cgame/cg_view.o \
  $(B)/$(MISSIONPACK)/cgame/cg_weapons.o \
  $(B)/$(MISSIONPACK)/ui/ui_shared.o \
  \
  $(B)/$(MISSIONPACK)/qcommon/q_math.o \
  $(B)/$(MISSIONPACK)/qcommon/q_shared.o

MPCGOBJ = $(MPCGOBJ_) $(B)/$(MISSIONPACK)/cgame/cg_syscalls.o
MPCGVMOBJ = $(MPCGOBJ_:%.o=%.asm)
MPCGLLVMOBJ = $(MPCGOBJ:%.o=%.llvmo)

$(B)/$(MISSIONPACK)/cgame$(SHLIBNAME): $(MPCGOBJ)
	$(echo_cmd) "LD $@"
	$(Q)$(LD) $(CFLAGS) $(SHLIBLDFLAGS) -o $@ $(MPCGOBJ)

$(B)/$(MISSIONPACK)/vm/cgame.qvm: $(MPCGVMOBJ) $(CGDIR)/cg_syscalls.asm $(Q3ASM)
	$(echo_cmd) "Q3ASM $@"
	$(Q)$(Q3ASM) -o $@ $(MPCGVMOBJ) $(CGDIR)/cg_syscalls.asm

$(B)/$(MISSIONPACK)/cgamellvm.bc: $(MPCGLLVMOBJ) $(LLVMLINK)
	$(echo_cmd) "LLVMLD $@"
	$(Q)$(LLVMLD) $(LLVMLDFLAGS) -o $@ $(MPCGLLVMOBJ)



#############################################################################
## BASEGAME GAME
#############################################################################

Q3GOBJ_ = \
  $(B)/$(BASEGAME)/game/g_main.o \
  $(B)/$(BASEGAME)/game/ai_chat.o \
  $(B)/$(BASEGAME)/game/ai_cmd.o \
  $(B)/$(BASEGAME)/game/ai_dmnet.o \
  $(B)/$(BASEGAME)/game/ai_dmq3.o \
  $(B)/$(BASEGAME)/game/ai_main.o \
  $(B)/$(BASEGAME)/game/ai_team.o \
  $(B)/$(BASEGAME)/game/ai_vcmd.o \
  $(B)/$(BASEGAME)/game/bg_misc.o \
  $(B)/$(BASEGAME)/game/bg_pmove.o \
  $(B)/$(BASEGAME)/game/bg_slidemove.o \
  $(B)/$(BASEGAME)/game/bg_lib.o \
  $(B)/$(BASEGAME)/game/g_active.o \
  $(B)/$(BASEGAME)/game/g_arenas.o \
  $(B)/$(BASEGAME)/game/g_bot.o \
  $(B)/$(BASEGAME)/game/g_client.o \
  $(B)/$(BASEGAME)/game/g_cmds.o \
  $(B)/$(BASEGAME)/game/g_combat.o \
  $(B)/$(BASEGAME)/game/g_items.o \
  $(B)/$(BASEGAME)/game/g_mem.o \
  $(B)/$(BASEGAME)/game/g_misc.o \
  $(B)/$(BASEGAME)/game/g_missile.o \
  $(B)/$(BASEGAME)/game/g_mover.o \
  $(B)/$(BASEGAME)/game/g_session.o \
  $(B)/$(BASEGAME)/game/g_spawn.o \
  $(B)/$(BASEGAME)/game/g_svcmds.o \
  $(B)/$(BASEGAME)/game/g_target.o \
  $(B)/$(BASEGAME)/game/g_team.o \
  $(B)/$(BASEGAME)/game/g_trigger.o \
  $(B)/$(BASEGAME)/game/g_utils.o \
  $(B)/$(BASEGAME)/game/g_weapon.o \
  \
  $(B)/$(BASEGAME)/qcommon/q_math.o \
  $(B)/$(BASEGAME)/qcommon/q_shared.o

Q3GOBJ = $(Q3GOBJ_) $(B)/$(BASEGAME)/game/g_syscalls.o
Q3GVMOBJ = $(Q3GOBJ_:%.o=%.asm)
Q3GLLVMOBJ = $(Q3GOBJ:%.o=%.llvmo)

$(B)/$(BASEGAME)/qagame$(SHLIBNAME): $(Q3GOBJ)
	$(echo_cmd) "LD $@"
	$(Q)$(LD) $(CFLAGS) $(SHLIBLDFLAGS) -o $@ $(Q3GOBJ)

$(B)/$(BASEGAME)/vm/qagame.qvm: $(Q3GVMOBJ) $(GDIR)/g_syscalls.asm $(Q3ASM)
	$(echo_cmd) "Q3ASM $@"
	$(Q)$(Q3ASM) -o $@ $(Q3GVMOBJ) $(GDIR)/g_syscalls.asm

$(B)/$(BASEGAME)/qagamellvm.bc: $(Q3GLLVMOBJ) $(LLVMLINK)
	$(echo_cmd) "LLVMLD $@"
	$(Q)$(LLVMLD) $(LLVMLDFLAGS) -o $@ $(Q3GLLVMOBJ)

#############################################################################
## MISSIONPACK GAME
#############################################################################

MPGOBJ_ = \
  $(B)/$(MISSIONPACK)/game/g_main.o \
  $(B)/$(MISSIONPACK)/game/ai_chat.o \
  $(B)/$(MISSIONPACK)/game/ai_cmd.o \
  $(B)/$(MISSIONPACK)/game/ai_dmnet.o \
  $(B)/$(MISSIONPACK)/game/ai_dmq3.o \
  $(B)/$(MISSIONPACK)/game/ai_main.o \
  $(B)/$(MISSIONPACK)/game/ai_team.o \
  $(B)/$(MISSIONPACK)/game/ai_vcmd.o \
  $(B)/$(MISSIONPACK)/game/bg_misc.o \
  $(B)/$(MISSIONPACK)/game/bg_pmove.o \
  $(B)/$(MISSIONPACK)/game/bg_slidemove.o \
  $(B)/$(MISSIONPACK)/game/bg_lib.o \
  $(B)/$(MISSIONPACK)/game/g_active.o \
  $(B)/$(MISSIONPACK)/game/g_arenas.o \
  $(B)/$(MISSIONPACK)/game/g_bot.o \
  $(B)/$(MISSIONPACK)/game/g_client.o \
  $(B)/$(MISSIONPACK)/game/g_cmds.o \
  $(B)/$(MISSIONPACK)/game/g_combat.o \
  $(B)/$(MISSIONPACK)/game/g_items.o \
  $(B)/$(MISSIONPACK)/game/g_mem.o \
  $(B)/$(MISSIONPACK)/game/g_misc.o \
  $(B)/$(MISSIONPACK)/game/g_missile.o \
  $(B)/$(MISSIONPACK)/game/g_mover.o \
  $(B)/$(MISSIONPACK)/game/g_session.o \
  $(B)/$(MISSIONPACK)/game/g_spawn.o \
  $(B)/$(MISSIONPACK)/game/g_svcmds.o \
  $(B)/$(MISSIONPACK)/game/g_target.o \
  $(B)/$(MISSIONPACK)/game/g_team.o \
  $(B)/$(MISSIONPACK)/game/g_trigger.o \
  $(B)/$(MISSIONPACK)/game/g_utils.o \
  $(B)/$(MISSIONPACK)/game/g_weapon.o \
  \
  $(B)/$(MISSIONPACK)/qcommon/q_math.o \
  $(B)/$(MISSIONPACK)/qcommon/q_shared.o

MPGOBJ = $(MPGOBJ_) $(B)/$(MISSIONPACK)/game/g_syscalls.o
MPGVMOBJ = $(MPGOBJ_:%.o=%.asm)
MPGLLVMOBJ = $(MPGOBJ:%.o=%.llvmo)

$(B)/$(MISSIONPACK)/qagame$(SHLIBNAME): $(MPGOBJ)
	$(echo_cmd) "LD $@"
	$(Q)$(LD) $(CFLAGS) $(SHLIBLDFLAGS) -o $@ $(MPGOBJ)

$(B)/$(MISSIONPACK)/vm/qagame.qvm: $(MPGVMOBJ) $(GDIR)/g_syscalls.asm $(Q3ASM)
	$(echo_cmd) "Q3ASM $@"
	$(Q)$(Q3ASM) -o $@ $(MPGVMOBJ) $(GDIR)/g_syscalls.asm

$(B)/$(MISSIONPACK)/qagamellvm.bc: $(MPGLLVMOBJ) $(LLVMLINK)
	$(echo_cmd) "LLVMLD $@"
	$(Q)$(LLVMLD) $(LLVMLDFLAGS) -o $@ $(MPGLLVMOBJ)



#############################################################################
## BASEGAME UI
#############################################################################

Q3UIOBJ_ = \
  $(B)/$(BASEGAME)/ui/ui_main.o \
  $(B)/$(BASEGAME)/ui/bg_misc.o \
  $(B)/$(BASEGAME)/ui/bg_lib.o \
  $(B)/$(BASEGAME)/ui/ui_addbots.o \
  $(B)/$(BASEGAME)/ui/ui_atoms.o \
  $(B)/$(BASEGAME)/ui/ui_cinematics.o \
  $(B)/$(BASEGAME)/ui/ui_confirm.o \
  $(B)/$(BASEGAME)/ui/ui_connect.o \
  $(B)/$(BASEGAME)/ui/ui_controls2.o \
  $(B)/$(BASEGAME)/ui/ui_credits.o \
  $(B)/$(BASEGAME)/ui/ui_demo2.o \
  $(B)/$(BASEGAME)/ui/ui_display.o \
  $(B)/$(BASEGAME)/ui/ui_gameinfo.o \
  $(B)/$(BASEGAME)/ui/ui_ingame.o \
  $(B)/$(BASEGAME)/ui/ui_ingame_selectplayer.o \
  $(B)/$(BASEGAME)/ui/ui_joystick.o \
  $(B)/$(BASEGAME)/ui/ui_loadconfig.o \
  $(B)/$(BASEGAME)/ui/ui_menu.o \
  $(B)/$(BASEGAME)/ui/ui_mfield.o \
  $(B)/$(BASEGAME)/ui/ui_mods.o \
  $(B)/$(BASEGAME)/ui/ui_network.o \
  $(B)/$(BASEGAME)/ui/ui_options.o \
  $(B)/$(BASEGAME)/ui/ui_playermodel.o \
  $(B)/$(BASEGAME)/ui/ui_players.o \
  $(B)/$(BASEGAME)/ui/ui_playersettings.o \
  $(B)/$(BASEGAME)/ui/ui_preferences.o \
  $(B)/$(BASEGAME)/ui/ui_qmenu.o \
  $(B)/$(BASEGAME)/ui/ui_removebots.o \
  $(B)/$(BASEGAME)/ui/ui_saveconfig.o \
  $(B)/$(BASEGAME)/ui/ui_selectplayer.o \
  $(B)/$(BASEGAME)/ui/ui_serverinfo.o \
  $(B)/$(BASEGAME)/ui/ui_servers2.o \
  $(B)/$(BASEGAME)/ui/ui_setup.o \
  $(B)/$(BASEGAME)/ui/ui_sound.o \
  $(B)/$(BASEGAME)/ui/ui_sparena.o \
  $(B)/$(BASEGAME)/ui/ui_specifyserver.o \
  $(B)/$(BASEGAME)/ui/ui_splevel.o \
  $(B)/$(BASEGAME)/ui/ui_sppostgame.o \
  $(B)/$(BASEGAME)/ui/ui_spskill.o \
  $(B)/$(BASEGAME)/ui/ui_startserver.o \
  $(B)/$(BASEGAME)/ui/ui_team.o \
  $(B)/$(BASEGAME)/ui/ui_teamorders.o \
  $(B)/$(BASEGAME)/ui/ui_video.o \
  \
  $(B)/$(BASEGAME)/qcommon/q_math.o \
  $(B)/$(BASEGAME)/qcommon/q_shared.o

Q3UIOBJ = $(Q3UIOBJ_) $(B)/$(MISSIONPACK)/ui/ui_syscalls.o
Q3UIVMOBJ = $(Q3UIOBJ_:%.o=%.asm)
Q3UILLVMOBJ = $(Q3UIOBJ:%.o=%.llvmo)

$(B)/$(BASEGAME)/ui$(SHLIBNAME): $(Q3UIOBJ)
	$(echo_cmd) "LD $@"
	$(Q)$(LD) $(CFLAGS) $(SHLIBLDFLAGS) -o $@ $(Q3UIOBJ)

$(B)/$(BASEGAME)/vm/ui.qvm: $(Q3UIVMOBJ) $(UIDIR)/ui_syscalls.asm $(Q3ASM)
	$(echo_cmd) "Q3ASM $@"
	$(Q)$(Q3ASM) -o $@ $(Q3UIVMOBJ) $(UIDIR)/ui_syscalls.asm

$(B)/$(BASEGAME)/uillvm.bc: $(Q3UILLVMOBJ) $(LLVMLINK)
	$(echo_cmd) "LLVMLD $@"
	$(Q)$(LLVMLD) $(LLVMLDFLAGS) -o $@ $(Q3UILLVMOBJ)

#############################################################################
## MISSIONPACK UI
#############################################################################

MPUIOBJ_ = \
  $(B)/$(MISSIONPACK)/ui/ui_main.o \
  $(B)/$(MISSIONPACK)/ui/ui_atoms.o \
  $(B)/$(MISSIONPACK)/ui/ui_gameinfo.o \
  $(B)/$(MISSIONPACK)/ui/ui_players.o \
  $(B)/$(MISSIONPACK)/ui/ui_shared.o \
  \
  $(B)/$(MISSIONPACK)/ui/bg_misc.o \
  $(B)/$(MISSIONPACK)/ui/bg_lib.o \
  \
  $(B)/$(MISSIONPACK)/qcommon/q_math.o \
  $(B)/$(MISSIONPACK)/qcommon/q_shared.o

MPUIOBJ = $(MPUIOBJ_) $(B)/$(MISSIONPACK)/ui/ui_syscalls.o
MPUIVMOBJ = $(MPUIOBJ_:%.o=%.asm)
MPUILLVMOBJ = $(MPUIOBJ_:%.o=%.llvmo)

$(B)/$(MISSIONPACK)/ui$(SHLIBNAME): $(MPUIOBJ)
	$(echo_cmd) "LD $@"
	$(Q)$(LD) $(CFLAGS) $(SHLIBLDFLAGS) -o $@ $(MPUIOBJ)

$(B)/$(MISSIONPACK)/vm/ui.qvm: $(MPUIVMOBJ) $(UIDIR)/ui_syscalls.asm $(Q3ASM)
	$(echo_cmd) "Q3ASM $@"
	$(Q)$(Q3ASM) -o $@ $(MPUIVMOBJ) $(UIDIR)/ui_syscalls.asm

$(B)/$(MISSIONPACK)/uillvm.bc: $(MPUILLVMOBJ) $(LLVMLINK)
	$(echo_cmd) "LLVMLD $@"
	$(Q)$(LLVMLD) $(LLVMLDFLAGS) -o $@ $(MPUILLVMOBJ)


#############################################################################
## CLIENT/SERVER RULES
#############################################################################

$(B)/client/%.o: $(ASMDIR)/%.s
	$(DO_AS)

# k8 so inline assembler knows about SSE
$(B)/client/%.o: $(ASMDIR)/%.c
	$(DO_CC) -march=k8

$(B)/client/%.o: $(CDIR)/%.c
	$(DO_CC)

$(B)/client/%.o: $(SDIR)/%.c
	$(DO_CC)

$(B)/client/%.o: $(CMDIR)/%.c
	$(DO_CC)

$(B)/client/%.o: $(BLIBDIR)/%.c
	$(DO_BOT_CC)

$(B)/client/%.o: $(SPEEXDIR)/%.c
	$(DO_CC)

$(B)/client/%.o: $(ZDIR)/%.c
	$(DO_CC)

$(B)/client/%.o: $(SDLDIR)/%.c
	$(DO_CC)

$(B)/renderersmp/%.o: $(SDLDIR)/%.c
	$(DO_SMP_CC)

$(B)/client/%.o: $(SYSDIR)/%.c
	$(DO_CC)

$(B)/client/%.o: $(SYSDIR)/%.m
	$(DO_CC)

$(B)/client/%.o: $(SYSDIR)/%.rc
	$(DO_WINDRES)

$(B)/client/vm_llvm.o: $(CMDIR)/vm_llvm.cpp
	$(DO_CXX)


$(B)/renderer/%.o: $(CMDIR)/%.c
	$(DO_REF_CC)

$(B)/renderer/%.o: $(SDLDIR)/%.c
	$(DO_REF_CC)

$(B)/renderer/%.o: $(JPDIR)/%.c
	$(DO_REF_CC)

$(B)/renderer/%.o: $(RDIR)/%.c
	$(DO_REF_CC)

$(B)/renderer/%.o: $(FTDIR)/src/base/%.c
	$(DO_REF_CC)

$(B)/renderer/ftbzip2.o: $(FTDIR)/src/bzip2/ftbzip2.c
	$(DO_REF_CC)

$(B)/renderer/ftgzip.o: $(FTDIR)/src/gzip/ftgzip.c
	$(DO_REF_CC)

$(B)/renderer/ftlzw.o: $(FTDIR)/src/lzw/ftlzw.c
	$(DO_REF_CC)

$(B)/renderer/autofit.o: $(FTDIR)/src/autofit/autofit.c
	$(DO_REF_CC)

$(B)/renderer/bdf.o: $(FTDIR)/src/bdf/bdf.c
	$(DO_REF_CC)

$(B)/renderer/cff.o: $(FTDIR)/src/cff/cff.c
	$(DO_REF_CC)

$(B)/renderer/pcf.o: $(FTDIR)/src/pcf/pcf.c
	$(DO_REF_CC)

$(B)/renderer/pfr.o: $(FTDIR)/src/pfr/pfr.c
	$(DO_REF_CC)

$(B)/renderer/psaux.o: $(FTDIR)/src/psaux/psaux.c
	$(DO_REF_CC)

$(B)/renderer/pshinter.o: $(FTDIR)/src/pshinter/pshinter.c
	$(DO_REF_CC)

$(B)/renderer/psnames.o: $(FTDIR)/src/psnames/psnames.c
	$(DO_REF_CC)

$(B)/renderer/raster.o: $(FTDIR)/src/raster/raster.c
	$(DO_REF_CC)

$(B)/renderer/sfnt.o: $(FTDIR)/src/sfnt/sfnt.c
	$(DO_REF_CC)

$(B)/renderer/smooth.o: $(FTDIR)/src/smooth/smooth.c
	$(DO_REF_CC)

$(B)/renderer/truetype.o: $(FTDIR)/src/truetype/truetype.c
	$(DO_REF_CC)

$(B)/renderer/type1.o: $(FTDIR)/src/type1/type1.c
	$(DO_REF_CC)

$(B)/renderer/type1cid.o: $(FTDIR)/src/cid/type1cid.c
	$(DO_REF_CC)

$(B)/renderer/type42.o: $(FTDIR)/src/type42/type42.c
	$(DO_REF_CC)

$(B)/renderer/winfnt.o: $(FTDIR)/src/winfonts/winfnt.c
	$(DO_REF_CC)

$(B)/ded/%.o: $(ASMDIR)/%.s
	$(DO_AS)

# k8 so inline assembler knows about SSE
$(B)/ded/%.o: $(ASMDIR)/%.c
	$(DO_CC) -march=k8

$(B)/ded/%.o: $(SDIR)/%.c
	$(DO_DED_CC)

$(B)/ded/%.o: $(CMDIR)/%.c
	$(DO_DED_CC)

$(B)/ded/%.o: $(ZDIR)/%.c
	$(DO_DED_CC)

$(B)/ded/%.o: $(BLIBDIR)/%.c
	$(DO_BOT_CC)

$(B)/ded/%.o: $(SYSDIR)/%.c
	$(DO_DED_CC)

$(B)/ded/%.o: $(SYSDIR)/%.m
	$(DO_DED_CC)

$(B)/ded/%.o: $(SYSDIR)/%.rc
	$(DO_WINDRES)

$(B)/ded/%.o: $(NDIR)/%.c
	$(DO_DED_CC)

$(B)/ded/vm_llvm.o: $(CMDIR)/vm_llvm.cpp
	$(DO_DED_CXX)

# Extra dependencies to ensure the SVN version is incorporated
ifeq ($(USE_SVN),1)
  $(B)/client/cl_console.o : .svn/entries
  $(B)/client/common.o : .svn/entries
  $(B)/ded/common.o : .svn/entries
endif

$(CMDIR)/vm_llvm.c:

#############################################################################
## GAME MODULE RULES
#############################################################################

$(B)/$(BASEGAME)/cgame/bg_%.o: $(GDIR)/bg_%.c
	$(DO_CGAME_CC)

$(B)/$(BASEGAME)/cgame/%.o: $(CGDIR)/%.c
	$(DO_CGAME_CC)

$(B)/$(BASEGAME)/cgame/bg_%.asm: $(GDIR)/bg_%.c $(Q3LCC)
	$(DO_CGAME_Q3LCC)

$(B)/$(BASEGAME)/cgame/%.asm: $(CGDIR)/%.c $(Q3LCC)
	$(DO_CGAME_Q3LCC)

$(B)/$(BASEGAME)/cgame/bg_%.llvmo: $(GDIR)/bg_%.c $(CLANG)
	$(DO_CGAME_LLVMCC)

$(B)/$(BASEGAME)/cgame/%.llvmo: $(CGDIR)/%.c $(CLANG)
	$(DO_CGAME_LLVMCC)

$(B)/$(MISSIONPACK)/cgame/bg_%.o: $(GDIR)/bg_%.c
	$(DO_CGAME_CC_MISSIONPACK)

$(B)/$(MISSIONPACK)/cgame/%.o: $(CGDIR)/%.c
	$(DO_CGAME_CC_MISSIONPACK)

$(B)/$(MISSIONPACK)/cgame/bg_%.asm: $(GDIR)/bg_%.c $(Q3LCC)
	$(DO_CGAME_Q3LCC_MISSIONPACK)

$(B)/$(MISSIONPACK)/cgame/%.asm: $(CGDIR)/%.c $(Q3LCC)
	$(DO_CGAME_Q3LCC_MISSIONPACK)

$(B)/$(MISSIONPACK)/cgame/bg_%.llvmo: $(GDIR)/bg_%.c $(CLANG)
	$(DO_CGAME_LLVMCC_MISSIONPACK)

$(B)/$(MISSIONPACK)/cgame/%.llvmo: $(CGDIR)/%.c $(CLANG)
	$(DO_CGAME_LLVMCC_MISSIONPACK)


$(B)/$(BASEGAME)/game/%.o: $(GDIR)/%.c
	$(DO_GAME_CC)

$(B)/$(BASEGAME)/game/%.asm: $(GDIR)/%.c $(Q3LCC)
	$(DO_GAME_Q3LCC)

$(B)/$(BASEGAME)/game/%.llvmo: $(GDIR)/%.c $(CLANG)
	$(DO_GAME_LLVMCC)

$(B)/$(MISSIONPACK)/game/%.o: $(GDIR)/%.c
	$(DO_GAME_CC_MISSIONPACK)

$(B)/$(MISSIONPACK)/game/%.asm: $(GDIR)/%.c $(Q3LCC)
	$(DO_GAME_Q3LCC_MISSIONPACK)

$(B)/$(MISSIONPACK)/game/%.llvmo: $(GDIR)/%.c $(CLANG)
	$(DO_GAME_LLVMCC_MISSIONPACK)

$(B)/$(BASEGAME)/ui/bg_%.o: $(GDIR)/bg_%.c
	$(DO_UI_CC)

$(B)/$(BASEGAME)/ui/%.o: $(Q3UIDIR)/%.c
	$(DO_UI_CC)

$(B)/$(BASEGAME)/ui/bg_%.asm: $(GDIR)/bg_%.c $(Q3LCC)
	$(DO_UI_Q3LCC)

$(B)/$(BASEGAME)/ui/%.asm: $(Q3UIDIR)/%.c $(Q3LCC)
	$(DO_UI_Q3LCC)

$(B)/$(BASEGAME)/ui/bg_%.llvmo: $(GDIR)/bg_%.c $(CLANG)
	$(DO_UI_LLVMCC)

$(B)/$(BASEGAME)/ui/%.llvmo: $(Q3UIDIR)/%.c $(CLANG)
	$(DO_UI_LLVMCC)

$(B)/$(MISSIONPACK)/ui/bg_%.o: $(GDIR)/bg_%.c
	$(DO_UI_CC_MISSIONPACK)

$(B)/$(MISSIONPACK)/ui/%.o: $(UIDIR)/%.c
	$(DO_UI_CC_MISSIONPACK)

$(B)/$(MISSIONPACK)/ui/bg_%.asm: $(GDIR)/bg_%.c $(Q3LCC)
	$(DO_UI_Q3LCC_MISSIONPACK)

$(B)/$(MISSIONPACK)/ui/%.asm: $(UIDIR)/%.c $(Q3LCC)
	$(DO_UI_Q3LCC_MISSIONPACK)

$(B)/$(MISSIONPACK)/ui/bg_%.llvmo: $(GDIR)/bg_%.c $(CLANG)
	$(DO_UI_LLVMCC_MISSIONPACK)

$(B)/$(MISSIONPACK)/ui/%.llvmo: $(UIDIR)/%.c $(CLANG)
	$(DO_UI_LLVMCC_MISSIONPACK)


$(B)/$(BASEGAME)/qcommon/%.o: $(CMDIR)/%.c
	$(DO_SHLIB_CC)

$(B)/$(BASEGAME)/qcommon/%.asm: $(CMDIR)/%.c $(Q3LCC)
	$(DO_Q3LCC)

$(B)/$(BASEGAME)/qcommon/%.llvmo: $(CMDIR)/%.c $(CLANG)
	$(DO_LLVMCC)

$(B)/$(MISSIONPACK)/qcommon/%.o: $(CMDIR)/%.c
	$(DO_SHLIB_CC_MISSIONPACK)

$(B)/$(MISSIONPACK)/qcommon/%.asm: $(CMDIR)/%.c $(Q3LCC)
	$(DO_Q3LCC_MISSIONPACK)

$(B)/$(MISSIONPACK)/qcommon/%.llvmo: $(CMDIR)/%.c $(CLANG)
	$(DO_LLVMCC_MISSIONPACK)


#############################################################################
# MISC
#############################################################################

OBJ = $(Q3OBJ) $(Q3POBJ) $(Q3POBJ_SMP) $(Q3ROBJ) $(Q3DOBJ) \
  $(MPGOBJ) $(Q3GOBJ) $(Q3CGOBJ) $(MPCGOBJ) $(Q3UIOBJ) $(MPUIOBJ) \
  $(MPGVMOBJ) $(Q3GVMOBJ) $(Q3CGVMOBJ) $(MPCGVMOBJ) $(Q3UIVMOBJ) $(MPUIVMOBJ) \
  $(MPGLLVMOBJ) $(Q3GLLVMOBJ) $(Q3CGLLVMOBJ) $(MPCGLLVMOBJ) $(Q3UILLVMOBJ) $(MPUILLVMOBJ)
TOOLSOBJ = $(LBURGOBJ) $(Q3CPPOBJ) $(Q3RCCOBJ) $(Q3LCCOBJ) $(Q3ASMOBJ) $(CLANGOBJ) $(LLVMLINKOBJ)


copyfiles: release
ifneq ($(BUILD_GAME_SO),0)
  ifneq ($(BUILD_BASEGAME),0)
	-$(MKDIR) -p -m 0755 $(COPYDIR)/$(BASEGAME)
  endif
  ifneq ($(BUILD_MISSIONPACK),0)
	-$(MKDIR) -p -m 0755 $(COPYDIR)/$(MISSIONPACK)
  endif
endif

ifneq ($(BUILD_CLIENT),0)
	$(INSTALL) $(STRIP_FLAG) -m 0755 $(BR)/$(CLIENTBIN)$(FULLBINEXT) $(COPYBINDIR)/$(CLIENTBIN)$(FULLBINEXT)
  ifneq ($(USE_RENDERER_DLOPEN),0)
	$(INSTALL) $(STRIP_FLAG) -m 0755 $(BR)/renderer_opengl1_$(SHLIBNAME) $(COPYBINDIR)/renderer_opengl1_$(SHLIBNAME)
  endif
endif

# Don't copy the SMP until it's working together with SDL.
ifneq ($(BUILD_CLIENT_SMP),0)
  ifneq ($(USE_RENDERER_DLOPEN),0)
	$(INSTALL) $(STRIP_FLAG) -m 0755 $(BR)/renderer_opengl1_smp_$(SHLIBNAME) $(COPYBINDIR)/renderer_opengl1_smp_$(SHLIBNAME)
  else
	$(INSTALL) $(STRIP_FLAG) -m 0755 $(BR)/$(CLIENTBIN)-smp$(FULLBINEXT) $(COPYBINDIR)/$(CLIENTBIN)-smp$(FULLBINEXT)
  endif
endif

ifneq ($(BUILD_SERVER),0)
	@if [ -f $(BR)/$(SERVERBIN)$(FULLBINEXT) ]; then \
		$(INSTALL) $(STRIP_FLAG) -m 0755 $(BR)/$(SERVERBIN)$(FULLBINEXT) $(COPYBINDIR)/$(SERVERBIN)$(FULLBINEXT); \
	fi
endif

ifneq ($(BUILD_GAME_SO),0)
  ifneq ($(BUILD_BASEGAME),0)
	$(INSTALL) $(STRIP_FLAG) -m 0755 $(BR)/$(BASEGAME)/cgame$(SHLIBNAME) \
					$(COPYDIR)/$(BASEGAME)/.
	$(INSTALL) $(STRIP_FLAG) -m 0755 $(BR)/$(BASEGAME)/qagame$(SHLIBNAME) \
					$(COPYDIR)/$(BASEGAME)/.
	$(INSTALL) $(STRIP_FLAG) -m 0755 $(BR)/$(BASEGAME)/ui$(SHLIBNAME) \
					$(COPYDIR)/$(BASEGAME)/.
  endif
  ifneq ($(BUILD_MISSIONPACK),0)
	$(INSTALL) $(STRIP_FLAG) -m 0755 $(BR)/$(MISSIONPACK)/cgame$(SHLIBNAME) \
					$(COPYDIR)/$(MISSIONPACK)/.
	$(INSTALL) $(STRIP_FLAG) -m 0755 $(BR)/$(MISSIONPACK)/qagame$(SHLIBNAME) \
					$(COPYDIR)/$(MISSIONPACK)/.
	$(INSTALL) $(STRIP_FLAG) -m 0755 $(BR)/$(MISSIONPACK)/ui$(SHLIBNAME) \
					$(COPYDIR)/$(MISSIONPACK)/.
  endif
endif

clean: clean-debug clean-release
ifeq ($(PLATFORM),mingw32)
	@$(MAKE) -C $(NSISDIR) clean
else
	@$(MAKE) -C $(LOKISETUPDIR) clean
endif

clean-debug:
	@$(MAKE) clean2 B=$(BD)

clean-release:
	@$(MAKE) clean2 B=$(BR)

clean2:
	@echo "CLEAN $(B)"
	@rm -f $(OBJ)
	@rm -f $(OBJ_D_FILES)
	@rm -f $(TARGETS)

toolsclean: toolsclean-debug toolsclean-release

toolsclean-debug:
	@$(MAKE) toolsclean2 B=$(BD)

toolsclean-release:
	@$(MAKE) toolsclean2 B=$(BR)

toolsclean2:
	@echo "TOOLS_CLEAN $(B)"
	@rm -f $(TOOLSOBJ)
	@rm -f $(TOOLSOBJ_D_FILES)
	@rm -f $(LBURG) $(DAGCHECK_C) $(Q3RCC) $(Q3CPP) $(Q3LCC) $(Q3ASM) $(CLANG) $(LLVMLINK)

distclean: clean toolsclean
	@rm -rf $(BUILD_DIR)

installer: release
ifeq ($(PLATFORM),mingw32)
	@$(MAKE) VERSION=$(VERSION) -C $(NSISDIR) V=$(V) \
		SDLDLL=$(SDLDLL) \
		USE_RENDERER_DLOPEN=$(USE_RENDERER_DLOPEN) \
		USE_OPENAL_DLOPEN=$(USE_OPENAL_DLOPEN) \
		USE_CURL_DLOPEN=$(USE_CURL_DLOPEN) \
		USE_INTERNAL_SPEEX=$(USE_INTERNAL_SPEEX) \
		USE_INTERNAL_ZLIB=$(USE_INTERNAL_ZLIB) \
		USE_INTERNAL_JPEG=$(USE_INTERNAL_JPEG)
else
	@$(MAKE) VERSION=$(VERSION) -C $(LOKISETUPDIR) V=$(V)
endif

dist:
	rm -rf $(CLIENTBIN)-$(VERSION)
	svn export . $(CLIENTBIN)-$(VERSION)
	tar --owner=root --group=root --force-local -cjf $(CLIENTBIN)-$(VERSION).tar.bz2 $(CLIENTBIN)-$(VERSION)
	rm -rf $(CLIENTBIN)-$(VERSION)

#############################################################################
# DEPENDENCIES
#############################################################################

ifneq ($(B),)
  OBJ_D_FILES=$(filter %.d,$(OBJ:%.o=%.d))
  TOOLSOBJ_D_FILES=$(filter %.d,$(TOOLSOBJ:%.o=%.d))
  -include $(OBJ_D_FILES) $(TOOLSOBJ_D_FILES)
endif

.PHONY: all clean clean2 clean-debug clean-release copyfiles \
	debug default dist distclean installer makedirs \
	release targets \
	toolsclean toolsclean2 toolsclean-debug toolsclean-release \
	$(OBJ_D_FILES) $(TOOLSOBJ_D_FILES)
