#
# Makefile
#

#USE_FATFS = 1

CIRCLEHOME = ../../..

ifeq ($(strip $(USE_FATFS)),1)
CFLAGS += -DUSE_FATFS
endif

OBJS	= main.o kernel.o

ifeq ($(strip $(USE_FATFS)),1)
LIBS	= ../libproperties$(RASPPI).a \
	  $(CIRCLEHOME)/addon/SDCard/libsdcard$(RASPPI).a \
	  $(CIRCLEHOME)/addon/fatfs/libfatfs$(RASPPI).a \
	  $(CIRCLEHOME)/lib/fs/libfs$(RASPPI).a \
	  $(CIRCLEHOME)/lib/libcircle$(RASPPI).a
else
LIBS	= ../libproperties$(RASPPI).a \
	  $(CIRCLEHOME)/addon/SDCard/libsdcard$(RASPPI).a \
	  $(CIRCLEHOME)/lib/fs/fat/libfatfs$(RASPPI).a \
	  $(CIRCLEHOME)/lib/fs/libfs$(RASPPI).a \
	  $(CIRCLEHOME)/lib/libcircle$(RASPPI).a
endif

include $(CIRCLEHOME)/sample/Rules.mk
