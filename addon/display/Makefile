#
# Makefile
#

CIRCLEHOME = ../..

OBJS	= hd44780device.o

libdisplay$(RASPPI).a: $(OBJS)
	@echo "  AR    $@"
	@rm -f $@
	@$(AR) cr $@ $(OBJS)

include $(CIRCLEHOME)/Rules.mk
