#
# Makefile
#

CIRCLEHOME = ../..

OBJS	= hcsr04.o mpu6050.o bmp180.o

libsensor$(RASPPI).a: $(OBJS)
	@echo "  AR    $@"
	@rm -f $@
	@$(AR) cr $@ $(OBJS)

include $(CIRCLEHOME)/Rules.mk
