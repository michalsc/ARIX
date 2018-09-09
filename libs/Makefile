LIBS := exec debug utility dos uuid
OBJDIR := $(BUILD_DIR)/$(CURDIR)

includes:
	@$(foreach dir, $(LIBS), make -C $(dir) includes;)

all: $(LIBS)
$(LIBS):
	@$(MAKE) -C $@

.PHONY: all includes $(LIBS)