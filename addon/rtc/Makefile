#
# Makefile
#

CIRCLEHOME = ../..

OBJS	= mcp7941x.o

librtc$(RASPPI).a: $(OBJS)
	@echo "  AR    $@"
	@rm -f $@
	@$(AR) cr $@ $(OBJS)

include $(CIRCLEHOME)/Rules.mk
