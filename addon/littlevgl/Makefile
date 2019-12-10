#
# Makefile
#

CIRCLEHOME = ../..

LVGL_DIR = .

CSRCS :=
include $(LVGL_DIR)/lvgl/lvgl.mk
OBJS := littlevgl.o $(CSRCS:.c=.o)

CSRCS :=
include $(LVGL_DIR)/lv_examples/lv_examples.mk
EXOBJS := $(CSRCS:.c=.o)

all: liblvgl$(RASPPI).a liblv_examples.a

liblvgl$(RASPPI).a: $(OBJS)
	@echo "  AR    $@"
	@rm -f $@
	@$(AR) cr $@ $(OBJS)

liblv_examples$(RASPPI).a: $(EXOBJS)
	@echo "  AR    $@"
	@rm -f $@
	@$(AR) cr $@ $(EXOBJS)

include $(CIRCLEHOME)/Rules.mk

INCLUDE += -I $(LVGL_DIR)
CFLAGS += -Wno-narrowing
