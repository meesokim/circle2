#
# Makefile
#

DEFINE += -DGLFW_INCLUDE_ES2 -D_GLFW_CIRCLE -DIMGUI_IMPL_OPENGL_ES2 -DIMGUI_IMPL_OPENGL_LOADER_GL3W -DSOKOL_GLES2 -D__circle__ 
CIRCLEHOME = ../..
SOURCES = $(shell find . -type f \( -iname "*.c" -o -iname "*.cpp" -o -iname "*.cc" \) -print)
OBJS	= $(shell echo $(SOURCES) | sed -r 's/\.c|\.cpp|.cc/\.o/g')
#OBJS = main.o kernel.o triangle2.o
INCLUDE += -I. -Ilibs/glfw/include -Iinclude -Isokol -Isokol/util -Iimgui

LIBS =$(CIRCLEHOME)/addon/vc4/interface/bcm_host/libbcm_host$(RASPPI).a \
	  $(CIRCLEHOME)/addon/vc4/interface/khronos/libkhrn_client$(RASPPI).a \
	  $(CIRCLEHOME)/addon/vc4/interface/vmcs_host/libvmcs_host$(RASPPI).a \
	  $(CIRCLEHOME)/addon/vc4/interface/vcos/libvcos$(RASPPI).a \
	  $(CIRCLEHOME)/addon/vc4/vchiq/libvchiq$(RASPPI).a \
	  $(CIRCLEHOME)/addon/linux/liblinuxemu$(RASPPI).a \
	  $(CIRCLEHOME)/addon/SDCard/libsdcard$(RASPPI).a \
	  $(CIRCLEHOME)/lib/sched/libsched$(RASPPI).a \
	  $(CIRCLEHOME)/lib/usb/libusb$(RASPPI).a \
	  $(CIRCLEHOME)/lib/input/libinput$(RASPPI).a \
	  $(CIRCLEHOME)/lib/fs/libfs$(RASPPI).a \
	  $(CIRCLEHOME)/lib/fs/fat/libfatfs$(RASPPI).a \
      $(CIRCLEHOME)/lib/libcircle$(RASPPI).a \
      $(CIRCLEHOME)/newlib/libc.a \
      $(CIRCLEHOME)/newlib/libm.a \
      $(CIRCLEHOME)/newlib/libcirclenewlib.a

include ../Rules.mk
