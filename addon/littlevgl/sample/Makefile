#
# Makefile
#

CIRCLEHOME = ../../..

OBJS	= main.o kernel.o

LIBS	= ../liblvgl$(RASPPI).a \
	  ../liblv_examples$(RASPPI).a \
	  $(CIRCLEHOME)/lib/usb/libusb$(RASPPI).a \
	  $(CIRCLEHOME)/lib/input/libinput$(RASPPI).a \
	  $(CIRCLEHOME)/lib/fs/libfs$(RASPPI).a \
	  $(CIRCLEHOME)/lib/libcircle$(RASPPI).a

include $(CIRCLEHOME)/Rules.mk

CFLAGS += -Wno-narrowing
