#
# Makefile
#

DEFINE += -DGLFW_INCLUDE_ES2 -D_GLFW_CIRCLE -DIMGUI_IMPL_OPENGL_ES2 -DIMGUI_IMPL_OPENGL_LOADER_GL3W
CIRCLEHOME = ../..
OBJS	= $(shell find . -type f \( -iname "*.c" -o -iname "*.cpp" \) -print | sed -r 's/\.c|\.cpp/\.o/g')
#OBJS = main.o kernel.o triangle2.o
INCLUDE += -I. -Ilibs/glfw/include -Iinclude

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
#	  libSDL2.a
#      libcommon.a libui.a libroms.a 
#      libspc1000.a
#      libcommon.a libui.a libimgui.a libroms.a spc1000.c.obj spc1000-ui.cc.obj 

include ../Rules.mk
