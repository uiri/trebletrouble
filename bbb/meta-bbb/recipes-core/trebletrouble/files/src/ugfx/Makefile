# Possible Targets:	all clean Debug cleanDebug Release cleanRelease

# /home/vagrant/trebletrouble/bbb/build/tmp/sysroot-destdir/x86_64-linux/gcc-cross-arm/5.3.0-r0/image/

OPT_MAKE_LIB = yes
HOME=/home/$(shell whoami)

#TRGT = $(HOME)/trebletrouble/bbb/build/tmp/sysroots/x86_64-linux/usr/bin/arm-poky-linux-gnueabi/arm-poky-linux-gnueabi-
TRGT = arm-poky-linux-gnueabi-
XCC = $(TRGT)gcc
CPPC = $(TRGT)g++

XLD = $(TRGT)gcc
XCP = $(TRGT)objcopy
XAS = $(TRGT)gcc -x assembler-with-cpp
XAR = $(TRGT)ar
XOD = $(TRGT)objdump
XSZ = $(TRGT)size
XHEX = $(CP) -0 ihex
XBIN = $(CP) -0 binary


##############################################################################################
# Settings
#

# General settings
	# See $(GFXLIB)/tools/gmake_scripts/readme.txt for the list of variables
	OPT_OS					= linux
	OPT_LINK_OPTIMIZE		= 
	# Change this next setting (or add the explicit compiler flags) if you are not compiling for x86 linux
	OPT_CPU					= armv6

# uGFX settings
	# See $(GFXLIB)/tools/gmake_scripts/library_ugfx.mk for the list of variables
	GFXLIB					= resources/ugfx
	GFXBOARD				= Linux-Framebuffer-Touch
	GFXDEMO					= 

# Linux settings
	# See $(GFXLIB)/tools/gmake_scripts/os_linux.mk for the list of variables

##############################################################################################
# Set these for your project
#

ARCH     =
SRCFLAGS = -ggdb -O0
CFLAGS   =
CXXFLAGS =
ASFLAGS  =
LDFLAGS  =

SRC      = 
OBJS     =
DEFS     = 
LIBS     = -lm
INCPATH  = $(HOME)/trebletrouble/bbb/build/tmp/sysroots/beaglebone/usr/include
LIBPATH  = $(HOME)/trebletrouble/bbb/build/tmp/sysroots/beaglebone/usr/include/glibc-locale-internal-cortexa8hf-neon-poky-linux-gnueabi/usr/lib

##############################################################################################
# These should be at the end
#

include $(GFXLIB)/tools/gmake_scripts/library_ugfx.mk
include $(GFXLIB)/tools/gmake_scripts/os_$(OPT_OS).mk
include $(GFXLIB)/tools/gmake_scripts/compiler_gcc.mk
# *** EOF ***
