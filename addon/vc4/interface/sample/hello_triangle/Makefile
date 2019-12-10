#
# Makefile
#

CIRCLEHOME = ../../../../..

OBJS	= main.o kernel.o triangle.o

LIBS	= $(CIRCLEHOME)/addon/vc4/interface/bcm_host/libbcm_host$(RASPPI).a \
	  $(CIRCLEHOME)/addon/vc4/interface/khronos/libkhrn_client$(RASPPI).a \
	  $(CIRCLEHOME)/addon/vc4/interface/vmcs_host/libvmcs_host$(RASPPI).a \
	  $(CIRCLEHOME)/addon/vc4/interface/vcos/libvcos$(RASPPI).a \
	  $(CIRCLEHOME)/addon/vc4/vchiq/libvchiq$(RASPPI).a \
	  $(CIRCLEHOME)/addon/linux/liblinuxemu$(RASPPI).a \
	  $(CIRCLEHOME)/lib/sched/libsched$(RASPPI).a \
	  $(CIRCLEHOME)/lib/libcircle$(RASPPI).a

IMAGES	= Djenne_128_128.h Gaudi_128_128.h Lucca_128_128.h

EXTRACLEAN = $(IMAGES) converttool

include $(CIRCLEHOME)/Rules.mk

%.h: %.raw
	@echo "  GEN   $@"
	@./converttool -b $< > $@

triangle.o: $(IMAGES)

$(IMAGES): converttool

converttool: converttool.c
	@echo "  TOOL  $@"
	@gcc -o converttool converttool.c
