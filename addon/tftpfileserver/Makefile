#
# Makefile
#

CIRCLEHOME = ../..

OBJS	= tftpfileserver.o

libtftpfileserver$(RASPPI).a: $(OBJS)
	@echo "  AR    $@"
	@rm -f $@
	@$(AR) cr $@ $(OBJS)

include $(CIRCLEHOME)/Rules.mk
