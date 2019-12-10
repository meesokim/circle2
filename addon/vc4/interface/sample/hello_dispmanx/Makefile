#
# Makefile
#

CIRCLEHOME = ../../../../..

OBJS	= main.o kernel.o dispmanx.o

LIBS	= $(CIRCLEHOME)/addon/vc4/interface/bcm_host/libbcm_host$(RASPPI).a \
	  $(CIRCLEHOME)/addon/vc4/interface/vmcs_host/libvmcs_host$(RASPPI).a \
	  $(CIRCLEHOME)/addon/vc4/interface/vcos/libvcos$(RASPPI).a \
	  $(CIRCLEHOME)/addon/vc4/vchiq/libvchiq$(RASPPI).a \
	  $(CIRCLEHOME)/addon/linux/liblinuxemu$(RASPPI).a \
	  $(CIRCLEHOME)/lib/sched/libsched$(RASPPI).a \
	  $(CIRCLEHOME)/lib/libcircle$(RASPPI).a

include $(CIRCLEHOME)/Rules.mk
