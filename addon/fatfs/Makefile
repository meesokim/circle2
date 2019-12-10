#
# Makefile
#

CIRCLEHOME = ../..

OBJS	= ff.o diskio.o syscall.o ffunicode.o

libfatfs$(RASPPI).a: $(OBJS)
	@echo "  AR    $@"
	@rm -f $@
	@$(AR) cr $@ $(OBJS)

include $(CIRCLEHOME)/Rules.mk
