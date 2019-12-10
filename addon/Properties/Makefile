#
# Makefile
#

CIRCLEHOME = ../..

OBJS	= properties.o propertiesfile.o propertiesfatfsfile.o propertiesbasefile.o

libproperties$(RASPPI).a: $(OBJS)
	@echo "  AR    $@"
	@rm -f $@
	@$(AR) cr $@ $(OBJS)

include $(CIRCLEHOME)/Rules.mk
