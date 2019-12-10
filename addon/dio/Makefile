#
# Makefile
#

CIRCLEHOME = ../..

OBJS	= spidio.o

libdio$(RASPPI).a: $(OBJS)
	@echo "  AR    $@"
	@rm -f $@
	@$(AR) cr $@ $(OBJS)

include $(CIRCLEHOME)/Rules.mk
