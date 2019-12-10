#
# Makefile
#

CIRCLEHOME = ../..
OBJS	= main.o kernel.o triangle2.o

LIBS =$(CIRCLEHOME)/addon/vc4/interface/bcm_host/libbcm_host$(RASPPI).a \
	  $(CIRCLEHOME)/addon/vc4/interface/khronos/libkhrn_client$(RASPPI).a \
	  $(CIRCLEHOME)/addon/vc4/interface/vmcs_host/libvmcs_host$(RASPPI).a \
	  $(CIRCLEHOME)/addon/vc4/interface/vcos/libvcos$(RASPPI).a \
	  $(CIRCLEHOME)/addon/vc4/vchiq/libvchiq$(RASPPI).a \
	  $(CIRCLEHOME)/addon/linux/liblinuxemu$(RASPPI).a \
	  $(CIRCLEHOME)/lib/sched/libsched$(RASPPI).a \
	  $(CIRCLEHOME)/lib/usb/libusb$(RASPPI).a \
	  $(CIRCLEHOME)/lib/input/libinput$(RASPPI).a \
	  $(CIRCLEHOME)/lib/fs/libfs$(RASPPI).a \
	  $(CIRCLEHOME)/lib/fs/fat/libfatfs$(RASPPI).a \
      $(CIRCLEHOME)/lib/libcircle$(RASPPI).a \
      $(CIRCLEHOME)/newlib/libc.a \
      $(CIRCLEHOME)/newlib/libm.a \
      $(CIRCLEHOME)/newlib/libcirclenewlib.a \
      libcommon.a libui.a libimgui.a libroms.a \
      libspc1000.a
#      libcommon.a libui.a libimgui.a libroms.a spc1000.c.obj spc1000-ui.cc.obj libc.a libg.a libm.a libcirclenewlib.a

include ../Rules.mk
