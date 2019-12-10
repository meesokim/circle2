#
# Makefile
#

CIRCLEHOME = ../..

OBJS	= OneWire.o

libonewire$(RASPPI).a: $(OBJS)
	@echo "  AR    $@"
	@rm -f $@
	@$(AR) cr $@ $(OBJS)

include $(CIRCLEHOME)/Rules.mk
