#
# Makefile
#

CIRCLEHOME = ../../..

OBJS	= myclass.o

libtempl$(RASPPI).a: $(OBJS)
	@echo "  AR    $@"
	@rm -f $@
	@$(AR) cr $@ $(OBJS)

include $(CIRCLEHOME)/Rules.mk
